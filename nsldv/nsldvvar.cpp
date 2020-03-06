// -----------------------------------------------------------------------------
// nsldvvar.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.20 $
// $Author: pameline $
// $Date: 2015/02/07 11:34:44 $
// -----------------------------------------------------------------------------
// Ligne de vie - Arrays d'objets de type Vue (au sens Document/Vue)
// Ligne de vie - Arrays of View objects (for Document/View model)
// -----------------------------------------------------------------------------
// FLP - aout 2003
// PA  - juillet 2003
// ...
// -----------------------------------------------------------------------------

#include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\gauge.h>

#include "nssavoir\nsBdmDriver.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "nsdn\nsdochis.h"
#include "nsldv\nsldvvar.h"
#include "nsldv\nsldvvue_base.h"
#include "nautilus\nsldvvuetech.h"
#include "nsldv\nsldvdoc_base.h"
#include "nautilus\nstrihis.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nstlibre.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\ns_fic.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nsmediccycle.h"
#include "nsbb\ns_cisp.h"
#include "nsbb\nsbbtran.h"
#include "nsepisod\nsclasser.h"
#include "nsepisod\nssoapdiv.h"
#include "nsepisod\nsToDo.h"
#include "nsepisod\nsepidiv.h"
#include "nsldv\nsldvstructs.h"

#include "curl\nsRest.h"
#include "nsoutil\ibdm.h"
#include "nsoutil\nsBdmDlg.h"
#include "nsoutil\nsBdmDrugInfoDlg.h"
#include "nsoutil\nsBdmListsDlg.h"
#include "nsoutil\nsdicobdm.h"
#include "nsutil\tinyxml.h"
#include "dcodeur/nsdkd.h"

#include "nautilus\nsldvvue.rh"
#include "nsbb\medicament.rh"

// --------------------------------------------------------------------------
// --------------------- METHODES DE ArrayPreoccupView ----------------------
// --------------------------------------------------------------------------

ArrayPreoccupView::ArrayPreoccupView(const ArrayPreoccupView& rv)
                  :ArrayPreocView()
{
try
{
	if (false == rv.empty())
  	for (ArrayPreoccupViewConstIter i = rv.begin() ; rv.end() != i ; i++)
    	push_back(new NSPreoccupView(*(*i))) ;
}
catch (...)
{
	erreur("Exception ArrayPreoccupView copy ctor.",  standardError, 0) ;
}
}


// va falloir revenir sur la méthode init pour l'index
void
ArrayPreoccupView::init(ArrayConcern* pConcerns, NSContexte* pCtx, NSLdvViewBase* pView, int iIdx)
{
/*
    if (!(pConcerns->empty()))
        for (ArrayPreoccuIter i = pPreoccupations->begin() ; i != pPreoccupations->end() ; i++)
            push_back(new NSPreoccupView(pCtx, pView, *i, iIdx));
*/
}

void
ArrayPreoccupView::vider()
{
	if (empty())
  	return ;

	for (ArrayPreoccupViewIter i = begin() ; end() != i ; )
	{
  	delete *i ;
    erase(i) ;
	}
}

ArrayPreoccupView::~ArrayPreoccupView()
{
	vider() ;
}

ArrayPreoccupView&
ArrayPreoccupView::operator=(const ArrayPreoccupView& src)
{
try
{
	if (this == &src)
  	return *this ;

	vider() ;

	if (false == src.empty())
  	for (ArrayPreoccupViewConstIter i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSPreoccupView(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayPreoccupView (=).",  standardError, 0) ;
	return *this ;
}
}

// --------------------------------------------------------------------------
// ---------------------- METHODES DE ArrayObjetsView -----------------------
// --------------------------------------------------------------------------

ArrayObjetsView::ArrayObjetsView(const ArrayObjetsView& rv)
                :ArrayObjView()
{
try
{
	if (false == rv.empty())
  	for (ArrayObjViewConstIter i = rv.begin() ; rv.end() != i ; i++)
    	push_back(new NSLdvObjetView(*(*i))) ;
}
catch (...)
{
	erreur("Exception ArrayObjetsView copy ctor.",  standardError, 0) ;
}
}

void
ArrayObjetsView::init(ArrayObjets* pObj, NSContexte* pCtx, NSLdvViewBase* pView)
{
try
{
	if (((ArrayObjets*) NULL == pObj) || pObj->empty())
		return ;

  NSLdvViewArea* pActiveWorkingArea = pView->getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pActiveWorkingArea)
    return ;

	for (ArrayObjIter i = pObj->begin() ; pObj->end() != i ; i++)
	{
		NSConcernView   *pPbView ;
		bool            bContinue = true ;

		for (ToonsIter PbIter = pView->_aToons.begin() ; (pView->_aToons.end() != PbIter) && bContinue ; PbIter++)
		{
			if (dynamic_cast<NSConcernView *>(*PbIter))
			{
				pPbView = (NSConcernView *)(*PbIter) ;
				if ((*i)->getConcern() == pPbView->getConcern()->_sReference)
					bContinue = false ;
			}
		}

		push_back(new NSLdvObjetView(pCtx, pActiveWorkingArea, *i, pPbView)) ;
	}
}
catch (...)
{
	erreur("Exception ArrayObjetsView::init.", standardError, 0) ;
}
}

void
ArrayObjetsView::vider()
{
	if (empty())
  	return ;

	for (ArrayObjViewIter i = begin() ; end() != i ; )
  {
  	delete *i ;
    erase(i) ;
  }
}

ArrayObjetsView::~ArrayObjetsView()
{
	vider() ;
}

ArrayObjetsView&
ArrayObjetsView::operator=(const ArrayObjetsView& src)
{
try
{
  if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
  	for (ArrayObjViewConstIter i = src.begin(); src.end() != i ; i++)
    	push_back(new NSLdvObjetView(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayObjetsView (=).",  standardError, 0) ;
	return *this ;
}
}

// -----------------------------------------------------------------
//
//  Méthodes de NSNewConcernDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSNewConcernDlg, NSUtilDialog)
  EV_COMMAND(IDOK,                        CmOk),
  EV_COMMAND(IDCANCEL,                    CmCancel),
  EV_COMMAND_AND_ID(IDR_CHRONIQUE,        UserName),
  EV_COMMAND_AND_ID(IDR_DANS,             UserName),
  EV_COMMAND_AND_ID(IDR_DUREE,            UserName),
  EV_COMMAND_AND_ID(IDR_LE,               UserName),
  EV_CHILD_NOTIFY_ALL_CODES(IDC_GRAVITE,  UpdateSeverity),
  EV_COMMAND(DUSOI_BTN,                   EvaluateDusoi),
  EV_COMMAND(IDC_COC,                     SetCocCodeVerbose),
  EV_COMMAND(IDC_CIM,                     SetCimCodeVerbose),
END_RESPONSE_TABLE;

NSNewConcernDlg::NSNewConcernDlg(NSLdvViewBase* pView, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo)
                :NSUtilDialog((TWindow*) pView, pCtx, (pCtx->getBamType() == NSContexte::btNone) ? "NEW_PROBLEM" : "NEW_PROBLEM_BAM", pCtx->GetMainWindow()->GetModule()),
                 _PptDetails(pCtx->getSuperviseur())
{
try
{
  _pLdvView = pView ;
  _pLdvDoc  = (NSLdvDocument*) 0 ;

  _sCocCode = string("") ;
  _sCimCode = string("") ;

  _pNewConcernInfo = pNewConcernInfo ;
  InitFromInfo() ;

  CreateControls() ;

/*
  if ((strlen(sLabel->c_str()) > 0) && (sLabel->rfind(")") == strlen(sLabel->c_str()) - 1))
  {
		int iPos = sLabel->rfind("(") ;
		sLabel->replace(iPos - 1, NPOS, "\0") ;
  }
*/

  _bDontHandleRadio = false ;
}
catch (...)
{
  erreur("Exception NSNewConcernDlg ctor 1.",  standardError, 0) ;
}
}

NSNewConcernDlg::NSNewConcernDlg(TWindow* pPere, NSLdvDocumentBase* pDoc, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo)
                :NSUtilDialog(pPere, pCtx, (pCtx->getBamType() == NSContexte::btNone) ? "NEW_PROBLEM" : "NEW_PROBLEM_BAM", pCtx->GetMainWindow()->GetModule()),
                 _PptDetails(pCtx->getSuperviseur())
{
try
{
  _pLdvView = (NSLdvView*) 0 ;
  _pLdvDoc  = pDoc ;

  _sCocCode = string("") ;
  _sCimCode = string("") ;

  _pNewConcernInfo = pNewConcernInfo ;
  InitFromInfo() ;

  CreateControls() ;

  _bDontHandleRadio = false ;
}
catch (...)
{
	erreur("Exception NSNewConcernDlg ctor 2.",  standardError, 0) ;
}
}

void
NSNewConcernDlg::InitFromInfo()
{
  if ((NewConcernInfo*) NULL == _pNewConcernInfo)
    return ;

  _sCocCode = _pNewConcernInfo->getCocCode() ;
  _sCimCode = _pNewConcernInfo->getCimCode() ;

  NSPatPathoArray* pNCIPatho = _pNewConcernInfo->getDetails() ;
  if (pNCIPatho)
    _PptDetails = *pNCIPatho ;
}

void
NSNewConcernDlg::CreateControls()
{
try
{
  _pType        = new NSUtilLexique(pContexte, this, PREOCCUP_EDIT, pContexte->getDico()) ;
  _pType->setLostFocusFunctor(new MemFunctor<NSNewConcernDlg>((NSNewConcernDlg*) this, &NSNewConcernDlg::SetCocCodeNoVerbose)) ;

  _pDateDebText = new OWL::TStatic(this, DATE_DEB_TEXT) ;
  _pDateDeb     = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_DEB) ;
  _pGroupFin    = new TGroupBox(this, DATE_FIN_TEXT) ;
  _pDateFin     = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_FIN) ;
  _pNbJours     = new NSEditNum(pContexte, this, IDC_NBJOURS, 10) ;
  _pRChronique  = new OWL::TRadioButton(this, IDR_CHRONIQUE) ;
  _pRDans       = new OWL::TRadioButton(this, IDR_DANS) ;
  _pRDuree      = new OWL::TRadioButton(this, IDR_DUREE) ;
  _pJoursText   = new OWL::TStatic(this, JOURS_TEXT) ;
  _pRLe         = new OWL::TRadioButton(this, IDR_LE) ;
  _pGravite 	  = new OWL::THSlider(this, IDC_GRAVITE) ;

  _pSeverityText = new OWL::TStatic(this, SEVER_TEXT) ;
  _pGravGauge    = new OWL::TGauge(this, "%d%%", IDC_PROGRESSGRAV, 18, 182, 300, 20) ;
  _pGravGauge->SetRange(0, 100) ;
  _pGravGauge->SetNativeUse(nuNever) ;

  _pDusoiButton = new OWL::TButton(this, DUSOI_BTN) ;
  _bDusoiWasCalculated = false ;
  _iDusoiValue         = 0 ;
  _iSymptomValue       = 0 ;
  _iComplicationValue  = 0 ;
  _iPrognosisValue     = 0 ;
  _iTreatabilityValue  = 0 ;

  _pCocButton = new OWL::TButton(this, IDC_COC) ;
  _sClassif  = string("6CISP") ;
  _sPostCase = string("0SOA41") ;

  if (pContexte->getBamType() != NSContexte::btNone)
    _pCimButton = new OWL::TButton(this, IDC_CIM) ;
  else
    _pCimButton = (OWL::TButton*) 0 ;
}
catch (...)
{
	erreur("Exception NSNewConcernDlg::CreateControls",  standardError, 0) ;
}
}

NSNewConcernDlg::~NSNewConcernDlg()
{
  delete _pType ;
  delete _pDateDebText ;
  delete _pDateDeb ;
  delete _pGroupFin ;
  delete _pDateFin ;
  delete _pNbJours ;
  delete _pRChronique ;
  delete _pRDans ;
  delete _pRDuree ;
  delete _pJoursText ;
  delete _pRLe ;
  delete _pGravite ;
  delete _pSeverityText ;
  delete _pGravGauge ;
  delete _pDusoiButton ;
  delete _pCocButton ;

  if (_pCimButton)
    delete _pCimButton ;
}

void
NSNewConcernDlg::SetupWindow()
{
	TDialog::SetupWindow() ;

  // Initialize starting date
  //

	NVLdVTemps tpsNow ;
	tpsNow.takeTime() ;

  string sDateDeb = string("") ;
  if (_pNewConcernInfo)
    sDateDeb = _pNewConcernInfo->getStartDate() ;

	if (string("") != sDateDeb)
	{
		// We must normalize the incoming date
		NVLdVTemps tpsNormalizer ;
    if (tpsNormalizer.initFromDate(sDateDeb))
		{
    	tpsNormalizer.normalize() ;
			_pDateDeb->setDate(tpsNormalizer.donneDateHeure()) ;
		}
    else
			_pDateDeb->setDate(tpsNow.donneDateHeure()) ;
	}
	else
		_pDateDeb->setDate(tpsNow.donneDateHeure()) ;

	_pDateFin->setDate(tpsNow.donneDateHeure()) ;

  // Fix "Continuity of care code" label early so that it can be replaced
  // by the real information if
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sText = pSuper->getText("healthConcernDialog", "continuityOfCareCode") ;
  _pCocButton->SetCaption(sText.c_str()) ;

  if (_pCimButton)
  {
    sText = pSuper->getText("healthConcernDialog", "cimCode") ;
    _pCimButton->SetCaption(sText.c_str()) ;
  }

  // Initialize concern's concept
  //

  string sConcernCode = string("") ;
  if (_pNewConcernInfo)
    sConcernCode = _pNewConcernInfo->getLexique() ;

	if (string("") != sConcernCode)
  {
		_pType->setLabel(sConcernCode) ;

    if (string("") == _sCocCode)
      FindCocCode(false) ;
    else
      DisplayCocCode() ;

    FindDuration() ;
    _pNbJours->EvKillFocus(0) ;
  }
	else if (_pNewConcernInfo)
  {
    string sFreeText = _pNewConcernInfo->getFreeText() ;
		_pType->SetText(sFreeText.c_str()) ;
  }
  else
    _pType->SetText("") ;

  // If FindDuration() let the duration empty, then check as chronic
  //
  _pNbJours->donneValeur() ;
  if (0 == _pNbJours->_dValeur)
  {
	  _pNbJours->SetText("") ;

	  _pRChronique->SetCheck(BF_CHECKED) ;
	  _pDateFin->SetReadOnly(true) ;
	  _pNbJours->SetReadOnly(true) ;
  }

	_pGravite->SetRange(0, 100) ;   //fixer min et max
	_pGravite->SetRuler(5, false) ; //espacement entre deux graduations
	_pGravite->SetPosition(10) ;

  int iSeverity = 10 ;
  if (_pNewConcernInfo)
    iSeverity = _pNewConcernInfo->getSeverity() ;

	_pGravGauge->SetValue(iSeverity) ;

  // Texts
  //
  string sTitle = pSuper->getText("healthConcernDialog", "newHealthConcern") ;
  SetCaption(sTitle.c_str()) ;

  sText = pSuper->getText("healthConcernDialog", "startingDate") ;
  _pDateDebText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "endingDate") ;
  _pGroupFin->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "undetermined") ;
  _pRChronique->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "fromNow") ;
  _pRDans->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "totalDuration") ;
  _pRDuree->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "onThe") ;
  _pRLe->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "days") ;
  _pJoursText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "severityIndex") ;
  _pSeverityText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "dusoi") ;
  _pDusoiButton->SetCaption(sText.c_str()) ;
}

void
NSNewConcernDlg::ResultCons()
{
}

void
NSNewConcernDlg::UserName(WPARAM wParam)
{
  if (_bDontHandleRadio)
    return ;

  switch (wParam)
  {
    case (IDR_CHRONIQUE) :
    	_pDateFin->SetReadOnly(true) ;
      _pNbJours->SetReadOnly(true) ;
      break ;
    case (IDR_DANS) :
    	_pDateFin->SetReadOnly(true) ;
      _pNbJours->SetReadOnly(false) ;
      _pNbJours->SetFocus() ;
      break ;
    case (IDR_DUREE) :
    	_pDateFin->SetReadOnly(true) ;
      _pNbJours->SetReadOnly(false) ;
      _pNbJours->SetFocus() ;
      break ;
    case (IDR_LE) :
    	_pNbJours->SetReadOnly(true) ;
      _pDateFin->SetReadOnly(false) ;
      _pDateFin->SetFocus() ;
      break ;
  }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSNewConcernDlg::CmOk()
{
try
{
  // on récupère d'abord un éventuel élément lexique sélectionné par les flêches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (_pType->getDico()->getDicoDialog()->EstOuvert())
  {
    _pType->getDico()->getDicoDialog()->InsererElementLexique() ;
    return ;
  }

  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

  string sTL = string("") ;

  // Ne pas accepter les textes libres
  if (_pType->getCode() == string("£?????"))
  {
    if (false == pContexte->getEpisodus()->bAllowFreeTextLdV)
    {
      string sWarningTxt = pContexte->getSuperviseur()->getText("lexiqueManagement", "freeTextNotAllowed") ;
      erreur(sWarningTxt.c_str(), warningError, 0, GetHandle()) ;
      _pType->SetFocus() ;
      return ;
    }

    int       iBuffLen  = _pType->GetTextLen() ;
    char far  *szBuff   = new char[iBuffLen + 1] ;
    _pType->GetText(szBuff, iBuffLen + 1) ;
    sTL = string(szBuff) ;
    delete[] szBuff ;

    if (_pNewConcernInfo)
      _pNewConcernInfo->setFreeText(sTL) ;
  }

  // Starting date
  //
  string sDateDeb ;
  _pDateDeb->getDate(&sDateDeb) ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setStartDate(sDateDeb) ;

  // Ending date
  //
  string sDateFin ;

  // récupération de la valeur du champs edit correspondant au nombre de jours
  _pNbJours->donneValeur() ;

  // si on est dans le cas où "chronique" est cochée
  if (_pRChronique->GetCheck() == BF_CHECKED)
    sDateFin = "00000000000000" ;

  // si on dans le cas où "dans" est cochée
  if (_pRDans->GetCheck() == BF_CHECKED)
  {
    if (_pNbJours->getValue() < 1)
    {
      erreur("La préoccupation doit durer au moins 1 jour.", standardError, 0, GetHandle()) ;
      return ;
    }

    NVLdVTemps tDateFin ;
    tDateFin.takeTime() ;
    tDateFin.ajouteJours(_pNbJours->getValue()) ;
    sDateFin = tDateFin.donneDateHeure() ;
  }

  // si on est dans le cas où "durée" est cochée
  if (_pRDuree->GetCheck() == BF_CHECKED)
  {
    if (_pNbJours->getValue() < 1)
    {
      erreur("La préoccupation doit durer au moins 1 jour.", standardError, 0, GetHandle()) ;
      return ;
    }

    NVLdVTemps tDateFin ;
    tDateFin.initFromDate(sDateDeb) ;
    tDateFin.ajouteJours(_pNbJours->getValue()) ;
    sDateFin = tDateFin.donneDateHeure() ;
  }

  // si on est dans le cas où "le" est cochée
  if (_pRLe->GetCheck() == BF_CHECKED)
    _pDateFin->getDate(&sDateFin) ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setEndDate(sDateFin) ;

  // Severity
  //
  int iGravite = _pGravite->GetPosition() ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setSeverity(iGravite) ;

  // Coc code
  //
  if (_pNewConcernInfo)
  {
    _pNewConcernInfo->setCocCode(_sCocCode) ;
    _pNewConcernInfo->setCimCode(_sCimCode) ;
  }

  // Concern code
  //
  string sConcernCode = _pType->getCode() ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setLexique(sConcernCode) ;

  // Details
  //
  if (_pNewConcernInfo)
    _pNewConcernInfo->setDetails(_PptDetails) ;

/*
  NSLdvDocument *pDoc = pLdvDoc ;
  if ((NULL == pDoc) && pLdvView)
    pDoc = pLdvView->getDoc() ;

  if (NULL == pDoc)
    return ;

  bool bResult =  pDoc->newLine(ldvframeHealth, pType->getCode(), psNewNode, sDateDeb, sDateFin, iGravite, *psCocCode, pPtDetails, sTL) ;

  if (psType)
    *psType = pType->getCode() ;
  if (psCode)
    *psCode = pType->getCode() ;
*/

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSNewConcernDlg::CmOk.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSNewConcernDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSNewConcernDlg::UpdateSeverity(uint)
{
	int setting = _pGravite->GetPosition() ;

	if      (setting < 25)
		_pGravGauge->SetColor(NS_CLASSLIB::TColor(0, 0, 128)) ;   // Blue
	else if (setting < 50)
		_pGravGauge->SetColor(NS_CLASSLIB::TColor(0, 128, 0)) ;   // Green
	else if (setting < 75)
		_pGravGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 0)) ; // Yellow
	else
		_pGravGauge->SetColor(NS_CLASSLIB::TColor(128, 0, 0)) ;   // Red

	_pGravGauge->SetValue(setting) ;
}

/*
void
NSNewConcernDlg::UpdateRisk(uint)
{
    int setting = pRisque->GetPosition();

    if      (setting < 10)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(0, 0, 128));
    else if (setting < 15)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(0, 128, 0));
    else if (setting < 20)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 0));
    else
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(128, 0, 0));

    pRiskGauge->SetValue(setting);
} */

void
NSNewConcernDlg::SetDetails()
{
try
{
# ifdef __OB1__
	NSSmallBrother BigBoss(pContexte, &_PptDetails, 0) ;
#else
	NSSmallBrother BigBoss(pContexte, &_PptDetails, 0, false /* initFromBbk */) ;
#endif
	BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
	// on lance l'archetype lié au code médicament (avec fil guide associé)
	BigBoss.lance12("0PRO11", "0DETA1") ;
}
catch (...)
{
	erreur("Exception NSNewConcernDlg::SetDetails.",  standardError, 0) ;
}
}

void
NSNewConcernDlg::EvaluateDusoi()
{
try
{
  int iDusoi = _iDusoiValue ;
  int iSympt = _iSymptomValue ;
  int iCompl = _iComplicationValue ;
  int iProgn = _iPrognosisValue ;
  int iTreat = _iTreatabilityValue ;

  NSDusoiDlg* pDusoiDialog = new NSDusoiDlg(this, pContexte, &iDusoi, &iSympt,
                                            &iCompl, &iProgn, &iTreat) ;
  int iExecReturn = pDusoiDialog->Execute() ;
  delete pDusoiDialog ;

  if (IDOK != iExecReturn)
    return ;

  _iDusoiValue        = iDusoi ;
  _iSymptomValue      = iSympt ;
  _iComplicationValue = iCompl ;
  _iPrognosisValue    = iProgn ;
  _iTreatabilityValue = iTreat ;

  _bDusoiWasCalculated = true ;

  _pGravite->SetPosition(_iDusoiValue) ;
  UpdateSeverity(_iDusoiValue) ;
	// pGravGauge->SetValue(iSeverity) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSNewConcernDlg::EvaluateDusoi : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSNewConcernDlg::EvaluateDusoi.",  standardError, 0) ;
}
}

void
NSNewConcernDlg::SetCocCodeVerbose()
{
  if (string("") == _sCocCode)
  {
    FindCocCode(true) ;
    return ;
  }

  string sLabel = _pType->getLabel() ;
  string sResP  = string("") ;
  SOAPObject SOAPObj(sLabel, sResP, _sClassif, 0, string("")) ;

  NSEpiClassifInfoArray aArrayClassif ;
  int    iInitialLevel = classifExpert::niveauTous ;
  string sCtrlData     = string("") ;

  classifExpert CispExpert(pContexte) ;
  CispExpert.donneClassifArray(&SOAPObj, &aArrayClassif, iInitialLevel) ;

  CISPFindCodeDlg* pFindCodeDlg
                            = new CISPFindCodeDlg(this,
                                                  pContexte,
                                                  &SOAPObj,
                                                  iInitialLevel,
                                                  &aArrayClassif,
                                                  &sCtrlData) ;
  int iResult = pFindCodeDlg->Execute() ;

  if (IDOK == iResult)
  {
    _sCocCode = pFindCodeDlg->getResult() ;
    DisplayCocCode() ;
  }

  delete pFindCodeDlg ;
}

void
NSNewConcernDlg::SetCocCodeNoVerbose()
{
  FindCocCode(false) ;
  FindCimCode(false) ;
  FindDuration() ;
}

void
NSNewConcernDlg::DisplayCocCode()
{
  if (string("") == _sCocCode)
  {
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    _pCocButton->SetCaption(sText.c_str()) ;
    return ;
  }

  NSEpiClassifDB   dbClassif(pContexte) ;
  NSEpiClassifInfo classifInfo ;
  bool bFoundCode = dbClassif.searchCode(_sClassif, _sCocCode, &classifInfo) ;

  string sCaption = _sCocCode ;
  if (bFoundCode)
    sCaption += string(" - ") + classifInfo.getLabel() ;

  _pCocButton->SetCaption(sCaption.c_str()) ;
}

void
NSNewConcernDlg::FindCocCode(bool bVerbose)
{
try
{
  string sHealthConcern = _pType->getCode() ;
  if (string("") == sHealthConcern)
    return ;

  string sLabel = _pType->getLabel() ;

  NSLdvDocumentBase *pDoc = _pLdvDoc ;
  if (((NSLdvDocumentBase*) NULL == pDoc) && _pLdvView)
    pDoc = _pLdvView->getDoc() ;

  if ((NSLdvDocumentBase*) NULL == pDoc)
    return ;

  string sGoodCode = string("") ;

  pDoc->findCocCode(bVerbose, sGoodCode, sHealthConcern, sLabel, _sClassif, _sPostCase) ;

  if (string("") == sGoodCode)
  {
    _sCocCode = string("") ;
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    _pCocButton->SetCaption(sText.c_str()) ;
  	return ;
  }

  _sCocCode = sGoodCode ;

  DisplayCocCode() ;

  return ;

/*
  string sHealthConcern = pType->getCode() ;
  if (string("") == sHealthConcern)
    return ;

  string sConcept ;
  pContexte->getDico()->donneCodeSens(&sHealthConcern, &sConcept) ;

  string sResO = string("") ;
  string sResP = string("") ;
  string sResI = string("") ;
  string sRes3 = string("") ;

  string sDomain = string("") ;

//  if (sConcept == string("£??"))
//  {
//  	string sLibelle  = pType->sTexteGlobal ;
//    string sLocalize = pNSTreeNode->getPosition() ;
//
//    ParseSOAP Parser(pContexte, &sClassif) ;
//    Parser.computeParsing(&sLibelle, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;
//  }
//  else
//  {

  	Classify ClassifTool(pContexte, &_sClassif, &sConcept, &_sPostCase) ;
  	ClassifTool.computeParsing(&sResO, &sResP, &sResI) ;

    // If Classer has found 'Mandatory code(s)', then we must take it
    //
    if (string("") != sResO)
      sDomain = sResO ;

    // Else, 3BT is the gold standard, so, we have to use it
    //
    else
    {
      string sClasserDomain  = sResP ;

      //
      //
      string sLabel = pType->getLabel() ;
      string sLocalize = _sPostCase ;

      ParseSOAP Parser(pContexte, &_sClassif) ;
      Parser.computeParsing(&sLabel, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;

      if (string("") != sResP)
        sDomain  = sResP ;
      else
        sDomain  = sClasserDomain ;
    }

//  }

  //
  // On trouve le code
  //
  NSEpisodus* pEpisodus = pContexte->getSuperviseur()->getEpisodus() ;

  classifExpert* pExpert = pEpisodus->pClassifExpert ;
  if (NULL == pExpert)
    return ;

  NSEpiClassifInfoArray arrayClassif ;

  ElemSetArray* pElemDomain = 0 ;
  //
  // On instancie le domaine
  // Instanciating the domain
  //
  ParseCategory Parser(pExpert->donneCodeSize(_sClassif), _sClassif,
                                             pExpert->donnePattern(_sClassif)) ;
  pElemDomain = Parser.DefDomain(sDomain) ;
  //
  // On trouve les codes qui correspondent au domaine
  // Finding the codes that belong to the domain
  //
  string sCaseSens ;
  pContexte->getDico()->donneCodeSens(&_sPostCase, &sCaseSens) ;

  pExpert->fillList(_sClassif, pElemDomain, &arrayClassif, sCaseSens) ;

  if (pElemDomain)
    delete pElemDomain ;

  int      iBuffLen = pType->GetTextLen() ;
  char far *szBuff  = new char[iBuffLen + 1] ;
  pType->GetText(szBuff, iBuffLen + 1) ;

  string sLabel = string(szBuff) ;

  string sGoodCode = "" ;
  //
  //
  //
  if (arrayClassif.size() != 1)
  {
    if (bVerbose)
    {
  	  SOAPObject SOAPObj(sLabel, sResP, _sClassif, 0, string("")) ;
#ifndef _EXT_CAPTURE
		  NSCapture Capture(pContexte) ;
#else
		  NSCapture Capture ;
#endif

		  Capture.sClassifResultO = sResO ;
      Capture.sClassifResultP = sResP ;
      Capture.sClassifResultI = sResI ;
      Capture.sClassifResult3 = sRes3 ;

      SOAPObj.pCaptElemnt = &Capture ;
      SOAPObj.sCase       = sCaseSens ;

      sGoodCode = pExpert->chooseCode(&SOAPObj) ;
    }
  }
  else
  {
  	sGoodCode = (*(arrayClassif.begin()))->pDonnees->code ;

    string sCtrlData = "" ;
    NVLdVTemps tpsDebut ;
    tpsDebut.takeTime() ;
    pExpert->setControlString(&sCtrlData, _sClassif, sCaseSens, sLabel,
                                  classifExpert::niveauPreselection, 0,
                                  &tpsDebut, &tpsDebut, sGoodCode, sConcept) ;
    pExpert->storeControlData(sCtrlData) ;
  }

  if (string("") == sGoodCode)
  {
    *psCocCode = string("") ;
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    pCocButton->SetCaption(sText.c_str()) ;
  	return ;
  }

  *psCocCode = sGoodCode ;

  DisplayCocCode() ;
*/
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSNewConcernDlg::FindCocCode : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSNewConcernDlg::FindCocCode.",  standardError, 0) ;
}
}

void
NSNewConcernDlg::SetCimCodeVerbose()
{
  if ((OWL::TButton*) NULL == _pCimButton)
    return ;

  if (string("") == _sCimCode)
  {
    FindCimCode(true) ;
    return ;
  }

  FindCimCode(true) ;
}

void
NSNewConcernDlg::DisplayCimCode()
{
  if ((OWL::TButton*) NULL == _pCimButton)
    return ;

  if (string("") == _sCimCode)
  {
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "cimCode") ;
    _pCimButton->SetCaption(sText.c_str()) ;
    return ;
  }

  NSEpiClassifDB   dbClassif(pContexte) ;
  NSEpiClassifInfo classifInfo ;
  bool bFoundCode = dbClassif.searchCode(string("6CIMA"), _sCimCode, &classifInfo) ;

  string sCaption = _sCimCode ;
  if (bFoundCode)
    sCaption += string(" - ") + classifInfo.getLabel() ;

  _pCimButton->SetCaption(sCaption.c_str()) ;
}

