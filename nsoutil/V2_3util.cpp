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
#include "nsoutil\v2_3util.h"

#include "nsbb\nsednum.h"

// -----------------------------------------------------------------
//
//  Méthodes de NsCvtPatientDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsCvtUtilisatDlg, TDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),
   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCvtUtilisatDlg::NsCvtUtilisatDlg(TWindow* pere, NSContexte* pCtx)
                 :NSUtilDialog(pere, pCtx, "CVT_UTIL", pNSResModule)
{
    pEditCode 	= new TEdit(this, IDC_CVT_CODE);
    pEditNom 	= new TEdit(this, IDC_CVT_NOM);
#ifndef N_TIERS
    pUtiliImport = new NSUtilisateur(pContexte);#endif

    pBufCor     = new V2_3Utilisat;

    cvtPossible = true;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NsCvtUtilisatDlg::~NsCvtUtilisatDlg()
{
    //
	// Suppression de tous les objets
	//
    delete pEditCode;
    delete pEditNom;

#ifndef N_TIERS
    delete pUtiliImport;#endif
    delete pBufCor;

    if (infile != NULL)
   	    fclose(infile);
}

//---------------------------------------------------------------------------//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCvtUtilisatDlg::SetupWindow()
{
    TDialog::SetupWindow();
    //
    // Ouverture du fichier d'importation
    //
    infile = fopen("ENDOMEN.DAT", "r");
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
    //
#ifndef N_TIERS
    pUtiliImport->lastError = pUtiliImport->open();    if (pUtiliImport->lastError != DBIERR_NONE)
    {
   	    erreur("Impossible d'ouvrir le fichier des utilisateurs", 0, pUtiliImport->lastError);
        cvtPossible = false;
        return;
    }
#endif
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsCvtUtilisatDlg::demarreCvt()
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
NsCvtUtilisatDlg::stoppeCvt()
{
    cvtTourner = false;
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsCvtUtilisatDlg::emplisBuffer()
{
    bool tourner = true;
    int  champ, taille_champ, j;
    char buffer[100];

    for (champ = 0; (champ < 13) && (tourner); champ++)
    {
   	    switch (champ)
        {
      	    case  0 : taille_champ =  3; break;
            case  1 : taille_champ =  6; break;
            case  2 :
            case  3 : taille_champ = 25; break;
            case  4 : taille_champ = 50; break;
            case  5 : taille_champ = 25; break;
            case  6 : taille_champ = 13; break;
            case  7 : taille_champ =  9; break;
            case  8 : taille_champ =  9; break;
            case  9 : taille_champ =  6; break;
            case 10 : taille_champ =  6; break;
            case 11 : taille_champ =  8; break;
            case 12 : taille_champ = 20; break;
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
            case  0 : strcpy(pBufCor->V2indice,   buffer);
                      pEditCode->SetText(buffer);
                      break;
            case  1 : strcpy(pBufCor->V2code,     buffer); break;
            case  2 : strcpy(pBufCor->V2nom, 	  buffer);
                      pEditNom->SetText(buffer);
                      break;
            case  3 : strcpy(pBufCor->V2prenom,   buffer); break;
            case  4 : strcpy(pBufCor->V2adresse,  buffer); break;
            case  5 : strcpy(pBufCor->V2ville,    buffer); break;
            case  6 : strcpy(pBufCor->V2GParam,   buffer); break;
            case  7 : strcpy(pBufCor->V2CParam,   buffer); break;
            case  8 : strcpy(pBufCor->V2PParam,   buffer); break;
            case  9 : strcpy(pBufCor->V2WParam,   buffer); break;
            case 10 : strcpy(pBufCor->V2LParam,   buffer); break;
            case 11 : strcpy(pBufCor->V2dercod,   buffer); break;
            case 12 : strcpy(pBufCor->V2securite, buffer); break;
        }
    }

    return tourner;
}

//---------------------------------------------------------------------------//  Dispatche le buffer dans les tables
//---------------------------------------------------------------------------
bool
NsCvtUtilisatDlg::traiteBuffer()
{
#ifndef N_TIERS
    if (strcmp(pBufCor->V2indice, "") == 0)
        return true;

    int i, j;

    string sCode;

	pUtiliImport->pDonnees->metAZero();

    strcpy(pUtiliImport->pDonnees->indice, pBufCor->V2indice);
    for (i = 0; i < 5; i++)
        pUtiliImport->pDonnees->code[i] = pBufCor->V2code[i + 1];
    strcpy(pUtiliImport->pDonnees->nom,    pBufCor->V2nom);
    strcpy(pUtiliImport->pDonnees->prenom, pBufCor->V2prenom);

    if (strcmp(pUtiliImport->pDonnees->indice, "001") != 0)
    {
        pUtiliImport->lastError = pUtiliImport->appendRecord();
        if (pUtiliImport->lastError != DBIERR_NONE)
        {
            erreur("Erreur à l'ajout de la fiche utilisateur", 0, pUtiliImport->lastError);
            return false;
        }
    }
    else
    {
        pUtiliImport->lastError = pUtiliImport->chercheClef(&(string("001")),
                                                            "",
                                                            0,
                                                            keySEARCHEQ,
                                                            dbiWRITELOCK);
        // Si on n'a pas trouvé, on ajoute
        //
        if (pUtiliImport->lastError != DBIERR_NONE)
        {
            pUtiliImport->lastError = pUtiliImport->modifyRecord(TRUE);
            if (pUtiliImport->lastError != DBIERR_NONE)
      	        erreur("Erreur à la mise à jour de la fiche 1", 0, pUtiliImport->lastError);
        }
    }
#endif
    return true;
}


//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsCvtUtilisatDlg::initAffichage()
{
    pEditCode->SetText("");
    pEditNom->SetText("");
}
