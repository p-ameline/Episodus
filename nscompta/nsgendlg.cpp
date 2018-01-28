// nsgendlg.cpp : dialogues de compta generale// RS Novembre 02
//////////////////////////////////////////////////////////

#include <stdio.h>#include <classlib\date.h>
#include <classlib\time.h>
#include <owl\eventhan.h>

#include "partage\nsdivfct.h"#include "nautilus\nssuper.h"
#include "partage\nsperson.h"
#include "nautilus\nspatdlg.h"	// pour le code prescript
#include "nscompta\nscompta.rh"
#include "nscompta\nsdepens.rh"
#include "nscompta\nsfsedlg.h"
#include "nscompta\nsgendlg.h"

//***************************************************************************////  							Méthodes de NSDepensDocument
//
//***************************************************************************

// Constructeur NSDepensDocument////////////////////////////////////////////////////////////////

NSDepensDocument::NSDepensDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,										NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
              :NSNoyauDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, true)
{
    // Initialisation des donnees
    pCriteres   = new NSCriteres;
	pDepensArray   = new NSAffDepensArray;
    nbDepens 	= 0;
    pTotaux		= new NSEncaissData;
	pVar 	    = new NSVarCompta(pCtx);
    bImprimer   = false;
}


NSDepensDocument::NSDepensDocument(TDocument *parent, NSContexte *pCtx)              :NSNoyauDocument(parent, pCtx)
{
    // Initialisation des donnees
    pCriteres   = new NSCriteres;
	pDepensArray   = new NSAffDepensArray;
    nbDepens 	    = 0;
    pTotaux		= new NSEncaissData;
	pVar 	    = new NSVarCompta(pCtx);
    bImprimer   = false;
}

NSDepensDocument::~NSDepensDocument(){
	delete pVar;
    delete pDepensArray;
    delete pTotaux;
    delete pCriteres;
}

// Ouverture du document////////////////////////////////////////////////////////////////

bool NSDepensDocument::Open(NSCriteres* pCritGen){
    *pCriteres = *pCritGen;

    if (!InitDepensArray())   	    return false;

    CalculeTotaux();

  	return true;}

// Fermeture du document////////////////////////////////////////////////////////////////

bool NSDepensDocument::Close(){
  	return true;
}

bool
NSDepensDocument::ChercheEcriture(string sNumEcriture, NSEcritureArray* pEcritureArray)
{
    // on récupère la cle
	string		sCleEcriture =  sNumEcriture + string("  ");

    NSEcriture* 	pEcriture = new NSEcriture(pContexte->getSuperviseur());
    pEcriture->lastError = pEcriture->open();    if (pEcriture->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base Ecriture.", standardError, 0) ;
        delete pEcriture;
        return false;
    }

    pEcriture->lastError = pEcriture->chercheClef(&sCleEcriture,                                                "",
                                                0,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK);

    if (pEcriture->lastError == DBIERR_BOF)	// cas fichier vide    {
   	    pEcriture->close();
        delete pEcriture;
        return false;		// le tableau est vide
    }

    if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))    {
   	    erreur("Erreur à la recherche d'une fiche Ecriture.", standardError, pEcriture->lastError);
        pEcriture->close();
        delete pEcriture;
        return false;
    }

    while (pEcriture->lastError != DBIERR_EOF)    {
   	    pEcriture->lastError = pEcriture->getRecord();
        if (pEcriture->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la lecture d'une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return false;
        }

        // condition d'arret        if (!(string(pEcriture->pDonnees->numero) == sNumEcriture))
            break;

        // on remplit le tableau        pEcritureArray->push_back(new NSEcritureInfo(pEcriture));

        // ... on passe au suivant        pEcriture->lastError = pEcriture->suivant(dbiWRITELOCK);
        if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))
        {
      	    erreur("Erreur d'acces à une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return false;
        }
    } // fin du while (recherche des écritures)

    // on ferme la base    pEcriture->lastError = pEcriture->close();
    if (pEcriture->lastError != DBIERR_NONE)
   	    erreur("Erreur de fermeture du fichier Ecriture.", standardError, pEcriture->lastError);

    delete pEcriture;
    return true;
}

