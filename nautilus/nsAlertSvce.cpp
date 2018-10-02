// NSMailSvce.CPP : Incoming messages processing
////////////////////////////////////////////////////////////
// #include <cstring.h>
// #include <stdio.h>// #include <assert.h>// #include <vcl/controls.hpp>// #include <commctrl.h>// #include <owl/treewind.h>
// #include <vcl.h>// #pragma hdrstop// #include "ActiveX.h"

#include "nssavoir\nspathor.h"
#include "nautilus\nsAlertSvce.h"
#include "nsoutil\nsAlertSvceTools.h"
#include "nautilus\nssuper.h"
#include "nsbb\nslistwind.h"#include "nautilus\nsepicap.h"#include "nautilus\ns_html.h"#include "nautilus\nsdocview.h"#include "nautilus\nsbrowse.h"#include "nssavoir\nsFileCaptur.h"#include "nssavoir\nspathor.h"#include "nsldv\nsldvuti.h"#include "partage\nsdivfile.h"#include "nsldv\nsldvdrug.h"// #include "nautilus\psapi.h"#include "nsoutil\nsAlertBox.rh" /****************** classe NSImportWindow **************************/
const int ID_AlertList = 0x100 ;
const int ID_AlertText = 0x101 ;
const int ID_AlertHtml = 0x102 ;

/*
DEFINE_RESPONSE_TABLE1(NSMailServiceWindow, TPaneSplitter)
   EV_WM_CLOSE,
   EV_WM_SIZE,
END_RESPONSE_TABLE;
*/

NSAlertServiceWindow::NSAlertServiceWindow(NSContexte* pCtx, NSPrescriptionCheckingMessage* pCheckInfo, ArrayCopyDrugs* pDrugs, TWindow* parent, const char far* title, TModule* module)
                     :TPaneSplitter(parent, title, 0, module),
                      NSRoot(pCtx)
{
	Attr.Style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN) ;

  if (pCheckInfo)
    _CheckInfo = *pCheckInfo ,

  loadDrugs(pDrugs) ;

	_pAlertListWindow     = new NSAlertListWindow(pCtx, this, ID_AlertList, 0, 0, 0, 0) ;
  _pAlertElementsWindow = new NSAlertElementsViewerWindow(pCtx, this, ID_AlertList, 0, 0, 0, 0) ;
  _pAlertTextWindow     = new NSTxtAlertViewerWindow(pCtx, this, ID_AlertText, 0, 0, 0, 0) ;
  _pAlertHtmlWindow     = (NSHtmlAlertViewerWindow*) 0 ;

  _pCurrentViewer       = _pAlertTextWindow ;

	_pAlertViewWindow     = (NSWebServiceWindow*) 0 ;
}

