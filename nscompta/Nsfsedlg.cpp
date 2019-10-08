// nsfsedlg.cpp : fichier des dialogues compt et fact/////////////////////////////////////////////////////////

#include <stdio.h>#include <classlib\date.h>
#include <classlib\time.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nspatdlg.h"
#include "nscompta\nscompta.h"
#include "nscompta\nscompta.rh"
#include "nscompta\nsf16dlg.h"
#include "nscompta\nsfsedlg.h"

//***************************************************************************
//
// 						Méthodes de CreerFicheComptDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(CreerFicheComptDialog, NSUtilDialog)
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),

   EV_LV_DISPINFO_NOTIFY(IDC_CPT_LISTFSE,     LVN_GETDISPINFO, DispInfoListeFse),
   EV_LV_DISPINFO_NOTIFY(IDC_CPT_LISTFACT,    LVN_GETDISPINFO, DispInfoListeFact),
   EV_LV_DISPINFO_NOTIFY(IDC_CPT_LISTTPAYANT, LVN_GETDISPINFO, DispInfoListeTP),

   EV_BN_CLICKED(IDC_CPT_PREST, Cm1610),
   EV_BN_CLICKED(IDC_CPT_CCAM,  CmCCAM),

   EV_BN_CLICKED(IDC_CPT_CLAM,  CmLibre),
   EV_BN_CLICKED(IDC_CPT_LOT,   CmMat),

   EV_BN_CLICKED(IDC_CPT_IDEPL, Cm1620),
   EV_BN_CLICKED(IDC_CPT_IKM,   Cm1630),

   // Les fonctions CmModifxxx sont appelées par le double-click
   EV_BN_CLICKED(IDC_CPT_PAIEMENT, CmPaiement),
   EV_BN_CLICKED(IDC_CPT_LOC,      SwitchFFEuro),
   EV_BN_CLICKED(IDC_CPT_EURO,     SwitchFFEuro),
   EV_BN_CLICKED(IDC_CPT_PRENDCOR, CmCorresp),
   EV_BN_CLICKED(IDC_CPT_TPAYANT,  CmTiersPayant),
   EV_BN_CLICKED(IDC_CPT_CONSULT,  CmCodeConsult),
END_RESPONSE_TABLE;

//
// Constructeur
//
CreerFicheComptDialog::CreerFicheComptDialog(TWindow* pere, NSContexte* pCtx, NSPersonInfo* pPat, bool bCreation, NSComptData* pCompData, bool bNoInit)
                      :NSUtilDialog(pere, pCtx, "IDD_COMPT", pNSResModule)
{
	// fichiers d'aide
	sHindex = string("") ;
	sHcorps = string("Fiche_Compta.html") ;

	// stockage du patient
  if (pPat)
	  pPatient = new NSPersonInfo(*pPat) ;
  else
    pPatient = new NSPersonInfo(pContexte->getSuperviseur(), pContexte, pContexte->getPatient()->getNss()) ;

	// Initialisation des donnees et des variables compta
  //
	pData = new NSComptData ;
  if (pCompData)
    *pData = *pCompData ;

	pVar = new NSVarCompta(pCtx) ;
	pVar->setCreation(bCreation) ;

	// Création de tous les "objets de contrôle"
	pDate			  = new NSEditDateC(pContexte, this, IDC_CPT_DATE) ;
	pHeure			= new NSEditHeureC(pContexte, this, IDC_CPT_HEURE) ;
	pExamen			= new NSUtilExamen(pContexte, this, IDC_CPT_EXAMEN, pCtx->getDico()) ;
	pGBMonnaie	= new TGroupBox(this, IDC_CPT_GBMONNAIE) ;
	pLocal			= new TRadioButton(this, IDC_CPT_LOC, pGBMonnaie) ;
	pEuro			  = new TRadioButton(this, IDC_CPT_EURO, pGBMonnaie) ;
	pGBContexte = new TGroupBox(this, IDC_CPT_GBCONTEXTE) ;
	pExterne		= new TRadioButton(this, IDC_CPT_EXTERNE, pGBContexte) ;
  pAmbulatoire= new TRadioButton(this, IDC_CPT_AMBULATOIRE, pGBContexte) ;
	pHospital		= new TRadioButton(this, IDC_CPT_HOSPITAL, pGBContexte) ;

	pSommeDue		= new NSEditSommeDue(pContexte, this, IDC_CPT_SOMDUE) ;
	pDepass			= new NSUtilEditSomme(pContexte, this, IDC_CPT_DEPASS) ;
	pPaye			  = new NSUtilEditSomme(pContexte, this, IDC_CPT_PAYE) ;
	pResteDu		= new NSUtilEditSomme(pContexte, this, IDC_CPT_RESTEDU) ;
	pCorresp		= new TStatic(this, IDC_CPT_PRENDCOR) ;

	pListeFse 	= new NSListFseWindow(this, IDC_CPT_LISTFSE) ;
	pListeFact	= new NSListFactWindow(this, IDC_CPT_LISTFACT) ;
	pListeTP		= new NSListTPWindow(this, IDC_CPT_LISTTPAYANT) ;

  pFseArray 	= new NSFse16Array ;
	pFactArray 	= new NSFactArray ;	pTPArray		= new NSTPArray ;

	nbPrest 		= 0 ;
	nbFact 			= 0 ;
	nbTP			  = 0 ;
	bEuro          = true ;	bCreerPaiement = true ;	sCodeAlerte    = "" ;

  if (false == bNoInit)
    InitData() ;
}

//// Destructeur
//
CreerFicheComptDialog::~CreerFicheComptDialog()
{
	delete pPatient ;
	delete pData ;
	delete pVar ;
	delete pDate ;
	delete pHeure ;
	delete pExamen ;
	delete pGBMonnaie ;
	delete pLocal ;
	delete pEuro ;
	delete pGBContexte ;
	delete pExterne ;
  delete pAmbulatoire ;
	delete pHospital ;
	delete pSommeDue ;
	delete pDepass ;
	delete pPaye ;
	delete pResteDu ;
	delete pCorresp ;
	delete pListeFse ;
	delete pListeFact ;
	delete pListeTP ;
	delete pFseArray ;
	delete pFactArray ;
	delete pTPArray ;
}

// Init information
//
void
CreerFicheComptDialog::InitData()
{
  // en création on met la date et l'heure du jour par défaut
  if (!strcmp(pData->date, ""))
  {
    struct date dateSys ;
  	getdate(&dateSys) ;
    sprintf(pData->date, "%4d%02d%02d", dateSys.da_year, dateSys.da_mon, dateSys.da_day) ;
	}

  pDate->setDate(string(pData->date)) ;
  pVar->setDateC(string(pData->date)) ;

  if (!strcmp(pData->heure, ""))
  {
    struct time heureSys ;
  	gettime(&heureSys) ;
    sprintf(pData->heure, "%02d%02d", heureSys.ti_hour, heureSys.ti_min) ;
  }

  pHeure->setHeure(string(pData->heure)) ;
  pVar->setHeureC(string(pData->heure)) ;

  // correction des examens étant par erreur en texte libre
  if (!strcmp(pData->examen, "£????"))
  {
  	strcpy(pData->examen, "") ;
    strcpy(pData->synonyme, "") ;
  }

  if (MONNAIE_LOCALE == pVar->getMonnaieRef())
  	bEuro = false ;
  else
    bEuro = true ;

  strcpy(codeCorresp, pData->prescript) ;

  InitFse16Array() ;
  InitFactArray() ;
  InitTPArray() ;
}

//// Fonction SetupWindow : Mise en place des data à l'écran et initialisation
//	du tableau des Fse16xx + liste
//
void
CreerFicheComptDialog::SetupWindow()
{
	string      sCodeLexique ;
	string	    sTitre ;

	TDialog::SetupWindow() ;

  string sExamSens = pData->getExam() ;

	// affichage de l'examen : cas normal
  if (string("") != sExamSens)
  {
  	sCodeAlerte  = sExamSens ;
    sCodeLexique = sExamSens + pData->getSynonym() ;
    pExamen->setLabel(sCodeLexique) ;

    if (pVar->isCreation())
    {
			if (ExisteExamen(pPatient->getNss(), pVar->getDateC(), sCodeAlerte))
			{
        string sExamLabel = string("") ;
        pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sCodeLexique, &sExamLabel) ;

      	string sCaption = string("Message du module de comptabilité ") + pContexte->getSuperviseur()->getAppName();
        string sMessage = string("Un même examen (") + sExamLabel + string(") est déjà comptabilisé pour le même patient, à la même date. Voulez-vous quand même en comptabiliser un autre ?") ;
        int ret = MessageBox(sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;
        if (IDNO == ret)
        	CmCancel() ;
      }
		}
	}
  else
  	pExamen->SetText("") ;

	if      (string("NEXTE") == pData->getContext())
  	pExterne->Check() ;
  else if (string("NAMBU") == pData->getContext())
  	pAmbulatoire->Check() ;
  else if (string("NHOST") == pData->getContext())
  	pHospital->Check() ;
  // cas par défaut : à placer dans Préférences
  else if (string("") == pData->getContext())
  	pExterne->Check() ;

	// on met la monnaie à FF par défaut
  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
  {
    pLocal->SetCaption(pVar->getSigle().c_str()) ;
    pLocal->Check() ;
    pSommeDue->setSomme(string(pData->duFranc)) ;
    pDepass->setSomme(string(pData->depassFranc)) ;
    pPaye->setSomme(string(pData->payeFranc)) ;
  }
  else
  {
    pLocal->SetCaption("FF") ;
    pEuro->Check() ;
    pSommeDue->setSomme(string(pData->duEuro)) ;
    pDepass->setSomme(string(pData->depassEuro)) ;
    pPaye->setSomme(string(pData->payeEuro)) ;
  }

  if (strcmp(codeCorresp, ""))
  {
  	RetrouveCorresp(sTitre) ;
		pCorresp->SetCaption(sTitre.c_str()) ;
  }

  // if (InitFse16Array())
  // {
  	InitListeFse() ;
    AfficheListeFse() ;
    AfficheSommeDue() ;
  // }

  // if (InitFactArray())
  // {
  	InitListeFact() ;
    AfficheListeFact() ;
    AffichePaye() ;
  // }

	// if (InitTPArray())
  // {
  	InitListeTP() ;
    AfficheListeTP() ;
  // }
}

//***************************************************************************//
// 								Méthodes InitxxxArray()
//
//		Fonctions pour charger les bases rattachées à NSCompt dans leurs
//		Arrays respectifs : base Fse16xx, Fact et TPayant.
//
//***************************************************************************

