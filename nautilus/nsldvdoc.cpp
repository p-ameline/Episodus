// -----------------------------------------------------------------------------
// Nsldvdoc.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.29 $
// $Author: pameline $
// $Date: 2014/10/31 15:01:29 $
// -----------------------------------------------------------------------------
// Ligne de Vie document
// -----------------------------------------------------------------------------
// FLP - aout 2003
// ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//           Ligne de vie - Document du modèle Document/Vue
// -----------------------------------------------------------------------------

#include <owl\filedoc.h>
#include <iostream.h>
#include <cstring.h>

#include "nssavoir\nsBdmDriver.h"
#include "partage\nsdivfct.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsdecode.h"
#include "nsbb\nsbb.h"
#include "nautilus\nsrechd2.h"
#include "dcodeur\nsdkd.h"
#include "dcodeur\decoder.h"
#include "dcodeur\nsphrase.h"
#include "dcodeur\nsgen.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nstrihis.h"
#include "nssavoir\nsgraphe.h"
#include "nautilus\nscsdoc.h"
#include "dcodeur\nsdkd.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nstlibre.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nsarc.h"
#include "nsldv\nsldvstructs.h"
#include "nssavoir\nsrightsmanager.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsldvvue.h"
#include "nsldv\nsldvgoal.h"
#include "nautilus\nsdrugview.h"
#include "nautilus\nsgoalview.h"
#include "nautilus\nsprocessview.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nscompub.h"
#include "nautilus\ns_html.h"
#include "nsoutil\ns_arche.h"
#include "nsoutil\ibdm.h"

#include "nautilus\nssoapview.h"

#include "ns_ob1\TypedVal.h"
#include "ns_ob1\NautilusType.h"
#include "ns_ob1\BB1Task.h"
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\BB1KS.h"
#include "ns_ob1\Interface.h"

const string drugPhaseSepar    = " | ";
const char   drugMMSSepar      = ':';
const char   drugMMSUpSepar    = '^';
const char   drugMMSBNoonSepar = '[';
const char   drugMMSANoonSepar = ']';
const char   drugMMSDownSepar  = '_';

long NSConcernModifier::lObjectCount = 0 ;
long NSConcern::lObjectCount = 0 ;
long NSLdvSousObjet::lObjectCount = 0 ;
long NSLdvObjet::lObjectCount = 0 ;
long NSLdvDrugTake::lObjectCount = 0 ;
long NSLdvDrugCycle::lObjectCount = 0 ;
long NSLdvDrugPhase::lObjectCount = 0 ;
long NSLdvDrug::lObjectCount = 0 ;
long NSFrameInformation::lObjectCount = 0 ;
long NSLdvDocument::lObjectCount = 0 ;

// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSLdvDocument -------------------------
// --------------------------------------------------------------------------


//---------------------------------------------------------------------------
//            Constructeur
//---------------------------------------------------------------------------

