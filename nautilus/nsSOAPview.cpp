// --------------------------------------------------------------------------// NSSOAPVIEW.CPP		Document/Vues SOAP
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Novembre/Décembre 2001
// source : Fabrice LE PERRU -- nsepisodview.cpp - Aout 2001
//          -- source : Rémi SPAAK - Mai 1997
// --------------------------------------------------------------------------

#define __NS_SOAPVIEW_CPP__

#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

#include <owl/uihelper.h>

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include <owl/button.h>

#include "partage\nsdivfct.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "partage\ns_timer.h"
#include "nautilus\ns_html.h"
#include "nautilus\nscompub.h"		// pour les formules
#include "nautilus\nsresour.h"      // pour AutoWordDialog
#include "nautilus\nsvisual.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nscsdoc.h"

#include "nautilus\nsepicap.h"
#include "nautilus\nsSOAPview.h"

#include "nssavoir\nspatho.h"
#include "nssavoir\nspathor.h"
#include "nssavoir\nsgraphe.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsmdiframe.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nspanesplitter.h"
#include "nsbb\nsfltbtnga.h"

#include "nautilus\nsSOAP.rh"
// #include "nautilus\nsSOAP.rc"
#include "nautilus\nautilus.rh"

#include "nsepisod\eptables.h"

// --------------------------------------------------------------------------
//
//					METHODES DE NSSOAP Document / Vue
//
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
//
// Class NSSOAPDocument
//
// --------------------------------------------------------------------------

// Constructeur

NSSOAPDocument::NSSOAPDocument(TDocument* parent, NSContexte* pCtx, NSDocumentInfo* pDocInf)
               :TDocument(parent), NSRoot(pCtx)
{
try
{
  SetTitle("Organisation des éléments de rencontre") ;

  pEpisodus    = pContexte->getEpisodus() ;
  pDocInfo     = pDocInf ;

  bDragAndDrop = false ;

  nProblems    = 0 ;

  if (pEpisodus)
  {
    pBasketList = pEpisodus->pBasketList ;
    pPrincipes  = pEpisodus->pPrincipes ;

    pEpisodus->SetEpisoState(NSEpisodus::stateSOAPing) ;
  }
  else
  {
    pBasketList = 0 ;
    pPrincipes  = 0 ;
  }

  nCategory = 0 ;
  if (false == pPrincipes->empty())
    for (PrinciplesIter Iter = pPrincipes->begin(); pPrincipes->end() != Iter ; Iter++)
      nCategory++ ;

  pTankObjectList = new SOAPObjectArray() ;

  // TMyApp* pApp = pContexte->getSuperviseur()->pNSApplication;

  creerChroniques() ;
}
catch (...)
{
  erreur("Exception NSSOAPDocument ctor.", standardError, 0) ;
}
}

NSSOAPDocument::~NSSOAPDocument(){
  delete pTankObjectList ;
}

void
NSSOAPDocument::addToTank(SOAPObject* pObject)
{
	if (NULL == pObject)
		return ;

	donneChamp(pObject) ;

	pTankObjectList->push_back(pObject) ;

  NSSOAPTankView* pTankView = pEpisodus->pSOAPTank ;
  if (NULL == pTankView)
  	return ;

  pTankView->addObject(pObject) ;

  if (pEpisodus->bAutoFlocule)
  	pTankView->floculer() ;

  pTankView->Invalidate() ;
}

bool
NSSOAPDocument::floculer()
{
try
{
  //
  // Si le réservoir n'existe pas ou est vide, on sort
  // If tank doesn't exist or is empty, immediate return
  //
  if ((NULL == pTankObjectList) || (pTankObjectList->empty()))
    return false ;

  NSSOAPView*     pView     = pEpisodus->pSOAPView ;
  NSSOAPTankView* pTankView = pEpisodus->pSOAPTank ;

  string sCaseSens ;

  //
  // Première étape, on passe en revue les diagnostics pour créer de nouveaux panniers
  // At first, we look for diagnosis in order to create new baskets
  //
  ArraySOAPObjectIter iObject = pTankObjectList->begin() ;

  for ( ; pTankObjectList->end() != iObject ; )
  {
    bool bMoved = false ;
    SOAPObject* pObject = *iObject ;

    if (pObject->estFloculable())
    {
      //
      // On trouve la case qui correspond à ce code
      //
      ClassificationPrinciple* pCase = pPrincipes->trouvePrincipe(pObject->sCase) ;

      if ((pCase) && (100 == pObject->iConfidence))
      {
        SOAPBasket* pBasket = 0 ;
        pContexte->getDico()->donneCodeSens(&(pObject->sCase), &sCaseSens) ;

        //
        // Si c'est un code 'A', on crée un nouveau panier
        // Assessementcode ('A'), create a new basket
        //
        if (string("0SOA4") == sCaseSens)
        {
          pBasket = new SOAPBasket(pPrincipes) ;
          addBasket(pBasket) ;
        }

        if (pBasket)
        {
          SOAPObjectArray* pCaseDuPanier = pBasket->trouveCase(pCase) ;
          if (pCaseDuPanier)
          {
            pCaseDuPanier->push_back(pObject) ;
            pTankObjectList->erase(iObject) ;
            bMoved = true ;

            // Mise à jour de la fenêtre réservoir
            //
            if ((pTankView) && (pTankView->pMyListWindow))
              pTankView->pMyListWindow->delObject(pObject) ;

            // Mise à jour de le fenêtre de destination
            //
            if (pView)
            {
              SOAPListWinArray* pList = pView->getLists() ;
              if (pList)
              {
                NSSOAPListWindow* pLW = pList->trouveList(pBasket, pCase) ;
                if (pLW)
                {
                  pLW->addObject(pObject) ;
                  pLW->Invalidate() ;
                }
              }
            }
          }
        }
      }
    }
    if (!bMoved)
      iObject++ ;
  }

  if (pTankObjectList->empty())
    return true ;

  //
  // Seconde étape, on distribue les autres éléments dans leur panier
  // Then, we put other elements in the proper basket
  //
  iObject = pTankObjectList->begin() ;

  for ( ; pTankObjectList->end() != iObject ; )
  {
    bool bMoved = false ;
    SOAPObject* pObject = *iObject ;

    if (pObject->estFloculable())
    {
      //
      // On trouve la case qui correspond à ce code
      //
      ClassificationPrinciple* pCase = pPrincipes->trouvePrincipe(pObject->sCase) ;

      if ((pCase) && (100 == pObject->iConfidence))
      {
        SOAPBasket* pBasket = 0 ;

        pContexte->getDico()->donneCodeSens(&(pObject->sCase), &sCaseSens) ;

        //
        // Sinon on cherche le panier dont le 'A' appartient au même chapitre
        //
        if ((string("") != pObject->donneChapitre()) && (false == pBasketList->empty()))
        {
          SOAPBasketArrayIter baskIter = pBasketList->begin() ;
          for (; pBasketList->end() != baskIter ; baskIter++)
          if ((*baskIter)->donneChapitre() == pObject->donneChapitre())
            pBasket = *baskIter ;
        }

        if (pBasket)
        {
          SOAPObjectArray* pCaseDuPanier = pBasket->trouveCase(pCase) ;
          if (pCaseDuPanier)
          {
            pCaseDuPanier->push_back(pObject) ;
            pTankObjectList->erase(iObject) ;
            bMoved = true ;

            // Mise à jour de la fenêtre réservoir
            //
            if ((pTankView) && (pTankView->pMyListWindow))
              pTankView->pMyListWindow->delObject(pObject) ;

            // Mise à jour de le fenêtre de destination
            //
            if (pView)
            {
              SOAPListWinArray* pList = pView->getLists() ;
              if (pList)
              {
                NSSOAPListWindow* pLW = pList->trouveList(pBasket, pCase) ;
                if (pLW)
                {
                  pLW->addObject(pObject) ;
                  pLW->Invalidate() ;
                }
              }
            }
          }
        }
      }
    }
    if (!bMoved)
      iObject++ ;
  }
  return true ;
}
catch (...)
{
  erreur("Exception NSSOAPDocument floculer.", standardError, 0) ;
  return false ;
}
}

