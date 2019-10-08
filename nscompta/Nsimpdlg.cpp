// nsimpdlg.cpp : dialogues des impayes// RS Octobre 98
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <classlib\date.h>
#include <classlib\time.h>
#include <owl\eventhan.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "partage\nsperson.h"
#include "nautilus\nspatdlg.h"	// pour le code prescript
#include "nautilus\nsadmiwd.h"
#include "nscompta\nscompta.rh"
#include "nscompta\nsfsedlg.h"
#include "nscompta\nsactdlg.h"
#include "nscompta\nsimpdlg.h"

//***************************************************************************
//  							Méthodes de NSImpDocument
//***************************************************************************

// Constructeur NSImpDocument
////////////////////////////////////////////////////////////////
NSImpDocument::NSImpDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
										NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
              :NSNoyauDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, true)
{
	// Initialisation des donnees
	pCriteres = new NSMultiCriteres ;
	pImpArray = new NSImpArray ;
	nbImp 	  = 0 ;
	pVar 	    = new NSVarCompta(pCtx) ;
	pTPArray  = new NSTPArray ;
	bImprimer = false ;
}

NSImpDocument::NSImpDocument(TDocument *parent, NSContexte *pCtx)              :NSNoyauDocument(parent, pCtx)
{
	// Initialisation des donnees
	pCriteres = new NSMultiCriteres ;
	pImpArray = new NSImpArray ;
	nbImp     = 0 ;
	pVar      = new NSVarCompta(pCtx) ;
	pTPArray  = new NSTPArray ;
	bImprimer = false ;
}

NSImpDocument::~NSImpDocument()
{
	delete pTPArray ;
	delete pVar ;
	delete pImpArray ;
	delete pCriteres ;
}

// Ouverture du document////////////////////////////////////////////////////////////////
bool NSImpDocument::Open(int /* mode */, LPCSTR /* path */)
{
	if (false == LanceCriteres())
		return false ;

	if (false == InitImpArray())		return false ;

	return true ;}

// Fermeture du document////////////////////////////////////////////////////////////////
bool NSImpDocument::Close()
{
	return true ;
}

boolNSImpDocument::LanceCriteres()
{
	NSMultiCritImpDialog* pCritImpDlg =
   	    new NSMultiCritImpDialog(pContexte->GetMainWindow(), pContexte) ;

	if (pCritImpDlg->Execute() != IDOK)
	{
  	delete pCritImpDlg ;
    return false ;
	}

	*pCriteres = *(pCritImpDlg->pCriteres) ;

	delete pCritImpDlg ;
	return true ;
}

/*
boolNSImpDocument::CherchePatient(string sNumSS, string& sNomPatient, NSPersonInfo& patInfo)
{
	NSPersonInfo tempInfo(pContexte, sNumSS, pidsPatient) ;

	sNomPatient = "" ;

	// on fabrique le nom long et on le stocke dans la string

	string sLang = "" ;
	if (pContexte->getUtilisateur())
		sLang = pContexte->getUserLanguage() ;

	sNomPatient = tempInfo.sCivilite ;
  patInfo = NSPersonInfo(tempInfo) ;
	// patInfo.initAdresseInfo() ;	// patInfo.initCorrespArray() ;

	return true ;}
*/

