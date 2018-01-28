// -----------------------------------------------------------------------------// nsgoalview.h
// affichage de la fenêtre des objectifs
// -----------------------------------------------------------------------------
// FLP - Janvier/Février 2004
// PA  - Juillet 2003
// -----------------------------------------------------------------------------
#if !defined(__NSFOLLOWUPVIEW_H)
#define __NSFOLLOWUPVIEW_H

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
#include "nsbb\nslistwind.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsldvvar.h"

class NSFollowUpElement : public NSRoot
{
	public:

		string     sLexique ;
		string     sColumnLabel ;

    NSLdvGoal* pGoal ;

		NSHistoryValManagementArray ElementsArray ;
    NSHistoryValManagementIter  currentIter ;
    bool                        bSortOrderByDateInf ;

    // Constructeur - Destructeur
  	NSFollowUpElement(NSContexte *pCtx, NSLdvGoal* pTheGoal) ;
		~NSFollowUpElement() ;

		bool   initIterToBegin() ;
    bool   initIterToEnd() ;
    bool   nextIter() ;
    bool   previousIter() ;
    void   sortArray() ;
    void   sortArrayByDateInf() ;
    void   sortArrayByDateSup() ;
    string getCurrentIterDate() ;
    void   insertBlankDate(string sNewDate) ;

    void   setLexique(string sNewLex, bool bUpdateLabel = true) ;
} ;

typedef vector<NSFollowUpElement*> NSFollowUpElementArray ;

typedef NSFollowUpElementArray::iterator NSFollowUpElementIter ;typedef NTArray<NSFollowUpElement> NSFollowUpArray ;

class NSFollowUpPropertyWindow ;

class NSFollowUpView : public NSLDVView{	public:

    NSHistoryValManagementArray followUpValues ;
		NSFollowUpArray             followUpIndexes ;

    // Variables
		NSLdvDocument               *pLdVDoc ;	// Pointeur sur document EPISOD		NSFollowUpPropertyWindow    *pListeWindow ;		OWL::TControlBar            *pToolBar ;	        // Barre d'outils

		bool                        bNaturallySorted ;
		int                         iSortedColumn ;

		bool	                      bSetupToolBar ;

		// Constructeur - Destructeur
  	NSFollowUpView(NSLdvDocument& doc, string sConcern = "") ;
		~NSFollowUpView() ;

		void                  CmNouveau() ;
		void                  CmModifier() ;
		void                  CmClore() ;

    void CmOk() {}
    void CmCancel() {}

		// Redéfiniton des fonctions virtuelles de TView et TRichEdit
		static LPCSTR         StaticName() { return "NSFollowUpView" ; }

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

		string                donneContexte(string sContexte) ;
		void                  initCurrentGoals() ;
    void                  processGoal(NSLdvGoal* pGoal) ;
    void                  synchronizeElements() ;

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

 DECLARE_RESPONSE_TABLE(NSFollowUpView) ;
} ;

// -----------------------------------------------------------------------------
// Objet "ListWindow" avec gestion du double-click (pour les médicaments à modifier)
// -----------------------------------------------------------------------------
class NSFollowUpPropertyWindow : public TListWindow
{
	public:

		NSFollowUpView*  pView ;
    int              iRes ;    OWL::TImageList* Images ;
    NSFollowUpPropertyWindow(NSFollowUpView *pere, int resId, int x, int y, int w, int h, TModule *module = 0) ;    ~NSFollowUpPropertyWindow() ;

    void        CmClore() ;
    void        CmCreate() ;
    void        CmModify() ;
    void				CmTraitSelectedObj() ;
    void        SetupWindow() ;
    void        EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void        EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;    void        EvKeyDown(uint key, uint repeatCount, uint flags);    int         IndexItemSelect() ;
    void        EvSetFocus(HWND hWndLostFocus) ;

	DECLARE_RESPONSE_TABLE(NSFollowUpPropertyWindow) ;} ;

#endif		// __NSFOLLOWUPVIEW_H
