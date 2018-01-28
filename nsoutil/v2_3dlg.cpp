#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\button.h>
#include <owl\edit.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "nautilus\nssuper.h"
#include "partage\nsperson.h"
#include "nautilus\nsannexe.h"
#include "nsutil\nsdivers.h"
#include "partage\nsdivfct.h"
#include "nsoutil\v2_3pers.h"

#include "nsoutil\nsoutil.h"

#include "nsoutil\v2_3.rh"
#include "nsoutil\v2_3dlg.h"
#include "nsoutil\v2_3pat.h"
#include "nsoutil\v2_3corr.h"
#include "nsoutil\v2_3util.h"
#include "nsoutil\v2_3mat.h"
#include "nsoutil\v2_3reso.h"
#include "nsoutil\vcorlill.h"

// -----------------------------------------------------------------
//
//  Méthodes de NsOutilDialog
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsCvtDialog, TDialog)
    EV_CHILD_NOTIFY(IDC_CVT_PAT,  	BN_CLICKED, lanceCvtPat),
    EV_CHILD_NOTIFY(IDC_CVT_COR,  	BN_CLICKED, lanceCvtCor),
    EV_CHILD_NOTIFY(IDC_CVT_UTIL, 	BN_CLICKED, lanceCvtUti),
    EV_CHILD_NOTIFY(IDC_CVT_MAT,   	BN_CLICKED, lanceCvtMat),
    EV_CHILD_NOTIFY(IDC_CVT_PATSUP, BN_CLICKED, lanceCvtPatSup),
    EV_CHILD_NOTIFY(IDC_CVT_AUTRE,  BN_CLICKED, lanceCvtAutre),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCvtDialog::NsCvtDialog(TWindow* pere, NSContexte* pCtx)
			:NSUtilDialog(pere, pCtx, "IMPORTATION", pNSResModule)
{
    pChkPat    = new TCheckBox(this, IDC_CVT_PAT);
    pChkCor    = new TCheckBox(this, IDC_CVT_COR);
    pChkUti    = new TCheckBox(this, IDC_CVT_UTIL);
    pChkMat    = new TCheckBox(this, IDC_CVT_MAT);
    pChkPatSup = new TCheckBox(this, IDC_CVT_PATSUP);
    pChkAutre  = new TCheckBox(this, IDC_CVT_AUTRE);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsCvtDialog::~NsCvtDialog()
{
	//
	// Suppression de tous les objets
	//
	delete pChkPat;
	delete pChkCor;
    delete pChkUti;
    delete pChkMat;
    delete pChkPatSup;
    delete pChkAutre;
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCvtDialog::SetupWindow()
{
	TDialog::SetupWindow();
}

//---------------------------------------------------------------------------
//  Lancement de la conversion du fichier PATIENTS
//---------------------------------------------------------------------------
void
NsCvtDialog::lanceCvtPat()
{
#ifndef _MUE
    NsCvtPatientDlg* pNSPatCvtDlg = new NsCvtPatientDlg(pContexte->GetMainWindow(), pContexte);    pNSPatCvtDlg->Execute();
#endif
}

//---------------------------------------------------------------------------
//  Lancement de la conversion du fichier CORRESPONDANTS
//---------------------------------------------------------------------------
void
NsCvtDialog::lanceCvtCor()
{
    NsCvtCorrespDlg* pNSCorCvtDlg = new NsCvtCorrespDlg(pContexte->GetMainWindow(), pContexte);
    pNSCorCvtDlg->Execute();
}

//---------------------------------------------------------------------------
//  Lancement de la conversion du fichier UTILISATEURS
//---------------------------------------------------------------------------
void
NsCvtDialog::lanceCvtUti()
{
    NsCvtUtilisatDlg* pNSUtiCvtDlg = new NsCvtUtilisatDlg(pContexte->GetMainWindow(), pContexte);
    pNSUtiCvtDlg->Execute();
}

//---------------------------------------------------------------------------
//  Lancement de la conversion du fichier MATERIELS
//---------------------------------------------------------------------------
void
NsCvtDialog::lanceCvtMat()
{
    NsCvtMaterielDlg* pNSMatCvtDlg = new NsCvtMaterielDlg(pContexte->GetMainWindow(), pContexte);
    pNSMatCvtDlg->Execute();
}

//---------------------------------------------------------------------------
//  Lancement de la conversion d'un fichier PATIENTS supplémentaire
//---------------------------------------------------------------------------
void
NsCvtDialog::lanceCvtPatSup()
{
    // on positionne le mode "append" à true (false par défaut)
#ifndef _MUE
    NsCvtPatientDlg* pNSPatCvtDlg =        new NsCvtPatientDlg(pContexte->GetMainWindow(), pContexte, true);
    pNSPatCvtDlg->Execute();
#endif
}

//---------------------------------------------------------------------------
//  Lancement de la conversion d'un fichier PATIENTS supplémentaire
//---------------------------------------------------------------------------
void
NsCvtDialog::lanceCvtAutre()
{
    // on positionne le mode "append" à true (false par défaut)
#ifndef _MUE
    NsCvtPatientDlg* pNSPatCvtDlg =        new NsCvtPatientDlg(pContexte->GetMainWindow(), pContexte, true);
    pNSPatCvtDlg->Execute();
#endif
}

// -----------------------------------------------------------------
//
//  Méthodes de NsCvtAutreDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsCvtAutreDlg, TDialog)
    EV_CHILD_NOTIFY(IDC_CVA_LILLE, BN_CLICKED, lanceCvtLille),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCvtAutreDlg::NsCvtAutreDlg(TWindow* pere, NSContexte* pCtx)
			  :NSUtilDialog(pere, pCtx, "IMPORT_AUTRE", pNSResModule)
{
    pChkLille = new TCheckBox(this, IDC_CVA_LILLE);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsCvtAutreDlg::~NsCvtAutreDlg()
{
	//
	// Suppression de tous les objets
	//
	delete pChkLille;
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCvtAutreDlg::SetupWindow()
{
	TDialog::SetupWindow();
}

//---------------------------------------------------------------------------
//  Lancement de la conversion du fichier correspondants au format de
//  l'hôpital de Lille
//---------------------------------------------------------------------------
void
NsCvtAutreDlg::lanceCvtLille()
{
    NsCvtCorrLilleDlg* pNSCorrDlg =
    			new NsCvtCorrLilleDlg(pContexte->GetMainWindow(), pContexte);
    pNSCorrDlg->Execute();
}

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(V2_3Dialog, TDialog)
	EV_COMMAND(PAT_LANCER, CmModificationPatient),
	EV_COMMAND(COR_LANCER, CmModificationCorrespondant),
	//EV_COMMAND(CR_LANCER,  CmModificationComptesRendus),
	//EV_COMMAND(CR_INIDOC,  CmInitialisationDocuments),
END_RESPONSE_TABLE;

// -----------------------------------------------------------------
struct XFREBUFFERV2_3
{
	char CROuverture[20+1];
	char CRNumero[10+1];
	char PatOuverture[20+1];
	char PatNumero[10+1];
} XBV2_3;


// -----------------------------------------------------------------
V2_3Dialog::V2_3Dialog(TWindow* pere, NSContexte* pCtx)
			  :TDialog(pere, IDD_IMPORTATION)
{
    pContexte = pCtx;

	new TEdit(this, CR_OUV, 	sizeof(XBV2_3.CROuverture));
	new TEdit(this, CR_NBREC, 	sizeof(XBV2_3.CRNumero));
	new TEdit(this, PAT_OUV, 	sizeof(XBV2_3.PatOuverture));
	new TEdit(this, PAT_NBREC, sizeof(XBV2_3.PatNumero));
	new TButton(this, PAT_LANCER);

	SetTransferBuffer(&XBV2_3);
	memset(&XBV2_3, 0, sizeof(XBV2_3));
}

// -----------------------------------------------------------------
void V2_3Dialog::CmModificationPatient()
{
	DBIResult ErrDBI;
	char      nouveauTitre[60];

	/*V2_3Patient* pModificateur = new V2_3Patient(pSuper);
	ErrDBI = pModificateur->importe();
	if (ErrDBI != DBIERR_NONE)
		erreur("Erreur à la transformation", 0, ErrDBI);
	delete pModificateur;*/
}

// -----------------------------------------------------------------
void V2_3Dialog::CmModificationCorrespondant()
{
	DBIResult ErrDBI;
	char      nouveauTitre[60];

	/*V2_3Correspondant* pModificateur = new V2_3Correspondant(pSuper);
	ErrDBI = pModificateur->importe();
	if (ErrDBI != DBIERR_NONE)
		erreur("Erreur à la transformation", 0, ErrDBI);
	delete pModificateur;*/
}

// -----------------------------------------------------------------
/*void V2_3Dialog::CmModificationComptesRendus()
{
	DBIResult ErrDBI;
	char      nouveauTitre[60];

	V2_3CompteRendu* pModificateur = new V2_3CompteRendu(pSuper);
	ErrDBI = pModificateur->importe();
	if (ErrDBI != DBIERR_NONE)
		erreur("Erreur à la transformation", 0, ErrDBI);
	delete pModificateur;
} */

// -----------------------------------------------------------------
/*void V2_3Dialog::CmInitialisationDocuments()
{
	BOOL		 	  initialiser = FALSE;
	BOOL		 	  initPatient = FALSE;
	DBIResult 	  ErrDBI;
	UINT			  CrChamp, CrZone, CrFiche, iChemise, iDocument;
	UINT			  i, iContinuer, iReparable;
	unsigned char ptgag[4], message[31], emplacement[15], nomExam[36];
	unsigned char dateDoc[9], typeDoc[6], docDoc[4];

	// Création des objets
	NSPatient* 		pPatient = new NSPatient;
	NSCompteRendu* pC_R		= new NSCompteRendu;
	NSDocument*		pDoc		= new NSDocument;
	NSChemise*		pChemise = new NSChemise;
	NSArmoire*		pArmoire = new NSArmoire;

	// Ouverture des fichiers
	ErrDBI = pPatient->open();
	if (ErrDBI == DBIERR_NONE)
		ErrDBI = pC_R->open();
	if (ErrDBI == DBIERR_NONE)
		ErrDBI = pDoc->open();
	if (ErrDBI == DBIERR_NONE)
		ErrDBI = pChemise->open();
	if (ErrDBI == DBIERR_NONE)
		ErrDBI = pArmoire->open();

	if (ErrDBI != DBIERR_NONE)
	{
		delete pPatient;
		delete pC_R;
		delete pDoc;
		delete pChemise;
		delete pArmoire;
		return;
	}
	initialiser = TRUE;
	//
	// Création de l'armoire "comptes rendus"
	//
	pArmoire->metABlanc();
	strcpy(pArmoire->code, "001");
	strcpy(pArmoire->nom,  "Comptes rendus");
	ErrDBI = pArmoire->appendRecord();
	if (ErrDBI != DBIERR_NONE)
	{
		erreur("Erreur à la création de l\'armoire \"Comptes rendus\"", 0, ErrDBI);
		initialiser = FALSE;
	}
	//
	// pCRString est destiné à recevoir les comptes rendus
	//
	string* pCRString = new string;
	//
	// Boucle de patient en patient
	//
	if (initialiser)
	{
		ErrDBI = pPatient->debut(dbiNOLOCK);
		if (ErrDBI != DBIERR_NONE)
		{
			erreur("Impossible de se positionner sur la première fiche Patient", 0, ErrDBI);
			initialiser = FALSE;
		}
		ErrDBI = pPatient->getRecord();
		if (ErrDBI != DBIERR_NONE)
		{
			erreur("Impossible de lire la première fiche Patient", 0, ErrDBI);
			initialiser = FALSE;
		}
	}
	while (initialiser)
	{
		//
		// Vérification de la validité du champ "dernier"
		//
		initPatient = TRUE;
		iContinuer = valide_dernier(pPatient, message, &iReparable);
		if ((strcmp(pPatient->dernier, "01010010101001") == 0) ||
			 (strcmp(pPatient->dernier, "01010000101000") == 0))
			iContinuer = 1;
		//
		// Prise du dernier compte rendu
		//
		if (iContinuer == 0)
		{
			iContinuer = cr_premier(pC_R, pCRString, pPatient->nss, emplacement,
											pPatient->dernier, message, &iReparable);
			// strcpy(emplacement, pPatient->dernier);
			// *(pCRString) = "";
			// iContinuer = pC_R->prise_cr(pCRString, pPatient, emplacement,
			//									 message, &iReparable);
		}
		if (iContinuer != 0)
		{
			initPatient = FALSE;
			if ((iContinuer == 4) && (pC_R->lastError != DBIERR_NONE))
				erreur("Erreur à la recherche du compte rendu", 0, pC_R->lastError);
		}
		iChemise = 0;
		iDocument = 0;
		//
		// Boucle de compte rendu en compte rendu
		//
		while (initPatient)
		{
			//
			// Préparation des données
			//
			switch ((*pCRString)[13])
			{
				case 'G' : strcpy(nomExam, "Gastroscopie"); strcpy(typeDoc, "NG   "); break;
				case 'C' : strcpy(nomExam, "Coloscopie ");  strcpy(typeDoc, "NC   "); break;
				case 'P' : strcpy(nomExam, "Proctologie");  strcpy(typeDoc, "NP   "); break;
				case 'J' : strcpy(nomExam, "Jejunoscopie"); strcpy(typeDoc, "NJ   "); break;
				case 'W' : strcpy(nomExam, "C.P.R.E."); 	  strcpy(typeDoc, "NW   "); break;
				case 'E' : strcpy(nomExam, "Echographie");  strcpy(typeDoc, "NE   "); break;
				case 'K' : switch ((*pCRString)[14])
							  {
									case 'E' : strcpy(nomExam, "Echocardiographie"); strcpy(typeDoc, "NKE  "); break;
									case 'e' : strcpy(nomExam, "E.T.O."); 				 strcpy(typeDoc, "NKe  "); break;
									default  : strcpy(nomExam, "");						 strcpy(typeDoc, "NK?  ");
							  }
							  break;
				case 'g' : switch ((*pCRString)[14])
							  {
									case 'O' : strcpy(nomExam, "Manométrie oesophagienne"); strcpy(typeDoc, "NgO  "); break;
									case 'M' : strcpy(nomExam, "Manométrie rectale");       strcpy(typeDoc, "NgM  "); break;
									case 'P' : strcpy(nomExam, "pH métrie   "); 				  strcpy(typeDoc, "NgP  "); break;
									default  : strcpy(nomExam, "");								  strcpy(typeDoc, "Ng?  ");
							  }
							  break;
				default  : strcpy(nomExam, ""); strcpy(typeDoc, "N?   ");
			}
			for (i = 0; i < 8; i++)
				dateDoc[i] = (*pCRString)[i+5];
			dateDoc[i] = '\0';
			for (i = 0; i < 3; i++)
				docDoc[i] = (*pCRString)[i+1];
			docDoc[i] = '\0';
			//
			// Création de la chemise
			//
			iChemise++;
			pChemise->metABlanc();
			numacar(ptgag, iChemise, 3);
			strcpy(pChemise->code, pPatient->nss);
			strcat(pChemise->code, "001");
			strcat(pChemise->code, ptgag);
			strcpy(pChemise->nom, nomExam);
			strcpy(pChemise->creation, dateDoc);
			strcpy(pChemise->createur, docDoc);
			ErrDBI = pChemise->appendRecord();
			if (ErrDBI != DBIERR_NONE)
			{
				erreur("Erreur à la création de la chemise", 0, ErrDBI);
			}
			else
			{
				//
				// Création du document
				//
				iDocument++;
				pDoc->metABlanc();
				numacar(ptgag, iDocument, 3);
				strcpy(pDoc->code, 		  pPatient->nss);
				strcat(pDoc->code, 		  ptgag);
				strcpy(pDoc->localisation, pChemise->code);
				strcpy(pDoc->nom, 			  nomExam);
				strcpy(pDoc->creation, 	  dateDoc);
				strcpy(pDoc->createur, 	  docDoc);
				strcpy(pDoc->type, 		  typeDoc);
				strcpy(pDoc->fichier, 	  "GASTRO2FDB ");
				strcpy(pDoc->emplacement,  emplacement);
				ErrDBI = pDoc->appendRecord();
				if (ErrDBI != DBIERR_NONE)
				{
					erreur("Erreur à la création du document", 0, ErrDBI);
				}
			}
			//
			// Passage au compte rendu suivant
			//
			if (strcmp(emplacement, pPatient->dernier) != 0)
			{
				*(pCRString) = "";
				iContinuer = cr_suivant(pC_R, pCRString, pPatient->nss, emplacement,
												pPatient->dernier, message, &iReparable);
				if (iContinuer != 0)
					initPatient = FALSE;
			}
			else
				initPatient = FALSE;
		}
		//
		// Passage au patient suivant
		//
		ErrDBI = pPatient->suivant(dbiNOLOCK);
		if (ErrDBI != DBIERR_NONE)
		{
			erreur("Impossible de se positionner sur le Patient suivant", 0, ErrDBI);
			initialiser = FALSE;
		}
		else
		{
			ErrDBI = pPatient->getRecord();
			if (ErrDBI != DBIERR_NONE)
			{
				erreur("Impossible de lire la fiche Patient", 0, ErrDBI);
				initialiser = FALSE;
			}
		}
	}

	delete pCRString;

	pPatient->close();
	pC_R->close();
	pDoc->close();
	pChemise->close();
	pArmoire->close();

	delete pPatient;
	delete pC_R;
	delete pDoc;
	delete pChemise;
	delete pArmoire;

	CloseWindow(IDOK);
}

// unsigned int V2_3Dialog::cr_precedent(NSCompteRendu* pCR,
//													  string* pCRString,
//												  	  unsigned char *nss,
//													  unsigned char *emplacement,
//													  unsigned char *message,
// 												  unsigned int *reparable)
//---------------------------------------------------------------------------
//  Fonction  : Renvoie dans pCRString le compte rendu précédent / emplacement
//	 Variables : pCR				->	fiche compte rendu à utiliser
//					 pCRString		->	string qui reçoit le compte rendu
//					 nss				-> numéro NAUTILUS du patient
//					 emplacement	-> emplacement du CR en cours
//					 message			-> message explicatif en cas de défaut (30 car.)
//					 reparable		-> défaut réparable ou non
//	 Retour	  : 0 -> dernier est correct
//					 1 -> dernier est correct, il n'y a pas de compte rendu
//					 2 -> dernier est incorrect
//
//  Créé le 15/06/1992  Dernière mise à jour le 16/08/1994
UINT V2_3Dialog::cr_precedent(NSCompteRendu* pCR,
										string* pCRString,
										unsigned char *nss,
										unsigned char *emplacement,
										unsigned char *message,
										UINT *reparable)
{
  unsigned char *buffer[2], temporaire[256], num[4], nssCR[9];
  UINT  			 ndebfi, ndebzo, ndebch, nfinfi, nfinzo, nfinch, j, k, taille;
  UINT			 contient, aura, tourner, l, m;
  //
  //  Si c'était le dernier CR, on sort tout de suite
  //
  if (strncmp(emplacement, "0101001", 7) == 0) return(1);
  //
  //  Initialisation des "pointeurs" de fin de CR
  //
  nfinfi = 10 * (emplacement[0] - 48) + emplacement[1] - 48;
  nfinzo = 10 * (emplacement[2] - 48) + emplacement[3] - 48;
  nfinch = 100 * (emplacement[4] - 48) + 10 * (emplacement[5] - 48) + emplacement[6] - 48;
  nfinch--;
  if (nfinch == 0)
  {
	  nfinzo--;
	  nfinch = 255;
  }
  if (nfinzo == 0)
  {
	  nfinfi--;
	  if (nfinfi == 0) return(1);
	  //
	  // Recherche de la fiche ad hoc de GASTRO2F
	  //
	  numacar(num, nfinfi, 2);
	  strcpy(nssCR, nss); strcat(nssCR, num);
	  pCR->lastError = pCR->chercheClef(nssCR,
													"",
													0,
													keySEARCHEQ,
													dbiNOLOCK);
	  if (pCR->lastError == DBIERR_NONE)
			pCR->lastError = pCR->getRecord();
	  if (pCR->lastError != DBIERR_NONE)
	  {
			strcpy(message, "Pas de fiche GASTRO2F (début) ");
			return(1);
	  }
	  nfinzo = 5; nfinch = 255;
  }
  ndebfi = nfinfi;
  ndebzo = nfinzo;
  ndebch = nfinch;
  //
  //  On vérifie qu'on est sur la bonne fiche GASTRO2F
  //
  numacar(num, nfinfi, 2);
  strcpy(nssCR, nss); strcat(nssCR, num);
  if (strcmp(nssCR, pCR->nss) != 0)
  {
	  pCR->lastError = pCR->chercheClef(nssCR,
													"",
													0,
													keySEARCHEQ,
													dbiNOLOCK);
	  if (pCR->lastError == DBIERR_NONE)
			pCR->lastError = pCR->getRecord();
	  if (pCR->lastError != DBIERR_NONE)
	  {
			strcpy(message, "Pas de fiche GASTRO2F (début) ");
			return(1);
	  }
  }
  //
  // Initialisation de pCRString
  //
  *pCRString = "";
  tourner = 1;
  while (tourner) {
		  //
		  // Mise d'une chaine dans temporaire
		  //
		  j = 0;
		  while ((ndebch > 0) && (pCR->groupe[ndebzo-1][ndebch-1] != 2))
		  {
				  *pCRString = pCR->groupe[ndebzo-1][ndebch-1] + *pCRString;
				  ndebch--;
		  }
		  if ((ndebch > 0) && (pCR->groupe[ndebzo-1][ndebch-1] == 2)) {
			  *pCRString = pCR->groupe[ndebzo-1][ndebch-1] + *pCRString;
			  tourner = 0;
		  }
		  //
		  // Changement de groupe
		  //
		  if (tourner) {
			  if (ndebch == 0)
			  {
					ndebzo--;
					ndebch = 255;
			  }
			  if (ndebzo == 0)
			  {
				  ndebfi--;
				  if (ndebfi == 0) return(1);
				  numacar(num, ndebfi, 2);
				  strcpy(nssCR, nss); strcat(nssCR, num);
				  pCR->lastError = pCR->chercheClef(nssCR,
																"",
																0,
																keySEARCHEQ,
																dbiNOLOCK);
				  if (pCR->lastError == DBIERR_NONE)
						pCR->lastError = pCR->getRecord();
				  if (pCR->lastError != DBIERR_NONE)
				  {
						strcpy(message, "Pas de fiche GASTRO2F (début) ");
						return(1);
				  }
				  ndebzo = 5; ndebch = 255;
			  }
		  }
  }
  //
  //Fabrication du nouvel "emplacement"
  //
  numacar(num, ndebfi, 2); strcpy(emplacement, num);
  numacar(num, ndebzo, 2); strcat(emplacement, num);
  numacar(num, ndebch, 3); strcat(emplacement, num);
  numacar(num, nfinfi, 2); strcat(emplacement, num);
  numacar(num, nfinzo, 2); strcat(emplacement, num);
  numacar(num, nfinch, 3); strcat(emplacement, num);
  l = valide_cr(pCRString, temporaire, &k);
  if (l == 0) return(0);
  else        return(1);
}

// unsigned int V2_3Dialog::cr_premier(NSCompteRendu* pCR,
//													  string* pCRString,
//												  	  unsigned char *nss,
//													  unsigned char *emplacement,
//                                       unsigned char *dernier,
//													  unsigned char *message,
// 												  unsigned int *reparable)
//---------------------------------------------------------------------------
//  Fonction  : Renvoie dans pCRString le premier compte rendu / emplacement
//	 Variables : pCR				->	fiche compte rendu à utiliser
//					 pCRString		->	string qui reçoit le compte rendu
//					 nss				-> numéro NAUTILUS du patient
//					 emplacement	-> emplacement du CR en cours
//					 message			-> message explicatif en cas de défaut (30 car.)
//					 reparable		-> défaut réparable ou non
//	 Retour	  : 0 -> dernier est correct
//					 1 -> dernier est correct, il n'y a pas de compte rendu
//					 2 -> dernier est incorrect
//
//  Créé le 15/06/1992  Dernière mise à jour le 16/08/1994
UINT V2_3Dialog::cr_premier(NSCompteRendu* pCR,
									 string* pCRString,
									 unsigned char *nss,
									 unsigned char *emplacement,
									 unsigned char *dernier,
									 unsigned char *message,
									 UINT *reparable)
{
  unsigned char *buffer[2], temporaire[256], num[4], nssCR[9];
  UINT  			 ndebfi, ndebzo, ndebch, nfinfi, nfinzo, nfinch, j, k, taille;
  UINT			 contient, aura, tourner, l, m, continuer;
  UINT			 dernfi, dernzo, dernch;
  //
  // Recherche de la fiche ad hoc de GASTRO2F
  //
  numacar(num, nfinfi, 2);
  strcpy(nssCR, nss); strcat(nssCR, "01");
  pCR->lastError = pCR->chercheClef(nssCR,
												"",
												0,
												keySEARCHEQ,
												dbiNOLOCK);
  if (pCR->lastError == DBIERR_NONE)
		pCR->lastError = pCR->getRecord();
  if (pCR->lastError != DBIERR_NONE)
  {
		strcpy(message, "Pas de première fiche GASTRO2F");
		return(1);
  }
  ndebfi = 1;
  ndebzo = 1;
  ndebch = 1;
  nfinfi = ndebfi;
  nfinzo = ndebzo;
  nfinch = ndebch;
  //
  // Le dernier CR sert de buttoir
  //
  dernfi = 10 * (dernier[7] - 48) + dernier[8] - 48;
  dernzo = 10 * (dernier[9] - 48) + dernier[10] - 48;
  dernch = 100 * (dernier[11] - 48) + 10 * (dernier[12] - 48) + dernier[13] - 48;
  //
  // Initialisation de pCRString
  //
  *pCRString = "";
  *pCRString += pCR->groupe[nfinzo-1][nfinch-1];
  nfinch++;
  tourner = 1;
  while (tourner) {
		  //
		  // Mise d'une chaine dans temporaire
		  //
		  continuer = 1;
		  j = 0;
		  while ((nfinch < 256) &&
					(pCR->groupe[nfinzo-1][nfinch-1] != 2) &&
					(continuer))
		  {
				  *pCRString += pCR->groupe[nfinzo-1][nfinch-1];
				  if ((nfinfi == dernfi) && (nfinzo == dernzo) && (nfinch == dernch))
					  continuer = 0;
				  nfinch++;

		  }
		  if ((pCR->groupe[nfinzo-1][nfinch-1] == 2) || (!continuer))
			  tourner = 0;
		  //
		  // Changement de groupe
		  //
		  if (tourner) {
			  if (nfinch == 256)
			  {
					nfinzo++;
					nfinch = 1;
			  }
			  if (nfinzo == 6)
			  {
				  nfinfi++;
				  numacar(num, nfinfi, 2);
				  strcpy(nssCR, nss); strcat(nssCR, num);
				  pCR->lastError = pCR->chercheClef(nssCR,
																"",
																0,
																keySEARCHEQ,
																dbiNOLOCK);
				  if (pCR->lastError == DBIERR_NONE)
						pCR->lastError = pCR->getRecord();
				  if (pCR->lastError != DBIERR_NONE)
				  {
						strcpy(message, "Pas de fiche GASTRO2F (fin)   ");
						return(1);
				  }
				  nfinzo = 1; nfinch = 1;
			  }
		  }
  }
  //
  // Fabrication du nouvel "emplacement"
  //
  nfinch--;
  if (nfinch == 0)
  {
		nfinzo--;
		if (nfinzo == 0)
			nfinfi--;
  }
  numacar(num, ndebfi, 2); strcpy(emplacement, num);
  numacar(num, ndebzo, 2); strcat(emplacement, num);
  numacar(num, ndebch, 3); strcat(emplacement, num);
  numacar(num, nfinfi, 2); strcat(emplacement, num);
  numacar(num, nfinzo, 2); strcat(emplacement, num);
  numacar(num, nfinch, 3); strcat(emplacement, num);
  l = valide_cr(pCRString, temporaire, &k);
  if (l == 0) return(0);
  else        return(1);
}

// unsigned int V2_3Dialog::cr_suivant(NSCompteRendu* pCR,
//													  string* pCRString,
//												  	  unsigned char *nss,
//													  unsigned char *emplacement,
//													  unsigned char *dernier,
//													  unsigned char *message,
// 												  unsigned int *reparable)
//---------------------------------------------------------------------------
//  Fonction  : Renvoie dans pCRString le compte rendu suivant / emplacement
//	 Variables : pCR				->	fiche compte rendu à utiliser
//					 pCRString		->	string qui reçoit le compte rendu
//					 nss				-> numéro NAUTILUS du patient
//					 emplacement	-> emplacement du CR en cours
//					 message			-> message explicatif en cas de défaut (30 car.)
//					 reparable		-> défaut réparable ou non
//	 Retour	  : 0 -> dernier est correct
//					 1 -> dernier est correct, il n'y a pas de compte rendu
//					 2 -> dernier est incorrect
//
//  Créé le 15/06/1992  Dernière mise à jour le 16/08/1994
UINT V2_3Dialog::cr_suivant(NSCompteRendu* pCR,
									 string* pCRString,
									 unsigned char *nss,
									 unsigned char *emplacement,
									 unsigned char *dernier,
									 unsigned char *message,
									 UINT *reparable)
{
  unsigned char *buffer[2], temporaire[256], num[4], nssCR[9];
  UINT  			 ndebfi, ndebzo, ndebch, nfinfi, nfinzo, nfinch, j, k, taille;
  UINT			 contient, aura, tourner, l, m, continuer;
  UINT			 dernfi, dernzo, dernch;
  //
  //  Initialisation des "pointeurs" de début de CR
  //
  ndebfi = 10 * (emplacement[7] - 48) + emplacement[8] - 48;
  ndebzo = 10 * (emplacement[9] - 48) + emplacement[10] - 48;
  ndebch = 100 * (emplacement[11] - 48) + 10 * (emplacement[12] - 48) + emplacement[13] - 48;
  //
  // Le dernier CR sert de buttoir
  //
  dernfi = 10 * (dernier[7] - 48) + dernier[8] - 48;
  dernzo = 10 * (dernier[9] - 48) + dernier[10] - 48;
  dernch = 100 * (dernier[11] - 48) + 10 * (dernier[12] - 48) + dernier[13] - 48;
  if ( (ndebfi > dernfi) ||
		((ndebfi == dernfi) && (ndebzo > dernzo)) ||
		((ndebfi == dernfi) && (ndebzo == dernzo) && (ndebch > dernch)))
	  return(1);
  //
  // On passe au caractère qui suit
  //
  ndebch++;
  if (ndebch == 256)
  {
	  ndebzo++;
	  ndebch = 1;
  }
  if (ndebzo == 6)
  {
	  ndebfi++;
	  //
	  // Recherche de la fiche ad hoc de GASTRO2F
	  //
	  numacar(num, ndebfi, 2);
	  strcpy(nssCR, nss); strcat(nssCR, num);
	  pCR->lastError = pCR->chercheClef(nssCR,
													"",
													0,
													keySEARCHEQ,
													dbiNOLOCK);
	  if (pCR->lastError == DBIERR_NONE)
			pCR->lastError = pCR->getRecord();
	  if (pCR->lastError != DBIERR_NONE)
	  {
			strcpy(message, "Pas de fiche GASTRO2F (début) ");
			return(1);
	  }
	  ndebzo = 1; ndebch = 1;
  }
  nfinfi = ndebfi;
  nfinzo = ndebzo;
  nfinch = ndebch;
  //
  //  On vérifie qu'on est sur la bonne fiche GASTRO2F
  //
  numacar(num, ndebfi, 2);
  strcpy(nssCR, nss); strcat(nssCR, num);
  if (strcmp(nssCR, pCR->nss) != 0)
  {
	  pCR->lastError = pCR->chercheClef(nssCR,
													"",
													0,
													keySEARCHEQ,
													dbiNOLOCK);
	  if (pCR->lastError == DBIERR_NONE)
			pCR->lastError = pCR->getRecord();
	  if (pCR->lastError != DBIERR_NONE)
	  {
			strcpy(message, "Pas de fiche GASTRO2F (début) ");
			return(1);
	  }
  }
  //
  // Initialisation de pCRString
  //
  *pCRString = "";
  *pCRString += pCR->groupe[nfinzo-1][nfinch-1];
  nfinch++;
  tourner = 1;
  while (tourner) {
		  //
		  // Mise d'une chaine dans temporaire
		  //
		  continuer = 1;
		  j = 0;
		  while ((nfinch < 256) &&
					(pCR->groupe[nfinzo-1][nfinch-1] != 2) &&
					(continuer))
		  {
				  *pCRString += pCR->groupe[nfinzo-1][nfinch-1];
				  if ((nfinfi == dernfi) && (nfinzo == dernzo) && (nfinch == dernch))
					  continuer = 0;
				  nfinch++;
		  }
		  if ((pCR->groupe[nfinzo-1][nfinch-1] == 2) || (!continuer))
			  tourner = 0;
		  //
		  // Changement de groupe
		  //
		  if (tourner) {
			  if (nfinch == 256)
			  {
					nfinzo++;
					nfinch = 1;
			  }
			  if (nfinzo == 6)
			  {
				  nfinfi++;
				  numacar(num, nfinfi, 2);
				  strcpy(nssCR, nss); strcat(nssCR, num);
				  pCR->lastError = pCR->chercheClef(nssCR,
																"",
																0,
																keySEARCHEQ,
																dbiNOLOCK);
				  if (pCR->lastError == DBIERR_NONE)
						pCR->lastError = pCR->getRecord();
				  if (pCR->lastError != DBIERR_NONE)
				  {
						strcpy(message, "Pas de fiche GASTRO2F (fin)   ");
						return(1);
				  }
				  nfinzo = 1; nfinch = 1;
			  }
		  }
  }
  //
  // Fabrication du nouvel "emplacement"
  //
  nfinch--;
  if (nfinch == 0)
  {
		nfinzo--;
		if (nfinzo == 0)
			nfinfi--;
  }
  numacar(num, ndebfi, 2); strcpy(emplacement, num);
  numacar(num, ndebzo, 2); strcat(emplacement, num);
  numacar(num, ndebch, 3); strcat(emplacement, num);
  numacar(num, nfinfi, 2); strcat(emplacement, num);
  numacar(num, nfinzo, 2); strcat(emplacement, num);
  numacar(num, nfinch, 3); strcat(emplacement, num);
  l = valide_cr(pCRString, temporaire, &k);
  if (l == 0) return(0);
  else        return(1);
}
*/
