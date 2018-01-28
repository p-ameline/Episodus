// --------------------------------------------------------------------------// NSCOOPCHAR.CPP		Coop Charts scores over time
// --------------------------------------------------------------------------
// PA October 2010
// --------------------------------------------------------------------------

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsbb\nspatpat.h"
#include "nsepisod\nsCoopChart.h"

long NSCoopCardButton::lObjectCount = 0 ;
long NSCoopCard::lObjectCount = 0 ;
long NSCoopLine::lObjectCount = 0 ;
long NSQualityOfLifeInformation::lObjectCount = 0 ;
long NSQualityOfLifeChart::lObjectCount = 0 ;
long NSCoopChart::lObjectCount = 0 ;

LINECOLOR getColorFromChar(char cColor)
{
  switch(cColor)
  {
    case 'B' : return colorBlue ;
    case 'G' : return colorGreen ;
    case 'Y' : return colorYellow ;
    case 'O' : return colorOrange ;
    case 'R' : return colorRed ;
  }
  return colorNone ;
}

char getCharFromColor(LINECOLOR iColor)
{
  switch(iColor)
  {
    case colorBlue   : return 'B' ;
    case colorGreen  : return 'G' ;
    case colorYellow : return 'Y' ;
    case colorOrange : return 'O' ;
    case colorRed    : return 'R' ;
  }
  return ' ' ;
}

//
// sColorIntervals is in the form 0-1:B|1-2:G|2-3:Y|3-4:O|4-5:R
//                 meaning Blue between 0 included and 1 excluded, etc
//
LINECOLOR getColorFromScore(int iScore, string sColorIntervals)
{
  if (string("") == sColorIntervals)
    return colorNone ;

  ClasseStringVector aColorBlocks ;
  DecomposeChaine(&sColorIntervals, &aColorBlocks, string("|")) ;

  for (iterClassString it = aColorBlocks.begin() ; aColorBlocks.end() != it ; it++)
  {
    string sColorBlock = (*it)->getItem() ;

    string sInterval = string("") ;
    string sColor    = string("") ;
    splitString(sColorBlock, &sInterval, &sColor, ':') ;

    string sMinValue = string("") ;
    string sMaxValue = string("") ;
    splitString(sInterval, &sMinValue, &sMaxValue, '-') ;

    int iMinValue = StringToInt(sMinValue) ;
    int iMaxValue = StringToInt(sMaxValue) ;

    if ((iScore >= iMinValue) && (iScore < iMaxValue))
      return getColorFromChar(sColor[0]) ;
  }

  return colorNone ;
}

// -----------------------------------------------------------------------------
//
// NSQualityOfLifeInformation methods//
// -----------------------------------------------------------------------------
NSQualityOfLifeInformation::NSQualityOfLifeInformation(NSContexte *pCtx, string sFileName)
                           :NSRoot(pCtx)
{
  _sRootFileName   = sFileName ;
  _sRootConcept    = string("") ;
  _sScoreLexique   = string("") ;
  _sScoreUnit      = string("") ;  _sScoreEquation  = string("") ;
  _iDurationValue  = 0 ;
  _sDurationUnit   = string("") ;
  _sColorIntervals = string("") ;

  if (string("") != _sRootFileName)
    initFromFile() ;

  lObjectCount++ ;
}

NSQualityOfLifeInformation::~NSQualityOfLifeInformation()
{
  lObjectCount-- ;
}

NSQualityOfLifeInformation::NSQualityOfLifeInformation(const NSQualityOfLifeInformation& rv)
                           :NSRoot(rv.pContexte)
{
  _sRootConcept    = rv._sRootConcept ;

  _sScoreLexique   = rv._sScoreLexique ;
  _sScoreUnit      = rv._sScoreUnit ;  _sScoreEquation  = rv._sScoreEquation ;

  _iDurationValue  = rv._iDurationValue ;
  _sDurationUnit   = rv._sDurationUnit ;

  _sColorIntervals = rv._sColorIntervals ;

  if (false == rv._iconsForColors.empty())
    for (std::map<char, std::string>::const_iterator i = rv._iconsForColors.begin() ; rv._iconsForColors.end() != i ; i++)
      _iconsForColors[(*i).first] = (*i).second ;

  _aCards          = rv._aCards ;
  _aLines          = rv._aLines ;

  lObjectCount++ ;
}