boolNSDepensDocument::InitDepensArray()
{
  pDepensArray->vider() ;
  nbDepens = 0 ;

  NSDepens Depens(pContexte->getSuperviseur()) ;

  Depens.lastError = Depens.open() ;  if (Depens.lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ouverture de la base Depens.", standardError, Depens.lastError) ;
    return false ;
  }

  // on cherche les Depens par date  //  string sStartingDate = pCriteres->getDateAga1() ;  Depens.lastError = Depens.chercheClef(&sStartingDate,
                                        "DATE_DEPENSE",
                                        NODEFAULTINDEX,
                                        keySEARCHGEQ,
                                        dbiWRITELOCK) ;

  if (Depens.lastError == DBIERR_BOF)	// cas fichier vide  {
    Depens.close() ;
    return true;		// le tableau est vide
  }

  if ((Depens.lastError != DBIERR_NONE) && (Depens.lastError != DBIERR_EOF))  {
    erreur("Erreur à la recherche d'une fiche Depens.", standardError, Depens.lastError) ;
    Depens.close() ;
    return false ;
  }

  string sOperateur = string("") ;
  if (pCriteres->isActesPerso())    sOperateur = pContexte->getUtilisateur()->getNss() ;

  bool bCpsLibOk = true ;

  NScptsLibs cptsLibs(pContexte->getSuperviseur()) ;
  cptsLibs.lastError = cptsLibs.open() ;
  if (cptsLibs.lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ouverture de la base cptsLibs.", standardError, cptsLibs.lastError) ;
    bCpsLibOk = false ;
  }

  while (Depens.lastError != DBIERR_EOF)  {
    Depens.lastError = Depens.getRecord();
    if (Depens.lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture d'une fiche Depens.", standardError, Depens.lastError) ;
      Depens.close() ;
      if (bCpsLibOk)
        cptsLibs.close() ;
      return false ;
    }

    if (string(Depens.pDonnees->date) > pCriteres->getDateAga2())      break ;    // on remplit le tableau (éventuellement on trie sur l'operateur)    if (false == pCriteres->isActesPerso())
      sOperateur = string(Depens.pDonnees->operateur) ;

    if (sOperateur == string(Depens.pDonnees->operateur))    {
      NSAffDepensData AffDepensData ;
      NSEcritureArray EcritureArray ;

      string sNumEcriture = string("D") + string(Depens.pDonnees->numdeps) ;
      if (false == ChercheEcriture(sNumEcriture, &EcritureArray))      {        erreur("Impossible de récupérer les écritures correspondant à une dépense", standardError, 0) ;        Depens.close() ;        if (bCpsLibOk)          cptsLibs.close() ;
        return false ;
      }
      AffDepensData._depense = *(Depens.pDonnees) ;

      if (false == EcritureArray.empty())
        for (NSEcritureIter i = EcritureArray.begin(); EcritureArray.end() != i ; i++)
        {
          // on prend la première ligne d'écriture au débit
          // (en principe l'array ne contient que deux lignes d'écriture actuellement)
          if (!strcmp((*i)->pDonnees->sens, "D"))
          {
            AffDepensData._ecriture = *((*i)->pDonnees) ;

            if (bCpsLibOk)
            {
              string sCompte = (*i)->pDonnees->compte ;
              cptsLibs.lastError = cptsLibs.chercheClef(&sCompte,
                                                        "",
                                                        0,
                                                        keySEARCHEQ,
                                                        dbiWRITELOCK) ;
              if (cptsLibs.lastError == DBIERR_NONE)	// cas fichier vide
              {
                cptsLibs.lastError = cptsLibs.getRecord() ;
                if (cptsLibs.lastError != DBIERR_NONE)
                  erreur("Erreur à la lecture d'une fiche cptsLibs.", standardError, cptsLibs.lastError) ;
                else
                  AffDepensData._cpt_lib = *(cptsLibs.pDonnees) ;
              }
            }
          }
        }

      pDepensArray->push_back(new NSAffDepensData(AffDepensData)) ;
      nbDepens++ ;    }
    // ... on passe au suivant    //    Depens.lastError = Depens.suivant(dbiWRITELOCK) ;
    if ((Depens.lastError != DBIERR_NONE) && (Depens.lastError != DBIERR_EOF))
    {
      erreur("Erreur d'acces à une fiche Depens.", standardError, Depens.lastError) ;
      Depens.close() ;
      if (bCpsLibOk)
        cptsLibs.close() ;
      return false ;
    }
  } // fin du while (recherche des fiches dépenses)

  // on ferme la base DEPENSES  //  Depens.lastError = Depens.close() ;
  if (Depens.lastError != DBIERR_NONE)
    erreur("Erreur de fermeture du fichier Depens.", standardError, Depens.lastError) ;

  if (bCpsLibOk)
    cptsLibs.close() ;  return true ;
}voidNSDepensDocument::CalculeTotaux(){  pTotaux->paieLoc  = 0 ;  pTotaux->paieEuro = 0 ;
  pTotaux->espLoc   = 0 ;
  pTotaux->espEuro  = 0 ;
  pTotaux->chqLoc   = 0 ;
  pTotaux->chqEuro  = 0 ;
  pTotaux->virLoc   = 0 ;
  pTotaux->virEuro  = 0 ;
  pTotaux->cbLoc    = 0 ;
  pTotaux->cbEuro   = 0 ;

  if (pDepensArray->empty())
    return ;

	for (NSAffDepensIter i = pDepensArray->begin() ; pDepensArray->end() != i ; i++)  {
    int montant = atoi((*i)->_ecriture.somme) ;
    if (!strcmp((*i)->_ecriture.signe, "-"))
      montant = (-1) * montant ;
    int iMontantLoc = pVar->getLocalFromEuro(montant) ;    if (!strcmp((*i)->_depense.mode_paie, "E"))
    {
      pTotaux->espEuro += montant ;
      pTotaux->espLoc  += iMontantLoc ;
    }
    else if (!strcmp((*i)->_depense.mode_paie, "C"))
    {
      pTotaux->chqEuro += montant ;
      pTotaux->chqLoc  += iMontantLoc ;
    }
    else if (!strcmp((*i)->_depense.mode_paie, "V"))
    {
      pTotaux->virEuro += montant ;
      pTotaux->virLoc  += iMontantLoc ;
    }
    else if (!strcmp((*i)->_depense.mode_paie, "B"))
    {
      pTotaux->cbEuro  += montant ;
      pTotaux->cbLoc   += iMontantLoc ;
    }

    pTotaux->paieEuro += montant ;    pTotaux->paieLoc  += iMontantLoc ;
  }
}
//***************************************************************************//
//  							Méthodes de NSListDepensWindow//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSListDepensWindow, TListWindow)   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Function: NSListDepensWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------

voidNSListDepensWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		pDlg->CmModifDepens() ;
}

//---------------------------------------------------------------------------//  Function: NSListDepensWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------

intNSListDepensWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;}

//***************************************************************************//
//  Méthodes de NSListDepensDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSListDepensDialog, NSUtilDialog)    EV_LV_DISPINFO_NOTIFY(IDC_LISTDEP_LW, LVN_GETDISPINFO, DispInfoListeDepens),
    EV_COMMAND(IDC_LISTDEP_IMPRIMER, CmImprimer),
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//// Constructeur
//

NSListDepensDialog::NSListDepensDialog(TWindow* pere, NSContexte* pCtx, NSDepensDocument* pDepDoc)                :NSUtilDialog(pere, pCtx, "IDD_LISTDEP", pNSResModule)
{
    pDoc	    = pDepDoc;
    pListeDepens   = new NSListDepensWindow(this, IDC_LISTDEP_LW);
    pListeDetail = new TListBox(this, IDC_LISTDEP_LB);
    pEditTotal = new TEdit(this, IDC_LISTDEP_EDIT);
    pVar        = new NSVarCompta(pCtx);
}

//// Destructeur
//

NSListDepensDialog::~NSListDepensDialog(){
    delete pListeDepens;
    delete pListeDetail;
    delete pEditTotal;
    delete pVar;
}

//// Fonction SetupWindow
//

voidNSListDepensDialog::SetupWindow()
{
    NSUtilDialog::SetupWindow();

    InitListeDepens();    AfficheListeDepens();
    InitListesTotaux();
}

voidNSListDepensDialog::InitListeDepens()
{
	TListWindColumn colLibelle("Libelle", 150, TListWindColumn::Left, 0);
  	pListeDepens->InsertColumn(0, colLibelle);

    TListWindColumn colOper("Operateur", 150, TListWindColumn::Left, 1);  	pListeDepens->InsertColumn(1, colOper);

    TListWindColumn colMontant("Montant", 60, TListWindColumn::Right, 2);  	pListeDepens->InsertColumn(2, colMontant);

    TListWindColumn colModePaie("Mode", 30, TListWindColumn::Left, 3);  	pListeDepens->InsertColumn(3, colModePaie);
    TListWindColumn colDatePaie("Date", 80, TListWindColumn::Left, 4);    pListeDepens->InsertColumn(4, colDatePaie);}

voidNSListDepensDialog::AfficheListeDepens()
{
  pListeDepens->DeleteAllItems() ;

	for (int i = pDoc->nbDepens - 1 ; i >= 0 ; i--)  {
    TListWindItem Item(((*(pDoc->pDepensArray))[i])->_depense.libelle, 0) ;
    pListeDepens->InsertItem(Item) ;
  }
}

