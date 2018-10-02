// -----------------------------------------------------------------------------// nsdrughistoview.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.15 $
// $Author: pameline $
// $Date: 2014/01/02 20:44:50 $
// -----------------------------------------------------------------------------
// Vue Document/Vues de gestion du médicament
// Doc/View View for drug management
// -----------------------------------------------------------------------------
// PA  - mai 2018
// -----------------------------------------------------------------------------
#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

#include <owl/uihelper.h>
#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nssuper.h"#include "partage\nsdivfct.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nsdrughistoview.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nscompub.h"
#include "nautilus\nsldvvue.rh"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nspanesplitter.h"
#include "nsbb\nswhoisthere.h"
#include "nsbb\nsattvaltools.h"
#include "dcodeur\decoder.h"
#include "nsoutil\ibdm.h"
#include "nsoutil\nsDicoBdm.h"

bool
drugSortByNameInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pseumaj(pDrug1->_sTitre) < pseumaj(pDrug2->_sTitre)) ;
}

bool
drugSortByNameSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pseumaj(pDrug1->_sTitre) > pseumaj(pDrug2->_sTitre)) ;
}

bool
drugSortByPrescribeInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
  if (pDrug1->isToPrescribe() == pDrug2->isToPrescribe())
	  return (pseumaj(pDrug1->_sTitre) < pseumaj(pDrug2->_sTitre)) ;

  return pDrug1->isToPrescribe() ;
}

bool
drugSortByPrescribeSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	if (pDrug1->isToPrescribe() == pDrug2->isToPrescribe())
	  return (pseumaj(pDrug1->_sTitre) < pseumaj(pDrug2->_sTitre)) ;

  return pDrug2->isToPrescribe() ;
}

bool
drugSortByAdminInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return ((true == pDrug1->_bALD) && (false == pDrug2->_bALD)) ;
}

bool
drugSortByAdminSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return ((false == pDrug1->_bALD) && (true == pDrug2->_bALD)) ;
}

bool
drugSortByDurationInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (getDuration(pDrug1) < getDuration(pDrug2)) ;
}

bool
drugSortByDurationSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (getDuration(pDrug1) > getDuration(pDrug2)) ;
}

bool
drugSortByBeginInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->_tDateOuverture < pDrug2->_tDateOuverture) ;
}

bool
drugSortByBeginSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->_tDateOuverture > pDrug2->_tDateOuverture) ;
}

bool
drugSortByEndInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->_tDateFermeture < pDrug2->_tDateFermeture) ;
}

bool
drugSortByEndSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->_tDateFermeture > pDrug2->_tDateFermeture) ;
}

bool
drugSortByPrescrEndInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
  NSLdvDrugPhase* pPhase1 = pDrug1->getCurrentActivePhase() ;
  NSLdvDrugPhase* pPhase2 = pDrug2->getCurrentActivePhase() ;

  if ((NSLdvDrugPhase*) NULL == pPhase1)
    return true ;
  if ((NSLdvDrugPhase*) NULL == pPhase2)
    return false ;

	return (pPhase1->_tDateFermeture < pPhase2->_tDateFermeture) ;
}

bool
drugSortByPrescrEndSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
  NSLdvDrugPhase* pPhase1 = pDrug1->getCurrentActivePhase() ;
  NSLdvDrugPhase* pPhase2 = pDrug2->getCurrentActivePhase() ;

  if ((NSLdvDrugPhase*) NULL == pPhase2)
    return true ;
  if ((NSLdvDrugPhase*) NULL == pPhase1)
    return false ;

	return (pPhase1->_tDateFermeture > pPhase2->_tDateFermeture) ;
}

bool
drugSortByPrescrDateInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->getLatestPrescriptionDate() < pDrug2->getLatestPrescriptionDate()) ;
}

bool
drugSortByPrescrDateSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->getLatestPrescriptionDate() > pDrug2->getLatestPrescriptionDate()) ;
}

// -----------------------------------------------------------------------------
//
// Class NSDrugHistoView
//
// -----------------------------------------------------------------------------

const int ID_DrugList = 0x100 ;const int ID_DrugEdit = 0x101 ;const int ID_AtcEdit  = 0x102 ;
// Table de réponses de la classe NSDrugView

