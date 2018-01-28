#include <owl\owlpch.h>#include <owl\applicat.h>#include <owl\button.h>
#include <owl\checkbox.h>
#include <bwcc.h>

//#include "ns_vector.h"
#include "partage\nsglobal.h"
#include "nautilus\nssuper.h"
#include "nsbb\nsbouton.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsbb.rh"
DEFINE_RESPONSE_TABLE1(NSButton, TButton)	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),END_RESPONSE_TABLE;

//
// Constructeur
//
NSButton::NSButton(NSContexte *pCtx, TWindow* parent, int resId)
         :TButton(parent, resId), NSRoot(pCtx)
{
	pControle = (NSControle*) 0 ;
  ActiveToi = true ;
	DisableTransfer() ;
}

NSButton::NSButton(NSContexte *pCtx, TWindow* parent, int resId, const char far* text,
                 int X, int Y, int W, int H, bool isDefault, TModule* module)
         :TButton(parent, resId, text, X, Y, W, H, isDefault, module), NSRoot(pCtx)
{
	pControle = (NSControle*) 0 ;
  ActiveToi = true ;
	DisableTransfer() ;
}

//
// Destructeur
//
NSButton::~NSButton()
{
  if (pControle && pControle->getControle())
    pControle->setControle(0) ;

  /*if (pControle)
	{
		delete pControle;
		pControle = 0;
	}*/
}

void
NSButton::SetupWindow()
{
  OWL::TButton::SetupWindow() ;
}

//
// Fonction utilisée lorsque le bouton est cliqué
//
void
NSButton::BNClicked()
{
  if (GetDlgCtrlID() == IDBBKCALCUL)
  {
    Parent->SendMessage(WM_COMMAND, IDBBKCALCUL) ;
    return ;
  }

	if (false == ActiveToi)
  {
    ActiveToi = true ;
    return ;
  }
	int iNotifier = 1 ;
  Message Msg ;

  if (pControle)
  {
    //
    // Si une fonction est attachée au contrôle, on l'exécute
    //
    if (pControle->getFonction())
      iNotifier = pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;
    //
    // Prévient Big Brother (si l'éventuelle fonction l'autorise)
    //
    if (1 == iNotifier)
    {
      if (pControle->getTransfert())
        pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, &Msg) ;

      // In case it is a Free Text button, transfert can be captured by the
      // Free Text Checked button
      //
      else if ((GetId() == IDTL) || (GetId() == IDTL_ACTIF))
      {
        if (pControle->getNSDialog())
        {
          NSButton* pStandardButton = (NSButton*) 0 ;
          NSButton* pCheckedButton  = (NSButton*) 0 ;
          pControle->getNSDialog()->getFreeTextButtons(&pStandardButton, &pCheckedButton) ;
          NSControle* pOtherControl = (NSControle*) 0 ;
          if ((GetId() == IDTL) && pCheckedButton)
            pOtherControl = pCheckedButton->pControle ;
          if ((GetId() == IDTL_ACTIF) && pStandardButton)
            pOtherControl = pStandardButton->pControle ;

          if (pOtherControl && (pOtherControl->getTransfert()))
            pOtherControl->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, &Msg) ;
        }
      }
    }

    if (pControle->getFonction())      iNotifier = pControle->getFonction()->execute(NSDLGFCT_POSTEXEC) ;
  }
}

boolNSButton::canWeClose()
{
  return true ;
}
void
NSButton::activeControle(int activation, Message* pMessage)
{
  if ((Message*) NULL == pMessage)
    return ;

  string sLexCodeSens = pMessage->GetLexique() ;
  pContexte->getDico()->donneCodeSens(sLexCodeSens) ;

  switch (activation)
	{
    //
    // activating
    //
    case BF_CHECKED	:

      if ((GetId() == IDTL) || (GetId() == IDTL_ACTIF))
        pControle->getNSDialog()->checkFreeText() ;

      if (string("") == sLexCodeSens)
        return ;

      if (string("£SGDR") == sLexCodeSens) // person
      {
        string sComplement = pMessage->GetComplement() ;
        if (string("") == sComplement)
          return ;

        if ((NSPersonArray*) NULL == pContexte->getPersonArray())
          return ;
        NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sComplement, pidsCorresp) ;
        if ((NSPersonInfo*) NULL == pPersonInfo)
          return ;

        string sIdentite = pPersonInfo->getNom() + string(" ") + pPersonInfo->getPrenom() ;

        if (string("") != sIdentite)
          SetCaption(sIdentite.c_str()) ;
      }
      break;

		case BF_UNCHECKED :      if ((GetId() == IDTL) || (GetId() == IDTL_ACTIF))        pControle->getNSDialog()->uncheckFreeText() ;
      break ;
  }
}

char far*
NSButton::GetClassName()
{
#if defined(BI_COMP_BORLANDC)
  if (GetApplication()->BWCCEnabled() &&
          ((GetId() == IDTL) || (GetId() == IDTL_ACTIF))) {
    return BUTTON_CLASS ;
  }
#endif

  return "BUTTON" ;
}

