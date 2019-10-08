//#include <checks.h>
#include <owl\control.h>
#include <owl\dc.h>
#include <owl\edit.h>

#include "nssavoir\nspathor.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsedilex.h"
#include "nsbb\nStlibre.h"
#include "partage\nsdivfct.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nscombo.h"
#include "nsbb\nsdlg.h"

#include "nsbb\nsbb.rh"

//
// General Controls diagnostic group
//
DIAG_DEFINE_GROUP_INIT(OWL_INI, OwlControl, 1, 0);

DEFINE_RESPONSE_TABLE1(NSEditLexique, NSEditDicoGlobal)
  EV_WM_CHAR,
  EV_WM_KEYDOWN,
  EV_WM_KEYUP,
  EV_WM_GETDLGCODE,
	EV_WM_KILLFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_EDITPASTE, CmEditPaste),
   //EV_NOTIFY_AT_CHILD(WM_KEYDOWN, EvKeyDown),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSEditLexique::NSEditLexique(TWindow* parent, NSContexte* pCtx, int resId,
                             NSDico* pDictio, uint textLen, string sTypeLexique,
                             TModule* module)
              :NSEditDicoGlobal(pCtx, parent, resId, pDictio, sTypeLexique, textLen, module)
{
  pControle       = (NSControle*) 0 ;
  sContenuPere    = string("") ;
  pContexte       = pCtx ;
  pNSEdit         = (NSEditLexiqueDerive*) 0 ;
  sComplement     = string("") ;
  bWinStd         = false ;
  _bIsFreeTextEnabled = true ;
}

NSEditLexique::NSEditLexique(TWindow* parent, NSContexte* pCtx, int resourceId,                             NSDico* pDictio, const char far* text,
                             int x, int y, int w, int h, uint textLimit,
                             bool multiline, string sTypeLexiq, TModule* module)
              :NSEditDicoGlobal(pCtx, parent,resourceId, pDictio, text, x, y, w, h,
                                textLimit, multiline, sTypeLexiq, module)
{
  pControle       = (NSControle*) 0 ;
  pContexte       = pCtx ;
  sContenuPere    = string("") ;
  pNSEdit         = (NSEditLexiqueDerive*) 0 ;
  sComplement     = string("") ;
  bWinStd         = false ;

  _bIsFreeTextEnabled = true ;
}

NSEditLexique::~NSEditLexique(){
  if (pNSEdit)
    delete pNSEdit ;
}

//----------------------------------------------------------------------------//----------------------------------------------------------------------------
void
NSEditLexique::SetupWindow()
{
  NSEditDicoGlobal::SetupWindow() ;
}

