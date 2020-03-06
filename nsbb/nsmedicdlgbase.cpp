// -----------------------------------------------------------------------------
// nsmedicdlg.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.11 $
// $Author: pameline $
// $Date: 2017/04/12 20:19:06 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#include <owl\window.h>

#include "nssavoir/nsBdmDriver.h"
#include "nautilus/nssuper.h"

#include "nsbb/nsednum.h"
#include "nsbb/nscomboutil.h"
#include "nsbb/nsmedicdlgbase.h"
#include "nsbb/nsmediccontrols.h"
#include "nautilus/nsldvdoc.h"
#include "nsepisod/nsclasser.h"

#include "nsoutil/ibdm.h"
#include "nsoutil/nsBdmDlg.h"
#include "nsoutil/nsBdmDrugInfoDlg.h"
#include "dcodeur/nsdkd.h"

#include "nsbb/medicament.rh"

NSPrescriptionBaseDlg::NSPrescriptionBaseDlg(TWindow* parent, NSContexte* pCtx, TResId resID, OWL::TModule* module)
                      :NSUtilDialog(parent, pCtx, resID, module)
{
  _sLexiqCode     = string("") ;
	_sPriseUnit     = string("") ;
  _sLexiqRoute    = string("") ;
  _sFreeTextRoute = string("") ;
	_sDateOuverture = string("") ;
	_sDateFermeture = string("") ;
  _sALD           = string("") ;
  _sFreeText      = string("") ;
  _sPositionRepas = string("") ;
  _sLexiqEvent    = string("") ;
  _sFreeTextEvent = string("") ;
  _sATCCode       = string("") ;
  _sCICode        = string("") ;

  _bNonSubstituable = false ;

  createInterfaceElements() ;
}

NSPrescriptionBaseDlg::~NSPrescriptionBaseDlg()
{
  delete _pDrug ;
	delete _pUnitePriseTxt ;
	delete _pUnitePrise ;
  delete _pRouteTxt ;
  delete _pRoute ;
  delete _pEventTxt ;
  delete _pEvent ;

  delete _pALD ;
  delete _pIssues ;
  delete _pNonSubstituable ;
  delete _pFreeTextButton ;

	delete _pDateDebTxt ;
	delete _pDateDeb ;
  delete _pDateFinGroup ;
	delete _pRChronique ;
	delete _pRDans ;
	delete _pRDuree ;
	delete _pRLe ;
	delete _pNbJours ;
	delete _pCBDureeTtt ;
	delete _pDateFin ;
}

void
NSPrescriptionBaseDlg::createInterfaceElements()
{
	_pDrug            = new NSUtilLexique(pContexte, this, DRUG_EDIT, pContexte->getDico()) ;
  _pDrug->setLostFocusFunctor(new MemFunctor<NSPrescriptionBaseDlg>((NSPrescriptionBaseDlg*) this, &NSPrescriptionBaseDlg::ExecutedAfterDrugSelection)) ;

	_pUnitePriseTxt   = new OWL::TStatic(this, DRG_UNIT_TXT) ;
	_pUnitePrise      = new NSUtilLexique(pContexte, this, DRG_UNIT, pContexte->getDico()) ;

  _pALD             = new OWL::TCheckBox(this, ALD_BUTTON) ;
  _pNonSubstituable = new OWL::TCheckBox(this, NS_BUTTON) ;
  _pFreeTextButton  = new OWL::TButton(this, FREE_TEXT_BUTTON) ;
  _pIssues          = new OWL::TCheckBox(this, IDC_ISSUE_BTN) ;

  _pRouteTxt        = new OWL::TStatic(this, IDC_ROUTE_TXT) ;
  _pRoute           = new NSUtilLexique(pContexte, this, IDC_ROUTE_EDIT, pContexte->getDico()) ;
  _pRoute->setLostFocusFunctor(new MemFunctor<NSPrescriptionBaseDlg>((NSPrescriptionBaseDlg*) this, &NSPrescriptionBaseDlg::ExecutedAfterRouteSelection)) ;

  _pEventTxt        = new OWL::TStatic(this, IDC_EVENT_TXT) ;
  _pEvent           = new NSUtilLexique(pContexte, this, IDC_EVENT_EDIT, pContexte->getDico()) ;
  _pEvent->setLostFocusFunctor(new MemFunctor<NSPrescriptionBaseDlg>((NSPrescriptionBaseDlg*) this, &NSPrescriptionBaseDlg::ExecutedAfterEventSelection)) ;

  if (pContexte->getBamType() != NSContexte::btNone)
  {
    _pDrugInfoButton            = new OWL::TButton(this, DRUG_INFORMATION) ;
    _pBdmSearchIndicationButton = new OWL::TButton(this, BDM_SEARCH_INDIC) ;
    _pBdmSearchSubstanceButton  = new OWL::TButton(this, BDM_SEARCH_SUBST) ;
    _pBdmSearchAtcButton        = new OWL::TButton(this, BDM_SEARCH_ATC) ;
  }
  else
  {
    _pDrugInfoButton            = (OWL::TButton*) 0 ;
    _pBdmSearchIndicationButton = (OWL::TButton*) 0 ;
    _pBdmSearchSubstanceButton  = (OWL::TButton*) 0 ;
    _pBdmSearchAtcButton        = (OWL::TButton*) 0 ;
  }
  _pBdmDrugInformation = (NsSelectableDrug*) 0 ;

	_pDateDebTxt    = new OWL::TStatic(this, DATE_DEBPRESC_TEXT) ;
	_pDateDeb       = new NSUtilEditDateHeure(pContexte, this, DRUG_DATE_DEB) ;
  _pDateDeb->setLostFocusFunctor(new MemFunctor<NSPrescriptionBaseDlg>((NSPrescriptionBaseDlg*)this, &NSPrescriptionBaseDlg::ExecutedAfterTrtBeginDate )) ;
  _pDateFinGroup  = new OWL::TGroupBox(this, DATE_FIN_DRG_GROUP) ;
	_pRChronique    = new OWL::TRadioButton(this, IDR_DRG_CHRONIQUE) ;
	_pRDans         = new OWL::TRadioButton(this, IDR_DRG_DANS) ;
	_pRDuree        = new OWL::TRadioButton(this, IDR_DRG_DUREE) ;
	_pRLe           = new OWL::TRadioButton(this, IDR_DRG_LE) ;
	_pNbJours       = new NSEditNum(pContexte, this, IDC_DRG_NBJOURS, 10) ;
  _pNbJours->setLostFocusFunctor(new MemFunctor<NSPrescriptionBaseDlg>((NSPrescriptionBaseDlg*)this, &NSPrescriptionBaseDlg::ExecutedAfterTrtEndDate ));
  const char *temp1[] = { "2HEUR1", "2DAT01", "2DAT11", "2DAT21" } ;
  _pCBDureeTtt    = new NSComboBox(this, IDC_DRG_NBJOURS_TXT, pContexte, temp1, 4) ;
  _pCBDureeTtt->SetLostFocusResponse(new MemFunctor<NSPrescriptionBaseDlg>((NSPrescriptionBaseDlg*)this, &NSPrescriptionBaseDlg::ExecutedAfterTrtEndDate ));
	_pDateFin       = new NSUtilEditDateHeure(pContexte, this, DRUG_DATE_FIN) ;
}

