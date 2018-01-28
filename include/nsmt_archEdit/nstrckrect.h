#if !defined(__NSTRSCRECT_H)#define __NSTRSCRECT_H

#define CX_BORDER   1
#define CY_BORDER   1

#include "partage\nsglobal.h"
#include "partage\ns_vector.h"
#include "nsbb\nsutidlg.h"

class NSRectTracker : public NSRoot
{
  public:

    // Constructors
	  NSRectTracker(NSContexte *pCtx) ;
	  NSRectTracker(NSContexte *pCtx, NS_CLASSLIB::TRect &rect, UINT nStyle) ;

    // Style Flags
	  enum StyleFlags
	  {
		  solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		  resizeInside = 8, resizeOutside = 16, hatchInside = 32,
	  };

    // Hit-Test codes
	  enum TrackerHit
	  {
		  hitNothing = -1,
		  hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		  hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	  };

    // Attributes
	  UINT               _nStyle ;  // current state
	  NS_CLASSLIB::TRect _rect ;    // current position (always in pixels)
	  NS_CLASSLIB::TSize _sizeMin ; // minimum X and Y size during track operation
	  int _nHandleSize ;  // size of resize handles (default from WIN.INI)

    // Operations
	  void         Draw(TDC* pDC) const ;
	  void         GetTrueRect(NS_CLASSLIB::TRect *pTrueRect) const ;
	  bool         SetCursor(TWindow* pWnd, UINT nHitTest) const ;
	  bool         Track(TWindow* pWnd, NS_CLASSLIB::TPoint point, bool bAllowInvert = false,
		                                               TWindow* pWndClipTo = NULL) ;
    bool         TrackRubberBand(TWindow* pWnd, NS_CLASSLIB::TPoint point, bool bAllowInvert = true) ;
    bool         TrackHandle(int nHandle, TWindow* pWnd, NS_CLASSLIB::TPoint point, TWindow* pWndClipTo) ;
	  int          HitTest(NS_CLASSLIB::TPoint point) const ;
    TrackerHit   HitTestHandles(NS_CLASSLIB::TPoint point) const ;
	  TrackerHit   NormalizeHit(TrackerHit nHandle) const ;

    // Overridables
	  virtual void DrawTrackerRect(NS_CLASSLIB::TRect *pRect, TWindow* pWndClipTo,
		                                                TDC* pDC, TWindow* pWnd) ;
    virtual void AdjustRect(int nHandle, NS_CLASSLIB::TRect *pRect) ;
	  virtual void OnChangedRect(const NS_CLASSLIB::TRect& rectOld) ;
	  virtual UINT GetHandleMask() const ;
    string       GetHandleName(TrackerHit nHandle) ;

  // Implementation
  public:

	  virtual ~NSRectTracker() ;

  protected:

	  BOOL _bAllowInvert ;    // flag passed to Track or TrackRubberBand
	  BOOL _bErase ;          // TRUE if DrawTrackerRect is called for erasing
	  BOOL _bFinalErase ;     // TRUE if DragTrackerRect called for final erase

    NS_CLASSLIB::TRect _rectLast ;
	  NS_CLASSLIB::TSize _sizeLast ;

	  // implementation helpers
	  // int HitTestHandles(NS_CLASSLIB::TPoint point) const ;
	  void        GetHandleRect(TrackerHit nHandle, NS_CLASSLIB::TRect* pHandleRect) const ;
	  void        GetModifyPointers(int nHandle, int**ppx, int**ppy, int* px, int*py) ;
	  virtual int GetHandleSize(NS_CLASSLIB::TRect* pRect = NULL) const ;
	  // bool TrackHandle(int nHandle, TWindow* pWnd, NS_CLASSLIB::TPoint point, TWindow* pWndClipTo) ;
	  void        Construct() ;
    void        DrawDragRect(TDC* pDC, NS_CLASSLIB::TRect* pRect, SIZE size,
		                   NS_CLASSLIB::TRect* pRectLast, SIZE sizeLast,
		                   HBRUSH hBrush = NULL, HBRUSH hBrushLast = NULL) ;
};