boolNSImpDocument::InitTPArray(string sNumCompt)
{
	// on récupère la cle
	string sNumTPayant = sNumCompt + string("  ") ;

	NSTPayant TPayant(pContexte->getSuperviseur()) ;

	pTPArray->vider() ;
	nbTP = 0 ;

	TPayant.lastError = TPayant.open() ;	if (TPayant.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base TPayant.", standardError, 0) ;
    return false ;
	}

	TPayant.lastError = TPayant.chercheClef(&sNumTPayant,                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

	if (TPayant.lastError == DBIERR_BOF)	// cas fichier vide
	{
		TPayant.close() ;
    return true ;		// le tableau est vide
	}

	if ((TPayant.lastError != DBIERR_NONE) && (TPayant.lastError != DBIERR_EOF))
	{
  	erreur("Erreur à la recherche d'une fiche TPayant.", standardError, TPayant.lastError) ;
    TPayant.close() ;
    return false ;
	}

	while (TPayant.lastError != DBIERR_EOF)
	{
  	TPayant.lastError = TPayant.getRecord() ;
    if (TPayant.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche TPayant.", standardError, TPayant.lastError) ;
      TPayant.close() ;
      return false ;
    }

    // condition d'arret
    if (!(string(TPayant._Donnees.numcompt) == sNumCompt))
    	break ;

    // on remplit le tableau
    pTPArray->push_back(new NSTPayantInfo(&TPayant)) ;
    nbTP++ ;

    // ... on passe au composant suivant
    TPayant.lastError = TPayant.suivant(dbiWRITELOCK) ;
    if ((TPayant.lastError != DBIERR_NONE) && (TPayant.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche TPayant.", standardError, TPayant.lastError) ;
      TPayant.close() ;
      return false ;
    }
  } // fin du while (recherche des tiers payants)

	// on ferme la base
	TPayant.lastError = TPayant.close() ;
  if (TPayant.lastError != DBIERR_NONE)
	{
  	erreur("Erreur de fermeture du fichier TPayant.", standardError, TPayant.lastError) ;
    return false ;
	}

	return true ;
}

boolNSImpDocument::SelectionCriteres(NSComptInfo* pComptInfo)
{
  if ((NSComptInfo*) NULL == pComptInfo)
    return false ;

	if (pCriteres->bActesPerso)
  	if (pComptInfo->getOperator() != pContexte->getUtilisateur()->getNss())
    	return false ;

	if (string("") != pCriteres->sCodeExamen)
  	if (pComptInfo->_Donnees.getExam() != pCriteres->sCodeExamen)
    	return false ;

	if (string("") != pCriteres->sCodePrescript)
		if (pComptInfo->_Donnees.getPrescriber() != pCriteres->sCodePrescript)
			return false ;

	if ((pCriteres->iContexte == 1) && (pComptInfo->_Donnees.contexte[0] != '\0'))
  	return false ;

	if ((pCriteres->iContexte == 2) && (strcmp(pComptInfo->_Donnees.contexte, "NEXTE")))
		return false ;

	if ((pCriteres->iContexte == 4) && (strcmp(pComptInfo->_Donnees.contexte, "NHOST")))
		return false ;

	if ((pCriteres->iContexte == 8) && (strcmp(pComptInfo->_Donnees.contexte, "NAMBU")))
  	return false ;

	return true ;}

boolNSImpDocument::InitImpArray()
{
	NSCompt Compt(pContexte->getSuperviseur()) ;

	// string sCodeExam, sLibExam ;

	// NSComptInfo* pComptInfo ;
	NSPersonInfo patInfo(pContexte->getSuperviseur()) ;

	pImpArray->vider() ;
	nbImp = 0 ;

	Compt.lastError = Compt.open() ;
	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base Compt.", standardError, Compt.lastError) ;
		return false ;
	}

	// on cherche les Compt par date
	Compt.lastError = Compt.chercheClef(&(pCriteres->sDate1),
                                      "DATE_COMPT",
                                      NODEFAULTINDEX,
                                      keySEARCHGEQ,
                                      dbiWRITELOCK) ;

	if (DBIERR_BOF == Compt.lastError)	// cas fichier vide
	{
		Compt.close() ;
		return true ; // le tableau est vide
	}

	if ((Compt.lastError != DBIERR_NONE) && (Compt.lastError != DBIERR_EOF))
	{
		erreur("Erreur à la recherche d'une fiche Compt.", standardError, Compt.lastError) ;
		Compt.close() ;
		return false ;
	}

	string sLang = pContexte->getUserLanguage() ;

	CptaSearchProgressDialog progessDialog(pContexte->GetMainWindow(), pContexte) ;
	progessDialog.Create() ;
	progessDialog.Show(SW_SHOW) ;

	while (Compt.lastError != DBIERR_EOF)
	{
		Compt.lastError = Compt.getRecord() ;
		if (Compt.lastError != DBIERR_NONE)
		{
			erreur("Erreur à la lecture d'une fiche Compt.", standardError, Compt.lastError) ;
      Compt.close() ;
      return false ;
    }

    progessDialog.ClearText() ;
    progessDialog.SetNewDate(string(Compt._Donnees.date)) ;    progessDialog.SetNewCode(string(Compt._Donnees.numcompt)) ;    progessDialog.SetAck("Lecture OK.") ;

    // cas des fiches compt représentant des recettes et non des actes
    // dans ce cas le nss patient est vide et on passe au suivant    // car une recette n'est forcement pas un impayé    //    if (strcmp(Compt._Donnees.patient, "") != 0)    {    	NSImpData ImpData(pContexte) ;

    	// condition d'arret
      if (string(Compt._Donnees.date) > (pCriteres->sDate2))
      	break ;

      NSComptInfo ComptInfo(&Compt) ;
      if (SelectionCriteres(&ComptInfo))      {
      	ImpData.metAZero() ;

/*
        if (!CherchePatient(string(Compt.pDonnees->patient), sNomLong, patInfo))        {
        	erreur("Impossible de retrouver le patient rattaché à la fiche Compt.", standardError, 0) ;
          Compt.close() ;
          return false ;
        }

        ImpData.patInfo = patInfo ;
*/

        InitImpdataFromCompt(&ImpData, &ComptInfo, sLang) ;

        // on retrouve le TPArray relatif à la fiche compt en cours        if (false == InitTPArray(string(Compt._Donnees.numcompt)))
        {
        	erreur("Impossible d'initialiser les tiers-payant rattachés à le fiche Compt.", standardError, 0) ;
          Compt.close() ;
          return false ;
        }

        insertInTPArrayForCompt(&ImpData, &ComptInfo) ;      } // fin du if (SelectionCriteres(pComptInfo))
    }
    // ... on passe à la fiche Compt suivante    Compt.lastError = Compt.suivant(dbiWRITELOCK) ;
    if ((Compt.lastError != DBIERR_NONE) && (Compt.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche Compt.", standardError, Compt.lastError) ;
      Compt.close() ;
      return false ;
    }
  } // fin du while

	// on ferme la base COMPT	Compt.lastError = Compt.close() ;
	if (Compt.lastError != DBIERR_NONE)
		erreur("Erreur de fermeture du fichier Compt.", standardError, Compt.lastError) ;

	return true ;
}

void
NSImpDocument::insertInTPArrayForCompt(NSImpData *pImpData, NSComptInfo *pComptInfo)
{
  if (NULL == pImpData)
    return ;

  // s'il existe une sélection sur le code organisme
  if ((pCriteres->sCodeOrga != "") && (false == pTPArray->empty()))
  {
    // On supprime tous les autres
    //
    for (NSTPIter i = pTPArray->begin(); pTPArray->end() != i ; )
    {
      if (string((*i)->_Donnees.organisme) != pCriteres->sCodeOrga)
      {
        delete *i ;
        pTPArray->erase(i) ;
      }
      else
        i++ ;
    }

    int iSommeLoc = 0 ;
    int iSommeEuro = 0 ;

    InsertTpFromArray(pImpData, iSommeEuro, iSommeLoc) ;
  }
  else // on itere pour tous les organismes + le patient
  {
    int iSommeLoc = 0 ;
    int iSommeEuro = 0 ;

    InsertTpFromArray(pImpData, iSommeEuro, iSommeLoc) ;

    // on termine par le patient    int iDuPatLoc = (atoi(pComptInfo->_Donnees.duFranc)
                                - atoi(pComptInfo->_Donnees.payeFranc)
                                - iSommeLoc) ;

    int iDuPatEuro = (atoi(pComptInfo->_Donnees.duEuro)                                - atoi(pComptInfo->_Donnees.payeEuro)
                                - iSommeEuro) ;

    if ((iDuPatLoc > 0) || (iDuPatEuro > 0))    {
      char cDuPatLoc[9] = "" ;
      char cDuPatEuro[9] = "" ;

      itoa(iDuPatLoc, cDuPatLoc, 10) ;
      itoa(iDuPatEuro, cDuPatEuro, 10) ;
      strcpy(pImpData->impayeLoc, cDuPatLoc) ;
      strcpy(pImpData->impayeEuro, cDuPatEuro) ;
      strcpy(pImpData->libOrga, "") ;

      pImpArray->push_back(new NSImpData(*pImpData)) ;
      nbImp++ ;
    }
  } 
}

