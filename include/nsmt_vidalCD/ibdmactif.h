// -----------------------------------------------------------------------------
// fichier ibdmactif.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: fabrice $
// $Date: 2004/05/14 16:56:08 $
// -----------------------------------------------------------------------------
// FLP - mai/juin 2003
// -----------------------------------------------------------------------------

#ifndef __INTERFACEBDMACTIF_H__
# define __INTERFACEBDMACTIF_H__

# include <string.h>

# include "nsmt_vidalCD\bdmactif_bases.h"
# include "nautilus\nssuper.h"

class	InterfaceBdmActif : public NSRoot
{
 public:
	InterfaceBdmActif(NSContexte *pCtx) ;
	InterfaceBdmActif(InterfaceBdmActif& src) ;
	~InterfaceBdmActif() ;

	InterfaceBdmActif&			operator=(InterfaceBdmActif& src) ;

	bool										init() ;
	bool										isInDB(string sLabel) ;
	string									getCode(string sLabel) ;
	bool										insertElem(string sLabel, string sCode) ;
	bool										close() ;

  void                    verif() ;

	void    								ConvertitMajuscule(char *chaine) ;

	NSBdmActif							*pBdmActif ;
} ;

#endif // __INTERFACEBDMACTIF_H__
