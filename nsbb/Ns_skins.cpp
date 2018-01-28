
#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"

#include "nsbb\ns_skins.h"

// -------------------------------------------------------------------------
// -------------------------------  nsSkin  --------------------------------
// -------------------------------------------------------------------------

nsSkin::nsSkin(string sPath)
{
  initToNull() ;

  _sSkinPath = sPath ;
}

nsSkin::nsSkin(const nsSkin& rv)
{
  initToNull() ;

  _sName       = rv._sName ;
  _sSkinPath   = rv._sSkinPath ;
  setBackColor(rv._pBackColor) ;  setBackBmp(rv._pBackBmp) ;  _sBmp        = rv._sBmp ;  _iBmpDraw    = rv._iBmpDraw ;  setIco(rv._iCon);  setLogFont(rv._pFontDesc) ;
  setFontColor(rv._pFontColor) ;
  setFontBackColor(rv._pFontBackColor) ;

  _boxPos = rv._boxPos ;
}

nsSkin::~nsSkin()
{
  if (_pBackColor)
  	delete _pBackColor ;
  if (_pFontDesc)
  	delete _pFontDesc ;
  if (_pFontColor)
  	delete _pFontColor ;
  if (_pFontBackColor)
  	delete _pFontBackColor ;
  if (_pBackBmp)
  	delete _pBackBmp ;
  if (_iCon)
    delete _iCon ;
}

void
nsSkin::initToNull()
{
  _sSkinPath      = string("") ;

  _pBackColor     = (ClassLib::TColor*) 0 ;
  _pBackBmp       = (OWL::TDib*) 0 ;
  _sName          = string("") ;
  _sBmp           = string("") ;  _iBmpDraw       = DrawUndefined ;

  _pFontDesc      = (LOGFONT*) 0 ;  _pFontColor     = (ClassLib::TColor*) 0 ;
  _pFontBackColor = (ClassLib::TColor*) 0 ;
  _iCon           = (OWL::TIcon*) 0 ;
}

void
nsSkin::setBackBmp(string sB)
{
try
{
	if (_pBackBmp)
  {
  	delete _pBackBmp ;
    _pBackBmp = (OWL::TDib*) 0 ;
  }

	_sBmp = sB ;

	if (string("") == sB)
  	return ;

	string sFileName = _sSkinPath + sB ;
	//
	// On vérifie que le fichier existe bien
	// Checking that file really exists
	//
	WIN32_FIND_DATA stRecherche ;
	HANDLE			hFichierTrouve ;
	hFichierTrouve = FindFirstFile(sFileName.c_str(), &stRecherche) ;
	//	if (hFichierTrouve == INVALID_HANDLE_VALUE)
		return ;

	_pBackBmp = new TDib(sFileName.c_str()) ;
}
catch (...){
	erreur("Exception nsSkin::setBackBmp.", standardError, 0) ;
}
}

void
nsSkin::setBackBmp(OWL::TDib* pBackBmp)
{
	if (_pBackBmp)
  {
  	delete _pBackBmp ;
    _pBackBmp = (OWL::TDib*) 0 ;
  }

  if (pBackBmp)
	  _pBackBmp = new OWL::TDib(*pBackBmp) ;
}

void
nsSkin::setBackIco(string sB)
{
try
{
	if (_iCon)
  {
  	delete _iCon ;
    _iCon = (OWL::TIcon*) 0 ;
  }

	_sBmp = sB ;

	if (string("") == sB)
  	return ;

  string sFileName = _sSkinPath + sB ;
  //
  // On vérifie que le fichier existe bien
  // Checking that file really exists
  //
	WIN32_FIND_DATA stRecherche ;
  HANDLE			    hFichierTrouve ;
	hFichierTrouve = FindFirstFile(sFileName.c_str(), &stRecherche) ;
	//  if (hFichierTrouve == INVALID_HANDLE_VALUE)
  	return ;

	_iCon = new OWL::TIcon(0, sFileName.c_str(), 0) ;
}
catch (...){
	erreur("Exception nsSkin::setBackIco.", standardError, 0) ;
}
}