// So far, this function is limited to the healthcare frame
//
void
NSSOAPDocument::creerChroniques()
{
  NSPatientChoisi* pPat = pContexte->getPatient() ;
  if (((NSPatientChoisi*) NULL == pPat) || (NULL == pPat->getLdvDocument()))
		return ;

  ArrayConcern* pConcerns = pPat->getLdvDocument()->getConcerns(ldvframeHealth) ;
  if (((ArrayConcern*) NULL == pConcerns) || pConcerns->empty())
  	return ;

  // Classification utilisée pour coder l'Assesment ?
  // Which classification is used for Assesment coding ?

  ClassificationPrinciple* pPrincA = pPrincipes->trouvePrincipe(string("0SOA41")) ;
  if (pPrincA == 0)
  	return ;
  string sClassifSens = pPrincA->sClassification ;

  string sLang = pContexte->getUserLanguage() ;

  string sDate   = string("") ;
  string sCode   = string("") ;
  string sTexte  = string("") ;
  string sChemin = string("") ;
  //string sClassification =

	for (ArrayConcernIter it = pConcerns->begin() ; pConcerns->end() != it ; it++)
	{
  	//
    // Préoccupation chronique
    //
    if ((*it)->_tDateFermeture.estNoLimit())
    {
    	// Codage (le plus récent) ?
      VecteurString vResult ;
			NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
      pGraphe->TousLesVrais((*it)->_sReference, NSRootLink::problemContactElement, &vResult, "ENVERS") ;

      if (false == vResult.empty())
      {
      	sDate  = string("") ;
        sCode  = string("") ;
        sTexte = string("") ;

        EquiItemIter iterLiens = vResult.begin() ;
        for ( ; vResult.end() != iterLiens ; iterLiens++)
        {
          NSPatPathoArray PPt(pContexte->getSuperviseur()) ;
          string          sDateDoc = string("") ;
          //
          // On récupère l'arbre
          //
          string sDocId = string(**iterLiens, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

          if (pPat->getDocHis()->donneArbre(sDocId, &PPt, sDateDoc))
          {
            if (sDateDoc >= sDate)
            {
              sDate = sDateDoc ;

              PatPathoIter pptIt = PPt.ChercherNoeud(**iterLiens) ;

              if ((NULL != pptIt) && (PPt.end() != pptIt))
              {
                string sLexSens ;
                string sLexique = (*pptIt)->getLexique() ;
                pContexte->getDico()->donneCodeSens(&sLexique, &sLexSens) ;

                if (sLexSens == sClassifSens)
                {
                  sCode   = (*pptIt)->getComplement() ;
                  sChemin = PPt.donneCheminItem(pptIt) ;

                  // On prend pour libellé celui de son père
                  PatPathoIter pptItPere = PPt.ChercherPere(pptIt) ;
                  if ((NULL != pptItPere) && (PPt.end() != pptItPere))
                  {
                    string sLexiPere = (*pptItPere)->getLexique() ;
                    if (string("£?????") == sLexiPere)
                      sTexte = (*pptItPere)->getTexteLibre() ;
                    else
                      pContexte->getDico()->donneLibelle(sLang, &sLexiPere, &sTexte) ;
                  }
                }
                //
                // Si ce n'est pas lui, c'est donc un de ses fils
                //
                else
                {
                  // Si le noeud ne contient pas le codage, il contient le texte
                  if (string("£?????") == sLexique)
                    sTexte = (*pptIt)->getTexteLibre() ;
                  else
                    pContexte->getDico()->donneLibelle(sLang, &sLexique, &sTexte) ;
                  // Et sert de chemin de référence
                  sChemin = PPt.donneCheminItem(pptIt) ;

                                    /* PatPathoIter pptItPere = pPPt->ChercherPere(pptIt);
                                    sChemin = "";
                                    while ((pptItPere != NULL) && (pptItPere != pPPt->end()))
                                    {
                                        if (sChemin != "")
                                            sChemin = string("/") + sChemin;
                                        sChemin = string((*pptItPere)->pDonnees->lexique) + sChemin;

                                        pptItPere = pPPt->ChercherPere(pptItPere);
                                    } */

                  PatPathoIter pptItFils = PPt.ChercherPremierFils(pptIt) ;

                  while ((string("") == sCode) && (NULL != pptItFils) && (PPt.end() != pptItFils))
                  {
                    sLexique = (*pptItFils)->getLexique() ;
                    pContexte->getDico()->donneCodeSens(&sLexique, &sLexSens) ;

                    if (sLexSens == sClassifSens)
                      sCode = (*pptItFils)->getComplement() ;

                    pptItFils = PPt.ChercherFrereSuivant(pptItFils) ;
                  }
                }
              }
            }
          }
        }
        if (string("") != sCode)
        {
#ifndef _EXT_CAPTURE
          NSCapture* pNewCaptElemnt = new NSCapture(pContexte, sChemin, sTexte, FromUnknown, sClassifSens) ;
#else
          NSCapture* pNewCaptElemnt = new NSCapture(sChemin, sTexte, NSCapture::FromUnknown, sClassifSens) ;
#endif
          SOAPObject* pNewObject = new SOAPObject(sTexte, sCode, sClassifSens, 100, "");

          pNewObject->pCaptElemnt = pNewCaptElemnt ;
          pContexte->getEpisodus()->CaptureArray.push_back(pNewCaptElemnt) ;

          //
          // Pour éviter que le médecin ne laisse en place des
          // données de problèmes qu'il n'a pas évoqués, on
          // considère ces éléments comme non floculables
          //
          // To avoid the MD to let datas from non really studied
          // problems, this elements are considered as non floculable
          //
          pNewObject->setFloculable(false) ;

          addToTank(pNewObject) ;
        }
      }
    }
  }
}

boolNSSOAPDocument::enregistrer()
{
try
{
  //
  // Si les informations n'ont jamais été enregistrées, créer un document CS
  // If datas have never been stored, we must first create a CS document
  //
  if (NULL == pDocInfo)
    createCSDocument() ;
    
  return true ;
}
catch (...)
{
  erreur("Exception NSSOAPDocument enregistrer.", standardError, 0) ;
  return false ;
}
}

boolNSSOAPDocument::createCSDocument()
{
try
{
  //
  // Si le document est vide, on sort
  // If document is empty, immediate return
  //
	if (((NULL == pTankObjectList) || (pTankObjectList->empty())) &&
      ((NULL == pBasketList) || (pBasketList->empty())))
  	return false ;

	bool bNeedToSave = false ;

	//
	// On vérifie que chaque panier possède un A
	// We check that each basket has a A
	//
	if ((pBasketList) && (false == pBasketList->empty()))
	{
  	SOAPBasketArrayIter baskIter = pBasketList->begin() ;
    for ( ; pBasketList->end() != baskIter ; baskIter++)
    {
    	SOAPBasket* pBasket = *baskIter ;
      if (false == pBasket->empty())
      {
      	SOAPBasketIter objsIter = pBasket->begin() ;
        for ( ; pBasket->end() != objsIter ; objsIter++)
        {
        	SOAPObjectArray* pObjs = *objsIter ;
          if (pObjs->empty())
          {
          	string sCaseSens ;
            pContexte->getDico()->donneCodeSens(&(pObjs->pPrincipe->sCase), &sCaseSens) ;
            //
            // Si c'est un code 'A', on crée un nouveau panier
            //
            if (string("0SOA4") == sCaseSens)
            {
            	erreur("Chaque pannier doit avoir un code dans la case diagnostic.", standardError, 0) ;
              return false ;
            }
          }
        }
      }
    }
  }
  //
  // ATTENTION, on pourrait imaginer que les éléments sont hétérogénes,
  // et qu'il faille créer plusieurs patpatho (plusieurs documents)
  //

	NSCSDocument CSDoc(0, pContexte, string("GCONS1")) ;
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

  NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
  CSDoc.initFromPatPatho(&PatPatho) ;

  PatPathoIter pptIter = PatPatho.begin() ;

  //
  // Ajout à la patpatho des éléments des paniers
  // Objects from baskets are added to the patpatho
  //
  if ((pBasketList) && (false == pBasketList->empty()))
  {
  	SOAPBasketArrayIter baskIter = pBasketList->begin() ;
    for ( ; pBasketList->end() != baskIter ; baskIter++)
    {
    	SOAPBasket* pBasket = *baskIter ;
      if (false == pBasket->empty())
      {
      	SOAPBasketIter objsIter = pBasket->begin() ;
        for ( ; pBasket->end() != objsIter ; objsIter++)
        {
        	SOAPObjectArray* pObjs = *objsIter ;
          if (false == pObjs->empty())
          {
          	ArraySOAPObjectIter Iter = pObjs->begin() ;
            for ( ; pObjs->end() != Iter ; Iter++)
            {
            	//
              // Elément nouvellement capturé : création d'un noeud
              //
              if ((*Iter)->pCaptElemnt)
              {
              	bNeedToSave = true ;

                string sChemin = (*Iter)->pCaptElemnt->getChemin() ;

                Message Msg ;
                Msg.SetLexique("£?????") ;
                Msg.SetTexteLibre((*Iter)->sTexteInitial) ;
                Msg.SetVisible("1") ;
                PatPatho.AjouterChemin(sChemin, &Msg, false, "/") ;

                // On ajoute les données classificatoires
                //
                // Positionnement sur la ligne "mère"
                //
                string sLocalisation ;
                pptIter = PatPatho.ChaineDansPatpatho(sChemin, &sLocalisation, "/") ;
                if ((pptIter) && (PatPatho.end() != pptIter) &&
                                    (sLocalisation == sChemin))
                {
                	int colonneMere = (*pptIter)->getColonne() ;
                  //
                  // On cherche l'élément lui même
                  //
                  bool bTrouve = false ;
                  pptIter++ ;
                  for ( ; (PatPatho.end() != pptIter) &&
                         ((*pptIter)->getColonne() > colonneMere) &&
                         (!bTrouve); )
                  {
                  	if (((*pptIter)->getColonne() == colonneMere+1) &&
                        ((*pptIter)->getLexique() == string("£?????")) &&
                        ((*pptIter)->getTexteLibre() == (*Iter)->sTexteInitial))
                    {
                    	pptIter++ ;
                      string sClassifCode = (*Iter)->sClassification + "1" ;
                      Message CodeMsg ;
                      CodeMsg.SetLexique(sClassifCode) ;
                      CodeMsg.SetComplement((*Iter)->sCode) ;
                      CodeMsg.SetVisible("1") ;
                      int iDecalLigne ;
                      if (PatPatho.end() != pptIter)
                      	iDecalLigne = 0 ;
                      else
                      	iDecalLigne = 1 ;
                      PatPatho.ajoutePatho(pptIter,
                                           sClassifCode,
                                           &CodeMsg,
                                           colonneMere+2,
                                           iDecalLigne,
                                           true) ;
                      bTrouve = true ;
                    }
                    if (false == bTrouve)
                    	pptIter++ ;
                  }
                }
              }
              //
              // Elément qui existe déjà en temps que noeud,
              // on corrige éventuellement
              //
              else
              {
              }
            }
          }
        }
      }
    }
  }
  //
  // Puis les objets du réservoir
  //
  /*
  if ((pTankObjectList) && (!(pTankObjectList->empty())))
  {
      ArraySOAPObjectIter Iter = pTankObjectList->begin();
      for (; Iter != pTankObjectList->end(); Iter++)
      {
          if ((*Iter)->pCaptElemnt)
          {
              string sChemin = (*Iter)->pCaptElemnt->sChemin;

              Message* pMsg = new Message("");
              pMsg->SetLexique("£?????");
              pMsg->SetTexteLibre((*Iter)->sTexteInitial);
              pMsg->SetVisible("1");
              pPatPatho->AjouterChemin(sChemin, pMsg, false, "/");
              delete pMsg;
          }
      }
  } */


  bool bSaved = true ;

  if (bNeedToSave)
  {
  	//
    // On rend visible tous les noeuds
    // Having all nodes visible
    //
    if (false == PatPatho.empty())
    	for (PatPathoIter i = PatPatho.begin() ; PatPatho.end() != i ; i++)
      	(*i)->setVisible("1") ;

    //
    // Enregistrement du document
    // Saving the document
    //
    CSDoc.setPatPatho(&PatPatho) ;

    bSaved = CSDoc.enregistre(false /* verbose */, false /* soaping */) ;

    if (false == bSaved)
      return bSaved ;
	}

  //
  // Si le document a été enregistré, on connecte les objets aux noeuds
  // If document was saved, we can connect the objects to the nodes
  //
  pDocInfo = CSDoc._pDocInfo ;

  bool bReload = ((NSNoyauDocument*)&CSDoc)->chargePatPatho() ;

  if (false == bReload)
  	return false ;

  CSDoc.initFromPatPatho(&PatPatho) ;
  pptIter = PatPatho.begin() ;

  //
  // Eléments des paniers : connexion aux noeuds et établissement des liens
  // Baskets objects : connecting to the nodes and linking each other
  //
  if ((pBasketList) && (false == pBasketList->empty()))
  {
  	SOAPBasketArrayIter baskIter = pBasketList->begin() ;
    for ( ; pBasketList->end() != baskIter ; baskIter++)
    {
    	SOAPBasket* pBasket = *baskIter ;
      if (false == pBasket->empty())
      {
      	//
        // Etape 1 : connexion des noeuds
        // Step 1 : connecting the nodes
        //
        SOAPBasketIter objsIter = pBasket->begin() ;
        for ( ; pBasket->end() != objsIter ; objsIter++)
        {
        	SOAPObjectArray* pObjs = *objsIter ;
          if (false == pObjs->empty())
          {
          	ArraySOAPObjectIter Iter = pObjs->begin() ;
            for ( ; pObjs->end() != Iter ; Iter++)
            {
            	string sChemin ;
              if ((*Iter)->pCaptElemnt)
              	sChemin = (*Iter)->pCaptElemnt->getChemin() ;
              else
              	sChemin = "" ;
              string sLocalisation ;

            	//
              // Positionnement sur la ligne "mère"
              //
              pptIter = PatPatho.ChaineDansPatpatho(sChemin, &sLocalisation, "/") ;
              if ((pptIter) && (PatPatho.end() != pptIter) &&
                                    (sLocalisation == sChemin))
              {
              	int colonneMere = (*pptIter)->getColonne() ;
                //
                // On cherche l'élément lui même
                //
                bool bTrouve = false ;
                pptIter++ ;
                for (; (PatPatho.end() != pptIter) &&
                       ((*pptIter)->getColonne() > colonneMere) &&
                       (false == bTrouve) ;
                              pptIter++)
                {
                	if (((*pptIter)->getColonne() == colonneMere+1) &&
                      ((*pptIter)->getLexique() == string("£?????")) &&
                      ((*pptIter)->getTexteLibre() == (*Iter)->sTexteInitial))
                  {
                  	(*Iter)->sNoeud = (*pptIter)->getNode() ;
                    (*Iter)->pCaptElemnt = 0 ;
                    bTrouve = true ;
                  }
                }
              }
            }
          }
        }
        //
        // Etape 2 : Etablissement des liens entre noeuds
        // Step 2 : Linking the nodes
        //

        // Recherche du compartiment 'A'
        // Looking for the 'A' slot
        ClassificationPrinciple* pPrA = pBasket->pPrincipes->trouvePrincipe("0SOA41") ;
        if (pPrA)
        {
        	SOAPObjectArray* pSlotA = pBasket->trouveCase(pPrA) ;
          if (pSlotA && (false == pSlotA->empty()))
          {
          	SOAPObject* pObjectA = *(pSlotA->begin()) ;

            if (string("") != pObjectA->sNoeud)
            {
            	objsIter = pBasket->begin();
              for (; pBasket->end() != objsIter ; objsIter++)
              {
              	SOAPObjectArray* pObjs = *objsIter ;
                if ((false == pObjs->empty()) && (pObjs->pPrincipe != pPrA))
                {
                	ArraySOAPObjectIter Iter = pObjs->begin() ;
                  for ( ; pObjs->end() != Iter ; Iter++)
                  	if (string("") != (*Iter)->sNoeud)
                    	pGraphe->etablirLien(pObjectA->sNoeud, NSRootLink::soapRelatedTo, (*Iter)->sNoeud) ;
                }
              }
            }
          }
        }
      }
    }
  }
  //
  // Puis les objets du réservoir
  //
  /*
  if ((pTankObjectList) && (!(pTankObjectList->empty())))
  {
      ArraySOAPObjectIter Iter = pTankObjectList->begin();
      for (; Iter != pTankObjectList->end(); Iter++)
      {
          string sChemin = (*Iter)->pCaptElemnt->sChemin;

          Message* pMsg = new Message("");
          pMsg->SetLexique("£?????");
          pMsg->SetTexteLibre((*Iter)->sTexteInitial);
          pPatPatho->AjouterChemin(sChemin, pMsg, false, "/");
          delete pMsg;
      }
  } */

	return bSaved ;

    /*
    for (int i = 0; i < CLASSIF_PRINCIPLE_NBMAXSLOT, i++)    {
        string sSlotCode = donneCaseLexique(i);

        //
        // On cherche s'il existe un objet correspondant à ce slot
        // Looking for an object for that slot
        //
        string sGenuineCode = "";
        if (!(BasketList->empty()))
        {
            SOAPBasketArrayIter baskIter = pBasketList->begin();
            for (; baskIter != pBasketList->end(); baskIter++)
            {
                SOAPBasket* pBasket = *baskIter;
                if (pBasket->empty())
                {
                    SOAPBasketIter objsIter = pBasket->begin();
                    for (; (objsIter != pBasket->end()) && (sGenuineCode == ""); objsIter++)
                    {
                        SOAPObjectArray* pObjs = *objsIter;
                        if (pObjs->pPrincipe->sCase == sSlotCode)
                        {
                            if (!(pObjs->empty()))
                            {
                                ArraySOAPObjectIter Iter = pObjs->begin();
                                if (sGenuineCode == "")
                                {
                                    sGenuineCode = (*Iter)->sCase;
                                    pCaptElemnt
                                AjouterChemin(string sChaine, Message* pMessage, bool bEcraser = true, string separateur = "|");
                            }
                        }
                    }
                }
            }
        }
        if ((sGenuineCode == "") && (!(pTankObjectList->empty())))
        {
        }

    }


    //
    // Fabrication d'une pathpatho
    //
    SOAPBasketArrayIter baskIter = pBasketList->begin();
        for (; baskIter != pBasketList->end();)

    NSSOAPView*     pView     = pContexte->getSuperviseur()->pEpisodus->pSOAPView;
    NSSOAPTankView* pTankView = pContexte->getSuperviseur()->pEpisodus->pSOAPTank;

    string sCaseSens;
    */

}
catch (...)
{
	erreur("Exception NSSOAPDocument createCSDocument.", standardError, 0) ;
	return false ;
}
}

voidNSSOAPDocument::addBasket(SOAPBasket* pBasket)
{
  if (NULL == pBasket)
    return ;

  pBasketList->push_back(pBasket) ;

  //
  // On prévient la vue
  //
  NSSOAPView* pView = pEpisodus->pSOAPView ;
  if (pView)
    pView->addColumn(pBasket) ;
}

void
NSSOAPDocument::removeBasket(SOAPBasket* pBasket)
{
  if (NULL == pBasket)
    return ;

  // On trouve la vue
  //
  NSSOAPView* pView = pEpisodus->pSOAPView ;

  //
  // On supprime les ListWindows
  //
  if (pView)
    pView->delColumn(pBasket) ;

  if (false == pBasketList->empty())
  {
    SOAPBasketArrayIter baskIter = pBasketList->begin() ;
    for (; pBasketList->end() != baskIter ; )
    {
      if ((*baskIter) == pBasket)
        pBasketList->erase(baskIter) ;
      else
        baskIter++ ;
    }
  }
  delete pBasket ;
}

boolNSSOAPDocument::acceptDragDrop(SOAPObject* pObject2Drop)
{
try
{
  if ((NULL == pDstDragDrop) || (NULL == pObject2Drop))
    return false ;

  //
  // La ListWindow de destination est-elle au sein de la NSSOAPView ?
  //
  NSSOAPView* pview = TYPESAFE_DOWNCAST(pDstDragDrop->pParentWindow, NSSOAPView) ;
  if (pview)
  {
    //
    // On ne peut déposer qu'un élément "vert"
    //
    if (100 != pObject2Drop->iConfidence)
    {
      erreur("Seuls les éléments codés peuvent figurer dans les paniers.", standardError, 0) ;
      return false ;
    }
    SOAPBasket* pBasket ;

    //
    // Quelle est la bonne case pour cet objet ?
    //
    ClassificationPrinciple* pPrin = pPrincipes->trouvePrincipe(pObject2Drop->sCase) ;

    //
    // Si la case est fictive, il faut créer un nouveau panier
    //
    if (NULL == pDstDragDrop->pObjectList)
    {
      pBasket = new SOAPBasket(pPrincipes) ;
      addBasket(pBasket) ;
    }
    else
      pBasket = pDstDragDrop->pBasket ;

    //
    // On choisit au sein du panier la case qui correspond à l'élément
    //
    SOAPListWinArray* pList = pview->getLists() ;
    NSSOAPListWindow* pLW = pList->trouveList(pBasket, pPrin) ;

    if (NULL == pLW)
      return false ;

    //    // Si cette case ne doit contenir qu'un seul élément et qu'elle est    // déjà occupée
    //
    if ((pPrin->bUnique) && (false == pLW->pObjectList->empty()))
    {
      erreur("Un panier ne peut contenir qu'un seul élément de ce type.", standardError, 0) ;
      return false ;
    }

    pDstDragDrop = pLW ;
  }

  //
  // Rien à faire si on retombe sur la même case
  //
  if (pDstDragDrop == pSrcDragDrop)
    return false ;

  pDstDragDrop->pObjectList->push_back(pObject2Drop) ;
  pObject2Drop->iOldListParam = pObject2Drop->iListParam ;

  if (pDstDragDrop->pObjectList)
    pObject2Drop->iListParam = pDstDragDrop->pObjectList->donneNextListParam() ;
  else
    pObject2Drop->iListParam = -1 ;

  string sLib = pObject2Drop->donneLibelle() ;
  TListWindItem Item(sLib.c_str()) ;
  Item.SetText(sLib.c_str()) ;
  Item.SetImageIndex(pObject2Drop->iImageIndex) ;
  Item.SetItemData(pObject2Drop->iListParam) ;
  pDstDragDrop->AddItem(Item) ;

  return (true) ;  // temporaire
}
catch (...)
{
  erreur("Exception NSSOAPDocument::acceptDragDrop.", standardError, 0) ;
  return false ;
}
}

bool
NSSOAPDocument::deleteItemDragDrop(SOAPObject *pObject2Drag)
{
    // efface la donnée de sa liste si la liste le veut bien...
    if (!(pSrcDragDrop->pObjectList) || (pSrcDragDrop->pObjectList->empty()))
        return false ;

    // On supprime l'item de la ListWindow
    //
    int index = pSrcDragDrop->getIndex(pObject2Drag->iOldListParam);
    if (index == -1)
        return false;

    pSrcDragDrop->DeleteAnItem(index);    pObject2Drag->iOldListParam = -1;
    //
    // On supprime l'objet de l'array
    //
    ArraySOAPObjectIter myIter;
    for (myIter = pSrcDragDrop->pObjectList->begin();
                myIter != pSrcDragDrop->pObjectList->end(); myIter++)
    {
        if (*myIter == pObject2Drag)
        {
            pSrcDragDrop->pObjectList->erase(myIter);

            if ((pSrcDragDrop->pBasket) && (pSrcDragDrop->pBasket->isEmpty()))
                removeBasket(pSrcDragDrop->pBasket);

            /*if (pDocSOAP->pSrcDragDrop->iColumn == pDocSOAP->GetNProblems() - 1)
                    if (pContexte->getSuperviseur()->pEpisodus->pSOAPView->noItem(pDocSOAP->pSrcDragDrop->iColumn))
                        pContexte->getSuperviseur()->pEpisodus->pSOAPView->delColumn(); */
            return true;
        }
    }

    return false;
}

voidNSSOAPDocument::donneChamp(SOAPObject* pObject)
{
  if (!pObject || (pPrincipes->empty()))
    return ;

  string sContexte = string("") ;

  if (pObject->pCaptElemnt)
  {
    if (pObject->pCaptElemnt->getChemin() != string(""))
    {
      string sPath = pObject->pCaptElemnt->getChemin() ;
      pContexte->getDico()->donneCodeSens(&sPath, &sContexte) ;
    }
  }

    if (sContexte != "")
    {
        for (PrinciplesIter Iter = pPrincipes->begin(); Iter != pPrincipes->end() ; Iter++)
        {
            string sCaseSens ;
            pContexte->getDico()->donneCodeSens(&((*Iter)->sCase), &sCaseSens) ;
            if (sContexte.find(sCaseSens) != NPOS)
            {
                pObject->sCase = (*Iter)->sCase;
                break;
            }
        }
    }

    if (pObject->sCase == "")
        return ;

    pObject->iImageIndex = donneIconIndex(pObject->sCase);

    if      (pObject->iConfidence == 100)
        pObject->iImageIndex += 2 * CLASSIF_PRINCIPLE_NBMAXSLOT ;
    else if (pObject->iConfidence > 0)
        pObject->iImageIndex += CLASSIF_PRINCIPLE_NBMAXSLOT ;
}

stringNSSOAPDocument::donneCaseLexique(int index)
{
  switch (index)
  {
    case 0 : return string("0MOTI") ; // Motif
    case 1 : return string("0SOA0") ; // S du SOAP
    case 2 : return string("0SOA2") ; // O du SOAP
    case 3 : return string("0SOA5") ; // Procédure pré-appréciation SOPAP
    case 4 : return string("0SOA4") ; // A du SOAP
    case 5 : return string("0SOA3") ; // P du SOAP
    case 6 : return string("0SOPR") ; // Procédure réalisée du SOAP1P2
    case 7 : return string("0SOPP") ; // Procédure prescrite du SOAP1P2
  }

  return string("") ;
}

intNSSOAPDocument::donneIconIndex(string sCode)
{
  string sCaseSens ;
  pContexte->getDico()->donneCodeSens(&sCode, &sCaseSens) ;

  for (int i = 0; i < CLASSIF_PRINCIPLE_NBMAXSLOT; i++)
    if (donneCaseLexique(i) == sCaseSens)
      return i ;

  return -1 ;
}

boolNSSOAPDocument::setDocument(NSRefDocument* pDoc){  if (pBasketList)    return false ;  pDocument = pDoc ;  return true ;}// --------------------------------------------------------------------------
//
// Class NSSOAPView
//
// --------------------------------------------------------------------------

const int IDSOAPWindow      = 0x200;
const int IDSOAPTank        = 0x300;
const int IDSOAPList        = 0x210;
const int IDSOAPTankList    = 0x310;
const int IDSOAPDialog      = 0x400;


// Table de réponses de la classe NSSOAPView

DEFINE_RESPONSE_TABLE2(NSSOAPView, TView, TWindow)
    EV_VN_ISWINDOW,
    EV_LVN_GETDISPINFO(IDSOAPWindow, DispInfoSOAP),
    EV_WM_SIZE,
    EV_WM_MOVE,
    EV_WM_PAINT,
    EV_WM_WINDOWPOSCHANGED,
    EV_WM_SETFOCUS,
    EV_WM_MOUSEMOVE,
    EV_COMMAND(CM_EPISO_TRANSMIT, transmit),    EV_WM_DESTROY,
    // EV_COMMAND(IDC_EPISOD_CLASSIF, EpisodSetClassif),
END_RESPONSE_TABLE;

// Constructeur

NSSOAPView::NSSOAPView(NSSOAPDocument& doc, TWindow *parent, string sPreoccu )
 //          :TWindowView(doc, parent)
             : NSMUEView(doc.pContexte, &doc, parent, "Soaping", "SoapDoc", sPreoccu)
  // NSMUEView(NSContexte* pCtx, TDocument* pDoc, TPaneSplitter *pPaneSplitter = 0, TWindow *parent = 0, string sFunction = "", string sPreoccu = "");
{
try
{
	pDocSOAP = &doc;
	bFirstSetup = true;

	//
	// On référence cette fenêtre au niveau du module Episodus
	//
	pContexte->getEpisodus()->pSOAPView = this ;

	//
	// On crée l'array de Listwindows
	//
	pArraySOAPListWindows = new SOAPListWinArray() ;

	int     iCol    = 0;
	long    newIDlw = IDSOAPList;

	//
	// On crée les ListWindows qui correspondent au Panier vide
	//
	if (!(pDocSOAP->pPrincipes->empty()))
	{
		int iLine = 0;
		PrinciplesIter Iter = pDocSOAP->pPrincipes->begin();
		for (; Iter != pDocSOAP->pPrincipes->end() ; Iter++, iLine++)
		{
			newIDlw++ ;

			NSSOAPListWindow *pNewSOAPlw = new NSSOAPListWindow(this,
                                                newIDlw, 0, 0, 0, 0, *Iter) ;

			pNewSOAPlw->iColumn     = iCol ;
			pNewSOAPlw->iLine       = iLine ;

			pArraySOAPListWindows->push_back(pNewSOAPlw);
		}
	}
	iCol++ ;

	bSetupToolBar = true ;

	setViewName() ;
}
catch (...)
{
	erreur("Exception NSSOAPView ctor.", standardError, 0) ;
}
}

// Destructeur
NSSOAPView::~NSSOAPView(){
  string ps = string("Deleting SOAPView.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  if (pContexte->getEpisodus()->pSOAPView == this)
    pContexte->getEpisodus()->pSOAPView = 0 ;

  for (ArraySOAPListWindowIter WindowIter = pArraySOAPListWindows->begin() ;
                                pArraySOAPListWindows->end() != WindowIter ; )
    pArraySOAPListWindows->erase(WindowIter) ;

  delete pArraySOAPListWindows ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)

TWindow*
NSSOAPView::GetWindow()
{
  return (TWindow*) this ;
}

void
NSSOAPView::setViewName()
{
	_sViewName = pContexte->getSuperviseur()->getText("SOAPManagement", "SOAPViewTitle") ;

  addConcernTitle() ;
}

// Appel de la fonction de remplissage de la vue
void
NSSOAPView::SetupWindow()
{
try
{
	//SetWindowPosit();

	if (bFirstSetup)
	{
  	//
    // On crée les ListWindows qui correspondent aux Paniers existants
    //
    if (false == pDocSOAP->pBasketList->empty())
    {
    	SOAPBasketArrayIter baskIter = pDocSOAP->pBasketList->begin() ;
      for (; pDocSOAP->pBasketList->end() != baskIter ; baskIter++)
      {
      	SOAPBasket* pBasket = (*baskIter) ;
        if (false == pBasket->empty())
        {
        	pDocSOAP->SetNProblems(pDocSOAP->GetNProblems() + 1) ;

          //
          // Taille de la fenêtre
          // Size of the window
          //
          ClassLib::TRect SurfaceWindow = GetClientRect() ;
          ClassLib::TRect CoordsListWindow ;

          long hOffset = (SurfaceWindow.right - SurfaceWindow.left) / (pDocSOAP->GetNProblems() + 1) ;
          long vOffset = (SurfaceWindow.bottom - SurfaceWindow.top) / pDocSOAP->GetNCategory() ;
          //          // On passe en revue l'ensemble des ListWindows existantes
          //
          ArraySOAPListWindowIter ItLW = pArraySOAPListWindows->begin() ;
          for (; ItLW != pArraySOAPListWindows->end(); ItLW++)
          {
          	// Les ListWindows fictives sont décalées
            //
            if ((*ItLW)->iColumn >= pDocSOAP->GetNProblems() - 1)
            	(*ItLW)->iColumn++ ;

            //
            // Chaque ListWindow est repositionnée
            //
            CoordsListWindow.left   = SurfaceWindow.left + ((*ItLW)->iColumn * hOffset) ;
            CoordsListWindow.top    = SurfaceWindow.top + ((*ItLW)->iLine * vOffset) ;
            CoordsListWindow.right  = SurfaceWindow.left + ((((*ItLW)->iColumn)+1) * hOffset) ;
            CoordsListWindow.bottom = SurfaceWindow.top + ((((*ItLW)->iLine)+1) * vOffset) ;

            (*ItLW)->MoveWindow(CoordsListWindow, true) ;
          }

          // int iLine = 0;
          SOAPBasketIter objsIter = pBasket->begin() ;
          int iCategory = 0 ;
          for (; objsIter != pBasket->end(); objsIter++, iCategory++)
          {
          	int newIDlw = IDSOAPList + (pDocSOAP->GetNProblems() * pDocSOAP->GetNCategory()) + iCategory + 1 ;

            NSSOAPListWindow *pNewSOAPlw = new NSSOAPListWindow(this,
                                newIDlw, 0, 0, 0, 0, (*objsIter)->pPrincipe) ;

            pNewSOAPlw->SetBkgndColor(TColor::LtCyan) ;

            pNewSOAPlw->iColumn     = pDocSOAP->GetNProblems() - 1 ;
            pNewSOAPlw->iLine       = iCategory ;
            pNewSOAPlw->pBasket     = pBasket ;
            pNewSOAPlw->pObjectList = *objsIter ;
            if (false == (*objsIter)->empty())
            {
              SOAPObjectArray* pObs = (*objsIter) ;
              ArraySOAPObjectIter oIter = pObs->begin() ;
              for (; pObs->end() != oIter ; oIter++)
              {
                // (*oIter)->iListParam = pNewSOAPlw->pObjectList->donneNextListParam();
                string sLib = (*oIter)->donneLibelle() ;
                TListWindItem Item(sLib.c_str()) ;
                Item.SetText(sLib.c_str()) ;
                Item.SetImageIndex((*oIter)->iImageIndex) ;
                Item.SetItemData((*oIter)->iListParam) ;
                pNewSOAPlw->AddItem(Item) ;
              }
            }

            //
            // Positionnement
            //
            CoordsListWindow.left   = SurfaceWindow.left + (pNewSOAPlw->iColumn * hOffset) ;
            CoordsListWindow.top    = SurfaceWindow.top + (pNewSOAPlw->iLine * vOffset) ;
            CoordsListWindow.right  = SurfaceWindow.left + (((pNewSOAPlw->iColumn)+1) * hOffset) ;
            CoordsListWindow.bottom = SurfaceWindow.top + (((pNewSOAPlw->iLine)+1) * vOffset) ;

            pNewSOAPlw->MoveWindow(CoordsListWindow, true) ;

            pArraySOAPListWindows->push_back(pNewSOAPlw) ;
          }
        }
      }
    }

    Parent->ChildBroadcastMessage(WM_MOVE) ;

    // NSSOAPTankView* pTankView = pContexte->getSuperviseur()->pEpisodus->pSOAPTank;
    // if (pTankView)
    //    pTankView->reposit();
  }

  TWindow::SetupWindow() ;
  TWindow::SetDocTitle("Eléments de rencontre constitués", 0) ;

  bFirstSetup = false ;
}
catch (...)
{
	erreur("Exception NSSOAPView::SetupWindow.", standardError, 0) ;
}
}

void
NSSOAPView::transmit()
{
  if (pDocSOAP->enregistrer())
    pDocSOAP->pEpisodus->endingSOAP() ;
}

void
NSSOAPView::DispInfoSOAP(TLwDispInfoNotify& dispInfo)
{
/*
    const int       BufLen = 255;
    static char     buffer[BufLen];
    TListWindItem   &dispInfoItem = *(TListWindItem*)&dispInfo.item;

    int index = dispInfoItem.GetIndex();
*/
}

// Fonction CanClose : Appel de CanClose du document

bool
NSSOAPView::CanClose()
{
  TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;

  pMyApp->FlushControlBar() ;
  bSetupToolBar = false ;

  return true ;
}

bool
NSSOAPView::VnIsWindow(HWND hWnd)
{
  return HWindow == hWnd ;
}

void
NSSOAPView::EvDestroy()
{
/*
    if (pDocSOAP->pEpisodus->bAutoSave)
        RecordWindowPosit(); */
}

void
NSSOAPView::SetWindowPosit()
{
  string sPosition = pDocSOAP->pEpisodus->sPosSOAP ;

  if (string("") == sPosition)
    return;

    int     X,Y,W,H;
    string  sTaille;
    size_t  debut = 0;
    size_t  positVide = sPosition.find("|");  //chaîne vide
    int     nbCoords = 0;

    vector<string> Coordonnees;  //contient les coordonnées sous forme de string

    while (positVide != NPOS)
    {
        Coordonnees.push_back(string(sPosition, debut, positVide - debut));
        nbCoords++;
        debut = positVide + 1;
        positVide = sPosition.find("|", debut+1);
    }
    Coordonnees.push_back(string(sPosition, debut, strlen(sPosition.c_str()) - debut));
    nbCoords++;
    //
    //récupérer les coordonnées
    //
    vector<string> :: iterator i;
    i = Coordonnees.begin();
    X = StringToDouble(*i);
    i++;
    Y = StringToDouble(*i);
    i++;
    W  = StringToDouble(*i);
    i++;
    H = StringToDouble(*i);

    // cas en icone ou plein ecran
    if (nbCoords > 4)
    {
        i++;
        sTaille = *i;
    }
    else
        sTaille = "N";

    Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER);

    if (sTaille == "I")
        Parent->Show(SW_SHOWMINIMIZED);
    else if (sTaille == "Z")
        Parent->Show(SW_SHOWMAXIMIZED);
    else
        Parent->Show(SW_SHOWNORMAL);
}

void
NSSOAPView::RecordWindowPosit()
{
    string sPosSoap = pDocSOAP->pEpisodus->sPosSOAP;

    NS_CLASSLIB::TRect rectDlg = Parent->GetWindowRect();//coordonnées par % à l'écran
    int X, Y ;
    // int W, H ;
    NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y());
    Parent->Parent->ScreenToClient(point);
    X = point.X();
  	Y = point.Y();
    // W = rectDlg.Width();
    // H = rectDlg.Height();

    double TamponX  = (double)X;
    double TamponY  = (double)Y;
    double TamponW  = (double)rectDlg.Width();
    double TamponH  = (double)rectDlg.Height();
    string sPosition =  DoubleToString(&TamponX, 0 , 0)
                       + "|" + DoubleToString(&TamponY, 0 , 0)
                       + "|" + DoubleToString(&TamponW, 0 , 0)
                       + "|" + DoubleToString(&TamponH, 0 , 0);

    // On regarde si la fenetre est en icone ou en plein ecran
    if ((Parent->IsIconic()) || (Parent->IsZoomed()))
    {
        size_t poslast;

        sPosition = sPosSoap;
        poslast = sPosition.find_last_of("|");
        // si le '|' n'est pas le dernier caractere
        if ((poslast != NPOS) && (poslast < (strlen(sPosition.c_str()) - 1)))
        {
            // si la fenetre était déja en icone ou en zoom :
            // on reprend l'ancienne taille
            if ((sPosition[poslast + 1] == 'I') || (sPosition[poslast + 1] == 'Z'))
                sPosition = string(sPosition, 0, poslast);

            if (Parent->IsIconic())
                sPosition += "|I";
            else
                sPosition += "|Z";
        }
        else
        {
            erreur("Coordonnée éronnée.", standardError, 0, GetHandle());
            // on remet dans ce cas la fenetre a une taille par défaut
            sPosition = "0|0|100|100";
        }
    }
    pDocSOAP->pEpisodus->sPosSOAP = sPosition;
    pDocSOAP->pEpisodus->saveParams();
}

