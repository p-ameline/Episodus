
#ifndef __NSPANESPLITT_H
# define __NSPANESPLITT_H

# include <owl\button.h>
# include <owl/layoutwi.h>
# include <owl/panespli.h>
# include <owl/listwind.h>
# include <owl\mdichild.h>
# include <owl\control.h>
# include <owl\gadgetwi.h>

# include "nsbb\nsmview.h"
# include "nsbb\nsexport.h"


//class NSMUEView ;
//typedef ArrayMUEView ;

/*typedef vector<NSMUEView*> ArrayMUEView;
typedef ArrayMUEView::iterator ArrayMUEViewIter; */

/*class NSSplitter : public TSplitter
{
  public :
    NSSplitter(TWindow* parent, NSPaneSplitter* ps, float percent);
    ~NSSplitter();
    void    SetupEpilog(TSplitter* s, TWindow* targetPane, TWindow* newPane,
                  TLayoutMetrics& lmOfTargetPane, TLayoutMetrics& lmOfNewPane);
    NSPaneSplitter* pPaneSplitter;
    NS_CLASSLIB::TRect               GetRect() ;
    TSplitDirection     SplitDirection();
    TSplitterIndicator* CreateSplitterIndicator();
    TWindow*            Pane1(){};
    TWindow*            Pane2(){};
    void                Split(TWindow* targetPane,
                                      TWindow* newPane,
                                      TSplitDirection sd,
                                      float percent);
    TLayoutMetrics      Setup(TWindow* targetPane,
                                      TWindow* newPane,
                                      float percent);
    void                AdjForResize(const NS_CLASSLIB::TSize& sz);


    void                Move(int dist);
    void                AdjSplitterWidth(int w);

};
//horizontal splitter
class NSHSplitter : public NSSplitter
{
  public :
      NSHSplitter(TWindow* parent, NSPaneSplitter* ps, float percent=50.0);
      ~NSHSplitter();
      TLayoutMetrics    SetupPane(TWindow* targetPane, TWindow* newPane, float percent);
} ;  */

class NSGadgetWindow ;

class _NSBBCLASSE NSPaneSplitter : public TPaneSplitter, public NSRoot
{
	public:

    ArrayMUEView*       _aSplitWindows ;
    NSGadgetWindow*     _pGadgetPanelWindow ;
    TMDIChild*          _pMDIChild ;
    NSMUEView*          _pCurrentFocusedView ;

    // NSClientWindow(TWindow* parent, TSplitDirection splitDir, float percent = .50);
    NSPaneSplitter(NSContexte* pCtx, TWindow* parent, const char far* title = 0,TModule* module = 0);
    ~NSPaneSplitter();

    void                concernChanged(string sConcern) ;

    bool                nsSplitPane(TWindow* target, TWindow* newPane,
                        TSplitDirection splitDir, float percent) ;
    ClassLib::TRect     computeMetaRectWindow(NSMUEView *pwndView) ;
    void                closeAllWindows() ;
    NSMUEView*          researchMotherWindow(string sWndChild, string sWndCreateMDIChild) ;
    //VecteurString*      researchChildWindFonction(string sWndCreateMDIChild);
    void                researchChildWindFonction(string sWndCreateMDIChild, VecteurString* pChildWindsFct) ;
    NSMUEView*          researchDocument(string sDocType) ;
    TLayoutMetrics      PaneSetup(OWL::TSplitter* s,TWindow* targetPane,
                            TWindow* newPane, TSplitDirection splitDir, float percent) ;
    void                SetFrameTitle(string sFunction, string sTitle = "") ;
    void                GetDefLM(TLayoutMetrics& lm) ;
    void                youCanCloseNow() ;

    void                CmOk() ;
    void                CmHelp() ;
    void                CmCancel() ;