NSLdvDocument::NSLdvDocument(TDocument* parent, NSContexte* pCtx)
              :NSLdvDocumentBase(parent, pCtx)
{
try
{
	SetTitle("Ligne de Vie") ;

  _pPubli = (NSPublication*) 0 ;

  lObjectCount++ ;

  string sTraceText = string("NSLdvDocument is now ready.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;
}
catch (...)
{
	erreur("Exception NSLdvDocument ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Description : Constructeur copie
//---------------------------------------------------------------------------

NSLdvDocument::NSLdvDocument(const NSLdvDocument& rv)
              :NSLdvDocumentBase(rv)
{
try
{
  _pPubli = (NSPublication*) 0 ;

  lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSLdvDocument copy ctor", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Description : Destructeur
//---------------------------------------------------------------------------

NSLdvDocument::~NSLdvDocument()
{
  if (_pPubli)
    delete _pPubli ;

  lObjectCount-- ;
}


//---------------------------------------------------------------------------
//  Description : Operateur =
//---------------------------------------------------------------------------

NSLdvDocument&
NSLdvDocument::operator=(const NSLdvDocument& src)
{
try
{
	if (this == &src)
		return *this ;

  _aObjets     = src._aObjets ;
  _aSousObjets = src._aSousObjets ;
  _aQoLCharts  = src._aQoLCharts ;

	_sLangue = src._sLangue ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSLdvDocument::operator=.", standardError, 0) ;
	return *this ;
}
}

void
NSLdvDocument::Publish(NSLdvPubli *pPubliDriver, bool bCorrespPatient)
{
try
{
  if ((NSLdvPubli*) NULL == pPubliDriver)
    return ;

	if (((NSUtilisateurChoisi*) NULL == pContexte->getUtilisateur()) ||
   	 	(bCorrespPatient && ((NSPatientChoisi*) NULL == pContexte->getPatient())))
		return ;

  if (_pPubli)
  {
		delete _pPubli ;
    _pPubli = (NSPublication*) 0 ;
  }

	// on publie le document en cours (pointeur this) avec ou sans corresp
	if (bCorrespPatient)
		_pPubli = new NSPublication(pContexte->getPatient(), this, pPubliDriver, pContexte) ;
	else
		_pPubli = new NSPublication(0, this, pPubliDriver, pContexte) ;

	_pPubli->Publier() ;
}
catch (...)
{
  erreur("Exception NSLdvDocument::Publish.", standardError, 0) ;
}
}

bool
NSLdvDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation, NSLdvPubli *pPubliDriver, string sAdresseCorresp, string sPathDest)
{
  if ((NSLdvPubli*) NULL == pPubliDriver)
    return false ;

  string sFichierHtml = string("") ;

  switch(pPubliDriver->_iPrintType)
  {
    case NSLdvPubli::printGoals :
    {
      NSHtmlGoals hGoals(typeOperation, this, pContexte, pPubliDriver, sAdresseCorresp) ;

      // on trouve le nom du fichier temporaire à visualiser
      sNomHtml = hGoals.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
      sFichierHtml = sPathHtml + sNomHtml ;

      hGoals.ajouteLignes() ;

      // on passe une base d'images à cause de la référence
      string sBaseImg ;
      if (false == hGoals.genereHtml(sFichierHtml, sBaseImg,
                                      pPubliDriver->_sTemplateFile,
                                      pPubliDriver->_sHeaderFile))
        return false ;

      break ;
    }
    case NSLdvPubli::printDrugs :
    {
      NSHtmlDrugs hDrugs(typeOperation, this, pContexte, pPubliDriver, sAdresseCorresp) ;

      // on trouve le nom du fichier temporaire à visualiser
      sNomHtml = hDrugs.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
      sFichierHtml = sPathHtml + sNomHtml ;

      hDrugs.ajouteLignes() ;

      // on passe une base d'images à cause de la référence
      string sBaseImg ;
      if (false == hDrugs.genereHtml(sFichierHtml, sBaseImg,
                                      pPubliDriver->_sTemplateFile,
                                      pPubliDriver->_sHeaderFile))
        return false ;

      break ;
    }
  }

	return true ;
}

NSDocumentHisto*
NSLdvDocument::getStorageDocument(LDVFRAME iFrame, NSRootLink::NODELINKTYPES iRelation)
{
  NSPatientChoisi* pPatientChoisi = pContexte->getPatient() ;
	// Can we work ?
	if ((NSPatientChoisi*) NULL == pPatientChoisi)
	{
		pContexte->getSuperviseur()->traceAndDisplay("patientManagement", "cannotFindPatient") ;
		return (NSDocumentHisto*) 0 ;
	}

  // First step, find the root tree for frame
  //
  DocumentIter iterRootDoc = pPatientChoisi->getFrameIndex(iFrame) ;
  if ((DocumentIter) NULL == iterRootDoc)
  	return (NSDocumentHisto*) 0 ;

  NSDocumentHisto* pMasterDocum = *iterRootDoc ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;
  NSSuper::INDEXMANAGEMENT idxMgmtRule = pSuper->getIndexManagementRule() ;

  // In case of single tree management, we are done
  //
  if (NSSuper::umSingleTree == idxMgmtRule)
    return pMasterDocum ;

  int iMaxItemsInTree = 10 ;

  // Later on, we will need the chapter
  //
  string sChapterRoot = string("") ;
  switch(iRelation)
  {
    case NSRootLink::indexConcerns : sChapterRoot = string("0PRO11") ; break ;
    case NSRootLink::drugOf        : sChapterRoot = string("N00001") ; break ;
    case NSRootLink::indexGoals    : sChapterRoot = string("0OBJE1") ; break ;
    default                        : return *iterRootDoc ;
  }

  // Find subtrees
  //
  string sFrameIndexMeta = pMasterDocum->getCodeDocMeta() ;

  VecteurString aVecteurString ;
  if (false == NSLdvDocumentBase::getStorageDocumentsIds(&aVecteurString, sFrameIndexMeta, iRelation))
    return (NSDocumentHisto*) 0 ;

  // No extension tree
  //
  if (aVecteurString.empty())
  {
    if (NSSuper::umAuto == idxMgmtRule)
    {
      int iRoot = getStorageDocumentCardinality(*iterRootDoc, sChapterRoot) ;
      if (iRoot >= iMaxItemsInTree)
        return newStorageDocument(pMasterDocum, iRelation) ;
      else
        return *iterRootDoc ;
    }
    else
      return newStorageDocument(pMasterDocum, iRelation) ;
  }

  NSDocumentHisto* pDocum = getMoreRecentDocument(&aVecteurString) ;
  if ((NSDocumentHisto*) NULL == pDocum)
    return (NSDocumentHisto*) 0 ;

  if (NSSuper::umAuto != idxMgmtRule)
    return pDocum ;

  int iRoot = getStorageDocumentCardinality(pDocum, sChapterRoot) ;
  if (iRoot >= iMaxItemsInTree)
    return newStorageDocument(pMasterDocum, iRelation) ;
  else
    return pDocum ;
}

NSDocumentHisto*
NSLdvDocument::newStorageDocument(NSDocumentHisto* pRootDoc, NSRootLink::NODELINKTYPES iRelation)
{
  if ((NSDocumentHisto*) NULL == pRootDoc)
    return (NSDocumentHisto*) 0 ;

  NSPatientChoisi* pPatientChoisi = pContexte->getPatient() ;
	if ((NSPatientChoisi*) NULL == pPatientChoisi)
	  return (NSDocumentHisto*) 0 ;

  NSPersonGraphManager* pGraphManager = pPatientChoisi->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return (NSDocumentHisto*) 0 ;

  NSRefDocument	newTank(0, 0, 0, pContexte, false) ;

  switch(iRelation)
  {
    case NSRootLink::indexConcerns :
      if (false == newTank.createFrameConcernsTree(pGraphManager, pRootDoc->getCodeDocMeta()))
        return (NSDocumentHisto*) 0 ;
      break ;
    case NSRootLink::drugOf :
      if (false == newTank.createFrameTreatmentsTree(pGraphManager, pRootDoc->getCodeDocMeta()))
        return (NSDocumentHisto*) 0 ;
      break ;
    case NSRootLink::indexGoals :
      if (false == newTank.createFrameGoalsTree(pGraphManager, pRootDoc->getCodeDocMeta()))
        return (NSDocumentHisto*) 0 ;
      break ;
  }

  newTank.enregistrePatPatho() ;

  if ((NSHISTODocument*) NULL == pPatientChoisi->getDocHis())
    return (NSDocumentHisto*) 0 ;

  NSPatPathoArray PptNewTank(pContexte->getSuperviseur()) ;
  newTank.initFromPatPatho(&PptNewTank) ;

  pPatientChoisi->getDocHis()->Rafraichir(newTank.DonneDocInfo(), &PptNewTank, &newTank) ;

  DocumentIter iterNewDoc = pPatientChoisi->getDocHis()->TrouveDocHisto(newTank.getCodeDocMeta()) ;
  if (iterNewDoc)
  {
    // Add index to array
    //
    NSPatPathoInfo* pRootPathoInfo = pRootDoc->getRootPathoInfo() ;

    if (pRootPathoInfo)
    {
      string sRootNode = pRootPathoInfo->getNode() ;

      NSFrameInformation* pFrame = getFrameForNode(sRootNode) ;
      if (pFrame)
      {
        switch(iRelation)
        {
          case NSRootLink::indexConcerns :
            pFrame->addConcernDocument(*iterNewDoc) ;
            break ;
          case NSRootLink::drugOf :
            pFrame->addTreatmentDocument(*iterNewDoc) ;
            break ;
          case NSRootLink::indexGoals :
            pFrame->addGoalsDocument(*iterNewDoc) ;
            break ;
        }
      }
    }

    return *iterNewDoc ;
  }

  return (NSDocumentHisto*) 0 ;
}

bool
NSLdvDocument::newLine(LDVFRAME iFrame, string& sNode, NewConcernInfo *pConcernInfo)
{
try
{
  if ((NewConcernInfo*) NULL == pConcernInfo)
    return false ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

	string ps2 = "NSLdvDocument::newLine : begin" ;
  pSuper->trace(&ps2, 1, NSSuper::trSubSteps) ;

  sNode = string("") ;

  NSDocumentHisto* pIndexDocHisto = getStorageDocument(iFrame, NSRootLink::indexConcerns) ;
  if ((NSDocumentHisto*) NULL == pIndexDocHisto)
  {
		erreur("Erreur grave : impossible de trouver le document Index de santé de ce patient.", standardError, 0) ;
		return false ;
	}

  NSPatientChoisi* pPatientChoisi = pContexte->getPatient() ;
	// Can we work ?
	if ((NSPatientChoisi*) NULL == pPatientChoisi)
	{
		pContexte->getSuperviseur()->traceAndDisplay("patientManagement", "cannotFindPatient") ;
		return false ;
	}

	// Health index found
	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pIndexDocHisto->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	NSNoyauDocument Noyau((TDocument*) 0, &Docum, 0, pContexte, false, true) ;

  NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
  Noyau.initFromPatPatho(&PptNoyau) ;

	// On cherche le chapitre "préoccupations de santé"
	// Looking for "Health concerns" chapter
	PatPathoIter iter = PptNoyau.ChercherItem("0PRO11") ;

	if (((PatPathoIter) NULL == iter) || (PptNoyau.end() == iter))
	{
		erreur("Erreur grave : le document Index de santé de ce patient ne possède pas de chapitre Préoccupations de santé.", standardError, 0) ;
		return false ;
	}

	// Ajout d'un nouveau segment d'arbre
	// Adding a new tree branch
	NSPatPathoArray PatPatho(pSuper) ;

  string sLexique = pConcernInfo->getLexique() ;

	if (string("£?????") != sLexique)
		PatPatho.ajoutePatho(sLexique, 0) ;
	else
	{
		Message Msg ;
		Msg.SetLexique(sLexique) ;
		Msg.SetTexteLibre(pConcernInfo->getFreeText()) ;
		PatPatho.ajoutePatho(sLexique, &Msg, 0) ;
	}

  string sCocCode = pConcernInfo->getCocCode() ;

  if (string("") != sCocCode)
  {
    Message Msg ;
		Msg.SetComplement(sCocCode) ;
		PatPatho.ajoutePatho("6CISP1", &Msg, 1) ;
  }

  string sCimCode = pConcernInfo->getCimCode() ;

  if (string("") != sCimCode)
  {
    Message Msg ;
		Msg.SetComplement(sCimCode) ;
		PatPatho.ajoutePatho("6CIMA1", &Msg, 1) ;
  }

  string sDateDeb = pConcernInfo->getStartDate() ;

	// Date de début - Date of start
	if ((string("") != sDateDeb) && (sDateDeb != string("19000000000000")) && (sDateDeb != string("00000000000000")))
	{
		PatPatho.ajoutePatho("KOUVR1", 1) ;
		Message Msg ;
    Msg.SetUnit("2DA021") ;
		Msg.SetComplement(sDateDeb) ;
		PatPatho.ajoutePatho("£T0;19", &Msg, 2) ;
	}

  string sDateFin = pConcernInfo->getEndDate() ;

	// Date de fin - Date of ending
	if ((string("") != sDateFin) && (string("19000000000000") != sDateFin) &&	(string("00000000000000") != sDateFin))
	{
		PatPatho.ajoutePatho("KFERM1", 1) ;
		Message Msg ;
    Msg.SetUnit("2DA021") ;
		Msg.SetComplement(sDateFin) ;
		PatPatho.ajoutePatho("£T0;19", &Msg, 2) ;
	}

  int iSeverity = pConcernInfo->getSeverity() ;

	// Quantification
	if (iSeverity > 0)
	{
		PatPatho.ajoutePatho("VQUAN1", 1) ;

		PatPatho.ajoutePatho("KDAT01", 2) ;
		Message Msg ;
		Msg.SetUnit("2DA021") ;
		Msg.SetComplement(sDateDeb) ;
		PatPatho.ajoutePatho("£T0;19", &Msg, 3) ;

    PatPatho.ajoutePatho("VIGRA1", 2) ;

    char szGravite[4] ;
    numacar(szGravite, iSeverity, 3) ;
    Message Msg2 ;
    Msg2.SetUnit("200001") ;
    Msg2.SetComplement(string(szGravite)) ;
    PatPatho.ajoutePatho("£N2;04", &Msg2, 3) ;
	}

	// Details
	if (false == pConcernInfo->getDetails()->empty())
		PatPatho.InserePatPatho(PatPatho.end(), pConcernInfo->getDetails(), 1) ;

	// Ajout de la branche à l'arbre Index de santé
	// Adding the branch to the Health index tree

	// On cherche où il faut insérer la branche
	// Looking for the place to insert the branch
	int colonneMere = (*iter)->getColonne() ;
	iter++ ;
	while ((PptNoyau.end() != iter) && ((*iter)->getColonne() > colonneMere))
		iter++ ;

	if (PptNoyau.end() != iter)
		PptNoyau.InserePatPatho(iter, &PatPatho, colonneMere + 1) ;
	else
  	PptNoyau.InserePatPatho(iter, &PatPatho, colonneMere + 1) ;

	// En n-tiers, on crée les liens avant l'enregistrement de la patpatho
	//
	int ligPreo, colPreo ;

	if (iSeverity > 0)
  {
    string sRootCode = getRootForFrame(iFrame) ;
    string sRootSens = string("") ;
    pContexte->getDico()->donneCodeSens(&sRootCode, &sRootSens) ;
  	// On cherche la préoccupation qui vient d'être créée (dernière du genre)
    // Looking for the concern we have just created (last of the kind)
    pPatientChoisi->getGraphPerson()->setTree(&PptNoyau, "") ;
		iter = PptNoyau.begin() ;
    string sDocRootSens = (*iter)->getLexiqueSens() ;
		if ((sDocRootSens != sRootSens) && (sDocRootSens != string("0PRO1")))
		{
			erreur("Erreur à l'inscription de la sévérité : le document Index de santé de ce patient semble endommagé.", standardError, 0) ;
			return false ;
		}
		iter = PptNoyau.ChercherItem("0PRO11") ;
		if ((NULL == iter) || (PptNoyau.end() == iter))
		{
			erreur("Erreur à l'inscription de la sévérité : le document Index de santé de ce patient ne possède pas de chapitre Préoccupations de santé.", standardError, 0) ;
			return false ;
		}
		int iConcernCol = (*iter)->getColonne() ;
		iter++ ;

		PatPathoIter iterConcern = PptNoyau.begin() ;
		while ((PptNoyau.end() != iter) && ((*iter)->getColonne() > iConcernCol))
		{
			if ((*iter)->getLexique() == sLexique)
				iterConcern = iter ;
			iter++ ;
		}
		if (PptNoyau.begin() == iterConcern)
		{
			erreur("Erreur à l'inscription de la gravité et du risque : impossible de retrouver la préoccupation de santé.", standardError, 0) ;
			return false ;
		}

    PatPathoIter iterQuantif = PptNoyau.ChercherItem(string("VQUAN1"), iterConcern) ;
		if (((PatPathoIter) NULL == iterQuantif) || (PptNoyau.end() == iterQuantif))
		{
			erreur("Erreur à la connexion à la Ligne de vie des index de quantification : le document de contact ne semble pas correctement sauvegardé.", standardError, 0) ;
			return false ;
		}

    //(*iterQuantif)->addTemporaryLink(*iterConcern, NSRootLink::problemContactElement, NSLinkedNode::dirFleche) ;
    // on fait ici un setTree pour appeler numberTemporarayNodes avant de faire la copie
    // de la patpatho - avec ses liens en attente - qui a lieu dans enregistrePatPatho
    pPatientChoisi->getGraphPerson()->getLinkManager()->etablirLien((*iterQuantif)->getNode(), NSRootLink::problemContactElement, (*iterConcern)->getNode()) ;

    ligPreo = (*iterConcern)->getLigne() ;
    colPreo = (*iterConcern)->getColonne() ;
	}

	ps2 = "NSLdvDocument::newLine : before enregistrePatPatho" ;
	pSuper->trace(&ps2, 1, NSSuper::trSubSteps) ;

	Noyau.setPatPatho(&PptNoyau) ;
  Noyau.enregistrePatPatho() ;

	ps2 = "NSLdvDocument::newLine : before chargePatPatho" ;
	pSuper->trace(&ps2, 1, NSSuper::trSubSteps) ;

	// Mise à jour de l'historique
	bool bReload = Noyau.chargePatPatho() ;
	Noyau.initFromPatPatho(&PptNoyau) ;
	pPatientChoisi->getDocHis()->Rafraichir(&Docum, &PptNoyau, &Noyau) ;

	if (iSeverity > 0)
  {
  	PatPathoIter iterPreo = PptNoyau.ChercherItem(ligPreo, colPreo) ;
    sNode = (*iterPreo)->getNode() ;
	}

  NautilusObjectType iObjectType = Preocupation ;
  if (ldvframeRisk == iFrame)
    iObjectType = Risk ;
	pContexte->getBBinterface()->insertAnswerOnBlackboard("Preocupation", &PatPatho, iObjectType, 0) ;

	ps2 = "NSLdvDocument::newLine : before reinit and invalidateViews" ;
	pSuper->trace(&ps2, 1, NSSuper::trSubSteps) ;

  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (pFrameInfo)
  {
    ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;
    if (pConcerns)
	    pConcerns->reinit() ;
  }

	invalidateViews("CONCERN_NEW") ;

	// on appelle la fonction addDatasFromNautilusOnBlackboard par un todo
	//addDatasFromNautilusOnBlackboard(pContexte, pPt) ;

  /*#ifdef __OB1__
  pContexte->getSuperviseur()->blackboardInterface->insertAnswerOnBlackboard("Preocupation", pPt, Preocupation);
  #else
	NSToDoTask  *pTask = new NSToDoTask() ;

	pTask->sWhatToDo = "AddDatasOnBBK" ;
	pTask->pPointer1 = (void *)pPt ;

	pContexte->getSuperviseur()->aToDo.push_back(pTask) ;
	pContexte->getSuperviseur()->pNSApplication->GetMainWindow()->PostMessage(WM_COMMAND, IDM_TODO) ;
  #endif        */

	ps2 = "NSLdvDocument::newLine : done" ;
  pSuper->trace(&ps2, 1, NSSuper::trSubSteps) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSLdvDocument::newConcern.", standardError, 0) ;
	return false ;
}
}

bool
NSLdvDocument::connectObjectToConcern(LDVFRAME iFrame, string sCodeConcern, int iSeverity, string sNoeud, NSRootLink::NODELINKTYPES iRelation, bool bAutoCreate)
{
try
{
	string sCodeSensConcern ;
	pContexte->getDico()->donneCodeSens(&sCodeConcern, &sCodeSensConcern) ;

	//
	// Recherche d'une préoccupation OUVERTE correspondant à sCodeConcern
	//
	NSConcern* pConcern = donneOpenConcern(iFrame, sCodeSensConcern) ;

	if ((NSConcern*) NULL == pConcern)
	{
		if (false == bAutoCreate)
			return false ;

		//
		// Recherche de la vue
		//
		NSLdvView* pLdvView = (NSLdvView*) 0 ;

		TView* pView = GetViewList() ;
    if (pView)
    {
			do
			{
				pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView) ;
				if (pLdvView)
					break ;

				pView = NextView(pView) ;
			}
			while (pView) ;
    }

		bool bVerbose = false ;
		if (bVerbose)
		{
			//
			// Fenêtre de création de la préoccupation
			//
			if ((NSLdvView*) NULL == pLdvView)
				return false ;

			string sCreatedConcern = sCodeConcern ;
      string sCocCode = string("") ;
			NSNewConcernDlg* pNPDialog ;

			NSPatPathoArray Details(pContexte->getSuperviseur()) ;

      NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;
      concernInfo.setLexique(sCodeConcern) ;
      concernInfo.setSeverity(iSeverity) ;

			string sNewNode = string("") ;
			// NSNewConcernDlg* pNPDialog = new NSNewConcernDlg(pLdvView, pContexte, &sCreatedConcern);
			// else
			if (pLdvView)
				pNPDialog = new NSNewConcernDlg(pLdvView, this, pContexte, &concernInfo) ;
			else
				pNPDialog = new NSNewConcernDlg(pContexte->GetMainWindow(), this, pContexte, &concernInfo) ;

			int iExecReturn = pNPDialog->Execute() ;

			if (IDOK != iExecReturn)
				return false ;

      bool bResult = newLine(ldvframeHealth, sNewNode, &concernInfo) ;
		}
    // Automatic creation
    //
		else
		{
			NVLdVTemps tpsNow ;
			tpsNow.takeTime() ;

      string sLabel ;
      pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sCodeConcern, &sLabel) ;

      string sCocCode = string("") ;
      findCocCode(false, sCocCode, sCodeConcern, sLabel, string("6CISP"), string("0SOA41")) ;

      NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;
      concernInfo.setLexique(sCodeConcern) ;
      concernInfo.setSeverity(iSeverity) ;
      concernInfo.setCocCode(sCocCode) ;
      concernInfo.setStartDate(tpsNow.donneDateHeure()) ;

			string sNode = string("") ;
			if (false == newLine(iFrame, sNode, &concernInfo))
				return false ;
		}

    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;
      if (pConcerns)
	      pConcerns->reinit() ;
    }

		pConcern = donneOpenConcern(iFrame, sCodeSensConcern) ;
    //
    // Décidemment ça ne marche pas
    //
    if ((NSConcern*) NULL == pConcern)
			return false ;
	}

	string sConcernRef = pConcern->getNoeud() ;

  //
	// On a trouvé la préoccupation, on connecte
	//
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
  pGraphe->etablirLienDocument(sNoeud, iRelation, sConcernRef) ;

	//
	// On met à jour l'Objet qui représente le document
	//
	NSLdvObjet* pObj = _aObjets.getObjet(sNoeud) ;

	if (pObj)
	{
		pObj->setConcern(sConcernRef) ;

		TView* pView = GetViewList();
    if (pView)
    {
			do
			{
				NSLdvView* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView) ;
				if (pLdvView)
					pLdvView->addObjOnConcern(pConcern, pObj) ;

				pView = NextView(pView) ;
			}
			while (pView) ;
    }
	}

	return true ;
}
catch (...)
{
	erreur("Exception NSLdvDocument::connectObjectToConcern.", standardError, 0) ;
	return false ;
}
}

void
NSLdvDocument::showNewDrug(const NSPatPathoArray *pPPT, PatPathoIter iter)
{
  // ---------------------------------------------------------------------------
  // cette fonction est une spécialisation de showNewTree pour le médicament
  // this function is specialization of function showNewTree for Drugs
  // ---------------------------------------------------------------------------

  // ---------------------------------------------------------------------------
  // TODO
  // il faudrait également parser les fils de la patpatho pointé par iter, afin
  // de pouvoir gérer des cas particuliers comme une perfusion
  // we have to parse already sons of patpatho represented by iter, to manage
  // some particular cases like perfusion.
  // ---------------------------------------------------------------------------

	if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty() || (pPPT->end() == iter))
  	return ;

  // Declare new drug as event for blackboard
  //
	NautilusEvent nsEvent(pPPT, iter, ADD, true) ;
  pContexte->getBBinterface()->addNautilusEvent(&nsEvent) ;

  bool bGoalExist = false ;

  for (int i = 0 ; (i < FRAMECOUNT) && (false == bGoalExist) ; i++)
  {
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
      if (pGoals && (false == pGoals->empty()))
        bGoalExist = true ;
    }
  }

  if (false == bGoalExist)
  {
		viewsReloadDrugToons() ;
		return ;
	}

	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
	bool      bModified = false ;

  // Le noeud courant (correspondant à l'iterateur passé en paramètre) remet-il
  // à zéro un objectif ?
  // Does current node (which is represented by iterator iter passed in
  // parameter in function) reset a goal ?
  string  sConcept = (*iter)->getLexique() ;
  string  sConceptSens ;
  pContexte->getDico()->donneCodeSens(&sConcept, &sConceptSens) ;

  string  sConceptNode = (*iter)->getNode() ;
  if (string("") == sConceptNode)
		return ;

  // parsage de la patpatho afin de savoir les informations qu'elle contient
  // parsing the patpatho instance to know what informations are in it

  // variables pour récupérer la date d'ouverture et de fermeture de la
  // prescription du médicament
  // variables for retrieving opening and ending date of Drug Administration
  string  sOpeningDate  = string("") ;
  string  sEndingDate   = string("") ;

  // récupération de la date d'ouverture et de fermeture
  // getting opening and ending date
  PatPathoIter iterTemp = iter ;
  int iRootCol = (*iterTemp)->getColonne() ;

  iterTemp++ ;

  while (pPPT->end() != iterTemp)
  {
  	//
    // Date d'ouverture - Opening date (only the root opening date is taken
    // into account; phases opening dates are not handled there)
    //
		if      ((string("KOUVR") == (*iterTemp)->getLexiqueSens()) &&
             ((*iterTemp)->getColonne() == iRootCol + 1))
		{
			int	iColBaseOuverture = (*iterTemp)->getColonne() ;
			if (iterTemp != pPPT->end())
				iterTemp++ ;

			string sCodeSensUnit = (*iterTemp)->getUnitSens() ;
      string sRootCodelex = string((*iterTemp)->getLexique(), 0, 2) ;

			if (((*iterTemp)->getColonne() > iColBaseOuverture) &&
      				((sRootCodelex == "£D") || (sRootCodelex == "£T")) &&
      				((sCodeSensUnit == "2DA01") || (sCodeSensUnit == "2DA02")))
			{
				sOpeningDate = (*iterTemp)->getComplement() ;
				if (iterTemp != pPPT->end())
					iterTemp++ ;
			}
		}
    //
    // Date de fermeture - Closing date
    //
		else if ((string("KFERM") == (*iterTemp)->getLexiqueSens()) &&
             ((*iterTemp)->getColonne() == iRootCol + 1))
		{
			int iColBaseOuverture = (*iterTemp)->getColonne() ;
			if (iterTemp != pPPT->end())
				iterTemp++ ;
			string sCodeSensUnit = (*iterTemp)->getUnitSens() ;
      string sRootCodelex = string((*iterTemp)->getLexique(), 0, 2) ;

			if (((*iterTemp)->getColonne() > iColBaseOuverture) &&
      				((sRootCodelex == "£D") || (sRootCodelex == "£T")) &&
      				((sCodeSensUnit == "2DA01") || (sCodeSensUnit == "2DA02")))
			{
				sEndingDate = (*iterTemp)->getComplement() ;
				if (iterTemp != pPPT->end())
					iterTemp++ ;
			}
		}
		else
			// on passe à l'itérateur suivant
			// next iterator
			iterTemp++ ;
	}

	// si date au format AAAAMMJJ : on ajoute l'heure 00:00:00
	// if date is in AAAAMMGG format : we add time 00:00:00
	if (strlen(sOpeningDate.c_str()) == 8)
		sOpeningDate += "000000" ;

	VectString VSEquivalent ;
	pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sConceptSens, "ES", &VSEquivalent, "FLECHE") ;

	// parcours des objectifs
	// searching in the goals
	//  ArrayGoals    *pGoalsWithValue = new ArrayGoals(this) ;
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
      if (pGoals && (false == pGoals->empty()))
      {
        for (ArrayGoalIter goalIt = pGoals->begin() ; pGoals->end() != goalIt ; goalIt++)
        {
          // Si l'objectif est ouvert, on regarde si le noeud est un reseter
          // if goal is open, we look if the node is a reseter
          if (false == (*goalIt)->_tOuvertLe.estVide())
          {
            string sGoalOpeningDate = (*goalIt)->_tOuvertLe.donneDateHeure() ;
            string sGoalEndingDate = "" ;
            if (false == (*goalIt)->_tFermeLe.estVide())
              sGoalEndingDate = (*goalIt)->_tFermeLe.donneDateHeure() ;

            // Evénement à l'intérieur de la période de validité de l'objectif
            // Event inside this goal's validity period
            if ((sOpeningDate >= sGoalOpeningDate) && ((sGoalEndingDate == "") || (sOpeningDate < sGoalEndingDate)))
            {
              // Si l'objectif correspond au médicament ou si le médicament a un lien ESTUN avec cet objectif
              // Does goal interact with drug or does the medic have a IsA link with this goal
              string sGoalSens ;
              pContexte->getDico()->donneCodeSens(&((*goalIt)->getLexique()), &sGoalSens) ;

              if ((sGoalSens == sConceptSens) || VSEquivalent.contains(sGoalSens))
              {
                pGraphe->etablirLien(sConceptNode, NSRootLink::goalReseter, (*goalIt)->getNode()) ;
                bModified = true ;
              }

              // Same thing with closing event (if applicable)
              //
              if (string("") != (*goalIt)->_sCloseEventNode)
              {
                string sCloseSens ;
                pContexte->getDico()->donneCodeSens(&((*goalIt)->_sCloseEventNode), &sCloseSens) ;
                if ((sCloseSens == sConceptSens) || VSEquivalent.contains(sCloseSens))
                {
                  pGraphe->etablirLien(sConceptNode, NSRootLink::goalAlarm, (*goalIt)->getNode()) ;
                  bModified = true ;
                }
              }
            }
          }
          // Si l'objectif est encore fermé, on regarde si le noeud l'ouvre
          // if goal is currently closed, we look if the node can open it
          else if (string("") != (*goalIt)->_sOpenEventNode)
          {
            string sOpenSens ;
            pContexte->getDico()->donneCodeSens(&((*goalIt)->_sOpenEventNode), &sOpenSens) ;
            if (sOpenSens == sConceptSens)
            {
              pGraphe->etablirLien(sConceptNode, NSRootLink::goalOpener, (*goalIt)->getNode()) ;
              bModified = true ;
            }
          }
        }
      }
    }
  }

	if (bModified)
	{
		reinitAllGoals() ;
		// no need to invalidateViews this is done in CreeObjectif
		//    invalidateViews("DOCUM_NEW") ;
	}

  //
  //
  viewsReloadDrugToons() ;
}