/*void    NSSOAPView::addButton()
{
    ClassLib::TRect     SurfaceWindow = GetClientRect();
    ClassLib::TRect     CoordsListWindow;

    long    hOffset = (SurfaceWindow.right - SurfaceWindow.left) / (pDocSOAP->GetNProblems() + 2);
    long    vOffset = (SurfaceWindow.bottom - SurfaceWindow.top) / pDocSOAP->GetNCategory();

    int   iProblem = 0;
    int   iCategory = 0;

    for (ArraySOAPListWindowIter pSOAPlwIter = pArraySOAPListWindows->begin() ;
        pSOAPlwIter != pArraySOAPListWindows->end() ;
        pSOAPlwIter++)
    {
        if (iProblem == pDocSOAP->GetNProblems() + 1)
        {
            iProblem = 0;
            iCategory++;
        }

        if (iProblem == 0)
        {
            int ButtonId = IDSOAPButton;
            TButton *myButton = new TButton(this->GetWindow(), ButtonId, "text", SurfaceWindow.left + (iProblem * hOffset), SurfaceWindow.top + (iCategory * vOffset), hOffset, vOffset);
            this->SetupWindow();
            iProblem++;
        }

        if (iCategory == pDocSOAP->GetNCategory())
            iCategory = 0;

        CoordsListWindow.left     = SurfaceWindow.left + (iProblem * hOffset);
        CoordsListWindow.top      = SurfaceWindow.top + (iCategory * vOffset);
        CoordsListWindow.right    = SurfaceWindow.left + ((iProblem+1) * hOffset);
        CoordsListWindow.bottom   = SurfaceWindow.top + ((iCategory+1) * vOffset);

        (*pSOAPlwIter)->MoveWindow(CoordsListWindow, true);

        iProblem++;

        pContexte->getSuperviseur()->pEpisodus->pSOAPView->UpdateWindow();
    }
}
*/

