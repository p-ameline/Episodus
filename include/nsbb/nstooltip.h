#ifndef __NSTOOLTIP_H#define __NSTOOLTIP_H

#include <owl\tooltip.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsbb\nsbb_glo.h"

#define TTS_BALLOON 0x40

class _NSBBCLASSE NSToolTip : public TTooltip, public NSRoot
{
  protected:

    TOOLINFO _ti ;
	  string   _sText ;

	  void ShowTip(const NS_CLASSLIB::TPoint& pt) ;

	public:

    // Constructeur et destructeur
    NSToolTip(NSContexte *pCtx, TWindow* parent, bool alwaysTip = true, TModule* module = 0) ;
    NSToolTip(NSContexte *pCtx, HWND hWnd, TModule* module = 0);
    virtual ~NSToolTip() ;

	  void ShowTip(const NS_CLASSLIB::TPoint& pt, string sText) ;
	  void ShowTip(const NS_CLASSLIB::TPoint& pt, UINT nID) ;
	  void HideTip() ;
	  void PreSetParent(TWindow* pParent) ;

	// DECLARE_RESPONSE_TABLE(NSToolTip) ;
};

#endif   // __NSTOOLTIP_H

