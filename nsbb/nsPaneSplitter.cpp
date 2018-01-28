/*#include <owl/panespli.h>
#include <owl/layoutwi.h>
#include <owl\listbox.h>*/

#include "partage\nsdivfct.h"
#include "nssavoir\nsvarray.h"

#include <owl\panespli.h>
#include <owl\layoutwi.h>
#include <owl\listbox.h>
#include <owl\gadgetwi.h>
#include <owl\buttonga.h>
#include <dos.h>
#include <time.h>
#include <stdio.h>

#include <winsys/uimetric.h>
#include <stdlib.h>
#include "nautilus\nssuper.h"
#include "nsbb\nsPaneSplitter.h"
#include "nsbb\nsMView.h"
#include "nautilus\nscqvue.h"
#include "nsbb\nsdlg.h"
#include "ns_ob1\BB1BB.h"

#include "nautilus\nautilus.rh"
//#include "nautilus\nsSOAPView.h"

/*//-----------------------------------------------------------------------------
//                      classe NSSplitter
//----------------------------------------------------------------------------
NSSplitter::NSSplitter(TWindow* parent, NSPaneSplitter* ps, float percent)
                  :  TSplitter(parent, (TPaneSplitter*)ps, percent)
{
  pPaneSplitter = ps;
}
NSSplitter::~NSSplitter()
{
}
void
NSSplitter::SetupEpilog(TSplitter* s, TWindow* targetPane, TWindow* newPane,
                  TLayoutMetrics& lmOfTargetPane, TLayoutMetrics& lmOfNewPane)
{
  targetPane->SetParent(s);
  newPane->SetParent(s);
  if (!(newPane->IsWindow()))
    newPane->Create();
  SetChildLayoutMetrics(*targetPane, lmOfTargetPane);
  SetChildLayoutMetrics(*newPane, lmOfNewPane);
}

//------------------------------------------------------------------------------
//            classe NSHSplitter
//------------------------------------------------------------------------------

NSHSplitter::NSHSplitter(TWindow* parent, NSPaneSplitter* ps, float percent)
                  :  NSSplitter(parent, ps, percent)
{
}
NSHSplitter::~NSHSplitter()
{
}
TLayoutMetrics
NSHSplitter::SetupPane(TWindow* targetPane, TWindow* newPane, float percent)
{
  TLayoutMetrics  lmOfSplitter;
  TLayoutMetrics  lmOfPane1;
  TLayoutMetrics  lmOfPane2;

  pPaneSplitter->GetDefLM(lmOfSplitter);
  lmOfPane1 = lmOfPane2 = lmOfSplitter;

  // Target pane's height is 1/2 of what it was.  New pane is below
  // target pane.
  //
  lmOfPane1.Height.Absolute(targetPane->Attr.H * percent);
  lmOfPane2.Y.Below(targetPane, pPaneSplitter->GetSplitterWidth());
  SetupEpilog(this, targetPane, newPane, lmOfPane1, lmOfPane2);

  return lmOfSplitter;
}  */


//------------------------------------------------------------------------------
//                classe NSGadgetWindow
//------------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSGadgetWindow, TGadgetWindow)
	EV_COMMAND(IDC_OK_PANESPLIT,       CmChildOK),
  EV_COMMAND(IDC_CANCEL_PANESPLIT,   CmChildCancel),
	EV_COMMAND(IDC_HELP_PANESPLIT,     CmChildHelp),
  EV_COMMAND(IDC_PREVIOUS_PANESPLIT, CmChildPrevious),
  EV_COMMAND_ENABLE(IDC_OK_PANESPLIT,       CeHasChildOK),
  EV_COMMAND_ENABLE(IDC_CANCEL_PANESPLIT,   CeHasChildCancel),
  EV_COMMAND_ENABLE(IDC_HELP_PANESPLIT,     CeHasChildHelp),
  EV_COMMAND_ENABLE(IDC_PREVIOUS_PANESPLIT, CeHasChildPrevious),
	EV_COMMAND(CM_HELP,              CmChildHelp),
  EV_COMMAND(CM_GENERAL_ADD,       CmAdd),
  EV_COMMAND(CM_GENERAL_CLOSE,     CmClose),
  EV_COMMAND(CM_GENERAL_MODIFY,    CmChange),
	EV_COMMAND(CM_GENERAL_REMOVE,    CmSuppress),
	EV_COMMAND(CM_GENERAL_CONTINUE,  CmContinue),
	EV_COMMAND(CM_GENERAL_FCT1,      CmFct1),
	EV_COMMAND(CM_GENERAL_FCT2,      CmFct2),
	EV_COMMAND(CM_GENERAL_FCT3,      CmFct3),
	EV_COMMAND(CM_GENERAL_FCT4,      CmFct4),
	EV_COMMAND(CM_GENERAL_FCT5,      CmFct5),
	EV_COMMAND(CM_GENERAL_FCT6,      CmFct6),
	EV_COMMAND(CM_GENERAL_FCT7,      CmFct7),
	EV_COMMAND(CM_GENERAL_FCT8,      CmFct8),
	EV_COMMAND(CM_GENERAL_FCT9,      CmFct9),
  EV_COMMAND_ENABLE(CM_GENERAL_ADD,      CeHasChildAdd),
  EV_COMMAND_ENABLE(CM_GENERAL_CLOSE,    CeHasChildClose),
  EV_COMMAND_ENABLE(CM_GENERAL_MODIFY,   CeHasChildChange),
  EV_COMMAND_ENABLE(CM_GENERAL_REMOVE,   CeHasChildSuppress),
  EV_COMMAND_ENABLE(CM_GENERAL_CONTINUE, CeHasChildContinue),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT1,     CeHasChildFct1),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT2,     CeHasChildFct2),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT3,     CeHasChildFct3),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT4,     CeHasChildFct4),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT5,     CeHasChildFct5),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT6,     CeHasChildFct6),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT7,     CeHasChildFct7),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT8,     CeHasChildFct8),
  EV_COMMAND_ENABLE(CM_GENERAL_FCT9,     CeHasChildFct9),
	EV_WM_CLOSE,
	EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

