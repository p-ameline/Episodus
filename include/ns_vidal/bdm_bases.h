// -----------------------------------------------------------------------------// bdm_bases.h
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2003/07/10 09:51:55 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------
// Définition des objets de base de donnée de base medicamenteuse
// -----------------------------------------------------------------------------
#ifndef __BDMBASES_H__
# define __BDMBASES_H__


# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

# include "ns_sgbd\nsfiche.h"
// Taille des champs du fichier BDM# define BDM_ID_LEN						4# define BDM_CIP_LEN    		 20   // Code CIP
# define BDM_EXT_LEN      		2   // Extension Nautilus
# define BDM_LEXIMED_LEN  		5   // Code Leximed
# define BDM_VIDAL_LEN    		7   // Code Vidal
# define BDM_LEXOLD_LEN   		6   // Code ancien Leximed

// Indice des champs du fichier BDM# define BDM_ID_FIELD					1
# define BDM_CIP_FIELD				2
# define BDM_EXT_FIELD				3
# define BDM_LEXIMED_FIELD    4
# define BDM_VIDAL_FIELD      5
# define BDM_LEXOLD_FIELD     6

// **************************** Table BDM.DB ****************************
//---------------------------------------------------------------------------//  Classe NSPatLinkData
//
// Objet contenant les données
//---------------------------------------------------------------------------
//

class NSBdmData{
 public:
	char				id				[BDM_ID_LEN				+ 1] ;
	char				codeCIP		[BDM_CIP_LEN 			+ 1] ;
	char 				extension	[BDM_EXT_LEN			+ 1] ;
	char 				leximed		[BDM_LEXIMED_LEN 	+ 1] ;
	char 				codeVidal	[BDM_VIDAL_LEN		+ 1] ;
	char 				leximedOld[BDM_LEXOLD_LEN		+ 1] ;

	NSBdmData() ;	NSBdmData(NSBdmData& rv) ;
	~NSBdmData() ;

	NSBdmData&	operator=(NSBdmData src) ;
	int 	    	operator==(const NSBdmData& o) ;

	void				metAZero() ;
} ;


class NSBdm ;

//---------------------------------------------------------------------------//  Classe NSBdmInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSBdmInfo
{
 public:
	NSBdmData		*pDonnees ;

	NSBdmInfo() ;
	NSBdmInfo(NSBdm*) ;
	NSBdmInfo(NSBdmInfo& rv) ;
	~NSBdmInfo() ;

	NSBdmInfo&	operator=(NSBdmInfo src) ;
	int 	    	operator==(const NSBdmInfo& o) ;
} ;

//---------------------------------------------------------------------------
//  Classe NSBdm
//
// Objet dérivé de NSFiche servant aux accès de base de données
//---------------------------------------------------------------------------
//
class NSBdm : public NSBdmInfo, public NSFiche
{
 public:
	NSBdm(NSContexte* pCtx) ;
	NSBdm(NSBdm& rv) ;
	~NSBdm() ;

	void      alimenteFiche() ;
	void      videFiche() ;
	DBIResult	open() ;

	NSBdm&    operator=(NSBdm src) ;
  int 	    operator==(const NSBdm& o) ;
} ;

//
// Définition de NSBdmArray (Array de NSBdmInfo(s))
//

typedef vector<NSBdmInfo *> NSBMArray ;
typedef NSBMArray::iterator NSBdmIter ;

class NSBdmArray : public NSBMArray
{
 public:
	// Constructeurs
	NSBdmArray() : NSBMArray() {}
	NSBdmArray(NSBdmArray& rv) ;
	// Destructeur
	virtual			~NSBdmArray() ;
	void				vider() ;

	NSBdmArray&	operator=(NSBdmArray src) ;
} ;

//---------------------------------------------------------------------------
#endif    // __BDMBASES_H__

