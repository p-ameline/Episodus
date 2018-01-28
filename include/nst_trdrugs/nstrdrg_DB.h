// -----------------------------------------------------------------------------//   Paradox data base management for Turkish drug database mapping// -----------------------------------------------------------------------------

#ifndef __TRDRUGSDB_H__# define __TRDRUGSDB_H__

# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

#include "partage\NTArray.h"# include "ns_sgbd\nsfiche.h"// Taille des champs du fichier BDM# define TRBDM_ID_LEN					4# define TRBDM_BARCOD_LEN    13   // Turkish barcode number# define TRBDM_LEXIMED_LEN  	5

// Indice des champs du fichier BDM# define TRBDM_ID_FIELD				1
# define TRBDM_BARCOD_FIELD		2
# define TRBDM_LEXIMED_FIELD  3

// **************************** Table BDM.DB ****************************
//---------------------------------------------------------------------------//  Classe NSTrBdmData
//
// Data container
//---------------------------------------------------------------------------
//

class NSTrBdmData{
	public:

  char id     [TRBDM_ID_LEN      + 1] ;
	char barcode[TRBDM_BARCOD_LEN  + 1] ;
	char leximed[TRBDM_LEXIMED_LEN + 1] ;

	NSTrBdmData() ;	NSTrBdmData(NSTrBdmData& rv) ;
	~NSTrBdmData() ;

	NSTrBdmData& operator=(NSTrBdmData src) ;
	int 	    	 operator==(const NSTrBdmData& o) ;

	void				 metAZero() ;
} ;


class NSTrBdm ;
//---------------------------------------------------------------------------//  Classe NSTrBdmInfo  (for in memory storage in array)
//---------------------------------------------------------------------------
class NSTrBdmInfo
{
 public:

	NSTrBdmData	*pDonnees ;

	NSTrBdmInfo() ;
	NSTrBdmInfo(NSTrBdm*) ;
	NSTrBdmInfo(NSTrBdmInfo& rv) ;
	~NSTrBdmInfo() ;

	NSTrBdmInfo& operator=(NSTrBdmInfo src) ;
	int 	    	 operator==(const NSTrBdmInfo& o) ;
} ;

//---------------------------------------------------------------------------
// Database access object (cursor)
//---------------------------------------------------------------------------
//
class NSTrBdm : public NSTrBdmInfo, public NSFiche
{
	public:

	NSTrBdm(NSSuper* pSuper) ;
	NSTrBdm(NSTrBdm& rv) ;
	~NSTrBdm() ;

	void      alimenteFiche() ;
	void      videFiche() ;
	DBIResult	open() ;

	NSTrBdm&  operator=(NSTrBdm src) ;
  int 	    operator==(const NSTrBdm& o) ;
} ;

//
// Defining NSTrBdmArray (Array of NSTrBdmInfo(s))
//

typedef vector<NSTrBdmInfo *> NSTRBMArray ;
typedef NSTRBMArray::iterator NSTrBdmIter ;
typedef NTArray<NSTrBdmInfo>  NSTrBdmArray ;

#endif  // #ifndef __TRDRUGSDB_H__

