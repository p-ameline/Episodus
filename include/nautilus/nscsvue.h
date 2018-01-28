#if !defined(__NSCSVUE_H)#define __NSCSVUE_H

class NSTypeDocument ;
class NSHtml ;

#include <owl\docview.h>

// #include "nautilus\nsdocref.h"
// #include "nautilus\nautilus.rh"
// #include "nautilus\richedap.h"
#include "nsbb\nstrewi.h"
// #include "nautilus\nscsdoc.h"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
class NSCsVue : public TView, public NSTreeWindow
// class NSCsVue : public NSMUEView, public NSTreeWindow
{
	public:

  	NSSmallBrother* 	_pBigBoss ;
    NSTypeDocument* 	_pNSTypeDocument ;
    // Pointeurs
    NSCSDocument*	 	  _pDoc ;			    // Pointeur sur document CS
    OWL::TControlBar* _pToolBar ;		    // Barre d'outils    bool            	_bFermerDocument ;   //mis à true quand on ferme un document
    bool				      _bConsultVide ;

    bool				      _bSetupToolBar ;

    TMenuDescr        *_pMUEViewMenu ;
    HACCEL            _hAccelerator ;

    NSCsVue(NSCSDocument& doc, TWindow* parent = 0) ;
	  ~NSCsVue() ;

	  void 				lanceConsult() ;
    void 				CmEnregistre() ;    void        CmQuickSave() ;
    static LPCSTR     StaticName()  { return "NSCsVue" ; }
	  const char* 	    GetViewName() { return StaticName() ; }
    TWindow*	        GetWindow();

    // Fonction d'initialisation
	  void SetupWindow() ;
    void SetupToolBar() ;

    void EvSize(uint sizeType, NS_CLASSLIB::TSize& size) ;
    void EvSysCommand(uint cmdType, NS_CLASSLIB::TPoint& point) ;
    void EvSetFocus(THandle hWndLostFocus) ;
    void EvClose() ;
    void EvDestroy() ;
    bool EvQueryEndSession() ;

    // Overrides TWindows' in order to process our accelerators table
    bool PreProcessMsg(MSG &msg) ;

    void SetWindowPosSynthse() ;     //fixer la position de la synthèse
    void EnregistrePosSynthse() ;    //enregistrer la position de la synthèse dans
                                            //la base de registre (BR)
    void InscrireHtml(NSHtml* pHtml) ;
    void InscrireFils(NSTreeNode* pNode, NSHtml* pHtml) ;

    // Document notifications
    bool VnIsWindow(HWND hWnd) { return NSTreeWindow::HWindow == hWnd ; }
    bool CanClose();
    void CmFileClose();
    void CmPublier();
    void CmVisualiser();
    void CmComposer();

    void EditionFilGuide();
    void Encyclop();
    void Compta();

    void Cm1Plus() ;
    void Cm2Plus() ;
    void Cm3Plus() ;
    void Cm4Plus() ;
    void CmXPlus(string sModifier) ;

    void Ce1Plus(TCommandEnabler&) ;
    void Ce2Plus(TCommandEnabler&) ;
    void Ce3Plus(TCommandEnabler&) ;
    void Ce4Plus(TCommandEnabler&) ;
    void CeXPlus(TCommandEnabler&, string sModifier) ;

    void initMUEViewMenu(string sMenuName) ;
    void activateMUEViewMenu() ;

    void reloadView(string sReason) ;

    bool isSynthesis() ;

  DECLARE_RESPONSE_TABLE(NSCsVue) ;
};

#endif

