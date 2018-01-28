// -----------------------------------------------------------------------------
// nsmedicdlg.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/04 09:15:50 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#include <owl\window.h>
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbb.rh"
#include "partage\nsdivfct.h"
#include "medicament.rh"
#include "nautilus\nssuper.h"
#include "nsepisod\nsldvuti.h"

const std::string SECONDE 				= "2SEC01";        // code lexique pour l'unité de temps seconde
const std::string FOIS 						= "2FOIS1";        // code lexique pour le terme fois : ex 3 fois pendant 2 jours
/*const std::string SECONDE = "2MINU1";
const std::string SECONDE = "2HEUR1"; */
const std::string JOUR 						= "2DAT01";  			// code lexique pour heure
const std::string DATE_OUVERTURE 	= "KOUVR1"; 			// code date de commencement du médicament
const std::string DATE_FERMETURE 	= "KFERM1";       // ... la fin du médicament
const std::string PHASE						= "KPHAT1";       // ... représente une phase

/*const std::string TEMP    = "KMATI1";
const std::string TEMP    = "KREVE1";
const std::string TEMP 		= "KGOUT1";
const std::string TEMP    = "KMIDI1";
const std::string TEMP    = "KSOIR1";
const std::string TEMP    = "KCOUC1";
const std::string TEMP    = "KNOCT3";    */

void Avance(PatPathoIter& pptIter,PatPathoIter& pptEnd)
{
  if (pptIter != pptEnd)
		pptIter++ ;
}

#define PMG_TAB_POSO      1
#define PMG_GRP_POSO      2

#define PMG_GRP_MORNING   3
#define PMG_EDIT_MORNING  4
#define PMG_UPDN_MORNING  5

#define PMG_GRP_NOON      6
#define PMG_EDIT_NOON     7
#define PMG_UPDN_NOON     8

#define PMG_GRP_EVENING   9
#define PMG_EDIT_EVENING 10
#define PMG_UPDN_EVENING 11

#define PMG_GRP_BED      12
#define PMG_EDIT_BED     13
#define PMG_UPDN_BED     14

//Tableau de reponse des tab control
int periodeMMSC[]  = {PMG_GRP_MORNING,  PMG_GRP_NOON,  PMG_GRP_EVENING,  PMG_GRP_BED,
                      PMG_EDIT_MORNING, PMG_EDIT_NOON, PMG_EDIT_EVENING, PMG_EDIT_BED,
                      PMG_UPDN_MORNING, PMG_UPDN_NOON, PMG_UPDN_EVENING, PMG_UPDN_BED};

#define PMG_GRP_WAKEUP    15
#define PMG_EDIT_WAKEUP   16
#define PMG_UPDN_WAKEUP   17

#define PMG_GRP_MORNING2  18
#define PMG_EDIT_MORNING2 19
#define PMG_UPDN_MORNING2 20

#define PMG_GRP_NOON2     21
#define PMG_EDIT_NOON2    22
#define PMG_UPDN_NOON2    23

#define PMG_GRP_SNACK     24
#define PMG_EDIT_SNACK    25
#define PMG_UPDN_SNACK    26

#define PMG_GRP_EVENING2  27
#define PMG_EDIT_EVENING2 28
#define PMG_UPDN_EVENING2 29

#define PMG_GRP_BED2      30
#define PMG_EDIT_BED2     31
#define PMG_UPDN_BED2     32

#define PMG_GRP_NIGHT     33
#define PMG_EDIT_NIGHT    34
#define PMG_UPDN_NIGHT    35

int periodeEvents[] = {PMG_GRP_WAKEUP,  PMG_GRP_MORNING2,  PMG_GRP_NOON2,  PMG_GRP_SNACK,  PMG_GRP_EVENING2,  PMG_GRP_BED2,  PMG_GRP_NIGHT,
                      PMG_EDIT_WAKEUP, PMG_EDIT_MORNING2, PMG_EDIT_NOON2, PMG_EDIT_SNACK, PMG_EDIT_EVENING2, PMG_EDIT_BED2, PMG_EDIT_NIGHT,
                      PMG_UPDN_WAKEUP, PMG_UPDN_MORNING2, PMG_UPDN_NOON2, PMG_UPDN_SNACK, PMG_UPDN_EVENING2, PMG_UPDN_BED2, PMG_UPDN_NIGHT};

#define PMG_GRP_SINGLE    36
#define PMG_EDIT_SINGLE   37
#define PMG_UPDN_SINGLE   38

int PosoUniques[]   = {PMG_GRP_SINGLE,  PMG_EDIT_SINGLE,  PMG_UPDN_SINGLE};

#define PMG_GRP_HOUR_Q    39
#define PMG_EDIT_HOUR_Q   40
#define PMG_UPDN_HOUR_Q   41
#define PMG_GRP_HOUR_H    42
#define PMG_EDIT_HOUR_H   43
#define PMG_UPDN_HOUR_H   44
#define PMG_BTN_HOUR_A    45
#define PMG_LIST_HOUR_L   46

int hourCycles[]    = {PMG_GRP_HOUR_Q,  PMG_GRP_HOUR_H,
                      PMG_EDIT_HOUR_Q, PMG_EDIT_HOUR_H,
                      PMG_UPDN_HOUR_Q, PMG_UPDN_HOUR_H,
                      PMG_BTN_HOUR_A,  PMG_LIST_HOUR_L};

#define PMG_GRP_REGCYC_Q  47
#define PMG_EDIT_REGCYC_Q 48
#define PMG_UPDN_REGCYC_Q 49
#define PMG_GRP_REGCYC_F  50
#define PMG_EDIT_REGCYC_F 51
#define PMG_UPDN_REGCYC_F 52
#define PMG_STATIC_REGCYC 53
#define PMG_COMBO_REGCYC  54

int regularDays[]   = {PMG_GRP_REGCYC_Q,  PMG_GRP_REGCYC_F,
                      PMG_EDIT_REGCYC_Q, PMG_EDIT_REGCYC_F,
                      PMG_UPDN_REGCYC_Q, PMG_UPDN_REGCYC_F,
                      PMG_STATIC_REGCYC, PMG_COMBO_REGCYC};

#define PMG_GRP_FRECYC_Q  55
#define PMG_EDIT_FRECYC_Q 56
#define PMG_UPDN_FRECYC_Q 57
#define PMG_GRP_FRECYC_T  58
#define PMG_EDIT_FRECYC_T 59
#define PMG_UPDN_FRECYC_T 60
#define PMG_GRP_FRECYC_F  61
#define PMG_EDIT_FRECYC_F 62
#define PMG_UPDN_FRECYC_F 63
#define PMG_STATIC_FRECYC 64
#define PMG_COMBO_FRECYC  65

int idcPosoLibres[] = {PMG_GRP_FRECYC_Q,  PMG_GRP_FRECYC_T,  PMG_GRP_FRECYC_F,
                      PMG_EDIT_FRECYC_Q, PMG_EDIT_FRECYC_T, PMG_EDIT_FRECYC_F,
                      PMG_UPDN_FRECYC_Q, PMG_UPDN_FRECYC_T, PMG_UPDN_FRECYC_F,
                      PMG_STATIC_FRECYC, PMG_COMBO_FRECYC};

#define PMG_EDIT_FREETEXT 66

int TxtLibres[]     = {PMG_EDIT_FREETEXT};

#define PMG_TAB_CYCLES    67
#define PMG_GRP_CYCLES    68

#define PMG_COMBO_SIMPLE  69

int CyclSimples[]   = {PMG_COMBO_SIMPLE};

#define PMG_RADIO_MONDAY  70
#define PMG_RADIO_TUESDAY 71
#define PMG_RADIO_WEDNDAY 72
#define PMG_RADIO_THURDAY 73
#define PMG_RADIO_FRIDAY  74
#define PMG_RADIO_SATUDAY 75
#define PMG_RADIO_SUNDAY  76

int CycleJours[]    = {PMG_RADIO_MONDAY,  PMG_RADIO_TUESDAY, PMG_RADIO_WEDNDAY,
                      PMG_RADIO_THURDAY, PMG_RADIO_FRIDAY,  PMG_RADIO_SATUDAY,
                      PMG_RADIO_SUNDAY};

#define PMG_STATIC_D1D2D3 77
#define PMG_EDIT_D1D2D3   78
#define PMG_UPDN_D1D2D3   79
#define PMG_RADIO_D1D2_Y  80
#define PMG_RADIO_D1D2_N  81

int CycleJourAlts[] = {PMG_STATIC_D1D2D3, PMG_EDIT_D1D2D3, PMG_UPDN_D1D2D3,
                      PMG_RADIO_D1D2_Y,  PMG_RADIO_D1D2_N};

#define PMG_GRP_REGCUR_Q  82
#define PMG_EDIT_REGCUR_Q 83
#define PMG_UPDN_REGCUR_Q 84
#define PMG_COMBO_REGCURQ 85
#define PMG_STATIC_REGCUR 86
#define PMG_GRP_REGCUR_F  87
#define PMG_EDIT_REGCUR_F 88
#define PMG_UPDN_REGCUR_F 89
#define PMG_COMBO_REGCURF 90

int RegularCycles[] = {PMG_GRP_REGCUR_Q,  PMG_GRP_REGCUR_F,
                      PMG_EDIT_REGCUR_Q, PMG_EDIT_REGCUR_F,
                      PMG_UPDN_REGCUR_Q, PMG_UPDN_REGCUR_F,
                      PMG_COMBO_REGCURQ, PMG_COMBO_REGCURF, PMG_STATIC_REGCUR};

#define PMG_GRP_FRECUR_Q  91
#define PMG_EDIT_FRECUR_Q 92
#define PMG_UPDN_FRECUR_Q 93
#define PMG_COMBO_FRECURQ 94
#define PMG_GRP_FRECUR_N  95
#define PMG_EDIT_FRECUR_N 96
#define PMG_UPDN_FRECUR_N 97
#define PMG_STATIC_FRECUR 98
#define PMG_GRP_FRECUR_F  99
#define PMG_EDIT_FRECUR_F 100
#define PMG_UPDN_FRECUR_F 101
#define PMG_COMBO_FRECURF 102

int FreeCycles[]    = {PMG_GRP_FRECUR_Q,  PMG_GRP_FRECUR_N,  PMG_GRP_FRECUR_F,
                      PMG_EDIT_FRECUR_Q, PMG_EDIT_FRECUR_N, PMG_EDIT_FRECUR_F,
                      PMG_UPDN_FRECUR_Q, PMG_UPDN_FRECUR_N, PMG_UPDN_FRECUR_F,
                      PMG_COMBO_FRECURQ, PMG_STATIC_FRECUR, PMG_COMBO_FRECURF};

#define PMG_BASE          200
#define PMG_BLOCKS_WIDTH  110
#define PMG_PHASES_WIDTH  3 * PMG_BLOCKS_WIDTH + 10

bool isAllowedForGlovCycle(std::string temp)
{
	return ((std::string("KRYTP") == temp )
       || (std::string("KRYTH") == temp )) ;
}

bool isAllowedForCicadien(std::string& temp)
{
  return ((std::string("VNBDO") == temp)
       || (std::string("KCYCI") == temp)
       || (std::string("KMATI") == temp)
       || (std::string("KMIDI") == temp)
       || (std::string("KSOIR") == temp)
       || (std::string("KCOUC") == temp)
       || (std::string("KREVE") == temp)
       || (std::string("KGOUT") == temp)
       || (std::string("KNOCT") == temp)
       || (std::string("£?????") == temp)
       || (std::string("KHHMM") == temp)) ;
}

bool isAllowedForRythme(std::string& temp)
{
  return (( std::string("KRYRE") == temp)
      ||  ( std::string("KRYLI") == temp)
      ||  ( std::string("KJLUN") == temp)
      ||  ( std::string("KJMAR") == temp)
      ||  ( std::string("KJMER") == temp)
      ||  ( std::string("KJJEU") == temp)
      ||  ( std::string("KJVEN") == temp)
      ||  ( std::string("KJSAM") == temp)
      ||  ( std::string("KJDIM") == temp)
      ||  ( std::string("2DAT0") == temp)) ;
}

DEFINE_RESPONSE_TABLE1(NSPosoManagerGroup, TGroupBox)
	// EV_TCN_KEYDOWN(IDC_TABPSOLO,           KeyForCircadien),
	// EV_TCN_KEYDOWN(IDC_TABCYCLE,           KeyForRythme),
	EV_TCN_SELCHANGE(PMG_BASE + PMG_TAB_POSO,                  TabSelChangePoso), // Changement de table de la posologie
  EV_TCN_SELCHANGE(PMG_BASE + PMG_BLOCKS_WIDTH   + PMG_TAB_POSO,   TabSelChangePoso),
  EV_TCN_SELCHANGE(PMG_BASE + 2*PMG_BLOCKS_WIDTH + PMG_TAB_POSO,   TabSelChangePoso),
  EV_TCN_SELCHANGE(PMG_BASE + 3*PMG_BLOCKS_WIDTH + PMG_TAB_POSO,   TabSelChangePoso),
	EV_TCN_SELCHANGE(PMG_BASE + PMG_TAB_CYCLES,                TabSelChangeCycle), // Changement de table des cycles
  EV_TCN_SELCHANGE(PMG_BASE + PMG_BLOCKS_WIDTH   + PMG_TAB_CYCLES, TabSelChangeCycle),
  EV_TCN_SELCHANGE(PMG_BASE + 2*PMG_BLOCKS_WIDTH + PMG_TAB_CYCLES, TabSelChangeCycle),
  EV_TCN_SELCHANGE(PMG_BASE + 3*PMG_BLOCKS_WIDTH + PMG_TAB_CYCLES, TabSelChangeCycle),
	// EV_BN_CLICKED(IDC_CHANGECYCLE,         DrawCycleMode),
END_RESPONSE_TABLE ;

NSPosoManagerGroup::NSPosoManagerGroup(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, int Id, const char far *text, int x, int y, int w, int h, TModule* module)
                   :NSRoot(pCtx), TGroupBox(parent, Id, text, x, y, w, h, module), _PPT(pCtx)
{
	if (NULL != pPPToInit)
		_PPT = *pPPToInit ;

	_pActiveDailyBlock = NULL ;
	_pActiveCycleBlock = NULL ;

  _iIdBase = Id ;

	createInterfaceElements() ;
}

NSPosoManagerGroup::NSPosoManagerGroup(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, int resourceId, TModule* module)
                   :NSRoot(pCtx), TGroupBox(parent, resourceId, module), _PPT(pCtx)
{
	if (NULL != pPPToInit)
		_PPT = *pPPToInit ;

	_pActiveDailyBlock = NULL ;
	_pActiveCycleBlock = NULL ;

	_iIdBase = resourceId ;

	createInterfaceElements() ;
}

NSPosoManagerGroup::~NSPosoManagerGroup()
{
	deleteInterfaceElements() ;
}

void
NSPosoManagerGroup::SetupWindow()
{
	TGroupBox::SetupWindow() ;

	if (!(aDailyInterfaces.empty()))
	{
		NSPosoInterfaceBlockIter ieIt = aDailyInterfaces.begin() ;
		for ( ; ieIt != aDailyInterfaces.end() ; ieIt++)
			(*ieIt)->setupWindow() ;
  }

  if (!(aCycleInterfaces.empty()))
	{
		NSPosoInterfaceBlockIter ieIt = aCycleInterfaces.begin() ;
		for ( ; ieIt != aCycleInterfaces.end() ; ieIt++)
    	(*ieIt)->setupWindow() ;
  }
}

void
NSPosoManagerGroup::TabSelChangePoso(TNotify far& /* nm */)
{
	int index = tabPoso->GetSel() ;
	// SaveSimpleRythme() ;
	updatePosologie(index) ;
}

void
NSPosoManagerGroup::updatePosologie(int index)
{
	if (aDailyInterfaces.empty())
		return ;

	NSPosoInterfaceBlockIter ieIt = aDailyInterfaces.begin() ;
	for (int i = 0 ; ieIt != aDailyInterfaces.end() ; ieIt++, i++)
	{
  	if (i == index)
    	(*ieIt)->display(true) ;
    else
			(*ieIt)->display(false) ;
	}

	tabPoso->SetSel(index) ;
}

void
NSPosoManagerGroup::TabSelChangeCycle(TNotify far& /* nm */)
{
	register int index = tabCycle->GetSel() ;
	updateCycle(index) ;
}

void
NSPosoManagerGroup::updateCycle(int index)
{
	if (aCycleInterfaces.empty())
		return ;

	NSPosoInterfaceBlockIter ieIt = aCycleInterfaces.begin() ;
	for (int i = 0 ; ieIt != aCycleInterfaces.end() ; ieIt++, i++)
	{
  	if (i == index)
    	(*ieIt)->display(true) ;
    else
			(*ieIt)->display(false) ;
	}

	tabCycle->SetSel(index) ;
}

void
NSPosoManagerGroup::createInterfaceElements()
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "daylyTake") ;
  _pGrpPoso = new TGroupBox(this, _iIdBase + PMG_GRP_POSO, sMsgTxt.c_str(), 0, 0, 0, 0) ;
	tabPoso   = new TTabControl(this, _iIdBase + PMG_TAB_POSO, 0, 0, 0, 0) ;

  // Poso

  aDailyInterfaces.push_back(new NSPosoMMSCInterfaceBlock(this, pContexte)) ;
  aDailyInterfaces.push_back(new NSPosoLMMGSCNInterfaceBlock(this, pContexte)) ;
  aDailyInterfaces.push_back(new NSPosoSingleTakeInterfaceBlock(this, pContexte)) ;
  aDailyInterfaces.push_back(new NSPosoHoursInterfaceBlock(this, pContexte)) ;
  aDailyInterfaces.push_back(new NSPosoRegularCycleInterfaceBlock(this, pContexte)) ;
  aDailyInterfaces.push_back(new NSPosoFreeCycleInterfaceBlock(this, pContexte)) ;
  aDailyInterfaces.push_back(new NSPosoFreeTextInterfaceBlock(this, pContexte)) ;

	// Cycle

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "cycle") ;
  _pGrpCycle = new TGroupBox(this, _iIdBase + PMG_GRP_CYCLES, sMsgTxt.c_str(), 0, 0, 0, 0) ;
	tabCycle   = new TTabControl(this, _iIdBase + PMG_TAB_CYCLES, 0, 0, 0, 0) ;

  aCycleInterfaces.push_back(new NSEverydayCycleInterfaceBlock(this, pContexte)) ;
  aCycleInterfaces.push_back(new NSDaysOfWeekCycleInterfaceBlock(this, pContexte)) ;
  aCycleInterfaces.push_back(new NSD1D2D3CycleInterfaceBlock(this, pContexte)) ;
  aCycleInterfaces.push_back(new NSRegularCycleInterfaceBlock(this, pContexte)) ;
  aCycleInterfaces.push_back(new NSFreeCycleInterfaceBlock(this, pContexte)) ;

  parsePpt() ;

  if (NULL == _pActiveDailyBlock)
		_pActiveDailyBlock = *(aDailyInterfaces.begin()) ;

	if (NULL == _pActiveCycleBlock)
		_pActiveCycleBlock = *(aCycleInterfaces.begin()) ;
}

void
NSPosoManagerGroup::deleteInterfaceElements()
{
	delete tabPoso ;
  delete _pGrpPoso ;
  delete tabCycle ;
	delete _pGrpCycle ;
}

bool
NSPosoManagerGroup::parsePpt()
{
	if (_PPT.empty())
		return true ;

	PatPathoIter pptIter = _PPT.begin() ;
  PatPathoIter pptEnd  = _PPT.end() ;

  while (pptIter != pptEnd)
  {
  	int         col  = (*pptIter)->getColonne() ;
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if (isAllowedForGlovCycle(temp) == false)
    	if (recupere(pptIter, pptEnd, col, pContexte) == false)
      	return false ;

    if (temp == "KRYTP")   // Rythme
    {
    	NSPatPathoArray patpathoCycle(pContexte) ;
      patpathoCycle.ExtrairePatPatho(pptIter, &_PPT) ;
      parsePptCycle(&patpathoCycle) ;
    }
    if (temp == "KRYTH")   // Cycle circadien
    {
    	NSPatPathoArray patpathoCircadian(pContexte) ;
      patpathoCircadian.ExtrairePatPatho(pptIter, &_PPT) ;
      parsePptCircadian(&patpathoCircadian) ;
    }
	}

  return true ;
}

bool
NSPosoManagerGroup::parsePptCircadian(NSPatPathoArray* pPPT)
{
	if (aDailyInterfaces.empty())
		return true ;

	NSPosoInterfaceBlockIter ieIt = aDailyInterfaces.begin() ;

	for ( ; ieIt != aDailyInterfaces.end() ; ieIt++)
	{
  	if ((*ieIt)->parsePpt(pPPT))
    {
    	(*ieIt)->setPatho(pPPT) ;
      if ((*ieIt)->initFromPatPatho())
      {
      	_pActiveDailyBlock = *ieIt ;
      	return true ;
      }
    }
  }

  return false ;
}

bool
NSPosoManagerGroup::parsePptCycle(NSPatPathoArray* pPPT)
{
	if (aCycleInterfaces.empty())
		return true ;

	NSPosoInterfaceBlockIter ieIt = aCycleInterfaces.begin() ;

	for ( ; ieIt != aCycleInterfaces.end() ; ieIt++)
	{
  	if ((*ieIt)->parsePpt(pPPT))
    {
    	(*ieIt)->setPatho(pPPT) ;
      if ((*ieIt)->initFromPatPatho())
      {
      	_pActiveCycleBlock = *ieIt ;
      	return true ;
      }
    }
  }

  return false ;
}

bool
NSPosoManagerGroup::recupere(PatPathoIter& pptIter, PatPathoIter& pptend, int Col, NSContexte* pContexte)
{
	if (pptIter == pptend)
		return false ;

  int col = (*pptIter)->getColonne() ;

  while ((pptIter != pptend) || (col <= Col))
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;
    col = (*pptIter)->getColonne() ;
  	if (isAllowedForGlovCycle(temp) == true)
    	return true ;
    Avance(pptIter, pptend) ;
  }
  return false ;
}

void
NSPosoManagerGroup::display(bool bVisible)
{
  enableControl(tabPoso->GetId(), bVisible) ;
  enableControl(_pGrpPoso->GetId(), bVisible) ;
  enableControl(tabCycle->GetId(), bVisible) ;
  enableControl(_pGrpCycle->GetId(), bVisible) ;

	if (NULL != _pActiveDailyBlock)
  	_pActiveDailyBlock->display(bVisible) ;
  if (NULL != _pActiveCycleBlock)
		_pActiveCycleBlock->display(bVisible) ;
}

