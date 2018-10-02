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

// #include <owl\filedoc.h>
// #include <iostream.h>
#include <string>

#include "nssavoir\nsBdmDriver.h"
// #include "partage\nsdivfct.h"
// #include "nautilus\nssuper.h"
// #include "nautilus\nsdecode.h"
// #include "nsbb\nsbb.h"
// #include "nautilus\nsrechd2.h"
#include "dcodeur\nsdkd.h"
// #include "dcodeur\decoder.h"
#include "dcodeur\nsphrase.h"
#include "dcodeur\nsgen.h"
#include "nautilus\nshistdo.h"
// #include "nautilus\nstrihis.h"
// #include "nssavoir\nsgraphe.h"
#include "nssavoir\nscaptur.h"
// #include "nautilus\nscsdoc.h"
// #include "dcodeur\nsdkd.h"
// #include "nsbb\nsbbtran.h"
// #include "nsbb\nstlibre.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nsarc.h"
#include "nsbb\tagNames.h"
// #include "nsldv\nsldvstructs.h"
#include "nssavoir\nsrightsmanager.h"
#include "nsldv\nsldvdoc_base.h"
#include "nsldv\nsldvvue_base.h"
#include "nsldv\nsldvgoal.h"
#include "nsldv\nsldvdrug.h"
// #include "nautilus\nsdrugview.h"
// #include "nautilus\nsgoalview.h"
// #include "nautilus\nsprocessview.h"
// #include "nautilus\nsdocview.h"
// #include "nautilus\nscompub.h"
// #include "nautilus\ns_html.h"
#include "nsoutil\ns_arche.h"
// #include "nsoutil\ibdm.h"
#include "nsepisod\nsToDo.h"
#include "nsepisod\nsSOAPdiv.h"
#include "nsepisod\nsclasser.h"
// #include "nautilus\nssoapview.h"
#include "nautilus\nsepicap.h"

// #include "ns_ob1\TypedVal.h"
// #include "ns_ob1\NautilusType.h"
// #include "ns_ob1\BB1Task.h"
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\BB1KS.h"
#include "ns_ob1\BB1types.h"
// #include "ns_ob1\Interface.h"

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
// long NSLdvDrugTake::lObjectCount = 0 ;
// long NSLdvDrugCycle::lObjectCount = 0 ;
// long NSLdvDrugPhase::lObjectCount = 0 ;
// long NSLdvDrug::lObjectCount = 0 ;
long NSFrameInformation::lObjectCount = 0 ;

// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSLdvDocument -------------------------
// --------------------------------------------------------------------------


//---------------------------------------------------------------------------
//            Constructeur
//---------------------------------------------------------------------------

NSLdvDocumentBase::NSLdvDocumentBase(TDocument* parent, NSContexte* pCtx)
                  :TDocument(parent), NSRoot(pCtx), aFrameIndexes(this),
                   _aObjets(this), _aSousObjets(this)
{
try
{
	SetTitle("Ligne de Vie") ;

	// Initialisation de la langue
	_sLangue = pContexte->getUserLanguage() ;

  // Creation and initialization of information structures
	//
  string sTraceText = string("NSLdvDocument loading frames.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

  loadFrames() ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    sTraceText = string("NSLdvDocument init concerns for frame ") + IntToString(i) ;
    pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

    aFrameIndexes.initConcernsForFrameInformation(iFrame) ;

    sTraceText = string("NSLdvDocument init treatments for frame ") + IntToString(i) ;
    pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

    aFrameIndexes.initTreatmentsForFrameInformation(iFrame) ;
  }

  // _pQoLCharts  = new NSQualityOfLifeChartArray() ; // must be created before _pObjets->initialiser()
	// _pObjets     = new ArrayObjets(this) ;

  sTraceText = string("NSLdvDocument init objects.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	_aObjets.initialiser() ;

	// _pSousObjets = new ArraySsObjet(this) ;

  sTraceText = string("NSLdvDocument init sub-objects.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	_aSousObjets.initialiser() ;

  //
  // Do it last, since Goals will get connected to concerns and drugs
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    sTraceText = string("NSLdvDocument init goals for frame ") + IntToString(i) ;
    pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

    aFrameIndexes.initGoalsForFrameInformation(getFrameForIndex(i)) ;
  }

  sTraceText = string("NSLdvDocument is now ready.") ;
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

NSLdvDocumentBase::NSLdvDocumentBase(const NSLdvDocumentBase& rv)
                  :TDocument(rv.GetParentDoc()), NSRoot(rv.pContexte),
                   aFrameIndexes(rv.aFrameIndexes), _aObjets(rv._aObjets),
                   _aSousObjets(rv._aSousObjets), _aQoLCharts(rv._aQoLCharts)
{
try
{
	_sLangue = rv._sLangue ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase copy ctor", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Description : Destructeur
//---------------------------------------------------------------------------

NSLdvDocumentBase::~NSLdvDocumentBase()
{
}

void
NSLdvDocumentBase::loadFrames()
{
  // First step: connect frame indexes
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
    aFrameIndexes.openFrameInformation(getFrameForIndex(i)) ;

/*  Now done in nschoisi.cpp

  // Second step: check if all indexes were found
  //
  int iNbIndex = 0 ;

  int i = 0 ;
  for ( ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      NSDocumentHisto* pDoc = pFrameInfo->getIndexDocument() ;
      if (pDoc)
      {
        NSPatPathoArray *pPpt = pDoc->getPatPatho() ;
        if (pPpt)
        {
          iNbIndex++ ;
          bool bNeedFixing = false ;

          // "Concerns" branch
          PatPathoIter iter = pPpt->ChercherItem("0PRO11") ;
          if ((NULL == iter) || (pPpt->end() == iter))
          {
            bNeedFixing = true ;
            pPpt->ajoutePatho("0PRO11", 1) ;
          }
          // "Goals" branch
          iter = pPpt->ChercherItem("0OBJE1") ;
          if ((NULL == iter) || (pPpt->end() == iter))
          {
            bNeedFixing = true ;
            pPpt->ajoutePatho("0OBJE1", 1) ;
          }
          // "Concerns" branch
          iter = pPpt->ChercherItem("N00001") ;
          if ((NULL == iter) || (pPpt->end() == iter))
          {
            bNeedFixing = true ;
            pPpt->ajoutePatho("N00001", 1) ;
          }
          if (bNeedFixing)
            updateIndexDocument(pDoc, false) ;
        }
      }
    }
  }

  // If Ok, lets quit
  //
  if (FRAMECOUNT == iNbIndex)
    return ;

  // string sQuestionText = pContexte->getSuperviseur()->getText("LigneDeVieErrors", "doYouWantToCreateMissingIndexes?") ;
	// int iAnswer = MessageBox(0, sQuestionText.c_str(), 0, MB_YESNO) ;

  // if (ID_NO == iAnswer)
  //  return ;

  // Create missing indexes
  //
  NSPersonGraphManager* pGraphPerson = pContexte->getPatient()->getGraphPerson() ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    if (NULL == getFrameIndex(iFrame))
    {
      NSRefDocument	PbIndex(0, 0, 0, pContexte, false) ;
      PbIndex.createFrame(pGraphPerson, iFrame) ;

      string sMetaID = PbIndex.sCodeDocMeta ;
      pGraphPerson->commitGraphTree(sMetaID, pidsPatient) ;

      string sTreeID = PbIndex.DonneDocInfo()->getID() ;
      pGraphPerson->commitGraphTree(sTreeID, pidsPatient) ;

      PbIndex.updateAfterSave(sTreeID, sMetaID) ;
      pContexte->getPatient()->getDocHis()->Rafraichir(PbIndex.pDocInfo, PbIndex.pPatPathoArray, &PbIndex) ;
    }
  }
  // Iterate
  //
  loadFrames() ;
*/
}

//---------------------------------------------------------------------------
//  Open
//---------------------------------------------------------------------------

bool
NSLdvDocumentBase::Open(int mode, const char far* path)
{
	return true ;
}

//---------------------------------------------------------------------------
//  Description : Operateur =
//---------------------------------------------------------------------------

NSLdvDocumentBase&
NSLdvDocumentBase::operator=(const NSLdvDocumentBase& src)
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
	erreur("Exception NSLdvDocumentBase::operator=.", standardError, 0) ;
	return *this ;
}
}

ArrayConcern*
NSLdvDocumentBase::getConcerns(LDVFRAME iFrame)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return 0 ;

  return pFrameInfo->getConcernsArray() ;
}

/* now in header file
ArrayGoals*
NSLdvDocumentBase::getGoals(LDVFRAME iFrame)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return 0 ;

  return pFrameInfo->getGoalsArray() ;
}
*/

ArrayLdvDrugs*
NSLdvDocumentBase::getDrugs(LDVFRAME iFrame)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return 0 ;

  return pFrameInfo->getTreatmentsArray() ;
}

/*
NSPatPathoArray*
NSLdvDocumentBase::getFrameIndex(LDVFRAME iFrame)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return 0 ;

  return pFrameInfo->getPatPathoIndex() ;
}
*/

void
NSLdvDocumentBase::initFromFrameIndex(LDVFRAME iFrame, NSPatPathoArray* pPpt) const
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

  pPpt->vider() ;

  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
    return ;

  pFrameInfo->initFromPatPathoIndex(pPpt) ;
}

NSConcern*
NSLdvDocumentBase::getConcern(string sNode, LDVFRAME &iFrame)
{
  // First step, look in the specified frame, if any
  //
  if (ldvframeNotSpecified != iFrame)
  {
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      NSConcern* pConcern = pFrameInfo->getConcern(sNode) ;
      if (pConcern)
        return pConcern ;
    }
  }
  // No specified frame, or not found in specified frame: look in every frames
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iProposedFrame = getFrameForIndex(i) ;

    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iProposedFrame) ;
    if (pFrameInfo)
    {
      NSConcern* pConcern = pFrameInfo->getConcern(sNode) ;
      if (pConcern)
      {
        iFrame = iProposedFrame ;
        return pConcern ;
      }
    }
  }
  // Really not found!
  //
  return 0 ;
}

NSLdvDrug*
NSLdvDocumentBase::getTreatment(string sNode, LDVFRAME &iFrame)
{
  // First step, look in the specified frame, if any
  //
  if (ldvframeNotSpecified != iFrame)
  {
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      NSLdvDrug* pTreatment = pFrameInfo->getTreatment(sNode) ;
      if (pTreatment)
        return pTreatment ;
    }
  }
  // No specified frame, or not found in specified frame: look in every frames
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iProposedFrame = getFrameForIndex(i) ;

    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iProposedFrame) ;
    if (pFrameInfo)
    {
      NSLdvDrug* pTreatment = pFrameInfo->getTreatment(sNode) ;
      if (pTreatment)
      {
        iFrame = iProposedFrame ;
        return pTreatment ;
      }
    }
  }
  // Really not found!
  //
  return 0 ;
}

NSLdvGoal*
NSLdvDocumentBase::getGoal(string sNode, LDVFRAME &iFrame)
{
  // First step, look in the specified frame, if any
  //
  if (ldvframeNotSpecified != iFrame)
  {
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      NSLdvGoal* pGoal = pFrameInfo->getGoal(sNode) ;
      if (pGoal)
        return pGoal ;
    }
  }
  // No specified frame, or not found in specified frame: look in every frames
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iProposedFrame = getFrameForIndex(i) ;

    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iProposedFrame) ;
    if (pFrameInfo)
    {
      NSLdvGoal* pGoal = pFrameInfo->getGoal(sNode) ;
      if (pGoal)
      {
        iFrame = iProposedFrame ;
        return pGoal ;
      }
    }
  }
  // Really not found!
  //
  return 0 ;
}

NSFrameInformation*
NSLdvDocumentBase::getFrameForNode(string sNode)
{
  if (string("") == sNode)
    return (NSFrameInformation*) 0 ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      NSDocumentHisto* pFoundDoc = pFrameInfo->findNode(sNode) ;
      if (pFoundDoc)
        return pFrameInfo ;
    }
  }

  return (NSFrameInformation*) 0 ;
}

LDVFRAME
NSLdvDocumentBase::getFrameTypeFromRoot(string sRootCode)
{
  if (string("") == sRootCode)
    return ldvframeNotSpecified ;

  string sRootSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sRootCode, &sRootSens) ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    string sFrameRoot = getRootForFrame(iFrame) ;

    string sFrameRootSens = string("") ;
    pContexte->getDico()->donneCodeSens(&sFrameRoot, &sFrameRootSens) ;

    if (sFrameRootSens == sRootSens)
      return iFrame ;
  }

  return ldvframeNotSpecified ;
}

//
// ----------------------- Drug oriented services -----------------------
//

/**
 *  Add a new drug
 */
NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugNewService(TWindow* pCallingView, string sDrugLexiCode, VecteurString* pRelatedConcerns)
{
try
{
	if (false == pContexte->userHasPrivilege(NSContexte::createDrug, -1, -1, string(""), string(""), NULL, NULL))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

	NSPatPathoArray* pPPT = new NSPatPathoArray(pContexte->getSuperviseur()) ;
  if (string("") != sDrugLexiCode)
  	pPPT->ajoutePatho(sDrugLexiCode, 0) ;

  NSSimpleNewDrugDlg MedicDlg(pCallingView, pContexte, pPPT, string("")) ;

  if (pRelatedConcerns && (false == pRelatedConcerns->empty()))
    MedicDlg.initConnectedIssues(pRelatedConcerns) ;

  int iResult = MedicDlg.Execute() ;
  bool bComplexMode = MedicDlg.mustSwitchToComplexMode() ;

  if (bComplexMode)
	{
  	NSMedicamentDlg ComplexMedicDlg(pCallingView, pContexte, pPPT) ;
    iResult = ComplexMedicDlg.Execute() ;

    // NSComplexMedicamentDlg *pComplexMedicDlg = new NSComplexMedicamentDlg(pCallingView, pContexte, &PPT) ;
    // iResult = pComplexMedicDlg->Execute() ;
		// delete (pComplexMedicDlg) ;
	}

  if (IDOK != iResult)
  {
    delete pPPT ;
  	return ldvseCanceledByUser ;
  }

	if (pPPT->empty())
  {
    delete pPPT ;
  	return ldvseInvalidData ;
  }

	// on enregistre ici dans l'index de santé
  //
  if (pRelatedConcerns)
    *pRelatedConcerns = *(MedicDlg.getUpdatedLinkedConcern()) ;

	// pContexte->getPatient()->CreeTraitement(&PPT, pRelatedConcerns) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("CreateMedication")) ;
  pTask->setPointer1(pPPT, true) ;
  pTask->setPointer2(new VecteurString(*pRelatedConcerns), true) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  // invalidateViews("DRUG_NEW") ; // Already done by CreeTraitement

  return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::DrugNewService : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::DrugNewService.", standardError, 0) ;
  return ldvseUndefined ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugFromProtocolService(TWindow* pCallingView, VecteurString* pRelatedConcerns)
{
try
{
	if (false == pContexte->userHasPrivilege(NSContexte::createDrug, -1, -1, string(""), string(""), NULL, NULL))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

  ReferentialChoiceDialog *pProtocolSelect = new ReferentialChoiceDialog(pCallingView, pContexte, ldvframeHealth, string("N0000"), ReferentialChoiceDialog::sortDrugs) ;
  int iResult = pProtocolSelect->Execute() ;

  if (IDOK != iResult)
	{
  	delete (pProtocolSelect) ;
  	return ldvseCanceledByUser ;
	}

  string sRefID = pProtocolSelect->getSelectedRefId() ;

  delete (pProtocolSelect) ;

	if (string("") == sRefID)
  	return ldvseInvalidData ;

	// on enregistre ici dans l'index de santé
  //
	// pContexte->getPatient()->CmGenericReferentiel(sRefID, string("")) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("GenericReferential")) ;
  pTask->setParam1(sRefID) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  // invalidateViews("DRUG_NEW") ; // Already done by CreeTraitement

  return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::DrugFromProtocolService : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::DrugFromProtocolService.", standardError, 0) ;
  return ldvseUndefined ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugRenewService(TWindow* pCallingView, string sDrugNode)
{
try
{
	if (string("") == sDrugNode)
		return ldvseNothingToDo ;

	if (!(pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

	NSPatPathoArray* pPPT = new NSPatPathoArray(pContexte->getSuperviseur()) ;

	NSRenewMedicDlg* pRenewMedicDlg = new NSRenewMedicDlg(pCallingView, pContexte, pPPT) ;
  int iResult = pRenewMedicDlg->Execute() ;
  delete pRenewMedicDlg ;

	if (IDCANCEL == iResult)
		return ldvseCanceledByUser ;

  VecteurString* pNodeMedic = new VecteurString() ;
	pNodeMedic->AddString(sDrugNode) ;

	// on renouvelle la précédente prescription
  //
	// pContexte->getPatient()->RenouvelerTraitement(&NodeMedic, &PPT) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("RenewDrugFromNodes")) ;
  pTask->setPointer1(pNodeMedic, true) ;
  pTask->setPointer2(pPPT, true) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  // invalidateViews("DRUG_RENEW") ; // Already done by RenouvelerTraitement

  return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::DrugRenewService : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::DrugRenewService.", standardError, 0) ;
  return ldvseUndefined ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugModifyService(TWindow* pCallingView, string sDrugNode)
{
try
{
	if (string("") == sDrugNode)
		return ldvseNothingToDo ;

	if (!(pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

  // Get drug from node
  //
  LDVFRAME iFrame = ldvframeNotSpecified ;

  NSLdvDrug* pDrug = getTreatment(sDrugNode, iFrame) ;
  if ((NSLdvDrug*) NULL == pDrug)
  	return ldvseInvalidData ;

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  string sCodeModif = pDrug->getLexique() ;

  // récupération de la date du jour
  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  // on récupère d'abord la sous-patpatho du noeud à modifier
  NSPatPathoArray SousPPT(pContexte->getSuperviseur()) ;
  pContexte->getPatient()->DonneSousArray(sDrugNode, &SousPPT) ;
  // on reconstitue la patpatho à partir du noeud
  PPT.ajoutePatho(sCodeModif, 0) ;
  // Insertion en queue (iter doit être ici egal à end())
  PPT.InserePatPatho(PPT.end(), &SousPPT, 1) ;

  // La patpatho source est constituée. On doit maintenant remplacer l'ancienne
  // date d'ouverture par la date du jour (car les modifs éventuelles datent de ce jour)
  PatPathoIter pptIt = PPT.begin() ;
  // Recherche de la date d'ouverture en sous-niveau du noeud  //  PatPathoIter pptItValeur ;  bool bDateTrouvee = false ;  int iColBase = (*pptIt)->getColonne() ;  pptIt++ ;  while ((PPT.end() != pptIt) && ((*pptIt)->getColonne() > iColBase))  {
    string sSens = (*pptIt)->getLexiqueSens() ;

    if ((*pptIt)->getColonne() == iColBase + 1)
    {
      // Dates
      if (sSens == "KOUVR")
      {
        pptIt++ ;
        int iLigneBase = (*pptIt)->getLigne() ;
        string sUnite  = string("") ;
        string sFormat = string("") ;
        string sValeur = string("") ;
        string sTemp   = string("") ;

        while ((PPT.end() != pptIt) &&
                ((*pptIt)->getLigne() == iLigneBase))
        {
          if (pContexte->getDico()->CodeCategorie((*pptIt)->getLexique()) == string("£"))
          {
            sFormat = (*pptIt)->getLexiqueSens() ;
            sValeur = (*pptIt)->getComplement() ;
            sUnite  = (*pptIt)->getUnitSens() ;
            pptItValeur = pptIt ;
            break ;
          }
          pptIt++ ;
        }

        // sFormat est du type £D0;03
        if ((string("") != sFormat) && (('D' == sFormat[1]) || ('T' == sFormat[1])) && (string("") != sValeur))
        {
          if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
          {
            // on remplace la date d'ouverture par la date du jour
            (*pptItValeur)->setComplement(tpsNow.donneDateHeure()) ;

            bDateTrouvee = true ;
          }
        }
      }
      else
        pptIt++ ;
    }
    else
      pptIt++ ;
  }

  if (false == bDateTrouvee)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("drugManagementErrors", "drugToEditHasNoStartingDate") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return ldvseUndefined ;
  }

  // on crée une copie
  NSPatPathoArray PPTCopy(PPT) ;

  int iResult ;
  bool bComplexMode = false ;

  NSSimpleNewDrugDlg* pSimpleMedicDialog = new NSSimpleNewDrugDlg(pContexte->GetMainWindow(), pContexte, &PPT, sDrugNode) ;
  bool bEnableToParse = pSimpleMedicDialog->ParseMedicament() ;
  if (true == bEnableToParse)
  {
    pSimpleMedicDialog->initializeConnectedIssues(sDrugNode) ;

  	iResult = pSimpleMedicDialog->Execute() ;
  	bComplexMode = pSimpleMedicDialog->mustSwitchToComplexMode() ;
  }
  else
  	bComplexMode = false ;

	delete pSimpleMedicDialog ;

	if (true == bComplexMode)
  {
  	/*NSMedicDlg */NSMedicamentDlg *pMedicDialog = new NSMedicamentDlg(pContexte->GetMainWindow(), pContexte, &PPT) ;
  	iResult = pMedicDialog->Execute() ;  	delete pMedicDialog ;  }  if (IDOK != iResult)    return ldvseCanceledByUser ;  // Si aucune modif : on sort...  if (PPT.estEgal(&PPTCopy))    return ldvseNothingToDo ;  VecteurString NodeArret ;
  NodeArret.AddString(sDrugNode) ;  // on cloture la précédente prescription  // pContexte->getPatient()->ArreterElement(&NodeArret, tpsNow.donneDateHeure()) ;  //  NSToDoTask* pTask = new NSToDoTask ;  pTask->setWhatToDo(string("StopElementsFromNodes")) ;
  pTask->setPointer1(new VecteurString(NodeArret), true) ;
  pTask->setParam1("DRUG_CHANGED") ;
  pTask->sendMe(pContexte->getSuperviseur()) ;  // on enregistre la nouvelle prescription dans l'index de santé  if (PPT.empty())
    return ldvseNoError ;

  // pContexte->getPatient()->CreeTraitement(&PPT, &(pDrug->_aConcerns)) ;
  //
  NSToDoTask* pTask1 = new NSToDoTask ;
  pTask1->setWhatToDo(string("CreateMedication")) ;
  pTask1->setPointer1(new NSPatPathoArray(PPT), true) ;
  pTask1->setPointer2(new VecteurString(pDrug->_aConcerns), true) ;
  pTask1->sendMe(pContexte->getSuperviseur()) ;

  // invalidateViews("DRUG_CHANGED") ;

	return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::DrugModifyService : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::DrugModifyService.", standardError, 0) ;
  return ldvseUndefined ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugChangePosoService(TWindow* pCallingView, string sDrugNode)
{
	if (string("") == sDrugNode)
		return ldvseNothingToDo ;

  LDVFRAME iFrame = ldvframeNotSpecified ;

	NSLdvDrug* pLdvDrug = getTreatment(sDrugNode, iFrame) ;
  if (NULL == pLdvDrug)
		return ldvseInvalidData ;

	return DrugChangePosoService(pCallingView, pLdvDrug) ;
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugChangePosoService(TWindow* pCallingView, NSLdvDrug* pLdvDrug)
{
	if (NULL == pLdvDrug)
		return ldvseNothingToDo ;

	if (!(pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

  string sCodeModif = pLdvDrug->getLexique() ;
  string sDrugNode  = pLdvDrug->getNoeud() ;

  // récupération de la date du jour
  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  // on récupère d'abord la sous-patpatho du noeud à modifier
  // getting the tree for drug to modify
  NSPatPathoArray drugPPT(pContexte->getSuperviseur()) ;
  pContexte->getPatient()->DonneArray(sDrugNode, &drugPPT) ;

  // Getting the active phase and its tree
  //
	NSLdvDrugPhase* pActivePhase = pLdvDrug->getCurrentActivePhase() ;
  if (NULL == pActivePhase)
  	return ldvseNothingToDo ;

	string sPhaseNode = pActivePhase->getNoeud() ;
  if (string("") == sPhaseNode)
		return ldvseNothingToDo ;
	NSPatPathoArray phasePPT(pContexte->getSuperviseur()) ;
	pContexte->getPatient()->DonneArray(sPhaseNode, &phasePPT) ;

  // on crée une copie - creating a copy
  NSPatPathoArray PPTCopy(phasePPT) ;

	NSMedicModifPosoDlg *pModifPosoDialog = new NSMedicModifPosoDlg(pContexte->GetMainWindow(), pContexte, &phasePPT, &drugPPT) ;  int iResult = pModifPosoDialog->Execute() ;  delete pModifPosoDialog ;  if (iResult != IDOK)    return ldvseCanceledByUser ;	if (true == phasePPT.empty())		return ldvseNothingToDo ;	NVLdVTemps tStart = pActivePhase->_tDateOuverture ;	NVLdVTemps tEnd   = pActivePhase->_tDateFermeture ;
	// removing all that is not KCYTR in PPTCopy  if (false == PPTCopy.empty())	{		PatPathoIter copyIter = PPTCopy.begin() ;
    int iCol = (*copyIter)->getColonne() ;
    copyIter++ ;
    while (PPTCopy.end() != copyIter)
    {
    	if ((*copyIter)->getColonne() == iCol + 1)
      {
      	string sSens = (*copyIter)->getLexiqueSens() ;
        if (string("KCYTR") != sSens)
        	PPTCopy.SupprimerItem(copyIter) ;
        else
        	copyIter++ ;
      }
      else
      	copyIter++ ;
    }
	}
  // Si aucune modif : on sort...  if (phasePPT.estEgal(&PPTCopy))    return ldvseNothingToDo ;	NSPatPathoArray PPTcycles(pContexte->getSuperviseur()) ;  phasePPT.ExtrairePatPatho(phasePPT.begin(), &PPTcycles) ;  phasePPT.SupprimerFils(phasePPT.begin()) ;	// Adding starting and ending dates to PPT  //  NSPatPathoArray PPTdates(pContexte->getSuperviseur()) ;	PPTdates.ajoutePatho("KOUVR1", 0) ;	Message Msg ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(tpsNow.donneDateHeure()) ;
	PPTdates.ajoutePatho("£T0;19", &Msg, 1) ;

	// Date de fin - Date of ending
	if ((false == tEnd.estVide()) && (false == tEnd.estNoLimit()))
	{
		PPTdates.ajoutePatho("KFERM1", 0) ;
		Message Msg ;
    Msg.SetUnit("2DA021") ;
		Msg.SetComplement(tEnd.donneDateHeure()) ;
		PPTdates.ajoutePatho("£T0;19", &Msg, 1) ;
	}	phasePPT.InserePatPathoFille(phasePPT.begin(), &PPTdates) ;  phasePPT.InserePatPathoFille(phasePPT.begin(), &PPTcycles) ;	// Close the previous phase  VecteurString NodeArret ;  NodeArret.AddString(sPhaseNode) ;  // pContexte->getPatient()->ArreterElement(&NodeArret, tpsNow.donneDateHeure()) ;  //  NSToDoTask* pTask = new NSToDoTask ;  pTask->setWhatToDo(string("StopElementsFromNodes")) ;
  pTask->setPointer1(new VecteurString(NodeArret), true) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;  // Add the new phase  // pContexte->getPatient()->addSubElement(sDrugNode, &phasePPT, true) ;  //  NSToDoTask* pTask2 = new NSToDoTask ;  pTask2->setWhatToDo(string("AddSubElement")) ;
  pTask2->setPointer1(new NSPatPathoArray(phasePPT), true) ;
  pTask2->setParam1(sDrugNode) ;
  pTask2->setParam2(string("true")) ;
  pTask2->sendMe(pContexte->getSuperviseur()) ;	return ldvseNoError ;}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugStopService(TWindow* pCallingView, string sDrugNode)
{
	if (string("") == sDrugNode)
		return ldvseNothingToDo ;

	if (false == pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

  VecteurString NodeArret ;
  NodeArret.AddString(sDrugNode) ;

  // récupération de la date du jour
  // NVLdVTemps tpsNow ;
  // tpsNow.takeTime() ;  // on cloture la précédente prescription  // pContexte->getPatient()->ArreterElement(&NodeArret, tpsNow.donneDateHeure()) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("StopElementsFromNodes")) ;
  pTask->setPointer1(new VecteurString(NodeArret), true) ;
  pTask->setParam1("DRUG_CHANGED") ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

	return ldvseNoError ;
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugDeleteService(TWindow* pCallingView, string sDrugNode)
{
	if (sDrugNode == string(""))
		return ldvseNothingToDo ;

	if (!(pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

  // pContexte->getPatient()->SupprimerElement(sDrugNode) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("DeleteElementFromNode")) ;
  pTask->setParam1(sDrugNode) ;
  pTask->setParam2("DRUG_DELETED") ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvDrug* pLdvDrug = getTreatment(sDrugNode, iFrame) ;
  if ((NSLdvDrug*) NULL == pLdvDrug)
		return ldvseInvalidData ;

  ArrayLdvDrugs* pDrugs = 0 ;

  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (pFrameInfo)
    pDrugs = pFrameInfo->getTreatmentsArray() ;

  if ((ArrayLdvDrugs*) NULL == pDrugs)
    return ldvseInvalidData ;

  pDrugs->deleteDrug(sDrugNode) ;

	return ldvseNoError ;
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugCreatePrescriptionService(TWindow* pCallingView)
{
	// pContexte->getPatient()->CreeOrdonnance(true) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("CreatePrescription")) ;
  pTask->setParam1(string("true")) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

	return ldvseNoError ;
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugCreatePrescriptionForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs, bool bSecured)
{
	if (((VecteurString*) NULL == pDrugsRefs) || (true == pDrugsRefs->empty()))
		return ldvseNothingToDo ;

	// pContexte->getPatient()->CreeOrdonnanceFromSelection(true, pDrugsRefs, false) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("CreatePrescriptionFromSelection")) ;
  pTask->setPointer1(new VecteurString(*pDrugsRefs), true) ;
  pTask->setParam1(string("true")) ;
  pTask->setParam2(string("false")) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

	return ldvseNoError ;
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugCreateNoNamePrescriptionForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs, bool bSecured)
{
	if (((VecteurString*) NULL == pDrugsRefs) || (true == pDrugsRefs->empty()))
		return ldvseNothingToDo ;

	// pContexte->getPatient()->CreeOrdonnanceFromSelection(true, pDrugsRefs, true) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("CreatePrescriptionFromSelection")) ;
  pTask->setPointer1(new VecteurString(*pDrugsRefs), true) ;
  pTask->setParam1(string("true")) ;
  pTask->setParam2(string("true")) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

	return ldvseNoError ;
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugSwitchToDciForSelectionService(TWindow* pCallingView, ArrayCopyDrugs* pDrugs)
{
  if (((ArrayCopyDrugs*) NULL == pDrugs) || (true == pDrugs->empty()))
		return ldvseNothingToDo ;

  drugsIter it = pDrugs->begin() ;
  string sNodeId = (*it)->getNoeud() ;

  NSFrameInformation* pFrameInfo = getFrameForNode(sNodeId) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
    return ldvseNothingToDo ;

  ArrayLdvDrugs* pModifiedDrugs = new ArrayLdvDrugs(pFrameInfo) ;

	for (drugsIter drugIter = pDrugs->begin() ; pDrugs->end() != drugIter ; drugIter++)
	{
    NSLdvDrug* pDrug = new NSLdvDrug(**drugIter) ;

    string sDrugCode = pDrug->getLexique() ;

    // Find the virtual drug using the semantic network
    //
    string sVirtualDrugCode = pContexte->getSuperviseur()->getFilGuide()->getVirtualDrug(sDrugCode, pContexte) ;
    if (string("") == sVirtualDrugCode)
    {
      // If not found using the semantic network, try using the drugs database
      //
      NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
      if ((NSBdmDriver*) NULL != pDriver)
        sVirtualDrugCode = pDriver->getVirtualDrug(sDrugCode) ;
    }

    if (string("") != sVirtualDrugCode)
    {
      pContexte->getDico()->donneCodeComplet(sVirtualDrugCode) ;
      pDrug->setLexique(sVirtualDrugCode) ;
      pModifiedDrugs->push_back(pDrug) ;
    }
  }

  if (pModifiedDrugs->empty())
  {
    delete pModifiedDrugs ;
    return ldvseNothingToDo ;
  }

  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("SwitchElementLexicon")) ;
  pTask->setPointer1(pModifiedDrugs, true) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

	return ldvseNoError ;
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugCreateProtocolForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs)
{
	if (((VecteurString*) NULL == pDrugsRefs) || (true == pDrugsRefs->empty()))
		return ldvseNothingToDo ;

  // Get frame from first element
  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvDrug* pDrugFromFrame = getTreatment(**(pDrugsRefs->begin()), iFrame) ;
  if ((NSLdvDrug*) NULL == pDrugFromFrame)
    return ldvseInterfaceError ;

try
{
	string sTitle   = string("") ;
	string sIdent   = string("") ;
  string sConcept = string("") ;
  string sRoot    = getRootForFrame(iFrame) ;

	ReferentialTitleDialog *pTitleDlg = new ReferentialTitleDialog(pCallingView, pContexte, &sTitle, &sIdent, &sConcept, &sRoot) ;
	int iResult = pTitleDlg->Execute() ;
	delete pTitleDlg ;

  if (IDOK != iResult)
  	return ldvseCanceledByUser ;

	// Creating and initing the Creferentiel object
  //
	Creferentiel referential("", "", 0, pContexte->getSuperviseur()) ;
	referential.addAttribute(ATTRIBUT_REFER_TITLE, sTitle) ;
	string sID = string("trait.ref.") + sIdent + string(".1.0") ;
  referential.addAttribute(ATTRIBUT_REFER_NAME, sID) ;

  // Create CValeur and Creferences
  //
  CValeur* pValeur = new CValeur(LABEL_REFERENCES, string(""), string(""), &referential) ;
	referential.vect_val.push_back(pValeur) ;

  pValeur->setObject(new Creferences(string(""), string(""), &referential)) ;

  Creferences* pRef = dynamic_cast<Creferences*>(pValeur->getObject()) ;
  CValeur* pValRef = new CValeur(LABEL_CONCERN, string(""), string(""), pRef) ;
	pRef->vect_val.push_back(pValRef) ;
  pValRef->setObject(new Cconcern(string(""), string(""), pRef)) ;
  pValRef->getObject()->addAttribute(ATTRIBUT_CONCERN_CATEGORY, string("N0000")) ;
  if (string("") != sConcept)
    pValRef->getObject()->addAttribute(ATTRIBUT_CONCERN_CODE, sConcept) ;
  if (string("") != sRoot)
    pValRef->getObject()->addAttribute(ATTRIBUT_CONCERN_FRAME, sRoot) ;

  VecteurString groupsLabelsArray ;

	// Create Propositions
  //
  string sMaxId = string("0") ;

  EquiItemIter itemsIter = pDrugsRefs->begin() ;
	for ( ; pDrugsRefs->end() != itemsIter ; itemsIter++)
	{
    LDVFRAME iFrame = ldvframeNotSpecified ;
    NSLdvDrug* pDrug = getTreatment(**itemsIter, iFrame) ;

    if (pDrug)
    {
    	string sPropTitle    = pDrug->_sTitreCourt ;
			string sPropGroup    = string("") ;
    	string sPropHelp     = string("") ;
      string sPropCheck    = string("") ;
      string sPropEvidence = string("") ;
      string sUncheckArche = string("") ;
			ReferentialPropParamsDialog *pParamsDlg = new ReferentialPropParamsDialog(pCallingView, pContexte, &sPropTitle, &sPropGroup, &sPropHelp, &(pDrug->_sTitreCourt), &(pDrug->_sTitre), &sPropCheck, &sPropEvidence, &sUncheckArche, &groupsLabelsArray) ;
			int iParamsResult = pParamsDlg->Execute() ;
			delete pParamsDlg ;

      if (IDOK == iParamsResult)
      {
        incremente_code(sMaxId) ;
        string sNumId = string("o") + sMaxId ;

      	CValeur* pValProp = new CValeur(LABEL_PROPOSITION, string(""), string(""), &referential) ;
				referential.vect_val.push_back(pValProp) ;

				pValProp->setObject(new Cproposition(string(""), string(""), &referential, pContexte->getSuperviseur())) ;

        Cproposition* pProposition = dynamic_cast<Cproposition*>(pValProp->getObject()) ;

        pProposition->addAttribute(ATTRIBUT_PROP_ID,           sNumId) ;
        pProposition->addAttribute(ATTRIBUT_PROP_NOM,          sPropTitle) ;

        if (string("") != sPropGroup)
          pProposition->addAttribute(ATTRIBUT_PROP_GROUPE,       sPropGroup) ;
        if (string("") != sPropHelp)
          pProposition->addAttribute(ATTRIBUT_PROP_HELP,         sPropHelp) ;
        if (string("") != sPropCheck)
          pProposition->addAttribute(ATTRIBUT_PROP_AUTOCHECK,    sPropCheck) ;
        if (string("") != sPropEvidence)
          pProposition->addAttribute(ATTRIBUT_PROP_EVIDENCE_LVL, sPropEvidence) ;
        if (string("") != sUncheckArche)
          pProposition->addAttribute(ATTRIBUT_PROP_UNCHECK_ARCH, sUncheckArche) ;

        CValeur* pValTree = new CValeur(LABEL_TREE, string(""), string(""), pProposition) ;
				pProposition->vect_val.push_back(pValTree) ;
				pValTree->setObject(new Ctree(string(""), string(""), pProposition, pContexte->getSuperviseur())) ;

        Ctree* pTree = dynamic_cast<Ctree*>(pValTree->getObject()) ;

        string sRootForFrame = getRootForFrame(iFrame) ;
        string sRootSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sRootForFrame, &sRootSens) ;
        sRootSens += string(1, cheminSeparationMARK) + string("N0000") ;

        createXmlTree(pDrug, pTree, sRootSens) ;
      }
    }
	}

	// Filename = Archetype ID where '.' are replaced with "_"
  //
	string sFileName = sID ;

  size_t j ;
	while ((j = sFileName.find(".")) != NPOS)
		sFileName.replace( j, 1, "_") ;

  sFileName += string(".xml") ;

  // Write the protocol file in the SysImport directory
  //
  ofstream outFile ;
  string sFileToSave = pContexte->PathName("INEW") + sFileName ;
  outFile.open(sFileToSave.c_str(), ios::ate | ios::app) ;
  if (!outFile)
  {
		string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFileToSave ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return ldvseInterfaceError ;
	}

	referential.inscritFichier(&outFile, 0, LABEL_REFERENTIEL) ;

  outFile.close() ;

  // Ask for the referencing of all that's in SysImport
  //
  // pContexte->getSuperviseur()->ReferenceTousArchetypes() ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("ReferenceArchetypes")) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

	return ldvseNoError ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::DrugCreateProtocolForSelectionService.", standardError, 0) ;
  return ldvseInterfaceError ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugAddToProtocolForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs)
{
try
{
	if ((NULL == pDrugsRefs) || (true == pDrugsRefs->empty()))
		return ldvseNothingToDo ;

  // Find if every selected items belong to the same frame
  //
  EquiItemIter itemsIter = pDrugsRefs->begin() ;
  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvDrug *pDrug = getTreatment(**itemsIter, iFrame) ;
  itemsIter++ ;
  for ( ; pDrugsRefs->end() != itemsIter ; itemsIter++)
	{
    LDVFRAME iTempFrame = ldvframeNotSpecified ;
    pDrug = getTreatment(**itemsIter, iTempFrame) ;
    if (pDrug && (iTempFrame != iFrame))
    {
      iFrame = ldvframeNotSpecified ;
      break ;
    }
  }
  if (ldvframeNotSpecified == iFrame)
    iFrame = ldvframeHealth ;

	// Selecting the protocol
  //
	ReferentialChoiceDialog *pProtocolSelect = new ReferentialChoiceDialog(pCallingView, pContexte, iFrame, string("N0000"), ReferentialChoiceDialog::sortDrugs) ;
  int iResult = pProtocolSelect->Execute() ;

  if (IDOK != iResult)
	{
  	delete (pProtocolSelect) ;
  	return ldvseCanceledByUser ;
	}

  string sRefID = pProtocolSelect->getSelectedRefId() ;

  delete (pProtocolSelect) ;

	if (string("") == sRefID)
  	return ldvseInvalidData ;

	// Opening the file and parsing it
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sRefFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::referentiel, sRefID) ;
  if (string("") == sRefFile)
    return ldvseInvalidData ;

  string ps = string("Parsing file ") + sRefFile ;
  pSuper->trace(&ps, 1) ;

  nsrefParseur Parseur(pContexte->getSuperviseur()) ;
  if (false == Parseur.open(sRefFile))
    return ldvseInvalidData ;

  if ((NULL == Parseur.getReferentiel()) || (NULL == Parseur.getReferentiel()->getValArray()))
  {
  	string sErrorText = pSuper->getText("referentialManagement", "invalidReferential") + string(" ") + sRefID ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ldvseInvalidData ;
  }

  //
  // Find max Id for existing propositions
  //
  string sMaxPropId = string("") ;
  if (false == Parseur.getReferentiel()->vect_val.empty())
  {
    Creferentiel* pRef = Parseur.getReferentiel() ;
    for (ValIter ival = pRef->vect_val.begin() ; pRef->vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet dialogue
      if (LABEL_PROPOSITION == (*ival)->getLabel())
      {
        Cproposition *pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;
        if (pCprop)
        {
          string sProId = pCprop->getStringAttribute(ATTRIBUT_PROP_ID) ;
          if ((strlen(sProId.c_str()) > strlen(sMaxPropId.c_str())) ||
              ((strlen(sProId.c_str()) == strlen(sMaxPropId.c_str())) && (sProId > sMaxPropId))
              )
            sMaxPropId = sProId ;
        }
      }
    }
  }
  if (string("") == sMaxPropId)
    sMaxPropId = string("o0") ;

  VecteurString groupsLabelsArray ;
  Parseur.getGroups(&groupsLabelsArray, _sLangue) ;

  // Creating new Propositions
  //
  itemsIter = pDrugsRefs->begin() ;
	for ( ; pDrugsRefs->end() != itemsIter ; itemsIter++)
	{
    LDVFRAME iFrame = ldvframeNotSpecified ;

    NSLdvDrug* pDrug = getTreatment(**itemsIter, iFrame) ;

    if (pDrug)
    {
    	string sPropTitle    = pDrug->_sTitreCourt ;
			string sPropGroup    = string("") ;
    	string sPropHelp     = string("") ;
      string sPropCheck    = string("") ;
      string sPropEvidence = string("") ;
      string sUncheckArche = string("") ;
			ReferentialPropParamsDialog *pParamsDlg = new ReferentialPropParamsDialog(pCallingView, pContexte, &sPropTitle, &sPropGroup, &sPropHelp, &(pDrug->_sTitreCourt), &(pDrug->_sTitre), &sPropCheck, &sPropEvidence, &sUncheckArche, &groupsLabelsArray) ;
			int iParamsResult = pParamsDlg->Execute() ;
			delete pParamsDlg ;

      if (IDOK == iParamsResult)
      {
        if ('o' == sMaxPropId[0])
        {
          if (strlen(sMaxPropId.c_str()) == 1)
            sMaxPropId = string("o1") ;
          else
          {
            string sCounter = string(sMaxPropId, 1, strlen(sMaxPropId.c_str()) - 1) ;
            incremente_code(sCounter) ;
            sMaxPropId = string("o") + sCounter ;
          }
        }
        else
          incremente_code(sMaxPropId) ;

      	CValeur* pValProp = new CValeur(LABEL_PROPOSITION, string(""), string(""), Parseur.getReferentiel()) ;
				Parseur.getReferentiel()->vect_val.push_back(pValProp) ;
				pValProp->setObject(new Cproposition(string(""), string(""), Parseur.getReferentiel(), pContexte->getSuperviseur())) ;

        Cproposition* pProposition = dynamic_cast<Cproposition*>(pValProp->getObject()) ;

        pProposition->addAttribute(ATTRIBUT_PROP_ID,           sMaxPropId) ;
        pProposition->addAttribute(ATTRIBUT_PROP_NOM,          sPropTitle) ;

        if (string("") != sPropGroup)
          pProposition->addAttribute(ATTRIBUT_PROP_GROUPE,       sPropGroup) ;
        if (string("") != sPropHelp)
          pProposition->addAttribute(ATTRIBUT_PROP_HELP,         sPropHelp) ;
        if (string("") != sPropCheck)
          pProposition->addAttribute(ATTRIBUT_PROP_AUTOCHECK,    sPropCheck) ;
        if (string("") != sPropEvidence)
          pProposition->addAttribute(ATTRIBUT_PROP_EVIDENCE_LVL, sPropEvidence) ;
        if (string("") != sUncheckArche)
          pProposition->addAttribute(ATTRIBUT_PROP_UNCHECK_ARCH, sUncheckArche) ;

        CValeur* pValTree = new CValeur(LABEL_TREE, string(""), string(""), pProposition) ;
				pProposition->vect_val.push_back(pValTree) ;
				pValTree->setObject(new Ctree(string(""), string(""), pProposition, pContexte->getSuperviseur())) ;

        Ctree* pTree = dynamic_cast<Ctree*>(pValTree->getObject()) ;

        string sRootForFrame = getRootForFrame(iFrame) ;
        string sRootSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sRootForFrame, &sRootSens) ;
        sRootSens += string(1, cheminSeparationMARK) + string("N0000") ;

        createXmlTree(pDrug, pTree, sRootSens) ;
      }
    }
	}

  // File saving
  //
	ofstream outFile ;
  string sFileToSave = sRefFile ;
  outFile.open(sFileToSave.c_str(), ios::ate | ios::trunc) ;
  if (!outFile)
  {
		string sErrorText = pSuper->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFileToSave ; 
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return ldvseInterfaceError ;
	}

	Parseur.getReferentiel()->inscritFichier(&outFile, 0, LABEL_REFERENTIEL) ;

  outFile.close() ;

	return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::DrugAddToProtocolForSelectionService : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::DrugAddToProtocolForSelectionService.", standardError, 0) ;
  return ldvseUndefined ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::DrugSelectProtocolForConcern(TWindow* pCallingView, string sRelatedConcern)
{
try
{
  string sConcernLexiqueCode = string("") ;
  LDVFRAME iFrame = ldvframeNotSpecified ;

  if (string("") != sRelatedConcern)
  {
    for (int i = 0 ; (i < FRAMECOUNT) && (string("") == sConcernLexiqueCode) ; i++)
    {
      iFrame = getFrameForIndex(i) ;
      NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
      if (pFrameInfo)
      {
        NSDocumentHisto* pFoundDoc = pFrameInfo->findNode(sRelatedConcern) ;
        if (pFoundDoc)
        {
          NSPatPathoArray PathoIndex(pContexte->getSuperviseur()) ;
          pFoundDoc->initFromPatPatho(&PathoIndex) ;

          if (false == PathoIndex.empty())
          {
	          PatPathoIter pptIter = PathoIndex.ChercherNoeud(sRelatedConcern) ;
            if ((NULL != pptIter) && (PathoIndex.end() != pptIter))
            {
              sConcernLexiqueCode = (*pptIter)->getLexiqueSens() ;
              break ;
            }
          }
        }
      }
    }
  }

  if (string("") == sConcernLexiqueCode)
    return ldvseInvalidData ;

  string sRefID = string("") ;

  ReferentialChoiceDialog *pProtocolSelect = new ReferentialChoiceDialog(pCallingView, pContexte, iFrame, string("N0000"), ReferentialChoiceDialog::sortDrugs, sConcernLexiqueCode) ;

  // Check if there is a single proposal
  //
  pProtocolSelect->InitArray() ;
  if (pProtocolSelect->_aArray.size() == 1)
  {
    RefInfoStorageIter it = pProtocolSelect->_aArray.begin() ;
    sRefID = (*it)->sRefId ;
  }
  else
  {
    // If there is no ref for the concept, we extand inquiry to semantic fathers
    //
    if (pProtocolSelect->_aArray.size() == 0)
    {
      pProtocolSelect->_bUseSemanticNetwork = true ;
      pProtocolSelect->InitArray() ;
    }

    int iResult = pProtocolSelect->Execute() ;

    if (IDOK != iResult)
	  {
  	  delete (pProtocolSelect) ;
  	  return ldvseCanceledByUser ;
	  }

    sRefID = pProtocolSelect->getSelectedRefId() ;
  }

  delete (pProtocolSelect) ;

	if (string("") == sRefID)
  	return ldvseInvalidData ;

  return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::DrugSelectProtocolForConcern: ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::DrugSelectProtocolForConcern.", standardError, 0) ;
  return ldvseUndefined ;
}
}

//
// Goal oriented services
//

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::GoalFromProtocolService(TWindow* pCallingView, LDVFRAME iFrame, VecteurString* pRelatedConcerns)
{
try
{
	if (!(pContexte->userHasPrivilege(NSContexte::createGoal, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

  ReferentialChoiceDialog *pProtocolSelect = new ReferentialChoiceDialog(pCallingView, pContexte, iFrame, string("0OBJE")) ;
  int iResult = pProtocolSelect->Execute() ;

  if (IDOK != iResult)
	{
  	delete (pProtocolSelect) ;
  	return ldvseCanceledByUser ;
	}

  string sRefID = pProtocolSelect->getSelectedRefId() ;

  delete (pProtocolSelect) ;

	if (string("") == sRefID)
  	return ldvseInvalidData ;

	// on enregistre ici dans l'index de santé
  //
	// pContexte->getPatient()->CmReferentiel(sRefID, string(""), iFrame) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("Referential")) ;
  pTask->setParam1(sRefID) ;
  pTask->setParam2(IntToString(getIndexForFrame(iFrame))) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::GoalFromProtocolService : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::GoalFromProtocolService.", standardError, 0) ;
  return ldvseUndefined ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::GoalSelectProtocolForConcern(TWindow* pCallingView, string sRelatedConcern)
{
try
{
	if (false == pContexte->userHasPrivilege(NSContexte::createGoal, -1, -1, string(""), string(""), NULL, NULL))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}

  string sConcernLexiqueCode = string("") ;
  LDVFRAME iFrame = ldvframeNotSpecified ;

  if (string("") != sRelatedConcern)
  {
    for (int i = 0 ; (i < FRAMECOUNT) && (string("") == sConcernLexiqueCode) ; i++)
    {
      iFrame = getFrameForIndex(i) ;
      NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
      if (pFrameInfo)
      {
        NSDocumentHisto* pFoundNode = pFrameInfo->findNode(sRelatedConcern) ;

        if (pFoundNode)
        {
          NSPatPathoArray PathoIndex(pContexte->getSuperviseur()) ;
          pFoundNode->initFromPatPatho(&PathoIndex) ;

	        PatPathoIter pptIter = PathoIndex.ChercherNoeud(sRelatedConcern) ;
          if ((NULL != pptIter) && (PathoIndex.end() != pptIter))
          {
            sConcernLexiqueCode = (*pptIter)->getLexiqueSens() ;
            break ;
          }
        }
      }
    }
  }

  if (string("") == sConcernLexiqueCode)
    return ldvseInvalidData ;
    
  string sRefID = string("") ;

  ReferentialChoiceDialog *pProtocolSelect = new ReferentialChoiceDialog(pCallingView, pContexte, iFrame, string("0OBJE"), ReferentialChoiceDialog::sortNone, sConcernLexiqueCode) ;

  // Check if there is a single proposal
  //
  pProtocolSelect->InitArray() ;
  if (pProtocolSelect->_aArray.size() == 1)
  {
    RefInfoStorageIter it = pProtocolSelect->_aArray.begin() ;
    sRefID = (*it)->sRefId ;
  }
  else
  {
    // If there is no ref for the concept, we extand inquiry to semantic fathers
    //
    if (pProtocolSelect->_aArray.size() == 0)
    {
      pProtocolSelect->_bUseSemanticNetwork = true ;
      pProtocolSelect->InitArray() ;
    }

    int iResult = pProtocolSelect->Execute() ;

    if (IDOK != iResult)
	  {
  	  delete (pProtocolSelect) ;
  	  return ldvseCanceledByUser ;
	  }

    sRefID = pProtocolSelect->getSelectedRefId() ;
  }

  delete (pProtocolSelect) ;

	if (string("") == sRefID)
  	return ldvseInvalidData ;

	// on enregistre ici dans l'index de santé
  //
	// pContexte->getPatient()->CmGenericReferentiel(sRefID, string(""), sRelatedConcern) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("GenericReferential")) ;
  pTask->setParam1(sRefID) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  // Prise en compte du  sur la Ligne de vie
  // invalidateViews("DRUG_NEW") ;  // Already done by CmGenericReferentiel

  return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::GoalSelectProtocolForConcern: ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::GoalSelectProtocolForConcern.", standardError, 0) ;
  return ldvseUndefined ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::GoalCreateProtocolForSelectionService(TWindow* pCallingView, VecteurString* pGoalsRefs)
{
	if ((NULL == pGoalsRefs) || (true == pGoalsRefs->empty()))
		return ldvseNothingToDo ;

try
{
  // Get frame from first element
  LDVFRAME iFrame = ldvframeNotSpecified ;
  // NSLdvGoal* pGoal = getGoal(**(pGoalsRefs->begin()), iFrame) ;

	string sTitle   = string("") ;
	string sIdent   = string("") ;
  string sConcept = string("") ;
  string sRoot    = getRootForFrame(iFrame) ;

	ReferentialTitleDialog *pTitleDlg = new ReferentialTitleDialog(pCallingView, pContexte, &sTitle, &sIdent, &sConcept, &sRoot) ;
	int iResult = pTitleDlg->Execute() ;
	delete pTitleDlg ;

  if (IDOK != iResult)
  	return ldvseCanceledByUser ;

	// Creating and initing the Creferentiel object
  //
	Creferentiel referential("", "", 0, pContexte->getSuperviseur()) ;
	referential.addAttribute(ATTRIBUT_REFER_TITLE, sTitle) ;
	string sID = string("goals.ref.") + sIdent + string(".1.0") ;
  referential.addAttribute(ATTRIBUT_REFER_NAME, sID) ;

  // Create CValeur and Creferences
  //
  CValeur* pValeur = new CValeur(LABEL_REFERENCES, string(""), string(""), &referential) ;
	referential.vect_val.push_back(pValeur) ;
  pValeur->setObject(new Creferences(string(""), string(""), &referential)) ;

  Creferences* pRef = dynamic_cast<Creferences*>(pValeur->getObject()) ;
  CValeur* pValRef = new CValeur(LABEL_CONCERN, string(""), string(""), pRef) ;
	pRef->vect_val.push_back(pValRef) ;
  pValRef->setObject(new Cconcern(string(""), string(""), pRef)) ;
  pValRef->getObject()->addAttribute(ATTRIBUT_CONCERN_CATEGORY, string("0OBJE")) ;
  if (string("") != sConcept)
    pValRef->getObject()->addAttribute(ATTRIBUT_CONCERN_CODE, sConcept) ;
  if (string("") != sRoot)
    pValRef->getObject()->addAttribute(ATTRIBUT_CONCERN_FRAME, sRoot) ;

  VecteurString groupsLabelsArray ;

	// Create Propositions
  //
  string sMaxId = string("0") ;

  EquiItemIter itemsIter = pGoalsRefs->begin() ;
	for ( ; pGoalsRefs->end() != itemsIter ; itemsIter++)
	{
    iFrame = ldvframeNotSpecified ;

    NSLdvGoal* pGoal = getGoal(**itemsIter, iFrame) ;

    if (pGoal)
    {
    	string sPropTitle = pGoal->_sComplementText ;
      if (string("") == sPropTitle)
        sPropTitle = pGoal->getTitle() ;

			string sPropGroup    = string("") ;
    	string sPropHelp     = string("") ;
      string sPropCheck    = string("") ;
      string sPropEvidence = string("") ;
      string sUncheckArche = string("") ;
			ReferentialPropParamsDialog *pParamsDlg = new ReferentialPropParamsDialog(pCallingView, pContexte, &sPropTitle, &sPropGroup, &sPropHelp, &(pGoal->getTitle()), &(pGoal->getTitle()), &sPropCheck, &sPropEvidence, &sUncheckArche, &groupsLabelsArray) ;
			int iParamsResult = pParamsDlg->Execute() ;
			delete pParamsDlg ;

      if (IDOK == iParamsResult)
      {
        incremente_code(sMaxId) ;
        string sNumId = string("o") + sMaxId ;

      	CValeur* pValProp = new CValeur(LABEL_PROPOSITION, string(""), string(""), &referential) ;
				referential.vect_val.push_back(pValProp) ;
				pValProp->setObject(new Cproposition(string(""), string(""), &referential, pContexte->getSuperviseur())) ;

        Cproposition* pProposition = dynamic_cast<Cproposition*>(pValProp->getObject()) ;

        pProposition->addAttribute(ATTRIBUT_PROP_ID,           sNumId) ;
        pProposition->addAttribute(ATTRIBUT_PROP_NOM,          sPropTitle) ;

        if (string("") != sPropGroup)
          pProposition->addAttribute(ATTRIBUT_PROP_GROUPE,       sPropGroup) ;
        if (string("") != sPropHelp)
          pProposition->addAttribute(ATTRIBUT_PROP_HELP,         sPropHelp) ;
        if (string("") != sPropCheck)
          pProposition->addAttribute(ATTRIBUT_PROP_AUTOCHECK,    sPropCheck) ;
        if (string("") != sPropEvidence)
          pProposition->addAttribute(ATTRIBUT_PROP_EVIDENCE_LVL, sPropEvidence) ;
        if (string("") != sUncheckArche)
          pProposition->addAttribute(ATTRIBUT_PROP_UNCHECK_ARCH, sUncheckArche) ;

        CValeur* pValTree = new CValeur(LABEL_TREE, string(""), string(""), pProposition) ;
				pProposition->vect_val.push_back(pValTree) ;
				pValTree->setObject(new Ctree(string(""), string(""), pProposition, pContexte->getSuperviseur())) ;

        Ctree* pTree = dynamic_cast<Ctree*>(pValTree->getObject()) ;

        string sRootForFrame = getRootForFrame(iFrame) ;
        string sRootSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sRootForFrame, &sRootSens) ;
        sRootSens += string(1, cheminSeparationMARK) + string("0OBJE") ;

        pGoal->createXmlTree(this, pTree, sRootSens, true /* remove title */) ;
      }
    }
	}

	// Filename = Archetype ID where '.' are replaced with "_"
  //
	string sFileName = sID ;

  size_t j ;
	while ((j = sFileName.find(".")) != NPOS)
		sFileName.replace( j, 1, "_") ;

  sFileName += string(".xml") ;

  // Write the protocol file in the SysImport directory
  //
  ofstream outFile ;
  string sFileToSave = pContexte->PathName("INEW") + sFileName ;
  outFile.open(sFileToSave.c_str(), ios::ate | ios::app) ;
  if (!outFile)
  {
		string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFileToSave ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return ldvseInterfaceError ;
	}

	referential.inscritFichier(&outFile, 0, LABEL_REFERENTIEL) ;

  outFile.close() ;

  // Reference all that's in the SysImport directory
  //
  // pContexte->getSuperviseur()->ReferenceTousArchetypes() ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("ReferenceArchetypes")) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

	return ldvseNoError ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::DrugCreateProtocolForSelectionService.", standardError, 0) ;
  return ldvseInterfaceError ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::GoalAddToProtocolForSelectionService(TWindow* pCallingView, VecteurString* pGoalsRefs)
{
try
{
	if ((NULL == pGoalsRefs) || (true == pGoalsRefs->empty()))
		return ldvseNothingToDo ;

  // Find if every selected items belong to the same frame
  //
  EquiItemIter itemsIter = pGoalsRefs->begin() ;
  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvGoal *pGoal = getGoal(**itemsIter, iFrame) ;
  itemsIter++ ;
  for ( ; pGoalsRefs->end() != itemsIter ; itemsIter++)
	{
    LDVFRAME iTempFrame = ldvframeNotSpecified ;
    pGoal = getGoal(**itemsIter, iTempFrame) ;
    if (pGoal && (iTempFrame != iFrame))
    {
      iFrame = ldvframeNotSpecified ;
      break ;
    }
  }
  if (ldvframeNotSpecified == iFrame)
    iFrame = ldvframeHealth ;

	// Selecting the protocol
  //
	ReferentialChoiceDialog *pProtocolSelect = new ReferentialChoiceDialog(pCallingView, pContexte, iFrame, string("0OBJE"), ReferentialChoiceDialog::sortNone) ;
  int iResult = pProtocolSelect->Execute() ;

  if (IDOK != iResult)
	{
  	delete (pProtocolSelect) ;
  	return ldvseCanceledByUser ;
	}

  string sRefID = pProtocolSelect->getSelectedRefId() ;

  delete (pProtocolSelect) ;

	if (string("") == sRefID)
  	return ldvseInvalidData ;

	// Opening the file and parsing it
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sRefFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::referentiel, sRefID) ;
  if (string("") == sRefFile)
    return ldvseInvalidData ;

  string ps = string("Parsing file ") + sRefFile ;
  pSuper->trace(&ps, 1) ;

  nsrefParseur Parseur(pContexte->getSuperviseur()) ;
  if (!(Parseur.open(sRefFile)))
    return ldvseInvalidData ;

  if ((NULL == Parseur.getReferentiel()) || (NULL == Parseur.getReferentiel()->getValArray()))
  {
  	string sErrorText = pSuper->getText("referentialManagement", "invalidReferential") + string(" ") + sRefID ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ldvseInvalidData ;
  }

  //
  // Find max Id for existing propositions
  //
  string sMaxPropId = string("") ;
  if (false == Parseur.getReferentiel()->vect_val.empty())
  {
    Creferentiel* pRef = Parseur.getReferentiel() ;
    for (ValIter ival = pRef->vect_val.begin() ; pRef->vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet dialogue
      if (LABEL_PROPOSITION == (*ival)->getLabel())
      {
        Cproposition *pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;
        if (pCprop)
        {
          string sProId = pCprop->getStringAttribute(ATTRIBUT_PROP_ID) ;
          if ((strlen(sProId.c_str()) > strlen(sMaxPropId.c_str())) ||
              ((strlen(sProId.c_str()) == strlen(sMaxPropId.c_str())) && (sProId > sMaxPropId))
              )
            sMaxPropId = sProId ;
        }
      }
    }
  }
  if (string("") == sMaxPropId)
    sMaxPropId = string("o0") ;

  VecteurString groupsLabelsArray ;
  Parseur.getGroups(&groupsLabelsArray, _sLangue) ;

  // Creating new Propositions
  //
  itemsIter = pGoalsRefs->begin() ;
	for ( ; pGoalsRefs->end() != itemsIter ; itemsIter++)
	{
    LDVFRAME iFrame = ldvframeNotSpecified ;

    NSLdvGoal* pGoal = getGoal(**itemsIter, iFrame) ;

    if (pGoal)
    {
      string sPropTitle = pGoal->_sComplementText ;
      if (string("") == sPropTitle)
        sPropTitle = pGoal->getTitle() ;

			string sPropGroup    = string("") ;
    	string sPropHelp     = string("") ;
      string sPropCheck    = string("") ;
      string sPropEvidence = string("") ;
      string sUncheckArche = string("") ;
			ReferentialPropParamsDialog *pParamsDlg = new ReferentialPropParamsDialog(pCallingView, pContexte, &sPropTitle, &sPropGroup, &sPropHelp, &(pGoal->getTitle()), &(pGoal->getTitle()), &sPropCheck, &sPropEvidence, &sUncheckArche, &groupsLabelsArray) ;
			int iParamsResult = pParamsDlg->Execute() ;
			delete pParamsDlg ;

      if (IDOK == iParamsResult)
      {
        if ('o' == sMaxPropId[0])
        {
          if (strlen(sMaxPropId.c_str()) == 1)
            sMaxPropId = string("o1") ;
          else
          {
            string sCounter = string(sMaxPropId, 1, strlen(sMaxPropId.c_str()) - 1) ;
            incremente_code(sCounter) ;
            sMaxPropId = string("o") + sCounter ;
          }
        }
        else
          incremente_code(sMaxPropId) ;

      	CValeur* pValProp = new CValeur(LABEL_PROPOSITION, string(""), string(""), Parseur.getReferentiel()) ;
				Parseur.getReferentiel()->vect_val.push_back(pValProp) ;
				pValProp->setObject(new Cproposition(string(""), string(""), Parseur.getReferentiel(), pContexte->getSuperviseur())) ;

        Cproposition* pProposition = dynamic_cast<Cproposition*>(pValProp->getObject()) ;

        pProposition->addAttribute(ATTRIBUT_PROP_ID,           sMaxPropId) ;
        pProposition->addAttribute(ATTRIBUT_PROP_NOM,          sPropTitle) ;

        if (string("") != sPropGroup)
          pProposition->addAttribute(ATTRIBUT_PROP_GROUPE,       sPropGroup) ;
        if (string("") != sPropHelp)
          pProposition->addAttribute(ATTRIBUT_PROP_HELP,         sPropHelp) ;
        if (string("") != sPropCheck)
          pProposition->addAttribute(ATTRIBUT_PROP_AUTOCHECK,    sPropCheck) ;
        if (string("") != sPropEvidence)
          pProposition->addAttribute(ATTRIBUT_PROP_EVIDENCE_LVL, sPropEvidence) ;
        if (string("") != sUncheckArche)
          pProposition->addAttribute(ATTRIBUT_PROP_UNCHECK_ARCH, sUncheckArche) ;

        CValeur* pValTree = new CValeur(LABEL_TREE, string(""), string(""), pProposition) ;
				pProposition->vect_val.push_back(pValTree) ;
				pValTree->setObject(new Ctree(string(""), string(""), pProposition, pContexte->getSuperviseur())) ;

        Ctree* pTree = dynamic_cast<Ctree*>(pValTree->getObject()) ;

        string sRootForFrame = getRootForFrame(iFrame) ;
        string sRootSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sRootForFrame, &sRootSens) ;
        sRootSens += string(1, cheminSeparationMARK) + string("0OBJE") ;

        pGoal->createXmlTree(this, pTree, sRootSens, true /* remove title */) ;
      }
    }
	}

  // File saving
  //
	ofstream outFile ;
  string sFileToSave = sRefFile ;
  outFile.open(sFileToSave.c_str(), ios::ate | ios::trunc) ;
  if (!outFile)
  {
		string sErrorText = pSuper->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFileToSave ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return ldvseInterfaceError ;
	}

	Parseur.getReferentiel()->inscritFichier(&outFile, 0, LABEL_REFERENTIEL) ;

  outFile.close() ;

	return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::DrugAddToProtocolForSelectionService : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::DrugAddToProtocolForSelectionService.", standardError, 0) ;
  return ldvseUndefined ;
}
}

NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::NewAppointmentService(TWindow* pCallingView)
{
try
{
/*
	if (!(pContexte->userHasPrivilege(NSContexte::createDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ldvseInsuficientRights ;
	}
*/

	string sAppointmentDate = pContexte->getPatient()->getConvoc() ;

	NVLdVTemps tAppointmentDate ;
  if (string("") != sAppointmentDate)
		tAppointmentDate.initFromDate(sAppointmentDate) ;
	else
  	tAppointmentDate.takeTime() ;

  NSConvocDlg *pAppointmentSetter = new NSConvocDlg(pCallingView, pContexte, &tAppointmentDate) ;
  int iResult = pAppointmentSetter->Execute() ;

  if (IDOK != iResult)
	{
  	delete (pAppointmentSetter) ;
  	return ldvseCanceledByUser ;
	}

  pContexte->getPatient()->setConvoc(tAppointmentDate.donneDateHeure()) ;

  delete (pAppointmentSetter) ;

  NSPersonsAttributesArray PatiensList ;
  NSBasicAttributeArray    AttrList ;

  AttrList.push_back(new NSBasicAttribute(CONSOLE,          pContexte->getSuperviseur()->getConsoleString())) ;
  AttrList.push_back(new NSBasicAttribute(INSTANCE,	        pContexte->getSuperviseur()->getInstanceString())) ;
  AttrList.push_back(new NSBasicAttribute(OPERATOR,         pContexte->getUtilisateurID())) ;
  AttrList.push_back(new NSBasicAttribute(PERSON,           pContexte->getPatient()->getNss())) ;
	AttrList.push_back(new NSBasicAttribute(APPOINTMENT_DATE, pContexte->getPatient()->getConvoc())) ;

  bool bRes = pContexte->getSuperviseur()->getPilot()->modifyTraitsForPersonOrObject(NautilusPilot::SERV_MODIFY_PERSON_TRAITS.c_str(), &PatiensList, &AttrList) ;
  if (false == bRes)
  	return ldvseInterfaceError ;

  return ldvseNoError ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::NewAppointmentService : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ldvseInterfaceError ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::NewAppointmentService.", standardError, 0) ;
  return ldvseUndefined ;
}
}

// Get all KSs of type USER_PROJECT_KS with provided "formulaire" code
//
NSLdvDocumentBase::LDVSERVICESERROR
NSLdvDocumentBase::GetProjectKsForLine(TWindow* pCallingView, std::vector<OB1NKS*> *kss, string sCode)
{
  if (((std::vector<OB1NKS*>*) NULL == kss) || (string("") == sCode))
    return ldvseInvalidData ;

  BB1BB* bbtemp = pContexte->getBlackboard() ;
  if ((BB1BB*) NULL == bbtemp)
    return ldvseUndefined ;

  std::vector<OB1NKS*>* pKsVector = bbtemp->getKSOfType(USER_PROJ_FU_KS) ;
  if ((std::vector<OB1NKS*>*) NULL == pKsVector)
    return ldvseNoError ;

  bbtemp->getValidKs(pKsVector, VALID) ;
  if (pKsVector->empty())
  {
    delete pKsVector ;
    return ldvseNoError ;
  }

  for (std::vector<OB1NKS*>::iterator iter = pKsVector->begin() ; pKsVector->end() != iter ; iter++)
	{
    string sFormulaire = (*iter)->getKS()->Value(string("formulaire"), string("")) ;
    if (sFormulaire == sCode)
      kss->push_back(*iter) ;
	}

  delete pKsVector ;

  return ldvseNoError ;
}

bool
NSLdvDocumentBase::TemplateCompo(string typeDoc, string& tempDoc, string& enteteDoc)
{
try
{
	string sIdGlobal = string("") ;
	string sIdUtil   = string("") ;

	string sVoid = string("9VOID1") ;
	bool   trouve = false ;

	// on appelle ici le pilote pour chercher la liste des template par défaut de type "typeDoc"
	// -----------------------------------------------------------------------------------------
	NSDataGraph Graph(pContexte->getSuperviseur(), graphObject) ;

	//Il faut creer la variable sTraitName = "_" + nomObjet + "_" + typeobjet
	string sTraitType = string("_0OTPL_0TYPE");
	string sTraitDefa = string("_0OTPL_0DEFA");
	// On transmet le code lexique pour le type
	string sTypeDoc = typeDoc ;
  NSDico::donneCodeComplet(sTypeDoc) ;

	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray AttrList ;
	AttrList.push_back(new NSBasicAttribute(sTraitType,  sTypeDoc)) ;
	AttrList.push_back(new NSBasicAttribute(sTraitDefa,  string("WCEA01"))) ;

	bool res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT.c_str(),
                                                &Graph, &ObjList, &AttrList);

	if ((false == res) || (ObjList.empty()))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "defaultTemplateNotFound");
    sErrorText += string(" (Doc= ") + sTypeDoc + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return false ;
	}

	for (NSPersonsAttributeIter k = ObjList.begin() ; ObjList.end() != k ; k++)
	{
		string sUtil = (*k)->getAttributeValue("_0OTPL_DOPER");
		string sCons = (*k)->getAttributeValue("_0OTPL_LNUCO");

		if ((sUtil == pContexte->getUtilisateurID()) ||
            (sCons == pContexte->getSuperviseur()->getConsole()))
		{
      sIdUtil = (*k)->getAttributeValue(OIDS) ;
      trouve = true ;
    }
    else if (sUtil == sVoid)
    {
      sIdGlobal = (*k)->getAttributeValue(OIDS) ;
      trouve = true ;
		}
	}

	string sObjectId ;

	if (trouve)
	{    // on prend la template trouvée par ordre de priorité
    if (string("") != sIdUtil)
      sObjectId = sIdUtil ;
    else
      sObjectId = sIdGlobal ;

    // Appel du pilote pour retrouver la template en fonction de l'ObjectId
    // --------------------------------------------------------------------
    Graph.graphReset() ; // au cas où le graphe était déjà rempli
    NSBasicAttributeArray AttrArray ;
    //pAttrArray->push_back(new NSBasicAttribute("graphID" , sObjectId));
    AttrArray.push_back(new NSBasicAttribute(OBJECT , sObjectId)) ;
    bool res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(),
                                    &Graph,  &AttrArray) ;
    if (!res)
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "templateNotFound") ;
      sErrorText += string(" (ObjectID= ") + sObjectId + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError, 0) ;
      return "" ;
    }

    NSPatPathoArray TplPatPathoArray(pContexte->getSuperviseur(), graphObject) ;
    NSDataTreeIter iterTree;
    PatPathoIter iter, iterEnd;
    int iColTpl;
    string sElemLex, sSens, sType, sTypeLex;
    string sFichierTpl = "", sEnTeteTpl = "";

    if (Graph.getTrees()->ExisteTree("0OTPL1", pContexte->getSuperviseur(), &iterTree))
    {
    	(*iterTree)->getRawPatPatho(&TplPatPathoArray) ;
      iter = TplPatPathoArray.begin() ;
    }
    else
			return false ;

		iterEnd = TplPatPathoArray.end() ;

    // Parsing de la template.......
    if ((NULL != iter) && (iter != iterEnd))
    {
    	sSens = (*iter)->getLexiqueSens() ;

      if (string("0OTPL") == sSens)
      {
      	iColTpl = (*iter)->getColonne() ;
        iter++ ;

        // on charge les données de l'archetype
        while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl))
        {
        	sSens = (*iter)->getLexiqueSens() ;

          // identifiant (unique)
          if (string("0ENTE") == sSens)
          {
          	iter++ ;
            sEnTeteTpl = "" ;

            while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
            {
            	// on cherche ici un texte libre
              sElemLex = (*iter)->getLexique() ;

              if (string("£?????") == sElemLex)
              	sEnTeteTpl = (*iter)->getTexteLibre() ;

              iter++ ;
            }
          }
          // nom du fichier (unique)
          else if (string("0TPL0") == sSens)
          {
          	iter++ ;
            sFichierTpl = "" ;

            while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
            {
            	// on cherche ici un texte libre
              sElemLex = (*iter)->getLexique() ;

              if (string("£?????") == sElemLex)
              	sFichierTpl = (*iter)->getTexteLibre() ;

              iter++ ;
            }
          }
          else
          	iter++ ;
        }

        if ((sEnTeteTpl != "") && (sFichierTpl != ""))
        {
        	tempDoc = sFichierTpl ;
          enteteDoc = sEnTeteTpl ;
        }
      }
      else
      {
      	erreur("Le noeud template est mal positionné dans ::TemplateCompo().", standardError, 0) ;
        return false ;
      }
    }
    else
    {
    	erreur("Le noeud template est incorrect dans ::TemplateCompo().", standardError, 0) ;
      return false ;
    }

		return true ;
	}

	char msg[255] ;
	sprintf(msg, "Pas de template de composition par defaut pour le type %s.", typeDoc.c_str()) ;
	MessageBox(0, msg, 0, MB_OK) ;
	return false ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::TemplateCompo.", standardError, 0) ;
	return false ;
}
}

void
NSLdvDocumentBase::addConcern(LDVFRAME iFrame, string sTitle, string sDateDeb, string sDateFin, string sHIssue, string sRef, string sPbOrigine)
{
try
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return ;

  ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;
  if (NULL == pConcerns)
    return ;

	NSConcern* pLdvPb = new NSConcern(pContexte, this, iFrame) ;

	pLdvPb->_sTitre       = sTitle ;

	pLdvPb->_tDateOuverture.initFromDate(sDateDeb) ;
	pLdvPb->_tDateFermeture.initFromDate(sDateFin) ;

	pLdvPb->_sHealthIssue = sHIssue ;
	pLdvPb->_sPrimoPb     = sPbOrigine ;
	pLdvPb->_sReference   = sRef ;

	pConcerns->push_back(pLdvPb) ;

	TView* pView = GetViewList() ;
  if (NULL == pView)
  	return ;

	do
	{
		NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
		if (pLdvView)
			pLdvView->addProb(pLdvPb) ;

		pView = NextView(pView) ;
	}
  while (pView) ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::addConcern.", standardError, 0) ;
}
}

void
NSLdvDocumentBase::addObjet(string sTitle, string sDateDeb, string sDateFin, string sPb, string sRef, string sTypDoc, string sTypeContenu)
{
try
{
	NSLdvObjet *pLdvObj = new NSLdvObjet(pContexte) ;

	pLdvObj->setTitre(sTitle) ;
	pLdvObj->setDateHeureDebut(sDateDeb) ;
	pLdvObj->setDateHeureFin(sDateFin) ;
	pLdvObj->setConcern(sPb) ;
	pLdvObj->setReference(sRef) ;
	pLdvObj->setTypeDoc(sTypDoc) ;
  pLdvObj->setLexique(sTypeContenu) ;

	_aObjets.push_back(pLdvObj) ;

	TView* pView = GetViewList();
  if (NULL == pView)
  	return ;

	do
	{
  	NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
		if (pLdvView)
			pLdvView->addObj(pLdvObj) ;

		pView = NextView(pView) ;
	}
	while (pView) ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::addObjet.", standardError, 0) ;
}
}

void
NSLdvDocumentBase::removeObjet(string sRef)
{
  NSLdvObjet* pObject = _aObjets.getObjet(sRef) ;

  if (NULL == pObject)
    return ;

  TView* pView = GetViewList();
  if (pView)
  {
	  do
	  {
  	  NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
		  if (pLdvView)
			  pLdvView->removeObj(pObject) ;

		  pView = NextView(pView) ;
	  }
	  while (pView) ;
  }

  _aObjets.removeObjet(sRef) ;
}

void
NSLdvDocumentBase::addSsObjet(string sTitle, string sDateDeb, string sDateFin, string sPb, string sRef, string sObjet)
{
try
{
	NSLdvSousObjet* pLdvSsObj = new NSLdvSousObjet(pContexte) ;

	pLdvSsObj->sTitre = sTitle ;

	pLdvSsObj->tDateHeureDebut.initFromDate(sDateDeb) ;
	pLdvSsObj->tDateHeureFin.initFromDate(sDateFin) ;

	pLdvSsObj->sConcern     = sPb ;
	pLdvSsObj->sReference   = sRef ;
	pLdvSsObj->sObject      = sRef ;

	_aSousObjets.push_back(pLdvSsObj) ;

	TView* pView = GetViewList() ;
  if (NULL == pView)
		return ;

	do
	{
		NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
		if (pLdvView)
			pLdvView->addSsObj(pLdvSsObj) ;

		pView = NextView(pView) ;
	}
  while (pView) ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::addSsObjet.", standardError, 0) ;
}
}

void
NSLdvDocumentBase::addGoal(LDVFRAME iFrame, string sTitle, string sDateDeb, string sDateFin, string sPb, string sRef)
{
try
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return ;

  ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
  if (NULL == pGoals)
    return ;

	NSLdvGoal *pGoal = new NSLdvGoal(pContexte, this, iFrame) ;

	pGoal->setTitle(sTitle) ;

	pGoal->_tDateOuverture.initFromDate(sDateDeb) ;
	pGoal->_tDateFermeture.initFromDate(sDateFin) ;

	pGoal->setConcern(sPb) ;
	pGoal->setNode(sRef) ;

	pGoals->push_back(pGoal) ;

	/*

	// ---------------------------------------------------------------------------
	// voir comment on va afficher réellement les différents objectifs
	// ---------------------------------------------------------------------------

	TView* pView = GetViewList();
	do
	{
		NSLdvView* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView);
		if (pLdvView)
			pLdvView->addGoal(pGoal);

		pView = NextView(pView);
	}
	while (pView);

	*/
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::addGoal.", standardError, 0) ;
}
}

void
NSLdvDocumentBase::addQualityOfLifeCharts(string sDate, NSPatPathoArray *pPatho)
{
  if (NULL == pPatho)
    return ;

  NSQualityOfLifeChart* pChart = createQualityOfLifeChart(sDate, pPatho) ;
  if (NULL == pChart)
    return ;
    
  _aQoLCharts.push_back(pChart) ;

  TView* pView = GetViewList() ;
  if (NULL == pView)
  	return ;

	do
	{
  	NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
		if (pLdvView)
			pLdvView->addQualityOfLifeChart(pChart) ;

		pView = NextView(pView) ;
	}
	while (pView) ;
}

void
NSLdvDocumentBase::modifyQualityOfLifeCharts(string sDate, NSPatPathoArray *pPatho)
{
  if (NULL == pPatho)
    return ;

  bool bMustErase = true ;

  NSQualityOfLifeChart* pChart = createQualityOfLifeChart(sDate, pPatho) ;
  if (NULL == pChart)
    return ;

  if (_aQoLCharts.empty())
  {
    _aQoLCharts.push_back(pChart) ;
    bMustErase = false ;
  }
  else
  {
    QualityOfLifeChartIter iter = _aQoLCharts.begin() ;
    for ( ; _aQoLCharts.end() != iter ; iter++)
      if ((*iter)->getDocId() == pChart->getDocId())
        break ;

    if (_aQoLCharts.end() == iter)
    {
      _aQoLCharts.push_back(pChart) ;
      bMustErase = false ;
    }
    else
      *(*iter) = *pChart ;
  }

  TView* pView = GetViewList() ;
  if (NULL == pView)
  	return ;

	do
	{
  	NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
		if (pLdvView)
    {
      if (bMustErase)
        pLdvView->removeQualityOfLifeChart(pChart) ;
			pLdvView->addQualityOfLifeChart(pChart) ;
    }

		pView = NextView(pView) ;
	}
	while (pView) ;
}

void
NSLdvDocumentBase::addQualityOfLifeCharts(string sConcern, string sDate, NSPatPathoArray *pPatho)
{
  if ((NSPatPathoArray*) NULL == pPatho)
    return ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSConcern* pConcern = getConcern(sConcern, iFrame) ;
  if (((NSConcern*) NULL == pConcern) || (NULL == pConcern->getQualityOfLifeCharts()))
    return ;

  NSQualityOfLifeChart* pChart = createQualityOfLifeChart(sDate, pPatho) ;
  if ((NSQualityOfLifeChart*) NULL == pChart)
    return ;

  pConcern->getQualityOfLifeCharts()->push_back(pChart) ;

  TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
  	return ;

	do
	{
  	NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
		if (pLdvView)
			pLdvView->addQualityOfLifeChart(pChart, pConcern) ;

		pView = NextView(pView) ;
	}
	while (pView) ;
}

void
NSLdvDocumentBase::modifyQualityOfLifeCharts(string sConcern, string sDate, NSPatPathoArray *pPatho)
{
  if ((NSPatPathoArray*) NULL == pPatho)
    return ;

  bool bMustErase = true ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSConcern* pConcern = getConcern(sConcern, iFrame) ;
  if (((NSConcern*) NULL == pConcern) || (NULL == pConcern->getQualityOfLifeCharts()))
    return ;

  NSQualityOfLifeChart* pChart = createQualityOfLifeChart(sDate, pPatho) ;
  if ((NSQualityOfLifeChart*) NULL == pChart)
    return ;

  NSQualityOfLifeChartArray *pQualityOfLifeCharts = pConcern->getQualityOfLifeCharts() ;

  if (pQualityOfLifeCharts->empty())
  {
    pQualityOfLifeCharts->push_back(pChart) ;
    bMustErase = false ;
  }
  else
  {
    QualityOfLifeChartIter iter = pQualityOfLifeCharts->begin() ;
    for ( ; pQualityOfLifeCharts->end() != iter ; iter++)
      if ((*iter)->getDocId() == pChart->getDocId())
        break ;

    if (pQualityOfLifeCharts->end() == iter)
    {
      pQualityOfLifeCharts->push_back(pChart) ;
      bMustErase = false ;
    }
    else
      *(*iter) = *pChart ;
  }

  TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
  	return ;

	do
	{
  	NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
		if (pLdvView)
    {
      if (bMustErase)
        pLdvView->removeQualityOfLifeChart(pChart, pConcern) ;
			pLdvView->addQualityOfLifeChart(pChart, pConcern) ;
    }

		pView = NextView(pView) ;
	}
	while (pView) ;
}

NSQualityOfLifeChart*
NSLdvDocumentBase::createQualityOfLifeChart(string sDate, NSPatPathoArray *pPatho)
{
  if (((NSPatPathoArray*) NULL == pPatho) || pPatho->empty())
    return (NSQualityOfLifeChart*) 0 ;

  NVLdVTemps tpsInfo ;
  tpsInfo.initFromDate(sDate) ;

  string sRootSens = (*(pPatho->begin()))->getLexiqueSens() ;

  if (string("ZCOOP") == sRootSens)
    return new NSCoopChart(pContexte, tpsInfo, pPatho) ;
  else
    return new NSQualityOfLifeChart(pContexte, tpsInfo, pPatho) ;
}

bool
NSLdvDocumentBase::getStorageDocumentsIds(VecteurString *pIdsVector, string sFrameIndexMeta, NSRootLink::NODELINKTYPES iRelation)
{
  if (((VecteurString*) NULL == pIdsVector) || (string("") == sFrameIndexMeta))
    return false ;

  NSPatientChoisi* pPatientChoisi = pContexte->getPatient() ;
	if ((NSPatientChoisi*) NULL == pPatientChoisi)
		return false ;

  NSPersonGraphManager* pGraphManager = pPatientChoisi->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

  NSLinkManager* pGraphe = pGraphManager->getLinkManager() ;
  if ((NSLinkManager*) NULL == pGraphe)
    return false ;

  pGraphe->TousLesVrais(sFrameIndexMeta, iRelation, pIdsVector) ;

  return true ;
}

NSDocumentHisto*
NSLdvDocumentBase::getMoreRecentDocument(VecteurString* pDocumentsIds)
{
  if (((VecteurString*) NULL == pDocumentsIds) || pDocumentsIds->empty())
    return (NSDocumentHisto*) 0 ;

  NSPatientChoisi* pPatientChoisi = pContexte->getPatient() ;
	if ((NSPatientChoisi*) NULL == pPatientChoisi)
    return (NSDocumentHisto*) 0 ;

  NSHISTODocument* pDocHis = pPatientChoisi->getDocHis() ;
  if ((NSHISTODocument*) NULL == pDocHis)
    return (NSDocumentHisto*) 0 ;

  NSDocumentHisto* pRecentDoc      = (NSDocumentHisto*) 0 ;
  string           sRecenteDocDate = string("") ;

  // Find most recent extension tree
  //
  for (EquiItemIter i = pDocumentsIds->begin(); pDocumentsIds->end() != i; i++)
  {
    DocumentIter docIter = pDocHis->TrouveDocHisto(**i) ;
    if (docIter)
    {
      string sDateDoc = (*docIter)->getDateDoc() ;
      if ((string("") == sRecenteDocDate) || (sDateDoc > sRecenteDocDate))
      {
        pRecentDoc      = *docIter ;
        sRecenteDocDate = sDateDoc ;
      }
    }
  }

  return pRecentDoc ;
}

int
NSLdvDocumentBase::getStorageDocumentCardinality(NSDocumentHisto* pDoc, string sChapter)
{
  if (NULL == pDoc)
    return -1 ;

  NSPatientChoisi* pPatientChoisi = pContexte->getPatient() ;
	// Can we work ?
	if (NULL == pPatientChoisi)
	{
		pContexte->getSuperviseur()->traceAndDisplay("patientManagement", "cannotFindPatient") ;
		return false ;
	}

  NSPersonGraphManager* pGraphManager = pPatientChoisi->getGraphPerson() ;
  if (NULL == pGraphManager)
    return -1 ;

  // Getting document's patpatho
  //
  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  if (false == pDoc->DonnePatPatho(&PPT, pGraphManager))
    return -1 ;

	// Looking for the chapter
  //
	PatPathoIter iter = PPT.ChercherItem(sChapter) ;
	if ((NULL == iter) || (PPT.end() == iter))
		return -1 ;

  int iColBase = (*iter)->getColonne() ;
  iter++ ;  int iNumElements = 0 ;  while ((PPT.end() != iter) && ((*iter)->getColonne() > iColBase))  {
    if ((*iter)->getColonne() == iColBase + 1)
      iNumElements++ ;

    iter++ ;
  }

  return iNumElements ;
}

void
NSLdvDocumentBase::addFromBasketList()
{
try
{
	SOAPBasketArray *pBasketArray = pContexte->getEpisodus()->pBasketList ;

	if (((SOAPBasketArray*) NULL == pBasketArray) || pBasketArray->empty())
  	return ;

	// On passe en revue chaque panier - Looking into each basket
	for (SOAPBasketArrayIter basketIter = pBasketArray->begin() ; basketIter != pBasketArray->end() ; basketIter++)
	{
		// On prend l'objet qui constitue le 'A' du panier
    // Taking the object which represents the 'A' position in the basket
		SOAPObject *pSOAPObj = (*basketIter)->donneDiagObject() ;

		// On regarde s'il appartient déjà aux sous-objets de la Ligne de vie
		// We check that it doesn't already belongs to Ligne de vie's sub-objects
		ArraySsObjIter ssObjIter = _aSousObjets.begin() ;
    for ( ; (_aSousObjets.end() != ssObjIter) && ((*ssObjIter)->sReference != pSOAPObj->sNoeud) ; ssObjIter++)
    	;

		if (_aSousObjets.end() == ssObjIter)
    {
			// Création d'un sous-objet de la ligne de vie
			// Creation of a new sub-object of the Ligne de vie
			NSLdvSousObjet  *pLdvSsObj = new NSLdvSousObjet(pContexte) ;

			pLdvSsObj->sTitre = pSOAPObj->donneLibelle() ;
			pLdvSsObj->tDateHeureDebut.takeTime() ;
			pLdvSsObj->tDateHeureFin.takeTime() ;
			pLdvSsObj->sReference = pSOAPObj->sNoeud ;
			pLdvSsObj->sObject  = string("") ;
			pLdvSsObj->sConcern = string("") ;

			_aSousObjets.push_back(pLdvSsObj) ;

			// On répercute la création à chaque vue
			// We tell each view there is a new sub-object
			TView* pView = GetViewList() ;
      if (pView)
      {
				do
				{
					NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
					if (pLdvView)
						pLdvView->addSsObj(pLdvSsObj) ;

					pView = NextView(pView) ;
				}
				while (pView) ;
      }
		}
	}
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::addFromBasketList.", standardError, 0) ;
}
}

bool
NSLdvDocumentBase::openLdVObject(NSLdvObjet *pObj)
{
	if ((NSLdvObjet*) NULL == pObj)
		return false ;

	string sObjRef = pObj->getReference() ;

	NSPatientChoisi* pPat = pContexte->getPatient() ;
	if ((NSPatientChoisi*) NULL == pPat)
		return false ;

	// Document his
	NSHISTODocument* pHistory = pPat->getDocHis() ;
	if ((NSHISTODocument*) NULL == pHistory)
  	return false ;

  NSDocHistoArray* pVectDocument = pHistory->getVectDocument() ;
	if (((NSDocHistoArray*) NULL == pVectDocument) || pVectDocument->empty())
  	return false ;

	DocumentIter iterDoc = pVectDocument->begin() ;
	for ( ; pVectDocument->end() != iterDoc ; iterDoc++)
		if (sObjRef == (*iterDoc)->getID())
    {
    	// pHistory->AutoriserOuverture((NSDocumentInfo *)(*iterDoc)) ;

      NSToDoTask* pTask = new NSToDoTask ;
      pTask->setWhatToDo(string("OpenDocument")) ;
      pTask->setParam1(sObjRef) ;
      pTask->sendMe(pContexte->getSuperviseur()) ;
    }

	// La Synthèse et l'Index POMR ne sont pas affichés
	// Synthesis and POMR index are not displayed
	return true ;
}

bool
NSLdvDocumentBase::openLdVObjectsInArray(ArrayObjets* pObjects, NSLdvViewBase* pView)
{
try
{
	if (((ArrayObjets*) NULL == pObjects) || pObjects->empty())
		return false ;

	// S'il n'y en a qu'un, on l'ouvre directement
	//
	ArrayObjIter objIt = pObjects->begin() ;
	NSLdvObjet* pFirstObj = *objIt ;
	objIt++ ;
	if (pObjects->end() == objIt)
  	return openLdVObject(pFirstObj) ;

	NSOpenMultiDocsDlg* pOpenDlg = new NSOpenMultiDocsDlg(pView, pContexte, pObjects) ;
	int iResult = pOpenDlg->Execute() ;
	delete pOpenDlg ;

	if ((IDOK == iResult) && (false == pObjects->empty()))
	{
  	objIt = pObjects->begin() ;
    for ( ; pObjects->end() != objIt ; objIt++)
    	openLdVObject(*objIt) ;
	}
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::openLdVObjectsInArray.", standardError, 0) ;
	return false;
}
}

bool
NSLdvDocumentBase::evolConcern(NSConcern *pPere, string sLexique, string sDateDeb, string sDateFin)
{
  return true ;
}

bool
NSLdvDocumentBase::changeConcernDateDeb(NSConcern* pConcern, NVLdVTemps* pDateDeb)
{
  if ((NULL == pConcern) || (NULL == pDateDeb))
    return false ;

	if (*pDateDeb == pConcern->_tDateOuverture)
		return true ;

	NVLdVTemps tPreviousDateDeb = pConcern->_tDateOuverture ;

	// Recherche de cette préoccupation dans l'index de santé
	// Looking for this concern in the health index document
  //
	NSPatPathoArray* pPatho = (NSPatPathoArray*) 0 ;
	PatPathoIter iter = donnePreoccup(pConcern, &pPatho) ;

	// not found !
	if ((NULL == iter) || (NULL == pPatho) || (pPatho->empty()) || (pPatho->end() == iter))
		return false ;

	int iColonne = (*iter)->getColonne() ;

	PatPathoIter iterBis = pPatho->begin() ;
	iterBis = iter ;

	iterBis++;
	while ((pPatho->end() != iterBis) && ((*iterBis)->getColonne() > iColonne) &&
					(((*iterBis)->getColonne() != iColonne+1) ||
						((*iterBis)->getLexiqueSens() != string("KOUVR"))))
		iterBis++ ;

	if ((pPatho->end() == iterBis) || ((*iterBis)->getColonne() != iColonne+1))
		return false ;

	iterBis++ ;
  if (pPatho->end() != iterBis)
	  (*iterBis)->setComplement(pDateDeb->donneDateHeure()) ;
  else
  {
    iterBis-- ;

    Message Msg ;
    Msg.SetUnit("2DA021") ;
    Msg.SetComplement(pDateDeb->donneDateHeure()) ;

    NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
    PatPatho.ajoutePatho("£T0;19", &Msg, 1) ;

    pPatho->InserePatPatho(iterBis, &PatPatho, iColonne+1) ;
  }

	pConcern->_tDateOuverture = *pDateDeb ;

	// S'il y a un élément de quantification qui démarre à date_deb, il faut
  // le modifier également
  //
	iterBis = iter ;

	iterBis++ ;
	while ((pPatho->end() != iterBis) && ((*iterBis)->getColonne() > iColonne) &&
					(((*iterBis)->getColonne() != iColonne+1) ||
						((*iterBis)->getLexiqueSens() != string("VQUAN"))))
		iterBis++ ;

	if ((pPatho->end() == iterBis) || ((*iterBis)->getColonne() != iColonne+1))
  	return true ;

	// On passe en revue tous les éléments de quantification
	iterBis++ ;
	while ((pPatho->end() != iterBis) &&
					((*iterBis)->getColonne() > iColonne+1))
	{
		if (((*iterBis)->getColonne() == iColonne+2) &&
				((*iterBis)->getLexiqueSens() == string("KDAT0")))
		{
			iterBis++ ;
      if (pPatho->end() != iterBis)
      {
        if ((*iterBis)->getComplement() == tPreviousDateDeb.donneDateHeure())
        {
          (*iterBis)->setComplement(pDateDeb->donneDateHeure()) ;

          // Mise à jour du modificateur
          // Recherche de cette préoccupation dans l'Array
          // Looking for this health upset in the Array
          if (false == pConcern->_aModificateurs.empty())
          {
            ArrayModifierIter iModif = pConcern->_aModificateurs.begin() ;
            for ( ; pConcern->_aModificateurs.end() != iModif ; iModif++)
              if ((*iModif)->tDateHeureDeb == tPreviousDateDeb)
                (*iModif)->tDateHeureDeb = *pDateDeb ;
          }
        }
      }
		}
    if (pPatho->end() != iterBis)
		  iterBis++ ;
	}

  return true ;
}

bool
NSLdvDocumentBase::changeConcernDateFin(NSConcern* pConcern, NVLdVTemps* pDateFin)
{
try
{
	if (*pDateFin == pConcern->_tDateFermeture)
		return true ;

	NVLdVTemps tPreviousDateFin = pConcern->_tDateFermeture ;

	// Recherche de cette préoccupation dans l'index de santé
	// Looking for this concern in the health index document
	NSPatPathoArray* pPatho;
	PatPathoIter iter = donnePreoccup(pConcern, &pPatho) ;

    // not found !
	if ((NULL == iter) || (NULL == pPatho) || (pPatho->empty()) || (pPatho->end() == iter))
  	return false ;

	int iColonne = (*iter)->getColonne() ;

	// Recherche de la date de fin pour la modifier
	// Looking for ending date - in order to modify it
	PatPathoIter iterBis = pPatho->begin() ;
	iterBis = iter ;

	iterBis++ ;
	while ((pPatho->end() != iterBis) && ((*iterBis)->getColonne() > iColonne) &&
					(((*iterBis)->getColonne() != iColonne+1) ||
						((*iterBis)->getLexiqueSens() != string("KFERM"))))
  	iterBis++ ;

	// On a trouvé une précédente date de fin, on la modifie modifier
	// We have found a previous ending date, we can change it
	if ((pPatho->end() != iterBis) &&
			((*iterBis)->getColonne() == iColonne+1) &&
			((*iterBis)->getLexiqueSens() == string("KFERM")))
	{
		iterBis++ ;
    if (pPatho->end() != iterBis)
		  (*iterBis)->setComplement(pDateFin->donneDateHeure()) ;
    else
    {
      iterBis-- ;

      Message Msg ;
      Msg.SetUnit("2DA021") ;
      Msg.SetComplement(pDateFin->donneDateHeure()) ;

      NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
      PatPatho.ajoutePatho("£T0;19", &Msg, 1) ;

      pPatho->InserePatPatho(iterBis, &PatPatho, iColonne+1) ;
    }
	}

	// Si on n'a pas trouvé de date de fin à modifier, on en crée une
	// If we didn't find any ending date, we must create it
	else
	{
  	iterBis = iter ;

		// On recherche la date de début, pour s'insérer juste après
		// Looking for starting date - to insert ending date just after
		iterBis++;
		while ((pPatho->end() != iterBis) && ((*iterBis)->getColonne() > iColonne) &&
						(((*iterBis)->getColonne() != iColonne+1) ||
						 ((*iterBis)->getLexique() != "KOUVR1")))
			iterBis++ ;

    // Si on est à la fin ou qu'on est tombé sur un père sans passer par une date d'ouverture, on sort
		if ((pPatho->end() == iterBis) || ((*iterBis)->getColonne() != iColonne+1))
    	return false ;

    iterBis++ ;
    while ((pPatho->end() != iterBis) && ((*iterBis)->getColonne() > iColonne+1))
    	iterBis++ ;

		NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

    PatPatho.ajoutePatho("KFERM1", 0) ;
    Message Msg ;
    Msg.SetUnit("2DA021") ;
    Msg.SetComplement(pDateFin->donneDateHeure()) ;
    PatPatho.ajoutePatho("£T0;19", &Msg, 1) ;

    pPatho->InserePatPatho(iterBis, &PatPatho, iColonne+1) ;
	}

	pConcern->_tDateFermeture = *pDateFin ;

  return true ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::newConcern.", standardError, 0) ;
  return false ;
}
}

bool
NSLdvDocumentBase::changeConcernModifier(NSConcern* pConcern, NSConcernModifier* pModifier, int iSev, int iRisk)
{
try
{
	if (NULL == pModifier)
		return false ;

	NVLdVTemps tDateHeureModif = pModifier->tDateHeureDeb ;

	// Recherche de cette préoccupation dans l'index de santé
	// Looking for this concern in the health index document
	NSPatPathoArray* pPatho ;
	PatPathoIter iter = donnePreoccup(pConcern, &pPatho) ;

	// not found !
	if ((NULL == iter) || (NULL == pPatho) || (pPatho->empty()) || (pPatho->end() == iter))
		return false ;

	int iColonne = (*iter)->getColonne() ;

	PatPathoIter iterBis = pPatho->begin() ;
  iterBis = iter ;

  iterBis++ ;

	// Positionnement sur le sous arbre des quantifications
	while ((pPatho->end() != iterBis) && ((*iterBis)->getColonne() > iColonne) &&
					(((*iterBis)->getColonne() != iColonne+1) ||
						((*iterBis)->getLexiqueSens() != string("VQUAN"))))
		iterBis++ ;

	if ((pPatho->end() == iterBis) || ((*iterBis)->getColonne() != iColonne+1))
		return true ;

	bool bMisRisk = false;
	bool bMisSeve = false;

	// On passe en revue tous les éléments de quantification
	iterBis++;
	while ((pPatho->end() != iterBis) &&
					((*iterBis)->getColonne() > iColonne+1))
	{
		if (((*iterBis)->getColonne() == iColonne+2) &&
    		((*iterBis)->getLexiqueSens() == string("KDAT0")))
		{
			iterBis++ ;
      if (pPatho->end() != iterBis)
      {
        if ((*iterBis)->getComplement() == tDateHeureModif.donneDateHeure())
        {
          char szGravite[4] ;
          char szRisk[4] ;
          if (iSev > 0)
            numacar(szGravite, iSev, 3) ;
          if (iRisk > 0)
            numacar(szRisk, iRisk, 3) ;

          while ((pPatho->end() != iterBis) &&
                  ((*iterBis)->getColonne() > iColonne+1))
          {
            if (((*iterBis)->getColonne() == iColonne+2) &&
                ((*iterBis)->getLexiqueSens() == string("VIGRA")))
            {
              if (iSev > 0)
              {
                iterBis++ ;
                if (pPatho->end() != iterBis)
                  (*iterBis)->setComplement(string(szGravite)) ;
                bMisSeve = true ;
              }
              else
                if (iSev == 0)
                {
                  pPatho->SupprimerItem(iterBis) ;
                  pPatho->SupprimerItem(iterBis) ;
                }
            }
            else
            {
              if (((*iterBis)->getColonne() == iColonne+2) &&
                  ((*iterBis)->getLexiqueSens() == string("VIRIS")))
              {
                if (iRisk > 0)
                {
                  iterBis++ ;
                  if (pPatho->end() != iterBis)
                    (*iterBis)->setComplement(string(szRisk)) ;
                  bMisRisk = true ;
                }
                else
                  if (iRisk == 0)
                  {
                    pPatho->SupprimerItem(iterBis) ;
                    pPatho->SupprimerItem(iterBis) ;
                  }
              }
            }
            iterBis++ ;
          }
          if ((false == bMisSeve) && (iSev > 0))
          {
            NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

            PatPatho.ajoutePatho("VIGRA1", 2) ;

            char szGravite[4] ;
            numacar(szGravite, iSev, 3) ;
            Message Msg ;
            Msg.SetUnit("200001") ;
            Msg.SetComplement(string(szGravite)) ;
            PatPatho.ajoutePatho("£N2;04", &Msg, 3) ;

            pPatho->InserePatPatho(iterBis, &PatPatho, iColonne+1) ;
          }
          if ((false == bMisRisk) && (iRisk > 0))
          {
            NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

            PatPatho.ajoutePatho("VIRIS1", 2) ;

            char szRisque[4] ;
            numacar(szRisque, iRisk, 3) ;
            Message Msg ;
            Msg.SetUnit("200001") ;
            Msg.SetComplement(string(szRisque)) ;
            PatPatho.ajoutePatho("£N2;04", &Msg, 3) ;

            pPatho->InserePatPatho(iterBis, &PatPatho, iColonne+1) ;
          }
        }
      }
		}
		if (iterBis != pPatho->end())
			iterBis++ ;
	}

	if (-1 != iSev)
		pModifier->iSeverite = iSev ;
	if (-1 != iRisk)
  	pModifier->iRisque = iRisk ;

	return true;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::changeConcernModifier.", standardError, 0) ;
	return false;
}
}

bool
NSLdvDocumentBase::changeConcernType(LDVFRAME iFrame, NSConcern* pConcern, NSPatPathoInfo* pNode)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
    return false ;

  // Recherche de cette préoccupation dans l'index de santé
	// Looking for this concern in the health index document
	NSPatPathoArray* pPatho ;
	PatPathoIter iter = donnePreoccup(pConcern, &pPatho) ;
  // not found !
	if ((NULL == iter) || (NULL == pPatho) || (pPatho->empty()) || (pPatho->end() == iter))
		return false ;

  // We need to remember root Id to get the proper patpatho later
  //
  PatPathoIter rootIter = pPatho->begin() ;
  string sRootNodeId = (*rootIter)->getNode() ;

  string sConcernNodeId = pConcern->getNoeud() ;

  // Since node ID will change, the only way to bookmark it is by line and col.
  //
  int iColonne = (*iter)->getColonne() ;
	int iLigne   = (*iter)->getLigne() ;
	string sPrevNode = (*iter)->getNode() ;

	(*iter)->setLexique(pNode->getLexique()) ;
	(*iter)->setTexteLibre(pNode->getTexteLibre()) ;

  // Plug all previous links to the new node
  //
  // Now automatically done in NSDataGraph::updateNodesTL
  //
  // NSLinkManager* pGraphe = pContexte->getPatient()->pGraphPerson->pLinkManager ;
	// pGraphe->swapLiens(sPrevNode, *iter) ;

  NSDocumentHisto* pDocument = pFrameInfo->findNode(sPrevNode) ;
  if ((NSDocumentHisto*) NULL == pDocument)
    return false ;

  updateIndexDocument(pDocument, false) ;

  // Find back the new node to update the concern
  //
  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  pDocument->initFromPatPatho(&PPT) ;

  if (PPT.empty())
    return false ;

	iter = PPT.begin() ;
  while ((PPT.end() != iter) &&
					(((*iter)->getColonne() != iColonne) ||
					 ((*iter)->getLigne()   != iLigne)))
    iter++ ;

	// not found !
	if (PPT.end() == iter)
		return false ;

  string sNewNode = (*iter)->getNode() ;

  pConcern->_sReference = sNewNode ;

  pConcern->initTitle(*iter) ;

	return true ;

/*
	// Recherche de cette préoccupation dans l'index de santé
	// Looking for this concern in the health index document
	NSPatPathoArray* pPatho ;
	PatPathoIter iter = donnePreoccup(pConcern, &pPatho) ;

  // not found !
	if ((iter == NULL) || (iter == pPatho->end()))
		return false;

	int iColonne        = (*iter)->getColonne() ;
	int iLigne          = (*iter)->getLigne() ;
	string sPrevNoeud   = (*iter)->getNode() ;

	(*iter)->setLexique(pNode->getLexique()) ;
	(*iter)->setTexteLibre(pNode->getTexteLibre()) ;

	updateIndex() ;

	// On doit retrouver le nouveau noeud pour réorienter vers lui les liens
	if (!pPathoPOMRIndex)
		return false;

	iter = pPathoPOMRIndex->begin();
	while (	(iter != pPathoPOMRIndex->end()) &&
					(((*iter)->getColonne() != iColonne) ||
					((*iter)->getLigne() != iLigne)))
		iter++;

	// not found !
	if (iter == pPathoPOMRIndex->end())
		return false;

	string sNewNoeud = (*iter)->getNode() ;
	NSLinkManager* pGraphe = pContexte->getPatient()->pGraphPerson->pLinkManager ;

	pGraphe->swapLiens(sPrevNoeud, sNewNoeud);

	pConcern->sReference = sNewNoeud;

	string sCodeLex = (*iter)->getLexique() ;

	if (sCodeLex != string("£?????"))
		pContexte->getDico()->donneLibelle(sLangue, &sCodeLex, &(pConcern->sTitre));

	// Texte libre - Free text
	else
  	pConcern->sTitre = (*iter)->getTexteLibre() ;

	return true ;
*/
}

void
NSLdvDocumentBase::showConcernProperty(NSConcern* pConcern)
{
	if (NULL == pContexte->getPatient())
    return ;

  string sNode = string("") ;
  if (pConcern)
    sNode = pConcern->getNoeud() ;

  // NSSuper *pSuper = pContexte->getSuperviseur() ;

/*
  if ((NULL == pContexte->getPredi()) || (false == pContexte->getPredi()->_bMinimalInterface))
    pContexte->getPatient()->drugs_show(sNode) ;
*/

  // pContexte->getPatient()->goals_show(sNode) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("OpenNewWindow")) ;
  pTask->setParam1(string("GoalManagement")) ;
  pTask->setParam2(sNode) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  // pContexte->getPatient()->followUp_show(sNode) ;
  //
  NSToDoTask* pTask2 = new NSToDoTask ;
  pTask2->setWhatToDo(string("OpenNewWindow")) ;
  pTask2->setParam1(string("FollowUpShow")) ;
  pTask2->setParam2(sNode) ;
  pTask2->sendMe(pContexte->getSuperviseur()) ;

  // if ((!(pSuper->getDPIO())) || (!(pSuper->getDPIO()->bMinimalInterface)))
  //	pContexte->getPatient()->process_show(sNode) ;

//    NSConcernPropertyDlg* pCPDialog;
//    pCPDialog = new NSConcernPropertyDlg(pLdvView, pContexte, pConcern);
//    pCPDialog->Create();
}

bool
NSLdvDocumentBase::evolveConcernModifier(LDVFRAME iFrame, NSConcern* pConcern, NVLdVTemps* pDateDeb, int iSev, int iRisk)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return false ;

  if (NULL == pConcern)
    return false ;

	// Recherche de cette préoccupation dans l'index de santé
	// Looking for this concern in the health index document
	NSPatPathoArray* pPatho ;
	PatPathoIter iter = donnePreoccup(pConcern, &pPatho) ;

	// not found !
	if ((NULL == iter) || (NULL == pPatho) || (pPatho->empty()) || (pPatho->end() == iter))
		return false;

	int iColonne = (*iter)->getColonne() ;

	PatPathoIter iterBis = iter ;

	// On passe en revue tous les éléments de quantification
	iterBis++ ;
	while ((pPatho->end() != iterBis) &&
					((*iterBis)->getColonne() > iColonne))
	{
		if (((*iterBis)->getColonne() == iColonne+1) &&
    		((*iterBis)->getLexiqueSens() == string("VQUAN")))
		{
			iterBis++ ;
			while ((pPatho->end() != iterBis) &&
							((*iterBis)->getColonne() > iColonne+1))
			{
				// Sous arbre date
				if (((*iterBis)->getColonne() == iColonne+1) &&
						((*iterBis)->getLexiqueSens() == string("KDAT0")))
				{
					iterBis++ ;
					if ((pPatho->end() != iterBis) && ((*iterBis)->getComplement() == pDateDeb->donneDateHeure()))
					{
						erreur("Il existe déjà une modification de sévérité à cette date - utilisez la correction si vous souhaitez la modifier.", standardError, 0) ;
						return false ;
					}
				}
        if (pPatho->end() != iterBis)
				  iterBis++ ;
			}
		}
		if ((pPatho->end() != iterBis) &&
				((*iterBis)->getColonne() > iColonne))
			iterBis++ ;
	}

	// Création de l'arbre de quantification
	NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

	PatPatho.ajoutePatho("VQUAN1", 0) ;

	PatPatho.ajoutePatho("KDAT01", 1) ;
	Message Msg ;
  Msg.SetUnit("2DA021") ;
	Msg.SetComplement(pDateDeb->donneDateHeure()) ;
	PatPatho.ajoutePatho("£T0;19", &Msg, 2) ;

	// Index de gravité
	if (iSev > 0)
	{
		PatPatho.ajoutePatho("VIGRA1", 1) ;

		char szGravite[4] ;
    numacar(szGravite, iSev, 3) ;
		Message Mes ;
    Mes.SetUnit("200001") ;
		Mes.SetComplement(string(szGravite)) ;
		PatPatho.ajoutePatho("£N2;04", &Mes, 2) ;
	}

	// Index de risque
	if (iRisk > 0)
	{
		PatPatho.ajoutePatho("VIRIS1", 1) ;

		char szRisque[4] ;
		numacar(szRisque, iRisk, 3) ;
		Message Mes ;
    Mes.SetUnit("200001") ;
		Mes.SetComplement(string(szRisque)) ;
		PatPatho.ajoutePatho("£N2;04", &Mes, 2) ;
	}

	// On cherche où il faut insérer la branche
	// Looking for the place to insert the branch
	int iRefLine;

	if (pPatho->end() != iterBis)
	{
		iRefLine = (*iterBis)->getLigne() - 1 ;
		pPatho->InserePatPatho(iterBis, &PatPatho, iColonne + 1) ;
	}
	else
	{
		iRefLine = pPatho->back()->getLigne() ;
		pPatho->InserePatPatho(iterBis, &PatPatho, iColonne + 1) ;
	}

  NSDocumentHisto* pDocument = pFrameInfo->findNode(pConcern->getNoeud()) ;
  if (NULL == pDocument)
    return false ;

	// Enregistrement du document modifié
	updateIndexDocument(pDocument, false) ;

	iter = donnePreoccup(pConcern, &pPatho) ;

  // not found !
	if ((NULL == iter) || (pPatho->end() == iter))
		return false ;

	string sConcernNode = (*iter)->getNode() ;

	// On passe en revue tous les éléments de quantification
	while ((pPatho->end() != iter) && ((*iter)->getLigne() <= iRefLine))
		iter++ ;
	if (pPatho->end() == iter)
		return false ;

	if ((*iter)->getLexiqueSens() != string("VQUAN"))
  	return false ;

	string sItemNode = (*iter)->getNode() ;
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

	pGraphe->etablirLien(sItemNode, NSRootLink::problemContactElement, sConcernNode) ;

	// Création d'un ConcernModifier
	// Creation of a ConcernModifier
	NVLdVTemps tDateTimeFin ;
	tDateTimeFin.estNoLimit() ;

	NSConcernModifier Modifier(pConcern, *pDateDeb, tDateTimeFin, iSev, 0, iRisk) ;
	Modifier.setNode(sItemNode) ;
	pConcern->_aModificateurs.addModifier(&Modifier) ;

	return true ;
}

bool
NSLdvDocumentBase::creeConcernSuite(LDVFRAME iFrame, NSConcern* pConcernPere, NSConcern* pConcernFils)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return false ;

	// Pédigré du père
	NSPatPathoArray* pPatho ;
	PatPathoIter iterPere = donnePreoccup(pConcernPere, &pPatho) ;

  if ((NULL == iterPere) || (NULL == pPatho) || (pPatho->empty()) || (pPatho->end() == iterPere))
		return false ;

	string sNoeudPere = (*iterPere)->getNode() ;

	// Pédigré du fils
	NSPatPathoArray* pPathoBis ;
	PatPathoIter iterFils = donnePreoccup(pConcernFils, &pPathoBis) ;

  if ((NULL == iterFils) || (NULL == pPathoBis) || (pPathoBis->empty()) || (pPathoBis->end() == iterFils))
		return false ;

	string sNoeudFils = (*iterFils)->getNode() ;
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

	// On donne au père une date de fin qui correspond à la date de début du fils
	changeConcernDateFin(pConcernPere, &(pConcernFils->_tDateOuverture)) ;

  NSDocumentHisto* pDocument = pFrameInfo->findNode(sNoeudFils) ;
  if (NULL == pDocument)
    return false ;

	// Enregistrement du document modifié
	updateIndexDocument(pDocument, false) ;

	// Création du lien
	if (false == pGraphe->etablirLien(sNoeudFils, NSRootLink::problemRelatedTo, sNoeudPere))
  	return false ;

	pConcernFils->_sPrimoPb = sNoeudPere ;

  ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;
  if (NULL == pConcerns)
    return false ;

  pConcerns->reinit() ;

	return true ;
}


bool
NSLdvDocumentBase::chgConcernType(string sLexique)
{
  return true ;
}


bool
NSLdvDocumentBase::chgConcernDate(string sDateDeb, string sDateFin)
{
  return true ;
}

bool
NSLdvDocumentBase::deleteConcern(LDVFRAME iFrame, NSConcern* pConcern)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return false ;

	// Recherche de cette préoccupation dans l'index de santé
	// Looking for this concern in the health index document
	NSPatPathoArray* pPatho ;
	PatPathoIter iter = donnePreoccup(pConcern, &pPatho) ;

	// not found !
  if ((NULL == iter) || (NULL == pPatho) || (pPatho->empty()) || (pPatho->end() == iter))
		return false ;

	string sNoeud = (*iter)->getNode() ;

  NSDocumentHisto* pDocument = pFrameInfo->findNode(sNoeud) ;
  if (NULL == pDocument)
    return false ;

	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

	// Suppression du noeud
	// Killing the node
	pPatho->SupprimerItem(iter) ;

	// Suppression des liens vers le noeud
	// Killing all links concernig the node
	if (false == pGraphe->detruireTousLesLiens(sNoeud))
		return false ;

	// Enregistrement du document modifié
	updateIndexDocument(pDocument, false) ;

  ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;
  if (NULL == pConcerns)
    return false ;
	pConcerns->deleteConcern(pConcern) ;

	return true ;
}

void
NSLdvDocumentBase::manageRights(LDVFRAME iFrame, TWindow* parent, string sNode)
{
try
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return ;

  NSDocumentHisto* pDocument = pFrameInfo->findNode(sNode) ;
  if (NULL == pDocument)
    return ;

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  pDocument->initFromPatPatho(&PPT) ;

  if (PPT.empty())
    return ;

	PatPathoIter iterNode = PPT.ChercherNoeud(sNode) ;

	string sNodeRights = (*iterNode)->getNodeRight() ;

	RightsDialog* pRightsDlg = new RightsDialog(parent, pContexte, &sNodeRights) ;
	int iReturn = pRightsDlg->Execute() ;

	if (IDOK == iReturn)
  {
  	(*iterNode)->setNodeRight(sNodeRights) ;
    updateIndexDocument(pDocument) ;
	}

	delete pRightsDlg ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::manageRights : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  erreur("Exception NSLdvDocumentBase::manageRights.", standardError, 0) ;
  return ;
}
}

PatPathoIter
NSLdvDocumentBase::getElement(LDVFRAME iFrame, string sNodeId, NSPatPathoArray **ppPatho)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return NULL ;

  NSDocumentHisto* pDocument = pFrameInfo->findNode(sNodeId) ;
  if (NULL == pDocument)
    return NULL ;

  NSPatPathoArray *pPpt = pDocument->getPatPathoPointer() ;

	if ((NULL == pPpt) || (pPpt->empty()))
	{
		*ppPatho = (NSPatPathoArray*) 0 ;
		return NULL ;
	}
	*ppPatho = pPpt ;

  if (string("") == sNodeId)
		return NULL ;

	PatPathoIter iter = pPpt->ChercherNoeud(sNodeId) ;

	// not found !
	if ((pPpt->end() == iter) || (NULL == iter))
		return NULL ;

	return iter ;
}

PatPathoIter
NSLdvDocumentBase::donneElement(string sNodeId, NSPatPathoArray **ppPatho)
{
  return getElement(ldvframeHealth, sNodeId, ppPatho) ;
}

PatPathoIter
NSLdvDocumentBase::getRiskElement(string sNodeId, NSPatPathoArray **ppPatho)
{
  return getElement(ldvframeRisk, sNodeId, ppPatho) ;
}

PatPathoIter
NSLdvDocumentBase::getSocialElement(string sNodeId, NSPatPathoArray **ppPatho)
{
  return getElement(ldvframeSocial, sNodeId, ppPatho) ;
}

PatPathoIter
NSLdvDocumentBase::donnePreoccup(NSConcern* pConcern, NSPatPathoArray** pPatho)
{
  if ((NSConcern*) NULL == pConcern)
    return NULL ;

  string sNodeId = pConcern->getNoeud() ;

	return getElement(pConcern->getIFrame(), sNodeId, pPatho) ;
}

PatPathoIter
NSLdvDocumentBase::donneGoal(NSLdvGoal* pGoal, NSPatPathoArray** pPatho)
{
  if ((NSLdvGoal*) NULL == pGoal)
    return NULL ;

  string sNodeId = pGoal->getNode() ;

	return getElement(pGoal->getIFrame(), sNodeId, pPatho) ;
}

PatPathoIter
NSLdvDocumentBase::donneDrug(NSLdvDrug *pDrug, NSPatPathoArray **pPatho)
{
  if ((NSLdvDrug*) NULL == pDrug)
    return NULL ;

  string sNodeId = pDrug->getNoeud() ;

	return getElement(pDrug->getIFrame(), sNodeId, pPatho) ;
}

NSConcern*
NSLdvDocumentBase::donneOpenConcern(LDVFRAME iFrame, string sCodeSens)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
		return (NSConcern*) 0 ;

  ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;
	if (((ArrayConcern*) NULL == pConcerns) || (pConcerns->empty()))
		return (NSConcern*) 0 ;

	NVLdVTemps tToday ;
	tToday.takeTime() ;

	for (ArrayConcernIter j = pConcerns->begin(); j != pConcerns->end(); j++)
	{
		// Préoccupation ouverte
		if (((*j)->_tDateOuverture <= tToday) &&
				(((*j)->_tDateFermeture.estNoLimit()) || ((*j)->_tDateFermeture >= tToday)))
		{
      string sNodeId = (*j)->getNoeud() ;

      NSDocumentHisto* pDocument = pFrameInfo->findNode(sNodeId) ;
      if (pDocument)
      {
        NSPatPathoArray* pPPT = pDocument->getPatPathoPointer() ;

        if (false == pPPT->empty())
        {
			    PatPathoIter pptIter = pPPT->ChercherNoeud(sNodeId) ;
			    if ((NULL != pptIter) && (pPPT->end() != pptIter))
			    {
				    string sConcernCodeSens = (*pptIter)->getLexiqueSens() ;
				    if (sCodeSens == sConcernCodeSens)
        	    return *j ;
          }
        }
			}
		}
	}

	return (NSConcern*) 0 ;
}

NSLdvDrug*
NSLdvDocumentBase::donneMostRecentOpenDrug(LDVFRAME iFrame, string sCodeSens)
{
	return donneNextRecentOpenDrug(iFrame, sCodeSens, NULL) ;
}

NSLdvDrug*
NSLdvDocumentBase::donneNextRecentOpenDrug(LDVFRAME iFrame, string sCodeSens, NSLdvDrug* pPreviousDrug)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
		return (NSLdvDrug*) 0 ;

  ArrayLdvDrugs* pDrugs = pFrameInfo->getTreatmentsArray() ;
	if (((ArrayLdvDrugs*) NULL == pDrugs) || (pDrugs->empty()))
		return (NSLdvDrug*) 0 ;

	drugsIter iStartIter = pDrugs->begin() ;
  if (pPreviousDrug)
	{
		for ( ; (pDrugs->end() != iStartIter) && (*iStartIter != pPreviousDrug); iStartIter++) ;
    if (pDrugs->end() == iStartIter)
  		return (NSLdvDrug*) 0 ;

		iStartIter++ ;
    if (pDrugs->end() == iStartIter)
  		return (NSLdvDrug*) 0 ;
	}

	NVLdVTemps tToday ;
	tToday.takeTime() ;

  // Exact search
  //
	for (drugsIter j = iStartIter ; pDrugs->end() != j ; j++)
	{
		// Préoccupation ouverte
		if (((*j)->_tDateOuverture <= tToday) &&
				(((*j)->_tDateFermeture.estNoLimit()) || ((*j)->_tDateFermeture >= tToday)))
		{
    	string sConcern = (*j)->getLexique() ;
      string sConcernCodeSens ;
      pContexte->getDico()->donneCodeSens(&sConcern, &sConcernCodeSens) ;
      if (sCodeSens == sConcernCodeSens)
      	return *j ;
		}
	}

  // Semantic search
  //


  // Don't do that because "X -est un-> antihypertenseur" renvoit 20000 éléments
  //
  // VectString VSEquivalent ;
	// pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sCodeSens, "ES", &VSEquivalent, "ENVERS") ;

  for (drugsIter j = iStartIter ; pDrugs->end() != j ; j++)
	{
		// Préoccupation ouverte
		if (((*j)->_tDateOuverture <= tToday) &&
				(((*j)->_tDateFermeture.estNoLimit()) || ((*j)->_tDateFermeture >= tToday)))
		{
      string sConcern = (*j)->getLexique() ;
      string sConcernCodeSens ;
      pContexte->getDico()->donneCodeSens(&sConcern, &sConcernCodeSens) ;

      VectString VSEquivalent ;
      pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sConcernCodeSens, "ES", &VSEquivalent) ;

      if (VSEquivalent.contains(sCodeSens))
      	return *j ;
		}
	}

	return (NSLdvDrug*) 0 ;
}

/**
 * Set the latest precription date for a drug
 */
void
NSLdvDocumentBase::setDrugPrescriptionDate(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
    return ;

  pDrug->setLatestPrescriptionDate(string("")) ;

  NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

  string sNodeMedic = pDrug->getNoeud() ;

  NSFrameInformation* pFrame = getFrameForNode(sNodeMedic) ;
  if ((NSFrameInformation*) NULL == pFrame)
    return ;

  ArrayLdvDrugs* pDrugs = pFrame->getTreatmentsArray() ;
  if ((ArrayLdvDrugs*) NULL == pDrugs)
    return ;

  string sNodeRenew = pDrugs->getLastRenewNode(sNodeMedic) ;
  if ((string("") != sNodeRenew) && (sNodeRenew != sNodeMedic))
    sNodeMedic = sNodeRenew ;

  // on récupère le lien éventuel sur un traitement
  VecteurString aTreatments ;
  pGraphe->TousLesVrais(sNodeMedic, NSRootLink::treatmentOf, &aTreatments) ;
  if (aTreatments.empty())
    return ;

  // on récupère le premier noeud pour avoir le code document ordonnance
  string sNodeOrdo = *(*(aTreatments.begin())) ;
  string sDocOrdo  = string(sNodeOrdo, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

  // on recherche le pDocHis correspondant dans l'historique
  // et on affiche la date du document (en général date de création)
  if ((pContexte->getPatient()) && (pContexte->getPatient()->getDocHis()))
  {
    NSDocHistoArray* pVectDocument = pContexte->getPatient()->getDocHis()->getVectDocument() ;
    if (pVectDocument && (false == pVectDocument->empty()))
    {
      DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sDocOrdo) ;
      if ((DocumentIter) NULL != iterDoc)
        pDrug->setLatestPrescriptionDate((*iterDoc)->getDateDoc()) ;
    }
  }
}

bool
NSLdvDocumentBase::updateIndexDocument(NSDocumentHisto* pDocument, bool bUpdateDisplay)
{
  if ((NSDocumentHisto*) NULL == pDocument)
    return false ;

  // Can we work ?
	if ((NSPatientChoisi*) NULL == pContexte->getPatient())
	{
    pContexte->getSuperviseur()->traceAndDisplay("patientManagement", "cannotFindPatient") ;
		return false;
	}

	NSHISTODocument* pHistory = pContexte->getPatient()->getDocHis() ;
  if ((NSHISTODocument*) NULL == pHistory)
	{
    pContexte->getSuperviseur()->traceAndDisplay("documentManagementErrors", "cannotFindHistoryManager") ;
		return false ;
	}

	NSNoyauDocument Noyau((TDocument*) 0, pDocument, 0, pContexte, false, true) ;

  NSPatPathoArray* pDocPatho = new NSPatPathoArray(pContexte->getSuperviseur()) ;
  pDocument->initFromPatPatho(pDocPatho) ;

	Noyau.setPatPatho(pDocPatho) ;

	// Enregistrement du document modifié
	Noyau.enregistrePatPatho() ;

	bool bReload = Noyau.chargePatPatho() ;
	if (false == bReload)
		return false ;

  Noyau.initFromPatPatho(pDocPatho) ;

  // pHistory->Rafraichir(pDocument, &DocPatho) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("FreshenDocument")) ;
  pTask->setPointer1(pDocument, false) ;
  pTask->setPointer2(pDocPatho, true) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  return true ;
}

/*
bool
NSLdvDocumentBase::updateGenericIndex(LDVFRAME iFrame)
{
  if (ldvframeNotSpecified == iFrame)
  {
    for (int i = 0 ; i < FRAMECOUNT ; i++)
      updateGenericIndex(getFrameForIndex(i)) ;

    return true ;
  }

	// Can we work ?
	if (NULL == pContexte->getPatient())
	{
		pContexte->getSuperviseur()->traceAndDisplay("patientManagement", "cannotFindPatient") ;
		return false;
	}

	NSHISTODocument* pHistory = pContexte->getPatient()->pDocHis ;

  if (NULL == pHistory)
	{
		erreur("Erreur grave (historique introuvable). Vous devriez relancer le logiciel.", standardError, 0) ;
		return false;
	}
  NSDocHistoArray* pVectDocument = pHistory->getVectDocument() ;
	if ((NULL == pVectDocument) || pVectDocument->empty())
	{
		erreur("Erreur grave (historique vide). Vous devriez relancer le logiciel.", standardError, 0) ;
		return false;
	}

  string sRoot = getRootForFrame(iFrame) ;
  if (string("") == sRoot)
    return false ;

  string sRootSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sRoot, &sRootSens) ;

	// Recherche du document "Index de santé"
	// Looking for "Health index" document
	DocumentIter iterDoc ;

	bool bSearchIndex = true ;
	while (bSearchIndex)
	{
		bool bContinuer = true ;
		iterDoc = pVectDocument->begin() ;
		while ((pVectDocument->end() != iterDoc) && bContinuer )
		{
    	if ((*iterDoc)->pPatPathoArray && (false == (*iterDoc)->pPatPathoArray->empty()))
			{
      	PatPathoIter iter = (*iterDoc)->pPatPathoArray->begin() ;
        if ((*iter)->getLexiqueSens(pContexte->getSuperviseur()) == sRootSens)
        	bContinuer = false ;
        else
        	iterDoc++ ;
      }
      else
      	iterDoc++ ;
		}

		// Index de santé non trouvé - Health index not found
		if (pVectDocument->end() == iterDoc)
		{
    	// En MUE on signale une erreur car l'index de santé est normalement créé
      // lors de l'initialisation du patient (méthode NSPatientChoisi::CreeRootDocs())
      ::MessageBox(pContexte->GetMainWindow()->GetHandle(),
              "Ce patient ne semble pas avoir de document des Index de santé.",
                            "Attention", MB_OK) ;

      return false ;

			// Rafraichir l'historique
		}
		else
			bSearchIndex = false ;
	}
	if ((pVectDocument->end() == iterDoc) || (NULL == iterDoc))
		return false ;

  // Find frame information
  //
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if (NULL == pFrameInfo)
    return false ;

  NSPatPathoArray *pPpt = pFrameInfo->getPatPatho() ;
  if (NULL == pPpt)
    return false ;

	// Health index found

	NSDocumentInfo Docum(pContexte) ;
	*(Docum.getData()) = *((*iterDoc)->getData()) ;

	NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false) ;

	*(Noyau.pPatPathoArray) = *pPpt ;

	// Enregistrement du document modifié
	Noyau.enregistrePatPatho() ;

	bool bReload = Noyau.chargePatPatho() ;
	if (false == bReload)
		return false ;

  pHistory->Rafraichir(*iterDoc, Noyau.pPatPathoArray) ;

  // No use, since it is a pointer to the ppt in history that we just refreshed
	// *_pPathoPOMRIndex = *(Noyau.pPatPathoArray) ;

	return true ;
}
*/

bool
NSLdvDocumentBase::connectObjectToDrug(LDVFRAME iFrame, string sCodeDrug, int iSeverity, string sNoeud, NSRootLink::NODELINKTYPES iRelation, bool bAutoCreate)
{
try
{
	string sCodeSensDrug ;
	pContexte->getDico()->donneCodeSens(&sCodeDrug, &sCodeSensDrug) ;

	//
	// Recherche d'une préoccupation OUVERTE correspondant à sCodeConcern
	//
	NSLdvDrug* pDrug = donneMostRecentOpenDrug(iFrame, sCodeSensDrug) ;

  if (NULL == pDrug)
		return false ;

	string sDrugRef = pDrug->getNoeud() ;

  //
	// Drug was found, we connect
	//
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
  pGraphe->etablirLienDocument(sNoeud, iRelation, sDrugRef) ;

	//
	// On met à jour l'Objet qui représente le document
	//
/*
	NSLdvObjet* pObj = pObjets->getObjet(sNoeud);

	if (pObj != NULL)
	{
		pObj->sConcern = sConcernRef;

		TView* pView = GetViewList();
		do
		{
			NSLdvView* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView);
			if (pLdvView)
				pLdvView->addObjOnConcern(pConcern, pObj);

			pView = NextView(pView);
		}
		while (pView);
	}
*/

	return true ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::connectObjectToDrug.", standardError, 0) ;
	return false ;
}
}

// A new tree is introduced to goals and bbk
//
bool
NSLdvDocumentBase::showNewTree(const NSPatPathoArray *pPatPathoArray, string sDateDoc)
{
	if (((NSPatPathoArray*) NULL == pPatPathoArray) || (pPatPathoArray->empty()))
    return false ;

  // Are there goals?
  //
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

  // No goals, just introduce to bbk
  //
  if (false == bGoalExist)
  {
    showNewTreeToBbk(pPatPathoArray, sDateDoc) ;
    return false ;
  }

	bool bModified = false ;

	// ---------------------------------------------------------------------------
	// Le noeud racine remet-il à zéro un objectif ?
	string sRootConcept = (*(pPatPathoArray->begin()))->getLexique() ;
	string sRootSens ;
	pContexte->getDico()->donneCodeSens(&sRootConcept, &sRootSens) ;

	string sRootNode = (*(pPatPathoArray->begin()))->getNode() ;
	if (string("") == sRootNode)
		return bModified ;

	string sDateDocum = sDateDoc ;
	// NSLinkManager* pGraphe = pContexte->getPatient()->pGraphPerson->pLinkManager ;

	// si date au format AAAAMMJJ :
  	// on ajoute l'heure
	if (strlen(sDateDocum.c_str()) == 8)
		sDateDocum += "000000" ;

	ArrayGoals GoalsWithValue(this) ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
      if (pGoals && (false == pGoals->empty()))
      {
        // ---------------------------------------------------------------------------
        // On passe en revue tous les objectifs
        for (ArrayGoalIter goalIt = pGoals->begin() ; pGoals->end() != goalIt ; goalIt++)
        {
          // -------------------------------------------------------------------------
          // Si l'objectif est ouvert, on regarde si le noeud est un reseter
          if (false == (*goalIt)->_tOuvertLe.estVide())
          {
            string sDateOuvr = (*goalIt)->_tOuvertLe.donneDateHeure() ;
            string sDateFerm = string("") ;
            if (false == (*goalIt)->_tFermeLe.estVide())
              sDateFerm = (*goalIt)->_tFermeLe.donneDateHeure() ;

            // Does document occur between goal opening and closing dates?
            //
            if ((sDateDocum >= sDateOuvr) && ((string("") == sDateFerm) || (sDateDocum < sDateFerm)))
            {
              // Si l'objectif correspond au document
              string sGoalSens ;
              pContexte->getDico()->donneCodeSens(&((*goalIt)->getLexique()), &sGoalSens) ;

              if (sGoalSens == sRootSens)
              {
                if      (NSLdvGoal::cyclic == (*goalIt)->_iRythme)
                  pContexte->getPatient()->getGraphPerson()->ConnectRootTreeToGoal(pContexte, sRootNode, (*goalIt)->getNode(), NSRootLink::goalReseter) ;
                else if (NSLdvGoal::permanent != (*goalIt)->_iRythme)
                  pContexte->getPatient()->getGraphPerson()->ConnectRootTreeToGoal(pContexte, sRootNode, (*goalIt)->getNode(), NSRootLink::goalCloser) ;

                // Permanent goals are not modified here
                //
                if (NSLdvGoal::permanent != (*goalIt)->_iRythme)
                  bModified = true ;
              }

              // If document is a closing event for goal
              //
              string sCloseSens ;
              pContexte->getDico()->donneCodeSens(&((*goalIt)->_sCloseEventNode), &sCloseSens) ;
              if (sCloseSens == sRootSens)
              {
                pContexte->getPatient()->getGraphPerson()->ConnectRootTreeToGoal(pContexte, sRootNode, (*goalIt)->getNode(), NSRootLink::goalCloser) ;
                bModified = true ;
              }
            }
          }

          // -------------------------------------------------------------------------
          // Si l'objectif est encore fermé, on regarde si le noeud l'ouvre
          else
          {
            string sOpenSens ;
            pContexte->getDico()->donneCodeSens(&((*goalIt)->_sOpenEventNode), &sOpenSens) ;
            if (sOpenSens == sRootSens)
            {
              pContexte->getPatient()->getGraphPerson()->ConnectRootTreeToGoal(pContexte, sRootNode, (*goalIt)->getNode(), NSRootLink::goalOpener) ;
              bModified = true ;
            }
          }

          // -------------------------------------------------------------------------
          // on fait une liste des objectifs qui ont une valeur
          if (((*goalIt)->_bValue) || ('V' == (*goalIt)->getLexique()[0]))
            GoalsWithValue.push_back((*goalIt)) ;
        }
      }
    }
  }

	//
	// Gestion des objectifs chiffrés - Managing goals on values
	//
	if (false == GoalsWithValue.empty())
	{
		// -------------------------------------------------------------------------
		// on parcourt la patpatho
		for (PatPathoConstIter pathoIter = pPatPathoArray->begin() ; pathoIter != pPatPathoArray->end() ; pathoIter++)
		{
			if (pContexte->getDico()->CodeCategorie((*pathoIter)->getLexique()) == string(1, '£'))
			{
				// on est dans le cas d'une valeur chiffrée
				// on recherche le père qui correspond à la valeur mesurée
				PatPathoConstIter pptPereIter = pathoIter ;

				while ( (NULL != pptPereIter)                         &&
        				(pPatPathoArray->begin() != pptPereIter)      &&
                (pContexte->getDico()->CodeCategorie((*pptPereIter)->getLexique()) != string(1, 'V')) &&
                (pContexte->getDico()->CodeCategorie((*pptPereIter)->getLexique()) != string(1, 'K')))
					pptPereIter = pPatPathoArray->ChercherPere(pptPereIter) ;

				if (NULL != pptPereIter)
				{
					NSPatPathoArray PatPathoPere(pContexte->getSuperviseur()) ;
          pPatPathoArray->ExtrairePatPathoWithRoot(pptPereIter, &PatPathoPere) ;

          string sPereConcept = (*pptPereIter)->getLexique() ;
          string sPereSens ;
          pContexte->getDico()->donneCodeSens(&sPereConcept, &sPereSens) ;

          string sPereNode = (*pptPereIter)->getNode() ;

          // on a trouvé le pere, on regarde si ce pere correspond à un objectif à valeur
          for (ArrayGoalIter goalIter = GoalsWithValue.begin() ; GoalsWithValue.end() != goalIter ; goalIter++)
          {
          	// -------------------------------------------------------------------------
            // Si l'objectif est ouvert, on regarde si le noeud est un reseter
            if (false == (*goalIter)->_tOuvertLe.estVide())
            {
            	string sDateOuvr = (*goalIter)->_tOuvertLe.donneDateHeure() ;
              string sDateFerm = "" ;
              if (false == (*goalIter)->_tFermeLe.estVide())
              	sDateFerm = (*goalIter)->_tFermeLe.donneDateHeure() ;

              if ((sDateDocum >= sDateOuvr) && ((string("") == sDateFerm) || (sDateDocum < sDateFerm)))
              {
              	// Si l'objectif correspond au document
                string sGoalSens ;
                pContexte->getDico()->donneCodeSens(&((*goalIter)->getLexique()), &sGoalSens) ;

                if (sGoalSens == sPereSens)
                {
                	if      ((NSLdvGoal::cyclic    == (*goalIter)->_iRythme) ||
                  				 (NSLdvGoal::permanent == (*goalIter)->_iRythme)) // pour suivi permanent d'une valeur
										pContexte->getPatient()->getGraphPerson()->ConnectNodeToGoal(pContexte, sPereNode, (*goalIter)->getNode(), &PatPathoPere, NSRootLink::goalReseter) ;
                  else if (NSLdvGoal::permanent != (*goalIter)->_iRythme)
										pContexte->getPatient()->getGraphPerson()->ConnectNodeToGoal(pContexte, sPereNode, (*goalIter)->getNode(), &PatPathoPere, NSRootLink::goalCloser) ;

                  // if ((*goalIter)->iRythme != NSLdvGoal::permanent)
                  	bModified = true ;
								}

                string sCloseSens ;
                pContexte->getDico()->donneCodeSens(&((*goalIter)->_sCloseEventNode), &sCloseSens) ;
                if (sCloseSens == sPereSens)
                {
									pContexte->getPatient()->getGraphPerson()->ConnectNodeToGoal(pContexte, sPereNode, (*goalIter)->getNode(), &PatPathoPere, NSRootLink::goalCloser) ;
									bModified = true ;
								}
							}
						}
						// -------------------------------------------------------------------------
            // Si l'objectif est encore fermé, on regarde si le noeud l'ouvre
            else
            {
            	string sOpenSens ;
              pContexte->getDico()->donneCodeSens(&((*goalIter)->_sOpenEventNode), &sOpenSens) ;
							if (sOpenSens == sPereConcept)
							{
								pContexte->getPatient()->getGraphPerson()->ConnectNodeToGoal(pContexte, sPereNode, (*goalIter)->getNode(), &PatPathoPere, NSRootLink::goalOpener) ;
								bModified = true ;
							}
						}
					}
				}
			}
		}

		// -------------------------------------------------------------------------
		// on delete la liste de goals avec valeur sans effacer les objectifs
		GoalsWithValue.clear() ;
	}

  return bModified ;
}

