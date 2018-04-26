// -----------------------------------------------------------------------------
// fichier ibdm.h
// interface avec la base bdm
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: fabrice $
// $Date: 2004/05/12 17:49:17 $
// -----------------------------------------------------------------------------
// FLP - mai/juin/juillet 2003
// -----------------------------------------------------------------------------
// Tout ce qui concerne la base bdm doit -- à priori -- être fait à partir de
// cette interface
// -----------------------------------------------------------------------------

#ifndef __INTERFACEBDM_H__
# define __INTERFACEBDM_H__

# include <string.h>

# include "nsoutil/nsexport.h"
# include "nsoutil/bdm_bases.h"
# include "nautilus/nssuper.h"

class _NSOUTILCLASSE InterfaceBdm : public NSRoot
{
  public:

    InterfaceBdm(NSContexte *pCtx) ;
    InterfaceBdm(InterfaceBdm& src) ;
    ~InterfaceBdm() ;

    InterfaceBdm&	operator=(InterfaceBdm& src) ;

    bool				  init() ;
    bool          updateCipForLeximed(string sCodeCIP, string sLeximed) ;
    bool					insertElem(string sCodeCIP, string sExtension, string sLeximed, string sCodeVidal, string sLeximedOld) ;
    bool					close() ;

    bool          isInDB(string sCodeCIP, string sExtension, string sLexiMedSens, string sCodeVidal, string sLeximedOld) ;
    bool					isIDInDB(string sID) ;
    bool					isCodeCIPInDB(string sCodeCIP) ;
    bool					isCodeLexiMedInDB(string sLeximed) ;
    bool					isCodeVidalInDB(string sCodeVidal) ;
    bool					isCodeLexiMedOldInDB(string sLeximedOld) ;

    string				getLeximedForCIP(string sCodeCIP) ;

    string				getNewFreeID() ;

    string				sIdCpt ;

    NSBdm				  *pBdm ;
} ;

#endif // __INTERFACEBDM_H__
