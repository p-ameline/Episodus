#include <stdio.h>
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"#include "nsbb\nsednum.h"
#include "nsbb\nsutidlg.h"
#include "nsbb\nsbb.h"
#include "partage\nsdivfct.h"
#include "nautilus\nsepicap.h"

//
// General Controls diagnostic group
//
DIAG_DEFINE_GROUP_INIT(OWL_INI, OwlControl, 1, 0);

const char decimSepar = ',' ;   // Edit double bouton

// ----------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUtilEdit, TEdit)
	// EV_WM_SETFOCUS,
	EV_WM_KEYUP,
	EV_WM_GETDLGCODE,
	EV_WM_CHAR,
	EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSUtilEdit::NSUtilEdit(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int resId, int iTextLen, bool bReturn, TModule* module)
           :TEdit((TWindow*)pUtilDialog, resId, iTextLen+1, module), NSRoot(pCtx)
{
	_pNSUtilDialog = pUtilDialog ;
  _bIntercepte   = bReturn ;        // permet de shunter le 1er Return ou Tab

  initCommonData() ;
}

NSUtilEdit::NSUtilEdit(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int Id, const char far* text, int x, int y, int w, int h, int iTextLen, bool multiline, bool bReturn, TModule* module)
           :TEdit((TWindow*)pUtilDialog, Id, text, x, y, w, h, iTextLen+1, multiline, module), NSRoot(pCtx)
{
	_pNSUtilDialog = pUtilDialog ;
  _bIntercepte   = bReturn ;        // permet de shunter le 1er Return ou Tab

  initCommonData() ;
}

