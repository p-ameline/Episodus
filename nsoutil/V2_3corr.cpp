#include <owl\olemdifr.h>
#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>
#include <mem.h>
#include <io.h>
#include <fcntl.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"

#include "nsoutil\nsoutil.h"
#include "nsoutil\nscorlib.h"
#include "nsoutil\v2_3.rh"
#include "nsoutil\v2_3corr.h"

#include "nsbb\nsednum.h"

// -----------------------------------------------------------------
//
//  Méthodes de NsCvtPatientDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsCvtCorrespDlg, TDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),
   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCvtCorrespDlg::NsCvtCorrespDlg(TWindow* pere, NSContexte* pCtx)
				  	 :NSUtilDialog(pere, pCtx, "CVT_COR", pNSResModule)
{
   pEditCode 	= new TEdit(this, IDC_CVT_CODE);
   pEditNom 	= new TEdit(this, IDC_CVT_NOM);
#ifndef N_TIERS
   pCorImport  = new NSCorrespondant(pContexte);
  	pAdrImport  = new NSAdresses(pContexte);#endif

   pBufCor     = new V2_3Correspondant;

   cvtPossible = true;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsCvtCorrespDlg::~NsCvtCorrespDlg()
{
	//
	// Suppression de tous les objets
	//
   delete pEditCode;
   delete pEditNom;
#ifndef N_TIERS
   delete pCorImport;
  	delete pAdrImport;#endif

   delete pBufCor;

   if (infile != NULL)
   	fclose(infile);
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCvtCorrespDlg::SetupWindow()
{
	TDialog::SetupWindow();
   //
   // Ouverture du fichier d'importation
   //
   infile = fopen("MEDIFI.DAT", "r");
  	if (infile == NULL)
   {
   	erreur("Fichier d'importation non trouvé", 0, 0);
	  	cvtPossible = false;
      return;
  	}
   setmode(fileno(infile), O_BINARY);

   entree = getc(infile);
  	if (entree == EOF)
   {
   	erreur("Le fichier d'importation est vide", 0, 0);
	  	cvtPossible = false;
      return;
  	}
	//
   // Ouverture des tables
   //#ifndef N_TIERS

   pCorImport->lastError = pCorImport->open();
   if (pCorImport->lastError != DBIERR_NONE)
   {
   	erreur("Impossible d'ouvrir le fichier des correspondants", 0, pCorImport->lastError);
      cvtPossible = false;
      return;
   }
   pAdrImport->lastError = pAdrImport->open();
   if (pAdrImport->lastError != DBIERR_NONE)
   {
   	erreur("Impossible d'ouvrir le fichier des adresses", 0, pAdrImport->lastError);
      cvtPossible = false;
      return;
   }#endif
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsCvtCorrespDlg::demarreCvt()
{
#ifndef _INCLUS
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
#endif
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsCvtCorrespDlg::stoppeCvt()
{
   cvtTourner = false;
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsCvtCorrespDlg::emplisBuffer()
{
	bool tourner = true;
   int  champ, taille_champ, j;
   char buffer[100];

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
         case  0 : strcpy(pBufCor->V2code,       buffer);
                   pEditCode->SetText(buffer);
         			 break;
         case  1 : strcpy(pBufCor->V2nom, 	    buffer);
                   pEditNom->SetText(buffer);
         			 break;
         case  2 : strcpy(pBufCor->V2prenom,     buffer); break;
         case  3 : strcpy(pBufCor->V2adresse,    buffer); break;
         case  4 : strcpy(pBufCor->V2ville,      buffer); break;
         case  5 : strcpy(pBufCor->V2docteur,    buffer); break;
         case  6 : strcpy(pBufCor->V2telephone,  buffer); break;
         case  7 : strcpy(pBufCor->V2nb_exemp,   buffer); break;
         case  8 : strcpy(pBufCor->V2messagerie, buffer); break;
      }
   }

   return tourner;
}

//---------------------------------------------------------------------------
//  Dispatche le buffer dans les tables
//---------------------------------------------------------------------------
bool
NsCvtCorrespDlg::traiteBuffer()
{
   int i, j;

   string sCode;
#ifndef N_TIERS

	pCorImport->pDonnees->metAZero();
   strcpy(pCorImport->pDonnees->code,    	  pBufCor->V2code);
   strcpy(pCorImport->pDonnees->nom,     	  pBufCor->V2nom);
   strcpy(pCorImport->pDonnees->prenom,  	  pBufCor->V2prenom);
   strcpy(pCorImport->pDonnees->docteur, 	  pBufCor->V2docteur);
   strcpy(pCorImport->pDonnees->nb_exemp,   pBufCor->V2nb_exemp);
   strcpy(pCorImport->pDonnees->messagerie, pBufCor->V2messagerie);
   //
   // Ajout de l'adresse
   //
   pAdrImport->pDonnees->metAZero();
   for (i = 0;  i < 25; i++) pAdrImport->pDonnees->adresse1[i]  = pBufCor->V2adresse[i];
   for (i = 0;  i < 25; i++) pAdrImport->pDonnees->adresse2[i]  = pBufCor->V2adresse[i+25];
   for (i = 0;  i <  5; i++) pAdrImport->pDonnees->code_post[i] = pBufCor->V2ville[i+20];
   for (i = 0;  i < 20; i++) pAdrImport->pDonnees->ville[i] 	 = pBufCor->V2ville[i];
   strcpy(pAdrImport->pDonnees->telephone, pBufCor->V2telephone);

   if ((strcmp(pAdrImport->pDonnees->adresse1, "                         ") != 0) ||
   	 (strcmp(pAdrImport->pDonnees->adresse2, "                         ") != 0))
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

         strcpy(pCorImport->pDonnees->adresse, pAdrImport->pDonnees->code);
         pCorImport->lastError = pCorImport->appendRecord();
   		if (pCorImport->lastError != DBIERR_NONE)
   		{
   			erreur("Erreur à l'ajout de la fiche correspondant", 0, pCorImport->lastError);
      		return false;
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
   	pCorImport->lastError = pCorImport->appendRecord();
      if (pCorImport->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à l'ajout de la fiche correspondant", 0, pCorImport->lastError);
         return false;
      }
   }#endif
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsCvtCorrespDlg::initAffichage()
{
   pEditCode->SetText("");
   pEditNom->SetText("");
}