voidNSListDepensDialog::DispInfoListeDepens(TLwDispInfoNotify& dispInfo)
{
  const int 	BufLen = 1024 ;
  static char buffer[BufLen] ;

  int			    montant;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;
  string sDate = string("") ;

  NSAffDepensData *pAffDepensData = (*(pDoc->pDepensArray))[index] ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case 1: // operateur
    {
      string sNss = string(pAffDepensData->_depense.operateur) ;
      NSPersonInfo* pUtil = pContexte->getPersonArray()->getPerson(pContexte, sNss, pidsUtilisat) ;
      strcpy(buffer, pUtil->getCivilite().c_str()) ;

      break ;
    }
    case 2: // montant
      char szSigne[2] ;
      strcpy(szSigne, pAffDepensData->_ecriture.signe) ;
      montant = atoi(pAffDepensData->_ecriture.somme) ;      if (!strcmp(szSigne, "+"))        strcpy(szSigne, "") ;      sprintf(buffer, "%s%d,%02d ", szSigne, montant/100, montant%100) ;
      strcat(buffer, "€") ;
      break ;

    case 3: 	// mode de paiement
      sprintf(buffer, "%s", pAffDepensData->_depense.mode_paie) ;      break;
    case 4: 	// date de paiement
       sDate = donne_date(pAffDepensData->_depense.date, sLang) ;       dispInfoItem.SetText(sDate.c_str()) ;
       break ;
  } // fin du switch

  dispInfoItem.SetText(buffer) ;
}

void
NSListDepensDialog::InitListesTotaux()
{
  char buffer[255];
  int montant;

  pListeDetail->ClearList();

  montant = pDoc->pTotaux->espEuro;
  sprintf(buffer, "Espèces : %d,%02d", montant/100, montant%100);
  pListeDetail->AddString(buffer);

  montant = pDoc->pTotaux->chqEuro;
  sprintf(buffer, "Chèques : %d,%02d", montant/100, montant%100);
  pListeDetail->AddString(buffer);

  montant = pDoc->pTotaux->virEuro;
  sprintf(buffer, "Virements : %d,%02d", montant/100, montant%100);
  pListeDetail->AddString(buffer);

  montant = pDoc->pTotaux->cbEuro;
  sprintf(buffer, "C.B. : %d,%02d", montant/100, montant%100);
  pListeDetail->AddString(buffer);

  montant = pDoc->pTotaux->paieEuro;
  sprintf(buffer, "Total : %d,%02d", montant/100, montant%100);
  pEditTotal->SetText(buffer);
}

voidNSListDepensDialog::CmModifDepens()
{
  int index = pListeDepens->IndexItemSelect() ;

  if (index == -1)  {
    erreur("Vous devez sélectionner une fiche Depens.", standardError, 0) ;
    return ;
  }

  string sNumDeps = string(((*(pDoc->pDepensArray))[index])->_depense.numdeps) ;
  NSDepensDlg* pDepensesDlg = new NSDepensDlg(this, pContexte, sNumDeps) ;  pDepensesDlg->Execute() ;
  delete pDepensesDlg ;

  // on recharge le tableau depuis la base  pDoc->InitDepensArray() ;
  pDoc->CalculeTotaux() ;
  AfficheListeDepens() ;
  InitListesTotaux() ;}

voidNSListDepensDialog::CmImprimer()
{
  pDoc->bImprimer = true ;
  CmOk() ;
}

voidNSListDepensDialog::CmOk()
{
	NSUtilDialog::CmOk();
}

voidNSListDepensDialog::CmCancel()
{
	NSUtilDialog::CmCancel();
}

//***************************************************************************
//
//  Méthodes de NSAnnee2035Dialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSAnnee2035Dialog, NSUtilDialog)    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSAnnee2035Dialog::NSAnnee2035Dialog(TWindow* pere, NSContexte* pCtx)
                    :NSUtilDialog(pere, pCtx, "IDD_ANNEE2035", pNSResModule)
{
  pAnnee = new NSUtilEdit(pContexte, this, IDC_A2035_ANNEE, 5);
}

NSAnnee2035Dialog::~NSAnnee2035Dialog()
{
  delete pAnnee ;
}

void
NSAnnee2035Dialog::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;
}

void
NSAnnee2035Dialog::CmOk()
{
  sAnnee = pAnnee->GetText(5) ;
  NSUtilDialog::CmOk() ;
}

void
NSAnnee2035Dialog::CmCancel()
{
  sAnnee = "" ;
  NSUtilDialog::CmCancel() ;
}

//***************************************************************************
//
//  Méthodes de NS2035Dialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NS2035Dialog, NSUtilDialog)    EV_COMMAND(IDC_2035_FERMER, CmFermer),
    EV_COMMAND(IDC_2035_IMPRIMER, CmImprimer),