NSQualityOfLifeInformation&
NSQualityOfLifeInformation::operator=(const NSQualityOfLifeInformation& src)
{
  if (this == &src)
    return *this ;

  _sRootConcept   = src._sRootConcept ;

  _sScoreLexique  = src._sScoreLexique ;
  _sScoreUnit     = src._sScoreUnit ;  _sScoreEquation = src._sScoreEquation ;

  _iDurationValue = src._iDurationValue ;
  _sDurationUnit  = src._sDurationUnit ;

  _sColorIntervals = src._sColorIntervals ;

  if (false == _iconsForColors.empty())
    _iconsForColors.clear() ;

  if (false == src._iconsForColors.empty())
    for (std::map<char, std::string>::const_iterator i = src._iconsForColors.begin() ; src._iconsForColors.end() != i ; i++)
      _iconsForColors[(*i).first] = (*i).second ;

  _aCards         = src._aCards ;
  _aLines         = src._aLines ;

  return *this ;
}

void
NSQualityOfLifeInformation::initFromFile()
{
  NSLocalChapterArray cardsArray ;

  // Get information from disk
  //
  getFileInformation(&cardsArray) ;

  if (cardsArray.empty())
    return ;

  // Initialize global data, create cards and lines
  //
  NSLocalChapterArrayIter chaptIt = cardsArray.begin() ;
  for ( ; cardsArray.end() != chaptIt ; chaptIt++)
  {
    string sChapterName = (*chaptIt).first ;

    if (string("Main") == sChapterName)
    {
      NSLocalisedChapter *pChapter = (*chaptIt).second ;

      _sRootConcept    = pChapter->getLocalText("Lexique") ;
      _sScoreLexique   = pChapter->getLocalText("LxScore") ;
      _sScoreUnit      = pChapter->getLocalText("UnScore") ;      _sScoreEquation  = pChapter->getLocalText("AlScore") ;
      _sColorIntervals = pChapter->getLocalText("Colors") ;

      _iconsForColors['B'] = pChapter->getLocalText("IconForB") ;
      _iconsForColors['G'] = pChapter->getLocalText("IconForG") ;
      _iconsForColors['Y'] = pChapter->getLocalText("IconForY") ;
      _iconsForColors['O'] = pChapter->getLocalText("IconForO") ;
      _iconsForColors['R'] = pChapter->getLocalText("IconForR") ;
    }
    if (string("DisplayLines") == sChapterName)
    {
      NSLocalisedChapter *pChapter = (*chaptIt).second ;

      _sDurationUnit  = pChapter->getLocalText("TimeIntervalUnit") ;
      _iDurationValue = StringToInt(pChapter->getLocalText("TimeIntervalValue")) ;
    }    else if (string(sChapterName, 0, 4) == string("Line"))
    {
      NSCoopLine *pLine = new NSCoopLine((*chaptIt).second) ;
      pLine->setLineNumber(StringToInt(string(sChapterName, 5, 1))) ;

      _aLines.push_back(pLine) ;
    }
    else
    {
      NSCoopCard *pCard = new NSCoopCard((*chaptIt).second) ;
      pCard->setTitle((*chaptIt).first) ;

      _aCards.push_back(pCard) ;
    }
  }
}

