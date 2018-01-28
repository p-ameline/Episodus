// -----------------------------------------------------------------------------
// VidalMaster.h
// -----------------------------------------------------------------------------
// construction des bases liées au médicament avec comme source le VIDAL
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: fabrice $
// $Date: 2004/05/03 18:02:36 $
// -----------------------------------------------------------------------------
// FLP - mars/avril/mai 2004
// FLP - mai/juin/juillet 2003
// -----------------------------------------------------------------------------

#ifndef __VIDALMASTER_H__
# define __VIDALMASTER_H__


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
/*
# include <stdio.h>
# include <fstream.h>
*/
# include "partage\nsglobal.h"

// Masters
# include "nsmt_vidalCD\DCMaster.h"
# include "nsmt_vidalCD\CDrugMaster.h"

// DB Interfaces
# include "nsmt_vidalCD\ibdm.h"
# include "nsmt_vidalCD\ibdmactif.h"
# include "nsmt_vidalCD\iflechies.h"
# include "nsmt_vidalCD\ilexique.h"
# include "nsmt_vidalCD\ileximed.h"
# include "nsmt_vidalCD\isavoir.h"
# include "nsmt_vidalCD\isavoirmed.h"
# include "nsmt_vidalCD\ividal.h"



class __VIDALEXPORT__	VidalMaster : public NSRoot
{
 public:
	VidalMaster(NSContexte *pCtx) ;
	VidalMaster(VidalMaster& src) ;
	~VidalMaster() ;

	VidalMaster&						operator=(VidalMaster& src) ;

//	bool										init() ;
	bool										doAll() ;

  bool                    doDCs() ;
	bool										doCDrugs() ;

	bool										rebuildBdm() ;

	void										verifCodeLexiq() ;

  DCMaster                *pDCMaster ;
	CDrugMaster						  *pCDrugMaster ;

	InterfaceVidal					*pIVidal ;
	InterfaceBdm						*pIBdm ;
	InterfaceBdmActif				*pIBdmActif ;
	InterfaceFlechies				*pIFlechies ;
	InterfaceLexique				*pILexique ;
	InterfaceLexiMed				*pILexiMed ;
	InterfaceSavoir					*pISavoir ;
	InterfaceSavoirMed			*pISavoirMed ;
} ;


#endif // __VIDALMASTER_H__