END_RESPONSE_TABLE;

NS2035Dialog::NS2035Dialog(TWindow* pere, NSContexte* pCtx, string sAnnee)
                :NSUtilDialog(pere, pCtx, "IDD_2035", pNSResModule)
{
  p2035_1     = new NSUtilEditSomme(pContexte, this, IDC_2035_1);
  p2035_2     = new NSUtilEditSomme(pContexte, this, IDC_2035_2);
  p2035_3     = new NSUtilEditSomme(pContexte, this, IDC_2035_3);
  p2035_4     = new NSUtilEditSomme(pContexte, this, IDC_2035_4);
  p2035_5     = new NSUtilEditSomme(pContexte, this, IDC_2035_5);
  p2035_6     = new NSUtilEditSomme(pContexte, this, IDC_2035_6);
  p2035_7     = new NSUtilEditSomme(pContexte, this, IDC_2035_7);
  p2035_8     = new NSUtilEditSomme(pContexte, this, IDC_2035_8);
  p2035_9     = new NSUtilEditSomme(pContexte, this, IDC_2035_9);
  p2035_10    = new NSUtilEditSomme(pContexte, this, IDC_2035_10);
  p2035_11    = new NSUtilEditSomme(pContexte, this, IDC_2035_11);
  p2035_12    = new NSUtilEditSomme(pContexte, this, IDC_2035_12);
  p2035_13    = new NSUtilEditSomme(pContexte, this, IDC_2035_13);
  p2035_14    = new NSUtilEditSomme(pContexte, this, IDC_2035_14);
  p2035_15    = new NSUtilEditSomme(pContexte, this, IDC_2035_15);
  p2035_16    = new NSUtilEditSomme(pContexte, this, IDC_2035_16);
  p2035_17    = new NSUtilEditSomme(pContexte, this, IDC_2035_17);
  p2035_18    = new NSUtilEditSomme(pContexte, this, IDC_2035_18);
  p2035_19    = new NSUtilEditSomme(pContexte, this, IDC_2035_19);
  p2035_20    = new NSUtilEditSomme(pContexte, this, IDC_2035_20);
  p2035_21    = new NSUtilEditSomme(pContexte, this, IDC_2035_21);
  p2035_22    = new NSUtilEditSomme(pContexte, this, IDC_2035_22);
  p2035_23    = new NSUtilEditSomme(pContexte, this, IDC_2035_23);
  p2035_24    = new NSUtilEditSomme(pContexte, this, IDC_2035_24);
  p2035_25    = new NSUtilEditSomme(pContexte, this, IDC_2035_25);
  p2035_26    = new NSUtilEditSomme(pContexte, this, IDC_2035_26);
  p2035_27    = new NSUtilEditSomme(pContexte, this, IDC_2035_27);
  p2035_28    = new NSUtilEditSomme(pContexte, this, IDC_2035_28);
  p2035_29    = new NSUtilEditSomme(pContexte, this, IDC_2035_29);
  p2035_30    = new NSUtilEditSomme(pContexte, this, IDC_2035_30);
  p2035_31    = new NSUtilEditSomme(pContexte, this, IDC_2035_31);
  p2035_32    = new NSUtilEditSomme(pContexte, this, IDC_2035_32);
  p2035_BH    = new NSUtilEditSomme(pContexte, this, IDC_2035_BH);
  p2035_BJ    = new NSUtilEditSomme(pContexte, this, IDC_2035_BJ);
  p2035_BM    = new NSUtilEditSomme(pContexte, this, IDC_2035_BM);

  pCptsArray  = new NSEcritureArray;
  sAnnee2035  = sAnnee;
}

NS2035Dialog::~NS2035Dialog()
{
    delete p2035_1;
    delete p2035_2;
    delete p2035_3;
    delete p2035_4;
    delete p2035_5;
    delete p2035_6;
    delete p2035_7;
    delete p2035_8;
    delete p2035_9;
    delete p2035_10;
    delete p2035_11;
    delete p2035_12;
    delete p2035_13;
    delete p2035_14;
    delete p2035_15;
    delete p2035_16;
    delete p2035_17;
    delete p2035_18;
    delete p2035_19;
    delete p2035_20;
    delete p2035_21;
    delete p2035_22;
    delete p2035_23;
    delete p2035_24;
    delete p2035_25;
    delete p2035_26;
    delete p2035_27;
    delete p2035_28;
    delete p2035_29;
    delete p2035_30;
    delete p2035_31;
    delete p2035_32;
    delete p2035_BH;
    delete p2035_BJ;
    delete p2035_BM;
    delete pCptsArray;
}

