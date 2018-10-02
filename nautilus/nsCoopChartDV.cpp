// --------------------------------------------------------------------------// NSCOOPCHARTDV.CPP		Document/Vues Coop Charts
// --------------------------------------------------------------------------
// PA - janvier 2007
// --------------------------------------------------------------------------

#include <owl\dc.h>
#include <owl\gdiobjec.h>
#include <winsys\color.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsbb\nsPaneSplitter.h"
#include "nsbb\nsbbtran.h"
#include "nsdn\nsdocnoy.h"

#include "nautilus\nsCoopChartDV.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\ns_html.h"

#include "partage\ns_timer.h"
#include "partage\nsdivfile.h"

#include "nssavoir\nsgraphe.h"

#include "nssavoir\HealthTeam.rh"

// --------------------------------------------------------------------------
//
//					METHODES DE NSCoopChart Document / Vue
//
// --------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//
// NSCoopChartDocument methods//
// -----------------------------------------------------------------------------
NSCoopChartDocument::NSCoopChartDocument(TDocument* parent,
                                         NSDocumentInfo* pDocumentInfo,
                                         NSDocumentInfo* pDocHtmlInfo,
                                         NSContexte* pCtx, bool bROnly,
                                         string sFileName)
                    :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
  if (pContexte->userHasPrivilege(NSContexte::createDocument, -1, -1, string(""), string(""), NULL, NULL))
		_bReadOnly = false ;
  else
		_bReadOnly = true ;

  _sConcern = string("") ;

  if (string("") == sFileName)
    sFileName = getFileName() ;

  initCardsFromFilename(sFileName) ;
}

NSCoopChartDocument::NSCoopChartDocument(TDocument* parent, NSContexte* pCtx, string sFileName, string sConcern)                    :NSRefDocument(parent, pCtx){  if (pContexte->userHasPrivilege(NSContexte::createDocument, -1, -1, string(""), string(""), NULL, NULL))		_bReadOnly = false ;
  else
		_bReadOnly = true ;  _sConcern = sConcern ;  if (string("") == sFileName)    sFileName = getFileName() ;  initCardsFromFilename(sFileName) ;}boolNSCoopChartDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
    						                string sAdresseCorresp, string sPathDest)
{
	NSHtmlQoL hqol(typeOperation, this, pContexte, sAdresseCorresp) ;

	// on trouve le nom du fichier temporaire à visualiser	sNomHtml = hqol.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	string sFichierHtml = sPathHtml + sNomHtml ;

	// on passe aussi le path destination pour les images  string sBaseImg = string("") ;	if (false == hqol.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo, sPathDest))
		return false ;

	// Mise à jour de la base d'images	switch (typeOperation)
	{
		case toComposer:
    	_sBaseCompo = sBaseImg ;
      break ;

    default:    	_sBaseImages = sBaseImg ;
	}

	return true ;
}

bool
NSCoopChartDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  NSHtmlQoL hqol(tCR, this, pContexte, string("")) ;

  string sOut = string("") ;

  for (int i = 0 ; i < hqol.nbHtml ; i++)
  {
    NSHtml* pHtml = hqol.blocHtml[i]->getHtml() ;
    if (pHtml && (pHtml->getType() == string("QoL")) && (hqol.blocHtml[i]->occur()))
    {
      pHtml->genere(sHtmlText) ;
      return true ;
    }
  }

  return false ;
}
stringNSCoopChartDocument::getFileName(){  if (NULL == _pDocInfo)    return string("CoopCharts") ;  string sSemContent = _pDocInfo->getSemCont() ;
  string sWhat  = string("") ;
  string sWhere = string("") ;
  bool bIsSpecific = pContexte->getSuperviseur()->getWhereToFind(sSemContent, sWhat, sWhere) ;

  if (false == bIsSpecific)
  {
    bIsSpecific = true ;
    sWhat  = string("QoL") ;
    sWhere = string("CoopCharts") ;
  }

  string sWHAT = pseumaj(sWhat) ;

  // Quality of life cards, à la Coop Charts
  //
  if (string("QOL") == sWHAT)    return sWhere ;  return string("") ;}voidNSCoopChartDocument::initCardsFromFilename(string sFileName){  string sRootFileName = string("coopCharts") ;  if (string("") != sFileName)    sRootFileName = sFileName ;  _pQoLInformation = new NSQualityOfLifeInformation(pContexte, sRootFileName) ;  initCards() ;}NSCoopChartDocument::~NSCoopChartDocument()
{
  if (_pQoLInformation)
    delete _pQoLInformation ;
}