void
NSSOAPView::addColumn(SOAPBasket* pBasket)
{
try
{
    //
    // On incrémente le nombre de problèmes
    //
    pDocSOAP->SetNProblems(pDocSOAP->GetNProblems() + 1);

    //
    // On se connecte au principe de classification pour cette colonne
    // Connecting to the classification principle for that column
    //
    PrinciplesArray* pPrincipes;

    if (pBasket)
        pPrincipes = pBasket->pPrincipes;
    else
        pPrincipes = pDocSOAP->pPrincipes;

    //
    // Taille de la fenêtre
    // Size of the window
    //
    ClassLib::TRect   SurfaceWindow = GetClientRect();
    ClassLib::TRect   CoordsListWindow;

    long    hOffset = (SurfaceWindow.right - SurfaceWindow.left) / (pDocSOAP->GetNProblems() + 1);
    long    vOffset = (SurfaceWindow.bottom - SurfaceWindow.top) / pDocSOAP->GetNCategory();

    //
    // On passe en revue l'ensemble des ListWindows existantes
    //
    ArraySOAPListWindowIter ItLW = pArraySOAPListWindows->begin();
    for (; ItLW != pArraySOAPListWindows->end(); ItLW++)
    {
        // Les ListWindows fictives sont décalées
        //
        if ((*ItLW)->iColumn >= pDocSOAP->GetNProblems() - 1)
            (*ItLW)->iColumn++;

        //
        // Chaque ListWindow est repositionnée
        //
        CoordsListWindow.left     = SurfaceWindow.left + ((*ItLW)->iColumn * hOffset);
        CoordsListWindow.top      = SurfaceWindow.top + ((*ItLW)->iLine * vOffset);
        CoordsListWindow.right    = SurfaceWindow.left + ((((*ItLW)->iColumn)+1) * hOffset);
        CoordsListWindow.bottom   = SurfaceWindow.top + ((((*ItLW)->iLine)+1) * vOffset);

        (*ItLW)->MoveWindow(CoordsListWindow, true);
    }

    //
    // On crée une NSSOAPListWindow pour chaque case du principle
    // Creating a NSSOAPListWindow for each principle element
    //
    if (!(pPrincipes->empty()))
    {
        PrinciplesIter Iter = pPrincipes->begin();
        int iCategory = 0;
        for (; Iter != pPrincipes->end() ; Iter++, iCategory++)
        {
            int newIDlw = IDSOAPList + (pDocSOAP->GetNProblems() * pDocSOAP->GetNCategory()) + iCategory + 1;
            NSSOAPListWindow *pNewSOAPlw = new NSSOAPListWindow(this, newIDlw,
                                                    0, 0, 0, 0, *Iter);
            pNewSOAPlw->SetBkgndColor( TColor::LtCyan );

            pNewSOAPlw->iColumn     = pDocSOAP->GetNProblems() - 1;
            pNewSOAPlw->iLine       = iCategory;
            pNewSOAPlw->pBasket     = pBasket;

            //
            // Connection de la NSSOAPListWindow à la bonne liste d'objects
            // Connecting the NSSOAPListWindow to the proper objects list
            //
            if ((pBasket) && (!(pBasket->empty())))
            {
                SOAPBasketIter iOA = pBasket->begin();
                for (; (iOA != pBasket->end()) &&
                        (!(*((*iOA)->pPrincipe) == **Iter)) ; iOA++);
                if (iOA != pBasket->end())
                    pNewSOAPlw->pObjectList = *iOA;

                if ((pNewSOAPlw->pObjectList) &&
                            (!(pNewSOAPlw->pObjectList->empty())))
                {
                    SOAPObjectArray* pObs = pNewSOAPlw->pObjectList;
                    ArraySOAPObjectIter oIter = pObs->begin();
                    for (; oIter != pObs->end(); oIter++)
                    {
                        (*oIter)->iListParam = pNewSOAPlw->pObjectList->donneNextListParam();
                        string sLib = (*oIter)->donneLibelle();
                        TListWindItem Item(sLib.c_str());
                        Item.SetText(sLib.c_str());
                        Item.SetImageIndex((*oIter)->iImageIndex);
                        Item.SetItemData((*oIter)->iListParam);
                        pNewSOAPlw->AddItem(Item);
                    }
                }
            }

            pArraySOAPListWindows->push_back(pNewSOAPlw);

            //
            // Positionnement
            //
            CoordsListWindow.left     = SurfaceWindow.left + (pNewSOAPlw->iColumn * hOffset);
            CoordsListWindow.top      = SurfaceWindow.top + (pNewSOAPlw->iLine * vOffset);
            CoordsListWindow.right    = SurfaceWindow.left + (((pNewSOAPlw->iColumn)+1) * hOffset);
            CoordsListWindow.bottom   = SurfaceWindow.top + (((pNewSOAPlw->iLine)+1) * vOffset);

            pNewSOAPlw->MoveWindow(CoordsListWindow, true);
        }
    }

    // reinitialisation de la fenetre
    SetupWindow();
}
catch (...)
{
    erreur("Exception NSSOAPView::addColumn.", standardError, 0) ;
}
}

void
NSSOAPView::delColumn(SOAPBasket* pBasket)
{
    if (!pBasket || (pArraySOAPListWindows->empty()))
        return;

    ClassLib::TRect   SurfaceWindow = GetClientRect();    // size of the window
    ClassLib::TRect   CoordsListWindow;

    long    hOffset = (SurfaceWindow.right - SurfaceWindow.left) / pDocSOAP->GetNProblems();
    long    vOffset = (SurfaceWindow.bottom - SurfaceWindow.top) / pDocSOAP->GetNCategory();

    ArraySOAPListWindowIter Iter = pArraySOAPListWindows->begin();

    int iDelCol = -1;

    //
    // En premier lieu, on supprime les cases du panier
    //
    for (; Iter != pArraySOAPListWindows->end(); )
    {
        if ((*Iter)->pBasket == pBasket)
        {
            iDelCol = (*Iter)->iColumn;
            (*Iter)->Destroy();
            delete *Iter;
            pArraySOAPListWindows->erase(Iter);
        }
        else
            Iter++;
    }

    //
    // Ensuite, on repositionne les cases restantes
    //
    if (!(pArraySOAPListWindows->empty()))
    {
        Iter = pArraySOAPListWindows->begin();
        for (; Iter != pArraySOAPListWindows->end(); Iter++)
        {
            if ((iDelCol != -1) && ((*Iter)->iColumn > iDelCol))
                ((*Iter)->iColumn)--;

            CoordsListWindow.left     = SurfaceWindow.left + ((*Iter)->iColumn * hOffset);
            CoordsListWindow.top      = SurfaceWindow.top + ((*Iter)->iLine * vOffset);
            CoordsListWindow.right    = SurfaceWindow.left + ((((*Iter)->iColumn)+1) * hOffset);
            CoordsListWindow.bottom   = SurfaceWindow.top + ((((*Iter)->iLine)+1) * vOffset);

            (*Iter)->MoveWindow(CoordsListWindow, true);
            (*Iter)->Invalidate();
        }
    }

    pDocSOAP->SetNProblems(pDocSOAP->GetNProblems() - 1);

    // reinitialisation de la fenetre
    this->SetupWindow();
}

boolNSSOAPView::noItem(int iCol)
{
	if (pArraySOAPListWindows->empty())
		return true ;

	for (ArraySOAPListWindowIter pSOAPlwIter = pArraySOAPListWindows->begin() ;
        pSOAPlwIter != pArraySOAPListWindows->end() ;
        pSOAPlwIter++)
	{
  	if ((*pSOAPlwIter)->iColumn == iCol)
    	if (!((*pSOAPlwIter)->pObjectList->empty()))
      	return false ;
  }
  return true ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindow

void
NSSOAPView::EvSize(uint sizeType, ClassLib::TSize& size)
{
    TWindow::EvSize(sizeType, size);

    if (pArraySOAPListWindows->empty())        return;

    ClassLib::TRect   SurfaceWindow = GetClientRect();    // size of the window
    ClassLib::TRect   CoordsListWindow;

    long    hOffset = (SurfaceWindow.right - SurfaceWindow.left) / (pDocSOAP->GetNProblems() + 1);
    long    vOffset = (SurfaceWindow.bottom - SurfaceWindow.top) / pDocSOAP->GetNCategory();

    for (ArraySOAPListWindowIter ItLW = pArraySOAPListWindows->begin() ;
        ItLW != pArraySOAPListWindows->end() ;
        ItLW++)
    {
        CoordsListWindow.left     = SurfaceWindow.left + ((*ItLW)->iColumn * hOffset);
        CoordsListWindow.top      = SurfaceWindow.top + ((*ItLW)->iLine * vOffset);
        CoordsListWindow.right    = SurfaceWindow.left + ((((*ItLW)->iColumn)+1) * hOffset);
        CoordsListWindow.bottom   = SurfaceWindow.top + ((((*ItLW)->iLine)+1) * vOffset);

        (*ItLW)->MoveWindow(CoordsListWindow, true);
        (*ItLW)->Invalidate();
    }

//    NSSOAPTankView* pTankView = pContexte->getSuperviseur()->pEpisodus->pSOAPTank;//    if (pTankView)
//    {
//        pTankView->reposit();
        // pTankView->Invalidate();
//    }
}

void
NSSOAPView::EvMove(ClassLib::TPoint& clientOrigin)
{
	TWindow::EvMove(clientOrigin);

//    NSSOAPTankView* pTankView = pContexte->getSuperviseur()->pEpisodus->pSOAPTank;
//    if (pTankView)
//        pTankView->reposit();
}

voidNSSOAPView::EvWindowPosChanged(WINDOWPOS far& windowPos)
{
	TWindow::EvWindowPosChanged(windowPos);

//    NSSOAPTankView* pTankView = pContexte->getSuperviseur()->pEpisodus->pSOAPTank;
//    if (pTankView)
//        pTankView->reposit();
}

voidNSSOAPView::gotMoved()
{
//    NSSOAPTankView* pTankView = pContexte->getSuperviseur()->pEpisodus->pSOAPTank;
//    if (pTankView)
//        pTankView->reposit();
}

voidNSSOAPView::EvPaint(){
	TWindow::EvPaint() ;

//    NSSOAPTankView* pTankView = pContexte->getSuperviseur()->pEpisodus->pSOAPTank;
//    if (pTankView)
//        pTankView->reposit();
}

// fonction EVSetFocusvoid
NSSOAPView::EvSetFocus(HWND hWndLostFocus)
{
	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  NSMUEView::EvSetFocus(hWndLostFocus) ;
	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar();
		pMyApp->SetToolBarWindow(GetWindow());
	}

	_pPaneSplitter->SetFrameTitle(getFunction(), _sViewName) ;	pContexte->setAideIndex("");	pContexte->setAideCorps("epi_soaping.htm");
}