void
NSQualityOfLifeInformation::getFileInformation(NSLocalChapterArray *pCards)
{
  string sLang = pContexte->getUserLanguage() ;

  string sPersoPath = pContexte->PathName("FGLO") ;

  // Loading cards for user language
  //
  if (string("") != sLang)
  {
    ifstream inFileForLanguage ;
    string sLangFileName = sPersoPath + _sRootFileName + string("_") + sLang + string(".dat") ;

    inFileForLanguage.open(sLangFileName.c_str(), ofstream::in) ;
    if (inFileForLanguage.is_open())
    {
      string sErrorMsg = string("") ;
  	  pCards->init(&inFileForLanguage, &sErrorMsg) ;
      inFileForLanguage.close() ;

      return ;
    }
  }

  // Loading generic cards
  //
  ifstream inFile ;
  string sFileName = sPersoPath + _sRootFileName + string(".dat") ;

  inFile.open(sFileName.c_str(), ofstream::in) ;
  if (false == inFile.is_open())
  {
    string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFileName ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return ;
  }

  string sErrorMsg = string("") ;
  pCards->init(&inFile, &sErrorMsg) ;

  inFile.close() ;
}

NSCoopCard*
NSQualityOfLifeInformation::getCardFromTitle(string sTitle)
{
  if ((string("") == sTitle) || _aCards.empty())
    return (NSCoopCard*) 0 ;

  for (CoopCardIter cardIt = _aCards.begin() ; _aCards.end() != cardIt ; cardIt++)
    if ((*cardIt)->getTitle() == sTitle)
      return *cardIt ;

  return (NSCoopCard*) 0 ;
}

NSCoopLine*
NSQualityOfLifeInformation::getLineFromNumber(int iCardNb)
{
  if (_aLines.empty())
    return (NSCoopLine*) 0 ;

  for (CoopLineIter lineIt = _aLines.begin() ; _aLines.end() != lineIt ; lineIt++)
    if ((*lineIt)->getLineNumber() == iCardNb)
      return *lineIt ;

  return (NSCoopLine*) 0 ;
}

// -----------------------------------------------------------------------------
//
// NSQualityOfLifeChart methods//
// -----------------------------------------------------------------------------
NSQualityOfLifeChart::NSQualityOfLifeChart(NSContexte *pCtx, NVLdVTemps tpsInfo, NSPatPathoArray* pPPT)
                     :NSRoot(pCtx)
{
  _tpsInfo     = tpsInfo ;

  _sDocumentConcept  = string("") ;
  _sMainScoreConcept = string("") ;
  _sMainScoreUnit    = string("") ;
  _sDocumentId       = string("") ;
  _pQolInfo          = (NSQualityOfLifeInformation*) 0 ;
  _iGlobalColor      = colorNone ;

  initCoreInformation(pPPT) ;
  initQolInformation() ;
  initChart(pPPT) ;
  initLines() ;

  if ((false == _tpsInfo.estVide()) && (false == _tpsInfo.estNoLimit()))
  {
    _tpsStart = _tpsInfo ;

    if (_pQolInfo && (string("") != _pQolInfo->getDurationUnit()) &&
        (0 != _pQolInfo->getDurationValue()))
    {
      int iDuration = - _pQolInfo->getDurationValue() ;
      _tpsStart.ajouteTemps(iDuration, _pQolInfo->getDurationUnit(), pContexte) ;
    }
    else
      _tpsStart.ajouteJours(-14) ;
  }

   lObjectCount++ ;
}

NSQualityOfLifeChart::~NSQualityOfLifeChart()
{
  if (_pQolInfo)
    delete _pQolInfo ;

  lObjectCount-- ;
}

NSQualityOfLifeChart::NSQualityOfLifeChart(const NSQualityOfLifeChart& rv)
                     :NSRoot(rv.pContexte)
{
  copyFrom(rv) ;

  lObjectCount++ ;
}

NSQualityOfLifeChart&
NSQualityOfLifeChart::operator=(const NSQualityOfLifeChart& src)
{
  if (this == &src)
    return *this ;

  if (_pQolInfo)
  {
    delete _pQolInfo ;
    _pQolInfo = (NSQualityOfLifeInformation*) 0 ;
  }

  copyFrom(src) ;

  return *this ;
}

void
NSQualityOfLifeChart::initCoreInformation(NSPatPathoArray* pPPT)
{
  if ((NULL == pPPT) || (pPPT->empty()))
    return ;

  PatPathoIter it = pPPT->begin() ;

  _sDocumentConcept = (*it)->getLexique() ;
  _sDocumentId      = (*it)->getDoc() ;

  getScoreConceptFromDocConcept() ;
}

