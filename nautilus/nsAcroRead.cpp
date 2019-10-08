//---------------------------------------------------------------------------// #include "stdafx.h"

#include <vcl.h>
#pragma hdrstop

#include <dstring.h>
#include "AcroReader.h"
#include "nautilus\ActiveX.h"

#include "nautilus\nsAcroRead.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
#include "partage\ole_utils.h"
#include "nautilus\nautilus.rh" 	// pour l'id de commande CM_IMPRIME (et CM_EDITER)
#include "nautilus\nshistdo.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nsvisual.h"
#include "nsbb\nsattvaltools.h"
#include "nsoutil\nsoutil.h"

#define  ID_PDF_TIMER 100

typedef enum BrowserNavConstants {
    navOpenInNewWindow = 1,
    navNoHistory = 2,
    navNoReadFromCache = 4,
    navNoWriteToCache = 8
} BrowserNavConstants;

// Table de réponses de la classe NSVisualView////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE1(NSAcrobatView, TWindowView)
  EV_WM_CLOSE,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_WM_KEYDOWN,
  EV_WM_RBUTTONDOWN,
  EV_WM_SIZE,
  EV_WM_TIMER,
  EV_COMMAND(CM_ENREGISTRE,	CmFileSave),
  EV_COMMAND(CM_IMPRIME,		CmPublier),
END_RESPONSE_TABLE;

// Constructeur NSAcrobatView
////////////////////////////////////////////////////////////////

NSAcrobatView::NSAcrobatView(NSAcrobatDocument& doc, TWindow *parent)							:TWindowView(doc, parent), _pDoc(&doc){
  _AcrobatForm = (TAcrobatProxy*) 0 ;
  _IEForm      = (TWebProxy*) 0 ;

  _pMUEViewMenu = (TMenuDescr*) 0 ;
	nsMenuIniter menuIter(_pDoc->pContexte) ;
  _pMUEViewMenu = new OWL::TMenuDescr ;
  menuIter.initMenuDescr(_pMUEViewMenu, "menubar") ;

  TMyApp* pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->GetMainWindow()->SetMenuDescr(*_pMUEViewMenu) ;

	_bSetupToolBar = true ;
}

// Destructeur NSVisualView////////////////////////////////////////////////////////////////

NSAcrobatView::~NSAcrobatView(){
  string sTrace = string("NSAcrobatView Entering destructor") ;
  _pDoc->pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	if (_pMUEViewMenu)
  	delete _pMUEViewMenu ;

	// Delete de la Form
  if (_AcrobatForm)
	  delete _AcrobatForm ;
  if (_IEForm)
	  delete _IEForm ;
	//CoUninitialize();
	//OleUninitialize() ;
}

// GetWindow renvoie this////////////////////////////////////////////////////////////////

TWindow*NSAcrobatView::GetWindow()
{
	return (TWindow*) this ;
}

// Fonction CanClose pour détruire la barre d'outils
////////////////////////////////////////////////////////////////

boolNSAcrobatView::CanClose()
{
  NSSuper* pSuper = _pDoc->pContexte->getSuperviseur() ;

  string sTrace = string("NSAcrobatView CanClose") ;
  pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

	TMyApp* pMyApp = pSuper->getApplication() ;
	pMyApp->FlushControlBar() ;

	_bSetupToolBar = false ;

  if (pSuper->mustKillPdfProcess())
  {
    sTrace = string("NSAcrobatView CanClose: killing the PDF control.") ;
    _pDoc->pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;


    if (false == pSuper->mustUseIeForPdf())
    {
      HWND hOleCtrlWin = GetOleControlWindow() ;
      if (hOleCtrlWin)
        TerminateProcess(hOleCtrlWin) ;

      TAcroPDF* pPdfControl = _AcrobatForm->Control ;
      if (pPdfControl)
        pPdfControl->Parent = (TWinControl*) 0 ;
    }
    else
    {
      HWND hOleCtrlWin = GetSonWindow(GetHandle(), 3) ;
      if (hOleCtrlWin)
        TerminateProcess(hOleCtrlWin) ;

      // TWebBrowser* pIEControl = _IEForm->Control ;
      // if (pIEControl)
      //   pIEControl->Parent = (TWinControl*) 0 ;
    }
  }

	return TWindow::CanClose() ;}

//
// Override a TWindow virtual to create the HWND directly.
// NSVisualView and the VCL TForm1 class both end up
// wrapping the same HWND.
//