void
NSPrescriptionBaseDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  initInterfaceElements() ;
}

void
NSPrescriptionBaseDlg::initInterfaceElements()
{
	// Init static texts
  //
	string sTxt = pContexte->getSuperviseur()->getText("drugDialog", "takeUnit") ;
	_pUnitePriseTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "ALD") ;
	_pALD->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "notReplacable") ;
  _pNonSubstituable->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "route") ;
	_pRouteTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "inCaseOf") ;
	_pEventTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "indications") ;
	_pIssues->SetCaption(sTxt.c_str()) ;

  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "startingDate") ;
	_pDateDebTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "endingDate") ;
  _pDateFinGroup->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "undetermined") ;
	_pRChronique->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "fromNow") ;
	_pRDans->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "totalDuration") ;
	_pRDuree->SetCaption(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "onThe") ;
	_pRLe->SetCaption(sTxt.c_str()) ;

  // Init contents
  //
  string sLang  = pContexte->getUserLanguage() ;

  string sForme = string("") ;
  string sLabel = string("") ;

  if (string("") != _sLexiqCode)
  {
		pContexte->getDico()->donneLibelle(sLang, &_sLexiqCode, &sLabel) ;
    _pDrug->setLabel(_sLexiqCode.c_str(), sLabel.c_str()) ;
    sForme = initDispUnit(sLang, _sLexiqCode, sLabel, pContexte) ;
  }
  else
    _pDrug->SetText("") ;

  if (string("") != _sPriseUnit)
    _pUnitePrise->setLabel(_sPriseUnit) ;
  else
  {
  	string sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "missingInformation") ;
    _pUnitePrise->SetText(sMsgTxt.c_str()) ;
    if ((string("") != sForme) && (sForme != sMsgTxt))
			_pUnitePrise->setLabel(sForme) ;
		else
			_pUnitePrise->SetText(sMsgTxt.c_str()) ;
  }

  if      (string("") != _sLexiqRoute)
    _pRoute->setLabel(_sLexiqRoute) ;
  else if (string("") != _sFreeTextRoute)
    _pRoute->setLabel(string("£?????"), _sFreeTextRoute) ;

  if      (string("") != _sLexiqEvent)
    _pEvent->setLabel(_sLexiqEvent) ;
  else if (string("") != _sFreeTextEvent)
    _pEvent->setLabel(string("£?????"), _sFreeTextEvent) ;

  if (_bNonSubstituable)
  	_pNonSubstituable->SetCheck(BF_CHECKED) ;
  else
    _pNonSubstituable->SetCheck(BF_UNCHECKED) ;

  if (string("") != _sALD)
  	_pALD->SetCheck(BF_CHECKED) ;
  else
    _pALD->SetCheck(BF_UNCHECKED) ;

  setSelectedIssuesBtnState() ;

  NVLdVTemps tpNow ;
	tpNow.takeTime() ;

  if (string("") != _sDateOuverture)
		_pDateDeb->setDate(_sDateOuverture) ;
  else
		_pDateDeb->setDate(tpNow.donneDateHeure()) ;

  if (string("") != _sDateFermeture)
  {
  	_pRLe->SetCheck(BF_CHECKED) ;
  	_pDateFin->setDate(_sDateFermeture) ;
  }
  else
  {
  	_pRChronique->SetCheck(BF_CHECKED) ;
    _pDateFin->SetReadOnly(true) ;
  }
  _pNbJours->SetReadOnly(true) ;
}