void
NSQualityOfLifeChart::initChart(NSPatPathoArray* pPPT)
{
  if ((NULL == pPPT) || (pPPT->empty()))
    return ;

  if (_pQolInfo && (string("") == _sMainScoreConcept))
    _sMainScoreConcept = _pQolInfo->getScoreLexique() ;

  PatPathoIter it = pPPT->begin() ;
  while (pPPT->end() != it)
  {
    if ((*it)->getColonne() == 1)
    {
      string sLexique = (*it)->getLexique() ;
      string sLexSens = (*it)->getLexiqueSens() ;
      string sCode    = (*it)->getComplement() ;

      it++ ;

      while ((pPPT->end() != it) && ((*it)->getColonne() > 1))
      {
        if (string("£N0;02") == (*it)->getLexique())
        {
          int    iValue = StringToInt((*it)->getComplement()) ;
          string sUnit  = (*it)->getUnit() ;
          addScore(sLexique, sCode, sUnit, iValue) ;

          if (sLexSens == _sMainScoreConcept)
          {
            _sMainScoreConcept = sLexique ;
            _sMainScoreUnit    = sUnit ;
            if (_pQolInfo)
              _iGlobalColor = getColorFromScore(iValue, _pQolInfo->getColorIntervals()) ;
          }

          it++ ;
        }
        else if (string("0REPO1") == (*it)->getLexique())
        {
          it++ ;

          while ((pPPT->end() != it) && ((*it)->getColonne() > 2))
          {
            if (string("£N0;02") == (*it)->getLexique())
            {
              int    iValue = StringToInt((*it)->getComplement()) ;
              string sUnit  = (*it)->getUnit() ;
              addScore(sLexique, sCode, sUnit, iValue) ;
            }

            it++ ;
          }
        }
        else
          it++ ;
      }
    }
    else
      it++ ;
  }
}

void
NSQualityOfLifeChart::initQolInformation()
{
  string sWhat  = string("") ;
  string sWhere = string("") ;

  // Remember that there is not always a main score (for example Coop Charts)
  //
  if (false == pContexte->getSuperviseur()->getWhereToFind(_sMainScoreConcept, sWhat, sWhere))
  {
    if (false == pContexte->getSuperviseur()->getWhereToFind(_sDocumentConcept, sWhat, sWhere))
    {
      if (string("ZCOOP1") == _sDocumentConcept)
        sWhere = string("coopCharts") ;
      else
        return ;
    }
  }

  _pQolInfo = new NSQualityOfLifeInformation(pContexte, sWhere) ;
}

void
NSQualityOfLifeChart::initLines()
{
  if (NULL == _pQolInfo)
    return ;

  NSCoopLineArray* pLines = _pQolInfo->getLines() ;
  if ((NULL == pLines) || pLines->empty())
    return ;

  for (CoopLineIter lineIt = pLines->begin() ; pLines->end() != lineIt ; lineIt++)
  {
    string sImpliedCards = (*lineIt)->getElements() ;
    string sOperation    = (*lineIt)->getOperation() ;

    int iLineScore = -1 ;
    if (string("_mean_") == sOperation)
      iLineScore = getMeanScore(sImpliedCards) ;

    (*lineIt)->setScore(iLineScore) ;
  }
}

int
NSQualityOfLifeChart::getMeanScore(string sImpliedCards)
{
  if ((string("") == sImpliedCards) || (NULL == _pQolInfo))
    return -1 ;

  int iTotalScore = 0 ;
  int iTotalElmts = 0 ;

  ClasseStringVector aElements ;
  DecomposeChaine(&sImpliedCards, &aElements, string("|")) ;

  for (iterClassString it = aElements.begin() ; aElements.end() != it ; it++)
  {
    int iCardScore = -1 ;

    string sCardTitle = (*it)->getItem() ;

    if (string("Main") == sCardTitle)
      iCardScore = getMainScore() ;
    else
    {
      NSCoopCard* pCard = _pQolInfo->getCardFromTitle((*it)->getItem()) ;
      if (pCard)
        iCardScore = getScoreValueForCard(pCard) ;
    }

    if (-1 != iCardScore)
    {
      iTotalScore += iCardScore ;
      iTotalElmts++ ;
    }
  }

  double dMeanValue = double(iTotalScore) / double(iTotalElmts) ;

  return dtoi(dMeanValue) ;
}