void
NSPosoManagerGroup::enableControl(int RessourceId, bool visible)
{
	int iResId = _iIdBase + RessourceId ;
  HWND hwndCtrl = GetDlgItem(iResId) ;
  if (NULL == hwndCtrl)
		return ;

	if (visible)
  {
    ::ShowWindow(hwndCtrl,   SW_SHOW) ;
    ::EnableWindow(hwndCtrl, TRUE ) ;
  }
  else
  {
    ::ShowWindow(hwndCtrl,   SW_HIDE) ;
    ::EnableWindow(hwndCtrl, FALSE ) ;
  }
}

void
NSPosoManagerGroup::initTabPosologie()
{
	if (aDailyInterfaces.empty())
		return ;

	NSPosoInterfaceBlockIter ieIt = aDailyInterfaces.begin() ;

	for ( ; ieIt != aDailyInterfaces.end() ; ieIt++)
	{
    TTabItem tab(pContexte->getSuperviseur()->getText("drugDialog", (*ieIt)->getTitle()).c_str()) ;
    tabPoso->Add(tab) ;
  }
}

void
NSPosoManagerGroup::drawPoso()
{
}

void
NSPosoManagerGroup::initTabCycle()
{
	if (aCycleInterfaces.empty())
		return ;

	NSPosoInterfaceBlockIter ieIt = aCycleInterfaces.begin() ;

	for ( ; ieIt != aCycleInterfaces.end() ; ieIt++)
	{
    TTabItem tab(pContexte->getSuperviseur()->getText("drugDialog", (*ieIt)->getTitle()).c_str()) ;
    tabCycle->Add(tab) ;
  }
}

void
NSPosoManagerGroup::drawCycle()
{
}

void
NSPosoManagerGroup::moveControl(int iResId, int left, int top, int h , int w)
{
	TDialog* pParentDlg = TYPESAFE_DOWNCAST(Parent, TDialog) ;
	if (NULL == pParentDlg)
		return ;

	HWND hwndCtrl = pParentDlg->GetDlgItem(iResId) ;
  if (NULL == hwndCtrl)
		return ;

	NS_CLASSLIB::TRect recGen(left, top, left + w, top + h) ;
  pParentDlg->MapDialogRect(recGen) ;
  ::MoveWindow(hwndCtrl, recGen.left , recGen.top, recGen.Width(), recGen.Height(), true) ;
}

//
// ------------------------ Class NSPosoInterfaceBlock --------------------
//

NSPosoInterfaceBlock::NSPosoInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                     :NSRoot(pCtx), _PPT(pCtx)
{
	_parent = parent ;

	if (NULL != pPPToInit)
		_PPT = *pPPToInit ;
}

NSPosoInterfaceBlock::~NSPosoInterfaceBlock()
{
}

void
NSPosoInterfaceBlock::setPatho(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return ;

	_PPT = *pPpt ;
}

string
NSPosoInterfaceBlock::getNbDose(PatPathoIter& pptIter, PatPathoIter& pptend)
{
	string temp = string("") ;

	Avance(pptIter, pptend) ;
  if (pptIter == pptend)
  	return temp ;

  Avance(pptIter, pptend) ;
  if (pptIter == pptend)
  	return temp ;

	temp = (*pptIter)->getComplement() ;

  Avance(pptIter, pptend) ;

  return temp ;
}

void
NSPosoInterfaceBlock::addElement(std::string sroot, std::string quant, NSPatPathoArray* pat, int col)
{
	if ((NULL == pat) || (std::string("") == quant))
		return ;

  Message CodeMsg ;

	if (sroot != "VNBDO1")
	{
  	pat->ajoutePatho(sroot, col, 0) ;
    pat->ajoutePatho("VNBDO1", col+1, 0) ;
		CodeMsg.SetUnit("200001") ;
    CodeMsg.SetComplement(quant) ;
    pat->ajoutePatho("£N0;03", &CodeMsg, col+2, 1) ;
	}
  else
  {
  	pat->ajoutePatho("VNBDO1", col, 0) ;
		CodeMsg.SetUnit("200001") ;
    CodeMsg.SetComplement(quant) ;
    pat->ajoutePatho("£N0;03", &CodeMsg, col+3, 1) ;
	}
}

bool
NSPosoInterfaceBlock::isValidValue(string* psValue)
{
	if (!psValue)
  	return false ;

  strip(*psValue, stripBoth, ' ') ;
  strip(*psValue, stripLeft, '0') ;

	if (*psValue == "")
  	return false ;

  // Handling decimal value of the kind .5 -> 0.5
  if ((*psValue)[0] == '.')
  	*psValue = string(1, '0') + *psValue ;
	// Handling decimal value of the kind /2 -> 1/2
  else if ((*psValue)[0] == '/')
  	*psValue = string(1, '1') + *psValue ;

  size_t iLen = strlen(psValue->c_str()) ;
  if (iLen > BASE_COMPLEMENT_LEN)
  	return false ;

  bool bAlreadySeenADot   = false ;
  bool bAlreadySeenASlash = false ;
  for (size_t i = 0 ; i < iLen ; i++)
  {
  	if ((*psValue)[i] == '.')
    {
    	if (bAlreadySeenASlash || bAlreadySeenADot)
      	return false ;
      bAlreadySeenADot = true ;
    }
    else if ((*psValue)[i] == '/')
    {
    	if (bAlreadySeenASlash || bAlreadySeenADot)
      	return false ;
      bAlreadySeenASlash = true ;
    }
  	else if (!(isdigit((*psValue)[i])))
    	return false ;
  }

  return true ;
}

//
// -------------------- Class NSPosoMMSCInterfaceBlock ----------------
//

NSPosoMMSCInterfaceBlock::NSPosoMMSCInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "morning") ;
  aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_MORNING, sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "noon") ;
	aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_NOON,    sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "evening") ;
  aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_EVENING, sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "bedtime") ;
  aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_BED,     sMsgTxt.c_str(), 0, 0, 0, 0)) ;

  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KMATI1", _parent->getIdBase() + PMG_EDIT_MORNING, _parent->getIdBase() + PMG_UPDN_MORNING, "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KMIDI1", _parent->getIdBase() + PMG_EDIT_NOON,    _parent->getIdBase() + PMG_UPDN_NOON,    "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KSOIR1", _parent->getIdBase() + PMG_EDIT_EVENING, _parent->getIdBase() + PMG_UPDN_EVENING, "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KCOUC1", _parent->getIdBase() + PMG_EDIT_BED,     _parent->getIdBase() + PMG_UPDN_BED,     "", 0, 0, 0, 0)) ;
}

NSPosoMMSCInterfaceBlock::~NSPosoMMSCInterfaceBlock()
{
}

void
NSPosoMMSCInterfaceBlock::setupWindow()
{
	if (aEdits.empty())
		return ;

	NSUpDownEditIter udeIt = aEdits.begin() ;
	for ( ; udeIt != aEdits.end() ; udeIt++)
		(*udeIt)->setupWindow() ;
}

bool
NSPosoMMSCInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	if (aEdits.empty())
		return false ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

    NSUpDownEditIter udeIt = aEdits.begin() ;
    for ( ; (udeIt != aEdits.end()) && (temp != (*udeIt)->getRootSens()) ; udeIt++) ;
    if (udeIt == aEdits.end())
    	return false ;

    pptIter++ ;
    if (pptIter == pptEnd)
    	return false ;

    string sNbDose = getNbDose(pptIter, pptEnd) ;
    if (string("") == sNbDose)
    	return false ;

    if (pPpt == &_PPT)
    	(*udeIt)->setInit(sNbDose) ;
	}

	return true ;
}

bool
NSPosoMMSCInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

  if (aEdits.empty())
		return false ;

	NSUpDownEditIter udeIt = aEdits.begin() ;
	for ( ; udeIt != aEdits.end() ; udeIt++)
		(*udeIt)->createTree(&_PPT, 0) ;

	return true ;
}

void
NSPosoMMSCInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 12; i++)
  	_parent->enableControl(periodeMMSC[i], bVisible) ;
}

void
NSPosoMMSCInterfaceBlock::moveBlock()
{        
	//              MORNING, NOON, EVENING, BED

	int xValues[]  = {  8,  72, 135, 198,        // Grp
                     12,  76, 137, 201,        // Edit
                     50, 114, 178, 240 };      // UpDn
  int yValues[]  = { 26,  26,  26,  26,
                     35,  35,  35,  35,
                     35,  35,  35,  35 };
  int wValues[]  = { 60,  60,  60,  60,
                     40,  40,  40,  40,
                     11,  11,  11,  11 };
  int hValues[]  = { 28,  28,  28,  28,
                     15,  15,  15,  15,
                     15,  15,  15,  15 };

	for (int i = 0; i < 12; i++)
		_parent->moveControl(periodeMMSC[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSPosoMMSCInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

//
// -------------------- Class NSPosoLMMGSCNInterfaceBlock ----------------
//

NSPosoLMMGSCNInterfaceBlock::NSPosoLMMGSCNInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "wakeup") ;
	aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_WAKEUP,   sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "morning") ;
	aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_MORNING2, sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "noon") ;
	aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_NOON2,    sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "afternoonSnack") ;
	aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_SNACK,    sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "evening") ;
  aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_EVENING2, sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "bedtime") ;
  aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_BED,      sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "night") ;
  aGroups.push_back(new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_NIGHT,    sMsgTxt.c_str(), 0, 0, 0, 0)) ;

  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KREVE1", _parent->getIdBase() + PMG_EDIT_WAKEUP,   _parent->getIdBase() + PMG_UPDN_WAKEUP,   "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KMATI1", _parent->getIdBase() + PMG_EDIT_MORNING2, _parent->getIdBase() + PMG_UPDN_MORNING2, "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KMIDI1", _parent->getIdBase() + PMG_EDIT_NOON2,    _parent->getIdBase() + PMG_UPDN_NOON2,    "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KGOUT1", _parent->getIdBase() + PMG_EDIT_SNACK,    _parent->getIdBase() + PMG_UPDN_SNACK,    "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KSOIR1", _parent->getIdBase() + PMG_EDIT_EVENING2, _parent->getIdBase() + PMG_UPDN_EVENING2, "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KCOUC1", _parent->getIdBase() + PMG_EDIT_BED2,     _parent->getIdBase() + PMG_UPDN_BED2,     "", 0, 0, 0, 0)) ;
  aEdits.push_back(new NSUpDownEdit(_parent, pContexte, "KNOCT3", _parent->getIdBase() + PMG_EDIT_NIGHT,    _parent->getIdBase() + PMG_UPDN_NIGHT,    "", 0, 0, 0, 0)) ;
}

NSPosoLMMGSCNInterfaceBlock::~NSPosoLMMGSCNInterfaceBlock()
{
}

void
NSPosoLMMGSCNInterfaceBlock::setupWindow()
{
	if (aEdits.empty())
		return ;

	NSUpDownEditIter udeIt = aEdits.begin() ;
	for ( ; udeIt != aEdits.end() ; udeIt++)
		(*udeIt)->setupWindow() ;
}

bool
NSPosoLMMGSCNInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

    NSUpDownEditIter udeIt = aEdits.begin() ;
    for ( ; (udeIt != aEdits.end()) && (temp != (*udeIt)->getRootSens()) ; udeIt++) ;
    if (udeIt == aEdits.end())
    	return false ;

    pptIter++ ;
    if (pptIter == pptEnd)
    	return false ;

    string sNbDose = getNbDose(pptIter, pptEnd) ;
    if (string("") == sNbDose)
    	return false ;

    if (pPpt == &_PPT)
			(*udeIt)->setInit(sNbDose) ;
	}

	return true ;
}

bool
NSPosoLMMGSCNInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

  if (aEdits.empty())
		return false ;

	NSUpDownEditIter udeIt = aEdits.begin() ;
	for ( ; udeIt != aEdits.end() ; udeIt++)
		(*udeIt)->createTree(&_PPT, 0) ;

	return true ;
}

void
NSPosoLMMGSCNInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 21; i++)
  	_parent->enableControl(periodeEvents[i], bVisible) ;
}

void
NSPosoLMMGSCNInterfaceBlock::moveBlock()
{     

	//              WAKEUP, MORNING, NOON, SNACK, EVENING, BED, NIGHT

	int xValues[]  = {  6,  58, 110, 162, 214, 266, 318,        // Grp
                     10,  62, 113, 165, 217, 269, 321,        // Edit
                     38,  90, 142, 189, 245, 297, 349 };      // UpDn
  int yValues[]  = { 26,  26,  26,  26,  26,  26,  26,
                     35,  35,  35,  35,  35,  35,  35,
                     35,  35,  35,  35,  35,  35,  35 };
  int wValues[]  = { 50,  50,  50,  50,  50,  50,  50,
                     30,  30,  30,  30,  30,  30,  30,
                     11,  11,  11,  11,  11,  11,  11 };
  int hValues[]  = { 28,  28,  28,  28,  28,  28,  28,
                     15,  15,  15,  15,  15,  15,  15,
                     15,  15,  15,  15,  15,  15,  15 };

	for (int i = 0; i < 21; i++)
		_parent->moveControl(periodeEvents[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSPosoLMMGSCNInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}
                                //
// -------------------- Class NSPosoSingleTakeInterfaceBlock ----------------
//

NSPosoSingleTakeInterfaceBlock::NSPosoSingleTakeInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "quantity") ;
	_pGroupPriseUnique = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_SINGLE, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  priseUnique        = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_SINGLE, _parent->getIdBase() + PMG_UPDN_SINGLE, "", 0, 0, 0, 0) ;
}

NSPosoSingleTakeInterfaceBlock::~NSPosoSingleTakeInterfaceBlock()
{
  delete _pGroupPriseUnique ;
  delete priseUnique ;
}

void
NSPosoSingleTakeInterfaceBlock::setupWindow()
{
	priseUnique->setupWindow() ;
}

bool
NSPosoSingleTakeInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

	std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

  if (string("VNBDO") != temp)
		return false ;

	string sNbDose = getNbDose(pptIter, pptEnd) ;
	if (string("") == sNbDose)
		return false ;

	if (pPpt == &_PPT)
		priseUnique->setInit(sNbDose) ;

	return true ;
}

bool
NSPosoSingleTakeInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

	priseUnique->createTree(&_PPT, 0) ;

	return true ;
}

void
NSPosoSingleTakeInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 3; i++)
  	_parent->enableControl(PosoUniques[i], bVisible) ;
}

void
NSPosoSingleTakeInterfaceBlock::moveBlock()
{        

	//              WAKEUP, MORNING, NOON, SNACK, EVENING, BED, NIGHT

	int xValues[]  = {  3,        // Grp
                      9,        // Edit
                     63 };      // UpDn
  int yValues[]  = { 26,
                     35,
                     35 };
  int wValues[]  = { 76,
                     56,
                     11 };
  int hValues[]  = { 28,
                     15,
                     15 };

	for (int i = 0; i < 3; i++)
		_parent->moveControl(PosoUniques[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSPosoSingleTakeInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

//
// -------------------- Class NSPosoHoursInterfaceBlock ----------------
//

NSPosoHoursInterfaceBlock::NSPosoHoursInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "quantity") ;
  _pGroupHourQtty = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_HOUR_Q, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "takeHour") ;
  _pGroupHour     = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_HOUR_H, sMsgTxt.c_str(), 0, 0, 0, 0) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "addANewTake") ;
  _pNewHourlyTake = new TButton(_parent, _parent->getIdBase() + PMG_BTN_HOUR_A, sMsgTxt.c_str(), 0, 0, 0, 0) ;

  pPriseHeure     = new NSUpDownEditHeureControle(_parent, pContexte, "KHHMM1", _parent->getIdBase() + PMG_EDIT_HOUR_Q, _parent->getIdBase() + PMG_UPDN_HOUR_Q, "", 0, 0, 0, 0, _parent->getIdBase() + PMG_EDIT_HOUR_H, _parent->getIdBase() + PMG_UPDN_HOUR_H, "", 0, 0, 0, 0) ;
  pPriseHeure->getEditNum()->getEditNum()->SetLostFocusResponse(new MemFunctor<NSPosoHoursInterfaceBlock>( (NSPosoHoursInterfaceBlock*)this, &NSPosoHoursInterfaceBlock::saveChangeHour)) ;
  pPriseHeure->getEditNum()->getUpDown()->SetLostFocusResponse(new MemFunctor<NSPosoHoursInterfaceBlock>( (NSPosoHoursInterfaceBlock*)this, &NSPosoHoursInterfaceBlock::saveChangeHour)) ;

  ViewPriseHeure  = new NTTList<PriseHeure, NSPosoHoursInterfaceBlock>(_parent, _parent->getIdBase() + PMG_LIST_HOUR_L, 0, 0, 0, 0) ;
  ViewPriseHeure->setKeyUpResponse(new MemFunctor<NSPosoHoursInterfaceBlock>( (NSPosoHoursInterfaceBlock*)this, &NSPosoHoursInterfaceBlock::funcMoveForHour)) ;
  ViewPriseHeure->setKeyDownResponse(new MemFunctor<NSPosoHoursInterfaceBlock>( (NSPosoHoursInterfaceBlock*)this, &NSPosoHoursInterfaceBlock::funcMoveForHour)) ;

  _prises = new std::vector<PriseHeure*>() ;
  ViewPriseHeure->SetAssociatedData(_prises) ;
}

NSPosoHoursInterfaceBlock::~NSPosoHoursInterfaceBlock()
{
	delete _pGroupHourQtty ;
	delete _pGroupHour ;
	delete _pNewHourlyTake ;

	delete pPriseHeure ;

  delete ViewPriseHeure ;

  for (size_t i = 0; i < _prises->size() ; i++)
		delete((*_prises)[i]) ;
  _prises->clear() ;
  delete(_prises) ;
}

void
NSPosoHoursInterfaceBlock::setupWindow()
{
}

bool
NSPosoHoursInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if (string("KHHMM") != temp)
    	return false ;

    bool bSuccess = loadPriseHeure(pptIter, pptEnd) ;
    if (false == bSuccess)
    	return false ;
	}

	return true ;
}

bool
NSPosoHoursInterfaceBlock::loadPriseHeure(PatPathoIter& pptIter, PatPathoIter& pptend)
{
  std::string heure = "";
  std::string quantity = "";
  Avance(pptIter,pptend); // mange KHHM1
  if (pptIter !=  pptend)
  {
    std::string temp = (*pptIter)->getLexique();
    if  (temp == "£H0;04")
    {
    	heure = (*pptIter)->getComplement() ;
      Avance(pptIter, pptend) ;
      if (pptIter !=  pptend)
      {
      	Avance(pptIter, pptend) ; // Mange le VBNOI
        if (pptIter != pptend)
        {
        	quantity = (*pptIter)->getComplement() ;
        	Avance(pptIter, pptend) ;
        }
      }
    }
  }

  if ((heure != "") && (quantity != ""))
  {
  	_prises->push_back(new PriseHeure(quantity, heure)) ;
    return true ;
	}

  return false ;
}

bool
NSPosoHoursInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

	for (size_t i = 0; i < _prises->size(); i++)
 	{
    NSPatPathoArray* pTemp = (*_prises)[i]->CreateTree(pContexte) ;
    if (NULL != pTemp)
    {
    	if (!(pTemp->empty()))
    		_PPT.InserePatPatho(_PPT.end(), pTemp, 0) ;
   		delete pTemp ;
    }
 	}

  return true ;
}

void
NSPosoHoursInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 8; i++)
  	_parent->enableControl(hourCycles[i], bVisible) ;
}

void
NSPosoHoursInterfaceBlock::moveBlock()
{         // 4, 88, 408, 58

	//               QTTY, HOUR

	int xValues[]  = {  3,  77,        // Grp
                      7,  82,        // Edit
                     53, 129,        // UpDn
                    150, 232 };
  int yValues[]  = { 26,  26,
                     35,  35,
                     35,  35,
                     35,  26 };
  int wValues[]  = { 70,  70,
                     48,  48,
                     11,  11,
                     64, 164 };
  int hValues[]  = { 28,  28,
                     15,  15,
                     15,  15,
                     15,  32 };

	for (int i = 0; i < 8; i++)
		_parent->moveControl(hourCycles[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSPosoHoursInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

void
NSPosoHoursInterfaceBlock::saveChangeHour()
{
  int selectIndex = ViewPriseHeure->GetSelIndex() ;
  if (selectIndex < 0)
		return ;

	std::string heure = pPriseHeure->getHeure() ;
	std::string quant = pPriseHeure->getValue() ;
	PriseHeure* temp  = ViewPriseHeure->getDataAt(selectIndex) ;
	temp->setHeure(heure) ;
	temp->setQuantity(quant) ;
	std::string toAff = CreateLabelForHour(quant,heure ) ;
	ViewPriseHeure->ChangeLabel(toAff, selectIndex) ;
	// ViewPriseHeure->sortLabel(CompareMedicament) ;
}

void
NSPosoHoursInterfaceBlock::funcMoveForHour()
{
  int index = ViewPriseHeure->GetSelIndex();
  if (index  >=0)
  {
  }
}

//
// -------------------- Class NSPosoRegularCycleInterfaceBlock ----------------
//

NSPosoRegularCycleInterfaceBlock::NSPosoRegularCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "quantity") ;
  _pGroupNbOfUnit  = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_REGCYC_Q, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  _quantRCycle     = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_REGCYC_Q, _parent->getIdBase() + PMG_UPDN_REGCYC_Q, "", 0, 0, 0, 0) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "every") ;
  _pStaticEvery    = new TStatic(_parent, _parent->getIdBase() + PMG_STATIC_REGCYC, sMsgTxt.c_str(), 0, 0, 0, 0) ;

	sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "takeHour") ;
  _pGroupFrequency = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_REGCYC_F, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  _freqRCycle      = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_REGCYC_F, _parent->getIdBase() + PMG_UPDN_REGCYC_F, "", 0, 0, 0, 0) ;
	char *tempre[]   = {"2SEC01","2MINU1","2HEUR1"} ;
	VecteurString LexiqJours(tempre) ;
	delete[] (tempre) ;
  _RCycleComboF    = new NSComboBox(_parent, _parent->getIdBase() + PMG_COMBO_REGCYC, pContexte, 0, 0, 0, 0, CBS_DROPDOWNLIST, &LexiqJours) ;
}

NSPosoRegularCycleInterfaceBlock::~NSPosoRegularCycleInterfaceBlock()
{
	delete _pGroupNbOfUnit ;
  delete _quantRCycle ;
  delete _pStaticEvery ;

  delete _pGroupFrequency ;
  delete _freqRCycle ;
  delete _RCycleComboF ;
}