void
NSNewConcernDlg::FindCimCode(bool bVerbose)
{
  if ((OWL::TButton*) NULL == _pCimButton)
    return ;

try
{
  string sSeed = pseumaj(_pType->getLabel()) ;
  if (string("") == sSeed)
  {
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "cimCode") ;
    _pCimButton->SetCaption(sText.c_str()) ;
    _sCimCode = string("") ;
    return ;
  }

  // Get BDM driver
  //
  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if ((NSBdmDriver*) NULL == pDriver)
    return ;

  if (false == bVerbose)
  {
    NSBdmEntryArray aListeArray ;
    pDriver->getListForSeed(&aListeArray, &sSeed, NSBdmDriver::bamTableCim10) ;

    if (aListeArray.empty())
    {
      string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "cimCode") ;
      _pCimButton->SetCaption(sText.c_str()) ;
      _sCimCode = string("") ;
      return ;
    }

    sort(aListeArray.begin(), aListeArray.end(), BdmEntrySortByIdInf) ;

    NSBdmEntry* pFirst = *(aListeArray.begin()) ;

    _sCimCode = pFirst->getCode() ;

    string sCaption = _sCimCode + string(" - ") + pFirst->getLabel() ;
    _pCimButton->SetCaption(sCaption.c_str()) ;

    return ;
  }

  NSBdmEntry result ;
  ChoixBdmListDialog bdmList(this, pDriver, NSBdmDriver::bamTableCim10, &result, sSeed) ;
  int iExecReturn = bdmList.Execute() ;

  if (IDOK != iExecReturn)
    return ;

  _sCimCode = result.getCode() ;

  // DisplayCimCode() ;

  string sCaption = _sCimCode + string(" - ") + result.getLabel() ;

  _pCimButton->SetCaption(sCaption.c_str()) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSNewConcernDlg::FindCimCode : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSNewConcernDlg::FindCimCode.",  standardError, 0) ;
}
}

void
NSNewConcernDlg::FindDuration()
{
  string sHealthConcern = _pType->getCode() ;
  if (string("") == sHealthConcern)
    return ;

  string sConcept ;
  pContexte->getDico()->donneCodeSens(&sHealthConcern, &sConcept) ;

  NSPatPathoArray ficheTree(pContexte->getSuperviseur()) ;

  //
	// Accessing Fiches' table
	//
	NSSavFiche Fiches(pContexte->getSuperviseur()) ;
	//
	// Opening table
	//
	Fiches.lastError = Fiches.open() ;
	if (DBIERR_NONE != Fiches.lastError)
	{
		erreur("Erreur à l'ouverture du fichier de données.", standardError, Fiches.lastError) ;
		return ;
	}
  //
  // Looking for first table
  //
  string sCle = sConcept + string(BASE_LOCALISATION_LEN, ' ') ;

  Fiches.lastError = Fiches.chercheClef(&sCle,
                                        "",
                                        0,
                                        keySEARCHGEQ,
                                        dbiWRITELOCK) ;
  if (DBIERR_NONE != Fiches.lastError)
	{
    Fiches.close() ;
		return ;
	}

  Fiches.lastError = Fiches.getRecord() ;
  if (DBIERR_NONE != Fiches.lastError)
	{
		erreur("Le fichier de Fiches est défectueux.", standardError, Fiches.lastError) ;
    Fiches.close() ;
		return ;
	}
  if (Fiches.getSens() != sConcept)
  {
    // erreur("Il n'existe pas de fiche pour cet élément.", 0, pCR->lastError);
    Fiches.close() ;
		return ;
  }
  //
  // On avance dans le fichier tant que les fiches trouvées appartiennent
  // à ce compte rendu
  //
  while ((DBIERR_EOF != Fiches.lastError) &&
   		   (Fiches.getSens() == sConcept))
  {
    ficheTree.push_back(new NSPatPathoInfo(&Fiches)) ;

    Fiches.lastError = Fiches.suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != Fiches.lastError) && (DBIERR_EOF != Fiches.lastError))
    {
      erreur("Pb d'accès à la l'enregistrement suivant de la base des Fiches.", standardError, Fiches.lastError) ;
      Fiches.close() ;
			return ;
		}

    if (Fiches.lastError != DBIERR_EOF)
    {
      Fiches.lastError = Fiches.getRecord() ;
      if (Fiches.lastError != DBIERR_NONE)
			{
        erreur("Le fichier de données est défectueux.", standardError, Fiches.lastError) ;
        Fiches.close() ;
				return ;
      }
    }
  }
  //
  // Fermeture du fichier
  //
  Fiches.close() ;

  if (ficheTree.empty())
    return ;

  PatPathoIter pptIt = ficheTree.ChercherItem(string("0DEFI1/VDURE1"), string("/")) ;
  if ((NULL == pptIt) || (ficheTree.end() == pptIt))
    return ;

  pptIt++ ;
  if (ficheTree.end() == pptIt)
    return ;

  string sNbDays = (*pptIt)->getComplement() ;

  _bDontHandleRadio = true ;

  _pRChronique->Uncheck() ;
  _pRDans->Uncheck() ;
  _pRDuree->Check() ;
  _pDateFin->SetReadOnly(true) ;
  _pNbJours->SetReadOnly(false) ;
  _pNbJours->SetText(sNbDays.c_str()) ;
  // pNbJours->SetFocus() ;

  _bDontHandleRadio = false ;
}

// Risk

// -----------------------------------------------------------------
//
//  Méthodes de NSNewRiskDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSNewRiskDlg, NSUtilDialog)
  EV_COMMAND(IDOK,                        CmOk),
  EV_COMMAND(IDCANCEL,                    CmCancel),
  EV_COMMAND_AND_ID(IDR_CHRONIQUE,        UserName),
  EV_COMMAND_AND_ID(IDR_DANS,             UserName),
  EV_COMMAND_AND_ID(IDR_DUREE,            UserName),
  EV_COMMAND_AND_ID(IDR_LE,               UserName),
  EV_COMMAND(IDC_COC,                     SetCocCodeVerbose),
END_RESPONSE_TABLE;

NSNewRiskDlg::NSNewRiskDlg(NSLdvViewBase* pView, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo)
             :NSUtilDialog((TWindow*) pView, pCtx, "NEW_RISK", pCtx->GetMainWindow()->GetModule()),
              _PptDetails(pCtx->getSuperviseur())
{
try
{
  pLdvView  = pView ;
  pLdvDoc   = (NSLdvDocument*) 0 ;

  _pNewConcernInfo = pNewConcernInfo ;

  _sCocCode = string("") ;
  InitFromInfo() ;

  CreateControls() ;

/*
  if ((strlen(sLabel->c_str()) > 0) && (sLabel->rfind(")") == strlen(sLabel->c_str()) - 1))
  {
		int iPos = sLabel->rfind("(") ;
		sLabel->replace(iPos - 1, NPOS, "\0") ;
  }
*/
}
catch (...)
{
  erreur("Exception NSNewRiskDlg ctor.",  standardError, 0) ;
}
}

NSNewRiskDlg::NSNewRiskDlg(TWindow* pPere, NSLdvDocument* pDoc, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo)
             :NSUtilDialog(pPere, pCtx, "NEW_RISK", pCtx->GetMainWindow()->GetModule()),
              _PptDetails(pCtx->getSuperviseur())
{
try
{
  pLdvView  = (NSLdvView*) 0 ;
  pLdvDoc   = pDoc ;

  _pNewConcernInfo = pNewConcernInfo ;

  _sCocCode = string("") ;
  InitFromInfo() ;

  CreateControls() ;
}
catch (...)
{
	erreur("Exception NSNewRiskDlg ctor.",  standardError, 0) ;
}
}

void
NSNewRiskDlg::InitFromInfo()
{
  if ((NewConcernInfo*) NULL == _pNewConcernInfo)
    return ;

  _sCocCode = _pNewConcernInfo->getCocCode() ;

  NSPatPathoArray* pNCIPatho = _pNewConcernInfo->getDetails() ;
  if (pNCIPatho)
    _PptDetails = *pNCIPatho ;
}

void
NSNewRiskDlg::CreateControls()
{
  pType        = new NSUtilLexique(pContexte, this, PREOCCUP_EDIT, pContexte->getDico()) ;
  pType->setLostFocusFunctor(new MemFunctor<NSNewRiskDlg>((NSNewRiskDlg*)this, &NSNewRiskDlg::SetCocCodeNoVerbose)) ;

  //pRc         = new OWL::TRadioButton(this, IDC_RC) ;
  pDateDebText = new OWL::TStatic(this, DATE_DEB_TEXT) ;
  pDateDeb     = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_DEB) ;
  pGroupFin    = new TGroupBox(this, DATE_FIN_TEXT) ;
  pDateFin     = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_FIN) ;
  pNbJours     = new NSEditNum(pContexte, this, IDC_NBJOURS, 10) ;
  pRChronique  = new OWL::TRadioButton(this, IDR_CHRONIQUE) ;
  pRDans       = new OWL::TRadioButton(this, IDR_DANS) ;
  pRDuree      = new OWL::TRadioButton(this, IDR_DUREE) ;
  pJoursText   = new OWL::TStatic(this, JOURS_TEXT) ;
  pRLe         = new OWL::TRadioButton(this, IDR_LE) ;

  pCocButton = new OWL::TButton(this, IDC_COC) ;
  _sClassif  = string("6CISP") ;
  _sPostCase = string("0SOA41") ;
}

NSNewRiskDlg::~NSNewRiskDlg()
{
  delete pType ;
  delete pDateDebText ;
  //delete pRc ;
  delete pDateDeb ;
  delete pGroupFin ;
  delete pDateFin ;
  delete pNbJours ;
  delete pRChronique ;
  delete pRDans ;
  delete pRDuree ;
  delete pJoursText ;
  delete pRLe ;
  delete pCocButton ;
}

void
NSNewRiskDlg::SetupWindow()
{
	TDialog::SetupWindow() ;

	// char szDateJour[9];
	// donne_date_duJour(szDateJour);
	NVLdVTemps tpsNow ;
	tpsNow.takeTime() ;

  string sDateDeb = string("") ;
  if (_pNewConcernInfo)
    sDateDeb = _pNewConcernInfo->getStartDate() ;

	if (string("") != sDateDeb)
	{
		// We must normalize the incoming date
		NVLdVTemps tpsNormalizer ;
    if (tpsNormalizer.initFromDate(sDateDeb))
		{
    	tpsNormalizer.normalize() ;
			pDateDeb->setDate(tpsNormalizer.donneDateHeure()) ;
		}
    else
			pDateDeb->setDate(tpsNow.donneDateHeure()) ;
	}
	else
		pDateDeb->setDate(tpsNow.donneDateHeure()) ;

	pDateFin->setDate(tpsNow.donneDateHeure()) ;

  // Fix "Continuity of care code" label early so that it can be replaced
  // by the real information if
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sText = pSuper->getText("healthConcernDialog", "continuityOfCareCode") ;
  pCocButton->SetCaption(sText.c_str()) ;

  string sCode = string("") ;
  if (_pNewConcernInfo)
    sCode = _pNewConcernInfo->getLexique() ;

  string sFreeTxt = string("") ;
  if (_pNewConcernInfo)
    sFreeTxt = _pNewConcernInfo->getFreeText() ;

	if (string("") != sCode)
  {
		pType->setLabel(sCode) ;

    if (string("") == _sCocCode)
      FindCocCode(false) ;
    else
      DisplayCocCode() ;
  }
	else
		pType->SetText(sFreeTxt.c_str()) ;

	pNbJours->SetText("") ;

	pRChronique->SetCheck(BF_CHECKED) ;
	pDateFin->SetReadOnly(true) ;
	pNbJours->SetReadOnly(true) ;

  // Texts
  //
  string sTitle = pSuper->getText("healthConcernDialog", "newRisk") ;
  SetCaption(sTitle.c_str()) ;

  sText = pSuper->getText("healthConcernDialog", "startingDate") ;
  pDateDebText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "endingDate") ;
  pGroupFin->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "undetermined") ;
  pRChronique->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "fromNow") ;
  pRDans->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "totalDuration") ;
  pRDuree->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "onThe") ;
  pRLe->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "days") ;
  pJoursText->SetCaption(sText.c_str()) ;
}

void
NSNewRiskDlg::ResultCons()
{
}

void
NSNewRiskDlg::UserName(WPARAM wParam)
{
  switch (wParam)
  {
    case (IDR_CHRONIQUE) :
    	pDateFin->SetReadOnly(true) ;
      pNbJours->SetReadOnly(true) ;
      break ;
    case (IDR_DANS) :
    	pDateFin->SetReadOnly(true) ;
      pNbJours->SetReadOnly(false) ;
      pNbJours->SetFocus() ;
      break ;
    case (IDR_DUREE) :
    	pDateFin->SetReadOnly(true) ;
      pNbJours->SetReadOnly(false) ;
      pNbJours->SetFocus() ;
      break ;
    case (IDR_LE) :
    	pNbJours->SetReadOnly(true) ;
      pDateFin->SetReadOnly(false) ;
      pDateFin->SetFocus() ;
      break ;
  }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSNewRiskDlg::CmOk()
{
try
{
  // on récupère d'abord un éventuel élément lexique sélectionné par les flêches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (pType->getDico()->getDicoDialog()->EstOuvert())
  {
    pType->getDico()->getDicoDialog()->InsererElementLexique() ;
    return ;
  }

  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

  string sTL = string("") ;

  // Ne pas accepter les textes libres
  if (pType->getCode() == string("£?????"))
  {
    if (false == pContexte->getEpisodus()->bAllowFreeTextLdV)
    {
      string sWarningTxt = pContexte->getSuperviseur()->getText("lexiqueManagement", "freeTextNotAllowed") ;
      erreur(sWarningTxt.c_str(), warningError, 0, GetHandle()) ;
      pType->SetFocus() ;
      return ;
    }

    int       iBuffLen  = pType->GetTextLen() ;
    char far  *szBuff   = new char[iBuffLen + 1] ;
    pType->GetText(szBuff, iBuffLen + 1) ;
    sTL = string(szBuff) ;
    delete[] szBuff ;

    if (_pNewConcernInfo)
      _pNewConcernInfo->setFreeText(sTL) ;
  }

  string sDateDeb ;
  pDateDeb->getDate(&sDateDeb) ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setStartDate(sDateDeb) ;

  string sDateFin ;

  // récupération de la valeur du champs edit correspondant au nombre de jours
  pNbJours->donneValeur() ;

  // si on est dans le cas où "chronique" est cochée
  if (pRChronique->GetCheck() == BF_CHECKED)
    sDateFin = "00000000000000" ;

  // si on dans le cas où "dans" est cochée
  if (pRDans->GetCheck() == BF_CHECKED)
  {
    if (pNbJours->_dValeur < 1)
    {
      erreur("La préoccupation doit durer au moins 1 jour.", standardError, 0, GetHandle()) ;
      return ;
    }

    NVLdVTemps  tDateFin ;
    tDateFin.takeTime() ;
    tDateFin.ajouteJours(pNbJours->_dValeur) ;
    sDateFin = tDateFin.donneDateHeure() ;
  }

  // si on est dans le cas où "durée" est cochée
  if (pRDuree->GetCheck() == BF_CHECKED)
  {
    if (pNbJours->_dValeur < 1)
    {
      erreur("La préoccupation doit durer au moins 1 jour.", standardError, 0, GetHandle()) ;
      return ;
    }

    NVLdVTemps  tDateFin ;
    tDateFin.initFromDate(sDateDeb) ;
    tDateFin.ajouteJours(pNbJours->_dValeur) ;
    sDateFin = tDateFin.donneDateHeure() ;
  }

  // si on est dans le cas où "le" est cochée
  if (pRLe->GetCheck() == BF_CHECKED)
    pDateFin->getDate(&sDateFin) ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setEndDate(sDateFin) ;

  // Coc code
  //
  if (_pNewConcernInfo)
    _pNewConcernInfo->setCocCode(_sCocCode) ;

  // Concern code
  //
  string sConcernCode = pType->getCode() ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setLexique(sConcernCode) ;

  // Details
  //
  if (_pNewConcernInfo)
    _pNewConcernInfo->setDetails(_PptDetails) ;

/*
  NSLdvDocument *pDoc = pLdvDoc ;
  if ((NULL == pDoc) && pLdvView)
    pDoc = pLdvView->getDoc() ;

  if (!pDoc)
    return ;

  bool bResult = pDoc->newLine(ldvframeRisk, pType->getCode(), psNewNode, sDateDeb, sDateFin, 0, *psCocCode, pPtDetails, sTL) ;

  if (psType)
    *psType = pType->getCode() ;
  if (psCode)
    *psCode = pType->getCode() ;
*/

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSNewRiskDlg::CmOk.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSNewRiskDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSNewRiskDlg::SetDetails()
{
try
{
# ifdef __OB1__
	NSSmallBrother BigBoss(pContexte, &_PptDetails, 0) ;
#else
	NSSmallBrother BigBoss(pContexte, &_PptDetails, 0, false /* initFromBbk */) ;
#endif
	BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
	// on lance l'archetype lié au code médicament (avec fil guide associé)
	BigBoss.lance12("0PRO11", "0DETA1") ;
}
catch (...)
{
	erreur("Exception NSNewRiskDlg::SetDetails.",  standardError, 0) ;
}
}

void
NSNewRiskDlg::SetCocCodeVerbose()
{
  FindCocCode(true) ;
}

void
NSNewRiskDlg::SetCocCodeNoVerbose()
{
  FindCocCode(false) ;
}

void
NSNewRiskDlg::DisplayCocCode()
{
  if (string("") == _sCocCode)
  {
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    pCocButton->SetCaption(sText.c_str()) ;
    return ;
  }

  NSEpiClassifDB   dbClassif(pContexte) ;
  NSEpiClassifInfo classifInfo ;
  bool bFoundCode = dbClassif.searchCode(_sClassif, _sCocCode, &classifInfo) ;

  string sCaption = _sCocCode ;
  if (bFoundCode)
    sCaption += string(" - ") + classifInfo.getLabel() ;

  pCocButton->SetCaption(sCaption.c_str()) ;
}

void
NSNewRiskDlg::FindCocCode(bool bVerbose)
{
try
{
  string sHealthConcern = pType->getCode() ;
  if (string("") == sHealthConcern)
    return ;

  string sLabel = pType->getLabel() ;

  NSLdvDocumentBase *pDoc = pLdvDoc ;
  if (((NSLdvDocumentBase*) NULL == pDoc) && pLdvView)
    pDoc = pLdvView->getDoc() ;

  if ((NSLdvDocumentBase*) NULL == pDoc)
    return ;

  string sGoodCode = string("") ;

  pDoc->findCocCode(bVerbose, sGoodCode, sHealthConcern, sLabel, _sClassif, _sPostCase) ;

  if (string("") == sGoodCode)
  {
    _sCocCode = string("") ;
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    pCocButton->SetCaption(sText.c_str()) ;
  	return ;
  }

  _sCocCode = sGoodCode ;

  DisplayCocCode() ;

  return ;

/*
  string sHealthConcern = pType->getCode() ;
  if (string("") == sHealthConcern)
    return ;

  string sConcept ;
  pContexte->getDico()->donneCodeSens(&sHealthConcern, &sConcept) ;

  string sResO = string("") ;
  string sResP = string("") ;
  string sResI = string("") ;
  string sRes3 = string("") ;

  string sDomain = string("") ;

//  if (sConcept == string("£??"))
//  {
//  	string sLibelle  = pType->sTexteGlobal ;
//    string sLocalize = pNSTreeNode->getPosition() ;
//
//    ParseSOAP Parser(pContexte, &sClassif) ;
//    Parser.computeParsing(&sLibelle, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;
//  }
//  else
//  {

  	Classify ClassifTool(pContexte, &_sClassif, &sConcept, &_sPostCase) ;
  	ClassifTool.computeParsing(&sResO, &sResP, &sResI) ;

    // If Classer has found 'Mandatory code(s)', then we must take it
    //
    if (string("") != sResO)
      sDomain = sResO ;

    // Else, 3BT is the gold standard, so, we have to use it
    //
    else
    {
      string sClasserDomain  = sResP ;

      //
      //
      string sLabel = pType->getLabel() ;
      string sLocalize = _sPostCase ;

      ParseSOAP Parser(pContexte, &_sClassif) ;
      Parser.computeParsing(&sLabel, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;

      if (string("") != sResP)
        sDomain  = sResP ;
      else
        sDomain  = sClasserDomain ;
    }

//  }

  //
  // On trouve le code
  //
  NSEpisodus* pEpisodus = pContexte->getSuperviseur()->getEpisodus() ;

  classifExpert* pExpert = pEpisodus->pClassifExpert ;
  if (NULL == pExpert)
    return ;

  NSEpiClassifInfoArray arrayClassif ;

  ElemSetArray* pElemDomain = 0 ;
  //
  // On instancie le domaine
  // Instanciating the domain
  //
  ParseCategory Parser(pExpert->donneCodeSize(_sClassif), _sClassif,
                                             pExpert->donnePattern(_sClassif)) ;
  pElemDomain = Parser.DefDomain(sDomain) ;
  //
  // On trouve les codes qui correspondent au domaine
  // Finding the codes that belong to the domain
  //
  string sCaseSens ;
  pContexte->getDico()->donneCodeSens(&_sPostCase, &sCaseSens) ;

  pExpert->fillList(_sClassif, pElemDomain, &arrayClassif, sCaseSens) ;

  if (pElemDomain)
    delete pElemDomain ;

  int      iBuffLen = pType->GetTextLen() ;
  char far *szBuff  = new char[iBuffLen + 1] ;
  pType->GetText(szBuff, iBuffLen + 1) ;

  string sLabel = string(szBuff) ;

  string sGoodCode = "" ;
  //
  //
  //
  if (arrayClassif.size() != 1)
  {
    if (bVerbose)
    {
  	  SOAPObject SOAPObj(sLabel, sResP, _sClassif, 0, string("")) ;
#ifndef _EXT_CAPTURE
		  NSCapture Capture(pContexte) ;
#else
		  NSCapture Capture ;
#endif

		  Capture.sClassifResultO = sResO ;
      Capture.sClassifResultP = sResP ;
      Capture.sClassifResultI = sResI ;
      Capture.sClassifResult3 = sRes3 ;

      SOAPObj.pCaptElemnt = &Capture ;
      SOAPObj.sCase       = sCaseSens ;

      sGoodCode = pExpert->chooseCode(&SOAPObj) ;
    }
  }
  else
  {
  	sGoodCode = (*(arrayClassif.begin()))->pDonnees->code ;

    string sCtrlData = "" ;
    NVLdVTemps tpsDebut ;
    tpsDebut.takeTime() ;
    pExpert->setControlString(&sCtrlData, _sClassif, sCaseSens, sLabel,
                                  classifExpert::niveauPreselection, 0,
                                  &tpsDebut, &tpsDebut, sGoodCode, sConcept) ;
    pExpert->storeControlData(sCtrlData) ;
  }

  if (string("") == sGoodCode)
  {
    *psCocCode = string("") ;
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    pCocButton->SetCaption(sText.c_str()) ;
  	return ;
  }

  *psCocCode = sGoodCode ;

  DisplayCocCode() ;
*/
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSNewRiskDlg::FindCocCode : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSNewRiskDlg::FindCocCode.",  standardError, 0) ;
}
}

// -----------------------------------------------------------------
//
//  Méthodes de NSNewConcernDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSNewSocialDlg, NSUtilDialog)
  EV_COMMAND(IDOK,                        CmOk),
  EV_COMMAND(IDCANCEL,                    CmCancel),
  EV_COMMAND_AND_ID(IDR_CHRONIQUE,        UserName),
  EV_COMMAND_AND_ID(IDR_DANS,             UserName),
  EV_COMMAND_AND_ID(IDR_DUREE,            UserName),
  EV_COMMAND_AND_ID(IDR_LE,               UserName),
  EV_CHILD_NOTIFY_ALL_CODES(IDC_GRAVITE,  UpdateSeverity),
  EV_COMMAND(IDC_COC,                     SetCocCodeVerbose),
END_RESPONSE_TABLE;

NSNewSocialDlg::NSNewSocialDlg(NSLdvViewBase* pView, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo)
               :NSUtilDialog((TWindow*) pView, pCtx, "NEW_SOCIAL", pCtx->GetMainWindow()->GetModule()),
                _PptDetails(pCtx->getSuperviseur())
{
try
{
  pLdvView    = pView ;
  pLdvDoc     = 0 ;

  _pNewConcernInfo = pNewConcernInfo ;
  InitFromInfo() ;

  CreateControls() ;

  // pRiskGauge  = new OWL::TGauge(this, "%d%%", IDC_PROGRESSRISK, 25, 261, 215, 20) ;
  // pRiskGauge->SetNativeUse(nuNever) ;

/*
  if ((strlen(sLabel->c_str()) > 0) && (sLabel->rfind(")") == strlen(sLabel->c_str()) - 1))
  {
		int iPos = sLabel->rfind("(") ;
		sLabel->replace(iPos - 1, NPOS, "\0") ;
  }
*/
}
catch (...)
{
  erreur("Exception NSNewConcernDlg ctor.",  standardError, 0) ;
}
}

NSNewSocialDlg::NSNewSocialDlg(TWindow* pPere, NSLdvDocument* pDoc, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo)
               :NSUtilDialog(pPere, pCtx, "NEW_SOCIAL", pCtx->GetMainWindow()->GetModule()),
                _PptDetails(pCtx->getSuperviseur())
{
try
{
  pLdvView    = 0 ;
  pLdvDoc     = pDoc ;

  _pNewConcernInfo = pNewConcernInfo ;
  InitFromInfo() ;

  CreateControls() ;
}
catch (...)
{
	erreur("Exception NSNewConcernDlg ctor.",  standardError, 0) ;
}
}

void
NSNewSocialDlg::InitFromInfo()
{
  if ((NewConcernInfo*) NULL == _pNewConcernInfo)
    return ;

  _sCocCode = _pNewConcernInfo->getCocCode() ;

  NSPatPathoArray* pNCIPatho = _pNewConcernInfo->getDetails() ;
  if (pNCIPatho)
    _PptDetails = *pNCIPatho ;
}

void
NSNewSocialDlg::CreateControls()
{
  pType        = new NSUtilLexique(pContexte, this, PREOCCUP_EDIT, pContexte->getDico()) ;
  pType->setLostFocusFunctor(new MemFunctor<NSNewSocialDlg>((NSNewSocialDlg*)this, &NSNewSocialDlg::SetCocCodeNoVerbose)) ;

  //pRc         = new OWL::TRadioButton(this, IDC_RC) ;
  pDateDebText = new OWL::TStatic(this, DATE_DEB_TEXT) ;
  pDateDeb     = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_DEB) ;
  pGroupFin    = new TGroupBox(this, DATE_FIN_TEXT) ;
  pDateFin     = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_FIN) ;
  pNbJours     = new NSEditNum(pContexte, this, IDC_NBJOURS, 10) ;
  pRChronique  = new OWL::TRadioButton(this, IDR_CHRONIQUE) ;
  pRDans       = new OWL::TRadioButton(this, IDR_DANS) ;
  pRDuree      = new OWL::TRadioButton(this, IDR_DUREE) ;
  pJoursText   = new OWL::TStatic(this, JOURS_TEXT) ;
  pRLe         = new OWL::TRadioButton(this, IDR_LE) ;
  pGravite     = new OWL::THSlider(this, IDC_GRAVITE) ;
  // pRisque 	= new OWL::THSlider(this, IDC_RISQUE);

  pSeverityText = new OWL::TStatic(this, SEVER_TEXT) ;
  pGravGauge    = new OWL::TGauge(this, "%d%%", IDC_PROGRESSGRAV, 18, 182, 300, 20) ;
  // pGravGauge  = new OWL::TGauge(this, "%d%%", IDC_PROGRESSGRAV, 25, 209, 215, 20) ;
  pGravGauge->SetRange(0, 100) ;
  pGravGauge->SetNativeUse(nuNever) ;

  pCocButton = new OWL::TButton(this, IDC_COC) ;
  _sClassif  = string("6CISP") ;
  _sPostCase = string("0SOA41") ;
}

NSNewSocialDlg::~NSNewSocialDlg()
{
  delete pType ;
  delete pDateDebText ;
  //delete pRc ;
  delete pDateDeb ;
  delete pGroupFin ;
  delete pDateFin ;
  delete pNbJours ;
  delete pRChronique ;
  delete pRDans ;
  delete pRDuree ;
  delete pJoursText ;
  delete pRLe ;
  delete pGravite ;
  delete pSeverityText ;
  delete pGravGauge ;
  delete pCocButton ;
}

void
NSNewSocialDlg::SetupWindow()
{
	TDialog::SetupWindow() ;

	// char szDateJour[9];
	// donne_date_duJour(szDateJour);
	NVLdVTemps tpsNow ;
	tpsNow.takeTime() ;

  string sDateDeb = string("") ;
  if (_pNewConcernInfo)
    sDateDeb = _pNewConcernInfo->getStartDate() ;

	if (string("") != sDateDeb)
	{
		// We must normalize the incoming date
		NVLdVTemps tpsNormalizer ;
    if (tpsNormalizer.initFromDate(sDateDeb))
		{
    	tpsNormalizer.normalize() ;
			pDateDeb->setDate(tpsNormalizer.donneDateHeure()) ;
		}
    else
			pDateDeb->setDate(tpsNow.donneDateHeure()) ;
	}
	else
		pDateDeb->setDate(tpsNow.donneDateHeure()) ;

	pDateFin->setDate(tpsNow.donneDateHeure()) ;

  // Fix "Continuity of care code" label early so that it can be replaced
  // by the real information if
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sText = pSuper->getText("healthConcernDialog", "continuityOfCareCode") ;
  pCocButton->SetCaption(sText.c_str()) ;

  string sCode = string("") ;
  if (_pNewConcernInfo)
    sCode = _pNewConcernInfo->getLexique() ;

  string sFreeText = string("") ;
  if (_pNewConcernInfo)
    sFreeText = _pNewConcernInfo->getFreeText() ;

	if (string("") != sCode)
  {
		pType->setLabel(sCode) ;

    if (string("") == _sCocCode)
      FindCocCode(false) ;
    else
      DisplayCocCode() ;
  }
	else
		pType->SetText(sFreeText.c_str()) ;

	pNbJours->SetText("") ;

	pRChronique->SetCheck(BF_CHECKED) ;
	pDateFin->SetReadOnly(true) ;
	pNbJours->SetReadOnly(true) ;

	pGravite->SetRange(0, 100) ;   //fixer min et max
	pGravite->SetRuler(5, false) ; //espacement entre deux graduations
	pGravite->SetPosition(10) ;

  int iSeverity = 10 ;
  if (_pNewConcernInfo)
    iSeverity = _pNewConcernInfo->getSeverity() ;

	pGravGauge->SetValue(iSeverity) ;

	//pRisque->SetRange(0, 100);
	//pRisque->SetRuler(5, false);
	//pRisque->SetPosition(0);

	//pRiskGauge->SetRange(0, 100);

  // Texts
  //
  string sTitle = pSuper->getText("healthConcernDialog", "newSocialConcern") ;
  SetCaption(sTitle.c_str()) ;

  sText = pSuper->getText("healthConcernDialog", "startingDate") ;
  pDateDebText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "endingDate") ;
  pGroupFin->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "undetermined") ;
  pRChronique->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "fromNow") ;
  pRDans->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "totalDuration") ;
  pRDuree->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "onThe") ;
  pRLe->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "days") ;
  pJoursText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "severityIndex") ;
  pSeverityText->SetCaption(sText.c_str()) ;
}