voidNSAcrobatView::PerformCreate(int /*menuOrId*/)
{
  NSSuper *pSuper = _pDoc->pContexte->getSuperviseur() ;

  string sFileToOpen = getFileName() ;
try
{

  if (false == pSuper->mustUseIeForPdf())
  {
    string sTrace = string("NSAcrobatView::PerformCreate Entering, using Acrobat Reader") ;
    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

    /********************************************************************
    PRECONDITION(!Form);

    // create the actual VCL form object
    //
    CoInitialize(NULL);
    Form = new TWebProxy(Parent->GetHandle(), this);
    if (Form) {
      CHECK(Form->Handle);

      // Give the OWL TWindow object the handle of the Windows
      // object it aliases
      //
      SetHandle(Form->Handle);
    }
    *****************************************************************************/

    //  USING ACROBAT DLL

    // CoInitialize(NULL) ;    // OleInitialize(NULL) ;    // Use OleInitialize to get clipboard functionality
    // on crée la Form pour servir de zone client (on lui passe le handle parent)
    // The Form is created as the client (it receaves the Parent handle)
    //

    _AcrobatForm = new TAcrobatProxy(Parent->GetHandle(), this) ;
    _AcrobatForm->Visible = true ;
    // _AcrobatForm->ParentWindow = Parent->HWindow ;

    // Give the OWL TWindow object the handle of the Windows object it aliases
    //
    SetHandle(_AcrobatForm->Handle) ;    // BCB Help: Some controls (such as ActiveX controls) are contained in    // non-VCL windows rather than in a parent control. For these controls,    // the value of Parent is NULL and the ParentWindow property specifies    // the non-VCL parent window.    //    // ::SetParent(Forms::Application->Handle, _pDoc->pContexte->GetMainWindow()->HWindow) ;
    /*    if (string("") != sFileToOpen)  	  displayFile(sFileToOpen) ;    */    TAcroPDF* pPdfControl = _AcrobatForm->Control ;    if (pPdfControl)
    {      TVariant pdfVersion = pPdfControl->GetVersions() ;      string sVersion = BSTRtoSTLstring(pdfVersion) ;
      sTrace = string("NSAcrobatView::PerformCreate: AcrobatReader version = ") + sVersion ;
      pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;    }    sTrace = string("NSAcrobatView::PerformCreate Leaving") ;    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;    return ;  }  // ****************** USING IE  //  string sTrace = string("NSAcrobatView::PerformCreate Entering, using Internet Explorer") ;  pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;	//CoInitialize(NULL) ;	_IEForm = new TWebProxy(Parent->GetHandle(), 0) ;

  // TWebProxy* _IEForm = ShowActiveXForm(pSuper->getApplication()->_pFormsApplication, Parent->HWindow) ;

	_IEForm->Visible = false ;
	_IEForm->ParentWindow = Parent->HWindow ;
	SetHandle(_IEForm->Handle) ;
	::SetParent(Forms::Application->Handle, _pDoc->pContexte->GetMainWindow()->HWindow) ;

	// on navigue vers le fichier
  //
  if (string("") != sFileToOpen)
    displayFile(sFileToOpen) ;  sTrace = string("NSAcrobatView::PerformCreate Leaving") ;  pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;}catch (...)
{
  string sTrace = string("Exception NSAcrobatView::PerformCreate") ;
  pSuper->trace(&sTrace, 1, NSSuper::trError) ;

	erreur("Exception NSAcrobatView::PerformCreate.", standardError, 0) ;
}}

void
NSAcrobatView::displayFile(string sFileName)
{
  NSSuper *pSuper = _pDoc->pContexte->getSuperviseur() ;

  if (string("") == sFileName)
  {
    string ps = string("NSAcrobatView::displayFile with empty file name") ;
	  pSuper->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

try
{
  string sTrace = string("NSAcrobatView::displayFile for file \"") + sFileName + string("\"") ;
  pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

	wchar_t buff[1024] ;
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sFileName.c_str(), -1, buff, sizeof(buff)) ;

  // ******************** USING ACROBAT DLL
  //

  if (false == pSuper->mustUseIeForPdf())
  {
    TAcroPDF* pPdfControl = _AcrobatForm->Control ;
    if ((TAcroPDF*) NULL == pPdfControl)
    {
      string sTrace = string("NSAcrobatView::displayFile for file \"") + sFileName + string("\". Null Acrobat control, leaving.") ;
      pSuper->trace(&sTrace, 1, NSSuper::trError) ;
      return ;
    }

    // View mode
    //
    // 'Fit'   — Fits the entire page within the window both vertically and horizontally.
    // 'FitH'  — Fits the entire width of the page within the window.
    // 'FitV'  — Fits the entire height of the page within the window.
    // 'FitB'  — Fits the bounding box within the window both vertically and horizontally.
    // 'FitBH' — Fits the width of the bounding box within the window.
    // 'FitB'  — Fits the height of the bounding box within the window
    //
    wchar_t wMode[1024] ;
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "Fit", -1, wMode, sizeof(wMode)) ;
    pPdfControl->setView(wMode) ;

    TOLEBOOL bResult = pPdfControl->LoadFile(buff) ;
    if (bResult == false)
    {
      string sTrace = string("NSAcrobatView::displayFile: Acrobat failed to load file \"") + sFileName + string("\"") ;
      pSuper->trace(&sTrace, 1, NSSuper::trError) ;
      return ;
    }
    else if (bResult == true)
    {
      string sTrace = string("NSAcrobatView::displayFile: Acrobat properly loaded file \"") + sFileName + string("\"") ;
      pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;
      return ;
    }
  }
  // ****************** USING IE
  //
  else
  {

	  Variant  Flags(navNoReadFromCache) ;
	  TVariant VFlags = Flags.operator TVariant() ;

	  _IEForm->Control->Navigate(buff, &VFlags) ;
  }
}
catch (Exception &ex)
{
  string sTrace = string("Exception NSAcrobatView::displayFile") + string(ex.Message.c_str()) ;
  pSuper->trace(&sTrace, 1, NSSuper::trError) ;
	// erreur(sTrace.c_str(), standardError, 0) ;
}
catch (...)
{
  string sTrace = string("Exception NSAcrobatView::displayFile") ;
  pSuper->trace(&sTrace, 1, NSSuper::trError) ;
	// erreur(sTrace.c_str(), standardError, 0) ;
}
}