//----------------------------------------------------------------------------// positionner le lexique selon les coordonnées de l'écran et celles de la boîte
// mère
//----------------------------------------------------------------------------
void
NSEditLexique::SetPositionLexique()
{
try
{
	//
  // coordonnées de l'écran en pixels
  //
  TIC* pTic = new TIC("DISPLAY", 0, 0) ;
  int XScreen = pTic->GetDeviceCaps(HORZRES) ;
  int YScreen = pTic->GetDeviceCaps(VERTRES) ;
  delete pTic ;

	//position du lexique
  NS_CLASSLIB::TRect rectBoiteMere = Parent->GetWindowRect() ; //boîte mère
  NS_CLASSLIB::TRect rectEdition   = GetWindowRect() ; //rectangle d'édition (this)

  //  // Attr : this
  //
  int x = Attr.X + Attr.W + rectBoiteMere.left  + 5 ;
  int y = rectEdition.top ;

  NS_CLASSLIB::TRect rectLex = _pDico->getDicoDialog()->GetWindowRect() ; //lexique
  if ((x + rectLex.Width()) >= XScreen)
    x = rectEdition.left  - rectLex.Width() - 5 ;

  if ((y + rectLex.Height()) >= YScreen)
    y = YScreen - rectLex.bottom + rectLex.top ;

  _pDico->getDicoDialog()->SetWindowPos(0, x, y, rectLex.Width(), rectLex.Height(), SWP_NOZORDER) ;

  //
  // coordonnées de l'écran
  //
   /*   // Device metrics for screen
		HDC hDCScreen = GetDC(NULL);
		//ASSERT(hDCScreen != NULL);
		rectEdition.right = GetDeviceCaps(hDCScreen, HORZRES);
		rectEdition.bottom = GetDeviceCaps(hDCScreen, VERTRES);
		ReleaseDC(NULL, hDCScreen);
   */
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSEditLexique::SetPositionLexique : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSEditLexique::SetPositionLexique.", standardError, 0) ;
}
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSEditLexique::UpdateDico()
{
  //
  // Récupèration du contenu du contrôle
  //
  string sContenu = getTextAsString() ;
  strip(sContenu) ;
  //
  // Transmission du contenu à la boite de dialogue
  //
	int iOouF = _pDico->getDicoDialog()->DonneAmmorce(&sContenu) ;
  if (1 == iOouF)
  {
    // bGardeFocus = true ;
    //
    // fixer la position du lexique
    //
    SetPositionLexique() ;
		_pDico->getDicoDialog()->montrerDialogue() ;
    SetFocus() ;
  }
  else if (2 == iOouF)
    _pDico->getDicoDialog()->cacherDialogue() ;
}

//-------------------------------------------------------------------------//-------------------------------------------------------------------------
void
NSEditLexique::EvChar(uint key, uint repeatCount, uint flags)
{
  if ((VK_RETURN == key) || (VK_TAB == key) || (VK_ESCAPE == key))
    //pour enlever le Beep ne pas appliquer TEdit::EvChar(key, repeatCount, flags)
    return ;

  if (pControle && pControle->isDisconnected())
    return ;

  TEdit::EvChar(key, repeatCount, flags) ;
  UpdateDico() ;
}

//---------------------------------------------------------------------------//---------------------------------------------------------------------------
void
NSEditLexique::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  TEdit::EvKeyDown(key, repeatCount, flags) ;
  Parent->SendNotification(Attr.Id, 200, HWindow) ;
  //
  // Transmission du contenu à la boite de dialogue
  //

  // flêche basse
  //
  if (VK_DOWN == key)
  {
    if (string("") == _pDico->getDicoDialog()->getAmmorce())
      return ;
    else
      _pDico->getDicoDialog()->ElementSuivant() ;
  }
  //
  // flêche haute
  //
  else if (VK_UP == key)
  {
    //si ammorce vide alors passer au NStrreNode suivant
    if (string("") == _pDico->getDicoDialog()->getAmmorce())
      return ;
    else
      _pDico->getDicoDialog()->ElementPrecedent() ;
  }
  else if ((VK_INSERT == key) || (VK_RETURN == key))
    _pDico->getDicoDialog()->InsererElementLexique() ;
  else if (VK_TAB == key)
  {
    if (bWinStd)
    {
      THandle hNextTab ;
      if (GetKeyState(VK_SHIFT) < 0)
        hNextTab = Parent->GetNextDlgTabItem(((TWindow*)this)->GetHandle(), /*bool previous=*/ true) ;
      else
        hNextTab = Parent->GetNextDlgTabItem(((TWindow*)this)->GetHandle(), /*bool previous=*/ false) ;
      _bGardeFocus = false ;
      ::SetFocus(hNextTab) ;
    }
  }
  else
    UpdateDico() ;
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
void
NSEditLexique::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  // Gestion windows standard
  // Windows standard behaviour
  if (pControle && pControle->getNSDialog())
  {
    NSContexte* pContexte = pControle->getNSDialog()->pContexte ;
    if (pContexte && pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
    {
      TEdit::EvKeyUp(key, repeatcount, flags) ;
      return ;
    }
  }

  switch (key)
  {
    //return
    case VK_RETURN :
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // suivant pControle sinon le premier
      //
            /*if (pControle && pControle->getNSDialog())
                pControle->getNSDialog()->ActiveControlSuivant(pControle);
            else
            {
                if (parentEdit->Parent->ChildWithId(1))
                    parentEdit->Parent->ChildWithId(1)->SetFocus();
                else
                    parentEdit->CloseWindow(IDOK);
            } */
      break ;

    case VK_ESCAPE :
      break ;

    //tabulation
    case VK_TAB	:
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // suivant pControle sinon le premier
      //
      if (pControle && pControle->getNSDialog() && (!bWinStd))
      {
        pControle->getNSDialog()->ActiveControlSuivant(pControle) ;
        break ;
      }

    default		:
      TEdit::EvKeyUp(key, repeatcount, flags) ;
  }
}

//fonction permettant de voir passer toutes les touchesuint
NSEditLexique::EvGetDlgCode(MSG far* msg)
{
  uint retVal = (uint) DefaultProcessing() ;

  if (msg && (VK_RETURN == msg->wParam))
  {
    if (_pDico && _pDico->getDicoDialog() && _pDico->getDicoDialog()->IsWindow())
      _pDico->getDicoDialog()->InsererElementLexique() ;
    // retVal |= DLGC_WANTCHARS ;
    retVal |= DLGC_WANTALLKEYS ;
  }
  return retVal ;
}

void
NSEditLexique::CmEditPaste()
{
	TEdit::CmEditPaste() ;
}

//-------------------------------------------------------------------------
// 					l'utilisateur a choisi un élément dans le lexique
//-------------------------------------------------------------------------
void
NSEditLexique::ElementSelectionne()
{
try
{
	char code[PATHO_CODE_LEN + 1] ;
	GetCodeLexiqueChoisi(code) ;
	string sContenu ;
	GetLabelChoisi(&sContenu) ;
	SetText(sContenu.c_str()) ;
	_pDico->getDicoDialog()->cacherDialogue() ;

  if ((NULL == pControle) || (NULL == pControle->getTransfert()) || (NULL == pControle->getTransfert()->getFilsItem()))
	{
		erreur("Non connected control.", standardError, 0) ;
		return ;
	}
	sContenuPere = sContenu ;

/*
  BBItem* pBBItemPere = pControle->getTransfert()->pBBFilsItem->pPere ;
	//
	// Vider la liste des fils du BBItem sur cet edit
	//
	pControle->getTransfert()->pBBFilsItem->VectorFils.vider();
	if (pNSEdit)
	{
		delete pNSEdit ;
		pNSEdit = 0 ;
	}
	if (!pNSEdit)
	{
		pNSEdit 	         = new NSEditLexiqueDerive(pContexte, this) ;
		pNSEdit->pControle = new NSControle(pBBItemPere, string(code), "") ;
		pNSEdit->pControle->setControle(dynamic_cast<void*>(pNSEdit)) ;
		pNSEdit->pControle->setType(pNSEdit->donneType()) ;
		pNSEdit->pContexte = pContexte ;
		pNSEdit->pControle->setNSDialog(pBBItemPere->pNSDialog) ;
	}
	sComplement = "" ;
	pNSEdit->sContenuTransfert = sContenu ;
	pNSEdit->pControle->setIdentite(string(code)) ;
	CreerBBItem() ;
*/

  CreerEditDerive(string(code), sContenu) ;

  // Set local transfert so that old value doesn't prevent the new one to be activated
  //
  NSTransferInfo *pTransfert = pControle->getTransfert() ;
  Message        *pTrMessage = pTransfert->getTransfertMessage() ;
  pTrMessage->SetLexique(string(code)) ;

	if (pNSEdit->pControle->getTransfert())
	{
		Message MessageNewFils ;
		pNSEdit->pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, &MessageNewFils) ;
	}
}
catch (...)
{
	erreur("Exception NSEditLexique::ElementSelectionne.", standardError, 0) ;
}
}