void
NSPrescriptionBaseDlg::editFreeText()
{
  string sFreeText = _sFreeText ;

  NSFreeTextDlg* pFreeTextDlg = new NSFreeTextDlg(this, pContexte, &sFreeText) ;
  int iExecReturn = pFreeTextDlg->Execute() ;
  delete pFreeTextDlg ;

  if (IDOK != iExecReturn)
    return ;

  _sFreeText = sFreeText ;
}

void
NSPrescriptionBaseDlg::setSelectedIssuesBtnState()
{
  if (_aLinkedIssues.empty())
  	_pIssues->SetCheck(BF_UNCHECKED) ;
  else
    _pIssues->SetCheck(BF_CHECKED) ;
}

/**
 * Get drug information from BDM
 */
void
NSPrescriptionBaseDlg::getDrugInformation()
{
  // Delete existing drug information
  //
  if (_pBdmDrugInformation)
  {
    delete _pBdmDrugInformation ;
    _pBdmDrugInformation = (NsSelectableDrug*) 0 ;
  }

  _sATCCode = string("") ;
  _sCICode  = string("") ;

  string sSelectedDrug = _pDrug->getCode() ;
  if (string("") == sSelectedDrug)
    return ;

  // Get BDM driver
  //
  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if ((NSBdmDriver*) NULL == pDriver)
    return ;

  // Is this drug referenced as having a CIS code?
  //
  string sSelectedDrugSens ;
  pContexte->getDico()->donneCodeSens(&sSelectedDrug, &sSelectedDrugSens) ;

  InterfaceBdm IBdm(pContexte) ;
  if (false == IBdm.isCodeLexiMedInDB(sSelectedDrugSens))
    return ;

  string sCisCode = IBdm.pBdm->getCodeCIP() ;
  if (string("") == sCisCode)
    return ;

  _sCICode = sCisCode ;

  // Get drug information and instanciate
  //
  _pBdmDrugInformation = new NsSelectableDrug() ;

  if (pDriver->getDrugInformation(_pBdmDrugInformation, &sCisCode))
  {
    _sATCCode = pDriver->getAtcCodeFromBamId(_pBdmDrugInformation->getBdmID()) ;
    return ;
  }

  // If BDM access failed, delete the drug information vector
  //
  if (_pBdmDrugInformation)
  {
    delete _pBdmDrugInformation ;
    _pBdmDrugInformation = (NsSelectableDrug*) 0 ;
  }
}

/**
 * All the controls to check if this drug is safe for the patient and can
 * be inserted among all others
 */
void
NSPrescriptionBaseDlg::checkDrugSafety()
{
  if ((NsSelectableDrug*) NULL == _pBdmDrugInformation)
    return ;

  checkSecurityCheckInformation() ;
  checkSportEnabledDrug() ;
  checkRedundancy() ;
  checkAllergies() ;
}

/**
 * Check wether biological information that allow for safety control are present
 */
void
NSPrescriptionBaseDlg::checkSecurityCheckInformation()
{
  if ((NsSelectableDrug*) NULL == _pBdmDrugInformation)
    return ;

  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatEnCours)
    return ;

  string sMissing  = string("") ;
  string sMissing1 = string("") ;
  string sMissing2 = string("") ;

  decodageBase dcode(pContexte, pContexte->getUserLanguage()) ;

  int iMissingInformation = 0 ;

  if (_pBdmDrugInformation->isWeightNeededForSecurityControl())
  {
    string sAnswerDate = string("") ;
    NSPatPathoArray* pAnswer = (NSPatPathoArray*) 0 ;

    bool bSuccess = pContexte->getBBinterface()->synchronousCall(string("VPOID1"), &pAnswer, (HWND) 0, &sAnswerDate) ;
    if ((false == bSuccess) || ((NSPatPathoArray*) NULL == pAnswer) || pAnswer->empty())
    {
      sMissing2 = pContexte->getSuperviseur()->getText("drugInformationAlerts", "theWeight") ;
      dcode.etDuMilieu(&sMissing, &sMissing1, &sMissing2, string(", ")) ;
      iMissingInformation++ ;
    }
  }

  if (_pBdmDrugInformation->isHeightNeededForSecurityControl())
  {
    string sAnswerDate = string("") ;
    NSPatPathoArray* pAnswer = (NSPatPathoArray*) 0 ;

    bool bSuccess = pContexte->getBBinterface()->synchronousCall(string("VTAIL1"), &pAnswer, (HWND) 0, &sAnswerDate) ;
    if ((false == bSuccess) || ((NSPatPathoArray*) NULL == pAnswer) || pAnswer->empty())
    {
      sMissing2 = pContexte->getSuperviseur()->getText("drugInformationAlerts", "theHeight") ;
      dcode.etDuMilieu(&sMissing, &sMissing1, &sMissing2, string(", ")) ;
      iMissingInformation++ ;
    }
  }

  if (_pBdmDrugInformation->isClearanceNeededForSecurityControl())
  {
    string sAnswerDate = string("") ;
    NSPatPathoArray* pAnswer = (NSPatPathoArray*) 0 ;

    bool bSuccess = pContexte->getBBinterface()->synchronousCall(string("VCREA1"), &pAnswer, (HWND) 0, &sAnswerDate) ;
    if ((false == bSuccess) || ((NSPatPathoArray*) NULL == pAnswer) || pAnswer->empty())
    {
      sMissing2 = pContexte->getSuperviseur()->getText("drugInformationAlerts", "theCreatininemia") ;
      dcode.etDuMilieu(&sMissing, &sMissing1, &sMissing2, string(", ")) ;
      iMissingInformation++ ;
    }
  }

  // At leat one information missing
  //
  if (0 == iMissingInformation)
    return ;

  string sEt = string(" ") + pContexte->getSuperviseur()->getText("drugInformationAlerts", "and") + string(" ") ;
  dcode.etFinal(&sMissing, &sMissing1, sEt) ;

  string sFollow = string("") ;
  if (1 == iMissingInformation)
    sFollow = pContexte->getSuperviseur()->getText("drugInformationAlerts", "isMissingForSecurityControl") ;
  else
    sFollow = pContexte->getSuperviseur()->getText("drugInformationAlerts", "areMissingForSecurityControl") ;
  sMissing += string(" ") + sFollow ;

  sMissing[0] = pseumaj(sMissing[0]) ;

  erreur(sMissing.c_str(), warningError, 0, GetHandle()) ;
}

