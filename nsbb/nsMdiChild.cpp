// -----------------------------------------------------------------------------
// nsMdiChild.cpp
// -----------------------------------------------------------------------------

#include <owl\owlpch.h>#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include "nautilus\nssuper.h"
#include "nsbb\nsMdiChild.h"
#include "nsbb\nsPaneSplitter.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsWhoIsThere.h"

// -----------------------------------------------------------------------------
// Class NSMDIChildContent
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSMDIChildContent, TWindow)
	EV_WM_SIZE,
  EV_COMMAND(IDC_OK_PANESPLIT,     CmChildOK),
	EV_COMMAND(IDC_HELP_PANESPLIT,   CmChildHelp),
	EV_COMMAND(IDC_CANCEL_PANESPLIT, CmChildCancel),
	EV_COMMAND(CM_HELP,              CmChildHelp),
END_RESPONSE_TABLE;

NSMDIChildContent::NSMDIChildContent(NSContexte* pCtx, TWindow* parent)
                  :TWindow(parent), NSRoot(pCtx)
{
	_pSplitter = (TPaneSplitter*) 0 ;
	_pToolBar  = (TGadgetWindow*) 0 ;
}
voidNSMDIChildContent::EvSize(uint sizeType, ClassLib::TSize &size){	TWindow::EvSize(sizeType, size) ;
  if (((TGadgetWindow*) NULL == _pToolBar) &&
      ((NSPaneSplitter*) NULL == _pSplitter))
  	return ;

  NS_CLASSLIB::TRect clientRect = GetClientRect() ;

  if (_pToolBar)
	{
    int iPanelHeight = 0 ;

    NSGadgetWindow* pGadgetWindow = dynamic_cast<NSGadgetWindow*>(_pToolBar) ;
    if (pGadgetWindow)
      iPanelHeight = pGadgetWindow->getGadgetsHeight() ;
    else
    {
      NSInPersonsGadgetWindow* pIPGadgetWindow = dynamic_cast<NSInPersonsGadgetWindow*>(_pToolBar) ;
      if (pIPGadgetWindow)
        iPanelHeight = pIPGadgetWindow->getGadgetsHeight() ;
    }

    if (0 == iPanelHeight)
      _pSplitter->MoveWindow(clientRect, true) ;
    else
    {
      // Take care: The size fixed here is the gadget window's container
      //            dimension. The size of the "grey area" for controls
      //            will depend from gadgets size and shrink settings
      //
      NS_CLASSLIB::TRect barRect   = NS_CLASSLIB::TRect(clientRect.Left(), clientRect.Top(), clientRect.Right(), clientRect.Top() + iPanelHeight) ;
      _pToolBar->MoveWindow(barRect, true) ;

      NS_CLASSLIB::TRect splitRect = NS_CLASSLIB::TRect(clientRect.Left(), barRect.Bottom(), clientRect.Right(), clientRect.Bottom()) ;
      _pSplitter->MoveWindow(splitRect, true) ;
    }
  }
  else
  	_pSplitter->MoveWindow(clientRect, true) ;}
void
NSMDIChildContent::CmChildOK()
{
	if ((TPaneSplitter*) NULL == _pSplitter)
    return ;

  NSPaneSplitter* pSplitter = dynamic_cast<NSPaneSplitter*>(_pSplitter) ;
  if (pSplitter)
    pSplitter->CmOk() ;
}

void
NSMDIChildContent::CmChildHelp()
{
  if ((TPaneSplitter*) NULL == _pSplitter)
    return ;

  NSPaneSplitter* pSplitter = dynamic_cast<NSPaneSplitter*>(_pSplitter) ;
	if (pSplitter)
		pSplitter->CmHelp() ;
}

void
NSMDIChildContent::CmChildCancel()
{
  if ((TPaneSplitter*) NULL == _pSplitter)
    return ;

  NSPaneSplitter* pSplitter = dynamic_cast<NSPaneSplitter*>(_pSplitter) ;
	if (pSplitter)
		pSplitter->CmCancel() ;
}