void
NSNewSocialDlg::ResultCons()
{
}

void
NSNewSocialDlg::UserName(WPARAM wParam)
{
  switch (wParam)
  {
    case (IDR_CHRONIQUE) :
    	pDateFin->SetReadOnly(true) ;
      pNbJours->SetReadOnly(true) ;
      break ;
    case (IDR_DANS) :
    	pDateFin->SetReadOnly(true) ;
      pNbJours->SetReadOnly(false) ;
      pNbJours->SetFocus() ;
      break ;
    case (IDR_DUREE) :
    	pDateFin->SetReadOnly(true) ;
      pNbJours->SetReadOnly(false) ;
      pNbJours->SetFocus() ;
      break ;
    case (IDR_LE) :
    	pNbJours->SetReadOnly(true) ;
      pDateFin->SetReadOnly(false) ;
      pDateFin->SetFocus() ;
      break ;
  }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSNewSocialDlg::CmOk()
{
try
{
  // on récupère d'abord un éventuel élément lexique sélectionné par les flêches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (pType->getDico()->getDicoDialog()->EstOuvert())
  {
    pType->getDico()->getDicoDialog()->InsererElementLexique() ;
    return ;
  }

  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

  string sTL = string("") ;

  // Ne pas accepter les textes libres
  if (pType->getCode() == string("£?????"))
  {
    if (false == pContexte->getEpisodus()->bAllowFreeTextLdV)
    {
      string sWarningTxt = pContexte->getSuperviseur()->getText("lexiqueManagement", "freeTextNotAllowed") ;
      erreur(sWarningTxt.c_str(), warningError, 0, GetHandle()) ;
      pType->SetFocus() ;
      return ;
    }

    int       iBuffLen  = pType->GetTextLen() ;
    char far  *szBuff   = new char[iBuffLen + 1] ;
    pType->GetText(szBuff, iBuffLen + 1) ;
    sTL = string(szBuff) ;
    delete[] szBuff ;

    if (_pNewConcernInfo)
      _pNewConcernInfo->setFreeText(sTL) ;
  }

  string sDateDeb ;
  pDateDeb->getDate(&sDateDeb) ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setStartDate(sDateDeb) ;

  string sDateFin ;

  // récupération de la valeur du champs edit correspondant au nombre de jours
  pNbJours->donneValeur() ;

  // si on est dans le cas où "chronique" est cochée
  if (pRChronique->GetCheck() == BF_CHECKED)
    sDateFin = "00000000000000" ;

  // si on dans le cas où "dans" est cochée
  if (pRDans->GetCheck() == BF_CHECKED)
  {
    if (pNbJours->_dValeur < 1)
    {
      erreur("La préoccupation doit durer au moins 1 jour.", standardError, 0, GetHandle()) ;
      return ;
    }

    NVLdVTemps  tDateFin ;
    tDateFin.takeTime() ;
    tDateFin.ajouteJours(pNbJours->_dValeur) ;
    sDateFin = tDateFin.donneDateHeure() ;
  }

  // si on est dans le cas où "durée" est cochée
  if (pRDuree->GetCheck() == BF_CHECKED)
  {
    if (pNbJours->_dValeur < 1)
    {
      erreur("La préoccupation doit durer au moins 1 jour.", standardError, 0, GetHandle()) ;
      return ;
    }

    NVLdVTemps  tDateFin ;
    tDateFin.initFromDate(sDateDeb) ;
    tDateFin.ajouteJours(pNbJours->_dValeur) ;
    sDateFin = tDateFin.donneDateHeure() ;
  }

  // si on est dans le cas où "le" est cochée
  if (pRLe->GetCheck() == BF_CHECKED)
    pDateFin->getDate(&sDateFin) ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setEndDate(sDateFin) ;

  int iGravite = pGravite->GetPosition() ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setSeverity(iGravite) ;

  // Coc code
  //
  if (_pNewConcernInfo)
    _pNewConcernInfo->setCocCode(_sCocCode) ;

  // Concern code
  //
  string sConcernCode = pType->getCode() ;

  if (_pNewConcernInfo)
    _pNewConcernInfo->setLexique(sConcernCode) ;

  // Details
  //
  if (_pNewConcernInfo)
    _pNewConcernInfo->setDetails(_PptDetails) ;

/*
  NSLdvDocument *pDoc = pLdvDoc ;
  if ((NULL == pDoc) && pLdvView)
    pDoc = pLdvView->getDoc() ;

  if (!pDoc)
    return ;

  bool bResult = pDoc->newLine(ldvframeSocial, pType->getCode(), psNewNode, sDateDeb, sDateFin, iGravite, *psCocCode, pPtDetails, sTL) ;

  if (psType)
    *psType = pType->getCode() ;
  if (psCode)
    *psCode = pType->getCode() ;
*/

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSNewSocialDlg::CmOk.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSNewSocialDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSNewSocialDlg::UpdateSeverity(uint)
{
	int setting = pGravite->GetPosition() ;

	if      (setting < 25)
		pGravGauge->SetColor(NS_CLASSLIB::TColor(0, 0, 128)) ;   // Blue
	else if (setting < 50)
		pGravGauge->SetColor(NS_CLASSLIB::TColor(0, 128, 0)) ;   // Green
	else if (setting < 75)
		pGravGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 0)) ; // Yellow
	else
		pGravGauge->SetColor(NS_CLASSLIB::TColor(128, 0, 0)) ;   // Red

	pGravGauge->SetValue(setting) ;
}

/*
void
NSNewConcernDlg::UpdateRisk(uint)
{
    int setting = pRisque->GetPosition();

    if      (setting < 10)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(0, 0, 128));
    else if (setting < 15)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(0, 128, 0));
    else if (setting < 20)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 0));
    else
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(128, 0, 0));

    pRiskGauge->SetValue(setting);
} */

void
NSNewSocialDlg::SetDetails()
{
try
{
# ifdef __OB1__
	NSSmallBrother BigBoss(pContexte, &_PptDetails, 0) ;
#else
	NSSmallBrother BigBoss(pContexte, &_PptDetails, 0, false /* initFromBbk */) ;
#endif
	BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
	// on lance l'archetype lié au code médicament (avec fil guide associé)
	BigBoss.lance12("0PRO11", "0DETA1") ;
}
catch (...)
{
	erreur("Exception NSNewSocialDlg::SetDetails.",  standardError, 0) ;
}
}

void
NSNewSocialDlg::SetCocCodeVerbose()
{
  FindCocCode(true) ;
}

void
NSNewSocialDlg::SetCocCodeNoVerbose()
{
  FindCocCode(false) ;
}

void
NSNewSocialDlg::DisplayCocCode()
{
  if (string("") == _sCocCode)
  {
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    pCocButton->SetCaption(sText.c_str()) ;
    return ;
  }

  NSEpiClassifDB   dbClassif(pContexte) ;
  NSEpiClassifInfo classifInfo ;
  bool bFoundCode = dbClassif.searchCode(_sClassif, _sCocCode, &classifInfo) ;

  string sCaption = _sCocCode ;
  if (bFoundCode)
    sCaption += string(" - ") + classifInfo.getLabel() ;

  pCocButton->SetCaption(sCaption.c_str()) ;
}

void
NSNewSocialDlg::FindCocCode(bool bVerbose)
{
try
{
  string sHealthConcern = pType->getCode() ;
  if (string("") == sHealthConcern)
    return ;

  string sLabel = pType->getLabel() ;

  NSLdvDocumentBase *pDoc = pLdvDoc ;
  if (((NSLdvDocumentBase*) NULL == pDoc) && pLdvView)
    pDoc = pLdvView->getDoc() ;

  if ((NSLdvDocumentBase*) NULL == pDoc)
    return ;

  string sGoodCode = string("") ;

  pDoc->findCocCode(bVerbose, sGoodCode, sHealthConcern, sLabel, _sClassif, _sPostCase) ;

  if (string("") == sGoodCode)
  {
    _sCocCode = string("") ;
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    pCocButton->SetCaption(sText.c_str()) ;
  	return ;
  }

  _sCocCode = sGoodCode ;

  DisplayCocCode() ;

  return ;

/*
  string sConcept ;
  pContexte->getDico()->donneCodeSens(&sHealthConcern, &sConcept) ;

  string sResO = string("") ;
  string sResP = string("") ;
  string sResI = string("") ;
  string sRes3 = string("") ;

  string sDomain = string("") ;

//  if (sConcept == string("£??"))
//  {
//  	string sLibelle  = pType->sTexteGlobal ;
//    string sLocalize = pNSTreeNode->getPosition() ;
//
//    ParseSOAP Parser(pContexte, &sClassif) ;
//    Parser.computeParsing(&sLibelle, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;
//  }
//  else
//  {

  	Classify ClassifTool(pContexte, &_sClassif, &sConcept, &_sPostCase) ;
  	ClassifTool.computeParsing(&sResO, &sResP, &sResI) ;

    // If Classer has found 'Mandatory code(s)', then we must take it
    //
    if (string("") != sResO)
      sDomain = sResO ;

    // Else, 3BT is the gold standard, so, we have to use it
    //
    else
    {
      string sClasserDomain  = sResP ;

      //
      //
      string sLabel = pType->getLabel() ;
      string sLocalize = _sPostCase ;

      ParseSOAP Parser(pContexte, &_sClassif) ;
      Parser.computeParsing(&sLabel, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;

      if (string("") != sResP)
        sDomain  = sResP ;
      else
        sDomain  = sClasserDomain ;
    }

//  }

  //
  // On trouve le code
  //
  NSEpisodus* pEpisodus = pContexte->getSuperviseur()->getEpisodus() ;

  classifExpert* pExpert = pEpisodus->pClassifExpert ;
  if (NULL == pExpert)
    return ;

  NSEpiClassifInfoArray arrayClassif ;

  ElemSetArray* pElemDomain = 0 ;
  //
  // On instancie le domaine
  // Instanciating the domain
  //
  ParseCategory Parser(pExpert->donneCodeSize(_sClassif), _sClassif,
                                             pExpert->donnePattern(_sClassif)) ;
  pElemDomain = Parser.DefDomain(sDomain) ;
  //
  // On trouve les codes qui correspondent au domaine
  // Finding the codes that belong to the domain
  //
  string sCaseSens ;
  pContexte->getDico()->donneCodeSens(&_sPostCase, &sCaseSens) ;

  pExpert->fillList(_sClassif, pElemDomain, &arrayClassif, sCaseSens) ;

  if (pElemDomain)
    delete pElemDomain ;

  int      iBuffLen = pType->GetTextLen() ;
  char far *szBuff  = new char[iBuffLen + 1] ;
  pType->GetText(szBuff, iBuffLen + 1) ;

  string sLabel = string(szBuff) ;

  string sGoodCode = "" ;
  //
  //
  //
  if (arrayClassif.size() != 1)
  {
    if (bVerbose)
    {
  	  SOAPObject SOAPObj(sLabel, sResP, _sClassif, 0, string("")) ;
#ifndef _EXT_CAPTURE
		  NSCapture Capture(pContexte) ;
#else
		  NSCapture Capture ;
#endif

		  Capture.sClassifResultO = sResO ;
      Capture.sClassifResultP = sResP ;
      Capture.sClassifResultI = sResI ;
      Capture.sClassifResult3 = sRes3 ;

      SOAPObj.pCaptElemnt = &Capture ;
      SOAPObj.sCase       = sCaseSens ;

      sGoodCode = pExpert->chooseCode(&SOAPObj) ;
    }
  }
  else
  {
  	sGoodCode = (*(arrayClassif.begin()))->pDonnees->code ;

    string sCtrlData = "" ;
    NVLdVTemps tpsDebut ;
    tpsDebut.takeTime() ;
    pExpert->setControlString(&sCtrlData, _sClassif, sCaseSens, sLabel,
                                  classifExpert::niveauPreselection, 0,
                                  &tpsDebut, &tpsDebut, sGoodCode, sConcept) ;
    pExpert->storeControlData(sCtrlData) ;
  }

  if (string("") == sGoodCode)
  {
    *psCocCode = string("") ;
    string sText = pContexte->getSuperviseur()->getText("healthConcernDialog", "continuityOfCareCode") ;
    pCocButton->SetCaption(sText.c_str()) ;
  	return ;
  }

  *psCocCode = sGoodCode ;

  DisplayCocCode() ;
*/
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSNewConcernDlg::FindCocCode : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSNewSocialDlg::FindCocCode.",  standardError, 0) ;
}
}

// -----------------------------------------------------------------
//
//  Méthodes de NSSimpleNewDrugDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSSimpleNewDrugDlg, NSUtilDialog)
  EV_COMMAND(IDOK,                     CmOk),
  EV_COMMAND(IDCANCEL,                 CmCancel),
  EV_COMMAND_AND_ID(IDR_DRG_CHRONIQUE, UserName),
  EV_COMMAND_AND_ID(IDR_DRG_DANS,      UserName),
  EV_COMMAND_AND_ID(IDR_DRG_DUREE,     UserName),
  EV_COMMAND_AND_ID(IDR_DRG_LE,        UserName),
  EV_COMMAND(DRUG_COMPLEX_MODE,        switchToComplexMode),
  EV_COMMAND(FREE_TEXT_BUTTON,         editFreeText),
  EV_COMMAND(NR_BUTTON,                nonRenouvelable),
  // BDM specific functions
  EV_COMMAND(DRUG_INFORMATION,         drugInformation),
  EV_COMMAND(IDC_ISSUE_BTN,            selectIndication),
  EV_COMMAND(BDM_SEARCH_INDIC,         searchInBdmByIndication),
  EV_COMMAND(BDM_SEARCH_SUBST,         searchInBdmBySubstance),
  EV_COMMAND(BDM_SEARCH_ATC,           searchInBdmByATC),
END_RESPONSE_TABLE;

NSSimpleNewDrugDlg::NSSimpleNewDrugDlg(TWindow* pView, NSContexte *pCtx, NSPatPathoArray *pPPTinit, string sEditedNodeId)
                   :NSPrescriptionBaseDlg(pView, pCtx, (pCtx->getBamType() == NSContexte::btNone) ? "NEW_DRUG" : "NEW_DRUG_BAM")
{
try
{
  _sEditedNodeId  = sEditedNodeId ;
  _sReferential   = string("") ;

  _pPhase = (NSphaseMedic*) 0 ;

  // Creating interface elements
  //
  createInterfaceElements() ;

  _pPPT = pPPTinit ;

  _bMustSwitchToComplexMode = false ;
}
catch (...)
{
	erreur("Exception NSSimpleNewDrugDlg ctor.",  standardError, 0) ;
}
}

void
NSSimpleNewDrugDlg::createInterfaceElements()
{
	_pTrtGroup              = new OWL::TGroupBox(this, TRT_GROUP) ;

	_pPrescriptionGroup     = new OWL::TGroupBox(this, PRESCR_GROUP) ;

	_pDureePhaseTxt         = new OWL::TStatic(this, PRESCR_DURA_TXT) ;
	_pDureePhase            = new NSUpDownEdit(this, pContexte, string(""), PRESCR_DURA, PRESCR_DURA_UPDN) ;
  _pDureePhase->getEditNum()->SetLostFocusResponse(new MemFunctor<NSSimpleNewDrugDlg>( (NSSimpleNewDrugDlg*)this, &NSSimpleNewDrugDlg::ActualisePhase )) ;
  _pDureePhase->getUpDown()->SetLostFocusResponse(new MemFunctor<NSSimpleNewDrugDlg>( (NSSimpleNewDrugDlg*)this, &NSSimpleNewDrugDlg::ActualisePhase )) ;

  const char *temp[] = { "2HEUR1", "2DAT01", "2DAT11", "2DAT21" } ;
  _pCBDureePhase          = new NSComboBox(this, PRESCR_DURA_UNIT, pContexte, temp, 4) ;
  _pCBDureePhase->SetLostFocusResponse(new MemFunctor<NSSimpleNewDrugDlg>( (NSSimpleNewDrugDlg*)this, &NSSimpleNewDrugDlg::ActualisePhase ));
	_pRenouvellementTxt     = new OWL::TStatic(this, PRESCR_RENEW_TXT) ;
	_pRenouvellement        = new NSUpDownEdit(this, pContexte, "", PRESCR_RENEW, PRESCR_RENEW_UPDN) ;
  _pNonRenouvelable       = new OWL::TCheckBox(this, NR_BUTTON) ;

	_pRenouvellementTimeTxt = new OWL::TStatic(this, PRESCR_RENEW_NBTXT) ;

	_pDateDebPrescrTxt      = new OWL::TStatic(this, DATE_DEB_DRG_TEXT) ;
	_pDateDebPrescr         = new NSUtilEditDateHeure(pContexte, this, PRESC_DATE_DEB) ;
	_pDateFinPrescrTxt      = new OWL::TStatic(this, DATE_FINPRESC_TEXT) ;
	_pDateFinPrescr         = new NSUtilEditDateHeure(pContexte, this, PRESC_DATE_FIN) ;

  _pPosologieGroup        = new OWL::TGroupBox(this, POSO_GROUP) ;

	_pPriseMatin            = new NSUpDownEdit(this, pContexte, string(""), POSO_MORNING, POSO_MORNING_UPDN) ;
  _pPriseMidi             = new NSUpDownEdit(this, pContexte, string(""), POSO_NOON,    POSO_NOON_UPDN) ;
  _pPriseSoir             = new NSUpDownEdit(this, pContexte, string(""), POSO_NIGHT,   POSO_NIGHT_UPDN) ;
  _pPriseCoucher          = new NSUpDownEdit(this, pContexte, string(""), POSO_BED,     POSO_BED_UPDN) ;

	_pPriseMatinTxt         = new OWL::TStatic(this, POSO_MORNING_TXT) ;
  _pPriseMidiTxt          = new OWL::TStatic(this, POSO_NOON_TXT) ;
  _pPriseSoirTxt          = new OWL::TStatic(this, POSO_NIGHT_TXT) ;
  _pPriseCoucherTxt       = new OWL::TStatic(this, POSO_BED_TXT) ;

	_pComplexModeButton     = new OWL::TButton(this, DRUG_COMPLEX_MODE) ;
}

NSSimpleNewDrugDlg::~NSSimpleNewDrugDlg()
{
  if (_pPhase)
    delete _pPhase ;

  delete _pTrtGroup ;

	delete _pPrescriptionGroup ;

	delete _pDureePhaseTxt ;
	delete _pDureePhase ;
  delete _pCBDureePhase ;
	delete _pRenouvellementTxt ;
	delete _pRenouvellement ;
	delete _pRenouvellementTimeTxt ;
  delete _pNonRenouvelable ;

	delete _pDateDebPrescrTxt ;
	delete _pDateDebPrescr ;
	delete _pDateFinPrescrTxt ;
	delete _pDateFinPrescr ;

  delete _pPosologieGroup ;

	delete _pPriseMatin ;
  delete _pPriseMidi ;
  delete _pPriseSoir ;
  delete _pPriseCoucher ;

	delete _pPriseMatinTxt ;
  delete _pPriseMidiTxt ;
  delete _pPriseSoirTxt ;
  delete _pPriseCoucherTxt ;

  if (_pDrugInfoButton)
    delete _pDrugInfoButton ;
  if (_pBdmSearchIndicationButton)
    delete _pBdmSearchIndicationButton ;
  if (_pBdmSearchSubstanceButton)
    delete _pBdmSearchSubstanceButton ;
  if (_pBdmSearchAtcButton)
    delete _pBdmSearchAtcButton ;
  if (_pBdmDrugInformation)
    delete _pBdmDrugInformation ;

  delete _pComplexModeButton ;
}

void
NSSimpleNewDrugDlg::SetupWindow()
{
	NSPrescriptionBaseDlg::SetupWindow() ;

  initInterfaceElements() ;
}

void
NSSimpleNewDrugDlg::initInterfaceElements()
{
	// Init static texts
  //
	string sTxt = pContexte->getSuperviseur()->getText("drugDialog", "newDrug") ;
  SetCaption(sTxt.c_str()) ;

  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "treatment") ;
	_pTrtGroup->SetCaption(sTxt.c_str()) ;

  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "endingDate") ;
  _pDateFinGroup->SetCaption(sTxt.c_str()) ;

	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "prescription") ;
	_pPrescriptionGroup->SetCaption(sTxt.c_str()) ;

	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "during") ;
	_pDureePhaseTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "mayBeRenewed") ;
	_pRenouvellementTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "time") ;
	_pRenouvellementTimeTxt->SetText(sTxt.c_str()) ;

	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "from") ;
	_pDateDebPrescrTxt->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "to") ;
	_pDateFinPrescrTxt->SetText(sTxt.c_str()) ;

	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "posology") ;
  _pPosologieGroup->SetCaption(sTxt.c_str()) ;

  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "morning") ;
	_pPriseMatinTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "noon") ;
  _pPriseMidiTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "evening") ;
  _pPriseSoirTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "bedtime") ;
  _pPriseCoucherTxt->SetText(sTxt.c_str()) ;

  string sLang = pContexte->getUserLanguage() ;

  // Init contents
  //
  string sForme = string("") ;
  string sLabel = string("") ;

  if ((NSphaseMedic*) NULL == _pPhase)
  {
    NVLdVTemps tpNow ;
	  tpNow.takeTime() ;

  	_pDateDebPrescr->setDate(tpNow.donneDateHeure()) ;
    
		return ;
	}

	_pDureePhase->initControle(_pPhase->GetDureePhase()) ;
	_pCBDureePhase->setCode(_pPhase->GetSymBolOfPhase()) ;
  _pDateDebPrescr->setDate(_pPhase->GetStartingDate().donneDateHeure()) ;
  _pDateFinPrescr->setDate(_pPhase->GetClosingDate().donneDateHeure()) ;

  _pRenouvellement->initControle(_pPhase->GetNumberOfRenouvellement()) ;

  std::vector<NSMedicCycleGlobal*>* pCycles = _pPhase->getCycles() ;

  if (((std::vector<NSMedicCycleGlobal*>*) NULL == pCycles) || pCycles->empty())
		return ;

	NSMedicCycleGlobal* pCurrentCycle = *(pCycles->begin()) ;
  NSCircadien* pCirc = pCurrentCycle->GetCycleCircadien() ;
  if (((NSCircadien*) NULL == pCirc) || (pCirc->getType() != MMS))
		return ;

	// unfortunately not possible : pCirc->Load() ;

  BaseCirc* pCircData = pCirc->getData() ;
  if ((BaseCirc*) NULL == pCircData)
		return ;

	CircBaseMMS* pMMSData = dynamic_cast<CircBaseMMS*>(pCircData) ;
  if ((CircBaseMMS*) NULL == pMMSData)
		return ;

  _pPriseMatin->getEditNum()->setText(pMMSData->getMatin()) ;
	_pPriseMidi->getEditNum()->setText(pMMSData->getMidi()) ;
	_pPriseSoir->getEditNum()->setText(pMMSData->getSoir()) ;
	_pPriseCoucher->getEditNum()->setText(pMMSData->getCoucher()) ;
}

