// --------------------------------------------------------------------------// NSMVIEW.H		Document/Vues G�n�rales pour MUE
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------

#if !defined(__NSMVIEW_H)#define __NSMVIEW_H

#include <owl/docmanag.h>
#include <owl/docview.h>
#include <owl/mdichild.h>
#include <owl/splitter.h>
#include <owl/tabctrl.h>
#include <owl/groupbox.h>
#include <owl/tooltip.h>

class BBItem ;
class NSPaneSplitter ;
class NSGadgetWindow ;
class NSDialog  ;
class NSControle ;
class NSToolTip ;
class NSControleVector ;
class NSControlsArray ;
class NSPatPathoArray ;
class NSOneTabWindow ;
class NSWindowsArray ;
class Ccontrol ;
class Cdialogbox ;
class NSMDIChild ;

#include "nsbb\nsexport.h"
#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif
#include "nsldv\nsldvuti.h"
// #include "nsTPaneSplitter.h"

// #include "nsbb\nsbb_dlg.h"

#define MYWS_OK         0x00000001
#define MYWS_CANCEL     0x00000010
#define MYWS_HELP       0x00000100
#define MYWS_ICONS      0x00001111
#define MYWS_PREVIOUS   0x00010000
#define MYWS_NONE       0x00000000

class _NSBBCLASSE NSMUEView : public OWL::TWindowView, public NSRoot// class _NSBBCLASSE NSMUEView : public TView, public OWL::TWindow, public NSRoot{
	public:

		TDocument* pDoc ;
    uint32     uButtonsStyle ;
    NSDialog*  pDialog ;

    // Constructeur - Destructeur    NSMUEView(NSContexte* pCtx, TDocument* pDoc, TWindow *parent = 0, string sFunction = "", string sDoc = "", string sPreoccu = "") ;
    virtual ~NSMUEView() ;

    virtual void    concernChanged(string sConcern) ;

    // Red�finiton des fonctions virtuelles de TView    static LPCSTR   StaticName() { return "NSSOAPView" ; }
    const char      *GetViewName() { return StaticName() ; }
    TWindow         *GetWindow() ;
    bool            CanClose() ;           // de TEditFile
    bool            Create() ;
    // bool         CanClear();           // de TEditFile
    bool            SetDocTitle(const char far* docname, int index)
    {
    	//return OWL::TWindowView::SetDocTitle(docname, index) ;
      return OWL::TWindow::SetDocTitle(docname, index) ;
    }

    virtual void    setViewName() = 0 ;
    void						addConcernTitle() ;
    string          getViewName() { return _sViewName ; }

    // Overrides TWindows' in order to process our accelerators table
    bool            PreProcessMsg(MSG &msg) ;

    void            SetupWindow() ;
    void            InitBasicMetrics(Cdialogbox *pDialogBox) ;
    void            SetFont(TDC& dc, Cdialogbox *pDialogBox) ;
    void            EvDestroy() ;    void            SetWindowPosit() ;
    bool 	          VnIsWindow(HWND hWnd) ;

                        // Fonctions de r�ponse aux evts
    void            EvSize(uint sizeType, ClassLib::TSize &size) ;
    void            EvMove(ClassLib::TPoint& clientOrigin) ;
    void            EvWindowPosChanged(WINDOWPOS far& windowPos) ;
    void            EvPaint() ;
    void            EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void            gotMoved() ;
    void            EvSetFocus(HWND hWndLostFocus) ;

    bool            isControlProperty(TWindow *pCtrl, uint32 StyleInfo) ;

    void            setFocusToNextSplitterView() ;
    void            setFocusToPrevSplitterView() ;

    void            newWindow(string sWndChild) ;
    void            focusView() ;
    void            activate() ;

    string          getConcern()    { return _sPreoccup ; }
    string          getFunction()   { return _sFonction ; }

    void            setConcern(string sPr)      { _sPreoccup = sPr ; }
    void            setFunction(string sFonct)  { _sFonction = sFonct ; }

    void            setDocType(string sType)  { _sDocType = sType ; }
    string          getDocType()              { return _sDocType ; }

    void            setPercent(float fPercent)  { _percent = fPercent ; }
    float           getPercent()                { return _percent ; }

    void            setSplitDirection(TSplitDirection dir)    { _splitDir = dir ; }
    TSplitDirection getSplitDirection()             { return _splitDir ; }

    void            setSplittedWindow(TWindow *pWindow) { _pSplittedWindow = pWindow ; }
    TWindow*        getSplittedWindow();

    void            setCreateWindow(TWindow *pWindow) { _pCreateWindow = pWindow ; }
    TWindow*        getCreateWindow() ;

    void            setGadgetPanelWindow(NSGadgetWindow *pWindow) { _pGadgetPanelWindow = pWindow ; }
    NSGadgetWindow* getGadgetPanelWindow()  { return _pGadgetPanelWindow ; }

    void            setPaneSplitter (NSPaneSplitter* pPane) { _pPaneSplitter = pPane ; }
    NSPaneSplitter* getPaneSplitter() { return _pPaneSplitter ; }

    HACCEL          getAccelerator() { return _hAccelerator ; }

    void            setCreatedMdiChild(NSMDIChild* pMdiC) { _pCreatedMDIChild = pMdiC ; }
    NSMDIChild*     getCreatedMdiChild()                  { return _pCreatedMDIChild ; }

    void            initMUEViewMenu(string sMenuName) ;
    void            activateMUEViewMenu() ;

    void            activateParent() ;

    int             getWindowXFromDialogX(int iX) ;
    int             getWindowYFromDialogY(int iY) ;
    ClassLib::TRect getWindowRectFromDialogRect(int iX, int iY, int iW, int iH) ;

    virtual void    CmOk() = 0 ;
    virtual void    CmCancel() = 0 ;

    virtual void    CmAdd()      { return ; }
    virtual void    CmClose()    { return ; }
    virtual void    CmSuppress() { return ; }
    virtual void    CmChange()   { return ; }
    virtual void    CmContinue() { return ; }
    virtual void    CmPrevious() { return ; }
    virtual void    CmAll()      { return ; }
    virtual void    CmFct1()     { return ; }
    virtual void    CmFct2()     { return ; }
    virtual void    CmFct3()     { return ; }
    virtual void    CmFct4()     { return ; }
    virtual void    CmFct5()     { return ; }
    virtual void    CmFct6()     { return ; }
    virtual void    CmFct7()     { return ; }
    virtual void    CmFct8()     { return ; }
    virtual void    CmFct9()     { return ; }

	protected:
  	// Variables    string          _sFonction ;    string          _sPreoccup ;    string          _sDocType ;    string          _sViewName ;    bool            _bFirstSetup ;
    bool            _bSetupToolBar ;

    TWindow*        _pCreateWindow ;
    TWindow*        _pSplittedWindow ;
    TSplitDirection _splitDir ;
    float           _percent ;
    NSPaneSplitter* _pPaneSplitter ;
    NSGadgetWindow* _pGadgetPanelWindow ;

    TMenuDescr*     _pMUEViewMenu ;
    HACCEL          _hAccelerator ;

    UINT            _cxSysChar ;
    UINT            _cySysChar ;

    bool            _bClosing ;

    NSMDIChild*     _pCreatedMDIChild ;

	DECLARE_RESPONSE_TABLE(NSMUEView) ;};

