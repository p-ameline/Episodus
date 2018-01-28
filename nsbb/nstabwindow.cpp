// -----------------------------------------------------------------------------
// source :
// OWL Extensions (OWLEXT) Class Library - Copyright(c) 1996 by Manic Software.
// -----------------------------------------------------------------------------
// $Revision: 1.10 $
// $Author: pameline $
// $Date: 2015/02/07 12:53:30 $
// -----------------------------------------------------------------------------
// FLP - juin 2003
// -----------------------------------------------------------------------------
// original comment :
// TTabWindow (& friends) implementation
// TODO: * Change tabbedWindowArray to be a collection of TTabWindowDatum,
//         in order to support adding tabs w/ controls for client areas,
//         since controls don't have a "title" per se
// -----------------------------------------------------------------------------

#include <owl\pch.h>
#pragma hdrstop

#include <owl\window.h>

#include "nsbb\nstabwindow.h"
#include "partage\nsdivfct.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbb.rh"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsarc.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsedilex.h"
#include "nsbb\nsedit.h"
#include "nsbb\nsbouton.h"
#include "nsbb\nschkbox.h"
#include "nsbb\nsradiob.h"
#include "nsbb\nsgroupe.h"
#include "nsbb\nsonglet.h"
#include "nsbb\nscombo.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsmview.h"
#include "nsbb\ns_multi.h"

#ifdef _MUE
#include "nsbb\nslistwind.h"
#endif

#define ID_TABCTRL 100

const char  codeMARK    = 'O';

// -----------------------------------------------------------------------------
// NSControlsArray
// -----------------------------------------------------------------------------
// classe contenant une liste de TWindow (les TWindows des controles de chaque
// onglet). Chaque NSOneTabWindow contient un array de controles
// -----------------------------------------------------------------------------

NSControlsArray::NSControlsArray()
                :NSControlsVector()
{
}

