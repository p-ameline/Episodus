//---------------------------------------------------------------------------
#ifndef NS_VISUAL_H#define NS_VISUAL_H

#include <owl\controlg.h>
#include <owl\edit.h>

class NSHISTODocument ;

// #include "SHDocVw_OCX.h"
#include "nsoutil\nsvisualview.h"
#include "nautilus\nsdocref.h"
class NSVisualView ;

class NSEditUrl : public NSEditUrlBase{
	public :

  	NSVisualView* _pVue ;
    NSEditUrl(NSVisualView* pView) ;    ~NSEditUrl() ;

    void EvChar(uint key, uint repeatCount, uint flags) ;
	DECLARE_RESPONSE_TABLE(NSEditUrl) ;};

class NSHtmlModelDocument : public NSRefDocument
{
	public:

		string _sActiveFileName ;       // fichier texte associ�		string _sModelFileName ;
    string _sPathCode ;

    string _sDocTitle ;
    string _sDocLexique ;

    bool   _bTemporaryDoc ;

		NSHtmlModelDocument(TDocument *parent, bool bROnly, NSDocumentInfo *pDocumentInfo,      					NSDocumentInfo* pDocHtmlInfo = 0, NSContexte *pCtx = 0) ;

		NSHtmlModelDocument(TDocument *parent = 0, NSContexte *pCtx = 0) ;		~NSHtmlModelDocument() ;

		void   SetModelFile(string sFileName) ;		string GetDocExt() ;

    void   replaceTagsByValues() ;

		// M�thodes virtuelles de TDocument		bool Open(int mode, const char far *path = 0) ;
		bool Close() ;

		// G�n�ration du html correspondant au document		bool GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
      						string sAdresseCorresp = "", string sPathDest = "") ;
    bool GenereHtmlText(string& sHtmlText) ;

		bool   IsOpen() ;    void   Visualiser() ;
    bool   SaveNewFile() ;
    bool   SaveExistingFile() ;

		// Fonctions de gestion du document Nautilus		bool   Renommer(string &sNewFileName) ;    void   SetDocTitle(string sTitle)  { _sDocTitle = sTitle ; }    void   SetDocLexique(string sCode) { _sDocLexique = sCode ; }    string GetDocTitle()   { return _sDocTitle ; }    string GetDocLexique() { return _sDocLexique ; }    bool   IsTemporaryDoc() { return _bTemporaryDoc ; }};

//******************************************************************
//
//              CLASSE NSVisualView
//
//******************************************************************

class TExamHistoForm ;
class CWebBrowserPrint ;
class NSVisualView : public NSVisualViewBase
{
	public :

  	// Donn�es de visualisation
    NSRefDocument* _pDocRef ;	  // pointeur sur document de r�f�rence

    string 		     _sLettreHtml ;   // fichier html de lettre � imprimer    bool			     _bSetupToolBar ; // booleen pour g�rer la ToolBar

    // Param�tres d'impression

    int         delai ;
    bool        bUseHook ;
    bool        bSimpleCtrl ;
    bool        bVerbose ;
    bool        bKillTmp ;

    bool        bWaitImp ;
    int         iTempImp ;
    int         iTempPostNav ;
    bool        bWaitPrintComplete ;
    NSVisualView(NSRefDocument& doc, TWindow *parent = 0) ;
    NSVisualView(NSHtmlModelDocument* pDoc, TWindow *parent = 0) ;
    ~NSVisualView() ;

    // Red�finiton des fonctions virtuelles de TView    static LPCSTR	StaticName()  { return "NSVisualView" ; }
    const char*		GetViewName() { return StaticName() ; }
    TWindow*			GetWindow() ;
    bool					CanClose() ;

    // SetDocTitle et GetFileName    bool 					SetDocTitle(const char far* docname, int index)
      { return TWindowView::SetDocTitle(docname, index) ; }

    const char far* GetFileName() { return GetDocument().GetTitle() ; }
    void            EvClose() ;
    // Fonctions de la VisualView    void  PerformCreate(int menuOrId) ;
    void  SetURL(const char* _URL) ;
    const char* GetURL() ;

    // void SetParentProperty(HWND hWnd);    void  MakeVisible() ;
    void 	SetupWindow() ;
    void  SetupNavBar() ;
    void  SetPrintParams(NSPrintParams& nspp);
    void 	EvSetFocus(THandle hWndLostFocus) ;
    void 	EvKillFocus(THandle hWndGetFocus) ;
    void  EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void  EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void  Navigate(string sUrlTarget) ;
    // void  ExecWB(Shdocvw_tlb::OLECMDID iCommand, Shdocvw_tlb::OLECMDEXECOPT iOption) ;
    // void  ExecWB(int iCommand, int iOption) ;
    void  ExecWB_old(int iCommand, int iOption) ;
    // modif RS du 09/04/04 : pour int�grer WebBrowserPrint
    void  ExecWB(int iCommand, int iNbExpl, bool bVerbose = false) ;
    void  PrintComplete() ;
    void  NavigateComplete() ;
    void  postNavigateComplete() ;
    void  NavigateErrorEvent(int iStatusCode, string sURL) ;
    bool  GenereHtml() ;
    void  initValues() ;
    void  initParams() ;
    void  Tempo() ;
    bool  ImprimerLettre() ;
    void  CmPublier() ;
    void  CmEditer() ;
    void  CmComposer() ;
    void  DetruireTemp() ;
    void  Rafraichir() ;
    void  CmFileClose() ;
    void  CmPrecedent() ;
    void  CmSuivant() ;

    void  openPrevSameKindDocument() ;
    void  openNextSameKindDocument() ;

  protected:
  	bool  IsTabKeyMessage(MSG &msg) ;    bool  PreProcessMsg(MSG &msg) ;

  // private:

    // TWebProxy*        Form ;    TExamHistoForm*   Form ;
    CWebBrowserPrint* pBrowserPrint ;
    TMenuDescr*       pMUEViewMenu ;
    HACCEL            hAccelerator ;

    NSHISTODocument*  getDocManager() ;
    NSDocumentInfo*   getPrevSameKindDocument() ;
    NSDocumentInfo*   getNextSameKindDocument() ;

  DECLARE_RESPONSE_TABLE(NSVisualView) ;};

class NSHtmlModelView : public NSVisualView
{
	public :

  	// Donn�es de visualisation
    NSHtmlModelDocument* pDoc ;		// pointeur sur document de r�f�rence

    bool bSetupToolBar ; // booleen pour g�rer la ToolBar

    NSHtmlModelView(NSHtmlModelDocument& doc, TWindow *parent = 0) ;
    ~NSHtmlModelView() ;

    // Red�finiton des fonctions virtuelles de TView    static LPCSTR StaticName() { return "NSHtmlModelView" ; }
    const char*		GetViewName() { return StaticName() ; }
    TWindow*			GetWindow() ;
    bool					CanClose() ;

    // SetDocTitle et GetFileName    bool 					SetDocTitle(const char far* docname, int index)
      { return TWindowView::SetDocTitle(docname, index) ; }

    const char far* GetFileName() { return GetDocument().GetTitle() ; }
    void EvClose() ;    void EvSize(uint sizeType, NS_CLASSLIB::TSize& size) ;    // void SetParentProperty(HWND hWnd);
    void SetupWindow() ;
    void SetupNavBar() ;
    void EvSetFocus(THandle hWndLostFocus) ;
    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    bool GenereHtml() ;
    void CmPublier() ;
    void CmFileSave() ;

    bool Enregistrer() ;

	DECLARE_RESPONSE_TABLE(NSHtmlModelView) ;};

#endif
// fin de nsvisual.h