// ----------------------------------------------------------------------
//                          Mise à jour des vues
// ----------------------------------------------------------------------
void
NSLdvDocument::invalidateViews(string sReason, string sReference)
{
	//
	// Valeurs possibles de sReason - Possible values for sReason
	//
	// CONCERN_NEW  CONCERN_STOP
	// GOAL_NEW     GOAL_REFUSE
	// DRUG_NEW     DRUG_CHANGED  DRUG_DELETED
	// DOCUM_NEW
	// MISCELLANOUS

	TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
  	return ;

	do
	{
		// Vue principale : Ligne de vie
		//
		NSLdvView* pMainLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView) ;
		if (pMainLdvView)
			pMainLdvView->reloadView(sReason, sReference) ;
    else
    {
    	// Vue secondaire
      //
      NSLDVView* pLdvView = TYPESAFE_DOWNCAST(pView, NSLDVView) ;
      if (pLdvView)
      	pLdvView->reloadView(sReason) ;
    }

		pView = NextView(pView) ;
	}
	while (pView) ;
}

void
NSLdvDocument::viewsReloadDrugToons()
{
	TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
		return ;

	do
	{
		NSLdvView* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView) ;
		if (pLdvView)
			pLdvView->reinitDrugs() ;

		pView = NextView(pView) ;
	}
  while (pView) ;
}