void
NSSimpleNewDrugDlg::BuildPatpatho()
{
	_pPPT->vider() ;

	// Insert root (drug name)
	//
	int iColonne = 0 ;
  std::string sMEdicName = _pDrug->getCode() ;

  if (string("") == sMEdicName)
		return ;

  _pPPT->ajoutePatho(sMEdicName, iColonne++) ;

  int iMedicRoot  = iColonne ;

  // Insert dates
  //
  string sDate ;

  _pDateDeb->getDate(&sDate) ;
  if ((string("") != sDate) && (sDate != string("19000000000000")) &&	(sDate != string("00000000000000")))
	{
  	_pPPT->ajoutePatho(string("KOUVR1"), iColonne++) ;
  	Message CodeMsg ;
		CodeMsg.SetUnit(string("2DA021")) ;
		CodeMsg.SetComplement(sDate) ;
		_pPPT->ajoutePatho(string("£D0;19"), &CodeMsg, iColonne++) ;
	}
	iColonne = iMedicRoot ;

	_pDateFin->getDate(&sDate) ;
  if ((string("") != sDate) && (sDate != string("19000000000000")) &&	(sDate != string("00000000000000")))
	{
    _pPPT->ajoutePatho(string("KFERM1"), iColonne++) ;
    Message CodeMsg ;
		CodeMsg.SetUnit(string("2DA021")) ;
		CodeMsg.SetComplement(sDate) ;
		_pPPT->ajoutePatho(string("£D0;19"), &CodeMsg, iColonne++) ;
	}
	iColonne = iMedicRoot ;

  // Take unit
  //
  std::string sTakeUnitCode = _pUnitePrise->getCode() ;
	if (string("") != sTakeUnitCode)
  {
  	_pPPT->ajoutePatho(string("0MEDF1"), iColonne++) ;
    _pPPT->ajoutePatho(sTakeUnitCode, iColonne++) ;
  }
  iColonne = iMedicRoot ;

  // Route
  //
  std::string sRouteCode = _pRoute->getCode() ;
  if (string("") != sRouteCode)
  {
    _pPPT->ajoutePatho(string("0VADM1"), iColonne++) ;
    if (string("£?????") != sRouteCode)
      _pPPT->ajoutePatho(sRouteCode, iColonne++) ;
    else
    {
      Message CodeMsg ;
		  CodeMsg.SetTexteLibre(_pRoute->getRawText()) ;
		  _pPPT->ajoutePatho(sRouteCode, &CodeMsg, iColonne++) ;
    }
  }
  iColonne = iMedicRoot ;

  // Event to take in case of
  //
  std::string sEventCode = _pEvent->getCode() ;
  if (string("") != sEventCode)
  {
    _pPPT->ajoutePatho(string("KEVEI2"), iColonne++) ;
    if (string("£?????") != sEventCode)
      _pPPT->ajoutePatho(sEventCode, iColonne++) ;
    else
    {
      Message CodeMsg ;
		  CodeMsg.SetTexteLibre(_pEvent->getRawText()) ;
		  _pPPT->ajoutePatho(sEventCode, &CodeMsg, iColonne++) ;
    }
  }
  iColonne = iMedicRoot ;

  // Codes
  //
  if (string("") != _sATCCode)
  {
    Message CodeMsg ;
		CodeMsg.SetComplement(_sATCCode) ;
		_pPPT->ajoutePatho(string("6ATC01"), &CodeMsg, iColonne++) ;
  }
  iColonne = iMedicRoot ;

  if (string("") != _sCICode)
  {
    string sLexique = string("6CIS01") ;
    size_t iLen = strlen(_sCICode.c_str()) ;
    if      (7 == iLen)
      sLexique = string("6CIP71") ;
    else if (13 == iLen)
      sLexique = string("6CIPT1") ;

    Message CodeMsg ;
		CodeMsg.SetComplement(_sCICode) ;
		_pPPT->ajoutePatho(sLexique, &CodeMsg, iColonne++) ;
  }
  iColonne = iMedicRoot ;

  // Prescribed for an exonerated condition
  //
  if ((string("") != _sALD) || _bNonSubstituable)
  {
    _pPPT->ajoutePatho(string("LADMI1"), iColonne++) ;
    if (string("") != _sALD)
      _pPPT->ajoutePatho(string("LBARZ1"), iColonne) ;
    if (_bNonSubstituable)
    {
      Message Msg ;
      Msg.SetCertitude(string("WCE001")) ;
      _pPPT->ajoutePatho(string("LSUBS1"), &Msg, iColonne) ;
    }
  }
  iColonne = iMedicRoot ;

  // Insert reason(s) for prescription
  //
  if (false == _aUpdatedLinkedIssues.empty())
  {
    ArrayConcern* pConcerns = (ArrayConcern*) 0 ;

    NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
    if (pPatEnCours && pPatEnCours->getLdvDocument())
      pConcerns = pPatEnCours->getLdvDocument()->getConcerns(ldvframeHealth) ;

    if (pConcerns)
    {
      _pPPT->ajoutePatho(string("0MOTF1"), iColonne++) ;

      EquiItemIter it = _aUpdatedLinkedIssues.begin() ;
      for ( ; _aUpdatedLinkedIssues.end() != it ; it++)
      {
        NSConcern* pConcern = pConcerns->getConcern(**it) ;
        if (pConcern)
          _pPPT->ajoutePatho(pConcern->getLexique(), iColonne) ;
      }
    }
  }
  iColonne = iMedicRoot ;

  // Narcotic or assimilated to
  //
  if (_pBdmDrugInformation && _pBdmDrugInformation->isNarcotic())
  {
    _pPPT->ajoutePatho(string("LTYPA1"), iColonne++) ;
    _pPPT->ajoutePatho(string("ISTUA1"), iColonne++) ;
  }
  iColonne = iMedicRoot ;

	NSPatPathoArray phaseTree(pContexte->getSuperviseur()) ;
  BuildPhaseTree(&phaseTree) ;

  if (false == phaseTree.empty())
		_pPPT->InserePatPatho(_pPPT->end(), &phaseTree, iColonne) ;

  if (string("") != _sFreeText)
	{
    Message CodeMsg ;
		CodeMsg.SetTexteLibre(_sFreeText) ;
		_pPPT->ajoutePatho(string("£C;020"), &CodeMsg, iColonne) ;
	}
}

void
NSSimpleNewDrugDlg::BuildPhaseTree(NSPatPathoArray *pPptPhase)
{
	if ((NSPatPathoArray*) NULL == pPptPhase)
		return ;

	pPptPhase->vider() ;

	int iColonne = 0 ;
  int iColBasePhase = iColonne ;

  // Insert root ("phase")
	//
  pPptPhase->ajoutePatho("KPHAT1", iColonne) ;

  // Insert dates
  //
  string sDate ;
  iColonne = iColBasePhase + 1 ;

  _pDateDebPrescr->getDate(&sDate) ;
  if ((string("") != sDate) && (sDate != string("19000000000000")) &&	(sDate != string("00000000000000")))
	{
  	pPptPhase->ajoutePatho("KOUVR1", iColonne++) ;
  	Message CodeMsg ;
		CodeMsg.SetUnit("2DA021") ;
		CodeMsg.SetComplement(sDate) ;
		pPptPhase->ajoutePatho("£D0;19", &CodeMsg, iColonne++) ;
	}
	iColonne = iColBasePhase + 1 ;

	_pDateFinPrescr->getDate(&sDate) ;
  if ((string("") != sDate) && (sDate != string("19000000000000")) &&	(sDate != string("00000000000000")))
	{
    pPptPhase->ajoutePatho("KFERM1", iColonne++) ;
    Message CodeMsg ;
		CodeMsg.SetUnit("2DA021") ;
		CodeMsg.SetComplement(sDate) ;
		pPptPhase->ajoutePatho("£D0;19", &CodeMsg, iColonne++) ;
	}
	iColonne = iColBasePhase + 1 ;

  // traitement des paramètres de la phase
  // pendant
  int iPhaseDurationValue = _pDureePhase->getValue() ;
	string sPhaseDurationUnit = _pCBDureePhase->getSelCode() ;

  if ((iPhaseDurationValue > 0) && (string("") != sPhaseDurationUnit))
  {
  	pPptPhase->ajoutePatho("VDURE2", iColonne++) ;    // code pour pendant
    createNodeComplement(pPptPhase, "£N0;03", sPhaseDurationUnit, iPhaseDurationValue, iColonne) ;
	}
  iColonne = iColBasePhase + 1 ;

  int iRenewValue = _pRenouvellement->getValue() ;

  if (iRenewValue > 0)
  {
  	pPptPhase->ajoutePatho("VRENO1", iColonne++) ;    // code pour à renouveler
    createNodeComplement(pPptPhase, "£N0;03", "200001", iRenewValue, iColonne) ;
	}
  iColonne = iColBasePhase + 1 ;

  // Cycle insertion
  //
  NSPatPathoArray cycleTree(pContexte->getSuperviseur()) ;
  BuildCycleTree(&cycleTree) ;

  if (false == cycleTree.empty())
  {
  	pPptPhase->ajoutePatho("KCYTR1", iColonne++) ;
		pPptPhase->InserePatPatho(pPptPhase->end(), &cycleTree, iColonne) ;
	}
}

void
NSSimpleNewDrugDlg::BuildCycleTree(NSPatPathoArray *pPptCycle)
{
	if ((NSPatPathoArray*) NULL == pPptCycle)
		return ;

	pPptCycle->vider() ;

  // Insert root ("cycle")
	//
	pPptCycle->ajoutePatho("KRYTH1", 0) ;

	std::string temp ;

  temp = _pPriseMatin->getText() ;
  if (isValidValue(&temp))
		func_create_patho_quant("KMATI1", temp, pPptCycle, 1) ;

	temp = _pPriseMidi->getText() ;
	if (isValidValue(&temp))
		func_create_patho_quant("KMIDI1", temp, pPptCycle, 1) ;

	temp = _pPriseSoir->getText() ;
	if (isValidValue(&temp))
		func_create_patho_quant("KSOIR1", temp, pPptCycle, 1) ;

	temp = _pPriseCoucher->getText() ;
	if (isValidValue(&temp))
		func_create_patho_quant("KCOUC1", temp, pPptCycle, 1) ;
}

bool
NSSimpleNewDrugDlg::ParseMedicament()
{
	if (((NSPatPathoArray*) NULL == _pPPT) || (true == _pPPT->empty()))
		return true ;

	int phasesCount = 0 ;

  PatPathoIter pptIter = _pPPT->begin() ;
	_sLexiqCode = (*pptIter)->getLexique() ; // Recuperation du nom  du medicament

  pptIter++ ;

  while (_pPPT->end() != pptIter)
  {
    std::string temp = (*pptIter)->getLexiqueSens() ;

    if (string("KOUVR") == temp)
    {
    	int iColBase = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

			if (((*pptIter)->getColonne() > iColBase)  &&
          (((*pptIter)->getUnitSens() == "2DA02") ||
                     ((*pptIter)->getUnitSens() == "2DA01")) &&
           ((*pptIter)->getLexiqueSens() == "£D0"))
				_sDateOuverture = (*pptIter)->getComplement() ;
      else
      	return false ;

      pptIter++ ;
    }
    else if (string("KFERM") == temp)
    {
    	int iColBase = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

			if (((*pptIter)->getColonne() > iColBase)  &&
          (((*pptIter)->getUnitSens() == "2DA02") ||
                     ((*pptIter)->getUnitSens() == "2DA01")) &&
           ((*pptIter)->getLexiqueSens() == "£D0"))
				_sDateFermeture = (*pptIter)->getComplement() ;
      else
      	return false ;

    	pptIter++ ;
    }
    else if (string("0MEDF") == temp)
		{
			int iColBase = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

      if ((*pptIter)->getColonne() > iColBase)
				_sPriseUnit = (*pptIter)->getLexique() ;

			pptIter++ ;
    }
    else if (string("0VADM") == temp)
		{
			int iColBase = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

      string sElemLex = (*pptIter)->getLexique() ;
      if (string("£?????") == sElemLex)
        _sFreeTextRoute = (*pptIter)->getTexteLibre() ;
      else
        _sLexiqRoute = sElemLex ;

			pptIter++ ;
    }
    else if (string("KEVEI") == temp)
		{
			int iColBase = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

      string sElemLex = (*pptIter)->getLexique() ;
      if (string("£?????") == sElemLex)
        _sFreeTextEvent = (*pptIter)->getTexteLibre() ;
      else
        _sLexiqEvent = sElemLex ;

			pptIter++ ;
    }
    else if (string("KPHAT") == temp) // Charge une phase
    {
    	// This interface element can only manage single phase treatments
      //
    	phasesCount++ ;
      if (phasesCount > 1)
      	return false ;

    	_pPhase = new NSphaseMedic(pContexte) ;
      if (false == ParsePhase(pptIter))
      	return false ;
    }
    else if ("£RE" == temp) // Proposition Id
    {
      _sReferential = (*pptIter)->getComplement() ;
      pptIter++ ;
    }
    else if ("£C;" == temp)
    {
      _sFreeText = (*pptIter)->getTexteLibre() ;
      pptIter++ ;
    }
    else if ("LADMI" == temp)
    {
      int iColBaseAdmin = (*pptIter)->getColonne() ;
      pptIter++ ;
      while ((_pPPT->end() != pptIter) && ((*pptIter)->getColonne() > iColBaseAdmin))
      {
        std::string tempAdm = (*pptIter)->getLexiqueSens() ;
        if ("LBARZ" == tempAdm)
        {
          _sALD = string("LBARZ") ;
          pptIter++ ;
        }
        else if ("LSUBS" == tempAdm)
        {
          _bNonSubstituable = true ;
          pptIter++ ;
        }
        else
          return false ;
      }
    }
    else if (string("6ATC0") == temp)
    {
      _sATCCode = (*pptIter)->getComplement() ;
      pptIter++ ;
    }
    else if ((string("6CIS0") == temp) ||
             (string("6CIP0") == temp) ||
             (string("6CIP7") == temp) ||
             (string("6CIPT") == temp))
    {
      _sCICode = (*pptIter)->getComplement() ;
      pptIter++ ;
    }
    //
    // Indications for treatment are inserted here, even if it duplicates
    // the link that holds this information... both for to keep history and
    // to ease the search for "drugs prescribed for X"
    //
    else if (string("0MOTF") == temp)
    {
      int iColBaseAdmin = (*pptIter)->getColonne() ;
      pptIter++ ;
      while ((_pPPT->end() != pptIter) && ((*pptIter)->getColonne() > iColBaseAdmin))
        pptIter++ ;
    }
    else
    	return false ;
  }
  return true ;
}

bool
NSSimpleNewDrugDlg::ParsePhase(PatPathoIter &pptIter)
{
	if ((NSphaseMedic*) NULL == _pPhase)
		return false ;

	if (_pPPT->end() == pptIter)
		return true ;

	int iColBase = (*pptIter)->getColonne() ;

  pptIter++ ;
	if (_pPPT->end() == pptIter)
		return false ;

	int cyclesCount = 0 ;

	while ((_pPPT->end() != pptIter) && ((*pptIter)->getColonne() > iColBase))
  {
  	string temp = (*pptIter)->getLexiqueSens() ;

  	if (string("KOUVR") == temp)
    {
    	int iColLevel = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

			if (((*pptIter)->getColonne() > iColLevel)  &&
          (((*pptIter)->getUnitSens() == "2DA02") ||
                     ((*pptIter)->getUnitSens() == "2DA01")) &&
           ((*pptIter)->getLexiqueSens() == "£D0"))
      	_pPhase->setStartingDate((*pptIter)->getComplement()) ;
      else
      	return false ;

      pptIter++ ;
    }
		else if (string("KFERM") == temp)
    {
    	int iColLevel = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

			if (((*pptIter)->getColonne() > iColLevel)  &&
          (((*pptIter)->getUnitSens() == "2DA02") ||
                     ((*pptIter)->getUnitSens() == "2DA01")) &&
           ((*pptIter)->getLexiqueSens() == "£D0"))
      	_pPhase->setClosingDate((*pptIter)->getComplement()) ;
      else
      	return false ;

      pptIter++ ;
    }
    else if (string("VDURE") == temp)
    {
    	int iColLevel = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

			if ((*pptIter)->getColonne() > iColLevel)
      {
      	_pPhase->setSymBolOfPhase((*pptIter)->getUnit()) ;
				_pPhase->setDureePhase(atoi(((*pptIter)->getComplement()).c_str())) ;
      }
      else
      	return false ;

      pptIter++ ;
    }
    else if (string("VRENO") == temp)
    {
    	int iColLevel = (*pptIter)->getColonne() ;

			pptIter++ ;
      if (_pPPT->end() == pptIter)
      	return false ;

			if ((*pptIter)->getColonne() > iColLevel)
				_pPhase->setNumberOfRenouvellement(atoi(((*pptIter)->getComplement()).c_str())) ;
      else
      	return false ;

      pptIter++ ;
    }
    else if (string("KCYTR") == temp)
    {
    	cyclesCount++ ;
      if (cyclesCount > 1)
    		return false ;

      NSMedicCycleGlobal* cycle = new NSMedicCycleGlobal(pContexte, _pPhase) ;
      PatPathoIter pptEnd = _pPPT->end() ;
      cycle->Load(pptIter, pptEnd) ;

      // Check that there is only circadian information
      //
      NSRythme* pRythme = cycle->GetRythme() ;
      if (pRythme && (NULL != pRythme->getData()))
      	return false ;

      NSCircadien* pCirc = cycle->GetCycleCircadien() ;
      if (pCirc && (pCirc->getType() != MMS) &&
                   (pCirc->getType() != UndefinedCircadien))
      	return false ;

      _pPhase->getCycles()->push_back(cycle) ;
    }
    else
    	return false ;
	}
  return true ;
}

/**
 * Set of operation to be executed after a drug has been selected
 */
void
NSSimpleNewDrugDlg::ExecutedAfterDrugSelection()
{
	string sLexiqCode = _pDrug->getCode() ;

  if (string("") == sLexiqCode)
		return ;

  // Display drug's name
  //
	string sLabel ;
	string sLang = pContexte->getUserLanguage() ;
	pContexte->getDico()->donneLibelle(sLang, &sLexiqCode, &sLabel) ;

  if (pContexte->getBamType() != NSContexte::btNone)
  {
    getDrugInformation() ;

    if (_pBdmDrugInformation)
      checkDrugSafety() ;
  }

  // Find and display dispensation unit
  //
  string sMITxt = pContexte->getSuperviseur()->getText("drugDialog", "missingInformation") ;

	string sCodeDisp = initDispUnit(sLang, sLexiqCode, sLabel, pContexte) ;
	if (sCodeDisp != sMITxt) // Verifie que le code exite
		_pUnitePrise->setLabel(sCodeDisp) ;
	else
		_pUnitePrise->SetText(sCodeDisp.c_str()) ;

	// initPosoAndCycleForDrug() ;
}

void
NSSimpleNewDrugDlg::ExecutedAfterTrtBeginDate()
{
	std::string dateDeb ;
  _pDateDeb->getDate(&dateDeb) ;
  NVLdVTemps data ;
  data.initFromDate(dateDeb) ;

  std::string date ;
  _pDateDebPrescr->getDate(&date) ;
  NVLdVTemps dataPrescr ;
  dataPrescr.initFromDate(date) ;

	NVLdVTemps tpsNow ;
	tpsNow.takeTime() ;

  if (data >= tpsNow)
	{
		_pDateDebPrescr->setDate(data.donneDateHeure()) ;
    ActualiseEndOfPrescription() ;
	}
}

void
NSSimpleNewDrugDlg::ExecutedAfterTrtEndDate()
{
	_pNbJours->donneValeur() ;
	int iTrtDurationValue = (int) _pNbJours->_dValeur ;
	if (iTrtDurationValue <= 0)
		return ;

	string sTrtDurationUnit = _pCBDureeTtt->getSelCode() ;
	if (string("") == sTrtDurationUnit)
		return ;

	NVLdVTemps tDateFin ;

  std::string sDateDeb ;
	_pDateDeb->getDate(&sDateDeb) ;

	// si on dans le cas où "dans" est cochée
  if (_pRDans->GetCheck() == BF_CHECKED)
    tDateFin.takeTime() ;

  // si on est dans le cas où "durée" est cochée
  else if (_pRDuree->GetCheck() == BF_CHECKED)
    tDateFin.initFromDate(sDateDeb) ;

  else
  	return ;

	tDateFin.ajouteTemps(iTrtDurationValue, sTrtDurationUnit, pContexte) ;
	string sDateFin = tDateFin.donneDateHeure() ;

  _pDateFin->setDate(sDateFin) ;
  _pDateFinPrescr->setDate(sDateFin) ;

  std::string dateDebPresc ;
  _pDateDebPrescr->getDate(&dateDebPresc) ;

  // si on dans le cas où "dans" est cochée, on remplit la durée de prescription
  if ((_pRDans->GetCheck() == BF_CHECKED) ||
      ((_pRDuree->GetCheck() == BF_CHECKED) && (dateDebPresc == sDateDeb)))
	{
		_pDureePhase->setValue(iTrtDurationValue) ;
    _pCBDureePhase->setCode(sTrtDurationUnit) ;
    ActualiseEndOfPrescription() ;
  }
}

/**
 * Set of operation to be executed after a route has been selected
 */
void
NSSimpleNewDrugDlg::ExecutedAfterRouteSelection()
{
}

/**
 * Set of operation to be executed after an event has been selected
 */