void
NSLdvDocumentBase::showNewTreeToBbk(const NSPatPathoArray *pPatPathoArray, string sDateDoc)
{
  NautilusEvent nsEvent(pPatPathoArray, pPatPathoArray->begin(), ADD, true) ;

  NVLdVTemps dateDoc ;
  dateDoc.initFromDate(sDateDoc) ;
  nsEvent.putDate(dateDoc) ;

  pContexte->getBBinterface()->addNautilusEvent(&nsEvent) ;
}

bool
NSLdvDocumentBase::isALDDrug(string sNodeMedic)
{
	if (sNodeMedic == string(""))
		return false ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvDrug *pDrug = getTreatment(sNodeMedic, iFrame) ;
  if (NULL == pDrug)
    return false ;

  return pDrug->_bALD ;

  //
  // Ancienne méthode : regarder si le médicament est attaché à une PS en ALD
  //
/*
	NSLinkManager* pGraphe = pContexte->getPatient()->pGraphPerson->pLinkManager ;

	VecteurString aConcerns ;
	pGraphe->TousLesVrais(sNodeMedic, NSRootLink::drugOf, &aConcerns) ;
	bool bALDdrug = false ;
	if (aConcerns.empty())
		return false ;

	ArrayConcern* pConcerns = getConcerns() ;
	EquiItemIter iCcc = aConcerns.begin();
	for ( ; iCcc != aConcerns.end(); iCcc++)
	{
		NSConcern* pConcern = pConcerns->getConcern(**iCcc) ;
    if (pConcern && pConcern->pPptDetails && (!(pConcern->pPptDetails->empty())))
    {
    	if (pConcern->pPptDetails->CheminDansPatpatho("LADMI/LBARZ"))
      {
      	bALDdrug = true ;
        break ;
      }
    }
	}
	return bALDdrug ;
*/
}

