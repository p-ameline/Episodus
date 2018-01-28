// -----------------------------------------------------------------------------// nsgoalview.h
// affichage de la fenêtre des objectifs
// -----------------------------------------------------------------------------
// FLP - Janvier/Février 2004
// PA  - Juillet 2003
// -----------------------------------------------------------------------------
#if !defined(__NSGOALVIEW_H)
#define __NSGOALVIEW_H

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
#include "nautilus\nsldvvar.h"

class NSGoalsPropertyWindow ;

class NSGoalView : public NSLDVView
{  public:

    NSObjectifArray aGoals ;

    // Constructeur - Destructeur
    NSGoalView(NSLdvDocument& doc, string sConcern = "") ;
    ~NSGoalView() ;

    void CmNouveau() ;
    void CmModifier() ;
    void CmClore() ;
    void CmFct3() ; // protocol
	  void CmFct4() ; // validate a goal
    void CmFct5() ; // add to a new protocol
	  void CmFct6() ; // add to an existing protocol
    void CmFct7() ; // invalidate a goal
    void CmPublish() ;

    void CmOk() {}
    void CmCancel() {}

    // Redéfiniton des fonctions virtuelles de TView et TRichEdit
    static LPCSTR         StaticName() { return "NSGoalView" ; }

    const char            *GetViewName() { return sViewName.c_str() ; }
    TWindow               *GetWindow() ;
    bool                  SetDocTitle(const char far *docname, int index)
    {
      return OWL::TWindow::SetDocTitle(docname, index) ;
    }
    void                  setViewName() ;

    void                  SetupColumns() ;
    void                  AfficheListe() ;
    void                  AfficheModel() ;
    void                  EvSetFocus(HWND hWndLostFocus) ;
    void                  focusFct() ;

    string                donneContexte(string sContexte) ;
    void                  initCurrentGoals() ;

    void                  reloadView(string sReason) ;
    void									CmTraitSelectedObj() ;
    bool									SelectedObjectifs() ;

  protected:

    // Remplissage de la vue
    void 	                SetupWindow() ;
    void                  SetupTxtBar() ;
    void                  InitFormatFont() ;
    void                  SetupToolBar() ;

    void 	                DispInfoListe(TLwDispInfoNotify& dispInfo) ;
    void                  LVNColumnclick(TLwNotify& lwn) ;

    void                  sortByColumn(int iColumn) ;

    // view event handlers
    bool 	                VnIsWindow(HWND hWnd) ;

    // Fonctions de réponse aux evts
    void                  EvSize(uint sizeType, ClassLib::TSize& size) ;

    void                  EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void                  EvChar(uint key, uint repeatCount, uint flags) ;

    bool                  InitObjectif(NSObjectif *pObj, NSLdvGoal *pGoal) ;
    void                  InitDateObjectifPonctuel(string& sDateObj, NSLdvGoal *pGoal, string sDateJalon, double dDelaiJalon, string sUniteJalon) ;

    // Variables
    NSLdvDocument         *pLdVDoc ;	// Pointeur sur document EPISOD    NSGoalsPropertyWindow *pListeWindow ;    OWL::TControlBar      *pToolBar ;	        // Barre d'outils

    bool                  bNaturallySorted ;
    int                   iSortedColumn ;
    string 								sViewConcern ;

    bool	                bSetupToolBar ;

	DECLARE_RESPONSE_TABLE(NSGoalView) ;
} ;

// -----------------------------------------------------------------------------
// Objet "ListWindow" avec gestion du double-click (pour les médicaments à modifier)
// -----------------------------------------------------------------------------
class NSGoalsPropertyWindow : public TListWindow
{
	public:

		NSGoalView*      pView ;
    int              iRes ;    OWL::TImageList* Images ;
    NSGoalsPropertyWindow(NSGoalView *pere, int resId, int x, int y, int w, int h, TModule *module = 0) ;    ~NSGoalsPropertyWindow() ;

    void        CmClore() ;
    void        CmCreate() ;
    void        CmModify() ;
    void        CmProtocol() ;
    void				CmTraitSelectedObj() ;
    void        CmNewProtocol() ;
    void        CmAddToProtocol() ;
    void        CmValidate() ;
    void        SetupWindow() ;
    void        EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void        EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;    void        EvKeyDown(uint key, uint repeatCount, uint flags) ;    int         IndexItemSelect() ;
    void        EvSetFocus(HWND hWndLostFocus) ;

	DECLARE_RESPONSE_TABLE(NSGoalsPropertyWindow) ;} ;

#endif		// __NSGOALVIEW_H