DEFINE_RESPONSE_TABLE1(NSDrugHistoView, NSLDVView)
  EV_VN_ISWINDOW,
  EV_LVN_GETDISPINFO(ID_DrugList, DispInfoListe),
  EV_LVN_COLUMNCLICK(ID_DrugList, LVNColumnclick),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_IMPRIME,          CmPublish),
  EV_COMMAND(CM_DRUG_HISTORY,     CmHistory),
END_RESPONSE_TABLE ;

// Constructeur
NSDrugHistoView::NSDrugHistoView(NSLdvDocument &doc, string sConcern)
                :NSLDVView(doc.pContexte, &doc, 0, string("DrugHistory"), string("LdvDoc"), sConcern)
{
try
{
  Attr.AccelTable = DRUGS_ACCEL ;

  _sLexiqCode   = string("") ;
  _sAtcCode     = string("") ;

  _pLdVDoc      = &doc ;
  _pListWindow  = new NSDrugsHistoPropertyWindow(this, doc.pContexte, ID_DrugList, 0, 0, 0, 0) ;

	initMUEViewMenu("menubar_drug") ;

  _pToolBar     = (OWL::TControlBar*) 0 ;
  bSetupToolBar = true ;
  uButtonsStyle = MYWS_ICONS ;

  _iSortedColumn = -1 ;

	setViewName() ;
}
catch (...)
{
  erreur("Exception NSDrugView ctor.", standardError, 0) ;
  return ;
}
}

// Destructeur
NSDrugHistoView::~NSDrugHistoView()
{
  delete _pListWindow ;

  string ps = string("Deleting DrugView.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSDrugHistoView::setViewName()
{
	sViewName = pContexte->getSuperviseur()->getText("drugHistory", "drugHistory") ;

  addConcernTitle() ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow
*NSDrugHistoView::GetWindow()
{
  return (TWindow *) this ;
}

// Appel de la fonction de remplissage de la vuevoid
NSDrugHistoView::SetupWindow()
{
  NSMUEView::SetupWindow() ;
  Parent->SetCaption(sViewName.c_str()) ;

  SetupColumns() ;

  getCodes() ;

  AfficheListe() ;

  SetWindowPosDH() ;
}

/**
 * Set of operation to be executed after a drug has been selected
 */
void
NSDrugHistoView::getCodes()
{
  NSDrugHistoryDlg selectionDlg(this, pContexte) ;

  if (selectionDlg.Execute() == IDCANCEL)
	  return ;

  sViewName = pContexte->getSuperviseur()->getText("drugHistory", "drugHistory") ;

  _sLexiqCode = selectionDlg.getDrugCode() ;
  if (string("") != _sLexiqCode)
    sViewName += string(" ") + selectionDlg.getDrugLabel() ;

  _sAtcCode   = selectionDlg.getAtcCode() ;
  if (string("") != _sAtcCode)
  {
    sViewName += string(" ATC = ") + _sAtcCode ;
    if (string("") != selectionDlg.getAtcLabel())
      sViewName += string(" (") + selectionDlg.getAtcLabel() + string(")") ;
  }

  Parent->SetCaption(sViewName.c_str()) ;
  Parent->Invalidate() ;

  initDrugs() ;
}

/**
 * Set of operation to be executed after a drug has been selected
 */
void
NSDrugHistoView::ExecutedAfterDrugSelection()
{
	string sLexiqCode = _pDrugModel->getCode() ;

  if (string("") == sLexiqCode)
		return ;
}

void
NSDrugHistoView::CmPublish()
{
  NSLdvPubli publiDriver ;
  publiDriver._iPrintType = NSLdvPubli::printDrugs ;
  publiDriver._pPrintArray = (void*) &_aDrugs ;

  _pLdVDoc->Publish(&publiDriver) ;
}

void
NSDrugHistoView::reloadView(string sReason)
{
  initDrugs() ;
  AfficheListe() ;
}

void
NSDrugHistoView::initDrugs()
{
  if      (string("") != _sLexiqCode)
    initDrugsForConcept() ;
  else if (string("") != _sAtcCode)
    initDrugsForAtcCode() ;
}

void
NSDrugHistoView::initDrugsForConcept()
{
  _aDrugs.vider() ;

  if (string("") == _sLexiqCode)
    return ;

  string sCodeSensConcept ;
  pContexte->getDico()->donneCodeSens(&_sLexiqCode, &sCodeSensConcept) ;

  // We could imagine that we get once here all the "IS A" elements for the
  // concept, but for some concepts (for example "antihypertension drug")
  // the "IS A" tree contains thousands of elements and takes a long time to
  // obtain.
  //

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    ArrayLdvDrugs *pDrugs = _pLdVDoc->getDrugs(iFrame) ;
    if (pDrugs && (false == pDrugs->empty()))
    {
      for (drugsIter itDg = pDrugs->begin() ; pDrugs->end() != itDg ; itDg++)
      {
        string sLexique = (*itDg)->getLexique() ;

        string sCodeSensItDg ;
        pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSensItDg) ;

        VectString VSEquivalent ;
        pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sCodeSensItDg, "ES", &VSEquivalent) ;

        if (VSEquivalent.contains(sCodeSensConcept))
        {
          (*itDg)->setLatestPrescriptionDate(_pLdVDoc->getPrescriptionDate(*itDg)) ;
          _aDrugs.push_back(*itDg) ;
        }
      }
    }
  }
}

