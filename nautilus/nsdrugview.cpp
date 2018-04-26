// -----------------------------------------------------------------------------// nsdrugview.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.15 $
// $Author: pameline $
// $Date: 2014/01/02 20:44:50 $
// -----------------------------------------------------------------------------
// Vue Document/Vues de gestion du médicament
// Doc/View View for drug management
// -----------------------------------------------------------------------------
// modif FLP - mars 2004
// PA  - juillet 2003
// -----------------------------------------------------------------------------
#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

#include <owl/uihelper.h>
#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>

#include "nautilus\nssuper.h"#include "partage\nsdivfct.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nsdrugview.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nscompub.h"
#include "nautilus\nsldvvue.rh"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nspanesplitter.h"
#include "nsbb\nsattvaltools.h"
#include "dcodeur\decoder.h"

bool
drugSortByNameInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pseumaj(pDrug1->sTitre) < pseumaj(pDrug2->sTitre)) ;
}

bool
drugSortByNameSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pseumaj(pDrug1->sTitre) > pseumaj(pDrug2->sTitre)) ;
}

bool
drugSortByPrescribeInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
  if (pDrug1->isToPrescribe() == pDrug2->isToPrescribe())
	  return (pseumaj(pDrug1->sTitre) < pseumaj(pDrug2->sTitre)) ;

  return pDrug1->isToPrescribe() ;
}

bool
drugSortByPrescribeSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	if (pDrug1->isToPrescribe() == pDrug2->isToPrescribe())
	  return (pseumaj(pDrug1->sTitre) < pseumaj(pDrug2->sTitre)) ;

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
	return (pDrug1->tDateOuverture < pDrug2->tDateOuverture) ;
}

bool
drugSortByBeginSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->tDateOuverture > pDrug2->tDateOuverture) ;
}

bool
drugSortByEndInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->tDateFermeture < pDrug2->tDateFermeture) ;
}

bool
drugSortByEndSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->tDateFermeture > pDrug2->tDateFermeture) ;
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

	return (pPhase1->tDateFermeture < pPhase2->tDateFermeture) ;
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

	return (pPhase1->tDateFermeture > pPhase2->tDateFermeture) ;
}

bool
drugSortByPrescrDateInf(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->getPrescriptionDate() < pDrug2->getPrescriptionDate()) ;
}

bool
drugSortByPrescrDateSup(NSLdvDrug *pDrug1, NSLdvDrug *pDrug2)
{
	return (pDrug1->getPrescriptionDate() > pDrug2->getPrescriptionDate()) ;
}

// -----------------------------------------------------------------------------
//
// Class NSDrugView
//
// -----------------------------------------------------------------------------

const int ID_DrugList = 0x100 ;
// Table de réponses de la classe NSDrugView

DEFINE_RESPONSE_TABLE1(NSDrugView, NSMUEView)
  EV_VN_ISWINDOW,
  EV_LVN_GETDISPINFO(ID_DrugList, DispInfoListe),
  EV_LVN_COLUMNCLICK(ID_DrugList, LVNColumnclick),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_DRUG_NEW,         CmAdd),
  EV_COMMAND(CM_REFERENTIAL,      CmFct3),
  EV_COMMAND(IDC_NEW_REF,         CmFct5),
  EV_COMMAND(IDC_ADD_TO_REF,      CmFct6),
  EV_COMMAND(CM_DRUG_RENEW,       CmContinue),
  EV_COMMAND(CM_DRUG_CHANGE,      CmChange),
  EV_COMMAND(CM_DRUG_MODIF_POSO,  CmModifPoso),
  EV_COMMAND(CM_DRUG_STOP,        CmClose),
  EV_COMMAND(CM_DRUG_DELETE,      CmSuppress),
  EV_COMMAND(IDC_ORDONNANCE,      CmFct1),
  // EV_COMMAND(IDC_ORDO_SEL,        CmFct4),
  EV_COMMAND(IDC_MANAGE_RIGHTS,   CmRights),
  EV_COMMAND(CM_IMPRIME,          CmPublish),
END_RESPONSE_TABLE ;

// Constructeur
NSDrugView::NSDrugView(NSLdvDocument &doc, string sConcern)
//  : NSMUEView(pCtx, &doc, parent, string("DrugManagement"), sPreoccu)
           : NSLDVView(doc.pContexte, &doc, 0, string("DrugManagement"), string("LdvDoc"), sConcern)
{
try
{
  Attr.AccelTable = DRUGS_ACCEL ;

  _pLdVDoc      = &doc ;
  _pListeWindow = new NSDrugsPropertyWindow(this, doc.pContexte, ID_DrugList, 0, 0, 0, 0) ;

	initMUEViewMenu("menubar_drug") ;

  _pToolBar     = (OWL::TControlBar*) 0 ;
  bSetupToolBar = true ;
  uButtonsStyle = MYWS_ICONS ;

  _iSortedColumn = -1 ;

  // Initialisation des médicaments actifs
  initCurentDrugs() ;

	setViewName() ;
}
catch (...)
{
  erreur("Exception NSDrugView ctor.", standardError, 0) ;
  return ;
}
}