void
NS2035Dialog::SetupWindow()
{

   // fichiers d'aide

    sHindex = "";
    sHcorps = "Comptabilite.html";

    NSUtilDialog::SetupWindow();

    if (!InitCptsArray())
    {
        erreur("Erreur à l'initialisation du tableau des comptes.", standardError, 0);
        return;
    }

    if (!pCptsArray->empty())
    {
        for (NSEcritureIter k = pCptsArray->begin(); k != pCptsArray->end(); k++)
        {
            string sCpt = string((*k)->pDonnees->compte);
            int ligne = LigneCompte(sCpt);
            int signe, somme;
            char msg[255];

            if (ligne > 0)
            {
                if (!strcmp((*k)->pDonnees->signe, "+"))
                    signe = 1;
                else
                    signe = -1;

                somme = signe * atoi((*k)->pDonnees->somme);
                itoa(somme, msg, 10);

                AfficheLigne(string(msg), ligne);
            }
        }

        AfficheTotaux();
    }
    else
        erreur("Le tableau des comptes est vide.", standardError, 0);
}

bool
NS2035Dialog::InitCptsArray()
{
    string sCompte, sNum, sCode;
    int signe, total, somme;
    bool bDep;
    NSEcriture* 	pEcriture = new NSEcriture(pContexte->getSuperviseur());

    pCptsArray->vider();

    pEcriture->lastError = pEcriture->open();    if (pEcriture->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base Ecriture.", standardError, 0) ;
        delete pEcriture;
        return false;
    }

    pEcriture->lastError = pEcriture->debut(dbiWRITELOCK);    if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))    {
   	    erreur("Erreur à la recherche d'une fiche Ecriture.", standardError, pEcriture->lastError);
        pEcriture->close();
        delete pEcriture;
        return false;
    }

    while (pEcriture->lastError != DBIERR_EOF)    {
   	    pEcriture->lastError = pEcriture->getRecord();
        if (pEcriture->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la lecture d'une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return false;
        }

        sCompte = string(pEcriture->pDonnees->compte);        sNum = string(pEcriture->pDonnees->numero);        sCode = string(sNum, 1, strlen(sNum.c_str()) - 1);
        string sAnnee;

        if (sNum[0] == 'C')
        {
            sAnnee = GetAnneeRecette(sCode);
            bDep = false;
        }
        else
        {
            sAnnee = GetAnneeDepense(sCode);            bDep = true;        }        if (sAnnee == sAnnee2035)        {            NSEcritureIter i;            bool trouve = false;            total = 0;            // on récupère le total précédent            if (!pCptsArray->empty())
            {
                for (i = pCptsArray->begin(); i != pCptsArray->end(); i++)
                {
                    if (sCompte == string((*i)->pDonnees->compte))
                    {
                        // on ne tient compte que du total (signe et somme)
                        if (!strcmp((*i)->pDonnees->signe, "+"))
                            signe = 1;
                        else
                            signe = -1;

                        total = signe * atoi((*i)->pDonnees->somme);
                        trouve = true;
                        break;
                    }
                }
            }

            // on calcule le nouveau total
            if (!strcmp(pEcriture->pDonnees->signe, "+"))
                signe = 1;
            else
                signe = -1;

            somme = signe * atoi(pEcriture->pDonnees->somme);

            if (!strcmp(pEcriture->pDonnees->sens, "C"))
            {
                if (bDep)
                    total = total - somme;
                else
                    total = total + somme;
            }
            else
            {
                if (bDep)
                    total = total + somme;
                else
                    total = total - somme;
            }

            if (total >= 0)
                signe = 1;
            else
            {
                signe = -1;
                total = -1 * total;
            }

            if (trouve)
            {
                if (signe == 1)
                    strcpy((*i)->pDonnees->signe, "+");
                else
                    strcpy((*i)->pDonnees->signe, "-");

                itoa(total, (*i)->pDonnees->somme, 10);
            }
            else
            {
                NSEcritureInfo* pInfo = new NSEcritureInfo();

                strcpy(pInfo->pDonnees->compte, sCompte.c_str());

                if (signe == 1)
                    strcpy(pInfo->pDonnees->signe, "+");
                else
                    strcpy(pInfo->pDonnees->signe, "-");

                itoa(total, pInfo->pDonnees->somme, 10);

                pCptsArray->push_back(new NSEcritureInfo(*pInfo));
                delete pInfo;
            }
        }

        // ... on passe au suivant        pEcriture->lastError = pEcriture->suivant(dbiWRITELOCK);
        if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))
        {
      	    erreur("Erreur d'acces à une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return false;
        }
    } // fin du while (recherche des écritures)

    // on ferme la base    pEcriture->lastError = pEcriture->close();
    if (pEcriture->lastError != DBIERR_NONE)
   	    erreur("Erreur de fermeture du fichier Ecriture.", standardError, pEcriture->lastError);

    delete pEcriture;
    return true;
}