voidNSSOAPView::SetupToolBar()
{
try
{
	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	//pMyApp->cb2->Insert(*new TButtonGadget(IDC_EPISOD_CLASSIF, IDC_EPISOD_CLASSIF, TButtonGadget::Command));	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EPISO_TRANSMIT, CM_EPISO_TRANSMIT, TButtonGadget::Command)) ;

	pMyApp->getSecondaryControlBar()->LayoutSession() ;
}
catch (...)
{
	erreur("Exception NSSOAPView::SetupToolBar.", standardError, 0) ;
}
}

void
NSSOAPView::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	if (pDocSOAP->bDragAndDrop)
	{
  	ScreenToClient(point) ;
    TLwHitTestInfo hitTestInfo ;
    hitTestInfo.pt = point ;

    pDocSOAP->ImgDragDrop->DragMove(point.x, point.y) ;
//        SetCursor(0, IDC_IBEAM);
	}
}

voidNSSOAPView::logMessage(string sLogMessage){
    ifstream    inFile;
    ofstream    outFile;
    string      line;
    string      sLastLogMessage;

    inFile.open("SOAP.log");
    if (inFile)
    {
   	    while (!inFile.eof())
        {
            getline(inFile,line);
            if (line != "")
                sLastLogMessage += line + "\n";
        }

        inFile.close();
    }
    else // cas fichier vide
        sLastLogMessage = "Nautilus log : ";

    // on ajoute la nouvelle erreur
    sLastLogMessage += sLogMessage + "\n";

    // On écrit le nouveau fichier reqerror.log
   	outFile.open("SOAP.log");
	if (!outFile)
   	{
        // cas d'erreur bloquant...
        string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
        MessageBox(/*NULL,*/ "Erreur d'ouverture en écriture du fichier SOAP.log.", sCaption.c_str(), MB_ICONEXCLAMATION);
   		return;
   	}

   	for (size_t i = 0; i < strlen(sLastLogMessage.c_str()); i++)
     	 outFile.put(sLastLogMessage[i]);

    outFile.close();
}


// --------------------------------------------------------------------------
//
// Class NSSOAPTankView
//
// --------------------------------------------------------------------------

// Table de réponses de la classe NSSOAPTankView

DEFINE_RESPONSE_TABLE2(NSSOAPTankView, TView, TWindow)
    EV_VN_ISWINDOW,
    EV_LVN_GETDISPINFO(IDSOAPTank, DispInfoTank),
    EV_WM_SIZE,
    EV_WM_SETFOCUS,
    EV_WM_MOUSEMOVE,
    EV_COMMAND(CM_EPISO_FLOCULE, floculer),
    EV_WM_DESTROY,
END_RESPONSE_TABLE;

// Constructeur
NSSOAPTankView::NSSOAPTankView(NSSOAPDocument& doc, TWindow *parent,
                                                            string sPreoccu )
               :NSMUEView(doc.pContexte, &doc, parent, "Tanking", "SoapDoc", sPreoccu )
{
try
{
	pDocSOAP = &doc;

	pContexte->getEpisodus()->pSOAPTank = this ;
	pMyListWindow = new NSSOAPListWindow(this, IDSOAPTankList, 0, 0, 0, 0);
	pMyListWindow->pObjectList = pDocSOAP->pTankObjectList;

	if (pDocSOAP->pTankObjectList && (false == pDocSOAP->pTankObjectList->empty()))
	{
		ArraySOAPObjectIter    myObject;

		for (myObject = pDocSOAP->pTankObjectList->begin();
                    myObject != pDocSOAP->pTankObjectList->end() ; myObject++)
		{
			addObject(*myObject);
            // SOAPObject *pObjectSOAP = *myObject;
            // pMyListWindow->pObjectList->push_back(pObjectSOAP);
            // TListWindItem Item(pObjectSOAP->sTexteInitial.c_str());
            // Item.SetText(pObjectSOAP->sTexteInitial.c_str());
		}
	}
	bSetupToolBar = true;

	setViewName() ;
}
catch (...)
{
	erreur("Exception NSSOAPTankView ctor.", standardError, 0) ;
}
}

// Destructeur

NSSOAPTankView::~NSSOAPTankView(){
	if (pContexte->getEpisodus()->pSOAPTank == this)
  	pContexte->getEpisodus()->pSOAPTank = 0 ;

	delete pMyListWindow ;
}

void
NSSOAPTankView::setViewName()
{
	_sViewName = pContexte->getSuperviseur()->getText("SOAPManagement", "SOAPTankViewTitle") ;

  addConcernTitle() ;
}

void
NSSOAPTankView::addObject(SOAPObject* pObjectSOAP)
{
	if ((SOAPObject*) NULL == pObjectSOAP)
  	return ;

  if (pMyListWindow->HWindow)
  	pMyListWindow->addObject(pObjectSOAP) ;
}

void
NSSOAPTankView::floculer()
{
	pDocSOAP->floculer() ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow*
NSSOAPTankView::GetWindow()
{
   return (TWindow*) this ;
}

// Appel de la fonction de remplissage de la vue
void
NSSOAPTankView::SetupWindow()
{
	// reposit();

	SetWindowPosit() ;

	TWindow::SetupWindow() ;
	TWindow::SetDocTitle(_sViewName.c_str(), 0) ;

	if (pContexte->getEpisodus()->bAutoFlocule)
		floculer() ;
}

void
NSSOAPTankView::reposit()
{
    NSSuper* pSuper = pContexte->getSuperviseur();

    NSSOAPView* pView = pContexte->getEpisodus()->pSOAPView;
    if (pView)
    {
        WINDOWPLACEMENT placement;
        if (pView->GetWindowPlacement(&placement))
        {
            if (placement.showCmd == SW_SHOWMINIMIZED)
                Parent->Show(SW_SHOWMINIMIZED);
            else if (placement.showCmd == SW_SHOWNORMAL)
            {
                NSMDIFrame* pFrame = pSuper->getApplication()->getFrame() ;
                ClassLib::TRect frameRect = pFrame->GetClientWindow()->GetWindowRect() ;

                ClassLib::TRect viewRect = pView->Parent->GetWindowRect();

                ClassLib::TRect tankRect;
                tankRect.left     = viewRect.left;
                tankRect.top      = viewRect.bottom - frameRect.top;
                tankRect.right    = viewRect.right;
                tankRect.bottom   = tankRect.top + (viewRect.Height() / 4);

                Parent->SetWindowPos(0, tankRect, SWP_NOZORDER);
                Parent->Show(SW_SHOWNORMAL);
            }
        }
    }
}

void    NSSOAPTankView::DispInfoTank(TLwDispInfoNotify& dispInfo)
{
/*
    const int       BufLen = 255;
    static char     buffer[BufLen];
    TListWindItem   &dispInfoItem = *(TListWindItem*)&dispInfo.item;

    int index = dispInfoItem.GetIndex();
*/
}

// Fonction CanClose : Appel de CanClose du document

bool
NSSOAPTankView::CanClose()
{
  TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;

  pMyApp->FlushControlBar() ;
  bSetupToolBar = false ;

  return true ;
}

bool
NSSOAPTankView::VnIsWindow(HWND hWnd)
{
  return HWindow == hWnd ;
}

void
NSSOAPTankView::EvDestroy()
{
/*    if (pDocSOAP->pEpisodus->bAutoSave)
        RecordWindowPosit();  */
}

void
NSSOAPTankView::SetWindowPosit()
{
    string sPosition = pDocSOAP->pEpisodus->sPosTank;

    if (sPosition == "")
        return;

    int     X,Y,W,H;
    string  sTaille;
    size_t  debut = 0;
    size_t  positVide = sPosition.find("|");  //chaîne vide
    int     nbCoords = 0;

    vector<string> Coordonnees;  //contient les coordonnées sous forme de string

    while (positVide != NPOS)
    {
        Coordonnees.push_back(string(sPosition, debut, positVide - debut));
        nbCoords++;
        debut = positVide + 1;
        positVide = sPosition.find("|", debut+1);
    }
    Coordonnees.push_back(string(sPosition, debut, strlen(sPosition.c_str()) - debut));
    nbCoords++;
    //
    //récupérer les coordonnées
    //
    vector<string> :: iterator i;
    i = Coordonnees.begin();
    X = StringToDouble(*i);
    i++;
    Y = StringToDouble(*i);
    i++;
    W  = StringToDouble(*i);
    i++;
    H = StringToDouble(*i);

    // cas en icone ou plein ecran
    if (nbCoords > 4)
    {
        i++;
        sTaille = *i;
    }
    else
        sTaille = "N";

    Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER);

    if (sTaille == "I")
        Parent->Show(SW_SHOWMINIMIZED);
    else if (sTaille == "Z")
        Parent->Show(SW_SHOWMAXIMIZED);
    else
        Parent->Show(SW_SHOWNORMAL);
}

void
NSSOAPTankView::RecordWindowPosit()
{
    string sPosSoap = pDocSOAP->pEpisodus->sPosTank;

    NS_CLASSLIB::TRect rectDlg = Parent->GetWindowRect();//coordonnées par % à l'écran
    int X, Y ;
    // int W, H ;
    NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y());
    Parent->Parent->ScreenToClient(point);
    X = point.X();
  	Y = point.Y();
    // W = rectDlg.Width();
    // H = rectDlg.Height();

    double TamponX  = (double)X;
    double TamponY  = (double)Y;
    double TamponW  = (double)rectDlg.Width();
    double TamponH  = (double)rectDlg.Height();

    string sPosition =  DoubleToString(&TamponX, 0 , 0)                       + "|" + DoubleToString(&TamponY, 0 , 0)
                       + "|" + DoubleToString(&TamponW, 0 , 0)
                       + "|" + DoubleToString(&TamponH, 0 , 0);

    // On regarde si la fenetre est en icone ou en plein ecran
    if ((Parent->IsIconic()) || (Parent->IsZoomed()))
    {
        size_t poslast;

        sPosition = sPosSoap;
        poslast = sPosition.find_last_of("|");
        // si le '|' n'est pas le dernier caractere
        if ((poslast != NPOS) && (poslast < (strlen(sPosition.c_str()) - 1)))
        {
            // si la fenetre était déja en icone ou en zoom :
            // on reprend l'ancienne taille
            if ((sPosition[poslast + 1] == 'I') || (sPosition[poslast + 1] == 'Z'))
                sPosition = string(sPosition, 0, poslast);

            if (Parent->IsIconic())
                sPosition += "|I";
            else
                sPosition += "|Z";
        }
        else
        {
            erreur("Coordonnée éronnée.", standardError, 0, GetHandle());
            // on remet dans ce cas la fenetre a une taille par défaut
            sPosition = "0|0|100|100";
        }
    }
    pDocSOAP->pEpisodus->sPosTank = sPosition;
    pDocSOAP->pEpisodus->saveParams();
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindow
voidNSSOAPTankView::EvSize(uint sizeType, ClassLib::TSize& size){
  TWindow::EvSize(sizeType, size) ;
  pMyListWindow->MoveWindow(GetClientRect(), true) ;
}

// fonction EVSetFocusvoid
NSSOAPTankView::EvSetFocus(HWND hWndLostFocus)
{
	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	NSMUEView::EvSetFocus(hWndLostFocus) ;
	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar();
		pMyApp->SetToolBarWindow(GetWindow());
	}

  _pPaneSplitter->SetFrameTitle(getFunction(), _sViewName);
	pContexte->setAideIndex("");
	pContexte->setAideCorps("epi_soaping.htm");
}

voidNSSOAPTankView::SetupToolBar()
{
try
{
	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar();

	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_EPISO_FLOCULE, CM_EPISO_FLOCULE, TButtonGadget::Command));

	pMyApp->getSecondaryControlBar()->LayoutSession();
}
catch (...)
{
	erreur("Exception NSSOAPTankView::SetupToolBar.", standardError, 0) ;
}
}

void
NSSOAPTankView::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
}

// --------------------------------------------------------------------------//
//					méthodes de NSSOAPListWindow
//                  ListWindow pour les NSSOAP View
//
// --------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSSOAPListWindow, TListWindow)
    EV_VN_ISWINDOW,
    EV_LVN_GETDISPINFO(IDSOAPList, DispInfoSOAP),
    EV_WM_SETFOCUS,
    EV_WM_MOVE,
    EV_WM_WINDOWPOSCHANGED,
    //Drag And Drop
    EV_WM_TIMER,
    EV_WM_MOUSEMOVE,
    EV_WM_LBUTTONDOWN,
  	EV_WM_LBUTTONUP,
    EV_WM_RBUTTONDOWN,
    EV_WM_LBUTTONDBLCLK,  //double click
    NS_LVN_BEGINDRAG(LvnBeginDrag),

    EV_COMMAND(IDC_CODE,        EvCode),
    EV_COMMAND(IDC_POSITION,    EvPosition),
    EV_COMMAND(IDC_DUPLIQU,     EvDupliquer),
    EV_COMMAND(IDC_CLONE,       EvCloner),
    EV_COMMAND(IDC_LIBELLE,     EvLibelle),

    EV_COMMAND(IDC_PRINCIPE1,   EvPosit1),
    EV_COMMAND(IDC_PRINCIPE2,   EvPosit2),
    EV_COMMAND(IDC_PRINCIPE3,   EvPosit3),
    EV_COMMAND(IDC_PRINCIPE4,   EvPosit4),
    EV_COMMAND(IDC_PRINCIPE5,   EvPosit5),
    EV_COMMAND(IDC_PRINCIPE6,   EvPosit6),
    EV_COMMAND(IDC_PRINCIPE7,   EvPosit7),
    EV_COMMAND(IDC_PRINCIPE8,   EvPosit8),
    EV_COMMAND(IDC_PRINCIPE9,   EvPosit9),

    EV_COMMAND(IDC_DUPLIQUE1,   EvDupli1),
    EV_COMMAND(IDC_DUPLIQUE2,   EvDupli2),
    EV_COMMAND(IDC_DUPLIQUE3,   EvDupli3),
    EV_COMMAND(IDC_DUPLIQUE4,   EvDupli4),
    EV_COMMAND(IDC_DUPLIQUE5,   EvDupli5),
    EV_COMMAND(IDC_DUPLIQUE6,   EvDupli6),
    EV_COMMAND(IDC_DUPLIQUE7,   EvDupli7),
    EV_COMMAND(IDC_DUPLIQUE8,   EvDupli8),
    EV_COMMAND(IDC_DUPLIQUE9,   EvDupli9),

END_RESPONSE_TABLE;

NSSOAPListWindow::NSSOAPListWindow(NSSOAPView* parent, int id, int x, int y,
                                    int w, int h, ClassificationPrinciple* pPrin,
                                    TModule* module)
                 :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