// Destructeur
NSDrugView::~NSDrugView()
{
  string ps = string("Deleting DrugView.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSDrugView::setViewName()
{
	sViewName = pContexte->getSuperviseur()->getText("drugManagement", "drugManagement") ;

  addConcernTitle() ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow
*NSDrugView::GetWindow()
{
  return (TWindow *) this ;
}

// Appel de la fonction de remplissage de la vuevoid
NSDrugView::SetupWindow()
{
  NSMUEView::SetupWindow() ;
  Parent->SetCaption(sViewName.c_str()) ;

  SetupColumns() ;
  AfficheListe() ;
}

void
NSDrugView::CmAdd()
{
/*
try
{
	if (!(pContexte->userHasPrivilege(NSContexte::createDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	string sChoixMed ;

	//NSNewTrt        *pNewTrtDlg = new NSNewTrt(this, &sChoixMed, sPreoccup, pContexte, "GPRSC1") ;
	///	if (pNewTrtDlg->Execute() == IDCANCEL)
	//	return ;
	//if (sChoixMed == "")
	// return ;

	NSPatPathoArray PPT(pContexte) ;
  //pPPT->ajoutePatho(sChoixMed, 0, 0) ;

	// NSMedicDlg      *pMedicDlg  = new NSMedicDlg(this, pContexte, pPPT) ;
  // if (pMedicDlg->Execute() != IDOK)
  // {
  //    delete pPPT ;
  //    return ;
  // }
  // delete pMedicDlg ;

  NSMedicamentDlg *pMedicDlg  = new NSMedicamentDlg(this, pContexte, &PPT) ;
  int iResult = pMedicDlg->Execute() ;
  delete (pMedicDlg) ;

  if (iResult != IDOK)
  	return ;

  // NSSmallBrother  *pBigBoss = new NSSmallBrother(pContexte, pPPT, 0, false) ;
  // pBigBoss->pFenetreMere    = pContexte->GetMainWindow() ;
  // pBigBoss->lance12("GPRSC1", sChoixMed) ;
  // delete pBigBoss ;

	// on enregistre ici dans l'index de santé
	if (!(PPT.empty()))
		pContexte->getPatient()->CreeTraitement(&PPT, sPreoccup) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSDrugView::CmNouveau : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSDrugView::CmNouveau.", standardError, 0) ;
}
*/

  initCurrentList() ;

	VecteurString RelatedConcerns ;
  if (string("") != sPreoccup)
  	RelatedConcerns.AddString(sPreoccup) ;

	_pLdVDoc->DrugNewService(this, string(""), &RelatedConcerns) ;
}

void
NSDrugView::CmFct3()
{
  string ps = string("User opens drugs referential list") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  initCurrentList() ;

	VecteurString RelatedConcerns ;
  if (sPreoccup != string(""))
  	RelatedConcerns.AddString(sPreoccup) ;

	_pLdVDoc->DrugFromProtocolService(this, &RelatedConcerns) ;
}

void
NSDrugView::CmAddWithCode(string sLexiCode)
{
/*
try
{
  if (sLexiCode == "")
  {
    CmNouveau() ;
    return ;
  }

  NSPatPathoArray PPT(pContexte) ;
  PPT.ajoutePatho(sLexiCode, 0, 0) ;

  NSMedicamentDlg *pMedicDlg  = new NSMedicamentDlg(this, pContexte, &PPT) ;
  int iResult = pMedicDlg->Execute() ;
  delete pMedicDlg ;

  if (iResult != IDOK)
    return ;

  // on enregistre ici dans l'index de santé
  if (!(PPT.empty()))
    pContexte->getPatient()->CreeTraitement(&PPT, sPreoccup) ;
}
catch (...)
{
  erreur("Exception NSDrugView::CmAddWithCode(string).", standardError, 0) ;
  return ;
}
*/

	VecteurString RelatedConcerns ;
  if (sPreoccup != string(""))
  	RelatedConcerns.AddString(sPreoccup) ;

	_pLdVDoc->DrugNewService(this, sLexiCode, &RelatedConcerns) ;
}

void
NSDrugView::CmContinue()
{
/*
	if (!(pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	int index = pListeWindow->IndexItemSelect() ;

	if (index == -1)
	{
		erreur("Vous devez sélectionner un médicament à renouveler dans la liste des médicaments en cours.", standardError, 0) ;
		return ;
	}

	string sNodeModif = (aCurrentDrugs[index])->getNoeud() ;

	VecteurString NodeMedic ;
	NodeMedic.push_back(new string(sNodeModif)) ;

	NSPatPathoArray PPT(pContexte) ;

	NSRenewMedicDlg* pRenewMedicDlg = new NSRenewMedicDlg(this, pContexte, &PPT);
  int iResult = pRenewMedicDlg->Execute() ;
  delete pRenewMedicDlg ;

	if (iResult == IDCANCEL)
		return ;

	// on renouvelle la précédente prescription
	pContexte->getPatient()->RenouvelerTraitement(&NodeMedic, &PPT) ;

	initCurentDrugs() ;
	AfficheListe() ;
*/

	string sNodeToAlter = getDrugRefToModify() ;
  if (string("") != sNodeToAlter)
  	_pLdVDoc->DrugRenewService(this, sNodeToAlter) ;
}

/*void
NSDrugView::autoAddInDrugView(string sConcern, NSPatPathoArray *pPPT)  */

void
NSDrugView::autoAddInDrugView(string sConcern)
{
  if (NULL == pContexte->getSuperviseur()->pBufCopie)
  	return ;

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  pContexte->getSuperviseur()->pBufCopie->initFromPatPatho(&PPT) ;

  if (PPT.empty())
  	return ;

	VecteurString NodeConcern ;
	NodeConcern.AddString(sConcern) ;
	pContexte->getPatient()->CreeTraitement(&PPT, &NodeConcern) ;

  initCurentDrugs() ;
	AfficheListe() ;
}

void
NSDrugView::CmChange()
{
/*
	if (!(pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

  int index = pListeWindow->IndexItemSelect() ;

  // if (pConcern == 0)
  // {
  //   erreur("Le médicament doit être rattaché à une préoccupation particulière.", 0, 0) ;
  //   return ;
  // }

  if (index == -1)
  {
    erreur("Vous devez sélectionner un médicament à modifier dans la liste des médicaments en cours.", standardError, 0) ;
    return ;
  }

  string sNodeModif, sCodeModif ;
  NSPatPathoArray PPT(pContexte) ;

  sNodeModif = (aCurrentDrugs[index])->getNoeud() ;
  sCodeModif = (aCurrentDrugs[index])->getLexique() ;

  // récupération de la date du jour au format AAAAMMJJ

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  // char szDateJour[10] ;
  // donne_date_duJour(szDateJour) ;

  // on récupère d'abord la sous-patpatho du noeud à modifier
  NSPatPathoArray SousPPT(pContexte) ;
  pContexte->getPatient()->DonneSousArray(sNodeModif, &SousPPT) ;
  // on reconstitue la patpatho à partir du noeud
  PPT.ajoutePatho(sCodeModif, 0, 1) ;
  // Insertion en queue (iter doit être ici egal à end())
  PPT.InserePatPatho(PPT.end(), &SousPPT, 1) ;

  // La patpatho source est constituée. On doit maintenant remplacer l'ancienne
  // date d'ouverture par la date du jour (car les modifs éventuelles datent de ce jour)
  PatPathoIter pptIt = PPT.begin() ;
  // Recherche de la date d'ouverture en sous-niveau du noeud  //  PatPathoIter pptItValeur ;  bool bDateTrouvee = false ;  int iColBase = (*pptIt)->pDonnees->getColonne() ;  pptIt++ ;  while ((pptIt != PPT.end()) && ((*pptIt)->getColonne() > iColBase))  {
    string sElemLex = (*pptIt)->getLexique() ;
    string sSens ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    if ((*pptIt)->getColonne() == iColBase + 1)
    {
      // Dates
      if (sSens == "KOUVR")
      {
        pptIt++ ;
        int iLigneBase = (*pptIt)->getLigne() ;
        string sUnite  = "" ;
        string sFormat = "" ;
        string sValeur = "" ;
        string sTemp   = "" ;

        while ( (pptIt != PPT.end()) &&
                ((*pptIt)->getLigne() == iLigneBase))
        {
          if (((*pptIt)->pDonnees->lexique)[0] == '£')
          {
            sTemp   = (*pptIt)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
            sValeur = (*pptIt)->getComplement() ;
            sTemp   = (*pptIt)->getUnit() ;
            pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
            pptItValeur = pptIt ;
            break ;
          }
          pptIt++;
        }

        // sFormat est du type £D0;03
        if ((sFormat != "") && (sFormat[1] == 'D') && (sValeur != ""))
        {
          if ((sUnite == "2DA01") || (sUnite == "2DA02"))
          {
            // on remplace la date d'ouverture par la date du jour
            strcpy((*pptItValeur)->pDonnees->complement, tpsNow.donneDateHeure().c_str()) ;

            bDateTrouvee = true ;
          }
        }
      }
      else
        pptIt++ ;
    }
    else
      pptIt++ ;
  }

  if (!bDateTrouvee)
  {
    erreur("Le médicament à modifier ne comporte pas de date d'ouverture.", standardError, 0) ;
    return ;
  }

  // on crée une copie
  NSPatPathoArray PPTCopy(PPT) ;

  // NSSmallBrother  *pBigBoss = new NSSmallBrother(pContexte, pPPT, 0, false /- initFromBbk -/) ;
  // pBigBoss->pFenetreMere = pContexte->GetMainWindow() ;
  // on lance l'archetype lié au code médicament (avec fil guide associé)
  // pBigBoss->lance12("GPRSC1", sCodeModif) ;
  // delete pBigBoss ;
  // NSMedicDlg  NSMedicamentDlg *pMedicDialog = new NSMedicamentDlg(pContexte->GetMainWindow(), pContexte, &PPT) ;     //FIXME TABUN  int iResult = pMedicDialog->Execute() ;  delete pMedicDialog ;  if (iResult != IDOK)    return ;  // Si aucune modif : on sort...  if (PPT.estEgal(&PPTCopy))    return ;  // on cloture la précédente prescription  //pContexte->getPatient()->ArreterElement(sNodeModif, string(szDateJour)) ;  // on enregistre la nouvelle prescription dans l'index de santé // if (!(pPPT->empty()))

  //delete pBigBoss ;  VecteurString NodeArret ;
  NodeArret.push_back(new string(sNodeModif)) ;  // on cloture la précédente prescription  // pContexte->getPatient()->ArreterElement(pNodeArret, string(szDateJour)) ;  pContexte->getPatient()->ArreterElement(&NodeArret, tpsNow.donneDateHeure()) ;  // on enregistre la nouvelle prescription dans l'index de santé  if (!(PPT.empty()))
    pContexte->getPatient()->CreeTraitement(&PPT, sPreoccup) ;

  initCurentDrugs() ;
  AfficheListe() ;
*/

	string sNodeToAlter = getDrugRefToModify() ;
  if (sNodeToAlter != string(""))
  	_pLdVDoc->DrugModifyService(this, sNodeToAlter) ;
}

void
NSDrugView::CmModifPoso()
{
	string sNodeToAlter = getDrugRefToModify() ;
  if (string("") != sNodeToAlter)
  	_pLdVDoc->DrugChangePosoService(this, sNodeToAlter) ;
}

void
NSDrugView::CmClose()
{
/*
	if (!(pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

  int index = pListeWindow->IndexItemSelect() ;

  if (index == -1)
  {
    erreur("Vous devez sélectionner un médicament à arrêter dans la liste des médicaments en cours.", standardError, 0) ;
    return ;
  }

  int retVal = ::MessageBox(0, "Etes-vous sûr de vouloir arrêter ce traitement ?", "Message Nautilus", MB_YESNO) ;
  if (retVal == IDNO)
    return ;

  string sNodeModif = (aCurrentDrugs[index])->getNoeud() ;
  VecteurString NodeArret ;
  NodeArret.push_back(new string(sNodeModif)) ;

  // récupération de la date du jour au format AAAAMMJJ
  // char szDateJour[10] ;
  // donne_date_duJour(szDateJour) ;  NVLdVTemps tpsNow ;  tpsNow.takeTime() ;  // on cloture la précédente prescription  // pContexte->getPatient()->ArreterElement(pNodeArret, string(szDateJour)) ;
  pContexte->getPatient()->ArreterElement(&NodeArret, tpsNow.donneDateHeure()) ;

  initCurentDrugs() ;
  AfficheListe() ;
*/

	string sWarningMsg = pContexte->getSuperviseur()->getText("drugManagementWarnings", "doYouReallyWantToStopThisPrescription") ;

	string sNodeToAlter = getDrugRefToModify(sWarningMsg) ;
  if (string("") != sNodeToAlter)
  	_pLdVDoc->DrugStopService(this, sNodeToAlter) ;
}

void
NSDrugView::CmSuppress()
{
/*
	if (!(pContexte->userHasPrivilege(NSContexte::modifyDrug, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

  int index = pListeWindow->IndexItemSelect() ;

  if (index == -1)
  {
    erreur("Vous devez sélectionner un médicament à supprimer dans la liste des médicaments en cours.", standardError, 0) ;
    return ;
  }

  int retVal = ::MessageBox(0, "Etes-vous sûr de vouloir supprimer ce traitement ?", "Message Nautilus", MB_YESNO) ;
  if (retVal == IDNO)
    return ;

  string sNodeModif = (aCurrentDrugs[index])->getNoeud() ;

  pContexte->getPatient()->SupprimerElement(sNodeModif) ;
  pLdVDoc->getDrugs()->deleteDrug(aCurrentDrugs[index]) ;

  initCurentDrugs() ;
  AfficheListe() ;
*/

  string sWarningMsg = pContexte->getSuperviseur()->getText("drugManagementWarnings", "doYouReallyWantToDeleteThisPrescription") ;

	string sNodeToAlter = getDrugRefToModify(sWarningMsg) ;
  if (sNodeToAlter != string(""))
		_pLdVDoc->DrugDeleteService(this, sNodeToAlter) ;
}

void
NSDrugView::CmPrevious()
{
  if (NULL == pContexte->getPatient())
    return ;

  NSHISTODocument* pDocManager = pContexte->getPatient()->getDocHis() ;
  if (NULL == pDocManager)
    return ;

  string sLexique = string("ZORDO1") ;
  DocumentIter iterPrevDoc = pDocManager->DonnePrevPatPathoDocument(sLexique, (NSPatPathoArray*) 0, 0) ;
  if (NULL == iterPrevDoc)
    return ;

  pDocManager->AutoriserOuverture((NSDocumentInfo*) *iterPrevDoc) ;
}

void
NSDrugView::CmFct1()
{
  // pLdVDoc->DrugCreatePrescriptionService(this) ;

  if (_aCurrentDrugs.empty())
    return ;

  VecteurString aRefStrings ;

  for (drugsIter itDg = _aCurrentDrugs.begin() ; _aCurrentDrugs.end() != itDg ; itDg++)
    if ((*itDg)->isToPrescribe())
    	aRefStrings.AddString((*itDg)->getNoeud()) ;

  _pLdVDoc->DrugCreatePrescriptionForSelectionService(this, &aRefStrings) ;

  AfficheListe() ;
}

/*
void
NSDrugView::CmFct4()
{
	VecteurString aRefStrings ;

	int count = pListeWindow->GetItemCount() ;

	for (int i = 0 ; i < count ; i++)  	if (pListeWindow->GetItemState(i, LVIS_SELECTED))
    	aRefStrings.push_back(new string((aCurrentDrugs[i])->getNoeud())) ;

  pLdVDoc->DrugCreatePrescriptionForSelectionService(this, &aRefStrings) ;
  AfficheListe() ;
}
*/

void
NSDrugView::CmFct4()
{
  bool bMustSelect = true ;

	for (drugsIter itDg = _aCurrentDrugs.begin() ; _aCurrentDrugs.end() != itDg ; itDg++)
  {
    if ((*itDg)->isToPrescribe())
    {
      bMustSelect = false ;
      break ;
    }
  }

  int item = 0 ;
  for (drugsIter itDg = _aCurrentDrugs.begin() ; _aCurrentDrugs.end() != itDg ; itDg++, item++)
  {
    setDrugPrescribeStatus(*itDg, bMustSelect) ;

    _pListeWindow->switchPrescribeImage(item, bMustSelect) ;
  }

  Invalidate() ;
}

// Create new guideline file from selection
//
void
NSDrugView::CmFct5()
{
	VecteurString aRefStrings ;

	int count = _pListeWindow->GetItemCount() ;

	for (int i = 0 ; i < count ; i++)  	if (_pListeWindow->GetItemState(i, LVIS_SELECTED))
    	aRefStrings.AddString((_aCurrentDrugs[i])->getNoeud()) ;

  _pLdVDoc->DrugCreateProtocolForSelectionService(this, &aRefStrings) ;
  AfficheListe() ;
}

// Add selection to existing guideline file
//
void
NSDrugView::CmFct6()
{
	VecteurString aRefStrings ;

	int count = _pListeWindow->GetItemCount() ;

	for (int i = 0 ; i < count ; i++)  	if (_pListeWindow->GetItemState(i, LVIS_SELECTED))
    	aRefStrings.AddString((_aCurrentDrugs[i])->getNoeud()) ;

  if (aRefStrings.empty())
    return ;

  _pLdVDoc->DrugAddToProtocolForSelectionService(this, &aRefStrings) ;
  AfficheListe() ;
}

void
NSDrugView::CmPublish()
{
  NSLdvPubli publiDriver ;
  publiDriver._iPrintType = NSLdvPubli::printDrugs ;
  publiDriver._pPrintArray = (void*) &_aCurrentDrugs ;

  _pLdVDoc->Publish(&publiDriver) ;
}

void
NSDrugView::CmRights()
{
	int index = _pListeWindow->IndexItemSelect() ;

  if (index == -1)
    return ;

  NSLdvDrug *pCurrentDrug = _aCurrentDrugs[index] ;
  if ((NSLdvDrug*) NULL == pCurrentDrug)
    return ;

  string sNodeModif = pCurrentDrug->getNoeud() ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvDrug *pDrug = _pLdVDoc->getTreatment(sNodeModif, iFrame) ;
  if (pDrug)
    _pLdVDoc->manageRights(iFrame, this, sNodeModif) ;
}

void
NSDrugView::reloadView(string sReason)
{
  initCurentDrugs() ;

  markAddedDrugs() ;

  AfficheListe() ;
}

void
NSDrugView::initCurentDrugs()
{
  _aCurrentDrugs.vider() ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    ArrayLdvDrugs *pDrugs = _pLdVDoc->getDrugs(iFrame) ;
    if (pDrugs && (false == pDrugs->empty()))
    {
      NVLdVTemps tpsNow ;
      tpsNow.takeTime() ;

      for (drugsIter itDg = pDrugs->begin() ; pDrugs->end() != itDg ; itDg++)
      {
        if (((*itDg)->tDateFermeture.estNoLimit()) || ((*itDg)->tDateFermeture >= tpsNow))
        {
          if ((sPreoccup == "") || ((*itDg)->bIsLinkedConcern(sPreoccup)))
          {
            (*itDg)->setLatestPrescriptionDate((*itDg)->getPrescriptionDate()) ;
            adaptDrugToPrescribeList(*itDg) ;

            _aCurrentDrugs.push_back(*itDg) ;
          }
        }
      }
    }
  }
}

// Initialisation des colonnes de la ListWindowvoid
NSDrugView::SetupColumns()
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

  _pListeWindow->InsertColumn(0, TListWindColumn((char*)sDrugName.c_str(), 300, TListWindColumn::Left, 0)) ;
  _pListeWindow->InsertColumn(1, TListWindColumn((char*)sDrugOrdo.c_str(),  20, TListWindColumn::Center,  1)) ;
  _pListeWindow->InsertColumn(2, TListWindColumn((char*)sDrugDose.c_str(),  65, TListWindColumn::Left, 2)) ;
  _pListeWindow->InsertColumn(3, TListWindColumn((char*)sDrugDurat.c_str(), 65, TListWindColumn::Left, 3)) ;
  _pListeWindow->InsertColumn(4, TListWindColumn((char*)sAdminInfo.c_str(), 40, TListWindColumn::Left, 4)) ;
  _pListeWindow->InsertColumn(5, TListWindColumn((char*)sStartDate.c_str(), 80, TListWindColumn::Left, 5)) ;
  _pListeWindow->InsertColumn(6, TListWindColumn((char*)sEndDate.c_str(),   80, TListWindColumn::Left, 6)) ;
  _pListeWindow->InsertColumn(7, TListWindColumn((char*)sPrEndDate.c_str(), 80, TListWindColumn::Left, 7)) ;
  _pListeWindow->InsertColumn(8, TListWindColumn((char*)sPrescDate.c_str(), 90, TListWindColumn::Left, 8)) ;
}

// Affichage des éléments de la listevoid
NSDrugView::AfficheListe()
{
  _pListeWindow->DeleteAllItems() ;

  if (_aCurrentDrugs.empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  drugsIter itDg = _aCurrentDrugs.end() ;
  do
  {
    itDg-- ;
    TListWindItem Item(((*itDg)->sTitre).c_str(), 0) ;

    if ((*itDg)->isToPrescribe())
      Item.SetStateImage(0) ;
    else
      Item.SetStateImage(1) ;

    _pListeWindow->InsertItem(Item) ;
	}
  while (_aCurrentDrugs.begin() != itDg) ;
}

void
NSDrugView::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  TListWindItem &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  NSLdvDrug *pDrug = _aCurrentDrugs[index] ;
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
        sText = pPhase->sTitreCourt ;
      else if (pLastPhase)
        sText = string("(") + pLastPhase->sTitreCourt + string(")") ;
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

      sText = donne_date(pDrug->tDateOuverture.donneDate(), sLang) ;
      break ;

    case 6  : // date fin    	if (pDrug->tDateFermeture.estNoLimit())
      {
        sText = pContexte->getSuperviseur()->getText("drugManagement", "chronicTreatment") ;
        if (string("") == sText)
          sText = string("...") ;
      }
      else
        sText = donne_date(pDrug->tDateFermeture.donneDate(), sLang) ;
      break ;

    case 7  : // date fin prescription

      if (pPhase)
      {
    	  if (false == pPhase->tDateFermeture.estVide())
        {
          NVLdVTemps tpsNow ;
          tpsNow.takeTime() ;

          GENRE     iGenreObj ;
          int       iCertObj ;
          GlobalDkd Dcode(pContexte, sLang) ;

          string sNumberOf = string("") ;
          unsigned long ulDeltaDays = pPhase->tDateFermeture.daysBetween(tpsNow) ;
          if (ulDeltaDays > 90)
          {
            unsigned long ulDeltaMonths = pPhase->tDateFermeture.monthsBetween(tpsNow) ;
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

          sText = donne_date(pPhase->tDateFermeture.donneDate(), sLang) ;

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

/*
			NSLinkManager* pGraphe = pContexte->getPatient()->pGraphPerson->pLinkManager ;
			VecteurString Traitement ;
      string sNodeMedic = (aCurrentDrugs[index])->getNoeud() ;
      string sNodeRenew = pLdVDoc->getDrugs()->getLastRenewNode(sNodeMedic) ;
      if ((sNodeRenew != "") && (sNodeRenew != sNodeMedic))
      	sNodeMedic = sNodeRenew ;
      // on récupère le lien éventuel sur un traitement
      Traitement.vider() ;
      pGraphe->TousLesVrais(sNodeMedic, NSRootLink::treatmentOf, &Traitement) ;
      if (!Traitement.empty())
      {
      	// on récupère le premier noeud pour avoir le code document ordonnance
        string sNodeOrdo = *(*(Traitement.begin())) ;
        string sDocOrdo = string(sNodeOrdo, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

        // on recherche le pDocHis correspondant dans l'historique
        // et on affiche la date du document (en général date de création)
        if ((pContexte->getPatient()) && (pContexte->getPatient()->pDocHis))
        {
        	DocumentIter iterDoc = pContexte->getPatient()->pDocHis->VectDocument.TrouveDocHisto(sDocOrdo) ;
          if ((iterDoc != NULL) && (iterDoc != pContexte->getPatient()->pDocHis->VectDocument.end()))
          {
          	strcpy(szDate, (*iterDoc)->GetDateDoc()) ;
            donne_date(szDate, buffer, sLang) ;
          }
        }
      }
*/
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
NSDrugView::LVNColumnclick(TLwNotify& lwn)
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
NSDrugView::sortByName()
{
  if (0 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 0 ;
    _bNaturallySorted = true ;
  }

  if (_aCurrentDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByNameInf) ;
  else
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByNameSup) ;

  AfficheListe() ;
}

void
NSDrugView::sortByPrescribe()
{
  if (1 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 1 ;
    _bNaturallySorted = true ;
  }

  if (_aCurrentDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByPrescribeInf) ;
  else
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByPrescribeSup) ;

  AfficheListe() ;
}

void
NSDrugView::sortByAdmin()
{
  if (4 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 4 ;
    _bNaturallySorted = true ;
  }

  if (_aCurrentDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByAdminInf) ;
  else
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByAdminSup) ;

  AfficheListe() ;
}

void
NSDrugView::sortByDuration()
{
  if (3 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 3 ;
    _bNaturallySorted = true ;
  }

  if (_aCurrentDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByDurationInf) ;
  else
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByDurationSup) ;

  AfficheListe() ;
}

void
NSDrugView::sortByEnding()
{
  if (6 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 6 ;
    _bNaturallySorted = true ;
  }

  if (_aCurrentDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByEndSup) ;
  else
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByEndInf) ;

  AfficheListe() ;
}

