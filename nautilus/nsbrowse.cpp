// NSBROWSE.CPP : Dialogues de composition et de publication
////////////////////////////////////////////////////////////
#include <cstring.h>#include <stdio.h>#include <assert.h>/** Includes spécifiques capture **/#include <string.h>#include <malloc.h>#include <windows.h>#include <wingdi.h>#include <mshtml.h>///////////////////////////////////
#include <owl\clipboar.h>

#include "Compos.h"#include "Import.h"#include "ImportPdf.h"#include "ImpImg.h"#include "Lettre.h"#include "log_form.h"#include "WebService.h"#include "nautilus\nsbrowse.h"#include "nautilus\nsbasimg.h"#include "nautilus\nsdocref.h"#include "nautilus\nautilus.rh"#include "nautilus\nssuper.h"#include "nautilus\nshistdo.h"#include "nautilus\nsmodhtm.h"#include "nautilus\ns_html.h"#include "nautilus\nsvisual.h"#include "nautilus\nsresour.h"#include "nautilus\nsdocview.h"#include "nautilus\nscsdoc.h"// #include "nautilus\nsannexe.h"#include "ns_grab\nsgrabfc.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsattvaltools.h"
#include "nssavoir\nsfilecaptur.h"
#include "nautilus\nsGrabObjects.h"
#include "nsepisod\nsclasser.h"
#include "partage\nsdivfile.h"

#include "nautilus\nsepicap.h"
#include "nautilus\nsacroread.h"

#define  ID_PDF_IMPORT_TIMER 101

/****************** classe NSComposView **************************/
DEFINE_RESPONSE_TABLE1(NSComposView, TWindowView)  EV_WM_CLOSE,
  EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;

// Constructeur NSComposView
////////////////////////////////////////////////////////////////
NSComposView::NSComposView(NSRefDocument& doc, TWindow *parent)
	           :TWindowView(doc,parent), pDocBrut(&doc), Form(0)
{
  TMyApp* pMyApp = pDocBrut->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->setMenu(string("menubar"), &hAccelerator) ;
}

// Destructeur NSComposView
////////////////////////////////////////////////////////////////
NSComposView::~NSComposView()
{
	// Delete de la Form
	delete Form ;
	// CoUninitialize() ;

  if (0 != hAccelerator)
  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }
}

// GetWindow renvoie this
////////////////////////////////////////////////////////////////
TWindow*
NSComposView::GetWindow()
{
	return (TWindow*) this ;
}

void
NSComposView::EvClose()
{
	TWindow::EvClose() ;
}

//// Override a TWindow virtual to create the HWND directly.
// NSComposView and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSComposView::PerformCreate(int /*menuOrId*/)
{
try
{
	//CoInitialize(NULL) ;
	Form = new TWebCompos(Parent->GetHandle(), this) ;
	Form->Visible = false ;
	Form->ParentWindow = Parent->HWindow ;
	SetHandle(Form->Handle) ;
	::SetParent(Forms::Application->Handle, pDocBrut->pContexte->GetMainWindow()->HWindow) ;

	SetDocTitle(pDocBrut->GetTitle(), 0) ;

	// on navigue vers le fichier html à composer
	Navigate(pDocBrut->getUrlHtml()) ;
}
catch (Exception &ex)
{
  string ps = string("Exception à la création de la forme de composition (") + string(ex.Message.c_str()) + string(")") ;
  pDocBrut->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception à la création de la forme de composition.", standardError, 0) ;
}
}

