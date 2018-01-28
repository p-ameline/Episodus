#include "nsbb\nsutiDlg.h"
#include "nsbb\nscomboutil.h"
#include "nautilus\nsepicap.h"

DEFINE_RESPONSE_TABLE1(NSComboBox, TComboBox)
   EV_WM_KEYUP,
   EV_WM_KEYDOWN,
   EV_WM_GETDLGCODE,
   EV_WM_KILLFOCUS ,
END_RESPONSE_TABLE;

NSComboBox::NSComboBox(TWindow *parent, int resId, NSContexte *pCtx, VecteurString *pVecteurString)
           :TComboBox(parent, resId), NSRoot(pCtx)
{
	_pDlgParent = parent ;
  initCommonData() ;
  initLexiqCodes(pVecteurString) ;
}

NSComboBox::NSComboBox(TWindow *parent, int Id, NSContexte *pCtx, int x, int y, int w, int h, uint32 style, VecteurString *pVecteurString)
           :TComboBox(parent, Id, x, y, w, h, style, 0), NSRoot(pCtx)
{
	_pDlgParent = parent ;
  initCommonData() ;
  initLexiqCodes(pVecteurString) ;
}

NSComboBox::NSComboBox(TWindow *parent, int resId, NSContexte *pCtx, VectString *pVectString)
           :TComboBox(parent, resId), NSRoot(pCtx)
{
  _pDlgParent = parent ;
  initCommonData() ;
  initLexiqCodes(pVectString) ;
}

NSComboBox::NSComboBox(TWindow *parent, int Id, NSContexte *pCtx, int x, int y, int w, int h, uint32 style, VectString *pVectString)
           :TComboBox(parent, Id, x, y, w, h, style, 0), NSRoot(pCtx)
{
  _pDlgParent = parent ;
  initCommonData() ;
  initLexiqCodes(pVectString) ;
}

NSComboBox::NSComboBox(TWindow *parent, int resId, NSContexte *pCtx, char *input[], int iInputSize)
           :TComboBox(parent, resId), NSRoot(pCtx)
{
  _pDlgParent = parent ;
  initCommonData() ;
  initLexiqCodes(input, iInputSize) ;
}

NSComboBox::NSComboBox(TWindow *parent, int Id, NSContexte *pCtx, int x, int y, int w, int h, uint32 style, char *input[], int iInputSize)
           :TComboBox(parent, Id, x, y, w, h, style, 0), NSRoot(pCtx)
{
  _pDlgParent = parent ;
  initCommonData() ;
  initLexiqCodes(input, iInputSize) ;
}

void
NSComboBox::initCommonData()
{
	_sCode   = string("") ;
  _bWinStd = false ;
  _LostFocusResponse = (Functor*) 0 ;
}

void
NSComboBox::initLexiqCodes(VecteurString *pVecteurString)
{
	_pLexiqCodes = new VecteurString() ;

  if (((VecteurString*) NULL == pVecteurString) || (pVecteurString->empty()))
		return ;

	for (EquiItemIter iter = pVecteurString->begin() ; iter != pVecteurString->end() ; iter++)
		_pLexiqCodes->AddString(**iter) ;
}

void
NSComboBox::initLexiqCodes(VectString *pVectString)
{
	_pLexiqCodes = new VecteurString() ;

  if (((VectString*) NULL == pVectString) || (pVectString->empty()))
		return ;

	for (IterString iter = pVectString->begin() ; iter != pVectString->end() ; iter++)
		_pLexiqCodes->AddString(**iter) ;
}

// The reason why iInputSize is passed is that the usual way to evaluate
// the size of an array is to use sizeof(input) / sizeof(input[0])
// however, WHEN AN ARRAY IS PASSED AS A PARAMETER TO A FUNCTION,
// sizeof(input) returns the size of the pointer (i.e. 4) instead of the
// size of the array
void
NSComboBox::initLexiqCodes(char *input[], int iInputSize)
{
	_pLexiqCodes = new VecteurString() ;

  if ((NULL == input) || (0 == iInputSize))
		return ;

	for (int i = 0 ; i < iInputSize ; i++)
		_pLexiqCodes->AddString(std::string(input[i])) ;
}

