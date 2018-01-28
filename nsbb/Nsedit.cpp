#include <owl\checkbox.h>
#include "nsbb\nsedit.h"
#include "partage\nsdivfct.h"
// #include "partage\nsperson.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsutiDlg.h"
#include "nsbb\nslistwind.h"
#include "nsbb\nsdlg.h"
#include "nautilus\nsepicap.h"
#include "nsbb\ns_skins.h"

const char codeMARK   = 'O' ;

const char decimSepar = ',' ;   // Edit double bouton

DEFINE_RESPONSE_TABLE1(NSEdit, TEdit)
	EV_WM_KILLFOCUS,
	EV_WM_SETFOCUS,
	EV_WM_KEYUP,
	EV_WM_KEYDOWN,
	EV_WM_GETDLGCODE,
	EV_WM_CHAR,
  EV_WM_TIMER,
END_RESPONSE_TABLE ;

//Type of character	Character	Description
//
//			Special				#		Accept only a digit
//									?		Accept only a letter (case_insensitive)
//									&		Accept only a letter, force to uppercase
//									@		Accept any character
//									!		Accept any character, force to uppercase
//			Match					;		Take next character literally
//									*		Repetition count
//									[]		Option
//									{}		Grouping operators
//									,		Set of alternatives
//			All others					Taken literally
//
// {A{1[#[#]],2{[{0,1,2,3,4}[#],5[{0,1,2,3,4,5}],6,7,8,9]},{3,4,5,6,7,8,9}[#]},B,D,F,G,I,L,M,N,O,R,S,T,$,;#,;@,+}

//
// Constructeur
//
NSEdit::NSEdit(NSContexte* pCtx, TWindow* parent, int resId,  char* szTypeEdit, string sUserLang)
       :TEdit(parent, resId, 0), NSRoot(pCtx)
{
	strcpy(_szType, szTypeEdit) ;
	DisableTransfer() ;

  initCommonData() ;

	_sContenuBrut        = string("") ;
	_sContenuTransfert   = string("") ;
	_pControle           = (NSControle*) 0 ;
	_bEncoursEdition     = false ;
	_parentEdit          = parent ;
	TextLimit            = 15 ;
	_bValidContent       = true ;
	_bCurrentlyWarning   = false ;
	_bHistoryOpen        = false ;
  _sLang               = sUserLang ;

  _bWaitingForOb1      = false ;
  _bNeedUserValidation = false ;
  _bUserValidated      = false ;

	//
	// Mise en place du validateur
	//
	string sValidateur = string("") ;
	// int     iDecimale   = 0 ;
	switch (_szType[0])
	{
		// Saisie des chiffres
		//
		// ATTENTION : textLen = iMaxInput + 1
		//
		case nbMARK :
			sValidateur = "0-9,-" ;
			sValidateur[3] = decimSepar ;
			SetValidator(new TFilterValidator(sValidateur.c_str())) ;
			break ;
		//
		// Saisie des lettres
		//
		case charMARK  :
			SetValidator(new TFilterValidator("0-9A-Za-z. ")) ;
			break ;
		//
		// Saisie des dates
		//
		case dateMARK :
			SetValidator(new TPXPictureValidator("##/##/####")) ;
			break ;
		//
		// Saisie des dates et heures
		//
    case dateHeureMARK :
			SetValidator(new TPXPictureValidator("##/##/#### ##:##:##")) ;
      TextLimit = 20 ;
			break ;

		case codeMARK :
			sValidateur = ",.0-9A-Za-z";
			//sValidateur[4] = decimSepar;
			SetValidator(new TFilterValidator(sValidateur.c_str()));
			break ;
	}
}

NSEdit::NSEdit(NSContexte* pCtx, TWindow* parent, int resId, string sUserLang)
       :TEdit(parent, resId, 0), NSRoot(pCtx)
{
	DisableTransfer();

  initCommonData() ;

	_sContenuBrut        = string("") ;
	_sContenuTransfert   = string("") ;
	_pControle           = (NSControle*) 0 ;
	_bEncoursEdition     = false ;
	_parentEdit          = parent ;
	_bValidContent       = true ;
	_bCurrentlyWarning   = false ;
	_bHistoryOpen        = false ;
  _sLang               = sUserLang ;

  _bWaitingForOb1      = false ;
  _bNeedUserValidation = false ;
  _bUserValidated      = false ;
}

NSEdit::NSEdit(NSContexte* pCtx, TWindow* parent, int resId, char* szTypeEdit,
                const char far* text, int x, int y, int w, int h, string sUserLang,
                uint textLimit, bool multiline, TModule* module)
        :TEdit(parent, resId, text, x, y, w, h, textLimit, multiline, module), NSRoot(pCtx)
{
	strcpy(_szType, szTypeEdit) ;
	DisableTransfer() ;

  initCommonData() ;

	_sContenuBrut        = string("") ;
	_sContenuTransfert   = string("") ;
	_pControle           = (NSControle*) 0 ;
  _bEncoursEdition     = false ;
  _parentEdit          = parent ;
  TextLimit            = 15 ;
  _bValidContent       = true ;
  _bCurrentlyWarning   = false ;
  _bHistoryOpen        = false ;
  _sLang               = sUserLang ;

  _bWaitingForOb1      = false ;
  _bNeedUserValidation = false ;
  _bUserValidated      = false ;

	//
	// Mise en place du validateur
	//
	string sValidateur = string("") ;
	// int     iDecimale   = 0 ;
	switch (_szType[0])
	{
    // Saisie des chiffres
    //
    // ATTENTION : textLen = iMaxInput + 1
    //
		case nbMARK :
      sValidateur = "0-9,-" ;
      sValidateur[3] = decimSepar ;
      SetValidator(new TFilterValidator(sValidateur.c_str())) ;
      break ;

		// Saisie des lettres
    //
		case charMARK  :
      SetValidator(new TFilterValidator("0-9A-Za-z. ")) ;
      break ;

		// Saisie des dates
    //
		case dateMARK :
      SetValidator(new TPXPictureValidator("##/##/####")) ;
      break ;

		// Saisie des dates et heures
		//
    case dateHeureMARK :
			SetValidator(new TPXPictureValidator("##/##/#### ##:##:##")) ;
      TextLimit = 20 ;
			break ;

    case codeMARK :
      sValidateur = ",.0-9A-Za-z" ;
      //sValidateur[4] = decimSepar;
      SetValidator(new TFilterValidator(sValidateur.c_str())) ;
      break ;
	}
}

//
// Destructeur
//
NSEdit::~NSEdit()
{}

