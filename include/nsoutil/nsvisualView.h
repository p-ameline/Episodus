//---------------------------------------------------------------------------
#ifndef NS_VISUALVIEW_H#define NS_VISUALVIEW_H

#include <owl\controlg.h>
#include <owl\edit.h>

class NSHISTODocument ;

// #include "SHDocVw_OCX.h"
#include "nautilus\nsdocref.h"#include "nsoutil/nsexport.h"
//
// HTTP Response Status Codes: some of them can be found in wininet.h
//

#define HTTP_STATUS_CONTINUE            100 // OK to continue with request
#define HTTP_STATUS_SWITCH_PROTOCOLS    101 // server has switched protocols in upgrade header

#define HTTP_STATUS_OK                  200 // request completed
#define HTTP_STATUS_CREATED             201 // object created, reason = new URI
#define HTTP_STATUS_ACCEPTED            202 // async completion (TBS)
#define HTTP_STATUS_PARTIAL             203 // partial completion
#define HTTP_STATUS_NO_CONTENT          204 // no info to return
#define HTTP_STATUS_RESET_CONTENT       205 // request completed, but clear form
#define HTTP_STATUS_PARTIAL_CONTENT     206 // partial GET furfilled

#define HTTP_STATUS_AMBIGUOUS           300 // server couldn't decide what to return
#define HTTP_STATUS_MOVED               301 // object permanently moved
#define HTTP_STATUS_REDIRECT            302 // object temporarily moved
#define HTTP_STATUS_REDIRECT_METHOD     303 // redirection w/ new access method
#define HTTP_STATUS_NOT_MODIFIED        304 // if-modified-since was not modified
#define HTTP_STATUS_USE_PROXY           305 // redirection to proxy, location header specifies proxy to use
#define HTTP_STATUS_REDIRECT_KEEP_VERB  307 // HTTP/1.1: keep same verb

#define HTTP_STATUS_BAD_REQUEST         400 // invalid syntax
#define HTTP_STATUS_DENIED              401 // access denied
#define HTTP_STATUS_PAYMENT_REQ         402 // payment required
#define HTTP_STATUS_FORBIDDEN           403 // request forbidden
#define HTTP_STATUS_NOT_FOUND           404 // object not found
#define HTTP_STATUS_BAD_METHOD          405 // method is not allowed
#define HTTP_STATUS_NONE_ACCEPTABLE     406 // no response acceptable to client found
#define HTTP_STATUS_PROXY_AUTH_REQ      407 // proxy authentication required
#define HTTP_STATUS_REQUEST_TIMEOUT     408 // server timed out waiting for request
#define HTTP_STATUS_CONFLICT            409 // user should resubmit with more info
#define HTTP_STATUS_GONE                410 // the resource is no longer available
#define HTTP_STATUS_LENGTH_REQUIRED     411 // the server refused to accept request w/o a length
#define HTTP_STATUS_PRECOND_FAILED      412 // precondition given in request failed
#define HTTP_STATUS_REQUEST_TOO_LARGE   413 // request entity was too large
#define HTTP_STATUS_URI_TOO_LONG        414 // request URI too long
#define HTTP_STATUS_UNSUPPORTED_MEDIA   415 // unsupported media type

#define HTTP_STATUS_RETRY_WITH          449 // The request should be retried after doing the appropriate action

#define HTTP_STATUS_SERVER_ERROR        500 // internal server error
#define HTTP_STATUS_NOT_SUPPORTED       501 // required not supported
#define HTTP_STATUS_BAD_GATEWAY         502 // error response received from gateway
#define HTTP_STATUS_SERVICE_UNAVAIL     503 // temporarily overloaded
#define HTTP_STATUS_GATEWAY_TIMEOUT     504 // timed out waiting for gateway
#define HTTP_STATUS_VERSION_NOT_SUP     505 // HTTP version not supported

//
//
//

#ifndef __urlmon_h__ // these symbols may come from URLMON.H

#define INET_E_INVALID_URL               _HRESULT_TYPEDEF_(0x800C0002L)
#define INET_E_NO_SESSION                _HRESULT_TYPEDEF_(0x800C0003L)
#define INET_E_CANNOT_CONNECT            _HRESULT_TYPEDEF_(0x800C0004L)
#define INET_E_RESOURCE_NOT_FOUND        _HRESULT_TYPEDEF_(0x800C0005L)
#define INET_E_OBJECT_NOT_FOUND          _HRESULT_TYPEDEF_(0x800C0006L)
#define INET_E_DATA_NOT_AVAILABLE        _HRESULT_TYPEDEF_(0x800C0007L)
#define INET_E_DOWNLOAD_FAILURE          _HRESULT_TYPEDEF_(0x800C0008L)
#define INET_E_AUTHENTICATION_REQUIRED   _HRESULT_TYPEDEF_(0x800C0009L)
#define INET_E_NO_VALID_MEDIA            _HRESULT_TYPEDEF_(0x800C000AL)
#define INET_E_CONNECTION_TIMEOUT        _HRESULT_TYPEDEF_(0x800C000BL)
#define INET_E_INVALID_REQUEST           _HRESULT_TYPEDEF_(0x800C000CL)
#define INET_E_UNKNOWN_PROTOCOL          _HRESULT_TYPEDEF_(0x800C000DL)
#define INET_E_SECURITY_PROBLEM          _HRESULT_TYPEDEF_(0x800C000EL)
#define INET_E_CANNOT_LOAD_DATA          _HRESULT_TYPEDEF_(0x800C000FL)
#define INET_E_CANNOT_INSTANTIATE_OBJECT _HRESULT_TYPEDEF_(0x800C0010L)
#define INET_E_DEFAULT_ACTION            _HRESULT_TYPEDEF_(0x800C0011L)
#define INET_E_REDIRECT_FAILED           _HRESULT_TYPEDEF_(0x800C0014L)
#define INET_E_REDIRECT_TO_DIR           _HRESULT_TYPEDEF_(0x800C0015L)
#define INET_E_CANNOT_LOCK_REQUEST       _HRESULT_TYPEDEF_(0x800C0016L)