void
NSComboBox::EvKillFocus(HWND hWndGetFocus)
{
	if (_LostFocusResponse)
  	(*_LostFocusResponse)() ;

	TComboBox::EvKillFocus(hWndGetFocus) ;
}

NSComboBox::~NSComboBox()
{
  _pLexiqCodes->vider() ;
  delete _pLexiqCodes ;
}

void
NSComboBox::SetupWindow()
{
  TComboBox::SetupWindow() ;
 	if (_pDlgParent)
  {
    if (pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
    	_bWinStd = true ;
  }

  // ---------------------------------------------------------------------------
  // la liste inclut dans pLexiqCodes correpond aux codes lexiques des libellés
  // à mettre dans le combobox
  // ---------------------------------------------------------------------------
  // il faut donc faire des addstring à partir des libellés et non des codes
  // lexique
  // ---------------------------------------------------------------------------
  if (false == _pLexiqCodes->empty())
  {
    for (EquiItemIter iter = _pLexiqCodes->begin() ; _pLexiqCodes->end() != iter ; iter++)
    {
      string sLabel ;
      string sLexiqCode = (*iter)->c_str() ;
      pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqCode, &sLabel) ;
      if (string("") != sLabel)
      	AddString(sLabel.c_str()) ;
    }
  }
}

std::string
NSComboBox::getSelCode()
{
  // récupétion de l'index de la sélection au sein du combobox
  int index = GetSelIndex() ;
  _sCode    = string("") ;

  // récupération du code lexique
  EquiItemIter  iter ;
  int           iIter ;
  if (false == _pLexiqCodes->empty())
  {
    for (iter = _pLexiqCodes->begin(), iIter = 0 ; (iter != _pLexiqCodes->end()) && (index != iIter) ; iter++, iIter++)
    ;
    if ((_pLexiqCodes->end() != iter) && (iIter == index))
    	_sCode = **iter ;
  }

  return _sCode ;
}

void
NSComboBox::setCode(std::string sValue)
{
  EquiItemIter  iter ;
  int           iIter ;

  if (false == _pLexiqCodes->empty())
  {
    for (iter = _pLexiqCodes->begin(), iIter = 0 ;
            (_pLexiqCodes->end() != iter) && ((**iter) != sValue) ;
                                                        iter++, iIter++)
    ;
    if ((_pLexiqCodes->end() != iter) && ((**iter) == sValue))
    {
    	SetSelIndex(iIter) ;
      _sCode = sValue ;
      return ;
    }
  }
  SetSelIndex(-1) ;
  _sCode = string("") ;
}


void
NSComboBox::EvKeyUp(uint key, uint repeatcount, uint flags)
{
	if (_bWinStd)
  {
  	TComboBox::EvKeyUp(key, repeatcount, flags) ;
    return ;
  }
  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(_pDlgParent, NSUtilDialog) ;
  if ((NSUtilDialog*) NULL == pNSUtilDialog)
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
    	TComboBox::EvKeyUp(key, repeatcount, flags) ;
   }
}


void
NSComboBox::AddLexiqueCode(char *temp)
{
  _pLexiqCodes->AddString(std::string(temp)) ;
}

void
NSComboBox::AddLexiqueCode(std::string& temp)
{
  _pLexiqCodes->AddString(temp) ;
}

void
NSComboBox::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (_bWinStd)
  {
  	TComboBox::EvKeyUp(key, repeatCount, flags) ;
    return ;
  }

  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(_pDlgParent, NSUtilDialog) ;
  if ((NSUtilDialog*) NULL == pNSUtilDialog)
    return ;

  switch(key)
  {
    case VK_TAB	:
    	 TComboBox::EvKeyUp(key, repeatCount, flags);
   //    pNSUtilDialog->ActiveControlSuivant(this);
        break;

    case VK_RETURN  :
    case VK_DOWN  :
        pNSUtilDialog->ActiveControlSuivant(this);
        break;
    default       :
        TComboBox::EvKeyUp(key, repeatCount, flags);
  }
}


