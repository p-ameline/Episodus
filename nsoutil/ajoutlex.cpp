//---------------------------------------------------------------------------
//    V2_3PERS.CPP
//  Implémentation de objets utilisés pour passer en version 3.0 de NAUTILUS
//---------------------------------------------------------------------------
#include <mem.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>

#include "nautilus\nssuper.h"
#include "nautilus\nsannexe.h"
#include "partage\nsdivfct.h"

#include "nsoutil\v2_3pers.h"


//***************************************************************************
// Implémentation des mèthodes V2_3Patient
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:  V2_3Patient::importe()
//
//  Arguments:
//
//  Description:
//             Importation du fichier PATFI
//  Returns:
//             Erreur DBI, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult V2_3Patient::importe()
{
	short   	 i, j, champ, taille_champ;
	int     	 entree;
	bool    	 tourner = true;
	char 		 buffer[100];
	DBIResult lastError;

   string	 sCode;

  	//
  	// Ouverture du fichier d'importation
  	//
  	FILE* infile = fopen("SP", "r");
  	if (infile == NULL)
   {
   	erreur("Fichier d'importation non trouvé", 0, 0);
	  	return(-1);
  	}
  	//PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "PATFI.DAT");
  	setmode(fileno(infile), O_BINARY);

  	NSSP*  pSP = new NSSP(pSuper);
  	//
  	// Ouverture de la table
  	//
  	//PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "Non");
  	lastError = pSP->open();
  	if (lastError != DBIERR_NONE)
   {
   	lastError = pSP->close();
      delete PSP;
   	return(lastError);
   }
  	//PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "OK");

  	entree = getc(infile);
  	if (entree == EOF) tourner = false;
  	//
  	// Boucle globale d'importation
  	//
  	while (tourner)
  	{
   	//
      // Prise des champs
      //
      for (champ = 0; (champ < 19) && (tourner); champ++)
      {
      	switch (champ)
         {
         	case  0 : taille_champ =  6; break;
            case  1 : taille_champ = 6; break;
            case  2 : taille_champ = 6; break;
            case  3 : taille_champ = 10; break;
            case  4 : taille_champ = 10; break;
            case  5 : taille_champ = 10; break;
            case  6 : taille_champ = 10; break;
            case  7 :
            case  8 :                   ...
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

         for (j = 0; (j < taille_champ) && (entree != EOF) && (entree != '|'); j++)
         {
         	buffer[j] = entree;
            entree = getc(infile);
         }
         buffer[j] = '\0';
         if (entree == EOF) tourner = false;
         else if (entree == '|') entree = getc(infile);

         OemToAnsi(buffer, buffer);

         pSP->pDonnees->metAZero();
         switch (champ)
         {
         	case  0 : strcpy(pDonnees->CodeInterne,       buffer); break;
            ....
            case  1 : strcpy(V2dernier,   buffer); break;
            case  2 : strcpy(V2nom, 	   buffer); break;
            case  3 : strcpy(V2prenom,    buffer); break;
            case  4 : strcpy(V2convoc,    buffer); break;
            case  5 : strcpy(V2adresse,   buffer); break;
            case  6 : strcpy(V2ville,     buffer); break;
            case  7 :
            case  8 :
            case  9 :
            case 10 :
            case 11 :
            case 12 : strcpy(V2medic[champ-7], buffer); break;
            case 13 : strcpy(V2medinom,   buffer); break;
            case 14 : strcpy(V2medidres,  buffer); break;
            case 15 : strcpy(V2mediville, buffer); break;
            case 16 : strcpy(V2naissance, buffer); break;
            case 17 : strcpy(V2telephone, buffer); break;
            case 18 : strcpy(V2code,      buffer); break; //TRANSFORMER LES DATES
         }
      }
      pSP->lastError = pSP->appendRecord();
      if (pSP->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à l'ajout de la fiche patient", 0, pSP->lastError);
         fclose(infile);
         pSP->lastError = pSP->close();
         delete pSP;
      }
  }
  fclose(infile);
  lastError = pSP->close();
  delete pSP;
  return(lastError);
}

//***************************************************************************
// Implémentation des mèthodes V2_3Correspondant
//***************************************************************************

V2_3Correspondant::V2_3Correspondant(NSSuper* pSup)
						:NSCorrespondant(pSup)
{
	pSuper = pSup;
}

//---------------------------------------------------------------------------
//  Function:  V2_3Correspondant::importe()
//
//  Arguments:
//
//  Description:
//             Importation du fichier MEDIFI
//  Returns:
//             Erreur DBI, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
V2_3Correspondant::importe()
{
	short   	 i, j, champ, taille_champ;
  	int     	 entree;
  	bool    	 tourner = true;
  	char 		 buffer[100];
  	DBIResult lastError;
  	bool		 bAdresse;
   string	 sCode;

  	//
  	// Ouverture du fichier d'importation
  	//
  	FILE* infile = fopen("MEDIFI.DAT", "r");
  	if (infile == NULL) {
	  	erreur("Fichier d'importation non trouvé", 0, 0);
  		return(-1);
  	}
  	//PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "PATFI.DAT");
  	setmode(fileno(infile), O_BINARY);

  	NSAdresses* pAdrImport = new NSAdresses(pSuper);
  	pAdrImport->lastError = pAdrImport->open();
	if (pAdrImport->lastError != DBIERR_NONE)
  	{
      lastError = pAdrImport->lastError;
   	erreur("Impossible d'ouvrir le fichier des adresses", 0, pAdrImport->lastError);
      delete pAdrImport;
      return(lastError);
   }

  	NSCorrespondant*  pCorImport = new NSCorrespondant(pSuper);
  	//
  	// Ouverture de la table
  	//
  	//PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "Non");
  	lastError = pCorImport->open();
  	if (lastError != DBIERR_NONE)
   {
   	pAdrImport->close();
      delete pAdrImport;
      delete pCorImport;
   	return(lastError);
   }
  	//PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "OK");

  	entree = getc(infile);
  	if (entree == EOF) tourner = false;
  	//
  	// Boucle globale d'importation
  	//
  	while (tourner)
  	{
   	//
      // Prise des champs
      //
      for (champ = 0; (champ < 9) && (tourner); champ++)
      {
      	switch (champ)
         {
         	case  0 : taille_champ =  3; break;
            case  1 :
            case  2 :
            case  4 : taille_champ = 25; break;
            case  3 : taille_champ = 50; break;
            case  5 : taille_champ =  1; break;
            case  6 : taille_champ = 15; break;
            case  7 : taille_champ =  2; break;
            case  8 : taille_champ = 35; break;
         }

         for (j = 0; (j < taille_champ) && (entree != EOF) ; j++)
         {
         	buffer[j] = entree;
            entree = getc(infile);
         }
         buffer[j] = '\0';
         if (entree == EOF) tourner = false;

         OemToAnsi(buffer, buffer);

         //if (champ == 0)
         //	PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_NBREC, buffer);

         switch (champ)
         {
         	case  0 : strcpy(V2code,       buffer); break;
            case  1 : strcpy(V2nom, 	    buffer); break;
            case  2 : strcpy(V2prenom,     buffer); break;
            case  3 : strcpy(V2adresse,    buffer); break;
            case  4 : strcpy(V2ville,      buffer); break;
            case  5 : strcpy(V2docteur,    buffer); break;
            case  6 : strcpy(V2telephone,  buffer); break;
            case  7 : strcpy(V2nb_exemp,   buffer); break;
            case  8 : strcpy(V2messagerie, buffer); break;
         }
      }
      //
      // Mise à jour de la fiche version 3
      //
      pCorImport->pDonnees->metAZero();
      strcpy(pCorImport->pDonnees->code,   V2code);
      strcpy(pCorImport->pDonnees->nom,    V2nom);
      strcpy(pCorImport->pDonnees->prenom, V2prenom);
      strcpy(pCorImport->pDonnees->docteur,V2docteur);
      strcpy(pCorImport->pDonnees->nb_exemp, 	V2nb_exemp);
      strcpy(pCorImport->pDonnees->messagerie, V2messagerie);

      pCorImport->lastError = pCorImport->appendRecord();
      if (pCorImport->lastError != DBIERR_NONE)
      {
         lastError = pCorImport->lastError;
      	erreur("Erreur à l'ajout de la fiche correspondant", 0, pCorImport->lastError);
         fclose(infile);
         pAdrImport->close();
      	delete pAdrImport;
         pCorImport->close();
      	delete pCorImport;
         return(lastError);
      }

      //
      // Ajout de l'adresse
      //
      pAdrImport->pDonnees->metAZero();
      for (i = 0;  i < 25; i++) pAdrImport->pDonnees->adresse1[i] 	= V2adresse[i];
      for (i = 0;  i < 25; i++) pAdrImport->pDonnees->adresse2[i] 	= V2adresse[i+25];
      for (i = 0;  i <  5; i++) pAdrImport->pDonnees->code_post[i] = V2ville[i+20];
      for (i = 0;  i < 20; i++) pAdrImport->pDonnees->ville[i] 	  	= V2ville[i];
      strcpy(pAdrImport->pDonnees->telephone, V2telephone);

      if (pAdrImport->LastCode(&sCode,tCodeAdresse))
      {
   		strcpy(pAdrImport->pDonnees->code, sCode.c_str());

      	pAdrImport->lastError = pAdrImport->appendRecord();
      	if (pAdrImport->lastError != DBIERR_NONE)
      	{
      		erreur("Erreur à l'ajout de la fiche patient", 0, pAdrImport->lastError);
         	fclose(infile);
         	pCorImport->lastError = pCorImport->close();
         	delete pCorImport;
         	pAdrImport->lastError = pAdrImport->close();
         	delete pAdrImport;
         	return(lastError);
         }
      }
      else
      {
      	erreur("Impossible de trouver un nouveau code d'adresse", 0, pAdrImport->lastError);
         fclose(infile);
         lastError = pCorImport->close();
         delete pCorImport;
         lastError = pAdrImport->close();
         delete pAdrImport;
         return(lastError);
      }

  }
  fclose(infile);
  lastError = pCorImport->close();
  delete pCorImport;

  return(lastError);
}

//***************************************************************************
// Implémentation des mèthodes V2_3CompteRendu
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:  V2_3CompteRendu::importe()
//
//  Arguments:
//
//  Description:
//             Importation du fichier MEDIFI
//  Returns:
//             Erreur DBI, s'il y a lieu
//---------------------------------------------------------------------------
/*DBIResult V2_3CompteRendu::importe()
{
  short   		 i, j, champ, taille_champ;
  int     		 entree;
  BOOL    		 tourner = TRUE;
  char 			 buffer[256];
  DBIResult 	 lastError;

  //
  // Ouverture du fichier d'importation
  //
  FILE* infile = fopen("GASTRO2F.DAT", "r");
  if (infile == NULL) {
	  erreur("Fichier d'importation non trouvé", 0, 0);
	  return(-1);
  }
  //PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "PATFI.DAT");
  setmode(fileno(infile), O_BINARY);

  NSCompteRendu* pCrImport = new NSCompteRendu;
  //
  // Ouverture de la table
  //
  //PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "Non");
  lastError = pCrImport->open();
  if (lastError != DBIERR_NONE) return(lastError);
  //PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_OUV, "OK");

  entree = getc(infile);
  if (entree == EOF) tourner = FALSE;
  //
  // Boucle globale d'importation
  //
  while (tourner)
  {
		  //
		  // Prise des champs
		  //
		  for (champ = 0; (champ < 6) && (tourner); champ++)
		  {
				switch (champ)
				{
						case  0 : taille_champ =  8; break;
						case  1 :
						case  2 :
						case  3 :
						case  4 :
						case  5 : taille_champ = 255; break;
				}

				for (j = 0; (j < taille_champ) && (entree != EOF) ; j++)
				{
					 buffer[j] = entree;
					 entree = getc(infile);
				}
				buffer[j] = '\0';
				if (entree == EOF) tourner = FALSE;

				OemToAnsi(buffer, buffer);

				//if (champ == 0)
				//	PV2_3Dialog(pNSSuperviseur->PrendWndCourant())->PeindreEdit(PAT_NBREC, buffer);

				switch (champ)
				{
						case  0 : strcpy(V2nss, buffer); break;
						case  1 :
						case  2 :
						case  3 :
						case  4 :
						case  5 : strcpy(V2groupe[champ-1], buffer); break;
				}
		  }
		  //
		  // Mise à jour de la fiche version 3
		  //
		  pCrImport->metABlanc();
		  for (i = 0;  i < 6;  i++) pCrImport->nss[i] = V2nss[i+2];
		  for (i = 0;  i < 2;  i++) pCrImport->nss[i+6] = V2nss[i];
		  pCrImport->nss[i+6] = '\0';
		  for (i = 0;  i < 5;  i++) strcpy(pCrImport->groupe[i], V2groupe[i]);
		  lastError = pCrImport->appendRecord();
		  if (lastError != DBIERR_NONE)
		  {
				erreur("Erreur à l'ajout de la fiche correspondant", 0, lastError);
				fclose(infile);
				return(lastError);
		  }
  }
  fclose(infile);
  lastError = pCrImport->close();
  delete pCrImport;

  return(lastError);
} */

