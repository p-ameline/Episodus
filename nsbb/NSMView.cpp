// --------------------------------------------------------------------------// NSSOAPVIEW.CPP		Document/Vues SOAP
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Novembre/Décembre 2001
// source : Fabrice LE PERRU -- nsepisodview.cpp - Aout 2001
//          -- source : Rémi SPAAK - Mai 1997
// --------------------------------------------------------------------------

#include <owl/uihelper.h>
#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <owl\splitter.h>
//#include <nautilus\nsSOAPview.h>
#include <fstream.h>
#include <stdlib.h>

#include <bwcc.h>
#include <owl/button.h>

#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "nsdn\nsdochis.h"
#include "nsbb\nsmview.h"
#include "nsbb\nspanesplitter.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nsarc.h"

#include "nsbb\nsbb.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsedilex.h"
#include "nsbb\nsedit.h"
#include "nsbb\nsdico.h"
#include "nsbb\nsbouton.h"
#include "nsbb\nschkbox.h"
#include "nsbb\nsradiob.h"
#include "nsbb\nsgroupe.h"
#include "nsbb\nsonglet.h"
#include "nsbb\nscombo.h"
#include "nsbb\nslistwind.h"
#include "nsbb\nstabwindow.h"
#include "nsbb\nstooltip.h"
#include "nsbb\nsMdiChild.h"
#include "nsepisod\nsToDo.h"

#include "nssavoir\nsconver.h"

#include "nautilus\nsepicap.h"
#include "nautilus\nsmdiframe.h"
#include "nsldv\nsldvdoc_base.h"
#include "nautilus\nshistdo.h"

#include "ns_ob1\BB1BB.h"

#include "partage\ns_timer.h"
#include "ns_ob1\OB1.rh"
#include "nautilus\nautilus.rh"
#include "nsbb\nsbb.rh"

const char codeMARK = 'O' ;

// --------------------------------------------------------------------------
//
//		   METHODES DES VUES GENERALES DU MODELE Document / Vue de MUE
//
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
//
// Class NSMUEView
//
// --------------------------------------------------------------------------

// Table de réponses
DEFINE_RESPONSE_TABLE1(NSMUEView, TWindowView)
    EV_VN_ISWINDOW,
    EV_WM_SIZE,
    EV_WM_MOVE,
    EV_WM_PAINT,
    EV_WM_WINDOWPOSCHANGED,
    EV_WM_DESTROY,
    EV_WM_KEYDOWN,
    EV_WM_SETFOCUS,
END_RESPONSE_TABLE ;

// ConstructeurNSMUEView::NSMUEView(NSContexte* pCtx, TDocument* doc, TWindow *parent, string sFunction, string sDoc, string sPreoccu)
          :TWindowView(*doc, parent), NSRoot(pCtx)
//            :TView(*doc), TWindow(parent), NSRoot(pCtx)
{
try
{
	pDoc        = doc ;
  _sFonction   = sFunction ;
  _sPreoccup   = sPreoccu ;
  _sDocType    = sDoc ;

  _bFirstSetup = true ;
  //
  // On référence cette fenêtre au niveau du Superviseur
  //

  _bSetupToolBar    = true ;

  _pCreateWindow    = (TWindow *) 0 ;
  _pSplittedWindow  = (TWindow *) 0 ;
  pDialog           = NULL ;
  _splitDir         = psNone ;
  _percent          = 0.0 ;
  uButtonsStyle     = MYWS_NONE ;

  _pMUEViewMenu     = (TMenuDescr *) 0 ;

  _bClosing         = false ;

  _pCreatedMDIChild = (NSMDIChild*) 0 ;
}
catch (...)
{
	erreur("Exception NSMUEView ctor.", standardError, 0) ;
}
}

// DestructeurNSMUEView::~NSMUEView()
{
  string ps = string("Deleting NSMUEView.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  if (pContexte->getBBinterface())
  {
    char szHandle[20] ;
    sprintf(szHandle, "%p", HWindow) ;
    string ps = string("NSMUEView destructor: disconnecting OB1 interface for ") + string(szHandle) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    pContexte->getBBinterface()->DisconnectInterface(HWindow) ;
  }

	if (_pMUEViewMenu)
  	delete _pMUEViewMenu ;
}

void
NSMUEView::concernChanged(string sConcern)
{
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)TWindow*
NSMUEView::GetWindow()
{
	return (TWindow*) this;
}

// Appel de la fonction de remplissage de la vue
void
NSMUEView::SetupWindow()
{
try
{
  //TWindowView::SetupWindow();
  TWindow::SetupWindow() ;

  // Window posit is set by TMyApp::EvNewView
  // SetWindowPosit() ;

  _bFirstSetup = false ;
}
catch (...)
{
	erreur("Exception NSMUEView::SetupWindow.", standardError, 0) ;
}
}

void
NSMUEView::InitBasicMetrics(Cdialogbox *pDialogBox)
{
  if ((Cdialogbox*) NULL == pDialogBox)
    return ;

  // Doesn't work properly because it is based on a claimed "system font"
  // that is seldom used actually
  //
  // cxSysChar = LOWORD(GetDialogBaseUnits()) ;
  // cySysChar = HIWORD(GetDialogBaseUnits()) ;

  TScreenDC screenDC ;
  SetFont(screenDC, pDialogBox) ;

  TEXTMETRIC tm ;
  screenDC.GetTextMetrics(tm) ;

  _cySysChar = tm.tmHeight ;

  ClassLib::TSize size = screenDC.GetTextExtent(
           "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52) ;
  _cxSysChar = (size.X() / 26 + 1) / 2 ;

  string ps = string("NSBBMUEView::InitMetrics") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  ps = string(" --> cxSysChar = ") + IntToString(_cxSysChar) + string(" ; cySysChar = ") + IntToString(_cySysChar) ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  ps = string(" --> tmAveCharWidth = ") + IntToString(int(tm.tmAveCharWidth)) + string(" ; tmMaxCharWidth = ") + IntToString(int(tm.tmMaxCharWidth)) ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSMUEView::SetFont(TDC& dc, Cdialogbox *pDialogBox)
{
  if ((Cdialogbox*) NULL == pDialogBox)
    return ;

  LOGFONT logFont ;
  logFont.lfHeight         = 8 ;
	logFont.lfWidth          = 0 ;
	logFont.lfEscapement     = 0 ;
	logFont.lfOrientation    = 0 ;
	logFont.lfWeight         = FW_NORMAL ;
	logFont.lfItalic         = 0 ;
	logFont.lfUnderline      = 0 ;
	logFont.lfStrikeOut      = 0 ;
	logFont.lfCharSet        = ANSI_CHARSET ;
	logFont.lfOutPrecision   = OUT_TT_PRECIS ;
	logFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
	logFont.lfQuality        = PROOF_QUALITY ;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN ;
	strcpy(logFont.lfFaceName, "MS Sans Serif") ;

  string sFontType = pDialogBox->getFontType() ;
  if (string("") != sFontType)
    strcpy(logFont.lfFaceName, sFontType.c_str()) ;

  string sFontSize = pDialogBox->getFontSize() ;
  if (string("") != sFontSize)
    logFont.lfHeight = (LONG) atoi(sFontSize.c_str()) ;

  dc.SelectObject(TFont(&logFont)) ;
}

bool
NSMUEView::isControlProperty(TWindow *pCtrl, uint32 StyleInfo)
{
  if ((TWindow*) NULL == pCtrl)
    return false ;

  return ((pCtrl->GetStyle() & StyleInfo) == StyleInfo) ;
}

// Fonction CanClose : Appel de CanClose du documentbool
NSMUEView::CanClose()
{
  _bClosing = true ;

	TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->FlushControlBar() ;
  _bSetupToolBar = false ;

  return true ;
}

//
// Does a given HWND belong to this view? Yes if it is us, or a child of us
//
boolNSMUEView::VnIsWindow(HWND hWnd)
{
	return (hWnd == GetHandle()) || IsChild(hWnd) ;
}

bool
NSMUEView::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  // return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;
  /* bool bTranslate = */ _hAccelerator ? ::TranslateAccelerator(GetHandle(), _hAccelerator, &msg) : false ;

  return TWindowView::PreProcessMsg(msg) ;
}

void
NSMUEView::addConcernTitle()
{
	if (string("") == _sPreoccup)
		return ;

	NSLdvDocumentBase *pLdvDoc = (NSLdvDocumentBase*) pContexte->getPatient()->getLdvDocument() ;
  if ((NSLdvDocumentBase*) NULL == pLdvDoc)
    return ;

  NSFrameInformationArray *pFrames = pLdvDoc->getFrames() ;
  if (((NSFrameInformationArray*) NULL == pFrames) || pFrames->empty())
    return ;

  for (FrameInfoIter i = pFrames->begin() ; pFrames->end() != i ; i++)
  {
    ArrayConcern *pConcerns = (*i)->getConcernsArray() ;
    NSConcern* pConcern = pConcerns->getConcern(_sPreoccup) ;
    if (pConcern)
    {
      _sViewName += string(" - ") + pConcern->getTitle() ;
      return ;
    }
  }
}

voidNSMUEView::EvDestroy()
{
  // RecordWindowPosit();
}

void
NSMUEView::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if      (VK_TAB == key)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	_pPaneSplitter->setFocusToPrevView(this, HWindow) ;
    else
    	_pPaneSplitter->setFocusToNextView(this, HWindow) ;
  }
  else
  	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}

void
NSMUEView::EvSetFocus(HWND hWndLostFocus)
{
	_pPaneSplitter->_pCurrentFocusedView = this ;

	TWindowView::EvSetFocus(hWndLostFocus) ;

  activateMUEViewMenu() ;
}

void
NSMUEView::setFocusToNextSplitterView()
{
	_pPaneSplitter->setFocusToNextView(this, HWindow) ;
}

void
NSMUEView::setFocusToPrevSplitterView()
{
	_pPaneSplitter->setFocusToPrevView(this, HWindow) ;
}

void
NSMUEView::SetWindowPosit()
{
  NSMDIChild* pParentMdiChild = getCreatedMdiChild() ;
  if ((NSMDIChild*) NULL == pParentMdiChild)
    return ;

	pParentMdiChild->Show(SW_HIDE) ;

  NSWindowProperty* pWinProp = pContexte->getUtilisateur()->getWindowProperty(_sFonction) ;
  if (pWinProp)
	{
    NS_CLASSLIB::TRect targetRect = pWinProp->getRect(pContexte->getSuperviseur()) ;

    TMyApp*      pMyApp     = pContexte->getSuperviseur()->getApplication() ;
    NSMDIClient* pMdiClient = pMyApp->prendClient() ;
    NSMDIFrame*  pMdiFrame  = pMyApp->getFrame() ;

    NS_CLASSLIB::TRect MdiClientRect = pMdiClient->GetWindowRect() ;
    NS_CLASSLIB::TRect MdiFrameRect  = pMdiFrame->GetWindowRect() ;

    // int iLeftDecal = MdiClientRect.Left() - MdiFrameRect.Left() ;
    int iTopDecal = MdiClientRect.Top() - MdiFrameRect.Top() ;

    //
    // fixer la nouvelle position
    // (on ne tient pas compte de la taille, vu le probleme pour restaurer
    //  une fenetre TView,TWindow mise en icone)
    //
    pParentMdiChild->SetWindowPos(0, targetRect.Left(), targetRect.Top() + iTopDecal, targetRect.Width(), targetRect.Height(), SWP_NOZORDER) ;
  }

  pParentMdiChild->Show(SW_SHOWNORMAL) ;
}

/*void
NSMUEView::RecordWindowPosit()
{
    NS_CLASSLIB::TRect rectDlg = Parent->GetWindowRect();//coordonnées par % à l'écran
    int X,Y,W,H;
    NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y());
    Parent->Parent->ScreenToClient(point);
    X = point.X();
  	Y = point.Y();
    W = rectDlg.Width();
    H = rectDlg.Height();

    if (Parent->IsIconic())
        return ;

    string sUserId = string(pContexte->getUtilisateur()->pDonnees->indice) ;
    NS_CLASSLIB::TRect  rect(X, Y, X + W, Y + H) ;
    pContexte->getUtilisateur()->aWinProp.setProperty(sUserId, sFonction, rect, pContexte->PathName("FGLO")) ;
} */

// fonction permettant de prendre toute la taille de TWindow par la TListWindow
void
NSMUEView::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TWindow::EvSize(sizeType, size) ;
}

voidNSMUEView::EvMove(ClassLib::TPoint& clientOrigin)
{
  TWindow::EvMove(clientOrigin);
}

voidNSMUEView::EvWindowPosChanged(WINDOWPOS far& windowPos)
{
  TWindow::EvWindowPosChanged(windowPos);
}

voidNSMUEView::gotMoved()
{
}

voidNSMUEView::EvPaint(){
	TWindow::EvPaint() ;
}

//   bDependence = true  -> fenetre maitre
//   bDependence = false -> fenetre esclave
//  par defeult la fenetre est maitre

/*void
NSMUEView::setDependance(bool bDep)
{
      bDependence = bDep;
}

bool
NSMUEView:: getDependance()
{
      return  bDependence;
} */


TWindow*
NSMUEView::getCreateWindow()
{
	return _pCreateWindow ;
}

TWindow*
NSMUEView::getSplittedWindow()
{
	return _pSplittedWindow ;
}

void
NSMUEView::newWindow(string sWndChild)
{
	if (string("") == sWndChild)
		return ;

	NSSuper* pSuper = pContexte->getSuperviseur();
	NSToDoTask* pTask = new NSToDoTask;
	pTask->setWhatToDo(string("OpenNewWindow")) ;
	pTask->setPointer1((void*) this, false) ;
	pTask->setParam1(string(sWndChild)) ;

	pTask->sendMe(pSuper) ;
}

void
NSMUEView::focusView()
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;
  string ps = string("NSMUEView is setting focus to window with function ") + _sFonction ;  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;	NSToDoTask* pTask = new NSToDoTask ;
	pTask->setWhatToDo(string("FocusWindow")) ;
	pTask->setPointer1((void*) this, false) ;

	pTask->sendMe(pSuper) ;
}

void
NSMUEView::activate()
{
	if (((NSPaneSplitter*) NULL == _pPaneSplitter) || (NULL == _pPaneSplitter->_pMDIChild))
		return ;

	if (false == _pPaneSplitter->_pMDIChild->IsWindow())
  	return ;

	::SetFocus(_pPaneSplitter->_pMDIChild->GetHandle()) ;
}

bool
NSMUEView::Create()
{
	if (this->IsWindow())
		return false ;
	OWL::TWindow::Create() ;
	return true ;
}

void
NSMUEView::activateParent()
{
	activateMUEViewMenu() ;
	//_pPaneSplitter->pMDIChild->GetHandle()) ;
	_pPaneSplitter->activateParent() ;
  _pPaneSplitter->SetFrameTitle(getFunction(), _sViewName) ;
}

void
NSMUEView::initMUEViewMenu(string sMenuName)
{
	if (_pMUEViewMenu)
  	delete _pMUEViewMenu ;

	nsMenuIniter menuIter(pContexte) ;
	_pMUEViewMenu = new OWL::TMenuDescr ;
  menuIter.initMenuDescr(_pMUEViewMenu, sMenuName) ;
}

void
NSMUEView::activateMUEViewMenu()
{
	if (NULL == _pMUEViewMenu)
		return ;

	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->GetMainWindow()->SetMenuDescr(*_pMUEViewMenu) ;
}

//-----------------------------------------------------------------------//
//				   	             Classe NSLDVView                              //
//                                                                       //
//                    NSMUEView liée à NSLdvDocument                     //
//-----------------------------------------------------------------------//

NSLDVView::NSLDVView(NSContexte* pCtx, TDocument* pDoc, TWindow *parent, string sFunction, string sDoc, string sPreoccu)
          :NSMUEView(pCtx, pDoc, parent, sFunction, sDoc, sPreoccu)
{
}

NSLDVView::~NSLDVView()
{
}

//-----------------------------------------------------------------------//
//				   	           Classe NSBBMUEView                              //
//                                                                       //
//                    NSMUEView liée aux BBItems                         //
//-----------------------------------------------------------------------//

#define ID_TABCTRL 100

DEFINE_RESPONSE_TABLE1(NSBBMUEView, NSMUEView)
  EV_WM_SIZE,
  EV_WM_KEYUP,
  EV_COMMAND(IDM_BBK_EVENT,      CmBbkEvent),
  EV_COMMAND(IDM_BBK_UPDATE,     CmBbkUpdate),
  EV_TCN_SELCHANGE(ID_TABCTRL,	 TabSelChange),
	EV_TCN_SELCHANGING(ID_TABCTRL, TabSelChanging),
	EV_TCN_KEYDOWN(ID_TABCTRL,		 TabKeyDown),
  EV_MESSAGE(WM_CTLCOLORSTATIC,  EvSetBackColor),
  EV_WM_TIMER,
  EV_WM_CTLCOLOR,
END_RESPONSE_TABLE ;

NSBBMUEView::NSBBMUEView(NSContexte* pCtx, TDocument* pDoc, TWindow *parent, string sFunction, string sDoc, string sPreoccu)
            :NSMUEView(pCtx, pDoc, parent, sFunction, sDoc, sPreoccu)
{
  _pBBItem       = (BBItem *) 0 ;
  _pNSCtrl       = (NSControleVector *) 0 ;
  _m_tabCtrl     = (OWL::TTabControl *) 0 ;
  _tabbedWindows = (NSWindowsArray   *) 0 ;
  _bControlInit  = false ;

  _pNSToolTip    = new NSToolTip(pCtx, this) ;
  _pToolTip      = new TTooltip(this) ;
  _pToolTip->Attr.Style |= TTS_BALLOON ;

  _bWaitingForKsAnswer     = false ;
  _bWaitingForSystemAction = false ;
  _bAcceptUpdate           = false ;
  _bNeedToRefresh          = false ;
}

NSBBMUEView::~NSBBMUEView()
{
  char szHandle[20] ;
  sprintf(szHandle, "%p", HWindow) ;
  string ps = string("NSBBMUEView ") + string(szHandle) + string(" entering delete") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  if (_pNSCtrl)
    delete _pNSCtrl ;

  if (_tabbedWindows)
  {
    _tabbedWindows->vider() ;
    delete _tabbedWindows ;
  }

  if (_pToolTip)
    delete _pToolTip ;
  if (_pNSToolTip)
    delete _pNSToolTip ;

  if (_pCreatedMDIChild)
    _pCreatedMDIChild->resetCreateWindow() ;
}

