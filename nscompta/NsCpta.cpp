//---------------------------------------------------------------------------//    NSCPTA.CPP
//    KRZISCH PH.   janvier 92
//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

//using namespace std;

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nscompta\nscpta.h"
#include "nscompta\nsfseuti.h"

//***************************************************************************
// Implémentation des méthodes NSCompt
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSComptData::NSComptData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSComptData::NSComptData(const NSComptData& rv)
{
	strcpy(numcompt,    rv.numcompt) ;
	strcpy(numfse, 	    rv.numfse) ;
	strcpy(patient,     rv.patient) ;
	strcpy(date,        rv.date) ;
	strcpy(heure, 	    rv.heure) ;
	strcpy(duFranc,     rv.duFranc) ;
	strcpy(payeFranc,   rv.payeFranc) ;
	strcpy(depassFranc, rv.depassFranc) ;
	strcpy(duEuro, 	    rv.duEuro) ;
	strcpy(payeEuro,    rv.payeEuro) ;
	strcpy(depassEuro,  rv.depassEuro) ;
	strcpy(examen, 	    rv.examen) ;
	strcpy(synonyme,    rv.synonyme) ;
	strcpy(okPaye, 	    rv.okPaye) ;
	strcpy(prescript,   rv.prescript) ;
	strcpy(contexte,    rv.contexte) ;
	strcpy(code,        rv.code) ;
	strcpy(operateur,   rv.operateur) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSComptData::~NSComptData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSComptData&
NSComptData::operator=(const NSComptData& src)
{
	if (this == &src)
		return *this ;

	strcpy(numcompt,    src.numcompt) ;
	strcpy(numfse, 	    src.numfse) ;
	strcpy(patient,     src.patient) ;
	strcpy(date,        src.date) ;
	strcpy(heure, 	    src.heure) ;
	strcpy(duFranc,     src.duFranc) ;
	strcpy(payeFranc,   src.payeFranc) ;
	strcpy(depassFranc, src.depassFranc) ;
	strcpy(duEuro, 	    src.duEuro) ;
	strcpy(payeEuro,    src.payeEuro) ;
	strcpy(depassEuro,  src.depassEuro) ;
	strcpy(examen, 	    src.examen) ;
	strcpy(synonyme,    src.synonyme) ;
	strcpy(okPaye, 	    src.okPaye) ;
	strcpy(prescript,   src.prescript) ;
	strcpy(contexte,    src.contexte) ;
	strcpy(code,        src.code) ;
	strcpy(operateur,   src.operateur) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSComptData::operator == ( const NSComptData& o )
{
	if ((strcmp(numcompt, o.numcompt) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSComptData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt,	  0, CPTA_NUMCOMPT_LEN + 1) ;
	memset(numfse, 	    0, CPTA_NUMFSE_LEN + 1) ;
	memset(patient, 	  0, CPTA_PATIENT_LEN + 1) ;
	memset(date, 	 	    0, CPTA_DATE_LEN + 1) ;
	memset(heure, 	 	  0, CPTA_HEURE_LEN + 1) ;
	memset(duFranc, 	  0, CPTA_DUE_F_LEN + 1) ;
	memset(payeFranc,   0, CPTA_PAYE_F_LEN + 1) ;
	memset(depassFranc, 0, CPTA_DEPAS_F_LEN + 1) ;
	memset(duEuro, 	    0, CPTA_DUE_E_LEN + 1) ;
	memset(payeEuro, 	  0, CPTA_PAYE_E_LEN + 1) ;
	memset(depassEuro,  0, CPTA_DEPAS_E_LEN + 1) ;
	memset(examen, 	    0, CPTA_EXAMEN_LEN + 1) ;
	memset(synonyme,	  0, CPTA_SYNONYME_LEN + 1) ;
	memset(okPaye, 	    0, CPTA_OKPAYE_LEN + 1) ;	memset(prescript,   0, CPTA_PRESCRIPT_LEN + 1) ;
	memset(contexte, 	  0, CPTA_CONTEXTE_LEN + 1) ;
	memset(code, 	 	    0, CPTA_CODE_LEN + 1) ;
	memset(operateur,   0, CPTA_OPERATEUR_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSCompt::NSCompt(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCompt::NSCompt(const NSCompt& rv) : NSFiche(rv._pSuper)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCompt::~NSCompt()
{
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCompt::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSCompt::alimenteFiche()
{
	alimenteChamp(_Donnees.numcompt, 	 CPTA_NUMCOMPT_FIELD,  CPTA_NUMCOMPT_LEN) ;
	alimenteChamp(_Donnees.numfse, 	   CPTA_NUMFSE_FIELD, 	 CPTA_NUMFSE_LEN) ;
	alimenteChamp(_Donnees.patient, 	   CPTA_PATIENT_FIELD,   CPTA_PATIENT_LEN) ;
	alimenteChamp(_Donnees.date, 	 	   CPTA_DATE_FIELD, 	   CPTA_DATE_LEN) ;
	alimenteChamp(_Donnees.heure, 	 	   CPTA_HEURE_FIELD, 	   CPTA_HEURE_LEN) ;
	alimenteChamp(_Donnees.duFranc, 	   CPTA_DUE_F_FIELD, 	   CPTA_DUE_F_LEN) ;
	alimenteChamp(_Donnees.payeFranc,   CPTA_PAYE_F_FIELD, 	 CPTA_PAYE_F_LEN) ;
	alimenteChamp(_Donnees.depassFranc, CPTA_DEPAS_F_FIELD,   CPTA_DEPAS_F_LEN) ;
	alimenteChamp(_Donnees.duEuro, 	   CPTA_DUE_E_FIELD, 	   CPTA_DUE_E_LEN) ;
	alimenteChamp(_Donnees.payeEuro, 	 CPTA_PAYE_E_FIELD, 	 CPTA_PAYE_E_LEN) ;
	alimenteChamp(_Donnees.depassEuro,  CPTA_DEPAS_E_FIELD,   CPTA_DEPAS_E_LEN) ;
	alimenteChamp(_Donnees.examen, 	   CPTA_EXAMEN_FIELD, 	 CPTA_EXAMEN_LEN) ;
	alimenteChamp(_Donnees.synonyme,	   CPTA_SYNONYME_FIELD,  CPTA_SYNONYME_LEN) ;
	alimenteChamp(_Donnees.okPaye, 	   CPTA_OKPAYE_FIELD, 	 CPTA_OKPAYE_LEN) ;
	alimenteChamp(_Donnees.prescript,   CPTA_PRESCRIPT_FIELD, CPTA_PRESCRIPT_LEN) ;
	alimenteChamp(_Donnees.contexte, 	 CPTA_CONTEXTE_FIELD,  CPTA_CONTEXTE_LEN) ;
	alimenteChamp(_Donnees.code, 	 	   CPTA_CODE_FIELD, 	   CPTA_CODE_LEN) ;
	alimenteChamp(_Donnees.operateur,   CPTA_OPERATEUR_FIELD, CPTA_OPERATEUR_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :   	NSCompt::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCompt::videFiche()
{
	videChamp(_Donnees.numcompt, 	 CPTA_NUMCOMPT_FIELD,  CPTA_NUMCOMPT_LEN) ;
	videChamp(_Donnees.numfse, 	   CPTA_NUMFSE_FIELD, 	 CPTA_NUMFSE_LEN) ;
	videChamp(_Donnees.patient, 	 	 CPTA_PATIENT_FIELD,   CPTA_PATIENT_LEN) ;
	videChamp(_Donnees.date, 	 	 	 CPTA_DATE_FIELD, 	   CPTA_DATE_LEN) ;
	videChamp(_Donnees.heure, 	 	   CPTA_HEURE_FIELD, 	   CPTA_HEURE_LEN) ;
	videChamp(_Donnees.duFranc, 	 	 CPTA_DUE_F_FIELD, 	   CPTA_DUE_F_LEN) ;
	videChamp(_Donnees.payeFranc,   CPTA_PAYE_F_FIELD, 	 CPTA_PAYE_F_LEN) ;
	videChamp(_Donnees.depassFranc, CPTA_DEPAS_F_FIELD,   CPTA_DEPAS_F_LEN) ;
	videChamp(_Donnees.duEuro, 	   CPTA_DUE_E_FIELD, 	   CPTA_DUE_E_LEN) ;
	videChamp(_Donnees.payeEuro, 	 CPTA_PAYE_E_FIELD, 	 CPTA_PAYE_E_LEN) ;
	videChamp(_Donnees.depassEuro,  CPTA_DEPAS_E_FIELD,   CPTA_DEPAS_E_LEN) ;
	videChamp(_Donnees.examen, 	   CPTA_EXAMEN_FIELD, 	 CPTA_EXAMEN_LEN) ;
	videChamp(_Donnees.synonyme,	 	 CPTA_SYNONYME_FIELD,  CPTA_SYNONYME_LEN) ;
	videChamp(_Donnees.okPaye, 	   CPTA_OKPAYE_FIELD, 	 CPTA_OKPAYE_LEN) ;
	videChamp(_Donnees.prescript,   CPTA_PRESCRIPT_FIELD, CPTA_PRESCRIPT_LEN) ;
	videChamp(_Donnees.contexte, 	 CPTA_CONTEXTE_FIELD,  CPTA_CONTEXTE_LEN) ;
	videChamp(_Donnees.code, 	 	 	 CPTA_CODE_FIELD, 	   CPTA_CODE_LEN) ;
	videChamp(_Donnees.operateur,   CPTA_OPERATEUR_FIELD, CPTA_OPERATEUR_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCompt::getRecord()
//
//  Description : Prend l'enregistrement en cours et assigne la valeur des
//             	champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSCompt::getPatRecord()
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

//---------------------------------------------------------------------------
//  Fonction :  	NSCompt::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSCompt::open()
{
	char tableName[] = "NSM_COMPT.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;
	return(lastError) ;
}

//---------------------------------------------------------------------------//  Function:  NSCompt::Create()
//---------------------------------------------------------------------------
bool
NSCompt::Create()
{
	return true ;
}

//---------------------------------------------------------------------------//  Function:  NSCompt::Modify()
//---------------------------------------------------------------------------
bool
NSCompt::Modify()
{
	return true ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCompt&
NSCompt::operator=(const NSCompt& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCompt::operator == (const NSCompt& o)
{
	return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSComptInfo::NSComptInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSComptInfo::NSComptInfo()
{
}

//---------------------------------------------------------------------------//  Fonction:		NSComptInfo::NSComptInfo(NSCompt*)
//
//  Description:	Constructeur à partir d'un NSTransaction
//---------------------------------------------------------------------------
NSComptInfo::NSComptInfo(const NSCompt* pTransac)
{
  if (pTransac)
	  _Donnees = pTransac->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSComptInfo::NSComptInfo(const NSComptInfo& rv)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSComptInfo::~NSComptInfo()
{
}

bool
NSComptInfo::InitFse1610Array(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte)
{
	string    sNumCompt   = _Donnees.getNumCompt() ;
	string		sNumFse1610 = sNumCompt + string("    ") ;

	NSFse1610 Fse1610(pContexte->getSuperviseur()) ;

  Fse1610.lastError = Fse1610.open() ;
  if (Fse1610.lastError != DBIERR_NONE)
  {
  	erreur("Erreur à l'ouverture de la base Fse1610.", standardError, Fse1610.lastError) ;
    return false ;
  }

  Fse1610.lastError = Fse1610.chercheClef(&sNumFse1610,
      														        "",
                                          0,
																          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

	if (Fse1610.lastError == DBIERR_BOF)	// cas fichier vide
  {
   	Fse1610.close() ;
    return true ;		// le tableau est vide
  }

  if ((Fse1610.lastError != DBIERR_NONE) && (Fse1610.lastError != DBIERR_EOF))
  {
   	erreur("Erreur à la recherche d'une fiche Fse1610.", standardError, Fse1610.lastError) ;
    Fse1610.close() ;
    return false ;
  }

  while (Fse1610.lastError != DBIERR_EOF)  {
  	Fse1610.lastError = Fse1610.getRecord() ;
    if (Fse1610.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche Fse1610.", standardError, Fse1610.lastError) ;
      Fse1610.close() ;
      return false ;
    }

    // condition d'arret
    if (Fse1610.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&Fse1610)) ;
    if (NULL != pCounter)
    	(*pCounter)++ ;

    // ... on passe au composant suivant
    Fse1610.lastError = Fse1610.suivant(dbiWRITELOCK) ;
    if ((Fse1610.lastError != DBIERR_NONE) && (Fse1610.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche Fse1610.", standardError, Fse1610.lastError) ;
      Fse1610.close() ;
      return false ;
    }
  } // fin du while

  // on ferme la base Fse1610
  Fse1610.lastError = Fse1610.close() ;
  if (Fse1610.lastError != DBIERR_NONE)
  {
  	erreur("Erreur de fermeture du fichier Fse1610.", standardError, Fse1610.lastError) ;
    return false ;
  }

	return true ;
}

bool
NSComptInfo::InitFse1620Array(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte)
{
	string    sNumCompt   = _Donnees.getNumCompt() ;
	string 		sNumFse1620 = sNumCompt + string("    ") ;
	NSFse1620 Fse1620(pContexte->getSuperviseur()) ;

	Fse1620.lastError = Fse1620.open() ;
	if (Fse1620.lastError != DBIERR_NONE)
	{
   	erreur("Erreur à l'ouverture de la base Fse1620.", standardError, Fse1620.lastError) ;
		return false ;
	}

	Fse1620.lastError = Fse1620.chercheClef(&sNumFse1620,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

	if (Fse1620.lastError == DBIERR_BOF)	// cas fichier vide
	{
  	Fse1620.close() ;
		return true ;		// le tableau est vide
	}

	if ((Fse1620.lastError != DBIERR_NONE) && (Fse1620.lastError != DBIERR_EOF))
	{
		erreur("Erreur à la recherche d'une fiche Fse1620.", standardError, Fse1620.lastError) ;
		Fse1620.close() ;
		return false ;
	}

	while (Fse1620.lastError != DBIERR_EOF)
	{
		Fse1620.lastError = Fse1620.getRecord() ;
		if (Fse1620.lastError != DBIERR_NONE)
		{
    	erreur("Erreur à la lecture d'une fiche Fse1620.", standardError, Fse1620.lastError) ;
      Fse1620.close() ;
      return false ;
    }

    // condition d'arret
    if (Fse1620.getNumCompt() != sNumCompt)
    	break;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&Fse1620)) ;
    if (NULL != pCounter)
    	(*pCounter)++ ;

    // ... on passe au composant suivant
    Fse1620.lastError = Fse1620.suivant(dbiWRITELOCK) ;
    if ((Fse1620.lastError != DBIERR_NONE) && (Fse1620.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche Fse1620.", standardError, Fse1620.lastError) ;
      Fse1620.close() ;
			return false ;
    }
	} // fin du while (recherche des composants images)

	// on ferme la base Fse1620
	Fse1620.lastError = Fse1620.close() ;
	if (Fse1620.lastError != DBIERR_NONE)
	{
   	erreur("Erreur de fermeture du fichier Fse1620.", standardError, Fse1620.lastError) ;
    return false ;
	}

	return true ;
}

bool
NSComptInfo::InitFse1630Array(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte)
{
	string    sNumCompt   = _Donnees.getNumCompt() ;
	string 		sNumFse1630 = sNumCompt + string("    ") ;
	NSFse1630 Fse1630(pContexte->getSuperviseur()) ;

	Fse1630.lastError = Fse1630.open() ;
	if (Fse1630.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base Fse1630.", standardError, Fse1630.lastError) ;
    return false ;
	}

	Fse1630.lastError = Fse1630.chercheClef(&sNumFse1630,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

	if (Fse1630.lastError == DBIERR_BOF)	// cas fichier vide
	{
		Fse1630.close() ;
		return true ;		// le tableau est vide
	}

	if ((Fse1630.lastError != DBIERR_NONE) && (Fse1630.lastError != DBIERR_EOF))
	{
		erreur("Erreur à la recherche d'une fiche Fse1630.", standardError, Fse1630.lastError) ;
		Fse1630.close() ;
		return false ;
	}

	while (Fse1630.lastError != DBIERR_EOF)
	{
		Fse1630.lastError = Fse1630.getRecord() ;
		if (Fse1630.lastError != DBIERR_NONE)
		{
    	erreur("Erreur à la lecture d'une fiche Fse1630.", standardError, Fse1630.lastError) ;
    	Fse1630.close() ;
      return false ;
    }

    // condition d'arret
    if (Fse1630.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&Fse1630)) ;
    if (NULL != pCounter)
    	(*pCounter)++ ;

    // ... on passe au composant suivant    Fse1630.lastError = Fse1630.suivant(dbiWRITELOCK) ;
    if ((Fse1630.lastError != DBIERR_NONE) && (Fse1630.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche Fse1630.", standardError, Fse1630.lastError) ;
      Fse1630.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

	// on ferme la base Fse1630
	Fse1630.lastError = Fse1630.close() ;
	if (Fse1630.lastError != DBIERR_NONE)
	{
		erreur("Erreur de fermeture du fichier Fse1630.", standardError, Fse1630.lastError) ;
    return false ;
	}

	return true ;
}

bool
NSComptInfo::InitCCAMArray(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte)
{
	string    sNumCompt = string(_Donnees.numcompt) ;
	string		sNumCCAM  = sNumCompt ;
	NSFseCCAM FseCCAM(pContexte->getSuperviseur()) ;

  FseCCAM.lastError = FseCCAM.open() ;
  if (FseCCAM.lastError != DBIERR_NONE)
  {
  	erreur("Erreur à l'ouverture de la base FseCCAM.", standardError, FseCCAM.lastError) ;
    return false ;
  }

  FseCCAM.lastError = FseCCAM.chercheClef(&sNumCCAM,
      														        "",
                                          0,
																          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

	if (FseCCAM.lastError == DBIERR_BOF)	// cas fichier vide
  {
   	FseCCAM.close() ;
    return true ;		// le tableau est vide
  }

  if ((FseCCAM.lastError != DBIERR_NONE) && (FseCCAM.lastError != DBIERR_EOF))
  {
   	erreur("Erreur à la recherche d'une fiche FseCCAM.", standardError, FseCCAM.lastError) ;
    FseCCAM.close() ;
    return false ;
  }

  while (FseCCAM.lastError != DBIERR_EOF)  {
  	FseCCAM.lastError = FseCCAM.getRecord() ;
    if (FseCCAM.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche FseCCAM.", standardError, FseCCAM.lastError) ;
      FseCCAM.close() ;
      return false ;
    }

    // condition d'arret
    if (FseCCAM.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&FseCCAM)) ;
    if (NULL != pCounter)
    	(*pCounter)++ ;

    // ... on passe au composant suivant
    FseCCAM.lastError = FseCCAM.suivant(dbiWRITELOCK) ;
    if ((FseCCAM.lastError != DBIERR_NONE) && (FseCCAM.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche FseCCAM.", standardError, FseCCAM.lastError) ;
      FseCCAM.close() ;
      return false ;
    }
  } // fin du while

  // on ferme la base Fse1610
  FseCCAM.lastError = FseCCAM.close() ;
  if (FseCCAM.lastError != DBIERR_NONE)
  {
  	erreur("Erreur de fermeture du fichier FseCCAM.", standardError, FseCCAM.lastError) ;
    return false ;
  }

	return true ;
}

bool
NSComptInfo::InitLibreArray(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte)
{
	string     sNumCompt = string(_Donnees.numcompt) ;
	string		 sNumCCAM  = sNumCompt ;
	NSFseLibre FseLibre(pContexte->getSuperviseur()) ;

  FseLibre.lastError = FseLibre.open() ;
  if (DBIERR_NONE != FseLibre.lastError)
  {
  	erreur("Erreur à l'ouverture de la base FseLibre.", standardError, FseLibre.lastError) ;
    return false ;
  }

  FseLibre.lastError = FseLibre.chercheClef(&sNumCCAM,
      						  								        "",
                                            0,
											  					          keySEARCHGEQ,
                                            dbiWRITELOCK) ;

	if (DBIERR_BOF == FseLibre.lastError)	// cas fichier vide
  {
   	FseLibre.close() ;
    return true ;		// le tableau est vide
  }

  if ((FseLibre.lastError != DBIERR_NONE) && (FseLibre.lastError != DBIERR_EOF))
  {
   	erreur("Erreur à la recherche d'une fiche FseLibre.", standardError, FseLibre.lastError) ;
    FseLibre.close() ;
    return false ;
  }

  while (FseLibre.lastError != DBIERR_EOF)  {
  	FseLibre.lastError = FseLibre.getRecord() ;
    if (FseLibre.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche FseLibre.", standardError, FseLibre.lastError) ;
      FseLibre.close() ;
      return false ;
    }

    // condition d'arret
    if (FseLibre.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFseArray->push_back(new NSBlocFse16(&FseLibre)) ;
    if (NULL != pCounter)
    	(*pCounter)++ ;

    // ... on passe au composant suivant
    FseLibre.lastError = FseLibre.suivant(dbiWRITELOCK) ;
    if ((FseLibre.lastError != DBIERR_NONE) && (FseLibre.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche FseCCAM.", standardError, FseLibre.lastError) ;
      FseLibre.close() ;
      return false ;
    }
  } // fin du while

  // on ferme la base FseLibre
  FseLibre.lastError = FseLibre.close() ;
  if (FseLibre.lastError != DBIERR_NONE)
  {
  	erreur("Erreur de fermeture du fichier FseLibre.", standardError, FseLibre.lastError) ;
    return false ;
  }

	return true ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSComptInfo&
NSComptInfo::operator=(const NSComptInfo& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSComptInfo::operator == (const NSComptInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

//***************************************************************************// 					Implémentation des méthodes NSComptArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSComptArray::NSComptArray(const NSComptArray& rv) : NSFicheComptArray()
{
	if (false == rv.empty())
		for (NSComptConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSComptInfo(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSComptArray::vider()
{
	if (empty())
		return ;

	for (NSComptIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSComptArray::~NSComptArray(){
	vider() ;
}

NSComptArray&
NSComptArray::operator=(const NSComptArray& src)
{
  if (this == &src)
  	return *this ;

  vider() ;

  if (false == src.empty())
		for (NSComptConstIter i = src.begin() ; src.end() != i ; i++)
   		push_back(new NSComptInfo(*(*i))) ;

  return *this ;
}

//***************************************************************************// Implémentation des méthodes NSFact
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFactData::NSFactData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFactData::NSFactData(const NSFactData& rv)
{
	strcpy(numcompt,  rv.numcompt) ;
	strcpy(numero,    rv.numero) ;
	strcpy(operateur, rv.operateur) ;
	strcpy(date_paie, rv.date_paie) ;
	strcpy(organisme, rv.organisme) ;
	strcpy(libelle,   rv.libelle) ;
	strcpy(montant,   rv.montant) ;
	strcpy(unite,     rv.unite) ;
	strcpy(mode_paie, rv.mode_paie) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFactData::~NSFactData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSFactData&
NSFactData::operator=(const NSFactData& src)
{
  if (this == &src)
  	return *this ;

  strcpy(numcompt, 	src.numcompt) ;
  strcpy(numero,		src.numero) ;
	strcpy(operateur, src.operateur) ;
	strcpy(date_paie, src.date_paie) ;
  strcpy(organisme, src.organisme) ;
  strcpy(libelle,		src.libelle) ;
	strcpy(montant,  	src.montant) ;
	strcpy(unite, 		src.unite) ;
  strcpy(mode_paie,	src.mode_paie) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFactData::operator == ( const NSFactData& o )
{
	if ((strcmp(numcompt, o.numcompt) == 0) &&
   	 (strcmp(numero, o.numero) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFactData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 	0, FACT_NUMCOMPT_LEN + 1);
  memset(numero,		0, FACT_NUMERO_LEN + 1);
	memset(operateur, 0, FACT_OPERATEUR_LEN + 1);
	memset(date_paie, 0, FACT_DATE_PAIE_LEN + 1);
	memset(organisme,	0, FACT_ORGANISME_LEN + 1);
  memset(libelle,	  0, FACT_LIBELLE_LEN + 1);
	memset(montant,  	0, FACT_MONTANT_LEN + 1);
	memset(unite, 		0, FACT_UNITE_LEN + 1);
  memset(mode_paie, 0, FACT_MODE_PAIE_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFact::NSFact(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFact::NSFact(const NSFact& rv) : NSFiche(rv._pSuper)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFact::~NSFact()
{
}

//---------------------------------------------------------------------------//  Fonction :  	NSFact::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFact::alimenteFiche()
{
	alimenteChamp(_Donnees.numcompt,	 FACT_NUMCOMPT_FIELD,	 FACT_NUMCOMPT_LEN) ;
  alimenteChamp(_Donnees.numero,		 FACT_NUMERO_FIELD,		 FACT_NUMERO_LEN) ;
  alimenteChamp(_Donnees.operateur, FACT_OPERATEUR_FIELD, FACT_OPERATEUR_LEN) ;
  alimenteChamp(_Donnees.date_paie, FACT_DATE_PAIE_FIELD, FACT_DATE_PAIE_LEN) ;
  alimenteChamp(_Donnees.organisme, FACT_ORGANISME_FIELD, FACT_ORGANISME_LEN) ;
  alimenteChamp(_Donnees.libelle,	 FACT_LIBELLE_FIELD,	 FACT_LIBELLE_LEN) ;
  alimenteChamp(_Donnees.montant,   FACT_MONTANT_FIELD, 	 FACT_MONTANT_LEN) ;
  alimenteChamp(_Donnees.unite, 		 FACT_UNITE_FIELD, 		 FACT_UNITE_LEN) ;
  alimenteChamp(_Donnees.mode_paie, FACT_MODE_PAIE_FIELD, FACT_MODE_PAIE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :   	NSFact::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFact::videFiche()
{
	videChamp(_Donnees.numcompt,  FACT_NUMCOMPT_FIELD,  FACT_NUMCOMPT_LEN) ;
  videChamp(_Donnees.numero,		 FACT_NUMERO_FIELD,		 FACT_NUMERO_LEN) ;
  videChamp(_Donnees.operateur, FACT_OPERATEUR_FIELD, FACT_OPERATEUR_LEN) ;
  videChamp(_Donnees.date_paie, FACT_DATE_PAIE_FIELD, FACT_DATE_PAIE_LEN) ;
	videChamp(_Donnees.organisme, FACT_ORGANISME_FIELD, FACT_ORGANISME_LEN) ;
  videChamp(_Donnees.libelle,	 FACT_LIBELLE_FIELD,	 FACT_LIBELLE_LEN) ;
  videChamp(_Donnees.montant,   FACT_MONTANT_FIELD, 	 FACT_MONTANT_LEN) ;
  videChamp(_Donnees.unite, 		 FACT_UNITE_FIELD, 		 FACT_UNITE_LEN) ;
  videChamp(_Donnees.mode_paie, FACT_MODE_PAIE_FIELD, FACT_MODE_PAIE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :  	NSFact::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFact::open()
{
	char tableName[] = "NSM_FACT.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);
	return(lastError);
}

//---------------------------------------------------------------------------//  Function:  NSFact::Create()
//---------------------------------------------------------------------------
bool
NSFact::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSFact::Modify()
//---------------------------------------------------------------------------
bool
NSFact::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFact&
NSFact::operator=(const NSFact& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFact::operator == (const NSFact& o)
{
	 return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSFactInfo::NSFactInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFactInfo::NSFactInfo()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSFactInfo::NSFactInfo(NSFact*)
//
//  Description:	Constructeur à partir d'un NSFact
//---------------------------------------------------------------------------
NSFactInfo::NSFactInfo(const NSFact* pFact)
{
  if (pFact)
    _Donnees = pFact->_Donnees ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFactInfo::NSFactInfo(const NSFactInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFactInfo::~NSFactInfo()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFactInfo&
NSFactInfo::operator=(const NSFactInfo& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFactInfo::operator == (const NSFactInfo& o)
{
  return (_Donnees == o._Donnees) ;
}

//***************************************************************************
// 					Implémentation des méthodes NSFactArray
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFactArray::NSFactArray(const NSFactArray& rv) : NSFicheFactArray()
{
	if (false == rv.empty())
		for (NSFactConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSFactInfo(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSFactArray::vider()
{
	if (false == empty())
    for (NSFactIter i = begin() ; end() != i ; )
		{
			delete *i ;
      erase(i) ;
		}
}

NSFactArray::~NSFactArray(){
	vider() ;
}

NSFactArray&
NSFactArray::operator=(const NSFactArray& src)
{
  if (this == &src)
  	return *this ;

  vider() ;

  if (false == src.empty())
		for (NSFactConstIter i = src.begin() ; src.end() != i ; i++)
   		push_back(new NSFactInfo(*(*i))) ;

  return *this ;
}

//***************************************************************************
// Implémentation des méthodes NSComptDocu
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSComptDocuData::NSComptDocuData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSComptDocuData::NSComptDocuData(const NSComptDocuData& rv)
{
	strcpy(numcompt, rv.numcompt) ;
	strcpy(indice, 	 rv.indice) ;
  strcpy(patient,  rv.patient) ;
	strcpy(document, rv.document) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSComptDocuData::~NSComptDocuData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSComptDocuData&
NSComptDocuData::operator=(const NSComptDocuData& src)
{
	if (this == &src)
		return *this ;

	strcpy(numcompt, src.numcompt) ;
	strcpy(indice, 	 src.indice) ;
  strcpy(patient,  src.patient) ;
	strcpy(document, src.document) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSComptDocuData::operator == ( const NSComptDocuData& o )
{
	if ((strcmp(numcompt, o.numcompt) == 0) &&
      (strcmp(indice,   o.indice) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSComptDocuData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 0, CPTDOCU_NUMCOMPT_LEN + 1) ;
	memset(indice, 	 0, CPTDOCU_INDICE_LEN + 1) ;
  memset(patient,  0, CPTDOCU_PATIENT_LEN + 1) ;
	memset(document, 0, CPTDOCU_DOCUMENT_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSComptDocu::NSComptDocu(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSComptDocu::NSComptDocu(const NSComptDocu& rv) : NSFiche(rv._pSuper)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSComptDocu::~NSComptDocu()
{
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCompt::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSComptDocu::alimenteFiche()
{
	alimenteChamp(_Donnees.numcompt, CPTDOCU_NUMCOMPT_FIELD, CPTDOCU_NUMCOMPT_LEN) ;
  alimenteChamp(_Donnees.indice,   CPTDOCU_INDICE_FIELD,   CPTDOCU_INDICE_LEN) ;
  alimenteChamp(_Donnees.patient,  CPTDOCU_PATIENT_FIELD,  CPTDOCU_PATIENT_LEN) ;
  alimenteChamp(_Donnees.document, CPTDOCU_DOCUMENT_FIELD, CPTDOCU_DOCUMENT_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :   	NSCompt::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSComptDocu::videFiche()
{
	videChamp(_Donnees.numcompt, CPTDOCU_NUMCOMPT_FIELD, CPTDOCU_NUMCOMPT_LEN) ;
  videChamp(_Donnees.indice,   CPTDOCU_INDICE_FIELD,   CPTDOCU_INDICE_LEN) ;
  videChamp(_Donnees.patient,  CPTDOCU_PATIENT_FIELD,  CPTDOCU_PATIENT_LEN) ;
  videChamp(_Donnees.document, CPTDOCU_DOCUMENT_FIELD, CPTDOCU_DOCUMENT_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCompt::getRecord()
//
//  Description : Prend l'enregistrement en cours et assigne la valeur des
//             	champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSComptDocu::getPatRecord()
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

//---------------------------------------------------------------------------
//  Fonction :  	NSCompt::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSComptDocu::open()
{
	char tableName[] = "NSM_CP_DOCU.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA) ;
	return(lastError) ;
}

//---------------------------------------------------------------------------//  Function:  NSCompt::Create()
//---------------------------------------------------------------------------
bool
NSComptDocu::Create()
{
	return true ;
}

//---------------------------------------------------------------------------//  Function:  NSCompt::Modify()
//---------------------------------------------------------------------------
bool
NSComptDocu::Modify()
{
	return true ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSComptDocu&
NSComptDocu::operator=(const NSComptDocu& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSComptDocu::operator == (const NSComptDocu& o)
{
	return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSComptInfo::NSComptInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSComptDocuInfo::NSComptDocuInfo()
{
}

//---------------------------------------------------------------------------//  Fonction:		NSComptInfo::NSComptInfo(NSCompt*)
//
//  Description:	Constructeur à partir d'un NSTransaction
//---------------------------------------------------------------------------
NSComptDocuInfo::NSComptDocuInfo(const NSComptDocu* pTransac)
{
  if (pTransac)
	  _Donnees = pTransac->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSComptDocuInfo::NSComptDocuInfo(const NSComptDocuInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSComptDocuInfo::~NSComptDocuInfo()
{
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSComptDocuInfo&
NSComptDocuInfo::operator=(const NSComptDocuInfo& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSComptDocuInfo::operator == (const NSComptDocuInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

//***************************************************************************// 					Implémentation des méthodes NSComptDocuArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSComptDocuArray::NSComptDocuArray(const NSComptDocuArray& rv) : NSComptDocuVector()
{
	if (false == rv.empty())
		for (NSComptDocuConstIter i = rv.begin() ; rv.end() != i ; i++)
   		push_back(new NSComptDocuInfo(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSComptDocuArray::vider()
{
	if (empty())
		return ;

	for (NSComptDocuIter i = begin(); end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSComptDocuArray::~NSComptDocuArray(){
	vider() ;
}

NSComptDocuArray&
NSComptDocuArray::operator=(const NSComptDocuArray& src)
{
  if (this == &src)
  	return *this ;

  vider() ;

  if (false == src.empty())
		for (NSComptDocuConstIter i = src.begin() ; src.end() != i ; i++)
   		push_back(new NSComptDocuInfo(*(*i))) ;

  return *this ;
}

//***************************************************************************
// Implémentation des méthodes NSTPayant
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSTPayantData::NSTPayantData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTPayantData::NSTPayantData(const NSTPayantData& rv)
{
	strcpy(numcompt, 	rv.numcompt);
	strcpy(numero,	 	rv.numero);
	strcpy(organisme,	rv.organisme);
	strcpy(libelle,		rv.libelle);
	strcpy(reste_du, 	rv.reste_du);
	strcpy(monnaie,  	rv.monnaie);
	strcpy(okpaye,	 	rv.okpaye);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSTPayantData::~NSTPayantData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSTPayantData&
NSTPayantData::operator=(const NSTPayantData& src)
{
  if (this == &src)
  	return *this ;

  strcpy(numcompt, 	src.numcompt) ;
  strcpy(numero,		src.numero) ;
  strcpy(organisme, src.organisme) ;
  strcpy(libelle,		src.libelle) ;
	strcpy(reste_du,  src.reste_du) ;
	strcpy(monnaie, 	src.monnaie) ;
  strcpy(okpaye,		src.okpaye) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSTPayantData::operator == ( const NSTPayantData& o )
{
	if ((strcmp(numcompt, o.numcompt) == 0) &&
   	 (strcmp(numero, o.numero) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSTPayantData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 	0, TPAY_NUMCOMPT_LEN + 1) ;
  memset(numero,		0, TPAY_NUMERO_LEN + 1) ;
	memset(organisme,	0, TPAY_ORGANISME_LEN + 1) ;
  memset(libelle,   0, TPAY_LIBELLE_LEN + 1) ;
	memset(reste_du,  0, TPAY_RESTE_DU_LEN + 1) ;
	memset(monnaie, 	0, TPAY_MONNAIE_LEN + 1) ;
  memset(okpaye,    0, TPAY_OKPAYE_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSTPayant::NSTPayant(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTPayant::NSTPayant(const NSTPayant& rv) : NSFiche(rv._pSuper)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSTPayant::~NSTPayant()
{
}

//---------------------------------------------------------------------------//  Fonction :  	NSTPayant::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSTPayant::alimenteFiche()
{
  alimenteChamp(_Donnees.numcompt,  TPAY_NUMCOMPT_FIELD,  TPAY_NUMCOMPT_LEN) ;
  alimenteChamp(_Donnees.numero,    TPAY_NUMERO_FIELD,    TPAY_NUMERO_LEN) ;
  alimenteChamp(_Donnees.organisme, TPAY_ORGANISME_FIELD, TPAY_ORGANISME_LEN) ;
  alimenteChamp(_Donnees.libelle,   TPAY_LIBELLE_FIELD,   TPAY_LIBELLE_LEN) ;
  alimenteChamp(_Donnees.reste_du,  TPAY_RESTE_DU_FIELD,  TPAY_RESTE_DU_LEN) ;
  alimenteChamp(_Donnees.monnaie,   TPAY_MONNAIE_FIELD,   TPAY_MONNAIE_LEN) ;
  alimenteChamp(_Donnees.okpaye,    TPAY_OKPAYE_FIELD,    TPAY_OKPAYE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :   	NSTPayant::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSTPayant::videFiche()
{
	videChamp(_Donnees.numcompt,  TPAY_NUMCOMPT_FIELD,  TPAY_NUMCOMPT_LEN) ;
  videChamp(_Donnees.numero,    TPAY_NUMERO_FIELD,    TPAY_NUMERO_LEN) ;
  videChamp(_Donnees.organisme, TPAY_ORGANISME_FIELD, TPAY_ORGANISME_LEN) ;
  videChamp(_Donnees.libelle,   TPAY_LIBELLE_FIELD,   TPAY_LIBELLE_LEN) ;
  videChamp(_Donnees.reste_du,  TPAY_RESTE_DU_FIELD,  TPAY_RESTE_DU_LEN) ;
  videChamp(_Donnees.monnaie,   TPAY_MONNAIE_FIELD,   TPAY_MONNAIE_LEN) ;
  videChamp(_Donnees.okpaye,    TPAY_OKPAYE_FIELD,    TPAY_OKPAYE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :  	NSTPayant::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSTPayant::open()
{
	char tableName[] = "TPAYANT.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSTPayant::Create()
//---------------------------------------------------------------------------
bool
NSTPayant::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSTPayant::Modify()
//---------------------------------------------------------------------------
bool
NSTPayant::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSTPayant&
NSTPayant::operator=(const NSTPayant& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSTPayant::operator == (const NSTPayant& o)
{
  return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSTPayantInfo::NSTPayantInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSTPayantInfo::NSTPayantInfo(){
}

//---------------------------------------------------------------------------
//  Fonction:		NSTPayantInfo::NSTPayantInfo(NSTPayant*)
//
//  Description:	Constructeur à partir d'un NSTPayant
//---------------------------------------------------------------------------
NSTPayantInfo::NSTPayantInfo(const NSTPayant* pTPayant)
{
  if (pTPayant)
	  _Donnees = pTPayant->_Donnees ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSTPayantInfo::NSTPayantInfo(const NSTPayantInfo& rv)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSTPayantInfo::~NSTPayantInfo()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSTPayantInfo&
NSTPayantInfo::operator=(const NSTPayantInfo& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSTPayantInfo::operator == (const NSTPayantInfo& o)
{
  return (_Donnees == o._Donnees) ;
}

//***************************************************************************// 					Implémentation des méthodes NSTPArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSTPArray::NSTPArray(const NSTPArray& rv) : NSTPayantArray()
{
  if (false == rv.empty())
	  for (NSTPConstIter i = rv.begin() ; rv.end() != i ; i++)
   	  push_back(new NSTPayantInfo(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSTPArray::vider()
{
  if (empty())
    return ;

  for (NSTPIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSTPArray::~NSTPArray(){
	vider() ;
}

NSTPArray&
NSTPArray::operator=(const NSTPArray& src)
{
  if (this == &src)
  	return *this ;

  vider() ;

  if (false == src.empty())
	  for (NSTPConstIter i = src.begin() ; src.end() != i ; i++)
   	  push_back(new NSTPayantInfo(*(*i))) ;

  return *this ;
}

//***************************************************************************// Implémentation des méthodes NSCodeOrga
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSCodeOrgaData::NSCodeOrgaData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie//---------------------------------------------------------------------------
NSCodeOrgaData::NSCodeOrgaData(const NSCodeOrgaData& rv)
{
  strcpy(code,			rv.code) ;
  strcpy(lib_court, rv.lib_court) ;
  strcpy(lib_long,	rv.lib_long) ;
  strcpy(adresse,		rv.adresse) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCodeOrgaData::~NSCodeOrgaData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSCodeOrgaData&
NSCodeOrgaData::operator=(const NSCodeOrgaData& src)
{
  if (this == &src)
  	return *this ;

  strcpy(code,			src.code) ;
  strcpy(lib_court, src.lib_court) ;
  strcpy(lib_long,	src.lib_long) ;
  strcpy(adresse,		src.adresse) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodeOrgaData::operator == ( const NSCodeOrgaData& o )
{
	if (strcmp(code, o.code) == 0)
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSCodeOrgaData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(code,		  0, ORGA_CODE_LEN + 1) ;
  memset(lib_court,	0, ORGA_LIB_COURT_LEN + 1) ;
  memset(lib_long,	0, ORGA_LIB_LONG_LEN + 1) ;
  memset(adresse,	  0, ORGA_ADRESSE_LEN + 1) ;
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSCodeOrga::NSCodeOrga(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodeOrga::NSCodeOrga(const NSCodeOrga& rv) : NSFiche(rv._pSuper)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCodeOrga::~NSCodeOrga()
{
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCodeOrga::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSCodeOrga::alimenteFiche()
{
  alimenteChamp(_Donnees.code,      ORGA_CODE_FIELD,      ORGA_CODE_LEN) ;
  alimenteChamp(_Donnees.lib_court, ORGA_LIB_COURT_FIELD, ORGA_LIB_COURT_LEN) ;
  alimenteChamp(_Donnees.lib_long,  ORGA_LIB_LONG_FIELD,  ORGA_LIB_LONG_LEN) ;
  alimenteChamp(_Donnees.adresse,   ORGA_ADRESSE_FIELD,   ORGA_ADRESSE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :   	NSCodeOrga::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCodeOrga::videFiche()
{
  videChamp(_Donnees.code,      ORGA_CODE_FIELD,      ORGA_CODE_LEN) ;
  videChamp(_Donnees.lib_court, ORGA_LIB_COURT_FIELD, ORGA_LIB_COURT_LEN) ;
  videChamp(_Donnees.lib_long,  ORGA_LIB_LONG_FIELD,  ORGA_LIB_LONG_LEN) ;
  videChamp(_Donnees.adresse,   ORGA_ADRESSE_FIELD,   ORGA_ADRESSE_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction :  	NSCodeOrga::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSCodeOrga::open()
{
	char tableName[] = "CODEORGA.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSCodeOrga::Create()
//---------------------------------------------------------------------------
bool
NSCodeOrga::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSCodeOrga::Modify()
//---------------------------------------------------------------------------
bool
NSCodeOrga::Modify()
{
	return true ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCodeOrga&
NSCodeOrga::operator=(const NSCodeOrga& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodeOrga::operator == (const NSCodeOrga& o)
{
	 return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCodeOrgaInfo::NSCodeOrgaInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSCodeOrgaInfo::NSCodeOrgaInfo()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSCodeOrgaInfo::NSCodeOrgaInfo(NSCodeOrga*)
//
//  Description:	Constructeur à partir d'un NSCodeOrga
//---------------------------------------------------------------------------
NSCodeOrgaInfo::NSCodeOrgaInfo(const NSCodeOrga* pCodeOrga)
{
  if (pCodeOrga)
    _Donnees = pCodeOrga->_Donnees ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodeOrgaInfo::NSCodeOrgaInfo(const NSCodeOrgaInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSCodeOrgaInfo::~NSCodeOrgaInfo()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCodeOrgaInfo&
NSCodeOrgaInfo::operator=(const NSCodeOrgaInfo& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodeOrgaInfo::operator == (const NSCodeOrgaInfo& o)
{
	 return (_Donnees == o._Donnees) ;
}

//***************************************************************************
// 					Implémentation des méthodes NSCodeOrgaArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodeOrgaArray::NSCodeOrgaArray(const NSCodeOrgaArray& rv) : NSOrganArray()
{
  if (rv.empty())
	  for (NSCodeOrgaConstIter i = rv.begin() ; rv.end() != i ; i++)
   	  push_back(new NSCodeOrgaInfo(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSCodeOrgaArray::vider()
{
  if (empty())
    return ;

	for (NSCodeOrgaIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSCodeOrgaArray::~NSCodeOrgaArray(){
	vider() ;
}

NSCodeOrgaArray&
NSCodeOrgaArray::operator=(const NSCodeOrgaArray& src)
{
  if (this == &src)
  	return *this ;

  vider() ;

  if (src.empty())
	  for (NSCodeOrgaConstIter i = src.begin() ; src.end() != i ; i++)
   	  push_back(new NSCodeOrgaInfo(*(*i))) ;

  return *this ;
}

//***************************************************************************// Implémentation des méthodes NSCodeRegime
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSCodeRegimeData::NSCodeRegimeData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie//---------------------------------------------------------------------------
NSCodeRegimeData::NSCodeRegimeData(const NSCodeRegimeData& rv)
{
  strcpy(code,         rv.code) ;
  strcpy(lib_court,    rv.lib_court) ;
  strcpy(lib_long,     rv.lib_long) ;
  strcpy(destinataire, rv.destinataire) ;
  strcpy(centre_info,  rv.centre_info) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCodeRegimeData::~NSCodeRegimeData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSCodeRegimeData&
NSCodeRegimeData::operator=(const NSCodeRegimeData& src)
{
  if (this == &src)
  	return *this ;

  strcpy(code,			   src.code) ;
  strcpy(lib_court,		 src.lib_court) ;
  strcpy(lib_long,		 src.lib_long) ;
  strcpy(destinataire, src.destinataire) ;
  strcpy(centre_info,	 src.centre_info) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodeRegimeData::operator == ( const NSCodeRegimeData& o )
{
	if (strcmp(code, o.code) == 0)
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSCodeRegimeData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(code,			   0, REG_CODE_LEN + 1);
  memset(lib_court,		 0, REG_LIB_COURT_LEN + 1);
  memset(lib_long,		 0, REG_LIB_LONG_LEN + 1);
  memset(destinataire, 0, REG_DESTINATAIRE_LEN + 1);
  memset(centre_info,	 0, REG_CENTRE_INFO_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSCodeRegime::NSCodeRegime(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodeRegime::NSCodeRegime(const NSCodeRegime& rv) : NSFiche(rv._pSuper)
{
	_Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSCodeRegime::~NSCodeRegime()
{
}

//---------------------------------------------------------------------------//  Fonction :  	NSCodeRegime::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSCodeRegime::alimenteFiche()
{
  alimenteChamp(_Donnees.code,         REG_CODE_FIELD,         REG_CODE_LEN) ;
  alimenteChamp(_Donnees.lib_court,    REG_LIB_COURT_FIELD,    REG_LIB_COURT_LEN) ;
  alimenteChamp(_Donnees.lib_long,     REG_LIB_LONG_FIELD,     REG_LIB_LONG_LEN) ;
  alimenteChamp(_Donnees.destinataire, REG_DESTINATAIRE_FIELD, REG_DESTINATAIRE_LEN) ;
  alimenteChamp(_Donnees.centre_info,  REG_CENTRE_INFO_FIELD,  REG_CENTRE_INFO_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :   	NSCodeRegime::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCodeRegime::videFiche()
{
  videChamp(_Donnees.code,         REG_CODE_FIELD,         REG_CODE_LEN) ;
  videChamp(_Donnees.lib_court,    REG_LIB_COURT_FIELD,    REG_LIB_COURT_LEN) ;
  videChamp(_Donnees.lib_long,     REG_LIB_LONG_FIELD,     REG_LIB_LONG_LEN) ;
  videChamp(_Donnees.destinataire, REG_DESTINATAIRE_FIELD, REG_DESTINATAIRE_LEN) ;
  videChamp(_Donnees.centre_info,  REG_CENTRE_INFO_FIELD,  REG_CENTRE_INFO_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSCodeRegime::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSCodeRegime::open()
{
	char tableName[] = "REGIMES.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSCodeRegime::Create()
//---------------------------------------------------------------------------
bool
NSCodeRegime::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSCodeRegime::Modify()
//---------------------------------------------------------------------------
bool
NSCodeRegime::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCodeRegime&
NSCodeRegime::operator=(const NSCodeRegime& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodeRegime::operator == (const NSCodeRegime& o)
{
  return (_Donnees == o._Donnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCodeRegimeInfo::NSCodeRegimeInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSCodeRegimeInfo::NSCodeRegimeInfo()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSCodeRegimeInfo::NSCodeRegimeInfo(NSCodeRegime*)
//
//  Description:	Constructeur à partir d'un NSCodeRegime
//---------------------------------------------------------------------------
NSCodeRegimeInfo::NSCodeRegimeInfo(const NSCodeRegime* pCodeRegime)
{
  if (pCodeRegime)
    _Donnees = pCodeRegime->_Donnees ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodeRegimeInfo::NSCodeRegimeInfo(const NSCodeRegimeInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCodeRegimeInfo::~NSCodeRegimeInfo()
{
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCodeRegimeInfo&
NSCodeRegimeInfo::operator=(const NSCodeRegimeInfo& src)
{
  if (this == &src)
  	return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCodeRegimeInfo::operator == (const NSCodeRegimeInfo& o)
{
  return (_Donnees == o._Donnees) ;
}

//***************************************************************************
// 					Implémentation des méthodes NSCodeRegimeArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCodeRegimeArray::NSCodeRegimeArray(const NSCodeRegimeArray& rv) : NSRegimeArray()
{
  if (false == rv.empty())
	  for (NSCodeRegimeConstIter i = rv.begin() ; rv.end() != i ; i++)
   	  push_back(new NSCodeRegimeInfo(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSCodeRegimeArray::vider()
{
  if (empty())
    return ;

	for (NSCodeRegimeIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSCodeRegimeArray::~NSCodeRegimeArray()
{
	vider() ;
}

NSCodeRegimeArray&
NSCodeRegimeArray::operator=(const NSCodeRegimeArray& src)
{
  if (this == &src)
  	return *this ;

  vider() ;

  if (false == src.empty())
	  for (NSCodeRegimeConstIter i = src.begin() ; src.end() != i ; i++)
   	  push_back(new NSCodeRegimeInfo(*(*i))) ;

  return *this ;
}

//////////////////////////// fin de nscpta.cpp