void
NSUtilEdit::initCommonData()
{
	if (_pNSUtilDialog)
		_pNSUtilDialog->ReferenceControl(this) ;

	_LostFocusFunctor = NULL ;

	_bWinStd = false ;
	if (_pNSUtilDialog && _pNSUtilDialog->pContexte)
	{
  	NSContexte* pContexte = _pNSUtilDialog->pContexte ;
    if (pContexte && pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
    	_bWinStd = true ;
	}
}

//
// Destructeur
//
NSUtilEdit::~NSUtilEdit()
{
}

void
NSUtilEdit::SetupWindow()
{
	TEdit::SetupWindow() ;
}

string
NSUtilEdit::GetText(size_t iSizeLimit)
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
NSUtilEdit::SetText(const string sContent)
{
  TEdit::SetText(sContent.c_str()) ;
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
void
NSUtilEdit::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  // Gestion windows standard
  // Windows standard behaviour
  if (_bWinStd)
  {
    // return TEdit::EvKeyUp(key, repeatcount, flags) ;
    TEdit::EvKeyUp(key, repeatcount, flags) ;
    return ;
  }

  switch(key)
  {
    //return
    case VK_RETURN  :
    case VK_DOWN    :
      //
      //demander à la boîte de dialogue mère d'activer le controle
      //suivant pControle sinon le premier
      //
      if (_bIntercepte)
        _pNSUtilDialog->ActiveControlSuivant(this) ;
      else
        _bIntercepte = true ;
      break ;

    //?????????????? PENSER A LE FAIRE    /*
    case VK_UP     : 	//
      						//demander à la boîte de dialogue mère d'activer le controle
                        //précédent pControle sinon le dernier
                        //
                        if (bIntercepte)
	      						pNSUtilDialog->ActiveControlPrecedent(this);
                        else
                        	bIntercepte = true;
                        break;
    */
    //tabulation
    case VK_TAB	:
      //
      //demander à la boîte de dialogue mère d'activer le controle
      //suivant pControle sinon le premier
      //
      if (false == _bIntercepte)
        _bIntercepte = true ;
      _pNSUtilDialog->ActiveControlSuivant(this) ;
      break ;

    default       :
      if (VK_F1 == key)
        _pNSUtilDialog->CmHelp() ;
      if (false == _bIntercepte)
        _bIntercepte = true ;
      TEdit::EvKeyUp(key, repeatcount, flags) ;
  }
}

//-------------------------------------------------------------------------
// enlever les beeps
//-------------------------------------------------------------------------
void
NSUtilEdit::EvChar(uint key, uint repeatCount, uint flags)
{
	if ((false == _bWinStd) && ((VK_RETURN == key) || (VK_TAB == key)))
		//Process this message to avoid message beeps.
		return ;
	else
		TEdit::EvChar(key, repeatCount, flags) ;
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSUtilEdit::EvGetDlgCode(MSG far* /* msg */)
{
	uint retVal = (uint)DefaultProcessing() ;
	if (false == _bWinStd)
		retVal |= DLGC_WANTALLKEYS ;
	return retVal ;
}

void
NSUtilEdit::EvKillFocus(HWND hWndGetFocus)
{
  fireLostFocusFunctor() ;

  if (0 != hWndGetFocus)
    TEdit::EvKillFocus(hWndGetFocus) ;
}

void
NSUtilEdit::fireLostFocusFunctor()
{
	if (NULL != _LostFocusFunctor)
		(*_LostFocusFunctor)() ;
}

// ----------------------------------------------------------------------
//
//						NSUtilEdit2 : pour les saisies
//
// ----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUtilEdit2, NSUtilEdit)
END_RESPONSE_TABLE;

//
// Constructeur
//
NSUtilEdit2::NSUtilEdit2(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int resId, int iTextLen)
            :NSUtilEdit(pCtx, pUtilDialog, resId, iTextLen)
{
	_ciTexteLen = iTextLen + 1 ;
}

//
// Destructeur
//
NSUtilEdit2::~NSUtilEdit2()
{
}

void
NSUtilEdit2::SetupWindow()
{
	NSUtilEdit::SetupWindow() ;
}

//
// Fonction redéfinie GetText pour récupérer une string
//
void
NSUtilEdit2::GetText(string& sResultText)
{
  int iTextLen = _ciTexteLen ;
  if (iTextLen <= 1)
    iTextLen = GetTextLen() + 1 ;

  char far *texte = new char[iTextLen] ;

  TEdit::GetText(texte, iTextLen) ;
  _sTexte = string(texte) ;

  delete[] texte ;

  sResultText = _sTexte ;
}

// ----------------------------------------------------------------------
//
//				 NSMultiEdit : pour les NSUtilEdit multilignes
//
// ----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSMultiEdit, NSUtilEdit2)
    EV_WM_KEYUP,
    EV_WM_CHAR,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSMultiEdit::NSMultiEdit(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int resId, int iTextLen)
            :NSUtilEdit2(pCtx, pUtilDialog, resId, iTextLen)
{
}

//// Destructeur
//
NSMultiEdit::~NSMultiEdit()
{
}

void
NSMultiEdit::SetupWindow()
{
	NSUtilEdit2::SetupWindow() ;
}

voidNSMultiEdit::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  if ((VK_RETURN == key) || (VK_DOWN))
    TEdit::EvKeyUp(key, repeatcount, flags) ;
  else
    NSUtilEdit::EvKeyUp(key, repeatcount, flags) ;
}

voidNSMultiEdit::EvChar(uint key, uint repeatCount, uint flags)
{
  if (VK_RETURN == key)
    TEdit::EvChar(key, repeatCount, flags) ;
  else
    NSUtilEdit::EvChar(key, repeatCount, flags) ;
}

// ----------------------------------------------------------------------
//
//           		 classe NSFilterValidator (pour NSEditNum)
//
// ----------------------------------------------------------------------
NSFilterValidator::NSFilterValidator(NSContexte* pCtx, string sCharSet)                  :TFilterValidator(sCharSet.c_str()), NSRoot(pCtx)
{
	_sValidator = sCharSet ;
}

NSFilterValidator::~NSFilterValidator()
{
}

// on redéfinit la fonction Error
void
NSFilterValidator::Error(TWindow* /* owner */)
{
	char far msg[255] ;

	// PRECONDITION(owner) ;
	sprintf(msg, "Caractère non valide : CharSet = [%s].", _sValidator.c_str()) ;
	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
	pContexte->GetMainWindow()->MessageBox(msg, sCaption.c_str(), MB_OK) ;
}

// ----------------------------------------------------------------------
//
//           					classe NSEditNum
//
// ----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSEditNum, NSUtilEdit)
	EV_WM_KILLFOCUS,
	EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

NSEditNum::NSEditNum(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int resId, int iTextLen, int iDecimales,
						string sValidator)
          :NSUtilEdit(pCtx, pUtilDialog, resId, iTextLen + 1)
{
	_sContenuBrut      = string("") ;
	_sContenuTransfert = string("") ;

  //ParentNSEditNum  = (TWindow*)pNSUtilDialog;
  _dValeur = 0 ;

	DisableTransfer() ;
  //
  _iDecimale = iDecimales ;
	_iMaxInput = iTextLen ;

	//
	// Mise en place du validateur
	//
  _pFilterValidator = new NSFilterValidator(pCtx, sValidator) ;
  SetValidator(_pFilterValidator) ;

  	/* if (iDecimale == 0)
   			SetValidator(new TFilterValidator("0-9"));*/
    /* else
   		{
        string sValidateur = "" ;
   			sValidateur = string(iMaxInput+1, '#');
      		sValidateur[iMaxInput-iDecimale-1] = ';';
      		sValidateur[iMaxInput-iDecimale]   = decimSepar;
      		SetValidator(new TPXPictureValidator(sValidateur.c_str(), 1));
   		}  */
}

//
// Destructeur
//
NSEditNum::~NSEditNum()
{
	// delete pFilterValidator;
}

void
NSEditNum::setNum(string sNum)
{
	_sContenuTransfert = sNum ;
  donneBrut() ;
  SetText(_sContenuBrut.c_str()) ;
}

void
NSEditNum::setNum(double dNum)
{
  string sNum = DoubleToString(&dNum, -1, -1) ;
  setNum(sNum) ;
}

void
NSEditNum::MettreAJourValeur(string sContenu)
{
	//
	// Elaboration de sContenuTransfert
	//
  _sContenuBrut = sContenu ;
  donneTransfert() ;
  donneValeur() ;
}

//// Fonction déclenché lorsque l'Edit perd le focus.
//
void
NSEditNum::EvKillFocus(HWND hWndGetFocus)
{
	// Getting text
	//
	string sContenu = GetText() ;

	//
	// Elaboration de sContenuTransfert
	//
	MettreAJourValeur(sContenu) ;

  fireLostFocusFunctor() ;

  if (0 != hWndGetFocus)
    TEdit::EvKillFocus(hWndGetFocus) ;
}

//---------------------------------------------------------------------------//  Function: 		NSEditNum::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par
//						l'utilisateur
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSEditNum::donneBrut()
{
  if (string("") == _sContenuTransfert)
  {
    //sContenuBrut = "0" + "," + string(iDecimale, '0');
    _sContenuBrut = string("") ;
    return ;
  }

	size_t iDecimalCharPos = _sContenuTransfert.find('.') ;
	//
	// Pour les entiers, on supprime les '0' initiaux
	//
	if (NPOS == iDecimalCharPos)
	{
  	if (string("") == _sContenuTransfert)
    	_sContenuBrut = string("") ;
    else
    {
    	size_t i = _sContenuTransfert.find_first_not_of('0') ;
      if (NPOS == i)
      	_sContenuBrut = string("0") ;
      else
      	_sContenuBrut = string(_sContenuTransfert, i, strlen(_sContenuTransfert.c_str())-i) ;
    }
    return ;
  }

	// Pour les décimaux, on positionne d'abord la virgule
	//
  string sDecimalSeparator = string(1, decimSepar) ;
  if (_pNSUtilDialog && _pNSUtilDialog->pContexte)
    sDecimalSeparator = _pNSUtilDialog->pContexte->getSuperviseur()->getText("0localInformation", "decimalSeparator") ;

  size_t iTransfertSize = strlen(_sContenuTransfert.c_str()) ;

  string sIntValue = string(_sContenuTransfert, 0, iDecimalCharPos) ;
  string sDecValue = string(_sContenuTransfert, iDecimalCharPos + 1, iTransfertSize - iDecimalCharPos - 1) ;

  size_t i = sIntValue.find_first_not_of('0') ;
  if (NPOS == i)
    sIntValue = string("0") ;
  else
    sIntValue = string(sIntValue, i, strlen(sIntValue.c_str())-i) ;

  _sContenuBrut = sIntValue + sDecimalSeparator + sDecValue ;

/*
  size_t i = sContenuBrut.find_first_not_of('0') ;
  size_t j = min(i, iMaxInput-3-iDecimale) ;
  if (j > 0)
    sContenuBrut = string(sContenuBrut, i, strlen(sContenuBrut.c_str())-i) ;
*/
}

//---------------------------------------------------------------------------//  Function: 		NSEditNum::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//					   en données NAUTILUS
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSEditNum::donneTransfert()
{
	strip(_sContenuBrut) ;

	if ((string("") == _sContenuBrut) || (!_sContenuBrut.find_first_of("0123456789") == 0))
	{
  	_sContenuTransfert = string("") ;
    return ;
  }

  // Recherche du premier caract différent de '0'
  //
  size_t k = _sContenuBrut.find_first_not_of('0') ;
  //
  // ATTENTION : on suppose que 0 = rien
  //
  if (NPOS == k)
  {
  	_sContenuTransfert = string("") ;
    return ;
  }

  string sDecimalSeparator = string(1, decimSepar) ;
	if (_pNSUtilDialog && _pNSUtilDialog->pContexte)
		sDecimalSeparator = _pNSUtilDialog->pContexte->getSuperviseur()->getText("0localInformation", "decimalSeparator") ;
  size_t i = _sContenuTransfert.find(sDecimalSeparator) ;

  // on teste si on a 0,XX (il ne faut pas ôter le 0)
  // ou 00035,XX (il faut ôter les 0 initiaux)
  if (i == k)
  {
  	if (0 == k)
    	_sContenuTransfert = string("0") + _sContenuBrut ;
    else
    	_sContenuTransfert = string(_sContenuBrut, k-1, strlen(_sContenuBrut.c_str())-k+1) ;
  }
	else
		_sContenuTransfert = string(_sContenuBrut, k, strlen(_sContenuBrut.c_str())-k) ;

	// Replace local decimal separator by '.'
	if (NPOS != i)
	{
		size_t iSeparLen = strlen(sDecimalSeparator.c_str()) ;
    size_t iTextLen  = strlen(_sContenuTransfert.c_str()) ;

		if (0 == i)
    	_sContenuTransfert = string("0") + string(".") + string(_sContenuTransfert, iSeparLen, iTextLen - iSeparLen) ;
    else if (i + iSeparLen >= iTextLen)
			_sContenuTransfert = string(_sContenuTransfert, 0, i) ;
    else
    	_sContenuTransfert = string(_sContenuTransfert, 0, i) + string(".") + string(_sContenuTransfert, i + iSeparLen, iTextLen - i - iSeparLen) ;
  }
}

//---------------------------------------------------------------------------//  Function: 		NSEditNum::donneValeur()
//
//  Description:	Calcule la valeur à partir de sContenuTransfert
//
//  Arguments:		Aucun
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSEditNum::donneValeur()
{
	_dValeur = 0 ;
	if (string("") == _sContenuTransfert)
		return ;

	_dValeur = StringToDouble(_sContenuBrut) ;
}

// ----------------------------------------------------------------------
//
//           					classe NSEditNumTel
//
// ----------------------------------------------------------------------

NSEditNumTel::NSEditNumTel(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, int iTextLen)             :NSEditNum(pCtx, pNSUtilDialog, resId, iTextLen, 0, "0-9. -"){
}

NSEditNumTel::~NSEditNumTel(){
}

// ----------------------------------------------------------------------//
//           					classe NSUtilEditDate
//
// ----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUtilEditDate, NSUtilEdit)
   EV_WM_CHAR,
   EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSUtilEditDate::NSUtilEditDate(NSContexte* pCtx, NSUtilDialog* parent, int resId, uint textLen, bool b2000, string sLang)
               :NSUtilEdit(pCtx, parent, resId, textLen)
{
  _sLang = sLang ;

  initialise(b2000) ;
}

NSUtilEditDate::NSUtilEditDate(NSContexte* pCtx, NSUtilDialog* parent, int Id, int x, int y, int w, int h, int iTextLen, bool b2000, string sLang)
               :NSUtilEdit(pCtx, parent, Id, "", x, y, w, h, iTextLen)
{
  _sLang = sLang ;

  initialise(b2000) ;
}

//// Destructeur
//
NSUtilEditDate::~NSUtilEditDate()
{
}

void
NSUtilEditDate::SetupWindow()
{
  NSUtilEdit::SetupWindow() ;

  donneBrut() ;
  SetText(_sContenuBrut.c_str()) ;
}

voidNSUtilEditDate::setDate(string sAAAAMMJJ)
{
  _sContenuTransfert = sAAAAMMJJ ;
  donneBrut() ;
  SetText(_sContenuBrut.c_str()) ;
}

void
NSUtilEditDate::getDate(string* pAAAAMMJJ)
{
	_sContenuBrut = GetText() ;

	donneTransfert() ;

	*pAAAAMMJJ = _sContenuTransfert ;
}

//---------------------------------------------------------------------------
//  Function: 		NSUtilEditDate::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par
//						l'utilisateur
//---------------------------------------------------------------------------
void
NSUtilEditDate::donneBrut()
{
  string sDateFormat = string("") ;
  if (_pNSUtilDialog && _pNSUtilDialog->pContexte)
    sDateFormat = _pNSUtilDialog->pContexte->getSuperviseur()->getText("0localInformation", "dateFormat") ;

	_sContenuBrut = _sMask ;
	_sContenuBrut[2] = dateSeparationMARK ;
	_sContenuBrut[5] = dateSeparationMARK ;
	//
	// On passe de AAAAMMJJ à JJ/MM/AAAA
	//
  size_t iTransfertSize = strlen(_sContenuTransfert.c_str()) ;

	if (string("") != _sContenuTransfert)
	{
  	if (iTransfertSize >= 4)
    	for (int i = 0 ; i < 4 ; i++)
      	_sContenuBrut[i+6] = _sContenuTransfert[i] ;
    if (iTransfertSize >= 6)
    	for (int i = 0 ; i < 2 ; i++)
      	_sContenuBrut[i+3] = _sContenuTransfert[i+4] ;
    if (iTransfertSize >= 8)
    	for (int i = 0 ; i < 2 ; i++)
      	_sContenuBrut[i] = _sContenuTransfert[i+6] ;
	}
}

//---------------------------------------------------------------------------//  Function: 		NSEditDate::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//					   en données NAUTILUS
//---------------------------------------------------------------------------
void
NSUtilEditDate::donneTransfert()
{
	_sContenuTransfert = string("") ;
	//
  // On passe de JJ/MM/AAAA à AAAAMMJJ
  //
	char cValidateur[11] = "" ;

	strcpy(cValidateur, _sMask.c_str()) ;
	cValidateur[2] = dateSeparationMARK ;
	cValidateur[5] = dateSeparationMARK ;
	if (_sContenuBrut != cValidateur)
	{
  	_sContenuTransfert  = string(_sContenuBrut, 6, 4) ;
    _sContenuTransfert += string(_sContenuBrut, 3, 2) ;
    _sContenuTransfert += string(_sContenuBrut, 0, 2) ;
	}
}

////  TStatic
//
void
NSUtilEditDate::EvChar(uint key, uint repeatCount, uint flags)
{
  if (Validator && (VK_BACK   != key) &&
                   (VK_RETURN != key) &&
                   (VK_SPACE  != key) &&
                   (VK_DELETE != key) &&
                   (VK_TAB    != key))
  {
    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;
    TEdit::GetText(oldBuff, oldBuffLen+1) ;

    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    // On saute les '/'
    if (2 == startSel)
      startSel = 3 ;
    if (5 == startSel)
      startSel = 6 ;

    //Si la sélection est multi-caractères, on la ramène à 1 car.
    endSel = startSel ;
    SetSelection(startSel, endSel) ;

    bool wasAppending = ( endSel == (unsigned int) oldBuffLen ) ;

    if ((false == wasAppending) && (startSel == endSel))
    {
      for (int i = startSel; i < oldBuffLen; i++)
        oldBuff[i] = oldBuff[i+1] ;
      SetText(oldBuff) ;
      SetSelection(startSel, endSel) ;
    }

    bool preMsgModify = IsModified() ;           // Save (pre)  MODIFY flag

    TStatic::EvChar(key, repeatCount, flags) ;   // Process the new char...

    bool postMsgModify = IsModified() ;          // Save (post) MODIFY flag

    GetSelection(startSel, endSel) ;
    int buffLen = GetTextLen() ;
    char far* buff = LockBuffer(max((int)TextLimit,max(oldBuffLen,buffLen))+1) ;

    // Run the result of the edit through the validator.  If incorrect,
    // then restore the original text.  Otherwise, range check & position
    // the selection as needed.
    //
    if (!Validator->HasOption(voOnAppend) || wasAppending && (endSel == (unsigned int)buffLen))
    {
      if (!Validator->IsValidInput(buff, false))
      {
        strcpy(buff, oldBuff) ;          // Restore old buffer
        postMsgModify = preMsgModify ;   // Restore old modify state too!
      }
      else
      {
        if (wasAppending)
          startSel = endSel = strlen(buff) ; // may have autoFilled--move to end
      }
      UnlockBuffer(buff, true) ;
      SetSelection(startSel, endSel) ;
    }
    else
    {
      if ((endSel == (unsigned int)buffLen) &&
          (false == Validator->IsValidInput(buff, false)))
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
      if (5 == startSel)
        SetSelection(6, 6) ;
    }
    delete[] oldBuff ;
  }
  else if (VK_BACK == key)
  {
    uint   startSel, endSel ;
    GetSelection(startSel, endSel) ;

    // On saute les '/'
    if (3 == startSel)
      startSel = 2 ;
    if (6 == startSel)
      startSel = 5 ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;
    TEdit::GetText(oldBuff, oldBuffLen+1) ;

    if (startSel > 0)
    {
      startSel-- ;
      oldBuff[startSel] = '0' ;
    }
    endSel = startSel;
    SetText(oldBuff) ;
    SetSelection(startSel, endSel) ;
    delete[] oldBuff ;
  }
  else if ((VK_RETURN == key) || (VK_TAB == key) || (VK_SPACE == key))
    //Process this message to avoid message beeps.
    return ;
  else
    TStatic::EvChar(key, repeatCount, flags) ;
}

void
NSUtilEditDate::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  if (VK_DELETE == key)
  {
    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;
    TEdit::GetText(oldBuff, oldBuffLen+1) ;

    string dateVide = _sMask ;
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

void
NSUtilEditDate::initialise(bool b2000)
{
  // Date pattern
  //
  _sFormat = pContexte->getSuperviseur()->getText("0localInformation", "dateFormat") ;
  if (string("") == _sFormat)
  {
    _sFormat = string("DD/MM/AAAA") ;
    _sFormat[2] = dateSeparationMARK ;
    _sFormat[5] = dateSeparationMARK ;
  }

	// Validator setting
	//
  string sValidator = _sFormat ;
  for (size_t i = 0 ; i < strlen(sValidator.c_str()) ; i++)
    if (('A' == sValidator[i]) || ('D' == sValidator[i]) || ('M' == sValidator[i]))
      sValidator[i] = '#' ;

  SetValidator(new TPXPictureValidator(sValidator.c_str())) ;

  // Mask setting
  //
  _sMask = _sFormat ;
  for (size_t i = 0 ; i < strlen(_sMask.c_str()) ; i++)
    if (('A' == _sMask[i]) || ('D' == _sMask[i]) || ('M' == _sMask[i]))
      _sMask[i] = '0' ;

  size_t iPos = _sFormat.find("AAAA") ;
  if (NPOS != iPos)
  {
    if (b2000)
      _sMask.replace(iPos, 4, "2000") ;
    else
      _sMask.replace(iPos, 4, "1900") ;
  }

  _sContenuTransfert = string("") ;
}

// ----------------------------------------------------------------------
//
//           					classe NSUtilEditHeure
//
// ----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUtilEditHeure, NSUtilEdit)
   EV_WM_CHAR,
   EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

//// Constructeur
//
NSUtilEditHeure::NSUtilEditHeure(NSContexte* pCtx, NSUtilDialog* parent, int resId, uint textLen, string sLang)
                :NSUtilEdit(pCtx, parent, resId, textLen)
{
  _sLang = sLang ;
  initialise() ;
}

NSUtilEditHeure::NSUtilEditHeure(NSContexte* pCtx, NSUtilDialog* parent, int Id, int x, int y, int w, int h, int iTextLen, string sLang)
                :NSUtilEdit(pCtx, parent, Id, "", x, y, w, h, iTextLen)
{
  _sLang = sLang ;
  initialise() ;
}

//// Destructeur
//
NSUtilEditHeure::~NSUtilEditHeure()
{
}

void
NSUtilEditHeure::SetupWindow()
{
  NSUtilEdit::SetupWindow() ;
	donneBrut() ;
  SetText(_sContenuBrut.c_str()) ;
}

void
NSUtilEditHeure::setHeure(string sHHMM)
{
  _sContenuTransfert = sHHMM ;
  donneBrut() ;
  SetText(_sContenuBrut.c_str()) ;
}

void
NSUtilEditHeure::getHeure(string* pHHMM)
{
  if ((string*) NULL == pHHMM)
    return ;

  _sContenuBrut = GetText() ;

	donneTransfert() ;

  *pHHMM = _sContenuTransfert ;
}

//---------------------------------------------------------------------------
//  Function: 		NSUtilEditHeure::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par
//						l'utilisateur
//---------------------------------------------------------------------------
void
NSUtilEditHeure::donneBrut()
{
  _sContenuBrut = string("00:00") ;
  _sContenuBrut[2] = heureSeparationMARK ;

  //
  // On passe de HHmm à HH:mm
  //
  if (string("") != _sContenuTransfert)
    _sContenuBrut = string(_sContenuTransfert, 0, 2) + string(1,heureSeparationMARK) +
      					string(_sContenuTransfert, 2, 2) ;
}

//---------------------------------------------------------------------------//  Function: 		NSEditDate::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//					   en données NAUTILUS
//---------------------------------------------------------------------------
void
NSUtilEditHeure::donneTransfert()
{
	_sContenuTransfert = string("") ;
  //
  // On passe de HH:mm à HHmm
  //
  string sValidateur = string("00:00") ;
  sValidateur[2] = heureSeparationMARK ;

  if (_sContenuBrut != sValidateur)
    _sContenuTransfert = string(_sContenuBrut, 0, 2) + string(_sContenuBrut, 3, 2) ;
}

////  TStatic
//
void
NSUtilEditHeure::EvChar(uint key, uint repeatCount, uint flags)
{
  if (Validator && (key != VK_BACK) 	&&
                   (key != VK_RETURN) &&
                   (key != VK_SPACE) 	&&
                   (key != VK_DELETE) &&
                   (key != VK_TAB))
  {
    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen + 1] ;
    TEdit::GetText(oldBuff, oldBuffLen + 1) ;

    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    // On saute le ':'
    if (2 == startSel)
      startSel = 3 ;

    //Si la sélection est multi-caractères, on la ramène à 1 car.
    endSel = startSel ;
    SetSelection(startSel, endSel) ;

    bool wasAppending = ( endSel == (unsigned int) oldBuffLen ) ;

    if ((false == wasAppending) && (startSel == endSel))
    {
      for (int i = startSel ; i < oldBuffLen ; i++)
        oldBuff[i] = oldBuff[i + 1] ;
      SetText(oldBuff) ;
      SetSelection(startSel, endSel) ;
    }

    bool preMsgModify = IsModified() ;             // Save (pre)  MODIFY flag

    TStatic::EvChar(key, repeatCount, flags) ;     // Process the new char...

    bool postMsgModify= IsModified() ;             // Save (post) MODIFY flag

    GetSelection(startSel, endSel) ;
    int buffLen = GetTextLen() ;
    char far* buff = LockBuffer(max((int)TextLimit,max(oldBuffLen,buffLen))+1) ;

    // Run the result of the edit through the validator.  If incorrect,
    // then restore the original text.  Otherwise, range check & position
    // the selection as needed.
    //
    if (!Validator->HasOption(voOnAppend) || wasAppending && int(endSel) == buffLen)
    {
      if (!Validator->IsValidInput(buff, false))
      {
        strcpy(buff, oldBuff) ;          // Restore old buffer
        postMsgModify = preMsgModify ;   // Restore old modify state too!
      }
      else
      {
        if (wasAppending)
          startSel = endSel = strlen(buff) ; // may have autoFilled--move to end
      }
      UnlockBuffer(buff, true) ;
      SetSelection(startSel, endSel) ;
    }
    else
    {
      if ((endSel == (unsigned int) buffLen) &&
                (false == Validator->IsValidInput(buff, false)))
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

    delete[] oldBuff ;
  }
  else if (VK_BACK == key)
  {
    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    // On saute le ':'
    if (3 == startSel)
      startSel = 2 ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen + 1] ;
    TEdit::GetText(oldBuff, oldBuffLen+1) ;

    if (startSel > 0)
    {
      startSel-- ;
      oldBuff[startSel] = '0' ;
    }
    endSel = startSel ;
    SetText(oldBuff) ;
    SetSelection(startSel, endSel) ;
    delete[] oldBuff ;
  }
  else if ((VK_RETURN == key) || (VK_TAB == key))
    //Process this message to avoid message beeps.
    return ;
  else if (VK_SPACE == key)
    //Process this message to avoid message beeps.
    return ;
  else
    TStatic::EvChar(key, repeatCount, flags) ;
}

void
NSUtilEditHeure::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  if (VK_DELETE == key)
  {
    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen + 1] ;
    TEdit::GetText(oldBuff, oldBuffLen+1) ;

    string heureVide = string("00:00") ;
    string heureEdit = string(oldBuff) ;

    if (startSel == endSel)
    {
      heureEdit.replace(startSel, 1, heureVide, startSel, 1) ;
      startSel++ ;
      endSel++ ;
    }
    else
    {
      heureEdit.replace(startSel, endSel-startSel, heureVide, startSel, endSel-startSel) ;
      startSel = endSel ;
    }

    strcpy(oldBuff, heureEdit.c_str()) ;
    SetText(oldBuff) ;
    delete[] oldBuff ;

    SetSelection(startSel, endSel) ;
  }
  else
    TStatic::EvKeyDown(key, repeatCount, flags) ;
}

