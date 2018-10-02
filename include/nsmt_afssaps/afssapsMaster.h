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

#ifndef  __AFSSAPSMASTER_H__
# define __AFSSAPSMASTER_H__


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

class __AFSSAPSEXPORT__	AfssapsMaster : public NSRoot
{
  public:

	  AfssapsMaster(NSContexte *pCtx) ;
	  AfssapsMaster(const AfssapsMaster& src) ;
	  ~AfssapsMaster() ;

	  AfssapsMaster& operator=(const AfssapsMaster& src) ;

    bool             doCIS() ;
	  bool						 processLine(string* psLine) ;
    bool             processDrug(string sCisCode, string sDenomination,
                                 string sForme, string sVoies, string sStatutAmm,
                                 string sTypeAmm, string sEtatCommerc,
                                 string sDateAMM, string sStatutBdm,
                                 string sNumEurope, string sTitulaire,
                                 string sSurveilRenfo) ;

    bool             doCISGener() ;
    void						 processGenerLine(string* psLine) ;
    void             processGener(string sGroupId, string sDenomination,
                                string sRefCisCode, string sType,
                                string sIdInGroup) ;
    void             updateGenerSemantic(string sCode,
                                string sRefCisCode, string sType,
                                string sIdInGroup) ;

    bool             doDci() ;
    void						 processDciLine(string* psLine) ;
    void             processDci(string sDciId, string sDenomination) ;

	  bool						 rebuildBdm() ;

	  void						 verifCodeLexiq() ;

	  InterfaceBdm		 *pIBdm ;
	  InterfaceLexiMed *pILexiMed ;
} ;

#endif // __AFSSAPSMASTER_H__