// SetupWindow : on fixe la police du dialogue
void
NSEdit::SetupWindow()
{
	TEdit::SetupWindow() ;

  initColors() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

string
NSEdit::GetText(size_t iSizeLimit)
{
  size_t iBuffLen = GetTextLen() ;
  if ((0 != iSizeLimit) && (iBuffLen > iSizeLimit))
    iBuffLen = iSizeLimit ;

	char far* str = new char[iBuffLen + 1] ;

	TEdit::GetText(str, iBuffLen + 1) ;
	string sContent = string(str) ;
	delete[] str ;

  return sContent ;
}

void
NSEdit::SetText(const string sContent)
{
  TEdit::SetText(sContent.c_str()) ;
}

void
NSEdit::initCommonData()
{
  _bWinStd = false ;
	if (pContexte)
	{
    if (pContexte && pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
    	_bWinStd = true ;
	}
}

void
NSEdit::initColors()
{
  initDefaultColors() ;

  if (NULL == pContexte)
    return ;

  nsSkin* pSkin = pContexte->getSkins()->donneSkin("EditStandard") ;
	if (pSkin && pSkin->getFontBackColor() && pSkin->getFontColor())
  {
  	_StandardBackgroundColor = *(pSkin->getFontBackColor()) ;
    _StandardTextColor       = *(pSkin->getFontColor()) ;
  }

  pSkin = pContexte->getSkins()->donneSkin("EditStartWaiting") ;
	if (pSkin && pSkin->getFontBackColor() && pSkin->getFontColor())
  {
  	_StartWaitingBackgroundColor = *(pSkin->getFontBackColor()) ;
    _StartWaitingTextColor       = *(pSkin->getFontColor()) ;
  }

  pSkin = pContexte->getSkins()->donneSkin("EditEndWaiting") ;
	if (pSkin && pSkin->getFontBackColor() && pSkin->getFontColor())
  {
  	_EndWaitingBackgroundColor = *(pSkin->getFontBackColor()) ;
    _EndWaitingTextColor       = *(pSkin->getFontColor()) ;
  }

  pSkin = pContexte->getSkins()->donneSkin("EditNeedValidation") ;
	if (pSkin && pSkin->getFontBackColor() && pSkin->getFontColor())
  {
  	_NeedValidationBackgroundColor = *(pSkin->getFontBackColor()) ;
    _NeedValidationTextColor       = *(pSkin->getFontColor()) ;
  }
}

void
NSEdit::initDefaultColors()
{
  _StandardBackgroundColor       = NS_CLASSLIB::TColor(TColor::White) ;
  _StandardTextColor             = NS_CLASSLIB::TColor(TColor::Black) ;
  _StartWaitingBackgroundColor   = NS_CLASSLIB::TColor(TColor::LtYellow) ;
  _StartWaitingTextColor         = NS_CLASSLIB::TColor(TColor::Black) ;
  _EndWaitingBackgroundColor     = NS_CLASSLIB::TColor(TColor::LtBlue) ;
  _EndWaitingTextColor           = NS_CLASSLIB::TColor(TColor::Black) ;

  _NeedValidationBackgroundColor = NS_CLASSLIB::TColor(TColor::LtGray) ;
  _NeedValidationTextColor       = NS_CLASSLIB::TColor(TColor::Black) ;
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
void
NSEdit::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  if (_pControle && _pControle->getNSDialog())
    _pControle->getNSDialog()->setLastAction(NSDialog::actionControl) ;

  // Gestion windows standard
  // Windows standard behaviour
  if (_bWinStd && _pControle && (_pControle->getNSDialog()))
  {
    // return TEdit::EvKeyUp(key, repeatcount, flags) ;
    TEdit::EvKeyUp(key, repeatcount, flags) ;
    return ;
  }

  // THandle NextControl = 0;
  switch (key)
  {
    //return
    case VK_RETURN :
    case VK_DOWN   :
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // suivant pControle sinon le premier
      //
      if      (_pControle && _pControle->getNSDialog())
        _pControle->getNSDialog()->ActiveControlSuivant(_pControle) ;
      else if (_pControle && _pControle->getMUEView())
        _pControle->getMUEView()->SetFocusToNextControl((TControl*)this) ;
      else
      {
        if (_parentEdit->Parent->ChildWithId(1))
          _parentEdit->Parent->ChildWithId(1)->SetFocus() ;
        else
          _parentEdit->CloseWindow(IDOK) ;
      }
      break ;

    case VK_UP    :
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // précédent pControle sinon le dernier
      //
      if      (_pControle && _pControle->getNSDialog())
        _pControle->getNSDialog()->ActiveControlPrecedent(_pControle) ;
      else if (_pControle && _pControle->getMUEView())
        _pControle->getMUEView()->SetFocusToPreviousControl((TControl*)this) ;
      break ;

    //tabulation
    case VK_TAB	:
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // suivant pControle sinon le premier
      //
      if      (_pControle && _pControle->getNSDialog())
        _pControle->getNSDialog()->ActiveControlSuivant(_pControle) ;
      else if (_pControle && _pControle->getMUEView())
      {
        if (GetKeyState(VK_SHIFT) < 0)
          _pControle->getMUEView()->SetFocusToPreviousControl((TControl*)this) ;
        else
          _pControle->getMUEView()->SetFocusToNextControl((TControl*)this) ;
      }
      break ;

    default		:
      //return TEdit::EvKeyUp(key, repeatcount, flags);
      TEdit::EvKeyUp(key, repeatcount, flags) ;
      return ;
  }
}

void
NSEdit::EvKeyDown(uint key, uint repeatcount, uint flags)
{
try
{
  if (_pControle && _pControle->getNSDialog())
    _pControle->getNSDialog()->setLastAction(NSDialog::actionControl) ;

  // Not a Ctrl-something key
  //
  if (GetKeyState(VK_CONTROL) >= 0)
  {
    TEdit::EvKeyDown(key, repeatcount, flags) ;
    return ;
  }

  // Ctrl-H -> History
  //
  if (('H' == key) || ('h' == key))
  {
    if (_pControle && _pControle->getTransfert() && _pControle->getTransfert()->getFilsItem())
    {
      NSContexte* pContexte = _pControle->getInterfaceContext() ;

      // On récupère la fiche avec l'unité
      BBFilsItem*	pBBFilsItemUnit = _pControle->getTransfert()->getFilsItem() ;

      if ((pBBFilsItemUnit->getItemFather()) && (pBBFilsItemUnit->getItemFather()->_pBBFilsPere))
      {
        BBFilsItem*	pBBFilsItemElement = pBBFilsItemUnit->getItemFather()->_pBBFilsPere ;
        string sItemValue = pBBFilsItemElement->getItemLabel() ;
        //
        // Unité
        //
        string sUnit        = "" ;
        string sItemUnit    = pBBFilsItemUnit->getItemLabel() ;
        size_t posit = sItemUnit.find(string(1, cheminSeparationMARK)) ;
        if (posit != NPOS)
          sUnit = string(sItemUnit, 0, posit) ;

        BBItem* pPereItem = pBBFilsItemElement->getItemFather() ;
        // string 	sLoc = pPereItem->sLocalisation ;
        string sLoc = pPereItem->donneVraiChemin() ;
        string sResult = "" ;

        _bHistoryOpen = true ;

        NSValListHistoryDialog* pDialogHistory = new NSValListHistoryDialog(Parent, pContexte, sItemValue, sLoc, sUnit, &sResult) ;
        pDialogHistory->Execute() ;
        delete pDialogHistory ;

        _bHistoryOpen = false ;

        if (string("") != sResult)
          SetText(sResult.c_str()) ;

        return ;
      }
    }
  }

  // If a function is connected, send it the NSDLGFCT_CTRL_KEY signal
  //
  else if (_pControle && _pControle->getFonction())
		/* int iNotifier = */ _pControle->getFonction()->execute(NSDLGFCT_CTRL_KEY, HWND(0), key) ;

  TEdit::EvKeyDown(key, repeatcount, flags) ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception NSEdit::EvKeyDown : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
  erreur("Exception NSEdit::EvKeyDown.", standardError, 0) ;
  return ;
}
}

//-------------------------------------------------------------------------
// enlever les beeps
//-------------------------------------------------------------------------
void
NSEdit::EvChar(uint key, uint repeatCount, uint flags)
{
  if (_pControle && _pControle->getNSDialog())
    _pControle->getNSDialog()->setLastAction(NSDialog::actionControl) ;

	// Usually Ctrl-H = Back-space
	if (_bHistoryOpen)
		return ;

	if ((false == _bWinStd) && ((VK_RETURN == key) || (VK_TAB == key)))
		// Process this message to avoid message beeps.
		return ;

  if (_pControle && _pControle->isDisconnected())
    return ;

	if (Validator && (_szType[0] == nbMARK))
	{
    _bUserValidated = true ;

  	if ((',' == key) || ('.' == key))
    	key = decimSepar ;

    if ((VK_DECIMAL == key) || (VK_SEPARATOR == key))
    	key = decimSepar ;

    if (key == decimSepar)
    {
    	string sChaineFin = string("") ;
      uint startSel = 0 ;

    	int oldBuffLen = GetTextLen() ;
      if (oldBuffLen > 0)
      {
      	char far* oldBuff = new char[oldBuffLen+1] ;
        TEdit::GetText(oldBuff, oldBuffLen + 1) ;
        uint endSel ;
        GetSelection(startSel, endSel) ;

      	for (int i = 0; i < oldBuffLen; i++)
      	{
      		if (oldBuff[i] == decimSepar)
          {
          	delete[] oldBuff ;
            return ;
          }
        }

        string sChaineDeb = string(oldBuff) ;
        delete[] oldBuff ;

        sChaineFin = string(sChaineDeb, 0, startSel) + string(1,decimSepar)
                + string(sChaineDeb, endSel, strlen(sChaineDeb.c_str())-endSel) ;
      }

      SetText(sChaineFin.c_str()) ;
      SetSelection(startSel+1, startSel+1) ;
    }
    else
    	TEdit::EvChar(key, repeatCount, flags) ;
	}
  else
  	TEdit::EvChar(key, repeatCount, flags) ;
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSEdit::EvGetDlgCode(MSG far* /* msg */)
{
	//if (bWinStd)
	//    return 0 ;

	uint retVal = (uint)DefaultProcessing() ;
	if (false == _bWinStd)
		retVal |= DLGC_WANTALLKEYS ;
	return retVal ;
	//return DLGC_WANTALLKEYS;
}

void
NSEdit::EvTimer(uint id)
{
  if (ID_EDIT_OB_TIMER == id)
		ChangeGlowBackground() ;
}

HBRUSH
NSEdit::getCtlColor(HDC hdc)
{
  TDC dc(hdc);

  if (_bWaitingForOb1)
  {
		dc.SetTextColor(_CurrentWaitingTextColor) ;
		dc.SetBkColor(_CurrentWaitingBackgroundColor) ;
    return (HBRUSH)TBrush(_CurrentWaitingBackgroundColor) ;
  }

  if (_bNeedUserValidation)
  {
		dc.SetTextColor(_NeedValidationTextColor) ;
		dc.SetBkColor(_NeedValidationBackgroundColor) ;
    return (HBRUSH)TBrush(_NeedValidationBackgroundColor) ;
  }

  dc.SetTextColor(_StandardTextColor) ;
  dc.SetBkColor(_StandardBackgroundColor) ;
  return (HBRUSH)TBrush(_StandardBackgroundColor) ;
}

void
NSEdit::ChangeGlowBackground()
{
  if ((_StartWaitingBackgroundColor == _EndWaitingBackgroundColor) &&
      (_StartWaitingTextColor       == _EndWaitingTextColor))
    return ;
    
  _iGlowingDeciles++ ;
  if (11 == _iGlowingDeciles)
    _iGlowingDeciles = 0 ;

  if (_StartWaitingBackgroundColor != _EndWaitingBackgroundColor)
  {
    int iRedBkgnd   = getDecileInBetween(_StartWaitingBackgroundColor.Red(),   _EndWaitingBackgroundColor.Red(),   _iGlowingDeciles) ;
    int iGreenBkgnd = getDecileInBetween(_StartWaitingBackgroundColor.Green(), _EndWaitingBackgroundColor.Green(), _iGlowingDeciles) ;
    int iBlueBkgnd  = getDecileInBetween(_StartWaitingBackgroundColor.Blue(),  _EndWaitingBackgroundColor.Blue(),  _iGlowingDeciles) ;

    _CurrentWaitingBackgroundColor = NS_CLASSLIB::TColor(iRedBkgnd, iGreenBkgnd, iBlueBkgnd) ;
  }
  else
    _CurrentWaitingBackgroundColor = _StartWaitingBackgroundColor ;

  if (_StartWaitingTextColor != _EndWaitingTextColor)
  {
    int iRedBkgnd   = getDecileInBetween(_StartWaitingTextColor.Red(),   _EndWaitingTextColor.Red(),   _iGlowingDeciles) ;
    int iGreenBkgnd = getDecileInBetween(_StartWaitingTextColor.Green(), _EndWaitingTextColor.Green(), _iGlowingDeciles) ;
    int iBlueBkgnd  = getDecileInBetween(_StartWaitingTextColor.Blue(),  _EndWaitingTextColor.Blue(),  _iGlowingDeciles) ;

    _CurrentWaitingTextColor = NS_CLASSLIB::TColor(iRedBkgnd, iGreenBkgnd, iBlueBkgnd) ;
  }
  else
    _CurrentWaitingTextColor = _StartWaitingTextColor ;

  Invalidate() ;
}

int
NSEdit::getDecileInBetween(int iColor1, int iColor2, int iDecile)
{
  if ((iColor1 == iColor2) || (0 == iDecile))
    return iColor1 ;

  if (10 == iDecile)
    return iColor2 ;

  int iColorMin = min(iColor1, iColor2) ;
  int iColorMax = max(iColor1, iColor2) ;

  int iDeltaColor = iColorMax - iColorMin ;

  return iColorMin + (iDecile * iDeltaColor) / 10 ;
}

void
NSEdit::setWaitingForOb1(bool bW)
{
  // Just get warned that we are waiting for an answer from OB1
  //
  if (bW && (false == _bWaitingForOb1))
  {
    _iGlowingDeciles = 0 ;
    _CurrentWaitingBackgroundColor = _StartWaitingBackgroundColor ;
    _CurrentWaitingTextColor       = _StartWaitingTextColor ;
    SetTimer(ID_EDIT_OB_TIMER, 300) ;
  }
  //
  // Just get warned that we are no longer waiting for OB1
  //
  else
  {
    KillTimer(ID_EDIT_OB_TIMER) ;
  }

  _bWaitingForOb1 = bW ;
}

//
// Fonction déclenché lorsque l'Edit perd le focus.
//
void
NSEdit::EvKillFocus(HWND hWndGetFocus)
{
	int iNotifier = 1 ;

	string sContenu = GetText() ;

	_bEncoursEdition = false ;

	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_EXECUTE);
	//
	// Prévient Big Brother (si l'éventuelle fonction l'autorise)
	//

	Message theMessage ;

	if (iNotifier == 1)
	{
		if (string("") == sContenu)
		{
			if (string("") != _sContenuBrut)
			{
				_sContenuBrut = string("") ;
				donneTransfert() ;
        theMessage.SetComplement(_sContenuTransfert) ;
        theMessage.SetType(_sType) ;
        if (_pControle && _pControle->getTransfert())
        	_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_UNCHECKED,
                                                       &theMessage) ;
      }
    }
		else
		{
    	_sContenuBrut = sContenu ;
      donneTransfert() ;
      theMessage.SetComplement(_sContenuTransfert) ;
      theMessage.SetType(_sType) ;

      if (_pControle && _pControle->getTransfert())
      {
				if (string("") == _sContenuBrut)
        	_pControle->getTransfert()->ctrlNotification(BF_UNCHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
				else
        	_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
      }
    }	}
	/************************************************************
	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (pControle && pControle->getFonction())
		iNotifier = pControle->getFonction()->execute(NSDLGFCT_POSTEXEC, hWndGetFocus);
    *****************************************************************/

	TEdit::EvKillFocus(hWndGetFocus) ;

	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_POSTEXEC, hWndGetFocus) ;
}

//---------------------------------------------------------------------------//  Function: 		NSEdit::Transfer(TTransferDirection direction,
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
NSEdit::Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (tdSetData == direction)
	{
		switch (*pActif)
		{
			case inactiveCtrl :

      	_sContenuTransfert = "" ;
        donneBrut() ;
        SetText(_sContenuBrut.c_str()) ;
        break ;

			case activeCtrl :

      	if (pMessage)
        {
      		_sContenuTransfert = pMessage->GetComplement() ;
          _sType             = pMessage->GetType() ;
        }
        donneBrut() ;
        SetText(_sContenuBrut.c_str()) ;
        break ;
		}
	}
  else if (tdGetData == direction)
	{
  	if (_bEncoursEdition)
      _sContenuBrut = GetText() ;

    donneTransfert() ;
    if (string("") == _sContenuTransfert)
    	*pActif = inactiveCtrl ;
    else
    	*pActif = activeCtrl ;

    //
    //transformer la virgule si elle existe en un point
    //
    size_t pos = _sContenuTransfert.find(decimSepar) ;
    if (NPOS != pos)
    	_sContenuTransfert.replace(pos, 1, string(".")) ;

    if (pMessage)
    {
    	pMessage->SetComplement(_sContenuTransfert) ;
      pMessage->SetType(_sType) ;

      string sEtiquette = string("") ;
      if (_pControle && _pControle->getTransfert())
      	sEtiquette = _pControle->getTransfert()->getFilsItem()->getItemLabel() ;

      pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
    }
  }
	return 1 ;
}