void
NSImpDocument::InitImpdataFromCompt(NSImpData *pImpData, NSComptInfo *pComptInfo, string sLang)
{
  if (((NSImpData*) NULL == pImpData) || ((NSComptInfo*) NULL == pComptInfo))
    return ;

  string sCodeExam = pComptInfo->_Donnees.getExam() + pComptInfo->_Donnees.getSynonym() ;
  string sLibExam ;
  pContexte->getDico()->donneLibelle(sLang, &sCodeExam, &sLibExam) ;
  pImpData->sLibExam = sLibExam ;

  strcpy(pImpData->numCompt,     pComptInfo->_Donnees.numcompt) ;  strcpy(pImpData->dateCompt,    pComptInfo->_Donnees.date) ;
  strcpy(pImpData->codePatient,  pComptInfo->_Donnees.patient) ;
  strcpy(pImpData->sommeDueLoc,  pComptInfo->_Donnees.duFranc) ;
  strcpy(pImpData->sommeDueEuro, pComptInfo->_Donnees.duEuro) ;
}

void
NSImpDocument::InsertTpFromArray(NSImpData* pImpdata, int &iSommeEuro, int &iSommeLoc)
{
  if ((NULL == pImpdata) || pTPArray->empty())
    return ;

  for (NSTPIter i = pTPArray->begin() ; pTPArray->end() != i ; i++)
  {
    int  iResteDu = 0 ;
    char cResteDu[9] = "" ;

    if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
    {
      strcpy(pImpdata->impayeLoc, (*i)->_Donnees.reste_du) ;
      iResteDu = atoi((*i)->_Donnees.reste_du) ;
                               
      int iResteDuEuro = pVar->getEuroFromLocal(iResteDu) ;
      itoa(iResteDuEuro, cResteDu, 10) ;
      strcpy(pImpdata->impayeEuro, cResteDu) ;

      if (iResteDu > 0)
      {
        iSommeLoc  += iResteDu ;
        iSommeEuro += iResteDuEuro ;
      }
    }
    else // le reste du est exprimé en Euro
    {
      strcpy(pImpdata->impayeEuro, (*i)->_Donnees.reste_du) ;
      iResteDu = atoi((*i)->_Donnees.reste_du) ;

      int iResteDuLocal = pVar->getLocalFromEuro(iResteDu) ;
      itoa(iResteDuLocal, cResteDu, 10) ;
      strcpy(pImpdata->impayeLoc, cResteDu) ;

      if (iResteDu > 0)
      {
        iSommeLoc  += iResteDuLocal ;
        iSommeEuro += iResteDu ;
      }
    }

    strcpy(pImpdata->libOrga, (*i)->_Donnees.libelle) ;
    if (iResteDu > 0)    {
      pImpArray->push_back(new NSImpData(*pImpdata)) ;
      nbImp++ ;
    }
  }
}

