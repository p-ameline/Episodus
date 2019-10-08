// --------------------------------------------------------------------------// NSRCVIEW.H		Document/Vues Tableau de bord des RC
// --------------------------------------------------------------------------
// PA - Octobre 2003
// --------------------------------------------------------------------------
#if !defined(__NSRCVIEW_H)
#define __NSRCVIEW_H

#include <owl\docmanag.h>#include <owl\docview.h>
#include <owl\mdichild.h>
#include <owl\decframe.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\applicat.h>
#include <owl\richedpr.h>
#include <owl\choosefo.h>
#include <owl\listwind.h>

#include "nautilus\nsepicap.h"

#include "nautilus\nsdocref.h"
#include "nautilus\nautilus.rh"
#include "nsbb\nsmview.h"
#include "nautilus\nsldvdoc.h"
#include "nsldv\nsldvvar.h"
#include "dcodeur\nscr_anx.h"
#include "dcodeur\nstxstar.h"
#include "nsbb\nsarc.h"

#define MAXCARS 100000      // Limite du texte pour RichEdit

#define _NSDRUGVIEWCLASS _USERCLASS

class NSRCEpisod
{
    public:

        NSConcern*  pConcern ;

        string      sPosDiag ;
        string      sLastDoc ;

        NSRCEpisod() ;
        NSRCEpisod(const NSRCEpisod& rv) ;
        ~NSRCEpisod() ;

        NSRCEpisod& operator=(const NSRCEpisod& src) ;
} ;

class NSEpisodRCView ;

// définition d'un array représentant la fenêtre de capture
typedef vector<NSRCEpisod*>          ArrayRCEpiso ;
typedef ArrayRCEpiso::iterator       ArrayRCEpisodIter ;
typedef ArrayRCEpiso::const_iterator ArrayRCEpisodConstIter ;

class ArrayRCEpisodes : public ArrayRCEpiso
{
  public:

    NSEpisodRCView* pRCEpiView ;

    // Constructeurs - Destructeur
    ArrayRCEpisodes(NSEpisodRCView* pRCView) ;
    ArrayRCEpisodes(const ArrayRCEpisodes& rv) ;
    ~ArrayRCEpisodes() ;

    void    vider() ;
    void    initialiser(bool bActive, string sMinDate) ;

    ArrayRCEpisodes& operator=(const ArrayRCEpisodes& src) ;
};

class NSRCListWindow ;

class NSEpisodRCView : public NSLDVView{    public:

        ArrayRCEpisodes*    pEpisods ;

        // Constructeur - Destructeur
        NSEpisodRCView(NSLdvDocument& doc, string sConcern = "") ;
        ~NSEpisodRCView();

        void    CmNouveau() ;
        void    CmEvolve() ;

        void    CmSelect() ;

        void CmOk() {}
        void CmCancel() {}

        // Redéfiniton des fonctions virtuelles de TView et TRichEdit
        static LPCSTR   StaticName() { return "NSEpisodRCView"; }
        //const char      *GetViewName() {return StaticName();}
        const char      *GetViewName() { return _sViewName.c_str() ; }
        TWindow         *GetWindow();
        bool            SetDocTitle(const char far* docname, int index)
        {
            return OWL::TWindow::SetDocTitle(docname, index) ;
        }
        void            setViewName() ;

        void            SetupColumns() ;
        void            AfficheListe() ;
        void            EvSetFocus(HWND hWndLostFocus) ;
        void            focusFct() ;

        string          donneContexte(string sContexte) ;
        void            initCurentEpiRC() ;

        void            reloadView(string sReason) ;

        NSLdvDocument*  getDoc() { return pLdVDoc ; }

    protected:

        // Remplissage de la vue
        void 	SetupWindow() ;
        void    SetupTxtBar() ;
        void    InitFormatFont() ;
        void    SetupToolBar() ;

        void 	DispInfoListe(TLwDispInfoNotify& dispInfo) ;
        void    LVNColumnclick(TLwNotify& lwn) ;

        void    sortByName() ;
        void    sortByEnding() ;
        void    sortByBegining() ;

        // view event handlers
        bool 	VnIsWindow(HWND hWnd) ;

        // Fonctions de réponse aux evts
        void    EvSize(uint sizeType, ClassLib::TSize &size) ;

        void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
        void    EvChar(uint key, uint repeatCount, uint flags) ;

        // Variables
        NSLdvDocument       *pLdVDoc ;	// Pointeur sur document EPISOD        NSRCListWindow*     pListeWindow ;        OWL::TControlBar    *pToolBar ;	        // Barre d'outils

        bool	            bSetupToolBar ;