typedef vector<NSMUEView*> MUEViewArray;
typedef MUEViewArray::iterator ArrayMUEViewIter;

//------------------------------------------

class _NSBBCLASSE ArrayMUEView : public MUEViewArray
{
	public:

		ArrayMUEView() : MUEViewArray() {}
    ~ArrayMUEView() ;
    void vider() ;
} ;

class _NSBBCLASSE NSLDVView : public NSMUEView
{
	public:

		NSLDVView(NSContexte* pCtx, TDocument* pDoc, TWindow *parent = 0, string sFunction = "", string sDoc = "", string sPreoccu = "") ;
    ~NSLDVView() ;

    virtual void reloadView(string sReason) = 0 ;
} ;

class _NSBBCLASSE NSBBMUEView : public NSMUEView
{
	public:

    BBItem *_pBBItem ;

		NSBBMUEView(NSContexte* pCtx, TDocument* pDoc, TWindow *parent = 0, string sFunction = "", string sDoc = "", string sPreoccu = "") ;
    ~NSBBMUEView() ;

    void SetupWindow() ;
    void InitMetrics() ;
    void CreerControlesArchetype() ;
    virtual TWindow* CreateControl(Ccontrol *pControl, OWL::TGroupBox *pGroupCurr, int &hSysButton, string sLang) ;
    void CmBbkCalcul() ;
    void CmBbkCalculFinalStep() ;
    void waitForKS() ;