// -----------------------------------------------------------------------------
// Class NSMDIChild
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSMDIChild, TMDIChild)
	// EV_WM_CLOSE,
	EV_WM_DESTROY,
	EV_WM_SETFOCUS,
	EV_WM_ACTIVATE,
	EV_WM_NCACTIVATE,
	EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE;


NSMDIChild::NSMDIChild(NSContexte* pCtx, TMDIClient& parent, const char far *title, TWindow *clientWnd, bool shrinkToClient, TModule *module)
           :TMDIChild(parent, title, clientWnd, shrinkToClient, module), NSRoot(pCtx)
{
	bShrinkToClient  = shrinkToClient ;
	client           = clientWnd ;
	_pWndCreatePanel = (TWindow*) 0 ;
	hWndToFocus      = 0 ;
  _bDestroyed      = false ;
}

NSMDIChild::NSMDIChild(NSContexte* pCtx, HWND hWnd, TModule *module)
           :TMDIChild(hWnd, module), NSRoot(pCtx)
{
	bShrinkToClient  = true ;
  client           = (TWindow*) 0 ;
	_pWndCreatePanel = (TWindow*) 0 ;
	hWndToFocus      = 0 ;
  _bDestroyed      = false ;
}

NSMDIChild::~NSMDIChild()
{
}

void
NSMDIChild::closeChildren()
{
  if ((TWindow*) NULL == _pWndCreatePanel)
    return ;

/*
  NSMUEView* pWndCreate = TYPESAFE_DOWNCAST(_pWndCreatePanel, NSMUEView) ;
  if (pWndCreate)
  {
    NSMDIChildContent* pChildContent = TYPESAFE_DOWNCAST(client, NSMDIChildContent) ;
    if (pChildContent)
    {
      NSPaneSplitter* pSplitter = pChildContent->getSplitter() ;
      if (pSplitter)
        pSplitter->closeAllWindows() ;
    }
  }
  else
  {
*/
    NSDialog* pWndCr = TYPESAFE_DOWNCAST(_pWndCreatePanel, NSDialog) ;
    if (pWndCr)
      pWndCr->Desactive() ;
//  }

  _pWndCreatePanel = (TWindow*) 0 ;
}

void
NSMDIChild::initClient(NSDialog *pDlg)
{
	TFrameWindow::Init(pDlg, bShrinkToClient) ;
}

void
NSMDIChild::initClientArchetype(TWindow *pView)
{
  TFrameWindow::Init(pView, bShrinkToClient) ;
}

void
NSMDIChild::split(TWindow* pMaster, TWindow* pSlave, TSplitDirection splitDir, float percent)
{
	NSMDIChildContent* pChildContent = TYPESAFE_DOWNCAST(client, NSMDIChildContent) ;
	if ((NSMDIChildContent*) NULL == pChildContent)
		return ;

	TPaneSplitter* pTSplitter = pChildContent->getSplitter() ;
  if ((TPaneSplitter*) NULL == pTSplitter)
    return ;

  NSPaneSplitter* pSplitter = dynamic_cast<NSPaneSplitter*>(pTSplitter) ;
  if ((NSPaneSplitter*) NULL == pSplitter)
		return ;

	pSplitter->nsSplitPane(pMaster, pSlave, splitDir, percent) ;
}