//------------------------------------------------------------------------// le champ edit perd son focus
//------------------------------------------------------------------------
void
NSEditLexique::EvKillFocus(THandle hWndGetFocus)
{
try
{
  if (hWndGetFocus == HWindow)
    return ;

  if (_pDico->getDicoDialog() && _pDico->getDicoDialog()->doesBelong(hWndGetFocus))
    return ;

  //
  // Récupère le texte - Get text
  //
  string sContenu = getTextAsString() ;

  NSTransferInfo* pTransferInfo = (NSTransferInfo*) 0 ;
  if (pControle && pControle->getTransfert())
    pTransferInfo = pControle->getTransfert() ;

  if (pTransferInfo)
  {
    BBFilsItem* pFilsItem = pTransferInfo->getFilsItem() ;
    if (pFilsItem && (false == pFilsItem->VectorFils.empty()))
    {
      BBItem* pSonItem = *(pFilsItem->VectorFils.begin()) ;
      if (pSonItem)
      {
        BBFilsItem* pFilsEditLexique = *(pSonItem->_aBBItemFils.begin()) ;
        // if (pFilsEditLexique && (pFilsEditLexique->getItemLabel() == "")) //désactiver ce fils
        if (pFilsEditLexique && (pFilsEditLexique->getItemLabel() != string("")))
        {
          if (string("") == sContenu) //désactiver ce fils
          {
            pFilsEditLexique->setItemLabel(string("")) ;
            pFilsEditLexique->Desactive() ;
          }
          // When user switches from a term from the lexique to a free text,
          // this is the only moment we can do something
          // (when switching from a free text to a lexique term, the lexique
          // control dialog sends a signal when the concept is selected from
          // the list)
          //
          else if (sContenu != sContenuPere)
          {
            Message* pTranfertMsg = pFilsItem->getItemTransfertMsg() ;

            // Control's identity is in the form N00001/£C;020
            // We have to set the ending concept as Item label
            //
            string sIdentite = pControle->getIdentite() ;

            ClasseStringVector aVect ;
            DecomposeChaine(&sIdentite, &aVect, string(1, cheminSeparationMARK)) ;
            if (false == aVect.empty())
            {
              iterStringReverse it = aVect.rbegin() ;
              string sLabel = (*it)->getItem() ;

              // pFilsEditLexique->setItemLabel(sLabel) ;

              if (string("£C;020") == sLabel)
              {
                pFilsEditLexique->setItemLabel(string("£?????")) ;
                if (pTranfertMsg)
                  pTranfertMsg->SetLexique(string("£?????")) ;
              }
            }

            if (pTranfertMsg)
              pTranfertMsg->SetTexteLibre(sContenu) ;
            if (pNSEdit)
              pNSEdit->sContenuTransfert = sContenu ;
          }
        }
      }
    }
  }
  if (false == _bGardeFocus)
  {
    // If not empty and free text not enabled, prevent exit if not a Lexique entry
    //
    if ((string("") != sContenu) &&
        (false == _bIsFreeTextEnabled) &&
        (pTransferInfo && pTransferInfo->getTransfertMessage()))
    {
      Message *pTrMessage = pTransferInfo->getTransfertMessage() ;
      string sMsgLex = pTrMessage->GetLexique() ;
      if (string("") == sMsgLex)
      {
        SetText("") ;

        NSEditDicoGlobal::EvKillFocus(hWndGetFocus) ;

        ::PostMessage(Parent->HWindow, WM_COMMAND, IDNOFREETXT, 0) ;
      }
    }

    if (hWndGetFocus != THandle(this))
    {
      if (_pDico->getDicoDialog() && (false == _pDico->getDicoDialog()->doesBelong(hWndGetFocus)))
      {
        _pDico->getDicoDialog()->cacherDialogue() ;
        NSEditDicoGlobal::EvKillFocus(hWndGetFocus) ;
      }
      else
        TEdit::EvKillFocus(hWndGetFocus) ;
    }
  }
  else
    _bGardeFocus = false ;
}
catch (...)
{
	erreur("Exception NSEditLexique::EvKillFocus.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Function: 		NSEdit::activeControle(int activation, string message)
//
//  Description:	Initialise le contrôle avant affichage de la boîte de
//						dialogue
//
//  Arguments:		activation : BF_CHECKED ou BF_UNCHECKED
//						message    : Contenu de la boîte
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSEditLexique::activeControle(int activation, Message* pMessage)
{
  // Gestion windows standard
  // Windows standard behaviour
  if (pControle && pControle->getNSDialog())
  {
    NSContexte* pContexte = pControle->getNSDialog()->pContexte ;
    if (pContexte && pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
      bWinStd = true ;
  }

  string sContenu = string("") ;
	switch (activation)
	{
    //
    // recupérer la strinf à afficher cher le fils fictif
    //
    case BF_CHECKED	:

      if ((pMessage->GetLexique() != string("£?????")) &&
          (pMessage->GetLexique() != string(""))) //pas de texte libre
      {
        string sLang = pContexte->getUserLanguage() ;

        pContexte->getDico()->donneLibelle(sLang, &pMessage->GetLexique(), &sContenu) ;

        CreerEditDerive(pMessage->GetLexique(), sContenu) ;
        SetText(sContenu.c_str()) ;
      }
      else // texte libre
        sContenu = pMessage->GetTexteLibre() ;

      sContenuPere = sContenu ;

      // Note : dans le cas où on vient d'être activé par le pNSEdit (NSEditLexiqueDerive)
      // on ne doit pas faire de SetText car le message transmis est vide.
      if ((NSEditLexiqueDerive*) NULL == pNSEdit)
        SetText(sContenu.c_str()) ;
      break ;
		case BF_UNCHECKED :
      sContenu     = string("") ;
      sContenuPere = string("") ;
      SetText(sContenu.c_str()) ;
      //
      // Le BBFilsItem (qui controle ce champ edit) dont l'étiquette
      // contient £C0;020 doit désactiver son fils contenu dans l'EditLexique
      //
      BBFilsItem*	pFather = pControle->getTransfert()->getFilsItem() ;

      if (pFather && (false == pFather->VectorFils.empty()))
      {
        BBItem* pSonItem = *(pFather->VectorFils.begin()) ;
        if (pSonItem)
        {
          BBFilsItem* pFilsEditLexique = *(pSonItem->_aBBItemFils.begin()) ;
          pFilsEditLexique->Desactive() ;
          pSonItem->_aBBItemFils.vider() ;
        }
        pFather->VectorFils.vider() ;
      }
      break ;
  }
  sComplement = pMessage->GetComplement() ;

  if (_pDico->getDicoDialog())
    _pDico->getDicoDialog()->cacherDialogue() ;
}

//---------------------------------------------------------------------------
//  Function: 		NSEdit::Transfer(TTransferDirection direction,
//															 int* pActif, string* pMessage)
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
NSEditLexique::Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage)
{
try
{
  string sContenu = getTextAsString() ;

  string sCodeLexique = string("") ;  if (pNSEdit)
    sCodeLexique = pNSEdit->pControle->getIdentite() ;

  if (((string("") == sCodeLexique) || (string("£?????") == sCodeLexique)) &&
         (string("") != sContenu) && (sContenu != sContenuPere) ) //texte libre
  {
/*
    pControle->getTransfert()->pBBFilsItem->VectorFils.vider() ;
    if (pNSEdit)
    {
      	    delete pNSEdit;
      	    pNSEdit = 0;
   	    }
        if (!pNSEdit)
        {
      	    BBItem* pBBItemPere = pControle->getTransfert()->pBBFilsItem->pPere;
            pNSEdit = new NSEditLexiqueDerive(pContexte, this);
            pNSEdit->pControle  = new NSControle(pBBItemPere, string("£?????"), "");
            pNSEdit->pControle->setControle(dynamic_cast<void*>(pNSEdit));
            pNSEdit->pControle->setType(pNSEdit->donneType());
            pNSEdit->pContexte            = pContexte;
            pNSEdit->pControle->setNSDialog(pBBItemPere->pNSDialog);
        }
        pNSEdit->sContenuTransfert = sContenu;
        pNSEdit->sIdentite = string("£?????");
        pNSEdit->pControle->setIdentite(string("£?????"));
        CreerBBItem();
*/

    CreerEditDerive(string("£?????"), sContenu) ;
  }

  if (tdSetData == direction)
  {
    switch (*pActif)
		{
      case inactiveCtrl :

        sContenu     = string("") ;
        sContenuPere = string("") ;
        break ;

			case activeCtrl :

        if ((pMessage->GetLexique() != string("£?????")) &&
            (pMessage->GetLexique() != string(""))) //pas de texte libre
        {
          string sLang = pContexte->getUserLanguage() ;
          pContexte->getDico()->donneLibelle(sLang, &pMessage->GetLexique(), &sContenu) ;
        }
        else // texte libre
          sContenu = pMessage->GetTexteLibre() ;

        SetText(sContenu.c_str()) ;
        sContenuPere = sContenu ;
        break ;
    }
  }
	else if (tdGetData == direction)
	{
    if (string("") == sContenu)
      *pActif = inactiveCtrl ;
    else
      *pActif = activeCtrl ;
    //
		//transferer les données
    //    if (pMessage)    {      string sEtiquette = string("") ;
      if (pControle->getTransfert())
        sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

      // pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
      pMessage->InitFromEtiquette(sEtiquette) ;
      pMessage->SetTexteLibre(sContenu) ;
    }
  }
  if (_pDico->getDicoDialog())
    _pDico->getDicoDialog()->cacherDialogue() ;

  return 1 ;
}
catch (...)
{
  erreur("Exception NSEditLexique::Transferer.", standardError, 0) ;
  return 0 ;
}
}

uintNSEditLexique::TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage)
{
  string sContenu = getTextAsString() ;

  string sCodeLexique = string("") ;
  if (pNSEdit)
    sCodeLexique = pNSEdit->pControle->getIdentite() ;

  if (string("") == sContenu)
    *pActif = inactiveCtrl ;
  else
    *pActif = activeCtrl ;

  //
  // Transferer les données si il n'existe pas de contrôle lexique dérivé
  //
  // S'il existe un contrôle lexique dérivé, le message de transfert contient
  // le code Lexique de l'élément saisi et par l'étiquette du FilsItem (£C;020)
  // Il ne faut surtout pas remplacer l'un par l'autre ici.
  //  if (pMessage && ((NSEditLexiqueDerive*) NULL == pNSEdit))  {    string sEtiquette = string("") ;
    if (pControle->getTransfert() && pControle->getTransfert()->getFilsItem())
      sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

    // pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
    pMessage->InitFromEtiquette(sEtiquette) ;
    pMessage->SetTexteLibre(sContenu) ;
  }
  return 1 ;
}
voidNSEditLexique::CreerEditDerive(string sCode, string sContenu){  //	// Vider la liste des fils du BBItem sur cet edit
	//
  BBFilsItem* pFilsItemPere = (BBFilsItem*) 0 ;
  BBItem*     pBBItemPere   = (BBItem*) 0 ;
  if (pControle && pControle->getTransfert())
  {
    pFilsItemPere = pControle->getTransfert()->getFilsItem() ; //le BBFilsItem rattaché au père
    pBBItemPere = pFilsItemPere->getItemFather() ;
  }
  if (pFilsItemPere)
    pFilsItemPere->VectorFils.vider() ;

	if (pNSEdit)
	{
		delete pNSEdit ;
		pNSEdit = (NSEditLexiqueDerive*) 0 ;
	}
	if ((NSEditLexiqueDerive*) NULL == pNSEdit)
	{
		pNSEdit 	         = new NSEditLexiqueDerive(pContexte, this) ;
		pNSEdit->pControle = new NSControle(pContexte, pBBItemPere, sCode, "") ;
		pNSEdit->pControle->setControle(dynamic_cast<void*>(pNSEdit)) ;
		pNSEdit->pControle->setType(pNSEdit->donneType()) ;
		pNSEdit->pContexte = pContexte ;
		pNSEdit->pControle->setNSDialog(pBBItemPere->getDialog()) ;
    pNSEdit->pControle->setMUEView(pBBItemPere->getView()) ;
	}
	sComplement = string("") ;
	pNSEdit->sContenuTransfert = sContenu ;
  pNSEdit->sIdentite         = sCode ;
	pNSEdit->pControle->setIdentite(sCode) ;
	CreerBBItem() ;}void
