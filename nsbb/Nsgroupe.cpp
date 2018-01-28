#include "nautilus\nssuper.h"#include "nsbb\nsdlg.h"
#include "nsbb\nsgroupe.h"

DEFINE_RESPONSE_TABLE1(NSGroupBox, TGroupBox)
	//EV_NOTIFY_AT_CHILD(WM_KILLFOCUS, EvKillFocus),
	//EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSGroupBox::NSGroupBox(NSContexte *pCtx, TWindow* parent, int resId)
           :TGroupBox(parent, resId), NSRoot(pCtx)
{
	pControle = 0 ;
	DisableTransfer() ;
}

NSGroupBox::NSGroupBox(NSContexte *pCtx, TWindow* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module)           :TGroupBox(parent, resId, text, x, y, w, h, module), NSRoot(pCtx){  pControle = 0 ;	DisableTransfer() ;}
//// Destructeur
//
NSGroupBox::~NSGroupBox()
{
  /*	if (pControle)
	{
		delete pControle;
		pControle = 0;
	}*/
}

// SetupWindow : on fixe la police du dialogue
void
NSGroupBox::SetupWindow()
{
	TGroupBox::SetupWindow() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

//
// Fonction déclenché lorsque la boite à cocher est activée.
//
void
NSGroupBox::SelectionChanged(int /* controlId */)
{
	//
	// Prévient Big Brother
	//
	/*if (GetCheck() == BF_CHECKED)
		pControle->pTransfert->ctrlNotification(BF_CHECKED, BF_UNCHECKED, "");
	else
		pControle->pTransfert->ctrlNotification(BF_UNCHECKED, BF_CHECKED, "");*/
	//
	// Prévient la boite de dialogue
	//
	#if defined(__WIN32__)
		Parent->HandleMessage(WM_CORRECT, (MAKEWPARAM(Attr.Id, WM_CORRECT)),
									 LPARAM(HWindow));
	#else
		Parent->HandleMessage(WM_CORRECT, Attr.Id, MAKELPARAM(HWindow, WM_CORRECT));
	#endif
	//TGroupBox::EvKillFocus(hWndGetFocus);
}

//---------------------------------------------------------------------------
//  Function: 		NSGroupBox::Transfer(TTransferDirection direction,
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
NSGroupBox::Transferer(TTransferDirection /* direction */,
                            CTRL_ACTIVITY* /* pActif */, Message * /* pMessage */)
{
	/*if (direction == tdSetData)
	{
		switch (*pActif) {
				case 	0 : SetCheck(BF_UNCHECKED); break;
				case 	1 : SetCheck(BF_CHECKED); 	 break;
				default : SetCheck(BF_GRAYED); 	 break;
		}
	}
	else if (direction == tdGetData)
	{
		switch (GetCheck()) {
				case BF_UNCHECKED : *pActif =  0; break;
				case BF_CHECKED 	: *pActif =  1; break;
				case BF_GRAYED 	: *pActif = -1; break;
		}
		*pMessage = "";
	} */
	return 1 ;
}

bool
NSGroupBox::canWeClose()
{
  return true ;
}