void
NSCoopChartDocument::initCards()
{
  if ((NSQualityOfLifeInformation*) NULL == _pQoLInformation)
    return ;

  NSCoopCardArray* pCards = _pQoLInformation->getCards() ;
  if (((NSCoopCardArray*) NULL == pCards) || pCards->empty())
    return ;

  if (_PatPathoArray.empty())
    return ;

  // Init scores from patpatho
  //
  PatPathoIter iter = _PatPathoArray.begin() ;
  if ((*iter)->getLexique() != _pQoLInformation->getRootConcept())
    return ;

  int iBaseCol = (*iter)->getColonne() ;
  iter++ ;

  NSCoopCard *pCurrentCard = (NSCoopCard *) 0 ;

  for ( ; _PatPathoArray.end() != iter ; iter++)
  {
    if ((*iter)->getColonne() == iBaseCol + 1)
    {
      string sLexique    = (*iter)->getLexique() ;
      string sComplement = (*iter)->getComplement() ;

      if (string("") == sComplement)
        pCurrentCard = getCardForLexique(sLexique) ;
      else
        pCurrentCard = getGenericCardForCode(sLexique, sComplement) ;
    }
    else if (((*iter)->getColonne() > iBaseCol + 1) && pCurrentCard)
    {
      string sLexique = (*iter)->getLexique() ;

      if (('£' == sLexique[0]) && (string("£CL") != string(sLexique, 0, 3))
                               && (pCurrentCard->getUnit() == (*iter)->getUnit()))
      {
				string sValeur = (*iter)->getComplement() ;
        if (string("") != sValeur)
        {
          pCurrentCard->setScore(StringToInt(sValeur)) ;
          pCurrentCard->setScoreIsSet(true) ;
        }
      }
    }
  }
}

bool
NSCoopChartDocument::savePatpatho()
{
  string sLang = pContexte->getUserLanguage() ;

  bool bNewDoc ;
	if (NULL == _pDocInfo)
		bNewDoc = true ;
	else
		bNewDoc = false ;

  if (bNewDoc)
	{
    buildPatPatho() ;

		string sLibelleDoc = string("") ;

    if (false == _PatPathoArray.empty())
    {
    	string sCode = (*(_PatPathoArray.begin()))->getLexique() ;
      pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
    }

		bool existeInfo = Referencer("ZQQOL", sLibelleDoc, "", "", true, true) ;
		if (false == existeInfo)
		{
			_bEnregEnCours = false ;
			return false ;
		}

    if ((string("") != _sConcern) && _pDocInfo)
    {
      NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
      pGraphe->etablirLienDocument(_pDocInfo->getID(), NSRootLink::problemRelatedTo, _sConcern) ;
    }
	}
  else
    updatePatPatho() ;

	// On enregistre la patpatho
	bool bSaveSuccess = enregistrePatPatho() ;
	if (false == bSaveSuccess)
	{
		_bEnregEnCours = false ;
		return false ;
	}

	bool bReloaded = NSNoyauDocument::chargePatPatho() ;
	if (false == bReloaded)
	{
		_bEnregEnCours = false ;
		return false ;
	}

  // The new document must be inserted in "history" before checking if it resets
  // any goal, because this document's date will be asked to "history"
  //
	pContexte->getPatient()->getDocHis()->Rafraichir(_pDocInfo, &_PatPathoArray, this) ;

	// Si c'est un nouveau compte rendu on vérifie s'il remet à zéro un objectif
  if (pContexte->getPatient()->getLdvDocument())
  {
	  if (bNewDoc)
    {
		  pContexte->getPatient()->getLdvDocument()->showNewTree(&_PatPathoArray, GetDateDoc(false)) ;

      if (_pDocInfo)
      {
        string sDate = _pDocInfo->getCreDate() ;

        if (string("ZCOOP1") == _pQoLInformation->getRootConcept())
          pContexte->getPatient()->getLdvDocument()->addQualityOfLifeCharts(sDate, &_PatPathoArray) ;
        else
          pContexte->getPatient()->getLdvDocument()->addQualityOfLifeCharts(_sConcern, sDate, &_PatPathoArray) ;
      }
    }
    else if (_pDocInfo)
    {
      string sDate = _pDocInfo->getCreDate() ;
      pContexte->getPatient()->getLdvDocument()->modifyQualityOfLifeCharts(sDate, &_PatPathoArray) ;
    }
  }

	pContexte->getSuperviseur()->afficheStatusMessage("Document enregistré") ;

	_bEnregEnCours = false ;

  return true ;
}

