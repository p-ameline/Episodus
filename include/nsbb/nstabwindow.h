// -----------------------------------------------------------------------------
// source :
// OWL Extensions (OWLEXT) Class Library - Copyright(c) 1996 by Manic Software.
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: pameline $
// $Date: 2015/02/07 14:31:43 $
// -----------------------------------------------------------------------------
// FLP - juin 2003
// -----------------------------------------------------------------------------
// original comment :
// TTabWindow
// -----------------------------------------------------------------------------

#ifndef __NSTABWINDOW_H__
# define __NSTABWINDOW_H__

# ifndef OWL_TABCTRL_H
#  include <owl/tabctrl.h>
# endif

# include <vector>

# include "nsbb\nsexport.h"
# include "nsbb\nsdlg.h"

typedef vector<TWindow*>                 NSControlsVector ;
typedef NSControlsVector::iterator       NSControlsIter ;
typedef NSControlsVector::const_iterator NSControlsConstIter ;

class _NSBBCLASSE NSControlsArray : public NSControlsVector
{
	public:

		NSControlsArray() ;
		NSControlsArray(const NSControlsArray& src) ;
		~NSControlsArray() ;

		NSControlsArray operator=(const NSControlsArray& src) ;
		void            vider() ;
} ;

// classe TWindow avec GetWindowClass
// pour la redéfinition du style
class _NSBBCLASSE NSOneTabWindow : public TWindow
{
	public:

		NSControlsArray* controls ;

    NSOneTabWindow(TWindow* parent, const char far* title = 0, TModule* module = 0) ;
    ~NSOneTabWindow() ;

    void      SetupWindow() ;
    void      EvSetFocus(THandle hWndLostFocus /* may be 0 */) ;
    char far* GetClassName() ;
    void      GetWindowClass(WNDCLASS& wndClass) ;
    bool      Create() ;
    void      Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

	DECLARE_RESPONSE_TABLE(NSOneTabWindow) ;
} ;


typedef vector<NSOneTabWindow*>         NSWindowsVector ;
typedef NSWindowsVector::iterator       NSWindowsIter ;
typedef NSWindowsVector::const_iterator NSWindowsConstIter ;

struct NSTabWindowDatum
{
	TWindow *m_ptr ;
	LPCTSTR m_title ;
} ;

class _NSBBCLASSE NSWindowsArray : public NSWindowsVector
{
	public:

		NSWindowsArray() ;
  	NSWindowsArray(const NSWindowsArray& src) ;
  	~NSWindowsArray() ;

  	NSWindowsArray operator=(const NSWindowsArray& src) ;
  	void           vider() ;
} ;

class _NSBBCLASSE NSTabWindow : public NSDialog
{
	// Object lifetime methods
 	public:
		// NSTabWindow(TWindow *parent, string title, NSWindowsArray *array = 0) ;
		// NSTabWindow(TWindow *parent, string title, uint32 X = 0, uint32 Y = 0, uint32 W = 0, uint32 H = 0, NSWindowsArray *array = 0) ;
#ifdef __OB1__
    NSTabWindow(NSContexte* pCtx, TWindow* AParent, BBItem* pItem = 0, TModule* pNSResModule = 0, NsMultiDialog* pControl = 0) ;
#else
		NSTabWindow(NSContexte* pCtx, TWindow* AParent, BBItem* pItem = 0, TModule* pNSResModule = 0, NsMultiDialog* pControl = 0, bool initFromBbk = false) ;
#endif
    ~NSTabWindow() ;

	private:
		NSTabWindow() ;                                       // DISALLOWED METHOD
		NSTabWindow(const NSTabWindow&) ;                     // DISALLOWED METHOD
		NSTabWindow& operator=(const NSTabWindow& src) ;   // DISALLOWED METHOD

	// Tabbed-window interface: adding windows, removing windows
 	public:
		virtual void            Attach(NSOneTabWindow *ptr, const char *title = 0) ;
		virtual void            Attach(NSWindowsArray *windowArray) ;
		virtual NSOneTabWindow *Detach(int index)/* = 0*/ ;
		virtual NSOneTabWindow *Detach(NSOneTabWindow *ptr)     { return Detach(Retrieve(ptr)) ; }
		virtual NSOneTabWindow *Retrieve(int index)        	    { return ((*tabbedWindows)[index]) ; }
		virtual int             Retrieve(NSOneTabWindow *ptr) ;
		virtual NSOneTabWindow *operator[](int index)      	    { return ((*tabbedWindows)[index]) ; }
		virtual int             operator[](NSOneTabWindow *win) { return Retrieve(win) ; }

		virtual int             GetSelectedTab() 							  { return m_tabCtrl->GetSel() ; }
		virtual void            SetSelectedTab(int index)/* = 0*/ ;

		void		    SetupWindow() ;
    void        CreerControlesArchetype() ;
    void        CmOk() ;
    void        CmCancel() ;
    void        CmHelp() ;

/*
//	TO DO
		virtual int32 GetWindowCount()
				{ return tabbedWindowArray.GetItemsInContainer(); }
*/