boolNSComposView::IsTabKeyMessage(MSG &msg)
{
	if (GetCapture() == NULL)
	{
  	if ((WM_KEYDOWN == msg.message) || (WM_KEYUP == msg.message))
    {
    	if (VK_TAB == msg.wParam)
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
NSComposView::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;
	if (result)
    return true ;

	PRECONDITION(GetHandle()) ;
  return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;
}

// Fonction MakeVisible
void
NSComposView::MakeVisible()
{
	Form->Visible = true ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSComposView::SetupWindow()
{
	TWindowView::SetupWindow() ;

	// fichiers d'aide

	pDocBrut->pContexte->setAideIndex("") ;  pDocBrut->pContexte->setAideCorps("Les_Fiches_de.html") ;
  ModifyStyle(WS_BORDER, WS_CHILD) ;
  Form->Show() ;
  MakeVisible() ;
}

// Fonction Navigate
////////////////////////////////////////////////////////////////
void
NSComposView::Navigate(string url)
{
  if (string("") == url)
  {
    string ps = string("NSComposView::Navigate with empty URL") ;
	  pDocBrut->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

try
{
	Variant  Flags(navNoReadFromCache) ;
	TVariant VFlags = Flags.operator TVariant() ;

	fichCompo = pDocBrut->pContexte->PathName("SHTM") + url ;
	url = pDocBrut->pContexte->PathName("UHTM") + url ;

  wchar_t buff[1024] ;
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, url.c_str(), -1, buff, sizeof(buff)) ;
	Form->Control->Navigate(buff,&VFlags) ;
}
catch (Exception &ex)
{
  string ps = string("NSComposView::Navigate (") + string(ex.Message.c_str()) + string(")") ;
  pDocBrut->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
}
catch (...)
{
  string ps = string("NSComposView::Navigate Exception") ;
  pDocBrut->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
}
}

void
NSComposView::NavigateErrorEvent(int iStatusCode, string sURL)
{
	string ps = string("Ev : NavigateError for URL \"") + sURL + string("\"") ;
  ps += string(" (") + getNavigateErrorShortMsg(iStatusCode) + string(")") ;
	pDocBrut->pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  erreur(ps.c_str(), standardError, 0) ;
}

voidNSComposView::CmOk()
{
	string fichTmpl   = pDocBrut->_sTemplate ;
	string enteteTmpl = pDocBrut->_sEnTete ;

	size_t pos = fichTmpl.find_last_of('\\') ;
	if (NPOS != pos)
		fichTmpl = string(fichTmpl, pos+1, strlen(fichTmpl.c_str())-pos-1) ;

	pos = enteteTmpl.find_last_of('\\') ;
	if (NPOS != pos)
		enteteTmpl = string(enteteTmpl, pos+1, strlen(enteteTmpl.c_str())-pos-1) ;

	string sDroits ;
  NSPersonGraphManager* pManager = pDocBrut->pContexte->getPatient()->getGraphPerson() ;

  NSDocumentInfo *pDocInfoBrut = pDocBrut->_pDocInfo ;

  // on met les infos de template et d'en-tete dans le graphe
	pManager->setTemplatePres(pDocBrut->pContexte, pDocInfoBrut->getCodeDocPres(), fichTmpl, enteteTmpl) ;

  // on met à jour pDocInfo
  NSPatPathoArray PptPres(pDocBrut->pContexte->getSuperviseur()) ;
  pManager->getTree(pDocInfoBrut->getCodeDocPres(), &PptPres, &sDroits) ;
  pDocInfoBrut->setPres(&PptPres) ;

  // écriture des composants image
  pDocBrut->DetruireComposants(&PptPres) ;
  pDocBrut->EcrireComposants(&PptPres) ;
  pDocInfoBrut->setPres(&PptPres) ;

  // enregistrement des composants et mise à jour de pDocInfo
  pDocInfoBrut->initFromPres(&PptPres) ;
  pManager->setTree(&PptPres, "", pDocInfoBrut->getCodeDocPres()) ;

  pManager->commitGraphTree(pDocBrut->pContexte, pDocInfoBrut->getCodeDocPres()) ;

  pManager->getTree(pDocInfoBrut->getCodeDocPres(), &PptPres, &sDroits) ;
  pDocInfoBrut->setPres(&PptPres) ;

  pDocInfoBrut->ParseMetaDonnees() ;
  pDocBrut->pContexte->getPatient()->getDocHis()->Rafraichir(pDocBrut->_pDocInfo, 0) ;

  // destruction du fichier de composition
  if (string("") != fichCompo)
  {
  	if (false == NsDeleteTemporaryFile(pDocBrut->pContexte->getSuperviseur(), fichCompo))
    	MessageBox("Pb de destruction du fichier de composition", 0, MB_OK) ;
  }

  // on rafraichit éventuellement la Visual View
  // pour montrer la nouvelle composition
  for (TView* pView = pDocBrut->GetViewList(); 0 != pView ; pView = pView->GetNextView())
  {
  	if (!strcmp(pView->GetViewName(), "NSVisualView"))
    {
    	NSVisualView* pVisualView = dynamic_cast<NSVisualView*>(pView) ;
      if (pVisualView)
        pVisualView->Rafraichir() ;
      break ;
    }
  }

	CloseWindow() ;
}

voidNSComposView::CmCancel()
{
	// destruction du fichier de composition
	if (string("") != fichCompo)
	{
  	if (false == NsDeleteTemporaryFile(pDocBrut->pContexte->getSuperviseur(), fichCompo))
    	MessageBox("Pb de destruction du fichier de composition", 0, MB_OK) ;
	}

	// destruction de la base de composition
	if (string("") != pDocBrut->_sBaseCompo)		// si la base a été initialisée
	{
  	NSBaseImages BaseCompo(pDocBrut->_sBaseCompo) ;
    BaseCompo.lire() ;
    BaseCompo.detruire() ;
	}

	CloseWindow() ;
}

/****************** classe NSImportWindow **************************/

DEFINE_RESPONSE_TABLE1(NSImportWindow, TWindow)
   EV_WM_CLOSE,
   EV_WM_SIZE,
   EV_WM_TIMER,
END_RESPONSE_TABLE;

NSImportWindow::NSImportWindow(TWindow* parent, string sFichier, NSContexte* pCtx, bool bAutoMode)
               :TWindow(parent)
{
  _bAutomaticImportMode = bAutoMode ;
  _bImportSuccessful    = false ;

  _sFileName     = sFichier ;

	_sHtml         = string("") ;
  _sPathName     = string("") ;
  _sNomFichier   = string("") ;
  _sExtension    = string("") ;
  _sNomSeul      = string("") ;
  _sTypeNautilus = string("") ;

	_pContexte     = pCtx ;
	_pNewDoc       = (NSRefDocument*) 0 ;
	_bCanClose     = false ;
  _Form          = (TWebImport*) 0 ;
  _PdfForm       = (TPdfImport*) 0 ;
  _hAccelerator  = 0 ;

	_bNavOk = InitInfosFichier() ;

  if (_bNavOk)
    _bNavOk = InitNautilusType() ;

  NSSuper *pSuper = _pContexte->getSuperviseur() ;

	if (_bNavOk && ((string("ZTPDF") != _sTypeNautilus) || pSuper->mustUseIeForPdf()))
		_bNavOk = GenereHtml() ;

  TMyApp* pMyApp = pSuper->getApplication() ;
  if (pMyApp)    pMyApp->setMenu(string("menubar"), &_hAccelerator) ;
}

NSImportWindow::~NSImportWindow()
{
  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  string sMsg = string("NSImportWindow entering destructor") ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

	if (string("") != _sHtml)
    NsDeleteTemporaryFile(pSuper, _sHtml) ;

	if (_pNewDoc)
		delete _pNewDoc ;

	// Delete de la Form
/*
  if (Form)
	{
    sMsg = string("NSImportWindow destructor deleting Form") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

		delete Form ;
		CoUninitialize() ;

    sMsg = string("NSImportWindow destructor Form deleted") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
	}
*/

  if (0 != _hAccelerator)
  {
    sMsg = string("NSImportWindow destructor destroying accelerator") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    DestroyAcceleratorTable(_hAccelerator) ;
    _hAccelerator = 0 ;
  }

  if (_bImportSuccessful && _bAutomaticImportMode)
    NsDeleteTemporaryFile(pSuper, _sFileName) ;

  sMsg = string("NSImportWindow leaving destructor") ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;
}

bool
NSImportWindow::CanClose()
{
  if (false == _bCanClose)
    return false ;

  ClosingPatch() ;

  return true ;
}

void
NSImportWindow::ClosingPatch()
{
  if (string("ZTPDF") == _sTypeNautilus)
  {
    if (false == _pContexte->getSuperviseur()->mustKillPdfProcess())
      return ;

    HWND hOleCtrlWin = NSAcrobatView::GetSonWindow(GetHandle(), 3) ;
    if (hOleCtrlWin)
      NSAcrobatView::TerminateProcess(hOleCtrlWin) ;
  }
}

void
NSImportWindow::EvClose()
{
  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  // Delete de la Form
  if (_Form)
	{
    string sMsg = string("NSImportWindow closing Form") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    _Form->Close() ;

    sMsg = string("NSImportWindow::EvClose deleting Form") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

		delete _Form ;
    _Form = (TWebImport*) 0 ;

    sMsg = string("NSImportWindow::EvClose Form deleted") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    // sMsg = string("TWebImport::Close Uninitializing Ole") ;
    // pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
    // ::OleUninitialize() ;
	}

  // Delete de la Form PDF
  if (_PdfForm)
	{
    string sMsg = string("NSImportWindow closing PDF Form") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    _PdfForm->Close() ;

    sMsg = string("NSImportWindow::EvClose deleting PDF Form") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

		delete _PdfForm ;
    _PdfForm = (TPdfImport*) 0 ;

    sMsg = string("NSImportWindow::EvClose PDF Form deleted") ;
    pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
	}

	TWindow::EvClose() ;
}

//
// Override a TWindow virtual to create the HWND directly.
// NSImportWindow and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSImportWindow::PerformCreate(int /*menuOrId*/)
{
  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  string sMsg = string("NSImportWindow entering PerformCreate ; creating Form TWebImport") ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

/*
try
{
	// CoInitialize(NULL) ;


  HRESULT hOleInitResult = ::OleInitialize(NULL) ;

  bool bInitOk = true ;
  sMsg = string("NSImportWindow Ole initialization returns ") ;
  switch (hOleInitResult)
  {
    case S_OK    : sMsg += "Ok" ; break ;
    case S_FALSE : sMsg += "that COM library is already initialized on this apartment" ; break ;
    case OLE_E_WRONGCOMPOBJ :
      sMsg += "that the versions of COMPOBJ.DLL and OLE2.DLL on your machine are incompatible with each other" ;
      bInitOk = false ;
      break ;
    case RPC_E_CHANGED_MODE :
      sMsg += "that a previous call to CoInitializeEx specified a different concurrency model for this apartment" ;
      bInitOk = false ;
      break ;
  }
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
  if (false == bInitOk)
    erreur(sMsg.c_str(), standardError, 0) ;
}
catch (...)
{
  string ps = string("Exception NSImportWindow::PerformCreate OleInitialize") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
	erreur(ps.c_str(), standardError, 0) ;
}*/
  if ((string("ZTPDF") == _sTypeNautilus) && (false == pSuper->mustUseIeForPdf()))
  {
    string sTrace = string("NSImportWindow::PerformCreate Entering, using Acrobat Reader") ;
    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

    // on crée la Form pour servir de zone client (on lui passe le handle parent)
    // The Form is created as the client (it receaves the Parent handle)
    //

    _PdfForm = new TPdfImport(Parent->GetHandle(), this) ;
    _PdfForm->Visible = true ;
    // _AcrobatForm->ParentWindow = Parent->HWindow ;

    // Give the OWL TWindow object the handle of the Windows object it aliases
    //
    SetHandle(_PdfForm->Handle) ;

    return ;
  }

try
{
	_Form = new TWebImport(Parent->GetHandle(), this) ;
	_Form->Visible = false ;
	_Form->ParentWindow = Parent->HWindow ;
}
catch (...)
{
  string ps = string("Exception NSImportWindow::PerformCreate Form creation") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;
	erreur(ps.c_str(), standardError, 0) ;
}

try
{
	SetHandle(_Form->Handle) ;
	::SetParent(Forms::Application->Handle, _pContexte->GetMainWindow()->HWindow) ;
}
catch (...)
{
  string ps = string("Exception NSImportWindow::PerformCreate SetHandle") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;
	erreur(ps.c_str(), standardError, 0) ;
}

	// on navigue vers le fichier html à importer
try
{
	Navigate() ;
}
catch (Exception &ex)
{
  string ps = string("Exception NSImportWindow::PerformCreate Navigate (") + string(ex.Message.c_str()) + string(")") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
}
}

bool
NSImportWindow::IsTabKeyMessage(MSG &msg)
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
NSImportWindow::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;
	if (result)
    return true ;

  // return TWindow::PreProcessMsg(msg) ;
	PRECONDITION(GetHandle()) ;
  return _hAccelerator ? ::TranslateAccelerator(GetHandle(), _hAccelerator, &msg) : false ;
}

// Fonction MakeVisiblevoid
NSImportWindow::MakeVisible()
{
  if (_Form)
    _Form->Visible = true ;
  if (_PdfForm)
    _PdfForm->Visible = true ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSImportWindow::SetupWindow()
{
	TWindow::SetupWindow() ;

  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  if ((string("ZTPDF") == _sTypeNautilus) && (false == pSuper->mustUseIeForPdf()))
  {
    string sMsg = string("NSImportWindow::SetupWindow showing the PdfForm") ;
    _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    TAcroPDF* pPdfControl = _PdfForm->Control ;
    if ((TAcroPDF*) NULL == pPdfControl)
    {
      string sTrace = string("NSImportWindow::SetupWindow for file. Null Acrobat control, leaving.") ;
      pSuper->trace(&sTrace, 1, NSSuper::trError) ;
      return ;
    }

    string sTrace = string("NSImportWindow::SetupWindow, showing Acrobat Reader Form") ;
    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;    pPdfControl->setShowToolbar(false) ;    if (string("") != _sFileName)      displayFile(_sFileName) ;    _PdfForm->Show() ;    pPdfControl->setZoom(100) ;    pPdfControl->gotoFirstPage() ;    // The file doesn't show if the window is not resized after opening    //    SetTimer(ID_PDF_IMPORT_TIMER, 1000) ;

    return ;
  }

  string sMsg = string("NSImportWindow::SetupWindow showing the Form") ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	ModifyStyle(WS_BORDER, WS_CHILD) ;
	_Form->Show() ;
	MakeVisible() ;
}

void
NSImportWindow::EvTimer(uint timerId)
{
	if (ID_PDF_IMPORT_TIMER != timerId)
    return ;

  string sTrace = string("NSImportWindow::EvTimer woke up.") ;
  _pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  EvSize(SIZE_RESTORED, Parent->GetClientRect().Size()) ;

  KillTimer(ID_PDF_IMPORT_TIMER) ;
}

// keep the control at full window size
//
voidNSImportWindow::EvSize(uint sizeType, NS_CLASSLIB::TSize& size){
	TWindow::EvSize(sizeType, size) ;

  NS_CLASSLIB::TRect clientRect = Parent->GetClientRect() ;

  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  if ((string("ZTPDF") == _sTypeNautilus) && (false == pSuper->mustUseIeForPdf()))
  {
    if ((TPdfImport*) NULL == _PdfForm)
      return ;

    _PdfForm->ClientWidth  = clientRect.Width() ;
    _PdfForm->ClientHeight = clientRect.Height() ;

    _PdfForm->Left   = 0 ;
    _PdfForm->Top    = 0 ;
    _PdfForm->Width  = clientRect.Width() ;
    _PdfForm->Height = clientRect.Height() ;

    // TWindow(_AcrobatForm->ParentWindow).SetWindowPos(NULL, 0, 0, clientRect.Width(), clientRect.Height(), SWP_NOZORDER | SWP_NOMOVE) ;

    TAcroPDF* pPdfControl = _PdfForm->Control ;
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

    _PdfForm->Invalidate() ;

    if (SIZE_RESTORED == sizeType)
      Parent->Invalidate() ;
  }
  else
  {
    if ((TWebImport*) NULL == _Form)
      return ;

    _Form->Left   = 0 ;
    _Form->Top    = 0 ;
    _Form->Width  = clientRect.Width() ;
    _Form->Height = clientRect.Height() ;
    _Form->Invalidate() ;
  }
}

// Official hack from Adobe to have TAcroPDF resize when windows is resized
//
void
NSImportWindow::resizeOleControl(NS_CLASSLIB::TRect clientRect)
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


// Fonction Navigate
////////////////////////////////////////////////////////////////
void
NSImportWindow::Navigate()
{
  if (((TWebImport*) NULL == _Form) || (NULL == _Form->Control))
    return ;

  string sUrl ;
  if (string("") != _sHtml)
		sUrl = _sHtml ;
	else
		sUrl = _sFileName ;

  if (string("") == sUrl)
  {
    string ps = string("NSImportWindow::Navigate with empty URL") ;
	  _pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

try
{
	Variant Flags(navNoReadFromCache) ;
	TVariant VFlags = Flags.operator TVariant() ;

  string sMsg = string("NSImportWindow starting to Navigate on url= ") + sUrl ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  wchar_t buff[1024] ;
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sUrl.c_str(), -1, buff, sizeof(buff)) ;
	_Form->Control->Navigate(buff, &VFlags) ;
}
catch (Exception &ex)
{
  string ps = string("Exception NSImportWindow::Navigate when navigating to ") + sUrl + string(" (") + string(ex.Message.c_str()) + string(")") ;
  _pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
}
catch (...)
{
  string ps = string("Exception NSImportWindow::Navigate when navigating to ") + sUrl ;
  _pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
}
}

void
NSImportWindow::NavigateErrorEvent(int iStatusCode, string sURL)
{
	string ps = string("Ev : NavigateError for URL \"") + sURL + string("\"") ;
  ps += string(" (") + getNavigateErrorShortMsg(iStatusCode) + string(")") ;
	_pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  erreur(ps.c_str(), standardError, 0) ;
}

bool
NSImportWindow::InitInfosFichier()
{
	if (string("") == _sFileName)
		return false ;

	// Récupération du path
  //
	size_t pos1 = _sFileName.find_last_of("\\") ;

	if ((NPOS == pos1) || (strlen(_sFileName.c_str())-1 == pos1))
	{
		// on doit normalement récupérer au moins 'C:\\'
		// => on génère une erreur s'il n'y a pas de '\\' ou que le '\\' est le dernier caractère
    string sErrorText = _pContexte->getSuperviseur()->getText("fileErrors", "cantParsePath") + string(" ") + _sFileName ;
    _pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return false ;
	}

	_sPathName   = string(_sFileName, 0, pos1+1) ;
  _sNomFichier = string(_sFileName, pos1+1, strlen(_sFileName.c_str())-pos1-1) ;

  string sMsg = string("NSImportWindow:InitInfosFichier detect path= ") + _sPathName + string(" file= ") + _sNomFichier ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

	// Récupération de l'extension
	size_t pos2 = _sNomFichier.find_last_of(".");

	if ((NPOS == pos2) || (strlen(_sNomFichier.c_str())-1 == pos2))
	{
  	//
    // On vérifie d'abord si ce n'est pas un fichier texte de l'ancien
    // Nautilus : gastroda
    //
    //sNomSeul = pseumaj(sNomSeul.c_str());
    if (string("GASTRODA") == _sNomFichier)
    {
    	if (!convertGdata(&_sFileName))
      	return false ;
      pos2 = _sFileName.find(".") ;
    }
    else
    {
    	string sErrorText = _pContexte->getSuperviseur()->getText("fileErrors", "cantFindExtension") + string(" ") + _sFileName ;
      _pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
      return false ;
    }
  }

  _sExtension = string(_sNomFichier, pos2+1, strlen(_sNomFichier.c_str())-pos2-1) ;

  // nom du fichier sans path et sans extension
  _sNomSeul = string(_sNomFichier, 0, pos2) ;

  // nom du fichier (sans path)
  // sNomFichier = sNomSeul + "." + sExtension ;

	return true ;
}

void
NSImportWindow::displayFile(const string sFileName)
{
  if (((TPdfImport*) NULL == _PdfForm) || ((TAcroPDF*) NULL == _PdfForm->Control))
    return ;

  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  if (string("") == sFileName)
  {
    string ps = string("NSImportWindow::displayFile with empty file name") ;
	  pSuper->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

try
{
  string sTrace = string("NSImportWindow::displayFile for file \"") + sFileName + string("\"") ;
  pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

	wchar_t buff[1024] ;
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sFileName.c_str(), -1, buff, sizeof(buff)) ;

  // ******************** USING ACROBAT DLL
  //

  TAcroPDF* pPdfControl = _PdfForm->Control ;
  if ((TAcroPDF*) NULL == pPdfControl)
  {
    string sTrace = string("NSImportWindow::displayFile for file \"") + sFileName + string("\". Null Acrobat control, leaving.") ;
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
    string sTrace = string("NSImportWindow::displayFile: Acrobat failed to load file \"") + sFileName + string("\"") ;
    pSuper->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }
  else if (bResult == true)
  {
    string sTrace = string("NSImportWindow::displayFile: Acrobat properly loaded file \"") + sFileName + string("\"") ;
    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;
    return ;
  }
}
catch (Exception &ex)
{
  string sTrace = string("Exception NSImportWindow::displayFile") + string(ex.Message.c_str()) ;
  pSuper->trace(&sTrace, 1, NSSuper::trError) ;
	// erreur(sTrace.c_str(), standardError, 0) ;
}
catch (...)
{
  string sTrace = string("Exception NSImportWindow::displayFile") ;
  pSuper->trace(&sTrace, 1, NSSuper::trError) ;
	// erreur(sTrace.c_str(), standardError, 0) ;
}
}

bool
NSImportWindow::InitNautilusType()
{
  // On récupère les infos du type mime lié à l'extension du fichier

	NSTypeMimeInfo infoTypeMime ;

	if (false == _pContexte->getSuperviseur()->chercheTypeMimeInfo(_sExtension, &infoTypeMime))
    return false ;

	_sTypeNautilus = infoTypeMime.getType() ;

  string sMsg = string("NSImportWindow:InitNautilusType found type ") + _sTypeNautilus ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

	return true ;
}

bool
NSImportWindow::GenereHtml()
{
  string sMsg = string("NSImportWindow entering GenereHtml") ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

	string sType = string(_sTypeNautilus, 0, 2) ;
	string sTemplateHtml, sPathHtml;

	if (_pContexte->typeDocument(_sTypeNautilus, NSSuper::isHTML)) // cas des html statiques
	{
		// cas des html : le fichier est sFileName et ne doit pas etre détruit
    return true ;
	}
  else if (string("ZTPDF") == _sTypeNautilus)
	{
		// cas des pdf : le fichier est sFileName et ne doit pas etre détruit
    return true ;
	}
	else if (_pContexte->typeDocument(_sTypeNautilus, NSSuper::isImage))
	{
  	// cas des images :
    // on construit un html temporaire dans le répertoire FPER
    sPathHtml = _pContexte->PathName("FPER") ;

    NSModHtml html(toImporter, (NSNoyauDocument *) 0, _pContexte) ;

    // on trouve le nom du fichier temporaire à visualiser
    _sHtml = html.nomSansDoublons(sPathHtml, _sNomSeul, "htm") ;
    _sHtml = sPathHtml + _sHtml ;

    // on passe la template des documents images
    sTemplateHtml = _pContexte->PathName("NTPL") + string("docimage.htm") ;

    // generation du fichier html (dans le repertoire FPER)
    if (!html.genereHtml(sTemplateHtml, _sHtml, _sPathName, _sNomFichier, _sTypeNautilus))
    {
    	string sErrorText = _pContexte->getSuperviseur()->getText("importManagement", "errorCreatingImportHtmlFile") ;
      _pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
      return false ;
    }
  }
	else if (string("ZTRTF") == _sTypeNautilus)
  {
		// cas des RTF :
    // on construit un html temporaire dans le répertoire FPER
    sPathHtml = _pContexte->PathName("FPER") ;

    NSHtmlRTF html(toImporter, 0, _sFileName.c_str(), _pContexte) ;

    // on trouve le nom du fichier temporaire à visualiser
    _sHtml = html.nomSansDoublons(sPathHtml, _sNomSeul, "htm") ;
    _sHtml = sPathHtml + _sHtml ;

    // on convertit le fichier RTF en HTML
    html.Convertir() ;

    // on passe la template des documents RTF
    sTemplateHtml = _pContexte->PathName("NTPL") + string("docrtf.htm") ;

    // generation du fichier html (dans le repertoire FPER)
    if (!html.genereHtml(sTemplateHtml, _sHtml, _sPathName, _sNomFichier, _sTypeNautilus))
    {
    	string sErrorText = _pContexte->getSuperviseur()->getText("importManagement", "errorCreatingImportHtmlFile") ;
    	erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
      return false ;
    }
  }
	else if (string("ZTTXT") == _sTypeNautilus)
	{
  	// cas des TXT :
    // on construit un html temporaire dans le répertoire FPER
    sPathHtml = _pContexte->PathName("FPER") ;

    NSHtmlTXT html(toImporter, 0, _sFileName.c_str(), _pContexte) ;

    // on trouve le nom du fichier temporaire à visualiser
    _sHtml = html.nomSansDoublons(sPathHtml, _sNomSeul, "htm") ;
    _sHtml = sPathHtml + _sHtml ;

    // on passe la template des documents TXT
    sTemplateHtml = _pContexte->PathName("NTPL") + string("doctxt.htm") ;

    // generation du fichier html (dans le repertoire FPER)
    if (!html.genereHtml(sTemplateHtml, _sHtml, _sPathName, _sNomFichier, _sTypeNautilus))
    {
    	string sErrorText = _pContexte->getSuperviseur()->getText("importManagement", "errorCreatingImportHtmlFile") ;
    	erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
      return false ;
    }
  }
  else // types non gérés
  {
  	string sErrorText = _pContexte->getSuperviseur()->getText("importManagement", "importNotHandledForSuchFile") ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return false ;
  }

  sMsg = string("NSImportWindow leaving GenereHtml") ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

	return true ;
}

void
NSImportWindow::CmImporter()
{
  string sMsg = string("NSImportWindow : User asks to import") ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

	_pNewDoc = new NSRefDocument(0, _pContexte) ;
  _pNewDoc->setReadOnly(false) ;

	if ((string("") == _sHtml) &&
      (_pContexte->typeDocument(_sTypeNautilus, NSSuper::isHTML)))
	{
  	// cas de l'importation d'un html
    // on importe le nouveau document html statique
    _bImportSuccessful = _pNewDoc->ImporterHtml(_sFileName) ;
  }
  else
  {
    string sDocTitle = string("") ;
  	// cas de l'importation d'un fichier externe
    _bImportSuccessful = _pNewDoc->ImporterFichier(_sTypeNautilus, sDocTitle, _sNomFichier, _sPathName) ;
	}

  if (false == _bImportSuccessful)
  {
    string sErrorText = _pContexte->getSuperviseur()->getText("importManagement", "importFailed") ;
    _pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
  }

	_bCanClose = true ;
	CloseWindow() ;

  sMsg = string("NSImportWindow leaving CmImporter") ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;
}

voidNSImportWindow::CmReferencer()
{
	// on référence le fichier par un pointeur
	_pNewDoc = new NSRefDocument(0, _pContexte) ;

	if (string("") == _sHtml)
	{
  	// cas du référencement d'un html statique
    if (!_pNewDoc->Referencer("ZSHTM", _sNomFichier, _sFileName))
    {
    	string sErrorText = _pContexte->getSuperviseur()->getText("importManagement", "referenceFailed") ;
  		erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    }
  }
  else
  {
  	// cas du référencement d'un fichier externe
    if (!_pNewDoc->Referencer(_sTypeNautilus, _sNomFichier, _sFileName))
    {
    	string sErrorText = _pContexte->getSuperviseur()->getText("importManagement", "referenceFailed") ;
  		erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    }
	}

	_bCanClose = true ;
	CloseWindow() ;
}

void
NSImportWindow::CmCapturer()
{
	NSCaptureArray  CaptureArray(_pContexte) ;
  NSPatPathoArray PPT(_pContexte->getSuperviseur()) ;

  NSCaptureFromHPRIM HprimHunter(_pContexte) ;

/*
  bool bSuccess = importHPRIM2(&CaptureArray, &PPT) ;

  if (!bSuccess)
		bSuccess = importHPRIM1(&CaptureArray, &PPT) ;
*/

	HprimHunter.setFileName(_sFileName) ;

  string sProcessLog = string("") ;

  int iAlertLevel = 0 ;
  bool bHeaderValidity ;
  string sRootExam = string("") ;
  bool bSuccess = HprimHunter.importHPRIM2(&CaptureArray, &PPT, &iAlertLevel, &sProcessLog, &bHeaderValidity, sRootExam) ;

  if (false == bSuccess)
		bSuccess = HprimHunter.importHPRIM1(&CaptureArray, &PPT, &sProcessLog, &bHeaderValidity, sRootExam) ;

	if (bSuccess && bHeaderValidity)
  {
	  NSCSDocument* pCSDoc = new NSCSDocument(0, _pContexte, "") ;

    NSPatPathoArray PatPatho(_pContexte->getSuperviseur()) ;
    pCSDoc->initFromPatPatho(&PatPatho) ;

    PatPatho.ajoutePatho("GBIOL1", 0) ;
    PatPatho.InserePatPathoFille(PatPatho.begin(), &PPT) ;

    pCSDoc->setPatPatho(&PatPatho) ;

    NSDocViewManager dvManager(_pContexte) ;
	  dvManager.createView(pCSDoc, "CS Format") ;
  }
}

bool
NSImportWindow::importHPRIM2(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT)
{
	ifstream inFile ;
	inFile.open(_sFileName.c_str());
	if (!inFile)
	{
  	string sErrorText = _pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return false ;
	}

  parseHPRIM1header(&inFile, pCaptureArray, pPPT) ;

  NSEpiFlechiesDB flechiesDB(_pContexte) ;

  bool bFinFound = false ;
  bool bFinFichierFound = false ;

  bool bValueWelcomed = false ;

  bool bLabTagFound = false ;

  // On saute tout jusqu'à la balise LAB
  //
  while (!inFile.eof() && !bLabTagFound)
	{
    string sLine = string("") ;
  	getline(inFile, sLine) ;

    strip(sLine, stripBoth) ;
    if (string("****LAB****") == sLine)
    	bLabTagFound = true ;
	}

  // no LAB tag : not HPRIM2
  //
  if (false == bLabTagFound)
  {
  	inFile.close() ;
    return false ;
  }

  string sConcept  = string("") ;
  string sValue    = string("") ;
  string sUnit     = string("") ;
  string sNormInf  = string("") ;
  string sNormSup  = string("") ;
  string sValue2   = string("") ;
  string sUnit2    = string("") ;
  string sNormInf2 = string("") ;
  string sNormSup2 = string("") ;

	while (!inFile.eof())
	{
    string sStripLine = string("") ;
  	getline(inFile, sStripLine) ;

    strip(sStripLine, stripBoth) ;

    if      (string("****FIN****") == sStripLine)
    	bFinFound = true ;
    else if (string("****FINFICHIER****") == sStripLine)
    	bFinFichierFound = true ;
    else if ((string("") != sStripLine) && (strlen(sStripLine.c_str()) > 4))
    {
      NSPatPathoArray PPTnum(_pContexte->getSuperviseur()) ;

    	// text: we try to analyze it
      //
      if (string(sStripLine, 0, 4) == string("TEX|"))
      {
      	sStripLine = string(sStripLine, 4, strlen(sStripLine.c_str()) - 4) ;

    		// What is it ?
      	string sPattern = string("") ;

      	// Title ?
      	string sResult = flechiesDB.getCodeLexiq(sStripLine, 'T') ;
  			if (sResult == string(""))
      		sResult = flechiesDB.getCodeLexiq(sStripLine, 'G') ;
      	if (sResult == string(""))
      		sResult = flechiesDB.getCodeLexiq(sStripLine, 'Z') ;

      	// value
      	if (string("") == sResult)
      	{
      		analysedCapture aCapt(_pContexte, sStripLine, (NSCaptureArray*) 0, FromMailBox) ;
      		ParseElemArray aRawParseResult ;
      		ParseElemArray aSemanticParseResult ;
      		aCapt.getSemanticParseElemArray(&aRawParseResult, &aSemanticParseResult) ;
        	//
        	// We only use the information if it has been fully recognized
        	// elsewhere, it can lead to much stupid behaviour
        	//
        	bool bFullSuccess ;
      		sPattern = aCapt.getNumPattern(&aRawParseResult, &aSemanticParseResult, &bFullSuccess) ;
        	if (sPattern != string(""))
          {
          	// We try to get the concept here, because sometimes, the RES
            // information only contains sender specific codes
            //
          	string sResult = aCapt.getElementByTagInNumPattern(sPattern, 'V') ;
            if (string("") != sResult)
            	sConcept = sResult ;

        		if (false == bFullSuccess)
          		bValueWelcomed = false ;
          	else
          	{
        			if (string("") != sConcept)
          			bValueWelcomed = true ;
          	}
        	}
        }

      	int iCol = 0 ;
      	if (string("") != sResult)
      		iCol = 0 ;
      	else
      	{
      		if (false == PPTnum.empty())
        	{
        		size_t posValue = sPattern.find("[V") ;
          	if (NPOS != posValue)
          		iCol = 0 ;
          	else
          		iCol = 1 ;
        	}
      	}

    		// Pushing the text
      	//
    		Message theMessage ;
      	theMessage.SetTexteLibre(sStripLine) ;
      	pPPT->ajoutePatho("£?????", &theMessage, iCol) ;
      }
      //
      // RES: structured part
      //
      else if (string(sStripLine, 0, 4) == string("RES|"))
      {
      	sStripLine = string(sStripLine, 4, strlen(sStripLine.c_str()) - 4) ;

        int iNumInfo  = 0 ;
        int iNumSerie = 0 ;

        while (string("") != sStripLine)
        {
        	size_t posPipe = sStripLine.find("|") ;

        	if (posPipe > 0)
          {
          	string sResData ;

          	if (NPOS == posPipe)
            	sResData = sStripLine ;
            else
          		sResData = string(sStripLine, 0, posPipe) ;

            if (string("N") == sResData)
            {
            	iNumSerie++ ;
              iNumInfo = -1 ;
            }
            else if (string("L") == sResData)
            {
            	iNumSerie++ ;
              iNumInfo = -1 ;
            }
            else if (string("F") == sResData)
            {
              iNumInfo = -1 ;
            }
            else
            {
              switch (iNumInfo)
              {
                // Value inside the series
                case 0 :
                	if (0 == iNumSerie)
                  {
                    string sResult = flechiesDB.getCodeLexiq(sResData, 'V') ;
                    if (string("") != sResult)
                      sConcept = sResult ;
                  }
                  else if (1 == iNumSerie)
                    sValue = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                  else if (2 == iNumSerie)
                    sValue2 = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                  break ;
                // Concept outside the series, unit inside
                case 1 :
                {
                  if (0 == iNumSerie)
                  {
                    string sResult = flechiesDB.getCodeLexiq(sResData, 'V') ;
                    if (string("") != sResult)
                      sConcept = sResult ;
                  }
                  else
                  {
                    string sResult = flechiesDB.getCodeLexiq(sResData, '2') ;
                    if (string("") != sResult)
                    {
                      if      (1 == iNumSerie)
                        sUnit = sResult ;
                      else if (2 == iNumSerie)
                        sUnit2 = sResult ;
                    }
                  }
                  break ;
                }
                // lower normal value inside the series
                case 2 :
                	if      (1 == iNumSerie)
                    sNormInf = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                  else if (2 == iNumSerie)
                    sNormInf2 = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                  break ;
                // upper normal value inside the series
                case 3 :
                	if      (1 == iNumSerie)
                    sNormSup = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                  else if (2 == iNumSerie)
                    sNormSup2 = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                  break ;
              }
            }

          	if (posPipe < strlen(sStripLine.c_str()) - 1)
          		sStripLine = string(sStripLine, posPipe + 1, strlen(sStripLine.c_str()) - posPipe - 1) ;
            else
            	sStripLine = string("") ;
          }
          else
          {
						sStripLine = string(sStripLine, 1, strlen(sStripLine.c_str()) - 1) ;
            if (1 == iNumInfo)
            {
            	if      (1 == iNumSerie)
              	sUnit = "200001" ;
              else if (2 == iNumSerie)
              	sUnit2 = "200001" ;
            }
          }

          iNumInfo++ ;
        }

        if (string("") != sConcept)
        {
        	pPPT->ajoutePatho(sConcept, 1) ;

          bool bValuesFound = false ;

        	if ((string("") != sValue) && (string("") != sUnit))
          {
          	bValuesFound = true ;

          	Message theMessage ;
      			theMessage.SetComplement(sValue) ;
            theMessage.SetUnit(sUnit) ;
            pPPT->ajoutePatho(string("£N0;03"), &theMessage, 2) ;

          	if (string("") != sNormInf)
          	{
          		pPPT->ajoutePatho(string("VNOMI1"), 2) ;

            	Message theMessage ;
      				theMessage.SetComplement(sNormInf) ;
            	theMessage.SetUnit(sUnit) ;
            	pPPT->ajoutePatho(string("£N0;03"), &theMessage, 3) ;
          	}
          	if (string("") != sNormSup)
          	{
          		pPPT->ajoutePatho(string("VNOMS1"), 2) ;

            	Message theMessage ;
      				theMessage.SetComplement(sNormSup) ;
            	theMessage.SetUnit(sUnit) ;
            	pPPT->ajoutePatho(string("£N0;03"), &theMessage, 3) ;
          	}
          }
          if ((string("") != sValue2) && (string("") != sUnit2))
          {
          	bValuesFound = true ;

          	Message theMessage ;
      			theMessage.SetComplement(sValue2) ;
            theMessage.SetUnit(sUnit2) ;
            pPPT->ajoutePatho(string("£N0;03"), &theMessage, 2) ;

          	if (string("") != sNormInf2)
          	{
          		pPPT->ajoutePatho(string("VNOMI1"), 2) ;

            	Message theMessage ;
      				theMessage.SetComplement(sNormInf2) ;
            	theMessage.SetUnit(sUnit2) ;
            	pPPT->ajoutePatho(string("£N0;03"), &theMessage, 3) ;
          	}
          	if (string("") != sNormSup2)
          	{
          		pPPT->ajoutePatho(string("VNOMS1"), 2) ;

            	Message theMessage ;
      				theMessage.SetComplement(sNormSup2) ;
            	theMessage.SetUnit(sUnit2) ;
            	pPPT->ajoutePatho(string("£N0;03"), &theMessage, 3) ;
          	}
          }
          if (bValuesFound)
          {
          	PatPathoIter iterPere = pPPT->end() ;
          	iterPere-- ;
          	pPPT->InserePatPathoFille(iterPere, &PPTnum) ;
          }
        }

      	sConcept  = string("") ;
				sValue    = string("") ;
				sUnit     = string("") ;
				sNormInf  = string("") ;
				sNormSup  = string("") ;
				sValue2   = string("") ;
				sUnit2    = string("") ;
				sNormInf2 = string("") ;
				sNormSup2 = string("") ;
      }
    }
  }

	inFile.close() ;

  if (!bFinFound || !bFinFichierFound)
  {
  	string sErrorText = _pContexte->getSuperviseur()->getText("hprimManagement", "endOfFileMarkupsNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
  }

  return true ;
}

bool
NSImportWindow::importHPRIM1(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT)
{
	ifstream inFile ;
	string sLine ;
	string sText = "" ;

	inFile.open(_sFileName.c_str());	if (!inFile)
	{
  	string sErrorText = _pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return false ;
	}

  parseHPRIM1header(&inFile, pCaptureArray, pPPT) ;

  NSEpiFlechiesDB flechiesDB(_pContexte) ;

  bool bFinFound = false ;
  bool bFinFichierFound = false ;

  bool bValueWelcomed = false ;

	while (!inFile.eof())
	{
  	getline(inFile, sLine) ;

    string sStripLine = sLine ;
    strip(sStripLine, stripBoth) ;

    if (sStripLine == string("****FIN****"))
    	bFinFound = true ;
    else if (sStripLine == string("****FINFICHIER****"))
    	bFinFichierFound = true ;
    else if (sStripLine != string(""))
    {
    	// What is it ?
      string sPattern = string("") ;

      // Title ?
      string sResult = flechiesDB.getCodeLexiq(sStripLine, 'T') ;
  		if (sResult == string(""))
      	sResult = flechiesDB.getCodeLexiq(sStripLine, 'G') ;
      if (sResult == string(""))
      	sResult = flechiesDB.getCodeLexiq(sStripLine, 'Z') ;

      NSPatPathoArray PPTnum(_pContexte->getSuperviseur()) ;

      // value
      if (sResult == string(""))
      {
      	analysedCapture aCapt(_pContexte, sStripLine, (NSCaptureArray*) 0, FromMailBox) ;
      	ParseElemArray aRawParseResult ;
      	ParseElemArray aSemanticParseResult ;
      	aCapt.getSemanticParseElemArray(&aRawParseResult, &aSemanticParseResult) ;
        //
        // We only use the information if it has been fully recognized
        // elsewhere, it can lead to much stupid behaviour
        //
        bool bFullSuccess ;
      	sPattern = aCapt.getNumPattern(&aRawParseResult, &aSemanticParseResult, &bFullSuccess) ;
        if (sPattern != string(""))
        {
        	if (!bFullSuccess)
          	bValueWelcomed = false ;
          else
          {
        		aCapt.numPatternToTree(&sPattern, &PPTnum) ;
            size_t posValue = sPattern.find("[V") ;
          	if (posValue != NPOS)
          		bValueWelcomed = true ;
          }
        }
      }

      int iCol = 0 ;
      if (sResult != string(""))
      	iCol = 0 ;
      else
      {
      	if (false == PPTnum.empty())
        {
        	size_t posValue = sPattern.find("[V") ;
          if (posValue != NPOS)
          	iCol = 0 ;
          else
          	iCol = 1 ;
        }
      }

    	// Pushing the text
      //
    	Message theMessage ;
      theMessage.SetTexteLibre(sStripLine) ;
      pPPT->ajoutePatho("£?????", &theMessage, iCol) ;

      if (sResult != string(""))
      	pPPT->ajoutePatho(sResult, iCol+1) ;
      else
      {
      	if (bValueWelcomed && (false == PPTnum.empty()))
        {
        	PatPathoIter iterPere = pPPT->end() ;
          iterPere-- ;

          size_t posValue = sPattern.find("[V") ;
          if (posValue == NPOS)
          {
          	while ((iterPere != pPPT->begin()) && ((*iterPere)->getLexique() == string("£?????")))
            	iterPere-- ;
            while ((iterPere != pPPT->begin()) && (((*iterPere)->getLexique())[0] != 'V'))
            	iterPere-- ;
          }

        	pPPT->InserePatPathoFille(iterPere, &PPTnum) ;
        }
      }
    }
  }

	inFile.close() ;

  if (!bFinFound || !bFinFichierFound)
  {
  	string sErrorText = _pContexte->getSuperviseur()->getText("hprimManagement", "endOfFileMarkupsNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
  }

  return true ;
}

bool
NSImportWindow::parseHPRIM1header(ifstream* pInFile, NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT)
{
	if (!pInFile || (NULL == pCaptureArray) || (NULL == pPPT))
		return false ;

	string sLang = _pContexte->getUserLanguage() ;

	// HPRIM Format
	// line 1 Patient ID (num or name + surname)
	//      2 Patient Familly name
	//      3 Patient name
	//      4 Adresse 1
	//      5 Adresse 2
	//      6 Code Postal / Ville
	//      7 Date Naissance
	//      8 No SS
	//      9 Code ?
	//     10 Date Examen
	//     11 Identifiant Labo
	//     12 Identifiant Médecin

	// Loading header
  //
  size_t iNumLine = 0 ;
  string sLine ;

  while (!pInFile->eof() && (iNumLine < 12))
	{
  	getline(*pInFile, sLine) ;

    string sPathString      = string("") ;
    string sUnitString      = string("") ;
    string sClassifString   = string("") ;
    string sInformationDate = string("") ;

    strip(sLine, stripBoth) ;

    if (sLine != string(""))
    {
    	switch(iNumLine)
      {
      	case 1 :
          pCaptureArray->ajouter(new NSCapture(_pContexte, "ZADMI1/LIDET1/LNOM01", sLine, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        case 2 :
          pCaptureArray->ajouter(new NSCapture(_pContexte, "ZADMI1/LIDET1/LNOM21", sLine, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        case 3 :
          pCaptureArray->ajouter(new NSCapture(_pContexte, "ZADMI1/LCOOR1/LADRE1/LADL11", sLine, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        case 4 :
          pCaptureArray->ajouter(new NSCapture(_pContexte, "ZADMI1/LCOOR1/LADRE1/LADL21", sLine, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        case 5 :
        	{
          	size_t blkPos = sLine.find(" ") ;
            if (blkPos != NPOS)
            {
            	string sZip  = string(sLine, 0, blkPos) ;
              string sCity = string(sLine, blkPos + 1, strlen(sLine.c_str()) - blkPos - 1) ;
              strip(sCity, stripBoth) ;
              pCaptureArray->ajouter(new NSCapture(_pContexte, "ZADMI1/LCOOR1/LADRE1/LVILL1/LZIP01", sZip, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
              pCaptureArray->ajouter(new NSCapture(_pContexte, "ZADMI1/LCOOR1/LADRE1/LVILL1/LCOMU1", sCity, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
            }
          }
          break ;
        //
        // Birthdate
        //
        case 6 :
        	{
          	string sDate = string("") ;
          	//
          	// JJ/MM/AA
            //
          	if (strlen(sLine.c_str()) == 8)
            {
          		NVLdVTemps currentDate ;
            	currentDate.takeTime() ;
            	string sCurrentDate = currentDate.donneDate() ;
            	string sTreshold = string(sCurrentDate, 2, 2) ;

            	sDate = getDateFromHPRIM1date(sLine, sTreshold) ;
            }
            //
            // JJ/MM/AAAA
            //
            else if (strlen(sLine.c_str()) == 10)
            	donne_date_inverse(sLine, sDate, sLang) ;

            if (sDate != string(""))
            	pCaptureArray->ajouter(new NSCapture(_pContexte, "ZADMI1/LIDET1/KNAIS1", sDate, FromMailBox, sClassifString, "2DA011", sInformationDate)) ;
          }
          break ;
        case 7 :
          pCaptureArray->ajouter(new NSCapture(_pContexte, "ZADMI1/LFRAN1/LFRAB1", sLine, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        //
        // Document's date
        //
        case 9 :
        	{
          	string sDate = string("") ;

          	// JJ/MM/AA
          	if (strlen(sLine.c_str()) == 8)
            	sDate = getDateFromHPRIM1date(sLine, "") ;
            // JJ/MM/AAAA
            else if (strlen(sLine.c_str()) == 10)
            	donne_date_inverse(sLine, sDate, sLang) ;

            if (sDate != string(""))
            {
            	pPPT->ajoutePatho("KCHIR2", 0) ;
							Message theMessage ;
    					theMessage.SetUnit("2DA011") ;
							theMessage.SetComplement(sDate) ;
							pPPT->ajoutePatho("£D0;10", &theMessage, 1) ;
            }
          }
      }
    }
    iNumLine++ ;
  }
  return true ;
}

string
NSImportWindow::getDateFromHPRIM1date(string sHPRIMdate, string s2kTreshold)
{
	if (strlen(sHPRIMdate.c_str()) != 8)
		return string("") ;

	if (('/' != sHPRIMdate[2]) || ('/' != sHPRIMdate[5]))
		return string("") ;

  string sYear  = string(sHPRIMdate, 6, 2) ;
  string sMonth = string(sHPRIMdate, 3, 2) ;
  string sDay   = string(sHPRIMdate, 0, 2) ;

  string sDate = sYear + sMonth + sDay ;

  // Year ?
  if (s2kTreshold == string(""))
  	sDate = string("20") + sDate ;
  else
  {
  	int iTreshold = atoi(s2kTreshold.c_str()) ;
    int iYear     = atoi(sYear.c_str()) ;
    if (iYear > iTreshold)
    	sDate = string("19") + sDate ;
    else
    	sDate = string("20") + sDate ;
  }

  return sDate ;
}

voidNSImportWindow::CmAnnuler()
{
	string sCaption = string("Message ") + _pContexte->getSuperviseur()->getAppName().c_str() ;
  string sErrorText = _pContexte->getSuperviseur()->getText("importManagement", "doYouReallyWantToCancelThisImport") ;
	int retVal = MessageBox(sErrorText.c_str(), sCaption.c_str(), MB_YESNO) ;

	if (IDYES == retVal)
	{
		_bCanClose = true ;
		CloseWindow() ;
	}
}

bool
NSImportWindow::convertGdata(string* psFichier)
{
	//
	// Ouverture du fichier des modules
	//
	ifstream inFile;
	inFile.open(psFichier->c_str());
	if (!inFile)
	{
		erreur("Erreur à l'ouverture du fichier gastroda", standardError, 0, GetHandle());
		return false;
	}

  string sFichierOut = _pContexte->PathName("FPER") + string("GASTRODA.HTM");
  ofstream outFile;
  outFile.open(sFichierOut.c_str());
  if (!outFile)
  {
    erreur("Erreur d'ouverture en écriture du fichier gastroda.htm", standardError, 0) ;
    inFile.close() ;
    return false ;
  }

  outFile.write("<html>\n", 7) ;
  outFile.write("<body>\n", 7) ;

  char buffer[1001] ;
  int	 num_ligne = 0 ;
	//
	// Lecture du fichier
	//
	inFile.unsetf(ios::skipws) ;
  while (inFile.getline(buffer, 1000))
	{
  	OemToAnsi(buffer, buffer) ;

		num_ligne++ ;
    int iTailleBuf = strlen(buffer) ;

    for (int j = 0; j < iTailleBuf; j++)
    {
    	switch (buffer[j])
      {
      	case  9 : // Tabulation, non prise en compte
        	j += 2 ;
          break ;
        case 27 :
        	j++ ;
          break ;        case 28 :
        	j++ ;
          break ;
        default :
        	if ((num_ligne < 4) || (num_ligne > 8))
          	outFile.put(buffer[j]) ;
      }
    }
    outFile.write("<br>\n", 5) ;
	}
	inFile.close() ;

  outFile.write("</body>\n", 8) ;
  outFile.write("</html>\n", 8) ;

  outFile.close() ;

  *psFichier = sFichierOut ;

	return true ;
}

/****************** classe NSImportChild **************************/
DEFINE_RESPONSE_TABLE1(NSImportChild, TMDIChild)   EV_WM_CLOSE,
   EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;

NSImportChild::NSImportChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSImportWindow* clientWnd)
              :TMDIChild(parent, title, clientWnd), NSRoot(pCtx)
{
	pClient = clientWnd ;
}

NSImportChild::~NSImportChild()
{
  string sMsg = string("NSImportChild destructor") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;
}

void
NSImportChild::EvClose()
{
  string sMsg = string("NSImportChild entering EvClose") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	if (false == pClient->okToClose())
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "doYouReallyWantToCloseImport") ;
		int retVal = MessageBox(sErrorText.c_str(), sCaption.c_str(), MB_YESNO) ;

		if (retVal == IDYES)
    {
      pClient->_bCanClose = true ;
      pClient->ClosingPatch() ;
      pClient->EvClose() ;

			TWindow::EvClose() ;

      // sMsg = string("NSImportWindow destructor Uninitializing Ole") ;
      // pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      // ::OleUninitialize() ;
    }
	}
	else
  {
    pClient->EvClose() ;
		TWindow::EvClose() ;
  }

  sMsg = string("NSImportChild leaving EvClose") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  // pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
}

/****************** classe NSImpImgWindow **************************/

DEFINE_RESPONSE_TABLE1(NSImpImgWindow, TWindow)
   EV_WM_CLOSE,END_RESPONSE_TABLE;
NSImpImgWindow::NSImpImgWindow(TWindow* parent, NSContexte* pCtx, AssistCaptureDialog* pAssistDlg)               :TWindow(parent, "IMPORTATION_IMAGES"){	sHtml     = "" ;	pContexte = pCtx ;	pNewDoc   = 0 ;	bCanClose = false ;	pAssist   = pAssistDlg ;  Form      = 0 ;
	bNavOk = InitImageArray() ;
	if (bNavOk)		bNavOk = GenerePannel() ;  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;  if (NULL != pMyApp)    pMyApp->setMenu(string("menubar"), &hAccelerator) ;}
NSImpImgWindow::~NSImpImgWindow(){	string sFichImg, sFichHtm ;
	// on détruit les html restant dans l'array global	DetruireHtmlTemp() ;
	// on détruit la pile des images importées et leurs html associés	if (false == aImportArray.empty())    for (NSImgIter i = aImportArray.begin() ; aImportArray.end() != i ; i++)    {      if (string("") != (*i)->html)      {        sFichHtm = sPathName + (*i)->html ;        if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichHtm))        {          string sMsg = "Pb à la destruction du fichier html associé à l'image " + sFichHtm ;          erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;        }        else          (*i)->html = string("") ;      }      if (string("") != (*i)->supphtml)      {        string sFichHtm = sPathName + (*i)->supphtml ;        if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichHtm))        {          string sMsg = "Pb à la destruction du fichier html associé à l'image " + sFichHtm ;          erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;        }        else          (*i)->supphtml = string("") ;      }
      sFichImg = sPathName + (*i)->fichier ;      if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichImg))      {        string sMsg = "Pb à la destruction de l'image " + sFichImg ;        erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;      }      else        (*i)->fichier = string("") ;    }

  // on détruit le fichier pannel.htm
	if (string("") != sHtml)
	{
		if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sHtml))
			erreur("Pb à la destruction du fichier pannel html.", standardError, 0, GetHandle()) ;
	}

	// on détruit le document ayant servi à l'importation
	if (pNewDoc)
		delete pNewDoc ;

	// Delete de la Form
	if (Form)
	{
		delete Form ;
    // CoUninitialize() ;
	}

  if (0 != hAccelerator)
  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }
}

voidNSImpImgWindow::EvClose(){	TWindow::EvClose() ;}
bool
NSImpImgWindow::CanClose(){	// on réveille éventuellement le dialogue assistant	if (pAssist)		pAssist->PostMessage(WM_COMMAND, CM_ASSIST) ;	return true ;}
//
// Override a TWindow virtual to create the HWND directly.
// NSImpImgWindow and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSImpImgWindow::PerformCreate(int /*menuOrId*/)
{
  //CoInitialize(NULL) ;
  Form = new TImportImg(Parent->GetHandle(), this) ;
  Form->Visible = false ;
  Form->ParentWindow = Parent->HWindow ;
  SetHandle(Form->Handle) ;
  ::SetParent(Forms::Application->Handle, pContexte->GetMainWindow()->HWindow) ;

  // on navigue vers le fichier html à importer
  Navigate() ;
}

bool
NSImpImgWindow::IsTabKeyMessage(MSG &msg)
{
	if (NULL == GetCapture())
	{
  	if ((WM_KEYDOWN == msg.message) || (WM_KEYUP == msg.message))
    {
    	if (VK_TAB == msg.wParam)
      {
      	SendMessage(CN_BASE + msg.message, msg.wParam, msg.lParam) ;
        return true ;
      }
    }
	}

	return false ;
}

//
// Let the form process keystrokes in its own way.  Without
// this method, you can't tab between control on the form.
//
bool
NSImpImgWindow::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;
	if (result)
    return true ;

	PRECONDITION(GetHandle()) ;
  return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;
}

// Fonction MakeVisiblevoid
NSImpImgWindow::MakeVisible()
{
  Form->Visible = true ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSImpImgWindow::SetupWindow()
{
	TWindow::SetupWindow() ;

  ModifyStyle(WS_BORDER, WS_CHILD) ;
  Form->Show() ;
  MakeVisible() ;
}

// Fonction Navigate////////////////////////////////////////////////////////////////
void
NSImpImgWindow::Navigate()
{
  if ((NULL == Form) || (NULL == Form->Control))
    return ;

  if (string("") == sHtml)
  {
    string ps = string("NSImpImgWindow::Navigate with empty URL") ;
	  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

  Variant Flags(navNoReadFromCache) ;
  TVariant VFlags = Flags.operator TVariant() ;

  wchar_t buff[1024] ;
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sHtml.c_str(), -1, buff, sizeof(buff)) ;

  Form->Control->Navigate(buff, &VFlags) ;
}

voidNSImpImgWindow::NavigateComplete(){  bool        bSucces = false ;
  WideString autoUrl = Form->Control->LocationURL ;  AnsiString url     = AnsiString(autoUrl) ;  string     sUrl    = string(url.c_str()) ;
  // Récupération du fichier associé à l'URL  size_t pos = sUrl.find_last_of("/") ;
  if (NPOS == pos)  {    // on doit normalement récupérer une chaine du type : "file:///path/fichier"    // => on génère une erreur s'il n'y a pas de '/'    erreur("Impossible de récupérer le nom du fichier en cours.", standardError, 0, GetHandle()) ;    return ;	}
	string sFich = string(sUrl, pos+1, strlen(sUrl.c_str())-pos-1) ;
	// Si on a navigué sur une image	// on l'importe automatiquement	if (!(sFich == string("pannel.htm")))	{  	if (!aImageArray.empty())    	for (NSImgIter i = aImageArray.begin(); i != aImageArray.end(); i++)      {      	if ((sFich == (*i)->fichier) || (sFich == (*i)->html))        {        	pNewDoc = new NSRefDocument(0, pContexte) ;          // cas de l'importation d'un fichier externe          string sDocTitle = string("") ;          if (pNewDoc->ImporterFichier((*i)->type, sDocTitle, (*i)->fichier ,sPathName))          {          	// on stocke l'image pour la détruire ultérieurement            // car on ne peut pas le faire dans NavigateComplete()            aImportArray.push_back(new NSImage(*(*i))) ;
            // on enleve l'image du tableau            delete (*i) ;            aImageArray.erase(i) ;
            bSucces = true ;          }          else          	Navigate() ;
          break ;        }        else if (sFich == (*i)->supphtml)        {        	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;          int retVal = MessageBox("Etes-vous sûr de vouloir supprimer cette image?", sCaption.c_str(), MB_YESNO) ;          if (retVal == IDYES)          {          	// on stocke l'image pour une destruction ultérieure            aSuppArray.push_back(new NSImage(*(*i))) ;            // on enlève l'image du tableau            delete (*i) ;            aImageArray.erase(i) ;          }          bSucces = true ;          break ;        }      }	}
	// Si l'importation a réussi, on réaffiche un nouveau pannel	if (bSucces)  {  	if (GenerePannel())    	Navigate() ;    else    	erreur("Erreur ==> Navigation annulée.", standardError, 0, GetHandle()) ;	}}voidNSImpImgWindow::NavigateErrorEvent(int iStatusCode, string sURL)
{
	string ps = string("Ev : NavigateError for URL \"") + sURL + string("\"") ;
  ps += string(" (") + getNavigateErrorShortMsg(iStatusCode) + string(")") ;
	pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  erreur(ps.c_str(), standardError, 0) ;
}
voidNSImpImgWindow::DetruireHtmlTemp(){  // Dans cette fonction, on ne détruit que les liens html  // vers des images à importer ou à supprimer et non  // les fichiers image proprement dits.	// array des images restant à importer	if (false == aImageArray.empty())  	for (NSImgIter i = aImageArray.begin(); aImageArray.end() != i ; i++)    {    	if (string("") != (*i)->html)      {      	string sFichHtm = sPathName + (*i)->html ;
        if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichHtm))        {        	string sMsg = "Pb à la destruction du fichier html associé à l'image " + (*i)->fichier ;          erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;        }        else        	(*i)->html = string("") ;      }      if (string("") != (*i)->supphtml)      {      	string sFichHtm = sPathName + (*i)->supphtml ;
        if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichHtm))        {        	string sMsg = "Pb à la destruction du fichier html associé à l'image " + (*i)->fichier ;          erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;        }        else        	(*i)->supphtml = string("") ;      }    }	// array des images supprimées du pannel	if (false == aSuppArray.empty())  	for (NSImgIter i = aSuppArray.begin(); aSuppArray.end() != i ; i++)    {    	if (string("") != (*i)->html)      {      	string sFichHtm = sPathName + (*i)->html ;
        if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichHtm))        {        	string sMsg = "Pb à la destruction du fichier html associé à l'image " + (*i)->fichier ;          erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;        }        else        	(*i)->html = string("") ;      }      if (string("") != (*i)->supphtml)      {      	string sFichHtm = sPathName + (*i)->supphtml ;
        if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichHtm))        {        	string sMsg = "Pb à la destruction du fichier html associé à l'image " + (*i)->fichier ;          erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;        }        else        	(*i)->supphtml = string("") ;      }    }}
boolNSImpImgWindow::InitImageArray(){  WIN32_FIND_DATA FileData ;  HANDLE          hSearch ;  char            szMask[255] ;  bool            bFinish = false ;  size_t          pos ;  string          sFileName, sExtension, sTypeNau ;  // NSTypeMime      ficheTypeMime(pContexte->getSuperviseur()) ;  NSTypeMimeInfo  infoTypeMime ;  DWORD           dwAttr ;  int             nbImg = 0, nbVid = 0 ;  string          sFichierHtml ;  string          sFichierSupp ;  aImageArray.vider() ;  aImportArray.vider() ;  aSuppArray.vider() ;  NSSuper* pSuper = pContexte->getSuperviseur() ;
  string ps = string("-- Entrée dans InitImageArray") ;
  pSuper->trace(&ps, 1) ;  sPathName = pContexte->PathName("IHTM") ;  strcpy(szMask, sPathName.c_str()) ;  strcat(szMask, "*.*") ;
  hSearch = FindFirstFile(szMask, &FileData) ;
  if (hSearch == INVALID_HANDLE_VALUE)  {  	erreur("Aucun fichier trouvé dans le répertoire d'importation.", standardError, 0, GetHandle()) ;    return false ;  }
	while (!bFinish)  {    dwAttr = FileData.dwFileAttributes ;

    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))    {    	sFileName = string(FileData.cFileName) ;      // Récupération de l'extension      pos = sFileName.find_last_of(".") ;    }
    if ((!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) && (pos != NPOS))    {    	sExtension = string(sFileName, pos+1, strlen(sFileName.c_str())-pos-1) ;
      ps = string("Fichier trouvé : ") + sFileName ;
      pSuper->trace(&ps, 1) ;

      ps = string("Extension : ") + sExtension ;
      pSuper->trace(&ps, 1) ;

      // On récupère les infos du type mime lié à l'extension du fichier      if (/*ficheTypeMime.*/pContexte->getSuperviseur()->chercheTypeMimeInfo(sExtension, &infoTypeMime))      {      	sTypeNau = infoTypeMime.getType() ;        ps = string("Type Nautilus : ") + sTypeNau ;        pSuper->trace(&ps, 1) ;
        if (pContexte->typeDocument(sTypeNau, NSSuper::isImageFixe))
        {          nbImg++ ;          sFichierHtml = "" ;          sFichierSupp = "" ;
          if (EncapsuleImage(sFileName, sTypeNau, nbImg, sFichierHtml, sFichierSupp))          	aImageArray.push_back(new NSImage(sFileName, sTypeNau, sFichierHtml, sFichierSupp)) ;          else          {          	string sMsg = "L'image " + sFileName + " ne peut etre importée." ;            erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;          }        }        else if (pContexte->typeDocument(sTypeNau, NSSuper::isImageAnimee))        {          nbVid++ ;          sFichierHtml = "" ;          sFichierSupp = "" ;
          if (EncapsuleImage(sFileName, sTypeNau, nbVid, sFichierHtml, sFichierSupp))          	aImageArray.push_back(new NSImage(sFileName, sTypeNau, sFichierHtml, sFichierSupp)) ;          else          {          	string sMsg = "La vidéo " + sFileName + " ne peut etre importée.";            erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;          }        }      }      else      {      	ps = string("Fiche TypeMime non trouvée...") ;        pSuper->trace(&ps, 1) ;      }    }
    if (!FindNextFile(hSearch, &FileData))    {    	if (ERROR_NO_MORE_FILES == GetLastError())      	bFinish = true ;      else      {      	erreur("Impossible de trouver le fichier suivant dans le répertoire d'importation.", standardError, 0, GetHandle()) ;        return false ;      }    }  }  ps = string("-- Sortie de InitImageArray.") ;  pSuper->trace(&ps, 1) ;
  return true ;}