void
NSImpDocument::removeFromTPArray(string sNumCompt, NSImpData *pImpData)
{
  if (pImpArray->empty())
    return ;

  for (NSImpIter iter = pImpArray->begin() ; pImpArray->end() != iter ; )
  {
    if (sNumCompt == string((*iter)->numCompt))
    {
      if (NULL != pImpData)
        *pImpData = **iter ;

      delete *iter ;
      pImpArray->erase(iter) ;
      nbImp-- ;
    }
    else
      iter++ ;
  }
}

//***************************************************************************//  							Méthodes de NSListImpWindow
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSListImpWindow, TListWindow)
   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

void
NSListImpWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
}

//---------------------------------------------------------------------------
//  Function: NSListImpWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListImpWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
		pDlg->CmModifImp() ;
}

//---------------------------------------------------------------------------//  Function: NSListImpWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListImpWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0; i < count; i++)
		if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;
}

//***************************************************************************//
//  Méthodes de NSListImpDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSListImpDialog, NSUtilDialog)
    EV_LV_DISPINFO_NOTIFY(IDC_LI_LW, LVN_GETDISPINFO, DispInfoListeImp),
    EV_BN_CLICKED(IDC_LI_LOC, SwitchLocEuro),
    EV_BN_CLICKED(IDC_LI_EURO, SwitchLocEuro),
    EV_COMMAND(IDC_LI_IMPRIMER, CmImprimer),
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSListImpDialog::NSListImpDialog(TWindow* pere, NSContexte* pCtx, NSImpDocument* pImpDoc)
                :NSUtilDialog(pere, pCtx, "IDD_LISTIMP", pNSResModule)
{
	pDoc	    = pImpDoc ;

	pMonnaie  = new TGroupBox(this, IDC_LI_MONNAIE) ;
	pLocal	  = new TRadioButton(this, IDC_LI_LOC, pMonnaie) ;
	pEuro	    = new TRadioButton(this, IDC_LI_EURO, pMonnaie) ;

	pListeImp = new NSListImpWindow(this, IDC_LI_LW) ;
	pVar      = new NSVarCompta(pCtx) ;
}

//// Destructeur
//
NSListImpDialog::~NSListImpDialog()
{
	delete pMonnaie ;
	delete pLocal ;
  delete pEuro ;
  delete pListeImp ;
  delete pVar ;
}