        bool                bNaturallySorted ;
        int                 iSortedColumn ;

    DECLARE_RESPONSE_TABLE(NSEpisodRCView) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les médicaments à modifier)
//
class NSRCListWindow : public TListWindow
{
    public:

	    NSEpisodRCView* pView;        int             iRes;
	    NSRCListWindow(NSEpisodRCView* pere, int resId, int x, int y, int w,                                                int h, TModule* module = 0) ;

        ~NSRCListWindow() {}        void 	SetupWindow() ;
	    void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;        void    EvKeyDown(uint key, uint repeatCount, uint flags) ;        int     IndexItemSelect() ;

        void    EvSetFocus(HWND hWndLostFocus) ;

    DECLARE_RESPONSE_TABLE(NSRCListWindow) ;};

class NSItemHistoriser
{
  public:

    string      sLocalisation ;
    string      sLexique ;
    string      sComplement ;

    NSLigne*    pTextLine ;
    TimeArray   aDates ;
    bool        bInLastConsult ;

    NSItemHistoriser();
    NSItemHistoriser(const NSItemHistoriser& rv);
    ~NSItemHistoriser();

    NSItemHistoriser& operator=(const NSItemHistoriser& src) ;
};

typedef vector<NSItemHistoriser*>       ItemHistoVector ;
typedef ItemHistoVector::iterator       ItemHistoIter ;
typedef ItemHistoVector::const_iterator ItemHistoConstIter ;

class ItemHistoArray : public ItemHistoVector
{
  public:

    // Constructeurs - Destructeur
    ItemHistoArray();
    ItemHistoArray(const ItemHistoArray& rv);
    ~ItemHistoArray();

    void vider();

    ItemHistoArray& operator=(const ItemHistoArray& src);
};

class NSRCHistoryView : public NSLDVView
{    public:

        string          sClassifSens ;

        string          sArchetype ;
        nsarcParseur*   pParseur ;
        ItemHistoArray	Lignes ;

        string          sConcernText ;

        // Constructeur - Destructeur
        NSRCHistoryView(NSLdvDocument& doc, string sConcern = "") ;
        ~NSRCHistoryView();

        void            concernChanged(string sConcern) ;

        void CmOk() {}
        void CmCancel() {}

        // Redéfiniton des fonctions virtuelles de TView et TRichEdit
        static LPCSTR   StaticName() { return "NSRCHistoryView"; }
        //const char      *GetViewName() {return StaticName();}
        const char      *GetViewName() { return _sViewName.c_str() ; }
        TWindow         *GetWindow();
        bool            SetDocTitle(const char far* docname, int index)
        {
            return OWL::TWindow::SetDocTitle(docname, index) ;
        }
        void            setViewName() ;

        void            SetupColumns() ;
        void            AfficheListe() ;
        void            EvSetFocus(HWND hWndLostFocus) ;

        string          donneContexte(string sContexte) ;
        void            initCurentEpiRC() ;

        void            reloadView(string sReason) ;

        NSLdvDocument*  getDoc() { return pLdVDoc ; }

    protected:

        NS_CLASSLIB::TPoint	    HautGcheFenetre ;
        UINT				    PremLigne ;		   // numéro de la ligne du haut
        LONG				    LargeurPolice ;
        NS_CLASSLIB::TRect	    RectangleGlobal ;
        NSFontArray			    StylesPolice ;
        NSStyleParagrapheArray  StylesParagraphe ;

        // Remplissage de la vue
        void 	SetupWindow() ;

        void    initParams() ;
        void    reInitParams() ;
        void    initArchetype() ;
        void    initLines() ;
        void    initLines(Citem* pItem, NSEpiClassif* pClasser) ;
        void    initHistory() ;
        void    InitialiseLignes() ;

        void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

        void    SetupTxtBar() ;
        void    InitFormatFont() ;
        void    SetupToolBar() ;

        // view event handlers
        bool 	VnIsWindow(HWND hWnd) ;

        // Fonctions de réponse aux evts
        void    EvSize(uint sizeType, ClassLib::TSize &size) ;

        void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
        void    EvChar(uint key, uint repeatCount, uint flags) ;

        void    EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl) ;
	    void    EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl) ;
        LRESULT EvMouseWheel(WPARAM wparam, LPARAM lparam) ;

        // Variables
        NSLdvDocument       *pLdVDoc ;	// Pointeur sur document EPISOD        OWL::TControlBar    *pToolBar ;	        // Barre d'outils
        bool	            bSetupToolBar ;

    DECLARE_RESPONSE_TABLE(NSRCHistoryView) ;
};

#endif		// __NSRCVIEW_H