try
{
  pParentWindow   = parent;
  pDocSOAP        = parent->getDoc();
  pBasket         = 0;
  pPrincipe       = pPrin;
  pObjectList     = 0; //new SOAPObjectArray(false);

  Attr.Style      |= LVS_SHOWSELALWAYS | LVS_AUTOARRANGE;
  //Attr.ExStyle    |= WS_EX_NOPARENTNOTIFY;

  Attr.Style |= LVS_SHAREIMAGELISTS | LVS_SMALLICON;
  SetBkgndColor( TColor::Sys3dFace );

  pLgImageList = new TImageList(NS_CLASSLIB::TSize(32, 32), ILC_COLOR16, 15, 5);
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP2));

  pSmImageList = new TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR16, 15, 5);
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M2));  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR2));
	pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP2));
}
catch (...){
	erreur("Exception NSSOAPListWindow ctor.", standardError, 0) ;
}
}

NSSOAPListWindow::NSSOAPListWindow(NSSOAPTankView* parent, int id, int x, int y, int w, int h, TModule* module)                 :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
try
{
  pParentWindow   = parent;
  pDocSOAP        = parent->getDoc();
  pBasket         = 0;
  pPrincipe       = 0;
  pObjectList     = 0; //new SOAPObjectArray(false);

  Attr.Style      |= LVS_SHOWSELALWAYS | LVS_AUTOARRANGE;
  //Attr.ExStyle    |= WS_EX_NOPARENTNOTIFY;

  pLgImageList = new TImageList(NS_CLASSLIB::TSize(32, 32), ILC_COLOR16, 15, 5);
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP0));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP1));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR2));
  pLgImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP2));

  pSmImageList = new TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR16, 15, 5);
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP0));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP1));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_M2));  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_S2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_O2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PA2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_A2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_P2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PR2));
  pSmImageList->Add(OWL::TIcon(*GetApplication(), (TResId)IDB_SOAP_PP2));
}
catch (...)
{
	erreur("Exception NSSOAPListWindow ctor.", standardError, 0) ;
}
}

NSSOAPListWindow::~NSSOAPListWindow(){
    // Surtout pas :
    // delete pObjectList ;
    // Les ListWindows ne sont pas propriétaires de leur array d'objets
    // (elles appartiennent aux SOAPBasket ou au SOAPDocument (pour tankview))

    delete pLgImageList;
    delete pSmImageList;
}

void
NSSOAPListWindow::SetupWindow()
{
  TListWindow::SetupWindow();

  pLgImageList->SetBkColor(CLR_NONE);
  pSmImageList->SetBkColor(CLR_NONE);

  SetImageList(*pLgImageList, TListWindow::Normal);
  SetImageList(*pSmImageList, TListWindow::Small);

  SetTextBkColor(CLR_NONE);
  SetTextBkColor(CLR_NONE);

  /* for (int i = 0 ; i < 3*CLASSIF_PRINCIPLE_NBMAXSLOT; i++)
  {
      char szNum[3] ;
      numacar(szNum, i, 2) ;
      TListWindItem Item("");
      Item.SetText(szNum);
      Item.SetImageIndex(i);

      AddItem(Item);
  } */

  if (pPrincipe)
  {
    int iCaseIndex = pDocSOAP->donneIconIndex(pPrincipe->sCase) ;
    if (iCaseIndex >= 0)
    {
    	TListWindItem Item("") ;
      Item.SetText("") ;
      Item.SetImageIndex(iCaseIndex) ;

      AddItem(Item) ;
    }
  }

  ArraySOAPObjectIter    myObjectIterator;

  if (pObjectList && !(pObjectList->empty()))
  {
      for (myObjectIterator = pObjectList->begin(); myObjectIterator != pObjectList->end(); myObjectIterator++)
      {
          addObject(*myObjectIterator);
          /*TListWindItem Item((*myObjectIterator)->sTexteInitial.c_str());
          Item.SetText((*myObjectIterator)->sTexteInitial.c_str()); */
          /*
          char sBuffer[128];
          sprintf(sBuffer, "this <%d> ObjectList <%d> Iterator <%d> : <%s>", this, pObjectList, *myObjectIterator, (*myObjectIterator)->sTexteInitial.c_str());
          logMessage(sBuffer);
          */
          // int index = AddItem(Item);
      }
  }}

voidNSSOAPListWindow::addObject(SOAPObject* pObject)
{
  if (!pObjectList || !pObject)
  	return ;

  pObject->iListParam = pObjectList->donneNextListParam();  string sLib = pObject->donneLibelle();

  TListWindItem Item(sLib.c_str());
  Item.SetText(sLib.c_str());
  Item.SetImageIndex(pObject->iImageIndex);
  Item.SetItemData(pObject->iListParam);

  AddItem(Item);
}

void
NSSOAPListWindow::delObject(SOAPObject* pObjectSOAP, bool bOldID)
{
	if (!pObjectSOAP)
  	return ;

  int iParam ;
  if (bOldID)
  	iParam = pObjectSOAP->iOldListParam ;
  else
  	iParam = pObjectSOAP->iListParam ;

  int iIndex = getIndex(iParam) ;  if (iIndex != -1)
  	DeleteAnItem(iIndex) ;
}

void
NSSOAPListWindow::EvCode()
{
  int iFocus = getFocusedItem();

  TListWindItem Item;
  Item.SetIndex(iFocus);
  Item.SetSubItem(0);
  Item.mask = LVIF_PARAM;

  if (!(GetItem(Item)))
    return ;

  int iParams = Item.lParam ;

  SOAPObject* pObjet = pObjectList->donneObjet(iParams);
  if (NULL == pObjet)
    return ;

  classifExpert* pExpert = pDocSOAP->pContexte->getEpisodus()->pClassifExpert ;

  string sResult = pExpert->chooseCode(pObjet) ;
  if (sResult != "")
  {
    pObjet->sCode       = sResult;
    pObjet->iConfidence = 100;
    string sLib = pObjet->donneLibelle();

    pDocSOAP->donneChamp(pObjet);

    Item.SetText(sLib.c_str());
    Item.SetImageIndex(pObjet->iImageIndex);
    Item.mask = LVIF_TEXT | LVIF_IMAGE;
    SetItem(Item);

    Invalidate();
  }
}

voidNSSOAPListWindow::EvPosition()
{
try
{
  int iFocus = getFocusedItem();

  TListWindItem Item;
  Item.SetIndex(iFocus);
  Item.SetSubItem(0);
  Item.mask = LVIF_PARAM;

  if (!(GetItem(Item)))
      return;

  int iParams = Item.lParam;

  SOAPObject* pObjet = pObjectList->donneObjet(iParams);
  if (!pObjet)
      return;

  string sChemin         = "";
  string sLibelle        = pObjet->donneLibelle();
  string sClassifier     = "";

  ChoixCaptureContexte* pChercheDlg = new ChoixCaptureContexte(0, pDocSOAP->pContexte,
                                                                  &sChemin,
                                                                  &sClassifier,
                                                                  &sLibelle,                                                                  true);
  int iResult = pChercheDlg->Execute();
  delete pChercheDlg;

  if (iResult != IDOK)
      return;

  if (pObjet->pCaptElemnt)
  {
    pObjet->pCaptElemnt->setChemin(sChemin) ;
    pObjet->pCaptElemnt->setClassifier(sClassifier) ;
    pObjet->pCaptElemnt->setLibelle(sLibelle) ;
  }

  pObjet->sTexteInitial = sLibelle;
  // pObjet->sCase = sChemin;
  pObjet->sClassification = sClassifier;

  Item.SetText(sLibelle.c_str());

  pDocSOAP->donneChamp(pObjet);
  Item.SetImageIndex(pObjet->iImageIndex);

  Item.mask = LVIF_TEXT | LVIF_IMAGE;
  SetItem(Item);

  RedrawItems(iFocus, iFocus);
  UpdateWindow();

  /*
  classifExpert* pExpert = pContexte->getSuperviseur()->pEpisodus->pClassifExpert;

  string sResult = pExpert->chooseCode(pObjet);
  if (sResult != "")
  {
      pObjet->sCode       = sResult;
      pObjet->iConfidence = 100;
      string sLib = pObjet->donneLibelle();

      pDocSOAP->donneChamp(pObjet);

      Item.SetText(sLib.c_str());
      Item.SetImageIndex(pObjet->iImageIndex);
      Item.mask = LVIF_TEXT | LVIF_IMAGE;
      SetItem(Item);

      Invalidate();
  } */
}
catch (...)
{
    erreur("Exception NSSOAPListWindow::EvPosition.", standardError, 0) ;
}
}

voidNSSOAPListWindow::EvPosit(int iPos)
{
  int iFocus = getFocusedItem();

  TListWindItem Item;
  Item.SetIndex(iFocus);
  Item.SetSubItem(0);
  Item.mask = LVIF_PARAM;

  if (!(GetItem(Item)))
    return ;

  int iParams = Item.lParam;

  SOAPObject* pObjet = pObjectList->donneObjet(iParams);
  if (!pObjet)
    return ;

  if (pDocSOAP->pPrincipes->empty())
      return;
  PrinciplesIter prIt = pDocSOAP->pPrincipes->begin();
  for (int i =1; (prIt != pDocSOAP->pPrincipes->end()) && (i < iPos); prIt++, i++);
  if (prIt == pDocSOAP->pPrincipes->end())
    return ;

  string sContexte = pObjet->pCaptElemnt->getChemin() ;
  size_t iPosit = sContexte.find(pObjet->sCase) ;

  if (iPosit != NPOS)
  {
    size_t  iTailleCase = strlen(pObjet->sCase.c_str());
    size_t  iTailleCtx = strlen(sContexte.c_str());

    string  sDepass = "";
    if (iPosit + iTailleCase > iTailleCtx)
      sDepass = string(sContexte, iPosit + iTailleCase, iTailleCtx - iPosit - iTailleCase) ;
    pObjet->pCaptElemnt->setChemin(string(pObjet->pCaptElemnt->getChemin(), 0, iPosit)                                    + (*prIt)->sCase + sDepass) ;
  }

  Item.SetText(pObjet->sTexteInitial.c_str());

  pDocSOAP->donneChamp(pObjet);
  Item.SetImageIndex(pObjet->iImageIndex);

  Item.mask = LVIF_TEXT | LVIF_IMAGE;
  SetItem(Item);

  RedrawItems(iFocus, iFocus);
  UpdateWindow();
}

voidNSSOAPListWindow::EvDupli(int iPos)
{
try
{
  int iFocus = getFocusedItem();

  TListWindItem Item;
  Item.SetIndex(iFocus);
  Item.SetSubItem(0);
  Item.mask = LVIF_PARAM;

  if (!(GetItem(Item)))
      return;

  int iParams = Item.lParam;

  SOAPObject* pObjet = pObjectList->donneObjet(iParams);
  if (!pObjet)
      return;

  string sChemin      = pObjet->pCaptElemnt->getChemin() ;
  string sLibelle     = pObjet->sTexteInitial ;
  string sClassifier  = pObjet->pCaptElemnt->getClassifier() ;

  if (pDocSOAP->pPrincipes->empty())
      return;
  PrinciplesIter prIt = pDocSOAP->pPrincipes->begin();
  for (int i =1; (prIt != pDocSOAP->pPrincipes->end()) && (i < iPos); prIt++, i++);
  if (prIt == pDocSOAP->pPrincipes->end())
      return;

  string sContexte = pObjet->pCaptElemnt->getChemin() ;
  size_t iPosit = sContexte.find(pObjet->sCase);

  if (iPosit != NPOS)
  {
    size_t iTailleCase = strlen(pObjet->sCase.c_str()) ;
    size_t iTailleCtx  = strlen(sContexte.c_str()) ;

    string sDepass = string("") ;
    if (iPosit + iTailleCase > iTailleCtx)
      sDepass = string(sContexte, iPosit + iTailleCase, iTailleCtx - iPosit - iTailleCase) ;

    sChemin = string(pObjet->pCaptElemnt->getChemin(), 0, iPosit) + (*prIt)->sCase + sDepass ;
  }

  SOAPObject* pNewObject = new SOAPObject(*pObjet);  if (pObjet->pCaptElemnt)
  {
    NSCapture* pNewCaptElemnt = new NSCapture(*(pObjet->pCaptElemnt));

    pNewCaptElemnt->setChemin(sChemin) ;
    pNewCaptElemnt->setClassifier(sClassifier) ;
    pNewCaptElemnt->setLibelle(sLibelle) ;

    pNewObject->pCaptElemnt = pNewCaptElemnt ;

    pDocSOAP->pContexte->getEpisodus()->CaptureArray.push_back(pNewCaptElemnt) ;
  }

  pNewObject->sTexteInitial   = sLibelle;
  pNewObject->sClassification = sClassifier;

  pDocSOAP->addToTank(pNewObject);
}
catch (...)
{
    erreur("Exception NSSOAPListWindow::EvDupli.", standardError, 0) ;
}
}

voidNSSOAPListWindow::EvDupliquer()
{
try
{
	int iFocus = getFocusedItem() ;

	TListWindItem Item ;
  Item.SetIndex(iFocus) ;
  Item.SetSubItem(0) ;
  Item.mask = LVIF_PARAM ;

  if (!(GetItem(Item)))
  	return ;

  int iParams = Item.lParam;

  SOAPObject* pObjet = pObjectList->donneObjet(iParams);
  if (!pObjet)
  	return ;

  if (pDocSOAP->pPrincipes->empty())
		return ;

  RECT itemRect ;
  GetItemRect(iFocus, &itemRect, Bounds) ;

  NS_CLASSLIB::TPoint lp = NS_CLASSLIB::TPoint(itemRect.left, itemRect.top) ;

  TPopupMenu menu ;  PrinciplesIter prIt = pDocSOAP->pPrincipes->begin();
  for (int i =1; prIt != pDocSOAP->pPrincipes->end(); prIt++, i++)
  {
  	if (pObjet->sCase != (*prIt)->sCase)
    {
    	switch (i)
      {
      	case 1 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE1, (*prIt)->sLibelle.c_str()); break;
        case 2 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE2, (*prIt)->sLibelle.c_str()); break;
        case 3 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE3, (*prIt)->sLibelle.c_str()); break;
        case 4 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE4, (*prIt)->sLibelle.c_str()); break;
        case 5 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE5, (*prIt)->sLibelle.c_str()); break;
        case 6 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE6, (*prIt)->sLibelle.c_str()); break;
        case 7 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE7, (*prIt)->sLibelle.c_str()); break;
        case 8 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE8, (*prIt)->sLibelle.c_str()); break;
        case 9 : menu.AppendMenu(MF_STRING, IDC_DUPLIQUE9, (*prIt)->sLibelle.c_str()); break;
      }
    }
  }

  ClientToScreen(lp);
  menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
}
catch (...)
{
	erreur("Exception NSSOAPListWindow::EvDupliquer.", standardError, 0) ;
}
}

