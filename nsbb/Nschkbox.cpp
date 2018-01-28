#include <bwcc.h>#include "nautilus\nssuper.h"
#include "nsbb\nschkbox.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsdlg.h"

DEFINE_RESPONSE_TABLE1(NSCheckBox, TCheckBox)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
  EV_MESSAGE(WM_CTLCOLORSTATIC, EvSetBackColor),
  EV_WM_KEYUP,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSCheckBox::NSCheckBox(NSContexte *pCtx, TWindow* parent, int resId, TGroupBox* group, bool bStandardStyle, string sBackColor)
           :TCheckBox(parent, resId, group), NSRoot(pCtx)
{
  pControle = (NSControle*) 0 ;
  ActiveToi = true ;
	DisableTransfer() ;

  _bIsEmpty = true ;

  _bStandardStyle = bStandardStyle ;
  _sBackColor     = sBackColor ;
}

NSCheckBox::NSCheckBox(NSContexte *pCtx, TWindow* parent, int resId, const char far* title,
                             int x, int y, int w, int h, TGroupBox *group, bool bStandardStyle, string sBackColor,
                             TModule* module)
           :TCheckBox(parent, resId, title, x, y, w, h, group, module), NSRoot(pCtx)
{
  pControle = (NSControle*) 0 ;
  ActiveToi = true ;
	DisableTransfer() ;
  //
  // Attention, le constructeur de TCheckBox employé ici attribue le style
  // BS_AUTOCHECKBOX, que nous remplaçons par BS_CHECKBOX
  //
  Attr.Style    = (Attr.Style ^ BS_AUTOCHECKBOX) | BS_CHECKBOX ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;

  _bIsEmpty = true ;
  _bStandardStyle = bStandardStyle ;
  _sBackColor     = sBackColor ;

  if ((string("") != sBackColor) || _bStandardStyle)
    Attr.Style = Attr.Style | BS_OWNERDRAW ;
}

//
// Destructeur
//
NSCheckBox::~NSCheckBox()
{
  if ((pControle) && (pControle->getControle()))
    pControle->setControle(0) ;
}

void
NSCheckBox::SetupWindow()
{
	TCheckBox::SetupWindow() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

bool
NSCheckBox::canWeClose()
{
	return true ;
}

void
NSCheckBox::activeControle(int activation, Message* /* pMessage */)
{
	SetCheck(activation) ;
}

//
// Fonction déclenchée lorsque la boite à cocher est activée.
//void
NSCheckBox::BNClicked()
{
  _bIsEmpty = false ;

  if (pControle && pControle->getNSDialog())
    pControle->getNSDialog()->setLastAction(NSDialog::actionControl) ;

  if (false == ActiveToi)
  {
    ActiveToi = true ;
    return ;
  }

  // If disconnected, don't react
  if (pControle && pControle->isDisconnected())
    return ;

	//
	// Prévient Big Brother
	//
  if (GetCheck() == BF_CHECKED)
  {
    if (pControle && pControle->getTransfert())
      pControle->getTransfert()->ctrlNotification(BF_CHECKED, BF_UNCHECKED) ;
  }
	else if (pControle)
  {
    if (pControle->getFonction())
      pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;

    if (pControle->getTransfert())
      pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED) ;

    if (pControle->getFonction())
      pControle->getFonction()->execute(NSDLGFCT_POSTEXEC) ;
  }

  string sEtiquette = string("") ;
  if (pControle && pControle->getTransfert())
    sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

	TCheckBox::BNClicked() ;
}

//---------------------------------------------------------------------------
//  Function: 		NSCheckBox::Transfer(TTransferDirection direction,
//																int* pActif, string* pMessage)
//
//  Description:	Si direction = tdSetData
//							Initialise le contrôle
//						Si direction = tdGetData
//							Initialise pActif et pMessage
//
//  Arguments:		direction : tdGetData ou tdSetData
//
//  Returns:		1 si le transfert a fonctionné
//						0 sinon
//---------------------------------------------------------------------------
uint
NSCheckBox::Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif,
                                                            Message* pMessage)
{
	if (tdSetData == direction)
	{
    switch (*pActif)
    {
      case inactiveCtrl : SetCheck(BF_UNCHECKED) ; break ;
      case activeCtrl   : SetCheck(BF_CHECKED) ;   break ;
      default           : SetCheck(BF_GRAYED) ;    break ;
		}
	}
	else if (tdGetData == direction)
	{
		switch (GetCheck())
    {
      case BF_UNCHECKED : *pActif = inactiveCtrl ; break ;
      case BF_CHECKED   : *pActif = activeCtrl ;   break ;
      case BF_GRAYED 	  : *pActif = disabledCtrl ; break ;
		}

    if (pMessage)
    {
      string sEtiquette = string("") ;
      if (pControle && pControle->getTransfert())
        sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

      pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
    }
  }
  return 1 ;
}