void
NSDrugHistoView::initDrugsForAtcCode()
{
  _aDrugs.vider() ;

  if (string("") == _sAtcCode)
    return ;

  size_t iCodeLen = strlen(_sAtcCode.c_str()) ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    ArrayLdvDrugs *pDrugs = _pLdVDoc->getDrugs(iFrame) ;
    if (pDrugs && (false == pDrugs->empty()))
    {
      for (drugsIter itDg = pDrugs->begin() ; pDrugs->end() != itDg ; itDg++)
      {
        size_t iItCodeLen = strlen((*itDg)->getAtcCode().c_str()) ;

        if ((iItCodeLen >= iCodeLen) && (string((*itDg)->getAtcCode(), 0, iCodeLen) == _sAtcCode))
        {
          (*itDg)->setLatestPrescriptionDate(_pLdVDoc->getPrescriptionDate(*itDg)) ;
          _aDrugs.push_back(*itDg) ;
        }
      }
    }
  }
}

// Initialisation des colonnes de la ListWindowvoid
NSDrugHistoView::SetupColumns()
{
	string sDrugName  = pContexte->getSuperviseur()->getText("drugManagement", "drugName") ;
  string sDrugOrdo  = pContexte->getSuperviseur()->getText("drugManagement", "drugOrdo") ;
  string sDrugDose  = pContexte->getSuperviseur()->getText("drugManagement", "drugDose") ;
  string sDrugDurat = pContexte->getSuperviseur()->getText("drugManagement", "duration") ;
  string sAdminInfo = pContexte->getSuperviseur()->getText("drugManagement", "adminInformation") ;
  string sStartDate = pContexte->getSuperviseur()->getText("drugManagement", "startingDate") ;
  string sEndDate   = pContexte->getSuperviseur()->getText("drugManagement", "endingDate") ;
  string sPrEndDate = pContexte->getSuperviseur()->getText("drugManagement", "prescriptionEndingDate") ;
  string sPrescDate = pContexte->getSuperviseur()->getText("drugManagement", "prescriptionDate") ;

  _pListWindow->InsertColumn(0, TListWindColumn((char*)sDrugName.c_str(), 300, TListWindColumn::Left, 0)) ;
  _pListWindow->InsertColumn(1, TListWindColumn((char*)sDrugOrdo.c_str(),  20, TListWindColumn::Center,  1)) ;
  _pListWindow->InsertColumn(2, TListWindColumn((char*)sDrugDose.c_str(),  65, TListWindColumn::Left, 2)) ;
  _pListWindow->InsertColumn(3, TListWindColumn((char*)sDrugDurat.c_str(), 65, TListWindColumn::Left, 3)) ;
  _pListWindow->InsertColumn(4, TListWindColumn((char*)sAdminInfo.c_str(), 40, TListWindColumn::Left, 4)) ;
  _pListWindow->InsertColumn(5, TListWindColumn((char*)sStartDate.c_str(), 80, TListWindColumn::Left, 5)) ;
  _pListWindow->InsertColumn(6, TListWindColumn((char*)sEndDate.c_str(),   80, TListWindColumn::Left, 6)) ;
  _pListWindow->InsertColumn(7, TListWindColumn((char*)sPrEndDate.c_str(), 80, TListWindColumn::Left, 7)) ;
  _pListWindow->InsertColumn(8, TListWindColumn((char*)sPrescDate.c_str(), 90, TListWindColumn::Left, 8)) ;
}