boolNSImpImgWindow::EncapsuleImage(string sFichier, string sType, int index, string& sFichHtml, string& sFichSupp){  string  sOut = "";  ofstream outFile;  char     fich[255];  char     fichsupp[255];  // On genere d'abord le fichier html image
  sOut += "<HTML>\n" ;  sOut += "<BODY>\n" ;
	if (pContexte->typeDocument(sType, NSSuper::isImageFixe))
  {  	sOut += "<IMG SRC=\"" ;    sprintf(fich, "image%d.htm", index) ;    sprintf(fichsupp, "image%d_x.htm", index) ;  }  else // cas des vidéos  {  	sOut += "<IMG START=1 LOOP=0 DYNSRC=\"" ;    sprintf(fich, "video%d.htm", index) ;    sprintf(fichsupp, "video%d_x.htm", index) ;  }
  sOut += sFichier + "\">\n" ;  sOut += "</BODY>\n" ;  sOut += "</HTML>\n" ;
  // On fixe le nom du fichier html associé  // pour la visualisation et pour la suppression  string sPathHtml = sPathName + string(fich) ;  string sPathSupp = sPathName + string(fichsupp) ;
  // s'il existait déjà il est détruit  if (NsFileExists(sPathHtml))  	if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sPathHtml))      return false ;  if (NsFileExists(sPathSupp))  	if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sPathSupp))      return false ;  // ecriture du fichier html
  outFile.open(sPathHtml.c_str());  if (!outFile)  {  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFichier ;    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;    return false ;  }
  for (size_t i = 0; i < strlen(sOut.c_str()); i++)  	outFile.put(sOut[i]) ;  outFile.close() ;
  sFichHtml = string(fich) ;  sFichSupp = string(fichsupp) ;  // On genere maintenant le fichier servant à la suppression :  if (!CopyFile(sPathHtml.c_str(), sPathSupp.c_str(), false))  {    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") ;  	sErrorText += string(" ") + sPathHtml + string(" -> ") + sPathSupp ;
  	erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;    return false ;  }
	return true ;}