uint
NSEdit::TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (_bEncoursEdition)
    _sContenuBrut = GetText() ;

  donneTransfert() ;

  if (string("") == _sContenuTransfert)
  	*pActif = inactiveCtrl ;
  else
  	*pActif = activeCtrl ;

  //
  //transformer la virgule si elle existe en un point
  //
  size_t pos = _sContenuTransfert.find(decimSepar) ;
  if (NPOS != pos)
  	_sContenuTransfert.replace(pos, 1, ".") ;

	pMessage->SetComplement(_sContenuTransfert) ;
  pMessage->SetType(_sType) ;

  string sEtiquette = string("") ;
  if (_pControle && _pControle->getTransfert())
    sEtiquette = _pControle->getTransfert()->getFilsItem()->getItemLabel() ;

  pMessage->MettreAJourCertitudePluriel(sEtiquette) ;

	return 1 ;
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
NSEdit::activeControle(int activation, Message* pMessage)
{
	// Gestion windows standard
	// Windows standard behaviour
	if (_pControle && _pControle->getInterfaceContext())
	{
		NSContexte* pContexte = _pControle->getInterfaceContext() ;
		if (pContexte && pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
			_bWinStd = true ;
	}

	switch (activation)
	{
		case BF_CHECKED   :

      if (pMessage->GetComplement() != _sContenuTransfert)
      {
        _sContenuTransfert = pMessage->GetComplement() ;
        _sType             = pMessage->GetType() ;
        donneBrut() ;
        SetText(_sContenuBrut.c_str()) ;

        //
	      // Si une fonction est attachée au contrôle, on l'exécute
	      //
	      if (_pControle && _pControle->getFonction())
          _pControle->getFonction()->execute(NSDLGFCT_POSTEXEC, 0) ;
      }

      break ;

		case BF_UNCHECKED :

      _sContenuTransfert = string("") ;
      donneBrut() ;
      SetText(_sContenuBrut.c_str()) ;
      break ;
  }
}

bool
NSEdit::canWeClose()
{
  return _bValidContent ;
}

//---------------------------------------------------------------------------
//  Function: 		NSEdit::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par//						l'utilisateur
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void NSEdit::donneBrut()
{
	size_t i ;

	switch (_szType[0])
	{
		// Saisie des chiffres
    case nbMARK  	:

      _sContenuBrut = _sContenuTransfert ;
      if (string("") == _sContenuBrut)
        break ;
      //
      // On remplace le '.' par decimSepar
      //
      i = _sContenuBrut.find(".") ;
      if (NPOS != i)
        _sContenuBrut[i] = decimSepar ;
      break ;

		// Saisie des lettres
		case charMARK	:
    case codeMARK	:

      _sContenuBrut = _sContenuTransfert ;
      break ;

		// Saisie des dates
		case dateMARK 	:

      _sContenuBrut = "00/00/1900" ;
      //
      // On passe de AAAAMMJJ à JJ/MM/AAAA
      //
      if (string("") != _sContenuTransfert)
      {
        for (i = 0; i < 4; i++)
          _sContenuBrut[i+6] = _sContenuTransfert[i] ;
        for (i = 0; i < 2; i++)
          _sContenuBrut[i+3] = _sContenuTransfert[i+4] ;
        for (i = 0; i < 2; i++)
          _sContenuBrut[i] = _sContenuTransfert[i+6] ;
      }
      break ;
  }
}

//---------------------------------------------------------------------------//  Function: 		NSEdit::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//					   en données NAUTILUS
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSEdit::donneTransfert()
{
	size_t      i ;
	size_t      k ;
	char        ptgag[3];

	switch (_szType[0])
	{
		// Saisie des chiffres
		case nbMARK  	:

      strip(_sContenuBrut) ;

      if ((string("") == _sContenuBrut) ||
            	(!_sContenuBrut.find_first_of("-0123456789") == 0))
      {
        _sContenuTransfert = string("") ;
        break ;
      }
      else
      {
        // If there is a '-' in first position, remove it and put it back later
        //
        bool bIsNegative = false ;

        size_t iPos = _sContenuBrut.find("-") ;
        if (NPOS != iPos)
        {
          if (iPos > 0)
            break ;

          if (strlen(_sContenuBrut.c_str()) == 1)
            break ;

          size_t iPos = _sContenuBrut.find("-", 1) ;
          if (NPOS != iPos)
            break ;

          bIsNegative = true ;
          _sContenuBrut = string(_sContenuBrut, 1, strlen(_sContenuBrut.c_str()) - 1) ;
        }

        // Recherche du premier caract différent de '0'
        k = _sContenuBrut.find_first_not_of('0') ;
        //
        // ATTENTION : on ne suppose plus que 0 = rien
        //
        if (NPOS == k)
        {
          _sContenuTransfert = string("0") ;
          break ;
        }
        // on teste si on a 0,XX (il ne faut pas ôter le 0)
        // ou 00035,XX (il faut ôter les 0 initiaux)
        if (_sContenuBrut[k] == decimSepar)
        {
          if (0 == k)
            _sContenuTransfert = "0" + _sContenuBrut ;
          else
            _sContenuTransfert = string(_sContenuBrut, k-1, strlen(_sContenuBrut.c_str())-k+1) ;
        }
        else
          _sContenuTransfert = string(_sContenuBrut, k, strlen(_sContenuBrut.c_str())-k) ;

        i = _sContenuTransfert.find(decimSepar) ;
        if (NPOS != i)
          _sContenuTransfert[i] = '.' ;

        if (bIsNegative)
        {
          _sContenuTransfert = string("-") + _sContenuTransfert ;
          _sContenuBrut      = string("-") + _sContenuBrut ;
        }
      }
      break ;

		// Saisie des lettres
    case codeMARK	:
		case charMARK	:

      // On ajoute l'indicateur de taille (2 premiers car.)
      strip(_sContenuBrut, stripRight) ;
      if (string("") != _sContenuBrut)
      {
        i = strlen(_sContenuBrut.c_str()) ;
        numacar(ptgag, i+1, 2) ;
        _sContenuTransfert = ptgag + _sContenuBrut ;
      }
      else
        _sContenuTransfert = string("") ;
      break ;

		// Saisie des dates
		case dateMARK 	:

      _sContenuTransfert = string("") ;
      //
      // On passe de JJ/MM/AAAA à AAAAMMJJ
      //
      if (string("00/00/1900") != _sContenuBrut)
      {
        _sContenuTransfert  = string(_sContenuBrut, 6, 4) ;
        _sContenuTransfert += string(_sContenuBrut, 3, 2) ;
        _sContenuTransfert += string(_sContenuBrut, 0, 2) ;
      }
      break ;
	}
}

voidNSEdit::EvSetFocus(HWND hWndLostFocus)
{
	_bEncoursEdition = true ;
  //pControle->SetFocus() ;
  //Parent->SetFocus() ;

  if (_pControle)
    _pControle->activateParent() ;

  TEdit::EvSetFocus(hWndLostFocus) ;
  // SetSelection(0,0) ;
}

//******************************************************************
//									TRAITEMENT DES DATES
//******************************************************************

//
//----------------------------------------------------------------------
//
DEFINE_RESPONSE_TABLE1(NSEditDate, NSEdit)
	EV_WM_KILLFOCUS,
    EV_WM_KEYDOWN,
    EV_WM_CHAR,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSEditDate::NSEditDate(NSContexte* pCtx, TWindow* parent, int resId, char* szTypeEdit, string sUserLang, char cRefDate)
           :NSEdit(pCtx, parent, resId, szTypeEdit, sUserLang)
{
	strcpy(_szType, szTypeEdit) ;
	//
	// Mise en place du validateur
	//
  char cValidateur[] = "##/##/####" ;
  cValidateur[2] = dateSeparationMARK ;
  cValidateur[5] = dateSeparationMARK ;  SetValidator(new TPXPictureValidator(cValidateur)) ;
  cRefYear = cRefDate ;
}

NSEditDate::NSEditDate(NSContexte* pCtx, TWindow* parent, int resId, char* szTypeEdit,
                const char far* text, int x, int y, int w, int h, string sUserLang, char cRefDate,
                uint textLimit, bool multiline, TModule* module)
            : NSEdit(pCtx, parent, resId, szTypeEdit, text, x, y, w, h, sUserLang, textLimit, multiline, module)
{
	strcpy(_szType, szTypeEdit) ;
	//
	// Mise en place du validateur
	//
	char cValidateur[] = "##/##/####" ;
	cValidateur[2] = dateSeparationMARK ;
	cValidateur[5] = dateSeparationMARK ;	SetValidator(new TPXPictureValidator(cValidateur)) ;
	cRefYear = cRefDate ;
}

//
// Destructeur
//
NSEditDate::~NSEditDate()
{}

//
// Fonction déclenché lorsque l'Edit perd le focus.
//
void
NSEditDate::EvKillFocus(HWND hWndGetFocus)
{
try
{
	if (_bCurrentlyWarning)
		return ;

  bool bCanLetItBe = true ;
  actionForKillFocus(bCanLetItBe) ;

  if (false == bCanLetItBe)
  {
    // TEdit::EvKillFocus(hWndGetFocus) ;
		SetFocus() ;
    return ;
  }

	TEdit::EvKillFocus(hWndGetFocus) ;
}
catch (...)
{
	erreur("Exception NSEditDate::EvKillFocus.", standardError, 0) ;
}
}

void
NSEditDate::actionForKillFocus(bool &bCanLetItBe)
{
	//
	// Get content
	//
  string sContent = GetText() ;

	_bEncoursEdition = false ;

	// Vérification de la validité de la date
	string sMessage ;
	donne_date_inverse(sContent, sMessage, _sLang) ;
	if (string("") == sMessage)
	{
		_bCurrentlyWarning = true ;
		erreur("Date invalide.", standardError, 0) ;
		_bCurrentlyWarning = false ;
		_bValidContent     = false ;

    // To prevent an unescapable wrong date
    _sContenuTransfert = string(8, '0') ;
    donneBrut() ;

		SetText(_sContenuBrut.c_str()) ;
		// sContenu = sContenuBrut;

    bCanLetItBe = false ;

		return ;
	}
  bCanLetItBe = true ;

	_bValidContent = true ;

  int iNotifier = 1 ;
	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;

	//
	// Prévient Big Brother (si l'éventuelle fonction l'autorise)
	//
  Message theMessage("", "", "", "A", "", "", "1") ;

	if (1 == iNotifier)
	{		if (string("") == sContent)
		{
			if (string("") != _sContenuBrut)
			{
				_sContenuBrut = string("") ;
				donneTransfert() ;
        theMessage.SetComplement(_sContenuTransfert) ;
        theMessage.SetType(_sType) ;
        if (_pControle && _pControle->getTransfert())
        	_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_UNCHECKED,
                                                       &theMessage) ;
			}
		}
		else
		{
    	_sContenuBrut = sContent ;
      donneTransfert() ;
      theMessage.SetComplement(_sContenuTransfert) ;
      theMessage.SetType(_sType) ;

      if (_pControle && _pControle->getTransfert())
      {
				if (string("") == _sContenuBrut)
      		_pControle->getTransfert()->ctrlNotification(BF_UNCHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
				else
          _pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
			}
		}
	}

	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_POSTEXEC) ;
}