void
NSPosoRegularCycleInterfaceBlock::setupWindow()
{
	_quantRCycle->setupWindow() ;
	_freqRCycle->setupWindow() ;
  _RCycleComboF->setCode(_sInitForCombo) ;
}

bool
NSPosoRegularCycleInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

  if (std::string("KCYCI") != temp)
  	return false ;

  Avance(pptIter, pptEnd) ;

  if (std::string("KRYRE") != temp)
  	return false ;

	Avance(pptIter, pptEnd) ;

  while (pptIter != pptEnd)
  {
    temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if (std::string("KDURC") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sCodeFreq = (*pptIter)->getUnit() ;
        if (pPpt == &_PPT)
  				_sInitForCombo = sCodeFreq ;

        string sFreq     = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				_freqRCycle->setInit(sFreq) ;

        Avance(pptIter, pptEnd) ;
      }
    }
    else if (std::string("KDURA") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      	Avance(pptIter, pptEnd) ;
    }
    else if (std::string("VNBDO") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sQuantity = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				_quantRCycle->setInit(sQuantity) ;
      	Avance(pptIter, pptEnd) ;
      }
    }
    else
    	return false ;
	}

	return true ;
}

bool
NSPosoRegularCycleInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

	_PPT.ajoutePatho("KCYCI1", 0, 1) ;
	_PPT.ajoutePatho("KRYRE1", 1, 1) ;

	Message CodeMsg  ;

  _PPT.ajoutePatho("KDURC1", 2, 1) ;    // duree de la cure premier edit
	CodeMsg.SetUnit(_RCycleComboF->getSelCode()) ;
	CodeMsg.SetComplement(_freqRCycle->getText()) ;
	_PPT.ajoutePatho("£N0;03", &CodeMsg, 3, 1) ;

	_PPT.ajoutePatho("KDURA1", 2, 1) ;
  CodeMsg.Reset() ;
	CodeMsg.SetUnit("2FOIS1") ;
	CodeMsg.SetComplement("1") ;
	_PPT.ajoutePatho("£N0;03", &CodeMsg, 3, 1) ;

	_PPT.ajoutePatho("VNBDO1",  1, 1) ;
  CodeMsg.Reset() ;
	CodeMsg.SetUnit("200001") ;
	CodeMsg.SetComplement(_quantRCycle->getText()) ;
	_PPT.ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;

	return true ;
}

void
NSPosoRegularCycleInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 8; i++)
  	_parent->enableControl(regularDays[i], bVisible) ;
}

void
NSPosoRegularCycleInterfaceBlock::moveBlock()
{ 

	//               QTTY, HOUR

	int xValues[]  = {  3, 131,        // Grp
                     11, 135,        // Edit
                     39, 165,        // UpDn
                     66, 180 };
  int yValues[]  = { 26,  26,
                     35,  35,
                     35,  35,
                     37,  35 };
  int wValues[]  = { 60, 116,
                     30,  32,
                     11,  11,
                     60,  50 };
  int hValues[]  = { 28,  28,
                     15,  15,
                     15,  15,
                     10,  35 };

	for (int i = 0; i < 8; i++)
		_parent->moveControl(regularDays[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSPosoRegularCycleInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

//
// -------------------- Class NSPosoFreeCycleInterfaceBlock ----------------
//

NSPosoFreeCycleInterfaceBlock::NSPosoFreeCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "quantity") ;
  _pGroupFNbOfUnit = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_FRECYC_Q, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  _quantFCycle     = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_FRECYC_Q, _parent->getIdBase() + PMG_UPDN_FRECYC_Q, "", 0, 0, 0, 0) ;

  _pGroupFTime     = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_FRECYC_T, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  _quantFoisCycle  = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_FRECYC_T, _parent->getIdBase() + PMG_UPDN_FRECYC_T, "", 0, 0, 0, 0) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "every") ;
  _pStaticFEvery   = new TStatic(_parent, _parent->getIdBase() + PMG_STATIC_FRECYC, sMsgTxt.c_str(), 0, 0, 0, 0) ;

  char *champpLexiqJour[] = {"2SEC01", "2MINU1", "2HEUR1", "2DAT01", "2FOIS1"} ; // sec min  heure    jour fois
  VecteurString LexiqJour(champpLexiqJour) ;
  delete[] (champpLexiqJour) ;

  _pGroupFFreq     = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_FRECYC_F, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  _quantFreqFCycle = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_FRECYC_F, _parent->getIdBase() + PMG_UPDN_FRECYC_F, "", 0, 0, 0, 0) ;

  _FCycleComboF    = new NSComboBox(_parent, _parent->getIdBase() + PMG_COMBO_FRECYC, pContexte, 0, 0, 0, 0, CBS_DROPDOWNLIST, &LexiqJour) ;
  _sInitForCombo   = string("") ;
}

NSPosoFreeCycleInterfaceBlock::~NSPosoFreeCycleInterfaceBlock()
{
	delete _pGroupFNbOfUnit ;
	delete _quantFCycle ;
  delete _pGroupFTime ;
  delete _quantFoisCycle ;
  delete _pStaticFEvery ;
  delete _pGroupFFreq ;
  delete _quantFreqFCycle ;
  delete _FCycleComboF ;
}

void
NSPosoFreeCycleInterfaceBlock::setupWindow()
{
	_quantFCycle->setupWindow() ;
	_quantFoisCycle->setupWindow() ;
	_quantFreqFCycle->setupWindow() ;
  _FCycleComboF->setCode(_sInitForCombo) ;
}

bool
NSPosoFreeCycleInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

  if (std::string("KCYCI") != temp)
  	return false ;

  Avance(pptIter, pptEnd) ;

  if (std::string("KRYLI") != temp)
  	return false ;

  while (pptIter != pptEnd)
  {
    temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if (std::string("KDURC") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sCodeFreq = (*pptIter)->getUnit() ;
        if (pPpt == &_PPT)
  				_sInitForCombo = sCodeFreq ;

        string sFreq     = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				_quantFreqFCycle->setInit(sFreq) ;

        Avance(pptIter, pptEnd) ;
      }
    }
    else if (std::string("VAINC") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sQuantity = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				_quantFoisCycle->setInit(sQuantity) ;
      	Avance(pptIter, pptEnd) ;
      }
    }
    else if (std::string("VNBDO") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sQuantity = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				_quantFCycle->setInit(sQuantity) ;
      	Avance(pptIter, pptEnd) ;
      }
    }
    else
    	return false ;
	}

	return true ;
}

bool
NSPosoFreeCycleInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

	_PPT.ajoutePatho("KCYCI1", 0, 1) ;
	_PPT.ajoutePatho("KRYLI1", 1, 1) ;

	Message CodeMsg  ;

  string sFrequence = _quantFreqFCycle->getText() ;
  if (isValidValue(&sFrequence))
	{
		_PPT.ajoutePatho("KDURC1", 3, 1) ;
		CodeMsg.SetUnit(_FCycleComboF->getSelCode()) ;
		CodeMsg.SetComplement(sFrequence) ;
		_PPT.ajoutePatho("£N0;03", &CodeMsg, 4, 1) ;
	}

  string sFois = _quantFoisCycle->getText() ;
	if (isValidValue(&sFois))
	{
		_PPT.ajoutePatho("VAINC1", 3, 1) ;
    CodeMsg.Reset() ;
		CodeMsg.SetUnit("2FOIS1") ;
		CodeMsg.SetComplement(sFois) ;
		_PPT.ajoutePatho("£N0;03", &CodeMsg, 4, 1) ;
	}

  string sQuantity = _quantFCycle->getText() ;
	if (isValidValue(&sQuantity))
	{
		_PPT.ajoutePatho("VNBDO1", 2, 1) ;
    CodeMsg.Reset() ;
		CodeMsg.SetUnit("200001") ;
		CodeMsg.SetComplement(sQuantity) ;
		_PPT.ajoutePatho("£N0;03", &CodeMsg, 5, 1) ;
	}

	return true ;
}

void
NSPosoFreeCycleInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 11; i++)
  	_parent->enableControl(idcPosoLibres[i], bVisible) ;
}

void
NSPosoFreeCycleInterfaceBlock::moveBlock()
{         // 4, 88, 408, 58
	//               QTTY, TIME, FREQ

	int xValues[]  = {  3,  67, 168,        // Grp
                     11,  74, 171,        // Edit
                     39, 108, 213,       // UpDn
                    132, 231 };
  int yValues[]  = { 26,  26,  26,
                     35,  35,  35,
                     35,  35,  35,
                     37,  35 };
  int wValues[]  = { 60,  60, 116,
                     30,  36,  44,
                     11,  11,  11,
                     38,  50 };
  int hValues[]  = { 28,  28,  28,
                     15,  15,  15,
                     15,  15,  15,
                     10,  88 };

	for (int i = 0; i < 11; i++)
		_parent->moveControl(idcPosoLibres[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSPosoFreeCycleInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

//
// -------------------- Class NSPosoFreeTextInterfaceBlock ----------------
//

NSPosoFreeTextInterfaceBlock::NSPosoFreeTextInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	pEditTextLibre = new NSTexteLibre(_parent, pContexte, "", 0, 0, 0, 0, "£C;020", _parent->getIdBase() + PMG_EDIT_FREETEXT) ;
  _sText         = string("") ;
}

NSPosoFreeTextInterfaceBlock::~NSPosoFreeTextInterfaceBlock()
{
	delete pEditTextLibre ;
}

void
NSPosoFreeTextInterfaceBlock::setupWindow()
{
	pEditTextLibre->setText(_sText) ;
}

bool
NSPosoFreeTextInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;

	std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

  if (string("£?????") != temp)
		return false ;

	if (pPpt == &_PPT)
		_sText = (*pptIter)->getTexteLibre() ;

	return true ;
}

bool
NSPosoFreeTextInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

	string sText = pEditTextLibre->getTexte() ;
  if (string("") == sText)
		return true ;

	Message CodeMsg ;
	CodeMsg.SetTexteLibre(sText) ;
	_PPT.ajoutePatho("£?????", &CodeMsg, 0, 1) ;

	return true ;
}

void
NSPosoFreeTextInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 1; i++)
  	_parent->enableControl(TxtLibres[i], bVisible) ;
}

void
NSPosoFreeTextInterfaceBlock::moveBlock()
{         // 4, 88, 408, 58
	int xValues[]  = {   3 };
  int yValues[]  = {  26 };
  int wValues[]  = { 401 };
  int hValues[]  = {  30 };

	for (int i = 0; i < 1; i++)
		_parent->moveControl(TxtLibres[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSPosoFreeTextInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{

}

//
// -------------------- Class NSEverydayCycleInterfaceBlock ----------------
//

NSEverydayCycleInterfaceBlock::NSEverydayCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	CycleSimple = new NTComboBox<NSPatPathoArray>(_parent, _parent->getIdBase() + PMG_COMBO_SIMPLE, 0, 0, 0, 0, CBS_DROPDOWNLIST, 0) ;

  std::string temp1 = pContexte->getSuperviseur()->getText("drugDialog", "everyDay") ;
  std::string temp2 = pContexte->getSuperviseur()->getText("drugDialog", "everyOtherDay") ;
  std::string temp3 = pContexte->getSuperviseur()->getText("drugDialog", "1DayOutOf3") ;
  std::string temp4 = pContexte->getSuperviseur()->getText("drugDialog", "1DayOutOf4") ;
  std::string temp5 = pContexte->getSuperviseur()->getText("drugDialog", "4DaysOutOf7") ;
  std::string temp6 = pContexte->getSuperviseur()->getText("drugDialog", "5DaysOutOf7") ;
  std::string temp7 = pContexte->getSuperviseur()->getText("drugDialog", "6DaysOutOf7") ;
  
  CycleSimple->AddElement(temp1, NULL); // tous les jours
  CycleSimple->AddElement(temp2, CreateRegularRythme("1", "2DAT01", "2", "2DAT01", pContexte)) ; // 1/2
  CycleSimple->AddElement(temp3, CreateRegularRythme("1", "2DAT01", "3", "2DAT01", pContexte)) ; // 1/3
  CycleSimple->AddElement(temp4, CreateRegularRythme("1", "2DAT01", "4", "2DAT01", pContexte)) ; // 1/4
  CycleSimple->AddElement(temp5, CreateRegularRythme("4", "2DAT01", "7", "2DAT01", pContexte)) ; // 4/7
  CycleSimple->AddElement(temp6, CreateRegularRythme("5", "2DAT01", "7", "2DAT01", pContexte)) ; // 5/7
  CycleSimple->AddElement(temp7, CreateRegularRythme("6", "2DAT01", "7", "2DAT01", pContexte)) ; // 5/7

	iSelectedIndex = 0 ;
}

NSEverydayCycleInterfaceBlock::~NSEverydayCycleInterfaceBlock()
{
	delete CycleSimple ;
}

void
NSEverydayCycleInterfaceBlock::setupWindow()
{
	CycleSimple->PrintCombo() ;
	CycleSimple->SetSelIndex(iSelectedIndex) ;
}

bool
NSEverydayCycleInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	size_t iSize = CycleSimple->getSize() ;

  for (size_t i = 0; i < iSize; i++)
  {
  	if (NULL != (*CycleSimple)[i])
    {
    	NSPatPathoArray* pPPT = (*CycleSimple)[i]->getElement() ;
			if ((NULL != pPPT) && (pPpt->estEgal(pPPT)))
      {
				if (pPpt == &_PPT)
        	iSelectedIndex = i ;
        return true ;
      }
    }
	}

	return true ;
}

bool
NSEverydayCycleInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

	int iSelectedIndex = CycleSimple->GetSelIndex() ;
  if (iSelectedIndex < 0)
		return false ;

	if (NULL == (*CycleSimple)[iSelectedIndex])
		return false ;

	NSPatPathoArray* pPPT = (*CycleSimple)[iSelectedIndex]->getElement() ;
  if (NULL == pPPT)
		return false ;

	_PPT = *pPPT ;

	return true ;
}

void
NSEverydayCycleInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 1; i++)
  	_parent->enableControl(CyclSimples[i], bVisible) ;
}

void
NSEverydayCycleInterfaceBlock::moveBlock()
{         // 4, 88, 408, 58
	//               QTTY, TIME, FREQ

	int xValues[]  = {  8 };
  int yValues[]  = { 35 };
  int wValues[]  = { 78 };
  int hValues[]  = { 55 };

	for (int i = 0; i < 11; i++)
		_parent->moveControl(idcPosoLibres[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSEverydayCycleInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

NSPatPathoArray*
NSEverydayCycleInterfaceBlock::createRegularRythme(std::string dure_cure, std::string symcure, std::string duree_cycle, std::string SymCycle)
{
	NSPatPathoArray* result = new NSPatPathoArray(pContexte) ;

  result->ajoutePatho("KRYRE1", 0, 1) ;

  Message CodeMsg  ;

  result->ajoutePatho("KDURA1", 1, 1) ;    // duree de la cure premier edit
	CodeMsg.SetUnit(symcure) ;
	CodeMsg.SetComplement(dure_cure) ;
	result->ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;

	result->ajoutePatho("KDURC1", 1, 1) ;     //duree du cycle
  CodeMsg.Reset() ;
	CodeMsg.SetUnit(SymCycle) ;
	CodeMsg.SetComplement(duree_cycle) ;
	result->ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;

  return result ;
}

//
// -------------------- Class NSDaysOfWeekCycleInterfaceBlock ----------------
//

NSDaysOfWeekCycleInterfaceBlock::NSDaysOfWeekCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "monday") ;
	aControls.push_back(new NSCheckBoxControle(_parent, pContexte, "KJLUN1", _parent->getIdBase() + PMG_RADIO_MONDAY,  sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "tuesday") ;
  aControls.push_back(new NSCheckBoxControle(_parent, pContexte, "KJMAR1", _parent->getIdBase() + PMG_RADIO_TUESDAY, sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "wednesday") ;
  aControls.push_back(new NSCheckBoxControle(_parent, pContexte, "KJMER1", _parent->getIdBase() + PMG_RADIO_WEDNDAY, sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "thursday") ;
  aControls.push_back(new NSCheckBoxControle(_parent, pContexte, "KJLEU1", _parent->getIdBase() + PMG_RADIO_THURDAY, sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "friday") ;
  aControls.push_back(new NSCheckBoxControle(_parent, pContexte, "KJVEN1", _parent->getIdBase() + PMG_RADIO_FRIDAY,  sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "saturday") ;
  aControls.push_back(new NSCheckBoxControle(_parent, pContexte, "KJSAM1", _parent->getIdBase() + PMG_RADIO_SATUDAY, sMsgTxt.c_str(), 0, 0, 0, 0)) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "sunday") ;
  aControls.push_back(new NSCheckBoxControle(_parent, pContexte, "KJDIM1", _parent->getIdBase() + PMG_RADIO_SUNDAY,  sMsgTxt.c_str(), 0, 0, 0, 0)) ;
}

NSDaysOfWeekCycleInterfaceBlock::~NSDaysOfWeekCycleInterfaceBlock()
{
}

void
NSDaysOfWeekCycleInterfaceBlock::setupWindow()
{
	if (aControls.empty())
		return ;

	vector<NSCheckBoxControle*>::iterator itCtrls = aControls.begin() ;
	for ( ; itCtrls != aControls.end() ; itCtrls++)
  	(*itCtrls)->setupWindow() ;
}

bool
NSDaysOfWeekCycleInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	if (aControls.empty())
		return false ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  while (pptIter != pptEnd)
  {
    string temp = (*pptIter)->getLexiqueSens(pContexte) ;

    vector<NSCheckBoxControle*>::iterator itCtrls = aControls.begin() ;

    for ( ; (itCtrls != aControls.end()) && ((*itCtrls)->getRootSens() != temp) ; itCtrls++) ;
    if (itCtrls == aControls.end())
    	return false ;

    if (pPpt == &_PPT)
    	(*itCtrls)->setActiveStatus(BF_CHECKED) ;
	}

	return true ;
}

bool
NSDaysOfWeekCycleInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

  if (aControls.empty())
		return true ;

	vector<NSCheckBoxControle*>::iterator itCtrls = aControls.begin() ;
	for ( ; itCtrls != aControls.end() ; itCtrls++)
		if (BF_CHECKED == (*itCtrls)->getValue())
    	_PPT.ajoutePatho((*itCtrls)->getRoot(), 0, 1) ;

	return true ;
}

void
NSDaysOfWeekCycleInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 7; i++)
  	_parent->enableControl(CycleJours[i], bVisible) ;
}

void
NSDaysOfWeekCycleInterfaceBlock::moveBlock()
{         // 12, 162, 328, 44
	int xValues[]  = {  2,  44,  86, 139, 175, 224, 273 };
  int yValues[]  = { 26,  26,  26,  26,  26,  26,  26 };
  int wValues[]  = { 43,  34,  45,  38,  47,  44,  48 };
  int hValues[]  = { 12,  12,  12,  12,  12,  12,  12 };

	for (int i = 0; i < 7; i++)
		_parent->moveControl(CycleJours[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSDaysOfWeekCycleInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

//
// -------------------- Class NSD1D2D3CycleInterfaceBlock ----------------
//

NSD1D2D3CycleInterfaceBlock::NSD1D2D3CycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "dayNumber") ;
  _numJourLabel    = new TStatic(_parent, _parent->getIdBase() + PMG_STATIC_D1D2D3, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  _numJour         = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_D1D2D3, _parent->getIdBase() + PMG_UPDN_D1D2D3, "", 0, 0, 0, 0) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "take") ;
  pJour1           = new NSRadioButtonControle(_parent, pContexte, "2DAT01", _parent->getIdBase() + PMG_RADIO_D1D2_Y, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "noTake") ;
  pJour2           = new NSRadioButtonControle(_parent, pContexte, "2DAT01", _parent->getIdBase() + PMG_RADIO_D1D2_N, sMsgTxt.c_str(), 0, 0, 0, 0) ;
}

NSD1D2D3CycleInterfaceBlock::~NSD1D2D3CycleInterfaceBlock()
{
	delete _numJourLabel ;
  delete _numJour ;
  delete pJour1 ;
  delete pJour2 ;
}

void
NSD1D2D3CycleInterfaceBlock::setupWindow()
{
	_numJour->setupWindow() ;
	pJour1->setupWindow() ;
	pJour2->setupWindow() ;
}

bool
NSD1D2D3CycleInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  while (pptIter != pptEnd)
  {
    string temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if (temp == std::string("2DAT0"))
    {
    	Avance(pptIter, pptEnd) ;
      if ((pptIter == pptEnd) || (std::string("VNUMT") != (*pptIter)->getLexiqueSens(pContexte)))
      	return false ;

      Avance(pptIter, pptEnd) ;
      if (pptIter == pptEnd)
      	return false ;

      string sNumDay = (*pptIter)->getComplement() ;
      if (pPpt == &_PPT)
      	_numJour->setInit(sNumDay) ;

      Avance(pptIter, pptEnd) ;
    }
    else if (temp == std::string("9VOID1"))
    {
    	Avance(pptIter, pptEnd) ;

    	if (pPpt == &_PPT)
      	pJour2->setActiveStatus(BF_CHECKED) ;
    }
	}

  if (BF_CHECKED != pJour2->getActiveStatus())
  	pJour1->setActiveStatus(BF_CHECKED) ;

	return true ;
}

bool
NSD1D2D3CycleInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

  std::string temp = _numJour->getText() ;
  if (temp == std::string(""))
		return false ;

	int iIndice = atoi((char*) temp.c_str()) ;
  _PPT.ajoutePatho("2DAT01", 0, 1) ;
  if (iIndice > 0)
  {
		_PPT.ajoutePatho("VNUMT1", 1, 1) ;

		Message CodeMsg  ;
		CodeMsg.SetUnit("200001") ;
		CodeMsg.SetComplement(IntToString(iIndice)) ;
		_PPT.ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;
  }

  bool bVoid = (bool) pJour2->getValue() ;
  if (bVoid)
  	_PPT.ajoutePatho("9VOID1", 0, 1) ;

	return true ;
}

void
NSD1D2D3CycleInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 5; i++)
  	_parent->enableControl(CycleJourAlts[i], bVisible) ;
}