void
NSUtilEditHeure::initialise()
{
  //
	// Mise en place du validateur
	//
  char cValidateur[] = "##:##" ;
  cValidateur[2] = heureSeparationMARK ;
  SetValidator(new TPXPictureValidator(cValidateur)) ;

  _sContenuTransfert = string("") ;
}

// ----------------------------------------------------------------------
//
//           					classe NSUtilEditDateHeure
//
// ----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUtilEditDateHeure, NSUtilEdit)
	EV_WM_CHAR,
	EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSUtilEditDateHeure::NSUtilEditDateHeure(NSContexte* pCtx, NSUtilDialog* parent, int resId, uint textLen, bool b2000, string sLang)
                    :NSUtilEdit(pCtx, parent, resId, textLen)
{
  _sLang = sLang ;

	initCommonData(b2000) ;
}

NSUtilEditDateHeure::NSUtilEditDateHeure(NSContexte* pCtx, NSUtilDialog* parent, int Id, const char far* text, int x, int y, int w, int h, int iTextLen, bool b2000, string sLang)
                    :NSUtilEdit(pCtx, parent, Id, text, x, y, w, h, iTextLen)
{
  _sLang = sLang ;

	initCommonData(b2000) ;
}

//// Destructeur
//
NSUtilEditDateHeure::~NSUtilEditDateHeure()
{
}

