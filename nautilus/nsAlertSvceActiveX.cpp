// NSMailSvce.CPP : Incoming messages processing
////////////////////////////////////////////////////////////
// #include <cstring.h>
// #include <stdio.h>// #include <assert.h>#include <vcl.h>#pragma hdrstop#include "ActiveX.h"

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsbrowse.h"
#include "nautilus\nsAlertSvce.h"

// From https://edn.embarcadero.com/article/20536
//
// Question: How do I create an instance TForm in a DLL?
//
// Answer: To create an instance of a TForm derived object, pass the
// global Application instance to the constructor of the TForm object. For example,
// if you have the following form:
//
// class TForm1 : public TForm
// {
// __published:	// IDE-managed Components
// private:	// User declarations
// public:		// User declarations
//    __fastcall TForm1(TComponent* Owner);
// };
//
// and you wanted to create an instance of this class within a DLL, you would do the
// following:
//
// TForm1* form = new TForm1(Application);
//
// within the DLL.
//
// If the DLL and the EXE use different RTLs (for instance if they use different
// compilers. Note that you will not be able to share objects with Visual C++ because
// objects are stored differently in memory) then you should make sure to not
// allocate and deallocate memory accross the DLL and EXE boundries. A safe way to
// create the form and use it in the EXE is to export two functions from your DLL
// that allocate and deallocate the form. For example:
//
// #ifdef __DLL__
// #define EXPORT __declspec(dllexport)
// #else
// #define EXPORT __declspec(dllimport)
// #endif
//
// extern "C" TForm1* EXPORT CreateForm1()
// {
//    return new TForm1(Application);
// }
//
// extern "C" void EXPORT FreeForm1(TForm1* Value)
// {
//    delete Value;
// }
//
// So on the EXE side you can get an instance of TForm1 by calling:
//
// TForm1* form = CreateForm1();
//
// and you can free it by calling:
//
// FreeForm1(form);
//

// -----------------------------------------------------------------------------
//
// Méthodes de NSHtmlAlertViewerWindow//
// -----------------------------------------------------------------------------
NSHtmlAlertViewerWindow::NSHtmlAlertViewerWindow(NSContexte* pCtx, NSAlertServiceWindow *parent, int /*id*/, int /*x*/, int /*y*/, int /*w*/, int /*h*/, TModule* /*module*/)
                        :TWindow(parent), NSRoot(pCtx)
{
	_pParentWindow = parent ;

  _Form = (TWebProxy*) 0 ;
}

NSHtmlAlertViewerWindow::~NSHtmlAlertViewerWindow()
{
  // Delete de la Form
  if (_Form)
	  delete _Form ;
	// CoUninitialize();
	// OleUninitialize() ;
}

void
NSHtmlAlertViewerWindow::display(const string sBodyText)
{
  if ((NSAlertServiceWindow*) NULL != _pParentWindow)
    _pParentWindow->openHtmlViewer() ;

  _sBodyText = sBodyText ;
  if (string("") != _sBodyText)
		Navigate() ;
}

//
// Override a TWindow virtual to create the HWND directly.
// NSImportWindow and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSHtmlAlertViewerWindow::PerformCreate(int /*menuOrId*/)
{
  // OleInitialize(NULL) ;    // Use OleInitialize to get clipboard functionality
	// Form = new TWebProxy(NULL == Parent ? 0 : Parent->GetHandle(), 0) ;

  TMyApp* pApplication = pContexte->getSuperviseur()->getApplication() ;
  if ((TMyApp*) NULL == pApplication)
    return ;

  _Form = new TWebProxy(GetHandle(), 0) ;
  //
  // From https://edn.embarcadero.com/article/20536
  //
  //
  //
  // _Form = new TWebProxy((TComponent*) pApplication) ;
  // _Form = new TWebProxy((TComponent*) GetApplication()) ;

	_Form->Visible      = false ;
  _Form->BorderStyle  = bsNone ;  // Not resizeable; no visible border line
	_Form->ParentWindow = HWindow ; // ( NULL == Parent ? 0 : Parent->HWindow ) ;
  SetHandle(_Form->Handle) ;
  // ::SetParent(Forms::Application->Handle, pContexte->GetMainWindow()->HWindow) ;

	// on navigue vers le fichier html à importer
  if (string("") != _sBodyText)
		Navigate() ;
}

void
NSHtmlAlertViewerWindow::SetupWindow(){	TWindow::SetupWindow() ;	ShowWindow( SW_HIDE ) ;}
void
NSHtmlAlertViewerWindow::Navigate()
{
  if (((TWebProxy*) NULL == _Form) || ((TWebBrowser*) NULL == _Form->Control))
    return ;

  if (string("") == _sBodyText)
  {
    string ps = string("NSHtmlAlertViewerWindow::Navigate with empty body text") ;
	  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

  // _Form->Visible = true ;

  // string sAboutText = string("about:") + _sBodyText ;
  // string sAboutText = string("about:") + string("<h1>Test title</h1>") ;

  Variant Flags(navNoReadFromCache) ;
  TVariant VFlags = Flags.operator TVariant() ;

  wchar_t buff[1024] ;
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _sBodyText.c_str(), -1, buff, sizeof(buff)) ;

  _Form->Control->Navigate(buff, &VFlags) ;

/*
  IDispatch* pdisp = _Form->Control->Document ;

  _Form->Control->DocumentText = "0" ;
  _Form->Control->Document->OpenNew(true) ;
  pdisp->Write(buff) ;
  _Form->Control->Refresh() ;
*/
}

//////////////////////// ending nsMailSvceActiveX.cpp //////////////////////////
