// -----------------------------------------------------------------------------
// nsmediccontrols.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Author: pameline $
// $Date: 2014/01/29 20:40:59 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#include "nautilus\nssuper.h"
#include "nautilus\nsepicap.h"

#include "nsbb\nsbb.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsmediccontrols.h"

#include "nsbb.rh"
#include "medicament.rh"

const char decimSepar = ',' ;

DEFINE_RESPONSE_TABLE1(NSEditNumSimpl, TEdit)
   // EV_WM_SETFOCUS,
   EV_WM_KEYUP,
   EV_WM_KEYDOWN,
   EV_WM_CHAR,
   EV_WM_GETDLGCODE,
   EV_WM_KILLFOCUS ,
END_RESPONSE_TABLE;

NSEditNumSimpl::NSEditNumSimpl(TWindow *windowParent, int resId, NSUpDownEdit *NSparent, int iTextLen, string sValidator)
               :TEdit(windowParent, resId, iTextLen + 1)
{
  pValidator  = new TFilterValidator(sValidator.c_str()) ;
  pFather     = NSparent ;
  pDlgParent  = windowParent ;

	initCommonData() ;
}

NSEditNumSimpl::NSEditNumSimpl(TWindow *windowParent, int Id, const char far *text, int x, int y, int w, int h, NSUpDownEdit *NSparent, int iTextLen, string sValidator)
               :TEdit(windowParent, Id, text, x, y, w, h, iTextLen + 1)
{
  pValidator  = new TFilterValidator(sValidator.c_str()) ;
  pFather     = NSparent ;
  pDlgParent  = windowParent ;

	initCommonData() ;
}

void
NSEditNumSimpl::initCommonData()
{
  SetValidator(pValidator) ;

  bIntercepte = true;
  bWinStd     = false ;

  iMaxValue     = -1 ;
  iMinValue     = 0 ;
  bAllowDivider = true ;

  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(pDlgParent, NSUtilDialog);
  if (pNSUtilDialog && pNSUtilDialog->pContexte)
  {
    NSContexte* pContexte = pNSUtilDialog->pContexte ;
    if (pContexte && pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
      bWinStd = true ;
  }
  LostFocusResponse = NULL ;
}

void
NSEditNumSimpl::EvKillFocus(HWND hWndGetFocus)
{
  TEdit::EvKillFocus(hWndGetFocus);

  // Checks if between min and max values
  //
  string sContenu = getEditContent() ;
  if (sContenu != string(""))
  {
  	string sLeft  = "" ;
  	string sRight = "" ;
  	getLeftAndRightFromSlash(&sContenu, &sLeft, &sRight) ;
    if (sRight == string(""))
    {
    	int iLeft = atoi(sLeft.c_str()) ;
      if ((iMaxValue != -1) && (iLeft > iMaxValue))
      {
      	string sMaxText = IntToString(iMaxValue) ;
      	setText(sMaxText) ;
      }
      else if (iLeft < iMinValue)
      {
      	string sMinText = IntToString(iMinValue) ;
      	setText(sMinText) ;
      }
    }
  }

	// Calls the functors, if any
  //
  if (NULL != LostFocusResponse)
    (*LostFocusResponse)() ;
}

NSEditNumSimpl::~NSEditNumSimpl()
{
//  delete pValidator ;
}

string
NSEditNumSimpl::getEditContent()
{
	int iBuffLen = GetTextLen() ;
  if (iBuffLen <= 0)
		return string("") ;

  char* pszBuff = new char[iBuffLen + 1] ;
  GetText(pszBuff, iBuffLen + 1) ;
  string sContenu = string(pszBuff) ;
  delete[] pszBuff ;

  return sContenu ;
}

void
NSEditNumSimpl::incremente()
{
  string sContenu = getEditContent() ;
  string sLeft    = "" ;
  string sRight   = "" ;

  getLeftAndRightFromSlash(&sContenu, &sLeft, &sRight) ;

  if (sLeft == string(""))
  	return ;

  int iLeft = atoi(sLeft.c_str()) ;
  if ((iMaxValue != -1) && (iLeft >= iMaxValue))
  	return ;

  iLeft++ ;

  char szNewLeft[33] ;
  itoa(iLeft, szNewLeft, 10) ;

  sContenu = string(szNewLeft) + sRight ;
  SetText(sContenu.c_str()) ;
  size_t iNewLen = strlen(szNewLeft) ;
  SetSelection(0, iNewLen) ;
}

void
NSEditNumSimpl::decremente()
{
  string sContenu = getEditContent() ;
  string sLeft    = "" ;
  string sRight   = "" ;

  getLeftAndRightFromSlash(&sContenu, &sLeft, &sRight) ;

  if (sLeft == string(""))
  	return ;

	int iLeft = atoi(sLeft.c_str()) ;
	if (iLeft <= iMinValue)
  	return ;

	iLeft-- ;

	char szNewLeft[33] ;
	itoa(iLeft, szNewLeft, 10) ;

	sContenu = string(szNewLeft) + sRight ;
	SetText(sContenu.c_str()) ;
	size_t iNewLen = strlen(szNewLeft) ;
	SetSelection(0, iNewLen) ;
}

void
NSEditNumSimpl::getLeftAndRightFromSlash(string* psText, string* psLeft, string* psRight)
{
	if ((NULL == psText) || (NULL == psLeft) || (NULL == psRight))
		return ;

	*psLeft = string("") ;
  *psRight = string("") ;

  if (*psText == string(""))
		return ;

	size_t iPos = psText->find("/") ;
  if (iPos != NPOS)
  {
  	if (iPos == 0)
    {
    	*psLeft  = "1" ;
      *psRight = *psText ;
    }
    else
    {
    	*psLeft  = string(*psText, 0, iPos) ;
      *psRight = string(*psText, iPos, strlen(psText->c_str()) - iPos) ;
    }
  }
  else
  	*psLeft = *psText ;
}

void
NSEditNumSimpl::incrementeDiviseur()
{
	string sContenu = getEditContent() ;
  if (sContenu == "")
  	return ;

	size_t iPos = sContenu.find("/") ;

	if ((iPos == NPOS) || (iPos >= strlen(sContenu.c_str())-1))
  	return ;

  string sDiviseur = string(sContenu, iPos + 1, strlen(sContenu.c_str()) - iPos - 1) ;
  int iDiviseur = atoi(sDiviseur.c_str()) ;
  if (iDiviseur == 0)
  	return ;

	iDiviseur++ ;

  char szNewDiv[33] ;
  itoa(iDiviseur, szNewDiv, 10) ;

  sContenu = string(sContenu, 0, iPos + 1) + szNewDiv ;
  SetText(sContenu.c_str()) ;
  size_t iNewLen = strlen(sContenu.c_str()) ;
  SetSelection(iPos + 1, iNewLen) ;
}

void
NSEditNumSimpl::decrementeDiviseur()
{
  string sContenu = getEditContent() ;
  if (sContenu == "")
  	return ;

	size_t iPos = sContenu.find("/") ;

	if ((iPos == NPOS) || (iPos >= strlen(sContenu.c_str())-1))
  	return ;

  string sDiviseur = string(sContenu, iPos + 1, strlen(sContenu.c_str()) - iPos - 1) ;
  int iDiviseur = atoi(sDiviseur.c_str()) ;
  if (iDiviseur == 0)
  	return ;

  iDiviseur-- ;

  if (iDiviseur == 1)
  {
    sContenu = string(sContenu, 0, iPos) ;
    SetText(sContenu.c_str()) ;
    size_t iNewLen = strlen(sContenu.c_str()) ;
    SetSelection(iNewLen, iNewLen) ;
    return ;
  }

  char szNewDiv[33] ;
  itoa(iDiviseur, szNewDiv, 10) ;

  sContenu = string(sContenu, 0, iPos + 1) + szNewDiv ;
  SetText(sContenu.c_str()) ;
  size_t iNewLen = strlen(sContenu.c_str()) ;
  SetSelection(iPos + 1, iNewLen) ;
}

void
NSEditNumSimpl::setVal(int iValue)
{
	string sValue = IntToString(iValue) ;
  SetText(sValue.c_str()) ;

  // 23 est le nombre de digit maximal renvoyé par atoi
	/*
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
  */
}

void
NSEditNumSimpl::setText(string sTxt)
{
	// replace '.' by local separator
  //
  string sNumTexte = string("") ;
  
  size_t i = sTxt.find(".") ;
  if (i == NPOS)
  	sNumTexte = sTxt ;
  else
  {
  	string sDecimalSeparator = string(1, decimSepar) ;
		if (pFather && pFather->pContexte)
			sDecimalSeparator = pFather->pContexte->getSuperviseur()->getText("0localInformation", "decimalSeparator") ;

    size_t iTextLen = strlen(sTxt.c_str()) ;
    if (iTextLen > 1)
		{
    	if (i == 0)
    		sNumTexte = string("0") + sDecimalSeparator + string(sTxt, 1, strlen(sTxt.c_str())) ;
      else if (i == iTextLen - 1)
      	sNumTexte = string(sTxt, 0, i) ;
    	else
    		sNumTexte = string(sTxt, 0, i) + sDecimalSeparator + string(sTxt, i+1, strlen(sTxt.c_str()) - i - 1) ;
    }
  }

	SetText(sNumTexte.c_str()) ;
}

string
NSEditNumSimpl::getText()
{
  string sText = getEditContent() ;
  if (sText == "")
  	return sText ;

	if (sText[0] == '/')
  	sText = string("1") + sText ;

	size_t pos = sText.find_first_not_of('0') ;
	if (pos == string::npos)
  	return string("") ;

	strip(sText, stripLeft, '0') ;

	if (sText[0] == '/')
		return string("") ;

	string sDecimalSeparator = string(1, decimSepar) ;
	if (pFather && pFather->pContexte)
		sDecimalSeparator = pFather->pContexte->getSuperviseur()->getText("0localInformation", "decimalSeparator") ;
  size_t i = sText.find(sDecimalSeparator) ;

  // Replace local decimal separator by '.'
	if (i != NPOS)
	{
		size_t iSeparLen = strlen(sDecimalSeparator.c_str()) ;
    size_t iTextLen  = strlen(sText.c_str()) ;

    if (i == 0)
    	sText = string("0") + string(".") + string(sText, iSeparLen, iTextLen - iSeparLen) ;
    else if (i + iSeparLen >= iTextLen)
			sText = string(sText, 0, i) ;
    else
    	sText = string(sText, 0, i) + string(".") + string(sText, i + iSeparLen, iTextLen - i - iSeparLen) ;
  }

	return sText ;
}

int
NSEditNumSimpl::getVal()
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
NSEditNumSimpl::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (bWinStd)
	{
  	// return TEdit::EvKeyUp(key, repeatcount, flags) ;
    TEdit::EvKeyUp(key, repeatCount, flags) ;
    return ;
	}

  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(pDlgParent, NSUtilDialog);
  if (!pNSUtilDialog)
    return;

  switch(key)
  {
    case VK_TAB	:
        pNSUtilDialog->ActiveControlSuivant(this);
        break;
    case VK_RETURN  :
    case VK_DOWN  :
        pNSUtilDialog->ActiveControlSuivant(this);
        break;
    default       :
        TEdit::EvKeyDown(key, repeatCount, flags);
  }
  /*if ((key == VK_RETURN)||(key ==VK_TAB) )
        pNSUtilDialog->ActiveControlSuivant(this);
  else
    TEdit::EvKeyDown(key, repeatCount, flags);        */
}


