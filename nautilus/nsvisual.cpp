//---------------------------------------------------------------------------// #include "stdafx.h"

#include <vcl.h>
#pragma hdrstop

#include <dstring.h>#include "WebBrowserPrint.h"#include "ExamHisto.h"

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
#include "nautilus\nsvisual.h"
#include "nautilus\nautilus.rh" 	// pour l'id de commande CM_IMPRIME (et CM_EDITER)
#include "nautilus\nscompub.h"
#include "nautilus\nsbasimg.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nsmodhtm.h"
#include "nautilus\nsttx.h"
#include "nsbb\nsattvaltools.h"
#include "nsepisod\nsToDo.h"

/************************************
typedef enum BrowserNavConstants {    navOpenInNewWindow = 1,
    navNoHistory = 2,
    navNoReadFromCache = 4,
    navNoWriteToCache = 8
} BrowserNavConstants;
**********************************/

// Table de réponses de la classe NSVisualView
////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE1(NSVisualView, TWindowView)
  EV_WM_CLOSE,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_WM_KEYDOWN,
  EV_WM_RBUTTONDOWN,

  EV_COMMAND(IDC_EDITION_HTM, CmEditer),          // pour les fichiers Word  EV_COMMAND(IDC_RAFRAICHIR_HTM, Rafraichir),
  EV_COMMAND(CM_IMPRIME, CmPublier),
  EV_COMMAND(CM_COMPOSE, CmComposer),
  EV_COMMAND(CM_FILECLOSE, EvClose),
  EV_COMMAND(CM_PRECEDENT, CmPrecedent),
  EV_COMMAND(CM_SUIVANT, CmSuivant),
END_RESPONSE_TABLE;

// Constructeur NSVisualView
////////////////////////////////////////////////////////////////

NSVisualView::NSVisualView(NSRefDocument& doc, TWindow *parent)             :NSVisualViewBase(doc.pContexte->getSuperviseur(), doc, parent),              _pDocRef(&doc), Form((TExamHistoForm*) 0){
	pMUEViewMenu = new OWL::TMenuDescr ;

	nsMenuIniter menuIter(_pDocRef->pContexte) ;
  menuIter.initMenuDescr(pMUEViewMenu, "menubar_visual") ;

  TMyApp* pMyApp = _pDocRef->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->GetMainWindow()->SetMenuDescr(*pMUEViewMenu) ;

	initValues() ;
  initParams() ;
  if (0 == _pDocRef->getNbImpress())  {
  	// cas utilisé pour la visualisation
		if (string("") != _pDocRef->_sTemplate) // le document a une template associée
    {
    	// on récupère l'url du serveur nautilus
      _sUrl = _pDocRef->pContexte->PathName("UHTM") ;

      // on génère le fichier à visualiser      GenereHtml() ;
    }
    else
    {
    	// l'url est contenue dans le champ fichier du document
      // on passera par ftp ou http selon le champ fichier
      if (_pDocRef->_pDocInfo)
				_sUrl = _pDocRef->_pDocInfo->getFichier() ;
			_sHtml = string("") ;		}
	}
  else // cas utilisé dans la publication
  {
  	// Dans ce cas, on a déjà généré le fichier html final (sNomDocHtml)

		_sUrl  = _pDocRef->pContexte->PathName("UHTM") ;    _sHtml = _pDocRef->getNomDocHtml() ;
  }
}

NSVisualView::NSVisualView(NSHtmlModelDocument* pDoc, TWindow *parent)
             :NSVisualViewBase(pDoc->pContexte->getSuperviseur(), *pDoc, parent), Form((TExamHistoForm*) 0){
  initValues() ;

	_pDocRef     = (NSRefDocument*) pDoc ;
	pMUEViewMenu = (TMenuDescr*) 0 ;
}

void
NSVisualView::initValues()
{
  initBaseValues() ;

	delai      	   = 0 ;
  bUseHook       = false ;
  bVerbose       = false ;
  bSimpleCtrl    = false ;
  bKillTmp       = true ;
  bWaitImp       = false ;
  iTempPostNav   = 0 ;
  iTempImp       = 0 ;

	_bSetupToolBar = true ;
  _sLettreHtml   = string("") ;
  _pEditUrl      = (NSEditUrl*) 0 ; // ne pas oublier
  pBrowserPrint  = (CWebBrowserPrint*) 0 ;
}

// Destructeur NSVisualView////////////////////////////////////////////////////////////////

NSVisualView::~NSVisualView(){
	if (pMUEViewMenu)
  	delete pMUEViewMenu ;

	if (true == bKillTmp)
	{
		// boucle d'attente		Tempo() ;

		// !! Ne pas deleter pEditUrl (déjà fait par FlushControlBar)		DetruireTemp() ;
	}

	if (pBrowserPrint)
		delete pBrowserPrint ;

	// Delete de la Form  if (Form)		delete Form ;
	//CoUninitialize();
	//OleUninitialize() ;
}

// GetWindow renvoie this////////////////////////////////////////////////////////////////

TWindow*NSVisualView::GetWindow()
{
	return (TWindowView*) this ;
}

// Fonction CanClose pour détruire la barre d'outils
////////////////////////////////////////////////////////////////

boolNSVisualView::CanClose()
{
	TMyApp* pMyApp = _pDocRef->pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	_pEditUrl = (NSEditUrl*) 0 ;
	_bSetupToolBar = false ;

	return TWindowView::CanClose() ;}

// Fonction SetupWindow////////////////////////////////////////////////////////////////

voidNSVisualView::SetupWindow()
{
	TWindowView::SetupWindow() ;

	/******************************	ModifyStyle(WS_BORDER, 0);
	MakeVisible();
	EnableWindow(true);
	********************************/
	// if (useformsize){
	// ::SetWindowPos(Parent->HWindow, NULL, 0, 0, Form->Width, Form->Height, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOZORDER);
	// }

	TWindowView::ModifyStyle(WS_BORDER, WS_CHILD) ;	Form->Show() ;
	MakeVisible() ;

  // Print mode: hide command pannel
  //
  if (_pDocRef->getNbImpress() > 0)
    Form->Pannel->Hide() ;
  else
  {
    // Check if there is a previous and a next document of the kind in order to
    // invalidate corresponding buttons
    //
    NSDocumentInfo* pPrevDocInfo = getPrevSameKindDocument() ;
    if ((NSDocumentInfo*) NULL == pPrevDocInfo)
      Form->Previous->Enabled = false ;

    NSDocumentInfo* pNextDocInfo = getNextSameKindDocument() ;
    if ((NSDocumentInfo*) NULL == pNextDocInfo)
      Form->Next->Enabled = false ;
  }
}

//
// Override a TWindow virtual to create the HWND directly.
// NSVisualView and the VCL TForm1 class both end up
// wrapping the same HWND.
//

voidNSVisualView::PerformCreate(int /*menuOrId*/)
{
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

	// CoInitialize(NULL);/*try{	OleInitialize(NULL);    // Use OleInitialize to get clipboard functionality}catch (...)
{
  string ps = string("Exception NSVisualView::PerformCreate OleInitialize") ;
  pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
	erreur(ps.c_str(), standardError, 0) ;
}*/

try
{
	Form = new TExamHistoForm(Parent->GetHandle(), this) ;
	Form->Visible = false ;
	Form->ParentWindow = Parent->HWindow ;
}
catch (...)
{
  string ps = string("Exception NSVisualView::PerformCreate Form creation") ;
  _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
	erreur(ps.c_str(), standardError, 0) ;
}

try
{
	SetHandle(Form->Handle) ;
	::SetParent(Forms::Application->Handle, _pDocRef->pContexte->GetMainWindow()->HWindow) ;
}
catch (...)
{
  string ps = string("Exception NSVisualView::PerformCreate SetHandle") ;
  _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
	erreur(ps.c_str(), standardError, 0) ;
}

	if (string("") != _sHtml)		SetDocTitle(_pDocRef->GetTitle(), 0) ;

	// on navigue vers le fichier html à visualiser	string ps = string("Navigate to url: ") + _sUrl + _sHtml ;
	_pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

try
{
	Navigate(_sUrl + _sHtml) ;}catch (Exception &ex){
  string ps = string("Exception NSVisualView::PerformCreate Navigate when navigating to ") + _sUrl + _sHtml + string(" (") + string(ex.Message.c_str()) + string(")") ;
  _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
}
catch (...)
{
  string ps = string("Exception NSVisualView::PerformCreate Navigate when navigating to ") + _sUrl + _sHtml ;
  _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  // ----- Switched off because it can get thrown "at random" and block printing
  // erreur("Exception NSVisualView::PerformCreate Navigate.", standardError, 0) ;
}}
voidNSVisualView::SetupNavBar()
{
	TMyApp* pMyApp = _pDocRef->pContexte->getSuperviseur()->getApplication() ;

	if (pMUEViewMenu)
  	pMyApp->GetMainWindow()->SetMenuDescr(*pMUEViewMenu) ;

  pMyApp->FlushControlBar() ;

  _pEditUrl = new NSEditUrl(this) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(IDC_EDITION_HTM, IDC_EDITION_HTM, TButtonGadget::Command)) ;  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(IDC_RAFRAICHIR_HTM, IDC_RAFRAICHIR_HTM, TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_PRECEDENT, CM_PRECEDENT, TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TSeparatorGadget) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TEditGadget(*_pEditUrl)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TSeparatorGadget) ;
  pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_SUIVANT, CM_SUIVANT, TButtonGadget::Command)) ;

  pMyApp->getSecondaryControlBar()->LayoutSession() ;}