void
NSDrugView::sortByPrescrEnd()
{
  if (7 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 7 ;
    _bNaturallySorted = true ;
  }

  if (_aCurrentDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByPrescrEndSup) ;
  else
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByPrescrEndInf) ;

  AfficheListe() ;
}

void
NSDrugView::sortByBegining()
{
  if (5 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 5 ;
    _bNaturallySorted = true ;
  }

  if (_aCurrentDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByBeginSup) ;
  else
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByBeginInf) ;

  AfficheListe() ;
}

void
NSDrugView::sortByPrescrDate()
{
  if (8 == _iSortedColumn)
    _bNaturallySorted = (false == _bNaturallySorted) ;
  else
  {
    _iSortedColumn = 8 ;
    _bNaturallySorted = true ;
  }

  if (_aCurrentDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByPrescrDateSup) ;
  else
    sort(_aCurrentDrugs.begin(), _aCurrentDrugs.end(), drugSortByPrescrDateInf) ;

  AfficheListe() ;
}

void
NSDrugView::addToPrescribeList(string sNodeId)
{
  _aToPrescribe.AddString(sNodeId) ;
}

void
NSDrugView::removeFromPrescribeList(string sNodeId)
{
  _aToPrescribe.RemoveString(sNodeId) ;
}