void
NSBBMUEView::SetupWindow()
{
  NSMUEView::SetupWindow() ;

  InitMetrics() ;

  CreerControlesArchetype() ;

  // Hide all windows but the first
	if (_tabbedWindows && (false == _tabbedWindows->empty()))
		for (NSWindowsIter i = _tabbedWindows->begin() ; _tabbedWindows->end() != i ; i++)
    {
      if ((NULL != (*i)->controls) && (false == (*i)->controls->empty()))
        for (NSControlsIter j = (*i)->controls->begin(); (*i)->controls->end() != j ; j++)
        {
          // SetupWindowForControl(*j) ;  // SetupWindow is called during Create()
          (*j)->Create() ;

          bool bChildIsVisible = true ; // (((*j)->GetStyle() & WS_VISIBLE) == WS_VISIBLE) ;
          NSControle* pCtrl = getNSControl(*j) ;
          if (pCtrl)
          {
            bChildIsVisible = pCtrl->isVisible() ;

            // Reference control for ToolTip
            //
            if (string("") != pCtrl->getHelpText())
            {
              // NSToolInfo toolInfo(*this, (*j)->HWindow, pCtrl->getHelpText().c_str()) ;
              // _pNSToolTip->AddTool(toolInfo) ;

              TToolInfo toolInfo(*this, (*j)->HWindow, pCtrl->getHelpText().c_str()) ;
              _pToolTip->AddTool(toolInfo) ;
            }
          }

          if ((_tabbedWindows->begin() == i) && bChildIsVisible)
            (*j)->Show(SW_SHOW) ;
          else
            (*j)->Show(SW_HIDE) ;
        }

    	if (_tabbedWindows->begin() != i)
    	{
      	(*i)->Show(SW_HIDE) ;
        // Don't do this because it switchs the control to "NOT WS_VISIBLE"
        // and it becomes undistinguishable from true hidden controls
        // (*i)->EnableWindow(false) ;
      }
    }

  // Make sure controls have been created before following initializations
  //
  initialiseControles() ;

  // Don't initialize from BBK or Capture if in edit mode
  //
  if (false == isInEditMode())
  {
    initControlesFromBbk() ;
    initControlesFromCapture() ;
  }

  rafraichitControles() ;

	// Set to the first tab
  if ((NULL != _m_tabCtrl) && (_tabbedWindows->size() > 1))
  {
    SetSelectedTab(0) ;
	  _m_tabCtrl->Show(SW_SHOW) ;
    // Don't do this because it switchs the control to "WS_VISIBLE"
    // even for hidden controls
	  // m_tabCtrl->EnableWindow(true) ;
  }

  // Set focus to first control
  SetFocusToNextControl(0) ;

  _bAcceptUpdate = true ;

  _pToolTip->Activate() ;

  // In case some information arrived during SetupWindow
  //
  if (_bNeedToRefresh)
    ::PostMessage(HWindow, WM_COMMAND, IDM_BBK_UPDATE, 0) ;
}

void
NSBBMUEView::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  if (false == _tabbedWindows->empty())
		for (NSWindowsIter i = _tabbedWindows->begin() ; _tabbedWindows->end() != i ; i++)
      if ((NULL != (*i)->controls) && (false == (*i)->controls->empty()))
        for (NSControlsIter j = (*i)->controls->begin(); (*i)->controls->end() != j ; j++)
          (*j)->Paint(dc, erase, RectangleAPeindre) ;
}

//
// Setting control's color
//
HBRUSH
NSBBMUEView::EvCtlColor(HDC hdc, HWND hWndCtl, uint /* ctlType */)
{
  	//
	// S'il s'agit de NSTreeWindow tuer pEDitDico
	//
	if ((NULL == _pNSCtrl) || _pNSCtrl->empty())
    return (HBRUSH)DefaultProcessing() ;

	for (iterNSControle iCtl = _pNSCtrl->begin() ; _pNSCtrl->end() != iCtl ; iCtl++)
	{
		if (((*iCtl)->getHWND() == hWndCtl) && ((*iCtl)->getType() == isEdit))
		{
      NSEdit* pNSEditWindow = static_cast<NSEdit*>((*iCtl)->getControle()) ;
      if (pNSEditWindow)
        return pNSEditWindow->getCtlColor(hdc) ;
    }
  }

  return (HBRUSH)DefaultProcessing() ;
}

void
NSBBMUEView::EvSize(uint sizeType, ClassLib::TSize& size)
{
	NSMUEView::EvSize(sizeType, size) ;

  // Adjust tab control to new window size
  if (_m_tabCtrl && _m_tabCtrl->GetHandle() && (_tabbedWindows->size() > 1))
  {
		_m_tabCtrl->SetWindowPos(0, 0, 0, size.cx, size.cy, SWP_NOZORDER | SWP_NOMOVE) ;

    NSOneTabWindow *pSelectedTab = Retrieve(_m_tabCtrl->GetSel()) ;
    if ((NULL == pSelectedTab) || (NULL == pSelectedTab->GetHandle()))
      return ;

    ClassLib::TRect rect ;
		_m_tabCtrl->GetWindowRect(rect) ;

    // NOTE: GetWindowRect returns screen coordinates... we'll need
		//       to have the coordinates relative to the frame window,
		//       the parent of both the tab and client window
		::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rect, 2);

    _m_tabCtrl->AdjustRect(false, rect) ;

    pSelectedTab->SetWindowPos(0, rect, SWP_NOZORDER) ;
  }
}

// -----------------------------------------------------------------------------
// Description:	Init controls with BBItems information
// -----------------------------------------------------------------------------
void
NSBBMUEView::rafraichitControles()
{
	if ((NULL == _pNSCtrl) || _pNSCtrl->empty())
  	return ;

  for (iterNSControle i = _pNSCtrl->begin(); (_pNSCtrl->end() != i); i++)
  	(*i)->prepareControle() ;
}

void
NSBBMUEView::referenceControle(NSControle* pNSControle)
{
	_pNSCtrl->push_back(pNSControle) ;
}

//
// See http://support.microsoft.com/default.aspx?scid=kb;EN-US;145994
//
void
NSBBMUEView::InitMetrics()
{
  if ((NULL == _pBBItem) || (NULL == _pBBItem->getParseur()))
    return ;

	string sLang = pContexte->getUserLanguage() ;

  Cdialogbox * pDialogBox = _pBBItem->getArchetypeDialog(sLang) ;
  if (NULL == pDialogBox)
    return ;

  InitBasicMetrics(pDialogBox) ;
}

void
NSBBMUEView::CreerControlesArchetype()
{
try
{
  if ((NULL == _pBBItem) || (NULL == _pBBItem->getParseur()))
    return ;

	string sLang = pContexte->getUserLanguage() ;

  Cdialogbox * pDialogBox = _pBBItem->getArchetypeDialog(sLang) ;
  if (NULL == pDialogBox)
    return ;

  Ccontrol * pControl = pDialogBox->getFirstControl() ;
  if (NULL == pControl)
    return ;

  int hSysButton = 0 ;

  // Tabs management
  //
  _tabbedWindows = new NSWindowsArray() ;

  Ctabcontrol* pTabControl = pDialogBox->getTabControl() ;
  if (pTabControl)
  {
    // int iWidth  = max(GetClientRect().Width(), 0) ;
    // int iHeight = max(GetClientRect().Height(), 0) ;
    int iWidth  = 0 ;
    int iHeight = 0 ;
    _m_tabCtrl = new TTabControl(this, /*NSTabWindow::*/ID_TABCTRL, 0, 0, iWidth, iHeight) ;

    Ctab* pTab = pTabControl->getFirstTab() ;

    // We create the array of tabbedWindows; controls will be added later
	  //
    while (pTab)
    {
		  string sTitle = pTab->getTitle() ;
		  string sOrder = pTab->getOrder() ;
		  // int    iOrder = atoi(sOrder.c_str()) ;

		  NSOneTabWindow *pWinToAdd = new NSOneTabWindow(this, sTitle.c_str()) ;
		  _tabbedWindows->push_back(pWinToAdd) ;

		  pTab = pTabControl->getNextTab(pTab) ;
	  }
  }
  // If there is no tabs, we create a virtual one
  //
  else
  {
    _tabbedWindows->push_back(new NSOneTabWindow(this, "virtual tab")) ;
    // NSWindowsIter i = _tabbedWindows->begin() ;
    // (*i)->Show(SW_HIDE) ;
  }

  OWL::TGroupBox *pGroupCurr = 0 ;

  _pNSCtrl = new NSControleVector() ;

  while (pControl)
  {
    /* TWindow* pNewCtrl = */ CreateControl(pControl, pGroupCurr, hSysButton, sLang) ;

/*
    if (ctrlID == IDBBKCALCUL)
    {
      // cas où le contrôle n'est pas un des contrôles précédents, mais que
      // c'est le contrôle qui lance le calcul par le Blackboard
      NSControle * pCtr = new NSControle(pBBItem, sIdentite, sDlgFonction) ;
      referenceControle(pCtr) ;
    }
*/

    string ps = string(" --> Get Next Control ") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

    pControl = pDialogBox->getNextControl(pControl) ;
  }

  // ------------------------------------------------------------

  // On fixe la caption du dialogue
  string sDlgCaption = pDialogBox->getCaption() ;
  if ('$' == sDlgCaption[0])
  {
    if (string("$NAME") == sDlgCaption)
    {
      if (pContexte->getPatient())
        sDlgCaption = pContexte->getPatient()->getNomLong() ;
      else
        sDlgCaption = string("") ;
    }
    else
      sDlgCaption = string("") ;
  }
  SetCaption(sDlgCaption.c_str()) ;

  int hTab = 0 ;

  if (_m_tabCtrl && (_tabbedWindows->size() > 1))
  {
    _m_tabCtrl->Create() ;

    // on construit maintenant les tabs
	  // Add the tabs & pages already in tabbedWindowArray
	  NSOneTabWindow *ptr ;
	  int hTemp = 0 ;
    for (NSWindowsIter i = _tabbedWindows->begin() ; _tabbedWindows->end() != i ; i++)
    {
      ptr = *i ;

      // Reparent and create
      ptr->SetParent(this) ;
      if (!ptr->GetHandle())
        ptr->Create() ;
      // Retrieve window's text, use that for tab text
      TAPointer<_TCHAR> txt = new _TCHAR[ptr->GetWindowTextLength() + 1] ;
      ptr->GetWindowText(txt, ptr->GetWindowTextLength()+1) ;
      _m_tabCtrl->Add(TTabItem(txt)) ;
      // Select new tab as current
      _m_tabCtrl->SetSel(_m_tabCtrl->GetCount() - 1) ;
      // Center and display associated TWindow*
      hTemp = AdjustTabClient() ;

      // on note la hauteur de la barre des tabs
      if (hTemp > hTab)
        hTab = hTemp ;
    }
  }

  // ------------------------------------------------------------
}
catch (...)
{
  erreur("Exception NSBBMUEView::CreerControlesArchetype.", standardError, 0) ;
}
}