int MyEnumEachPane(TWindow &p, void*)
{
	ClassLib::TRect rectMother, rectChild ;
	// Trect rectChild;
	// NSMUEView* wndMother;
	TSplitDirection dir ;
	// NSPaneSplitter* pPanelSplitter;
	float percent = 1.0 ;
	// NSSplittingWindowProperty *wndProp = new  NSSplittingWindowProperty();

	NSMUEView* wndChild = TYPESAFE_DOWNCAST(&p, NSMUEView) ;
	if  (wndChild)
	{
		//pour tous les fenetre sauf la premiere qui a fait le MDIChild
		if (wndChild->getSplittedWindow())
		{
			NSMUEView* wndMother = TYPESAFE_DOWNCAST(wndChild->getSplittedWindow(), NSMUEView) ;
			NSPaneSplitter* pPanelSplitter = wndChild->getPaneSplitter() ;
			rectChild = pPanelSplitter->computeMetaRectWindow(wndChild) ;
			rectMother = pPanelSplitter->computeMetaRectWindow(wndMother) ;
			dir = wndChild->getSplitDirection();
			switch (dir)
			{
				case psHorizontal :
					percent = (1.0 - ((float)rectChild.Height())/((float)rectMother.Height() )) ;
					break ;
				case psVertical   :
					percent = (1.0 - ((float)rectChild.Width())/((float)rectMother.Width())) ;
					break ;
				case psNone       :
					percent = 1.0 ;
					break ;
			}

			//wndChild->getCreateWindow() la fenetre qui a créé le MDIChild
			NSMUEView* wndCreateChild = TYPESAFE_DOWNCAST(wndChild->getCreateWindow(), NSMUEView) ;
      if (wndCreateChild)
			{
				NSWindowProperty *pWinProp = wndCreateChild->pContexte->getUtilisateur()->getWindowProperty(wndCreateChild->getFunction()) ;
				if ((pWinProp) && (wndMother))
					pWinProp->addSplit(wndMother->getFunction(), wndChild->getFunction(), dir, percent) ;
    	}
		}
		return 1 ;
	}
	else
		return 1 ;
}

void
NSMDIChild::setCreateWindow(TWindow* pWnd)
{
  _pWndCreatePanel = pWnd ;

  if ((TWindow*) NULL == pWnd)
    return ;

  NSMUEView* pWndCreate = TYPESAFE_DOWNCAST(pWnd, NSMUEView) ;
  if (pWndCreate)
    pWndCreate->setCreatedMdiChild(this) ;
}

void
NSMDIChild::RecordChildPos()
{
	if (false == pContexte->getUtilisateur()->isEnregWin())
  	return ;

	NSWindowProperty wndProp ;

	ClassLib::TRect rect = GetWindowRect() ;
	wndProp.setX(rect.left) ;
	wndProp.setY(rect.top) ;
	wndProp.setW(rect.Width()) ;
	wndProp.setH(rect.Height()) ;
  wndProp.setActivity(NSWindowProperty::undefined) ;

	TWindow *pWnd = getCreateWindow() ;
	if (NULL == pWnd)
		return ;


	NSMUEView* wndCreate = TYPESAFE_DOWNCAST(pWnd, NSMUEView) ;
	if (wndCreate)
		wndProp.setFunction(wndCreate->getFunction()) ;
	else
	{
		NSDialog* wndCr = TYPESAFE_DOWNCAST(pWnd, NSDialog) ;
		if (wndCr)
		{
			TWindow* pWV = wndCr->_pView ;
			NSMUEView* pGenuineView = TYPESAFE_DOWNCAST(pWV, NSMUEView) ;
			if (pGenuineView)
				wndProp.setFunction(pGenuineView->getFunction()) ;
		}
	}
	//NSWindowProperty *pProp = pContexte->getUtilisateur()->aWinProp.getProperty(wndProp->sFunction) ;
	//  pProp->resetSplitList();

	string sUserId = pContexte->getUtilisateur()->getNss() ;

	pContexte->getUtilisateur()->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &wndProp, false) ;

	/* NSWindowProperty *pProp = pContexte->getUtilisateur()->aWinProp.getProperty(wndProp->sFunction) ;
	pProp->resetSplitList();   */

  NSMDIChildContent* pChildContent = TYPESAFE_DOWNCAST(client, NSMDIChildContent) ;
	if (pChildContent)
	{
    TPaneSplitter* pTSplitter = pChildContent->getSplitter() ;
    if (pTSplitter)
    {
      NSPaneSplitter* pSplitter = dynamic_cast<NSPaneSplitter*>(pTSplitter) ;
		  if (pSplitter)
			  pSplitter->ForEachPane(MyEnumEachPane,(void*) wndCreate) ;
    }
  }

	pContexte->getUtilisateur()->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &wndProp, true) ;
}

int
NSMDIChild::GetFrameIndex()
{
  return GetDocTitleIndex() ;
}

void
NSMDIChild::PerformCreate(int menuOrId)
{
  TMDIChild::PerformCreate(menuOrId) ;

/*
  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (NULL == pMyApp)
    return ;

  pMyApp->prendClient()->HandleMessage(WM_MDIREFRESHMENU) ;
  pMyApp->getFrame()->DrawMenuBar() ;
*/
}