void
NSSimpleNewDrugDlg::ExecutedAfterEventSelection()
{
}

void
NSSimpleNewDrugDlg::ActualisePhase()
{
	ActualiseEndOfPrescription() ;
}

void
NSSimpleNewDrugDlg::ActualiseEndOfPrescription()
{
	int iPhaseDurationValue = _pDureePhase->getValue() ;
	if (iPhaseDurationValue <= 0)
		return ;

	string sPhaseDurationUnit = _pCBDureePhase->getSelCode() ;
	if (string("") == sPhaseDurationUnit)
		return ;

	std::string date ;
  _pDateDebPrescr->getDate(&date) ;
  NVLdVTemps data ;
  data.initFromDate(date) ;

  data.ajouteTemps(iPhaseDurationValue, sPhaseDurationUnit, pContexte) ;

  date = data.donneDateHeure() ;
  _pDateFinPrescr->setDate(date) ;
}

void
NSSimpleNewDrugDlg::UserName(WPARAM wParam)
{
  switch (wParam)
  {
    case (IDR_DRG_CHRONIQUE) :
    	_pDateFin->SetReadOnly(true) ;
      _pNbJours->SetReadOnly(true) ;
      //pCBDureeTtt->SetReadOnly(true) ;
      break ;
    case (IDR_DRG_DANS) :
    	_pDateFin->SetReadOnly(true) ;
      _pNbJours->SetReadOnly(false) ;
      //pCBDureeTtt->SetReadOnly(false) ;
      _pNbJours->SetFocus() ;
      break ;
    case (IDR_DRG_DUREE) :
    	_pDateFin->SetReadOnly(true) ;
      _pNbJours->SetReadOnly(false) ;
      //pCBDureeTtt->SetReadOnly(false) ;
      _pNbJours->SetFocus() ;
      break ;
    case (IDR_DRG_LE) :
    	_pNbJours->SetReadOnly(true) ;
      //pCBDureeTtt->SetReadOnly(true) ;
      _pDateFin->SetReadOnly(false) ;
      _pDateFin->SetFocus() ;
      break ;
  }
}

void
NSSimpleNewDrugDlg::nonRenouvelable()
{
}

void
NSSimpleNewDrugDlg::validateInformation()
{
  // on récupère d'abord un éventuel élément lexique sélectionné par les flêches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (_pDrug->getDico()->getDicoDialog()->EstOuvert())
    _pDrug->getDico()->getDicoDialog()->InsererElementLexique() ;

	if (_pUnitePrise->getDico()->getDicoDialog()->EstOuvert())
    _pUnitePrise->getDico()->getDicoDialog()->InsererElementLexique() ;

  string sTL = string("") ;

  // Ne pas accepter les textes libres
  if (string("£?????") == _pDrug->getCode())
  {
    if (false == pContexte->getEpisodus()->bAllowFreeTextLdV)
    {
    	string sErrorTxt = pContexte->getSuperviseur()->getText("drugDialogErrors", "freeTextNotAllowedForTreatment") ;
      erreur(sErrorTxt.c_str(), standardError, 0, GetHandle()) ;
      _pDrug->SetFocus() ;
      return ;
    }
  }

	if (string("£?????") == _pUnitePrise->getCode())
  {
    string sLabel = _pUnitePrise->getLabel() ;
    NSPatPathoArray pptTest(pContexte->getSuperviseur()) ;
    BuildCycleTree(&pptTest) ;
    if ((string("") != sLabel) || (false == pptTest.empty()))
    {
  	  string sErrorTxt = pContexte->getSuperviseur()->getText("drugDialogErrors", "freeTextNotAllowedForPrescriptionUnit") ;
      erreur(sErrorTxt.c_str(), standardError, 0, GetHandle()) ;
      _pUnitePrise->SetFocus() ;
    }
    return ;
  }

  if (_pALD->GetCheck() == BF_CHECKED)
    _sALD = string("LBARZ1") ;
  else
    _sALD = string("") ;

  if (_pNonSubstituable->GetCheck() == BF_CHECKED)
    _bNonSubstituable = true ;
  else
    _bNonSubstituable = false ;

  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSSimpleNewDrugDlg::CmOk()
{
try
{
  validateInformation() ;
  BuildPatpatho() ;

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSSimpleNewDrugDlg::CmOk.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSSimpleNewDrugDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSSimpleNewDrugDlg::switchToComplexMode()
{
	_bMustSwitchToComplexMode = true ;

  validateInformation() ;
  BuildPatpatho() ;

	CloseWindow(IDOK) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSModifConcernTypeDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSModifConcernTypeDlg, NSUtilDialog)
    EV_COMMAND(IDOK,        CmOk),
    EV_COMMAND(IDCANCEL,    CmCancel),
END_RESPONSE_TABLE;

NSModifConcernTypeDlg::NSModifConcernTypeDlg(LDVFRAME iFrame, TWindow* pView, NSPatPathoInfo* pNode, NSContexte* pCtx)
                      :NSUtilDialog(pView, pCtx, "MODIF_PREO_TYPE")
{
try
{
	pNoeud  = pNode ;
	pType   = new NSUtilLexique(pContexte, this, PREOCCUP_EDIT, pContexte->getDico()) ;
  _iFrame = iFrame ;
}
catch (...)
{
	erreur("Exception NSModifConcernTypeDlg ctor.", standardError, 0) ;
}
}

NSModifConcernTypeDlg::~NSModifConcernTypeDlg()
{
	delete pType ;
}

void
NSModifConcernTypeDlg::SetupWindow()
{
	TDialog::SetupWindow();

	string sCodeLex = pNoeud->getLexique() ;

	if (sCodeLex != string("£?????"))
		pType->setLabel(sCodeLex) ;
	//
	// Texte libre - Free text
	//
	else
		pType->SetText(pNoeud->getTexteLibre().c_str()) ;

  // Texts
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sTitle = pSuper->getText("healthConcernDialog", "changeConcernType") ;
  SetCaption(sTitle.c_str()) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSModifConcernTypeDlg::CmOk()
{
try
{
  //
  // on récupère d'abord un éventuel élément lexique sélectionné par les flêches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (pType->getDico()->getDicoDialog()->EstOuvert())
  {
    pType->getDico()->getDicoDialog()->InsererElementLexique() ;
    return ;
  }
  //
  // Ne pas accepter les textes libres
  //
  if (pType->getCode() == string("£?????"))
  {
    if (false == pContexte->getEpisodus()->bAllowFreeTextLdV)
    {
      erreur("Il faut choisir un code lexique et non pas du texte libre ", standardError, 0, GetHandle()) ;
      pType->SetFocus() ;
      return ;
    }
    pNoeud->setTexteLibre(pType->getRawText()) ;
  }
  else
    pNoeud->setTexteLibre("") ;

  pNoeud->setLexique(pType->getCode()) ;
  pNoeud->setComplement(string("")) ;

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSModifConcernTypeDlg::CmOk.",  standardError, 0) ;
}
}

void
NSModifConcernTypeDlg::CmCancel()
{
  Destroy(IDCANCEL) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSModifConcernDateDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSModifConcernDateDlg, NSUtilDialog)
    EV_COMMAND(IDOK,        CmOk),
    EV_COMMAND(IDCANCEL,    CmCancel),
    EV_COMMAND_AND_ID(IDR_CHRONIQUE, adminDates),
    EV_COMMAND_AND_ID(IDR_DANS,      adminDates),
    EV_COMMAND_AND_ID(IDR_DUREE,     adminDates),
    EV_COMMAND_AND_ID(IDR_LE,        adminDates),
END_RESPONSE_TABLE;

NSModifConcernDateDlg::NSModifConcernDateDlg(LDVFRAME iFrame, TWindow* pView, NVLdVTemps* pDeb, NVLdVTemps* pFin, NSContexte* pCtx)
                      :NSUtilDialog(pView, pCtx, "MODIF_PREO_DATE")
{
try
{
  ptpsDeb      = pDeb ;
  ptpsFin      = pFin ;

  pDateDebText = new OWL::TStatic(this, DATE_DEB_TEXT) ;
  pDateDeb     = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_DEB) ;
  pGroupFin    = new TGroupBox(this, DATE_FIN_TEXT) ;
  pDateFin     = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_FIN) ;
  pNbJours     = new NSEditNum(pContexte, this, IDC_NBJOURS, 10) ;
  pRChronique  = new OWL::TRadioButton(this, IDR_CHRONIQUE) ;
  pRDans       = new OWL::TRadioButton(this, IDR_DANS) ;
  pRDuree      = new OWL::TRadioButton(this, IDR_DUREE) ;
  pRLe         = new OWL::TRadioButton(this, IDR_LE) ;
  pJoursText   = new OWL::TStatic(this, JOURS_TEXT) ;

  _iFrame     = iFrame ;
}
catch (...)
{
  erreur("Exception NSModifConcernDateDlg ctor.", standardError, 0) ;
}
}

NSModifConcernDateDlg::~NSModifConcernDateDlg()
{
  delete pDateDebText ;
  delete pDateDeb ;
  delete pGroupFin ;
  delete pDateFin ;
  delete pNbJours ;
  delete pRChronique ;
  delete pRDans ;
  delete pRDuree ;
  delete pRLe ;
  delete pJoursText ;
}

void
NSModifConcernDateDlg::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  string sDateDeb = ptpsDeb->donneDateHeure() ;
  pDateDeb->setDate(sDateDeb.c_str()) ;

  if ((ptpsFin->estVide()) || (ptpsFin->estNoLimit()))
  {
    pRChronique->SetCheck(BF_CHECKED) ;

    NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;
    string sDateNow = tpsNow.donneDateHeure() ;
    pDateFin->setDate(sDateNow) ;
  }
  else
  {
    string sDateFin = ptpsFin->donneDateHeure() ;
    pDateFin->setDate(sDateFin) ;
    pRLe->SetCheck(BF_CHECKED) ;
  }

  pNbJours->SetText("0") ;

  pDateFin->SetReadOnly(false) ;
  pNbJours->SetReadOnly(true) ;

  // Texts
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sTitle = pSuper->getText("healthConcernDialog", "changeDates") ;
  SetCaption(sTitle.c_str()) ;

  string sText = pSuper->getText("healthConcernDialog", "startingDate") ;
  pDateDebText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "endingDate") ;
  pGroupFin->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "undetermined") ;
  pRChronique->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "fromNow") ;
  pRDans->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "totalDuration") ;
  pRDuree->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "onThe") ;
  pRLe->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "days") ;
  pJoursText->SetCaption(sText.c_str()) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSModifConcernDateDlg::CmOk()
{
try
{
  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

  string sDDeb;
  pDateDeb->getDate(&sDDeb);
  ptpsDeb->initFromDateHeure(sDDeb) ;

  string sDFin;

  // récupération de la valeur du champs edit correspondant au nombre de jours
  pNbJours->donneValeur() ;

  // si on est dans le cas où "chronique" est cochée
  if (pRChronique->GetCheck() == BF_CHECKED)
    ptpsFin->setNoLimit() ;

  // si on dans le cas où "dans" est cochée
  else if (pRDans->GetCheck() == BF_CHECKED)
  {
    if (pNbJours->_dValeur < 1)
    {
      erreur("La préoccupation doit durer au moins 1 jour.", standardError, 0, GetHandle());
      return;
    }
    ptpsFin->takeTime() ;
    ptpsFin->ajouteJours(pNbJours->_dValeur) ;
  }

  // si on est dans le cas où "durée" est cochée
  else if (pRDuree->GetCheck() == BF_CHECKED)
  {
    if (pNbJours->_dValeur < 1)
    {
      erreur("La préoccupation doit durer au moins 1 jour.", standardError, 0, GetHandle());
      return;
    }
    ptpsFin->initFromDateHeure(sDDeb) ;
    ptpsFin->ajouteJours(pNbJours->_dValeur) ;
  }

  // si on est dans le cas où "le" est cochée
  if (pRLe->GetCheck() == BF_CHECKED)
  {
    pDateFin->getDate(&sDFin) ;
    ptpsFin->initFromDateHeure(sDFin) ;
  }

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSModifConcernDateDlg::CmOk.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
NSModifConcernDateDlg::CmCancel()
{
  Destroy(IDCANCEL) ;
}

void
NSModifConcernDateDlg::adminDates(WPARAM wParam)
{
  switch (wParam)
  {
    case (IDR_CHRONIQUE) :
        pDateFin->SetReadOnly(true) ;
        pNbJours->SetReadOnly(true) ;
        break ;
    case (IDR_DANS) :
        pDateFin->SetReadOnly(true) ;
        pNbJours->SetReadOnly(false) ;
        break ;
    case (IDR_DUREE) :
        pDateFin->SetReadOnly(true) ;
        pNbJours->SetReadOnly(false) ;
        break ;
    case (IDR_LE) :
        pNbJours->SetReadOnly(true) ;
        pDateFin->SetReadOnly(false) ;
        break ;
  }
}

// -----------------------------------------------------------------
//
//  Méthodes de NSModifConcernIndexDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSModifConcernIndexDlg, NSUtilDialog)
    EV_COMMAND(IDOK,        CmOk),
    EV_COMMAND(IDCANCEL,    CmCancel),
    EV_CHILD_NOTIFY_ALL_CODES(IDC_GRAVITE,  UpdateSeverity),
END_RESPONSE_TABLE;

NSModifConcernIndexDlg::NSModifConcernIndexDlg(LDVFRAME iFrame, TWindow* pView, int* pSev, NSContexte* pCtx)
                       :NSUtilDialog(0, pCtx, "MODIF_PREO_INDEX")
{
try
{
  piSeverite = pSev ;

  pSeverityText = new OWL::TStatic(this, SEVER_TEXT) ;
  pGravite 	    = new OWL::THSlider(this, IDC_GRAVITE) ;

  pGravGauge    = new OWL::TGauge(this, "%d%%", IDC_PROGRESSGRAV, 24, 45, 215, 20) ;
  pGravGauge->SetRange(0, 100) ;
  pGravGauge->SetNativeUse(nuNever) ;

  _iFrame = iFrame ;
}
catch (...)
{
  erreur("Exception NSModifConcernIndexDlg ctor.", standardError, 0) ;
}
}

NSModifConcernIndexDlg::~NSModifConcernIndexDlg()
{
  delete pGravite ;
  delete pGravGauge ;
  delete pSeverityText ;
}

void
NSModifConcernIndexDlg::SetupWindow()
{
  TDialog::SetupWindow() ;

  pGravite->SetRange(0, 100) ;   // fixer min et max
  pGravite->SetRuler(5, false) ; // espacement entre deux graduations
  pGravite->SetPosition(*piSeverite) ;
  pGravGauge->SetValue(*piSeverite) ;

  // Texts
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sTitle = pSuper->getText("healthConcernDialog", "changeSeverityLevel") ;
  SetCaption(sTitle.c_str()) ;

  string sText = pSuper->getText("healthConcernDialog", "severityIndex") ;
  pSeverityText->SetCaption(sText.c_str()) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSModifConcernIndexDlg::CmOk()
{
try
{
  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

  *piSeverite = pGravite->GetPosition() ;
    // *piRisque   = pRisque->GetPosition();

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSModifConcernIndexDlg::CmOk.", standardError, 0) ;
}
}

void
NSModifConcernIndexDlg::CmCancel()
{
  Destroy(IDCANCEL) ;
}

void
NSModifConcernIndexDlg::UpdateSeverity(uint)
{
  int setting = pGravite->GetPosition() ;

  if      (setting < 25)
    pGravGauge->SetColor(NS_CLASSLIB::TColor(0, 0, 128)) ;   // Blue
  else if (setting < 50)
    pGravGauge->SetColor(NS_CLASSLIB::TColor(0, 128, 0)) ;   // Green
  else if (setting < 75)
    pGravGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 0)) ; // Yellow
  else
    pGravGauge->SetColor(NS_CLASSLIB::TColor(128, 0, 0)) ;   // Red

  pGravGauge->SetValue(setting) ;
}

/*******************************************
void
NSModifConcernIndexDlg::UpdateRisk(uint)
{
    int setting = pRisque->GetPosition();

    if      (setting < 10)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(0, 0, 128));
    else if (setting < 15)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(0, 128, 0));
    else if (setting < 20)
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 0));
    else
        pRiskGauge->SetColor(NS_CLASSLIB::TColor(128, 0, 0));

    pRiskGauge->SetValue(setting);
}
*******************************************/

// -----------------------------------------------------------------
//
//  Méthodes de NSModifConcernIndexDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSEvolConcernIndexDlg, NSUtilDialog)
    EV_COMMAND(IDOK,        CmOk),
    EV_COMMAND(IDCANCEL,    CmCancel),
    EV_CHILD_NOTIFY_ALL_CODES(IDC_GRAVITE,  UpdateSeverity),
END_RESPONSE_TABLE;

NSEvolConcernIndexDlg::NSEvolConcernIndexDlg(LDVFRAME iFrame, TWindow* pView, NVLdVTemps* pDeb, int* pSev, NSContexte* pCtx)
                      :NSUtilDialog(0, pCtx, "EVOL_PREO_INDEX")
{
try
{
  ptpsDeb    = pDeb ;
  piSeverite = pSev ;

  pDateDebText  = new OWL::TStatic(this, DATE_DEB_TEXT) ;
  pDate         = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_DEB) ;
  pSeverityText = new OWL::TStatic(this, SEVER_TEXT) ;
  pGravite 	    = new OWL::THSlider(this, IDC_GRAVITE) ;

  pGravGauge = new OWL::TGauge(this, "%d%%", IDC_PROGRESSGRAV, 24, 74, 215, 20) ;
  pGravGauge->SetRange(0, 100) ;
  pGravGauge->SetNativeUse(nuNever) ;
}
catch (...)
{
  erreur("Exception NSModifConcernIndexDlg ctor.", standardError, 0) ;
}
}

NSEvolConcernIndexDlg::~NSEvolConcernIndexDlg()
{
  delete pDate ;
  delete pGravite ;
  delete pGravGauge ;
  delete pDateDebText ;
  delete pSeverityText ;
}

void
NSEvolConcernIndexDlg::SetupWindow()
{
  TDialog::SetupWindow() ;

  string sDateDeb = ptpsDeb->donneDateHeure() ;
  pDate->setDate(sDateDeb.c_str()) ;

  pGravite->SetRange(0, 100) ;   //fixer min et max
  pGravite->SetRuler(5, false) ; //espacement entre deux graduations
  pGravite->SetPosition(*piSeverite) ;
  pGravGauge->SetValue(*piSeverite) ;

  // Texts
  //
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  string sTitle = pSuper->getText("healthConcernDialog", "severityLevelEvolution") ;
  SetCaption(sTitle.c_str()) ;

  string sText = pSuper->getText("healthConcernDialog", "date") ;
  pDateDebText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("healthConcernDialog", "severityIndex") ;
  pSeverityText->SetCaption(sText.c_str()) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSEvolConcernIndexDlg::CmOk()
{
try
{
  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

  string sDDeb ;
  pDate->getDate(&sDDeb) ;
  ptpsDeb->initFromDate(sDDeb) ;

  *piSeverite = pGravite->GetPosition() ;
    // *piRisque   = pRisque->GetPosition();

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSModifConcernIndexDlg::CmOk.", standardError, 0) ;
}
}

void
NSEvolConcernIndexDlg::CmCancel()
{
  Destroy(IDCANCEL) ;
}

void
NSEvolConcernIndexDlg::UpdateSeverity(uint)
{
  int setting = pGravite->GetPosition() ;

  if      (setting < 25)
    pGravGauge->SetColor(NS_CLASSLIB::TColor(0, 0, 128)) ;   // Blue
  else if (setting < 50)
    pGravGauge->SetColor(NS_CLASSLIB::TColor(0, 128, 0)) ;   // Green
  else if (setting < 75)
    pGravGauge->SetColor(NS_CLASSLIB::TColor(128, 128, 0)) ; // Yellow
  else
    pGravGauge->SetColor(NS_CLASSLIB::TColor(128, 0, 0)) ;   // Red

  pGravGauge->SetValue(setting) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSModifConcern
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSModifConcernDlg, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_COMMAND_AND_ID(IDR_CHRONIQUE, UserName),
    EV_COMMAND_AND_ID(IDR_DANS, UserName),
    EV_COMMAND_AND_ID(IDR_DUREE, UserName),
    EV_COMMAND_AND_ID(IDR_LE, UserName),
END_RESPONSE_TABLE;

NSModifConcernDlg::NSModifConcernDlg(LDVFRAME iFrame, NSLdvViewBase* pView, NSContexte* pCtx, NSConcern* pPb)
                  :NSUtilDialog((TWindow*) pView, pCtx, "MODIF_PROBLEM")
{
try
{
  pLdvView    = pView;

  pType       = new NSUtilLexique(pContexte, this, PREOCCUP_EDIT, pContexte->getDico()) ;
  pDateDeb    = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_DEB) ;
  pDateFin    = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_FIN) ;
  pNbJours    = new NSEditNum(pContexte, this, IDC_NBJOURS, 10) ;
  pRChronique = new OWL::TRadioButton(this, IDR_CHRONIQUE) ;
  pRDans      = new OWL::TRadioButton(this, IDR_DANS) ;
  pRDuree     = new OWL::TRadioButton(this, IDR_DUREE) ;
  pRLe        = new OWL::TRadioButton(this, IDR_LE) ;

  pConcern    = pPb ;

  _iFrame     = iFrame ;
}
catch (...)
{
  erreur("Exception NSModifConcernDlg ctor.", standardError, 0) ;
}
}

NSModifConcernDlg::~NSModifConcernDlg()
{
  delete pType ;
  delete pDateDeb ;
  delete pDateFin ;
  delete pNbJours ;
  delete pRChronique ;
  delete pRDans ;
  delete pRDuree ;
  delete pRLe ;
}

void
NSModifConcernDlg::SetupWindow()
{
  TDialog::SetupWindow() ;

  string sDateDeb = pConcern->_tDateOuverture.donneDateHeure() ;
  pDateDeb->setDate(sDateDeb.c_str()) ;

  if ((pConcern->_tDateFermeture.estVide()) ||
      (pConcern->_tDateFermeture.estNoLimit()))
    pRChronique->SetCheck(BF_CHECKED) ;
  else
  {
    string sDateFin = pConcern->_tDateFermeture.donneDateHeure();
    pDateFin->setDate(sDateFin.c_str()) ;
    pRLe->SetCheck(BF_CHECKED) ;
  }

  pNbJours->SetText("0") ;

  pDateFin->SetReadOnly(false) ;
  pNbJours->SetReadOnly(true) ;

  // Recherche de cette préoccupation dans l'Array
  // Looking for this health upset in the Array
  //
  if (NULL == pConcern->_pDoc)
    return ;

  NSPatPathoArray PptIndex(pContexte->getSuperviseur()) ;
  pConcern->_pDoc->initFromFrameIndex(_iFrame, &PptIndex) ;

  if (PptIndex.empty())
    return ;

  PatPathoIter iter = PptIndex.begin() ;
  while ((PptIndex.end() != iter) &&
            ((*iter)->getNode() != pConcern->getNoeud()))
    iter++ ;

  // not found !
  if (PptIndex.end() == iter)
    return ;

  sLexique = (*iter)->getLexique() ;
  pType->setLabel(sLexique) ;
}