void
NSLdvDocumentBase::connectValuedNode(NSPatPathoArray *pPatPathoArray, PatPathoIter iter, string sGoalNode)
{
}

void
NSLdvDocumentBase::showDrugProperty(NSLdvDrug *pDrug)
{
	if (((NSLdvDrug*) NULL == pDrug) || ((NSPatientChoisi*) NULL == pContexte->getPatient()))
		return ;

	string sNode = pDrug->getNoeud() ;

  // pContexte->getPatient()->goals_show(sNode) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("OpenNewWindow")) ;
  pTask->setParam1(string("GoalManagement")) ;
  pTask->setParam2(sNode) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  // pContexte->getPatient()->followUp_show(sNode) ;
  //
  NSToDoTask* pTask2 = new NSToDoTask ;
  pTask2->setWhatToDo(string("OpenNewWindow")) ;
  pTask2->setParam1(string("FollowUpShow")) ;
  pTask2->setParam2(sNode) ;
  pTask2->sendMe(pContexte->getSuperviseur()) ;
}

bool
NSLdvDocumentBase::createXmlTree(NSLdvDrug *pDrug, Ctree* pTreeObject, string sTargetLocalisation)
{
	if (((Ctree*) NULL == pTreeObject) || ((NSLdvDrug*) NULL == pDrug))
		return false ;

	// Setting the "localisation" attribute
  //
  if (string("") == sTargetLocalisation)
		sTargetLocalisation = string("ZPOMR/N0000") ;

  string sLoc = pTreeObject->getStringAttribute(ATTRIBUT_TREE_LOC) ;
  if (string("") != sLoc)
  {
  	if (sTargetLocalisation != sLoc)
    	pTreeObject->setStringAttribute(ATTRIBUT_TREE_LOC, sTargetLocalisation) ;
  }
  else
  	pTreeObject->addAttribute(ATTRIBUT_TREE_LOC, sTargetLocalisation) ;

	//
  // Adding Root nodes and asking them to create their sons recursively
  //

	// Looking for this drug in the health index document
  //
	NSPatPathoArray* pPatho ;
	PatPathoIter iter = donneDrug(pDrug, &pPatho) ;
  // not found !
	if (((PatPathoIter) NULL == iter) || (pPatho->end() == iter))
  	return false ;

	NSPatPathoArray drugPatho(pContexte->getSuperviseur()) ;
  pPatho->ExtrairePatPathoWithRoot(iter, &drugPatho) ;

  // First step: remove referential Ids
  //
  PatPathoIter RefIdIter ;
  bool bIsID = drugPatho.CheminDansPatpatho(string("£RE"), string(1, cheminSeparationMARK), &RefIdIter) ;
  while (true == bIsID)
  {
    drugPatho.SupprimerItem(RefIdIter) ;
    bIsID = drugPatho.CheminDansPatpatho(string("£RE"), string(1, cheminSeparationMARK), &RefIdIter) ;
  }

	int iCol = 0 ;

	// Find opening
  //
  PatPathoIter iter2 = drugPatho.begin() ;
  iter2++ ;
  PatPathoIter iterClose = 0 ;

  int iTotalDurationValueYears  = 0 ;
  int iTotalDurationValueMonths = 0 ;
  int iTotalDurationValueDays   = 0 ;

  while (drugPatho.end() != iter2)
  {
    if (iCol + 1 == (*iter2)->getColonne())
    {
    	string sSens = (*iter2)->getLexiqueSens() ;

      // Opening date : switch to "now"
      //
      if      (string("KOUVR") == sSens)
      {
      	iter2++ ;
        if (drugPatho.end() != iter2)
        {
        	sSens = (*iter2)->getLexiqueSens() ;
          if ((string("£D0") == sSens) || (string("£T0") == sSens))
          	(*iter2)->setComplement("$KAUJO+000000") ;
        }
      }
      else if (string("KFERM") == sSens)
      {
      	iter2++ ;
        if (drugPatho.end() != iter2)
        {
        	sSens = (*iter2)->getLexiqueSens() ;
          if ((string("£D0") == sSens) || (string("£T0") == sSens))
          	iterClose = iter2 ;
        }
      }
      else if (string("KPHAT") == sSens)
      {
      	int iColPhase = (*iter2)->getColonne() ;
      	iter2++ ;

        PatPathoIter iterPhaseClose = 0 ;

        int iPhaseDurationValueYears  = 0 ;
				int iPhaseDurationValueMonths = 0 ;
				int iPhaseDurationValueDays   = 0 ;

        while ((drugPatho.end() != iter2) && ((*iter2)->getColonne() > iColPhase))
        {
        	sSens = (*iter2)->getLexiqueSens() ;

          // Opening date : switch to "now"
      		//
      		if      (string("KOUVR") == sSens)
      		{
      			iter2++ ;
        		if (drugPatho.end() != iter2)
        		{
        			sSens = (*iter2)->getLexiqueSens() ;
          		if ((string("£D0") == sSens) || (string("£T0") == sSens))
              {
              	string sDeltaTps = donne_deltaTemps(iTotalDurationValueYears, iTotalDurationValueMonths, iTotalDurationValueDays) ;
          			(*iter2)->setComplement(string("$KAUJO+") + sDeltaTps) ;
              }
        		}
      		}
      		else if (string("KFERM") == sSens)
      		{
      			iter2++ ;
        		if (drugPatho.end() != iter2)
        		{
        			sSens = (*iter2)->getLexiqueSens() ;
          		if ((string("£D0") == sSens) || (string("£T0") == sSens))
          			iterPhaseClose = iter2 ;
        		}
      		}
          else if (string("VDURE") == sSens)
      		{
      			iter2++ ;
        		if (drugPatho.end() != iter2)
        		{
        			sSens = (*iter2)->getLexiqueSens() ;
          		if (string("£N0") == sSens)
              {
                string sValue = (*iter2)->getComplement() ;
                int    iPhaseDurationValue = atoi(sValue.c_str()) ;
								string sPhaseDurationUnit  = (*iter2)->getUnitSens() ;

                // Year
            		if      (string("2DAT3") == sPhaseDurationUnit)
                	iPhaseDurationValueYears  = iPhaseDurationValue ;
            		// Month
            		else if (string("2DAT2") == sPhaseDurationUnit)
                	iPhaseDurationValueMonths = iPhaseDurationValue ;
                // Week
            		else if (string("2DAT1") == sPhaseDurationUnit)
                	iPhaseDurationValueDays   = 7 * iPhaseDurationValue ;
            		// Day
            		else if (string("2DAT0") == sPhaseDurationUnit)
                	iPhaseDurationValueDays   = iPhaseDurationValue ;
              }
        		}
      		}
          iter2++ ;
        }
        iTotalDurationValueYears  = iPhaseDurationValueYears ;
				iTotalDurationValueMonths = iPhaseDurationValueMonths ;
				iTotalDurationValueDays   = iPhaseDurationValueDays ;

        if (NULL != iterPhaseClose)
        {
        	string sDeltaTps = donne_deltaTemps(iTotalDurationValueYears, iTotalDurationValueMonths, iTotalDurationValueDays) ;
          (*iterPhaseClose)->setComplement(string("$KAUJO+") + sDeltaTps) ;
        }
      }
    }
    if (drugPatho.end() != iter2)
  		iter2++ ;
  }
  if (iterClose)
  {
  	string sDeltaTps = donne_deltaTemps(iTotalDurationValueYears, iTotalDurationValueMonths, iTotalDurationValueDays) ;
    (*iterClose)->setComplement(string("$KAUJO+") + sDeltaTps) ;
  }

	*(pTreeObject->pPatPathoArray) = drugPatho ;

	return pTreeObject->initFromPatPatho() ;
}