string
NSQualityOfLifeChart::getScoreIcon()
{
  if ((NULL == _pQolInfo) || (colorNone == _iGlobalColor))
    return string("") ;

  char cColor = getCharFromColor(_iGlobalColor) ;

  return _pQolInfo->getIconForColor(cColor) ;
}

int
NSQualityOfLifeChart::getScoreValueForCard(NSCoopCard *pCard)
{
  if (NULL == pCard)
    return -1 ;

  string sLexique = string("") ;

  if (string("") != pCard->getSpecific())
    sLexique = pCard->getSpecific() + string(1, cheminSeparationMARK) + pCard->getUnit() ;
  else
    sLexique = pCard->getGeneric() + string(1, cheminSeparationMARK) + string(1, '$') + pCard->getCode() + string(1, cheminSeparationMARK) + pCard->getUnit() ;

  return _subScores[sLexique] ;
}

void
NSQualityOfLifeChart::copyFrom(const NSQualityOfLifeChart& src)
{
  _tpsInfo     = src._tpsInfo ;
  _tpsStart    = src._tpsStart ;

  _sDocumentConcept  = src._sDocumentConcept ;
  _sMainScoreConcept = src._sMainScoreConcept ;
  _sMainScoreUnit    = src._sMainScoreUnit ;
  _sDocumentId       = src._sDocumentId ;
  _iGlobalColor      = src._iGlobalColor ;

  if (src._pQolInfo)
    _pQolInfo = new NSQualityOfLifeInformation(*(src._pQolInfo)) ;
  else
    _pQolInfo = (NSQualityOfLifeInformation*) 0 ;

  if (false == _subScores.empty())
    _subScores.clear() ;

  if (false == src._subScores.empty())
    for (std::map<std::string, int>::const_iterator i = src._subScores.begin() ; src._subScores.end() != i ; i++)
      _subScores[(*i).first] = (*i).second ;
}

void
NSQualityOfLifeChart::addScore(string sSpecificLexique, string sUnit, int iScore)
{
  if ((string("") == sSpecificLexique) || (string("") == sUnit))
    return ;

  string sLexique = sSpecificLexique + string(1, cheminSeparationMARK) + sUnit ;
  _subScores[sLexique] = iScore ;
}

void
NSQualityOfLifeChart::addScore(string sGenericLexique, string sCode, string sUnit, int iScore)
{
  if ((string("") == sGenericLexique) || (string("") == sUnit))
    return ;

  if (string("") == sCode)
  {
    addScore(sGenericLexique, sUnit, iScore) ;
    return ;
  }

  string sLexique = sGenericLexique + string(1, cheminSeparationMARK) + string(1, '$') + sCode + string(1, cheminSeparationMARK) + sUnit ;
  _subScores[sLexique] = iScore ;
}

int
NSQualityOfLifeChart::getScore(string sSpecificLexique, string sUnit)
{
  string sLexique = sSpecificLexique + string(1, cheminSeparationMARK) + sUnit ;

  if (_subScores.find(sLexique) != _subScores.end())
    return _subScores[sLexique] ;

  return -1 ;
}

int
NSQualityOfLifeChart::getScore(string sGenericLexique, string sCode, string sUnit)
{
  string sLexique = sGenericLexique + string(1, cheminSeparationMARK) + string(1, '$') + sCode + string(1, cheminSeparationMARK) + sUnit ;

  if (_subScores.find(sLexique) != _subScores.end())
    return _subScores[sLexique] ;

  return -1 ;
}

