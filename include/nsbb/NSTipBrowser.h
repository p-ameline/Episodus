#ifndef __NSTIPBROWSER_H#define __NSTIPBROWSER_H
#include <owl\applicat.h>
#include <owl\window.h>#ifdef _ENTERPRISE_DLL  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif#include "nsbb\nsexport.h"#include "nsbb\ns_skins.h"#define CM_TIPTIMER_ON  201#define CM_TIPTIMER_OFF 202# define ID_HT_TIMER 25# define TIMER_VALUE	 200
class TWebTip ;

//// Bulles d'aides Nautilus
//
class _NSBBCLASSE NSWebTipWindow : public TWindow, public NSRoot{	// Construction	public:		NSWebTipWindow(TWindow* pParent, NSContexte* pCtx) ;		~NSWebTipWindow() ;
    void PerformCreate(int menuOrId) ;
    void SetupWindow() ;    // Création de la fenêtre    bool Create() ;
    // Affichage de la fenetre windows
    void Show(NS_CLASSLIB::TRect rectTitle, string* psURL,                    NS_CLASSLIB::TRect* pHoverRect = NULL, int time = -1) ;

    // Cache la fenêtre
    void Hide() ;		void Navigate() ;
    void EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;    // void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;    void EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvMButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvRButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvTimer(uint timerId) ;    void setTipsTimer()  { SetTimer(ID_HT_TIMER, TIMER_VALUE) ; }    void killTipsTimer() { KillTimer(ID_HT_TIMER) ; }    // void EvPaint() ;    void CmFermer() ;    void disableTipTimer() ;    void enableTipTimer() ;
	protected:
		TWindow             *pParentWnd ;  // Fenetre parent    std::string         sURl ;	    // Label ç afficher    nsSkin*             pSkin ;        // Skin de l'affichage    NS_CLASSLIB::TRect  rectTitle ;    NS_CLASSLIB::TRect  rectHover ;    NS_CLASSLIB::TRect  rectDisplay ;    unsigned int				timercount ;
    unsigned int 				time_to ;

    TWebTip				      *Form ;

	DECLARE_RESPONSE_TABLE(NSWebTipWindow) ;};

#endif // __NSTIPBROWSER_H