// Affichage des éléments de la listevoid
NSDrugHistoView::AfficheListe()
{
  _pListWindow->DeleteAllItems() ;

  if (_aDrugs.empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  drugsIter itDg = _aDrugs.end() ;
  do
  {
    itDg-- ;
    TListWindItem Item(((*itDg)->_sTitre).c_str(), 0) ;

    if ((*itDg)->isToPrescribe())
      Item.SetStateImage(0) ;
    else
      Item.SetStateImage(1) ;

    _pListWindow->InsertItem(Item) ;
	}
  while (_aDrugs.begin() != itDg) ;
}

void
NSDrugHistoView::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  TListWindItem &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  NSLdvDrug *pDrug = _aDrugs[index] ;
  if ((NSLdvDrug*) NULL == pDrug)
    return ;

  int iItem = dispInfoItem.GetSubItem() ;

	string sLang = pContexte->getUserLanguage() ;

  NSLdvDrugPhase* pPhase     = (NSLdvDrugPhase*) 0 ;
  NSLdvDrugPhase* pLastPhase = (NSLdvDrugPhase*) 0 ;

  if ((1 == iItem) || (2 == iItem) || (6 == iItem))
  {
    pPhase = pDrug->getCurrentActivePhase() ;
    if ((NSLdvDrugPhase*) NULL == pPhase)
      pLastPhase = pDrug->getLastActivePhase() ;
  }

  string sText = string("") ;

  // Affiche les informations en fonction de la colonne  switch (iItem)
  {
    case 1  : // to be prescribed

      if (pDrug->isToPrescribe())
      {
        sText = string("X") ;
        dispInfoItem.SetStateImage(0) ;
      }
      else
        dispInfoItem.SetStateImage(1) ;

      break ;

    case 2  : // dose

      if (pPhase)
        sText = pPhase->_sTitreCourt ;
      else if (pLastPhase)
        sText = string("(") + pLastPhase->_sTitreCourt + string(")") ;
      break ;

    case 3  : // durée
    {
      unsigned long ulDeltaDays = getDuration(pDrug) ;
      if ((unsigned long) 0 == ulDeltaDays)
        return ;

      string sNumberOf  = string("") ;
      string sDeltaDays = IntToString((int)ulDeltaDays) ;

      GlobalDkd Dcode(pContexte, sLang) ;
      string sUnitCode = string("2DAT01") ;
      if (ulDeltaDays > 1)
        sUnitCode += string(1, intranodeSeparationMARK) + string("WPLUR1") ;
      GENRE iGenreObj ;
      int   iCertObj ;
      string sUnitLib = Dcode.decodeMot(&sUnitCode, genreNull, &iGenreObj, &iCertObj) ;

      sText = sDeltaDays + string(" ") + sUnitLib ;

      if ((NSLdvDrugPhase*) NULL == pPhase)
        sText = string("(") + sText + string(")") ;
    }
    break ;

    case 4  : // ALD, etc

      if (true == pDrug->_bALD)
        sText = string("ALD") ;
      break ;

    case 5  : // date début

      sText = donne_date(pDrug->_tDateOuverture.donneDate(), sLang) ;
      break ;

    case 6  : // date fin    	if (pDrug->_tDateFermeture.estNoLimit())
      {
        sText = pContexte->getSuperviseur()->getText("drugManagement", "chronicTreatment") ;
        if (string("") == sText)
          sText = string("...") ;
      }
      else
        sText = donne_date(pDrug->_tDateFermeture.donneDate(), sLang) ;
      break ;

    case 7  : // date fin prescription

      if (pPhase)
      {
    	  if (false == pPhase->_tDateFermeture.estVide())
        {
          NVLdVTemps tpsNow ;
          tpsNow.takeTime() ;

          GENRE     iGenreObj ;
          int       iCertObj ;
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
            string sUnitLib = Dcode.decodeMot(&sUnitCode, genreNull, &iGenreObj, &iCertObj) ;
            sNumberOf = sDeltaMonths + string(" ") + sUnitLib ;
          }
          else
          {
            string sDeltaDays = IntToString((int)ulDeltaDays) ;
            string sUnitCode = string("2DAT01") ;
            if (ulDeltaDays > 1)
              sUnitCode += string(1, intranodeSeparationMARK) + string("WPLUR1") ;
            string sUnitLib = Dcode.decodeMot(&sUnitCode, genreNull, &iGenreObj, &iCertObj) ;
            sNumberOf = sDeltaDays + string(" ") + sUnitLib ;
          }

          sText = donne_date(pPhase->_tDateFermeture.donneDate(), sLang) ;

          if (string("") != sNumberOf)
            sText = sNumberOf + string(" (") + sText + string(")") ;
        }
      }
      break ;

    case 8  : // prescription

      // string sPrescriptionDate = pDrug->getPrescriptionDate() ;
      string sPrescriptionDate = pDrug->getLatestPrescriptionDate() ;

      if (string("") != sPrescriptionDate)
        sText = donne_date(sPrescriptionDate, sLang) ;
      else
      	sText = pContexte->getSuperviseur()->getText("drugManagement", "notPrescribed") ;
			break ;
  }

  static char buffer[255] ;
  buffer[0] = '\0' ;

  sprintf(buffer, "%s", sText.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
NSDrugHistoView::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
    case 0  : sortByName() ;       break ;
    case 1  : sortByPrescribe() ;  break ;
    case 3  : sortByDuration() ;   break ;
    case 4  : sortByAdmin() ;      break ;
    case 5  : sortByBegining() ;   break ;
    case 6  : sortByEnding() ;     break ;
    case 7  : sortByPrescrEnd() ;  break ;
    case 8  : sortByPrescrDate() ; break ;
  }
}

