// nsfsedlg.cpp : fichier des dialogues des fiches FSE16XX//////////////////////////////////////////////////////////

#include <stdio.h>
#include <classlib\date.h>
#include <classlib\time.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nscompta\nscompta.h"
#include "nscompta\nscompta.rh"
#include "nscompta\nsf16dlg.h"
#include "partage\nscim10.h"

// -----------------------------------------------------------------//
//  Méthodes de CreerFse1610Dialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CreerFse1610Dialog, NSUtilDialog)
	// Recalcul du montant sur les killfocus des controles agissants
	EV_CBN_KILLFOCUS(IDC_1610_CODE, CalculMontant),
	EV_EN_KILLFOCUS(IDC_1610_COEFF, CalculMontant),
  EV_EN_KILLFOCUS(IDC_1610_DIVIS, CalculMontant),
  EV_EN_KILLFOCUS(IDC_1610_QTE, 	CalculMontant),

	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//
CreerFse1610Dialog::CreerFse1610Dialog(TWindow* pere, NSContexte* pCtx, NSVarCompta* pVarCompt)
                   :NSUtilDialog(pere, pCtx, "IDD_1610", pNSResModule)
{
	// Initialisation des donnees
	pData        = new NSFse1610Data ;
	pVar         = new NSVarCompta(*pVarCompt) ;

	// Création de tous les "objets de contrôle"
	pDateExe     = new NSUtilEditDate(pContexte, this, IDC_1610_DATE) ;
	pHeureExe	   = new NSUtilEditHeure(pContexte, this, IDC_1610_HEURE) ;
	pLieuExe     = new TComboBox(this, IDC_1610_LIEU) ;
	pQualif	     = new TComboBox(this, IDC_1610_QUALIF) ;
	pCode	       = new TComboBox(this, IDC_1610_CODE) ;
	pCoeff       = new NSUtilEditSomme(pContexte, this, IDC_1610_COEFF, FSE1610_COEFF_LEN + 1) ;
	pDivis	     = new NSUtilEdit2(pContexte, this, IDC_1610_DIVIS, FSE1610_DIVIS_LEN) ;
	pQuantite    = new NSUtilEdit2(pContexte, this, IDC_1610_QTE, FSE1610_QUANTITE_LEN) ;
	pCompl       = new TComboBox(this, IDC_1610_COMPL) ;
	pDenombr	   = new NSUtilEdit2(pContexte, this, IDC_1610_DNBR, FSE1610_DENOMBR_LEN) ;
	pMontant	   = new NSUtilEditSomme(pContexte, this, IDC_1610_MONTANT) ;
	pSigle			 = new TStatic(this, IDC_1610_SIGLE) ;

	pCodeArray   = new NSCodPrestArray ;

	pLieuArray   = new NSComboArray ;
	pComplArray  = new NSComboArray ;
	pQualifArray = new NSComboArray ;

	nbCode = 0 ;
}

//// Destructeur
//
CreerFse1610Dialog::~CreerFse1610Dialog()
{
	delete pData ;
  delete pDateExe ;
  delete pHeureExe ;
  delete pLieuExe ;
  delete pQualif ;
  delete pCode ;
  delete pCoeff ;
  delete pDivis ;
  delete pQuantite ;
  delete pCompl ;
  delete pDenombr ;
  delete pMontant ;
  delete pSigle ;
  delete pCodeArray ;
  delete pLieuArray ;
  delete pComplArray ;
  delete pQualifArray ;
}

//// Fonction SetupWindow : Mise en place des data à l'écran et initialisation
//	du tableau des Fse16xx + liste
//
void
CreerFse1610Dialog::SetupWindow()
{
	string 			sFichLieu = pContexte->getSuperviseur()->PathBase("BCPT") + string("lieu_exe.dat") ;
	string 			sFichCompl = pContexte->getSuperviseur()->PathBase("BCPT") + string("complem.dat") ;
	string 			sFichQualif = pContexte->getSuperviseur()->PathBase("BCPT") + string("qualif.dat") ;
	NSComboIter i ;
	int         j ;
	NSCodPrestIter k ;
	char			dateExe[9] = "" ;
	char			heureExe[5] = "" ;

	TDialog::SetupWindow() ;

  string sMonnaieSigle = pVar->getMonnaieSigle() ;
  pSigle->SetText(sMonnaieSigle.c_str()) ;

	if (!strcmp(pData->date, ""))
	{
  	pDateExe->setDate(pVar->getDateC()) ;
    pHeureExe->setHeure(pVar->getHeureC()) ;
	}
	else // si la date n'est pas vide, on l'affiche
	{
  	strncpy(dateExe, pData->date, 8) ;
    strcpy(heureExe, &((pData->date)[8])) ;

    pDateExe->setDate(string(dateExe)) ;
    pHeureExe->setHeure(string(heureExe)) ;
	}

	if (!InitComboBox(pLieuExe, pLieuArray, sFichLieu))
		erreur("Pb à l'initialisation de la combobox LieuExe", standardError, 0) ;
	else
		for (i = pLieuArray->begin(), j = 0; i != pLieuArray->end(); i++,j++)
    {
    	if (!strcmp(((*i)->sCode).c_str(), pData->lieu))
      {
      	pLieuExe->SetSelIndex(j) ;
        break ;
      }
    }

	if (!InitComboBox(pQualif, pQualifArray, sFichQualif))
		erreur("Pb à l'initialisation de la combobox Qualif", standardError, 0) ;
	else
		for (i = pQualifArray->begin(), j = 0; i != pQualifArray->end(); i++, j++)
    {
    	if (!strcmp(((*i)->sCode).c_str(), pData->qualif))
      {
      	pQualif->SetSelIndex(j) ;
        break ;
      }
    }

	// on charge la table des codes prest
	if (!InitCodeArray())
	{
		erreur("Probleme au chargement du tableau des codes prestation.", standardError, 0) ;
    return ;
	}

	// sélection du code dans la combobox
  for (k = pCodeArray->begin(), j = 0 ; pCodeArray->end() != k ; k++,j++)
  {
  	if ((*k)->getCode() == string(pData->code))
    {
    	pCode->SetSelIndex(j) ;
      break ;
    }
	}

	// pour éviter la division par zéro
	if (atoi(pData->divis) == 0)
	{
  	strcpy(pData->divis, "1") ;
    pDivis->SetText(pData->divis) ;
	}
	else
  	pDivis->SetText(pData->divis) ;

	// pour l'instant on est obligé d'initialiser l'affichage du coeff
  char coeff[255] ;
  itoa(atoi(pData->coeff) * atoi(pData->divis), coeff, 10) ;
  pCoeff->setSomme(string(coeff)) ;

	if (!strcmp(pData->quantite, ""))
  	pQuantite->SetText("1") ;
  else
  	pQuantite->SetText(pData->quantite) ;

	if (!InitComboBox(pCompl, pComplArray, sFichCompl))
  	erreur("Pb à l'initialisation de la combobox Compl", standardError, 0) ;
	else
  	for (i = pComplArray->begin(), j = 0; i != pComplArray->end(); i++,j++)
    {
    	if (!strcmp(((*i)->sCode).c_str(), pData->complmnt))
      {
      	pCompl->SetSelIndex(j) ;
        break ;
      }
    }

	if (!strcmp(pData->denombr, ""))
  	pDenombr->SetText("1") ;
	else
  	pDenombr->SetText(pData->denombr) ;

	// on calcule le montant de la prestation en fonction des données
	CalculMontant() ;
}

