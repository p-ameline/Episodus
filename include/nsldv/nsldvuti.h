//----------------------------------------------------------------------------
//   Programme protege par une licence Open Source GPL
//   Philippe Ameline
//----------------------------------------------------------------------------
#ifndef __NSLDVUTI_H
#define __NSLDVUTI_H

#ifndef __linux__
# include "nsldv/nsldvexport.h"
#endif

#ifndef __linux__
// #include <classlib/time.h>
  #ifndef _DANSNSLDVDLL
    #include <owl/window.h>
    #include <WinSys/geometry.h>
    #include <WinSys/color.h>
  #endif
  #include <classlib/time.h>
#else
  #include "classlib/time.h"
#endif

#define DATETIMESTRLEN 14
#define DATESTRLEN      8

/**
* Classe de gestion du temps dans tout le logiciel
* Time management class (to be used everywhere)
*/
#ifndef __linux__
class _NSLDVCLASSE NVLdVTemps
#else
class NVLdVTemps
#endif
{
	protected:

  	// AAAAMMJJhhmmss
    string _sTemps ;

    static long lObjectCount ;

	public:

  	NVLdVTemps() ;
    NVLdVTemps(int iA, int iM, int iJ, int ih, int im, int is) ;
    NVLdVTemps(const NVLdVTemps& rv) ;
    ~NVLdVTemps() ;

    void    init()       { _sTemps = string(DATETIMESTRLEN, '0') ; }
    void    setNoLimit() { _sTemps = string("9999") + string(DATETIMESTRLEN - 4, '0') ; }
    void    takeTime() ;

    bool    initFromDate(string sDate) ;
    bool    initFromDateHeure(string sDateHeure) ;
#ifndef __linux__
    bool    initFromSystemTime(LPSYSTEMTIME lpSystemTime) ;
#endif
    void		normalize() ;

    bool    estVide() const    { return ((string(DATETIMESTRLEN, '0') == _sTemps) ||
                                                    (string("") == _sTemps)) ; }
    bool    estNoLimit() const { return ( string("9999") + string(DATETIMESTRLEN - 4, '0') == _sTemps) ; }

    int     donneAns() const ;
    int     donneMois() const ;
    int     donneJours() const ;
    int     donneHeures() const ;
    int     donneMinutes() const ;
    int     donneSecondes() const ;

    string  donneDateHeure() const { return _sTemps ; }
    string  donneDate()      const { return string(_sTemps, 0, 8) ; }
    string  donneHeure()     const { return string(_sTemps, 8, 6) ; }
    string  donneFormattedDateHeure(NSContexte* pContexte, string sLang) const ;
    string  donneFormattedDate(NSContexte* pContexte, string sLang) const ;
    string  donneFormattedHeure(NSContexte* pContexte, string sLang) const ;

    string  getIso8601() ;

    void    placeAns(int iA) ;
    void    placeMois(int iM) ;
    void    placeJours(int iJ) ;
    void    placeHeures(int ih) ;
    void    placeMinutes(int im) ;
    void    placeSecondes(int is) ;

    // Fonctions de gestion du temps
    void    ajouteAns(int iDeltaT, bool bAdjust = true) ;
    void    ajouteMois(int iDeltaT, bool bAdjust = true) ;
    void    ajouteJours(int iDeltaT) ;
    void    ajouteHeures(long iDeltaT) ;
    void    ajouteMinutes(long iDeltaT) ;
    void    ajouteSecondes(long iDeltaT) ;

    void    ajouteTemps(int iDeltaT, string sUnite, NSContexte* pCtx) ;
    unsigned long yearsBetween(const NVLdVTemps& otherDate)   const ;
    unsigned long monthsBetween(const NVLdVTemps& otherDate)  const ;
    unsigned long daysBetween(const NVLdVTemps& otherDate)    const ;
    unsigned long hoursBetween(const NVLdVTemps& otherDate)   const ;
    unsigned long minutesBetween(const NVLdVTemps& otherDate) const ;
    unsigned long secondsBetween(const NVLdVTemps& otherDate) const ;

    struct tm   *convert2tm() ;
    void    convert(struct tm *tm2Convert) ;
    void    convert(ClassLib::TTime tt2Convert) ;
    void    convert(ClassLib::TDate *tt2Convert) ;

    string  donneMonthName() const ;

    NVLdVTemps& operator=(const NVLdVTemps& src) ;

    bool operator==(const NVLdVTemps& src) const { return _sTemps == src._sTemps ; }
    bool operator>(const NVLdVTemps& src)  const { return _sTemps >  src._sTemps ; }
    bool operator>=(const NVLdVTemps& src) const { return _sTemps >= src._sTemps ; }
    bool operator<(const NVLdVTemps& src)  const { return _sTemps <  src._sTemps ; }
    bool operator<=(const NVLdVTemps& src) const { return _sTemps <= src._sTemps ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

// definition d'un array de pixels

typedef vector<NVLdVTemps*> TimeVector ;
typedef TimeVector::iterator       TimeIter ;
typedef TimeVector::const_iterator TimeConstIter ;

#ifndef __linux__
class _NSLDVCLASSE TimeArray : public TimeVector
#else
class TimeArray : public TimeVector
#endif
{
	public:

  	// Constructeurs - Destructeur
    TimeArray() ;
    TimeArray(const TimeArray& rv) ;
    ~TimeArray() ;

    void sortDates(bool bRecentOnTop) ;

    void vider() ;

    TimeArray& operator=(const TimeArray& src) ;
};

#ifndef _ENTERPRISE_DLL

class NSLdvViewBase ;
class NSLdvViewArea ;

#ifndef __linux__
class _NSLDVCLASSE NVLdVPoint
#else
class NVLdVPoint
#endif
{
	protected:

  	NVLdVTemps  _tX ;
    long        _dY ;

    NSLdvViewBase*  _pView ;

    static long lObjectCount ;

	public:

  	NVLdVPoint(NSLdvViewBase* pTheView) ;
    NVLdVPoint(const NVLdVPoint& rv) ;
    ~NVLdVPoint();

    void initialise(NS_CLASSLIB::TPoint* pOrigine, NS_CLASSLIB::TPoint* pPt, long lWorkingAreaPosInfDroit) ;

    NVLdVPoint& operator=(const NVLdVPoint& src) ;

    NVLdVTemps  getX()  { return _tX ; }
    long        getY()  { return _dY ; }

    void        setX(NVLdVTemps tTemps) ;
    void        setY(long lHaut)        { _dY = lHaut ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

#ifndef __linux__
class _NSLDVCLASSE NVLdVRect
#else
class NVLdVRect
#endif
{
	protected:

  	NVLdVTemps  tLeft ;
    long        lTop ;
    NVLdVTemps  tRight ;
    long        lBottom ;

    NSLdvViewBase* pView ;

    static long lObjectCount ;

	public:

  	NVLdVRect(NSLdvViewBase* pTheView) ;
    NVLdVRect(NSLdvViewBase* pTheView, NVLdVTemps tDeb, NVLdVTemps tFin,
                                            long lPlancher, long lPlafond) ;
    NVLdVRect(NSLdvViewBase* pTheView, NVLdVPoint* upLeft, NVLdVPoint* loRight) ;
    NVLdVRect(const NVLdVRect& rv) ;
    ~NVLdVRect() ;

    void initialise(NS_CLASSLIB::TPoint* pOrigine, NS_CLASSLIB::TRect* pRect, long lWorkingAreaPosInfDroit) ;
    void initialise(NVLdVTemps tDeb, NVLdVTemps tFin,
                                            long lPlancher, long lPlafond) ;

    NVLdVRect& operator=(const NVLdVRect& src) ;

    void setLeft(NVLdVTemps tps)    { tLeft   = tps ; }
    void setRight(NVLdVTemps tps)   { tRight  = tps ; }
    void setTop(long lv)            { lTop    = lv ; }
    void setBottom(long lv)         { lBottom = lv ; }

    NVLdVTemps getLeft()    { return tLeft ;    }
    NVLdVTemps getRight()   { return tRight ;   }
    long       getTop()     { return lTop ;     }
    long       getBottom()  { return lBottom ;  }

    NVLdVPoint BottomLeft() ;
    NVLdVPoint BottomRight() ;
    NVLdVPoint TopLeft() ;
    NVLdVPoint TopRight() ;

    unsigned long Width() ;     // Width in pixels

    long       Height()     { return lBottom - lTop ; }

    bool Contains(NVLdVPoint point) ;  // Contient other
    bool Contains(NVLdVRect other) ;
    bool Touches(NVLdVRect other) ;    // A une intersection avec other

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

#ifndef _EXT_CAPTURE

#ifndef __linux__
class _NSLDVCLASSE NSPixel
{
	protected:

  	NS_CLASSLIB::TPoint point ;
    NS_CLASSLIB::TColor color ;

    static long lObjectCount ;

	public:

  	NSPixel(NS_CLASSLIB::TPoint Point, NS_CLASSLIB::TColor Color) ;
    NSPixel(int X, int Y, NS_CLASSLIB::TColor Color) ;
    NSPixel(const NSPixel& rv) ;
    ~NSPixel() ;

    NSPixel& operator=(const NSPixel& src) ;

    NS_CLASSLIB::TPoint getPoint()  { return point ; }
    int                 X()         { return point.X() ; }
    int                 Y()         { return point.Y() ; }
    NS_CLASSLIB::TColor getColor()  { return color ; }

    char*               getTuple() ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

// definition d'un array de pixels

typedef vector<NSPixel*>         PixArray ;
typedef PixArray::iterator       pixelIter ;
typedef PixArray::const_iterator pixelConstIter ;

class _NSLDVCLASSE PixelArray : public PixArray
{
	public:

  	// Constructeurs - Destructeur
    PixelArray() ;
    PixelArray(const PixelArray& rv) ;
    ~PixelArray() ;

    NS_CLASSLIB::TRect ArrayRect ;

    bool ScreenCapture(TWindowDC* pWinDC, ClassLib::TRect* pSrcRect) ;

    bool Affiche(TDC* pDestDC, NS_CLASSLIB::TRect* pDstRect, NS_CLASSLIB::TPoint* pSrcPt) ;

    void ajouter(NSPixel* pNewPix) ;

    NS_CLASSLIB::TColor getColor(NS_CLASSLIB::TPoint pt, bool* pExist) ;

    int getBottom() { return ArrayRect.Bottom() ; }
    int getTop()    { return ArrayRect.Top() ;    }
    int getLeft()   { return ArrayRect.Left() ;   }
    int getRight()  { return ArrayRect.Right() ;  }

    void vider() ;
		PixelArray& operator=(const PixelArray& src) ;
};

class NSOCRbloc;

class _NSLDVCLASSE NSBitmap
{
	public:

  	// Constructeurs - Destructeur
    NSBitmap() ;
    NSBitmap(const NSBitmap& rv) ;
    ~NSBitmap() ;

    bool ScreenCapture(TWindowDC* pWinDC, ClassLib::TRect* pSrcRect) ;
    bool Affiche(TDC* pDestDC, NS_CLASSLIB::TRect* pDstRect, NS_CLASSLIB::TPoint* pSrcPt, int iZoomFactor = 1) ;

    bool trouveBloc(NSOCRbloc* pBloc, NS_CLASSLIB::TPoint* pPoint) ;

    bool enableCapture(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor) ;
    void nearBlank(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor, bool *pLeft, bool *pTop, bool *pRight, bool *pBottom) ;
    void existEdges(ClassLib::TRect* pRect, NS_CLASSLIB::TColor* pBgColor, bool *pLeft, bool *pTop, bool *pRight, bool *pBottom) ;

    NS_CLASSLIB::TColor getColor(int X, int Y, bool* pExist) ;
    NS_CLASSLIB::TColor getColor(NS_CLASSLIB::TPoint pt, bool* pExist) ;

    int getBottom() { return pBitmap->Height() ; }
    int getTop()    { return 0 ; }
    int getLeft()   { return 0 ; }
    int getRight()  { return pBitmap->Width() ; }

    TDib* getDib()  { return pBitmap ; }

    NSBitmap& operator=(const NSBitmap& src) ;

	protected:

  	TDib* pBitmap ;
};

class _NSLDVCLASSE NSOCRbloc
{
	public:

  	NS_CLASSLIB::TRect  BlocRect ;
    NS_CLASSLIB::TRect  BlocOCR ;
    string              sTexte ;
    NS_CLASSLIB::TColor bgColor ;
    NS_CLASSLIB::TColor fgColor ;

    NSOCRbloc(NS_CLASSLIB::TRect Bloc) ;
    NSOCRbloc(NS_CLASSLIB::TRect Bloc,  NS_CLASSLIB::TColor bColor,
            NS_CLASSLIB::TColor fColor = NS_CLASSLIB::TColor::Black) ;
    NSOCRbloc(const NSOCRbloc& rv) ;
    ~NSOCRbloc() ;

    NSOCRbloc& operator=(const NSOCRbloc& src) ;
};

// definition d'un array de modeles
typedef vector<NSOCRbloc*>       OCRArray ;
typedef OCRArray::iterator       blocIter ;
typedef OCRArray::const_iterator blocConstIter ;

class _NSLDVCLASSE OCRBlocsArray : public OCRArray
{
	public:

  	// Constructeurs - Destructeur
    OCRBlocsArray() : OCRArray() {}
    OCRBlocsArray(const OCRBlocsArray& rv) ;
    ~OCRBlocsArray() ;

    void    vider() ;
    OCRBlocsArray& operator=(const OCRBlocsArray& src) ;
};

#endif       // #ifndef __linux__

#endif     // #ifndef _EXT_CAPTURE

#endif   // #ifndef _ENTERPRISE_DLL

#endif // __NSLDVUTI_H