void
NSEditNumSimpl::EvKeyUp(uint key, uint repeatcount, uint flags)
{
	if (bWinStd)
	{
  	// return TEdit::EvKeyUp(key, repeatcount, flags) ;
    TEdit::EvKeyUp(key, repeatcount, flags) ;
    return ;
	}

	NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(pDlgParent, NSUtilDialog);
	if (!pNSUtilDialog)
  	return ;

	switch(key)
	{
  	//return
    case VK_RETURN  :
    case VK_DOWN  :
    	//
      //demander à la boîte de dialogue mère d'activer le controle
      //suivant pControle sinon le premier
      //
      pNSUtilDialog->ActiveControlSuivant(this) ;

    default       :
    	TEdit::EvKeyUp(key, repeatcount, flags);
	}
}

void
NSEditNumSimpl::EvChar(uint key, uint repeatCount, uint flags)
{
	if (Validator && (key != VK_BACK) 	&&
                   (key != VK_RETURN) &&
                   (key != VK_SPACE) 	&&
                   (key != VK_DELETE) &&
                   (key != VK_TAB))
	{
  	if (key == '*')
    {
    	decrementeDiviseur() ;
      return ;
    }
    if (key == '+')
    {
    	incremente() ;
      return ;
    }
    if (key == '-')
    {
    	decremente() ;
      return ;
    }

    int oldBuffLen = GetTextLen() ;
    char far* oldBuff = new char[oldBuffLen+1] ;
    GetText(oldBuff, oldBuffLen+1) ;

    uint   startSel, endSel ;
    GetSelection(startSel, endSel) ;

    if (key == '.')
    	key = ',' ;

    // On vérifie qu'il n'existe pas déjà une ',' ou un '/'
    if (key == ',')
    {
    	for (uint i = 0; i < strlen(oldBuff); i++)
      {
      	if (((oldBuff[i] == '/') || (oldBuff[i] == ',')) &&
                        ((i < startSel) || (i > endSel)))
        {
        	delete[] oldBuff ;
          return ;
        }
      }
    }

    bool wasAppending = endSel == uint(oldBuffLen) ;

    if (key == '/')
    {
    	string sContenu = string(oldBuff) ;
      size_t iPos = sContenu.find("/") ;
      //
      // pas de '/' auparavant
      //
      if (iPos == NPOS)
      {
      	if (wasAppending)
        {
        	if (startSel != endSel)
          	sContenu = string(sContenu, 0, startSel) ;
          sContenu += string("/2") ;
          SetText(sContenu.c_str()) ;
          size_t iNewLen = strlen(sContenu.c_str()) ;
          SetSelection(iNewLen-1, iNewLen) ;
          return ;
        }
      }
      //
      // déjà un '/' auparavant, en dehors de la sélection
      //
      else if ((iPos < startSel) || (iPos > endSel))
      {
      	incrementeDiviseur() ;
        return ;
      }
    }
    // On vérifie qu'il n'existe pas déjà une ',' ou un '/'
    if (key == '/')
    {
    	for (uint i = 0; i < strlen(oldBuff); i++)
      {
      	if (((oldBuff[i] == '/') || (oldBuff[i] == ',')) &&
                        ((i < startSel) || (i > endSel)))
        {
        	delete[] oldBuff ;
          return ;
        }
      }
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
    if (!Validator->HasOption(voOnAppend) || wasAppending && endSel == uint(buffLen))
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
      if (NULL != LostFocusResponse)
      	(*LostFocusResponse)();
    }
    else
    {
    	if (endSel == uint(buffLen) && !Validator->IsValidInput(buff, false))
      	Validator->Error(this) ;
      UnlockBuffer(buff) ;
    }
    SendMessage(EM_SETMODIFY, (TParam1)postMsgModify) ;

    delete[] oldBuff ;
  }
  else
  	TStatic::EvChar(key, repeatCount, flags) ;
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSEditNumSimpl::EvGetDlgCode(MSG far* /* msg */)
{
	uint retVal = (uint)DefaultProcessing() ;
	if (!bWinStd)
  	retVal |= DLGC_WANTALLKEYS ;
	return retVal ;
}

void
NSEditNumSimpl::SetupWindow()
{
	TEdit::SetupWindow() ;
}

bool
NSEditNumSimpl::isVisible()
{
	return IsWindowVisible() ;
}

//------------------------------------------------------------------------------
//                     classe  NSUpDown
//------------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUpDown, TUpDown)
  EV_WM_LBUTTONDOWN,
  EV_WM_KILLFOCUS,
END_RESPONSE_TABLE ;


NSUpDown::NSUpDown(TWindow *windowParent, int resId, NSUpDownEdit *parent)
         :TUpDown(windowParent, resId)
{
  pFather   = parent ;
  _LostFocusResponse = NULL ;
}

NSUpDown::NSUpDown(TWindow *windowParent, int Id, int x, int y, int w, int h, NSUpDownEdit *parent)
         :TUpDown(windowParent, Id, x, y, w, h)
{
  pFather   = parent ;
  _LostFocusResponse = NULL ;
}

NSUpDown::~NSUpDown()
{
	if (NULL != _LostFocusResponse)
  	delete (_LostFocusResponse) ;
}

void NSUpDown::EvKillFocus(HWND hWndGetFocus)
{
  TUpDown::EvKillFocus(hWndGetFocus) ;
  if (NULL != _LostFocusResponse)
    (*_LostFocusResponse)() ;
}

void
NSUpDown::EvLButtonDown(uint /* modKeys */, ClassLib::TPoint& point)
{
  ClassLib::TRect rect ;
  uint            state = GetSpinRectFromPoint(rect, point) ;
  if      (state == csIncrement)
    pFather->getEditNum()->incremente() ;
  else if (state == csDecrement)
    pFather->getEditNum()->decremente() ;
  if (NULL != _LostFocusResponse)
    (*_LostFocusResponse)();
}

void
NSUpDown::SetupWindow()
{
  TUpDown::SetupWindow();
}

bool
NSUpDown::isVisible()
{
	return IsWindowVisible() ;
}

//------------------------------------------------------------------------------
//                      classe  NSUpDownEdit
//------------------------------------------------------------------------------

NSUpDownEdit::NSUpDownEdit(TWindow *windowParent, NSContexte *pCtx, string sRacine, int resEditId,
                    int resUpDownId, int iTextLen, string sValidator)
{
	pContexte = pCtx ;

  pEditNumControl = new NSEditNumSimpl(windowParent, resEditId, this, iTextLen, sValidator) ;
  pUpDownControl  = new NSUpDown      (windowParent, resUpDownId, this) ;

  sRoot = sRacine;
}

NSUpDownEdit::NSUpDownEdit(TWindow *windowParent, NSContexte *pCtx, string sRacine, int EditId,
                    int UpDownId, const char far *text, int x, int y, int w, int h, int iTextLen, string sValidator)
{
	pContexte = pCtx ;

  pEditNumControl = new NSEditNumSimpl(windowParent, EditId, text, x, y, w, h, this, iTextLen, sValidator) ;
  pUpDownControl  = new NSUpDown      (windowParent, UpDownId, x+w, y, 11, h, this) ;

  sRoot = sRacine ;
}

NSUpDownEdit::~NSUpDownEdit()
{
  delete pEditNumControl ;
  delete pUpDownControl ;
}

void
NSUpDownEdit::setupWindow()
{
	setText(sInitialValue) ;
}

void
NSUpDownEdit::Show(int cmdShow)
{
	pEditNumControl->Show(cmdShow) ;
	pUpDownControl->Show(cmdShow) ;
}

bool
NSUpDownEdit::isVisible()
{
	if ( (pEditNumControl->isVisible()) && (pUpDownControl->isVisible()))
  	return true ;
	else
  	return false ;
}

TEdit*
NSUpDownEdit::GetEdit()
{
  return  pEditNumControl ;
}

TUpDown*
NSUpDownEdit::GetUpdown()
{
  return pUpDownControl ;
}

bool
NSUpDownEdit::MoveWindow(int /*x*/, int /*y*/, int /*w*/, int /*h*/, bool /*repaint*/)
{
	return true ;
}

std::string
NSUpDownEdit::getRootSens()
{
	string sRootSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sRoot, &sRootSens) ;
  return sRootSens ;
}