void
NSDrugHistoView::sortByName()
{
  if (0 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 0 ;
    _bNaturallySorted = true ;
  }

  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByNameInf) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByNameSup) ;

  AfficheListe() ;
}

void
NSDrugHistoView::sortByPrescribe()
{
  if (1 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 1 ;
    _bNaturallySorted = true ;
  }

  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByPrescribeInf) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByPrescribeSup) ;

  AfficheListe() ;
}

void
NSDrugHistoView::sortByAdmin()
{
  if (4 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 4 ;
    _bNaturallySorted = true ;
  }

  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByAdminInf) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByAdminSup) ;

  AfficheListe() ;
}

void
NSDrugHistoView::sortByDuration()
{
  if (3 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 3 ;
    _bNaturallySorted = true ;
  }

  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByDurationInf) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByDurationSup) ;

  AfficheListe() ;
}

void
NSDrugHistoView::sortByEnding()
{
  if (6 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 6 ;
    _bNaturallySorted = true ;
  }

  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByEndSup) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByEndInf) ;

  AfficheListe() ;
}

void
NSDrugHistoView::sortByPrescrEnd()
{
  if (7 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 7 ;
    _bNaturallySorted = true ;
  }

  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByPrescrEndSup) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByPrescrEndInf) ;

  AfficheListe() ;
}

void
NSDrugHistoView::sortByBegining()
{
  if (5 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 5 ;
    _bNaturallySorted = true ;
  }

  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByBeginSup) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByBeginInf) ;

  AfficheListe() ;
}

void
NSDrugHistoView::sortByPrescrDate()
{
  if (8 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 8 ;
    _bNaturallySorted = true ;
  }

  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByPrescrDateSup) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), drugSortByPrescrDateInf) ;

  AfficheListe() ;
}

boolNSDrugHistoView::VnIsWindow(HWND hWnd){
  return (HWindow == hWnd) ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindowvoid
NSDrugHistoView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  NSMUEView::EvSize(sizeType, size) ;
  _pListWindow->MoveWindow(GetClientRect(), true) ;
}

// fonction EVSetFocusvoid
NSDrugHistoView::EvSetFocus(HWND hWndLostFocus)
{
	NSMUEView::EvSetFocus(hWndLostFocus);

  focusFct() ;

  _pListWindow->SetFocus() ;
}

// fonction Click droit : menu contextuel
void
NSDrugHistoView::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
  return ;

	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point;

	TPopupMenu *menu = new TPopupMenu();

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	ClientToScreen(lp);
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
	delete menu;
}

void
NSDrugHistoView::EvRButtonDownOut(uint modkeys, NS_CLASSLIB::TPoint& point)
{
  return ;

	// création d'un menu popup
  NS_CLASSLIB::TPoint lp = point ;

  TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}

void
NSDrugHistoView::focusFct()
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
  pContexte->setAideCorps("medicaments.htm") ;
}

