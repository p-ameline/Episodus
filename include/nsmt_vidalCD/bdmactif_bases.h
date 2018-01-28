// -----------------------------------------------------------------------------
// bdmactif_bases.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: fabrice $
// $Date: 2003/07/10 09:51:55 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------
// Définition des objets de base de donnée de base medicamenteuse
// -----------------------------------------------------------------------------
#ifndef __BDMACTIFBASES_H__
# define __BDMACTIFBASES_H__

# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

# include "ns_sgbd\nsfiche.h"

// Taille des champs du fichier BDMACTIF
# define BDMACTIF_LIBELLE_LEN    255   // Code CIP
# define BDMACTIF_LEXIQCODE_LEN    6   // Code ancien Leximed


// Indice des champs du fichier BDMACTIF
# define BDMACTIF_LIBELLE_FIELD		1
# define BDMACTIF_LEXIQCODE_FIELD	2

// **************************** Table BDMACTIF.DB ****************************
//---------------------------------------------------------------------------
//  Classe NSBdmActifData
//
// Objet contenant les données
//---------------------------------------------------------------------------
//

class NSBdmActifData
{
 public:
	char				libelle[BDMACTIF_LIBELLE_LEN + 1] ;
	char 				code[BDMACTIF_LEXIQCODE_LEN + 1] ;

	NSBdmActifData() ;
	NSBdmActifData(NSBdmActifData& rv) ;
	~NSBdmActifData() ;

	NSBdmActifData&	operator=(NSBdmActifData src) ;
	int 	    	operator==(const NSBdmActifData& o) ;

	void				metAZero() ;
};


class NSBdmActif ;

//---------------------------------------------------------------------------
//  Classe NSBdmActifInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSBdmActifInfo
{
 public:
	NSBdmActifData		*pDonnees ;

	NSBdmActifInfo() ;
	NSBdmActifInfo(NSBdmActif*) ;
	NSBdmActifInfo(NSBdmActifInfo& rv) ;
	~NSBdmActifInfo() ;

	NSBdmActifInfo&	operator=(NSBdmActifInfo src) ;
	int 	    	operator==(const NSBdmActifInfo& o) ;
};

//---------------------------------------------------------------------------
//  Classe NSBdmActif
//
// Objet dérivé de NSFiche servant aux accès de base de données
//---------------------------------------------------------------------------
//
class NSBdmActif : public NSBdmActifInfo, public NSFiche
{
 public:
	NSBdmActif(NSContexte* pCtx) ;
	NSBdmActif(NSBdmActif& rv) ;
	~NSBdmActif() ;

	void      alimenteFiche() ;
	void      videFiche() ;
	DBIResult	open() ;

	NSBdmActif&    operator=(NSBdmActif src) ;
  int 	    operator==(const NSBdmActif& o) ;
} ;

//
// Définition de NSBdmActifArray (Array de NSBdmActifInfo(s))
//

typedef vector<NSBdmActifInfo *> NSBMActifArray ;
typedef NSBMActifArray::iterator NSBdmActifIter ;

class NSBdmActifArray : public NSBMActifArray
{
 public:
	// Constructeurs
	NSBdmActifArray() : NSBMActifArray() {}
	NSBdmActifArray(NSBdmActifArray& rv) ;
	// Destructeur
	virtual			~NSBdmActifArray() ;
	void				vider() ;

	NSBdmActifArray&	operator=(NSBdmActifArray src) ;
} ;

//---------------------------------------------------------------------------
#endif    // __BDMACTIFBASES_H__
