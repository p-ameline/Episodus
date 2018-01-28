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
#include "nsoutil\importPat.h"
#include "nsbb\nsednum.h"
#include "nautilus\nsdocref.h"

#ifndef _MUE
NSPatientImport::NSPatientImport(NSContexte* pCtx)
                :NSPatient(pCtx)
{
}

bool
NSPatientImport::creeTout()
{
    CreeChemiseDefaut("défaut");
    CreePOMRindex();
    CreeDocumentZero();
}

bool
NSPatientImport::CreeChemiseDefaut(string sNomChemise)
{
try
{
	//
    // Création d'une nouvelle chemise
    //
    NSChemise* pNewChemise = new NSChemise(NSPatInfo::pContexte);
    pNewChemise->lastError = pNewChemise->open();
    if (pNewChemise->lastError != DBIERR_NONE)
    {
        erreur("Il est impossible d'ouvrir le fichier des Chemises.", 0, pNewChemise->lastError, NSPatInfo::pContexte->GetMainWindow()->GetHandle());
        delete pNewChemise;
        return false;
    }

    pNewChemise->pDonnees->metAZero();

    char codeChemise[CHE_CODE_LEN + 1];    strcpy(codeChemise, pDonnees->nss);
    strcat(codeChemise, "001");

    char creation[CHE_CREATION_LEN + 1];
    donne_date_duJour(creation);
    strcpy(pNewChemise->pDonnees->code,     codeChemise);
    strcpy(pNewChemise->pDonnees->nom, 	    sNomChemise.c_str());
    strcpy(pNewChemise->pDonnees->creation, creation);
    strcpy(pNewChemise->pDonnees->acces, 	"");
    strcpy(pNewChemise->pDonnees->createur, NSPatInfo::pContexte->getUtilisateur()->pDonnees->indice);

    pNewChemise->lastError = pNewChemise->appendRecord();
    if (pNewChemise->lastError != DBIERR_NONE)
    {
   	    erreur("Il est impossible d'ajouter une nouvelle Chemise.", 0, pNewChemise->lastError);
        return false;
    }

    pNewChemise->lastError = pNewChemise->close();
    if (pNewChemise->lastError != DBIERR_NONE)
   	    erreur("Erreur de fermeture du fichier des chemises.", 0, pNewChemise->lastError, NSPatInfo::pContexte->GetMainWindow()->GetHandle());

    delete pNewChemise;
    return true;
}
catch (...)
{
    erreur("Exception CreeChemiseDefaut.", 0, 0);
    return false;
}
}

bool
NSPatientImport::CreeDocumentZero()
{
try
{
	//
	// ------------- CREATION DE LA FICHE DE SYNTHESE --------------------
	//
	// On sauvegarde d'abord la synthèse en tant que nouveau document, puis
	// on crée un NSCSDocument à partir de ce document.
	// On initialise le NSCSDocument avec l'ammorce "ZSYNT1" et on lui demande
	// de s'enregistrer

	//
	// Création d'une fiche d'accès au fichier des Documents
	//
	NSDocument* pNewDocument = new NSDocument(NSPatInfo::pContexte);
	//
	// Ouverture du fichier
	//
	pNewDocument->lastError = pNewDocument->open();
	if (pNewDocument->lastError != DBIERR_NONE)
	{
    erreur("Il est impossible d'ouvrir le fichier des Documents.", 0, pNewDocument->lastError, NSPatInfo::pContexte->GetMainWindow()->GetHandle());
    delete pNewDocument;
    return false;
	}
	//
	// Initialisation de la fiche
	//
	pNewDocument->pDonnees->metAZero();

	// Patient : Patient en cours
	strcpy(pNewDocument->pDonnees->codePatient, pDonnees->nss);
	// Code : par définition "0000"
	strcpy(pNewDocument->pDonnees->codeDocument,
                     (string(DOC_CODE_DOCUM_LEN, '0')).c_str());

	//
	// Niveau de sécurité pour l'accès : Celui de l'utilisateur
	strcpy(pNewDocument->pDonnees->acces, NSPatInfo::pContexte->getUtilisateur()->pDonnees->groupe);
	// Créateur : Utilisateur en cours
	strcpy(pNewDocument->pDonnees->createur, NSPatInfo::pContexte->getUtilisateur()->pDonnees->indice);
	// Type de document : Fiche de consultation
	strcpy(pNewDocument->pDonnees->type, "CS030");
	// Intêret 5 = moyen
	strcpy(pNewDocument->pDonnees->interet, "5");
	// Date de creation = date du jour
	char creation[DOC_CREATION_LEN + 1];
	donne_date_duJour(creation);
	strcpy(pNewDocument->pDonnees->creation, creation);
	// visible est à 1 quand on référence un document
	pNewDocument->pDonnees->rendVisible();
	// Copie du nom du document par défaut
	strncpy(pNewDocument->pDonnees->nom, "Synthèse", DOC_NOM_LEN);
	//
	// Ajout de la fiche au fichier des Documents
	//
	pNewDocument->lastError = pNewDocument->appendRecord();
	if (pNewDocument->lastError != DBIERR_NONE)
	{
		erreur("Il est impossible de mettre à jour la base des Documents.", 0,
                             pNewDocument->lastError, NSPatInfo::pContexte->GetMainWindow()->GetHandle());
		pNewDocument->close();
		delete pNewDocument;
		return false;
	}
	//
	// Enregistrement de la patpatho réduite à l'ammorce "ZSYNT1"
	// On utilise un NSRefDocument et non un NSCSDocument parce qu'un
	// NSCSDocument lance Open dans son constructeur
	//
	NSDocumentInfo* pDocInfo = new NSDocumentInfo(pNewDocument);
	pNewDocument->close();
	delete pNewDocument;

	//
	// Ne pas passer pDocInfo dans le cteur pour éviter qu'il essaie de
	// charger la patpatho
	//
	NSNoyauDocument* pDocSynth = new NSNoyauDocument(0, 0, 0, NSPatInfo::pContexte, false) ;
	pDocSynth->pDocInfo = new NSDocumentInfo(*pDocInfo);
	pDocSynth->pPatPathoArray->ajoutePatho("ZSYNT1", 0);

	pDocSynth->enregistrePatPatho();

	delete pDocSynth;

	delete pDocInfo;

	return true;}
catch (const exception& e)
{
    string sExept = "Exception CreeDocumentZero " + string(e.what());
    erreur(sExept.c_str(), 0, 0);
    return false;
}
catch (...)
{
    erreur("Exception CreeDocumentZero.", 0, 0);
    return false;
}
}