NSControlsArray::NSControlsArray(const NSControlsArray& src)
                :NSControlsVector()
{
try
{
  if (false == src.empty())
    for (NSControlsConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(*i) ;
}
catch (...)
{
  erreur("Exception NSControlsArray copy ctor.", standardError, 0);
}
}


NSControlsArray::~NSControlsArray()
{
  vider() ;
}


NSControlsArray
NSControlsArray::operator=(const NSControlsArray& src)
{
try
{
	if (this == &src)
		return (*this) ;

  vider() ;

  if (false == src.empty())
    for (NSControlsConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(*i) ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception NSControlsArray copy ctor.", standardError, 0) ;
  return (*this) ;
}
}

void
NSControlsArray::vider()
{
  if (false == empty())
    for (NSControlsIter i = begin() ; end() != i ; )
      erase(i) ;
}

// -----------------------------------------------------------------------------
// NSOneTabWindow
// -----------------------------------------------------------------------------
// classe surchargeant TWindow (les TWindows gérés par la fenêtre à
// onglets). Chaque TWindow correspond à un onglet
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSOneTabWindow, TWindow)
  EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

NSOneTabWindow::NSOneTabWindow(TWindow* parent, const char far* title, TModule* module)
               :TWindow(parent, title, module)
{
  controls = new NSControlsArray() ;

  SetStyle(WS_CHILD | WS_VISIBLE | GetStyle()) ;

  if (TSystem::Has3dUI())
  {
    uint32 exStyle = GetExStyle() ;
    SetExStyle(exStyle |= WS_EX_CLIENTEDGE) ;
  }
}

NSOneTabWindow::~NSOneTabWindow()
{
	delete controls ;
}

void
NSOneTabWindow::SetupWindow()
{
	TWindow::SetupWindow() ;
}

void
NSOneTabWindow::EvSetFocus(THandle /* hWndLostFocus */ /* may be 0 */)
{
}

void
NSOneTabWindow::Paint(TDC& dc, bool /* erase */, NS_CLASSLIB::TRect& RectAPeindre)
{
	int iAncDC = dc.SaveDC() ;

	dc.FillRect(RectAPeindre, OWL::TBrush(NS_CLASSLIB::TColor::Sys3dFace)) ;

	dc.RestoreDC(iAncDC) ;
}


char far*
NSOneTabWindow::GetClassName()
{
/***********************************************
    TResId dlgClass;

    #if defined(BI_COMP_BORLANDC)    if (GetApplication()->BWCCEnabled()) {
        GetApplication()->GetBWCCModule()->Register(*GetModule());
        dlgClass = BORDLGCLASS;
    }
    else
    #endif
        dlgClass = WC_DIALOG;

    if (!TUser::GetClassInfo(0, dlgClass, &wndClass))        GetModule()->GetClassInfo(dlgClass, &wndClass);

    wndClass.lpszClassName = GetClassName();    wndClass.hInstance = *GetModule();   //!CQ Win32 only? (B50.26561)
**************************************************/

#if defined(BI_COMP_BORLANDC)
  if (GetApplication()->BWCCEnabled())
    return SHADE_CLASS ;
  else
#endif
    return "BUTTON" ;

  // return SHADE_CLASS ;
}

void
NSOneTabWindow::GetWindowClass(WNDCLASS& wndClass)
{
    /**************************
    TResId dlgClass;

    #if defined(BI_COMP_BORLANDC)
    if (GetApplication()->BWCCEnabled()) {        GetApplication()->GetBWCCModule()->Register(*GetModule());
        dlgClass = BORDLGCLASS;
    }
    else

    #endif        dlgClass = WC_DIALOG;

    if (!TUser::GetClassInfo(0, dlgClass, &wndClass))        GetModule()->GetClassInfo(dlgClass, &wndClass);
    ************************************/

  wndClass.lpszClassName = GetClassName() ;  wndClass.hInstance = *GetModule() ;   //!CQ Win32 only? (B50.26561)
}

extern void     _OWLFUNC SetCreationWindow(TWindow*);
extern TWindow* _OWLFUNC GetCreationWindow();

bool
NSOneTabWindow::Create()
{
  if (GetHandle())
    return true;

  int  menuOrId = 0;

  DisableAutoCreate();

  // If this is the main window, make sure it is treated as one by the shell
  //
  if (IsFlagSet(wfMainWindow))
    ModifyExStyle(0, WS_EX_APPWINDOW);

  if (IsFlagSet(wfFromResource))
    SetHandle(Parent ? Parent->GetDlgItem(Attr.Id) : 0);  // Windows already created it

  else if (Register()) {
    ::SetCreationWindow(this);

    LoadAcceleratorTable();

    if (Attr.Menu) {
      menuOrId = (int)GetModule()->LoadMenu(Attr.Menu);
      WARNX(OwlWin, !menuOrId, 0, "Unable to load menu: " << Attr.Menu <<
            " from " << *GetModule());
    }
    else
      menuOrId = Attr.Id;

    PerformCreate(menuOrId);
    GetApplication()->ResumeThrow();
  }
  else
    TXWindow::Raise(this, IDS_CLASSREGISTERFAIL);

  if (!GetHandle()) {
    if (Attr.Menu)  // && !IsFlagSet(wfFromResource) ?
      DestroyMenu(HMENU(menuOrId));

    TXWindow::Raise(this, IDS_WINDOWCREATEFAIL);
  }

  // Here we have a non-thunked handle. This may be caused by two scenarios:
  // 1. Predefined-class (non-owl) windows.
  // 2. OWL controls which were created from resource [Although these
  //    are registered with InitWndProc, they did not get thunked since
  //    'CreationWindow' was not set when they received their first messages].
  //
  if (!GetWindowPtr(GetHandle())) {

    // Grab the state info.
    //
    GetWindowTextTitle();
    GetHWndState();

    // If it's a 'predefinedClass' window, subclass it
    //
    if (GetWindowProc() != InitWndProc) {
      SubclassWindowFunction();

      // Reset the 'CreationWindow' pointer [if necessary]
      //
      if (::GetCreationWindow() == this)
        ::SetCreationWindow(0);

      // Set status flag [since we missed EvCreate]
      //
      SetFlag(wfPredefinedClass|wfFullyCreated);
    }
    else {
      // This window's WNDPROC is 'InitWndProc' - However, it has not
      // been thunked since 'CreationWindow' was not set when it received
      // its first messages [it was probably created from a resource but
      // registered by OWL]. We'll set 'CreationWindow' and send a dummy
      // message to allow thunking to take place
      //
      if (!::GetCreationWindow())
        ::SetCreationWindow(this);
      SendMessage(WM_USER+0x4000, 0, 0);

      // Update flags
      //
      SetFlag(wfFullyCreated);
    }

    // Invoke SetupWindow since 'EvCreate' was missed..
    //
    SetupWindow();
  }
  return true;
}

// -----------------------------------------------------------------------------
// NSWindowsArray
// -----------------------------------------------------------------------------
// classe contenant une liste de TWindow (les TWindows gérés par la fenêtre à
// onglets). Chaque TWindow correspond à un onglet
// -----------------------------------------------------------------------------

NSWindowsArray::NSWindowsArray()
               :NSWindowsVector()
{
}


NSWindowsArray::NSWindowsArray(const NSWindowsArray& src)
               :NSWindowsVector()
{
try
{
  if (false == src.empty())
    for (NSWindowsConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(*i) ;
}
catch (...)
{
  erreur("Exception NSWindowsArray copy ctor.", standardError, 0);
}
}

NSWindowsArray::~NSWindowsArray()
{
  vider() ;
}

NSWindowsArray
NSWindowsArray::operator=(const NSWindowsArray& src)
{
try
{
  if (this == &src)
		return (*this) ;

  vider() ;

  if (false == src.empty())
    for (NSWindowsConstIter it = src.begin() ; src.end() != it ; it++)
      push_back(*it) ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception NSWindowsArray copy ctor.", standardError, 0) ;
  return (*this) ;
}
}

void
NSWindowsArray::vider()
{
  if (false == empty())
    for (NSWindowsIter i = begin() ; end() != i ; )
    {
      delete (*i) ;
      erase(i) ;
    }
}

// -----------------------------------------------------------------------------
// NSTabWindow
// -----------------------------------------------------------------------------
// dérivé de TDialog. Boite de dialogue à onglets.
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSTabWindow, NSDialog)
	// EV_WM_SIZE,
	EV_WM_SETFOCUS,
	EV_COMMAND(IDOK,			                CmOk),
	EV_COMMAND(IDCANCEL,	                    CmCancel),
	EV_COMMAND(IDHELP,		                    CmHelp),
	// EV_TCN_SELCHANGE(/*NSTabWindow::*/ID_TABCTRL,	TabSelChange),
	// EV_TCN_SELCHANGING(/*NSTabWindow::*/ID_TABCTRL, TabSelChanging),
	// EV_TCN_KEYDOWN(/*NSTabWindow::*/ID_TABCTRL,		TabKeyDown),
  EV_TCN_SELCHANGE(ID_TABCTRL,	 TabSelChange),
	EV_TCN_SELCHANGING(ID_TABCTRL, TabSelChanging),
	EV_TCN_KEYDOWN(ID_TABCTRL,		 TabKeyDown),
	EV_WM_ACTIVATE,
	EV_WM_CHILDACTIVATE,
    /*EV_WM_MOVE,
   EV_WM_WINDOWPOSCHANGED,
   EV_WM_WINDOWPOSCHANGING,   */
END_RESPONSE_TABLE;

/*****************************************
NSTabWindow::NSTabWindow(TWindow *parent, string title, NSWindowsArray *array)
	:	TDialog(parent, "DLG_BASE", pNSDLLModule),
		m_tabCtrl(0),
    sTitle(title)
{
  uint32 styleEx = GetExStyle() ;
  SetExStyle(styleEx |= WS_EX_DLGMODALFRAME) ;

  uint32 style = GetStyle() ;
  SetStyle(style |= WS_CAPTION | WS_POPUP | WS_SYSMENU) ;

  tabbedWindows = new NSWindowsArray() ;
	// add array to tabbedWindowArray
	if (array && !(array->empty()))
		Attach(array) ;

}


NSTabWindow::NSTabWindow(TWindow *parent, string title, uint32 X, uint32 Y, uint32 W, uint32 H, NSWindowsArray *array)
	:	TDialog(parent, "DLG_BASE", pNSDLLModule),
	  m_tabCtrl(0),
    sTitle(title)
{
  uint32 styleEx = GetExStyle() ;
  SetExStyle(styleEx |= WS_EX_DLGMODALFRAME) ;

  uint32 style = GetStyle() ;
  SetStyle(style |= WS_CAPTION | WS_POPUP | WS_SYSMENU) ;

	// add array to tabbedWindowArray
  tabbedWindows = new NSWindowsArray() ;

	if (array && !(array->empty()))
		Attach(array) ;
}
*********************************************************/

#ifdef __OB1__
NSTabWindow::NSTabWindow(NSContexte* pCtx, TWindow* AParent, BBItem* pItem, TModule* pNSResModule,
                            NsMultiDialog* pControl)
            : NSDialog(pCtx, AParent, pItem, pNSResModule, pControl),
#else
NSTabWindow::NSTabWindow(NSContexte* pCtx, TWindow* AParent, BBItem* pItem, TModule* pNSResModule,
                            NsMultiDialog* pControl, bool initFromBbk)
            : NSDialog(pCtx, AParent, pItem, pNSResModule, pControl, initFromBbk),
#endif
            m_tabCtrl(0), sTitle("")
{
try
{
	NSSuper* pSuper = 0 ;
	if (pContexte)
	{
		pSuper = pContexte->getSuperviseur() ;
		string ps = string("Entering NSTabWindow Constructor") ;
  	pSuper->trace(&ps, 1, NSSuper::trDetails) ;
	}

	uint32 styleEx = GetExStyle() ;
	SetExStyle(styleEx |= WS_EX_DLGMODALFRAME) ;

  // Suppress caption buttons
	uint32 style = GetStyle();
	style = style^WS_CAPTION ;
  style = style^WS_POPUP ;

	//style &= ~WS_THICKFRAME;
	// SetStyle(style |= WS_CAPTION | WS_POPUP | WS_SYSMENU) ; don't
	// SetStyle(style |= WS_POPUP | WS_SYSMENU) ;
  SetStyle(style |= WS_CHILD | WS_SYSMENU) ;

	// add array to tabbedWindowArray
	tabbedWindows = new NSWindowsArray() ;

	bControlInit = false ;

	// Create tab control, if not done already
  int iWidth  = max(Parent->GetClientRect().Width(), 0) ;
  int iHeight = max(Parent->GetClientRect().Height(), 0) ;
	m_tabCtrl = new TTabControl(this, /*NSTabWindow::*/ID_TABCTRL, 0, 0, iWidth, iHeight) ;

  if ((NSSuper*) NULL != pSuper)
	{
		string ps = string("Leaving NSTabWindow Constructor") ;
		pSuper->trace(&ps, 1, NSSuper::trDetails) ;
	}
}
catch (...)
{
	erreur("Exception NSTabWindow ctor.", standardError, 0) ;
}
}

NSTabWindow::~NSTabWindow()
{
	tabbedWindows->vider() ;
  delete tabbedWindows ;

  // Done by OWL's garbage collector
  // delete m_tabCtrl ;
}

/*bool TTabWindow::Find(TEventInfo& eventInfo, TEqualOperator equal)
{
	eventInfo.Object = (GENERIC*)this;
	return SearchEntries((TGenericTableEntry *)__entries, eventInfo, equal)
		|| TWindow::Find(eventInfo, equal);
}*/
/*void
NSTabWindow::EvMove(ClassLib::TPoint& clientOrigin)
{
     /*ClientToScreen( clientOrigin );
    uint hitTest = ((int)SendMessage(WM_NCHITTEST,0,MAKELONG(clientOrigin.x,clientOrigin.y)));
    static char DisallowCodes[] = {
        HTLEFT,HTRIGHT,HTTOP,HTSIZE};
     MSG Msg;
    Msg.message = HTTOP;
    Msg.wParam = hitTest;
    Msg.lParam = MAKELONG(clientOrigin.x,clientOrigin.y);
   // = strchr(DisallowCodes, hit)) ? HTTOP: hit;
    PostMessage(Msg.message, Msg.wParam, Msg.lParam);
}

void
NSTabWindow::EvWindowPosChanging(WINDOWPOS far& windowPos)
{
   /* ClientToScreen( clientOrigin );
    uint hitTest = ((int)SendMessage(WM_NCHITTEST,0,MAKELONG(clientOrigin.x,clientOrigin.y)));
    static char DisallowCodes[] = {
        HTLEFT,HTRIGHT,HTTOP,HTSIZE};
     MSG Msg;
    Msg.message = HTTOP;
    Msg.wParam = hitTest;
    Msg.lParam = MAKELONG(clientOrigin.x,clientOrigin.y);
   // = strchr(DisallowCodes, hit)) ? HTTOP: hit;
    PostMessage(Msg.message, Msg.wParam, Msg.lParam);
}
voidNSTabWindow::EvWindowPosChanged(WINDOWPOS far& windowPos)
{

}  */


void
NSTabWindow::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TDialog::EvSize(sizeType, size) ;
	if (m_tabCtrl && m_tabCtrl->GetHandle())
	{
		// Adjust tab control to new window size
		m_tabCtrl->SetWindowPos(0, 0, 0, size.cx, size.cy, SWP_NOZORDER | SWP_NOMOVE) ;
	}

    //if (m_tabCtrl->GetCount() > 0)
	//    AdjustTabClient() ;
}

void
NSTabWindow::EvActivate(uint active, bool minimized, HWND hwnd)
{
    NSDialog::EvActivate(active, minimized, hwnd);
}

void
NSTabWindow::EvSetFocus(THandle hWndLostFocus)
{
    NSDialog::EvSetFocus(hWndLostFocus) ;
}

void
NSTabWindow::EvChildActivate()
{
}

