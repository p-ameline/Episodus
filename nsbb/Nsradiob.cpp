#include <windows.h>#include <bwcc.h>
#include "nautilus\nssuper.h"#include "nsbb\nsradiob.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsdlg.h"

DEFINE_RESPONSE_TABLE1(NSRadioButton, TRadioButton)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
  EV_WM_KEYUP,
END_RESPONSE_TABLE;

//
// Constructeurs
//
NSRadioButton::NSRadioButton(NSContexte *pCtx, TWindow* parent, int resId, TGroupBox* group)
              :TRadioButton(parent, resId, group), NSRoot(pCtx)
{
  pControle = (NSControle*) 0 ;
  ActiveToi = true ;
	DisableTransfer() ;
  _bIsEmpty = true ;
}

NSRadioButton::NSRadioButton(NSContexte *pCtx, TWindow* parent, int resId, const char far* title,
                             int x, int y, int w, int h, TGroupBox *group,
                             TModule* module)
              :TRadioButton(parent, resId, title, x, y, w, h, group, module), NSRoot(pCtx)
{
  pControle = (NSControle*) 0 ;
  ActiveToi = true ;
	DisableTransfer() ;
  //
  // Attention, le constructeur de TRadioButton employé ici attribue le style
  // BS_AUTORADIOBUTTON, que nous remplaçons par BS_RADIOBUTTON
  //
  Attr.Style = (Attr.Style ^ BS_AUTORADIOBUTTON) | BS_RADIOBUTTON ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
  _bIsEmpty = true ;
}

char far*
NSRadioButton::GetClassName()
{
  return RADIO_CLASS ;
}

void
NSRadioButton::GetWindowClass(WNDCLASS far& wc)
{
  TWindow::GetWindowClass(wc) ;
  // static TBrush yellow(TColor::LtYellow);
  // wc.hbrBackground = yellow;
  // wc.lpszClassName = RADIO_CLASS;
  // wc.style |= BS_RADIOBUTTON;
}

void
NSRadioButton::SetupWindow()
{
  TRadioButton::SetupWindow() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

bool
NSRadioButton::canWeClose()
{
  return true ;
}

//
// Destructeur
//
NSRadioButton::~NSRadioButton()
{
  if ((pControle) && (pControle->getControle()))
    pControle->setControle(0) ;
}

//
// Fonction déclenché lorsque la boite à cocher est activée.
//
void
NSRadioButton::BNClicked()
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

	Message Msg ;

	int iNotifier = 1 ;	if (GetCheck() == BF_CHECKED)
	{
  	if (pControle && pControle->getTransfert())
    	pControle->getTransfert()->ctrlNotification(BF_CHECKED, BF_UNCHECKED, &Msg) ;
  }
  else
  {
  	if (pControle && pControle->getFonction() && iNotifier)
    	iNotifier = pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;

    if (pControle && pControle->getTransfert())
    	pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, &Msg) ;
	}

	TRadioButton::BNClicked() ;

	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (pControle && pControle->getFonction() && iNotifier)
		pControle->getFonction()->execute(NSDLGFCT_POSTEXEC) ;}

//---------------------------------------------------------------------------
//  Function: 		NSRadioButton::Transfer(TTransferDirection direction,
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
NSRadioButton::Transferer(TTransferDirection direction,
                                            CTRL_ACTIVITY* pActif, Message* pMessage)
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
      case BF_CHECKED 	: *pActif = activeCtrl ;   break ;
      case BF_GRAYED 	  : *pActif = disabledCtrl ; break ;
    }

    if (pMessage)
    {
    	string sEtiquette = string("") ;

      if (pControle && pControle->getTransfert())
      	sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

      if (pMessage)
      	pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
    }
	}
	return 1 ;
}

uint
NSRadioButton::TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage)
{
	switch (GetCheck())
	{
  	case BF_UNCHECKED : *pActif = inactiveCtrl ; break ;
    case BF_CHECKED   : *pActif = activeCtrl ;   break ;
    case BF_GRAYED    : *pActif = disabledCtrl ; break ;
	}

	if (pMessage)
	{
		string sEtiquette = string("") ;

    if (pControle && pControle->getTransfert() && pControle->getTransfert()->getFilsItem())
    	sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

    pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
	}
	return 1 ;
}

void
NSRadioButton::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  // Gestion windows standard
  // Windows standard behaviour
  if (pControle->getNSDialog())
  {
    // return TEdit::EvKeyUp(key, repeatcount, flags) ;
    TRadioButton::EvKeyUp(key, repeatcount, flags) ;
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
        pControle->getMUEView()->SetFocusToNextControl((TControl*) this) ;
      break ;

    case VK_UP    :
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // précédent pControle sinon le dernier
      //
      if (pControle && pControle->getNSDialog())
        pControle->getNSDialog()->ActiveControlPrecedent(pControle) ;
      else if (pControle && pControle->getMUEView())
        pControle->getMUEView()->SetFocusToPreviousControl((TControl*) this) ;
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
          pControle->getMUEView()->SetFocusToPreviousControl((TControl*) this) ;
        else
          pControle->getMUEView()->SetFocusToNextControl((TControl*) this) ;
      }
      break ;

    default		:
      //return TEdit::EvKeyUp(key, repeatcount, flags);
      TRadioButton::EvKeyUp(key, repeatcount, flags) ;
      return ;
  }
}

