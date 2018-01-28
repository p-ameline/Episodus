// -----------------------------------------------------------------------------// nscaptur.h// -----------------------------------------------------------------------------
// Contient les définitions des objets intervenant dans la capture
// -----------------------------------------------------------------------------
// PA Septembre 2001
// -----------------------------------------------------------------------------
#ifndef __NSFILECAPTUR_H__
# define __NSFILECAPTUR_H__

# include <owl/dialog.h>
# include <owl/listbox.h>
# include <owl/edit.h>

class NSEpiFlechiesDB ;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "partage\ns_vector.h"
# include "nsbb\nsednum.h"
# include "nsbb\nspatpat.h"
# include "nssavoir\nscaptur.h"

# if defined(_DANSLEXIDLL)
#  define _CLASSELEXI __export
# else
#  define _CLASSELEXI __import
# endif

//! Mode de parse
enum ImportedFileType
{
	FILETYPE_HPRIM1,
	FILETYPE_HPRIM2,
  FILETYPE_HPRIM1_MULTI,
	FILETYPE_HPRIM2_MULTI,
  FILETYPE_EPISODUS_3,
  FILETYPE_EPISODUS_4,
	FILETYPE_UNKNOWN
} ;

#define HPRIMHEADER_LINESNB 12

class _CLASSELEXI HPRIM2ResEntry
{
  public:

    string _sLabel ;
    string _sCode ;
    string _sResultType ;
    string _sResult1 ;
    string _sUnit1 ;
    string _sLowerN1 ;
    string _sUpperN1 ;
    string _sAbnormal ;
    string _sStatus ;
    string _sResult2 ;
    string _sUnit2 ;
    string _sLowerN2 ;
    string _sUpperN2 ;

    // Constructeurs
    HPRIM2ResEntry(string sResLine) ;
    HPRIM2ResEntry(const HPRIM2ResEntry& rv) ;
    ~HPRIM2ResEntry() ;

    void init() ;
    void initFromResEntry(const HPRIM2ResEntry& rv) ;
    bool initFromResLine(string sResLine) ;

    HPRIM2ResEntry&	operator=(const HPRIM2ResEntry& src) ;
		int 		   	    operator==(const HPRIM2ResEntry& o) ;
} ;

// -----------------------------------------------------------------------------
// HPRIM import
// -----------------------------------------------------------------------------
class _CLASSELEXI NSCaptureFromHPRIM : public NSRoot
{
 public:

  // Constructeurs
  NSCaptureFromHPRIM(NSContexte* pCtx) ;
  ~NSCaptureFromHPRIM() ;

  bool   importHPRIM2(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT, int* iAlertLevel, string* pLogMessage, bool *pbValidHeader, string &sRootExam) ;
  bool   importHPRIM1(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT, string* pLogMessage, bool *pbValidHeader, string &sRootExam) ;
  bool   importTextContent(NSCaptureArray* pCaptureArray) ;

  ImportedFileType importHPRIMheader(NSCaptureArray* pCaptureArray, string* pLogMessage) ;
  string           getHPRIMBody() ;

  bool   parseHPRIM1header(ifstream* pInFile, NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT, string* pLogMessage, string* pLabName = 0, string* pDocName = 0) ;
	bool   parseHPRIM1text(ifstream* pInFile, NSPatPathoArray* pPPT, bool* pbStoppedOnLab, string &sRootExam) ;

  PatPathoIter findStringInPatPatho(string sRef, NSPatPathoArray* pPPT) ;

  bool   explodeMultipleMessagesFile() ;

  string getDateFromHPRIM1date(string sHPRIMdate, string s2kTreshold) ;

  void   setFileName(string sNameOfFile) { sFileName = sNameOfFile ; }
  string getFileName()                   { return sFileName ; }

  string sFindFirstExamInSentence(string sSentence, NSEpiFlechiesDB* pFlechies) ;

  bool   getHprim2ResEntries(string sLine) ;

 protected:

	string sFileName ;
  string sFileContent ;
} ;

#endif // !__NSFILECAPTUR_H__