void
NSAcrobatView::NavigateErrorEvent(int iStatusCode, string sURL)
{
	string ps = string("NSAcrobatView: NavigateError for URL \"") + sURL + string("\"") ;
  ps += string(" (") + getNavigateErrorShortMsg(iStatusCode) + string(")") ;
	_pDoc->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  erreur(ps.c_str(), standardError, 0) ;
}

// Fonction SetupWindow////////////////////////////////////////////////////////////////

voidNSAcrobatView::SetupWindow()
{
	TWindowView::SetupWindow() ;

  NSSuper *pSuper = _pDoc->pContexte->getSuperviseur() ;

  int X = 0 ;
  int Y = 0 ;

  int W = 0 ;
  int H = 0 ;

  if (false == pSuper->mustUseIeForPdf())
  {
	  W = _AcrobatForm->Width ;
    H = _AcrobatForm->Height ;
  }
  else
  {
    W = _IEForm->Width ;
    H = _IEForm->Height ;
  }

  Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
	ModifyStyle(WS_BORDER, WS_CHILD) ;
  NS_CLASSLIB::TRect clientRect = GetClientRect() ;  if (false == pSuper->mustUseIeForPdf())  {    TAcroPDF* pPdfControl = _AcrobatForm->Control ;    if ((TAcroPDF*) NULL == pPdfControl)
    {
      string sTrace = string("NSAcrobatView::SetupWindow for file. Null Acrobat control, leaving.") ;
      pSuper->trace(&sTrace, 1, NSSuper::trError) ;
      return ;
    }    /*    pPdfControl->Left   = 0 ;    pPdfControl->Top    = 0 ;
    pPdfControl->Width  = clientRect.Width() - 5 ;
    pPdfControl->Height = clientRect.Height() - 5 ;
    pPdfControl->setViewRect(pPdfControl->Left, pPdfControl->Top, pPdfControl->Width, pPdfControl->Height) ;
    */

    // Form->Control->setViewRect(0, 0, r.Width() - 10, r.Height() - 10) ;
    string sTrace = string("NSAcrobatView::SetupWindow, showing Acrobat Reader Form") ;    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;    pPdfControl->setShowToolbar(false) ;    string sFileToOpen = getFileName() ;    if (string("") != sFileToOpen)      displayFile(sFileToOpen) ;    _AcrobatForm->Show() ;    pPdfControl->setZoom(100) ;    pPdfControl->gotoFirstPage() ;    // Ne fait rien de visible :    // wchar_t buff[1024] ;    // MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "FitH", -1, buff, sizeof(buff)) ;    // pPdfControl->setView(buff) ;    // The file doesn't show if the window is not resized after opening    //    SetTimer(ID_PDF_TIMER, 1000) ;  }  else  {    TWebBrowser* pIEControl = _IEForm->Control ;    pIEControl->Left   = 0 ;    pIEControl->Top    = 0 ;
    pIEControl->Width  = clientRect.Width() - 5 ;
    pIEControl->Height = clientRect.Height() - 5 ;

    // Form->Control->setViewRect(0, 0, r.Width() - 10, r.Height() - 10) ;
    string sTrace = string("NSAcrobatView::SetupWindow, showing Internet Explorer Form") ;    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;    _IEForm->Show() ;
  }

  MakeVisible() ;
}

void
NSAcrobatView::EvTimer(uint timerId)
{
	if (timerId != ID_PDF_TIMER)
    return ;

  string sTrace = string("NSAcrobatView::EvTimer woke up.") ;
  _pDoc->pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  EvSize(SIZE_RESTORED, Parent->GetClientRect().Size()) ;

  KillTimer(ID_PDF_TIMER) ;
}

void
NSAcrobatView::CmPublier()
{
	// Form->Control->printWithDialog() ;
  _pDoc->Publier() ;
}

void
NSAcrobatView::CmFileSave()
{
	Enregistrer() ;
}