int
NS2035Dialog::LigneCompte(string sCompte)
{
	int numLigne;
	char szCpt[255];
	string sUtilEnCours = pContexte->getUtilisateur()->getNss() ;

	strcpy(szCpt, sCompte.c_str()) ;

	// Les comptes BANQUE ne sont pas dans la 2035
	if (!strncmp(szCpt, BANQUE_ROOT, strlen(BANQUE_ROOT)))
		return 0 ;

	NScptsLibs* 	pcptsLibs = new NScptsLibs(pContexte->getSuperviseur());
	pcptsLibs->lastError = pcptsLibs->open();	if (pcptsLibs->lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ouverture de la base cptsLibs.", standardError, pcptsLibs->lastError) ;
    delete pcptsLibs ;
    return 0 ;
	}

    pcptsLibs->lastError = pcptsLibs->chercheClef(&sCompte,                                                "",
                                                0,
                                                keySEARCHEQ,
                                                dbiWRITELOCK);

    if (pcptsLibs->lastError == DBIERR_BOF)	// cas fichier vide    {
   	    pcptsLibs->close();
        delete pcptsLibs;
        return 0;
    }

    if ((pcptsLibs->lastError != DBIERR_NONE) && (pcptsLibs->lastError != DBIERR_EOF))    {
   	    erreur("Erreur à la recherche d'une fiche cptsLibs.", standardError, pcptsLibs->lastError);
        pcptsLibs->close();
        delete pcptsLibs;
        return 0;
    }

    if (pcptsLibs->lastError != DBIERR_EOF)    {
   	    pcptsLibs->lastError = pcptsLibs->getRecord();
        if (pcptsLibs->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la lecture d'une fiche cptsLibs.", standardError, pcptsLibs->lastError);
            pcptsLibs->close();
            delete pcptsLibs;
            return 0;
        }

        string sUtil = string(pcptsLibs->pDonnees->operateur);

        if ((sUtil == "") || (sUtil == sUtilEnCours))
            numLigne = atoi(pcptsLibs->pDonnees->ligne);
        else
            numLigne = 0;
    }    else
        numLigne = 0;

    // on ferme la base    pcptsLibs->lastError = pcptsLibs->close();
    if (pcptsLibs->lastError != DBIERR_NONE)
   	    erreur("Erreur de fermeture du fichier cptsLibs.", standardError, pcptsLibs->lastError);

    delete pcptsLibs;
    return numLigne;
}

string
NS2035Dialog::GetAnneeDepense(string sNumDep)
{
    //
    // Recherche de la fiche "dépense"
    //
    NSDepens* pDepens = new NSDepens(pContexte->getSuperviseur());

    pDepens->lastError = pDepens->open();
    if (pDepens->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base NSDEPENS.", standardError, pDepens->lastError);
        delete pDepens;
        return "";
    }

    pDepens->lastError = pDepens->chercheClef(&sNumDep,                                              "",
                                              0,
                                              keySEARCHEQ,
                                              dbiWRITELOCK);

   	if (pDepens->lastError != DBIERR_NONE)   	{
   		erreur("Erreur à la recherche de la fiche depense.", standardError, pDepens->lastError);
      	pDepens->close();
        delete pDepens;
        return "";
    }

    pDepens->lastError = pDepens->getRecord();
   	if (pDepens->lastError != DBIERR_NONE)   	{
   		erreur("Erreur à lecture du fichier Depenses.db", standardError, pDepens->lastError);
      	pDepens->close();
      	delete pDepens;
      	return "";
   	}

    string sDateDep = string(pDepens->pDonnees->date);    string sAnneeDep = string(sDateDep, 0, 4);
    pDepens->close();    delete pDepens;

    return sAnneeDep;
}