//---------------------------------------------------------------------------//  Function: 		NSEditDate::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par
//						l'utilisateur
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSEditDate::donneBrut()
{
	_sContenuBrut    = string("00/00/0000") ;
	_sContenuBrut[2] = dateSeparationMARK ;
	_sContenuBrut[5] = dateSeparationMARK ;

	//
	// On passe de AAAAMMJJ à JJ/MM/AAAA
	//
	if (string("") != _sContenuTransfert)
	{
  	for (int i = 0 ; i < 4 ; i++)
    	_sContenuBrut[i+6] = _sContenuTransfert[i] ;
    for (int i = 0 ; i < 2 ; i++)
    	_sContenuBrut[i+3] = _sContenuTransfert[i+4] ;
    for (int i = 0 ; i < 2 ; i++)
    	_sContenuBrut[i]   = _sContenuTransfert[i+6] ;

    return ;
	}

  TWindow* pParent = TWindow::GetParentO() ;
	NSDialog* pDlg = TYPESAFE_DOWNCAST(pParent, NSDialog) ;
	NSContexte *pContexte ;
	if ((pDlg) &&(pDlg->_pBBItem) && (pDlg->_pBBItem->_pBigBoss) &&
                                        (pDlg->_pBBItem->_pBigBoss->pContexte))
		pContexte = pDlg->_pBBItem->_pBigBoss->pContexte ;

  string sRefDate = string("") ;

	switch (cRefYear)
  {
  	case 'B' :   //birthday
    	sRefDate = pContexte->getPatient()->donneNaissance() ;
      _sContenuBrut = donne_date(sRefDate, _sLang) ;
      break ;

    case 'N' :    //now
    	sRefDate = donne_date_duJour() ;
      _sContenuBrut = donne_date(sRefDate, _sLang) ;
      break ;
  }
}

//---------------------------------------------------------------------------
//  Function: 		NSEditDate::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//					   en données NAUTILUS
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSEditDate::donneTransfert()
{
	_sContenuTransfert = string("") ;
  //
  // On passe de JJ/MM/AAAA à AAAAMMJJ
  //
  char cValidateur[] = "00/00/0000" ;

  cValidateur[2] = dateSeparationMARK ;
  cValidateur[5] = dateSeparationMARK ;

  if ((strlen(_sContenuBrut.c_str()) >= 10) && (_sContenuBrut != string(cValidateur)))
  {
  	_sContenuTransfert  = string(_sContenuBrut, 6, 4) ;
    _sContenuTransfert += string(_sContenuBrut, 3, 2) ;
    _sContenuTransfert += string(_sContenuBrut, 0, 2) ;
  }
}