NSLdvView*
NSLdvDocument::getLdvView(string sConcern)
{
	TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
		return (NSLdvView*) 0 ;

	do
	{
  	// LdvView ?
    //
		NSLdvView* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView) ;
		if (pLdvView)
    {
    	string sViewConcern = pLdvView->getConcern() ;
      if (sViewConcern == sConcern)
      	return pLdvView ;
    }

		pView = NextView(pView) ;
	}
  while (pView) ;

	return (NSLdvView*) 0 ;
}

NSDrugView*
NSLdvDocument::getDrugView(string sConcern)
{
	TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
		return (NSDrugView*) 0 ;

	do
	{
  	// DrugView ?
    //
		NSDrugView* pDrugView = TYPESAFE_DOWNCAST(pView, NSDrugView);
		if (pDrugView)
    {
    	string sViewConcern = pDrugView->getConcern() ;
      if (sViewConcern == sConcern)
      	return pDrugView ;
    }

		pView = NextView(pView);
	}
  while (pView) ;

	return (NSDrugView*) 0 ;
}

NSProcessView*
NSLdvDocument::getProcessView(string sConcern)
{
	TView* pView = GetViewList() ;
	if ((TView*) NULL == pView)
		return NULL ;

	do
	{
  	// NSProcessView ?
    //
		NSProcessView* pProcessView = TYPESAFE_DOWNCAST(pView, NSProcessView) ;
		if (pProcessView)
    {
    	string sViewConcern = pProcessView->getConcern() ;
      if (sViewConcern == sConcern)
      	return pProcessView ;
    }

		pView = NextView(pView) ;
	}
	while (pView) ;

	return (NSProcessView*) 0 ;
}