voidNSVisualView::EvClose()
{
	TWindow::EvClose() ;
}

// Fonctions EvSetFocus et EvKillFocus pour la control Bar
////////////////////////////////////////////////////////////////
void
NSVisualView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
	TMyApp* 	pMyApp = _pDocRef->pContexte->getSuperviseur()->getApplication() ;
  WideString  autoUrl ;
	AnsiString 	url ;

  TWindow::EvSetFocus(hWndLostFocus);

  if (_bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
  	SetupNavBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;

    // pour le cas où on change de focus entre plusieurs NSVisualView
    if (_nbNav)
    {
    	autoUrl = Form->Control->LocationURL ;
      url = AnsiString(autoUrl) ;
      // ici à priori pEditUrl est valide car on fait le new dans SetupNavBar
      _pEditUrl->SetText(url.c_str()) ;
    }
  }
}

voidNSVisualView::EvKillFocus(THandle hWndGetFocus /* may be 0 */)
{
	TWindow::EvKillFocus(hWndGetFocus) ;

  if (_pEditUrl)
  {
    // delete pEditUrl ;
    _pEditUrl = (NSEditUrl*) 0 ;
  }
}

voidNSVisualView::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	WORD wScrollNotify = 0xFFFF ;
	bool bVScroll = true ;
	int  dx, dy ;

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
          if (command)
          {
          	command->Exec(NULL, Shdocvw_tlb::OLECMDID_COPY, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT, NULL, NULL);
            command->Release();
          }
          pdisp->Release();
          // Form->Control->ExecWB(Shdocvw_tlb::OLECMDID_COPY, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT, NULL, NULL);
        }
        break ;

			// Ctrl A : Tout sélectionner
      //
      case 'A':      	if (GetKeyState(VK_CONTROL) < 0)
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
        break ;
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
    	SendMessage(WM_VSCROLL, (MAKELONG(wScrollNotify, 0)), 0L);
    else
    	SendMessage(WM_HSCROLL, (MAKELONG(wScrollNotify, 0)), 0L);

    // Form->Control->ScrollBy(dx, dy);
	}

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}
voidNSVisualView::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	OWL::TPopupMenu *menu = new OWL::TPopupMenu();

	if (string("ZTHTM") == _pDocRef->_pDocInfo->getTypeSem())
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
}

// Fonction Navigate////////////////////////////////////////////////////////////////
void
NSVisualView::Navigate(string sUrlTarget)
{
// ---- Switched off for Vista compatibility : an exception is fired and block
//      the printing process
// try
// {
	wchar_t buff[1024] ;
	Variant Flags(navNoReadFromCache) ;
	TVariant VFlags = Flags.operator TVariant() ;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sUrlTarget.c_str(), -1, buff, sizeof(buff)) ;  if (Form && Form->Control)		Form->Control->Navigate(buff, &VFlags) ;
  else
  {
  	string ps = string("Form not ready when navigating to ") + sUrlTarget ;
    _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  }
// }
// catch (...)
// {
//	string sError = string("Exception NSVisualView::Navigate on ") + sUrlTarget ;
//  erreur(sError.c_str(), standardError, 0) ;
// }
}

void
NSVisualView::SetPrintParams(NSPrintParams& nspp)
{
	// Page Setup dialog settings
  if (nspp.sPaperSize != "")
    pBrowserPrint->m_sPaperSize = CString((nspp.sPaperSize).c_str()) ;
  if (nspp.sPaperSource != "")
    pBrowserPrint->m_sPaperSource = CString((nspp.sPaperSource).c_str()) ;
  if (nspp.sHeader != "")
    pBrowserPrint->m_sHeader = CString((nspp.sHeader).c_str()) ;
  if (nspp.sFooter != "")
    pBrowserPrint->m_sFooter = CString((nspp.sFooter).c_str()) ;

  if (nspp.sOrientation == "UNDEFINED")
  	pBrowserPrint->m_Orientation = CWebBrowserPrint::OrientationUndefined ;
  else if (nspp.sOrientation == "PORTRAIT")
  	pBrowserPrint->m_Orientation = CWebBrowserPrint::OrientationPortrait ;
  else if (nspp.sOrientation == "LANDSCAPE")
  	pBrowserPrint->m_Orientation = CWebBrowserPrint::OrientationLandscape ;

  if (nspp.sfLeftMargin != "")
  	pBrowserPrint->m_fLeftMargin = atof((nspp.sfLeftMargin).c_str()) ;
  if (nspp.sfTopMargin != "")
  	pBrowserPrint->m_fTopMargin = atof((nspp.sfTopMargin).c_str()) ;
  if (nspp.sfRightMargin != "")
  	pBrowserPrint->m_fRightMargin = atof((nspp.sfRightMargin).c_str()) ;
  if (nspp.sfBottomMargin != "")
  	pBrowserPrint->m_fBottomMargin = atof((nspp.sfBottomMargin).c_str()) ;

  // Print dialog settings
  if (nspp.sPrinterName != "")
    pBrowserPrint->m_sPrinterName = CString((nspp.sPrinterName).c_str()) ;

  if      (IsYes(nspp.sbPrintToFile))
  	pBrowserPrint->m_bPrintToFile = true ;
  else if (IsNo(nspp.sbPrintToFile))
  	pBrowserPrint->m_bPrintToFile = false ;

  if (nspp.sPrintRange == "UNDEFINED")
  	pBrowserPrint->m_PrintRange = CWebBrowserPrint::PrintRangeUndefined ;
  else if (nspp.sPrintRange == "ALL")
  	pBrowserPrint->m_PrintRange = CWebBrowserPrint::PrintRangeAll ;
  else if (nspp.sPrintRange == "PAGES")
  	pBrowserPrint->m_PrintRange = CWebBrowserPrint::PrintRangePages ;
  else if (nspp.sPrintRange == "SELECTION")
  	pBrowserPrint->m_PrintRange = CWebBrowserPrint::PrintRangeSelection ;

  if (nspp.slPrintRangePagesFrom != "")
    pBrowserPrint->m_lPrintRangePagesFrom = atol((nspp.slPrintRangePagesFrom).c_str()) ;
  if (nspp.slPrintRangePagesTo != "")
    pBrowserPrint->m_lPrintRangePagesTo = atol((nspp.slPrintRangePagesTo).c_str()) ;

  if      (IsYes(nspp.sbCollate))
  	pBrowserPrint->m_bCollate = true ;
  else if (IsNo(nspp.sbCollate))
  	pBrowserPrint->m_bCollate = false ;

  if (nspp.sPrintFrames == "UNDEFINED")
  	pBrowserPrint->m_PrintFrames = CWebBrowserPrint::PrintFramesUndefined ;
  else if (nspp.sPrintFrames == "SCREEN")
  	pBrowserPrint->m_PrintFrames = CWebBrowserPrint::PrintFramesScreen ;
  else if (nspp.sPrintFrames == "SELECTED")
  	pBrowserPrint->m_PrintFrames = CWebBrowserPrint::PrintFramesSelected ;
  else if (nspp.sPrintFrames == "INDIVIDUALLY")
  	pBrowserPrint->m_PrintFrames = CWebBrowserPrint::PrintFramesIndividually ;

  if      (IsYes(nspp.sbPrintLinks))
  	pBrowserPrint->m_bPrintLinks = true ;
  else if (IsNo(nspp.sbPrintLinks))
  	pBrowserPrint->m_bPrintLinks = false ;

  if      (IsYes(nspp.sbPrintLinkTable))
  	pBrowserPrint->m_bPrintLinkTable = true ;
  else if (IsNo(nspp.sbPrintLinkTable))
  	pBrowserPrint->m_bPrintLinkTable = false ;
}