/*NSGadgetWindow::NSGadgetWindow(TWindow* parent, TTileDirection direction,
                    TFont* font, TModule* module )
                    :TGadgetWindow(parent, direction,font,module)  */
//NSGadgetWindow::NSGadgetWindow(TWindow* parent, bool bAddButtons, const char far* title)

NSGadgetWindow::NSGadgetWindow(NSContexte* pCtx, TWindow* parent, uint32 iBtnStyle, const char far* /*title*/)
               :TGadgetWindow(parent),
                NSRoot(pCtx)
{
	_uButtonsStyle = iBtnStyle ;

	// bWithButtons = bAddButtons ;
/*
	if (_uButtonsStyle > 0)
	{
		pOK     = new TButton(this, IDC_OK_PANESPLIT, "", 0, 0, 15, 5) ;
		pCancel = new TButton(this, IDC_CANCEL_PANESPLIT, "", 90, 0, 15, 5) ;
		pHelp   = new TButton(this, IDC_HELP_PANESPLIT, "", 180, 0, 15, 5) ;
	}
	else
	{ */
  	_pOK     = (OWL::TButton*) 0 ;
		_pCancel = (OWL::TButton*) 0 ;
		_pHelp   = (OWL::TButton*) 0 ;
// 	}

	NSMDIChildContent* pContent = TYPESAFE_DOWNCAST(parent, NSMDIChildContent) ;
	if (pContent)
		_pPaneSplitter = TYPESAFE_DOWNCAST(pContent->getSplitter(), NSPaneSplitter) ;
  else
  	_pPaneSplitter = (NSPaneSplitter*) 0 ;

	_sTextForCmAdd      = string("") ;
	_sTextForCmClose    = string("") ;
	_sTextForCmSuppress = string("") ;
	_sTextForCmChange   = string("") ;
	_sTextForCmContinue = string("") ;
	_sTextForCmFct1     = string("") ;
	_sTextForCmFct2     = string("") ;
	_sTextForCmFct3     = string("") ;
	_sTextForCmFct4     = string("") ;
	_sTextForCmFct5     = string("") ;
	_sTextForCmFct6     = string("") ;
	_sTextForCmFct7     = string("") ;
	_sTextForCmFct8     = string("") ;
	_sTextForCmFct9     = string("") ;
}

NSGadgetWindow::~NSGadgetWindow()
{
	// Done by OWL's garbage collector
	// delete pOK ;
	// delete pCancel ;
	// delete pHelp ;
}

void
NSGadgetWindow::CmChildOK()
{
	if (_pPaneSplitter)
		_pPaneSplitter->CmOk() ;
}

void
NSGadgetWindow::CmChildHelp()
{
	if (_pPaneSplitter)
		_pPaneSplitter->CmHelp() ;
}

void
NSGadgetWindow::CmChildCancel()
{
	if (_pPaneSplitter)
		_pPaneSplitter->CmCancel() ;
}

void
NSGadgetWindow::CmChildPrevious()
{
  if (_pPaneSplitter)
		_pPaneSplitter->CmPrevious() ;
}

void
NSGadgetWindow::EvSetFocus(HWND hWndLostFocus)
{
	TWindow::EvSetFocus(hWndLostFocus) ;
	activateParent() ;
}

void
NSGadgetWindow::activateParent()
{
	_pPaneSplitter->activateParent() ;
}

void
NSGadgetWindow::SetupWindow()
{
	TGadgetWindow::SetupWindow() ;

  EnableTooltip(true) ;

  SetCaption("Gadget Window") ;

  insertMainGadgets() ;

	enableCommandForGadgets() ;

	LayoutSession() ;
}

void
NSGadgetWindow::FlushControlBar()
{
	TGadget* pGadget = FirstGadget() ;

	while (pGadget)	{
		TGadget* pGadgetRemoved = Remove(*pGadget) ;
		if (NULL == pGadgetRemoved)
		{
			erreur("Erreur à la destruction d'un bouton dans la barre d'outils", standardError, 0) ;
			return ;
  	}
		else
    	delete pGadgetRemoved ;

		pGadget = FirstGadget() ;	}

	LayoutSession() ;
}

void
NSGadgetWindow::insertMainGadgets()
{
	if ((_uButtonsStyle & MYWS_OK) == MYWS_OK)
		Insert(*new TButtonGadget(IDC_OK_PANESPLIT, IDC_OK_PANESPLIT, TButtonGadget::Command)) ;
	if ((_uButtonsStyle & MYWS_CANCEL) == MYWS_CANCEL)
  	Insert(*new TButtonGadget(IDC_CANCEL_PANESPLIT,   IDC_CANCEL_PANESPLIT,   TButtonGadget::Command)) ;
	if ((_uButtonsStyle & MYWS_HELP) == MYWS_HELP)
  	Insert(*new TButtonGadget(IDC_HELP_PANESPLIT,     IDC_HELP_PANESPLIT,     TButtonGadget::Command)) ;
  if ((_uButtonsStyle & MYWS_PREVIOUS) == MYWS_PREVIOUS)
  	Insert(*new TButtonGadget(IDC_PREVIOUS_PANESPLIT, IDC_PREVIOUS_PANESPLIT, TButtonGadget::Command)) ;
}