bool
NSDrugView::isInsidePrescribeList(string sNodeId)
{
  return _aToPrescribe.ItemDansUnVecteur(sNodeId) ;
}

void
NSDrugView::setDrugPrescribeStatus(NSLdvDrug *pDrug, bool bToPrescribe)
{
  if ((NSLdvDrug*) NULL == pDrug)
    return ;

  pDrug->setToPrescribe(bToPrescribe) ;

  if (bToPrescribe)
    addToPrescribeList(pDrug->getNoeud()) ;
  else
    removeFromPrescribeList(pDrug->getNoeud()) ;
}

void
NSDrugView::adaptDrugToPrescribeList(NSLdvDrug *pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
    return ;

  if (isInsidePrescribeList(pDrug->getNoeud()))
    pDrug->setToPrescribe(true) ;
  else
    pDrug->setToPrescribe(false) ;
}

boolNSDrugView::VnIsWindow(HWND hWnd){
  return (HWindow == hWnd) ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindowvoid
NSDrugView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  NSMUEView::EvSize(sizeType, size) ;
  _pListeWindow->MoveWindow(GetClientRect(), true) ;
}

// fonction EVSetFocusvoid
NSDrugView::EvSetFocus(HWND hWndLostFocus)
{
	NSMUEView::EvSetFocus(hWndLostFocus);

  focusFct() ;

  _pListeWindow->SetFocus() ;
}