uint
NSCheckBox::TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage)
{
  switch (GetCheck())
  {
    case BF_UNCHECKED : *pActif = inactiveCtrl ; break ;
    case BF_CHECKED   : *pActif = activeCtrl ;   break ;
    case BF_GRAYED 	  : *pActif = disabledCtrl ; break ;
  }

  if (pMessage)
  {
    string sEtiquette = string("") ;
    if (pControle && pControle->getTransfert())
      sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

    pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
  }

  return 1 ;
}

void
NSCheckBox::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  // Gestion windows standard
  // Windows standard behaviour
  if (pControle && pControle->getNSDialog())
  {
    // return TEdit::EvKeyUp(key, repeatcount, flags) ;
    TCheckBox::EvKeyUp(key, repeatcount, flags) ;
    return ;
  }

  // THandle NextControl = 0;
  switch (key)
  {
    //return
    case VK_DOWN   :
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // suivant pControle sinon le premier
      //
      if (pControle && pControle->getNSDialog())
        pControle->getNSDialog()->ActiveControlSuivant(pControle) ;
      else if (pControle && pControle->getMUEView())
        pControle->getMUEView()->SetFocusToNextControl((TControl*)this) ;
      break ;

    case VK_UP    :
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // précédent pControle sinon le dernier
      //
      if (pControle && pControle->getNSDialog())
        pControle->getNSDialog()->ActiveControlPrecedent(pControle) ;
      else if (pControle && pControle->getMUEView())
        pControle->getMUEView()->SetFocusToPreviousControl((TControl*)this) ;
      break ;

    //tabulation
    case VK_TAB	:
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // suivant pControle sinon le premier
      //
      if (pControle && pControle->getNSDialog())
        pControle->getNSDialog()->ActiveControlSuivant(pControle) ;
      else if (pControle && pControle->getMUEView())
      {
        if (GetKeyState(VK_SHIFT) < 0)
          pControle->getMUEView()->SetFocusToPreviousControl((TControl*)this) ;
        else
          pControle->getMUEView()->SetFocusToNextControl((TControl*)this) ;
      }
      break ;

    default		:
      TCheckBox::EvKeyUp(key, repeatcount, flags) ;
      return ;
  }
}

//
// Return name of predefined BWCC or Windows check box class
//
char far*
NSCheckBox::GetClassName()
{
  if ((false == _bStandardStyle) && (GetApplication()->BWCCEnabled()))
    return CHECK_CLASS ;

  return "BUTTON" ;
}

TResult
NSCheckBox::EvSetBackColor(TParam1 param1, TParam2 param2)
{
  HBRUSH hbr = (HBRUSH)DefWindowProc(GetCurrentEvent().Message, param1, param2) ;
  ::DeleteObject(hbr) ;
  SetBkMode((HDC)param1, TRANSPARENT) ;
  return (TResult)::GetStockObject(NULL_BRUSH) ;
}

/*

  int  ctlType = GetCurrentEvent().Message - WM_CTLCOLORMSGBOX;

  CHECK(ctlType >= CTLCOLOR_MSGBOX && ctlType <= CTLCOLOR_STATIC);

  TEventInfo  eventInfo(WM_CTLCOLOR);

  if (Find(eventInfo)) {
    typedef HBRUSH(GENERIC::*CTL_COLOR_PMF)(HDC, HWND, uint);

    CTL_COLOR_PMF&  pmf = (CTL_COLOR_PMF&)(eventInfo.Entry->Pmf);

    return (TResult)(eventInfo.Object->*pmf)((HDC)param1, (HWND)param2, ctlType);
  }
  return DefWindowProc(GetCurrentEvent().Message, param1, param2);
}

if( checkBoxButton == (HWND)lParam)

    {

        HBRUSH hbr = (HBRUSH)DefWindowProc(hwnd, message, wParam, lParam);

        ::DeleteObject(hbr);

        SetBkMode((HDC)wParam, TRANSPARENT);

        return (LRESULT)::GetStockObject(NULL_BRUSH);

    }
*/

