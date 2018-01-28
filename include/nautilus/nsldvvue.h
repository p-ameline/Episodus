//----------------------------------------------------------------------------
//   Programme protégé par une licence Open Source GPL
//   NAUTILUS - www.nautilus-info.com
//----------------------------------------------------------------------------
#ifndef __NSLDVVUE_H
# define __NSLDVVUE_H

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
# include <classlib\arrays.h>

# include "nsbb\nstip.h"
# include "nsbb\nslistwind.h"
# include "nautilus\nsldvdoc.h"
# include "nsepisod\nsldvuti.h"
# include "nautilus\nsldvvar.h"

# include "nautilus\nsldvgoal.h"
# include "partage\ns_timer.h"

# include "nautilus\nsldvtoons.h"

#define ICONWIDTH 16

enum pixUnit { pixAns = 0, pixMois, pixSemaine, pixJour, pixHeure, pixMinute, pixSeconde } ;

class NSLdvViewArea ;

class NSLdvTimeToon : public NSLdvToon
{
	public :

	  NVLdVRect _Box ;            // Boite temporelle réelle
	  NVLdVRect _VisibleBox ;     // Boite temporelle visible (exemple icone)

    NSLdvTimeToon(NSContexte* pCtx, NSLdvViewArea *pWorkArea, bool bVScrollEnabled) ;
	  NSLdvTimeToon(const NSLdvTimeToon& rv) ;
	  virtual ~NSLdvTimeToon() { lLocalObjectCount-- ; }

    void			  	     initialiser(const NSLdvTimeToon* pSrc) ;

    NS_CLASSLIB::TRect donneRectangle() ;
    NS_CLASSLIB::TRect donneVisibleRectangle() ;
	  void               fixeRectangle(NVLdVTemps tDeb, NVLdVTemps tFin, long lPlancher, long lPlafond) ;
    void               donneRectangle(NVLdVTemps* ptDeb, NVLdVTemps* ptFin, long* plPlancher, long* plPlafond) ;
    void               adjustToPixelTimeUnit() ;

    int                getFixedWidth() { return _iFixedWidthInPixels ; }

    NSLdvTimeToon& 		 operator=(const NSLdvTimeToon& src) ;

    bool               isVerticalScrollEnabled() { return _bVerticalScrollEnabled ; }

    NSLdvViewArea*     getWorkingArea() { return _pWorkingArea ; }

    static long getNbInstance()  { return lLocalObjectCount ; }
    static void initNbInstance() { lLocalObjectCount = 0 ; }

  protected:

	  int           _iFixedWidthInPixels ;
    bool          _bVerticalScrollEnabled ;
    NSLdvViewArea *_pWorkingArea ;

    static long lLocalObjectCount ;
} ;

# include "nautilus\nsldvvuetech.h"

class NSPbModifView : public NSLdvTimeToon
{
  protected:

    static long lObjectCount ;

  public:

    NSConcernModifier* pModifier ;