bool
NSCoopChartDocument::buildPatPatho()
{
  NSCoopCardArray* pCards = _pQoLInformation->getCards() ;
  if ((NULL == pCards) || pCards->empty())
    return false ;

  _PatPathoArray.ajoutePatho(_pQoLInformation->getRootConcept(), 0) ;

  if (string("") != _pQoLInformation->getScoreLexique())
    addScoreToPatpatho() ;

  for (CoopCardIter cardIt = pCards->begin() ; pCards->end() != cardIt ; cardIt++)
    if ((*cardIt)->getScore() > 0)
      addCardToPatpatho(*cardIt) ;

  return true ;
}

void
NSCoopChartDocument::addCardToPatpatho(NSCoopCard *pCard)
{
  if ((NSCoopCard*) NULL == pCard)
    return ;

  // ---------- Question ----------
  //
  if (string("") != pCard->getSpecific())
  {
    _PatPathoArray.ajoutePatho(pCard->getSpecific(), 1) ;
  }
  else if (string("") != pCard->getGeneric())
  {
    Message RootMsg ;
    RootMsg.SetLexique(pCard->getGeneric()) ;
    RootMsg.SetComplement(pCard->getCode()) ;
    _PatPathoArray.ajoutePatho(pCard->getGeneric(), &RootMsg, 1) ;
  }
  else
    _PatPathoArray.ajoutePatho(string("6QUES1"), 1) ;

  // Insert question's original verbatim
  //
  if (string("") != pCard->getLabel())
  {
    Message LabelMsg ;
    LabelMsg.SetTexteLibre(pCard->getLabel()) ;
    _PatPathoArray.ajoutePatho("£?????", &LabelMsg, 2) ;
  }

  // ---------- Answer ----------
  //
  _PatPathoArray.ajoutePatho("0REPO1", 2) ;

  int iCardScore = pCard->getScore() ;

  Message CodeMsg ;
  CodeMsg.SetLexique("£N0;02") ;
  CodeMsg.SetUnit(pCard->getUnit()) ;
  CodeMsg.SetComplement(IntToString(iCardScore)) ;

  // Insert answer's original verbatim
  //
  NSCoopCardButton* pButton = pCard->getButtonForScore(iCardScore) ;
  if (pButton && (string("") != pButton->getLabel()))
  {
    Message LabelMsg ;
    LabelMsg.SetTexteLibre(pButton->getLabel()) ;
    _PatPathoArray.ajoutePatho("£?????", &LabelMsg, 3) ;
  }

  _PatPathoArray.ajoutePatho("£N0;02", &CodeMsg, 3) ;
}

void
NSCoopChartDocument::addScoreToPatpatho()
{
  if (string("") == _pQoLInformation->getScoreLexique())
    return ;

  int iGlobal = calculateGlobalScore() ;
  if (-1 == iGlobal)
    return ;

  _PatPathoArray.ajoutePatho(_pQoLInformation->getScoreLexique(), 1) ;

  Message CodeMsg ;
  CodeMsg.SetLexique("£N0;02") ;
  if (string("") == _pQoLInformation->getScoreUnit())
    CodeMsg.SetUnit("200001") ;
  else
    CodeMsg.SetUnit(_pQoLInformation->getScoreUnit()) ;
  CodeMsg.SetComplement(IntToString(iGlobal)) ;

  _PatPathoArray.ajoutePatho("£N0;02", &CodeMsg, 2) ;
}