boolNSImpImgWindow::GenerePannel(){  string  sOut = "";  int     nbImages = 1;  ofstream outFile;
  sOut += "<HTML>\n";  sOut += "<HEAD>\n";  sOut += "<TITLE>Pannel d'importation</TITLE>\n";  sOut += "</HEAD>\n";  sOut += "<BODY>\n";  sOut += "<B>Choisissez une image à importer :</B>\n<HR>\n";
  sOut += "<TABLE BORDER=0 WIDTH=100%>\n";
  if (!aImageArray.empty())
  for (NSImgIter i = aImageArray.begin(); i != aImageArray.end(); i++)  {  	if (((nbImages - 1) % 4) == 0)    	sOut += "<TR ALIGN=\"center\">\n" ;    if (pContexte->typeDocument((*i)->type, NSSuper::isImageFixe))    {      sOut += "<TD><A HREF=\"" + (*i)->html + "\"> Importer </A><BR>\n" ;      sOut += "<A HREF=\"" + (*i)->html + "\">" ;      sOut += "<IMG SRC=\"" ;    }    else // cas des vidéos    {      sOut += "<TD><A HREF=\"" + (*i)->html + "\"> Importer </A><BR>\n" ;      sOut += "<A HREF=\"" + (*i)->html + "\">" ;      sOut += "<IMG START=1 LOOP=0 DYNSRC=\"" ;    }
    sOut += (*i)->fichier + "\" width=70 height=70>" ;    sOut += "</A><BR>\n<A HREF=\"" + (*i)->supphtml + "\"> Supprimer </A></TD>\n" ;
    if ((nbImages % 4) == 0)    	sOut += "</TR>\n" ;
    nbImages++ ;	}
	// si on est au milieu d'une ligne, on la termine à vide  if (((nbImages - 1) % 4) != 0)  {  	while (((nbImages - 1) % 4) != 0)    {    	sOut += "<TD></TD>\n" ;      nbImages++ ;    }
    sOut += "</TR>\n" ;	}
  sOut += "</TABLE>\n" ;  sOut += "</BODY>\n" ;  sOut += "</HTML>\n" ;
  // On fixe le nom du fichier pannel  sHtml = sPathName + string("pannel.htm") ;
	// s'il existait un pannel il est détruit  if (NsFileExists(sHtml))  	if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sHtml))    	return false ;
	outFile.open(sHtml.c_str()) ;
	if (!outFile)	{  	erreur("Impossible de créer le fichier pannel html à générer", standardError, 0, GetHandle()) ;    return false ;	}
	for (size_t i = 0; i < strlen(sOut.c_str()); i++)  	outFile.put(sOut[i]) ;
	outFile.close() ;	return true ;}voidNSImpImgWindow::CmQuitter(){	bCanClose = true ;
	if (!aImageArray.empty())	{  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    int retVal = MessageBox("Voulez-vous détruire les images qui n'ont pas été importées ?", sCaption.c_str(), MB_YESNO);
    if (IDYES == retVal)    {      for (NSImgIter i = aImageArray.begin() ; aImageArray.end() != i ; i++)      {      	string sFichImg = sPathName + (*i)->fichier ;
        if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichImg))        {        	string sMsg = "Pb à la destruction de l'image " + (*i)->fichier ;          erreur(sMsg.c_str(), standardError, 0, GetHandle()) ;        }        else        	(*i)->fichier = string("") ;      }    }	}
	CloseWindow() ;}