void
NSSOAPListWindow::EvCloner()
{
try
{
    int iFocus = getFocusedItem();

    TListWindItem Item;
    Item.SetIndex(iFocus);
    Item.SetSubItem(0);
    Item.mask = LVIF_PARAM;

    if (!(GetItem(Item)))
        return;

    int iParams = Item.lParam;

    SOAPObject* pObjet = pObjectList->donneObjet(iParams);

    if (!pObjet)        return;

    SOAPObject* pNewObject = new SOAPObject(*pObjet);

    if (pObjet->pCaptElemnt)
    {
        NSCapture* pNewCaptElemnt = new NSCapture(*(pObjet->pCaptElemnt));

        pNewObject->pCaptElemnt = pNewCaptElemnt;

        pDocSOAP->pContexte->getEpisodus()->CaptureArray.push_back(pNewCaptElemnt);
    }

    pDocSOAP->addToTank(pNewObject);
}
catch (...)
{
    erreur("Exception NSSOAPListWindow::EvCloner.", standardError, 0) ;
}
}

void
NSSOAPListWindow::EvLibelle()
{
try
{
  int iFocus = getFocusedItem();

  TListWindItem Item;
  Item.SetIndex(iFocus);
  Item.SetSubItem(0);
  Item.mask = LVIF_PARAM;

  if (!(GetItem(Item)))
      return;

  int iParams = Item.lParam;

  SOAPObject* pObjet = pObjectList->donneObjet(iParams);
  if ((SOAPObject*) NULL == pObjet)
    return ;

  string sLibelle = pObjet->sTexteInitial;

  NSSOAPLibelleDlg* pLibDlg = new NSSOAPLibelleDlg(0, pDocSOAP->pContexte, &sLibelle);
  int iResult = pLibDlg->Execute() ;
  delete pLibDlg ;

  if (IDOK != iResult)    return ;

  if (pObjet->pCaptElemnt)
  {
    NSCapture* pCaptured = pObjet->pCaptElemnt ;

    pCaptured->setLibelle(sLibelle) ;

    string sClassif = pCaptured->getClassifier() ;
    ParseSOAP Parser(pDocSOAP->pContexte, sClassif, string("")) ;

    string sLibelle        = pCaptured->getLibelle() ;
    string sChemin         = pCaptured->getChemin() ;
    string sClassifResultO = pCaptured->getClassifResultO() ;
    string sClassifResultP = pCaptured->getClassifResultP() ;
    string sClassifResultI = pCaptured->getClassifResultI() ;    string sClassifResult3 = pCaptured->getClassifResult3() ;

    Parser.computeParsing(&sLibelle,
                          &sChemin,
                          &sClassifResultO,
                          &sClassifResultP,
                          &sClassifResultI,                          &sClassifResult3);    pCaptured->setLibelle(sLibelle) ;    pCaptured->setChemin(sChemin) ;
    pCaptured->setClassifResultO(sClassifResultO) ;
    pCaptured->setClassifResultP(sClassifResultP) ;
    pCaptured->setClassifResultI(sClassifResultI) ;    pCaptured->setClassifResult3(sClassifResult3) ;
    pObjet->initFromCapture(pCaptured) ;
  }

  pObjet->sTexteInitial = sLibelle;

  Item.SetText(pObjet->sTexteInitial.c_str());

  pDocSOAP->donneChamp(pObjet);
  Item.SetImageIndex(pObjet->iImageIndex);

  Item.mask = LVIF_TEXT | LVIF_IMAGE;
  SetItem(Item);

  RedrawItems(iFocus, iFocus);
  UpdateWindow();
}
catch (...)
{
    erreur("Exception NSSOAPListWindow::EvClonerEvLibelle.", standardError, 0) ;
}
}

bool
NSSOAPListWindow::VnIsWindow(HWND hWnd)
{
  	return (HWindow == hWnd);
}

voidNSSOAPListWindow::EvSetFocus(HWND hWndLostFocus)
{
	pParentWindow->SetFocus();
}

void
NSSOAPListWindow::EvMove(ClassLib::TPoint& clientOrigin)
{
    TListWindow::EvMove(clientOrigin);
    if (!pObjectList)
    {
        NSSOAPView* pView = pDocSOAP->pContexte->getEpisodus()->pSOAPView ;
        if (pView)
            pView->gotMoved();
    }
}

void
NSSOAPListWindow::EvWindowPosChanged(WINDOWPOS far& windowPos)
{
    TListWindow::EvWindowPosChanged(windowPos);
    if (!pObjectList)
    {
        NSSOAPView* pView = pDocSOAP->pContexte->getEpisodus()->pSOAPView ;
        if (pView)
            pView->gotMoved();
    }
}

int
NSSOAPListWindow::getFocusedItem()
{
    return GetNextItem(-1, Focused);
}

int
NSSOAPListWindow::getIndex(int iParam)
{
    TLwFindInfo TfindInfo;
    TfindInfo.SetData(iParam);
    return FindItem(-1, &TfindInfo);
}

void    NSSOAPListWindow::DispInfoSOAP(TLwDispInfoNotify& dispInfo)
{
/*
    const int       BufLen = 255;
    static char     buffer[BufLen];
    TListWindItem   &dispInfoItem = *(TListWindItem*)&dispInfo.item;

    string sLang = "";
    if ((pContexte) && (pContexte->getUtilisateur()))
        sLang = pContexte->getUserLanguage() ;

    int index = dispInfoItem.GetIndex();

    if (pObjectList && !(pObjectList->empty()) && (pObjectList->size() < index))
       dispInfoItem.SetText((*(pObjectList->begin() + index))->sTexteInitial.c_str());
*/
}

//// drag-and-drop
//
bool
NSSOAPListWindow::LvnBeginDrag(TLwNotify& nmHdr)
{
try
{
	// Au sein de la SOAPView, le premier élément est fictif
  NSSOAPView* pview = TYPESAFE_DOWNCAST(pParentWindow, NSSOAPView) ;
  if ((pview) && (nmHdr.iItem == 0))
  	return false ;

  if (!pObjectList)
  	return false ;

  pDocSOAP->bDragAndDrop = true ;

  NS_CLASSLIB::TRect itemRect ;
  TListWindow::GetItemRect(nmHdr.iItem, &itemRect, Bounds) ;

  pDocSOAP->ImgDragDrop = new TImageList(CreateDragImage(nmHdr.iItem, &itemRect.TopLeft())) ;
  pDocSOAP->ImgDragDrop->BeginDrag(0, itemRect.Width() + nmHdr.iItem, itemRect.Height()) ;
  pDocSOAP->ImgDragDrop->DragEnter(*this, nmHdr.ptAction.x, nmHdr.ptAction.y) ;

  pDocSOAP->pSrcDragDrop = this ;

  TListWindItem Item ;
  Item.SetIndex(nmHdr.iItem) ;
  Item.SetSubItem(0) ;
  Item.mask = LVIF_PARAM ;

  if (!(GetItem(Item)))
  	return false ;

  int iParams = Item.lParam ;

  pDocSOAP->pObject2Drop = pObjectList->donneObjet(iParams) ;

  SetTimer(ID_TLISTTIMER, 60) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSSOAPListWindow::LvnBeginDrag.", standardError, 0) ;
  return false ;
}
}

void    NSSOAPListWindow::EvTimer(uint id)
{
  if (pDocSOAP->bDragAndDrop && (id == ID_TLISTTIMER))
  {
    NS_CLASSLIB::TPoint pt ;
    GetCursorPos(pt) ;
    ScreenToClient(pt) ;

    // Draw the image at the cursor's position
    TLwHitTestInfo hitTestInfo ;
    hitTestInfo.pt = pt ;

    pDocSOAP->ImgDragDrop->DragMove(pt.x, pt.y) ;
  }
}

voidNSSOAPListWindow::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
/*
    if (pDocSOAP->bDragAndDrop)
    {
        ScreenToClient(point);
        TLwHitTestInfo hitTestInfo;
        hitTestInfo.pt = point;

        pDocSOAP->ImgDragDrop->DragMove(point.x, point.y);
    }
*/
}

voidNSSOAPListWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& pt)
{
    // int count = GetItemCount();
	TLwHitTestInfo info(pt);
    /*int index =*/ HitTest(info);

    TListWindow::EvLButtonDown(modKeys, pt);
}

voidNSSOAPListWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt)
{
    if (pDocSOAP->bDragAndDrop)
    {
        pDocSOAP->pDstDragDrop = this;

        // acceptation des données par la fenêtre destination
        // si true alors demande d'effacer les données de la fenêtre source

        if (pDocSOAP->acceptDragDrop(pDocSOAP->pObject2Drop))
            pDocSOAP->deleteItemDragDrop(pDocSOAP->pObject2Drop);

        pDocSOAP->ImgDragDrop->DragLeave(*this);
        pDocSOAP->ImgDragDrop->EndDrag();
        delete pDocSOAP->ImgDragDrop;
        pDocSOAP->ImgDragDrop = 0;
        ReleaseCapture();
        pDocSOAP->bDragAndDrop = false;

    	KillTimer(ID_TLISTTIMER);
//    	Update();
    }
}

//-----------------------------------------------------------------------
// double click sur un objet
//-----------------------------------------------------------------------
void
NSSOAPListWindow::EvLButtonDblClk(uint /*modkeys*/, NS_CLASSLIB::TPoint& point)
{
    //
    // On trouve l'index de l'élément (s'il existe) au point de click
    //
    TLwHitTestInfo info(point);
    int index = HitTest(info);

    if (index == -1)
        return;

    //
    // On trouve l'objet qui correspond
    //
    if (!pObjectList)
        return;

    TListWindItem Item;
    Item.SetIndex(index);
    Item.SetSubItem(0);
    Item.mask = LVIF_PARAM;

    if (!(GetItem(Item)))
        return;

    int iParams = Item.lParam;

    SOAPObject* pObjet = pObjectList->donneObjet(iParams);
    if (!pObjet)
        return;

    classifExpert* pExpert = pDocSOAP->pContexte->getEpisodus()->pClassifExpert ;

    string sResult = pExpert->chooseCode(pObjet);
    if (sResult != "")
    {
        pObjet->sCode       = sResult;
        pObjet->iConfidence = 100;
        string sLib = pObjet->donneLibelle();

        pDocSOAP->donneChamp(pObjet);

        Item.SetText(sLib.c_str());
        Item.SetImageIndex(pObjet->iImageIndex);
        Item.mask = LVIF_TEXT | LVIF_IMAGE;
        SetItem(Item);

        Invalidate();
    }
}

//-----------------------------------------------------------------------
// click droit de la souris sur un objet
//-----------------------------------------------------------------------
void
NSSOAPListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
try
{
    EvLButtonDown(modkeys, point);
    //
    // On trouve l'index de l'élément (s'il existe) au point de click
    //
    TLwHitTestInfo info(point);
    int index = HitTest(info);

    if (index == -1)
        return;

    /*
    SetItemState(index, LVIS_FOCUSED | LVIS_SELECTED, LVIF_STATE);

    pContexte->getSuperviseur()->pNSApplication->PumpWaitingMessages();
    */

    //
    // On trouve l'objet qui correspond
    //
    if (!pObjectList)
        return;

    TListWindItem Item;
    Item.SetIndex(index);
    Item.SetSubItem(0);
    Item.mask = LVIF_PARAM;

    if (!(GetItem(Item)))        return;

    int iParams = Item.lParam;
    SOAPObject* pObjet = pObjectList->donneObjet(iParams);    if (!pObjet)
        return;

    NS_CLASSLIB::TPoint lp = point;    TPopupMenu *menu = new TPopupMenu();

    menu->AppendMenu(MF_STRING, IDC_CODE,       "Code");
    menu->AppendMenu(MF_STRING, IDC_LIBELLE,    "Libellé");

    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;

    if (!(pDocSOAP->pPrincipes->empty()))
    {
        PrinciplesIter prIt = pDocSOAP->pPrincipes->begin();
        for (int i =1; prIt != pDocSOAP->pPrincipes->end(); prIt++, i++)
        {
            if (pObjet->sCase != (*prIt)->sCase)
            {
                switch (i)
                {
                    case 1 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE1, (*prIt)->sLibelle.c_str()); break;
                    case 2 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE2, (*prIt)->sLibelle.c_str()); break;
                    case 3 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE3, (*prIt)->sLibelle.c_str()); break;
                    case 4 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE4, (*prIt)->sLibelle.c_str()); break;
                    case 5 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE5, (*prIt)->sLibelle.c_str()); break;
                    case 6 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE6, (*prIt)->sLibelle.c_str()); break;
                    case 7 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE7, (*prIt)->sLibelle.c_str()); break;
                    case 8 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE8, (*prIt)->sLibelle.c_str()); break;
                    case 9 : menu->AppendMenu(MF_STRING, IDC_PRINCIPE9, (*prIt)->sLibelle.c_str()); break;
                }
            }
        }
    }

    // menu->AppendMenu(MF_STRING, IDC_POSITION,   "Position");

    menu->AppendMenu(MF_SEPARATOR, 0, 0);

    menu->AppendMenu(MF_STRING, IDC_DUPLIQU,    "Dupliquer");
    menu->AppendMenu(MF_STRING, IDC_CLONE,      "Cloner");

  	ClientToScreen(lp);
  	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
  	delete menu;
}
catch (...)
{
    erreur("Exception NSSOAPListWindow::EvRButtonDown.", standardError, 0) ;
}
}


voidNSSOAPListWindow::logMessage(string sLogMessage)
{
  ifstream inFile ;
  ofstream outFile ;
  string   line ;
  string   sLastLogMessage ;

  inFile.open("SOAP.log") ;
  if (inFile)
  {
    while (!inFile.eof())
    {
      getline(inFile,line);
      if (line != "")
        sLastLogMessage += line + "\n" ;
    }

    inFile.close() ;
  }
  else // cas fichier vide
    sLastLogMessage = "Nautilus log : " ;

  // on ajoute la nouvelle erreur
  sLastLogMessage += sLogMessage + "\n" ;

  // On écrit le nouveau fichier reqerror.log
  outFile.open("SOAP.log") ;
	if (!outFile)
  {
    // cas d'erreur bloquant...
    string sCaption = string("Message ") + pDocSOAP->pContexte->getSuperviseur()->getAppName().c_str();
    MessageBox(/*NULL,*/ "Erreur d'ouverture en écriture du fichier SOAP.log.", sCaption.c_str(), MB_ICONEXCLAMATION);
    return;
  }

  for (size_t i = 0; i < strlen(sLastLogMessage.c_str()); i++)
    outFile.put(sLastLogMessage[i]) ;

  outFile.close() ;
}

// --------------------------------------------------------------------------
//
//                          Class SOAPListWinArray
//
// --------------------------------------------------------------------------

SOAPListWinArray::SOAPListWinArray()
                 :ArraySOAPLW()
{
  bOwner = true ;
}


NSSOAPListWindow*
SOAPListWinArray::trouveList(SOAPBasket* pBasket, ClassificationPrinciple* pPrincipe)
{
  if (empty())
    return (NSSOAPListWindow*) 0 ;

  for (ArraySOAPListWindowIter i = begin() ; end() != i ; i++)
    if (((*i)->pBasket == pBasket) && ((*i)->pPrincipe == pPrincipe))
      return *i ;

  return (NSSOAPListWindow*) 0 ;
}

SOAPListWinArray::SOAPListWinArray(const SOAPListWinArray &rv)
                 :ArraySOAPLW()
{
try
{
  if (rv.empty())
    return ;

  bOwner = false ;

  for (ArraySOAPListWindowConstIter i = rv.begin() ; rv.end() != i ; i++)
    push_back(*i) ;
}
catch (...)
{
  erreur("Exception SOAPListWinArray copy ctor.", standardError, 0) ;
}
}

