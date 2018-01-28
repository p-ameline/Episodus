// -----------------------------------------------------------------------------
// header des vues nscqvue.h
// -----------------------------------------------------------------------------
#if !defined(__NSCQVUE_H)
# define __NSCQVUE_H

class NSCQWindowsView ;
class NSCQDocument ;
class NSSmallBrother ;

# include <owl\docview.h># include <owl\mdichild.h>//#include <owl\panespli.h>#include <owl\splitter.h>#include <owl\controlb.h>// # include "nautilus\nsdocref.h"
// # include "nautilus\nautilus.rh"
// # include "nsbb\nstrewi.h"
// # include "nautilus\nscqdoc.h"
# include "nsbb\nsMView.h"
# include "nsbb\nsMdiChild.h"

// -----------------------------------------------------------------------------/**
* View (doc/view) for Archetypes
* Vue (document/vue) pour les Archetypes
*/
class NSCQWindowsView : public NSBBMUEView
{
	public:

		NSSmallBrother*     _pBigBoss ;
		NSCQDocument*       _pDoc ;			    // Pointeur sur document formulaire
		NS_CLASSLIB::TRect  _dlgRect ;

		OWL::TControlBar*   _pToolBar ;		    // Barre d'outils		bool			          _bSetupToolBar ;

		NSCQWindowsView(NSCQDocument& doc, TWindow *parent = 0) ;		~NSCQWindowsView() ;

		void                PerformCreate(int menuOrId) ;
		void                SetupWindow() ;
    void                Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
		void 			          lanceArchetype() ;    void                EvKeyDown(uint key, uint repeatCount, uint flags) ;    void                EvSysKeyDown(uint key, uint repeatCount, uint flags) ;    void                EvKeyUp(uint key, uint repeatCount, uint flags) ;    bool                PreProcessMsg(MSG &msg) ;		static LPCSTR       StaticName()    { return "NSCQWindowsView" ; }
		const char*         GetViewName()   { return StaticName() ; }
		TWindow*            GetWindow() ;
		void                setViewName() ;

		// Fonction d'initialisation		void 		            SetupToolBar() ;
		void		            EvSetFocus(THandle hWndLostFocus) ;
		void 		            EvClose() ;
		void                EvDestroy() ;

		// Document notifications		bool 		            CanClose() ;		void 		            CmPublier() ;
		void 		            CmVisualiser() ;
		void 		            CmEnregistre() ;
    void                CmQuickSave() ;
		void                CmFileClose() ;
    void                CmHelp() ;
		void                CmOk() ;
    void                CmOkFinalStep() ;
    void                CmCancel() ;

    void                Compta() ;

  protected:

    bool                _bCanClose ;
    bool                _bOkActivated ;
    bool                _bWantToSaveAsked ;

    bool                askWantToSave() ;

		// bool                PreProcessMsg(MSG& msg) ;

	DECLARE_RESPONSE_TABLE(NSCQWindowsView) ;
} ;

/**
* View (doc/view) for Archetypes
* Vue (document/vue) pour les Archetypes
*/
class NSCQVue : public NSMUEView
{
	public:

		NSSmallBrother*     pBigBoss ;
		NSCQDocument*       pDoc ;			    // Pointeur sur document formulaire
		NSDialog*           clientWin ;          // Pointeur sur la TWindow du dialogue archetype client
		NS_CLASSLIB::TRect  dlgRect ;

		OWL::TControlBar*   pToolBar ;		    // Barre d'outils		bool			          bSetupToolBar ;

		NSCQVue(NSCQDocument& doc, TWindow *parent = 0) ;		~NSCQVue() ;

		void                PerformCreate(int menuOrId) ;
		void                SetupWindow() ;
		void 			          lanceArchetype() ;		static LPCSTR       StaticName()    { return "NSCQVue" ; }
		const char*         GetViewName()   { return StaticName() ; }
		TWindow*            GetWindow() ;
		void                setViewName() ;

		// Fonction d'initialisation		void 		            SetupToolBar() ;
		void		            EvSetFocus(THandle hWndLostFocus) ;
		void 		            EvClose() ;
		void                EvDestroy() ;

		// Document notifications		bool 		            CanClose() ;
		void 		            CmPublier() ;
		void 		            CmVisualiser() ;
		void 		            CmEnregistre() ;
		void                CmFileClose() ;
		void                CmOk() ;

		bool                PreProcessMsg(MSG& msg) ;

	DECLARE_RESPONSE_TABLE(NSCQVue) ;
} ;

#endif // __NSCQVUE_H