// fonction Click droit : menu contextuel
void
NSDrugView::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point;

	TPopupMenu *menu = new TPopupMenu();

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sNewD = pSuper->getText("drugManagement", "newDrug") ;
  string sProt = pSuper->getText("drugManagement", "newDrugFromGuideline") ;
	string sStop = pSuper->getText("drugManagement", "stopThisDrug") ;
	string sKill = pSuper->getText("drugManagement", "suppresThisDrug") ;
  string sPoso = pSuper->getText("drugManagement", "modifyPosology") ;
	string sModi = pSuper->getText("drugManagement", "modifyThisDrug") ;
	string sRene = pSuper->getText("drugManagement", "renewThisDrug") ;
	string sPres = pSuper->getText("drugManagement", "buildAPrescription") ;
	string sPreS = pSuper->getText("drugManagement", "buildAPrescriptionFromSelectedDrugs") ;
  string sProN = pSuper->getText("drugManagement", "buildAProtocolFromSelectedDrugs") ;
  string sProA = pSuper->getText("drugManagement", "addSelectedDrugsToAProtocol") ;
  string sRigh = pSuper->getText("rightsManagement", "manageRights") ;

	menu->AppendMenu(MF_STRING, CM_DRUG_NEW,    sNewD.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_REFERENTIAL, sProt.c_str()) ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, CM_DRUG_MODIF_POSO, sPoso.c_str()) ;
	menu->AppendMenu(MF_SEPARATOR, 0, 0);
	menu->AppendMenu(MF_STRING, CM_DRUG_STOP,   sStop.c_str()) ;
	menu->AppendMenu(MF_STRING, CM_DRUG_DELETE, sKill.c_str()) ;
	menu->AppendMenu(MF_STRING, CM_DRUG_CHANGE, sModi.c_str()) ;
	menu->AppendMenu(MF_STRING, CM_DRUG_RENEW,  sRene.c_str()) ;
	menu->AppendMenu(MF_SEPARATOR, 0, 0);
  menu->AppendMenu(MF_STRING, IDC_MANAGE_RIGHTS,  sRigh.c_str()) ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0);
	menu->AppendMenu(MF_STRING, IDC_ORDONNANCE, sPres.c_str()) ;
	menu->AppendMenu(MF_STRING, IDC_ORDO_SEL,   sPreS.c_str()) ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0);
  menu->AppendMenu(MF_STRING, IDC_NEW_REF,    sProN.c_str()) ;
  menu->AppendMenu(MF_STRING, IDC_ADD_TO_REF, sProA.c_str()) ;

	ClientToScreen(lp);
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
	delete menu;
}