TWindow*
NSBBMUEView::CreateControl(Ccontrol *pControl, OWL::TGroupBox *pGroupCurr, int &hSysButton, string sLang)
{
  if (NULL == pControl)
    return (TWindow*) 0 ;

try
{
  string sType     = pControl->getType() ;
  string sCoords   = pControl->getCoords() ;
  string sCaption  = pControl->getCaption() ;
  string sFilling  = pControl->getFilling() ;
  string sHelpText = pControl->getHelpText() ;
  int    ctrlID    = pControl->getRefId() ;

  int    indexTab = 1 ;
  string sTabNb   = pControl->getTab() ;
  if (string("") != sTabNb)
    indexTab = atoi(sTabNb.c_str()) ;

  // conversion du type en majuscules
  pseumaj(&sType) ;

  int coords[4] ;
  coords[0] = getWindowXFromDialogX(pControl->getX()) ;
  coords[1] = getWindowYFromDialogY(pControl->getY()) ;
  coords[2] = getWindowXFromDialogX(pControl->getW()) ;
  coords[3] = getWindowYFromDialogY(pControl->getH()) ;

  string sIdentity    = pControl->getDataIdentity() ;
  string sDlgFonction = pControl->getDataFunction() ;

  // -------------------------------------------------------------------------
  // Traitement suivant le type de contrôle rencontré

  if (string("SYSTREEVIEW32") == sType)
  {
    // TreeView
    //
    NSTreeWindow * pNSTreeWindow = new NSTreeWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;

    pNSTreeWindow->Attr.Style = pControl->getIStyle() ;
    // Don't strcpy(pNSTreeWindow->Title, sIdentity.c_str()) ;
    pNSTreeWindow->SetCaption(sIdentity.c_str()) ;

    ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSTreeWindow) ;

    pNSTreeWindow->pBBitemNSTreeWindow = _pBBItem ;
    NSControle* pControl = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
    pControl->setControle(dynamic_cast<void *>(pNSTreeWindow)) ;
    pControl->setType(pNSTreeWindow->donneType()) ;
    pControl->setMUEView(this) ;
    pControl->setFilling(sFilling) ;
    pControl->setHelpText(sHelpText) ;
    pControl->setVisible(isControlProperty(pNSTreeWindow, WS_VISIBLE)) ;
    pNSTreeWindow->setControl(pControl) ;
    referenceControle(pControl) ;

    if ((pControl->getTransfert()) &&
                (pControl->getTransfert()->getFilsItem()))
    {
      BBFilsItem* pSonItem = pControl->getTransfert()->getFilsItem() ;
      BBItem*     pFatherItem = pSonItem->getItemFather() ;
      pNSTreeWindow->pBBitemNSTreeWindow = pFatherItem ;
      //
      // Créer le deuxième BBItem (GCONS le fils en cas de consultation par exemple)
      //
      if ((BBItem*) NULL != pFatherItem)
        pFatherItem->developperConsultation(pSonItem) ;
    }
    return pNSTreeWindow ;
  }

  if (string("SYSLISTVIEW32") == sType)
  {
/*
      // ListView
      if (sIdentity.find(string("LCADR")) != NPOS)
      {
        // Liste d'adresses
        NSAdrListWindow * pNSAdrListWindow = new NSAdrListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
        pNSAdrListWindow->Attr.Style = pControl->getIStyle() ;
        pNSAdrListWindow->pControle = new NSControle(pContexte, pBBItem, sIdentity, sDlgFonction) ;
        pNSAdrListWindow->pControle->setControle(dynamic_cast<void *>(pNSAdrListWindow)) ;
        pNSAdrListWindow->pControle->setType(pNSAdrListWindow->donneType()) ;
        pNSAdrListWindow->pControle->setFilling(sFilling) ;
        pNSAdrListWindow->pControle->setMUEView(this) ;

        // on ajoute ici un indicateur pour préciser que ce controle gère
        // lui-même la gestion multiple sans lancer de multi-dialogue
        pNSAdrListWindow->pControle->setGestionMultiple(true) ;
        referenceControle(pNSAdrListWindow->pControle) ;
      }
      else if (sIdentity.find(string("LCORR")) != NPOS)
      {
        NSCorListWindow * pNSCorListWindow = new NSCorListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
        pNSCorListWindow->Attr.Style = pControl->getIStyle() ;
        pNSCorListWindow->pControle = new NSControle(pContexte, pBBItem, sIdentity, sDlgFonction) ;
        pNSCorListWindow->pControle->setControle(dynamic_cast<void *>(pNSCorListWindow)) ;
        pNSCorListWindow->pControle->setType(pNSCorListWindow->donneType()) ;
        pNSCorListWindow->pControle->setFilling(sFilling) ;
        pNSCorListWindow->pControle->setMUEView(this) ;
        // on ajoute ici un indicateur pour préciser que ce controle gère
        // lui-même la gestion multiple sans lancer de multi-dialogue
        pNSCorListWindow->pControle->setGestionMultiple(true) ;
        referenceControle(pNSCorListWindow->pControle) ;
      }
      else
      {
*/
    string sItem ;
    size_t pos = sIdentity.find_last_of('/') ;
    if (NPOS != pos)
      sItem = string(sIdentity, pos + 1, strlen(sIdentity.c_str()) - pos - 1) ;
    else
      sItem = sIdentity ;

    if ('V' == sItem[0])
    {
      NSHistorizedValListWindow * pNSHistoListWindow = new NSHistorizedValListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
      pNSHistoListWindow->Attr.Style = pControl->getIStyle() ;
      pNSHistoListWindow->setItemValue(sItem) ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSHistoListWindow) ;

      pNSHistoListWindow->_pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pNSHistoListWindow->_pControle->setControle(dynamic_cast<void *>(pNSHistoListWindow)) ;
      pNSHistoListWindow->_pControle->setType(pNSHistoListWindow->donneType()) ;
      pNSHistoListWindow->_pControle->setFilling(sFilling) ;
      pNSHistoListWindow->_pControle->setHelpText(sHelpText) ;
      pNSHistoListWindow->_pControle->setVisible(isControlProperty(pNSHistoListWindow, WS_VISIBLE)) ;
      pNSHistoListWindow->_pControle->setMUEView(this) ;

      // on ajoute ici un indicateur pour préciser que ce controle gère
      // lui-même la gestion multiple sans lancer de multi-dialogue
      pNSHistoListWindow->_pControle->setGestionMultiple(true) ;
      referenceControle(pNSHistoListWindow->_pControle) ;

      return pNSHistoListWindow ;
    }
    else
    {
      NSHistorizedListWindow * pNSHistoListWindow = new NSHistorizedListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
      pNSHistoListWindow->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSHistoListWindow) ;

      pNSHistoListWindow->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pNSHistoListWindow->pControle->setControle(dynamic_cast<void *>(pNSHistoListWindow)) ;
      pNSHistoListWindow->pControle->setType(pNSHistoListWindow->donneType()) ;
      pNSHistoListWindow->pControle->setFilling(sFilling) ;
      pNSHistoListWindow->pControle->setHelpText(sHelpText) ;
      pNSHistoListWindow->pControle->setVisible(isControlProperty(pNSHistoListWindow, WS_VISIBLE)) ;
      pNSHistoListWindow->pControle->setMUEView(this) ;

      // on ajoute ici un indicateur pour préciser que ce controle gère
      // lui-même la gestion multiple sans lancer de multi-dialogue
      pNSHistoListWindow->pControle->setGestionMultiple(true) ;
      referenceControle(pNSHistoListWindow->pControle) ;

      return pNSHistoListWindow ;
    }
      // }
  }

  if (string("VALLISTVIEW32") == sType)
  {
    // contrôle vallistview - from FLP
    NSHistorizedValListWindow * pNSHistoValWindow = new NSHistorizedValListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
    pNSHistoValWindow->Attr.Style = pControl->getIStyle() ;

    ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSHistoValWindow) ;

    pNSHistoValWindow->_pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
    pNSHistoValWindow->_pControle->setControle(dynamic_cast<void*>(pNSHistoValWindow)) ;
    pNSHistoValWindow->_pControle->setType(pNSHistoValWindow->donneType()) ;
    pNSHistoValWindow->_pControle->setFilling(sFilling);
    pNSHistoValWindow->_pControle->setHelpText(sHelpText) ;
    pNSHistoValWindow->_pControle->setVisible(isControlProperty(pNSHistoValWindow, WS_VISIBLE)) ;
    pNSHistoValWindow->_pControle->setMUEView(this) ;
    // on ajoute ici un indicateur pour préciser que ce controle gère lui-même
    // la gestion multiple sans lancer de multi-dialogue
    pNSHistoValWindow->_pControle->setGestionMultiple(true) ;
    referenceControle(pNSHistoValWindow->_pControle) ;
    return pNSHistoValWindow ;
  }

  if (string("EDIT") == sType)
  {
    UINT nMaxInput = 255 ; // Nombre maximal de caractères à saisir
    char szType[25] ;       // Type de contrôle EDIT
    char szEditDateType[2] ;         //type de date dans le controle EDIT (NSEditDate)

    // Contrôle Edit
    string sIden = "" ;
    //
    // Edit for Lexique or free text
    //
    if ((sIdentity.find(string("£C;")) != NPOS) || (sIdentity.find(string("/£C;")) != NPOS))
    {
      NSEditLexique * pNSEdit = new NSEditLexique(this, pContexte, ctrlID,
                                                    pContexte->getDico(),
                                                    "",
                                                    coords[0], coords[1],
                                                    coords[2], coords[3],
                                                    nMaxInput + 1) ;
      pNSEdit->setFreeTextEnabled(true) ;
      pNSEdit->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSEdit) ;

      pNSEdit->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pNSEdit->pControle->setControle(dynamic_cast<void *>(pNSEdit)) ;
      pNSEdit->pControle->setType(pNSEdit->donneType()) ;
      pNSEdit->pControle->setFilling(sFilling) ;
      pNSEdit->pControle->setHelpText(sHelpText) ;
      pNSEdit->pControle->setVisible(isControlProperty(pNSEdit, WS_VISIBLE)) ;
      pNSEdit->pControle->setMUEView(this) ;
      referenceControle(pNSEdit->pControle) ;
      return pNSEdit ;
    }
    //
    // Edit for Lexique only
    //
    else if (sIdentity.find(string("£CX")) != NPOS)
    {
      NSEditLexique * pNSEdit = new NSEditLexique(this, pContexte, ctrlID,
                                                    pContexte->getDico(),
                                                    "",
                                                    coords[0], coords[1],
                                                    coords[2], coords[3],
                                                    nMaxInput + 1) ;
      pNSEdit->setFreeTextEnabled(false) ;
      pNSEdit->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSEdit) ;

      pNSEdit->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pNSEdit->pControle->setControle(dynamic_cast<void *>(pNSEdit)) ;
      pNSEdit->pControle->setType(pNSEdit->donneType()) ;
      pNSEdit->pControle->setFilling(sFilling) ;
      pNSEdit->pControle->setHelpText(sHelpText) ;
      pNSEdit->pControle->setVisible(isControlProperty(pNSEdit, WS_VISIBLE)) ;
      pNSEdit->pControle->setMUEView(this) ;
      referenceControle(pNSEdit->pControle) ;
      return pNSEdit ;
    }
    else if (sIdentity.find(string("£CC")) != NPOS)
    {      // Champ edit qui n'ouvre pas le lexique et aliment Complement      NSEditNoLex * pEdit ;      strcpy(szType, "C0") ;      size_t posLex = sIdentity.find(string("£CC")) ;      string sElemLex = string(sIdentity, posLex, BASE_LEXIQUE_LEN) ;      string sLen = string(sElemLex, 4, 2) ;      int iLen = atoi(sLen.c_str()) ;      pEdit = new NSEditNoLex(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang, iLen) ;      pEdit->Attr.Style = pControl->getIStyle() ;      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pEdit) ;      pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction)) ;      pEdit->getControl()->setControle(dynamic_cast<void *>(pEdit)) ;
      pEdit->getControl()->setType(pEdit->donneType()) ;
      pEdit->getControl()->setFilling(sFilling) ;
      pEdit->getControl()->setHelpText(sHelpText) ;
      pEdit->getControl()->setVisible(isControlProperty(pEdit, WS_VISIBLE)) ;
      pEdit->getControl()->setMUEView(this) ;
      referenceControle(pEdit->getControl()) ;
      return pEdit ;
    }
    else if (sIdentity.find(string("£CL")) != NPOS)    {      // Champ edit qui n'ouvre pas le lexique et alimente Texte libre      NSEditNoLex* pEdit ;      size_t posLex = sIdentity.find(string("£CL")) ;      string sElemLex = string(sIdentity, posLex, BASE_LEXIQUE_LEN) ;      strcpy(szType, string(sElemLex, 2, 2).c_str()) ;      string sLen = string(sElemLex, 4, 2) ;      int iLen = atoi(sLen.c_str()) ;      pEdit = new NSEditNoLex(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang, iLen) ;      pEdit->Attr.Style = pControl->getIStyle() ;      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pEdit) ;      pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction)) ;      pEdit->getControl()->setControle(dynamic_cast<void *>(pEdit)) ;
      pEdit->getControl()->setType(pEdit->donneType()) ;
      pEdit->getControl()->setFilling(sFilling) ;
      pEdit->getControl()->setHelpText(sHelpText) ;
      pEdit->getControl()->setVisible(isControlProperty(pEdit, WS_VISIBLE)) ;
      pEdit->getControl()->setMUEView(this) ;
      referenceControle(pEdit->getControl()) ;
      return pEdit ;
    }
    else
    {
      // Type de saisie dans le contrôle
      string sLivre ;
      string sTypeDate ;
      size_t pos = sIdentity.find(string("£")) ;
      if (pos != NPOS)
      {
        sLivre = string(sIdentity, pos + 1, 1) ;
        strcpy(szType, sLivre.c_str()) ;

        sTypeDate = string(sIdentity, pos + 2, 1) ;
        strcpy(szEditDateType, sTypeDate.c_str()) ;
      }

      // Création de l'objet OWL correspondant à l'objet d'interface
      NSEdit* pEdit = NULL ;
      WNDTYPE	iEditType = isEdit ;
      switch (szType[0])
      {
        case codeMARK       : pEdit = new NSEdit(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang) ;
                              break ;
        case nbMARK         : pEdit = new NSEdit(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang) ;
                              break ;
        case charMARK       : pEdit = new NSEdit(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang) ;
                              break ;
        case dateMARK       : // NSEditDate*
                              pEdit = new NSEditDate(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang, szEditDateType[0]) ;
                              iEditType = isEditDate ;
                              break ;
        case dateHeureMARK  : pEdit = new NSEditDateHeure(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang, szEditDateType[0]) ;
                              iEditType = isEditDateHeure ;
                              break ;
        case heureMARK      : pEdit = new NSEditHeure(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang) ;
                              break ;
        default             : string sErrMess = string("Unknown Edit type : ") + string(1, szType[0]) ;
                              erreur(sErrMess.c_str(), standardError, 0) ;
      }

      if (pEdit)
      {
        pEdit->Attr.Style = pControl->getIStyle() ;

        ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pEdit) ;

        pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction)) ;
        pEdit->getControl()->setControle(dynamic_cast<void *>(pEdit)) ;
        // pEdit->pControle->setType(pEdit->donneType()) ; // doesn't work, always set a isEdit type
        pEdit->getControl()->setType(iEditType) ;
        pEdit->getControl()->setFilling(sFilling) ;
        pEdit->getControl()->setHelpText(sHelpText) ;
        pEdit->getControl()->setVisible(isControlProperty(pEdit, WS_VISIBLE)) ;
        pEdit->getControl()->setMUEView(this) ;
        referenceControle(pEdit->getControl()) ;
        return pEdit ;
      }
    }
  }

  // Static
  if ((string("STATIC") == sType) || (string("BORSTATIC") == sType))
  {
    // Création de l'objet OWL correspondant à l'objet d'interface
    NSStatic* pNSStatic = new NSStatic(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
    pNSStatic->Attr.Style = pControl->getIStyle() ;

    ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSStatic) ;

    pNSStatic->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
    pNSStatic->pControle->setControle(dynamic_cast<void *>(pNSStatic)) ;
    pNSStatic->pControle->setType(pNSStatic->donneType()) ;
    pNSStatic->pControle->setFilling(sFilling) ;
    pNSStatic->pControle->setHelpText(sHelpText) ;
    pNSStatic->pControle->setVisible(isControlProperty(pNSStatic, WS_VISIBLE)) ;
    pNSStatic->pControle->setMUEView(this) ;
    referenceControle(pNSStatic->pControle) ;
    return pNSStatic ;
  }

  if ((string("BUTTON") == sType) &&
             (IDOK   != ctrlID) && (IDCANCEL   != ctrlID) &&
             (IDB_OK != ctrlID) && (IDB_CANCEL != ctrlID))
  {
    // Contrôle Button (à voir pour OK et Cancel)

    // -----------------------------------------------------------------------
    // ATTENTION : Il faut penser à tester du plus grand au plus petit sinon
    // un groupbox répond oui à radiobouton (par exemple)
    // #define BS_PUSHBUTTON       0x00000000L
    // #define BS_DEFPUSHBUTTON    0x00000001L
    // #define BS_CHECKBOX         0x00000002L
    // #define BS_AUTOCHECKBOX     0x00000003L
    // #define BS_RADIOBUTTON      0x00000004L
    // #define BS_3STATE           0x00000005L
    // #define BS_AUTO3STATE       0x00000006L
    // #define BS_GROUPBOX         0x00000007L
    // #define BS_USERBUTTON       0x00000008L
    // #define BS_AUTORADIOBUTTON  0x00000009L

    // Détermination du style
    // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE);
    int nIndex = pControl->getIStyle();

    if ((nIndex & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)    {
      // Contrôle AutoRadioButton

      // Création de l'objet OWL correspondant à l'objet d'interface
      NSRadioButton * pRaBut = new NSRadioButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr) ;
      pRaBut->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pRaBut) ;

      pRaBut->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pRaBut->pControle->setControle(dynamic_cast<void *>(pRaBut)) ;
      pRaBut->pControle->setType(pRaBut->donneType()) ;
      pRaBut->pControle->setFilling(sFilling) ;
      pRaBut->pControle->setHelpText(sHelpText) ;
      pRaBut->pControle->setVisible(isControlProperty(pRaBut, WS_VISIBLE)) ;
      pRaBut->pControle->setMUEView(this) ;
      referenceControle(pRaBut->pControle) ;
      return pRaBut ;
    }
    if ((nIndex & BS_GROUPBOX) == BS_GROUPBOX)
    {
      // Contrôle Groupbox
      if (sIdentity != "")
      {
        NSGroupBox * pGrpBox = new NSGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
        pGrpBox->Attr.Style = pControl->getIStyle() ;

        ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pGrpBox) ;

        pGrpBox->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
        pGrpBox->pControle->setControle(dynamic_cast<void *>(pGrpBox)) ;
        pGrpBox->pControle->setType(pGrpBox->donneType()) ;
        pGrpBox->pControle->setFilling(sFilling) ;
        pGrpBox->pControle->setHelpText(sHelpText) ;
        pGrpBox->pControle->setVisible(isControlProperty(pGrpBox, WS_VISIBLE)) ;
        pGrpBox->pControle->setMUEView(this) ;
        referenceControle(pGrpBox->pControle) ;
        pGroupCurr = (OWL::TGroupBox *)pGrpBox ;
        return pGrpBox ;
      }
      else
      {
        OWL::TGroupBox* pGroupBox = new OWL::TGroupBox(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
        pGroupBox->Attr.Style = pControl->getIStyle() ;

        ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pGroupBox) ;

        // aGroups.push_back(pGroupBox) ;
        pGroupCurr = pGroupBox ;
        return pGroupBox ;
      }
    }
    if ((nIndex & BS_RADIOBUTTON) == BS_RADIOBUTTON)
    {      // Contrôle RadioButton

      // Création de l'objet OWL correspondant à l'objet d'interface
      NSRadioButton * pRaBut = new NSRadioButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr) ;
      pRaBut->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pRaBut) ;

      pRaBut->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pRaBut->pControle->setControle(dynamic_cast<void *>(pRaBut)) ;
      pRaBut->pControle->setType(pRaBut->donneType()) ;
      pRaBut->pControle->setFilling(sFilling) ;
      pRaBut->pControle->setHelpText(sHelpText) ;
      pRaBut->pControle->setVisible(isControlProperty(pRaBut, WS_VISIBLE)) ;
      pRaBut->pControle->setMUEView(this) ;
      referenceControle(pRaBut->pControle) ;
      return pRaBut ;
    }
    if (((nIndex& BS_CHECKBOX) == BS_CHECKBOX) || ((nIndex& BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX))
    {
      // Contrôle CheckBox
      NSCheckBox * pChkBox = new NSCheckBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr, true) ;
      pChkBox->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pChkBox) ;

      pChkBox->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pChkBox->pControle->setControle(dynamic_cast<void *>(pChkBox)) ;
      pChkBox->pControle->setType(pChkBox->donneType()) ;
      pChkBox->pControle->setFilling(sFilling) ;
      pChkBox->pControle->setHelpText(sHelpText) ;
      pChkBox->pControle->setVisible(isControlProperty(pChkBox, WS_VISIBLE)) ;
      pChkBox->pControle->setMUEView(this) ;
      referenceControle(pChkBox->pControle) ;
      return pChkBox ;
    }
    // Contrôle Button (par défaut ne participe au transfert )
    NSButton * pButt = new NSButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
    pButt->Attr.Style = pControl->getIStyle() ;

    ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pButt) ;

    pButt->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
    pButt->pControle->setControle(dynamic_cast<void *>(pButt)) ;
    pButt->pControle->setType(pButt->donneType()) ;
    pButt->pControle->setFilling(sFilling) ;
    pButt->pControle->setHelpText(sHelpText) ;
    pButt->pControle->setVisible(isControlProperty(pButt, WS_VISIBLE)) ;
    pButt->pControle->setMUEView(this) ;
    referenceControle(pButt->pControle) ;
    return pButt ;
  }
  if (string("BUTTON") == sType)
  {
    TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
    pButt->Attr.Style = pControl->getIStyle() ;

    // Les boutons OK et CANCEL ne sont pas créés dans la vue
    // OK and CANCEL buttons are not created inside the view
    //
    if ((IDOK == ctrlID) || (IDCANCEL == ctrlID))
    {
      if      (IDOK == ctrlID)
        uButtonsStyle |= MYWS_OK ;
      else if (IDCANCEL == ctrlID)
        uButtonsStyle |= MYWS_CANCEL ;
    }
    // IDB_OK and IDB_CANCEL are not created in windows (they are in dialogs)
    //
    else if ((IDB_OK != ctrlID) && (IDB_CANCEL != ctrlID))
    {
      // Bouton systeme : ne pas referencer dans les controles de tabbedWindows
      TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(),
                                            coords[0], coords[1], coords[2], coords[3]);
      pButt->Attr.Style = pControl->getIStyle() ;

      if ((coords[1] + coords[3]) > hSysButton)
        hSysButton = coords[1] + coords[3] ;
    }
    return pButt ;
  }

  if (string("SCROLLBAR") == sType)
  {
    // Contrôle ScrollBar
    int nIndex = pControl->getIStyle() ;
    if ((nIndex& WS_HSCROLL) == WS_HSCROLL)
    {      // Création de l'objet OWL correspondant à l'objet d'interface
      TScrollBar* pScroll = new TScrollBar(this, ctrlID, coords[0], coords[1], coords[2], coords[3], true) ;
      pScroll->Attr.Style = pControl->getIStyle() ;
      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pScroll) ;
      return pScroll ;
    }
    else
    {
      // Création de l'objet OWL correspondant à l'objet d'interface
      TScrollBar* pScroll = new TScrollBar(this, ctrlID, coords[0], coords[1], coords[2], coords[3], false) ;
      pScroll->Attr.Style = pControl->getIStyle() ;
      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pScroll) ;
      return pScroll ;
    }
    // typeCtrl = "ScrollBar" ;

  }
  // BORLAND : Contrôle Bouton
  if ((string("BORBTN") == sType) &&  (ctrlID != IDOK)          && (ctrlID != IDCANCEL)     &&
                                        (ctrlID != IDB_OK)        && (ctrlID != IDB_CANCEL)   &&
                                        (ctrlID != IDRETOUR)      && (ctrlID != IDSUITE)      &&
                                        (ctrlID != IDHELP)        && (ctrlID != IDHELPWWW)    &&
                                        (ctrlID != IDHELPNEW)     && (ctrlID != IDTREEPASTE)  &&
                                        (ctrlID != IDCONCLUSION)  && (ctrlID != IDBBKCALCUL))
  {
    // Détermination du style
    // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE);
    int nIndex = pControl->getIStyle() ;

    if (!((nIndex & BBS_BITMAP) == BBS_BITMAP))
    {
      // Contrôle RadioButton
      NSButton * pButt = new NSButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
      pButt->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pButt) ;

      pButt->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pButt->pControle->setControle(dynamic_cast<void *>(pButt)) ;
      pButt->pControle->setType(pButt->donneType()) ;
      pButt->pControle->setFilling(sFilling) ;
      pButt->pControle->setHelpText(sHelpText) ;
      pButt->pControle->setVisible(isControlProperty(pButt, WS_VISIBLE)) ;
      pButt->pControle->setMUEView(this) ;
      referenceControle(pButt->pControle) ;
      return pButt ;
    }
    return (TWindow*) 0 ;
  }
  if (string("BORBTN") == sType)
  {
    if ((IDOK == ctrlID) || (IDCANCEL == ctrlID))
    {
      if      (IDOK == ctrlID)
      {
        uButtonsStyle |= MYWS_OK ;

        // Not in edit mode (means new document): display "previous" button
        //
        if (false == isInEditMode())
          uButtonsStyle |= MYWS_PREVIOUS ;
      }
      else if (IDCANCEL == ctrlID)
        uButtonsStyle |= MYWS_CANCEL ;
    }
    // IDB_OK and IDB_CANCEL are not created in windows (they are in dialogs)
    //
    else if ((IDB_OK != ctrlID) && (IDB_CANCEL != ctrlID))
    {
      // bouton systeme : ne pas referencer dans les controles de tabbedWindows
      TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(),
                                            coords[0], coords[1], coords[2], coords[3]);
      pButt->Attr.Style = pControl->getIStyle() ;

      if ((coords[1] + coords[3]) > hSysButton)
        hSysButton = coords[1] + coords[3] ;

      return pButt ;
    }
    return (TWindow*) 0 ;
  }

  // BORLAND : Contrôle CheckBox
  if (string("BORCHECK") == sType)
  {
    NSCheckBox * pChkBox = new NSCheckBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr) ;
    pChkBox->Attr.Style = pControl->getIStyle() ;

    ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pChkBox) ;

    pChkBox->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
    pChkBox->pControle->setControle(dynamic_cast<void *>(pChkBox)) ;
    pChkBox->pControle->setType(pChkBox->donneType()) ;
    pChkBox->pControle->setFilling(sFilling) ;
    pChkBox->pControle->setHelpText(sHelpText) ;
    pChkBox->pControle->setVisible(isControlProperty(pChkBox, WS_VISIBLE)) ;
    pChkBox->pControle->setMUEView(this) ;
    referenceControle(pChkBox->pControle) ;
    return pChkBox ;
  }

  if (string("BORRADIO") == sType)
  {
    // BORLAND : Contrôle RadioBouton
    NSRadioButton * pRaBut = new NSRadioButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr) ;
    pRaBut->Attr.Style = pControl->getIStyle() ;

    ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pRaBut) ;

    pRaBut->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
    pRaBut->pControle->setControle(dynamic_cast<void *>(pRaBut)) ;
    pRaBut->pControle->setType(pRaBut->donneType()) ;
    pRaBut->pControle->setFilling(sFilling) ;
    pRaBut->pControle->setHelpText(sHelpText) ;
    pRaBut->pControle->setVisible(isControlProperty(pRaBut, WS_VISIBLE)) ;
    pRaBut->pControle->setMUEView(this) ;
    referenceControle(pRaBut->pControle) ;
    return pRaBut ;
  }

  if (string("SYSTABCONTROL32") == sType)
  {
    // BORLAND : Contrôle Onglets
    NSOnglet * pOnglet = new NSOnglet(pContexte, this, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
    pOnglet->Attr.Style = pControl->getIStyle() ;

    ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pOnglet) ;

    NSControle* pControl = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
    pControl->setControle(dynamic_cast<void *>(pOnglet)) ;
    pControl->setType(pOnglet->donneType()) ;
    pControl->setFilling(sFilling) ;
    pControl->setHelpText(sHelpText) ;
    pControl->setVisible(isControlProperty(pOnglet, WS_VISIBLE)) ;
    pControl->setMUEView(this) ;

    pOnglet->setControl(pControl) ;
    referenceControle(pControl) ;
    return pOnglet ;
  }

  if (string("COMBOBOX") == sType)
  {
    // is it a classification or not ?
    bool bClassif = false ;
    VectString aVecteurString ;
    NSSuper* pSuper = pContexte->getSuperviseur() ;
    pSuper->getFilGuide()->TousLesVrais("0CODE", "ES", &aVecteurString, "ENVERS") ;    if (false == aVecteurString.empty())    {      string sIdentSens ;      NSDico::donneCodeSens(&sIdentity, &sIdentSens) ;      if (aVecteurString.contains(sIdentSens))        bClassif = true ;    }
    if (bClassif)
    {
      // BORLAND : Contrôle Combobox
      NSComboClassif * pComboB = new NSComboClassif(pContexte, this, ctrlID, coords[0], coords[1], coords[2], coords[3], pControl->getIStyle(), 0) ;
      pComboB->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pComboB) ;

      // NSComboClassif * pComboB = new NSComboClassif(this, ctrlID) ;
      pComboB->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pComboB->pControle->setControle(dynamic_cast<void *>(pComboB)) ;
      pComboB->pControle->setType(pComboB->donneType()) ;
      pComboB->pControle->setFilling(sFilling) ;
      pComboB->pControle->setHelpText(sHelpText) ;
      pComboB->pControle->setVisible(isControlProperty(pComboB, WS_VISIBLE)) ;
      pComboB->pControle->setMUEView(this) ;
      referenceControle(pComboB->pControle) ;
      return pComboB ;
    }
    else
    {
      NSComboSemantique * pComboB = new NSComboSemantique(pContexte, this, ctrlID, coords[0], coords[1], coords[2], coords[3], pControl->getIStyle(), 0) ;
      pComboB->Attr.Style = pControl->getIStyle() ;

      ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pComboB) ;

      // NSComboClassif * pComboB = new NSComboClassif(this, ctrlID) ;
      pComboB->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
      pComboB->pControle->setControle(dynamic_cast<void *>(pComboB)) ;
      pComboB->pControle->setType(pComboB->donneType()) ;
      pComboB->pControle->setFilling(sFilling) ;
      pComboB->pControle->setHelpText(sHelpText) ;
      pComboB->pControle->setVisible(isControlProperty(pComboB, WS_VISIBLE)) ;
      pComboB->pControle->setMUEView(this) ;
      referenceControle(pComboB->pControle) ;
      return pComboB ;
    }
  }

  // BORLAND : Contrôle BorShade
  if (string("BORSHADE") == sType)
  {
    // Détermination du style
    // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE) ;
    int nIndex = pControl->getIStyle() ;

    if (((nIndex & BSS_RGROUP) == BSS_RGROUP) || ((nIndex & WS_GROUP) == WS_GROUP))
    {
      // Contrôle Group
      if (string("") != sIdentity)
      {
        NSGroupBox * pGrpBox = new NSGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
        pGrpBox->Attr.Style = pControl->getIStyle() ;

        ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pGrpBox) ;

        pGrpBox->pControle = new NSControle(pContexte, _pBBItem, sIdentity, sDlgFonction) ;
        pGrpBox->pControle->setControle(dynamic_cast<void *>(pGrpBox)) ;
        pGrpBox->pControle->setType(pGrpBox->donneType()) ;
        pGrpBox->pControle->setFilling(sFilling) ;
        pGrpBox->pControle->setHelpText(sHelpText) ;
        pGrpBox->pControle->setVisible(isControlProperty(pGrpBox, WS_VISIBLE)) ;
        pGrpBox->pControle->setMUEView(this) ;
        referenceControle(pGrpBox->pControle) ;
        pGroupCurr = (OWL::TGroupBox *)pGrpBox ;
        return pGrpBox ;
      }
    }

    OWL::TGroupBox* pGroupBox = new OWL::TGroupBox(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
    pGroupBox->Attr.Style = pControl->getIStyle() ;

    ((*_tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pGroupBox) ;

    // aGroups.push_back(pGroupBox) ;
    pGroupCurr = pGroupBox ;
    return pGroupBox ;
  }

  return (TWindow*) 0 ;
}
catch (...)
{
	erreur("Exception NSBBMUEView::CreateControl().", standardError, 0) ;
  return (TWindow*) 0 ;
}
}

