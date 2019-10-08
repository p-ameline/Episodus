// NSBROWSE.H : D�finition des objets dialogues contenant ie4/////////////////////////////////////////////////////////////
#if !defined(__NSBROWSE_H)#define __NSBROWSE_H
#include <owl\edit.h>#include <owl\controlg.h>#include <owl\docmanag.h>#include <owl\mdichild.h>#include <owl\checkbox.h>#include <owl\groupbox.h>

class NSContexte ;

class NSRefDocument ;
class TWebCompos ;class TWebImport ;class TPdfImport ;class TWebLog ;class TImportImg ;class TLettreType ;
// #include "nautilus\nssuper.h"#include "partage\ns_vector.h"
#include "partage\NTArray.h"
#include "nsbb\nsutidlg.h"
#include "nssavoir\nscaptur.h"

#ifndef __EPIPUMP__// #include <exdisp.h>typedef enum BrowserNavConstants {   navOpenInNewWindow   = 1,
   navNoHistory         = 2,
   navNoReadFromCache   = 4,
   navNoWriteToCache    = 8
} BrowserNavConstants ;#endif//---------------------------------------------------------------------------//// Classes controle browser :////---------------------------------------------------------------------------
class NSComposView : public TWindowView{	public:

		string 				 fichCompo ;    NSRefDocument* pDocBrut ;

		NSComposView(NSRefDocument& doc, TWindow *parent = 0) ;
    ~NSComposView() ;

    // Red�finiton des fonctions virtuelles de TView
    static LPCSTR StaticName() { return "NSComposView" ; }
    const char*   GetViewName() { return StaticName() ; }
    TWindow*      GetWindow() ;
    void          EvClose() ;

    void PerformCreate(int menuOrId) ;
    void MakeVisible() ;
    void SetupWindow() ;
    void Navigate(string url) ;
    void NavigateErrorEvent(int iStatusCode, string sURL) ;
    void CmOk() ;
  	void CmCancel() ;

	protected:

		bool IsTabKeyMessage(MSG &msg);		bool PreProcessMsg(MSG &msg);

	private:

		TWebCompos* Form ;    HACCEL      hAccelerator ;
	DECLARE_RESPONSE_TABLE(NSComposView);};

// Classe TWindow permettant de visualiser les fichiers avant importation// R�alise selon les cas l'importation ou le r�f�rencement du fichier visualis�
/////////////////////////////////////////////////////////////////////////////////
class NSImportWindow : public TWindow
{
	public:

  	bool           _bNavOk ;
    bool           _bCanClose ;
    string				 _sHtml ;
    string				 _sFileName ;     // path + nom + ext
    string				 _sPathName ;     // path
    string				 _sNomFichier ;   // nom + ext
    string				 _sExtension ;    // ext
    string				 _sNomSeul ;      // nom
    NSRefDocument* _pNewDoc ;       // Document � cr�er dans nautilus
    string				 _sTypeNautilus ; // Type nautilus
    NSContexte* 	 _pContexte ;

    NSImportWindow(TWindow* parent, string sFichier, NSContexte* pCtx, bool bAutoMode = false) ;
    ~NSImportWindow() ;

    bool   CanClose() ;
    void   EvClose() ;
    void   EvTimer(uint timerId) ;
    void   EvSize(uint sizeType, NS_CLASSLIB::TSize& size) ;
    void   resizeOleControl(NS_CLASSLIB::TRect clientRect) ;
    void   PerformCreate(int menuOrId) ;
    void   MakeVisible() ;
    void   SetupWindow() ;
    void   Navigate() ;
    void   NavigateErrorEvent(int iStatusCode, string sURL) ;
    bool   InitInfosFichier() ;
    bool   InitNautilusType() ;
    bool   GenereHtml() ;
    void   CmImporter() ;
    void   CmReferencer() ;
    void   CmCapturer() ;
    void   CmAnnuler() ;
    bool   convertGdata(string* psFichier) ;
    bool   importHPRIM2(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT) ;
    bool   importHPRIM1(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT) ;
    bool   parseHPRIM1header(ifstream* pInFile, NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT) ;
    string getDateFromHPRIM1date(string sHPRIMdate, string s2kTreshold) ;

    bool   okToClose()       { return _bCanClose ; }
    bool   isNavOk()         { return _bNavOk ; }
    string getExtension()    { return _sExtension ; }
    string getNautilusType() { return _sTypeNautilus ; }

    void   ClosingPatch() ;

	protected:

    bool _bAutomaticImportMode ;
    bool _bImportSuccessful ;

		bool IsTabKeyMessage(MSG &msg) ;    bool PreProcessMsg(MSG &msg) ;

    void displayFile(const string sFileName) ;

	private:

		TWebImport* _Form ;    TPdfImport* _PdfForm ;    HACCEL      _hAccelerator ;
	DECLARE_RESPONSE_TABLE(NSImportWindow) ;
};

// Classe TMDIChild pour la red�finition de EvClose/////////////////////////////////////////////////////////
class NSImportChild : public TMDIChild, public NSRoot
{
	public :

		NSImportWindow* pClient ;

		NSImportChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSImportWindow* clientWnd = 0) ;
		~NSImportChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSImportChild) ;
};