void
NSPrescriptionBaseDlg::checkSportEnabledDrug()
{
  if ((NsSelectableDrug*) NULL == _pBdmDrugInformation)
    return ;

  if (false == _pBdmDrugInformation->isDrugInSport())
    return ;

  // Find competition sport in the synthesis
  //
  NSSearchStruct searchStruct ;

  // Find competition sport in the synthesis
  //
  string sPaths[4] ;
  sPaths[0] = string("ZSYNT/EMOVI/ESPOC") ;
  sPaths[1] = string("ZSYNT/EHABI/ESPOC") ;
  sPaths[2] = string("ZSYNT/EMOVI/ESPO2") ;
  sPaths[3] = string("ZSYNT/EHABI/ESPO2") ;

  for (int i = 0 ; i < 4 ; i++)
  {
    bool bOk = pContexte->getPatient()->ChercheChemin(sPaths[i], &searchStruct) ;
    if (bOk && (NSSearchStruct::foundNothing != searchStruct.getFoundStatus()))
    {
      string sWarning = pContexte->getSuperviseur()->getText("drugInformationAlerts", "isDrugInSport") ;
      erreur(sWarning.c_str(), warningError, 0, GetHandle()) ;
      return ;
    }
  }
}

/**
 * Check drugs active principles redundancy
 */
void
NSPrescriptionBaseDlg::checkRedundancy()
{
  if ((NsSelectableDrug*) NULL == _pBdmDrugInformation)
    return ;

  NsSelectableDrugArray aDrugsList ;
  pContexte->getPatient()->getLdvDocument()->getActiveBamCompatibleDrugs(&aDrugsList) ;

  if (aDrugsList.empty())
    return ;

  NsDrugComponentsArray aActiveComponents ;
  _pBdmDrugInformation->getActiveComponents(&aActiveComponents) ;

  if (aActiveComponents.empty())
    return ;

  string sMsg = string("") ;

  for (NsSelectableDrugIter it = aDrugsList.begin() ; aDrugsList.end() != it ; it++)
  {
    if ((string("") == _sEditedNodeId) || ((*it)->getPrescribedNodeId() != _sEditedNodeId))
    {
      NsDrugComponentsArray aLocalActiveComponents ;
      (*it)->getActiveComponents(&aLocalActiveComponents) ;

      if (false == aLocalActiveComponents.empty())
      {
        for (NsDrugComponentConstIter itLoc = aLocalActiveComponents.begin() ; aLocalActiveComponents.end() != itLoc ; itLoc++)
        {
          for (NsDrugComponentConstIter itGlo = aActiveComponents.begin() ; aActiveComponents.end() != itGlo ; itGlo++)
          {
            // The 2 drugs contain the same active principle
            //
            if ((*itGlo)->getBdmID() == (*itLoc)->getBdmID())
            {
              if (string("") != sMsg)
                sMsg += string(", ") ;
              sMsg += (*itLoc)->getLabel() + string(" (") + (*it)->getLabel() + string(")") ;
            }
          }
        }
      }
    }
  }

  if (string("") == sMsg)
    return ;

  string sWarning = pContexte->getSuperviseur()->getText("drugInformationAlerts", "activePrincipleRedundancy") ;
  sWarning += string(" ") + sMsg ;
  erreur(sWarning.c_str(), warningError, 0, GetHandle()) ;
}

