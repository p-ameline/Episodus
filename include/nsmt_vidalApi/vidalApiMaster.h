// -----------------------------------------------------------------------------
// AfssapsMaster.h
// -----------------------------------------------------------------------------
// construction des bases liées au médicament avec comme source l'Afssaps
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2015/05/01 21:55:38 $
// -----------------------------------------------------------------------------
// PA - feb 2012
// -----------------------------------------------------------------------------

#ifndef  __VIDALAPIMASTER_H__
# define __VIDALAPIMASTER_H__


# ifndef __DANSAFSSAPSDLL__
#  define __AFSSAPSEXPORT__	__import
# else
#  define __AFSSAPSEXPORT__	__export
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

// DB Interfaces
# include "nsoutil\ibdm.h"
# include "nsmt_vidalCD\ibdmactif.h"
# include "nsmt_vidalCD\iflechies.h"
# include "nsmt_vidalCD\ilexique.h"
# include "nsmt_vidalCD\ileximed.h"
# include "nsmt_vidalCD\isavoir.h"
# include "nsmt_vidalCD\isavoirmed.h"
# include "nsmt_vidalCD\ividal.h"

class __AFSSAPSEXPORT__	VidalApiMaster : public NSRoot
{
 public:

	VidalApiMaster(NSContexte *pCtx) ;
	VidalApiMaster(const VidalApiMaster& src) ;
	~VidalApiMaster() ;

	VidalApiMaster& operator=(const VidalApiMaster& src) ;

  bool             doMedicabase() ;
	bool						 processLine(string* psLine) ;
  bool             processVirtualDrug(string sMedicabaseCode, string sDenomination) ;

	void						 verifCodeLexiq() ;

	InterfaceBdm		 *_pIBdm ;
	InterfaceLexiMed *_pILexiMed ;
} ;

#endif // __VIDALAPIMASTER_H__

