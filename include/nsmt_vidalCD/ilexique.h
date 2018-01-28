// -----------------------------------------------------------------------------
// fichier ilexique.h
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2004/04/29 17:34:48 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------


#ifndef __INTERFACELEXIQUE_H__
# define __INTERFACELEXIQUE_H__

# include <string.h>

# include "nautilus\nssuper.h"


class InterfaceLexique : public NSRoot
{
 public:
	InterfaceLexique(NSContexte *pCtx) ;
	InterfaceLexique(InterfaceLexique& src) ;
	~InterfaceLexique() ;

	InterfaceLexique&		 		operator=(InterfaceLexique& src) ;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// this class has no init and no close function because the Lexique is always
	// open in Nautilus
	// we can not initialize this DB while it is already open
	// also we must not close this DB while it will create error in main program
	// execution
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	bool										isLibelleInDB(string sLibelle, string *sCode) ;
	bool										isCodeInDB(string sCode) ;
  string                  getCode(string sLabel) ;
	bool										insertElem(string sLibelle, string sCode, string sGrammaire, string sFrequence) ;
	string									computeNewFreeCodeLexiq(char cCodeType) ;
	void										incrementeDigitAlphaNum(string *sCodeTmp, int iNbDigit) ;
	string									firstFreeLCodeFromCodeSens(string sCodeSens) ;

	void										verifCodeLexiq() ;

	NSPatholog							*pLexique ;
  string                  sNewFreeLCodeI ;
  string                  sNewFreeLCode8 ;
} ;

#endif // __INTERFACELEXIQUE_H__