NSAlertServiceWindow::~NSAlertServiceWindow()
{
  // Delete components
  //
	delete _pAlertListWindow ;
  delete _pAlertTextWindow ;
  delete _pAlertElementsWindow ;
  if (_pAlertHtmlWindow)
    delete _pAlertHtmlWindow ;
	// delete _pAlertViewWindow ;

  // Update the context
  //
	if (pContexte->_pAlertBoxWindow == this)
		pContexte->_pAlertBoxWindow = (NSAlertServiceWindow*) 0 ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSAlertServiceWindow::SetupWindow()
{
  string sCaptionText = pContexte->getSuperviseur()->getText("drugAlertWindow", "prescriptionAlerts") ;
  Parent->SetCaption(sCaptionText.c_str()) ;

  TPaneSplitter::SetupWindow() ;

  // splitDir = psVertical, psHorizontal	or psNone
  //
  SplitPane(_pAlertElementsWindow, _pAlertListWindow, psVertical) ;
  SetSplitterWidth(5) ;

	SplitPane(_pAlertListWindow, _pAlertTextWindow, psVertical) ;
  SetSplitterWidth(5) ;

  // Install the HTMK viewer
  //
  _pAlertHtmlWindow = new NSHtmlAlertViewerWindow(pContexte, this, ID_AlertHtml, 0, 0, 0, 0) ;
  _pAlertHtmlWindow->Create() ;
  setHtmlWindowPos() ;

  // Fill the alert elements display structure
  //
  _pAlertElementsWindow->displayInformation(&_CheckInfo) ;
}

void
NSAlertServiceWindow::EvClose()
{
	TWindow::EvClose() ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindow
void
NSAlertServiceWindow::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TPaneSplitter::EvSize(sizeType, size) ;
  setHtmlWindowPos() ;
}

/**
 * Initialize drug array from a model
 */
void
NSAlertServiceWindow::loadDrugs(ArrayCopyDrugs* pDrugs)
{
  _aDrugs.vider() ;

  if (((ArrayCopyDrugs*) NULL == pDrugs) || pDrugs->empty())
    return ;

  _aDrugs = *pDrugs ;
}

void
NSAlertServiceWindow::reloadElements(const NSPrescriptionCheckingMessage* pCheckInfo)
{
  _pAlertElementsWindow->displayInformation(pCheckInfo) ;
}

void
NSAlertServiceWindow::reloadDrugs(ArrayCopyDrugs* pDrugs)
{
  loadDrugs(pDrugs) ;
  
  _pAlertListWindow->Reload() ;
}

void
NSAlertServiceWindow::displayHtml(const string sAlertContent, const string sAlertDetail)
{
  if ((NSHtmlAlertViewerWindow*) NULL == _pAlertHtmlWindow)
    return ;

  // creating a temporary html file in the SHTM directory
  //
  string sPathHtml     = pContexte->PathName("SHTM") ;
  string sFullHtmlName = sPathHtml + string("bdmAlert.htm") ;

  ofstream outFile ;
	outFile.open(sFullHtmlName.c_str()) ;
	if (!outFile)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFullHtmlName ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;

    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    return ;
	}

  outFile << string("<body><h3>") + sAlertContent + string("</h3>") + sAlertDetail + string("</body>") ;

  outFile.close() ;

  _pAlertHtmlWindow->display(sFullHtmlName) ;
}

void
NSAlertServiceWindow::displayTxt(const string sAlertContent, const string sAlertDetail)
{
  openTextViewer() ;
  _pAlertTextWindow->displayText((string*) &sAlertContent, (string*) &sAlertDetail) ;
}

void
NSAlertServiceWindow::setHtmlWindowPos()
{
  // return ;

  NS_CLASSLIB::TRect rectDlg = _pAlertTextWindow->GetWindowRect() ; // coordonnées par % à l'écran
  int X, Y ;
  // int W, H ;
  NS_CLASSLIB::TPoint pointTopLeft(rectDlg.X(), rectDlg.Y()) ;
  ScreenToClient(pointTopLeft) ;
  NS_CLASSLIB::TPoint pointBottomRight(rectDlg.X() + rectDlg.Width(), rectDlg.Y() + rectDlg.Height()) ;
  ScreenToClient(pointBottomRight) ;

  NS_CLASSLIB::TRect localRect(pointTopLeft, pointBottomRight) ;

  _pAlertHtmlWindow->SetWindowPos(0, localRect, SWP_NOZORDER) ;
}

void
NSAlertServiceWindow::openTextViewer()
{
  if ((_pCurrentViewer == _pAlertTextWindow) || (NULL == _pAlertHtmlWindow))
    return ;

  _pAlertHtmlWindow->ShowWindow(SW_HIDE) ;
  _pAlertTextWindow->ShowWindow(SW_SHOW) ;

  bool bReplaced = ReplacePane(_pAlertHtmlWindow, _pAlertTextWindow, TShouldDelete::NoDelete) ;
  if (true == bReplaced)
    _pCurrentViewer = _pAlertTextWindow ;
}