void//NSVisualView::ExecWB(Shdocvw_tlb::OLECMDID iCommand, Shdocvw_tlb::OLECMDEXECOPT iOption)NSVisualView::ExecWB(int iCommand, int iNbExpl, bool bVerbose){try{  // on remplace ici l'ancienne commande d'impression par un controle MFC	// Form->Control->ExecWB(iCommand, iOption) ;	if (NULL == pBrowserPrint)		pBrowserPrint = new CWebBrowserPrint() ;  // Vieux	// IWebBrowser2Disp controlInterface = Form->Control->ControlInterface;  // Moins vieux  // Shdocvw_tlb::IWebBrowser2* pWebBrowser = (Shdocvw_tlb::IWebBrowser2*) Form->Control->ControlInterface ;  if ((NULL == Form) || (NULL == Form->Control))    return ;  IDispatch* pdisp = Form->Control->Application_ ;  if (NULL == pdisp)
		return ;	GUID DEF_CTL_INTF = {0xD30C1661, 0xCDAF, 0x11D0,{ 0x8A, 0x3E, 0x00, 0xC0, 0x4F, 0xC9, 0xE2, 0x6E} };	Shdocvw_tlb::IWebBrowser2* pWebBrowser = 0 ;	HRESULT hr = pdisp->QueryInterface(DEF_CTL_INTF, (void**)&pWebBrowser) ;  if ((FAILED(hr)) || (NULL == pWebBrowser))
    return ;	pBrowserPrint->SetWebBrowser(pWebBrowser) ;	// pBrowserPrint->m_Orientation = CWebBrowserPrint::OrientationLandscape;	SetPrintParams(_pDocRef->_nspp) ;	pBrowserPrint->Print(iNbExpl, bVerbose) ;}catch (...)
{
  string ps = string("Exception NSVisualView::ExecWB") ;
  _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
	erreur(ps.c_str(), standardError, 0) ;
}}voidNSVisualView::ExecWB_old(int iCommand, int iOption){	Form->Control->ExecWB(iCommand, iOption) ;}
voidNSVisualView::NavigateComplete()
{
	WideString  autoUrl, autoTitle ;
	AnsiString 	url, title ;
	string ps = "Ev : NavigateComplete..." ;
	_pDocRef->pContexte->getSuperviseur()->trace(&ps,1) ;

	autoUrl = Form->Control->LocationURL ;	url = AnsiString(autoUrl) ;

	if (!(url == _lastUrl))	{
  	// ici on vérifie que pEditUrl est valide car dans certains cas bizarres
    // la vue reçoit NavigateComplete avant EvSetFocus (qui fait le new sur pEditUrl)
    // si pEditUrl est invalide, on n'affiche rien.

    if ((_pEditUrl) && (_pEditUrl->GetHandle() != NULL))    	_pEditUrl->SetText(url.c_str()) ;

    _lastUrl = url ;
    // on incrémente ici nbNav et non dans Navigate    // pour tenir compte des navigations par liens

    if (_bNewNav)    	_nbNav = ++_page ;

    _bNewNav = true ;
    // si le titre est vide, on récupère LocationName    // if (!strcmp(pDocRef->GetTitle(), ""))
    // {
    //     autoTitle = Form->Control->Name;
    //     title = AnsiString(autoTitle);
    //     SetDocTitle(title.c_str(),0);
    // }

    if (_pDocRef->getNbImpress())    {    	// if (iTempPostNav > 0)      // pDocRef->pContexte->getSuperviseur()->Delay(iTempPostNav) ;      // CmPublier() ;      postNavigateComplete() ;    }
  }
}

void
NSVisualView::postNavigateComplete()
{
	NSSuper* pSuper = _pDocRef->pContexte->getSuperviseur() ;
	NSToDoTask* pTask = new NSToDoTask ;
	pTask->setWhatToDo(string("NavigateComplete")) ;
	pTask->setPointer1((void*) this, false) ;

	pTask->sendMe(pSuper) ;
}

void
NSVisualView::NavigateErrorEvent(int iStatusCode, string sURL)
{
	string ps = string("Ev : NavigateError for URL \"") + sURL + string("\"") ;
  ps += string(" (") + getNavigateErrorShortMsg(iStatusCode) + string(")") ;
	_pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  ps = string("Ev : NavigateError for URL \"") + sURL + string("\"") ;
  ps += string(" (") + getNavigateErrorDetailedMsg(iStatusCode) + string(")") ;
  erreur(ps.c_str(), standardError, 0) ;
}

string getNavigateErrorShortMsg(int iStatusCode)
{
  switch(iStatusCode)
  {
    case HTTP_STATUS_BAD_GATEWAY       : return string("HTTP_STATUS_BAD_GATEWAY") ;
    case HTTP_STATUS_BAD_METHOD        : return string("HTTP_STATUS_BAD_METHOD") ;
    case HTTP_STATUS_BAD_REQUEST       : return string("HTTP_STATUS_BAD_REQUEST") ;
    case HTTP_STATUS_CONFLICT          : return string("HTTP_STATUS_CONFLICT") ;
    case HTTP_STATUS_DENIED            : return string("HTTP_STATUS_DENIED") ;
    case HTTP_STATUS_FORBIDDEN         : return string("HTTP_STATUS_FORBIDDEN") ;
    case HTTP_STATUS_GATEWAY_TIMEOUT   : return string("HTTP_STATUS_GATEWAY_TIMEOUT") ;
    case HTTP_STATUS_GONE              : return string("HTTP_STATUS_GONE") ;
    case HTTP_STATUS_LENGTH_REQUIRED   : return string("HTTP_STATUS_LENGTH_REQUIRED") ;
    case HTTP_STATUS_NONE_ACCEPTABLE   : return string("HTTP_STATUS_NONE_ACCEPTABLE") ;
    case HTTP_STATUS_NOT_FOUND         : return string("HTTP_STATUS_NOT_FOUND") ;
    case HTTP_STATUS_NOT_SUPPORTED     : return string("HTTP_STATUS_NOT_SUPPORTED") ;
    case HTTP_STATUS_PAYMENT_REQ       : return string("HTTP_STATUS_PAYMENT_REQ") ;
    case HTTP_STATUS_PRECOND_FAILED    : return string("HTTP_STATUS_PRECOND_FAILED") ;
    case HTTP_STATUS_PROXY_AUTH_REQ    : return string("HTTP_STATUS_PROXY_AUTH_REQ") ;
    case HTTP_STATUS_REQUEST_TIMEOUT   : return string("HTTP_STATUS_REQUEST_TIMEOUT") ;
    case HTTP_STATUS_REQUEST_TOO_LARGE : return string("HTTP_STATUS_REQUEST_TOO_LARGE") ;
    case HTTP_STATUS_RETRY_WITH        : return string("HTTP_STATUS_RETRY_WITH") ;
    case HTTP_STATUS_SERVER_ERROR      : return string("HTTP_STATUS_SERVER_ERROR") ;
    case HTTP_STATUS_SERVICE_UNAVAIL   : return string("HTTP_STATUS_SERVICE_UNAVAIL") ;
    case HTTP_STATUS_UNSUPPORTED_MEDIA : return string("HTTP_STATUS_UNSUPPORTED_MEDIA") ;
    case HTTP_STATUS_URI_TOO_LONG      : return string("HTTP_STATUS_URI_TOO_LONG") ;
    case HTTP_STATUS_VERSION_NOT_SUP   : return string("HTTP_STATUS_VERSION_NOT_SUP") ;

    case INET_E_AUTHENTICATION_REQUIRED   : return string("INET_E_AUTHENTICATION_REQUIRED") ;
    case INET_E_BLOCKED_REDIRECT_XSECURITYID : return string("INET_E_BLOCKED_REDIRECT_XSECURITYID") ;
    case INET_E_CANNOT_CONNECT            : return string("INET_E_CANNOT_CONNECT") ;
    case INET_E_CANNOT_INSTANTIATE_OBJECT : return string("INET_E_CANNOT_INSTANTIATE_OBJECT") ;
    case INET_E_CANNOT_LOAD_DATA          : return string("INET_E_CANNOT_LOAD_DATA") ;
    case INET_E_CANNOT_LOCK_REQUEST       : return string("INET_E_CANNOT_LOCK_REQUEST") ;
    case INET_E_CANNOT_REPLACE_SFP_FILE   : return string("INET_E_CANNOT_REPLACE_SFP_FILE") ;
    case INET_E_CODE_DOWNLOAD_DECLINED    : return string("INET_E_CODE_DOWNLOAD_DECLINED") ;
    case INET_E_CODE_INSTALL_BLOCKED_BY_HASH_POLICY : return string("INET_E_CODE_INSTALL_BLOCKED_BY_HASH_POLICY") ;
    case INET_E_CODE_INSTALL_SUPPRESSED   : return string("INET_E_CODE_INSTALL_SUPPRESSED") ;
    case INET_E_CONNECTION_TIMEOUT        : return string("INET_E_CONNECTION_TIMEOUT") ;
    case INET_E_DATA_NOT_AVAILABLE        : return string("INET_E_DATA_NOT_AVAILABLE") ;
    case INET_E_DOWNLOAD_FAILURE          : return string("INET_E_DOWNLOAD_FAILURE") ;
    case INET_E_INVALID_CERTIFICATE       : return string("INET_E_INVALID_CERTIFICATE") ;
    case INET_E_INVALID_REQUEST           : return string("INET_E_INVALID_REQUEST") ;
    case INET_E_INVALID_URL               : return string("INET_E_INVALID_URL") ;
    case INET_E_NO_SESSION                : return string("INET_E_NO_SESSION") ;
    case INET_E_NO_VALID_MEDIA            : return string("INET_E_NO_VALID_MEDIA") ;
    case INET_E_OBJECT_NOT_FOUND          : return string("INET_E_OBJECT_NOT_FOUND") ;
    case INET_E_REDIRECT_FAILED           : return string("INET_E_REDIRECT_FAILED") ;
    case INET_E_REDIRECT_TO_DIR           : return string("INET_E_REDIRECT_TO_DIR") ;
    // case INET_E_REDIRECTING               : return string("INET_E_REDIRECTING") ;
    case INET_E_RESOURCE_NOT_FOUND        : return string("INET_E_RESOURCE_NOT_FOUND") ;
    case INET_E_RESULT_DISPATCHED         : return string("INET_E_RESULT_DISPATCHED") ;
    case INET_E_SECURITY_PROBLEM          : return string("INET_E_SECURITY_PROBLEM") ;
    case INET_E_TERMINATED_BIND           : return string("INET_E_TERMINATED_BIND") ;
    case INET_E_UNKNOWN_PROTOCOL          : return string("INET_E_UNKNOWN_PROTOCOL") ;
    case INET_E_USE_EXTEND_BINDING        : return string("INET_E_USE_EXTEND_BINDING") ;
  }
  return string("Unknown error") ;
}

string getNavigateErrorDetailedMsg(int iStatusCode)
{
  switch(iStatusCode)
  {
    case HTTP_STATUS_BAD_GATEWAY       : return string("HTTP_STATUS_BAD_GATEWAY") ;
    case HTTP_STATUS_BAD_METHOD        : return string("HTTP_STATUS_BAD_METHOD") ;
    case HTTP_STATUS_BAD_REQUEST       : return string("HTTP_STATUS_BAD_REQUEST") ;
    case HTTP_STATUS_CONFLICT          : return string("HTTP_STATUS_CONFLICT") ;
    case HTTP_STATUS_DENIED            : return string("HTTP_STATUS_DENIED") ;
    case HTTP_STATUS_FORBIDDEN         : return string("HTTP_STATUS_FORBIDDEN") ;
    case HTTP_STATUS_GATEWAY_TIMEOUT   : return string("HTTP_STATUS_GATEWAY_TIMEOUT") ;
    case HTTP_STATUS_GONE              : return string("HTTP_STATUS_GONE") ;
    case HTTP_STATUS_LENGTH_REQUIRED   : return string("HTTP_STATUS_LENGTH_REQUIRED") ;
    case HTTP_STATUS_NONE_ACCEPTABLE   : return string("HTTP_STATUS_NONE_ACCEPTABLE") ;
    case HTTP_STATUS_NOT_FOUND         : return string("HTTP_STATUS_NOT_FOUND") ;
    case HTTP_STATUS_NOT_SUPPORTED     : return string("HTTP_STATUS_NOT_SUPPORTED") ;
    case HTTP_STATUS_PAYMENT_REQ       : return string("HTTP_STATUS_PAYMENT_REQ") ;
    case HTTP_STATUS_PRECOND_FAILED    : return string("HTTP_STATUS_PRECOND_FAILED") ;
    case HTTP_STATUS_PROXY_AUTH_REQ    : return string("HTTP_STATUS_PROXY_AUTH_REQ") ;
    case HTTP_STATUS_REQUEST_TIMEOUT   : return string("HTTP_STATUS_REQUEST_TIMEOUT") ;
    case HTTP_STATUS_REQUEST_TOO_LARGE : return string("HTTP_STATUS_REQUEST_TOO_LARGE") ;
    case HTTP_STATUS_RETRY_WITH        : return string("HTTP_STATUS_RETRY_WITH") ;
    case HTTP_STATUS_SERVER_ERROR      : return string("HTTP_STATUS_SERVER_ERROR") ;
    case HTTP_STATUS_SERVICE_UNAVAIL   : return string("HTTP_STATUS_SERVICE_UNAVAIL") ;
    case HTTP_STATUS_UNSUPPORTED_MEDIA : return string("HTTP_STATUS_UNSUPPORTED_MEDIA") ;
    case HTTP_STATUS_URI_TOO_LONG      : return string("HTTP_STATUS_URI_TOO_LONG") ;
    case HTTP_STATUS_VERSION_NOT_SUP   : return string("HTTP_STATUS_VERSION_NOT_SUP") ;

    case INET_E_AUTHENTICATION_REQUIRED      : return string("Authentication is needed to access the object") ;
    case INET_E_BLOCKED_REDIRECT_XSECURITYID : return string("A redirect request was blocked because security identifiers (SIDs) do not match and BINDF2_DISABLE_HTTP_REDIRECT_XSECURITYID is set in bind options") ;
    case INET_E_CANNOT_CONNECT               : return string("The attempt to connect to the Internet has failed") ;
    case INET_E_CANNOT_INSTANTIATE_OBJECT    : return string("CoCreateInstance failed") ;
    case INET_E_CANNOT_LOAD_DATA             : return string("The object could not be loaded") ;
    case INET_E_CANNOT_LOCK_REQUEST          : return string("The requested resource could not be locked") ;
    case INET_E_CANNOT_REPLACE_SFP_FILE      : return string("Cannot replace a file that is protected by System File Protection (SFP)") ;
    case INET_E_CODE_DOWNLOAD_DECLINED       : return string("The component download was declined by the user") ;
    case INET_E_CODE_INSTALL_BLOCKED_BY_HASH_POLICY : return string("Installation of ActiveX control (as identified by cryptographic file hash) has been disallowed by registry key policy") ;
    case INET_E_CODE_INSTALL_SUPPRESSED      : return string("The Microsoft Authenticode prompt for installing a Microsoft ActiveX control was not shown because the page restricts the installation of the ActiveX controls. The usual cause is that the Information Bar is shown instead of the Authenticode prompt") ;
    case INET_E_CONNECTION_TIMEOUT           : return string("The Internet connection has timed out") ;
    case INET_E_DATA_NOT_AVAILABLE           : return string("An Internet connection was established, but the data cannot be retrieved") ;
    case INET_E_DOWNLOAD_FAILURE             : return string("The download has failed (the connection was interrupted)") ;
    case INET_E_INVALID_CERTIFICATE          : return string("INET_E_INVALID_CERTIFICATE") ;
    case INET_E_INVALID_REQUEST              : return string("The request was invalid") ;
    case INET_E_INVALID_URL                  : return string("The URL could not be parsed") ;
    case INET_E_NO_SESSION                   : return string("No Internet session was established") ;
    case INET_E_NO_VALID_MEDIA               : return string("The object is not in one of the acceptable MIME types") ;
    case INET_E_OBJECT_NOT_FOUND             : return string("The object was not found") ;
    case INET_E_REDIRECT_FAILED              : return string("Microsoft Win32 Internet (WinInet) cannot redirect. This error code might also be returned by a custom protocol handler") ;
    case INET_E_REDIRECT_TO_DIR              : return string("The request is being redirected to a directory") ;
    // case INET_E_REDIRECTING                : return string("INET_E_REDIRECTING") ;
    case INET_E_RESOURCE_NOT_FOUND           : return string("The server or proxy was not found") ;
    case INET_E_RESULT_DISPATCHED            : return string("The binding has already been completed and the result has been dispatched, so your abort call has been canceled") ;
    case INET_E_SECURITY_PROBLEM             : return string("A security problem was encountered") ;
    case INET_E_TERMINATED_BIND              : return string("Binding was terminated") ;
    case INET_E_UNKNOWN_PROTOCOL             : return string("The protocol is not known and no pluggable protocols have been entered that match") ;
    case INET_E_USE_EXTEND_BINDING           : return string("Reissue request with extended binding") ;
  }
  return string("Unknown error") ;
}

boolNSVisualView::IsTabKeyMessage(MSG &msg)
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

//// Let the form process keystrokes in its own way.  Without
// this method, you can't tab between control on the form.
//
bool
NSVisualView::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;

	if (false == result)
		result = TWindow::PreProcessMsg(msg) ;

	return result ;
}