NSEditLexique::CreerBBItem()
{
  if (((NSControle*) NULL == pControle) || (NULL == pControle->getTransfert()))
    return ;

  //
  //créer un BBfilsItem rattaché à ce nouveau controle
  //
  BBFilsItem* pFilsItemPere = pControle->getTransfert()->getFilsItem() ; //le BBFilsItem rattaché au père
  if ((BBFilsItem*) NULL == pFilsItemPere)
    return ;

  pFilsItemPere->CreerFilsManuel(pNSEdit->pControle, 0) ;

  // on ajoute maintenant manuellement le nom du dialogue du BBItem père
  // l'array des fils contient en principe le fils créé ci-dessus.
  if (pFilsItemPere->VectorFils.empty())
    return ;

  BBiterFils iter = pFilsItemPere->VectorFils.begin() ;
  for ( ; pFilsItemPere->VectorFils.end() != iter ; iter++)
  {
    (*iter)->setDialogName(pFilsItemPere->getItemFather()->getDialogName()) ;
    (*iter)->_sIdArchetype = pFilsItemPere->getItemFather()->_sIdArchetype ;
  }
}

string
NSEditLexique::getTextAsString()
{
  int iBuffLen = GetTextLen() ;
  if (iBuffLen <= 0)
    return string("") ;

  char far* szBuff = new char[iBuffLen + 1] ;
  GetText(szBuff, iBuffLen + 1) ;  string sContenu = string(szBuff) ;
  
  delete[] szBuff ;

  return sContenu ;
}