bool
NSPatientImport::CreePOMRindex()
{
try
{
    //
    // ------------- CREATION DE L'INDEX POMR --------------------
    //
    //
    // Création d'une fiche d'accès au fichier des Documents
    //
    NSDocument* pNewDocument = new NSDocument(NSPatInfo::pContexte);
    //    // Ouverture du fichier
    //
	pNewDocument->lastError = pNewDocument->open();
    if (pNewDocument->lastError != DBIERR_NONE)
    {
   	    erreur("Il est impossible d'ouvrir le fichier des Documents.", 0, pNewDocument->lastError, NSPatInfo::pContexte->GetMainWindow()->GetHandle());
        delete pNewDocument;
        return false;
    }
    //
    // On vérifie que ce document n'est pas déjà créé
    //
   	string codeDocu =  string(pDonnees->nss) + string(DOC_CODE_DOCUM_LEN, '-');

	pNewDocument->lastError = pNewDocument->chercheClef(&codeDocu,
                                                        "",
                                                        0,
                                                        keySEARCHEQ,
                                                        dbiWRITELOCK);    //
    // Ce document existe déjà
    //
	if (pNewDocument->lastError == DBIERR_NONE)
	{
		delete pNewDocument;
		return true;
	}
    //
    // La table des documents a un problème
    //
    if (pNewDocument->lastError != DBIERR_RECNOTFOUND)
	{
		erreur("La table des documents n'est pas utilisable.", 0,
            pNewDocument->lastError, NSPatInfo::pContexte->GetMainWindow()->GetHandle());
		delete pNewDocument;
		return false;
	}
   	//
	// Si on est encore là, c'est qu'il faut créer le document
    // Still there ? It means we have to create the document
	//

	//
    // Initialisation de la fiche
    //
    pNewDocument->pDonnees->metAZero();

    // Patient : Patient en cours
    strcpy(pNewDocument->pDonnees->codePatient, pDonnees->nss);
    // Code : par définition "0000"
    strcpy(pNewDocument->pDonnees->codeDocument,
                     (string(DOC_CODE_DOCUM_LEN, '-')).c_str());

    //
    // Niveau de sécurité pour l'accès : Celui de l'utilisateur
    strcpy(pNewDocument->pDonnees->acces, NSPatInfo::pContexte->getUtilisateur()->pDonnees->groupe);
    // Créateur : Utilisateur en cours
    strcpy(pNewDocument->pDonnees->createur, NSPatInfo::pContexte->getUtilisateur()->pDonnees->indice);
    // Type de document : Fiche de consultation
    strcpy(pNewDocument->pDonnees->type, "CS030");
    // Intêret 5 = moyen
    strcpy(pNewDocument->pDonnees->interet, "5");
    // Date de creation = date du jour
    char creation[DOC_CREATION_LEN + 1];
    donne_date_duJour(creation);
    strcpy(pNewDocument->pDonnees->creation, creation);
    // visible est à 1 quand on référence un document
    pNewDocument->pDonnees->rendVisible();
    // Copie du nom du document par défaut
    strncpy(pNewDocument->pDonnees->nom, "Index de santé", DOC_NOM_LEN);
    //
	// Ajout de la fiche au fichier des Documents
	//
	pNewDocument->lastError = pNewDocument->appendRecord();
	if (pNewDocument->lastError != DBIERR_NONE)
    {
        erreur("Il est impossible de mettre à jour la base des Documents.", 0,
                             pNewDocument->lastError, NSPatInfo::pContexte->GetMainWindow()->GetHandle());
        pNewDocument->close();
        delete pNewDocument;
		return false;
    }
	//
    // Enregistrement de la patpatho réduite à l'ammorce "ZPOMR1"
    // On utilise un NSRefDocument et non un NSCSDocument parce qu'un
    // NSCSDocument lance Open dans son constructeur
    //
    NSDocumentInfo* pDocInfo = new NSDocumentInfo(pNewDocument);
    pNewDocument->close();
    delete pNewDocument;

    //
    // Ne pas passer pDocInfo dans le cteur pour éviter qu'il essaie de
    // charger la patpatho
    //
	NSNoyauDocument* pPbIndex = new NSNoyauDocument(0, 0, 0, NSPatInfo::pContexte, false);
    pPbIndex->pDocInfo = new NSDocumentInfo(*pDocInfo);
    pPbIndex->pPatPathoArray->ajoutePatho("ZPOMR1", 0, 0);
    pPbIndex->pPatPathoArray->ajoutePatho("0PRO11", 1, 1);
    pPbIndex->pPatPathoArray->ajoutePatho("0OBJE1", 1, 1);
    pPbIndex->pPatPathoArray->ajoutePatho("N00001", 1, 1) ;

    pPbIndex->enregistrePatPatho();

    delete pPbIndex;

    delete pDocInfo;
    return true;
}
catch (const exception& e)
{
    string sExept = "Exception CreePOMRindex " + string(e.what());
    erreur(sExept.c_str(), 0, 0);
    return false;
}
catch (...)
{
    erreur("Exception CreePOMRindex.", 0, 0);
    return false;
}
}
#endif