    void           CmAdd()      { if (_pCurrentFocusedView) _pCurrentFocusedView->CmAdd() ; }
    void           CmClose()    { if (_pCurrentFocusedView) _pCurrentFocusedView->CmClose() ; }
    void           CmSuppress() { if (_pCurrentFocusedView) _pCurrentFocusedView->CmSuppress() ; }
    void           CmChange()   { if (_pCurrentFocusedView) _pCurrentFocusedView->CmChange() ; }
    void           CmContinue() { if (_pCurrentFocusedView) _pCurrentFocusedView->CmContinue() ; }
    void           CmPrevious() { if (_pCurrentFocusedView) _pCurrentFocusedView->CmPrevious() ; }
    void           CmAll()      { if (_pCurrentFocusedView) _pCurrentFocusedView->CmAll() ;  }
    void           CmFct1()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct1() ; }
    void           CmFct2()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct2() ; }
    void           CmFct3()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct3() ; }
    void           CmFct4()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct4() ; }
    void           CmFct5()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct5() ; }
    void           CmFct6()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct6() ; }
    void           CmFct7()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct7() ; }
    void           CmFct8()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct8() ; }
    void           CmFct9()     { if (_pCurrentFocusedView) _pCurrentFocusedView->CmFct9() ; }

    void                setFocusToNextView(NSMUEView* pCurrentView, HWND hWndLostFocus) ;
    void                setFocusToPrevView(NSMUEView* pCurrentView, HWND hWndLostFocus) ;
    void                activateParent() ;

    TGadgetWindow*      getGadgetWindow() ;
    void                FlushControlBar() ;
    void                Insert(TGadget& gadget, TGadgetList::TPlacement iPlac = TGadgetList::After, TGadget* sibling = 0) ;
    void                SetTootipText(int gadgetId, string sText) ;
    void                LayoutSession() ;

  protected:

    void                SetupWindow() ;
    void                CleanupWindow() ;

    void                setGadgetWindowLayout() ;

  private:
    bool                HasPane(TWindow* p) ;
    TWindow*            Pere ;

  DECLARE_RESPONSE_TABLE(NSPaneSplitter) ;
};