////
//
void
NSVisualView::MakeVisible()
{
  if (NULL == Form)
    return ;

	Form->Visible = true ;
}

void
NSVisualView::SetURL(const char* _URL)
{
	if (_URL)
	{
  	_sUrl = string(_URL) ;
    if (Form)
    	Form->RetrieveURL(_URL) ;
	}
}

// Fonction GenereHtml////////////////////////////////////////////////////////////////

boolNSVisualView::GenereHtml()
{
  string sPathHtml = _pDocRef->pContexte->PathName("SHTM") ;

  // on récupère le code du document à visualiser
  string codeDoc = _pDocRef->getDocId() ;

  _sHtml = codeDoc ;

  // generation du fichier html (dans le repertoire du serveur)
  if (false == _pDocRef->GenereHtml(sPathHtml, _sHtml, toVisualiser))
  {
  	erreur("Impossible de générer le fichier html à visualiser", standardError, 0, GetHandle()) ;
    _sHtml = string("") ;
    return false ;
  }

  return true ;}

// Fonction initParams()//////////////////////////////////////////////////////////////////////

voidNSVisualView::initParams()
{
  ifstream inFile;

  //int     delai;

  string sFichierTempo = _pDocRef->pContexte->PathName("FPER") + string("tempo.dat") ;
  inFile.open(sFichierTempo.c_str()) ;  if (!inFile)
  {
    NSSuper* pSuper = _pDocRef->pContexte->getSuperviseur() ;

    string sErrorText = pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sFichierTempo ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return ;
  }

  string sData = string("") ;

  while (!inFile.eof())  {
    string sLine = string("") ;
  	getline(inFile, sLine) ;
    if (string("") != sLine)
    	sData += sLine + string(1, '\n') ;
  }

  inFile.close() ;
  size_t iDataLen = strlen(sData.c_str()) ;

  // boucle de chargement des attributs de tempo.dat  //  size_t i = 0 ;  while (i < iDataLen)
  {
  	string sNomAttrib = string("") ;

    while ((i < iDataLen) && (' ' != sData[i]) && ('\t' != sData[i]))    	sNomAttrib += pseumaj(sData[i++]) ;

    while ((i < iDataLen) && ((' ' == sData[i]) || ('\t' == sData[i])))    	i++ ;

    string sValAttrib = string("") ;    while ((i < iDataLen) && ('\n' != sData[i]))
    	sValAttrib += sData[i++] ;

    if      (sNomAttrib == "TEMPO")    	delai = atoi(sValAttrib.c_str()) ;
    else if (sNomAttrib == "HOOK")
      bUseHook = IsNotNo(sValAttrib) ;
    else if (sNomAttrib == "VERBOSE")
      bVerbose = IsNotNo(sValAttrib) ;
    else if (sNomAttrib == "KILLTMP")
      bKillTmp = IsNotNo(sValAttrib) ;
    else if (sNomAttrib == "WAITIMP")
      bWaitImp = IsNotNo(sValAttrib) ;
    else if (sNomAttrib == "SIMPLECTRL")
      bSimpleCtrl = IsNotNo(sValAttrib) ;
    else if (sNomAttrib == "TEMPOSTNAV")
    	iTempPostNav = atoi(sValAttrib.c_str()) ;
    else if (sNomAttrib == "TEMPIMP")
    	iTempImp = atoi(sValAttrib.c_str()) ;

    i++ ;  }
}

