//----------------------------------------------------------------------------
//   Programme protégé par une licence Open Source GPL
//   NAUTILUS - www.nautilus-info.com
//----------------------------------------------------------------------------
#ifndef __NSLDVVUETECH_H
# define __NSLDVVUETECH_H

// # include <owl\owlpch.h>
# include <owl\dc.h>
# include <owl\inputdia.h>
# include <owl\chooseco.h>
# include <owl\gdiobjec.h>
# include <owl\docmanag.h>
# include <owl\filedoc.h>
# include <owl\listbox.h>
# include <owl\draglist.h>
# include <owl\imagelst.h>
# include <owl\scrollba.h>
# include <owl\glyphbtn.h>
# include <classlib\arrays.h>

# include "nsbb\nstip.h"
# include "nsldv\nsldvdoc_base.h"
# include "nsldv\nsldvuti.h"
# include "nsldv\nsldvvar.h"

# include "nsldv\nsldvgoal.h"
# include "partage\ns_timer.h"

# include "nsldv\nsldvtoons.h"

# include "nsldv/nsldvexport.h"

# define FOCUS_AUTO   "AUTO"
# define FOCUS_SKIN   "SKIN"

# define VIEW_TOP     "TOP"
# define VIEW_BOTTOM  "BOTTOM"
# define VIEW_LEFT    "LEFT"
# define VIEW_RIGHT   "RIGHT"
# define VIEW_HMIDDLE "HMIDDLE"
# define VIEW_VMIDDLE "VMIDDLE"

# define REF_PARENT   "PARENT"
# define REF_WINDOW   "WINDOW"
# define REF_CLIENT   "CLIENT"
# define REF_APPLIC   "APPLICATION"
# define REF_SCREEN   "SCREEN"

class NSLdvVerticalScrollBar ;
class NSLdvViewAreaWindow ;
class NSLdvViewBase ;

class _NSLDVCLASSE NSLdvWindowToon : public NSLdvToon
{
	public :

    // enum TOONSHAPE { toonFullWidth = 0, toonFullHeight, toonSizeLimited };
    // TOONSHAPE	         toonShape ;

    // NS_CLASSLIB::TRect wBox ;          // Boite temporelle réelle
    // NS_CLASSLIB::TRect wVisibleBox ;   // Boite temporelle visible (exemple icone)

    string _sSkinName ;

    NSLdvWindowToon(NSContexte* pCtx, NSLdvWindowToon* parent, NSLdvViewBase* pTheView) ;
    NSLdvWindowToon(const NSLdvWindowToon& rv) ;
    virtual ~NSLdvWindowToon() { lLocalObjectCount-- ; }

    void initialiser(const NSLdvWindowToon* pSrc) ;
    void registerAsToon() ;

    void resetPos() ;
    bool isPosValid() ;

    void initDimensions() ;
    bool initBoxFromSkin() ;