int
NSQualityOfLifeChart::getMainScore()
{
  return getScore(_sMainScoreConcept, _sMainScoreUnit) ;
}

// Use a "measure with" semantic trait to get score from concept
//
void
NSQualityOfLifeChart::getScoreConceptFromDocConcept()
{
  if (string("") == _sDocumentConcept)
    return ;

  string sDocumentConceptSens ;
  pContexte->getDico()->donneCodeSens(&_sDocumentConcept, &sDocumentConceptSens) ;

  // Semantic search
  //
  VectString VSEquivalent ;
	pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sDocumentConceptSens, "ME", &VSEquivalent) ;

  if (false == VSEquivalent.empty())
    _sMainScoreConcept = *(*VSEquivalent.begin()) ;
}

// -----------------------------------------------------------------------------
//
// NSCoopChart methods//
// -----------------------------------------------------------------------------
NSCoopChart::NSCoopChart(NSContexte *pCtx, NVLdVTemps tpsInfo, NSPatPathoArray* pPPT)
            :NSQualityOfLifeChart(pCtx, tpsInfo, pPPT)
{
  lObjectCount++ ;
}

NSCoopChart::~NSCoopChart()
{
  lObjectCount-- ;
}

NSCoopChart::NSCoopChart(const NSCoopChart& rv)
            :NSQualityOfLifeChart(rv)
{
  lObjectCount++ ;
}

NSCoopChart&
NSCoopChart::operator=(const NSCoopChart& src)
{
  if (this == &src)
    return *this ;

  copyFrom(src) ;

  return *this ;
}

/*
void
NSCoopChart::initChart(NSPatPathoArray* pPPT)
{
  if ((NULL == pPPT) || (pPPT->empty()))
    return ;

  PatPathoIter it = pPPT->begin() ;
  if ((*it)->getLexique() != string("ZCOOP1"))
    return ;

  _sDocumentId = (*it)->getDoc() ;

  it++ ;

  for (PatPathoIter it = pPPT->begin() ; pPPT->end() != it ; it++)
  {
    if ((*it)->getColonne() == 1)
    {
      string sLexique = (*it)->getLexique() ;
      it++ ;
      if ((pPPT->end() != it) && ((*it)->getColonne() == 2))
      {
        if ((string("£N0;02") == (*it)->getLexique()) &&
            (string("2SUR51") == (*it)->getUnit()))
        {
          int iValue = StringToInt((*it)->getComplement()) ;

          if      (string("VCWCP1") == sLexique)
            _iPhysicalScore = iValue ;
          else if (string("VCWEE1") == sLexique)
            _iEmotionsScore = iValue ;
          else if (string("VCWAQ1") == sLexique)
            _iDaylyTasksScore = iValue ;
          else if (string("VCWAS1") == sLexique)
            _iSocialScore = iValue ;
          else if (string("VCWCH1") == sLexique)
            _iChangeScore = iValue ;
          else if (string("VCWSG1") == sLexique)
            _iHealthScore = iValue ;
        }
      }
    }
  }
}
*/

// -----------------------------------------------------------------------------
//
// NSCoopCard methods//
// -----------------------------------------------------------------------------

NSCoopCard::NSCoopCard(NSLocalisedChapter *pChapter)
{
  lObjectCount++ ;

  _iScore           = 0 ;
  _bIsScoreSet      = false ;
  _sTitle           = string("") ;
  _sFileName        = string("") ;

  _sSpecificConcept = string("") ;
  _sGenericConcept  = string("") ;
  _sConceptCode     = string("") ;
  _sUnit            = string("") ;

  _sQuestionLabel   = string("") ;

  if (NULL == pChapter)
    return ;

  _sGenericConcept  = pChapter->getLocalText("Generic") ;
  _sSpecificConcept = pChapter->getLocalText("Lexique") ;
  _sConceptCode     = pChapter->getLocalText("Code") ;
  _sUnit            = pChapter->getLocalText("Unit") ;

  _sFileName        = pChapter->getLocalText("File") ;

  _sQuestionLabel   = pChapter->getLocalText("Label") ;

  string sStarter = string("Button") ;
  for (int i = 1 ; ; i++)
  {
    string sIndex = IntToString(i) ;
    string sButtonName = sStarter + sIndex ;

    string sButtonParams = pChapter->getLocalText(sButtonName) ;
    if (string("") == sButtonParams)
      break ;

    _aCardButtons.push_back(new NSCoopCardButton(sButtonParams)) ;
  }
}