//// 	InitCodeArray() : Charge les codes prest en mémoire
//
bool
CreerFse1610Dialog::InitCodeArray()
{
	NSCodPrest CodPrest(pContexte->getSuperviseur()) ;

	CodPrest.lastError = CodPrest.open() ;	if (CodPrest.lastError != DBIERR_NONE)
	{
   	erreur("Erreur à l'ouverture de la base CodPrest.", standardError, CodPrest.lastError) ;
    return false ;
	}

	CodPrest.lastError = CodPrest.debut(dbiWRITELOCK) ;
	if ((CodPrest.lastError != DBIERR_NONE) && (CodPrest.lastError != DBIERR_EOF))
	{
		erreur("Erreur de positionnement dans le fichier CodPrest.db.", standardError, CodPrest.lastError) ;
    CodPrest.close() ;
    return false ;
	}

	while (CodPrest.lastError != DBIERR_EOF)
	{
  	CodPrest.lastError = CodPrest.getRecord() ;
    if (CodPrest.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche CodPrest.", standardError, CodPrest.lastError) ;
      CodPrest.close() ;
      return false ;
    }

    // on remplit le tableau et la combobox
    if (CodPrest.getFse16xx() == string("1610"))
    {
    	pCodeArray->push_back(new NSCodPrestInfo(&CodPrest)) ;
      pCode->AddString(CodPrest.getCode().c_str()) ;
      nbCode++ ;
    }

    // ... on passe au composant suivant
    CodPrest.lastError = CodPrest.suivant(dbiWRITELOCK) ;
    if ((CodPrest.lastError != DBIERR_NONE) && (CodPrest.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche CodPrest.", standardError, CodPrest.lastError) ;
      CodPrest.close() ;
      return false ;
    }
	} // fin du while (recherche des composants images)

	// on ferme la base CARTE_SV2
	CodPrest.lastError = CodPrest.close() ;
	if (CodPrest.lastError != DBIERR_NONE)
	{
  	erreur("Erreur de fermeture du fichier CodPrest.", standardError, CodPrest.lastError) ;
    return false ;
	}

	return true ;
}

voidCreerFse1610Dialog::CalculMontant()
{
  // calcul du diviseur
  //
  string sDivis ;
  pDivis->GetText(sDivis) ;
  int divis = atoi(sDivis.c_str()) ;
  if (0 == divis)
  {
    erreur("Attention division par zéro !", standardError, 0) ;
    return ;
  }

  // valeur du K
  //
  int indexCode = pCode->GetSelIndex() ;
  int	valk      = 0 ;
  if (indexCode >= 0)
  {
    if (MONNAIE_LOCALE == pVar->getMonnaieRef())
      valk = atoi((*pCodeArray)[indexCode]->getPriceF().c_str()) ;
    else
      valk = atoi((*pCodeArray)[indexCode]->getPriceE().c_str()) ;
  }

  // coefficient
  //
  string sCoeff ;
  pCoeff->getSomme(&sCoeff) ;
  double coeff = atof(sCoeff.c_str()) / (divis * 100) ;

  string sQte ;
  pQuantite->GetText(sQte) ;
  int qte = atoi(sQte.c_str()) ;

  int montant = valk * coeff * qte ;

  char cMontant[33] ;
	itoa(montant, cMontant, 10) ;

  pMontant->setSomme(string(cMontant)) ;
}

voidCreerFse1610Dialog::CmOk()
{
  string sDate ;
  pDateExe->getDate(&sDate) ;

  string sHeure ;
  pHeureExe->getHeure(&sHeure) ;

  if (sDate > pVar->getDateC())
    erreur("La date de prestation est supérieure à celle de l'examen", standardError, 0) ;
  else if (sDate == pVar->getDateC())
  {
    if (sHeure > pVar->getHeureC())
      erreur("L'heure de prestation est supérieure à celle de l'examen", standardError, 0) ;
  }

  string sDateHeure = sDate + sHeure ;
  strcpy(pData->date, sDateHeure.c_str()) ;

  int indexLieu = pLieuExe->GetSelIndex() ;
  if (indexLieu >= 0)
    strcpy(pData->lieu, ((*pLieuArray)[indexLieu]->sCode).c_str()) ;

  int indexQual = pQualif->GetSelIndex() ;
  if (indexQual >= 0)
    strcpy(pData->qualif, ((*pQualifArray)[indexQual]->sCode).c_str()) ;

  int indexCode = pCode->GetSelIndex() ;
  if (indexCode >= 0)
  {
    strcpy(pData->code,        (*pCodeArray)[indexCode]->getCode().c_str()) ;
    strcpy(pData->prix_unit_f, (*pCodeArray)[indexCode]->getPriceF().c_str()) ;
    strcpy(pData->prix_unit_e, (*pCodeArray)[indexCode]->getPriceE().c_str()) ;
  }

  string sTexte ;
  pDivis->GetText(sTexte) ;
  if (atoi(sTexte.c_str()) == 0)
  {
    erreur("Attention division par zéro !", standardError, 0) ;
    return ;
  }
  strcpy(pData->divis, sTexte.c_str()) ;

  pCoeff->getSomme(&sTexte) ;
  // le coeff est exprimé en centièmes
  int coeff = atoi(sTexte.c_str()) / atoi(pData->divis) ;
  itoa(coeff, pData->coeff, 10) ;

  pQuantite->GetText(sTexte) ;
  strcpy(pData->quantite, sTexte.c_str()) ;

  int indexCompl = pCompl->GetSelIndex() ;
  if (indexCompl >= 0)
    strcpy(pData->complmnt, ((*pComplArray)[indexCompl]->sCode).c_str()) ;

  pDenombr->GetText(sTexte);
  strcpy(pData->denombr, sTexte.c_str()) ;

  pMontant->getSomme(&sTexte) ;

  // conversions du montant
  //
  if (MONNAIE_LOCALE == pVar->getMonnaieRef())
  {
    strcpy(pData->montant_f, sTexte.c_str()) ;
    int iF = atoi(pData->montant_f) ;
    int iE = pVar->getEuroFromLocal(iF) ;
    itoa(iE, pData->montant_e, 10) ;
  }
  else
  {
    strcpy(pData->montant_e, sTexte.c_str()) ;
    int iE = atoi(pData->montant_e) ;
    int iF = pVar->getLocalFromEuro(iE) ;
    itoa(iF, pData->montant_f, 10) ;
  }

  TDialog::CmOk() ;
}

void
CreerFse1610Dialog::CmCancel()
{
	TDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de InitFse1610Dialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(InitFse1610Dialog, NSUtilDialog)
	// Recalcul du montant sur les killfocus des controles agissants
  EV_CBN_KILLFOCUS(IDC_1610_INIT_CODE, CalculMontant),
  EV_EN_KILLFOCUS(IDC_1610_INIT_COEFF, CalculMontant),
  EV_EN_KILLFOCUS(IDC_1610_INIT_DIVIS, CalculMontant),

  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//
InitFse1610Dialog::InitFse1610Dialog(TWindow* pere, NSContexte* pCtx, string sCodeExam)
                  :NSUtilDialog(pere, pCtx, "IDD_1610_INIT", pNSResModule)
{
	// Initialisation des donnees
  pData 			= new NSFse1610Data ;
  pVar        = new NSVarCompta(pContexte) ;
  sCodeExamen = sCodeExam ;

  // Création de tous les "objets de contrôle"
  pDateExe		= new NSUtilEditDate(pContexte, this, IDC_1610_INIT_DATE) ;
  pHeureExe		= new NSUtilEditHeure(pContexte, this, IDC_1610_INIT_HEURE) ;
  pExamen			= new NSUtilLexique(pContexte, this, IDC_1610_INIT_EXAMEN, pContexte->getDico()) ;
  pCode       = new TComboBox(this, IDC_1610_INIT_CODE) ;
  pCoeff			= new NSUtilEditSomme(pContexte, this, IDC_1610_INIT_COEFF, FSE1610_COEFF_LEN + 1) ;
  pDivis			= new NSUtilEdit2(pContexte, this, IDC_1610_INIT_DIVIS, FSE1610_DIVIS_LEN) ;
  pMontant    = new NSUtilEditSomme(pContexte, this, IDC_1610_INIT_MONTANT) ;
  pSigle			= new TStatic(this, IDC_1610_INIT_SIGLE) ;

  pCodeArray  = new NSCodPrestArray ;
  nbCode = 0 ;
}

//
// Destructeur
//
InitFse1610Dialog::~InitFse1610Dialog()
{
  delete pData ;
  delete pVar ;
  delete pDateExe ;
  delete pHeureExe ;
  delete pExamen ;
  delete pCode ;
  delete pCoeff ;
  delete pDivis ;
  delete pMontant ;
  delete pSigle ;
  delete pCodeArray ;
}

//// Fonction SetupWindow : Mise en place des data à l'écran et initialisation
//	du tableau des Fse16xx + liste
//
void
InitFse1610Dialog::SetupWindow()
{
  int            j ;
  NSCodPrestIter k ;
  char	 dateExe[9]  = "" ;
  char	 heureExe[5] = "" ;
  string sKCode = string("") ;

	TDialog::SetupWindow() ;

  string sMonnaieSigle = pVar->getMonnaieSigle() ;
  pSigle->SetText(sMonnaieSigle.c_str()) ;
  if (!strcmp(pData->date, ""))  {
    pDateExe->setDate(pVar->getDateC()) ;    pHeureExe->setHeure(pVar->getHeureC()) ;
  }
  else // si la date n'est pas vide, on l'affiche
  {    strncpy(dateExe, pData->date, 8);
    strcpy(heureExe, &((pData->date)[8]));

    pDateExe->setDate(string(dateExe));
    pHeureExe->setHeure(string(heureExe));

    // on remet à jour DateC et HeureC dans ce cas
    // car ce dialogue est lancé avant le dialogue Compt
    pVar->setDateC(string(dateExe)) ;
    pVar->setHeureC(string(heureExe)) ;
  }

  if (string("") != sCodeExamen)
  {
    pExamen->setLabel(sCodeExamen) ;

    if (string(sCodeExamen,0,5) == "GCONS")    {      sKCode = pVar->getIndiceConsult() ;
      strcpy(pData->coeff, "100") ;
    }
    else
      sKCode = "K" ;
  }

  // on charge la table des codes prest  if (!InitCodeArray())
  {
    erreur("Probleme au chargement du tableau des codes prestation.", standardError, 0) ;
    return ;
  }

  // sélection du code "K" ou "C" dans la combobox  for (k = pCodeArray->begin(), j = 0 ; pCodeArray->end() != k ; k++, j++)
  {
    if ((*k)->getCode() == sKCode)
    {
      pCode->SetSelIndex(j) ;
      break;
    }
  }

  // pour éviter la division par zéro  if (atoi(pData->divis) == 0)
  {
    strcpy(pData->divis, "1");
    pDivis->SetText(pData->divis);
  }
  else
    pDivis->SetText(pData->divis);

  char coeff[255];
  itoa(atoi(pData->coeff) * atoi(pData->divis), coeff, 10);
  pCoeff->setSomme(string(coeff));

  // Quantité et dénombrement : valeurs par défaut
  if (!strcmp(pData->quantite, ""))
    strcpy(pData->quantite, "1");

  if (!strcmp(pData->denombr, ""))
    strcpy(pData->denombr, "1");

  // unité de référence
  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    pMontant->setSomme(string(pData->montant_f));  else
    pMontant->setSomme(string(pData->montant_e));

  CalculMontant();
  // on met le focus sur coefficient  pCoeff->SetFocus();
}

//// 	InitCodeArray() : Charge les codes prest en mémoire
//
bool
InitFse1610Dialog::InitCodeArray()
{
  NSCodPrest CodPrest(pContexte->getSuperviseur()) ;

  CodPrest.lastError = CodPrest.open() ;
  if (CodPrest.lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ouverture de la base CodPrest.", standardError, 0) ;
    return false;
  }

  CodPrest.lastError = CodPrest.debut(dbiWRITELOCK) ;
  if ((CodPrest.lastError != DBIERR_NONE) && (CodPrest.lastError != DBIERR_EOF))
  {
    erreur("Erreur de positionnement dans le fichier CodPrest.db.", standardError, CodPrest.lastError) ;
    CodPrest.close() ;
    return false ;
  }

  while (CodPrest.lastError != DBIERR_EOF)
  {
    CodPrest.lastError = CodPrest.getRecord() ;
    if (CodPrest.lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture d'une fiche CodPrest.", standardError, CodPrest.lastError) ;
      CodPrest.close() ;
      return false ;
    }

    // on remplit le tableau et la combobox
    if (CodPrest.getFse16xx() == string("1610"))    {
      pCodeArray->push_back(new NSCodPrestInfo(&CodPrest)) ;
      pCode->AddString(CodPrest.getCode().c_str()) ;
      nbCode++ ;
    }

    // ... on passe au composant suivant
    CodPrest.lastError = CodPrest.suivant(dbiWRITELOCK) ;
    if ((CodPrest.lastError != DBIERR_NONE) && (CodPrest.lastError != DBIERR_EOF))
    {
      erreur("Erreur d'acces à une fiche CodPrest.", standardError, CodPrest.lastError) ;
      CodPrest.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base CARTE_SV2  CodPrest.lastError = CodPrest.close() ;
  if (CodPrest.lastError != DBIERR_NONE)
  {
    erreur("Erreur de fermeture du fichier CodPrest.", standardError, CodPrest.lastError) ;
    return false ;
  }

  return true ;
}

voidInitFse1610Dialog::CalculMontant()
{
  // calcul du diviseur
  //
  string sDivis ;
  pDivis->GetText(sDivis);
  int divis = atoi(sDivis.c_str());
  if (divis == 0)
  {
    erreur("Attention division par zéro !", standardError, 0) ;
    return ;
  }

  // valeur du K  //  int indexCode = pCode->GetSelIndex() ;

  int valk = 0 ;
  if (indexCode >= 0)
  {
    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
      valk = atoi((*pCodeArray)[indexCode]->getPriceF().c_str()) ;
    else
      valk = atoi((*pCodeArray)[indexCode]->getPriceE().c_str()) ;
  }

  // coefficient  //  string sCoeff ;  pCoeff->getSomme(&sCoeff) ;
  double coeff = atof(sCoeff.c_str()) / (divis * 100) ;

  int qte = atoi(pData->quantite) ;
  int  montant = valk * coeff * qte ;  char cMontant[33] ;  itoa(montant, cMontant, 10) ;
  pMontant->setSomme(string(cMontant)) ;
}

voidInitFse1610Dialog::CmOk()
{
  string sDate ;
  pDateExe->getDate(&sDate) ;
  string sHeure ;
  pHeureExe->getHeure(&sHeure) ;

  if (sDate > pVar->getDateC())
    erreur("La date de prestation est supérieure à celle de l'examen", standardError, 0) ;
  else if (pVar->getDateC() == sDate)
  {
    if (sHeure > pVar->getHeureC())
      erreur("L'heure de prestation est supérieure à celle de l'examen", standardError, 0) ;
  }

  string sDateHeure = sDate + sHeure ;
  strcpy(pData->date, sDateHeure.c_str()) ;

  int indexCode = pCode->GetSelIndex() ;
  if (indexCode >= 0)
  {
    strcpy(pData->code,        (*pCodeArray)[indexCode]->getCode().c_str()) ;
    strcpy(pData->prix_unit_f, (*pCodeArray)[indexCode]->getPriceF().c_str()) ;
    strcpy(pData->prix_unit_e, (*pCodeArray)[indexCode]->getPriceE().c_str()) ;
  }

  string sTexte ;

  pDivis->GetText(sTexte) ;
  if (atoi(sTexte.c_str()) == 0)
  {
    erreur("Attention division par zéro !", standardError, 0) ;
    return ;
  }
  strcpy(pData->divis, sTexte.c_str()) ;

  pCoeff->getSomme(&sTexte) ;
  // le coeff est exprimé en centièmes
  int coeff = atoi(sTexte.c_str()) / atoi(pData->divis) ;
  itoa(coeff, pData->coeff, 10) ;
  pMontant->getSomme(&sTexte) ;

  // conversions du montant
  //
  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
  {    strcpy(pData->montant_f, sTexte.c_str()) ;
    int iF = atoi(pData->montant_f) ;
    int iE = pVar->getEuroFromLocal(iF) ;
    itoa(iE, pData->montant_e, 10) ;  }
  else
  {
    strcpy(pData->montant_e, sTexte.c_str()) ;    int iE = atoi(pData->montant_e) ;
    int iF = pVar->getLocalFromEuro(iE) ;
    itoa(iF, pData->montant_f, 10) ;
  }

  TDialog::CmOk() ;
}

voidInitFse1610Dialog::CmCancel()
{
	TDialog::CmCancel();
}

// -----------------------------------------------------------------//
//  Méthodes de CreerFse1620Dialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CreerFse1620Dialog, NSUtilDialog)
  // Recalcul du montant sur les killfocus des controles agissants
	EV_CBN_KILLFOCUS(IDC_1620_CODEIFD, CalculMontantIfd),
  EV_EN_KILLFOCUS(IDC_1620_QUANTITE, CalculMontantIfd),

  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//// Constructeur
//
CreerFse1620Dialog::CreerFse1620Dialog(TWindow* pere, NSContexte* pCtx, NSVarCompta* pVarCompt)
                   :NSUtilDialog(pere, pCtx, "IDD_1620", pNSResModule)
{
  // Initialisation des donnees
  pData     = new NSFse1620Data ;
  pVar      = new NSVarCompta(*pVarCompt) ;

  // Création de tous les "objets de contrôle"
  pDate     = new NSUtilEditDate(pContexte, this, IDC_1620_DATE) ;
  pHeure    = new NSUtilEditHeure(pContexte, this, IDC_1620_HEURE) ;
  pCodeIfd  = new TComboBox(this, IDC_1620_CODEIFD) ;
  pQuantite = new NSUtilEdit2(pContexte, this, IDC_1620_QUANTITE, FSE1620_QUANTITE_LEN) ;
  pMontant  = new NSUtilEditSomme(pContexte, this, IDC_1620_MONTANT) ;
  pSigle    = new TStatic(this, IDC_1620_SIGLE) ;

  pCodeIfdArray	= new NSCodPrestArray ;
  nbCode = 0 ;
}

//// Destructeur
//
CreerFse1620Dialog::~CreerFse1620Dialog()
{
	delete pData ;
  delete pDate ;
  delete pHeure ;
  delete pCodeIfd ;
  delete pQuantite ;
  delete pMontant ;
  delete pSigle ;
  delete pCodeIfdArray ;
}

//// Fonction SetupWindow : Mise en place des data à l'écran et initialisation
//	du tableau des Fse16xx + liste
//
void
CreerFse1620Dialog::SetupWindow()
{
   NSCodPrestIter k;
   int				j;
   char				dateExe[9] = "";
   char				heureExe[5] = "";

	TDialog::SetupWindow();

  string sMonnaieSigle = pVar->getMonnaieSigle() ;
  pSigle->SetText(sMonnaieSigle.c_str()) ;

  if (!strcmp(pData->date_depl, ""))
  {
    pDate->setDate(pVar->getDateC()) ;
   	pHeure->setHeure(pVar->getHeureC()) ;
  }
  else // si la date n'est pas vide, on l'affiche
  {
    strncpy(dateExe, pData->date_depl, 8);
    strcpy(heureExe, &((pData->date_depl)[8]));

    pDate->setDate(string(dateExe));
    pHeure->setHeure(string(heureExe));
  }

  // on charge la table des codes prest
	if (!InitCodeArray())
  {
    erreur("Probleme au chargement du tableau des codes prestation.",standardError,0);
    return;
  }

  // sélection du code dans la combobox
  for (k = pCodeIfdArray->begin(), j = 0 ; pCodeIfdArray->end() != k ; k++, j++)
  {
    if ((*k)->getCode() == string(pData->code_ifd))
    {
      pCodeIfd->SetSelIndex(j);
      break;
    }
  }

  if (!strcmp(pData->quantite, ""))
    pQuantite->SetText("1");
  else
    pQuantite->SetText(pData->quantite);

  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    pMontant->setSomme(string(pData->montant_ifd_f)) ;
  else
    pMontant->setSomme(string(pData->montant_ifd_e)) ;
}

//
// 	InitCodeArray() : Charge les codes prest en mémoire
//
bool
CreerFse1620Dialog::InitCodeArray()
{
  NSCodPrest CodPrest(pContexte->getSuperviseur()) ;

  CodPrest.lastError = CodPrest.open() ;
  if (DBIERR_NONE != CodPrest.lastError)
  {
    erreur("Erreur à l'ouverture de la base CodPrest.", standardError, CodPrest.lastError) ;
    return false ;
  }

  CodPrest.lastError = CodPrest.debut(dbiWRITELOCK) ;
  if ((DBIERR_NONE != CodPrest.lastError) && (DBIERR_EOF != CodPrest.lastError))
  {
    erreur("Erreur de positionnement dans le fichier CodPrest.db.", standardError, CodPrest.lastError) ;
    CodPrest.close() ;
    return false ;
  }

  while (DBIERR_EOF != CodPrest.lastError)
  {
    CodPrest.lastError = CodPrest.getRecord() ;
    if (DBIERR_NONE != CodPrest.lastError)
    {
      erreur("Erreur à la lecture d'une fiche CodPrest.", standardError, CodPrest.lastError) ;
      CodPrest.close() ;
      return false ;
    }

    // on remplit le tableau et la combobox
    if (CodPrest.getFse16xx() == string("1620"))
    {
      pCodeIfdArray->push_back(new NSCodPrestInfo(&CodPrest)) ;
      pCodeIfd->AddString(CodPrest.getCode().c_str()) ;
      nbCode++ ;
    }

    // ... on passe au composant suivant
    CodPrest.lastError = CodPrest.suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != CodPrest.lastError) && (DBIERR_EOF != CodPrest.lastError))
    {
      erreur("Erreur d'acces à une fiche CodPrest.", standardError, CodPrest.lastError) ;
      CodPrest.close() ;
      return false ;
    }
  } // fin du while 

  CodPrest.lastError = CodPrest.close() ;
  if (DBIERR_NONE != CodPrest.lastError)
  {
    erreur("Erreur de fermeture du fichier CodPrest.", standardError, CodPrest.lastError) ;
    return false ;
  }

  return true ;
}

voidCreerFse1620Dialog::CalculMontantIfd()
{
  int valid = 0 ;

  int indexCode = pCodeIfd->GetSelIndex() ;
  if (indexCode >= 0)
  {
    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
      valid = atoi((*pCodeIfdArray)[indexCode]->getPriceF().c_str()) ;
    else
      valid = atoi((*pCodeIfdArray)[indexCode]->getPriceE().c_str()) ;
  }

  string sQte ;
  pQuantite->GetText(sQte);
  int qte = atoi(sQte.c_str()) ;

  char cMontant[33] ;
	itoa(valid * qte, cMontant, 10) ;

  pMontant->setSomme(string(cMontant)) ;
}

voidCreerFse1620Dialog::CmOk()
{
  string sDate ;
	pDate->getDate(&sDate) ;
  string sHeure ;
  pHeure->getHeure(&sHeure) ;

  if (sDate > pVar->getDateC())
    erreur("La date de prestation est supérieure à celle de l'examen", standardError, 0) ;
  else if (sDate == pVar->getDateC())
  {
    if (sHeure > pVar->getHeureC())
      erreur("L'heure de prestation est supérieure à celle de l'examen",standardError,0);
  }

  string sDateHeure = sDate + sHeure ;
  strcpy(pData->date_depl, sDateHeure.c_str()) ;

  int indexCode = pCodeIfd->GetSelIndex() ;
  if (indexCode >= 0)
  {
    strcpy(pData->code_ifd,    (*pCodeIfdArray)[indexCode]->getCode().c_str()) ;
    strcpy(pData->prix_unit_f, (*pCodeIfdArray)[indexCode]->getPriceF().c_str()) ;
    strcpy(pData->prix_unit_e, (*pCodeIfdArray)[indexCode]->getPriceE().c_str()) ;
  }

  string sTexte ;
  pQuantite->GetText(sTexte) ;
  strcpy(pData->quantite, sTexte.c_str()) ;

  pMontant->getSomme(&sTexte) ;

  // conversions du montant
  //
  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
  {
    strcpy(pData->montant_ifd_f, sTexte.c_str()) ;
    int iF = atoi(pData->montant_ifd_f) ;
    int iE = pVar->getEuroFromLocal(iF) ;
    itoa(iE, pData->montant_ifd_e, 10) ;
  }
  else
  {
    strcpy(pData->montant_ifd_e, sTexte.c_str()) ;
    int iE = atoi(pData->montant_ifd_e) ;
    int iF = pVar->getLocalFromEuro(iE) ;
    itoa(iF, pData->montant_ifd_f, 10) ;
  }

  TDialog::CmOk() ;
}

voidCreerFse1620Dialog::CmCancel(){
	TDialog::CmCancel();
}

// -----------------------------------------------------------------//
//  Méthodes de CreerFse1630Dialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CreerFse1630Dialog, NSUtilDialog)
	// Recalcul du montant sur les killfocus des controles agissants
  EV_CBN_KILLFOCUS(IDC_1630_CODEIK, CalculMontantIk),
  EV_EN_KILLFOCUS(IDC_1630_NBREKM, CalculMontantIk),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//// Constructeur
//
CreerFse1630Dialog::CreerFse1630Dialog(TWindow* pere, NSContexte* pCtx, NSVarCompta* pVarCompt)
							 : NSUtilDialog(pere, pCtx, "IDD_1630", pNSResModule)
{
	// Initialisation des donnees
  pData    = new NSFse1630Data ;
  pVar     = new NSVarCompta(*pVarCompt) ;

  // Création de tous les "objets de contrôle"
  pDate    = new NSUtilEditDate(pContexte, this, IDC_1630_DATE) ;
  pHeure   = new NSUtilEditHeure(pContexte, this, IDC_1630_HEURE) ;
  pCodeIk  = new TComboBox(this, IDC_1630_CODEIK) ;
  pNbreKm  = new NSUtilEdit2(pContexte, this, IDC_1630_NBREKM, FSE1630_NBRE_KM_LEN) ;
  pMontant = new NSUtilEditSomme(pContexte, this, IDC_1630_MONTANT) ;
  pSigle   = new TStatic(this, IDC_1630_SIGLE) ;

  pCodeIkArray = new NSCodPrestArray ;
  nbCode = 0 ;
}

//// Destructeur
//
CreerFse1630Dialog::~CreerFse1630Dialog()
{
  delete pData ;
  delete pDate ;
  delete pHeure ;
  delete pCodeIk ;
  delete pNbreKm ;
  delete pMontant ;
  delete pSigle ;
  delete pCodeIkArray ;
}

//
// Fonction SetupWindow : Mise en place des data à l'écran et initialisation
//	du tableau des Fse16xx + liste
//
void
CreerFse1630Dialog::SetupWindow()
{
  NSCodPrestIter k;
  int				j;
  char				dateExe[9] = "";
  char				heureExe[5] = "";

	TDialog::SetupWindow() ;

  string sMonnaieSigle = pVar->getMonnaieSigle() ;
  pSigle->SetText(sMonnaieSigle.c_str()) ;

  if (!strcmp(pData->date_depl, ""))
  {
    pDate->setDate(pVar->getDateC()) ;
   	pHeure->setHeure(pVar->getHeureC()) ;
  }
  else // si la date n'est pas vide, on l'affiche
  {
    strncpy(dateExe, pData->date_depl, 8) ;
    strcpy(heureExe, &((pData->date_depl)[8])) ;

    pDate->setDate(string(dateExe)) ;
    pHeure->setHeure(string(heureExe)) ;
  }

   // on charge la table des codes prest
	if (!InitCodeArray())
  {
    erreur("Probleme au chargement du tableau des codes prestation.",standardError,0);
    return;
  }

   // sélection du code dans la combobox
  for (k = pCodeIkArray->begin(), j = 0 ; pCodeIkArray->end() != k ; k++, j++)
  {
    if ((*k)->getCode() == string(pData->code_ik))
    {
      pCodeIk->SetSelIndex(j);
      break;
    }
  }

  if (!strcmp(pData->nbre_km, ""))
    pNbreKm->SetText("1");
  else
    pNbreKm->SetText(pData->nbre_km);

  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    pMontant->setSomme(string(pData->montant_ik_f));
  else
    pMontant->setSomme(string(pData->montant_ik_e));
}

//// 	InitCodeArray() : Charge les codes prest en mémoire
//
bool
CreerFse1630Dialog::InitCodeArray()
{
	NSCodPrest* 	pCodPrest = new NSCodPrest(pContexte->getSuperviseur());

   pCodPrest->lastError = pCodPrest->open();
   if (pCodPrest->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base CodPrest.",standardError,0);
      delete pCodPrest;
      return false;
   }

	pCodPrest->lastError = pCodPrest->debut(dbiWRITELOCK);
   if ((pCodPrest->lastError != DBIERR_NONE) && (pCodPrest->lastError != DBIERR_EOF))
   {
   	erreur("Erreur de positionnement dans le fichier CodPrest.db.", standardError, pCodPrest->lastError);
      pCodPrest->close();
      delete pCodPrest;
      return false;
   }

   while (pCodPrest->lastError != DBIERR_EOF)
   {
   	pCodPrest->lastError = pCodPrest->getRecord();
      if (pCodPrest->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la lecture d'une fiche CodPrest.", standardError, pCodPrest->lastError);
         pCodPrest->close();
         delete pCodPrest;
         return false;
      }

      // on remplit le tableau et la combobox
      if (pCodPrest->getFse16xx() == string("1630"))
      {
      	pCodeIkArray->push_back(new NSCodPrestInfo(pCodPrest)) ;
      	pCodeIk->AddString(pCodPrest->getCode().c_str()) ;
      	nbCode++ ;
      }

      // ... on passe au composant suivant
      pCodPrest->lastError = pCodPrest->suivant(dbiWRITELOCK);
      if ((pCodPrest->lastError != DBIERR_NONE) && (pCodPrest->lastError != DBIERR_EOF))
      {
      	erreur("Erreur d'acces à une fiche CodPrest.", standardError, pCodPrest->lastError);
         pCodPrest->close();
         delete pCodPrest;
         return false;
      }
   } // fin du while (recherche des composants images)

   // on ferme la base CARTE_SV2
   pCodPrest->lastError = pCodPrest->close();
   if (pCodPrest->lastError != DBIERR_NONE)
   {
   	erreur("Erreur de fermeture du fichier CodPrest.", standardError, pCodPrest->lastError);
      delete pCodPrest;
      return false;
   }

   delete pCodPrest;
   return true;
}

voidCreerFse1630Dialog::CalculMontantIk()
{
  int indexCode = pCodeIk->GetSelIndex();
  int valik = 0 ;
  if (indexCode >= 0)  {
    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
      valik = atoi((*pCodeIkArray)[indexCode]->getPriceF().c_str()) ;
    else
      valik = atoi((*pCodeIkArray)[indexCode]->getPriceE().c_str()) ;
  }

  string sNbreKm ;
  pNbreKm->GetText(sNbreKm) ;
  int nbreKm = atoi(sNbreKm.c_str()) ;
  char cMontant[33] ;
	itoa(valik * nbreKm, cMontant, 10);
  pMontant->setSomme(string(cMontant));}

voidCreerFse1630Dialog::CmOk()
{
  string sDate ;
	pDate->getDate(&sDate) ;
  string sHeure ;
  pHeure->getHeure(&sHeure) ;

  if (sDate > pVar->getDateC())
    erreur("La date de prestation est supérieure à celle de l'examen", standardError, 0) ;
  else if (sDate == pVar->getDateC())
  {
    if (sHeure > pVar->getHeureC())
      erreur("L'heure de prestation est supérieure à celle de l'examen", standardError, 0) ;
  }

  string sDateHeure = sDate + sHeure ;
  strcpy(pData->date_depl, sDateHeure.c_str()) ;

  int indexCode = pCodeIk->GetSelIndex() ;
  if (indexCode >= 0)
  {
    strcpy(pData->code_ik,     (*pCodeIkArray)[indexCode]->getCode().c_str()) ;
    strcpy(pData->prix_unit_f, (*pCodeIkArray)[indexCode]->getPriceF().c_str()) ;
    strcpy(pData->prix_unit_e, (*pCodeIkArray)[indexCode]->getPriceE().c_str()) ;
  }

  string sTexte ;
  pNbreKm->GetText(sTexte) ;
  strcpy(pData->nbre_km, sTexte.c_str()) ;

  pMontant->getSomme(&sTexte) ;

  // conversions du montant
  //
  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
  {
    strcpy(pData->montant_ik_f, sTexte.c_str()) ;
    int iF = atoi(pData->montant_ik_f) ;
    int iE = pVar->getEuroFromLocal(iF) ;
    itoa(iE, pData->montant_ik_e, 10) ;
  }
  else
  {
    strcpy(pData->montant_ik_e, sTexte.c_str()) ;
    int iE = atoi(pData->montant_ik_e) ;
    int iF = pVar->getLocalFromEuro(iE) ;
    itoa(iF, pData->montant_ik_f, 10) ;
  }

  TDialog::CmOk() ;
}

void
CreerFse1630Dialog::CmCancel()
{
	TDialog::CmCancel();
}

// -----------------------------------------------------------------
//
//  Méthodes de CreerFseCCAMDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(CreerFseCCAMDialog, NSUtilDialog)	// Recalcul du montant sur les killfocus des controles agissants
	// EV_CBN_KILLFOCUS(IDC_1630_CODEIK, CalculMontantIk),
	EV_EN_KILLFOCUS(IDC_CCAM_POURCENT, CmCalculMontant),
	EV_COMMAND(IDC_CCAM_CODE, CmCalculCode),
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//

CreerFseCCAMDialog::CreerFseCCAMDialog(TWindow* pere, NSContexte* pCtx, NSVarCompta* pVarCompt, string sExamen)							     :NSUtilDialog(pere, pCtx, "IDD_CCAM_INIT", pNSResModule)
{
	// Initialisation des donnees

	pData 			= new NSFseCCAMData ;	pVar				= new NSVarCompta(*pVarCompt) ;
  sExam       = sExamen ;

	// Création de tous les "objets de contrôle"
	pDate			  = new NSUtilEditDate(pContexte, this, IDC_CCAM_DATE) ;	pCode       = new TStatic(this, IDC_CCAM_CODE) ;
	pPourcent   = new NSUtilEdit2(pContexte, this, IDC_CCAM_POURCENT, FSECCAM_POURCENT_LEN) ;
	pModifs     = new NSUtilEdit2(pContexte, this, IDC_CCAM_MODIFS, FSECCAM_MODIFICATEURS_LEN) ;
	pMontant		= new NSUtilEditSomme(pContexte, this, IDC_CCAM_MONTANT) ;	pSigle			= new TStatic(this, IDC_CCAM_SIGLE) ;
	pCcamSelect = new NSCcamInfo() ;
}

//// Destructeur
//

CreerFseCCAMDialog::~CreerFseCCAMDialog(){
	delete pData ;
	delete pDate ;
	delete pCode ;	delete pPourcent ;	delete pModifs ;	delete pMontant ;
	delete pSigle ;
	delete pCcamSelect ;
}
void
CreerFseCCAMDialog::SetupWindow()
{
	char dateExe[9] = "" ;

	NSUtilDialog::SetupWindow() ;
	//if (pVar->monnaieRef == MONNAIE_LOCALE)	//    pSigle->SetText((pVar->sigle).c_str());
	//else
	pSigle->SetText("Euros") ;

	if (!strcmp(pData->date, ""))		pDate->setDate(pVar->getDateC()) ;
	else // si la date n'est pas vide, on l'affiche
	{
		strcpy(dateExe, pData->date) ;
		pDate->setDate(string(dateExe)) ;
	}

	if (strcmp(pData->code, "") != 0)
	{
		string sLibelle ;
		RechercheLibelle(pData->code, &sLibelle) ;
		pCode->SetText(sLibelle.c_str()) ;
	}

	if (!strcmp(pData->pourcent, ""))
		pPourcent->SetText("") ;
	else
		pPourcent->SetText(pData->pourcent) ;

	if (!strcmp(pData->modificateurs, ""))
		pModifs->SetText("") ;
	else
		pModifs->SetText(pData->modificateurs) ;

	if (strcmp(pData->montant_e, "") != 0)
		pMontant->setSomme(string(pData->montant_e)) ;
}

void
CreerFseCCAMDialog::RechercheLibelle(string sCode, string* pLibelle)
{
	NSCcam Ccam(pContexte->getSuperviseur()) ;

	Ccam.lastError = Ccam.open() ;
	if (Ccam.lastError != DBIERR_NONE)	{
		erreur("Erreur à l'ouverture du fichier Ccam.db", standardError, Ccam.lastError, GetHandle()) ;
		return ;
	}

	Ccam.lastError = Ccam.chercheClef(&sCode,                                    "",
                                    0,
                                    keySEARCHEQ,
                                    dbiWRITELOCK) ;

	if (Ccam.lastError != DBIERR_NONE)	{
		erreur("Erreur à la recherche du code Ccam.", standardError, Ccam.lastError, GetHandle()) ;
    Ccam.close() ;
		return ;
	}

	Ccam.lastError = Ccam.getRecord() ;
	if (Ccam.lastError != DBIERR_NONE)	{
		erreur("Erreur à lecture du fichier Ccam.db", standardError, Ccam.lastError, GetHandle()) ;
		Ccam.close() ;
		return ;
	}

	// On récupère le libellé	*pLibelle = Ccam.getCode() + string(" : ") + Ccam.getLibelle() ;
	*pCcamSelect = NSCcamInfo(&Ccam) ;

	Ccam.lastError = Ccam.close() ;	if (Ccam.lastError != DBIERR_NONE)
		erreur("Erreur à la fermeture du fichier Ccam.db", standardError, Ccam.lastError, GetHandle()) ;
}

voidCreerFseCCAMDialog::CmCalculCode(){	//	// Lancement de la boîte de dialogue de choix du code CDAM
	//
	if (ChoixCcamDialog(this, pContexte, &sExam, pCcamSelect).Execute() == IDOK)
	{
		string sLibelle = pCcamSelect->getCode() + string(" : ") + pCcamSelect->getLibelle() ;
		pCode->SetText(sLibelle.c_str()) ;
		CmCalculMontant() ;	}}voidCreerFseCCAMDialog::CmCalculMontant(){	if (string("") == pCcamSelect->getCode())	{		pMontant->setSomme(string("000")) ;		return ;	}	string sMontant, sPourcent ;	double fMontant, fPourcent ;	sMontant = pCcamSelect->getPrix() ;	pPourcent->GetText(sPourcent) ;	// cas sPourcent == "" ==> 100%	if (sPourcent != "")	{		fMontant = atof(sMontant.c_str()) ;		fPourcent = atof(sPourcent.c_str()) ;		fMontant = (fMontant * (fPourcent / 100)) ;		sMontant = DoubleToString(&fMontant, -1, 0) ;	}	pMontant->setSomme(sMontant) ;}voidCreerFseCCAMDialog::CmOk(){	if (string("") == pCcamSelect->getCode())	{
		erreur("Aucun code CCAM n'a été sélectionné", standardError, 0) ;
		return ;
	}

  string sDate ;

	pDate->getDate(&sDate);
	if (sDate > pVar->getDateC())	{		erreur("La date de prestation est supérieure à celle de l'examen", standardError, 0) ;
		return ;
	}

	strcpy(pData->date, sDate.c_str()) ;	strcpy(pData->code, pCcamSelect->getCode().c_str()) ;

  string sTexte ;

	pPourcent->GetText(sTexte) ;	strcpy(pData->pourcent, sTexte.c_str()) ;

	pModifs->GetText(sTexte) ;
	strcpy(pData->modificateurs, sTexte.c_str()) ;
	pMontant->getSomme(&sTexte) ;	strcpy(pData->montant_e, sTexte.c_str()) ;

	NSUtilDialog::CmOk() ;}
voidCreerFseCCAMDialog::CmCancel(){	NSUtilDialog::CmCancel() ;}
// -----------------------------------------------------------------
//
//  Méthodes de CreerFseLibreDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(CreerFseLibreDialog, NSUtilDialog)	// Recalcul du montant sur les killfocus des controles agissants
	// EV_CBN_KILLFOCUS(IDC_1630_CODEIK, CalculMontantIk),
	EV_EN_KILLFOCUS(IDC_CCAM_POURCENT, CmCalculMontant),
	EV_COMMAND(IDC_CCAM_CODE, CmCalculCode),
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//
CreerFseLibreDialog::CreerFseLibreDialog(TWindow* pere, NSContexte* pCtx, NSVarCompta* pVarCompt, string sExamen)
							      :NSUtilDialog(pere, pCtx, "IDD_CLAM_INIT", pNSResModule)
{
	// Initialisation des donnees

	pData 			= new NSFseLibreData ;	pVar				= new NSVarCompta(*pVarCompt) ;
  sExam       = sExamen ;

	// Création de tous les "objets de contrôle"
	pDate			   = new NSUtilEditDate(pContexte, this, IDC_LIBRE_DATE) ;	pCode        = new TStatic(this, IDC_LIBRE_CODE) ;
	pPourcent    = new NSUtilEdit2(pContexte, this, IDC_LIBRE_POURCENT, FSECCAM_POURCENT_LEN) ;
	pMontantE		 = new NSUtilEditSomme(pContexte, this, IDC_LIBRE_MONTANT_E) ;
  pMontantF		 = new NSUtilEditSomme(pContexte, this, IDC_LIBRE_MONTANT_F) ;
	pSigleE			 = new TStatic(this, IDC_LIBRE_SIGLE_E) ;
  pSigleF			 = new TStatic(this, IDC_LIBRE_SIGLE_F) ;
	pLibreSelect = new NSLibreInfo(pContexte) ;
}

//// Destructeur
//
CreerFseLibreDialog::~CreerFseLibreDialog()
{
	delete pData ;
	delete pDate ;
	delete pCode ;	delete pPourcent ;	delete pMontantE ;  delete pMontantF ;	delete pSigleE ;
  delete pSigleF ;
	delete pLibreSelect ;
}
void
CreerFseLibreDialog::SetupWindow()
{
	char dateExe[9] = "" ;

	NSUtilDialog::SetupWindow() ;
	//if (pVar->monnaieRef == MONNAIE_LOCALE)	//    pSigle->SetText((pVar->sigle).c_str());
	//else
	pSigleE->SetText("€") ;
  string sSigle = pContexte->getSuperviseur()->getSigle() ;
  pSigleF->SetText((char*) sSigle.c_str()) ;

	if (!strcmp(pData->date, ""))		pDate->setDate(pVar->getDateC()) ;
	else // si la date n'est pas vide, on l'affiche
	{
		strcpy(dateExe, pData->date) ;
		pDate->setDate(string(dateExe)) ;
	}

	if (strcmp(pData->code, "") != 0)
	{
		string sLibelle ;
		RechercheLibelle(pData->code, &sLibelle) ;
		pCode->SetText(sLibelle.c_str()) ;
	}

	if (!strcmp(pData->pourcent, ""))
		pPourcent->SetText("") ;
	else
		pPourcent->SetText(pData->pourcent) ;

	if (strcmp(pData->montant_e, "") != 0)
		pMontantE->setSomme(string(pData->montant_e)) ;
  if (strcmp(pData->montant_f, "") != 0)
		pMontantF->setSomme(string(pData->montant_f)) ;
}

void
CreerFseLibreDialog::RechercheLibelle(string sCode, string* pLibelle)
{
	NSClam Clam(pContexte->getSuperviseur()) ;

	Clam.lastError = Clam.open() ;
	if (Clam.lastError != DBIERR_NONE)	{
		erreur("Erreur à l'ouverture du fichier Clam.db", standardError, Clam.lastError, GetHandle()) ;
		return ;
	}

	Clam.lastError = Clam.chercheClef(&sCode,                                    "",
                                    0,
                                    keySEARCHEQ,
                                    dbiWRITELOCK) ;

	if (Clam.lastError != DBIERR_NONE)	{
		erreur("Erreur à la recherche du code Clam.", standardError, Clam.lastError, GetHandle()) ;
    Clam.close() ;
		return ;
	}

	Clam.lastError = Clam.getRecord() ;
	if (Clam.lastError != DBIERR_NONE)	{
		erreur("Erreur à lecture du fichier Clam.db", standardError, Clam.lastError, GetHandle()) ;
		Clam.close() ;
		return ;
	}

	// On récupère le libellé	*pLibelle     = Clam.getCode() + string(" : ") + Clam.getLibelle() ;
	*pLibreSelect = NSLibreInfo(&Clam, pContexte) ;

	Clam.lastError = Clam.close() ;	if (Clam.lastError != DBIERR_NONE)
		erreur("Erreur à la fermeture du fichier Clam.db", standardError, Clam.lastError, GetHandle()) ;
}

voidCreerFseLibreDialog::CmCalculCode(){	//	// Lancement de la boîte de dialogue de choix du code CDAM
	//
	if (ChoixLibreDialog(this, pContexte, &sExam, pLibreSelect).Execute() == IDOK)
	{
		string sLibelle = pLibreSelect->getCode() + string(" : ") + pLibreSelect->getLibelle() ;
		pCode->SetText(sLibelle.c_str()) ;
		CmCalculMontant() ;	}}voidCreerFseLibreDialog::CmCalculMontant(){	if (string("") == pLibreSelect->getCode())	{		pMontantE->setSomme(string("000")) ;    pMontantF->setSomme(string("000")) ;		return ;	}	string sMontantE = pLibreSelect->getPrixE() ;  string sMontantF = pLibreSelect->getPrixF() ;  string sPourcent = string("") ;	pPourcent->GetText(sPourcent) ;	// cas sPourcent == "" ==> 100%	if (string("") != sPourcent)	{    double fPourcent = atof(sPourcent.c_str()) ;		double fMontantE = atof(sMontantE.c_str()) ;		fMontantE = (fMontantE * (fPourcent / 100)) ;		sMontantE = DoubleToString(&fMontantE, -1, 0) ;    double fMontantF = atof(sMontantF.c_str()) ;    fMontantF = (fMontantF * (fPourcent / 100)) ;		sMontantF = DoubleToString(&fMontantF, -1, 0) ;	}	pMontantE->setSomme(sMontantE) ;  pMontantF->setSomme(sMontantF) ;}voidCreerFseLibreDialog::CmOk(){	if (string("") == pLibreSelect->getCode())	{
		erreur("Aucun code d'acte n'a été sélectionné", standardError, 0) ;
		return ;
	}

  string sDate ;

	pDate->getDate(&sDate);
	if (sDate > pVar->getDateC())	{		erreur("La date de prestation est supérieure à celle de l'examen", standardError, 0) ;
		return ;
	}

	strcpy(pData->date, sDate.c_str()) ;	strcpy(pData->code, pLibreSelect->getCode().c_str()) ;

  string sTexte ;

	pPourcent->GetText(sTexte) ;	strcpy(pData->pourcent, sTexte.c_str()) ;

	pMontantE->getSomme(&sTexte) ;
	strcpy(pData->montant_e, sTexte.c_str()) ;

  pMontantF->getSomme(&sTexte) ;
	strcpy(pData->montant_f, sTexte.c_str()) ;

	NSUtilDialog::CmOk() ;}