void
NSModifConcernDlg::UserName(WPARAM wParam)
{
  switch (wParam)
  {
    case (IDR_CHRONIQUE) :
        pDateFin->SetReadOnly(true) ;
        pNbJours->SetReadOnly(true) ;
        break ;
    case (IDR_DANS) :
        pDateFin->SetReadOnly(true) ;
        pNbJours->SetReadOnly(false) ;
        break ;
    case (IDR_DUREE) :
        pDateFin->SetReadOnly(true) ;
        pNbJours->SetReadOnly(false) ;
        break ;
    case (IDR_LE) :
        pNbJours->SetReadOnly(true) ;
        pDateFin->SetReadOnly(false) ;
        break ;
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSModifConcernDlg::CmOk()
{
try
{
	//
	// on récupère d'abord un éventuel élément lexique sélectionné par les flêches
	// Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
	if (pType->getDico()->getDicoDialog()->EstOuvert())
	{
		pType->getDico()->getDicoDialog()->InsererElementLexique() ;
		return ;
	}
	//
	// Ne pas accepter les textes libres
	//
	if (pType->getCode() == string("£?????"))
	{
		erreur("Il faut choisir un code lexique et non pas du texte libre ", standardError, 0, GetHandle()) ;
		pType->SetFocus() ;
		return ;
	}

  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

	string sDDeb ;
	pDateDeb->getDate(&sDDeb) ;

	string sDFin ;

	// récupération de la valeur du champs edit correspondant au nombre de jours
	pNbJours->donneValeur() ;

	// si on est dans le cas où "chronique" est cochée
	if (pRChronique->GetCheck() == BF_CHECKED)
		sDFin = "00000000000000" ;

	// si on dans le cas où "dans" est cochée
	if (pRDans->GetCheck() == BF_CHECKED)
	{
  	NVLdVTemps  tDateFin ;
    tDateFin.takeTime() ;
    tDateFin.ajouteJours(pNbJours->_dValeur) ;
    sDFin = tDateFin.donneDateHeure() ;
	}

	// si on est dans le cas où "durée" est cochée
	if (pRDuree->GetCheck() == BF_CHECKED)
	{
  	NVLdVTemps  tDateFin ;
    tDateFin.initFromDate(sDDeb) ;
    tDateFin.ajouteJours(pNbJours->_dValeur) ;
    sDFin = tDateFin.donneDateHeure() ;
	}

	// si on est dans le cas où "le" est cochée
	if (pRLe->GetCheck() == BF_CHECKED)
		pDateFin->getDate(&sDFin) ;

	//
	// Le titre de la préoccupation a changé
	// Health concern's title changed
	//
	if (pType->getCode() != sLexique)


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	// maintenant il faut modifier le problème dans l'index
	//
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//
	// Positionnement du pointeur sur l'index
	//
	// NSPatPathoArray* pPathoIndex = 0 ;

	if ((pContexte->getPatient()) && (pContexte->getPatient()->getDocHis()))
	{
    NSDocHistoArray* pVectDocument = pContexte->getPatient()->getDocHis()->getVectDocument() ;
	  if (pVectDocument && (false == pVectDocument->empty()))
		{
			//
			// Enumération des documents
			//
			bool bContinuer = true ;
			DocumentIter iterDoc = pVectDocument->begin() ;
			while ((pVectDocument->end() != iterDoc) && bContinuer)
			{
        NSPatPathoInfo* pRootPathoInfo = (*iterDoc)->getRootPathoInfo() ;

				if (pRootPathoInfo)
				{
          if (pRootPathoInfo->getLexique() == string("ZPOMR1"))
          {
          	NSDocumentInfo Docum(pContexte) ;

            NSDocumentData InfoData ;
            (*iterDoc)->initFromData(&InfoData) ;
	          Docum.setData(&InfoData) ;

            NSNoyauDocument Noyau(0, &Docum, 0, pContexte, false, true) ;

            NSPatPathoArray PptNoyau(pContexte->getSuperviseur()) ;
            Noyau.initFromPatPatho(&PptNoyau) ;

            //
            // On cherche le chapitre "préoccupations de santé"
            //
            PatPathoIter iter = PptNoyau.ChercherItem("0PRO11") ;

            if ((NULL != iter) && (PptNoyau.end() != iter))
            {
            	int colonneMere = (*iter)->getColonne() ;

              iter++ ;
              NSPatPathoInfo* pRepere = 0 ;
              if (PptNoyau.end() != iter)
              	pRepere = *iter ;

              Message CodeMsg("") ;
              CodeMsg.SetLexique(pType->getCode()) ;
              int iDecalLigne ;
              if (PptNoyau.end() != iter)
              	iDecalLigne = 0 ;
              else
              	iDecalLigne = 1 ;
              PptNoyau.ajoutePatho(iter, pType->getCode(), &CodeMsg, colonneMere+1,
                                                   iDecalLigne, true) ;
              //
              // Date de début
              //
              if ((sDDeb != "") &&
                  (sDDeb != string("19000000000000")) &&
                  (sDDeb != string("00000000000000")))
              {
              	//
                // Recalage
                //
                if (pRepere == 0)
                	iter = PptNoyau.end() ;
                else
                {
                	iter = PptNoyau.begin() ;
                  while ((PptNoyau.end() != iter) && (*iter != pRepere))
                  	iter++ ;
                }

                Message CodeMsg2("") ;

                CodeMsg2.SetLexique("KOUVR1") ;
                int iDecalLigne ;
                if (PptNoyau.end() != iter)
                	iDecalLigne = 0 ;
                else
                	iDecalLigne = 1 ;
                PptNoyau.ajoutePatho(iter, "KOUVR1", &CodeMsg2, colonneMere+2,
                                                   iDecalLigne, true) ;

                //
                // Recalage
                //
                if (0 == pRepere)
                	iter = PptNoyau.end() ;
                else
                {
                	iter = PptNoyau.begin() ;
                  while ((PptNoyau.end() != iter) && (*iter != pRepere))
                  	iter++ ;
                }

                CodeMsg2.Reset() ;
                CodeMsg2.SetLexique("£T0;19");
                CodeMsg2.SetComplement(sDDeb);
                CodeMsg2.SetUnit("2DA021");
                PptNoyau.ajoutePatho(iter, "£T0;19", &CodeMsg2, colonneMere+3,
                                                   iDecalLigne, false) ;
              }

              //
              // Date de fin
              //
              if ((sDFin != "") &&
                  (sDFin != string("19000000000000")) &&
                  (sDFin != string("00000000000000")))
              {
              	//
                // Recalage
                //
                if (pRepere == 0)
                	iter = PptNoyau.end() ;
                else
                {
                	iter = PptNoyau.begin() ;
                  while ((PptNoyau.end() != iter) && (*iter != pRepere))
                  	iter++ ;
                }

                Message CodeMsg2("") ;

                CodeMsg2.SetLexique("KFERM1") ;
                int iDecalLigne ;
                if (PptNoyau.end() != iter)
                	iDecalLigne = 0 ;
                else
                	iDecalLigne = 1 ;
                PptNoyau.ajoutePatho(iter, "KFERM1", &CodeMsg2, colonneMere+2,
                                                   iDecalLigne, true) ;

                //
                // Recalage
                //
                if (pRepere == 0)
                	iter = PptNoyau.end() ;
                else
              	{
                	iter = PptNoyau.begin() ;
                  while ((PptNoyau.end() != iter) && (*iter != pRepere))
                  	iter++ ;
                }

                CodeMsg2.Reset() ;
                CodeMsg2.SetLexique("£D0;19") ;
                CodeMsg2.SetComplement(sDFin) ;
                CodeMsg2.SetUnit("2DA021") ;
                PptNoyau.ajoutePatho(iter, "£D0;19", &CodeMsg2, colonneMere+3,
                                                   iDecalLigne, false) ;
              }
              //
              // Enregistrement du document modifié
              //
              Noyau.setPatPatho(&PptNoyau) ;
              Noyau.enregistrePatPatho() ;
              //
              // Mise à jour de l'historique
              //
              /* bool bReload = */ Noyau.chargePatPatho() ;
              Noyau.initFromPatPatho(&PptNoyau) ;
              pContexte->getPatient()->getDocHis()->Rafraichir(&Docum, &PptNoyau, &Noyau) ;
            }
            bContinuer = false ;
          }
          else
          	iterDoc++ ;
				}
			}
		}
	}

        // *sType = pType->sCode ;
	CloseWindow(IDOK) ;
}
catch (...)
{
	erreur("Exception NSModifConcernDlg::CmOk.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
NSModifConcernDlg::CmCancel()
{
  Destroy(IDCANCEL) ;
}

///////////////////////////////////////////////////////////////
//
// Classe NSMedic
//
///////////////////////////////////////////////////////////////

NSMedic::NSMedic()
{
  name    = string("") ;
  node    = string("") ;
  code    = string("") ;
  datedeb = string("") ;
  datefin = string("") ;
}

NSMedic::NSMedic(string sName, string sNode)
{
  name    = sName ;
  node    = sNode ;
  code    = string("") ;
  datedeb = string("") ;
  datefin = string("") ;
}

NSMedic::~NSMedic()
{
}

NSMedic::NSMedic(const NSMedic& rv)
{
  name    = rv.name ;
  node    = rv.node ;
  code    = rv.code ;
  datedeb = rv.datedeb ;
  datefin = rv.datefin ;
}

NSMedic&
NSMedic::operator=(const NSMedic& src)
{
  if (this == &src)
  	return *this ;

  name    = src.name ;
  node    = src.node ;
  code    = src.code ;
  datedeb = src.datedeb ;
  datefin = src.datefin ;

  return *this ;
}

// Classe NSMedicArray
//////////////////////////////////////////

//---------------------------------------------------------------------------
//  Constructeur copie//---------------------------------------------------------------------------
NSMedicArray::NSMedicArray(const NSMedicArray& rv) : NSMedicVector(){  if (false == rv.empty())	  for (NSMedicConstIter i = rv.begin() ; rv.end() != i ; i++)      push_back(new NSMedic(*(*i))) ;}
NSMedicArray&
NSMedicArray::operator=(const NSMedicArray& src)
{  if (this == &src)  	return *this ;  if (false == src.empty())	  for (NSMedicConstIter i = src.begin() ; src.end() != i ; i++)      push_back(new NSMedic(*(*i))) ;  return *this ;}

//---------------------------------------------------------------------------//  Destructeur//---------------------------------------------------------------------------
voidNSMedicArray::vider(){	if (false == empty())		for (NSMedicIter i = begin() ; end() != i ; )    {			delete *i ;			erase(i) ;    }}
NSMedicArray::~NSMedicArray(){	vider() ;}

///////////////////////////////////////////////////////////////
//
// Classe NSObjectif
//
///////////////////////////////////////////////////////////////

NSObjectif::NSObjectif()
{
  _sName         = string("") ;
  _sNode         = string("") ;
  _sCode         = string("") ;
  _iRythme       = 0 ;
  _sDateAuto     = string("") ;
  _sDateCons     = string("") ;
  _sDateIdeal    = string("") ;
  _sDateIdealMax = string("") ;
  _sDateConsMax  = string("") ;
  _sDateCrit     = string("") ;
  _sDatePrec     = string("") ;
  _sValue        = string("") ;
  _iLevel        = 0 ;
  _pCorrespGoal  = (NSLdvGoal	*) 0 ;
}

NSObjectif::NSObjectif(string sName, string sNode)
{
  _sName         = sName ;
  _sNode         = sNode ;
  _sCode         = string("") ;
  _iRythme       = 0 ;
  _sDateAuto     = string("") ;
  _sDateCons     = string("") ;
  _sDateIdeal    = string("") ;
  _sDateIdealMax = string("") ;
  _sDateConsMax  = string("") ;
  _sDateCrit     = string("") ;
  _sDatePrec     = string("") ;
  _sValue        = string("") ;
  _iLevel        = 0 ;
  _pCorrespGoal  = (NSLdvGoal	*) 0 ;
}

NSObjectif::~NSObjectif()
{
}

NSObjectif::NSObjectif(const NSObjectif& rv)
{
  _sName         = rv._sName ;
  _sNode         = rv._sNode ;
  _sCode         = rv._sCode ;
  _iRythme       = rv._iRythme ;
  _sDateAuto     = rv._sDateAuto ;
  _sDateCons     = rv._sDateCons ;
  _sDateIdeal    = rv._sDateIdeal ;
  _sDateIdealMax = rv._sDateIdealMax ;
  _sDateConsMax  = rv._sDateConsMax ;
  _sDateCrit     = rv._sDateCrit ;
  _sDatePrec     = rv._sDatePrec ;
  _sValue        = rv._sValue ;
  _iLevel        = rv._iLevel ;

	_pCorrespGoal  = rv._pCorrespGoal ;
}

NSObjectif&
NSObjectif::operator=(const NSObjectif& src)
{
  if (this == &src)
  	return *this ;

  _sName         = src._sName ;
  _sNode         = src._sNode ;
  _sCode         = src._sCode ;
  _iRythme       = src._iRythme ;
  _sDateAuto     = src._sDateAuto ;
  _sDateCons     = src._sDateCons ;
  _sDateIdeal    = src._sDateIdeal ;
  _sDateIdealMax = src._sDateIdealMax ;
  _sDateConsMax  = src._sDateConsMax ;
  _sDateCrit     = src._sDateCrit ;
  _sDatePrec     = src._sDatePrec ;
  _sValue        = src._sValue ;
  _iLevel        = src._iLevel ;

	_pCorrespGoal  = src._pCorrespGoal ;

  return *this ;
}

// Classe NSObjectifArray
//////////////////////////////////////////

//---------------------------------------------------------------------------
//  Constructeur copie//---------------------------------------------------------------------------  /*
NSObjectifArray::NSObjectifArray(NSObjectifArray& rv) : NSObjectifVector(){    if (!rv.empty())	for (NSObjectifIter i = rv.begin(); i != rv.end(); i++)   	    push_back(new NSObjectif(*(*i)));}
NSObjectifArray&
NSObjectifArray::operator=(NSObjectifArray src)
{    if (!src.empty())	for (NSObjectifIter i = src.begin(); i != src.end(); i++)   	    push_back(new NSObjectif(*(*i)));    return *this;}

//---------------------------------------------------------------------------//  Destructeur//---------------------------------------------------------------------------
voidNSObjectifArray::vider(){    if (!empty())	for (NSObjectifIter i = begin(); i != end(); )    {   	    delete *i;        erase(i);    }}
NSObjectifArray::~NSObjectifArray(){	vider();}      */

//***************************************************************************
//
//  							Méthodes de NSConcernPropertyWindow//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSConcernPropertyWindow, TListWindow)   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Function: NSConcernPropertyWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------

voidNSConcernPropertyWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TLwHitTestInfo info(point);

    HitTest(info);
    if (info.GetFlags() & LVHT_ONITEM)        pDlg->CmModifier();
}

//---------------------------------------------------------------------------//  Function: NSConcernPropertyWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------

intNSConcernPropertyWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;}

//***************************************************************************//
//  Méthodes de NSConcernPropertyDlg
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSConcernPropertyDlg, NSUtilDialog)    EV_LV_DISPINFO_NOTIFY(IDC_LISTVIEW_ACTUEL, LVN_GETDISPINFO, DispInfoListeActuel),
    EV_LV_DISPINFO_NOTIFY(IDC_LISTVIEW_ANCIEN, LVN_GETDISPINFO, DispInfoListeAncien),
    EV_LV_DISPINFO_NOTIFY(IDC_LISTVIEW_GOALS, LVN_GETDISPINFO, DispInfoListeExams),
    EV_BN_CLICKED(IDC_GLOBAL, CmGlobal),
    EV_BN_CLICKED(IDC_LOCAL, CmLocal),
    EV_COMMAND(IDC_NOUVEAU, CmNouveau),
    EV_COMMAND(IDC_RENOUV, CmRenouveler),
    EV_COMMAND(IDC_MODIFIER, CmModifier),
    EV_COMMAND(IDC_ARRETER, CmArreter),
    EV_COMMAND(IDC_ORDONNANCE, CmOrdonnance),
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    // EV_WM_DRAWITEM,
END_RESPONSE_TABLE;

NSConcernPropertyDlg::NSConcernPropertyDlg(NSLdvViewBase* pView, NSContexte* pCtx, NSConcern* pPb)
                     :NSUtilDialog((TWindow*) pView, pCtx, "PROP_PREO")
{
  pLdvView        = pView ;
  pConcern        = pPb ;
  pOldConcern     = 0;
  pListeActuel    = new NSConcernPropertyWindow(this, IDC_LISTVIEW_ACTUEL) ;
  pListeAncien    = new NSConcernPropertyWindow(this, IDC_LISTVIEW_ANCIEN) ;
  pListeExams     = new NSConcernPropertyWindow(this, IDC_LISTVIEW_GOALS) ;

  pListGroup      = new TGroupBox(this, IDC_LIST_GROUP);
  pGlobal         = new TRadioButton(this, IDC_GLOBAL, pListGroup);
  pLocal          = new TRadioButton(this, IDC_LOCAL, pListGroup);
  pNouveau        = new TButton(this, IDC_NOUVEAU) ;
  pRenouveler     = new TButton(this, IDC_RENOUV);
  pModifier       = new TButton(this, IDC_MODIFIER);
  pArreter        = new TButton(this, IDC_ARRETER);
  pOrdonnance     = new TButton(this, IDC_ORDONNANCE);

  pMedicActuel    = new NSMedicArray() ;
  pMedicAncien    = new NSMedicArray() ;
  pExams          = new NSObjectifArray() ;

  // In which frame are we working on?
  //
  if (NULL == pConcern)
    _iFrame = ldvframeNotSpecified ;
  else
    _iFrame = pLdvView->getDoc()->getFrameTypeFromRoot(pConcern->getNoeud()) ;
}

NSConcernPropertyDlg::~NSConcernPropertyDlg()
{
  delete pListeActuel ;
  delete pListeAncien ;
  delete pListeExams ;
  delete pMedicActuel ;
  delete pMedicAncien ;
  delete pExams ;
  delete pGlobal ;
  delete pLocal ;
  delete pListGroup ;
  delete pNouveau ;
  delete pRenouveler ;
  delete pModifier ;
  delete pArreter ;
  delete pOrdonnance ;
}

void
NSConcernPropertyDlg::SetupWindow()
{
    NSUtilDialog::SetupWindow() ;
    if (pConcern)
    {
        pLocal->Check();
        bGlobal = false;
    }
    else
    {
        pGlobal->Check();
        bGlobal = true;
    }

    InitListeActuel() ;
    InitListeAncien() ;
    InitListeExams() ;
    InitListes();
}

void
NSConcernPropertyDlg::InitListes()
{
try
{
  NSFrameInformationArray *pFrames = pLdvView->getDoc()->getFrames() ;
  //
  // Médicaments - Drugs
  //
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
  VecteurString VectString ;
  if (NULL == pConcern)
  {
    // on récupère ici l'ensemble des noeuds médicaments liés à chaque préoccupation
    //
    if ((NULL == pFrames) || pFrames->empty())
      return ;

    for (int i = 0 ; i < FRAMECOUNT ; i++)
    {
      LDVFRAME iFrame = getFrameForIndex(i) ;
      NSFrameInformation *pFrameInfo = pFrames->getFrameInformation(iFrame) ;
      if (pFrameInfo)
      {
        ArrayConcern *pConcerns = pFrameInfo->getConcernsArray() ;
        if (pConcerns && (false == pConcerns->empty()))
          for (ArrayConcernIter k = pConcerns->begin(); pConcerns->end() != k; k++)
            pGraphe->TousLesVrais((*k)->getNoeud(), NSRootLink::drugOf, &VectString, "ENVERS") ;
      }
    }
  }
  else
    pGraphe->TousLesVrais(pConcern->getNoeud(), NSRootLink::drugOf, &VectString, "ENVERS") ;

  pMedicActuel->vider() ;
  pMedicAncien->vider() ;
  pExams->vider() ;
  nbMedicActuel = 0 ;
  nbMedicAncien = 0 ;
  nbExams = 0 ;

  // récupération de la date du jour au format AAAAMMJJ
  string sDateJour = donne_date_duJour() ;

  if (false == VectString.empty())
  {
    for (EquiItemIter i = VectString.begin(); i != VectString.end(); i++)
    {
      NSMedic* pMedic = new NSMedic;

      if (InitMedicament(pMedic, *(*i)))
      {
        if ((pMedic->datedeb <= sDateJour) &&
            ((pMedic->datefin > sDateJour) || (string("") == pMedic->datefin)))
        {
          pMedicActuel->push_back(new NSMedic(*pMedic)) ;
          nbMedicActuel++ ;
        }
        else
        {
          pMedicAncien->push_back(new NSMedic(*pMedic)) ;
          nbMedicAncien++ ;
        }
      }
    }
  }

  //
  // Objectifs - Goals
  //

  if (pFrames)
  {
    for (int i = 0 ; i < FRAMECOUNT ; i++)
    {
      LDVFRAME iFrame = getFrameForIndex(i) ;
      NSFrameInformation *pFrameInfo = pFrames->getFrameInformation(iFrame) ;
      if (pFrameInfo)
      {
        ArrayGoals *pGoals = pFrameInfo->getGoalsArray() ;
        if (pGoals && (false == pGoals->empty()))
        {
          for (ArrayGoalIter j = pGoals->begin() ; j != pGoals->end() ; j++)
          {
            NSObjectif Obj ;

            if (InitObjectif(&Obj, *j))
            {
              pExams->push_back(new NSObjectif(Obj)) ;
              nbExams++ ;
            }
          }
        }
      }
    }
  }

  AfficheListeActuel() ;
  AfficheListeAncien() ;
  AfficheListeExams() ;
}
catch (...)
{
  erreur("Exception NSConcernPropertyDlg::InitListes", standardError, 0) ;
}
}

bool
NSConcernPropertyDlg::InitMedicament(NSMedic* pMedic, string sNoeud)
{
  if (NULL == pMedic)
    return false;

  NSPatientChoisi* pPat = pContexte->getPatient() ;
  if (NULL == pPat)
    return false ;

	if ((NULL == pPat->getDocHis()) || (pPat->getDocHis()->getVectDocument()->empty()))
		return false ;  if (strlen(sNoeud.c_str()) != PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + PPD_NOEUD_LEN)    return false ;	string sDocument = string(sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;  NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;  //  // Recherche du document auquel appartient le noeud  //	DocumentIter DocIt = pPat->getDocHis()->getVectDocument()->begin();	for ( ; pPat->getDocHis()->getVectDocument()->end() != DocIt ; DocIt++)	{    NSPatPathoInfo* pRootPathoInfo = (*DocIt)->getRootPathoInfo() ;		if (pRootPathoInfo)		{      if (pRootPathoInfo->getDoc() == sDocument)      {        (*DocIt)->initFromPatPatho(&DocPatho) ;        break ;      }    }  }  if (DocPatho.empty())    return false ;  //  // Recherche du noeud  //  PatPathoIter pptIt = DocPatho.begin() ;  for ( ; (DocPatho.end() != pptIt) && ((*pptIt)->getNode() != sNoeud) ; pptIt++) ;    if (DocPatho.end() == pptIt)      return false ;  pMedic->node    = sNoeud ;  string sLexique = (*pptIt)->getLexique() ;  pMedic->code    = sLexique;  string sLang = string("") ;  if (pContexte)
    sLang = pContexte->getUserLanguage() ;  pContexte->getDico()->donneLibelle(sLang, &sLexique, &(pMedic->name)) ;  // Paramètres du médicament  //  int iColBase = (*pptIt)->getColonne() ;  pptIt++ ;  while ((DocPatho.end() != pptIt) && ((*pptIt)->getColonne() > iColBase))  {
    string sElemLex = (*pptIt)->getLexique() ;
    string sSens;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    if ((*pptIt)->getColonne() == iColBase+1)
    {
      // Dates
      if ((sSens == "KOUVR") || (sSens == "KFERM"))
      {
        pptIt++;
        int iLigneBase = (*pptIt)->getLigne() ;
        string sUnite  = "";
        string sFormat = "";
        string sValeur = "";
        string sTemp   = "";

        while ((pptIt != DocPatho.end()) &&
                    ((*pptIt)->getLigne() == iLigneBase))
        {
          string sLexique = (*pptIt)->getLexique() ;
          if ((string("") != sLexique) && ('£' == sLexique[0]))
          {
            sTemp   = (*pptIt)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
            sValeur = (*pptIt)->getComplement() ;
            sTemp   = (*pptIt)->getUnit() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
            break ;
          }
          pptIt++ ;
        }

        // sFormat est du type £D0;03
        if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (sValeur != ""))
        {
          if ((sUnite == "2DA01") || (sUnite == "2DA02"))
          {
            if      (sSens == "KOUVR")
              pMedic->datedeb = sValeur ;
            else if (sSens == "KFERM")
              pMedic->datefin = sValeur ;
          }
        }
      }
      else
        pptIt++ ;
    }
    else
      pptIt++ ;
  }  return true ;}boolNSConcernPropertyDlg::InitObjectif(NSObjectif* pObj, NSLdvGoal* pGoal){  if ((NULL == pObj) || (NULL == pGoal))    return false ;  // Si on cherche les objectifs d'une préoccupation précise, on vérifie  // que cet objectif est concerné  //  if ((pConcern) && (pConcern->getNoeud() != pGoal->getConcern()))    return false ;  // cas d'un objectif futur non prévisible  if (pGoal->_tOuvertLe.estVide())    return false ;  NVLdVTemps tToday;  tToday.takeTime() ;  // cas d'un objectif déjà fermé  if ((false == pGoal->_tFermeLe.estVide()) && (pGoal->_tFermeLe <= tToday))    return false ;  bool bIsOpened = (tToday >= pGoal->_tOuvertLe) ;  pObj->_sName   = pGoal->getTitle() ;  pObj->_sNode   = pGoal->getNode() ;  // le titre dans Goal est en clair (pas de code lexique)  pObj->_sCode   = string("") ;  pObj->_iRythme = pGoal->_iRythme ;  // Note : un objectif cyclique non ouvert (futur) se comporte comme  // un objectif ponctuel  if ((pGoal->_iRythme != NSLdvGoal::cyclic) || (false == bIsOpened))  {    InitDateObjectifPonctuel(pObj->_sDateAuto, pGoal, pGoal->_sDateDebutAutorise,                                    pGoal->_dDelaiDebutAutorise, pGoal->_sUniteDebutAutorise);    InitDateObjectifPonctuel(pObj->_sDateCons, pGoal, pGoal->_sDateDebutConseille,                                    pGoal->_dDelaiDebutConseille, pGoal->_sUniteDebutConseille);    InitDateObjectifPonctuel(pObj->_sDateIdeal, pGoal, pGoal->_sDateDebutIdeal,                                    pGoal->_dDelaiDebutIdeal, pGoal->_sUniteDebutIdeal);    InitDateObjectifPonctuel(pObj->_sDateIdealMax, pGoal, pGoal->_sDateDebutIdealMax,                                    pGoal->_dDelaiDebutIdealMax, pGoal->_sUniteDebutIdealMax);    InitDateObjectifPonctuel(pObj->_sDateConsMax, pGoal, pGoal->_sDateDebutConseilMax,                                    pGoal->_dDelaiDebutConseilMax, pGoal->_sUniteDebutConseilMax);    InitDateObjectifPonctuel(pObj->_sDateCrit, pGoal, pGoal->_sDateDebutCritique,                                    pGoal->_dDelaiDebutCritique, pGoal->_sUniteDebutCritique);  }  else if (false == pGoal->getJalons()->empty())  {    // cas des objectifs cycliques déjà ouverts    // et non déjà fermés    GoalInfoIter i = pGoal->getJalons()->end() ;    do    {      i-- ;      if (NSLdvGoalInfo::jalonIntermediaire == (*i)->iTypeJalonEvent)      {        if ((*i)->iLevel == NSLdvGoalInfo::AProuge)          pObj->_sDateCrit = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iLevel == NSLdvGoalInfo::APjaune)          pObj->_sDateConsMax = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iLevel == NSLdvGoalInfo::APvert)          pObj->_sDateIdealMax = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iLevel == NSLdvGoalInfo::Bleu)          pObj->_sDateIdeal = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iLevel == NSLdvGoalInfo::AVvert)          pObj->_sDateCons = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iLevel == NSLdvGoalInfo::AVjaune)          pObj->_sDateAuto = (*i)->tpsInfo.donneDate() ;      }      else if (NSLdvGoalInfo::jalonCycle == (*i)->iTypeJalonEvent)      {        pObj->_sDatePrec = (*i)->tpsInfo.donneDate() ;        break ;      }      else if (NSLdvGoalInfo::jalonOuverture == (*i)->iTypeJalonEvent)        break ;    }    while (pGoal->getJalons()->begin() != i) ;  }  return true ;}
void
NSConcernPropertyDlg::InitDateObjectifPonctuel(string& sDateObj, NSLdvGoal* pGoal, string sDateJalon,
                                                    double dDelaiJalon, string sUniteJalon)
{
	NVLdVTemps tpsBuff;
	sDateObj = "";

	if (sDateJalon != "")
		sDateObj = sDateJalon ;	else if (dDelaiJalon > 0)	{		if (string("") == pGoal->_sOpenEventNode)		{			tpsBuff.initFromDate(pGoal->_tDateOuverture.donneDate()) ;			tpsBuff.ajouteTemps(int(dDelaiJalon), sUniteJalon, pContexte) ;			sDateObj = tpsBuff.donneDate() ;		}		else if (false == pGoal->_tOuvertLe.estVide())		{			tpsBuff.initFromDate(pGoal->_tOuvertLe.donneDate()) ;			tpsBuff.ajouteTemps(int(dDelaiJalon), sUniteJalon, pContexte) ;			sDateObj = tpsBuff.donneDate() ;		}	}}
void
NSConcernPropertyDlg::InitListeActuel()
{
    TListWindColumn colMedic("Medicament", 150, TListWindColumn::Left, 0);
  	pListeActuel->InsertColumn(0, colMedic);

    TListWindColumn colDateDeb("Date debut", 80, TListWindColumn::Left, 1);    pListeActuel->InsertColumn(1, colDateDeb);

    TListWindColumn colDateFin("Date fin", 80, TListWindColumn::Left, 2);
    pListeActuel->InsertColumn(2, colDateFin);
}

void
NSConcernPropertyDlg::AfficheListeActuel()
{
  pListeActuel->DeleteAllItems() ;

	for (int i = nbMedicActuel - 1 ; i >= 0 ; i--)  {
    TListWindItem Item((((*pMedicActuel)[i])->name).c_str(), 0) ;
    pListeActuel->InsertItem(Item) ;
  }
}

void
NSConcernPropertyDlg::DispInfoListeActuel(TLwDispInfoNotify& dispInfo)
{
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;

  string sDate ;

	int index = dispInfoItem.GetIndex() ;
  string sLang = pContexte->getUserLanguage() ;

	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // date début

      sDate = donne_date(((*pMedicActuel)[index])->datedeb, sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;

    case 2: // date fin
    	sDate = ((*pMedicActuel)[index])->datefin ;
      if (string("") == sDate)
      	sDate = string("indeterminée") ;
      else
      	sDate = donne_date(sDate, sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;
   } // fin du switch
}

void
NSConcernPropertyDlg::InitListeAncien()
{
    TListWindColumn colMedic("Medicament", 150, TListWindColumn::Left, 0);
  	pListeAncien->InsertColumn(0, colMedic);

    TListWindColumn colDateDeb("Date debut", 80, TListWindColumn::Left, 1);    pListeAncien->InsertColumn(1, colDateDeb);

    TListWindColumn colDateFin("Date fin", 80, TListWindColumn::Left, 2);
    pListeAncien->InsertColumn(2, colDateFin);
}

void
NSConcernPropertyDlg::AfficheListeAncien()
{
  pListeAncien->DeleteAllItems() ;

	for (int i = nbMedicAncien - 1 ; i >= 0 ; i--)  {
    TListWindItem Item((((*pMedicAncien)[i])->name).c_str(), 0) ;
    pListeAncien->InsertItem(Item) ;
  }
}

void
NSConcernPropertyDlg::DispInfoListeAncien(TLwDispInfoNotify& dispInfo)
{
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;

  string sDate ;

  int index = dispInfoItem.GetIndex();
	string sLang = pContexte->getUserLanguage() ;

	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // date début

      sDate = donne_date(((*pMedicAncien)[index])->datedeb, sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;

    case 2: // date fin
    	sDate = ((*pMedicAncien)[index])->datefin ;
      if (string("") == sDate)
      	sDate = string("indeterminée") ;
      else
      	sDate = donne_date(sDate, sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
    	break ;
	} // fin du switch
}

void
NSConcernPropertyDlg::InitListeExams()
{
    TListWindColumn colExam("Examen", 150, TListWindColumn::Left, 0);
  	pListeExams->InsertColumn(0, colExam);

    TListWindColumn colType("Type", 55, TListWindColumn::Left, 1);
  	pListeExams->InsertColumn(1, colType);

    TListWindColumn colDateDebAuto("Autorisé", 75, TListWindColumn::Left, 2);    pListeExams->InsertColumn(2, colDateDebAuto);

    TListWindColumn colDateDebCons("Conseillé", 75, TListWindColumn::Left, 3);
    pListeExams->InsertColumn(3, colDateDebCons);

    TListWindColumn colDateDebIdeal("Idéal", 75, TListWindColumn::Left, 4);
    pListeExams->InsertColumn(4, colDateDebIdeal);

    TListWindColumn colDateDebIdealMax("Idéal max", 75, TListWindColumn::Left, 5);
    pListeExams->InsertColumn(5, colDateDebIdealMax);

    TListWindColumn colDateDebConsMax("Cons. max", 75, TListWindColumn::Left, 6);
    pListeExams->InsertColumn(6, colDateDebConsMax);

    TListWindColumn colDateDebCrit("Critique", 75, TListWindColumn::Left, 7);
    pListeExams->InsertColumn(7, colDateDebCrit);

    TListWindColumn colDatePrec("Précédent", 75, TListWindColumn::Left, 8);
    pListeExams->InsertColumn(8, colDatePrec);
}

void
NSConcernPropertyDlg::AfficheListeExams()
{
  pListeExams->DeleteAllItems() ;

	for (int i = nbExams - 1; i >= 0; i--)  {
    TListWindItem Item((((*pExams)[i])->getName()).c_str(), 0);
    pListeExams->InsertItem(Item);
  }
}

void
NSConcernPropertyDlg::DispInfoListeExams(TLwDispInfoNotify& dispInfo)
{
  TListWindItem& dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  const int 	BufLen = 255 ;
  static char buffer[BufLen] ;
  string      sDate ;

  strcpy(buffer, "") ;
	string sLang = pContexte->getUserLanguage() ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case  1 : // type objectif
              if       (((*pExams)[index])->_iRythme == NSLdvGoal::cyclic)
                strcpy(buffer, "cyclique") ;
              else if  (((*pExams)[index])->_iRythme == NSLdvGoal::permanent)
                strcpy(buffer, "permanent") ;
              else if  (((*pExams)[index])->_iRythme == NSLdvGoal::ponctuel)
                strcpy(buffer, "ponctuel") ;
              else
                strcpy(buffer, "") ;
              dispInfoItem.SetText(buffer) ;
              break ;

    case  2 : // date début autorisé
              sDate = ((*pExams)[index])->_sDateAuto ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    case  3 : // date début conseillé              sDate = ((*pExams)[index])->_sDateCons ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    case  4 : // date début idéal
              sDate = ((*pExams)[index])->_sDateIdeal ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    case  5 : // date début ideal max
              sDate = ((*pExams)[index])->_sDateIdealMax ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    case  6 : // date début conseillé max
              sDate = ((*pExams)[index])->_sDateConsMax ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    case  7 : // date début critique
              sDate = ((*pExams)[index])->_sDateCrit ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    case  8 : // date précédent
              sDate = ((*pExams)[index])->_sDatePrec ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;
   } // fin du switch
}

void
NSConcernPropertyDlg::EvDrawItem(uint ctrlId, DRAWITEMSTRUCT far& drawInfo)
{
	// const int 	    BufLen = 255;
	// static char     buffer[BufLen+1];
	//
	// Liste des objectifs
	//
	if (ctrlId == IDC_LISTVIEW_GOALS)
	{
  	if (drawInfo.itemID == -1)
    	return ;

    // On retrouve l'élément qui correspond à cet index
    // Finding this index correponding element
    //
    /* int index = */ drawInfo.itemID ;
    ListView_SetTextBkColor(drawInfo.hwndItem, RGB(8,0,0));


        /*************************************
        TListWindItem Item ;
        Item.SetIndex(index) ;
        // Item.SetSubItem(0) ;
        Item.mask = LVIF_PARAM ;

        if (!(pListeExams->GetItem(Item)))            return ;

        Item.GetText(buffer);
        TDC dcItem(drawInfo.hDC);

        switch (index)
        {
            case 0:
                dcItem.SetBkColor(TColor::LtRed);
                break;

            case 1: // type objectif
                dcItem.SetBkColor(TColor::LtGreen);
                break;
        } // fin du switch

        // dcItem.TextOut(drawInfo.rcItem.left, drawInfo.rcItem.top, buffer);
        *****************************/
    }
}

void
NSConcernPropertyDlg::CmNouveau()
{
    string sChoixMed ;

    if (pConcern == 0)
    {
        erreur("Le médicament doit être rattaché à une préoccupation particulière.",  standardError, 0) ;
        return;
    }

	NSNewTrt* pNewTrtDlg = new NSNewTrt(this, &sChoixMed, pConcern->getNoeud(), pContexte, "GPRSC1") ;
  int iRetour = pNewTrtDlg->Execute() ;
  delete pNewTrtDlg ;

	if (iRetour == IDCANCEL)
		return ;

  if (sChoixMed == "")
    return ;
}

void
NSConcernPropertyDlg::CmRenouveler()
{
}

void
NSConcernPropertyDlg::CmModifier()
{
try
{
	int index = pListeActuel->IndexItemSelect() ;

	if (pConcern == 0)
  {
  	erreur("Le médicament doit être rattaché à une préoccupation particulière.",  standardError, 0) ;
    return ;
  }

	if (index == -1)
	{
  	erreur("Vous devez sélectionner un médicament à modifier dans la liste des médicaments en cours.",  standardError, 0) ;
    return ;
  }

	string sNodeModif, sCodeModif ;
	NSPatPathoArray PPT(pContexte->getSuperviseur()) ;

	sNodeModif = ((*pMedicActuel)[index])->node ;
	sCodeModif = ((*pMedicActuel)[index])->code ;

	// on récupère d'abord la sous-patpatho du noeud à modifier
	NSPatPathoArray SousPPT(pContexte->getSuperviseur()) ;
	pContexte->getPatient()->DonneSousArray(sNodeModif, &SousPPT) ;
	// on reconstitue la patpatho à partir du noeud
	PPT.ajoutePatho(sCodeModif, 0) ;
	// Insertion en queue (iter doit être ici egal à end())
	PPT.InserePatPatho(PPT.end(), &SousPPT, 1) ;
	// on crée une copie
	NSPatPathoArray PPTCopy(PPT) ;

	NSSmallBrother BigBoss(pContexte, &PPT, 0) ;
	BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
	// on lance l'archetype lié au code médicament (avec fil guide associé)
	BigBoss.lance12("GPRSC1", sCodeModif) ;
	// Si aucune modif : on sort...	if (PPT.estEgal(&PPTCopy))  	return ;	// récupération de la date du jour au format AAAAMMJJ	// char szDateJour[10];
	// donne_date_duJour(szDateJour);	// NVLdVTemps tpsNow ;	// tpsNow.takeTime() ;	VecteurString NodeArret ;	NodeArret.AddString(sNodeModif) ;	// on cloture la précédente prescription	// pContexte->getPatient()->ArreterElement(&NodeArret, tpsNow.donneDateHeure()) ;  //  NSToDoTask* pTask = new NSToDoTask ;  pTask->setWhatToDo(string("StopElementsFromNodes")) ;
  pTask->setPointer1(new VecteurString(NodeArret), true) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;	// on enregistre ici dans l'index de santé	if (false == PPT.empty())
  {
  	VecteurString NodeConcern ;
    NodeConcern.AddString(pConcern->getNoeud()) ;

		// pContexte->getPatient()->CreeTraitement(&PPT, &NodeConcern) ;
    //
    NSToDoTask* pTask1 = new NSToDoTask ;
    pTask1->setWhatToDo(string("CreateMedication")) ;
    pTask1->setPointer1(new NSPatPathoArray(PPT), true) ;
    pTask1->setPointer2(new VecteurString(NodeConcern), true) ;
    pTask1->sendMe(pContexte->getSuperviseur()) ;
  }

	InitListes() ;
}
catch (...)
{
	erreur("Exception NSConcernPropertyDlg::CmModifier.",  standardError, 0) ;
}}

void
NSConcernPropertyDlg::CmArreter()
{
  int index = pListeActuel->IndexItemSelect() ;

  if (-1 == index)
  {
    erreur("Vous devez sélectionner un médicament à arreter dans la liste des médicaments en cours.",  standardError, 0) ;
    return ;
  }

  string sNodeModif = ((*pMedicActuel)[index])->node ;

  // récupération de la date du jour au format AAAAMMJJ
  // char szDateJour[10];
  // donne_date_duJour(szDateJour);  NVLdVTemps tpsNow ;  tpsNow.takeTime() ;  VecteurString NodeArret ;  NodeArret.AddString(sNodeModif) ;  // on cloture la précédente prescription  // pContexte->getPatient()->ArreterElement(pNodeArret, string(szDateJour));

  // pContexte->getPatient()->ArreterElement(&NodeArret, tpsNow.donneDateHeure()) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("StopElementsFromNodes")) ;
  pTask->setPointer1(new VecteurString(NodeArret), true) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;

  InitListes() ;
}

void
NSConcernPropertyDlg::CmOrdonnance()
{
  // pContexte->getPatient()->CreeOrdonnance(true) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("CreatePrescription")) ;
  pTask->setParam1(string("true")) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSConcernPropertyDlg::CmGlobal()
{
    if (bGlobal)
    {
        pGlobal->Check();
        pLocal->Uncheck();
        return;
    }

    bGlobal = true;
    pOldConcern = pConcern;
    pConcern = 0;
    pGlobal->Check();
    pLocal->Uncheck();
    InitListes();
}

void
NSConcernPropertyDlg::CmLocal()
{
    if (!bGlobal)
    {
        pLocal->Check();
        pGlobal->Uncheck();
        return;
    }

    if ((pConcern == 0) && (pOldConcern != 0))
    {
        bGlobal = false;
        pConcern = pOldConcern;
        pOldConcern = 0;
        pLocal->Check();
        pGlobal->Uncheck();
        InitListes();
    }
    else if ((pConcern == 0) && (pOldConcern == 0))
    {
        erreur("Cette option n'est utilisable qu'à partir d'une préoccupation particulière.",  standardError, 0) ;
        bGlobal = true;
        pLocal->Uncheck();
        pGlobal->Check();
    }
}

void
NSConcernPropertyDlg::CmOk()
{
  NSUtilDialog::CmOk();
}

void
NSConcernPropertyDlg::CmCancel()
{
    NSUtilDialog::CmCancel();
}

// -----------------------------------------------------------------------------
//
//                         CHOIX D'UN NOUVEAU TRAITEMENT
//                                classe NSNewTrt
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSNewTrt, TDialog)  EV_COMMAND(IDOK,      CmOk),
  EV_COMMAND(IDCANCEL,  CmCancel),
END_RESPONSE_TABLE ;

// NSNewTrt::NSNewTrt(NSConcernPropertyDlg* parent, string* pTypeDocument, string sPreoccup, NSContexte* pCtx)//         :TDialog((TWindow *) parent, "TYPE_MEDIC"), NSRoot(pCtx)NSNewTrt::NSNewTrt(TWindow *parent, string *pTypeDocument, string sPreoccup, NSContexte *pCtx, string sArcCode, string sCaptionDlg)         :NSUtilDialog(parent, pCtx, "TYPE_MEDIC")
{
//    pDlg          = parent ;
    pType           = new NSUtilLexique(pContexte, this, IDC_MEDIC_EDIT, pContexte->getDico()) ;
    pTypeDocum      = pTypeDocument ;
    sPreoccupation  = sPreoccup ;
    sDlgCaption     = sCaptionDlg ;
    sCodeArc        = sArcCode ;
}


NSNewTrt::~NSNewTrt(){
  delete pType ;
}


void
NSNewTrt::SetupWindow()
{
    TDialog::SetupWindow() ;
    if (sDlgCaption != "")
        SetCaption(sDlgCaption.c_str()) ;
    if (pTypeDocum && (*pTypeDocum != ""))
        pType->setLabel(*pTypeDocum) ;
}


//----------------------------------------------------------------------------//----------------------------------------------------------------------------
void
NSNewTrt::CmOk()
{
try
{
  // on récupère d'abord un éventuel élément lexique sélectionné par les flèches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (pType->getDico()->getDicoDialog()->EstOuvert())
  {
    pType->getDico()->getDicoDialog()->InsererElementLexique() ;
    return ;
  }

  // Ne pas accepter les textes libres  if (pType->getCode() == string("£?????"))
  {
    erreur("Il faut choisir un code lexique et non pas du texte libre ", standardError, 0, GetHandle()) ;
    pType->SetFocus() ;
    return ;
  }

  *pTypeDocum = pType->getCode() ;

/*
  NSPatPathoArray *pPPT     = new NSPatPathoArray(pContexte) ;
  NSSmallBrother  *pBigBoss = new NSSmallBrother(pContexte, pPPT, 0, false /- initFromBbk -/) ;
  pBigBoss->pFenetreMere    = pContexte->GetMainWindow() ;

  if (sCodeArc != "")
    pBigBoss->lance12(sCodeArc, pType->sCode) ;
  else
  {
    // normalement on devrait jamais être dans ce cas

    // on lance l'archetype lié au code médicament (avec fil guide associé)
    pBigBoss->lance12("GPRSC1", pType->sCode) ;
  }
  //pBigBoss->lanceBbkArchetype(sArchetype) ;  delete pBigBoss ;
  // on enregistre ici dans l'index de santé
  if (!(pPPT->empty()))
  {
    pContexte->getPatient()->CreeTraitement(pPPT, sPreoccupation) ;
    // pDlg->InitListes();
  }

  delete pPPT ;
*/
	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSNewTrt::CmOk.",  standardError, 0) ;
  return ;
}
}

//-------------------------------------------------------------------------//-------------------------------------------------------------------------
void
NSNewTrt::CmCancel()
{
    *pTypeDocum = "";
    Destroy(IDCANCEL);
}

// -----------------------------------------------------------------
//
//  Méthodes de NSOpenMultiDocsDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSOpenMultiDocsDlg, NSUtilDialog)
    EV_LV_DISPINFO_NOTIFY(IDC_LISTVIEW_DOCS, LVN_GETDISPINFO, DispInfoListeDocs),
    EV_COMMAND(IDOK,            CmOk),
    EV_COMMAND(IDCANCEL,        CmCancel),
    EV_COMMAND(IDC_SELECT_ALL,  CmSelectAll),
END_RESPONSE_TABLE;

NSOpenMultiDocsDlg::NSOpenMultiDocsDlg(NSLdvViewBase* pView, NSContexte* pCtx, ArrayObjets* pObjects)
                   :NSUtilDialog((TWindow*) pView, pCtx, "OPEN_MULTIDOCS")
{
try
{
    pObjectsArray   = pObjects ;

    pSelectAllBtn   = new TButton(this, IDC_SELECT_ALL) ;
    pObjectsList    = new NSMultiDocsWindow(this, IDC_LISTVIEW_DOCS) ;
}
catch (...)
{
  erreur("Exception NSOpenMultiDocsDlg ctor.",  standardError, 0) ;
}
}

NSOpenMultiDocsDlg::~NSOpenMultiDocsDlg()
{
}

void
NSOpenMultiDocsDlg::SetupWindow()
{
    // ListView_SetExtendedListViewStyle(pObjectsList->HWindow, LVS_EX_FULLROWSELECT) ;

    TDialog::SetupWindow();

    InitListeDocs() ;
    AfficheListeDocs() ;
}

void
NSOpenMultiDocsDlg::dblClickDoc()
{
    CmOk() ;
}

void
NSOpenMultiDocsDlg::CmOk()
{
	if (pObjectsArray->empty())
		return ;

	int* pObjectsIndex = new int[pObjectsArray->size()] ;
	ArrayObjets aObjectsSelArray(pObjectsArray->pDoc) ;

	int nbSel = pObjectsList->GetSelIndexes(pObjectsIndex, pObjectsArray->size()) ;
	// max = pObjectsArray->size() - 1 ;

	for (int i = 0; i < nbSel; i++)
	{
		int index = pObjectsIndex[i] ;
		// pObjectsSelArray->push_back(new NSLdvObjet(*((*pObjectsArray)[max - index])));
		aObjectsSelArray.push_back(new NSLdvObjet(*((*pObjectsArray)[index]))) ;
	}

  delete[] pObjectsIndex ;

	// Attention à ne pas utiliser vider() pour ne pas deleter les objets
	for (ArrayObjIter objIt = pObjectsArray->begin() ; pObjectsArray->end() != objIt ; )
		pObjectsArray->erase(objIt) ;

	// l'operateur = appelle vider() mais comme pObjectsArray est vide
	// il ne fait que la recopie
	*pObjectsArray = aObjectsSelArray ;

	for (ArrayObjIter objIt = aObjectsSelArray.begin() ; aObjectsSelArray.end() != objIt ; )
		aObjectsSelArray.erase(objIt) ;

	TDialog::CmOk() ;
}

void
NSOpenMultiDocsDlg::CmCancel()
{
  TDialog::CmCancel() ;
}

void
NSOpenMultiDocsDlg::CmSelectAll()
{
	if (pObjectsArray->empty())
		return ;

	int* pObjectsIndex = new int[pObjectsArray->size()] ;
	int k = 0 ;

	for (ArrayObjIter objIt = pObjectsArray->begin() ; pObjectsArray->end() != objIt ; objIt++)
	{
		pObjectsIndex[k] = k ;
		k++ ;
	}

	pObjectsList->SetSelIndexes(pObjectsIndex, k, true) ;
	pObjectsList->SetFocus() ;
  delete[] pObjectsIndex ;
}

void
NSOpenMultiDocsDlg::InitListeDocs()
{
  TListWindColumn colExam("Examen", 300, TListWindColumn::Left, 0) ;
  pObjectsList->InsertColumn(0, colExam) ;
	TListWindColumn colNombre("Date", 100, TListWindColumn::Left, 1) ;
  pObjectsList->InsertColumn(1, colNombre) ;
}

void
NSOpenMultiDocsDlg::AfficheListeDocs()
{
	pObjectsList->DeleteAllItems() ;
  if (pObjectsArray->empty())
    return ;

  char cLibExam[255] = "" ;
  string sCodeExam, sLibExam ;

  ArrayObjIter objIt = pObjectsArray->end() ;
  do
  {
    objIt-- ;

    strcpy(cLibExam, (*objIt)->getTitre().c_str()) ;
    TListWindItem Item(cLibExam, 0) ;
    pObjectsList->InsertItem(Item) ;
  }
  while (pObjectsArray->begin() != objIt) ;
}

void
NSOpenMultiDocsDlg::DispInfoListeDocs(TLwDispInfoNotify& dispInfo)
{
	if (pObjectsArray->empty())
  	return ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

	// const int 	    BufLen = 255;
	// static char     buffer[BufLen];
	// char			cCodeExam[80];
	// int			    occur;

	ArrayObjIter objIt = pObjectsArray->begin() ;
	for (int i = 0 ; (i < index) && (objIt != pObjectsArray->end()); i++, objIt++) ;
    if (objIt == pObjectsArray->end())
        return ;

	string sLang = string("") ;
	if (pContexte)
		sLang = pContexte->getUserLanguage() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // nombre d'examens

    	string sDate = (*objIt)->getDateHeureDebut().donneDate() ;
      sDate = donne_date(sDate, sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;
	}

	return ;
}

//***************************************************************************
//
//  					Méthodes de NSDrugsPropertyWindow//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSMultiDocsWindow, TListWindow)   EV_WM_LBUTTONDBLCLK,
   // EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

NSMultiDocsWindow::NSMultiDocsWindow(NSOpenMultiDocsDlg* parent, int id, TModule* module)
    :TListWindow((TWindow *) parent, id, module)
{
    pView   = parent;
    iRes    = id;
    Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS;
    // Attr.ExStyle |= WS_EX_NOPARENTNOTIFY;
}
void
NSMultiDocsWindow::SetupWindow()
{
    ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

    TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSMultiDocsWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TLwHitTestInfo info(point) ;

    HitTest(info) ;
    if (info.GetFlags() & LVHT_ONITEM)        pView->CmOk() ;
}

//---------------------------------------------------------------------------//  Retourne l'index du premier item sélectionné
//---------------------------------------------------------------------------
int
NSMultiDocsWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0; i < count; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

void
NSMultiDocsWindow::EvSetFocus(HWND hWndLostFocus)
{
    pView->EvSetFocus(hWndLostFocus) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSNewCurveDlg
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSNewCurveDlg, NSUtilDialog)
	EV_COMMAND(IDOK,     CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND_AND_ID(COLORSAMPLE_RED,     ColorSample),
  EV_COMMAND_AND_ID(COLORSAMPLE_GREEN,   ColorSample),
  EV_COMMAND_AND_ID(COLORSAMPLE_YELLOW,  ColorSample),
  EV_COMMAND_AND_ID(COLORSAMPLE_BLUE,    ColorSample),
  EV_COMMAND_AND_ID(COLORSAMPLE_MAGENTA, ColorSample),
  EV_COMMAND_AND_ID(COLORSAMPLE_CYAN,    ColorSample),
  EV_COMMAND_AND_ID(TYPE_SOLID,          DotStyle),
  EV_COMMAND_AND_ID(TYPE_DASH,           DotStyle),
  EV_COMMAND_AND_ID(TYPE_DOT,            DotStyle),
  EV_COMMAND_AND_ID(TYPE_DASHDOT,        DotStyle),
  EV_COMMAND_AND_ID(TYPE_DASHDOTDOT,     DotStyle),
  EV_COMMAND_AND_ID(POINT_CIRCLE,        PointAspect),
  EV_COMMAND_AND_ID(POINT_SQUARE,        PointAspect),
  EV_COMMAND_AND_ID(POINT_TRIANGLE,      PointAspect),
  EV_COMMAND_AND_ID(POINT_STAR,          PointAspect),
  EV_COMMAND(INCLUDE_ZERO, ZeroIncluded),
END_RESPONSE_TABLE;

NSNewCurveDlg::NSNewCurveDlg(TWindow* pPere, NSContexte* pCtx, NSLdvCurve* pCurveToEdit, bool bModif)
              :NSUtilDialog(pPere, pCtx, "NEW_CURVE")
{
try
{
	pCurve         = pCurveToEdit ;

  bModifyMode    = bModif ;

  pConceptGroup  = new OWL::TGroupBox(this, SHADE_CURVE) ;

	pType          = new NSUtilLexique(pContexte, this, CURVE_EDIT, pContexte->getDico()) ;
  pUnitText      = new OWL::TStatic(this, UNIT_TEXT) ;
  pUnit          = new NSUtilLexique(pContexte, this, UNIT_EDIT, pContexte->getDico()) ;

  pAxisPropertiesGroup  = new OWL::TGroupBox(this, SHADE_AXIS) ;

  pMinAxisText   = new OWL::TStatic(this, LOWER_AXIS_TEXT) ;
  pMaxAxisText   = new OWL::TStatic(this, UPPER_AXIS_TEXT) ;

  pMinAxisValue  = new NSUpDownEdit(this, pContexte, "", LOWER_AXIS_EDIT, LOWER_AXIS_UPDN) ;
  pMaxAxisValue  = new NSUpDownEdit(this, pContexte, "", UPPER_AXIS_EDIT, UPPER_AXIS_UPDN) ;

  pIncludeZeroBtn = new OWL::TRadioButton(this, INCLUDE_ZERO, pAxisPropertiesGroup) ;

  pLineColourGroup = new OWL::TGroupBox(this, SHADE_TYPE) ;

  pRedText       = new OWL::TStatic(this, COLOR_RED_TEXT) ;
  pGreenText     = new OWL::TStatic(this, COLOR_GRE_TEXT) ;
  pBlueText      = new OWL::TStatic(this, COLOR_BLU_TEXT) ;

  pRedValue      = new NSUpDownEdit(this, pContexte, "", COLOR_RED_EDIT, COLOR_RED_UPDN) ;
  pGreenValue    = new NSUpDownEdit(this, pContexte, "", COLOR_GRE_EDIT, COLOR_GRE_UPDN) ;
  pBlueValue     = new NSUpDownEdit(this, pContexte, "", COLOR_BLU_EDIT, COLOR_BLU_UPDN) ;

  pRedSampleColorBtn     = new OWL::TRadioButton(this, COLORSAMPLE_RED, pLineColourGroup) ;
  pGreenSampleColorBtn   = new OWL::TRadioButton(this, COLORSAMPLE_GREEN, pLineColourGroup) ;
  pYellowSampleColorBtn  = new OWL::TRadioButton(this, COLORSAMPLE_YELLOW, pLineColourGroup) ;
  pBlueSampleColorBtn    = new OWL::TRadioButton(this, COLORSAMPLE_BLUE, pLineColourGroup) ;
  pMagentaSampleColorBtn = new OWL::TRadioButton(this, COLORSAMPLE_MAGENTA, pLineColourGroup) ;
  pCyanSampleColorBtn    = new OWL::TRadioButton(this, COLORSAMPLE_CYAN, pLineColourGroup) ;

  pLineTypeGroup = new OWL::TGroupBox(this, SHADE_LINE_TYPE) ;

  pWidthValue    = new NSUpDownEdit(this, pContexte, "", SIZE_EDIT, SIZE_UPDN) ;
  pWidthText     = new OWL::TStatic(this, SIZE_TEXT) ;

  pTSolid        = new OWL::TRadioButton(this, TYPE_SOLID, pLineTypeGroup) ;
  pTDash         = new OWL::TRadioButton(this, TYPE_DASH, pLineTypeGroup) ;
  pTDot          = new OWL::TRadioButton(this, TYPE_DOT, pLineTypeGroup) ;
  pTDashDot      = new OWL::TRadioButton(this, TYPE_DASHDOT, pLineTypeGroup) ;
  pTDashDotDot   = new OWL::TRadioButton(this, TYPE_DASHDOTDOT, pLineTypeGroup) ;

  pPointTypeGroup = new OWL::TGroupBox(this, SHADE_POINT) ;

  pDynamicColouringBtn = new OWL::TRadioButton(this, DYNAMIC_COLOURING, pLineTypeGroup) ;

  pPointAspectCircleBtn   = new OWL::TRadioButton(this, POINT_CIRCLE, pLineTypeGroup) ;
  pPointAspectSquareBtn   = new OWL::TRadioButton(this, POINT_SQUARE, pLineTypeGroup) ;
  pPointAspectTriangleBtn = new OWL::TRadioButton(this, POINT_TRIANGLE, pLineTypeGroup) ;
  pPointAspectStarBtn     = new OWL::TRadioButton(this, POINT_STAR, pLineTypeGroup) ;

  pPointRadiusText   = new OWL::TStatic(this, POINT_RADIUS_TEXT) ;
  pPointRadiusValue  = new NSUpDownEdit(this, pContexte, "", POINT_RADIUS_EDIT, POINT_RADIUS_UPDN) ;

  pSampleGroup   = new NSCurveSampleGroupBox(this, SHADE_SAMPLE) ;
}
catch (...)
{
	erreur("Exception NSNewCurveDlg ctor.",  standardError, 0) ;
}
}

NSNewCurveDlg::~NSNewCurveDlg()
{
	delete pConceptGroup ;

	delete pType ;
  delete pUnitText ;
  delete pUnit ;

  delete pAxisPropertiesGroup ;

  delete pMinAxisText ;
  delete pMaxAxisText ;

  delete pMinAxisValue ;
  delete pMaxAxisValue ;

  delete pIncludeZeroBtn ;

  delete pLineColourGroup ;

  delete pRedText ;
  delete pGreenText ;
  delete pBlueText ;

  delete pRedValue ;
  delete pGreenValue ;
  delete pBlueValue ;

  delete pRedSampleColorBtn ;
  delete pGreenSampleColorBtn ;
  delete pYellowSampleColorBtn ;
  delete pBlueSampleColorBtn ;
  delete pMagentaSampleColorBtn ;
  delete pCyanSampleColorBtn ;

  delete pLineTypeGroup ;

  delete pWidthValue ;
  delete pWidthText ;

  delete pTSolid ;
  delete pTDash ;
  delete pTDot ;
  delete pTDashDot ;
  delete pTDashDotDot ;

  delete pPointTypeGroup ;

  delete pDynamicColouringBtn ;

  delete pPointAspectCircleBtn ;
  delete pPointAspectSquareBtn ;
  delete pPointAspectTriangleBtn ;
  delete pPointAspectStarBtn ;

  delete pPointRadiusText ;
  delete pPointRadiusValue ;

  delete pSampleGroup ;
}

void
NSNewCurveDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  // ------- Information group
  //

  if (pCurve->getItemValue() != string(""))
		pType->setLabel(pCurve->getItemValue()) ;
  if (pCurve->getUnit() != string(""))
		pUnit->setLabel(pCurve->getUnit()) ;

  // ------- Axis group
  //

  pMinAxisValue->setValue(pCurve->getYAxis()->_dLowAxisValue) ;
  pMaxAxisValue->setValue(pCurve->getYAxis()->_dHighAxisValue) ;

  if (pCurve->getYAxis()->_bIncludeZero)
  	pIncludeZeroBtn->SetCheck(BF_CHECKED) ;

  // ------- Line color group
  //
	pRedValue->setMinMaxValue(0, 255) ;
  pRedValue->setValue(pCurve->_color.Red()) ;
  pRedValue->getEditNum()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::manualColorChange ));
  pRedValue->getUpDown()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::manualColorChange ));
  pGreenValue->setMinMaxValue(0, 255) ;
  pGreenValue->setValue(pCurve->_color.Green()) ;
  pGreenValue->getEditNum()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::manualColorChange ));
  pGreenValue->getUpDown()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::manualColorChange ));
  pBlueValue->setMinMaxValue(0, 255) ;
  pBlueValue->setValue(pCurve->_color.Blue()) ;
  pBlueValue->getEditNum()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::manualColorChange ));
  pBlueValue->getUpDown()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::manualColorChange ));

  // ------- Line style group
  //

  pWidthValue->setValue(pCurve->getDotWidth()) ;
  pWidthValue->getEditNum()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::drawExampleLine ));
  pWidthValue->getUpDown()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::drawExampleLine ));

  switch (pCurve->getDotStyle())
  {
  	case PS_SOLID :
  		pTSolid->SetCheck(BF_CHECKED) ;
      break ;
    case PS_DASH :
    	pTDash->SetCheck(BF_CHECKED) ;
      break ;
    case PS_DOT :
    	pTDot->SetCheck(BF_CHECKED) ;
      break ;
    case PS_DASHDOT :
    	pTDashDot->SetCheck(BF_CHECKED) ;
      break ;
    case PS_DASHDOTDOT :
    	pTDashDotDot->SetCheck(BF_CHECKED) ;
      break ;
    default :
    	pTSolid->SetCheck(BF_CHECKED) ;
  }

  // ------- Point style group
  //

  pPointRadiusValue->setValue(pCurve->getPointRadius()) ;
  pPointRadiusValue->getEditNum()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::drawExampleLine ));
  pPointRadiusValue->getUpDown()->SetLostFocusResponse(new MemFunctor<NSNewCurveDlg>( (NSNewCurveDlg*)this, &NSNewCurveDlg::drawExampleLine ));

  switch (pCurve->getPointAspect())
  {
  	case NSLdvCurve::paCircle :
  		pPointAspectCircleBtn->SetCheck(BF_CHECKED) ;
      break ;
    case NSLdvCurve::paSquare :
    	pPointAspectSquareBtn->SetCheck(BF_CHECKED) ;
      break ;
    case NSLdvCurve::paTriangle :
    	pPointAspectTriangleBtn->SetCheck(BF_CHECKED) ;
      break ;
    case NSLdvCurve::paStar :
    	pPointAspectStarBtn->SetCheck(BF_CHECKED) ;
      break ;
    default :
    	pPointAspectCircleBtn->SetCheck(BF_CHECKED) ;
  }

  drawExampleLine() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSNewCurveDlg::CmOk()
{
try
{
  if (false == bModifyMode)
  {
  	// on récupère d'abord un éventuel élément lexique sélectionné par les flêches
  	// Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  	if (pType->getDico()->getDicoDialog()->EstOuvert())
  	{
    	pType->getDico()->getDicoDialog()->InsererElementLexique() ;
    	return ;
  	}

  	// Ne pas accepter les textes libres
  	if (pType->getCode() == string("£?????"))
  	{
  		erreur("Il faut choisir un code lexique et non pas du texte libre ", standardError, 0, GetHandle()) ;
    	pType->SetFocus() ;
    	return ;
  	}

  	pCurve->setItemValue(pType->getCode()) ;
  }

  // idem for Unit
  //
  if (pUnit->getDico()->getDicoDialog()->EstOuvert())
  {
    pUnit->getDico()->getDicoDialog()->InsererElementLexique() ;
    return ;
  }

  // Ne pas accepter les textes libres
  if (pUnit->getCode() == string("£?????"))
  {
  	erreur("Il faut choisir un code lexique et non pas du texte libre ", standardError, 0, GetHandle()) ;
    pUnit->SetFocus() ;
    return ;
  }

  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

  pCurve->setUnit(pUnit->getCode()) ;

  // ------- Axis group
  //

  pCurve->getYAxis()->_dLowAxisValue  = pMinAxisValue->getValue() ;
  pCurve->getYAxis()->_dHighAxisValue = pMaxAxisValue->getValue() ;

  if (pIncludeZeroBtn->GetCheck() == BF_CHECKED)
		pCurve->getYAxis()->_bIncludeZero = true ;
  else
  	pCurve->getYAxis()->_bIncludeZero = false ;

  // ------- Line color group
  //
  int iRedColor = pRedValue->getValue() ;
  int iGreColor = pGreenValue->getValue() ;
  int iBluColor = pBlueValue->getValue() ;

  pCurve->_color = NS_CLASSLIB::TColor(iRedColor, iGreColor, iBluColor);

  // ------- Line style group
  //

  // Dot size
  //
  pCurve->_dotWidth = pWidthValue->getValue() ;

  // Dot type
  //
  pCurve->_dotStyle = getDotStyle() ;

  // ------- Point style group
  //

  if (pDynamicColouringBtn->GetCheck() == BF_CHECKED)
		pCurve->_bAdjustPointColorToValue = true ;
  else
  	pCurve->_bAdjustPointColorToValue = false ;

  pCurve->_pointRadius = pPointRadiusValue->getValue() ;

  pCurve->_pointAspect = getPointStyle() ;

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSNewCurveDlg::CmOk.", standardError, 0) ;
}
}