string
NSLdvDocumentBase::getPrescriptionDate(NSLdvDrug *pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
    return string("") ;

  NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

  string sNodeMedic = pDrug->getNoeud() ;

  NSFrameInformation* pFrame = getFrameForNode(sNodeMedic) ;
  if ((NSFrameInformation*) NULL == pFrame)
    return string("") ;

  ArrayLdvDrugs* pDrugs = pFrame->getTreatmentsArray() ;
  if ((ArrayLdvDrugs*) NULL == pDrugs)
    return string("") ;

  string sNodeRenew = pDrugs->getLastRenewNode(sNodeMedic) ;
  if ((string("") != sNodeRenew) && (sNodeRenew != sNodeMedic))
    sNodeMedic = sNodeRenew ;

  // on récupère le lien éventuel sur un traitement
  VecteurString aTreatments ;
  pGraphe->TousLesVrais(sNodeMedic, NSRootLink::treatmentOf, &aTreatments) ;
  if (aTreatments.empty())
    return string("") ;

  // on récupère le premier noeud pour avoir le code document ordonnance
  string sNodeOrdo = *(*(aTreatments.begin())) ;
  string sDocOrdo  = string(sNodeOrdo, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

  // on recherche le pDocHis correspondant dans l'historique
  // et on affiche la date du document (en général date de création)
  if ((pContexte->getPatient()) && (pContexte->getPatient()->getDocHis()))
  {
    NSDocHistoArray* pVectDocument = pContexte->getPatient()->getDocHis()->getVectDocument() ;
    if (pVectDocument && (false == pVectDocument->empty()))
    {
      DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sDocOrdo) ;
      if (NULL != iterDoc)
        return (*iterDoc)->getDateDoc() ;
    }
  }

  return string("") ;
}

/**
 * Get all active drugs that are in a BAM compatible format
 */
bool
NSLdvDocumentBase::getActiveBamCompatibleDrugs(NsSelectableDrugArray* paDrugsList)
{
  if ((NsSelectableDrugArray*) NULL == paDrugsList)
    return false ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    ArrayLdvDrugs *pDrugs = getDrugs(iFrame) ;
    if (pDrugs && (false == pDrugs->empty()))
    {
      NVLdVTemps tpsNow ;
      tpsNow.takeTime() ;

      for (drugsIter itDg = pDrugs->begin() ; pDrugs->end() != itDg ; itDg++)
      {
        if (((*itDg)->_tDateFermeture.estNoLimit()) || ((*itDg)->_tDateFermeture >= tpsNow))
        {
          if ((NsSelectableDrug*) NULL == (*itDg)->getBamDrug())
            (*itDg)->initBamDrug() ;

          if ((*itDg)->getBamDrug())
            paDrugsList->push_back(new NsSelectableDrug(*((*itDg)->getBamDrug()))) ;
        }
      }
    }
  }
  return true ;
}

void
NSLdvDocumentBase::addGoalToConcern(NSLdvGoal* pAddedGoal)
{
	if ((NULL == pAddedGoal) || (string("") == pAddedGoal->getConcern()))
		return ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
	NSConcern* pGoalConcern = getConcern(pAddedGoal->getConcern(), iFrame) ;
	if (pGoalConcern)
  {
  	pGoalConcern->goalAdded(pAddedGoal) ;
		return ;
  }

  // It is no longer allowed to add a goal to a treatment
/*
	NSLdvDrug* pGoalDrug = _pDrugs->getDrug(pAddedGoal->sConcern) ;
	if (pGoalDrug)
  {
  	pGoalDrug->goalAdded(pAddedGoal) ;
		return ;
  }
*/
}

void
NSLdvDocumentBase::goalModified(NSLdvGoal* pModifiedGoal)
{
	if ((NULL == pModifiedGoal) || (pModifiedGoal->getConcern() == string("")))
		return ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
	NSConcern* pGoalConcern = getConcern(pModifiedGoal->getConcern(), iFrame) ;
	if (pGoalConcern)
  {
  	goalModifiedForConcern(pModifiedGoal, pGoalConcern) ;
		return ;
  }

/*
	NSLdvDrug* pGoalDrug = _pDrugs->getDrug(pModifiedGoal->sConcern) ;
	if (pGoalDrug)
  {
  	goalModifiedForDrug(pModifiedGoal, pGoalDrug) ;
		return ;
  }
*/
}

void
NSLdvDocumentBase::reinitAllGoals()
{
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
      if (pGoals)
        pGoals->reinit() ;

      // Update for health concerns
      //
      ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;
      if (pConcerns && (false == pConcerns->empty()))
        for (ArrayConcernIter itConcern = pConcerns->begin();
                             pConcerns->end() != itConcern ; itConcern++)
			    (*itConcern)->goalModified(NULL) ;
    }
  }

	// Update for drugs
  //
/*
	if (false == _pDrugs->empty())
		for (drugsIter itDrug = _pDrugs->begin();
                                    itDrug != _pDrugs->end(); itDrug++)
			(*itDrug)->goalModified(NULL) ;
*/
}

void
NSLdvDocumentBase::closeGoal(NSLdvGoal* pGoal, NSPatPathoArray* pPatPathoMotif)
{
	NVLdVTemps tDateFin ;
	tDateFin.takeTime() ;

	closeGoal(pGoal, &tDateFin, pPatPathoMotif) ;
}

void
NSLdvDocumentBase::closeGoal(NSLdvGoal* pGoal, NVLdVTemps* pDateFin, NSPatPathoArray* pPatPathoMotif)
{
try
{
	if ((NSLdvGoal*) NULL == pGoal)
		return ;

  LDVFRAME iFrame = pGoal->getIFrame() ;
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
    return ;

	// Recherche de cette préoccupation dans l'index de santé
	// Looking for this concern in the health index document
  //
	NSPatPathoArray* pPatho ;
	PatPathoIter iter = donneGoal(pGoal, &pPatho) ;

    // not found !
	if ((NULL == iter) || (pPatho->end() == iter))
  	return ;

	int iColonne = (*iter)->getColonne() ;

	// Recherche de la date de fin pour la modifier
	// Looking for ending date - in order to modify it
	PatPathoIter iterBis = pPatho->begin() ;
	iterBis = iter ;

	iterBis++ ;
	while (	(iterBis != pPatho->end()) && ((*iterBis)->getColonne() > iColonne) &&
					(((*iterBis)->getColonne() != iColonne+1) ||
						((*iterBis)->getLexique() != "KFERM1")))
		iterBis++ ;

	// On a trouvé une précédente date de fin, on la modifie
	// We have found a previous ending date, we can change it
	if ((iterBis != pPatho->end()) &&
			((*iterBis)->getColonne() == iColonne+1) &&
			((*iterBis)->getLexique() == "KFERM1"))
	{
		iterBis++ ;
    (*iterBis)->setLexique(string("£T0;19")) ;
		(*iterBis)->setComplement(pDateFin->donneDateHeure()) ;
    (*iterBis)->setUnit(string("2DA021")) ;

    if ((pPatPathoMotif) && (!(pPatPathoMotif->empty())))
    {
    	iterBis++ ;
      while ((iterBis != pPatho->end()) && ((*iterBis)->getColonne() > iColonne+1))
      	iterBis++ ;
      pPatho->InserePatPatho(iterBis, pPatPathoMotif, iColonne+1) ;
    }
	}

	// Si on n'a pas trouvé de date de fin à modifier, on en crée une
	// If we didn't find any ending date, we must create it
	else
	{
  	iterBis = iter ;

		// On recherche la date de début, pour s'insérer juste après
		// Looking for starting date - to insert ending date just after
		iterBis++ ;
		while (	(iterBis != pPatho->end()) && ((*iterBis)->getColonne() > iColonne) &&
						(((*iterBis)->getColonne() != iColonne+1) ||
						((*iterBis)->getLexiqueSens() != string("KOUVR"))))
			iterBis++ ;

        // Si on est à la fin ou qu'on est tombé sur un père sans passer par une date d'ouverture, on sort
		if ((iterBis == pPatho->end()) || ((*iterBis)->getColonne() != iColonne+1))
    	return ;

    iterBis++ ;
    while ((iterBis != pPatho->end()) && ((*iterBis)->getColonne() > iColonne+1))
    	iterBis++ ;

    NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

  	PatPatho.ajoutePatho("KFERM1", 0) ;
    Message Msg ;
    Msg.SetUnit("2DA021") ;
    Msg.SetComplement(pDateFin->donneDateHeure()) ;
    PatPatho.ajoutePatho("£T0;19", &Msg, 1) ;

    if ((pPatPathoMotif) && (false == pPatPathoMotif->empty()))
    	PatPatho.InserePatPatho(PatPatho.end(), pPatPathoMotif, 0) ;

    pPatho->InserePatPatho(iterBis, &PatPatho, iColonne+1) ;
	}

	pGoal->_tDateFermeture = *pDateFin ;

  NSDocumentHisto* pDocument = pFrameInfo->findNode(pGoal->getNode()) ;
  if ((NSDocumentHisto*) NULL == pDocument)
    return ;

	updateIndexDocument(pDocument, false) ;

	reinitAllGoals() ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::closeGoal.", standardError, 0) ;
	return ;
}
}

