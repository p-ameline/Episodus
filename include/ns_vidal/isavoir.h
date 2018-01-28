// -----------------------------------------------------------------------------
// fichier isavoir.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: fabrice $
// $Date: 2004/04/29 17:34:48 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------

#ifndef __INTERFACESAVOIR_H__
# define __INTERFACESAVOIR_H__

# include "nautilus\nssuper.h"

class InterfaceSavoir : public NSRoot
{
 public:
	InterfaceSavoir(NSContexte *pCtx) ;
	InterfaceSavoir(InterfaceSavoir& src) ;
	~InterfaceSavoir() ;

	InterfaceSavoir&				operator=(InterfaceSavoir& src) ;

	bool										init() ;
	bool										insertElem(string sQualifie, string sLien, string sQualifiant, string sDegre, string sClasse, string sScenario) ;
	bool										close() ;

	bool										isCodeInDB(string sCode) ;
	string                  getNewFreeCode() ;

	NSSavoir								*pSavoirDB ;

  string                  sNewFreeCode ;
} ;

#endif // __INTERFACESAVOIR_H__
