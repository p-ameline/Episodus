// -----------------------------------------------------------------------------
// fichier CDrugMaster.h
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2004/05/10 18:11:50 $
// -----------------------------------------------------------------------------
// FLP - avril/mai 2004
// -----------------------------------------------------------------------------

#ifndef __CDRUGMASTER_H__
# define __CDRUGMASTER_H__


# ifndef __DANSVIDALDLL__
#  define __VIDALEXPORT__	__import
# else
#  define __VIDALEXPORT__	__export
# endif

# include <string.h>

# include "partage\nsglobal.h"

# include "ns_vidal\DrugElem.h"

//# include <stdio.h>


class	VidalMaster ;


class	CDrugMaster : public NSRoot
{
 public:
 	// constructor/destructor
	CDrugMaster(NSContexte *pCtx) ;
	CDrugMaster(CDrugMaster& src) ;
	~CDrugMaster() ;

  // operator
	CDrugMaster&						operator=(CDrugMaster& src) ;

  // functions
	bool										init(VidalMaster *pVM) ;
	bool										doIt() ;
	bool										rebuildBdm() ;
	bool										rebuildBdmOneLetter() ;

	bool										initOneLetter(char c) ;
	bool										doOneLetter() ;

  bool                    initDrug(CommercialDrugElem *CMElem) ;
  bool                    processCDrug(CommercialDrugElem *CMElem) ;

	// variables
	VidalMaster 						*pVidalMaster ;

	ArrayOfCommercialDrugs	*pCDrugs ;
	string									sVidalCDrugs ;
} ;


/*
class stringVector : public VectString
{
 public:
  // constructors/destructor
  stringVector() ;
  stringVector(stringVector& src) ;
  ~stringVector() ;

  // operator
  stringVector&             operator=(stringVector& src) ;

  // functions
  bool                    isP(string sItem) ;
  void                    init(CommercialDrugElem *pCDElem) ;
} ;
*/

#endif // __CDRUGMASTER_H__