void
NSD1D2D3CycleInterfaceBlock::moveBlock()
{         // 4, 88, 408, 58
	int xValues[]  = {   8,  11,  46,  76, 176 };
  int yValues[]  = {  26,  34,  34,  35,  35 };
  int wValues[]  = { 124,  37,  11, 100, 100 };
  int hValues[]  = {  28,  15,  15,  12,  12 };

	for (int i = 0; i < 5; i++)
		_parent->moveControl(CycleJourAlts[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSD1D2D3CycleInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

//
// -------------------- Class NSRegularCycleInterfaceBlock ----------------
//

NSRegularCycleInterfaceBlock::NSRegularCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  char *champpLexiqCodesCB[] = {"2HEUR1", "2DAT01", "2DAT11", "2DAT21"} ;  // heure  jour semaine  mois
  VecteurString LexiqCodesCB(champpLexiqCodesCB) ;
  delete[] (champpLexiqCodesCB) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "periodOfTake") ;
  _pGroupCure      = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_REGCUR_Q, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  pDureeCure       = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_REGCUR_Q, _parent->getIdBase() + PMG_UPDN_REGCUR_Q, "", 0, 0, 0, 0) ;
  psymDureeCure    = new NSComboBox(_parent, _parent->getIdBase() + PMG_COMBO_REGCURQ, pContexte, 0, 0, 0, 0, CBS_DROPDOWNLIST, &LexiqCodesCB) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "every") ;
  _pStaticCureEvery = new TStatic(_parent, _parent->getIdBase() + PMG_STATIC_REGCUR, sMsgTxt.c_str(), 0, 0, 0, 0) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "cycleLength") ;
  _pGroupRCycle    = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_REGCUR_F, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  pDureeCycleR     = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_REGCUR_F, _parent->getIdBase() + PMG_UPDN_REGCUR_F, "", 0, 0, 0, 0) ;
  psymDureeCycleR  = new NSComboBox(_parent, _parent->getIdBase() + PMG_COMBO_REGCURF, pContexte, 0, 0, 0, 0, CBS_DROPDOWNLIST, &LexiqCodesCB) ;

	_sUnitCure = string("") ;
	_sUnitCycle = string("") ;
}

NSRegularCycleInterfaceBlock::~NSRegularCycleInterfaceBlock()
{
	delete _pGroupCure ;
  delete pDureeCure ;
  delete psymDureeCure ;
  delete _pStaticCureEvery ;
  delete _pGroupRCycle ;
  delete pDureeCycleR ;
  delete psymDureeCycleR ;
}

void
NSRegularCycleInterfaceBlock::setupWindow()
{
	pDureeCure->setupWindow() ;
	psymDureeCure->setCode(_sUnitCure) ;

	pDureeCycleR->setupWindow() ;
	psymDureeCycleR->setCode(_sUnitCycle) ;
}

bool
NSRegularCycleInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

  if (std::string("KRYRE") != temp)
  	return false ;

	Avance(pptIter, pptEnd) ;

  while (pptIter != pptEnd)
  {
    temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if (std::string("KDURC") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sCodeFreq = (*pptIter)->getUnit() ;
        if (pPpt == &_PPT)
  				_sUnitCycle = sCodeFreq ;

        string sFreq     = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				pDureeCycleR->setInit(sFreq) ;

        Avance(pptIter, pptEnd) ;
      }
    }
    else if (std::string("KDURA") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sCodeFreq = (*pptIter)->getUnit() ;
        if (pPpt == &_PPT)
  				_sUnitCure = sCodeFreq ;

        string sFreq     = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				pDureeCure->setInit(sFreq) ;

        Avance(pptIter, pptEnd) ;
      }
    }
    else
    	return false ;
	}

	return true ;
}

bool
NSRegularCycleInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

	_PPT.ajoutePatho("KRYRE1", 0, 1) ;

	Message CodeMsg  ;

  _PPT.ajoutePatho("KDURA1", 1, 1) ;

	CodeMsg.SetUnit(psymDureeCure->getSelCode()) ;
	CodeMsg.SetComplement(pDureeCure->getText()) ;
	_PPT.ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;

  _PPT.ajoutePatho("KDURC1", 1, 1) ;    // duree de la cure premier edit

	CodeMsg.Reset() ;
	CodeMsg.SetUnit(psymDureeCycleR->getSelCode()) ;
	CodeMsg.SetComplement(pDureeCycleR->getText()) ;
	_PPT.ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;

	return true ;
}

void
NSRegularCycleInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 9; i++)
  	_parent->enableControl(RegularCycles[i], bVisible) ;
}

void
NSRegularCycleInterfaceBlock::moveBlock()
{         // 4, 88, 408, 58

                     // QTY - FREQ
	int xValues[]  = {   8, 170,
                      11, 175,
                      46, 211,
                      61, 224 };
  int yValues[]  = {  26,  26,
                      35,  35,
                      35,  35,
                      36,  35 };
  int wValues[]  = { 124, 174,
                      37,  37,
                      11,  11,
                      68,  68 };
  int hValues[]  = {  28,  28,
                      15,  15,
                      15,  15,
                      55,  55 };

	for (int i = 0; i < 9; i++)
		_parent->moveControl(RegularCycles[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSRegularCycleInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

//
// -------------------- Class NSFreeCycleInterfaceBlock ----------------
//

NSFreeCycleInterfaceBlock::NSFreeCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                          :NSPosoInterfaceBlock(parent, pCtx, pPPToInit)
{
	string sMsgTxt ;

  char *champpLexiqCodesCB[] = {"2HEUR1", "2DAT01", "2DAT11", "2DAT21"} ;  // heure  jour semaine  mois
  VecteurString LexiqCodesCB(champpLexiqCodesCB) ;
  delete[] (champpLexiqCodesCB) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "periodOfTake") ;
  _pGroupFCure     = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_FRECUR_Q, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  pDureeCureF      = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_FRECUR_Q, _parent->getIdBase() + PMG_UPDN_FRECUR_Q, "", 0, 0, 0, 0) ;
  psymDureeCureF   = new NSComboBox(_parent, _parent->getIdBase() + PMG_COMBO_FRECURQ, pContexte, 0, 0, 0, 0, CBS_DROPDOWNLIST, &LexiqCodesCB) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "numberOfTakePeriods") ;
  _pGroupFNbre     = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_FRECUR_N, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  pDureeCureFTime  = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_FRECUR_N, _parent->getIdBase() + PMG_UPDN_FRECUR_N, "", 0, 0, 0, 0) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "every") ;
  _pStaticFCureEvery = new TStatic(_parent, _parent->getIdBase() + PMG_STATIC_FRECUR, sMsgTxt.c_str(), 0, 0, 0, 0) ;

  sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "numberOfTakePeriods") ;
  _pGroupFNbre     = new TGroupBox(_parent, _parent->getIdBase() + PMG_GRP_FRECUR_F, sMsgTxt.c_str(), 0, 0, 0, 0) ;
  pDureeCycleFreqF = new NSUpDownEdit(_parent, pContexte, "", _parent->getIdBase() + PMG_EDIT_FRECUR_F, _parent->getIdBase() + PMG_UPDN_FRECUR_F, "", 0, 0, 0, 0) ;
  psymDureeCycleFreqF = new NSComboBox(_parent, _parent->getIdBase() + PMG_COMBO_FRECURF, pContexte, 0, 0, 0, 0, CBS_DROPDOWNLIST, &LexiqCodesCB) ;

  _sUnitCure = string("") ;
	_sUnitCycle = string("") ;
}

NSFreeCycleInterfaceBlock::~NSFreeCycleInterfaceBlock()
{
	delete _pGroupFCure ;
  delete pDureeCureF ;
  delete psymDureeCureF ;
  delete _pGroupFNbre ;
  delete pDureeCureFTime ;
  delete _pStaticFCureEvery ;
  delete _pGroupFNbre ;
  delete pDureeCycleFreqF ;
  delete psymDureeCycleFreqF ;
}

void
NSFreeCycleInterfaceBlock::setupWindow()
{
	pDureeCureF->setupWindow() ;
	psymDureeCureF->setCode(_sUnitCure) ;

  pDureeCureFTime->setupWindow() ;

	pDureeCycleFreqF->setupWindow() ;
	psymDureeCycleFreqF->setCode(_sUnitCycle) ;
}

bool
NSFreeCycleInterfaceBlock::parsePpt(NSPatPathoArray *pPpt)
{
	if (NULL == pPpt)
		return false ;

	if (pPpt->empty())
		return true ;

	PatPathoIter pptIter = pPpt->begin() ;
  PatPathoIter pptEnd  = pPpt->end() ;

  std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

  if (std::string("KRYRE") != temp)
  	return false ;

	Avance(pptIter, pptEnd) ;

  while (pptIter != pptEnd)
  {
    temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if (std::string("KDURC") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sCodeFreq = (*pptIter)->getUnit() ;
        if (pPpt == &_PPT)
  				_sUnitCycle = sCodeFreq ;

        string sFreq     = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				pDureeCycleFreqF->setInit(sFreq) ;

        Avance(pptIter, pptEnd) ;
      }
    }
    else if (std::string("KDURA") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
      	string sCodeFreq = (*pptIter)->getUnit() ;
        if (pPpt == &_PPT)
  				_sUnitCure = sCodeFreq ;

        string sFreq     = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				pDureeCureF->setInit(sFreq) ;

        Avance(pptIter, pptEnd) ;
      }
    }
    else if (std::string("VAINC") == temp)
    {
    	Avance(pptIter, pptEnd) ;
      if (pptIter != pptEnd)
      {
        string sFreq     = (*pptIter)->getComplement() ;
        if (pPpt == &_PPT)
  				pDureeCureFTime->setInit(sFreq) ;

        Avance(pptIter, pptEnd) ;
      }
    }
    else
    	return false ;
	}

	return true ;
}

bool
NSFreeCycleInterfaceBlock::freshenPatPatho()
{
	_PPT.vider() ;

	_PPT.ajoutePatho("KRYLI1", 0, 1) ;

	Message CodeMsg  ;

  _PPT.ajoutePatho("KDURA1", 1, 1) ;

	CodeMsg.SetUnit(psymDureeCureF->getSelCode()) ;
	CodeMsg.SetComplement(pDureeCureF->getText()) ;
	_PPT.ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;

  _PPT.ajoutePatho("KDURC1", 1, 1) ;    // duree de la cure premier edit

	CodeMsg.Reset() ;
	CodeMsg.SetUnit(psymDureeCycleFreqF->getSelCode()) ;
	CodeMsg.SetComplement(pDureeCycleFreqF->getText()) ;
	_PPT.ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;

  _PPT.ajoutePatho("VAINC1", 1, 1) ;

	CodeMsg.SetUnit("2FOIS1") ;
	CodeMsg.SetComplement(pDureeCureFTime->getText()) ;
	_PPT.ajoutePatho("£N0;03", &CodeMsg, 2, 1) ;

	return true ;
}

void
NSFreeCycleInterfaceBlock::display(bool bVisible)
{
	for (int i = 0; i < 12; i++)
  	_parent->enableControl(FreeCycles[i], bVisible) ;
}

void
NSFreeCycleInterfaceBlock::moveBlock()
{       
                     // QTY - TIME - FREQ
	int xValues[]  = {   8, 137, 274,
                      12, 141, 278,
                      49, 183, 313,
                      65, 205, 328 };
  int yValues[]  = {  26,  26,  26,
                      35,  35,  35,
                      35,  35,  35,
                      35,  37,  35 };
  int wValues[]  = { 124, 180, 124,
                      40,  44,  36,
                      11,  11,  11,
                      63,  76,  60 };
  int hValues[]  = {  28,  28,  28,
                      15,  15,  15,
                      15,  15,  15,
                      55,   8,  55 };

	for (int i = 0; i < 12; i++)
		_parent->moveControl(FreeCycles[i], xValues[i], yValues[i], wValues[i], hValues[i]) ;
}

void
NSFreeCycleInterfaceBlock::initPosition(NS_CLASSLIB::TRect theRect)
{
}

// -----------------------------------------------------------------------------
//
// classe NSPhaseManagerGroup
// classe définissant une phase
//
// -----------------------------------------------------------------------------

/*
** Teste si les noeuds sont auhtorizés comme fils d'un KPATH
*/
bool TestIfPathoOkForPhase(std::string &temp)
{
  return ((std::string("VDURE") == temp )
        || (std::string("VRENO") == temp )
        || (std::string("KOUVR") == temp )
        || (std::string("KFERM") == temp )
        || (std::string("KCYTR") == temp )) ;
}

NSPhaseManagerGroup::NSPhaseManagerGroup(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, int Id, const char far *text, int x, int y, int w, int h, TModule* module)
                    :NSRoot(pCtx), TGroupBox(parent, Id, text, x, y, w, h, module), _PPT(pCtx)
{
	if (NULL != pPPToInit)
		_PPT = *pPPToInit ;

  _iIdBase = Id ;

  createControls() ;
}

NSPhaseManagerGroup::NSPhaseManagerGroup(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, int resourceId, int iBaseId, TModule* module)
                    :NSRoot(pCtx), TGroupBox(parent, resourceId, module), _PPT(pCtx)
{
	if (NULL != pPPToInit)
		_PPT = *pPPToInit ;

  _iIdBase  = iBaseId ;

  createControls() ;
}

NSPhaseManagerGroup::~NSPhaseManagerGroup()
{
	delete pDureePhaseTxt ;
	delete pDureePhase ;
  delete pCBDureePhase ;
	delete pRenouvellementTxt ;
	delete pRenouvellement ;
	delete pRenouvellementTimeTxt ;
	delete pDateDebPrescrTxt ;
	delete pDateDebPrescr ;
	delete pDateFinPrescrTxt ;
	delete pDateFinPrescr ;
	delete pPosologieGroup ;
}

void
NSPhaseManagerGroup::createControls()
{
	pDureePhaseTxt         = new OWL::TStatic(this, _iIdBase + 1, "", 0, 0, 0, 0) ;
	pDureePhase            = new NSUpDownEdit(this, pContexte, "", _iIdBase + 2, _iIdBase + 3, "", 0, 0, 0, 0) ;
  pDureePhase->getEditNum()->SetLostFocusResponse(new MemFunctor<NSPhaseManagerGroup>( (NSPhaseManagerGroup*)this, &NSPhaseManagerGroup::actualisePhase )) ;
  pDureePhase->getUpDown()->SetLostFocusResponse(new MemFunctor<NSPhaseManagerGroup>( (NSPhaseManagerGroup*)this, &NSPhaseManagerGroup::actualisePhase )) ;

  char *temp[] = {"2HEUR1","2DAT01","2DAT11","2DAT21"} ;
  pCBDureePhase          = new NSComboBox(this, _iIdBase + 4, pContexte, 0, 0, 0, 0, CBS_DROPDOWNLIST, temp) ;
  pCBDureePhase->SetLostFocusResponse(new MemFunctor<NSPhaseManagerGroup>( (NSPhaseManagerGroup*)this, &NSPhaseManagerGroup::actualisePhase ));
	pRenouvellementTxt     = new OWL::TStatic(this, _iIdBase + 5, "", 0, 0, 0, 0) ;
	pRenouvellement        = new NSUpDownEdit(this, pContexte, "", _iIdBase + 6, _iIdBase + 7, "", 0, 0, 0, 0) ;

	pRenouvellementTimeTxt = new OWL::TStatic(this, _iIdBase + 8, "", 0, 0, 0, 0) ;

  NSUtilDialog* pParentDlg = TYPESAFE_DOWNCAST(Parent, NSUtilDialog) ;
	pDateDebPrescrTxt      = new OWL::TStatic(this, _iIdBase + 9, "", 0, 0, 0, 0) ;
	pDateDebPrescr         = new NSUtilEditDateHeure(pParentDlg, _iIdBase + 10, "", 0, 0, 0, 0) ;
	pDateFinPrescrTxt      = new OWL::TStatic(this, _iIdBase + 11, "", 0, 0, 0, 0) ;
	pDateFinPrescr         = new NSUtilEditDateHeure(pParentDlg, _iIdBase + 12, "", 0, 0, 0, 0) ;

	pPosologieGroup = 0 ;

  parsePpt() ;

  if (_aPosos.empty())
	{
  	int iNextId = getLastPosoId() + PMG_BLOCKS_WIDTH ;

		NSPosoManagerGroup* pNewPoso = new NSPosoManagerGroup(this, pContexte, NULL, iNextId, "", 0, 0, 0, 0) ;
		_aPosos.push_back(pNewPoso) ;
	}
}

void
NSPhaseManagerGroup::initInterfaceElements()
{
	string sTxt ;

	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "prescription") ;
	SetCaption(sTxt.c_str()) ;

	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "during") ;
	pDureePhaseTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "mayBeRenewed") ;
	pRenouvellementTxt->SetText(sTxt.c_str()) ;
  sTxt = pContexte->getSuperviseur()->getText("drugDialog", "time") ;
	pRenouvellementTimeTxt->SetText(sTxt.c_str()) ;

	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "from") ;
	pDateDebPrescrTxt->SetText(sTxt.c_str()) ;
	sTxt = pContexte->getSuperviseur()->getText("drugDialog", "to") ;
	pDateFinPrescrTxt->SetText(sTxt.c_str()) ;
}

void
NSPhaseManagerGroup::moveBlock()
{
	int xValues[]  = {   4,  33,  64, 112, 156, 186,   4,  24, 108, 124 };
  int yValues[]  = {  16,  13,  13,  16,  13,  16,  34,  32,  34,  32 };
  int wValues[]  = {  28,  19,  45,  44,  15,  14,  16,  80,  12,  80 };
  int hValues[]  = {   8,  14,  50,   8,  14,   8,   8,  12,   8,  12 };

	TDialog* pParentDlg = TYPESAFE_DOWNCAST(Parent, TDialog) ;
	if (NULL == pParentDlg)
		return ;

	for (int i = 0; i < 10; i++)
	{
		HWND hwndCtrl = pParentDlg->GetDlgItem(_iIdBase + 1 + i) ;
  	if (NULL != hwndCtrl)
    {
			NS_CLASSLIB::TRect recGen(xValues[i], yValues[i], xValues[i] + wValues[i], yValues[i] + hValues[i]) ;
  		pParentDlg->MapDialogRect(recGen) ;
			::MoveWindow(hwndCtrl, recGen.left , recGen.top, recGen.Width(), recGen.Height(), true) ;
		}
	}
}

void
NSPhaseManagerGroup::SetupWindow()
{
	TGroupBox::SetupWindow() ;

	initInterfaceElements() ;
  moveBlock() ;
}

void
NSPhaseManagerGroup::CmOk()
{
}

void
NSPhaseManagerGroup::CmCancel()
{
}

bool
NSPhaseManagerGroup::parsePpt()
{
	if (_PPT.empty())
		return true ;

	PatPathoIter pptIter = _PPT.begin() ;
  PatPathoIter pptEnd  = _PPT.end() ;

  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if (TestIfPathoOkForPhase(temp) == false)
      if (recupereData(pptIter, pptEnd) == false )
        return false ;

    if ((temp == "KOUVR") && (loadDate(pptIter, pptEnd, &_tStartingDate) == false))
      if (recupereData(pptIter, pptEnd) == false)
        return false ;

		if ((temp == "KFERM") && (loadDate(pptIter, pptEnd, &_tClosingDate) == false))
      if (recupereData(pptIter, pptEnd) == false)
        return false ;

    if ((temp == "VDURE") && (loadDuree(pptIter, pptEnd) == false))
      if (recupereData(pptIter, pptEnd) == false)
        return false ;

    if ((temp == "VRENO") && (loadRenouvellement(pptIter, pptEnd) == false))
    	if (recupereData(pptIter, pptEnd) == false)
        return false ;

    if (temp == "KCYTR")
    {
    	NSPatPathoArray patpathoPoso(pContexte) ;
    	patpathoPoso.ExtrairePatPatho(pptIter, &_PPT) ;

      int iNextId = getLastPosoId() + PMG_BLOCKS_WIDTH ;

      NSPosoManagerGroup* pNewPoso = new NSPosoManagerGroup(this, pContexte, &patpathoPoso, iNextId, "", 0, 0, 0, 0) ;
      _aPosos.push_back(pNewPoso) ;

      int iCol = (*pptIter)->getColonne() ;
      Avance(pptIter, pptEnd) ;
      while ((pptIter != pptEnd) && ((*pptIter)->getColonne() > iCol))
      	Avance(pptIter, pptEnd) ;
    }
  }
  return true ;
}

bool
NSPhaseManagerGroup::loadDate(PatPathoIter& pptIter, PatPathoIter& pptIEnd, NVLdVTemps* pDate)
{
	if (pptIter == pptIEnd)
		return false ;

	pptIter++ ;

  if (pptIter == pptIEnd)
		return false ;

	string sFormat = (*pptIter)->getLexiqueSens(pContexte) ;
	string sValeur = (*pptIter)->getComplement() ;
	string sUnite  = (*pptIter)->getUnitSens(pContexte) ;

	if (string("") == sValeur)
		return false ;

  if (string("") == sFormat)
		return false ;
	if (('D' != sFormat[1]) && ('T' != sFormat[1]))
		return false ;

	if (("2DA01" != sUnite) && ("2DA02" != sUnite))
		return false ;

  pDate->initFromDate(sValeur) ;

  Avance(pptIter, pptIEnd) ;
  return true ;
}

bool
NSPhaseManagerGroup::loadDuree(PatPathoIter& pptIter, PatPathoIter& pptIEnd)
{
	if (pptIter == pptIEnd)
		return false ;

	pptIter++ ;

  if (pptIter == pptIEnd)
		return false ;

  _sUnitDureeCycle = (*pptIter)->getUnit() ;
  _iDureeCycle     = atoi(((*pptIter)->getComplement()).c_str()) ;

  Avance(pptIter, pptIEnd) ;
  return true ;
}

bool
NSPhaseManagerGroup::loadRenouvellement(PatPathoIter& pptIter, PatPathoIter& pptIEnd)
{
	if (pptIter == pptIEnd)
		return false ;

	pptIter++ ;

  if (pptIter == pptIEnd)
		return false ;

	std::string complement = (*pptIter)->getComplement() ;
	_iNbRenouvellement = atoi(complement.c_str()) ;

	Avance(pptIter, pptIEnd) ;
	return true ;
}

bool
NSPhaseManagerGroup::recupereData(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;
    if ((TestIfPathoOkForPhase(temp) == true) && ((*pptIter)->getColonne() == 1))
    {
      // pptIter-- ;
      return true ;
    }
    Avance(pptIter, pptEnd) ;// ++;
  }
  return false ;
}

void
NSPhaseManagerGroup::actualisePhase()
{
	actualiseEndOfPrescription() ;
}

void
NSPhaseManagerGroup::actualiseEndOfPrescription()
{
	int iPhaseDurationValue = pDureePhase->getValue() ;
	if (iPhaseDurationValue <= 0)
		return ;

	string sPhaseDurationUnit = pCBDureePhase->getSelCode() ;
	if (string("") == sPhaseDurationUnit)
		return ;

	std::string date ;
  pDateDebPrescr->getDate(&date) ;
  NVLdVTemps data ;
  data.initFromDate(date) ;

  data.ajouteTemps(iPhaseDurationValue, sPhaseDurationUnit, pContexte) ;

  date = data.donneDateHeure() ;
  pDateFinPrescr->setDate(date) ;
}