void
NSMDIChild::EvDestroy()
{
  RecordChildPos() ;

  closeChildren() ;

  _bDestroyed = true ;
}

void
NSMDIChild::EvSetFocus(THandle hWndLostFocus)
{
	TMDIChild::EvSetFocus(hWndLostFocus) ;

  if ((GetHandle() == hWndLostFocus) || IsChild(hWndLostFocus))
		return ;

  // Les SetFocus viennent de la MDIClient
  // TMDIClient* pMDIClt = pContexte->getSuperviseur()->getApplication()->prendClient() ;
  // if (pMDIClt->GetHandle() == hWndLostFocus)
	// 	return ;

  NSMDIChildContent* pChildContent = TYPESAFE_DOWNCAST(client, NSMDIChildContent) ;
	if ((NSMDIChildContent*) NULL == pChildContent)
		return ;

  TPaneSplitter* pTSplitter = pChildContent->getSplitter() ;
  if ((TPaneSplitter*) NULL == pTSplitter)
    return ;

  NSPaneSplitter* pClientSplit = dynamic_cast<NSPaneSplitter*>(pTSplitter) ;
	if (pClientSplit)
	{
    if (!hWndToFocus)
    {
		  if (pClientSplit->_pCurrentFocusedView)
    	  // pClientSplit->pCurrentFocusedView->SetFocus() ;
			  pClientSplit->_pCurrentFocusedView->EvSetFocus(hWndLostFocus) ;
		  else
			  pClientSplit->setFocusToNextView(0, hWndLostFocus) ;

		  if (pClientSplit->_pCurrentFocusedView)
    	  hWndToFocus = pClientSplit->_pCurrentFocusedView->GetHandle() ;
      else
    	  hWndToFocus = 0 ;

  	  // Action spécifique pour les CQVues
      // Specific action for NSCQVue : set focus to the dialog inside
      // Warning : it may iterate endlessly and not allow to select another view

/* TODO Disconnected when switching nsMdiChild to nsmbb.dll looking for a solution

  	  if (pClientSplit->_pCurrentFocusedView && (NULL == pInitialFocusedView))
      {
  		  NSCQVue* pCQfocused = dynamic_cast<NSCQVue*>(pClientSplit->_pCurrentFocusedView) ;
  		  if (pCQfocused && pCQfocused->clientWin)
      	  hWndToFocus = pCQfocused->clientWin->GetHandle() ;
      	  // HoldFocusHWnd(hWndLostFocus, pCQfocused->clientWin->GetHandle()) ;
      	  // if ((pCQfocused->clientWin->GetHandle() != hWndLostFocus) && (!(pCQfocused->clientWin->IsChild(hWndLostFocus))))
				  //	 pCQfocused->clientWin->EvSetFocus(hWndLostFocus) ;
		  }
*/
    }
    else
    {
      TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
	    if (pMyApp)
      {
		    pMyApp->FlushControlBar() ;
        pMyApp->getSecondaryControlBar()->LayoutSession() ;
      }
    }
	}
  // if (hWndToFocus)
	// 	::SetFocus(hWndToFocus) ;
}

void
NSMDIChild::EvActivate(uint active, bool minimized, HWND hwnd)
{
	TMDIChild::EvActivate(active, minimized, hwnd) ;
}

bool
NSMDIChild::EvNCActivate(bool active)
{
	return TMDIChild::EvNCActivate(active) ;
}

bool
NSMDIChild::PreProcessMsg(MSG& msg)
{
  return TMDIChild::PreProcessMsg(msg) ;
}

bool
NSMDIChild::ShowWindow(int cmdShow)
{
  return TMDIChild::ShowWindow(cmdShow) ;
}

bool
NSMDIChild::EnableWindow(bool enable)
{
  return TMDIChild::EnableWindow(enable) ;
}

void
NSMDIChild::Destroy(int retVal)
{
  // TMDIChild::Destroy asks MDIClient to send a DESTROY message to this Child
  //
  // Parent->HandleMessage(WM_MDIDESTROY, TParam1(GetHandle()));
  // SetHandle(0);  // Assume success

  TMDIChild::Destroy(retVal) ;
}