//
//  TStatic
//
void
NSEditDate::EvChar(uint key, uint repeatCount, uint flags)
{
  if (_pControle && _pControle->isDisconnected())
    return ;

	if (Validator && (key != VK_BACK)   &&
                   (key != VK_RETURN) &&
                   (key != VK_SPACE) 	&&
                   (key != VK_DELETE) &&
                   (key != VK_TAB))
	{
  	if ((key < '0') || (key > '9'))
    	return ;

    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;

    TEdit::GetText(oldBuff, oldBuffLen+1) ;
    size_t iGenuineLength = strlen(oldBuff) ;

    endSel = startSel ;
    // On doit être certain de ne pas écraser les '/'
    if (2 == startSel)
      startSel = 3 ;
    if (5 == startSel)
      startSel = 8 ;
    endSel = startSel ;

    if (startSel < iGenuineLength)
    {
      oldBuff[startSel] = char(key) ;
      startSel++ ;
      endSel++ ;

      switch (cRefYear)
      {
        case 'F' :    //  future  >=2000
                     // An 20xx
          if ('0' == oldBuff[6])
            oldBuff[6] = '2' ;
          break ;
        case 'P' :       //past   <2000
          if ('0' == oldBuff[6])
          {
            oldBuff[6] = '1' ;
            oldBuff[7] = '9' ;
          }
          break ;
      }
    }

    // On saute les '/'
    if (2 == startSel)
      startSel = 3 ;
    if (5 == startSel)
    {
      if (('F' == cRefYear) || (cRefYear == 'O'))
        startSel = 8 ;
      else
        startSel = 6 ;
    }
    endSel = startSel ;

    SetText(oldBuff) ;
    SetSelection(startSel, endSel) ;
    delete[] oldBuff ;

/*
        int oldBuffLen = GetTextLen();
    	char far* oldBuff = new char[oldBuffLen+1];
        GetText(oldBuff, oldBuffLen+1);

    	uint   startSel, endSel;
    	GetSelection(startSel, endSel);

        // On saute les '/'
        if (startSel == 2)
      	    startSel = 3;
        if (startSel == 5)
      	    startSel = 8;

        //Si la sélection est multi-caractères, on la ramène à 1 car.
        endSel = startSel;
        SetSelection(startSel, endSel);

        bool wasAppending = endSel == oldBuffLen;

  		if ((!wasAppending) && (startSel == endSel))
    	{
            for (int i = startSel; i < oldBuffLen; i++)
         	    oldBuff[i] = oldBuff[i+1];
            SetText(oldBuff);
            SetSelection(startSel, endSel);
        }

    	bool preMsgModify = IsModified();             // Save (pre)  MODIFY flag

    	TStatic::EvChar(key, repeatCount, flags);     // Process the new char...

    	bool postMsgModify= IsModified();             // Save (post) MODIFY flag

    	GetSelection(startSel, endSel);
    	int buffLen = GetTextLen();
    	char far* buff = LockBuffer(max((int)TextLimit,max(oldBuffLen,buffLen))+1);

    	// Run the result of the edit through the validator.  If incorrect,
    	// then restore the original text.  Otherwise, range check & position
    	// the selection as needed.
    	//
    	if (!Validator->HasOption(voOnAppend) || wasAppending && endSel == buffLen)
        {
      	    if (!Validator->IsValidInput(buff, false))
            {
        		strcpy(buff, oldBuff);          // Restore old buffer
        		postMsgModify = preMsgModify;   // Restore old modify state too!
      	    }
      	    else
            {
                if (wasAppending)
          		    startSel = endSel = strlen(buff); // may have autoFilled--move to end
      	    }
      	    UnlockBuffer(buff, true);
      	    SetSelection(startSel, endSel);
        }
    	else
        {
      	    if (endSel == buffLen && !Validator->IsValidInput(buff, false))
                Validator->Error(this);
      	    UnlockBuffer(buff);
    	}
    	SendMessage(EM_SETMODIFY, (TParam1)postMsgModify);

        //
        // On saute les caractères inutiles
        //
    	GetSelection(startSel, endSel);
        if (startSel == endSel)
        {
      	    if (startSel == 2)
      		    SetSelection(3, 3);
      	    if (startSel == 5)
      		    SetSelection(8, 8);
   	    }

    	delete[] oldBuff;
*/
	}
  else if (key == VK_BACK)
  {
    uint startSel, endSel ;
    GetSelection(startSel, endSel);

    // On saute les '/'
    if (3 == startSel)
      startSel = 2 ;
    if (6 == startSel)
      startSel = 5 ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen + 1] ;

    TEdit::GetText(oldBuff, oldBuffLen + 1) ;

    endSel = startSel ;
    if (startSel > 0)
    {
      startSel-- ;
      oldBuff[startSel] = '0' ;
    }
    SetText(oldBuff) ;
    delete[] oldBuff ;

    SetSelection(startSel, endSel) ;
  }
  else if ((false == _bWinStd) && ((VK_RETURN == key) || (VK_TAB == key) || (VK_SPACE == key)))
    //Process this message to avoid message beeps.
    return ;
  else
    TStatic::EvChar(key, repeatCount, flags);
}

void
NSEditDate::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  if (VK_DELETE == key)
  {
    uint iStartSel, iEndSel ;
    GetSelection(iStartSel, iEndSel) ;

    int iOldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[iOldBuffLen + 1] ;
    TEdit::GetText(oldBuff, iOldBuffLen + 1) ;

    string dateVide = "00/00/0000" ;
    string dateEdit = string(oldBuff) ;

    if (iStartSel == iEndSel)
    {
      dateEdit.replace(iStartSel, 1, dateVide, iStartSel, 1);
      iStartSel++ ;
      iEndSel++ ;
    }
    else
    {
      dateEdit.replace(iStartSel, iEndSel-iStartSel, dateVide, iStartSel, iEndSel-iStartSel) ;
      iStartSel = iEndSel ;
    }

    strcpy(oldBuff, dateEdit.c_str()) ;

    SetText(oldBuff) ;
    SetSelection(iStartSel, iEndSel) ;
    delete[] oldBuff ;
  }
  else
    TStatic::EvKeyDown(key, repeatCount, flags) ;
}

bool
NSEditDate::canWeClose()
{
	if (false == _bValidContent)
  	erreur("Date invalide.", standardError, 0) ;
	return _bValidContent ;
}

//******************************************************************
//						TRAITEMENT DES DATES + HEURE
//******************************************************************

//
//----------------------------------------------------------------------
//
DEFINE_RESPONSE_TABLE1(NSEditDateHeure, NSEdit)
	EV_WM_KILLFOCUS,
	EV_WM_KEYDOWN,
	EV_WM_CHAR,
END_RESPONSE_TABLE ;

//
// Constructeur
//
NSEditDateHeure::NSEditDateHeure(NSContexte* pCtx, TWindow* parent, int resId,  char* szTypeEdit, string sUserLang, char cRefDate)
                :NSEdit(pCtx, parent, resId, szTypeEdit, sUserLang)
{
	strcpy(_szType, szTypeEdit);
	//
	// Mise en place du validateur
	//
	char cValidateur[] = "##/##/#### ##:##:##" ;
	cValidateur[2]  = dateSeparationMARK ;
	cValidateur[5]  = dateSeparationMARK ;	cValidateur[13] = heureSeparationMARK ;	cValidateur[16] = heureSeparationMARK ;	SetValidator(new TPXPictureValidator(cValidateur)) ;

	cRefYear = cRefDate ;
}

NSEditDateHeure::NSEditDateHeure(NSContexte* pCtx, TWindow* parent, int resId, char* szTypeEdit,
                const char far* text, int x, int y, int w, int h, string sUserLang, char cRefDate,
                uint textLimit, bool multiline, TModule* module)
                :NSEdit(pCtx, parent, resId, szTypeEdit, text, x, y, w, h, sUserLang, textLimit, multiline, module)
{
	strcpy(_szType, szTypeEdit) ;
	//
	// Mise en place du validateur
	//
	char cValidateur[] = "##/##/#### ##:##:##" ;
	cValidateur[2] = dateSeparationMARK ;
	cValidateur[5] = dateSeparationMARK ;	cValidateur[13] = heureSeparationMARK ;	cValidateur[16] = heureSeparationMARK ;	SetValidator(new TPXPictureValidator(cValidateur)) ;

	cRefYear = cRefDate ;
}

//
// Destructeur
//
NSEditDateHeure::~NSEditDateHeure()
{}

//
// Fonction déclenché lorsque l'Edit perd le focus.
//
void
NSEditDateHeure::EvKillFocus(HWND hWndGetFocus)
{
try
{
	if (_bCurrentlyWarning)
		return ;

  bool bCanLetItBe = true ;
  actionForKillFocus(bCanLetItBe) ;

  if (false == bCanLetItBe)
  {
    TEdit::EvKillFocus(hWndGetFocus) ;
    SetFocus() ;
  }

	TEdit::EvKillFocus(hWndGetFocus) ;
}
catch (...)
{
  erreur("Exception NSEditDateHeure::EvKillFocus.", standardError, 0) ;
}
}

void
NSEditDateHeure::actionForKillFocus(bool &bCanLetItBe)
{
	string sContent = GetText() ;
	_bEncoursEdition = false ;

	// Vérification de la validité de la date
	string sMessage ;
	donne_date_inverse(sContent, sMessage, _sLang) ;
	if (string("") == sMessage)
	{
		_bCurrentlyWarning = true ;
    erreur("Date invalide.", standardError, 0) ;
    _bCurrentlyWarning = false ;
    _bValidContent = false ;

    // To prevent an unescapable wrong date
    _sContenuTransfert = string(14, '0') ;
    donneBrut() ;

    SetText(_sContenuBrut.c_str()) ;
    // sContenu = sContenuBrut;

    bCanLetItBe = false ;

    return ;
	}
  bCanLetItBe = true ;

	_bValidContent = true ;

  int iNotifier = 1 ;
	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;

	//
	// Prévient Big Brother (si l'éventuelle fonction l'autorise)
	//
  Message theMessage("", "", "", "A", "", "", "1") ;

	if (iNotifier == 1)
	{		if (string("") == sContent)
		{
			if (string("") != _sContenuBrut)
			{
				_sContenuBrut = string("") ;
				donneTransfert() ;
        theMessage.SetComplement(_sContenuTransfert) ;
        theMessage.SetType(_sType) ;
        if (_pControle && _pControle->getTransfert())
        	_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_UNCHECKED,
                                                       &theMessage) ;
      }
    }
		else
		{
    	_sContenuBrut = sContent ;
      donneTransfert() ;
      theMessage.SetComplement(_sContenuTransfert) ;
      theMessage.SetType(_sType) ;

      if (_pControle && _pControle->getTransfert())
      {
				if (_sContenuBrut == "")
        	_pControle->getTransfert()->ctrlNotification(BF_UNCHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
				else
					_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
			}
		}
	}

  //
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_POSTEXEC) ;
}