void
NSPrescriptionBaseDlg::checkAllergies()
{
  if ((NsSelectableDrug*) NULL == _pBdmDrugInformation)
    return ;

  // Get components
  //
  NsDrugComponentsArray* pComponents = _pBdmDrugInformation->getComponents() ;

  if (((NsDrugComponentsArray*) NULL == pComponents) || pComponents->empty())
    return ;

  // Get allergies patpatho from the proper chapter in synthesis
  //
  NSPatPathoArray AllergiesArray(pContexte->getSuperviseur()) ;

  NSSearchStruct searchStruct ;
  string sSynthesisPath = string("ZSYNT/QANTP/EALMD") ;

  bool bOk = pContexte->getPatient()->ChercheChemin(sSynthesisPath, &searchStruct) ;
  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
    return ;

  string sDate = string("") ;
  string sNode = string("") ;

  MappingNSSearchResult::MMapIt it = searchStruct.getFoundNodes()->begin() ;
  searchStruct.getFoundNodes()->fullRData(it, sDate, sNode) ;

  pContexte->getPatient()->DonneArray(sNode, &AllergiesArray) ;

  if (AllergiesArray.empty())
    return ;

  // Search active components in allergies
  //
  NSEpiFlechiesDB flechiesDb(pContexte) ;

  string sMsg = string("") ;

  for (NsDrugComponentConstIter itGlo = pComponents->begin() ; pComponents->end() != itGlo ; itGlo++)
  {
    string sLabel = (*itGlo)->getLabel() ;

    // Find the Lexique code for the component label
    //
    string sLex = flechiesDb.getCodeLexiq(sLabel) ;

    if (string("") != sLex)
    {
      // If there is an allergy to X, there is an allergy to Y if (Y->is a->X)
      //
      VectString aIsA ;
	    pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sLex, "ES", &aIsA) ;

      for (PatPathoIter iterAllergy = AllergiesArray.begin() ; AllergiesArray.end() != iterAllergy ; iterAllergy++)
      {
        string sNodeSens = (*iterAllergy)->getLexiqueSens() ;

        bool bFound = false ;

        if (string("£??") == sNodeSens)
        {
          if (pseumaj(sLabel) == pseumaj((*iterAllergy)->getTexteLibre()))
            bFound = true ;
        }
        else if (aIsA.contains(sNodeSens) || aIsA.contains((*iterAllergy)->getLexique()))
          bFound = true ;

        if (bFound)
        {
          if (string("") != sMsg)
            sMsg += string(", ") ;
          sMsg += sLabel ;
        }
      }
    }
  }

  if (string("") == sMsg)
    return ;

  string sWarning = pContexte->getSuperviseur()->getText("drugInformationAlerts", "containAllergen") ;
  sWarning += string(" ") + sMsg ;
  erreur(sWarning.c_str(), warningError, 0, GetHandle()) ;
}

/**
 * Display the drug information dialog
 */
void
NSPrescriptionBaseDlg::drugInformation()
{
  if ((NsSelectableDrug*) NULL == _pBdmDrugInformation)
    return ;

  NSBdmDrugInfoDlg drugInfoDlg(this, pContexte, _pBdmDrugInformation) ;
  drugInfoDlg.Execute() ;
}

void
NSPrescriptionBaseDlg::initConnectedIssues(VecteurString* pLinkArray)
{
  if ((VecteurString*) NULL == pLinkArray)
    return ;

  _aLinkedIssues        = *pLinkArray ;
  _aUpdatedLinkedIssues = *pLinkArray ;

  setSelectedIssuesBtnState() ;
}

/**
 * Display the health issues dialog
 */
void
NSPrescriptionBaseDlg::selectIndication()
{
  // Get current concerns
  //
  ArrayConcern aCurrentConcerns((NSFrameInformation*) 0) ;
  getCurrentConcerns(&aCurrentConcerns) ;
  if (aCurrentConcerns.empty())
  {
    _pIssues->SetCheck(BF_UNCHECKED) ;
    return ;
  }

  // Initialize the array of selected elements
  //
  int* pSelectedIssues = new int[aCurrentConcerns.size()] ;

  for (size_t i = 0 ; i < aCurrentConcerns.size() ; i++)
    pSelectedIssues[i] = 0 ;

  int iSelectedCount = 0 ;

  if (false == _aUpdatedLinkedIssues.empty())
  {
    ArrayConcernIter it = aCurrentConcerns.begin() ;
    for (int iIndex = 0 ; aCurrentConcerns.end() != it ; it++, iIndex++)
      if (_aUpdatedLinkedIssues.ItemDansUnVecteur((*it)->getNoeud()))
        pSelectedIssues[iSelectedCount++] = iIndex ;
  }

  NSSelectIssuesDlg selectIssues(this, pContexte, &aCurrentConcerns, pSelectedIssues, iSelectedCount) ;
  int iExecReturn = selectIssues.Execute() ;

  if (IDOK != iExecReturn)
  {
    delete[] pSelectedIssues ;

    setSelectedIssuesBtnState() ;

    return ;
  }

  _aUpdatedLinkedIssues.vider() ;

  // The vector of selected arrayx contains _iSelectedCount elements.
  // Each element is the index of a selected element
  //
  if (selectIssues._iSelectedCount > 0)
  {
    int iArraySize = aCurrentConcerns.size() ;

	  for (size_t i = 0 ; i < selectIssues._iSelectedCount ; i++)
    {
      NSConcern *pConcern = (*selectIssues._pIssuesArray)[pSelectedIssues[i]] ;
      if (pConcern)
        addConnectedIssue(pConcern->getNoeud()) ;
    }
  }

  setSelectedIssuesBtnState() ;
}

void
NSPrescriptionBaseDlg::initializeConnectedIssues(string sDrugNode)
{
  if (string("") == sDrugNode)
    return ;

  NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
  if ((NSLinkManager*) NULL == pGraphe)
    return ;

  pGraphe->TousLesVrais(sDrugNode, NSRootLink::drugOf, &_aLinkedIssues) ;

  _aUpdatedLinkedIssues = _aLinkedIssues ;

  setSelectedIssuesBtnState() ;
}

