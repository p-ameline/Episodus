// -----------------------------------------------------------------------------
// fichier isavoir.h
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2004/05/03 18:02:36 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------


#ifndef __INTERFACESAVOIRMED_H__
# define __INTERFACESAVOIRMED_H__

# include "nautilus\nssuper.h"
# include	"nsmt_vidalCD\savoimed_bases.h"

class InterfaceSavoirMed : public NSRoot
{
 public:
	InterfaceSavoirMed(NSContexte *pCtx) ;
	InterfaceSavoirMed(InterfaceSavoirMed& src) ;
	~InterfaceSavoirMed() ;

	InterfaceSavoirMed&     operator=(InterfaceSavoirMed& src) ;

	bool										init() ;
	bool										insertElem(string sCode, string sQualifie, string sLien, string sQualifiant, string sDegre, string sClasse, string sScenario) ;
	bool										close() ;

	NSSavoirMed							*pSavoirMedDB ;
  string                  sNewFreeCode ;
} ;

#endif // __INTERFACESAVOIRMED_H__
