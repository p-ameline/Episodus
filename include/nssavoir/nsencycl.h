//---------------------------------------------------------------------------//     NSPERSON.H
//
//  Contient les définitions des objets PATIENT et UTILISATEUR de NAUTILUS
//
//  1ere version : KRZISCH Ph. janvier 92   Dernière modif : 12/08/93
//---------------------------------------------------------------------------
#ifndef __NSENCYCL_H
#define __NSENCYCL_H

#include <cstring.h>
#include "ns_sgbd\nsfiche.h"

#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

//
// Taille des champs du fichier ENCYCLOPEDIE
//
#define ENCYCLO_URL_LEN   			  200
#define ENCYCLO_LEXIQUE_LEN        6

//
// Indice des champs du fichier ENCYCLOPEDIE
//
#define ENCYCLO_LEXIQUE_FIELD  1
#define ENCYCLO_URL_FIELD   	 2

//---------------------------------------------------------------------------
//  Classe NSEncyclo
//---------------------------------------------------------------------------

//// Objet contenant les données//
class _CLASSELEXI NSEncycloData
{
  public :
	  //
	  // Variables de stockage des valeurs
	  //
	  char lexique[ENCYCLO_LEXIQUE_LEN + 1] ;
	  char url[ENCYCLO_URL_LEN + 1] ;

	  NSEncycloData() ;
	  NSEncycloData(const NSEncycloData& rv) ;
     ~NSEncycloData() ;

    string getLexique() { return string(lexique) ; }
    string getUrl()     { return string(url) ; }

	  NSEncycloData& operator=(const NSEncycloData& src) ;
	  int 			  	 operator==(const NSEncycloData& o) ;
};

//// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSEncyclo : public NSFiche
{
  public :

	  //	  // Objet qui contient les données	  //
	  NSEncycloData Donnees ;

	  NSEncyclo(NSSuper* pSuper) ;
    NSEncyclo(const NSEncyclo& rv) ;
	  ~NSEncyclo();

	  void alimenteFiche() ;
	  void videFiche() ;
	  DBIResult open() ;
	  DBIResult getPatRecord() ;

	  virtual bool Create() ;
	  virtual bool Modify() ;

    string getLexique() { return Donnees.getLexique() ; }
    string getUrl()     { return Donnees.getUrl() ; }

    NSEncyclo& operator=(const NSEncyclo& src) ;
    int        operator==(const NSEncyclo& o) ;
};

//---------------------------------------------------------------------------
//  Classe NSPatInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSEncycloInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSEncycloData Donnees ;

		NSEncycloInfo() ;
		NSEncycloInfo(const NSEncyclo*) ;
		NSEncycloInfo(const NSEncycloInfo& rv) ;
    ~NSEncycloInfo() ;

    string getLexique() { return Donnees.getLexique() ; }
    string getUrl()     { return Donnees.getUrl() ; }

		NSEncycloInfo& operator=(const NSEncycloInfo& src) ;
		int            operator==(const NSEncycloInfo& o) ;
};

#endif