voidNSAcrobatView::SetupNavBar()
{
	TMyApp* pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;

  if (_pMUEViewMenu)
  	pMyApp->GetMainWindow()->SetMenuDescr(*_pMUEViewMenu) ;

	pMyApp->FlushControlBar() ;

/*
	pMyApp->cb2->Insert(*new TButtonGadget(IDC_EDITION_HTM, IDC_EDITION_HTM, TButtonGadget::Command));
  pMyApp->cb2->Insert(*new TButtonGadget(IDC_RAFRAICHIR_HTM, IDC_RAFRAICHIR_HTM, TButtonGadget::Command));
  pMyApp->cb2->Insert(*new TButtonGadget(CM_PRECEDENT, CM_PRECEDENT, TButtonGadget::Command));
  pMyApp->cb2->Insert(*new TSeparatorGadget);
  pMyApp->cb2->Insert(*new TSeparatorGadget);
  pMyApp->cb2->Insert(*new TButtonGadget(CM_SUIVANT, CM_SUIVANT, TButtonGadget::Command));
*/

  pMyApp->getSecondaryControlBar()->LayoutSession() ;}

voidNSAcrobatView::EvClose()
{
	TWindow::EvClose() ;
}

// Fonctions EvSetFocus et EvKillFocus pour la control Bar
////////////////////////////////////////////////////////////////
void
NSAcrobatView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
	TMyApp* pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;

  TWindow::EvSetFocus(hWndLostFocus) ;

	if (_bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
  	SetupNavBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }
}

voidNSAcrobatView::EvKillFocus(THandle hWndGetFocus /* may be 0 */)
{
	TWindow::EvKillFocus(hWndGetFocus);
}

voidNSAcrobatView::EvKeyDown(uint key, uint repeatCount, uint flags)
{
/*
    WORD wScrollNotify = 0xFFFF;
    bool bVScroll = true;
    int  dx, dy;

    // Touches Ctrl - Quelque chose
    //
    if (GetKeyState(VK_CONTROL) < 0)
    {
    	switch (key)
		{
        	// Ctrl C : Copier
            //
        	case 'C':
            	if (GetKeyState(VK_CONTROL) < 0)
            	{
                	// copie du texte sélectionné dans le presse-papier
                	IDispatch* pdisp = Form->Control->Document;
                	IOleCommandTarget* command;
                	pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command);
                	if(command)
                	{
      	            	command->Exec(NULL, Shdocvw_tlb::OLECMDID_COPY, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT, NULL, NULL);
                    	command->Release();
                	}
                	pdisp->Release();
                	// Form->Control->ExecWB(Shdocvw_tlb::OLECMDID_COPY, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT, NULL, NULL);
            	}
            	break;
			// Ctrl A : Tout sélectionner
            //
        	case 'A':            	if (GetKeyState(VK_CONTROL) < 0)
            	{
                	// copie du texte sélectionné dans le presse-papier
                	IDispatch* pdisp = Form->Control->Document;
                	IOleCommandTarget* command;
                	pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command);
                	if(command)
                	{
      	            	command->Exec(NULL, Shdocvw_tlb::OLECMDID_SELECTALL, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT, NULL, NULL);
                    	command->Release();
                	}
                	pdisp->Release();
                	// Form->Control->ExecWB(Shdocvw_tlb::OLECMDID_SELECTALL, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT, NULL, NULL);
            	}
            	break;
		}
	}
    else
    {
    	switch (key)
		{
   	    	case VK_UP:
      	    	wScrollNotify = SB_LINEUP;
            	dx = 0; dy = -10;
            	break;

        	case VK_PRIOR:
      	    	wScrollNotify = SB_PAGEUP;
            	dx = 0; dy = -100;
            	break;

        	case VK_NEXT:
      	    	wScrollNotify = SB_PAGEDOWN;
            	dx = 0; dy = 100;
            	break;

        	case VK_DOWN:
      	    	wScrollNotify = SB_LINEDOWN;
            	dx = 0; dy = 10;
            	break;

        	case VK_HOME:
      	    	wScrollNotify = SB_TOP;
            	break;

        	case VK_END:
      	    	wScrollNotify = SB_BOTTOM;
            	break;

        	case VK_RIGHT:
            	wScrollNotify = SB_LINERIGHT;
            	dx = 10; dy = 0;
            	bVScroll = false;
            	break;

        	case VK_LEFT:
            	wScrollNotify = SB_LINELEFT;
            	dx = -10; dy = 0;
            	bVScroll = false;
            	break;
        }
    }

    if (wScrollNotify != 0xFFFF)
    {
        // tentatives de scroll...
        if (bVScroll)
            SendMessage(WM_VSCROLL, MAKELONG(wScrollNotify, 0), 0L);
        else
            SendMessage(WM_HSCROLL, MAKELONG(wScrollNotify, 0), 0L);

        // Form->Control->ScrollBy(dx, dy);
    }
*/
	TWindowView::EvKeyDown(key, repeatCount, flags);
}
voidNSAcrobatView::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
/*
	OWL::TPopupMenu *menu = new OWL::TPopupMenu();

#ifndef N_TIERS
	if (!strcmp(pDocRef->pDocInfo->pDonnees->type, "TWHTM"))
#else
	if (!strcmp(pDocRef->pDocInfo->pDonnees->type, "ZTHTM"))
#endif
	{
		menu->AppendMenu(MF_STRING, CM_EDITER, "Editer");
    menu->AppendMenu(MF_SEPARATOR, 0, 0);
    menu->AppendMenu(MF_STRING, CM_IMPRIME, "Publier");
    menu->AppendMenu(MF_STRING, CM_COMPOSE, "Composer");
	}
  else
	{
		menu->AppendMenu(MF_GRAYED, 0,         "Editer");
    menu->AppendMenu(MF_SEPARATOR, 0, 0);
    menu->AppendMenu(MF_STRING, CM_IMPRIME, "Imprimer");
    menu->AppendMenu(MF_STRING, CM_COMPOSE, "Composer");
	}

	ClientToScreen(point) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
	delete menu ;
*/
}