void
NSNewCurveDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSNewCurveDlg::ColorSample(WPARAM wParam)
{
	int iRed   = 0 ;
  int iGreen = 0 ;
  int iBlue  = 0 ;

	switch (wParam)
	{
  	case (COLORSAMPLE_RED) :
    	iRed   = 255 ;
      break ;
  	case (COLORSAMPLE_GREEN) :
    	iGreen = 255 ;
      break ;
    case (COLORSAMPLE_YELLOW) :
    	iRed   = 255 ;
    	iGreen = 255 ;
      break ;
    case (COLORSAMPLE_BLUE) :
    	iBlue  = 255 ;
      break ;
    case (COLORSAMPLE_MAGENTA) :
    	iRed   = 255 ;
    	iBlue  = 255 ;
      break ;
    case (COLORSAMPLE_CYAN) :
    	iGreen = 255 ;
    	iBlue  = 255 ;
      break ;
  }

  pRedValue->setValue(iRed) ;
  pGreenValue->setValue(iGreen) ;
  pBlueValue->setValue(iBlue) ;

  drawExampleLine() ;
}

void
NSNewCurveDlg::DotStyle(WPARAM wParam)
{
	switch (wParam)
	{
  	case (TYPE_DASH) :
  	case (TYPE_DOT) :
    case (TYPE_DASHDOT) :
    case (TYPE_DASHDOTDOT) :
    	pWidthValue->setValue(1) ;
      break ;
  }

  drawExampleLine() ;
}

