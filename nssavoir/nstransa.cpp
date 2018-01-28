//---------------------------------------------------------------------------//  NSTRANSA.CPP
//  Implémentation des transactions
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

//using namespace std;
#ifdef _INCLUS  #include "partage\Nsglobal.h"
  #include "nsbb\nsutidlg.h"
#endif

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "partage\nsperson.h"
#include "nssavoir\nstransa.h"

//***************************************************************************// Implémentation des méthodes NSTransaction
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSTransacData::NSTransacData()
{
	//
	// Met les champs de données à zéro
	//
    metAZero();
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSTransacData::NSTransacData(NSTransacData& rv)
{
#ifdef _MUE
    strcpy(patient,   rv.patient);
#endif
	strcpy(code, 	  rv.code);
	strcpy(operateur, rv.operateur);
	strcpy(dateDeb,   rv.dateDeb);
    strcpy(dateFin,   rv.dateFin);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSTransacData::~NSTransacData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSTransacData&
NSTransacData::operator=(NSTransacData src)
{
#ifdef _MUE
    strcpy(patient,   src.patient);
#endif
	strcpy(code, 	  src.code);
	strcpy(operateur, src.operateur);
	strcpy(dateDeb,   src.dateDeb);
    strcpy(dateFin,   src.dateFin);

	return *this;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSTransacData::operator == ( const NSTransacData& o )
{
#ifdef _MUE
	if ((strcmp(patient, o.patient) == 0) &&
        (strcmp(code, o.code) == 0))#else
	if ((strcmp(code, o.code) == 0))
#endif
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSTransacData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
#ifdef _MUE
    memset(patient,   0, TRAN_PATIENT_LEN + 1);
#endif
	memset(code, 	  0, TRAN_CODE_LEN + 1);
	memset(operateur, 0, TRAN_OPERATEUR_LEN + 1);
	memset(dateDeb,   0, TRAN_DATE_DEB_LEN + 1);
    memset(dateFin,   0, TRAN_DATE_FIN_LEN + 1);
}

/*
//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSTransaction::NSTransaction(NSContexte* pCtx) : NSFiche(pCtx)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSTransacData();
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSTransaction::NSTransaction(NSTransaction& rv) : NSFiche(rv.pContexte)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSTransacData();
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSTransaction::~NSTransaction()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSTransaction::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSTransaction::alimenteFiche()
{
#ifdef _MUE
    alimenteChamp(pDonnees->patient,   TRAN_PATIENT_FIELD, 	 TRAN_PATIENT_LEN);
#endif
    alimenteChamp(pDonnees->code, 	   TRAN_CODE_FIELD, 	 TRAN_CODE_LEN);
	alimenteChamp(pDonnees->operateur, TRAN_OPERATEUR_FIELD, TRAN_OPERATEUR_LEN);
    alimenteChamp(pDonnees->dateDeb,   TRAN_DATE_DEB_FIELD,  TRAN_DATE_DEB_LEN);
    alimenteChamp(pDonnees->dateFin,   TRAN_DATE_FIN_FIELD,  TRAN_DATE_FIN_LEN);
}
//---------------------------------------------------------------------------//  Fonction :   	NSTransaction::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSTransaction::videFiche()
{
#ifdef _MUE    videChamp(pDonnees->patient,   TRAN_PATIENT_FIELD, 	 TRAN_PATIENT_LEN);#endif    videChamp(pDonnees->code, 	   TRAN_CODE_FIELD, 	 TRAN_CODE_LEN);
	videChamp(pDonnees->operateur, TRAN_OPERATEUR_FIELD, TRAN_OPERATEUR_LEN);
    videChamp(pDonnees->dateDeb,   TRAN_DATE_DEB_FIELD,  TRAN_DATE_DEB_LEN);
    videChamp(pDonnees->dateFin,   TRAN_DATE_FIN_FIELD,  TRAN_DATE_FIN_LEN);
}

bool
NSTransaction::fermeTransaction()
{
	bool        onDoitFermer = false;

#ifdef _MUE
    string sTranSys = pContexte->getTransactionSysteme(false) ;

    if ((sTranSys == "") && (pContexte->getPatient() == 0))
        return true;
#endif

    if (!isOpen)
    {
   	    lastError = open();
        if (lastError != DBIERR_NONE)
		{
			erreur("Il n'est pas possible d'ouvrir le fichier des transactions.", standardError, lastError, pContexte->GetMainWindow()->GetHandle());
			return false;
		}
        onDoitFermer = true;
    }
    //
    // Recherche de la fiche transaction
    //
#ifndef _MUE
    string sCleTran = pContexte->getTransaction();
#else
    string sCleTran;
    if (sTranSys != "")
        sCleTran = string("_------") + sTranSys;
    else
        sCleTran = pContexte->getPatient()->getNss() + pContexte->getTransaction();
#endif

    lastError = chercheClef(&(sCleTran),                            "",
                            0,
                            keySEARCHEQ,
                            dbiWRITELOCK);
    if (lastError != DBIERR_NONE)
    {
   	    erreur("Il n'est pas possible de trouver la transaction en cours au sein du fichier des transactions.", standardError, lastError, pContexte->GetMainWindow()->GetHandle());
        return false;
    }
    lastError = getRecord();
    if (lastError != DBIERR_NONE)
    {
   	    erreur("Le fichier des transactions est défectueux.", standardError, lastError, pContexte->GetMainWindow()->GetHandle());
        return false;
    }
    //
    // Inscription de la date de fin
    //
    char date[9];
    donne_date_duJour(date);
    char heure[7];
    donne_heure(heure);
    strcpy(pDonnees->dateFin, date);
    strncat(pDonnees->dateFin, heure, 4);
    //
    // Mise à jour de la fiche
    //
    lastError = modifyRecord(TRUE);
    if (lastError != DBIERR_NONE)
    {
   	    erreur("Il n'est pas possible de mettre à jour le fichier des transactions.", standardError, lastError, pContexte->GetMainWindow()->GetHandle());
        return false;
    }

    if (onDoitFermer)
    {
   	    lastError = close();
        if (lastError != DBIERR_NONE)
            erreur("Il n'est pas possible de fermer le fichier des transactions.", standardError, lastError, pContexte->GetMainWindow()->GetHandle());
    }

#ifndef _MUE
    pContexte->setTransaction("");
#else
    if (sTranSys != "")
        pContexte->setTransactionSysteme("");
    else
        pContexte->setTransaction("");#endif
    return true;}

boolNSTransaction::nouvelleTransaction(bool bTranSys)
{
    bool         onDoitFermer = false;

#ifndef _MUE
    if (pContexte->getTransaction() != "")   	    fermeTransaction();
#else
    if (bTranSys)
    {
        if (pContexte->getTransactionSysteme(false) != "")
            fermeTransaction();
    }
    else
    {
        if (pContexte->getTransaction() != "")
            fermeTransaction();
    }
#endif
    if (!isOpen)
    {
   	    lastError = open();
        if (lastError != DBIERR_NONE)
        {
            erreur("Il n'est pas possible d'ouvrir le fichier des transactions.", standardError, lastError, pContexte->GetMainWindow()->GetHandle());
			return false;
		}
        onDoitFermer = true;
    }
	//
    // Prise du nouveau code
    //
    char szNewCode[TRAN_CODE_LEN + 1];

    if (!DonneNouveauCode(szNewCode, bTranSys))
   	    return false;

    // on remet pDonnees à zéro pour enlever les données
    // chargées par DonneNouveauCode()
    pDonnees->metAZero();
    strcpy(pDonnees->code, szNewCode);

    //    // Inscription du code de l'utilisateur
    //
    if (pContexte->getUtilisateur())
        strcpy(pDonnees->operateur, (pContexte->getUtilisateurID()).c_str());    else        memset(pDonnees->operateur, '-', TRAN_OPERATEUR_LEN);    //    // Inscription de la date de début
    //
    char date[9] ;
    donne_date_duJour(date) ;
    char heure[7] ;
    donne_heure(heure) ;
    strcpy(pDonnees->dateDeb, date) ;
    strncat(pDonnees->dateDeb, heure, 4) ;
    //
    // Mise à jour de la fiche
    //
#ifndef _MUE
    lastError = modifyRecord(TRUE) ;
#else
    if (bTranSys)
        strcpy(pDonnees->patient, "_------");
    else
        strcpy(pDonnees->patient, pContexte->getPatient()->getNss().c_str()) ;

    lastError = appendRecord() ;
#endif
    if (lastError != DBIERR_NONE)
    {
   	    erreur("Il n'est pas possible de mettre à jour le fichier des transactions.", standardError, lastError, pContexte->GetMainWindow()->GetHandle());
        return false ;
    }

    if (onDoitFermer)
    {
   	    lastError = close();
        if (lastError != DBIERR_NONE)
            erreur("Il n'est pas possible de fermer le fichier des transactions.", standardError, lastError, pContexte->GetMainWindow()->GetHandle());
    }

#ifndef _MUE
    pContexte->setTransaction(string(pDonnees->code));#else
    if (bTranSys)
        pContexte->setTransactionSysteme(string(pDonnees->code));
    else
        pContexte->setTransaction(string(pDonnees->code));
#endif
    return true;
}

bool
NSTransaction::DonneNouveauCode(char* code, bool bTranSys)
{
#ifdef _MUE

    string sCodeTrans;
    string sNss;

	char PremOrdre[TRAN_CODE_LEN + 1], DernOrdre[TRAN_CODE_LEN +1 ];
	//    // Préparation des compteurs mini et maxi
	//

    int i;
    memset(PremOrdre, '0', TRAN_CODE_LEN) ;
    memset(DernOrdre, 'z', TRAN_CODE_LEN);
	PremOrdre[TRAN_CODE_LEN]  = '\0';
	DernOrdre[TRAN_CODE_LEN]  = '\0';
    PremOrdre[0]                = '_';
	DernOrdre[0]                = '_';

	PremOrdre[TRAN_CODE_LEN-1] = '1';
	//	// On tente de se positionner sur le dernier code possible : "zzz"
	//

	//strcpy(codeDocu, pSuperviseur->pUtilisateur->pPatient->pDonnees->nss);	//strcat(codeDocu, DernOrdre);
    if (bTranSys)
        sNss = string("_------");
    else
        sNss = pContexte->getPatient()->getNss();

    sCodeTrans = sNss + string(DernOrdre);
	lastError = chercheClef(&sCodeTrans,                                "",
                                0,
                                keySEARCHGEQ,
                                dbiWRITELOCK);

	//	// Si on se trouve positionné en fin de fichier on recule
	//
	if (lastError == DBIERR_EOF)
		lastError = precedent(dbiWRITELOCK);

	//	// Toute autre erreur est anormale
	//
	else if (lastError != DBIERR_NONE)
	{
		erreur("Le fichier des transactions est endommagé.", standardError, lastError);
		return false;
	}
	//
	// On récupère l'enregistrement
	//
	lastError = getRecord();
	//
	// Si on est en fin ou début de fichier, c'est qu'il est vide
	//
	if ((lastError == DBIERR_BOF) ||
		 (lastError == DBIERR_EOF) ||
		 (lastError == DBIERR_NOCURRREC))
	{
		strcpy(code, PremOrdre);
		return true;
	}

	//	// Toute autre erreur est anormale
	//

	if (lastError != DBIERR_NONE)	{
		erreur("Le fichier des transactions est endommagé.", standardError, lastError);
		return false;
    }

	//	// Si le document trouvé n'appartient pas au patient, on recule d'un cran
	//

	if (string(pDonnees->patient) != sNss)	{
		lastError = precedent(dbiWRITELOCK);
		//
		// Si on est en début de fichier, c'est qu'il n'y a pas de document
		//
		if (lastError == DBIERR_BOF)
		{
			strcpy(code, PremOrdre);
			return true;
		}
		//
		// Toute autre erreur est anormale
		//
		if (lastError == DBIERR_NONE)
			lastError = getRecord();

		if (lastError != DBIERR_NONE)		{
			erreur("Le fichier des transactions est endommagé.", standardError, lastError);
			return false;
		}
		//
		// Si la transaction trouvée n'appartient pas au patient, il n'y a pas
		// encore de transaction pour ce patient
		//
		if (string(pDonnees->patient) != sNss)
        {
			strcpy(code, PremOrdre);
			return true;
		}
	}

    // Si ce n'est pas un document local, c'est qu'il n'y en a pas
    //
    if (pDonnees->code[0] != '_')
    {
        strcpy(code, PremOrdre);
        return true;
    }

	//
	// On est positionné sur le dernier document du patient, que l'on doit
	// incrémenter pour avoir le nouveau code
	//
	char compteur[DOC_CODE_DOCUM_LEN + 1] ;
	strcpy(compteur, pDonnees->code) ;
	//
    // On incrémente le compteur
	//
	int j;
	int calculer = 1;

	i = strlen(compteur) - 1;
	while (calculer)    {
        j = (int) compteur[i];
        j++;

        if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))        {
            compteur[i] = (char) j;
            calculer = 0;
        }
        else if ((j > '9') && (j < 'A'))
        {
            compteur[i] = 'A';
            calculer = 0;
        }
        else
        {
            compteur[i] = '0';

            if (i == 0)            {
                erreur("Compteur de transactions saturé.", standardError, 0);
                return false;
            }
            i--;
        }
    }
	strcpy(code, compteur);

#else

    char compteur[TRAN_CODE_LEN + 1];
    //
    // Positionnement à la fin du fichier
    //
    lastError = fin(dbiREADLOCK);
    if ((lastError == DBIERR_EOF) || (lastError == DBIERR_BOF))
    {
        int i = 0;
        for (; i < TRAN_CODE_LEN - 1; i++)
      	    compteur[i] = '0';
        compteur[i++] = '1';
        compteur[i]   = '\0';
    }
    else if (lastError != DBIERR_NONE)
	{
        erreur("Le fichier des transactions est endommagé.", 0, lastError, pContexte->GetMainWindow()->GetHandle());
		return false;
	}
    else
    {
   	    //
		// Récupération de l'enregistrement
		//
		lastError = getRecord();
   	    if (lastError != DBIERR_NONE)
		{
            erreur("Le fichier des transactions est endommagé.", 0, lastError, pContexte->GetMainWindow()->GetHandle());
			return false;
		}
		strcpy(compteur, pDonnees->code);
		//
		// Incrémentation du compteur
		//
		int j;
		bool calculer = true;
		int i = strlen(compteur) - 1;
		while (calculer)
   	    {
   		    j = (int) compteur[i] ;
      	    j++ ;
      	    if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))
      	    {
      		    compteur[i] = (char) j ;
         	    calculer = false ;
      	    }
      	    else if ((j > '9') && (j < 'A'))
      	    {
      		    compteur[i] = 'A' ;
         	    calculer = false ;
      	    }
      	    else
      	    {
      		    compteur[i] = '0' ;
         	    if (i == 0)
         	    {
         		    erreur("Compteur de transactions saturé.", 0, 0) ;
            	    return false ;
         	    }
         	    i-- ;
      	    }
        }
    }
    pDonnees->metAZero() ;
    strcpy(pDonnees->code, compteur) ;
    //
    // On ajoute immédiatement la fiche pour réserver le numéro de transaction
    //
    appendRecord() ;
    if (lastError != DBIERR_NONE)
	{
		erreur("Le fichier des transactions ne peut plus recevoir ne nouveaux enregistrements.", 0, lastError, pContexte->GetMainWindow()->GetHandle());
		return false;
	}

    strcpy(code, compteur) ;

#endif

	return true;
}

//---------------------------------------------------------------------------//  Fonction :  	NSTransaction::getRecord()
//
//  Description : Prend l'enregistrement en cours et assigne la valeur des
//             	champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSTransaction::getPatRecord()
{
	//
	// La table est-elle ouverte ?
    //
	if (!isOpen)
        return(lastError = ERROR_TABLENOTOPEN);
	//
	// Appel de la classe de base pour récupérer l'enregistrement.
	//
	lastError = getDbiRecord(dbiWRITELOCK);

	return(lastError);
}

//---------------------------------------------------------------------------
//  Fonction :  	NSTransaction::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSTransaction::open()
{
#ifndef _MUE
    char tableName[] = "TRANSAC.DB";#else    char tableName[] = "NSMTRAN.DB";#endif	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSPatholog::Create()
//
//---------------------------------------------------------------------------
bool
NSTransaction::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSPatholog::Modify()
//
//---------------------------------------------------------------------------
bool
NSTransaction::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSTransaction&
NSTransaction::operator=(NSTransaction src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSTransaction::operator == ( const NSTransaction& o )
{
	 return (*pDonnees == *(o.pDonnees));
}
*/

//---------------------------------------------------------------------------
//  Fonction:		NSTransacInfo::NSTransacInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSTransacInfo::NSTransacInfo()
{
	// Crée l'objet de données
	pDonnees = new NSTransacData() ;
}

/*
//---------------------------------------------------------------------------
//  Fonction:		NSTransacInfo::NSTransacInfo(NSTransaction*)
//
//  Description:	Constructeur à partir d'un NSTransaction
//---------------------------------------------------------------------------
NSTransacInfo::NSTransacInfo(NSTransaction* pTransac)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSTransacData();
	//
	// Copie les valeurs du NSTransaction
	//
	*pDonnees = *(pTransac->pDonnees);
}
*/

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTransacInfo::NSTransacInfo(NSTransacInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSTransacData();
	//
	// Copie les valeurs du NSTransacInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSTransacInfo::~NSTransacInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSTransacInfo&
NSTransacInfo::operator=(NSTransacInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int NSTransacInfo::operator == ( const NSTransacInfo& o )
{
	 return (*pDonnees == *(o.pDonnees));
}