void
NSUtilEditDateHeure::initCommonData(bool b2000)
{
  // Date pattern
  //
  _sFormat = pContexte->getSuperviseur()->getText("0localInformation", "timeFormat") ;

  if (string("") == _sFormat)
  {
    _sFormat = string("DD/MM/AAAA hh:mm:ss") ;
    _sFormat[2]  = dateSeparationMARK ;
    _sFormat[5]  = dateSeparationMARK ;
    _sFormat[13] = heureSeparationMARK ;
    _sFormat[16] = heureSeparationMARK ;
  }

	// Validator setting
	//
  string sValidator = _sFormat ;
  for (size_t i = 0 ; i < strlen(sValidator.c_str()) ; i++)
    if (('A' == sValidator[i]) || ('D' == sValidator[i]) || ('M' == sValidator[i]) ||
        ('h' == sValidator[i]) || ('m' == sValidator[i]) || ('s' == sValidator[i]))
      sValidator[i] = '#' ;

  SetValidator(new TPXPictureValidator(sValidator.c_str())) ;

  // Mask setting
  //
  _sMask = _sFormat ;
  for (size_t i = 0 ; i < strlen(_sMask.c_str()) ; i++)
    if (('A' == _sMask[i]) || ('D' == _sMask[i]) || ('M' == _sMask[i]) ||
        ('h' == _sMask[i]) || ('m' == _sMask[i]) || ('s' == _sMask[i]))
      _sMask[i] = '0' ;

  _sContenuTransfert = string("") ;

  size_t iPos = _sFormat.find("AAAA") ;
  if (NPOS != iPos)
  {
    if (b2000)
      _sMask.replace(iPos, 4, "2000") ;
    else
      _sMask.replace(iPos, 4, "1900") ;
  }
}

void
NSUtilEditDateHeure::SetupWindow()
{
	NSUtilEdit::SetupWindow() ;
	donneBrut() ;
	SetText(_sContenuBrut.c_str()) ;
}

voidNSUtilEditDateHeure::setDate(string sAAAAMMJJ)
{
	_sContenuTransfert = sAAAAMMJJ ;
	donneBrut() ;
	SetText(_sContenuBrut.c_str()) ;
}

void
NSUtilEditDateHeure::getDate(string* pAAAAMMJJ)
{
  if ((string*) NULL == pAAAAMMJJ)
    return ;

	_sContenuBrut = GetText() ;
	donneTransfert() ;

	*pAAAAMMJJ = _sContenuTransfert ;
}

//---------------------------------------------------------------------------
//  Function: 		NSUtilEditDate::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par
//						l'utilisateur
//---------------------------------------------------------------------------
void
NSUtilEditDateHeure::donneBrut()
{
	_sContenuBrut = _sMask ;
  //
  // On passe de AAAAMMDDhhmmss à DD/MM/AAAA hh:mm:ss
  //             01234567890123   0123456789012345678
  //
  if ((string("") != _sContenuTransfert) && (strlen(_sContenuTransfert.c_str()) >= 8))
    _sContenuBrut = getFormatedTime(_sContenuTransfert, _sLang, _sFormat) ;
}

//---------------------------------------------------------------------------//  Function: 		NSEditDate::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//                en données NAUTILUS
//---------------------------------------------------------------------------
void
NSUtilEditDateHeure::donneTransfert()
{
  _sContenuTransfert = string("") ;

  if ((_sMask == _sContenuBrut) || (strlen(_sContenuBrut.c_str()) != strlen(_sFormat.c_str())))
    return ;

  _sContenuTransfert = getRawTime(_sContenuBrut, _sLang, _sFormat) ;
}

bool
NSUtilEditDateHeure::isFormatChar(char c)
{
  switch(c)
  {
    case 'A' :
    case 'M' :
    case 'D' :
    case 'h' :
    case 'm' :
    case 's' : return true ;
  }
  return false ;
}