// Of course, we could fully rebuild the patpatho, but we would loose
// existing nodes ID
//
bool
NSCoopChartDocument::updatePatPatho()
{
  if (string("") != _pQoLInformation->getScoreLexique())
  {
    int iGlobal = calculateGlobalScore() ;
    PatPathoIter pptIter = getPptLocationForScore() ;

    // There is a score for this card
    //
    if (-1 != iGlobal)
    {
      //
      // Already existing... update it
      //
      if (pptIter)
      {
        int iCol = (*pptIter)->getColonne() ;
        pptIter++ ;
        if ((_PatPathoArray.end() != pptIter) && ((*pptIter)->getColonne() == iCol+1))
          (*pptIter)->setComplement(IntToString(iGlobal)) ;
      }
      else
        addScoreToPatpatho() ;
    }
    //
    // There is no score for this card
    //
    else
    {
      if (pptIter)
        _PatPathoArray.SupprimerItem(pptIter) ;
    }
  }

  NSCoopCardArray* pCards = _pQoLInformation->getCards() ;
  if ((NULL == pCards) || pCards->empty())
    return false ;

  for (CoopCardIter cardIt = pCards->begin() ; pCards->end() != cardIt ; cardIt++)
  {
    PatPathoIter pptIter = getPptLocationForCard(*cardIt) ;

    int iCardScore = (*cardIt)->getScore() ;

    // There is a score for this card
    //
    if (iCardScore > 0)
    {
      //
      // Already existing... update it
      //
      if (pptIter)
      {
        // Looking for the answer
        //
        int iCol = (*pptIter)->getColonne() ;

        PatPathoIter pptAnswerIter = pptIter ;
        do
          pptAnswerIter++ ;
        while ((_PatPathoArray.end() != pptAnswerIter) &&
               ((*pptAnswerIter)->getColonne() > iCol) &&
               ((*pptAnswerIter)->getLexiqueSens() != string("0REPO"))) ;

        // Answer not found... check if score is not simply put there (deprecated)
        if ((_PatPathoArray.end() == pptAnswerIter) || ((*pptAnswerIter)->getColonne() <= iCol))
        {
          pptIter++ ;
          if ((_PatPathoArray.end() != pptIter) && ((*pptIter)->getColonne() == iCol+1))
            (*pptIter)->setComplement(IntToString(iCardScore)) ;
        }
        else
        {
          int iAnswerCol = (*pptAnswerIter)->getColonne() ;

          // Must update both answer's verbatim and score
          //
          pptAnswerIter++ ;

          while ((_PatPathoArray.end() != pptAnswerIter) &&
                 ((*pptAnswerIter)->getColonne() > iAnswerCol))
          {
            string sLexSens = (*pptAnswerIter)->getLexiqueSens() ;

            if (string("£N0") == sLexSens)
              (*pptAnswerIter)->setComplement(IntToString(iCardScore)) ;
            else if (string("£??") == sLexSens)
            {
              NSCoopCardButton* pButton = (*cardIt)->getButtonForScore(iCardScore) ;
              if (pButton && (string("") != pButton->getLabel()))
                (*pptAnswerIter)->setTexteLibre(pButton->getLabel()) ;
            }

            pptAnswerIter++ ;
          }
        }
      }
      else
        addCardToPatpatho(*cardIt) ;
    }
    //
    // There is no score for this card
    //
    else
    {
      if (pptIter)
        _PatPathoArray.SupprimerItem(pptIter) ;
    }
  }
  return true ;
}

PatPathoIter
NSCoopChartDocument::getPptLocationForCard(NSCoopCard *pCard)
{
  if ((NULL == pCard) || _PatPathoArray.empty())
    return (PatPathoIter) 0 ;

  // Skip root patho
  //
  PatPathoIter iter = _PatPathoArray.begin() ;
  int iBaseCol = (*iter)->getColonne() ;
  iter++ ;

  for ( ; _PatPathoArray.end() != iter ; iter++)
  {
    if ((*iter)->getColonne() == iBaseCol + 1)
    {
      string sLexique    = (*iter)->getLexique() ;
      string sComplement = (*iter)->getComplement() ;

      if (pCard->getSpecific() == sLexique)
        return iter ;

      if ((pCard->getGeneric() == sLexique) && (pCard->getCode() == sComplement))
        return iter ;
    }
  }

  return (PatPathoIter) 0 ;
}

PatPathoIter
NSCoopChartDocument::getPptLocationForScore()
{
  if (_PatPathoArray.empty() || (string("") == _pQoLInformation->getScoreLexique()))
    return (PatPathoIter) 0 ;

  // Skip root patho
  //
  PatPathoIter iter = _PatPathoArray.begin() ;
  int iBaseCol = (*iter)->getColonne() ;
  iter++ ;

  for ( ; _PatPathoArray.end() != iter ; iter++)
  {
    if ((*iter)->getColonne() == iBaseCol + 1)
    {
      string sLexique = (*iter)->getLexique() ;

      if (_pQoLInformation->getScoreLexique() == sLexique)
        return iter ;
    }
  }

  return (PatPathoIter) 0 ;
}

PatPathoIter
NSCoopChartDocument::getPptScore()
{
  PatPathoIter pptIter = getPptLocationForScore() ;
  if (NULL == pptIter)
    return NULL ;

  int iBaseCol = (*pptIter)->getColonne() ;

  pptIter++ ;

  if (_PatPathoArray.end() == pptIter)
    return NULL ;

  string sLexSens = (*pptIter)->getLexiqueSens() ;

  if (string("£N0") == sLexSens)
    return pptIter ;

  return NULL ;
}

NSCoopCard*
NSCoopChartDocument::getCardForLexique(string sLexique)
{
  NSCoopCardArray* pCards = _pQoLInformation->getCards() ;
  if ((NULL == pCards) || pCards->empty())
    return (NSCoopCard*) 0 ;

  for (CoopCardIter cardIt = pCards->begin() ; pCards->end() != cardIt ; cardIt++)
    if ((*cardIt)->getSpecific() == sLexique)
      return *cardIt ;

  return (NSCoopCard*) 0 ;
}

