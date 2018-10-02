// -----------------------------------------------------------------------------// nsrcview.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.13 $
// $Author: pameline $
// $Date: 2015/02/07 11:34:44 $
// -----------------------------------------------------------------------------
// Vue Document/Vues de gestion du tableau de bord des RC
// Doc/View View for RC management
// -----------------------------------------------------------------------------
// PA  - octoble 2003
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
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsrcdlg.h"
#include "nsbb\nsmview.h"
#include "nsbb\nspanesplitter.h"
#include "nautilus\nsrcview.h"
#include "nautilus\nsldvvue.rh"

#include "nautilus\nscsdoc.h"
#include "nautilus\nscsvue.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nshistdo.h"
#include "nssavoir\nsgraphe.h"

// --------------------------------------------------------------------------
//
// Class NSRCEpisod
//
// --------------------------------------------------------------------------

NSRCEpisod::NSRCEpisod()
{
    pConcern    = NULL ;
    sPosDiag    = "" ;
    sLastDoc    = "" ;
}

NSRCEpisod::NSRCEpisod(const NSRCEpisod& rv)
{
    pConcern    = rv.pConcern ;
    sPosDiag    = rv.sPosDiag ;
    sLastDoc    = rv.sLastDoc ;
}

NSRCEpisod::~NSRCEpisod()
{
    // Ne pas faire/Don't do delete pConcern
}

NSRCEpisod&
NSRCEpisod::operator=(const NSRCEpisod& src)
{
  if (&src == this)
		return *this ;

  pConcern    = src.pConcern ;
  sPosDiag    = src.sPosDiag ;
  sLastDoc    = src.sLastDoc ;

  return *this ;
}

// --------------------------------------------------------------------------
//
// Class ArrayRCEpisodes
//
// --------------------------------------------------------------------------

ArrayRCEpisodes::ArrayRCEpisodes(NSEpisodRCView* pRCView)
                :ArrayRCEpiso()
{
  pRCEpiView = pRCView ;
}

ArrayRCEpisodes::ArrayRCEpisodes(const ArrayRCEpisodes& rv)
                :ArrayRCEpiso()
{
try
{
  pRCEpiView = rv.pRCEpiView ;

	if (false == rv.empty())
		for (ArrayRCEpisodConstIter i = rv.begin() ; rv.end() != i ; i++)
    	    push_back(new NSRCEpisod(*(*i)));
}
catch (const exception& e)
{
	string sExept = "Exception ArrayRCEpisodes copy ctor " + string(e.what());
	erreur(sExept.c_str(),  standardError, 0) ;
}
catch (...)
{
  erreur("Exception ArrayRCEpisodes.",  standardError, 0) ;
}
}

void
ArrayRCEpisodes::vider()
{
	if (empty())
		return;

	for (ArrayRCEpisodIter i = begin(); i != end(); )
	{
		delete *i;
		erase(i);
	}
}

void
ArrayRCEpisodes::initialiser(bool bActive, string sMinDate)
{
    if ((!pRCEpiView) || (!(pRCEpiView->getDoc())))
        return ;
    ArrayConcern* pConcerns = pRCEpiView->getDoc()->getConcerns(ldvframeHealth) ;
    if ((!pConcerns) || (pConcerns->empty()))
        return ;

    NVLdVTemps  tDateNow ;
    NVLdVTemps  tDateMin ;

    tDateNow.takeTime() ;
    if ((!bActive) && (sMinDate != ""))
        tDateMin.initFromDate(sMinDate) ;
    else
        tDateMin.takeTime() ;

    ArrayConcernIter itConcerns = pConcerns->begin() ;
    for ( ; itConcerns != pConcerns->end() ; itConcerns++)
    {
        if (((*itConcerns)->_tDateFermeture.estNoLimit()) ||
            ((*itConcerns)->_tDateFermeture >= tDateMin))
        {
            NSRCEpisod* pRCEpisod = new NSRCEpisod ;
            pRCEpisod->pConcern = (*itConcerns) ;
            push_back(pRCEpisod) ;
        }
    }
}

ArrayRCEpisodes::~ArrayRCEpisodes()
{
	vider();
}

ArrayRCEpisodes&
ArrayRCEpisodes::operator=(const ArrayRCEpisodes& src)
{
try
{
  if (&src == this)
		return *this ;

  pRCEpiView = src.pRCEpiView ;

	vider() ;

	if (false == src.empty())
		for (ArrayRCEpisodConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSRCEpisod(*(*i))) ;

	return *this ;
}
catch (const exception& e)
{
	string sExept = "Exception ArrayRCEpisodes= " + string(e.what());
	erreur(sExept.c_str(),  standardError, 0) ;
	return *this;
}
catch (...)
{
	erreur("Exception ArrayRCEpisodes= .",  standardError, 0) ;
	return *this;
}
}

bool rcSortByNameInf(NSRCEpisod* p1, NSRCEpisod* p2)
{
	return (p1->pConcern->_sTitre < p2->pConcern->_sTitre) ;
}

bool rcSortByNameSup(NSRCEpisod* p1, NSRCEpisod* p2)
{
	return (p1->pConcern->_sTitre > p2->pConcern->_sTitre) ;
}

bool rcSortByBeginInf(NSRCEpisod* p1, NSRCEpisod* p2)
{
	return (p1->pConcern->_tDateOuverture < p2->pConcern->_tDateOuverture) ;
}

bool rcSortByBeginSup(NSRCEpisod* p1, NSRCEpisod* p2)
{
	return (p1->pConcern->_tDateOuverture > p2->pConcern->_tDateOuverture) ;
}

bool rcSortByEndInf(NSRCEpisod* p1, NSRCEpisod* p2)
{
	return (p1->pConcern->_tDateFermeture < p2->pConcern->_tDateFermeture) ;
}

bool rcSortByEndSup(NSRCEpisod* p1, NSRCEpisod* p2)
{
	return (p1->pConcern->_tDateFermeture > p2->pConcern->_tDateFermeture) ;
}

// --------------------------------------------------------------------------
//
// Class NSEpisodRCView
//
// --------------------------------------------------------------------------

const int ID_EpiRCList = 0x100;
// Table de réponses de la classe NSEpisodView

DEFINE_RESPONSE_TABLE1(NSEpisodRCView, NSLDVView)
	EV_VN_ISWINDOW,
	EV_LVN_GETDISPINFO(ID_EpiRCList, DispInfoListe),
	EV_LVN_COLUMNCLICK(ID_EpiRCList, LVNColumnclick),
	EV_WM_SIZE,
	EV_WM_SETFOCUS,
	EV_COMMAND(CM_EPISO_NEW,        CmNouveau),
	EV_COMMAND(CM_EPISO_EVOLUER,    CmEvolve),
END_RESPONSE_TABLE;

// Constructeur

NSEpisodRCView::NSEpisodRCView(NSLdvDocument &doc, string sConcern)               :NSLDVView(doc.pContexte, &doc, 0, string("EpiRCManagement"), sConcern)
{
try
{
	pLdVDoc         = &doc;
	pEpisods        = new ArrayRCEpisodes(this) ;
	pListeWindow    = new NSRCListWindow(this, ID_EpiRCList, 0, 0, 0, 0);

	//SetViewMenu(new TMenuDescr(IDM_DRUG_VIEW));
	pToolBar        = 0 ;
	bSetupToolBar   = true ;

	iSortedColumn   = -1 ;

	// Initialisation des épisodes actifs
	initCurentEpiRC() ;

	setViewName() ;
}
catch (...)
{
	erreur("Exception NSEpisodRCView ctor.",  standardError, 0) ;
}
}

// Destructeur
NSEpisodRCView::~NSEpisodRCView()
{
}

