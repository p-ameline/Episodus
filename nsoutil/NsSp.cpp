//    NSSP.CPP
//    KADDACHI Hafedh août 97
//  Implémentation des objets de TLIBRE.DB
//---------------------------------------------------------------------------
#include <cstring.h>
#include <windows.h>
#include <mem.h>
#include <bde.hpp>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsoutil\nssp.h"
#include "nssavoir\nsfilgd.h"

//***************************************************************************
// 				  Implémentation des mèthodes NSSPData
//***************************************************************************

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::NSPatientData(NSPatientData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSSPData::NSSPData(NSSPData& rv)
{
    strcpy(code, 	  	       rv.code);
	strcpy(codeInterne, 	   rv.codeInterne);
    strcpy(codeGenerique, 	   rv.codeGenerique);
    strcpy(codeForme, 	  	   rv.codeForme);

    strcpy(codeStatutLabo, 	   rv.codeStatutLabo);
    strcpy(listeSpecialite,    rv.listeSpecialite);

    strcpy(codeATC, 	       rv.codeATC);
    strcpy(codeEPHMRA, 	  	   rv.codeEPHMRA);

    strcpy(codeGestion, 	   rv.codeGestion);
    strcpy(codeInterneProduit, rv.codeInterneProduit);
    strcpy(nomSpecialite, 	   rv.nomSpecialite);
    strcpy(complementNom, 	   rv.complementNom);

    strcpy(PhSpecialite, 	   rv.PhSpecialite);
    strcpy(solvantSpecialite,  rv.solvantSpecialite);

    strcpy(codeCIP_UCD, 	   rv.codeCIP_UCD);
    strcpy(dateCreation, 	   rv.dateCreation);
    strcpy(dateModif, 	  	   rv.dateModif);

    strcpy(numDossier, 	  	   rv.numDossier);
    strcpy(codeAPHP,           rv.codeAPHP);
    strcpy(codeInterneRef,     rv.codeInterneRef);
    strcpy(dateJOGenerique,    rv.dateJOGenerique);
    strcpy(origineGenerique,   rv.origineGenerique);
}

NSSPData::~NSSPData()
{}

//---------------------------------------------------------------------------
//  Fonction :		NSSPData::operator=(NSSPData src)
//  Description :	Opérateur =
//  Retour :		Référence sur l'objet cible
//---------------------------------------------------------------------------
NSSPData&
NSSPData::operator=(NSSPData rv)
{
    strcpy(code, 	  	       rv.code);
	strcpy(codeInterne, 	   rv.codeInterne);
    strcpy(codeGenerique, 	   rv.codeGenerique);
    strcpy(codeForme, 	  	   rv.codeForme);

    strcpy(codeStatutLabo, 	   rv.codeStatutLabo);
    strcpy(listeSpecialite,    rv.listeSpecialite);

    strcpy(codeATC, 	       rv.codeATC);
    strcpy(codeEPHMRA, 	  	   rv.codeEPHMRA);

    strcpy(codeGestion, 	   rv.codeGestion);
    strcpy(codeInterneProduit, rv.codeInterneProduit);
    strcpy(nomSpecialite, 	   rv.nomSpecialite);

    strcpy(complementNom, 	   rv.complementNom);
    strcpy(PhSpecialite, 	   rv.PhSpecialite);
    strcpy(solvantSpecialite,  rv.solvantSpecialite);

    strcpy(codeCIP_UCD, 	   rv.codeCIP_UCD);
    strcpy(dateCreation, 	   rv.dateCreation);
    strcpy(dateModif, 	  	   rv.dateModif);

    strcpy(numDossier, 	  	   rv.numDossier);
    strcpy(codeAPHP,           rv.codeAPHP);
    strcpy(codeInterneRef,     rv.codeInterneRef);
    strcpy(dateJOGenerique,    rv.dateJOGenerique);
    strcpy(origineGenerique,   rv.origineGenerique);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction :		NSSPData::operator==(NSSPData& o)
//  Description :	Opérateur de comparaison
//  Retour :		0 ou 1
//---------------------------------------------------------------------------
int
NSSPData::operator == ( const NSSPData& rv )
{
    if ((strcmp(code, 	  	 rv.code)        == 0) &&
   		(strcmp(codeInterne, rv.codeInterne) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Function:    NSSPData::metAZero()
//
//  Description: Met à 0 les variables.
//---------------------------------------------------------------------------
void
NSSPData::metAZero()
{
    //
	// Met les champs de données à zéro
	//
    memset(code,               0, SP_CODE_LEN + 1);
	memset(codeInterne,        0, SP_CODE_INTERNE_LEN + 1);
    memset(codeGenerique,      0, SP_CODE_GENERIQUE_LEN + 1);
    memset(codeForme,          0, SP_CODE_FORME_PHRMA_LEN + 1);
    memset(codeStatutLabo,     0, SP_CODE_STATUT_LABO_LEN + 1);
    memset(listeSpecialite,    0, SP_LISTE_SPECIALITE_LEN + 1);
    memset(codeATC,            0, SP_CODE_CLASSE_ATC_LEN + 1);
    memset(codeEPHMRA,         0, SP_CODE_CLASSE_EPHMRA_LEN + 1);
    memset(codeGestion,        0, SP_CODE_CLASSE_GESTION_LEN + 1);
    memset(codeInterneProduit, 0, SP_CODE_INTER_PROD_LEN + 1);
    memset(nomSpecialite,      0, SP_NOM_SPECIALITE_LEN + 1);
    memset(complementNom,      0, SP_COMPLEMENT_NOM_LEN + 1);
    memset(PhSpecialite,       0, SP_PH_SPECIALITE_LEN + 1);
    memset(solvantSpecialite,  0, SP_SOLVANT_SPECIALITE_LEN + 1);
    memset(codeCIP_UCD,        0, SP_CODE_CIP_UCD_LEN + 1);
    memset(dateCreation,       0, SP_DATE_CREATION_LEN + 1);
    memset(dateModif,          0, SP_DATE_MODIFICATION_LEN + 1);
    memset(numDossier,         0, SP_NUMERO_DOSSIER_LEN + 1);
    memset(codeAPHP,           0, SP_CODE_APHP_LEN + 1);
    memset(codeInterneRef,     0, SP_INTERNE_REF_LEN + 1);
    memset(dateJOGenerique,    0, SP_DATE_JO_GENERIQUE_LEN + 1);
    memset(origineGenerique,   0, SP_ORIGINE_GENERIQUE_LEN + 1);
}

//---------------------------------------------------------------------------
//  Function:    NSSPData::metABlanc()
//
//  Description: Met à blanc les variables.
//---------------------------------------------------------------------------
void
NSSPData::metABlanc()
{
	//
	// Met les champs de données à blanc
	//
    memset(code,               ' ', SP_CODE_LEN);
    memset(codeInterne,        ' ', SP_CODE_INTERNE_LEN);
    memset(codeGenerique,      ' ', SP_CODE_GENERIQUE_LEN);
    memset(codeForme,          ' ', SP_CODE_FORME_PHRMA_LEN);
    memset(codeStatutLabo,     ' ', SP_CODE_STATUT_LABO_LEN);
    memset(listeSpecialite,    ' ', SP_LISTE_SPECIALITE_LEN);
    memset(codeATC,            ' ', SP_CODE_CLASSE_ATC_LEN);
    memset(codeEPHMRA,         ' ', SP_CODE_CLASSE_EPHMRA_LEN);
    memset(codeGestion,        ' ', SP_CODE_CLASSE_GESTION_LEN);
    memset(codeInterneProduit, ' ', SP_CODE_INTER_PROD_LEN);
    memset(nomSpecialite,      ' ', SP_NOM_SPECIALITE_LEN);
    memset(complementNom,      ' ', SP_COMPLEMENT_NOM_LEN);
    memset(PhSpecialite,       ' ', SP_PH_SPECIALITE_LEN);
    memset(solvantSpecialite,  ' ', SP_SOLVANT_SPECIALITE_LEN);
    memset(codeCIP_UCD,        ' ', SP_CODE_CIP_UCD_LEN);
    memset(dateCreation,       ' ', SP_DATE_CREATION_LEN);
    memset(dateModif,          ' ', SP_DATE_MODIFICATION_LEN);
    memset(numDossier,         ' ', SP_NUMERO_DOSSIER_LEN);
    memset(codeAPHP,           ' ', SP_CODE_APHP_LEN);
    memset(codeInterneRef,     ' ', SP_INTERNE_REF_LEN);
    memset(dateJOGenerique,    ' ', SP_DATE_JO_GENERIQUE_LEN);
    memset(origineGenerique,   ' ', SP_ORIGINE_GENERIQUE_LEN);
}

//***************************************************************************
//							 Implémentation des mèthodes NSSP
//***************************************************************************
//---------------------------------------------------------------------------
//  Function:    NSSP::NSSP(NSSuper* pSuper)
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSSP::NSSP(NSContexte* pCtx) : NSFiche(pCtx)
{
	pDonnees = new NSSPData();
}

//---------------------------------------------------------------------------
//  Function:    NSSP::~NSSP()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSSP::~NSSP()
{
    if (pDonnees)
    {
   	    delete pDonnees;
        pDonnees = 0   ;
    }
}

//---------------------------------------------------------------------------
//  Function :  	NSSP::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSSP::alimenteFiche()
{
    alimenteChamp(pDonnees->code,               SP_CODE_FIELD,                SP_CODE_LEN);
    alimenteChamp(pDonnees->codeInterne,        SP_CODE_INTERNE_FIELD,        SP_CODE_INTERNE_LEN);
    alimenteChamp(pDonnees->codeGenerique,      SP_CODE_GENERIQUE_FIELD,      SP_CODE_GENERIQUE_LEN);
    alimenteChamp(pDonnees->codeForme,          SP_CODE_FORME_PHRMA_FIELD,    SP_CODE_FORME_PHRMA_LEN);
    alimenteChamp(pDonnees->codeStatutLabo,     SP_CODE_STATUT_LABO_FIELD,    SP_CODE_STATUT_LABO_LEN );
    alimenteChamp(pDonnees->listeSpecialite,    SP_LISTE_SPECIALITE_FIELD,    SP_LISTE_SPECIALITE_LEN );
    alimenteChamp(pDonnees->codeATC,            SP_CODE_CLASSE_ATC_FIELD,     SP_CODE_CLASSE_ATC_LEN );
    alimenteChamp(pDonnees->codeEPHMRA,         SP_CODE_CLASSE_EPHMRA_FIELD,  SP_CODE_CLASSE_EPHMRA_LEN);
    alimenteChamp(pDonnees->codeGestion,        SP_CODE_CLASSE_GESTION_FIELD, SP_CODE_CLASSE_GESTION_LEN);
    alimenteChamp(pDonnees->codeInterneProduit, SP_CODE_INTER_PROD_FIELD,     SP_CODE_INTER_PROD_LEN);
    alimenteChamp(pDonnees->nomSpecialite,      SP_NOM_SPECIALITE_FIELD,      SP_NOM_SPECIALITE_LEN);
    alimenteChamp(pDonnees->complementNom,      SP_COMPLEMENT_NOM_FIELD,      SP_COMPLEMENT_NOM_LEN);
    alimenteChamp(pDonnees->PhSpecialite,       SP_PH_SPECIALITE_FIELD,       SP_PH_SPECIALITE_LEN);
    alimenteChamp(pDonnees->solvantSpecialite,  SP_SOLVANT_SPECIALITE_FIELD,  SP_SOLVANT_SPECIALITE_LEN );
    alimenteChamp(pDonnees->codeCIP_UCD,        SP_CODE_CIP_UCD_FIELD,        SP_CODE_CIP_UCD_LEN);
    alimenteChamp(pDonnees->dateCreation,       SP_DATE_CREATION_FIELD,       SP_DATE_CREATION_LEN);
    alimenteChamp(pDonnees->dateModif,          SP_DATE_MODIFICATION_FIELD,   SP_DATE_MODIFICATION_LEN);
    alimenteChamp(pDonnees->numDossier,         SP_NUMERO_DOSSIER_FIELD,      SP_NUMERO_DOSSIER_LEN );
    alimenteChamp(pDonnees->codeAPHP,           SP_CODE_APHP_FIELD,           SP_CODE_APHP_LEN);
    alimenteChamp(pDonnees->codeInterneRef,     SP_INTERNE_REF_FIELD,         SP_INTERNE_REF_LEN);
    alimenteChamp(pDonnees->dateJOGenerique,    SP_DATE_JO_GENERIQUE_FIELD,   SP_DATE_JO_GENERIQUE_LEN);
    alimenteChamp(pDonnees->origineGenerique,   SP_ORIGINE_GENERIQUE_FIELD,   SP_ORIGINE_GENERIQUE_LEN);
}

//---------------------------------------------------------------------------
//  Fonction :  	NSSP::videFiche()
//
//  Description :	Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSSP::videFiche()
{
    videChamp(pDonnees->code,               SP_CODE_FIELD,                SP_CODE_LEN);
    videChamp(pDonnees->codeInterne,        SP_CODE_INTERNE_FIELD,        SP_CODE_INTERNE_LEN);
    videChamp(pDonnees->codeGenerique,      SP_CODE_GENERIQUE_FIELD,      SP_CODE_GENERIQUE_LEN);
    videChamp(pDonnees->codeForme,          SP_CODE_FORME_PHRMA_FIELD,    SP_CODE_FORME_PHRMA_LEN);
    videChamp(pDonnees->codeStatutLabo,     SP_CODE_STATUT_LABO_FIELD,    SP_CODE_STATUT_LABO_LEN );
    videChamp(pDonnees->listeSpecialite,    SP_LISTE_SPECIALITE_FIELD,    SP_LISTE_SPECIALITE_LEN );
    videChamp(pDonnees->codeATC,            SP_CODE_CLASSE_ATC_FIELD,     SP_CODE_CLASSE_ATC_LEN );
    videChamp(pDonnees->codeEPHMRA,         SP_CODE_CLASSE_EPHMRA_FIELD,  SP_CODE_CLASSE_EPHMRA_LEN);
    videChamp(pDonnees->codeGestion,        SP_CODE_CLASSE_GESTION_FIELD, SP_CODE_CLASSE_GESTION_LEN);
    videChamp(pDonnees->codeInterneProduit, SP_CODE_INTER_PROD_FIELD,     SP_CODE_INTER_PROD_LEN);
    videChamp(pDonnees->nomSpecialite,      SP_NOM_SPECIALITE_FIELD,      SP_NOM_SPECIALITE_LEN);
    videChamp(pDonnees->complementNom,      SP_COMPLEMENT_NOM_FIELD,      SP_COMPLEMENT_NOM_LEN);
    videChamp(pDonnees->PhSpecialite,       SP_PH_SPECIALITE_FIELD,       SP_PH_SPECIALITE_LEN);
    videChamp(pDonnees->solvantSpecialite,  SP_SOLVANT_SPECIALITE_FIELD,  SP_SOLVANT_SPECIALITE_LEN );
    videChamp(pDonnees->codeCIP_UCD,        SP_CODE_CIP_UCD_FIELD,        SP_CODE_CIP_UCD_LEN);
    videChamp(pDonnees->dateCreation,       SP_DATE_CREATION_FIELD,       SP_DATE_CREATION_LEN);
    videChamp(pDonnees->dateModif,          SP_DATE_MODIFICATION_FIELD,   SP_DATE_MODIFICATION_LEN);
    videChamp(pDonnees->numDossier,         SP_NUMERO_DOSSIER_FIELD,      SP_NUMERO_DOSSIER_LEN );
    videChamp(pDonnees->codeAPHP,           SP_CODE_APHP_FIELD,           SP_CODE_APHP_LEN);
    videChamp(pDonnees->codeInterneRef,     SP_INTERNE_REF_FIELD,         SP_INTERNE_REF_LEN);
    videChamp(pDonnees->dateJOGenerique,    SP_DATE_JO_GENERIQUE_FIELD,   SP_DATE_JO_GENERIQUE_LEN);
    videChamp(pDonnees->origineGenerique,   SP_ORIGINE_GENERIQUE_FIELD,   SP_ORIGINE_GENERIQUE_LEN);
}

//---------------------------------------------------------------------------
//  Function :  	NSSP::open()
//
//  Description : Ouvre la table primaire
//
//  Retour :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSSP::open()
{
    char tableName[] = "SP.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}


//***************************************************************************
// 				Implémentation des méthodes NSSPInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSSPInfo::NSSPInfo()
{
    // Crée l'objet de données
	pDonnees = new NSSPData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSPatPatho
//---------------------------------------------------------------------------
NSSPInfo::NSSPInfo(NSSP* pNSTextelibre)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSSPData();
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pNSTextelibre->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSSPInfo::NSSPInfo(NSSPInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSSPData();
	//
	// Copie les valeurs du NSSPInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}


//destructeur
NSSPInfo::~NSSPInfo()
{
	if(pDonnees)
    {
        delete pDonnees;
        pDonnees = 0;
    }
}
//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSSPInfo&
NSSPInfo::operator=(NSSPInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int NSSPInfo::operator == ( const NSSPInfo& o )
{
	return (*pDonnees == *(o.pDonnees));
}



//***************************************************************************
// 				  Implémentation des mèthodes NSSpecialite
//***************************************************************************
NSSpecialite::NSSpecialite(NSContexte* pCtx)
			:NSRoot(pCtx)
{
	pNSSP = new NSSP(pCtx);
}
//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::NSPatientData(NSPatientData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSSpecialite::NSSpecialite(NSSpecialite& rv)
			:NSRoot(rv.pContexte)
{}


NSSpecialite::~NSSpecialite()
{
	delete pNSSP;
}
//---------------------------------------------------------------------------
//  Fonction :		NSSpecialite::operator=(NSSpecialite src)
//  Description :	Opérateur =
//  Retour :		Référence sur l'objet cible
//---------------------------------------------------------------------------
NSSpecialite&
NSSpecialite::operator=(NSSpecialite rv)
{
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction :		NSSpecialite::operator==(NSSpecialite& o)
//  Description :	Opérateur de comparaison
//  Retour :		0 ou 1
//---------------------------------------------------------------------------
int
NSSpecialite::operator == ( const NSSpecialite& o )
{
	return 1;
}
//---------------------------------------------------------------------------
//calcul le code
//---------------------------------------------------------------------------
bool
NSSpecialite::IncrementeCode(string* pTexte)
{
    int i = strlen(pTexte->c_str()) ;
    if (i == 0)
   	    return false;
    bool tourner = true;

    while((tourner) && (i > 0) )
    {
        i--;
   	    if ( (((*pTexte)[i] >= '0') && ((*pTexte)[i] < '9')) ||
      	     (((*pTexte)[i] >= 'A') && ((*pTexte)[i] < 'Z')) )
        {
            (*pTexte)[i] = (*pTexte)[i] + 1;
            tourner = false;
        }
        else if ((*pTexte)[i] == '9')
        {
            (*pTexte)[i] = 'A';
            tourner = false;
        }
        else
            (*pTexte)[i] = '0';
    }
    return(!tourner);
}


void
NSSpecialite::Import()
{
    short   	 i, j, champ, taille_champ;
	int     	 entree;
	bool    	 tourner = true;
	char 		 buffer[100];
	DBIResult lastError;
    FILE* infile;

    string	 sCode;

  	//
  	// Ouverture du fichier d'importation
  	//
  	infile = fopen("\\NAUTILUS\\SP.TXT", "r");
  	if (infile == NULL)
    {
   	    erreur("Fichier SP.TXT non trouvé", 0, 0);
	  	return;
    }
  	setmode(fileno(infile), O_BINARY);
    //
  	// Ouverture de la table
  	//
  	//PV2_3Dialog(pNSSup->PrendWndCourant())->PeindreEdit(PAT_OUV, "Non");
  	lastError = pNSSP->open();
  	if (lastError != DBIERR_NONE)
    {
   	    lastError = pNSSP->close();
   	    return;
    }

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
        pNSSP->pDonnees->metAZero();

        /*
        string sCode;

        pNSSP->lastError = pNSSP->fin(dbiWRITELOCK);
   	    //
		// Si la table est vide
		//
		if ((pNSSP->lastError == DBIERR_BOF) ||
		 	(pNSSP->lastError == DBIERR_EOF) ||
		 	(pNSSP->lastError == DBIERR_NOCURRREC))
            sCode = string(SP_CODE_LEN - 1, '0');
   	    else
   	    {
      	    if (pNSSP->lastError != DBIERR_NONE)
			{
				erreur("3.Erreur d'accés à la fin de la base SP.", 0, pNSSP->lastError);
      		    pNSSP->close();
				return ;
			}
	   	    pNSSP->lastError = pNSSP->getRecord();
   		    if (pNSSP->lastError != DBIERR_NONE)
   		    {
   			    erreur("4.Le fichier de la base sp est inutilisable.", 0, pNSSP->lastError);
      		    pNSSP->close();
      		    return ;
   		    }
      	    sCode = string(pNSSP->pDonnees->code, 1, SP_CODE_LEN - 1);
      	    IncrementeCode(&sCode);
   	    }

        sCode = "_" + sCode;
        strcpy(pNSSP->pDonnees->code, sCode.c_str()); */

        for (champ = 0; (champ < 21) && (tourner); champ++)
        {
      	    switch (champ)
            {
         	    case  0 : taille_champ =  6; break;
                case  1 : taille_champ =  6; break;
                case  2 : taille_champ = 10; break;
                case  3 : taille_champ = 10; break;
                case  4 : taille_champ = 10; break;
                case  5 : taille_champ = 10; break;
                case  6 : taille_champ = 10; break;
                case  7 : taille_champ =  6; break;
                case  8 : taille_champ =  6; break;
                case  9 : taille_champ = 36; break;
                case 10 : taille_champ = 40; break;
                case 11 : taille_champ = 10; break;
                case 12 : taille_champ =  1; break;
                case 13 : taille_champ =  7; break;
                case 14 : taille_champ = 10; break;
                case 15 : taille_champ = 10; break;
                case 16 : taille_champ = 10; break;
                case 17 : taille_champ =  8; break;
                case 18 : taille_champ =  6; break;
                case 19 : taille_champ = 10; break;
                case 20 : taille_champ =  1; break;
            }

            for (j = 0; (j < taille_champ) && (entree != EOF) && (entree != '|'); j++)
            {
         	    buffer[j] = entree;
                entree = getc(infile);
            }
            buffer[j] = '\0';
            if (entree == EOF)
         	    tourner = false;
            else if (entree == '|')
         	    entree = getc(infile);
            OemToAnsi(buffer, buffer);
            string sDate = "", sDateSP = "";

            switch (champ)
            {
         	    case  0 : strcpy(pNSSP->pDonnees->codeInterne,        buffer); break;
                case  1 : strcpy(pNSSP->pDonnees->codeGenerique,      buffer); break;
                case  2 : strcpy(pNSSP->pDonnees->codeForme, 	      buffer); break;
                case  3 : strcpy(pNSSP->pDonnees->codeStatutLabo,     buffer); break;
                case  4 : strcpy(pNSSP->pDonnees->listeSpecialite,    buffer); break;
                case  5 : strcpy(pNSSP->pDonnees->codeATC,            buffer); break;
                case  6 : strcpy(pNSSP->pDonnees->codeEPHMRA,         buffer); break;
                case  7 : strcpy(pNSSP->pDonnees->codeGestion,        buffer); break;
                case  8 : strcpy(pNSSP->pDonnees->codeInterneProduit, buffer); break;
                case  9 : strcpy(pNSSP->pDonnees->nomSpecialite,      buffer); break;
                case 10 : strcpy(pNSSP->pDonnees->complementNom,      buffer); break;
                case 11 : strcpy(pNSSP->pDonnees->PhSpecialite,       buffer); break;
                case 12 : strcpy(pNSSP->pDonnees->solvantSpecialite,  buffer); break;
                case 13 : strcpy(pNSSP->pDonnees->codeCIP_UCD,        buffer); break;
                case 14 :
                    sDate = buffer;
                    if (sDate != "")
                    {
                        sDateSP = string(sDate, 6, 4) +
                                  string(sDate, 3, 2) +
                                  string(sDate, 0, 2);
                      	strcpy(pNSSP->pDonnees->dateCreation, sDateSP.c_str());
                    }
                    else
                        strcpy(pNSSP->pDonnees->dateCreation, "");
                    break;
                case 15 :
                    sDate = buffer;
                    if (sDate != "")
                    {
                        sDateSP = string(sDate, 6, 4) +
                                  string(sDate, 3, 2) +
                                  string(sDate, 0, 2);
                      	strcpy(pNSSP->pDonnees->dateModif, sDateSP.c_str());
                    }
                    else
                        strcpy(pNSSP->pDonnees->dateModif, "");
                    break;
                case 16 : strcpy(pNSSP->pDonnees->numDossier,       buffer); break;
                case 17 : strcpy(pNSSP->pDonnees->codeAPHP,         buffer); break;
                case 18 : strcpy(pNSSP->pDonnees->codeInterneRef,   buffer); break;
                case 19 :
                    sDate = buffer;
                    if (sDate != "")
                    {
                        sDateSP = string(sDate, 6, 4) +
                                  string(sDate, 3, 2) +
                                  string(sDate, 0, 2);
                      	strcpy(pNSSP->pDonnees->dateJOGenerique, sDateSP.c_str());
                    }
                    else
                        strcpy(pNSSP->pDonnees->dateJOGenerique, "");
                    break;
                case 20 : strcpy(pNSSP->pDonnees->origineGenerique, buffer); break;
            }
        }

        //
        // Elaboration du code
        //
        string sCode = pNSSP->pDonnees->codeInterne;
        while (strlen(sCode.c_str()) < 5)
            sCode = '0' + sCode;
        sCode = '_' + sCode;

        strcpy(pNSSP->pDonnees->code, sCode.c_str());

        pNSSP->lastError = pNSSP->appendRecord();
        if (pNSSP->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à l'ajout de la fiche SP", 0, pNSSP->lastError);
            fclose(infile);
            pNSSP->lastError = pNSSP->close();
            return;
        }
        //***************************************
  		//mettre à jour la base leximed.db
  		//***************************************
        string sLibelle = string(pNSSP->pDonnees->nomSpecialite) + " " +
								      	string(pNSSP->pDonnees->complementNom);
        NSSuper* pSuper = pContexte->getSuperviseur();
        
        if(strlen(sLibelle.c_str()) > PATHO_LIBELLE_LEN)
      	    sLibelle = string(sLibelle, 0, PATHO_LIBELLE_LEN);
        strcpy(pSuper->getDico()->pLexiMed->pDonnees->libelle, sLibelle.c_str());
        strcpy(pSuper->getDico()->pLexiMed->pDonnees->grammaire, "INV");
        strcpy(pSuper->getDico()->pLexiMed->pDonnees->code, pNSSP->pDonnees->code);

        pSuper->getDico()->pLexiMed->lastError = pSuper->getDico()->pLexiMed->appendRecord();
        if (pSuper->getDico()->pLexiMed->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à l'ajout de leximed", 0, pSuper->getDico()->pLexiMed->lastError);
            pSuper->getDico()->pLexiMed->lastError = pSuper->getDico()->pLexiMed->close();
            return;
        }
    }
  	fclose(infile);
  	lastError = pNSSP->close();
}

//***************************************************************************
// 				  Implémentation des mèthodes NSCPHData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCPHData::NSCPHData(NSCPHData& rv)
{
    strcpy(code, 	  	 rv.code);
	strcpy(codeInterne,  rv.codeInterne);
    strcpy(codePere, 	 rv.codePere);
    strcpy(nom, 	     rv.nom);
    strcpy(dateCreation, rv.dateCreation);
    strcpy(dateModif, 	 rv.dateModif);
    strcpy(modifie, 	 rv.modifie);
}

NSCPHData::~NSCPHData()
{}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCPHData&
NSCPHData::operator=(NSCPHData rv)
{
    strcpy(code, 	  	 rv.code);
	strcpy(codeInterne,  rv.codeInterne);
    strcpy(codePere, 	 rv.codePere);
    strcpy(nom, 	     rv.nom);
    strcpy(dateCreation, rv.dateCreation);
    strcpy(dateModif, 	 rv.dateModif);
    strcpy(modifie, 	 rv.modifie);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction :		NSSPData::operator==(NSSPData& o)
//  Description :	Opérateur de comparaison
//  Retour :		0 ou 1
//---------------------------------------------------------------------------
int
NSCPHData::operator == ( const NSCPHData& rv )
{
    if ((strcmp(code, 	  	 rv.code)        == 0) &&
   		(strcmp(codeInterne, rv.codeInterne) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Function:    NSSPData::metAZero()
//
//  Description: Met à 0 les variables.
//---------------------------------------------------------------------------
void
NSCPHData::metAZero()
{
    //
	// Met les champs de données à zéro
	//
    memset(code,         0, CPH_CODE_LEN + 1);
	memset(codeInterne,  0, CPH_CODE_INTERNE_LEN + 1);
    memset(codePere,     0, CPH_CODE_PERE_LEN + 1);
    memset(nom,          0, CPH_NOM_LEN + 1);
    memset(dateCreation, 0, CPH_DATE_CREATION_LEN + 1);
    memset(dateModif,    0, CPH_DATE_MODIFICATION_LEN + 1);
    memset(modifie,      0, CPH_MODIFIE_LEN + 1);
}

//***************************************************************************
//							 Implémentation des mèthodes NSCPH
//***************************************************************************
//---------------------------------------------------------------------------
//  Constructeur.
//---------------------------------------------------------------------------
NSCPH::NSCPH(NSContexte* pCtx) : NSFiche(pCtx)
{
	pDonnees = new NSCPHData();
}

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSCPH::~NSCPH()
{
    if (pDonnees)
    {
   	    delete pDonnees;
        pDonnees = 0   ;
    }
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSCPH::alimenteFiche()
{
    alimenteChamp(pDonnees->code,         CPH_CODE_FIELD,              CPH_CODE_LEN);
    alimenteChamp(pDonnees->codeInterne,  CPH_CODE_INTERNE_FIELD,      CPH_CODE_INTERNE_LEN);
    alimenteChamp(pDonnees->codePere,     CPH_CODE_PERE_FIELD,         CPH_CODE_PERE_LEN);
    alimenteChamp(pDonnees->nom,          CPH_NOM_FIELD,               CPH_NOM_LEN);
    alimenteChamp(pDonnees->dateCreation, CPH_DATE_CREATION_FIELD,     CPH_DATE_CREATION_LEN);
    alimenteChamp(pDonnees->dateModif,    CPH_DATE_MODIFICATION_FIELD, CPH_DATE_MODIFICATION_LEN);
    alimenteChamp(pDonnees->modifie,      CPH_MODIFIE_FIELD,           CPH_MODIFIE_LEN);
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCPH::videFiche()
{
    videChamp(pDonnees->code,         CPH_CODE_FIELD,              CPH_CODE_LEN);
    videChamp(pDonnees->codeInterne,  CPH_CODE_INTERNE_FIELD,      CPH_CODE_INTERNE_LEN);
    videChamp(pDonnees->codePere,     CPH_CODE_PERE_FIELD,         CPH_CODE_PERE_LEN);
    videChamp(pDonnees->nom,          CPH_NOM_FIELD,               CPH_NOM_LEN);
    videChamp(pDonnees->dateCreation, CPH_DATE_CREATION_FIELD,     CPH_DATE_CREATION_LEN);
    videChamp(pDonnees->dateModif,    CPH_DATE_MODIFICATION_FIELD, CPH_DATE_MODIFICATION_LEN);
    videChamp(pDonnees->modifie,      CPH_MODIFIE_FIELD,           CPH_MODIFIE_LEN);
}

//---------------------------------------------------------------------------
//  Ouvre la table primaire
//---------------------------------------------------------------------------
DBIResult
NSCPH::open()
{
    char tableName[] = "T_CPH.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//***************************************************************************
// 				Implémentation des méthodes NSSPInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSCPHInfo::NSCPHInfo()
{
    // Crée l'objet de données
	pDonnees = new NSCPHData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSPatPatho
//---------------------------------------------------------------------------
NSCPHInfo::NSCPHInfo(NSCPH* pCPH)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCPHData();
	//
	// Copie les valeurs de l'objet SGBD
	//
	*pDonnees = *(pCPH->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCPHInfo::NSCPHInfo(NSCPHInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCPHData();
	//
	// Copie les valeurs du NSSPInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}


//destructeur
NSCPHInfo::~NSCPHInfo()
{
	if(pDonnees)
    {
        delete pDonnees;
        pDonnees = 0;
    }
}
//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCPHInfo&
NSCPHInfo::operator=(NSCPHInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int NSCPHInfo::operator == ( const NSCPHInfo& o )
{
	return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
// 				  Implémentation des mèthodes NSClassePharma
//***************************************************************************
NSClassePharma::NSClassePharma(NSContexte* pCtx)
               :NSRoot(pCtx)
{
	pConvert = new NSCPH(pCtx);
}

NSClassePharma::~NSClassePharma()
{
	delete pConvert;
}

//---------------------------------------------------------------------------
// Met à jour la table DB à partir du fichier TXT
//---------------------------------------------------------------------------
void
NSClassePharma::Import()
{
    short   	 i, j, champ, taille_champ;
	int     	 entree;
	bool    	 tourner = true;
	char 		 buffer[100];
	DBIResult lastError;
    FILE* infile;

    string	 sCode;

  	//
  	// Ouverture du fichier d'importation
  	//
  	infile = fopen("\\NAUTILUS\\CPH.TXT", "r");
  	if (infile == NULL)
    {
   	    erreur("Fichier CPH.TXT non trouvé", 0, 0);
	  	return;
    }
  	setmode(fileno(infile), O_BINARY);
    //
  	// Ouverture de la table
  	//
  	//PV2_3Dialog(pNSSup->PrendWndCourant())->PeindreEdit(PAT_OUV, "Non");
  	lastError = pConvert->open();
  	if (lastError != DBIERR_NONE)
    {
   	    lastError = pConvert->close();
   	    return;
    }

    NSCPHInfo* pImport = new NSCPHInfo();

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
        pConvert->pDonnees->metAZero();
        pImport->pDonnees->metAZero();

        for (champ = 0; (champ < 5) && (tourner); champ++)
        {
      	    switch (champ)
            {
         	    case  0 : taille_champ = 10; break;
                case  1 : taille_champ = 10; break;
                case  2 : taille_champ = 40; break;
                case  3 : taille_champ = 10; break;
                case  4 : taille_champ = 10; break;
            }

            for (j = 0; (j < taille_champ) && (entree != EOF) && (entree != '|'); j++)
            {
         	    buffer[j] = entree;
                entree = getc(infile);
            }
            buffer[j] = '\0';
            if (entree == EOF)
         	    tourner = false;
            else if (entree == '|')
         	    entree = getc(infile);
            OemToAnsi(buffer, buffer);
            string sDate = "", sDateSP = "";

            switch (champ)
            {
         	    case  0 : strcpy(pImport->pDonnees->codeInterne, buffer); break;
                case  1 : strcpy(pImport->pDonnees->codePere,    buffer); break;
                case  2 : strcpy(pImport->pDonnees->nom, 	     buffer); break;
                case  3 :
                    sDate = buffer;
                    if (sDate != "")
                    {
                        sDateSP = string(sDate, 6, 4) +
                                  string(sDate, 3, 2) +
                                  string(sDate, 0, 2);
                      	strcpy(pImport->pDonnees->dateCreation, sDateSP.c_str());
                    }
                    else
                        strcpy(pImport->pDonnees->dateCreation, "");
                    break;
                case 4 :
                    sDate = buffer;
                    if (sDate != "")
                    {
                        sDateSP = string(sDate, 6, 4) +
                                  string(sDate, 3, 2) +
                                  string(sDate, 0, 2);
                      	strcpy(pImport->pDonnees->dateModif, sDateSP.c_str());
                    }
                    else
                        strcpy(pImport->pDonnees->dateModif, "");
                    break;
            }
        }
        //
        // On cherche si cet élément est deja référencé dans la base
        //
        pConvert->lastError = pConvert->chercheClef(&(string(pImport->pDonnees->codeInterne)),
                                                  "",
                                                  0,
                                                  keySEARCHEQ,
                                                  dbiWRITELOCK);
        // Si on n'a pas trouvé, on ajoute
        //
        if (pConvert->lastError != DBIERR_NONE)
        {
            *(pConvert->pDonnees) = *(pImport->pDonnees);
            strcpy(pConvert->pDonnees->modifie, "2");
            pConvert->lastError = pConvert->appendRecord();
            if (pConvert->lastError != DBIERR_NONE)
            {
      	        erreur("Erreur à l'ajout de la fiche CPH", 0, pConvert->lastError);
                fclose(infile);
                pConvert->lastError = pConvert->close();
                delete pImport;
                return;
            }
        }
        else
        {
            // On compare l'ancien et le nouveau
            if ((strcmp(pConvert->pDonnees->codePere, pImport->pDonnees->codePere) != 0) ||
                (strcmp(pConvert->pDonnees->nom,      pImport->pDonnees->nom) != 0))
                strcpy(pConvert->pDonnees->modifie, "1");

            strcpy(pConvert->pDonnees->codePere, 	 pImport->pDonnees->codePere);
            strcpy(pConvert->pDonnees->nom, 	     pImport->pDonnees->nom);
            strcpy(pConvert->pDonnees->dateCreation, pImport->pDonnees->dateCreation);
            strcpy(pConvert->pDonnees->dateModif, 	 pImport->pDonnees->dateModif);

            pConvert->lastError = pConvert->modifyRecord(TRUE);
            if (pConvert->lastError != DBIERR_NONE)
            {
      	        erreur("Erreur à la mise à jour de la fiche PCH", 0, pConvert->lastError);
                fclose(infile);
                pConvert->lastError = pConvert->close();
                delete pImport;
                return;
            }
        }
    }
  	fclose(infile);
  	lastError = pConvert->close();

    delete pImport;
}

//***************************************************************************
// 				  Implémentation des mèthodes NSPRData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPRData::NSPRData(NSPRData& rv)
{
    strcpy(code, 	  	 rv.code);
	strcpy(codeInterne,  rv.codeInterne);
    strcpy(codePere, 	 rv.codePere);
    strcpy(nom, 	     rv.nom);
    strcpy(dateCreation, rv.dateCreation);
    strcpy(dateModif, 	 rv.dateModif);
    strcpy(modifie, 	 rv.modifie);
}

NSPRData::~NSPRData()
{}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPRData&
NSPRData::operator=(NSPRData rv)
{
    strcpy(code, 	  	 rv.code);
	strcpy(codeInterne,  rv.codeInterne);
    strcpy(codePere, 	 rv.codePere);
    strcpy(nom, 	     rv.nom);
    strcpy(dateCreation, rv.dateCreation);
    strcpy(dateModif, 	 rv.dateModif);
    strcpy(modifie, 	 rv.modifie);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPRData::operator == ( const NSPRData& rv )
{
    if ((strcmp(code, 	  	 rv.code)        == 0) &&
   		(strcmp(codeInterne, rv.codeInterne) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à 0 les variables
//---------------------------------------------------------------------------
void
NSPRData::metAZero()
{
    //
	// Met les champs de données à zéro
	//
    memset(code,         0, PRO_CODE_LEN + 1);
	memset(codeInterne,  0, PRO_CODE_INTERNE_LEN + 1);
    memset(codePere,     0, PRO_CODE_PERE_LEN + 1);
    memset(nom,          0, PRO_NOM_LEN + 1);
    memset(dateCreation, 0, PRO_DATE_CREATION_LEN + 1);
    memset(dateModif,    0, PRO_DATE_MODIFICATION_LEN + 1);
    memset(modifie,      0, PRO_MODIFIE_LEN + 1);
}

//***************************************************************************
//							 Implémentation des mèthodes NSCPH
//***************************************************************************
//---------------------------------------------------------------------------
//  Constructeur.
//---------------------------------------------------------------------------
NSPR::NSPR(NSContexte* pCtx) : NSFiche(pCtx)
{
	pDonnees = new NSPRData();
}

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSPR::~NSPR()
{
    if (pDonnees)
    {
   	    delete pDonnees;
        pDonnees = 0   ;
    }
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSPR::alimenteFiche()
{
    alimenteChamp(pDonnees->code,         PRO_CODE_FIELD,              PRO_CODE_LEN);
    alimenteChamp(pDonnees->codeInterne,  PRO_CODE_INTERNE_FIELD,      PRO_CODE_INTERNE_LEN);
    alimenteChamp(pDonnees->codePere,     PRO_CODE_PERE_FIELD,         PRO_CODE_PERE_LEN);
    alimenteChamp(pDonnees->nom,          PRO_NOM_FIELD,               PRO_NOM_LEN);
    alimenteChamp(pDonnees->dateCreation, PRO_DATE_CREATION_FIELD,     PRO_DATE_CREATION_LEN);
    alimenteChamp(pDonnees->dateModif,    PRO_DATE_MODIFICATION_FIELD, PRO_DATE_MODIFICATION_LEN);
    alimenteChamp(pDonnees->modifie,      PRO_MODIFIE_FIELD,           PRO_MODIFIE_LEN);
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSPR::videFiche()
{
    videChamp(pDonnees->code,         PRO_CODE_FIELD,              PRO_CODE_LEN);
    videChamp(pDonnees->codeInterne,  PRO_CODE_INTERNE_FIELD,      PRO_CODE_INTERNE_LEN);
    videChamp(pDonnees->codePere,     PRO_CODE_PERE_FIELD,         PRO_CODE_PERE_LEN);
    videChamp(pDonnees->nom,          PRO_NOM_FIELD,               PRO_NOM_LEN);
    videChamp(pDonnees->dateCreation, PRO_DATE_CREATION_FIELD,     PRO_DATE_CREATION_LEN);
    videChamp(pDonnees->dateModif,    PRO_DATE_MODIFICATION_FIELD, PRO_DATE_MODIFICATION_LEN);
    videChamp(pDonnees->modifie,      PRO_MODIFIE_FIELD,           PRO_MODIFIE_LEN);
}

//---------------------------------------------------------------------------
//  Ouvre la table primaire
//---------------------------------------------------------------------------
DBIResult
NSPR::open()
{
    char tableName[] = "T_PR.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//***************************************************************************
// 				Implémentation des méthodes NSSPInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSPRInfo::NSPRInfo()
{
    // Crée l'objet de données
	pDonnees = new NSPRData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSPatPatho
//---------------------------------------------------------------------------
NSPRInfo::NSPRInfo(NSPR* pCPH)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPRData();
	//
	// Copie les valeurs de l'objet SGBD
	//
	*pDonnees = *(pCPH->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPRInfo::NSPRInfo(NSPRInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPRData();
	//
	// Copie les valeurs du NSSPInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}


//destructeur
NSPRInfo::~NSPRInfo()
{
	if(pDonnees)
    {
        delete pDonnees;
        pDonnees = 0;
    }
}
//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPRInfo&
NSPRInfo::operator=(NSPRInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int NSPRInfo::operator == ( const NSPRInfo& o )
{
	return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
// 				  Implémentation des mèthodes NSClasseProduit
//***************************************************************************
NSClasseProduit::NSClasseProduit(NSContexte* pCtx)
                :NSRoot(pCtx)
{
	pConvert = new NSPR(pContexte);
}

NSClasseProduit::~NSClasseProduit()
{
	delete pConvert;
}

//---------------------------------------------------------------------------
// Met à jour la table DB à partir du fichier TXT
//---------------------------------------------------------------------------
void
NSClasseProduit::Import()
{
    short   	 i, j, champ, taille_champ;
	int     	 entree;
	bool    	 tourner = true;
	char 		 buffer[100];
	DBIResult lastError;
    FILE* infile;

    string	 sCode;

  	//
  	// Ouverture du fichier d'importation
  	//
  	infile = fopen("\\NAUTILUS\\PR.TXT", "r");
  	if (infile == NULL)
    {
   	    erreur("Fichier PR.TXT non trouvé", 0, 0);
	  	return;
    }
  	setmode(fileno(infile), O_BINARY);
    //
  	// Ouverture de la table
  	//
  	//PV2_3Dialog(pNSSup->PrendWndCourant())->PeindreEdit(PAT_OUV, "Non");
  	lastError = pConvert->open();
  	if (lastError != DBIERR_NONE)
    {
   	    lastError = pConvert->close();
   	    return;
    }

    NSPRInfo* pImport = new NSPRInfo();

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
        pConvert->pDonnees->metAZero();
        pImport->pDonnees->metAZero();

        for (champ = 0; (champ < 5) && (tourner); champ++)
        {
      	    switch (champ)
            {
         	    case  0 : taille_champ =  6; break;
                case  1 : taille_champ =  6; break;
                case  2 : taille_champ = 40; break;
                case  3 : taille_champ = 10; break;
                case  4 : taille_champ = 10; break;
            }

            for (j = 0; (j < taille_champ) && (entree != EOF) && (entree != '|'); j++)
            {
         	    buffer[j] = entree;
                entree = getc(infile);
            }
            buffer[j] = '\0';
            if (entree == EOF)
         	    tourner = false;
            else if (entree == '|')
         	    entree = getc(infile);
            OemToAnsi(buffer, buffer);
            string sDate = "", sDateSP = "";

            switch (champ)
            {
         	    case  0 : strcpy(pImport->pDonnees->codeInterne, buffer); break;
                case  1 : strcpy(pImport->pDonnees->codePere,    buffer); break;
                case  2 : strcpy(pImport->pDonnees->nom, 	     buffer); break;
                case  3 :
                    sDate = buffer;
                    if (sDate != "")
                    {
                        sDateSP = string(sDate, 6, 4) +
                                  string(sDate, 3, 2) +
                                  string(sDate, 0, 2);
                      	strcpy(pImport->pDonnees->dateCreation, sDateSP.c_str());
                    }
                    else
                        strcpy(pImport->pDonnees->dateCreation, "");
                    break;
                case 4 :
                    sDate = buffer;
                    if (sDate != "")
                    {
                        sDateSP = string(sDate, 6, 4) +
                                  string(sDate, 3, 2) +
                                  string(sDate, 0, 2);
                      	strcpy(pImport->pDonnees->dateModif, sDateSP.c_str());
                    }
                    else
                        strcpy(pImport->pDonnees->dateModif, "");
                    break;
            }
        }

        //
        // Elaboration du code
        //
        string sCode = pImport->pDonnees->codeInterne;
        while (strlen(sCode.c_str()) < 4)
            sCode = '0' + sCode;
        sCode = "__" + sCode;

        //
        // On cherche si cet élément est deja référencé dans la base
        //
        pConvert->lastError = pConvert->chercheClef(&(string(pImport->pDonnees->codeInterne)),
                                                  "",
                                                  0,
                                                  keySEARCHEQ,
                                                  dbiWRITELOCK);
        // Si on n'a pas trouvé, on ajoute
        //
        if (pConvert->lastError != DBIERR_NONE)
        {
            strcpy(pConvert->pDonnees->codeInterne,  pImport->pDonnees->codeInterne);
            strcpy(pConvert->pDonnees->codePere,     pImport->pDonnees->codePere);
            strcpy(pConvert->pDonnees->nom,          pImport->pDonnees->nom);
            strcpy(pConvert->pDonnees->dateCreation, pImport->pDonnees->dateCreation);
            strcpy(pConvert->pDonnees->dateModif,    pImport->pDonnees->dateModif);

            strcpy(pConvert->pDonnees->code, sCode.c_str());
            strcpy(pConvert->pDonnees->modifie, "2");

            pConvert->lastError = pConvert->appendRecord();
            if (pConvert->lastError != DBIERR_NONE)
            {
      	        erreur("Erreur à l'ajout de la fiche PR", 0, pConvert->lastError);
                fclose(infile);
                pConvert->lastError = pConvert->close();
                delete pImport;
                return;
            }
        }
        else
        {
            // On compare l'ancien et le nouveau
            if ((strcmp(pConvert->pDonnees->codePere, pImport->pDonnees->codePere) != 0) ||
                (strcmp(pConvert->pDonnees->nom,      pImport->pDonnees->nom) != 0))
                strcpy(pConvert->pDonnees->modifie, "1");

            strcpy(pConvert->pDonnees->codePere, 	 pImport->pDonnees->codePere);
            strcpy(pConvert->pDonnees->nom, 	     pImport->pDonnees->nom);
            strcpy(pConvert->pDonnees->dateCreation, pImport->pDonnees->dateCreation);
            strcpy(pConvert->pDonnees->dateModif, 	 pImport->pDonnees->dateModif);
            strcpy(pConvert->pDonnees->code, sCode.c_str());

            pConvert->lastError = pConvert->modifyRecord(TRUE);
            if (pConvert->lastError != DBIERR_NONE)
            {
      	        erreur("Erreur à la mise à jour de la fiche PR", 0, pConvert->lastError);
                fclose(infile);
                pConvert->lastError = pConvert->close();
                delete pImport;
                return;
            }
        }
        //***************************************
  		//mettre à jour la base leximed.db
  		//***************************************
        //
        // On cherche si cet élément est deja référencé dans la base
        //
        NSSuper* pSuper = pContexte->getSuperviseur();

        pSuper->getDico()->pLexiMed->lastError = pSuper->getDico()->pLexiMed->chercheClef(&sCode,
                                                  "CODE",
                                                  NODEFAULTINDEX,
                                                  keySEARCHEQ,
                                                  dbiWRITELOCK);
        // Si on n'a pas trouvé, on ajoute
        //
        if (pSuper->getDico()->pLexiMed->lastError != DBIERR_NONE)
        {
            strcpy(pSuper->getDico()->pLexiMed->pDonnees->libelle, pConvert->pDonnees->nom);
            strcpy(pSuper->getDico()->pLexiMed->pDonnees->grammaire, "INV");
            strcpy(pSuper->getDico()->pLexiMed->pDonnees->code, pConvert->pDonnees->code);

            pSuper->getDico()->pLexiMed->lastError = pSuper->getDico()->pLexiMed->appendRecord();
            if (pSuper->getDico()->pLexiMed->lastError != DBIERR_NONE)
            {
      	        erreur("Erreur à l'ajout de leximed", 0, pSuper->getDico()->pLexiMed->lastError);
                pSuper->getDico()->pLexiMed->lastError = pSuper->getDico()->pLexiMed->close();
                delete pImport;
                return;
            }
        }
        else
        {
            strcpy(pSuper->getDico()->pLexiMed->pDonnees->libelle, pConvert->pDonnees->nom);
            strcpy(pSuper->getDico()->pLexiMed->pDonnees->grammaire, "INV");
            strcpy(pSuper->getDico()->pLexiMed->pDonnees->code, pConvert->pDonnees->code);
            pSuper->getDico()->pLexiMed->lastError = pSuper->getDico()->pLexiMed->modifyRecord(TRUE);
            if (pSuper->getDico()->pLexiMed->lastError != DBIERR_NONE)
            {
      	        erreur("Erreur à la modification de leximed", 0, pSuper->getDico()->pLexiMed->lastError);
                pSuper->getDico()->pLexiMed->lastError = pSuper->getDico()->pLexiMed->close();
                delete pImport;
                return;
            }
        }
    }
  	fclose(infile);
  	lastError = pConvert->close();

    delete pImport;
}