NSCoopCard*
NSCoopChartDocument::getGenericCardForCode(string sLexique, string sCode)
{
  NSCoopCardArray* pCards = _pQoLInformation->getCards() ;
  if ((NULL == pCards) || pCards->empty())
    return (NSCoopCard*) 0 ;

  for (CoopCardIter cardIt = pCards->begin() ; pCards->end() != cardIt ; cardIt++)
    if (((*cardIt)->getGeneric() == sLexique) && ((*cardIt)->getCode() == sCode))
      return *cardIt ;

  return (NSCoopCard*) 0 ;
}

int
NSCoopChartDocument::calculateGlobalScore()
{
  if (string("") == _pQoLInformation->getScoreEquation())
    return -1 ;

  NSCoopCardArray* pCards = _pQoLInformation->getCards() ;
  if ((NULL == pCards) || pCards->empty())
    return -1 ;

  // Global score is cards mean value
  //
  if (string("_mean_") == _pQoLInformation->getScoreEquation())
  {
    int iCardNb     = 0 ;
    int iSetCardNb  = 0 ;
    int iTotalScore = 0 ;

    for (CoopCardIter cardIt = pCards->begin() ; pCards->end() != cardIt ; cardIt++)
    {
      iCardNb++ ;
      if ((*cardIt)->iCardSet())
      {
        iTotalScore += (*cardIt)->getScore() ;
        iSetCardNb++ ;
      }
    }

    return (int) arrondiInt(double(iTotalScore) / (double) iSetCardNb) ;
  }

  return -1 ;
}

// -----------------------------------------------------------------------------
//
// NSCoopChartView methods//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSCoopChartView, NSMUEView)
	EV_WM_CLOSE,
  EV_WM_SIZE,
  EV_WM_PAINT,
  EV_WM_SETFOCUS,
  EV_WM_KEYUP,
  EV_WM_TIMER,
  EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE ;

NSCoopChartView::NSCoopChartView(NSCoopChartDocument& doc, TWindow *parent)
                :NSMUEView(doc.pContexte, &doc, parent, "CoopChart", "CoopChartDoc")
{
  _pCoopDoc          = &doc ;
  _pCardWindowsArray = new NSCoopCardWindowArray() ;
  _pCurrentWindow    = 0 ;

  _iMaxCardWidth     = 0 ;
  _iMaxCardHeight    = 0 ;

  _iTopMargin        = 5 ;
  _iLeftMargin       = 5 ;  _iTopDelta         = 3 ;  _iLeftDelta        = 3 ;

  uButtonsStyle = MYWS_ICONS ;

  setViewName() ;
}

NSCoopChartView::~NSCoopChartView()
{
}

void
NSCoopChartView::SetupWindow()
{
  NSMUEView::SetupWindow() ;
  Parent->SetCaption(sViewName.c_str()) ;

  NSCoopCardArray* pCardsArray = _pCoopDoc->getCards() ;
  if (pCardsArray->empty())
    return ;

  NSCoopCardWindow *pPreviousCard = 0 ;

  for (CoopCardIter cardsIt = pCardsArray->begin() ; pCardsArray->end() != cardsIt ; cardsIt++)
  {
    NSCoopCardWindow* pCardWindow = new NSCoopCardWindow(pContexte, this, *cardsIt) ;

    _pCardWindowsArray->push_back(pCardWindow) ;
    pCardWindow->Create() ;

    pCardWindow->Show(SW_HIDE) ;

    if (pCardWindow->getFixedWidth() > _iMaxCardWidth)
      _iMaxCardWidth = pCardWindow->getFixedWidth() ;
    if (pCardWindow->getFixedHeight() > _iMaxCardHeight)
      _iMaxCardHeight = pCardWindow->getFixedHeight() ;

    if (pPreviousCard)
      SetWindowPos(pPreviousCard->GetHandle(), pCardWindow->GetWindowRect(), SWP_NOMOVE | SWP_NOSIZE) ;

    pPreviousCard = pCardWindow ;
  }

  _pCurrentWindow = *(_pCardWindowsArray->begin()) ;
  _pCurrentWindow->Show(SW_SHOWNORMAL) ;

  SetWindowPosit() ;
}

void
NSCoopChartView::CmOk()
{
  setCardsScores() ;

  _pCoopDoc->savePatpatho() ;
}

void
NSCoopChartView::CmCancel()
{
}