#ifndef _MUE
// -----------------------------------------------------------------
//
//  Méthodes de NsImportCocciPatientDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsImportCocciPatientDlg, TDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NsImportCocciPatientDlg::NsImportCocciPatientDlg(TWindow* pere, NSContexte* pCtx)
				  	    :NSUtilDialog(pere, pCtx, "CVT_COCCI_ADMIN", pNSResModule)
{
    pEditNSS 	    = new TEdit(this, IDC_CVT_NSS);
    pEditCode 	    = new TEdit(this, IDC_CVT_CODE);
    pEditNom 	    = new TEdit(this, IDC_CVT_NOM);
    pEditPrenom     = new TEdit(this, IDC_CVT_PRENOM);

    pPatImport      = new NSPatientImport(pContexte);
  	pPcoImport      = new NSPatCor(pContexte);
  	pAdrImport      = new NSAdresses(pContexte);
    pCLiImport      = new NSCorLibre(pContexte);

    pNewChemise     = new NSChemise(pContexte);
    pNewDocument    = new NSDocument(pContexte);
    pData 		    = new NSPatPaDat(pContexte);
    pLoca 		    = new NSPatPaLoc(pContexte);

    pBufPat         = new CocciPatient;

    cvtPossible     = true;

    bAppend         = true;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsImportCocciPatientDlg::~NsImportCocciPatientDlg()
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

    pNewChemise->close();
    delete pNewChemise;
    pNewDocument->close();
    delete pNewDocument;
    pData->close();
    delete pData;
    pLoca->close();
    delete pLoca;

    delete pBufPat;

    //if (infile != NULL)
    //    fclose(infile);

    if (inFile)
        inFile.close();
}

//---------------------------------------------------------------------------//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsImportCocciPatientDlg::SetupWindow()
{
    TDialog::SetupWindow();
    //
    // Ouverture du fichier d'importation
    //
    inFile.open("ADMINISTRATIF.TXT");
    if (!inFile)
    {
   	    erreur("Fichier d'importation non trouvé", 0, 0);
	  	cvtPossible = false;
  	}

    getline(inFile, sLine);
    if (inFile.eof())
    {
   	    erreur("Fichier d'importation vide", 0, 0);
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
    }
    if (cvtPossible)
    {
        pPcoImport->lastError = pPcoImport->open();
  		if (pPcoImport->lastError != DBIERR_NONE)
   	    {
      	    erreur("Impossible d'ouvrir le fichier patcor", 0, pPcoImport->lastError);
            cvtPossible = false;        }
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

//---------------------------------------------------------------------------//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsImportCocciPatientDlg::demarreCvt()
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
        else if (inFile.eof())
      	    traiteBuffer();
    }
}