NSGoalView*
NSLdvDocument::getGoalView(string sConcern)
{
	TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
		return (NSGoalView*) 0 ;

	do
	{
		// GoalView ?
    //
		NSGoalView* pGoalView = TYPESAFE_DOWNCAST(pView, NSGoalView) ;
		if (pGoalView)
    {
    	string sViewConcern = pGoalView->getConcern() ;
      if (sViewConcern == sConcern)
      	return pGoalView ;
    }

		pView = NextView(pView) ;
	}
	while (pView) ;

	return (NSGoalView*) 0 ;
}

bool
NSLdvDocument::getStorageDocumentsIds(VecteurString *pIdsVector, LDVFRAME iFrame, NSRootLink::NODELINKTYPES iRelation)
{
  if ((VecteurString*) NULL == pIdsVector)
    return false ;

  NSPatientChoisi* pPatientChoisi = pContexte->getPatient() ;
	if ((NSPatientChoisi*) NULL == pPatientChoisi)
		return false ;

  // First step, find the root tree for frame
  //
  DocumentIter iterRootDoc = pPatientChoisi->getFrameIndex(iFrame) ;
  if ((DocumentIter) NULL == iterRootDoc)
  	return false ;

  string sFrameIndexMeta = (*iterRootDoc)->getCodeDocMeta() ;

  return NSLdvDocumentBase::getStorageDocumentsIds(pIdsVector, sFrameIndexMeta, iRelation) ;
}