void
NSImpImgWindow::CmImporterTout()
{
	bool bOk = true ;
	bCanClose = true ;

	if (false == aImageArray.empty())
  	for (NSImgIter i = aImageArray.begin(); aImageArray.end() != i ; i++)    {    	pNewDoc = new NSRefDocument(0, pContexte) ;      // cas de l'importation d'un fichier externe (sans dialogue)      string sDocTitle = string("") ;      if (pNewDoc->ImporterFichier((*i)->type, sDocTitle, (*i)->fichier ,sPathName, false))      {      	// on stocke l'image pour la détruire ultérieurement        aImportArray.push_back(new NSImage(*(*i))) ;      }      else      {      	erreur("Erreur lors de l'importation des images", standardError, 0, GetHandle()) ;        bOk = false ;        break ;      }    }

	// on détruit aImageArray car les images sont déjà dans aImportArray
	if (bOk)
		aImageArray.vider() ;

	CloseWindow() ;
}
/****************** classe NSImpImgChild **************************/
DEFINE_RESPONSE_TABLE1(NSImpImgChild, TMDIChild)   EV_WM_CLOSE,
   EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;

NSImpImgChild::NSImpImgChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSImpImgWindow* clientWnd)
              :TMDIChild(parent, title, clientWnd), NSRoot(pCtx)
{
	pClient = clientWnd ;
}