void
NSPhaseManagerGroup::display(bool bVisible)
{
	NSComplexMedicamentDlg* pParentDlg = TYPESAFE_DOWNCAST(Parent, NSComplexMedicamentDlg) ;
	if (NULL == pParentDlg)
		return ;

	for (int i = 0; i < 10; i++)
  	pParentDlg->enableControl(_iIdBase + 1 + i, bVisible) ;

	if (_aPosos.empty())
		return ;

	NSPosoManagerGroupIter posoIter = _aPosos.begin() ;
  for (; posoIter != _aPosos.end() ; posoIter++)
  	(*posoIter)->display(bVisible) ;
}

int
NSPhaseManagerGroup::getLastPosoId()
{
	if (_aPosos.empty())
		return 0 ;

	int iPosoId = 0 ;

	NSPosoManagerGroupIter posoIter = _aPosos.begin() ;
  for (; posoIter != _aPosos.end() ; posoIter++)
  	if ((*posoIter)->getIdBase() > iPosoId)
    	iPosoId = (*posoIter)->getIdBase() ;

	return iPosoId ;
}

/*
** Indique que les code authorize a ce niveau
*/
bool TestIfPathoOkForDialog(std::string& temp)
{
  return ( (std::string("KOUVR") == temp )    // date d'ouverture
        || (std::string("KPHAT") == temp )    // phase
        || (std::string("KEVEI") == temp )    // en cas d'urgence
        || (std::string("KFERM") == temp )    // date de fermeture
        || (std::string("0MEDF") == temp )    //  mode d'emploie
        || (std::string("LSUBS") == temp )    // Non substituable
        || (std::string("LREMB") == temp )
        || (std::string("LDAMI") == temp ));  // Nom remboursable
}

// -----------------------------------------------------------------------------
//
// classe NSComplexMedicamentDlg
// classe définissant la boïte de dialogue de prescription d'un médicament
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSComplexMedicamentDlg, NSUtilDialog)
	EV_COMMAND(IDOK,                       CmOk),
	EV_COMMAND(IDCANCEL,                   CmCancel),
END_RESPONSE_TABLE ;

NSComplexMedicamentDlg::NSComplexMedicamentDlg(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit)
                       :NSUtilDialog(parent, pCtx, "COMPLEXMEDICAMENTDLG", pNSDLLModule), _PPT(pCtx)
{
	if (NULL != pPPToInit)
		_PPT = *pPPToInit ;

	pActivePhase = 0 ;

	createControls() ;
}

NSComplexMedicamentDlg::~NSComplexMedicamentDlg()
{
	delete pTrtGroup ;

	delete pType ;
	delete pUnitePriseTxt ;
	delete pUnitePrise ;

	delete pDateDebTxt ;
	delete pDateDeb ;
  delete pDateFinGroup ;
	delete pRChronique ;
	delete pRDans ;
	delete pRDuree ;
	delete pRLe ;
	delete pNbJours ;
  delete pCBDureeTtt ;
	delete pDateFin ;

  delete pEnCasDeTxt ;
  delete pEnCasDe ;
  delete modedepriseTxt ;
  delete modedeprise ;
  delete substituable ;
  delete remboursable ;

	delete pPrescriptionGroup ;
}

void
NSComplexMedicamentDlg::createControls()
{
	pTrtGroup      = new OWL::TGroupBox(this, TRT_GROUP) ;

	pType          = new NSUtilLexique(this, DRUG_EDIT, pContexte->getSuperviseur()->getDico()) ;
  pType->setLostFocusFunctor(new MemFunctor<NSComplexMedicamentDlg>( (NSComplexMedicamentDlg*)this, &NSComplexMedicamentDlg::executedAfterDrugSelection )) ;

	pUnitePriseTxt = new OWL::TStatic(this, DRG_UNIT_TXT) ;
	pUnitePrise    = new NSUtilLexique(this, DRG_UNIT, pContexte->getSuperviseur()->getDico()) ;

	pDateDebTxt    = new OWL::TStatic(this, DATE_DEBPRESC_TEXT) ;
	pDateDeb       = new NSUtilEditDateHeure(this, DRUG_DATE_DEB) ;
  pDateDeb->setLostFocusFunctor(new MemFunctor<NSComplexMedicamentDlg>( (NSComplexMedicamentDlg*)this, &NSComplexMedicamentDlg::executedAfterTrtBeginDate )) ;
  pDateFinGroup  = new OWL::TGroupBox(this, DATE_FIN_DRG_GROUP) ;
	pRChronique    = new OWL::TRadioButton(this, IDR_DRG_CHRONIQUE) ;
	pRDans         = new OWL::TRadioButton(this, IDR_DRG_DANS) ;
	pRDuree        = new OWL::TRadioButton(this, IDR_DRG_DUREE) ;
	pRLe           = new OWL::TRadioButton(this, IDR_DRG_LE) ;
	pNbJours       = new NSEditNum(this, IDC_DRG_NBJOURS, 10) ;
  pNbJours->setLostFocusFunctor(new MemFunctor<NSComplexMedicamentDlg>( (NSComplexMedicamentDlg*)this, &NSComplexMedicamentDlg::executedAfterTrtEndDate ));
  char *temp1[] = {"2HEUR1","2DAT01","2DAT11","2DAT21"} ;
  pCBDureeTtt    = new NSComboBox(this, IDC_DRG_NBJOURS_TXT, pContexte, temp1) ;
  pCBDureeTtt->SetLostFocusResponse(new MemFunctor<NSComplexMedicamentDlg>( (NSComplexMedicamentDlg*)this, &NSComplexMedicamentDlg::executedAfterTrtEndDate ));
	pDateFin       = new NSUtilEditDateHeure(this, DRUG_DATE_FIN) ;

  pEnCasDeTxt    = new OWL::TStatic(this, IDC_EVENT_TXT) ;
  pEnCasDe       = new NSUtilLexique(this, IDC_EVENT_EDIT, pContexte->getSuperviseur()->getDico()) ;
  modedepriseTxt = new OWL::TStatic(this, IDC_EVENT_TXT) ;
  modedeprise    = new NTComboBox<NSPatPathoArray>(this, IDC_ADVICE_COMBO) ;
  substituable   = new NSCheckBoxControle(this, pContexte, "LSUBS1", IDC_SUBSTIT) ;
  remboursable   = new NSCheckBoxControle(this, pContexte, "LREMB1", IDC_REMBURS) ;

	pPrescriptionGroup = new OWL::TGroupBox(this, PRESCR_GROUP) ;

  parsePpt() ;

  if (_aPhases.empty())
	{
  	int iNextId = getLastPhaseId() + PMG_PHASES_WIDTH ;

		NSPhaseManagerGroup* pNewPhase = new NSPhaseManagerGroup(this, pContexte, NULL, iNextId, "", pPrescriptionGroup->Attr.X, pPrescriptionGroup->Attr.Y, pPrescriptionGroup->Attr.W, pPrescriptionGroup->Attr.H) ;
		_aPhases.push_back(pNewPhase) ;
	}
}

void
NSComplexMedicamentDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  initComboModedePrise() ;         // Initialisation de la combo ( modedeprise)

  std::string Forme  = "" ;
  std::string sLabel = "" ;

  if (sLexiqCode != "")
  {
  	string sLang = pContexte->getUtilisateur()->donneLang() ;
		pContexte->getDico()->donneLibelle(sLang, &sLexiqCode, &sLabel) ;
    pType->setLabel(sLexiqCode.c_str(), sLabel.c_str()) ;
    Forme = initDispUnit(sLang, sLexiqCode, sLabel, pContexte) ;
  }
  else
    pType->SetText("") ;
    
  if (sPriseUnit != "")
    pUnitePrise->setLabel(sPriseUnit) ;
  else
  {
  	string sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "missingInformation") ;
    pUnitePrise->SetText(sMsgTxt.c_str()) ;
  }

  NVLdVTemps sez ;
  sez.takeTime() ;
  std::string sDateOuvertureTemp = sez.donneDateHeure() ;   /* Affichage des donnée par default */
  if (sDateOuverture != "")
		pDateDeb->setDate(sDateOuverture) ;
  else
		pDateDeb->setDate(sDateOuvertureTemp) ;

  if (sDateFermeture != "")
  	pDateFin->setDate(sDateFermeture) ;

  if (sEnCasDe_Code != "")
  	pEnCasDe->setLabel(sEnCasDe_Code) ;

	// Prescription
  //
  pActivePhase = getCurrentPhase() ;
  if (NULL != pActivePhase)
  	displayCurrentPhase() ;
}

void
NSComplexMedicamentDlg::CmOk()
{
	if (checkValidity())
  {
    createTree() ;
    TDialog::CmOk() ;
  }
}

void
NSComplexMedicamentDlg::CmCancel()
{
}

bool
NSComplexMedicamentDlg::parsePpt()
{
	if (_PPT.empty())
		return true ;

	PatPathoIter pptIter = _PPT.begin() ;
  PatPathoIter pptEnd  = _PPT.end() ;
  if (pptIter != pptEnd)
  	sLexiqCode = (*pptIter)->getLexique() ; // Recuperation du nom  du medicament

  Avance(pptIter, pptEnd) ;

  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;
    if (TestIfPathoOkForDialog(temp) == false)
      if ( recupereData(pptIter, pptEnd) == false )
        return false ;

    if (temp == "KOUVR")
    	if (loadDateOuverture(pptIter, pptEnd) == false)
      	return false ;

    if (temp == "KFERM")
    	if (loadDateFermeture(pptIter, pptEnd) == false)
      	return false ;

    if (("0MEDF" == temp) && (loadUniteDePrise(pptIter, pptEnd) == false))
    	if (recupereData(pptIter, pptEnd) == false)
      	return false ;

    if (temp == "KPHAT") // Charge une phase
    {
    	NSPatPathoArray patpathoPhase(pContexte) ;
    	patpathoPhase.ExtrairePatPatho(pptIter, &_PPT) ;

      int iNextId = getLastPhaseId() + PMG_PHASES_WIDTH ;

      NSPhaseManagerGroup* pNewPhase = new NSPhaseManagerGroup(this, pContexte, &patpathoPhase, iNextId, "", pPrescriptionGroup->Attr.X, pPrescriptionGroup->Attr.Y, pPrescriptionGroup->Attr.W, pPrescriptionGroup->Attr.H) ;
      _aPhases.push_back(pNewPhase) ;

      int iCol = (*pptIter)->getColonne() ;
      Avance(pptIter, pptEnd) ;
      while ((pptIter != pptEnd) && ((*pptIter)->getColonne() > iCol))
      	Avance(pptIter, pptEnd) ;
    }

    if (temp == "LADMI")
      loadAdmin(pptIter, pptEnd) ;
  }
  return true;
}

bool
NSComplexMedicamentDlg::checkValidity()
{
	return true ;
}

void
NSComplexMedicamentDlg::createTree()
{
  int iColonne = 0 ;
  std::string sMEdicName = pType->getCode() ;

  _PPT.vider() ;
  _PPT.ajoutePatho(sMEdicName, iColonne++, 0) ;

  // (*pPPT)[0]->setNodeRight("droit pipo");

  //pPPT->ajoutePatho(sLexiqCode, iColonne++, 0) ; // nom du medicament a la racine
  int iMedicRoot  = iColonne ;
  // Create Header
  pDateDeb->getDate(&sDateOuverture) ;
  if (sDateOuverture == "")
  {
        // (initialisation par défaut)
  }
  _PPT.ajoutePatho("KOUVR1", iColonne++, 1) ;
  Message CodeMsg ;
	CodeMsg.SetUnit("2DA021") ;
	CodeMsg.SetComplement(sDateOuverture) ;
	_PPT.ajoutePatho("£D0;19", &CodeMsg, iColonne++, 1) ;
	iColonne = iMedicRoot ;

	pDateFin->getDate(&sDateFermeture) ;
	if (sDateFermeture != "")
	{
  	// insertion de la date d'ouverture au sein de la patpatho
    _PPT.ajoutePatho("KFERM1", iColonne++, 1) ;
    Message CodeMsg2 ;
		CodeMsg2.SetUnit("2DA021") ;
		CodeMsg2.SetComplement(sDateFermeture) ;
		_PPT.ajoutePatho("£D0;19", &CodeMsg2, iColonne++, 1) ;

		iColonne = iMedicRoot ;
	}
	iColonne = iMedicRoot ;

	std::string sEnCas = pEnCasDe->getCode() ;
	if (sEnCas != "")
	{
  	_PPT.ajoutePatho("KEVEI1", iColonne++, 1) ;
    _PPT.ajoutePatho(sEnCas, iColonne++, 1) ;
  }
  iColonne = iMedicRoot ;

	std::string sCode = pUnitePrise->getCode() ;
	if (sCode != "")
  {
  	_PPT.ajoutePatho("0MEDF1", iColonne++, 1) ;
    _PPT.ajoutePatho(sCode, iColonne++, 1) ;
  }
  iColonne = iMedicRoot ;

	if ((remboursable->getValue() == BF_CHECKED) || (substituable->getValue() == BF_CHECKED))
	{
  	_PPT.ajoutePatho("LADMI1", iColonne,1) ;

    if (substituable->getValue() == BF_CHECKED)
    {
    	Message Msg ;
      Msg.SetCertitude("WCE001") ;
      _PPT.ajoutePatho("LSUBS1", &Msg, iColonne +1 , 1) ;
    }
    iColonne = iMedicRoot ;

    if (remboursable->getValue() == BF_CHECKED)
    {
    	Message Msg ;
      Msg.SetCertitude("WCE001") ;
      _PPT.ajoutePatho("LREMB1", &Msg, iColonne + 1, 1) ;
    }
    iColonne = iMedicRoot ;
  }

	// Inserer mode de prise     FIXME
/*
	for (size_t i = 0; i < pPhases->size(); i++)
	{
		NSPatPathoArray* phase_temp = (*pPhases)[i]->CreateTree() ;
    if (phase_temp)
    {
    	pPPT->InserePatPatho(pPPT->end(), phase_temp, iColonne) ;
      delete phase_temp ;
    }
	}
*/
}

bool
NSComplexMedicamentDlg::loadDateOuverture(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
	int iColBaseOuverture = (*pptIter)->getColonne() ;

	Avance(pptIter, pptEnd);

	if (((*pptIter)->getColonne() > iColBaseOuverture)  &&
      (((*pptIter)->getUnitSens(pContexte) == "2DA02") ||
                     ((*pptIter)->getUnitSens(pContexte) == "2DA01")) &&
       ((*pptIter)->getLexiqueSens(pContexte) == "£D0"))
		sDateOuverture = (*pptIter)->getComplement() ;

  if (sDateOuverture == "")
  {
    char pszDateBrut[10] ;
    donne_date_duJour(pszDateBrut) ;

    NVLdVTemps tpsNow ;
  	tpsNow.takeTime() ;
		sDateOuverture = tpsNow.donneDateHeure() ;
  }
  Avance(pptIter, pptEnd) ;
  pDateDeb->setDate(sDateOuverture) ;
  return true ;
}

bool
NSComplexMedicamentDlg::loadDateFermeture(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
	int iColBaseOuverture = (*pptIter)->getColonne() ;

	Avance(pptIter, pptEnd) ;
  if (((*pptIter)->getColonne() > iColBaseOuverture)  &&
      (((*pptIter)->getUnitSens(pContexte) == "2DA02") ||
                    ((*pptIter)->getUnitSens(pContexte) == "2DA01")) &&
          ((*pptIter)->getLexiqueSens(pContexte) == "£D0"))
		sDateFermeture = (*pptIter)->getComplement() ;

  if (sDateFermeture == "")
  {
    NVLdVTemps tpsNow ;
  	tpsNow.takeTime() ;
		sDateFermeture = tpsNow.donneDateHeure() ;
  }
  Avance(pptIter, pptEnd) ;
  pDateFin->setDate(sDateFermeture) ;
  return true ;
}

bool
NSComplexMedicamentDlg::loadUniteDePrise(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
	if (pptIter == pptEnd)
		return true ;

	Avance(pptIter, pptEnd) ;
	if (pptIter != pptEnd)
		sPriseUnit = (*pptIter)->getLexique() ;

	Avance(pptIter, pptEnd) ;
  return true ;
}

bool
NSComplexMedicamentDlg::loadAdmin(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
  std::string temp = "" ;

  int Col;
  if (pptIter != pptEnd)
  {
    Col  = (*pptIter)->getColonne() ;
    temp = (*pptIter)->getLexiqueSens(pContexte) ;
  }

  Avance(pptIter, pptEnd);
  int tempcol = Col;
  while ((pptIter != pptEnd) && (tempcol > Col))
  {
    temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if ((temp != std::string("LSUBS")) &&  (temp != std::string("LREMB")))
      return false ;

    if (temp == std::string("LSUBS"))
    {
      substituable->SetCheck(BF_CHECKED) ;
      Avance(pptIter, pptEnd) ;
    }

    if (temp == std::string ("LREMB"))
    {
      remboursable->SetCheck(BF_CHECKED) ;
      Avance(pptIter, pptEnd) ;
    }

		if (pptIter != pptEnd)
    	tempcol = (*pptIter)->getColonne() ;
  }
  return true ;
}

bool
NSComplexMedicamentDlg::recupereData(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;
    if ((TestIfPathoOkForDialog(temp) == true) && ((*pptIter)->getColonne() == 1))
    {
      // pptIter-- ;
      return true ;
    }
    Avance(pptIter, pptEnd) ;// ++;
  }
  return false ;
}

void
NSComplexMedicamentDlg::initComboModedePrise()
{
  NSPatPathoArray* ava = new NSPatPathoArray(pContexte) ;
  ava->ajoutePatho("KAVAN1", 0, 0) ;
  ava->ajoutePatho("KREPA1", 1, 0) ;

  NSPatPathoArray* pend = new NSPatPathoArray(pContexte) ;
  pend->ajoutePatho("KPEND1",0, 0) ;
  pend->ajoutePatho("KREPA1",1, 0) ;

  NSPatPathoArray* aft = new NSPatPathoArray(pContexte) ;
  aft->ajoutePatho("KAPRE1",0, 0) ;
  aft->ajoutePatho("KREPA1",1, 0) ;

  NSPatPathoArray* nulli = new NSPatPathoArray(pContexte) ;

  std::string val1 = pContexte->getSuperviseur()->getText("drugDialog", "beforeLunch") ;
  std::string val2 = pContexte->getSuperviseur()->getText("drugDialog", "duringLunch") ;
  std::string val3 = pContexte->getSuperviseur()->getText("drugDialog", "afterLunch") ;
  std::string val4 = pContexte->getSuperviseur()->getText("drugDialog", "awayFromLunch") ;
  std::string val5("") ;

  modedeprise->AddElement(val5, nulli) ;
  modedeprise->AddElement(val1, ava) ;
  modedeprise->AddElement(val2, pend) ;
  modedeprise->AddElement(val3, aft) ;
  modedeprise->PrintCombo() ;
  modedeprise->SetSelIndex(0) ;
}

void
NSComplexMedicamentDlg::executedAfterDrugSelection()
{
	string sLexiqCode = pType->getCode() ;

  if (string("") == sLexiqCode)
		return ;

	string sLabel ;
	string sLang = pContexte->getUtilisateur()->donneLang() ;
	pContexte->getDico()->donneLibelle(sLang, &sLexiqCode, &sLabel) ;

	string sMITxt = pContexte->getSuperviseur()->getText("drugDialog", "missingInformation") ;

	string sCodeDisp = initDispUnit(sLang, sLexiqCode, sLabel, pContexte) ;
	if (sCodeDisp != sMITxt) // Verifie que le code exite
		pUnitePrise->setLabel(sCodeDisp) ;
	else
		pUnitePrise->SetText(sCodeDisp.c_str()) ;
}

void
NSComplexMedicamentDlg::executedAfterTrtBeginDate()
{
/*
	std::string dateDeb ;
  pDateDeb->getDate(&dateDeb) ;
  NVLdVTemps data ;
  data.initFromDate(dateDeb) ;

  std::string date ;
  pDateDebPrescr->getDate(&date) ;
  NVLdVTemps dataPrescr ;
  dataPrescr.initFromDate(date) ;

	NVLdVTemps tpsNow ;
	tpsNow.takeTime() ;

  if (data >= tpsNow)
	{
		pDateDebPrescr->setDate(data.donneDateHeure()) ;
    ActualiseEndOfPrescription() ;
	}
*/
}

void
NSComplexMedicamentDlg::executedAfterTrtEndDate()
{
/*
	pNbJours->donneValeur() ;
	int iTrtDurationValue = (int) pNbJours->dValeur ;
	if (iTrtDurationValue <= 0)
		return ;

	string sTrtDurationUnit = pCBDureeTtt->getSelCode() ;
	if (string("") == sTrtDurationUnit)
		return ;

	NVLdVTemps tDateFin ;

	// si on dans le cas où "dans" est cochée
  if (pRDans->GetCheck() == BF_CHECKED)
    tDateFin.takeTime() ;

  // si on est dans le cas où "durée" est cochée
  else if (pRDuree->GetCheck() == BF_CHECKED)
  {
    std::string sDateDeb ;
		pDateDeb->getDate(&sDateDeb) ;

    tDateFin.initFromDate(sDateDeb) ;
  }
  else
  	return ;

	tDateFin.ajouteTemps(iTrtDurationValue, sTrtDurationUnit, pContexte) ;
	string sDateFin = tDateFin.donneDateHeure() ;

  pDateFin->setDate(sDateFin) ;
  pDateFinPrescr->setDate(sDateFin) ;
*/
}

int
NSComplexMedicamentDlg::getLastPhaseId()
{
	if (_aPhases.empty())
		return 0 ;

	int iMaxId = 0 ;

	NSPhaseManagerGroupIter phaseIter = _aPhases.begin() ;
  for ( ;  phaseIter != _aPhases.end(); phaseIter++)
  	if (iMaxId < (*phaseIter)->getPhaseId())
    	iMaxId = (*phaseIter)->getPhaseId() ;

	return iMaxId ;
}

NSPhaseManagerGroup*
NSComplexMedicamentDlg::getCurrentPhase()
{
	if (_aPhases.empty())
		return NULL ;

	NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  // We suppose that phases are sorted by startingDate

	NSPhaseManagerGroupIter phaseIter = _aPhases.begin() ;

  // All phases in the future: return the first one
  if (tpsNow <= (*phaseIter)->getPhaseStartingDate())
  	return *phaseIter ;

	// Try to find a phase including "now"
  for ( ;  phaseIter != _aPhases.end(); phaseIter++)
  	if ((tpsNow >= (*phaseIter)->getPhaseStartingDate()) &&
        (tpsNow <  (*phaseIter)->getPhaseClosingDate()))
    	return *phaseIter ;

	// If we are there, it means that all phases are in the past: return the last one
  return _aPhases.back() ;
}

