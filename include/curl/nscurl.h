//-------------------------------------------------------------------------//                   HEADER FILE : NSCURL.H
//
//                   Interface with LibCurl
//
//             Initial release : August 29 2007 (PA)
//             Final release   : August 29 2007 (PA)
//-------------------------------------------------------------------------
#ifndef __NSCURL_H
#define __NSCURL_H

#include <map>
#include <owl\module.h>

#include "curl\curl.h"
#include "curl\easy.h"
#include "nsbb\NTFunctor.h"

// LibCurl Functions
//
#define CURL_EASY_INIT 		   5
#define CURL_EASY_SETOPT     8
#define CURL_EASY_PERFORM    6
#define CURL_EASY_RESET      7
#define CURL_EASY_CLEANUP    1
#define CURL_EASY_GETINFO    4
#define CURL_EASY_STRERROR   9
#define CURL_FORMADD        12
#define CURL_FORMFREE       13
#define CURL_GLOBAL_INIT    19
#define CURL_GLOBAL_CLEANUP 18
#define CURL_SLIST_APPEND   49
#define CURL_SLIST_FREE_ALL 50

class NSLibCurl
{
	public :

		TModule* pLibcurlModule ;

    CURL*    pCurlEasyHandle ;

    char     szUrl[1024] ;
    char     szLogin[1024] ;
    char     szPassword[1024] ;
    char     szLogAndPass[2048] ;

    map<string, string> aCurlOptParams ;

    string   sLastCurlError ;

    // Pointers to Easy funstions set
    //
    static CURL*        (FAR* pCurlEasyInit)( void ) ;
    static CURLcode     (FAR* pCurlEasySetopt)( CURL *curl, CURLoption option, ... ) ;
    static CURLcode     (FAR* pCurlEasyPerform)( CURL *curl ) ;
    static void         (FAR* pCurlEasyReset)( CURL *curl ) ;
    static void         (FAR* pCurlEasyCleanup)( CURL *curl ) ;
    static CURLcode     (FAR* pCurlEasyGetInfo)( CURL *curl, CURLINFO info, ... ) ;
    static const char*  (FAR* pCurlEasyStrerror)( CURLcode ) ;
    static CURLcode     (FAR* pCurlGlobalInit)( long flags ) ;
    static void         (FAR* pCurlGlobalCleanup)( void ) ;
    static CURLFORMcode (FAR* pCurlFormAdd)(struct curl_httppost **httppost, struct curl_httppost **last_post, ...) ;
    static void         (FAR* pCurlFormFree)(struct curl_httppost *form) ;
    static curl_slist*  (FAR* pCurlSlistAppend)(struct curl_slist *, const char *) ;
    static void         (FAR* pCurlSlistFreeAll)(struct curl_slist *) ;

		NSLibCurl() ;
		~NSLibCurl() ;

    bool bIsReady() ;

    bool initCurlOptParams(string sCurlParamsFile) ;
		void init() ;
    void cleanup() ;
    bool fileDownload(string sServerFileName, string sDownloadFileName, bool bProgressBar = false, UnaFunctor<int>* pProgressBarSetter = 0) ;
    void setCurlOptParams() ;

    void setURL(string sUrl)       { strcpy(szUrl,      sUrl.c_str()) ; }
    void setLogin(string sLog)     { strcpy(szLogin,    sLog.c_str()) ; }
    void setPassword(string sPass) { strcpy(szPassword, sPass.c_str()) ; }
} ;

// custom data type to be used in callback function

class XferInfo
{
	private:

		int      _bytesTransferred ;
		int      _invocations ;

		string   _sFileName ;
		ofstream _outFile ;

	protected:
	// empty

	public:

  	FILE     *_stream ;

		XferInfo() ;
    ~XferInfo() ;

    bool init(string sFile, bool bForceReplace = false) ;
    void write(const char* input, int size) ;
    void close() ;

		/// get the amount of data transferred, in bytes
		int getBytesTransferred() const { return( _bytesTransferred ) ; }
		/// get the number of times add() has been called
		int getTimesCalled(){ return( _invocations ) ; }
} ;

#endif 		// __NSCURL_H