// Check if a control with Id IDBBKCALCUL is present
//
void
NSBBMUEView::CmBbkCalcul()
{
try
{
  string ps = string("Entering NSBBMUEView::CmBbkCalcul") ;
	pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  _bAcceptUpdate = false ;

  if ((NULL == _pNSCtrl) || (_pNSCtrl->empty()))
		return ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	bool trouve = false ;

  // Is there a KS involved?
  //
  iterNSControle i = _pNSCtrl->begin() ;
	for ( ; (_pNSCtrl->end() != i) ; i++)
	{		TControl * pCtrl = static_cast<TControl *>((*i)->getControle()) ;		int idCtrl = pCtrl->GetDlgCtrlID() ;		if (IDBBKCALCUL == idCtrl)		{			trouve = true ;			break ;
		}
	}

	if (false == trouve)
		return ;

	SetCursor(pNSDLLModule, IDC_THINKING_CURSOR) ;

	// on enregistre les modifs de tous les fils du BBItem root
	_pBBItem->okFermerDialogue(true, false) ;

	NSPatPathoArray PatPathoArray(pContexte->getSuperviseur()) ;

	// L'archetype est par convention dans le champ fils du BBItem lanceur
	string sArchetype = _pBBItem->getItemSonsList() ;
	pSuper->afficheStatusMessage("Renseignement du Blackboard...") ;
	string sDejaRepondu = "" ;

	// Enumération de tous les contrôles situés avant le contrôle IDBBKCALCUL
  //
	for (i = _pNSCtrl->begin() ; (_pNSCtrl->end() != i) ; i++)
	{    TControl * pCtrl = static_cast<TControl *>((*i)->getControle()) ;    int idCtrl = pCtrl->GetDlgCtrlID() ;    if (IDBBKCALCUL == idCtrl)      break ;    if (((*i)->getTransfert()) && (-1 != idCtrl))    {      // string sCheminBBFilsItem = (*i)->getPath() ;      // sCheminBBFilsItem = (*i)->cutPath(&sCheminBBFilsItem, string("/0QUES/"), false) ;      string sCheminBBFilsItem = string("") ;    	(*i)->getPathForBlackboard(&sCheminBBFilsItem) ;      NSSearchStruct searchStruct = *((*i)->getSearchStruct()) ;      // searchStruct._sStartingDatePattern = (*i)->getFillingEndPeriod() ;      // (*i)->getEndValidityDate(&(searchStruct._sStartingDate)) ;/*      // on récupère le chemin du BBItem pere du BBFilsItem associé      // au NSControle via son pTransfert      string sCheminBBFilsItem = (*i)->getTransfert()->pBBFilsItem->getItemFather()->sLocalisation ;      // on rajoute l'étiquette du fils      string sEtiq ;      string sEtiquette = (*i)->getTransfert()->pBBFilsItem->getItemLabel() ;      pSuper->getDico()->donneCodeSens(&sEtiquette, &sEtiq) ;
      sCheminBBFilsItem += string(1, cheminSeparationMARK) + sEtiq ;

      // on enleve le chemin jusqu'à "0QUES1"
      size_t pos = sCheminBBFilsItem.find("/0QUES/") ;      if (pos == string::npos)        continue ;

      sCheminBBFilsItem = string(sCheminBBFilsItem, pos + 7, strlen(sCheminBBFilsItem.c_str()) - pos - 7) ;
*/
      if (string("") == sCheminBBFilsItem)
        continue ;

      // Cas d'une valeur chiffrée sur un bouton : enlever ce qui suit /$
      size_t pos = sCheminBBFilsItem.find("/$") ;
      if (0 == pos)
        continue ;
      if (string::npos != pos)
        sCheminBBFilsItem = string(sCheminBBFilsItem, 0, pos) ;

      sCheminBBFilsItem = getRegularPath(sCheminBBFilsItem, cheminSeparationMARK, intranodeSeparationMARK) ;

      NSTransferInfo* pTrans = (*i)->getTransfert() ;

      // on a ici un chemin non vide = une réponse à donner au blackboard
      PatPathoArray.vider() ;

      if ((*i)->getType() == isHistoryValListWindow)
      {
        // cas des listes d'elts multiples : on prend le pTransfert du père et
        // on récupère la premiere patpatho de ce transfert
        //
        BBItem* pBBItemPere = pTrans->getFilsItem()->getItemFather() ;
        if ((pBBItemPere) && (pBBItemPere->_pBBFilsPere) && (pBBItemPere->_pBBFilsPere->getItemTransfertData()))
        {
          NSTransferInfo *pTransfert = pBBItemPere->_pBBFilsPere->getItemTransfertData() ;
          NSVectFatheredPatPathoArray* pFatheredArray = pTransfert->getPatPatho() ;
          if ((pFatheredArray) && (false == pFatheredArray->empty()))
          {
            NSFatheredPatPathoArray* pFatheredElement = *(pFatheredArray->begin()) ;
            if ((pFatheredElement) && (false == pFatheredElement->getPatPatho()->empty()))
              PatPathoArray = *(pFatheredElement->getPatPatho()) ;
          }
        }
      }
      else
      {
        NSVectFatheredPatPathoArray* pFatheredArray = pTrans->getPatPatho() ;
        if ((pFatheredArray) && (false == pFatheredArray->empty()) && (false == (*(pFatheredArray->begin()))->getPatPatho()->empty()))
        {
          NSFatheredPatPathoArray* pFatheredElement = *(pFatheredArray->begin()) ;
          PatPathoArray = *(pFatheredElement->getPatPatho()) ;
        }
        else if (pTrans->getFilsItem()->isActif())
        {
          BBFilsItem * pFils = pTrans->getFilsItem() ;
          BBItem * pPere = pFils->getItemFather() ;
          if (pPere)
          {
            // pFils->getItemTransfertData()->setTmpTransfertMessage(pFils->getItemTransfertData()->getTransfertMessage()) ;
            pFils->getItemTransfertData()->setTmpActif(pFils->getItemTransfertData()->getActif()) ;
            // Modif 16/05/09 PA
            // pPere->pTempPPT->vider() ;
            // pPere->AjouteTmpEtiquette(pFils) ;
            // PatPathoArray = *(pPere->pTempPPT) ;
            NSPatPathoArray TempPPT(pContexte->getSuperviseur()) ;
            pPere->AjouteTmpEtiquette(&TempPPT, pFils) ;
            PatPathoArray = TempPPT ;
          }
        }
      }

      // on donne les réponses au blackboard
      if (sCheminBBFilsItem != sDejaRepondu)
      {
				if (((*i)->getTransfert()->getFilsItem()->VectorFils.empty()) && (PatPathoArray.size() > 1))
        {
        	// Insert the array as an answer to this control's path
          //
        	pContexte->getBBinterface()->insertAnswerOnBlackboard(sCheminBBFilsItem, &PatPathoArray, Undefined, HWindow, &searchStruct) ;
          //
          // Insert leaves as elements with their own path
          //
        	pContexte->getBBinterface()->insertLeavesOnBlackBoard(sCheminBBFilsItem, &PatPathoArray) ;
        }
        else
        	pContexte->getBBinterface()->insertAnswerOnBlackboard(sCheminBBFilsItem, &PatPathoArray, Undefined, HWindow, &searchStruct) ;
      }
      // Evite qu'un bouton non clické remette à zéro ce que vient de mettre un bouton clické
      // To prevent a non clicked button to reset the value just set up by an activated button
      if (false == PatPathoArray.empty())
        sDejaRepondu = sCheminBBFilsItem ;
    }
	}

  iterNSControle iPostCalcul = i ;

/*
	// On libère le KS afin qu'il puisse effectuer les calculs
  string sKsName = pBBItem->KsInterface.getKsName() ;
  if (sKsName != "")
  	pSuper->getBBinterface()->driveKSfromDialog(sKsName, BB1BBInterface::ksFree) ;
*/

	string sErrMsg = pSuper->getText("bbkKsManagement", "blackboardInDuty") ;
	pSuper->afficheStatusMessage((char*)sErrMsg.c_str()) ;

	// This is the case when the KS is already running and waits for the user
  // to press Ok
  //
	if (_pBBItem->_KsInterface.getTokenId() > 0)
  {
		// On libère le token afin qu'il puisse lancer le KS afin d'effectuer les calculs
		pContexte->getBBinterface()->driveKSfromDialog(_pBBItem->_KsInterface.getTokenId(), BB1BBInterface::ksFree) ;

  	_bWaitingForKsAnswer = true ;

    waitForKS() ;
	}
  // This is the case when the KS will only start when results are asked
  // We ask the first question, then wait for the KS end of work signal
  //
  else
  {
  	bool bFirstCompute = true ;
		for (i = iPostCalcul ; _pNSCtrl->end() != i ; i++)
		{			if ((*i)->getTransfert())			{				// on récupère le chemin du BBItem pere du BBFilsItem associé				// au NSControle via son pTransfert      	//        string sCheminBBFilsItem = (*i)->getPath() ;        sCheminBBFilsItem = (*i)->cutPath(&sCheminBBFilsItem, string("/0CALC/"), false) ;				if (string("") != sCheminBBFilsItem)        {					// on a ici un chemin non vide = une question à poser au blackboard
					NSPatPathoArray * pPatPathoLocal = (NSPatPathoArray *) 0 ;

          char szThis[20] ;
          sprintf(szThis, "%p", HWindow) ;
          string ps = string("NSBBMUEView ") + string(szThis) + string(" asking BBK for question ") + sCheminBBFilsItem ;
          pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

      		string sAnswerDate ;
					AnswerStatus::ANSWERSTATUS res = pContexte->getBBinterface()->getAnswer2Question(sCheminBBFilsItem, sArchetype, &pPatPathoLocal, sAnswerDate, bFirstCompute, true /* bUserIsWaiting */, HWindow) ;
					bFirstCompute = false ;
          if ((AnswerStatus::astatusProcessed == res) && (pPatPathoLocal) && (false == pPatPathoLocal->empty()))
					{
						// il y a une réponse du blackboard ==> on transmet la patpatho au
        		// BBFilsItem via le pTransfert et on met à jour le contrôle
            //
        		initControlFromBbkAnswer(*i, pPatPathoLocal, &sAnswerDate) ;
          }
          else if (AnswerStatus::astatusProcessing == res)
      			_bWaitingForKsAnswer = true ;
          if (pPatPathoLocal)
      			delete pPatPathoLocal ;
        }
      }
    }

    if (_bWaitingForKsAnswer)
    	waitForKS() ;
  }
}
catch (...)
{
	erreur("Exception NSBBMUEView::CmBbkCalcul().", standardError, 0) ;
}
}

void
NSBBMUEView::CmBbkCalculFinalStep()
{
  string ps = string("Dialog \"") + string(Title) + string("\" --> entering CmBbkCalculFinalStep") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  _bWaitingForKsAnswer = false ;

  SetCursor(0, IDC_ARROW) ;

  if ((NULL == _pNSCtrl) || (_pNSCtrl->empty()))
		return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sErrMsg = pSuper->getText("bbkKsManagement", "displayingResults") ;
	pSuper->afficheStatusMessage((char*) sErrMsg.c_str()) ;

  iterNSControle i = _pNSCtrl->begin() ;
	for ( ; (_pNSCtrl->end() != i) ; i++)
	{		TControl *pCtrl = static_cast<TControl *>((*i)->getControle()) ;    if (pCtrl && (IDBBKCALCUL == pCtrl->GetDlgCtrlID()))      break ;  }  if (_pNSCtrl->end() == i)  {    ps = string("Dialog \"") + string(Title) + string("\" --> no IDBBKCALCUL control... quitting CmBbkCalculFinalStep") ;    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;    return ;  }  i++ ;  if (_pNSCtrl->end() == i)  {    ps = string("Dialog \"") + string(Title) + string("\" --> no IDBBKCALCUL control... quitting CmBbkCalculFinalStep") ;    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;    return ;  }  // L'archetype est par convention dans le champ fils du BBItem lanceur	string sArchetype = _pBBItem->getItemSonsList() ;	for ( ; (_pNSCtrl->end() != i) ; i++)
	{		if ((*i)->getTransfert())		{			// on récupère le chemin du BBItem pere du BBFilsItem associé			// au NSControle via son pTransfert      //			// sCheminBBFilsItem = getPathForBbkQuestion(i) ;      string sCheminBBFilsItem = (*i)->getPath() ;      sCheminBBFilsItem = (*i)->cutPath(&sCheminBBFilsItem, string("/0CALC/"), false) ;			if (string("") == sCheminBBFilsItem)				continue ;

			// on a ici un chemin non vide = une question à poser au blackboard
			NSPatPathoArray * pPatPathoLocal = NULL ;

      string sAnswerDate ;
			AnswerStatus::ANSWERSTATUS res = pContexte->getBBinterface()->getAnswer2Question(sCheminBBFilsItem, sArchetype, &pPatPathoLocal, sAnswerDate, false /* bFirstCompute */, true /* bUserIsWaiting */, HWindow) ;
			// bFirstCompute = false ;
			if ((AnswerStatus::astatusProcessed == res) && (NULL != pPatPathoLocal) && (false == pPatPathoLocal->empty()))
			{
				// il y a une réponse du blackboard ==> on transmet la patpatho au
        // BBFilsItem via le pTransfert et on met à jour le contrôle

        initControlFromBbkAnswer(*i, pPatPathoLocal, &sAnswerDate) ;
      }
      else if (res == AnswerStatus::astatusProcessing)
      	_bWaitingForKsAnswer = true ;

      if (pPatPathoLocal)
      	delete pPatPathoLocal ;
		}
	}

  sErrMsg = pSuper->getText("bbkKsManagement", "resultsDisplayed") ;
	pSuper->afficheStatusMessage((char*) sErrMsg.c_str()) ;

  CmOkFinalStep() ;

  ps = string("Dialog \"") + string(Title) + string("\" --> quitting CmBbkCalculFinalStep") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}

void
NSBBMUEView::waitForKS()
{
	_tpsBBKStart.takeTime() ;

  SetTimer(ID_KSWAIT_TIMER, 1000) ;
}

void
NSBBMUEView::CmBbkEvent()
{
  string ps = string("Dialog \"") + string(Title) + string("\" --> BBK EVENT message received") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	_bWaitingForKsAnswer = false ;
  KillTimer(ID_KSWAIT_TIMER) ;

  CmBbkCalculFinalStep() ;
}

NSControle*
NSBBMUEView::getNSControl(TWindow *pCtrl)
{
  // For efficiency reasons, try to order tests according to frequency
  //

  // Static
  //
  NSStatic* pStatic = TYPESAFE_DOWNCAST(pCtrl, NSStatic) ;
  if (pStatic)
    return pStatic->pControle ;

  // Groups
  //
  NSGroupBox* pGroup = TYPESAFE_DOWNCAST(pCtrl, NSGroupBox) ;
  if (pGroup)
    return pGroup->pControle ;

  // Edit
  //
  NSEdit* pEdit = TYPESAFE_DOWNCAST(pCtrl, NSEdit) ;
  if (pEdit)
    return pEdit->getControl() ;
  NSEditDate* pEditDate = TYPESAFE_DOWNCAST(pCtrl, NSEditDate) ;
  if (pEditDate)
    return pEditDate->getControl() ;
  NSEditDateHeure* pEditDateHeure = TYPESAFE_DOWNCAST(pCtrl, NSEditDateHeure) ;
  if (pEditDateHeure)
    return pEditDateHeure->getControl() ;
  NSEditHeure* pEditHeure = TYPESAFE_DOWNCAST(pCtrl, NSEditHeure) ;
  if (pEditHeure)
    return pEditHeure->getControl() ;
  NSEditLexique* pEditLex = TYPESAFE_DOWNCAST(pCtrl, NSEditLexique) ;
  if (pEditLex)
    return pEditLex->pControle ;
  NSEditNoLex* pEditNoLex = TYPESAFE_DOWNCAST(pCtrl, NSEditNoLex) ;
  if (pEditNoLex)
    return pEditNoLex->getControl() ;

  // Buttons (radio, check...)
  //
  NSRadioButton* pRadio = TYPESAFE_DOWNCAST(pCtrl, NSRadioButton) ;
  if (pRadio)
    return pRadio->pControle ;
  NSCheckBox* pCheck = TYPESAFE_DOWNCAST(pCtrl, NSCheckBox) ;
  if (pCheck)
    return pCheck->pControle ;
  NSButton* pButton = TYPESAFE_DOWNCAST(pCtrl, NSButton) ;
  if (pButton)
    return pButton->pControle ;

  // Trees
  //
  NSTreeWindow* pTree = TYPESAFE_DOWNCAST(pCtrl, NSTreeWindow) ;
  if (pTree)
    return pTree->getControl() ;

  // Combos
  //
  NSComboSemantique* pSemantic = TYPESAFE_DOWNCAST(pCtrl, NSComboSemantique) ;
  if (pSemantic)
    return pSemantic->pControle ;
  NSComboClassif* pClassif = TYPESAFE_DOWNCAST(pCtrl, NSComboClassif) ;
  if (pClassif)
    return pClassif->pControle ;

  // Tabs
  //
  NSOnglet* pTab = TYPESAFE_DOWNCAST(pCtrl, NSOnglet) ;
  if (pTab)
    return pTab->getControl() ;

  // Lists
  //
  NSHistorizedValListWindow* pHVList = TYPESAFE_DOWNCAST(pCtrl, NSHistorizedValListWindow) ;
  if (pHVList)
    return pHVList->_pControle ;
  NSHistorizedListWindow* pHList = TYPESAFE_DOWNCAST(pCtrl, NSHistorizedListWindow) ;
  if (pHList)
    return pHList->pControle ;
  NSAdrListWindow* pAdrList = TYPESAFE_DOWNCAST(pCtrl, NSAdrListWindow) ;
  if (pAdrList)
    return pAdrList->pControle ;
  NSCorListWindow* pCorList = TYPESAFE_DOWNCAST(pCtrl, NSCorListWindow) ;
  if (pCorList)
    return pCorList->pControle ;

  return 0 ;
}