NSPhaseManagerGroup*
NSComplexMedicamentDlg::getNextPhase(NSPhaseManagerGroup *pCurrentPhase)
{
	if (_aPhases.empty())
		return NULL ;

	NSPhaseManagerGroupIter phaseIter = _aPhases.begin() ;

	if (NULL == pCurrentPhase)
  	return *phaseIter ;

  for ( ;  (phaseIter != _aPhases.end()) && (pCurrentPhase != *phaseIter); phaseIter++)
  	;

	if (phaseIter != _aPhases.end())
	{
		phaseIter++ ;
    if (phaseIter != _aPhases.end())
    	return *phaseIter ;
	}

	return NULL ;
}

NSPhaseManagerGroup*
NSComplexMedicamentDlg::getPrevPhase(NSPhaseManagerGroup *pCurrentPhase)
{
	if (_aPhases.empty())
		return NULL ;

	NSPhaseManagerGroupReverseIter phaseRIter = _aPhases.rbegin() ;

  if (NULL == pCurrentPhase)
  	return *phaseRIter ;

	for ( ;  (phaseRIter != _aPhases.rend()) && (pCurrentPhase != *phaseRIter); phaseRIter++)
  	;

	if (phaseRIter != _aPhases.rend())
	{
		phaseRIter++ ;
    if (phaseRIter != _aPhases.rend())
    	return *phaseRIter ;
	}

	return NULL ;
}

void
NSComplexMedicamentDlg::displayCurrentPhase()
{
	if (_aPhases.empty())
		return ;

	NSPhaseManagerGroupIter phaseIter = _aPhases.begin() ;

  for ( ;  phaseIter != _aPhases.end(); phaseIter++)
  	if (*phaseIter != pActivePhase)
    	(*phaseIter)->hide() ;

	if (NULL != pActivePhase)
		pActivePhase->show() ;
}

void
NSComplexMedicamentDlg::enableControl(int RessourceId, bool visible)
{
  if (visible)
  {
    ::ShowWindow(GetDlgItem(RessourceId),   SW_SHOW);
    ::EnableWindow(GetDlgItem(RessourceId), TRUE );
  }
  else
  {
    ::ShowWindow(GetDlgItem(RessourceId),   SW_HIDE);
    ::EnableWindow(GetDlgItem(RessourceId), FALSE );
  }
}

// -----------------------------------------------------------------------------
//
// classe NSMedicDlg
// classe définissant la boïte de dialogue de prescription d'un médicament
//
// -----------------------------------------------------------------------------

//Tableau de reponse des tab control
int periodeTab[]   = {IDC_GROUPMATIN, IDC_GROUPMIDI , IDC_GROUPSOIR, IDC_MATED ,  IDC_UPDOWN3 ,
                      IDC_MIDED,      IDC_UPDOWN2,    IDC_SOIRED,    IDC_UPDOWN1, IDC_UPDOWN4,
                      IDC_EDSOIR,     IDC_GROUPCOUCHER};

int CycleJour[]    = {IDC_LUNDI,      IDC_MARDI,      IDC_MERCREDI,  IDC_JEUDI,   IDC_VENDREDI,
                      IDC_SAMEDI,     IDC_DIMANCHE};

int CycleJourAlt[] = {IDC_J1, IDC_J2, IDC_J3, IDC_J4, IDC_J5 };

int regularDay[]   = {IDC_CMPSUNREGU, IDC_EDREGPOS,   IDC_UPDOWN12,  IDC_STATICTEXT4,
                      IDC_FREQPOSREG, IDC_EDIT9,      IDC_UPDOWN14,  IDC_COMBREG2POS};

int periodeCycle[] = {IDC_GROUPBOX4,  IDC_GROUPBOX7,  IDC_GROUPBOX10, IDC_GROUPBOX11,
                      IDC_GROUPBOX12, IDC_EDREVEIL,   IDC_UPDOWN13,   IDC_EDMATIN2,
                      IDC_UPDOWN15,   IDC_GROUPBOX13, IDC_EDMIDI2,    IDC_UPDOWN16,
                      IDC_EDGOUTER2,  IDC_UPDOWN17,   IDC_EDSOIR2,    IDC_UPDOWN18,
                      IDC_EDCOUCHER2, IDC_UPDOWN19,   IDC_GROUPBOX14, IDC_EDNUIT,
                      IDC_UPDOWN20};

int hourCycle[]    = {IDC_GROUPBOX15, IDC_EDHOURTAKE, IDC_UPDOWN21,   IDC_GROUPBOX16,
                      IDC_NUMHOURED,  IDC_UPDOWN22,   IDC_BUTTON3,    IDC_LISTVIEW3};

int idcPosoLibre[] = {IDC_GROUPBOX17, IDC_GROUPBOX18, IDC_STATICTEXT5, IDC_UNITEDFOIS, IDC_PSOEDFOIS, IDC_UPDOWN23,
                      IDC_UPDOWN24,   IDC_GROUPBOX19, IDC_EDFREQ3,     IDC_UPDOWN25,   IDC_COMBOFREQ3};

int PosoUnique[]   = {IDC_GRUNIQUE,IDC_UPUNIQUE, IDC_EDUNIQUE };

int TxtLibre[]     = {IDC_TEXTLIBRE};

int ExtendPhases[] = {IDC_LISTPHASE, IDC_ADDPHASE};

int RegularCycle[] = {IDC_CUREED,     IDC_UPDOWN5,  IDC_COMCURE,  IDC_EDCURCYC,   IDC_UPDOWN6,
                      IDC_DURCYCBOX,  IDC_DURCUR,   IDC_TOUTELES, IDC_COMBOBOX2};

int FreeCycle[]    = {IDC_NBCURELIBRE,   IDC_CUREEDLI,  IDC_UPDOWN9,  IDC_STATICECLI, IDC_EDLIBRE2,
                      IDC_UPDOWN10,      IDC_DURCUR2,   IDC_UPDOWN11, IDC_COMCUR2,    IDC_EDITCUR2,
                      IDC_COMIREGCY2};


    //  EV_TCN_KEYDOWN
    //EV_TCN_SELCHANGING
// -----------------------------------------------------------------------------
// table de réponse aux événements de la boite de dialogue de prescription de
// médicaments
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSMedicamentDlg, NSUtilDialog)
	EV_TCN_KEYDOWN(IDC_TABPSOLO,           KeyForCircadien),
	EV_TCN_KEYDOWN(IDC_TABCYCLE,           KeyForRythme),
	EV_COMMAND(IDOK,                       CmOk),
	EV_COMMAND(IDCANCEL,                   CmCancel),
	EV_TCN_SELCHANGE(IDC_TABPSOLO,         TabSelChangePso), // Changement de table de la posologie
	EV_TCN_SELCHANGE(IDC_TABCYCLE,         TabSelChangeCyc), // Changement de table des cycles
	EV_BN_CLICKED(IDC_EXTENDPHASE,         lanceExtended),
	EV_BN_CLICKED(IDC_CHANGECYCLE,         DrawCycleMode),
	EV_BN_CLICKED(IDC_BUTTON3,             AddHour),       // Ajoute une heure de prise de médicament
	EV_BN_CLICKED(IDC_ADDPHASE,            YAddPhase),
	EV_BN_CLICKED(IDC_ADDRYTHM,            AddCRythm),    // Ajoute un rythme
	EV_LBN_SELCHANGE(IDC_LISTVIEW3,        EvListBoxSelChange),   // Change heure
	EV_LBN_SELCHANGE(IDC_LISTPHASE,        EvListBoxSelPhase),    // Change phase
	EV_LBN_SELCHANGE(IDC_LISTVIEWFORCYCLE, EvListBoxSelCycle),
END_RESPONSE_TABLE ;

NSMedicamentDlg::NSMedicamentDlg(TWindow *parent, NSContexte *pCtx, NSPatPathoArray *pPPTinit)
                :NSUtilDialog(parent, pCtx, "MEDICAMENTDLG", pNSDLLModule)
{
  _extendedForm   = false ;
  _extendtedCycle = true ;
  _topPhase       = 188 ;

  tabPoso        = new TTabControl(this,IDC_TABPSOLO) ;     // Control des cycles circadiens
  tabCycle       = new TTabControl(this, IDC_TABCYCLE) ;    // Control des cycles des rythmes
  medicname      = new NSUtilLexique(this,  IDC_MEDOCCHOICE, pContexte->getSuperviseur()->getDico()) ;     // Nom du medicament
  medicname->setLostFocusFunctor(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::ExecutedAfterMedicamentSelection ));

  idc_phase1     = new TStatic(this, IDC_PHASE1) ;
  idc_static3    = new TStatic(this, IDC_STATICTEXT3) ;
  idc_static2    = new TStatic(this, IDC_STATICTEXT2) ;
  AddPhase       = new TButton(this, IDC_ADDPHASE) ;
  _ChangeCyle    = new TButton(this, IDC_CHANGECYCLE) ;

  pUnitePrise    = new NSUtilLexique (this, IDC_COMPRIM,pContexte->getSuperviseur()->getDico()) ;  // unité de la prise
  pUnitePrise->setLostFocusFunctor(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::FullPriseUnit )) ;

  //groupe de bouton de controle
  ok             = new TButton(this, IDOK) ;
  cancel         = new TButton(this, IDCANCEL) ;
  help           = new TButton(this, IDHELP) ;
  ModePhase      = new TButton(this, IDC_EXTENDPHASE) ;

	CycleSimple    = new NTComboBox<NSPatPathoArray>(this,IDC_SIMPLECYCLE) ;

  pDateOuverture = new NSUtilEditDateHeure(this, IDC_EDIT3) ;
  pDateFermeture = new NSUtilEditDateHeure(this, IDC_EDIT4) ;
  pEnCasDe       = new NSUtilLexique(this, IDC_EDIT1, pContexte->getSuperviseur()->getDico()) ;
  substituable   = new NSCheckBoxControle(this, pContexte, "LSUBS1", IDC_CHECKSUB) ;
  remboursable   = new NSCheckBoxControle(this, pContexte, "LREMB1", IDC_REMBOURS) ;

  pEditTextLibre = new NSTexteLibre       (this, pContexte, "£C;020", IDC_TEXTLIBRE) ;

  _PhaseGroup    = new TGroupBox(this, IDC_GROUPPHASE) ;
  _CycleGroup    = new TGroupBox(this, IDC_GROUPCYCLE) ;

  // Temps pour les durée d'une phase
  pDureePhase    = new NSUpDownEdit(this, pContexte, "", IDC_EDPHASEPEND, IDC_UPDOWN7) ;
  pDureePhase->getEditNum()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::ActualisePhasePreview ));
  pDureePhase->getUpDown()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::ActualisePhasePreview ));

  pRenouvellement = new NSUpDownEdit  (this, pContexte,"", IDC_EDRENEW, IDC_UPDOWN8) ;
  pRenouvellement->getEditNum()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::ActualisePhasePreview ));
  pRenouvellement->getUpDown()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::ActualisePhasePreview ));

  pPhases         = new NSphaseMedicArray() ;       // Creation  du tableau contenant les phases
  phaseBox        = new NTTList<NSphaseMedic, NSMedicamentDlg>(this, IDC_LISTPHASE, pPhases) ;  // initialisation de la liste affichant les différente phase
  phaseBox->setKeyUpResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::EvListBoxSelPhase ) );
  phaseBox->setKeyDownResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::EvListBoxSelPhase ) );

  char *champpLexiqCodesCB[] = {"2HEUR1", "2DAT01", "2DAT11", "2DAT21"} ;  // heure  jour semaine  mois
  VecteurString *pLexiqCodesCB = new VecteurString(champpLexiqCodesCB) ;
  delete[] (champpLexiqCodesCB) ;

  char *champpLexiqJour[] = {"2SEC01", "2MINU1", "2HEUR1", "2DAT01", "2FOIS1"} ; // sec min  heure    jour fois
  VecteurString *pLexiqJour = new VecteurString(champpLexiqJour) ;
  delete[] (champpLexiqJour) ;

  // char *champpLexiqJourSansFois[] = {"2SEC01", "2MINU1", "2HEUR1", "2DAT01"} ;   // sec min  heure    jour
  // VecteurString *pLexiqJourSansFois = new VecteurString(champpLexiqJourSansFois) ;
  // delete[] (champpLexiqJourSansFois) ;

  pPriseMatin     = new NSUpDownEdit(this, pContexte,"KMATI1", IDC_MATED, IDC_UPDOWN3) ;
  pPriseMidi      = new NSUpDownEdit(this, pContexte,"KMIDI1", IDC_MIDED, IDC_UPDOWN2) ;
  pPriseSoir      = new NSUpDownEdit(this, pContexte,"KSOIR1", IDC_SOIRED, IDC_UPDOWN1) ;
  pPriseCoucher   = new NSUpDownEdit(this, pContexte,"KCOUC1", IDC_EDSOIR, IDC_UPDOWN4) ;

  pPriseReveil    = new NSUpDownEdit(this, pContexte,"KREVE1", IDC_EDREVEIL, IDC_UPDOWN13) ;
  pPriseMatin2    = new NSUpDownEdit(this, pContexte,"KMATI1", IDC_EDMATIN2, IDC_UPDOWN15) ;
  pPriseMidi2     = new NSUpDownEdit(this, pContexte,"KMIDI1", IDC_EDMIDI2, IDC_UPDOWN16) ;
  pPriseGouter    = new NSUpDownEdit(this, pContexte,"KGOUT1", IDC_EDGOUTER2, IDC_UPDOWN17) ;
  pPriseSoir2     = new NSUpDownEdit(this, pContexte,"KSOIR1", IDC_EDSOIR2, IDC_UPDOWN18) ;
  pPriseCoucher2  = new NSUpDownEdit(this, pContexte,"KCOUC1", IDC_EDCOUCHER2, IDC_UPDOWN19) ;
  pPriseNuit      = new NSUpDownEdit(this, pContexte,"KNOCT3", IDC_EDNUIT, IDC_UPDOWN20) ;

  // Cycle horraire
  pPriseHeure     = new NSUpDownEditHeureControle(this, pContexte, "KHHMM1", IDC_NUMHOURED, IDC_UPDOWN22, IDC_EDHOURTAKE, IDC_UPDOWN21) ;
  pPriseHeure->getEditNum()->getEditNum()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::SaveChangeHour));
  pPriseHeure->getEditNum()->getUpDown()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::SaveChangeHour));

  // durée de la phase
  char *temp[] = {"2HEUR1","2DAT01","2DAT11","2DAT21"} ;
  pCBDureePhase   = new NSComboBox(this, IDC_PHASECOMBO1, pContexte, temp) ;
  pCBDureePhase->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::ActualisePhasePreview ));

	modedeprise     = new NTComboBox<NSPatPathoArray>(this, IDC_COMMODPRISE) ;

  pLundi          = new NSCheckBoxControle(this, pContexte, "KJLUN1", IDC_LUNDI) ;
  pLundi->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pMardi          = new NSCheckBoxControle(this, pContexte, "KJMAR1", IDC_MARDI) ;
  pMardi->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pMercredi       = new NSCheckBoxControle(this, pContexte, "KJMER1", IDC_MERCREDI) ;
  pMercredi->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pJeudi          = new NSCheckBoxControle(this, pContexte, "KJLEU1", IDC_JEUDI) ;
  pJeudi->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pVendredi       = new NSCheckBoxControle(this, pContexte, "KJVEN1", IDC_VENDREDI) ;
  pVendredi->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pSamedi         = new NSCheckBoxControle(this, pContexte, "KJSAM1", IDC_SAMEDI) ;
  pSamedi->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pDimanche       = new NSCheckBoxControle(this, pContexte, "KJDIM1", IDC_DIMANCHE) ;
  pDimanche->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  _numJourLabel   = new TStatic(this, IDC_J3) ;
  _numJour        = new NSUpDownEdit(this, pContexte, "", IDC_J4, IDC_J5) ;  // Cycle regulier

  pJour1          = new NSRadioButtonControle(this, pContexte, "2DAT01", IDC_J1) ;
  pJour1->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pJour2          = new NSRadioButtonControle(this, pContexte, "2DAT01", IDC_J2) ;
  pJour2->setChangeResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  _quantRCycle    = new NSUpDownEdit(this, pContexte,"",IDC_EDREGPOS, IDC_UPDOWN12);  // Cycle regulier
  _freqRCycle     = new NSUpDownEdit(this, pContexte,"",IDC_EDIT9, IDC_UPDOWN14);     // Cycle Rythme regulier
	char *tempre[] = {"2SEC01","2MINU1","2HEUR1"} ;
	VecteurString   *pLexiqJours = new VecteurString(tempre) ;
	delete[] (tempre) ;
  _RCycleComboF    = new NSComboBox(this, IDC_COMBREG2POS, pContexte, pLexiqJours) ;     // Cycle Rythme regulier

  _quantFCycle     = new NSUpDownEdit(this, pContexte,"",IDC_EDFREQ3  ,IDC_UPDOWN25) ;        // Cycle irregulier
  _quantFreqFCycle = new NSUpDownEdit(this, pContexte,"",IDC_UNITEDFOIS, IDC_UPDOWN23) ;     // Cycle irregulier
	_quantFoisCycle  = new NSUpDownEdit(this, pContexte,"", IDC_PSOEDFOIS,  IDC_UPDOWN24) ;      // Cycle irregulier
  _FCycleComboF    = new NSComboBox(this, IDC_COMBOFREQ3, pContexte, pLexiqJour) ;           // Cycle irregulier

  pDureeCure       = new NSUpDownEdit(this, pContexte,"", IDC_CUREED, IDC_UPDOWN5) ;        // rythme régulier
  pDureeCure->getEditNum()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pDureeCure->getUpDown()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  psymDureeCure    = new NSComboBox(this, IDC_COMCURE, pContexte, pLexiqCodesCB);    // rythme régulier
  psymDureeCure->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  pDureeCycleR     = new NSUpDownEdit(this, pContexte,"",IDC_EDCURCYC  ,IDC_UPDOWN6);        // rythme régulier
  pDureeCycleR->getEditNum()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pDureeCycleR->getUpDown()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  psymDureeCycleR  = new NSComboBox(this, IDC_COMBOBOX2, pContexte, pLexiqCodesCB);    // rythme régulie
  psymDureeCycleR->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  pDureeCureF      = new NSUpDownEdit(this, pContexte,"", IDC_EDITCUR2, IDC_UPDOWN11) ;  // rythme libre
  pDureeCureF->getEditNum()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pDureeCureF->getUpDown()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  psymDureeCureF   = new NSComboBox(this, IDC_COMCUR2, pContexte, pLexiqCodesCB);        // rythme libre
  psymDureeCureF->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  pDureeCureFTime  = new NSUpDownEdit(this, pContexte,"", IDC_CUREEDLI, IDC_UPDOWN9) ;    // rythme libre
  pDureeCureFTime->getEditNum()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pDureeCureFTime->getUpDown()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  pDureeCycleFreqF = new NSUpDownEdit(this, pContexte,"", IDC_EDLIBRE2, IDC_UPDOWN10) ;  // rythme libre
  pDureeCycleFreqF->getEditNum()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));
  pDureeCycleFreqF->getUpDown()->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview ));

  psymDureeCycleFreqF = new NSComboBox(this, IDC_COMIREGCY2, pContexte, pLexiqCodesCB) ;    // rythme libre
  psymDureeCycleFreqF->SetLostFocusResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::AcutaliseCyclePreview )) ;

  viewCycle        = new NTTList<NSMedicCycleGlobal, NSMedicamentDlg> (this, IDC_LISTVIEWFORCYCLE) ;
  viewCycle->setKeyUpResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::EvCycleKey ) ) ;
  viewCycle->setKeyDownResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::EvCycleKey ) ) ;

  //prise_hours = new NSVectPatPathoArray();
  ViewPriseHeure = new NTTList<PriseHeure, NSMedicamentDlg>(this, IDC_LISTVIEW3/*,prise_hours*/) ;
  ViewPriseHeure->setKeyUpResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::FuncMoveForHour ) ) ;
  ViewPriseHeure->setKeyDownResponse(new MemFunctor<NSMedicamentDlg>( (NSMedicamentDlg*)this, &NSMedicamentDlg::FuncMoveForHour ) ) ;
  _LoadForChange = false ;
  priseUnique = new  NSUpDownEdit(this, pContexte,"", IDC_EDUNIQUE, IDC_UPUNIQUE) ;  // rythme libre

	// tableau contenant les heures
  sLexiqCode = "" ;
  sDateOuverture = "" ;
  pPPT = pPPTinit ;

  currentPhase    = NULL ;

  if ((NULL != pPPT) && (pPPT->size() > 0)) // Il y a un medicament a charger
  {
    ParseMedicament() ;
    _LoadForChange = true ;
  }

  pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSubSteps, "Initilisation médicament") ;
  // pContexte->getSuperviseur()->ReadConsole();

  rythmIndex = phaseIndex = 'a' ;

  if (!pPhases->empty())    // Il y a des phase provenant des parametres donne au constructeur
    currentPhase = *(pPhases->begin()) ;
}

NSMedicamentDlg::~NSMedicamentDlg()
{
  delete pRenouvellement ;
  delete modedeprise ;
  delete tabPoso ;
  delete _freqRCycle ;
  delete pDateOuverture ;
  delete pDateFermeture ;
  delete pDureePhase ;
  delete ok ;
  delete cancel ;
  delete help ;
  delete _PhaseGroup ;
  delete _CycleGroup ;
  delete idc_phase1 ;
  delete idc_static3 ;
  delete idc_static2 ;
  delete pCBDureePhase ;
  delete _RCycleComboF ;
  delete ModePhase ;
  delete pPriseMatin ;
  delete pPriseMidi ;
  delete pPriseSoir ;
  delete pPriseCoucher ;
  delete pPriseReveil ;
  delete _quantRCycle ;
  delete pPriseMatin2 ;
  delete pPriseMidi2 ;
  delete pPriseGouter ;
  delete CycleSimple ;
  delete pPriseSoir2 ;
  delete pPriseCoucher2 ;
  delete pPriseNuit ;
  delete pDureeCycleR ;
  delete psymDureeCycleR ;
  delete psymDureeCureF ;
  delete pEnCasDe ;
  delete _quantFCycle ;
  delete _ChangeCyle ;
  delete pLundi ;
  delete pMardi ;
  delete pMercredi ;
  delete pJeudi ;
  delete pVendredi ;
  delete pSamedi ;
  delete pDimanche ;
  delete psymDureeCycleFreqF ;
  delete pDureeCycleFreqF ;
  delete pDureeCureFTime ;
  delete pUnitePrise ;
  delete priseUnique ;
  delete pEditTextLibre ;
  delete substituable ;
  delete remboursable ;
  delete ViewPriseHeure ;
  delete viewCycle ;
  delete AddPhase ;
  delete pDureeCureF ;
  delete psymDureeCure ;
  delete pDureeCure ;
  delete _FCycleComboF ;
  delete _quantFoisCycle ;
  delete _quantFreqFCycle ;
  delete pPriseHeure ;
  delete phaseBox ;
  delete _numJourLabel ;
  delete _numJour ;
  delete pJour1 ;
  delete pJour2 ;
}