void
NSEditLexique::EvRButtonDown(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
try
{
  NS_CLASSLIB::TPoint lp = point ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  string sPaste = pSuper->getText("treeViewManagement", "paste") ;

  OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

  menu->AppendMenu(MF_STRING, CM_EDITPASTE, sPaste.c_str()) ;

  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}
catch (...)
{
  erreur("Exception NSEditLexique::EvRButtonDown.", standardError, 0) ;
}
}

//*************************************************************************
// 								classe NSEditLexiqueDerive
//*************************************************************************

NSEditLexiqueDerive::NSEditLexiqueDerive(NSContexte* pCtx, NSEditLexique* pEdit)
                    :NSRoot(pCtx)
{
  pEditPere         = pEdit ;
  pComboPere        = 0 ;
	sContenuTransfert = "" ;
  pControle         = 0 ;
  sIdentite         = "" ;
  pContexte         = pCtx ;
  sComplement       = "" ;
}

NSEditLexiqueDerive::NSEditLexiqueDerive(NSContexte* pCtx, NSComboSemantique* pCombo)
                    :NSRoot(pCtx)
{
  pEditPere         = 0 ;
  pComboPere        = pCombo ;
	sContenuTransfert = "" ;
  pControle         = 0 ;
  sIdentite         = "" ;
  pContexte         = pCtx ;
  sComplement       = "" ;
}

