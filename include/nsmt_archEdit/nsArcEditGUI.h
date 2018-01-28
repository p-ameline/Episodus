#if !defined(__NSARCEDITGUI_H)#define __NSARCEDITGUI_H

#include <owl\docview.h>
// #include <mfc\afxext.h>

class NSEditNum ;
class NSRectTracker ;
class NsControlEditDialog ;
class NsCaptionAndRectEditDialog ;

#include "nsbb\NSMView.h"
#include "nsbb\nsutidlg.h"
#include "nsmt_archEdit\nsArcEditDoc.h"
#include "nsmt_archEdit\nstrckrect.h"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
class NSArcEditInterfaceView : public NSBBMUEView
{
	public:

  	NSSmallBrother     *_pBigBoss ;
    NSPatPathoArray    *_pPatPathoArray ;
    // Pointeurs
    NSArchEditDocument *_pDoc ;			    // Pointeur sur document

    OWL::TControlBar   *_pToolBar ;		    // Barre d'outils    bool               _bSetupToolBar ;

    TMenuDescr         *_pMUEViewMenu ;
    HACCEL             _hAccelerator ;

    bool               _bIsModif ;

    NSArcEditInterfaceView(NSArchEditDocument& doc, TWindow* parent = 0) ;
	  ~NSArcEditInterfaceView() ;

    void CmEditWindowParams() ;

    void CmEditControle() ;
    void CmNewControle() ;

    void CmEnregistre() ;
    void CmSetParams() ;

    void startEditing() ;
    void CmOk() ;
    void CmOkFinalStep() ;
    void setViewName() ;
    void CmCancel() ;
    void CmHelp() ;

    static LPCSTR     StaticName()  { return "NSArcEditInterfaceView" ; }
	  const char* 	    GetViewName() { return StaticName() ; }
    TWindow*	        GetWindow() ;

    // Fonction d'initialisation
	  void SetupWindow() ;
    void SetupToolBar() ;
    void SetWindowPosit() ;
    void CmEnableButton(TCommandEnabler& tce) ;

    // Doc/Views functions
    void        selectObjectForLocalisation(string sLocalisation) ;
    NSControle* getControlForLocalisation(string sLocalisation) ;
    void        createControlForLocalisation(string sLocalisation, string sLabel) ;

    // User interaction functions
    void EvDelete() ;
    void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void EvLButtonUp(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void ControlActivated(uint id) ;

    void EvPaint() ;
    void EvNCPaint(HRGN) ;
    bool EvEraseBkgnd(HDC) ;
    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;
    void RedrawChildren() ;
    void EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    void EvTimer(uint id) ;

    void EvSize(uint sizeType, NS_CLASSLIB::TSize& size) ;
    void EvSysCommand(uint cmdType, NS_CLASSLIB::TPoint& point) ;
    void EvSetFocus(THandle hWndLostFocus) ;
    void EvClose() ;
    void EvDestroy() ;
    bool EvQueryEndSession() ;
    bool PreProcessMsg(MSG &msg) ;

    // Utilitary functions
    void        newSelection(TWindow* pSelected) ;

    TWindow*    hitTest(NS_CLASSLIB::TPoint& point) ;
    TWindow*    getElementFromLocalization(string sLocalization) ;
    NSControle* getNSControl(TWindow *pCtrl) { return NSBBMUEView::getNSControl(pCtrl) ; }
    Ccontrol*   getCControl(TWindow *pCtrl) ;
    void        setChildsStyle() ;

    // Document notifications
    bool VnIsWindow(HWND hWnd) { return NSBBMUEView::HWindow == hWnd ; }
    bool CanClose();
    void CmFileClose();

    void initMUEViewMenu(string sMenuName) ;
    void activateMUEViewMenu() ;

    int  getNextControlId() ;

  protected:

    string           _sLang ;

    TWindow         *_pEditedElement ;
    NSRectTracker   *_pEditTracker ;
    bool             _bTrackerIsVisible ;

    bool             _bLButtonDown ;
    TWindow         *_pWin2Drag ;
    OWL::TImageList *_pDragDrop ;

    NSRectTracker::TrackerHit _iTrackedHandle ;

    VectString       _aHistoryManager ;
    IterString       _itHistoryCursor ;

    HIMAGELIST  CreateDragImage() ;

    void        addToHistory() ;
    void        undo() ;
    void        redo() ;

    void        initDialogFromControl(NsControlEditDialog* pDialog, Ccontrol* pCControl) ;
    void        initControlFromDialog(NsControlEditDialog* pDialog, Ccontrol* pCControl) ;

    void        initDialogFromWindow(NsCaptionAndRectEditDialog* pDialog, Cdialogbox* pBox) ;
    void        initWindowFromDialog(NsCaptionAndRectEditDialog* pDialog, Cdialogbox* pBox) ;

    void        updateCaption() ;

    TWindow*    getMdiChildWindow() ;

  DECLARE_RESPONSE_TABLE(NSArcEditInterfaceView) ;
};

#endif // __NSARCEDITGUI_H