void
NSBBMUEView::SetupWindowForControl(TWindow *pCtrl)
{
  // For efficiency reasons, try to order tests according to frequency
  //

  // Static
  //
  NSStatic* pStatic = TYPESAFE_DOWNCAST(pCtrl, NSStatic) ;
  if (pStatic)
  {
    pStatic->SetupWindow() ;
    return ;
  }

  // Groups
  //
  NSGroupBox* pGroup = TYPESAFE_DOWNCAST(pCtrl, NSGroupBox) ;
  if (pGroup)
  {
    pGroup->SetupWindow() ;
    return ;
  }
/*
  OWL::TGroupBox* pGrpBox = TYPESAFE_DOWNCAST(pCtrl, OWL::TGroupBox) ;
  if (pGrpBox)
  {
    pGrpBox->SetupWindow() ;
    return ;
  }
*/

  // Edit
  //
  NSEdit* pEdit = TYPESAFE_DOWNCAST(pCtrl, NSEdit) ;
  if (pEdit)
  {
    pEdit->SetupWindow() ;
    return ;
  }
  NSEditDate* pEditDate = TYPESAFE_DOWNCAST(pCtrl, NSEditDate) ;
  if (pEditDate)
  {
    pEditDate->SetupWindow() ;
    return ;
  }
  NSEditDateHeure* pEditDateHeure = TYPESAFE_DOWNCAST(pCtrl, NSEditDateHeure) ;
  if (pEditDateHeure)
  {
    pEditDateHeure->SetupWindow() ;
    return ;
  }
  NSEditHeure* pEditHeure = TYPESAFE_DOWNCAST(pCtrl, NSEditHeure) ;
  if (pEditHeure)
  {
    pEditHeure->SetupWindow() ;
    return ;
  }
  NSEditLexique* pEditLex = TYPESAFE_DOWNCAST(pCtrl, NSEditLexique) ;
  if (pEditLex)
  {
    pEditLex->SetupWindow() ;
    return ;
  }
  NSEditNoLex* pEditNoLex = TYPESAFE_DOWNCAST(pCtrl, NSEditNoLex) ;
  if (pEditNoLex)
  {
    pEditNoLex->SetupWindow() ;
    return ;
  }

  // Buttons (radio, check...)
  //
  NSRadioButton* pRadio = TYPESAFE_DOWNCAST(pCtrl, NSRadioButton) ;
  if (pRadio)
  {
    pRadio->SetupWindow() ;
    return ;
  }
  NSCheckBox* pCheck = TYPESAFE_DOWNCAST(pCtrl, NSCheckBox) ;
  if (pCheck)
  {
    pCheck->SetupWindow() ;
    return ;
  }
  NSButton* pButton = TYPESAFE_DOWNCAST(pCtrl, NSButton) ;
  if (pButton)
  {
    pButton->SetupWindow() ;
    return ;
  }

  // Trees
  //
  NSTreeWindow* pTree = TYPESAFE_DOWNCAST(pCtrl, NSTreeWindow) ;
  if (pTree)
  {
    pTree->SetupWindow() ;
    return ;
  }

  // Combos
  //
  NSComboSemantique* pSemantic = TYPESAFE_DOWNCAST(pCtrl, NSComboSemantique) ;
  if (pSemantic)
  {
    pSemantic->SetupWindow() ;
    return ;
  }
  NSComboClassif* pClassif = TYPESAFE_DOWNCAST(pCtrl, NSComboClassif) ;
  if (pClassif)
  {
    pClassif->SetupWindow() ;
    return ;
  }

  // Tabs
  //
  NSOnglet* pTab = TYPESAFE_DOWNCAST(pCtrl, NSOnglet) ;
  if (pTab)
  {
    pTab->SetupWindow() ;
    return ;
  }

  // Lists
  //
  NSHistorizedValListWindow* pHVList = TYPESAFE_DOWNCAST(pCtrl, NSHistorizedValListWindow) ;
  if (pHVList)
  {
    pHVList->SetupWindow() ;
    return ;
  }
  NSHistorizedListWindow* pHList = TYPESAFE_DOWNCAST(pCtrl, NSHistorizedListWindow) ;
  if (pHList)
  {
    pHList->SetupWindow() ;
    return ;
  }
  NSAdrListWindow* pAdrList = TYPESAFE_DOWNCAST(pCtrl, NSAdrListWindow) ;
  if (pAdrList)
  {
    pAdrList->SetupWindow() ;
    return ;
  }
  NSCorListWindow* pCorList = TYPESAFE_DOWNCAST(pCtrl, NSCorListWindow) ;
  if (pCorList)
  {
    pCorList->SetupWindow() ;
    return ;
  }

  // Scroll bars
  //
/*
  TScrollBar* pScroll = TYPESAFE_DOWNCAST(pCtrl, TScrollBar) ;
  if (pScroll)
  {
    pScroll->SetupWindow() ;
    return ;
  }
*/
}

void
NSBBMUEView::executeClosingFunctions()
{
  _bClosing = true ;

  if ((NULL == _pNSCtrl) || (true == _pNSCtrl->empty()))
		return ;

	for (iterNSControle i = _pNSCtrl->begin() ; _pNSCtrl->end() != i ; i++)
		if ((*i)->getFonction())
    	(*i)->getFonction()->execute(NSDLGFCT_FERMETURE) ;
}

void
NSBBMUEView::executePreClosingFunctions()
{
  if ((NULL == _pNSCtrl) || (true == _pNSCtrl->empty()))
		return ;

	for (iterNSControle i = _pNSCtrl->begin() ; _pNSCtrl->end() != i ; i++)
		if ((*i)->getFonction())
    	(*i)->getFonction()->execute(NSDLGFCT_PREFERME) ;
}

void
NSBBMUEView::disconnectControls()
{
  if ((NULL == _pNSCtrl) || (true == _pNSCtrl->empty()))
		return ;

  // Disconnect controls to make certain they are not late modified
  //
  for (iterNSControle i = _pNSCtrl->begin() ; _pNSCtrl->end() != i ; i++)
		(*i)->setDisconnected(true) ;
}

void
NSBBMUEView::SetFocusToNextControl(OWL::TControl* pTControle)
{
  NSControlsArray* pControlsArray = getActiveControls() ;
  if ((NULL == pControlsArray) || pControlsArray->empty())
    return ;

  NSControlsIter itCtrl = pControlsArray->begin() ;

  if (NULL == pTControle)
  {
    bool bSearch = true ;
    while (bSearch)
    {
      if (pControlsArray->end() == itCtrl)
        return ;

      if (isControlProperty(*itCtrl, WS_TABSTOP) &&
          isControlProperty(*itCtrl, WS_VISIBLE))
        bSearch = false ;
      else
        itCtrl++ ;
    }
  }
  else
  {
    HWND hControlWnd = pTControle->HWindow ;

    for ( ; (pControlsArray->end() != itCtrl) && (hControlWnd != (*itCtrl)->HWindow) ; itCtrl++) ;

    bool bSearch = true ;
    while (bSearch)
    {
      if (pControlsArray->end() == itCtrl)
        itCtrl = pControlsArray->begin() ;
      else
      {
        itCtrl++ ;
        if (pControlsArray->end() == itCtrl)
          itCtrl = pControlsArray->begin() ;
      }

      if (hControlWnd == (*itCtrl)->HWindow)
        return ;

      if (isControlProperty(*itCtrl, WS_TABSTOP) &&
          isControlProperty(*itCtrl, WS_VISIBLE))
        bSearch = false ;
    }
  }

  (*itCtrl)->SetFocus() ;

  // We want Edit controls to be in "full selection" state
  //
  TEdit* pEdit = TYPESAFE_DOWNCAST(*itCtrl, TEdit) ;
	if (pEdit)
	{
    int iTextSize = pEdit->GetTextLen() ;
    if (iTextSize < 1)
      return ;

    pEdit->SetSelection(0, iTextSize) ;
  }
}

void
NSBBMUEView::SetFocusToPreviousControl(OWL::TControl* pTControle)
{
  NSControlsArray* pControlsArray = getActiveControls() ;
  if ((NULL == pControlsArray) || pControlsArray->empty())
    return ;

  NSControlsIter itCtrl = pControlsArray->begin() ;

  if (NULL == pTControle)
  {
    itCtrl = pControlsArray->end() ;
    itCtrl-- ;

    bool bSearch = true ;
    while (bSearch)
    {
      if (isControlProperty(*itCtrl, WS_TABSTOP) &&
          isControlProperty(*itCtrl, WS_VISIBLE))
        bSearch = false ;
      else if (pControlsArray->begin() == itCtrl)
        return ;
      else
        itCtrl-- ;
    }
    (*itCtrl)->SetFocus() ;
    return ;
  }
  else
  {
    HWND hControlWnd = pTControle->HWindow ;

    for ( ; (pControlsArray->end() != itCtrl) && (hControlWnd != (*itCtrl)->HWindow) ; itCtrl++) ;

    bool bSearch = true ;
    while (bSearch)
    {
      if (pControlsArray->begin() == itCtrl)
        itCtrl = pControlsArray->end() ;
      itCtrl-- ;

      if (hControlWnd == (*itCtrl)->HWindow)
        return ;

      if (isControlProperty(*itCtrl, WS_TABSTOP) &&
          isControlProperty(*itCtrl, WS_VISIBLE))
        bSearch = false ;
    }
  }

  (*itCtrl)->SetFocus() ;

  // We want Edit controls to be in "full selection" state
  //
  TEdit* pEdit = TYPESAFE_DOWNCAST(*itCtrl, TEdit) ;
	if (pEdit)
	{
    int iTextSize = pEdit->GetTextLen() ;
    if (iTextSize < 1)
      return ;

    pEdit->SetSelection(0, iTextSize) ;
  }
}

void
NSBBMUEView::initialiseControles()
{
  if ((NULL == _pNSCtrl) || _pNSCtrl->empty())
		return ;

	for (iterNSControle it = _pNSCtrl->begin() ; _pNSCtrl->end() != it ; it++)
    //
    // On lance l'éventuelle fonction d'initialisation
    //
		if ((*it)->getFonction())
    	(*it)->getFonction()->execute(NSDLGFCT_CREATION) ;
}