void
nsSkin::setBackColor(ClassLib::TColor* color)
{
try
{
	if (_pBackColor)
	{
  	delete _pBackColor ;
    _pBackColor = (ClassLib::TColor*) 0 ;
  }

  if ((ClassLib::TColor*) NULL == color)
  	return ;

	_pBackColor = new ClassLib::TColor(COLORREF(*color)) ;
}
catch (...){
	erreur("Exception nsSkin::setBackColor.", standardError, 0) ;
}
}

void
nsSkin::setLogFont(LOGFONT* lFT)
{
try
{
	if (_pFontDesc)
	{
		delete _pFontDesc ;
    _pFontDesc = (LOGFONT*) 0 ;
	}

	if ((LOGFONT*) NULL == lFT)
		return ;

  _pFontDesc = new LOGFONT ;
  _pFontDesc->lfHeight         = lFT->lfHeight ;
  _pFontDesc->lfWidth          = lFT->lfWidth ;  _pFontDesc->lfEscapement     = lFT->lfEscapement ;
  _pFontDesc->lfOrientation    = lFT->lfOrientation ;
  _pFontDesc->lfWeight         = lFT->lfWeight ;
  _pFontDesc->lfItalic         = lFT->lfItalic ;
  _pFontDesc->lfUnderline      = lFT->lfUnderline ;
  _pFontDesc->lfStrikeOut      = lFT->lfStrikeOut ;
  _pFontDesc->lfCharSet        = lFT->lfCharSet ;
  _pFontDesc->lfOutPrecision   = lFT->lfOutPrecision ;
  _pFontDesc->lfClipPrecision  = lFT->lfClipPrecision ;
  _pFontDesc->lfQuality        = lFT->lfQuality ;
  _pFontDesc->lfPitchAndFamily = lFT->lfPitchAndFamily ;
  strcpy(_pFontDesc->lfFaceName, lFT->lfFaceName) ;
}
catch (...){
	erreur("Exception nsSkin::setLogFont.", standardError, 0) ;
}
}

void
nsSkin::setFontColor(ClassLib::TColor* color)
{
try
{
	if (_pFontColor)
	{
  	delete _pFontColor ;
    _pFontColor = (ClassLib::TColor*) 0 ;
  }
  if ((ClassLib::TColor*) NULL == color)
  	return ;

	_pFontColor = new ClassLib::TColor(COLORREF(*color)) ;
}
catch (...){
	erreur("Exception nsSkin::setFontColor.", standardError, 0) ;
}
}

void
nsSkin::setFontBackColor(ClassLib::TColor* color)
{
try
{
	if (_pFontBackColor)
  {
  	delete _pFontBackColor ;
    _pFontBackColor = (ClassLib::TColor*) 0 ;
  }
  if ((ClassLib::TColor*) NULL == color)
  	return ;

	_pFontBackColor = new ClassLib::TColor(COLORREF(*color)) ;
}
catch (...){
	erreur("Exception nsSkin::setFontBackColor.", standardError, 0) ;
}
}

nsSkin&
nsSkin::operator=(const nsSkin& src)
{
try
{
	if (this == &src)
		return *this ;

  _sName       = src._sName ;
  _sSkinPath   = src._sSkinPath ;
  setBackColor(src._pBackColor) ;  setBackBmp(src._pBackBmp) ;  _sBmp        = src._sBmp ;  _iBmpDraw    = src._iBmpDraw ;  setIco(src._iCon) ;  setLogFont(src._pFontDesc) ;
  setFontColor(src._pFontColor) ;
  setFontBackColor(src._pFontBackColor) ;

  _boxPos = src._boxPos ;

	return *this ;
}
catch (...){
	erreur("Exception nsSkin (= operator).", standardError, 0);
  return *this ;
}
}

// -------------------------------------------------------------------------
// ------------------------------  skinArray  ------------------------------
// -------------------------------------------------------------------------