void
NSCoopChartView::setViewName()
{
  string sLang = pContexte->getUserLanguage() ;

  string sRootLabel   = string("") ;
  string sRootConcept = _pCoopDoc->getRootConcept() ;  pContexte->getDico()->donneLibelle(sLang, &sRootConcept, &sRootLabel) ;

  if (string("") != sRootLabel)
  {
    sRootLabel[0] = pseumaj(sRootLabel[0]) ;
    sViewName = sRootLabel ;
  }
  else
	  sViewName = pContexte->getSuperviseur()->getText("coopCharts", "coopCharts") ;

  addConcernTitle() ;
}

void
NSCoopChartView::EvClose()
{
	CloseWindow() ;
}

bool
NSCoopChartView::CanClose()
{
	return true ;
}// fonction EVSetFocusvoid
NSCoopChartView::EvSetFocus(HWND hWndLostFocus)
{
	NSMUEView::EvSetFocus(hWndLostFocus);

  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (GetWindow() != pMyApp->GetToolBarWindow())
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}

  focusFct() ;

  if (_pCurrentWindow)
  {
    _pCurrentWindow->BringWindowToTop() ;
    _pCurrentWindow->SetFocus() ;
  }
}

void
NSCoopChartView::focusFct()
{
	activateMUEViewMenu() ;

  TMyApp  *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
    SetupToolBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }

  _pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;
  pContexte->setAideIndex("") ;
  pContexte->setAideCorps("coopChart.htm") ;
}
voidNSCoopChartView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  NSMUEView::EvSize(sizeType, size) ;

  if (_pCardWindowsArray->empty())
    return ;

  int iDecalX = _iLeftMargin ;
  int iDecalY = _iTopMargin ;

  ClassLib::TRect rect = GetClientRect() ;

  for (CoopCardWindowIter cardsIt = _pCardWindowsArray->begin() ; _pCardWindowsArray->end() != cardsIt ; cardsIt++)
  {
    int iX = rect.X() + iDecalX ;
    int iY = rect.Y() + iDecalY ;
    NS_CLASSLIB::TPoint topLeft     = NS_CLASSLIB::TPoint(iX, iY) ;
    NS_CLASSLIB::TPoint bottomRight = NS_CLASSLIB::TPoint(iX + (*cardsIt)->getFixedWidth(), iY + (*cardsIt)->getFixedHeight()) ;

    (*cardsIt)->MoveWindow(ClassLib::TRect(topLeft, bottomRight), true) ;

    iDecalX += _iLeftDelta ;
    iDecalY += _iTopDelta ;
  }
}voidNSCoopChartView::cardJustValidated(){  // Redraw, so that the validated status becomes visible to the user  //  _pCurrentWindow->Invalidate() ;  // Starts timer before switching to next card - (visible feedback)  //  SetTimer(ID_COOPTIMER, 1000) ;}voidNSCoopChartView::switchToNextWindow(){  if (_pCardWindowsArray->empty())    return ;  CoopCardWindowIter cardsIt = _pCardWindowsArray->begin() ;  for ( ; _pCardWindowsArray->end() != cardsIt ; cardsIt++)    if (*cardsIt == _pCurrentWindow)      break ;  if (_pCardWindowsArray->end() == cardsIt)    return ;  cardsIt++ ;  if (_pCardWindowsArray->end() == cardsIt)    return ;  _pCurrentWindow = *cardsIt ;  cardChanged() ;}voidNSCoopChartView::switchToPreviousWindow(){  if (_pCardWindowsArray->empty())    return ;  CoopCardWindowIter cardsIt = _pCardWindowsArray->begin() ;  if (*cardsIt == _pCurrentWindow)    return ;  CoopCardWindowIter previousIt = cardsIt ;  cardsIt++ ;  for ( ; _pCardWindowsArray->end() != cardsIt ; cardsIt++)    if (*cardsIt == _pCurrentWindow)      break ;    else      previousIt = cardsIt ;  _pCurrentWindow = *previousIt ;  cardChanged() ;}voidNSCoopChartView::setCardsScores(){  if (_pCardWindowsArray->empty())    return ;  for (CoopCardWindowIter cardsIt = _pCardWindowsArray->begin() ; _pCardWindowsArray->end() != cardsIt ; cardsIt++)    (*cardsIt)->setCardScore() ;}voidNSCoopChartView::EvPaint(){  NSMUEView::EvPaint() ;  _pCurrentWindow->BringWindowToTop() ;  _pCurrentWindow->Invalidate() ;}voidNSCoopChartView::SetupToolBar()
{
  _pPaneSplitter->FlushControlBar() ;

  _pPaneSplitter->Insert(*new TButtonGadget(IDC_OK_PANESPLIT,     IDC_OK_PANESPLIT,     TButtonGadget::Command)) ;
  _pPaneSplitter->Insert(*new TButtonGadget(IDC_CANCEL_PANESPLIT, IDC_CANCEL_PANESPLIT, TButtonGadget::Command)) ;
  _pPaneSplitter->Insert(*new TButtonGadget(IDC_HELP_PANESPLIT,   IDC_HELP_PANESPLIT,   TButtonGadget::Command)) ;

  _pPaneSplitter->LayoutSession() ;
}

void
NSCoopChartView::SetWindowPosit()
{
	// Parent->Parent->Show(SW_HIDE) ;
  NS_CLASSLIB::TRect rectDlg = Parent->Parent->GetWindowRect() ;//coordonnées par % à l'écran
  int     X,Y,W,H ;
  string  sTaille ;
  NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y()) ;
  Parent->Parent->ScreenToClient(point) ;
  // X = rectDlg.X() ;
  // Y = rectDlg.Y() ;
  X = 10 ;
  Y = 10 ;

  int iNbDelta = _pCardWindowsArray->size() - 1 ;

  W = rectDlg.Width()  + _iMaxCardWidth  + _iLeftMargin /* + _iLeftDelta * iNbDelta */ ;
  H = rectDlg.Height() + _iMaxCardHeight + _iTopMargin  /* + _iTopDelta * iNbDelta  */ ;

  NS_CLASSLIB::TRect rectGadget = _pPaneSplitter->getGadgetWindow()->GetWindowRect() ;
  H += rectGadget.Height() ;

  // set new location
  //

  Parent->Parent->Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
  /*
	Parent->Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
  Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
  SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
  pPaneSplitter->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
  pPaneSplitter->Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
  */
  // Parent->Parent->Show(SW_SHOWNORMAL) ;
}