voidNSVisualView::Tempo()
{
  NSSuper* pSuper = _pDocRef->pContexte->getSuperviseur() ;

	if (((NSSuper*) NULL == pSuper) || (0 == delai))
  	return ;

	// application du délai (unité : centième de seconde)
  pSuper->Delay(delai) ;
}

// Fonction ImprimerLettre////////////////////////////////////////////////////////////////
bool
NSVisualView::ImprimerLettre()
{
try
{
	NSDocumentInfo* pDocTtxInfo = 0 ;
	// NSTtxDocument*  pDocTtx ;
	// string			sFichHtml;
	string sPathHtml = _pDocRef->pContexte->PathName("SHTM") ;

	if (string("") == _pDocRef->getCodeLettre())
		return false ;

  NSPersonGraphManager* pGraphManager = _pDocRef->pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
  	return false ;

  NSDocumentInfo docInfo(_pDocRef->getCodeLettre(), _pDocRef->pContexte, pGraphManager) ;
	bool bGetRawDoc = docInfo.InitDocumentBrut(&pDocTtxInfo) ;
  if (false == bGetRawDoc)
    return false ;

	NSTtxDocument DocTtx(0, true, pDocTtxInfo, 0, _pDocRef->pContexte) ;	if (DocTtx.IsOpen())
	{
		if (!_pDocRef->TemplateCompo(pDocTtxInfo->getTypeSem(), DocTtx._sTemplate, DocTtx._sEnTete))
    	return false ;
  }
  DocTtx._sTemplate = _pDocRef->pContexte->PathName("NTPL") + DocTtx._sTemplate ;  DocTtx._sEnTete   = _pDocRef->pContexte->PathName("NTPL") + DocTtx._sEnTete ;
  _sLettreHtml      = _pDocRef->getCodeLettre() ;

	if (false == DocTtx.GenereHtml(sPathHtml, _sLettreHtml, toImprimer, _pDocRef->getPubli()->_sAdresseCorresp))	{
  	erreur("Impossible de créer le fichier html à imprimer pour la lettre", standardError, 0, GetHandle()) ;
    return false ;
	}

	_pDocRef->getPubli()->_sLettreHtml[_pDocRef->getPubli()->_indexLettre] = _sLettreHtml ;
}
catch (...)
{
	erreur("Exception NSVisualView::ImprimerLettre.", standardError, 0) ;
	return false ;
}

  // on navigue vers le fichier html à visualiser
	string ps = string("Navigate to letter url: ") + _sUrl + _sLettreHtml ;
	_pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

try
{
  Navigate(_sUrl + _sLettreHtml) ;
}
catch (Exception &ex)
{
  string ps = string("Exception NSVisualView::ImprimerLettre Navigate when navigating to ") + _sUrl + _sLettreHtml + string(" (") + string(ex.Message.c_str()) + string(")") ;
  _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  return true ;
}
catch (...)
{
  string ps = string("Exception NSVisualView::ImprimerLettre Navigate when navigating to ") + _sUrl + _sLettreHtml ;
  _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  // ----- Switched off because it can get thrown "at random" and block printing
	// erreur("Exception NSVisualView::ImprimerLettre, Navigate.", standardError, 0) ;
	// return false ;

  return true ;
}

	return true ;
}

// Fonction de destruction des fichiers temporaires////////////////////////////////////////////////////////////////
void
NSVisualView::DetruireTemp()
{
  NSSuper* pSuper = _pDocRef->pContexte->getSuperviseur() ;

	// destruction des fichiers temporaires de visualisation
	if (string("") != _sHtml) // si un fichier a été généré
	{
		// destruction du fichier de visualisation
    string sFichVisual = _pDocRef->pContexte->PathName("SHTM") + _sHtml ;

    string sTrace = string("Deleting temporary file ") + sFichVisual ;
    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

    NsDeleteTemporaryFile(pSuper, sFichVisual) ;
	}

	if (string("") != _sLettreHtml) // si un fichier a été généré
	{
		// destruction du fichier de visualisation
    string sFichVisual = _pDocRef->pContexte->PathName("SHTM") + _sLettreHtml ;

    string sTrace = string("Deleting temporary file ") + sFichVisual ;
    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

    NsDeleteTemporaryFile(pSuper, sFichVisual) ;
	}

	// ne pas oublier, sinon bug du destructeur
	_pDocRef = 0 ;
}

