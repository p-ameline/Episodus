#include <owl/pch.h>#include <owl/tabctrl.h>
#include <owl/inputdia.h>
#include <owl/notetab.h>
#include <owl/commctrl.h>

#include "partage\nsglobal.h"
#include "nsbb\nsbb_msg.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsonglet.h"

DEFINE_RESPONSE_TABLE1(NSOnglet, TTabControl)
	// Tab Control Notifications
	//
	//EV_NOTIFY_AT_CHILD(TCN_SELCHANGE, TabSelChange),
	//EV_NOTIFY_AT_CHILD(TCN_SELCHANGING, TabSelChanging),
	//EV_NOTIFY_AT_CHILD(EV_TCN_KEYDOWN, TabKeyDown),
	EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE;

//// Constructeur
//
NSOnglet::NSOnglet(NSContexte *pCtx, TWindow* parent, int resId)
         :TTabControl(parent, resId), NSRoot(pCtx)
{
	_pControle    = (NSControle*) 0 ;
  _iOngletActif = -1 ;
  _iLevel       = -1 ;

	DisableTransfer() ;
}

NSOnglet::NSOnglet(NSContexte *pCtx, TWindow* parent, int resId, int x, int y, int w, int h, TModule* module)         :TTabControl(parent, resId, x, y, w, h, module), NSRoot(pCtx){	_pControle    = (NSControle*) 0 ;  _iOngletActif = -1 ;  _iLevel       = -1 ;	DisableTransfer() ;}
//// Destructeur
//
NSOnglet::~NSOnglet()
{
 /*	if (pControle)
	{
		delete pControle;
		pControle = 0;
	} */
}

void
NSOnglet::SetupWindow()
{
  // This method is private
  // OWL::TTabControl::SetupWindow() ;

  TControl::SetupWindow() ;
}

voidNSOnglet::InitialiseTabs(NSDialogOnglet* pConfigOnglet)
{
  if ((NSDialogOnglet*) NULL == pConfigOnglet)
    return ;

	int iTabIndex ;
	for (int i = 0 ; i < pConfigOnglet->nNbOnglets ; i++)
	{
    string* psCurrentText = pConfigOnglet->pTexteOnglets[i] ;

  	iTabIndex = Add(psCurrentText->c_str()) ;
    if (*psCurrentText == pConfigOnglet->sOngletActif)
    {
    	SetSel(iTabIndex) ;
      _iOngletActif = iTabIndex ;
    }
	}
}

//// Fonction utilisée lorsque l'onglet est cliqué
//
void
NSOnglet::EvLButtonDown(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	int iNotifier = 1 ;
	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;
	//
	// Prévient Big Brother (si l'éventuelle fonction l'autorise)
	//
	if (1 == iNotifier)
	{
  	TTabHitTestInfo TabInfo ;
    TabInfo.pt = point ;
		int iOngletClick = HitTest(TabInfo) ;
    if ((-1 != iOngletClick) && (iOngletClick != _iOngletActif))
    {
      int iDirectIndex = NSDLGRETOUR_DIRECT + iOngletClick ;
      if (_iLevel > 0)
        iDirectIndex += (_iLevel - 1) * NSDLGRETOUR_SEPARATOR ;

			_pControle->getNSDialog()->CmOkMessage(iDirectIndex) ;
    }
	}
	//
	// Prévient la boite de dialogue
	//
	/*#if defined(__WIN32__)
		Parent->HandleMessage(WM_CORRECT, MAKEWPARAM(Attr.Id, WM_CORRECT),
									 LPARAM(HWindow));
	#else
		Parent->HandleMessage(WM_CORRECT, Attr.Id, MAKELPARAM(HWindow, WM_CORRECT));
	#endif */
}