void
NSLdvDocumentBase::goalAdded(NSLdvGoal* pGoal, GoalInfoArray *pWorstJalons)
{
try
{
	if (((NSLdvGoal*) NULL == pGoal) || ((GoalInfoArray*) NULL == pWorstJalons))
		return ;

	GoalInfoArray* pJalons = pGoal->getMetaJalons() ;

	if (((GoalInfoArray*) NULL == pJalons) || (pJalons->empty()))
		return ;

	if (pWorstJalons->empty())
	{
		*pWorstJalons = *pJalons ;
    return ;
	}

	bool        bNeedSort = false ;

	bool        bContinue ;     // Faut-il traiter les anciens jalons au delà
                              // du premier qui recoupe le nouveau ?
	NVLdVTemps  tpsContinue ;   // Moment de départ de l'intervalle à prolonger

	GoalInfoIter iOld ;
	GoalInfoIter iNew = pJalons->begin() ;
	//
	// Pour chaque jalon, on regarde son influence pour aWorstJalons
	//
	for ( ; pJalons->end() != iNew ; iNew++)
	{
  	// On cherche le premier worstJalon qui inclut celui ci
    //
    for (iOld = pWorstJalons->begin() ;
                        (pWorstJalons->end() != iOld) &&
                        ((*iOld)->tpsClosed <= (*iNew)->tpsInfo) ; iOld++) ;
    //
    // S'il n'y en a pas, c'est que ce jalon est au delà du dernier worstJalon
    //
    if (pWorstJalons->end() == iOld)
    	break ;

    // Le pire est déjà sans limites !
    // Worst is already unlimited !
    if (((*iOld)->iLevel == NSLdvGoalInfo::AProuge) &&
        ((*iOld)->tpsClosed.estNoLimit()))
    	break ;

		bContinue = false ;
    tpsContinue.init() ;

    // Cas 1 : le nouveau est inclu dans le worst
    if (((*iOld)->tpsInfo <= (*iNew)->tpsInfo) &&
        ((*iOld)->tpsClosed >= (*iNew)->tpsClosed))
    {
    	// Le nouveau est pire que l'ancien
      if ((*iNew)->iLevel > (*iOld)->iLevel)
      {
      	// recouvrement parfait : le nouveau prend la place de l'ancien
        if (((*iOld)->tpsInfo == (*iNew)->tpsInfo) &&
            ((*iOld)->tpsClosed == (*iNew)->tpsClosed))
        	**iOld = **iNew ;
        // l'ancien commence plus tôt
        else if ((*iOld)->tpsInfo < (*iNew)->tpsInfo)
        {
        	// S'ils finissent en même temps : le nouveau tronque
          if ((*iOld)->tpsClosed == (*iNew)->tpsClosed)
          {
          	(*iOld)->tpsClosed = (*iNew)->tpsInfo ;
            pWorstJalons->push_back(new NSLdvGoalInfo(**iNew)) ;
            bNeedSort = true ;
          }
          // si le nouveau est inclu, alors il s'insère (-> 3 jalons)
          else
          {
          	NSLdvGoalInfo* pThird = new NSLdvGoalInfo(**iOld) ;
            pThird->tpsInfo = (*iNew)->tpsClosed ;
            (*iOld)->tpsClosed = (*iNew)->tpsInfo ;
            pWorstJalons->push_back(new NSLdvGoalInfo(**iNew)) ;
            pWorstJalons->push_back(pThird) ;
            bNeedSort = true ;
          }
        }
        // Les deux commencent en même temps, le nouveau est plus court
        else
        {
        	(*iOld)->tpsInfo = (*iNew)->tpsClosed ;
          pWorstJalons->push_back(new NSLdvGoalInfo(**iNew)) ;
          bNeedSort = true ;
        }
      }
    }
    //
    // Cas 2 : le nouveau précède le premier ancien
    //
    else if ((*iOld)->tpsInfo > (*iNew)->tpsInfo)
    {
    	//
      // Cas trivial : pas de recoupement
      // Trivial case : no crossing
      //
      if ((*iOld)->tpsInfo >= (*iNew)->tpsClosed)
      {
      	// On ajoute le nouveau et on sort
        pWorstJalons->push_back(new NSLdvGoalInfo(**iNew)) ;
        bNeedSort = true ;
      }
      //
      // Cas avec recoupement
      // Crossing exists
      //
      else
      {
      	//
        // si le nouveau est pire que l'ancien, il le mange ou le tronque
        //
        if ((*iNew)->iLevel > (*iOld)->iLevel)
        {
        	// si le nouveau dure plus longtemps : l'ancien disparait
          if ((*iOld)->tpsClosed <= (*iNew)->tpsClosed)
          {
          	if ((*iOld)->tpsClosed < (*iNew)->tpsClosed)
            {
            	bContinue   = true ;
              tpsContinue = (*iNew)->tpsInfo ;
            }
            delete *iOld ;
            pWorstJalons->erase(iOld) ;
          }
          // sinon, l'ancien est tronqué
          else
          {
          	(*iOld)->tpsInfo = (*iNew)->tpsClosed ;
            // On ajoute le nouveau et on sort
            pWorstJalons->push_back(new NSLdvGoalInfo(**iNew)) ;
            bNeedSort = true ;
          }
        }
        //
        // sinon, c'est le nouveau qui est raccourci
        //
        else
        {
        	NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
          pJalonNew->tpsClosed = (*iOld)->tpsInfo ;
          pWorstJalons->push_back(pJalonNew) ;
          bNeedSort = true ;
          if ((*iOld)->tpsClosed < (*iNew)->tpsClosed)
          	bContinue = true ;
        }
      }
    }
    //
    // Cas 3 : le nouveau est à cheval sur plusieurs anciens, ou dure plus longtemps
    //
    else
    {
    	NSLdvGoalInfo newJalon(**iNew) ;

      //
      // On traite le jalon de début
      //
      //
      // si le nouveau est pire que l'ancien, il le mange ou le tronque
      //
      if ((*iNew)->iLevel > (*iOld)->iLevel)
      {
      	bContinue   = true ;
        tpsContinue = (*iNew)->tpsInfo ; // c'est le nouveau segment qui fixe sa date de début

        // si les deux commencent en même temps : l'ancien disparait
        if ((*iOld)->tpsInfo == (*iNew)->tpsInfo)
        {
        	delete *iOld ;
          pWorstJalons->erase(iOld) ;
        }
        // sinon, l'ancien est tronqué
        else
        {
        	(*iOld)->tpsClosed = (*iNew)->tpsInfo ;
          iOld++ ;
        }
      }
      //
      // sinon, c'est le nouveau qui est raccourci
      //
      else
      	iOld++ ;

      // S'il n'existe plus d'ancien jalon au delà...
      //
      /*
      if (iOld == pWorstJalons->end())
      {
          pWorstJalons->push_back(new NSLdvGoalInfo(newJalon)) ;
          iNew++ ;
          break;
      } */

    }
    //
    // Le nouveau jalon déborde sur plusieurs anciens jalons, on doit
    // les traiter
    // The new jalon is crossing several existing ones, we must treat them
    //
    if (bContinue)
    {
    	//
      // On traite les éventuels jalons intermédiaires
      // Jalons fully contained in the new jalon
      //
      for ( ; (iOld != pWorstJalons->end()) &&
                                ((*iOld)->tpsClosed < (*iNew)->tpsClosed) ; )
      {
      	// si le nouveau est pire que l'ancien, il le supprime
        // if the new one is worse, it deletes the previous one
        if ((*iNew)->iLevel > (*iOld)->iLevel)
        {
        	if (tpsContinue.estVide())
          	tpsContinue = (*iOld)->tpsInfo ;
          delete *iOld ;
          pWorstJalons->erase(iOld) ;
        }
        // sinon, on ne prend en compte que la partie plus ancienne de
        // newJalon, si elle existe
        else
        {
        	if (!(tpsContinue.estVide()))
          {
          	NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
            pJalonNew->tpsInfo   = tpsContinue ;
            pJalonNew->tpsClosed = (*iOld)->tpsInfo ;
            pWorstJalons->push_back(pJalonNew) ;
            bNeedSort = true ;
          }
          iOld++ ;
        }
      }
      //
      // Le nouveau jalon est au moins aussi long que le plus long des anciens
      //
      if (iOld == pWorstJalons->end())
      {
      	if (!(tpsContinue.estVide()))
        {
        	NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
          pJalonNew->tpsInfo      = tpsContinue ;
          pWorstJalons->push_back(pJalonNew) ;
          bNeedSort = true ;
        }
      }
      //
      // On traite le dernier jalon
      //
      else
      {
      	// si le nouveau est pire que l'ancien, il le mange ou le tronque
        //
        if ((*iNew)->iLevel > (*iOld)->iLevel)
        {
        	if (tpsContinue.estVide())
          	tpsContinue = (*iOld)->tpsInfo ;

        	// si les deux finissent en même temps : l'ancien disparait
          if ((*iOld)->tpsClosed == (*iNew)->tpsClosed)
          {
          	delete *iOld ;
            pWorstJalons->erase(iOld) ;
        	}
          // sinon, l'ancien est tronqué
          else
          	(*iOld)->tpsInfo = (*iNew)->tpsClosed ;

        	NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
          pJalonNew->tpsInfo = tpsContinue ;
          pWorstJalons->push_back(pJalonNew) ;
          bNeedSort = true ;
        }
        //
        // sinon, c'est le nouveau qui est raccourci
        //
        else
        {
        	if (!(tpsContinue.estVide()))
          {
          	NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
            pJalonNew->tpsInfo      = tpsContinue ;
            pJalonNew->tpsClosed    = (*iOld)->tpsInfo ;
            pWorstJalons->push_back(pJalonNew) ;
            bNeedSort = true ;
          }
        }
      }
    }

  	//
    // On re-trie par ordre chronologique worstJalons
    //
    if (bNeedSort)
    {
    	sort(pWorstJalons->begin(), pWorstJalons->end(), infGoalInfo) ;
      bNeedSort = false ;
    }
  }
  //
  // Tous les jalons restants sont ajoutés tels quels
  // All remaining steps are added "as is"
  //
  if ((iNew != pJalons->end()) && (iOld == pWorstJalons->end()))
  	for ( ; iNew != pJalons->end(); iNew++)
    	pWorstJalons->push_back(new NSLdvGoalInfo(**iNew)) ;

	//
	// On re-trie par ordre chronologique worstJalons
	//
	if (bNeedSort)
		sort(pWorstJalons->begin(), pWorstJalons->end(), infGoalInfo) ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::goalAdded.", standardError, 0) ;
}
}

void
NSLdvDocumentBase::goalModifiedForDrug(NSLdvGoal* pGoal, NSLdvDrug *pDrug)
{
	if (((NSLdvGoal*) NULL == pGoal) || ((NSLdvDrug*) NULL == pDrug))
		return ;

  // It is no longer allowed to add a goal to a treatment

/*
  if (NULL == pDrug->_pWorstJalons)
		return ;

	//
	// On reconstruit entièrement worstJalons
	// worstJalons is completely rebuilt
	//
	pDrug->_pWorstJalons->vider() ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
      if (pGoals && (false == pGoals->empty()))
      {
	      // On passe en revue tous les objectifs
	      //
	      for (ArrayGoalIter goalIt = pGoals->begin();
                                        pGoals->end() != goalIt ; goalIt++)
		      if ((*goalIt)->getConcern() == pDrug->getNoeud())
    	      pDrug->goalAdded(*goalIt) ;
      }
    }
  }
*/
}

void
NSLdvDocumentBase::goalModifiedForConcern(NSLdvGoal* pGoal, NSConcern *pConcern)
{
	if (((NSLdvGoal*) NULL == pGoal) || ((NSConcern*) NULL == pConcern))
		return ;

	//
	// On reconstruit entièrement worstJalons
	// worstJalons is completely rebuilt
	//
	pConcern->_aWorstJalons.vider() ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
      if (pGoals && (false == pGoals->empty()))
      {
	      // On passe en revue tous les objectifs
	      //
	      for (ArrayGoalIter goalIt = pGoals->begin() ;
                                        pGoals->end() != goalIt ; goalIt++)
		      if ((*goalIt)->getConcern() == pConcern->_sReference)
    	      pConcern->goalAdded(*goalIt) ;
      }
    }
  }
}

void
NSLdvDocumentBase::traiteSelectedObjectifs(string sConcern)
{
/*
	ArrayGoals* pLdvGoals = getGoals() ;
  if (pLdvGoals->empty())
		return ;
  ArrayGoals GoalsProcess(this) ;  bool initLogMed = true ;  for (ArrayGoalIter iter = pLdvGoals->begin(); iter!= pLdvGoals->end(); iter++)  {
  	if ((*iter)->isASelectedObj())
    {
    	if ((*iter)->getGoalType() == NSLdvGoal::medicament)
      {
        transferInDrugView(sConcern, (*iter), initLogMed) ;
        (*iter)->unselectObjectif() ;
        initLogMed = false ;
      }
      else
      {
         	//TO DO for biology, traitement and exam
      	if ((*iter)->getGoalType() == NSLdvGoal::biology)
        {
        	GoalsProcess.push_back(new NSLdvGoal(**iter)) ;
          (*iter)->unselectObjectif() ;
        }
      }

    }
  }
  transferInProccess(sConcern, &GoalsProcess) ;
*/
}

//
// le medicament est juste passé a drug view et la view s'ocupe a faire l'ordonnance et les liens
// avec les objectifs
//
void
NSLdvDocumentBase::transferInDrugView(string /*sConcern*/, NSLdvGoal */*pGoal*/, bool /*initLogMed*/)
{
/*
	if (!pGoal)
  	return ;

	NSDrugView* pDrugView = getDrugView(sConcern) ;
  if (pDrugView == NULL)
  {
    NSDocViewManager dvManager(pContexte) ;
		dvManager.createView(this, "Drug Management") ;
    pDrugView = getDrugView(sConcern) ;
  }

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  int colonne = 0 ;

  PPT.ajoutePatho(pGoal->sLexique, colonne) ;
  colonne++ ;

  if (pGoal->sComplementText != "")
  	PPT.ajoutePatho(pGoal->sComplementText, colonne) ;

  //ajout la date d'ouverture du medicament
  PPT.ajoutePatho("KOUVR1", 1) ;

  Message CodeMsg ;
  CodeMsg.SetUnit("2DA021") ;
  NVLdVTemps ldvTemp ;
  ldvTemp.takeTime() ;
  string sDateOuverture = ldvTemp.donneDateHeure() ;
  CodeMsg.SetComplement(sDateOuverture) ;
  colonne++ ;
  PPT.ajoutePatho("£T0;19", &CodeMsg, colonne) ;

  //sauvagarde des données avant que les objectifs soit resetés in CreerTraitement
  string sTitle = pGoal->sTitre ;
  string sCompl = pGoal->sComplementText ;

  pContexte->getSuperviseur()->pBufCopie->vider() ;
  NSCutPaste CP(pContexte->getSuperviseur()) ;
  CP.pPatPatho = new NSPatPathoArray(PPT) ;
  if(!pContexte->getSuperviseur()->pBufCopie)
   	return ;

  *(pContexte->getSuperviseur()->pBufCopie) = CP ;
	pDrugView->autoAddInDrugView(sConcern) ;

  if (pContexte->getSuperviseur()->getDPIO() && pContexte->getSuperviseur()->getDPIO()->bLogPage)
  {
    if (initLogMed)
    {
			pContexte->getSuperviseur()->getDPIO()->addToLogPage(string(""), string("<br>\r\n")) ;
    	pContexte->getSuperviseur()->getDPIO()->addToLogPage(string("Prescription médicamenteuse :"), string("<b>"), string("</b><br>\r\n")) ;
    }
		pContexte->getSuperviseur()->getDPIO()->addToLogPage(sTitle, string("<li>")) ;
		if (sCompl != "")
			pContexte->getSuperviseur()->getDPIO()->addToLogPage(string(1, ' ') + sCompl, string("<i>"), string("</i>")) ;
		pContexte->getSuperviseur()->getDPIO()->addToLogPage(string(""), string("</li>\r\n")) ;
		pContexte->getSuperviseur()->getDPIO()->AppendLogFile();
	}
*/
}

void
NSLdvDocumentBase::transferInProccess(string sConcern, ArrayGoals *pGoals)
{
/*
  if((!pGoals)||(pGoals->empty()))
  	return ;

  // Ajout d'un nouveau segment d'arbre
	// Adding a new tree branch
  //
	NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

	int colonne = 0 ;
  PatPatho.ajoutePatho("ZPRBI1", colonne) ;
  colonne++;
  bool initLogMed = true ;
  for(ArrayGoalIter pGoalIter = pGoals->begin(); pGoalIter!= pGoals->end(); pGoalIter++)
  {
    int newCol = colonne ;
  	PatPatho.ajoutePatho((*pGoalIter)->sLexique, newCol) ;
  	newCol++ ;
  	if((*pGoalIter)->sComplementText != "")
    {
    	PatPatho.ajoutePatho((*pGoalIter)->sComplementText, newCol) ;
      newCol++ ;
    }

 		PatPatho.ajoutePatho("KOUVR1", newCol) ;

  	Message CodeMsg ;
  	CodeMsg.SetUnit("2DA021") ;
  	NVLdVTemps ldvTemp;
  	ldvTemp.takeTime();
  	string sDateOuverture = ldvTemp.donneDateHeure();
  	CodeMsg.SetComplement(sDateOuverture) ;
  	newCol++;
  	PatPatho.ajoutePatho("£T0;19", &CodeMsg, newCol) ;

    if (pContexte->getSuperviseur()->getDPIO() && pContexte->getSuperviseur()->getDPIO()->bLogPage)
  	{
    	if (initLogMed)
    	{
				pContexte->getSuperviseur()->getDPIO()->addToLogPage(string(""), string("<br>\r\n")) ;
    		pContexte->getSuperviseur()->getDPIO()->addToLogPage(string("Prescription de biologie :"), string("<b>"), string("</b><br>\r\n")) ;
        initLogMed = false ;
    	}
			pContexte->getSuperviseur()->getDPIO()->addToLogPage((*pGoalIter)->sTitre, string("<li>")) ;
   		if ((*pGoalIter)->sComplementText != "")
    		pContexte->getSuperviseur()->getDPIO()->addToLogPage((*pGoalIter)->sComplementText, string("<i>"), string("</i>")) ;
   		pContexte->getSuperviseur()->getDPIO()->addToLogPage(string(""), string("</li>\r\n")) ;
      pContexte->getSuperviseur()->getDPIO()->AppendLogFile();
  	}
  }

  // enregistrement du document
  NSCSDocument CSDoc(0, pContexte) ;
  *(CSDoc.pPatPathoArray) = PatPatho;

  string sLibelle ;
  string sTxt = "ZPRBI1" ;
  pContexte->getDico()->donneLibelle( pContexte->getUserLanguage(), &sTxt, &sLibelle) ;

  bool existeInfo = CSDoc.Referencer("ZCN00", sLibelle, "", "", true, false) ;

  if (existeInfo)
  	existeInfo = CSDoc.enregistrePatPatho() ;

  if (existeInfo)
  {
  	//
    // Rafraichir l'historique
    //
    bool bReload = CSDoc.NSNoyauDocument::chargePatPatho() ;
    pContexte->getPatient()->pDocHis->Rafraichir(CSDoc.pDocInfo, CSDoc.pPatPathoArray) ;
    PatPatho = *(CSDoc.pPatPathoArray) ;
  }

  PatPathoIter iter;
  string sNodeALier;
  NSLinkManager* pLinks = pContexte->getPatient()->pGraphPerson->getLinkManager() ;
  for(ArrayGoalIter pGoalIter = pGoals->begin(); pGoalIter!= pGoals->end(); pGoalIter++)
  {
		iter = PatPatho.ChercherItem((*pGoalIter)->sLexique) ;
    if ((iter != NULL) && (iter != PatPatho.end()))
    {
    	sNodeALier = (*iter)->getNode() ;
      pLinks->etablirLien((*pGoalIter)->getNoeud(), NSRootLink::processWaitingFor, sNodeALier) ;
    }
  }

	if (existeInfo)
		existeInfo = CSDoc.enregistrePatPatho() ;

  NSProcessView* pProcView = getProcessView(sConcern) ;
  if(pProcView == NULL)
  {
  	// TO DO creer la view
  	//pDrugView = new NSDrugView(*this, sConcern) ;
    NSDocViewManager dvManager(pContexte) ;
		dvManager.createView(this, "Process Management") ;
    pProcView = getProcessView(sConcern) ;
  }
  pProcView->initCurentProcesses() ;
  pProcView->AfficheListe() ;
*/
}

void
NSLdvDocumentBase::initGoal(NSLdvGoal* pGoal)
{
	if ((NSLdvGoal*) NULL == pGoal)
		return ;

	if (string("") == pGoal->getLexique())
		return ;

	// Objectif qui ne s'ouvre jamais
	if ((string("") == pGoal->_sOpenEventNode) && (pGoal->_tDateOuverture.estVide()))
		return ;

	//
	// On regarde si les conditions d'ouverture sont réalisées
	//
	NVLdVTemps tNow ;
	tNow.takeTime() ;
	// Date de fermeture déjà atteinte : on sort
	if ((false == pGoal->_tDateFermeture.estVide()) && (pGoal->_tDateFermeture < tNow))
		return ;

	// Drug ?
  //
  if (pGoal->getGoalType() == NSLdvGoal::medicament)
  // if (('I' == pGoal->_sLexique[0]) || (pGoal->_sLexique[0] == '_'))
  {
		initGoalForDrug(pGoal) ;
    return ;
	}

	initGoalForAct(pGoal) ;
}

void
NSLdvDocumentBase::initGoalForDrug(NSLdvGoal* pGoal)
{
	if ((NULL == pGoal) || (pGoal->getNode() == string("")))
		return ;

	// if (_pDrugs->empty())
	//	return ;

  if (pGoal->getGoalType() != NSLdvGoal::medicament)
    return ;

	string sNewConcept ;
	pContexte->getDico()->donneCodeSens(&(pGoal->getLexique()), &sNewConcept) ;

/*
	if ((string("") == sNewConcept) ||
             (('I' != sNewConcept[0]) && ('_' != sNewConcept[0])))
		return ;
*/

  NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

  // Now, we are loooking for open treatments that can open or goal
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    NSLdvDrug* pOpenDrug = donneMostRecentOpenDrug(iFrame, sNewConcept) ;
    while (pOpenDrug)
	  {
  	  pGraphe->etablirLien(pOpenDrug->getNoeud(), NSRootLink::goalReseter, pGoal->getNode()) ;

      pOpenDrug = donneNextRecentOpenDrug(iFrame, sNewConcept, pOpenDrug) ;
	  }
  }

	return ;
}

void
NSLdvDocumentBase::initGoalForAct(NSLdvGoal* pGoal)
{
	if (NULL == pGoal)
		return ;

	//
	// On regarde le document le plus récent du même type
	//
	string sNewConcept ;
	pContexte->getDico()->donneCodeSens(&(pGoal->getLexique()), &sNewConcept) ;

	NVLdVTemps  tDateDoc ;
	tDateDoc.init() ;

	string sDocId = string("") ;

  if (NULL == pContexte->getPatient())
		return ;

	NSHISTODocument* pDocHis = pContexte->getPatient()->getDocHis() ;
  if (NULL == pDocHis)
		return ;

	NSPatPathoArray PatPathoDoc(pContexte->getSuperviseur()) ;
	DocumentIter itDoc = pDocHis->DonnePatPathoDocument(sNewConcept, &PatPathoDoc) ;
	if ((NULL != itDoc) && (pDocHis->getVectDocument()->end() != itDoc))
	{
  	string sDate = (*itDoc)->getDateDoc() ;
    tDateDoc.initFromDate(sDate) ;

    PatPathoIter iter = PatPathoDoc.begin() ;
    sDocId = (*iter)->getNode() ;
	}

  NVLdVTemps tNow ;
	tNow.takeTime() ;

	//
	// Si l'objectif est ponctuel, on regarde si le document est dans les
	// dates prévues
	//
	if (NSLdvGoal::cyclic != pGoal->_iRythme)
	{
		// Si il existe une date de début autorisé
    if (string("") != pGoal->_sDateDebutAutorise)
    {
    	NVLdVTemps tDateDebut ;
      tDateDebut.initFromDate(pGoal->_sDateDebutAutorise) ;
      // Si le document est antérieur à la date de début autorisé, on ne
      // doit pas en tenir compte
      if (tDateDoc < tDateDebut)
      	return ;
      // Sinon, le document ferme l'objectif
      closeGoal(pGoal, &tNow, NULL) ;
      return ;
    }
    string sDateMin = "" ;
    if (string("") != pGoal->_sDateDebutConseille)
    	sDateMin = pGoal->_sDateDebutConseille ;
    else
    	if (string("") != pGoal->_sDateDebutIdeal)
      	sDateMin = pGoal->_sDateDebutIdeal ;
      else
      	if (string("") != pGoal->_sDateDebutIdealMax)
        	sDateMin = pGoal->_sDateDebutIdealMax ;
        else
        	if (string("") != pGoal->_sDateDebutConseilMax)
          	sDateMin = pGoal->_sDateDebutConseilMax ;
          else
          	if (string("") != pGoal->_sDateDebutCritique)
            	sDateMin = pGoal->_sDateDebutCritique ;

    if (string("") == sDateMin)
    	return ;

    // Sinon, le document ferme l'objectif
    NVLdVTemps tDateDebut ;
    tDateDebut.initFromDate(sDateMin) ;
    if (tDateDoc >= tDateDebut)
    	closeGoal(pGoal, &tNow, NULL) ;
    return ;
	}

	//
	// ------------------------- Objectif cyclique ------------------------
	//

  // On cherche s'il existe un événement d'ouverture
  if (string("") != pGoal->_sOpenEventNode)
  {
    string sEventConcept ;
    pContexte->getDico()->donneCodeSens(&(pGoal->_sOpenEventNode), &sEventConcept) ;

    NSPatPathoArray PPathoDoc(pContexte->getSuperviseur()) ;
    itDoc = pDocHis->DonnePatPathoDocument(sEventConcept, &PPathoDoc) ;
    if ((NULL != itDoc) && (pDocHis->getVectDocument()->end() != itDoc))
    {
    	string sDate = (*itDoc)->getDateDoc() ;
      tDateDoc.initFromDate(sDate) ;
    }
    else
      return ;

    PatPathoIter iter = PPathoDoc.begin() ;
    string sOpener = (*iter)->getNode() ;
    NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
    pGraphe->etablirLien(sOpener, NSRootLink::goalOpener, pGoal->getNode()) ;

    NVLdVTemps tDateOpener ;
    tDateOpener.init() ;
    string sDate = (*itDoc)->getDateDoc() ;
    tDateOpener.initFromDate(sDate) ;

    // Si l'événement d'ouverture est plus récent que le document, on sort
    if (tDateOpener > tDateDoc)
    	return ;
  }

	if (string("") == sDocId)
		return ;

	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

	pGraphe->etablirLien(sDocId, NSRootLink::goalReseter, pGoal->getNode()) ;
}

void
NSLdvDocumentBase::initGoalForNumValue(NSLdvGoal* pGoal)
{
	if (NULL == pGoal)
		return ;
}