	  NSPbModifView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcernModifier* pModif) ;
	  NSPbModifView(const NSPbModifView& rv) ;
	  ~NSPbModifView() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void EvRButtonDown() ;
	  void createComponents(ArrayOfToons* pToonsArray) ;

	  NSPbModifView& operator=(const NSPbModifView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class NSLigneView : public NSLdvTimeToon
{
  public:

	  bool bClos ;

	  NSLigneView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, bool bVScrollEnabled) ;
	  NSLigneView(const NSLigneView& rv) ;
	  virtual ~NSLigneView() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void EvRButtonDown() ;
	  void boxFitting() ;

    int  getIndex()        { return _iIndex ; }
    void setIndex(int iDx) { _iIndex = iDx ; }

	  NSLigneView& operator=(const NSLigneView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    // Position en y : juste au dessus de la ligne de base -> iIndex = 0
	  // yOrder : if just above the baseline, then iIndex = 0;
    //
    int _iIndex ;

    static long lObjectCount ;
};

class NSConcernView : public NSLigneView
{
  public:

	  // Marges dues aux decorations (ex effet 3D) en pixels
	  // Margins due to decorations (ex 3D effect); unit = pixel
    int _iLeftMargin, _iRigthMargin, _iTopMargin, _iBottomMargin ;

	  NSConcernView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pPb, LDVFRAME iFrame, int iIdx = -1) ;
	  NSConcernView(const NSConcernView& rv) ;
	  ~NSConcernView() ;

    void            SetupWindow() ;
	  virtual void		draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    void						drawTitle(TDC* pDc, NVLdVRect* pRectARepeindre, NS_CLASSLIB::TPoint* pPtTopLeft) ;
    void						draw3DBorders(TDC* pDc, NVLdVRect* pRectARepeindre, OWL::TPen* pLightPen, OWL::TPen* pDarkPen, OWL::TPen* pTransitionPen, NS_CLASSLIB::TPoint* pPtTopLeft, NS_CLASSLIB::TPoint* pPtBotRight) ;
    void						drawGoals(TDC* pDc, NVLdVRect* pRectARepeindre, OWL::TPen* pLightPen, OWL::TPen* pDarkPen, OWL::TPen* pTransitionPen, NVLdVPoint* pLdvTopLeft, NVLdVPoint* pLdvBottomRight, NVLdVTemps* pTLeftTime, NVLdVTemps* pTRightTime) ;
	  void						drawBaddestGoal(TDC* pDc, NVLdVRect* pRectARepeindre) ;
    void						drawQolPattern(TDC* pDc, NVLdVRect* pRectARepeindre, NSQualityOfLifeChart* pQolChart, NVLdVPoint* pLdvTopLeft, NVLdVPoint* pLdvBottomRight) ;
    static int 			getYForQolLine(int iYForLineTop, int iLineNb) { return iYForLineTop + 3 * iLineNb ; }
    static int 			getQolLineHeight() { return 2 ; }
    static int 			getQolTopMargin()  { return 2 ; }
    ldvBrush*       getBrushForScore(int iScore) ;
    ldvBrush*       getBrushForColor(int iScore) ;
    void            initPens(OWL::TBrush** ppBrush, OWL::TPen** ppLightPen, OWL::TPen** ppDarkPen, OWL::TPen** ppTransitionPen) ;
    OWL::TBrush*    getBrushForSeverity(int iSeverity) ;
    OWL::TPen*      getPenForScore(int iScore) ;
	  void						EvRButtonDown() ;
	  void						createComponents(ArrayOfToons* pToonsArray) ;

    NSConcern*      getConcern() { return _pConcern ; }
    LDVFRAME        getFrame()   { return _iFrame ; }

	  NVLdVTemps			getGoalInfoDate(NSLdvGoalInfo *pInfoGoal, NVLdVTemps tDateOrigine) ;
	  NSConcernView&	operator=(const NSConcernView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSConcern*   _pConcern ;
    LDVFRAME     _iFrame ;

    static long lObjectCount ;
} ;

/*
class NSSocialView : public NSConcernView
{
 public:

	NSSocialView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pPb, int iIdx = -1) ;
	NSSocialView(NSSocialView& rv) ;
	~NSSocialView() {}

	NSSocialView&	  operator=(NSSocialView& src) ;
} ;

class NSRiskView : public NSConcernView
{
 public:

	NSRiskView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pRisk, int iIdx = -1) ;
	NSRiskView(NSRiskView& rv) ;
	~NSRiskView() {}

  void draw(TDC* pDc, NVLdVRect* pRectARepeindre) ;

	NSRiskView&	 operator=(NSRiskView& src) ;
} ;
*/

class NSPreoccupView : public NSLigneView
{
  protected:

    static long lObjectCount ;

  public:

/*
	// Position en y : juste au dessus de la ligne de base -> iIndex = 0
	// yOrder : if just above the baseline, then iIndex = 0;
  int iIndex;
*/

	  // Marges dues aux decorations (ex effet 3D) en pixels
	  // Margins due to decorations (ex 3D effect); unit = pixel
	  int _iLeftMargin, _iRigthMargin, _iTopMargin, _iBottomMargin ;

	  NSConcern* pPreoccupation ;

	  NSPreoccupView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pPreoccup, int iIdx) ;
	  NSPreoccupView(const NSPreoccupView& rv) ;
	  ~NSPreoccupView() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void EvRButtonDown() ;
	  void createComponents(ArrayOfToons* pToonsArray) ;

	  NSPreoccupView& operator=(const NSPreoccupView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class NSEventView : public NSLigneView
{
  public:

/*
	// Position en y : juste au dessus de la ligne de base -> iIndex = 0
	// yOrder : if just above the baseline, then iIndex = 0;
	int iIndex;
*/

	  // Marges dues aux decorations (ex effet 3D) en pixels
	  // Margins due to decorations (ex 3D effect); unit = pixel
    int _iLeftMargin, _iRigthMargin, _iTopMargin, _iBottomMargin ;

	  NSConcern*   pConcern ;

	  NSEventView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pPb, int iIdx = -1) ;
	  NSEventView(const NSConcernView& rv) ;
	  ~NSEventView() ;

    void         SetupWindow() ;
	  void				 draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void				 EvRButtonDown() ;
	  void         createComponents(ArrayOfToons* pToonsArray) ;

    bool         isPlannedEvent()                 { return _bIsPlannedFutureEvent ; }
    void         setPlannedEvent(bool bIsPlanned) { _bIsPlannedFutureEvent = bIsPlanned ; }

	  NSEventView& operator=(const NSEventView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    bool         _bIsPlannedFutureEvent ;

    static long lObjectCount ;
} ;

class NSDrugLineView : public NSLigneView
{
  public:

	  // Marges dues aux decorations (ex effet 3D) en pixels
	  // Margins due to decorations (ex 3D effect); unit = pixel
	  int _iLeftMargin, _iRigthMargin, _iTopMargin, _iBottomMargin ;

	  NSDrugLineView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvDrug* pLdvDrug, LDVFRAME iFrame, int iIdx) ;
	  NSDrugLineView(const NSDrugLineView& rv) ;
	  ~NSDrugLineView() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void EvRButtonDown() ;
	  void createComponents(ArrayOfToons* pToonsArray) ;

	  NSDrugLineView& operator=(const NSDrugLineView& src) ;

    NSLdvDrug* getDrug()  { return _pDrug ; }
    LDVFRAME   getFrame() { return _iFrame ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSLdvDrug* _pDrug ;
    LDVFRAME   _iFrame ;

    static long lObjectCount ;
};

class NSGoalLineView : public NSLigneView
{
  protected:

    static long lObjectCount ;

  public:

	  // Marges dues aux decorations (ex effet 3D) en pixels
	  // Margins due to decorations (ex 3D effect); unit = pixel
	  int _iLeftMargin, _iRigthMargin, _iTopMargin, _iBottomMargin ;

	  NSLdvGoal* pGoal ;

    NSGoalLineView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvGoal* pLdvGoal, int iIdx) ;
	  NSGoalLineView(const NSGoalLineView& rv) ;
	  ~NSGoalLineView() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void EvRButtonDown() ;
	  void createComponents(ArrayOfToons* pToonsArray) ;

	  NSGoalLineView& operator=(const NSGoalLineView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class NSBaseLineView : public NSLigneView
{
  protected:

    static long lObjectCount ;

  public:

 	  NSBaseLineView(NSContexte* pCtx, NSLdvViewArea *pWorkArea) ;
	  NSBaseLineView(const NSBaseLineView& rv) ;
	  ~NSBaseLineView() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void EvRButtonDown() ;

	  NSBaseLineView& operator=(const NSBaseLineView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};


// Partie d'un objet spécifique à une préoccupation
class NSSsObjView : public NSLdvTimeToon
{
  protected:

    static long lObjectCount ;

  public:

	  NSLdvSousObjet  *_pSsObjet ;
	  NSLdvObjetView  *_pObjetView ;
	  NSConcernView   *_pConcernView ;

    NSSsObjView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvSousObjet* pSsObj, NSLdvObjetView *pObjV, NSConcernView *pPbV) ;
	  NSSsObjView(const NSSsObjView& rv);
	  ~NSSsObjView();

    void          SetupWindow() ;
	  void    			draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void    			drawInBasket(TDC *pDc, NVLdVRect *pRectARepeindre) ;

	  void					EvRButtonDown() ;
	  void					createComponents(ArrayOfToons* pToonsArray) ;

	  void					setConcernView(NSConcernView *pPbView) { _pConcernView = pPbView ; }
	  NSConcernView	*getConcernView() { return _pConcernView ; }

    NSSsObjView&	operator=(const NSSsObjView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

typedef vector<NSSsObjView*> SsObjViewVector ;
typedef SsObjViewVector::iterator       SsObjViewIterator ;
typedef SsObjViewVector::const_iterator SsObjViewConstIterator ;

// Point for curve drawing
//
class NSLdvCurvePoint : public NSLdvTimeToon
{
  protected:

    NSLdvCurvePoint* _pNextBrother ;

    static long lObjectCount ;

  public:

 	  NSLdvCurve* _pCurve ;

    // Initial information
    //
 	  NSHistoryValueManagement _value ;

    // Information for selected unit
    //
    double _dValue ;
    string _sUnitSens ;

	  NSLdvCurvePoint* _pPreviousBrother ;

	  NSLdvCurvePoint(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvCurve* pTheCurve, NSHistoryValueManagement* pHistoryValue) ;
	  NSLdvCurvePoint(const NSLdvCurvePoint& rv) ;
	  ~NSLdvCurvePoint() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    void drawPoint(TDC* pDc, NS_CLASSLIB::TPoint screenPoint, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen) ;
    void drawPointCircle(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen) ;
    void drawPointSquare(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen) ;
    void drawPointUpTriangle(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen) ;
    void drawPointDownTriangle(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen) ;
    void drawPointStar(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen) ;

    void setActualValues(double dNewValue, string sNewUnitSens) ;
    void resetActualValues() ;

	  void EvRButtonDown() ;

    NSLdvCurvePoint* getNextBrother()                        { return _pNextBrother ; }
    void             setNextBrother(NSLdvCurvePoint* pLdvPt) { _pNextBrother = pLdvPt ; }

    NSLdvCurvePoint& operator=(const NSLdvCurvePoint& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class NSLdvObjetView : public NSLdvTimeToon
{
  protected:

    static long lObjectCount ;

  public:

	  NSLdvObjet  *_pObjet ;
	  NSLigneView *_pLigneView ;

	  NSLdvObjetView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvObjet* pObj, NSLigneView *pPbView) ;
	  NSLdvObjetView(const NSLdvObjetView& rv) ;
	  ~NSLdvObjetView() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
    int  drawIcon(TDC* pDc, NS_CLASSLIB::TPoint* pPtTopLeft, NS_CLASSLIB::TRect* pPixelVisibleBox, bool bDontDraw = false) ;
	  void EvRButtonDown() ;
	  void createComponents(ArrayOfToons* pToonsArray) ;

	  void         setLigneView(NSLigneView *pLineView) { _pLigneView = pLineView ; }
	  NSLigneView* getLigneView() { return _pLigneView ; }

    NSLdvObjet*  getObject() { return _pObjet ; }

	  NSLdvObjetView& operator=(const NSLdvObjetView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class NSQualityOfLifeChartView : public NSLdvTimeToon
{
  protected:

    static long lObjectCount ;

  public:

	  NSQualityOfLifeChart *_pQoLChart ;
	  NSLigneView          *_pLigneView ;

	  NSQualityOfLifeChartView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSQualityOfLifeChart* pCC, NSLigneView *pPbView) ;
	  NSQualityOfLifeChartView(const NSQualityOfLifeChartView& rv) ;
	  ~NSQualityOfLifeChartView() ;

    void SetupWindow() ;
	  void draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) ;
	  void EvRButtonDown() ;
	  void createComponents(ArrayOfToons* pToonsArray) ;

	  void         setLigneView(NSLigneView *pLineView) { _pLigneView = pLineView ; }
	  NSLigneView* getLigneView() { return _pLigneView ; }

    NSQualityOfLifeChart* getQualityOfLifeChart() { return _pQoLChart ; }

    ldvBrush*    getBrushForScore(int iScore) ;

	  NSQualityOfLifeChartView& operator=(const NSQualityOfLifeChartView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

// affichage des objectifs
//
// Ce Toon représente un objectif ponctuel ou un cycle pour un objectif cyclique

class		NSLdvGoalCycleView : public NSLdvTimeToon
{
  protected:

    static long lObjectCount ;

  public:

	  NSLdvGoal       *_pGoal ;
    NSLigneView     *_pLigneView ;

    GoalInfoArray   _aJalons ;

  /*NVLdVTemps      tpsPossible;
        NVLdVTemps      tpsGood;                // |----PPPGGGGIIIIIGGGGGEEEECCCCCCCCC
        NVLdVTemps      tpsIdeal;
        NVLdVTemps      tpsEndIdeal;
        NVLdVTemps      tpsEndGood;
        NVLdVTemps      tpsCritic; */

    NSLdvGoalCycleView(NSContexte *pCtx, NSLdvViewArea *pWorkArea, NSLdvGoal* pGo, NSLigneView *pPbView) ;
    NSLdvGoalCycleView(const NSLdvGoalCycleView& rv) ;
    ~NSLdvGoalCycleView() ;

    void SetupWindow() ;
    void draw(TDC *pDC, NVLdVRect *pRectARepeindre) ;

	  NSLdvGoalCycleView& operator=(const NSLdvGoalCycleView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

class NSLdvTankView : public NSRoot
{
	public :

	  NSLdvTankView(NSLdvView *pView, NSContexte *pCtx) ;
	  ~NSLdvTankView() ;

	  void    NSLdvTankView::updateTankList() ;

	  NS_CLASSLIB::TRect  coords() ;
	  bool                setCoords(NS_CLASSLIB::TRect rect) ;
	  bool                setCoords(int left, int top, int right, int bottom) ;
	  void                affiche(TDC *pDc, NS_CLASSLIB::TRect rectARepeindre) ;
	  bool                addSsObj(NSLdvSousObjet *pSsObj) ;
	  bool                delSsObj(SsObjViewIterator SsObjIter) ;

	  SsObjViewVector     _aSsObjetTankArray ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected :

	  NS_CLASSLIB::TRect  _rectCoords ;
    NSLdvView           *_pLdvView ;

    static long lObjectCount ;
} ;

class NSTimeZoomLevel
{
  public :

	  NSTimeZoomLevel(pixUnit iPixUnit, int iNbUPP, int iLowerUPP, int iUpperUPP) ;
	  ~NSTimeZoomLevel() ;

	  pixUnit getPixUnit()    { return _iPixUnit ; }
    int     getUppRate()    { return _iNumberOfUnitPerPixel ; }
    int     getUpperLimit() { return _iUpperLimitForUnitPerPixel ; }
    int     getLowerLimit() { return _iLowerLimitForUnitPerPixel ; }

    void setPixUnit(pixUnit iPU) { _iPixUnit = iPU ; }
    void setUppRate(int iUR)     { _iNumberOfUnitPerPixel = iUR ; }
    void setUpperLimit(int iUL)  { _iUpperLimitForUnitPerPixel = iUL ; }
    void setLowerLimit(int iLL)  { _iLowerLimitForUnitPerPixel = iLL ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected :

	  pixUnit _iPixUnit ;
    int     _iNumberOfUnitPerPixel ;
    int     _iUpperLimitForUnitPerPixel ;
	  int     _iLowerLimitForUnitPerPixel ;

    static long lObjectCount ;
} ;

typedef vector<NSTimeZoomLevel*> NSTimeZoomLevelVector ;
typedef NSTimeZoomLevelVector::iterator       NSTimeZoomLevelIter ;
typedef NSTimeZoomLevelVector::const_iterator NSTimeZoomLevelConstIter ;
typedef NTArray<NSTimeZoomLevel> NSTimeZoomLevelArray ;

class NSLdvWindowToon ;
class NSLdvChronoLine ;
class NSLdvCommandPannel ;

class NSLdvView : public TWindowView, public NSRoot
{
  public:

	  enum mouseMode { modeCursor = 0, modeZoom } ;

  protected:

	  NSLdvDocument        *_pLdVDoc ;

    string               _sConcern ; // concern dedicated view

    // double            dUniteX, dUniteY;
	  pixUnit              iXunit ;
 	  int                  iZoomFactor ;

    NSTimeZoomLevelArray aZoomLevels ;
    NSTimeZoomLevel*     pCurrentTimeInfo ;

	  mouseMode           iMouseMode ;

	  int                 iLargeurChronoLine ;

	  string              skinName ;              // nom de la fenetre de skin

	  // Position du coin inférieur-droit par rapport au coin inf-dt global
	  NVLdVTemps          _tDateHeureInfDroit ;

	  bool                _bSystemColors ;
	  bool                _bDisplayDrugs, _bDisplayGoals ;
    int                 _iMinSeverityOfDisplayedConcerns ;

	  NS_CLASSLIB::TRect  rectDateTimeArea ;
	  NSLdvTankView       *pLdvTankView ;

	  OWL::TStatusBar*    pSB ;
    HACCEL              hAccelerator ;

	  OWL::TImageList     *ImgDragDrop ;

    int                 _iNbTimerTicks ;
    NS_CLASSLIB::TPoint _previousCursorLocation ;

    bool                _bPainted ;

    static long lObjectCount ;

  public:

    //
    // Les objets à afficher sont là
    // Here are the objects to draw
    //
	  ArrayOfToons    _aToons ;

  	bool            bSetupToolBar ;

	  ldvBrush*       pPinceauFond ;
	  ldvBrush*       pPinceauFondSys ;

	  ldvBrush*       pPinceauBlack ;
	  ldvBrush*       pPinceauRed ;
	  ldvBrush*       pPinceauBleu ;
	  ldvBrush*       pPinceauS1 ;
	  ldvBrush*       pPinceauS2 ;
	  ldvBrush*       pPinceauS3 ;
    ldvBrush*       pPinceauS34 ;
	  ldvBrush*       pPinceauS4 ;
    ldvBrush*       pPinceauSH1 ;
	  ldvBrush*       pPinceauSH2 ;
	  ldvBrush*       pPinceauSH3 ;
	  ldvBrush*       pPinceauSH4 ;

    OWL::TPen*      pPenEclaire ;
    OWL::TPen*      pPenEclaireSys ;
    OWL::TPen*      pPenOmbre ;
    OWL::TPen*      pPenOmbreSys ;
    OWL::TPen*      pPenTransition ;
    OWL::TPen*      pPenTransitionSys ;

    OWL::TPen*      pSelPenEclaire ;
    OWL::TPen*      pSelPenOmbre ;
    OWL::TPen*      pSelPenTransition ;

    OWL::TPen*      pPenS1 ;
    OWL::TPen*      pPenS2 ;
    OWL::TPen*      pPenS3 ;
    OWL::TPen*      pPenS4 ;
    OWL::TPen*      pPenS5 ;

    OWL::TIcon*     pLdvObject ;
    OWL::TIcon*     pLdvSsObject ;

    NSLdvView(NSLdvDocument& doc, TWindow* parent = 0);
    NSLdvView(const NSLdvView& rv);
    ~NSLdvView();

    void                reinit() ;
    void                disconnectCurves() ;
    void                reinitCurves() ;

    void                createWorkingArea(string sConcernRef) ;
    void                initWorkingArea(NSLdvViewArea* pWorkArea, bool bAtCreation = false) ;
    void                emptyWorkingAreas() ;
    void                reinitWorkingAreas() ;

    // Fonctions de réponse aux messages
    void                Paint(TDC&, bool, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void                EvSize(uint sizeType, ClassLib::TSize& size) ;
    void                EvSetFocus(THandle hWndLostFocus /* may be 0 */) ;
    void                EvTimer(uint timerId) ;
    void                EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl) ;
    LRESULT             EvMouseWheel(WPARAM wparam, LPARAM lparam) ;
    void                EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void                EvSysKeyDown(uint key, uint repeatCount, uint flags) ;
    void                CmHelp() ;

    void                EvaluateWinToonsPos(TWindow* pWindow, ArrayOfToons* pToons) ;

    void                UpdatePixelPerUnitRate(uint) ;
    void                UpdateSeverityThreshold(uint) ;

    void                EvDragDrop() ; // temp -- pour l'instant inutilisé (méthode vide)

    void                EvHotKey(int idHotKey) ;
    HACCEL              getAcceleratorHandle() { return hAccelerator ; }

    void                CmLdvZoomIn() ;
    void                CmLdvZoomOut() ;
    void                change() ;
    void                timeBumper() ;

    void                CmLdvPixSecond() ;
    void                CmLdvPixMinute() ;
    void                CmLdvPixHour() ;
    void                CmLdvPixDay() ;
    void                CmLdvPixWeek() ;
    void                CmLdvPixMonth() ;

    void                handlePixelUnitChange() ;
    void                initZoomLevelInfo() ;
    void                setZoomLevelInfo() ;
    string              getLdvPixLexiqueCode() ;

    void                pixelUnitChanged() ;

    void                showMousePositionMessage() ;
    void                skinSwitchOn(string sSkinName);
    void                skinSwitchOff(string sSkinName);

    //
    // Concern related response functions
    //
    void                CmNewPreoccup() ;
    void                CmNewRisk() ;
    void                CmNewSocialConcern() ;

    void                CmNewDrugsFromRefForConcern(NSConcern* pConcern) ;
    void                CmNewGoalsFromRefForConcern(NSConcern* pConcern) ;

    void                CmChgPreoccup() ;
    void                CmChgPreoType(NSConcern* pConcern, LDVFRAME iFrame) ;
    void                CmChgPreoDate(NSConcern* pConcern, LDVFRAME iFrame) ;
    void                CmChgPreoIndx(NSConcern* pConcern, NVLdVTemps tpsTarget, LDVFRAME iFrame) ;
    void                CmAddObjectifs(NSConcern* pConcern, LDVFRAME iFrame) ;
    void                CmProject(NSConcern* pConcern, LDVFRAME iFrame, string sKsName, string sKsArchetype) ;

    void                CmEvolPreoccup(LDVFRAME iFrame) ;
    void                CmEvolPreoType(NSConcern* pConcern, LDVFRAME iFrame) ;
    void                CmEvolPreoIndx(NSConcern* pConcern, NVLdVTemps tpsTarget, LDVFRAME iFrame) ;

    void                CmSuppressPreo(NSConcern* pConcern) ;

    void                CmManageRights(NSConcern* pConcern) ;
    void                CmManageRights(NSLdvDrug* pDrug) ;

    //
    // Drug related response functions
    //
    void                CmDrugNew() ;
    void                CmDrugNewForConcern(NSConcern* pRelatedConcern) ;
    void                CmDrugRenew(NSLdvDrug* pDrug) ;
    void                CmDrugModify(NSLdvDrug* pDrug) ;
    void                CmDrugChangePoso(NSLdvDrug* pDrug) ;
    void                CmDrugStop(NSLdvDrug* pDrug) ;
    void                CmDrugDelete(NSLdvDrug* pDrug) ;
    void                CmDrugPrescription() ;
    void                CmDrugRights() ;

    //
    // Miscellanous response functions
    //
    void                CmChange2Cursor() ;
    void                CmCursorEnable(TCommandEnabler &ce) ;
    void                CmChange2Zoom() ;
    void                CmZoomEnable(TCommandEnabler &ce) ;
    void                CmSwitchDrugs() ;
    void                CmSwitchGoals() ;

    void                CmShowGoalsForConcern() ;
    void                CmShowDrugsForConcern() ;
    void                CmShowFollowUpForConcern() ;
    void                CmShowCurvesForConcern() ;

    // Implémentation des méthodes virtuelles pures de TView
    static LPCSTR       StaticName()    { return "NSLdvView" ; }
    const char*         GetViewName()   { return StaticName() ; }
    TWindow*            GetWindow()     { return (TWindow*) this ; }

    // Overrides TWindows' in order to process our accelerators table
    bool PreProcessMsg(MSG &msg) ;

    // Fonction d'initialisation
    void                SetupWindow() ;
    void                SetupToolBar() ;
    void                SetWindowPosLdv() ;
    void                EnregistrePosLdv() ;
    void                SetWindowPosit() ;     // deprecated
    void                RecordWindowPosit() ;
    void                reloadView(string sReason, string sReference = string("")) ;

    bool                CanClose() ;
    void                EvClose() ;
    void                EvDestroy() ;
    bool                EvQueryEndSession() ;

    string              getConcern()            { return _sConcern ; }
    pixUnit             getXunit()              { return iXunit ; }
    bool                isZoomMax()             { return (pixSeconde == iXunit) ; }
    bool                isZoomMin()             { return (pixMois == iXunit) ; }
    mouseMode           getMouseMode()          { return iMouseMode ; }
    int                 getSeverityThreshold()  { return _iMinSeverityOfDisplayedConcerns ; }
    int                 getZoomFactor()         { return iZoomFactor ; }
    int                 getPpuRate()            { return pCurrentTimeInfo->getUppRate() ; }
    int                 getUpperPpuLevel()      { return pCurrentTimeInfo->getUpperLimit() ; }
    int                 getLowerPpuLevel()      { return pCurrentTimeInfo->getLowerLimit() ; }
    int                 getLargeurChronoLine() ;

    NVLdVTemps          getDateHeureInfDroit()  { return _tDateHeureInfDroit ; }
    void                setRightTimeFromLeftTime(const NVLdVTemps& tLeftTime) ;

    bool                useSystemColors()       { return _bSystemColors ; }
    OWL::TImageList*    getImgDragDrop()        { return ImgDragDrop ; }
    bool                mustDisplayDrugs()      { return _bDisplayDrugs ; }
    bool                mustDisplayGoals()      { return _bDisplayGoals ; }

    void                setXunit(pixUnit iU)         { iXunit = iU ; }
    void                refreshChronoLine() ;
    void                setSeverityThreshold(int iT) { _iMinSeverityOfDisplayedConcerns = iT ; }
    void                setZoomFactor(int iZ)        { iZoomFactor = iZ ; }

    int                 getPhysicalWidthFromTimeUnit(int iTimeUnitCount) { return (pCurrentTimeInfo->getUppRate() == 0) ? iTimeUnitCount : iTimeUnitCount * (pCurrentTimeInfo->getUpperLimit() - pCurrentTimeInfo->getUppRate()) / pCurrentTimeInfo->getUpperLimit() ; }
    int                 getTimeUnitFromPhysicalWidth(int iPhysicalWidth) { return (pCurrentTimeInfo->getUppRate() == 0) ? iPhysicalWidth : iPhysicalWidth * pCurrentTimeInfo->getUpperLimit() / (pCurrentTimeInfo->getUpperLimit() - pCurrentTimeInfo->getUppRate()) ; }

    NSTimeZoomLevel*    getTimeZoomInfo(pixUnit iU) ;
    NSTimeZoomLevel*    getCurrentZoomInfo()         { return pCurrentTimeInfo ; }

    int                 getNbTimerTicks()            { return _iNbTimerTicks ; }
    void                resetNbTimerTicks()          { _iNbTimerTicks = 0 ; }

    NSLdvDocument*      getDoc()                { return _pLdVDoc ; }

    NS_CLASSLIB::TRect  getDateTimeArea() { return rectDateTimeArea ; }
    void                setDateTimeArea(NS_CLASSLIB::TRect rectArea) { rectDateTimeArea = rectArea ; }
    void                setDateTimeArea(int left, int top, int right, int bottom) { setDateTimeArea(NS_CLASSLIB::TRect(left, top, right, bottom)) ; }

    NS_CLASSLIB::TRect  getLdvTankView() { return pLdvTankView->coords() ; }

    HBRUSH              getBlackBrushHandle() { return pPinceauBlack->GetHandle() ; }
    HBRUSH              getRedBrushHandle()   { return pPinceauRed->GetHandle() ; }

    // ajouter des méthodes qui permettent de récupérer des sous-objets
    // objets problèmes préoccupations sans avoir à tout réinitialiser
    // leur liste respectives
    // juste en leur passant un pointeur sur cet "objet" en question
    bool                addSsObj(NSLdvSousObjet *pSsObj) ;
    bool                addObj(NSLdvObjet *pObj) ;
    void                removeObj(NSLdvObjet *pObj) ;
    bool                addQualityOfLifeChart(NSQualityOfLifeChart *pCharts) ;
    void                removeQualityOfLifeChart(NSQualityOfLifeChart *pCharts) ;
    bool                addQualityOfLifeChart(NSQualityOfLifeChart *pCharts, NSConcern *pPb) ;
    void                removeQualityOfLifeChart(NSQualityOfLifeChart *pCharts, NSConcern *pPb) ;
    bool                addProb(NSConcern *pPb) ;

    bool                addObjOnConcern(NSConcern *pPb, NSLdvObjet *pObj) ;

    void		            addDrugs() ;
    void		            removeDrugs() ;
    void		            reinitDrugs() ;
    void		            addGoals() ;
    void                removeGoals() ;

    void                reorganizeLines() ;

    void                displayCurvesForBiometricalGoals(string sReference) ;

    void                setAppointment() ;

    //                  ------------- toons management -----------------

    void                addToToonsArray(ArrayOfToons* pToonsArray, NSLdvToon* pToon) ;
    bool                removeFromToonsArray(ArrayOfToons* pToonsArray, NSLdvToon* pToon) ;
    void                deleteAllFromToonsArray(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType) ;

    NSLdvTimeToon*      getLdVTimeToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType) ;
    NSLdvWindowToon*    getLdVWinToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType) ;
    NSLdvToon*          getLdVToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType) ;
    NSLdvChronoLine*    getLdVChronoLine() ;
    NSLdvCommandPannel* getLdVCommandPannel() ;
    NSLdvViewArea*      getActiveWorkingArea() ;

    NVLdVTemps          getDateNaissance() ;

    void                afficheStatusMessage(string& sMessage, bool bScroll) ;

    void                setTipsTimer()  { SetTimer(ID_LDVTIMER, TIMER_VALUE) ; }
    void                killTipsTimer() { KillTimer(ID_LDVTIMER) ; }

    NS_CLASSLIB::TPoint getGlobalPhysicalPoint(NVLdVPoint ldvPoint) ;
    NS_CLASSLIB::TRect  getGlobalPhysicalRect(NVLdVRect ldvRect) ;
    NS_CLASSLIB::TRect  getGlobalPhysicalRect(NS_CLASSLIB::TRect ldvWinRect) ;

    bool                convertUnit(double* pdValue, string sResultUnit, string sInitialUnit, string sRelatedTo = "", bool bVerbose = false) ;

    bool                getRelativeValue(int* piNewValue, TWindow* pRefWindow, NSLdvWindowToon* pToon, string sReference, string sDirection) ;
    void                getReferenceAndDirection(string sReferenceString, string sNeededDirection, string* psReference, string* psDirection) ;

    NSLdvView&          operator=(const NSLdvView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

 DECLARE_RESPONSE_TABLE(NSLdvView) ;
};

class NSLdvViewArea : public NSLdvWindowToon
{
  public:

    NSLdvViewArea(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pTheView, string sConcernRef, bool bMain = false) ;
    NSLdvViewArea(const NSLdvViewArea& rv) ;
    ~NSLdvViewArea() ;

    void SetupWindow() ;

    void reinit() ;
    void calculRectangleGlobal() ;

    void reinitCurves() ;

    bool isMainView()          { return _bMainView ; }

    string getConcernRef()     { return _sConcernReference ; }

    int  getLargeurLigne()     { return _iLargeurLigne ; }
    int  getLargeurSubLigne()  { return _iLargeurSubLigne ; }
    int  getLargeurBaseline()  { return _iLargeurBaseline ; }
    int  getInterligne()       { return _iInterligne ; }
    int  getDrugInterligne()   { return _iDrugInterLigne ; }
    int  getBottomMargin()     { return _iBottomMargin ; }

    NVLdVRect*          getGlobalBox() { return &_GlobalBox ; }

    NS_CLASSLIB::TPoint getAreaPhysicalPoint(NVLdVPoint ldvPoint) ;
    NS_CLASSLIB::TRect  getAreaPhysicalRect(NVLdVRect ldvRect) ;
    NS_CLASSLIB::TRect  getAreaPhysicalRect(NS_CLASSLIB::TRect ldvWinRect) ;

    NS_CLASSLIB::TPoint getScrollablePhysicalPoint(NVLdVPoint ldvPoint) ;
    NS_CLASSLIB::TRect  getScrollablePhysicalRect(NVLdVRect ldvRect) ;

    NVLdVPoint          getAreaLogicalPointFromWindowPoint(NS_CLASSLIB::TPoint winPoint) ;
    NVLdVRect           getAreaLogicalRectFromWindowRect(NS_CLASSLIB::TRect winRect) ;

    NS_CLASSLIB::TRect  getWorkingSpaceRect() ;
    int                 getWorkingSpaceHeight() ;
    int                 getWorkingSpaceRight() ;
    int                 getWorkingSpaceLeft() ;
    int                 getWorkingSpaceTop() ;
    int                 getWorkingSpaceBottom() ;

    int  getMaxConcernIndex()  { return _iMaxConcernIndex ; }
    int  getMaxGoalIndex()     { return _iMaxGoalsIndex ; }
    int  getMaxDrugsIndex()    { return _iMaxDrugsIndex ; }

    NSConcernView* getCurrentConcern() ;

    long getPosInfDroit() ;
    void increasePosInfDroit(int iDeltaPos) ;

    long preoccupDonneTopLine(int iIndex) ;
    long preoccupDonneBottomLine(int iIndex) ;
    long preoccupDonneTopBox(int iIndex) ;
    long preoccupDonneBottomBox(int iIndex) ;

    long drugDonneTopBox(int iIndex) ;
    long drugDonneBottomBox(int iIndex) ;
    long drugDonneTopLine(int iIndex) ;
    long drugDonneBottomLine(int iIndex) ;

    void setInterligne(int iIdx)      { _iInterligne = iIdx ;     }
    void setDrugInterligne(int iIdx)  { _iDrugInterLigne = iIdx ; }

    void setMaxConcernIndex(int iIdx) { _iMaxConcernIndex = iIdx ; }
    void setMaxGoalIndex(int iIdx)    { _iMaxGoalsIndex   = iIdx ; }
    void setMaxDrugsIndex(int iIdx)   { _iMaxDrugsIndex   = iIdx ; }

    void EvRButtonDown() ;
    void showContextHelp() ;
    void subSignalForHelp() ;
    void afterToonsInit() ;
    void setScrollParams() ;

    bool addSsObj(NSLdvSousObjet *pSsObj) ;
    bool addObj(NSLdvObjet *pObj) ;
    void removeObj(NSLdvObjet *pObj) ;
    bool addQualityOfLifeChart(NSQualityOfLifeChart *pCharts) ;
    void removeQualityOfLifeChart(NSQualityOfLifeChart *pCharts) ;
    bool addQualityOfLifeChart(NSQualityOfLifeChart *pCharts, NSConcern *pPb) ;
    void removeQualityOfLifeChart(NSQualityOfLifeChart *pCharts, NSConcern *pPb) ;
    bool addProb(NSConcern *pPb) ;

    bool addObjOnConcern(NSConcern *pPb, NSLdvObjet *pObj) ;

    int  getLigneProb(NSConcern* pConcern, int iLineBegin) ;

    NSLdvViewArea& operator=(const NSLdvViewArea& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    string    _sConcernReference ;

    bool      _bMainView ;

    int       _iLargeurLigne, _iLargeurSubLigne, _iLargeurBaseline ;
    int       _iInterligne, _iDrugInterLigne ;
    int       _iBottomMargin ;

    int       _iMaxConcernIndex ;
    int       _iMaxGoalsIndex ;
    int       _iMaxDrugsIndex ;

    // Rectangle contenant toute la ligne de vie (de la naissance
    //                                      au dernier élément plannifié)
    NVLdVRect _GlobalBox ;

    static long lObjectCount ;
};

// This is the window
//
class NSLdvViewAreaWindow : public TWindow, public NSRoot
{
  protected:

    NSLdvViewArea* _pToon ;

    bool          _bLButtonDown ;
    NSLdvToon     *_pToon2Drag ;
    long          _lPosInfDroit ;

    NSTitleTip*               _pToolTip ;
    NSLdvViewIconsShowWindow* _pMultiTips ;
    bool                      _bMustCloseMultitips ;
    bool                      _bShowTooltipsBeforeMultitips ;

    NSLdvViewMagnifier*       _pMagnifier ;
    bool                      _bMustCloseMagnifier ;

    static long lObjectCount ;

  public:

    NSLdvToon     *_pCurrentToon ;

    NSLdvToon     *_pHitToon ;
    hitToonStatus  _hitToonStatus ;

    NVLdVPoint    _currentPoint ;
    NSConcernView *_pCurrentConcern ;

    NSLdvViewAreaWindow(NSContexte* pCtx, TWindow* parent, NSLdvViewArea* pToon) ;
    ~NSLdvViewAreaWindow() ;

    void                SetupWindow() ;

    void                reinit() ;

    void                Paint(TDC&, bool, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void                EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void                EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void                EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void                EvSize(uint sizeType, ClassLib::TSize& size) ;
    void                EvSetFocus(THandle hWndLostFocus /* may be 0 */) ;
    void                EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void                EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    // void                EvTimer(uint timerId) ;
    void                EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl) ;
    LRESULT             EvMouseWheel(WPARAM wparam, LPARAM lparam) ;
    void                EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void                EvSysKeyDown(uint key, uint repeatCount, uint flags) ;
    void                CmHelp() ;

    // Overrides TWindows' in order to process our accelerators table
    bool                PreProcessMsg(MSG &msg) ;

    void                CmLdvZoomIn() ;
    void                CmLdvZoomOut() ;

    //
    // Concern related response functions
    //
    void                CmNewPreoccup() ;
    void                CmNewRisk() ;
    void                CmNewSocialConcern() ;

    void                CmChgPreoccup() ;
    void                CmChgPreoType() ;
    void                CmChgPreoDate() ;
    void                CmChgPreoIndx() ;
    void                CmAddObjectifs() ;
    void                CmPreoProject() ;
    void                CmPreoQualityOfLife() ;

    void                CmEvolPreoccup() ;
    void                CmEvolPreoType() ;
    void                CmEvolPreoIndx() ;

    void                CmSuppressPreo() ;

    void                CmManageRights() ;

    void                CmNewDrugsFromRefForConcern() ;
    void                CmNewGoalsFromRefForConcern() ;

    void                CmShowGoalsForConcern() ;
    void                CmShowDrugsForConcern() ;
    void                CmShowFollowUpForConcern() ;
    void                CmShowCurvesForConcern() ;

    //
    // Drug related response functions
    //
    void                CmDrugNew() ;
    void                CmDrugRenew() ;
    void                CmDrugModify() ;
    void                CmDrugChangePoso() ;
    void                CmDrugStop() ;
    void                CmDrugDelete() ;
    void                CmDrugPrescription() ;
    void                CmDrugRights() ;

    void                drawSpecialDates(TDC *pDc, NVLdVRect *rectARepeindre) ;

    void                showContextHelp() ;
    void                showMultitips(NSLdvObjetView* pObject, NSLdvToon* pFirstHitToon, NS_CLASSLIB::TPoint curPos) ;
    void                subSignalForHelp() ;
    void                actionsOnIconHalt(bool bIsSubSignal) ;

    NSLdvToon*          HitTest(NS_CLASSLIB::TPoint& point) ;
    void                HitTestAll(NS_CLASSLIB::TPoint& point, ArrayOfToons* pToons, int iToonZOrder = -1, bool bResetArray = true) ;
    NSLdvToon*          nextHitTest(NS_CLASSLIB::TPoint& point, NSLdvToon* pFormerToon) ;

    NSTitleTip*         getToolTip() { return _pToolTip ; }

    NSLdvViewArea*      getToon() { return _pToon ; }
    NSLdvView*          getView() { return _pToon->getView() ; }

    int                 getLargeurLigne()    { return _pToon->getLargeurLigne() ; }
    int                 getLargeurSubLigne() { return _pToon->getLargeurSubLigne() ; }
    int                 getLargeurBaseline() { return _pToon->getLargeurBaseline() ; }
    int                 getInterligne()      { return _pToon->getInterligne() ; }
    int                 getDrugInterligne()  { return _pToon->getDrugInterligne() ; }

    int                 getPixelPosAboveChronoline(int iYPos) ;
    int                 getPixelPosAboveBaseline(int iYPos) ;
    NS_CLASSLIB::TRect  getClientRectAboveChronoline() ;
    NS_CLASSLIB::TRect  getClientRectAboveBaseline() ;

    int                 getProccupGrossHeight() ;
    long                preoccupDonneTopBox(int iIndex) ;
    long                preoccupDonneBottomBox(int iIndex) ;
    long                preoccupDonneTopLine(int iIndex) ;
    long                preoccupDonneBottomLine(int iIndex) ;
    int			            getMaxConcernIndex()         { return _pToon->getMaxConcernIndex() ; }
    void		            setMaxConcernIndex(int iIdx) { _pToon->setMaxConcernIndex(iIdx) ; }

    long                getPosInfDroit()        { return _lPosInfDroit ; }
    void                increasePosInfDroit(int iDeltaPos) ;
    void                setPosInfDroit(long lNewPosition) ;

    int                 getWorkingSpaceTop() ;
    int                 getWorkingSpaceBottom() ;
    int                 getWorkingSpaceRight() ;
    int                 getWorkingSpaceLeft() ;
    NS_CLASSLIB::TRect  getWorkingSpaceRect() ;

    int                 getWorkingSpaceHeight() ;
    int                 getWorkingSpaceWidth() ;

    int                 getDrugBasePosition() ;
    int                 getDrugGrossHeight() ;
    long                drugDonneTopBox(int iIndex) ;
    long                drugDonneBottomBox(int iIndex) ;
    long                drugDonneTopLine(int iIndex) ;
    long                drugDonneBottomLine(int iIndex) ;
    int			            getMaxDrugsIndex()         { return _pToon->getMaxDrugsIndex() ; }
    void		            setMaxDrugsIndex(int iIdx) { _pToon->setMaxDrugsIndex(iIdx) ; }

    int                 getGoalBasePosition() ;
    int                 getGoalGrossHeight() ;
    long                goalDonneTopBox(int iIndex);
    long                goalDonneBottomBox(int iIndex);
    long                goalDonneTopLine(int iIndex);
    long                goalDonneBottomLine(int iIndex);
    int			            getMaxGoalIndex()         { return _pToon->getMaxGoalIndex() ; }
    void		            setMaxGoalIndex(int iIdx) { _pToon->setMaxGoalIndex(iIdx) ; }

    NS_CLASSLIB::TPoint getAreaPhysicalPoint(NVLdVPoint ldvPoint) ;
    NS_CLASSLIB::TRect  getAreaPhysicalRect(NVLdVRect ldvRect) ;
    NS_CLASSLIB::TRect  getAreaPhysicalRect(NS_CLASSLIB::TRect ldvWinRect) ;

    NS_CLASSLIB::TPoint getScrollablePhysicalPoint(NVLdVPoint ldvPoint) ;
    NS_CLASSLIB::TRect  getScrollablePhysicalRect(NVLdVRect ldvRect) ;

    void                adaptPhysicalPointToChronoLine(NS_CLASSLIB::TPoint &inboxPoint) ;
    int                 adjustPhysicalPointToChronoLine() ;

    NVLdVPoint          getAreaLogicalPointFromWindowPoint(NS_CLASSLIB::TPoint winPoint) ;
    NVLdVRect           getAreaLogicalRectFromWindowRect(NS_CLASSLIB::TRect winRect) ;

    void                setScrollParams() ;

    void                closeMultiTips() ;
    void                mustCloseMultiTips() { _bMustCloseMultitips = true ; }

    void                closeMagnifier() ;
    void                mustCloseMagnifier() { _bMustCloseMagnifier = true ; }

    NSHISTODocument*    getDocumentsManager() ;
    NSDocumentInfo*     getDocumentInfo(NSLdvObjetView* pLdvView) ;
    NSDocumentInfo*     getDocumentInfo(string sDocCode) ;

    void                closeOpenObject() ;
    void                setPosForActiveObject() ;
    void                setPosForActiveObjectWindow(TWindow* pMdiWindow) ;
    hitToonStatus       openObject(NSLdvObjetView *pObject) ;

    TWindow*            getDocViewWindow(NSNoyauDocument *pDocument) ;

    void                leftButtonIsDown()    { _bLButtonDown = true ; }
    void                leftButtonIsNotDown() { _bLButtonDown = false ; }
    bool                isLeftButtonDown()    { return _bLButtonDown ; }

    void                setToonToDrag(NSLdvToon *pTo) { _pToon2Drag = pTo ; }
    NSLdvToon*          getToonToDrag()               { return _pToon2Drag ; }

    void                startDragging(NSLdvToon *pTo, NS_CLASSLIB::TPoint& point, int iImageIndex) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  DECLARE_RESPONSE_TABLE(NSLdvViewAreaWindow) ;
};

inline long
NSLdvViewArea::getPosInfDroit()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getPosInfDroit() ;

  return long(0) ;
}

#endif // __NSLDVVUE_H

