#if !defined(__NS_TTX_H)#define __NS_TTX_H

class NSTtxDocument ;        // classe générique de documents texte nautilus
class TWordBasicProxy ;      // classe d'automation Word.Basic (par ProxyWizard)
class TAutoWordForm ;        // classe "Form" permettant de faire une vue - dialogue

class NSUtilDialog ;
class NSUtilEdit ;

#include <owl\docmanag.h>
#include <owl\docview.h>
#include <owl\mdichild.h>
#include <owl\decframe.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\applicat.h>
#include <owl\richedit.h>
#include <owl\richedpr.h>
#include <owl\choosefo.h>
#include <owl\richedit.h>

#include "nautilus\nsdocref.h"#include "nautilus\nautilus.rh"
#include "nautilus\nsmodhtm.h"
// #include "nautilus\richedap.h"
// #include "nsbb\nsednum.h"

#define MAXCARS 100000      // Limite du texte pour RichEdit
// #if defined(_DANSTXTDLL)// #define _NSTTXCLASS __export
// #else
// #define _NSTTXCLASS __import
// #endif

// Constantesconst int IDRichEd = 0x100;
const int UnChk    = TCommandEnabler::Unchecked;
const int Chk	   = TCommandEnabler::Checked;

#define _NSTTXCLASS _USERCLASS
// Vue pour les documents Word : windowview - dialogue d'automation
class NSAutoWordView : public TWindowView
{
  public :

    NSUtilDialog*    _pDialog ;        // on garde un dialogue comme toolbox    NSUtilEdit*      _pDocName ;
    bool             _bSessionEnCours ;
    TWordBasicProxy* _pControl ;
    wchar_t          _wcWindowName[255] ;
    NSTtxDocument*   _pDocTtx ;

    NSAutoWordView(NSTtxDocument& doc, TWindow *parent = 0) ;    ~NSAutoWordView() ;

    // fonction virtuelles de TView    static LPCSTR StaticName()  { return "NSAutoWordView" ; }
    const char*   GetViewName() { return StaticName() ; }
    TWindow*      GetWindow()   { return (TWindow*) this ; }

    void PerformCreate(int menuOrId) ;    void MakeVisible() ;
    void SetupWindow() ;
    void EvClose() ;
    bool CanClose() ;
    void FillStartupInfo(LPSTARTUPINFO psi) ;

    void CmLancer() ;    void CmQuitter() ;
    void CmArchiver() ;
    void CmPublier() ;
    void CmVisualiser() ;
    void CmEditer() ;
    void CmComposer() ;
    void CmFormules() ;
    void CmFileOpen() ;

    void ReplaceTags() ;

    bool GotoBookmark(string sBookmarkName) ;
    bool GotoStartOfDoc() { return GotoBookmark("\\StartOfDoc") ; }
    bool GotoEndOfDoc()   { return GotoBookmark("\\EndOfDoc") ; }

    short GetSelInfo(short iType) ;
    short GetSelStartRow()  { return GetSelInfo(13) ; }
    short GetSelStartCol()  { return GetSelInfo(16) ; }
    short GetSelEndRow()    { return GetSelInfo(14) ; }
    short GetSelEndCol()    { return GetSelInfo(17) ; }

    bool saveHTMLtoWORD(string sWordName) ;

  protected:
    bool IsTabKeyMessage(MSG &msg) ;    bool PreProcessMsg(MSG &msg) ;

	private:
    TAutoWordForm* _Form ;
  DECLARE_RESPONSE_TABLE(NSAutoWordView) ;};

class  NSTtxDocument : public NSRefDocument
{
	public:

		char far nomFichier[1024] ;       // fichier texte associé		char far szFichierExterne[1024] ;

    string   sDocTitle ;
    string   sDocLexique ;

    bool     bMustReplaceTagsWhenOpening ;

		NSTtxDocument(TDocument *parent, bool bROnly, NSDocumentInfo *pDocumentInfo,      					NSDocumentInfo* pDocHtmlInfo = 0, NSContexte *pCtx = 0) ;

		NSTtxDocument(TDocument *parent = 0, NSContexte *pCtx = 0) ;		~NSTtxDocument() {}

		void   SetNomFichier(string sFich) ;		void   SetFichierExterne(string sFich) ;    void   SetDocTitle(string sTitle)  { sDocTitle = sTitle ; }    void   SetDocLexique(string sCode) { sDocLexique = sCode ; }		string GetDocExt() ;
    string GetDocTitle()   { return sDocTitle ; }
    string GetDocLexique() { return sDocLexique ; }

