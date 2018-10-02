// --------------------------------------------------------------------------// NSEPISODVIEW.H		Document/Vues Capture Données Utilisateur
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Aout 2001
// source : Rémi SPAAK - Mai 1997
// --------------------------------------------------------------------------

#if !defined(__NSCAPTVIEW_H)
#define __NSCAPTVIEW_H

#include <owl\docmanag.h>
#include <owl\docview.h>
#include <owl\mdichild.h>
#include <owl\decframe.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\applicat.h>
#include <owl\richedpr.h>
#include <owl\choosefo.h>
#include <owl\listwind.h>

//#include "nautilus\nsepicap.h"

#include "nsepisod\eptables.h"
#include "nsepisod\nsclasser.h"

#include "nssavoir\nspatho.h"
#include "nssavoir\nspathor.h"

#include "nautilus\nsdocref.h"
#include "nautilus\nautilus.rh"
#include "nautilus\richedap.h"
#include "dcodeur\nscr_anx.h"
#include "dcodeur\nstxstar.h"
#include "nsbb\nsednum.h"
#include "partage\NTArray.h"

class  NSCaptDocument : public TDocument, public NSRoot
{
	public:

		NSEpisodus* pEpisodus ;
		// string      sTexte;
		// FlexArray   aFlex;
		AnaCaptArray aCaptures ;

		NSCaptDocument(TDocument *parent, NSContexte *pCtx = 0) ;

		~NSCaptDocument();

		void    PatChanged() ;

		void    newCapture(NSCaptureArray* pCaptArray, string sTxt, string sCaptPath = "", string sClassif = "", string sUnite = "") ;
		void    newCapture(NSCaptureArray* pCaptArray, NSOCRbloc* pOcrBloc, string sCaptPath = "", string sClassif = "", string sUnite = "") ;
    void    newCapture(NSCaptureArray* pCaptArray, NSCapture* pCapture) ;
    void    processNewCapture(NSCapture* pCapt, analysedCapture* pPrimeAnalizedCapt, analysedCapture* pSecAnalizedCapt) ;
		string  getText(size_t iIndice) ;
    void    refreshViewsNewLigne() ;

    void    openView() ;

		// Méthodes virtuelles de TDocument
		bool    Open(int mode, const char far *path = 0)   { return true ; }
		bool    Close()                                    { return true ; }

		bool    IsOpen()                                   { return true ; }
};
class NSCaptView : public TView, public OWL::TEdit{
	public:

		// Constructeur - Destructeur
    NSCaptView(NSCaptDocument& doc, TWindow *parent = 0) ;
    ~NSCaptView() ;

    // Redéfiniton des fonctions virtuelles de TView
    static LPCSTR   StaticName()    { return "NSCaptView" ; }
    const char      *GetViewName()  { return StaticName() ; }
    TWindow         *GetWindow() ;
    // bool            CanClose();           // de TEditFile
    // bool         CanClear();           // de TEditFile
    bool            SetDocTitle(const char far* docname, int index)
    {
    	return OWL::TView::SetDocTitle(docname, index) ;
    }

    NSCaptDocument* getDoc()    { return pDoc ; }

    void            EvDestroy() ;    void            SetWindowPosit() ;    void            RecordWindowPosit() ;

	protected:

		// Remplissage de la vue
    void 	FillViewData() ;
    void 	SetupWindow() ;
    void  SetupToolBar() ;
    void  SetupTxtBar() ;

    // view event handlers
    bool 	VnIsWindow(HWND hWnd) ;

    void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void    EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void    EvChar(uint key, uint repeatCount, uint flags) ;
    void    EvSetFocus(HWND hWndLostFocus) ;

    void    EvPosit(int iPosit) ;
    void    EvPosit1() { EvPosit(1) ; }
    void    EvPosit2() { EvPosit(2) ; }
    void    EvPosit3() { EvPosit(3) ; }
    void    EvPosit4() { EvPosit(4) ; }
    void    EvPosit5() { EvPosit(5) ; }
    void    EvPosit6() { EvPosit(6) ; }
    void    EvPosit7() { EvPosit(7) ; }
    void    EvPosit8() { EvPosit(8) ; }
    void    EvPosit9() { EvPosit(9) ; }
    void    EvCode() ;

    // Variables
    NSCaptDocument* pDoc ;	            // Pointeur sur document soap

    string          sSelection ;
    bool	          bSetupToolBar ;

	DECLARE_RESPONSE_TABLE(NSCaptView) ;
};

// --------------------------------------------------------------------------
//
//      Vue multi-captures / Multi-captures view
//
// --------------------------------------------------------------------------