//---------------------------------------------------------------------------//  Description:	Transforme les données NAUTILUS en données à saisir par
//					l'utilisateur
//---------------------------------------------------------------------------
void
NSEditDateHeure::donneBrut()
{
	_sContenuBrut = "00/00/0000 00:00:00" ;
	_sContenuBrut[2] = dateSeparationMARK ;
	_sContenuBrut[5] = dateSeparationMARK ;
	_sContenuBrut[13] = heureSeparationMARK ;
	_sContenuBrut[16] = heureSeparationMARK ;

	// char date[20] ;

	TWindow* pParent = TWindow::GetParentO() ;
  NSDialog* pDlg = TYPESAFE_DOWNCAST(pParent, NSDialog) ;
  NSContexte *pContexte ;
  if ((pDlg) &&(pDlg->_pBBItem) && (pDlg->_pBBItem->_pBigBoss) &&
                                        (pDlg->_pBBItem->_pBigBoss->pContexte))
		pContexte = pDlg->_pBBItem->_pBigBoss->pContexte ;

	string sProcessedDate = _sContenuTransfert ;

	if ((sProcessedDate == string("")) || (strlen(sProcessedDate.c_str()) < 8))
  {
  	sProcessedDate = string("") ;

  	// char szMessage[11] ;

  	switch (cRefYear)
    {
    	case 'B' :   //birthday
        sProcessedDate = pContexte->getPatient()->donneNaissance() ;
        break ;

      case 'N' :    //now
      {
      	NVLdVTemps tpsNow ;
    		tpsNow.takeTime() ;
				sProcessedDate = tpsNow.donneDateHeure() ;
        break ;
      }
    }
  }

	//
	// On passe de AAAAMMJJhhmmss à JJ/MM/AAAA hh:mm:ss
	//
	if (sProcessedDate != string(""))
	{
  	if (strlen(sProcessedDate.c_str()) >= 8)
    {
  		string sDate = donne_date(string(sProcessedDate, 0, 8), _sLang) ;

    	_sContenuBrut = sDate + string(" 00:00:00") ;

      if (strlen(sProcessedDate.c_str()) >= 14)
      {
        for (int i = 0; i < 2; i++)
        {
        	_sContenuBrut[i+11] = sProcessedDate[i+8] ;
          _sContenuBrut[i+14] = sProcessedDate[i+10] ;
          _sContenuBrut[i+17] = sProcessedDate[i+12] ;
        }
      }
    }
	}
}

//---------------------------------------------------------------------------
//  Description:	Transforme les données saisies par l'utilisateur
//					en données NAUTILUS
//---------------------------------------------------------------------------
void
NSEditDateHeure::donneTransfert()
{
	_sContenuTransfert = string("") ;
	//
	// On passe de JJ/MM/AAAA hh:mm:ss à AAAAMMJJhhmmss
	//
	char cValidateur[] = "00/00/0000 00:00:00" ;

	cValidateur[2] = dateSeparationMARK ;
	cValidateur[5] = dateSeparationMARK ;
	cValidateur[13] = heureSeparationMARK ;
	cValidateur[16] = heureSeparationMARK ;

	if (_sContenuBrut != cValidateur)
	{
  	donne_date_inverse(_sContenuBrut, _sContenuTransfert, _sLang) ;
    if (strlen(_sContenuBrut.c_str()) >= strlen(cValidateur))
    {
			_sContenuTransfert += string(_sContenuBrut, 11, 2) ;
			_sContenuTransfert += string(_sContenuBrut, 14, 2) ;
			_sContenuTransfert += string(_sContenuBrut, 17, 2) ;
    }
	}
}

//
//  TStatic
//
void
NSEditDateHeure::EvChar(uint key, uint repeatCount, uint flags)
{
  if (_pControle && _pControle->isDisconnected())
    return ;

	if (Validator && (key != VK_BACK) 	&&
                   (key != VK_RETURN) &&
                   (key != VK_SPACE) 	&&
                   (key != VK_DELETE) &&
                   (key != VK_TAB))
	{
  	if ((key < '0') || (key > '9'))
    	return ;

    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;

    TEdit::GetText(oldBuff, oldBuffLen+1) ;
    size_t iGenuineLength = strlen(oldBuff) ;

    endSel = startSel ;

    // On doit être certain de ne pas écraser les '/'
    if      (2 == startSel)
    	startSel = 3 ;
    else if (5 == startSel)
    	startSel = 8 ;
    else if (10 == startSel)
    	startSel = 11 ;
    else if (13 == startSel)
    	startSel = 14 ;
    else if (16 == startSel)
    	startSel = 17 ;

    endSel = startSel ;

    if (startSel < iGenuineLength)
    {
    	oldBuff[startSel] = char(key) ;
      startSel++ ;
      endSel++ ;

      switch (cRefYear)
      {
      	case 'F' :    //  future  >=2000
        	// An 20xx
          if ('0' == oldBuff[6])
          	oldBuff[6] = '2' ;
          break ;
        case 'P' :       //past   <2000
        	if ('0' == oldBuff[6])
          {
          	oldBuff[6] = '1' ;
            oldBuff[7] = '9' ;
          }
          break ;
      }
    }

    // On saute les '/'
    if      (2 == startSel)
    	startSel = 3 ;
    else if (5 == startSel)
    {
    	if (('F' == cRefYear) || ('O' == cRefYear))
      	startSel = 8 ;
      else
      	startSel = 6 ;
    }
    else if (10 == startSel)
    	startSel = 11 ;
    else if (13 == startSel)
    	startSel = 14 ;
    else if (16 == startSel)
    	startSel = 17 ;

    endSel = startSel ;

    SetText(oldBuff) ;
    delete[] oldBuff ;

    SetSelection(startSel, endSel) ;
	}
  else if (VK_BACK == key)
  {
    uint   startSel, endSel ;
    GetSelection(startSel, endSel) ;

    // On saute les '/'
    if      (3 == startSel)
    	startSel = 2;
    else if (6 == startSel)
    	startSel = 5;
    else if (11 == startSel)
    	startSel = 10 ;
    else if (14 == startSel)
    	startSel = 13 ;
    else if (17 == startSel)
    	startSel = 16 ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;

    TEdit::GetText(oldBuff, oldBuffLen+1) ;

    endSel = startSel ;
    if (startSel > 0)
    {
    	startSel-- ;
      oldBuff[startSel] = '0' ;
    }
    SetText(oldBuff) ;
    delete[] oldBuff ;

    SetSelection(startSel, endSel) ;
  }
  else if ((false == _bWinStd) &&
                ((VK_RETURN == key) || (VK_TAB == key) || (VK_SPACE == key)))
  	//Process this message to avoid message beeps.
    return ;
  else
  	TStatic::EvChar(key, repeatCount, flags) ;
}

void
NSEditDateHeure::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (VK_DELETE == key)
	{
		string dateVide = "00/00/0000 00:00:00" ;

    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    int oldBuffLen = GetTextLen();
    if ((oldBuffLen <= 0) || (uint(oldBuffLen) < strlen(dateVide.c_str())))
    {
    	SetText(dateVide.c_str()) ;
      SetSelection(startSel, endSel) ;
      return ;
    }

    char far* oldBuff = new char[oldBuffLen + 1] ;
    TEdit::GetText(oldBuff, oldBuffLen + 1) ;

    string dateEdit = string(oldBuff) ;

    if (startSel == endSel)
    {
    	dateEdit.replace(startSel, 1, dateVide, startSel, 1) ;
      startSel++ ;
      endSel++ ;
    }
    else
    {
    	dateEdit.replace(startSel, endSel-startSel, dateVide, startSel, endSel-startSel) ;
      startSel = endSel ;
    }

    strcpy(oldBuff, dateEdit.c_str()) ;

    SetText(oldBuff) ;
    delete[] oldBuff ;

    SetSelection(startSel, endSel) ;
	}
	else
		TStatic::EvKeyDown(key, repeatCount, flags) ;
}

bool
NSEditDateHeure::canWeClose()
{
	if (false == _bValidContent)
  	erreur("Date invalide.", standardError, 0) ;
	return _bValidContent ;
}

//******************************************************************
//									TRAITEMENT DES HEURES
//******************************************************************

////----------------------------------------------------------------------
//
DEFINE_RESPONSE_TABLE1(NSEditHeure, NSEdit)
    EV_WM_KILLFOCUS,
    EV_WM_CHAR,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSEditHeure::NSEditHeure(NSContexte* pCtx, TWindow* parent, int resId,  char* szTypeEdit, string sUserLang)
            :NSEdit(pCtx, parent, resId, szTypeEdit, sUserLang)
{
	strcpy(_szType, szTypeEdit) ;
	//
	// Mise en place du validateur
	//
  char cValidateur[] = "##:##" ;
  cValidateur[2] = heureSeparationMARK ;
  SetValidator(new TPXPictureValidator(cValidateur)) ;
}

NSEditHeure::NSEditHeure(NSContexte* pCtx, TWindow* parent, int resId, char* szTypeEdit,
                const char far* text, int x, int y, int w, int h, string sUserLang,
                uint textLimit, bool multiline, TModule* module)
            : NSEdit(pCtx, parent, resId, szTypeEdit, text, x, y, w, h, sUserLang, textLimit, multiline, module)
{
	strcpy(_szType, szTypeEdit) ;
	//
	// Mise en place du validateur
	//
  char cValidateur[] = "##:##" ;
  cValidateur[2] = heureSeparationMARK ;
  SetValidator(new TPXPictureValidator(cValidateur)) ;
}

//
// Destructeur
//
NSEditHeure::~NSEditHeure()
{}