		// Méthodes virtuelles de TDocument		bool Open(int mode, const char far *path = 0) ;
		bool Close() ;

		// Génération du html correspondant au document		bool GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
      						string sAdresseCorresp = "", string sPathDest = "") ;
    bool GenereHtmlText(string& sHtmlText) ;

		bool IsOpen() ;    void ReplaceTagsWhenOpening()     { bMustReplaceTagsWhenOpening = true ; }    void DontReplaceTagsWhenOpening() { bMustReplaceTagsWhenOpening = false ; }
    bool HaveToReplaceTags()          { return bMustReplaceTagsWhenOpening ; }

		// Fonctions de gestion du document Nautilus		bool Renommer(string& sNomFichier, string& sLocalis, string sType = "ZTRTF", string* psNomBrut = 0) ;};

class NSTtxView : public TView, public OWL::TRichEdit{
	public:

		NSTtxView(NSTtxDocument& doc, TWindow *parent = 0) ;    ~NSTtxView() ;

    // Redéfiniton des fonctions virtuelles de TView et TRichEdit    static LPCSTR StaticName() { return "NSTtxView" ; }
    const char*   GetViewName() { return StaticName() ; }
    TWindow*      GetWindow() ;
    bool 	        CanClose() ;           // de TEditFile
    bool	        CanClear() ;           // de TEditFile
    bool	        SetDocTitle(const char far* docname, int index)
    {
    	return OWL::TRichEdit::SetDocTitle(docname, index) ;
    }

  protected:
  	bool bSetupToolBar ;
    // Remplissage de la vue    void   FillViewData() ;
    void   SetupWindow() ;
    void   SetupTxtBar() ;
    void   InitDefaultFont() ;
    void   SetFormatFont(LOGFONT lf) ;
    void   RemplaceTagsRTF() ;
    string getText() ;

    // view event handlers    bool VnIsWindow(HWND hWnd) ;

    // Fonctions de réponse aux evts    void EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point) ;
    void EvChar(uint key, uint repeatCount, uint flags) ;
    void EvSetFocus(THandle hWndLostFocus) ;
    void EvKillFocus(THandle hWndGetFocus) ;
    bool EvQueryEndSession() ;
    void EvDestroy() ;

    // Overrides TWindows' in order to process our accelerators table
    bool PreProcessMsg(MSG &msg) ;

    // Fonctions d'appel des méthodes de NSRefDocument    void CmComposer() ;
    void CmPublier() ;
    void CmVisualiser() ;

    // Fonction d'appel des formules pré-enregistrées    void CmFormules() ;

    // Fonction d'appel des CR Nautidos
    void CmNautidos() ;

    // Redéfinition de ces fonctions pour gérer le flag Dirty    void CmEditCopy() ;
    void CmEditCut() ;
    void CmEditPaste() ;

    // Fonctions de sauvegarde    bool Enregistrer() ;
    bool EnregistrerSous() ;

    // File Command handlers    void CmFileNew() ;
    void CmFileOpen() ;
    void CmFileSave() ;
    void CmFileSaveAs() ;
    void CmFilePrint() ;
    void CmFilePrintPreview() ;
    void CmOuvrir() ;
    void CmAutoFilling() ;

    // Format Command handlers    void CmFormatFont() ;
    void CmFormatPara() ;
    void CmFormatTabs() ;
    void CmFormatBold() ;
    void CmFormatItalic() ;
    void CmFormatUnderline() ;
    void CmFormatStrikeout() ;
    void CmFormatTextData() ;
    void CmFormatRTFData() ;

    void CeFormatFont(TCommandEnabler&) ;    void CeFormatPara(TCommandEnabler&) ;
    void CeFormatTabs(TCommandEnabler&) ;
    void CeFormatBold(TCommandEnabler&) ;
    void CeFormatItalic(TCommandEnabler&) ;
    void CeFormatUnderline(TCommandEnabler&) ;
    void CeFormatStrikeout(TCommandEnabler&) ;
    void CeFormatTextData(TCommandEnabler&) ;
    void CeFormatRTFData(TCommandEnabler&) ;

    bool EnDropFiles(TEnDropFiles&)	   { return false ; }    bool EnMsgFilter(OWL::TMsgFilter&) { return false ; }
    bool EnProtected(TEnProtected&)	   { return false ; }
    void EnReqResize(TReqResize&)	{}

    void initMUEViewMenu(string sMenuName) ;
    void activateMUEViewMenu() ;

