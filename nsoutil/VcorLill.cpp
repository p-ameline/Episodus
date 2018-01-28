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
#include "nsoutil\vcorlill.h"

#include "nsbb\nsednum.h"

// -----------------------------------------------------------------
//
//  Méthodes de NsCvtCorrLilleDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsCvtCorrLilleDlg, TDialog)
	EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),
   	EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCvtCorrLilleDlg::NsCvtCorrLilleDlg(TWindow* pere, NSContexte* pCtx)
				  :NSUtilDialog(pere, pCtx, "CVT_COR", pNSResModule)
{
	pEditCode 	= new TEdit(this, IDC_CVT_CODE);
   	pEditNom 	= new TEdit(this, IDC_CVT_NOM);
#ifndef N_TIERS
   	pCorImport  = new NSCorrespondant(pContexte);
  	pAdrImport  = new NSAdresses(pContexte);#endif

   	pBufCor     = new V2_3CorresLille;

   	cvtPossible = true;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsCvtCorrLilleDlg::~NsCvtCorrLilleDlg()
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
   		infile.close();
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCvtCorrLilleDlg::SetupWindow()
{
	TDialog::SetupWindow();
   	//
   	// Ouverture du fichier d'importation
   	//
   	infile.open("MEDIFI.DAT");
    if (!infile)
   	{
   		erreur("Fichier d'importation (medifi.dat) non trouvé", 0, 0);
	  	cvtPossible = false;
      	return;
	}

    char ligne[300];
    if (!(infile.getline(ligne, 300)))
   	{
   		erreur("Le fichier d'importation (medifi.dat) est vide", 0, 0);
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
NsCvtCorrLilleDlg::demarreCvt()
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
NsCvtCorrLilleDlg::stoppeCvt()
{
	cvtTourner = false;
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsCvtCorrLilleDlg::emplisBuffer()
{
	bool tourner = true;
   	int  champ, taille_champ, j, k;
   	char buffer[100], ligne[300];

    strcpy(pBufCor->V2code,      "");
    strcpy(pBufCor->V2titre,     "");
    strcpy(pBufCor->V2tit_suite, "");
    strcpy(pBufCor->V2nom,       "");
    strcpy(pBufCor->V2prenom,    "");
    strcpy(pBufCor->V2nom_ep,    "");
    strcpy(pBufCor->V2adresse1,  "");
    strcpy(pBufCor->V2adresse2,  "");
    strcpy(pBufCor->V2ville, 	 "");
    strcpy(pBufCor->V2lieu, 	 "");
    strcpy(pBufCor->V2codepost,  "");

    if (!(infile.getline(ligne, 300)))
    	return false;

    if (strncmp(ligne, "MEDADR_CO", 9) == 0)
        return false;

    MessageBox(ligne, "ligne");

	for (champ = 0, k = 0; (champ < 11) && (tourner); champ++)
   	{
   		switch (champ)
      	{
      		case  0 : taille_champ = 23; break;
         	case  1 : taille_champ = 10; break;
         	case  2 : taille_champ = 23; break;
         	case  4 : taille_champ = 25; break;
         	case  3 : taille_champ = 14; break;
         	case  5 : taille_champ = 19; break;
         	case  6 : taille_champ = 37; break;
         	case  7 : taille_champ = 35; break;
         	case  8 : taille_champ = 38; break;
            case  9 : taille_champ =  6; break;
            case 10 : taille_champ =  5; break;
      	}

      	for (j = 0; (j < taille_champ) && (ligne[k] != '\0') ; j++, k++)
      	{
        	if (entree >= 32)
      			buffer[j] = ligne[k];
         	else
         		buffer[j] = 32;
      	}
      	buffer[j] = '\0';
        MessageBox(buffer, "buffer");

      	//OemToAnsi(buffer, buffer);

      	switch (champ)
      	{
        	case  0 : strcpy(pBufCor->V2code,      buffer); break;
         	case  1 : strcpy(pBufCor->V2titre,     buffer); break;
         	case  2 : strcpy(pBufCor->V2tit_suite, buffer); break;
         	case  3 : strcpy(pBufCor->V2nom,       buffer); break;
         	case  4 : strcpy(pBufCor->V2prenom,    buffer); break;
         	case  5 : strcpy(pBufCor->V2nom_ep,    buffer); break;
         	case  6 : strcpy(pBufCor->V2adresse1,  buffer); break;
         	case  7 : strcpy(pBufCor->V2adresse2,  buffer); break;
         	case  8 : strcpy(pBufCor->V2ville, 	   buffer); break;
            case  9 : strcpy(pBufCor->V2lieu, 	   buffer); break;
            case 10 : strcpy(pBufCor->V2codepost,  buffer); break;
      	}
   	}

   	return tourner;
}

//---------------------------------------------------------------------------//  Dispatche le buffer dans les tables
//---------------------------------------------------------------------------
bool
NsCvtCorrLilleDlg::traiteBuffer()
{
#ifndef N_TIERS
   	int i, j;

   	string sCode;

	pCorImport->pDonnees->metAZero();

    if (!(pCorImport->LastCode(&sCode,tCodeCorresp)))
   		return false;
   	else
   		strcpy(pCorImport->pDonnees->code, sCode.c_str());

   	strcpy(pCorImport->pDonnees->nom,     	 pBufCor->V2nom);
   	strcpy(pCorImport->pDonnees->prenom,  	 pBufCor->V2prenom);

    if (strcmp(pBufCor->V2tit_suite, "le Professeur") == 0)
   		strcpy(pCorImport->pDonnees->docteur, "P");
    else
    	strcpy(pCorImport->pDonnees->docteur, "D");

    char Message[500];
    strcpy(Message, "Nom "); strcat(Message, pCorImport->pDonnees->nom);
    strcat(Message, " Prénom "); strcat(Message, pCorImport->pDonnees->prenom);
    MessageBox(Message, "message");

   	//
   	// Ajout de l'adresse
   	//
   	pAdrImport->pDonnees->metAZero();
    strcpy(pAdrImport->pDonnees->adresse1, 	pBufCor->V2adresse1);
    strcpy(pAdrImport->pDonnees->adresse2, 	pBufCor->V2adresse2);
   	strcpy(pAdrImport->pDonnees->code_post, pBufCor->V2codepost);
   	strcpy(pAdrImport->pDonnees->ville, 	pBufCor->V2ville);

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
   	}
#endif
	return true ;
}


//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsCvtCorrLilleDlg::initAffichage()
{
	pEditCode->SetText("");
   	pEditNom->SetText("");
}