//---------------------------------------------------------------------------//  Fonction: 		NSEditHeure::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par
//						l'utilisateur
//---------------------------------------------------------------------------
void
NSEditHeure::donneBrut()
{
	//
  // On passe de mmss à mm:ss
  //
  if (strlen(_sContenuTransfert.c_str()) >= 4)
    _sContenuBrut = string(_sContenuTransfert, 0, 2) +
                    string(1, heureSeparationMARK) +
                    string(_sContenuTransfert, 2, 2) ;
	else
	{
  	_sContenuBrut = "00:00" ;
    _sContenuBrut[2] = heureSeparationMARK ;
	}
}

//---------------------------------------------------------------------------//  Fonction: 		NSEditHeure::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//					   en données NAUTILUS
//---------------------------------------------------------------------------
void
NSEditHeure::donneTransfert()
{
	_sContenuTransfert = string("") ;
  //
  // On passe de mm:ss à mmss
  //
  char cValidateur[] = "00:00" ;
  cValidateur[2] = heureSeparationMARK ;
  if (_sContenuBrut != cValidateur)
    _sContenuTransfert = string(_sContenuBrut, 0, 2) + string(_sContenuBrut, 3, 2) ;
}
//
//  TStatic
//
void
NSEditHeure::EvChar(uint key, uint repeatCount, uint flags)
{
  if (_pControle && _pControle->isDisconnected())
    return ;

  if (Validator && (VK_BACK   != key) &&
   					       (VK_RETURN != key) &&
                   (VK_SPACE  != key) &&
                   (VK_DELETE != key) &&
                   (VK_TAB    != key))
  {
    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen + 1] ;
    TEdit::GetText(oldBuff, oldBuffLen + 1) ;
    size_t iGenuineSize = strlen(oldBuff) ;

    size_t startSel, endSel ;
    GetSelection(startSel, endSel) ;

    // On saute le ':'
    if (2 == startSel)
      startSel = 3 ;

    // Si la sélection est multi-caractères, on la ramène à 1 car.
    endSel = startSel ;
    SetSelection(startSel, endSel) ;

    bool wasAppending = endSel == iGenuineSize ;

    if ((false == wasAppending) && (startSel == endSel))
    {
      for (int i = startSel; i < oldBuffLen; i++)
        oldBuff[i] = oldBuff[i+1] ;
      SetText(oldBuff) ;
      SetSelection(startSel, endSel) ;
    }

    bool preMsgModify = IsModified();             // Save (pre)  MODIFY flag

    TStatic::EvChar(key, repeatCount, flags);     // Process the new char...

    bool postMsgModify= IsModified();             // Save (post) MODIFY flag

    GetSelection(startSel, endSel) ;
    int buffLen = GetTextLen() ;
    char far* buff = LockBuffer(max((int)TextLimit,max(oldBuffLen,buffLen))+1) ;

    // Run the result of the edit through the validator.  If incorrect,
    // then restore the original text.  Otherwise, range check & position
    // the selection as needed.
    //
    if (!Validator->HasOption(voOnAppend) || wasAppending && (int(endSel) == buffLen))
    {
      if (!Validator->IsValidInput(buff, false))
      {
        strcpy(buff, oldBuff) ;          // Restore old buffer
        postMsgModify = preMsgModify ;   // Restore old modify state too!
      }
      else
      {
        if (wasAppending)
          startSel = endSel = strlen(buff); // may have autoFilled--move to end
      }
      UnlockBuffer(buff, true) ;
      SetSelection(startSel, endSel) ;
    }
    else
    {
      if ((int(endSel) == buffLen) && !Validator->IsValidInput(buff, false))
        Validator->Error(this) ;
      UnlockBuffer(buff) ;
    }
    SendMessage(EM_SETMODIFY, (TParam1)postMsgModify) ;

    //
    // On saute les caractères inutiles
    //
    GetSelection(startSel, endSel) ;
    if (startSel == endSel)
    {
      if (2 == startSel)
        SetSelection(3, 3) ;
    }

    delete[] oldBuff;
	}
  else if (VK_BACK == key)
  {
    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    // On saute le ':'
    if (3 == startSel)
      startSel = 2 ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;
    TEdit::GetText(oldBuff, oldBuffLen+1) ;

    if (startSel > 0)
    {
      startSel-- ;
      oldBuff[startSel] = '0' ;
    }
    endSel = startSel ;
    SetText(oldBuff) ;
    delete[] oldBuff ;

    SetSelection(startSel, endSel) ;
  }
  else if ((false == _bWinStd) && ((VK_RETURN == key) || (VK_TAB == key)))
    //Process this message to avoid message beeps.
    return ;
  else if ((false == _bWinStd) && (VK_SPACE == key))
    //Process this message to avoid message beeps.
    return ;
  else
    TStatic::EvChar(key, repeatCount, flags) ;
}

//// Fonction déclenché lorsque l'Edit perd le focus.
//
void
NSEditHeure::EvKillFocus(HWND hWndGetFocus)
{
	int iNotifier = 1 ;

	string sContenu = GetText() ;
	_bEncoursEdition = false ;

	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;

	//	// Prévient Big Brother (si l'éventuelle fonction l'autorise)
	//
  Message theMessage("", "", "", "A", "", "", "1") ;

	if (1 == iNotifier)
	{
		if (string("") == sContenu)
		{
			if (string("") != _sContenuBrut)
			{
				_sContenuBrut = string("") ;
				donneTransfert() ;
        theMessage.SetComplement(_sContenuTransfert) ;
        theMessage.SetType(_sType) ;
        if (_pControle && _pControle->getTransfert())
					_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_UNCHECKED,
                                                       &theMessage) ;
			}
		}
		else
		{
    	_sContenuBrut = sContenu ;
      donneTransfert() ;
      theMessage.SetComplement(_sContenuTransfert) ;
      theMessage.SetType(_sType) ;

      if (_pControle && _pControle->getTransfert())
      {
				if (string("") == _sContenuBrut)
					_pControle->getTransfert()->ctrlNotification(BF_UNCHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
				else
					_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
			}
		}
	}

	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_POSTEXEC) ;

	TEdit::EvKillFocus(hWndGetFocus) ;
}

//******************************************************************////                  TRAITEMENT DES EDIT NO LEXIQUE////******************************************************************DEFINE_RESPONSE_TABLE1(NSEditNoLex, NSEdit)	EV_WM_KEYUP,
	EV_WM_CHAR,
	EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