NSImpImgChild::~NSImpImgChild()
{
}

void
NSImpImgChild::EvClose()
{
	if (!pClient->bCanClose)
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
		int retVal = MessageBox("Voulez-vous vraiment fermer l'importation ?", sCaption.c_str(), MB_YESNO) ;

		if (retVal == IDYES)
			TWindow::EvClose() ;
	}
	else
		TWindow::EvClose() ;
}

// *********************************// Définition de la classe NSImage// *********************************
NSImage::NSImage(){  fichier  = "" ;  type     = "" ;  html     = "" ;  supphtml = "" ;}
NSImage::NSImage(string sFichier, string sType, string sFichHtml, string sFichSupp){  fichier  = sFichier ;  type     = sType ;  html     = sFichHtml ;  supphtml = sFichSupp ;
}
NSImage::~NSImage(){}
NSImage::NSImage(const NSImage& rv){  fichier  = rv.fichier ;  type     = rv.type ;  html     = rv.html ;  supphtml = rv.supphtml ;
}
NSImage&NSImage::operator=(const NSImage& src){	if (this == &src)		return *this ;  fichier  = src.fichier ;  type     = src.type ;  html     = src.html ;  supphtml = src.supphtml ;
	return *this ;}
intNSImage::operator==(const NSImage& o){	if ((fichier  == o.fichier) &&      (type     == o.type) &&      (html     == o.html) &&      (supphtml == o.supphtml))		return 1 ;
	else
		return 0 ;
}