string
NS2035Dialog::GetAnneeRecette(string sNumRec)
{
    //
    // Recherche de la fiche "compt"
    //
    NSCompt* pCompt = new NSCompt(pContexte->getSuperviseur());

    pCompt->lastError = pCompt->open();
    if (pCompt->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base NSCompt.", standardError, pCompt->lastError);
        delete pCompt;
        return "";
    }

    pCompt->lastError = pCompt->chercheClef(&sNumRec,                                              "",
                                              0,
                                              keySEARCHEQ,
                                              dbiWRITELOCK);

   	if (pCompt->lastError != DBIERR_NONE)   	{
   		erreur("Erreur à la recherche de la fiche Compt.", standardError, pCompt->lastError);
      	pCompt->close();
        delete pCompt;
        return "";
    }

    pCompt->lastError = pCompt->getRecord();
   	if (pCompt->lastError != DBIERR_NONE)   	{
   		erreur("Erreur à lecture du fichier Compt.db", standardError, pCompt->lastError);
      	pCompt->close();
      	delete pCompt;
      	return "";
   	}

    string sDateRec  = pCompt->_Donnees.getDate() ;    string sAnneeRec = string(sDateRec, 0, 4) ;
    pCompt->close();    delete pCompt;

    return sAnneeRec;
}

void
NS2035Dialog::AfficheLigne(string somme, int ligne)
{
    switch (ligne)
    {
        case 1 :
            p2035_1->setSomme(somme); break;
        case 2 :
            p2035_2->setSomme(somme); break;
        case 3 :
            p2035_3->setSomme(somme); break;
        case 4 :
            p2035_4->setSomme(somme); break;
        case 5 :
            p2035_5->setSomme(somme); break;
        case 6 :
            p2035_6->setSomme(somme); break;
        case 7 :
            p2035_7->setSomme(somme); break;
        case 8 :
            p2035_8->setSomme(somme); break;
        case 9 :
            p2035_9->setSomme(somme); break;
        case 10 :
            p2035_10->setSomme(somme); break;
        case 11 :
            p2035_11->setSomme(somme); break;
        case 12 :
            p2035_12->setSomme(somme); break;
        case 13 :
            p2035_13->setSomme(somme); break;
        case 14 :
            p2035_14->setSomme(somme); break;
        case 15 :
            p2035_15->setSomme(somme); break;
        case 16 :
            p2035_16->setSomme(somme); break;
        case 17 :
            p2035_17->setSomme(somme); break;
        case 18 :
            p2035_18->setSomme(somme); break;
        case 19 :
            p2035_19->setSomme(somme); break;
        case 20 :
            p2035_20->setSomme(somme); break;
        case 21 :
            p2035_21->setSomme(somme); break;
        case 22 :
            p2035_22->setSomme(somme); break;
        case 23 :
            p2035_23->setSomme(somme); break;
        case 24 :
            p2035_24->setSomme(somme); break;
        case 25 :
            p2035_25->setSomme(somme); break;
        case 26 :
            p2035_26->setSomme(somme); break;
        case 27 :
            p2035_27->setSomme(somme); break;
        case 28 :
            p2035_28->setSomme(somme); break;
        case 29 :
            p2035_29->setSomme(somme); break;
        case 30 :
            p2035_30->setSomme(somme); break;
        case 31 :
            p2035_31->setSomme(somme); break;
        case 32 :
            p2035_32->setSomme(somme); break;
    }
}

void
NS2035Dialog::AfficheTotaux()
{
    int total, ligne;
    string sSomme;
    char szSomme[255];

    // 1. Total des recettes (lignes 4 à 6)
    total = 0;
    p2035_4->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_5->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_6->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    itoa(total, szSomme, 10);
    p2035_7->setSomme(string(szSomme));

    // 2. Total BH : lignes 16 à 21
    total = 0;
    p2035_16->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_17->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_18->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_19->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_20->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_21->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    itoa(total, szSomme, 10);
    p2035_BH->setSomme(string(szSomme));

    // 3. Total BJ : lignes 22 à 23
    total = 0;
    p2035_22->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_23->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    itoa(total, szSomme, 10);
    p2035_BJ->setSomme(string(szSomme));

    // 4. Total BM : lignes 26 à 29
    total = 0;
    p2035_26->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_27->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_28->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_29->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    itoa(total, szSomme, 10);
    p2035_BM->setSomme(string(szSomme));

    // 5. Total des dépenses (lignes 8 à 31)
    total = 0;
    p2035_8->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_9->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_10->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_11->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_12->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_13->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_14->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_15->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_16->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_17->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_18->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_19->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_20->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_21->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_22->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_23->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_24->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_25->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_26->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_27->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_28->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_29->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_30->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    p2035_31->getSomme(&sSomme);
    ligne = atoi(sSomme.c_str());
    total += ligne;
    itoa(total, szSomme, 10);
    p2035_32->setSomme(string(szSomme));
}

void
NS2035Dialog::CmFermer()
{
    NSUtilDialog::CmOk();
}

void
NS2035Dialog::CmImprimer()
{
}
// fin de nsgendlg.cpp
//////////////////////////////////////////////////////////

