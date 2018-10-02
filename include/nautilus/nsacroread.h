//---------------------------------------------------------------------------
#ifndef NS_ACRO_READ_H#define NS_ACRO_READ_H

// #include "SHDocVw_OCX.h"
#include "nautilus\nsdocref.h"#include <owl\controlg.h>

class TWebProxy ;

class NSAcrobatDocument : public NSRefDocument
{
	public:

		char far nomFichier[255] ;       // fichier texte associé		char far szFichierExterne[255] ;

		NSAcrobatDocument(TDocument *parent, bool bROnly, NSDocumentInfo *pDocumentInfo,      					NSDocumentInfo* pDocHtmlInfo = 0, NSContexte *pCtx = 0) ;

		NSAcrobatDocument(TDocument *parent = 0, NSContexte *pCtx = 0) ;		~NSAcrobatDocument() ;

		void   SetNomFichier(string sFich) ;		void   SetFichierExterne(string sFich) ;		string GetDocExt() ;

    void   replaceTagsByValues() ;
    string pdfStringToWindowString(string *pPdfString) ;
    string pdfCharToWindowString(int iPdfValue) ;

		// Méthodes virtuelles de TDocument		bool Open(int mode, const char far *path = 0) ;
		bool Close() ;

		// Génération du html correspondant au document		bool GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
      						string sAdresseCorresp = "", string sPathDest = "") ;

		bool IsOpen() ;    void Visualiser() ;
		// Fonctions de gestion du document Nautilus		bool Renommer(string& sNomFichier, string& sLocalis, string sType = "ZTPDF", string* psNomBrut = 0) ;};


//
// Définition de la classe NSAcrobatView : Affichage des PDF
//

//******************************************************************//
//              CLASSE NSAcrobatView
//
//******************************************************************

class TAcrobatProxy;class NSAcrobatView : public TWindowView
{
	public :

  	// Données de visualisation
    NSAcrobatDocument* _pDoc ;		// pointeur sur document de référence

    bool _bSetupToolBar ; // booleen pour gérer la ToolBar

    NSAcrobatView(NSAcrobatDocument& doc, TWindow *parent = 0) ;
    ~NSAcrobatView() ;

    // Redéfiniton des fonctions virtuelles de TView    static LPCSTR StaticName() { return "NSAcrobatView" ; }
    const char*		GetViewName() { return StaticName() ; }
    TWindow*			GetWindow() ;
    bool					CanClose() ;

    // SetDocTitle et GetFileName    bool 					SetDocTitle(const char far* docname, int index)
      { return TWindow::SetDocTitle(docname, index) ; }

    const char far* GetFileName() { return GetDocument().GetTitle() ; }
    void EvClose() ;    void EvSize(uint sizeType, NS_CLASSLIB::TSize& size) ;    void EvTimer(uint timerId) ;    // Fonctions de la View    void PerformCreate(int menuOrId) ;
    void displayFile(string sFileName) ;

    // void SetParentProperty(HWND hWnd);    void MakeVisible() ;
    void SetupWindow() ;
    void SetupNavBar() ;
    void EvSetFocus(THandle hWndLostFocus) ;
    void EvKillFocus(THandle hWndGetFocus) ;
    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    void NavigateErrorEvent(int iStatusCode, string sURL) ;

    bool GenereHtml() ;
    void CmPublier() ;
    void CmFileSave() ;

    bool Enregistrer() ;

           HWND GetOleControlWindow() ;
    static HWND GetSonWindow(HWND hThis, int iLevel) ;

    static void TerminateProcess(HWND hTarget) ;
    static bool TerminateProcess(unsigned long dwProcessId, UINT uExitCode) ;

  protected:
  	bool  IsTabKeyMessage(MSG &msg) ;    bool  PreProcessMsg(MSG &msg) ;

    string getFileName() ;
    void   resizeOleControl(NS_CLASSLIB::TRect clientRect) ;

  private:
    // TAcrobatProxy* _AcrobatForm ;    TWebProxy*     _IEForm ;    TMenuDescr*    _pMUEViewMenu ;    HACCEL         _hAccelerator ;
	DECLARE_RESPONSE_TABLE(NSAcrobatView) ;};

#endif
// fin de nsAcroRead.h

