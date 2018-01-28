#include <windows.h>#include <bwcc.h>
#if !defined(WINSYS_UIMETRIC_H)# include <winsys/uimetric.h>
#endif
#include "nautilus\nssuper.h"#include "nsbb\nstooltip.h"

//
// Timer id used by Tooltip window
//
const unsigned TOOLTIP_TIMERID = 0xABBA;

//----------------------------------------------------------------------------
// NSToolTip
//

//
// Constructor for NSToolTip
//
// Initializes its data fields using parameters passed and default values
//
// By default, a NSToolTip associated with the NSToolTip will:
//   - be active regardless of whether its owner is active or inactive
//
NSToolTip::NSToolTip(NSContexte *pCtx, TWindow* parent, bool alwaysTip, TModule* module)
          :TTooltip(parent, alwaysTip, module), NSRoot(pCtx)
{
}

//
// Constructor to alias a non-OWL TOOLTIP control. Specially useful when
// used with controls that automatically create a tooltip (eg TabControls
// with TCS_TOOLTIPS style).
//
NSToolTip::NSToolTip(NSContexte *pCtx, THandle handle, TModule* module)
          :TTooltip(handle, module), NSRoot(pCtx)
{
}

//
// Destructor of NSToolTip class.
//
NSToolTip::~NSToolTip()
{
}

void
NSToolTip::ShowTip(const NS_CLASSLIB::TPoint& pt)
{
	// create a tooltip window
	if (!IsWindow())
	{
		DWORD dwStyle = TTS_ALWAYSTIP ;

		PRECONDITION((Parent == NULL) || IsWindow(Parent->GetSafeHwnd())) ;
		if (Parent->IsWindow())
			dwStyle |= WS_CHILD ;

    SetStyle(dwStyle | GetStyle()) ;
    SetExStyle(WS_EX_TOPMOST | GetExStyle()) ;

    Create() ;
    
		// CreateEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, dwStyle,
		//				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		//				m_pParent->GetSafeHwnd(), NULL, NULL));
	}

	// initialize members of the TOOLINFO structure
	_ti.cbSize = sizeof(TOOLINFO);
	_ti.uFlags = TTF_TRACK;
	_ti.hwnd   = NULL ;
	/*if (AfxIsValidString(lpszText))*/
		_ti.hinst = NULL;
	_ti.uId = 0;
	_ti.lpszText = (LPTSTR)(LPCTSTR) _sText.c_str() ;

	// tooltip control will cover the whole window
	_ti.rect.left   = 0;
	_ti.rect.top    = 0;
	_ti.rect.right  = 0;
	_ti.rect.bottom = 0;

	if (IsWindow())
	{
		// send an TTM_ADDTOOL message to the tooltip control window
		::SendMessage(HWindow, TTM_ADDTOOL, 0, (LPARAM)&_ti);

		// multiline support
		::SendMessage(HWindow, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);

		::SendMessage(HWindow, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x, pt.y));
		::SendMessage(HWindow, TTM_TRACKACTIVATE, TRUE, (LPARAM)&_ti);

		// force visibility
		if (!IsWindowVisible())
			ShowWindow(SW_SHOWNA);
	}
}

void
NSToolTip::ShowTip(const NS_CLASSLIB::TPoint& pt, string sText)
{
	_sText = sText ;
	ShowTip(pt) ;
}

void
NSToolTip::ShowTip(const NS_CLASSLIB::TPoint& pt, UINT nIDText)
{
	// This would restrict the tooltip to 80 characters.
	/*m_ti.hinst = AfxFindResourceHandle(MAKEINTRESOURCE((nIDText>>4)+1),
RT_STRING);
	ASSERT(m_ti.hinst != NULL);

	ShowTip(pt, MAKEINTRESOURCE(nIDText));*/

	// m_strText.LoadString(nIDText) ;
	ShowTip(pt) ;
}

void
NSToolTip::HideTip()
{
	if (IsWindow())
	{
		::SendMessage(HWindow, TTM_TRACKACTIVATE, FALSE, (LPARAM) &_ti) ;

		// send an TTM_DELTOOL message to the tooltip control window
		::SendMessage(HWindow, TTM_DELTOOL, 0, (LPARAM) &_ti) ;
	}
}

void NSToolTip::PreSetParent(TWindow* pParent)
{
	PRECONDITION(((TWindow*) NULL == Parent) || Parent->IsWindow());
	Parent = pParent ;

	DestroyWindow(HWindow) ;
}