voidCreerFseLibreDialog::CmCancel(){	NSUtilDialog::CmCancel() ;}
// -----------------------------------------------------------------
//
//  Méthodes de CreerFseMatDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(CreerFseMatDialog, NSUtilDialog)  EV_LVN_GETDISPINFO(IDC_LOTS_CHOIX,   LvnGetDispInfoChoix),
  EV_LVN_COLUMNCLICK(IDC_LOTS_CHOIX,   LVNColumnclickChoix),
  EV_LVN_GETDISPINFO(IDC_LOTS_CHOISIS, LvnGetDispInfoSelected),
  EV_LVN_COLUMNCLICK(IDC_LOTS_CHOISIS, LVNColumnclickSelected),
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(ID_ELARGIR, CmElargir),
END_RESPONSE_TABLE;

//
// Constructeur
//
CreerFseMatDialog::CreerFseMatDialog(TWindow* pere, NSContexte* pCtx, NSVarCompta* pVarCompt)                  :NSUtilDialog(pere, pCtx, "IDD_MAT_INIT", pNSResModule)
{
  _bGlobalList = false ;

	// Initialisation des donnees

	_pData 			= new NSFseMatData ;	_pVar				= new NSVarCompta(*pVarCompt) ;

	// Création de tous les "objets de contrôle"
  _pChoixMatBox  = new NSLotsWindow(this, IDC_LOTS_CHOIX) ;
  _pSelectMatBox = new NSLotsWindow(this, IDC_LOTS_CHOISIS) ;

	_pMontantE	   = new NSUtilEditSomme(pContexte, this, IDC_LOTS_MONTANT_E) ;  _pMontantF	   = new NSUtilEditSomme(pContexte, this, IDC_LOTS_MONTANT_F) ;
	_pSigleE		   = new TStatic(this, IDC_LOTS_SIGLE_E) ;
  _pSigleF		   = new TStatic(this, IDC_LOTS_SIGLE_F) ;
  _pChoiceButton = new OWL::TButton(this, ID_ELARGIR) ;
	_pMatSelect    = new NSLotInfo(pContexte) ;
}