// Fonction Enregistrer
////////////////////////////////////////////////////////////////
bool NSAcrobatView::Enregistrer(){	string sFichier = _pDoc->nomFichier ;
  if (string("") != sFichier)
  {
  	erreur("Erreur lors de la sauvegarde.", warningError, 0, _pDoc->pContexte->GetMainWindow()->GetHandle()) ;
		return true ;
  }

  sFichier = _pDoc->szFichierExterne ;
  if (string("") == sFichier)
  	return false ;

  _pDoc->SetNomFichier(sFichier) ;

  string sNomFichier ;
  string sLocalis ;

  bool bRenommerOk = _pDoc->Renommer(sNomFichier, sLocalis, "ZTPDF") ;  if (false == bRenommerOk)  	return false ;
  // Copying the temporary file, with the new name
  //
  if (!CopyFile(sFichier.c_str(), _pDoc->nomFichier, true))
  {  	string sErrorText = _pDoc->pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") ;    sErrorText += string(" ") + sFichier + string(" -> ") + _pDoc->nomFichier ;
    erreur(sErrorText.c_str(), standardError, 0, _pDoc->pContexte->GetMainWindow()->GetHandle()) ;  	_pDoc->SetNomFichier("") ;    return false ;  }

	if (_pDoc->Referencer("ZTPDF", "", sNomFichier, sLocalis))
  {    _pDoc->SetDirty(false) ;    // Displaying the new file instead of the temporary one    //    displayFile(_pDoc->nomFichier) ;
    // on remet le titre à jour    TWindow::SetDocTitle(_pDoc->GetTitle(), 0) ;

    // Now, we can delete the temporary file
    if (NsDeleteTemporaryFile(_pDoc->pContexte->getSuperviseur(), sFichier))
    	_pDoc->SetFichierExterne(string("")) ;
  }
  else
  {
  	_pDoc->SetDirty(true) ;

    // on remet le document dans l'état initial    _pDoc->SetNomFichier("") ;
  }

  return true ;}

bool
NSAcrobatView::IsTabKeyMessage(MSG &msg)
{
	if (GetCapture() == NULL)
  {
  	if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
    {
    	if (msg.wParam == VK_TAB)
      {
      	SendMessage(CN_BASE + msg.message, msg.wParam, msg.lParam) ;
        return true ;
      }
    }
  }

	return false ;
}

// Get the window handle of the OLE control
//
HWND
NSAcrobatView::GetOleControlWindow()
{
  return GetSonWindow(GetHandle(), 2) ;
}

// Get the window handle of the OLE control
//
HWND
NSAcrobatView::GetSonWindow(HWND hThis, int iLevel)
{
  // return value indicates the child window at the top of the Z order
  //
  HWND hChildWindow = ::GetWindow(hThis, GW_CHILD) ;

  int iCurrentLevel = 0 ;
  while ((iCurrentLevel < iLevel) && (0 != hChildWindow))
  {
    iCurrentLevel++ ;
    hChildWindow = ::GetWindow(hChildWindow, GW_CHILD) ;
  }

  return hChildWindow ;
}