// Fonction de rafraichissement de la vue////////////////////////////////////////////////////////////////
void
NSVisualView::Rafraichir()
{
try
{
  // destruction du fichier temporaire de visualisation
  if (string("") != _sHtml) // si un fichier a été généré
  {
    // destruction du fichier de visualisation
    string sFichVisual = _pDocRef->pContexte->PathName("SHTM") + _sHtml ;

    string sTrace = string("Deleting temporary file ") + sFichVisual ;
    _pDocRef->pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

    NsDeleteTemporaryFile(_pDocRef->pContexte->getSuperviseur(), sFichVisual) ;
  }
  else
    return ;     // cas d'un fichier externe

  // Destruction de la base d'images du document
  // (pour pouvoir recharger la nouvelle)
  if (string("") != _pDocRef->_sBaseImages)  // si la base a été initialisée
  {
    NSBaseImages BaseImg(_pDocRef->_sBaseImages) ;
    BaseImg.lire() ;
    BaseImg.detruire() ;
    _pDocRef->_sBaseImages = string("") ;
  }

  // on regénère le fichier
  if (false == GenereHtml())
  {
    erreur("Erreur : Impossible de rafraichir la vue...", standardError, 0, GetHandle()) ;
    return ;
  }

  // on navigue vers le fichier html à visualiser
  Navigate(_sUrl + _sHtml) ;
}
catch (Exception &ex)
{
  string ps = string("Exception NSVisualView::Rafraichir (") + string(ex.Message.c_str()) + string(")") ;
  _pDocRef->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
}
catch (...)
{
  erreur("Exception NSVisualView::Rafraichir.", standardError, 0) ;
}
}

// Fonction de réponse à la commande Imprimer (publier)////////////////////////////////////////////////////////////////

voidNSVisualView::CmPublier()
{
try
{
  NSSuper* pSuper = _pDocRef->pContexte->getSuperviseur() ;

  if (NULL == _pDocRef->_pDocInfo)	{
  	string sWarningText = pSuper->getText("documentManagement", "youMustSaveBeforePublishing") ;
		MessageBox(sWarningText.c_str(), 0, MB_OK) ;
		return ;
	}

	// on traite à part le cas des documents de type texte (TWHTM, TNRTF)	// ou les documents codés (CNxxx, CSxxx)
	// qui sont en visualisation (cas nbImpress == 0).
	// Pour ce type de documents, on lance ici le processus de publication
	// qui rappelera la VisualView avec nbImpress > 0, ce qui permettra d'imprimer

	if ((0 == _pDocRef->getNbImpress()) && // (NULL != pDocRef->pDocInfo) &&
    	((_pDocRef->pContexte->typeDocument(_pDocRef->_pDocInfo->getTypeSem(), NSSuper::isTree)) ||
			 (_pDocRef->pContexte->typeDocument(_pDocRef->_pDocInfo->getTypeSem(), NSSuper::isText))))
	{
		// on vérifie d'abord si le document n'est pas en cours d'impression

		if ((_pDocRef->getPubli()) && (_pDocRef->getPubli()->ImpressionEnCours()))			return ;

		_pDocRef->Publier() ; // publication avec correspondants		return ;
	}

	// Ici commence le processus d'impression de la page. Pour les documents	// issus de la publication (nbImpress != 0), CmPublier est appelé par
	// NavigateComplete. Les autres documents s'impriment en un exemplaire
	// Ajout RS du 09/04/04 : adaptation avec WebBrowserPrint de ExecWb

	if (0 == _pDocRef->getNbImpress())
	{
		if (bSimpleCtrl)
    {
    	if (bVerbose)
      	ExecWB_old(Shdocvw_tlb::OLECMDID_PRINT, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT) ;
      else
      	ExecWB_old(Shdocvw_tlb::OLECMDID_PRINT, Shdocvw_tlb::OLECMDEXECOPT_DONTPROMPTUSER) ;
    }
    else
    	ExecWB(Shdocvw_tlb::OLECMDID_PRINT, 1, bVerbose) ;
	}
	else if (_pDocRef->getNbImpress() > 0)
	{
		if (bSimpleCtrl)
    {
    	for (int i = 0; i < _pDocRef->getNbImpress(); i++)
      {
      	if (bVerbose)
        	ExecWB_old(Shdocvw_tlb::OLECMDID_PRINT, Shdocvw_tlb::OLECMDEXECOPT_DODEFAULT) ;
        else
        	ExecWB_old(Shdocvw_tlb::OLECMDID_PRINT, Shdocvw_tlb::OLECMDEXECOPT_DONTPROMPTUSER) ;
      }
    }
    else
    	ExecWB(Shdocvw_tlb::OLECMDID_PRINT, _pDocRef->getNbImpress(), bVerbose) ;
	}
	else
	{
		erreur("Le nombre d'exemplaires spécifié pour l'impression est négatif.", standardError, 0) ;
    return ;
	}

/*
    IDispatch* pdisp = Form->Control->Document;    IOleCommandTarget* command;
    pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command);

    ps = "Appel de la commande d'impression d'Internet Explorer...\n\n";    pDocRef->pContexte->getSuperviseur()->trace(&ps,1);

    if(command)    {
        if (!pDocRef->nbImpress)
        {
            if (bVerbose)
                command->Exec(NULL, Shdocvw_tlb::OLECMDID_PRINT, NULL, NULL, NULL);
            else
                command->Exec(NULL, Shdocvw_tlb::OLECMDID_PRINT, Shdocvw_tlb::OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
        }
        else
        {
   		    for (int i = 0; i < pDocRef->nbImpress; i++)
            {
                if (bVerbose)
      		        command->Exec(NULL, Shdocvw_tlb::OLECMDID_PRINT, NULL, NULL, NULL);
                else
                    command->Exec(NULL, Shdocvw_tlb::OLECMDID_PRINT, Shdocvw_tlb::OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
            }
        }

        command->Release();    }

    pdisp->Release();*/
  string ps ;

	if (_pDocRef->getNbImpress())	{
    bool bCallNextPrint = true ;

  	if (_pDocRef->getImprimerLettre())
    {
      bCallNextPrint = false ;

    	ps = "Appel de ImprimerLettre..." ;
      pSuper->trace(&ps,1) ;

      _pDocRef->setImprimerLettre(false) ;      if (false == ImprimerLettre())
        bCallNextPrint = true ;
    }
    if (bCallNextPrint)
    {
    	// On envoie le Navigate du corresp suivant ou fin (bFinish == true)

      ps = "Appel de ImprimerSuivant..." ;      pSuper->trace(&ps,1) ;

      bool bFinish = _pDocRef->getPubli()->ImprimerSuivant() ;
      // ici on fait appel à une fonction de synchronisation      if (bFinish)
      {
      	_pDocRef->getPubli()->FinImpression() ;

        // on n'affiche pas la MessageBox de fin d'impression        // pour certains documents (tous les documents de compta pour l'instant)
        // (résoud bug dans l'impression des AGA en série)

        if (string("CP") != string(_pDocRef->_pDocInfo->getTypeSem(), 0, 2))        {
        	ps = "Impression terminée." ;
          pSuper->trace(&ps,1) ;

          string sMsgText = pSuper->getText("printing", "printingDone") ;
          MessageBox(sMsgText.c_str(), 0, MB_OK) ;          _pDocRef->setNbImpress(0) ;
          _pDocRef->setNomDocHtml(string("")) ;   // variable de nsdocnoy
          _pDocRef->getPubli()->DeleteVisualViews() ;
        }
      }
    }
	}
}
catch (...)
{
	erreur("Exception NSVisualView::CmPublier.", standardError, 0) ;
}
}
// Fonction de réponse à la commande Composer////////////////////////////////////////////////////////////////
void
NSVisualView::CmComposer()
{
  if (_pDocRef->_pDocInfo)
  {
    // if (!strcmp(pDocRef->pDocInfo->pDonnees->type, "HIHTM"))
      _pDocRef->Composer() ;
    // else
      // erreur("Ce type de document ne peut etre composé en visualisation.", standardError, 0, GetHandle());
  }
}