	protected:
		// enum { ID_TABCTRL = 100 } ;
		virtual TWindow *GetTabCtrl()/* = 0 */;
		void						EvSize(uint sizeType, ClassLib::TSize& size) ;
    void 	          EvActivate(uint active, bool minimized, HWND hwnd) ;
    void            EvChildActivate() ;
    void            EvSetFocus(THandle hWndLostFocus /* may be 0 */);
		void						TabSelChange(TNotify far&) ;
		bool 						TabSelChanging(TNotify far&) ;
		void 						TabKeyDown(TTabKeyDown far&) ;
		virtual int 		AdjustTabClient() ;
   /*  void                      EvMove(ClassLib::TPoint& clientOrigin);
    void                       EvWindowPosChanged(WINDOWPOS far& windowPos);
    void                       EvWindowPosChanging(WINDOWPOS far& windowPos); */

	// OWL methods
 	protected:
		DECLARE_RESPONSE_TABLE(NSTabWindow) ;

	// Internal Data
	protected:      // so deriveds can see it more easily
		NSWindowsArray  *tabbedWindows ;

	private:
		TTabControl			*m_tabCtrl ;
    string          sTitle ;
    HFONT           hBorFont ;
    bool            bControlInit ;
};

// -----------------------------------------------------------------------------
//                                                               TWin95TabWindow
// -----------------------------------------------------------------------------
/*
class OWLEXTCLASS TWin95TabWindow : public TTabWindow {
		// Ctors & Dtors
		//
public:
		TWin95TabWindow(TWindow* parent, TWindow* array[] = 0);
		TWin95TabWindow(TWindow* parent, uint32 X, uint32 Y, uint32 W, uint32 H, TWindow* array[] = 0);
		virtual ~TWin95TabWindow();

		// OWL Methods
		//
protected:
		virtual void SetupWindow();
		DECLARE_RESPONSE_TABLE(TWin95TabWindow);
		void EvSize(uint sizeType, TSize& size);
		void TabSelChange(TNotify far&);
		bool TabSelChanging(TNotify far&);
		void TabKeyDown(TTabKeyDown far&);

		// Tabbed-window interface: adding windows, removing windows
		//
public:
		virtual void Attach(TWindow* ptr, const char* title = 0);
		virtual void Attach(TWindow* windowArray[])
			{ TTabWindow::Attach(windowArray); }
		virtual TWindow* Detach(int32 index);
		virtual TWindow* Detach(TWindow* ptr)
			{ return TTabWindow::Detach(ptr); }
		virtual int32 GetSelectedTab()
			{ return m_tabCtrl->GetSel(); }
		virtual void SetSelectedTab(int32 index);
		virtual int32 GetWindowCount();
protected:
#if defined(BI_NO_COVAR_RET)
		virtual TWindow* 			GetTabCtrl()
#else
		virtual TTabControl* 	GetTabCtrl()
#endif
			{ return m_tabCtrl; }

		virtual void AdjustTabClient();

		// Internal data
		//
private:
		TTabControl* m_tabCtrl;
};
*/



// -----------------------------------------------------------------------------
// By arranging the code this way, we allow for other tabbed-window schemes to
// customize the actual window-arrangement; for example, the class declaration
// below would allow us to use the TNoteTab control instead of the TTabControl
// control to manage the window-flipping.
// -----------------------------------------------------------------------------
// TNoteTabWindow
// -----------------------------------------------------------------------------
/*
#include <owl/notetab.h>

class OWLEXTCLASS TNoteTabWindow : public TTabWindow
{
  // Ctors & Dtors
 public:
  TNoteTabWindow(TWindow *parent, TWindow *array[] = 0) ;
  TNoteTabWindow(TWindow *parent, uint32 X, uint32 Y, uint32 W, uint32 H, TWindow *array[] = 0) ;
  virtual           ~TNoteTabWindow() ;

  // OWL Methods
 protected:
  virtual void      SetupWindow() ;
  DECLARE_RESPONSE_TABLE(TNoteTabWindow) ;
  void              EvSize(uint sizeType, TSize& size) ;
  void              TabSelChange(TNotify far&) ;
  bool              TabSelChanging(TNotify far&) ;
  void              TabKeyDown(TTabKeyDown far&) ;

  // Tabbed-window interface: adding windows, removing windows
 public:
  virtual void      Attach(TWindow *ptr, const char *title = 0) ;
  virtual void      Attach(TWindow *windowArray[])  { TTabWindow::Attach(windowArray) ; }
  virtual TWindow   *Detach(int32 index) ;
  virtual TWindow   *Detach(TWindow *ptr)           { return TTabWindow::Detach(ptr) ; }
  virtual int32     GetSelectedTab()                { return m_tabCtrl->GetSel() ; }
  virtual void      SetSelectedTab(int32 index)     { m_tabCtrl->SetSel(index) ; }
  virtual int32     GetWindowCount()
    {
      if (m_tabCtrl->GetCount() != tabbedWindowArray.GetItemsInContainer())
        TRACE("This is a problem--tabCtrl->GetCount() != tabbedWindowArray") ;
      return TTabWindow::GetWindowCount() ;
    }

 protected:
  virtual TNoteTab  *GetTabCtrl()                   { return m_tabCtrl ; }
  virtual void      AdjustTabClient() ;

 private:
  TNoteTab          *m_tabCtrl ;
} ;
*/

#endif // __NSTABWINDOW_H__