// keep the control at full window size
//
voidNSAcrobatView::EvSize(uint sizeType, NS_CLASSLIB::TSize& size){
	TWindowView::EvSize(sizeType, size) ;

  NS_CLASSLIB::TRect clientRect = Parent->GetClientRect() ;

  NSSuper *pSuper = _pDoc->pContexte->getSuperviseur() ;

  if (false == pSuper->mustUseIeForPdf())
  {
    _AcrobatForm->ClientWidth  = clientRect.Width() ;
    _AcrobatForm->ClientHeight = clientRect.Height() ;

    _AcrobatForm->Left   = 0 ;
    _AcrobatForm->Top    = 0 ;
    _AcrobatForm->Width  = clientRect.Width() ;
    _AcrobatForm->Height = clientRect.Height() ;

    // TWindow(_AcrobatForm->ParentWindow).SetWindowPos(NULL, 0, 0, clientRect.Width(), clientRect.Height(), SWP_NOZORDER | SWP_NOMOVE) ;

    TAcroPDF* pPdfControl = _AcrobatForm->Control ;
    if (pPdfControl && pPdfControl->Visible)
    {
      pPdfControl->Left   = 0 ;
      pPdfControl->Top    = 0 ;
      pPdfControl->Width  = clientRect.Width() ;
      pPdfControl->Height = clientRect.Height() ;

      string sTrace = string("NSAcrobatView::EvSize") ;
      switch(sizeType)
      {
        case SIZE_RESTORED  : sTrace += string(" (restored)") ; break ;
        case SIZE_MINIMIZED : sTrace += string(" (minimized)") ; break ;
        case SIZE_MAXIMIZED : sTrace += string(" (maximized)") ; break ;
        case SIZE_MAXSHOW   : sTrace += string(" (maxshow)") ; break ;
        case SIZE_MAXHIDE   : sTrace += string(" (maxhide)") ; break ;
      }
      pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

      // Official hack from Adobe to have TAcroPDF resize when windows is resized
      //
      resizeOleControl(clientRect) ;
    }

    // This function is a zoom (size of the page displayed inside the view)
    // pPdfControl->setViewRect(pPdfControl->Left, pPdfControl->Top,pPdfControl->Width, pPdfControl->Height) ;

    _AcrobatForm->Invalidate() ;

    if (SIZE_RESTORED == sizeType)
      Parent->Invalidate() ;
  }
  else
  {
    _IEForm->Left   = 0 ;
    _IEForm->Top    = 0 ;
    _IEForm->Width  = clientRect.Width() ;
    _IEForm->Height = clientRect.Height() ;
    _IEForm->Invalidate() ;
  }
}

//// Let the form process keystrokes in its own way.  Without
// this method, you can't tab between control on the form.
//
bool
NSAcrobatView::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;

  if (false == result)
  	result = TWindow::PreProcessMsg(msg) ;

  return result ;
}

////
//
void
NSAcrobatView::MakeVisible()
{
  NSSuper *pSuper = _pDoc->pContexte->getSuperviseur() ;

  if (false == pSuper->mustUseIeForPdf())
	  _AcrobatForm->Visible = true ;
  else

    _IEForm->Visible = true ;
}

string
NSAcrobatView::getFileName()
{
  if ('\0' == _pDoc->nomFichier[0])
    return string(_pDoc->szFichierExterne) ;  return string(_pDoc->nomFichier) ;}

// Official hack from Adobe to have TAcroPDF resize when windows is resized
//
void
NSAcrobatView::resizeOleControl(NS_CLASSLIB::TRect clientRect)
{
  // Get the first child, as the OLE wrapper
  //
  HWND hChildWindow = ::GetWindow(GetHandle(), GW_CHILD) ;
  if ((HWND) 0 == hChildWindow)
    return ;

  TWindow(hChildWindow).SetWindowPos(NULL, 0, 0, clientRect.Width(), clientRect.Height(), SWP_NOZORDER | SWP_NOMOVE) ;

  // Get the first little child as the control itself
  //
  HWND hLittleChildWindow = ::GetWindow(hChildWindow, GW_CHILD) ;
  if ((HWND) 0 == hLittleChildWindow)
    return ;

  TWindow(hLittleChildWindow).SetWindowPos(NULL, 0, 0, clientRect.Width(), clientRect.Height(), SWP_NOZORDER | SWP_NOMOVE) ;
}

void
NSAcrobatView::TerminateProcess(HWND hTarget)
{
  if ((HWND) NULL == hTarget)
    return ;

  unsigned long lProcessId ;
  DWORD dId = ::GetWindowThreadProcessId(hTarget, &lProcessId) ;
  if (0 == lProcessId)
    return ;

  uint uExitCode = 0 ;
  TerminateProcess(lProcessId, uExitCode) ;
}

// Kills a process from its PID
//
bool
NSAcrobatView::TerminateProcess(unsigned long dwProcessId, UINT uExitCode)
{
  DWORD dwDesiredAccess = PROCESS_TERMINATE ;
  bool  bInheritHandle  = false ;
  HANDLE hProcess = ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId) ;
  if ((HANDLE) NULL == hProcess)
    return false ;

  bool result = ::TerminateProcess(hProcess, uExitCode) ;

  CloseHandle(hProcess) ;

  return result ;
}

/******************************************************************************/
//					METHODES DE Document / Vue pour Acrobat
/******************************************************************************/

// Constructeur NSAcrobatDocument////////////////////////////////////////////////////////////////

NSAcrobatDocument::NSAcrobatDocument(TDocument* parent, bool bROnly, NSDocumentInfo* pDocumentInfo,                             NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
                  :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
	// Mise à vide du nom de fichier
  SetNomFichier("") ;
  SetFichierExterne("") ;
	Open(0, "") ;
}

NSAcrobatDocument::NSAcrobatDocument(TDocument *parent, NSContexte *pCtx)                  :NSRefDocument(parent, pCtx)
{
	// Mise à vide du nom de fichier
  SetNomFichier("") ;
  SetFichierExterne("") ;
	Open(0, "") ;
}

