// -----------------------------------------------------------------------------// NS_HTML.CPP
// RS Juin 97
// Génération de fichiers HTML à partir d'un compte-rendu
// -----------------------------------------------------------------------------
#define __NS_HTML_CPP

#include <stdio.h>
#include <classlib\filename.h>

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nssuper.h"
#include "nsbb\nsutidlg.h"

#include "nautilus\ns_html.h"

#include "partage\nsdivfct.h"
#include "nautilus\nsresour.h"

#include "partage\nsperson.h"     // pour l'utilisateur  (NSHtmlCR)
#include "nautilus\nscrdoc.h"		  // pour récupérer sFichDecod  (NSHtmlCR)
#include "nautilus\nscsdoc.h"		  // pour récupérer le html de consultation (NSHtmlCS)
#include "nautilus\nsttx.h"		    // pour la constante MAXCARS (taille max RTF)
#include "nautilus\nsdocaga.h"
#include "nautilus\nsperary.h"
#include "nautilus\nsrechdl.h"
#include "nautilus\nscqdoc.h"
#include "nautilus\nscompub.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsCoopChartDV.h"
#include "nsldv\nsldvvar.h"
#include "nsldv\nsldvgoal.h"
#include "nsldv\nsldvdrug.h"
#include "dcodeur\nsdkd.h"
#include "dcodeur\decoder.h"
#include "nsbb\nsmanager.h"

#define ESC_CHAR 27
#define FS_CHAR  28

NSHtmlHistoIndex::NSHtmlHistoIndex(OperationType typeOp, NSContexte *pCtx)
                 :NSModHtml(typeOp, (NSNoyauDocument*) 0, pCtx)
{
}

NSHtmlHistoIndex::~NSHtmlHistoIndex()
{
}

void
NSHtmlHistoIndex::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

void
NSHtmlHistoIndex::ajouteLigne(string sDocLabel, string sDocExportFile, string sDocId, string sDocDate, string sDocName)
{
  ligneBlocHtml.vider() ;

  string sCompleteDocName = sDocName ;
  if (string("") != sDocDate)
    sCompleteDocName += string(" - ") + sDocDate ;

  ajouteBloc(new NSHtml("TBLIndexDocLabel", texteHtml(sCompleteDocName))) ;
  ajouteBloc(new NSHtml("TBLIndexDocFile",  texteHtml(sDocExportFile))) ;
  ajouteBloc(new NSHtml("TBLIndexDocId",    texteHtml(sDocId))) ;
  ajouteBloc(new NSHtml("TBLIndexDocDate",  texteHtml(sDocDate))) ;
  ajouteBloc(new NSHtml("TBLIndexDocName",  texteHtml(sDocName))) ;

  _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;

  bExisteTable = true ;
}

// -----------------------------------------------------------------------------
// Classe NSHtmlCS
//
// On récupère le html correspondant à l'état en cours de la TreeView (pHtmlCS)
// -----------------------------------------------------------------------------