// Fonction de réponse à la commande Editer (du menu click-droit)
// on crée un bouton Editer car impossible de récupérer le click
// à cause du controle TWebBrowser qui fait écran
/////////////////////////////////////////////////////////////////
void
NSVisualView::CmEditer()
{
  NSHISTODocument* pDocManager = getDocManager() ;
  if ((NSHISTODocument*) NULL == pDocManager)
    return ;

	::SetCursor(::LoadCursor(NULL, IDC_WAIT)) ;
	pDocManager->AutoriserEdition(_pDocRef->_pDocInfo) ;
	::SetCursor(::LoadCursor(NULL, IDC_ARROW)) ;
}

// Fonction de réponse à la commande CM_FILECLOSE////////////////////////////////////////////////////////////////
void
NSVisualView::CmFileClose()
{
	DetruireTemp() ;

	// On détruit dans ce cas la partie NSRefDocument avant de détruire NSVisualView
	GetApplication()->GetDocManager()->CmFileClose() ;
}

// Fonctions de navigation de la VisualView////////////////////////////////////////////////////////////////
void
NSVisualView::CmPrecedent()
{
	if (_page > 1)
  {
    _bNewNav = false ;
		Form->Control->GoBack() ;
    _page-- ;
  }
}

voidNSVisualView::CmSuivant()
{
  if (_page < _nbNav)
  {
    _bNewNav = false ;
		Form->Control->GoForward() ;
    _page++ ;
  }
}

void
NSVisualView::openPrevSameKindDocument()
{
  NSDocumentInfo* pDocInfo = getPrevSameKindDocument() ;
  if ((NSDocumentInfo*) NULL == pDocInfo)
    return ;

  NSHISTODocument* pDocManager = getDocManager() ;
  if ((NSHISTODocument*) NULL == pDocManager)
    return ;

  pDocManager->AutoriserOuverture(pDocInfo) ;
}

void
NSVisualView::openNextSameKindDocument()
{
  NSDocumentInfo* pDocInfo = getNextSameKindDocument() ;
  if ((NSDocumentInfo*) NULL == pDocInfo)
    return ;

  NSHISTODocument* pDocManager = getDocManager() ;
  if ((NSHISTODocument*) NULL == pDocManager)
    return ;

  pDocManager->AutoriserOuverture(pDocInfo) ;
}

NSHISTODocument*
NSVisualView::getDocManager()
{
  if ((NULL == _pDocRef) || (NULL == _pDocRef->pContexte))
    return (NSHISTODocument*) 0 ;

  NSPatientChoisi* pPatient = _pDocRef->pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatient)
    return (NSHISTODocument*) 0 ;

  return pPatient->getDocHis() ;
}

NSDocumentInfo*
NSVisualView::getPrevSameKindDocument()
{
  NSHISTODocument* pDocManager = getDocManager() ;
  if ((NSHISTODocument*) NULL == pDocManager)
    return (NSDocumentInfo*) 0 ;

  NSPatPathoArray PptDocRef(_pDocRef->pContexte->getSuperviseur()) ;
  _pDocRef->initFromPatPatho(&PptDocRef) ;
  if (PptDocRef.empty())
    return (NSDocumentInfo*) 0 ;

  DocumentIter iterDoc = pDocManager->TrouveDocHisto(_pDocRef->getDocId()) ;
  if ((DocumentIter) NULL == iterDoc)
    return (NSDocumentInfo*) 0 ;

  string sLexique = (*(PptDocRef.begin()))->getLexique() ;
  DocumentIter iterPrevDoc = pDocManager->DonnePrevPatPathoDocument(sLexique, (NSPatPathoArray*) 0, iterDoc) ;
  if ((DocumentIter) NULL == iterPrevDoc)
    return (NSDocumentInfo*) 0 ;

  return (NSDocumentInfo*) *iterPrevDoc ;
}

NSDocumentInfo*
NSVisualView::getNextSameKindDocument()
{
  NSHISTODocument* pDocManager = getDocManager() ;
  if ((NSHISTODocument*) NULL == pDocManager)
    return (NSDocumentInfo*) 0 ;

  NSPatPathoArray PptDocRef(_pDocRef->pContexte->getSuperviseur()) ;
  _pDocRef->initFromPatPatho(&PptDocRef) ;
  if (PptDocRef.empty())
    return (NSDocumentInfo*) 0 ;

  DocumentIter iterDoc = pDocManager->TrouveDocHisto(_pDocRef->getDocId()) ;
  if ((DocumentIter) NULL == iterDoc)
    return (NSDocumentInfo*) 0 ;

  string sLexique = (*(PptDocRef.begin()))->getLexique() ;
  DocumentIter iterNextDoc = pDocManager->DonneNextPatPathoDocument(sLexique, (NSPatPathoArray*) 0, iterDoc) ;
  if ((DocumentIter) NULL == iterNextDoc)
    return (NSDocumentInfo*) 0 ;

  return (NSDocumentInfo*) *iterNextDoc ;
}

//
//

/******************************************************************************/
//					METHODES DE Document / Vue pour Modèles HTML
/******************************************************************************/

// Constructeur NSHtmlModelDocument////////////////////////////////////////////////////////////////

NSHtmlModelDocument::NSHtmlModelDocument(TDocument* parent, bool bROnly, NSDocumentInfo* pDocumentInfo,                             NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
                    :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
  _sActiveFileName = string("") ;
  _sModelFileName  = string("") ;
	_sDocTitle       = string("") ;
	_sDocLexique     = string("") ;
	_bTemporaryDoc   = false ;

	Open(0, "") ;
}

NSHtmlModelDocument::NSHtmlModelDocument(TDocument *parent, NSContexte *pCtx)                    :NSRefDocument(parent, pCtx)
{
  _sActiveFileName = string("") ;
  _sModelFileName  = string("") ;
	_sDocTitle       = string("") ;
	_sDocLexique     = string("") ;
	_bTemporaryDoc   = true ;

	Open(0, "") ;
}

NSHtmlModelDocument::~NSHtmlModelDocument()
{
	// If the file is still a temporary file when closing, then delete it
  //
	if ((true == _bTemporaryDoc) && (string("") != _sActiveFileName))
	{
    string sTrace = string("Deleting temporary file ") + _sActiveFileName ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

    NsDeleteTemporaryFile(pContexte->getSuperviseur(), _sActiveFileName) ;
	}
}

void
NSHtmlModelDocument::Visualiser()
{
	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(this, "Html Format") ;
}

void
NSHtmlModelDocument::SetModelFile(string sFich)
{
	if (string("") != _sActiveFileName)
		return ;

	_sModelFileName = sFich ;
	replaceTagsByValues() ;
}

stringNSHtmlModelDocument::GetDocExt(){
	size_t i = 0 ;
	for ( ; (i < strlen(_sActiveFileName.c_str())) && (_sActiveFileName[i] != '.'); i++) ;

  string sExt = string("") ;

  if (i < strlen(_sActiveFileName.c_str()))
  {
  	// nomDoc[i] == '.'
    i++ ;
    while (i < strlen(_sActiveFileName.c_str()))
    	sExt += string(1, _sActiveFileName[i++]) ;
  }

  return sExt ;
}

void
NSHtmlModelDocument::replaceTagsByValues()
{
	ifstream inFile ;
	inFile.open(_sModelFileName.c_str(), ios::binary) ;
  if (!inFile)
    return ;

  string sPathTxt = pContexte->PathName("NTTX") ;
  // on trouve le nom du fichier temporaire à visualiser

  string sFileBase = string("tmp") ;
	sFileBase += pContexte->getPatient()->getNss() ;
  string sFileName = sPathTxt + ::nomSansDoublons(sPathTxt, sFileBase, "htm") ;

  ofstream outFile ;
  outFile.open(sFileName.c_str(), ios::binary) ;
	if (!outFile)
  {
  	inFile.close() ;
    return ;
  }

  map<string, string> tagsBuffer ;

  // If a Tag is similar to the previous one, and the PatPatho is made of
  // more than one element, we answer with next element
  //
  string          sLastTag = "" ;
  string          sLastResult = "" ;
	NSPatPathoArray lastPatPathoArray(pContexte->getSuperviseur()) ;
  // PatPathoIter    iterPremier ;

  int iChar = inFile.get() ;

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
              	sBalise += string("$") + string(1, char(iChar)) ;
            }
          }
          else
          	sBalise += string(1, char(iChar)) ;
        }
        //
        // Traiter la balise
        //
        string sBeginner = string("<!-- startNStag ") + sBalise + string(" -->") ;
        string sEnder    = string("<!-- endNStag ") + sBalise + string(" -->") ;

        if ((strlen(sBalise.c_str()) > 8) && (string("docTitle") == string(sBalise, 0, 8)))
      	{
      		string sDocTitle = string(sBalise, 8, strlen(sBalise.c_str()) - 8) ;
        	strip(sDocTitle) ;
        	SetDocTitle(sDocTitle) ;
      	}
      	else if (string("nomPatient") == sBalise)
      		sBalise = pContexte->getPatient()->getNomLong() ;
      	else if (string("dateJour") == sBalise)
      	{
      		string sLang = pContexte->getUserLanguage() ;
        	sBalise = donne_date(donne_date_duJour(), sLang) ;
      	}
      	else if (string("") != sBalise)
        {
          NSPatPathoArray* pPatPathoArray = NULL ;
        	pContexte->getPatient()->remplaceTagsChemin(sBalise, pPatPathoArray, &tagsBuffer) ;
          if (pPatPathoArray)
          	delete pPatPathoArray ;
        }

        sBalise = sBeginner + sBalise + sEnder ;

        if (string("") != sBalise)
        	for (size_t i = 0 ; i < strlen(sBalise.c_str()) ; i++)
          	outFile.put(sBalise[i]) ;
      }
      else
      {
      	outFile.put('[') ;
      	outFile.put(char(iChar)) ;
      }
    }
    else
    	outFile.put(char(iChar)) ;

    iChar = inFile.get() ;
  }
  inFile.close() ;
  outFile.close() ;

  _sActiveFileName = sFileName ;

  SetDirty(true) ;

  return ;
}