int
NSTabWindow::AdjustTabClient()
{
    int height = 0;
    int width = 0;

	// Retrieve pointer to tab control, the client window
	if (m_tabCtrl && m_tabCtrl->GetHandle())
	{
		// Retrieve the window rectangle of the tab control
		ClassLib::TRect rect;
		m_tabCtrl->GetWindowRect(rect);

		// NOTE: GetWindowRect returns screen coordinates... we'll need
		//       to have the coordinates relative to the frame window,
		//       the parent of both the tab and client window
		::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rect, 2);

		// Ask the tab for it's 'client-area' based in it window location
		m_tabCtrl->AdjustRect(false, rect);
        height = rect.top;
        width = rect.left;

		// Move the Client window
		// TWindow *Client = (*tabbedWindows)[m_tabCtrl->GetSel()];
		// if (Client && Client->GetHandle())
		//  	Client->SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);

        int k = 0;
        if (!(tabbedWindows->empty()))
        {
            for (NSWindowsIter i = tabbedWindows->begin(); i != tabbedWindows->end(); i++, k++)
            {
                if ((!bControlInit) && ((*i)->controls) && (!((*i)->controls->empty())))
                {
                    for (NSControlsIter j = (*i)->controls->begin(); j != (*i)->controls->end(); j++)
                    {
                        (*j)->MoveWindow((*j)->Attr.X + width, (*j)->Attr.Y + height, (*j)->Attr.W, (*j)->Attr.H) ;
                    }
                }

                if (k == m_tabCtrl->GetSel())
                {
                    // Move the Client window
		            NSOneTabWindow *Client = *i;
		            if (Client && Client->GetHandle())
			            Client->SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);

                    if (((*i)->controls) && (!((*i)->controls->empty())))
                    {
                        for (NSControlsIter j = (*i)->controls->begin(); j != (*i)->controls->end(); j++)
                        {
                            (*j)->Show(SW_SHOW) ;
                            (*j)->EnableWindow(true) ;
                            (*j)->SetWindowPos(HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW) ;
                        }
                    }
                }
                else
                {
                    if (((*i)->controls) && (!((*i)->controls->empty())))
                        for (NSControlsIter j = (*i)->controls->begin(); j != (*i)->controls->end(); j++)
                        {
                            (*j)->Show(SW_HIDE) ;
                            (*j)->EnableWindow(false) ;
                        }
                }
            }
        }

        if (!bControlInit)
            bControlInit = true;
	}

    return height;
}


void
NSTabWindow::TabSelChange(TNotify far&)
{
	AdjustTabClient();
	// Here, GetSel() returns the after-the-click-current tab selected
	(*tabbedWindows)[m_tabCtrl->GetSel()]->Show(SW_SHOW) ;
    (*tabbedWindows)[m_tabCtrl->GetSel()]->EnableWindow(true) ;
	(*tabbedWindows)[m_tabCtrl->GetSel()]->SetWindowPos(HWND_TOP, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW) ;
}


bool
NSTabWindow::TabSelChanging(TNotify far&)
{
	// Here, GetSel() returns the before-the-click-current tab selected
	if ((*tabbedWindows)[m_tabCtrl->GetSel()])
	{
		(*tabbedWindows)[m_tabCtrl->GetSel()]->Show(SW_HIDE) ;
		(*tabbedWindows)[m_tabCtrl->GetSel()]->EnableWindow(false) ;
	}
	return false ;
}


void
NSTabWindow::TabKeyDown(TTabKeyDown far&)
{
}


void
NSTabWindow::Attach(NSOneTabWindow *newChildWindow, LPCTSTR /* title */)
{
try
{
	// Add to collection of TWindow*'s
	tabbedWindows->push_back(newChildWindow) ;
    /************************
	if (GetHandle())
	{
		// Reparent the given widget
		newChildWindow->SetParent(this) ;

		if (!newChildWindow->GetHandle())
			newChildWindow->Create() ;
	}
    *****************************/

	if (m_tabCtrl)
	{
  	// Retrieve window's text, use that for tab text
    TAPointer<_TCHAR> txt = new _TCHAR[newChildWindow->GetWindowTextLength() + 1] ;
    newChildWindow->GetWindowText(txt, newChildWindow->GetWindowTextLength() + 1) ;
    m_tabCtrl->Add(TTabItem(txt)) ;
    // Select new tab as current
    SetSelectedTab(m_tabCtrl->GetCount() - 1) ;
    newChildWindow->Show(SW_SHOW) ;
    newChildWindow->EnableWindow(true) ;
  }
}
catch (...)
{
	erreur("Exception NSTabWindow::Attach.", standardError, 0) ;
}
}

void
NSTabWindow::SetupWindow()
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string ps = string("Entering NSTabWindow::SetupWindow") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	SetCaption(sTitle.c_str()) ;

    // Create tab control, if not done already
	// if (GetTabCtrl() == 0)
	//  	m_tabCtrl = new TTabControl(this, NSTabWindow::ID_TABCTRL, 0, 0, Parent->GetClientRect().Width(), Parent->GetClientRect().Height()) ;

	//
	// En cas de perte du focus, on cache l'éventuelle fenêtre de pilotage
	// de description multiple pour éviter sa sélection à partir d'une fenêtre
	// fille de cette boite de dialogue
	//
	TWindow* pMere = Parent ;
	NSDialog* pDialog = TYPESAFE_DOWNCAST(pMere, NSDialog) ;
	if (pDialog)
	{
		if (pDialog->_pControleurMulti)
		{
    	pDialog->_pControleurMulti->Show(SW_HIDE) ;
      pDialog->_pControleurMulti->EnableWindow(false) ;
    }
	}

	if (_pControleurMulti)
	{
		_pControleurMulti->setPosition(this) ;
    // donner un titre aux (pBBItem->pBBFilsPere->VectorFils.size())
    // boîtes pBBItem->pBBFilsPere->VectorFils.size() = nombre de
    // boîtes multidialogue
    _pControleurMulti->setTitre(this, _pBBItem->_pBBFilsPere->VectorFils.size()) ;
	}

	// Si le dialogue est statique, on énumère ses contrôles pour créer les
	// bons objets
	//
	if (0 != _ResID)
	{
		erreur("L'archetype possède un DialogBox et contient une ressource", standardError, 0) ;
		return ;
	}
	else if ((_pBBItem == _pBBItem->_pBigBoss->getBBItem()) && (_pBBItem->ouvreArchetype()))
		CreerControlesArchetype() ;
	// Si le dialogue est dynamique, on crée à la main ses contrôles
	else
	{
  	erreur("Le BBItem père n'ouvre pas d'archetype.", standardError, 0) ;
    return ;
	}

	//
	// Lance les éventuelles fonctions d'initialisation des contrôles
	//
	initialiseControles() ;

	//
	// Lorsqu'on initialise à partir du blackboard
	//
#ifdef __OB1__
	if (_pBBItem->_KsInterface.getInitFromBbk())
#else
  if (bInitFromBbk)
#endif
		initControlesFromBbk() ;

// --
// ici fab has something to do with capture
// --
	if (false == pContexte->getEpisodus()->CaptureArray.empty())
	{
		initControlesFromCapture() ;
		// pSuper->pEpisodus->CaptureArray.vider() ;
	}

	//
	// Initialise les contrôles NAUTILUS en fonction du contexte Big Brother
	//
	rafraichitControles() ;

  // It's time for the BBItem to cut bad branches: Ordinary "Fils Guides" that
  // got connected to leaves but don't open a dialog. They tend to cause system
  // hanging
  //
  // Disconnected because it should not be useful for Archetypes (and so far it
  // leads to internal link to open other Archetypes not working)
  //
	// pBBItem->cutBadDialogBranches(this) ;

	//$$ Temporaire
	// ConfigShow();
	pSuper->afficheStatusMessage("") ;

	if (_pView)
	{
		if (pContexte->getPredi())
		{
      NS_CLASSLIB::TRect dlgRect ;
      GetWindowRect(dlgRect) ;

      int iWidth  = dlgRect.Width() ;
      int iHeigth = dlgRect.Height() ;

      int iX = pContexte->getPredi()->_iCurrentX ;
      int iY = pContexte->getPredi()->_iCurrentY ;

      SetWindowPos(0, iX, iY, iWidth, iHeigth, SWP_NOZORDER) ;

      pContexte->getPredi()->_iCurrentX += pContexte->getPredi()->_iDeltaPosX ;
      pContexte->getPredi()->_iCurrentY += pContexte->getPredi()->_iDeltaPosY ;
    }
	}

	// Hide all windows but the first
	if (false == tabbedWindows->empty())
		for (NSWindowsIter i = tabbedWindows->begin() ; tabbedWindows->end() != i; i++)
    	if (tabbedWindows->begin() != i)
    	{
      	(*i)->Show(SW_HIDE) ;
        (*i)->EnableWindow(false) ;
      }

	// Set to the first tab
	SetSelectedTab(0) ;
	m_tabCtrl->Show(SW_SHOW) ;
	m_tabCtrl->EnableWindow(true) ;

	ps = string("Leaving NSTabWindow::SetupWindow") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;
}

void
NSTabWindow::Attach(NSWindowsArray *windowArray)
{
  if (false == windowArray->empty())
    for (NSWindowsIter i = windowArray->begin() ; windowArray->end() != i ; i++)
      Attach(*i) ;
}


NSOneTabWindow
*NSTabWindow::Detach(int index)
{
	NSOneTabWindow *retval = (*tabbedWindows)[index] ;

	// Remove window element from list
    //	tabbedWindows->Detach(index) ;

	return retval ;
}


int
NSTabWindow::Retrieve(NSOneTabWindow *ptr)
{
	int index = 0 ;
  if (false == tabbedWindows->empty())
    for (NSWindowsIter iter = tabbedWindows->begin() ; tabbedWindows->end() != iter ; iter++, index++)
      if (*iter == ptr)
        return index ;

	// throw an exception instead?
	return -1 ;
}


void
NSTabWindow::SetSelectedTab(int index)
{
    m_tabCtrl->SetSel(index) ;

    // TTabControl does not send the TCN_ messages, so we need to "fake" it
    TNotify     dummy ;
    TabSelChanging(dummy) ;
    TabSelChange(dummy) ;
}


TWindow
*NSTabWindow::GetTabCtrl()
{
  return m_tabCtrl ;
}