    void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    void draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;
    void skinDraw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;
    void skinDrawForm(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;
    void skinDrawTile(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

    virtual bool MoveWindow(bool repaint = false) ;
    bool MoveWindow(const NS_CLASSLIB::TRect& rect, bool repaint = false) ;
    void EvaluateWinToonsPos() ;

    virtual void setScrollParams() = 0 ;

    bool Touches(NS_CLASSLIB::TRect rect) ;
    bool Contains(const NS_CLASSLIB::TPoint& point) ;

    // NS_CLASSLIB::TRect donneRectangle()        { return wBox ; }
    // NS_CLASSLIB::TRect donneVisibleRectangle() { return wVisibleBox ; }
    NS_CLASSLIB::TRect donneRectangle() ;
    NS_CLASSLIB::TRect donneVisibleRectangle() ;

    int                getYValueFromChildYValue(int iLdVYValue, TWindow* pChild) ;

    int                getWindowYValueFromLdVYValue(int iLdVYValue) ;
    NS_CLASSLIB::TRect getScreenRectFromLdvRect(NVLdVRect* pLdvRect) ;
    NS_CLASSLIB::TRect getScreenRect(NS_CLASSLIB::TRect *pFixedRect) ;

    NS_CLASSLIB::TPoint getPointForDraw(NS_CLASSLIB::TPoint *pLdvPoint) ;
    NS_CLASSLIB::TRect  getRectForDraw(NS_CLASSLIB::TRect *pLdvRect) ;

    NS_CLASSLIB::TPoint getPointForDraw(NVLdVPoint ldvPoint) ;
    NS_CLASSLIB::TRect  getRectForDraw(NVLdVRect ldvRect) ;

    NS_CLASSLIB::TPoint getRelativePoint(NS_CLASSLIB::TPoint ldvPoint) ;
    NS_CLASSLIB::TRect  getRelativeRect(NS_CLASSLIB::TRect ldvRect) ;

    NSLdvWindowToon& 	 operator=(const NSLdvWindowToon& src) ;

    //
    // Actions
    //
    void statusChanged(bool ) ;
    void levelChanged(int iNewLevel) ;

    //
    // GETs
    //

    TWindow*           getInterface()  { return _pInterface ; }
    TWindow*           getParentInterface() ;
    NS_CLASSLIB::TRect GetClientRect() { return _pInterface->GetClientRect() ; }

    ArrayOfToons*      getToonsArray() { return &_aToons ; }

    NSLdvWindowToon*   getToonParent() { return _pParent ; }

    // NSLdvVerticalScrollBar* getVerticalScroll() ;

    bool isLeftSet()   { return _bLeftSet ;   }
    bool isRightSet()  { return _bRightSet ;  }
    bool isTopSet()    { return _bTopSet ;    }
    bool isBottomSet() { return _bBottomSet ; }
    bool isWidthSet()  { return _bWidthSet ;  }
    bool isHeightSet() { return _bHeightSet ; }

    int  getLeftValue()   { return _iLeftValue ;   }
    int  getRightValue()  { return _iRightValue ;  }
    int  getTopValue()    { return _iTopValue ;    }
    int  getBottomValue() { return _iBottomValue ; }
    int  getWidthValue()  { return _iWidthValue ;  }
    int  getHeightValue() { return _iHeightValue ; }

    //
    // SETs
    //

    void setLeftSet(bool bVal)    { _bLeftSet   = bVal ; }
    void setRightSet(bool bVal)   { _bRightSet  = bVal ; }
    void setTopSet(bool bVal)     { _bTopSet    = bVal ; }
    void setBottomSet(bool bVal)  { _bBottomSet = bVal ; }
    void setWidthSet(bool bVal)   { _bWidthSet  = bVal ; }
    void setHeightSet(bool bVal)  { _bHeightSet = bVal ; }

    void setLeftValue(int iVal)   { _iLeftValue   = iVal ; }
    void setRightValue(int iVal)  { _iRightValue  = iVal ; }
    void setTopValue(int iVal)    { _iTopValue    = iVal ; }
    void setBottomValue(int iVal) { _iBottomValue = iVal ; }
    void setWidthValue(int iVal)  { _iWidthValue  = iVal ; }
    void setHeightValue(int iVal) { _iHeightValue = iVal ; }

    void calculateWidthValue() ;
    void calculateHeightValue() ;
    void calculateLeftValue() ;
    void calculateRightValue() ;
    void calculateTopValue() ;
    void calculateBottomValue() ;

    static long getNbInstance()  { return lLocalObjectCount ; }
    static void initNbInstance() { lLocalObjectCount = 0 ; }

  protected:

    //
    // Les objets à afficher sont là
    // Here are the objects to draw
    //
    ArrayOfToons _aToons ;

    TWindow* _pInterface ;

    NSLdvWindowToon* _pParent ;

    bool _bLeftSet ;
    int  _iLeftValue ;
    bool _bRightSet ;
    int  _iRightValue ;
    bool _bTopSet ;
    int  _iTopValue ;
    bool _bBottomSet ;
    int  _iBottomValue ;

    bool _bWidthSet ;
    bool _bFixedWidth ;
    int  _iWidthValue ;
    bool _bHeightSet ;
    bool _bFixedHeight ;
    int  _iHeightValue ;

    static long lLocalObjectCount ;
} ;

typedef vector<NSLdvWindowToon*> WinToonsVector ;
typedef WinToonsVector::iterator       WinToonsIterator ;
typedef WinToonsVector::const_iterator WinToonsConstIterator ;

//
// ------------------ Command Pannel ------------------
//

class NSCurvesProperty ;

class _NSLDVCLASSE NSLdvCommandPannel : public NSLdvWindowToon
{
  protected:

    static long lObjectCount ;

  public:

    NSLdvCommandPannel(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvViewBase* pTheView) ;
    NSLdvCommandPannel(const NSLdvCommandPannel& rv) ;
    ~NSLdvCommandPannel() ;

    void SetupWindow() ;

    // void draw(TDC* pDc, NVLdVRect* pRectARepeindre) ;
    // void draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

    void EvRButtonDown() ;

    void dysplayStatusMessage(string* pMessage) ;
    void dysplayPatientInformation() ;

    void drawText(TDC* pDc, string sText, string sSkinName, NS_CLASSLIB::TRect* pRect) ;

    NSLdvCommandPannel& operator=(const NSLdvCommandPannel& src) ;

    void setScrollParams() ;
    int  getSeverityThreshold() ;

    NSCurvesProperty *_pCurvesProperty ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class _NSLDVCLASSE NSLdvCommandPannelInterface : public TWindow, public NSRoot
{
  public:

    NSLdvCommandPannelInterface(NSContexte* pCtx, TWindow* parent, NSLdvCommandPannel* pToon) ;
    NSLdvCommandPannelInterface(const NSLdvCommandPannelInterface& rv) ;
    ~NSLdvCommandPannelInterface() ;

    void SetupWindow() ;

    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    NSLdvCommandPannelInterface& operator=(const NSLdvCommandPannelInterface& src) ;

    NSLdvCommandPannel* getToon() { return _pToon ; }

    int getSeverityThreshold() ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSLdvCommandPannel* _pToon ;
    OWL::TVSlider*      _pSeverityThresholdSlider ;
    OWL::TGlyphButton*  _pDrugsDisplayOnOff ;
    OWL::TGlyphButton*  _pCompressedMode ;

    void UpdateThresholdSlider(WPARAM code) ;

    static long lObjectCount ;

  DECLARE_RESPONSE_TABLE(NSLdvCommandPannelInterface) ;
};

//
// ------------------ ChronoLine ------------------
//

class NSTimeZoomLevel ;

class _NSLDVCLASSE NSLdvChronoLine : public NSLdvWindowToon
{
  public:

    NSLdvChronoLine(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvViewBase* pTheView) ;
    NSLdvChronoLine(const NSLdvChronoLine& rv) ;
    ~NSLdvChronoLine() ;

    void SetupWindow() ;

    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

    void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    void draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

    void EvRButtonDown() ;

    void drawChronoStep(TDC *pDc, NVLdVTemps tGauche, NVLdVTemps tDroite, string sTexte, bool bRed) ;

    void drawChronoStepMinutes(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoStepHours(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoStepDays(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoStepMonths(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoStepYears(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoStepDecads(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;

    void drawChronoContext(TDC *pDc, NVLdVTemps tGauche, NVLdVTemps tDroite, string sTexte) ;

    void drawChronoContextHours(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoContextDays(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoContextMonths(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoContextYears(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;
    void drawChronoContextDecads(TDC *pDc, NS_CLASSLIB::TRect* pRectARepeindre, NVLdVTemps tDateHeureInfGauche) ;

    void refresh() ;

    int  getLineHeight() ;

    void setScrollParams() ;

    NSLdvChronoLine& operator=(const NSLdvChronoLine& src) ;

    void initPpuRateInterface(NSTimeZoomLevel* pTimeInfo) ;
    int  getPpuRatePosition() ;

    void unitChanging() ;
    void unitChanged() ;
    bool isUnitChanging() { return _bUnitIsChanging ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

 protected:

    bool _bUnitIsChanging ;

    static long lObjectCount ;
};

class _NSLDVCLASSE NSLdvChronoLineInterface : public TWindow, public NSRoot
{
  public:

    NSLdvChronoLineInterface(NSContexte* pCtx, TWindow* parent, NSLdvChronoLine* pToon) ;
    NSLdvChronoLineInterface(const NSLdvChronoLineInterface& rv) ;
    ~NSLdvChronoLineInterface() ;

    void SetupWindow() ;

    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    NSLdvChronoLineInterface& operator=(const NSLdvChronoLineInterface& src) ;

    void CmLdvPixSecond() ;
    void CmLdvPixMinute() ;
    void CmLdvPixHour() ;
    void CmLdvPixDay() ;
    void CmLdvPixWeek() ;
    void CmLdvPixMonth() ;

    NSLdvChronoLine* getToon() { return _pToon ; }

    int  getPpuRatePosition() ;
    THSlider* getSlider() { return _pSlider ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

 protected:

    NSLdvChronoLine* _pToon ;
    THSlider*        _pSlider ;

    void UpdateTimeSlider(WPARAM code) ;

    static long lObjectCount ;

 DECLARE_RESPONSE_TABLE(NSLdvChronoLineInterface) ;
};

//
// ------------------ Vertical Scrollbar ------------------
//

/*

class NSLdvVerticalScrollBar : public NSLdvWindowToon
{
 public:

	NSLdvVerticalScrollBar(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pTheView) ;

  void SetupWindow() ;
  void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

  void draw(TDC* pDc, NVLdVRect* pRectARepeindre) ;
	void draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

  void EvRButtonDown() ;

  void setVisibleBox() ;

	void FixeRange(int max, int taille) ;
  void PositionneCurseur() ;

  NSLdvWindowToon* getToon() { return _pToon ; }

 protected:

  NSLdvWindowToon* _pToon ;
};

class NSLdvVerticalScrollBarInterface : public OWL::TScrollBar, public NSRoot
{
 public:

  NSLdvVerticalScrollBarInterface(NSContexte* pCtx, TWindow* parent, NSLdvVerticalScrollBar* pToon) ;
	NSLdvVerticalScrollBarInterface(NSLdvVerticalScrollBarInterface& rv) ;
	~NSLdvVerticalScrollBarInterface() ;

  void SetupWindow() ;

  void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
  void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void EvPaint() ;

  void SBLineDown() ;
  void SBLineUp() ;
  void SBPageDown() ;
  void SBPageUp() ;

	NSLdvVerticalScrollBarInterface& operator=(NSLdvVerticalScrollBarInterface& src) ;

  NSLdvVerticalScrollBar* getToon() { return _pToon ; }

  bool PreProcessMsg(MSG& msg) ;

  protected:

  NSLdvVerticalScrollBar* _pToon ;

  DECLARE_RESPONSE_TABLE(NSLdvVerticalScrollBarInterface) ;
};

*/

//
// ------------------ Y axis for curves ------------------
//

class _NSLDVCLASSE NSLdvCurveYAxis : public NSLdvWindowToon
{
  public:

    double _dLowActualValue ;
    double _dHigActualValue ;

    double _dLowAxisValue ;
    double _dHighAxisValue ;

    bool   _bIncludeZero ;

    NSLdvCurveYAxis(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvViewBase* pTheView, NSLdvCurve* pTheCurve) ;
    NSLdvCurveYAxis(const NSLdvCurveYAxis& rv) ;
    ~NSLdvCurveYAxis() ;

    void   SetupWindow() ;

    void   Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

    void   draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    void   draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

    bool   MoveWindow(bool repaint = false) ;

    void   setVisible() ;
    void   setHidden() ;

    void   EvRButtonDown() ;

    void   drawAxis(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

    void   initAxisValues(NSLdvCurvePoint* pFirstPoint) ;
    void   adjustAxisMinMaxValues(double& axisMinVal, double& axisMaxVal) ;
    int    giveYCoordinateForValue(double dValue, bool bLimitedToAxisLimits = false) ;
    int    giveWorkingAreaYCoordinateForValue(double dValue, bool bLimitedToAxisLimits = false) ;

    void   emptyTicks() ;
    void   computeTicks(double ticMinVal, double ticMaxVal, int maxTicks) ;
    double quantaRoundUp(double val) ;
    int    numFracDigits(double num) ;
    bool   getCommonValue(double val1, double val2, double& common, int& iCommon10log) ;
    int    getPrecision(double dVal) ;
    double roundUp(double dVal, int iPrecision) ;
    double roundDown(double dVal, int iPrecision) ;
    double trunc(double dVal, int iPrecision) ;

    void   setScrollParams() ;

    NSLdvCurve* getCurve() { return pCurve ; }

    NSLdvCurveYAxis& operator=(const NSLdvCurveYAxis& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

 protected:

    NSLdvCurve* pCurve ;

    vector<string> aTicks ;

    static long lObjectCount ;
};

class _NSLDVCLASSE NSLdvCurveYAxisInterface : public TWindow, public NSRoot
{
  public:

    NSLdvCurveYAxisInterface(NSContexte* pCtx, TWindow* parent, NSLdvCurveYAxis* pToon) ;
    NSLdvCurveYAxisInterface(const NSLdvCurveYAxisInterface& rv) ;
    ~NSLdvCurveYAxisInterface() ;

    void SetupWindow() ;

    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    NSLdvCurveYAxisInterface& operator=(const NSLdvCurveYAxisInterface& src) ;

    NSLdvCurveYAxis* getToon() { return _pToon ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSLdvCurveYAxis* _pToon ;

    static long lObjectCount ;
};

class _NSLDVCLASSE NSLdvCurveBackgroundQuadri
{
  public:

    NVLdVTemps _tpsLowerLeft ;
    double     _dValueLowerLeft ;
    string     _sUnitSensLowerLeft ;

    NVLdVTemps _tpsLowerRight ;
    double     _dValueLowerRight ;
    string     _sUnitSensLowerRight ;

    NVLdVTemps _tpsUpperLeft ;
    double     _dValueUpperLeft ;
    string     _sUnitSensUpperLeft ;

    NVLdVTemps _tpsUpperRight ;
    double     _dValueUpperRight ;
    string     _sUnitSensUpperRight ;

    NS_CLASSLIB::TColor color ;

    NSLdvCurveBackgroundQuadri(NSLdvViewBase* pVw) ;
    // Ctor for rectangular quadri
    NSLdvCurveBackgroundQuadri(NSLdvViewBase* pVw, NS_CLASSLIB::TColor qColor, NVLdVTemps beginDate, NVLdVTemps endDate, double lowerValue, string lowerUnit, double upperValue, string upperUnit) ;
    NSLdvCurveBackgroundQuadri(const NSLdvCurveBackgroundQuadri& rv) ;
    ~NSLdvCurveBackgroundQuadri() ;

    NSLdvCurveBackgroundQuadri& operator=(const NSLdvCurveBackgroundQuadri& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

	  NSLdvViewBase* _pView ;

    static long lObjectCount ;
} ;

typedef vector<NSLdvCurveBackgroundQuadri*> NSLdvCurveBackgroundQuadriArray ;
typedef NSLdvCurveBackgroundQuadriArray::iterator       NSLdvQuadriIter ;
typedef NSLdvCurveBackgroundQuadriArray::const_iterator NSLdvQuadriConstIter ;
typedef NTArray<NSLdvCurveBackgroundQuadri> NSLdvCurveBackgroundQuadrisArray ;

class _NSLDVCLASSE NSLdvCurveBackground : public NSLdvTimeToon
{
  public:

    bool   _bExistMaxLimit ;
    double _dMaxLimit ;
    string _sMaxLimitUnit ;

    bool   _bExistMinLimit ;
    double _dMinLimit ;
    string _sMinLimitUnit ;

    NSLdvCurveBackground(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvCurve* pTheCurve) ;
    NSLdvCurveBackground(const NSLdvCurveBackground& rv) ;
    ~NSLdvCurveBackground() ;

    void   SetupWindow() ;
    void   draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    void   draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

    void   EvRButtonDown() ;

    void    drawQuadris(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;
    TRegion getQuadriAsARegion(NSLdvCurveBackgroundQuadri* pQuadri, bool* pSuccess) ;
    void    drawAQuadri(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre, NSLdvCurveBackgroundQuadri* pQuadri) ;

    void    eraseQuadri() ;
    void    addQuadri(NSLdvCurveBackgroundQuadri* pQuadri) ;

    NSLdvCurveBackground& operator=(const NSLdvCurveBackground& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSLdvCurve* _pCurve ;

    NSLdvCurveBackgroundQuadrisArray _aQuadris ;

    static long lObjectCount ;
};

class NSLdvCurvesManagementPannel ;
class NSCurvesPropertyWindow ;

class _NSLDVCLASSE NSCurvesProperty : public NSLdvWindowToon
{
  protected:

    static long lObjectCount ;

  public:

    NSCurvesProperty(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvViewBase* pTheView) ;
    NSCurvesProperty(const NSCurvesProperty& rv) ;
    ~NSCurvesProperty() ;

    void SetupWindow() ;

    void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    void draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

    void EvRButtonDown() ;

    void CmNew() ;
    bool CmSupres(string sLabelToSuppress) ;
    void CmPutUp(string sLabelToPutUp) ;
    void CmModify(string sLabelToPutUp) ;

    void displayElement(NSLdvCurve* pCurve) ;
    void selectItem(string sItemLabel) ;

    void setScrollParams() ;

    NSLdvCurvesManagementPannel* getCurveManagementPannel() ;
    NSCurvesPropertyWindow*      getInterfaceAsList() ;

    NSCurvesProperty& operator=(const NSCurvesProperty& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class _NSLDVCLASSE NSCurvesPropertyWindow : public TListWindow, public NSRoot
{
  public:

    int iRes ;
    NSCurvesPropertyWindow(NSContexte* pCtx, TWindow* parent,                           NSCurvesProperty* pToon, int resId,                           int x, int y, int w,                           int h, TModule* module = 0) ;

    ~NSCurvesPropertyWindow() { lObjectCount-- ; }    void   SetupWindow() ;    void   Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;    void   EvPaint() ;
    void   displayElement(NSLdvCurve* pCurve) ;
    void   suppress() ;
    void   putUp() ;
    void   modify() ;
    void   newCurve() ;

    void   EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void   EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void   EvKeyDown(uint key, uint repeatCount, uint flags) ;    void   EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt) ;    void   LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;    // bool   PreProcessMsg(MSG& msg) ;    void   ButtonDblClk() ;    void   RButtonDown() ;    int    IndexItemSelect() ;
    int    getLabelOfSelectedItem(string* pLabelString) ;
    void   selectItem(string sItemLabel) ;

    void   EvSetFocus(HWND hWndLostFocus) ;
    void   EvKillFocus(HWND hWndGetFocus) ;

    NSCurvesProperty* getToon() { return _pToon ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSCurvesProperty* _pToon ;

    static long lObjectCount ;

	DECLARE_RESPONSE_TABLE(NSCurvesPropertyWindow) ;
};

// Drawing
class _NSLDVCLASSE NSLdvCurve
{
  public:

    NSLdvCurveYAxis*      _pAxis ;
    NSLdvCurveBackground* _pBackground ;

    string                _sItemLabel ;
    string                _sUnitLabel ;
    string                _sCompoundLabel ; // (value + unit)

    string                _sItemValue ;
    string                _sPath ;
    string                _sUnit ;

    NS_CLASSLIB::TColor   _color ;
    int                   _dotWidth ;

    // PS_SOLID 	    Creates a solid pen.
    // PS_DASH 	      Creates a dashed pen. Valid only when the pen width is one or less in device units.
    // PS_DOT 	      Creates a dotted pen. Valid only when the pen width is one or less in device units.
    // PS_DASHDOT	    Creates a pen with alternating dashes dots. Valid only when the pen width is one or less in device units.
    // PS_DASHDOTDOT	Creates a pen with alternating dashes double-dots. Valid only when the pen width is one or less in device units.
    // PS_NULL 	      Creates a null pen.
    // PS_INSIDEFRAME	Creates a solid pen. When this pen is used in any GDI drawing function that takes a bounding rectangle, the dimensions of the figure will be shrunk so that it fits entirely in the bounding rectangle, taking into account the width of the pen.
    //
    int                   _dotStyle ;

    enum POINT_ASPECT { paCircle = 0, paSquare, paTriangle, paStar };
    int                   _pointAspect ;
    int                   _pointRadius ;
    bool                  _bAdjustPointColorToValue ;

    NSLdvCurve(NSLdvCurvesManagementPannel* pPannel) ;
    NSLdvCurve(const NSLdvCurve& rv) ;
    ~NSLdvCurve() ;

    void init() ;
    void reinit(bool bUnitChanged = true) ;
    void reset() ;
    void disconnect() ;
    void reconnect() ;
    void pointsSetUnit() ;
    void pointsSetup() ;
    void close() ;

    int  giveYCoordinateForValue(double dValue, bool* pSuccess, string sValueUnitSens = string(""), bool bLimitedToAxisLimits = false) ;

    void setUnit(string sUnitToSet) ;
    void setCompoundLabel() ;

    NSLdvCurvesManagementPannel* getCurvesPannel() { return _pCurvesPannel ; }

    NSLdvCurve&	operator=(const NSLdvCurve& src) ;

    void initHistoryOfValuedGoals(ArrayGoals *pGoals) ;
    void buildBackgroundQuadrisFromGoals() ;

    void buildBackgroundQuadrisFromFile() ;

    NSLdvCurvePoint*      getFirstPoint()    { return _pFirstPoint ; }
    int                   getDotWidth()      { return _dotWidth ; }
    int                   getPointRadius()   { return _pointRadius ; }
    int                   getDotStyle()      { return _dotStyle ; }
    int                   getPointAspect()   { return _pointAspect ; }
    NSLdvCurveYAxis*      getYAxis()         { return _pAxis ; }
    NSLdvCurveBackground* getBackground()    { return _pBackground ; }
    string                getCompoundLabel() { return _sCompoundLabel ; }
    string                getUnit()          { return _sUnit ; }
    string                getItemValue()     { return _sItemValue ; }

    void                  setItemValue(string sIV) { _sItemValue = sIV ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

 protected:

    NSLdvCurvesManagementPannel* _pCurvesPannel ;
    NSLdvCurvePoint*             _pFirstPoint ;
    vector<NSLdvGoal*>           _aHistoryOfValuedGoals ;

    static long lObjectCount ;
};

typedef vector<NSLdvCurve*>   ArrayCurves ;
typedef ArrayCurves::iterator ArrayOfCurvesIter ;
typedef NTArray<NSLdvCurve>   ArrayOfCurves ;

class _NSLDVCLASSE NSLdvCurvesManagementPannel : public NSLdvWindowToon
{
  public:

    NSLdvCurvesManagementPannel(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvViewBase* pTheView) ;
    NSLdvCurvesManagementPannel(const NSLdvCurvesManagementPannel& rv) ;
    ~NSLdvCurvesManagementPannel() ;

    void CmNew() ;
    bool CmSupres(string sLabelToSuppress) ;
    void CmPutUp(string sLabelToPutUp) ;
    void CmModify(string sLabelToPutUp) ;

    void autoCreateNewCurve(string sLexiqueCode) ;
    void searchNextCurveParams(NS_CLASSLIB::TColor* pColor, int* pPtAspect, int* pPtRadius, int* pLineWidth) ;

    NSLdvCurve* searchExistingCurveForThisConcept(string sLexique) ;
    NSLdvCurve* searchExistingCurveForThisColor(int iRed, int iGreen, int iBlue) ;
    int         findCommonLineWidth() ;
    int         findCommonPointRadius() ;
    void        findNextColor(NS_CLASSLIB::TColor* pColor) ;

    void SetupWindow() ;
    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

    void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    void draw(TDC* pDc, NS_CLASSLIB::TRect* pRectARepeindre) ;

    void EvRButtonDown() ;

    void reinit() ;
    void disconnectCurves() ;

    void dysplayStatusMessage(string* pMessage) ;
    void dysplayPatientInformation() ;

    void drawText(TDC* pDc, string sText, string sSkinName, NS_CLASSLIB::TRect* pRect) ;

    int  getYAxisPixelRange() { return _lYAxisPixelRange ; }

    void setScrollParams() ;

    NSLdvCurvesManagementPannel& operator=(const NSLdvCurvesManagementPannel& src) ;

    NSLdvWindowToon* getToon() { return _pToon ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

 protected:

    ArrayOfCurves     _aCurves ;
    NSCurvesProperty* _pCurvesListManager ;

    // Every NSLdvCurve must work with the same pixel range
    //
    int               _lYAxisPixelRange ;

    void DispInfoListe(TLwDispInfoNotify& dispInfo) ;
    void LVNColumnclick(TLwNotify& lwn) ;

    void sortByName() ;
    void sortByEnding() ;
    void sortByBegining() ;

    bool bNaturallySorted ;
    int  iSortedColumn ;

    NSLdvWindowToon* _pToon ;

    static long lObjectCount ;
};

class _NSLDVCLASSE NSLdvCurvesManagementPannelInterface : public TWindow, public NSRoot
{
  public:

    NSLdvCurvesManagementPannelInterface(NSContexte* pCtx, TWindow* parent, NSLdvCurvesManagementPannel* pToon) ;
    NSLdvCurvesManagementPannelInterface(const NSLdvCurvesManagementPannelInterface& rv) ;
    ~NSLdvCurvesManagementPannelInterface() ;

    void SetupWindow() ;

    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    NSLdvCurvesManagementPannelInterface& operator=(const NSLdvCurvesManagementPannelInterface& src) ;

    NSLdvCurvesManagementPannel* getToon() { return _pToon ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSLdvCurvesManagementPannel* _pToon ;

    static long lObjectCount ;

  DECLARE_RESPONSE_TABLE(NSLdvCurvesManagementPannelInterface) ;
};

// Icon inside a NSLdvViewIconsShowWindow
//
class _NSLDVCLASSE NsObjectIcon
{
  public:

    NsObjectIcon(NSLdvObjetView* pObject) ;
    NsObjectIcon(NSLdvObjetView* pObject, NS_CLASSLIB::TRect rect) ;
    ~NsObjectIcon() ;

    NSLdvObjetView*    getObject() { return _pObject ; }

    NS_CLASSLIB::TRect getBox()                       { return _VisibleBox ; }
    void               setBox(NS_CLASSLIB::TRect box) { _VisibleBox = box ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSLdvObjetView*    _pObject ;
    NS_CLASSLIB::TRect _VisibleBox ;

    static long lObjectCount ;
};

// Icons window
//
class _NSLDVCLASSE NSLdvViewIconsShowWindow : public TWindow, public NSRoot
{
  protected:

    NSLdvViewAreaWindow*  _pParent ;
    vector<NsObjectIcon*> _aObjects ;

    hitToonStatus         _ActiveObjectStatus ;
    NsObjectIcon*         _ActiveObject ;

    NSTitleTip*           _pToolTip ;
    OWL::TPopupMenu*      _pPopupMenu ;

    bool                  _bTimerEnabled ;

    bool                  _bJustIcons ;
    bool                  _bJustPileUp ;

    OWL::TFont*           _pTextFont ;

    int                   _iBottomMargin ; // bottom margin allow for displaying
                                           // tips above cursor icon while
                                           // having cursor inside tips window

    int                   _iLeftSideWidth ;   // windows width where text is at icon's left
    int                   _iRightSideWidth ;  // windows width where text is at icon's right

    int                   _iExpectedLeftSideWidth ;
    int                   _iExpectedRightSideWidth ;
    int                   _iExpectedHeight ;

    int                   _iMaxLeftExcursion ;
    int                   _iMaxRightExcursion ;

    bool                  _bSizeIsSet ;

    int                   _iNbTimerTicks ;
    NS_CLASSLIB::TPoint   _previousCursorLocation ;

    static long lObjectCount ;

    void closeOpenObject() ;
    void setPosForActiveObject() ;
    void setPosForActiveObjectWindow(TWindow* pMdiWindow) ;
    void openActiveObject() ;

    void resize() ;
    void setSize() ;

    void Ouvrir() ;
    void Editer() ;
    void Oter() ;
    void Proprietes() ;
    void Parametres() ;
    void Detruire() ;
    void XmlExport() ;
    void Transfert() ;
    void Reassign() ;

    NSDocumentHisto* getHistoDocumentForActiveObject() ;

  public:

    NSLdvViewIconsShowWindow(NSContexte* pCtx, TWindow* parent) ;
    ~NSLdvViewIconsShowWindow() ;

    void SetupWindow() ;

    void Paint(TDC&, bool, NS_CLASSLIB::TRect& RectToPaint) ;
    void EvTimer(uint timerId) ;
    void EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    void EnableTimer() ;
    void DisableTimer() ;

    void resetObjects() ;
    void addObject(NSLdvObjetView*) ;

    int  getDiameter() ;

    NsObjectIcon*   getObjectFromPoint(NS_CLASSLIB::TPoint curLoc) ;
    bool            containsPoint(NS_CLASSLIB::TPoint curLoc) ;
    bool            isPointInPopup(NS_CLASSLIB::TPoint curLoc) ;

    NSDocumentInfo* getDocumentInfo(NsObjectIcon* pObjectIcon) ;

    int  getLeftSideWidth()        { return _iLeftSideWidth ; }
    void setLeftSideWidth(int iW)  { _iLeftSideWidth = iW ; }
    int  getRightSideWidth()       { return _iRightSideWidth ; }
    void setRightSideWidth(int iW) { _iRightSideWidth = iW ; }

    int getExpectedLeftSideWidth()  { return _iExpectedLeftSideWidth ; }
    int getExpectedRightSideWidth() { return _iExpectedRightSideWidth ; }

    void setBottomMargin(int iBM)  { _iBottomMargin = iBM ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  DECLARE_RESPONSE_TABLE(NSLdvViewIconsShowWindow) ;
};

// Icons window
//
class NSLdvViewMagnifier : public TWindow, public NSRoot
{
  protected:

    NSLdvViewAreaWindow* _pParent ;

    NSLigneView*         _pLine ;
    NVLdVPoint           _centerPoint ;
    int                  _iZoomFactor ;
    int                  _iOriginalWidth ;
    int                  _iOriginalHeight ;

    NSBitmap             _origBitmap ;

    NS_CLASSLIB::TPoint  _previousPoint ;

    static long lObjectCount ;

    void initPixels() ;
    int  getOriginalCoordinate(int iLocalCoordinate) ;

    NS_CLASSLIB::TRect getLocalRectFromLdvRect(const NS_CLASSLIB::TRect& QolRect, const NS_CLASSLIB::TPoint& centerPoint) ;
    NS_CLASSLIB::TRect getRectForQolLine(int iLine, const NS_CLASSLIB::TRect& QolRect, const NS_CLASSLIB::TPoint& centerPoint) ;

  public:

    NSLdvViewMagnifier(NSContexte* pCtx, TWindow* parent, NVLdVPoint& centerPoint, NSLigneView* pLine) ;
    ~NSLdvViewMagnifier() ;

    void SetupWindow() ;

    void EvPaint() ;
    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectToPaint) ;
    void PaintDecorations(TDC& dc, NS_CLASSLIB::TRect& RectToPaint) ;
    void PaintQoLDecorations(TDC& dc, NS_CLASSLIB::TRect& RectToPaint, NSConcernView *pConcernView) ;
    void EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void EvKillFocus(HWND hWndGetFocus) ;

    int getOriginalWidth()  { return _iOriginalWidth ;  }
    int getOriginalHeight() { return _iOriginalHeight ; }

    int getMagnifiedWidth()  { return _iOriginalWidth * _iZoomFactor ;  }
    int getMagnifiedHeight() { return _iOriginalHeight * _iZoomFactor ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  DECLARE_RESPONSE_TABLE(NSLdvViewMagnifier) ;
};


#endif // __NSLDVVUETECH_H