void
NSBBMUEView::initControlesFromBbk(string sAction)
{
  string ps = string("Dialog \"") + string(Title) + string("\" --> entering initControlesFromBbk") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  // No use to init from bbk if patient is not opened
	//
	if (NULL == pContexte->getPatient())
		return ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;
  if (NULL == pContexte->getBBinterface())
		return ;

	pContexte->getBBinterface()->connectTokenToWindow(_pBBItem->_KsInterface.getTokenId(), HWindow) ;

	if ((NULL == _pNSCtrl) || _pNSCtrl->empty())
  	return ;

  // L'archetype est par convention dans le champ fils du BBItem lanceur
  string sArchetype = _pBBItem->getItemSonsList() ;

  pSuper->afficheStatusMessage("Interrogation du Blackboard...") ;

  bool bAsynchronousQuestionsAsked = false ;

  for (iterNSControle i = _pNSCtrl->begin(); _pNSCtrl->end() != i ; i++)  {  	if ((*i)->getTransfert() && ((*i)->isToFilling(sAction)))    {      string sCheminBBFilsItem = string("") ;    	(*i)->getPathForBlackboard(&sCheminBBFilsItem) ;      NSSearchStruct searchStruct = *((*i)->getSearchStruct()) ;
      // searchStruct._sStartingDatePattern = (*i)->getFillingEndPeriod() ;      // (*i)->getEndValidityDate(&(searchStruct._sStartingDate)) ;

      // on a ici un chemin non vide = une question à poser au blackboard
      NSPatPathoArray* pPatPathoArray = NULL ;
      string date_fils ;

      // on pose la question au blackboard
			int res = pContexte->getBBinterface()->precoche(sCheminBBFilsItem, &pPatPathoArray, HWindow, &date_fils, true, &searchStruct) ;
			if ((1 == res) && (pPatPathoArray) && (false == pPatPathoArray->empty()))
        // il y a une réponse du blackboard ==>
        // on transmet la patpatho au BBFilsItem via le pTransfert
        // On met à jour le contrôle
        //
				initControlFromBbkAnswer(*i, pPatPathoArray, &date_fils) ;

      else if ((*i)->initOnTimer())
      {
      	bAsynchronousQuestionsAsked = true ;
        // TypedVal typ(sCheminBBFilsItem) ;
      	// AskDeterministicQuestion *DPIOMessage = new AskDeterministicQuestion("From dialog", &typ, 10) ;
  			// pSuper->bbkToDo(pBBItem->pBigBoss->pContexte, 0, "AskDeterministicQuestion", "", "", DPIOMessage, true, NULL, false) ;
      }
      if (pPatPathoArray)
        delete pPatPathoArray ;

      pSuper->getApplication()->PumpWaitingMessages() ;
		}
  }  if (bAsynchronousQuestionsAsked)  {    ps = string("Dialog \"") + string(Title) + string("\" --> quitting initControlesFromBbk - some controls still need to be initialized") ;    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;  	// SetTimer(ID_OB1_TIMER, 1000) ; // Now OB1 warns interfaces for changes  }  else  {    ps = string("Dialog \"") + string(Title) + string("\" --> quitting initControlesFromBbk - no control still needs to be initialized") ;    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;  }}

void
NSBBMUEView::initControlFromBbkAnswer(NSControle* pControl, NSPatPathoArray* pPatPathoArray, string* psDate)
{
  if (NULL == pControl)
  	return ;
  if ((NULL == pPatPathoArray) || pPatPathoArray->empty())
  	return ;

  pControl->initFromArray(pPatPathoArray) ;

  // If Edit control, tell him to stop waiting
  //
  if (pControl->getType() == isEdit)
  {
    NSEdit* pNSEditControl = static_cast<NSEdit*>(pControl->getControle()) ;
    if (pNSEditControl)
      pNSEditControl->setWaitingForOb1(false) ;
  }

  if (NULL == psDate)
		return ;

  //
  // Recherche d'un contrôle qui contiendrait la date de cette information
  // We look for another control whose subject would be this control's date
  //
  if (_pNSCtrl->empty())
    return ;

  iterNSControle k ;
  for (k = _pNSCtrl->begin(); (_pNSCtrl->end() != k) && (*k != pControl); k++) ;
  if (_pNSCtrl->end() == k)
  	return ;

  string sIdentity = pControl->getIdentite() ;
  bool trouve = false ;

  k++ ;

  while (_pNSCtrl->end() != k)
  {
    if (((*k)->getFilling() == "D") && ((*k)->getPathControl() == sIdentity))
    {
      trouve = true ;
      break ;
    }

    k++ ;
  }

  if (false == trouve)
		return ;

  // on regarde si on a une date ou une date et heure
  string sDateIdent = (*k)->getIdentite() ;
  string typeDate = "" ;
  size_t pos = sDateIdent.find_last_of("/£") ;
  if (pos != string::npos)
  	typeDate = string(sDateIdent, pos, 2) ;

  string date_fils = *psDate ;

  if ((typeDate == "£T") || (typeDate == "£D"))
  {
  	NVLdVTemps tpsObj ;
    tpsObj.initFromDate(date_fils) ;

    if (typeDate == "£T")
    	date_fils = tpsObj.donneDateHeure() ;
    else
    	date_fils = tpsObj.donneDate() ;
  }

  (*k)->getTransfert()->getTransfertMessage()->SetComplement(date_fils) ;
  (*k)->getTransfert()->getFilsItem()->Active() ; //rendre ce fils actif
  (*k)->prepareControle() ;
}

void
NSBBMUEView::CmBbkUpdate()
{
  _bNeedToRefresh = true ;

  if (false == _bAcceptUpdate)
    return ;

  char szHandle[20] ;
  sprintf(szHandle, "%p", HWindow) ;
  string ps = string("NSBBMUEView ") + string(szHandle) + string(" update asked by BBK") ;

  if (false == IsWindow())
  {
    ps += string(" (skipping; not a valid window)") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
    return ;
  }

  if (_bClosing)
  {
    ps += string(" (skipping; closing on duty)") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
    return ;
  }

  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  checkPrecocheOnTimer() ;

  _bNeedToRefresh = false ;
}

void
NSBBMUEView::EvTimer(uint id)
{
	if      (ID_OB1_TIMER == id)
		checkPrecocheOnTimer() ;

	else if (ID_OB1_TIMER2 == id)
		checkResultOnTimer() ;

  else if (ID_KSWAIT_TIMER == id)
		stillWaitingForKsTimer() ;

	return ;
}

void
NSBBMUEView::checkPrecocheOnTimer()
{
	if ((NULL == _pNSCtrl) || _pNSCtrl->empty())
		return ;

  char szHandle[20] ;
  sprintf(szHandle, "%p", HWindow) ;
  string ps = string("Entering NSBBMUEView::checkPrecocheOnTimer for window ") + string(szHandle) ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // L'archetype est par convention dans le champ fils du BBItem lanceur
  string sArchetype = _pBBItem->getItemSonsList() ;

  pSuper->afficheStatusMessage("Interrogation du Blackboard...") ;

  bool bSomeAnswerStillMissing = false ;

  for (iterNSControle i = _pNSCtrl->begin() ; _pNSCtrl->end() != i ; i++)  {  	if ((*i) && (*i)->getTransfert() && ((*i)->isEmpty() || (false == (*i)->hasBeenValidated()))) // && ((*i)->initOnTimer()))
    {
    	string sCheminBBFilsItem ;
    	(*i)->getPathForBlackboard(&sCheminBBFilsItem) ;

      // on a ici un chemin non vide = une question à poser au blackboard
      NSPatPathoArray* pPatPathoArray = NULL ;

      string date_fils = string("") ;

      NSSearchStruct searchStruct = *((*i)->getSearchStruct()) ;
      // searchStruct._sStartingDatePattern = (*i)->getFillingEndPeriod() ;      // (*i)->getEndValidityDate(&(searchStruct._sStartingDate)) ;

      // on pose la question au blackboard
			int res = pContexte->getBBinterface()->precoche(sCheminBBFilsItem, &pPatPathoArray, HWindow, &date_fils, false, &searchStruct) ;
			if ((1 == res) && (pPatPathoArray) && (false == pPatPathoArray->empty()))
        // il y a une réponse du blackboard ==>
        // on transmet la patpatho au BBFilsItem via le pTransfert
        // On met à jour le contrôle
        //
      	initControlFromBbkAnswer(*i, pPatPathoArray, &date_fils) ;
      else
      {
        // If Edit control, tell him that it is waiting for BBK answer
        //
        if ((*i)->getType() == isEdit)
        {
          NSEdit* pNSEditControl = static_cast<NSEdit*>((*i)->getControle()) ;
          if (pNSEditControl)
            pNSEditControl->setWaitingForOb1(true) ;
        }

      	bSomeAnswerStillMissing = true ;
      }

      // pSuper->getApplication()->PumpWaitingMessages() ;
    }
  }
  // if (false == bSomeAnswerStillMissing)
  //	KillTimer(ID_OB1_TIMER) ;

  if (false == bSomeAnswerStillMissing)
  {
    for (iterNSControle i = _pNSCtrl->begin() ; _pNSCtrl->end() != i ; i++)
    {  	  if ((*i) && ((*i)->getType() == isEdit))
      {
        NSEdit* pNSEditControl = static_cast<NSEdit*>((*i)->getControle()) ;
        if (pNSEditControl)
          pNSEditControl->setWaitingForOb1(false) ;
      }
    }
  }

  ps = string("Leaving NSBBMUEView::checkPrecocheOnTimer for window ") + string(szHandle) ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSBBMUEView::checkResultOnTimer()
{
}

void
NSBBMUEView::stillWaitingForKsTimer()
{
  int iMaxTimeBeforeStop = 90 ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;
  NVLdVTemps tpsCompare(_tpsBBKStart) ;
  tpsCompare.ajouteSecondes(iMaxTimeBeforeStop) ;
  if (tpsCompare > tpsNow)
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sWarningMessage = pSuper->getText("bbkKsManagement", "thisKsSeemsHanged") ;
  string sMsg = pSuper->getText("bbkKsManagement", "doYouWantToResetIt") ;
  sWarningMessage += string(1, '\n') + sMsg ;
  int rep = MessageBox(sWarningMessage.c_str(), "", MB_YESNO) ;
  if (IDYES == rep)
  {
    _bWaitingForKsAnswer = false ;
    KillTimer(ID_KSWAIT_TIMER) ;
  }
  else
    _tpsBBKStart.takeTime() ;
}

void
NSBBMUEView::initControlesFromCapture()
{
  if (pContexte->getEpisodus() == NULL)
    return ;

  NSCaptureArray* pCapt = &(pContexte->getEpisodus()->CaptureArray) ;
  if (pCapt->empty())
    return ;

  // ici on en est
  for (iterNSControle i = _pNSCtrl->begin() ; _pNSCtrl->end() != i ; i++)
  {
    if ((*i)->getTransfert())
    {
      // on récupère le chemin du BBItem père du BBFilesItem associé au
      // NSControle via son transfert
      //
      BBFilsItem* pFilsItem = (*i)->getTransfert()->getFilsItem() ;

      string sCheminBBFilsItem = string("");
      string sEtiq = string("") ;

      if (pFilsItem)
      {
      	// on récupère le chemin du BBItem père
      	sCheminBBFilsItem = (*i)->getTransfert()->getFilsItem()->getItemFather()->_sLocalisation ;
      	// on récupère l'étiquette du fils
      	string  sEtiquette = (*i)->getTransfert()->getFilsItem()->getItemLabel() ;
      	NSDico::donneCodeSens(&sEtiquette, &sEtiq) ;
      }

      for (CaptureIter captIter = pCapt->begin() ; pCapt->end() != captIter ; captIter++)
      {
      	if ((*captIter)->getChemin() != string(""))
        {
          //
          // On regarde si le chemin de l'élément se termine par le chemin
          // de capture
          //
          string sCheminCapture ;
          string sPath = (*captIter)->getChemin() ;
          NSDico::donneCodeSens(&sPath, &sCheminCapture) ;

          // Attention, la détection automatique ajoute GCONS1, il faut le
          // supprimer
          if ((strlen(sCheminCapture.c_str()) > 6) && (string(sCheminCapture, 0, 6) == "GCONS/"))
            sCheminCapture = string(sCheminCapture, 6, strlen(sCheminCapture.c_str())-6) ;

          string sCheminCompare = "" ;

          size_t iCaptLen = strlen(sCheminCapture.c_str()) ;
          size_t iBBLen   = strlen(sCheminBBFilsItem.c_str()) ;

          if (iBBLen >= iCaptLen)
              sCheminCompare = string(sCheminBBFilsItem, iBBLen - iCaptLen, iCaptLen) ;

          //if (sCheminCapture == sCheminBBFilsItem)
          if (sCheminCompare == sCheminCapture)
          {
            if      (sEtiq[0] == '£')
            {
              (*i)->getTransfert()->getTransfertMessage()->SetTexteLibre((*captIter)->getLibelle()) ;
              (*i)->getTransfert()->getFilsItem()->Active() ;
            }
            else if (sEtiq[0] == '2')
            {
              size_t iChemMark = sEtiq.find(cheminSeparationMARK) ;
              if (string::npos != iChemMark)
              {
                string  sUnit   = string(sEtiq, 0, iChemMark) ;
                string  sLexiq  = string(sEtiq, iChemMark + 1, strlen(sEtiq.c_str()) - iChemMark - 1) ;

                string sValeur = (*captIter)->getLibelle() ;

                // Conversion d'unités
                if (((*captIter)->getUnit() != string("")) && (sUnit != (*captIter)->getUnit()))
                {
                  NSCV NsCv(pContexte->getSuperviseur()) ;
                  DBIResult Resultat = NsCv.open() ;
                  if (Resultat != DBIERR_NONE)
                  {
                    erreur("Erreur à l'ouverture de la base convert.", standardError, Resultat) ;
                    sValeur = "" ;
                  }
                  else
                  {
                    double dVal = StringToDouble(sValeur) ;
                    if (NsCv.ConvertirUnite(&dVal, sUnit, (*captIter)->getUnit()))
                      sValeur = DoubleToString(&dVal, 0, 2) ;
                    else
                      sValeur = "" ;

                    NsCv.close();
                  }
                }

                if (sValeur != "")
                {
                  (*i)->getTransfert()->getTransfertMessage()->SetLexique(sLexiq) ;
                  (*i)->getTransfert()->getTransfertMessage()->SetUnit(sUnit) ;
                  (*i)->getTransfert()->getTransfertMessage()->SetComplement(sValeur) ;
                  (*i)->getTransfert()->getFilsItem()->Active() ;
                }
              }
            }
            else
            {
              string sLabel ;
              string sLib = (*captIter)->getLibelle() ;
              NSDico::donneCodeSens(&sLib, &sLabel) ;
              if (sEtiq == sLabel)
                (*i)->getTransfert()->getFilsItem()->Active() ;
            }
            (*i)->executeKillFocusBehaviour() ;
            (*i)->prepareControle() ;
            break ;
          }
        }
      }
    }
  }
}

int
NSMUEView::getWindowXFromDialogX(int iX)
{
  return MulDiv(iX, _cxSysChar, 4) ;
}

int
NSMUEView::getWindowYFromDialogY(int iY)
{
  return MulDiv(iY, _cySysChar, 8) ;
}

ClassLib::TRect
NSMUEView::getWindowRectFromDialogRect(int iX, int iY, int iW, int iH)
{
  int newX = getWindowXFromDialogX(iX) ;
  int newY = getWindowYFromDialogY(iY) ;
  int newW = getWindowXFromDialogX(iW) ;
  int newH = getWindowYFromDialogY(iH) ;

  return ClassLib::TRect(newX, newY, newX + newW, newY + newH) ;
}

int
NSBBMUEView::getDialogXFromWindowX(int iX)
{
  return MulDiv(iX, 4, _cxSysChar) ;
}

int
NSBBMUEView::getDialogYFromWindowY(int iY)
{
  return MulDiv(iY, 8, _cySysChar) ;
}

ClassLib::TRect
NSBBMUEView::getDialogRectFromWindowRect(int iX, int iY, int iW, int iH)
{
  int newX = getDialogXFromWindowX(iX) ;
  int newY = getDialogYFromWindowY(iY) ;
  int newW = getDialogXFromWindowX(iW) ;
  int newH = getDialogYFromWindowY(iH) ;

  return ClassLib::TRect(newX, newY, newX + newW, newY + newH) ;
}

bool
NSBBMUEView::CanSave()
{
  if (NULL == _pBBItem)
    return true ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  pSuper->getApplication()->PumpWaitingMessages() ;

  //
  // Are controls happy with their contents
  //
  if ((_pNSCtrl) && (false == _pNSCtrl->empty()))
  {
    THandle hWndFocused = GetFocus() ;

  	for (iterNSControle i = _pNSCtrl->begin() ; _pNSCtrl->end() != i ; i++)
    {
      // TreeWindow: get free text that would be in "editing" state
      //
      if ((*i)->getType() == isTreeWindow)
      {
      	NSTreeWindow* pNSTreeWindow = static_cast<NSTreeWindow*>((*i)->getControle()) ;
        if (pNSTreeWindow && pNSTreeWindow->getEditDico())
        	pNSTreeWindow->getEditDico()->TextLibre() ;
      }
      else if ((*i)->getType() == isEditDate)
      {
      	NSEditDate* pEditDate = static_cast<NSEditDate*>((*i)->getControle()) ;
        if (pEditDate && (pEditDate->HWindow == hWndFocused))
        {
          bool bCanLetItBe = true ;
          pEditDate->actionForKillFocus(bCanLetItBe) ;
        	if (false == bCanLetItBe)
      	    return false ;
        }
      }
      else if ((*i)->getType() == isEditDateHeure)
      {
      	NSEditDateHeure* pEditDate = static_cast<NSEditDateHeure*>((*i)->getControle()) ;
        if (pEditDate && (pEditDate->HWindow == hWndFocused))
        {
          bool bCanLetItBe = true ;
          pEditDate->actionForKillFocus(bCanLetItBe) ;
        	if (false == bCanLetItBe)
      	    return false ;
        }
      }
      else if ((*i)->getType() == isEditNoLexique)
      {
      	NSEditNoLex* pEditNoLex = static_cast<NSEditNoLex*>((*i)->getControle()) ;
        if (pEditNoLex && (pEditNoLex->HWindow == hWndFocused))
        {
          bool bCanLetItBeAndExecPostExec = true ;
          pEditNoLex->actionForKillFocus(bCanLetItBeAndExecPostExec) ;
        	if (false == bCanLetItBeAndExecPostExec)
      	    return false ;
        }
      }

    	if (false == (*i)->canWeClose())
      	return false ;
    }
  }

  // Checking constraints
  //
  string sMessage ;
	if (false == _pBBItem->verifConstraintItem(&sMessage))
	{
    string sWarningMsg = pSuper->getText("archetypesMessages", "someMandatoryInformationIsMissing") ;
    if (string("") != sMessage)
      sWarningMsg += string(" ") + sMessage ;
  	erreur(sWarningMsg.c_str(), warningError, 0) ;
    return false ;
  }

  return true ;
}

bool
NSBBMUEView::isInEditMode()
{
  NSNoyauDocument* pDocNoy = static_cast<NSNoyauDocument*>(pDoc) ;
  if (pDocNoy)
    return (NULL != pDocNoy->DonneDocInfo()) ;

  return false ;
}

// Open previous document of the kind
//
void
NSBBMUEView::CmPrevious()
{
  if ((NULL == pContexte->getPatient()) || (NULL == _pBBItem))
    return ;

  NSHISTODocument* pDocManager = pContexte->getPatient()->getDocHis() ;
  if ((NSHISTODocument*) NULL == pDocManager)
    return ;

  Citem* pRootItem = _pBBItem->getArchetypeRootItem() ;
  if ((Citem*) NULL == pRootItem)
    return ;

  Citem* pFirstItem = pRootItem->getFirstSon() ;
  if ((Citem*) NULL == pFirstItem)
    return ;

  string sLexique = pFirstItem->getCode() ;
  DocumentIter iterPrevDoc = pDocManager->DonnePrevPatPathoDocument(sLexique, (NSPatPathoArray*) 0, 0) ;
  if ((DocumentIter) NULL == iterPrevDoc)
    return ;

  // pDocManager->AutoriserOuverture((NSDocumentInfo*) *iterPrevDoc) ;

  NSToDoTask* pTask = new NSToDoTask ;

  pTask->setWhatToDo(string("OpenDocument")) ;
  pTask->setParam1((*iterPrevDoc)->getID()) ;

  pTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSBBMUEView::Attach(NSWindowsArray *pWindowArray)
{
  if ((NULL == pWindowArray) || (false == pWindowArray->empty()))
    return ;

  for (NSWindowsIter i = pWindowArray->begin() ; pWindowArray->end() != i ; i++)
    Attach(*i) ;
}

void
NSBBMUEView::Attach(NSOneTabWindow *newChildWindow, LPCTSTR /* title */)
{
try
{
	// Add to collection of TWindow*'s
	_tabbedWindows->push_back(newChildWindow) ;
    /************************
	if (GetHandle())
	{
		// Reparent the given widget
		newChildWindow->SetParent(this) ;

		if (!newChildWindow->GetHandle())
			newChildWindow->Create() ;
	}
    *****************************/

	if (_m_tabCtrl)
	{
  	// Retrieve window's text, use that for tab text
    TAPointer<_TCHAR> txt = new _TCHAR[newChildWindow->GetWindowTextLength() + 1] ;
    newChildWindow->GetWindowText(txt, newChildWindow->GetWindowTextLength() + 1) ;
    _m_tabCtrl->Add(TTabItem(txt)) ;
    // Select new tab as current
    SetSelectedTab(_m_tabCtrl->GetCount() - 1) ;
    newChildWindow->Show(SW_SHOW) ;
    // newChildWindow->EnableWindow(true) ;
  }
}
catch (...)
{
	erreur("Exception NSTabWindow::Attach.", standardError, 0) ;
}
}

NSOneTabWindow*
NSBBMUEView::Detach(int index)
{
	NSOneTabWindow *retval = (*_tabbedWindows)[index] ;

	// Remove window element from list
    //	tabbedWindows->Detach(index) ;

	return retval ;
}

int
NSBBMUEView::Retrieve(NSOneTabWindow *ptr)
{
	int index = 0 ;
  if (false == _tabbedWindows->empty())
    for (NSWindowsIter iter = _tabbedWindows->begin() ; _tabbedWindows->end() != iter ; iter++, index++)
      if (*iter == ptr)
        return index ;

	// throw an exception instead?
	return -1 ;
}

NSOneTabWindow*
NSBBMUEView::Detach(NSOneTabWindow *ptr)
{
  return Detach(Retrieve(ptr)) ;
}

NSOneTabWindow*
NSBBMUEView::Retrieve(int index)
{
  return ((*_tabbedWindows)[index]) ;
}

NSOneTabWindow*
NSBBMUEView::operator[](int index)
{
  return ((*_tabbedWindows)[index]) ;
}

int
NSBBMUEView::operator[](NSOneTabWindow *win)
{
  return Retrieve(win) ;
}

NSControlsArray*
NSBBMUEView::getActiveControls()
{
  if (NULL == _tabbedWindows)
    return 0 ;

  NSOneTabWindow* pTab ;
  if (_tabbedWindows->size() == 1)
    pTab = *(_tabbedWindows->begin()) ;
  else
    pTab = (*_tabbedWindows)[_m_tabCtrl->GetSel()] ;

  if (NULL == pTab)
    return 0 ;

  return pTab->controls ;
}

void
NSBBMUEView::SetSelectedTab(int index)
{
  _m_tabCtrl->SetSel(index) ;

  // TTabControl does not send the TCN_ messages, so we need to "fake" it
  TNotify     dummy ;
  TabSelChanging(dummy) ;
  TabSelChange(dummy) ;
}

void
NSBBMUEView::TabSelChange(TNotify far&)
{
	AdjustTabClient() ;
	// Here, GetSel() returns the after-the-click-current tab selected
	(*_tabbedWindows)[_m_tabCtrl->GetSel()]->Show(SW_SHOW) ;
  // (*_tabbedWindows)[m_tabCtrl->GetSel()]->EnableWindow(true) ;
	(*_tabbedWindows)[_m_tabCtrl->GetSel()]->SetWindowPos(HWND_TOP, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW) ;
}

bool
NSBBMUEView::TabSelChanging(TNotify far&)
{
	// Here, GetSel() returns the before-the-click-current tab selected
	if ((*_tabbedWindows)[_m_tabCtrl->GetSel()])
	{
		(*_tabbedWindows)[_m_tabCtrl->GetSel()]->Show(SW_HIDE) ;
		// (*_tabbedWindows)[m_tabCtrl->GetSel()]->EnableWindow(false) ;
	}
	return false ;
}

void
NSBBMUEView::TabKeyDown(TTabKeyDown far&)
{
}

int
NSBBMUEView::AdjustTabClient()
{
  int height = 0 ;
  int width  = 0 ;

	// Retrieve pointer to tab control, the client window
	if (_m_tabCtrl && _m_tabCtrl->GetHandle())
	{
		// Retrieve the window rectangle of the tab control
		ClassLib::TRect rect;
		_m_tabCtrl->GetWindowRect(rect);

		// NOTE: GetWindowRect returns screen coordinates... we'll need
		//       to have the coordinates relative to the frame window,
		//       the parent of both the tab and client window
		::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rect, 2);

		// Ask the tab for it's 'client-area' based in it window location
		_m_tabCtrl->AdjustRect(false, rect);
        height = rect.top;
        width = rect.left;

		// Move the Client window
		// TWindow *Client = (*tabbedWindows)[m_tabCtrl->GetSel()];
		// if (Client && Client->GetHandle())
		//  	Client->SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);

    int k = 0;
    if (false == _tabbedWindows->empty())
    {
      for (NSWindowsIter i = _tabbedWindows->begin() ; _tabbedWindows->end() != i ; i++, k++)
      {
        if ((!_bControlInit) && ((*i)->controls) && (false == (*i)->controls->empty()))
          for (NSControlsIter j = (*i)->controls->begin() ; (*i)->controls->end() != j ; j++)
            (*j)->MoveWindow((*j)->Attr.X + width, (*j)->Attr.Y + height, (*j)->Attr.W, (*j)->Attr.H) ;

        if (k == _m_tabCtrl->GetSel())
        {
          // Move the Client window
          // NSOneTabWindow *Client = *i;
          // if (Client && Client->GetHandle())
          //   Client->SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);

          if (((*i)->controls) && (false == (*i)->controls->empty()))
          {
            for (NSControlsIter j = (*i)->controls->begin() ; (*i)->controls->end() != j ; j++)
            {
              bool bChildIsVisible = true ; // (((*j)->GetStyle() & WS_VISIBLE) == WS_VISIBLE) ;

              NSControle* pCtrl = getNSControl(*j) ;
              if (pCtrl)
                bChildIsVisible = pCtrl->isVisible() ;

              if (bChildIsVisible)
                (*j)->Show(SW_SHOW) ;

              // Don't do this because it switchs the control to "WS_VISIBLE"
              // even for hidden controls
              // (*j)->EnableWindow(true) ;
              (*j)->SetWindowPos(HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW) ;
            }
          }
        }
        else
        {
          if (((*i)->controls) && (false == (*i)->controls->empty()))
            for (NSControlsIter j = (*i)->controls->begin() ; (*i)->controls->end() != j ; j++)
            {
              (*j)->Show(SW_HIDE) ;
              // Don't do this because it switchs the control to "NOT WS_VISIBLE"
              // and it becomes undistinguishable from true hidden controls
              // (*j)->EnableWindow(false) ;
            }
        }
      }
    }

    if (false == _bControlInit)
      _bControlInit = true ;
	}

  return height ;
}

void
NSBBMUEView::EvKeyUp(uint key, uint repeatCount, uint flags)
{
  if (VK_F1 == key)
		CmHelp() ;

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}

TResult
NSBBMUEView::EvSetBackColor(TParam1 param1, TParam2 param2)
{
  HBRUSH hbr = (HBRUSH)DefWindowProc(GetCurrentEvent().Message, param1, param2) ;
  ::DeleteObject(hbr) ;
  SetBkMode((HDC)param1, TRANSPARENT) ;
  return (TResult)::GetStockObject(NULL_BRUSH) ;
}

bool
NSBBMUEView::PreProcessMsg(MSG &msg)
{
  if (_pToolTip && _pToolTip->IsWindow())
    _pToolTip->RelayEvent(msg) ;
  return NSMUEView::PreProcessMsg(msg) ;
}

//-----------------------------------------------------------------------//
//				   	    Classe NSWindowProperty                          //
//-----------------------------------------------------------------------//

// Default ctor
NSWindowProperty::NSWindowProperty()
{
	_sFunction  = string("") ;

  _iX         = 0 ;  _iY         = 0 ;  _iW         = 0 ;  _iH         = 0 ;

  _posX       = undefpos ;
  _posY       = undefpos ;  _posW       = undefpos ;  _posH       = undefpos ;

  _paramSplit = new ArraySplitWindow() ;
  _iActivity  = available ;
  _cHotKey    = '\0' ;
  _sModifiers = string("") ;
}

// Copy ctor
NSWindowProperty::NSWindowProperty(const NSWindowProperty& rv)
{
	_sFunction  = rv._sFunction ;

  _iX         = rv._iX ;  _iY         = rv._iY ;  _iW         = rv._iW ;  _iH         = rv._iH ;

  _posX       = rv._posX ;
  _posY       = rv._posY ;  _posW       = rv._posW ;  _posH       = rv._posH ;

  _paramSplit = new ArraySplitWindow(*rv._paramSplit) ;
  _iActivity  = rv._iActivity ;
  _cHotKey    = rv._cHotKey ;
  _sModifiers = rv._sModifiers ;
}

// Destructor
NSWindowProperty::~NSWindowProperty(){	if (_paramSplit)		delete _paramSplit ;}intNSWindowProperty::X(NSSuper* pSuper){  switch(_posX)  {    case undefpos :      return _iX ;    case left :      return 0 ;    case right :    {      int iWidth = W(pSuper) ;      if (0 == iWidth)        iWidth = 100 ;      return getGlobalW(pSuper) - iWidth ;    }  }  return _iX ;}intNSWindowProperty::Y(NSSuper* pSuper){  switch(_posY)  {    case undefpos :      return _iY ;    case top :      return 0 ;    case bottom :    {      int iHeight = W(pSuper) ;      if (0 == iHeight)        iHeight = 100 ;      return getGlobalH(pSuper) - iHeight ;    }  }  return _iY ;}intNSWindowProperty::W(NSSuper* pSuper){  if (undefpos == _posW)    return _iW ;  if (automatic == _posW)  {    int iPotentialRectX = 0 ;    int iPotentialRectY = 0 ;    int iPotentialRectW = getGlobalW(pSuper) ;    int iPotentialRectH = 0 ;    if (automatic == _posH)    {      iPotentialRectH = getGlobalH(pSuper) ;      if (undefpos == _posY)      {        iPotentialRectY =  _iY ;        iPotentialRectH -= _iY ;      }    }    else    {      iPotentialRectH = _iH ;    }    if (undefpos == _posX)    {      iPotentialRectX =  _iX ;      iPotentialRectW -= _iX ;    }    return getAvailableWForRect(pSuper, NS_CLASSLIB::TRect(iPotentialRectX, iPotentialRectY, iPotentialRectX + iPotentialRectW, iPotentialRectY + iPotentialRectH)) ;  }  return _iW ;}intNSWindowProperty::H(NSSuper* pSuper){  if (undefpos == _posH)    return _iH ;  if (automatic == _posH)  {    int iPotentialRectX = 0 ;    int iPotentialRectY = 0 ;    int iPotentialRectW = 0 ;    int iPotentialRectH = getGlobalH(pSuper) ;    if (automatic == _posW)    {      iPotentialRectW = getGlobalW(pSuper) ;      if (undefpos == _posX)      {        iPotentialRectX =  _iX ;        iPotentialRectW -= _iX ;      }    }    else    {      iPotentialRectW = _iW ;    }    if (undefpos == _posY)    {      iPotentialRectY =  _iY ;      iPotentialRectH -= _iY ;    }    return getAvailableHForRect(pSuper, NS_CLASSLIB::TRect(iPotentialRectX, iPotentialRectY, iPotentialRectX + iPotentialRectW, iPotentialRectY + iPotentialRectH)) ;  }  return _iH ;}NS_CLASSLIB::TRectNSWindowProperty::getRect(NSSuper* pSuper)
{
	return NS_CLASSLIB::TRect(X(pSuper), Y(pSuper),
                            X(pSuper) + W(pSuper),
                            Y(pSuper) + H(pSuper)) ;
}
intNSWindowProperty::getGlobalH(NSSuper* pSuper){  NS_CLASSLIB::TRect globalRect ;  if (false == getMdiClientRect(pSuper, globalRect))    return 0 ;  return globalRect.Height() ;}intNSWindowProperty::getGlobalW(NSSuper* pSuper){  NS_CLASSLIB::TRect globalRect ;  if (false == getMdiClientRect(pSuper, globalRect))    return 0 ;  return globalRect.Width() ;}intNSWindowProperty::getAvailableHForRect(NSSuper* pSuper, NS_CLASSLIB::TRect rect){  TMyApp *pMyApp = pSuper->getApplication() ;  if ((TMyApp*) NULL == pMyApp)    return false ;  NSMDIClient* pMdiClient = pMyApp->prendClient() ;  if ((NSMDIClient*) NULL == pMdiClient)    return false ;  pMdiClient->getAvailableHForRect(rect) ;  return rect.Height() ;}intNSWindowProperty::getAvailableWForRect(NSSuper* pSuper, NS_CLASSLIB::TRect rect){  TMyApp *pMyApp = pSuper->getApplication() ;  if ((TMyApp*) NULL == pMyApp)    return false ;  NSMDIClient* pMdiClient = pMyApp->prendClient() ;  if ((NSMDIClient*) NULL == pMdiClient)    return false ;  pMdiClient->getAvailableWForRect(rect) ;  return rect.Width() ;}boolNSWindowProperty::getMdiClientRect(NSSuper* pSuper, NS_CLASSLIB::TRect& rect){  TMyApp *pMyApp = pSuper->getApplication() ;  if ((TMyApp*) NULL == pMyApp)    return false ;  NSMDIClient* pMdiClient = pMyApp->prendClient() ;  if ((NSMDIClient*) NULL == pMdiClient)    return false ;  pMdiClient->GetClientRect(rect) ;  return true ;}voidNSWindowProperty::initFromRect(NS_CLASSLIB::TRect rect){	_iX = rect.X() ;
	_iY = rect.Y() ;
	_iW = rect.Width() ;
	_iH = rect.Height() ;

  _posX = undefpos ;
  _posY = undefpos ;  _posW = undefpos ;  _posH = undefpos ;
}NSWindowProperty&
NSWindowProperty::operator=(const NSWindowProperty& src)
{
	if (this == &src)
		return *this ;

	_sFunction   = src._sFunction ;

  _iX          = src._iX ;  _iY          = src._iY ;  _iW          = src._iW ;  _iH          = src._iH ;

  _posX        = src._posX ;
  _posY        = src._posY ;  _posW        = src._posW ;  _posH        = src._posH ;

  _iActivity   = src._iActivity ;
  _cHotKey     = src._cHotKey ;
  _sModifiers  = src._sModifiers ;

try
{
	if (src._paramSplit)
		*_paramSplit = *(src._paramSplit) ;
  else
  	resetSplitList() ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSWindowProperty =operator.", standardError, 0) ;
	return *this ;
}
}

void
NSWindowProperty::resetSplitList()
{
  if (false == _paramSplit->empty())
  	_paramSplit->vider() ;
}

void
NSWindowProperty::addSplit(string sTargetFct, string sSplitFct, TSplitDirection dir, float percent)
{
  if (string("") == sSplitFct)
		return ;

	if (string("") == sTargetFct)
  	sTargetFct = _sFunction ;

	ArraySWIter iter;
	if (false == _paramSplit->empty())
  	for (iter = _paramSplit->begin(); (_paramSplit->end() != iter) && (((*iter)->getMotherWindow() != sTargetFct)
                                    || ((*iter)->getChildWindow() != sSplitFct)); iter++ ) ;

	if ((_paramSplit->empty()) || (_paramSplit->end() == iter))
	{
  	NSSplittingWindowProperty* pWinSplit = new NSSplittingWindowProperty() ;
    pWinSplit->setMotherWindow(sTargetFct);
    pWinSplit->setChildWindow(sSplitFct);
    pWinSplit->setSplitDirection(dir);
    pWinSplit->setPercent(percent);

    _paramSplit->push_back(pWinSplit);
  }
  else
  {
  	(*iter)->setSplitDirection(dir);
    (*iter)->setPercent(percent);
  }
}

UINT
NSWindowProperty::getModifiersAsInt()
{
	UINT Mods = 0 ;

  if (true == isInModifier(MOD_ALT))
  	Mods = Mods | MOD_ALT ;
	if (true == isInModifier(MOD_CONTROL))
  	Mods = Mods | MOD_CONTROL ;
  if (true == isInModifier(MOD_SHIFT))
  	Mods = Mods | MOD_SHIFT ;
  if (true == isInModifier(MOD_WIN))
  	Mods = Mods | MOD_WIN ;

	return Mods ;
}

boolNSWindowProperty::isInModifier(UINT iModConst){	switch (iModConst)  {  	case MOD_ALT     : return ((NPOS != _sModifiers.find("A")) || (NPOS != _sModifiers.find("a"))) ;    case MOD_CONTROL : return ((NPOS != _sModifiers.find("C")) || (NPOS != _sModifiers.find("c"))) ;    case MOD_SHIFT   : return ((NPOS != _sModifiers.find("S")) || (NPOS != _sModifiers.find("s"))) ;    case MOD_WIN     : return ((NPOS != _sModifiers.find("W")) || (NPOS != _sModifiers.find("w"))) ;  }  return false ;}voidNSWindowProperty::addToModifiers(UINT iModConst){	if (true == isInModifier(iModConst))		return ;	switch (iModConst)  {  	case MOD_ALT     : _sModifiers += string("A") ; break ;    case MOD_CONTROL : _sModifiers += string("C") ; break ;    case MOD_SHIFT   : _sModifiers += string("S") ; break ;    case MOD_WIN     : _sModifiers += string("W") ; break ;  }}
string
NSWindowProperty::getPosAsString()
{
  string sPosAsString = string("") ;

  if (undefpos == _posX)
    sPosAsString += IntToString(_iX) ;
  else
    sPosAsString += DynPosToString(_posX) ;

  sPosAsString += string(",") ;
  if (undefpos == _posY)
    sPosAsString += IntToString(_iY) ;
  else
    sPosAsString += DynPosToString(_posY) ;

  sPosAsString += string(",") ;
  if (undefpos == _posW)
    sPosAsString += IntToString(_iW) ;
  else
    sPosAsString += DynPosToString(_posW) ;

  sPosAsString += string(",") ;
  if (undefpos == _posH)
    sPosAsString += IntToString(_iH) ;
  else
    sPosAsString += DynPosToString(_posH) ;

  return sPosAsString ;
}

string
NSWindowProperty::DynPosToString(DYNAMICPOS pos)
{
  switch(pos)
  {
    case automatic : return string("auto") ;
    case left :      return string("left") ;
    case right :     return string("right") ;
    case top :       return string("top") ;
    case bottom :    return string("bottom") ;
  }
  return string("") ;
}

NSWindowProperty::DYNAMICPOS
NSWindowProperty::StringToDynPos(string sPos)
{
  if (string("auto") == sPos)
    return automatic ;
  if (string("left") == sPos)
    return left ;
  if (string("right") == sPos)
    return right ;
  if (string("top") == sPos)
    return top ;
  if (string("bottom") == sPos)
    return bottom ;

  return undefpos ;
}

//-----------------------------------------------------------------------//
//				   		 Classe ArrayWinProp                            //
//-----------------------------------------------------------------------//

ArrayWinProp::ArrayWinProp(const ArrayWinProp& rv)
             :ArrayWP(), NSRoot(rv.pContexte)
{
try
{
	if (false == rv.empty())
		for (ArrayWPConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSWindowProperty(*(*i))) ;
}
catch (...)
{
	erreur("Exception ArrayWinProp copy ctor.", standardError, 0) ;
}
}

void
ArrayWinProp::vider()
{
	if (empty())
		return ;

	for (ArrayWPIter i = begin(); end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

ArrayWinProp::~ArrayWinProp()
{
	vider();
}

ArrayWinProp&
ArrayWinProp::operator=(const ArrayWinProp& src)
{
	if (this == &src)
		return *this ;

try
{
	if (false == empty())
		vider() ;

	if (false == src.empty())
    for (ArrayWPConstIter It = src.begin(); src.end() != It ; It++)
    	push_back(new NSWindowProperty(*(*It))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayWinProp =operator.", standardError, 0) ;
	return *this ;
}
}

/*void
ArrayWinProp::initialiser(string sUserId, string sPersoDirectory)
{
	// une erreur bizzare pousse à définir deux ifstreams
	ifstream    inFile1, inFile2 ;

	string      line ;
	string      sData = "" ;
	string      sNomAttrib ;
	string      sValAttrib ;
	size_t      i = 0 ;	string      sFichier ;

	if (sUserId != "")
	{
        sFichier = sPersoDirectory + string("winpos_") + sUserId + string(".dat") ;
		inFile1.open(sFichier.c_str()) ;
	}
	else
	{
		inFile1.open("perso\\winpos.dat") ;
		if (!inFile1)
			return ;
	}

	if (!inFile1)
	{
		sFichier = sPersoDirectory + string("winpos.dat") ;
		inFile2.open(sFichier.c_str()) ;
		if (!inFile2)
		{
			erreur("Impossible d'ouvrir le fichier winpos.dat.", 0, 0) ;
			return ;
		}

		while (!inFile2.eof())
		{
			getline(inFile2,line) ;
			if (line != "")
				sData += line + "\n" ;
		}

		inFile2.close() ;
	}
	else
	{
		while (!inFile1.eof())
		{
			getline(inFile1,line) ;
			if (line != "")
				sData += line + "\n" ;
		}

		inFile1.close() ;
	}

	// boucle de chargement des attributs
	i = 0 ;
	while (i < strlen(sData.c_str()))
	{
		sNomAttrib = "" ;
		sValAttrib = "";

		while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t'))
			sNomAttrib += sData[i++] ;
		while ((strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))
			i++ ;

		while ((strlen(sData.c_str())) && (sData[i] != '\n'))
			sValAttrib += sData[i++] ;

		i++ ;

        if ((sNomAttrib != "") && (sValAttrib != ""))
        {
            NSWindowProperty* pWinProp = new NSWindowProperty() ;
            pWinProp->sFunction = sNomAttrib ;

            int     j = 0, iVal ;
            size_t  pos = sValAttrib.find(',') ;
            while (pos != string::npos)
            {
                string sData = string(sValAttrib, 0, pos) ;
                sValAttrib = string(sValAttrib, pos+1, strlen(sValAttrib.c_str())-pos-1) ;
                pos = sValAttrib.find(',') ;
                iVal = atoi(sData.c_str()) ;
                j++ ;
                switch (j)
                {
                    case 1 : pWinProp->iX = iVal ; break ;
                    case 2 : pWinProp->iY = iVal ; break ;
                    case 3 : pWinProp->iW = iVal ; break ;
                    case 4 : pWinProp->iH = iVal ; break ;
                }
            }
            iVal = atoi(sValAttrib.c_str()) ;
            j++ ;
            switch (j)
            {
                case 1 : pWinProp->iX = iVal ; break ;
                case 2 : pWinProp->iY = iVal ; break ;
                case 3 : pWinProp->iW = iVal ; break ;
                case 4 : pWinProp->iH = iVal ; break ;
            }

            push_back(pWinProp) ;
        }
	}
	return ;
}   */

NS_CLASSLIB::TRect
ArrayWinProp::getPropertyRect(string sFunction, NSSuper* pSuper)
{
	NSWindowProperty* pProp = getProperty(sFunction) ;
  if ((NSWindowProperty*) NULL == pProp)
  	return NS_CLASSLIB::TRect(0, 0, 100, 100) ;

	return pProp->getRect(pSuper) ;
}

void
ArrayWinProp::setProperty(string sUserId, string sFunction, NS_CLASSLIB::TRect rect, string sPersoDirectory)
{
	ArrayWPIter iter ;
  if (false == empty())
  	for (iter = begin(); (end() != iter) && ((*iter)->getFunction() != sFunction); iter++) ;

  // Pas trouvé : on ajoute
  // Not found : we add it
  if ((empty()) || (end() == iter))
	{
		NSWindowProperty* pWinProp = new NSWindowProperty() ;
    pWinProp->setFunction(sFunction) ;
    pWinProp->initFromRect(rect) ;
    push_back(pWinProp) ;
	}
  else
  	(*iter)->initFromRect(rect) ;

  // Mise à jour du fichier
  string sFichier = sPersoDirectory + string("winpos_") + sUserId + string(".dat") ;

  ofstream outFile ;
  outFile.open(sFichier.c_str(), ios::out) ;
  if (!outFile)
  {
    string sErreur = "Erreur d'ouverture du fichier " + sFichier ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return ;
  }

  string sLine;  for (iter = begin(); iter != end(); iter++)  {    sLine = (*iter)->getFunction() + string(" ") + (*iter)->getPosAsString() ;    sLine += string("\n") ;    outFile << sLine ;  }  outFile.close() ;
}

void
ArrayWinProp::saveWindowProperty(string sUserId, string sPersoDirectory, NSWindowProperty *wndProp, bool bWriteFile)
{
	ArrayWPIter iter ;
	if (false == bWriteFile)
	{
		if (NULL == wndProp)
			return ;

		string sFunction = wndProp->getFunction() ;
    if (string("") == sFunction)
			return ;

		if (false == empty())
			for (iter = begin(); (end() != iter) && ((*iter)->getFunction() != sFunction); iter++) ;

    // Pas trouvé : on ajoute
    // Not found : we add it
		if ((empty()) || (iter == end()))
		{
			push_back(new NSWindowProperty(*wndProp)) ;
		}
		else
		{
    	if (-1 != wndProp->getX())
				(*iter)->setX(wndProp->getX()) ;
      if (-1 != wndProp->getY())
				(*iter)->setY(wndProp->getY()) ;
      if (-1 != wndProp->getW())
				(*iter)->setW(wndProp->getW()) ;
      if (-1 != wndProp->getH())
				(*iter)->setH(wndProp->getH()) ;

      (*iter)->setPosX(wndProp->posX()) ;
      (*iter)->setPosY(wndProp->posY()) ;
      (*iter)->setPosW(wndProp->posW()) ;
      (*iter)->setPosH(wndProp->posH()) ;

      if (NSWindowProperty::undefined != wndProp->getActivity())
      {
      	(*iter)->setActivity(wndProp->getActivity()) ;
      	(*iter)->setHotKey(wndProp->getHotKey()) ;
        (*iter)->setModifiers(wndProp->getModifiers()) ;
      }
			(*iter)->getSplit()->vider() ;
		}
	}
	else
	{
		// Mise à jour du fichier
		string sFichier = sPersoDirectory + string("winpos_") + sUserId + string(".dat") ;

		ofstream outFile ;
		outFile.open(sFichier.c_str(), ios::out) ;
		if (!outFile)
		{
			string sErreur = "Erreur d'ouverture du fichier " + sFichier ;
			erreur(sErreur.c_str(), standardError, 0) ;
			return ;
		}

    string ps = string("Saving windows properties to file ") + sFichier ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

		string sLine ;		for (iter = begin(); end() != iter ; iter++)		{			sLine = (*iter)->getFunction() + string(" ") + (*iter)->getPosAsString() ;			if (false == (*iter)->getSplit()->empty())			{				ArraySWIter iterSplit ;				for (iterSplit = (*iter)->getSplit()->begin() ; (*iter)->getSplit()->end() != iterSplit ; iterSplit++)				{					string sDir = string("") ;					switch ((*iterSplit)->getSplitDirection())					{						case psHorizontal :							sDir = "H" ;
							break ;
						case psVertical   :
							sDir = "V" ;							break ;					}					int dec, sign, ndig = 3 ;					char *str ;					str = fcvt((*iterSplit)->getPercent(), ndig, &dec, &sign) ;

					// ltoa((*iterSplit)->getPercent(), cValue,10);					sLine += string(" [") + (*iterSplit)->getMotherWindow() + string(",") +                        (*iterSplit)->getChildWindow() + string(",") +  sDir +                        string(",") +  string(str) + string ("]");				}			}      sLine += string(" (") ;      switch ((*iter)->getActivity())      {      	case NSWindowProperty::blackboardedWithPatient :        	sLine += string("B") ;          break ;        case NSWindowProperty::openedWithPatient :        	sLine += string("A") ;          break ;        case NSWindowProperty::available :        	sLine += string("1") ;          break ;        case NSWindowProperty::notAvailable :        	sLine += string("0") ;          break ;        //        // Default is considered as Available        //        default :        	sLine += string("1") ;      }      sLine += string(",") ;      if ((*iter)->getHotKey() != '\0')      {      	sLine += string(1, (*iter)->getHotKey()) ;        sLine += string(",") ;        sLine += (*iter)->getModifiers() ;      }      sLine += string(")") ;			sLine += string("\n") ;			outFile << sLine ;		}		outFile.close() ;
	}
}

//----------------------------------------------------------------
void
ArrayWinProp::initWindsChild(string sUserId, string sPersoDirectory)
{
	string sFileName ;

	if (sUserId != "")
	{
  	sFileName = sPersoDirectory + string("winpos_") + sUserId + string(".dat") ;

    string ps = string("Initializing windows properties from file ") + sFileName ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

    if (initFromFile(sFileName))
    	return ;
  }

  sFileName = sPersoDirectory + string("winpos.dat") ;

  string ps = string("Initializing windows properties from file ") + sFileName ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  if (initFromFile(sFileName))
    	return ;

	string sErreur = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" winpos.dat") ;
  erreur(sErreur.c_str(), standardError, 0) ;
}

bool
ArrayWinProp::initFromFile(string sFileName)
{
	if (string("") == sFileName)
		return false ;

	string sData = string("") ;

  if (false == getStringFromFile(sFileName, &sData))
		return false ;

  if (string("") == sData)
		return false ;

	return initFromString(sFileName, &sData) ;
}

bool
ArrayWinProp::getStringFromFile(string sFileName, string* pData)
{
	if (((string*) NULL == pData) || (string("") == sFileName))
		return false ;

	*pData = string("") ;

try
{
	ifstream inFile ;
  inFile.open(sFileName.c_str()) ;
  if (!inFile)
  	return false ;

  while (!inFile.eof())
  {
    string sLine("") ;
  	getline(inFile, sLine) ;
    if (string("") != sLine)
    	*pData += sLine + string("\n") ;
  }

  inFile.close() ;

  return true ;
}
catch (...)
{
	string sErreur = string("Exception ArrayWinProp::initFromFile : ") + pContexte->getSuperviseur()->getText("fileErrors", "errorReadingFile") + string(" ") + sFileName ;
	erreur(sErreur.c_str(), standardError, 0) ;
  return false ;
}
}

bool
ArrayWinProp::initFromString(string sFileName, string* pData)
{
	if (NULL == pData)
		return false ;

	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "corruptedFile") + string(" ") + sFileName ;

	string sFichier ;

	// boucle de chargement des attributs
	size_t i = 0 ;
  size_t iStrLen = strlen(pData->c_str()) ;
	while (i < iStrLen)
	{
		string sNomAttrib  = string("") ;
		string sValAttrib  = string("") ;
    string sValSplit   = string("") ;
    string sValDetails = string("") ;

    string sBloc1 = string("") ;
    string sBloc2 = string("") ;

    bool bContinue = gotoNextBlock(pData, &i) ;
    if (bContinue)
    	bContinue = readBlockInString(pData, &i, &sNomAttrib) ;

    if (bContinue)
    	bContinue = gotoNextBlock(pData, &i) ;
    if (bContinue)
    	bContinue = readBlockInString(pData, &i, &sValAttrib) ;

    if ((string("") == sNomAttrib) || (string("") == sValAttrib))
    {
    	erreur(sErrorText.c_str(), standardError, 0) ;
  		return false ;
    }

    if (bContinue)
    	bContinue = gotoNextBlock(pData, &i) ;
    if (bContinue)
    	bContinue = readBlockInString(pData, &i, &sBloc1) ;

    if (bContinue)
    	bContinue = gotoNextBlock(pData, &i) ;
    if (bContinue)
    	bContinue = readBlockInString(pData, &i, &sBloc2) ;

    if (string("") != sBloc1)
    {
    	if      ('[' == sBloc1[0])
    		sValSplit = sBloc1 ;
      else if ('(' == sBloc1[0])
    		sValDetails = sBloc1 ;
    }

    if (string("") != sBloc2)
    {
    	if      ('[' == sBloc2[0])
    		sValSplit = sBloc2 ;
      else if ('(' == sBloc2[0])
    		sValDetails = sBloc2 ;
    }

		if ((i < iStrLen) && ('\n' == (*pData)[i]))
			i++ ;
    NSWindowProperty* pWinProp = new NSWindowProperty() ;
    pWinProp->setFunction(sNomAttrib) ;
    if (!initSizeFromString(&sValAttrib, pWinProp))
    {
    	delete pWinProp ;
      return false ;
    }
    if ((string("") != sValSplit) && (!initSplitFromString(&sValSplit, pWinProp)))
    {
    	erreur(sErrorText.c_str(), standardError, 0) ;
      return false ;
    }
    if ((string("") != sValDetails) && (!initDetailsFromString(&sValDetails, pWinProp)))
    {
    	erreur(sErrorText.c_str(), standardError, 0) ;
      return false ;
    }

    push_back(pWinProp) ;
	}

	return true ;
}

// return : true = can continue ; false = must stop
//
bool
ArrayWinProp::readBlockInString(string* psSource, size_t* piCursor, string* psResult)
{
	if ((string*) NULL == psResult)
		return false ;
	*psResult = string("") ;

  if (((string*) NULL == psSource) || ((size_t*) NULL == piCursor))
		return false ;

  size_t iSourceLen = strlen(psSource->c_str()) ;
  if (*piCursor >= iSourceLen)
  	return false ;

	while ((*piCursor < iSourceLen) &&
         ((*psSource)[*piCursor] != ' ') &&
         ((*psSource)[*piCursor] != '\t') &&
         ((*psSource)[*piCursor] != '\n'))
  	*psResult += (*psSource)[(*piCursor)++] ;

	if ((*piCursor >= iSourceLen) || ((*psSource)[*piCursor] == '\n'))
  	return false ;
  return true ;
}

// return : true = can continue ; false = must stop
//
bool
ArrayWinProp::gotoNextBlock(string* psSource, size_t* piCursor)
{
	if (((string*) NULL == psSource) || ((size_t*) NULL == piCursor))
		return false ;

  size_t iSourceLen = strlen(psSource->c_str()) ;
  if (*piCursor >= iSourceLen)
  	return false ;

  while ((*piCursor < iSourceLen) &&
         ((' '  == (*psSource)[*piCursor]) ||
          ('\t' == (*psSource)[*piCursor])))
  	(*piCursor)++ ;

	if ((*piCursor >= iSourceLen) || ((*psSource)[*piCursor] == '\n'))
  	return false ;
  return true ;
}

// return : true = good string ; false = string with wrong format
//
// Format : 148,268,437,282
//          no space allowed
//
bool
ArrayWinProp::initSizeFromString(string* psSource, NSWindowProperty* pWinProp)
{
	if ((NULL == psSource) || (NULL == pWinProp))
		return false ;

  if (*psSource == string(""))
  	return false ;
  string sValAttrib = *psSource ;

  char cSeparator = ',' ;

  int     j = 0, iVal ;
  size_t  pos = sValAttrib.find(cSeparator) ;
  while (pos != string::npos)
  {
  	string sData = string(sValAttrib, 0, pos) ;

    bool bOnlyDigits = true ;
	  for (size_t i = 0 ; i < pos ; i++)
		  if (!(isdigit(sData[i])))
			  bOnlyDigits = false ;

    j++ ;

    if (bOnlyDigits)
    {
      int iVal = atoi(sData.c_str()) ;
      switch (j)
      {
        case 1  : pWinProp->setX(iVal) ; break ;
        case 2  : pWinProp->setY(iVal) ; break ;
        case 3  : pWinProp->setW(iVal) ; break ;
        case 4  : pWinProp->setH(iVal) ; break ;
        default : return false ;
      }
    }
    else
    {
      NSWindowProperty::DYNAMICPOS dynPos = NSWindowProperty::StringToDynPos(sData) ;

      switch (j)
      {
        case 1  : pWinProp->setPosX(dynPos) ; break ;
        case 2  : pWinProp->setPosY(dynPos) ; break ;
        case 3  : pWinProp->setPosW(dynPos) ; break ;
        case 4  : pWinProp->setPosH(dynPos) ; break ;
        default : return false ;
      }
    }

    sValAttrib = string(sValAttrib, pos+1, strlen(sValAttrib.c_str())-pos-1) ;
    pos = sValAttrib.find(cSeparator) ;
  }

  if (3 != j)
  	return false ;

  iVal = atoi(sValAttrib.c_str()) ;
  pWinProp->setH(iVal) ;

  return true ;
}

// return : true = good string ; false = string with wrong format
//
// Format : [EpiRCManagement,RcItemHistory,H,462][EpiRCManagement,ProcessManagement,H,462]
//          no space allowed
//
bool
ArrayWinProp::initSplitFromString(string* psSource, NSWindowProperty* pWinProp)
{
	if ((NULL == psSource) || (NULL == pWinProp))
		return false ;

  if (string("") == *psSource)
  	return true ;

  char cStart     = '[' ;
  char cSeparator = ',' ;
  char cEnd       = ']' ;

	string sWndMother, sWndChild, sPercent, sSplit ;
  size_t count = 0 ;
  size_t iSize = strlen(psSource->c_str()) ;
  while (count < iSize)
  {
  	sWndMother = "" ;
    sWndChild  = "" ;
    sSplit     = "" ;
    sPercent   = "" ;

    if ((*psSource)[count] != cStart)
    	return false ;
    count++ ;

    while ((count < iSize) && ((*psSource)[count] != cSeparator))
    	sWndMother += (*psSource)[count++] ;

    if (count >= iSize)
    	return false ;
    count++ ;

    while ((count < iSize) && ((*psSource)[count] != cSeparator))
    	sWndChild += (*psSource)[count++] ;

    if (count >= iSize)
    	return false ;
    count++ ;

    while ((count < iSize) && ((*psSource)[count] != cSeparator))
    	sSplit += (*psSource)[count++] ;

    if (count >= iSize)
    	return false ;
    count++ ;

    while ((count < iSize) && ((*psSource)[count] != cEnd))
    	sPercent += (*psSource)[count++] ;

    if (count >= iSize)
    	return false ;
    count++ ;

    int iVal = atoi(sPercent.c_str()) ;

    switch (sSplit[0])
    {
    	case 'H' :
      	pWinProp->addSplit(sWndMother, sWndChild, psHorizontal, (float(iVal))/(float(1000))) ;
        break ;
      case 'V' :
      	pWinProp->addSplit(sWndMother,sWndChild, psVertical, (float(iVal))/(float(1000))) ;
        break ;
      default :
      	return false ;
    }
  }
  return true ;
}

// return : true = good string ; false = string with wrong format
//
// Format : (1,B) or (1,)
//          no space allowed
//
bool
ArrayWinProp::initDetailsFromString(string* psSource, NSWindowProperty* pWinProp)
{
	if ((NULL == psSource) || (NULL == pWinProp))
		return false ;

  if (string("") == *psSource)
  	return true ;

  char cStart     = '(' ;
  char cSeparator = ',' ;
  char cEnd       = ')' ;

  if ((*psSource)[0] != cStart)
  	return false ;
  size_t count = 1 ;

	string sAutoOpen = string("") ;
  string sHotKey   = string("") ;
  string sModifier = string("") ;

  size_t iSize = strlen(psSource->c_str()) ;

  while ((count < iSize) && ((*psSource)[count] != cSeparator))
  	sAutoOpen += (*psSource)[count++] ;

  if (count >= iSize)
  	return false ;
  count++ ;

  while ((count < iSize) && ((*psSource)[count] != cSeparator) && ((*psSource)[count] != cEnd))
  	sHotKey += (*psSource)[count++] ;

  if (count >= iSize)
  	return false ;
	if (count < iSize - 1)
		count++ ;

  while ((count < iSize) && ((*psSource)[count] != cSeparator) && ((*psSource)[count] != cEnd))
  	sModifier += (*psSource)[count++] ;

	if (count >= iSize)
  	return false ;

  if      (sAutoOpen == string(1, 'B'))
  	pWinProp->setActivity(NSWindowProperty::blackboardedWithPatient) ;
  else if (sAutoOpen == string(1, 'A'))
  	pWinProp->setActivity(NSWindowProperty::openedWithPatient) ;
  else if (sAutoOpen == string(1, '1'))
  	pWinProp->setActivity(NSWindowProperty::available) ;
  else if (sAutoOpen == string(1, '0'))
  	pWinProp->setActivity(NSWindowProperty::notAvailable) ;
  else
  	return false ;

  if (string("") != sHotKey)
	{
   	if (1 == strlen(sHotKey.c_str()))
  		pWinProp->setHotKey(sHotKey[0]) ;
    else
    {
    	size_t i = 0 ;
    	for ( ; (i < strlen(sHotKey.c_str())) && (isdigit(sHotKey[i])) ; i++) ;
    	if (i == strlen(sHotKey.c_str()))
      {
      	int iValue = atoi(sHotKey.c_str()) ;
        pWinProp->setHotKey(char(iValue)) ;
      }
    }
	}

	if (string("") != sModifier)
  	pWinProp->setModifiers(sModifier) ;

  return true ;
}

//trouve les proprietes d'une fenetre qui a créé un MDIChild
NSWindowProperty*
ArrayWinProp::getProperty(string sFunction)
{
	if (empty())
		return NULL ;

	ArrayWPIter iter ;
	for (iter = begin(); (end() != iter) && ((*iter)->getFunction() != sFunction); iter++) ;

	if (end() == iter)
		return NULL ;
	else
  	return *iter ;
}

//--------------------------------------------------------

/*//trouve les proprietes d'une fenetre a partir de son nom
NSWindowProperty*
ArrayWinProp::searchProperty(string sFunction)
{
    ArrayWPIter iter ;
    for (iter = begin(); (iter != end()) && ((*iter)->sFunction != sFunction); iter++) ;
    if (iter != end())
        return *iter ;

    NSWindowProperty *pWinProp = new NSWindowProperty();
    for (iter = begin(); ((iter != end())&& !((*iter)->paramSplit->empty())); iter++)
    {

        ArraySWIter iterSplit ;        for (iterSplit = (*iter)->paramSplit->begin(); iterSplit != (*iter)->paramSplit->end(); iterSplit++)
        {
            if ((*iterSplit)->getChildWindow() == sFunction)
            {
                pWinProp->addSplit((*iterSplit)->getMotherWindow(),sFunction,(*iterSplit)->getSplitDirection(),(*iterSplit)->getSplitDirection());
                return pWinProp;
            }
            if ((*iterSplit)->getMotherWindow() == sFunction)
            {
                pWinProp->addSplit(sFunction,(*iterSplit)->getChildWindow() ,(*iterSplit)->getSplitDirection(),(*iterSplit)->getSplitDirection());
                return pWinProp;
             }

        }
    }

}  */


//recherche des proprieté pour chaque paneSplitter

NSSplittingWindowProperty*
ArrayWinProp::searchProperty(string sMother, string sChild)
{
	if (empty())
		return (NSSplittingWindowProperty*) 0 ;

	ArrayWPIter iter ;
  for (iter = begin() ; end() != iter ; iter++)
  {
  	if (false == (*iter)->getSplit()->empty())
    {    	ArraySWIter iterSplit ;      for (iterSplit = (*iter)->getSplit()->begin() ; (*iter)->getSplit()->end() != iterSplit ; iterSplit++)      	if (((*iterSplit)->getChildWindow() == sChild) && ((*iterSplit)->getMotherWindow() == sMother))
        {
        	NSSplittingWindowProperty *pSplitProp = new NSSplittingWindowProperty(**iterSplit) ;
          return pSplitProp ;
        }
    }
  }
  return (NSSplittingWindowProperty*) 0 ;
}

//---------------------------------------------------



//-----------------------------------------------------------------------------
//              classe NSSplittingWindowProperty
//-----------------------------------------------------------------------------
NSSplittingWindowProperty::NSSplittingWindowProperty()
{
	sMotherWind  = "" ;
  sChildWind   = "" ;  percent      = 1 ;  dirSplitting = psNone ;}

NSSplittingWindowProperty::NSSplittingWindowProperty(const NSSplittingWindowProperty& rv)
{
	sMotherWind  = rv.sMotherWind  ;
  sChildWind   = rv.sChildWind ;  percent      = rv.percent ;  dirSplitting = rv.dirSplitting ;
}

// Destructor
NSSplittingWindowProperty::~NSSplittingWindowProperty(){}

NSSplittingWindowProperty&
NSSplittingWindowProperty::operator=(const NSSplittingWindowProperty& src)
{
  if (&src == this)
		return *this ;

	sMotherWind  = src.sMotherWind  ;
  sChildWind   = src.sChildWind ;  percent      = src.percent ;  dirSplitting = src.dirSplitting ;

  return *this ;
}

//------------------------------------------------------------------------------
ArraySplitWindow::ArraySplitWindow(const ArraySplitWindow& rv)
{
try
{
	if (false == rv.empty())
		for (ArraySWConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSSplittingWindowProperty(*(*i))) ;
}
catch (...)
{
	erreur("Exception ArraySplitWindow copy ctor.", standardError, 0) ;
}
}

void
ArraySplitWindow::vider()
{
	if (empty())
		return ;

	for (ArraySWIter i = begin() ; end() != i ; )
	{
  	if (*i)
    	delete *i ;
		erase(i) ;
	}
}

ArraySplitWindow::~ArraySplitWindow()
{
	vider();
}

ArraySplitWindow&
ArraySplitWindow::operator=(const ArraySplitWindow& src)
{
  if (&src == this)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (ArraySWConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSSplittingWindowProperty(*(*i))) ;

	return *this ;
}

//------------------------------------------------------------------------------

void
ArrayMUEView::vider()
{
	if (empty())
		return ;

	for (ArrayMUEViewIter it = begin() ; end() != it ; )
		erase(it) ;
}

ArrayMUEView::~ArrayMUEView()
{
	vider() ;
}