void
NSMDIChild::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TMDIChild::EvLButtonDown(modKeys, point) ;
}

void
NSMDIChild::EvParentNotify(uint event, uint childHandleOrX, uint childIDOrY)
{
	if (event == WM_LBUTTONDOWN)
	{
	}
  TMDIChild::EvParentNotify(event, childHandleOrX, childIDOrY) ;
}

bool
NSMDIChild::SetDocTitle(const char far* docname, int index)
{
	NSMDIChildContent* pChildContent = TYPESAFE_DOWNCAST(client, NSMDIChildContent) ;
	if ((NSMDIChildContent*) NULL == pChildContent)
  {
  	if (docname && ('\0' != docname[0]))
			return TFrameWindow::SetDocTitle(docname, index) ;
    else
			return true ;
  }

	// SetDocTitle puts document's name as view caption
  // If the view has a viewName, it means it does it itself (in SetFocus)
	//
  NSPaneSplitter* pClientSplit = (NSPaneSplitter*) 0 ;

  TPaneSplitter* pTSplitter = pChildContent->getSplitter() ;
  if (pTSplitter)
    pClientSplit = dynamic_cast<NSPaneSplitter*>(pTSplitter) ;

	if (((NSPaneSplitter*) NULL == pClientSplit) ||
      (NULL == pClientSplit->_pCurrentFocusedView) ||
      (pClientSplit->_pCurrentFocusedView->getViewName() == ""))
	{
    // If document's name exists, we let OWL work
    //
  	if (docname && (docname[0] != '\0'))
			return TFrameWindow::SetDocTitle(docname, index) ;

    // If we have got an Archetype, we paste dialog's title in MDI title
    //
    if (pClientSplit && pClientSplit->_pCurrentFocusedView)
		{
/* TODO Disconnected when switching nsMdiChild to nsmbb.dll looking for a solution
			NSCQVue* pCQfocused = dynamic_cast<NSCQVue*>(pClientSplit->_pCurrentFocusedView) ;
			if (pCQfocused && pCQfocused->clientWin)
			{
    		char szDialogTitle[256] ;
    		int iLen = pCQfocused->clientWin->GetWindowText(szDialogTitle, 255) ;
      	if (iLen > 0)
    			SetWindowText(szDialogTitle) ;
			}
*/
		}
	}
  else
  {
    string sViewName = pClientSplit->_pCurrentFocusedView->getViewName() ;
    SetWindowText(sViewName.c_str()) ;
  }

  return true ;

/*
	if (index >= 0) {
    string title;

    if (Title && *Title) {
      title = Title;
      title += " - ";
    }

    if (docname)
      title += docname;

    if (index > 0) {
      title += ':';
      char num[10];
      itoa(index, num, 10);
      title += num;
    }

    SetWindowText(title.c_str());
  }  // else if index negative, simply acknowledge that title will display

  return true ;
*/
}

NS_CLASSLIB::TRect
NSMDIChild::GetRectInMdiClient()
{
  TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if ((TMyApp*) NULL == pMyApp)    return NS_CLASSLIB::TRect(0, 0 ,0, 0) ;  NSMDIClient* pMdiClient = pMyApp->prendClient() ;  if ((NSMDIClient*) NULL == pMdiClient)    return NS_CLASSLIB::TRect(0, 0 ,0, 0) ;
  NS_CLASSLIB::TRect clientRect = pMdiClient->GetWindowRect() ;
  NS_CLASSLIB::TRect thisRect   = GetWindowRect() ;

  return NS_CLASSLIB::TRect(thisRect.X() - clientRect.X(),
                            thisRect.Y() - clientRect.Y(),
                            thisRect.X() + thisRect.Width(),
                            thisRect.Y() + thisRect.Height()) ;
}

// -----------------------------------------------------------------------------
// Class NSBasicMDIClient
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSBasicMDIClient, TMDIClient)
END_RESPONSE_TABLE;


NSBasicMDIClient::NSBasicMDIClient(TModule* module)
                 :OWL::TMDIClient(module)
{
}

NSBasicMDIClient::~NSBasicMDIClient()
{
}