////
void
NSUtilEditDateHeure::EvChar(uint key, uint repeatCount, uint flags)
{
  if (Validator && (VK_BACK   != key) &&
                   (VK_RETURN != key) &&
                   (VK_SPACE  != key) &&
                   (VK_DELETE != key) &&
                   (VK_TAB    != key))
  {
    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;
    TEdit::GetText(oldBuff, oldBuffLen + 1) ;

    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    // On saute les '/'
    if ((startSel > 0) && (startSel < strlen(oldBuff)) && (strlen(oldBuff) == strlen(_sFormat.c_str())))
    {
      if (false == isFormatChar(_sFormat[startSel]))
      {
        startSel++ ;
        while ((startSel < strlen(oldBuff)) && (false == isFormatChar(_sFormat[startSel])))
          startSel++ ;
      }
    }

    //Si la sélection est multi-caractères, on la ramène à 1 car.
    endSel = startSel ;
    SetSelection(startSel, endSel) ;

    bool wasAppending = ( endSel == (unsigned int)oldBuffLen ) ;

    if ((false == wasAppending) && (startSel == endSel))
    {
      char far* newBuff = new char[oldBuffLen+1] ;
      strcpy(newBuff, oldBuff) ;
      for (int i = startSel; i < oldBuffLen; i++)
        newBuff[i] = newBuff[i+1] ;
      SetText(newBuff) ;
      SetSelection(startSel, endSel) ;
      delete[] newBuff ;
    }

    bool preMsgModify = IsModified() ;             // Save (pre)  MODIFY flag

    TStatic::EvChar(key, repeatCount, flags) ;    // Process the new char...

    bool postMsgModify= IsModified() ;            // Save (post) MODIFY flag

    GetSelection(startSel, endSel) ;
    int buffLen = GetTextLen() ;
    char far* buff = LockBuffer(max((int)TextLimit,max(oldBuffLen,buffLen))+1) ;

    // Run the result of the edit through the validator.  If incorrect,
    // then restore the original text.  Otherwise, range check & position
    // the selection as needed.
    //
/*
    if (!Validator->HasOption(voOnAppend) || wasAppending && endSel == buffLen)
    {
*/
      if (!Validator->IsValidInput(buff, false))
      {
        strcpy(buff, oldBuff) ;          // Restore old buffer
        postMsgModify = preMsgModify ;   // Restore old modify state too!
        if (startSel > 0)
          startSel-- ;
        endSel = startSel ;
      }
      else
      {
        if (wasAppending)
          startSel = endSel = strlen(buff) ; // may have autoFilled--move to end

        bool   bChecked = true ;
        string sChecker = string(buff) ;
        size_t iPos = _sFormat.find("MM") ;
        if (NPOS != iPos)
        {
          if ((startSel-1 == iPos) && (buff[iPos] > '1'))
            bChecked = false ;
          if ((startSel-1 == iPos+1) && (buff[iPos] == '1') && (buff[iPos+1] > '2'))
            bChecked = false ;
        }
        iPos = _sFormat.find("DD") ;
        if (NPOS != iPos)
        {
          if ((startSel-1 == iPos) && (buff[iPos] > '3'))
            bChecked = false ;
          if ((startSel-1 == iPos+1) && (buff[iPos] == '3') && (buff[iPos+1] > '1'))
            bChecked = false ;
        }
        iPos = _sFormat.find("hh") ;
        if (NPOS != iPos)
        {
          if ((startSel-1 == iPos) && (buff[iPos] > '2'))
            bChecked = false ;
          if ((startSel-1 == iPos+1) && (buff[iPos] == '2') && (buff[iPos+1] > '3'))
            bChecked = false ;
        }
        iPos = _sFormat.find("mm") ;
        if (NPOS != iPos)
        {
          if ((startSel-1 == iPos) && (buff[iPos] > '5'))
            bChecked = false ;
          if ((startSel-1 == iPos+1) && (buff[iPos] == '5') && (buff[iPos+1] > '9'))
            bChecked = false ;
        }
        iPos = _sFormat.find("ss") ;
        if (NPOS != iPos)
        {
          if ((startSel-1 == iPos) && (buff[iPos] > '5'))
            bChecked = false ;
          if ((startSel-1 == iPos+1) && (buff[iPos] == '5') && (buff[iPos+1] > '9'))
            bChecked = false ;
        }
        if (false == bChecked)
        {
          strcpy(buff, oldBuff) ;          // Restore old buffer
          postMsgModify = preMsgModify ;   // Restore old modify state too!

          if (startSel > 0)
            startSel-- ;
          endSel = startSel ;
        }
      }
      UnlockBuffer(buff, true) ;
      SetSelection(startSel, endSel) ;
/*
    }
    else
    {
      if (endSel == buffLen && !Validator->IsValidInput(buff, true))
        Validator->Error(this) ;
      UnlockBuffer(buff) ;
    }
*/
    SendMessage(EM_SETMODIFY, (TParam1)postMsgModify) ;

    //
    // On saute les caractères inutiles
    //
    GetSelection(startSel, endSel) ;
    if ((startSel == endSel) && (startSel < strlen(oldBuff)) && (strlen(oldBuff) == strlen(_sFormat.c_str())-1))
    {
      if (false == isFormatChar(_sFormat[startSel]))
      {
        startSel++ ;
        while ((startSel < strlen(oldBuff)) && (false == isFormatChar(_sFormat[startSel])))
          startSel++ ;
      }
      SetSelection(startSel, startSel) ;
    }
    delete[] oldBuff ;
  }
  else if (VK_BACK == key)
  {
    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen + 1] ;
    TEdit::GetText(oldBuff, oldBuffLen + 1) ;

    // On saute les '/'
    if ((startSel > 0) && (startSel < strlen(oldBuff)) && (strlen(oldBuff) == strlen(_sFormat.c_str())))
    {
      if (false == isFormatChar(_sFormat[startSel-1]))
      {
        startSel-- ;
        while ((startSel > 0) && (false == isFormatChar(_sFormat[startSel-1])))
          startSel-- ;
      }
    }

    string dateVide = _sMask ;
    if (startSel > 0)
    {
      startSel-- ;
      oldBuff[startSel] = dateVide[startSel] ;
    }
    endSel = startSel ;
    SetText(oldBuff) ;
    delete[] oldBuff ;

    SetSelection(startSel, endSel) ;
  }
  else if ((VK_RETURN == key) || (VK_TAB == key) || (VK_SPACE == key))
    //Process this message to avoid message beeps.
    return ;
  else
    TStatic::EvChar(key, repeatCount, flags) ;
}

