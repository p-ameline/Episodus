// -----------------------------------------------------------------------------
// nsprocessview.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.9 $
// $Author: pameline $
// $Date: 2013/07/21 17:05:13 $
// -----------------------------------------------------------------------------
// gestion des process
// -----------------------------------------------------------------------------
// FLP - aout 2003
// -----------------------------------------------------------------------------

#include "nssavoir\nsBdmDriver.h"
// #include "nautilus\nsanxary.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nsprocessview.h"
#include "nsldv\nsldvvar.h"
#include "nsldv\nsldvgoal.h"
#include "nsbb\nspanesplitter.h"
#include "nautilus\nshistdo.h"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nsbbsmal.h"

// -----------------------------------------------------------------------------
//
// Class NSProcessView
//
// -----------------------------------------------------------------------------

const int ID_ProcessList = 0x100 ;

// Table de réponses de la classe NSProcessView
DEFINE_RESPONSE_TABLE1(NSProcessView, NSMUEView)
  EV_VN_ISWINDOW,
  EV_LVN_GETDISPINFO(ID_ProcessList,  DispInfoListe),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_COMMAND(CM_PROCESS_NEW,          CmNouveau),
  EV_COMMAND(CM_PROCESS_STEP,         CmEtape),
  EV_COMMAND(CM_PROCESS_CHANGE,       CmModifier),
  EV_COMMAND(CM_PROCESS_STOP,         CmArreter),
//  EV_COMMAND(IDC_ORDONNANCE,          CmOrdonnance),
END_RESPONSE_TABLE;


// Constructeur
NSProcessView::NSProcessView(NSLdvDocument &doc, string sConcern)
  : NSMUEView(doc.pContexte, &doc, 0, string("ProcessManagement"), string("LdvDoc"), sConcern),
    aCurrentProcesses(&doc)
{
  pLdVDoc = &doc;
  pListeWindow = new NSProcessesPropertyWindow(this, ID_ProcessList, 0, 0, 0, 0);

	initMUEViewMenu("menubar_process") ;

  pToolBar = 0 ;
  bSetupToolBar = true ;

  // Initialisation des prescriptions en cours
  initCurentProcesses() ;

  setViewName() ;
}


// Destructeur
NSProcessView::~NSProcessView()
{
}