NSAcrobatDocument::~NSAcrobatDocument()
{
	if ('\0' != szFichierExterne[0])
    NsDeleteTemporaryFile(pContexte->getSuperviseur(), string(szFichierExterne)) ;
}

void
NSAcrobatDocument::Visualiser()
{
  string sTrace = string("NSAcrobatDocument::Visualiser for file ") + string(nomFichier) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSteps) ;

	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(this, "Pdf Format") ;
}

voidNSAcrobatDocument::SetNomFichier(string sFich){
  memset(nomFichier, 0, 255) ;
	strcpy(nomFichier, sFich.c_str()) ;
}

void
NSAcrobatDocument::SetFichierExterne(string sFich)
{
  memset(szFichierExterne, 0, 255) ;
	strcpy(szFichierExterne, sFich.c_str()) ;

  if ('\0' == nomFichier[0])
  	replaceTagsByValues() ;
}

stringNSAcrobatDocument::GetDocExt(){
	string sExt = string("") ;

  size_t i = 0 ;
	for ( ; (i < strlen(nomFichier)) && ('.' != nomFichier[i]) ; i++) ;

  if (i < strlen(nomFichier))
  {
    i++ ;
    while (i < strlen(nomFichier))
    	sExt += nomFichier[i++] ;
  }

  return sExt ;
}

void
NSAcrobatDocument::replaceTagsByValues()
{
  if ('\0' == szFichierExterne[0])
    return ;

	ifstream inFile ;
	inFile.open(szFichierExterne, ios::binary) ;
  if (!inFile)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText = string("NSAcrobatDocument::replaceTagsByValues ") + sErrorText + string(" ") + string(szFichierExterne) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return ;
  }

  ofstream outFile ;

  string sPathTxt = pContexte->PathName("NTTX") ;
  // on trouve le nom du fichier temporaire à visualiser

  string sFileBase = string("tmp") ;
	sFileBase += pContexte->getPatient()->getNss() ;
  string sFileName = sPathTxt + ::nomSansDoublons(sPathTxt, sFileBase, "pdf") ;

  outFile.open(sFileName.c_str(), ios::binary) ;
	if (!outFile)
  {
  	inFile.close() ;

    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") ;
    sErrorText = string("NSAcrobatDocument::replaceTagsByValues ") + sErrorText + string(" ") + sFileName ;
    erreur(sErrorText.c_str(), standardError, 0) ;

    return ;
  }

  map<string, string> tagsBuffer ;

  // If a Tag is similar to the previous one, and the PatPatho is made of
  // more than one element, we answer with next element
  //
  char iChar = inFile.get() ;

  while (!inFile.eof())
  {
    if (('[' == iChar) && (!inFile.eof()))
    {
    	iChar = inFile.get() ;
      if ('$' == iChar)
      {
      	string sBalise = string("") ;
        bool bTourner = true ;
        while (!inFile.eof() && bTourner)
        {
        	iChar = inFile.get() ;
          if ('$' == iChar)
          {
          	if (!inFile.eof())
            {
            	iChar = inFile.get() ;
              if (']' == iChar)
              	bTourner = false ;
              else
              	sBalise += string("$") + string(1, iChar) ;
            }
          }
          else
          	sBalise += string(1, iChar) ;
        }
        //
        // Traiter la balise
        //
        string sResult = pdfStringToWindowString(&sBalise) ;

        if (string("") != sResult)
        {
          NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) 0 ;
        	pContexte->getPatient()->remplaceTagsChemin(sResult, pPatPathoArray, &tagsBuffer) ;
          if (pPatPathoArray)
          	delete pPatPathoArray ;
        }

        if (string("") != sResult)
        	for (size_t i = 0 ; i < strlen(sResult.c_str()) ; i++)
          	outFile.put(sResult[i]) ;
      }
      else
      {
      	outFile.put('[') ;
      	outFile.put(iChar) ;
      }
    }
    else
    	outFile.put(iChar) ;

    iChar = inFile.get() ;
  }

  inFile.close() ;
  outFile.close() ;

  strcpy(szFichierExterne, sFileName.c_str()) ;

  return ;
}

string
NSAcrobatDocument::pdfStringToWindowString(string *pPdfString)
{
	if (!pPdfString)
		return string("") ;

  size_t iPdfLen = strlen(pPdfString->c_str()) ;

  string sWindowString = string("") ;

  size_t lastpos = 0 ;
  size_t pos = pPdfString->find('\\') ;
  while (NPOS != pos)
  {
    if (pos > lastpos)
  		sWindowString += string(*pPdfString, lastpos, pos - lastpos) ;

  	// abcd\040efg
		// 01234567890
    //     4
    if (pos <= iPdfLen - 4)
    {
    	string sOctalValue = string(*pPdfString, pos + 1, 3) ;
      int iPdfValue = atoi(sOctalValue.c_str()) ;
      if (iPdfValue > 0)
      {
      	string sWinChars = pdfCharToWindowString(iPdfValue) ;
        sWindowString += sWinChars ;
      }
      lastpos = pos + 4 ;
    }
    else
    	lastpos = pos ;

    pos = pPdfString->find('\\', lastpos) ;
  }
  if (lastpos <= iPdfLen)
  	sWindowString += string(*pPdfString, lastpos, iPdfLen - lastpos) ;

  return sWindowString ;
}

