// -----------------------------------------------------------------------------
// fichier DCMaster.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: fabrice $
// $Date: 2004/05/03 18:02:35 $
// -----------------------------------------------------------------------------
// FLP - avril/mai 2004
// -----------------------------------------------------------------------------

#ifndef __DCMASTER__
# define __DCMASTER__


# ifndef __DANSVIDALDLL__
#  define __VIDALEXPORT__	__import
# else
#  define __VIDALEXPORT__	__export
# endif

# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

# include <string.h>

# include "ns_vidal\DrugComponent.h"

# include "partage\nsglobal.h"


// -----------------------------------------------------------------------------
// fonctions de tri
// --
// sort functions
// -----------------------------------------------------------------------------
bool                      sortDCbyLabel                 (DrugComponent *pDC1, DrugComponent *pDC2) ;
bool                      sortDCbyCodeVidal             (DrugComponent *pDC1, DrugComponent *pDC2) ;
bool                      sortDCbyCodeVidalWithMetaFirst(DrugComponent *pDC1, DrugComponent *pDC2) ;


class VidalMaster ;


// -----------------------------------------------------------------------------
// class DCMaster
// --
// classe qui permet la recherche/comparaison et l'ajout des principes actifs et
// des excipients dans la base de données.
// --
// object that search/compare and add Active Ingredients and Excipients in
// database.
// -----------------------------------------------------------------------------
class	DCMaster : public NSRoot
{
 public:
  // constructor/destructor
	DCMaster(NSContexte *pCtx) ;
	DCMaster(DCMaster& src) ;
	~DCMaster() ;

  // operator
	DCMaster&								operator=(DCMaster& src) ;


  // functions of initialization
	bool										init(VidalMaster *pIVidal) ;
	int											initProcess(string sArray, DrugComponent::DCType type) ;
	int											initSsClasse(string sSsClasse, ArrayOfDrugComponents *pSsClass) ;

  // functions of action
	bool										doIt() ;
  bool                    doAIs() ;
  bool                    doExcipients() ;
	bool										processSsClasse(DrugComponent *DC) ;

  // functions adding terms in database
	string									addElemSubClass(DrugComponent *DC, DrugComponent *DCFather) ;
	bool										addElemInDB(DrugComponent *DC) ;


  // variables
	VidalMaster 						*pVidalMaster ;
	ArrayOfDrugComponents		*pDCs ;
} ;


#endif // __DCMASTER__