NSEditLexiqueDerive::~NSEditLexiqueDerive()
{
  if (pControle)
    delete pControle ;
}

//---------------------------------------------------------------------------//  Function: 		NSEdit::activeControle(int activation, string message)
//
//  Description:	Initialise le contrôle avant affichage de la boîte de
//						dialogue
//
//  Arguments:		activation : BF_CHECKED ou BF_UNCHECKED
//						message    : Contenu de la boîte
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSEditLexiqueDerive::activeControle(int activation, Message* pMessage)
{
  switch (activation)
	{
    //
    // recupérer la string à afficher chez le fils fictif
    //
		case BF_CHECKED	:
      if ((pMessage->GetLexique() != string("£?????")) &&
          (pMessage->GetLexique() != string("")))
      {
        string sLang = pContexte->getUserLanguage() ;

        pContexte->getDico()->donneLibelle(sLang, &pMessage->GetLexique(), &sContenuTransfert);
      }
      else
      {
        /*
        if (pMessage->GetComplement() != "")
                {
                    bool texteRecupere = pNSTlibre->RecupereTexte(pMessage->GetComplement(), &sContenuTransfert);
                    if (texteRecupere == false)
                    {
                        erreur("Erreur Récupération texte libre.", 0, 0);
                        delete pNSTlibre;
                        return ;
                    }
                }
        */
        if (pMessage->GetTexteLibre() != string(""))
          sContenuTransfert = pMessage->GetTexteLibre() ;
      }
      //pEditPere->SetText(sContenuTransfert.c_str());
      break ;

    case BF_UNCHECKED :

      sContenuTransfert = string("") ;
      break ;
  }
}

