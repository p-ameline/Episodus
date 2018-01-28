// NsGrabObjects.h : Définition des objets de capture d'images//////////////////////////////////////////////////////////////
#if !defined(__NSGRABOBJECTS_H)#define __NSGRABOBJECTS_H
#include <owl\edit.h>#include <owl\controlg.h>#include <owl\docmanag.h>#include <owl\mdichild.h>#include <owl\checkbox.h>#include <owl\groupbox.h>

// #include "nautilus\nssuper.h"#include "partage\ns_vector.h"
#include "partage\NTArray.h"
#include "nsbb\nsutidlg.h"
#include "nssavoir\nscaptur.h"

// defines pour NSGrabObject
#define M_CAPTUREIMAGE      1
#define M_CONVERTBMP        2
#define M_FREECAMERA        3
#define M_GRABFRAME         4
#define M_GRABFREE          5
#define M_GRABINIT          6
#define M_GRABONEBUFFER     7
#define M_INITCAMERA        8
#define M_NBCAMERA          9
#define M_PANIMAGE          10
#define M_STOPCAMERA        11
#define M_ZOOMIMAGE         12

#define T_CAPTUREIMAGE      13
#define T_SELECTSOURCE      14
/////////////////////////////////////////////

// id de commandes pour fenetre de capture (grabbing)#define IDC_PLAY          110#define IDC_STOP          111#define IDC_CAPTURE		    112#define IDC_PAN           113#define IDC_ZOOM          114#define IDC_GRAB          115#define IDC_SELECTSOURCE  116class NSContexte ;class AssistCaptureDialog ;
// Classe TWindow pour la capture Twain///////////////////////////////////////////////////////

class NSGrabberObject : public NSRoot
{
	public:

    NSGrabberObject(NSContexte *pCtx) ;
    ~NSGrabberObject() ;

    virtual bool CaptureImage(string sFichier, HWND hwnd) = 0 ;    bool ConvertImage(string sFichier) ;
    bool FindFileName(string sNameSeed, string* pBmpFileName, string* pJpgFileName) ;
};
// Classe TWindow pour la capture (Grabbing) des images///////////////////////////////////////////////////////

class NSMilGrabObject : public NSGrabberObject
{
	public:

    long lSizeX ;
    long lSizeY ;

    NSMilGrabObject(NSContexte *pCtx) ;
    ~NSMilGrabObject() ;

    bool InitCamera(HWND handleClient) ;
    bool NbCamera(int& nbCam) ;
    bool FreeCamera() ;
    bool StopCamera() ;
    bool GrabFree() ;
    bool GrabFrame() ;
    bool GrabOneBuffer() ;    bool CaptureImage(string sFichier, HWND hwnd) ;    bool PanImage(long X, long Y) ;    bool ZoomImage(long X, long Y) ;
};

class NSMilGrabWindow : public TWindow
{	public:
		long                 lSizeX ;
    long                 lSizeY ;
    bool                 bCanClose ;    bool                 bOpenMenu ;    NSContexte* 			   pContexte ;    AssistCaptureDialog* pAssist ;    NSMilGrabObject*     pGrabber ;    bool                 bGrabInProgress ;
    NSMilGrabWindow(TWindow* parent, NSContexte* pCtx, AssistCaptureDialog* pAssistDlg = 0);
    ~NSMilGrabWindow();
    void SetupWindow() ;    void EvRButtonUp(uint, NS_CLASSLIB::TPoint&) ;    void EvLButtonUp(uint, NS_CLASSLIB::TPoint&) ;    void EvSize(uint, NS_CLASSLIB::TSize&) ;    int  EvCreate(CREATESTRUCT far& createStruct) ;    bool CanClose() ;    void PlayCamera() ;    void GrabContinuous() ;    void StopCamera() ;    void CaptureImage() ;    void PanImage() ;    void ZoomImage() ;    void SetWindowPosit() ;    // Overrides TWindows' in order to process our accelerators table    bool PreProcessMsg(MSG &msg) ;  protected:    HACCEL hAccelerator ;
	DECLARE_RESPONSE_TABLE(NSMilGrabWindow) ;};
// Classe TMDIChild pour la redéfinition de EvClose
/////////////////////////////////////////////////////////class NSMilGrabChild : public TMDIChild, public NSRoot{	public :
		NSMilGrabWindow* pClient ;
		NSMilGrabChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSMilGrabWindow* clientWnd = 0) ;		~NSMilGrabChild() ;
		void EvClose() ;
	DECLARE_RESPONSE_TABLE(NSMilGrabChild) ;};

// Dialogue assistant de capture d'images
/////////////////////////////////////////////////
class AssistCaptureDialog : public NSUtilDialog
{	public:
		int phase ;

    OWL::TCheckBox* pPhase1 ;
    OWL::TCheckBox* pPhase2 ;
    OWL::TCheckBox* pPhase3 ;
    NSContexte* pContexte ;

		AssistCaptureDialog(TWindow* pere, NSContexte* pCtx) ;		~AssistCaptureDialog() ;
		void SetupWindow() ;
		void CmSuite() ;		void CmAnnuler() ;    void CheckPhase() ;

	DECLARE_RESPONSE_TABLE(AssistCaptureDialog) ;};

// Classe TWindow pour la capture Twain
///////////////////////////////////////////////////////

class NSTwainObject : public NSGrabberObject
{
	public:

		string sBmpFileName ;
    string sJpgFileName ;

    NSTwainObject(NSContexte *pCtx) ;
    ~NSTwainObject() ;

    bool CaptureImage(string sFichier, HWND hwnd) ;    int  SelectImageSource(HWND hwnd) ;};
class NSTwainWindow : public TWindow
{	public:
		long           lSizeX ;
    long           lSizeY ;
    bool           bOpenMenu ;    NSContexte*    pContexte ;    NSTwainObject* pGrabber ;    TDib*          pDib ;        NSTwainWindow(TWindow* parent, NSContexte* pCtx) ;
    ~NSTwainWindow() ;
    void SetupWindow() ;    void EvRButtonUp(uint, NS_CLASSLIB::TPoint&) ;    void EvSize(uint, NS_CLASSLIB::TSize&) ;    int  EvCreate(CREATESTRUCT far& createStruct) ;    void CaptureImage() ;    void SelectSource() ;    void SetWindowPosit() ;
	DECLARE_RESPONSE_TABLE(NSTwainWindow) ;};
// Classe TMDIChild pour la redéfinition de EvClose
/////////////////////////////////////////////////////////class NSTwainGrabChild : public TMDIChild, public NSRoot{	public :
		NSTwainWindow* pClient ;
		NSTwainGrabChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSTwainWindow* clientWnd = 0) ;		~NSTwainGrabChild() ;
		void EvClose() ;
	DECLARE_RESPONSE_TABLE(NSTwainGrabChild) ;};

#endif   // __NSGRABOBJECTS_H
// fin de nsbrowse.h///////////////////////////////////////////////////////////////

