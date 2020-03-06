//-------------------------------------------------------------------------
//                    BIBLIOTHEQUE : NSDIVFCT.H
//
//                     Pot-pourri de fonctions
//
//             Debut d'ecriture : 30 Juin 1988 (PA)
//             Derniere version : 28 Juillet 1994 (PA)
//-------------------------------------------------------------------------
#ifndef __NSDIVFILE_H
#define __NSDIVFILE_H

#include <string>

#include "partage/ns_vector.h"

#ifdef _NSUTILDLL

	bool _export NsFileExists(const string sFileName) ;
  bool _export NsFileExists2(const string sFileName) ;

  void _export NsParseFileName(const string sCompleteFileName, string &sPath, string &sName) ;
  void _export NsParseExtension(const string sCompleteFileName, string &sRoot, string &sExtension) ;

  bool _export NsDeleteTemporaryFile(NSSuper *pSuper, const string sFileName) ;

  string _export getFileContent(const NSSuper *pSuper, const string sFileName) ;

  string _export getFileError() ;
  string _export getErrorText(LONG lOsErr) ;

#else

  bool NsFileExists(const string sFileName) ;
  bool NsFileExists2(const string sFileName) ;

  void NsParseFileName(const string sCompleteFileName, string &sPath, string &sName) ;
  void NsParseExtension(const string sCompleteFileName, string &sRoot, string &sExtension) ;

  bool NsDeleteTemporaryFile(NSSuper *pSuper, const string sFileName) ;

  string getFileContent(const NSSuper *pSuper, const string sFileName) ;

  string getFileError() ;
  string getErrorText(LONG lOsErr) ;

#endif

#endif 		// __NSDIVFILE_H