void
NSDrugView::EvRButtonDownOut(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
  NS_CLASSLIB::TPoint lp = point ;

  TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sNewD = pSuper->getText("drugManagement", "newDrug") ;
  string sProt = pSuper->getText("drugManagement", "newDrugFromGuideline") ;
  string sPres = pSuper->getText("drugManagement", "buildAPrescription") ;
  string sPreS = pSuper->getText("drugManagement", "buildAPrescriptionFromSelectedDrugs") ;
  string sProN = pSuper->getText("drugManagement", "buildAProtocolFromSelectedDrugs") ;
  string sProA = pSuper->getText("drugManagement", "addSelectedDrugsToAProtocol") ;

  menu->AppendMenu(MF_STRING, CM_DRUG_NEW,    sNewD.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_REFERENTIAL, sProt.c_str()) ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0);
  menu->AppendMenu(MF_STRING, IDC_ORDONNANCE, sPres.c_str()) ;
  menu->AppendMenu(MF_STRING, IDC_ORDO_SEL,   sPreS.c_str()) ;
	menu->AppendMenu(MF_SEPARATOR, 0, 0);
  menu->AppendMenu(MF_STRING, IDC_NEW_REF,    sProN.c_str()) ;
  menu->AppendMenu(MF_STRING, IDC_ADD_TO_REF, sProA.c_str()) ;

  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}

