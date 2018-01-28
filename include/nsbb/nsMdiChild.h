// -----------------------------------------------------------------------------
// header for MDI child windows nsMdiChild.h
// -----------------------------------------------------------------------------
#if !defined(__NSMDICHILD_H)
# define __NSMDICHILD_H

class NSPaneSplitter ;
class NSGadgetWindow ;
class NSDialog ;

# include <owl\mdichild.h># include <owl\splitter.h>#include "nsbb\nsexport.h"// -----------------------------------------------------------------------------/*** Helper class for MDI windows, with splitting capabilities and toolbar* Classe facilitant l'usage de fenêtres MDI, avec cloisonnement*/class _NSBBCLASSE NSMDIChildContent : public TWindow, public NSRoot{	public :		NSMDIChildContent(NSContexte* pCtx, TWindow* parent) ;    void setSplitter(TPaneSplitter* pSplit) { _pSplitter = pSplit ; }    void setToolBar(TGadgetWindow* pTool)   { _pToolBar = pTool ; }    TPaneSplitter* getSplitter() { return _pSplitter ; }    TGadgetWindow* getToolBar()  { return _pToolBar ; }    void           CmChildOK() ;    void           CmChildHelp() ;
    void           CmChildCancel() ;	protected :		TPaneSplitter* _pSplitter ;    TGadgetWindow* _pToolBar ;    void   EvSize(uint sizeType, ClassLib::TSize &size) ;	DECLARE_RESPONSE_TABLE(NSMDIChildContent) ;} ;/*** Helper class for MDI windows, with splitting capabilities* Classe facilitant l'usage de fenêtres MDI, avec cloisonnement*/class _NSBBCLASSE NSMDIChild : public TMDIChild, public NSRoot
{
	public:

		bool     bShrinkToClient ;
		TWindow* client ;
    HWND     hWndToFocus ;

		NSMDIChild( NSContexte* pCtx,
    						TMDIClient&     parent,
								const char far  *title = 0,
								TWindow         *clientWnd = 0,
								bool            shrinkToClient = true,
								TModule         *module = 0) ;

		NSMDIChild(NSContexte* pCtx, HWND hWnd, TModule *module = 0) ;
		virtual ~NSMDIChild() ;

		void        split(TWindow *pMaster ,TWindow *pSlave,
                                TSplitDirection splitDir, float percent);

		//Chaque MDIChild a un panel qui a ete cree par une fenetre
		void     setCreateWindow(TWindow* pWind) ;
    void     resetCreateWindow()  { _pWndCreatePanel = (TWindow*) 0 ; }
		TWindow* getCreateWindow()    { return _pWndCreatePanel ; }

		void     RecordChildPos() ;
		int      GetFrameIndex() ;
		void     PerformCreate(int menuOrId) ;

		void     initClient(NSDialog *pDlg) ;
    void     initClientArchetype(TWindow *pView) ;
		void     EvDestroy() ;
		void     EvSetFocus(THandle hWndLostFocus) ;
    void 	   EvActivate(uint active, bool minimized, HWND hwnd) ;
    bool     EvNCActivate(bool active) ;
    void     EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
		void     EvParentNotify(uint event, uint childHandleOrX, uint childIDOrY) ;

		bool     SetDocTitle(const char far* docname, int index) ;

    // Override virtual methods defined by TWindow
    //
    bool     PreProcessMsg(MSG& msg);
    bool     ShowWindow(int cmdShow);
    bool     EnableWindow(bool enable);
    void     Destroy(int retVal = 0);

    bool     IsDestroyed() const { return _bDestroyed ; }

    NS_CLASSLIB::TRect GetRectInMdiClient() ;

	protected:

		TWindow* _pWndCreatePanel ;

    bool     _bDestroyed ;

    void     closeChildren() ;

	DECLARE_RESPONSE_TABLE(NSMDIChild) ;
} ;

class _NSBBCLASSE NSBasicMDIClient : public OWL::TMDIClient
{
  public:

    NSBasicMDIClient(OWL::TModule* module = 0) ;
   	virtual ~NSBasicMDIClient() ;

		bool CloseChildrenButServicesWindows(NSContexte *pContexte) ;
    void MdiChildrenGarbageCollector() ;

    void getAvailableHForRect(NS_CLASSLIB::TRect& rect) const ;
    void getAvailableWForRect(NS_CLASSLIB::TRect& rect) const ;

    // bool IsDecisionTreeOpened(NSContexte *pContexte, string sArchetype) ;

	DECLARE_RESPONSE_TABLE(NSBasicMDIClient) ;
} ;

#endif // __NSMDICHILD_H