void
NSCoopChartView::cardChanged()
{
  _pCurrentWindow->BringWindowToTop() ;

  // Redraw other windows in order to have the previously selected become regular
  //
  for (CoopCardWindowIter cardsIt = _pCardWindowsArray->begin() ; _pCardWindowsArray->end() != cardsIt ; cardsIt++)
    (*cardsIt)->Show(SW_HIDE) ;

  _pCurrentWindow->Show(SW_SHOWNORMAL) ;
}

void
NSCoopChartView::CmHelp()
{
  pContexte->NavigationAideEnLigne() ;
}

void
NSCoopChartView::EvTimer(uint timerId)
{
	if (ID_COOPTIMER != timerId)
		return ;

  KillTimer(ID_COOPTIMER) ;

	switchToNextWindow() ;
}

// -----------------------------------------------------------------------------
//
// NSCoopCardWindow methods//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSCoopCardWindow, TWindow)
  EV_WM_KEYDOWN,
  EV_WM_LBUTTONUP,   // bouton gauche relaché
END_RESPONSE_TABLE;

NSCoopCardWindow::NSCoopCardWindow(NSContexte* pCtx, NSCoopChartView* pMainView, NSCoopCard* pCard)
                 :TWindow(pMainView), NSRoot(pCtx)
{
try
{
  _pMainView    = pMainView ;
  _pBackBmp     = (OWL::TDib *) 0 ;
  _iFixedWidth  = 0 ;
  _iFixedHeight = 0 ;
  _pCoopCard    = pCard ;
  _iScore       = 0 ;

  if (pCard)
  {
    string sFileName = pCard->getFileName() ;
    if (string("") != sFileName)
    {
      bool bFileExist = NsFileExists2(sFileName) ;
      if (false == bFileExist)
      {
        string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "missingFile") + string(" ") + sFileName ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      }
      else
      {
        _pBackBmp = new TDib(sFileName.c_str()) ;

        _iFixedWidth  = _pBackBmp->Width() ;
        _iFixedHeight = _pBackBmp->Height() ;
        if (_iFixedHeight < 0)
          _iFixedHeight = -_iFixedHeight ;
      }
    }

    _iScore = pCard->getScore() ;
  }

  SetWindowPos(0, 10, 10, _iFixedWidth, _iFixedHeight, SWP_NOZORDER) ;
}
catch (...)
{
  erreur("Exception NSCoopCardWindow ctor.", standardError, 0) ;
}
}

NSCoopCardWindow::~NSCoopCardWindow()
{
  if (_pBackBmp)
    delete _pBackBmp ;
}

void
NSCoopCardWindow::SetupWindow()
{
  TWindow::SetupWindow() ;

  SetCaption(_pCoopCard->getTitle().c_str()) ;
}