void
NSDrugView::focusFct()
{
	activateMUEViewMenu() ;

  TMyApp  *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
    SetupToolBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }

  pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;
  pContexte->setAideIndex("") ;
  pContexte->setAideCorps("medicaments.htm") ;
}

// SetupToolBarvoid
NSDrugView::SetupToolBar()
{
/*
	TMyApp  *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->FlushControlBar() ;

	pMyApp->cb2->Insert(*new TButtonGadget(CM_DRUG_NEW,     CM_DRUG_NEW,    TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(CM_DRUG_STOP,    CM_DRUG_STOP,   TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(CM_DRUG_CHANGE,  CM_DRUG_CHANGE, TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(CM_DRUG_RENEW,   CM_DRUG_RENEW,  TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TSeparatorGadget) ;
  pMyApp->cb2->Insert(*new TButtonGadget(IDC_ORDONNANCE,  IDC_ORDONNANCE, TButtonGadget::Command)) ;

  pMyApp->cb2->LayoutSession() ;
*/

	pPaneSplitter->FlushControlBar() ;

  TButtonGadget* pDrugNew  = new TButtonGadget(CM_DRUG_NEW,    CM_GENERAL_ADD,      TButtonGadget::Command) ;
  TButtonGadget* pReferen  = new TButtonGadget(CM_REFERENTIAL, CM_GENERAL_FCT3,     TButtonGadget::Command) ;
  TButtonGadget* pDrugStop = new TButtonGadget(CM_DRUG_STOP,   CM_GENERAL_CLOSE,    TButtonGadget::Command) ;
  TButtonGadget* pModiPoso = new TButtonGadget(CM_DRUG_MODIF_POSO, CM_GENERAL_FCT2, TButtonGadget::Command) ;
  TButtonGadget* pDrugChng = new TButtonGadget(CM_DRUG_CHANGE, CM_GENERAL_MODIFY,   TButtonGadget::Command) ;
  TButtonGadget* pDrugRenw = new TButtonGadget(CM_DRUG_RENEW,  CM_GENERAL_CONTINUE, TButtonGadget::Command) ;
  TButtonGadget* pOrdoDate = new TButtonGadget(IDC_ORDONNANCE, CM_GENERAL_FCT1,     TButtonGadget::Command) ;
  TButtonGadget* pOrdoSele = new TButtonGadget(IDC_ORDO_SEL,   CM_GENERAL_FCT4,     TButtonGadget::Command) ;
  TButtonGadget* pNewRefer = new TButtonGadget(IDC_NEW_REF,    CM_GENERAL_FCT5,     TButtonGadget::Command) ;
  TButtonGadget* pAddRefer = new TButtonGadget(IDC_ADD_TO_REF, CM_GENERAL_FCT6,     TButtonGadget::Command) ;
	pPaneSplitter->Insert(*pDrugNew) ;
  pPaneSplitter->Insert(*pReferen) ;
  pPaneSplitter->Insert(*new TSeparatorGadget) ;
	pPaneSplitter->Insert(*pDrugStop) ;
  pPaneSplitter->Insert(*pModiPoso) ;
	pPaneSplitter->Insert(*pDrugChng) ;
	pPaneSplitter->Insert(*pDrugRenw) ;
	pPaneSplitter->Insert(*new TSeparatorGadget) ;
	pPaneSplitter->Insert(*pOrdoDate) ;
  pPaneSplitter->Insert(*pOrdoSele) ;
  pPaneSplitter->Insert(*new TSeparatorGadget) ;
  pPaneSplitter->Insert(*pNewRefer) ;
  pPaneSplitter->Insert(*pAddRefer) ;

  pPaneSplitter->LayoutSession() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
	string sNewD = pSuper->getText("drugManagement", "newDrug") ;
  string sNewR = pSuper->getText("drugManagement", "newDrugFromGuideline") ;
  string sStop = pSuper->getText("drugManagement", "stopThisDrug") ;
  string sSupp = pSuper->getText("drugManagement", "suppresThisDrug") ;
  string sPoso = pSuper->getText("drugManagement", "modifyPosology") ;
  string sModi = pSuper->getText("drugManagement", "modifyThisDrug") ;
  string sRene = pSuper->getText("drugManagement", "renewThisDrug") ;
  string sOrdo = pSuper->getText("drugManagement", "buildAPrescription") ;
  string sSele = pSuper->getText("drugManagement", "buildAPrescriptionFromSelectedDrugs") ;
  string sNPro = pSuper->getText("drugManagement", "buildAProtocolFromSelectedDrugs") ;
  string sAPro = pSuper->getText("drugManagement", "addSelectedDrugsToAProtocol") ;

  pPaneSplitter->SetTootipText(CM_GENERAL_ADD, sNewD) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_FCT3, sNewR) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_CLOSE, sStop) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_FCT2, sPoso) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_MODIFY, sModi) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_CONTINUE, sRene) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_FCT1, sOrdo) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_FCT4, sSele) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_FCT5, sNPro) ;
  pPaneSplitter->SetTootipText(CM_GENERAL_FCT6, sAPro) ;
}