void NSUpDownEdit::createTree(NSPatPathoArray *pPPT, int col)
{
	if (NULL == pPPT)
		return ;

	string sResult = pEditNumControl->getText() ;

	if (sResult == "")
  	return ;

	Message CodeMsg ;

	if (sRoot != "VNBDO1")
	{
  	pPPT->ajoutePatho(sRoot, col) ;
    pPPT->ajoutePatho("VNBDO1", col+1) ;

    CodeMsg.SetUnit("200001") ;
    CodeMsg.SetComplement(sResult) ;
    pPPT->ajoutePatho("£N0;03", &CodeMsg, col+2) ;
  }
  else
  {
  	pPPT->ajoutePatho("VNBDO1", col) ;

    CodeMsg.SetUnit("200001") ;
    CodeMsg.SetComplement(sResult) ;
    pPPT->ajoutePatho("£N0;03", &CodeMsg, col+3) ;
  }
}

void
NSUpDownEdit::createTree(NSPatPathoArray *pPPT, float fVal)//, int iColonneBase)
{
	if ((fVal < 0) || (NULL == pPPT))
  	return ;

	Message CodeMsg ;

  double dVal = fVal ;
  string sVal = DoubleToString(&dVal, 1, 4) ;
  strip(sVal, stripBoth, '0') ;
  strip(sVal, stripRight, '.') ;

  if (sRoot != "VNBDO1")
  {
  	pPPT->ajoutePatho(sRoot, 0) ;
    pPPT->ajoutePatho("VNBDO1", 1) ;

    CodeMsg.SetUnit("200001") ;
    CodeMsg.SetComplement(sVal) ;
    pPPT->ajoutePatho("£N0;03", &CodeMsg, 2) ;
  }
  else
  {
  	pPPT->ajoutePatho("VNBDO1", 0) ;

    CodeMsg.SetUnit("200001") ;
    CodeMsg.SetComplement(sVal) ;
    pPPT->ajoutePatho("£N0;03", &CodeMsg, 1) ;
  }
}

void
NSUpDownEdit::setMinMaxValue(int iMinVal, int iMaxVal)
{
	if (NULL == pEditNumControl)
		return ;

	pEditNumControl->setMinValue(iMinVal) ;
  pEditNumControl->setMaxValue(iMaxVal) ;
}

/*string
NSUpDownEdit::getValNodeComplement(NSPatPathoArray *pPPT, PatPathoIter *pptIter,
                         int iColonneBase, string sCode, string sUnit)
{
  string    sVal = "" ;
  if (((*pptIter) != pPPT->end()) && ((**pptIter)->getColonne() > iColonneBase))
  {
#ifndef _MUE
    if ((**pptIter)->getLexique() == sCode)
    {
      (*pptIter)++ ;
      if ((**pptIter)->getLexique() == sUnit)
      {
         sVal = (**pptIter)->getComplement() ;
        (*pptIter)++ ;
         return sVal ;
      }
    }
#else
    if (((**pptIter)->getLexique() == sCode) && ((**pptIter)->getUnit() == sUnit))
    {
      sVal = (**pptIter)->getComplement() ;
      (*pptIter)++ ;
      return sVal ;
    }
#endif // _MUE
  }

  return sVal ;
}    */

   /*
void
NSUpDownEdit::initFromTree(NSPatPathoArray *pPPT, PatPathoIter *pptIter, string sRoot, ValueControle *pReturnValue)
{
    if (((*pptIter) == pPPT->end()) || ((**pptIter)->getLexique() != sRoot))
        return;


    int iColonne = (**pptIter)->getColonne() ;
    (*pptIter)++ ;

   // string sVal = "" ;
    while (((*pptIter) != pPPT->end()) && ((**pptIter)->getColonne() > iColonne))
    {
        if ((**pptIter)->getLexique() == "VNBDO1")
        {
            (*pptIter)++ ;
            pReturnValue->setQuantValue(StringToDouble(getValNodeComplement(pPPT, pptIter, iColonne, "£N0;03", "200001"))) ;
        }
        else
            (*pptIter)++ ;
    }

 // return sVal ;

}      */



//------------------------------------------------------------------------------
//                     classe  NSUpDownHeure
//------------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUpDownHeure, TUpDown)
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE ;

NSUpDownHeure::NSUpDownHeure(NSContexte *pCtx, TWindow *windowParent, int resId, NSUpDownHeureEdit *parent)
              :TUpDown(windowParent, resId), NSRoot(pCtx)
{
  pFather = parent ;
}

NSUpDownHeure::NSUpDownHeure(NSContexte *pCtx, TWindow *windowParent, int Id, int x, int y, int w, int h, NSUpDownHeureEdit *parent)
              :TUpDown(windowParent, Id, x, y, w, h), NSRoot(pCtx)
{
  pFather = parent ;
}

NSUpDownHeure::~NSUpDownHeure()
{
}

void
NSUpDownHeure::EvLButtonDown(uint /* modKeys */, ClassLib::TPoint& point)
{
  ClassLib::TRect rect ;
  uint            state = GetSpinRectFromPoint(rect, point) ;
  if      (state == csIncrement)
    pFather->getEditHeure()->incrementeHeure() ;
  else if (state == csDecrement)
    pFather->getEditHeure()->decrementeHeure() ;
}