int
NSGadgetWindow::getGadgetsHeight()
{
	int iMaxGadgetHeight = 0 ;

	if (_pOK)
		return 20 ;

	TGadget* pGadget = FirstGadget() ;

	while (pGadget)	{
		ClassLib::TRect& boundsRect = pGadget->GetBounds() ;

    if (boundsRect.Height() > iMaxGadgetHeight)
    	iMaxGadgetHeight = boundsRect.Height() ;

		pGadget = NextGadget(*pGadget) ;	}

	return iMaxGadgetHeight ;
}

void
NSGadgetWindow::enableCommandForGadgets()
{
	TGadget* pGadget = FirstGadget() ;

	while (pGadget)	{
  	TButtonGadget* pButtonGadget = TYPESAFE_DOWNCAST(pGadget, TButtonGadget) ;
		if (pButtonGadget)
  		pButtonGadget->CommandEnable() ;

    pGadget = NextGadget(*pGadget) ;
	}
}

bool
NSGadgetWindow::IdleAction(long idleCount)
{
  return TGadgetWindow::IdleAction(idleCount) ;
}

void
NSGadgetWindow::CeHasChildOK(TCommandEnabler& commandHandler)
{
	commandHandler.Enable((_uButtonsStyle & MYWS_OK) == MYWS_OK) ;
}

void
NSGadgetWindow::CeHasChildCancel(TCommandEnabler& commandHandler)
{
	commandHandler.Enable((_uButtonsStyle & MYWS_CANCEL) == MYWS_CANCEL) ;
}

void
NSGadgetWindow::CeHasChildHelp(TCommandEnabler& commandHandler)
{
	commandHandler.Enable((_uButtonsStyle & MYWS_HELP) == MYWS_HELP) ;
}

