//---------------------------------------------------------------------------// #include "stdafx.h"

#include <vcl.h>
#pragma hdrstop

#include <dstring.h>#include "WebBrowserPrint.h"#include "ActiveX.h"

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
#include "nsoutil\nsvisualview.h"
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

//---------------------------------------------------------------------------//
//Helper classes for toolbar edit box:
//
//---------------------------------------------------------------------------

void  TEditGadgetEnabler::SetText(const char far* txt)
{
	char oldtxt[256] ;
  OWL::TEdit* edit = dynamic_cast<OWL::TEdit*>(Gadget->GetControl()) ;
  edit->GetText(oldtxt, 255) ;
  if (strcmp(txt, oldtxt) != 0)
  	edit->SetText(txt) ;
}

void TEditGadget::CommandEnable(){
	if (Window->GetHandle() && (GetFocus() != GetControl()->HWindow))
		Window->HandleMessage(WM_COMMAND_ENABLE, 0, TParam2(&TEditGadgetEnabler(*Window, this)));
}

DEFINE_RESPONSE_TABLE1(TNotifyEdit, OWL::TEdit)	EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

void TNotifyEdit::EvKeyDown(uint key, uint repeatCount, uint flags){
	if (key == VK_RETURN)
  {
  	if (text)
    	delete[] text ;

  	text = new char[GetTextLen() + 1] ;
    GetText(text, GetTextLen() + 1) ;
    Parent->HandleMessage(WM_COMMAND, GetId(), 0) ;
  }
  else
  	OWL::TEdit::EvKeyDown(key, repeatCount, flags) ;
}

TAPointer<char> TNotifyEdit::text = 0;

const char* TNotifyEdit::Text()
{
	return (const char*)text ;
}

// Classe NSEditUrl pour la barre d'edition des url
////////////////////////////////////////////////////////////////

DEFINE_RESPONSE_TABLE1(NSEditUrlBase, OWL::TEdit)
	EV_WM_CHAR,
END_RESPONSE_TABLE;

NSEditUrlBase::NSEditUrlBase(NSVisualViewBase* pView) : OWL::TEdit(0, 751, "", 0, 0, 200, 24, 0)
{
	_pVue = pView;
}

NSEditUrlBase::~NSEditUrlBase()
{
}

// Table de réponses de la classe NSVisualView
////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE1(NSVisualViewBase, TWindowView)
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_COMMAND(CM_FILECLOSE, EvClose),END_RESPONSE_TABLE;

// Constructeur NSVisualView
////////////////////////////////////////////////////////////////

NSVisualViewBase::NSVisualViewBase(NSSuper* pSuper, TDocument& doc, TWindow* parent)                 :TWindowView(doc, parent), _pSuper(pSuper){
  initBaseValues() ;
}

void
NSVisualViewBase::initBaseValues()
{
  _sUrl    = string("") ;
  _sHtml   = string("") ;

  _bNewNav = true ;
  _nbNav   = 0 ;
  _page    = 0 ;
  _lastUrl = AnsiString("") ;
}

// Destructeur NSVisualView////////////////////////////////////////////////////////////////

NSVisualViewBase::~NSVisualViewBase(){
}

// GetWindow renvoie this////////////////////////////////////////////////////////////////

TWindow*NSVisualViewBase::GetWindow()
{
	return (TWindow*) this ;
}

// Fonction SetupWindow////////////////////////////////////////////////////////////////

voidNSVisualViewBase::SetupWindow()
{
	TWindowView::SetupWindow() ;

	/******************************	ModifyStyle(WS_BORDER, 0);
	MakeVisible();
	EnableWindow(true);
	********************************/
	// if (useformsize){
	// ::SetWindowPos(Parent->HWindow, NULL, 0, 0, Form->Width, Form->Height, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOZORDER);
	// }
}
voidNSVisualViewBase::EvClose()
{
	TWindowView::EvClose() ;
}

// Fonctions EvSetFocus et EvKillFocus pour la control Bar
////////////////////////////////////////////////////////////////
void
NSVisualViewBase::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
  TWindowView::EvSetFocus(hWndLostFocus);
}

voidNSVisualViewBase::EvKillFocus(THandle hWndGetFocus /* may be 0 */)
{
	TWindowView::EvKillFocus(hWndGetFocus) ;
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

boolNSVisualViewBase::IsTabKeyMessage(MSG &msg)
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
NSVisualViewBase::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;

	if (false == result)
		result = TWindow::PreProcessMsg(msg) ;

	return result ;
}

const char*NSVisualViewBase::GetURL()
{
	return _URL ;
}
// fin de nsvisualview.cpp/////////////////////////////////////////////////////////////////