void
NSNewCurveDlg::PointAspect(WPARAM wParam)
{
	drawExampleLine() ;
}

void
NSNewCurveDlg::ZeroIncluded()
{
	if (pIncludeZeroBtn->GetCheck() == BF_CHECKED)
    pIncludeZeroBtn->Uncheck() ;
  else
  {
    pIncludeZeroBtn->Check() ;
    pMinAxisValue->setValue(0) ;
  }
}

void
NSNewCurveDlg::drawExampleLine()
{
	// Get information
  //
	int iRedColor = pRedValue->getValue() ;
  int iGreColor = pGreenValue->getValue() ;
  int iBluColor = pBlueValue->getValue() ;

  int iDotStyle = getDotStyle() ;

  int iDotWidth = pWidthValue->getValue() ;

  int iPtRadius = pPointRadiusValue->getValue() ;
  int iPtStyle  = getPointStyle() ;

  // Draw sample
  //
	NS_CLASSLIB::TRect groupScreenRect  = pSampleGroup->GetWindowRect() ;
  NS_CLASSLIB::TRect dialogScreenRect = GetWindowRect() ;
  NS_CLASSLIB::TPoint upperLeftPoint(groupScreenRect.Left() - dialogScreenRect.Left(), groupScreenRect.Top() - dialogScreenRect.Top()) ;
  NS_CLASSLIB::TRect groupRect(upperLeftPoint.X(),
                               upperLeftPoint.Y(),
                               upperLeftPoint.X() + groupScreenRect.Width(),
                               upperLeftPoint.Y() + groupScreenRect.Height()) ;
  NS_CLASSLIB::TRect drawingRect(groupRect.Left() + 3, groupRect.Top() + 3, groupRect.Right() - 3, groupRect.Bottom() - 3) ;

  TWindowDC* pWinDC = new TWindowDC(this->HWindow) ; // HWnd

  // Draw a white background
  //
  pWinDC->FillRect(drawingRect, NS_CLASSLIB::TColor::White) ;

  // Prepare pen and brush
  //
  OWL::TPen curvePen(NS_CLASSLIB::TColor(iRedColor, iGreColor, iBluColor), iDotWidth, iDotStyle) ;
  ::SelectObject(pWinDC->GetHDC(), curvePen) ;

  OWL::TBrush curvePointsBrush(NS_CLASSLIB::TColor(iRedColor, iGreColor, iBluColor)) ;
  ::SelectObject(pWinDC->GetHDC(), curvePointsBrush) ;

  // Draw the line
  //
  NS_CLASSLIB::TPoint leftLinePoint(drawingRect.Left() + 3, groupRect.Top() + groupRect.Height() / 2) ;
  NS_CLASSLIB::TPoint rightLinePoint(drawingRect.Right() - 3, leftLinePoint.Y()) ;

  pWinDC->MoveTo(leftLinePoint) ;
  pWinDC->LineTo(rightLinePoint) ;

  // Draw some points
  //
  NSLdvViewArea* pActiveWorkingArea = pCurve->getCurvesPannel()->getView()->getActiveWorkingArea() ;
  if (NULL == pActiveWorkingArea)
    return ;

  NSHistoryValueManagement historyValue ;
  historyValue.setDate(string("20060723115410")) ;
  NSLdvCurvePoint curvePoint(pContexte, pActiveWorkingArea, pCurve, &historyValue) ;

  int iNbSamples = 3 ;
  int iLineLenght = rightLinePoint.X() - leftLinePoint.X() ;

  for (int i = 0 ; i < iNbSamples ; i++)
  {
  	NS_CLASSLIB::TPoint samplePoint(leftLinePoint.X() + (i + 1) * iLineLenght / (iNbSamples + 1), leftLinePoint.Y()) ;

  	NS_CLASSLIB::TRect pointDrawingBox(samplePoint.X() - iPtRadius,
                                       samplePoint.Y() - iPtRadius,
                                       samplePoint.X() + iPtRadius,
                                       samplePoint.Y() + iPtRadius) ;

  	switch(iPtStyle)
  	{
  		case NSLdvCurve::paCircle :
    		curvePoint.drawPointCircle(pWinDC, pointDrawingBox, &curvePointsBrush, &curvePen) ;
      	break ;
    	case NSLdvCurve::paSquare :
    		curvePoint.drawPointSquare(pWinDC, pointDrawingBox, &curvePointsBrush, &curvePen) ;
      	break ;
    	case NSLdvCurve::paTriangle :
    		curvePoint.drawPointUpTriangle(pWinDC, pointDrawingBox, &curvePointsBrush, &curvePen) ;
      	break ;
    	case NSLdvCurve::paStar :
    		curvePoint.drawPointStar(pWinDC, pointDrawingBox, &curvePointsBrush, &curvePen) ;
      	break ;
  	}
  }

	delete pWinDC ;
}

void
NSNewCurveDlg::manualColorChange()
{
  resetSampleColor() ;
  drawExampleLine() ;
}

void
NSNewCurveDlg::resetSampleColor()
{
  pRedSampleColorBtn->Uncheck() ;
  pGreenSampleColorBtn->Uncheck() ;
  pYellowSampleColorBtn->Uncheck() ;
  pBlueSampleColorBtn->Uncheck() ;
  pMagentaSampleColorBtn->Uncheck() ;
  pCyanSampleColorBtn->Uncheck() ;
}

int
NSNewCurveDlg::getDotStyle()
{
	if      (pTSolid->GetCheck()      == BF_CHECKED)
  	return PS_SOLID ;
  else if (pTDash->GetCheck()       == BF_CHECKED)
  	return PS_DASH ;
  else if (pTDot->GetCheck()        == BF_CHECKED)
  	return PS_DOT ;
  else if (pTDashDot->GetCheck()    == BF_CHECKED)
  	return PS_DASHDOT ;
  else if (pTDashDotDot->GetCheck() == BF_CHECKED)
  	return PS_DASHDOTDOT ;

  return PS_SOLID ;
}

int
NSNewCurveDlg::getPointStyle()
{
	if      (pPointAspectCircleBtn->GetCheck()   == BF_CHECKED)
  	return NSLdvCurve::paCircle ;
  else if (pPointAspectSquareBtn->GetCheck()   == BF_CHECKED)
  	return NSLdvCurve::paSquare ;
  else if (pPointAspectTriangleBtn->GetCheck() == BF_CHECKED)
  	return NSLdvCurve::paTriangle ;
  else if (pPointAspectStarBtn->GetCheck()     == BF_CHECKED)
  	return NSLdvCurve::paStar ;

  return NSLdvCurve::paCircle ;
}

void
NSNewCurveDlg::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  Paint(dc, erase, RectAPeindre) ;
  drawExampleLine() ;
}

//
// ------------ NSCurveSampleGroupBox ------------
//

DEFINE_RESPONSE_TABLE1(NSCurveSampleGroupBox, TGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE ;

NSCurveSampleGroupBox::NSCurveSampleGroupBox(NSNewCurveDlg* parent, int resourceId, TModule* module)
                      :TGroupBox(parent, resourceId, module)
{
  _pNewCurveDlg = parent ;
}

void
NSCurveSampleGroupBox::EvPaint()
{
  TGroupBox::EvPaint() ;
  _pNewCurveDlg->drawExampleLine() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSConvocDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSConvocDlg, NSUtilDialog)
	EV_COMMAND(IDOK,        CmOk),
	EV_COMMAND(IDCANCEL,    CmCancel),
END_RESPONSE_TABLE;

NSConvocDlg::NSConvocDlg(TWindow* pPere, NSContexte* pCtx, NVLdVTemps* pDeb)
            :NSUtilDialog(pPere, pCtx, "CONVOC_DLG")
{
try
{
	ptpsDeb     = pDeb ;
	pDateDeb    = new NSUtilEditDateHeure(pContexte, this, PROB_DATE_DEB) ;

	pNbJoursTxt = new OWL::TStatic(this, IDR_DRG_DANS) ;
  pNbJours    = new NSEditNum(pContexte, this, IDC_DRG_NBJOURS, 10) ;

  const char *temp1[] = { "2HEUR1", "2DAT01", "2DAT11", "2DAT21", "2DAT31" } ;

  int iArraySize   = sizeof(temp1) ;
  int iElementSize = sizeof(temp1[0]) ;
  int iElementsCount = iArraySize / iElementSize ;

  pCBDureeTtt    = new NSComboBox(this, IDC_DRG_NBJOURS_TXT, pContexte, temp1, 5) ;
  pCBDureeTtt->SetLostFocusResponse(new MemFunctor<NSConvocDlg>((NSConvocDlg*)this, &NSConvocDlg::ExecutedAfterDelayChoiceDate)) ;
}
catch (...)
{
	erreur("Exception NSConvocDlg ctor.", standardError, 0) ;
}
}

NSConvocDlg::~NSConvocDlg()
{
  delete pDateDeb ;
  delete pNbJoursTxt ;
  delete pNbJours ;
  delete pCBDureeTtt ;
}

void
NSConvocDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	string sDateDeb = ptpsDeb->donneDateHeure() ;
	pDateDeb->setDate(sDateDeb.c_str()) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSConvocDlg::CmOk()
{
try
{
  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

	string sDDeb ;
	pDateDeb->getDate(&sDDeb) ;
	ptpsDeb->initFromDateHeure(sDDeb) ;

	CloseWindow(IDOK) ;
}
catch (...)
{
	erreur("Exception NSConvocDlg::CmOk.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
NSConvocDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

void
NSConvocDlg::ExecutedAfterDelayChoiceDate()
{
	pNbJours->donneValeur() ;
	int iTrtDurationValue = (int) pNbJours->_dValeur ;
	if (iTrtDurationValue <= 0)
		return ;

	string sTrtDurationUnit = pCBDureeTtt->getSelCode() ;
	if (string("") == sTrtDurationUnit)
		return ;

	NVLdVTemps tDateFin ;
	tDateFin.takeTime() ;

	tDateFin.ajouteTemps(iTrtDurationValue, sTrtDurationUnit, pContexte) ;
	string sDateFin = tDateFin.donneDateHeure() ;

  pDateDeb->setDate(sDateFin) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSValueInfoDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSValueInfoDlg, NSUtilDialog)
	EV_COMMAND(IDOK,        CmOk),
	EV_COMMAND(IDCANCEL,    CmCancel),
END_RESPONSE_TABLE;

NSValueInfoDlg::NSValueInfoDlg(TWindow* pPere, NSContexte* pCtx, string *pPath, string *pContent)
               :NSUtilDialog(pPere, pCtx, "VALUE_INFO")
{
try
{
	_pPath    = new OWL::TStatic(this, IDR_DRG_DANS) ;
  _pContent = new OWL::TEdit(this, PREOCCUP_EDIT) ;

  _sPath    = *pPath ;
  _sContent = *pContent ;

  if (string("") != _sPath)
  {
    string sPathLib = string("") ;
    string sLang    = pContexte->getUserLanguage() ;
    pContexte->getDico()->donneLibelleChemin(sLang, _sPath, &sPathLib, string(1, cheminSeparationMARK)) ;
    _sPath = sPathLib ;
  }
}
catch (...)
{
	erreur("Exception NSValueInfoDlg ctor.", standardError, 0) ;
}
}

NSValueInfoDlg::~NSValueInfoDlg()
{
  delete _pPath ;
  delete _pContent ;
}

void
NSValueInfoDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  _pPath->SetText(_sPath.c_str()) ;
	_pContent->SetText(_sContent.c_str()) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSValueInfoDlg::CmOk()
{
try
{
  _sContent = string("") ;

  int iBuffLen = _pContent->GetTextLen() ;
  if (iBuffLen > 0)
  {
    char far* szBuff = new char[iBuffLen + 1] ;
    _pContent->GetText(szBuff, iBuffLen + 1) ;

    _sContent = string(szBuff) ;

    delete[] szBuff ;
  }

	CloseWindow(IDOK) ;
}
catch (...)
{
	erreur("Exception NSValueInfoDlg::CmOk.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
NSValueInfoDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

//
// ------------ NSDrugHistoryDlg ------------
//

DEFINE_RESPONSE_TABLE1(NSDrugHistoryDlg, NSUtilDialog)
  EV_COMMAND(IDOK,     CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSDrugHistoryDlg::NSDrugHistoryDlg(TWindow* pView, NSContexte *pCtx)
                 :NSUtilDialog((TWindow*) pView, pCtx, (pCtx->getBamType() == NSContexte::btNone) ? "DRUG_HISTORY_CTRL" : "DRUG_HISTORY_BAM", pCtx->GetMainWindow()->GetModule())
{
  _pDrugText = new OWL::TStatic(this, CONCEPT_TEXT) ;
  _pDrugEdit = new NSUtilLexique(pContexte, this, CONCEPT_EDIT, pContexte->getDico()) ;

  if (pContexte->getBamType() != NSContexte::btNone)
  {
    _pAtcText = new OWL::TStatic(this, ATC_TEXT) ;
    _pAtcCode = new NSEditBdm(pCtx, this, ATC_EDIT, NSBdmDriver::bamTableATC) ;
  }
  else
  {
    _pAtcText = (OWL::TStatic*) 0 ;
    _pAtcCode = (NSEditBdm*) 0 ;
  }

  _sLexiqCode  = string("") ;
  _sLexiqLabel = string("") ;
  _sAtcCode    = string("") ;
  _sAtcLabel   = string("") ;
}

NSDrugHistoryDlg::~NSDrugHistoryDlg()
{
  delete _pDrugText ;
  delete _pDrugEdit ;

  if (_pAtcText)
    delete _pAtcText ;
  if (_pAtcCode)
    delete _pAtcCode ;
}

void
NSDrugHistoryDlg::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sText = pSuper->getText("drugHistory", "concept") ;
  _pDrugText->SetCaption(sText.c_str()) ;
  sText = pSuper->getText("drugHistory", "AtcCode") ;

  if (_pAtcText)
    _pAtcText->SetCaption(sText.c_str()) ;
}

string
NSDrugHistoryDlg::getDrugCode()
{
  if ((NSUtilLexique*) NULL == _pDrugEdit)
    return string("") ;

  return _pDrugEdit->getCode() ;
}

string
NSDrugHistoryDlg::getDrugLabel()
{
  if ((NSUtilLexique*) NULL == _pDrugEdit)
    return string("") ;

  return _pDrugEdit->getLabel() ;
}

string
NSDrugHistoryDlg::getAtcCode()
{
  if ((NSEditBdm*) NULL == _pAtcCode)
    return string("") ;

  return _pAtcCode->getCode() ;
}

string
NSDrugHistoryDlg::getAtcLabel()
{
  if ((NSEditBdm*) NULL == _pAtcCode)
    return string("") ;

  return _pAtcCode->getLabel() ;
}

void
NSDrugHistoryDlg::CmOk()
{
  // An ATC code can be entered manually
  //
  if (_pAtcCode)
  {
    string sAtcCode = _pAtcCode->getCode() ;

    if (string("") == sAtcCode)
    {
      string sCode = _pAtcCode->getTextAsString() ;
      if (IsValidAtcCode(sCode))
        _pAtcCode->injectCode(sCode) ;
    }
  }

  NSUtilDialog::CmOk() ;
}

void
NSDrugHistoryDlg::CmCancel()
{
  NSUtilDialog::CmCancel() ;
}

// fin de nsldvvar.cpp
/////////////////////////////////////////////////////