//
// Fonction SetupWindow
//
void
NSListImpDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	pLocal->SetCaption(pVar->getSigle().c_str()) ;

	if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
	{
  	pLocal->Check() ;
    pDoc->bEuro = false ;
	}
	else
	{
  	pEuro->Check() ;
    pDoc->bEuro = true ;
	}

	InitListeImp() ;
	AfficheListeImp() ;
}

voidNSListImpDialog::InitListeImp()
{
	TListWindColumn colDate("Date", 80, TListWindColumn::Left, 0) ;
	pListeImp->InsertColumn(0, colDate) ;
	TListWindColumn colNomPat("Nom - Prénom", 100, TListWindColumn::Left, 1) ;
  pListeImp->InsertColumn(1, colNomPat) ;
  TListWindColumn colCode("Code", 50, TListWindColumn::Left, 2) ;
  pListeImp->InsertColumn(2, colCode) ;
  TListWindColumn colExam("Examen", 80, TListWindColumn::Left, 3) ;
  pListeImp->InsertColumn(3, colExam) ;
  TListWindColumn colSommeDue("Somme Due", 80, TListWindColumn::Right, 4) ;
  pListeImp->InsertColumn(4, colSommeDue) ;
  TListWindColumn colImpaye("Impayé", 80, TListWindColumn::Right, 5) ;
  pListeImp->InsertColumn(5, colImpaye) ;
  TListWindColumn colOrga("Organisme", 80, TListWindColumn::Left, 6) ;
  pListeImp->InsertColumn(6, colOrga) ;
}

voidNSListImpDialog::AfficheListeImp()
{
	string sLang = pContexte->getUserLanguage() ;

	pListeImp->DeleteAllItems();
	for (int i = pDoc->nbImp - 1; i >= 0; i--)	{
    string sDateExam = donne_date(((*(pDoc->pImpArray))[i])->dateCompt, sLang) ;
    TListWindItem Item(sDateExam.c_str(), 0) ;
    pListeImp->InsertItem(Item) ;
	}
}

voidNSListImpDialog::DispInfoListeImp(TLwDispInfoNotify& dispInfo)
{
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;

  NSImpData* pImpData = (*(pDoc->pImpArray))[index] ;
  if ((NSImpData*) NULL == pImpData)
  {
    dispInfoItem.SetText("") ;
    return ;
  }

	const int 	BufLen = 1024 ;
	static char buffer[BufLen] = "" ;

  int	montant = 0 ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // nom long patient

      if (false == pImpData->bPatientLoaded)
        pImpData->initPatientInfo() ;

    	sprintf(buffer, "%s", pImpData->sNomPatient.c_str()) ;
      break ;

		case 2: // code nautilus patient

    	sprintf(buffer, "%s", pImpData->patInfo.getNss().c_str()) ;
      break ;

    case 3: // examen

    	sprintf(buffer, "%s", pImpData->sLibExam.c_str()) ;      break ;

    case 4: // somme due totale

      if (false == pDoc->bEuro)        montant = atoi(pImpData->sommeDueLoc) ;
      else
        montant = atoi(pImpData->sommeDueEuro) ;

      sprintf(buffer, "%d,%02d ", montant/100, montant%100) ;

      if (false == pDoc->bEuro)
        strcat(buffer, pVar->getSigle().c_str()) ;
      else
        strcat(buffer, "€") ;

      break;

    case 5: // impayé

      if (false == pDoc->bEuro)
        montant = atoi(pImpData->impayeLoc) ;
      else
        montant = atoi(pImpData->impayeEuro) ;

      sprintf(buffer, "%d,%02d ", montant/100, montant%100) ;

      if (false == pDoc->bEuro)
        strcat(buffer, pVar->getSigle().c_str()) ;
      else
        strcat(buffer, "€") ;

      break;

    case 6: 	// organisme

    	sprintf(buffer, "%s", pImpData->libOrga) ;
      break ;
	} // fin du switch

  dispInfoItem.SetText(buffer) ;
}

voidNSListImpDialog::SwitchLocEuro()
{
	if (pLocal->GetCheck() == BF_CHECKED)		pDoc->bEuro = false ;
	else
		pDoc->bEuro = true ;

	AfficheListeImp() ;
}