SOAPListWinArray::~SOAPListWinArray()
{
  vider() ;
}

SOAPListWinArray&
SOAPListWinArray::operator=(const SOAPListWinArray& src)
{
try
{
  if (this == &src)
    return *this ;

  vider() ;

  if (src.empty())
    return *this ;

  bOwner = false ;

  for (ArraySOAPListWindowConstIter i = src.begin() ; src.end() != i ; i++)
    push_back(*i) ;

  return *this ;
}
catch (...)
{
  erreur("Exception SOAPListWinArray (=).", standardError, 0) ;
  return *this ;
}
}

void
SOAPListWinArray::vider()
{
  if (empty())
    return ;

  for (ArraySOAPListWindowIter i = begin() ; end() != i ; )
  {
    if (bOwner)
      delete *i ;
    erase(i) ;  }}

// --------------------------------------------------------------------------
//
//                          Class NSSOAPLibelleDlg
//
// --------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSSOAPLibelleDlg, TDialog)    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSSOAPLibelleDlg::NSSOAPLibelleDlg(TWindow *parent, NSContexte *pCtx, string *psLib)
                 :TDialog(parent, "CAPTURE_LIBELLE"), NSRoot(pCtx)
{
  pLibelEdit = new OWL::TEdit(this, IDC_CHGLIBELLE) ;
  psLibelle  = psLib ;
}

NSSOAPLibelleDlg::~NSSOAPLibelleDlg(){
  delete pLibelEdit ;
}

void
NSSOAPLibelleDlg::SetupWindow()
{
    TDialog::SetupWindow();

    pLibelEdit->SetText(psLibelle->c_str());
}

void
NSSOAPLibelleDlg::CmOk()
{
    char    sSOAPEdit[1024];

    // on récupère l'info du premier champs Edit
    pLibelEdit->GetText(sSOAPEdit, 1024);
    *psLibelle = string(sSOAPEdit);

    TDialog::CmOk();
}

void
NSSOAPLibelleDlg::CmCancel()
{
    TDialog::CmCancel();
}

// --------------------------------------------------------------------------
//
//                          Class NSSOAPDlg
//
// --------------------------------------------------------------------------


DEFINE_RESPONSE_TABLE1(NSSOAPDlg, TDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_COMMAND(IDCONVERT, CmConvert),
END_RESPONSE_TABLE;


NSSOAPDlg::NSSOAPDlg(TWindow *parent, NSContexte *pCtx)
          :TDialog(parent, "SOAPDIALOG"), NSRoot(pCtx)
{
    pSOAPEdit1  = new OWL::TEdit(this, IDC_SOAPEDIT) ;
    pSOAPEdit2  = new OWL::TEdit(this, IDC_SOAPEDIT2) ;
    pSOAPEdit3  = new OWL::TEdit(this, IDC_SOAPEDIT3) ;
    psSOAPEdit  = new string();
}


NSSOAPDlg::NSSOAPDlg(TWindow *parent, NSContexte *pCtx, string *psdlg)          :TDialog(parent, "SOAPDIALOG"), NSRoot(pCtx)
{
  pSOAPEdit1 = new OWL::TEdit(this, IDC_SOAPEDIT) ;
  pSOAPEdit2 = new OWL::TEdit(this, IDC_SOAPEDIT2) ;
  pSOAPEdit3 = new OWL::TEdit(this, IDC_SOAPEDIT3) ;
  psSOAPEdit = psdlg ;
}

NSSOAPDlg::~NSSOAPDlg(){
	delete psSOAPEdit ;
  delete pSOAPEdit1 ;
  delete pSOAPEdit2 ;
  delete pSOAPEdit3 ;
}

void    NSSOAPDlg::CmOk(){
	char sSOAPEdit[1024] ;

  // on récupère l'info du premier champs Edit
  pSOAPEdit1->GetText(sSOAPEdit, 1024);
  *psSOAPEdit = string(sSOAPEdit);
  NSEpiClasser *pEpiClasser = new NSEpiClasser(pContexte->getSuperviseur()) ;

  ParseCategory *pElemParser1 = new ParseCategory(3, "6CISP", "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 0123456789");
  ParseCategory *pElemParser2 = new ParseCategory(3, "6CISP", "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 0123456789");
  ParseCategory *pElemParser3 = new ParseCategory(3, "6CISP", "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 0123456789");

  ParseSOAP   *pSOAPParser = new ParseSOAP(pContexte, *psSOAPEdit, string("")) ;
  string      *pParsingResult;

  string      sCodeSens1 ;
  string      sCodeSens2 ;
  string      sCodeSens3 ;

  TermsParser *pTheParser = new TermsParser(pContexte, string("6CISP"), string("")) ;

  if (pSOAPParser->ParsingEq())
  {
    string s2DefO = string("") ;
    string s2DefP = string("") ;
    string s2DefI = string("") ;
    string s2Def3 = string("") ;

    pParsingResult = pSOAPParser->getCodeParsing() ;
    pContexte->getDico()->donneCodeSens(pParsingResult, &sCodeSens1) ;

    pEpiClasser->donneRelations(&s2DefO, sCodeSens1, "6CISP", "O") ;
    pEpiClasser->donneRelations(&s2DefP, sCodeSens1, "6CISP", "P") ;
    pEpiClasser->donneRelations(&s2DefI, sCodeSens1, "6CISP", "I") ;

    pElemParser1->DefCritere(s2DefO, s2DefP, s2DefI, s2Def3) ;

    pTheParser->push_back(pElemParser1) ;
  }

  // on récupère l'info du deuxième champs Edit
  pSOAPEdit2->GetText(sSOAPEdit, 1024);
  *psSOAPEdit = string(sSOAPEdit);
  pSOAPParser->ChangeSOAPEdit(psSOAPEdit);

  if (pSOAPParser->ParsingEq())
  {
    string s2DefO = string("") ;
    string s2DefP = string("") ;
    string s2DefI = string("") ;
    string s2Def3 = string("") ;

    pParsingResult = pSOAPParser->getCodeParsing() ;
    pContexte->getDico()->donneCodeSens(pParsingResult, &sCodeSens2) ;

    pEpiClasser->donneRelations(&s2DefO, sCodeSens2, "6CISP", "O") ;
    pEpiClasser->donneRelations(&s2DefP, sCodeSens2, "6CISP", "P") ;
    pEpiClasser->donneRelations(&s2DefI, sCodeSens2, "6CISP", "I") ;

    pElemParser2->DefCritere(s2DefO, s2DefP, s2DefI, s2Def3) ;

    pTheParser->push_back(pElemParser2) ;
  }

  pSOAPEdit3->GetText(sSOAPEdit, 1024);
  *psSOAPEdit = string(sSOAPEdit);
  pSOAPParser->ChangeSOAPEdit(psSOAPEdit);

  if (pSOAPParser->ParsingEq())
  {
    string s2DefO = string("") ;
    string s2DefP = string("") ;
    string s2DefI = string("") ;
    string s2Def3 = string("") ;

    pParsingResult = pSOAPParser->getCodeParsing() ;
    pContexte->getDico()->donneCodeSens(pParsingResult, &sCodeSens3) ;

    pEpiClasser->donneRelations(&s2DefO, sCodeSens3, "6CISP", "O") ;
    pEpiClasser->donneRelations(&s2DefP, sCodeSens3, "6CISP", "P") ;
    pEpiClasser->donneRelations(&s2DefI, sCodeSens3, "6CISP", "I") ;

    pElemParser3->DefCritere(s2DefO, s2DefP, s2DefI, s2Def3) ;

    pTheParser->push_back(pElemParser3) ;
  }

  ElemSetArray *pResult = new ElemSetArray() ;
  pResult = pTheParser->Compute() ;

    //ElemSetArray *pDomain1 = new ElemSetArray() ;
    //ElemSetArray *pDomain2 = new ElemSetArray() ;
    //ElemSetArray *pDomain3 = new ElemSetArray() ;
    //pIntersection12 = pDomain1->ComputeIntersection(pDomain2);
    //ElemSetArray *pIntersection123 = pIntersection12->ComputeIntersection(pDomain3);
    //pIntersection12->ComputeUnion() ;
  pResult->print("** RESULT ");

    // --------------------------------------------------------------------
    // il va falloir faire l'opération précédente avec les 3 champs Edit
    // puis récupérer les codes correspondant à chaque champs Edit
    // pour chacun des 3 critères (Obligatoire, Possible, Interdit)
    // enfin on fait le calcul des intersections entre ces trois domaines
    // et on donne le résultat
    // --------------------------------------------------------------------

    // temporaire
/*
    ParseCategory *pCategoryParser = new ParseCategory(3, "CISP", "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 0123456789");
    ElemSetArray *pDomain1 = new ElemSetArray() ;
    ElemSetArray *pDomain2 = new ElemSetArray() ;
    pDomain1 = pCategoryParser->DefDomain(string("P75-R14;S12-S98;W89-Z12;S87-T47;A?7-A?9"));
    pDomain1->print("** domain 1 :");
    pDomain2 = pCategoryParser->DefDomain(string("A11-A13;P83-T20;?12"));
    ElemSet *pAnElem = new ElemSet("P83-T20");
    ElemSetArray *pIntersection = pDomain1->ComputeIntersection(pDomain2);
    pIntersection->ComputeUnion();
    pIntersection->print("** intersection");
*/
    TDialog::CmOk();
}

void
NSSOAPDlg::CmCancel()
{
  TDialog::CmCancel() ;
}

void    NSSOAPDlg::CmConvert(){
//    string  *sResult = new string() ;
  ParseSOAP Parser(pContexte, string("6CISP"), string("")) ;

  string sLibelle = string("phrase de test");
  string sClassifResultO = string("") ;
  string sClassifResultP = string("") ;
  string sClassifResultI = string("") ;
  string sClassifResult3 = string("") ;

  Parser.searchTermsInLexique(&sLibelle, &sClassifResultO, &sClassifResultP, &sClassifResultI, &sClassifResult3) ;

/*
    pParser->computeParsing(&(string("oeil")), &(string("")), sResult);

    ofstream    outFile ;
    outFile.open("resultTestSOAP.log") ;
    if (outFile)
        outFile.write(sResult->c_str(), strlen(sResult->c_str())) ;
    outFile.close() ;
*/
/*

//  remplissage de la table CLASSER.DB

    ifstream    inFile;
    ofstream    outFile;
    string      line;

    inFile.open("index.txt");
   	outFile.open("index-no_concordance.log");

    if (inFile && outFile)
    {
   	    while (!inFile.eof())
        {
            getline(inFile, line) ;
            if (!convert(line))
            {
                string temp = line + "\n" ;
                outFile.write(temp.c_str(), strlen(temp.c_str())) ;
            }
        }
        outFile.close() ;
        inFile.close() ;
    }
*/
}

boolNSSOAPDlg::convert(string line){
	if (line == "")
  	return false ;

  // position entre les deux index
  size_t iPos = line.find("\t") ;

  // position juste avant le libellé
  iPos = line.find("\t", iPos + 1) ;
  size_t iBeginLibelle = iPos + 1 ;
  // position juste avant le code CISP2
  iPos = line.find("\t", iPos + 1) ;
  size_t  iEndLibelle = iPos ;
  size_t  iBeginRelation = iPos + 1 ;
  size_t  iEndRelation = strlen(line.c_str()) ;

  string  sRelation   = string(line, iBeginRelation, iEndRelation - iBeginRelation) ;
  string  sLibelle    = string(line, iBeginLibelle, iEndLibelle - iBeginLibelle) ;

  ParseSOAP SOAPParser(pContexte, sLibelle, string("")) ;

  if (SOAPParser.ParsingEq())
  {
  	string *pParsingResult = SOAPParser.getCodeParsing() ;
    string  sCodeSens ;
    pContexte->getDico()->donneCodeSens(pParsingResult, &sCodeSens) ;
    bool bResult = newFicheClasserC2(sCodeSens, sRelation) ;
    return (bResult) ;
  }

  return false ;
}
boolNSSOAPDlg::newFicheClasserC2(string sCode, string sRelation){
	NSEpiClasser Classer(pContexte->getSuperviseur()) ;
	bool bRempli = false ;

	Classer.lastError = Classer.open() ;
  if (Classer.lastError != DBIERR_NONE)
  {
  	erreur("CLASSER.DB -- Erreur à l'ouverture du fichier CLASSER.DB", standardError, Classer.lastError, pContexte->GetMainWindow()->GetHandle());
		return false ;
  }

	Classer.lastError = Classer.debut(dbiWRITELOCK) ;
  if ((Classer.lastError != DBIERR_NONE) && (Classer.lastError != DBIERR_EOF))
	{
		erreur("CLASSER.DB -- Erreur de positionnement dans la base CLASSER.DB.", standardError, Classer.lastError, pContexte->GetMainWindow()->GetHandle());
		Classer.close() ;
		return false ;
	}

  while ((Classer.lastError != DBIERR_EOF) && (!bRempli))
  {
  	Classer.lastError = Classer.getRecord() ;
		if (Classer.lastError != DBIERR_NONE)
		{
			erreur("CLASSER.DB -- Erreur de lecture dans la base CLASSER.DB.", standardError, Classer.lastError, pContexte->GetMainWindow()->GetHandle());
			Classer.close() ;
			return false ;
		}

    // on se positionne sur le message suivant
    Classer.lastError = Classer.suivant(dbiWRITELOCK) ;
		if ((Classer.lastError != DBIERR_NONE) && (Classer.lastError != DBIERR_EOF))
    {
			erreur("CLASSER.DB -- Erreur d'accès au message suivant.", standardError, Classer.lastError, pContexte->GetMainWindow()->GetHandle());
      Classer.close() ;
			return false ;
    }

    if (Classer.getCode() == sCode)
    {
    	string sTempRelation = Classer.getRelation() ;
      if (sTempRelation.find(sRelation) == NPOS)
      {
      	sRelation = sRelation + ";" + Classer.getRelation() ;
        Classer.setRelation(sRelation) ;
        Classer.modifyRecord(true) ;
      }
      bRempli = true ;
      break ;
    }
  }

  if (!bRempli)
  {
  	Classer.setCode(sCode) ;
    Classer.setClassification("6CISP") ;
    Classer.setCriteria("P") ;
    Classer.setRelation(sRelation) ;

    // on l'insère dans la base Messages
    Classer.lastError = Classer.appendRecord() ;
    if (Classer.lastError != DBIERR_NONE)
    {
    	erreur("CLASSER.DB -- Erreur à l'insertion du message dans la base CLASSER.DB.", standardError, Classer.lastError, pContexte->GetMainWindow()->GetHandle());
      Classer.close() ;
      return false ;
    }
  }

  Classer.lastError = Classer.close() ;
  if (Classer.lastError != DBIERR_NONE)
		erreur("CLASSER.DB -- Erreur de fermeture de la base CLASSER.DB.", standardError, Classer.lastError, pContexte->GetMainWindow()->GetHandle());

	return true ;
}