    void EvKeyUp(uint key, uint repeatCount, uint flags) ;
    TResult EvSetBackColor(TParam1 param1, TParam2 param2) ;

    // Overrides TWindows' in order to have tooltip work
    bool    PreProcessMsg(MSG &msg) ;

    int  GetSelectedTab() { return _m_tabCtrl->GetSel() ; }
		void SetSelectedTab(int index) ;

    void SetFocusToNextControl(OWL::TControl* pTControle) ;
    void SetFocusToPreviousControl(OWL::TControl* pTControle) ;

    void SetFocusToNextControlInGroup(OWL::TControl* pTControle) ;
    void SetFocusToPreviousControlInGroup(OWL::TControl* pTControle) ;

    void              Attach(NSOneTabWindow *ptr, const char *title = 0) ;
		void              Attach(NSWindowsArray *windowArray) ;
		NSOneTabWindow*   Detach(int index) ;
		NSOneTabWindow*   Detach(NSOneTabWindow *ptr) ;
		NSOneTabWindow*   Retrieve(int index) ;
		int               Retrieve(NSOneTabWindow *ptr) ;
		NSOneTabWindow*   operator[](int index) ;
		int               operator[](NSOneTabWindow *win) ;

    int               getDialogXFromWindowX(int iX) ;
    int               getDialogYFromWindowY(int iY) ;
    ClassLib::TRect   getDialogRectFromWindowRect(int iX, int iY, int iW, int iH) ;

    NSControlsArray*  getActiveControls() ;
    NSControleVector* getControls()   { return _pNSCtrl ; }

    void              rafraichitControles() ;

    void setWaitingForSystem(bool bWFS) { _bWaitingForSystemAction = bWFS ; }
    void setWaitingForKS(bool bWFK)     { _bWaitingForKsAnswer     = bWFK ; }

    BBItem* getItem() { return _pBBItem ; }

  protected:

    void   Paint(TDC&, bool, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    HBRUSH EvCtlColor(HDC hdc, HWND hWndCtl, uint ctlType) ;

    void referenceControle(NSControle *pNSControle) ;
    void initialiseControles() ;
    void initControlesFromBbk(string filling = "SetupWindow") ;
    void initControlFromBbkAnswer(NSControle* pControl, NSPatPathoArray* pPatPathoArray, string* psDate = NULL) ;
    void initControlesFromCapture() ;

    void EvSize(uint sizeType, ClassLib::TSize& size) ;
    void TabSelChange(TNotify far&) ;
		bool TabSelChanging(TNotify far&) ;
    void TabKeyDown(TTabKeyDown far&) ;
    int  AdjustTabClient() ;

    NSControle* getNSControl(TWindow *pCtrl) ;
    void        SetupWindowForControl(TWindow *pCtrl) ;

    void executeClosingFunctions() ;
    void executePreClosingFunctions() ;
    void disconnectControls() ;

    bool CanSave() ;
    bool isInEditMode() ;

#ifdef __OB1__
    void EvTimer(uint id) ;
    void resetOB1PrecocheTimer() ;
    void checkPrecocheOnTimer() ;
    void checkResultOnTimer() ;
    void stillWaitingForKsTimer() ;
    void CmBbkEvent() ;
    void CmBbkUpdate() ;
#endif

    virtual void CmOkFinalStep() = 0 ;
    virtual void CmHelp() = 0 ;

    void         CmPrevious() ;

    NSWindowsArray   *_tabbedWindows ;

    NSControleVector *_pNSCtrl ;

    OWL::TTabControl *_m_tabCtrl ;
    bool             _bControlInit ;

    bool             _bWaitingForKsAnswer ;
    bool             _bWaitingForSystemAction ;
    bool             _bAcceptUpdate ;
    NVLdVTemps       _tpsBBKStart ;
    bool             _bNeedToRefresh ;  // a IDM_BBK_UPDATE event was received at a moment when it could not been processed

    NSToolTip       *_pNSToolTip ;
    TTooltip        *_pToolTip ;

  DECLARE_RESPONSE_TABLE(NSBBMUEView) ;
} ;

//--------------------------------------------


class ArraySplitWindow ;
//
// Structure de propri�t� de fen�tres suivant leur fonction// Structure of windows property depending on their function//class _NSBBCLASSE NSWindowProperty{	public :  	enum FCTACTIVITY { undefined = 0, notAvailable, available, openedWithPatient, blackboardedWithPatient };    enum DYNAMICPOS  { undefpos = 0, automatic, left, right, top, bottom };    NSWindowProperty() ;    NSWindowProperty(const NSWindowProperty& rv) ;
    ~NSWindowProperty() ;    void resetSplitList() ;    void addSplit(string sTargetFct, string sSplitFct, TSplitDirection dir, float percent) ;    string getFunction()            { return _sFunction ; }    void   setFunction(string sFct) { _sFunction = sFct ; }    int    X(NSSuper* pSuper) ;    int    Y(NSSuper* pSuper) ;    int    W(NSSuper* pSuper) ;    int    H(NSSuper* pSuper) ;    NS_CLASSLIB::TRect getRect(NSSuper* pSuper) ;    static int  getGlobalH(NSSuper* pSuper) ;    static int  getGlobalW(NSSuper* pSuper) ;    static int  getAvailableHForRect(NSSuper* pSuper, NS_CLASSLIB::TRect rect) ;    static int  getAvailableWForRect(NSSuper* pSuper, NS_CLASSLIB::TRect rect) ;    static bool getMdiClientRect(NSSuper* pSuper, NS_CLASSLIB::TRect& rect) ;    int    getX()      { return _iX ; }    void   setX(int x) { _iX = x ; }    int    getY()      { return _iY ; }    void   setY(int y) { _iY = y ; }    int    getW()      { return _iW ; }    void   setW(int w) { _iW = w ; }    int    getH()      { return _iH ; }    void   setH(int h) { _iH = h ; }    DYNAMICPOS posX()                { return _posX ; }    void       setPosX(DYNAMICPOS x) { _posX = x ; }    DYNAMICPOS posY()                { return _posY ; }    void       setPosY(DYNAMICPOS y) { _posY = y ; }    DYNAMICPOS posW()                { return _posW ; }    void       setPosW(DYNAMICPOS w) { _posW = w ; }    DYNAMICPOS posH()                { return _posH ; }    void       setPosH(DYNAMICPOS h) { _posH = h ; }    void   initFromRect(NS_CLASSLIB::TRect rect) ;    ArraySplitWindow* getSplit()                         { return _paramSplit ; }    void              setSplit(ArraySplitWindow* pSplit) { _paramSplit = pSplit ; }    FCTACTIVITY getActivity()            { return _iActivity ; }    void   setActivity(FCTACTIVITY iAct) { _iActivity = iAct ; }    char   getHotKey()         { return _cHotKey ; }    void   setHotKey(char cHK) { _cHotKey = cHK ; }    string getModifiers()            { return _sModifiers ; }    void   setModifiers(string sMod) { _sModifiers = sMod ; }    UINT   getModifiersAsInt() ;    bool   isInModifier(UINT iModConst) ;    void   resetModifiers()          { _sModifiers = string("") ; }    void   addToModifiers(UINT iModConst) ;    string getPosAsString() ;    static string     DynPosToString(DYNAMICPOS pos) ;    static DYNAMICPOS StringToDynPos(string sPos) ;    NSWindowProperty& operator=(const NSWindowProperty& src) ;	protected:  	//    // Function    //    string _sFunction ;    //    // Position    //    int    _iX ;    int    _iY ;    int    _iW ;    int    _iH ;    DYNAMICPOS _posX ;    DYNAMICPOS _posY ;    DYNAMICPOS _posW ;    DYNAMICPOS _posH ;    ArraySplitWindow* _paramSplit ;    FCTACTIVITY _iActivity ;    char        _cHotKey ;    string      _sModifiers ;};class NSSplittingWindowProperty ;// d�finition d'un array repr�sentant la fen�tre de capturetypedef vector<NSWindowProperty*> ArrayWP ;
typedef ArrayWP::iterator       ArrayWPIter ;
typedef ArrayWP::const_iterator ArrayWPConstIter ;

class _NSBBCLASSE ArrayWinProp : public ArrayWP, public NSRoot
{
	public:

		// Constructeurs - Destructeur
    ArrayWinProp(NSContexte* pCtx) : ArrayWP(), NSRoot(pCtx) {}
    ArrayWinProp(const ArrayWinProp& rv) ;
    ~ArrayWinProp() ;

    void                vider() ;
    void                initialiser(string sUserId, string sPersoDirectory) ;
    void                initWindsChild(string sUserId, string sPersoDirectory) ;

    NSWindowProperty*          getProperty(string sFunction) ;
    NSSplittingWindowProperty* searchProperty(string sMother, string sChild) ;
    NS_CLASSLIB::TRect         getPropertyRect(string sFunction, NSSuper* pSuper) ;

    void                setProperty(string sUserId, string sFunction, NS_CLASSLIB::TRect, string sPersoDirectory) ;

    void                saveWindowProperty(string sUserId, string sPersoDirectory, NSWindowProperty *wndProp,  bool bWriteFile) ;

    ArrayWinProp&       operator=(const ArrayWinProp& src) ;

	protected:

  	bool                initFromFile(string sFileName) ;
    bool                getStringFromFile(string sFileName, string* pData) ;
    bool                initFromString(string sFileName, string* pData) ;
    bool                readBlockInString(string* psSource, size_t* piCursor, string* psResult) ;
    bool                gotoNextBlock(string* psSource, size_t* piCursor) ;
    bool                initSizeFromString(string* psSource, NSWindowProperty* pWinProp) ;
    bool                initSplitFromString(string* psSource, NSWindowProperty* pWinProp) ;
    bool                initDetailsFromString(string* psSource, NSWindowProperty* pWinProp) ;
};

//------------------------------------------------------------------------------
// retient les elements qui vient du
//      [MotherWindow, ChildWindow, direction, percent]
//------------------------------------------------------------------------------
class _NSBBCLASSE NSSplittingWindowProperty
{	public :		NSSplittingWindowProperty() ;    NSSplittingWindowProperty(const NSSplittingWindowProperty& rv) ;    ~NSSplittingWindowProperty() ;

    /*void        setMotherWindow(NSMUEView *pWind)       {pMotherWind = pWind ; }
      NSMUEView*  getMotherWindow()                        { return pMotherWind ; }

      void        setChildWindow(NSMUEView *pWind)        {pChildWind = pWind ; }
      NSMUEView*  getChildWindow()                        { return pChildWind ; }  */

		void        setMotherWindow(string sFunction)       {sMotherWind = sFunction ; }
    string      getMotherWindow()                        { return sMotherWind ; }

    void        setChildWindow(string sFunction)        {sChildWind = sFunction ; }
    string      getChildWindow()                        { return sChildWind ; }

    void        setPercent(float fPercent)              {percent = fPercent;}
    float       getPercent()                            {return percent;}

    void            setSplitDirection(TSplitDirection dir)  {dirSplitting = dir;}
    TSplitDirection getSplitDirection()             {return dirSplitting;}

    NSSplittingWindowProperty& operator=(const NSSplittingWindowProperty& src) ;

	private:		string          sMotherWind ;    string          sChildWind ;    float           percent ;    TSplitDirection dirSplitting ;};
typedef vector <NSSplittingWindowProperty*> ArraySplitW ;
typedef ArraySplitW::iterator       ArraySWIter ;
typedef ArraySplitW::const_iterator ArraySWConstIter ;

class _NSBBCLASSE ArraySplitWindow : public ArraySplitW
{
	public:

		// Constructeurs - Destructeur
    ArraySplitWindow() : ArraySplitW() {}
    ArraySplitWindow(const ArraySplitWindow& rv) ;
    ~ArraySplitWindow() ;
    void          vider() ;

    ArraySplitWindow& operator=(const ArraySplitWindow& src) ;
} ;

//-------------------------------------

#endif		// __NSMVIEW_H