//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSComboBox::EvGetDlgCode(MSG far* /* msg */)
{
  uint retVal = (uint)DefaultProcessing() ;

  if (false == _bWinStd)
    retVal |= DLGC_WANTALLKEYS ;

  return retVal ;
}

bool
NSComboBox::isVisible()
{
	return IsWindowVisible() ;
}


DEFINE_RESPONSE_TABLE1(NSComboBoxClassif, TComboBox)
   EV_WM_KEYUP,
   EV_WM_KEYDOWN,
   EV_WM_GETDLGCODE,
   EV_WM_KILLFOCUS ,
END_RESPONSE_TABLE;

NSComboBoxClassif::NSComboBoxClassif(TWindow *parent, int resId, NSContexte *pCtx, string sClassif)
                  :TComboBox(parent, resId), NSRoot(pCtx)
{
	sClassification = sClassif ;

  sCode       = "" ;
  bWinStd     = false ;
  pDlgParent  = parent ;

  bWinStd     = false ;
 	LostFocusResponse = (Functor*) 0 ;
}

void
NSComboBoxClassif::EvKillFocus(HWND hWndGetFocus)
{
	if (LostFocusResponse)
  	(*LostFocusResponse)() ;
	TComboBox::EvKillFocus(hWndGetFocus) ;
}

NSComboBoxClassif::~NSComboBoxClassif()
{
}

void
NSComboBoxClassif::SetupWindow()
{
  TComboBox::SetupWindow() ;
 	if (pDlgParent)
  {
    if (pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
    	bWinStd = true ;
  }

  searchList(sClassification) ;
	if (false == aClassifList.empty())
  	for (NSEpiClassifInfoIter iter = aClassifList.begin(); iter != aClassifList.end() ; iter++)
    	AddString((*iter)->getLabel().c_str()) ;
}

std::string
NSComboBoxClassif::getSelCode()
{
	string sCode = string("") ;

	if (aClassifList.empty())
  	return sCode ;

	int iIdx = GetSelIndex() ;
  char far* sStringSel = new char[CLASSIF_LIBELLE_LEN + 1] ;
  GetString(sStringSel, iIdx) ;

  NSEpiClassifInfoIter iter = aClassifList.begin() ;
  for ( ; (aClassifList.end() != iter) &&
  	((*iter)->getLabel() != string(sStringSel)) ; iter++) ;

  if (aClassifList.end() != iter)
  	sCode = (*iter)->getCode() ;
  delete[]  sStringSel ;

  return sCode ;
}

void
NSComboBoxClassif::setCode(std::string sValue)
{
	if (aClassifList.empty())
  	return ;

  // First step : find the libelle for this code
  //
  NSEpiClassifInfoIter iter = aClassifList.begin() ;
  for ( ; (aClassifList.end() != iter) &&
  	((*iter)->getCode() != sValue) ; iter++) ;

  if (aClassifList.end() == iter)
  	return ;

  string sLib = (*iter)->getLabel() ;

  // Second step : select the proper lib
  //
  char far* szStringSel = new char[strlen(sLib.c_str()) + 1] ;
  strcpy(szStringSel, sLib.c_str()) ;

  int iIndex = FindString(szStringSel, 0 /*int indexStart*/) ;

  if (iIndex >= 0)
  	SetSelIndex(iIndex) ;
	else
  	SetSelIndex(-1) ;

  return ;
}

void
NSComboBoxClassif::EvKeyUp(uint key, uint repeatcount, uint flags)
{
	if (bWinStd)
  {
  	TComboBox::EvKeyUp(key, repeatcount, flags) ;
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
    	TComboBox::EvKeyUp(key, repeatcount, flags) ;
   }
}

void
NSComboBoxClassif::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (bWinStd)
  {
  	TComboBox::EvKeyUp(key, repeatCount, flags) ;
    return ;
  }

  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(pDlgParent, NSUtilDialog);
  if (!pNSUtilDialog)
    return;
  switch(key)
  {
    case VK_TAB	:
    	 TComboBox::EvKeyUp(key, repeatCount, flags);
   //    pNSUtilDialog->ActiveControlSuivant(this);
        break;

    case VK_RETURN  :
    case VK_DOWN  :
        pNSUtilDialog->ActiveControlSuivant(this);
        break;
    default       :
        TComboBox::EvKeyUp(key, repeatCount, flags);
  }
}