static void
sCloseMdiChildIfDestroyed(TWindow* win, void*)
{
  if (win)
  {
    NSMDIChild *pMdiChild = TYPESAFE_DOWNCAST(win, NSMDIChild) ;
    if (pMdiChild && pMdiChild->IsDestroyed())
  	  delete pMdiChild ;
  }
}

void
NSBasicMDIClient::MdiChildrenGarbageCollector()
{
  ForEach(sCloseMdiChildIfDestroyed) ;
}

static void
sGetAvailableHForRect(TWindow* win, void* pParam)
{
  NS_CLASSLIB::TRect *pRect = static_cast<NS_CLASSLIB::TRect*>(pParam) ;
  if ((NS_CLASSLIB::TRect*) NULL == pRect)
    return ;

  if (NS_CLASSLIB::TRect(0, 0, 0, 0) == *pRect)
    return ;

  NSMDIChild *pMdiChild = TYPESAFE_DOWNCAST(win, NSMDIChild) ;
  if ((NSMDIChild*) NULL == pMdiChild)
    return ;

  NS_CLASSLIB::TRect mdiRect = pMdiChild->GetRectInMdiClient() ;

  // Rectangles don't touch each other -> no interaction
  //
  if (false == pRect->Touches(mdiRect))
    return ;

  // This window contains rect -> rect is null
  //
  if (mdiRect.Contains(*pRect))
  {
    *pRect = NS_CLASSLIB::TRect(0, 0, 0, 0) ;
    return ;
  }

  // rect contains this window or rect is under this window -> rect goes under
  //
  if (pRect->Contains(mdiRect) || (pRect->Top() < mdiRect.Top()))
  {
    *pRect = NS_CLASSLIB::TRect(pRect->Left(), mdiRect.Bottom(), pRect->Right(), pRect->Bottom()) ;
    return ;
  }

  // rect is above this window
  //
  if (pRect->Top() < mdiRect.Top())
  {
    *pRect = NS_CLASSLIB::TRect(pRect->Left(), pRect->Top(), pRect->Right(), mdiRect.Top()) ;
    return ;
  }
}

void
NSBasicMDIClient::getAvailableHForRect(NS_CLASSLIB::TRect& rect) const
{
  ForEach(sGetAvailableHForRect, &rect) ;
}

static void
sGetAvailableWForRect(TWindow* win, void* pParam)
{
  NS_CLASSLIB::TRect *pRect = static_cast<NS_CLASSLIB::TRect*>(pParam) ;
  if ((NS_CLASSLIB::TRect*) NULL == pRect)
    return ;

  if (NS_CLASSLIB::TRect(0, 0, 0, 0) == *pRect)
    return ;

  NSMDIChild *pMdiChild = TYPESAFE_DOWNCAST(win, NSMDIChild) ;
  if ((NSMDIChild*) NULL == pMdiChild)
    return ;

  NS_CLASSLIB::TRect mdiRect = pMdiChild->GetRectInMdiClient() ;

  // Rectangles don't touch each other -> no interaction
  //
  if (false == pRect->Touches(mdiRect))
    return ;

  // This window contains rect -> rect is null
  //
  if (mdiRect.Contains(*pRect))
  {
    *pRect = NS_CLASSLIB::TRect(0, 0, 0, 0) ;
    return ;
  }

  // rect contains this window or rect on the right of this window -> rect goes to the right
  //
  if (pRect->Contains(mdiRect) || (pRect->Right() > mdiRect.Right()))
  {
    *pRect = NS_CLASSLIB::TRect(mdiRect.Right(), pRect->Top(), pRect->Right(), pRect->Bottom()) ;
    return ;
  }

  // rect is on the left of this window
  //
  if (pRect->Left() < mdiRect.Left())
  {
    *pRect = NS_CLASSLIB::TRect(pRect->Left(), pRect->Top(), mdiRect.Left(), pRect->Bottom()) ;
    return ;
  }
}

void
NSBasicMDIClient::getAvailableWForRect(NS_CLASSLIB::TRect& rect) const
{
  ForEach(sGetAvailableWForRect, &rect) ;
}

// -----------------------------------------------------------------------------
// end nsMdiChild.cpp
// -----------------------------------------------------------------------------

