#include <owl\olemdifr.h>
#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>
// #include <mem.h>
// #include <io.h>
// #include <fcntl.h>

#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\v2_3.rh"
#include "nsoutil\v2_3pat.h"
#include "nsbb\nsednum.h"
#include "nautilus\nsdocref.h"
#ifndef _MUE
DBIResult NSPatBug::open()
{
  char tableName[] = "PATBUG.DB";
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
  return(lastError);
}#endif

#ifndef _MUE
// -----------------------------------------------------------------//
//  Méthodes de NsCvtPatientDlg
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NsCvtPatientDlg, TDialog)   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),
   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCvtPatientDlg::NsCvtPatientDlg(TWindow* pere, NSContexte* pCtx, bool bModeAppend)
				  	 :NSUtilDialog(pere, pCtx, "CVT_PAT", pNSResModule)
{
    pEditNSS 	= new TEdit(this, IDC_CVT_NSS);
    pEditCode 	= new TEdit(this, IDC_CVT_CODE);
    pEditNom 	= new TEdit(this, IDC_CVT_NOM);
    pEditPrenom = new TEdit(this, IDC_CVT_PRENOM);

    pPatImport  = new NSPatient(pContexte);
  	pPcoImport  = new NSPatCor(pContexte);
  	pAdrImport  = new NSAdresses(pContexte);
    pCLiImport  = new NSCorLibre(pContexte);

    pPatBug     = new NSPatBug(pContexte);

    pNewChemise  = new NSChemise(pContexte);
    pNewDocument = new NSDocument(pContexte);
    pData 		 = new NSPatPaDat(pContexte);
    pLoca 		 = new NSPatPaLoc(pContexte);

    pBufPat     = new V2_3Patient;

    cvtPossible = true;

    bAppend = bModeAppend;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsCvtPatientDlg::~NsCvtPatientDlg()
{
	//
	// Suppression de tous les objets
	//
	delete pEditNSS;
   delete pEditCode;
   delete pEditNom;
   delete pEditPrenom;

   pPatImport->close();
   delete pPatImport;
   pPcoImport->close();
  	delete pPcoImport;
   pAdrImport->close();
  	delete pAdrImport;
   pCLiImport->close();
   delete pCLiImport;
   pPatBug->close();
   delete pPatBug;

   pNewChemise->close();
   delete pNewChemise;
   pNewDocument->close();
   delete pNewDocument;
   pData->close();
   delete pData;
   pLoca->close();
   delete pLoca;

   delete pBufPat;

   if (infile != NULL)
   	fclose(infile);
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCvtPatientDlg::SetupWindow()
{
    TDialog::SetupWindow();
    //
    // Ouverture du fichier d'importation
    //
    infile = fopen("PATFI.DAT", "r");
  	if (infile == NULL)
    {
   	    erreur("Fichier d'importation non trouvé", 0, 0);
	  	cvtPossible = false;
  	}
    if (cvtPossible)
	  	setmode(fileno(infile), O_BINARY);

    entree = getc(infile);
  	if (entree == EOF)
    {
   	    erreur("Le fichier d'importation est vide", 0, 0);
	  	cvtPossible = false;
    }
	//
    // Ouverture des tables
    //
    if (cvtPossible)
    {
   	    pPatImport->lastError = pPatImport->open();
  		if (pPatImport->lastError != DBIERR_NONE)
   	    {
      	    erreur("Impossible d'ouvrir le fichier patient", 0, pPatImport->lastError);
            cvtPossible = false;
   	    }

        // cas de la fusion d'une autre base patients
        if (cvtPossible && bAppend)
        {
            if (!initLastCodes())
            {
                erreur("Impossible de récupérer les derniers codes patients.", 0, 0);
                cvtPossible = false;
            }
        }

        pPatBug->lastError = pPatBug->open();
  		if (pPatBug->lastError != DBIERR_NONE)
      	erreur("Impossible d'ouvrir le fichier des patients doublons", 0, pPatBug->lastError);
   }
   if (cvtPossible)
   {
  		pPcoImport->lastError = pPcoImport->open();
  		if (pPcoImport->lastError != DBIERR_NONE)
   	{
      	erreur("Impossible d'ouvrir le fichier patcor", 0, pPcoImport->lastError);
         cvtPossible = false;
		}
   }
   if (cvtPossible)
   {
  		pAdrImport->lastError = pAdrImport->open();
  		if (pAdrImport->lastError != DBIERR_NONE)
   	{
      	erreur("Impossible d'ouvrir le fichier des adresses", 0, pAdrImport->lastError);
         cvtPossible = false;
      }
   }
   if (cvtPossible)
   {
  		pCLiImport->lastError = pCLiImport->open();
  		if (pCLiImport->lastError != DBIERR_NONE)
   	{
      	erreur("Impossible d'ouvrir le fichier des correspondants libres", 0, pCLiImport->lastError);
         cvtPossible = false;
      }
   }
   if (cvtPossible)
   {
   	pNewChemise->lastError = pNewChemise->open();
   	if (pNewChemise->lastError != DBIERR_NONE)
   	{
     		erreur("Il est impossible d'ouvrir le fichier des Chemises.", 0, pNewChemise->lastError);
      	cvtPossible = false;
      }
   }
   if (cvtPossible)
   {
   	pNewDocument->lastError = pNewDocument->open();
   	if (pNewDocument->lastError != DBIERR_NONE)
   	{
   		erreur("Il est impossible d'ouvrir le fichier des Documents.", 0, pNewDocument->lastError);
         cvtPossible = false;
   	}
   }
   if (cvtPossible)
   {
   	pData->lastError = pData->open();
      if (pData->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à l'ouverture du fichier de données.", 0, pData->lastError);
         cvtPossible = false;
      }
   }
   if (cvtPossible)
   {
   	pLoca->lastError = pLoca->open();
		if (pLoca->lastError != DBIERR_NONE)
		{
			erreur("Erreur à l'ouverture du fichier de localisations.", 0, pLoca->lastError);
         cvtPossible = false;
		}
   }
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsCvtPatientDlg::demarreCvt()
{
   donne_date_duJour(creation);

   cvtTourner = true;

	while ((cvtTourner) && (cvtPossible))
  	{
      initAffichage();

   	cvtPossible = emplisBuffer();
		pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages();
      //
      // Traite les enregistrements ordinaires
      //
      if (cvtPossible)
      	cvtPossible = traiteBuffer();
      //
      // Traite le dernier enregistrement
      //
      else if (entree == EOF)
      	traiteBuffer();
	}
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsCvtPatientDlg::stoppeCvt()
{
   cvtTourner = false;
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsCvtPatientDlg::emplisBuffer()
{
	bool tourner = true;
   int  champ, taille_champ, j;
   char buffer[100];

	for (champ = 0; (champ < 19) && (tourner); champ++)
   {
   	switch (champ)
      {
      	case  0 : taille_champ =  6; break;
         case  1 : taille_champ = 14; break;
         case  2 :
         case  3 : taille_champ = 25; break;
         case  4 : taille_champ = 20; break;
         case  5 : taille_champ = 50; break;
         case  6 : taille_champ = 25; break;
         case  7 :
         case  8 :
         case  9 :
         case 10 :
         case 11 :
         case 12 : taille_champ =  3; break;
         case 13 :
         case 14 :
         case 15 : taille_champ = 25; break;
         case 16 : taille_champ =  8; break;
         case 17 : taille_champ = 15; break;
         case 18 : taille_champ = 20; break;
      }

      for (j = 0; (j < taille_champ) && (entree != EOF) ; j++)
      {
         if (entree >= 32)
      		buffer[j] = entree;
         else
         	buffer[j] = 32;

         entree = getc(infile);
      }
      buffer[j] = '\0';
      if (entree == EOF)
      	tourner = false;

      OemToAnsi(buffer, buffer);

      switch (champ)
      {
      	case  0 : strcpy(pBufPat->V2nss,       buffer);
                   pEditNSS->SetText(buffer);
         			 break;
         case  1 : strcpy(pBufPat->V2dernier,   buffer); break;
         case  2 : strcpy(pBufPat->V2nom, 	   buffer);
                   pEditNom->SetText(buffer);
         			 break;
         case  3 : strcpy(pBufPat->V2prenom,    buffer);
         			 pEditPrenom->SetText(buffer);
                   break;
         case  4 : strcpy(pBufPat->V2convoc,    buffer); break;
         case  5 : strcpy(pBufPat->V2adresse,   buffer); break;
         case  6 : strcpy(pBufPat->V2ville,     buffer); break;
         case  7 :
         case  8 :
         case  9 :
         case 10 :
         case 11 :
         case 12 : strcpy(pBufPat->V2medic[champ-7], buffer); break;
         case 13 : strcpy(pBufPat->V2medinom,   buffer); break;
         case 14 : strcpy(pBufPat->V2medidres,  buffer); break;
         case 15 : strcpy(pBufPat->V2mediville, buffer); break;
         case 16 : strcpy(pBufPat->V2naissance, buffer); break;
         case 17 : strcpy(pBufPat->V2telephone, buffer); break;
         case 18 : strcpy(pBufPat->V2code,      buffer);
                   pEditCode->SetText(buffer);
         		 	 break;
      }
   }

   return tourner;
}

//---------------------------------------------------------------------------
//  Dispatche le buffer dans les tables
//---------------------------------------------------------------------------
bool
NsCvtPatientDlg::traiteBuffer()
{
    int i, j;
    int idRet;
    string sCode;
    string nom, prenom, sexe1, date1, titre1, sexe2, date2, titre2;

	//
    // Mise à jour de la fiche patient version 98
    //
    pPatImport->pDonnees->metAZero();

    if (bAppend && existePatient(nom,prenom,sexe1,date1,sexe2,date2))
    {
        char msg[255];

        // s'il s'agit d'un pur doublon : on skippe
        if ((date1 == date2) && (sexe1 == sexe2))
            return true;

        if (sexe1[0] == '1')
            titre1 = "M.";
        else
            titre1 = "Mme.";

        if (sexe2[0] == '1')
            titre2 = "M.";
        else
            titre2 = "Mme.";

        sprintf(msg, "Attention, %s %s %s né(e) le %s existe déjà. Voulez-vous tout de même créer %s %s %s né(e) le %s ? (Cliquez sur Annuler pour arrêter la fusion)",
                titre1.c_str(), nom.c_str(), prenom.c_str(), date1.c_str(),
                titre2.c_str(), nom.c_str(), prenom.c_str(), date2.c_str());

        idRet = MessageBox(msg, "Message Nautilus", MB_YESNOCANCEL);

        if (idRet == IDCANCEL)
            return false;
        else if (idRet == IDNO)
            return true;
    }

    if (bAppend)
    {
        if (pBufPat->V2nss[0] == '1')
        {
            pPatImport->IncrementeCode(&cptHomme);
            strcpy(pPatImport->pDonnees->nss, cptHomme.c_str());
        }
        else
        {
            pPatImport->IncrementeCode(&cptFemme);
            strcpy(pPatImport->pDonnees->nss, cptFemme.c_str());
        }
    }
    else // cas normal : on récupère le code de l'ancienne base
        strcpy(pPatImport->pDonnees->nss,    pBufPat->V2nss);
    //
    // Si le NSS est blanc, on ne traite pas la fiche
    //
    if (strcmp(pPatImport->pDonnees->nss, "      ") == 0)
   	    return true;

    strcpy(pPatImport->pDonnees->nom,    pBufPat->V2nom);
    ote_blancs(pPatImport->pDonnees->nom);
    strcpy(pPatImport->pDonnees->prenom, pBufPat->V2prenom);
    ote_blancs(pPatImport->pDonnees->prenom);
    strcpy(pPatImport->pDonnees->code,   pBufPat->V2code);
    ote_blancs(pPatImport->pDonnees->code);
    strcpy(pPatImport->pDonnees->convoc, pBufPat->V2convoc);
    pPatImport->pDonnees->sexe[0] = pBufPat->V2nss[0];
    strcpy(pPatImport->pDonnees->naissance, pBufPat->V2naissance);
    ote_blancs(pPatImport->pDonnees->naissance);
    //
    // Création d'une adresse version 98
    //
    pAdrImport->pDonnees->metAZero();
    for (i = 0;  i < 25; i++) pAdrImport->pDonnees->adresse1[i]  = pBufPat->V2adresse[i];
    ote_blancs(pAdrImport->pDonnees->adresse1);
    for (i = 0;  i < 25; i++) pAdrImport->pDonnees->adresse2[i]  = pBufPat->V2adresse[i+25];
    ote_blancs(pAdrImport->pDonnees->adresse2);
    for (i = 0;  i <  5; i++) pAdrImport->pDonnees->code_post[i] = pBufPat->V2ville[i+20];
    ote_blancs(pAdrImport->pDonnees->code_post);
    for (i = 0;  i < 20; i++) pAdrImport->pDonnees->ville[i] 	 = pBufPat->V2ville[i];
    ote_blancs(pAdrImport->pDonnees->ville);
    strcpy(pAdrImport->pDonnees->telephone, pBufPat->V2telephone);

    if ((pAdrImport->pDonnees->adresse1[0] != '\0') ||
   	    (pAdrImport->pDonnees->adresse2[0] != '\0'))
    {
   	    if (pAdrImport->LastCode(&sCode,tCodeAdresse))
        {
      	    strcpy(pAdrImport->pDonnees->code, sCode.c_str());

            pAdrImport->lastError = pAdrImport->appendRecord();
            if (pAdrImport->lastError != DBIERR_NONE)
            {
         	    erreur("Erreur à l'ajout de la fiche patient", 0, pAdrImport->lastError);
                return false;
            }

            strcpy(pPatImport->pDonnees->adresse, pAdrImport->pDonnees->code);
            pPatImport->lastError = pPatImport->appendRecord();
            if (pPatImport->lastError != DBIERR_NONE)
            {
         	    erreur("Erreur à l'ajout de la fiche patient", 0, pPatImport->lastError);
                *(pPatBug->pDonnees) = *(pPatImport->pDonnees);
                pPatBug->lastError = pPatBug->appendRecord();
                if (pPatBug->lastError != DBIERR_NONE)
                {
                    erreur("Erreur à l'ajout de la fiche de récupération", 0, pPatBug->lastError);
                    return false;
                }
                return true;
            }
        }
        else
        {
      	    erreur("Impossible de trouver un nouveau code d'adresse", 0, pAdrImport->lastError);
            return false;
        }
    }
    else
    {
   	    pPatImport->lastError = pPatImport->appendRecord();
        if (pPatImport->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à l'ajout de la fiche patient", 0, pPatImport->lastError);
            *(pPatBug->pDonnees) = *(pPatImport->pDonnees);
            pPatBug->lastError = pPatBug->appendRecord();
            if (pPatBug->lastError != DBIERR_NONE)
            {
                erreur("Erreur à l'ajout de la fiche de récupération", 0, pPatBug->lastError);
                return false;
            }
            return true;
        }
    }
    //
    // Mise à jour du fichier PATCOR
    //
    // Pour l'instant on ne met pas à jour les PatCor en mode append
    if (!bAppend)
    {
        for (j = 0; (j < 6) && (strcmp(pBufPat->V2medic[j], "   ") != 0); j++)
        {
            int k = 0;
		    for (; (k < j) && (strcmp(pBufPat->V2medic[j], pBufPat->V2medic[k]) != 0); k++);
   	        //pPcoImport->metAZero();
            if (k == j)
            {
      	        strcpy(pPcoImport->pDonnees->patient,       pPatImport->pDonnees->nss);
      	        strcpy(pPcoImport->pDonnees->correspondant, pBufPat->V2medic[j]);
      	        strcpy(pPcoImport->pDonnees->fonction,      "");
      	        //pPcoImport->rang = j;
      	        pPcoImport->lastError = pPcoImport->appendRecord();
      	        if (pPcoImport->lastError != DBIERR_NONE)
      	        {
      		        erreur("Erreur à l'ajout de la fiche patcor", 0, pPcoImport->lastError);
         	        return false;
      	        }
            }
        }
        //
        // Mise à jour du correspondant libre
        //
        if (strcmp(pBufPat->V2medinom, "                         ") != 0)
        {
   	        pCLiImport->pDonnees->metAZero();
            strcpy(pCLiImport->pDonnees->nss, pPatImport->pDonnees->nss);
            strcpy(pCLiImport->pDonnees->ligne1, pBufPat->V2medinom);
            strcpy(pCLiImport->pDonnees->ligne2, pBufPat->V2medidres);
            strcpy(pCLiImport->pDonnees->ligne3, pBufPat->V2mediville);
            pCLiImport->lastError = pCLiImport->appendRecord();
            if (pCLiImport->lastError != DBIERR_NONE)
            {
      	        erreur("Erreur à l'ajout de la fiche correspondant libre", 0, pCLiImport->lastError);
                return false;
            }
        }
    }

    //
    // ----------- CREATION DES DOCUMENTS, CHEMISES... -------
    //
    // Création d'une chemise par défaut
    //
    pNewChemise->pDonnees->metAZero();

    char codeChemise[CHE_CODE_LEN + 1];
    strcpy(codeChemise, pPatImport->pDonnees->nss);
    strcat(codeChemise, "001");

    strcpy(pNewChemise->pDonnees->code,     codeChemise);
    strcpy(pNewChemise->pDonnees->nom, 	    "défaut");
    strcpy(pNewChemise->pDonnees->creation, creation);
    strcpy(pNewChemise->pDonnees->acces, 	"");
    strcpy(pNewChemise->pDonnees->createur, pContexte->getUtilisateur()->pDonnees->indice);

    pNewChemise->lastError = pNewChemise->appendRecord();
    if (pNewChemise->lastError != DBIERR_NONE)
    {
   	    erreur("Il est impossible d'ajouter une nouvelle Chemise.", 0, pNewChemise->lastError);
        return false;
    }
	//
    // Création d'une fiche de synthèse
    //
    // Initialisation de la fiche document
    //
    pNewDocument->pDonnees->metAZero();

    // Patient : Patient en cours
    strcpy(pNewDocument->pDonnees->codePatient, pPatImport->pDonnees->nss);
    // Code : par définition "0000"
    strcpy(pNewDocument->pDonnees->codeDocument,
                     (string(DOC_CODE_DOCUM_LEN, '0')).c_str());

    //
    // Niveau de sécurité pour l'accès : Celui de l'utilisateur
    strcpy(pNewDocument->pDonnees->acces,
                                        pContexte->getUtilisateur()->pDonnees->groupe);
    // Créateur : Utilisateur en cours
    strcpy(pNewDocument->pDonnees->createur,
                                        pContexte->getUtilisateur()->pDonnees->indice);
    // Type de document : Fiche de consultation
    strcpy(pNewDocument->pDonnees->type, "CS030");
    // Intêret 5 = moyen
    strcpy(pNewDocument->pDonnees->interet, "5");
    // Date de creation = date du jour
    //char creation[DOC_CREATION_LEN + 1];
    //donne_date_duJour(creation);
    strcpy(pNewDocument->pDonnees->creation, creation);
    // visible est à 1 quand on référence un document
    strcpy(pNewDocument->pDonnees->visible, "1");
    // Copie du nom du document par défaut
    strncpy(pNewDocument->pDonnees->nom, "Synthèse", DOC_NOM_LEN);
    //
    // Ajout de la fiche au fichier des Documents
	//
	pNewDocument->lastError = pNewDocument->appendRecord();
	if (pNewDocument->lastError != DBIERR_NONE)
    {
        erreur("Il est impossible de mettre à jour la base des Documents.", 0,
                             pNewDocument->lastError);
		return false;
    }
	//
    // Enregistrement de la partie "données" de la patpatho
    //
    pData->pDonnees->metAZero();
    strcpy(pData->pDonnees->codePatient,  pPatImport->pDonnees->nss);
    strcpy(pData->pDonnees->codeDocument, "00000");
    strcpy(pData->pDonnees->noeud, 		  "0001");
    strcpy(pData->pDonnees->type, 		  "1");
    strcpy(pData->pDonnees->lexique, 	  "ZSYNT1");
    //strcpy(pData->pDonnees->complement,   "");
    //strcpy(pData->pDonnees->certitude, 	 "");

    //strcpy(pData->pDonnees->pluriel, 	  "");
    pData->lastError = pData->appendRecord();
	if (pData->lastError != DBIERR_NONE)
    {
        erreur("Il est impossible de mettre à jour la base des données.", 0,
                             pData->lastError);
		return false;
    }
    //
    // Enregistrement de la partie "données" de la patpatho
    //
    strcpy(pLoca->pDonnees->codePatient,  	  pPatImport->pDonnees->nss);
    strcpy(pLoca->pDonnees->codeDocument, 	  "00000");
    strcpy(pLoca->pDonnees->transaction,  	  "00000");
    strcpy(pLoca->pDonnees->noeud,		  	  "0001");
    strcpy(pLoca->pDonnees->codeLocalisation, "00000000");
    strcpy(pLoca->pDonnees->visible, 		  "");
    strcpy(pLoca->pDonnees->interet, 	      "A");
    pLoca->lastError = pLoca->appendRecord();
    if (pLoca->lastError != DBIERR_NONE)
    {
        erreur("Il est impossible de mettre à jour la base des données.", 0,
                             pLoca->lastError);
		return false;
    }

    return true;
}

bool
NsCvtPatientDlg::initLastCodes()
{
    cptHomme = string("200000");

    pPatImport->lastError = pPatImport->chercheClef(&cptHomme,
                                                        "",
                                                        0,
                                                        keySEARCHGEQ,
                                                        dbiWRITELOCK);

    if ((pPatImport->lastError != DBIERR_NONE) && (pPatImport->lastError != DBIERR_EOF))
    {
        erreur("Erreur à la recherche dans la base patients.", 0, pPatImport->lastError);
        return false;
    }

    // on est soit après le dernier soit sur EOF
    // ==> on recule d'un cran
    pPatImport->lastError = pPatImport->precedent(dbiWRITELOCK);
    if (pPatImport->lastError != DBIERR_NONE)
    {
        erreur("Erreur à l'accès à la fiche patient précédente.", 0, pPatImport->lastError);
        return false;
    }

    // on récupère le dernier patient Homme
    pPatImport->lastError = pPatImport->getRecord();
    if (pPatImport->lastError != DBIERR_NONE)
    {
        erreur("Erreur à la lecture dans la base patients.", 0, pPatImport->lastError);
        return false;
    }

    if (pPatImport->pDonnees->nss[0] == '1')
        cptHomme = string(pPatImport->pDonnees->nss);
    else
    {
        erreur("Impossible de trouver le dernier patient masculin.", 0, 0);
        return false;
    }

    cptFemme = string("300000");

    pPatImport->lastError = pPatImport->chercheClef(&cptFemme,
                                                        "",
                                                        0,
                                                        keySEARCHGEQ,
                                                        dbiWRITELOCK);

    if ((pPatImport->lastError != DBIERR_NONE) && (pPatImport->lastError != DBIERR_EOF))
    {
        erreur("Erreur à la recherche dans la base patients.", 0, pPatImport->lastError);
        return false;
    }

    // on est soit après le dernier soit sur EOF
    // ==> on recule d'un cran
    pPatImport->lastError = pPatImport->precedent(dbiWRITELOCK);
    if (pPatImport->lastError != DBIERR_NONE)
    {
        erreur("Erreur à l'accès à la fiche patient précédente.", 0, pPatImport->lastError);
        return false;
    }

    // on récupère le dernier patient Homme
    pPatImport->lastError = pPatImport->getRecord();
    if (pPatImport->lastError != DBIERR_NONE)
    {
        erreur("Erreur à la lecture dans la base patients.", 0, pPatImport->lastError);
        return false;
    }

    if (pPatImport->pDonnees->nss[0] == '2')
        cptFemme = string(pPatImport->pDonnees->nss);
    else
    {
        erreur("Impossible de trouver le dernier patient féminin.", 0, 0);
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//  Recherche l'existence d'un doublon
//---------------------------------------------------------------------------
bool
NsCvtPatientDlg::existePatient(string& nom, string& prenom, string& sexe1, string& date1, string& sexe2, string& date2)
{
    DBIResult   ErrDBI;
    CURProps    curProps;
    Byte*       pIndexRec;
    char        cDate1[10] = "", cDate2[10] = "";

    nom = ""; prenom = "";
    sexe1 = ""; date1 = "";
    sexe2 = ""; date2 = "";

    // on recherche ce patient dans la base pour
    // vérifier qu'il n'existe pas déjà
    ErrDBI = DbiGetCursorProps(pPatImport->PrendCurseur(), curProps);
    pIndexRec = new Byte[curProps.iRecBufSize];
    memset(pIndexRec, 0, curProps.iRecBufSize);
    DbiPutField(pPatImport->PrendCurseur(), PAT_NOM_FIELD, pIndexRec, (Byte*)pBufPat->V2nom);
    DbiPutField(pPatImport->PrendCurseur(), PAT_PRENOM_FIELD, pIndexRec, (Byte*)pBufPat->V2prenom);

    // on lance cette fois une recherche exacte
    ErrDBI = pPatImport->chercheClefComposite("NOM_PRENOM",
  												NODEFAULTINDEX,
 												keySEARCHEQ,
												dbiWRITELOCK,
                              	                pIndexRec);
    delete[] pIndexRec;

    if (ErrDBI == DBIERR_NONE)
    {
        ErrDBI = pPatImport->getRecord();
        if (ErrDBI != DBIERR_NONE)
        {
            erreur("Erreur à la lecture dans la base patient.db", 0, ErrDBI);
            return false;
        }

        donne_date(pPatImport->pDonnees->naissance, cDate1, sLang);
        donne_date(pBufPat->V2naissance, cDate2, sLang);

        nom = string(pPatImport->pDonnees->nom);
        prenom = string(pPatImport->pDonnees->prenom);
        sexe1 = string(pPatImport->pDonnees->sexe);
        date1 = string(cDate1);
        sexe2 = string(1, pBufPat->V2nss[0]);
        date2 = string(cDate2);

        return true;
    }
    else if (ErrDBI != DBIERR_RECNOTFOUND)
        erreur("Erreur à la recherche dans la base patient.db", 0, ErrDBI);

    return false;
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsCvtPatientDlg::initAffichage()
{
	pEditNSS->SetText("");
   pEditCode->SetText("");
   pEditNom->SetText("");
   pEditPrenom->SetText("");
}#endif

#ifndef _MUE
// -----------------------------------------------------------------//
//  Méthodes de NsReparePatientDlg
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NsReparePatientDlg, TDialog)    EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),
    EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsReparePatientDlg::NsReparePatientDlg(TWindow* pere, NSContexte* pCtx)
                   :NSUtilDialog(pere, pCtx, "CVT_PAT", pNSResModule)
{
    pEditNSS 	= new TEdit(this, IDC_CVT_NSS);
    pEditCode 	= new TEdit(this, IDC_CVT_CODE);
    pEditNom 	= new TEdit(this, IDC_CVT_NOM);
    pEditPrenom = new TEdit(this, IDC_CVT_PRENOM);

    pPatImport  = new NSPatient(pContexte);

    pBufPat     = new NSPatientData(pContexte) ;

    cvtPossible = true;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsReparePatientDlg::~NsReparePatientDlg()
{
	//
	// Suppression de tous les objets
	//
	delete pEditNSS;
    delete pEditCode;
    delete pEditNom;
    delete pEditPrenom;

    pPatImport->close();
    delete pPatImport;

    delete pBufPat;

    if (infile != NULL)
   	    fclose(infile);
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsReparePatientDlg::SetupWindow()
{
    TDialog::SetupWindow();
    //
    // Ouverture du fichier d'importation
    //
    infile = fopen("BADPAT.DB", "r");
  	if (infile == NULL)
    {
   	    erreur("Fichier BADPAT.DB non trouvé", 0, 0);
	  	cvtPossible = false;
        return;
  	}
    if (cvtPossible)
	  	setmode(fileno(infile), O_BINARY);

    entree = getc(infile);
  	if (entree == EOF)
    {
   	    erreur("Le fichier BADPAT.DB est vide", 0, 0);
	  	cvtPossible = false;
        return;
    }
	//
    // Ouverture des tables
    //
    if (cvtPossible)
    {
   	    pPatImport->lastError = pPatImport->open();
  		if (pPatImport->lastError != DBIERR_NONE)
   	    {
      	    erreur("Impossible d'ouvrir le fichier patient", 0, pPatImport->lastError);
            cvtPossible = false;
   	    }
    }
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsReparePatientDlg::demarreCvt()
{
    donne_date_duJour(creation);

    cvtTourner = true;

    cvtTourner = vaAuPremier();

    while ((cvtTourner) && (cvtPossible))
  	{
        initAffichage();

   	    cvtPossible = emplisBuffer();
		pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages();
        //
        // Traite les enregistrements ordinaires
        //
        if (cvtPossible)
      	    cvtPossible = traiteBuffer();
        //
        // Traite le dernier enregistrement
        //
        else if (entree == EOF)
      	    traiteBuffer();
    }
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsReparePatientDlg::stoppeCvt()
{
    cvtTourner = false;
}


//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsReparePatientDlg::vaAuPremier()
{
    // On cherche un enregistrement du type 10000X ou 20000X
    bool tourner = true;
    while (tourner)
    {
        if ((entree == '1') || (entree == '2'))
        {
            string sCode = string(1, char(entree));
            for (int i = 0; i < 4; i++)
            {
                entree = getc(infile);
                sCode += string(1, char(entree));
                if (entree == EOF)
      	            return false;
            }
            if ((sCode == "10000") || (sCode == "20000"))
            {
                entree = getc(infile);
                if (entree == EOF)
      	            return false;
                sCode += string(1, char(entree));
                strcpy(pBufPat->nss, sCode.c_str());

                entree = getc(infile);
                if (entree == EOF)
      	            return false;

                return true;
            }
        }
        entree = getc(infile);
        if (entree == EOF)
            return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsReparePatientDlg::emplisBuffer()
{
	bool tourner = true;
    int  champ, taille_champ, j;
    char buffer[100];

    //
    // Pour le premier enregistrement, on a déjà le nss
    //
    if (pBufPat->nss[0] != '\0')
        champ = 1;
    else
    {
        champ = 0;
        if ((entree != '1') && (entree != '2'))
        {
            while ((entree != '1') && (entree != '2'))
            {
                entree = getc(infile);
                if (entree == EOF)
      	            return false;
            }
        }
    }

	for (; (champ < 11) && (tourner); champ++)
    {
   	    switch (champ)
        {
      	    case  0 : taille_champ = PAT_NSS_LEN;       break;
            case  1 : taille_champ = PAT_NOM_LEN;       break;
            case  2 : taille_champ = PAT_PRENOM_LEN;    break;
            case  3 : taille_champ = PAT_CODE_LEN;      break;
            case  4 : taille_champ = PAT_CONVOC_LEN;    break;
            case  5 : taille_champ = PAT_SEXE_LEN;      break;
            case  6 : taille_champ = PAT_ADRESSE_LEN;   break;
            case  7 : taille_champ = PAT_NAISSANCE_LEN; break;
            case  8 : taille_champ = PAT_TELEPOR_LEN;   break;
            case  9 : taille_champ = PAT_TELEBUR_LEN;   break;
            case 10 : taille_champ = PAT_SITFAM_LEN;    break;
        }

        for (j = 0; (j < taille_champ) && (entree != EOF) ; j++)
        {
            if (entree >= 32)
      		    buffer[j] = entree;
            else
         	    buffer[j] = 32;

            entree = getc(infile);
        }
        buffer[j] = '\0';
        if (entree == EOF)
      	    tourner = false;

        switch (champ)
        {
      	    case  0 : strcpy(pBufPat->nss,       buffer);
                      pEditNSS->SetText(buffer);
                      break;
            case  1 : strcpy(pBufPat->nom,       buffer);
                      pEditNom->SetText(buffer);
                      break;
            case  2 : strcpy(pBufPat->prenom, 	 buffer);
                      pEditPrenom->SetText(buffer);
                      break;
            case  3 : strcpy(pBufPat->code,      buffer);
                      pEditCode->SetText(buffer);
                      break;
            case  4 : strcpy(pBufPat->convoc,    buffer); break;
            case  5 : strcpy(pBufPat->sexe,      buffer); break;
            case  6 : strcpy(pBufPat->adresse,   buffer); break;
            case  7 : strcpy(pBufPat->naissance, buffer); break;
            case  8 : strcpy(pBufPat->telepor,   buffer); break;
            case  9 : strcpy(pBufPat->telebur,   buffer); break;
            case 10 : strcpy(pBufPat->sitfam,    buffer); break;
        }
    }
    return tourner;
}

//---------------------------------------------------------------------------
//  Dispatche le buffer dans les tables
//---------------------------------------------------------------------------
bool
NsReparePatientDlg::traiteBuffer()
{
        //
    // Mise à jour de la fiche patient version 98
    //
    *(pPatImport->pDonnees) = *pBufPat;
    pPatImport->lastError = pPatImport->appendRecord();
    if (pPatImport->lastError != DBIERR_NONE)
    {
        erreur("Erreur à l'ajout de la fiche patient", 0, pPatImport->lastError);
        //return false;
    }
    pBufPat->metAZero();

    return true;
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsReparePatientDlg::initAffichage()
{
	pEditNSS->SetText("");
    pEditCode->SetText("");
    pEditNom->SetText("");
    pEditPrenom->SetText("");
}#endif