//// Destructeur
//
CreerFseMatDialog::~CreerFseMatDialog()
{
	delete _pData ;
  delete _pChoixMatBox ;
  delete _pSelectMatBox ;
	delete _pMontantE ;
  delete _pMontantF ;	delete _pSigleE ;
  delete _pSigleF ;
	delete _pMatSelect ;
  delete _pChoiceButton ;
}
void
CreerFseMatDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  if (string("") == _sExam)
  {
    uint32 styleGrayed = _pChoiceButton->TWindow::GetStyle() | WS_DISABLED ;
    _pChoiceButton->TWindow::SetStyle(styleGrayed) ;
  }

	_pSigleE->SetText("€") ;  string sSigle = pContexte->getSuperviseur()->getSigle() ;
  _pSigleF->SetText((char*) sSigle.c_str()) ;

  InitLotArray() ;
  sortLots(_pSelectMatBox) ;
  sortLots(_pChoixMatBox) ;

  InitListeSelected() ;
  AfficheListeSelected() ;

  InitListeChoix() ;
  AfficheListeChoix() ;

  UpdateSums() ;
}

void
CreerFseMatDialog::RechercheLibelle(string sCode, string* pLibelle)
{
	NSLot Lot(pContexte->getSuperviseur()) ;

	Lot.lastError = Lot.open() ;
	if (Lot.lastError != DBIERR_NONE)	{
		erreur("Erreur à l'ouverture du fichier lots.db", standardError, Lot.lastError, GetHandle()) ;
		return ;
	}

	Lot.lastError = Lot.chercheClef(&sCode,                                  "",
                                  0,
                                  keySEARCHEQ,
                                  dbiWRITELOCK) ;

	if (Lot.lastError != DBIERR_NONE)	{
		erreur("Erreur à la recherche du code de lot.", standardError, Lot.lastError, GetHandle()) ;
    Lot.close() ;
		return ;
	}

	Lot.lastError = Lot.getRecord() ;
	if (Lot.lastError != DBIERR_NONE)	{
		erreur("Erreur à lecture du fichier lots.db", standardError, Lot.lastError, GetHandle()) ;
		Lot.close() ;
		return ;
	}

	// On récupère le libellé	*pLibelle    = Lot.getLibelle() ;
	*_pMatSelect = NSLotInfo(&Lot, pContexte) ;

	Lot.lastError = Lot.close() ;	if (Lot.lastError != DBIERR_NONE)
		erreur("Erreur à la fermeture du fichier lots.db", standardError, Lot.lastError, GetHandle()) ;
}