voidNSListImpDialog::CmModifImp()
{
	int index = pListeImp->IndexItemSelect() ;

	if (-1 == index)
	{
		erreur("Vous devez sélectionner une fiche Impayée.", warningError, 0) ;
    return ;
	}

	string sNumCompt = string(((*(pDoc->pImpArray))[index])->numCompt) ;

  NSPersonInfo    PatInfo((*(pDoc->pImpArray))[index]->patInfo) ;
  NSComptaPatient compta(pContexte, &PatInfo) ;

  NSComptInfo     ComptInfo ;

  if (false == compta.EditCompt(sNumCompt, this, &ComptInfo))
    return ;

/*

	NSCompt Compt(pContexte->getSuperviseur()) ;

	Compt.lastError = Compt.open() ;
	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base Compt.", standardError, 0) ;
    return ;
	}

	Compt.lastError = Compt.chercheClef(&sNumCompt,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à la recherche d'une fiche compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    return ;
	}

	Compt.lastError = Compt.getRecord() ;  if (Compt.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à lecture du fichier Compt.db", standardError, Compt.lastError);
    Compt.close() ;
    return ;
  }

	// on lance la modif des infos récupérées
	NSPersonInfo PatInfo((*(pDoc->pImpArray))[index]->patInfo) ;

	CreerFicheComptDialog* pComptDlg = new CreerFicheComptDialog(this, pContexte, &PatInfo, false) ;

	*(pComptDlg->pData) = *(Compt.pDonnees) ;

	if ((pComptDlg->Execute()) == IDCANCEL)
	{
  	Compt.close() ;
    delete pComptDlg ;
    return ;
	}

	// on stocke les donnees du dialogue dans les Data
	*(Compt.pDonnees) = *(pComptDlg->pData) ;

	Compt.lastError = Compt.modifyRecord(TRUE) ;
	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à la modification de la fiche compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    delete pComptDlg ;
    return ;
	}

  NSComptInfo ComptInfo(&Compt) ;

	Compt.lastError = Compt.close() ;
	if (Compt.lastError != DBIERR_NONE)
  	erreur("Erreur à la fermeture de la base Compt.db.", standardError, Compt.lastError) ;

	// on enregistre les autres donnees sous le meme numcompt
	pComptDlg->EnregDonneesCompt(sNumCompt) ;

	delete pComptDlg ;

*/

  // Eviter qu'un rafraichissement se produise entre la mise à jour de l'array
  // et la mise à jour de la liste
  pListeImp->DeleteAllItems() ;

  // on recharge le tableau depuis la base
	// car on a pu modifier la fiche Compt.

	// pDoc->InitImpArray() ; Beaucoup trop cher !
  //

  NSImpData ImpData(pContexte) ;

  // First, we remove all entries related to this Compt (sending ImpData may allow us to get patient's name from deleted entries)
  //
  pDoc->removeFromTPArray(sNumCompt, &ImpData) ;

  // We can actualize ImpData from ComptInfo
  //
  string sLang = pContexte->getUserLanguage() ;
  pDoc->InitImpdataFromCompt(&ImpData, &ComptInfo, sLang) ;

  // Then we get an updated list of non patient debt for this Compt
  //
  if (false == pDoc->InitTPArray(sNumCompt))
    erreur("Impossible d'initialiser les tiers-payant rattachés à le fiche Compt.", standardError, 0) ;
  else
    // Now we can have an updated list
    //
    pDoc->insertInTPArrayForCompt(&ImpData, &ComptInfo) ;

	AfficheListeImp() ;
}

voidNSListImpDialog::CmImprimer()
{
	pDoc->bImprimer = true ;
	CmOk() ;
}

voidNSListImpDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

voidNSListImpDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

//***************************************************************************//
//  Méthodes de NSMultiCritImpDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSMultiCritImpDialog, NSUtilDialog)
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
   EV_BN_CLICKED(IDC_MCI_PRESCRIPT, CmPrescript),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSMultiCritImpDialog::NSMultiCritImpDialog(TWindow* pere, NSContexte* pCtx)
                     :NSUtilDialog(pere, pCtx, "IDD_MULTICRITIMP", pNSResModule)
{
	// Initialisation des donnees
	pCriteres 		= new NSMultiCriteres ;

	// Création de tous les "objets de contrôle"
  pDate1 	 		= new NSUtilEditDate(pContexte, this, IDC_MCI_DATE1) ;
  pDate2			= new NSUtilEditDate(pContexte, this, IDC_MCI_DATE2) ;
  pExamen			= new NSUtilLexique(pContexte, this, IDC_MCI_EXAMEN, pCtx->getDico()) ;
  pPrescript	= new TStatic(this, IDC_MCI_PRESCRIPT) ;
  pActes			= new TGroupBox(this, IDC_MCI_ACTES) ;
  pActesPerso = new TRadioButton(this, IDC_MCI_ACTES_PERSO, pActes) ;
  pActesTous	= new TRadioButton(this, IDC_MCI_ACTES_TOUS, pActes) ;
  pCtxt			  = new TGroupBox(this, IDC_MCI_CTXT) ;
  pCtxtTous		= new TRadioButton(this, IDC_MCI_CTXT_TOUS, pCtxt) ;
  pCtxtNP		  = new TRadioButton(this, IDC_MCI_CTXT_NP, pCtxt) ;
  pCtxtExt		= new TRadioButton(this, IDC_MCI_CTXT_EXT, pCtxt) ;
  pCtxtAmbu		= new TRadioButton(this, IDC_MCI_CTXT_AMBU, pCtxt) ;
  pCtxtHosp		= new TRadioButton(this, IDC_MCI_CTXT_HOSP, pCtxt) ;
  pOrga				= new TComboBox(this, IDC_MCI_ORGA) ;

  pCodeOrgaArray = new NSCodeOrgaArray ;
  nbCodeOrga		= 0 ;
}