void
NSProcessView::setViewName()
{
	_sViewName = pContexte->getSuperviseur()->getText("processManagement", "processViewTitle") ;

  addConcernTitle() ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow
*NSProcessView::GetWindow()
{
  return (TWindow *) this ;
}

// Appel de la fonction de remplissage de la vuevoid
NSProcessView::SetupWindow()
{
  NSMUEView::SetupWindow() ;

  string sName = string("Procédures en cours") ;

  if (string("") != _sPreoccup)
  {
    LDVFRAME iFrame = ldvframeNotSpecified ;
    NSConcern *pConcern = pLdVDoc->getConcern(_sPreoccup, iFrame) ;
    if (pConcern)
      sName += string(" - ") + pConcern->_sTitre ;
  }

  Parent->SetCaption(sName.c_str()) ;

  SetupColumns() ;
  AfficheListe() ;
}

void
NSProcessView::CmNouveau()
{
	if (!(pContexte->userHasPrivilege(NSContexte::createProcess, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	string sChoixProcess ;

	NSNewTrt *pNewTrtDlg = new NSNewTrt(this, &sChoixProcess, _sPreoccup, pContexte, "GDMEX1", "Type de procédure") ;

	if (pNewTrtDlg->Execute() == IDCANCEL)
		return ;

  	// on vérifie qu'on a récupéré un code lexique
	if (string("") == sChoixProcess)
    	return ;

	// anciennement dans NSNewTrt
	NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
#ifdef __OB1__
	NSSmallBrother BigBoss(pContexte, &PPT, 0) ;
#else
	NSSmallBrother BigBoss(pContexte, &PPT, 0, false /* initFromBbk */) ;
#endif
	BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
	BigBoss.lance12("GDMEX1", sChoixProcess) ;

	if (PPT.empty())
    return ;

  // on créé un document procédure
 	NSRefDocument DocNewProcess(0, 0, 0, pContexte, false) ;
  DocNewProcess.setPatPatho(&PPT) ;

	// on référence le document procédure
	string  sTitleProcess ;
	if (sChoixProcess != string("£?????"))
		pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sChoixProcess, &sTitleProcess) ;
	if (!DocNewProcess.Referencer("ZCQ00", string("Demande de " + sTitleProcess), "", "", true, true))
		erreur("procédure non référencée", standardError, 0) ;

	DocNewProcess.enregistrePatPatho() ;

	string sDocRoot = pContexte->getPatient()->getNss() + LocalPatient  + string("00000") ;

	// une fois qu'on a sélectionné notre prescription, on doit créer un lien "en
	// attente" entre le noeud patient racine et la prescription
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

  NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;
  DocNewProcess._pDocInfo->initFromMeta(&PptMeta) ;

  string sRootNode = (*(PptMeta.begin()))->getNode() ;

	pGraphe->etablirLien(sDocRoot, NSRootLink::processWaitingFor, sRootNode) ;

	DocNewProcess.NSNoyauDocument::chargePatPatho() ;
  NSPatPathoArray PptNewProcess(pContexte->getSuperviseur()) ;
  DocNewProcess.initFromPatPatho(&PptNewProcess) ;

	pContexte->getPatient()->getDocHis()->Rafraichir(DocNewProcess._pDocInfo, &PptNewProcess) ;

	// si on traite les procédures au niveau des préoccupations de la LdV, il faut faire un lien avec la préoccupation
	if (string("") != _sPreoccup)
    pGraphe->etablirLien(sRootNode, NSRootLink::problemRelatedTo, _sPreoccup) ;

	displayChanges() ;
}

void
NSProcessView::CmEtape()
{
  // un process peut être dans un état où il a des résultats partiels
  // voir comment l'on gère ce cas avec les liens
  // NB : un process est une prescription (d'un examen, d'une analyse, ...) qui
  // est en attente de résultat
  // comment faire si un résultat est présent mais qu'il n'est pas complet ?
  // plusieurs solutions :  * créer un nouveau type de liens
  //                        * garder le lien en attente, mais créer quand même
  //                          un lien résultat vers le résultat partiel
  // deuxième solution semble plus adéquate
}


void
NSProcessView::CmModifier()
{
	if (!(pContexte->userHasPrivilege(NSContexte::modifyProcess, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

  // on récupère l'index dans la processview de l'item à modifier
  int index = pListeWindow->IndexItemSelect() ;

  /*
  if (pConcern == 0)
  {
    erreur("Le médicament doit être rattaché à une préoccupation particulière.", 0, 0) ;
    return ;
  }
  */

  if (index == -1)
  {
    erreur("Vous devez sélectionner un process à modifier dans la liste des processes en cours.", standardError, 0) ;
    return ;
  }

  // on récupère la référence du noeud de l'item sélectionné
  string  sNodeModif = (aCurrentProcesses[index])->getDocNoeud() ;

  if (pContexte->getPatient() && pContexte->getPatient()->getDocHis() && (false == pContexte->getPatient()->getDocHis()->getVectDocument()->empty()))
  {
  	DocumentIter iterDoc = pContexte->getPatient()->getDocHis()->getVectDocument()->begin() ;
	  for ( ; iterDoc != pContexte->getPatient()->getDocHis()->getVectDocument()->end() ; iterDoc++)
    {
      // on récupère le code du noeud document du document courant de
      // l'historique et on le compare avec celui de l'item sélectionné
      string  sCodeCurrentDoc = (*iterDoc)->getID() ;
      if (strncmp(sCodeCurrentDoc.c_str(), sNodeModif.c_str(), PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) == 0)
      {
        // on a trouvé le bon document, maintenant on veut ouvrir ce document
        // afin de pouvoir le modifier
        NSDocumentHisto *pDocument = static_cast<NSDocumentHisto *>(*iterDoc) ;
        if (!pDocument)
          return ;
        pContexte->getPatient()->getDocHis()->AutoriserEdition(pDocument) ;
        return ;
      }
    }
  }
}

void
NSProcessView::CmArreter()
{
	if (!(pContexte->userHasPrivilege(NSContexte::modifyProcess, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	// pour arreter un process, il faut lui attribuer un résultat
	// serait-il possible d'arreter un process sans qu'il est de résultat ?

	// on récupère l'index du process à arrêter
	int index = pListeWindow->IndexItemSelect() ;
	if (index == -1)
	{
		// il faut d'abord sélectionner un process
		erreur("Vous devez sélectionner un process à arreter dans la liste des processes en cours.", standardError, 0) ;
		return ;
	}

	// on récupère la référence sur le noeud correspondant au process sélectionné
	string  sDocNode  = (aCurrentProcesses[index])->getDocNoeud() ;
	string  sMetaNode = (aCurrentProcesses[index])->getMetaNoeud() ;

	// on récupère le noeud racine du patient
	string sDocRoot = pContexte->getPatient()->getNss() + string("_00000") ;

	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

	pGraphe->detruireLien(sDocRoot, NSRootLink::processWaitingFor, sMetaNode) ;

/*
//  pContexte->getGraphe()->detruireLien(sDocRoot, NSGraphe::processWaitingFor, sNodeModif) ;
  if (pContexte->getPatient() && pContexte->getPatient()->pDocHis && !pContexte->getPatient()->pDocHis->VectDocument.empty())
  {
  	DocumentIter iterDoc = pContexte->getPatient()->pDocHis->VectDocument.begin() ;
	  for ( ; iterDoc != pContexte->getPatient()->pDocHis->VectDocument.end() ; iterDoc++)
    {
      // on récupère le code du noeud document du document courant de
      // l'historique et on le compare avec celui de l'item sélectionné
      string  sCodeCurrentDoc = string((*iterDoc)->pDonnees->codePatient) + string((*iterDoc)->pDonnees->codeDocument) ;
      if (strncmp(sCodeCurrentDoc.c_str(), sNodeModif.c_str(), PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) == 0)
      {
        // on a trouvé le bon document, maintenant on veut ouvrir ce document
        // afin de pouvoir le récupérer son code META
        NSDocumentHisto *pDocument = static_cast<NSDocumentHisto *>(*iterDoc) ;
        if ((pDocument) && (pDocument->pMeta != NULL) && (!pDocument->pMeta->empty()) && (pDocument->pMeta->begin()))
        {
          string  sCodeProcess =  string((*(pDocument->pMeta->begin()))->pDonnees->codePatient) +
                                  string((*(pDocument->pMeta->begin()))->pDonnees->codeDocument) +
                                  string((*(pDocument->pMeta->begin()))->pDonnees->noeud) ;
          pContexte->getGraphe()->detruireLien(sDocRoot, NSGraphe::processWaitingFor, sCodeProcess) ;
        }
      }
    }
  }
*/
  // pour arreter un process, on doit détruire le lien processWaitingFor entre
  // le noeud racine du patient et la demande de process


  // on cloture la précédente prescription  //  pContexte->getPatient()->ArreterElement(sNodeModif, string(szDateJour)) ;

  initCurentProcesses() ;
  AfficheListe() ;
}


void
NSProcessView::CmOrdonnance()
{
// à voir - reflechir à comment faire une ordonnance
//  pContexte->getPatient()->CreeOrdonnance() ;
}


void
NSProcessView::displayChanges()
{
  initCurentProcesses() ;
  AfficheListe() ;
}


void
NSProcessView::initCurentProcesses()
{
	aCurrentProcesses.vider() ;

  	// la liste des processes n'est pas stockée dans le LdvDoc
  	// donc tout ce qui suit n'est pas utile
	// récupération de la référence du noeud racine patient
	string sDocRoot = pContexte->getPatient()->getNss() + LocalPatient + string("00000"); ;

	// on recherche tous les noeuds avec lequel le patient à un lien processWaitingFor
	// ces noeuds correspondent aux PPT META des documents concernant les processes
  NSPersonGraphManager* pPersonGraph = pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pPersonGraph)
    return ;
  NSLinkManager* pGraphe = pPersonGraph->getLinkManager() ;

	VecteurString VectString ;
	pGraphe->TousLesVrais(sDocRoot, NSRootLink::processWaitingFor, &VectString) ;
  //recuperation process liés au root par NSRootLink::processWaitingFor
 /*	if (pVecteurString->empty())
		return  ;    */
 	char pcNode[PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + PPD_NOEUD_LEN + 1] ;

  if (false == VectString.empty())
	{
  	for (EquiItemIter i = VectString.begin() ; i != VectString.end() ; i++)
		{
    	VecteurString VSprocess ;
			// on veut le noeud document donc on prend utilisateur + document
    	sprintf(pcNode, "%s", (*i)->c_str()) ;
    	pcNode[PAT_NSS_LEN + DOC_CODE_DOCUM_LEN] = 0 ;
    	pGraphe->TousLesVrais(pcNode, NSRootLink::docData, &VSprocess) ;

    	if (false == VSprocess.empty())
      {
      	for (EquiItemIter j = VSprocess.begin() ; j != VSprocess.end() ; j++)
        {
          NSPatPathoArray Tmp(pContexte->getSuperviseur()) ;
          string sRosace ;
          pPersonGraph->getTree(pcNode, &Tmp, &sRosace) ;
          if (false == Tmp.empty())
          {
            PatPathoIter pptIt = Tmp.begin() ;
          	string sNode = (*pptIt)->getNode() ;
          	sprintf(pcNode, "%s", sNode.c_str()) ;
          }

          //string sDocument = string(sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;
          // FAIRE DELETE pTmpPPT QUELQUE PART
          //
          NSPatPathoArray TmpPPT(pContexte->getSuperviseur()) ;
          pContexte->getPatient()->DonneArray(pcNode, &TmpPPT) ;

          if (TmpPPT.empty())
            break ;

          NSProcess NewProcess(pContexte) ;

          // on doit ajouter tous les noeuds qui ont un lien prescripWaitingFor à la
          // liste des processes courant (aCurrentProcesses)

          // on récupère le code lexique, le noeud, et le titre
          initProcessFromPatho(pcNode, &NewProcess, &TmpPPT, **i) ;
        	pGraphe->TousLesVrais(NewProcess.getMetaNoeud(), NSRootLink::problemRelatedTo, &(NewProcess.aConcerns)) ;

        	if ((string("") == _sPreoccup) || (NewProcess.isLinkedWithConcern(_sPreoccup)))
          	aCurrentProcesses.push_back(new NSProcess(NewProcess)) ;
      	} //for
			} //if
    } //for
  } //if
  else
  {
    VecteurString ObjProcessVect ;

    NSFrameInformationArray *pFrames = pLdVDoc->getFrames() ;
    if ((NSFrameInformationArray*) NULL == pFrames)
      return ;
    for (int i = 0 ; i < FRAMECOUNT ; i++)
    {
      NSFrameInformation *pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
      if (pFrameInfo)
      {
  	    //recuperation des process liés aux objectifs
        ArrayGoals *pLdvGoals = pFrameInfo->getGoalsArray() ;
        if (pLdvGoals && (false == pLdvGoals->empty()))
    	    for (ArrayGoalIter iterGoal = pLdvGoals->begin() ; pLdvGoals->end() != iterGoal ; iterGoal++)
      	    pGraphe->TousLesVrais((*iterGoal)->getNode(), NSRootLink::processWaitingFor, &ObjProcessVect) ;
      }
    }

    if (false == ObjProcessVect.empty())
    {
    	for (EquiItemIter iterProc = ObjProcessVect.begin() ; ObjProcessVect.end() != iterProc ; iterProc++)
      {
      	// mainteant on veux un noeud qui correspond à une PPT

        NSPatPathoArray TmpPPT(pContexte->getSuperviseur()) ;
        pContexte->getPatient()->DonneArray((*iterProc)->c_str(), &TmpPPT) ;
        if (TmpPPT.empty())
          break ;

        NSProcess NewProcess(pContexte) ;
        // on récupère le code lexique, le noeud, et le titre
        initProcessFromPatho(pcNode, &NewProcess,  &TmpPPT, (**iterProc));
        pGraphe->TousLesVrais(NewProcess.getMetaNoeud(), NSRootLink::problemRelatedTo, &(NewProcess.aConcerns)) ;
        //recuperation date de la demande
        VecteurString DocVect ;
        // strncpy(pcNode, (**iterProc).c_str(), PAT_NSS_LEN + DOC_CODE_DOCUM_LEN);
        string docId = string((**iterProc), 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN);
        pGraphe->TousLesVrais(docId, NSRootLink::docData, &DocVect, "ENVERS") ;
        if (false == DocVect.empty())              //7
        {
          for (EquiItemIter iterDoc = DocVect.begin() ; iterDoc != DocVect.end() ; iterDoc++)     //6
          {
            DocumentIter DocIt = pContexte->getPatient()->getDocHis()->getVectDocument()->begin() ;            for ( ; DocIt != pContexte->getPatient()->getDocHis()->getVectDocument()->end() ; DocIt++)   //5            {              NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;              (*DocIt)->initFromMeta(&PptMeta) ;              if (false == PptMeta.empty())   //4              {                PatPathoConstIter pptIt = PptMeta.begin() ;                if ((*pptIt)->getDoc() == **iterDoc)     //3                {                  for ( ; (PptMeta.end() != pptIt) && ((*pptIt)->getLexique() != "KREDA1") ; pptIt++)                    ;                  if (PptMeta.end() != pptIt)     //2                  {                    pptIt++;                    if (PptMeta.end() != pptIt)  //1                    {                      string sUnite  = "" ;                      string sFormat = "" ;
                      string sValeur = "" ;
                      string sTemp = "" ;

                      sTemp = (*pptIt)->getUnit() ;
                      pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
                      sTemp = (*pptIt)->getLexique() ;
                      pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
                      sValeur = (*pptIt)->getComplement() ;

                      if (sUnite == "2DA02")
                        NewProcess.tDateDemande.initFromDate(sValeur) ;
                    }   //1                  }     //2                }     //3
              }      //4
            }      //5
          }     //6
       	}     //7
        aCurrentProcesses.push_back(new NSProcess(NewProcess)) ;
      }   //for
    }    //if
	}      //else
}


void
NSProcessView::initProcessFromPatho(char* sNode, NSProcess *pNewProcess,  NSPatPathoArray *pTmpPPT, string metaNode)
{
  if ((NULL == pNewProcess) || (NULL == pTmpPPT) || (pTmpPPT->empty()))
   	return ;

	// on récupère le code lexique, le noeud, et le titre
  PatPathoIter  pptIter = pTmpPPT->begin() ;
	string        sLexiqProcess = (*pptIter)->getLexique() ;
  string        sTitleProcess = "" ;

  if (sLexiqProcess != string("£?????"))
  	pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqProcess, &sTitleProcess) ;

  // on les insère dans le process
  //pNewProcess->setDocNoeud(pcNode) ;       sNode
  //pNewProcess->setMetaNoeud(**i) ;        metaNode
  pNewProcess->setDocNoeud(sNode) ;
  pNewProcess->setMetaNoeud(metaNode) ;
  pNewProcess->setLexique(sLexiqProcess) ;
  pNewProcess->sTitre = sTitleProcess ;

  // on recherche la date d'ouverture
  int iColBase = (*pptIter)->getColonne() ;
  if (pptIter != pTmpPPT->end())
  	pptIter++ ;

  if ((pptIter != pTmpPPT->end()) && ((*pptIter)->getColonne() > iColBase))
  {
  	if ((*pptIter)->getColonne() == iColBase + 1)
    {
    	string sElemLex = (*pptIter)->getLexique() ;
      string sSens ;
      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

      // Dates
      if (sSens == "KOUVR")
      {
      	pptIter++ ;
        int iLigneBase = (*pptIter)->getLigne() ;
        string sUnite  = "" ;
        string sFormat = "" ;
        string sValeur = "" ;
        string sTemp   = "" ;
        while (	(pptIter != pTmpPPT->end()) &&
										  ((*pptIter)->getLigne() == iLigneBase))
        {
          string sLexique = (*pptIter)->getLexique() ;
					if ((string("") != sLexique) && ('£' == sLexique[0]))
          {
          	sTemp   = (*pptIter)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
            sValeur = (*pptIter)->getComplement() ;
            sTemp   = (*pptIter)->getUnit() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
            break ;
          }
					pptIter++ ;
				}

        // sFormat est du type £D0;03
        if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (sValeur != ""))
        {
        	if (((sUnite == "2DA01") || (sUnite == "2DA02")) && (sSens == "KOUVR"))
          	pNewProcess->tDateOuverture.initFromDate(sValeur) ;
          else if (((sUnite == "2DA01") || (sUnite == "2DA01")) && (sSens == "KFERM"))
          	pNewProcess->tDateFermeture.initFromDate(sValeur) ;
        }

			}
		}
	}

}


// Initialisation des colonnes de la ListWindowvoid
NSProcessView::SetupColumns()
{
  pListeWindow->InsertColumn(0, TListWindColumn("Process",         150,   TListWindColumn::Left, 0)) ;
  pListeWindow->InsertColumn(1, TListWindColumn("Date de demande",  80,   TListWindColumn::Left, 1)) ;
  pListeWindow->InsertColumn(2, TListWindColumn("Date de début",    80,   TListWindColumn::Left, 2)) ;
  pListeWindow->InsertColumn(3, TListWindColumn("Date de fin",      80,   TListWindColumn::Left, 3)) ;
}


// Affichage des éléments de la listevoid
NSProcessView::AfficheListe()
{
  pListeWindow->DeleteAllItems() ;

  if (aCurrentProcesses.empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
/*
  ProcessesIter itProcess = aCurrentProcesses.end() ;
  do
  {
    itProcess-- ;
    TListWindItem Item(((*itProcess)->sTitre).c_str(), 0) ;
    pListeWindow->InsertItem(Item) ;
  }
  while (itDg != aCurrentProcess.begin()) ;
*/
  for (ProcessesRIter itProcess = aCurrentProcesses.rbegin() ; itProcess != aCurrentProcesses.rend() ; itProcess++)
  {
    TListWindItem Item(((*itProcess)->sTitre).c_str(), 0) ;
    pListeWindow->InsertItem(Item) ;
  }
}


void
NSProcessView::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  const int   BufLen = 255 ;
  static char buffer[BufLen] ;
  string      sDate = string("") ;
  buffer[0] = '\0' ;

  TListWindItem   &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case 1 :  // date de demande
              sDate = (aCurrentProcesses[index])->tDateDemande.donneDate() ;
              sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;
    case 2 :  // date de début
              sDate = (aCurrentProcesses[index])->tDateOuverture.donneDate() ;
              sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;
    case 3 :  // date de fin
              sDate = (aCurrentProcesses[index])->tDateFermeture.donneDate() ;
              sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;
  }
}

boolNSProcessView::VnIsWindow(HWND hWnd){
  return (HWindow == hWnd) ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindowvoid
NSProcessView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
  pListeWindow->MoveWindow(GetClientRect(), true) ;
}


// fonction EVSetFocusvoid
NSProcessView::EvSetFocus(HWND hWndLostFocus)
{
	activateMUEViewMenu() ;

  TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  NSMUEView::EvSetFocus(hWndLostFocus) ;
  if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
    SetupToolBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }

  _pPaneSplitter->SetFrameTitle(getFunction(), _sViewName) ;
  pContexte->setAideIndex("") ;
  pContexte->setAideCorps("process.htm") ;
}


// SetupToolBarvoid
NSProcessView::SetupToolBar()
{
  TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->FlushControlBar() ;

	pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_PROCESS_NEW,     CM_PROCESS_NEW,     TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_PROCESS_STOP,    CM_PROCESS_STOP,    TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_PROCESS_CHANGE,  CM_PROCESS_CHANGE,  TButtonGadget::Command)) ;

  pMyApp->getSecondaryControlBar()->LayoutSession() ;
}