//// Constructeur
//
NSEditNoLex::NSEditNoLex(NSContexte* pCtx, TWindow* parent, int resId,  char* szTypeEdit, string sUserLang, int iTextLimit)
            :NSEdit(pCtx, parent, resId, szTypeEdit, sUserLang)
{
	string sValidateur = "" ;
	_sContenuBrut      = string("") ;
	_sContenuTransfert = string("") ;
	_pControle = 0 ;
	DisableTransfer() ;
	strcpy(_szType, szTypeEdit) ;

	_bEncoursEdition = false ;	_parentEdit = parent ;
	TextLimit = iTextLimit ;

	//	// Mise en place du validateur
	//

	// SetValidator(new TFilterValidator("0-9A-Za-z. "));}

NSEditNoLex::NSEditNoLex(NSContexte* pCtx, TWindow* parent, int resId, char* szTypeEdit,
                const char far* text, int x, int y, int w, int h, string sUserLang,
                uint textLimit, bool multiline, TModule* module)
            : NSEdit(pCtx, parent, resId, szTypeEdit, text, x, y, w, h, sUserLang, textLimit, multiline, module)
{
	string sValidateur = "" ;
	_sContenuBrut      = string("") ;
	_sContenuTransfert = string("") ;
	_pControle = 0 ;
	DisableTransfer() ;
	strcpy(_szType, szTypeEdit) ;

	_bEncoursEdition = false ;	_parentEdit = parent ;
	TextLimit = textLimit ;

	//	// Mise en place du validateur
	//

	// SetValidator(new TFilterValidator("0-9A-Za-z. "));
}

//// Destructeur
//
NSEditNoLex::~NSEditNoLex()
{}

//// Fonction déclenché lorsque l'Edit perd le focus.
//
void
NSEditNoLex::EvKillFocus(HWND hWndGetFocus)
{
  bool bCanLetItBe = false ;
  actionForKillFocus(bCanLetItBe) ;
  if (false == bCanLetItBe)
    return ;

  /*********************************************
  //
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (pControle && pControle->getFonction())
		iNotifier = pControle->getFonction()->execute(NSDLGFCT_POSTEXEC, hWndGetFocus);
    *******************************************/

	TEdit::EvKillFocus(hWndGetFocus) ;

	if (_pControle && _pControle->getFonction())
		_pControle->getFonction()->execute(NSDLGFCT_POSTEXEC, hWndGetFocus) ;
}

void
NSEditNoLex::actionForKillFocus(bool &bCanLetItBe)
{
  bool bExecPostExec = bCanLetItBe ;

	int iNotifier = 1 ;

	string sContent = GetText() ;
	_bEncoursEdition = false ;

	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (_pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;

	//
	// Prévient Big Brother (si l'éventuelle fonction l'autorise)
	//
  Message theMessage ;

	if (1 == iNotifier)
	{
		if (string("") == sContent)
		{
			if (string("") != _sContenuBrut)
			{
				_sContenuBrut = string("") ;
				donneTransfert();
        if ('L' == _szType[0])
        	theMessage.SetTexteLibre(_sContenuTransfert) ;
        else
        	theMessage.SetComplement(_sContenuTransfert) ;
        theMessage.SetType(_sType);
        if (_pControle && _pControle->getTransfert())
        	_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_UNCHECKED,
                                                       &theMessage) ;
      }
    }
		else
		{
    	_sContenuBrut = sContent ;
      if      ('N' == _szType[1])
      	vraiemaj(&_sContenuBrut) ;
      else if ('P' == _szType[1])
      	// firstmaj(&sContenuBrut) ;
        _sContenuBrut = getProperFirstName(_sContenuBrut) ;

      donneTransfert() ;
      if ('L' == _szType[0])
      	theMessage.SetTexteLibre(_sContenuTransfert) ;
      else
      	theMessage.SetComplement(_sContenuTransfert) ;
      theMessage.SetType(_sType) ;

      if (_pControle && _pControle->getTransfert())
      {
				if (string("") == _sContenuBrut)
        	_pControle->getTransfert()->ctrlNotification(BF_UNCHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
				else
        	_pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                       BF_CHECKED,
                                                       &theMessage) ;
      }
    }  }

  //
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (bExecPostExec && _pControle && _pControle->getFonction())
		iNotifier = _pControle->getFonction()->execute(NSDLGFCT_POSTEXEC) ;

  bCanLetItBe = true ;
}
voidNSEditNoLex::executeKillFocusBehaviour(){  if ((NULL == _pControle) || (NULL == _pControle->getTransfert()))    return ;	string sDataToCome = _pControle->getTransfert()->getTransfertMessage()->GetTexteLibre() ;	if      (_szType[1] == 'N')  	vraiemaj(&sDataToCome) ;
  else if (_szType[1] == 'P')
    // firstmaj(&sDataToCome) ;    sDataToCome = getProperFirstName(sDataToCome) ;	_pControle->getTransfert()->getTransfertMessage()->SetTexteLibre(sDataToCome) ;}//---------------------------------------------------------------------------
//  Function: 		NSEditNoLex::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//					   en données NAUTILUS
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------

voidNSEditNoLex::donneTransfert()
{
	signed int i = strlen(_sContenuBrut.c_str()) ;
	if (i > 0)
  {
  	i-- ;
    for (; (i >= 0) && (' ' == _sContenuBrut[i]); i--)
    	_sContenuBrut[i] = '\0' ;
  }
  if (_sContenuBrut != "")
  	_sContenuTransfert = _sContenuBrut ;
  else
  	_sContenuTransfert = string("") ;
}

//---------------------------------------------------------------------------
//  Function: 		NSEdit::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par//						l'utilisateur
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void NSEditNoLex::donneBrut()
{
	_sContenuBrut = _sContenuTransfert ;
}

//---------------------------------------------------------------------------
//  Function: 		NSEdit::activeControle(int activation, string message)
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
NSEditNoLex::activeControle(int activation, Message* pMessage)
{
  // Gestion windows standard
  // Windows standard behaviour
  if (_pControle && _pControle->getNSDialog())
  {
    NSContexte* pContexte = _pControle->getNSDialog()->pContexte ;
    if (pContexte && pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
      _bWinStd = true ;
  }

	switch (activation)
	{
		case BF_CHECKED   :

      _sContenuTransfert = pMessage->GetTexteLibre() ;
      _sType             = pMessage->GetType() ;
      donneBrut() ;
      SetText(_sContenuBrut.c_str()) ;
      break ;

		case BF_UNCHECKED :

      _sContenuTransfert = string("") ;
      donneBrut() ;
      SetText(_sContenuBrut.c_str()) ;
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
NSEditNoLex::Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (tdSetData == direction)
	{
		switch (*pActif)
		{
			case inactiveCtrl :

      	_sContenuTransfert = string("") ;
        donneBrut() ;
        SetText(_sContenuBrut.c_str()) ;
        break ;

			case activeCtrl :

      	if ('L' == _szType[0])
        	_sContenuTransfert = pMessage->GetTexteLibre() ;
        else
        	_sContenuTransfert = pMessage->GetComplement() ;
        _sType = pMessage->GetType() ;
        donneBrut() ;
        SetText(_sContenuBrut.c_str()) ;
        break ;
		}
	}
  else if (tdGetData == direction)
	{
  	if (_bEncoursEdition)
      _sContenuBrut = GetText() ;

    donneTransfert() ;
    if (string("") == _sContenuTransfert)
    	*pActif = inactiveCtrl ;
    else
    	*pActif = activeCtrl ;

    if (pMessage)
    {
    	if ('L' == _szType[0])
      	pMessage->SetTexteLibre(_sContenuTransfert) ;
      else
      	pMessage->SetComplement(_sContenuTransfert) ;
      pMessage->SetType(_sType) ;

      string sEtiquette = string("") ;
      if (_pControle && _pControle->getTransfert())
      	sEtiquette = _pControle->getTransfert()->getFilsItem()->getItemLabel() ;

      pMessage->MettreAJourCertitudePluriel(sEtiquette) ;
    }
  }

	return 1 ;
}

uint
NSEditNoLex::TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (_bEncoursEdition)
    _sContenuBrut = GetText() ;

  donneTransfert() ;

  if (string("") == _sContenuTransfert)
  	*pActif = inactiveCtrl ;
  else
  	*pActif = activeCtrl ;

  if ('L' == _szType[0])
  	pMessage->SetTexteLibre(_sContenuTransfert) ;
  else
  	pMessage->SetComplement(_sContenuTransfert) ;
  pMessage->SetType(_sType) ;

  string sEtiquette = string("") ;
  if (_pControle && _pControle->getTransfert())
  	sEtiquette = _pControle->getTransfert()->getFilsItem()->getItemLabel() ;

  pMessage->MettreAJourCertitudePluriel(sEtiquette) ;

	return 1 ;
}

string
NSEditNoLex::getProperFirstName(string sInitialEntry)
{
  if (string("") == sInitialEntry)
    return string("") ;

  string sReturn = sInitialEntry ;
  firstmaj(&sReturn) ;

  //
  // Load the file
  //
  string sFileName = pContexte->PathName("FPER") + string("firstNames.txt") ;

  ifstream inFile ;
  inFile.open(sFileName.c_str()) ;
  if (!inFile)
    return sReturn ;

  int iCounter = 0 ;
  int iLoopThreshold = 100 ;

  string sPseumajEntry = pseumaj(sInitialEntry) ;

  string sLine ;
  while (false == inFile.eof())
  {
		getline(inFile, sLine) ;

    strip(sLine, stripBoth) ;
    string sLineText = pseumaj(sLine) ;

    if (sLineText == sPseumajEntry)
    {
      sReturn = sLine ;
      break ;
    }

    iCounter++ ;
    if (iCounter > iLoopThreshold)
    {
      iCounter = 0 ;
      pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
    }
  }

  inFile.close() ;

  return sReturn ;
}

//******************************************************************//						  TRAITEMENT DES STATICS
//******************************************************************

DEFINE_RESPONSE_TABLE1(NSStatic, TStatic)
	EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

// Constructeur
NSStatic::NSStatic(NSContexte* pCtx, TWindow* parent, int resId)
         :TStatic(parent, resId), NSRoot(pCtx)
{
	pControle = 0 ;
	DisableTransfer() ;
}

NSStatic::NSStatic(NSContexte* pCtx, TWindow* parent, int resId, const char far* title,
                    int x, int y, int w, int h,
                    uint textLimit, TModule* module)
            :TStatic(parent, resId, title, x, y, w, h, textLimit, module), NSRoot(pCtx)
{
	pControle = 0 ;
	DisableTransfer() ;
}

// Destructeur
NSStatic::~NSStatic()
{
}

// Fonction de transfert de données entre big brother et le contrôle// Pour un contrôle Static, il ne se passe rien
uint
NSStatic::Transferer(TTransferDirection /* direction */, CTRL_ACTIVITY* /* pActif */,
                                                            Message* /* pMessage */)
{
	return 1 ;
}

uint
NSStatic::TempTransferer(CTRL_ACTIVITY* /* pActif */, Message* /* pMessage */)
{
	return 1 ;
}

void
NSStatic::SetupWindow()
{
	TStatic::SetupWindow() ;

	// TWindow*  pParent = TWindow::GetParentO() ;
	// NSDialog* pDlg    = TYPESAFE_DOWNCAST(pParent, NSDialog) ;

  NSSuper*  pSuper  = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

void
NSStatic::EvSetFocus(HWND /* hWndLostFocus */)
{
	// si on gagne le focus, on s'en débarrasse aussitôt
	if (pControle && pControle->getNSDialog())
		pControle->getNSDialog()->ActiveControlSuivant(pControle) ;
}

bool
NSStatic::canWeClose()
{
	return true ;
}

//
// --------- Display Caps Lock status ---------
//
DEFINE_RESPONSE_TABLE1(NSModeStatic, TStatic)
	EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

// Constructeur
NSModeStatic::NSModeStatic(NSContexte* pCtx, TWindow* parent, int resId)
             :TStatic(parent, resId), NSRoot(pCtx)
{
	DisableTransfer() ;
}

NSModeStatic::NSModeStatic(NSContexte* pCtx, TWindow* parent, int resId, const char far* title,
                    int x, int y, int w, int h,
                    uint textLimit, TModule* module)
             :TStatic(parent, resId, title, x, y, w, h, textLimit, module), NSRoot(pCtx)
{
	DisableTransfer() ;
}

// Destructeur
NSModeStatic::~NSModeStatic()
{
}

voidNSModeStatic::SetupWindow()
{
	TStatic::SetupWindow() ;

	// TWindow*  pParent = TWindow::GetParentO() ;
	// NSDialog* pDlg    = TYPESAFE_DOWNCAST(pParent, NSDialog) ;

  NSSuper* pSuper  = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

// Derived from TModeGadget
//
bool
NSModeStatic::IdleAction(long count)
{
  TStatic::IdleAction(count) ;

  string sText = pContexte->getSuperviseur()->getText("loginDialog", "userIdentification") ;
  string loginText = pContexte->getSuperviseur()->getText("loginDialog", "userLogin") ;

  int state = ::GetKeyState(VK_CAPITAL) ;
  if (state & 1)
    sText = pContexte->getSuperviseur()->getText("0localInformation", "capsLockOn") ;
  else
    sText = pContexte->getSuperviseur()->getText("0localInformation", "capsLockOff") ;

  SetText(sText.c_str()) ;

  return false ;
}

bool
NSModeStatic::canWeClose()
{
	return true ;
}

