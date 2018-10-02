//---------------------------------------------------------------------------//    NSANNEXE.CPP
//    PA   juillet 94
//  Implémentation des objets annexes de NAUTILUS
//---------------------------------------------------------------------------
// #include <cstring.h>
// #include <windows.h>
// #include <mem.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
#else
  #include "nautilus\nssuper.h"
  #include "nautilus\nspatdlg.h"
#endif

#include "ns_sgbd\nsannexe.h"
// #include "nautilus\nsresour.h"

//***************************************************************************//
// Implémentation des méthodes NSTypeMime
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:  NSTypeMimeData::NSTypeMimeData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSTypeMimeData::NSTypeMimeData()
{
  //
  // Met les champs de données à zéro
  //
  metAZero() ;
}

//---------------------------------------------------------------------------//  Fonction:		NSTypeMimeData::NSTypeMimeData(NSTypeMimeData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTypeMimeData::NSTypeMimeData(const NSTypeMimeData& rv)
{
  _sExtension = rv._sExtension ;
  _sLibelle   = rv._sLibelle ;
  _sMime      = rv._sMime ;
  _sType      = rv._sType ;
}

//---------------------------------------------------------------------------//  Fonction:		NSTypeMimeData::operator=(NSTypeMimeData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSTypeMimeData&
NSTypeMimeData::operator=(const NSTypeMimeData& src)
{
  if (&src == this)
		return *this ;

  _sExtension = src._sExtension ;
  _sLibelle   = src._sLibelle ;
  _sMime      = src._sMime ;
  _sType      = src._sType ;

	return *this ;
}

//---------------------------------------------------------------------------//  Fonction:		NSTypeMimeData::operator==(NSTypeMimeData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSTypeMimeData::operator==(const NSTypeMimeData& o) const
{
  if ((_sExtension == o._sExtension) &&
      (_sLibelle   == o._sLibelle)   &&
      (_sMime      == o._sMime)      &&
      (_sType      == o._sType))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Function:		NSTypeMimeData::metAZero()
//  Description:	Initialise les champs de données à zéro.
//  Retour:			Aucun
//---------------------------------------------------------------------------
void
NSTypeMimeData::metAZero()
{
  _sExtension = string("") ;
  _sLibelle   = string("") ;
  _sMime      = string("") ;
  _sType      = string("") ;
}

#ifndef _MUE

//---------------------------------------------------------------------------//  Function:  NSTypeMime::NSTypeMime()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSTypeMime::NSTypeMime(NSSuper* pSuper) : NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------//  Function:  NSTypeMime::~NSTypeMime()
//
//  Description: Destructeur.
//---------------------------------------------------------------------------
NSTypeMime::~NSTypeMime()
{
}

//---------------------------------------------------------------------------
//  Function:  NSTypeMime::alimenteFiche()
//
//  Arguments:
//
//  Description:
//             Transfère le contenu de pRecBuff dans les variables de
//             la fiche
//  Returns:
//             Rien
//---------------------------------------------------------------------------
void NSTypeMime::alimenteFiche()
{
  char extension[MIM_EXTENSION_LEN + 1] ;
  char libelle[MIM_LIBELLE_LEN + 1] ;
  char mime[MIM_MIME_LEN + 1] ;
  char type[MIM_TYPE_LEN + 1] ;

  alimenteChamp(extension, MIM_EXTENSION_FIELD,	MIM_EXTENSION_LEN) ;
  alimenteChamp(libelle, 	 MIM_LIBELLE_FIELD,		MIM_LIBELLE_LEN) ;
  alimenteChamp(mime, 		 MIM_MIME_FIELD,			MIM_MIME_LEN) ;
  alimenteChamp(type, 		 MIM_TYPE_FIELD,			MIM_TYPE_LEN) ;

  _Donnees.setExtension(string(extension)) ;
  _Donnees.setLibelle(string(libelle)) ;
  _Donnees.setMime(string(mime)) ;
  _Donnees.setType(string(type)) ;
}

//---------------------------------------------------------------------------//  Function:  NSTypeMime::videFiche()
//
//  Arguments:
//
//  Description:
//             Transfère le contenu des valeurs de la fiche dans pRecBuff
//
//  Returns:
//             Rien
//---------------------------------------------------------------------------
void NSTypeMime::videFiche()
{
  char extension[MIM_EXTENSION_LEN + 1] ;
  char libelle[MIM_LIBELLE_LEN + 1] ;
  char mime[MIM_MIME_LEN + 1] ;
  char type[MIM_TYPE_LEN + 1] ;

  strcpy(extension, _Donnees.getExtension().c_str()) ;
  strcpy(libelle,   _Donnees.getLibelle().c_str()) ;
  strcpy(mime,      _Donnees.getMime().c_str()) ;
  strcpy(type,      _Donnees.getType().c_str()) ;

  videChamp(extension, MIM_EXTENSION_FIELD,	MIM_EXTENSION_LEN) ;
  videChamp(libelle, 	 MIM_LIBELLE_FIELD,		MIM_LIBELLE_LEN) ;
  videChamp(mime, 		 MIM_MIME_FIELD,			MIM_MIME_LEN) ;
  videChamp(type, 		 MIM_TYPE_FIELD,			MIM_TYPE_LEN) ;
}

//---------------------------------------------------------------------------//  Function:  NSTypeMime::open()
//
//  Arguments:
//
//  Description: Ouvre la table
//
//  Returns:
//             PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult NSTypeMime::open()
{
  char tableName[] = "TYPEMIME.DB" ;
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PERSONNEL) ;
  return(lastError) ;
}