void
NSUpDownHeure::SetupWindow()
{
  TUpDown::SetupWindow() ;
}

bool
NSUpDownHeure::isVisible()
{
   return IsWindowVisible() ;
}

//------------------------------------------------------------------------------
//                      classe  NSEditHeureMedic
//------------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSEditHeureMedic, NSEditHeure)
   // EV_WM_SETFOCUS,
   EV_WM_KEYUP,
   EV_WM_KEYDOWN,
   EV_WM_GETDLGCODE,
END_RESPONSE_TABLE;

NSEditHeureMedic::NSEditHeureMedic(NSContexte *pCtx, TWindow *windowParent, int resId, NSUpDownHeureEdit *NSparent, string sUserLang)
                 :NSEditHeure(pCtx, windowParent, resId, "H", sUserLang)
{
  pFather     = NSparent ;
  pDlgParent  = windowParent ;

  initCommonData() ;
}

NSEditHeureMedic::NSEditHeureMedic(NSContexte *pCtx, TWindow *windowParent, int resId, const char far *text, int x, int y, int w, int h, NSUpDownHeureEdit *NSparent, string sUserLang)
                 :NSEditHeure(pCtx, windowParent, resId, "H", text, x, y, w, h, sUserLang)
{
  pFather     = NSparent ;
  pDlgParent  = windowParent ;

  initCommonData() ;
}

void
NSEditHeureMedic::initCommonData()
{
	bIntercepte = true ;
  bWinStd     = false ;

  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(pDlgParent, NSUtilDialog);
  if (pNSUtilDialog && pNSUtilDialog->pContexte)
  {
    NSContexte* pContexte = pNSUtilDialog->pContexte ;
    if (pContexte && pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
      bWinStd = true ;
  }
}

NSEditHeureMedic::~NSEditHeureMedic()
{
//  delete pValidator ;
}

void
NSEditHeureMedic::incrementeHeure()
{
  string sContent = GetText() ;

  if (strlen(sContent.c_str()) != 5)
    return ;

  string sHour = string(sContent, 0, 2) ;
	string sMin  = string(sContent, 3, 2) ;

  if      (string("00") == sMin)
    sMin = string("30") ;
  else if (string("30") == sMin)
	{
  	sMin = string("00") ;

    int iHour = StringToInt(sHour) ;
    iHour++ ;

    if (iHour > 23)
      iHour = 0 ;
    if (iHour < 0)
      iHour = 23 ;

    sHour = IntToString(iHour) ;
  }

  sContent = sHour + string(1, ':') + sMin ;

  SetText(sContent) ;
}

void
NSEditHeureMedic::decrementeHeure()
{
  string sContent = GetText() ;

  if (strlen(sContent.c_str()) != 5)
    return ;

  string sHour = string(sContent, 0, 2) ;
	string sMin  = string(sContent, 3, 2) ;

  if      (string("30") == sMin)
    sMin = string("00") ;
  else if (string("00") == sMin)
	{
  	sMin = string("30") ;

    int iHour = StringToInt(sHour) ;
    iHour-- ;

    if (iHour > 23)
      iHour = 0 ;
    if (iHour < 0)
      iHour = 23 ;

    sHour = IntToString(iHour) ;
  }

  sContent = sHour + string(1, ':') + sMin ;

  SetText(sContent) ;
}

void
NSEditHeureMedic::setVal(string sValue)
{
  size_t iValueLen = strlen(sValue.c_str()) ;
	if (4 != iValueLen)
		return ;

	string sVal ;
	if ((sValue != "") && (iValueLen >= 4) && (sValue != "00:00"))
		sVal = string(sValue, 0, 2) + string(1, ':') + string(sValue, 2, 2) ;
	else
		sVal = string("00:00") ;

	SetText(sVal) ;
}

string
NSEditHeureMedic::getVal()
{
	string sVal = GetText() ;

  if (strlen(sVal.c_str()) != 5)
    return string("") ;

	string sRes = string(sVal, 0, 2) + string(sVal, 3, 2) ;

	return sRes ;
}

void
NSEditHeureMedic::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (bWinStd)
	{
  	TEdit::EvKeyUp(key, repeatCount, flags) ;
    return ;
	}

  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(pDlgParent, NSUtilDialog);
  if (!pNSUtilDialog)
    return;

  switch(key)
  {
    case VK_TAB	:
        pNSUtilDialog->ActiveControlSuivant(this);
        break;
    case VK_RETURN  :
    case VK_DOWN  :
        pNSUtilDialog->ActiveControlSuivant(this);
        break;
    default       :
        TEdit::EvKeyUp(key, repeatCount, flags);
  }
}