//
// Un analysedCapture peut être multi-lignes (c.à.d contenir des cr/lf),
// une NSCaptLine correspond à une (des) ligne(s) d'une analysedCapture
//
// An analysedCapture may be multi-lines (it means it can contain cr/lf),
// a NSCaptLine is one (among the) line(s) of an analysedCapture
//
class NSCaptLine
{
    public:

        analysedCapture*    pCapturedElement ;
        size_t              iStartPosInElement, iEndPosInElement ;

        enum CAPTSTATUS { StatusInit = 1, StatusCaptured, StatusNotFound, StatusSeparator };
        CAPTSTATUS          iStatus ;

        NSLigne*            pTextLine ;

        NSCaptLine() ;
        NSCaptLine(const NSCaptLine& rv) ;
        ~NSCaptLine() ;

        string              getText() ;
        size_t              getTextLen() ;
        void                setText(string sText) ;

        NS_CLASSLIB::TRect  getBox() const ;

	    NSCaptLine& operator=(const NSCaptLine& src) ;
};

typedef vector<NSCaptLine*> CaptLineVector ;
typedef CaptLineVector::iterator       CaptLineIter ;
typedef CaptLineVector::const_iterator CaptLineConstIter ;

class CaptLineArray : public CaptLineVector
{
  public:

    // Constructeurs - Destructeur
    CaptLineArray() : CaptLineVector() {}
    CaptLineArray(const CaptLineArray& rv) ;
    ~CaptLineArray() ;

    void vider() ;

    CaptLineIter    getLine(size_t iNumLine) ;

    CaptLineArray& operator=(const CaptLineArray& src) ;
};

struct CAPTHIT
{
	public:

		int    getLigne()  { return iLigne ; }
    size_t getPos()    { return iPos ; }
    bool   getInside() { return bInside ; }

		void   setLigne(int iLig)  { iLigne  = iLig ; }
    void   setPos(size_t iPo)  { iPos    = iPo ; }
    void   setInside(bool bIn) { bInside = bIn ; }

    void   nextLigne() { iLigne++ ; }
		void   precLigne() { iLigne-- ; }

    void   nextPos()   { iPos++ ; }
		void   precPos()   { iPos-- ; }
		void   resetPos()  { iPos = 0 ; }

		CAPTHIT& operator=(const CAPTHIT& src) ;
    int 		 operator==(const CAPTHIT& o) ;
    bool 		 operator>(const CAPTHIT& o) ;
    bool 		 operator<(const CAPTHIT& o) ;
    bool 		 operator>=(const CAPTHIT& o) ;
    bool 		 operator<=(const CAPTHIT& o) ;

	protected:

  	bool    bInside ;

		int     iLigne ;
		size_t  iPos ;
} ;

inline CAPTHIT&
CAPTHIT::operator=(const CAPTHIT& src)
{
  if (this == &src)
		return *this ;

	iLigne = src.iLigne ;
	iPos   = src.iPos ;

	return *this ;
}

inline int
CAPTHIT::operator==(const CAPTHIT& o)
{
	if ((iLigne == o.iLigne) &&
	    (iPos   == o.iPos))
		return 1 ;

	return 0 ;
}

inline bool
CAPTHIT::operator>(const CAPTHIT& o)
{
	if (iLigne > o.iLigne)
		return 1 ;

	if ((iLigne == o.iLigne) && (iPos > o.iPos))
		return 1 ;

	return 0 ;
}

inline bool
CAPTHIT::operator<(const CAPTHIT& o)
{
	if (iLigne < o.iLigne)
		return 1 ;

	if ((iLigne == o.iLigne) && (iPos < o.iPos))
		return 1 ;

	return 0 ;
}

inline bool
CAPTHIT::operator>=(const CAPTHIT& o)
{
	return ((*this == o) || (*this > o)) ;
}

inline bool
CAPTHIT::operator<=(const CAPTHIT& o)
{
	return ((*this == o) || (*this < o)) ;
}

class NSMultiCaptView : public NSMUEView
{	public:

		CaptLineArray	aLignes ;

		// Constructeur - Destructeur
		NSMultiCaptView(NSCaptDocument& doc, TWindow *parent = 0, string sConcern = "") ;
		~NSMultiCaptView() ;

		void            addedLines() ;

		void            concernChanged(string sConcern) ;

    void CmOk() {}
    void CmCancel() {}

		// Redéfiniton des fonctions virtuelles de TView et TRichEdit
		static LPCSTR   StaticName() { return "NSMultiCaptView" ; }
		//const char      *GetViewName() {return StaticName();}
		const char      *GetViewName() { return sViewName.c_str(); }
		TWindow         *GetWindow();
		bool            SetDocTitle(const char far* docname, int index)
		{
			return OWL::TWindow::SetDocTitle(docname, index) ;
		}
		void            setViewName() ;

		void            SetupColumns() ;
		void            AfficheListe() ;
		void            EvSetFocus(HWND hWndLostFocus) ;
		void            EvKillFocus(HWND hWndGetFocus) ;