void
NSPrescriptionBaseDlg::getCurrentConcerns(ArrayConcern *pCurrentConcerns)
{
  if ((ArrayConcern*) NULL == pCurrentConcerns)
    return ;

  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatEnCours)
    return ;

  ArrayConcern* pConcerns = pPatEnCours->getLdvDocument()->getConcerns(ldvframeHealth) ;
  if (((ArrayConcern*) NULL == pConcerns) || pConcerns->empty())
    return ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;

  for (ArrayConcernIter it = pConcerns->begin() ; pConcerns->end() != it ; it++)
    // If open now
    //
    if (((*it)->_tDateOuverture <= tNow) &&
        ((*it)->_tDateFermeture.estNoLimit() || ((*it)->_tDateFermeture > tNow)))
      pCurrentConcerns->push_back(new NSConcern(**it)) ;
}

bool
NSPrescriptionBaseDlg::isAConnectedIssue(string sConcernNode)
{
  return isAConnected(sConcernNode, &_aUpdatedLinkedIssues) ;
}

bool
NSPrescriptionBaseDlg::wasAConnectedIssue(string sConcernNode)
{
  return isAConnected(sConcernNode, &_aLinkedIssues) ;
}

bool
NSPrescriptionBaseDlg::isAConnected(string sConcernNode, VecteurString* pLinkArray)
{
  if (((VecteurString*) NULL == pLinkArray) || pLinkArray->empty())
    return false ;

  return pLinkArray->ItemDansUnVecteur(sConcernNode) ;
}

void
NSPrescriptionBaseDlg::addConnectedIssue(string sConcernNode)
{
  if (string("") == sConcernNode)
    return ;

  if (isAConnectedIssue(sConcernNode))
    return ;

  _aUpdatedLinkedIssues.AddString(sConcernNode) ;
}

void
NSPrescriptionBaseDlg::updateConnectedIssues(string sDrugNode)
{
  if (string("") == sDrugNode)
    return ;

  // The new list of connected issues is empty
  //
  if (_aUpdatedLinkedIssues.empty())
  {
    // If it was already the case, just leave
    //
    if (_aLinkedIssues.empty())
      return ;

    // if some concerns were already connected, we have to unconnect them
    //
    EquiItemIter it = _aLinkedIssues.begin() ;
    for ( ; _aLinkedIssues.end() != it ; it++)
      UnconnectIssue(sDrugNode, **it) ;
  }

  // Connect new linked concerns and unconnect no longer linked ones
  //
  EquiItemIter itNew = _aUpdatedLinkedIssues.begin() ;
  for ( ; _aUpdatedLinkedIssues.end() != itNew ; itNew++)
    if (false == wasAConnectedIssue(**itNew))
      ConnectIssue(sDrugNode, **itNew) ;

  EquiItemIter itOld = _aLinkedIssues.begin() ;
  for ( ; _aLinkedIssues.end() != itOld ; itOld++)
    if (false == isAConnectedIssue(**itOld))
      UnconnectIssue(sDrugNode, **itOld) ;
}

void
NSPrescriptionBaseDlg::ConnectIssue(string sDrugNode, string sConcernNode)
{
  if (string("") == sConcernNode)
    return ;

  NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

	pGraphe->etablirLien(sDrugNode, NSRootLink::drugOf, sConcernNode) ;
}

void
NSPrescriptionBaseDlg::UnconnectIssue(string sDrugNode, string sConcernNode)
{
  if (string("") == sConcernNode)
    return ;

  NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

	pGraphe->detruireLien(sDrugNode, NSRootLink::drugOf, sConcernNode) ;
}

void
NSPrescriptionBaseDlg::searchInBdmBySubstance()
{
  searchInBdm(NSBdmDriver::bamTableSubstance) ;
}

void
NSPrescriptionBaseDlg::searchInBdmByIndication()
{
  searchInBdm(NSBdmDriver::bamTableIndication) ;
}

void
NSPrescriptionBaseDlg::searchInBdmByATC()
{
  searchInBdm(NSBdmDriver::bamTableATC) ;
}

void
NSPrescriptionBaseDlg::searchInBdm(NSBdmDriver::BAMTABLETYPE iSearchBy)
{
  if (pContexte->getBamType() == NSContexte::btNone)
    return ;

  string sLexicode("") ;

  NSDrugResearchDlg* pSearchDlg = new NSDrugResearchDlg((TWindow*) this, pContexte, &sLexicode, iSearchBy) ;
  int iExecReturn = pSearchDlg->Execute() ;
  delete pSearchDlg ;

  if ((IDOK != iExecReturn) || (string("") == sLexicode))
    return ;

  _pDrug->setLabel(sLexicode) ;

  ExecutedAfterDrugSelection() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSOpenMultiDocsDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSSelectIssuesDlg, NSUtilDialog)
    EV_LV_DISPINFO_NOTIFY(IDC_LISTVIEW_ISSUES, LVN_GETDISPINFO, DispInfoIssuesList),
    EV_COMMAND(IDOK,            CmOk),
    EV_COMMAND(IDCANCEL,        CmCancel),
END_RESPONSE_TABLE;

NSSelectIssuesDlg::NSSelectIssuesDlg(TWindow* pPere, NSContexte* pCtx, ArrayConcern* pIssues, int* pSelectedIssues, int iSelectedCount)
                  :NSUtilDialog(pPere, pCtx, "SELECT_ISSUES")
{
try
{
  _pIssuesArray    = pIssues ;
  _pSelectedIssues = pSelectedIssues ;
  _iSelectedCount  = iSelectedCount ;
  _pIssuesList     = new NSIssuesListWindow(this, IDC_LISTVIEW_ISSUES) ;
}
catch (...)
{
  erreur("Exception NSSelectIssuesDlg ctor.",  standardError, 0) ;
}
}

