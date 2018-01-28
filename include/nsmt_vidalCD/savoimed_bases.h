// -----------------------------------------------------------------------------// savoimed_bases.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: fabrice $
// $Date: 2003/07/10 09:51:55 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------
// Définition des objets de base des liens entre les medicaments
// -----------------------------------------------------------------------------
#ifndef __SAVOIMEDBASES_H__
# define __SAVOIMEDBASES_H__

# include "ns_sgbd\nsfiche.h"
// Taille des champs du fichier SAVOIMED
# define SAVOIMED_CODE_LEN  				6
# define SAVOIMED_QUALIFIE_LEN    	6
# define SAVOIMED_LIEN_LEN      		2
# define SAVOIMED_QUALIFIANT_LEN  	6
# define SAVOIMED_DEGRE_LEN   			5
# define SAVOIMED_CLASSE_LEN      	3
# define SAVOIMED_SCENARIO_LEN			6

// Indice des champs du fichier SAVOIMED
# define SAVOIMED_CODE_FIELD  			1
# define SAVOIMED_QUALIFIE_FIELD    2
# define SAVOIMED_LIEN_FIELD      	3
# define SAVOIMED_QUALIFIANT_FIELD  4
# define SAVOIMED_DEGRE_FIELD   		5
# define SAVOIMED_CLASSE_FIELD      6
# define SAVOIMED_SCENARIO_FIELD		7

// **************************** Table SAVOIMED.DB ****************************
//---------------------------------------------------------------------------//  Classe NSSavoiMed
//
// Objet contenant les données
//---------------------------------------------------------------------------
//

class NSSavoirMedData{
 public:
	char 				code			[SAVOIMED_CODE_LEN 				+ 1] ;
	char				qualifie	[SAVOIMED_QUALIFIE_LEN 		+ 1] ;
	char 				lien			[SAVOIMED_LIEN_LEN				+ 1] ;
	char 				qualifiant[SAVOIMED_QUALIFIANT_LEN	+ 1] ;
	char 				degre			[SAVOIMED_DEGRE_LEN				+ 1] ;
	char				classe    [SAVOIMED_CLASSE_LEN			+ 1] ;
	char				scenario  [SAVOIMED_SCENARIO_LEN		+ 1] ;

	NSSavoirMedData() ;	NSSavoirMedData(NSSavoirMedData& rv) ;
	~NSSavoirMedData() ;

	NSSavoirMedData&	operator=(NSSavoirMedData src) ;
	int 	    				operator==(const NSSavoirMedData& o) ;

	void							metAZero() ;
} ;


class NSSavoirMed ;

//---------------------------------------------------------------------------//  Classe NSBdmInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSSavoirMedInfo
{
 public:
	NSSavoirMedData		*pDonnees ;

	NSSavoirMedInfo() ;
	NSSavoirMedInfo(NSSavoirMed	*) ;
	NSSavoirMedInfo(NSSavoirMedInfo& rv) ;
	~NSSavoirMedInfo() ;

	NSSavoirMedInfo&	operator=(NSSavoirMedInfo src) ;
	int 	    				operator==(const NSSavoirMedInfo& o) ;
} ;

//---------------------------------------------------------------------------
//  Classe NSBdm
//
// Objet dérivé de NSFiche servant aux accès de base de données
//---------------------------------------------------------------------------
//
class NSSavoirMed : public NSSavoirMedInfo, public NSFiche
{
 public:
	NSSavoirMed(NSContexte* pCtx) ;
	NSSavoirMed(NSSavoirMed& rv) ;
	~NSSavoirMed() ;

	void      				alimenteFiche() ;
	void      				videFiche() ;
	DBIResult					open() ;

	NSSavoirMed&    	operator=(NSSavoirMed src) ;
	int 	    				operator==(const NSSavoirMed& o) ;
} ;

//
// Définition de NSBdmArray (Array de NSBdmInfo(s))
//

typedef vector<NSSavoirMedInfo *> NSSavoimedArray ;
typedef NSSavoimedArray::iterator NSSavoirMedIter ;

class NSSavoirMedArray : public NSSavoimedArray
{
 public:
	// Constructeurs
	NSSavoirMedArray() : NSSavoimedArray() {}
	NSSavoirMedArray(NSSavoirMedArray& rv) ;

	// Destructeur
	virtual						~NSSavoirMedArray() ;
	void							vider() ;

	NSSavoirMedArray&	operator=(NSSavoirMedArray src) ;
} ;

#endif    // __SAVOIMEDBASES_H__