// SetupToolBarvoid
NSDrugHistoView::SetupToolBar()
{
	_pPaneSplitter->FlushControlBar() ;

  TButtonGadget* pDrugHist = new TButtonGadget(CM_DRUG_HISTORY, CM_GENERAL_FCT7, TButtonGadget::Command) ;
  _pPaneSplitter->Insert(*pDrugHist) ;

  _pPaneSplitter->LayoutSession() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  string sHist = pSuper->getText("drugManagement", "drugsHistory") ;

  _pPaneSplitter->SetTootipText(CM_GENERAL_FCT7, sHist) ;
}

void
NSDrugHistoView::initList()
{
}

NSLdvDrug*
NSDrugHistoView::getDrug(size_t iIndex)
{
  if (iIndex >= _aDrugs.size())
    return (NSLdvDrug*) 0 ;

  return _aDrugs[iIndex] ;
}

void
NSDrugHistoView::CmFct7()
{
  CmHistory() ;
}

void
NSDrugHistoView::CmHistory()
{
  getCodes() ;
  AfficheListe() ;
}

/**
 * Set Window size, getting width and height from the drug management window
 */
void
NSDrugHistoView::SetWindowPosDH()
{
  NSWindowProperty* pWinProp = pContexte->getUtilisateur()->getWindowProperty(string("DrugManagement")) ;
  if ((NSWindowProperty*) NULL == pWinProp)
	{
  	Parent->SetWindowPos(0, 0, 0, 200, 100, SWP_NOZORDER) ;
    Parent->Invalidate() ;
    return ;
	}

  Parent->Show(SW_HIDE) ;

  NS_CLASSLIB::TRect targetRect = pWinProp->getRect(pContexte->getSuperviseur()) ;

  //
  // fixer la nouvelle position
  // (on ne tient pas compte de la taille, vu le probleme pour restaurer
  //  une fenetre TView,TWindow mise en icone)
  //
  Parent->SetWindowPos(0, 0, 0, targetRect.Width(), targetRect.Height(), SWP_NOZORDER) ;
  Parent->Show(SW_SHOWNORMAL) ;
  Parent->Invalidate() ;
}

unsigned long getDuration(NSLdvDrug* pLdvDrug)
{
  if ((NSLdvDrug*) NULL == pLdvDrug)
    return (unsigned long) 0 ;

  NSLdvDrugPhase* pPhase = pLdvDrug->getCurrentActivePhase() ;
  if ((NSLdvDrugPhase*) NULL == pPhase)
    pPhase = pLdvDrug->getLastActivePhase() ;

  if ((NSLdvDrugPhase*) NULL == pPhase)
    return (unsigned long) 0 ;

  NVLdVTemps tpsStart = pPhase->_tDateOuverture ;
  NVLdVTemps tpsEnd   = pPhase->_tDateFermeture ;

  return tpsEnd.daysBetween(tpsStart) ;
}

