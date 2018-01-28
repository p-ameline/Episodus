#include <owl\olemdifr.h>#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>
// #include <mem.h>
// #include <io.h>
// #include <fcntl.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsimport.rh"
#include "nsoutil\importCorr.h"
#include "nsbb\nsednum.h"
#include "nautilus\nsdocref.h"

// -----------------------------------------------------------------
//
//  Méthodes de NsCvtPatientDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsImportCocciCorrDlg, TDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NsImportCocciCorrDlg::NsImportCocciCorrDlg(TWindow* pere, NSContexte* pCtx)
                     :NSUtilDialog(pere, pCtx, "CVT_COCCI_CORRESP", pNSResModule)
{
    pEditCode 	    = new TEdit(this, IDC_CVT_CODE);
    pEditNom 	    = new TEdit(this, IDC_CVT_NOM);
#ifndef N_TIERS
    pCorrImport     = new NSCorrespondant(pContexte);
  	pAdrImport      = new NSAdresses(pContexte);
#endif
    pBufCorr        = new CocciCorresp;

    cvtPossible     = true;

    bAppend         = true;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsImportCocciCorrDlg::~NsImportCocciCorrDlg()
{
    //
	// Suppression de tous les objets
	//
    delete pEditCode;
    delete pEditNom;
#ifndef N_TIERS
    pCorrImport->close();
    delete pCorrImport;
    pAdrImport->close();
  	delete pAdrImport;
#endif
    delete pBufCorr;

    if (inFile)
        inFile.close();
}

//---------------------------------------------------------------------------//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsImportCocciCorrDlg::SetupWindow()
{
    TDialog::SetupWindow();
    //
    // Ouverture du fichier d'importation
    //
    inFile.open("CONFRERES.TXT");
    if (!inFile)
    {
   	    erreur("Fichier d'importation des correspondants non trouvé", 0, 0);
	  	cvtPossible = false;
  	}

    getline(inFile, sLine);
    if (inFile.eof())
    {
   	    erreur("Fichier d'importation des correspondants vide", 0, 0);
	  	cvtPossible = false;
  	}
#ifndef N_TIERS
	//
    // Ouverture des tables
    //
    if (cvtPossible)
    {
   	    pCorrImport->lastError = pCorrImport->open();
  		if (pCorrImport->lastError != DBIERR_NONE)
   	    {
      	    erreur("Impossible d'ouvrir le fichier des correspondants", 0, pCorrImport->lastError);
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
#endif
}

//---------------------------------------------------------------------------//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsImportCocciCorrDlg::demarreCvt()
{
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
        else if (inFile.eof())
      	    traiteBuffer();
    }
}

//---------------------------------------------------------------------------//  Arrêt de la conversion en boucle
//---------------------------------------------------------------------------
void
NsImportCocciCorrDlg::stoppeCvt()
{
   cvtTourner = false;
}

//---------------------------------------------------------------------------//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsImportCocciCorrDlg::emplisBuffer()
{
    bool tourner = true;
    int  champ, taille_champ, j;
    char buffer[100];

    if (strlen(sLine.c_str()) == 262)
    {
        int i = 0;
	    for (champ = 0; (champ < 25) && (tourner); champ++)
        {
   	        switch (champ)
            {
      	        case  0 : taille_champ =  6;    break;
                case  1 : taille_champ = 10;    break;
                case  2 : taille_champ = 25;    break;
                case  3 : taille_champ = 50;    break;
                case  4 : taille_champ = 50;    break;
                case  5 : taille_champ = 30;    break;
                case  6 : taille_champ = 20;    break;
                case  7 : taille_champ = 20;    break;
                case  8 : taille_champ = 11;    break;
                case  9 : taille_champ = 30;    break;
                case 10 : taille_champ = 10;    break;
            }

            for (j = 0; j < taille_champ; j++, i++)
                buffer[j] = sLine[i];
            buffer[j] = '\0';

            // OemToAnsi(buffer, buffer);

            switch (champ)
            {
      	        case  0 :   strcpy(pBufCorr->CocciUser,         buffer);    break;
                case  1 :   strcpy(pBufCorr->CocciTitre,        buffer);    break;
                case  2 :   strcpy(pBufCorr->CocciNom,          buffer);
                            pEditNom->SetText(buffer);
                            break;
                case  3 :   strcpy(pBufCorr->CocciAdresse,      buffer);    break;
                case  4 :   strcpy(pBufCorr->CocciAd1,          buffer);    break;
                case  5 :   strcpy(pBufCorr->CocciCp,           buffer);    break;
                case  6 :   strcpy(pBufCorr->CocciTel,          buffer);    break;
                case  7 :   strcpy(pBufCorr->CocciFax,          buffer);    break;
                case  8 :   strcpy(pBufCorr->CocciCompteur,     buffer);
                            pEditCode->SetText(buffer);
                            break;
                case  9 :   strcpy(pBufCorr->CocciSpecialite,   buffer);    break;
                case 10 :   strcpy(pBufCorr->CocciTypeCorr,     buffer);    break;
            }
        }
    }

    getline(inFile, sLine);

    if (inFile.eof())
        return false;
    else
        return true;
}

//---------------------------------------------------------------------------//  Dispatche le buffer dans les tables
//---------------------------------------------------------------------------
bool
NsImportCocciCorrDlg::traiteBuffer()
{
    int i, j;
    int idRet;
    string sCode;
    string nom, prenom, sexe1, date1, titre1, sexe2, date2, titre2;
    string sNss;

    //
    // On importe uniquement les correspondants de type "MED"
    //
    ote_blancs(pBufCorr->CocciTypeCorr);
    if (string(pBufCorr->CocciTypeCorr) != "MED")
        return true;

#ifndef N_TIERS
	//
    // Initialisation de la fiche correspondant Nautilus
    //
    pCorrImport->pDonnees->metAZero();

    ote_blancs(pBufCorr->CocciTitre);
    if (pBufCorr->CocciTitre[0] != '\0')
    {
        for (int i = 0; i < strlen(pBufCorr->CocciTitre); i++)
            pBufCorr->CocciTitre[i] = pseumaj(pBufCorr->CocciTitre[i]);

        if      (string(pBufCorr->CocciTitre) == "DOCTEUR")
            strcpy(pCorrImport->pDonnees->docteur, "D");
        else if (string(pBufCorr->CocciTitre) == "PROFESSEUR")
            strcpy(pCorrImport->pDonnees->docteur, "P");
    }

    ote_blancs(pBufCorr->CocciNom);
    strcpy(pCorrImport->pDonnees->nom, pBufCorr->CocciNom);
#endif
    //
    // Conversion en base 36 du numéro
    //
    ote_blancs(pBufCorr->CocciCompteur);

    string sCompteur = "";

    double dCompteur = StringToDouble(string(pBufCorr->CocciCompteur));
    double dResu;
    double dReste;
    while (dCompteur > 36)
    {
        dResu = floor(dCompteur / 36);
        dReste = dCompteur - (dResu * 36);
        if (dReste <= 9)
            sCompteur = string(1, '0' + dReste) + sCompteur;
        else
            sCompteur = string(1, 'A' + dReste - 10) + sCompteur;

        dCompteur = dResu;
    }
    if (dCompteur > 0)
    {
        if (dCompteur <= 9)
            sCompteur = string(1, '0' + dCompteur) + sCompteur;
        else
            sCompteur = string(1, 'A' + dCompteur - 10) + sCompteur;
    }

    int iCptLen = strlen(sCompteur.c_str());
    if (iCptLen > COR_CODE_LEN)
        return true;
    if (iCptLen < COR_CODE_LEN)
        sCompteur = string(COR_CODE_LEN - iCptLen, '0') + sCompteur;

#ifndef N_TIERS
    strcpy(pCorrImport->pDonnees->code, sCompteur.c_str());

    //
    // Création d'une adresse Nautilus
    //
    pAdrImport->pDonnees->metAZero();

    ote_blancs(pBufCorr->CocciAdresse);
    strncpy(pAdrImport->pDonnees->adresse1, pBufCorr->CocciAdresse, ADR_ADRESSE1_LEN);

    ote_blancs(pBufCorr->CocciAd1);
    strncpy(pAdrImport->pDonnees->adresse2, pBufCorr->CocciAd1, ADR_ADRESSE1_LEN);

    ote_blancs(pBufCorr->CocciCp);
    string sCoccCp = string(pBufCorr->CocciCp);
    if (strlen(sCoccCp.c_str()) > 5)
    {
        string sCodePost    = string(sCoccCp, 0, 5);
        string sVille       = string(sCoccCp, 6, strlen(sCoccCp.c_str())-6);

        strcpy(pAdrImport->pDonnees->code_post, sCodePost.c_str());
        strcpy(pAdrImport->pDonnees->ville,     sVille.c_str());
    }

    ote_blancs(pBufCorr->CocciTel);
    strncpy(pAdrImport->pDonnees->telephone, pBufCorr->CocciTel, ADR_TELEPHONE_LEN);

    ote_blancs(pBufCorr->CocciFax);
    strncpy(pAdrImport->pDonnees->fax, pBufCorr->CocciFax, ADR_FAX_LEN);

    if ((pAdrImport->pDonnees->adresse1[0]  != '\0')    ||
   	    (pAdrImport->pDonnees->code_post[0] != '\0')    ||
        (pAdrImport->pDonnees->ville[0]     != '\0')    ||
        (pAdrImport->pDonnees->telephone[0] != '\0'))
    {
   	    if (pAdrImport->LastCode(&sCode, tCodeAdresse))
        {
      	    strcpy(pAdrImport->pDonnees->code, sCode.c_str());

            pAdrImport->lastError = pAdrImport->appendRecord();
            if (pAdrImport->lastError != DBIERR_NONE)
            {
         	    erreur("Erreur à l'ajout de la fiche patient", 0, pAdrImport->lastError);
                return false;
            }

            strcpy(pCorrImport->pDonnees->adresse, pAdrImport->pDonnees->code);
            pCorrImport->lastError = pCorrImport->appendRecord();
            if (pCorrImport->lastError != DBIERR_NONE)
            {
         	    erreur("Erreur à l'ajout de la fiche correspondant", 0, pCorrImport->lastError);
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
   	    pCorrImport->lastError = pCorrImport->appendRecord();
        if (pCorrImport->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à l'ajout de la fiche patient", 0, pCorrImport->lastError);
            return true;
        }
    }
#endif

    return true;
}

//---------------------------------------------------------------------------//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsImportCocciCorrDlg::initAffichage()
{
    pEditCode->SetText("");
    pEditNom->SetText("");
}