void
NSMedicamentDlg::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  InitTabPosologie() ;             // Initlaisation du tableau de posologie
  InitComboModedePrise() ;         // Initialisation de la combo ( modedeprise)
  InitTabCycle() ;
  std::string Forme  = "" ;
  std::string sLabel = "" ;

  if (sLexiqCode != "")
  {
  	string sLang = pContexte->getUtilisateur()->donneLang() ;
		pContexte->getDico()->donneLibelle(sLang, &sLexiqCode, &sLabel) ;
    medicname->setLabel(sLexiqCode.c_str(), sLabel.c_str()) ;
    Forme = initDispUnit(sLang, sLexiqCode, sLabel, pContexte) ;
  }
  else
    medicname->SetText("") ;
  if (sPriseUnit != "")
    pUnitePrise->setLabel(sPriseUnit) ;
  else
  {
  	string sMsgTxt = pContexte->getSuperviseur()->getText("drugDialog", "missingInformation") ;
    pUnitePrise->SetText(sMsgTxt.c_str()) ;
  }
  //tabPoso->SetSel(1);
  EnableControl(IDC_LISTPHASE, false) ;
  EnableControl(IDC_ADDPHASE, false) ;
  InitModeSimple() ;
  DrawCycleMode() ;
  RedrawForm(false) ;
  pPriseHeure->reinitControle() ;
  NVLdVTemps sez ;
  sez.takeTime() ;
  std:: string sDateOuvertureTemp = sez.donneDateHeure() ;   /* Affichage des donnée par default */
  if (sDateOuverture != "")
		pDateOuverture->setDate(sDateOuverture) ;
  else
		pDateOuverture->setDate(sDateOuvertureTemp) ;

  if (sDateFermeture != "")
  	pDateFermeture->setDate(sDateFermeture) ;
  if (sEnCasDe_Code != "")
  	pEnCasDe->setLabel(sEnCasDe_Code) ;

	if (pPhases->size() > 0)  // Il existe deja une phase courante provenant de l'initialisation
  	LoadPhaseOnViewAndConfigure() ;
	else
	{                         // Il n'y a pas de phase courante et on la change
  	YAddPhase() ;
    updatePosologie(1) ;
	}                         // On l'ajoute dans la base

	if (pPhases->size() > 1)
  	_Phase = true ;

  ControleWindow() ;
}

void
NSMedicamentDlg::DeletePhase()
{
  currentPhase = NULL ;
}

void
NSMedicamentDlg::KeyForCircadien(TTabKeyDown& nmHdr)
{
	int temp  =   nmHdr.flags ;
	int index = tabPoso->GetSel() ;

	if ((temp == 333) && (index <6))
		index++ ;
	if ((temp == 331) && (index >0))
		index-- ;
	updatePosologie(index) ;
}

void
NSMedicamentDlg::KeyForRythme(TTabKeyDown& nmHdr)
{
	int temp  = nmHdr.flags ;
	int index = tabCycle->GetSel() ;
	if ((temp == 333) && (index <2))
  	index++ ;
  if ((temp == 331) && (index >0))
  	index-- ;
	updateCycle(index) ;
}

void
NSMedicamentDlg::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

bool CompareMedicament(std::string* un, std::string* deu)
{
  char min_un[3] =  {(*un)[un->size()-2],  (*un)[un->size()-1], '\0'};
  char min_deu[3] = { (*deu)[deu->size()-2],  (*deu)[deu->size()-1], '\0'};
  char heur_un[3] =   {(*un)[un->size()-5],  (*un)[un->size()-4],'\0'};
  char heur_deu[3] = { (*deu)[deu->size()-5],  (*deu)[deu->size()-4],'\0'};

  int mimun =  atoi(min_un) ;
  int mindeu=  atoi(min_deu);
  int heureun = atoi(heur_un);
  int heuredeux = atoi(heur_deu);

  bool result;
  if (heureun == heuredeux)
    result =  (mimun >= mindeu) ?  false : true ;
  else
   result =  (heureun > heuredeux)  ? false : true;
  return result;
}

void
NSMedicamentDlg::SaveChangeHour()
{
  int selectIndex = ViewPriseHeure->GetSelIndex() ;
  if (selectIndex < 0)
		return ;

	std::string heure = pPriseHeure->getHeure() ;
	std::string quant = pPriseHeure->getValue() ;
	PriseHeure* temp  = ViewPriseHeure->getDataAt(selectIndex) ;
	temp->setHeure(heure) ;
	temp->setQuantity(quant) ;
	std::string toAff = CreateLabelForHour(quant,heure ) ;
	ViewPriseHeure->ChangeLabel(toAff, selectIndex) ;
	ViewPriseHeure->sortLabel(CompareMedicament) ;
}

void
NSMedicamentDlg::FuncMoveForHour()
{
  int index = ViewPriseHeure->GetSelIndex();
  if (index  >=0)
  {
  }
}

void
NSMedicamentDlg::InitModeSimple()
{
  std::string temp1 = pContexte->getSuperviseur()->getText("drugDialog", "everyDay") ;
  std::string temp2 = pContexte->getSuperviseur()->getText("drugDialog", "everyOtherDay") ;
  std::string temp3 = pContexte->getSuperviseur()->getText("drugDialog", "1DayOutOf3") ;
  std::string temp4 = pContexte->getSuperviseur()->getText("drugDialog", "1DayOutOf4") ;
  std::string temp5 = pContexte->getSuperviseur()->getText("drugDialog", "4DaysOutOf7") ;
  std::string temp6 = pContexte->getSuperviseur()->getText("drugDialog", "5DaysOutOf7") ;
  std::string temp7 = pContexte->getSuperviseur()->getText("drugDialog", "6DaysOutOf7") ;
  CycleSimple->AddElement(temp1, NULL); // tous les jours
  CycleSimple->AddElement(temp2, CreateRegularRythme("1", "2DAT01", "2", "2DAT01", pContexte)) ; // 1/2
  CycleSimple->AddElement(temp3, CreateRegularRythme("1", "2DAT01", "3", "2DAT01", pContexte)) ; // 1/3
  CycleSimple->AddElement(temp4, CreateRegularRythme("1", "2DAT01", "4", "2DAT01", pContexte)) ; // 1/4
  CycleSimple->AddElement(temp5, CreateRegularRythme("4", "2DAT01", "7", "2DAT01", pContexte)) ; // 4/7
  CycleSimple->AddElement(temp6, CreateRegularRythme("5", "2DAT01", "7", "2DAT01", pContexte)) ; // 5/7
  CycleSimple->AddElement(temp7, CreateRegularRythme("6", "2DAT01", "7", "2DAT01", pContexte)) ; // 5/7
  CycleSimple->SetLostFocus(new MemFunctor<NSMedicamentDlg>((NSMedicamentDlg*)this, &NSMedicamentDlg::SaveSimpleRythme ));
  CycleSimple->PrintCombo();
  CycleSimple->SetSelIndex(0);
}

void  NSMedicamentDlg::SaveSimpleRythme()
{
  if (NULL != currentPhase)
    currentPhase->SaveCurrentCycle();
  else
    erreur("vous devez creer une phase", warningError, 0) ;
}

bool NSMedicamentDlg::RecupereData(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;
    if ((TestIfPathoOkForDialog(temp) == true) && ((*pptIter)->getColonne() == 1))
    {
      // pptIter-- ;
      return true ;
    }
    Avance(pptIter, pptEnd) ;// ++;
  }
  return false ;
}

bool
NSMedicamentDlg::ParseMedicament()
{
  PatPathoIter pptIter = pPPT->begin() ;
  PatPathoIter pptEnd  = pPPT->end() ;
  if (pptIter != pptEnd)
  	sLexiqCode = (*pptIter)->getLexique() ; // Recuperation du nom  du medicament

  Avance(pptIter, pptEnd) ;

  while (pptIter != pptEnd)
  {
    std::string temp = (*pptIter)->getLexiqueSens(pContexte) ;
    if (TestIfPathoOkForDialog(temp) == false)
      if ( RecupereData(pptIter, pptEnd) == false )
        return false ;

    if (temp == "KOUVR")
    	if (loadDateOuverture(pptIter, pptEnd) == false)
      	return false ;

    if (temp == "KFERM")
    	if (loadDateFermeture(pptIter, pptEnd) == false)
      	return false ;

    if (("0MEDF" == temp) && ( LoadUniteDePrise(pptIter, pptEnd) == false) )
    	if (RecupereData(pptIter, pptEnd) == false)
      	return false ;

    if (temp == "KPHAT") // Charge une phase
    {
    	NSphaseMedic* pPhaseMedic = new NSphaseMedic(this, false) ;
      if (pPhaseMedic->Load(pptIter, pptEnd) == false)
      	if (RecupereData(pptIter, pptEnd) == false)
        	return false ;
      pPhases->push_back(pPhaseMedic) ;
    }

    if (temp == "LADMI")
      LoadAdmin(pptIter, pptEnd) ;
  }
  return true;
}

bool
NSMedicamentDlg::LoadAdmin(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
  std::string temp = "" ;

  int Col;
  if (pptIter != pptEnd)
  {
    Col  = (*pptIter)->getColonne() ;
    temp = (*pptIter)->getLexiqueSens(pContexte) ;
  }

  Avance(pptIter, pptEnd);
  int tempcol = Col;
  while ((pptIter != pptEnd) && (tempcol > Col))
  {
    temp = (*pptIter)->getLexiqueSens(pContexte) ;

    if ((temp != std::string("LSUBS")) &&  (temp != std::string("LREMB")))
      return false ;

    if (temp == std::string("LSUBS"))
    {
      substituable->SetCheck(BF_CHECKED) ;
      Avance(pptIter, pptEnd) ;
    }

    if (temp == std::string ("LREMB"))
    {
      remboursable->SetCheck(BF_CHECKED) ;
      Avance(pptIter, pptEnd) ;
    }

		if (pptIter != pptEnd)
    	tempcol = (*pptIter)->getColonne() ;
  }
  return true ;
}

bool
NSMedicamentDlg::LoadUniteDePrise(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
	if (pptIter == pptEnd)
		return true ;

	Avance(pptIter, pptEnd) ;
	if (pptIter != pptEnd)
		sPriseUnit = (*pptIter)->getLexique() ;

	Avance(pptIter, pptEnd) ;
  return true ;
}


/*
** Fonction qui charge la date d'ouverture a partir d'une PatPatho
*/
bool
NSMedicamentDlg::loadDateOuverture(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
	int iColBaseOuverture = (*pptIter)->getColonne() ;

	Avance(pptIter, pptEnd);

	if (((*pptIter)->getColonne() > iColBaseOuverture)  &&
      (((*pptIter)->getUnitSens(pContexte) == "2DA02") ||
                     ((*pptIter)->getUnitSens(pContexte) == "2DA01")) &&
       ((*pptIter)->getLexiqueSens(pContexte) == "£D0"))
		sDateOuverture = (*pptIter)->getComplement() ;

  if (sDateOuverture == "")
  {
    char pszDateBrut[10] ;
    donne_date_duJour(pszDateBrut) ;

    NVLdVTemps tpsNow ;
  	tpsNow.takeTime() ;
		sDateOuverture = tpsNow.donneDateHeure() ;
  }
  Avance(pptIter, pptEnd) ;
  pDateOuverture->setDate(sDateOuverture) ;
  return true ;
}

bool
NSMedicamentDlg::loadDateFermeture(PatPathoIter& pptIter, PatPathoIter& pptEnd)
{
	int iColBaseOuverture = (*pptIter)->getColonne() ;

	Avance(pptIter, pptEnd) ;
  if (((*pptIter)->getColonne() > iColBaseOuverture)  &&
      (((*pptIter)->getUnitSens(pContexte) == "2DA02") ||
                    ((*pptIter)->getUnitSens(pContexte) == "2DA01")) &&
          ((*pptIter)->getLexiqueSens(pContexte) == "£D0"))
		sDateFermeture = (*pptIter)->getComplement() ;

  if (sDateFermeture == "")
  {
    NVLdVTemps tpsNow ;
  	tpsNow.takeTime() ;
		sDateFermeture = tpsNow.donneDateHeure() ;
  }
  Avance(pptIter, pptEnd) ;
  pDateFermeture->setDate(sDateFermeture) ;
  return true;
}

void
NSMedicamentDlg::FullPriseUnit()
{
  sLexiqCode = medicname->getCode() ;
}

void
NSMedicamentDlg::ExecutedAfterMedicamentSelection()
{
  sLexiqCode = medicname->getCode() ;
  
  if (sLexiqCode != "")
  {
  	string sLabel ;
    string sLang = pContexte->getUtilisateur()->donneLang() ;
    pContexte->getDico()->donneLibelle(sLang, &sLexiqCode, &sLabel) ;

    string sMITxt = pContexte->getSuperviseur()->getText("drugDialog", "missingInformation") ;

    string sCodeDisp = initDispUnit(sLang, sLexiqCode, sLabel, pContexte) ;
    if (sCodeDisp != sMITxt) // Verifie que le code exite
    {
    	pUnitePrise->setLabel(sCodeDisp) ;
      sPriseUnit = sCodeDisp ;
    }
    else
    	pUnitePrise->SetText(sCodeDisp.c_str()) ;
	}
    //FIXME reinite la fenetre

	initPosoAndCycleForDrug() ;
}

void
NSMedicamentDlg::EvCycleKey()
{
  int index = viewCycle->GetSelIndex() ;
  if (index >= 0)
    currentPhase->SetCurrentCycle(index) ;
}

void
NSMedicamentDlg::ActualisePhasePreview()
{
  if (currentPhase != NULL)
	{
    int selectedPhase = phaseBox->GetSelIndex() ;
    if (selectedPhase == -1)
    	selectedPhase = currentPhase->GetPhasePreviewIndex() ;
    currentPhase->save() ;
    std::string decode = currentPhase->Decode() ;
    if (selectedPhase >= 0)
    	phaseBox->ChangeLabel(decode, selectedPhase) ;
    phaseBox->PrintListAsNow() ;
	}
	ActualiseEndOfPrescription() ;
}

/*
** Fonction qui actualise le contenu afficher pour resumer les rythmes
*/
void
NSMedicamentDlg::AcutaliseCyclePreview()
{
	int index ;
	if (currentPhase != NULL)
	{
		index = viewCycle->GetSelIndex() ;
    if (index == -1)
    {
    	index = currentPhase->GetIndexOfCyclePreview() ;
      if (index >= 0)
      	viewCycle->SetSelIndex(index) ;
    }
    currentPhase->SaveCurrentCycle() ;
    NSMedicCycleGlobal* currentCycle = NULL ;
    if (index >= 0)
    	currentCycle = (*currentPhase)[index] ;
    if (currentCycle != NULL)
    {
    	std::string actua = currentCycle->GetRythme()->Decode() ;
      viewCycle->ChangeLabel(actua, index) ;
    }
    viewCycle->PrintListAsNow() ;
  }
	viewCycle->SetSelIndex(index) ;
}

bool
NSMedicamentDlg::checkValidity()
{
	sPriseUnit = pUnitePrise->getCode() ; // Recupere le code du mode de prise
  std::string temp = "" ;
 // for (int i =0; i < pPhases->size(); i++)
   //temp += (*pPhases)[i]->isValid();

 	std::string medErr   = pContexte->getSuperviseur()->getText("drugWindow", "noMedicNameErr") + "\n" ;
  std::string PriseErr = pContexte->getSuperviseur()->getText("drugWindow", "noPriseUnitErr") + "\n" ;

  if (sLexiqCode == "")
    temp = medErr ;

  if (sPriseUnit == "")
    temp += PriseErr ;

  if (temp != "")
  {
  	erreur(temp.c_str(), standardError, 0) ;
		return false ;
  }
  else
    return true ;
}

void NSMedicamentDlg::AddCRythm()
{
  currentPhase->SaveCurrentCycle() ;
  currentPhase->AddCycle() ;
  NSMedicCycleGlobal::ReinitDialog(this) ;

  int iPosoIndex = tabPoso->GetSel() ;
  updatePosologie(iPosoIndex) ;
  int iCycleIndex = tabCycle->GetSel() ;
  updateCycle(iCycleIndex) ;

  InitPosoControls() ;
  InitCycleControls() ;
}

void
NSMedicamentDlg::EnableControl(int RessourceId, bool visible)
{
  if (visible)
  {
    ::ShowWindow(GetDlgItem(RessourceId),   SW_SHOW);
    ::EnableWindow(GetDlgItem(RessourceId), TRUE );
  }
  else
  {
    ::ShowWindow(GetDlgItem(RessourceId),   SW_HIDE);
    ::EnableWindow(GetDlgItem(RessourceId), FALSE );
  }
}

void
NSMedicamentDlg::ControleWindow()
{
  if (_Cycle == true)
  {
    if (_extendtedCycle == false)
      DrawCycleMode();
  }
  else
  {
    if (_extendtedCycle == true)
      DrawCycleMode();
  }

  if (_Phase == true)
  {
    if (_extendedForm ==false)
      RedrawForm(true);
  }
  else
  {
    if (_extendedForm ==true)
       RedrawForm(true);
  }
}

void
NSMedicamentDlg::LoadPhaseOnViewAndConfigure()
{
  for (size_t i = 0; i < pPhases->size(); i++)
  {
    std::string PhaseN = (*pPhases)[i]->Decode() ;
    phaseBox->AddLabel((char*)PhaseN.c_str()) ;
  }
  phaseBox->PrintListAsNow() ;
  currentPhase = (*pPhases)[0] ;
  currentPhase->Load() ;
 // phaseBox->SetSelIndex(0);
}

// Touche taper dans la windows
void NSMedicamentDlg::EvKeyDown(uint key, uint repeatCount, uint flags )
{
}

void
NSMedicamentDlg::initPosoAndCycleForDrug()
{
	string sStarterSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sLexiqCode, &sStarterSens) ;
  if (sStarterSens == string(""))
  	return ;

	VectString VSIsAList ;
	pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sStarterSens, "ES", &VSIsAList) ;

	// AVK : poso MMS, cycle Day1Day2Day3
  //
/*
  if (VSIsAList.contains("IA069"))
  {
  	DrawCycleExtended() ;
    updatePosologie(1) ;
		updateCycle(3) ;
    InitPosoControls() ;
    InitCycleControls() ;
  	return ;
  }
*/
}

void
NSMedicamentDlg::InitComboModedePrise()
{
  NSPatPathoArray* ava = new NSPatPathoArray(pContexte) ;
  ava->ajoutePatho("KAVAN1", 0, 0) ;
  ava->ajoutePatho("KREPA1", 1, 0) ;

  NSPatPathoArray* pend = new NSPatPathoArray(pContexte) ;
  pend->ajoutePatho("KPEND1",0, 0) ;
  pend->ajoutePatho("KREPA1",1, 0) ;

  NSPatPathoArray* aft = new NSPatPathoArray(pContexte) ;
  aft->ajoutePatho("KAPRE1",0, 0) ;
  aft->ajoutePatho("KREPA1",1, 0) ;

  NSPatPathoArray* nulli = new NSPatPathoArray(pContexte) ;

  std::string val1 = pContexte->getSuperviseur()->getText("drugDialog", "beforeLunch") ;
  std::string val2 = pContexte->getSuperviseur()->getText("drugDialog", "duringLunch") ;
  std::string val3 = pContexte->getSuperviseur()->getText("drugDialog", "afterLunch") ;
  std::string val4 = pContexte->getSuperviseur()->getText("drugDialog", "awayFromLunch") ;
  std::string val5("") ;

  modedeprise->AddElement(val5, nulli) ;
  modedeprise->AddElement(val1, ava) ;
  modedeprise->AddElement(val2, pend) ;
  modedeprise->AddElement(val3, aft) ;
  modedeprise->PrintCombo() ;
  modedeprise->SetSelIndex(0) ;
}

void
NSMedicamentDlg::PrintTabCycle(bool print)
{
  EnableControl(IDC_TABCYCLE, print) ;
  EnableControl(IDC_LISTVIEWFORCYCLE, print) ;
  EnableControl(IDC_SIMPLECYCLE, !print) ;
}

void
NSMedicamentDlg::InitTabPosologie()
{
  char *OngletNoms[] = { "periodOfTime", "morningNoonNight", "hours", "regularCycle", "irregularCycle", "freeText", "singleTake"} ;
  for (int i =0; i < 7; i++)
	{
    TTabItem tab(pContexte->getSuperviseur()->getText("drugDialog", OngletNoms[i]).c_str()) ;
    tabPoso->Add(tab) ;
  }
  updatePosologie(1) ;
}

void
NSMedicamentDlg::InitPosoControls()
{
}

void
NSMedicamentDlg::InitTabCycle()
{
  char *OngletCycle[] = { "regularRythme", "freeRythme", "daysOfTheWeek", "day1Day2Day3" } ;
  for (int i = 0 ; i < 4 ; i++)
  {
    TTabItem tab(pContexte->getSuperviseur()->getText("drugDialog", OngletCycle[i]).c_str()) ;
    tabCycle->Add(tab) ;
  }
  updateCycle(4) ;
  PrintTabCycle(false) ;
}

void
NSMedicamentDlg::InitCycleControls()
{
	if (!_extendtedCycle)
		return ;

	// Just for Day1Day2Day3
	//
	int index = tabCycle->GetSel() ;
  if (index != 3)
		return ;

	// No current phase: init to Day 1
	if (!currentPhase || (currentPhase->CycleNumber() == 0))
	{
  	_numJour->initControle(1) ;
    return ;
	}

  std::vector<int> aExistingDays ;

  int iNbCycles = currentPhase->CycleNumber() ;
  for (int i = 0 ; i < iNbCycles ; i++)
  {
  	NSMedicCycleGlobal* pCycle = (*currentPhase)[i] ;
    if (pCycle)
    {
    	NSRythme* pRythm = pCycle->GetRythme() ;
      if (pRythm && pRythm->getData())
      {
      	Day1Day2Day3Rythme* pDDD = dynamic_cast<Day1Day2Day3Rythme *>(pRythm->getData()) ;
        if ((pDDD != NULL) && (pDDD->getIndice() > 0))
        	aExistingDays.push_back(pDDD->getIndice()) ;
      }
    }
  }

  if (aExistingDays.empty())
  {
  	_numJour->initControle(1) ;
    return ;
	}

  sort(aExistingDays.begin(), aExistingDays.end()) ;

  int iRef = 0 ;
  std::vector<int>::iterator daysIt = aExistingDays.begin() ;
  for ( ; daysIt != aExistingDays.end() ; daysIt++)
  {
  	// detect a discrepancy
  	if (*daysIt > iRef + 1)
      break ;
    else
    	iRef = *daysIt ;
  }
  iRef++ ;

	_numJour->initControle(iRef) ;
	return ;
}