skinArray::skinArray(NSSuper* pSuper)
          :skinsArray(), NSSuperRoot(pSuper)
{
	_sTitle   = string("") ;
	_sAuthor  = string("") ;
	_sComment = string("") ;

	_sAppName = string("") ;
}

skinArray::skinArray(const skinArray& rv)
          :skinsArray(), NSSuperRoot(rv._pSuper){
try
{
  _sTitle   = rv._sTitle ;
	_sAuthor  = rv._sAuthor ;
	_sComment = rv._sComment ;

	_sAppName = rv._sAppName ;

	if (false == rv.empty())
		for (skin_constIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new nsSkin(**i)) ;
}
catch (...)
{
	erreur("Exception skinArray copy ctor.", standardError, 0) ;
}
}

void
skinArray::vider(){
	if (empty())
		return ;

	for (skin_iter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

skinArray::~skinArray()
{
	vider() ;
}

void
skinArray::init(string sPath)
{
try
{
	ifstream    inFile ;

	string sLang = "" ;
	string sFichierTempo = sPath + string("skin.ini") ;

	inFile.open(sFichierTempo.c_str()) ;
	if (!inFile)
		return ;

	string              _sTitre     = string("") ;

	string              _sBmp       = string("") ;
	nsSkin::TYPEBMPDRAW _iDraw      = nsSkin::DrawUndefined ;
	TColor*             _pBackColor = (TColor*) 0 ;

	LOGFONT*            _pFontDesc  = (LOGFONT*) 0 ;
	TColor*             _pFontColor = (TColor*) 0 ;
	TColor*             _pFontBackColor = (TColor*) 0 ;
	string	            _Ico        = string("") ;

  nsBoxPosition boxPos ;

	// int     inumLigne = 0 ;

	while (!inFile.eof())
	{
    string _sLine ;

		getline(inFile, _sLine) ;
    strip(_sLine, stripBoth) ;

    if (string("") != _sLine)
    {
    	if ('[' == _sLine[0])
      {
      	if (string("") != _sTitre)
        {
        	nsSkin* pSkin = new nsSkin(sPath) ;

          pSkin->setName(_sTitre) ;

          if (_pBackColor)
          {
          	pSkin->setBackColor(_pBackColor) ;
            delete _pBackColor ;
            _pBackColor = (TColor*) 0 ;
          }

          pSkin->setBackBmp(_sBmp) ;
          if (nsSkin::DrawUndefined != _iDraw)
          {
          	pSkin->setBackDraw(_iDraw) ;
          	_iDraw = nsSkin::DrawUndefined ;
          }

          if (_Ico != "")
    				pSkin->setBackIco(_Ico) ;

          if (_pFontColor)
          {
          	pSkin->setFontColor(_pFontColor) ;
          	delete _pFontColor ;
          	_pFontColor = (TColor*) 0 ;
          }
          if (_pFontBackColor)
          {
          	pSkin->setFontBackColor(_pFontBackColor) ;
            delete _pFontBackColor ;
            _pFontBackColor = (TColor*) 0 ;
          }
          if (_pFontDesc)
          {
          	pSkin->setLogFont(_pFontDesc) ;
            delete _pFontDesc ;
            _pFontDesc = (LOGFONT*) 0 ;
          }

          *(pSkin->getBoxPosition()) = boxPos ;
          boxPos.initDefault() ;

          push_back(pSkin) ;

          _sBmp = string("") ;
          _Ico  = string("") ;
        }

        _sTitre = string("") ;
        size_t iPos = _sLine.find(']') ;
        if (NPOS != iPos)
        	_sTitre = string(_sLine, 1, iPos-1) ;
      }
      else if ('/' != _sLine[0])
      {
      	size_t iPos = _sLine.find('=') ;
        if ((NPOS != iPos) && (strlen(_sLine.c_str())-1) != iPos)
        {
        	string _sType  = string(_sLine, 0, iPos) ;
          string _sLibel = string(_sLine, iPos+1, strlen(_sLine.c_str())-iPos-1) ;
          strip(_sType, stripBoth) ;
          strip(_sLibel, stripBoth) ;
          _sType = pseumaj(_sType) ;

          //
          // Bitmap
          //
          if      (string("BMP") == _sType)
          	_sBmp = _sLibel ;

          //
          // ICO
          //
          else if (string("ICO") == _sType)
          	_Ico = _sLibel ;

          //
          // Draw type
          //
          else if (string("DRAW") == _sType)
          {
            _sLibel = pseumaj(_sLibel) ;

            if      (string("CENTER")  == _sLibel)
            	_iDraw = nsSkin::DrawCenter ;
            else if (string("STRETCH") == _sLibel)
            	_iDraw = nsSkin::DrawStretch ;
            else if (string("TILE")    == _sLibel)
            	_iDraw = nsSkin::DrawTile ;
            else if (string("MAX")     == _sLibel)
            	_iDraw = nsSkin::DrawMax ;
            else if (string("FORM")    == _sLibel)
              _iDraw = nsSkin::DrawAsForm ;
          }
          //
          // Couleur
          //
          else if ((string("TEXTCOLOR")     == _sType) ||
                   (string("BACKCOLOR")     == _sType) ||
                   (string("TEXTBACKCOLOR") == _sType))
          {
            int iNumCol = 0 ;

            int iRed    = 0 ;
            int iGreen  = 0 ;
            int iBlue   = 0 ;

            size_t iLibPos = _sLibel.find(',') ;
            while (NPOS != iLibPos)
            {
            	string sColor = string(_sLibel, 0, iLibPos) ;
              _sLibel = string(_sLibel, iLibPos+1, strlen(_sLibel.c_str())-iLibPos-1) ;

              if      (0 == iNumCol)
              	iRed    = atoi(sColor.c_str()) ;
              else if (1 == iNumCol)
              	iGreen  = atoi(sColor.c_str()) ;
              else if (2 == iNumCol)
              	iBlue   = atoi(sColor.c_str()) ;

              iNumCol++ ;

              iLibPos = _sLibel.find(',') ;
            }

            string sColor = _sLibel ;

            if      (0 == iNumCol)
            	iRed    = atoi(sColor.c_str()) ;
            else if (1 == iNumCol)
            	iGreen  = atoi(sColor.c_str()) ;
            else if (2 == iNumCol)
            	iBlue   = atoi(sColor.c_str()) ;

            if      (string("TEXTCOLOR") == _sType)
            	_pFontColor = new TColor(iRed, iGreen, iBlue) ;
            else if (string("BACKCOLOR") == _sType)
            	_pBackColor = new TColor(iRed, iGreen, iBlue) ;
            else if (string("TEXTBACKCOLOR") == _sType)
            	_pFontBackColor = new TColor(iRed, iGreen, iBlue) ;
          }
          //
          // Position
          //
          else if (string("POS") == string(_sType, 0, 3))
            boxPos.initValue(_sType, _sLibel) ;

          else if (string("NAME") == _sType)
          {
          	if (string("Skin") == _sTitre)
            	_sTitle = _sLibel ;
          }
          else if (string("AUTHOR") == _sType)
          {
          	if (string("Skin") == _sTitre)
            	_sAuthor = _sLibel ;
          }
          else if (string("COMMENT") == _sType)
          {
          	if (string("Skin") == _sTitre)
            	_sComment = _sLibel ;
          }
          else if (string("APPNAME") == _sType)
          {
          	if (string("Skin") == _sTitre)
            	_sAppName = _sLibel ;
          }
        }
      }
    }
  }

  if (string("") != _sTitre)
  {
    nsSkin* pSkin = new nsSkin(sPath) ;

    pSkin->setName(_sTitre) ;

    if (_pBackColor)
    {
    	pSkin->setBackColor(_pBackColor) ;
      delete _pBackColor ;
    }

    pSkin->setBackBmp(_sBmp) ;
    if (nsSkin::DrawUndefined != _iDraw)
    	pSkin->setBackDraw(_iDraw) ;

    if (_pFontColor)
    {
    	pSkin->setFontColor(_pFontColor) ;
      delete _pFontColor ;
    }
    if (_pFontBackColor)
    {
    	pSkin->setFontBackColor(_pFontBackColor) ;
      delete _pFontBackColor ;
    }
    if (_pFontDesc)
    {
    	pSkin->setLogFont(_pFontDesc) ;
      delete _pFontDesc ;
    }

    if (string("") != _Ico)
    	pSkin->setBackIco(_Ico) ;

    *(pSkin->getBoxPosition()) = boxPos ;

    push_back(pSkin) ;
  }

  inFile.close() ;
  return ;
}
catch (...)
{
	erreur("Exception skinArray::init", standardError, 0) ;
}
}

// Get skin for a given concept
//
nsSkin*
skinArray::donneSkinForLexique(string sCode)
{
  if (string("") == sCode)
    return (nsSkin*) 0 ;

  string sCodeSens ;
  NSDico::donneCodeSens(&sCode, &sCodeSens) ;

  return donneSkin(string("icon_") + sCodeSens) ;
}

nsSkin*
skinArray::donneSkin(string sName)
{
	if (empty())
		return (nsSkin*) 0 ;

	for (skin_iter i = begin() ; end() != i ; i++)
  	if ((*i)->getName() == sName)
    	return *i ;

	return (nsSkin*) 0 ;
}

skinArray&
skinArray::operator=(const skinArray& src)
{
try
{
	if (this == &src)
		return *this ;

  _sTitle   = src._sTitle ;
	_sAuthor  = src._sAuthor ;
	_sComment = src._sComment ;

	_sAppName = src._sAppName ;

	vider();

	if (false == src.empty())
		for (skin_constIter i = src.begin() ; src.end() != i ; i++)
    	push_back(new nsSkin(**i)) ;

	return *this ;
}
catch (...)
{
	erreur("Exception skinArray::operator=", standardError, 0) ;
	return *this;
}
}

//
// --------------------- nsBoxPosition ---------------------
//

nsBoxPosition::nsBoxPosition()
{
  initDefault() ;
}

nsBoxPosition::nsBoxPosition(const nsBoxPosition& rv)
{
  initDefault() ;
  initFromBP(&rv) ;
}

nsBoxPosition::~nsBoxPosition()
{
}

nsBoxPosition&
nsBoxPosition::operator=(const nsBoxPosition& src)
{
  if (this == &src)
    return *this ;

  initDefault() ;
  initFromBP(&src) ;

  return *this ;
}

void
nsBoxPosition::initDefault()
{
  sGlobalVPos     = string("") ;
  sGlobalVPosRef  = string("") ;

  sGlobalHPos     = string("") ;
  sGlobalHPosRef  = string("") ;

  sTopPosRef      = string("") ;
  sTopUnit        = string("") ;
  iTopValue       = 0 ;

  sRightPosRef    = string("") ;
  sRightUnit      = string("") ;
  iRightValue     = 0 ;

  sBottomPosRef   = string("") ;
  sBottomUnit     = string("") ;
  iBottomValue    = 0 ;

  sLeftPosRef     = string("") ;
  sLeftUnit       = string("") ;
  iLeftValue      = 0 ;

  sHeightUnit     = string("") ;
  iHeightValue    = 0 ;
  sMinHeightUnit  = string("") ;
  iMinHeightValue = 0 ;

  sWidthUnit      = string("") ;
  iWidthValue     = 0 ;
  sMinWidthUnit   = string("") ;
  iMinWidthValue  = 0 ;
}

void
nsBoxPosition::initFromBP(const nsBoxPosition* pBP)
{
  if ((nsBoxPosition*) NULL == pBP)
    return ;

  sGlobalVPos     = pBP->sGlobalVPos ;
  sGlobalVPosRef  = pBP->sGlobalVPosRef ;

  sGlobalHPos     = pBP->sGlobalHPos ;
  sGlobalHPosRef  = pBP->sGlobalHPosRef ;

  sTopPosRef      = pBP->sTopPosRef ;
  sTopUnit        = pBP->sTopUnit ;
  iTopValue       = pBP->iTopValue ;

  sRightPosRef    = pBP->sRightPosRef ;
  sRightUnit      = pBP->sRightUnit ;
  iRightValue     = pBP->iRightValue ;

  sBottomPosRef   = pBP->sBottomPosRef ;
  sBottomUnit     = pBP->sBottomUnit ;
  iBottomValue    = pBP->iBottomValue ;

  sLeftPosRef     = pBP->sLeftPosRef ;
  sLeftUnit       = pBP->sLeftUnit ;
  iLeftValue      = pBP->iLeftValue ;

  sHeightUnit     = pBP->sHeightUnit ;
  iHeightValue    = pBP->iHeightValue ;
  sMinHeightUnit  = pBP->sMinHeightUnit ;
  iMinHeightValue = pBP->iMinHeightValue ;

  sWidthUnit      = pBP->sWidthUnit ;
  iWidthValue     = pBP->iWidthValue ;
  sMinWidthUnit   = pBP->sMinWidthUnit ;
  iMinWidthValue  = pBP->iMinWidthValue ;
}

string
nsBoxPosition::initValue(string sAttribute, string sValue)
{
  if      (string("POSVERTICAL") == sAttribute)
    return setVPos(sValue) ;
  else if (string("POSHORIZONTAL") == sAttribute)
    return setHPos(sValue) ;
  else if (string("POSTOP") == sAttribute)
    return setTop(sValue) ;
  else if (string("POSRIGHT") == sAttribute)
    return setRight(sValue) ;
  else if (string("POSBOTTOM") == sAttribute)
    return setBottom(sValue) ;
  else if (string("POSLEFT") == sAttribute)
    return setLeft(sValue) ;
  else if (string("POSHEIGHT") == sAttribute)
    return setHeight(sValue) ;
  else if (string("POSWIDTH") == sAttribute)
    return setWidth(sValue) ;
  else
    return string("BadAttribute") ;
}

string
nsBoxPosition::setVPos(string sValue)
{
  return setPosAndRef(sValue, string("CENTER:TOP:BOTTOM"), &sGlobalVPos, &sGlobalVPosRef) ;
}

string
nsBoxPosition::setHPos(string sValue)
{
  return setPosAndRef(sValue, string("CENTER:LEFT:RIGHT"), &sGlobalHPos, &sGlobalHPosRef) ;
}

string
nsBoxPosition::setPosAndRef(string sValue, string sPosLegalValues, string* psPos, string* psRef)
{
  *psPos = string("") ;
  *psRef = string("") ;

  // Model: "Reference,value" or just "value"
  //
  if (string("") == sValue)
    return string("") ;

  strip(sValue, stripBoth) ;

  string sPos ;
  string sRef ;

  size_t iLibPos = sValue.find(',') ;
  if (NPOS != iLibPos)
  {
    sRef = string(sValue, 0, iLibPos) ;
    sPos = string(sValue, iLibPos+1, strlen(sValue.c_str()) - iLibPos - 1) ;

    strip(sRef, stripBoth) ;
    strip(sPos, stripBoth) ;

    sRef = pseumaj(sRef) ;
    sPos = pseumaj(sPos) ;

    // Check that the order is not wrong
    //
    size_t iPosPos = sPosLegalValues.find(sRef) ;
    if (NPOS != iPosPos)
    {
      string sSwitch = sRef ;
      sRef = sPos ;
      sPos = sSwitch ;
    }
  }
  else
  {
    sPos = pseumaj(sValue) ;
    sRef = string("WINDOW") ;
  }

  size_t iPosPos = sPosLegalValues.find(sPos) ;
  if (NPOS == iPosPos)
    return string("BadPosValue") ;

  *psPos = sPos ;
  *psRef = sRef ;

  return string("") ;
}

string
nsBoxPosition::setTop(string sValue)
{
  return setNumericPosAndRef(sValue, &sTopPosRef, &iTopValue, &sTopUnit) ;
}

string
nsBoxPosition::setRight(string sValue)
{
  return setNumericPosAndRef(sValue, &sRightPosRef, &iRightValue, &sRightUnit) ;
}

string
nsBoxPosition::setBottom(string sValue)
{
  return setNumericPosAndRef(sValue, &sBottomPosRef, &iBottomValue, &sBottomUnit) ;
}

string
nsBoxPosition::setLeft(string sValue)
{
  return setNumericPosAndRef(sValue, &sLeftPosRef, &iLeftValue, &sLeftUnit) ;
}

string
nsBoxPosition::setNumericPosAndRef(string sValue, string* psRef, int* piNum, string* psUnit)
{
  *psUnit = string("") ;
  *psRef  = string("") ;
  *piNum  = 0 ;

  // Model: "Reference,value" or just "value"
  //
  if (string("") == sValue)
    return string("") ;

  strip(sValue, stripBoth) ;

  string sRef ;
  string sNum ;

  size_t iLibPos = sValue.find(',') ;
  if (NPOS != iLibPos)
  {
    sRef = string(sValue, 0, iLibPos) ;
    sNum = string(sValue, iLibPos+1, strlen(sValue.c_str()) - iLibPos - 1) ;

    strip(sRef, stripBoth) ;
    strip(sNum, stripBoth) ;

    sRef = pseumaj(sRef) ;
    sNum = pseumaj(sNum) ;

    // Check that the order is not wrong
    //
    if (isNumAndUnit(sRef))
    {
      string sSwitch = sRef ;
      sRef = sNum ;
      sNum = sSwitch ;
    }
  }
  else
  {
    sNum = pseumaj(sValue) ;
    sRef = string("PARENT") ;
  }

  if (false == isNumAndUnit(sNum))
    return string("BadNumValue") ;

  *psRef = sRef ;

  return setNumericPos(sNum, piNum, psUnit) ;
}

string
nsBoxPosition::setHeight(string sValue)
{
  return setNumericPos(sValue, &iHeightValue, &sHeightUnit) ;
}

string
nsBoxPosition::setWidth(string sValue)
{
  return setNumericPos(sValue, &iWidthValue, &sWidthUnit) ;
}

string
nsBoxPosition::setNumericPos(string sValue, int* piNum, string* psUnit)
{
  *psUnit = string("") ;
  *piNum  = 0 ;

  // Model: "Reference,value" or just "value"
  //
  if (string("") == sValue)
    return string("") ;

  if (string("AUTO") == sValue)
  {
    *psUnit = string("AUTO") ;
    return string("") ;
  }

  strip(sValue, stripBoth) ;

  if (false == isNumAndUnit(sValue))
    return string("BadNumValue") ;

  string sNum ;
  string sUnit ;

  size_t iLibPos = sValue.find('%') ;
  if (NPOS != iLibPos)
  {
    sNum  = string(sValue, 0, iLibPos) ;
    sUnit = string(sValue, iLibPos, strlen(sValue.c_str()) - iLibPos) ;

    strip(sUnit, stripBoth) ;
    strip(sNum, stripBoth) ;

    sUnit = pseumaj(sUnit) ;
    sNum  = pseumaj(sNum) ;
  }
  else
  {
    sNum  = sValue ;
    sUnit = string("PIX") ;
  }

  if (strlen(sNum.c_str()) > 32)
    return string("BadNumValue") ;

  for (size_t i = 0 ; i < strlen(sNum.c_str()); i++)
    if (!isdigit(sNum[i]))
      return string("BadNumValue") ;

  *psUnit = sUnit ;
  *piNum  = atoi(sNum.c_str()) ;

  return string("") ;
}

bool
nsBoxPosition::isNumAndUnit(string sValue)
{
  if (string("") == sValue)
    return false ;

  if (string("AUTO") == sValue)
    return true ;

  string sNum ;

  size_t iLibPos = sValue.find('%') ;
  if (NPOS != iLibPos)
    sNum = string(sValue, 0, iLibPos) ;
  else
    sNum = sValue ;

  if (strlen(sNum.c_str()) > 32)
    return false ;

  for (size_t i = 0 ; i < strlen(sNum.c_str()); i++)
    if (!isdigit(sNum[i]))
      return false ;

  return true ;
}