NSCoopCard::NSCoopCard(const NSCoopCard& rv)
{
  lObjectCount++ ;

  _iScore           = rv._iScore ;
  _bIsScoreSet      = rv._bIsScoreSet ;
  _sTitle           = rv._sTitle ;
  _sFileName        = rv._sFileName ;
  _sSpecificConcept = rv._sSpecificConcept ;
  _sGenericConcept  = rv._sGenericConcept ;
  _sConceptCode     = rv._sConceptCode ;
  _sUnit            = rv._sUnit ;
  _sQuestionLabel   = rv._sQuestionLabel ;
  _aCardButtons     = rv._aCardButtons ;
}

NSCoopCard&
NSCoopCard::operator=(const NSCoopCard& src)
{
  if (this == &src)
    return *this ;

  _iScore           = src._iScore ;
  _bIsScoreSet      = src._bIsScoreSet ;
  _sTitle           = src._sTitle ;
  _sFileName        = src._sFileName ;
  _sSpecificConcept = src._sSpecificConcept ;
  _sGenericConcept  = src._sGenericConcept ;
  _sConceptCode     = src._sConceptCode ;
  _sUnit            = src._sUnit ;
  _sQuestionLabel   = src._sQuestionLabel ;
  _aCardButtons     = src._aCardButtons ;

  return *this ;
}

NSCoopCardButton*
NSCoopCard::hitButtonTest(NS_CLASSLIB::TPoint *pPoint)
{
  if ((NULL == pPoint) || _aCardButtons.empty())
    return NULL ;

  CoopCardButtonIter buttonIter = _aCardButtons.begin() ;
  for ( ; _aCardButtons.end() != buttonIter ; buttonIter++)
    if ((*buttonIter)->contains(pPoint))
      return *buttonIter ;

  return NULL ;
}

NSCoopCardButton*
NSCoopCard::getButtonForScore(int iScore)
{
  if (_aCardButtons.empty())
    return NULL ;

  CoopCardButtonIter buttonIter = _aCardButtons.begin() ;
  for ( ; _aCardButtons.end() != buttonIter ; buttonIter++)
    if ((*buttonIter)->getScore() == iScore)
      return *buttonIter ;

  return NULL ;
}

// -----------------------------------------------------------------------------
//
// NSCoopCardButton methods//
// -----------------------------------------------------------------------------NSCoopCardButton::NSCoopCardButton(string sParams)
{
  _sButtonName = string("") ;
  _iScore      = -1 ;

  parseParams(sParams) ;

  lObjectCount++ ;
}

NSCoopCardButton::NSCoopCardButton(const NSCoopCardButton& rv)
{
  _sButtonName = rv._sButtonName ;
  _iScore      = rv._iScore ;
  _buttonRect  = rv._buttonRect ;

  lObjectCount++ ;
}

NSCoopCardButton&
NSCoopCardButton::operator=(const NSCoopCardButton& src)
{
  if (this == &src)
    return *this ;

  _sButtonName = src._sButtonName ;
  _iScore      = src._iScore ;
  _buttonRect  = src._buttonRect ;

  return *this ;
}

bool
NSCoopCardButton::contains(NS_CLASSLIB::TPoint *pPoint)
{
  if (NULL == pPoint)
    return false ;

  return _buttonRect.Contains(*pPoint) ;
}