void
NSEpisodRCView::setViewName()
{
	sViewName = pContexte->getSuperviseur()->getText("RCManagement", "RCEpisodViewTitle") ;

  addConcernTitle() ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow*
NSEpisodRCView::GetWindow()
{
	return (TWindow*) this;
}

// Appel de la fonction de remplissage de la vuevoid
NSEpisodRCView::SetupWindow()
{
	NSMUEView::SetupWindow();

	Parent->SetCaption(sViewName.c_str()) ;

	SetupColumns();
	AfficheListe();
}

void
NSEpisodRCView::CmNouveau()
{
try
{
	NSPatPathoArray *pPPT = new NSPatPathoArray(pContexte->getSuperviseur()) ;

#ifdef __OB1__
	NSSmallBrother BigBoss(pContexte, pPPT, 0) ;
#else
	NSSmallBrother BigBoss(pContexte, pPPT, 0, false) ;
#endif
	BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;

	// pBigBoss->lance12("GPRSC1", sChoixMed) ;

  string sNoeud1 = string("ZPOMR1") ;

	BigBoss._pBBItem = new BBItem(pContexte, &BigBoss) ;

	BBItemData* pDo = new BBItemData() ;
  strcpy(pDo->ouvreDialogue, "N") ;
  strcpy(pDo->fils, "") ;
  strcpy(pDo->decalageNiveau, "+01+01") ;
  strcpy(pDo->fichierDialogue, "NSBB") ;

	BigBoss.getBBItem()->setItemData(pDo) ;
	BigBoss.getBBItem()->setDialogName(string("")) ;

  string sCodeSens ;
  NSDico::donneCodeSens(&sNoeud1, &sCodeSens) ;
  BigBoss.getBBItem()->_sLocalisation = sCodeSens ;

  BigBoss.getBBItem()->setDialog(new NSRCDialog(pContexte, BigBoss.getBBItem(), this)) ;
  int iResult = BigBoss.getBBItem()->getDialog()->Execute() ;

  NSRCDialog* pRCDialog = static_cast<NSRCDialog*>(BigBoss.getBBItem()->getDialog());
  string sChosenRC        = pRCDialog->sCurrentLex ;
  string sChosenArchetype = pRCDialog->sCurrentArchetype ;

  string sLocalis = BigBoss.getBBItem()->_sLocalisation ;
  BigBoss.getBBItem()->okFermerDialogue(true) ;
  // *pPatPathoArray = *(pBigBoss->pBBItem->pPPTEnCours) ;

  delete BigBoss.getBBItem() ;

  if ((sChosenRC == "") || (iResult != IDOK))
  {
  	delete pPPT ;
    return ;
  }

  NSPatPathoArray *pTotalPPT = new NSPatPathoArray(pContexte->getSuperviseur()) ;
  pTotalPPT->ajoutePatho(sChosenRC, 0);

    if (!(pPPT->empty()))
        pTotalPPT->InserePatPatho(pTotalPPT->end(), pPPT, 1) ;
    delete pPPT ;

    NSCsVue* pCSView = 0 ;

    NSPatientChoisi* pPat = pContexte->getPatient() ;
    //
    // S'il n'y a pas de document consultation ouvert, on en ouvre un
    //
    if (NULL == pPat->_pConsultEnCours)
    {
        NSCSDocument* pNewDocCS = new NSCSDocument(0, pContexte, "GCONS1") ;
        pCSView = new NSCsVue(*pNewDocCS, 0) ;

        NSDocViewManager dvManager(pContexte) ;
        dvManager.createView(pNewDocCS, "CS Format", pCSView) ;

        pPat->_pConsultEnCours = pNewDocCS ;
    }
    //
    // Sinon, on cherche si le document consultation en cours a une vue ouverte
    //
    else
    {
        TView* pView = pPat->_pConsultEnCours->GetViewList() ;
        do
        {
            NSCsVue* pisCsView = TYPESAFE_DOWNCAST(pView, NSCsVue) ;
            if (pisCsView)
                pCSView = pisCsView ;

            pView = pPat->_pConsultEnCours->NextView(pView) ;
        }
        while (pView && !pCSView) ;
    }

    if (pCSView)
        pCSView->NewPreoccupRC(pTotalPPT, sChosenArchetype) ;

    delete pTotalPPT ;
}
catch (...)
{
    erreur("Exception NSEpisodRCView::CmNouveau.", standardError, 0) ;
}
}

void
NSEpisodRCView::CmEvolve()
{
try
{
  // Récupération de l'élément sélectionné
  //
  int index = pListeWindow->IndexItemSelect() ;
  if (index < 0)
  {
    erreur("Vous devez sélectionner un RC.", standardError, 0) ;
    return ;
  }

  if (pEpisods->empty())
    return ;
  NSRCEpisod* pSelectedEpisod = (*pEpisods)[index] ;

  NSPatientChoisi*    pPat    = pContexte->getPatient() ;
  //
  // Prise du RC
  //
  NSPatPathoArray* pPatho ;
  PatPathoIter iter = pLdVDoc->donnePreoccup(pSelectedEpisod->pConcern, &pPatho) ;
  if (NULL == iter)
  {
    erreur("Impossible de retrouver ce RC dans l'Index de santé.", standardError, 0) ;
    return ;
  }
  string sPreviousConcernLex = (*iter)->getLexique() ;
  string sPreviousConcernSens ;
  NSDico::donneCodeSens(&sPreviousConcernLex, &sPreviousConcernSens) ;

  //
  // Récupération de la dernière description
  //
  VecteurString vResult;

  string sSelectedPreoccup = pSelectedEpisod->pConcern->getNoeud() ;

#ifndef N_TIERS
  NSGraphe* pGraphe = new NSGraphe(pContexte) ;
#else
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
#endif
  pGraphe->TousLesVrais(sSelectedPreoccup, NSRootLink::problemContactElement, &vResult, "ENVERS");
#ifndef N_TIERS
  delete pGraphe;
#endif

  if (vResult.empty())
    return ;

  NSPatPathoArray PreviousPPt(pContexte->getSuperviseur()) ;

  string sMaxDateDoc = string("") ;

  EquiItemIter iterLiens = vResult.begin();
  for (; iterLiens != vResult.end(); iterLiens++)
  {
    string sDateDoc = string("") ;

    //
    // On récupère l'arbre
    //
    string sDocId = string(**iterLiens, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN);

    NSPatPathoArray PPt(pContexte->getSuperviseur()) ;

    if ((pPat->getDocHis()->donneArbre(sDocId, &PPt, sDateDoc)) &&
        (false == PPt.empty()))
    {
      PatPathoIter pptIt = PPt.begin() ;
      string sRootLex = (*pptIt)->getLexique() ;
      string sRootLexSens ;
      NSDico::donneCodeSens(&sRootLex, &sRootLexSens) ;
      //
      // On ne s'intéresse qu'aux consultations
      //
      if ((sRootLexSens == "GCONS") && (sDateDoc > sMaxDateDoc))
      {
        pptIt = PPt.ChercherNoeud(**iterLiens) ;
        if ((pptIt != NULL) && (pptIt != PPt.end()))
        {
          string sNodeLex = (*pptIt)->getLexique() ;
          string sNodeSens ;
          NSDico::donneCodeSens(&sNodeLex, &sNodeSens) ;

          if (sNodeSens == sPreviousConcernSens)
          {
            sMaxDateDoc = sDateDoc ;

            PreviousPPt.vider() ;

            int colonneElement  = (*pptIt)->getColonne() ;
            int ligneElement    = (*pptIt)->getLigne();

            PreviousPPt.ajoutePatho(pptIt, 0, 0) ;
            pptIt++ ;

            while ((pptIt != PPt.end()) && ((*pptIt)->getColonne() > colonneElement))
            {
              int colonne  = (*pptIt)->getColonne() - colonneElement ;
              int ligne    = (*pptIt)->getLigne() - ligneElement ;
              PreviousPPt.ajoutePatho(pptIt, ligne, colonne) ;
              pptIt++ ;
            }
          }
        }
      }
    }
  }

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;

  if (false == PreviousPPt.empty())
    PreviousPPt.ExtrairePatPatho(PreviousPPt.begin(), &PPT) ;

  string sChosenRC = sPreviousConcernLex ;

#ifdef __OB1__
	NSSmallBrother BigBoss(pContexte, &PPT, 0) ;
#else
	NSSmallBrother BigBoss(pContexte, &PPT, 0, false) ;
#endif
  BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
  BigBoss._pBBItem = new BBItem(pContexte, &BigBoss) ;

  BBItemData* pDo = new BBItemData() ;
  strcpy(pDo->ouvreDialogue, "N") ;
  strcpy(pDo->fils, "") ;
  strcpy(pDo->decalageNiveau, "+01+01") ;
	strcpy(pDo->fichierDialogue, "NSBB") ;

	BigBoss.getBBItem()->setItemData(pDo) ;
  BigBoss.getBBItem()->setDialogName(string("")) ;

  string sNoeud1 = string("ZPOMR1") ;

  string sCodeSens ;
  NSDico::donneCodeSens(&sNoeud1, &sCodeSens) ;
  BigBoss.getBBItem()->_sLocalisation = sCodeSens ;

  BigBoss.getBBItem()->setDialog(new NSRCDialog(pContexte, BigBoss.getBBItem(), this)) ;
  NSRCDialog* pRCDialog = static_cast<NSRCDialog*>(BigBoss.getBBItem()->getDialog()) ;
  pRCDialog->sCurrentLex = sPreviousConcernLex ;

  int iResult = BigBoss.getBBItem()->getDialog()->Execute() ;

  sChosenRC = pRCDialog->sCurrentLex ;
  string sChosenArchetype = pRCDialog->sCurrentArchetype ;

  string sLocalis = BigBoss.getBBItem()->_sLocalisation ;
  BigBoss.getBBItem()->okFermerDialogue(true) ;
  // *pPatPathoArray = *(pBigBoss->pBBItem->pPPTEnCours) ;

  delete BigBoss.getBBItem() ;

  if ((sChosenRC == "") || (iResult != IDOK))
    return ;

  NSPatPathoArray TotalPPT(pContexte->getSuperviseur()) ;
  TotalPPT.ajoutePatho(sChosenRC, 0);

  if (false == PPT.empty())
    TotalPPT.InserePatPatho(TotalPPT.end(), &PPT, 1) ;

  NSCsVue* pCSView = (NSCsVue*) 0 ;
  //
  // S'il n'y a pas de document consultation ouvert, on en ouvre un
  //
  if (NULL == pPat->_pConsultEnCours)
  {
    NSCSDocument* pNewDocCS = new NSCSDocument(0, pContexte, "GCONS1") ;
    pCSView = new NSCsVue(*pNewDocCS, 0) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pNewDocCS, "CS Format", pCSView) ;

    pPat->_pConsultEnCours = pNewDocCS ;
  }
  //
  // Sinon, on cherche si le document consultation en cours a une vue ouverte
  //
  else
  {
    TView* pView = pPat->_pConsultEnCours->GetViewList() ;
    do
    {
      NSCsVue* pisCsView = TYPESAFE_DOWNCAST(pView, NSCsVue) ;
      if (pisCsView)
        pCSView = pisCsView ;

      pView = pPat->_pConsultEnCours->NextView(pView) ;
    }
    while (pView && (NULL == pCSView)) ;
  }

  if (pCSView)
  {
    string sChosenConcernSens ;
    NSDico::donneCodeSens(&sChosenRC, &sChosenConcernSens) ;

    if (sChosenConcernSens == sPreviousConcernSens)
      pCSView->EvolPreoccupRC(&TotalPPT, pSelectedEpisod->pConcern, sChosenArchetype) ;
    else
      pCSView->ChangePreoccupRC(&TotalPPT, pSelectedEpisod->pConcern, sChosenArchetype) ;
  }
}
catch(TWindow::TXWindow& e)
{
    string sErr = string("Exception NSEpisodRCView::CmEvolve : ") + e.why() ;
    erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
    erreur("Exception NSEpisodRCView::CmEvolve.",  standardError, 0) ;
}
}