void
NSEditHeureMedic::EvKeyUp(uint key, uint repeatcount, uint flags)
{
	if (bWinStd)
	{
  	TEdit::EvKeyUp(key, repeatcount, flags) ;
    return ;
	}

	NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(pDlgParent, NSUtilDialog);
	if (!pNSUtilDialog)
		return ;

	switch(key)
	{
  	//return
    case VK_RETURN  :
  	case VK_DOWN  :
    	//
    	//demander à la boîte de dialogue mère d'activer le controle
  		//suivant pControle sinon le premier
			//
    	pNSUtilDialog->ActiveControlSuivant(this) ;

    default :
    	TEdit::EvKeyUp(key, repeatcount, flags) ;
	}
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSEditHeureMedic::EvGetDlgCode(MSG far* /* msg */)
{
	uint retVal = (uint)DefaultProcessing() ;
	if (false == bWinStd)
  	retVal |= DLGC_WANTALLKEYS ;
	return retVal ;
}

void
NSEditHeureMedic::SetupWindow()
{
	TEdit::SetupWindow() ;
}

bool
NSEditHeureMedic::isVisible()
{
	return IsWindowVisible() ;
}

//------------------------------------------------------------------------------
//                      classe NSUpDownHeureEdit
//------------------------------------------------------------------------------

NSUpDownHeureEdit::NSUpDownHeureEdit(NSContexte *pCtx, TWindow *windowParent, int resEditId, int resUpDownId, string sUserLang)
                  :NSRoot(pCtx)
{
	sLang = sUserLang ;

	pEditDateControl = new NSEditHeureMedic(pContexte, windowParent, resEditId, this, sLang) ;
	pUpDownControl   = new NSUpDownHeure   (pContexte, windowParent, resUpDownId, this) ;
}

NSUpDownHeureEdit::NSUpDownHeureEdit(NSContexte *pCtx, TWindow *windowParent, int EditId, int UpDownId, const char far *text, int x, int y, int w, int h, string sUserLang)
                  :NSRoot(pCtx)
{
	sLang = sUserLang ;

	pEditDateControl = new NSEditHeureMedic(pContexte, windowParent, EditId, text, x, y, w, h, this, sLang) ;
	pUpDownControl   = new NSUpDownHeure   (pContexte, windowParent, UpDownId, x+w, y, 11, h, this) ;
}

NSUpDownHeureEdit::~NSUpDownHeureEdit()
{
	delete pEditDateControl ;
	delete pUpDownControl ;
}

void
NSUpDownHeureEdit::Show(int cmdShow)
{
	pEditDateControl->Show(cmdShow) ;
	pUpDownControl->Show(cmdShow) ;
}

bool
NSUpDownHeureEdit::isVisible()
{
	if ( (pEditDateControl->isVisible()) && (pUpDownControl->isVisible()))
  	return true ;
	else
  	return false ;
}

NSPatPathoArray* PriseHeure::CreateTree(NSContexte* cont)
{
	NSPatPathoArray* result = new NSPatPathoArray(cont->getSuperviseur()) ;
	result->ajoutePatho("KHHMM1", 0) ;

  Message CodeMsg ;
	CodeMsg.SetUnit("2HE011") ;
	CodeMsg.SetComplement(_heure) ;
	result->ajoutePatho("£H0;04", &CodeMsg, 1) ;

	result->ajoutePatho("VNBDO1", 1) ;
  CodeMsg.Reset() ;
	CodeMsg.SetUnit("200001") ;
  CodeMsg.SetComplement(_quantity) ;
	result->ajoutePatho("£N0;03", &CodeMsg, 2) ;

	return result ;
}

//------------------------------------------------------------------------------
//                  classe NSUpDownEditHeureControle
//------------------------------------------------------------------------------
NSUpDownEditHeureControle::NSUpDownEditHeureControle(TWindow *windowParent, NSContexte *pCtx, string sRacine,
                            int resEditId, int resUpDownId, int resHeureEdit,
                            int resHeureUpDown, int /* iTextLen */, string /* sValidator */)
                          :NSRoot(pCtx)
{
	string sLang = pCtx->getUserLanguage() ;

	pEditHeure = new NSUpDownHeureEdit(pContexte, windowParent, resHeureEdit, resHeureUpDown, sLang) ;
	pEditNum   = new NSUpDownEdit(windowParent, pContexte, "", resEditId, resUpDownId/*, resStaticTextId*/) ;
	sRoot = sRacine ;
}

NSUpDownEditHeureControle::NSUpDownEditHeureControle(TWindow *windowParent, NSContexte *pCtx, string sRacine,
                            int EditId, int UpDownId,
                            const char far *text, int x, int y, int w, int h,
                            int HeureEdit, int HeureUpDown,
                            const char far *textH, int xH, int yH, int wH, int hH,
                            int /* iTextLen */, string /* sValidator */)
                          :NSRoot(pCtx)
{
	string sLang = pCtx->getUserLanguage() ;

	pEditNum   = new NSUpDownEdit(windowParent, pContexte, "", EditId, UpDownId, text, x, y, w, h) ;
	pEditHeure = new NSUpDownHeureEdit(pContexte, windowParent, HeureEdit, HeureUpDown, textH, xH, yH, wH, hH, sLang) ;

	sRoot = sRacine ;
}

NSUpDownEditHeureControle::~NSUpDownEditHeureControle()
{
  delete pEditHeure ;
  delete pEditNum ;
}

void
NSUpDownEditHeureControle::Show(int cmdShow)
{
	pEditHeure->Show(cmdShow) ;
	pEditNum->Show(cmdShow) ;
}

bool
NSUpDownEditHeureControle::isVisible()
{
	if ( (pEditHeure->isVisible()) && (pEditNum->isVisible()))
		return true ;
	else
		return false ;
}


/*bool
NSUpDownEditHeureControle::MoveWindow(int x, int y, int w, int h, bool repaint)
{

} */


void
NSUpDownEditHeureControle::createTree(NSPatPathoArray *pPPT, string sHeure)
{
	if ((sRoot == "") || (sHeure == "") || (NULL == pPPT))
  	return ;

	string sVal = pEditNum->getEditNum()->getText() ;
	if (sVal == "")
  	return ;

	pPPT->ajoutePatho(sRoot, 0) ;

	Message CodeMsg ;

   // pPPT->ajoutePatho("KHHMM1", 1, 1) ;

	CodeMsg.SetUnit("2HE011") ;
	CodeMsg.SetComplement(sHeure) ;
	pPPT->ajoutePatho("£H0;04", &CodeMsg, 1) ;

	pPPT->ajoutePatho("VNBDO1", 1) ;
  CodeMsg.Reset() ;
	CodeMsg.SetUnit("200001") ;
	CodeMsg.SetComplement(sVal) ;
	pPPT->ajoutePatho("£N0;03", &CodeMsg, 2) ;
}


void
NSUpDownEditHeureControle::CreateTree(NSPatPathoArray* /* pPPT*/)
{
  //createTree(pPPT, getHeure() , (float)getValue());             // delete fonction FIXME
}

void
NSUpDownEditHeureControle::createTree(NSPatPathoArray *pPPT, string sHeure, float fVal)
{
	if ((sRoot == "") || (sHeure == "") || (NULL == pPPT))
		return ;

	pPPT->ajoutePatho(sRoot, 0) ;

	Message CodeMsg ;

	CodeMsg.SetUnit("2HE011") ;
  CodeMsg.SetComplement(sHeure) ;
  pPPT->ajoutePatho("£H0;04", &CodeMsg, 1) ;

  pPPT->ajoutePatho("VNBDO1", 1) ;

  double dVal = fVal ;
  string sVal = DoubleToString(&dVal, 1, 4) ;
  strip(sVal, stripBoth, '0') ;
  strip(sVal, stripRight, '.') ;

  CodeMsg.Reset() ;
	CodeMsg.SetUnit("200001") ;
	CodeMsg.SetComplement(sVal) ;
	pPPT->ajoutePatho("£N0;03", &CodeMsg, 2) ;
}

     /*
void
NSUpDownEditHeureControle::initFromTree(NSPatPathoArray *pPPT, PatPathoIter *pptIter, string sRoot, ValueControle *pReturnValue)
{
    if (((*pptIter) == pPPT->end()) || ((**pptIter)->getLexique() != sRoot))
        return ;


    int iColonne = (**pptIter)->getColonne() ;
    (*pptIter)++ ;
    if ((**pptIter)->getLexique() == "£H0;04" )
    {
        pReturnValue->setStringValue(getValNodeComplement(pPPT, pptIter, iColonne, "£H0;04", "2HE011")) ;
    }

   // string sVal = "" ;
    while (((*pptIter) != pPPT->end()) && ((**pptIter)->getColonne() > iColonne)
                                        && ((**pptIter)->getLexique() != "VNBDO1"))
        (*pptIter)++ ;

    (*pptIter)++ ;
    pReturnValue->setQuantValue(StringToDouble(getValNodeComplement(pPPT, pptIter, iColonne, "£N0;03", "200001"))) ;
}         */


//------------------------------------------------------------------------------
//                  classe NSUpDownEditLexiqueControl
//------------------------------------------------------------------------------
NSUpDownEditLexiqueControl::NSUpDownEditLexiqueControl(TWindow *windowParent, NSContexte *pCtx, string sRacine,
                                int resEditId, int resUpDownId, int resStaticTextId,
                                int resEditLexiquId, int /* iTextLen */, string /* sValidator */)
                         //: NSUpDownEdit(windowParent, pCtx,"", resEditId, resUpDownId, resStaticTextId, iTextLen, sValidator)
                        // : NSMedicBaseControl(pCtx, sRacine)
                           :NSRoot(pCtx)
{
	if (resEditLexiquId != -1)
  	pEditLexique = new NSUtilLexique(pContexte, windowParent, resEditLexiquId,  pCtx->getDico()) ;
	pEditNum = new NSUpDownEdit(windowParent, pContexte, "", resEditId, resUpDownId, resStaticTextId) ;
	sRoot = sRacine ;
}


NSUpDownEditLexiqueControl::~NSUpDownEditLexiqueControl()
{
  if (pEditLexique)
    delete pEditLexique ;
  delete pEditNum ;
}


void
NSUpDownEditLexiqueControl::Show(int cmdShow)
{
  if (pEditLexique)
    pEditLexique->Show(cmdShow) ;
  pEditNum->Show(cmdShow) ;
}

bool
NSUpDownEditLexiqueControl::isVisible()
{
  //if ( (pEditLexique->isVisible()) && (pEditNum->isVisible()))
  if (pEditNum->isVisible())
    return true ;
  else
    return false ;
}


/*bool
NSUpDownEditLexiqueControl::MoveWindow(int x, int y, int w, int h, bool repaint)
{

} */

void
NSUpDownEditLexiqueControl::createTree(NSPatPathoArray *pPPT, string sCode, float fVal)
{
	if ((sRoot == "") || (sCode == "") || (fVal == 0) || (NULL == pPPT))
		return ;

	pPPT->ajoutePatho(sRoot, 0) ;    // sRoot = "KEVEI1"

	Message CodeMsg ;

	pPPT->ajoutePatho("LTYPE1", 1) ;
	pPPT->ajoutePatho(sCode, 2) ;

	pPPT->ajoutePatho("VNBDO1", 1) ;

	double dVal = fVal ;
	string sVal = DoubleToString(&dVal, 1, 4) ;
	strip(sVal, stripBoth, '0') ;
	strip(sVal, stripRight, '.') ;

	CodeMsg.SetUnit("200001") ;
	CodeMsg.SetComplement(sVal) ;
	pPPT->ajoutePatho("£N0;03", &CodeMsg, 2) ;
}

//------------------------------------------------------------------------------
//                  classe NSTexteLibre
//------------------------------------------------------------------------------

NSTexteLibre::NSTexteLibre(TWindow *win, NSContexte* pCtx, string sRacine, int resEditId)
             :TEdit(win, resEditId), NSRoot(pCtx)
{
	sRoot = sRacine ;
}

NSTexteLibre::NSTexteLibre(TWindow *win, NSContexte* pCtx, const char far *text, int x, int y, int w, int h, string sRacine, int EditId)
             :TEdit(win, EditId, text, x, y, w, h), NSRoot(pCtx)
{
	sRoot = sRacine ;
}

NSTexteLibre::~NSTexteLibre()
{
}

string
NSTexteLibre::getTexte()
{
  int       iBuffLen = GetTextLen() ;
  char far  *pszBuff = new char[iBuffLen + 1] ;
  GetText(pszBuff, iBuffLen + 1) ;
  string sRes = string(pszBuff) ;
  delete[] pszBuff ;

  return sRes ;
}

 /*
void
NSTexteLibre::createTree(NSPatPathoArray *pPPT, string sText)
{

   //la racine sert seulement pour identifier le controle
    if ((sRoot == "") || (sText == ""))
        return;
    Message *pCodeMsg = new Message("", "", "", "A", "", "", "") ;
#ifndef _MUE
    //pPPT->ajoutePatho("2HE011", 1, 1) ;
   // pCodeMsg->SetComplement(sText) ;
    pCodeMsg->SetTexteLibre(sText) ;
    pPPT->ajoutePatho("£C;020", pCodeMsg, 0, 0) ;

#else
    pCodeMsg->SetTexteLibre(sText) ;
    pPPT->ajoutePatho("£C;020", pCodeMsg, 0, 0) ;
#endif // _MUE
    delete pCodeMsg ;

}    */
  /*

void
NSTexteLibre::initFromTree(NSPatPathoArray *pPPT, PatPathoIter *pptIter, string sRoot, ValueControle *pReturnValue)
{
    if ((*pptIter) == pPPT->end())
        return ;

   // int iColonne = (**pptIter)->getColonne() ;
   //
    if ((**pptIter)->getLexique() == "£C;020" )
    {
        pReturnValue->setStringValue((**pptIter)->getTexteLibre()) ;
        if ((*pptIter) != pPPT->end())
            (*pptIter)++ ;
    }
}            */


DEFINE_RESPONSE_TABLE1(NSFreeTextDlg, NSUtilDialog)
  EV_COMMAND(IDOK,     CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSFreeTextDlg::NSFreeTextDlg(TWindow* pPere, NSContexte* pCtx, string* pText)
              :NSUtilDialog(pPere, pCtx, "FREETEXTDLG", pNSDLLModule)
{
try
{
  _pText = pText ;

  _pFreeText = new NSTexteLibre(this, pContexte, "£C;020", IDC_TEXTLIBRE) ;
}
catch (...)
{
	erreur("Exception NSFreeTextDlg ctor.",  standardError, 0) ;
}
}

NSFreeTextDlg::~NSFreeTextDlg()
{
  delete _pFreeText ;
}

void
NSFreeTextDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
  if (NULL != _pText)
    _pFreeText->setText(*_pText) ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSFreeTextDlg::CmOk()
{
try
{
  string sText = _pFreeText->getTexte() ;

  if (NULL != _pFreeText)
    *_pText = sText ;

	CloseWindow(IDOK) ;
}
catch (...)
{
  erreur("Exception NSFreeTextDlg::CmOk.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSFreeTextDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}

//------------------------------------------------------------------------------
//                  classe NSRythmesRegIregControle
//------------------------------------------------------------------------------
/*
NSRythmesRegIregControle::NSRythmesRegIregControle(TWindow *windowParent, NSContexte *pCtx, string sRacine,
                         int resEditTimeId, int resUpDownTimeId,
                         int resComboTimeId, VecteurString *pVecteurStringTime,
                         int resEditFoisId, int resUpDownFoisId,
                         int resEditPeriodId, int resUpDownPeriodId,
                         int resComboPeriodId, VecteurString *pVecteurStringPeriod,
                         int iTextLen, string sValidator )
                    : NSMedicBaseControl(pCtx, sRacine)
{
    pEditNoOfTimes = new NSUpDownEdit(windowParent, pContexte,"", resEditTimeId, resUpDownTimeId);
    pTimeUnits = new NSComboBox(windowParent, resComboTimeId, pContexte, pVecteurStringTime) ;
    if ((resEditFoisId != -1) && (resUpDownFoisId != -1))
        pEditNoFois = new NSUpDownEdit(windowParent, pContexte,"", resEditFoisId, resUpDownFoisId);
    else
        pEditNoFois = NULL;
    pEditNoOfPeriods = new NSUpDownEdit(windowParent, pContexte,"", resEditPeriodId, resUpDownPeriodId);
    pPeriodUnits  = new NSComboBox(windowParent, resComboPeriodId, pContexte, pVecteurStringPeriod) ;
}


NSRythmesRegIregControle::~NSRythmesRegIregControle()
{
    if (pEditNoFois)
        delete pEditNoFois ;
    delete pEditNoOfTimes ;
    delete pTimeUnits ;
    delete pEditNoOfPeriods ;
    delete pPeriodUnits ;
}


void
NSRythmesRegIregControle::Show(int cmdShow)
{
    if (pEditNoFois)
        pEditNoFois->Show(cmdShow);
    if (pStaticTextSupl)
        pStaticTextSupl->Show(cmdShow);
    pEditNoOfTimes->Show(cmdShow);
    pTimeUnits->Show(cmdShow);
    pEditNoOfPeriods->Show(cmdShow);
    pPeriodUnits->Show(cmdShow);
}


bool
NSRythmesRegIregControle::isVisible()
{
   // bool bVis = true ;
    if ((pEditNoFois) && (!pEditNoFois->isVisible()))
        //bVis = false;
        return false;
    if( (!pEditNoOfTimes->isVisible())       &&
        (!pTimeUnits->isVisible())           &&
        (!pEditNoOfPeriods->isVisible())     &&
        (!pPeriodUnits->isVisible()))
        return false;
    return true;

}





void
NSRythmesRegIregControle::createTree(NSPatPathoArray *pPPT, int iNoTime,
                                string sTimeUnit, int iFois, int iNoPeriods, string sPeriodUnit)
{
    if ((sRoot == "") || (sTimeUnit == "") || (sTimeUnit == "") || (iNoTime == 0) || (iNoPeriods == 0))
        return;

    pPPT->ajoutePatho(sRoot, 0, 0) ;

    Message *pCodeMsg = new Message("", "", "", "A", "", "", "") ;


    double dVal = iNoTime ;
    string sNoTime = DoubleToString(&dVal, 1, 4) ;
    strip(sNoTime, stripBoth, '0') ;
    strip(sNoTime, stripRight, '.') ;

    dVal = iNoPeriods ;
    string sNoPeriods = DoubleToString(&dVal, 1, 4) ;
    strip(sNoPeriods, stripBoth, '0') ;
    strip(sNoPeriods, stripRight, '.') ;

    pPPT->ajoutePatho("KDURA1", 1, 1) ;
#ifndef _MUE
        pPPT->ajoutePatho(sTimeUnit, 2, 1) ;
        pCodeMsg->SetComplement(sNoTime) ;
        pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 0) ;
#else
        pCodeMsg->SetUnit(sTimeUnit) ;
        pCodeMsg->SetComplement(sNoTime) ;
        pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 1) ;
#endif // _MUE

        pPPT->ajoutePatho("KDURC1", 1, 1) ;
#ifndef _MUE
        pPPT->ajoutePatho(sPeriodUnit, 2, 1) ;
        pCodeMsg->SetComplement(sNoPeriods) ;
        pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 0) ;
#else
        pCodeMsg->SetUnit(sPeriodUnit) ;
        pCodeMsg->SetComplement(sNoPeriods) ;
        pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 1) ;
#endif // _MUE

        if ((pEditNoFois) && ( iFois != 0))
        {
            pPPT->ajoutePatho("VAINC1", 1, 1) ;
            dVal = iFois ;
            string sFois = DoubleToString(&dVal, 1, 4) ;
            strip(sFois, stripBoth, '0') ;
            strip(sFois, stripRight, '.') ;
#ifndef _MUE
            pPPT->ajoutePatho("2FOIS1", 2, 1) ;
            pCodeMsg->SetComplement(sFois) ;
            pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 0) ;
#else
            pCodeMsg->SetUnit("2FOIS1") ;
            pCodeMsg->SetComplement(sFois) ;
            pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 1) ;
#endif // _MUE
        }

    delete pCodeMsg ;

}


void
NSRythmesRegIregControle::initFromTree(NSPatPathoArray *pPPT, PatPathoIter *pptIter, string sRoot, ValueControle *pReturnValue)
{
    if (((*pptIter) == pPPT->end()) || ((**pptIter)->getLexique() != sRoot))
        return ;
    int iColonne = (**pptIter)->getColonne() ;
   (*pptIter)++ ;

    while (((*pptIter) != pPPT->end()) && ((**pptIter)->getColonne() > iColonne)
                                        && ((**pptIter)->getLexique() != "KDURA1"))
    {
        (*pptIter)++ ;
    }
    if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;
    pReturnValue->setStringValue((**pptIter)->getUnit()) ;
    pReturnValue->setQuantValue(StringToDouble((**pptIter)->getComplement())) ;
    if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;
     if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;
    pReturnValue->setCodeSupl((**pptIter)->getUnit()) ;
    pReturnValue->setTempsUnits(StringToDouble((**pptIter)->getComplement())) ;

    if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;
    if ((pEditNoFois) && ((*pptIter) != pPPT->end()))
    {
         //pReturnValue->setFois(StringToDouble(getValNodeComplement(pPPT, pptIter, iColonne, "£N0;03", "2FOIS1"))) ;
         if ((*pptIter) != pPPT->end())
            (*pptIter)++ ;
         pReturnValue->setFois(StringToDouble((**pptIter)->getComplement())) ;
         if ((*pptIter) != pPPT->end())
            (*pptIter)++ ;
    }

}

 */
//------------------------------------------------------------------------------
//                  classe NSRythmesRegIregCycle
//------------------------------------------------------------------------------
/*
NSRythmesRegIregCycle::NSRythmesRegIregCycle(TWindow *windowParent, NSContexte *pCtx, string sRacine, string sPos,
                         int resEditTimeId, int resUpDownTimeId
                         int resComboTimeId, VecteurString *pVecteurStringTime,
                         int resEditFoisId, int resUpDownFoisId
                         int iTextLen, string sValidator )
                    : NSMedicBaseControl(pCtx, sRacine)
{
    pEditNoOfTimes = new NSUpDownEdit(windowParent, pContexte,"", resEditTimeId, resUpDownTimeId);
    pTimeUnits = new NSComboBox(windowParent, resComboTimeId, pContexte, pVecteurStringTime) ;
    if ((resEditFoisId != -1) && (resUpDownFoisId != -1))
        pEditNoFois = new NSUpDownEdit(windowParent, pContexte,"", resEditFoisId, resUpDownFoisId);
    else
        pEditNoFois = NULL;
    pEditNoUnitsMed = new NSUpDownEdit(windowParent, pContexte,"", resEditNoId, resUpDownNoId);
    sPosition = sPos;
}










void
NSRythmesRegIregCycle::createTree(NSPatPathoArray *pPPT)
{
    if ((sRoot == "") || !pPPT)
        return ;

    string  sDose       = pEditNoUnitsMed->getEditNum()->getText() ;

    int     iNoTime     = pEditNoOfTimes->getValue() ;
    string  sTimeUnit   = pTimeUnits->getSelCode() ;
    int     iFois = 0 ;
    if (sPosition == "KRYLI1")
        iFois = pEditNoFois->getValue() ;
    int     iNoPeriods  = pEditNoOfPeriods->getValue() ;
    string  sPeriodUnit = pPeriodUnits->getSelCode() ;

    if ((sTimeUnit == "") || (sTimeUnit == "") || (iNoTime == 0) || (iNoPeriods == 0))
        return ;

    pPPT->ajoutePatho(sRoot, 0, 0) ;

    if (sPosition == "KRYRE1")
        pPPT->ajoutePatho("KRYRE1", 1, 1) ;
    else if (sPosition == "KRYLI1")
        pPPT->ajoutePatho("KRYLI1", 1, 1) ;

    Message *pCodeMsg = new Message("", "", "", "A", "", "", "") ;

    double dVal = iNoTime ;
    string sNoTime = DoubleToString(&dVal, 1, 4) ;
    strip(sNoTime, stripBoth, '0') ;
    strip(sNoTime, stripRight, '.') ;

    dVal = iNoPeriods ;
    string sNoPeriods = DoubleToString(&dVal, 1, 4) ;
    strip(sNoPeriods, stripBoth, '0') ;
    strip(sNoPeriods, stripRight, '.') ;

    pPPT->ajoutePatho("KDURA1", 2, 1) ;
#ifndef _MUE
    pPPT->ajoutePatho(sTimeUnit, 3, 1) ;
    pCodeMsg->SetComplement(sNoTime) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 0) ;
#else
    pCodeMsg->SetUnit(sTimeUnit) ;
    pCodeMsg->SetComplement(sNoTime) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 1) ;
#endif // _MUE

    pPPT->ajoutePatho("KDURC1", 2, 1) ;
#ifndef _MUE
    pPPT->ajoutePatho(sPeriodUnit, 3, 1) ;
    pCodeMsg->SetComplement(sNoPeriods) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 0) ;
#else
    pCodeMsg->SetUnit(sPeriodUnit) ;
    pCodeMsg->SetComplement(sNoPeriods) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 1) ;
#endif // _MUE

    if ((pEditNoFois) && ( iFois != 0))
    {
        pPPT->ajoutePatho("VAINC1", 2, 1) ;
        dVal = iFois ;
        string sFois = DoubleToString(&dVal, 1, 4) ;
        strip(sFois, stripBoth, '0') ;
        strip(sFois, stripRight, '.') ;
#ifndef _MUE
        pPPT->ajoutePatho("2FOIS1", 3, 1) ;
        pCodeMsg->SetComplement(sFois) ;
        pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 0) ;
#else
        pCodeMsg->SetUnit("2FOIS1") ;
        pCodeMsg->SetComplement(sFois) ;
        pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 1) ;
#endif // _MUE
    }

    pPPT->ajoutePatho("VNBDO1", 1, 1) ;
#ifndef _MUE
    pPPT->ajoutePatho("200001", 2, 1) ;
    pCodeMsg->SetComplement(sDose) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 0) ;
#else
    pCodeMsg->SetUnit("200001") ;
    pCodeMsg->SetComplement(sDose) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 1) ;
#endif // _MUE

    delete pCodeMsg ;
}

void                                                    //            //                //          //             //                //
NSRythmesRegIregCycle::createTree(NSPatPathoArray *pPPT, int iNoTime, string sTimeUnit, int iFois, int iNoPeriods, string sPeriodUnit, float fVal)
{
    if ((sRoot == "") || (sTimeUnit == "") || (sTimeUnit == "") || (iNoTime == 0) || (iNoPeriods == 0))
        return;

    pPPT->ajoutePatho(sRoot, 0, 0) ;

    if (sPosition == "KRYRE1")
        pPPT->ajoutePatho("KRYRE1", 1, 1) ;
    else if (sPosition == "KRYLI1")
            pPPT->ajoutePatho("KRYLI1", 1, 1) ;

    Message *pCodeMsg = new Message("", "", "", "A", "", "", "") ;

    double dVal = iNoTime ;
    string sNoTime = DoubleToString(&dVal, 1, 4) ;
    strip(sNoTime, stripBoth, '0') ;
    strip(sNoTime, stripRight, '.') ;

    dVal = iNoPeriods ;
    string sNoPeriods = DoubleToString(&dVal, 1, 4) ;
    strip(sNoPeriods, stripBoth, '0') ;
    strip(sNoPeriods, stripRight, '.') ;

    pPPT->ajoutePatho("KDURA1", 2, 1) ;    // duree de la cure premier edit
#ifndef _MUE
    pPPT->ajoutePatho(sTimeUnit, 3, 1) ;
    pCodeMsg->SetComplement(sNoTime) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 0) ;
#else
    pCodeMsg->SetUnit(sTimeUnit) ;
    pCodeMsg->SetComplement(sNoTime) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 1) ;
#endif // _MUE

    pPPT->ajoutePatho("KDURC1", 2, 1) ;     //duree du cycle
#ifndef _MUE
    pPPT->ajoutePatho(sPeriodUnit, 3, 1) ;
    pCodeMsg->SetComplement(sNoPeriods) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 0) ;
#else
    pCodeMsg->SetUnit(sPeriodUnit) ;
    pCodeMsg->SetComplement(sNoPeriods) ;
    pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 1) ;
#endif // _MUE

    if ((pEditNoFois) && ( iFois != 0))
    {
        pPPT->ajoutePatho("VAINC1", 2, 1) ;   //   milieu
        dVal = iFois ;
        string sFois = DoubleToString(&dVal, 1, 4) ;
        strip(sFois, stripBoth, '0') ;
        strip(sFois, stripRight, '.') ;
#ifndef _MUE
        pPPT->ajoutePatho("2FOIS1", 3, 1) ;
        pCodeMsg->SetComplement(sFois) ;
        pPPT->ajoutePatho("£N0;03", pCodeMsg, 2, 0) ;
#else
        pCodeMsg->SetUnit("2FOIS1") ;
        pCodeMsg->SetComplement(sFois) ;
        pPPT->ajoutePatho("£N0;03", pCodeMsg, 3, 1) ;
#endif // _MUE
    }
    //pPPT->ajoutePatho("VNBDO1", 1, 1) ;


    delete pCodeMsg ;

}


void
NSRythmesRegIregCycle::initFromTree(NSPatPathoArray *pPPT, PatPathoIter *pptIter, string sRoot, ValueControle *pReturnValue)
{
    if (((*pptIter) == pPPT->end()) || ((**pptIter)->getLexique() != sRoot))
        return ;
   // int iColonne = (**pptIter)->getColonne() ;
    if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;

    while ( ((*pptIter) != pPPT->end()) &&
           ((**pptIter)->getLexique() != "KRYLI1") && ((**pptIter)->getLexique() != "KRYRE1") )
    {
        (*pptIter)++ ;
    }

    while (((*pptIter) != pPPT->end()) && ((**pptIter)->getLexique() != "KDURA1"))
    {
        (*pptIter)++ ;
    }
    if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;
    pReturnValue->setStringValue((**pptIter)->getUnit()) ;
   // pReturnValue->setQuantValue(StringToDouble((**pptIter)->getComplement())) ;
   pReturnValue->setNoTime(StringToDouble((**pptIter)->getComplement())) ;
    if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;
     if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;
    pReturnValue->setCodeSupl((**pptIter)->getUnit()) ;
    pReturnValue->setTempsUnits(StringToDouble((**pptIter)->getComplement())) ;

    if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;
  
    if  ((**pptIter)->getLexique() == "VAINC1")
    {
         if ((*pptIter) != pPPT->end())
            (*pptIter)++ ;
         pReturnValue->setFois(StringToDouble((**pptIter)->getComplement())) ;
         if ((*pptIter) != pPPT->end())
            (*pptIter)++ ;
    }
    while (((*pptIter) != pPPT->end()) && ((**pptIter)->getLexique() != "VNBDO1"))
    {
        (*pptIter)++ ;
    }
    if ((*pptIter) != pPPT->end())
        (*pptIter)++ ;

    //int iColonne = (**pptIter)->getColonne() ;
    pReturnValue->setQuantValue(StringToDouble((**pptIter)->getComplement())) ;
   // pReturnValue->setQuantValue(StringToDouble(getValNodeComplement(pPPT, pptIter, iColonne, "£N0;03", "200001"))) ;
    if ((*pptIter) != pPPT->end())
            (*pptIter)++ ;

}    */


//------------------------------------------------------------------------------
//                      classe  NSCheckBoxControle
//------------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSCheckBoxControle, TCheckBox)
	EV_WM_KILLFOCUS,
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
END_RESPONSE_TABLE;

NSCheckBoxControle::NSCheckBoxControle(TWindow *windowParent, NSContexte* pCtx, string sRacine, int resId)
                   :TCheckBox(windowParent, resId), NSRoot(pCtx)
{
  LostChangeReponse = NULL ;
  pContexte         = pCtx ;
  sRoot             = sRacine ;
  iInitialStatus    = BF_UNCHECKED ;
}

NSCheckBoxControle::NSCheckBoxControle(TWindow *windowParent, NSContexte* pCtx, string sRacine, int Id, const char far* title, int x, int y, int w, int h)
                   :TCheckBox(windowParent, Id, title, x, y, w, h), NSRoot(pCtx)
{
  LostChangeReponse = NULL ;
  pContexte         = pCtx ;
  sRoot             = sRacine ;
  iInitialStatus    = BF_UNCHECKED ;
}

void   NSCheckBoxControle::BNClicked()
{
  TCheckBox::BNClicked() ;
  if (LostChangeReponse != NULL)
    (*LostChangeReponse)() ;
}

NSCheckBoxControle::~NSCheckBoxControle()
{
  if (LostChangeReponse != NULL)
    delete(LostChangeReponse) ;
}

void
NSCheckBoxControle::EvKillFocus(HWND hWndGetFocus)
{
  TCheckBox::EvKillFocus(hWndGetFocus) ;
  if (NULL != LostChangeReponse)
    (*LostChangeReponse)() ;
}

void
NSCheckBoxControle::Show(int cmdShow)
{
  Show(cmdShow) ;
}

bool
NSCheckBoxControle::isVisible()
{
	return IsWindowVisible() ;
}

bool
NSCheckBoxControle::MoveWindow(int /*x*/, int /*y*/, int /*w*/, int /*h*/, bool /*repaint*/)
{
	return true ;
}

void
NSCheckBoxControle::createTree(NSPatPathoArray *pPPT, uint iVal)
{
	if ((iVal > 0) && (NULL != pPPT))
		pPPT->ajoutePatho(sRoot, 0) ;
}

std::string
NSCheckBoxControle::getRootSens()
{
	string sRootSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sRoot, &sRootSens) ;
  return sRootSens ;
}

//------------------------------------------------------------------------------
//                      classe  NSRadioButtonControle
//------------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSRadioButtonControle, TRadioButton)
	EV_WM_KILLFOCUS,
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
END_RESPONSE_TABLE;

NSRadioButtonControle::NSRadioButtonControle(TWindow *windowParent, NSContexte* pCtx, string sRacine, int resId)
                      :TRadioButton(windowParent, resId), NSRoot(pCtx)
{
  LostChangeReponse = NULL ;
  pContexte         = pCtx ;
  sRoot             = sRacine ;
  iInitialStatus    = BF_UNCHECKED ;
}

NSRadioButtonControle::NSRadioButtonControle(TWindow *windowParent, NSContexte* pCtx, string sRacine, int Id, const char far* title, int x, int y, int w, int h)
                      :TRadioButton(windowParent, Id, title, x, y, w, h), NSRoot(pCtx)
{
  LostChangeReponse = NULL ;
  pContexte         = pCtx ;
  sRoot             = sRacine ;
  iInitialStatus    = BF_UNCHECKED ;
}

void
NSRadioButtonControle::BNClicked()
{
  TRadioButton::BNClicked() ;
  if (LostChangeReponse != NULL)
    (*LostChangeReponse)() ;
}

NSRadioButtonControle::~NSRadioButtonControle()
{
  if (LostChangeReponse != NULL)
    delete(LostChangeReponse) ;
}

void
NSRadioButtonControle::EvKillFocus(HWND hWndGetFocus)
{
  TRadioButton::EvKillFocus(hWndGetFocus) ;
  if (NULL != LostChangeReponse)
    (*LostChangeReponse)() ;
}

void
NSRadioButtonControle::Show(int cmdShow)
{
  Show(cmdShow) ;
}

bool
NSRadioButtonControle::isVisible()
{
	return IsWindowVisible() ;
}

bool
NSRadioButtonControle::MoveWindow(int /*x*/, int /*y*/, int /*w*/, int /*h*/, bool /*repaint*/)
{
	return true ;
}

void
NSRadioButtonControle::createTree(NSPatPathoArray *pPPT, uint iVal)
{
	if ((iVal > 0) && (NULL != pPPT))
		pPPT->ajoutePatho(sRoot, 0) ;
}

std::string
NSRadioButtonControle::getRootSens()
{
	string sRootSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sRoot, &sRootSens) ;
  return sRootSens ;
}