//---------------------------------------------------------------------------
//  Function:  NSTypeMime::chercheTypeMimeInfo(char* fichExt, NSTypeMimeInfo* pTypeMimeInfo)
//
//  Arguments: Une clé extension et un pointeur sur la structure à remplir
//
//  Description: Remplit le NSTypeMimeInfo correspondant à la clé extension
//						Si la base est déjà ouverte, ne l'ouvre pas.
//
//  Returns: true->OK	false->Echec
//
//---------------------------------------------------------------------------
bool
NSTypeMime::chercheTypeMimeInfo(string sFichExt, NSTypeMimeInfo* pTypeMimeInfo)
{
  bool bOpenInside = false ;
  char cFichExt[100] ;

	//
	// Ouverture de TypeMime.db
	//
  if (!isOpen)
  {
    lastError = open() ;
		if (lastError != DBIERR_NONE)
		{
			erreur("Erreur à l'ouverture de la base TypeMime.db", standardError, lastError) ;
			return false ;		}
    bOpenInside = true ;
  }

  // La recherche n'est pas case-sensitive
  strcpy(cFichExt, sFichExt.c_str()) ;
  CharLower(cFichExt) ;
  sFichExt = string(cFichExt) ;

  // Recherche de l'extension choisie
  //
  lastError = chercheClef(&sFichExt,
   									      "",
                          0,
                          keySEARCHEQ,
                          dbiWRITELOCK) ;

	if (lastError == DBIERR_NONE)
		lastError = getRecord();

	if (lastError != DBIERR_NONE)
	{
    // Note : on ne signale plus cette erreur à cause de son apparition
    // fréquente dans la procédure d'importation des images
    // if (lastError != DBIERR_EOF)
		//  	erreur("Erreur à la recherche de l'extension dans TypeMime.db.", 0, lastError);
    if (bOpenInside)
      close() ;
		return false ;
	}
  else
    *pTypeMimeInfo = NSTypeMimeInfo(this) ;

  if (bOpenInside)
  {
		lastError = close() ;
   	if (lastError != DBIERR_NONE)
    {
      erreur("Erreur de fermeture de la base TypeMime.db.", standardError, lastError);
      return false ;
    }
  }

	return true ;
}

#endif // #ifndef _MUE

//***************************************************************************//
// Implémentation des méthodes NSTypeMimeInfo
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Fonction:		NSTypeMimeInfo::NSTypeMimeInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTypeMimeInfo::NSTypeMimeInfo()
{
}

//---------------------------------------------------------------------------//  Fonction:		NSTypeMimeInfo::NSTypeMimeInfo(NSTypeMime*)
//  Description:	Constructeur à partir d'un NSTypeMime
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTypeMimeInfo::NSTypeMimeInfo(NSTypeMime* pTypeMime)
{
	// *pDonnees = *(pTypeMime->pDonnees) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSTypeMimeInfo::~NSTypeMimeInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTypeMimeInfo::~NSTypeMimeInfo()
{
}

//---------------------------------------------------------------------------//  Fonction:		NSTypeMimeInfo::NSTypeMimeInfo(NSTypeMimeInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSTypeMimeInfo::NSTypeMimeInfo(const NSTypeMimeInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------//  Fonction:		NSTypeMimeInfo::operator=(NSTypeMimeInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSTypeMimeInfo&
NSTypeMimeInfo::operator=(const NSTypeMimeInfo& src)
{
  if (&src == this)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Fonction:		NSTypeMimeInfo::operator==(NSTypeMimeInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int
NSTypeMimeInfo::operator==(const NSTypeMimeInfo& o) const
{
	 return (_Donnees == o._Donnees) ;
}

// fin de NSAnnexe.cpp
///////////////////////////////////////////////////////////////////////////////