NSSelectIssuesDlg::~NSSelectIssuesDlg()
{
  delete _pIssuesList ;
}

void
NSSelectIssuesDlg::SetupWindow()
{
  // ListView_SetExtendedListViewStyle(pObjectsList->HWindow, LVS_EX_FULLROWSELECT) ;

  NSUtilDialog::SetupWindow() ;

  InitIssuesList() ;
  DisplayIssuesList() ;

  if (false == _pIssuesArray->empty())
    _pIssuesList->SetSelIndexes(_pSelectedIssues, _iSelectedCount, true /* bool select */) ;
}

void
NSSelectIssuesDlg::dblClickOnIssue()
{
  CmOk() ;
}

void
NSSelectIssuesDlg::CmOk()
{
	if (_pIssuesArray->empty())
		return ;

	_iSelectedCount = _pIssuesList->GetSelIndexes(_pSelectedIssues, _pIssuesArray->size()) ;

	TDialog::CmOk() ;
}

void
NSSelectIssuesDlg::CmCancel()
{
  TDialog::CmCancel() ;
}

void
NSSelectIssuesDlg::InitIssuesList()
{
  TListWindColumn colExam("Examen", 300, TListWindColumn::Left, 0) ;
  _pIssuesList->InsertColumn(0, colExam) ;
	TListWindColumn colNombre("Date", 100, TListWindColumn::Left, 1) ;
  _pIssuesList->InsertColumn(1, colNombre) ;
}

void
NSSelectIssuesDlg::DisplayIssuesList()
{
	_pIssuesList->DeleteAllItems() ;
  if (_pIssuesArray->empty())
    return ;

  char cLibExam[255] = "" ;
  string sCodeExam, sLibExam ;

  ArrayConcernIter concernIt = _pIssuesArray->end() ;
  do
  {
    concernIt-- ;

    strcpy(cLibExam, (*concernIt)->getTitle().c_str()) ;
    TListWindItem Item(cLibExam, 0) ;
    _pIssuesList->InsertItem(Item) ;
  }
  while (_pIssuesArray->begin() != concernIt) ;
}

void
NSSelectIssuesDlg::DispInfoIssuesList(TLwDispInfoNotify& dispInfo)
{
	if (_pIssuesArray->empty())
  	return ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

	// const int 	    BufLen = 255;
	// static char     buffer[BufLen];
	// char			cCodeExam[80];
	// int			    occur;

	ArrayConcernIter concernIt = _pIssuesArray->begin() ;
	for (int i = 0 ; (i < index) && (_pIssuesArray->end() != concernIt) ; i++, concernIt++) ;
    if (concernIt == _pIssuesArray->end())
        return ;

	string sLang = string("") ;
	if (pContexte)
		sLang = pContexte->getUserLanguage() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // nombre d'examens

    	string sDate = (*concernIt)->_tDateOuverture.donneDate() ;
      sDate = donne_date(sDate, sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;
	}

	return ;
}

//***************************************************************************
//
//  					Méthodes de NSIssuesListWindow//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSIssuesListWindow, TListWindow)   EV_WM_LBUTTONDBLCLK,
   // EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

NSIssuesListWindow::NSIssuesListWindow(NSSelectIssuesDlg* parent, int id, TModule* module)
                   :TListWindow((TWindow *) parent, id, module)
{
  _pView = parent ;
  _iRes  = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  // Attr.ExStyle |= WS_EX_NOPARENTNOTIFY;
}
void
NSIssuesListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

  TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSIssuesListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)    _pView->CmOk() ;
}

//---------------------------------------------------------------------------//  Retourne l'index du premier item sélectionné
//---------------------------------------------------------------------------
int
NSIssuesListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

void
NSIssuesListWindow::EvSetFocus(HWND hWndLostFocus)
{
  _pView->EvSetFocus(hWndLostFocus) ;
}

std::string
initDispUnit(string sLang, string sCode, string sLib, NSContexte* pContexte)
{
	return initDispUnitFr(sCode, sLib, pContexte) ;
}