void
NSAlertServiceWindow::openHtmlViewer()
{
  if (_pCurrentViewer == _pAlertHtmlWindow)
    return ;

  setHtmlWindowPos() ;

  _pAlertTextWindow->ShowWindow(SW_HIDE) ;
  _pAlertHtmlWindow->ShowWindow(SW_SHOW) ;

  bool bReplaced = ReplacePane(_pAlertTextWindow, _pAlertHtmlWindow, TShouldDelete::NoDelete) ;
  if (true == bReplaced)
    _pCurrentViewer = _pAlertHtmlWindow ;
}

#define SPLITTER(x) TYPESAFE_DOWNCAST(x,OWL::TSplitter)
#define LAYOUTWINDOW(x) TYPESAFE_DOWNCAST(x,OWL::TLayoutWindow)

bool
NSAlertServiceWindow::ReplacePane(TWindow* target, TWindow* newPane,
                           TPaneSplitter::TDelete dt)
{
/*
  TSplitter* split1 = TYPESAFE_DOWNCAST(target->Parent, TSplitter) ;
  // OWL::TSplitter* split1 = SPLITTER(target->Parent) ;
  if (NULL == split1)
    return false ;

  OWL::TSplitter* split2 = SPLITTER(newPane->Parent) ;
  if (NULL != split2)
    return false ;
*/
  // if (!HasPane(target) || HasPane(newPane)) // 'newPane' should not exist.
  //  return false;

  // if (target->CanClose()) {     // 'newPane' takes on target's layout metrics.
    TLayoutMetrics  lmOfTarget ;
    TLayoutMetrics  lmOfOther ;
    TLayoutWindow*  splitter = LAYOUTWINDOW(target->Parent) ;
    TWindow*        pane1 = splitter->GetFirstChild() ;
    TWindow*        pane2 = (TWindow*) 0 ;

    if (SPLITTER(splitter))
    {
      pane1 = SPLITTER(splitter)->Pane1() ;
      pane2 = SPLITTER(splitter)->Pane2() ;
    }
    splitter->GetChildLayoutMetrics(*target, lmOfTarget) ;

    if ((pane1 == target) && pane2)
    {
      splitter->GetChildLayoutMetrics(*pane2, lmOfOther) ;
      if (lmOfOther.X.RelWin == target)
        lmOfOther.X.RelWin = newPane ;
      else
        lmOfOther.Y.RelWin = newPane ;
      splitter->SetChildLayoutMetrics(*pane2, lmOfOther) ;
    }
    newPane->SetParent(splitter);
    // newPane->Create();
    splitter->SetChildLayoutMetrics(*newPane, lmOfTarget);
    splitter->RemoveChildLayoutMetrics(*target);
    //DestroyPane(target, dt);
    target->SetParent(this);
    splitter->Layout();
    return true;
  // }
  // return false;
}

/****************** classe NSImportChild **************************/
DEFINE_RESPONSE_TABLE1(NSAlertServiceChild, TMDIChild)
  EV_WM_CLOSE,
  EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;

NSAlertServiceChild::NSAlertServiceChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSAlertServiceWindow* clientWnd)					          :TMDIChild(parent, title, clientWnd), NSRoot(pCtx)
{
	pClient = clientWnd ;
}

NSAlertServiceChild::~NSAlertServiceChild()
{
	if (pContexte->_pAlertBoxChild == this)
		pContexte->_pAlertBoxChild = (NSAlertServiceChild*) 0 ;
}

voidNSAlertServiceChild::EvClose()
{
/*
	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
	int retVal = MessageBox("Voulez-vous vraiment fermer ?", sCaption.c_str(), MB_YESNO) ;

	if (retVal == IDYES)
		TWindow::EvClose() ;
*/
	TWindow::EvClose() ;
}


////////////////////////// fin de nsMailSvce.cpp ///////////////////////////////
