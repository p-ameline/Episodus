//----------------------------------------------------------------------------
//   Programme protégé par une licence Open Source GPL
//   NAUTILUS - www.nautilus-info.com
//----------------------------------------------------------------------------
#ifndef __NSLDVTOONS_H
# define __NSLDVTOONS_H

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
# include "nsldv\nsldvdoc_base.h"
# include "nsldv\nsldvuti.h"
# include "nsldv\nsldvvar.h"

# include "nsldv\nsldvgoal.h"
# include "partage\ns_timer.h"

# include "nsldv/nsldvexport.h"

// Z-order des objets

// Repères des grandes classes de composants sur l'axe des Z
// Absolute Z-orders for main component classes
# define LEVEL_BACKGROUND    0   // Ligne de base - base line
# define LEVEL_PREOCCUP     10   // Préoccupations - health issues
# define LEVEL_OBJECTS      15   // Objets - Objects
# define LEVEL_CURVES       20   // Curves
# define LEVEL_BASELINE     25   // Ligne de base - base line
# define LEVEL_BASE_OBJECTS 30   // Objets - Objects
# define LEVEL_COMMANDS     35   // Command objets

// Niveaux relatifs des composants de préoccupations de santé
// Relative Z-orders for components of "health issues" category
# define RELPRLEVEL_LINE     0   // Ligne - line
# define RELPRLEVEL_PROBLEM  1   // Problème - problem
# define RELPRLEVEL_PBEVENT  2   // Evénément de problème - problem event
# define RELPRLEVEL_GOAL     3   // Objectifs de santé - Health goals

// Niveaux relatifs des composants d'objets
// Relative Z-orders for components of "object" category
# define RELOBLEVEL_COOP     0   // Objet - object
# define RELOBLEVEL_OBJECT   1   // Objet - object
# define RELOBLEVEL_SSOBJT   2   // Sous-Objet - object part

# define TIMER_VALUE 200

class ldvBrush : public OWL::TGdiObject
{
 public:

	typedef HBRUSH THandle ;  // TBrush encapsulates an HBRUSH

	ldvBrush(const NS_CLASSLIB::TColor& color) ;
  ldvBrush(const NS_CLASSLIB::TColor& color, int iStyle) ;

	// Type Accessor & Conversion Operator
	HBRUSH   GetHandle() const ;
	operator HBRUSH() const ;

	// Get GDI Brush Object information
	bool        GetObject(LOGBRUSH far& logBrush) const ;
};

// Return the handle of the brush with type HBRUSH.
inline HBRUSH ldvBrush::GetHandle() const
{
	return HBRUSH(GetGdiHandle()) ;
}

// Return the handle of the brush with type HBRUSH.
inline ldvBrush::operator HBRUSH() const
{
  return GetHandle() ;
}

// Retrieve the attributes of the brush.
inline bool ldvBrush::GetObject(LOGBRUSH far& logBrush) const
{
	return TGdiObject::GetObject(sizeof(logBrush), &logBrush) != 0 ;
}

class NSLdvView;

class _NSLDVCLASSE NSLdvToon : public NSRoot
{
  private:

    static long lCoreObjectCount ;

 	public:

    enum TOONTYPE { toonLine = 0, toonConcern, toonRisk, toonSocial,
                    toonConcernEvent, toonObject, toonObjectPart, toonCoop,
                    toonGoal, toonDrug, toonValues, toonPannel, toonButton,
                    toonChronoLine, toonYAxis, toonCurvePannel, toonCurvePoint,
                    toonBackground, toonVerticalScroll, toonWorkingArea } ;
    TOONTYPE	   _toonType ;

    enum TOONCATEGORY { timeToon = 0, windowToon } ;
    TOONCATEGORY _toonCategory ;

    string       _sLibelle ;

    int          _iZOrder ;

    bool         _bVisible ;
    bool         _bInterceptRButtonDown ;

    // NS_CLASSLIB::TRect rectangle;

    NSLdvViewBase* _pView ;

    NSLdvToon(NSContexte* pCtx, NSLdvViewBase* pTheView) ;
    NSLdvToon(const NSLdvToon& rv) ;
    virtual ~NSLdvToon() { lCoreObjectCount-- ; }

    void			  	             initialiser(const NSLdvToon* pSrc) ;

    virtual NS_CLASSLIB::TRect donneRectangle() = 0 ;
    virtual NS_CLASSLIB::TRect donneVisibleRectangle() = 0 ;

    virtual void               SetupWindow() = 0 ;
    virtual void       	       draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal) = 0 ;

    virtual void       	       EvRButtonDown() = 0 ;

    virtual void               setVisible() { _bVisible = true ; }
    virtual void               setHidden()  { _bVisible = false ; }

    NSLdvViewBase*             getView()     { return _pView ; }
    TOONTYPE                   getType()     { return _toonType ; }
    TOONCATEGORY               getCategory() { return _toonCategory ; }

    void                       setView(NSLdvViewBase* pV)   { _pView        = pV ; }
    void                       setType(TOONTYPE ty)         { _toonType     = ty ; }
    void                       setCategory(TOONCATEGORY ca) { _toonCategory = ca ; }

    NSLdvToon& 			           operator=(const NSLdvToon& src) ;

    static long getNbInstance()  { return lCoreObjectCount ; }
    static void initNbInstance() { lCoreObjectCount = 0 ; }
};

// définition d'un array de toons
typedef vector<NSLdvToon*> ArrayLdvToon;
typedef ArrayLdvToon::iterator ToonsIter;

class NSLigneView ;
class NSLdvViewArea ;

class _NSLDVCLASSE ArrayOfToons : public ArrayLdvToon
{
	public:

    // Constructeurs - Destructeur
    ArrayOfToons() : ArrayLdvToon() {}
    // ArrayOfToons(ArrayOfToons& rv);
    ~ArrayOfToons() ;

    void vider() ;
    void viderType(NSLdvToon::TOONTYPE toonTypeToKill) ;

    void init(NSFrameInformationArray *pFrames, NSContexte* pCtx, NSLdvViewArea* pWrk) ;

    void initGoals(NSFrameInformationArray *pFrames, NSContexte* pCtx, NSLdvViewArea* pWrk) ;

    void initTreatments(NSFrameInformationArray *pFrames, NSContexte* pCtx, NSLdvViewArea* pWrk) ;
    void init(LDVFRAME iFrame, ArrayLdvDrugs  *pDrugs,    NSContexte* pCtx, NSLdvViewArea* pWrk) ;
    void reinitDrugs(NSFrameInformationArray *pFrames,   NSContexte* pCtx, NSLdvViewArea* pWrk) ;

    void init(NSQualityOfLifeChartArray *pCharts, NSContexte* pCtx, NSLdvViewArea* pWrk) ;
    void init(ArrayObjets  *pObjets, NSContexte* pCtx, NSLdvViewArea* pWrk) ;
    void init(ArraySsObjet *pSsObj,  NSContexte* pCtx, NSLdvViewArea* pWrk) ;

    NSLigneView    *getLigneView(string sToon) ;
    NSLigneView    *getBaseLine() ;

    NSLdvObjetView *getObjView(string sToon) ;
    NSConcernView  *getPbView(string sToon) ;

    // ArrayOfToons& operator=(ArrayOfToons src);
};

#endif // __NSLDVTOONS_H