string
NSDrugView::getDrugRefToModify(string sWarningMsg)
{
	int index = _pListeWindow->IndexItemSelect() ;

  if (index < 0)
  {
    string sErrorMsg = pContexte->getSuperviseur()->getText("drugManagementErrors", "aDrugMustBeSelected") ;
    erreur(sErrorMsg.c_str(), warningError, 0) ;
    return string("") ;
  }

	if (string("") != sWarningMsg)
	{
  	string sWarningTitle = pContexte->getSuperviseur()->getText("generalLanguage", "warning") ;
  	int retVal = ::MessageBox(0, sWarningMsg.c_str(), sWarningTitle.c_str(), MB_YESNO) ;
  	if (IDNO == retVal)
    	return string("") ;
	}

  NSLdvDrug *pDrug = _aCurrentDrugs[index] ;
  if ((NSLdvDrug*) NULL == pDrug)
    return string("") ;

	return pDrug->getNoeud() ;
}

void
NSDrugView::initCurrentList()
{
  _aPreviousDrugs.vider() ;

  if (_aCurrentDrugs.empty())
    return ;

  for (drugsIter itDg = _aCurrentDrugs.begin() ; _aCurrentDrugs.end() != itDg ; itDg++)
    _aPreviousDrugs.AddString((*itDg)->getNoeud()) ;
}

void
NSDrugView::markAddedDrugs()
{
  if (_aCurrentDrugs.empty())
    return ;

  for (drugsIter itDg = _aCurrentDrugs.begin() ; _aCurrentDrugs.end() != itDg ; itDg++)
    if (false == _aPreviousDrugs.ItemDansUnVecteur((*itDg)->getNoeud()))
      setDrugPrescribeStatus(*itDg, true) ;
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

  NVLdVTemps tpsStart = pPhase->tDateOuverture ;
  NVLdVTemps tpsEnd   = pPhase->tDateFermeture ;

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
  string sNodeRenew = pLdvDrug->pDoc->getDrugs(pLdvDrug->getIFrame())->getLastRenewNode(sNodeMedic) ;
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
    if (NULL != iterDoc)
      return (*iterDoc)->getDateDoc() ;
  }

  return string("") ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSDrugsPropertyWindow//
// -----------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(NSDrugsPropertyWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONDOWN,
  // EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSDrugsPropertyWindow::NSDrugsPropertyWindow(NSDrugView *parent, NSContexte* pCtx, int id, int x, int y, int w, int h, TModule *module)
                      :NSSkinableListWindow((TWindow *) parent, pCtx, id, x, y, w, h, module)
{
  _pView = parent ;
  _iRes  = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

  _pImages = (OWL::TImageList*) 0 ;
}
NSDrugsPropertyWindow::~NSDrugsPropertyWindow()
{
  if (_pImages)
		delete _pImages ;
}

void
NSDrugsPropertyWindow::SetupWindow()
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
NSDrugsPropertyWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  	_pView->CmChange() ;
  else
  	_pView->CmAdd() ;
}

void
NSDrugsPropertyWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    _pView->EvRButtonDown(modkeys, point) ;
  }
  else
  	_pView->EvRButtonDownOut(modkeys, point) ;
}

void
NSDrugsPropertyWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
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
}

void
NSDrugsPropertyWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
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
NSDrugsPropertyWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;  if (indexItem < 0)    return ;

  NSLdvDrug *pDrug = _pView->_aCurrentDrugs[indexItem] ;
  if ((NSLdvDrug*) NULL == pDrug)
    return ;

  bool bNewState = (false == pDrug->isToPrescribe()) ;

  _pView->setDrugPrescribeStatus(pDrug, bNewState) ;

  switchPrescribeImage(indexItem, bNewState) ;

  TListWindow::EvLButtonDown(modKeys, point) ;

  // if (_pView->isSortedOnPrescribe())
  //  _pView->sortByPrescribe() ;

  _pView->Invalidate() ;
}

void
NSDrugsPropertyWindow::switchPrescribeImage(int iItem, bool bToPrescribe)
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
NSDrugsPropertyWindow::IndexItemSelect()
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
NSDrugsPropertyWindow::EvSetFocus(HWND hWndLostFocus)
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
NSDrugsPropertyWindow::EvKillFocus(HWND hWndGetFocus)
{
  // Invalidate() ;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
/*
DrugTemplate::DrugTemplate(string sConcernNode)
  : DrugViewTmpl(const char far *filt, const char far *desc, const char far *dir, const char far *ext, long flags = 0, TModule*& module = ::Module, TDocTemplate*& phead = DocTemplateStaticHead)
{
  pLdVDoc = &doc ;
*/