void
NSEpisodRCView::CmSelect()
{
    int index = pListeWindow->IndexItemSelect() ;

    if (index < 0)
        return ;

    if (pEpisods->empty())
        return ;

    NSRCEpisod* pSelectedEpisod = (*pEpisods)[index] ;
    getPaneSplitter()->concernChanged(pSelectedEpisod->pConcern->getNoeud()) ;
}

void
NSEpisodRCView::reloadView(string sReason)
{
    initCurentEpiRC() ;
    AfficheListe() ;
}

void
NSEpisodRCView::initCurentEpiRC()
{
    pEpisods->vider() ;
    pEpisods->initialiser(true, "") ;
}

// Initialisation des colonnes de la ListWindowvoid
NSEpisodRCView::SetupColumns()
{
    pListeWindow->InsertColumn(0, TListWindColumn("Dénomination", 450, TListWindColumn::Left, 0));
    pListeWindow->InsertColumn(1, TListWindColumn("PD", 30, TListWindColumn::Left, 1));
    pListeWindow->InsertColumn(2, TListWindColumn("D. début", 90, TListWindColumn::Left, 2));
    pListeWindow->InsertColumn(3, TListWindColumn("Probable fin", 90, TListWindColumn::Left, 3));
}

// Affichage des éléments de la listevoid
NSEpisodRCView::AfficheListe()
{
    pListeWindow->DeleteAllItems();

    if (pEpisods->empty())
        return ;

    // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
    ArrayRCEpisodIter itDg = pEpisods->end() ;
    do
    {
        itDg-- ;
        TListWindItem Item(((*itDg)->pConcern->_sTitre).c_str(), 0);
        pListeWindow->InsertItem(Item);
    }
    while (itDg != pEpisods->begin()) ;
}

void
NSEpisodRCView::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  const int   BufLen = 255;
  static char buffer[BufLen];
  string      sDate = string("") ;
  buffer[0] = '\0' ;

  TListWindItem   &dispInfoItem = *(TListWindItem*)&dispInfo.item;
  int index = dispInfoItem.GetIndex();

	string sLang = pContexte->getUserLanguage() ;

	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // Position diagnostique

    	dispInfoItem.SetText(((*pEpisods)[index])->sPosDiag.c_str()) ;
      break ;

    case 2: // date début

    	sDate = ((*pEpisods)[index])->pConcern->_tDateOuverture.donneDate() ;
      sDate = donne_date(sDate, sLang) ;
      dispInfoItem.SetText(sDate.c_str()) ;
      break ;

    case 3: // date fin
    	if (((*pEpisods)[index])->pConcern->_tDateFermeture.estNoLimit())
      	sDate = string("...") ;
      else
        sDate = donne_date(((*pEpisods)[index])->pConcern->_tDateFermeture.donneDate(), sLang) ;

      dispInfoItem.SetText(sDate.c_str()) ;
      break;
	}
}

void
NSEpisodRCView::LVNColumnclick(TLwNotify& lwn)
{
    switch ( lwn.iSubItem )
    {
        case 0 :
            sortByName();
            break;
        case 2 :
            sortByBegining();
            break;
        case 3 :
            sortByEnding();
            break;
    }
}

void
NSEpisodRCView::sortByName()
{
    if (iSortedColumn == 0)
    {
        if (bNaturallySorted)
            bNaturallySorted = false ;
        else
            bNaturallySorted = true ;
    }
    else
    {
        iSortedColumn = 0 ;
        bNaturallySorted = true ;
    }

    if (pEpisods->empty())
        return ;

    if (bNaturallySorted)
        sort(pEpisods->begin(), pEpisods->end(), rcSortByNameInf) ;
    else
        sort(pEpisods->begin(), pEpisods->end(), rcSortByNameSup) ;

    AfficheListe() ;
}