std::string
initDispUnitFr(string sCode, string sLib, NSContexte* pContexte)
{
	if ((string("") == sCode) && (string("") == sLib))
		return string("") ;

	//
  // Sachets
  //
  if (sLib.find("sach-dose") != NPOS)
  	return string("7SACH1") ;

	// Cuillérées
	//
	if ((sLib.find(" sol buv") != NPOS) || (sLib.find(" solution buvable")!= NPOS) )
	{
  	if ((sLib.find("gts")!= NPOS) || (sLib.find("gouttes")!= NPOS))
    	return string("2GOUT1") ;
    if ((sLib.find("sach")!= NPOS) || (sLib.find("sachet")!= NPOS))
    	return string("7SACH1") ;
    return string("2CUIC2") ;
  }
  if ((sLib.find(" susp buv") != NPOS) || (sLib.find(" suspension buvable")!= NPOS) )
  {
  	if ((sLib.find("sach")!= NPOS) || (sLib.find("sachet")!= NPOS))
    	return string("7SACH1") ;
  	return string("2CUIC2") ;
  }
  if (sLib.find("sirop") != NPOS)
  	return string("2CUIC2") ;
  //
  // Applications
  //
  if ((sLib.find(" pom ") != NPOS) ||  (sLib.find("pommade")!= NPOS) )
  	return string("GAPPL1") ;
  if (sLib.find(" crème ") != NPOS)
  	return string("GAPPL1") ;
  if ((sLib.find("appl cutanée") != NPOS)  ||  (sLib.find(" application cutanée")!= NPOS) )
  	return string("GAPPL1") ;
  if ((sLib.find("appl locale") != NPOS)   ||  (sLib.find(" application locale")!= NPOS) )
  	return string("GAPPL1") ;
  if (sLib.find(" gel ") != NPOS)
  	return string("GAPPL1") ;
  //
  // Comprimés vaginaux
  //
  if ((sLib.find(" cp vaginal") != NPOS) || (sLib.find(" comprimé vaginal") != NPOS))
  	return string("7CQMO1") ;
  //
  // Comprimés
  //
  if ((sLib.find(" cp pelliculé") != NPOS) || (sLib.find(" comprimé pelliculé")!= NPOS) )
  	return string("7CQMN1") ;
  if ((sLib.find(" cp enrobé") != NPOS) || (sLib.find(" comprimé enrobé")!= NPOS) )
  	return string("7CQMN1") ;
  if ((sLib.find(" cp efferv") != NPOS)  || (sLib.find(" comprimé effervescent")!= NPOS) )
  	return string("7CQEF1") ;
  if ((sLib.find(" cp sublingual") != NPOS) || (sLib.find(" comprimé sublingual")!= NPOS) )
  	return string("7CQSL1") ;
  if ((sLib.find(" cp dispersible") != NPOS)  || (sLib.find(" comprimé dispersible")!= NPOS) )
  	return string("7CQMN1") ;
  if ((sLib.find(" cp à croquer") != NPOS) || (sLib.find(" comprimé à croquer")!= NPOS) )
  	return string("7CQMN1") ;
  if ((sLib.find(" cp osmotique") != NPOS)  || (sLib.find(" comprimé osmotique")!= NPOS) )
  	return string("7CQMN1") ;
  if ((sLib.find(" cp séc ") != NPOS)   || (sLib.find(" comprimé sécable")!= NPOS) )
  	return string("7CQSE1") ;
  if ((sLib.find(" cp :") != NPOS)   || (sLib.find(" comprimé") != NPOS)  || (sLib.find(" cp ") != NPOS) )
  	return string("7CQMN1") ;

  //
  // Gellules
  //
  if ((sLib.find(" gél ") != NPOS)  || (sLib.find("gélule")!= NPOS) )
  	return string("7GELU1") ;
  //
  // Capsules
  //
  if ((sLib.find(" caps ") != NPOS) || (sLib.find("capsule")!= NPOS) )
  	return string("7CAPS1") ;

  //
  // Lyophilisat oral
  //
  if ((sLib.find(" lyoph oral ") != NPOS) || (sLib.find("lyophilisat oral")!= NPOS) )
  	return string("ILYOC2") ;

	//
	// Pastille et tablette
  //
  if (sLib.find("pastille")!= NPOS)
  	return string("IPAST2") ;
  if (sLib.find("tablette")!= NPOS)
  	return string("IPAST1") ;

  //
  // Ovules
  //
  if (sLib.find(" ovule") != NPOS)
  	return string("7OVUL1") ;
  //
  // Perfusion
  //
  if ((sLib.find(" perf IV") != NPOS) ||  (sLib.find("perfusion") != NPOS))
  	return string("GINFR1") ;
  //
  // Injection
  //
  if (sLib.find(" inj IV") != NPOS)
  	return string("GINJE1") ;
  if ((sLib.find(" sol inj") != NPOS) || (sLib.find("solution injectable")!= NPOS) )
  	return string("GINJE1") ;
  //
  // Pulvérisations
  //
  if ((sLib.find(" pulv buccale") != NPOS) || (sLib.find("pulverisation buccale")!= NPOS) || (sLib.find("pulvérisation buccale")!= NPOS) || (sLib.find(" fl préssurisé ")!= NPOS) )
  	return string("GPULV1") ;
  //
  // Inhalations
  //
  if (sLib.find(" inhalation ") != NPOS)
  	return string("NINHA1") ;
  //
  // Bains de bouche
  //
  if (sLib.find("bain de bouche") != NPOS)
  	return string("NBAIN1") ;
  //
  // Suppositoire
  //
  if ((sLib.find(" suppos ") != NPOS)  || (sLib.find("suppositoire") != NPOS ) )
  	return string("7SUPP1") ;

  //
  // Gouttes
  //
  if ((sLib.find("compte-gtes") != NPOS) || (sLib.find("compte-gouttes") != NPOS))
  	return string("2GOUT1") ;
  if (sLib.find("collyre") != NPOS)
  	return string("2GOUT1") ;

  //
  // Ampoule
  //
	if ((sLib.find(" amp ") != NPOS) || (sLib.find(" ampoule ") != NPOS) )
		return string("7AMPO1") ;

  //
  // Patch
  //
	if ((sLib.find(" patch ") != NPOS) || (sLib.find(" dispositif transdermique") != NPOS) )
		return string("IPATC1") ;

	return pContexte->getSuperviseur()->getText("drugDialog", "missingInformation") ;
}

// fin de nsmedicdlg.cpp