//// Destructeur
//
NSMultiCritImpDialog::~NSMultiCritImpDialog()
{
	delete pCriteres ;
  delete pDate1 ;
  delete pDate2 ;
  delete pExamen ;
  delete pPrescript ;
  delete pActes ;
  delete pActesPerso ;
  delete pActesTous ;
  delete pCtxt ;
  delete pCtxtNP ;
  delete pCtxtTous ;
  delete pCtxtExt ;
  delete pCtxtAmbu ;
  delete pCtxtHosp ;
  delete pOrga ;
  delete pCodeOrgaArray ;
}

//// Fonction SetupWindow
//
void
NSMultiCritImpDialog::SetupWindow()
{
	// fichiers d'aide
	sHindex = "";
	sHcorps = "Liste_des_impayes.html";
	TDate	dateSys;
	char	dateJour[9] = "";

	NSUtilDialog::SetupWindow();

	sprintf(dateJour, "%4d%02d%02d", (int)dateSys.Year(),
      				(int)dateSys.Month(), (int)dateSys.DayOfMonth());

	pDate1->setDate(dateJour) ;
	pDate2->setDate(dateJour) ;

	pActesTous->Check() ;
	pCtxtTous->Check() ;

   // on charge la table des organismes
	if (!InitCodeOrgaArray())
	{
		erreur("Probleme au chargement du tableau des codes organisme.", standardError, 0) ;
		return;
	}
}