void
NSEpisodRCView::sortByEnding()
{
    if (iSortedColumn == 2)
    {
        if (bNaturallySorted)
            bNaturallySorted = false ;
        else
            bNaturallySorted = true ;
    }
    else
    {
        iSortedColumn = 2 ;
        bNaturallySorted = true ;
    }

    if (pEpisods->empty())
        return ;

    if (bNaturallySorted)
        sort(pEpisods->begin(), pEpisods->end(), rcSortByEndSup) ;
    else
        sort(pEpisods->begin(), pEpisods->end(), rcSortByEndInf) ;

    AfficheListe() ;
}

void
NSEpisodRCView::sortByBegining()
{
    if (iSortedColumn == 1)
    {
        if (bNaturallySorted)
            bNaturallySorted = false ;
        else
            bNaturallySorted = true ;
    }
    else
    {
        iSortedColumn = 1 ;
        bNaturallySorted = true ;
    }

    if (pEpisods->empty())
        return ;

    if (bNaturallySorted)
        sort(pEpisods->begin(), pEpisods->end(), rcSortByBeginSup) ;
    else
        sort(pEpisods->begin(), pEpisods->end(), rcSortByBeginInf) ;

    AfficheListe() ;
}

boolNSEpisodRCView::VnIsWindow(HWND hWnd){
  	return HWindow == hWnd;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindowvoid
NSEpisodRCView::EvSize(uint sizeType, ClassLib::TSize& size)
{
    TWindow::EvSize(sizeType, size);
    pListeWindow->MoveWindow(GetClientRect(), true);
}

// fonction EVSetFocusvoid
NSEpisodRCView::EvSetFocus(HWND hWndLostFocus)
{
	NSMUEView::EvSetFocus(hWndLostFocus) ;

	focusFct() ;

	pListeWindow->SetFocus() ;
}

void
NSEpisodRCView::focusFct()
{
	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;

	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}

	_pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;
	pContexte->setAideIndex("") ;
	pContexte->setAideCorps("rc_episode.htm") ;
}

// SetupToolBarvoid
NSEpisodRCView::SetupToolBar()
{
try
{
	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_EPISO_NEW,        CM_EPISO_NEW,       TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_EPISO_CLOSE,      CM_EPISO_CLOSE,     TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new TButtonGadget(CM_EPISO_EVOLUER,    CM_EPISO_EVOLUER,   TButtonGadget::Command)) ;

	pMyApp->getSecondaryControlBar()->LayoutSession() ;
}
catch (...)
{
	erreur("Exception NSEpisodRCView::SetupToolBar.",  standardError, 0) ;
}
}

//***************************************************************************
//
//  					Méthodes de NSDrugsPropertyWindow//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSRCListWindow, TListWindow)   EV_WM_LBUTTONDBLCLK,
   EV_WM_KEYDOWN,
   EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

NSRCListWindow::NSRCListWindow(NSEpisodRCView* parent, int id, int x, int y, int w, int h, TModule* module)
               :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
    pView   = parent;
    iRes    = id;
    Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS;
    // Attr.ExStyle |= WS_EX_NOPARENTNOTIFY;
}
void
NSRCListWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

	TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSRCListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TLwHitTestInfo info(point);

    HitTest(info);
    if (info.GetFlags() & LVHT_ONITEM)        pView->CmSelect() ;
}

void
NSRCListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
    //if      (key == VK_DELETE)
    //    CmSupprCorr() ;
    if (key == VK_INSERT)
    {
        if (GetKeyState(VK_SHIFT) < 0)
            pView->CmEvolve() ;
        else
            pView->CmNouveau() ;
    }
    else
    	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

//---------------------------------------------------------------------------//  Retourne l'index du premier item sélectionné
//---------------------------------------------------------------------------
int
NSRCListWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;}

