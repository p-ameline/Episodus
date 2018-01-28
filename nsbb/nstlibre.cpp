//    NSTLIBRE.CPP//    KADDACHI Hafedh août 97
//  Implémentation des objets de TLIBRE.DB
//---------------------------------------------------------------------------
#include <cstring.h>
#include <windows.h>
#include <mem.h>
#include <bde.hpp>

#ifdef _INCLUS
  #include "partage\Nsglobal.h"
  #include "nsbb\nsutidlg.h"
  #include "inclus\InclusWrapper.h"
#endif

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\nsdocdlg.h"

#include "nsbb\nStlibre.h"

//***************************************************************************
// 				  Implémentation des mèthodes NSTexteLibreData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTexteLibreData::NSTexteLibreData(NSTexteLibreData& rv)
{
#ifdef _MUE
    strcpy(patient, rv.patient);
#endif
    strcpy(code, 	rv.code);
    strcpy(suivant, rv.suivant);
    strcpy(texte, 	rv.texte);
}

NSTexteLibreData::~NSTexteLibreData()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSTexteLibreData&
NSTexteLibreData::operator=(NSTexteLibreData rv)
{
#ifdef _MUE
    strcpy(patient, rv.patient);
#endif
    strcpy(code, 	rv.code);
    strcpy(suivant, rv.suivant);
    strcpy(texte, 	rv.texte);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSTexteLibreData::operator == ( const NSTexteLibreData& o )
{
    if ((strcmp(code, 	 o.code) 	== 0) &&
#ifdef _MUE
        (strcmp(patient, o.patient) == 0) &&
#endif
        (strcmp(texte, 	 o.texte) 	== 0) &&
        (strcmp(suivant, o.suivant) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------//  Met à 0 les variables.
//---------------------------------------------------------------------------
void
NSTexteLibreData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
#ifdef _MUE
    memset(patient, 0, TLIBRE_PAT_LEN     + 1);
#endif
	memset(code,	0, TLIBRE_CODE_LEN    + 1);
    memset(texte, 	0, TLIBRE_TEXTE_LEN   + 1);
    memset(suivant, 0, TLIBRE_SUIVANT_LEN + 1);
}

//---------------------------------------------------------------------------//  NSTexteLibreData::metABlanc()
//---------------------------------------------------------------------------
void
NSTexteLibreData::metABlanc()
{
	//
	// Met les champs de données à blanc
	//
#ifdef _MUE
    memset(patient, ' ', TLIBRE_PAT_LEN);
#endif
	memset(code,	' ', TLIBRE_CODE_LEN);
    memset(texte, 	' ', TLIBRE_TEXTE_LEN);
    memset(suivant, ' ', TLIBRE_SUIVANT_LEN);
}

//***************************************************************************
// 				Implémentation des méthodes NSTlibreInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSTlibreInfo::NSTlibreInfo()
{
    // Crée l'objet de données
	pDonnees = new NSTexteLibreData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSPatPatho
//---------------------------------------------------------------------------
NSTlibreInfo::NSTlibreInfo(NSTextelibreBase* pNSTextelibre)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSTexteLibreData();
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pNSTextelibre->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTlibreInfo::NSTlibreInfo(NSTlibreInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSTexteLibreData();
	//
	// Copie les valeurs du NSTlibreInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSTlibreInfo::~NSTlibreInfo()
{
    if (pDonnees)
        delete pDonnees;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSTlibreInfo&
NSTlibreInfo::operator=(NSTlibreInfo src)
{
    *pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int NSTlibreInfo::operator == ( const NSTlibreInfo& o )
{
	return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************//				  Implémentation des mèthodes NSTextelibre
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSTextelibreBase::NSTextelibreBase(NSContexte* pCtx) : NSFiche(pCtx)
{
    pDonnees = new NSTexteLibreData();
}

//---------------------------------------------------------------------------//  Destructeur.
//---------------------------------------------------------------------------
NSTextelibreBase::~NSTextelibreBase()
{
    if (pDonnees)
   	    delete pDonnees;
}

//---------------------------------------------------------------------------//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSTextelibreBase::alimenteFiche()
{
#ifdef _MUE    alimenteChamp(pDonnees->patient, TLIBRE_PAT_FIELD,     TLIBRE_PAT_LEN);#endif	alimenteChamp(pDonnees->code,    TLIBRE_CODE_FIELD,    TLIBRE_CODE_LEN);
    alimenteChamp(pDonnees->texte,   TLIBRE_TEXTE_FIELD,   TLIBRE_TEXTE_LEN);
    alimenteChamp(pDonnees->suivant, TLIBRE_SUIVANT_FIELD, TLIBRE_SUIVANT_LEN);
}

//---------------------------------------------------------------------------//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSTextelibreBase::videFiche()
{
#ifdef _MUE
    videChamp(pDonnees->patient, TLIBRE_PAT_FIELD,     TLIBRE_PAT_LEN);
#endif
	videChamp(pDonnees->code,    TLIBRE_CODE_FIELD,    TLIBRE_CODE_LEN);
    videChamp(pDonnees->texte,   TLIBRE_TEXTE_FIELD,   TLIBRE_TEXTE_LEN);
    videChamp(pDonnees->suivant, TLIBRE_SUIVANT_FIELD, TLIBRE_SUIVANT_LEN);
}

//---------------------------------------------------------------------------//  Ouvre la table primaire
//---------------------------------------------------------------------------
DBIResult
NSTextelibreBase::open()
{
    return false ;
}
//***************************************************************************// 				  Implémentation des mèthodes NSTlibreBase
//***************************************************************************
NSTlibreBase::NSTlibreBase(NSContexte* pCtx)
             :NSRoot(pCtx)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTlibreBase::NSTlibreBase(NSTlibreBase& rv)
             :NSRoot(rv.pContexte)
{}

NSTlibreBase::~NSTlibreBase()
{}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSTlibreBase&
NSTlibreBase::operator=(NSTlibreBase rv)
{
    return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSTlibreBase::operator == (const NSTlibreBase& o )
{
    return 1;
}

//---------------------------------------------------------------------------// Calcule le code à donner au nouveau texte libre
//---------------------------------------------------------------------------
bool
NSTlibreBase::IncrementeCode(string* pTexte)
{
    int i = strlen(pTexte->c_str()) ;

#ifndef _MUE
    if (i == 0)   	    return false;
#else
    if (i == 1)
        return false;
#endif

    bool tourner = true;
#ifndef _MUE    while ((tourner) && (i > 0))#else    while ((tourner) && (i > 1))#endif
    {        i--;
   	    if ((((*pTexte)[i] >= '0') && ((*pTexte)[i] < '9')) ||
      	    (((*pTexte)[i] >= 'A') && ((*pTexte)[i] < 'Z')))
        {
            (*pTexte)[i] = char((*pTexte)[i] + 1);
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

//----------------------------------------------------------------------------//  Insertion d'un nouveau texte dans la base de textes libres
//
//  sTexte : string qui contient le texte à stocker
//	pCode  : code de la fiche qui va contenir le texte
//	         le code doit être différent de "00000"
//----------------------------------------------------------------------------

boolNSTlibreBase::InsereTexte(string sCodePat, string *pCode, string sTexte)
{
    //le nouveau code est le dernier + 1
    string code;
    string suivant;
    string sNss;

try{
#ifdef _MUE
    // Pour éviter de stocker des textes libres vides
    // on considère par convention qu'un texte libre vide
    // est associé à un code vide.
    if (sTexte == "")
    {
        *pCode = "";
        return true;
    }
#endif

    if (!(creerCurseur()))
        return false;

	pNSTexteLibre->lastError = pNSTexteLibre->open();
    if (pNSTexteLibre->lastError != DBIERR_NONE)    {
        erreur("1.Erreur à l'ouverture de la base des textes libres.", standardError, pNSTexteLibre->lastError);
        delete pNSTexteLibre;
        pNSTexteLibre = 0;
		return false;
    }

    bool blocage = true;
    while (blocage)    {
   	    // blocage de la base en écriture
   	    pNSTexteLibre->lastError = DbiAcqTableLock(pNSTexteLibre->PrendCurseur(), dbiWRITELOCK);

   	    if (pNSTexteLibre->lastError == DBIERR_LOCKED)            blocage = true;
        else
            blocage = false;
    }

    if (pNSTexteLibre->lastError != DBIERR_NONE)	{
        erreur("2.Erreur au bloquage de la base des textes libres.", standardError, pNSTexteLibre->lastError);
        delete pNSTexteLibre;
        pNSTexteLibre = 0;
		return false;
    }

#ifndef _MUE    pNSTexteLibre->lastError = pNSTexteLibre->fin(dbiWRITELOCK);
    //	// Si la table est vide
	//

	if ((pNSTexteLibre->lastError == DBIERR_BOF) ||        (pNSTexteLibre->lastError == DBIERR_EOF) ||
        (pNSTexteLibre->lastError == DBIERR_NOCURRREC))
    {
   	    *pCode = string(TLIBRE_CODE_LEN, '0');
        IncrementeCode(pCode);
        code = *pCode;
    }
    else
    {
   	    if (pNSTexteLibre->lastError != DBIERR_NONE)
		{
            erreur("3.Erreur d'accés à la fin de la base des textes libres.", 0, pNSTexteLibre->lastError);
      	    pNSTexteLibre->close();
      	    delete pNSTexteLibre;
            pNSTexteLibre = 0;
			return false;
        }

   	    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
   	    if (pNSTexteLibre->lastError != DBIERR_NONE)   	    {
   		    erreur("4.Le fichier de la base des textes libres est inutilisable.", 0, pNSTexteLibre->lastError);
      	    pNSTexteLibre->close();
      	    delete pNSTexteLibre;
            pNSTexteLibre = 0;
      	    return false;
   	    }

        *pCode = string(pNSTexteLibre->pDonnees->code);        IncrementeCode(pCode);
        code = *pCode;
    }
#else
    sNss = sCodePat;

    // on regarde si le texte n'existe pas déjà, auquel cas
    // on peut réatribuer son code.
    int premier = 0;
    int dernier;
    string sTL;
    string sTLTrouve;
    string sCodeTrouve;

    if (strlen(sTexte.c_str()) <= TLIBRE_TEXTE_LEN)
        dernier = strlen(sTexte.c_str());
    else
        dernier = TLIBRE_TEXTE_LEN;

    // on crée la chaine amorce
    sTL = string(sTexte, premier, dernier);

    CURProps curProps;
    DBIResult lastError = DbiGetCursorProps(pNSTexteLibre->PrendCurseur(), curProps) ;
    Byte* pIndexRec = new Byte[curProps.iRecBufSize] ;
    memset(pIndexRec, 0, curProps.iRecBufSize) ;
    DbiPutField(pNSTexteLibre->PrendCurseur(), TLIBRE_PAT_FIELD,    pIndexRec, (Byte*) sNss.c_str()) ;
    DbiPutField(pNSTexteLibre->PrendCurseur(), TLIBRE_TEXTE_FIELD,  pIndexRec, (Byte*) sTL.c_str()) ;

    pNSTexteLibre->lastError = pNSTexteLibre->chercheClefComposite("TEXTE_INDEX",
                                                                    NODEFAULTINDEX,
                                                                    keySEARCHEQ,
                                                                    dbiWRITELOCK,
                                                                    pIndexRec) ;
    delete[] pIndexRec ;
    if ((pNSTexteLibre->lastError != DBIERR_NONE) && (pNSTexteLibre->lastError != DBIERR_RECNOTFOUND))
    {
        erreur("Le fichier des textes libres est endommagé.", standardError, pNSTexteLibre->lastError);
        pNSTexteLibre->close() ;
        delete pNSTexteLibre ;
        return false;
    }

    // Si on ne trouve pas l'amorce, on sort.
    if (pNSTexteLibre->lastError == DBIERR_NONE)
    {
        pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
        if (pNSTexteLibre->lastError != DBIERR_NONE)
        {
            erreur("Le fichier des textes libres est endommagé.", standardError, pNSTexteLibre->lastError);
            pNSTexteLibre->close() ;
            delete pNSTexteLibre;
            return false;
        }

        // Sert à la recherche du texte
        NSTlibre* pNSTlibre = new NSTlibre(pContexte);

        do
        {
            if ((string(pNSTexteLibre->pDonnees->patient) != sNss) ||
                (string(pNSTexteLibre->pDonnees->texte) != sTL))
                break ;

            sCodeTrouve = string(pNSTexteLibre->pDonnees->code);

            if (!pNSTlibre->RecupereTexte(sNss, sCodeTrouve, &sTLTrouve))
            {
		        erreur("Le fichier des textes libres est endommagé.", standardError, pNSTexteLibre->lastError);
		        pNSTexteLibre->close() ;
		        delete pNSTexteLibre;
                delete pNSTlibre;
		        return false;
            }

            // Si on trouve le même texte libre dans la base, on renvoie son code
            if (sTLTrouve == sTexte)
            {
		        *pCode = sCodeTrouve;
		        pNSTexteLibre->close() ;
		        delete pNSTexteLibre;
                delete pNSTlibre;
		        return true;
            }

            pNSTexteLibre->lastError = pNSTexteLibre->suivant(dbiWRITELOCK);

		    if ((pNSTexteLibre->lastError != DBIERR_NONE) &&
			    (pNSTexteLibre->lastError != DBIERR_EOF))
            {
			    erreur("Erreur à l'accès au texte libre suivant.", standardError, pNSTexteLibre->lastError);
                pNSTexteLibre->close() ;
		        delete pNSTexteLibre;
                delete pNSTlibre;
                return false ;
            }
		    else if (pNSTexteLibre->lastError == DBIERR_NONE)
		    {
			    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
			    if (pNSTexteLibre->lastError != DBIERR_NONE)
                {
				    erreur("Erreur à la lecture d'un texte libre.", standardError, pNSTexteLibre->lastError);
                    pNSTexteLibre->close() ;
		            delete pNSTexteLibre;
                    delete pNSTlibre;
                    return false ;
                }
            }
            else
                break ;
        }
        while (1);

        delete pNSTlibre;
    }

    string codeTL;
    bool bTrouve = false;

	char PremOrdre[TLIBRE_CODE_LEN + 1], DernOrdre[TLIBRE_CODE_LEN +1 ];
	//    // Préparation des compteurs mini et maxi
	//

    int i;
    PremOrdre[0] = '_';
    DernOrdre[0] = '_';

	for (i = 1; i < TLIBRE_CODE_LEN; i++)	{
		PremOrdre[i] = '0';
		DernOrdre[i] = 'z';
	}

	PremOrdre[i] 	= '\0';	DernOrdre[i] 	= '\0';

	//	// On tente de se positionner sur le dernier code possible : "zzz"
	//
    codeTL = sNss + string(DernOrdre);

	pNSTexteLibre->lastError = pNSTexteLibre->chercheClef(&codeTL,                                                  "",
                                                  0,
                                                  keySEARCHGEQ,
                                                  dbiWRITELOCK);

	//	// Si on se trouve positionné en fin de fichier on recule
	//

	if (pNSTexteLibre->lastError == DBIERR_EOF)		pNSTexteLibre->lastError = pNSTexteLibre->precedent(dbiWRITELOCK);
	//
	// Toute autre erreur est anormale
	//
	else if (pNSTexteLibre->lastError != DBIERR_NONE)
	{
		erreur("Le fichier des textes libres est endommagé.", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
		delete pNSTexteLibre;
		return false;
	}

	//
	// On récupère l'enregistrement
	//

	pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
	//	// Si on est en fin ou début de fichier, c'est qu'il est vide
	//

	if ((pNSTexteLibre->lastError == DBIERR_BOF) ||		 (pNSTexteLibre->lastError == DBIERR_EOF) ||
		 (pNSTexteLibre->lastError == DBIERR_NOCURRREC))
	{
		*pCode = string(PremOrdre);
		bTrouve = true;
	}

    if (!bTrouve)
    {
	    //	    // Toute autre erreur est anormale
	    //

	    if (pNSTexteLibre->lastError != DBIERR_NONE)	    {
		    erreur("Le fichier des textes libres est endommagé.", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
		    delete pNSTexteLibre;
		    return false;
        }

	    //	    // Si le TL trouvé n'appartient pas au patient, on recule d'un cran
	    //

	    if (strcmp(pNSTexteLibre->pDonnees->patient, sNss.c_str()) != 0)	    {
		    pNSTexteLibre->lastError = pNSTexteLibre->precedent(dbiWRITELOCK);

		    //		    // Si on est en début de fichier, c'est qu'il n'y a pas de TL pour ce patient
		    //

		    if (pNSTexteLibre->lastError == DBIERR_BOF)		    {
			    *pCode = string(PremOrdre);
			    bTrouve = true;
		    }

            if (!bTrouve)
            {
		        //		        // Toute autre erreur est anormale
		        //

		        if (pNSTexteLibre->lastError == DBIERR_NONE)			        pNSTexteLibre->lastError = pNSTexteLibre->getRecord();

		        if (pNSTexteLibre->lastError != DBIERR_NONE)		        {
			        erreur("Le fichier des textes libres est endommagé.", 0, pNSTexteLibre->lastError);
                    pNSTexteLibre->close();
			        delete pNSTexteLibre;
			        return false;
		        }

		        //		        // Si le TL trouvé n'appartient pas au patient, il n'y a pas
		        // encore de TL pour ce patient
		        //

		        if (strcmp(pNSTexteLibre->pDonnees->patient,								sNss.c_str()) != 0)
		        {
			        *pCode = string(PremOrdre);
		        }
                else
                {
                    *pCode = string(pNSTexteLibre->pDonnees->code);
                }
	        }
        }
        else
        {
            *pCode = string(pNSTexteLibre->pDonnees->code);
        }
    }

	//	// On est positionné sur le dernier TL du patient, que l'on doit
	// incrémenter pour avoir le nouveau code
	//
    if (!IncrementeCode(pCode))
    {
        erreur("Erreur à l'incrémentation du code textelibre", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
        delete pNSTexteLibre;
        pNSTexteLibre = 0;
        return false;
    }

    code = *pCode;
#endif
    //    // Décomposition de sTexte en une liste chainée d'enregistrements de blocs
    // de TLIBRE_TEXTE_LEN caractères
    //

    size_t  bloc, j, h ;    int     k = 1 ;
    bool continuer = true;
    if (strlen(sTexte.c_str()) <= TLIBRE_TEXTE_LEN /*40*/)    {
   	    bloc = strlen(sTexte.c_str());
        suivant = string(TLIBRE_CODE_LEN, '0'); // pas de suivant
    }
    else
    {
   	    bloc = TLIBRE_TEXTE_LEN;
        suivant = *pCode;
        code    = *pCode;
        IncrementeCode(&suivant);
    }

    while (continuer)    {
        pNSTexteLibre->metABlanc();

#ifdef _MUE
        strcpy(pNSTexteLibre->pDonnees->patient, sNss.c_str());
#endif
        strcpy(pNSTexteLibre->pDonnees->code, code.c_str());
        strcpy(pNSTexteLibre->pDonnees->suivant, suivant.c_str());

        int t = (k - 1) * TLIBRE_TEXTE_LEN;
        for (j = t ; j < bloc ; j++)            pNSTexteLibre->pDonnees->texte[j - t] = sTexte[j];

        pNSTexteLibre->lastError = pNSTexteLibre->appendRecord();
        if (pNSTexteLibre->lastError != DBIERR_NONE)        {
      	    erreur("5.Erreur à l'ajout de la fiche textelibre", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
            pNSTexteLibre = 0;
            return false;
        }

        h = bloc;
        k++;   //nombre de blocs
        bloc = k * TLIBRE_TEXTE_LEN;
        if (!IncrementeCode(&code))        {            erreur("Erreur à l'incrémentation du code textelibre", 0, pNSTexteLibre->lastError);            pNSTexteLibre->close();
            delete pNSTexteLibre;
            pNSTexteLibre = 0;
            return false;        }        suivant = code;

        if (!IncrementeCode(&suivant))
        {
            erreur("Erreur à l'incrémentation du code suivant textelibre", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
            pNSTexteLibre = 0;
            return false;
        }

        if (bloc == strlen(sTexte.c_str()))        {
      	    suivant = string(TLIBRE_CODE_LEN, '0'); // pas de suivant
            continuer = false;
        }

        if (bloc > strlen(sTexte.c_str()))      	    continuer = false;
    }

    // si le reste de sTexte est inférieur à 40 caractéres
    if (h < strlen(sTexte.c_str()))    {
        suivant = string(TLIBRE_CODE_LEN, '0'); // pas de suivant
        pNSTexteLibre->metABlanc();

#ifdef _MUE
        strcpy(pNSTexteLibre->pDonnees->patient, sNss.c_str());
#endif
        strcpy(pNSTexteLibre->pDonnees->code,  code.c_str());
        strcpy(pNSTexteLibre->pDonnees->suivant, suivant.c_str() );

        for (j = h; j <strlen(sTexte.c_str()); j++)            pNSTexteLibre->pDonnees->texte[j - h] = sTexte[j];

        pNSTexteLibre->lastError = pNSTexteLibre->appendRecord();
        if (pNSTexteLibre->lastError != DBIERR_NONE)        {
      	    erreur("6.Erreur à l'ajout de la fiche textelibre", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
            pNSTexteLibre = 0;
            return false;
        }
    }

    pNSTexteLibre->close();    delete pNSTexteLibre;
    pNSTexteLibre = 0;
    return true;
}
catch (...)
{
    erreur("Exception (insert TL)", 0, 0);
    return false;
}
}


//----------------------------------------------------------------------------//  Mettre le contenu de la fiche de code sCode dans pTexte
//	le code doit être différent de "00000"
//----------------------------------------------------------------------------

boolNSTlibreBase::RecupereTexte(string sCodePat, string sCode, string* pTexte)
{
try
{
#ifdef _MUE
    // Pour éviter de stocker des textes libres vides
    // on considère par convention qu'un texte libre vide
    // est associé à un code vide.
    if (sCode == "")
    {
        *pTexte = "";
        return true;
    }
#endif

    if (!(creerCurseur()))
        return false;

    pNSTexteLibre->lastError = pNSTexteLibre->open();
    if (pNSTexteLibre->lastError != DBIERR_NONE)	{
        erreur("7.Erreur à l'ouverture de la base des textes libres.", 0, pNSTexteLibre->lastError);
        delete pNSTexteLibre;
		return false;
    }

#ifndef _MUE
    string codeTL = sCode;
#else
    string codeTL = sCodePat + sCode;
#endif

	pNSTexteLibre->lastError = pNSTexteLibre->chercheClef(&codeTL,                                                          "",
                                                          0,
                                                          keySEARCHEQ,
                                                          dbiWRITELOCK);

    if (pNSTexteLibre->lastError != DBIERR_NONE)    {
    	if (pContexte->isVerboseErrorMessages())
   	    	erreur("8.Erreur à recherche des textes libres.", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
        delete pNSTexteLibre;
   	    return false;
    }

    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
    if (pNSTexteLibre->lastError != DBIERR_NONE)    {
        erreur("9.Le fichier de la base des textes libres est inutilisable.", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
        delete pNSTexteLibre;
        return false;
    }

	(*pTexte) = (string(pNSTexteLibre->pDonnees->texte).c_str());    (*pTexte) += string(TLIBRE_TEXTE_LEN - strlen(pNSTexteLibre->pDonnees->texte), ' ');

    while (strcmp(pNSTexteLibre->pDonnees->suivant, string(TLIBRE_CODE_LEN, '0').c_str()))    {
#ifndef _MUE
        codeTL = string(pNSTexteLibre->pDonnees->suivant);
#else
        codeTL = sCodePat + string(pNSTexteLibre->pDonnees->suivant);
#endif

   	    pNSTexteLibre->lastError = pNSTexteLibre->chercheClef(&codeTL,
                                                              "",
                                                              0,
                                                              keySEARCHEQ,
                                                              dbiWRITELOCK);

        if (pNSTexteLibre->lastError != DBIERR_NONE)        {
        	if (pContexte->isVerboseErrorMessages())
      	    	erreur("10.Erreur à recherche clé des textes libres.", 0, pNSTexteLibre->lastError);
            delete pNSTexteLibre;
   		    return false;
        }

   	    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
   	    if (pNSTexteLibre->lastError != DBIERR_NONE)   	    {
   		    erreur("11.Le fichier de des textes libres est inutilisable.", 0, pNSTexteLibre->lastError);
            pNSTexteLibre->close();
            delete pNSTexteLibre;
      	    return false;
   	    }

   	    (*pTexte) += (string(pNSTexteLibre->pDonnees->texte).c_str());        (*pTexte) += string(TLIBRE_TEXTE_LEN - strlen(pNSTexteLibre->pDonnees->texte), ' ');
    }

    int Compteur = strlen(pTexte->c_str()) - 1;
    while (((*pTexte)[Compteur] == ' ') && (Compteur > 0))   	    Compteur--;

    *pTexte = string(*pTexte, 0, Compteur + 1);
    pNSTexteLibre->close();    delete pNSTexteLibre;
    return true;
}
catch (...)
{
    erreur("Exception (recup TL)", 0, 0);
    return false;
}
}

//---------------------------------------------------------------------------------

//  Mettre à jour le texte de la fiche (sCode)
// 				   avec le texte sTexte. Calculer le nouveau premier code
//---------------------------------------------------------------------------------
bool
NSTlibreBase::MettreFicheAJour(string sCode, string sNouveauTexte)
{
try
{
    string* pNewTexte = new string(sNouveauTexte);

    if (!(creerCurseur()))
        return false;

    pNSTexteLibre->lastError = pNSTexteLibre->open();
    if (pNSTexteLibre->lastError != DBIERR_NONE)
	{
        erreur("12.Erreur à l'ouverture de la base des textes libres.", 0, pNSTexteLibre->lastError);
        pNSTexteLibre->close();
        delete pNSTexteLibre;
		return false;
    }
    bool resultat = ChangerFiche(pNSTexteLibre, sCode, pNewTexte) ;
    if (!resultat)
        erreur("13.Erreur dans le changement de la fiche.", 0, 0);

    delete pNewTexte;

    pNSTexteLibre->close();
    delete pNSTexteLibre;
    return true;
}
catch (...)
{
    erreur("Exception (maj TL)", 0, 0);
    return false;
}
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool
NSTlibreBase::ChangerFiche(NSTextelibreBase* pNSTexteLibre, string sCode, string* pNewTexte)
{
try
{
    pNSTexteLibre->lastError = pNSTexteLibre->chercheClef(&sCode,
                                                          "",
                                                          0,
                                                          keySEARCHEQ,
                                                          dbiWRITELOCK);
    if (pNSTexteLibre->lastError != DBIERR_NONE)
    {
   	    erreur("14.Erreur à recherche clé des textes libres.", 0, pNSTexteLibre->lastError);
   	    return false;
    }

    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
    if (pNSTexteLibre->lastError != DBIERR_NONE)
    {
   	    erreur("15.Le fichier de la base des textes libres est inutilisable.", 0, pNSTexteLibre->lastError);
        return false;
    }
    if (strlen(pNewTexte->c_str()) > TLIBRE_TEXTE_LEN)
    {
   	    for (int i = 0 ; i < TLIBRE_TEXTE_LEN; i++)
   		    pNSTexteLibre->pDonnees->texte[i] = (*pNewTexte)[i];
   	    *pNewTexte = string(*pNewTexte, TLIBRE_TEXTE_LEN, strlen(pNewTexte->c_str()) - TLIBRE_TEXTE_LEN);
    }
    else
    {
        size_t i;
  		for (i = 0 ; i < strlen(pNewTexte->c_str()); i++)
   		    pNSTexteLibre->pDonnees->texte[i] = (*pNewTexte)[i];

   	    for (; i < TLIBRE_TEXTE_LEN; i++)
            pNSTexteLibre->pDonnees->texte[i] = ' ';

   	    *pNewTexte = "";
    }
    pNSTexteLibre->lastError = pNSTexteLibre->modifyRecord(TRUE);
    if (pNSTexteLibre->lastError != DBIERR_NONE)
    {
        erreur("16.Erreur Modification des textes libres.", 0, pNSTexteLibre->lastError);
        return false;
    }
    if (!(*pNewTexte == ""))
    {
        if (string(pNSTexteLibre->pDonnees->suivant) == string(TLIBRE_CODE_LEN, '0'))
 	 	{
			pNSTexteLibre->creerBookMarks(1);
			pNSTexteLibre->lastError = pNSTexteLibre->placeBookMark(1);
      	    if (CreerNouvelleFiche(pNSTexteLibre))
      	    {
      		    string sNewCode = pNSTexteLibre->pDonnees->code;
         	    pNSTexteLibre->lastError = pNSTexteLibre->retrouveBookMark(1);
                if (pNSTexteLibre->lastError != DBIERR_NONE)
   			    {
   				    erreur("17.Erreur BBoOkMark des textes libres.", 0, pNSTexteLibre->lastError);
   				    return false;
   			    }

                pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
   			    if (pNSTexteLibre->lastError != DBIERR_NONE)
   			    {
   				    erreur("171.Le fichier de des textes libres est inutilisable.", 0, pNSTexteLibre->lastError);
      			    return false;
   			    }

         	    // si ok
          	    strcpy(pNSTexteLibre->pDonnees->suivant, sNewCode.c_str());
        	 	pNSTexteLibre->lastError = pNSTexteLibre->modifyRecord(TRUE);
                if (pNSTexteLibre->lastError != DBIERR_NONE)
   			    {
   				    erreur("18.Erreur Modification des textes libres.", 0, pNSTexteLibre->lastError);
   				    return false;
   			    }
      	    }
            pNSTexteLibre->detruireBookMarks(1);
        }
        bool changerfiche = ChangerFiche(pNSTexteLibre, pNSTexteLibre->pDonnees->suivant, pNewTexte);
        if (changerfiche == false)
        {
            erreur("181.Erreur chagement fiche  des textes libres.", 0, pNSTexteLibre->lastError);
            return false;
        }
    }
    else
    {
        string suivant = pNSTexteLibre->pDonnees->suivant;
        if (!(suivant == string(TLIBRE_CODE_LEN, '0')))
        {
            strcpy(pNSTexteLibre->pDonnees->suivant, string(TLIBRE_CODE_LEN, '0').c_str());
            pNSTexteLibre->lastError = pNSTexteLibre->modifyRecord(TRUE);

            if (pNSTexteLibre->lastError != DBIERR_NONE)
            {
      	        erreur("19.Erreur Modification des textes libres.", 0, pNSTexteLibre->lastError);
                return false;
            }
    	    bool detruire = DetruireFiche(pNSTexteLibre, suivant);
            if (detruire == false)
            {
      	        erreur("191.Erreur Destruction fiche des textes libres.", 0, pNSTexteLibre->lastError);
                return false;
            }
        }
    }
    return true;

}
catch (...)
{
    erreur("Exception (change TL)", 0, 0);
    return false;
}
}

//----------------------------------------------------------------//----------------------------------------------------------------
bool
NSTlibreBase::CreerNouvelleFiche(NSTextelibreBase* pNSTexteLibre)
{
try
{
    string sNewCode;
 	pNSTexteLibre->lastError = pNSTexteLibre->fin(dbiWRITELOCK);
    //
	// Si la table est vide
	//
	if ((pNSTexteLibre->lastError == DBIERR_BOF) ||
        (pNSTexteLibre->lastError == DBIERR_EOF) ||
        (pNSTexteLibre->lastError == DBIERR_NOCURRREC))
    {
   	    sNewCode = string(TLIBRE_CODE_LEN, '0');
        IncrementeCode(&sNewCode);
    }
    else
    {
   	    if (pNSTexteLibre->lastError != DBIERR_NONE)
		{
			erreur("20.Erreur d'accés à la fin de la base des textes libres.", 0, pNSTexteLibre->lastError);
			return false;
		}
   	    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
   	    if (pNSTexteLibre->lastError != DBIERR_NONE)
   	    {
   		    erreur("21.Le fichier de la base des textes libres est inutilisable.", 0, pNSTexteLibre->lastError);
      	    return false;
   	    }

        sNewCode = string(pNSTexteLibre->pDonnees->code) ;
        IncrementeCode(&sNewCode);
    }

    pNSTexteLibre->metABlanc();
    strcpy(pNSTexteLibre->pDonnees->suivant, string(TLIBRE_CODE_LEN, '0').c_str());
    strcpy(pNSTexteLibre->pDonnees->code, sNewCode.c_str());

    pNSTexteLibre->lastError = pNSTexteLibre->appendRecord();
    if (pNSTexteLibre->lastError != DBIERR_NONE)
    {
        erreur("22.Erreur à l'ajout de la fiche textelibre", 0, pNSTexteLibre->lastError);
        return false;
    }

    return true;}
catch (...)
{
    erreur("Exception (NSTlibreBase::CreerNouvelleFiche)", 0, 0);
    return false;
}
}

//-----------------------------------------------------------------------------------//  Détruit l'ensemble des fiches d'une liste chainée de blocs de texte
//-----------------------------------------------------------------------------------
bool
NSTlibreBase::DetruireFiche(NSTextelibreBase* pNSTexteLibre, string sCode)
{
try
{
    pNSTexteLibre->lastError = pNSTexteLibre->chercheClef(&sCode,
                                                          "",
                                                          0,
                                                          keySEARCHEQ,
                                                          dbiWRITELOCK);
    if (pNSTexteLibre->lastError != DBIERR_NONE)
    {
   	    erreur("23.Erreur à recherche clé des textes libres.", 0, pNSTexteLibre->lastError);
   	    return false;
    }

    pNSTexteLibre->lastError = pNSTexteLibre->getRecord();
    if (pNSTexteLibre->lastError != DBIERR_NONE)
    {
   	    erreur("24.Le fichier de la base des textes libres est inutilisable.", 0, pNSTexteLibre->lastError);
        return false;
    }
    string suivant = string(pNSTexteLibre->pDonnees->suivant);
    pNSTexteLibre->deleteRecord();

    if (!(suivant == string(TLIBRE_CODE_LEN, '0')))        DetruireFiche(pNSTexteLibre, suivant);
    return true;}
catch (...)
{    erreur("Exception (detruire TL)", 0, 0);
    return false;
}
}

boolNSTlibreBase::creerCurseur()
{
    return false;
}

//***************************************************************************
// 				  Implémentation des mèthodes NSTextelibre
//***************************************************************************

NSTextelibre::NSTextelibre(NSContexte* pCtx)
             :NSTextelibreBase(pCtx)
{}

NSTextelibre::~NSTextelibre()
{
}

DBIResultNSTextelibre::open()
{
#ifndef _MUE
	char tableName[] = "TLIBRE.DB";#else    char tableName[] = "NSMTLIBRE.DB";#endif
	//	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//***************************************************************************
// 				  Implémentation des mèthodes NSTlibre
//***************************************************************************

bool
NSTlibre::creerCurseur()
{
try
{
    pNSTexteLibre = new NSTextelibre(pContexte);
    return true;
}
catch (...)
{
    erreur("Exception NSTlibre::creerCurseur", 0, 0);
    return false;
}
}


NSTlibre::NSTlibre(NSContexte* pCtx)
         :NSTlibreBase(pCtx)
{
}

NSTlibre::NSTlibre(NSTlibre& rv)
         :NSTlibreBase(rv.pContexte)
{}

NSTlibre::~NSTlibre()
{}

NSTlibre&
NSTlibre::operator=(NSTlibre rv)
{
    return *this;
}

int
NSTlibre::operator == (const NSTlibre& o )
{
    return 1;
}

//***************************************************************************
// 				  Implémentation des mèthodes NSTextelibreFileName
//***************************************************************************

NSTextelibreFileName::NSTextelibreFileName(NSContexte* pCtx)
                     :NSTextelibreBase(pCtx)
{
}

NSTextelibreFileName::~NSTextelibreFileName()
{
}

DBIResult
NSTextelibreFileName::open()
{
#ifndef _MUE	char tableName[] = "TLIBRE.DB";
#else
    char tableName[] = "NSMTLIBRE.DB";
#endif

	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//***************************************************************************
// 				  Implémentation des mèthodes NSTlibreFileName
//***************************************************************************

bool
NSTlibreFileName::creerCurseur()
{
try
{
    pNSTexteLibre = new NSTextelibreFileName(pContexte);
    return true;
}
catch (...)
{
    erreur("Exception (curseur TL)", 0, 0);
    return false;
}
}

NSTlibreFileName::NSTlibreFileName(NSContexte* pCtx)
                 :NSTlibreBase(pCtx)
{
}

NSTlibreFileName::NSTlibreFileName(NSTlibreFileName& rv)
                 :NSTlibreBase(rv.pContexte)
{}

NSTlibreFileName::~NSTlibreFileName()
{}

NSTlibreFileName&
NSTlibreFileName::operator=(NSTlibreFileName rv)
{
    return *this;
}

int
NSTlibreFileName::operator == (const NSTlibreFileName& o )
{
    return 1;
}