void
NSUtilEditDateHeure::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  if (VK_DELETE == key)
  {
    uint startSel, endSel ;
    GetSelection(startSel, endSel) ;

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;
    TEdit::GetText(oldBuff, oldBuffLen+1) ;

    string dateVide = _sMask ;
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

// ----------------------------------------------------------------------
//
//           					classe NSUtilEditSomme
//
// ----------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSUtilEditSomme, NSUtilEdit)   EV_WM_CHAR,
   EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

//// Constructeur
//
NSUtilEditSomme::NSUtilEditSomme(NSContexte* pCtx, NSUtilDialog* parent, int resId, uint iTextLen, string sLang)
                :NSUtilEdit(pCtx, parent, resId, iTextLen)
{
  _sLang = sLang ;

	_textLen = iTextLen ;

	if (_textLen < 4)
		erreur("La longueur du champ EditSomme est trop courte.", standardError, 0) ;

    /*********************************************
	// Mise en place du validateur
	//
    char* cValidateur = new char[textLen + 1];

    for (int i = 0; i < textLen; i++)
   	    cValidateur[i] = '#';

    cValidateur[textLen + 1] = '\0';

    cValidateur[textLen - 2] = sommeSeparationMARK;
    SetValidator(new TPXPictureValidator(cValidateur));
	delete[] cValidateur;
    ***************************************************/

  _sZero = string("") ;

  for (int i = 0; i < 4; i++)
    _sZero += ' ';

  _sZero[0] = '0' ;
  _sZero[1] = sommeSeparationMARK ;
  _sZero[2] = '0' ;
  _sZero[3] = '0' ;

  _sContenuTransfert = string("") ;
}

//// Destructeur
//
NSUtilEditSomme::~NSUtilEditSomme()
{
}

void
NSUtilEditSomme::SetupWindow()
{
  NSUtilEdit::SetupWindow() ;
  donneBrut() ;
  SetText(_sContenuBrut.c_str()) ;
}

voidNSUtilEditSomme::setSomme(string sSomme)
{
  _sContenuTransfert = sSomme ;
  donneBrut() ;
  SetText(_sContenuBrut.c_str()) ;
}

void
NSUtilEditSomme::getSomme(string* pSomme)
{
  if ((string*) NULL == pSomme)
    return ;

  _sContenuBrut = GetText() ;
	donneTransfert() ;

  *pSomme = _sContenuTransfert ;
}

//---------------------------------------------------------------------------
//  Function: 		NSUtilEditSomme::donneBrut()
//
//  Description:	Transforme les données NAUTILUS en données à saisir par
//						l'utilisateur
//---------------------------------------------------------------------------
void
NSUtilEditSomme::donneBrut()
{
  int   transfert ;
  char  nbDec[3] ;
  char* nbEnt = new char[_textLen - 2] ;

  _sContenuBrut = _sZero ;

  //
  // On passe d'un montant en centimes à un montant en francs
  //
  if (string("") != _sContenuTransfert)
  {
    transfert = atoi(_sContenuTransfert.c_str()) ;
    sprintf(nbEnt, "%d", transfert/100) ;
    sprintf(nbDec, "%02d", transfert%100) ;

    _sContenuBrut = string(nbEnt) + string(1, sommeSeparationMARK) + string(nbDec) ;
  }

  delete[] nbEnt ;
}

//---------------------------------------------------------------------------//  Function: 		NSEditDate::donneTransfert()
//
//  Description:	Transforme les données saisies par l'utilisateur
//					   en données NAUTILUS
//---------------------------------------------------------------------------
void
NSUtilEditSomme::donneTransfert()
{
	size_t i ;

	_sContenuTransfert = string("") ;

	//
	// On passe d'un montant en franc à un montant en centimes
	//
	if (_sContenuBrut != _sZero)
	{
  	for (i = 0; (i < strlen(_sContenuBrut.c_str())) && (',' != _sContenuBrut[i]); i++)
    	_sContenuTransfert += _sContenuBrut[i] ;

		if ((i < strlen(_sContenuBrut.c_str())) && (',' == _sContenuBrut[i]))
    	i++ ;

    int j = 0 ;
    for ( ; i < strlen(_sContenuBrut.c_str()) ; i++, j++)
    	_sContenuTransfert += _sContenuBrut[i] ;
    for ( ; j < 2 ; j++)
    	_sContenuTransfert += string("0") ;
	}
}

//
//  TStatic
//
void
NSUtilEditSomme::EvChar(uint key, uint repeatCount, uint flags)
{
  int 	  oldBuffLen = GetTextLen();
  char far* oldBuff = new char[_textLen + 1];
  uint   	  startSel, endSel;
  int		  posvir;
  int		  nbMaxEntier = _textLen - 3;

  TEdit::GetText(oldBuff, oldBuffLen + 1);
  GetSelection(startSel, endSel);

  string sEntree = oldBuff;

    // Si la sélection est multi-caractères, on la ramène à 1 car.
    //endSel = startSel;
    //SetSelection(startSel, endSel);

    // Recherche de la virgule
    for (	posvir = 0;
   		(posvir < oldBuffLen) && (oldBuff[posvir] != sommeSeparationMARK);
         posvir++ );

    if (posvir == oldBuffLen)
        posvir = -1;

    // Le caractère tapé est un chiffre
    //
    if ((key >= '0') && (key <= '9'))
    {
        bool bEcraser = false;
        bool bBeep = false;

        // s'il y a une virgule...
        if (posvir >= 0)
        {
      	    // si on est avant
      	    if (startSel <= (unsigned int)posvir)
            {
         	    if (posvir == nbMaxEntier)
                {
            	    if (startSel == (unsigned int)posvir)
               	        startSel += 1;

                    bEcraser = true;
                }
            }
            else // après la ','
            {
         	    // si le nombre max après la ',' est atteint
         	    if ((oldBuffLen - posvir - 1) == 2)
                {
                    if (startSel < (unsigned int)posvir + 3)
            		    bEcraser = true;
                    else
               	        bBeep = true;
                }            }
        }
        else // nombre sans ','
        {
      	    if (oldBuffLen == nbMaxEntier)
            {
                oldBuff[oldBuffLen] = sommeSeparationMARK;
                oldBuff[oldBuffLen + 1] = '0';
                oldBuff[oldBuffLen + 2] = '0';
                oldBuff[oldBuffLen + 3] = '\0';

                startSel = oldBuffLen + 1;
                oldBuffLen += 3;
                bEcraser = true;
            }
        }

  		if (bEcraser)
    	{
    		for (int i = startSel; i < oldBuffLen; i++)
         	    oldBuff[i] = oldBuff[i+1];
            SetText(oldBuff);
            SetSelection(startSel, endSel);
        }

        if (!bBeep)
        {
    		TStatic::EvChar(key, repeatCount, flags);     // Process the new char...
        }
        else
        {
      	    ::MessageBeep(0);     // BEEP
        }
    }
    else if ((key == ',') || (key == '.'))
    {
        if (posvir == -1)
        {
            int nbDecim = oldBuffLen - endSel;
            if (nbDecim > 2)
                ::MessageBeep(0);     // BEEP
            else if (nbDecim == 0)
            {
                string sResult;
                if (startSel == 0)
                    sResult = "0" + string(1, sommeSeparationMARK) + "00";
                else
                    sResult = string(sEntree, 0, startSel) +
                              string(1, sommeSeparationMARK) + "00";

                strcpy(oldBuff, sResult.c_str());
                SetText(oldBuff);
                startSel++;
                SetSelection(startSel, startSel);
            }
            else
            {
                string sResult;
                if (startSel == 0)
                    sResult = "0" +
                              string(1, sommeSeparationMARK) +
                              string(sEntree, endSel, nbDecim);
                else
                    sResult = string(sEntree, 0, startSel) +
                              string(1, sommeSeparationMARK) +
                              string(sEntree, endSel, nbDecim);

                strcpy(oldBuff, sResult.c_str());
                SetText(oldBuff);
                startSel++;
                SetSelection(startSel, startSel);
            }
        }
        else
        {
      	    ::MessageBeep(0);     // BEEP
        }
    }
    else if (key == VK_BACK)
    {
        // Si des caractères sont sélectionnés, Back fonctionne comme Suppr
        //
        if (endSel > startSel)
        {
            string sResult = "";
            if (startSel > 0)
                sResult = string(sEntree, 0, startSel);
            int nbDecim = oldBuffLen - endSel;
            if (nbDecim > 0)
                sResult += string(sEntree, endSel, nbDecim);
            strcpy(oldBuff, sResult.c_str());
            SetText(oldBuff);
            SetSelection(startSel, startSel);
        }
   	    else if (startSel > 0)
        {
      	    // On saute la ',' si le nombre de chiffres peut dépasser nbMaxEntier
      	    if ((startSel == (unsigned int)posvir + 1) && (oldBuffLen > nbMaxEntier + 1))
      		    startSel = posvir;

            if (startSel > 0)
         	    startSel -= 1;

   		    for (int i = startSel; i < oldBuffLen; i++)
         	    oldBuff[i] = oldBuff[i+1];
      	    SetText(oldBuff);
      	    SetSelection(startSel, startSel);
        }
        else
        {
      	    ::MessageBeep(0);     // BEEP
        }
    }
    else if ((key == VK_RETURN) || (key == VK_TAB))
    {
   	//Process this message to avoid message beeps.
        delete[] oldBuff;
        return ;
    }
    else if ((key == VK_SPACE))
    {
   	//Process this message to avoid message beeps.
        delete[] oldBuff;
        return ;
    }
    else
    {
   	    ::MessageBeep(0);     // BEEP
    }

    delete[] oldBuff;
}

voidNSUtilEditSomme::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	int 	  oldBuffLen = GetTextLen();
  char far* oldBuff = new char[_textLen + 1];
  uint   	  startSel, endSel;
  int		  posvir;
  int		  nbMaxEntier = _textLen - 3;

  TEdit::GetText(oldBuff, oldBuffLen + 1);
  GetSelection(startSel, endSel);

    //Si la sélection est multi-caractères, on la ramène à 1 car.
    //endSel = startSel;
    //SetSelection(startSel, endSel);

    for (posvir = 0;
   		(posvir < oldBuffLen) && (oldBuff[posvir] != sommeSeparationMARK);
         posvir++ );

    if (posvir == oldBuffLen)
        posvir = -1;

    if (key == VK_DELETE)
    {
        if (startSel < (unsigned int)oldBuffLen)
        {
            if ((startSel == (unsigned int)posvir) && (oldBuffLen > nbMaxEntier + 1))
         	    startSel += 1;

            int nbCar = endSel - startSel;
            for (int j = 0; j < nbCar; j++)
            {
      	        for (int i = startSel; i < oldBuffLen; i++)
         	        oldBuff[i] = oldBuff[i+1];
                startSel++;
            }
      	    SetText(oldBuff);
      	    SetSelection(startSel, startSel);
        }
        else
        {
      	    ::MessageBeep(MB_ICONEXCLAMATION);     // BEEP
        }
    }
    else
        TStatic::EvKeyDown(key, repeatCount, flags);

    delete[] oldBuff;
}