		string          donneContexte(string sContexte) ;
		void            initCurentEpiRC() ;

		void            reloadView(string sReason) ;

		NSCaptDocument*  getDoc() { return pCaptDoc ; }

	protected:

		NS_CLASSLIB::TPoint	    HautGcheFenetre ;
		UINT				            PremLigne ;		   // numéro de la ligne du haut
		LONG				            LargeurPolice ;
		NS_CLASSLIB::TRect	    RectangleGlobal ;
		NSFontArray             StylesPolice ;
		NSStyleParagrapheArray  StylesParagraphe ;

		bool                    bSelected ;
    CAPTHIT                 firstSelPoint ;
    CAPTHIT                 lastSelPoint ;

//		UINT                    iFirstSelLine, iLastSelLine ;
//		size_t                  iFirstSelPos, iLastSelPos ;
    bool                    bMouseSelecting ;
    CAPTHIT                 selectingRefPoint ;

		bool                    bDragDrop ;

		// Caret is displayed BEFORE the captCaret char
    // For example if iPos == 0 it means that the caret is before the first char
		CAPTHIT                 captCaret ;     // iPos == 0 : AVANT premier char

		// Variables
		NSCaptDocument          *pCaptDoc ;	// Pointeur sur document EPISOD		OWL::TControlBar        *pToolBar ;	        // Barre d'outils
		bool	                  bSetupToolBar ;

		// Remplissage de la vue
		void 	              SetupWindow() ;

		void                initParams() ;
		void                reInitParams() ;
		void                initLines() ;

		void                convertLine(AnaCaptIter cptIt) ;
		void                initHistory() ;
		void                InitialiseLignes() ;
		bool                getFont(NSCaptLine::CAPTSTATUS iStatus, NSFont* pFont) ;

		void                Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

		void                SetupTxtBar() ;
		void                InitFormatFont() ;
		void                SetupToolBar() ;

		size_t              getMaxLine() ;

		// view event handlers
		bool 	              VnIsWindow(HWND hWnd) ;

		CAPTHIT             HitTest(NS_CLASSLIB::TPoint& point, bool* pCaretBefore = 0) ;
		bool                inSelectedArea(CAPTHIT hitPoint) ;
		void                setCaretPosition() ;
    void                updateSelection() ;
    void                selecting() ;

		string              getSelText() ;
		analysedCapture*    getSelCapture() ;

		void                selectWord(CAPTHIT hitPoint) ;
		void                selectSentence(CAPTHIT hitPoint) ;
		void                selectParagraph(CAPTHIT hitPoint) ;
		void                selectCaptureSet(CAPTHIT hitPoint) ;
		void                selectAll(CAPTHIT hitPoint) ;

    CAPTHIT             previousPoint(CAPTHIT refPoint) ;
    CAPTHIT             nextPoint(CAPTHIT refPoint) ;
    CAPTHIT             firstLinePoint(CAPTHIT refPoint) ;
    CAPTHIT             lastLinePoint(CAPTHIT refPoint) ;
    CAPTHIT             upPoint(CAPTHIT refPoint) ;
    CAPTHIT             downPoint(CAPTHIT refPoint) ;

		// Fonctions de réponse aux evts
		void                EvSize(uint sizeType, ClassLib::TSize &size) ;
		void                EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
		void                EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
		void                EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point) ;
		void                EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;

		void                EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
		void                EvChar(uint key, uint repeatCount, uint flags) ;
		void                EvKeyDown(uint key, uint repeatCount, uint flags) ;

		void                EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl) ;
		void                EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl) ;
		LRESULT             EvMouseWheel(WPARAM wparam, LPARAM lparam) ;

		void                EvPosit(int iPosit) ;
		void                EvPosit1() { EvPosit(1) ; }
		void                EvPosit2() { EvPosit(2) ; }
		void                EvPosit3() { EvPosit(3) ; }
		void                EvPosit4() { EvPosit(4) ; }
		void                EvPosit5() { EvPosit(5) ; }
		void                EvPosit6() { EvPosit(6) ; }
		void                EvPosit7() { EvPosit(7) ; }
		void                EvPosit8() { EvPosit(8) ; }
		void                EvPosit9() { EvPosit(9) ; }
		void                EvCode() ;
		void                EvOcrLearn() ;

    NS_CLASSLIB::TPoint screenPointToCaptPoint(NS_CLASSLIB::TPoint& point, TIC* pDC = 0) ;
    NS_CLASSLIB::TPoint captPointToScreenPoint(NS_CLASSLIB::TPoint& point, TIC* pDC = 0) ;

	DECLARE_RESPONSE_TABLE(NSMultiCaptView) ;
};

#endif		// __NS_SOAPVIEW_H