class _NSBBCLASSE NSGadgetWindow : public TGadgetWindow, public NSRoot
//class _NSBBCLASSE NSGadgetWindow : public TWindow
{
  public:

		NSGadgetWindow(NSContexte* pCtx, TWindow* parent, uint32 iBtnStyle = 0, const char far* title = 0 );
  /* NSGadgetWindow(TWindow* parent = 0, TTileDirection direction = Horizontal,
        OWL::TFont* font = new TGadgetWindowFont, TModule* module = 0);  */
    ~NSGadgetWindow() ;

    void           setPaneSplitter (NSPaneSplitter* pPane) { _pPaneSplitter = pPane ; }
    NSPaneSplitter *getPaneSplitter() { return _pPaneSplitter ; }

    void           CmChildOK() ;
    void           CmChildCancel() ;
    void           CmChildHelp() ;
    void           CmChildPrevious() ;

    void           CmAdd()      { if (_pPaneSplitter) _pPaneSplitter->CmAdd() ; }
    void           CmClose()    { if (_pPaneSplitter) _pPaneSplitter->CmClose() ; }
    void           CmSuppress() { if (_pPaneSplitter) _pPaneSplitter->CmSuppress() ; }
    void           CmChange()   { if (_pPaneSplitter) _pPaneSplitter->CmChange() ; }
    void           CmContinue() { if (_pPaneSplitter) _pPaneSplitter->CmContinue() ; }
    void           CmAll()      { if (_pPaneSplitter) _pPaneSplitter->CmAll() ;  }
    void           CmFct1()     { if (_pPaneSplitter) _pPaneSplitter->CmFct1() ; }
    void           CmFct2()     { if (_pPaneSplitter) _pPaneSplitter->CmFct2() ; }
    void           CmFct3()     { if (_pPaneSplitter) _pPaneSplitter->CmFct3() ; }
    void           CmFct4()     { if (_pPaneSplitter) _pPaneSplitter->CmFct4() ; }
    void           CmFct5()     { if (_pPaneSplitter) _pPaneSplitter->CmFct5() ; }
    void           CmFct6()     { if (_pPaneSplitter) _pPaneSplitter->CmFct6() ; }
    void           CmFct7()     { if (_pPaneSplitter) _pPaneSplitter->CmFct7() ; }
    void           CmFct8()     { if (_pPaneSplitter) _pPaneSplitter->CmFct8() ; }
    void           CmFct9()     { if (_pPaneSplitter) _pPaneSplitter->CmFct9() ; }

    void           CeHasChildOK(TCommandEnabler& commandHandler) ;
    void           CeHasChildCancel(TCommandEnabler& commandHandler) ;
    void           CeHasChildHelp(TCommandEnabler& commandHandler) ;
    void           CeHasChildPrevious(TCommandEnabler& commandHandler) ;

    void           CeHasChildAdd(TCommandEnabler& commandHandler) ;
    void           CeHasChildClose(TCommandEnabler& commandHandler) ;
    void           CeHasChildSuppress(TCommandEnabler& commandHandler) ;
    void           CeHasChildChange(TCommandEnabler& commandHandler) ;
    void           CeHasChildContinue(TCommandEnabler& commandHandler) ;
    void           CeHasChildAll(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct1(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct2(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct3(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct4(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct5(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct6(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct7(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct8(TCommandEnabler& commandHandler) ;
    void           CeHasChildFct9(TCommandEnabler& commandHandler) ;

    void           EvSetFocus(HWND hWndLostFocus) ;
    OWL::TButton*  _pOK ;
    OWL::TButton*  _pCancel ;
    OWL::TButton*  _pHelp ;
    void           activateParent() ;

    void           FlushControlBar() ;
    int            getGadgetsHeight() ;
    void           enableCommandForGadgets() ;
    void           insertMainGadgets() ;

    string         _sTextForCmAdd ;
    string         _sTextForCmClose ;
    string         _sTextForCmSuppress ;
    string         _sTextForCmChange ;
    string         _sTextForCmContinue ;
    string         _sTextForCmAll ;
    string         _sTextForCmFct1 ;
    string         _sTextForCmFct2 ;
    string         _sTextForCmFct3 ;
    string         _sTextForCmFct4 ;
    string         _sTextForCmFct5 ;
    string         _sTextForCmFct6 ;
    string         _sTextForCmFct7 ;
    string         _sTextForCmFct8 ;
    string         _sTextForCmFct9 ;

    void           setButtonsStyle(unsigned int iBtnStyle) { _uButtonsStyle = iBtnStyle ; }
    unsigned int   getButtonsStyle()                       { return _uButtonsStyle ; }

  protected:

    void           SetupWindow() ;
    bool           IdleAction(long idleCount) ;

    TResult        EvNotify(uint id, TNotify far& notifyInfo) ;

    NSPaneSplitter* _pPaneSplitter;
    bool            _bWithButtons;
    unsigned int    _uButtonsStyle ;

	DECLARE_RESPONSE_TABLE(NSGadgetWindow) ;
} ;

class _NSBBCLASSE NSGadgetButton : public OWL::TButton
{
 public:

	NSGadgetButton(TWindow *parent, int Id, const char far* text, int X, int Y, int W, int H)
                :OWL::TButton(parent, Id, text, X, Y, W, H) {}
  char far* GetClassName() { return "BUTTON" ; }};

/*typedef  struct {

       string sString;
       NSMUEView* pWndMUEView;

} StructEnumPane ;   */

class StructEnumPane
{
	public :

		StructEnumPane() ;
		~StructEnumPane() ;

		string     getString()                 { return sString ; }
		void       setString(string sAct)      { sString = sAct ; }

		NSMUEView* getWindow()                 { return pWndMUEView ; }
		void       setWindow(NSMUEView* pWnd)  { pWndMUEView = pWnd ; }

		bool       getBParam()                 { return bParam ; }
		void       setBParam(bool b)           { bParam = b ; }

		bool       getUParam()                 { return uParam ; }
		void       setUParam(bool u)           { uParam = u ; }

	protected :

		string     sString ;
		NSMUEView* pWndMUEView ;
		bool       bParam ;
		uint32     uParam ;
};


#endif // __NSPANESPLITT_H