bool
NSUtilEditSomme::estValide(string sTest)
{
	string sEntier = "0" ;
	string sDecima = "" ;
	// Recherche de la virgule
	size_t posvir = sTest.find(string(1, sommeSeparationMARK)) ;
	if (NPOS != posvir)	{
		int iTaille = strlen(sTest.c_str()) ;

    if (posvir > 0)
    	sEntier = string(sTest, 0, posvir);
    if (posvir < (unsigned int)iTaille - 1)
    {
    	sDecima = string(sTest, posvir + 1, iTaille - posvir) ;
      // Y-a-t'il deux vigules ?
      size_t biposvir = sDecima.find(string(1, sommeSeparationMARK)) ;
      if (NPOS != biposvir)
      	return false ;
    }
	}
  return true ;
}

//**********************************************************************// classe NSUtilLexique : accés générique au lexique
//**********************************************************************

DEFINE_RESPONSE_TABLE1(NSUtilLexique, NSEditDicoGlobal)
	EV_WM_CHAR,
	EV_WM_KEYDOWN,
  EV_WM_GETDLGCODE,
  EV_WM_SETFOCUS,
	EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

//
// Constructeurs
//
NSUtilLexique::NSUtilLexique(NSContexte* pCtx, TWindow* parent, int resId, NSDico* pDictio,
                     uint textLen, TModule* module)
              :NSEditDicoGlobal(pCtx, parent, resId, pDictio, "", textLen, module)
{
  _pNSUtilDialog     = (NSUtilDialog*) 0 ;
  _sContenuTransfert = string("") ;
  _sCode             = string("") ;
  _LostFocusFunctor  = (Functor*) 0 ;
}

NSUtilLexique::NSUtilLexique(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int resId, NSDico* pDictio,
                     uint textLen, TModule* module)
              :NSEditDicoGlobal(pCtx, pUtilDialog, resId, pDictio, "", textLen, module)
{
  _pNSUtilDialog = pUtilDialog ;
  _pNSUtilDialog->ReferenceControl(this) ;

  _sContenuTransfert = string("") ;
  _sCode             = string("") ;
  _LostFocusFunctor  = (Functor*) 0 ;
}

NSUtilLexique::NSUtilLexique(NSContexte* pCtx, TWindow* parent, int resourceId, NSDico* pDictio,
                       const char far* text,
                       int x, int y, int w, int h,
                       uint textLimit, bool multiline, TModule* module)
              :NSEditDicoGlobal(pCtx, parent, resourceId, pDictio, text, x, y, w, h,
                       textLimit, multiline, "", module)
{
  _pNSUtilDialog     = (NSUtilDialog*) 0 ;
  _sContenuTransfert = string("") ;
  _sCode             = string("") ;
  _LostFocusFunctor  = (Functor*) 0 ;
}

NSUtilLexique::NSUtilLexique(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int resourceId, NSDico* pDictio,
                       const char far* text,
                       int x, int y, int w, int h,
                       uint textLimit, bool multiline, TModule* module)
              :NSEditDicoGlobal(pCtx, pUtilDialog,resourceId, pDictio, text, x, y, w, h,
                       textLimit, multiline, "", module)
{
  _pNSUtilDialog = pUtilDialog ;
  _pNSUtilDialog->ReferenceControl(this) ;

  _sContenuTransfert = string("") ;
  _sCode             = string("") ;
  _LostFocusFunctor  = (Functor*) 0 ;
}

NSUtilLexique::~NSUtilLexique()
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSUtilLexique::SetupWindow()
{
  NSEditDicoGlobal::SetupWindow() ;
}