void
NSMedicamentDlg::saveCycle()
{
}

void
NSMedicamentDlg::CreateTree()
{
  int iColonne = 0 ;
  std::string sMEdicName = medicname->getCode() ;
  pPPT->vider() ;
  pPPT->ajoutePatho(sMEdicName, iColonne++, 0) ;

  // (*pPPT)[0]->setNodeRight("droit pipo");

  //pPPT->ajoutePatho(sLexiqCode, iColonne++, 0) ; // nom du medicament a la racine
  int iMedicRoot  = iColonne ;
  // Create Header
  pDateOuverture->getDate(&sDateOuverture) ;
  if (sDateOuverture == "")
  {
        // (initialisation par défaut)
  }
  pPPT->ajoutePatho("KOUVR1", iColonne++, 1) ;
  Message CodeMsg ;
	CodeMsg.SetUnit("2DA021") ;
	CodeMsg.SetComplement(sDateOuverture) ;
	pPPT->ajoutePatho("£D0;19", &CodeMsg, iColonne++, 1) ;
	iColonne = iMedicRoot ;

	pDateFermeture->getDate(&sDateFermeture) ;
	if (sDateFermeture != "")
	{
  	// insertion de la date d'ouverture au sein de la patpatho
    pPPT->ajoutePatho("KFERM1", iColonne++, 1) ;
    Message CodeMsg2 ;
		CodeMsg2.SetUnit("2DA021") ;
		CodeMsg2.SetComplement(sDateFermeture) ;
		pPPT->ajoutePatho("£D0;19", &CodeMsg2, iColonne++, 1) ;

		iColonne = iMedicRoot ;
	}
	iColonne = iMedicRoot ;

	std::string sEnCas = pEnCasDe->getCode() ;
	if (sEnCas != "")
	{
  	pPPT->ajoutePatho("KEVEI1", iColonne++, 1) ;
    pPPT->ajoutePatho(sEnCas, iColonne++, 1) ;
  }
  iColonne = iMedicRoot ;

	std::string sCode = pUnitePrise->getCode() ;
	if (sCode != "")
  {
  	pPPT->ajoutePatho("0MEDF1", iColonne++, 1) ;
    pPPT->ajoutePatho(sCode, iColonne++, 1) ;
  }
  iColonne = iMedicRoot ;

	if ((remboursable->getValue() == BF_CHECKED) || (substituable->getValue() == BF_CHECKED))
	{
  	pPPT->ajoutePatho("LADMI1", iColonne,1) ;

    if (substituable->getValue() == BF_CHECKED)
    {
    	Message Msg ;
      Msg.SetCertitude("WCE001") ;
      pPPT->ajoutePatho("LSUBS1", &Msg, iColonne +1 , 1) ;
    }
    iColonne = iMedicRoot ;

    if (remboursable->getValue() == BF_CHECKED)
    {
    	Message Msg ;
      Msg.SetCertitude("WCE001") ;
      pPPT->ajoutePatho("LREMB1", &Msg, iColonne + 1, 1) ;
    }
    iColonne = iMedicRoot ;
  }

	// Inserer mode de prise     FIXME

	for (size_t i = 0; i < pPhases->size(); i++)
	{
		NSPatPathoArray* phase_temp = (*pPhases)[i]->CreateTree() ;
    if (phase_temp)
    {
    	pPPT->InserePatPatho(pPPT->end(), phase_temp, iColonne) ;
      delete phase_temp ;
    }
	}
}

void
NSMedicamentDlg::EvSetFocus(HWND hWndLostFocus)
{
}

void
NSMedicamentDlg::TabSelChangePso(TNotify far& /* nm */)
{
	int index = tabPoso->GetSel() ;
	SaveSimpleRythme() ;
	updatePosologie(index) ;
	// NSCircadien::ReinitDialog(this);   // On reinitialise le medicament
}

void
NSMedicamentDlg::updatePosologie(int index)
{
	register unsigned int i ;
	for (i = 0; i < 12; i++) EnableControl(periodeTab[i],   (index == 1)) ;
	for ( i = 0; i < 1; i++) EnableControl(TxtLibre[i],     (index == 5)) ;
	for ( i = 0; i < 8; i++) EnableControl(regularDay[i],   (index == 3)) ;
	for (i = 0; i < 21; i++) EnableControl(periodeCycle[i], (index == 0)) ;
	for (i = 0; i < 8;  i++) EnableControl(hourCycle[i],    (index == 2)) ;
	for (i = 0; i < 11; i++) EnableControl(idcPosoLibre[i], (index == 4)) ;
	for (i = 0; i < 3; i++)  EnableControl(PosoUnique[i],   (index == 6)) ;
	tabPoso->SetSel(index) ;
}

void
NSMedicamentDlg::TabSelChangeCyc(TNotify far& /* nm */)
{
	register int index = tabCycle->GetSel() ;
	updateCycle(index) ;
}

void
NSMedicamentDlg::updateCycle(int index)
{
	register int i ;
  for (i = 0; i < 5; i++)  EnableControl(CycleJourAlt[i], (index == 3)) ;
	for (i = 0; i < 7; i++)  EnableControl(CycleJour[i],    (index == 2)) ;
	for (i = 0; i < 9; i++)  EnableControl(RegularCycle[i], (index == 0)) ;
	for (i = 0; i < 11; i++) EnableControl(FreeCycle[i],    (index == 1)) ;
	tabCycle->SetSel(index) ;
}

void
NSMedicamentDlg::AddHour()
{
  std::string val       = pPriseHeure->getValue() ;
  std::string heure     = pPriseHeure->getHeure() ;
  std::string toAffiche = CreateLabelForHour(val,heure ) ;
  PriseHeure* news = new PriseHeure(val, heure) ;

  bool result = true;
  for (int i =0; i < ViewPriseHeure->Size(); i++)
    if ( (*news == *ViewPriseHeure->getDataAt(i))== true)
        result = false;
  if (result == true)
  {
    ViewPriseHeure->addItem(toAffiche.c_str(), news) ;
    ViewPriseHeure->sortLabel(CompareMedicament) ;
    ViewPriseHeure->SetSelIndex(ViewPriseHeure->Index()) ;
  }
  else
  {
  	delete news ;
    erreur("Prise existante", standardError, 0) ;
  }
}

void
NSMedicamentDlg::YAddPhase()
{
  if (currentPhase != NULL) // il existe une phase courrante
    currentPhase->save() ;

	NSphaseMedic* pNewPhase = new NSphaseMedic(this) ;
	std::string   phaseName2 = "" ;
	phaseName2 = std::string("phase ") + phaseIndex ;
	phaseBox->addItem(phaseName2.c_str(), pNewPhase) ;
	currentPhase = pNewPhase ;
	currentPhase->Load() ;
	phaseIndex++ ;
	currentPhase->save() ;
	phaseBox->SetSelIndex(pPhases->size()-1) ;
	currentPhase->SetPhasePreview(pPhases->size()-1) ;
}

void
NSMedicamentDlg::EvListBoxSelPhase()
{
	currentPhase->save() ;
	int index = phaseBox->GetSelIndex() ;      // Recuperer la phase d'avant
	if (index >= 0)
	{
  	currentPhase= (*pPhases)[index] ;
    currentPhase->SetPhasePreview(index) ;
    currentPhase->LinkToDialog() ; // Relie les controles au sous dialog
    currentPhase->Load() ;
	}
  currentPhase->save() ;
}

/*
** Calcul la date de fin
*/
void NSMedicamentDlg::ActualiseEndOfPrescription()
{
  std::string date;
  pDateOuverture->getDate(&date);
  NVLdVTemps data;
  data.initFromDate(date);
  for (size_t i = 0; i < pPhases->size(); i++)
  {
    NSphaseMedic* temp =  (*pPhases)[i] ;
    int duree =   temp->GetDureePhase() ;
    int renouv =  temp->GetNumberOfRenouvellement() + 1 ;
    std::string sym = temp->GetSymBolOfPhase() ;
    if (renouv > 0)
    {
      if (sym == std::string("2HEUR1")) // Ajoute heure
      	data.ajouteHeures(duree * renouv) ;
      if (sym == std::string("2DAT01")) // Ajoute jour
      	data.ajouteJours(duree * renouv) ;
      if (sym == std::string("2DAT11")) // Ajoute semaine
      	data.ajouteJours(duree * renouv * 7) ;
      if (sym == std::string("2DAT21")) //Ajoute moi
      	data.ajouteMois(duree * renouv) ;
    }
  }
  date = data.donneDateHeure() ;
  pDateFermeture->setDate(date) ;
}

void
NSMedicamentDlg::EvListBoxSelCycle()
{
	currentPhase->SaveCurrentCycle() ;
	int index = viewCycle->GetSelIndex() ;
	currentPhase->SetCurrentCycle(index) ;
}

void
NSMedicamentDlg::EvListBoxSelChange()
{
  int index = ViewPriseHeure->GetSelIndex() ;
  if ( index >= 0)
  {
    PriseHeure* prise = ViewPriseHeure->getDataAt(index) ;
    pPriseHeure->initFromPriseHeure(prise) ;
  }
}

void
NSMedicamentDlg::DrawCycleMode()
{
  if (_extendtedCycle == false)          // avant ==
		DrawCycleExtended() ;
  else
  	DrawCycleLimited() ;
}

void
NSMedicamentDlg::DrawCycleExtended(bool bForceRedraw)
{
	if (_extendtedCycle && !bForceRedraw)
		return ;

	MoveControl(_CycleGroup, 4, 148, 412, 244) ;
	PrintTabCycle(1) ;
	if (_LoadForChange == false)
  	updateCycle(0) ;
	_topPhase = 249 ;
	EnableControl(IDC_ADDRYTHM, true) ;
  _extendtedCycle = true ;
	_ChangeCyle->SetCaption(pContexte->getSuperviseur()->getText("drugDialog", "simpleMode").c_str()) ;

  RedrawForm(false) ;
}

void
NSMedicamentDlg::DrawCycleLimited(bool bForceRedraw)
{
	if (!_extendtedCycle && !bForceRedraw)
		return ;

	MoveControl(_CycleGroup, 4, 148, 412, 178) ;
	PrintTabCycle(0) ;
	updateCycle(4) ;
	_topPhase = 183 ;
	EnableControl(IDC_ADDRYTHM, false) ;
	_extendtedCycle = false ;
	_ChangeCyle->SetCaption(pContexte->getSuperviseur()->getText("drugDialog", "advancedMode").c_str()) ;

  RedrawForm(false) ;
}

void
NSMedicamentDlg::MoveControl(TControl* cont, int left, int top, int h , int w)
{
  NS_CLASSLIB::TRect recGen(left, top, h, w) ;
  MapDialogRect(recGen) ;
  cont->MoveWindow(recGen.left , recGen.top, recGen.Width(), recGen.Height(), true) ;
}

void
NSMedicamentDlg::lanceExtended()
{
  RedrawForm(true) ;
}

void
NSMedicamentDlg::RedrawForm(bool extend)
{
  if (_extendedForm == false)  // Passer en mode etendu
  {
  	if (extend == true)
    {
    	for (int i = 0; i < 2; i++)    // On fait apparaitre les controles
      	EnableControl(ExtendPhases[i], true) ;
      _extendedForm = true ;
      ModePhase->SetCaption(pContexte->getSuperviseur()->getText("drugDialog", "singlePhaseMode").c_str()) ;
    }
  }
  else //passer en mode simple
  {
    if (extend == true)
    {
    	if (pPhases->size() <= 1)
      {
      	for (int i = 0; i < 2; i++)    // On fait apparaitre les controles
        	EnableControl(ExtendPhases[i], false) ;
        _extendedForm = false;
        ModePhase->SetCaption(pContexte->getSuperviseur()->getText("drugDialog", "multiplePhasesMode").c_str()) ;
      }
      else
      {
      	string sErrorMsg = pContexte->getSuperviseur()->getText("drugDialogErrors", "singlePhaseModeNotAvailable") ;
      	erreur(sErrorMsg.c_str(), warningError, 0) ;
      }
    }
  }

  MoveControl(idc_phase1,18,_topPhase+16, 44, _topPhase +36);
  MoveControl(idc_static2,159,_topPhase+16, 208, _topPhase +36);
  MoveControl(idc_static3,266,_topPhase+16, 298, _topPhase +36);
  MoveControl(pDureePhase->GetEdit(),48,_topPhase + 14, 92, _topPhase + 29);
  MoveControl(pDureePhase->GetUpdown(),92,_topPhase +14, 103, _topPhase + 29);
  MoveControl(pCBDureePhase,105,_topPhase +14, 155, _topPhase +  65);
  MoveControl(pRenouvellement->GetEdit(), 208,_topPhase +14, 248, _topPhase + 29);
  MoveControl(pRenouvellement->GetUpdown(), 248,_topPhase +14, 259, _topPhase + 29);
  MoveControl(phaseBox, 8, _topPhase +45, 408 , _topPhase + 80);
  MoveControl(ModePhase,320,_topPhase +10, 407, _topPhase + 25);
  MoveControl(AddPhase,327,_topPhase +27, 407, _topPhase + 41);

  NS_CLASSLIB::TRect dlgWin ;  // Win
  GetWindowRect(dlgWin) ;      // win

  int adjust = ((_extendtedCycle ==true)? 0 : 60 ) + ((_extendedForm ==true)? 0 : 45 ) ;
  NS_CLASSLIB::TRect recw(0, 0, 0, 370-adjust) ;
  MapDialogRect(recw) ;
  MoveWindow(dlgWin.left, dlgWin.top, dlgWin.Width(), recw.Height(), true) ;

	int adj ;
	if (_extendedForm == true)
  	adj = 85 ;
	else
  	adj = 40 ;
  MoveControl(_PhaseGroup, 4, _topPhase, 412, _topPhase+adj) ;

  int less = 55 ;
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect recOk(256, 0, 0, 0) ;
  NS_CLASSLIB::TRect recCancel(308, 0, 0, 0) ;
  NS_CLASSLIB::TRect recHelp(360, 0, 0, 0) ;
  NS_CLASSLIB::TRect &rec2Ok  = ok->GetWindowRect() ;
  NS_CLASSLIB::TRect &rec2Cancel = ok->GetWindowRect() ;
  NS_CLASSLIB::TRect &rec2Help = ok->GetWindowRect() ;
  MapDialogRect(recOk) ;
  MapDialogRect(recCancel) ;
  MapDialogRect(recHelp) ;
  ok->MoveWindow(recOk.left, dlgRect.Height() - less, rec2Ok.Width(), rec2Ok.Height(), true) ;
  cancel->MoveWindow(recCancel.left, dlgRect.Height() - less, rec2Cancel.Width(), rec2Cancel.Height(), true) ;
  help->MoveWindow(recHelp.left, dlgRect.Height() - less, rec2Help.Width(), rec2Help.Height(), true) ;
}

void
NSMedicamentDlg::CmOk()
{
	if (checkValidity())
  {
    currentPhase->save() ;
    CreateTree() ;
    TDialog::CmOk() ;
  }
}

std::string
initDispUnit(string sLang, string sCode, string sLib, NSContexte* pContexte)
{
	return initDispUnitFr(sCode, sLib, pContexte) ;
}

std::string
initDispUnitFr(string sCode, string sLib, NSContexte* pContexte)
{
	if ((sCode == "") && (sLib == ""))
		return string("") ;

	// Cuillérées
	//
	if ((sLib.find(" sol buv") != NPOS) || (sLib.find(" solution buvable")!= NPOS) )
	{
  	if (sLib.find("gts")!= NPOS)
    	return string("2GOUT1") ;
    return string("2CUIC2") ;
  }
  if ((sLib.find(" susp buv") != NPOS) || (sLib.find(" suspension buvable")!= NPOS) )
  	return string("2CUIC2") ;
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
  if (sLib.find(" cp vaginal") != NPOS)
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
  if ((sLib.find(" pulv buccale") != NPOS) || (sLib.find("pulverisation buccale")!= NPOS) || (sLib.find(" fl préssurisé ")!= NPOS) )
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
  if (sLib.find("fl compte-gtes") != NPOS)
  	return string("2GOUT1") ;
  if (sLib.find("collyre") != NPOS)
  	return string("2GOUT1") ;
  //
  // Sachets
  //
  if (sLib.find("sach-dose") != NPOS)
  	return string("7SACH1") ;

  //
  // Ampoule
  //
	if ((sLib.find(" amp ") != NPOS) || (sLib.find(" ampoule ") != NPOS) )
		return string("7AMPO1") ;


	return pContexte->getSuperviseur()->getText("drugDialog", "missingInformation") ;
}

//
// Dialogue NSRenewMedicDlg
//

DEFINE_RESPONSE_TABLE1(NSRenewMedicDlg, NSUtilDialog)
  EV_COMMAND(IDOK,          CmOk),
  EV_COMMAND(IDCANCEL,      CmCancel),
  EV_COMMAND(IDHELP,        CmHelp),
  EV_WM_ACTIVATE,
  EV_WM_SETFOCUS,
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------
// constructeur par défaut
// -----------------------------------------------------------------------------
NSRenewMedicDlg::NSRenewMedicDlg(TWindow *parent, NSContexte *pCtx, NSPatPathoArray *pPPTinit)
            :NSUtilDialog(parent, pCtx, "IDD_RENEW_MEDIC", pNSDLLModule)
{
try
{
    sLexique            = "" ;
    sDateRenouvelement  = "" ;

    // codes lexiques correspondant aux combobox
    VecteurString   *pLexiqCodesCB = new VecteurString() ;
    pLexiqCodesCB->push_back(new string("2HEUR1")) ;  // heure
    pLexiqCodesCB->push_back(new string("2DAT01")) ;  // jour
    pLexiqCodesCB->push_back(new string("2DAT11")) ;  // semaine
    pLexiqCodesCB->push_back(new string("2DAT21")) ;  // mois

    iDureeCycle       = 0 ;
    sUnitDureeCycle   = "" ;
    iNbRenouvelement  = 0 ;

    //les contrles de la phase
    pDureeCycle     = new NSUpDownEdit  (this, pContexte, "", IDC_EDIT_PDT, IDC_UPDOWN_PDT) ;
    pCBDureeCycle   = new NSComboBox    (this, IDC_COMBOBOX_PDT, pContexte, pLexiqCodesCB) ;
    pRenouvelement  = new NSUpDownEdit  (this, pContexte,"", IDC_EDIT_RNV, IDC_UPDOWN_RNV) ;

    pLexiqCodesCB->vider() ;
    delete pLexiqCodesCB ;

    // rustine bug NSUtilDialog
    pOK        = new TButton       (this, IDOK) ;
    pCancel    = new TButton       (this, IDCANCEL) ;
    pHelp      = new TButton       (this, IDHELP) ;

    pPPT            = pPPTinit ;
}
catch (...)
{
	erreur("Exception NSRenewMedicDlg ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// destructeur
// -----------------------------------------------------------------------------
NSRenewMedicDlg::~NSRenewMedicDlg()
{
  //phase
  delete pRenouvelement ;
  delete pCBDureeCycle ;
  delete pDureeCycle ;

  delete pOK ;
  delete pCancel;
  delete pHelp;
}

void
NSRenewMedicDlg::SetupWindow()
{
    NSUtilDialog::SetupWindow();
}

bool
NSRenewMedicDlg::chercheNoeud(string sName)
{
    if ((!pPPT) || (pPPT->empty()))
        return false ;

    PatPathoIter pptIter = pPPT->begin();
    while ((pptIter != pPPT->end())&& ((*pptIter)->getLexique() != sName))
        pptIter++ ;

    if (pptIter == pPPT->end())
        return false ;
    return true ;
}

void
NSRenewMedicDlg::CmOk()
{
	// NSbaseObjMedic baseObj;

	// récupération de la date du jour au format AAAAMMJJHHMMSS
	NVLdVTemps sez ;
	sez.takeTime() ;
	sDateRenouvelement = sez.donneDateHeure() ;

	// sauvegarde des données concernant la phase
	if (pDureeCycle->getValue()     != iDureeCycle)
		iDureeCycle      = pDureeCycle->getValue() ;
	if (pCBDureeCycle->getSelCode() != sUnitDureeCycle)
  	sUnitDureeCycle  = pCBDureeCycle->getSelCode() ;
	if (pRenouvelement->getValue()  != iNbRenouvelement)
  	iNbRenouvelement = pRenouvelement->getValue() ;

	if (iDureeCycle == 0)
	{
  	string sErrorMsg = pContexte->getSuperviseur()->getText("drugDialogErrors", "renewDurationNotSpecified") ;
    erreur(sErrorMsg.c_str(), warningError, 0) ;
    return ;
	}

	if (sUnitDureeCycle == "")
	{
  	string sErrorMsg = pContexte->getSuperviseur()->getText("drugDialogErrors", "renewDurationUnitNotSpecified") ;
    erreur(sErrorMsg.c_str(), warningError, 0) ;
    return ;
	}

	pPPT->ajoutePatho("GRENT1", 0, 0) ;

	pPPT->ajoutePatho("KOUVR1", 1, 1) ;
	Message Msg ;
  Msg.SetUnit("2DA021") ;
  Msg.SetComplement(sDateRenouvelement.c_str()) ;
  pPPT->ajoutePatho("£D0;19", &Msg, 2, 1) ;

	pPPT->ajoutePatho("VDURE2", 1, 1) ;              // code pour pendant
 	createNodeComplement(pPPT, "£N0;03", sUnitDureeCycle, iDureeCycle, 2) ;        //FIXME

	if (iNbRenouvelement != 0)
	{
  	pPPT->ajoutePatho("VRENO1", 1, 1) ;            // code pour à renouveler
    createNodeComplement(pPPT, "£N0;03", "200001", iNbRenouvelement, 2) ;
	}

	NSUtilDialog::CmOk() ;
}

void
NSRenewMedicDlg::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

void
NSRenewMedicDlg::CmHelp()
{
}

// fin de nsmedicdlg.cpp