//---------------------------------------------------------------------------
//  Function: 		NSEdit::Transfer(TTransferDirection direction,
//															 int* pActif, string* pMessage)
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
NSEditLexiqueDerive::Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage)
{  // NSTlibre* pNSTlibre = new NSTlibre(pContexte);
	if (tdSetData == direction)
	{
    switch (*pActif)
		{
      case inactiveCtrl :

        sContenuTransfert = string("") ;
        break ;

			case activeCtrl :

        /*
        sComplement = pMessage->GetComplement();
                if (sComplement != "")
                {
                    bool texteRecupere = pNSTlibre->RecupereTexte(sComplement, &sContenuTransfert);
                   	if (texteRecupere == false)
                   	{
                        erreur("Erreur Récupération texte libre.", 0, 0);
                        return 0;
                   	}
        }
        */
        sContenuTransfert = pMessage->GetTexteLibre() ;
        //pEditPere->Insert(sContenuTransfert.c_str());
        break ;
    }
  }
	else if (tdGetData == direction)
	{
    if (string("") == sContenuTransfert)
      *pActif = inactiveCtrl ;
    else
      *pActif = activeCtrl ;

    if (pMessage)
    {
      pMessage->SetTexteLibre(sContenuTransfert) ;
      //
      // transferer les données
      //
      if (string("") != sContenuTransfert)
      {
        //
        // texte libre : mettre dans le complement de
        // la patpatho relative à ce fils le code
        // du texte libre
        //
        if (string("£?????") == sIdentite)
          pMessage->SetComplement(sComplement) ;
      }

      string sEtiquette = string("") ;
      if (pControle->getTransfert())
        sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

      pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
    }
  }
  return 1 ;
}

uintNSEditLexiqueDerive::TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage)
{  if (string("") == sContenuTransfert)
    *pActif = inactiveCtrl ;
  else
    *pActif = activeCtrl ;

  if (pMessage)
  {
    pMessage->SetTexteLibre(sContenuTransfert) ;
    //
    // transferer les données
    //
    if (sContenuTransfert != "")
    {
      //
      // texte libre : mettre dans le complement de
      // la patpatho relative à ce fils le code
      // du texte libre
      //
      if (sIdentite == string("£?????"))
        pMessage->SetComplement(sComplement) ;
    }

    string sEtiquette = string("") ;
    if (pControle->getTransfert())
      sEtiquette = pControle->getTransfert()->getFilsItem()->getItemLabel() ;

    pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
  }
  return 1 ;
}
boolNSEditLexiqueDerive::canWeClose(){  return true ;}