void
NSTabWindow::CreerControlesArchetype()
{
try
{
	if (((BBItem*) NULL == _pBBItem) || ((nsarcParseur*) NULL == _pBBItem->getParseur()))
		return ;

	NSSuper *pSuper = pContexte->getSuperviseur() ;
	string ps = string("Entrée dans CreerControlesArchetype (NSTabWindow)") ;
	pSuper->trace(&ps, 1) ;

	ps = string("Lancement de getDialogBox") ;
	pSuper->trace(&ps, 1) ;

  string sLang = pContexte->getUserLanguage() ;

	Cdialogbox* pDialogBox = _pBBItem->getArchetypeDialog(sLang) ;
	if ((Cdialogbox*) NULL == pDialogBox)
		return ;

	ps = string("Lancement de getFirstControl") ;
	pSuper->trace(&ps, 1) ;
	Ccontrol* pControl = pDialogBox->getFirstControl();

	ps = string("Lancement de getTabControl") ;
	pSuper->trace(&ps, 1) ;
	Ctabcontrol* pTabControl = pDialogBox->getTabControl();

	UINT  nMaxInput = 255;         // Nombre maximal de caractères à saisir

	// string sRCData = pRCData->getNSContro();

	ps = string("Lancement de getStringRCData") ;
	pSuper->trace(&ps, 1) ;
	string sRCData = pDialogBox->getStringRCData();

	int  prec = 0, suiv ;
	int  hSysButton = 0 ;

	if ((Ctabcontrol*) NULL == pTabControl)
	{
		erreur("L'archetype du DialogBox ne comporte pas d'onglets.", standardError, 0) ;
		return ;
	}

	ps = string("Initialisation des onglets") ;
	pSuper->trace(&ps, 1) ;

	int nbTabs = pTabControl->getNbTabs() ;
	if (0 == nbTabs)
	{
		erreur("L'archetype du DialogBox ne comporte pas d'onglets.", standardError, 0) ;
		return ;
	}

  Ctab* pTab = pTabControl->getFirstTab() ;

	// on crée d'abord l'array des tabbedWindows puis on rajoutera dedans les controles
	while (pTab)
	{
		string sTitle = pTab->getTitle() ;
		string sOrder = pTab->getOrder() ;
		// int    iOrder = atoi(sOrder.c_str()) ;

		NSOneTabWindow *pWinToAdd = new NSOneTabWindow(this, sTitle.c_str()) ;
		tabbedWindows->push_back(pWinToAdd) ;

		pTab = pTabControl->getNextTab(pTab) ;
	}

	suiv = sRCData.find("|") ;

	ps = string("Création des contrôles") ;
	pSuper->trace(&ps, 1) ;

	while (pControl)
	{
		string sType     = pControl->getType() ;
		string sCoords   = pControl->getCoords() ;
		string sCaption  = pControl->getCaption() ;
		string sFilling  = pControl->getFilling() ;
    string sHelpText = pControl->getHelpText() ;
		int    ctrlID    = pControl->getRefId() ;
		int    indexTab  = atoi((pControl->getTab()).c_str()) ;
		int    nIndex    = _iIndexCtrl ;  // Position dans *pConfigCache

		ps = string(" -> Controle : ") + sType ;
		pSuper->trace(&ps, 1) ;

		// conversion du type en majuscules
    pseumaj(&sType);

    int coords[4] ;
		coords[0] = pControl->getX();
		coords[1] = pControl->getY();
		coords[2] = pControl->getW();
		coords[3] = pControl->getH();

		// Rectangle de conversion des coordonnées en unités de boite de dialogue
		NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(coords[0], coords[1],
                                        coords[0] + coords[2],
                                        coords[1] + coords[3]) ;
		MapDialogRect(cvtRect);

		coords[0] = cvtRect.Left() ;
		coords[1] = cvtRect.Top() ;
		coords[2] = cvtRect.Width() ;
		coords[3] = cvtRect.Height() ;

		string sIdentite = string(sRCData, prec, suiv - prec) ;
		string sDlgFonction = "";

		if (sIdentite != "")
		{
			size_t fct_posit = sIdentite.find(':') ;
			if (fct_posit != string::npos)
			{
				if (fct_posit < strlen(sIdentite.c_str()))
					sDlgFonction = string(sIdentite, fct_posit + 1, strlen(sIdentite.c_str())) ;
				sIdentite = string(sIdentite, 0, fct_posit) ;
			}
		}

		ps = string(" --> Identité : ") + sIdentite ;
		pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

		// Traitement suivant le type de contrôle rencontré

		ps = string(" --> Création du controle : ") ;
		pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

		//
		// TreeView
		//
		if (sType == string("SYSTREEVIEW32"))
		{
			NSTreeWindow* pNSTreeWindow = new NSTreeWindow(this, pContexte, ctrlID,
                                                coords[0], coords[1], coords[2], coords[3]) ;
      pNSTreeWindow->Attr.Style = pControl->getIStyle() ;

			((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSTreeWindow) ;

      pNSTreeWindow->pBBitemNSTreeWindow = _pBBItem ;
      NSControle* pControl = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pControl->setControle(dynamic_cast<void*>(pNSTreeWindow)) ;
      pControl->setType(pNSTreeWindow->donneType()) ;
      pControl->setFilling(sFilling) ;
      pControl->setHelpText(sHelpText) ;
      pControl->setNSDialog(this) ;
      pNSTreeWindow->setControl(pControl) ;
      referenceControle(pControl) ;
		}

		//
		// ListView
		//
		if (sType == string("SYSLISTVIEW32"))
		{
			// Liste d'adresses
			if (sIdentite.find(string("LCADR")) != NPOS)
			{
				NSAdrListWindow* pNSAdrListWindow = new NSAdrListWindow(this,
                                                pContexte, ctrlID,
                                                coords[0], coords[1], coords[2], coords[3]) ;
				pNSAdrListWindow->Attr.Style = pControl->getIStyle() ;

				((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSAdrListWindow) ;

        pNSAdrListWindow->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pNSAdrListWindow->pControle->setControle(dynamic_cast<void*>(pNSAdrListWindow)) ;
        pNSAdrListWindow->pControle->setType(pNSAdrListWindow->donneType()) ;
        pNSAdrListWindow->pControle->setFilling(sFilling) ;
        pNSAdrListWindow->pControle->setHelpText(sHelpText) ;
        pNSAdrListWindow->pControle->setNSDialog(this) ;
        // on ajoute ici un indicateur pour préciser que ce controle
        // gère lui-même la gestion multiple sans lancer de multi-dialogue
        pNSAdrListWindow->pControle->setGestionMultiple(true);
        referenceControle(pNSAdrListWindow->pControle);
      }
      else if (sIdentite.find(string("LCORR")) != NPOS)
      {
      	NSCorListWindow* pNSCorListWindow = new NSCorListWindow(this,
            			                                  pContexte, ctrlID,
                                                coords[0], coords[1], coords[2], coords[3]) ;
        pNSCorListWindow->Attr.Style = pControl->getIStyle() ;

        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSCorListWindow);

        pNSCorListWindow->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction);
        pNSCorListWindow->pControle->setControle(dynamic_cast<void*>(pNSCorListWindow));
        pNSCorListWindow->pControle->setType(pNSCorListWindow->donneType());
        pNSCorListWindow->pControle->setFilling(sFilling);
        pNSCorListWindow->pControle->setHelpText(sHelpText) ;
        pNSCorListWindow->pControle->setNSDialog(this);
        // on ajoute ici un indicateur pour préciser que ce controle
        // gère lui-même la gestion multiple sans lancer de multi-dialogue
        pNSCorListWindow->pControle->setGestionMultiple(true);
        referenceControle(pNSCorListWindow->pControle);
      }
      else
      {
      	string sItem ;
        size_t pos = sIdentite.find_last_of('/') ;
        if (pos != NPOS)
        	sItem = string(sIdentite, pos+1, strlen(sIdentite.c_str())-pos-1) ;
        else
        	sItem = sIdentite ;

        if (sItem[0] == 'V')
        {
        	NSHistorizedValListWindow* pNSHistoListWindow = new NSHistorizedValListWindow(this,
                                                    pContexte, ctrlID,
                                                    coords[0], coords[1], coords[2], coords[3]) ;
          pNSHistoListWindow->Attr.Style = pControl->getIStyle() ;

          pNSHistoListWindow->setItemValue(sItem) ;

          ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSHistoListWindow) ;

          pNSHistoListWindow->_pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
          pNSHistoListWindow->_pControle->setControle(dynamic_cast<void*>(pNSHistoListWindow)) ;
          pNSHistoListWindow->_pControle->setType(pNSHistoListWindow->donneType()) ;
          pNSHistoListWindow->_pControle->setFilling(sFilling) ;
          pNSHistoListWindow->_pControle->setHelpText(sHelpText) ;
          pNSHistoListWindow->_pControle->setNSDialog(this) ;
          // on ajoute ici un indicateur pour préciser que ce controle
          // gère lui-même la gestion multiple sans lancer de multi-dialogue
          pNSHistoListWindow->_pControle->setGestionMultiple(true) ;
          referenceControle(pNSHistoListWindow->_pControle) ;
        }
        else
        {
        	NSHistorizedListWindow* pNSHistoListWindow = new NSHistorizedListWindow(this,
                                                    pContexte, ctrlID,
                                                    coords[0], coords[1], coords[2], coords[3]) ;
          pNSHistoListWindow->Attr.Style = pControl->getIStyle() ;

          ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSHistoListWindow) ;

          pNSHistoListWindow->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
          pNSHistoListWindow->pControle->setControle(dynamic_cast<void*>(pNSHistoListWindow)) ;
          pNSHistoListWindow->pControle->setType(pNSHistoListWindow->donneType()) ;
          pNSHistoListWindow->pControle->setFilling(sFilling) ;
          pNSHistoListWindow->pControle->setHelpText(sHelpText) ;
          pNSHistoListWindow->pControle->setNSDialog(this) ;
          // on ajoute ici un indicateur pour préciser que ce controle
          // gère lui-même la gestion multiple sans lancer de multi-dialogue
          pNSHistoListWindow->pControle->setGestionMultiple(true) ;
          referenceControle(pNSHistoListWindow->pControle) ;
        }
      }
    }

		// contrôle vallistview - from FLP
		if (sType == string("VALLISTVIEW32"))
		{
			NSHistorizedValListWindow *pNSHistoValWindow = new NSHistorizedValListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
      pNSHistoValWindow->Attr.Style = pControl->getIStyle() ;

      ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSHistoValWindow) ;

			pNSHistoValWindow->_pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
			pNSHistoValWindow->_pControle->setControle(dynamic_cast<void*>(pNSHistoValWindow)) ;
			pNSHistoValWindow->_pControle->setType(pNSHistoValWindow->donneType()) ;
			pNSHistoValWindow->_pControle->setFilling(sFilling);
      pNSHistoValWindow->_pControle->setHelpText(sHelpText) ;
			pNSHistoValWindow->_pControle->setNSDialog(this) ;
			// on ajoute ici un indicateur pour préciser que ce controle
			// gère lui-même la gestion multiple sans lancer de multi-dialogue
			pNSHistoValWindow->_pControle->setGestionMultiple(true) ;
			referenceControle(pNSHistoValWindow->_pControle) ;
		}

		//
    // Contrôle Edit
    //
    if (sType == string("EDIT"))
    {
    	char szType[25] ; // Type de contrôle EDIT
      size_t posLex ;
    	string sIden = "" ;
      //NSEdit qui bosse avec le lexique
      if ((sIdentite.find(string("£C;")) != NPOS) || (sIdentite.find(string("/£C;")) != NPOS))
      {
      	NSEditLexique* pNSEdit = new NSEditLexique(this, pContexte,
                                            ctrlID,
                                            pContexte->getDico(),
                                            "", coords[0], coords[1], coords[2], coords[3],
                                            nMaxInput+1) ;
        pNSEdit->setFreeTextEnabled(true) ;
        pNSEdit->Attr.Style = pControl->getIStyle() ;

        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSEdit);

        pNSEdit->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction);
        pNSEdit->pControle->setControle(dynamic_cast<void*>(pNSEdit));
        pNSEdit->pControle->setType(pNSEdit->donneType());
        pNSEdit->pControle->setFilling(sFilling);
        pNSEdit->pControle->setHelpText(sHelpText) ;
        pNSEdit->pControle->setNSDialog(this);
        referenceControle(pNSEdit->pControle);
      }
      else if (sIdentite.find(string("£CX")) != NPOS)
      {
        NSEditLexique* pNSEdit = new NSEditLexique(this, pContexte,
                                            ctrlID,
                                            pContexte->getDico(),
                                            "", coords[0], coords[1], coords[2], coords[3],
                                            nMaxInput+1) ;
        pNSEdit->setFreeTextEnabled(false) ;
        pNSEdit->Attr.Style = pControl->getIStyle() ;

        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSEdit);

        pNSEdit->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction);
        pNSEdit->pControle->setControle(dynamic_cast<void*>(pNSEdit));
        pNSEdit->pControle->setType(pNSEdit->donneType());
        pNSEdit->pControle->setFilling(sFilling);
        pNSEdit->pControle->setHelpText(sHelpText) ;
        pNSEdit->pControle->setNSDialog(this);
        referenceControle(pNSEdit->pControle);
      }
      // Champ edit qui n'ouvre pas le lexique et aliment Complement
      //
      else if (sIdentite.find(string("£CC")) != NPOS)      {      	NSEditNoLex* pEdit ;        strcpy(szType, "C0") ;        posLex = sIdentite.find(string("£CC")) ;        string sElemLex = string(sIdentite, posLex, BASE_LEXIQUE_LEN) ;        string sLen = string(sElemLex, 4, 2) ;        int iLen = atoi(sLen.c_str()) ;        pEdit = new NSEditNoLex(pContexte, this, ctrlID, szType, "",                                        coords[0], coords[1], coords[2], coords[3], sLang, iLen) ;        pEdit->Attr.Style = pControl->getIStyle() ;        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pEdit) ;        pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction)) ;        pEdit->getControl()->setControle(dynamic_cast<void*>(pEdit)) ;
        pEdit->getControl()->setType(pEdit->donneType()) ;
        pEdit->getControl()->setFilling(sFilling) ;
        pEdit->getControl()->setHelpText(sHelpText) ;
        pEdit->getControl()->setNSDialog(this) ;
        referenceControle(pEdit->getControl()) ;

        //        // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
        //
        _iIndexEdit++ ;
      }      // Champ edit qui n'ouvre pas le lexique et alimente Texte libre      //      else if (sIdentite.find(string("£CL")) != NPOS)      {      	NSEditNoLex* pEdit ;        posLex = sIdentite.find(string("£CL")) ;        string sElemLex = string(sIdentite, posLex, BASE_LEXIQUE_LEN) ;        strcpy(szType, string(sElemLex, 2, 2).c_str()) ;        string sLen = string(sElemLex, 4, 2) ;        int iLen = atoi(sLen.c_str()) ;        pEdit = new NSEditNoLex(pContexte, this, ctrlID, szType, "",                                        coords[0], coords[1], coords[2], coords[3], sLang, iLen) ;        pEdit->Attr.Style = pControl->getIStyle() ;        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pEdit) ;        pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction)) ;        pEdit->getControl()->setControle(dynamic_cast<void*>(pEdit)) ;
        pEdit->getControl()->setType(pEdit->donneType()) ;
        pEdit->getControl()->setFilling(sFilling) ;
        pEdit->getControl()->setHelpText(sHelpText) ;
        pEdit->getControl()->setNSDialog(this) ;
        referenceControle(pEdit->getControl()) ;

        //        // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
        //
        _iIndexEdit++ ;
      }      else
      {
      	//
        // Index du contrôle
				//
        nIndex = _iIndexEdit ;
        //
        // Type de saisie dans le contrôle
        //
        char szEditDateType[2] ; //type de date du controle EDIT
        string sLivre ;
        string sTypeDate ;
        size_t pos = sIdentite.find(string("£")) ;
        if (NPOS != pos)
        {
        	sLivre = string(sIdentite, pos + 1, 1) ;
          strcpy(szType, sLivre.c_str()) ;

          sTypeDate = string(sIdentite, pos + 2, 1) ;
          strcpy(szEditDateType, sTypeDate.c_str()) ;
        }
        //
        // Création de l'objet OWL correspondant à l'objet d'interface
        //
        NSEdit* pEdit = NULL ;
        WNDTYPE	iEditType = isEdit ;
        switch (szType[0])
        {
        	case codeMARK :
          	pEdit = new NSEdit(pContexte, this, ctrlID, szType, "",
                               coords[0], coords[1], coords[2], coords[3], sLang) ;
            break ;
          case nbMARK :
          	pEdit = new NSEdit(pContexte, this, ctrlID, szType, "",
                               coords[0], coords[1], coords[2], coords[3], sLang) ;
            break ;
          case charMARK :
          	pEdit = new NSEdit(pContexte, this, ctrlID, szType, "",
                               coords[0], coords[1], coords[2], coords[3], sLang) ;
            break ;
          case dateMARK :
          	//NSEditDate*
            pEdit = new NSEditDate(pContexte, this, ctrlID, szType, "",
                                   coords[0], coords[1], coords[2], coords[3], sLang, szEditDateType[0]) ;
            iEditType = isEditDate ;
            break ;
          case dateHeureMARK :
          	pEdit = new NSEditDateHeure(pContexte, this, ctrlID, szType, "",
                                   coords[0], coords[1], coords[2], coords[3], sLang, szEditDateType[0]) ;
            iEditType = isEditDateHeure ;
            break ;
          case heureMARK :
          	pEdit = new NSEditHeure(pContexte, this, ctrlID, szType, "",
                                    coords[0], coords[1], coords[2], coords[3], sLang) ;
            break ;
          default :
          	string sErrMess = string("Unknown Edit type : ") + string(1, szType[0]) ;
            erreur(sErrMess.c_str(), standardError, 0) ;
        }

        if (pEdit)
        {
        	pEdit->Attr.Style = pControl->getIStyle() ;

          ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pEdit) ;

          pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction)) ;
          pEdit->getControl()->setControle(dynamic_cast<void*>(pEdit)) ;
          // pEdit->pControle->setType(pEdit->donneType()) ; // doesn't work; always returns isEdit type
          pEdit->getControl()->setType(iEditType) ;
          pEdit->getControl()->setFilling(sFilling) ;
          pEdit->getControl()->setHelpText(sHelpText) ;
          pEdit->getControl()->setNSDialog(this) ;
          referenceControle(pEdit->getControl()) ;
          //
          // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
          //
          _iIndexEdit++ ;
        }
      }
    }

    //
    // Static
    //
    if ((sType == string("STATIC")) || (sType == string("BORSTATIC")))
    {
    	// Création de l'objet OWL correspondant à l'objet d'interface
      NSStatic* pNSStatic = new NSStatic(pContexte, this, ctrlID, sCaption.c_str(),
                                                coords[0], coords[1], coords[2], coords[3]);
      pNSStatic->Attr.Style = pControl->getIStyle() ;

      ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pNSStatic);

      pNSStatic->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction);
      pNSStatic->pControle->setControle(dynamic_cast<void*>(pNSStatic));
      pNSStatic->pControle->setType(pNSStatic->donneType());
      pNSStatic->pControle->setFilling(sFilling);
      pNSStatic->pControle->setHelpText(sHelpText) ;
      pNSStatic->pControle->setNSDialog(this);
      referenceControle(pNSStatic->pControle);
    }

    //
    // Contrôle Button (à voir pour OK et Cancel)
    // ---------------
    if ((string("BUTTON") == sType) &&
           (IDOK   != ctrlID) && (IDCANCEL   != ctrlID) &&
           (IDB_OK != ctrlID) && (IDB_CANCEL != ctrlID))
    {
    	// Détermination du style
      // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE);
      int nIndex = pControl->getIStyle();

      //      // Contrôle RadioButton
      //
      if (((nIndex & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)||
          ((nIndex & BS_RADIOBUTTON) == BS_RADIOBUTTON))
      {      	//
        // Création de l'objet OWL correspondant à l'objet d'interface
        //
        NSRadioButton* pRaBut = new NSRadioButton(pContexte, this, ctrlID, sCaption.c_str(),
                                                            coords[0], coords[1], coords[2], coords[3], _pGroupCurr) ;
        pRaBut->Attr.Style = pControl->getIStyle() ;

        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pRaBut) ;

        pRaBut->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pRaBut->pControle->setControle(dynamic_cast<void*>(pRaBut)) ;
        pRaBut->pControle->setType(pRaBut->donneType()) ;
        pRaBut->pControle->setFilling(sFilling) ;
        pRaBut->pControle->setHelpText(sHelpText) ;
        pRaBut->pControle->setNSDialog(this) ;
        referenceControle(pRaBut->pControle) ;
      }
      //
      // Contrôle CheckBox
      //
      else if (((nIndex & BS_CHECKBOX) == BS_CHECKBOX)||
               ((nIndex & BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX))
      {
      	//
        // Création de l'objet OWL correspondant à l'objet d'interface
        //
        //TCheckBox* pChkBox = new TCheckBox(pDlgCurrent, ctrlID, pDlgCurrent->pGroupCurr);
        //pControle->pCtrl = dynamic_cast<void*>(pChkBox);
        //pControle->sTypeCtrl = "CheckBox";
      }
      else if ((nIndex & BS_GROUPBOX) == BS_GROUPBOX)
      {
      	if (sIdentite != "")
        {
        	NSGroupBox* pGrpBox = new NSGroupBox(pContexte, this, ctrlID, sCaption.c_str(),
                                                            coords[0], coords[1], coords[2], coords[3]) ;
          pGrpBox->Attr.Style = pControl->getIStyle() ;

          ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pGrpBox) ;

          pGrpBox->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
          pGrpBox->pControle->setControle(dynamic_cast<void*>(pGrpBox)) ;
          pGrpBox->pControle->setType(pGrpBox->donneType()) ;
          pGrpBox->pControle->setFilling(sFilling) ;
          pGrpBox->pControle->setHelpText(sHelpText) ;
          pGrpBox->pControle->setNSDialog(this) ;
          referenceControle(pGrpBox->pControle) ;
          _pGroupCurr = (OWL::TGroupBox*)pGrpBox ;
        }
        else
        {
        	OWL::TGroupBox* pGroupBox = new OWL::TGroupBox(this, ctrlID, sCaption.c_str(),
                                                            coords[0], coords[1], coords[2], coords[3]) ;
          pGroupBox->Attr.Style = pControl->getIStyle() ;

          ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pGroupBox);

          _aGroups.push_back(pGroupBox) ;
          _pGroupCurr = pGroupBox ;
        }
      }
      //
      // Contrôle Button (par défaut ne participe au transfert )
      //
      else
      {
      	NSButton* pButt = new NSButton(pContexte, this, ctrlID, sCaption.c_str(),
                                                coords[0], coords[1], coords[2], coords[3]);
        pButt->Attr.Style = pControl->getIStyle() ;

        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pButt) ;

        pButt->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pButt->pControle->setControle(dynamic_cast<void*>(pButt)) ;
        pButt->pControle->setType(pButt->donneType()) ;
        pButt->pControle->setFilling(sFilling) ;
        pButt->pControle->setHelpText(sHelpText) ;
        pButt->pControle->setNSDialog(this) ;
        referenceControle(pButt->pControle) ;
      }
    }
    else if (sType == string("BUTTON"))
    {
    	// Les boutons OK et CANCEL ne sont pas créés dans le cas d'une vue
      // OK and CANCEL buttons are not created when the dialog is inside a view
      //
      if ((_pBBItem->getView()) &&
              ((ctrlID == IDOK)   || (ctrlID == IDCANCEL)))
      {
      	NSBBMUEView* pMueView = _pBBItem->getView() ;
        if (pMueView)
        {
        	if      (IDOK == ctrlID)
          	pMueView->uButtonsStyle |= MYWS_OK ;
          else if (ctrlID == IDCANCEL)
          	pMueView->uButtonsStyle |= MYWS_CANCEL ;
        }
      }
      else
      {
      	// Bouton systeme : ne pas referencer dans les controles de tabbedWindows
        TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(),
                                            coords[0], coords[1], coords[2], coords[3]);
        pButt->Attr.Style = pControl->getIStyle() ;

        if ((coords[1] + coords[3]) > hSysButton)
        	hSysButton = coords[1] + coords[3] ;
      }
    }

    //
    // Contrôle ScrollBar
    //------------------
    if (sType == string("SCROLLBAR"))
    {
    	int nIndex = pControl->getIStyle() ;

      if ((nIndex& WS_HSCROLL) == WS_HSCROLL)      {      	// Création de l'objet OWL correspondant à l'objet d'interface
        TScrollBar* pScroll = new TScrollBar(this, ctrlID, coords[0], coords[1], coords[2], coords[3], true) ;
        pScroll->Attr.Style = pControl->getIStyle() ;
        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pScroll) ;
      }
      else
      {
      	// Création de l'objet OWL correspondant à l'objet d'interface
        TScrollBar* pScroll = new TScrollBar(this, ctrlID, coords[0], coords[1], coords[2], coords[3], false) ;
        pScroll->Attr.Style = pControl->getIStyle() ;
        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pScroll) ;
      }
      // typeCtrl = "ScrollBar";
      // Allocation pour le transfert et initialisation
      // void* pTransferBufCtrl = reinterpret_cast<void*>(new TScrollBarData());
    }

    //
    // BORLAND : Contrôle Bouton
    //
    if ((sType == string("BORBTN"))
                && (ctrlID != IDOK)
                && (ctrlID != IDCANCEL)
                && (ctrlID != IDB_OK)
                && (ctrlID != IDB_CANCEL)
                && (ctrlID != IDRETOUR)
                && (ctrlID != IDSUITE)
                && (ctrlID != IDHELP)
                && (ctrlID != IDHELPWWW)
                && (ctrlID != IDHELPNEW)
                && (ctrlID != IDTREEPASTE)
                && (ctrlID != IDCONCLUSION)
                && (ctrlID != IDBBKCALCUL)            )
    {
    	// Détermination du style
      // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE);
      int nIndex = pControl->getIStyle();

      //
      // Contrôle RadioButton
      //
      if ( !((nIndex& BBS_BITMAP) == BBS_BITMAP) )
      {
      	NSButton* pButt = new NSButton(pContexte, this, ctrlID, sCaption.c_str(),
                                                coords[0], coords[1], coords[2], coords[3]);
        pButt->Attr.Style = pControl->getIStyle() ;

        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pButt) ;

        pButt->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pButt->pControle->setControle(dynamic_cast<void*>(pButt)) ;
        pButt->pControle->setType(pButt->donneType()) ;
        pButt->pControle->setFilling(sFilling) ;
        pButt->pControle->setHelpText(sHelpText) ;
        pButt->pControle->setNSDialog(this) ;
        referenceControle(pButt->pControle) ;
      }
    }
    else if (sType == string("BORBTN"))
    {
    	if ((_pBBItem->getView()) && ((IDOK == ctrlID) || (IDCANCEL == ctrlID)))
      {
      	NSBBMUEView* pMueView = _pBBItem->getView() ;
        if (pMueView)
        {
        	if      (ctrlID == IDOK)
          	pMueView->uButtonsStyle |= MYWS_OK ;
          else if (ctrlID == IDCANCEL)
          	pMueView->uButtonsStyle |= MYWS_CANCEL ;
        }
      }
      else
      {
      	// bouton systeme : ne pas referencer dans les controles de tabbedWindows
        TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(),
                                            coords[0], coords[1], coords[2], coords[3]);
        pButt->Attr.Style = pControl->getIStyle() ;

        if ((coords[1] + coords[3]) > hSysButton)
        	hSysButton = coords[1] + coords[3];
      }
    }

    //
    // BORLAND : Contrôle CheckBox
    //
    if (sType == string("BORCHECK"))
    {
    	NSCheckBox* pChkBox = new NSCheckBox(pContexte, this, ctrlID, sCaption.c_str(),
                                                    coords[0], coords[1], coords[2], coords[3], _pGroupCurr);
      pChkBox->Attr.Style = pControl->getIStyle() ;

      ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pChkBox) ;

      pChkBox->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pChkBox->pControle->setControle(dynamic_cast<void*>(pChkBox)) ;
      pChkBox->pControle->setType(pChkBox->donneType()) ;
      pChkBox->pControle->setFilling(sFilling) ;
      pChkBox->pControle->setHelpText(sHelpText) ;
      pChkBox->pControle->setNSDialog(this) ;
			referenceControle(pChkBox->pControle) ;
    }

    //
    // Contrôle ComboBox
    //------------------
    if (sType == string("COMBOBOX"))
    {
    	// is it a classification or not ?
      bool bClassif = false ;
      VectString aVecteurString ;
			NSSuper* pSuper = pContexte->getSuperviseur() ;
			pSuper->getFilGuide()->TousLesVrais("0CODE", "ES", &aVecteurString, "ENVERS") ;			if (false == aVecteurString.empty())			{				string sIdentSens ;				pContexte->getDico()->donneCodeSens(&sIdentite, &sIdentSens) ;        if (aVecteurString.contains(sIdentSens))        	bClassif = true ;			}

      if (bClassif)
      {
        NSComboClassif* pComboB = new NSComboClassif(pContexte, this, ctrlID, coords[0],
                            coords[1], coords[2], coords[3], pControl->getIStyle(), 0) ;
        pComboB->Attr.Style = pControl->getIStyle() ;

        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pComboB) ;

        pComboB->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction);
        pComboB->pControle->setControle(dynamic_cast<void*>(pComboB));
        pComboB->pControle->setType(pComboB->donneType());
        pComboB->pControle->setFilling(sFilling);
        pComboB->pControle->setHelpText(sHelpText) ;
        pComboB->pControle->setNSDialog(this);
        referenceControle(pComboB->pControle);
      }
      else
      {
      	NSComboSemantique * pComboB = new NSComboSemantique(pContexte, this, ctrlID, coords[0], coords[1], coords[2], coords[3], pControl->getIStyle(), 0) ;
        pComboB->Attr.Style = pControl->getIStyle() ;

        ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pComboB) ;

        pComboB->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pComboB->pControle->setControle(dynamic_cast<void *>(pComboB)) ;
        pComboB->pControle->setType(pComboB->donneType()) ;
        pComboB->pControle->setFilling(sFilling) ;
        pComboB->pControle->setHelpText(sHelpText) ;
        pComboB->pControle->setNSDialog(this) ;
        referenceControle(pComboB->pControle) ;
      }
    }

    //
    // BORLAND : Contrôle RadioBouton
    //
    if (sType == string("BORRADIO"))
    {
    	NSRadioButton* pRaBut = new NSRadioButton(pContexte, this, ctrlID, sCaption.c_str(),
                                                        coords[0], coords[1], coords[2], coords[3], _pGroupCurr) ;
      pRaBut->Attr.Style = pControl->getIStyle() ;

      ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pRaBut) ;

      pRaBut->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction);
      pRaBut->pControle->setControle(dynamic_cast<void*>(pRaBut));
      pRaBut->pControle->setType(pRaBut->donneType());
      pRaBut->pControle->setFilling(sFilling);
      pRaBut->pControle->setHelpText(sHelpText) ;
      pRaBut->pControle->setNSDialog(this);
      referenceControle(pRaBut->pControle);
    }

    //
    // BORLAND : Contrôle Onglets
    //
    if (string("SYSTABCONTROL32") == sType)
    {
    	NSOnglet* pOnglet = new NSOnglet(pContexte, this, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
      pOnglet->Attr.Style = pControl->getIStyle() ;

      ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pOnglet) ;

      NSControle* pControl = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pControl->setControle(dynamic_cast<void*>(pOnglet)) ;
      pControl->setType(pOnglet->donneType()) ;
      pControl->setFilling(sFilling) ;
      pControl->setHelpText(sHelpText) ;
      pControl->setNSDialog(this) ;

      pOnglet->setControl(pControl) ;
      referenceControle(pControl) ;
    }

    //
    // BORLAND : Contrôle BorShade
    //
    if (sType == string("BORSHADE"))
    {
    	// Détermination du style
      // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE);
      int nIndex = pControl->getIStyle();

      //
      // Contrôle Group
      //
      if (((nIndex & BSS_RGROUP) == BSS_RGROUP) ||
          ((nIndex & WS_GROUP) == WS_GROUP))
      {
      	if (sIdentite != "")
        {
        	NSGroupBox* pGrpBox = new NSGroupBox(pContexte, this, ctrlID, sCaption.c_str(),
                                                            coords[0], coords[1], coords[2], coords[3]) ;
          pGrpBox->Attr.Style = pControl->getIStyle() ;

          ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pGrpBox) ;

          pGrpBox->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
          pGrpBox->pControle->setControle(dynamic_cast<void*>(pGrpBox)) ;
          pGrpBox->pControle->setType(pGrpBox->donneType()) ;
          pGrpBox->pControle->setFilling(sFilling) ;
          pGrpBox->pControle->setHelpText(sHelpText) ;
          pGrpBox->pControle->setNSDialog(this) ;
          referenceControle(pGrpBox->pControle) ;
          _pGroupCurr = (OWL::TGroupBox*)pGrpBox ;
        }
        else
        {
        	OWL::TGroupBox* pGroupBox = new OWL::TGroupBox(this, ctrlID, sCaption.c_str(),
                                                            coords[0], coords[1], coords[2], coords[3]) ;
          pGroupBox->Attr.Style = pControl->getIStyle() ;

          ((*tabbedWindows)[indexTab-1])->controls->push_back((TWindow *) pGroupBox) ;

          _aGroups.push_back(pGroupBox) ;
          _pGroupCurr = pGroupBox ;
        }
      }
    }

    //
    // Incrémentation du compteur
    //
    _iIndexCtrl++ ;

    prec = suiv + 1 ;
    suiv = sRCData.find("|", prec) ;

    ps = string(" --> Controle suivant ") ;
    pSuper->trace(&ps, 1) ;

    pControl = pDialogBox->getNextControl(pControl) ;
	}

  ps = string("Fin de l'énumération") ;
  pSuper->trace(&ps, 1) ;

  // On fixe la taille de la boite de dialogue
  //
  NS_CLASSLIB::TRect dlgSizeRect(0, 0, pDialogBox->getW(), pDialogBox->getH());
  MapDialogRect(dlgSizeRect);

  ps = string("Appel du SetupWindow") ;
  pSuper->trace(&ps, 1) ;

  //
  // Appel du SetupWindow() de TDialog
  //
  TDialog::SetupWindow();

  // On récupère la HFONT du dialogue
  //
  hBorFont = GetWindowFont();

  ps = string("On fixe le titre du dialogue") ;
  pSuper->trace(&ps, 1) ;

  //
  // On fixe la caption du dialogue
  //
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

	//
	// Redimentionnement
	//
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect clientRect ;
  GetClientRect(clientRect) ;

	//
	// On compare le clientRect réel avec les dimensions souhaitées,
	// et on modifie le WindowRect en conséquence
	//
	int nouvWindowWidth  = dlgRect.Width()  + (dlgSizeRect.Width()  - clientRect.Width()) ;
	int nouvWindowHeight = dlgRect.Height() + (dlgSizeRect.Height() - clientRect.Height()) ;

	ps = string("On redimensionne") ;
	pSuper->trace(&ps, 1) ;

	_bCanMove = true ;
	MoveWindow(dlgRect.left, dlgRect.top, nouvWindowWidth, nouvWindowHeight) ;
	// ATTENTION : contrairement à ce que dit l'aide en ligne, les coordonnées X,Y
	// dans MoveWindow sont relatives à la zone client de la fenetre Parent (ici la NSTabWindow)
	m_tabCtrl->MoveWindow(0, 0, nouvWindowWidth, nouvWindowHeight, true) ;

    // on construit maintenant les tabs
	// Add the tabs & pages already in tabbedWindowArray
	NSOneTabWindow *ptr ;
	int hTemp, hTab = 0 ;
	if (!(tabbedWindows->empty()))
	{
		for (NSWindowsIter i = tabbedWindows->begin() ; i != tabbedWindows->end() ; i++)
		{
			ptr = *i ;

      // Reparent and create
      ptr->SetParent(this) ;
      if (!ptr->GetHandle())
      	ptr->Create() ;
      // Retrieve window's text, use that for tab text
      TAPointer<_TCHAR> txt = new _TCHAR[ptr->GetWindowTextLength() + 1] ;
      ptr->GetWindowText(txt, ptr->GetWindowTextLength()+1) ;
      m_tabCtrl->Add(TTabItem(txt)) ;
      // Select new tab as current
      m_tabCtrl->SetSel(m_tabCtrl->GetCount() - 1) ;
      // Center and display associated TWindow*
      hTemp = AdjustTabClient() ;

      // on note la hauteur de la barre des tabs
      if (hTemp > hTab)
      	hTab = hTemp ;
    }
	}

	if (hSysButton > 0)
	{
		dlgSizeRect.bottom = dlgSizeRect.top + hSysButton + 4 ;
		nouvWindowHeight = dlgRect.Height() + (dlgSizeRect.Height() - clientRect.Height()) ;
		hTab = 0 ;
	}

	_bCanMove = true ;
	MoveWindow(dlgRect.left, dlgRect.top, nouvWindowWidth, nouvWindowHeight + hTab) ;

	//
	// Evaluation du rectangle utilisable à l'écran
	//
	NS_CLASSLIB::TRect usableRect ;
	//
	// Taille de l'écran - Screen size
	//
	TScreenDC screenDC ;
	int iHorzRes = screenDC.GetDeviceCaps(HORZRES) ;
	int iVertRes = screenDC.GetDeviceCaps(VERTRES) ;

	usableRect = NS_CLASSLIB::TRect(dlgRect.TopLeft(), NS_CLASSLIB::TPoint(iHorzRes,iVertRes)) ;

	//
	// TaskBar de Windows
	//
	// RECT        rect;
	APPBARDATA  AppBarData;

	AppBarData.hWnd = ::FindWindow("Shell_TrayWnd", NULL);
	if (AppBarData.hWnd != 0)
	{
		AppBarData.cbSize = sizeof(AppBarData);
    int iResult = ::SHAppBarMessage(ABM_GETTASKBARPOS, &AppBarData);
    if (iResult)
    {
    	switch (AppBarData.uEdge)
      {
      	case ABE_BOTTOM :
        	usableRect.bottom = AppBarData.rc.top ;
          break ;
        case ABE_LEFT   :
        	if (usableRect.Left() < AppBarData.rc.right)
          	usableRect.left = AppBarData.rc.right ;
        	break ;
        case ABE_RIGHT  :
        	if (usableRect.Right() > AppBarData.rc.left)
          	usableRect.right = AppBarData.rc.left ;
          break ;
        case ABE_TOP    :
        	break ;
      }
    }
  }

	int newTop = dlgRect.top - ((nouvWindowHeight + hTab) - usableRect.Height()) ;
	if (newTop < 0)
		newTop = 0 ;

	if (newTop < dlgRect.top)
		MoveWindow(dlgRect.left, newTop, nouvWindowWidth, nouvWindowHeight + hTab) ;
}
catch (...)
{
	erreur("Exception NSTabWindow::CreerControlesArchetype.", standardError, 0) ;
}
}