string getPrescriptionDate(NSLdvDrug* pLdvDrug)
{
  if ((NSLdvDrug*) NULL == pLdvDrug)
    return string("") ;

  NSPatientChoisi* pPatient = pLdvDrug->pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatient)
    return string("") ;

  NSLinkManager* pGraphe = pPatient->getGraphPerson()->getLinkManager() ;

  string sNodeMedic = pLdvDrug->getNoeud() ;
  string sNodeRenew = pLdvDrug->_pDoc->getDrugs(pLdvDrug->getIFrame())->getLastRenewNode(sNodeMedic) ;
  if ((string("") != sNodeRenew) && (sNodeRenew != sNodeMedic))
    sNodeMedic = sNodeRenew ;

  // on récupère le lien éventuel sur un traitement
  VecteurString Traitement ;
  pGraphe->TousLesVrais(sNodeMedic, NSRootLink::treatmentOf, &Traitement) ;
  if (Traitement.empty())
    return string("") ;

  // on récupère le premier noeud pour avoir le code document ordonnance
  string sNodeOrdo = *(*(Traitement.begin())) ;
  string sDocOrdo  = string(sNodeOrdo, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

  // on recherche le pDocHis correspondant dans l'historique
  // et on affiche la date du document (en général date de création)
  if (pPatient->getDocHis())
  {
    NSHISTODocument* pDocHis = pPatient->getDocHis() ;

    DocumentIter iterDoc = pDocHis->TrouveDocHisto(sDocOrdo) ;
    if ((DocumentIter) NULL != iterDoc)
      return (*iterDoc)->getDateDoc() ;
  }

  return string("") ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSDrugsPropertyWindow//
// -----------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(NSDrugsHistoPropertyWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONDOWN,
  // EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSDrugsHistoPropertyWindow::NSDrugsHistoPropertyWindow(NSDrugHistoView *parent, NSContexte* pCtx, int id, int x, int y, int w, int h, TModule *module)
                           :NSSkinableListWindow((TWindow *) parent, pCtx, id, x, y, w, h, module)
{
  _pView = parent ;
  _iRes  = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

  _pImages = (OWL::TImageList*) 0 ;
}
NSDrugsHistoPropertyWindow::~NSDrugsHistoPropertyWindow()
{
  if (_pImages)
		delete _pImages ;
}

void
NSDrugsHistoPropertyWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  NSSkinableListWindow::SetupWindow() ;

  skinSwitchOn("patientListOn") ;

  HINSTANCE hInstModule = *GetApplication() ;

  _pImages = new TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR4, 15, 5) ;
  _pImages->Add(OWL::TBitmap(hInstModule, DRUG_ORDER_YES)) ;
  _pImages->Add(OWL::TBitmap(hInstModule, DRUG_ORDER_NO)) ;

  SetImageList(*_pImages, TListWindow::State) ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSDrugsHistoPropertyWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  return ;

/*
	TLwHitTestInfo info(point) ;
  HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  	_pView->CmChange() ;
  else
  	_pView->CmAdd() ;
*/
}

void
NSDrugsHistoPropertyWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
  return ;

/*
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    _pView->EvRButtonDown(modkeys, point) ;
  }
  else
  	_pView->EvRButtonDownOut(modkeys, point) ;
*/
}

void
NSDrugsHistoPropertyWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
/*
	if      (VK_DELETE == key)
  	_pView->CmSuppress() ;
  else if (VK_INSERT == key)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	_pView->CmChange() ;
    else
    	_pView->CmAdd() ;
  }
  else if (VK_TAB == key)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	_pView->setFocusToPrevSplitterView() ;
    else
    	_pView->setFocusToNextSplitterView() ;
  }
  else
  	TListWindow::EvKeyDown(key, repeatCount, flags) ;
*/
}

void
NSDrugsHistoPropertyWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	NSContexte  *pContexte  = _pView->pContexte ;
  NSSuper     *pSuper     = pContexte->getSuperviseur() ;

  if (pSuper->bDragAndDrop)
  {
  	pSuper->DragDrop->DragLeave( *this ) ;
    pSuper->DragDrop->EndDrag() ;
    delete pSuper->DragDrop ;
    pSuper->DragDrop = 0 ;
    pSuper->bDragAndDrop = false ;
    return ;
  }

  TListWindow::EvLButtonUp(modKeys, point) ;
}

void
NSDrugsHistoPropertyWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  return ;

/*
  TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;  if (indexItem < 0)    return ;

  NSLdvDrug *pDrug = _pView->getDrug(indexItem) ;
  if ((NSLdvDrug*) NULL == pDrug)
    return ;

  bool bNewState = (false == pDrug->isToPrescribe()) ;

  _pView->setDrugPrescribeStatus(pDrug, bNewState) ;

  switchPrescribeImage(indexItem, bNewState) ;

  TListWindow::EvLButtonDown(modKeys, point) ;

  // if (_pView->isSortedOnPrescribe())
  //  _pView->sortByPrescribe() ;

  _pView->Invalidate() ;
*/
}

void
NSDrugsHistoPropertyWindow::switchPrescribeImage(int iItem, bool bToPrescribe)
{
  TListWindItem item ;
  item.SetIndex(iItem) ;
  GetItem(item) ;

  item.SetStateImage(bToPrescribe ? 0 : 1) ;

  SetItem(item) ;
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSDrugsHistoPropertyWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0 ; i < count ; i++)  {  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }
  }

	return index ;}

void
NSDrugsHistoPropertyWindow::EvSetFocus(HWND hWndLostFocus)
{
  // Invalidate();

  _pView->focusFct() ;

  int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSDrugsHistoPropertyWindow::EvKillFocus(HWND hWndGetFocus)
{
  // Invalidate() ;
}

