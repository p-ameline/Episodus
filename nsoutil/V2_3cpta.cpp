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
#include "nsoutil\v2_3cpta.h"

#include "nsbb\nsednum.h"

// -----------------------------------------------------------------
//
//  Méthodes de NsCvtPatientDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsCvtComptDlg, TDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),
   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCvtComptDlg::NsCvtComptDlg(TWindow* pere, NSContexte* pCtx, bool cvtGlob)
              :NSUtilDialog(pere, pCtx, "CVT_CPTA", pNSResModule)
{
    pEditCode 	 = new TEdit(this, IDC_CVT_CODE);
    pEditNom 	 = new TEdit(this, IDC_CVT_NOM);

    pComptImport = new NSCompt(pContexte);
    pFactImport  = new NSFact(pContexte);

    pBufCompt   = new V2_3Compt;
    pBufFact    = new V2_3Fact;

    cvtPossible = true;

    cvtGlobale  = cvtGlob;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsCvtComptDlg::~NsCvtComptDlg()
{
    //
	// Suppression de tous les objets
	//
    delete pEditCode;
    delete pEditNom;

    delete pComptImport;
    delete pFactImport;

    delete pBufCompt;
    delete pBufFact;

    if (infile != NULL)
   	    fclose(infile);
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCvtComptDlg::SetupWindow()
{
    TDialog::SetupWindow();
	//
    // Ouverture des tables
    //
    pFactImport->lastError = pFactImport->open();
    if (pFactImport->lastError != DBIERR_NONE)
    {
   	    erreur("Impossible d'ouvrir le fichier des factures", 0, pFactImport->lastError);
        cvtPossible = false;
        return;
    }
    pComptImport->lastError = pComptImport->open();
    if (pComptImport->lastError != DBIERR_NONE)
    {
   	    erreur("Impossible d'ouvrir le fichier de comptabilité", 0, pComptImport->lastError);
        cvtPossible = false;
        pFactImport->close();
        return;
    }
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsCvtComptDlg::demarreCvt()
{
    cvtTourner = true;

    //
    // Conversion des factures
    //
    // Conversion globale
    //
    if (cvtGlobale)
    {
        infile = fopen("FACT.DAT", "r");
  	    if (infile == NULL)
   	        erreur("Fichier d'importation des factures non trouvé (FACT.DAT)", 0, 0);
  	    else
        {
            setmode(fileno(infile), O_BINARY);

            entree = getc(infile);
  	        if (entree == EOF)
   	            erreur("Le fichier d'importation des factures est vide", 0, 0);
  	        else
            {
                while ((cvtTourner) && (cvtPossible))
  	            {
                    initAffichage();

   	                cvtPossible = emplisBufferFact();
		            pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages();
                    //
                    // Traite les enregistrements ordinaires
                    //
                    if (cvtPossible)
      	                cvtPossible = traiteBufferFact();
                    //
                    // Traite le dernier enregistrement
                    //
                    else if (entree == EOF)
      	                traiteBufferFact();
                }
            }
        }
    }
    //
    // Conversion par utilisateur
    //
    else
    {
        bool bTourner = true;
        int  iCompteur = 1;
        char cCompteur[4];
        string sFichier;
        /*
        while (bTourner)
        {
            numacar(cCompteur, iCompteur, 3);
            sFichier = cCompteur + "FACT";

            infile = fopen(sFichier.c_str(), "r");
  	        if (infile == NULL)
   	            erreur("Fichier d'importation des factures non trouvé (FACT.DAT)", 0, 0);
  	        else
            {
                setmode(fileno(infile), O_BINARY);

                entree = getc(infile);
  	            if (entree == EOF)
   	                erreur("Le fichier d'importation des factures est vide", 0, 0);
  	            else
                {
                    while ((cvtTourner) && (cvtPossible))
  	                {
                        initAffichage();

   	                    cvtPossible = emplisBufferFact();
		                pSuper->pNSApplication->PumpWaitingMessages();
                        //
                        // Traite les enregistrements ordinaires
                        //
                        if (cvtPossible)
      	                    cvtPossible = traiteBufferFact(string(cCompteur));
                        //
                        // Traite le dernier enregistrement
                        //
                        else if (entree == EOF)
      	                    traiteBufferFact(string(cCompteur));
                    }
                }
            }
        } */
    }
    //
    // Conversion des echographes
    //
    /*infile = fopen("ECHOGRAF.DAT", "r");
  	if (infile == NULL)
   	    erreur("Fichier d'importation des échographes non trouvé", 0, 0);
  	else
    {
        setmode(fileno(infile), O_BINARY);

        entree = getc(infile);
  	    if (entree == EOF)
   	        erreur("Le fichier d'importation des échographes est vide", 0, 0);
  	    else
        {
            while ((cvtTourner) && (cvtPossible))
  	        {
                initAffichage();

   	            cvtPossible = emplisBufferEcho();
		        pSuper->pNSApplication->PumpWaitingMessages();
                //
                // Traite les enregistrements ordinaires
                //
                if (cvtPossible)
      	            cvtPossible = traiteBufferEcho();
                //
                // Traite le dernier enregistrement
                //
                else if (entree == EOF)
      	            traiteBufferEcho();
            }
        }
    } */
}

//---------------------------------------------------------------------------
//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsCvtComptDlg::stoppeCvt()
{
    cvtTourner = false;
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsCvtComptDlg::emplisBufferCompt()
{
    bool tourner = true;
    int  champ, taille_champ, j;
    char buffer[100];

    for (champ = 0; (champ < 2) && (tourner); champ++)
    {
   	    switch (champ)
        {
      	    case  0 : taille_champ =  2; break;
            case  1 : taille_champ = 12; break;
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
            case  0 : strcpy(pBufCompt->V2date,    buffer);
                      pEditCode->SetText(buffer);
                      break;
           /* case  1 : strcpy(pBufCompt->V2libelle, buffer);
                      pEditNom->SetText(buffer);
                      break; */
        }
    }

    return tourner;
}

//---------------------------------------------------------------------------
//  Dispatche le buffer dans les tables
//---------------------------------------------------------------------------
bool
NsCvtComptDlg::traiteBufferCompt()
{
    int i, j;

    string sCode;
   /*
	pMatImport->pDonnees->metAZero();

    switch(pBufEndo->V2code[0])
    {
        case 'G' : strcpy(pMatImport->pDonnees->type, "OENHA");
                   break;
        case 'P' : strcpy(pMatImport->pDonnees->type, "OENBA");
                   break;
    }
    strcpy(pMatImport->pDonnees->utilisateur, "___");
    strcpy(pMatImport->pDonnees->code,        "00?");
    pMatImport->pDonnees->code[2] = pBufEndo->V2code[1];

    strcpy(pMatImport->pDonnees->libelle, pBufEndo->V2libelle);
    strcpy(pMatImport->pDonnees->actif,   "1");

    pMatImport->lastError = pMatImport->appendRecord();
    if (pMatImport->lastError != DBIERR_NONE)
    {
        erreur("Erreur à l'ajout de la fiche matériel", 0, pMatImport->lastError);
        return false;
    }  */
}

//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsCvtComptDlg::emplisBufferFact()
{
/*    bool tourner = true;
    int  champ, taille_champ, j;
    char buffer[100];

    for (champ = 0; (champ < 3) && (tourner); champ++)
    {
   	    switch (champ)
        {
      	    case  0 : taille_champ =  1; break;
            case  1 : taille_champ = 35; break;
            case  2 : taille_champ =  8; break;
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
            case  0 : strcpy(pBufEcho->V2code,    buffer);
                      pEditCode->SetText(buffer);
                      break;
            case  1 : strcpy(pBufEcho->V2libelle, buffer);
                      pEditNom->SetText(buffer);
                      break;
            case  2 : strcpy(pBufEcho->V2date, buffer);
                      break;
        }
    }

    return tourner; */
}

//---------------------------------------------------------------------------
//  Dispatche le buffer dans les tables
//---------------------------------------------------------------------------
bool
NsCvtComptDlg::traiteBufferFact()
{
  /*  int i, j;

    string sCode;

	pMatImport->pDonnees->metAZero();

    strcpy(pMatImport->pDonnees->type,        "OECHO");
    strcpy(pMatImport->pDonnees->utilisateur, "___");
    strcpy(pMatImport->pDonnees->code,        "00?");
    pMatImport->pDonnees->code[2] = pBufEcho->V2code[0];

    strcpy(pMatImport->pDonnees->libelle,   pBufEcho->V2libelle);
    strcpy(pMatImport->pDonnees->mise_svce, pBufEcho->V2date);
    strcpy(pMatImport->pDonnees->actif,     "1");

    pMatImport->lastError = pMatImport->appendRecord();
    if (pMatImport->lastError != DBIERR_NONE)
    {
        erreur("Erreur à l'ajout de la fiche matériel", 0, pMatImport->lastError);
        return false;
    } */
}


//---------------------------------------------------------------------------
//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsCvtComptDlg::initAffichage()
{
    pEditCode->SetText("");
    pEditNom->SetText("");
}
