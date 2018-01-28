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
#include "nsoutil\nsrepair.h"
#include "nsbb\nsednum.h"
#include "nautilus\nsdocref.h"


#ifndef _MUE
// -----------------------------------------------------------------
//
//  Méthodes de NsImportCocciPatientDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsRepairDocumDlg, NSUtilDialog)
   EV_CHILD_NOTIFY(IDC_CVT_START, BN_CLICKED, repairStart),   EV_CHILD_NOTIFY(IDC_CVT_STOP,  BN_CLICKED, repairStop),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NsRepairDocumDlg::NsRepairDocumDlg(TWindow* pere, NSContexte* pCtx)
                 :NSUtilDialog(pere, pCtx, "IDD_REPAIR_DOCUM", pNSResModule)
{
    pEditNSS    = new TEdit(this, IDC_CVT_NSS);
    pEditCode   = new TEdit(this, IDC_CVT_CODE);
    pEditNom    = new TEdit(this, IDC_CVT_NOM);
    pEditPrenom = new TEdit(this, IDC_CVT_PRENOM);

    bTourner    = false ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsRepairDocumDlg::~NsRepairDocumDlg()
{
    //
	// Suppression de tous les objets
	//
	delete pEditNSS;
    delete pEditCode;
    delete pEditNom;
    delete pEditPrenom;
}

//---------------------------------------------------------------------------//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsRepairDocumDlg::SetupWindow()
{
    TDialog::SetupWindow();
}

//---------------------------------------------------------------------------//  Lancement de la conversion en boucle
//---------------------------------------------------------------------------
void
NsRepairDocumDlg::repairStart()
{
    SetCursor(0, IDC_WAIT) ;

    char creation[9] ;
    donne_date_duJour(creation) ;

    bTourner = true ;

    int iNbRepaired = 0 ;
    pEditCode->SetText("");

    //
    // Ouverture de la table des patients
    //
    NSPatient* pPatient = new NSPatient(pContexte) ;

	pPatient->lastError = pPatient->open();
	if (pPatient->lastError != DBIERR_NONE)
	{
        erreur("Erreur à l'ouverture du fichier des patients", 0, pPatient->lastError, GetHandle()) ;
        delete pPatient ;
        SetCursor(0, IDC_ARROW) ;
        return ;
	}

    pPatient->debut(dbiWRITELOCK) ;
    if (pPatient->lastError != DBIERR_NONE)
	{
        erreur("Erreur au positionnement sur la première fiche patient", 0, pPatient->lastError, GetHandle()) ;
        pPatient->close() ;
        delete pPatient ;
        SetCursor(0, IDC_ARROW) ;
        return ;
	}
    pPatient->lastError = pPatient->getRecord();
	if (pPatient->lastError != DBIERR_NONE)
	{
		erreur("Erreur à la lecture du premier patient.", 0, pPatient->lastError, GetHandle());
		pPatient->close() ;
        delete pPatient ;
        SetCursor(0, IDC_ARROW) ;
        return ;
	}

    //
    // Ouverture de la table des documents
    //
    NSDocument* pDocum = new NSDocument(pContexte);

	pDocum->lastError = pDocum->open();
	if (pDocum->lastError != DBIERR_NONE)
	{
        // on laisse les messages d'erreur de 1er niveau
        erreur("Erreur à l'ouverture du fichier des documents", 0, pDocum->lastError, GetHandle());
        delete pDocum;
        pPatient->close() ;
        delete pPatient;
        SetCursor(0, IDC_ARROW) ;
        return;
	}

    //
    // Ouverture de la table des noeuds
    //
    NSPatPaDat* pData = new NSPatPaDat(pContexte);

	pData->lastError = pData->open();
	if (pData->lastError != DBIERR_NONE)
	{
        // on laisse les messages d'erreur de 1er niveau
        erreur("Erreur à l'ouverture de la table nspatdat", 0, pData->lastError, GetHandle());
        delete pData;
        pDocum->close() ;
        delete pDocum;
        pPatient->close() ;
        delete pPatient;
        SetCursor(0, IDC_ARROW) ;
        return;
	}

    string sLang = "" ;
    if ((pContexte) && (pContexte->getUtilisateur()))
        sLang = pContexte->getUtilisateur()->donneLang() ;

	while (bTourner)
  	{
        pEditNSS->SetText(pPatient->pDonnees->nss);

        pEditNom->SetText(pPatient->pDonnees->nom);
        pEditPrenom->SetText(pPatient->pDonnees->prenom);

        string sNss = pPatient->pDonnees->nss ;

        //
        // On vérifie que tous les arbres ont un document
        //
        //
        // Recherche du premier noeud de ce patient
        //
        string cle = sNss ;

        pData->lastError = pData->chercheClef(&cle,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK);

        if (pData->lastError == DBIERR_NONE)
	    {
            pData->lastError = pData->getRecord() ;
            if (pData->lastError != DBIERR_NONE)
	        {
                erreur("Le fichier nspatdat est défectueux.", 0, pData->lastError) ;
                pData->close() ;
		        delete pData ;
                pDocum->close() ;
                delete pDocum ;
                pPatient->close() ;
                delete pPatient ;
                SetCursor(0, IDC_ARROW) ;
		        return ;
            }

            string sDocument = "" ;

            //
            // Tant que la fiche trouvée appartient bien au patient
            //
            while ((pData->lastError == DBIERR_NONE) &&
                    (string(pData->pDonnees->codePatient) == sNss))
            {
                // Nouveau document
                if (sDocument != pData->pDonnees->codeDocument)
                {
                    sDocument = pData->pDonnees->codeDocument ;
                    //
                    // On regarde si ce document existe
                    //
                    string sCleDoc = sNss + sDocument ;
                    pDocum->lastError = pDocum->chercheClef(&sCleDoc,
                                                            "",
                                                            0,
                                                            keySEARCHEQ,
                                                            dbiWRITELOCK) ;

                    if (pDocum->lastError != DBIERR_NONE)
	                {
                        // Création du document
                        strcpy(pDocum->pDonnees->codePatient,   sNss.c_str()) ;
                        strcpy(pDocum->pDonnees->codeDocument,  sDocument.c_str()) ;
                        strcpy(pDocum->pDonnees->nom,           "CR");
                        strcpy(pDocum->pDonnees->creation,      creation);
                        strcpy(pDocum->pDonnees->acces,         "");
                        strcpy(pDocum->pDonnees->createur,      "");
                        strcpy(pDocum->pDonnees->type,          "CN030");
                        strcpy(pDocum->pDonnees->localisation,  "");
                        strcpy(pDocum->pDonnees->fichier, 	    "");
                        strcpy(pDocum->pDonnees->entete,	    "");
                        strcpy(pDocum->pDonnees->emplacement,   "");
                        strcpy(pDocum->pDonnees->interet, 	    "5");
                        strcpy(pDocum->pDonnees->commentaire,   "");
                        strcpy(pDocum->pDonnees->visible, 	    "1");
                        strcpy(pDocum->pDonnees->tranNew,       (pContexte->getTransaction()).c_str());
                        strcpy(pDocum->pDonnees->tranDel, 	    "");
                        strcpy(pDocum->pDonnees->crc, 		    "");

                        // Type
                        string sTypeDoc = string(pData->pDonnees->lexique) ;

                        // Index de santé
                        if      (sTypeDoc == "ZPOMR1")
                        {
                            strcpy(pDocum->pDonnees->nom,   "Index de santé") ;
                            strcpy(pDocum->pDonnees->type,  "CS030") ;
                        }
                        // Synthèse
                        else if (sTypeDoc == "ZSYNT1")
                        {
                            strcpy(pDocum->pDonnees->nom,   "Synthèse") ;
                            strcpy(pDocum->pDonnees->type,  "CS030") ;
                        }
                        else
                        {
                            if (sTypeDoc == "GCONS1")
                                strcpy(pDocum->pDonnees->type, "CS030") ;

                            string  sTemplate, sEnTete ;
                            char    cType[3] = "";
                            strncpy(cType, pDocum->pDonnees->type, 2);
                            NSNoyauDocument* pNoyau = new NSNoyauDocument(0, pContexte) ;
                            pNoyau->TemplateCompo(cType, sTemplate, sEnTete) ;
                            strcpy(pDocum->pDonnees->fichier,   sTemplate.c_str()) ;
                            strcpy(pDocum->pDonnees->entete,    sEnTete.c_str()) ;
                            delete pNoyau ;

                            string sLibelleDoc = "?" ;
                            pContexte->getDico()->donneLibelle(sLang, &sTypeDoc, &sLibelleDoc) ;
                            strcpy(pDocum->pDonnees->nom, sLibelleDoc.c_str()) ;
                        }

                        pDocum->lastError = pDocum->appendRecord();

                        if (pDocum->lastError != DBIERR_NONE)				        {
					        erreur("Le fichier des documents ne permet pas l'ajout d'informations.", 0, pDocum->lastError);
      			            pData->close() ;
		                    delete pData ;
                            pDocum->close() ;
                            delete pDocum ;
                            pPatient->close() ;
                            delete pPatient ;
                            SetCursor(0, IDC_ARROW) ;
					        return ;
                        }
                        else
                        {
                            iNbRepaired ++ ;
                            char szNbRepaired[35] ;
                            itoa(iNbRepaired, szNbRepaired, 10) ;
                            pEditCode->SetText(szNbRepaired);
                        }
                    }
                }
                pData->lastError = pData->suivant(dbiWRITELOCK);
                if ((pData->lastError != DBIERR_NONE) && (pData->lastError != DBIERR_EOF))
		        {
                    erreur("Pb d'acces au noeud suivant.", 0, pData->lastError);
      	            pData->close();
			        delete pData;
                    pDocum->close() ;
                    delete pDocum ;
                    pPatient->close() ;
                    delete pPatient ;
                    SetCursor(0, IDC_ARROW) ;
			        return ;
		        }

                if (pData->lastError != DBIERR_EOF)
                {
   		            pData->lastError = pData->getRecord();
   		            if (pData->lastError != DBIERR_NONE)
			        {
                        erreur("Le fichier nspatdat est défectueux.", 0, pData->lastError);
      		            pData->close();
				        delete pData;
                        pDocum->close() ;
                        delete pDocum ;
                        pPatient->close() ;
                        delete pPatient ;
                        SetCursor(0, IDC_ARROW) ;
				        return ;
			        }
                }
            }
        } // Fin de l'énumération des noeuds

		pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages();

        // ----------------------------------------------------------------
        // Enumération des fichiers de traitement de texte
        // ----------------------------------------------------------------
        string sChemin = pContexte->PathName("NTTX");
	    char 		    *fichRecherche;
	    WIN32_FIND_DATA stRecherche;
   	    HANDLE			hFichierTrouve;
	    //
	    // Préparation d'une ammorce générique du type nss*.rtf
	    //

	    //	    // Préparation du nom de fichier complet
    	//
	    fichRecherche = new char[strlen(sChemin.c_str())+strlen(sNss.c_str())+6] ;
        strcpy(fichRecherche, sChemin.c_str()) ;
        strcat(fichRecherche, sNss.c_str()) ;
        strcat(fichRecherche, "*.rtf") ;
	    //
	    // Lancement de la recherche du premier fichier correspondant
	    //                  FindFirstFile
	    hFichierTrouve = FindFirstFile(fichRecherche, &stRecherche) ;
	    delete[] fichRecherche ;
	    //
	    // Si le fichier n'existe pas on renvoie le numéro de référence
	    //
	    if (hFichierTrouve != INVALID_HANDLE_VALUE)
	    {
	        //
	        // Boucle tant qu'on trouve des fichiers .rsp
	        //
            BOOL bTrouve = TRUE;
	        while (bTrouve)
	        {
                string sFichierRtf = string(stRecherche.cFileName);
                // size_t iAmmorceLen = strlen(sChemin.c_str()) ;
                // sFichierRtf = string(sFichierRtf, iAmmorceLen, strlen(sFichierRtf.c_str()) - iAmmorceLen) ;

                bool bTrouveDoc = false ;

                string sCleDoc = sNss ;
                pDocum->lastError = pDocum->chercheClef(&sCleDoc,
                                                            "",
                                                            0,
                                                            keySEARCHGEQ,
                                                            dbiWRITELOCK) ;

                if (pDocum->lastError == DBIERR_NONE)
	            {
                    pDocum->lastError = pDocum->getRecord() ;
                    if (pDocum->lastError != DBIERR_NONE)
	                {
                        erreur("Le fichier des documents est défectueux.", 0, pDocum->lastError) ;
                        pData->close() ;
		                delete pData ;
                        pDocum->close() ;
                        delete pDocum ;
                        pPatient->close() ;
                        delete pPatient ;
                        SetCursor(0, IDC_ARROW) ;
		                return ;
                    }

                    string sDocument = "" ;

                    //
                    // Tant que la fiche trouvée appartient bien au patient
                    //
                    while ((!bTrouveDoc) && (pDocum->lastError == DBIERR_NONE) &&
                        (string(pDocum->pDonnees->codePatient) == sNss))
                    {
                        if (string(pDocum->pDonnees->fichier) == sFichierRtf)
                            bTrouveDoc = true ;
                        else
                        {
                            pDocum->lastError = pDocum->suivant(dbiWRITELOCK);
                            if ((pDocum->lastError != DBIERR_NONE) && (pDocum->lastError != DBIERR_EOF))
		                    {
                                erreur("Pb d'acces au document suivant.", 0, pDocum->lastError);
      	                        pData->close();
			                    delete pData;
                                pDocum->close() ;
                                delete pDocum ;
                                pPatient->close() ;
                                delete pPatient ;
                                SetCursor(0, IDC_ARROW) ;
			                    return ;
		                    }

                            if (pDocum->lastError != DBIERR_EOF)
                            {
   		                        pDocum->lastError = pDocum->getRecord();
   		                        if (pDocum->lastError != DBIERR_NONE)
			                    {
                                    erreur("Le fichier des documents est défectueux.", 0, pDocum->lastError);
      		                        pData->close();
				                    delete pData;
                                    pDocum->close() ;
                                    delete pDocum ;
                                    pPatient->close() ;
                                    delete pPatient ;
                                    SetCursor(0, IDC_ARROW) ;
				                    return ;
			                    }
                            }
                        }
                    }
                }

                if (!bTrouveDoc)
                {
                    // Création du document
                    strcpy(pDocum->pDonnees->codePatient,   sNss.c_str()) ;
                    strcpy(pDocum->pDonnees->codeDocument,  "") ;
                    strcpy(pDocum->pDonnees->nom,           "Lettre");
                    strcpy(pDocum->pDonnees->creation,      creation);
                    strcpy(pDocum->pDonnees->acces,         "");
                    strcpy(pDocum->pDonnees->createur,      "");
                    strcpy(pDocum->pDonnees->type,          "TNRTF");
                    strcpy(pDocum->pDonnees->localisation,  "NTTX");
                    strcpy(pDocum->pDonnees->fichier, 	    sFichierRtf.c_str());
                    strcpy(pDocum->pDonnees->entete,	    "");
                    strcpy(pDocum->pDonnees->emplacement,   "");
                    strcpy(pDocum->pDonnees->interet, 	    "5");
                    strcpy(pDocum->pDonnees->commentaire,   "");
                    strcpy(pDocum->pDonnees->visible, 	    "1");
                    strcpy(pDocum->pDonnees->tranNew,       (pContexte->getTransaction()).c_str());
                    strcpy(pDocum->pDonnees->tranDel, 	    "");
                    strcpy(pDocum->pDonnees->crc, 		    "");

                    if (!pDocum->DonneNouveauCode(pDocum->pDonnees->codeDocument, sNss))
                    {
                        erreur("Le fichier des documents ne permet pas le calcul d'un nouveau code.", 0, pDocum->lastError);
                        pData->close() ;
                        delete pData ;
                        pDocum->close() ;
                        delete pDocum ;
                        pPatient->close() ;
                        delete pPatient ;
                        SetCursor(0, IDC_ARROW) ;
                        return ;
                    }

                    pDocum->lastError = pDocum->appendRecord();

                    if (pDocum->lastError != DBIERR_NONE)                    {
                        erreur("Le fichier des documents ne permet pas l'ajout d'informations.", 0, pDocum->lastError);
                        pData->close() ;
                        delete pData ;
                        pDocum->close() ;
                        delete pDocum ;
                        pPatient->close() ;
                        delete pPatient ;
                        SetCursor(0, IDC_ARROW) ;
                        return ;
                    }
                    else
                    {
                        iNbRepaired ++ ;
                        char szNbRepaired[35] ;
                        itoa(iNbRepaired, szNbRepaired, 10) ;
                        pEditCode->SetText(szNbRepaired);
                    }
                }

		        bTrouve = FindNextFile(hFichierTrouve, &stRecherche);
	        }
	        //
	        // On renvoie le numéro de référence
	        //
            FindClose(hFichierTrouve);
        }

        // Patient suivant
        pPatient->lastError = pPatient->suivant(dbiWRITELOCK);
        if ((pPatient->lastError != DBIERR_NONE) && (pPatient->lastError != DBIERR_EOF))
        {
            erreur("Pb d'acces au patient suivant.", 0, pPatient->lastError);
            bTourner = false ;
        }
        if (pPatient->lastError == DBIERR_EOF)
            bTourner = false ;
        if (bTourner)
        {
            pPatient->lastError = pPatient->getRecord();
            if (pPatient->lastError != DBIERR_NONE)
            {
                erreur("Le fichier des patients est défectueux.", 0, pPatient->lastError);
      		    bTourner = false ;
            }
        }
    }

    pData->close();
    delete pData;
    pDocum->close() ;
    delete pDocum ;
    pPatient->close() ;
    delete pPatient ;

    SetCursor(0, IDC_ARROW) ;
}

//---------------------------------------------------------------------------//  Arrêt de la conversion en boucle
//---------------------------------------------------------------------------
void
NsRepairDocumDlg::repairStop()
{
    bTourner = false;
}

#endif