void
NSTabWindow::CmOk()
{
	NSDialog::CmOk() ;
}

void
NSTabWindow::CmCancel()
{
	NSDialog::CmCancel() ;
}

void
NSTabWindow::CmHelp()
{
}


// -----------------------------------------------------------------------------
// TWin95TabWindow
// -----------------------------------------------------------------------------
// from OWL Extension Library
// present in original file - not use here
// -----------------------------------------------------------------------------
// most of the source code of NSTabWindow is inspired from TWin95TabWindow and
// TTabWindow
// -----------------------------------------------------------------------------

/*
TResponseTableEntry< TWin95TabWindow >  TWin95TabWindow::__entries[] =
{
  EV_WM_SIZE,
  EV_TCN_SELCHANGE(TTabWindow::ID_TABCTRL, TabSelChange),
  EV_TCN_SELCHANGING(TTabWindow::ID_TABCTRL, TabSelChanging),
  EV_TCN_KEYDOWN(TTabWindow::ID_TABCTRL, TabKeyDown),
  {0, 0, 0, 0}
};


TWin95TabWindow::TWin95TabWindow(TWindow *parent, TWindow *array[])
  : TTabWindow(parent, array),
    m_tabCtrl(0)
{
}


TWin95TabWindow::TWin95TabWindow(TWindow *parent, uint32 X, uint32 Y, uint32 W, uint32 H, TWindow *array[])
  : TTabWindow(parent, X, Y, W, H, array),
    m_tabCtrl(0)
{
}


TWin95TabWindow::~TWin95TabWindow()
{
	delete m_tabCtrl ;
}


int32
TWin95TabWindow::GetWindowCount()
{
	if ((unsigned)m_tabCtrl->GetCount() != tabbedWindowArray.GetItemsInContainer())
		TRACE("This is a problem--tabCtrl->GetCount() != tabbedWindowArray") ;
  return TTabWindow::GetWindowCount() ;
}


//DEFINE_RESPONSE_TABLE1(TWin95TabWindow, TTabWindow)

bool
TWin95TabWindow::Find(TEventInfo& eventInfo, TEqualOperator equal)
{
  eventInfo.Object = (GENERIC*) this ;
	return SearchEntries((TGenericTableEntry *)__entries, eventInfo, equal)
	  || TTabWindow::Find(eventInfo, equal) ;
}


//END_RESPONSE_TABLE;

void
TWin95TabWindow::SetupWindow()
{
  // If our W/H values are still 0, pull them from our parent
  if (Attr.W == 0 || Attr.W == CW_USEDEFAULT)
    Attr.W = Parent->GetClientRect().Width() ;
  if (Attr.H == 0 || Attr.H == CW_USEDEFAULT)
    Attr.H = Parent->GetClientRect().Height() ;

  // Create tab control, if not done already
  if (GetTabCtrl() == 0)
    m_tabCtrl = new TTabControl(this, TTabWindow::ID_TABCTRL, 0, 0, Attr.W, Attr.H) ;

  TTabWindow::SetupWindow() ;

  // Add the tabs & pages already in tabbedWindowArray
  TWindow *ptr ;
  uint i ;
  for (i = 0 ; i < tabbedWindowArray.GetItemsInContainer() ; i++)
  {
    ptr = tabbedWindowArray[i] ;

    // Reparent and create
    ptr->SetParent(this) ;
    if (!ptr->GetHandle())
      ptr->Create() ;
    // Retrieve window's text, use that for tab text
    TAPointer<_TCHAR> txt = new _TCHAR[ptr->GetWindowTextLength() + 1] ;
    ptr->GetWindowText(txt, ptr->GetWindowTextLength() + 1);
    m_tabCtrl->Add(TTabItem(txt));
    // Select new tab as current
    m_tabCtrl->SetSel(m_tabCtrl->GetCount() - 1) ;
    // Center and display associated TWindow*
    AdjustTabClient();
  }

  // Hide all windows but the first
  for (i = 1 ; i < tabbedWindowArray.GetItemsInContainer() ; i++)
    tabbedWindowArray[i]->Show(SW_HIDE) ;

  // Set to the first tab
  SetSelectedTab(0) ;
  m_tabCtrl->Show(SW_SHOW) ;
}


void
TWin95TabWindow::Attach(TWindow *ptr, LPCTSTR title)
{
  TTabWindow::Attach(ptr) ;

  if (m_tabCtrl)
  {
    // Retrieve window's text, use that for tab text
    TAPointer<_TCHAR> txt = new _TCHAR[ptr->GetWindowTextLength() + 1] ;
    ptr->GetWindowText(txt, ptr->GetWindowTextLength() + 1) ;
    m_tabCtrl->Add(TTabItem(txt)) ;
    // Select new tab as current
    SetSelectedTab(m_tabCtrl->GetCount() - 1) ;
    ptr->Show(SW_SHOW) ;
  }
}


TWindow
*TWin95TabWindow::Detach(int32 index)
{
  // Remove tab from tab control
  m_tabCtrl->Delete(index) ;

  return TTabWindow::Detach(index) ;
}


void
TWin95TabWindow::SetSelectedTab(int32 index)
{
  m_tabCtrl->SetSel(index) ;

  // TTabControl does not send the TCN_ messages, so we need to "fake" it
  TNotify dummy ;
  TabSelChanging(dummy) ;
  TabSelChange(dummy) ;
}


void
TWin95TabWindow::AdjustTabClient()
{
  // Retrieve pointer to tab control, the client window
  if (m_tabCtrl && m_tabCtrl->GetHandle())
  {
    // Retrieve the window rectangle of the tab control
    TRect rect;
    m_tabCtrl->GetWindowRect(rect);

    // NOTE: GetWindowRect returns screen coordinates... we'll need
    //       to have the coordinates relative to the frame window,
    //       the parent of both the tab and client window
    ::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rect, 2);

    // Ask the tab for it's 'client-area' based in it window location
    m_tabCtrl->AdjustRect(false, rect);

    // Move the Client window
    TWindow* Client = tabbedWindowArray[m_tabCtrl->GetSel()];
    if (Client && Client->GetHandle())
      Client->SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);
  }
}


void
TWin95TabWindow::EvSize(uint sizeType, TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
  if (m_tabCtrl && m_tabCtrl->GetHandle())
  {
    // Adjust tab control to new window size
    m_tabCtrl->SetWindowPos(0, 0, 0, size.cx, size.cy, SWP_NOZORDER | SWP_NOMOVE) ;
  }
  AdjustTabClient() ;
}


void
TWin95TabWindow::TabSelChange(TNotify far&)
{
  AdjustTabClient() ;
  // Here, GetSel() returns the after-the-click-current tab selected
  tabbedWindowArray[m_tabCtrl->GetSel()]->Show(SW_SHOW) ;
  tabbedWindowArray[m_tabCtrl->GetSel()]->SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW) ;
}


bool
TWin95TabWindow::TabSelChanging(TNotify far&)
{
  // Here, GetSel() returns the before-the-click-current tab selected
  if (tabbedWindowArray[m_tabCtrl->GetSel()])
    tabbedWindowArray[m_tabCtrl->GetSel()]->Show(SW_HIDE) ;
  return false ;
}


void
TWin95TabWindow::TabKeyDown(TTabKeyDown far&)
{
}
*/


// -----------------------------------------------------------------------------
// TNoteTabWindow
// -----------------------------------------------------------------------------
/*
DEFINE_RESPONSE_TABLE1(TNoteTabWindow, TWindow)
	EV_WM_SIZE,
	EV_TCN_SELCHANGE(TTabWindow::ID_TABCTRL,    TabSelChange),
	EV_TCN_SELCHANGING(TTabWindow::ID_TABCTRL,  TabSelChanging),
	EV_TCN_KEYDOWN(TTabWindow::ID_TABCTRL,      TabKeyDown),
END_RESPONSE_TABLE ;
*/

