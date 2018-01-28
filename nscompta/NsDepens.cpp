//---------------------------------------------------------------------------//    NSPERSON.CPP
//    KRZISCH PH.   janvier 92
//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

//using namespace std;

#include "nscompta\nsfsedlg.h"

#include "nsoutil\nsoutdlg.h"

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nscompta\nscompta.h"
#include "nscompta\nscompta.rh"
#include "nscompta\nscpta.h"
#include "nscompta\nsdepens.h"
#include "nscompta\nsdepens.rh"
#include "nsbb\nsednum.h"
#include "nautilus\nsadmiwd.h"
//***************************************************************************// Implémentation des méthodes NSEcriture
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSEcritureData::NSEcritureData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSEcritureData::NSEcritureData(const NSEcritureData& rv)
{
	strcpy(numero,  rv.numero);
	strcpy(ligne,   rv.ligne);
	strcpy(libelle, rv.libelle);
	strcpy(compte,  rv.compte);
	strcpy(sens,    rv.sens);
	strcpy(signe,   rv.signe);
	strcpy(somme,   rv.somme);
	strcpy(etat,    rv.etat);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSEcritureData::~NSEcritureData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSEcritureData&
NSEcritureData::operator=(const NSEcritureData& src)
{
	if (this == &src)
		return *this ;

	strcpy(numero,  src.numero) ;
	strcpy(ligne,   src.ligne) ;
	strcpy(libelle, src.libelle) ;
	strcpy(compte,  src.compte) ;
	strcpy(sens,    src.sens) ;
	strcpy(signe,   src.signe) ;
	strcpy(somme,   src.somme) ;
	strcpy(etat,    src.etat) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSEcritureData::operator == ( const NSEcritureData& o )
{
	if ((strcmp(numero, o.numero) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSEcritureData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numero,  0, ECRI_ECRITURE_LEN + 1);
	memset(ligne,   0, ECRI_LIGNE_LEN + 1);
	memset(libelle, 0, ECRI_LIBELLE_LEN + 1);
	memset(compte,  0, ECRI_COMPTE_LEN + 1);	memset(sens,    0, ECRI_SENS_LEN + 1);
	memset(signe,   0, ECRI_SIGNE_LEN + 1);
	memset(somme,   0, ECRI_SOMME_LEN + 1);
	memset(etat,    0, ECRI_ETAT_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSEcriture::NSEcriture(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSEcritureData() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSEcriture::NSEcriture(const NSEcriture& rv)
           :NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSEcritureData() ;
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSEcriture::~NSEcriture()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSEcriture::alimenteFiche()
{
   alimenteChamp(pDonnees->numero,  ECRI_ECRITURE_FIELD,    ECRI_ECRITURE_LEN);
   alimenteChamp(pDonnees->ligne,   ECRI_LIGNE_FIELD,       ECRI_LIGNE_LEN);
   alimenteChamp(pDonnees->libelle, ECRI_LIBELLE_FIELD,     ECRI_LIBELLE_LEN);
   alimenteChamp(pDonnees->compte,  ECRI_COMPTE_FIELD,      ECRI_COMPTE_LEN);
   alimenteChamp(pDonnees->sens,    ECRI_SENS_FIELD,        ECRI_SENS_LEN);
   alimenteChamp(pDonnees->signe,   ECRI_SIGNE_FIELD,       ECRI_SIGNE_LEN);
   alimenteChamp(pDonnees->somme,   ECRI_SOMME_FIELD,       ECRI_SOMME_LEN);
   alimenteChamp(pDonnees->etat,    ECRI_ETAT_FIELD,        ECRI_ETAT_LEN);
}

//---------------------------------------------------------------------------
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSEcriture::videFiche()
{
   videChamp(pDonnees->numero,  ECRI_ECRITURE_FIELD,    ECRI_ECRITURE_LEN);
   videChamp(pDonnees->ligne,   ECRI_LIGNE_FIELD,       ECRI_LIGNE_LEN);
   videChamp(pDonnees->libelle, ECRI_LIBELLE_FIELD,     ECRI_LIBELLE_LEN);
   videChamp(pDonnees->compte,  ECRI_COMPTE_FIELD,      ECRI_COMPTE_LEN);
   videChamp(pDonnees->sens,    ECRI_SENS_FIELD,        ECRI_SENS_LEN);
   videChamp(pDonnees->signe,   ECRI_SIGNE_FIELD,       ECRI_SIGNE_LEN);
   videChamp(pDonnees->somme,   ECRI_SOMME_FIELD,       ECRI_SOMME_LEN);
   videChamp(pDonnees->etat,    ECRI_ETAT_FIELD,        ECRI_ETAT_LEN);
}

//---------------------------------------------------------------------------
//  Fonction :  	NSDepens::open()
////  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSEcriture::open()
{
    char tableName[] = "ECRITURE.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);

	return(lastError);
}

//---------------------------------------------------------------------------//  Function:  NSDepens::Create()
//---------------------------------------------------------------------------
bool
NSEcriture::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSDepens::Modify()
//---------------------------------------------------------------------------
bool
NSEcriture::Modify()
{
	return true;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSEcriture&
NSEcriture::operator=(const NSEcriture& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSEcriture::operator == (const NSEcriture& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------//  Constructeur par défaut
//---------------------------------------------------------------------------
NSEcritureInfo::NSEcritureInfo()
{
	// Crée l'objet de données
	pDonnees = new NSEcritureData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSEcriture
//---------------------------------------------------------------------------
NSEcritureInfo::NSEcritureInfo(const NSEcriture* pFiche)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSEcritureData() ;
	//
	// Copie les valeurs du NSEcriture
	//
  if (pFiche)
	  *pDonnees = *(pFiche->pDonnees) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSEcritureInfo::NSEcritureInfo(const NSEcritureInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSEcritureData() ;
	//
	// Copie les valeurs du NSEcritureInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSEcritureInfo::~NSEcritureInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSEcritureInfo&
NSEcritureInfo::operator=(const NSEcritureInfo& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSEcritureInfo::operator==(const NSEcritureInfo& o)
{
  return (*pDonnees == *(o.pDonnees)) ;
}

//***************************************************************************
// 					Implémentation des méthodes NSDepensArray
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSEcritureArray::NSEcritureArray(const NSEcritureArray& rv)
                :NSFicheEcritureArray()
{
  if (false == rv.empty())
    for (NSEcritureConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSEcritureInfo(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSEcritureArray::vider()
{
  if (empty())
    return ;

  for (NSEcritureIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSEcritureArray::~NSEcritureArray(){
	vider() ;
}

//***************************************************************************
// Implémentation des méthodes NSDepens
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSDepensData::NSDepensData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDepensData::NSDepensData(const NSDepensData& rv)
{
	strcpy(numdeps,   rv.numdeps) ;
	strcpy(date,      rv.date) ;
	strcpy(operateur, rv.operateur) ;
	strcpy(libelle,   rv.libelle) ;
	strcpy(mode_paie, rv.mode_paie) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSDepensData::~NSDepensData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSDepensData&
NSDepensData::operator=(const NSDepensData& src)
{
  if (this == &src)
		return *this ;

  strcpy(numdeps,   src.numdeps) ;
	strcpy(date,      src.date) ;
  strcpy(operateur, src.operateur) ;
	strcpy(libelle,   src.libelle) ;
	strcpy(mode_paie, src.mode_paie) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSDepensData::operator==(const NSDepensData& o)
{
	if ((strcmp(numdeps, o.numdeps) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSDepensData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
  memset(numdeps,   0, DEPS_NUMDEPS_LEN + 1) ;
	memset(date,      0, DEPS_DATE_LEN + 1) ;
  memset(operateur, 0, DEPS_OPERATEUR_LEN + 1) ;
  memset(libelle,   0, DEPS_LIBELLE_LEN + 1) ;
	memset(mode_paie, 0, DEPS_MODE_PAIE_LEN + 1) ;
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSDepens::NSDepens(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSDepensData() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSDepens::NSDepens(const NSDepens& rv) : NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSDepensData() ;
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur//---------------------------------------------------------------------------
NSDepens::~NSDepens()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSDepens::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSDepens::alimenteFiche()
{
  alimenteChamp(pDonnees->numdeps,   DEPS_NUMDEPS_FIELD,   DEPS_NUMDEPS_LEN) ;
  alimenteChamp(pDonnees->date, 	   DEPS_DATE_FIELD,      DEPS_DATE_LEN) ;
  alimenteChamp(pDonnees->operateur, DEPS_OPERATEUR_FIELD, DEPS_OPERATEUR_LEN) ;
  alimenteChamp(pDonnees->libelle,   DEPS_LIBELLE_FIELD,   DEPS_LIBELLE_LEN) ;
  alimenteChamp(pDonnees->mode_paie, DEPS_MODE_PAIE_FIELD, DEPS_MODE_PAIE_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :   	NSDepens::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSDepens::videFiche()
{
  videChamp(pDonnees->numdeps,   DEPS_NUMDEPS_FIELD,   DEPS_NUMDEPS_LEN) ;
  videChamp(pDonnees->date, 	   DEPS_DATE_FIELD,      DEPS_DATE_LEN) ;
  videChamp(pDonnees->operateur, DEPS_OPERATEUR_FIELD, DEPS_OPERATEUR_LEN) ;
  videChamp(pDonnees->libelle,   DEPS_LIBELLE_FIELD,   DEPS_LIBELLE_LEN) ;
  videChamp(pDonnees->mode_paie, DEPS_MODE_PAIE_FIELD, DEPS_MODE_PAIE_LEN) ;
}

//---------------------------------------------------------------------------//  Ouvre la table primaire et les index secondaires
//---------------------------------------------------------------------------
DBIResult
NSDepens::open()
{
	char tableName[] = "NSM_DEPENSES.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;

	return(lastError) ;
}

//---------------------------------------------------------------------------//  Function:  NSDepens::Create()
//---------------------------------------------------------------------------
bool
NSDepens::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSDepens::Modify()
//---------------------------------------------------------------------------
bool
NSDepens::Modify()
{
	return true;
}

boolNSDepens::NewNum(bool bCritique)
{
    NSDepensData depData = NSDepensData(*pDonnees);

   	string 	sNumero, sNumCompt;
	char 	PremOrdre[DEPS_NUMDEPS_LEN + 1],
		  	DernOrdre[DEPS_NUMDEPS_LEN + 1];
	//
	// Préparation des compteurs mini et maxi
	//
   	int i;
	for (i = 0; i < DEPS_NUMDEPS_LEN; i++)
	{
		PremOrdre[i] = '0';
		DernOrdre[i] = 'z';
	}
	PremOrdre[i] 	= '\0';
	DernOrdre[i] 	= '\0';
	PremOrdre[i-1] = '1';

#ifndef N_TIERS
    if (bCritique)    {
        // Entrée en section critique
        if (!pContexte->DemandeAcces("DEPENSES"))
        {
    	    erreur("Impossible d'obtenir l'accès à la base depenses.",standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
            return false;
        }
    }
#endif
	//
	// On tente de se positionner sur le dernier code possible : "zzzzzzzz"
	//
   	sNumero = string(DernOrdre);

	lastError = chercheClef(&sNumero, "", 0, keySEARCHGEQ, dbiWRITELOCK);

	//
	// Si on se trouve positionné en fin de fichier on recule
	//
	if (lastError == DBIERR_EOF)
    lastError = precedent(dbiWRITELOCK) ;

	//	// Toute autre erreur est anormale
	//
	else if ((lastError != DBIERR_NONE) && (lastError != DBIERR_BOF))
	{
		erreur("Le fichier depenses.db est endommagé.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
#ifndef N_TIERS
    if (bCritique)      pContexte->LibereAcces("DEPENSES");
#endif
		return false ;	}

	//	// On récupère l'enregistrement si fichier non vide
	//
   	if (lastError != DBIERR_BOF)
   	{
		lastError = getRecord();
   		if (lastError != DBIERR_NONE)
		{
			erreur("erreur à la lecture du fichier depenses.db.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
#ifndef N_TIERS
            if (bCritique)                pContexte->LibereAcces("DEPENSES");
#endif
			return false;		}
   	}

	//	// Si on est en fin ou début de fichier, c'est qu'il est vide
	//
	if ((lastError == DBIERR_BOF) ||
		(lastError == DBIERR_EOF) ||
		(lastError == DBIERR_NOCURRREC))
	{
    	sNumCompt = string(PremOrdre);
        *pDonnees = depData;
    	strcpy(pDonnees->numdeps, sNumCompt.c_str());

    	lastError = appendRecord();
   		if (lastError != DBIERR_NONE)
   		{
         			erreur("Erreur à l'ajout d'une fiche Depenses vierge.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
#ifndef N_TIERS
            if (bCritique)                pContexte->LibereAcces("DEPENSES");
#endif
      		return false;   		}
#ifndef N_TIERS
        if (bCritique)            pContexte->LibereAcces("DEPENSES");
#endif
		return true;	}

	//	// Toute autre erreur est anormale
	//
	if (lastError != DBIERR_NONE)
	{
		erreur("Le fichier depenses.db est endommagé.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
#ifndef N_TIERS
        if (bCritique)            pContexte->LibereAcces("DEPENSES");
#endif
		return false;	}

	//	// On est positionné sur le dernier document du patient, que l'on doit
	// incrémenter pour avoir le nouveau code
	//
	char compteur[DEPS_NUMDEPS_LEN + 1];
	strcpy(compteur, pDonnees->numdeps);

	//
	// On incrémente le compteur
	//
	int j;
	int calculer = 1;
	i = strlen(compteur) - 1;

	while (calculer)
   	{
    	j = (int) compteur[i];
		j++;
		if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))
		{
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
			if (i == 0)
			{
				erreur("Compteur de fiches depenses saturé.", standardError, 0, _pSuper->donneMainWindow()->GetHandle());
#ifndef N_TIERS
                if (bCritique)
        		    pContexte->LibereAcces("DEPENSES");
#endif
				return false;			}
			i--;
		}
	}

	sNumCompt = string(compteur);

    // on stocke une fiche vierge pour etre plus sur
    *pDonnees = depData;
    strcpy(pDonnees->numdeps, sNumCompt.c_str());

    lastError = appendRecord();
   	if (lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'ajout d'une fiche Depenses vierge.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
#ifndef N_TIERS
        if (bCritique)            pContexte->LibereAcces("DEPENSES");
#endif
      	return false;   	}

    // Sortie de section critique
#ifndef N_TIERS
    if (bCritique)
        pContexte->LibereAcces("DEPENSES");
#endif
	return true;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSDepens&
NSDepens::operator=(const NSDepens& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;}
//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSDepens::operator==(const NSDepens& o)
{
	 return (*pDonnees == *(o.pDonnees)) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSDepensInfo::NSDepensInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSDepensInfo::NSDepensInfo()
{
	// Crée l'objet de données
	pDonnees = new NSDepensData() ;
}

//---------------------------------------------------------------------------//  Fonction:		NSDepensInfo::NSDepensInfo(NSCompt*)
//
//  Description:	Constructeur à partir d'un NSTransaction
//---------------------------------------------------------------------------
NSDepensInfo::NSDepensInfo(const NSDepens* pTransac)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSDepensData() ;
	//
	// Copie les valeurs du NSCompt
	//
  if (pTransac)
	  *pDonnees = *(pTransac->pDonnees) ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSDepensInfo::NSDepensInfo(const NSDepensInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSDepensData() ;
	//
	// Copie les valeurs du NSComptInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSDepensInfo::~NSDepensInfo()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSDepensInfo&
NSDepensInfo::operator=(const NSDepensInfo& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSDepensInfo::operator==(const NSDepensInfo& o)
{
	 return (*pDonnees == *(o.pDonnees)) ;
}

//***************************************************************************
// 					Implémentation des méthodes NSDepensArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDepensArray::NSDepensArray(const NSDepensArray& rv)
              :NSFicheDepensArray()
{
  if (false == rv.empty())
    for (NSDepenstConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSDepensInfo(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSDepensArray::vider()
{
  if (empty())
    return;

  for (NSDepenstIter i = begin() ; end() != i ; )
  {
    delete *i;
    erase(i);
  }
}

NSDepensArray::~NSDepensArray()
{
	vider() ;
}

//***************************************************************************// Implémentation des méthodes NS2035Libs
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NS2035LibsData::NS2035LibsData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NS2035LibsData::NS2035LibsData(const NS2035LibsData& rv)
{
	strcpy(ligne, 	  rv.ligne) ;
	strcpy(libelle,   rv.libelle) ;
	strcpy(categorie, rv.categorie) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NS2035LibsData::~NS2035LibsData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NS2035LibsData&
NS2035LibsData::operator=(const NS2035LibsData& src)
{
  if (this == &src)
		return *this ;

	strcpy(ligne, 	  src.ligne) ;
	strcpy(libelle,   src.libelle) ;
	strcpy(categorie, src.categorie) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NS2035LibsData::operator==(const NS2035LibsData& o)
{
	if ((strcmp(ligne, o.ligne) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NS2035LibsData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(ligne, 	  0, LIBS_LIGNE_LEN + 1) ;
	memset(libelle,   0, LIBS_LIBELLE_LEN + 1) ;
	memset(categorie, 0, LIBS_CATEGORIE_LEN+ 1) ;
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NS2035Libs::NS2035Libs(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NS2035LibsData() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NS2035Libs::NS2035Libs(const NS2035Libs& rv) : NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NS2035LibsData() ;
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NS2035Libs::~NS2035Libs()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NS2035Libs::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NS2035Libs::alimenteFiche()
{
  alimenteChamp(pDonnees->ligne, 	   LIBS_LIGNE_FIELD,     LIBS_LIGNE_LEN) ;
  alimenteChamp(pDonnees->libelle, 	 LIBS_LIBELLE_FIELD, 	 LIBS_LIBELLE_LEN) ;
  alimenteChamp(pDonnees->categorie, LIBS_CATEGORIE_FIELD, LIBS_CATEGORIE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :   	NS2035Libs::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NS2035Libs::videFiche()
{
  videChamp(pDonnees->ligne,     LIBS_LIGNE_FIELD,     LIBS_LIGNE_LEN) ;
  videChamp(pDonnees->libelle,   LIBS_LIBELLE_FIELD,   LIBS_LIBELLE_LEN) ;
  videChamp(pDonnees->categorie, LIBS_CATEGORIE_FIELD, LIBS_CATEGORIE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :  	NS2035Libs::getRecord()
//
//  Description : Prend l'enregistrement en cours et assigne la valeur des
//             	champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NS2035Libs::getPatRecord()
{
	//
	// La table est-elle ouverte ?
	//
	if (false == _isOpen)
	  return(lastError = ERROR_TABLENOTOPEN);
	//
	// Appel de la classe de base pour récupérer l'enregistrement.
	//
	lastError = getDbiRecord(dbiWRITELOCK);

	return(lastError);
}

//---------------------------------------------------------------------------//  Fonction :  	NS2035Libs::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NS2035Libs::open()
{
	char tableName[] = "2035Libs.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);

	return(lastError);}

//---------------------------------------------------------------------------//  Function:  NS2035Libs::Create()
//---------------------------------------------------------------------------
bool
NS2035Libs::Create()
{
	return true;
}

//---------------------------------------------------------------------------//  Function:  NS2035Libs::Modify()
//---------------------------------------------------------------------------
bool
NS2035Libs::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NS2035Libs&
NS2035Libs::operator=(const NS2035Libs& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NS2035Libs::operator==(const NS2035Libs& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------//  Fonction:		NS2035LibsInfo::NS2035LibsInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NS2035LibsInfo::NS2035LibsInfo()
{
	// Crée l'objet de données
	pDonnees = new NS2035LibsData() ;
}

//---------------------------------------------------------------------------
//  Fonction:		NS2035LibsInfo::NS2035LibsInfo(NSCompt*)
//
//  Description:	Constructeur à partir d'un NSTransaction
//---------------------------------------------------------------------------
NS2035LibsInfo::NS2035LibsInfo(const NS2035Libs* pTransac)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NS2035LibsData() ;
	//
	// Copie les valeurs du NSCompt
	//
  if (pTransac)
	  *pDonnees = *(pTransac->pDonnees) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NS2035LibsInfo::NS2035LibsInfo(const NS2035LibsInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NS2035LibsData() ;
	//
	// Copie les valeurs du NS d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NS2035LibsInfo::~NS2035LibsInfo()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NS2035LibsInfo&
NS2035LibsInfo::operator=(const NS2035LibsInfo& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NS2035LibsInfo::operator==(const NS2035LibsInfo& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
// 					Implémentation des méthodes NS2035LibsArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NS2035LibsArray::NS2035LibsArray(const NS2035LibsArray& rv)
                :NSFiche2035LibsArray()
{
  if (false == rv.empty())
    for (NS2035LibsConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NS2035LibsInfo(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NS2035LibsArray::vider()
{
  if (empty())
    return ;

  for (NS2035LibsIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NS2035LibsArray::~NS2035LibsArray()
{
	vider() ;
}

//***************************************************************************
// Implémentation des méthodes NScptsLibs
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NScptsLibsData::NScptsLibsData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NScptsLibsData::NScptsLibsData(const NScptsLibsData& rv)
{
  strcpy(code, 	    rv.code) ;
  strcpy(operateur, rv.operateur) ;
	strcpy(libelle,   rv.libelle) ;
  strcpy(ligne,     rv.ligne) ;
  strcpy(actif,     rv.actif) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NScptsLibsData::~NScptsLibsData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NScptsLibsData&
NScptsLibsData::operator=(const NScptsLibsData& src)
{
  if (this == &src)
		return *this ;

  strcpy(code,      src.code) ;  strcpy(operateur, src.operateur) ;
  strcpy(libelle,   src.libelle) ;
  strcpy(ligne,     src.ligne) ;
  strcpy(actif,     src.actif) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NScptsLibsData::operator==(const NScptsLibsData& o)
{
	if ((strcmp(code, o.code) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NScptsLibsData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(code,      0, CPTS_CODE_LEN + 1) ;
  memset(operateur, 0, CPTS_OPERATEUR_LEN + 1) ;
	memset(libelle,   0, CPTS_LIBELLE_LEN + 1) ;
  memset(ligne,     0, CPTS_LIGNE_LEN + 1) ;
  memset(actif,     0, CPTS_ACTIF_LEN + 1) ;
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NScptsLibs::NScptsLibs(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NScptsLibsData() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NScptsLibs::NScptsLibs(const NScptsLibs& rv) : NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NScptsLibsData() ;
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NScptsLibs::~NScptsLibs()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------//  Fonction :  	NS2035Libs::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NScptsLibs::alimenteFiche()
{
  alimenteChamp(pDonnees->code, 	   CPTS_CODE_FIELD,      CPTS_CODE_LEN) ;
  alimenteChamp(pDonnees->operateur, CPTS_OPERATEUR_FIELD, CPTS_OPERATEUR_LEN) ;
  alimenteChamp(pDonnees->libelle,   CPTS_LIBELLE_FIELD,   CPTS_LIBELLE_LEN) ;
  alimenteChamp(pDonnees->ligne, 	   CPTS_LIGNE_FIELD,     CPTS_LIGNE_LEN) ;
  alimenteChamp(pDonnees->actif, 	   CPTS_ACTIF_FIELD,     CPTS_ACTIF_LEN) ;
}

//---------------------------------------------------------------------------//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NScptsLibs::videFiche()
{
  videChamp(pDonnees->code, 	    CPTS_CODE_FIELD,        CPTS_CODE_LEN) ;
  videChamp(pDonnees->operateur,  CPTS_OPERATEUR_FIELD,   CPTS_OPERATEUR_LEN) ;
  videChamp(pDonnees->libelle,    CPTS_LIBELLE_FIELD,     CPTS_LIBELLE_LEN) ;
  videChamp(pDonnees->ligne, 	    CPTS_LIGNE_FIELD,       CPTS_LIGNE_LEN) ;
  videChamp(pDonnees->actif, 	    CPTS_ACTIF_FIELD,       CPTS_ACTIF_LEN) ;
}

//---------------------------------------------------------------------------//  Description : Prend l'enregistrement en cours et assigne la valeur des
//             	champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NScptsLibs::getPatRecord()
{
	//
	// La table est-elle ouverte ?
	//
	if (false == _isOpen)
		return(lastError = ERROR_TABLENOTOPEN) ;
	//
	// Appel de la classe de base pour récupérer l'enregistrement.
	//
	lastError = getDbiRecord(dbiWRITELOCK) ;

	return(lastError) ;
}

//---------------------------------------------------------------------------//  Fonction :  	NS2035Libs::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NScptsLibs::open()
{
	char tableName[] = "NSM_CPTSLIBS.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;
	return(lastError) ;
}

boolNScptsLibs::Create()
{
	return true ;
}

boolNScptsLibs::Modify()
{
	return true ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NScptsLibs&
NScptsLibs::operator=(const NScptsLibs& src)
{
  if (this == &src)
		return *this ;

  *pDonnees = *(src.pDonnees) ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NScptsLibs::operator==(const NScptsLibs& o)
{
  return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Fonction:		NS2035LibsInfo::NS2035LibsInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NScptsLibsInfo::NScptsLibsInfo()
{
	// Crée l'objet de données
  pDonnees = new NScptsLibsData() ;
}

//---------------------------------------------------------------------------
//  Description:	Constructeur à partir d'un NScptsLibs
//---------------------------------------------------------------------------
NScptsLibsInfo::NScptsLibsInfo(const NScptsLibs* pTransac)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NScptsLibsData() ;
	//
	// Copie les valeurs du NSCompt
	//
  if (pTransac)
	  *pDonnees = *(pTransac->pDonnees) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NScptsLibsInfo::NScptsLibsInfo(const NScptsLibsInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NScptsLibsData() ;
	//
	// Copie les valeurs du NS d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NScptsLibsInfo::~NScptsLibsInfo()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NScptsLibsInfo&
NScptsLibsInfo::operator=(const NScptsLibsInfo& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NScptsLibsInfo::operator==(const NScptsLibsInfo& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
// 					Implémentation des méthodes NSCptsLibsArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCptsLibsArray::NSCptsLibsArray(const NSCptsLibsArray& rv)
                :NSFicheCptsLibsArray()
{
  if (false == rv.empty())
    for (NSCptsLibsConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NScptsLibsInfo(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSCptsLibsArray::vider()
{
  if (empty())
    return ;

  for (NSCptsLibsIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSCptsLibsArray::~NSCptsLibsArray()
{
	vider() ;
}

/****************************************************************************************************************//****************************************Boîte de dialogue DEPENSES******************************************************/
/****************************************************************************************************************/

DEFINE_RESPONSE_TABLE1(NSDepensDlg, NSUtilDialog)  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_BN_CLICKED(IDC_DEPS_PLUS, Add),
  EV_BN_CLICKED(IDC_DEPS_MOINS, Less),
END_RESPONSE_TABLE;

NSDepensDlg::NSDepensDlg(TWindow* parent, NSContexte* pCtx, string sNumDep)
            :NSUtilDialog(parent, pCtx, "IDD_DEPENSES", pNSResModule)
{
  pDate               = new NSUtilEditDate(pContexte, this, IDC_DEPS_DATE) ;
  pLigne              = new TListBox(this, IDC_DEPS_LIGNE) ;
  pLibelle            = new TEdit(this, IDC_DEPS_LIBELLE) ;
  pSomme              = new NSUtilEditSomme(pContexte, this, IDC_DEPS_SOMME) ;
  pTypeDepenseArray   = new NSCptsLibsArray ;
  pBanquesArray       = new NSCptsLibsArray ;
  pPlus               = new TRadioButton(this, IDC_DEPS_PLUS) ;
  pMoins              = new TRadioButton(this, IDC_DEPS_MOINS) ;
  pBanque             = new TComboBox(this, IDC_DEPS_BANQUE) ;
  //GroupBox pour le mode de paiement.
  pMode       = new TGroupBox(this,IDC_DEPS_MODE) ;
  pButton[0]  = new TRadioButton(this,IDC_DEPS_ESPECES,pMode) ;  pButton[1]  = new TRadioButton(this,IDC_DEPS_CHEQUE,pMode) ;
  pButton[2]  = new TRadioButton(this,IDC_DEPS_VIREMENT,pMode) ;
  pButton[3]  = new TRadioButton(this,IDC_DEPS_CB,pMode) ;

  sMode[0] = "E" ;  sMode[1] = "C" ;
  sMode[2] = "V" ;
  sMode[3] = "B" ;

  pDepInfo   = 0 ;
  pEcritures = 0 ;

  sString[0] = "+" ;  sString[1] = "-" ;
  sNumDepens = sNumDep ;}

NSDepensDlg::~NSDepensDlg(){
    delete pDate;
    delete pLigne;
    delete pLibelle;
    delete pSomme;
    delete pTypeDepenseArray;
    delete pBanquesArray;
    delete pPlus;
    delete pMoins;
    delete pBanque;
    delete pMode;
    delete pButton[0];
    delete pButton[1];
    delete pButton[2];
    delete pButton[3];
    if (pDepInfo)
        delete pDepInfo;
    if (pEcritures)
        delete pEcritures;
}

voidNSDepensDlg::SetupWindow()
{
  // fichiers d'aide
  sHindex = string("") ;
  sHcorps = string("") ;

  if (string("") != sNumDepens)
    InitExistingValues() ;
  TDialog::SetupWindow() ;
  if ((string("") != sNumDepens) && (false == pEcritures->empty()))
    for (NSEcritureIter i = pEcritures->begin() ; pEcritures->end() != i ; i++)    {      if (!strcmp((*i)->pDonnees->sens, "D"))      {        pSomme->setSomme(string((*i)->pDonnees->somme));        if (!strcmp((*i)->pDonnees->signe, "+"))          pPlus->SetCheck(BF_CHECKED) ;        else          pMoins->SetCheck(BF_CHECKED) ;        break ;      }    }    else
      pPlus->SetCheck(BF_CHECKED) ;
  // mode de paiement  if ((sNumDepens != "") && (strcmp(pDepInfo->pDonnees->mode_paie, "") != 0))  {    if (!strcmp(pDepInfo->pDonnees->mode_paie, "E"))      pButton[0]->Check();    else if (!strcmp(pDepInfo->pDonnees->mode_paie, "C"))      pButton[1]->Check();    else if (!strcmp(pDepInfo->pDonnees->mode_paie, "V"))      pButton[2]->Check();    else if (!strcmp(pDepInfo->pDonnees->mode_paie, "B"))      pButton[3]->Check();  }  // Initialiser la date à la date du jour  string sDate ;
  if (sNumDepens != "")
  {
    if (!strcmp(pDepInfo->pDonnees->date, ""))
    {
      sDate = donne_date_duJour() ;      pDate->setDate(sDate) ;    }    else      pDate->setDate(string(pDepInfo->pDonnees->date)) ;  }  else  {    sDate = donne_date_duJour() ;    pDate->setDate(sDate) ;  }  if (sNumDepens != "")
    pLibelle->SetText(pDepInfo->pDonnees->libelle) ;

  InitDepensArray() ;    // s'il existe des écritures on resélectionne les comptes correspondants    if ((sNumDepens != "") && (!pEcritures->empty()))    {        for (NSEcritureIter i = pEcritures->begin(); i != pEcritures->end(); i++)        {            if (!strcmp((*i)->pDonnees->sens, "C"))            {                // cas compte banque                if (!pBanquesArray->empty())                for (NSCptsLibsIter j = pBanquesArray->begin(); j != pBanquesArray->end(); j++)                {                    if (!strcmp((*i)->pDonnees->compte, (*j)->pDonnees->code))                    {                        pBanque->SetSelString((getLibelle((*j)->pDonnees)).c_str(), -1);                        break;                    }                }            }            else            {                // cas compte perso                if (!pTypeDepenseArray->empty())                for (NSCptsLibsIter k = pTypeDepenseArray->begin(); k != pTypeDepenseArray->end(); k++)                {                    if (!strcmp((*i)->pDonnees->compte, (*k)->pDonnees->code))                    {                        pLigne->SetSelString((getLibelle((*k)->pDonnees)).c_str(), -1);                        break;                    }                }            }        }    }}
voidNSDepensDlg::CmOk()
{
    //
    // Type de dépense    //
    char szLigne[255];

    int iLength = pLigne->GetSelString(szLigne, 254);
    if (iLength < 2)
    {
        erreur("Vous devez sélectionner un type de dépense", standardError, 0) ;
        return;
    }
    string sLibDepense = string(szLigne);

    iLength = pBanque->GetSelString(szLigne, 254);
    if (iLength < 2)
    {
        erreur("Vous devez sélectionner une banque", standardError, 0) ;
        return;
    }
    string sLibBanque = string(szLigne);

    NScptsLibsInfo* pFicheBanque    = 0;
    NScptsLibsInfo* pFicheDepense   = 0;

    NSCptsLibsIter itLigne = pTypeDepenseArray->begin();
    for (; itLigne != pTypeDepenseArray->end(); itLigne++)
    {
        string sLibelle = getLibelle((*itLigne)->pDonnees);
        if (sLibelle == sLibDepense)
            pFicheDepense = *itLigne;
    }
    if (pFicheDepense == 0)
    {
        erreur("Libelle de dépense introuvable", standardError, 0) ;
        return;
    }

    NSCptsLibsIter itBanque = pBanquesArray->begin();
    for (; itBanque != pBanquesArray->end(); itBanque++)
    {
        string sLibelle = getLibelle((*itBanque)->pDonnees);
        if (sLibelle == sLibBanque)
            pFicheBanque = *itBanque;
    }
    if (pFicheBanque == 0)
    {
        erreur("Libelle de dépense introuvable", standardError, 0) ;
        return;
    }

    //
    // Somme
    //
    string sSigne = "";
    string sSomme = "";
    pSomme->getSomme(&sSomme);

    if (sSomme == "")
    {
        sSigne = "";
        MessageBox("Veuillez saisir une somme");
        return;
    }

    if      (pPlus->GetCheck() == BF_CHECKED)
        sSigne = "+";
    else if (pMoins->GetCheck() == BF_CHECKED)
        sSigne = "-";
    else
    {
        sSigne = "";
        MessageBox("Veuillez saisir le signe de la dépense");
        return;
    }
    //
    // Mode
    //
    string sResult = "";
    for (int i = 0; ((i < 4) && (sResult == "")) ; i++)
    {
        if (pButton[i]->GetCheck() == BF_CHECKED)
            sResult = sMode[i];
    }

    if (sResult == "")    {
        MessageBox ("Veuillez saisir le mode de paiement");
        return;
    }
    //
    // Date
    //

    string sDate;    pDate->getDate(&sDate);

    //    // Libellé
    //

    char szLibelle[DEPS_LIBELLE_LEN + 1];    pLibelle->GetText(szLibelle, DEPS_LIBELLE_LEN);

    //    // Création de la fiche
    //
    NSDepens* pDepensListe = new NSDepens(pContexte->getSuperviseur());

    pDepensListe->lastError = pDepensListe->open();
    if (pDepensListe->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base NSDEPENS.", standardError, pDepensListe->lastError) ;
        delete pDepensListe;
        return ;
    }
    string sNumDep = sNumDepens;
    if (sNumDep != "")        strcpy(pDepensListe->pDonnees->numdeps,     sNumDep.c_str());
    strcpy(pDepensListe->pDonnees->date,        sDate.c_str());    strcpy(pDepensListe->pDonnees->operateur,   pContexte->getUtilisateur()->getNss().c_str()) ;
    strcpy(pDepensListe->pDonnees->libelle,     szLibelle);
    strcpy(pDepensListe->pDonnees->mode_paie,   sResult.c_str());

    if (sNumDep == "")    {
        if (pDepensListe->NewNum())            sNumDep = string(pDepensListe->pDonnees->numdeps);
    }
    else
    {
        pDepensListe->lastError = pDepensListe->chercheClef(&sNumDep,
                                                            "",
                                                            0,
                                                            keySEARCHEQ,
                                                            dbiWRITELOCK);

   	    if (pDepensListe->lastError != DBIERR_NONE)   	    {
   		    erreur("Erreur à la recherche de la précédente fiche depense.", standardError, pDepensListe->lastError, pContexte->GetMainWindow()->GetHandle());
      	    pDepensListe->close();
            delete pDepensListe;
            return;
        }

        pDepensListe->lastError = pDepensListe->modifyRecord(TRUE);   	    if (pDepensListe->lastError != DBIERR_NONE)
   	    {
   		    erreur("Erreur à la modification du fichier Depenses.db", standardError, pDepensListe->lastError, pContexte->GetMainWindow()->GetHandle());
      	    pDepensListe->close();
      	    delete pDepensListe;
      	    return;
   	    }
    }

    pDepensListe->lastError = pDepensListe->close();
    if (pDepensListe->lastError != DBIERR_NONE)   	    erreur("Erreur à la fermeture de la base NSDEPENS.", standardError, pDepensListe->lastError);

    delete pDepensListe;
    if (sNumDep == "")        return;

    //    // Création des mouvements
    //

    NSEcriture* pEcriture = new NSEcriture(pContexte->getSuperviseur());
    pEcriture->lastError = pEcriture->open();
    if (pEcriture->lastError != DBIERR_NONE)    {
   	    erreur("Erreur à l'ouverture de la base ECRITURE.", standardError, pEcriture->lastError);
        delete pEcriture;
        return ;
    }

    string sNumEcriture = string("D") + sNumDep;    string sCleEcriture = sNumEcriture + string("  ");    string sLibelle     = string(szLibelle, 0, ECRI_LIBELLE_LEN);

    // Recherche du mode (création / modification) :    // on recherche dans la base    pEcriture->lastError = pEcriture->chercheClef(&sCleEcriture,      										  "",
											  0,
											  keySEARCHGEQ,
											  dbiWRITELOCK);

    if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF) && (pEcriture->lastError != DBIERR_BOF))    {
        erreur("Erreur à la recherche d'une fiche Ecriture.", standardError, pEcriture->lastError);
        pEcriture->close();
        delete pEcriture;
        return;
    }

    bool bModeAppend;

    if ((pEcriture->lastError != DBIERR_EOF) && (pEcriture->lastError != DBIERR_BOF))    {
        pEcriture->lastError = pEcriture->getRecord();
        if (pEcriture->lastError != DBIERR_NONE)
        {
            erreur("Erreur à la lecture d'une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return;
        }

        if (string(pEcriture->pDonnees->numero) != sNumEcriture)
            bModeAppend = true;
        else
            bModeAppend = false;
    }
    else
        bModeAppend = true;
    //    // Banque
    //

    strcpy(pEcriture->pDonnees->numero,     sNumEcriture.c_str());    strcpy(pEcriture->pDonnees->ligne,      "01");
    strcpy(pEcriture->pDonnees->libelle,    sLibelle.c_str());
    strcpy(pEcriture->pDonnees->compte,     pFicheBanque->pDonnees->code);
    strcpy(pEcriture->pDonnees->sens,       "C");
    strcpy(pEcriture->pDonnees->signe,      sSigne.c_str());
    strcpy(pEcriture->pDonnees->somme,      sSomme.c_str());

    if (bModeAppend)    {
        pEcriture->lastError = pEcriture->appendRecord();
        if (pEcriture->lastError != DBIERR_NONE)        {
            erreur("Erreur à l'ajout de la fiche ECRITURE.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return ;
        }
    }
    else
    {
        pEcriture->lastError = pEcriture->modifyRecord(TRUE);

        if (pEcriture->lastError != DBIERR_NONE)        {
            erreur("Erreur à la modification de la fiche ECRITURE.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return ;
        }
    }    if (!bModeAppend)    {
        // ... on passe au suivant
        pEcriture->lastError = pEcriture->suivant(dbiWRITELOCK);
        if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))
        {
            erreur("Erreur d'acces à une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return;
        }

        pEcriture->lastError = pEcriture->getRecord();
        if (pEcriture->lastError != DBIERR_NONE)
        {
            erreur("Erreur à la lecture d'une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return;
        }

        if (string(pEcriture->pDonnees->numero) != sNumEcriture)
            bModeAppend = true;
        else
            bModeAppend = false;
    }    //    // Contrepartie(s)
    //

    strcpy(pEcriture->pDonnees->numero,     sNumEcriture.c_str());    strcpy(pEcriture->pDonnees->ligne,      "02");
    strcpy(pEcriture->pDonnees->libelle,    sLibelle.c_str());
    strcpy(pEcriture->pDonnees->compte,     pFicheDepense->pDonnees->code);
    strcpy(pEcriture->pDonnees->sens,       "D");
    strcpy(pEcriture->pDonnees->signe,      sSigne.c_str());
    strcpy(pEcriture->pDonnees->somme,      sSomme.c_str());

    if (bModeAppend)    {
        pEcriture->lastError = pEcriture->appendRecord();
        if (pEcriture->lastError != DBIERR_NONE)        {
            erreur("Erreur à l'ajout de la fiche ECRITURE.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return ;
        }
    }
    else
    {
        pEcriture->lastError = pEcriture->modifyRecord(TRUE);

        if (pEcriture->lastError != DBIERR_NONE)        {
            erreur("Erreur à la modification de la fiche ECRITURE.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return ;
        }
    }    pEcriture->lastError = pEcriture->close();
    if (pEcriture->lastError != DBIERR_NONE)
   	    erreur("Erreur à la fermeture de la base ECRITURE.", standardError, pEcriture->lastError);

    delete pEcriture;
    CloseWindow(IDOK);}

voidNSDepensDlg::CmCancel()
{
    Destroy(IDCANCEL);
}

// -----------------------------------------------------------------------//
//  Initialisation des listes de comptes de dépense et de banque
//
//  On ne s'intéresse qu'aux comptes spécifiques à cet opérateur ou aux
//  comptes communs à tous les opérateurs
//
// -----------------------------------------------------------------------
void
NSDepensDlg::InitDepensArray()
{
	pTypeDepenseArray->vider() ;
	pBanquesArray->vider() ;

	if (pLigne->GetCount() > 0)
  	pLigne->ClearList() ;
	if (pBanque->GetCount() > 0)
  	pBanque->ClearList() ;

	string sUserCode = pContexte->getUtilisateur()->getNss() ;

	// créer un curseur sur NScptsLibs

	NScptsLibs CptsLibs(pContexte->getSuperviseur()) ;

	CptsLibs.lastError = CptsLibs.open() ;	if (CptsLibs.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ouverture de la base CptsLibs.", standardError, CptsLibs.lastError) ;
    return ;
	}

	CptsLibs.lastError = CptsLibs.debut(dbiWRITELOCK) ;

	if (CptsLibs.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à la recherche d'une fiche CptsLibs.", standardError, CptsLibs.lastError) ;
    CptsLibs.close() ;
    return ;
	}

	//
	//
	//
	string sMaxBqueNeutral  = "";
	string sMaxBquePerso    = "";
	string sDefaultLib      = "";

	while (DBIERR_NONE == CptsLibs.lastError)
	{
  	CptsLibs.lastError = CptsLibs.getRecord() ;
    if (DBIERR_NONE != CptsLibs.lastError)
  	{
    	erreur("Erreur à la lecture d'une fiche CptsLibs.", standardError, CptsLibs.lastError) ;
      CptsLibs.close() ;
      return ;
    }

    // On ne s'intéresse à ce compte que s'il est neutre ou spécifique
    // de l'utilisateur
    //
    if ((string(CptsLibs.pDonnees->operateur) == sUserCode) ||
            (CptsLibs.pDonnees->operateur[0] == '\0'))
    {
    	string sLibelle = getLibelle(CptsLibs.pDonnees) ;
      //
      // Ajout de la fiche à l'array
      //
      if (!strncmp(CptsLibs.pDonnees->code, BANQUE_ROOT, strlen(BANQUE_ROOT)))
      {
      	pBanquesArray->push_back(new NScptsLibsInfo(&CptsLibs)) ;
        pBanque->AddString(sLibelle.c_str()) ;

        if (CptsLibs.pDonnees->operateur[0] == '\0')
        {
        	if ((sMaxBquePerso == "") && (sMaxBqueNeutral < string(CptsLibs.pDonnees->code)))
          {
          	sMaxBqueNeutral = string(CptsLibs.pDonnees->code) ;
            sDefaultLib     = sLibelle ;
          }
        }
        else if (sMaxBquePerso < string(CptsLibs.pDonnees->code))
        {
        	sMaxBquePerso = string(CptsLibs.pDonnees->code) ;
          sDefaultLib   = sLibelle ;
        }
      }
      else if (!strncmp(CptsLibs.pDonnees->code, DEPENSE_ROOT, strlen(DEPENSE_ROOT)))
      {
      	pTypeDepenseArray->push_back(new NScptsLibsInfo(&CptsLibs)) ;
        pLigne->AddString(sLibelle.c_str()) ;
      }
    }

    // ... on passe au compte suivant
    CptsLibs.lastError = CptsLibs.suivant(dbiWRITELOCK) ;
    if ((CptsLibs.lastError != DBIERR_NONE) && (CptsLibs.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche CptsLibs.", standardError, CptsLibs.lastError) ;
      CptsLibs.close() ;
      return ;
    }
  } // fin du while

	// on ferme la base p2035Libs
  CptsLibs.lastError = CptsLibs.close() ;
  if (DBIERR_NONE != CptsLibs.lastError)
  	erreur("Erreur de fermeture du fichier CptsLibs.", standardError, CptsLibs.lastError) ;

	// Initialisation de la banque par défaut
	//
	pBanque->SetText(sDefaultLib.c_str()) ;
}

voidNSDepensDlg::InitExistingValues()
{
    if (sNumDepens == "")
        return;

    //
    // Recherche de la fiche "dépense"
    //
    NSDepens* pDepens = new NSDepens(pContexte->getSuperviseur());
    pDepens->lastError = pDepens->open();
    if (pDepens->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base NSDEPENS.", standardError, pDepens->lastError);
        delete pDepens;
        return;
    }
    pDepens->lastError = pDepens->chercheClef(&sNumDepens,
                                              "",
                                              0,
                                              keySEARCHEQ,
                                              dbiWRITELOCK);

   	if (pDepens->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la recherche de la précédente fiche depense.", standardError, pDepens->lastError, pContexte->GetMainWindow()->GetHandle());
      	pDepens->close();
        delete pDepens;
        return;
    }
    pDepens->lastError = pDepens->getRecord();
   	if (pDepens->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à lecture du fichier Depenses.db", standardError, pDepens->lastError, pContexte->GetMainWindow()->GetHandle());
      	pDepens->close();
      	delete pDepens;
      	return;
   	}

    pDepInfo = new NSDepensInfo(pDepens);

    pDepens->close();
    delete pDepens;

    //
    // Recherche des lignes d'écriture
    //
    pEcritures = new NSEcritureArray();

    string sNumEcriture = string("D") + sNumDepens;

    NSEcriture* pEcrit = new NSEcriture(pContexte->getSuperviseur());
    pEcrit->lastError = pEcrit->open();
    if (pEcrit->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base Ecritures.", standardError, pEcrit->lastError);
        delete pEcrit;
        return;
    }
    pEcrit->lastError = pEcrit->chercheClef(&sNumEcriture,
                                            "",
                                            0,
                                            keySEARCHGEQ,
                                            dbiWRITELOCK);

   	if (pEcrit->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la recherche des écritures.", standardError, pEcrit->lastError, pContexte->GetMainWindow()->GetHandle());
      	pEcrit->close();
        delete pEcrit;
        return;
    }
    pEcrit->lastError = pEcrit->getRecord();
   	if (pEcrit->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à lecture du fichier Ecritures.db", standardError, pEcrit->lastError, pContexte->GetMainWindow()->GetHandle());
      	pEcrit->close();
      	delete pEcrit;
      	return;
   	}
    while ((pEcrit->lastError == DBIERR_NONE) && (string(pEcrit->pDonnees->numero) == sNumEcriture))
    {
        pEcritures->push_back(new NSEcritureInfo(pEcrit));

        pEcrit->lastError = pEcrit->suivant(dbiWRITELOCK);
   	    if ((pEcrit->lastError != DBIERR_NONE) && (pEcrit->lastError != DBIERR_EOF))
			erreur("Erreur d'accès à l'écriture suivante.", standardError, pEcrit->lastError, GetHandle());
        else if (pEcrit->lastError != DBIERR_EOF)
        {
            pEcrit->lastError = pEcrit->getRecord();
   	        if (pEcrit->lastError != DBIERR_NONE)
   		        erreur("Erreur à lecture du fichier Ecritures.db", standardError, pEcrit->lastError, pContexte->GetMainWindow()->GetHandle());
        }
    }
    pEcrit->close();
    delete pEcrit;
}


string
NSDepensDlg::getLibelle(NScptsLibsData* pFiche)
{
    return string(pFiche->code) + string(" ") + string(pFiche->libelle);
}

void
NSDepensDlg::Add()
{
    if (pPlus->GetCheck() == BF_UNCHECKED)
    {
        pPlus->SetCheck(BF_CHECKED);
        pMoins->SetCheck(BF_UNCHECKED);
    }
}
void
NSDepensDlg::Less()
{
    if (pMoins->GetCheck() == BF_UNCHECKED)
    {
        pMoins->SetCheck(BF_CHECKED);
        pPlus->SetCheck(BF_UNCHECKED);
    }
}


//***************************************************************************
// Implémentation des méthodes NSRecettes
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSRecettesData::NSRecettesData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSRecettesData::NSRecettesData(const NSRecettesData& rv)
{
	strcpy(numcompt, rv.numcompt) ;
	strcpy(libelle,  rv.libelle) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSRecettesData::~NSRecettesData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSRecettesData&
NSRecettesData::operator=(const NSRecettesData& src)
{
  if (this == &src)
		return *this ;

  strcpy(numcompt, src.numcompt) ;  strcpy(libelle,  src.libelle) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSRecettesData::operator==(const NSRecettesData& o)
{
	if ((strcmp(numcompt, o.numcompt) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSRecettesData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
  memset(numcompt, 0, REC_NUMCOMPT_LEN + 1) ;
	memset(libelle,  0, REC_LIBELLE_LEN + 1) ;
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSRecettes::NSRecettes(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSRecettesData() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSRecettes::NSRecettes(const NSRecettes& rv) : NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSRecettesData() ;
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSRecettes::~NSRecettes()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSRecettes::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSRecettes::alimenteFiche()
{
  alimenteChamp(pDonnees->numcompt, REC_NUMCOMPT_FIELD, REC_NUMCOMPT_LEN) ;
  alimenteChamp(pDonnees->libelle,  REC_LIBELLE_FIELD,  REC_LIBELLE_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :   	NSRecettes::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSRecettes::videFiche()
{
  videChamp(pDonnees->numcompt, REC_NUMCOMPT_FIELD, REC_NUMCOMPT_LEN) ;
  videChamp(pDonnees->libelle,  REC_LIBELLE_FIELD,  REC_LIBELLE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :  	NSRecettes::open()
////  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSRecettes::open()
{
    char tableName[] = "RECETTES.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);

	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSRecettes::Create()
//---------------------------------------------------------------------------
bool
NSRecettes::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSRecettes::Modify()
//---------------------------------------------------------------------------
bool
NSRecettes::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSRecettes&
NSRecettes::operator=(const NSRecettes& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSRecettes::operator==(const NSRecettes& o)
{
  return (*pDonnees == *(o.pDonnees)) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSRecettesInfo::NSRecettesInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSRecettesInfo::NSRecettesInfo()
{
	// Crée l'objet de données
	pDonnees = new NSRecettesData() ;
}

//---------------------------------------------------------------------------//  Fonction:		NSRecettesInfo::NSRecettesInfo(NSCompt*)
//
//  Description:	Constructeur à partir d'un NSTransaction
//---------------------------------------------------------------------------
NSRecettesInfo::NSRecettesInfo(const NSRecettes* pTransac)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSRecettesData() ;
	//
	// Copie les valeurs du NSCompt
	//
  if (pTransac)
	  *pDonnees = *(pTransac->pDonnees) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSRecettesInfo::NSRecettesInfo(const NSRecettesInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSRecettesData() ;
	//
	// Copie les valeurs du NSComptInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSRecettesInfo::~NSRecettesInfo()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSRecettesInfo&
NSRecettesInfo::operator=(const NSRecettesInfo& src)
{
  if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSRecettesInfo::operator==(const NSRecettesInfo& o)
{
  return (*pDonnees == *(o.pDonnees)) ;
}

//***************************************************************************// 					Implémentation des méthodes NSRecettesArray
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSRecettesArray::NSRecettesArray(const NSRecettesArray& rv)
                :NSFicheRecettesArray()
{
  if (false == rv.empty())
    for (NSRecettesConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSRecettesInfo(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSRecettesArray::vider()
{
  if (empty())
    return ;

  for (NSRecettesIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSRecettesArray::~NSRecettesArray(){
	vider() ;
}

/****************************************************************************************************************//****************************************Boîte de dialogue RECETTES******************************************************/
/****************************************************************************************************************/

DEFINE_RESPONSE_TABLE1(NSRecettesDlg, NSUtilDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(IDC_ANALYSER, CmAnalyser),
END_RESPONSE_TABLE;

NSRecettesDlg::NSRecettesDlg(TWindow* parent, NSContexte* pCtx)
              :NSUtilDialog(parent, pCtx, "IDD_RECETTES", pNSResModule)
{
  sHindex = "" ;
  sHcorps = "Comptabilite.html" ;

  pData          = new NSComptData ;
  pFactData      = new NSFactData ;  pRecettesListe = new NSRecettes(pCtx->getSuperviseur()) ;
  pDate          = new NSUtilEditDate(pContexte, this, IDC_REC_DATE) ;

  pLibelle    = new TEdit(this, IDC_REC_LIBELLE) ;
  pSomme      = new NSUtilEditSomme(pContexte, this, IDC_REC_SOMME) ;
  //GroupBox pour le mode de paiement.
  pMode       = new TGroupBox(this,IDC_MODE) ;
  pButton[0]  = new TRadioButton(this,IDC_REC_ESPECES,pMode) ;
  pButton[1]  = new TRadioButton(this,IDC_REC_CHEQUE,pMode) ;
  pButton[2]  = new TRadioButton(this,IDC_REC_VIREMENT,pMode) ;
  pButton[3]  = new TRadioButton(this,IDC_REC_CB,pMode) ;

  sString[0] = "E" ;
  sString[1] = "C" ;
  sString[2] = "V" ;
  sString[3] = "B" ;

  pLigne              = new TListBox(this, IDC_REC_LIGNE) ;
  pTypeRecetteArray   = new NSCptsLibsArray ;
  pBanquesArray       = new NSCptsLibsArray ;
  pBanque             = new TComboBox(this, IDC_REC_BANQUE) ;
  pComptInfo          = 0 ;
  pEcritureArray      = new NSEcritureArray ;
}

NSRecettesDlg::~NSRecettesDlg(){
  delete pData ;
  delete pFactData ;
  delete pRecettesListe ;
  delete pDate ;

  delete pLibelle ;
  delete pSomme ;

  delete pMode ;
  delete pButton[0] ;
  delete pButton[1] ;
  delete pButton[2] ;
  delete pButton[3] ;

  delete pLigne ;
  delete pTypeRecetteArray ;
  delete pBanquesArray ;
  delete pBanque ;
  delete pEcritureArray ;
  if (pComptInfo)
    delete pComptInfo ;
}

voidNSRecettesDlg::SetupWindow()
{
  // fichiers d'aide
  sHindex = string("") ;
  sHcorps = string("Creation_Modification_d_un.html") ;

	TDialog::SetupWindow() ;
  // s'il existe un montant de recette, on l'affiche  if (strcmp(pData->duEuro, "") != 0)  {    // en principe les champs duEuro et payeEuro contiennent la même somme    pSomme->setSomme(string(pData->duEuro));  }  struct date dateSys;
  // en création on met la date et l'heure du jour par défaut  if (!strcmp(pData->date, ""))  {
    getdate(&dateSys);
    sprintf(pData->date, "%4d%02d%02d", dateSys.da_year,
    dateSys.da_mon, dateSys.da_day);
  }

  pDate->setDate(string(pData->date));
  // initialisation des comptes  InitRecetteArray() ;  InitComptes() ;  // s'il existe des écritures on resélectionne les comptes correspondants  if (false == pEcritureArray->empty())  {    for (NSEcritureIter i = pEcritureArray->begin() ; pEcritureArray->end() != i ; i++)    {      if (!strcmp((*i)->pDonnees->sens, "D"))      {        // cas compte banque        if (false == pBanquesArray->empty())          for (NSCptsLibsIter j = pBanquesArray->begin(); j != pBanquesArray->end(); j++)          {            if (!strcmp((*i)->pDonnees->compte, (*j)->pDonnees->code))            {              pBanque->SetSelString((getLibelle((*j)->pDonnees)).c_str(), -1);              break ;            }          }      }      else      {        // cas compte perso        if (false == pTypeRecetteArray->empty())          for (NSCptsLibsIter k = pTypeRecetteArray->begin(); k != pTypeRecetteArray->end(); k++)          {            if (!strcmp((*i)->pDonnees->compte, (*k)->pDonnees->code))            {              pLigne->SetSelString((getLibelle((*k)->pDonnees)).c_str(), -1);              break;            }          }      }    }  }  else  {    // Sélection du type de recette par défaut :    // on prend le premier du tableau (car en général unique)    if (false == pTypeRecetteArray->empty())      for (NSCptsLibsIter k = pTypeRecetteArray->begin(); k != pTypeRecetteArray->end(); k++)      {        pLigne->SetSelString((getLibelle((*k)->pDonnees)).c_str(), -1) ;        break ;      }  }  // mode de paiement  if (strcmp(pFactData->mode_paie, "") != 0)  {    if (!strcmp(pFactData->mode_paie, "E"))      pButton[0]->Check() ;    else if (!strcmp(pFactData->mode_paie, "C"))      pButton[1]->Check() ;    else if (!strcmp(pFactData->mode_paie, "V"))      pButton[2]->Check() ;    else if (!strcmp(pFactData->mode_paie, "B"))      pButton[3]->Check() ;  }
  //initialisation du libelle  pLibelle->SetText(pRecettesListe->pDonnees->libelle) ;}
voidNSRecettesDlg::CmOk()
{
  //
  // Type de dépense
  //
  char szLigne[255] ;

  int iLength = pLigne->GetSelString(szLigne, 254) ;
  if (iLength < 2)
  {
    erreur("Vous devez sélectionner un type de recette.", standardError, 0) ;
    return ;
  }
  string sLibDepense = string(szLigne) ;

  iLength = pBanque->GetSelString(szLigne, 254) ;  if (iLength < 2)
  {
    erreur("Vous devez sélectionner une banque", standardError, 0) ;
    return ;
  }
  string sLibBanque = string(szLigne) ;

  NScptsLibsInfo* pFicheBanque  = 0 ;
  NScptsLibsInfo* pFicheRecette = 0 ;

  NSCptsLibsIter itLigne = pTypeRecetteArray->begin() ;
  for ( ; pTypeRecetteArray->end() != itLigne ; itLigne++)
  {
    string sLibelle = getLibelle((*itLigne)->pDonnees) ;
    if (sLibelle == sLibDepense)
      pFicheRecette = *itLigne ;
  }
  if (pFicheRecette == 0)
  {
    erreur("Libelle de recette introuvable", standardError, 0) ;
    return ;
  }

  NSCptsLibsIter itBanque = pBanquesArray->begin() ;
  for ( ; pBanquesArray->end() != itBanque ; itBanque++)
  {
    string sLibelle = getLibelle((*itBanque)->pDonnees) ;
    if (sLibelle == sLibBanque)
      pFicheBanque = *itBanque ;
  }
  if (pFicheBanque == 0)
  {
    erreur("Libelle de banque introuvable", standardError, 0) ;
    return ;
  }

  //
  // Date
  //
  string sDate ;
  pDate->getDate(&sDate) ;

  //  // Libelle
  //
  char szLibelle[REC_LIBELLE_LEN + 1] ;
  pLibelle->GetText(szLibelle, REC_LIBELLE_LEN) ;

  string sLibelle = string(szLibelle) ;  if (string("") == sLibelle)
  {    MessageBox("Veuillez saisir un libelle") ;
    return ;
  }

  //  // Somme
  //
  string sSomme = string("") ;
  pSomme->getSomme(&sSomme) ;

  if (string("") == sSomme)  {
    MessageBox("Veuillez saisir une somme") ;
    return ;
  }

  //  // Mode
  //
  string sResult = string("") ;
  string sMode ;
  for (int i = 0 ; ((i < 4) && (sResult == "")) ; i++)
  {
    if (pButton[i]->GetCheck() == BF_CHECKED)
      sResult = sString[i] ;
  }

  if (string("") == sResult)  {
    MessageBox ("Veuillez saisir le mode de paiement") ;
    return ;
  }

  //  // Enregistrement des données
  //
  string sNumCompt ;

  if (NULL == pComptInfo)  {
    if (strcmp(pData->numcompt, "") != 0)
      sNumCompt = pData->numcompt ;
    else
    {
      if (false == ChercheNumCompt(sNumCompt))
        return;
    }
    NSCompt Compt(pContexte->getSuperviseur()) ;
    Compt.lastError = Compt.open() ;
    if (DBIERR_NONE != Compt.lastError)    {      erreur("Erreur à l'ouverture de la base Compt.", standardError, Compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      return ;
    }

    Compt.lastError = Compt.chercheClef(&sNumCompt,												                "",
												                0,
												                keySEARCHEQ,
                                        dbiWRITELOCK) ;

    if ((DBIERR_NONE != Compt.lastError) && (DBIERR_RECNOTFOUND != Compt.lastError))    {
      erreur("Erreur à la recherche de la nouvelle fiche compt.", standardError, Compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      Compt.close() ;
      return ;
    }

    Compt.lastError = Compt.getRecord() ;
    if (DBIERR_NONE != Compt.lastError)    {
      erreur("Erreur à lecture du fichier Compt.db", standardError, Compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      Compt.close() ;
      return ;
    }

    strcpy(Compt._Donnees.date,      sDate.c_str());
    strcpy(Compt._Donnees.duEuro,    sSomme.c_str());
    strcpy(Compt._Donnees.payeEuro,  sSomme.c_str());
    strcpy(Compt._Donnees.okPaye,    "1");
    strcpy(Compt._Donnees.operateur, pContexte->getUtilisateur()->getNss().c_str()) ;

    Compt.lastError = Compt.modifyRecord(TRUE) ;
    if (DBIERR_NONE != Compt.lastError)
    {
      erreur("Erreur à la modification de la fiche compt.", standardError, Compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;
      Compt.close() ;
      return ;
    }

    Compt.lastError = Compt.close() ;    if (DBIERR_NONE != Compt.lastError)
      erreur("Erreur à la fermeture de la base Compt.", standardError, Compt.lastError, pContexte->GetMainWindow()->GetHandle()) ;

    //    // Création des mouvements
    //

    NSEcriture Ecriture(pContexte->getSuperviseur()) ;
    Ecriture.lastError = Ecriture.open() ;
    if (Ecriture.lastError != DBIERR_NONE)    {
      erreur("Erreur à l'ouverture de la base ECRITURE.", standardError, Ecriture.lastError);
      return ;
    }

    string sNumEcriture = string("C") + sNumCompt;
    string sCleEcriture = sNumEcriture + string("  ");
    string sLibelle     = string(szLibelle, 0, ECRI_LIBELLE_LEN);

    // Recherche du mode (création / modification) :    // on recherche dans la base    Ecriture.lastError = Ecriture.chercheClef(&sCleEcriture,      										                    "",
                                              0,
											                        keySEARCHGEQ,
											                        dbiWRITELOCK) ;

    if ((Ecriture.lastError != DBIERR_NONE) && (Ecriture.lastError != DBIERR_EOF) && (Ecriture.lastError != DBIERR_BOF))
    {
      erreur("Erreur à la recherche d'une fiche Ecriture.", standardError, Ecriture.lastError);
      Ecriture.close() ;
      return ;
    }

    bool bModeAppend ;

    if ((Ecriture.lastError != DBIERR_EOF) && (Ecriture.lastError != DBIERR_BOF))    {
      Ecriture.lastError = Ecriture.getRecord() ;
      if (Ecriture.lastError != DBIERR_NONE)
      {
        erreur("Erreur à la lecture d'une fiche Ecriture.", standardError, Ecriture.lastError);
        Ecriture.close() ;
        return ;
      }

      if (string(Ecriture.pDonnees->numero) != sNumEcriture)
        bModeAppend = true ;
      else
        bModeAppend = false ;
    }
    else
      bModeAppend = true ;

    //    // Banque : on débite la banque
    //
    strcpy(Ecriture.pDonnees->numero,  sNumEcriture.c_str()) ;    strcpy(Ecriture.pDonnees->ligne,   "01") ;
    strcpy(Ecriture.pDonnees->libelle, sLibelle.c_str()) ;
    strcpy(Ecriture.pDonnees->compte,  pFicheBanque->pDonnees->code) ;
    strcpy(Ecriture.pDonnees->sens,    "D") ;
    strcpy(Ecriture.pDonnees->signe,   /*sSigne.c_str()*/ "+") ;
    strcpy(Ecriture.pDonnees->somme,   sSomme.c_str()) ;

    if (bModeAppend)
    {
      Ecriture.lastError = Ecriture.appendRecord() ;
      if (Ecriture.lastError != DBIERR_NONE)      {
        erreur("Erreur à l'ajout de la fiche ECRITURE.", standardError, Ecriture.lastError) ;
        Ecriture.close() ;
        return ;
      }
    }
    else
    {
      Ecriture.lastError = Ecriture.modifyRecord(TRUE) ;

      if (Ecriture.lastError != DBIERR_NONE)      {
        erreur("Erreur à la modification de la fiche ECRITURE.", standardError, Ecriture.lastError) ;
        Ecriture.close() ;
        return ;
      }
    }

    if (false == bModeAppend)
    {
      // ... on passe au suivant
      Ecriture.lastError = Ecriture.suivant(dbiWRITELOCK) ;
      if ((Ecriture.lastError != DBIERR_NONE) && (Ecriture.lastError != DBIERR_EOF))
      {
        erreur("Erreur d'acces à une fiche Ecriture.", standardError, Ecriture.lastError) ;
        Ecriture.close() ;
        return ;
      }

      Ecriture.lastError = Ecriture.getRecord() ;
      if (Ecriture.lastError != DBIERR_NONE)
      {
        erreur("Erreur à la lecture d'une fiche Ecriture.", standardError, Ecriture.lastError) ;
        Ecriture.close() ;
        return ;
      }

      if (string(Ecriture.pDonnees->numero) != sNumEcriture)
        bModeAppend = true ;
      else
        bModeAppend = false ;
    }

    //    // Contrepartie(s) : on crédite le compte
    //

    strcpy(Ecriture.pDonnees->numero,  sNumEcriture.c_str()) ;    strcpy(Ecriture.pDonnees->ligne,   "02") ;
    strcpy(Ecriture.pDonnees->libelle, sLibelle.c_str()) ;
    strcpy(Ecriture.pDonnees->compte,  pFicheRecette->pDonnees->code) ;
    strcpy(Ecriture.pDonnees->sens,    "C") ;
    strcpy(Ecriture.pDonnees->signe, /*sSigne.c_str()*/ "+") ;
    strcpy(Ecriture.pDonnees->somme,   sSomme.c_str()) ;

    if (bModeAppend)
    {
      Ecriture.lastError = Ecriture.appendRecord() ;
      if (Ecriture.lastError != DBIERR_NONE)      {
        erreur("Erreur à l'ajout de la fiche ECRITURE.", standardError, Ecriture.lastError) ;
        Ecriture.close() ;
        return ;
      }
    }
    else
    {
      Ecriture.lastError = Ecriture.modifyRecord(TRUE) ;

      if (Ecriture.lastError != DBIERR_NONE)      {
        erreur("Erreur à la modification de la fiche ECRITURE.", standardError, Ecriture.lastError) ;
        Ecriture.close() ;
        return ;
      }
    }

    Ecriture.lastError = Ecriture.close() ;    if (Ecriture.lastError != DBIERR_NONE)
      erreur("Erreur à la fermeture de la base ECRITURE.", standardError, Ecriture.lastError) ;

    //    // Création de la fiche fact
    //

    NSFact Fact(pContexte->getSuperviseur()) ;
    Fact.lastError = Fact.open() ;
    if (Fact.lastError != DBIERR_NONE)    {
      erreur("Erreur à l'ouverture de la base fact.", standardError, Fact.lastError) ;
      return ;
    }

    string sLibelleFact = string(szLibelle, 0, FACT_LIBELLE_LEN) ;    string sNumFact     = string(FACT_NUMERO_LEN, '0') ;
    string sCleFact     = sNumCompt + sNumFact ;

    Fact.lastError = Fact.chercheClef(&sCleFact,
      										            "",
											                0,
											                keySEARCHEQ,
											                dbiWRITELOCK) ;

    if ((Fact.lastError != DBIERR_NONE) && (Fact.lastError != DBIERR_RECNOTFOUND))    {
      erreur("Erreur à la recherche de la fiche Fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      return ;    }

    strcpy(Fact._Donnees.numcompt,   sNumCompt.c_str());
    strcpy(Fact._Donnees.numero,     sNumFact.c_str());
    strcpy(Fact._Donnees.operateur,  pContexte->getUtilisateur()->getNss().c_str()) ;
    strcpy(Fact._Donnees.date_paie,  sDate.c_str());
    strcpy(Fact._Donnees.libelle,    sLibelleFact.c_str());
    strcpy(Fact._Donnees.montant,    sSomme.c_str());
    strcpy(Fact._Donnees.unite,      "EUR");
    strcpy(Fact._Donnees.mode_paie,  sResult.c_str());

    if (Fact.lastError == DBIERR_RECNOTFOUND)    {
      Fact.lastError = Fact.appendRecord() ;

      if (Fact.lastError != DBIERR_NONE)      {
        erreur("Erreur à l'ajout de la fiche Fact.", standardError, Fact.lastError) ;
        Fact.close() ;
        return ;      }    }    else
    {
      Fact.lastError = Fact.modifyRecord(TRUE) ;

      if (Fact.lastError != DBIERR_NONE)      {
        erreur("Erreur à la modification de la fiche Fact.", standardError, Fact.lastError) ;
        Fact.close() ;
        return ;      }
    }

    Fact.lastError = Fact.close() ;    if (Fact.lastError != DBIERR_NONE)
      erreur("Erreur à la fermeture de la base Fact.", standardError, Fact.lastError) ;

    //    // Création de la fiche recette
    //

        NSRecettes* pRecette = new NSRecettes(pContexte->getSuperviseur());
        pRecette->lastError = pRecette->open();        if (pRecette->lastError != DBIERR_NONE)
        {
   	        erreur("Erreur à l'ouverture de la base recette.", standardError, pRecette->lastError);
            delete pRecette;
            return ;
        }

        pRecette->lastError = pRecette->chercheClef(&sNumCompt,      										  "",
											  0,
											  keySEARCHEQ,
											  dbiWRITELOCK);

        if ((pRecette->lastError != DBIERR_NONE) && (pRecette->lastError != DBIERR_RECNOTFOUND))        {
      	    erreur("Erreur à la recherche de la fiche Recette.", standardError, pRecette->lastError);
            pRecette->close();
            delete pRecette;            return ;        }
        strcpy(pRecette->pDonnees->numcompt,   sNumCompt.c_str());        strcpy(pRecette->pDonnees->libelle,    sLibelle.c_str());

        if (pRecette->lastError == DBIERR_RECNOTFOUND)        {
      	    pRecette->lastError = pRecette->appendRecord();

            if (pRecette->lastError != DBIERR_NONE)            {
         	    erreur("Erreur à l'ajout de la fiche Recette.", standardError, pRecette->lastError);
                pRecette->close();
                delete pRecette;                return;
            }        }        else
        {
      	    pRecette->lastError = pRecette->modifyRecord(TRUE);

            if (pRecette->lastError != DBIERR_NONE)            {
         	    erreur("Erreur à la modification de la fiche Recette.", standardError, pRecette->lastError);
                pRecette->close();
                delete pRecette;                return;
            }
        }
        pRecette->lastError = pRecette->close();        if (pRecette->lastError != DBIERR_NONE)
   	        erreur("Erreur à la fermeture de la base Recette.", standardError, pRecette->lastError);

        delete pRecette;  }

  CloseWindow(IDOK) ;
}

voidNSRecettesDlg::CmCancel()
{
  Destroy(IDCANCEL) ;
}

voidNSRecettesDlg::CmAnalyser()
{
	NSRecettes RecettesListe(pContexte->getSuperviseur()) ;

	pLibelle->GetText(RecettesListe.pDonnees->libelle, IDC_REC_LIBELLE + 1) ;
	if (!strcmp(RecettesListe.pDonnees->libelle, ""))
	{
		erreur("Le champ Libelle est obligatoire.", standardError, 0) ;
    return ;
	}

	//
	// on sait que le champ Libelle n'est pas vide
	//

	string sTexte = RecettesListe.pDonnees->libelle ;	if (string("£?????") == sTexte)		// texte libre
	{
		erreur("Vous devez entrer un libelle , et non en texte libre.", warningError, 0) ;
		return ;
	}

  //Creer une fiche.
  bool Cpremier  = true ;  bool Cnext     = true ;
  bool Code      = true ;
  bool Nbr       = true ;
  bool Operateur = true ;
  char prest[255] ;
  int  nbprest = 0 ;

  sTexte = string("") ;

  for (int i = 0; i < nbprest ; i++)
  {
    if (true == Cpremier)
    {
      Operateur = true ;

      while (true != Operateur)
      {
        while ((true != Code) && (true != Nbr))
        {
          sTexte = Code ;
          sTexte = Nbr ;
        }
      }
      // faire le calcul stocker le Result et passer au suivant
      if (false == Operateur)
      {
        sTexte = Code * Nbr ;
      }
    }
    else if (false == Cpremier)
    {
      Operateur = true ;

      while (false == Operateur)
      {
        while ((Nbr != true) && (Code != true))
        {
          sTexte = Code;
          sTexte = Nbr;
        }
      }
      //faire le calcul stocker le Result et passer au suivant
      if  (false == Operateur)
      {
        sTexte = Nbr * Code;
      }
    }

    if (false == Cnext)
      sTexte += string("+") ;
    else
      Cnext = false ;

    sTexte += string(prest) ;
  }
}

//
// Pour retrouver les comptes sélectionnés dans la base des écritures
//
voidNSRecettesDlg::InitComptes(){  // on récupère la cle  string sNumEcriture = string("C") + string(pData->numcompt);
	string sCleEcriture =  sNumEcriture + string("  ");

  NSEcriture* 	pEcriture = new NSEcriture(pContexte->getSuperviseur());
  pEcriture->lastError = pEcriture->open();  if (pEcriture->lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ouverture de la base Ecriture.", standardError, 0) ;
    delete pEcriture;
    return;
  }

    pEcriture->lastError = pEcriture->chercheClef(&sCleEcriture,                                                "",
                                                0,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK);

    if (pEcriture->lastError == DBIERR_BOF)	// cas fichier vide    {
   	    pEcriture->close();
        delete pEcriture;
        return;		// le tableau est vide
    }

    if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))    {
   	    erreur("Erreur à la recherche d'une fiche Ecriture.", standardError, pEcriture->lastError);
        pEcriture->close();
        delete pEcriture;
        return;
    }

    while (pEcriture->lastError != DBIERR_EOF)    {
   	    pEcriture->lastError = pEcriture->getRecord();
        if (pEcriture->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la lecture d'une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return;
        }

        // condition d'arret        if (!(string(pEcriture->pDonnees->numero) == sNumEcriture))
            break;

        // on remplit le tableau        pEcritureArray->push_back(new NSEcritureInfo(pEcriture));

        // ... on passe au suivant        pEcriture->lastError = pEcriture->suivant(dbiWRITELOCK);
        if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))
        {
      	    erreur("Erreur d'acces à une fiche Ecriture.", standardError, pEcriture->lastError);
            pEcriture->close();
            delete pEcriture;
            return;
        }
    } // fin du while (recherche des écritures)

    // on ferme la base    pEcriture->lastError = pEcriture->close();
    if (pEcriture->lastError != DBIERR_NONE)
   	    erreur("Erreur de fermeture du fichier Ecriture.", standardError, pEcriture->lastError);

    delete pEcriture;}
// -----------------------------------------------------------------------//
//  Initialisation des listes de comptes de dépense et de banque
//
//  On ne s'intéresse qu'aux comptes spécifiques à cet opérateur ou aux
//  comptes communs à tous les opérateurs
//
// -----------------------------------------------------------------------
void
NSRecettesDlg::InitRecetteArray()
{
	pTypeRecetteArray->vider() ;
	pBanquesArray->vider() ;

	if (pLigne->GetCount() > 0)
  	pLigne->ClearList() ;
	if (pBanque->GetCount() > 0)
  	pBanque->ClearList() ;

	string sUserCode = pContexte->getUtilisateur()->getNss() ;

	// créer un curseur sur NScptsLibs

	NScptsLibs* pCptsLibs = new NScptsLibs(pContexte->getSuperviseur()) ;

	pCptsLibs->lastError = pCptsLibs->open() ;
  if (pCptsLibs->lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ouverture de la base CptsLibs.", standardError, 0) ;
    delete pCptsLibs;
    return ;
  }

  pCptsLibs->lastError = pCptsLibs->debut(dbiWRITELOCK);

    if (pCptsLibs->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à la recherche d'une fiche CptsLibs.", standardError, pCptsLibs->lastError);
        pCptsLibs->close();
        delete pCptsLibs;
        return;
    }

    //
    //
    //
    string sMaxBqueNeutral  = "";
    string sMaxBquePerso    = "";
    string sDefaultLib      = "";

    while (pCptsLibs->lastError == DBIERR_NONE)
    {
        pCptsLibs->lastError = pCptsLibs->getRecord();
        if (pCptsLibs->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la lecture d'une fiche CptsLibs.", standardError, pCptsLibs->lastError);
            pCptsLibs->close();
            delete pCptsLibs;
            return ;
        }

        // On ne s'intéresse à ce compte que s'il est neutre ou spécifique
        // de l'utilisateur
        //
        if ((string(pCptsLibs->pDonnees->operateur) == sUserCode) ||
            (pCptsLibs->pDonnees->operateur[0] == '\0'))
        {
            string sLibelle = getLibelle(pCptsLibs->pDonnees);
            //
            // Ajout de la fiche à l'array
            //
            if (!strncmp(pCptsLibs->pDonnees->code, BANQUE_ROOT, strlen(BANQUE_ROOT)))
            {
                pBanquesArray->push_back(new NScptsLibsInfo(pCptsLibs));
                pBanque->AddString(sLibelle.c_str());

                if (pCptsLibs->pDonnees->operateur[0] == '\0')
                {
                    if ((sMaxBquePerso == "") && (sMaxBqueNeutral < string(pCptsLibs->pDonnees->code)))
                    {
                        sMaxBqueNeutral = string(pCptsLibs->pDonnees->code);
                        sDefaultLib     = sLibelle;
                    }
                }
                else if (sMaxBquePerso < string(pCptsLibs->pDonnees->code))
                {
                    sMaxBquePerso   = string(pCptsLibs->pDonnees->code);
                    sDefaultLib     = sLibelle;
                }
            }
            else if (!strncmp(pCptsLibs->pDonnees->code, RECETTE_ROOT, strlen(RECETTE_ROOT)))
            {
                pTypeRecetteArray->push_back(new NScptsLibsInfo(pCptsLibs));
                pLigne->AddString(sLibelle.c_str());
            }
        }

        // ... on passe au compte suivant
        pCptsLibs->lastError = pCptsLibs->suivant(dbiWRITELOCK);
        if ((pCptsLibs->lastError != DBIERR_NONE) && (pCptsLibs->lastError != DBIERR_EOF))
        {
      	    erreur("Erreur d'acces à une fiche CptsLibs.", standardError, pCptsLibs->lastError);
            pCptsLibs->close();
            delete pCptsLibs;
            return ;
        }
    } // fin du while

    // on ferme la base p2035Libs
    pCptsLibs->lastError = pCptsLibs->close();
    if (pCptsLibs->lastError != DBIERR_NONE)
   	    erreur("Erreur de fermeture du fichier CptsLibs.", standardError, pCptsLibs->lastError);

    delete pCptsLibs;

    // Initialisation de la banque par défaut
    //
    pBanque->SetText(sDefaultLib.c_str());

}

stringNSRecettesDlg::getLibelle(NScptsLibsData* pFiche)
{
    return string(pFiche->code) + string(" ") + string(pFiche->libelle);
}

bool
NSRecettesDlg::ChercheNumCompt(string& sNumCompt)
{
   	string 	sNumero;
	char 	PremOrdre[CPTA_NUMCOMPT_LEN + 1],
		  	DernOrdre[CPTA_NUMCOMPT_LEN + 1];

	//
	// Préparation des compteurs mini et maxi
	//
   	int i;
	for (i = 0; i < CPTA_NUMCOMPT_LEN; i++)
	{
		PremOrdre[i] = '0';
		DernOrdre[i] = 'z';
	}
	PremOrdre[i] 	= '\0';
	DernOrdre[i] 	= '\0';
	PremOrdre[i-1] = '1';

    // Entrée en section critique
#ifndef N_TIERS
    if (!pContexte->DemandeAcces("COMPT"))    {
    	erreur("Impossible d'obtenir l'accès à la base Compt.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        return false;
    }
#endif
	//
	// Prise d'un objet NSCompt
	//
	NSCompt* pComptTest = new NSCompt(pContexte->getSuperviseur()) ;

   	pComptTest->lastError = pComptTest->open();
   	if (pComptTest->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'ouverture de la base nscompt.db.", standardError, pComptTest->lastError,pContexte->GetMainWindow()->GetHandle()) ;
      	delete pComptTest;
#ifndef N_TIERS
        pContexte->LibereAcces("COMPT");#endif
      	return false;   	}

	//
	// On tente de se positionner sur le dernier code possible : "zzzzzzzz"
	//
   	sNumero = string(DernOrdre);

	pComptTest->lastError = pComptTest->chercheClef(&sNumero,
														 "",
														 0,
														 keySEARCHGEQ,
														 dbiWRITELOCK);
	//
	// Si on se trouve positionné en fin de fichier on recule
	//
	if (pComptTest->lastError == DBIERR_EOF)
		pComptTest->lastError = pComptTest->precedent(dbiWRITELOCK);
	//
	// Toute autre erreur est anormale
	//
	else if ((pComptTest->lastError != DBIERR_NONE) && (pComptTest->lastError != DBIERR_BOF))
	{
		erreur("Le fichier nscompt.db est endommagé.", standardError, pComptTest->lastError, pContexte->GetMainWindow()->GetHandle());
      	pComptTest->close();
		delete pComptTest;
#ifndef N_TIERS
        pContexte->LibereAcces("COMPT");#endif
		return false;	}

	//
	// On récupère l'enregistrement si fichier non vide
	//
   	if (pComptTest->lastError != DBIERR_BOF)
   	{
		pComptTest->lastError = pComptTest->getRecord();
   		if (pComptTest->lastError != DBIERR_NONE)
		{
			erreur("erreur à la lecture du fichier compt.db.", standardError, pComptTest->lastError, pContexte->GetMainWindow()->GetHandle());
      		pComptTest->close();
			delete pComptTest;
#ifndef N_TIERS
            pContexte->LibereAcces("COMPT");#endif
			return false;		}
   	}

	//
	// Si on est en fin ou début de fichier, c'est qu'il est vide
	//
	if ((pComptTest->lastError == DBIERR_BOF) ||
		(pComptTest->lastError == DBIERR_EOF) ||
		(pComptTest->lastError == DBIERR_NOCURRREC))
	{
    sNumCompt = string(PremOrdre) ;
    pComptTest->metAZero() ;
    strcpy(pComptTest->_Donnees.numcompt, sNumCompt.c_str()) ;

    	pComptTest->lastError = pComptTest->appendRecord();
   		if (pComptTest->lastError != DBIERR_NONE)
   		{
   			erreur("Erreur à l'ajout d'une fiche Compt vierge.", standardError, pComptTest->lastError, pContexte->GetMainWindow()->GetHandle());
      		pComptTest->close();
      		delete pComptTest;
#ifndef N_TIERS
            pContexte->LibereAcces("COMPT");#endif
      		return false;   		}
      	pComptTest->close();
		delete pComptTest;
#ifndef N_TIERS
        pContexte->LibereAcces("COMPT");
#endif
		return true;	}

	//
	// Toute autre erreur est anormale
	//
	if (pComptTest->lastError != DBIERR_NONE)
	{
		erreur("Le fichier compt.db est endommagé.", standardError, pComptTest->lastError, pContexte->GetMainWindow()->GetHandle());
      	pComptTest->close();
		delete pComptTest;
#ifndef N_TIERS
        pContexte->LibereAcces("COMPT");#endif
		return false;	}

	//
	// On est positionné sur le dernier document du patient, que l'on doit
	// incrémenter pour avoir le nouveau code
	//
	char compteur[CPTA_NUMCOMPT_LEN + 1] ;
	strcpy(compteur, pComptTest->_Donnees.numcompt) ;

	//
	// On incrémente le compteur
	//
	int j;
	int calculer = 1;
	i = strlen(compteur) - 1;

	while (calculer)
   	{
    	j = (int) compteur[i];
		j++;
		if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))
		{
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
			if (i == 0)
			{
				erreur("Compteur de fiches compt saturé.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
                pComptTest->close();
				delete pComptTest;
#ifndef N_TIERS
        		pContexte->LibereAcces("COMPT");#endif
				return false;			}
			i--;
		}
	}

	sNumCompt = string(compteur);

    // on stocke une fiche vierge pour etre plus sur
    pComptTest->metAZero();
    strcpy(pComptTest->_Donnees.numcompt, sNumCompt.c_str());

    pComptTest->lastError = pComptTest->appendRecord();
   	if (pComptTest->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'ajout d'une fiche Compt vierge.", standardError, pComptTest->lastError, pContexte->GetMainWindow()->GetHandle());
      	pComptTest->close();
      	delete pComptTest;
#ifndef N_TIERS
        pContexte->LibereAcces("COMPT");#endif
      	return false;   	}

    pComptTest->lastError = pComptTest->close();
   	if (pComptTest->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la fermeture de la base nscompt.db.", standardError, pComptTest->lastError,pContexte->GetMainWindow()->GetHandle()) ;
      	delete pComptTest;
#ifndef N_TIERS
        pContexte->LibereAcces("COMPT");#endif
      	return false;   	}

	delete pComptTest;

    // Sortie de section critique
#ifndef N_TIERS
    pContexte->LibereAcces("COMPT");
#endif

	return true;}