// **********************************// Définition de la classe NSImgArray// **********************************
//---------------------------------------------------------------------------//  Constructeur copie//---------------------------------------------------------------------------
/*
NSImgArray::NSImgArray(NSImgArray& rv) : NSImgVector(){    if (!rv.empty())	    for (NSImgIter i = rv.begin(); i != rv.end(); i++)   	        push_back(new NSImage(*(*i)));}
//---------------------------------------------------------------------------//  Destructeur//---------------------------------------------------------------------------
voidNSImgArray::vider(){    if (!empty())	    for (NSImgIter i = begin(); i != end(); )        {   	        delete *i;            erase(i);        }}
NSImgArray::~NSImgArray(){	vider();}           *//****************** classe NSLogWindow **************************/
DEFINE_RESPONSE_TABLE1(NSLogWindow, TWindow)	 EV_WM_CLOSE,
END_RESPONSE_TABLE;

NSLogWindow::NSLogWindow(TWindow* parent, string sFichier, NSContexte* pCtx)						:TWindow(parent)
{
  sFileName = sFichier ;
  pContexte = pCtx ;
  pNewDoc = 0 ;
  bCanClose = false ;

  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (NULL != pMyApp)    pMyApp->setMenu(string("menubar"), &hAccelerator) ;
}

NSLogWindow::~NSLogWindow()
{
	if (string("") != sHtml)
		NsDeleteTemporaryFile(pContexte->getSuperviseur(), sHtml) ;

	// Delete de la Form
	delete Form;
	// CoUninitialize();
	// OleUninitialize() ;

  if (0 != hAccelerator)
  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }
}

