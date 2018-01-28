// -----------------------------------------------------------------------------
// fichier ividal.h
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2004/04/01 18:15:25 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------

#ifndef __INTERFACEVIDAL_H__
# define __INTERFACEVIDAL_H__

# include <string.h>

#include "nsmt_vidalCD\Mvsmpl32fn.h"
#include "nautilus\nssuper.h"

class InterfaceVidal : public NSRoot
{
 public:
	InterfaceVidal(NSContexte *pCtx) ;
	InterfaceVidal(InterfaceVidal& src) ;
	~InterfaceVidal() ;

	InterfaceVidal&					operator=(InterfaceVidal& src) ;

	bool										init() ;
	string									searchAIs(string s2search) ;
	string									searchExcipients(string s2search) ;
	string									searchCDrugs(string s2search) ;
	string									searchDrugs(string sCDrugLabel) ;
  string                  searchDrugFullDetails(string sDrugId) ;
	string									searchCompo(string sCIPCode) ;
	string									searchClassif(string sLibelle) ;
	string									searchSsClasse(string sCodeVidal) ;
	bool										close() ;
} ;

#endif // __INTERFACEVIDAL_H__