void
NSCoopCardWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  NSCoopCardButton *pHitButton = hitButtonTest(point) ;
  if (pHitButton)
  {
    _iScore = pHitButton->getScore() ;
    setCheck(_iScore) ;

    _pMainView->cardJustValidated() ;
  }
}

void
NSCoopCardWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  switch(key)
  {
    case VK_F1 :
		  _pMainView->CmHelp() ;
      break ;

    case VK_PRIOR :
      _pMainView->switchToPreviousWindow() ;
      break;

    case VK_NEXT:      _pMainView->switchToNextWindow() ;
      break ;
  }

	TWindow::EvKeyDown(key, repeatCount, flags) ;
}

NSCoopCardButton*
NSCoopCardWindow::hitButtonTest(NS_CLASSLIB::TPoint& point)
{
  if ((NULL == _pCoopCard) || (_pCoopCard->isEmpty()))
    return NULL ;

  return _pCoopCard->hitButtonTest(&point) ;
}

void
NSCoopCardWindow::Paint(TDC& Dc, bool, NS_CLASSLIB::TRect& RectangleAPeindre)
{  if (NULL == _pBackBmp)    return ;  // Draw the bitmap  //  NS_CLASSLIB::TPoint dibOrigin ;
  // Bottom up dib : origin lies at the lower left corner
  //
  if (_pBackBmp->Height() > 0)
  {
    // bBottomUp = true ;
    dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
  }
  //
  // top down dib : origin lies at the upper left corner
  //
  else
  {
    // bBottomUp = false ;
    dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
  }

  // Using BitBlt
  //
  HDC hdcMem = CreateCompatibleDC(Dc.GetHDC()) ;

  HBITMAP bitmap = ::CreateDIBitmap(Dc,
                                    (LPBITMAPINFOHEADER)_pBackBmp->GetInfoHeader(),
                                    CBM_INIT,
                                    (const uint8 far*)_pBackBmp->GetBits(),
                                    (LPBITMAPINFO)_pBackBmp->GetInfo(),
                                    _pBackBmp->Usage()) ;
  if (NULL == bitmap)
  {
    DeleteDC(hdcMem) ;
    return ;
  }

  HGDIOBJ hbmOld = SelectObject(hdcMem, bitmap) ;

/*
  BITMAP bm ;
  GetObject(bitmap, sizeof(bm), &bm) ;
  int x = bm.bmWidth ;
  int y = bm.bmHeight ;
  TransparentBlt(Dc, 0, 0, _iFixedWidth, _iFixedHeight, hdcMem, 0, 0, x, y, RGB(255, 0, 255)) ;
*/

  Dc.BitBlt(RectangleAPeindre,
            hdcMem,
            RectangleAPeindre.TopLeft()) ;

  // Draw selected button
  //
  if (_iScore > 0)
  {
    NSCoopCardButton* pButton = _pCoopCard->getButtonForScore(_iScore) ;
    if (pButton)
    {
      Dc.BitBlt(*(pButton->getButtonRect()),
                hdcMem,
                pButton->getButtonRect()->TopLeft(),
                DSTINVERT) ;
    }
  }

  DeleteObject(bitmap) ;

  SelectObject(hdcMem, hbmOld) ;

  // Draw the rectangle around
  //
  HPEN oldPen = (HPEN) Dc.GetCurrentObject(OBJ_PEN) ;

  OWL::TPen *pPen = 0 ;
  if (this == _pMainView->getCurrentCard())
    pPen = new OWL::TPen(NS_CLASSLIB::TColor::LtBlue, /* int width */ 1 /*, int style=PS_SOLID*/) ;
  else
    pPen = new OWL::TPen(NS_CLASSLIB::TColor::Gray, /* int width */ 1 /*, int style=PS_SOLID*/) ;
  Dc.SelectObject(*pPen) ;

  Dc.MoveTo(0, 0) ;
  Dc.LineTo(_iFixedWidth-1, 0) ;
  Dc.LineTo(_iFixedWidth-1, _iFixedHeight-1) ;
  Dc.LineTo(0, _iFixedHeight-1) ;
  Dc.LineTo(0, 0) ;

  delete pPen ;

  Dc.SelectObject(oldPen) ;

  DeleteDC(hdcMem) ;
}

void
NSCoopCardWindow::setCardScore()
{
  if (NULL == _pCoopCard)
    return ;

  _pCoopCard->setScore(_iScore) ;

}

void
NSCoopCardWindow::setCheck(int iScore)
{
  _pCoopCard->setScoreIsSet(true) ;
  Invalidate() ;
}