void
NSLdvDocumentBase::findCocCode(bool bVerbose, string &sCocCode, string sLexCode, string sLabel, string sClassif, string sPostCase)
{
try
{
  string sHealthConcern = sLexCode ;
  if (string("") == sHealthConcern)
    return ;

  string sConcept ;
  pContexte->getDico()->donneCodeSens(&sHealthConcern, &sConcept) ;

  string sResO = string("") ;
  string sResP = string("") ;
  string sResI = string("") ;
  string sRes3 = string("") ;

  string sDomain = string("") ;

  Classify ClassifTool(pContexte, &sClassif, &sConcept, &sPostCase) ;
  ClassifTool.computeParsing(&sResO, &sResP, &sResI) ;

  // If Classer has found 'Mandatory code(s)', then we must take it
  //
  if (string("") != sResO)
    sDomain = sResO ;

  // Else, 3BT is the gold standard, so, we have to use it
  //
  else
  {
    string sClasserDomain = sResP ;

    //
    //
    string sLocalize = sPostCase ;

    ParseSOAP Parser(pContexte, sClassif, string("")) ;
    Parser.computeParsing(&sLabel, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;

    if (string("") != sResP)
      sDomain  = sResP ;
    else
      sDomain  = sClasserDomain ;
  }

  //
  // On trouve le code
  //
  NSEpisodus* pEpisodus = pContexte->getEpisodus() ;

  classifExpert* pExpert = pEpisodus->pClassifExpert ;
  if (NULL == pExpert)
    return ;

  NSEpiClassifInfoArray arrayClassif ;

  //
  // On instancie le domaine
  // Instanciating the domain
  //
  ParseCategory Parser(pExpert->donneCodeSize(sClassif), sClassif,
                                             pExpert->donnePattern(sClassif)) ;
  ElemSetArray* pElemDomain = Parser.DefDomain(sDomain) ;
  //
  // On trouve les codes qui correspondent au domaine
  // Finding the codes that belong to the domain
  //
  string sCaseSens ;
  pContexte->getDico()->donneCodeSens(&sPostCase, &sCaseSens) ;

  pExpert->fillList(sClassif, pElemDomain, &arrayClassif, sCaseSens) ;

  if (pElemDomain)
    delete pElemDomain ;

  string sGoodCode = "" ;
  //
  //
  //
  if (arrayClassif.size() != 1)
  {
    if (bVerbose)
    {
  	  SOAPObject SOAPObj(sLabel, sResP, sClassif, 0, string("")) ;
#ifndef _EXT_CAPTURE
		  NSCapture Capture(pContexte) ;
#else
		  NSCapture Capture ;
#endif

		  Capture.setClassifResultO(sResO) ;
      Capture.setClassifResultP(sResP) ;
      Capture.setClassifResultI(sResI) ;
      Capture.setClassifResult3(sRes3) ;

      SOAPObj.pCaptElemnt = &Capture ;
      SOAPObj.sCase       = sCaseSens ;

      sGoodCode = pExpert->chooseCode(&SOAPObj) ;
    }
  }
  else
  {
  	sGoodCode = (*(arrayClassif.begin()))->getCode() ;

    string sCtrlData = "" ;
    NVLdVTemps tpsDebut ;
    tpsDebut.takeTime() ;
    pExpert->setControlString(&sCtrlData, sClassif, sCaseSens, sLabel,
                                  classifExpert::niveauPreselection, 0,
                                  &tpsDebut, &tpsDebut, sGoodCode, sConcept) ;
    pExpert->storeControlData(sCtrlData) ;
  }

  if (string("") == sGoodCode)
  {
    sCocCode = string("") ;
  	return ;
  }

  sCocCode = sGoodCode ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSLdvDocumentBase::findCocCode : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSLdvDocumentBase::findCocCode.",  standardError, 0) ;
}
}

//-----------------------------------------------------------------------//
//         						Classe NSFrameInformation                          //
//-----------------------------------------------------------------------//

NSFrameInformation::NSFrameInformation(NSContexte *pCtx, NSLdvDocumentBase *pDoc, LDVFRAME iFrame, NSFrameDocuments *pFrameDocs)
                   :NSRoot(pCtx)
{
  _iFrame = iFrame ;
  _pDoc   = pDoc ;

  _pFrameDocs = pFrameDocs ;
  // loadSubTrees() ;

  _pConcerns   = (ArrayConcern*)  0 ;
  _pGoals      = (ArrayGoals*)    0 ;
  _pTreatments = (ArrayLdvDrugs*) 0 ;

  lObjectCount++ ;
}

NSFrameInformation::NSFrameInformation(const NSFrameInformation& rv)
                   :NSRoot(rv.pContexte)
{
  _iFrame = rv._iFrame ;
  _pDoc   = rv._pDoc ;

  _pFrameDocs = rv._pFrameDocs ;

  if (rv._pConcerns)
    _pConcerns = new ArrayConcern(*(rv._pConcerns)) ;
  else
    _pConcerns = (ArrayConcern*) 0 ;

  if (rv._pGoals)
    _pGoals = new ArrayGoals(*(rv._pGoals)) ;
  else
    _pGoals = (ArrayGoals*) 0 ;

  if (rv._pTreatments)
    _pTreatments = new ArrayLdvDrugs(*(rv._pTreatments)) ;
  else
    _pTreatments = (ArrayLdvDrugs*) 0 ;

  lObjectCount++ ;
}

NSFrameInformation::~NSFrameInformation()
{
  if (_pConcerns)
    delete _pConcerns ;
  if (_pGoals)
    delete _pGoals ;
  if (_pTreatments)
    delete _pTreatments ;

  lObjectCount-- ;
}

void
NSFrameInformation::initConcerns()
{
  if ((ArrayConcern*) NULL == _pConcerns)
    _pConcerns = new ArrayConcern(this) ;
	_pConcerns->init(_iFrame) ;
}

void
NSFrameInformation::initTreatments()
{
  if ((ArrayLdvDrugs*) NULL == _pTreatments)
    _pTreatments = new ArrayLdvDrugs(this) ;
	_pTreatments->initialiser() ;
}

void
NSFrameInformation::initGoals()
{
  if ((ArrayGoals*) NULL == _pGoals)
    _pGoals = new ArrayGoals(this) ;
	_pGoals->initialiser() ;
}

NSConcern*
NSFrameInformation::getConcern(string sNode)
{
  if ((NULL == _pConcerns) || (string("") == sNode))
    return (NSConcern*) 0 ;

  return (_pConcerns->getConcern(sNode)) ;
}

NSLdvDrug*
NSFrameInformation::getTreatment(string sNode)
{
  if ((NULL == _pTreatments) || (string("") == sNode))
    return (NSLdvDrug*) 0 ;

  return (_pTreatments->getDrug(sNode)) ;
}

NSLdvGoal*
NSFrameInformation::getGoal(string sNode)
{
  if ((NULL == _pGoals) || (string("") == sNode))
    return (NSLdvGoal*) 0 ;

  return (_pGoals->getGoal(sNode)) ;
}

NSFrameInformation&
NSFrameInformation::operator=(const NSFrameInformation& src)
{
try
{
	if (this == &src)
		return *this ;

  _iFrame = src._iFrame ;
  _pDoc   = src._pDoc ;

  _pFrameDocs = src._pFrameDocs ;

	if (src._pConcerns)
	{
		if (_pConcerns)
			*_pConcerns = *(src._pConcerns) ;
		else
			_pConcerns = new ArrayConcern(*(src._pConcerns)) ;
	}
  else
  {
    if (_pConcerns)
			delete _pConcerns ;
    _pConcerns = (ArrayConcern*) 0 ;
  }

	if (src._pGoals)
	{
		if (_pGoals)
			*_pGoals = *(src._pGoals) ;
		else
			_pGoals = new ArrayGoals(*(src._pGoals)) ;
	}
  else
  {
    if (_pGoals)
      delete _pGoals ;
    _pGoals = (ArrayGoals*) 0 ;
  }

  if (src._pTreatments)
	{
		if (_pTreatments)
			*_pTreatments = *(src._pTreatments) ;
		else
			_pTreatments = new ArrayLdvDrugs(*(src._pTreatments)) ;
	}
  else
  {
    if (_pTreatments)
      delete _pTreatments ;
    _pTreatments = (ArrayLdvDrugs*) 0 ;
  }

	return *this ;
}
catch (...)
{
	erreur("Exception NSFrameInformation::operator=", standardError, 0) ;
	return *this ;
}
}

//-----------------------------------------------------------------------//
//        					Classe NSFrameInformationArray                       //
//-----------------------------------------------------------------------//

NSFrameInformationArray::NSFrameInformationArray(NSLdvDocumentBase *pDoc)
                        :NSVectFrameInformation()
{
  _pDoc = pDoc ;
}

NSFrameInformationArray::NSFrameInformationArray(const NSFrameInformationArray& rv)
                        :NSVectFrameInformation()
{
try
{
  _pDoc = rv._pDoc ;

  if (false == rv.empty())
    for (FrameInfoConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSFrameInformation(*(*i))) ;
}
catch (...)
{
  erreur("Exception ArrayPbModifier.", standardError, 0) ;
}
}

NSFrameInformationArray::~NSFrameInformationArray()
{
  vider() ;
}

void
NSFrameInformationArray::vider()
{
  if (empty())
    return ;

  for (FrameInfoIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

void
NSFrameInformationArray::openFrameInformation(LDVFRAME iFrame)
{
  // If already opened, nothing to be done
  //
  NSFrameInformation* pFrame = getFrameInformation(iFrame) ;
  if (pFrame)
    return ;

  if (NULL == _pDoc->pContexte->getPatient())
    return ;

  NSFrameDocumentsArray* pFramesDocs = _pDoc->pContexte->getPatient()->getFramesDocuments() ;
  if (NULL == pFramesDocs)
    return ;

  NSFrameDocuments* pFrameDocuments = pFramesDocs->getFrameDocuments(iFrame) ;
  if (NULL == pFrameDocuments)
    return ;

  push_back(new NSFrameInformation(_pDoc->pContexte, _pDoc, iFrame, pFrameDocuments)) ;
}

void
NSFrameInformationArray::initConcernsForFrameInformation(LDVFRAME iFrame)
{
  NSFrameInformation* pFrame = getFrameInformation(iFrame) ;
  if (NULL == pFrame)
    return ;

  pFrame->initConcerns() ;
}

void
NSFrameInformationArray::initTreatmentsForFrameInformation(LDVFRAME iFrame)
{
  NSFrameInformation* pFrame = getFrameInformation(iFrame) ;
  if (NULL == pFrame)
    return ;

  pFrame->initTreatments() ;
}

void
NSFrameInformationArray::initGoalsForFrameInformation(LDVFRAME iFrame)
{
  NSFrameInformation* pFrame = getFrameInformation(iFrame) ;
  if (NULL == pFrame)
    return ;

  pFrame->initGoals() ;
}

/*
NSFrameInformation*
NSFrameInformationArray::getFrameInformation(LDVFRAME iFrame)
{
  if (empty())
    return 0 ;

  for (FrameInfoIter i = begin() ; end() != i ; i++)
    if ((*i)->getFrame() == iFrame)
      return *i ;

  return 0 ;
}
*/

NSFrameInformationArray&
NSFrameInformationArray::operator=(const NSFrameInformationArray& src)
{
  if (this == &src)
		return *this ;

try
{
  _pDoc = src._pDoc ;

	vider() ;

	if (false == src.empty())
		for (FrameInfoConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSFrameInformation(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSFrameInformationArray= .", standardError, 0) ;
	return *this ;
}
}

//-----------------------------------------------------------------------//
//         						Classe NSConcernModifier                           //
//-----------------------------------------------------------------------//

NSConcernModifier::NSConcernModifier(NSConcern* pThisConcern,
                                     NVLdVTemps tDateTimeDeb,
                                     NVLdVTemps tDateTimeFin,
                                     int        iSeverity,
                                     int        iFonction,
                                     int        iRisk)
{
	pConcern      = pThisConcern ;
	tDateHeureDeb = tDateTimeDeb ;
	tDateHeureFin = tDateTimeFin ;
	iSeverite     = iSeverity ;
	iStatFonct    = iFonction ;
	iRisque       = iRisk ;

  lObjectCount++ ;
}

NSConcernModifier::NSConcernModifier(const NSConcernModifier& rv)
{
	pConcern      = rv.pConcern ;
	tDateHeureDeb = rv.tDateHeureDeb ;
	tDateHeureFin = rv.tDateHeureFin ;
	iSeverite     = rv.iSeverite ;
	iStatFonct    = rv.iStatFonct ;
	iRisque       = rv.iRisque ;

  lObjectCount++ ;
}

NSConcernModifier::~NSConcernModifier()
{
  lObjectCount-- ;
}

NSConcernModifier&
NSConcernModifier::operator=(const NSConcernModifier& src)
{
  if (this == &src)
		return *this ;

	pConcern      = src.pConcern ;
	tDateHeureDeb = src.tDateHeureDeb ;
	tDateHeureFin = src.tDateHeureFin ;
	iSeverite     = src.iSeverite ;
	iStatFonct    = src.iStatFonct ;
	iRisque       = src.iRisque ;

  return *this ;
}


//-----------------------------------------------------------------------//
//				   		Classe ArrayPbModifier                           //
//-----------------------------------------------------------------------//

void
ArrayPbModifier::addModifier(NSConcernModifier* pModifier)
{
try
{
	// Recherche des modificateurs qui "encadrent" le nouveau
  // Looking for the modifiers that surround the new one
	NSConcernModifier* pModifierBefore = (NSConcernModifier*) 0 ;
	NSConcernModifier* pModifierAfter  = (NSConcernModifier*) 0 ;

	if (false == empty())
	{
		for (ArrayModifierIter i = begin() ; end() != i ; i++)
		{
			// Before
			if ((*i)->tDateHeureDeb < pModifier->tDateHeureDeb)
			{
				// The latest of the Modif before
				if ((NULL == pModifierBefore) ||
						(pModifierBefore->tDateHeureDeb < (*i)->tDateHeureDeb))
					pModifierBefore = *i;
			}
			// After
			else
			{
				// The earliest of the Modif after
				if ((NULL == pModifierAfter) ||
						(pModifierAfter->tDateHeureDeb > (*i)->tDateHeureDeb))
					pModifierAfter = *i ;
			}
		}
	}

	if (pModifierBefore)
		pModifierBefore->tDateHeureFin = pModifier->tDateHeureDeb ;

	if (pModifierAfter)
		pModifier->tDateHeureFin = pModifierAfter->tDateHeureDeb ;
	else
	{
		if (pModifier->pConcern->_tDateFermeture.estNoLimit())
			pModifier->tDateHeureFin.setNoLimit() ;
		else
			pModifier->tDateHeureFin = pModifier->pConcern->_tDateFermeture ;
	}

	push_back(new NSConcernModifier(*pModifier)) ;
	return ;
}
catch (...)
{
  erreur("Exception ArrayPbModifier::addModifier.", standardError, 0) ;
}
}

ArrayPbModifier::ArrayPbModifier(const ArrayPbModifier& rv)
                :ArrayModifier()
{
try
{
  if (false == rv.empty())
    for (ArrayModifierConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSConcernModifier(*(*i))) ;
}
catch (...)
{
  erreur("Exception ArrayPbModifier.", standardError, 0) ;
}
}

void
ArrayPbModifier::vider()
{
	if (empty())
		return ;

	for (ArrayModifierIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

ArrayPbModifier::~ArrayPbModifier()
{
	vider() ;
}

ArrayPbModifier&
ArrayPbModifier::operator=(const ArrayPbModifier& src)
{
  if (this == &src)
		return *this ;

try
{
	vider() ;

	if (false == src.empty())
		for (ArrayModifierConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSConcernModifier(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayPbModifier= .", standardError, 0) ;
	return *this;
}
}

//-----------------------------------------------------------------------//
//						    Classe NSConcern                             //
//-----------------------------------------------------------------------//

NSConcern::NSConcern(NSContexte* pCtx, NSLdvDocumentBase* pDocum, LDVFRAME iFrame)
          :NSRoot(pCtx)
{
try
{
	_pDoc           = pDocum ;
  _iFrame         = iFrame ;

	_sTitre         = string("") ;
	_sSignificatif  = string("") ;
	_sSeverite      = string("") ;
	_tDateOuverture.init();
	_tDateFermeture.setNoLimit();
	_tDateDebut.init();
	_tDateAutoClose.init();

	_sReference     = string("") ;
  _sLexique       = string("") ;

	_sHealthIssue   = string("") ;
	_sPrimoPb       = string("") ;
  _sCocCode       = string("") ;
  _sCimCode       = string("") ;

	_pPptDetails    = (NSPatPathoArray*) 0 ;

  lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSConcern ctor.", standardError, 0) ;
}
}

NSConcern::NSConcern(const NSConcern& rv)
          :NSRoot(rv.pContexte)
{
try
{
	_pDoc            = rv._pDoc ;
  _iFrame          = rv._iFrame ;

	_sReference      = rv._sReference ;
  _sLexique        = rv._sLexique ;
	_sHealthIssue    = rv._sHealthIssue ;
	_sPrimoPb        = rv._sPrimoPb ;
  _sCocCode        = rv._sCocCode ;
  _sCimCode        = rv._sCimCode ;

	_sTitre          = rv._sTitre ;
	_sSignificatif   = rv._sSignificatif ;
	_sSeverite       = rv._sSeverite ;
	_tDateOuverture  = rv._tDateOuverture ;
	_tDateFermeture  = rv._tDateFermeture ;
	_tDateDebut      = rv._tDateDebut ;
	_tDateAutoClose  = rv._tDateAutoClose ;

	_aModificateurs  = rv._aModificateurs ;

	_aWorstJalons    = rv._aWorstJalons ;

	if (rv._pPptDetails)
		_pPptDetails = new NSPatPathoArray(*(rv._pPptDetails)) ;
	else
		_pPptDetails = (NSPatPathoArray*) 0 ;

  lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSConcern copy ctor.", standardError, 0) ;
}
}

NSConcern::~NSConcern()
{
	if (_pPptDetails)
		delete _pPptDetails ;

  lObjectCount-- ;
}

void
NSConcern::goalAdded(NSLdvGoal* pGoal)
{
try
{
  if (NULL == pGoal)
    return ;

  GoalInfoArray* pJalons = pGoal->getMetaJalons() ;

  if ((NULL == pJalons) || (pJalons->empty()))
    return ;

  if (_aWorstJalons.empty())
  {
    _aWorstJalons = *pJalons ;
    return ;
  }

  bool        bNeedSort = false ;

  bool        bContinue ;     // Faut-il traiter les anciens jalons au delà
                              // du premier qui recoupe le nouveau ?
  NVLdVTemps  tpsContinue ;   // Moment de départ de l'intervalle à prolonger

  GoalInfoIter iOld ;
  GoalInfoIter iNew = pJalons->begin() ;
  //
  // Pour chaque jalon, on regarde son influence pour aWorstJalons
  //
  for ( ; pJalons->end() != iNew ; iNew++)
  {
    // On cherche le premier worstJalon qui inclut celui ci
    //
    for (iOld = _aWorstJalons.begin() ;
                (_aWorstJalons.end() != iOld) &&
                      ((*iOld)->tpsClosed <= (*iNew)->tpsInfo); iOld++) ;
    //
    // S'il n'y en a pas, c'est que ce jalon est au delà du dernier worstJalon
    //
    if (_aWorstJalons.end() == iOld)
      break ;

    // Le pire est déjà sans limites !
    // Worst is already unlimited !
    if (((*iOld)->iLevel == NSLdvGoalInfo::AProuge) &&
          ((*iOld)->tpsClosed.estNoLimit()))
      break ;

    bContinue = false ;
    tpsContinue.init() ;

    // Cas 1 : le nouveau est inclu dans le worst
    if (((*iOld)->tpsInfo <= (*iNew)->tpsInfo) &&
          ((*iOld)->tpsClosed >= (*iNew)->tpsClosed))
    {
      // Le nouveau est pire que l'ancien
      if ((*iNew)->iLevel > (*iOld)->iLevel)
      {
        // recouvrement parfait : le nouveau prend la place de l'ancien
        if (((*iOld)->tpsInfo == (*iNew)->tpsInfo) &&
                  ((*iOld)->tpsClosed == (*iNew)->tpsClosed))
          **iOld = **iNew ;
        // l'ancien commence plus tôt
        else if ((*iOld)->tpsInfo < (*iNew)->tpsInfo)
        {
          // S'ils finissent en même temps : le nouveau tronque
          if ((*iOld)->tpsClosed == (*iNew)->tpsClosed)
          {
            (*iOld)->tpsClosed = (*iNew)->tpsInfo ;
            _aWorstJalons.push_back(new NSLdvGoalInfo(**iNew)) ;
            bNeedSort = true ;
          }
          // si le nouveau est inclu, alors il s'insère (-> 3 jalons)
          else
          {
            NSLdvGoalInfo* pThird = new NSLdvGoalInfo(**iOld) ;
            pThird->tpsInfo = (*iNew)->tpsClosed ;
            (*iOld)->tpsClosed = (*iNew)->tpsInfo;
            _aWorstJalons.push_back(new NSLdvGoalInfo(**iNew)) ;
            _aWorstJalons.push_back(pThird) ;
            bNeedSort = true ;
          }
        }
        // Les deux commencent en même temps, le nouveau est plus court
        else
        {
          (*iOld)->tpsInfo = (*iNew)->tpsClosed;
          _aWorstJalons.push_back(new NSLdvGoalInfo(**iNew)) ;
          bNeedSort = true ;
        }
      }
    }
    //
    // Cas 2 : le nouveau précède le premier ancien
    //
    else if ((*iOld)->tpsInfo > (*iNew)->tpsInfo)
    {
      //
      // Cas trivial : pas de recoupement
      // Trivial case : no crossing
      //
      if ((*iOld)->tpsInfo >= (*iNew)->tpsClosed)
      {
        // On ajoute le nouveau et on sort
        _aWorstJalons.push_back(new NSLdvGoalInfo(**iNew)) ;
        bNeedSort = true ;
      }
      //
      // Cas avec recoupement
      // Crossing exists
      //
      else
      {
        //
        // si le nouveau est pire que l'ancien, il le mange ou le tronque
        //
        if ((*iNew)->iLevel > (*iOld)->iLevel)
        {
          // si le nouveau dure plus longtemps : l'ancien disparait
          if ((*iOld)->tpsClosed <= (*iNew)->tpsClosed)
          {
            if ((*iOld)->tpsClosed < (*iNew)->tpsClosed)
            {
              bContinue   = true ;
              tpsContinue = (*iNew)->tpsInfo ;
            }
            delete *iOld ;
            _aWorstJalons.erase(iOld) ;
          }
          // sinon, l'ancien est tronqué
          else
          {
            (*iOld)->tpsInfo = (*iNew)->tpsClosed ;
            // On ajoute le nouveau et on sort
            _aWorstJalons.push_back(new NSLdvGoalInfo(**iNew)) ;
            bNeedSort = true ;
          }
        }
        //
        // sinon, c'est le nouveau qui est raccourci
        //
        else
        {
          NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
          pJalonNew->tpsClosed = (*iOld)->tpsInfo ;
          _aWorstJalons.push_back(pJalonNew) ;
          bNeedSort = true ;
          if ((*iOld)->tpsClosed < (*iNew)->tpsClosed)
            bContinue = true ;
        }
      }
    }
    //
    // Cas 3 : le nouveau est à cheval sur plusieurs anciens, ou dure plus longtemps
    //
    else
    {
      NSLdvGoalInfo newJalon(**iNew) ;

      //
      // On traite le jalon de début
      //
      //
      // si le nouveau est pire que l'ancien, il le mange ou le tronque
      //
      if ((*iNew)->iLevel > (*iOld)->iLevel)
      {
        bContinue   = true ;
        tpsContinue = (*iNew)->tpsInfo ; // c'est le nouveau segment qui fixe sa date de début

        // si les deux commencent en même temps : l'ancien disparait
        if ((*iOld)->tpsInfo == (*iNew)->tpsInfo)
        {
          delete *iOld ;
          _aWorstJalons.erase(iOld) ;
        }
        // sinon, l'ancien est tronqué
        else
        {
          (*iOld)->tpsClosed = (*iNew)->tpsInfo ;
          iOld++ ;
        }
      }
      //
      // sinon, c'est le nouveau qui est raccourci
      //
      else
        iOld++ ;

      // S'il n'existe plus d'ancien jalon au delà...
      //
      /*
          if (iOld == pWorstJalons->end())
          {
              pWorstJalons->push_back(new NSLdvGoalInfo(newJalon)) ;
              iNew++ ;
              break;
      } */

    }
    //
    // Le nouveau jalon déborde sur plusieurs anciens jalons, on doit
    // les traiter
    // The new jalon is crossing several existing ones, we must treat them
    //
    if (bContinue)
    {
      //
      // On traite les éventuels jalons intermédiaires
      // Jalons fully contained in the new jalon
      //
      for ( ; (_aWorstJalons.end() != iOld) &&
                              ((*iOld)->tpsClosed < (*iNew)->tpsClosed) ; )
      {
        // si le nouveau est pire que l'ancien, il le supprime
        // if the new one is worse, it deletes the previous one
        if ((*iNew)->iLevel > (*iOld)->iLevel)
        {
          if (tpsContinue.estVide())
            tpsContinue = (*iOld)->tpsInfo ;
          delete *iOld ;
          _aWorstJalons.erase(iOld) ;
        }
        // sinon, on ne prend en compte que la partie plus ancienne de
        // newJalon, si elle existe
        else
        {
          if (false == tpsContinue.estVide())
          {
            NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
            pJalonNew->tpsInfo      = tpsContinue ;
            pJalonNew->tpsClosed    = (*iOld)->tpsInfo ;
            _aWorstJalons.push_back(pJalonNew) ;
            bNeedSort = true ;
          }
          iOld++ ;
        }
      }
      //
      // Le nouveau jalon est au moins aussi long que le plus long des anciens
      //
      if (_aWorstJalons.end() == iOld)
      {
        if (false == tpsContinue.estVide())
        {
          NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
          pJalonNew->tpsInfo      = tpsContinue ;
          _aWorstJalons.push_back(pJalonNew) ;
          bNeedSort = true ;
        }
      }
      //
      // On traite le dernier jalon
      //
      else
      {
        // si le nouveau est pire que l'ancien, il le mange ou le tronque
        //
        if ((*iNew)->iLevel > (*iOld)->iLevel)
        {
          if (tpsContinue.estVide())
            tpsContinue = (*iOld)->tpsInfo ;

          // si les deux finissent en même temps : l'ancien disparait
          if ((*iOld)->tpsClosed == (*iNew)->tpsClosed)
          {
            delete *iOld ;
            _aWorstJalons.erase(iOld) ;
          }
          // sinon, l'ancien est tronqué
          else
            (*iOld)->tpsInfo = (*iNew)->tpsClosed ;

          NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
          pJalonNew->tpsInfo = tpsContinue ;
          _aWorstJalons.push_back(pJalonNew) ;
          bNeedSort = true ;
        }
        //
        // sinon, c'est le nouveau qui est raccourci
        //
        else
        {
          if (false == tpsContinue.estVide())
          {
            NSLdvGoalInfo* pJalonNew = new NSLdvGoalInfo(**iNew) ;
            pJalonNew->tpsInfo      = tpsContinue ;
            pJalonNew->tpsClosed    = (*iOld)->tpsInfo ;
            _aWorstJalons.push_back(pJalonNew) ;
            bNeedSort = true ;
          }
        }
      }
    }

    //
    // On re-trie par ordre chronologique worstJalons
    //
    if (bNeedSort)
    {
      sort(_aWorstJalons.begin(), _aWorstJalons.end(), infGoalInfo) ;
      bNeedSort = false ;
    }
  }
  //
  // Tous les jalons restants sont ajoutés tels quels
  // All remaining steps are added "as is"
  //
  if ((pJalons->end() != iNew) && (_aWorstJalons.end() == iOld))
    for ( ; pJalons->end() != iNew ; iNew++)
      _aWorstJalons.push_back(new NSLdvGoalInfo(**iNew)) ;

  //
  // On re-trie par ordre chronologique worstJalons
  //
  if (bNeedSort)
    sort(_aWorstJalons.begin(), _aWorstJalons.end(), infGoalInfo) ;
}
catch (...)
{
	erreur("Exception NSConcern::goalAdded.", standardError, 0) ;
}
}

bool
NSConcern::isActiveConcern()
{
	NVLdVTemps tNow ;
	tNow.takeTime() ;

	if (_tDateOuverture <= tNow)
		if (_tDateFermeture.estNoLimit())
			return true ;
    else
    	if (_tDateFermeture >= tNow)
      	return true ;
      else
      	return false ;
	else
  	return false ;
}

int
NSConcern::getMaxSeverityIndex()
{
	if (_aModificateurs.empty())
		return -1 ;

	int iMaxSeverity = -1 ;

	ArrayModifierIter itMod = _aModificateurs.begin() ;
  for ( ; _aModificateurs.end() != itMod ; itMod++)
  	if ((*itMod)->iSeverite > iMaxSeverity)
    	iMaxSeverity = (*itMod)->iSeverite ;

	return iMaxSeverity ;
}

int
NSConcern::getMinSeverityIndex()
{
	if (_aModificateurs.empty())
		return -1 ;

	int iMinSeverity = -1 ;

	ArrayModifierIter itMod = _aModificateurs.begin() ;
  for ( ; _aModificateurs.end() != itMod ; itMod++)
  	if (((*itMod)->iSeverite < iMinSeverity) || (-1 == iMinSeverity))
    	iMinSeverity = (*itMod)->iSeverite ;

	return iMinSeverity ;
}

void
NSConcern::sendFocusEvent()
{
	sendEvent(FOCUS) ;
}

void
NSConcern::sendActivationEvent()
{
	sendEvent(ACTIVATE) ;
}

void
NSConcern::sendEvent(EventType iEvent)
{
  NSFrameInformationArray *pFrames = _pDoc->getFrames() ;
  if (((NSFrameInformationArray*) NULL == pFrames) || pFrames->empty())
    return ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      NSDocumentHisto* pDocument = pFrameInfo->findNode(_sReference) ;
      if (pDocument)
      {
        NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;
        pDocument->initFromPatPatho(&DocPatho) ;
        if (false == DocPatho.empty())
        {
          PatPathoIter pptIter = DocPatho.ChercherNoeud(_sReference) ;
          if ((pptIter) && (DocPatho.end() != pptIter))
          {
            NautilusEvent nsEvent(&DocPatho, pptIter, iEvent, false) ;
            pContexte->getBBinterface()->addNautilusEvent(&nsEvent) ;
            return ;
          }
        }
      }
    }
  }
}

void
NSConcern::initTitle(NSPatPathoInfo* pPatho)
{
  _sTitre = string("") ;

  if (NULL == pPatho)
    return ;

  _sLexique = pPatho->getLexique() ;

  // Free text
  if (string("£?????") == _sLexique)
  {
    _sTitre = pPatho->getTexteLibre() ;
    return ;
  }

  string sRootCode = getRootLabelForFrame(_iFrame) ;

  if (string("") == sRootCode)
  {
    pContexte->getDico()->donneLibelle(_pDoc->_sLangue, &_sLexique, &_sTitre) ;
    return ;
  }

  decodageBase sentenceMaker(pContexte, _pDoc->_sLangue) ;

  NSPhraseMot rootWord(pContexte) ;
  rootWord.setLexique(sRootCode) ;
  NSPhraseMot conceptWord(pContexte) ;
  conceptWord.setLexique(_sLexique) ;

  NSPhraseur concernAsComplement(pContexte) ;
  concernAsComplement.compNom.push_back(new NSPhraseMot(conceptWord)) ;

  rootWord.addComplementPhr(new NSPhraseur(concernAsComplement)) ;

  NSPhraseur sentence(pContexte) ;
  sentenceMaker.getPhraseur()->COD.push_back(new NSPhraseMot(rootWord)) ;

  sentenceMaker.getGenerateur()->genereProposition(dcTiret) ;

  _sTitre = sentenceMaker.getGenerateur()->getPropositionPhrase() ;
}

NSQualityOfLifeChart*
NSConcern::getQualityOfLifeChartAtTime(const NVLdVTemps& tTime)
{
  if (_aQualityOfLifeCharts.empty())
    return (NSQualityOfLifeChart*) 0 ;

  for (QualityOfLifeChartIter chartIt = _aQualityOfLifeCharts.begin() ; _aQualityOfLifeCharts.end() != chartIt ; chartIt++)
    if (((*chartIt)->getTpsInfo() >= tTime) && ((*chartIt)->getTpsStart() <= tTime))
      return *chartIt ;

  return (NSQualityOfLifeChart*) 0 ;
}

void
NSConcern::goalModified(NSLdvGoal* pGoal)
{
	// if (!pGoal)
	//     return ;

	//
	// On reconstruit entièrement worstJalons
	// worstJalons is completely rebuilt
	//
	_aWorstJalons.vider() ;

  NSFrameInformationArray *pFrames = _pDoc->getFrames() ;
  if ((NULL == pFrames) || pFrames->empty())
    return ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
      if (pGoals && (false == pGoals->empty()))
      {
	      // On passe en revue tous les objectifs
	      //
	      for (ArrayGoalIter goalIt = pGoals->begin() ;
                                        pGoals->end() != goalIt ; goalIt++)
		      if ((*goalIt)->getConcern() == _sReference)
    	      goalAdded(*goalIt) ;
      }
    }
  }
}

NSConcern&
NSConcern::operator=(const NSConcern& src)
{
try
{
	if (this == &src)
		return *this ;

	_pDoc            = src._pDoc ;
  _iFrame          = src._iFrame ;

	_sTitre          = src._sTitre ;
	_sSignificatif   = src._sSignificatif ;
	_sSeverite       = src._sSeverite ;
	_tDateOuverture  = src._tDateOuverture ;
	_tDateFermeture  = src._tDateFermeture ;
	_tDateDebut      = src._tDateDebut ;
	_tDateAutoClose  = src._tDateAutoClose ;

	_sReference      = src._sReference ;
  _sLexique        = src._sLexique ;
	_sHealthIssue    = src._sHealthIssue ;
	_sPrimoPb        = src._sPrimoPb ;
  _sCocCode        = src._sCocCode ;
  _sCimCode        = src._sCimCode ;

	_aModificateurs  = src._aModificateurs ;

	if (src._pPptDetails)
  {
  	if (_pPptDetails)
    	*_pPptDetails = *(src._pPptDetails) ;
    else
    	_pPptDetails = new NSPatPathoArray(*(src._pPptDetails)) ;
	}
	else if (_pPptDetails)
	{
		delete _pPptDetails ;
    _pPptDetails = (NSPatPathoArray*) 0 ;
	}

  _aWorstJalons.vider() ;
  if (false == src._aWorstJalons.empty())
  	_aWorstJalons = src._aWorstJalons ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSConcern::operator=.", standardError, 0) ;
	return *this;
}
}

//-----------------------------------------------------------------------//
//				   		 Classe ArrayConcern                             //
//-----------------------------------------------------------------------//

ArrayConcern::ArrayConcern(const ArrayConcern& rv)
             :ArrayPb()
{
try
{
	_pFrame = rv._pFrame ;

	if (false == rv.empty())
		for (ArrayConcernConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSConcern(*(*i))) ;
}
catch (...)
{
	erreur("Exception ArrayConcern ctor.", standardError, 0) ;
}
}

void
ArrayConcern::vider()
{
	if (empty())
		return ;

	for (ArrayConcernIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

ArrayConcern::~ArrayConcern()
{
	vider() ;
}

void
ArrayConcern::initFromPatho(NSPatPathoArray* pPpt)
{
	if (((NSPatPathoArray*) NULL == pPpt) || (pPpt->empty()))
		return ;

	// Looking for the "concerns" chapter
  //
	PatPathoIter iter = pPpt->ChercherItem("0PRO11") ;

	if (((PatPathoIter) NULL == iter) || (pPpt->end() == iter))
    return ;

  int iColBase = (*iter)->getColonne() ;

  iter++ ;
  if ((pPpt->end() == iter) || ((*iter)->getColonne() == iColBase))
    return ;

  loadConcerns(iter, iColBase, pPpt) ;
}

void
ArrayConcern::init(LDVFRAME iFrame)
{
  if (NULL == _pFrame)
		return ;

  NSFrameInformation *pFrameInfo = (NSFrameInformation *) 0 ;

  // Getting Frame Information
  //
  if (_pFrame->getFrame() != iFrame)
  {
    NSFrameInformationArray *pFrames = _pFrame->getDoc()->getFrames() ;
    if ((NULL == pFrames) || pFrames->empty())
      return ;

    pFrameInfo = pFrames->getFrameInformation(iFrame) ;
  }
  else
    pFrameInfo = _pFrame ;

  if (NULL == pFrameInfo)
    return ;

  NSPatPathoArray PptIndex(_pFrame->pContexte->getSuperviseur()) ;
  pFrameInfo->initFromPatPathoIndex(&PptIndex) ;

  // NSPatPathoArray* pPtIndex = pFrameInfo->getPatPathoIndex() ;
  initFromPatho(&PptIndex) ;

  NSDocumentHistoArray* pVectDocs = pFrameInfo->getConcernDocs() ;
  if ((NULL == pVectDocs) || pVectDocs->empty())
    return ;

  for (DocumentIter i = pVectDocs->begin() ; pVectDocs->end() != i ; i++)
    if (*i)
    {
      NSPatPathoArray PptDoc(_pFrame->pContexte->getSuperviseur()) ;
      (*i)->initFromPatPatho(&PptDoc) ;

      initFromPatho(&PptDoc) ;
    }
}

/*
void
ArrayConcern::initialiser()
{
	if (NULL == pDoc)
		return;

	NSPatPathoArray* pPtIndex = pDoc->getPOMRIndex() ;

  initFromPatho(pPtIndex) ;
}

void
ArrayConcern::initRisks()
{
	if (NULL == pDoc)
		return;

	NSPatPathoArray* pPtIndex = pDoc->getRiskIndex() ;

  initFromPatho(pPtIndex) ;
}

void
ArrayConcern::initSocial()
{
	if (NULL == pDoc)
		return;

	NSPatPathoArray* pPtIndex = pDoc->getSocialIndex() ;

  initFromPatho(pPtIndex) ;
}
*/

void
ArrayConcern::reinit()
{
	if (NULL == _pFrame)
		return ;

  NSPatPathoArray PptIndex(_pFrame->pContexte->getSuperviseur()) ;
  _pFrame->initFromPatPathoIndex(&PptIndex) ;

  if (PptIndex.empty())
		return ;

  reinitFromPatho(&PptIndex) ;

  NSDocumentHistoArray* pVectDoc = _pFrame->getConcernDocs() ;
  if ((NULL == pVectDoc) || pVectDoc->empty())
    return ;

  for (DocumentIter i = pVectDoc->begin() ; pVectDoc->end() != i ; i++)
    if (*i)
    {
      NSPatPathoArray PptDoc(_pFrame->pContexte->getSuperviseur()) ;
      (*i)->initFromPatPatho(&PptDoc) ;

      reinitFromPatho(&PptDoc) ;
    }
}

void
ArrayConcern::reinitFromPatho(NSPatPathoArray* pPpt)
{
	if ((NULL == pPpt) || (pPpt->empty()))
		return ;

  	// On cherche le chapitre "préoccupations de santé"
	PatPathoIter iter = pPpt->ChercherItem("0PRO11") ;
	if ((NULL == iter) || (pPpt->end() == iter))
    return ;

  int iColBase = (*iter)->getColonne() ;

  iter++ ;
  if ((pPpt->end() == iter) || ((*iter)->getColonne() == iColBase))
    return ;

  reloadConcerns(pPpt, iter, iColBase) ;
}

void
ArrayConcern::reloadConcerns(NSPatPathoArray* pPtIndex, PatPathoIter iter, int iColBase)
{
try
{
  if (((NSPatPathoArray*) NULL == pPtIndex) || pPtIndex->empty())
    return ;

	NSConcern* pConcern = (NSConcern*) 0 ;

	while ((pPtIndex->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		// Problème
		if ((*iter)->getColonne() == iColBase+1)
		{
			if (pConcern)
			{
				bool    b2Add = true ;
				if (false == empty())
				{
					for (ArrayConcernIter PbIter = begin() ; PbIter != end() ; PbIter++)
						if ((*PbIter)->_sReference == pConcern->_sReference)
							b2Add = false ;
				}
				if (b2Add)
				{
					NSConcern *pPb = new NSConcern(*pConcern) ;
					push_back(pPb) ;
          _pFrame->getDoc()->getSsObjets()->initForConcern(pPb) ;

					TView* pView = _pFrame->getDoc()->GetViewList() ;
          if (pView)
          {
						do
						{
							NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
							if (pLdvView)
								pLdvView->addProb(pPb) ;

							pView = _pFrame->getDoc()->NextView(pView) ;
						}
						while (pView) ;
          }
				}
				delete pConcern ;
				pConcern = 0 ;
			}

      string sCodeLex = (*iter)->getLexique() ;
      string sCodeSensConcern;
    	_pFrame->pContexte->getDico()->donneCodeSens(&sCodeLex, &sCodeSensConcern);

      //
      // Préoccupation cachée -- Hidden health concern
      //
      if (sCodeSensConcern == "90000")
      {
      	iter++ ;
      	while ( (iter != pPtIndex->end()) &&
                ((*iter)->getColonne() > iColBase + 1))
        	iter++ ;
      }
      //
      // Préoccupation visible -- Visible health concern
      //
      else
      {
        pConcern = new NSConcern(_pFrame->pContexte, _pFrame->getDoc(), _pFrame->getFrame()) ;

        // Label
        pConcern->initTitle(*iter) ;

        // Node
        pConcern->setNoeud((*iter)->getNode());

        iter++;

        // Paramètres du problème
        while ((iter != pPtIndex->end()) && ((*iter)->getColonne() > iColBase + 1))
        {
          string sSens = (*iter)->getLexiqueSens() ;

          if ((*iter)->getColonne() == iColBase+2)
          {
            // Dates
            if ((sSens == "KOUVR") || (sSens == "KFERM") || (sSens == "KSYMP"))
            {
              iter++ ;
              int iLigneBase = (*iter)->getLigne() ;
              // gereDate* pDate = new gereDate(pContexte);
              string sUnite  = string("") ;
              string sFormat = string("") ;
              string sValeur = string("") ;
              while ((iter != pPtIndex->end()) &&
                      ((*iter)->getLigne() == iLigneBase))
              {
                if (_pFrame->pContexte->getDico()->CodeCategorie((*iter)->getLexique()) == string(1, '£'))
                {
                	sFormat = (*iter)->getLexiqueSens() ;
                  sValeur = (*iter)->getComplement() ;
                  sUnite  = (*iter)->getUnitSens() ;
                  break ;
                }
								iter++ ;
              }

              // sFormat est du type £D0;03
              if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (sValeur != ""))
              {
                if ((sUnite == "2DA01") || (sUnite == "2DA02"))
                {
                	if (sSens == "KOUVR")
                    pConcern->_tDateOuverture.initFromDate(sValeur);
                  else
                    if (sSens == "KFERM")
                      pConcern->_tDateFermeture.initFromDate(sValeur);
                    else
                      if (sSens == "KSYMP")
                        pConcern->_tDateDebut.initFromDate(sValeur);
                }
              }
            }
            //
            // Continuity of care Code
            //
            else if (string("6CISP") == sSens)
            {
              pConcern->_sCocCode = (*iter)->getComplement() ;
              iter++ ;
            }
            //
            // ICD Code
            //
            else if (string("6CIMA") == sSens)
            {
              pConcern->_sCimCode = (*iter)->getComplement() ;
              iter++ ;
            }
            //
            // Si des détails ont été précisés, on les stocke dans leur patpatho
            // If details were given, we store them in their patpatho
            //
            else if (sSens == "0DETA")
            {
            	if (NULL == pConcern->_pPptDetails)
              	pConcern->_pPptDetails = new NSPatPathoArray(_pFrame->pContexte->getSuperviseur()) ;
              else
              	pConcern->_pPptDetails->vider() ;

              int iColDetails = (*iter)->getColonne() ;
              iter++ ;

              int iLigne   = 0 ;
              int iColonne = 0 ;

              while ((iter != pPtIndex->end()) &&
          								((*iter)->getColonne() > iColDetails))
              {
              	iColonne = (*iter)->getColonne() - iColDetails - 1 ;
                pConcern->_pPptDetails->ajoutePatho(iter, iLigne, iColonne) ;
                iter++ ;
                iLigne++ ;
              }
            }
            else
              iter++ ;
          }
          else
            iter++ ;
        }
  		}
		}
		else
			iter++ ;
	}

	if (pConcern)
	{
		bool    b2Add = true ;
		if (!empty())
		{
			for (ArrayConcernIter PbIter = begin() ; PbIter != end() ; PbIter++)
				if ((*PbIter)->_sReference == pConcern->_sReference)
					b2Add = false ;
		}
		if (b2Add)
		{
			NSConcern *pPb = new NSConcern(*pConcern) ;
			push_back(pPb) ;
			_pFrame->getDoc()->getSsObjets()->initForConcern(pPb) ;

			TView* pView = _pFrame->getDoc()->GetViewList() ;
      if (pView)
      {
				do
				{
					NSLdvViewBase* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvViewBase) ;
					if (pLdvView)
						pLdvView->addProb(pPb) ;

          pView = _pFrame->getDoc()->NextView(pView) ;
				}
      	while (pView) ;
      }
		}
		delete pConcern ;
		pConcern = (NSConcern*) 0 ;
	}
}
catch (...)
{
	erreur("Exception ArrayConcern::reloadConcerns.", standardError, 0) ;
  return ;
}
}

void
ArrayConcern::deleteConcern(NSConcern* pConcern)
{
	if (empty())
		return ;

	for (ArrayConcernIter i = begin() ; end() != i ; i++)
	{
		if (*i == pConcern)
		{
			delete pConcern ;
			erase(i) ;
			return ;
		}
	}
}

void
ArrayConcern::loadConcerns(PatPathoIter iter, int iColBase, NSPatPathoArray* pPtIndex)
{
try
{
  if (((NSPatPathoArray*) NULL == pPtIndex) || pPtIndex->empty())
    return ;

  // Is there a root concept to add to concerns concept?
  //
  string sRootCode = (*(pPtIndex->begin()))->getLexique() ;

  string sCodeSensRoot ;
  _pFrame->pContexte->getDico()->donneCodeSens(&sRootCode, &sCodeSensRoot) ;

  // So far, only the risk leads to adding a root concept
  //
  if (string("ORISK") != sCodeSensRoot)
    sRootCode = string("") ;

  // Let's parse the tree
  //
	NSConcern* pConcern = (NSConcern*) 0 ;

	while ( (pPtIndex->end() != iter) &&
					((*iter)->getColonne() > iColBase))
	{
		// Problème
		if ((*iter)->getColonne() == iColBase+1)
		{
			if (pConcern)
			{
				push_back(new NSConcern(*pConcern)) ;
				delete pConcern ;
				pConcern = (NSConcern*) 0 ;
			}

      string sCodeLex = (*iter)->getLexique() ;
      string sCodeSensConcern;
    	_pFrame->pContexte->getDico()->donneCodeSens(&sCodeLex, &sCodeSensConcern) ;

      //
      // Préoccupation cachée -- Hidden health concern
      //
      if (string("90000") == sCodeSensConcern)
      {
      	iter++ ;
      	while ( (pPtIndex->end() != iter) &&
                ((*iter)->getColonne() > iColBase + 1))
        	iter++ ;
      }
      //
      // Préoccupation visible -- Visible health concern
      //
      else
      {
        pConcern = new NSConcern(_pFrame->pContexte, _pFrame->getDoc(), _pFrame->getFrame()) ;

        // Label
        pConcern->initTitle(*iter) ;

        // Node
        string sNoeud = (*iter)->getNode() ;
        pConcern->setNoeud(sNoeud) ;

        iter++ ;

        // Paramètres du problème
        while ((pPtIndex->end() != iter) &&
                ((*iter)->getColonne() > iColBase + 1))
        {
          string sSens = (*iter)->getLexiqueSens() ;

          if ((*iter)->getColonne() == iColBase+2)
          {
            // Dates
            if ((sSens == "KOUVR") || (sSens == "KFERM") || (sSens == "KSYMP"))
            {
              iter++ ;
              int iLigneBase = (*iter)->getLigne() ;
              // gereDate* pDate = new gereDate(pContexte);
              string sUnite  = "" ;
              string sFormat = "" ;
              string sValeur = "" ;
              while ((pPtIndex->end() != iter) &&
                      ((*iter)->getLigne() == iLigneBase))
              {
								if (_pFrame->pContexte->getDico()->CodeCategorie((*iter)->getLexique()) == string(1, '£'))
								{
                  sFormat = (*iter)->getLexiqueSens() ;
                  sValeur = (*iter)->getComplement() ;
                  sUnite  = (*iter)->getUnitSens() ;
                  break ;
                }
                iter++ ;
              }

              // sFormat est du type £D0;03
              if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) &&
                          (sValeur != ""))
              {
                if ((sUnite == "2DA01") || (sUnite == "2DA02"))
                {
                  if (sSens == "KOUVR")
                    pConcern->_tDateOuverture.initFromDate(sValeur) ;
                  else
                    if (sSens == "KFERM")
                      pConcern->_tDateFermeture.initFromDate(sValeur) ;
                    else
                      if (sSens == "KSYMP")
                        pConcern->_tDateDebut.initFromDate(sValeur) ;
                }
              }
            }
            //
            // Codes
            //
            else if (string("6CISP") == sSens)
            {
              pConcern->_sCocCode = (*iter)->getComplement() ;
              iter++ ;
            }
            else if (string("6CIMA") == sSens)
            {
              pConcern->_sCimCode = (*iter)->getComplement() ;
              iter++ ;
            }
            //
            // Si des détails ont été précisés, on les stocke dans leur patpatho
            // If details were given, we store them in their patpatho
            //
            else if (sSens == "0DETA")
            {
            	if (NULL == pConcern->_pPptDetails)
              	pConcern->_pPptDetails = new NSPatPathoArray(_pFrame->pContexte->getSuperviseur()) ;
              else
              	pConcern->_pPptDetails->vider() ;

              int iColDetails = (*iter)->getColonne() ;
              iter++ ;

              int iLigne      = 0 ;
              int iColonne    = 0 ;

              while ( (iter != pPtIndex->end()) &&
                    ((*iter)->getColonne() > iColDetails))
              {
              	iColonne = (*iter)->getColonne() - iColDetails - 1 ;
                pConcern->_pPptDetails->ajoutePatho(iter, iLigne, iColonne) ;
                iter++ ;
                iLigne++ ;
              }
            }
            else
              iter++ ;
          }
          else
            iter++ ;
        }

        // Cette préoccupation est-elle l'évolution d'une autre ?
        // Is this health concern the evolution of a previous one ?
        NSLinkManager* pGraphe = _pFrame->pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
        VecteurString VectString ;
        pGraphe->TousLesVrais(sNoeud, NSRootLink::problemRelatedTo, &VectString) ;
        if (false == VectString.empty())
          pConcern->_sPrimoPb = *(*(VectString.begin())) ;
      }
		}
		else
			iter++ ;
	}

	if (pConcern)
		push_back(pConcern) ;
}
catch (...)
{
	erreur("Exception ArrayConcern::loadConcerns.", standardError, 0) ;
	return;
}
}

/*
NSConcern*
ArrayConcern::getConcern(string sRef)
{
	if (empty())
		return NULL;

	for (ArrayConcernIter i = begin(); i != end(); i++)
		if ((*i)->getNoeud() == sRef)
			return *i;

	return NULL;
}
*/


NSConcern*
ArrayConcern::getFils(NSConcern* pConcern)
{
	if (empty())
		return (NSConcern*) 0 ;

	string sNoeud = pConcern->getNoeud();

	for (ArrayConcernIter i = begin(); end() != i ; i++)
		if ((*i)->_sPrimoPb == sNoeud)
			return *i ;

	return (NSConcern*) 0 ;
}


ArrayConcern&
ArrayConcern::operator=(const ArrayConcern& src)
{
try
{
	if (this == &src)
		return *this ;

	_pFrame = src._pFrame ;

	vider() ;

	if (false == src.empty())
		for (ArrayConcernConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSConcern(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayConcern (=).", standardError, 0) ;
	return *this;
}
}

//-----------------------------------------------------------------------
//
//						 Classe NSLdvSousObjet
//
//-----------------------------------------------------------------------

NSLdvSousObjet::NSLdvSousObjet(NSContexte* pCtx)
               :NSRoot(pCtx)
{
	sTitre     = string("") ;
	tDateHeureDebut.init();
	tDateHeureFin.init();

	sConcern   = string("") ;
	sObject    = string("") ;
	sReference = string("") ;

  lObjectCount++ ;
}

NSLdvSousObjet::NSLdvSousObjet(const NSLdvSousObjet& rv)
               :NSRoot(rv.pContexte)
{
	sTitre          = rv.sTitre ;
	tDateHeureDebut = rv.tDateHeureDebut ;
	tDateHeureFin   = rv.tDateHeureFin ;
	sConcern        = rv.sConcern ;
	sObject         = rv.sObject ;
	sReference      = rv.sReference ;

  lObjectCount++ ;
}

NSLdvSousObjet::~NSLdvSousObjet()
{
  lObjectCount-- ;
}

NSLdvSousObjet&
NSLdvSousObjet::operator=(const NSLdvSousObjet& src)
{
  if (this == &src)
		return *this ;

	sTitre          = src.sTitre ;
	tDateHeureDebut = src.tDateHeureDebut ;
	tDateHeureFin   = src.tDateHeureFin ;
	sConcern        = src.sConcern ;
	sObject         = src.sObject ;
	sReference      = src.sReference ;

	return *this ;
}

//-----------------------------------------------------------------------
//
//				   		 Classe ArraySsObjet
//
//-----------------------------------------------------------------------

ArraySsObjet::ArraySsObjet(const ArraySsObjet& rv)
             :ArraySsObj()
{
try
{
	if (false == rv.empty())
		for (ArraySsObjConstIter i = rv.begin() ; rv.end() != i ; i++)
    	push_back(new NSLdvSousObjet(*(*i))) ;
}
catch (...)
{
  erreur("Exception ArraySsObjet copy ctor.", standardError, 0) ;
}
}

void
ArraySsObjet::vider()
{
	if (empty())
		return;

	for (ArraySsObjIter i = begin(); i != end(); )
	{
		delete *i;
		erase(i);
	}
}

ArraySsObjet::~ArraySsObjet()
{
	vider();
}

ArraySsObjet&
ArraySsObjet::operator=(const ArraySsObjet& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (ArraySsObjConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSLdvSousObjet(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArraySsObjet (=).", standardError, 0) ;
	return *this ;
}
}

void
ArraySsObjet::initialiser()
{
try
{
  NSFrameInformationArray *pFrames = pDoc->getFrames() ;
  if (NULL == pFrames)
    return ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;

      if (pConcerns && (false == pConcerns->empty()))
      {
	      ArrayConcernIter iPb = pConcerns->begin() ;
	      for ( ; pConcerns->end() != iPb ; iPb++)
		      initForConcern(*iPb) ;
      }
    }
  }
}
catch (...)
{
	erreur("Exception ArraySsObjet::initialiser.", standardError, 0) ;
}
}

void
ArraySsObjet::initForConcern(NSConcern* pConcern)
{
try
{
	// Recherche des éléments de contact liés à ce problème de santé
	// Looking for the contact elements linked to this health problem
	NSLinkManager* pGraphe = pDoc->pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
	VecteurString VectString;
	DBITBLNAME sensCle = "ENVERS";
	pGraphe->TousLesVrais(pConcern->_sReference, NSRootLink::problemContactElement,
                            &VectString, sensCle);

	if (false == VectString.empty())
	{
		EquiItemIter stringIter = VectString.begin() ;
		for ( ; VectString.end() != stringIter ; stringIter++)
			initSmartSsObject(*(*stringIter), pConcern);
	}
}
catch (...)
{
	erreur("Exception ArraySsObjet::initialiser.", standardError, 0) ;
}
}

bool
ArraySsObjet::initSmartSsObject(string sNoeud, NSConcern* pConcern)
{
try
{
	string sObjReference = string(sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

	// Recherche du document qui contient ce sous-objet
	// Looking for the document that contains this sub-object
	NSHISTODocument* pHistory = pDoc->pContexte->getPatient()->getDocHis() ;
  if (NULL == pHistory)
    return false ;

  NSDocHistoArray* pVectDocument = pHistory->getVectDocument() ;
  if ((NULL == pVectDocument) || pVectDocument->empty())
	{
		initStupidSsObject(sNoeud, pConcern) ;
		return true ;
	}

	DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sObjReference) ;

	// Not found
	if (NULL == iterDoc)
	{
		initStupidSsObject(sNoeud, pConcern) ;
		return true ;
	}

  NSPatPathoArray PPT(pDoc->pContexte->getSuperviseur()) ;
  (*iterDoc)->initFromPatPatho(&PPT) ;

	if (PPT.empty())
	{
		initStupidSsObject(sNoeud, pConcern) ;
		return true ;
	}

	// Recherche du noeud au sein du document
	// Looking for the node inside the document
	PatPathoIter iter = PPT.begin() ;
	while ((PPT.end() != iter) && ((*iter)->getNode() != sNoeud))
		iter++ ;

	if (PPT.end() == iter)
	{
		initStupidSsObject(sNoeud, pConcern) ;
		return true ;
	}

	NSPatPathoArray SousPt(pDoc->pContexte->getSuperviseur()) ;
	PPT.ExtrairePatPatho(iter, &SousPt) ;

	if (SousPt.empty())
	{
		initStupidSsObject(sNoeud, pConcern) ;
		return true;
	}

	string sLang = pDoc->pContexte->getUserLanguage() ;

	string  sDateDeb    = "";
	string  sDateFin    = "";
	int     iSeverity   = 0;
	int     iRisk       = 0;

	PatPathoIter ssIter = SousPt.begin() ;
	while (SousPt.end() != ssIter)
	{
		bool bIncrementer = true ;
		string sCodeSens = (*ssIter)->getLexiqueSens() ;

		if ((sCodeSens == "KDAT0") || (sCodeSens == "KOUVR") ||
				(sCodeSens == "KFERM"))
		{
			int iRefCol = (*ssIter)->getColonne() ;

			ssIter++ ;

			if ((SousPt.end() != ssIter) &&
					((*ssIter)->getColonne() > iRefCol))
			{
				gereDate Date(pDoc->pContexte->getSuperviseur(), sLang) ;

				int iLigne = (*ssIter)->getLigne() ;

				string sUnite ;
				string sFormat ;
				string sValeur ;

				while (	(ssIter != SousPt.end()) &&
								((*ssIter)->getLigne() == iLigne))
				{
        	if (pDoc->pContexte->getDico()->CodeCategorie((*ssIter)->getLexique()) == string(1, '£'))
          {
            sFormat = (*ssIter)->getLexiqueSens() ;
            sValeur = (*ssIter)->getComplement() ;
            sUnite  = (*ssIter)->getUnitSens() ;
            break ;
          }
					ssIter++ ;
				}
				bIncrementer = false ;

				Date.setDate(&sValeur) ;
				Date.setFormat(&sFormat) ;
				Date.setUnite(&sUnite) ;

        if ((sCodeSens == "KDAT0") || (sCodeSens == "KOUVR"))
					sDateDeb = Date.getDate() ;
				else
					if (sCodeSens == "KFERM")
						sDateFin = Date.getDate() ;
			}
		}
		if ((sCodeSens == "VIGRA") || (sCodeSens == "VIRIS"))
		{
			int iRefCol = (*ssIter)->getColonne() ;

			ssIter++ ;

			if ((SousPt.end() != ssIter) && ((*ssIter)->getColonne() > iRefCol))
			{
				gereNum Num(pDoc->pContexte->getSuperviseur(), sLang) ;

				int iLigne = (*ssIter)->getLigne() ;

				string sTemp ;
				string sUnite ;
				string sFormat ;
				string sValeur ;

				while ((SousPt.end() != ssIter) && ((*ssIter)->getLigne() == iLigne))
				{
          if (pDoc->pContexte->getDico()->CodeCategorie((*ssIter)->getLexique()) == string(1, '£'))
          {
          	sFormat = (*ssIter)->getLexique() ;
            sValeur = (*ssIter)->getComplement() ;
            sUnite  = (*ssIter)->getUnit() ;
            break ;
          }
					ssIter++ ;
				}
				bIncrementer = false ;

				Num.instancier(&sValeur, &sUnite, &sFormat) ;

				if (Num.estExact())
				{
					double dValue = Num.getValeur() ;

					if (sCodeSens == "VIGRA")
						iSeverity = dtoi(dValue) ;
					else
						if (sCodeSens == "VIRIS")
							iRisk = dtoi(dValue) ;
				}
			}
		}
		if ((bIncrementer) && (ssIter != SousPt.end()))
			ssIter++ ;
	}

	// Création d'un Sous objet
	// Creating a SubObject
	NVLdVTemps tDateTimeDeb;
	NVLdVTemps tDateTimeFin;

	if ((sDateDeb == "") && (sDateFin == ""))
		initStupidSsObject(sNoeud, pConcern) ;
	else
	{
		NSLdvSousObjet* pSSObj = new NSLdvSousObjet(pDoc->pContexte) ;

		pSSObj->sReference = sNoeud ;
		pSSObj->sConcern   = pConcern->_sReference ;

		if (sDateDeb != "")
			pSSObj->tDateHeureDebut.initFromDate(sDateDeb) ;
		if (sDateFin != "")
			pSSObj->tDateHeureFin.initFromDate(sDateFin) ;

		// S'il manque des informations, on va les chercher dans l'objet
		// If some data is missing, we look for it in the Object
		if (((sDateDeb == "") || (sDateFin == "")) && (!(pDoc->getObjets()->empty())))
		{
			ArrayObjets *pObjs = pDoc->getObjets() ;
			ArrayObjIter ObjIter = pObjs->begin();
			for (; (pObjs->end() != ObjIter) &&
				            ((*ObjIter)->getReference() != sObjReference); ObjIter++) ;
			if (pObjs->end() != ObjIter)
			{
				if (string("") == sDateDeb)
				{
					pSSObj->tDateHeureDebut = (*ObjIter)->getDateHeureDebut() ;
					tDateTimeDeb            = (*ObjIter)->getDateHeureDebut() ;
				}
				if (string("") == sDateFin)
					pSSObj->tDateHeureFin = (*ObjIter)->getDateHeureFin() ;
			}
		}
		push_back(pSSObj);
	}

	if ((iSeverity == 0) && (iRisk == 0))
		return true;

	// Création d'un ConcernModifier
	// Creation of a ConcernModifier
	tDateTimeFin.estNoLimit();
	if (sDateDeb != "")
		tDateTimeDeb.initFromDate(sDateDeb);

	NSConcernModifier Modifier(pConcern,
                             tDateTimeDeb,
                             tDateTimeFin,
                             iSeverity,
                             0,
                             iRisk) ;
	Modifier.setNode(sNoeud) ;
	pConcern->_aModificateurs.addModifier(&Modifier) ;

  return true ;
}
catch (...)
{
	erreur("Exception ArraySsObjet::initSmartSsObject.", standardError, 0) ;
	return false ;
}
}

void
ArraySsObjet::initStupidSsObject(string sNoeud, NSConcern* pConcern)
{
try
{
	NSLdvSousObjet* pSSObj = new NSLdvSousObjet(pDoc->pContexte) ;

	pSSObj->sReference = sNoeud ;
	pSSObj->sConcern   = pConcern->_sReference ;

	string  sObjReference = string(pSSObj->sReference, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

	// Si le sous-objet appartient à un objet déjà connu, il prend ses caractéristiques
	// If this sub-object belongs to a known object, it takes its values
	ArrayObjets *pObjs = pDoc->getObjets() ;

	if ((false == pObjs->empty()) && (sObjReference != ""))
	{
		for (ArrayObjIter ObjIter = pObjs->begin() ; pObjs->end() != ObjIter ; ObjIter++)
		{
			if ((*ObjIter)->getReference() == sObjReference)
			{
				pSSObj->tDateHeureDebut = (*ObjIter)->getDateHeureDebut() ;
				pSSObj->tDateHeureFin   = (*ObjIter)->getDateHeureFin() ;
			}
		}
	}

  push_back(pSSObj) ;
}
catch (...)
{
	erreur("Exception ArraySsObjet::initStupidSsObject.", standardError, 0) ;
}
}

//-----------------------------------------------------------------------
//
//						  Classe NSLdvObjet
//
//-----------------------------------------------------------------------

NSLdvObjet::NSLdvObjet(NSContexte* pCtx)
           :NSRoot(pCtx)
{
	_sTitre     = string("") ;
	_sLexique   = string("") ;
	_tDateHeureDebut.init() ;
	_tDateHeureFin.init() ;

	_sConcern   = string("") ;

	_sReference = string("") ;
	_sTypeDoc   = string("") ;

  _pQolChart  = (NSQualityOfLifeChart *) 0 ;

  lObjectCount++ ;
}

NSLdvObjet::NSLdvObjet(const NSLdvObjet& rv)
           :NSRoot(rv.pContexte)
{
	_sTitre          = rv._sTitre ;
  _sLexique        = rv._sLexique ;
	_tDateHeureDebut = rv._tDateHeureDebut ;
	_tDateHeureFin   = rv._tDateHeureFin ;

	_sConcern        = rv._sConcern ;

	_sReference      = rv._sReference ;
	_sTypeDoc        = rv._sTypeDoc ;

  _pQolChart       = rv._pQolChart ;

  lObjectCount++ ;
}

NSLdvObjet::~NSLdvObjet()
{
  lObjectCount-- ;
}

NSLdvObjet&
NSLdvObjet::operator=(const NSLdvObjet& src)
{
	if (this == &src)
		return *this ;

	_sTitre          = src._sTitre ;
	_sLexique        = src._sLexique ;
	_tDateHeureDebut = src._tDateHeureDebut ;
	_tDateHeureFin   = src._tDateHeureFin ;

	_sConcern        = src._sConcern ;

	_sReference      = src._sReference ;
	_sTypeDoc        = src._sTypeDoc ;

  _pQolChart       = src._pQolChart ;

	return *this ;
}

//-----------------------------------------------------------------------
//
//				   		 Classe ArraySsObjet
//
//-----------------------------------------------------------------------

ArrayObjets::ArrayObjets(const ArrayObjets& rv)
            :ArrayObj()
{
try
{
	if (false == rv.empty())
		for (ArrayObjConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSLdvObjet(*(*i))) ;
}
catch (...)
{
	erreur("Exception ArrayObjets copy ctor.", standardError, 0) ;
}
}

void
ArrayObjets::vider()
{
	if (empty())
		return;

	for (ArrayObjIter i = begin() ; end() != i ; )
	{
		delete *i;
		erase(i);
	}
}

ArrayObjets::~ArrayObjets()
{
	vider() ;
}

ArrayObjets&
ArrayObjets::operator=(const ArrayObjets& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (ArrayObjConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSLdvObjet(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayObjets operator=.", standardError, 0) ;
	return *this;
}
}

void
ArrayObjets::initialiser()
{
try
{
	// Recupérer un pointeur sur l'Historique
	// Get a pointer on the History tree

	if ((NULL == pDoc) || (NULL == pDoc->pContexte))
		return ;

	NSPatientChoisi* pPat = pDoc->pContexte->getPatient() ;
	if (NULL == pPat)
		return ;

	// Document his
	NSHISTODocument* pHistory = pPat->getDocHis() ;
	if (NULL == pHistory)
		return ;

  NSDocHistoArray* pVectDocument = pHistory->getVectDocument() ;
  if ((NULL == pVectDocument) || pVectDocument->empty())
		return ;

	DocumentIter iterDoc = pVectDocument->begin() ;
	for ( ; pVectDocument->end() != iterDoc ; iterDoc++)
	{
		bool bAjouter = true ;

		string sRootCode     = string("") ;
    string sRootCodeSens = string("") ;

    NSPatPathoInfo* pRootPathoInfo = (*iterDoc)->getRootPathoInfo() ;

		// La Synthèse et l'Index POMR ne sont pas affichés
		// Synthesis and POMR index are not displayed
		if (pRootPathoInfo)
		{
      sRootCode = pRootPathoInfo->getLexique() ;

      sRootCodeSens = pRootPathoInfo->getLexiqueSens() ;
			if (("ZSYNT" == sRootCodeSens) || ("ZPOMR" == sRootCodeSens))
				bAjouter = false ;
		}

    if ("ZCOOP" == sRootCodeSens)
    {
      NVLdVTemps tpsInfo ;
      tpsInfo.initFromDate((*iterDoc)->getDateDoc()) ;

      NSPatPathoArray DocPatho(pDoc->pContexte->getSuperviseur()) ;
      (*iterDoc)->initFromPatPatho(&DocPatho) ;

      NSCoopChart *pChart = new NSCoopChart(pDoc->pContexte, tpsInfo, &DocPatho) ;
      pDoc->getQoLCharts()->push_back(pChart) ;

      bAjouter = false ;
    }

		if (bAjouter)
		{
			NSLdvObjet* pObj = new NSLdvObjet(pDoc->pContexte) ;

			string sReference = (*iterDoc)->getID() ;

			pObj->setTitre((*iterDoc)->getDocName()) ;
      pObj->setLexique(sRootCode) ;
			pObj->setDateHeureDebut((*iterDoc)->getDateDoc()) ;
			pObj->setDateHeureFin((*iterDoc)->getDateDoc()) ;

			pObj->setReference(sReference) ;
			pObj->setTypeDoc((*iterDoc)->getTypeSem()) ;

			// Ce document est-il lié à un problème de santé ?
			// Is this document linked to a health problem ?

      VecteurString aVectString ;
			NSLinkManager* pGraphe = pDoc->pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
      pGraphe->TousLesVraisDocument(sReference, NSRootLink::problemRelatedTo, &aVectString) ;

			if (false == aVectString.empty())
      {
      	pObj->setConcern(*(*(aVectString.begin()))) ;

        // Quality of life card set?
        //
        if (pDoc->pContexte->getSuperviseur()->getFilGuide()->VraiOuFaux(sRootCodeSens, "ES", "ZQOFL"))
        {
          NVLdVTemps tpsInfo ;
          tpsInfo.initFromDate((*iterDoc)->getDateDoc()) ;

          LDVFRAME iFrame = ldvframeNotSpecified ;
          NSConcern* pConcern = pDoc->getConcern(pObj->getConcern(), iFrame) ;

          if (pConcern && pConcern->getQualityOfLifeCharts())
          {
            NSPatPathoArray DocPatho(pDoc->pContexte->getSuperviseur()) ;
            (*iterDoc)->initFromPatPatho(&DocPatho) ;

            NSQualityOfLifeChart *pChart = new NSQualityOfLifeChart(pDoc->pContexte, tpsInfo, &DocPatho) ;
            pConcern->getQualityOfLifeCharts()->push_back(pChart) ;
            pObj->setQolChart(pChart) ;
          }
        }
      }

			push_back(pObj) ;
    }
	}
}
catch (...)
{
	erreur("Exception ArrayObjets::initialiser.", standardError, 0) ;
	return;
}
}

NSLdvObjet*
ArrayObjets::getObjet(string sRef)
{
	if (empty())
		return (NSLdvObjet*) 0 ;

	for (ArrayObjIter i = begin() ; end() != i ; i++)
		if ((*i)->getReference() == sRef)
			return *i;

	return (NSLdvObjet*) 0 ;
}

void
ArrayObjets::removeObjet(string sRef)
{
  if (empty())
		return ;

	for (ArrayObjIter i = begin() ; end() != i ; )
  {
		if ((*i)->getReference() == sRef)
    {
		  delete *i;
		  erase(i);
	  }
    else
      i++ ;
  }
}