    // Variables    NSTtxDocument 	 *pDocTtx ;	 // Pointeur sur document TTX
    OWL::TControlBar *pToolBar ; // Barre d'outils
    OWL::TPrinter	   *pPrinter ; // Printer

    TMenuDescr       *pMUEViewMenu ;
    HACCEL           hAccelerator ;

    LOGFONT          _defaultFont ;

  DECLARE_RESPONSE_TABLE(NSTtxView) ;};

//	DEFINE_DOC_TEMPLATE_CLASS(NSTtxDocument,NSTtxView,TtxDlgViewTmpl);// s'utilise avec BEGIN / END REGISTRATION

// Classe NSSimpleTxtDocument : document pour les fichiers txt (bloc-note)
class  NSSimpleTxtDocument : public NSRefDocument
{
	public:

  	char far nomFichier[255] ;       // fichier texte associé
    NSSimpleTxtDocument(TDocument *parent, bool bROnly, NSDocumentInfo *pDocumentInfo,                     NSDocumentInfo* pDocHtmlInfo = 0, NSContexte *pCtx = 0) ;

    NSSimpleTxtDocument(TDocument *parent = 0, NSContexte *pCtx = 0) ;
    ~NSSimpleTxtDocument() {}

    void   SetNomFichier(string sFich) ;    string GetDocExt() ;

    // Méthodes virtuelles de TDocument    bool Open(int mode, const char far *path = 0) ;
    bool Close() ;

    // Génération du html correspondant au document    bool GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
      						string sAdresseCorresp = "", string sPathDest = "") ;
    bool GenereHtmlText(string& sHtmlText) ;

    bool IsOpen() ;
    // Fonctions de gestion du document Nautilus    bool Renommer(string& sNomFichier, string& sLocalis, string sType = "TNTXT", string* psNomBrut = 0) ;
};

// Classe NSSimpleTxtView : vue pour les fichiers txt (bloc-note)
class NSSimpleTxtView : public TView, public OWL::TEditFile
{
	public:

  	NSSimpleTxtView(NSSimpleTxtDocument& doc, TWindow *parent = 0) ;    ~NSSimpleTxtView() ;

    // Redéfiniton des fonctions virtuelles de TView et TRichEdit    static LPCSTR StaticName() { return "NSSimpleTxtView"; }
    const char*   GetViewName() { return StaticName(); }
    TWindow*      GetWindow();
    bool 	        CanClose();           // de TEditFile
    bool	        CanClear();           // de TEditFile
    // bool	      SetDocTitle(const char far* docname, int index);

  protected:
  	bool bSetupToolBar;

    // Remplissage de la vue    void FillViewData();
    void SetupWindow();
    void SetupTxtBar();

    // view event handlers    bool VnIsWindow(HWND hWnd);

    // Fonctions de réponse aux evts    void EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point);
    void EvChar(uint key, uint repeatCount, uint flags);
    void EvSetFocus(THandle hWndLostFocus);
    void EvKillFocus(THandle hWndGetFocus);
    bool EvQueryEndSession();

    // Overrides TWindows' in order to process our accelerators table
    bool PreProcessMsg(MSG &msg) ;

    // Fonctions d'appel des méthodes de NSRefDocument    void CmComposer();
    void CmPublier();
    void CmVisualiser();

    // Fonction d'appel des formules pré-enregistrées    void CmFormules();

    // Redéfinition de ces fonctions pour gérer le flag Dirty    void CmEditCopy();
    void CmEditCut();
    void CmEditPaste();

    // Fonctions de sauvegarde    bool Enregistrer();
    bool EnregistrerSous();

    // File Command handlers    void CmFileNew();
    void CmFileOpen();
    void CmFileSave();
    void CmFileSaveAs();

    bool EnDropFiles(TEnDropFiles&)	{ return false; }    bool EnMsgFilter(OWL::TMsgFilter&)	{ return false; }
    bool EnProtected(TEnProtected&)	{ return false; }
    void EnReqResize(TReqResize&)	{}

    void initMUEViewMenu(string sMenuName) ;
    void activateMUEViewMenu() ;

    // Variables    NSSimpleTxtDocument *pDocTxt ;	// Pointeur sur document TTX
    OWL::TControlBar    *pToolBar ;	// Barre d'outils

    TMenuDescr          *pMUEViewMenu ;
    HACCEL              hAccelerator ;

  DECLARE_RESPONSE_TABLE(NSSimpleTxtView) ;};

#endif		// __NS_TTX_H