// Ouverture du document
////////////////////////////////////////////////////////////////

boolNSHtmlModelDocument::Open(int /*mode*/, LPCSTR path){
	string sNomFichier = "" ;

	// Si il existe, prise du nom de fichier de la fiche document  ValideFichier(&sNomFichier) ;
	// Sinon, on sort
	if (false == _bDocumentValide)
		return false ;

	//	// Rangement du nom de fichier dans le document
	//
	_sActiveFileName = sNomFichier ;
  SetDirty(false) ;
  _bTemporaryDoc = false ;
  return true ;
}

// Fermeture du document////////////////////////////////////////////////////////////////
bool NSHtmlModelDocument::Close()
{
  // if (TDocument::Close())
  // 	 return false;

  return true ;
}

// Génération du html correspondant au document
////////////////////////////////////////////////////////////////
bool
NSHtmlModelDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	return true ;
}

bool
NSHtmlModelDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  return true ;
}

// Indique si document ouvert (il existe un fichier associé)////////////////////////////////////////////////////////////////
bool
NSHtmlModelDocument::IsOpen()
{
	return (string("") != _sActiveFileName) ;
}

bool
NSHtmlModelDocument::SaveNewFile(){  if (string("") == _sActiveFileName)
		return false ;

	string sCurrentFileName = _sActiveFileName ;

  string sNewFileName ;
  bool bRenommerOk = Renommer(sNewFileName) ;
  if (false == bRenommerOk)  	return false ;
  // Copying the temporary file, with the new name
  //
  if (!CopyFile(sCurrentFileName.c_str(), _sActiveFileName.c_str(), true))
  {  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") ;    sErrorText += string(" ") + sCurrentFileName + string(" -> ") + _sActiveFileName ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;  	_sActiveFileName = sCurrentFileName ;    return false ;  }

	if (Referencer("ZSHTM", "", sNewFileName /* _sActiveFileName */, _sPathCode))
  {    SetDirty(false) ;    _bTemporaryDoc = false ;  }  else
  {
  	SetDirty(true) ;
    _bTemporaryDoc = true ;
  }

  return true ;}

bool
NSHtmlModelDocument::SaveExistingFile()
{
	return false ;
}

// Trouve un nom de fichier nautilus au document
////////////////////////////////////////////////////////////////
bool
NSHtmlModelDocument::Renommer(string &sNewFileName)
{
  string sExt = GetDocExt() ;

  if (!TrouveNomFichier("ZSHTM", sExt, sNewFileName, _sPathCode))
  {
    erreur("Pb à l'attribution d'un nouveau nom pour ce fichier", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  string sPath = pContexte->PathName(_sPathCode) ;

  // on construit le nom complet pour la sauvegarde
  _sActiveFileName = sPath + sNewFileName ;

  SetDirty(true) ;

  return true ;
}

// Table de réponses de la classe NSHtmlModelView
////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE1(NSHtmlModelView, NSVisualView)
  EV_COMMAND(CM_ENREGISTRE,	CmFileSave),
END_RESPONSE_TABLE;

NSHtmlModelView::NSHtmlModelView(NSHtmlModelDocument& doc, TWindow *parent)
                :NSVisualView(&doc, parent), pDoc(&doc)
{
	pMUEViewMenu = 0 ;

	nsMenuIniter menuIter(_pDocRef->pContexte) ;
  pMUEViewMenu = new OWL::TMenuDescr ;
  menuIter.initMenuDescr(pMUEViewMenu, "menubar_visual") ;
  TMyApp* pMyApp = _pDocRef->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->GetMainWindow()->SetMenuDescr(*pMUEViewMenu) ;

	initValues() ;

  // Make sure that the file will not be deleted by NSVisualView destructor
  bKillTmp = false ;

  if (string("") == pDoc->_sActiveFileName)
  	_sHtml = pDoc->_sModelFileName ;  else  	_sHtml = pDoc->_sActiveFileName ;
}

NSHtmlModelView::~NSHtmlModelView()
{
}

void
NSHtmlModelView::SetupWindow()
{
	TWindowView::SetupWindow() ;

	ModifyStyle(WS_BORDER, WS_CHILD) ;	Form->Show() ;
	MakeVisible() ;
}

void
NSHtmlModelView::CmFileSave()
{
	Enregistrer() ;
}

// Fonction Enregistrer
////////////////////////////////////////////////////////////////
boolNSHtmlModelView::Enregistrer(){	bool bSaveSuccess = pDoc->SaveNewFile() ;

  if (false == bSaveSuccess)
		return false ;

  // Displaying the new file instead of the temporary one
	//	Navigate(pDoc->_sActiveFileName) ;
	// on remet le titre à jour	TWindow::SetDocTitle(pDoc->GetTitle(), 0) ;

  return true ;
}

bool
NSHtmlModelView::CanClose()
{
	if (pDoc->IsDirty())
	{
		string sCaption = string("Message ") + pDoc->pContexte->getSuperviseur()->getAppName().c_str() ;

    string sText ;
		if (pDoc->_pDocInfo)
    	sText = pDoc->pContexte->getSuperviseur()->getText("documentManagement", "doYouWantToSaveChangesForDocument") ;
		else
    	sText = pDoc->pContexte->getSuperviseur()->getText("documentManagement", "doYouWantToSaveDocument") ;
    if (pDoc->GetTitle())
      sText += string(" ") + pDoc->GetTitle() ;
    sText += string(" ?") ;

    int retVal = MessageBox(sText.c_str(), sCaption.c_str(), MB_YESNOCANCEL) ;
		if (IDCANCEL == retVal)
			return false ;
		if (IDYES == retVal)
		{
    	bool okEnreg ;
    	if (pDoc->_pDocInfo)
      	okEnreg = pDoc->SaveExistingFile() ;
      else
				okEnreg = pDoc->SaveNewFile() ;

			if (false == okEnreg)
				return false ;
		}
	}
	pDoc->SetDirty(false);

	TMyApp* pMyApp = _pDocRef->pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;
	_pEditUrl = (NSEditUrl*) 0 ;
	bSetupToolBar = false ;

	return true ;}

TWindow*
NSHtmlModelView::GetWindow()
{
	return (TWindow*) this ;
}

// Classe NSEditUrl pour la barre d'edition des url
////////////////////////////////////////////////////////////////

DEFINE_RESPONSE_TABLE1(NSEditUrl, NSEditUrlBase)
	EV_WM_CHAR,
END_RESPONSE_TABLE;

NSEditUrl::NSEditUrl(NSVisualView* pView)
          :NSEditUrlBase((NSVisualViewBase*) pView)
{
	_pVue = pView ;
}

NSEditUrl::~NSEditUrl()
{
}

void
NSEditUrl::EvChar(uint key, uint repeatCount, uint flags)
{
	if (VK_RETURN == key)
  {
  	char* url = new char[GetTextLen() + 2] ;
  	GetText(url, GetTextLen() + 1) ;
    _pVue->Navigate(string(url)) ;
    delete[] url ;
  }
  else
  	OWL::TEdit::EvChar(key, repeatCount, flags) ;
}

// fin de nsvisual.cpp/////////////////////////////////////////////////////////////////