void
NSGadgetWindow::CeHasChildAdd(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildClose(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildSuppress(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildChange(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildContinue(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildPrevious(TCommandEnabler& commandHandler)
{
	commandHandler.Enable((_uButtonsStyle & MYWS_PREVIOUS) == MYWS_PREVIOUS) ;
}

void
NSGadgetWindow::CeHasChildFct1(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildFct2(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildFct3(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildFct4(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildFct5(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildFct6(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildFct7(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildFct8(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

void
NSGadgetWindow::CeHasChildFct9(TCommandEnabler& commandHandler)
{
	commandHandler.Enable(true) ;
}

TResult
NSGadgetWindow::EvNotify(uint id, TNotify far& notifyInfo)
{
	if (notifyInfo.code != TTN_NEEDTEXT)
		return TGadgetWindow::EvNotify(id, notifyInfo) ;

  TTooltipText* pTTText = (TTooltipText*)(&notifyInfo) ;
  if (NULL == pTTText)
  	return TGadgetWindow::EvNotify(id, notifyInfo) ;

  UINT nID = notifyInfo.idFrom ;

	string sText = string("") ;

	switch(nID)
  {
  	case CM_GENERAL_ADD      : sText = _sTextForCmAdd ;      break ;
  	case CM_GENERAL_CLOSE    : sText = _sTextForCmClose ;    break ;
  	case CM_GENERAL_MODIFY   : sText = _sTextForCmChange ;   break ;
  	case CM_GENERAL_REMOVE   : sText = _sTextForCmSuppress ; break ;
  	case CM_GENERAL_CONTINUE : sText = _sTextForCmContinue ; break ;
  	case CM_GENERAL_FCT1     : sText = _sTextForCmFct1 ;     break ;
    case CM_GENERAL_FCT2     : sText = _sTextForCmFct2 ;     break ;
    case CM_GENERAL_FCT3     : sText = _sTextForCmFct3 ;     break ;
    case CM_GENERAL_FCT4     : sText = _sTextForCmFct4 ;     break ;
    case CM_GENERAL_FCT5     : sText = _sTextForCmFct5 ;     break ;
    case CM_GENERAL_FCT6     : sText = _sTextForCmFct6 ;     break ;
    case CM_GENERAL_FCT7     : sText = _sTextForCmFct7 ;     break ;
    case CM_GENERAL_FCT8     : sText = _sTextForCmFct8 ;     break ;
    case CM_GENERAL_FCT9     : sText = _sTextForCmFct9 ;     break ;
  }

  if (string("") == sText)
  	return TGadgetWindow::EvNotify(id, notifyInfo) ;

	strcpy(pTTText->lpszText, sText.c_str()) ;

	return true ;
}

//------------------------------------------------------------------------------
//                      classe  NSPaneSplitter
//------------------------------------------------------------------------------
int FillSplitterWindow(TWindow &p, void* pSplitWinds)
{
	if (NULL == pSplitWinds)
		return 0 ;

	NSMUEView* pWndChild = TYPESAFE_DOWNCAST(&p, NSMUEView) ;
  if ((NSMUEView*) NULL == pWndChild)
  {
  	//
  	// Pour les CQvues
  	//
  	NSDialog* pDlg = TYPESAFE_DOWNCAST(&p, NSDialog) ;
		if (pDlg)
  		pWndChild = TYPESAFE_DOWNCAST(pDlg->_pView, NSMUEView) ;
  }

	if (pWndChild)
	{
    ArrayMUEView* paSplitWind = (ArrayMUEView *)pSplitWinds ;
    paSplitWind->push_back(pWndChild) ;
    return 1 ;
	}

	return 1 ;
}

#define SPLITTER(x) TYPESAFE_DOWNCAST(x,TSplitter)
#define LAYOUTWINDOW(x) TYPESAFE_DOWNCAST(x,TLayoutWindow)

DEFINE_RESPONSE_TABLE1(NSPaneSplitter, TPaneSplitter)
	// EV_COMMAND(IDOK,  		CmChildOK),
	// EV_COMMAND(IDHELP,   CmChildHelp),
	// EV_COMMAND(IDCANCEL, CmChildCancel),
	EV_WM_CLOSE,
END_RESPONSE_TABLE;

 //splitDir = psVertical, psHorizontal	  ou psNone

NSPaneSplitter::NSPaneSplitter(NSContexte* pCtx,TWindow* parent, const char far* title,TModule* module )
               :TPaneSplitter(parent, title, 0, module),
                NSRoot(pCtx)
{
try
{
	Attr.Style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN) ;

  Pere = parent ;

  _aSplitWindows       = new ArrayMUEView ;
  _pMDIChild           = (TMDIChild*) 0 ;
  _pGadgetPanelWindow  = (NSGadgetWindow*) 0 ;
  _pCurrentFocusedView = (NSMUEView*) 0 ;
}
catch (...)
{
	erreur("Exception (NSPaneSplitter ctor).", standardError, 0) ;
}
}

//---------------------
NSPaneSplitter::~NSPaneSplitter()
{
	if (_aSplitWindows)
  {
    // Must delete splitted windows because OWL's garbage collector does it
    // very late
    //
    if (false == _aSplitWindows->empty())
    {
	    for (ArrayMUEViewIter iter = _aSplitWindows->begin() ; _aSplitWindows->end() != iter ; )
	    {
        delete *iter ;
        _aSplitWindows->erase(iter) ;
      }
    }

    delete _aSplitWindows ;
  }

	// Done by OWL's garbage collector
	// delete pGadgetPanelWindow ;
}

//
void
NSPaneSplitter::SetupWindow()
{
	TPaneSplitter::SetupWindow() ;

 // SplitPane(pM, pS, spDir);
  //SplitPane(pTWinF1, pTWinF2, spDir);
  //SplitPane(pTWinF1, pTWinF3, psHorizontal );
}

//Trouver le pointeur vers le nom du document d'une vue

int EnumPaneConcernChanged(TWindow &p, void* pstrConcern)
{
	NSMUEView* pView = TYPESAFE_DOWNCAST(&p, NSMUEView) ;
	if (NULL == pView)
		return 1 ;

	string* pConcern = (string *) pstrConcern ;
	pView->concernChanged(*pConcern) ;

	return 1 ;
}

void
NSPaneSplitter::concernChanged(string sConcern)
{
	ForEachPane(EnumPaneConcernChanged, (void*)&sConcern) ;
}

bool
NSPaneSplitter::HasPane(TWindow* p)
{
	if (NULL == p)
		return false ;

	TWindow* parent = p->Parent ;
	while (parent)
	{
		if (parent == this)
    	return true ;
		parent = parent->Parent ;
	}
	return false ;
}

//
void
NSPaneSplitter::CleanupWindow()
{
	// C'est le bon moment pour recenser les objets ouverts
  //
	_aSplitWindows->vider() ;
	ForEachPane(FillSplitterWindow, (void*) _aSplitWindows) ;

	TPaneSplitter::CleanupWindow() ;
}

void
NSPaneSplitter::SetFrameTitle(string sFunction, string sTitle)
{
	if ((string("") == sFunction) && (string("") == sTitle))
		return ;

	if (string("") != sTitle)
	{
		_pMDIChild->SetCaption(sTitle.c_str()) ;
		return ;
	}

	string sFctLib = pContexte->getSuperviseur()->getText("functions", sFunction) ;
	_pMDIChild->SetCaption(sFctLib.c_str()) ;
}

void
NSPaneSplitter::GetDefLM(TLayoutMetrics& lm)
{
	lm.SetMeasurementUnits(lmPixels) ;
	lm.X.SameAs(lmParent, lmLeft) ;
	lm.Y.SameAs(lmParent, lmTop) ;
	lm.Width.Set(lmRight, lmSameAs, lmParent, lmRight) ;
	lm.Height.Set(lmBottom, lmSameAs, lmParent, lmBottom) ;
}

bool
NSPaneSplitter::nsSplitPane(TWindow* target, TWindow* newPane,
                         TSplitDirection splitDir, float percent)
{
	PRECONDITION(target) ;
	PRECONDITION(target != newPane) ;

	NSDialog* pDlgPane = (NSDialog*) 0 ;
	if (newPane)
	{
		pDlgPane = dynamic_cast<NSDialog*>(newPane) ;
		if (pDlgPane)
    	pDlgPane->_bCanMove = true ;
	}

	NSDialog* pDlgTarget = (NSDialog*) 0 ;
	if (target)
	{
		pDlgTarget = dynamic_cast<NSDialog*>(target) ;
		if (pDlgTarget)
			pDlgTarget->_bCanMove = true ;
	}

	if (GetFirstChild() == (TWindow*) 0)
	{     // if no panes.
		TLayoutMetrics lmOfTarget ;

		GetDefLM(lmOfTarget) ;
		target->SetParent(this) ;
		//NSCQVue* pCQVue = TYPESAFE_DOWNCAST(target, NSCQVue) ;
		if (false == target->IsWindow())
    {
      string sMsg = string("PaneSplitter calls Create on target.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

			target->Create() ;
    }

		SetChildLayoutMetrics(*target, lmOfTarget) ;
		Layout() ;
	}

	if (newPane)
	{
		if (false == HasPane(target))
    	return false ;

		TSplitter*     splitter ;
		TLayoutWindow* parentSplitter = LAYOUTWINDOW(target->Parent) ;

		if (parentSplitter == this)
		{   // if first split.
    	if (splitDir == psHorizontal)
      	splitter = new THSplitter(target->Parent, this, percent) ;
      else
      	splitter = new TVSplitter(target->Parent, this, percent) ;
      splitter->Create() ;
      splitter->SetPercent(percent) ;
      /* target->SetParent(splitter);
         newPane->SetParent(splitter);  */
      TLayoutMetrics lm = splitter->Setup(target, newPane, percent) ;
      //TLayoutMetrics lm = this->PaneSetup((TSplitter*)splitter,target, newPane, splitDir,percent);
      parentSplitter->SetChildLayoutMetrics(*splitter, lm) ;
    }
    else
    {
    	// Ask splitter to split itself.
      //
      SPLITTER(parentSplitter)->Split(target, newPane, splitDir, percent) ;
    }
    parentSplitter->Layout() ;
	}

	if (pDlgPane)
		pDlgPane->_bCanMove = false ;
	if (pDlgTarget)
		pDlgTarget->_bCanMove = false ;

	return true ;
}

/*
//------------------------------------------------------------------------------
//                  redimensionne un panesplitter deja existant
//------------------------------------------------------------------------------
void
NSPaneSplitter::redimensioneSplitPane(TWindow* target, TWindow* newPane,
                         TSplitDirection splitDir, float percent)
{

    PRECONDITION(target);
    PRECONDITION(target != newPane);
    if (!HasPane(target))
            return;

    TSplitter*      splitter;
    TLayoutWindow*  parentSplitter = LAYOUTWINDOW(target->Parent);
    if (parentSplitter == this)
    {
         if (!newPane)
         {
            TLayoutMetrics  lmOfTarget;
            GetDefLM(lmOfTarget);
            SetChildLayoutMetrics(*target, lmOfTarget);
            Layout();
         }
         else
         {
            TLayoutMetrics lm = splitter->Setup(target, newPane, percent);
         }

    }


}    */



//-----------------------------------------------------------------------------
//pour trouver toutes les fenetre qui ont ete formees en coupant cette fenetre
//------------------------------------------------------------------------------
int EnumPaneSplitterWindow(TWindow &p, void* splitWinds)
{
	NSMUEView* wndChild = TYPESAFE_DOWNCAST(&p, NSMUEView) ;

	ArrayMUEView* aSplitWind = (ArrayMUEView *) splitWinds ;
	if (aSplitWind->empty())
		return 0 ;

	ArrayMUEViewIter iter ;
	for (iter = aSplitWind->begin() ; aSplitWind->end() != iter ; iter++)
	{
  	if ((wndChild)&& ((*iter) == wndChild->getSplittedWindow()))
    {
    	aSplitWind->push_back(wndChild) ;
      return 0 ;
    }
	}
	return 1 ;
}

ClassLib::TRect
NSPaneSplitter::computeMetaRectWindow(NSMUEView *pwndView)
{
try
{
  if ((NULL == pwndView) || (NULL == _aSplitWindows))
    return ClassLib::TRect(0, 0, 0, 0) ;

	_aSplitWindows->vider() ;
	_aSplitWindows->push_back(pwndView) ;

	ForEachPane(EnumPaneSplitterWindow, (void*) _aSplitWindows) ;

	ClassLib::TRect rectInt = pwndView->GetWindowRect() ;

	ArrayMUEViewIter iter ;
	for (iter = _aSplitWindows->begin() ; _aSplitWindows->end() != iter ; iter++)
	{
  	if ((*iter)->IsWindow())
    {
    	ClassLib::TRect rectComp = (*iter)->GetWindowRect() ;

      rectInt.top    = min(rectInt.Top(), rectComp.Top()) ;
      rectInt.left   = min(rectInt.Left(), rectComp.Left()) ;
      rectInt.right  = max(rectInt.Right(), rectComp.Right()) ;
      rectInt.bottom = max(rectInt.Bottom(), rectComp.Bottom()) ;
    }
	}

	return rectInt ;
}
catch (...)
{
	erreur("Exception (NSPaneSplitter::computeMetaRectWindow).", standardError, 0) ;
  return ClassLib::TRect(0, 0, 0, 0) ;
}
}

void
NSPaneSplitter::closeAllWindows()
{
	//void* param;
	// ForEachPane(EnumWindowsForClose, param) ;

	//
	// Les objets ouverts ont été recensés dans CleanupWindow(), qui a
	// fermé les fenêtres ; il ne reste plus qu'à détruire les objets
	//
	if ((NULL == _aSplitWindows) || (_aSplitWindows->empty()))
		return ;

	ArrayMUEViewIter iter = _aSplitWindows->begin() ;

	for ( ; _aSplitWindows->end() != iter ; )
	{
  	// (*iter)->Destroy() ;  // la fenêtre a été fermée dans CleanupWindow()

    // Prevent TApplication from deleting the TWindow
    // pContexte->getSuperviseur()->getApplication()->Uncondemn(*iter) ;

    delete (*iter) ;
    _aSplitWindows->erase(iter) ;
	}
}

//------------------------------------------------------------------------------

//Trouver le pointeur vers la fenetre pour laquelle on connait la fonction

int EnumPaneFonctionWindow(TWindow &p, void* paramEnum)
{
	NSMUEView* wndMother = TYPESAFE_DOWNCAST(&p, NSMUEView) ;
	StructEnumPane* pEnum = (StructEnumPane*)(paramEnum) ;
	// NSMUEView* wndMother = TYPESAFE_DOWNCAST(wndChild->getSplittedWindow(), NSMUEView) ;
	if ((wndMother) && (wndMother->getFunction() == pEnum->getString()))
	{
		pEnum->setWindow(wndMother) ;
		return 0 ;
	}
	return 1 ;
}

//donne le pointer vers la mere (fenetre qui sera coupée) d'une fenetre qui doit s'ouvrir
NSMUEView*
NSPaneSplitter::researchMotherWindow(string sWndChild, string sWndCreateMDIChild)
{
	NSWindowProperty *pWinProp = pContexte->getUtilisateur()->getWindowProperty(sWndCreateMDIChild) ;
  ArraySplitWindow* pSplitWins = pWinProp->getSplit() ;
  if (pSplitWins->empty())
  	return NULL ;

  StructEnumPane paramEnum ;
  ArraySWIter iterSplitWinds ;
  for (iterSplitWinds = pSplitWins->begin() ;
          (pSplitWins->end() != iterSplitWinds) &&
          ((*iterSplitWinds)->getChildWindow() != sWndChild) ;
              iterSplitWinds++ ) ;

  if (pSplitWins->end() != iterSplitWinds)
  {
  	paramEnum.setString((*iterSplitWinds)->getMotherWindow()) ;

    struct date date1 ;
    struct time time1 ;
    getdate(&date1) ;
    gettime(&time1) ;

    do
    {
    	ForEachPane(EnumPaneFonctionWindow, (void*) &paramEnum ) ;
      pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
    }
    while ((paramEnum.getWindow() == NULL) && (!pContexte->getSuperviseur()->TimeFuture(date1, time1, 100))) ;
  }

  return paramEnum.getWindow() ;
}


/*VecteurString
*NSPaneSplitter::researchChildWindFonction(string sWndCreateMDIChild) */
void
NSPaneSplitter::researchChildWindFonction(string sWndCreateMDIChild, VecteurString *pChildWindsFct)

{
  //VecteurString     *pChildWindsFct = new VecteurString() ;

  NSWindowProperty *pWinProp = pContexte->getUtilisateur()->getWindowProperty(sWndCreateMDIChild) ;
  if (NULL == pWinProp)
    return ;

  ArraySplitWindow *pSplitWins = pWinProp->getSplit() ;
  if ((NULL == pSplitWins) || pSplitWins->empty())
    return ;

  ArraySWIter iterSplitWinds = pSplitWins->begin() ;
  for ( ; pSplitWins->end() != iterSplitWinds ; iterSplitWinds++)
  // if (iterSplitWinds != pSplitWins->end())
  {
    EquiItemIter iterChildList;
    if (false == pChildWindsFct->empty())
    {
      for ( iterChildList = pChildWindsFct->begin() ;
            (pChildWindsFct->end() != iterChildList) && ((**iterChildList) != (*iterSplitWinds)->getChildWindow()) ;
            iterChildList++)
        ;
    }

    if ((pChildWindsFct->empty()) || (pChildWindsFct->end() == iterChildList))
    {
      string sWndChild = (*iterSplitWinds)->getChildWindow() ;
      pChildWindsFct->AddString(sWndChild) ;
    }
  }

 // return pChildWindsFct ;
}


//Trouver le pointeur vers le nom du document d'une vue

int EnumPaneViewDocument(TWindow &p, void* paramEnum)
{
	NSMUEView* wndMother = TYPESAFE_DOWNCAST(&p, NSMUEView) ;
	StructEnumPane* pEnum = (StructEnumPane*)(paramEnum) ;
	if ((wndMother) && (wndMother->getDocType() == pEnum->getString()))
	{
		pEnum->setWindow(wndMother) ;
		return 0 ;
	}
	return 1 ;
}

NSMUEView*
NSPaneSplitter::researchDocument(string sDocType)
{
	StructEnumPane* paramEnum = new StructEnumPane() ;
	paramEnum->setString(sDocType) ;
	ForEachPane(EnumPaneViewDocument, (void*)paramEnum ) ;

	return paramEnum->getWindow() ;
}

//on cherche toutes les fenetres qui ont des buttons OK ou Cancel

//int EnumPaneRechercheButtons(TWindow &p, void* paramEnum)
/*int EnumPaneRechercheButtons(TWindow &p, void* param)
{
   // StructEnumPane* pEnum = (StructEnumPane*)(paramEnum) ;
    NSMUEView* pWndCurr = TYPESAFE_DOWNCAST(&p, NSMUEView) ;
    if (pWndCurr)
        return 1;
    //uint32 uStyleBut = pEnum->getUParam() ;
    uint32* uStyleBut = (uint32*)param ;
    //uint32  styleButton = (*uStyleBut);
    uint32  styleButton = pWndCurr->uButtonsStyle;

    uint32  styleOK = styleButton|MYWS_OK;
    uint32  styleCancel = styleButton|MYWS_CANCEL;

    if (((*uStyleBut) == MYWS_OK)&&(styleButton == styleOK))
    {
       pWndCurr->CmOk();
    }
    else
        if  (((*uStyleBut) == MYWS_CANCEL)&&(styleButton == styleCancel))
        {
            pWndCurr->CmCancel();
        }
   // else

    return 1 ;
}  */


int EnumPaneRechercheButtons(TWindow &p, void* param)
{
  NSMUEView* pMUEWnd   = TYPESAFE_DOWNCAST(&p, NSMUEView) ;
  NSDialog*  pDiagCurr = TYPESAFE_DOWNCAST(&p, NSDialog) ;
  // NSCQVue*    pCQVue = TYPESAFE_DOWNCAST(&p, NSCQVue) ;
  /*if ((!pMUEWnd)||(!pDiagCurr))
        return 1;   */
  string* sButt = (string*)param ;
  if (pMUEWnd)
  {
    uint32 styleButton = pMUEWnd->uButtonsStyle ;

    uint32 styleOK     = styleButton|MYWS_OK ;
    uint32 styleCancel = styleButton|MYWS_CANCEL ;

    //uint32  styleButton = (*uStyleBut);
    if (((*sButt) == "OK") && (styleOK == styleButton))
    {
      if (pMUEWnd->pDialog)
        pMUEWnd->pDialog->CmOk() ;
      else
        pMUEWnd->CmOk() ;
    }
    if (((*sButt) == "Cancel")&& (styleCancel == styleButton))
    {
      if (pMUEWnd->pDialog)
        pMUEWnd->pDialog->CmCancel() ;
      else
        pMUEWnd->CmCancel() ;
    }
  }
  if (pDiagCurr)
  {
    if((*sButt) == "OK")
      pDiagCurr->CmOk() ;
    if ((*sButt) == "Cancel")
      pDiagCurr->CmCancel() ;
  }
   // else

  return 1 ;
}

int EnumPaneCanCloseWindows(TWindow& win, void* param )
{
	if (!(&win)->CanClose())
	{
		bool* n = (bool*)param ;
    (*n) = false ;
    return 0 ;
	}

	return 1 ;
}

void
NSPaneSplitter::youCanCloseNow()
{
	bool bCanClose = true ;

	ForEachPane(EnumPaneCanCloseWindows, (void*)&bCanClose ) ;

	if (bCanClose)
	{
  	// ForEachPane(EnumPaneCloseWindows, (void*)&bCanClose) ;

    // ::SendMessage(pMDIChild->GetHandle(), WM_DESTROY, 0, 0);
    if (_pMDIChild)
    	::SendMessage(_pMDIChild->GetHandle(), WM_CLOSE, 0, 0) ;
	}
}

void
NSPaneSplitter::CmOk()
{
	int iPaneCount = PaneCount() ;
  if (0 == iPaneCount)
		return ;

	string sName = "OK" ;
	// message à mdichild pour fermer les fenetres
	ForEachPane(EnumPaneRechercheButtons, (void*)&sName ) ;

	bool bCanClose = true ;

	ForEachPane(EnumPaneCanCloseWindows, (void*)&bCanClose ) ;

	if (bCanClose)
	{
		// ForEachPane(EnumPaneCloseWindows, (void*)&bCanClose) ;

		// ::SendMessage(pMDIChild->GetHandle(), WM_DESTROY, 0, 0);
		if (_pMDIChild)
			::SendMessage(_pMDIChild->GetHandle(), WM_CLOSE, 0, 0) ;
	}
}

void
NSPaneSplitter::CmHelp()
{
	pContexte->setAideIndex("h_index.htm") ;
	pContexte->setAideCorps("h_dospat.htm") ;
	pContexte->NavigationAideEnLigne() ;
}

void
NSPaneSplitter::CmCancel()
{
	string sName = "Cancel" ;
	ForEachPane(EnumPaneRechercheButtons, (void*)&sName ) ;
  //message à mdichild pour fermer les fenetres
	bool bCanClose = true ;

	ForEachPane(EnumPaneCanCloseWindows, (void*)&bCanClose ) ;
	if (bCanClose)
		::SendMessage(_pMDIChild->GetHandle(), WM_CLOSE, 0, 0) ;
}

//
// Donne le focus à la vue qui suit pCurrentView
// Set the focus to the view that follows pCurrentView
//
void
NSPaneSplitter::setFocusToNextView(NSMUEView* pCurrentView, HWND hWndLostFocus)
{
	// On recense les fenêtres ouvertes
	// Enumerating open windows
	//
	_aSplitWindows->vider() ;
	ForEachPane(FillSplitterWindow, (void*) _aSplitWindows) ;

	if (_aSplitWindows->empty())
		return ;

	// Par convention, si pas de pCurrentView, on donne le focus à la première
	// By convention, if no pCurrentView, focus is given to first window
	//
	if (NULL == pCurrentView)
	{
  	NSMUEView* pNextView = *(_aSplitWindows->begin()) ;
    // pNextView->SetFocus(hWndLostFocus) ;
    pNextView->EvSetFocus(hWndLostFocus) ;
	}

	// On cherche la fenêtre en cours
  // Looking for the current window
  //
  ArrayMUEViewIter iter ;
  for (iter = _aSplitWindows->begin() ; (_aSplitWindows->end() != iter) &&
                                            (*iter != pCurrentView) ; iter++) ;

	// On prend la suivante
	// Taking next
	//
	if (_aSplitWindows->end() != iter)
		iter++ ;

	NSMUEView* pNextView ;
	if (_aSplitWindows->end() == iter)
		pNextView = *(_aSplitWindows->begin()) ;
	else
		pNextView = *iter ;

	if ((pNextView) && (pNextView->IsWindow()))
		// pNextView->SetFocus(hWndLostFocus) ;
		pNextView->EvSetFocus(hWndLostFocus) ;
}

void
NSPaneSplitter::setFocusToPrevView(NSMUEView* pCurrentView, HWND hWndLostFocus)
{
	// On recense les fenêtres ouvertes
	// Enumerating open windows
	//
	_aSplitWindows->vider() ;
	ForEachPane(FillSplitterWindow, (void*) _aSplitWindows) ;

	if (_aSplitWindows->empty())
		return ;

	// On cherche la fenêtre en cours
	// Looking for the current window
	//
	ArrayMUEViewIter iter ;
	for (iter = _aSplitWindows->begin() ; (_aSplitWindows->end() != iter) &&
                                            (*iter != pCurrentView) ; iter++) ;
	if (_aSplitWindows->end() == iter)
		return ;

	// On prend la précédente
	// Taking previous
	//
	NSMUEView* pNextView ;
	if (_aSplitWindows->begin() == iter)
		pNextView = _aSplitWindows->back() ;
	else
	{
  	iter-- ;
    pNextView = *iter ;
	}

	if ((pNextView) && (pNextView->IsWindow()))
		// pNextView->SetFocus(hWndLostFocus) ;
		pNextView->EvSetFocus(hWndLostFocus) ;
}

void
NSPaneSplitter::activateParent()
{

  HWND hForeGround = ::GetForegroundWindow() ;
	// HWND hMainWindow = pContexte->GetMainWindow()->GetHandle() ;

  /* bool bIsChild = */ pContexte->GetMainWindow()->IsChild(hForeGround) ;

//  if ((hForeGround != hMainWindow) && (!(::IsChild(hMainWindow, hForeGround))))
//  	pContexte->GetMainWindow()->BringWindowToTop() ;


	_pMDIChild->PostMessage(WM_CHILDACTIVATE) ;

	// pMDIChild->BringWindowToTop();
}

TGadgetWindow*
NSPaneSplitter::getGadgetWindow()
{
	if (_pGadgetPanelWindow)
		return _pGadgetPanelWindow ;

	TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (pMyApp)
		return pMyApp->getSecondaryControlBar() ;

	return (TGadgetWindow*) 0 ;
}

void
NSPaneSplitter::FlushControlBar()
{
	TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
	if (pMyApp)
		pMyApp->FlushControlBar() ;

	if (_pGadgetPanelWindow)
		_pGadgetPanelWindow->FlushControlBar() ;
}

void
NSPaneSplitter::Insert(TGadget& gadget, TGadgetList::TPlacement iPlac, TGadget* sibling)
{
	if (_pGadgetPanelWindow)
	{
		_pGadgetPanelWindow->Insert(gadget, iPlac, sibling) ;
    return ;
	}

	TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (pMyApp)
		pMyApp->getSecondaryControlBar()->Insert(gadget, iPlac, sibling) ;
}

void
NSPaneSplitter::SetTootipText(int gadgetId, string sText)
{
  if ((NSGadgetWindow*) NULL == _pGadgetPanelWindow)
    return ;

  TTooltip* pTooltip = _pGadgetPanelWindow->GetTooltip() ;
  if ((TTooltip*) NULL == pTooltip)
    return ;

  TGadget* pGadgetForNew = _pGadgetPanelWindow->GadgetWithId(CM_GENERAL_ADD) ;
  if ((TGadget*) NULL == pGadgetForNew)
    return ;

  TToolInfo ti ;
  ti.SetToolInfo(_pGadgetPanelWindow->GetHandle(), gadgetId) ;
  ti.SetText(sText.c_str() /*, bool copy = true*/) ;

  pTooltip->UpdateTipText(ti) ;
}

void
NSPaneSplitter::LayoutSession()
{
	if (_pGadgetPanelWindow)
	{
		_pGadgetPanelWindow->LayoutSession() ;
    _pGadgetPanelWindow->enableCommandForGadgets() ;

    return ;
	}

	TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (pMyApp)
		pMyApp->getSecondaryControlBar()->LayoutSession() ;
}

void
NSPaneSplitter::setGadgetWindowLayout()
{
	if ((NSGadgetWindow*) NULL == _pGadgetPanelWindow)
		return ;

	int iGadgetsHeight = _pGadgetPanelWindow->getGadgetsHeight() ;

  TLayoutMetrics layoutMetrics ;
  layoutMetrics.SetMeasurementUnits(lmPixels) ;
  layoutMetrics.X.Absolute(lmLeft, 1) ;
	layoutMetrics.Y.Absolute(lmTop, 1) ;
  //layoutMetrics.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 0) ;
	//layoutMetrics.Y.Set(lmTop, lmBelow, lmParent, lmTop, 0) ;
	layoutMetrics.Width.Set(lmRight, lmLeftOf, lmParent, lmRight, 1) ;
	layoutMetrics.Height.Absolute(iGadgetsHeight) ;

  SetChildLayoutMetrics(*_pGadgetPanelWindow, layoutMetrics) ;
  Layout() ;
}

//-------------------------------------------
StructEnumPane:: StructEnumPane()
{
	sString = "";
	pWndMUEView = NULL;
}

StructEnumPane::~StructEnumPane()
{

}