//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
NSComboBoxClassif::EvGetDlgCode(MSG far* /* msg */)
{
  uint retVal = (uint)DefaultProcessing();
  if (!bWinStd)
    retVal |= DLGC_WANTALLKEYS;
  return retVal;
}

bool
NSComboBoxClassif::isVisible()
{
	return IsWindowVisible() ;
}

void
NSComboBoxClassif::searchList(string sClassif)
{
try
{
	NSEpiClassif Classif(pContexte->getSuperviseur()) ;

	Classif.lastError = Classif.open();
	if (Classif.lastError != DBIERR_NONE)
	{
		erreur("Ouverture du fichier Classif impossible", standardError, Classif.lastError, GetHandle()) ;
		return ;
	}

	CURProps curProps ;
  /* DBIResult lastError = */ DbiGetCursorProps(Classif.PrendCurseur(), curProps) ;
  Byte* pIndexRec = new Byte[curProps.iRecBufSize] ;
  memset(pIndexRec, 0, curProps.iRecBufSize) ;
  DbiPutField(Classif.PrendCurseur(), CLASSIF_CLASSIF_FIELD, pIndexRec, (Byte*) sClassification.c_str()) ;
  DbiPutField(Classif.PrendCurseur(), CLASSIF_CODE_FIELD,    pIndexRec, (Byte*) "") ;

  Classif.lastError = Classif.chercheClefComposite("",
                                                   0,
                                                   keySEARCHGEQ,
                                                   dbiWRITELOCK,
                                                   pIndexRec) ;
  delete[] pIndexRec ;

  if ((Classif.lastError != DBIERR_NONE) && (Classif.lastError != DBIERR_BOF))
  {
  	erreur("Erreur à la recherche dans la base Classif.", standardError, Classif.lastError, GetHandle()) ;
    Classif.close() ;
    return ;
  }

  if (Classif.lastError == DBIERR_BOF)
  {
  	erreur("Aucune fiche ne correspond.", standardError, Classif.lastError, GetHandle()) ;
    Classif.close() ;
    return ;
  }

  Classif.lastError = Classif.getRecord() ;
  if (Classif.lastError != DBIERR_NONE)
  {
  	erreur("Erreur à la lecture d'une fiche Classif.", standardError, Classif.lastError, GetHandle());
    Classif.close() ;
    return ;
  }

  while ((Classif.lastError != DBIERR_EOF) && (Classif.getClassification() == sClassification))
  {
    aClassifList.push_back(new NSEpiClassifInfo(&Classif)) ;

    Classif.lastError = Classif.suivant(dbiWRITELOCK) ;
    if ((Classif.lastError != DBIERR_NONE) && (Classif.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche Classif.", standardError, Classif.lastError, GetHandle()) ;
      Classif.close() ;
      return ;
    }
    if (Classif.lastError == DBIERR_NONE)
    {
    	Classif.lastError = Classif.getRecord();
      if (Classif.lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la lecture d'une fiche Classif.", standardError, Classif.lastError, GetHandle()) ;
        Classif.close() ;
        return ;
			}
		}
	}  //while

	Classif.close() ;
	return ;
}
catch (...)
{
	erreur("Exception Exception realisation table Classif", standardError, 0) ;
}
}



DEFINE_RESPONSE_TABLE1(EventCombo, TComboBox)
   EV_WM_KEYUP,
   EV_WM_KEYDOWN,
   EV_WM_GETDLGCODE,
   EV_WM_KILLFOCUS ,
END_RESPONSE_TABLE;

void
EventCombo::EvKillFocus(HWND /* hWndGetFocus */)
{
	if (_LostFocus != NULL)
  	(*_LostFocus)() ;
  DefaultProcessing() ;
  //TComboBox::EvKillFocus(hWndGetFocus);
}

EventCombo::~EventCombo()
{
  if (_LostFocus != NULL)
    delete (_LostFocus);
}