string
NSAcrobatDocument::pdfCharToWindowString(int iPdfValue)
{
	//
  // PDF value differs from Windows value
  //
	switch(iPdfValue)
  {
  	case  32 : return string(1, char(210)) ;
    case  37 : return string(1, char(230)) ;
    case  40 : return string(" ") ; // Space
  	case 200 : return string(1, char(225)) ;
    case 201 : return string(1, char(206)) ;
    case 202 : return string(1, char(207)) ;
    case 203 : return string(1, char(205)) ;
    case 204 : return string(1, char(227)) ;
    case 205 : return string(1, char(226)) ;
    case 210 : return string(1, char(213)) ;
    case 211 : return string(1, char(233)) ;
    case 213 : return string(1, char(211)) ;
    case 214 : return string(1, char(204)) ;
    case 215 : return string(1, char(223)) ;
    case 216 : return string(1, char(224)) ;
    case 217 : return string(1, char(221)) ;
    case 220 : return string(1, char(222)) ;
    case 221 : return string(1, char(202)) ;
    case 222 : return string(1, char(231)) ;
  	case 226 : return string(1, char(214)) ;
  	case 227 : return string(1, char(212)) ;
  	case 230 : return string(1, char(237)) ;
  	case 231 : return string(1, char(216)) ;
    case 235 : return string(1, char(232)) ;
    case 240 : return string(1, char(200)) ;
  }
  //
  // PDF value differs does not exist in Windows
  //
  switch(iPdfValue)
  {
  	case  30 :
    case  31 :
    case  33 :
    case  34 :
    case  35 :
    case  36 :
    case 207 :
    case 212 :
    case 223 :
    case 224 :
  	case 225 :
    case 232 :
    case 233 :
    	return string("") ;
  }
  //
  // PDF and Windows values are equal
  //
  char cWinVal = char(iPdfValue) ;
  return string(1, cWinVal) ;
}

// Ouverture du document////////////////////////////////////////////////////////////////

boolNSAcrobatDocument::Open(int /*mode*/, LPCSTR path){
	string sNomFichier = string("") ;

	// Si il existe, prise du nom de fichier de la fiche document  ValideFichier(&sNomFichier) ;

	// Sinon, on sort	if (false == _bDocumentValide)
		return false ;

	//	// Rangement du nom de fichier dans le document
	//
	SetNomFichier(sNomFichier) ;
  SetDirty(false) ;
  return true ;
}

// Fermeture du document////////////////////////////////////////////////////////////////
bool NSAcrobatDocument::Close()
{
  // if (TDocument::Close())
  // 	 return false;

  SetNomFichier("") ;
  return true ;
}

// Génération du html correspondant au document
////////////////////////////////////////////////////////////////
bool
NSAcrobatDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
  string sFileToCopy ;
  if ('\0' == nomFichier[0])  	sFileToCopy = szFichierExterne ;  else  	sFileToCopy = nomFichier ;

  sNomHtml = ::nomSansDoublons(sPathHtml, sNomHtml, "pdf") ;
  string sNomFich = sPathDest + sNomHtml ;

  return ::CopyFile(sFileToCopy.c_str(), sNomFich.c_str(), FALSE) ;
}

// Indique si document ouvert (il existe un fichier associé)////////////////////////////////////////////////////////////////
bool NSAcrobatDocument::IsOpen()
{
	if (_fstricmp(nomFichier, ""))
    return true ;
 	else
    return false ;
}

// Trouve un nom de fichier nautilus au document
////////////////////////////////////////////////////////////////
bool
NSAcrobatDocument::Renommer(string& sNomFichier, string& sLocalis, string sType, string* psNomBrut)
{
  string sExt = GetDocExt() ;

  if (false == TrouveNomFichier(sType, sExt, sNomFichier, sLocalis))
  {
    erreur("Pb à l'attribution d'un nouveau nom pour ce fichier",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (psNomBrut)
  {
  	// Dans ce cas on renvoie aussi le nom brut (sans extension)
    size_t pos = sNomFichier.find('.') ;
    if (NPOS != pos)
    	*psNomBrut = string(sNomFichier, 0, pos) ;
    else
    	*psNomBrut = sNomFichier ;
  }

  string sPath = pContexte->PathName(sLocalis) ;

  // on construit le nom complet pour la sauvegarde
  SetNomFichier(sPath + sNomFichier) ;

  SetDirty(true) ;

  return true ;
}

// fin de nsvisual.cpp/////////////////////////////////////////////////////////////////

