//---------------------------------------------------------------------------//    NSPATLNK.CPP
//    PA   janvier 2002
//  Liens entre noeuds
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "partage\nsdivfct.h"
#endif


#include "nssavoir\nspatlnk.h"

//***************************************************************************
// Implémentation des méthodes NSPatLinkData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPatLinkData::NSPatLinkData()
{
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Met les champs de données à zéro
//---------------------------------------------------------------------------
void
NSPatLinkData::metAZero()
{
/*
   memset(qualifie,     0, PATLINK_QUALIFIE_LEN + 1);
   memset(lien,         0, PATLINK_LIEN_LEN + 1);
   memset(lien_level,   0, PATLINK_LIEN_LEVEL_LEN + 1);
   memset(qualifiant,   0, PATLINK_QUALIFIANT_LEN + 1);
   memset(tranNew, 		0, PATLINK_TRAN_NEW_LEN + 1);
   memset(tranDel,      0, PATLINK_TRAN_DEL_LEN + 1);
   memset(cause,        0, PATLINK_CAUSE_LEN + 1);
*/

	sQualifie   = string("") ;
	sLien       = string("") ;
	sLienLevel  = string("") ;
	sQualifiant = string("") ;
	sCause      = string("") ;
}


//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatLinkData::NSPatLinkData(const NSPatLinkData& rv)
{
/*
	strcpy(qualifie,    rv.qualifie);
	strcpy(lien, 		rv.lien);
    strcpy(lien_level, 	rv.lien_level);
	strcpy(qualifiant,  rv.qualifiant);
    strcpy(tranNew, 	rv.tranNew);
	strcpy(tranDel, 	rv.tranDel);
	strcpy(cause, 		rv.cause);
*/

	sQualifie   = rv.sQualifie ;
	sLien       = rv.sLien ;
	sLienLevel  = rv.sLienLevel ;
	sQualifiant = rv.sQualifiant ;
	sCause      = rv.sCause ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPatLinkData::~NSPatLinkData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSPatLinkData&
NSPatLinkData::operator=(const NSPatLinkData& rv)
{
	if (this == &rv)
		return *this ;

/*
	strcpy(qualifie,    rv.qualifie);
	strcpy(lien, 		rv.lien);
	strcpy(lien_level, 	rv.lien_level);
	strcpy(qualifiant,  rv.qualifiant);
    strcpy(tranNew, 	rv.tranNew);
	strcpy(tranDel, 	rv.tranDel);
	strcpy(cause, 		rv.cause);
*/

	sQualifie   = rv.sQualifie ;
	sLien       = rv.sLien ;
	sLienLevel  = rv.sLienLevel ;
	sQualifiant = rv.sQualifiant ;
	sCause      = rv.sCause ;

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPatLinkData::operator == (const NSPatLinkData& o)
{
/*
	if ((strcmp(qualifie,   o.qualifie)     == 0) &&
        (strcmp(lien, 		o.lien) 		== 0) &&
        (strcmp(lien_level, o.lien_level)   == 0) &&
        (strcmp(qualifiant, o.qualifiant)   == 0) &&
        (strcmp(tranNew,    o.tranNew) 	    == 0) &&
        (strcmp(tranDel, 	o.tranDel) 	    == 0) &&
        (strcmp(cause, 	    o.cause) 		== 0)
       )
*/
	if ((sQualifie   == o.sQualifie)   &&
	    (sLien       == o.sLien)       &&
      (sLienLevel  == o.sLienLevel)  &&
	    (sQualifiant == o.sQualifiant) &&
	    (sCause      == o.sCause))
		return 1;
	else
		return 0;
}

//***************************************************************************// Implémentation des méthodes NSPatLink
//***************************************************************************

/*
//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPatLink::NSPatLink(NSContexte* pCtx, int iType) : NSPatLinkInfo(), NSFiche(pCtx)
{
    iTypeLink = iType;
}

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSPatLink::~NSPatLink()
{
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSPatLink::alimenteFiche()
{
    alimenteChamp(pDonnees->qualifie,   PATLINK_QUALIFIE_FIELD, 	PATLINK_QUALIFIE_LEN);
    alimenteChamp(pDonnees->lien, 	    PATLINK_LIEN_FIELD, 		PATLINK_LIEN_LEN);
    alimenteChamp(pDonnees->qualifiant, PATLINK_QUALIFIANT_FIELD,   PATLINK_QUALIFIANT_LEN);
    alimenteChamp(pDonnees->tranNew,    PATLINK_TRAN_NEW_FIELD, 	PATLINK_TRAN_NEW_LEN);
    alimenteChamp(pDonnees->tranDel,    PATLINK_TRAN_DEL_FIELD, 	PATLINK_TRAN_DEL_LEN);
    alimenteChamp(pDonnees->cause, 		PATLINK_CAUSE_FIELD, 		PATLINK_CAUSE_LEN);
#ifdef _MUE
    alimenteChamp(pDonnees->lien_level, PATLINK_LIEN_LEVEL_FIELD,   PATLINK_LIEN_LEVEL_LEN);
#endif
}

//---------------------------------------------------------------------------//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSPatLink::videFiche()
{
    videChamp(pDonnees->qualifie,   PATLINK_QUALIFIE_FIELD, 	PATLINK_QUALIFIE_LEN);
    videChamp(pDonnees->lien, 	    PATLINK_LIEN_FIELD, 		PATLINK_LIEN_LEN);
    videChamp(pDonnees->qualifiant, PATLINK_QUALIFIANT_FIELD,   PATLINK_QUALIFIANT_LEN);
    videChamp(pDonnees->tranNew,    PATLINK_TRAN_NEW_FIELD, 	PATLINK_TRAN_NEW_LEN);
    videChamp(pDonnees->tranDel,    PATLINK_TRAN_DEL_FIELD, 	PATLINK_TRAN_DEL_LEN);
    videChamp(pDonnees->cause, 		PATLINK_CAUSE_FIELD, 		PATLINK_CAUSE_LEN);
#ifdef _MUE
    videChamp(pDonnees->lien_level, PATLINK_LIEN_LEVEL_FIELD,   PATLINK_LIEN_LEVEL_LEN);
#endif
}

//---------------------------------------------------------------------------
//  Ouvre la table
//---------------------------------------------------------------------------
DBIResult
NSPatLink::open()
{
#ifndef _MUE
    char tableName[] = "NSPATLNK.DB";
#else    char tableName[255];
    if (iTypeLink == graphPerson)        strcpy(tableName, "NSMLNK.DB");    else        strcpy(tableName, "NSMLNKOBJ.DB");
#endif
    // Appelle la fonction open() de la classe de base
    lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
    return(lastError);
}
*/

//***************************************************************************
// Implémentation des méthodes NSPatLinkInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSPatLinkInfo::NSPatLinkInfo()
{
try
{
	_isDirty  = false ;
}
catch (...)
{
	erreur("Exception NSPatLinkInfo ctor.", standardError) ;
}
}

/*
//---------------------------------------------------------------------------
//  Constructeur à partir d'une NSChemise
//---------------------------------------------------------------------------
NSPatLinkInfo::NSPatLinkInfo(NSPatLink* pPatLink)
{
try
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSPatLinkData() ;
	isDirty  = false ;
	//
	// Initialise les données à partir de celles de la source
	//
	*pDonnees = *(pPatLink->pDonnees) ;
}
catch (...)
{
	erreur("Exception NSPatLinkInfo ctor(NSPatlink*).", standardError, 0);
}
}
*/

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSPatLinkInfo::~NSPatLinkInfo()
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatLinkInfo::NSPatLinkInfo(const NSPatLinkInfo& rv)
{
try
{
	//
	// Initialise les données à partir de celles de la source
	//
	_Donnees = rv._Donnees ;
	_isDirty = rv._isDirty ;
}
catch (...)
{
	erreur("Exception NSPatLinkInfo copy ctor.", standardError) ;
}
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPatLinkInfo&
NSPatLinkInfo::operator=(const NSPatLinkInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;
	_isDirty = src._isDirty ;

	return *this ;
}

int
NSPatLinkInfo::operator==(const NSPatLinkInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

// --------------------------------------------------------------------------
// ------------------------ METHODES DE NSPatLinkArray ----------------------
// --------------------------------------------------------------------------

NSPatLinkArray::NSPatLinkArray(const NSPatLinkArray& rv)
               :NSPatLkArray()
{
try
{
	if (false == rv.empty())
		for (NSPatLinkConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSPatLinkInfo(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSPatLinkArray copy ctor.", standardError) ;
}
}

voidNSPatLinkArray::vider()
{
	if (empty())
		return ;

	for (NSPatLinkIter i = begin(); i != end(); )
	{
		delete *i ;
    erase(i) ;
	}
}

NSPatLinkArray::~NSPatLinkArray()
{
	vider();
}

NSPatLinkArray&
NSPatLinkArray::operator=(const NSPatLinkArray& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())		for (NSPatLinkConstIter i = src.begin(); src.end() != i ; i++)
			push_back(new NSPatLinkInfo(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSPatLinkArray (= operator).", standardError) ;
	return *this ;
}
}

int
NSPatLinkArray::operator==(const NSPatLinkArray& o)
{
try
{
  if (empty())  {    if (o.empty())      return 1 ;    else      return 0 ;  }  NSPatLinkConstIter i = begin() ;  NSPatLinkConstIter j = o.begin() ;  int egal = 1 ;  while ((end() != i) && (o.end() != j))  {    if (!((*(*i)) == (*(*j))))    {      egal = 0 ;      break ;    }    i++ ;    j++ ;  }  if (egal)  {    if ((end() != i) || (o.end() != j))      egal = 0 ;  }  return egal ;
}
catch (...)
{
	erreur("Exception NSPatLinkArray (== operator).", standardError) ;
	return 0 ;
}
}

// fin de nspatlnk.cpp
/////////////////////////////////////////////////