voidCreerFseMatDialog::CmOk(){	NSUtilDialog::CmOk() ;}
voidCreerFseMatDialog::CmCancel(){	NSUtilDialog::CmCancel() ;}voidCreerFseMatDialog::CmElargir(){  _bGlobalList = !_bGlobalList ;
  InitLotArray() ;
  AfficheListeChoix() ;
}boolCreerFseMatDialog::InitLotArray()
{
  if ((false == _bGlobalList) && (string("") != _sExam) && InitLotArrayForCode())
  {
    _pChoiceButton->SetCaption("Elargir le choix") ;
    return true ;
  }

  _bGlobalList = true ;
  
  if (string("") != _sExam)
    _pChoiceButton->SetCaption("Réduire le choix") ;

  return InitGlobalLotArray() ;
}

bool
CreerFseMatDialog::InitLotArrayForCode()
{
try
{
	_aChoixMatArray.vider() ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSLot Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des lots.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.chercheClef(&_sExam,
                                    "MODULE_INDEX",
                                    NODEFAULTINDEX,
                                    keySEARCHGEQ,
                                    dbiWRITELOCK) ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Il n'existe pas de lot pour ce type d'examen.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture du 1er lot.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while ((DBIERR_NONE == Base.lastError) && (Base.getModule() == _sExam))
	{
		//
		// Ajout du matériel à l'array
		//
    if (Base.getStatusType() == NSLotData::statusOpen)
		  _aChoixMatArray.push_back(new NSLotInfo(&Base, pContexte)) ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès au lot suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture du lot.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

	return true ;
}
catch (...)
{
	erreur("Exception CreerFseMatDialog::InitLotArray.", standardError, 0) ;
  return false ;
}
}

bool
CreerFseMatDialog::InitGlobalLotArray()
{
try
{
	_aChoixMatArray.vider() ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSLot Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes LOTS.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.debut(dbiWRITELOCK) ;

  // si la table est vide debut() renvoie DBIERR_EOF
  if (DBIERR_EOF == Base.lastError)
  {
    erreur("La table des lots est vide.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
  }

  if (DBIERR_NONE != Base.lastError)
	{
		erreur("La table des lots est endommagée.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture du 1er lot.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while (DBIERR_NONE == Base.lastError)
	{
    if (Base.getStatusType() == NSLotData::statusOpen)
		  _aChoixMatArray.push_back(new NSLotInfo(&Base, pContexte)) ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès au lot suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture du lot.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

	return true ;
}
catch (...)
{
	erreur("Exception CreerFseMatDialog::InitGlobalLotArray.", standardError, 0) ;
  return false ;
}
}

void CreerFseMatDialog::AddSelectedLot(NSLotInfo *pLot)
{
  if ((NSLotInfo*) NULL == pLot)
    return ;

  _aSelectMatArray.push_back(new NSLotInfo(*pLot)) ;
}
voidCreerFseMatDialog::InitListeChoix()
{
	TListWindColumn colLibelle("Libelle", 200, TListWindColumn::Left, 0) ;
	_pChoixMatBox->InsertColumn(0, colLibelle) ;
	TListWindColumn colPrixE("Prix €", 50, TListWindColumn::Right, 1) ;
	_pChoixMatBox->InsertColumn(1, colPrixE) ;
  string sSigle = string("Prix ") + pContexte->getSuperviseur()->getSigle() ;
  TListWindColumn colPrixF((char*) sSigle.c_str(), 50, TListWindColumn::Right, 2) ;
	_pChoixMatBox->InsertColumn(2, colPrixF) ;
  TListWindColumn colReste("Reste", 50, TListWindColumn::Right, 3) ;
	_pChoixMatBox->InsertColumn(3, colReste) ;
}

void
CreerFseMatDialog::AfficheListeChoix()
{
	_pChoixMatBox->DeleteAllItems() ;

  if (_aChoixMatArray.empty())
    return ;

	for (LotsReverseIter i = _aChoixMatArray.rbegin() ; _aChoixMatArray.rend() != i ; i++)
	{
    string sLabel = (*i)->getLibelle() ;
    TListWindItem Item(sLabel.c_str(), 0) ;
    _pChoixMatBox->InsertItem(Item) ;
	}
}

void
CreerFseMatDialog::LvnGetDispInfoChoix(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  if (index < 0)
		return ;

  if ((size_t)index > _aChoixMatArray.size())
  	return ;

  string sBuffer = string("") ;

  NSLotInfo* pLotInfo = _aChoixMatArray[index] ;
  if ((NSLotInfo*) NULL == pLotInfo)
    return ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1 : // Prix Euros
    
    	sBuffer = NSBlocFse16::GetSumInUnits(pLotInfo->getPrixVenteE()) ;
      break ;

    case 2 : // Prix seconde monnaie

    	sBuffer = NSBlocFse16::GetSumInUnits(pLotInfo->getPrixVenteF()) ;
      break ;

    case 3 : // Remaining

    	sBuffer = pLotInfo->getReste() ;
      break ;
	}

  static char buffer[1024] ;
  strcpy(buffer, sBuffer.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
CreerFseMatDialog::LVNColumnclickChoix(TLwNotify& lwn)
{
  switch (lwn.iSubItem)
  {
    case 0 : sortByLabel(_pChoixMatBox) ; break ;
  }
}
voidCreerFseMatDialog::InitListeSelected()
{
	TListWindColumn colLibelle("Libelle", 200, TListWindColumn::Left, 0) ;
	_pSelectMatBox->InsertColumn(0, colLibelle) ;
	TListWindColumn colPrixE("Prix €", 50, TListWindColumn::Right, 1) ;
	_pSelectMatBox->InsertColumn(1, colPrixE) ;
  string sSigle = string("Prix ") + pContexte->getSuperviseur()->getSigle() ;
  TListWindColumn colPrixF((char*) sSigle.c_str(), 50, TListWindColumn::Right, 2) ;
	_pSelectMatBox->InsertColumn(2, colPrixF) ;
}

void
CreerFseMatDialog::AfficheListeSelected()
{
	_pSelectMatBox->DeleteAllItems() ;

  if (_aSelectMatArray.empty())
    return ;

	for (LotsReverseIter i = _aSelectMatArray.rbegin() ; _aSelectMatArray.rend() != i ; i++)
	{
    string sLabel = (*i)->getLibelle() ;
    TListWindItem Item(sLabel.c_str(), 0) ;
    _pSelectMatBox->InsertItem(Item) ;
	}
}

void
CreerFseMatDialog::LvnGetDispInfoSelected(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  if (index < 0)
		return ;

  if ((size_t)index > _aSelectMatArray.size())
  	return ;

  string sBuffer = string("") ;

  NSLotInfo* pLotInfo = _aSelectMatArray[index] ;
  if ((NSLotInfo*) NULL == pLotInfo)
    return ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1 : // Prix Euros
    
    	sBuffer = NSBlocFse16::GetSumInUnits(pLotInfo->getPrixVenteE()) ;
      break ;

    case 2 : // Prix seconde monnaie

    	sBuffer = NSBlocFse16::GetSumInUnits(pLotInfo->getPrixVenteF()) ;
      break ;
	}

  static char buffer[1024] ;
  strcpy(buffer, sBuffer.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
CreerFseMatDialog::LVNColumnclickSelected(TLwNotify& lwn)
{
  switch (lwn.iSubItem)
  {
    case 0 : sortByLabel(_pSelectMatBox) ; break ;
  }
}

void
CreerFseMatDialog::dblClickLotsWindow(NSLotsWindow* pLotWindow)
{
  // Dble click inside existing products: add it to selected
  //
  if (pLotWindow == _pChoixMatBox)
  {
    int iSelected = _pChoixMatBox->IndexItemSelect() ;
    if (iSelected >= 0)
    {
      NSLotInfo* pSelectedLot = _aChoixMatArray[iSelected] ;
      _aSelectMatArray.push_back(new NSLotInfo(*pSelectedLot)) ;
      sortLots(pLotWindow) ;
      AfficheListeSelected() ;
      UpdateSums() ;
    }
    return ;
  }
  // Dble click inside selected products: remove from selected
  //
  if (pLotWindow == _pSelectMatBox)
  {
    int iSelected = _pSelectMatBox->IndexItemSelect() ;
    if (iSelected >= 0)
    {
      LotsIter it = _aSelectMatArray.begin() ;
      for (int i = 0 ; (_aSelectMatArray.end() != it) && (i < iSelected) ; it++, i++) ;
      if (_aSelectMatArray.end() != it)
      {
        delete *it ;
        _aSelectMatArray.erase(it) ;
      }
      sortLots(pLotWindow) ;
      AfficheListeSelected() ;
      UpdateSums() ;
    }
    return ;
  }
}

void
CreerFseMatDialog::swapSortOrder(NSLotsWindow* pLotWindow, int iColumn)
{
  if ((NSLotsWindow*) NULL == pLotWindow)
    return ;

  if (pLotWindow->GetSortedColumn() == iColumn)
  {
    if (pLotWindow->IsNaturalySorted())
      pLotWindow->SetNaturalySorted(false) ;
    else
      pLotWindow->SetNaturalySorted(true) ;
  }
  else
  {
    pLotWindow->SetSortedColumn(iColumn) ;
    pLotWindow->SetNaturalySorted(true) ;
  }
}

bool
LotSortByLabelInf(NSLotInfo *pLot1, NSLotInfo *pLot2)
{
	return (pLot1->getLibelle() < pLot2->getLibelle()) ;
}

bool
LotSortByLabelSup(NSLotInfo *pLot1, NSLotInfo *pLot2)
{
	return (pLot1->getLibelle() > pLot2->getLibelle()) ;
}

void
CreerFseMatDialog::sortLots(NSLotsWindow* pLotWindow)
{
  if ((NSLotsWindow*) NULL == pLotWindow)
    return ;

  switch(pLotWindow->GetSortedColumn())
  {
    case 0 : sortByLabel(pLotWindow) ; break ;
  }
}

void
CreerFseMatDialog::sortByLabel(NSLotsWindow* pLotWindow)
{
  if ((NSLotsWindow*) NULL == pLotWindow)
    return ;

	swapSortOrder(pLotWindow, 0) ;

  NSLotArray* pArray = (NSLotArray*) 0 ;

  if      (pLotWindow == _pChoixMatBox)
    pArray = &_aChoixMatArray ;
  else if (pLotWindow == _pSelectMatBox)
    pArray = &_aSelectMatArray ;

  if (pArray->empty())
    return ;

  if (pLotWindow->IsNaturalySorted())
    sort(pArray->begin(), pArray->end(), LotSortByLabelInf) ;
  else
    sort(pArray->begin(), pArray->end(), LotSortByLabelSup) ;

  if      (pLotWindow == _pChoixMatBox)
    AfficheListeChoix() ;
  else if (pLotWindow == _pSelectMatBox)
    AfficheListeSelected() ;
}

void
CreerFseMatDialog::UpdateSums()
{
  int iSumE = 0 ;
  int iSumF = 0 ;

  if (false == _aSelectMatArray.empty())
  {
    for (LotsConstIter it = _aSelectMatArray.begin() ; _aSelectMatArray.end() != it ; it++)
    {
      iSumE += (*it)->getSellPriceE() ;
      iSumF += (*it)->getSellPriceF() ;
    }
  }

  _pMontantE->setSomme(IntToString(iSumE)) ;
  _pMontantF->setSomme(IntToString(iSumF)) ;
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------
//
//  Méthodes de NSLotsWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLotsWindow, TListWindow)
	EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSLotsWindow::NSLotsWindow(CreerFseMatDialog* pere, int resId)
             :TListWindow(pere, resId)
{
  _pDlg = pere ;

  _bNaturallySorted = true ;
  _iSortedColumn    = 0 ;
}

void
NSLotsWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSLotsWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
   	_pDlg->dblClickLotsWindow(this) ;
}

//---------------------------------------------------------------------------
//  Description: Retourne l'index du premier item sélectionné
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSLotsWindow::IndexItemSelect()
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
#endif // #ifndef _ENTERPRISE_DLL

// fin de nsf16dlg.cpp