void
NSRCListWindow::EvSetFocus(HWND hWndLostFocus)
{
	pView->focusFct() ;

	int count = GetItemCount() ;

	for (int i = 0; i < count; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------

/*
DrugTemplate::DrugTemplate(string sConcernNode)
             :DrugViewTmpl(const char far* filt, const char far* desc, const char far* dir, const char far* ext, long flags = 0, TModule*& module = ::Module, TDocTemplate*& phead = DocTemplateStaticHead)
{
    pLdVDoc = &doc;
*/

// --------------------------------------------------------------------------
//
// Class NSRCHistoryView
//
// --------------------------------------------------------------------------

// Table de réponses de la classe NSRCHistoryView
DEFINE_RESPONSE_TABLE1(NSRCHistoryView, NSLDVView)
	EV_WM_VSCROLL,
	EV_WM_HSCROLL,
	EV_MESSAGE(WM_MOUSEWHEEL, EvMouseWheel),
	EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

// Constructeur

NSRCHistoryView::NSRCHistoryView(NSLdvDocument &doc, string sConcern)               :NSLDVView(doc.pContexte, &doc, 0, string("RcItemHistory"), sConcern)
{
	//
	// Initialise le style avec des ascenseurs horizontaux et verticaux
	//
	Attr.Style = Attr.Style | WS_HSCROLL | WS_VSCROLL;

	sClassifSens    = "6MGIT" ;

	pLdVDoc         = &doc ;
	pToolBar        = 0 ;
	bSetupToolBar   = true ;

	initParams() ;

  setViewName() ;
}

void
NSRCHistoryView::setViewName()
{
	sViewName = pContexte->getSuperviseur()->getText("RCManagement", "RCHistoryViewTitle") ;

  addConcernTitle() ;
}

void
NSRCHistoryView::concernChanged(string sConcern)
{
    sPreoccup = sConcern ;

    reInitParams() ;
}

void
NSRCHistoryView::initParams()
{
  sArchetype      = "" ;
  pParseur        = (nsarcParseur*) 0 ;

  sConcernText    = "" ;

  PremLigne       = 1 ;
  HautGcheFenetre.x = HautGcheFenetre.y = 0 ;
  LargeurPolice   = 0 ;

  initArchetype() ;
}

void
NSRCHistoryView::reInitParams()
{
    sArchetype      = "" ;
    sConcernText    = "" ;

    PremLigne       = 1 ;
    HautGcheFenetre.x = HautGcheFenetre.y = 0 ;
    LargeurPolice   = 0 ;

    if (!(Lignes.empty()))
        Lignes.vider() ;
    if (!(StylesPolice.empty()))
        StylesPolice.vider() ;
    if (!(StylesParagraphe.empty()))
        StylesParagraphe.vider() ;

    if (pParseur)
    {
        delete pParseur ;
        pParseur = (nsarcParseur*) 0 ;
    }

    initArchetype() ;

    initLines() ;
    if (Lignes.empty())
        return ;

    initHistory() ;
    InitialiseLignes() ;

    Invalidate() ;
}

void
NSRCHistoryView::initArchetype()
{
try
{
    //
    // Recherche de l'Archetype
    //
    if (sPreoccup == "")
        return ;

    // Localisation : ZPOMR / sConcern
    string sNoeud1 = string("ZPOMR1") ;
    string sCodeSens ;
    NSSuper* pSuper = pContexte->getSuperviseur() ;
    NSDico::donneCodeSens(&sNoeud1, &sCodeSens) ;
    string sLocalisation = sCodeSens ;

    //recherche du fil guide

    NSConcern* pConcern = pLdVDoc->getConcerns(ldvframeHealth)->getConcern(sPreoccup);
    if (!pConcern)
        return ;

    sConcernText = pConcern->_sTitre ;

    NSPatPathoArray* pPatho ;
    PatPathoIter iter = pLdVDoc->donnePreoccup(pConcern, &pPatho) ;
    if (iter == NULL)
        return ;

    string sConcernLex = (*iter)->getLexique() ;

    VecteurRechercheSelonCritere* pVecteurSelonCritere = new VecteurRechercheSelonCritere(GUIDE) ;
    NSDico::donneCodeSens(&sConcernLex, &sCodeSens) ;
    pVecteurSelonCritere->AjouteEtiquette(sCodeSens) ;
    pSuper->getFilGuide()->chercheChemin(&sLocalisation ,
                             pVecteurSelonCritere, NSFilGuide :: compReseau) ;
    bool        trouve ;
    BBItemData* pDonnees = new BBItemData ;
    pVecteurSelonCritere->SetData(sCodeSens, &trouve, pDonnees) ;
    string sEtiquette ;
    if (trouve)
    {
        if (pDonnees->ouvreDialogue[0] == 'A')
            sArchetype = string(pDonnees->fils) ;
    }
    delete pDonnees ;
    delete pVecteurSelonCritere ;

    if (sArchetype == "")
        return ;

    // Ouverture du fichier, et parsing
    string sArchetypeFile = pContexte->getSuperviseur()->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::archetype, sArchetype) ;
    if (sArchetypeFile == "")
        return ;

    pParseur = new nsarcParseur(pContexte->getSuperviseur()) ;
    if (!(pParseur->open(sArchetypeFile)))
    {
        delete pParseur ;
        pParseur = (nsarcParseur*) 0 ;
        return ;
    }
}
catch (...)
{
    erreur("Exception NSRCHistoryView::initArchetype.",  standardError, 0) ;
}
}

// Destructeur
NSRCHistoryView::~NSRCHistoryView()
{
    if (pParseur)
        delete pParseur ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow*
NSRCHistoryView::GetWindow()
{
    return (TWindow*) this;
}

// Appel de la fonction de remplissage de la vuevoid
NSRCHistoryView::SetupWindow()
{
	NSMUEView::SetupWindow();

	Parent->SetCaption(sViewName.c_str()) ;

	if ((nsarcParseur*) NULL == pParseur)
		return ;

	initLines() ;
	if (Lignes.empty())
		return ;

	initHistory() ;
	InitialiseLignes() ;
}

// fonction EVSetFocus
void
NSRCHistoryView::EvSetFocus(HWND hWndLostFocus)
{
  NSMUEView::EvSetFocus(hWndLostFocus) ;

  _pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;
  // pContexte->setAideIndex("") ;
  // pContexte->setAideCorps("objectifs.htm") ;
}

void
NSRCHistoryView::initLines()
{
try
{
  if (((nsarcParseur*) NULL == pParseur) || (NULL == pParseur->getArchetype()))
    return ;

  Citem* pItems = pParseur->getArchetype()->getRootItem() ;
  if ((Citem*) NULL == pItems)
    return ;

  if (pItems->vect_val.empty())
    return ;

  NSEpiClassif Classif(pContexte->getSuperviseur()) ;

  Classif.lastError = Classif.open() ;
  if (DBIERR_NONE != Classif.lastError)
  {
    erreur("Erreur à l'ouverture de Classif.", standardError, Classif.lastError) ;
    return ;
  }

  for (ValIter ival = pItems->vect_val.begin() ; pItems->vect_val.end() != ival ; ival++)
  {
    if (LABEL_ITEM == (*ival)->getLabel()) // item
    {
      Citem* pItem = dynamic_cast<Citem*>((*ival)->getObject()) ;
      if (pItem)
        initLines(pItem, &Classif) ;
    }
  }

  Classif.lastError = Classif.close() ;
  if (DBIERR_NONE != Classif.lastError)
  {
    erreur("Erreur à la fermeture de classer.", standardError, Classif.lastError) ;
    return ;
  }
}
catch (...)
{
  erreur("Exception NSRCHistoryView::initLines 1", standardError, 0) ;
}
}

void
NSRCHistoryView::initLines(Citem* pItem, NSEpiClassif* pClassif)
{
try
{
  if (NULL == pItem)
    return ;

  //
  // Prise en compte de l'Item en cours
  //
  string sCode = pItem->getCode() ;
  size_t iSepare = sCode.find(cheminSeparationMARK) ;
  if (iSepare != NPOS)
  {
        string sLexi = string(sCode, 0, iSepare) ;
        sCode = string(sCode, iSepare + 1, strlen(sCode.c_str()) - iSepare - 1) ;
        if (sCode[0] == '$')
            sCode = string(sCode, 1, strlen(sCode.c_str()) - 1) ;

        string sLexSens;
        pContexte->getDico()->donneCodeSens(&sLexi, &sLexSens);

        if (sLexSens == sClassifSens)
        {
            NSItemHistoriser* pItemHisto = new NSItemHistoriser ;

            pItemHisto->sLexique     = sLexi ;
            pItemHisto->sComplement  = sCode ;

            string sClasserKey = sLexSens + pItemHisto->sComplement ;
            pClassif->lastError = pClassif->chercheClef(&sClasserKey,
                                                        "",
                                            		    0,
                                                        keySEARCHEQ,
                                            		    dbiWRITELOCK) ;
            if (pClassif->lastError != DBIERR_NONE)
            {
                string sErrMess =   string("Impossible de trouver le code ") +
                                    pItemHisto->sComplement +
                                    string(" dans la base des classifications.") ;
                erreur(sErrMess.c_str(), standardError, pClassif->lastError) ;
            }
            else
            {
                pClassif->lastError = pClassif->getRecord() ;
		        if (pClassif->lastError != DBIERR_NONE)
		        {
			        erreur("Erreur de lecture dans la base Classif.", standardError, pClassif->lastError) ;
			        return ;
		        }
                pItemHisto->pTextLine = new NSLigne ;
                pItemHisto->pTextLine->Texte = pClassif->getLabel() ;
            }

            Lignes.push_back(pItemHisto) ;
        }
    }

    //
    // Prise en compte de ses sous-items
    //
    if (pItem->vect_val.empty())
        return ;

    for (ValIter ival = pItem->vect_val.begin() ; pItem->vect_val.end() != ival ; ival++)
    {
        if ((*ival)->getLabel() == LABEL_ITEM) // item
        {
            Citem* pItemFils = dynamic_cast<Citem*>((*ival)->getObject()) ;
            if (pItemFils)
                initLines(pItemFils, pClassif) ;
        }
    }
}
catch (...)
{
    erreur("Exception NSRCHistoryView::initLines 2",  standardError, 0) ;
}
}

void
NSRCHistoryView::initHistory()
{
try
{
  if (sPreoccup == "")
    return ;
  if (Lignes.empty())
    return ;

  NSPatientChoisi* pPat = pContexte->getPatient();
  if ((!pPat) || (!(pPat->getDocHis())))
    return ;

  //
  // On cherche tous les noeuds en relation "problemContactElement" avec la préoccupation
  // We look for every node linked to this concern with the "problemContactElement" relationship
  //
  VecteurString vResult;

#ifndef N_TIERS
  NSGraphe* pGraphe = new NSGraphe(pContexte) ;
#else
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
#endif
  pGraphe->TousLesVrais(sPreoccup, NSRootLink::problemContactElement, &vResult, "ENVERS");
#ifndef N_TIERS
  delete pGraphe;
#endif

  if (vResult.empty())
    return ;

  string sMaxDateDoc = "";

  EquiItemIter iterLiens = vResult.begin();
  for (; iterLiens != vResult.end(); iterLiens++)
  {
    string sDateDoc = string("") ;

    //
    // On récupère l'arbre
    //
    string sDocId = string(**iterLiens, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN);

    NSPatPathoArray PPt(pContexte->getSuperviseur()) ;

    if (pPat->getDocHis()->donneArbre(sDocId, &PPt, sDateDoc))
    {
      if (sDateDoc >= sMaxDateDoc)
        sMaxDateDoc = sDateDoc ;

      PatPathoIter pptIt = PPt.ChercherNoeud(**iterLiens);

            if ((pptIt != NULL) && (pptIt != PPt.end()))
            {
                // On est sur le RC
                //
                int iColonne = (*pptIt)->getColonne() ;

                pptIt++ ;
                //
                // On passe en revue les items
                //
                while ((pptIt != PPt.end()) && ((*pptIt)->getColonne() > iColonne))
                {
                    string sLexSens ;
                    string sLexique = (*pptIt)->getLexique() ;
                    pContexte->getDico()->donneCodeSens(&sLexique, &sLexSens) ;

                    if (sLexSens == sClassifSens)
                    {
                        string sCode = (*pptIt)->getComplement() ;
                        for (ItemHistoIter it = Lignes.begin(); it != Lignes.end(); it++)
                        {
                            if (sCode == (*it)->sComplement)
                            {
                                NVLdVTemps* pCsTime = new NVLdVTemps ;
                                pCsTime->initFromDate(sDateDoc) ;
                                (*it)->aDates.push_back(pCsTime) ;
                            }
                        }
                    }
                    pptIt++ ;
                }
            }
        }
    }

    NVLdVTemps maxTime ;
    maxTime.initFromDate(sMaxDateDoc) ;

    for (ItemHistoIter it = Lignes.begin(); it != Lignes.end(); it++)
    {
        if (!((*it)->aDates.empty()))
        {
            (*it)->aDates.sortDates(true) ;
            if (**((*it)->aDates.begin()) == maxTime)
                (*it)->bInLastConsult = true ;
        }
    }
}
catch (...)
{
    erreur("Exception NSRCHistoryView::initHistory.",  standardError, 0) ;
}
}

void
NSRCHistoryView::InitialiseLignes()
{
try
{
    if (Lignes.empty())
        return ;

    RectangleGlobal.left    = 0 ;
	RectangleGlobal.top     = 0 ;
    RectangleGlobal.right   = 0 ;
    RectangleGlobal.bottom  = 0 ;

	//
	// Mise en place des polices dans le tableau - Storing fonts in their array
	//
    // Rappel : NSFont(facename, height, width, escapement, orientation, weight,
    //                 pitchAndFamily, italic, underline, strikeout, charSet,
    //                 outputPrecision, clipPrecision, quality)
    //
    // Police 0 : Eléments non utilisés - Font 0 : not used elements
	StylesPolice.push_back(new NSFont("Helvetica",14,0,0,0,0,0,0,0,0,1));
    // Police 1 : Eléments utilisés - Font 0 : used elements
	StylesPolice.push_back(new NSFont("Helvetica",14,0,0,0,0,0,0,0,0,1));
    // Police 2 : Eléments en cours - Font 0 : current elements
	StylesPolice.push_back(new NSFont("Helvetica",14,0,0,0,0,700,0,0,0,1));

    //
	// On récupère un "contexte d'information" concernant l'écran
	//
	TIC* pDC = new TIC("DISPLAY", NULL, NULL);
	pDC->SetMapMode(MM_LOMETRIC);

   	NS_CLASSLIB::TRect RectangleTexte;
   	RectangleTexte.Set(RectangleGlobal.Left() + 5,
                      RectangleGlobal.Top() - 5,
                      RectangleGlobal.Right(),
                      RectangleGlobal.Bottom()) ;
	//
	// On initialise le point de référence en haut et à gauche
	//
    NS_CLASSLIB::TPoint PointRef ;
	PointRef.x = RectangleTexte.Left() ;
   	PointRef.y = RectangleTexte.Top() ;

    // TFont* pPolice ;

    for (ItemHistoIter it = Lignes.begin(); it != Lignes.end(); it++)
    {
        NSFont PoliceVoulue ;

        // Item non utilisé
        if      ((*it)->aDates.empty())
        {
            PoliceVoulue = *(StylesPolice[0]) ;
            (*it)->pTextLine->CouleurTexte = TColor::LtGray ;
        }
        // Item en cours
        //
        else if ((*it)->bInLastConsult)
        {
            PoliceVoulue = *(StylesPolice[2]) ;
            (*it)->pTextLine->CouleurTexte = TColor::LtRed ;
        }
        // Item qui a été utilisé mais n'est pas en cours
        //
        else
        {
            PoliceVoulue = *(StylesPolice[1]) ;
            (*it)->pTextLine->CouleurTexte = TColor::Black ;
        }

        if (PoliceVoulue.logFont.lfHeight >= 0)
        {
            NS_CLASSLIB::TPoint	PointTransfert ;
            PointTransfert.x = PoliceVoulue.logFont.lfHeight ;
            PointTransfert.y = PoliceVoulue.logFont.lfWidth ;
            pDC->DPtoLP(&PointTransfert) ;
            PoliceVoulue.logFont.lfHeight = PointTransfert.x ;
            PoliceVoulue.logFont.lfWidth  = PointTransfert.y ;
        }

        TFont* pPolice = new TFont(&(PoliceVoulue.logFont)) ;
        pDC->SelectObject(*pPolice) ;
        TEXTMETRIC  TextMetric ;
        LONG        lPolicetmHeight = 0 ;
        if (pDC->GetTextMetrics(TextMetric))
        {
            if ((LargeurPolice == 0) || (LargeurPolice > TextMetric.tmAveCharWidth))
                LargeurPolice = TextMetric.tmAveCharWidth ;
            lPolicetmHeight = TextMetric.tmHeight + TextMetric.tmDescent ;
        }
        //
        // On prévoit l'espace au dessus du paragraphe
        //
        // PointRef.Offset(0, -StylePara.EspaceDessus);
        //
        // Initialisation de tous les paramètres déjà connus
        //
        (*it)->pTextLine->Boite.left    = RectangleTexte.Left() ;
        (*it)->pTextLine->Boite.top	    = PointRef.y ;
        (*it)->pTextLine->Boite.bottom  = PointRef.y ;

        string sTexte = (*it)->pTextLine->Texte ;
        NS_CLASSLIB::TSize TailleChaine = pDC->GetTextExtent(sTexte.c_str(), strlen(sTexte.c_str())) ;
        (*it)->pTextLine->Boite.right = (*it)->pTextLine->Boite.left + TailleChaine.cx ;

        LONG lHauteur = lPolicetmHeight ;
        if (lHauteur == 0)
            lHauteur = TailleChaine.cy ;
        (*it)->pTextLine->Boite.bottom -= lHauteur ;
        PointRef.Offset(0, -lHauteur) ;

        delete pPolice ;

        // Mise à jour de RectangleGlobal
        //
        if (RectangleGlobal.right < (*it)->pTextLine->Boite.right)
            RectangleGlobal.right = (*it)->pTextLine->Boite.right ;
        RectangleGlobal.bottom = (*it)->pTextLine->Boite.bottom ;
    }
	delete pDC;
}
catch (...)
{
    erreur("Exception NSRCHistoryView::InitialiseLignes.",  standardError, 0) ;
}
}

void
NSRCHistoryView::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
    if (Lignes.empty())
        return ;

	int iAncDC = dc.SaveDC() ;
	//
	// Fixe la métrique et l'origine
	//
	dc.SetMapMode(MM_LOMETRIC) ;
	//
	// Stockage des points supérieurs gauches et inférieurs droits du rectangle
	//
    NS_CLASSLIB::TPoint Points[2] ;
	Points[0] = RectAPeindre.TopLeft() ;
	Points[1] = RectAPeindre.BottomRight() ;
	//
	// Transformation de ces points en coordonnées logiques
	//
	dc.DPtoLP(Points, 2) ;
	//
	// Calcul du rectangle du document qui correspond au RectAPeindre de
	// l'écran
	// ATTENTION : dans le mode MM_LOMETRIC les coordonnées en ordonnées
	//				  sont négatives
	//
	NS_CLASSLIB::TRect* pRectDocument = new NS_CLASSLIB::TRect(Points[0], Points[1]) ;
	*pRectDocument = pRectDocument->Offset(HautGcheFenetre.x, HautGcheFenetre.y) ;
	pRectDocument->Normalized() ;

	//
	// On demande à toutes les lignes concernées de se repeindre
	//

    // repeinte de la zone d'affichage
    for (ItemHistoIter it = Lignes.begin(); it != Lignes.end(); it++)
    {
        NS_CLASSLIB::TRect BoiteNormale = (*it)->pTextLine->Boite;
		BoiteNormale.Normalized();
		if ((BoiteNormale.right  >= pRectDocument->left)  &&
			 (BoiteNormale.left   <= pRectDocument->right) &&
			 (BoiteNormale.bottom <= pRectDocument->top)   &&
			 (BoiteNormale.top 	 >= pRectDocument->bottom))
			//  ATTENTION : la méthode Touches de TRect ne fonctionne pas
			//	if (pRectDocument->Touches(BoiteNormale))
    	{
            string sTexte = (*it)->pTextLine->Texte ;
            //
            // Choix de la police
            //
            NSFont PoliceVoulue ;

            if      ((*it)->aDates.empty())
                PoliceVoulue = *(StylesPolice[0]) ;
            else if ((*it)->bInLastConsult)
                PoliceVoulue = *(StylesPolice[2]) ;
            else
                PoliceVoulue = *(StylesPolice[1]) ;

            if (PoliceVoulue.logFont.lfHeight >= 0)
            {
                NS_CLASSLIB::TPoint	PointTransfert ;
                PointTransfert.x = PoliceVoulue.logFont.lfHeight ;
                PointTransfert.y = PoliceVoulue.logFont.lfWidth ;
                dc.DPtoLP(&PointTransfert) ;
                PoliceVoulue.logFont.lfHeight = PointTransfert.x ;
                PoliceVoulue.logFont.lfWidth  = PointTransfert.y ;
            }

            TFont* pPolice = new TFont(&(PoliceVoulue.logFont)) ;
            dc.SelectObject(*pPolice) ;
            //
	        // Calcul de la taille effective de la ligne
        	//
            SIZE lpSize ;
	        BOOL Continuer = GetTextExtentPoint(HDC(dc), sTexte.c_str(), strlen(sTexte.c_str()), &lpSize) ;
	        if (Continuer)
            {
	            //
	            // Calcul du point écran où afficher
            	//
                NS_CLASSLIB::TPoint PointAffichage ;
	            PointAffichage.x = (*it)->pTextLine->Boite.left ;
	            PointAffichage.y = (*it)->pTextLine->Boite.top ;
	            PointAffichage -= HautGcheFenetre ;

	            dc.SetTextJustification(0, 0) ;                dc.SetTextColor((*it)->pTextLine->CouleurTexte) ;	            dc.TextOut(PointAffichage, sTexte.c_str()) ;
            }

	        dc.RestoreFont() ;
	        delete pPolice ;        }
	}
	delete pRectDocument ;
	dc.RestoreDC(iAncDC) ;
}

//---------------------------------------------------------------------------
//  Scroll vertical
//---------------------------------------------------------------------------
void
NSRCHistoryView::EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
{
	if (Lignes.empty())
		return ;

	const 	 NSItemHistoriser* pLigne;
	int		 iTailleScroll;
	NS_CLASSLIB::TPoint* pPointRef;
	//
	// On sort tout de suite si on est sur les lignes extrèmes
	//
	if (((scrollCode == SB_LINEDOWN) && (PremLigne == Lignes.size())) ||
        ((scrollCode == SB_PAGEDOWN) && (PremLigne == Lignes.size())) ||
        ((scrollCode == SB_LINEUP) && (PremLigne == 1)) ||
        ((scrollCode == SB_PAGEUP) && (PremLigne == 1)))
		return;
	//
	// On récupère un "contexte d'information" concernant l'écran
	//
	TIC* pDC = new TIC("DISPLAY", NULL, NULL);
	pDC->SetMapMode(MM_LOMETRIC);
	//
	// Scroll vers le haut (flêche basse)
	//
	if (scrollCode == SB_LINEDOWN)
	{
		//
		// Calcul de la quantité à scroller (hauteur de la première ligne)
		//
		pLigne = Lignes[PremLigne-1];
		iTailleScroll = pLigne->pTextLine->Boite.Height();
		//
		// Conversion en pixels
		//
		pPointRef = new NS_CLASSLIB::TPoint(0, iTailleScroll);
		pDC->LPtoDP(pPointRef);
		//
		// Demande de scroll
		//
		ScrollWindow(0, -pPointRef->y);
		delete pPointRef;
        pPointRef = 0;
		//
		// Ajustement des variables PremLigne et HautGcheFenetre
		//
		PremLigne++;
		HautGcheFenetre.y += iTailleScroll;
		//UpdateWindow();
	}
    //
	// Scroll vers le haut (page basse)	//
	else if (scrollCode == SB_PAGEDOWN)
    {
        NS_CLASSLIB::TRect rectClient = GetClientRect();
        int cyScroll = ::GetSystemMetrics(SM_CYHSCROLL);
        int hauteurLignes = 0;
        UINT numLigne = PremLigne; // numéro de la première ligne visible
        int hauteurFen = rectClient.bottom - rectClient.top - cyScroll; // hauteur fenetre en pixels
        int hBoite = 0, hLigne = 0;
        int tailleScroll = 0;

        while ((numLigne < Lignes.size()) && (hauteurLignes < hauteurFen))
        {
            pLigne = Lignes[numLigne - 1];
            hBoite = pLigne->pTextLine->Boite.Height();
            NS_CLASSLIB::TPoint py(0, hBoite);
            pDC->LPtoDP(&py);
            hLigne = py.y;

            tailleScroll += hBoite;
            hauteurLignes += hLigne;
            numLigne++;
        }

        PremLigne = numLigne;
        ScrollWindow(0, -hauteurLignes);
        HautGcheFenetre.y += tailleScroll;
    }
	//
	// Scroll vers le bas (flêche haute)
	//
	else if (scrollCode == SB_LINEUP)
	{
		//
		// Calcul de la quantité à scroller (hauteur de l'avant-première ligne)
		//
		pLigne = Lignes[PremLigne-2];
		iTailleScroll = pLigne->pTextLine->Boite.Height();
		//
		// Conversion en pixels
		//
		pPointRef = new NS_CLASSLIB::TPoint(0, iTailleScroll);
		pDC->LPtoDP(pPointRef);
		//
		// Demande de scroll
		//
		ScrollWindow(0, pPointRef->y);
		delete pPointRef;
        pPointRef = 0;
		//
		// Ajustement des variables PremLigne et HautGcheFenetre
		//
		PremLigne--;
		HautGcheFenetre.y -= iTailleScroll;
		//UpdateWindow();
	}
    //
	// Scroll vers le bas (page haute)
	//
	else if (scrollCode == SB_PAGEUP)
    {
        NS_CLASSLIB::TRect rectClient = GetClientRect();
        int cyScroll = ::GetSystemMetrics(SM_CYHSCROLL);
        int hauteurLignes = 0;
        int numLigne = PremLigne; // numéro de la première ligne visible
        int hauteurFen = rectClient.bottom - rectClient.top - cyScroll; // hauteur fenetre en pixels
        int hBoite = 0, hLigne = 0;
        int tailleScroll = 0;

        while ((numLigne > 1) && (hauteurLignes < hauteurFen))
        {
            pLigne = Lignes[numLigne - 2];  // Ligne précédente
            hBoite = pLigne->pTextLine->Boite.Height();
            NS_CLASSLIB::TPoint py(0, hBoite);
            pDC->LPtoDP(&py);
            hLigne = py.y;

            tailleScroll += hBoite;
            hauteurLignes += hLigne;
            numLigne--;
        }

        PremLigne = numLigne;
        ScrollWindow(0, hauteurLignes);
        HautGcheFenetre.y -= tailleScroll;
    }

	delete pDC;
    pDC = 0;
	SetScrollPos(SB_VERT, PremLigne);
}

//---------------------------------------------------------------------------
//  Scroll horizontal
//---------------------------------------------------------------------------
void
NSRCHistoryView::EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
{
	if (Lignes.empty())
        return ;

	NS_CLASSLIB::TPoint* pPointRef;
	//
	// On sort tout de suite si on est sur les lignes extrèmes
	//
	if (((scrollCode == SB_LINERIGHT) && (HautGcheFenetre.x == RectangleGlobal.Width())) ||
        ((scrollCode == SB_PAGERIGHT) && (HautGcheFenetre.x == RectangleGlobal.Width())) ||
		((scrollCode == SB_LINELEFT) && (HautGcheFenetre.x == 0)) ||
        ((scrollCode == SB_PAGELEFT) && (HautGcheFenetre.x == 0)))
		return;
	//
	// On récupère un "contexte d'information" concernant l'écran
	//
	TIC* pDC = new TIC("DISPLAY", NULL, NULL);
	pDC->SetMapMode(MM_LOMETRIC);
	//
	// Scroll vers la gauche (flêche droite)
	//
	if (scrollCode == SB_LINERIGHT)
	{
		//
		// Conversion en pixels
		//
		pPointRef = new NS_CLASSLIB::TPoint(LargeurPolice, 0);
		pDC->LPtoDP(pPointRef);
		//
		// Demande de scroll
		//
		ScrollWindow(-pPointRef->x, 0);
		delete pPointRef;
        pPointRef = 0;
		//
		// Ajustement de HautGcheFenetre
		//
		HautGcheFenetre.x += LargeurPolice;
	}
    //
	// Scroll vers la gauche (page droite)
	//
	if (scrollCode == SB_PAGERIGHT)
	{
		//
		// Conversion en pixels
		//
        int k = 1;
        pPointRef = new NS_CLASSLIB::TPoint(LargeurPolice, 0);
        pDC->LPtoDP(pPointRef);

        while ((k <= 10) && ((HautGcheFenetre.x + LargeurPolice) <= RectangleGlobal.Width()))
        {
		    //
		    // Demande de scroll
		    //
		    ScrollWindow(-pPointRef->x, 0);
            //
		    // Ajustement de HautGcheFenetre
		    //
            HautGcheFenetre.x += LargeurPolice;
            k++;
        }

		delete pPointRef;
        pPointRef = 0;
	}
	//
	// Scroll vers la droite (flêche gauche)
	//
	else if (scrollCode == SB_LINELEFT)
	{
		//
		// Conversion en pixels
		//
		pPointRef = new NS_CLASSLIB::TPoint(LargeurPolice, 0);
		pDC->LPtoDP(pPointRef);
		//
		// Demande de scroll
		//
		ScrollWindow(pPointRef->x, 0);
		delete pPointRef;
        pPointRef = 0;
		//
		// Ajustement de HautGcheFenetre
		//
		HautGcheFenetre.x -= LargeurPolice;
	}
    //
	// Scroll vers la droite (page gauche)
	//
	if (scrollCode == SB_PAGELEFT)
	{
		//
		// Conversion en pixels
		//
        int k = 1;
        pPointRef = new NS_CLASSLIB::TPoint(LargeurPolice, 0);
        pDC->LPtoDP(pPointRef);

        while ((k <= 10) && ((HautGcheFenetre.x - LargeurPolice) >= 0))
        {
		    //
		    // Demande de scroll
		    //
		    ScrollWindow(pPointRef->x, 0);
            //
		    // Ajustement de HautGcheFenetre
		    //
            HautGcheFenetre.x -= LargeurPolice;
            k++;
        }

		delete pPointRef;
        pPointRef = 0;
	}

	delete pDC;
    pDC = 0;
    if (LargeurPolice > 0)
		SetScrollPos(SB_HORZ, HautGcheFenetre.x / LargeurPolice);
}

//---------------------------------------------------------------------------
//  Molette
//---------------------------------------------------------------------------
LRESULT
NSRCHistoryView::EvMouseWheel(WPARAM wParam, LPARAM lParam)
{
	if (Lignes.empty())
		return 0 ;

    // WORD    fwKeys  = LOWORD(wParam) ;           // key flags
    short   zDelta  = (short) HIWORD(wParam) ;   // wheel rotation
    // short   xPos    = (short) LOWORD(lParam) ;   // horizontal position of pointer
    // short   yPos    = (short) HIWORD(lParam) ;   // vertical position of pointer

    UINT    scrollCode;

    // A positive value indicates that the wheel was rotated forward,
    // away from the user
    //
    if (zDelta > 0)
        scrollCode = SB_LINEUP ;

    // a negative value indicates that the wheel was rotated backward,
    // toward the user.
    //
    else if (zDelta < 0)
        scrollCode = SB_LINEDOWN ;

    for (int i = 0; i < 4; i++)
        EvVScroll(scrollCode, 0, 0) ;

    return 0 ;
}

void
NSRCHistoryView::reloadView(string sReason)
{
}

// --------------------------------------------------------------------------
//
// Class NSItemHistoriser
//
// --------------------------------------------------------------------------

NSItemHistoriser::NSItemHistoriser()
{
    sLocalisation   = "" ;
    sLexique        = "" ;
    sComplement     = "" ;

    pTextLine       = 0 ;
    bInLastConsult  = false ;
}

NSItemHistoriser::NSItemHistoriser(const NSItemHistoriser& rv)
{
try
{
  sLocalisation  = rv.sLocalisation ;
  sLexique       = rv.sLexique ;
  sComplement    = rv.sComplement ;

  if (rv.pTextLine)
    pTextLine    = new NSLigne(*(rv.pTextLine)) ;
  else
    pTextLine    = 0 ;
  aDates         = rv.aDates ;
  bInLastConsult = rv.bInLastConsult ;
}
catch (...)
{
    erreur("Exception NSItemHistoriser copy ctor.", standardError, 0) ;
}
}

NSItemHistoriser::~NSItemHistoriser()
{
    if (pTextLine)
        delete pTextLine ;
}

NSItemHistoriser&
NSItemHistoriser::operator=(const NSItemHistoriser& src)
{
try
{
  if (&src == this)
		return *this ;

  sLocalisation  = src.sLocalisation ;
  sLexique       = src.sLexique ;
  sComplement    = src.sComplement ;

  if (pTextLine)
    delete pTextLine ;
  if (src.pTextLine)
    pTextLine    = new NSLigne(*(src.pTextLine)) ;
  else
    pTextLine    = 0 ;
  aDates         = src.aDates ;
  bInLastConsult = src.bInLastConsult ;

  return *this ;
}
catch (...)
{
  erreur("Exception NSItemHistoriser = operator.", standardError, 0) ;
  return *this ;
}
}

// --------------------------------------------------------------------------
// ----------------------- METHODES DE ItemHistoArray -----------------------
// --------------------------------------------------------------------------

ItemHistoArray::ItemHistoArray()
               :ItemHistoVector()
{
}

ItemHistoArray::ItemHistoArray(const ItemHistoArray& rv)               :ItemHistoVector()
{
try
{
  if (false == rv.empty())
    for (ItemHistoConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSItemHistoriser(*(*i)));
}
catch (...)
{
  erreur("Exception ItemHistoArray copy ctor.",  standardError, 0) ;
}
}

void
ItemHistoArray::vider()
{
  if (empty())
    return ;

  for (ItemHistoIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

ItemHistoArray::~ItemHistoArray()
{
	vider();
}

ItemHistoArray&
ItemHistoArray::operator=(const ItemHistoArray& src)
{
try
{
  vider() ;

  if (false == src.empty())
    for (ItemHistoConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(new NSItemHistoriser(*(*i))) ;

  return *this ;
}
catch (...)
{
  erreur("Exception ItemHistoArray = operator.",  standardError, 0) ;
  return *this ;
}
}