string
NSUtilLexique::getCodeSens()
{
  if ((NSDico*) NULL == _pDico)
    return string("") ;

  string sCodeSens = string("") ;
  _pDico->donneCodeSens(&_sCode, &sCodeSens) ;

  return sCodeSens ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSUtilLexique::UpdateDico()
{
  //
  // Récupèration du contenu du contrôle
  //
  int oldBuffLen = GetTextLen() ;
	char far* oldBuff = new char[oldBuffLen+1] ;
  GetText(oldBuff, oldBuffLen+1) ;
  string sContenu = string(oldBuff) ;
  delete[] oldBuff ;

  strip(sContenu) ;

  ChoixPathoDialog* pDicoDialog = _pDico->getDicoDialog() ;
  if ((ChoixPathoDialog*) NULL == pDicoDialog)
    return ;

  //
  // Transmission du contenu à la boite de dialogue
  //
	int OouF = pDicoDialog->DonneAmmorce(&sContenu) ;
  if (1 == OouF)
  {
    _bGardeFocus = true ;
    //position du lexique
    NS_CLASSLIB::TRect rectPere = Parent->GetWindowRect() ;
    NS_CLASSLIB::TRect rectEdition = GetWindowRect() ;

    int x = Attr.X + Attr.W + rectPere.left  + 50 ;
    int y = rectEdition.top ;

    NS_CLASSLIB::TRect rectLex = pDicoDialog->GetWindowRect() ;
    pDicoDialog->SetWindowPos(0, x, y, rectLex.Width(), rectLex.Height(), SWP_NOZORDER) ;
    pDicoDialog->montrerDialogue() ;
    SetFocus() ;
  }
  else if (2 == OouF)
    pDicoDialog->cacherDialogue() ;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
NSUtilLexique::EvChar(uint key, uint repeatCount, uint flags)
{
  TEdit::EvChar(key, repeatCount, flags);

  //
  // Transmission du contenu à la boite de dialogue
  //
  if (VK_RETURN == key)
    return ;

  ChoixPathoDialog* pDicoDialog = _pDico->getDicoDialog() ;
  if ((ChoixPathoDialog*) NULL == pDicoDialog)
    return ;

  // flèche basse
  if (VK_DOWN == key)
  {
    if (pDicoDialog->getAmmorce() == string(""))
      return ;

    // sinon récupérer un élément dans le lexique
    pDicoDialog->ElementSuivant() ;
  }

  // flèche haut
  else if (VK_UP == key)
  {
    //si ammorce vide alors passer au NStrreNode suivant
    if (pDicoDialog->getAmmorce() == string(""))
      return ;

    //sinon récupérer un élément dans le lexique
    pDicoDialog->ElementPrecedent() ;
  }
  else if (VK_INSERT == key)
    pDicoDialog->InsererElementLexique() ;
  else
    UpdateDico() ;
}

//---------------------------------------------------------------------------//---------------------------------------------------------------------------
void
NSUtilLexique::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  TEdit::EvKeyDown(key, repeatCount, flags) ;
  Parent->SendNotification(Attr.Id, 200, HWindow) ;

  ChoixPathoDialog* pDicoDialog = _pDico->getDicoDialog() ;
  if ((ChoixPathoDialog*) NULL == pDicoDialog)
    return ;

  //
  // Transmission du contenu à la boite de dialogue
  //
  //flèche basse
  if (VK_DOWN == key)
  {
    if (pDicoDialog->getAmmorce() == string(""))
      return ;

    pDicoDialog->ElementSuivant() ;
  }

  //flèche haut
  else if (VK_UP == key)
  {
    //si ammorce vide alors passer au NStrreNode suivant
    if (pDicoDialog->getAmmorce() == string(""))
      return ;

    pDicoDialog->ElementPrecedent() ;
  }
  else if ((VK_INSERT == key) || (VK_RETURN == key))
    pDicoDialog->InsererElementLexique() ;
  else if (VK_F1 == key)
  {
    NSUtilDialog* pUtilDlg = TYPESAFE_DOWNCAST(Parent, NSUtilDialog) ;
    if (pUtilDlg)
      pUtilDlg->EvKeyDown(key, repeatCount, flags) ;
  }
  else
    UpdateDico() ;
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TABvoid
NSUtilLexique::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  // THandle NextControl = 0;
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

    default		:
      TEdit::EvKeyUp(key, repeatcount, flags) ;
  }
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSUtilLexique::EvGetDlgCode(MSG far* msg)
{
	uint retVal = (uint)DefaultProcessing() ;

	if (msg && (msg->wParam == VK_RETURN))
	{
		if (_pDico)
    {
      ChoixPathoDialog* pDicoDialog = _pDico->getDicoDialog() ;
      if (pDicoDialog && pDicoDialog->IsWindow())
			  pDicoDialog->InsererElementLexique() ;
    }
		retVal |= DLGC_WANTALLKEYS ;
	}
	return retVal ;
}

//-------------------------------------------------------------------------// 					l'utilisateur a choisi un élément dans le lexique
//-------------------------------------------------------------------------
void
NSUtilLexique::ElementSelectionne()
{
	char code[BASE_LEXI_LEN+1] ;
	GetCodeLexiqueChoisi(code) ;
	GetLabelChoisi(&_sContenuTransfert) ;
	SetText(_sContenuTransfert.c_str()) ;
	_sCode = string(code) ;
  ChoixPathoDialog* pDicoDialog = _pDico->getDicoDialog() ;
  if (pDicoDialog)
	  pDicoDialog->cacherDialogue() ;
}

void
NSUtilLexique::setLabel(string code, string texteFictif)
{
  _sCode = code ;

  if ((string("") != _sCode) && (string("£?????") != _sCode))
  {
    string sLang = _pDico->pContexte->getUserLanguage() ;
    _pDico->donneLibelle(sLang, &_sCode, &_sContenuTransfert) ;
  }
  else
    _sContenuTransfert = string("") ;

  // texteFictif est utilisé pour résoudre le problème des jokers
  // Dans ce cas sContenuTransfert est vide et on peut alors
  // forcer sa valeur avec texteFictif != "" pour que l'on ne
  // change pas la valeur de sCode sur le KillFocus.
  if (string("") != texteFictif)
    _sContenuTransfert = texteFictif ;

  SetText(_sContenuTransfert.c_str()) ;
}

//------------------------------------------------------------------------
// le champ edit perd son focus
//------------------------------------------------------------------------
void
NSUtilLexique::EvKillFocus(HWND hWndGetFocus)
{
	//
	// Récupère le texte
	//
	int oldBuffLen = GetTextLen() ;
	char far* str = new char[oldBuffLen + 1] ;
	GetText(str, oldBuffLen + 1) ;
	string sContenu = string(str) ;
	delete[] str ;

	//
	// si le texte recupéré est != de sContenuTransfert c'est qu'il s'agit d'un texte libre
	// mettre dans sCode string("£?????")
	//
	if (sContenu != _sContenuTransfert)
		_sCode = string("£?????") ;

	if (hWndGetFocus != HWindow)
	{
    ChoixPathoDialog* pDicoDialog = _pDico->getDicoDialog() ;
		if (pDicoDialog)
		{
      bool bMustHideDialog = true ;

      if ((hWndGetFocus == pDicoDialog->HWindow) || (NULL == hWndGetFocus))
        bMustHideDialog = false ;
      else if (pDicoDialog->IsChild(hWndGetFocus))
        bMustHideDialog = false ;
      else if (_LostFocusFunctor)
      {
        TMyApp* pApp = _pDico->pContexte->getSuperviseur()->getApplication() ;
        if (hWndGetFocus == pApp->prendClient()->HWindow)
          bMustHideDialog = false ;
      }

      if (bMustHideDialog)
      {
    	  pDicoDialog->cacherDialogue() ;
			  NSEditDicoGlobal::EvKillFocus(hWndGetFocus) ;
			  if (_LostFocusFunctor)
				  (*_LostFocusFunctor)() ;
      }
		}
		else
			TEdit::EvKillFocus(hWndGetFocus) ;
	}
}

voidNSUtilLexique::EvSetFocus(HWND hWndLostFocus)
{
	NSEditDicoGlobal::EvSetFocus(hWndLostFocus) ;
}

/*********************************************************************/
/*                        CLASSES NSUTILUPDOWN                       */
/*********************************************************************/

DEFINE_RESPONSE_TABLE1(NSUtilEditNumSimpl, TEdit)
   // EV_WM_SETFOCUS,
   EV_WM_KEYUP,
   EV_WM_KEYDOWN,
   EV_WM_CHAR,
   EV_WM_GETDLGCODE,
END_RESPONSE_TABLE;

NSUtilEditNumSimpl::NSUtilEditNumSimpl(NSContexte* pCtx, TWindow *windowParent, int resId, NSUtilUpDownEdit *NSparent)
                   :TEdit(windowParent, resId), NSRoot(pCtx)
{
  _pFather    = NSparent ;
  _pDlgParent = windowParent ;
}

NSUtilEditNumSimpl::~NSUtilEditNumSimpl()
{
}

void
NSUtilEditNumSimpl::incremente()
{
  int  iBuffLen = GetTextLen() ;
  char *pszBuff = new char[iBuffLen + 1] ;
  GetText(pszBuff, iBuffLen + 1) ;
  string sContenu = string(pszBuff) ;
  delete pszBuff ;

  int iContenu = atoi(sContenu.c_str()) ;

  iContenu++ ;

  char szNewContenu[255] ;
  itoa(iContenu, szNewContenu, 10) ;

  sContenu = string(szNewContenu) ;
  SetText(sContenu.c_str()) ;
}

void
NSUtilEditNumSimpl::decremente()
{
  int  iBuffLen = GetTextLen() ;
  char *pszBuff = new char[iBuffLen + 1] ;
  GetText(pszBuff, iBuffLen + 1) ;
  string sContenu = string(pszBuff) ;
  delete pszBuff ;

  int iContenu = atoi(sContenu.c_str()) ;

  if (iContenu > 0)
      iContenu-- ;

  char szNewContenu[255] ;
  itoa(iContenu, szNewContenu, 10) ;

  sContenu = string(szNewContenu) ;
  SetText(sContenu.c_str()) ;
}

void
NSUtilEditNumSimpl::setVal(int iValue)
{
  // 23 est le nombre de digit maximal renvoyé par atoi
  char *pszBuff = new char[23 + 1] ;

  int       i = 0 ;
  while (iValue > 9)
  {
    pszBuff[i++] = (char) ((iValue / 10) + '0') ;
    iValue       = iValue % 10 ;
  }
  pszBuff[i++] = (char) (iValue + '0') ;
  pszBuff[i] = '\0' ;
  SetText(pszBuff) ;

  delete[] pszBuff ;
}

int
NSUtilEditNumSimpl::getVal()
{
  int       iBuffLen = GetTextLen() ;
  char far  *pszBuff = new char[iBuffLen + 1] ;
  GetText(pszBuff, iBuffLen + 1) ;

  size_t    iLen      = strlen(pszBuff) ;
  int       iVal      = 0 ;
  for (size_t i = 0 ; i != iLen ; i++)
    iVal = iVal * 10 + (pszBuff[i] - '0') ;

  delete[] pszBuff ;

  return iVal ;
}

void
NSUtilEditNumSimpl::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(_pDlgParent, NSUtilDialog) ;
  if ((NSUtilDialog*) NULL == pNSUtilDialog)
    return ;

  switch(key)
  {
    case VK_TAB	:
        pNSUtilDialog->ActiveControlSuivant(this) ;
        break ;
    case VK_RETURN  :
    case VK_DOWN  :
        pNSUtilDialog->ActiveControlSuivant(this) ;
        break ;
    default       :
        TEdit::EvKeyDown(key, repeatCount, flags) ;
  }
}


void
NSUtilEditNumSimpl::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(_pDlgParent, NSUtilDialog);
  if ((NSUtilDialog*) NULL == pNSUtilDialog)
    return ;

  switch(key)
  {
    //return
    case VK_RETURN :
    case VK_DOWN   :
      //
      // demander à la boîte de dialogue mère d'activer le controle
      // suivant pControle sinon le premier
      //
      pNSUtilDialog->ActiveControlSuivant(this) ;

    default       :

      TEdit::EvKeyUp(key, repeatcount, flags) ;
  }
}

void
NSUtilEditNumSimpl::EvChar(uint key, uint repeatCount, uint flags)
{
  TEdit::EvChar(key, repeatCount, flags) ;     // Process the new char...
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSUtilEditNumSimpl::EvGetDlgCode(MSG far* /* msg */)
{
	uint retVal = (uint)DefaultProcessing() ;
  retVal |= DLGC_WANTALLKEYS ;
  return retVal ;
}

void
NSUtilEditNumSimpl::SetupWindow()
{
	TEdit::SetupWindow() ;
}

//------------------------------------------------------------------------------
//                     classe  NSUtilUpDown
//------------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUtilUpDown, TUpDown)
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE ;


NSUtilUpDown::NSUtilUpDown(NSContexte* pCtx, TWindow *windowParent, int resId, NSUtilUpDownEdit *parent)
             :TUpDown(windowParent, resId), NSRoot(pCtx)
{
  _pFather = parent ;
}


NSUtilUpDown::~NSUtilUpDown()
{
}

void
NSUtilUpDown::EvLButtonDown(uint /* modKeys */, ClassLib::TPoint& point)
{
	ClassLib::TRect rect ;
	uint            state = GetSpinRectFromPoint(rect, point) ;
	if      (state == csIncrement)
		_pFather->getEditNum()->incremente() ;
	else if (state == csDecrement)
		_pFather->getEditNum()->decremente() ;
}

void
NSUtilUpDown::SetupWindow()
{
  TUpDown::SetupWindow() ;
}

//------------------------------------------------------------------------------
//                      classe  NSUtilUpDownEdit
//------------------------------------------------------------------------------

NSUtilUpDownEdit::NSUtilUpDownEdit(TWindow *windowParent, NSContexte* pCtx, int resEditId, int resUpDownId)
                 :NSRoot(pCtx)
{
	_pEditNumControl = new NSUtilEditNumSimpl(pContexte, windowParent, resEditId, this) ;
	_pUpDownControl  = new NSUtilUpDown(pContexte, windowParent, resUpDownId, this) ;
}


NSUtilUpDownEdit::~NSUtilUpDownEdit()
{
  delete _pEditNumControl ;
  delete _pUpDownControl ;
}

//-------------------------------------------------------------------------

