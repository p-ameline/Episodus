// -----------------------------------------------------------------------------
// fichier iflechies.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: fabrice $
// $Date: 2003/07/10 09:51:30 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------


#ifndef __INTERFACEFLECHIES_H__
# define __INTERFACEFLECHIES_H__

# include <string.h>

# include "nsepisod\flechiesDB.h"
# include "nautilus\nssuper.h"


class InterfaceFlechies : public NSRoot
{
 public:
	InterfaceFlechies(NSContexte *pCtx) ;
	InterfaceFlechies(InterfaceFlechies& src) ;
	~InterfaceFlechies() ;

	InterfaceFlechies&		 	operator=(InterfaceFlechies& src) ;

	bool										init() ;
	bool										isInDB(string sLabel) ;
	bool										isInDB(string sLabel, char cTypeCode) ;
	string									getCode(string sLabel) ;
	string									getCode(string sLabel, char cTypeCode) ;

	bool										addTerm(NSPathologData *pDonnees) ;

	bool										close() ;

	NSFlechies							*pFlechies ;
} ;

#endif // __INTERFACEFLECHIES_H__