boolNSMultiCritImpDialog::InitCodeOrgaArray()
{
	NSCodeOrga CodeOrga(pContexte->getSuperviseur()) ;

	CodeOrga.lastError = CodeOrga.open() ;
	if (CodeOrga.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base CodeOrga.", standardError, CodeOrga.lastError) ;
    return false ;
	}

	CodeOrga.lastError = CodeOrga.debut(dbiWRITELOCK) ;
	if ((CodeOrga.lastError != DBIERR_NONE) && (CodeOrga.lastError != DBIERR_EOF))
	{
  	erreur("Erreur de positionnement dans le fichier CodeOrga.db.", standardError, CodeOrga.lastError) ;
    CodeOrga.close() ;
    return false ;
	}

	while (CodeOrga.lastError != DBIERR_EOF)
	{
		CodeOrga.lastError = CodeOrga.getRecord() ;
    if (CodeOrga.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche CodeOrga.", standardError, CodeOrga.lastError) ;
      CodeOrga.close() ;
      return false ;
    }

    // on remplit le tableau et la combobox
    pCodeOrgaArray->push_back(new NSCodeOrgaInfo(&CodeOrga)) ;
    pOrga->AddString(CodeOrga._Donnees.lib_court) ;
    nbCodeOrga++ ;

    // ... on passe au composant suivant
    CodeOrga.lastError = CodeOrga.suivant(dbiWRITELOCK) ;
    if ((CodeOrga.lastError != DBIERR_NONE) && (CodeOrga.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche CodeOrga.", standardError, CodeOrga.lastError) ;
      CodeOrga.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

	// on ferme la base CARTE_SV2
	CodeOrga.lastError = CodeOrga.close() ;
	if (CodeOrga.lastError != DBIERR_NONE)
	{
		erreur("Erreur de fermeture du fichier CodeOrga.", standardError, CodeOrga.lastError) ;
    return false ;
	}

	return true ;
}

voidNSMultiCritImpDialog::CmPrescript()
{
	string sTitre ;

  NSTPersonListDialog indep((TWindow *)this, pidsCorresp, false, pContexte, 0, true) ;
  int	iDialogReturn = indep.Execute() ;
  if (iDialogReturn != IDOK)
  	return ;

	NSPersonInfo *temp = indep.pPersonSelect ;
  if (string("") == temp->getNss())
		return ;

	string sPids = temp->getNss() ;
	pCriteres->sCodePrescript = sPids ;

	NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPids, pidsCorresp) ;
  if (NULL == pPersonInfo)
  	return ;

	sTitre = pPersonInfo->getNom() + " " + pPersonInfo->getPrenom() ;

  pPrescript->SetCaption(sTitre.c_str()) ;
	pCriteres->sTitrePrescript = sTitre ;

/*
	string sTitre;
	ChercheListePatCorDialog* pListeCorDlg;
   pListeCorDlg = new ChercheListePatCorDialog(this, pContexte, pNSResModule);
   if ((pListeCorDlg->Execute() == IDOK) && (pListeCorDlg->CorrespChoisi >= 0))
   {
      pCriteres->sCodePrescript = string(pListeCorDlg->pCorrespSelect->pDonnees->code);
      sTitre = pListeCorDlg->pCorrespSelect->pDonnees->donneTitre();
      pPrescript->SetCaption(sTitre.c_str());
      pCriteres->sTitrePrescript = sTitre;
   }
	delete pListeCorDlg;
*/
}

voidNSMultiCritImpDialog::CmOk()
{
  // Get starting date
  //
  string sDate = string("") ;
	pDate1->getDate(&sDate) ;
	if (string("") == sDate)
	{
  	erreur("Vous devez saisir une date de début.", warningError, 0) ;
    return ;
	}
  pCriteres->sDate1 = sDate ;

  // Get ending date
  //
  pDate2->getDate(&sDate) ;
	if (string("") == sDate)
	{
  	erreur("Vous devez saisir une date de fin.", warningError, 0) ;
    return ;
	}
	pCriteres->sDate2 = sDate ;

	if ((pCriteres->sDate2) < (pCriteres->sDate1))
	{
  	erreur("La date de début ne peut etre supérieure à la date de fin.", warningError, 0) ;
    return ;
	}

  char far cfTexte[7] = "" ;
	pExamen->GetText(cfTexte, 7) ;
	if (!strcmp(cfTexte, ""))
	{
  	pCriteres->sCodeExamen = string("") ;
    pCriteres->sSynExamen  = string("") ;
	}
	else
	{
  	string sCode = pExamen->getCode() ;
	  pCriteres->sCodeExamen = string(sCode, 0, 5) ;
    pCriteres->sSynExamen = string(sCode, 5, 1) ;
	}

	if      (pActesPerso->GetCheck() == BF_CHECKED)
   	pCriteres->bActesPerso = true ;
	else if (pActesTous->GetCheck() == BF_CHECKED)
		pCriteres->bActesPerso = false ;
	else
	{
  	erreur("Vous devez saisir un choix (Vos actes / Tous les actes).", warningError, 0) ;
		return ;
	}

	if      (pCtxtTous->GetCheck() == BF_CHECKED)
   	pCriteres->iContexte = 0 ;
  else if (pCtxtNP->GetCheck()   == BF_CHECKED)
   	pCriteres->iContexte = 1 ;
	else if (pCtxtExt->GetCheck()  == BF_CHECKED)
   	pCriteres->iContexte = 2 ;
	else if (pCtxtHosp->GetCheck() == BF_CHECKED)
   	pCriteres->iContexte = 4 ;
	else if (pCtxtAmbu->GetCheck() == BF_CHECKED)
   	pCriteres->iContexte = 8 ;
	else
	{
  	erreur("Vous devez choisir un contexte.", warningError, 0) ;
    return ;
	}

	int indexOrga = pOrga->GetSelIndex();
	if (indexOrga >= 0)
	{
  	pCriteres->sCodeOrga     = string((*pCodeOrgaArray)[indexOrga]->_Donnees.code) ;
    pCriteres->sLibCourtOrga = string((*pCodeOrgaArray)[indexOrga]->_Donnees.lib_court) ;
    pCriteres->sLibLongOrga  = string((*pCodeOrgaArray)[indexOrga]->_Donnees.lib_long) ;
	}

   TDialog::CmOk() ;
}

voidNSMultiCritImpDialog::CmCancel()
{
	TDialog::CmCancel();
}

// fin de nsimpdlg.cpp//////////////////////////////////////////////////////////