void
NSCoopCardButton::parseParams(string sParams)
{
  strip(sParams) ;
  if (string("") == sParams)
    return ;

  ClasseStringVector parseVector ;

  // sParams: "button name|(xTopLeft,yTopLeft)(xBottomRight,yBottomRight)|score
  //
  DecomposeChaine(&sParams, &parseVector, string("|")) ;
  if (parseVector.empty())
    return ;

  iterClassString it = parseVector.begin() ;
  _sButtonName = (*it)->getItem() ;

  it++ ;
  if (parseVector.end() == it)
    return ;

  parseParamsRect((*it)->getItem()) ;

  it++ ;
  if (parseVector.end() == it)
    return ;

  _iScore = StringToInt((*it)->getItem()) ;
}

void
NSCoopCardButton::parseParamsRect(string sParams)
{
  strip(sParams) ;
  if (string("") == sParams)
    return ;

  NS_CLASSLIB::TPoint ptTopLeft ;
  if (false == parseParamsPoint(&sParams, &ptTopLeft))
    return ;

  NS_CLASSLIB::TPoint ptBottomRight ;
  if (false == parseParamsPoint(&sParams, &ptBottomRight))
    return ;

  _buttonRect = NS_CLASSLIB::TRect(ptTopLeft, ptBottomRight) ;
}

bool
NSCoopCardButton::parseParamsPoint(string *psParams, NS_CLASSLIB::TPoint *pPoint)
{
  if ((NULL == psParams) || (NULL == pPoint))
    return false ;

  if ('(' != (*psParams)[0])
    return false ;

  size_t iParPos = psParams->find(")") ;
  if (NPOS == iParPos)
    return false ;

  size_t iSeparPos = psParams->find(",") ;
  if ((NPOS == iSeparPos) || (iSeparPos > iParPos))
    return false ;

  // Example (123,212)(13,22)
  //         0123456789012345
  string sX = string(*psParams, 1, iSeparPos - 1) ;
  string sY = string(*psParams, iSeparPos + 1, iParPos - iSeparPos - 1) ;

  *pPoint = NS_CLASSLIB::TPoint(StringToInt(sX), StringToInt(sY)) ;

  size_t iParamsLength = strlen(psParams->c_str()) ;
  if (iParamsLength == iParPos + 1)
    return true ;

  *psParams = string(*psParams, iParPos + 1, iParamsLength - iParPos - 1) ;

  return true ;
}

// -----------------------------------------------------------------------------
//
// NSCoopLine methods//
// -----------------------------------------------------------------------------

NSCoopLine::NSCoopLine(NSLocalisedChapter *pChapter)
{
  lObjectCount++ ;

  _iLineNumber     = 0 ;
  _sTitle          = string("") ;
  _sElements       = string("") ;
  _sOperation      = string("") ;
  _sColorIntervals = string("") ;
  _iScore          = 0 ;
  _iLineColor      = colorNone ;

  if (NULL == pChapter)
    return ;

  _sTitle          = pChapter->getLocalText("Title") ;
  _sElements       = pChapter->getLocalText("Elements") ;
  _sOperation      = pChapter->getLocalText("Operation") ;
  _sColorIntervals = pChapter->getLocalText("Colors") ;
}

void
NSCoopLine::setScore(int iScore)
{
  _iScore     = iScore ;
  _iLineColor = getColorFromScore(_iScore, _sColorIntervals) ;
}

NSCoopLine::NSCoopLine(const NSCoopLine& rv)
{
  lObjectCount++ ;

  _iLineNumber     = rv._iLineNumber ;
  _sTitle          = rv._sTitle ;
  _sElements       = rv._sElements ;
  _sOperation      = rv._sOperation ;
  _sColorIntervals = rv._sColorIntervals ;
  _iScore          = rv._iScore ;
  _iLineColor      = rv._iLineColor ;
}

NSCoopLine&
NSCoopLine::operator=(const NSCoopLine& src)
{
  if (this == &src)
    return *this ;

  _iLineNumber     = src._iLineNumber ;
  _sTitle          = src._sTitle ;
  _sElements       = src._sElements ;
  _sOperation      = src._sOperation ;
  _sColorIntervals = src._sColorIntervals ;
  _iScore          = src._iScore ;
  _iLineColor      = src._iLineColor ;

  return *this ;
}