NSHtmlCS::NSHtmlCS(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
         :NSModHtml(typeOp, pDocNoy, pCtx)
{
	NSCSDocument *pCSDoc = dynamic_cast<NSCSDocument *>(pDocNoy) ;
  if ((NSCSDocument*) NULL == pCSDoc)
    return ;

  string sIntitule   = string("") ;
  string sIntituleNB = string("") ;
  string sTitre      = string("") ;
	string sCreateur   = string("") ;
	string sDate       = string("") ;
	string sSignature  = string("") ;

	// on récupère le titre du document (au sens TDocument)
  if (pCSDoc->GetTitle())
	  sTitre = string(pCSDoc->GetTitle()) ;
  else
    sTitre = string("") ;

  // on prend la date du document et l'intitule
  sDate       = pCSDoc->GetDateDoc() ;
  sIntitule   = pCSDoc->InitIntitule() ;
  sIntituleNB = pCSDoc->InitIntitule() ;

  ajoute(tTitle,     sTitre) ;
  ajoute(tOperateur, sCreateur) ;
  ajoute(tDate,      sDate) ;
  ajoute(tDest,      sDest) ;
  ajoute(tIntitule,  sIntitule) ;

  if (pCSDoc->_pHtmlCS)
  {
    NSHtml* pAsHtml = pCSDoc->getAsHtml() ;
    if (pAsHtml)
      ajoute(new NSHtml(*pAsHtml)) ;
  }

  addSignature() ;
  addPatient() ;
}

NSHtmlCS::~NSHtmlCS()
{
}

// -----------------------------------------------------------------------------
// Classe NSHtmlCQ
//
// On récupère le html inclus dans l'archetype associé
// -----------------------------------------------------------------------------

NSHtmlCQ::NSHtmlCQ(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
         :NSModHtml(typeOp, pDocNoy, pCtx)
{
	string sTitre     = string("") ;
	string sCreateur  = string("") ;
	string sDate      = string("") ;
	string sIntitule  = string("") ;

  if (pDocNoy)
  {
		NSCQDocument *pCQDoc = dynamic_cast<NSCQDocument*>(pDocNoy) ;
    if (pCQDoc)
    {
			// on génère le html à visualiser
			pCQDoc->genereHtmlView() ;

			// on récupère le titre du document (au sens TDocument)
      if (pCQDoc->GetTitle())
			  sTitre = string(pCQDoc->GetTitle()) ;
      else
        sTitre = string("") ;

  		// on prend la date du document et l'intitule
			sDate     = pCQDoc->GetDateDoc() ;
  		sIntitule = pCQDoc->InitIntitule() ;

    	ajoute(new NSHtml(tCQ, pCQDoc->getHtmlView())) ;
    }
	}

  ajoute(tTitle,     sTitre) ;
  ajoute(tOperateur, sCreateur) ;
  ajoute(tDate,      sDate) ;
  ajoute(tDest,      sDest) ;
  ajoute(tIntitule,  sIntitule) ;
  addSignature() ;
  addPatient() ;
}

NSHtmlCQ::~NSHtmlCQ()
{
}

// -----------------------------------------------------------------------------
// Classe NSHtmlHisto
//
// On récupère le html correspondant à l'état en cours de la TreeView (pHtmlCS)
// -----------------------------------------------------------------------------

NSHtmlHisto::NSHtmlHisto(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
            :NSModHtml(typeOp, pDocNoy, pCtx)
{
  string              sIntro, sMessage ;
  char 	              cDate[9] ;
  TDate 	            dateSys ;
  NSHistoRefDocument  *pHistoDoc = dynamic_cast<NSHistoRefDocument *>(pDocNoy) ;

	string sLang  = pContexte->getUserLanguage() ;
  string sTitre = string("Historique") ;

  sprintf(cDate,"%4d%02d%02d", (int) dateSys.Year(), (int) dateSys.Month(), (int) dateSys.DayOfMonth()) ;
  string sDate = string(cDate) ;
  donne_date_claire(sDate, &sMessage, &sIntro, sLang) ;

  if (string("") != sIntro)
    sDate = sIntro + string(" ") + sMessage ;
  else
    sDate = sMessage ;

  if      (pHistoDoc->sImportance == "A")
    sDate += " (global)" ;
  else if (pHistoDoc->sImportance == "B")
    sDate += " (partiel : éléments d'importance 1+)" ;
  else if (pHistoDoc->sImportance == "C")
    sDate += " (partiel : éléments d'importance 2+)" ;
  else if (pHistoDoc->sImportance == "D")
    sDate += " (partiel : éléments d'importance 3+)" ;
  else if (pHistoDoc->sImportance == "E")
    sDate += " (partiel : éléments d'importance 4+)" ;

  ajoute(tTitle, sTitre) ;
  ajoute(new NSHtml(string("nomPatient"), texteHtml(pContexte->getPatient()->getNomLong()))) ;
  ajoute(tDate,  sDate) ;  ajoute(tDest,  sDest) ;
  ajoute(new NSHtml(*(pHistoDoc->pHtmlCS))) ;
}

NSHtmlHisto::~NSHtmlHisto()
{
}

NSHtmlQoL::NSHtmlQoL(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
          :NSModHtml(typeOp, pDocNoy, pCtx)
{
  ajoute(new NSHtml(string("nomPatient"), texteHtml(pContexte->getPatient()->getNomLong()))) ;
  ajoute(tDest, sDest) ;

  NSCoopChartDocument *pQoLDoc = dynamic_cast<NSCoopChartDocument *>(pDocNoy) ;
  if (pQoLDoc)
    buildHtml(pQoLDoc) ;
}

NSHtmlQoL::~NSHtmlQoL()
{
}

void
NSHtmlQoL::buildHtml(NSCoopChartDocument *pQoLDoc)
{
  if (NULL == pQoLDoc)
    return ;

  string sLang = string("") ;
  if (pContexte)
    sLang = pContexte->getUserLanguage() ;

  // Global information
  //
  addSignature() ;
  addPatient() ;
  if (pQoLDoc->GetTitle())     // (au sens TDocument)
    ajoute(tTitle, string(pQoLDoc->GetTitle())) ;
  else
    ajoute(tTitle, string("")) ;
  ajoute(tDate,     pQoLDoc->GetDateDoc()) ;
  ajoute(tIntitule, pQoLDoc->InitIntitule()) ;

  // Global score
  //
  PatPathoIter pptIter = pQoLDoc->getPptScore() ;
  if (pptIter)
  {
    // Score label
    //
    string sScoreLabel = string("") ;

    PatPathoIter pptScoreIter = pQoLDoc->getPptLocationForScore() ;
    if (pptScoreIter)
    {
      string sCode = (*pptScoreIter)->getLexique() ;

      string sLabel = string("") ;
      if (string("") != sCode)
        pContexte->getDico()->donneLibelle(sLang, &sCode, &sLabel) ;

      sScoreLabel = sLabel ;
    }

    // Score value
    //
    gereNum num(pContexte->getSuperviseur(), sLang) ;

    string sFormat = (*pptIter)->getLexique() ;
    string sValeur = (*pptIter)->getComplement() ;
    string sUnite  = (*pptIter)->getUnit() ;

    num.instancier(&sValeur, &sUnite, &sFormat) ;

    if (string("") != sScoreLabel)
      sScoreLabel += string(" ") ;

    sScoreLabel += num.getNum() ;

    string sUnitCode  = num.getUnite() ;
    if (string("") != sUnitCode)
    {
      string sUnitLabel = string("") ;
      pContexte->getDico()->donneLibelle(sLang, &sUnitCode, &sUnitLabel) ;
      if ('/' != sUnitLabel[0])
        sScoreLabel += string(" ") ;
      sScoreLabel += sUnitLabel ;
    }

    sScoreLabel[0] = pseumaj(sScoreLabel[0]) ;

    ajoute(new NSHtml(string("QOLScore"), texteHtml(sScoreLabel))) ;
  }
  else
    ajoute(new NSHtml(string("QOLScore"), string(""))) ;

  // Cards
  //
  NSCoopCardArray* pCards = pQoLDoc->getCards() ;

  if (((NSCoopCardArray*) NULL == pCards) || pCards->empty())
    return ;

  for (CoopCardIter cardIt = pCards->begin() ; pCards->end() != cardIt ; cardIt++)
  {
    int iCardScore = (*cardIt)->getScore() ;

    if (iCardScore > 0)
    {
      NSBlocHtmlArray* pTableRow = new NSBlocHtmlArray() ;

      string sSelectedButtonText = string("?") ;
      NSCoopCardButton* pSelectedButton = (*cardIt)->getButtonForScore(iCardScore) ;
      if (pSelectedButton)
        sSelectedButtonText = pSelectedButton->getLabel() ;
      sSelectedButtonText = texteHtml(sSelectedButtonText) ;

      string sQuestion = texteHtml((*cardIt)->getLabel()) ;

      pTableRow->push_back(new NSBlocHtml(new NSHtml("TBLCardQuestion", sQuestion), 1)) ;
      pTableRow->push_back(new NSBlocHtml(new NSHtml("TBLCardAnswer", sSelectedButtonText), 1)) ;

      _table.push_back(pTableRow) ;
    }
  }

  bExisteTable = true ;
}

// -----------------------------------------------------------------------------
// Classe NSHtmlGoals
//
// On instancie le html correspondant à la liste des actes
// -----------------------------------------------------------------------------

NSHtmlGoals::NSHtmlGoals(OperationType typeOp, NSLdvDocument *pDocNoy, NSContexte *pCtx, NSLdvPubli *pPubliDriver, string sDest)
            :NSModHtml(typeOp, pDocNoy, pCtx)
{
  _pPubliDriver = pPubliDriver ;

	string sTitre = string("") ;

  string sLang  = pContexte->getUserLanguage() ;
  string sToday = donne_date(donne_date_duJour(), sLang) ;

  ajoute(new NSHtml(string("titre"), texteHtml(sTitre))) ;
  ajoute(new NSHtml(string("date"),  texteHtml(sToday))) ;
}

NSHtmlGoals::~NSHtmlGoals()
{
}

void
NSHtmlGoals::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

void
NSHtmlGoals::ajouteLignes()
{
  if ((NULL == _pPubliDriver) || (NULL == _pPubliDriver->_pPrintArray))
    return ;

  NSObjectifArray *pGoalsArray = static_cast<NSObjectifArray*>(_pPubliDriver->_pPrintArray) ;
  if (((NSObjectifArray*) NULL == pGoalsArray) || (pGoalsArray->empty()))
    return ;

  string sLang = pContexte->getUserLanguage() ;

  string sCyclic = texteHtml(pContexte->getSuperviseur()->getText("goalsManagement", "cyclic")) ;
  string sPerman = texteHtml(pContexte->getSuperviseur()->getText("goalsManagement", "permanent")) ;
  string sOnce   = texteHtml(pContexte->getSuperviseur()->getText("goalsManagement", "once")) ;

  for (NSObjectifIter itGoal = pGoalsArray->begin() ; pGoalsArray->end() != itGoal ; itGoal++)
	{
    ligneBlocHtml.vider() ;

    string sData = texteHtml((*itGoal)->getName()) ;

    ajouteBloc(new NSHtml("TBLGoalName", sData)) ;

    switch ((*itGoal)->getLevel())
    {
      case NSLdvGoalInfo::AVrouge : sData = string("r") ; break ;
      case NSLdvGoalInfo::AVjaune : sData = string("j") ; break ;
      case NSLdvGoalInfo::AVvert  : sData = string("v") ; break ;
      case NSLdvGoalInfo::Bleu    : sData = string("b") ; break ;
      case NSLdvGoalInfo::APvert  : sData = string("V") ; break ;
      case NSLdvGoalInfo::APjaune : sData = string("J") ; break ;
      case NSLdvGoalInfo::AProuge : sData = string("R") ; break ;
      default                     : sData = string("&nbsp;") ;
    }

    ajouteBloc(new NSHtml("TBLGoalState", sData)) ;

    // Cycle type

    string sCycleType = string("") ;
    switch ((*itGoal)->getRythme())
    {
      case NSLdvGoal::cyclic    : sCycleType = sCyclic ; break ;
      case NSLdvGoal::permanent : sCycleType = sPerman ; break ;
      case NSLdvGoal::ponctuel  : sCycleType = sOnce ;   break ;
      default                   : sCycleType = string("&nbsp;") ;
    }

    ajouteBloc(new NSHtml("TBLGoalCycle", sCycleType)) ;

    for (int iDateType = 0 ; iDateType < 7 ; iDateType++)
    {
      string sDate = string("") ;
      switch(iDateType)
      {
        case 0 : sDate = (*itGoal)->getDateAuto() ;     break ;
        case 1 : sDate = (*itGoal)->getDateCons() ;     break ;
        case 2 : sDate = (*itGoal)->getDateIdeal() ;    break ;
        case 3 : sDate = (*itGoal)->getDateIdealMax() ; break ;
        case 4 : sDate = (*itGoal)->getDateConsMax() ;  break ;
        case 5 : sDate = (*itGoal)->getDateCrit() ;     break ;
        case 6 : sDate = (*itGoal)->getDatePrec() ;     break ;
      }

      string sFormDate = string("") ;
      if ((string("") != sDate) && (strlen(sDate.c_str()) < 20))
        sFormDate = texteHtml(donne_date(sDate, sLang)) ;
      else
        sFormDate = string("&nbsp;") ;

      switch(iDateType)
      {
        case 0 : ajouteBloc(new NSHtml("TBLGoalDateAuto",     sFormDate)) ; break ;
        case 1 : ajouteBloc(new NSHtml("TBLGoalDateCons",     sFormDate)) ; break ;
        case 2 : ajouteBloc(new NSHtml("TBLGoalDateIdeal",    sFormDate)) ; break ;
        case 3 : ajouteBloc(new NSHtml("TBLGoalDateIdealMax", sFormDate)) ; break ;
        case 4 : ajouteBloc(new NSHtml("TBLGoalDateConsMax",  sFormDate)) ; break ;
        case 5 : ajouteBloc(new NSHtml("TBLGoalDateCrit",     sFormDate)) ; break ;
        case 6 : ajouteBloc(new NSHtml("TBLGoalDatePrec",     sFormDate)) ; break ;
      }
    }

    string sPreviousValue = texteHtml((*itGoal)->getValue()) ;
    if (string("") == sPreviousValue)
      sPreviousValue = string("&nbsp;") ;

    ajouteBloc(new NSHtml("TBLGoalPrevValue", sPreviousValue)) ;

    _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;
  }

	bExisteTable = true ;
}

// -----------------------------------------------------------------------------
// Classe NSHtmlDrugs
//
// On instancie le html correspondant à la liste des traitements
// -----------------------------------------------------------------------------

NSHtmlDrugs::NSHtmlDrugs(OperationType typeOp, NSLdvDocument *pDocNoy, NSContexte *pCtx, NSLdvPubli *pPubliDriver, string sDest)
            :NSModHtml(typeOp, pDocNoy, pCtx)
{
  _pPubliDriver = pPubliDriver ;

	string sTitre = string("") ;

  string sLang  = pContexte->getUserLanguage() ;
  string sToday = donne_date(donne_date_duJour(), sLang) ;

  ajoute(new NSHtml(string("titre"), texteHtml(sTitre))) ;
  ajoute(new NSHtml(string("date"),  texteHtml(sToday))) ;
}

NSHtmlDrugs::~NSHtmlDrugs()
{
}

void
NSHtmlDrugs::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

void
NSHtmlDrugs::ajouteLignes()
{
  if (((NSLdvPubli*) NULL == _pPubliDriver) || (NULL == _pPubliDriver->_pPrintArray))
    return ;

  if ((NSLdvDocument*) NULL == _pLdvDoc)
    return ;

  ArrayCopyDrugs *pDrugsArray = static_cast<ArrayCopyDrugs*>(_pPubliDriver->_pPrintArray) ;
  if (((ArrayCopyDrugs*) NULL == pDrugsArray) || (pDrugsArray->empty()))
    return ;

  string sLang = pContexte->getUserLanguage() ;

  for (drugsIter itDrug = pDrugsArray->begin() ; pDrugsArray->end() != itDrug ; itDrug++)
	{
    ligneBlocHtml.vider() ;

    string sData = texteHtml((*itDrug)->_sTitre) ;

    ajouteBloc(new NSHtml("TBLTraitName", sData)) ;

    NSLdvDrugPhase* pPhase = (*itDrug)->getCurrentActivePhase() ;

    NSLdvDrugPhase* pLastPhase = 0 ;
    if ((NSLdvDrugPhase*) NULL == pPhase)
      pLastPhase = (*itDrug)->getLastActivePhase() ;

    NVLdVTemps tpsStart ;
    NVLdVTemps tpsEnd ;

    sData = string("&nbsp;") ;

    if (pPhase)
    {
      sData    = texteHtml(pPhase->_sTitreCourt) ;
      tpsStart = pPhase->_tDateOuverture ;
      tpsEnd   = pPhase->_tDateFermeture ;
    }
    else if (pLastPhase)
    {
      sData    = texteHtml(string("(") + pLastPhase->_sTitreCourt + string(")")) ;
      tpsStart = pLastPhase->_tDateOuverture ;
      tpsEnd   = pLastPhase->_tDateFermeture ;
    }

    ajouteBloc(new NSHtml("TBLTraitDose", sData)) ;

    unsigned long ulDeltaDays = tpsEnd.daysBetween(tpsStart) ;
    if ((unsigned long) 0 != ulDeltaDays)
    {
      string sNumberOf = string("") ;
      string sDeltaDays = IntToString((int)ulDeltaDays) ;

      GlobalDkd Dcode(pContexte, sLang) ;
      string sUnitCode = string("2DAT01") ;
      if (ulDeltaDays > 1)
        sUnitCode += string(1, intranodeSeparationMARK) + string("WPLUR1") ;
      GENRE iGenreObj ;
      int   iCertObj ;
      string sUnitLib = Dcode.decodeMot(&sUnitCode, genreNull, &iGenreObj, &iCertObj) ;

      sNumberOf = sDeltaDays + string(" ") + sUnitLib ;

      if (NULL == pPhase)
        sNumberOf = string("(") + sNumberOf + string(")") ;

      ajouteBloc(new NSHtml("TBLTraitDuration", texteHtml(sNumberOf))) ;
    }
    else
      ajouteBloc(new NSHtml("TBLTraitDuration", string("&nbsp;"))) ;

    if (true == (*itDrug)->_bALD)
      ajouteBloc(new NSHtml("TBLTraitALD", string("ALD"))) ;
    else
      ajouteBloc(new NSHtml("TBLTraitALD", string("non ALD"))) ;

    string sFormDate = string("") ;

    sFormDate = donne_date((*itDrug)->_tDateOuverture.donneDate(), sLang) ;
    ajouteBloc(new NSHtml("TBLTraitStartDate", texteHtml(sFormDate))) ;

    if ((*itDrug)->_tDateFermeture.estNoLimit())
      sFormDate = texteHtml(string("...")) ;
    else
      sFormDate = donne_date((*itDrug)->_tDateFermeture.donneDate(), sLang) ;
    ajouteBloc(new NSHtml("TBLTraitCloseDate", texteHtml(sFormDate))) ;

    if (pPhase)
    {
      sFormDate = string("") ;

      if (false == pPhase->_tDateFermeture.estVide())
      {
        NVLdVTemps tpsNow ;
        tpsNow.takeTime() ;

        GlobalDkd Dcode(pContexte, sLang) ;

        string sNumberOf = string("") ;
        unsigned long ulDeltaDays = pPhase->_tDateFermeture.daysBetween(tpsNow) ;
        if (ulDeltaDays > 90)
        {
          unsigned long ulDeltaMonths = pPhase->_tDateFermeture.monthsBetween(tpsNow) ;
          string sDeltaMonths = IntToString((int)ulDeltaMonths) ;
          string sUnitCode = string("2DAT21") ;
          if (ulDeltaMonths > 1)
            sUnitCode += string(1, intranodeSeparationMARK) + string("WPLUR1") ;
          GENRE iGenreObj ;
          int   iCertObj ;
          string sUnitLib = Dcode.decodeMot(&sUnitCode, genreNull, &iGenreObj, &iCertObj) ;
          sNumberOf = sDeltaMonths + string(" ") + sUnitLib ;
        }
        else
        {
          string sDeltaDays = IntToString((int)ulDeltaDays) ;
          string sUnitCode = string("2DAT01") ;
          if (ulDeltaDays > 1)
            sUnitCode += string(1, intranodeSeparationMARK) + string("WPLUR1") ;
          GENRE iGenreObj ;
          int   iCertObj ;
          string sUnitLib = Dcode.decodeMot(&sUnitCode, genreNull, &iGenreObj, &iCertObj) ;
          sNumberOf = sDeltaDays + string(" ") + sUnitLib ;
        }

        sFormDate = donne_date(pPhase->_tDateFermeture.donneDate(), sLang) ;

        if (string("") != sNumberOf)
          sFormDate = sNumberOf + string(" (") + sFormDate + string(")") ;
      }
      ajouteBloc(new NSHtml("TBLTraitRemains", texteHtml(sFormDate))) ;
    }
    else
      ajouteBloc(new NSHtml("TBLTraitRemains", string("&nbsp;"))) ;

    string sPrescriptionDate = _pLdvDoc->getPrescriptionDate(*itDrug) ;
    if (string("") != sPrescriptionDate)
    {
      sFormDate = donne_date(sPrescriptionDate, sLang) ;
      ajouteBloc(new NSHtml("TBLTraitPrescribed", texteHtml(sFormDate))) ;
    }
    else
    {
      string sText = pContexte->getSuperviseur()->getText("drugManagement", "notPrescribed") ;
      ajouteBloc(new NSHtml("TBLTraitPrescribed", texteHtml(sText))) ;
    }

    _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;
  }

	bExisteTable = true ;
}

// -----------------------------------------------------------------------------
//		Classe NSHtmlSecu//
// Pour l'impression des feuilles de soins à partir des données patient
// -----------------------------------------------------------------------------


NSHtmlSecu::NSHtmlSecu(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx)
           :NSModHtml(typeOp, pDocNoy, pCtx)
{
  NSSecuRefDocument *pSecuDoc = dynamic_cast<NSSecuRefDocument *>(pDocNoy) ;

  // Compatibilité CERFA 10441*2

  // on instancie d'abord les tags liés au malade
  ajoute(new NSHtml(string("nomMalade"),    texteHtml(pSecuDoc->pCarteMalade->pDonnees->nomPatro))) ;
  ajoute(new NSHtml(string("prenomMalade"), texteHtml(pSecuDoc->pCarteMalade->pDonnees->prenom))) ;
  if (pSecuDoc->bNonAssure)
  {
    ajoute(new NSHtml(string("numMalade"), string(""))) ;
    ajoute(new NSHtml(string("numAssu"),   texteHtml(pSecuDoc->pCarteMalade->pDonnees->immatricul))) ;
  }
  else
  {
    ajoute(new NSHtml(string("numMalade"), texteHtml(pSecuDoc->pCarteMalade->pDonnees->immatricul))) ;
    ajoute(new NSHtml(string("numAssu"),   string(""))) ;
  }

  if (strcmp(pSecuDoc->pCarteMalade->pDonnees->dateNais, ""))
  {
  	string sLang = pContexte->getUserLanguage() ;

    // on convertit la date de naissance au format jj/mm/aaaa
    string sFormDate = donne_date(string(pSecuDoc->pCarteMalade->pDonnees->dateNais, 0, 8), sLang) ;
    ajoute(new NSHtml(string("dateNaissMalade"), texteHtml(sFormDate))) ;
  }
  else
    ajoute(new NSHtml(string("dateNaissMalade"), string(""))) ;

  ajoute(new NSHtml(string("codeRegime"),  texteHtml(pSecuDoc->pCarte->pDonnees->regime))) ;
  ajoute(new NSHtml(string("codeCaisse"),  texteHtml(pSecuDoc->pCarte->pDonnees->caisse))) ;
  ajoute(new NSHtml(string("codeCentre"),  texteHtml(pSecuDoc->pCarte->pDonnees->centre))) ;
  ajoute(new NSHtml(string("codeGestion"), texteHtml(pSecuDoc->pCarte->pDonnees->codeGest))) ;

  // on instancie ensuite les tags liés à l'assuré
  if (pSecuDoc->bNonAssure)
  {
    ajoute(new NSHtml(string("nomAssu"),    texteHtml(pSecuDoc->pAssuInfo->getNom()))) ;
    ajoute(new NSHtml(string("prenomAssu"), texteHtml(pSecuDoc->pAssuInfo->getPrenom()))) ;
  }
  else    // on remplace les tags par des blancs
  {
    ajoute(new NSHtml(string("nomAssu"),    "")) ;
    ajoute(new NSHtml(string("prenomAssu"), "")) ;
  }
}

NSHtmlSecu::~NSHtmlSecu()
{
}

// -----------------------------------------------------------------------------
// Classe NSHtmlConvoc//
// On instancie le html pour éditer la lettre de reconvocation
// -----------------------------------------------------------------------------

NSHtmlConvoc::NSHtmlConvoc(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
             :NSModHtml(typeOp, pDocNoy, pCtx)
{
  NSConvocRefDocument *pConvocDoc = dynamic_cast<NSConvocRefDocument *>(pDocNoy) ;

  string sTitre  = string("Lettre de reconvocation") ;
  int index   = (pConvocDoc->pTabSelect)[pConvocDoc->getIndexCorresp()] ;

  string sM_Mme ;
  if (((pConvocDoc->pVarConvoc->_aPatientArray)[index])->estMasculin())
    sM_Mme = string("Monsieur") ;  else
    sM_Mme = string("Madame") ;

  string sLang = pContexte->getUserLanguage() ;

  string sDate = ((pConvocDoc->pVarConvoc->_aPatientArray)[index])->getConvoc() ;
  string sIntro, sMessage ;
  donne_date_claire(sDate, &sMessage, &sIntro, sLang) ;
  string sDateConvoc ;
  if (sIntro != "")
    sDateConvoc = sIntro + " " + sMessage ;
  else
    sDateConvoc = sMessage ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  string sDateNow = tpsNow.donneDate() ;
  string sIntroNow, sMessageNow ;
  donne_date_claire(sDateNow, &sMessageNow, &sIntroNow, sLang) ;

  string sDateClaireNow ;
  if (string("") != sIntroNow)
    sDateClaireNow = sIntroNow + string(" ") + sMessageNow ;
  else
    sDateClaireNow = sMessageNow ;

  ajoute(tTitle, sTitre) ;
  ajoute(tDest,  sDest) ;
  ajoute(new NSHtml(string("M_Mme"),      texteHtml(sM_Mme)), 2) ;
  ajoute(new NSHtml(string("dateConvoc"), texteHtml(sDateConvoc))) ;
}

NSHtmlConvoc::~NSHtmlConvoc()
{
}

// -----------------------------------------------------------------------------
// Classe NSHtmlAga
//
// On instancie le html correspondant à l'état des AGA en cours
// -----------------------------------------------------------------------------

NSHtmlAga::NSHtmlAga(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)          :NSModHtml(typeOp, pDocNoy, pCtx)
{
	string sTitre ;
	string sOperateur   = "" ;

  NSAgaRefDocument  *pAgaDoc = dynamic_cast<NSAgaRefDocument *>(pDocNoy) ;
	string sLang = pContexte->getUserLanguage() ;

  string sDateAga1 = donne_date(pAgaDoc->pCriteres->getDateAga1(), sLang) ;  string sDateAga2 = donne_date(pAgaDoc->pCriteres->getDateAga2(), sLang) ;

  if (false == pAgaDoc->bAgaCumules)    // on récupère le titre du document    sTitre = string("Liste des AGA du ") + sDateAga1 ;
  else
    sTitre = string("Liste des recettes du ") + sDateAga1 + string(" au ") + sDateAga2 ;

  if (pAgaDoc->pCriteres->isActesPerso())  {
    sOperateur = string(" pour ") + pContexte->getUtilisateur()->donneTitre(pContexte) ;
    sTitre += sOperateur ;
  }

  ajoute(new NSHtml(string("titre"), texteHtml(sTitre))) ;

  if (false == pAgaDoc->bAgaCumules)
    ajoute(new NSHtml(string("date"), texteHtml(sDateAga1))) ;  else    ajoute(new NSHtml(string("date"), texteHtml(sDateAga1 + string(" au ") + sDateAga2))) ;
  ajoute(new NSHtml(string("beneficiaire"), texteHtml(sOperateur))) ;}

NSHtmlAga::~NSHtmlAga()
{
}

void
NSHtmlAga::initTotaux()
{
  int 		montant ;
  char 		cData[100] ;
  string 	sData ;

  NSAgaRefDocument *pAgaDoc = dynamic_cast<NSAgaRefDocument *>(_pDocBrut) ;

  montant = pAgaDoc->pTotaux->paieLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totPaieLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pTotaux->paieEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totPaieEuro"), texteHtml(sData))) ;

  montant = pAgaDoc->pTotaux->espLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totEspLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pTotaux->espEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totEspEuro"), texteHtml(sData))) ;

	montant = pAgaDoc->pTotaux->chqLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totChqLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pTotaux->chqEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totChqEuro"), texteHtml(sData))) ;

  montant = pAgaDoc->pTotaux->virLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totVirLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pTotaux->virEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totVirEuro"), texteHtml(sData))) ;

  montant = pAgaDoc->pTotaux->cbLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totCbLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pTotaux->cbEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totCbEuro"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->paieLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("parPaieLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->paieEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("parPaieEuro"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->espLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("parEspLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->espEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("parEspEuro"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->chqLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("parChqLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->chqEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("parChqEuro"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->virLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("parVirLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->virEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("parVirEuro"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->cbLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pAgaDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("parCbLoc"), texteHtml(sData))) ;

  montant = pAgaDoc->pPartiels->cbEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("parCbEuro"), texteHtml(sData))) ;
}

void
NSHtmlAga::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

void
NSHtmlAga::ajouteLignes(){
	int     iMontant ;
	char    cMontant[10]  = "" ;
	string  sData ;

	NSAgaRefDocument *pAgaDoc = dynamic_cast<NSAgaRefDocument *>(_pDocBrut) ;
	string sLang    = pContexte->getUserLanguage() ;
	string sDateAga = donne_date(pAgaDoc->pCriteres->getDateAga1(), sLang) ;  if (false == pAgaDoc->pAgaArray->empty())  {	  for (NSAgaIter i = pAgaDoc->pAgaArray->begin() ; pAgaDoc->pAgaArray->end() != i ; i++)	  {
  	  ligneBlocHtml.vider() ;

      if (false == pAgaDoc->bAgaCumules)
      {
        ajouteBloc(new NSHtml("TBLNom",     texteHtml(((*i)->patInfo).getNom()))) ;
        ajouteBloc(new NSHtml("TBLPrenom",  texteHtml(((*i)->patInfo).getPrenom()))) ;
        ajouteBloc(new NSHtml("TBLNomLong", texteHtml(string((*i)->nomPatient)))) ;
        ajouteBloc(new NSHtml("TBLExamen",  texteHtml(string((*i)->actes)))) ;
        ajouteBloc(new NSHtml("TBLDate",    texteHtml(sDateAga))) ;
      }
      else
      {
        ajouteBloc(new NSHtml("TBLNom",     "&nbsp;")) ;
        ajouteBloc(new NSHtml("TBLPrenom",  "&nbsp;")) ;

        if (strcmp((*i)->actes, "") != 0)
          sData = texteHtml(string((*i)->actes)) ;
        else
          sData = string("&nbsp;") ;
        ajouteBloc(new NSHtml("TBLExamen", sData)) ;

        if (strcmp((*i)->nomPatient, "") != 0)
          sData = string((*i)->nomPatient) ;
        else
          sData = string((*i)->libelle) ;
        ajouteBloc(new NSHtml("TBLNomLong", texteHtml(sData))) ;

        ajouteBloc(new NSHtml("TBLDate",      texteHtml(string((*i)->datePaie)))) ;
        ajouteBloc(new NSHtml("TBLCompte",    texteHtml(string((*i)->ecriture.compte)))) ;
        ajouteBloc(new NSHtml("TBLCompteLib", texteHtml(string((*i)->cpt_lib.libelle)))) ;
      }

      iMontant = atoi((*i)->montant) ;      sprintf(cMontant, "%d,%02d", iMontant / 100, iMontant % 100) ;
		  sData = string(cMontant) ;
      ajouteBloc(new NSHtml("TBLMontant", texteHtml(sData))) ;

      if (!strcmp((*i)->unite, "LOC"))        sData = pAgaDoc->pVar->getSigle() ;
      else
        sData = string("Euro") ;

      ajouteBloc(new NSHtml("TBLUnite", texteHtml(sData))) ;      ajouteBloc(new NSHtml("TBLModePaie", texteHtml(string((*i)->modePaie)))) ;

      _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;    }
  }
	bExisteTable = true ;
}

// -----------------------------------------------------------------------------
// Classe NSHtmlReq
//
// On instancie le html correspondant à l'état de la recherche
// -----------------------------------------------------------------------------

NSHtmlReq::NSHtmlReq(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)          :NSModHtml(typeOp, pDocNoy, pCtx)
{
  string sDateJour  = donne_date_duJour() ;

  // on récupère le titre du document  string sTitre     = string("Résultats de la recherche du ") + sDateJour ;
  string sOperateur = string(" pour ") + pContexte->getUtilisateur()->donneTitre(pContexte) ;
  sTitre += sOperateur ;

  ajoute(new NSHtml(string("titre"), texteHtml(sTitre))) ;  ajoute(new NSHtml(string("date"),  texteHtml(sDateJour))) ;
  // ajoute(new NSHtml("beneficiaire", sOperateur)) ;}

NSHtmlReq::~NSHtmlReq()
{
}

void
NSHtmlReq::initTotaux()
{
  NSReqRefDocument *pReqDoc = dynamic_cast<NSReqRefDocument *>(_pDocBrut) ;
  if (NULL == pReqDoc)
    return ;

  int 		total ;
  char 		cData[100] ;
  string 	sData ;

  total = pReqDoc->nbPatTotal ;
  sprintf(cData, "%d", total) ;
  sData = string(cData) ;
  ajoute(new NSHtml(string("nbPatTotal"), texteHtml(sData))) ;

  total = pReqDoc->nbPatResult ;
  sprintf(cData, "%d", total) ;
  sData = string(cData) ;
  ajoute(new NSHtml(string("nbPatResult"), texteHtml(sData))) ;

  total = pReqDoc->nbDocResult ;
  sprintf(cData, "%d", total) ;
  sData = string(cData) ;
  ajoute(new NSHtml(string("nbDocResult"), texteHtml(sData))) ;

  total = pReqDoc->nbPatCritPat ;
  sprintf(cData, "%d", total) ;
  sData = string(cData) ;
  ajoute(new NSHtml(string("nbPatCritPat"), texteHtml(sData))) ;

  total = pReqDoc->nbDocCritPat ;
  sprintf(cData, "%d", total) ;
  sData = string(cData) ;
  ajoute(new NSHtml(string("nbDocCritPat"), texteHtml(sData))) ;

  total = pReqDoc->nbPatCritDoc ;
  sprintf(cData, "%d", total) ;
  sData = string(cData) ;
  ajoute(new NSHtml(string("nbPatCritDoc"), texteHtml(sData))) ;

  total = pReqDoc->nbDocCritDoc ;
  sprintf(cData, "%d", total) ;
  sData = string(cData) ;
  ajoute(new NSHtml(string("nbDocCritDoc"), texteHtml(sData))) ;
}

void
NSHtmlReq::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

void
NSHtmlReq::ajouteLignes(){
	NSReqRefDocument *pReqDoc = dynamic_cast<NSReqRefDocument *>(_pDocBrut) ;
  if (NULL == pReqDoc)    return ;
  int nbTrouves ;
	if (pReqDoc->bReqModeDoc)  	nbTrouves = pReqDoc->nbDocResult ;
	else
		nbTrouves = pReqDoc->nbPatResult ;
	string sLang = pContexte->getUserLanguage() ;	for (int i = 0 ; i < nbTrouves ; i++)	{
  	ligneBlocHtml.vider() ;

    NSDocumentHisto* pDocHisto = (pReqDoc->VectDocResultat)[i] ;
    NSPatInfo*       pPatInfo  = (pReqDoc->VectPatResultat)[i] ;

		if (pReqDoc->bReqModeDoc)
    {
      if (pDocHisto)
      {
        ajouteBloc(new NSHtml("TBLNss", texteHtml(pDocHisto->getPatient()))) ;
        ajouteBloc(new NSHtml("TBLDoc", texteHtml(pDocHisto->getDocument()))) ;
      }
      else
      {
        ajouteBloc(new NSHtml("TBLNss", string("???"))) ;
        ajouteBloc(new NSHtml("TBLDoc", string("???"))) ;
      }

      if (pPatInfo)
      {
        ajouteBloc(new NSHtml("TBLNom",    texteHtml(pPatInfo->getNom()))) ;
        ajouteBloc(new NSHtml("TBLPrenom", texteHtml(pPatInfo->getPrenom()))) ;
      }
      else
      {
        ajouteBloc(new NSHtml("TBLNom",    string("???"))) ;
        ajouteBloc(new NSHtml("TBLPrenom", string("???"))) ;
      }

      if (pDocHisto)
      {
        string sDateAffiche = donne_date(pDocHisto->getDateDoc(), sLang) ;
        string sTitle       = pDocHisto->getDocName() + string(" du ") + sDateAffiche ;
        ajouteBloc(new NSHtml("TBLExamen", texteHtml(sTitle))) ;
      }
      else
        ajouteBloc(new NSHtml("TBLExamen", string("???"))) ;
    }
    else
    {
      if (pPatInfo)
      {
        ajouteBloc(new NSHtml("TBLNss",    texteHtml(pPatInfo->getNss()))) ;
        ajouteBloc(new NSHtml("TBLDoc",    string("&nbsp;"))) ;
        ajouteBloc(new NSHtml("TBLNom",    texteHtml(pPatInfo->getNom()))) ;
        ajouteBloc(new NSHtml("TBLPrenom", texteHtml(pPatInfo->getPrenom()))) ;
        ajouteBloc(new NSHtml("TBLExamen", string("&nbsp;"))) ;
      }
      else
      {
        ajouteBloc(new NSHtml("TBLNss",    string("???"))) ;
        ajouteBloc(new NSHtml("TBLDoc",    string("&nbsp;"))) ;
        ajouteBloc(new NSHtml("TBLNom",    string("???"))) ;
        ajouteBloc(new NSHtml("TBLPrenom", string("???"))) ;
        ajouteBloc(new NSHtml("TBLExamen", string("&nbsp;"))) ;
      }
    }

    _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;	}

	bExisteTable = true ;
}

// -----------------------------------------------------------------------------
// Classe NSHtmlFact
//
// On instancie le html correspondant à une "facturette" (reçu de paiement)
// -----------------------------------------------------------------------------

NSHtmlFact::NSHtmlFact(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)           :NSModHtml(typeOp, pDocNoy, pCtx)
{
  string sDateJour  = donne_date_duJour() ;

  // on récupère le titre du document  string sTitre     = string("Reçu du ") + sDateJour ;
  string sOperateur = string(" pour ") + pContexte->getUtilisateur()->donneTitre(pContexte) ;
  sTitre += sOperateur ;

  ajoute(new NSHtml(string("titre"), texteHtml(sTitre))) ;  ajoute(new NSHtml(string("date"),  texteHtml(sDateJour))) ;
  // ajoute(new NSHtml("beneficiaire", sOperateur)) ;}

NSHtmlFact::~NSHtmlFact()
{
}

void
NSHtmlFact::InsertFactElements(const NSFactData* pData)
{
  if ((NSFactData*) NULL == pData)
    return ;

  ajoute(new NSHtml(string("payedAmount"), texteHtml(pData->getSum() + string(" ") + pData->getUnit()))) ;
}

// -----------------------------------------------------------------------------
// Classe NSHtmlDep
//
// On instancie le html correspondant à l'état des Dep en cours
// -----------------------------------------------------------------------------

NSHtmlDep::NSHtmlDep(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)          :NSModHtml(typeOp, pDocNoy, pCtx)
{
  NSDepRefDocument *pDepDoc = dynamic_cast<NSDepRefDocument *>(pDocNoy) ;
  if (NULL == pDepDoc)
    return ;

	string sLang = pContexte->getUserLanguage() ;

  string sDateDep1 = donne_date(pDepDoc->pCriteres->getDateAga1(), sLang) ;  string sDateDep2 = donne_date(pDepDoc->pCriteres->getDateAga2(), sLang) ;

  string sTitre = string("Liste des dépenses du ") + string(sDateDep1) + string(" au ") + string(sDateDep2) ;
  string sOperateur = string("") ;
  if (pDepDoc->pCriteres->isActesPerso())  {
    sOperateur = string(" pour ") + pContexte->getUtilisateur()->donneTitre(pContexte) ;
    sTitre += sOperateur ;
  }

  ajoute(new NSHtml(string("titre"),        texteHtml(sTitre))) ;
  ajoute(new NSHtml(string("date"),         texteHtml(sDateDep1 + string(" au ") + sDateDep2))) ;
  ajoute(new NSHtml(string("beneficiaire"), texteHtml(sOperateur))) ;
}

NSHtmlDep::~NSHtmlDep(){
}

voidNSHtmlDep::initTotaux()
{
  NSDepRefDocument *pDepDoc = dynamic_cast<NSDepRefDocument *>(_pDocBrut) ;
  if (NULL == pDepDoc)
    return ;

  int 		montant ;
  char 		cData[100] ;
  string 	sData ;

  montant = pDepDoc->pTotaux->paieLoc ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pDepDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totPaieLoc"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->paieEuro ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totPaieEuro"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->espLoc ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pDepDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totEspLoc"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->espEuro ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totEspEuro"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->chqLoc ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pDepDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totChqLoc"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->chqEuro ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totChqEuro"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->virLoc ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pDepDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totVirLoc"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->virEuro;  sprintf(cData, "%d,%02d ", montant/100, montant%100);
  sData = string(cData) + string("€");
  ajoute(new NSHtml(string("totVirEuro"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->cbLoc ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pDepDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totCbLoc"), texteHtml(sData))) ;

  montant = pDepDoc->pTotaux->cbEuro ;  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totCbEuro"), texteHtml(sData))) ;
}

void
NSHtmlDep::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

voidNSHtmlDep::ajouteLignes(){
try
{
	NSDepRefDocument *pDepDoc = dynamic_cast<NSDepRefDocument *>(_pDocBrut) ;
	if (!pDepDoc || !(pDepDoc->pDepensArray) || pDepDoc->pDepensArray->empty())  	return ;
	bExisteTable = true ;
}catch (...)
{
	erreur("Exception NSHtmlDep::ajouteLignes.", standardError, 0) ;
}
}
// -----------------------------------------------------------------------------// Classe NSHtmlImp//
// On instancie le html correspondant à l'état des impayés en cours
// -----------------------------------------------------------------------------

NSHtmlImp::NSHtmlImp(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
          :NSModHtml(typeOp, pDocNoy, pCtx)
{
  NSImpRefDocument *pImpDoc = dynamic_cast<NSImpRefDocument *>(pDocNoy) ;
  if (NULL == pImpDoc)
    return ;

  string sLang = string("") ;
  if (pContexte)
    sLang = pContexte->getUserLanguage() ;

  string sDate1 = donne_date(pImpDoc->pCriteres->sDate1, sLang) ;
  string sDate2 = donne_date(pImpDoc->pCriteres->sDate2, sLang) ;

  string sDate = sDate1 ;
  if ((pImpDoc->pCriteres->sDate2) > (pImpDoc->pCriteres->sDate1))
    sDate += string(" au ") + sDate2 ;

  // on récupère le titre du document
  string sTitre = string("Liste des impayes du ") + sDate ;

  string sOperateur = string("") ;
  if (pImpDoc->pCriteres->bActesPerso)
  {
    sOperateur = string(" pour ") + pContexte->getUtilisateur()->donneTitre(pContexte) ;
    sTitre += sOperateur ;
  }

  if (pImpDoc->pCriteres->bInterruptedProcess)
    sTitre += string(" (résultats partiels, processus interrompu en cours de recherche)") ;

  string sCode   = (pImpDoc->pCriteres->sCodeExamen) + (pImpDoc->pCriteres->sSynExamen) ;
  string sExamen = string("Tous") ;
  if (string("") != sCode)
    pContexte->getDico()->donneLibelle(sLang, &sCode, &sExamen) ;

  string sPrescript = pImpDoc->pCriteres->sTitrePrescript ;
  if (string("") == sPrescript)
    sPrescript = string("Tous") ;

  string sContexte = string("") ;

  switch (pImpDoc->pCriteres->iContexte)
  {
    case 0  : sContexte = string("Tous") ;
              break ;
    case 1  : sContexte = string("Non précisé") ;
              break ;
    case 2  : sContexte = string("Externe") ;
              break ;
    case 4  : sContexte = string("Hospitalisé") ;
              break ;
    case 8  : sContexte = string("Ambulatoire") ;
              break ;
  }

  string sLibCourtOrga = string("Tous") ;
  string sLibLongOrga  = string("Tous") ;
  if (string("") != pImpDoc->pCriteres->sCodeOrga)
  {
    sLibCourtOrga = pImpDoc->pCriteres->sLibCourtOrga ;
		sLibLongOrga  = pImpDoc->pCriteres->sLibLongOrga ;
  }

  ajoute(new NSHtml(string("titre"),        texteHtml(sTitre))) ;
  ajoute(new NSHtml(string("date"),         texteHtml(sDate))) ;
  ajoute(new NSHtml(string("dateDeb"),      texteHtml(sDate1))) ;
  ajoute(new NSHtml(string("dateFin"),      texteHtml(sDate2))) ;
  ajoute(new NSHtml(string("beneficiaire"), texteHtml(sOperateur))) ;
  ajoute(new NSHtml(string("libExam"),      texteHtml(sExamen))) ;
  ajoute(new NSHtml(string("prescripteur"), texteHtml(sPrescript))) ;
	ajoute(new NSHtml(string("contexte"),     texteHtml(sContexte))) ;
  ajoute(new NSHtml(string("libCourtOrga"), texteHtml(sLibCourtOrga))) ;
  ajoute(new NSHtml(string("libLongOrga"),  texteHtml(sLibLongOrga))) ;
}

NSHtmlImp::~NSHtmlImp()
{
}

void
NSHtmlImp::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

void
NSHtmlImp::ajouteLignes()
{
  NSImpRefDocument *pImpDoc = dynamic_cast<NSImpRefDocument *>(_pDocBrut) ;
  if (NULL == pImpDoc)
    return ;

	string sLang = pContexte->getUserLanguage() ;

  if (false == pImpDoc->pImpArray->empty())
  {
    for (NSImpIter i = pImpDoc->pImpArray->begin() ; i != pImpDoc->pImpArray->end() ; i++)
    {
      ligneBlocHtml.vider() ;

      string sDateExam = donne_date(string((*i)->dateCompt), sLang) ;
      ajouteBloc(new NSHtml("TBLDateExam", texteHtml(sDateExam))) ;

      ajouteBloc(new NSHtml("TBLNom",     texteHtml(((*i)->patInfo).getNom()))) ;
      ajouteBloc(new NSHtml("TBLPrenom",  texteHtml(((*i)->patInfo).getPrenom()))) ;
      ajouteBloc(new NSHtml("TBLCodePat", texteHtml(((*i)->patInfo).getNss()))) ;
      ajouteBloc(new NSHtml("TBLNomLong", texteHtml((*i)->sNomPatient))) ;
      ajouteBloc(new NSHtml("TBLLibExam", texteHtml((*i)->sLibExam))) ;

      string sSommeDue = string("") ;
      string sImpaye   = string("") ;

      if (false == pImpDoc->bEuro)
      {
        int  iSommeDue = atoi((*i)->sommeDueLoc) ;
        char cSommeDue[10] = "" ;
        sprintf(cSommeDue, "%d,%02d ", iSommeDue / 100, iSommeDue % 100) ;

        sSommeDue = string(cSommeDue) + pImpDoc->pVar->getSigle() ;

        int  iImpaye = atoi((*i)->impayeLoc) ;
        char cImpaye[10] = "" ;
        sprintf(cImpaye, "%d,%02d ", iImpaye / 100, iImpaye % 100) ;

        sImpaye = string(cImpaye) + pImpDoc->pVar->getSigle() ;
      }
      else // on affiche les sommes en Euro
      {
        int  iSommeDue = atoi((*i)->sommeDueEuro) ;
        char cSommeDue[10] = "" ;
        sprintf(cSommeDue, "%d,%02d ", iSommeDue / 100, iSommeDue % 100) ;

        sSommeDue = string(cSommeDue) + string("€") ;

        int  iImpaye = atoi((*i)->impayeEuro) ;
        char cImpaye[10] = "" ;
        sprintf(cImpaye, "%d,%02d ", iImpaye / 100, iImpaye % 100) ;

        sImpaye = string(cImpaye) + string("€") ;
      }

      ajouteBloc(new NSHtml("TBLSommeDue", texteHtml(sSommeDue))) ;
      ajouteBloc(new NSHtml("TBLImpaye",   texteHtml(sImpaye))) ;

      string sData = texteHtml(string((*i)->libOrga)) ;
      if (string("") == sData)
        sData = "&nbsp;" ;
      ajouteBloc(new NSHtml("TBLLibOrga", sData)) ;

      _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;
    }
  }

	bExisteTable = true ;
}

// -----------------------------------------------------------------------------
// Classe NSHtmlLac
//
// On instancie le html correspondant à la liste des actes
// -----------------------------------------------------------------------------

NSHtmlLac::NSHtmlLac(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
          :NSModHtml(typeOp, pDocNoy, pCtx)
{
  if (NULL == pDocNoy)
    return ;

  NSListActRefDocument *pActDoc = dynamic_cast<NSListActRefDocument *>(pDocNoy) ;
  if (NULL == pActDoc)
    return ;

  string sLang = string("") ;
  if (pContexte)
    sLang = pContexte->getUserLanguage() ;

  string sDate1 = donne_date(pActDoc->pCriteres->sDate1, sLang) ;
  string sDate2 = donne_date(pActDoc->pCriteres->sDate2, sLang) ;

  string sDate = string(sDate1) ;
  if ((pActDoc->pCriteres->sDate2) > (pActDoc->pCriteres->sDate1))
    sDate += string(" au ") + string(sDate2) ;

  // on récupère le titre du document
  string sTitre = string("Liste des actes du ") + sDate ;

  string sOperateur = string("") ;
  if (pActDoc->pCriteres->bActesPerso)
  {
    sOperateur = string(" pour ") + pContexte->getUtilisateur()->donneTitre(pContexte) ;
    sTitre += sOperateur ;
  }

  if (pActDoc->pCriteres->bInterruptedProcess)
    sTitre += string(" (résultats partiels, processus interrompu en cours de recherche)") ;

  string sExamen = string("Tous") ;
  string sCode   = (pActDoc->pCriteres->sCodeExamen) + (pActDoc->pCriteres->sSynExamen) ;
  if (string("") != sCode)
    pContexte->getDico()->donneLibelle(sLang, &sCode, &sExamen) ;

  string sPrescript = pActDoc->pCriteres->sTitrePrescript ;
  if (string("") == sPrescript)
    sPrescript = string("Tous") ;

  string sImpayes = string("") ;
  switch (pActDoc->pCriteres->iImpayes)
  {
    case 0  :
      sImpayes = string("Tous") ;
      break ;
    case 1  :
      sImpayes = string("Impayes") ;
      break ;
    case 2  :
      sImpayes = string("Payes") ;
  }

  string sContexte = string("") ;
  switch (pActDoc->pCriteres->iContexte)
  {
    case 0  :
      sContexte = string("Tous") ;
      break ;
    case 1  :
      sContexte = string("Non précisé") ;
      break ;
    case 2  :
      sContexte = string("Externe") ;
      break ;
    case 4  :
      sContexte = string("Hospitalisé") ;
      break ;
    case 8  :
      sContexte = string("Ambulatoire") ;
      break ;
  }

  ajoute(new NSHtml(string("titre"),        texteHtml(sTitre))) ;
  ajoute(new NSHtml(string("date"),         texteHtml(sDate))) ;
  ajoute(new NSHtml(string("dateDeb"),      texteHtml(sDate1))) ;
  ajoute(new NSHtml(string("dateFin"),      texteHtml(sDate2))) ;
  ajoute(new NSHtml(string("beneficiaire"), texteHtml(sOperateur))) ;
  ajoute(new NSHtml(string("libExam"),      texteHtml(sExamen))) ;
  ajoute(new NSHtml(string("prescripteur"), texteHtml(sPrescript))) ;
  ajoute(new NSHtml(string("impayes"),      texteHtml(sImpayes))) ;
	ajoute(new NSHtml(string("contexte"),     texteHtml(sContexte))) ;
}

NSHtmlLac::~NSHtmlLac()
{
}

void
NSHtmlLac::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

void
NSHtmlLac::ajouteLignes()
{
	string sLang = pContexte->getUserLanguage() ;

  NSListActRefDocument *pActDoc = dynamic_cast<NSListActRefDocument *>(_pDocBrut) ;
  if (NULL == pActDoc)
    return ;

  if (pActDoc->pActArray && (false == pActDoc->pActArray->empty()))
  {
    for (NSListActIter i = pActDoc->pActArray->begin() ; pActDoc->pActArray->end() != i ; i++)
    {
      ligneBlocHtml.vider() ;

      string sDateExam = donne_date(string((*i)->dateCompt), sLang) ;
      ajouteBloc(new NSHtml("TBLDateExam", texteHtml(sDateExam))) ;

      ajouteBloc(new NSHtml("TBLNom",     texteHtml(((*i)->patInfo).getNom()))) ;
      ajouteBloc(new NSHtml("TBLPrenom",  texteHtml(((*i)->patInfo).getPrenom()))) ;
      ajouteBloc(new NSHtml("TBLCodePat", texteHtml(((*i)->patInfo).getNss()))) ;
      ajouteBloc(new NSHtml("TBLNomLong", texteHtml((*i)->nomPatient))) ;

      string sData = texteHtml(string((*i)->libExam)) ;
      if (sData == "")
        sData = "&nbsp;" ;
      ajouteBloc(new NSHtml("TBLLibExam", sData)) ;

      sData = texteHtml(string((*i)->actes)) ;
      if (sData == "")
        sData = "&nbsp;" ;
      ajouteBloc(new NSHtml("TBLActes", sData)) ;

      _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;
    }
  }

	bExisteTable = true ;
}

// -----------------------------------------------------------------------------
// Classe NSHtmlSac
//
// On instancie le html correspondant à la somme des actes
// -----------------------------------------------------------------------------

NSHtmlSac::NSHtmlSac(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
          :NSModHtml(typeOp, pDocNoy, pCtx)
{
  if (NULL == pDocNoy)
    return ;

  NSSomActRefDocument *pActDoc = dynamic_cast<NSSomActRefDocument *>(pDocNoy) ;
  if (NULL == pActDoc)
    return ;

  string sLang = string("") ;
  if (pContexte)
    sLang = pContexte->getUserLanguage() ;

  string sDate1 = donne_date(pActDoc->pCriteres->sDate1, sLang) ;
  string sDate2 = donne_date(pActDoc->pCriteres->sDate2, sLang) ;

  string sDate = sDate1 ;
  if ((pActDoc->pCriteres->sDate2) > (pActDoc->pCriteres->sDate1))
    sDate += string(" au ") + sDate2 ;

  // on récupère le titre du document
  string sTitre = string("Somme des actes du ") + sDate ;

  string sOperateur = string("") ;
  if (pActDoc->pCriteres->bActesPerso)
  {
    sOperateur = string(" pour ") + pContexte->getUtilisateur()->donneTitre(pContexte) ;
    sTitre += sOperateur ;
  }

  if (pActDoc->pCriteres->bInterruptedProcess)
    sTitre += string(" (résultats partiels, processus interrompu en cours de recherche)") ;

  string sExamen = string("Tous") ;
  string sCode   = (pActDoc->pCriteres->sCodeExamen) + (pActDoc->pCriteres->sSynExamen) ;
  if (string("") != sCode)
    pContexte->getDico()->donneLibelle(sLang, &sCode, &sExamen) ;

  string sPrescript = pActDoc->pCriteres->sTitrePrescript ;
  if (string("") == sPrescript)
    sPrescript = string("Tous") ;

  string sImpayes = string("") ;
  switch (pActDoc->pCriteres->iImpayes)
  {
    case 0  :
      sImpayes = string("Tous") ;
      break ;
    case 1  :
      sImpayes = string("Impayes") ;
      break ;
    case 2  :
      sImpayes = string("Payes") ;
  }

  string sContexte = string("") ;
  switch (pActDoc->pCriteres->iContexte)
  {
    case 0  :
      sContexte = string("Tous") ;
      break ;
    case 1  :
      sContexte = string("Non précisé") ;
      break ;
    case 2  :
      sContexte = string("Externe") ;
      break ;
    case 4  :
      sContexte = string("Hospitalisé") ;
      break ;
    case 8  :
      sContexte = string("Ambulatoire") ;
      break ;
  }

  ajoute(new NSHtml(string("titre"),        texteHtml(sTitre))) ;
  ajoute(new NSHtml(string("date"),         texteHtml(sDate))) ;
  ajoute(new NSHtml(string("dateDeb"),      texteHtml(sDate1))) ;
  ajoute(new NSHtml(string("dateFin"),      texteHtml(sDate2))) ;
  ajoute(new NSHtml(string("beneficiaire"), texteHtml(sOperateur))) ;
  ajoute(new NSHtml(string("libExam"),      texteHtml(sExamen))) ;
  ajoute(new NSHtml(string("prescripteur"), texteHtml(sPrescript))) ;
  ajoute(new NSHtml(string("impayes"),      texteHtml(sImpayes))) ;
	ajoute(new NSHtml(string("contexte"),     texteHtml(sContexte))) ;
}

NSHtmlSac::~NSHtmlSac()
{
}

void
NSHtmlSac::initTotaux()
{
  int 		montant ;
  char 		cData[100] ;
  string 	sData ;

  NSSomActRefDocument *pActDoc = dynamic_cast<NSSomActRefDocument *>(_pDocBrut) ;

  montant = pActDoc->totaux._iTotalLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("totalLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux._iTotalEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("totalEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.depassLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("depassLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.depassEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("depassEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.paieLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("paieLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.paieEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("paieEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.espLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("espLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.espEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("espEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.chqLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("chqLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.chqEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("chqEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.virLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("virLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.virEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("virEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.cbLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("cbLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.cbEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("cbEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.impayeLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("impayeLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.impayeEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("impayeEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.impTPLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("impTPLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.impTPEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("impTPEuro"), texteHtml(sData))) ;

  montant = pActDoc->totaux.impAutreLoc ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + pActDoc->pVar->getSigle() ;
  ajoute(new NSHtml(string("impAutreLoc"), texteHtml(sData))) ;

  montant = pActDoc->totaux.impAutreEuro ;
  sprintf(cData, "%d,%02d ", montant / 100, montant % 100) ;
  sData = string(cData) + string("€") ;
  ajoute(new NSHtml(string("impAutreEuro"), texteHtml(sData))) ;

  ajoute(new NSHtml(string("countSentence"), texteHtml(pActDoc->totaux.GetCountsSentence()))) ;
  ajoute(new NSHtml(string("depasSentence"), texteHtml(pActDoc->totaux.GetDepassSentence()))) ;
}


void
NSHtmlSac::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}


void
NSHtmlSac::ajouteLignes()
{
  string sData ;
  string sCodeExam ;
  char   cNbExam[10] ;
  char   buffer[255] ;
  char   cCodeExam[80] ;
  int    occur ;

  NSSomActRefDocument *pActDoc = dynamic_cast<NSSomActRefDocument *>(_pDocBrut) ;

  string sLang = string("") ;
  if (pContexte)
    sLang = pContexte->getUserLanguage() ;

  for (NSExamIter i = pActDoc->pExamArray->begin() ; i != pActDoc->pExamArray->end() ; i++)
  {
    ligneBlocHtml.vider() ;

    sCodeExam = string((*i)->sCodeExam) ;
    pContexte->getDico()->donneLibelle(sLang, &sCodeExam, &sData) ;
    if (sData == "")
      sData = "&nbsp;" ;
    else
      sData = texteHtml(sData) ;
    ajouteBloc(new NSHtml("TBLLibExam", sData)) ;

    itoa((*i)->nbExam, cNbExam, 10) ;
		sData = string(cNbExam) ;
    ajouteBloc(new NSHtml("TBLNbExam", texteHtml(sData))) ;

    strcpy(buffer, "") ;

    for (NSKCodeIter j = (*i)->aKCodeArray.begin() ; j != (*i)->aKCodeArray.end() ; j++)
    {
      occur = (*j)->occur * 100 ;
      if ((occur % 100) == 0)
        sprintf(cCodeExam, " %s(%d)", ((*j)->sKCode).c_str(), occur / 100) ;
      else
        sprintf(cCodeExam, " %s(%d,%02d)", ((*j)->sKCode).c_str(), occur / 100, occur % 100) ;

      strcat(buffer, cCodeExam) ;
    }
    sData = string(buffer) ;
    ajouteBloc(new NSHtml("TBLKCodes", texteHtml(sData))) ;

    _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;
  }

  bExisteTable = true ;
}


// -----------------------------------------------------------------------------
// Classe NSHtmlSen
//
// On instancie le html correspondant à la somme des encaissements
// -----------------------------------------------------------------------------
NSHtmlSen::NSHtmlSen(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)          :NSModHtml(typeOp, pDocNoy, pCtx)
{
	string sLang = pContexte->getUserLanguage() ;

  NSSomEncaissRefDocument *pEncaissDoc = dynamic_cast<NSSomEncaissRefDocument *>(pDocNoy) ;

  string sDate1 = donne_date(pEncaissDoc->pCriteres->sDate1, sLang) ;
  string sDate2 = donne_date(pEncaissDoc->pCriteres->sDate2, sLang) ;

  string sDate = sDate1 ;
  if ((pEncaissDoc->pCriteres->sDate2) > (pEncaissDoc->pCriteres->sDate1))
    sDate += string(" au ") + sDate2 ;

  // on récupère le titre du document
  string sTitre = string("Somme des encaissements du ") + sDate ;

  string sOperateur = string("") ;
  if (pEncaissDoc->pCriteres->bActesPerso)
  {
    sOperateur = string(" pour ") + pContexte->getUtilisateur()->donneTitre(pContexte) ;
   	sTitre += sOperateur ;
  }

  if (pEncaissDoc->pCriteres->bInterruptedProcess)
    sTitre += string(" (résultats partiels, processus interrompu en cours de recherche)") ;

  string sLibCourtOrga = pEncaissDoc->pCriteres->sLibCourtOrga ;
  string sLibLongOrga  = pEncaissDoc->pCriteres->sLibLongOrga ;

  ajoute(new NSHtml(string("titre"),        texteHtml(sTitre))) ;
  ajoute(new NSHtml(string("date"),         texteHtml(sDate))) ;
  ajoute(new NSHtml(string("dateDeb"),      texteHtml(sDate1))) ;
  ajoute(new NSHtml(string("dateFin"),      texteHtml(sDate2))) ;
  ajoute(new NSHtml(string("beneficiaire"), texteHtml(sOperateur))) ;
  ajoute(new NSHtml(string("libCourtOrga"), texteHtml(sLibCourtOrga))) ;
  ajoute(new NSHtml(string("libLongOrga"),  texteHtml(sLibLongOrga))) ;
}

NSHtmlSen::~NSHtmlSen()
{
}

void
NSHtmlSen::initTotaux()
{
  NSSomEncaissRefDocument *pEncaissDoc = dynamic_cast<NSSomEncaissRefDocument *>(_pDocBrut) ;

  if ((NSSomEncaissRefDocument*) NULL == pEncaissDoc)
    return ;

  string sLocalUnit = pEncaissDoc->pVar->getSigle() ;

  addElement(string("totPaieLoc"),  pEncaissDoc->pTotaux->paieLoc,  sLocalUnit) ;
  addElement(string("totPaieEuro"), pEncaissDoc->pTotaux->paieEuro, string("€")) ;

  addElement(string("totEspLoc"),  pEncaissDoc->pTotaux->espLoc,  sLocalUnit) ;
  addElement(string("totEspEuro"), pEncaissDoc->pTotaux->espEuro, string("€")) ;

  addElement(string("totChqLoc"),  pEncaissDoc->pTotaux->chqLoc,  sLocalUnit) ;
  addElement(string("totChqEuro"), pEncaissDoc->pTotaux->chqEuro, string("€")) ;

  addElement(string("totVirLoc"),  pEncaissDoc->pTotaux->virLoc,  sLocalUnit) ;
  addElement(string("totVirEuro"), pEncaissDoc->pTotaux->virEuro, string("€")) ;

  addElement(string("totCbLoc"),  pEncaissDoc->pTotaux->cbLoc,  sLocalUnit) ;
  addElement(string("totCbEuro"), pEncaissDoc->pTotaux->cbEuro, string("€")) ;

  addElement(string("parPaieLoc"),  pEncaissDoc->pTotaux->paieLoc,  sLocalUnit) ;
  addElement(string("parPaieEuro"), pEncaissDoc->pTotaux->paieEuro, string("€")) ;

  addElement(string("parEspLoc"),  pEncaissDoc->pTotaux->espLoc,  sLocalUnit) ;
  addElement(string("parEspEuro"), pEncaissDoc->pTotaux->espEuro, string("€")) ;

  addElement(string("parChqLoc"),  pEncaissDoc->pTotaux->chqLoc,  sLocalUnit) ;
  addElement(string("parChqEuro"), pEncaissDoc->pTotaux->chqEuro, string("€")) ;

  addElement(string("parVirLoc"),  pEncaissDoc->pTotaux->virLoc,  sLocalUnit) ;
  addElement(string("parVirEuro"), pEncaissDoc->pTotaux->virEuro, string("€")) ;

  addElement(string("parCbLoc"),  pEncaissDoc->pTotaux->cbLoc,  sLocalUnit) ;
  addElement(string("parCbEuro"), pEncaissDoc->pTotaux->cbEuro, string("€")) ;
}

void
NSHtmlSen::addElement(string sLabel, int iAmount, string sCurrency)
{
  char cData[100] ;
  sprintf(cData, "%d,%02d ", iAmount / 100, iAmount % 100) ;

  string sData = string(cData) + sCurrency ;

  ajoute(new NSHtml(sLabel, texteHtml(sData))) ;
}

// -----------------------------------------------------------------------------
//
// Classe NSHtmlPubli//
// -----------------------------------------------------------------------------NSHtmlPubli::NSHtmlPubli(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)            :NSModHtml(typeOp, pDocNoy, pCtx)
{
	string sLang = pContexte->getUserLanguage() ;

  NSPubliRefDocument *pPBDoc = dynamic_cast<NSPubliRefDocument *>(_pDocBrut) ;
  if (NULL == pPBDoc)
    return ;

  // Initialisation des données statiques
  if (string("ZPCVC") == pPBDoc->_pDocInfo->getTypeSem())
  {
    string *psDate1 = static_cast<string *>((*(pPBDoc->pVoidArray))[0]) ;
    string *psDate2 = static_cast<string *>((*(pPBDoc->pVoidArray))[1]) ;

    string sDate1 = donne_date(*psDate1, sLang) ;
    string sDate2 = donne_date(*psDate2, sLang) ;
    string sTitre = string("Liste des patients à reconvoquer entre le ") +
                    sDate1 + string(" et le ") + sDate2 + string(".") ;

    ajoute(new NSHtml(string("titre"), texteHtml(sTitre))) ;
    ajoute(new NSHtml(string("date1"), texteHtml(sDate1))) ;
    ajoute(new NSHtml(string("date2"), texteHtml(sDate2))) ;
  }
}

NSHtmlPubli::~NSHtmlPubli()
{
}

void
NSHtmlPubli::ajouteBloc(NSHtml *ph)
{
  ligneBlocHtml.push_back(new NSBlocHtml(ph, 1)) ;
}

void
NSHtmlPubli::ajouteLignes()
{
	string sLang = pContexte->getUserLanguage() ;

  NSPubliRefDocument *pPBDoc = dynamic_cast<NSPubliRefDocument *>(_pDocBrut) ;
  if ((NULL == pPBDoc) || (NULL == pPBDoc->_pDocInfo))
    return ;

  if (string("ZPCVC") == pPBDoc->_pDocInfo->getTypeSem())
  {
    NSPersonArray *pPatArray = static_cast<NSPersonArray *>((*(pPBDoc->pVoidArray))[2]) ;
    if (((NSPersonArray*) NULL == pPatArray) || pPatArray->empty())
    	return ;

    for (NSPersonIter i = pPatArray->begin() ; pPatArray->end() != i ; i++)
    {
      ligneBlocHtml.vider() ;

      ajouteBloc(new NSHtml("TBLNomLong", texteHtml((*i)->getNomLong()))) ;

/*
      if ((*i)->initGraphs())
      {
        if ((*i)->sChez != "")
            sAdr = (*i)->sChez + string("\r\n");

        sAdr += (*i)->pGraphAdr->trouveLibLongAdr();
      }
*/

      ajouteBloc(new NSHtml("TBLAdresse", texteHtml((*i)->getMainAdr()))) ;

      string sDateNau = donne_date((*i)->getConvoc(), sLang) ;
      ajouteBloc(new NSHtml("TBLDateConvoc", texteHtml(sDateNau))) ;

      _table.push_back(new NSBlocHtmlArray(ligneBlocHtml)) ;
    }
  }

  bExisteTable = true ;
}

// -----------------------------------------------------------------------------
//
// Classe NSHtmlCR
//
// -----------------------------------------------------------------------------

NSHtmlCR::NSHtmlCR(OperationType typeOp, NSNoyauDocument *pDocNoy, NSContexte *pCtx, string sDest)
         :NSModHtml(typeOp, pDocNoy, pCtx)
{
try
{
  _sSource = string("") ;
  _iCc     = 0 ;

  if ((NSNoyauDocument*) NULL == pDocNoy)
    return ;

  NSCRDocument *pCRDoc = dynamic_cast<NSCRDocument *>(pDocNoy) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return ;

  // On récupère le nom de fichier associé et on le transforme en html
  //
  NSHtml *pHtmlCR = new NSHtml(tCR) ;

  if      (false == lireCR(pCRDoc->getTextFile()))
    MessageBox(0, "Pb lecture du compte-rendu", 0, MB_OK) ;
  else if (false == buildHtmlTree(pHtmlCR))
    MessageBox(0, "Pb construction du compte-rendu", 0, MB_OK) ;

  // on récupère le titre du document (au sens TDocument)
  string sTitre = string("") ;
  if (pCRDoc->GetTitle())
    sTitre = string(pCRDoc->GetTitle()) ;

  // recherche de l'operateur
  //
  string sCreateur = string("") ;

  // on prend la date du document
  string sDate = pCRDoc->GetDateDoc() ;

  ajoute(tTitle,        sTitre) ;
  ajoute(tOperateur,    sCreateur) ;
  ajoute(t2ndOperateur, pCRDoc->GetSecondOperator()) ;
  ajoute(tDate,         sDate) ;
  ajoute(tDest,         sDest) ;
  ajoute(pHtmlCR) ;
  addSignature() ;
  addPatient() ;
}
catch (...)
{
	erreur("Exception NSHtmlCR ctor.", standardError, 0) ;
}
}

NSHtmlCR::NSHtmlCR(NSNoyauDocument *pDocNoy, NSContexte *pCtx)
         :NSModHtml(toVisualiser, pDocNoy, pCtx)
{
  _sSource = string("") ;
  _iCc     = 0 ;
}

NSHtmlCR::~NSHtmlCR()
{
}

HtmlTypes
NSHtmlCR::typeHtml(char cc)
{
  HtmlTypes typeRetour ;

  switch (cc)
  {
    case 'B'  : typeRetour = tIndent ;
                break ;

    case 'C'  : typeRetour = tCR ;
                break ;

    case '1'  : typeRetour = tTitre1 ;
                break ;

    case '2'  : typeRetour = tTitre2 ;
                break ;

    case '3'  : typeRetour = tTitre3 ;
                break ;

    case '4'  : typeRetour = tTitre4 ;
                break ;

    case '5'  : typeRetour = tTitre5 ;
                break ;

    case '6'  : typeRetour = tTitre6 ;
                break ;

    case '7'  : typeRetour = tTitre7 ;                break ;

    case '8'  : typeRetour = tTitre8 ;
                break ;

    case '9'  : typeRetour = tTitre9 ;
                break ;

    case 'T'  : typeRetour = tTexte ;
                break ;

    case 'P'  : typeRetour = tPara ;
                break ;

    case 'G'  : typeRetour = tGras ;
                break ;

    case 'S'  : typeRetour = tSouligne ;
                break ;

    case 'I'  : typeRetour = tItalic ;
                break ;

    default   : typeRetour = tUndefined ;
  }
  return typeRetour ;
}

string
NSHtmlCR::lireTexte()
{
  string sTexte = string("") ;

  while ((_iCc < strlen(_sSource.c_str())) && (ESC_CHAR != _sSource[_iCc]) && (FS_CHAR != _sSource[_iCc]))
  {
    sTexte += _sSource[_iCc] ;
    _iCc++ ;
  }

  return sTexte ;
}

bool
NSHtmlCR::lireFils(NSHtml *pere, HtmlTypes typeDebut, string sChemin)
{
  NSHtml    *petitFils ;
  string    texte ;
  bool      bOk       = true ;
  bool      bEstPere  = false ;
  HtmlTypes typeNouv, typeFin = tHtml ;

  NSHtml *fils = new NSHtml(typeDebut) ;
  fils->setPath(sChemin) ;

  while ((bOk) && (typeFin != typeDebut))
  {
    if (_iCc < strlen(_sSource.c_str()))
    {
      switch(_sSource[_iCc])
      {
        case ESC_CHAR :
          _iCc++ ;
          if (_iCc >= strlen(_sSource.c_str())) // on ne peut pas lire le type
          {
            bOk = false ;
            delete fils ;
            break ;
          }
          typeNouv = typeHtml(_sSource[_iCc++]) ;
          bOk = lireFils(fils, typeNouv, sChemin) ;
          bEstPere = true ;
          break ;

        case FS_CHAR :
          _iCc++ ;
          if (_iCc >= strlen(_sSource.c_str()))
          {
            bOk = false ;
            delete fils ;
            break ;
          }
          typeFin = typeHtml(_sSource[_iCc++]) ;
          if (typeFin == typeDebut)
            pere->addSon(new NSHtml(*fils)) ;
          else
            bOk = false ;
          delete fils ;
          break ;

        default :
          texte = string("") ;
          texte = lireTexte() ;

          if (typeDebut != tIndent)
          {
            if (bEstPere)
            {
              petitFils = new NSHtml(typeDebut) ;
              petitFils->setText(texteHtml(texte)) ;
              petitFils->setPath(sChemin) ;
              fils->addSon(petitFils) ;
            }
            else
              fils->setText(texteHtml(texte)) ;
          }
      }	// fin du switch(source[cc])
    }
    else // on ne peut pas lire le fils
    {
      bOk = false ;
      delete fils ;
    }
  }
  return (bOk) ;
}

bool
NSHtmlCR::lireCR(string sFichier)
{
  if (string("") == sFichier)
    return false ;

  ifstream inFile ;
  inFile.open(sFichier.c_str()) ;
  if (!inFile)
  {
    string sErreur = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFichier ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

  string line ;
  while (!inFile.eof())
  {
    getline(inFile, line) ;
    if (string("") != line)
      _sSource += line + "\n" ;
  }

  inFile.close() ;

  return true ;
}

bool
NSHtmlCR::buildHtmlTree(NSHtml *pCR)
{
  HtmlTypes typeFils ;
  bool bOk = true ;
  while ((_iCc < strlen(_sSource.c_str())) && (bOk))
  {
    string sChemin = string("") ;

    // lecture jusqu'au prochain marqueur ou fin de fichier
    while ((_iCc < strlen(_sSource.c_str())) && (ESC_CHAR != _sSource[_iCc]) && ('|' != _sSource[_iCc]))
      _iCc++ ;

    if ('|' == _sSource[_iCc])
    {
      _iCc++ ;
      size_t iDeb = _iCc ;
      while ((_iCc < strlen(_sSource.c_str())) && (ESC_CHAR != _sSource[_iCc]) && ('|' != _sSource[_iCc]))
        _iCc++ ;

      if ('|' == _sSource[_iCc])
      {
        sChemin = string(_sSource, iDeb, _iCc - iDeb) ;
        size_t iCrochet1 = sChemin.find('[') ;
        if (NPOS == iCrochet1)
          sChemin = "" ;
        else
        {
          size_t iCrochet2 = sChemin.find(']') ;
          if (NPOS == iCrochet2)
            sChemin = "" ;
          else
          {
            sChemin = string(sChemin, iCrochet1 + 1, iCrochet2 - iCrochet1 - 1) ;
            // modif RS du 05/04/04 : on enlève la racine pour plus de généricité dans les templates
            size_t iFirst = sChemin.find('/');
            if (NPOS == iFirst)
              sChemin = "/";
            else
              sChemin = string(sChemin, iFirst+1, strlen(sChemin.c_str())-iFirst-1);
          }
        }
      }
    }

    // lecture jusqu'au prochain marqueur ou fin de fichier
    while ((_iCc < strlen(_sSource.c_str())) && (ESC_CHAR != _sSource[_iCc]))
      _iCc++ ;

    if (_iCc < strlen(_sSource.c_str()))
      _iCc++ ;						// lecture du char ESC_CHAR
    else
      break ;

    // recuperation du type (debut)
    if (_iCc < strlen(_sSource.c_str()))
    {
      typeFils  = typeHtml(_sSource[_iCc++]) ;
      bOk       = lireFils(pCR, typeFils, sChemin) ;
    }
  }
  return (bOk) ;
}

// -----------------------------------------------------------------------------
//
// Classe NSHtmlTXT
//
// -----------------------------------------------------------------------------

NSHtmlTXT::NSHtmlTXT(OperationType typeOp, NSNoyauDocument *pDocNoy, const char *fichier, NSContexte *pCtx, string sDest)
          :NSModHtml(typeOp, pDocNoy, pCtx)
{
try
{
	string sTitre     = string("") ;
	string sCreateur  = string("") ;
	string sDate      = string("") ;

	if (pDocNoy)
	{
		NSSimpleTxtDocument* pTxtDoc = dynamic_cast<NSSimpleTxtDocument *>(pDocNoy) ;
    if ((NSSimpleTxtDocument*) NULL == pTxtDoc)
    	return ;

		// on récupère le titre du document (au sens TDocument)
		sTitre = string(pDocNoy->GetTitle()) ;

		// on prend la date du document
		sDate = pTxtDoc->GetDateDoc() ;

		ajoute(tTitle, sTitre) ;
		ajoute(tDate,  sDate) ;
		ajoute(tDest,  sDest) ;
		addSignature() ;
    addPatient() ;
		ajoute(new NSHtml(tTXT, StringTexteHtml(fichier, false))) ;
	}
	// En cas d'importation, il est normal de ne pas avoir de pDocNoy
	else if (typeOp == toImporter)
	{
		ajoute(new NSHtml(tTXT, StringTexteHtml(fichier, true))) ;
	}
}
catch (...)
{
	erreur("Exception NSHtmlTXT ctor", standardError, 0) ;
}
}

NSHtmlTXT::~NSHtmlTXT()
{
}

string
NSHtmlTXT::StringTexteHtml(const char* fichier, bool bImport)
{
  ifstream inFile ;
  inFile.open(fichier) ;  if (!inFile)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + string(fichier) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return string("") ;
  }

  string line  = string("") ;
  string sText = string("") ;

  while (!inFile.eof())  {
  	getline(inFile, line) ;
    sText += line + "\n" ;
  }

  inFile.close() ;

  RemplaceDynamicTags(sText);

  return texteHtml(sText) ;
}

void
NSHtmlTXT::RemplaceTag(string& sTexte, string sTag, string sValeur)
{
	size_t pos = sTexte.find(sTag) ;

	while (pos != string::npos)
	{
		// on remplace le texte dans la string et on réitère
		sTexte.replace(pos, strlen(sTag.c_str()), sValeur.c_str()) ;
		pos = sTexte.find(sTag, pos + strlen(sValeur.c_str())) ;
	}
}

void
NSHtmlTXT::RemplaceDynamicTags(string& sTexte)
{
	string sLang = pContexte->getUserLanguage() ;

	// on convertit les tags internes et on remplace le texte
	pContexte->getPatient()->fabriqueNomLong() ;
	RemplaceTag(sTexte, "[nomPatient]", pContexte->getPatient()->getNomLong()) ;

	string sDateJour = donne_date(donne_date_duJour(), sLang) ;	RemplaceTag(sTexte, "[dateJour]", sDateJour) ;
}

// -----------------------------------------------------------------------------
//
// Classe NSHtmlRTF
//
// -----------------------------------------------------------------------------
NSHtmlRTF::NSHtmlRTF(OperationType typeOp, NSNoyauDocument *pDocNoy, string sFileName, NSContexte *pCtx, string sDest)
          :NSModHtml(typeOp, pDocNoy, pCtx), _pDialog((ConvertRTFDialog *) 0)
{
try
{
  NSTtxDocument *pTtxDoc ;

  string sTitre     = string("") ;
	string sCreateur  = string("") ;
	string sDate      = string("") ;

  if (pDocNoy)
  {
    pTtxDoc = dynamic_cast<NSTtxDocument*>(pDocNoy) ;
		if ((NSTtxDocument*) NULL == pTtxDoc)
			return ;

    // on récupère le titre du document (au sens TDocument)
    sTitre = string(pDocNoy->GetTitle()) ;

    // on prend la date du document
    sDate = pTtxDoc->GetDateDoc() ;

    ajoute(tTitle, sTitre) ;
    ajoute(tDate,  sDate) ;
    ajoute(tDest,  sDest) ;
    addSignature() ;
    addPatient() ;
  }

  // En cas d'importation, il est normal de ne pas avoir de pDocNoy
  else if (typeOp == toImporter)
  {
    _pDialog = new ConvertRTFDialog(pContexte, pContexte->GetMainWindow(), sFileName) ;
    _iAlignPrec = -1 ;
    return ;
  }
  else
  {
    erreur("Le document texte ne peut pas être converti en html", warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    // on initialise le dialogue de conversion pour éviter les bugs
    _pDialog = new ConvertRTFDialog(pContexte, pContexte->GetMainWindow(), sFileName) ;
    _iAlignPrec = -1 ;
    return ;
  }

	if (string("ZTRTF") == pTtxDoc->_pDocInfo->getTypeSem())
  {
    // on initialise le dialogue de conversion et l'alignement des paragraphes
    _pDialog = new ConvertRTFDialog(pContexte, pContexte->GetMainWindow(), sFileName) ;
    _iAlignPrec = -1 ;
  }
	else if (string("ZTHTM") == pTtxDoc->_pDocInfo->getTypeSem())
  {
    // on met à jour une variable de NSDocNoy
    // pour pouvoir créer le fichier dans NSModHtml::genereModele
    pTtxDoc->_sNomFichierHtml = sFileName ;
    if (false == AnalyseXML(sFileName))
    {
      erreur("Le document Word ne peut être converti en html.", warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return ;
    }
  }
}
catch (...)
{
	erreur("Exception NSHtmlRTF ctor", standardError, 0) ;
}
}

NSHtmlRTF::~NSHtmlRTF()
{
  if (_pDialog)
    delete _pDialog ;
}

bool
NSHtmlRTF::AnalyseXML(string sNomFichierXML)
{
try
{
	if (string("") == sNomFichierXML)
  	return false ;

  ifstream 	inFile ;
  inFile.open(sNomFichierXML.c_str()) ;
	if (!inFile)
  {
    char msg[255] ;
    sprintf(msg, "Impossible d'ouvrir le fichier : %s", sNomFichierXML.c_str()) ;
    erreur(msg, standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  string sxHtmlXML = string("") ;
  string sxHeadXML = string("") ;
  string sxBodyXML = string("") ;
  string sHeadXML  = string("") ;
  string sBodyXML  = string("") ;

  string sFichierXML = string("") ;
  while (!inFile.eof())
  {
    string sLine ;
    getline(inFile, sLine) ;
    if (string("") != sLine)
      sFichierXML += sLine + string("\n") ;
  }
  inFile.close() ;

  // recherche du param html (xhtmlXML)
  size_t pos1 = sFichierXML.find("<html") ;
  if (string::npos == pos1)
    pos1 = sFichierXML.find("<HTML") ;

  if (pos1 != string::npos)
  {
    size_t pos2 = sFichierXML.find(">", pos1) ;
    if (string::npos != pos2)
      sxHtmlXML = string(sFichierXML, pos1 + 5, pos2 - (pos1 + 5)) ;
    else
      return false ;
  }
  else
    return false ;

  ajoute(new NSHtml(string("xhtmlXML"), sxHtmlXML)) ;

  // recherche du param head (xheadXML)
  pos1 = sFichierXML.find("<head") ;
  if (string::npos == pos1)
    pos1 = sFichierXML.find("<HEAD") ;

  if (string::npos != pos1)
  {
    size_t pos2 = sFichierXML.find(">", pos1) ;
    if (string::npos != pos2)
    {
      sxHeadXML = string(sFichierXML, pos1 + 5, pos2 - (pos1 + 5)) ;
      // recherche du head html (headXML)
      pos1 = pos2 + 1 ;
      pos2 = sFichierXML.find("</head>", pos1) ;
      if (string::npos == pos2)
        pos2 = sFichierXML.find("</HEAD>", pos1) ;

      if (string::npos != pos2)
        sHeadXML = string(sFichierXML, pos1, pos2 - pos1) ;
      else
        return false ;
    }
    else
      return false ;
  }
  else
  {
    sxHeadXML = string("") ;
    sHeadXML  = string("") ;
  }

  ajoute(new NSHtml(string("xheadXML"), sxHeadXML)) ;
  ajoute(new NSHtml(string("headXML"),  sHeadXML)) ;

  // recherche du param body (xbodyXML)
  pos1 = sFichierXML.find("<body") ;
  if (string::npos == pos1)
    pos1 = sFichierXML.find("<BODY") ;

  if (string::npos != pos1)
  {
    size_t pos2 = sFichierXML.find(">", pos1) ;
    if (string::npos != pos2)
    {
      sxBodyXML = string(sFichierXML, pos1 + 5, pos2 - (pos1 + 5)) ;
      // recherche du head html (headXML)
      pos1 = pos2 + 1 ;
      pos2 = sFichierXML.find("</body>", pos1) ;
      if (string::npos == pos2)
        pos2 = sFichierXML.find("</BODY>", pos1) ;

      if (string::npos != pos2)
        sBodyXML = string(sFichierXML, pos1, pos2 - pos1) ;
      else
        return false ;
    }
    else
      return false ;
  }
  else
  {
    sxBodyXML = string("") ;
    sBodyXML  = string("") ;
  }

  ajoute(new NSHtml(string("xbodyXML"), sxBodyXML)) ;
  ajoute(new NSHtml(string("bodyXML"),  sBodyXML)) ;
  return true ;
}
catch (...)
{
	erreur("Exception NSHtmlRTF::AnalyseXML", standardError, 0) ;
  return false ;
}
}

/*
void
NSHtmlRTF::RemplaceTagRTF(char far *texte, string sTag, string sValeur)
{
  if (string("") == sTag)
    return ;

  string 	sTexte = string(texte) ;
  if (string("") == sTexte)
    return ;

  size_t iTagLen = strlen(sTag.c_str()) ;

  size_t pos = sTexte.find(sTag) ;
  while (string::npos != pos)
  {
    // on remplace le texte directement dans le controle
    _pDialog->getRichEdit()->DeleteSubText(pos, pos + iTagLen) ;
    _pDialog->getRichEdit()->Insert(sValeur.c_str()) ;
    // on effectue la meme modif dans la string pour le calcul des positions
    sTexte.replace(pos, iTagLen, sValeur.c_str()) ;
    pos = sTexte.find(sTag, pos + strlen(sValeur.c_str())) ;
  }

  strcpy(texte, sTexte.c_str()) ;
}
*/

voidNSHtmlRTF::Convertir()
{
try
{
  NSHtml *pHtmlRTF = new NSHtml(tRTF) ;

  _pDialog->Create() ;
  ConvertRTF(pHtmlRTF) ;
  _pDialog->CmOk() ;

  ajoute(pHtmlRTF) ;
}
catch (...)
{
	erreur("Exception NSHtmlRTF::Convertir", standardError, 0) ;
}
}

#define PFM_TABLE_RE3         0x40000000 // RE 3.0
#define PFM_TEXTWRAPPINGBREAK	0x20000000 // RE 3.0
#define PFM_TABLEROWDELIMITER	0x10000000

void
NSHtmlRTF::ConvertRTF(NSHtml *pTexte)
{
  if ((ConvertRTFDialog*) NULL == _pDialog)
    return ;

  NSRichEdit* pRichEdit = _pDialog->getRichEdit() ;
  if ((NSRichEdit*) NULL == pRichEdit)
    return ;

  pRichEdit->ConvertToHtml(pTexte, pContexte) ;
}

/*
  // constante MAXCARS dans nsttx.h
  char far        str[MAXCARS + 255] ; // on rajoute 255 pour pouvoir faire les replace des tags

  TCharFormat far cf, cfPrec ;
  TParaFormat far pf, pfPrec ;
  PoliceParam     policeDebut, policeCourant ;
  bool            newPara = false ;

	string sLang = pContexte->getUserLanguage() ;

  // on récupère le texte
  // Getting RTF text
  //
  pRichEdit->GetSubText(str, 0, MAXCARS) ;

  // on convertit les tags internes et on remplace le texte
  //
  pContexte->getPatient()->fabriqueNomLong() ;
  RemplaceTagRTF(str, "[nomPatient]", pContexte->getPatient()->getNomLong()) ;

  RemplaceTagRTF(str, "[dateJour]", donne_date(donne_date_duJour(), sLang)) ;  pRichEdit->ClearModify() ;

  // on réalise la conversion en HTML
  //
  pRichEdit->SetSelection(0, 1) ;
  pRichEdit->GetCharFormat(cfPrec, true) ;
  TextStyle styleDebut = TypeStyle(cfPrec) ;
  TypePolice(cfPrec, policeDebut) ;
  pRichEdit->GetParaFormat(pfPrec) ;

  NSHtml *pPara = CreerNewPara(pTexte, pfPrec) ;

  string sTexteCourant = string(1, str[0]) ;

  for (size_t i = 1 ; i < strlen(str) ; i++)
  {
    pRichEdit->SetSelection(i, i + 1) ;
    pRichEdit->GetCharFormat(cf, true) ;
    TextStyle styleCourant = TypeStyle(cf) ;
    TypePolice(cf, policeCourant) ;

    // Previous char was a "new line", create new paragraph
    //
    if ('\n' == str[i-1])
    {
      pRichEdit->GetParaFormat(pf) ;

      bool bTableInformation = false ;
      if ((pf.wEffects & PFM_TABLE_RE3) || (pf.wEffects & PFM_TABLEROWDELIMITER))
        bTableInformation = true ;

      if (pf.wEffects & PFM_TABLE)
        bTableInformation = true ;

      if ((pfPrec.wAlignment    != pf.wAlignment)     ||
          (pfPrec.dxStartIndent != pf.dxStartIndent)  ||
          (pfPrec.dxOffset      != pf.dxOffset))
      {
        newPara = true ;
        pfPrec  = pf ;
      }
    }

    if (newPara)
    {
      CreerFils(pPara, policeDebut, styleDebut, texteHtml(sTexteCourant)) ;
      styleDebut    = styleCourant ;
      policeDebut   = policeCourant ;
      sTexteCourant = string("") ;
      pPara         = CreerNewPara(pTexte, pf) ;
      newPara       = false ;
    }

    if ((styleCourant == styleDebut) && (policeCourant == policeDebut))
    {
      if ('\r' != str[i])
        sTexteCourant += str[i] ;
    }
    else
    {
      CreerFils(pPara, policeDebut, styleDebut, texteHtml(sTexteCourant)) ;
      styleDebut      = styleCourant ;
      policeDebut     = policeCourant ;
      if (str[i] != '\r')
        sTexteCourant = str[i] ;      else        sTexteCourant = string("") ;    }
  }

  CreerFils(pPara, policeDebut, styleDebut, texteHtml(sTexteCourant)) ;
}
*/

/*
NSHtml*NSHtmlRTF::CreerNewPara(NSHtml *pere, TParaFormat far pf){
  int alignPara ;
  int indentPara = 0 ;

  if (pf.wAlignment == PFA_LEFT)
    alignPara = 0 ;
  if (pf.wAlignment == PFA_CENTER)
    alignPara = 1 ;
  if (pf.wAlignment == PFA_RIGHT)
    alignPara = 2 ;
  if (pf.wAlignment == PFA_JUSTIFY)    alignPara = 3 ;
  if ((!alignPara) && (pf.dwMask & PFM_STARTINDENT) && (pf.dwMask & PFM_OFFSET))
  {
    indentPara = (int)((pf.dxStartIndent + pf.dxOffset) / DIVTAB) ;
  }

  return CreerPara(pere, indentPara, alignPara) ;
}

NSHtml*
NSHtmlRTF::CreerPara(NSHtml *pere, int indent, int align)
{
  if ((NSHtml*) NULL == pere)
    return (NSHtml*) 0 ;

try
{
  NSHtml *pCourant = pere ;

  // Construction de l'indentation du paragraphe (si aligné à gauche)
  if (align == 0)
  {
    for (int i = 0 ; i < indent ; i++)
    {
      pCourant->addSon(new NSHtml(tIndent)) ;
      pCourant = pCourant->GetLastSon() ;
    }
  }

  // Construction du paragraphe
  switch (align)
  {
    case 0  : pCourant->addSon(new NSHtml(tPara)) ;
         	    break ;
    case 1  : pCourant->addSon(new NSHtml(tParaCentre)) ;
         	    break ;
    case 2  : pCourant->addSon(new NSHtml(tParaDroite)) ;
         	    break ;
    // case 3 : justify
    default : pCourant->addSon(new NSHtml(tPara)) ;
  }

  pCourant = pCourant->GetLastSon() ;

  return pCourant ;
}
catch (...)
{
	erreur("Exception NSHtmlRTF::CreerPara", standardError, 0) ;
  return NULL ;
}
}

void
NSHtmlRTF::CreerFils(NSHtml *pere, PoliceParam pp, TextStyle style, string text)
{
try
{
  pere->addSon(new NSHtml(tPolice, pp.getFace(), pp.getSize())) ;
  NSHtml *fils = pere->GetLastSon() ;

  NSHtml *pfils  = (NSHtml*) 0 ;
  NSHtml *pHtml  = (NSHtml*) 0 ;
  NSHtml *pHtml2 = (NSHtml*) 0 ;

  switch (style)
  {
    case B    : pfils = new NSHtml(tGras) ;
                pfils->setText(text) ;
                break ;

    case I    : pfils = new NSHtml(tItalic) ;
                pfils->setText(text) ;
                break ;

    case U    : pfils = new NSHtml(tSouligne) ;
                pfils->setText(text) ;
                break ;

    case BI   : pfils = new NSHtml(tGras) ;
                pfils->addSon(new NSHtml(tItalic)) ;
                pHtml = pfils->GetFirstSon() ;
                pHtml->setText(text) ;
                break ;

    case BU   : pfils = new NSHtml(tGras) ;
                pfils->addSon(new NSHtml(tSouligne)) ;
                pHtml = pfils->GetFirstSon() ;
                pHtml->setText(text) ;
                break ;

    case IU   : pfils = new NSHtml(tItalic) ;
                pfils->addSon(new NSHtml(tSouligne)) ;
                pHtml = pfils->GetFirstSon() ;
                pHtml->setText(text) ;
                break ;

    case BIU  : pfils = new NSHtml(tGras) ;
                pfils->addSon(new NSHtml(tItalic)) ;
                pHtml = pfils->GetFirstSon() ;
                pHtml->addSon(new NSHtml(tSouligne)) ;
                pHtml2 = pHtml->GetFirstSon() ;
                pHtml2->setText(text) ;
                break ;

    default   : pfils = new NSHtml(tTexte) ;
                pfils->setText(text) ;
  }

  fils->addSon(new NSHtml(*pfils)) ;
  delete pfils ;
}
catch (...)
{
	erreur("Exception NSHtmlRTF::CreerFils", standardError, 0) ;
}
}

TextStyle
NSHtmlRTF::TypeStyle(TCharFormat far cf)
{
  bool      IsBold      = false ;
  bool      IsItalic    = false ;
  bool      IsUnderline = false ;

  DWORD effects = cf.dwEffects ;
  if (effects & CFE_BOLD)
    IsBold = true ;
  if (effects & CFE_ITALIC)
    IsItalic = true ;
  if (effects & CFE_UNDERLINE)
    IsUnderline = true ;

  TextStyle style ;

  if (IsBold)
  {
    if (IsItalic)
    {
      if (IsUnderline)
        style = BIU ;
      else
        style = BI ;
    }
    else
    {
      if (IsUnderline)
        style = BU ;
      else
        style = B ;
    }
  }
  else
  {
    if (IsItalic)
    {
      if (IsUnderline)
        style = IU ;
      else
        style = I ;
    }
    else
    {
      if (IsUnderline)
        style = U ;
      else
        style = N ;
    }
  }

  return style ;
}

void
NSHtmlRTF::TypePolice(TCharFormat far cf, PoliceParam& pp)
{
  int taillePoints = (int) (cf.yHeight / 20) ;
  int tailleHtml   = (taillePoints / 2) - 3 ;
  if (tailleHtml < 1)
    tailleHtml = 1 ;
  if (tailleHtml > 7)
    tailleHtml = 7 ;

  pp.setSize(tailleHtml) ;
  pp.setFace(string(cf.szFaceName)) ;
}
*/

// fin du fichier ns_html.cpp