class NSRectTrackerWindow : public TWindow, public NSRoot
{
  public:

    // Constructors
	  NSRectTrackerWindow(NSContexte *pCtx, TWindow* parent) ;
	  NSRectTrackerWindow(NSContexte *pCtx, TWindow* parent, NS_CLASSLIB::TRect &rect, UINT nStyle) ;

    // Style Flags
	  enum StyleFlags
	  {
		  solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		  resizeInside = 8, resizeOutside = 16, hatchInside = 32,
	  };

    // Hit-Test codes
	  enum TrackerHit
	  {
		  hitNothing = -1,
		  hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		  hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	  };

    // Attributes
	  UINT               _nStyle ;  // current state
	  NS_CLASSLIB::TRect _rect ;    // current position (always in pixels)
	  NS_CLASSLIB::TSize _sizeMin ; // minimum X and Y size during track operation
	  int _nHandleSize ;  // size of resize handles (default from WIN.INI)

    // Operations
    void         SetupWindow() ;
    void         EvPaint() ;
    void         Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
	  void         Draw(TDC* pDC) const ;
	  void         GetTrueRect(NS_CLASSLIB::TRect *pTrueRect) const ;
	  bool         SetCursor(TWindow* pWnd, UINT nHitTest) const ;
	  bool         Track(TWindow* pWnd, NS_CLASSLIB::TPoint point, bool bAllowInvert = false,
		                                               TWindow* pWndClipTo = NULL) ;
    bool         TrackRubberBand(TWindow* pWnd, NS_CLASSLIB::TPoint point, bool bAllowInvert = true) ;
	  TrackerHit   HitTest(NS_CLASSLIB::TPoint point) const ;
	  TrackerHit   NormalizeHit(TrackerHit nHandle) const ;

    // Overridables
	  virtual void DrawTrackerRect(NS_CLASSLIB::TRect *pRect, TWindow* pWndClipTo,
		                                                TDC* pDC, TWindow* pWnd) ;
    virtual void AdjustRect(TrackerHit nHandle, NS_CLASSLIB::TRect *pRect) ;
	  virtual void OnChangedRect(const NS_CLASSLIB::TRect& rectOld) ;
	  virtual UINT GetHandleMask() const ;

  // Implementation
  public:

	  virtual ~NSRectTrackerWindow() ;

  protected:

	  BOOL _bAllowInvert ;    // flag passed to Track or TrackRubberBand
	  BOOL _bErase ;          // TRUE if DrawTrackerRect is called for erasing
	  BOOL _bFinalErase ;     // TRUE if DragTrackerRect called for final erase

    NS_CLASSLIB::TRect _rectLast ;
	  NS_CLASSLIB::TSize _sizeLast ;

	  // implementation helpers
	  TrackerHit  HitTestHandles(NS_CLASSLIB::TPoint point) const ;
	  void        GetHandleRect(TrackerHit nHandle, NS_CLASSLIB::TRect* pHandleRect) const ;
	  void        GetModifyPointers(TrackerHit nHandle, int**ppx, int**ppy, int* px, int*py) ;
	  virtual int GetHandleSize(NS_CLASSLIB::TRect* pRect = NULL) const ;
	  bool        TrackHandle(TrackerHit nHandle, TWindow* pWnd, NS_CLASSLIB::TPoint point, TWindow* pWndClipTo) ;
	  void        Construct() ;
    void        DrawDragRect(TDC* pDC, NS_CLASSLIB::TRect* pRect, SIZE size,
		                   NS_CLASSLIB::TRect* pRectLast, SIZE sizeLast,
		                   HBRUSH hBrush = NULL, HBRUSH hBrushLast = NULL) ;
    string      GetHandleName(TrackerHit nHandle) ;

  DECLARE_RESPONSE_TABLE(NSRectTrackerWindow) ;
};

#endif // __NSTRSCRECT_H