//
// 	InitFse1610Array : Charge les fiches 1610 dans NSFseArray
//
bool
CreerFicheComptDialog::InitFse1610Array()
{
	// on récupère la cle
	string		sNumCompt   = string(pData->numcompt) ;
  string		sNumFse1610 = sNumCompt + string("    ") ;
	NSFse1610 Fse1610(pContexte->getSuperviseur()) ;

  Fse1610.lastError = Fse1610.open() ;
  if (Fse1610.lastError != DBIERR_NONE)
  {
  	erreur("Erreur à l'ouverture de la base Fse1610.", standardError, Fse1610.lastError) ;
    return false ;
  }

  Fse1610.lastError = Fse1610.chercheClef(&sNumFse1610,
      														        "",
																          0,
																          keySEARCHGEQ,
																          dbiWRITELOCK) ;

	if (Fse1610.lastError == DBIERR_BOF)	// cas fichier vide
  {
  	Fse1610.close() ;
    return true ;		// le tableau est vide
  }

  if ((Fse1610.lastError != DBIERR_NONE) && (Fse1610.lastError != DBIERR_EOF))
  {
  	erreur("Erreur à la recherche d'une fiche Fse1610.", standardError, Fse1610.lastError) ;
    Fse1610.close() ;
    return false ;
  }

  while (Fse1610.lastError != DBIERR_EOF)
  {
  	Fse1610.lastError = Fse1610.getRecord() ;
    if (Fse1610.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche Fse1610.", standardError, Fse1610.lastError) ;
      Fse1610.close() ;
      return false ;
    }

    // condition d'arret
    if (Fse1610.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&Fse1610)) ;
    nbPrest++ ;

    // ... on passe au composant suivant
    Fse1610.lastError = Fse1610.suivant(dbiWRITELOCK) ;
    if ((Fse1610.lastError != DBIERR_NONE) && (Fse1610.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche Fse1610.", standardError, Fse1610.lastError) ;
      Fse1610.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base Fse1610
  Fse1610.lastError = Fse1610.close() ;
  if (Fse1610.lastError != DBIERR_NONE)
  {
  	erreur("Erreur de fermeture du fichier Fse1610.", standardError, Fse1610.lastError) ;
    return false ;
  }

  return true ;
}

//// 	InitFse1620Array : Charge les fiches 1620 dans NSFseArray
//
bool
CreerFicheComptDialog::InitFse1620Array()
{
	// on récupère la cle
	string		sNumCompt = string(pData->numcompt);
   string 		sNumFse1620 = sNumCompt + string("    ");
	NSFse1620* 	pFse1620 = new NSFse1620(pContexte->getSuperviseur());

   pFse1620->lastError = pFse1620->open();
   if (pFse1620->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base Fse1620.",standardError, pFse1620->lastError) ;
      delete pFse1620;
      return false;
   }

   pFse1620->lastError = pFse1620->chercheClef(&sNumFse1620,
      														 "",
																 0,
																 keySEARCHGEQ,
																 dbiWRITELOCK);

   if (pFse1620->lastError == DBIERR_BOF)	// cas fichier vide
   {
   	pFse1620->close();
      delete pFse1620;
      return true;		// le tableau est vide
   }

   if ((pFse1620->lastError != DBIERR_NONE) && (pFse1620->lastError != DBIERR_EOF))
   {
   	erreur("Erreur à la recherche d'une fiche Fse1620.", standardError, pFse1620->lastError) ;
      pFse1620->close();
      delete pFse1620;
      return false;
   }

   while (pFse1620->lastError != DBIERR_EOF)
   {
   	pFse1620->lastError = pFse1620->getRecord();
      if (pFse1620->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la lecture d'une fiche Fse1620.", standardError, pFse1620->lastError) ;
         pFse1620->close();
         delete pFse1620;
         return false;
      }

      // condition d'arret
      if (pFse1620->getNumCompt() != sNumCompt)
        break ;

      // on remplit le tableau
      pFseArray->push_back(new NSBlocFse16(pFse1620));
      nbPrest++;

      // ... on passe au composant suivant
      pFse1620->lastError = pFse1620->suivant(dbiWRITELOCK);
      if ((pFse1620->lastError != DBIERR_NONE) && (pFse1620->lastError != DBIERR_EOF))
      {
      	erreur("Erreur d'acces à une fiche Fse1620.", standardError, pFse1620->lastError) ;
         pFse1620->close();
         delete pFse1620;
         return false;
      }
   } // fin du while (recherche des composants images)

   // on ferme la base Fse1620
   pFse1620->lastError = pFse1620->close();
   if (pFse1620->lastError != DBIERR_NONE)
   {
   	erreur("Erreur de fermeture du fichier Fse1620.", standardError, pFse1620->lastError) ;
      delete pFse1620;
      return false;
   }

   delete pFse1620;
   return true;
}

//// 	InitFse1630Array : Charge les fiches 1630 dans NSFseArray
//
bool
CreerFicheComptDialog::InitFse1630Array()
{
	// on récupère la cle
	string		sNumCompt = string(pData->numcompt) ;
   string 		sNumFse1630 = sNumCompt + string("    ") ;
	NSFse1630* 	pFse1630 = new NSFse1630(pContexte->getSuperviseur()) ;

   pFse1630->lastError = pFse1630->open() ;
   if (pFse1630->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base Fse1630.",standardError, pFse1630->lastError) ;
      delete pFse1630;
      return false;
   }

   pFse1630->lastError = pFse1630->chercheClef(&sNumFse1630,
      														 "",
																 0,
																 keySEARCHGEQ,
																 dbiWRITELOCK);

   if (pFse1630->lastError == DBIERR_BOF)	// cas fichier vide
   {
   	pFse1630->close();
      delete pFse1630;
      return true;		// le tableau est vide
   }

   if ((pFse1630->lastError != DBIERR_NONE) && (pFse1630->lastError != DBIERR_EOF))
   {
   	erreur("Erreur à la recherche d'une fiche Fse1630.", standardError, pFse1630->lastError) ;
      pFse1630->close();
      delete pFse1630;
      return false;
   }

   while (pFse1630->lastError != DBIERR_EOF)
   {
   	pFse1630->lastError = pFse1630->getRecord();
      if (pFse1630->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la lecture d'une fiche Fse1630.", standardError, pFse1630->lastError) ;
         pFse1630->close();
         delete pFse1630;
         return false;
      }

      // condition d'arret
      if (pFse1630->getNumCompt() != sNumCompt)
        break ;

      // on remplit le tableau
      pFseArray->push_back(new NSBlocFse16(pFse1630));
      nbPrest++;

      // ... on passe au composant suivant
      pFse1630->lastError = pFse1630->suivant(dbiWRITELOCK);
      if ((pFse1630->lastError != DBIERR_NONE) && (pFse1630->lastError != DBIERR_EOF))
      {
      	erreur("Erreur d'acces à une fiche Fse1630.", standardError, pFse1630->lastError) ;
         pFse1630->close();
         delete pFse1630;
         return false;
      }
   } // fin du while (recherche des composants images)

   // on ferme la base Fse1630
   pFse1630->lastError = pFse1630->close();
   if (pFse1630->lastError != DBIERR_NONE)
   {
   	erreur("Erreur de fermeture du fichier Fse1630.", standardError, pFse1630->lastError) ;
      delete pFse1630;
      return false;
   }

   delete pFse1630;
   return true;
}

bool
CreerFicheComptDialog::InitFseCCAMArray()
{
	// on récupère la cle
	string		sNumCompt   = string(pData->numcompt) ;

  // string 		sNumFseCCAM = sNumCompt + string("    ") ;
  string 		sNumFseCCAM = sNumCompt ;
	NSFseCCAM FseCCAM(pContexte->getSuperviseur()) ;

  FseCCAM.lastError = FseCCAM.open() ;
  if (FseCCAM.lastError != DBIERR_NONE)
  {
   	erreur("Erreur à l'ouverture de la base FseCCAM.", standardError, FseCCAM.lastError) ;
    return false ;
  }

  FseCCAM.lastError = FseCCAM.chercheClef(&sNumFseCCAM,
      														        "",
																          0,
																          keySEARCHGEQ,
																          dbiWRITELOCK) ;

  if (FseCCAM.lastError == DBIERR_BOF)	// cas fichier vide
  {
  	FseCCAM.close() ;
    return true ;		// le tableau est vide
  }

  if ((FseCCAM.lastError != DBIERR_NONE) && (FseCCAM.lastError != DBIERR_EOF))
  {
  	erreur("Erreur à la recherche d'une fiche FseCCAM.", standardError, FseCCAM.lastError) ;
    FseCCAM.close() ;
    return false ;
  }

  while (FseCCAM.lastError != DBIERR_EOF)
  {
  	FseCCAM.lastError = FseCCAM.getRecord() ;
    if (FseCCAM.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche FseCCAM.", standardError, FseCCAM.lastError) ;
      FseCCAM.close() ;
      return false ;
    }

    // condition d'arret
    if (FseCCAM.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&FseCCAM)) ;
    nbPrest++ ;

    // ... on passe au composant suivant
    FseCCAM.lastError = FseCCAM.suivant(dbiWRITELOCK) ;
    if ((FseCCAM.lastError != DBIERR_NONE) && (FseCCAM.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche FseCCAM.", standardError, FseCCAM.lastError) ;
      FseCCAM.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base FseCCAM
  FseCCAM.lastError = FseCCAM.close() ;
  if (FseCCAM.lastError != DBIERR_NONE)
  {
   	erreur("Erreur de fermeture du fichier FseCCAM.", standardError, FseCCAM.lastError) ;
    return false ;
  }

	return true ;
}
boolCreerFicheComptDialog::InitFseCLAMArray()
{
	// on récupère la cle
	string sNumCompt = string(pData->numcompt) ;

  string sNumFseCLAM = sNumCompt ;
	NSFseLibre FseCLAM(pContexte->getSuperviseur()) ;

  FseCLAM.lastError = FseCLAM.open() ;
  if (FseCLAM.lastError != DBIERR_NONE)
  {
   	erreur("Erreur à l'ouverture de la base FseLibre.", standardError, FseCLAM.lastError) ;
    return false ;
  }

  FseCLAM.lastError = FseCLAM.chercheClef(&sNumFseCLAM,
      														        "",
																          0,
																          keySEARCHGEQ,
																          dbiWRITELOCK) ;

  if (FseCLAM.lastError == DBIERR_BOF)	// cas fichier vide
  {
  	FseCLAM.close() ;
    return true ;		// le tableau est vide
  }

  if ((FseCLAM.lastError != DBIERR_NONE) && (FseCLAM.lastError != DBIERR_EOF))
  {
  	erreur("Erreur à la recherche d'une fiche FseLibre.", standardError, FseCLAM.lastError) ;
    FseCLAM.close() ;
    return false ;
  }

  while (DBIERR_EOF != FseCLAM.lastError)
  {
  	FseCLAM.lastError = FseCLAM.getRecord() ;
    if (DBIERR_NONE != FseCLAM.lastError)
    {
    	erreur("Erreur à la lecture d'une fiche FseLibre.", standardError, FseCLAM.lastError) ;
      FseCLAM.close() ;
      return false ;
    }

    // condition d'arret
    if (FseCLAM.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&FseCLAM)) ;
    nbPrest++ ;

    // ... on passe au composant suivant
    FseCLAM.lastError = FseCLAM.suivant(dbiWRITELOCK) ;
    if ((FseCLAM.lastError != DBIERR_NONE) && (FseCLAM.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche FseLibre.", standardError, FseCLAM.lastError) ;
      FseCLAM.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base FseCCAM
  FseCLAM.lastError = FseCLAM.close() ;
  if (FseCLAM.lastError != DBIERR_NONE)
  {
   	erreur("Erreur de fermeture du fichier FseLibre.", standardError, FseCLAM.lastError) ;
    return false ;
  }

	return true ;
}
boolCreerFicheComptDialog::InitFseLotsArray()
{
	// on récupère la cle
	string sNumCompt = string(pData->numcompt) ;

  string sNumFseMat = sNumCompt ;
	NSFseMat FseMat(pContexte->getSuperviseur()) ;

  FseMat.lastError = FseMat.open() ;
  if (DBIERR_NONE != FseMat.lastError)
  {
   	erreur("Erreur à l'ouverture de la base FseMat.", standardError, FseMat.lastError) ;
    return false ;
  }

  FseMat.lastError = FseMat.chercheClef(&sNumFseMat,
                                        "",
                                        0,
                                        keySEARCHGEQ,
                                        dbiWRITELOCK) ;

  if (FseMat.lastError == DBIERR_BOF)	// cas fichier vide
  {
  	FseMat.close() ;
    return true ;		// le tableau est vide
  }

  if ((FseMat.lastError != DBIERR_NONE) && (FseMat.lastError != DBIERR_EOF))
  {
  	erreur("Erreur à la recherche d'une fiche FseMat.", standardError, FseMat.lastError) ;
    FseMat.close() ;
    return false ;
  }

  while (DBIERR_EOF != FseMat.lastError)
  {
  	FseMat.lastError = FseMat.getRecord() ;
    if (DBIERR_NONE != FseMat.lastError)
    {
    	erreur("Erreur à la lecture d'une fiche FseMat.", standardError, FseMat.lastError) ;
      FseMat.close() ;
      return false ;
    }

    // condition d'arret
    if (FseMat.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&FseMat)) ;
    nbPrest++ ;

    // ... on passe au composant suivant
    FseMat.lastError = FseMat.suivant(dbiWRITELOCK) ;
    if ((FseMat.lastError != DBIERR_NONE) && (FseMat.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche FseMat.", standardError, FseMat.lastError) ;
      FseMat.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base FseCCAM
  FseMat.lastError = FseMat.close() ;
  if (FseMat.lastError != DBIERR_NONE)
  {
   	erreur("Erreur de fermeture du fichier FseMat.", standardError, FseMat.lastError) ;
    return false ;
  }

	return true ;
}
//// Initialise le tableau des fiches Fse16xx rattachées au meme numéro
// (cad à la meme fiche compt)
//
bool
CreerFicheComptDialog::InitFse16Array()
{
	if ((false == pVar->isCreation()) && (pFseArray->empty()))
  {
    nbPrest = 0 ;

    // on remplit le tableau
    if (false == InitFse1610Array())
      return false ;

    if (false == InitFse1620Array())
      return false ;

    if (false == InitFse1630Array())
      return false ;
    if (false == InitFseCCAMArray())      return false ;    if (false == InitFseCLAMArray())      return false ;    if (false == InitFseLotsArray())      return false ;
    // on trie par numéro de prestation    sort(pFseArray->begin(), pFseArray->end(), blocInferieur) ;
  }

  return true ;
}

//// Initialise le tableau des fiches fact rattachees à numcompt
//
bool
CreerFicheComptDialog::InitFactArray()
{
	// on récupère la cle
  string sNumCompt = string(pData->numcompt) ;
	string sNumFact  = sNumCompt + string("  ") ;

  if (pVar->isCreation() || (false == pFactArray->empty()))
    return true ;

  NSFact Fact(pContexte->getSuperviseur()) ;

  Fact.lastError = Fact.open() ;
  if (DBIERR_NONE != Fact.lastError)
  {
    erreur("Erreur à l'ouverture de la base FACT.", standardError, Fact.lastError) ;
    return false ;
  }

  Fact.lastError = Fact.chercheClef(&sNumFact,
      														  "",
                                    0,
                                    keySEARCHGEQ,
                                    dbiWRITELOCK) ;

  if (DBIERR_BOF == Fact.lastError)	// cas fichier vide
  {
    Fact.close() ;
    return true ;		// le tableau est vide
  }

  if ((DBIERR_NONE != Fact.lastError) && (DBIERR_EOF != Fact.lastError))
  {
    erreur("Erreur à la recherche d'une fiche fact.", standardError, Fact.lastError) ;
    Fact.close() ;
    return false ;
  }

  while (DBIERR_EOF != Fact.lastError)
  {
    Fact.lastError = Fact.getRecord() ;

    if (DBIERR_NONE != Fact.lastError)
    {
      erreur("Erreur à la lecture d'une fiche fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      return false ;
    }

    // condition d'arret
    if (Fact._Donnees.getNumCompt() != sNumCompt)
      break ;

    // on remplit le tableau
    pFactArray->push_back(new NSFactInfo(&Fact)) ;
    nbFact++ ;

    // ... on passe au composant suivant
    Fact.lastError = Fact.suivant(dbiWRITELOCK);
    if ((DBIERR_NONE != Fact.lastError) && (DBIERR_EOF != Fact.lastError))
    {
      erreur("Erreur d'acces à une fiche fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base CARTE_SV2
  Fact.lastError = Fact.close() ;
  if (DBIERR_NONE != Fact.lastError)
  {
    erreur("Erreur de fermeture du fichier FACT.", standardError, Fact.lastError) ;
    return false ;
  }

  // on trie les fact par date
  sort(pFactArray->begin(), pFactArray->end(), factAnterieure) ;

  return true ; 
}

boolCreerFicheComptDialog::InitTPArray()
{
	// on récupère la cle
  string sNumCompt   = string(pData->numcompt) ;
  string sNumTPayant =  sNumCompt + string("  ") ;

  if (pVar->isCreation() || (false == pTPArray->empty()))
    return true ;

  NSTPayant TPayant(pContexte->getSuperviseur()) ;

  TPayant.lastError = TPayant.open() ;
  if (DBIERR_NONE != TPayant.lastError)
  {
    erreur("Erreur à l'ouverture de la base TPayant.", standardError, TPayant.lastError) ;
    return false ;
  }

  TPayant.lastError = TPayant.chercheClef(&sNumTPayant,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

  if (DBIERR_BOF == TPayant.lastError)	// cas fichier vide
  {
    TPayant.close() ;
    return true;		// le tableau est vide
  }

  if ((DBIERR_NONE != TPayant.lastError) && (DBIERR_EOF != TPayant.lastError))
  {
   	erreur("Erreur à la recherche d'une fiche TPayant.", standardError, TPayant.lastError) ;
    TPayant.close() ;
    return false ;
  }

  while (DBIERR_EOF != TPayant.lastError)
  {
    TPayant.lastError = TPayant.getRecord() ;
    if (DBIERR_NONE != TPayant.lastError)
    {
      erreur("Erreur à la lecture d'une fiche TPayant.", standardError, TPayant.lastError) ;
      TPayant.close() ;
      return false ;
    }

    // condition d'arret
    if (string(TPayant._Donnees.numcompt) != sNumCompt)
      break ;

    // on remplit le tableau
    pTPArray->push_back(new NSTPayantInfo(&TPayant)) ;
    nbTP++ ;

    // ... on passe au composant suivant
    TPayant.lastError = TPayant.suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != TPayant.lastError) && (DBIERR_EOF != TPayant.lastError))
    {
      erreur("Erreur d'acces à une fiche TPayant.", standardError, TPayant.lastError) ;
      TPayant.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base CARTE_SV2
  TPayant.lastError = TPayant.close() ;
  if (DBIERR_NONE != TPayant.lastError)
  {
    erreur("Erreur de fermeture du fichier TPayant.", standardError, TPayant.lastError) ;
    return false ;
  }

  return true ;
}

//***************************************************************************//
// 					 Méthodes d'initialisation des ListWindows
//
//		InitListexxx 		: déclare les items de la liste
//    AfficheListexxx 	: charge la liste à partir de l'array correspondant
//    DispInfoListexxx 	: affiche les sub-items de la liste
//
//***************************************************************************

//
// Initialise la liste (ListWindow) des fiches Fse
//
void
CreerFicheComptDialog::InitListeFse()
{
	TListWindColumn colPrest("Prestation", 80, TListWindColumn::Left) ;
  pListeFse->InsertColumn(0, colPrest) ;
  TListWindColumn colQte("Qté", 40, TListWindColumn::Left) ;
  pListeFse->InsertColumn(1, colQte) ;
  TListWindColumn colVal("Valeur", 80, TListWindColumn::Left) ;
  pListeFse->InsertColumn(2, colVal) ;
}

//
// Affiche dans la liste les donnees du tableau
//
void
CreerFicheComptDialog::AfficheListeFse()
{
  char prest[255];
  char cCode[255], cCoeff[255];
  int  coeff, divis;

	pListeFse->DeleteAllItems() ;

	for (int i = nbPrest - 1; i >= 0; i--)
  {
    NSBlocFse16 *pBlocFse = (*pFseArray)[i] ;

    switch (pBlocFse->_iTypePrest)
    {
      case NSBlocFse16::bloc1610 :

        strcpy(cCode, pBlocFse->_p1610->_Donnees.code) ;

        divis = atoi(pBlocFse->_p1610->_Donnees.divis) ;
        coeff = atoi(pBlocFse->_p1610->_Donnees.coeff) * divis ;

        if ((coeff % 100) == 0)
          sprintf(cCoeff, "%d", coeff/100) ;
        else
          sprintf(cCoeff, "%d,%02d", coeff/100, coeff%100) ;

        if (divis == 1)
          sprintf(prest, "%s %s", cCode, cCoeff) ;
        else
          sprintf(prest, "%s %s/%d", cCode, cCoeff, divis) ;

        break ;

      case NSBlocFse16::bloc1620 :

        sprintf(prest, "%s", pBlocFse->_p1620->_Donnees.code_ifd) ;
        break ;

      case NSBlocFse16::bloc1630 :

        sprintf(prest, "%s", pBlocFse->_p1630->_Donnees.code_ik) ;
				break;

      case NSBlocFse16::blocCcam :

        sprintf(prest, "%s", pBlocFse->_pCCAM->_Donnees.code) ;
        break ;

      case NSBlocFse16::blocLibre :

        sprintf(prest, "%s", pBlocFse->_pLibre->_Donnees.code) ;
        break ;

      case NSBlocFse16::blocMat :
      {
        string sCode = pBlocFse->_pMat->getCode() ;

        NSLotInfo lotInfo(pContexte) ;
        NSLot     dbLots(pContexte->getSuperviseur()) ;
        if (DBIERR_NONE == dbLots.getRecordByCode(sCode, &lotInfo, true))
          sprintf(prest, "%s", lotInfo.getLibelle().c_str()) ;

        break ;
      }

      default:        erreur("Type de prestation erronné dans la liste des prestations", standardError, 0) ;        strcpy(prest, "?") ;    }

    TListWindItem Item(prest, 0) ;
    pListeFse->InsertItem(Item) ;
  }
}

//// DispInfoListeFse : Pour afficher les subitems (colonnes 2 à n)
// de la ListWindow des Fse
//
void
CreerFicheComptDialog::DispInfoListeFse(TLwDispInfoNotify& dispInfo)
{
  const int 	BufLen = 255 ;
  static char buffer[BufLen] ;

  int			montant;
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;  int index = dispInfoItem.GetIndex() ;
  // Affiche les informations en fonction de la colonne  NSBlocFse16* pFse16 = (*pFseArray)[index] ;
  switch (dispInfoItem.GetSubItem())  {    case 1: // quantite

      switch (pFse16->getTypePrest())
      {
        case NSBlocFse16::bloc1610 :
          sprintf(buffer, "%s", pFse16->get1610()->_Donnees.quantite) ;
          break ;

        case NSBlocFse16::bloc1620 :
          sprintf(buffer, "%s", pFse16->get1620()->_Donnees.quantite) ;
          break ;

        case NSBlocFse16::bloc1630 :
          sprintf(buffer, "%s", pFse16->get1630()->_Donnees.nbre_km) ;
          break ;
        case NSBlocFse16::blocCcam :          if (!strcmp(pFse16->getCcam()->_Donnees.pourcent, ""))            strcpy(buffer, "100");          else            sprintf(buffer, "%s", pFse16->getCcam()->_Donnees.pourcent) ;          strcat(buffer, "%") ;          break;        case NSBlocFse16::blocLibre :          if (!strcmp(pFse16->getLibre()->_Donnees.pourcent, ""))            strcpy(buffer, "100");          else            sprintf(buffer, "%s", pFse16->getLibre()->_Donnees.pourcent) ;          strcat(buffer, "%") ;          break ;        case NSBlocFse16::blocMat :          strcpy(buffer, "") ;          break;      }

      break ;

    case 2: 	// montant
      switch (pFse16->getTypePrest())      {        case NSBlocFse16::bloc1610 :

          if (false == bEuro)
            sprintf(buffer, "%s", pFse16->get1610()->_Donnees.montant_f) ;
          else
            sprintf(buffer, "%s", pFse16->get1610()->_Donnees.montant_e) ;
          break ;

        case NSBlocFse16::bloc1620 :

          if (false == bEuro)
            sprintf(buffer, "%s", pFse16->get1620()->_Donnees.montant_ifd_f) ;
          else
            sprintf(buffer, "%s", pFse16->get1620()->_Donnees.montant_ifd_e) ;
          break ;

        case NSBlocFse16::bloc1630 :

          if (false == bEuro)
            sprintf(buffer, "%s", pFse16->get1630()->_Donnees.montant_ik_f) ;
          else
            sprintf(buffer, "%s", pFse16->get1630()->_Donnees.montant_ik_e) ;
          break ;
        case NSBlocFse16::blocCcam :          if (false == bEuro)            strcpy(buffer, "") ;          else            sprintf(buffer, "%s", pFse16->getCcam()->_Donnees.montant_e) ;          break;

        case NSBlocFse16::blocLibre :

          if (false == bEuro)
            sprintf(buffer, "%s", pFse16->getLibre()->_Donnees.montant_f) ;
          else
            sprintf(buffer, "%s", pFse16->getLibre()->_Donnees.montant_e) ;
          break ;

        case NSBlocFse16::blocMat :
        
          if (false == bEuro)
            sprintf(buffer, "%s", pFse16->getMat()->_Donnees._montant_f) ;
          else
            sprintf(buffer, "%s", pFse16->getMat()->_Donnees._montant_e) ;
          break ;
      }

      montant = atoi(buffer) ;
      sprintf(buffer, "%5d,%02d", montant/100, montant%100) ;

      break ;
   } // fin du switch

   dispInfoItem.SetText(buffer) ;
}

//// Initialise la liste (ListWindow) des fiches Fact
//
void
CreerFicheComptDialog::InitListeFact()
{
  TListWindColumn colDate("Date", 80, TListWindColumn::Left, 0) ;
  pListeFact->InsertColumn(0, colDate);
  TListWindColumn colMontant("Montant", 80, TListWindColumn::Right, 1);
  pListeFact->InsertColumn(1, colMontant);
  TListWindColumn colModePaie("Mode", 50, TListWindColumn::Left, 2);
  pListeFact->InsertColumn(2, colModePaie);
  TListWindColumn colOrga("Organisme", 80, TListWindColumn::Left, 3);
  pListeFact->InsertColumn(3, colOrga);
}

//
// Affiche dans la liste les donnees du tableau
//
void
CreerFicheComptDialog::AfficheListeFact()
{
	string sLang = pContexte->getUserLanguage() ;

	pListeFact->DeleteAllItems();

	for (int i = nbFact - 1; i >= 0; i--)
  {
		string sDateFact = donne_date(((*pFactArray)[i])->_Donnees.date_paie, sLang) ;
   	TListWindItem Item(sDateFact.c_str(), 0) ;
    pListeFact->InsertItem(Item) ;
  }
}

//// DispInfoListeFact : Pour afficher les subitems (colonnes 2 à n)
// de la ListWindow des Fact
//
void
CreerFicheComptDialog::DispInfoListeFact(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255 ;
  static char buffer[BufLen] ;

  int	montant ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  // Affiche les informations en fonction de la colonne

  switch (dispInfoItem.GetSubItem())
  {
    case 1: // montant

      montant = atoi(((*pFactArray)[index])->_Donnees.montant) ;

      if (!strcmp(((*pFactArray)[index])->_Donnees.unite, "LOC"))
      {
        if (bEuro)
          montant = pVar->getEuroFromLocal(montant) ;
      }
      else // cas montant en euro
      {
        if (false == bEuro)
          montant = pVar->getLocalFromEuro(montant) ;
      }

      sprintf(buffer, "%5d,%02d", montant/100, montant%100) ;
      dispInfoItem.SetText(buffer) ;
      break ;

    case 2: 	// mode de paiement

      sprintf(buffer, "%s", ((*pFactArray)[index])->_Donnees.mode_paie) ;
      dispInfoItem.SetText(buffer);
      break;

    case 3:	// organisme

      sprintf(buffer, "%s", ((*pFactArray)[index])->_Donnees.libelle) ;
      dispInfoItem.SetText(buffer);
      break;
  } // fin du switch
}

//
// Initialise la liste (ListWindow) des fiches TPayant
//
void
CreerFicheComptDialog::InitListeTP()
{
  TListWindColumn colOrga("Organisme", 80, TListWindColumn::Left, 0);
  pListeTP->InsertColumn(0, colOrga);
  TListWindColumn colResteDu("Reste dû", 80, TListWindColumn::Left, 1);
  pListeTP->InsertColumn(1, colResteDu);
}

//
// Affiche dans la liste les donnees du tableau
//
void
CreerFicheComptDialog::AfficheListeTP()
{
   char orga[255];

	pListeTP->DeleteAllItems();

	for (int i = nbTP - 1; i >= 0; i--)
  {
    strcpy(orga, ((*pTPArray)[i])->_Donnees.libelle);
   	TListWindItem Item(orga, 0);
    pListeTP->InsertItem(Item);
  }
}

//
// DispInfoListeTP : Pour afficher les subitems (colonnes 2 à n)
// de la ListWindow des TPayant
//
void
CreerFicheComptDialog::DispInfoListeTP(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255 ;
  static char buffer[BufLen] ;

  int	montant ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  // Affiche les informations en fonction de la colonne

  switch (dispInfoItem.GetSubItem())
  {
    case 1: // montant

      montant = atoi(((*pTPArray)[index])->_Donnees.reste_du) ;

      if (!strcmp(((*pTPArray)[index])->_Donnees.monnaie, "LOC"))
      {
        if (bEuro)
          montant = pVar->getEuroFromLocal(montant) ;
      }
      else // cas montant en euro
      {
        if (false == bEuro)
          montant = pVar->getLocalFromEuro(montant) ;
      }

      sprintf(buffer, "%5d,%02d", montant/100, montant%100) ;
      dispInfoItem.SetText(buffer) ;
      break ;

   } // fin du switch
}

//***************************************************************************
//
// 					 			Méthodes InitDataxxx
//
//			Lancement des dialogues fils Fsexxx, Fact et TPayant
//
//***************************************************************************

//
// Fonction InitDataFse1610 pour la création / modification des fiches 1610
//
bool
CreerFicheComptDialog::InitDataFse1610(NSFse1610Info* pFse1610Info, bool bCreer)
{
	CreerFse1610Dialog Fse1610Dlg(this, pContexte, pVar) ;

	if (false == bCreer)	// on est en modification
		*(Fse1610Dlg.pData) = pFse1610Info->_Donnees ;

	if ((Fse1610Dlg.Execute()) != IDOK)
   	return false ;

	// on stocke les donnees du dialogue dans les Data
	pFse1610Info->_Donnees = *(Fse1610Dlg.pData) ;

	return true ;
}

//// Fonction InitDataFse1620 pour la création / modification des fiches 1620
//
bool
CreerFicheComptDialog::InitDataFse1620(NSFse1620Info* pFse1620Info, bool bCreer)
{
	CreerFse1620Dialog* pFse1620Dlg = new CreerFse1620Dialog(this, pContexte, pVar) ;

  if (false == bCreer)	// on est en modification
    *(pFse1620Dlg->pData) = pFse1620Info->_Donnees ;

  if ((pFse1620Dlg->Execute()) == IDCANCEL)
  {
    delete pFse1620Dlg ;
   	return false ;
  }

  // on stocke les donnees du dialogue dans les Data
  pFse1620Info->_Donnees = *(pFse1620Dlg->pData) ;

  delete pFse1620Dlg ;

  return true ;
}

//// Fonction InitDataFse1630 pour la création / modification des fiches 1630
//
bool
CreerFicheComptDialog::InitDataFse1630(NSFse1630Info* pFse1630Info, bool bCreer)
{
	CreerFse1630Dialog* pFse1630Dlg = new CreerFse1630Dialog(this, pContexte, pVar) ;

  if (false == bCreer)	// on est en modification
    *(pFse1630Dlg->pData) = pFse1630Info->_Donnees ;

  if ((pFse1630Dlg->Execute()) == IDCANCEL)
  {
    delete pFse1630Dlg ;
   	return false ;
  }

  // on stocke les donnees du dialogue dans les Data
  pFse1630Info->_Donnees = *(pFse1630Dlg->pData) ;

  delete pFse1630Dlg ;

  return true ;
}
boolCreerFicheComptDialog::InitDataFseCCAM(NSFseCCAMInfo* pFseCCAMInfo, bool bCreer){	CreerFseCCAMDialog* pFseCCAMDlg = new CreerFseCCAMDialog(this, pContexte, pVar, string(pData->examen)) ;
	if (false == bCreer)	// on est en modification		*(pFseCCAMDlg->pData) = pFseCCAMInfo->_Donnees ;

	if ((pFseCCAMDlg->Execute()) == IDCANCEL)	{
		delete pFseCCAMDlg ;
		return false ;
	}

	// on stocke les donnees du dialogue dans les Data	pFseCCAMInfo->_Donnees = *(pFseCCAMDlg->pData) ;

	delete pFseCCAMDlg ;
	return true ;}
bool
CreerFicheComptDialog::InitDataFseLibre(NSFseLibreInfo* pFseLibreInfo, bool bCreer){	CreerFseLibreDialog FseLibreDlg(this, pContexte, pVar, string(pData->examen)) ;
	if (false == bCreer)	// on est en modification		*(FseLibreDlg.pData) = pFseLibreInfo->_Donnees ;

	if (FseLibreDlg.Execute() == IDCANCEL)		return false ;

	// on stocke les donnees du dialogue dans les Data	pFseLibreInfo->_Donnees = *(FseLibreDlg.pData) ;

	return true ;}
bool
CreerFicheComptDialog::InitDataFseMat(){	CreerFseMatDialog FseMatDlg(this, pContexte, pVar) ;
  string sCodeSens = string("") ;
  string sCodeExam = pExamen->getCode() ;
  pContexte->getDico()->donneCodeSens(&sCodeExam, &sCodeSens) ;

  FseMatDlg.setExam(sCodeSens) ;

  // Initialize dialog's selected lots
  //
  if (false == pFseArray->empty())
  {
    NSLot baseLots(pContexte->getSuperviseur()) ;

    for (NSFse16Iter it = pFseArray->begin() ; pFseArray->end() != it ; it++)
    {
      if ((*it)->_pMat)
      {
        NSLotInfo lot(pContexte) ;
        if (DBIERR_NONE == baseLots.getRecordByCode((*it)->_pMat->getCode(), &lot, true)) ;
          FseMatDlg.AddSelectedLot(&lot) ;
      }
    }
  }

	if (FseMatDlg.Execute() == IDCANCEL)		return false ;

	// on stocke les donnees du dialogue dans les Data  //  NSLotArray* pActuatedLots = &(FseMatDlg._aSelectMatArray) ;  // First, check previous list  //  if (false == pFseArray->empty())  {
    for (NSFse16Iter it = pFseArray->begin() ; pFseArray->end() != it ; it++)
    {
      if ((*it)->_pMat)
      {
        // Try to find this lot in the new list
        //
        bool bFoundLot = false ;

        if (false == pActuatedLots->empty())
        {          for (LotsIter itLot = pActuatedLots->begin() ;                   (pActuatedLots->end() != itLot) && (false == bFoundLot) ;                                                                       itLot++)          {            if (((*itLot)->getCode()       == (*it)->_pMat->getCode()) &&                ((*itLot)->getPrixVenteE() == (*it)->_pMat->getMontant_e()) &&                ((*itLot)->getPrixVenteF() == (*it)->_pMat->getMontant_f()))            {              bFoundLot = true ;              delete *itLot ;              pActuatedLots->erase(itLot) ;            }          }        }

        // Not found in new lots selection, should be removed
        //
        if (false == bFoundLot)
          removeFromFseArray(it) ;
      }
    }
  }
  // When there, all lots that remain in dialog's selected lots are new ones  //  if (false == pActuatedLots->empty())  {    for (LotsIter itLot = pActuatedLots->begin() ;                                    pActuatedLots->end() != itLot ; itLot++)    {      NSFseMatInfo matInfo ;      matInfo.initFromProduitInfo(*itLot) ;      NSBlocFse16 newBloc(&matInfo) ;      addToFseArray(&newBloc) ;    }  }	return true ;}

//// Fonction InitDataFact pour la creation / modification des fiches Fact
//
bool
CreerFicheComptDialog::InitDataFact(NSFactInfo* pFactInfo, bool bCreer)
{
	CreerFicheFactDialog* pFactDlg = new CreerFicheFactDialog(this, pContexte, pVar) ;

  // on est en modification ou bien on a pré-initialisé la fiche pour un débiteur
  if ((false == bCreer) || (string("") != pFactInfo->_Donnees.getPayor()))
    *(pFactDlg->pData) = pFactInfo->_Donnees ;

  if (IDCANCEL == (pFactDlg->Execute()))
  {
    delete pFactDlg ;
   	return false ;
  }

  // on stocke les donnees du dialogue dans les Data
  pFactInfo->_Donnees = *(pFactDlg->pData) ;

  delete pFactDlg ;

  return true ;
}

bool
CreerFicheComptDialog::InitDataTP(NSTPayantInfo* pTPInfo, bool bCreer)
{
	CreerTPayantDialog* pTPDlg = new CreerTPayantDialog(this, pContexte, pVar) ;

  if (false == bCreer)	// on est en modification
    *(pTPDlg->pData) = pTPInfo->_Donnees ;

  if ((pTPDlg->Execute()) == IDCANCEL)
  {
    delete pTPDlg ;
    return false ;
  }

  // on stocke les donnees du dialogue dans les Data
  pTPInfo->_Donnees = *(pTPDlg->pData) ;

  delete pTPDlg ;

  return true ;
}

//***************************************************************************
//
// 					 			Méthodes EnregxxxArray
//
//			Enregistrement des arrays dans les bases respectives
//			Fsexx, Fact et TPayant.
//
//***************************************************************************

bool
CreerFicheComptDialog::EnregFseArray(string sNumCompt)
{
  if (pFseArray->empty())
    return true ;

  string sCodePrest ;
  int    nbPrestEnreg ;
  int    nbFse1610 = 0, nbFse1620 = 0, nbFse1630 = 0, nbFseCCAM = 0, nbFseLibre = 0, nbFseMat = 0 ;
  bool   bErreur = false ;

  NSFse1610 Fse1610(pContexte->getSuperviseur()) ;

  Fse1610.lastError = Fse1610.open() ;
  if (DBIERR_NONE != Fse1610.lastError)
  {
    erreur("Erreur à l'ouverture de la base Fse1610.", standardError, Fse1610.lastError) ;
    return false ;
  }

  NSFse1620 Fse1620(pContexte->getSuperviseur()) ;

  Fse1620.lastError = Fse1620.open() ;
  if (DBIERR_NONE != Fse1620.lastError)
  {
    erreur("Erreur à l'ouverture de la base Fse1620.", standardError, Fse1620.lastError) ;
    Fse1610.close() ;
    return false;
  }

  NSFse1630 Fse1630(pContexte->getSuperviseur()) ;

  Fse1630.lastError = Fse1630.open() ;
  if (DBIERR_NONE != Fse1630.lastError)
  {
    erreur("Erreur à l'ouverture de la base Fse1630.", standardError, Fse1630.lastError) ;
    Fse1610.close() ;
    Fse1620.close() ;
    return false ;
  }
  NSFseCCAM FseCCAM(pContexte->getSuperviseur()) ;  FseCCAM.lastError = FseCCAM.open() ;
  if (DBIERR_NONE != FseCCAM.lastError)
  {
    erreur("Erreur à l'ouverture de la base FseCCAM.", standardError, FseCCAM.lastError) ;
    Fse1610.close() ;
    Fse1620.close() ;
    Fse1630.close() ;
    return false ;
  }

  NSFseLibre FseLibre(pContexte->getSuperviseur()) ;
  FseLibre.lastError = FseLibre.open() ;
  if (DBIERR_NONE != FseLibre.lastError)
  {
    erreur("Erreur à l'ouverture de la base FseLibre.", standardError, FseLibre.lastError) ;
    // Fse1610.close() ;
    // Fse1620.close() ;
    // Fse1630.close() ;
    // FseCCAM.close() ;
    // return false ;
  }

  NSFseMat FseMat(pContexte->getSuperviseur()) ;
  FseMat.lastError = FseMat.open() ;
  if (DBIERR_NONE != FseMat.lastError)
  {
    erreur("Erreur à l'ouverture de la base FseMat.", standardError, FseMat.lastError) ;
    // Fse1610.close() ;
    // Fse1620.close() ;
    // Fse1630.close() ;
    // FseCCAM.close() ;
    // FseLibre.close() ;
    // return false ;
  }

  NSFse1610Info  Fse1610Info ;  NSFse1620Info  Fse1620Info ;
  NSFse1630Info  Fse1630Info ;
  NSFseCCAMInfo  FseCCAMInfo ;
  NSFseLibreInfo FseLibreInfo ;
  NSFseMatInfo   FseMatInfo ;
  int index = 0 ;  while ((false == bErreur) && (index < nbPrest))
  {
    NSBlocFse16 *pBlocFse = (*pFseArray)[index] ;

    switch (pBlocFse->getTypePrest())
   	{
      // cas Fse1610
      //////////////////////////////////////////////////////////////////
      case NSBlocFse16::bloc1610 :

        Fse1610Info = *(pBlocFse->get1610()) ;

        strcpy(Fse1610Info._Donnees.numcompt, sNumCompt.c_str()) ;

   			sCodePrest = Fse1610Info.getNumCompt() + Fse1610Info.getNumPrest() ;
   			Fse1610.lastError = Fse1610.chercheClef(&sCodePrest,                                                "",
                                                0,
                                                keySEARCHEQ,
                                                dbiWRITELOCK) ;

   			if ((DBIERR_NONE != Fse1610.lastError) && (DBIERR_RECNOTFOUND != Fse1610.lastError))
   			{
   				erreur("Erreur à la recherche de la fiche Fse1610.", standardError, Fse1610.lastError) ;
          bErreur = true ;
          break ;
   			}

   			Fse1610._Donnees = Fse1610Info._Donnees ;

        if (DBIERR_RECNOTFOUND == Fse1610.lastError)
        {
          Fse1610.lastError = Fse1610.appendRecord() ;
   				if (DBIERR_NONE != Fse1610.lastError)
   				{
   					erreur("Erreur à l'ajout de la fiche Fse1610.", standardError, Fse1610.lastError) ;
            bErreur = true ;
            break ;
   				}
        }
        else
        {
          Fse1610.lastError = Fse1610.modifyRecord(TRUE) ;
   				if (DBIERR_NONE != Fse1610.lastError)
   				{
   					erreur("Erreur à la modification de la fiche Fse1610.", standardError, Fse1610.lastError) ;
            bErreur = true ;
            break ;
   				}
        }

        nbFse1610++ ;
        break ;

      // cas Fse1620      //////////////////////////////////////////////////////////////////
      case NSBlocFse16::bloc1620 :

        Fse1620Info = *(pBlocFse->get1620()) ;
        strcpy(Fse1620Info._Donnees.numcompt, sNumCompt.c_str()) ;
   			sCodePrest = Fse1620Info.getNumCompt() + Fse1620Info.getNumPrest() ;
   			Fse1620.lastError = Fse1620.chercheClef(&sCodePrest,                                                "",
                                                0,
                                                keySEARCHEQ,
                                                dbiWRITELOCK) ;

   			if ((DBIERR_NONE != Fse1620.lastError) && (DBIERR_RECNOTFOUND != Fse1620.lastError))
   			{
          erreur("Erreur à la recherche de la fiche Fse1620.", standardError, Fse1620.lastError) ;
          bErreur = true ;
          break ;
   			}

	   		Fse1620._Donnees = Fse1620Info._Donnees ;

        if (DBIERR_RECNOTFOUND == Fse1620.lastError)
        {
          Fse1620.lastError = Fse1620.appendRecord() ;
   				if (DBIERR_NONE != Fse1620.lastError)
   				{
   					erreur("Erreur à l'ajout de la fiche Fse1620.", standardError, Fse1620.lastError) ;
            bErreur = true ;
            break ;
   				}
        }
        else
        {
          Fse1620.lastError = Fse1620.modifyRecord(TRUE) ;
   				if (DBIERR_NONE != Fse1620.lastError)
   				{
   					erreur("Erreur à la modification de la fiche Fse1620.", standardError, Fse1620.lastError) ;
            bErreur = true ;
            break ;
   				}
        }

        nbFse1620++ ;
        break ;

      // cas Fse1630
      //////////////////////////////////////////////////////////////////
      case NSBlocFse16::bloc1630 :

        Fse1630Info = *(pBlocFse->get1630()) ;

        strcpy(Fse1630Info._Donnees.numcompt, sNumCompt.c_str()) ;

   			sCodePrest = Fse1630Info.getNumCompt() + Fse1630Info.getNumPrest() ;

   			Fse1630.lastError = Fse1630.chercheClef(&sCodePrest,
                                                "",
                                                0,
                                                keySEARCHEQ,
                                                dbiWRITELOCK) ;

   			if ((DBIERR_NONE != Fse1630.lastError) && (DBIERR_RECNOTFOUND != Fse1630.lastError))
   			{
          erreur("Erreur à la recherche de la fiche Fse1630.", standardError, Fse1630.lastError) ;
          bErreur = true ;
          break ;
   			}

   			Fse1630._Donnees = Fse1630Info._Donnees ;

        if (DBIERR_RECNOTFOUND == Fse1630.lastError)
        {
          Fse1630.lastError = Fse1630.appendRecord() ;
   				if (DBIERR_NONE != Fse1630.lastError)
   				{
   					erreur("Erreur à l'ajout de la fiche Fse1630.", standardError, Fse1630.lastError) ;
            bErreur = true ;
            break ;
   				}
        }
        else
        {
   				Fse1630.lastError = Fse1630.modifyRecord(TRUE) ;
   				if (DBIERR_NONE != Fse1630.lastError)
   				{
   					erreur("Erreur à la modification de la fiche Fse1630.", standardError, Fse1630.lastError) ;
            bErreur = true ;
            break ;
   				}
        }

        nbFse1630++ ;
        break ;
      // cas FseCCAM      //////////////////////////////////////////////////////////////////
      case NSBlocFse16::blocCcam :
        FseCCAMInfo = *(pBlocFse->getCcam()) ;
        strcpy(FseCCAMInfo._Donnees.numcompt, sNumCompt.c_str()) ;

   			sCodePrest = FseCCAMInfo.getNumCompt() + FseCCAMInfo.getNumPrest() ;

   			FseCCAM.lastError = FseCCAM.chercheClef(&sCodePrest,
                                                "",
                                                0,
                                                keySEARCHEQ,
                                                dbiWRITELOCK) ;

   			if ((DBIERR_NONE != FseCCAM.lastError) && (DBIERR_RECNOTFOUND != FseCCAM.lastError))
   			{
   				erreur("Erreur à la recherche de la fiche FseCCAM.", standardError, FseCCAM.lastError) ;
          bErreur = true ;
          break ;
   			}

   			FseCCAM._Donnees = FseCCAMInfo._Donnees ;

        if (DBIERR_RECNOTFOUND == FseCCAM.lastError)
        {
          FseCCAM.lastError = FseCCAM.appendRecord() ;
   				if (DBIERR_NONE != FseCCAM.lastError)
   				{
   					erreur("Erreur à l'ajout de la fiche FseCCAM.", standardError, FseCCAM.lastError) ;
            bErreur = true ;
            break ;
   				}
        }
        else
        {
   				FseCCAM.lastError = FseCCAM.modifyRecord(TRUE) ;
   				if (DBIERR_NONE != FseCCAM.lastError)
   				{
   					erreur("Erreur à la modification de la fiche FseCCAM.", standardError, FseCCAM.lastError) ;
            bErreur = true ;
            break ;
   				}
        }

        nbFseCCAM++ ;
        break ;

      // cas FseLibre
      //////////////////////////////////////////////////////////////////
      case NSBlocFse16::blocLibre :
        FseLibreInfo = *(pBlocFse->getLibre()) ;
        FseLibreInfo.setNumCompt(sNumCompt) ;

   			sCodePrest = FseLibreInfo.getNumCompt() + FseLibreInfo.getNumPrest() ;

   			FseLibre.lastError = FseLibre.chercheClef(&sCodePrest,
                                                  "",
                                                  0,
                                                  keySEARCHEQ,
                                                  dbiWRITELOCK) ;

   			if ((DBIERR_NONE != FseLibre.lastError) && (DBIERR_RECNOTFOUND != FseLibre.lastError))
   			{
   				erreur("Erreur à la recherche de la fiche FseLibre.", standardError, FseLibre.lastError) ;
          bErreur = true ;
          break ;
   			}

   			FseLibre._Donnees = FseLibreInfo._Donnees ;

        if (DBIERR_RECNOTFOUND == FseLibre.lastError)
        {
          FseLibre.lastError = FseLibre.appendRecord() ;
   				if (DBIERR_NONE != FseLibre.lastError)
   				{
   					erreur("Erreur à l'ajout de la fiche FseLibre.", standardError, FseLibre.lastError) ;
            bErreur = true ;
            break ;
   				}
        }
        else
        {
   				FseLibre.lastError = FseLibre.modifyRecord(TRUE) ;
   				if (DBIERR_NONE != FseLibre.lastError)
   				{
   					erreur("Erreur à la modification de la fiche FseLibre.", standardError, FseLibre.lastError) ;
            bErreur = true ;
            break ;
   				}
        }

        nbFseLibre++ ;
        break ;

      // cas FseMat
      //////////////////////////////////////////////////////////////////
      case NSBlocFse16::blocMat :
        FseMatInfo = *(pBlocFse->getMat()) ;
        FseMatInfo.setNumCompt(sNumCompt) ;

   			sCodePrest = FseMatInfo.getNumCompt() + FseMatInfo.getNumPrest() ;

   			FseMat.lastError = FseMat.chercheClef(&sCodePrest,
                                              "",
                                              0,
                                              keySEARCHEQ,
                                              dbiWRITELOCK) ;

   			if ((DBIERR_NONE != FseMat.lastError) && (DBIERR_RECNOTFOUND != FseMat.lastError))
   			{
   				erreur("Erreur à la recherche de la fiche FseMat.", standardError, FseMat.lastError) ;
          bErreur = true ;
          break ;
   			}

   			FseMat._Donnees = FseMatInfo._Donnees ;

        if (DBIERR_RECNOTFOUND == FseMat.lastError)
        {
          FseMat.lastError = FseMat.appendRecord() ;
   				if (DBIERR_NONE != FseMat.lastError)
   				{
   					erreur("Erreur à l'ajout de la fiche FseMat.", standardError, FseMat.lastError) ;
            bErreur = true ;
            break ;
   				}
          else
          {
            NSLot BaseLot(pContexte->getSuperviseur()) ;
            BaseLot.decrementeLot(FseMatInfo.getCode()) ;
          }
        }
        else
        {
   				FseMat.lastError = FseMat.modifyRecord(TRUE) ;
   				if (DBIERR_NONE != FseMat.lastError)
   				{
   					erreur("Erreur à la modification de la fiche FseMat.", standardError, FseMat.lastError) ;
            bErreur = true ;
            break ;
   				}
        }

        nbFseMat++ ;
        break ;

   	} 		// fin du switch

    index++ ;
  }	// fin du while

  // Suppression des FSE1610 supplémentaires
  //
  sCodePrest = sNumCompt + string(FSE_NUMPREST_LEN, '0') ;

  Fse1610.lastError = Fse1610.chercheClef(&sCodePrest,                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

  if ((DBIERR_NONE != Fse1610.lastError) && (DBIERR_EOF != Fse1610.lastError))
  {
    erreur("Erreur à la recherche de la fiche Fse1610.", standardError, Fse1610.lastError) ;
    bErreur = true ;
  }

  nbPrestEnreg = 0 ;

  while ((false == bErreur) && (DBIERR_NONE == Fse1610.lastError))
  {
    Fse1610.lastError = Fse1610.getRecord() ;
		if (DBIERR_NONE != Fse1610.lastError)
		{
			erreur("Erreur de lecture dans la base Fse1610.", standardError, Fse1610.lastError) ;
			bErreur = true ;
      break ;
		}

    // condition d'arret
    if (Fse1610.getNumCompt() != sNumCompt)
      break ;

    nbPrestEnreg++ ;

    if (nbPrestEnreg > nbFse1610)
    {
      Fse1610.lastError = Fse1610.deleteRecord() ;
      if (DBIERR_NONE != Fse1610.lastError)
      {
        erreur("Erreur à la destruction de la fiche Fse1610.", standardError, Fse1610.lastError) ;
        bErreur = true ;
        break ;
      }
    }

    Fse1610.lastError = Fse1610.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Fse1610.lastError) && (DBIERR_EOF != Fse1610.lastError))
    {
      erreur("Erreur d'accès à la fiche Fse1610 suivante.", standardError, Fse1610.lastError) ;
			bErreur = true ;
      break ;
    }
  }

  /// Suppression des FSE1620 supplémentaires
  sCodePrest = sNumCompt + string(FSE_NUMPREST_LEN, '0') ;

  Fse1620.lastError = Fse1620.chercheClef(&sCodePrest,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

  if ((DBIERR_NONE != Fse1620.lastError) && (DBIERR_EOF != Fse1620.lastError))
  {
    erreur("Erreur à la recherche de la fiche Fse1620.", standardError, Fse1620.lastError) ;
    bErreur = true ;
  }

  nbPrestEnreg = 0 ;

  while ((!bErreur) && (DBIERR_NONE == Fse1620.lastError))
  {
    Fse1620.lastError = Fse1620.getRecord() ;
		if (DBIERR_NONE != Fse1620.lastError)
		{
			erreur("Erreur de lecture dans la base Fse1620.", standardError, Fse1620.lastError) ;
			bErreur = true ;
      break ;
		}

    // condition d'arret
    if (Fse1620.getNumCompt() != sNumCompt)
      break ;

    nbPrestEnreg++ ;

    if (nbPrestEnreg > nbFse1620)
    {
      Fse1620.lastError = Fse1620.deleteRecord() ;
      if (Fse1620.lastError != DBIERR_NONE)
      {
        erreur("Erreur à la destruction de la fiche Fse1620.", standardError, Fse1620.lastError) ;
        bErreur = true ;
        break ;
      }
    }

    Fse1620.lastError = Fse1620.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Fse1620.lastError) && (DBIERR_EOF != Fse1620.lastError))
    {
      erreur("Erreur d'accès à la fiche Fse1620 suivante.", standardError, Fse1620.lastError) ;
			bErreur = true ;
      break ;
    }
  }

  /// Suppression des FSE1630 supplémentaires
  sCodePrest = sNumCompt + string(FSE_NUMPREST_LEN, '0') ;

  Fse1630.lastError = Fse1630.chercheClef(&sCodePrest,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

  if ((DBIERR_NONE != Fse1630.lastError) && (DBIERR_EOF != Fse1630.lastError))
  {
    erreur("Erreur à la recherche de la fiche Fse1630.", standardError, Fse1630.lastError) ;
    bErreur = true ;
  }

  nbPrestEnreg = 0 ;

  while ((false == bErreur) && (DBIERR_NONE == Fse1630.lastError))
  {
    Fse1630.lastError = Fse1630.getRecord() ;
		if (DBIERR_NONE != Fse1630.lastError)
		{
			erreur("Erreur de lecture dans la base Fse1630.", standardError, Fse1630.lastError) ;
			bErreur = true ;
      break ;
		}

    // condition d'arret
    if (Fse1630.getNumCompt() != sNumCompt)
      break ;

    nbPrestEnreg++ ;

    if (nbPrestEnreg > nbFse1630)
    {
      Fse1630.lastError = Fse1630.deleteRecord() ;
      if (DBIERR_NONE != Fse1630.lastError)
      {
        erreur("Erreur à la destruction de la fiche Fse1630.", standardError, Fse1630.lastError) ;
        bErreur = true ;
        break ;
      }
    }

    Fse1630.lastError = Fse1630.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Fse1630.lastError) && (DBIERR_EOF != Fse1630.lastError))
    {
      erreur("Erreur d'accès à la fiche Fse1630 suivante.", standardError, Fse1630.lastError) ;
			bErreur = true ;
      break ;
    }
  }
  // Suppression des FSECCAM supplémentaires  //  sCodePrest = sNumCompt + string(FSE_NUMPREST_LEN, '0') ;
  FseCCAM.lastError = FseCCAM.chercheClef(&sCodePrest,                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

  if ((DBIERR_NONE != FseCCAM.lastError) && (DBIERR_EOF != FseCCAM.lastError))
  {
    erreur("Erreur à la recherche de la fiche FseCCAM.", standardError, FseCCAM.lastError) ;
    bErreur = true ;
  }

  nbPrestEnreg = 0 ;

  while ((false == bErreur) && (DBIERR_NONE == FseCCAM.lastError))
  {
    FseCCAM.lastError = FseCCAM.getRecord() ;
		if (DBIERR_NONE != FseCCAM.lastError)
		{
			erreur("Erreur de lecture dans la base FseCCAM.", standardError, FseCCAM.lastError) ;
			bErreur = true ;
      break ;
		}

    // condition d'arret
    if (FseCCAM.getNumCompt() != sNumCompt)
      break ;

    nbPrestEnreg++ ;

    if (nbPrestEnreg > nbFseCCAM)
    {
      FseCCAM.lastError = FseCCAM.deleteRecord() ;
      if (DBIERR_NONE != FseCCAM.lastError)
      {
        erreur("Erreur à la destruction de la fiche FseCCAM.", standardError, FseCCAM.lastError) ;
        bErreur = true ;
        break ;
      }
    }

    FseCCAM.lastError = FseCCAM.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != FseCCAM.lastError) && (DBIERR_EOF != FseCCAM.lastError))
    {
      erreur("Erreur d'accès à la fiche FseCCAM suivante.", standardError, FseCCAM.lastError) ;
			bErreur = true ;
      break ;
    }
  }

  // Suppression des FSELibre supplémentaires
  //  sCodePrest = sNumCompt + string(FSE_NUMPREST_LEN, '0') ;
  FseLibre.lastError = FseLibre.chercheClef(&sCodePrest,                                            "",
                                            0,
                                            keySEARCHGEQ,
                                            dbiWRITELOCK) ;

  if ((DBIERR_NONE != FseLibre.lastError) && (DBIERR_EOF != FseLibre.lastError))
  {
    erreur("Erreur à la recherche de la fiche FseLibre.", standardError, FseLibre.lastError) ;
    bErreur = true ;
  }

  nbPrestEnreg = 0 ;

  while ((false == bErreur) && (DBIERR_NONE == FseLibre.lastError))
  {
    FseLibre.lastError = FseLibre.getRecord() ;
		if (DBIERR_NONE != FseLibre.lastError)
		{
			erreur("Erreur de lecture dans la base FseLibre.", standardError, FseLibre.lastError) ;
			bErreur = true ;
      break ;
		}

    // condition d'arret
    if (FseLibre.getNumCompt() != sNumCompt)
      break ;

    nbPrestEnreg++ ;

    if (nbPrestEnreg > nbFseLibre)
    {
      FseLibre.lastError = FseLibre.deleteRecord() ;
      if (DBIERR_NONE != FseLibre.lastError)
      {
        erreur("Erreur à la destruction de la fiche FseLibre.", standardError, FseLibre.lastError) ;
        bErreur = true ;
        break ;
      }
    }

    FseLibre.lastError = FseLibre.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != FseLibre.lastError) && (DBIERR_EOF != FseLibre.lastError))
    {
      erreur("Erreur d'accès à la fiche FseLibre suivante.", standardError, FseLibre.lastError) ;
			bErreur = true ;
      break ;
    }
  }

  // Suppression des FSEMat supplémentaires
  //  sCodePrest = sNumCompt + string(FSE_NUMPREST_LEN, '0') ;
  FseMat.lastError = FseMat.chercheClef(&sCodePrest,                                        "",
                                        0,
                                        keySEARCHGEQ,
                                        dbiWRITELOCK) ;

  if ((DBIERR_NONE != FseMat.lastError) && (DBIERR_EOF != FseMat.lastError))
  {
    erreur("Erreur à la recherche de la fiche FSEMat.", standardError, FseMat.lastError) ;
    bErreur = true ;
  }

  nbPrestEnreg = 0 ;

  while ((false == bErreur) && (DBIERR_NONE == FseMat.lastError))
  {
    FseMat.lastError = FseMat.getRecord() ;
		if (DBIERR_NONE != FseMat.lastError)
		{
			erreur("Erreur de lecture dans la base FSEMat.", standardError, FseMat.lastError) ;
			bErreur = true ;
      break ;
		}

    // condition d'arret
    if (FseMat.getNumCompt() != sNumCompt)
      break ;

    nbPrestEnreg++ ;

    if (nbPrestEnreg > nbFseMat)
    {
      FseMat.lastError = FseMat.deleteRecord() ;
      if (DBIERR_NONE != FseMat.lastError)
      {
        erreur("Erreur à la destruction de la fiche FSEMat.", standardError, FseMat.lastError) ;
        bErreur = true ;
        break ;
      }
    }

    FseMat.lastError = FseMat.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != FseMat.lastError) && (DBIERR_EOF != FseMat.lastError))
    {
      erreur("Erreur d'accès à la fiche FSEMat suivante.", standardError, FseMat.lastError) ;
			bErreur = true ;
      break ;
    }
  }

  Fse1610.lastError = Fse1610.close() ;
  if (DBIERR_NONE != Fse1610.lastError)
    erreur("Erreur à la fermeture de la base Fse1610.", standardError, Fse1610.lastError) ;

  Fse1620.lastError = Fse1620.close() ;
  if (DBIERR_NONE != Fse1620.lastError)
   	erreur("Erreur à la fermeture de la base Fse1620.", standardError, Fse1620.lastError) ;

  Fse1630.lastError = Fse1630.close() ;
  if (DBIERR_NONE != Fse1630.lastError)
    erreur("Erreur à la fermeture de la base Fse1630.", standardError, Fse1630.lastError) ;
  FseCCAM.lastError = FseCCAM.close() ;  if (DBIERR_NONE != FseCCAM.lastError)  	erreur("Erreur à la fermeture de la base FseCCAM.", standardError, FseCCAM.lastError) ;

  FseLibre.lastError = FseLibre.close() ;
  if (DBIERR_NONE != FseLibre.lastError)  	erreur("Erreur à la fermeture de la base FseLibre.", standardError, FseLibre.lastError) ;

  FseMat.lastError = FseMat.close() ;
  if (DBIERR_NONE != FseMat.lastError)  	erreur("Erreur à la fermeture de la base FseMat.", standardError, FseMat.lastError) ;

  if (bErreur)    return false ;

  return true ;}

boolCreerFicheComptDialog::EnregFactArray(string sNumCompt)
{
    NSFactInfo* pFactInfo = new NSFactInfo();
    string		sCodeFact;
    int 		index = 0;
    int         nbFactEnreg;
    bool		bErreur = false;

    NSFact* pFact = new NSFact(pContexte->getSuperviseur());

	// on ajoute les Data à la base des Fact
    pFact->lastError = pFact->open();
    if (pFact->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base Fact.",standardError, 0) ;
        delete pFact;
        delete pFactInfo;
        return false;
    }

    while (index < nbFact)
    {
   	    *pFactInfo = *((*pFactArray)[index]);

        strcpy(pFactInfo->_Donnees.numcompt, sNumCompt.c_str());

   	    sCodeFact = pFactInfo->_Donnees.getNumCompt() +
      				            pFactInfo->_Donnees.getFactId() ;

   	    pFact->lastError = pFact->chercheClef(&sCodeFact,
      										  "",
											  0,
											  keySEARCHEQ,
											  dbiWRITELOCK);

        if ((pFact->lastError != DBIERR_NONE) && (pFact->lastError != DBIERR_RECNOTFOUND))
        {
      	    erreur("Erreur à la recherche de la fiche Fact.", standardError, pFact->lastError);
            bErreur = true;
            break;
        }

        pFact->_Donnees = pFactInfo->_Donnees ;

        if (pFact->lastError == DBIERR_RECNOTFOUND)
        {
      	    pFact->lastError = pFact->appendRecord();
            if (pFact->lastError != DBIERR_NONE)
            {
         	    erreur("Erreur à l'ajout de la fiche Fact.", standardError, pFact->lastError);
                bErreur = true;
                break;
            }
        }
        else
        {
      	    pFact->lastError = pFact->modifyRecord(TRUE);
            if (pFact->lastError != DBIERR_NONE)
            {
         	    erreur("Erreur à la modification de la fiche Fact.", standardError, pFact->lastError);
                bErreur = true;
                break;
            }
        }

        index++;
    }  // fin du while

    /// Suppression des FACT supplémentaires
    sCodeFact = sNumCompt + string(FACT_NUMERO_LEN, '0');

    pFact->lastError = pFact->chercheClef(&sCodeFact,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK);

    if ((pFact->lastError != DBIERR_NONE) && (pFact->lastError != DBIERR_EOF))
    {
        erreur("Erreur à la recherche de la fiche Fact.",standardError,pFact->lastError);
      	bErreur = true;
    }

    nbFactEnreg = 0;

    while ((!bErreur) && (pFact->lastError == DBIERR_NONE))
    {
        pFact->lastError = pFact->getRecord();
		if (pFact->lastError != DBIERR_NONE)
		{
			erreur("Erreur de lecture dans la base Fact.", standardError, pFact->lastError);
			bErreur = true;
            break;
		}

        // condition d'arret
        if (pFact->_Donnees.getNumCompt() != sNumCompt)
            break;

        nbFactEnreg++;

        if (nbFactEnreg > nbFact)
        {
            pFact->lastError = pFact->deleteRecord();
            if (pFact->lastError != DBIERR_NONE)
            {
                erreur("Erreur à la destruction de la fiche Fact.",standardError,pFact->lastError);
                bErreur = true;
                break;
            }
        }

        pFact->lastError = pFact->suivant(dbiWRITELOCK);
		if ((pFact->lastError != DBIERR_NONE) && (pFact->lastError != DBIERR_EOF))
      	{
			erreur("Erreur d'accès à la fiche Fact suivante.", standardError, pFact->lastError);
			bErreur = true;
            break;
      	}
    }

    pFact->lastError = pFact->close();
    if (pFact->lastError != DBIERR_NONE)
   	    erreur("Erreur à la fermeture de la base Fact.", standardError, pFact->lastError);

    delete pFact;
    delete pFactInfo;

    if (bErreur)
   	    return false;

    return true;
}

bool
CreerFicheComptDialog::EnregTPArray(string sNumCompt)
{
    NSTPayantInfo* pTPayantInfo = new NSTPayantInfo();
    string			sCodeTP;
    int 				index = 0;
    int              nbTPEnreg;
    bool				bErreur = false;

    NSTPayant* pTPayant = new NSTPayant(pContexte->getSuperviseur());

	// on ajoute les Data à la base des TPayant
    pTPayant->lastError = pTPayant->open();
    if (pTPayant->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base TPayant.",standardError, 0) ;
        delete pTPayant;
        delete pTPayantInfo;
        return false;
    }

    while (index < nbTP)
    {
   	    *pTPayantInfo = *((*pTPArray)[index]);

        strcpy(pTPayantInfo->_Donnees.numcompt, sNumCompt.c_str());

   	    sCodeTP = string(pTPayantInfo->_Donnees.numcompt) +
      			        string(pTPayantInfo->_Donnees.numero) ;

   	    pTPayant->lastError = pTPayant->chercheClef(&sCodeTP,
      												"",
													0,
													keySEARCHEQ,
													dbiWRITELOCK);

        if ((pTPayant->lastError != DBIERR_NONE) && (pTPayant->lastError != DBIERR_RECNOTFOUND))
        {
      	    erreur("Erreur à la recherche de la fiche TPayant.", standardError, pTPayant->lastError);
            bErreur = true;
            break;
        }

        pTPayant->_Donnees = pTPayantInfo->_Donnees ;

        // on met à jour okpaye
        if (atoi(pTPayant->_Donnees.reste_du) == 0)
      	    strcpy(pTPayant->_Donnees.okpaye, "1");
        else
      	    strcpy(pTPayant->_Donnees.okpaye, "0");

        if (pTPayant->lastError == DBIERR_RECNOTFOUND)
        {
      	    pTPayant->lastError = pTPayant->appendRecord();
            if (pTPayant->lastError != DBIERR_NONE)
            {
         	    erreur("Erreur à l'ajout de la fiche TPayant.", standardError, pTPayant->lastError);
                bErreur = true;
                break;
            }
        }
        else
        {
      	    pTPayant->lastError = pTPayant->modifyRecord(TRUE);
            if (pTPayant->lastError != DBIERR_NONE)
            {
         	    erreur("Erreur à la modification de la fiche TPayant.", standardError, pTPayant->lastError);
                bErreur = true;
                break;
            }
        }

        index++;
    }  // fin du while

    /// Suppression des Tiers-payants supplémentaires
    sCodeTP = sNumCompt + string(TPAY_NUMERO_LEN, '0');

    pTPayant->lastError = pTPayant->chercheClef(&sCodeTP,
                                                "",
											    0,
											    keySEARCHGEQ,
											    dbiWRITELOCK);

    if ((pTPayant->lastError != DBIERR_NONE) && (pTPayant->lastError != DBIERR_EOF))
    {
        erreur("Erreur à la recherche de la fiche TPayant.",standardError,pTPayant->lastError);
      	bErreur = true;
    }

    nbTPEnreg = 0;

    while ((!bErreur) && (pTPayant->lastError == DBIERR_NONE))
    {
        pTPayant->lastError = pTPayant->getRecord();
		if (pTPayant->lastError != DBIERR_NONE)
		{
			erreur("Erreur de lecture dans la base TPayant.", standardError, pTPayant->lastError);
			bErreur = true;
            break;
		}

        // condition d'arret
        if (!(string(pTPayant->_Donnees.numcompt) == sNumCompt))
            break;

        nbTPEnreg++;

        if (nbTPEnreg > nbTP)
        {
            pTPayant->lastError = pTPayant->deleteRecord();
            if (pTPayant->lastError != DBIERR_NONE)
            {
                erreur("Erreur à la destruction de la fiche TPayant.",standardError,pTPayant->lastError);
                bErreur = true;
                break;
            }
        }

        pTPayant->lastError = pTPayant->suivant(dbiWRITELOCK);
		if ((pTPayant->lastError != DBIERR_NONE) && (pTPayant->lastError != DBIERR_EOF))
      	{
			erreur("Erreur d'accès à la fiche TPayant suivante.", standardError, pTPayant->lastError);
			bErreur = true;
            break;
      	}
   }

   pTPayant->lastError = pTPayant->close();
   if (pTPayant->lastError != DBIERR_NONE)
   	erreur("Erreur à la fermeture de la base TPayant.", standardError, pTPayant->lastError);

   delete pTPayant;
   delete pTPayantInfo;

   if (bErreur)
   	return false;

   return true;
}

//***************************************************************************
//
// 					 	 Méthodes de gestion des champs Edit
//
//			SommeDue, Depass, Paye, DateC, HeureC
//			+ fonction SwitchFFEuro.
//
//***************************************************************************

void
CreerFicheComptDialog::AfficheSommeDue()
{
  int total = 0 ;

  for (int index = 0 ; index < nbPrest ; index++)
  {
    int iPrest = 0 ;

    NSBlocFse16 *pBlocFse = (*pFseArray)[index] ;

    switch (pBlocFse->getTypePrest())
    {
      case NSBlocFse16::bloc1610 :
        if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
          iPrest = atoi(pBlocFse->get1610()->_Donnees.montant_f) ;
        else
					iPrest = atoi(pBlocFse->get1610()->_Donnees.montant_e) ;
        break ;

      case NSBlocFse16::bloc1620 :
        if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
          iPrest = atoi(pBlocFse->get1620()->_Donnees.montant_ifd_f) ;
        else
          iPrest = atoi(pBlocFse->get1620()->_Donnees.montant_ifd_e) ;
        break ;

      case NSBlocFse16::bloc1630 :
        if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
          iPrest = atoi(pBlocFse->get1630()->_Donnees.montant_ik_f) ;
        else
          iPrest = atoi(pBlocFse->get1630()->_Donnees.montant_ik_e) ;
        break ;
      case NSBlocFse16::blocCcam :        if (pVar->getMonnaieRef() == MONNAIE_LOCALE)          iPrest = 0 ;
        else
          iPrest = atoi(pBlocFse->getCcam()->_Donnees.montant_e) ;
        break ;

      case NSBlocFse16::blocLibre :
        if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
          iPrest = atoi(pBlocFse->getLibre()->_Donnees.montant_f) ;
        else
          iPrest = atoi(pBlocFse->getLibre()->_Donnees.montant_e) ;
        break ;

      case NSBlocFse16::blocMat :
        if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
          iPrest = atoi(pBlocFse->getMat()->_Donnees._montant_f) ;
        else
          iPrest = atoi(pBlocFse->getMat()->_Donnees._montant_e) ;
        break ;
    }

    total = total + iPrest ;
  }

  sommeTheorique = total ;
  // on reprend le dépassement  // mais il n'est pas recalculé

  int dep ;

  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    dep = atoi(pData->depassFranc) ;
  else
    dep = atoi(pData->depassEuro) ;

  int iGlobal = total + dep ;

  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
  {
    itoa(iGlobal, pData->duFranc, 10) ;

    int iGlobalEuro = pVar->getEuroFromLocal(iGlobal) ;
    itoa(iGlobalEuro, pData->duEuro, 10) ;
  }
  else // Ref : euro
  {
    itoa(iGlobal, pData->duEuro, 10) ;

    int iGlobalLocal = pVar->getLocalFromEuro(iGlobal) ;
    itoa(iGlobalLocal, pData->duFranc, 10) ;
  }

  char affDepass[9]   = "" ;
  char affSommeDue[9] = "" ;

  if (bEuro)
  {
    strcpy(affSommeDue, pData->duEuro) ;
    strcpy(affDepass, pData->depassEuro) ;
  }
  else
  {
    strcpy(affSommeDue, pData->duFranc) ;
    strcpy(affDepass, pData->depassFranc) ;
  }

  pSommeDue->setSomme(string(affSommeDue)) ;
  pDepass->setSomme(string(affDepass)) ;
}

voidCreerFicheComptDialog::AfficheDepass()
{
  string sSommeReelle ;
  int    diff = 0 ;
  char   affDepass[9] = "" ;

  pSommeDue->getSomme(&sSommeReelle);
  int sommeDue = atoi(sSommeReelle.c_str());

  // on ne traite pas les dépassements négatifs
  if (sommeDue >= sommeTheorique)
  {
    diff = sommeDue - sommeTheorique ;
    itoa(diff, affDepass, 10) ;

    pDepass->setSomme(string(affDepass)) ;
  }
  else
    pDepass->SetText("") ;

  // on conserve la somme due et le depassement dans les deux monnaies
  if (false == bEuro)
  {
    itoa(sommeDue, pData->duFranc, 10) ;
    int iSommeDueEuro = pVar->getEuroFromLocal(sommeDue) ;
    itoa(iSommeDueEuro, pData->duEuro, 10) ;

    itoa(diff, pData->depassFranc, 10) ;
    int iDepassEuro = pVar->getEuroFromLocal(diff) ;
    itoa(iDepassEuro, pData->depassEuro, 10) ;
  }
  else
  {
    itoa(sommeDue, pData->duEuro, 10) ;
    int iSommeDueLoc = pVar->getLocalFromEuro(sommeDue) ;
    itoa(iSommeDueLoc, pData->duFranc, 10) ;

    itoa(diff, pData->depassEuro, 10) ;
    int iDepassLoc = pVar->getLocalFromEuro(diff) ;
    itoa(iDepassLoc, pData->depassFranc, 10) ;
  }
}

voidCreerFicheComptDialog::SwitchFFEuro()
{
  if (bEuro)
    bEuro = false ;
  else
    bEuro = true ;

  AfficheListeFse() ;
  AfficheListeFact() ;
  AfficheSommeDue() ;
  AffichePaye() ;
}

voidCreerFicheComptDialog::AffichePaye()
{
  int		sommeLoc = 0, sommeEuro = 0 ;
  int 	sommeDue ;
  char paye[9]  = "" ;
  char reste[9] = "" ;

  // on parcourt l'array des fiches fact
  // et on affiche la somme dans pPaye

  for (int i = 0; i < nbFact; i++)
  {
    string sUnite = string(((*pFactArray)[i])->_Donnees.unite) ;

    if (sUnite == "LOC")   // unite locale (FF ou FB)
    {
      sommeLoc = sommeLoc + atoi(((*pFactArray)[i])->_Donnees.montant);
    }
    else // cas Euro
    {
      sommeEuro = sommeEuro + atoi(((*pFactArray)[i])->_Donnees.montant);
    }
  }


  double sommeTotaleLoc  = double(sommeLoc) + (double(sommeEuro) * (pVar->getParite())) ;
  int    iTotLoc = dtoi(sommeTotaleLoc) ;

  double sommeTotaleEuro = double(sommeEuro) + (double(sommeLoc) / (pVar->getParite())) ;
  int    iTotEuro = dtoi(sommeTotaleEuro) ;

  // on met les data à jour
  itoa(iTotLoc, pData->payeFranc, 10) ;
  itoa(iTotEuro, pData->payeEuro, 10) ;

  if (!bEuro)
  {
    sommeDue = atoi(pData->duFranc) ;
    itoa(sommeDue - iTotLoc, reste, 10) ;
    strcpy(paye, pData->payeFranc) ;
  }
  else
  {
    sommeDue = atoi(pData->duEuro) ;
    itoa(sommeDue - iTotEuro, reste, 10) ;
    strcpy(paye, pData->payeEuro) ;
  }

  pPaye->setSomme(string(paye)) ;
  pResteDu->setSomme(string(reste)) ;

  // on affiche la liste des tiers-payant pour les reste-dus
  AfficheListeTP() ;
}

voidCreerFicheComptDialog::MajDateC()
{
	string sDate ;

	pDate->getDate(&sDate) ;
	strcpy(pData->date, sDate.c_str()) ;

	// on remet à jour la variable compta
	pVar->setDateC(sDate) ;

	// on reporte la modif dans la fact 0
	if (nbFact)
		strcpy((*pFactArray)[0]->_Donnees.date_paie, sDate.c_str()) ;
}

voidCreerFicheComptDialog::MajHeureC()
{
	string sHeure ;

	pHeure->getHeure(&sHeure) ;
	strcpy(pData->heure, sHeure.c_str()) ;

	// on remet à jour la variable compta
	pVar->setHeureC(sHeure) ;
}

voidCreerFicheComptDialog::RetrouveCorresp(string& sLibelle)
{
	NSPersonInfo* pUtil = pContexte->getPersonArray()->getPerson(pContexte, codeCorresp, pidsUtilisat) ;
	sLibelle = pUtil->getCivilite() ;
}

//// Decremente le reste du de l'organisme du montant spécifié
//
void
CreerFicheComptDialog::DecrementeResteDu(char* codeOrga, char* montant, char* unite)
{
  if (pTPArray->empty())
    return ;

  for (NSTPIter i = pTPArray->begin() ; pTPArray->end() != i ; i++)
  {
   	if (!strcmp((*i)->_Donnees.organisme, codeOrga))
    {
      int resteDu  = atoi((*i)->_Donnees.reste_du) ;
      int paiement = atoi(montant) ;

      string sUniteDu = string((*i)->_Donnees.monnaie) ;

      if (string("LOC") == sUniteDu)
      {
        if (!strcmp(unite, "LOC"))
          resteDu -= paiement ;
        else
          resteDu -= pVar->getLocalFromEuro(paiement) ;
      }
      else
      {
        if (!strcmp(unite, "EUR"))
          resteDu -= paiement ;
        else
          resteDu -= pVar->getEuroFromLocal(paiement) ;
      }

      // on replace le nouveau reste du
      itoa(resteDu, (*i)->_Donnees.reste_du, 10) ;

      break ;
    }
  }
}

//// Incremente le reste du de l'organisme du montant spécifié
//
void
CreerFicheComptDialog::IncrementeResteDu(char* codeOrga, char* montant, char* unite)
{
  if (pTPArray->empty())
    return ;

  for (NSTPIter i = pTPArray->begin() ; pTPArray->end() != i ; i++)
  {
    if (!strcmp((*i)->_Donnees.organisme, codeOrga))
    {
      int resteDu  = atoi((*i)->_Donnees.reste_du) ;
      int paiement = atoi(montant) ;

      string sUniteDue = string((*i)->_Donnees.monnaie) ;

      if (string("LOC") == sUniteDue)
      {
        if (!strcmp(unite, "LOC"))
          resteDu += paiement ;
        else
          resteDu += pVar->getLocalFromEuro(paiement) ;
      }
      else
      {
        if (!strcmp(unite, "EUR"))
          resteDu += paiement ;
        else
          resteDu += pVar->getEuroFromLocal(paiement) ;
      }

      // on replace le nouveau reste du
      itoa(resteDu, (*i)->_Donnees.reste_du, 10) ;

      break ;
    }
  }
}

intCreerFicheComptDialog::SommeResteDu()
{
  if (pTPArray->empty())
    return 0 ;

	int somme = 0 ;

  for (NSTPIter i = pTPArray->begin() ; pTPArray->end() != i ; i++)  {
    int iResteDu = atoi((*i)->_Donnees.reste_du) ;

   	if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
    {
      if (false == bEuro)		// calcul en francs
        somme += iResteDu ;
      else				// calcul en Euros
        somme += pVar->getEuroFromLocal(iResteDu) ;
    }
    else // le reste du est en euros
    {
      if (bEuro)		// calcul en Euros
        somme += iResteDu ;
      else				// calcul en francs
        somme += pVar->getLocalFromEuro(iResteDu) ;
    }
  }

  return somme ;
}

boolCreerFicheComptDialog::ExisteExamen(const string sCodePat, const string sDateExam, const string sCodeExam)
{
	bool bExiste = false ;

	NSCompt Compt(pContexte->getSuperviseur()) ;

	//
	// Ouverture du fichier
	//
	Compt.lastError = Compt.open() ;
	if (DBIERR_NONE != Compt.lastError)
	{
    erreur("Erreur à l'ouverture de la base compt.db.", standardError, Compt.lastError) ;
    return false ;
  }

  Compt.lastError = Compt.chercheClef(&sDateExam,                                      "DATE_COMPT",
                                      NODEFAULTINDEX,
                                      keySEARCHGEQ,
                                      dbiWRITELOCK) ;

  if ((DBIERR_NONE != Compt.lastError) && (DBIERR_EOF != Compt.lastError))  {
    erreur("Erreur de positionnement dans la base compt.db.", standardError, Compt.lastError) ;
		Compt.close() ;
		return false ;
	}

  while (DBIERR_EOF != Compt.lastError)  {
    Compt.lastError = Compt.getRecord() ;

		if (DBIERR_NONE != Compt.lastError)		{
			erreur("Erreur de lecture dans la base compt.db.", standardError, Compt.lastError) ;
			Compt.close() ;
			return false ;
		}

    // si on a dépassé la date : on sort...    if (atoi(Compt._Donnees.date) > atoi(sDateExam.c_str()))
      break ;

    if ((!strcmp(Compt._Donnees.date,    sDateExam.c_str())) &&        (!strcmp(Compt._Donnees.patient, sCodePat.c_str()))  &&
        (!strcmp(Compt._Donnees.examen,  sCodeExam.c_str())))
    {
      bExiste = true ;
      break ;
    }

    // on se positionne sur la fiche compt suivante    Compt.lastError = Compt.suivant(dbiWRITELOCK);
		if ((DBIERR_NONE != Compt.lastError) && (DBIERR_EOF != Compt.lastError))
    {
      erreur("Erreur d'accès à la fiche compt suivante.", standardError, Compt.lastError) ;
      Compt.close() ;
			return false ;
    }
  }

  Compt.lastError = Compt.close() ;
  if (DBIERR_NONE != Compt.lastError)
		erreur("Erreur de fermeture de la base compt.db.", standardError, Compt.lastError) ;

  return bExiste ;
}

//***************************************************************************//
// 				  Méthodes de réponse aux boutons et double-clicks
//
//***************************************************************************

//
// Fonction Cm1610 : insere une nouvelle Fse1610 dans pFseArray
//
void
CreerFicheComptDialog::Cm1610()
{
	NSFse1610Info Fse1610Info ;
	string        sNumCompt = string(pData->numcompt) ;
  char far      examen[CPTA_EXAMEN_LEN + 1] ;

	pExamen->GetText(examen, CPTA_EXAMEN_LEN + 1) ;

	// création automatique des fiches de consultation
	if ((strcmp(examen, "")) && (pExamen->getCode() == "GCONS1"))
	{
  	strcpy(Fse1610Info._Donnees.code, pVar->getIndiceConsult().c_str()) ;
    strcpy(Fse1610Info._Donnees.coeff,   "100") ;
    strcpy(Fse1610Info._Donnees.divis,    "1") ;
    strcpy(Fse1610Info._Donnees.quantite, "1") ;
    strcpy(Fse1610Info._Donnees.denombr,  "1") ;

    // cas particulier pour tenir compte des données ci-dessus
    if (!InitDataFse1610(&Fse1610Info, false))
      return ;
	}
	else
	{
  	// on appelle la création d'une fiche 1610
    if (!InitDataFse1610(&Fse1610Info, true))
      return ;
	}

  NSBlocFse16 blocFse(&Fse1610Info) ;
  addToFseArray(&blocFse) ;
}

//// Fonction Cm1620 : insere une nouvelle Fse1620 dans pFseArray
//
void
CreerFicheComptDialog::Cm1620()
{
	NSFse1620Info Fse1620Info ;

  // on appelle la création d'une fiche 1620
  if (false == InitDataFse1620(&Fse1620Info, true))
    return;

  NSBlocFse16 blocFse(&Fse1620Info) ;
  addToFseArray(&blocFse) ;
}

//// Fonction Cm1630 : insere une nouvelle Fse1630 dans pFseArray
//
void
CreerFicheComptDialog::Cm1630()
{
  NSFse1630Info Fse1630Info ;

  // on appelle la création d'une fiche 1630
  if (false == InitDataFse1630(&Fse1630Info, true))
    return;

  NSBlocFse16 blocFse(&Fse1630Info) ;
  addToFseArray(&blocFse) ;
}
voidCreerFicheComptDialog::CmCCAM(){	NSFseCCAMInfo FseCCAMInfo ;
  // On a besoin que l'examen soit en place
  setExamCode() ;

	// on appelle la création d'une fiche 1630
	if (false == InitDataFseCCAM(&FseCCAMInfo, true))
		return ;

  NSBlocFse16 blocFse(&FseCCAMInfo) ;
  addToFseArray(&blocFse) ;
}
voidCreerFicheComptDialog::CmLibre(){	NSFseLibreInfo FseLibreInfo ;
  // On a besoin que l'examen soit en place
  setExamCode() ;

	// on appelle la création d'une fiche 1630
	if (false == InitDataFseLibre(&FseLibreInfo, true))
		return ;

  NSBlocFse16 blocFse(&FseLibreInfo) ;
  addToFseArray(&blocFse) ;
}
void
CreerFicheComptDialog::CmMat(){	// on appelle la création d'une fiche de saisie des lots	InitDataFseMat() ;
}

//// Fonction CmModifFse : pour modifier la fiche Fse16xx sélectionnée
//
void
CreerFicheComptDialog::CmModifFse()
{
	NSFse1610Info  Fse1610Info ;
  NSFse1620Info  Fse1620Info ;
  NSFse1630Info  Fse1630Info ;
  NSFseCCAMInfo  FseCCAMInfo ;  NSFseLibreInfo FseLibreInfo ;  NSFseMatInfo   FseMatInfo ;
	int index = pListeFse->IndexItemSelect() ;
	if (-1 == index)
	{
  	erreur("Vous devez sélectionner une prestation.", standardError, 0) ;
    return ;
	}

  NSBlocFse16 *pBlocFse = (*pFseArray)[index] ;

  if ((NSBlocFse16*) NULL == pBlocFse)
    return ;

	switch (pBlocFse->getTypePrest())
	{
  	// cas Fse1610
    ////////////////////////////////////////////////////////////////////////////////
   	case NSBlocFse16::bloc1610 :

    	Fse1610Info = *(pBlocFse->get1610()) ;

   		if (false == InitDataFse1610(&Fse1610Info, false))
      	return ;

   		// on conserve la modif dans le tableau
   		*(pBlocFse->get1610()) = Fse1610Info ;

      break ;

    // cas Fse1620
    ////////////////////////////////////////////////////////////////////////////////
    case NSBlocFse16::bloc1620 :

    	Fse1620Info = *(pBlocFse->get1620()) ;

   		if (false == InitDataFse1620(&Fse1620Info, false))
      	return ;

   		// on conserve la modif dans le tableau
   		*(pBlocFse->get1620()) = Fse1620Info ;

      break ;

    // cas Fse1630
    ////////////////////////////////////////////////////////////////////////////////
    case NSBlocFse16::bloc1630 :

    	Fse1630Info = *(pBlocFse->get1630()) ;

   		if (false == InitDataFse1630(&Fse1630Info, false))
      	return ;

   		// on conserve la modif dans le tableau
   		*(pBlocFse->get1630()) = Fse1630Info ;

      break ;
    // cas FseCCAM    ////////////////////////////////////////////////////////////////////////////////
  	case NSBlocFse16::blocCcam :

    	FseCCAMInfo = *(pBlocFse->getCcam()) ;

   		if (false == InitDataFseCCAM(&FseCCAMInfo, false))
      	return ;

   		// on conserve la modif dans le tableau
   		*(pBlocFse->getCcam()) = FseCCAMInfo ;

      break ;

    // cas FseLibre
    ////////////////////////////////////////////////////////////////////////////////
  	case NSBlocFse16::blocLibre :

    	FseLibreInfo = *(pBlocFse->getLibre()) ;

   		if (false == InitDataFseLibre(&FseLibreInfo, false))
      	return ;

   		// on conserve la modif dans le tableau
   		*(pBlocFse->getLibre()) = FseLibreInfo ;

      break ;

    // cas FseMat
    ////////////////////////////////////////////////////////////////////////////////
  	case NSBlocFse16::blocMat :

   		InitDataFseMat() ;

      break ;
	}

	// on remet la liste à jour
	AfficheListeFse() ;
	AfficheSommeDue() ;
	AffichePaye() ;
}
//// Fonction CmModifFact : pour modifier la fiche Fact sélectionnée
//
void
CreerFicheComptDialog::CmModifFact()
{
	NSFactInfo factInfo ;

	int index = pListeFact->IndexItemSelect() ;
	if (-1 == index)
	{
  	erreur("Vous devez sélectionner une fiche fact", standardError, 0) ;
    return ;
	}

	if (0 == index)
  	pVar->setFact0(true) ;
	else
  	pVar->setFact0(false) ;

	factInfo = *((*pFactArray)[index]) ;

	// on annule le précédent paiement retrouvé dans FactInfo
	// pour décrémenter ensuite du paiement réel.
	IncrementeResteDu(factInfo._Donnees.organisme,
                    factInfo._Donnees.montant,
                    factInfo._Donnees.unite) ;

	if (false == InitDataFact(&factInfo, false))
		return ;

	DecrementeResteDu(factInfo._Donnees.organisme,
                    factInfo._Donnees.montant,
                    factInfo._Donnees.unite) ;

	*((*pFactArray)[index]) = factInfo ;

	// on remet la liste et le compteur à jour
	AfficheListeFact() ;
	AffichePaye() ;
}

//// Fonction CmModifTP : pour modifier la fiche Tiers-payant sélectionnée
//
void
CreerFicheComptDialog::CmModifTP()
{
	NSTPayantInfo TPayantInfo ;
  int index = pListeTP->IndexItemSelect() ;

  if (-1 == index)
  {
    erreur("Vous devez sélectionner une fiche de tiers-payant",standardError, 0) ;
    return ;
  }

  // on modifie la fiche TPayant en mémoire

  TPayantInfo = *((*pTPArray)[index]) ;

  if (false == InitDataTP(&TPayantInfo, false))
    return ;

  *((*pTPArray)[index]) = TPayantInfo ;

  // on remet la liste à jour
  AfficheListeTP() ;
}

voidCreerFicheComptDialog::CmPaiement()
{
	NSFactInfo factInfo ;

	string sNumCompt = string(pData->numcompt) ;	char   numero[FACT_NUMERO_LEN + 1] ;
	int    resteDuOrga   = 0 ;
	int    resteDuGlobal = 0 ;

	if (pVar->isCreation())  	pVar->setFact0(true) ;
	else
  	pVar->setFact0(false) ;

	// à la creation on ne peut créer qu'une seule fiche de paiement (Fact0)	if (bCreerPaiement)
	{
    // S'il existe un TP qui doit tout le reste du : on le propose
    for (NSTPIter i = pTPArray->begin() ; pTPArray->end() != i ; i++)
    {
      resteDuOrga += atoi((*i)->_Donnees.reste_du) ;

      if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
        resteDuGlobal = atoi(pData->duFranc) - atoi(pData->payeFranc) ;
      else
        resteDuGlobal = atoi(pData->duEuro) - atoi(pData->payeEuro) ;

      if (resteDuOrga == resteDuGlobal)
      {
        strcpy(factInfo._Donnees.organisme, (*i)->_Donnees.organisme) ;
        strcpy(factInfo._Donnees.montant, (*i)->_Donnees.reste_du) ;
        strcpy(factInfo._Donnees.unite, (*i)->_Donnees.monnaie) ;
        break ;
      }
    }

    // on appelle la création d'une fiche Fact
    if (false == InitDataFact(&factInfo, true))
      return ;

    // on copie le numero de fact
    sprintf(numero, "%02d", nbFact) ;
    strcpy(factInfo._Donnees.numero, numero) ;

    // on ajoute l'élément au tableau
    pFactArray->push_back(new NSFactInfo(factInfo)) ;
    nbFact++ ;

    // on decremente le reste du si tiers-payant
    DecrementeResteDu(factInfo._Donnees.organisme,                      factInfo._Donnees.montant,
                      factInfo._Donnees.unite) ;

    // on trie le tableau par date
    sort(pFactArray->begin(), pFactArray->end(), factAnterieure) ;

    // on ne peut creer qu'une fiche à la création
    if (pVar->isCreation())
      bCreerPaiement = false ;
  }
  else // on ne peut entrer dans ce cas qu'en création
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
    MessageBox("On ne peut créer qu'un seul paiement à la création d'une fiche compt.", sCaption.c_str(), MB_OK) ;
  }

  // on remet la liste et le compteur à jour
  AfficheListeFact() ;
  AffichePaye() ;
}

void
CreerFicheComptDialog::CmCorresp()
{
try
{
	string sTitre ;
	ChercheListePatCorDialog* pListeCorDlg ;

	pListeCorDlg = new ChercheListePatCorDialog(this, pContexte /*, pNSResModule*/) ;

	if ((pListeCorDlg->Execute() == IDOK) && (pListeCorDlg->CorrespChoisi >= 0) &&
      (NULL != pListeCorDlg->pCorrespSelect))
	{
  	strcpy(codeCorresp, pListeCorDlg->pCorrespSelect->getNss().c_str()) ;
    sTitre = pListeCorDlg->pCorrespSelect->getTitre() + string(" ") + pListeCorDlg->pCorrespSelect->getNom() + string(" ") + pListeCorDlg->pCorrespSelect->getPrenom() ;
    pCorresp->SetCaption(sTitre.c_str()) ;
	}

	delete pListeCorDlg ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception CreerFicheComptDialog::CmCorresp : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception CreerFicheComptDialog::CmCorresp.", standardError, 0) ;
}
}

voidCreerFicheComptDialog::CmTiersPayant()
{
	NSTPayantInfo TPayantInfo ;
	char numero[TPAY_NUMERO_LEN + 1] ;

	// on appelle la création d'une fiche TPayant
	if (false == InitDataTP(&TPayantInfo, true))
		return ;

	// on copie le numero de la fiche TPayant
	sprintf(numero, "%02d", nbTP) ;
	strcpy(TPayantInfo._Donnees.numero, numero) ;

	// on ajoute l'élément au tableau
	pTPArray->push_back(new NSTPayantInfo(TPayantInfo)) ;
	nbTP++ ;

	// on remet la liste à jour
	AfficheListeTP() ;
}

voidCreerFicheComptDialog::CmPaiementTiersPayant(CreerTPayantDialog* pTPDlg)
{
  NSFactInfo factInfo ;
  string     sNumCompt = string(pData->numcompt) ;
  char       numero[FACT_NUMERO_LEN + 1] ;

  if (pVar->isCreation())
    pVar->setFact0(true) ;
  else
    pVar->setFact0(false) ;

  // à la creation on ne peut créer qu'une seule fiche de paiement (Fact0)
  if (false == pVar->isCreation())
  {
    strcpy(factInfo._Donnees.organisme, pTPDlg->pData->organisme) ;
    strcpy(factInfo._Donnees.montant, pTPDlg->pData->reste_du) ;
    strcpy(factInfo._Donnees.unite, pTPDlg->pData->monnaie) ;

    // on appelle la modification d'une fiche Fact
    if (false == InitDataFact(&factInfo, false))
      return ;

    // on copie le numero de fact
    sprintf(numero, "%02d", nbFact) ;
    strcpy(factInfo._Donnees.numero, numero) ;

    // on ajoute l'élément au tableau
    pFactArray->push_back(new NSFactInfo(factInfo)) ;
    nbFact++ ;

    // on trie le tableau par date
    sort(pFactArray->begin(), pFactArray->end(), factAnterieure) ;

    // on decremente le reste du si tiers-payant
    DecrementeResteDu(factInfo._Donnees.organisme,
                      factInfo._Donnees.montant,
                      factInfo._Donnees.unite) ;

    // on reporte la modif dans le dialogue
    pTPDlg->DecrementeResteDu(factInfo._Donnees.organisme,
                              factInfo._Donnees.montant,
                        		  factInfo._Donnees.unite) ;
  }
  else
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
    MessageBox("Vous devez utiliser le bouton Paiement global à la création d'une fiche Compt.", sCaption.c_str(), MB_OK) ;
  }

  // on remet la liste et le compteur à jour
  AfficheListeFact() ;
  AffichePaye() ;
}

voidCreerFicheComptDialog::CmCodeConsult()
{
	// introduit "consultation" dans le code examen
	string sCodeLexique = "GCONS1" ;
	pExamen->setLabel(sCodeLexique) ;

	// on vérifie qu'il n'existe pas deja une consultation
	if (ExisteExamen(pPatient->getNss(), pVar->getDateC(), string("GCONS")))
	{
    string sExamLabel = string("") ;
    pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sCodeLexique, &sExamLabel) ;

    string sCaption = string("Message du module de comptabilité ") + pContexte->getSuperviseur()->getAppName();
    string sMessage = string("Un même examen (") + sExamLabel + string(") est déjà comptabilisé pour le même patient, à la même date. Voulez-vous quand même en comptabiliser un autre ?") ;
    int ret = MessageBox(sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;
    if (IDNO == ret)
      CmCancel() ;
	}}

voidCreerFicheComptDialog::EnregDonneesCompt(string sNumCompt)
{
	EnregFseArray(sNumCompt) ;

	if (pVar->isCreation() && (pFactArray->empty()))
	{
		// on crée une fact 0 fictive (montant nul)
    NSFactInfo FactInfo ;
    strcpy(FactInfo._Donnees.numcompt,  pData->numcompt) ;
    strcpy(FactInfo._Donnees.numero,    "00") ;
    strcpy(FactInfo._Donnees.operateur, pContexte->getUtilisateur()->getNss().c_str()) ;
    strcpy(FactInfo._Donnees.date_paie, pVar->getDateC().c_str()) ;
    strcpy(FactInfo._Donnees.montant,   "0") ;

    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    	strcpy(FactInfo._Donnees.unite, "LOC") ;
    else
    	strcpy(FactInfo._Donnees.unite, "EUR") ;

    pFactArray->push_back(new NSFactInfo(FactInfo)) ;
    nbFact = 1 ;
	}

	EnregFactArray(sNumCompt) ;
	EnregTPArray(sNumCompt) ;
}

//// Fonction CmOk : on conserve les données saisies dans pData
//
void
CreerFicheComptDialog::CmOk()
{
	string sTexte = string("") ;
	string sResteDu ;

	pResteDu->getSomme(&sResteDu) ;
	if (SommeResteDu() > atoi(sResteDu.c_str()))	{
		erreur("La somme des reste-dûs par les tiers-payant ne doit pas etre supérieure au reste dû.", warningError, 0) ;
		return ;
	}

	strcpy(pData->patient, pPatient->getNss().c_str()) ;	if (false == setExamCode())
  	return ;

	if (pExterne->GetCheck() == BF_CHECKED)
		strcpy(pData->contexte, "NEXTE") ;
  if (pAmbulatoire->GetCheck() == BF_CHECKED)
		strcpy(pData->contexte, "NAMBU") ;
	if (pHospital->GetCheck() == BF_CHECKED)
		strcpy(pData->contexte, "NHOST") ;

	// la somme due, le dépassement et le champ paye
	// sont déjà dans les data

	if (string(pData->payeFranc) < string(pData->duFranc))
		strcpy(pData->okPaye, "0") ;
	else
		strcpy(pData->okPaye, "1") ;

	strcpy(pData->prescript, codeCorresp) ;
	strcpy(pData->operateur, pContexte->getUtilisateur()->getNss().c_str()) ;

	// attention ne pas rentrer les old data
	// sinon n'enregistre pas les cartes vides (!!!)
	// - qui contiennent seulement le n° -

	TDialog::CmOk() ;
}

//// Fonction CmCancel : on supprime le tableau des nouvelles fiches Fse
// qui ont été créés par Cm16xx.
//
void
CreerFicheComptDialog::CmCancel()
{
	TDialog::CmCancel();
}

bool
CreerFicheComptDialog::setExamCode()
{
	char far examen[CPTA_EXAMEN_LEN + 1] ;

  // Saisie du code examen
	pExamen->GetText(examen, CPTA_EXAMEN_LEN + 1) ;
	if (!strcmp(examen, ""))
	{
		erreur("Le champ Examen est obligatoire.", warningError, 0) ;
		return false ;
	}

	// on sait que le champ Examen n'est pas vide	string sTexte = pExamen->getCode() ;
	if (sTexte == "£?????")		// texte libre
	{
		erreur("Vous devez choisir l'examen à partir du lexique, et non en texte libre.", warningError, 0) ;
		return false ;
	}

	string sExamen = string(sTexte, 0, 5) ;
	string sSyn    = string(sTexte, 5, 1) ;
	strcpy(pData->examen, sExamen.c_str()) ;
	strcpy(pData->synonyme, sSyn.c_str()) ;

  return true ;
}

void
CreerFicheComptDialog::Export(string sAction, string sNumCompt)
{
  if ((string("") == sAction) || (string("") == sNumCompt))
    return ;

  string sExportDll = pContexte->getSuperviseur()->getAccountingExportDll() ;

  if (string("") == sExportDll)
    return ;

  TModule* pExportModule = new TModule(sExportDll.c_str(), TRUE) ;
  if (NULL == pExportModule)
  {
    string sErreur = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") + string(" ") + sExportDll ;
    erreur(sErreur.c_str(), standardError, 0) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1, NSSuper::trError) ;
    return ;
  }

  bool (FAR *pAdrFctExport) (NSContexte far *, string far *, string far *) ;
  (FARPROC) pAdrFctExport = pExportModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
  if (NULL == pAdrFctExport)
  {
    delete pExportModule ;
    return ;
  }

  bool bSuccess = ((*pAdrFctExport)((NSContexte far *) pContexte, (string far *) &sAction, (string far *) &sNumCompt)) ;
  if (false == bSuccess)
  {
    delete pExportModule ;
    return ;
  }

  delete pExportModule ;
}

void
CreerFicheComptDialog::addToFseArray(NSBlocFse16 *pBloc)
{
  if ((NSBlocFse16*) NULL == pBloc)
    return ;

  // Setting numprest and incremaenting counter
  //
  pBloc->setNumprest(nbPrest++) ;

	// on ajoute l'élément au tableau
	pFseArray->push_back(new NSBlocFse16(*pBloc)) ;

	// on remet la liste à jour
  AfficheListeFse() ;
  AfficheSommeDue() ;
  AffichePaye() ;
}

void
CreerFicheComptDialog::removeFromFseArray(NSFse16Iter deleteIt)
{
  int iDeletedNumprest = StringToInt((*deleteIt)->getNumprest()) ;

  delete *deleteIt ;
  pFseArray->erase(deleteIt) ;
  nbPrest-- ;

  if (pFseArray->empty())
    return ;

  // on remet à jour les numéros de prestations ultérieurs à indexDel
  //
  for (NSFse16Iter it = pFseArray->begin() ; pFseArray->end() != it ; it++)
  {
    int iBlocNumprest = StringToInt((*it)->getNumprest()) ;
    if (iBlocNumprest > iDeletedNumprest)
    {
      iBlocNumprest-- ;
      (*it)->setNumprest(iBlocNumprest) ;
    }
  }

  // on remet la liste à jour
  //
  AfficheListeFse() ;
  AfficheSommeDue() ;
  AffichePaye() ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSUtilExamen
//
// -----------------------------------------------------------------

NSUtilExamen::NSUtilExamen(NSContexte *pCtx, CreerFicheComptDialog* pDlg, int resId, NSDico* pDico)
             :NSUtilLexique(pCtx, pDlg, resId, pDico)
{
	pDialog = pDlg ;
}

NSUtilExamen::~NSUtilExamen()
{
}

void
NSUtilExamen::ElementSelectionne()
{
	int  ret ;
	char far examen[CPTA_EXAMEN_LEN + 1] ;

	NSUtilLexique::ElementSelectionne() ;

	pDialog->pExamen->GetText(examen, CPTA_EXAMEN_LEN + 1) ;
	if (!strcmp(examen, ""))
		return ;

	// on sait que le champ Examen n'est pas vide
	string sTexte  = pDialog->pExamen->getCode() ;
	string sExamen = string(sTexte, 0, 5) ;

	if (pDialog->sCodeAlerte != sExamen)
	{
		string sDate ;

		pDialog->pDate->getDate(&sDate) ;

		if (pDialog->ExisteExamen(pDialog->pPatient->getNss().c_str(), sDate, sExamen))
    {
      string sExamLabel = string("") ;
      pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sTexte, &sExamLabel) ;

      string sCaption = string("Message du module de comptabilité ") + pContexte->getSuperviseur()->getAppName();
      string sMessage = string("Un même examen (") + sExamLabel + string(") est déjà comptabilisé pour le même patient, à la même date. Voulez-vous quand même en comptabiliser un autre ?") ;
      int ret = MessageBox(sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;
      if (IDNO == ret)
        pDialog->CmCancel() ;
    }

    pDialog->sCodeAlerte = sExamen ;
	}
}

// -----------------------------------------------------------------//
//  Méthodes de CreerFicheFactDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(CreerFicheFactDialog, NSUtilDialog)
  EV_CBN_SELCHANGE(IDC_FACT_ORGA, EvOrgaChange),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(IDC_PAI_IMPRIMER, CmImprimer),
END_RESPONSE_TABLE;

//// Constructeur
//
CreerFicheFactDialog::CreerFicheFactDialog(CreerFicheComptDialog* pere,
                                            NSContexte* pCtx,
                                            NSVarCompta* pVarCompt)
                     : NSUtilDialog(pere, pCtx, "IDD_FACT", pNSResModule)
{
  // Récupération du dialogue pere
  pDlg = pere;

	// Initialisation des donnees  pData          = new NSFactData ;
  pVar           = new NSVarCompta(*pVarCompt) ;

  // Création de tous les "objets de contrôle"
  pDate          = new NSUtilEditDate(pContexte, this, IDC_FACT_DATE) ;
  pOrga          = new TComboBox(this, IDC_FACT_ORGA) ;
  pMontant       = new NSUtilEditSomme(pContexte, this, IDC_FACT_MONTANT) ;
  pUnite         = new TGroupBox(this, IDC_FACT_UNITE) ;
  pLocal         = new TRadioButton(this, IDC_FACT_LOC, pUnite) ;
  pEuro          = new TRadioButton(this, IDC_FACT_EURO, pUnite) ;
  pModePaie      = new TComboBox(this, IDC_FACT_MODEPAIE) ;
  pModePaieArray = new NSComboArray ;

  pCodeOrgaArray = new NSCodeOrgaArray ;
  nbCodeOrga = 0 ;

  if (pVar->isFact0())
    pDate->Attr.Style |= ES_READONLY ;
}

//// Destructeur
//
CreerFicheFactDialog::~CreerFicheFactDialog()
{
  delete pData ;
  delete pVar ;
  delete pDate ;
  delete pOrga ;
  delete pMontant ;
  delete pUnite ;
  delete pLocal ;
  delete pEuro ;
  delete pModePaie ;
  delete pModePaieArray ;
  delete pCodeOrgaArray ;
}

//// Fonction SetupWindow : Mise en place des data à l'écran et initialisation
//	du tableau des Fse16xx + liste
//
void
CreerFicheFactDialog::SetupWindow()
{
  TDate			dateSys;
  NSComboIter     i;
	int 			j;
  NSCodeOrgaIter  k;

  string sFichier = pContexte->PathName("BCPT") + string("modepaie.dat");
  TDialog::SetupWindow();
  // on synchronise la date sur sDateC pour la fact 0  if (!strcmp(pData->date_paie, ""))
  {
    if (pVar->isFact0())
      strcpy(pData->date_paie, pVar->getDateC().c_str());
    else
    {
      sprintf(pData->date_paie, "%4d%02d%02d", (int)dateSys.Year(),
                           (int)dateSys.Month(), (int)dateSys.DayOfMonth()) ;
    }
  }

  pDate->setDate(pData->date_paie) ;

  // on charge la table des organismes
	if (!InitCodeOrgaArray())
  {
    erreur("Probleme au chargement du tableau des codes organisme.",standardError, 0) ;
    return;
  }

  // sélection du code dans la combobox
  for (k = pCodeOrgaArray->begin(), j = 0; k != pCodeOrgaArray->end(); k++,j++)
  {
    if (!strcmp((*k)->_Donnees.code, pData->organisme))
    {
      pOrga->SetSelIndex(j);
      break;
    }
  }

  if (!InitComboBox(pModePaie, pModePaieArray, sFichier))
    erreur("Pb à l'initialisation de la combobox ModePaie",standardError, 0) ;
  else
  {
    // si le mode de paiement n'est pas initialisé
    if (!strcmp(pData->mode_paie, ""))
    {
      // Ancien : S'il s'agit du patient on sélectionne "Carte bancaire" (auparavant "chèque")
      if (!strcmp(pData->organisme, ""))
        // strcpy(pData->mode_paie, "C");
        strcpy(pData->mode_paie, "B");
      else // cas d'un tiers-payant : on sélectionne "Virement"
        strcpy(pData->mode_paie, "V");
    }

    // on positionne la combo box par rapport à mode_paie
    for (i = pModePaieArray->begin(), j = 0 ; i != pModePaieArray->end(); i++,j++)
    {
      if (!strcmp(((*i)->sCode).c_str(), pData->mode_paie))
      {
        pModePaie->SetSelIndex(j);
        break;
      }
    }
  }

  pLocal->SetCaption(pVar->getSigle().c_str()) ;

  if (!strcmp(pData->unite, "LOC"))
  {
    pLocal->Check();
    pEuro->Uncheck();
  }
  else if (!strcmp(pData->unite, "EUR"))
  {
    pLocal->Uncheck() ;
    pEuro->Check() ;
  }
  else
  {
    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    {
      pLocal->Check() ;
      pEuro->Uncheck() ;
    }
    else
    {      pLocal->Uncheck() ;
      pEuro->Check() ;
    }
  }

  if (strcmp(pData->montant, ""))
    pMontant->setSomme(string(pData->montant)) ;
  else
  {
    if (!strcmp(pData->organisme, ""))
      AfficheResteDuPatient() ;
    else
      AfficheResteDuOrga(string(pData->organisme)) ;
  }
}

boolCreerFicheFactDialog::InitCodeOrgaArray()
{
  NSCodeOrga CodeOrga(pContexte->getSuperviseur()) ;

  NSCodeOrgaInfo CodePatientInfo ;

  // Le patient correspond au code orga ""
  //
  strcpy(CodePatientInfo._Donnees.lib_court, "Patient") ;
  // on le met dans le tableau et dans la combo
  //
  pCodeOrgaArray->push_back(new NSCodeOrgaInfo(CodePatientInfo)) ;
  pOrga->AddString(CodePatientInfo._Donnees.lib_court) ;
  nbCodeOrga++ ;

  CodeOrga.lastError = CodeOrga.open() ;  if (DBIERR_NONE != CodeOrga.lastError)
  {
    erreur("Erreur à l'ouverture de la base CodeOrga.", standardError, CodeOrga.lastError) ;
    return false ;
  }

	CodeOrga.lastError = CodeOrga.debut(dbiWRITELOCK) ;  if ((DBIERR_NONE != CodeOrga.lastError) && (DBIERR_EOF != CodeOrga.lastError))
  {
    erreur("Erreur de positionnement dans le fichier CodeOrga.db.", standardError, CodeOrga.lastError) ;
    CodeOrga.close() ;
    return false ;
  }

  while (DBIERR_EOF != CodeOrga.lastError)
  {
    CodeOrga.lastError = CodeOrga.getRecord() ;
    if (DBIERR_NONE != CodeOrga.lastError)
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
    CodeOrga.lastError = CodeOrga.suivant(dbiWRITELOCK);
    if ((DBIERR_NONE != CodeOrga.lastError) && (DBIERR_EOF != CodeOrga.lastError))
    {
      erreur("Erreur d'acces à une fiche CodeOrga.", standardError, CodeOrga.lastError);
      CodeOrga.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la table
  CodeOrga.lastError = CodeOrga.close() ;
  if (DBIERR_NONE != CodeOrga.lastError)
  {
    erreur("Erreur de fermeture du fichier CodeOrga.", standardError, CodeOrga.lastError) ;
    return false ;
  }

  return true ;
}

voidCreerFicheFactDialog::AfficheResteDuPatient()
{
	int resteDuGlobal ;

  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
  {
    resteDuGlobal = atoi(pDlg->pData->duFranc) - atoi(pDlg->pData->payeFranc) ;
    pLocal->Check() ;
    pEuro->Uncheck() ;
  }
  else  {
    resteDuGlobal = atoi(pDlg->pData->duEuro) - atoi(pDlg->pData->payeEuro) ;
    pLocal->Uncheck() ;
    pEuro->Check() ;
  }

  int totalResteDu = 0 ;
	// on calcule la somme totale des restes-du organismes
  for (NSTPIter i = pDlg->pTPArray->begin() ; pDlg->pTPArray->end() != i ; i++)
  {
    int iResteDu = atoi((*i)->_Donnees.reste_du) ;

    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    {      // on calcule en francs
      if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
        totalResteDu += iResteDu ;
      else
        totalResteDu += pVar->getLocalFromEuro(iResteDu) ;
    }
    else
    {
      // on calcule en Euros
      if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
        totalResteDu += pVar->getEuroFromLocal(iResteDu) ;
      else
        totalResteDu += iResteDu ;
    }
  }

  char reste[33] = "" ;
  itoa(resteDuGlobal - totalResteDu, reste, 10) ;
  pMontant->setSomme(string(reste)) ;
}

voidCreerFicheFactDialog::AfficheResteDuOrga(string sCodeOrga)
{
  if (pDlg->pTPArray->empty())
    return ;

	for (NSTPIter i = pDlg->pTPArray->begin() ; pDlg->pTPArray->end() != i ; i++)
  {
    if (!strcmp((*i)->_Donnees.organisme, sCodeOrga.c_str()))
    {
      pMontant->setSomme(string((*i)->_Donnees.reste_du)) ;

      if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
      {
        pLocal->Check() ;
        pEuro->Uncheck() ;
      }
      else
      {
        pLocal->Uncheck() ;
        pEuro->Check() ;
      }

      break ;
    }
  }
}

voidCreerFicheFactDialog::EvOrgaChange()
{
  // on affiche les restes du uniquement à la création (numcompt == "")
  if (!strcmp(pData->numcompt, ""))
  {
    int indexOrga = pOrga->GetSelIndex() ;
    if (indexOrga >= 0)
    {
      if (!strcmp((*pCodeOrgaArray)[indexOrga]->_Donnees.code, ""))
        AfficheResteDuPatient() ;
      else
        AfficheResteDuOrga(string((*pCodeOrgaArray)[indexOrga]->_Donnees.code));
    }
  }
}

voidCreerFicheFactDialog::CmOk()
{
  int    indexMode;
  string sDate ;  pDate->getDate(&sDate) ;
  if (sDate < pVar->getDateC())
  {
    erreur("La date du paiement ne peut être antérieure à l'examen", standardError, 0) ;
    return ;
  }

  if (pVar->isFact0() && (sDate != pVar->getDateC()))
  {
    erreur("La date du premier paiement n'est pas modifiable", standardError, 0) ;
    return ;
  }

  strcpy(pData->date_paie, sDate.c_str()) ;
  strcpy(pData->operateur, pContexte->getUtilisateur()->getNss().c_str()) ;

  int indexOrga = pOrga->GetSelIndex() ;
  if (indexOrga > 0) // Note : le patient correspond ici à indexOrga == 0
  {
    strcpy(pData->organisme, (*pCodeOrgaArray)[indexOrga]->_Donnees.code) ;
    strcpy(pData->libelle,   (*pCodeOrgaArray)[indexOrga]->_Donnees.lib_court) ;
  }

  string sTexte ;
  pMontant->getSomme(&sTexte) ;
  strcpy(pData->montant, sTexte.c_str()) ;

  if (pLocal->GetCheck() == BF_CHECKED)
    strcpy(pData->unite, "LOC") ;
  if (pEuro->GetCheck() == BF_CHECKED)
    strcpy(pData->unite, "EUR") ;

  indexMode = pModePaie->GetSelIndex() ;
  if (indexMode >= 0)
    strcpy(pData->mode_paie, ((*pModePaieArray)[indexMode]->sCode).c_str()) ;

  if (strcmp(pData->montant, "") != 0)
  {
    if (strcmp(pData->unite, "") == 0)
    {
      erreur("Vous devez préciser l'unité", standardError, 0) ;
      return ;
    }
  }
  else
    strcpy(pData->unite, "LOC") ;

  TDialog::CmOk() ;}
voidCreerFicheFactDialog::CmCancel(){
  TDialog::CmCancel();
}

void
CreerFicheFactDialog::CmImprimer()
{
}

// -----------------------------------------------------------------//
//  Méthodes de CreerTPayantDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(CreerTPayantDialog, NSUtilDialog)
    EV_CBN_SELCHANGE(IDC_TP_ORGA, EvOrgaChange),
    EV_BN_CLICKED(IDC_TP_PAIEMENT, CmPaiement),
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//// Constructeur
//
CreerTPayantDialog::CreerTPayantDialog(CreerFicheComptDialog* pere, NSContexte* pCtx, NSVarCompta* pVarCompt)
                   :NSUtilDialog(pere, pCtx, "IDD_TPAYANT", pNSResModule)
{
  pComptDlg = pere ;

	// Initialisation des donnees  pData     = new NSTPayantData ;
  pVar      = new NSVarCompta(*pVarCompt) ;

  // Création de tous les "objets de contrôle"
  pOrga     = new TComboBox(this, IDC_TP_ORGA) ;
  pResteDu  = new NSUtilEditSomme(pContexte, this, IDC_TP_RESTEDU) ;
  pUnite    = new TGroupBox(this, IDC_TP_UNITE) ;
  pLocal    = new TRadioButton(this, IDC_TP_LOC, pUnite) ;
  pEuro     = new TRadioButton(this, IDC_TP_EURO, pUnite) ;

  pCodeOrgaArray = new NSCodeOrgaArray ;
  nbCodeOrga = 0 ;
}

//// Destructeur
//
CreerTPayantDialog::~CreerTPayantDialog()
{
  delete pData ;
  delete pVar ;
  delete pOrga ;
  delete pResteDu ;
  delete pUnite ;
  delete pLocal ;
  delete pEuro ;
  delete pCodeOrgaArray ;
}

//// Fonction SetupWindow : Mise en place des data à l'écran
//
void
CreerTPayantDialog::SetupWindow()
{
  NSCodeOrgaIter k ;
  int            j ;
  int            resteDuGlobal = 0 ;
  int            resteDuOrga = 0 ;
  char           cResteDuPatient[TPAY_RESTE_DU_LEN + 1] = "" ;

	TDialog::SetupWindow() ;

    // on charge la table des organismes	if (false == InitCodeOrgaArray())
  {
    erreur("Probleme au chargement du tableau des codes organisme.", standardError, 0) ;
    return ;
  }

  // sélection du code dans la combobox
  if (false == pCodeOrgaArray->empty())
    for (k = pCodeOrgaArray->begin(), j = 0 ; pCodeOrgaArray->end() != k ; k++, j++)
    {
      if (!strcmp((*k)->_Donnees.code, pData->organisme))
      {
        pOrga->SetSelIndex(j) ;
        break;
      }
    }

  pLocal->SetCaption(pVar->getSigle().c_str()) ;
  if (!strcmp(pData->monnaie, "LOC"))    pLocal->Check() ;
  else if (!strcmp(pData->monnaie, "EUR"))
    pEuro->Check() ;
  else
  {
    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
      pLocal->Check() ;
    else
      pEuro->Check() ;
  }

  // on propose comme reste du par défaut le reste du patient  // si on ne connait pas l'organisme
  if (!strcmp(pData->organisme, ""))
  {
    // Note : on dépend ici de la monnaie de référence pour le calcul
    // car aucune monnaie n'existe à priori pour un organisme vide
    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
      resteDuGlobal = atoi(pComptDlg->pData->duFranc) - atoi(pComptDlg->pData->payeFranc);
    else
      resteDuGlobal = atoi(pComptDlg->pData->duEuro) - atoi(pComptDlg->pData->payeEuro);

    // on calcule la somme totale des restes-du organismes
    for (NSTPIter i = pComptDlg->pTPArray->begin() ; pComptDlg->pTPArray->end() != i ; i++)
    {
      int iResteDu = atoi((*i)->_Donnees.reste_du) ;

      if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
      {
        // on calcule en francs
        if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
          resteDuOrga += iResteDu ;
        else
          resteDuOrga += pVar->getLocalFromEuro(iResteDu) ;
      }
      else
      {
        // on calcule en Euros
        if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
          resteDuOrga += pVar->getEuroFromLocal(iResteDu) ;
        else
          resteDuOrga += iResteDu ;
      }
    }

    itoa(resteDuGlobal - resteDuOrga, cResteDuPatient, 10) ;

    pResteDu->setSomme(string(cResteDuPatient)) ;  }  else
    pResteDu->setSomme(string(pData->reste_du)) ;
}

voidCreerTPayantDialog::CmPaiement()
{
	if (SauveData())
		pComptDlg->CmPaiementTiersPayant(this);
}

//// Decremente le reste du de l'organisme du montant spécifié
//
void
CreerTPayantDialog::DecrementeResteDu(char* codeOrga, char* montant, char* unite)
{
  if (!strcmp(pData->organisme, codeOrga))
  {
   	int resteDu = atoi(pData->reste_du) ;
    int paiement = atoi(montant) ;

    string sUnit = string(pData->monnaie) ;

    if (string("LOC") == sUnit)
    {
      if (!strcmp(unite, "LOC"))
        resteDu -= paiement ;
      else
        resteDu -= pVar->getLocalFromEuro(paiement) ; 
    }
    else
    {
      if (!strcmp(unite, "EUR"))
        resteDu -= paiement ;
      else
        resteDu -= pVar->getEuroFromLocal(paiement) ;
    }

    // on replace le nouveau reste du
    //
    itoa(resteDu, pData->reste_du, 10) ;
    pResteDu->setSomme(string(pData->reste_du)) ;
  }
}

voidCreerTPayantDialog::EvOrgaChange()
{
    int	    indexOrga;

    indexOrga = pOrga->GetSelIndex();

    // on vérifie que l'organisme sélectionné n'appartient pas déjà aux tiers-payants
    // sauf s'il est lui-même dans les datas
    for (NSTPIter i = pComptDlg->pTPArray->begin(); i != pComptDlg->pTPArray->end(); i++)
    {
        if (!strcmp((*i)->_Donnees.organisme, (*pCodeOrgaArray)[indexOrga]->_Donnees.code))
        {
            if (!strcmp(pData->organisme, (*pCodeOrgaArray)[indexOrga]->_Donnees.code))
                break;
            else
            {
                erreur("Cet organisme a déjà été créé. Pour le modifier, double-cliquez sur son nom dans la liste des tiers-payants.", standardError, 0);
                for (int j = 0; j < nbCodeOrga; j++)
                {
                    if (!strcmp(pData->organisme, (*pCodeOrgaArray)[j]->_Donnees.code))
                    {
                        pOrga->SetSelIndex(j);
                        return;
                    }
                }
                pOrga->SetSelIndex(-1);
                break;
            }        }
    }
}

boolCreerTPayantDialog::InitCodeOrgaArray()
{
	NSCodeOrga* 	pCodeOrga = new NSCodeOrga(pContexte->getSuperviseur());

   pCodeOrga->lastError = pCodeOrga->open();
   if (pCodeOrga->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base CodeOrga.",standardError, 0) ;
      delete pCodeOrga;
      return false;
   }

	pCodeOrga->lastError = pCodeOrga->debut(dbiWRITELOCK);
   if ((pCodeOrga->lastError != DBIERR_NONE) && (pCodeOrga->lastError != DBIERR_EOF))
   {
   	erreur("Erreur de positionnement dans le fichier CodeOrga.db.", standardError, pCodeOrga->lastError);
      pCodeOrga->close();
      delete pCodeOrga;
      return false;
   }

   while (pCodeOrga->lastError != DBIERR_EOF)
   {
   	pCodeOrga->lastError = pCodeOrga->getRecord();
      if (pCodeOrga->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la lecture d'une fiche CodeOrga.", standardError, pCodeOrga->lastError);
         pCodeOrga->close();
         delete pCodeOrga;
         return false;
      }

      // on remplit le tableau et la combobox
      pCodeOrgaArray->push_back(new NSCodeOrgaInfo(pCodeOrga));
      pOrga->AddString(pCodeOrga->_Donnees.lib_court);
      nbCodeOrga++;

      // ... on passe au composant suivant
      pCodeOrga->lastError = pCodeOrga->suivant(dbiWRITELOCK);
      if ((pCodeOrga->lastError != DBIERR_NONE) && (pCodeOrga->lastError != DBIERR_EOF))
      {
      	erreur("Erreur d'acces à une fiche CodeOrga.", standardError, pCodeOrga->lastError);
         pCodeOrga->close();
         delete pCodeOrga;
         return false;
      }
   } // fin du while (recherche des composants images)

   // on ferme la base CARTE_SV2
   pCodeOrga->lastError = pCodeOrga->close();
   if (pCodeOrga->lastError != DBIERR_NONE)
   {
   	erreur("Erreur de fermeture du fichier CodeOrga.", standardError, pCodeOrga->lastError);
      delete pCodeOrga;
      return false;
   }

   delete pCodeOrga;
   return true;
}

boolCreerTPayantDialog::SauveData()
{
  int indexOrga = pOrga->GetSelIndex() ;
  if (indexOrga >= 0)
  {
    strcpy(pData->organisme, (*pCodeOrgaArray)[indexOrga]->_Donnees.code) ;
    strcpy(pData->libelle,   (*pCodeOrgaArray)[indexOrga]->_Donnees.lib_court) ;
  }
  else
  {
    erreur("Vous devez sélectionner un organisme de tiers-payant.", standardError, 0) ;
    return false ;
  }
  string sTexte ;
  pResteDu->getSomme(&sTexte) ;  strcpy(pData->reste_du, sTexte.c_str()) ;
  if (pLocal->GetCheck() == BF_CHECKED)
    strcpy(pData->monnaie, "LOC") ;
  if (pEuro->GetCheck() == BF_CHECKED)
    strcpy(pData->monnaie, "EUR") ;

  if (strcmp(pData->reste_du, "") != 0)
  {
    if (strcmp(pData->monnaie, "") == 0)
    {
      erreur("Vous devez préciser l'unité", standardError, 0) ;
      return false ;
    }
  }
  else
    strcpy(pData->monnaie, "LOC") ;

  // on vérifie que le total des reste du orga ne dépasse pas le reste du global
  //
  int resteDuGlobal = 0 ;

  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    resteDuGlobal = atoi(pComptDlg->pData->duFranc) - atoi(pComptDlg->pData->payeFranc);
  else
    resteDuGlobal = atoi(pComptDlg->pData->duEuro) - atoi(pComptDlg->pData->payeEuro);

  int totalResteDuOrga = 0 ;

  // on calcule la somme totale des restes-du organismes
  //
  for (NSTPIter i = pComptDlg->pTPArray->begin() ; pComptDlg->pTPArray->end() != i ; i++)
  {
    int resteDuOrga = 0 ;
    int resteDu     = atoi((*i)->_Donnees.reste_du) ;

    if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
    {
      // on calcule en francs
      if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
        resteDuOrga = resteDu ;
      else
        resteDuOrga = pVar->getLocalFromEuro(resteDu) ;
    }
    else
    {
      // on calcule en Euros
      if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
        resteDuOrga = pVar->getEuroFromLocal(resteDu) ;
      else
        resteDuOrga = resteDu ;
    }

    // si on tombe sur l'organisme en cours : on décompte son reste du
    // car on va ajouter le reste du en cours
    //
    if (!strcmp((*i)->_Donnees.organisme, pData->organisme))
      totalResteDuOrga -= resteDuOrga ;
    else
      totalResteDuOrga += resteDuOrga ;
  }

  // on ajoute au total le reste du en cours
  //
  int resteDuEnCours = atoi(pData->reste_du) ;

  if (pVar->getMonnaieRef() == MONNAIE_LOCALE)
  {
    // on calcule en francs
    if (!strcmp(pData->monnaie, "LOC"))
      totalResteDuOrga += resteDuEnCours ;
    else
      totalResteDuOrga += pVar->getLocalFromEuro(resteDuEnCours) ;
  }
  else
  {
    // on calcule en Euros
    if (!strcmp(pData->monnaie, "LOC"))
      totalResteDuOrga += pVar->getEuroFromLocal(resteDuEnCours) ; 
    else
      totalResteDuOrga += resteDuEnCours ;
  }

  if (totalResteDuOrga > resteDuGlobal)
  {
    erreur("La somme des reste-du par tiers-payants dépasse le reste-du global.", standardError, 0) ;
    return false ;
  }

  return true ;
}

voidCreerTPayantDialog::CmOk()
{
  if (SauveData())
		TDialog::CmOk() ;
}

voidCreerTPayantDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

// fin de nsfsedlg.cpp