// Classe TWindow permettant de visualiser les r�sultats de DPIO
/////////////////////////////////////////////////////////////////////////////////
class NSLogWindow : public TWindow
{
 public:
	NSLogWindow(TWindow *parent, string sFichier, NSContexte *pCtx) ;
	~NSLogWindow() ;

	void					EvClose() ;
	void					PerformCreate(int menuOrId) ;
	void					MakeVisible() ;
	void					SetupWindow() ;
	void					Navigate() ;
  void          NavigateErrorEvent(int iStatusCode, string sURL) ;
	void					Refresh() ;
	void					CmClipCopy() ;
	void					CmFermer() ;
  void          CmImprimer();
	bool					convertGdata(string *psFichier) ;

	bool          bNavOk ;
	bool          bCanClose ;
	string				sHtml ;
	string				sFileName ;			// path + nom + ext
	string				sPathName ;     // path
	string				sNomFichier ;		// nom + ext
	string				sExtension ;		// ext
	string				sNomSeul ;			// nom
	NSRefDocument	*pNewDoc ;			// Document � cr�er dans nautilus
	string				sTypeNautilus ;	// Type nautilus
	NSContexte		*pContexte ;

 protected:
	bool					IsTabKeyMessage(MSG& msg) ;
	bool					PreProcessMsg(MSG& msg) ;
	string				html2string(string s2convert) ;

 private:
 	TWebLog				*Form ;
  HACCEL        hAccelerator ;

 	DECLARE_RESPONSE_TABLE(NSLogWindow) ;
} ;

// Classe TMDIChild pour la red�finition de EvClose/////////////////////////////////////////////////////////
class NSLogChild : public TMDIChild, public NSRoot
{
	public :

		NSLogWindow* pClient ;

		NSLogChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSLogWindow* clientWnd = 0) ;
		~NSLogChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSLogChild) ;
};

//
// D�finition d'une classe NSImage contenant chaque image import�e//
class NSImage{	public:
    string fichier ;        // nom du fichier image source    string type ;           // type nautilus de l'image    string html ;           // fichier html associ�    string supphtml ;       // fichier html pour la suppression de l'image
    NSImage() ;    NSImage(string sFichier, string sType, string sFichHtml = "", string sFichSupp = "") ;    ~NSImage() ;
    NSImage(const NSImage& rv) ;    NSImage& operator=(const NSImage& src) ;    int      operator==(const NSImage& o) ;};
//// D�finition de NSImgArray (Array de NSImage(s))//
typedef vector<NSImage*> NSImgVector;typedef NSImgVector::iterator NSImgIter;typedef NTArray<NSImage>  NSImgArray;
/*
class NSImgArray : public NSImgVector    // TODO DELETE IF NO PROBLEM{	public :
		// Constructeurs		NSImgArray() : NSImgVector() {}		NSImgArray(NSImgArray& rv);
		// Destructeur		virtual ~NSImgArray();
        void vider();};      */
class AssistCaptureDialog ;
// Classe TWindow permettant l'importation en s�rie des images// R�alise dans tous les cas l'importation (et non le r�f�rencement)/////////////////////////////////////////////////////////////////////////////////
class NSImpImgWindow : public TWindow{	public:  	bool           bNavOk ;    bool           bCanClose ;    string			   sHtml ;    string			   sPathName ;          // r�pertoire d'importation    NSRefDocument* pNewDoc ;			// Document � cr�er dans nautilus		NSImgArray     aImageArray ;        // array des fichiers image � importer    NSImgArray     aImportArray ;       // array des images import�es    NSImgArray     aSuppArray ;         // array des images supprimees    NSContexte* 	 pContexte ;    AssistCaptureDialog* pAssist ;
    NSImpImgWindow(TWindow* parent, NSContexte* pCtx, AssistCaptureDialog* pAssistDlg = 0) ;    ~NSImpImgWindow() ;
		void EvClose() ;    bool CanClose() ;    void PerformCreate(int menuOrId) ;    void MakeVisible() ;    void SetupWindow() ;    void Navigate() ;    void NavigateComplete() ;    void NavigateErrorEvent(int iStatusCode, string sURL) ;    void DetruireHtmlTemp() ;    bool EncapsuleImage(string sFichier, string sType, int index, string& sFichHtml, string& sFichSupp) ;    bool InitImageArray() ;    bool GenerePannel() ;    void CmQuitter() ;    void CmImporterTout() ;
	protected:
		bool IsTabKeyMessage(MSG &msg) ;    bool PreProcessMsg(MSG &msg) ;
	private:		TImportImg* Form ;    HACCEL      hAccelerator ;
	DECLARE_RESPONSE_TABLE(NSImpImgWindow) ;
};
// Classe TMDIChild pour la red�finition de EvClose/////////////////////////////////////////////////////////
class NSImpImgChild : public TMDIChild, public NSRoot
{
	public :

		NSImpImgWindow* pClient ;

		NSImpImgChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSImpImgWindow* clientWnd = 0) ;
		~NSImpImgChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSImpImgChild) ;
};

#endif
// fin de nsbrowse.h///////////////////////////////////////////////////////////////