//***************************************************************************
//
//  					Méthodes de NSDrugsPropertyWindow//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSProcessesPropertyWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_KEYDOWN,
  EV_WM_SETFOCUS,
END_RESPONSE_TABLE ;

NSProcessesPropertyWindow::NSProcessesPropertyWindow(NSProcessView *parent, int id, int x, int y, int w, int h, TModule *module)
  : TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  pView   = parent ;
  iRes    = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}

void
NSProcessesPropertyWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

// -----------------------------------------------------------------------------
//  Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSProcessesPropertyWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;
  HitTest(info) ;

  //if (info.GetFlags() & LVHT_ONITEM)  //  pDlg->CmModifier() ;
}

void
NSProcessesPropertyWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
    if      (key == VK_DELETE)
        pView->CmArreter() ;
    else if (key == VK_INSERT)
    {
        if (GetKeyState(VK_SHIFT) < 0)
            pView->CmModifier() ;
        else
            pView->CmNouveau() ;
    }
    else if (key == VK_TAB)
    {
        if (GetKeyState(VK_SHIFT) < 0)
            pView->setFocusToPrevSplitterView() ;
        else
            pView->setFocusToNextSplitterView() ;
    }
    else
    	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

// -----------------------------------------------------------------------------//  Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSProcessesPropertyWindow::IndexItemSelect()
{
  int count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)    if (GetItemState(i, LVIS_SELECTED))
    {
      index = i ;
      break ;
    }

  return index ;}


void
NSProcessesPropertyWindow::EvSetFocus(HWND hWndLostFocus)
{
  pView->EvSetFocus(hWndLostFocus) ;
}