#endif

#define INET_E_USE_EXTEND_BINDING        _HRESULT_TYPEDEF_(0x800C0017L)
#define INET_E_TERMINATED_BIND           _HRESULT_TYPEDEF_(0x800C0018L)
#define INET_E_INVALID_CERTIFICATE       _HRESULT_TYPEDEF_(0x800C0019L)
#define INET_E_BLOCKED_REDIRECT_XSECURITYID _HRESULT_TYPEDEF_(0x800C001BL)
#define INET_E_CODE_DOWNLOAD_DECLINED    _HRESULT_TYPEDEF_(0x800C0100L)
#define INET_E_RESULT_DISPATCHED         _HRESULT_TYPEDEF_(0x800C0200L)
#define INET_E_CANNOT_REPLACE_SFP_FILE   _HRESULT_TYPEDEF_(0x800C0300L)
#define INET_E_CODE_INSTALL_SUPPRESSED   _HRESULT_TYPEDEF_(0x800C0400L)
#define INET_E_CODE_INSTALL_BLOCKED_BY_HASH_POLICY   _HRESULT_TYPEDEF_(0x800C0500L)

string getNavigateErrorShortMsg(int iStatusCode) ;
string getNavigateErrorDetailedMsg(int iStatusCode) ;

//---------------------------------------------------------------------------//
// Helper classes for toolbar edit box:
//
//---------------------------------------------------------------------------

class _NSOUTILCLASSE TEditGadget: public TControlGadget{
	public:

  	TEditGadget(TWindow& control, TBorderStyle style = None): TControlGadget(control, style){}    ~TEditGadget(){}

    virtual void CommandEnable() ;};

class _NSOUTILCLASSE TEditGadgetEnabler : public TCommandEnabler{
	public:

  	TEditGadgetEnabler(TWindow::THandle hReceiver, TEditGadget* g)            : TCommandEnabler(g->GetControl()->GetId(), hReceiver), Gadget(g){}

    void SetText(const char far*) ;    void SetCheck(int) {}

	protected:
		TEditGadget* Gadget ;};

class TNotifyEdit: public OWL::TEdit{
	public:

  	TNotifyEdit(TWindow* parent, int id, const char far* text,    			      int x, int y, int w, int h, uint textLimit = 0,
                bool multiline = false, TModule* module = 0):
                OWL::TEdit(parent, id, text, x, y, w, h, textLimit,
                    multiline, module) {}
    ~TNotifyEdit() ;

    static TAPointer<char> text ;    static const char* Text() ;
    void EvKeyDown(uint key, uint repeatCount, uint flags) ;

	DECLARE_RESPONSE_TABLE(TNotifyEdit) ;};

//// Définition de la classe NSVisualView : Affichage des documents dans une window
//

class NSVisualViewBase ;
class _NSOUTILCLASSE NSEditUrlBase : public OWL::TEdit{
	public :

  	NSVisualViewBase* _pVue ;
    NSEditUrlBase(NSVisualViewBase* pView) ;    virtual ~NSEditUrlBase() ;

    virtual void EvChar(uint key, uint repeatCount, uint flags) = 0 ;
	DECLARE_RESPONSE_TABLE(NSEditUrlBase) ;};

//******************************************************************
//
//              CLASSE NSVisualViewBase
//
//******************************************************************

class TWebProxy ;
class _NSOUTILCLASSE NSVisualViewBase : public TWindowView
{
	public :

  	// Données de visualisation    string 		     _sUrl ; 		    // url de référence
    string 		     _sHtml ;        // fichier html à visualiser/imprimer

    NSEditUrlBase* _pEditUrl ;     // Pointeur sur le champ Edit d'Url

    bool			     _bNewNav ;		  // booleen pour tenir compte des nav par liens
    int			       _nbNav ;        // compteur des navigations effectuées
    int			       _page ;			    // compteur de pages
    AnsiString     _lastUrl ;

    NSVisualViewBase(NSSuper* pSuper, TDocument& doc, TWindow* parent = 0) ;
    virtual ~NSVisualViewBase() ;

    // Redéfiniton des fonctions virtuelles de TView    static LPCSTR	StaticName()  { return "NSVisualViewBase" ; }
    const char*		GetViewName() { return StaticName() ; }
    TWindow*			GetWindow() ;

    // SetDocTitle et GetFileName    bool 					SetDocTitle(const char far* docname, int index)
      { return TWindow::SetDocTitle(docname, index) ; }

    void            EvClose() ;
    // Fonctions de la VisualView    const char* GetURL() ;

    // void SetParentProperty(HWND hWnd);    void 	SetupWindow() ;
    void  SetupNavBar() ;
    void  SetPrintParams(NSPrintParams& nspp);
    void 	EvSetFocus(THandle hWndLostFocus) ;
    void 	EvKillFocus(THandle hWndGetFocus) ;
    void  EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    // void  ExecWB(Shdocvw_tlb::OLECMDID iCommand, Shdocvw_tlb::OLECMDEXECOPT iOption) ;
    void  ExecWB(int iCommand, int iOption) ;

  protected:

  	bool  IsTabKeyMessage(MSG &msg) ;    bool  PreProcessMsg(MSG &msg) ;
    void  initBaseValues() ;

  // private:

  	char        _URL[2048] ;
    NSSuper*    _pSuper ;
  DECLARE_RESPONSE_TABLE(NSVisualViewBase) ;};

#endif     // NS_VISUALVIEW_H