void
NSLogWindow::EvClose()
{
  TWindow::EvClose() ;
}

//
// Override a TWindow virtual to create the HWND directly.
// NSImportWindow and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSLogWindow::PerformCreate(int /*menuOrId*/)
{
	// CoInitialize(NULL);
  // OleInitialize(NULL);    // Use OleInitialize to get clipboard functionality
	Form = new TWebLog(Parent->GetHandle(), this);
	Form->Visible = false;
	Form->ParentWindow = Parent->HWindow;
	SetHandle(Form->Handle);
	::SetParent(Forms::Application->Handle, pContexte->GetMainWindow()->HWindow);

	// on navigue vers le fichier html à importer
	Navigate();
}

boolNSLogWindow::IsTabKeyMessage(MSG &msg)
{
  if (GetCapture() == NULL)
  {
    if (WM_KEYDOWN == msg.message || WM_KEYUP == msg.message)
    {
      if (VK_TAB == msg.wParam)
      {
        SendMessage(CN_BASE + msg.message, msg.wParam, msg.lParam) ;
        return true ;
      }
    }
  }

  return false ;
}

//
// Let the form process keystrokes in its own way.  Without
// this method, you can't tab between control on the form.
//
bool
NSLogWindow::PreProcessMsg(MSG &msg)
{
  bool result = IsTabKeyMessage(msg) ;
	if (result)
    return true ;

	PRECONDITION(GetHandle()) ;
  return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;
}

// Fonction MakeVisible
void
NSLogWindow::MakeVisible()
{
	Form->Visible = true ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSLogWindow::SetupWindow()
{
  TWindow::SetupWindow() ;

  ModifyStyle(WS_BORDER, WS_CHILD) ;
  Form->Show() ;
  MakeVisible() ;

  pContexte->setAideIndex("") ;
  pContexte->setAideCorps("dpio_log.htm") ;
}

// Fonction Navigate////////////////////////////////////////////////////////////////
void
NSLogWindow::Navigate()
{
  if ((NULL == Form) || (NULL == Form->Control))
    return ;

  if (string("") == sFileName)
  {
    string ps = string("NSLogWindow::Navigate with empty URL") ;
	  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

  Variant Flags(navNoReadFromCache) ;
  TVariant VFlags = Flags.operator TVariant() ;

try
{
  wchar_t buff[1024] ;
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sFileName.c_str(), -1, buff, sizeof(buff)) ;

  Form->Control->Navigate(buff, &VFlags) ;
}
catch (EOleException* e)
{
  string ps = string("Exception NSLogWindow::Navigate (") + string(e->Message.c_str()) + string(")") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
}
catch (Exception &ex)
{
  string ps = string("Exception NSLogWindow::Navigate (") + string(ex.Message.c_str()) + string(")") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
}
}

void
NSLogWindow::NavigateErrorEvent(int iStatusCode, string sURL)
{
	string ps = string("Ev : NavigateError for URL \"") + sURL + string("\"") ;
  ps += string(" (") + getNavigateErrorShortMsg(iStatusCode) + string(")") ;
	pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  erreur(ps.c_str(), standardError, 0) ;
}

void
NSLogWindow::Refresh()
{
  Form->Control->Refresh() ;
}

string
NSLogWindow::html2string(string s2convert)
{
	string	sResult = s2convert ;
	size_t  iBeginPos = sResult.find("<") ;
	while (iBeginPos != string::npos)
	{
		int	iEndPos = sResult.find(">") + 1 ;
		sResult.erase(iBeginPos, iEndPos - iBeginPos) ;
		iBeginPos = sResult.find("<") ;
	}
	return texteCourant(sResult) ;
}

void
NSLogWindow::CmClipCopy()
{
	if (OpenClipboard())
	{
		string	sclipdata = html2string(pContexte->getPredi()->getLogPage()) ;

		HGLOBAL	clipbuffer ;
		char		*buffer ;
		
		EmptyClipboard(); // Empty whatever's already there

		clipbuffer	= GlobalAlloc(GMEM_DDESHARE, strlen(sclipdata.c_str()) + 1) ;
		buffer			= (char *)GlobalLock(clipbuffer) ;
		strcpy(buffer, sclipdata.c_str()) ;
		GlobalUnlock(clipbuffer) ;
		SetClipboardData(CF_TEXT, clipbuffer) ; // Send the data
		CloseClipboard() ; // VERY IMPORTANT
	}
}

void
NSLogWindow::CmFermer()
{
  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  int retVal = MessageBox("Voulez-vous vraiment fermer ?", sCaption.c_str(), MB_YESNO) ;

  if (IDYES == retVal)
  {
    bCanClose = true ;
    if (pContexte && (pContexte->getSuperviseur()) && (pContexte->getPredi()))
    {
      pContexte->getPredi()->_pLogWnd    = 0 ;
      pContexte->getPredi()->_pLogMDIWnd = 0 ;
    }
    CloseWindow() ;
  }
}

void
NSLogWindow::CmImprimer()
{
  IDispatch* pdisp = Form->Control->Document ;

  IOleCommandTarget* command ;
  pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command) ;
  if (command)
  {
    command->Exec(NULL, Shdocvw_tlb::OLECMDID_PRINT, Shdocvw_tlb::OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL) ;
    command->Release() ;
  }

  pdisp->Release() ;
}

/****************** classe NSImportChild **************************/DEFINE_RESPONSE_TABLE1(NSLogChild, TMDIChild)
	 EV_WM_CLOSE,
	 EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;

NSLogChild::NSLogChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSLogWindow* clientWnd)					 :TMDIChild(parent, title, clientWnd), NSRoot(pCtx)
{
	pClient = clientWnd ;
}

NSLogChild::~NSLogChild()
{
}

voidNSLogChild::EvClose()
{
	if (false == pClient->bCanClose)
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
		int retVal = MessageBox("Voulez-vous vraiment fermer ?", sCaption.c_str(), MB_YESNO) ;

		if (IDYES == retVal)
			TWindow::EvClose() ;
	}
	else
		TWindow::EvClose() ;
}
////////////////////////// fin de nsbrowse.cpp ///////////////////////////////