//---------------------------------------------------------------------------//  Arrêt de la conversion en boucle
//---------------------------------------------------------------------------
void
NsImportCocciPatientDlg::stoppeCvt()
{
   cvtTourner = false;
}

//---------------------------------------------------------------------------//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsImportCocciPatientDlg::emplisBuffer()
{
    bool tourner = true;
    int  champ, taille_champ, j;
    char buffer[100];

    if (strlen(sLine.c_str()) == 343)
    {
        int i = 0;
	    for (champ = 0; (champ < 25) && (tourner); champ++)
        {
   	        switch (champ)
            {
      	        case  0 : taille_champ = 20;    break;
                case  1 : taille_champ = 11;    break;
                case  2 : taille_champ = 20;    break;
                case  3 : taille_champ = 20;    break;
                case  4 : taille_champ = 10;    break;
                case  5 : taille_champ = 60;    break;
                case  6 : taille_champ =  5;    break;
                case  7 : taille_champ = 20;    break;
                case  8 : taille_champ = 15;    break;
                case  9 : taille_champ = 15;    break;
                case 10 : taille_champ = 20;    break;
                case 11 : taille_champ =  1;    break;
                case 12 : taille_champ = 25;    break;
                case 13 : taille_champ =  1;    break;
                case 14 : taille_champ = 10;    break;
                case 15 : taille_champ =  3;    break;
                case 16 : taille_champ =  5;    break;
                case 17 : taille_champ =  5;    break;
                case 18 : taille_champ =  5;    break;
                case 19 : taille_champ =  5;    break;
                case 20 : taille_champ =  5;    break;
                case 21 : taille_champ =  2;    break;
                case 22 : taille_champ = 20;    break;
                case 23 : taille_champ = 20;    break;
                case 24 : taille_champ = 20;    break;
            }

            for (j = 0; j < taille_champ; j++, i++)
                buffer[j] = sLine[i];
            buffer[j] = '\0';

            // OemToAnsi(buffer, buffer);

            switch (champ)
            {
      	        case  0 :   strcpy(pBufPat->CocciNom,       buffer);
                            pEditNom->SetText(buffer);
                            break;
                case  1 :   strcpy(pBufPat->CocciNumpa,     buffer);
                            pEditNSS->SetText(buffer);
                            break;
                case  2 :   strcpy(pBufPat->CocciPrenom,    buffer);
                            pEditPrenom->SetText(buffer);
                            break;
                case  3 :   strcpy(pBufPat->CocciNomJF,     buffer);    break;
                case  4 :   strcpy(pBufPat->CocciDateNaiss, buffer);    break;
                case  5 :   strcpy(pBufPat->CocciAdresse,   buffer);    break;
                case  6 :   strcpy(pBufPat->CocciCp,        buffer);    break;
                case  7 :   strcpy(pBufPat->CocciVille,     buffer);    break;
                case  8 :   strcpy(pBufPat->CocciTel_dom,   buffer);    break;
                case  9 :   strcpy(pBufPat->CocciTel_bur,   buffer);    break;
                case 10 :   strcpy(pBufPat->CocciNoSecu,    buffer);    break;
                case 11 :   strcpy(pBufPat->CocciSexe,      buffer);    break;
                case 12 :   strcpy(pBufPat->CocciActivite,  buffer);    break;
                case 13 :   strcpy(pBufPat->CocciALD,       buffer);    break;
                case 14 :   strcpy(pBufPat->CocciSit_fam,   buffer);    break;
                case 15 :   strcpy(pBufPat->CocciGrSng,     buffer);    break;
                case 16 :   strcpy(pBufPat->CocciTaille,    buffer);    break;
                case 17 :   strcpy(pBufPat->CocciPoids,     buffer);    break;
                case 18 :   strcpy(pBufPat->CocciPouls,     buffer);    break;
                case 19 :   strcpy(pBufPat->CocciTaMin,     buffer);    break;
                case 20 :   strcpy(pBufPat->CocciTaMax,     buffer);    break;
                case 21 :   strcpy(pBufPat->CocciNbEnfant,  buffer);    break;
                case 22 :   strcpy(pBufPat->CocciAllergie1, buffer);    break;
                case 23 :   strcpy(pBufPat->CocciAllergie2, buffer);    break;
                case 24 :   strcpy(pBufPat->CocciAllergie3, buffer);    break;
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
NsImportCocciPatientDlg::traiteBuffer()
{
    int i, j;
    int idRet;
    string sCode;
    string nom, prenom, sexe1, date1, titre1, sexe2, date2, titre2;
    string sNss;

	//
    // Initialisation de la fiche patient Nautilus
    //
    pPatImport->pDonnees->metAZero();

    if (existePatient(nom,prenom,sexe1,date1,sexe2,date2))
    {
        char msg[255];

        // s'il s'agit d'un pur doublon : on skippe
        if ((date1 == date2) && (sexe1 == sexe2))
            return true;

        if (sexe1[0] == '1')
            titre1 = "M.";
        else
            titre1 = "Mme.";

        if (sexe2[0] == 'M')
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

    int typeCode;
    if (pBufPat->CocciSexe[0] == 'M')
        typeCode = tCodePatient1;
    else
        typeCode = tCodePatient2;

    if (pPatImport->LastCode(&sNss, typeCode))
        strcpy(pPatImport->pDonnees->nss, sNss.c_str());
    else
        return true;

    //
    // Si le NSS est blanc, on ne traite pas la fiche
    //
    if (strcmp(pPatImport->pDonnees->nss, "      ") == 0)
   	    return true;

    strcpy(pPatImport->pDonnees->nom,    pBufPat->CocciNom);
    ote_blancs(pPatImport->pDonnees->nom);

    strcpy(pPatImport->pDonnees->prenom, pBufPat->CocciPrenom);
    ote_blancs(pPatImport->pDonnees->prenom);

    strcpy(pPatImport->pDonnees->code, "Cocci");
    strcat(pPatImport->pDonnees->code, pBufPat->CocciNumpa);
    ote_blancs(pPatImport->pDonnees->code);

    if      (pBufPat->CocciSexe[0] == 'M')
        strcpy(pPatImport->pDonnees->sexe, "1");
    else if (pBufPat->CocciSexe[0] == 'F')
        strcpy(pPatImport->pDonnees->sexe, "2");

    string sDateNaiss;
    donne_date_inverse(string(pBufPat->CocciDateNaiss), sDateNaiss, 33);
    strcpy(pPatImport->pDonnees->naissance, sDateNaiss.c_str());
    ote_blancs(pPatImport->pDonnees->naissance);

    strcpy(pPatImport->pDonnees->telebur, pBufPat->CocciTel_bur);
    ote_blancs(pPatImport->pDonnees->telebur);

    ote_blancs(pBufPat->CocciSit_fam);
    if      (strcmp(pBufPat->CocciSit_fam, "Marié(e)") == 0)
        strcpy(pPatImport->pDonnees->sitfam, "M");
    else if (strcmp(pBufPat->CocciSit_fam, "Divorcé(e)") == 0)
        strcpy(pPatImport->pDonnees->sitfam, "D");
    else
        strcpy(pPatImport->pDonnees->sitfam, " ");

    ote_blancs(pBufPat->CocciNbEnfant);
    if (pBufPat->CocciNbEnfant[0] != '\0')
    {
        int iNbEnf = -1;
        int i = donneDigit(pBufPat->CocciNbEnfant[0]);
        if (i != -1)
        {
            iNbEnf = i;
            if (pBufPat->CocciNbEnfant[1] != '\0')
            {
                i = donneDigit(pBufPat->CocciNbEnfant[1]);
                if (i != -1)
                {
                    iNbEnf = 10 * iNbEnf + i;
                }
            }
        }
        pPatImport->pDonnees->setNbEnfants(iNbEnf);
    }

    //
    // Création d'une adresse Nautilus
    //
    pAdrImport->pDonnees->metAZero();

    ote_blancs(pBufPat->CocciAdresse);

    if (pBufPat->CocciAdresse[0] != '\0')
    {
        int iTailleAdresse = strlen(pBufPat->CocciAdresse);

        if (iTailleAdresse <= ADR_ADRESSE1_LEN)
            strcpy(pAdrImport->pDonnees->adresse1, pBufPat->CocciAdresse);
        else
        {
            string sAdresse = pBufPat->CocciAdresse;
            int iPremBlanc = ADR_ADRESSE1_LEN;
            while ((iPremBlanc > 0) && (sAdresse[iPremBlanc] != ' '))
                iPremBlanc--;

            int iNextStart = iPremBlanc + 1;
            if (iPremBlanc == 0)
            {
                iPremBlanc = ADR_ADRESSE1_LEN;
                iNextStart--;
            }

            string sAdresse1 = string(sAdresse, 0, iPremBlanc);
            sAdresse  = string(sAdresse, iNextStart, strlen(sAdresse.c_str()) - iNextStart);

            strcpy(pAdrImport->pDonnees->adresse1, sAdresse1.c_str());

            if (strlen(sAdresse.c_str()) <= ADR_ADRESSE2_LEN)
                strcpy(pAdrImport->pDonnees->adresse2, sAdresse.c_str());
            else
            {
                iPremBlanc = ADR_ADRESSE2_LEN;
                while ((iPremBlanc > 0) && (sAdresse[iPremBlanc] != ' '))
                    iPremBlanc--;

                iNextStart = iPremBlanc + 1;
                if (iPremBlanc == 0)
                {
                    iPremBlanc = ADR_ADRESSE2_LEN;
                    iNextStart--;
                }

                sAdresse1 = string(sAdresse, 0, iPremBlanc);
                strcpy(pAdrImport->pDonnees->adresse2, sAdresse1.c_str());

                int iTaille3 = min(int(strlen(sAdresse.c_str()) - iNextStart), ADR_ADRESSE3_LEN);
                sAdresse  = string(sAdresse, iNextStart, iTaille3);
                strcpy(pAdrImport->pDonnees->adresse3, sAdresse.c_str());
            }
        }
    }
    ote_blancs(pBufPat->CocciCp);
    strcpy(pAdrImport->pDonnees->code_post, pBufPat->CocciCp);
    ote_blancs(pBufPat->CocciVille);
    strcpy(pAdrImport->pDonnees->ville,     pBufPat->CocciVille);
    ote_blancs(pBufPat->CocciTel_dom);
    strcpy(pAdrImport->pDonnees->telephone, pBufPat->CocciTel_dom);

    if ((pAdrImport->pDonnees->adresse1[0]  != '\0')    ||
   	    (pAdrImport->pDonnees->code_post[0] != '\0')    ||
        (pAdrImport->pDonnees->ville[0]     != '\0')    ||
        (pAdrImport->pDonnees->telephone[0] != '\0'))
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
            return true;
        }
    }

    //
    // ----------- CREATION DES DOCUMENTS, CHEMISES... -------
    //
    pPatImport->creeTout();

    return true;
}

bool
NsImportCocciPatientDlg::initLastCodes()
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
        erreur("Erreur à l'accès à la fiche patient précédente.", 0, pPatImport->lastError);        return false;    }

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

//---------------------------------------------------------------------------//  Recherche l'existence d'un doublon
//---------------------------------------------------------------------------
bool
NsImportCocciPatientDlg::existePatient(string& nom, string& prenom, string& sexe1, string& date1, string& sexe2, string& date2)
{
    DBIResult   ErrDBI;
    CURProps    curProps;
    Byte*       pIndexRec;
    char        cDate1[11] = "", cDate2[11] = "";

    nom = ""; prenom = "";
    sexe1 = ""; date1 = "";
    sexe2 = ""; date2 = "";

    // on recherche ce patient dans la base pour
    // vérifier qu'il n'existe pas déjà
    ErrDBI = DbiGetCursorProps(pPatImport->PrendCurseur(), curProps);
    pIndexRec = new Byte[curProps.iRecBufSize];
    memset(pIndexRec, 0, curProps.iRecBufSize);
    DbiPutField(pPatImport->PrendCurseur(), PAT_NOM_FIELD, pIndexRec, (Byte*)pBufPat->CocciNom);
    DbiPutField(pPatImport->PrendCurseur(), PAT_PRENOM_FIELD, pIndexRec, (Byte*)pBufPat->CocciPrenom);

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

        nom = string(pPatImport->pDonnees->nom);
        prenom = string(pPatImport->pDonnees->prenom);
        sexe1 = string(pPatImport->pDonnees->sexe);
        date1 = string(cDate1);
        sexe2 = string(1, pBufPat->CocciSexe[0]);
        date2 = string(pBufPat->CocciDateNaiss);

        return true;
    }
    else if (ErrDBI != DBIERR_RECNOTFOUND)
        erreur("Erreur à la recherche dans la base patient.db", 0, ErrDBI);

    return false;
}

//---------------------------------------------------------------------------//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsImportCocciPatientDlg::initAffichage()
{
	pEditNSS->SetText("");
    pEditCode->SetText("");
    pEditNom->SetText("");
    pEditPrenom->SetText("");
}
#endif

#ifndef _MUE
// -----------------------------------------------------------------
//
//  Méthodes de NsImportDPIOPatientDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsImportDPIOPatientDlg, TDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, demarreCvt),   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, stoppeCvt),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NsImportDPIOPatientDlg::NsImportDPIOPatientDlg(TWindow* pere, NSContexte* pCtx)
                       :NSUtilDialog(pere, pCtx, "CVT_COCCI_ADMIN", pNSResModule)
{
    pEditNSS 	    = new TEdit(this, IDC_CVT_NSS);
    pEditCode 	    = new TEdit(this, IDC_CVT_CODE);
    pEditNom 	    = new TEdit(this, IDC_CVT_NOM);
    pEditPrenom     = new TEdit(this, IDC_CVT_PRENOM);

    pPatImport      = new NSPatientImport(pContexte);
  	pPcoImport      = new NSPatCor(pContexte);
  	pAdrImport      = new NSAdresses(pContexte);
    pCLiImport      = new NSCorLibre(pContexte);

    pNewChemise     = new NSChemise(pContexte);
    pNewDocument    = new NSDocument(pContexte);
    pData 		    = new NSPatPaDat(pContexte);
    pLoca 		    = new NSPatPaLoc(pContexte);

    pBufPat         = new DPIOPatient;

    cvtPossible     = true;

    bAppend         = true;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsImportDPIOPatientDlg::~NsImportDPIOPatientDlg()
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

    pNewChemise->close();
    delete pNewChemise;
    pNewDocument->close();
    delete pNewDocument;
    pData->close();
    delete pData;
    pLoca->close();
    delete pLoca;

    delete pBufPat;

    //if (infile != NULL)
    //    fclose(infile);

    if (inFile)
        inFile.close();
}

//---------------------------------------------------------------------------//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsImportDPIOPatientDlg::SetupWindow()
{
    TDialog::SetupWindow();
    //
    // Ouverture du fichier d'importation
    //
    inFile.open("Patient_DPIO.txt");
    if (!inFile)
    {
   	    erreur("Fichier d'importation \"Patient_DPIO.txt\" non trouvé", 0, 0);
	  	cvtPossible = false;
  	}

    getline(inFile, sLine);
    if (inFile.eof())
    {
   	    erreur("Fichier d'importation \"Table_Patient_Test_DPIO.TXT\" vide", 0, 0);
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
    }
    if (cvtPossible)
    {
        pPcoImport->lastError = pPcoImport->open();
  		if (pPcoImport->lastError != DBIERR_NONE)
   	    {
      	    erreur("Impossible d'ouvrir le fichier patcor", 0, pPcoImport->lastError);
            cvtPossible = false;        }
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

//---------------------------------------------------------------------------//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsImportDPIOPatientDlg::demarreCvt()
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
        else if (inFile.eof())
      	    traiteBuffer();
    }
}

//---------------------------------------------------------------------------//  Arrêt de la conversion en boucle
//---------------------------------------------------------------------------
void
NsImportDPIOPatientDlg::stoppeCvt()
{
   cvtTourner = false;
}

//---------------------------------------------------------------------------//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
bool
NsImportDPIOPatientDlg::emplisBuffer()
{
    bool tourner = true ;
    int  champ, taille_champ, j, taille_ligne ;
    char buffer[100] ;

    pBufPat->DPIONom[0]         = '\0' ;
    pBufPat->DPIOPrenom[0]      = '\0' ;
    pBufPat->DPIOSexe[0]        = '\0' ;
    pBufPat->DPIODateNaiss[0]   = '\0' ;
    pBufPat->DPIOCode[0]        = '\0' ;

    if (sLine == "")
        return true;
    taille_ligne = strlen(sLine.c_str()) ;

    int i = 0;
    for (champ = 0; (champ < 5) && (tourner); champ++)
    {
        switch (champ)
        {
            case  0 : taille_champ = 35;    break;
            case  1 : taille_champ = 35;    break;
            case  2 : taille_champ = 2;     break;
            case  3 : taille_champ = 10;    break;
            case  4 : taille_champ = 20;    break;
        }

        for (j = 0; (j < taille_champ) && (i < taille_ligne) && (sLine[i] != '\t'); j++, i++)
            buffer[j] = sLine[i];
        buffer[j] = '\0';

        // OemToAnsi(buffer, buffer);

        switch (champ)
        {
            case  0 :   strcpy(pBufPat->DPIONom,        buffer);
                        pEditNom->SetText(buffer);
                        break;
            case  1 :   strcpy(pBufPat->DPIOPrenom,     buffer);
                        pEditPrenom->SetText(buffer);
                        break;
            case  2 :   strcpy(pBufPat->DPIOSexe,       buffer);    break;
            case  3 :   strcpy(pBufPat->DPIODateNaiss,  buffer);    break;
            case  4 :   strcpy(pBufPat->DPIOCode,       buffer);    break;
        }

        if (i >= taille_ligne)
            break ;

        if (sLine[i] == '\t')
            i++ ;
        //
        // Cas où la donnée est plus longue que prévu
        //
        else
        {
            for ( ; (i < taille_ligne) && (sLine[i] != '\t'); i++) ;
            if (i >= taille_ligne)
                break ;
            if (sLine[i] == '\t')
                i++ ;
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
NsImportDPIOPatientDlg::traiteBuffer()
{
    int i, j;
    int idRet;
    string sCode;
    string nom, prenom, sexe1, date1, titre1, sexe2, date2, titre2;
    string sNss;

	//
    // Initialisation de la fiche patient Nautilus
    //
    pPatImport->pDonnees->metAZero();

    if (existePatient(nom,prenom,sexe1,date1,sexe2,date2))
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

    int typeCode;
    if (pBufPat->DPIOSexe[0] == '1')
        typeCode = tCodePatient1;
    else
        typeCode = tCodePatient2;

    if (pPatImport->LastCode(&sNss, typeCode))
        strcpy(pPatImport->pDonnees->nss, sNss.c_str());
    else
        return true;

    //
    // Si le NSS est blanc, on ne traite pas la fiche
    //
    if (strcmp(pPatImport->pDonnees->nss, "      ") == 0)
   	    return true;

    strcpy(pPatImport->pDonnees->nom,    pBufPat->DPIONom);
    ote_blancs(pPatImport->pDonnees->nom);

    strcpy(pPatImport->pDonnees->prenom, pBufPat->DPIOPrenom);
    ote_blancs(pPatImport->pDonnees->prenom);

    strcpy(pPatImport->pDonnees->code, pBufPat->DPIOCode);
    ote_blancs(pPatImport->pDonnees->code);

    if      (pBufPat->DPIOSexe[0] == '1')
        strcpy(pPatImport->pDonnees->sexe, "1");
    else if (pBufPat->DPIOSexe[0] == '2')
        strcpy(pPatImport->pDonnees->sexe, "2");
    else
        strcpy(pPatImport->pDonnees->sexe, "1");

    string sDateNaiss;
    donne_date_inverse(string(pBufPat->DPIODateNaiss), sDateNaiss, 33);
    strcpy(pPatImport->pDonnees->naissance, sDateNaiss.c_str());
    ote_blancs(pPatImport->pDonnees->naissance);
    if (strlen(pPatImport->pDonnees->naissance) != PAT_NAISSANCE_LEN)
        strcpy(pPatImport->pDonnees->naissance, "19000000") ;
    else
    {
        // On vérifie qu'il y a PAT_NAISSANCE_LEN chiffres
        int iNbDigit ;
        for (iNbDigit = 0 ; (iNbDigit < PAT_NAISSANCE_LEN) && (pPatImport->pDonnees->naissance[iNbDigit] >= '0') && (pPatImport->pDonnees->naissance[iNbDigit] <= '9'); iNbDigit++) ;
        if (iNbDigit < PAT_NAISSANCE_LEN)
            strcpy(pPatImport->pDonnees->naissance, "19000000") ;
    }

    // Création d'une adresse Nautilus
    //
    pAdrImport->pDonnees->metAZero();

    if ((pAdrImport->pDonnees->adresse1[0]  != '\0')    ||
   	    (pAdrImport->pDonnees->code_post[0] != '\0')    ||
        (pAdrImport->pDonnees->ville[0]     != '\0')    ||
        (pAdrImport->pDonnees->telephone[0] != '\0'))
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
            return true;
        }
    }

    //
    // ----------- CREATION DES DOCUMENTS, CHEMISES... -------
    //
    pPatImport->creeTout();

    return true;
}

bool
NsImportDPIOPatientDlg::initLastCodes()
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
        erreur("Erreur à l'accès à la fiche patient précédente.", 0, pPatImport->lastError);        return false;    }

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

//---------------------------------------------------------------------------//  Recherche l'existence d'un doublon
//---------------------------------------------------------------------------
bool
NsImportDPIOPatientDlg::existePatient(string& nom, string& prenom, string& sexe1, string& date1, string& sexe2, string& date2)
{
    DBIResult   ErrDBI;
    CURProps    curProps;
    Byte*       pIndexRec;
    char        cDate1[11] = "", cDate2[11] = "";

    nom = ""; prenom = "";
    sexe1 = ""; date1 = "";
    sexe2 = ""; date2 = "";

    // on recherche ce patient dans la base pour
    // vérifier qu'il n'existe pas déjà
    ErrDBI = DbiGetCursorProps(pPatImport->PrendCurseur(), curProps);
    pIndexRec = new Byte[curProps.iRecBufSize];
    memset(pIndexRec, 0, curProps.iRecBufSize);
    DbiPutField(pPatImport->PrendCurseur(), PAT_NOM_FIELD, pIndexRec, (Byte*)pBufPat->DPIONom);
    DbiPutField(pPatImport->PrendCurseur(), PAT_PRENOM_FIELD, pIndexRec, (Byte*)pBufPat->DPIOPrenom);

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

        nom = string(pPatImport->pDonnees->nom);
        prenom = string(pPatImport->pDonnees->prenom);
        sexe1 = string(pPatImport->pDonnees->sexe);
        date1 = string(cDate1);
        sexe2 = string(1, pBufPat->DPIOSexe[0]);
        date2 = string(pBufPat->DPIODateNaiss);

        return true;
    }
    else if (ErrDBI != DBIERR_RECNOTFOUND)
        erreur("Erreur à la recherche dans la base patient.db", 0, ErrDBI);

    return false;
}

//---------------------------------------------------------------------------//  Charge un enregistrement dans le buffer
//---------------------------------------------------------------------------
void
NsImportDPIOPatientDlg::initAffichage()
{
	pEditNSS->SetText("");
    pEditCode->SetText("");
    pEditNom->SetText("");
    pEditPrenom->SetText("");
}

#endif
