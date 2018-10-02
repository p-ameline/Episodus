// -----------------------------------------------------------------------------
// fichieer ileximed.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: fabrice $
// $Date: 2004/04/29 17:34:48 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------
// fichier d'interface entre utilitaire base de données nautilus - médicaments
// et leximed.db
// -----------------------------------------------------------------------------

#ifndef __INTERFACELEXIMED_H__
# define __INTERFACELEXIMED_H__

# include "nautilus\nssuper.h"

class InterfaceLexiMed : public NSRoot
{
 public:

	InterfaceLexiMed(NSContexte *pCtx) ;
	InterfaceLexiMed(InterfaceLexiMed& src) ;
	~InterfaceLexiMed() ;

	InterfaceLexiMed& operator=(InterfaceLexiMed& src) ;

	bool							init() ;
	bool							isLibelleInDB(string sLibelle) ;
	bool							isCodeInDB(string sCode) ;
	string						getCode(string sLibelle, bool bAskUser = true, bool bForceAskUser = false) ;
  string						getCodeForGeneric(string sLibelle, bool bAskUser = true, bool bForceAskUser = false) ;
	bool							insertElem(string sLexique, string sCode, string sGrammaire, string sFreq) ;
	bool							close() ;

	string						getNewCodeLexiMed() ;
	void							incrementeDigitAlphaNum(string *sCodeTmp, int iNbDigit) ;

	void    					ConvertitMajuscule(char *chaine) ;

  string						getCapitalString(string sLabel) ;
  bool              isCapitalChar(char cChar) ;
  bool              isUpper(char cToTest) ;
  void              removeBlanks(string *pLabel) ;

  bool              mustStopAfterThisOne() { return _bStopAfterThisOne ; }

	NSLexiMed					*_pLexiMed ;
  string            _sNewFreeLMCode ;
  bool              _bStopAfterThisOne ;
} ;

#endif // __INTERFACELEXIMED_H__

