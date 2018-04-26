//---------------------------------------------------------------------------#include <stdio.h>
#include <sysutils.hpp>		// needed for FileExists

#include "curl\nscurl.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"

/****************************************************************************/
/***                         Callback functions                           ***/
/****************************************************************************/

// C++ programmers, take note of the "extern" call for C-style linkage
extern "C"
size_t downloadFile(void *buffer , size_t size , size_t nmemb , void *userData )
{

	// this function may be called any number of times for even a single
	// transfer; be sure to write it accordingly.

	// userData is called "stream" in the docs, which is misleading:
	// that parameter can be _any_ data type, not necessarily a FILE*
	// Here, we use it to save state between calls to this function
	// and track number of times this callback is invoked.
	XferInfo* info = static_cast< XferInfo* >( userData ) ;

  /*
  // buffer is the actual data fetched by libcURL; cast it to whatever
	// type you need (usually char*).  It has NO terminating NULL byte.
  //
  const char* data = static_cast< const char* >( buffer ) ;
	const int bufferSize = size * nmemb ;
	info->write(data, bufferSize) ;
	return( bufferSize ) ;
  */

  return fwrite(buffer, size, nmemb, info->_stream) ;
}

extern "C"
int progress_func(UnaFunctor<int>* pProgressBarSetter,
                     double t, /* dltotal */
                     double d, /* dlnow */
                     double ultotal,
                     double ulnow)
{
/*  printf("%d / %d (%g %%)\n", d, t, d*100.0/t);*/
  (*pProgressBarSetter)(d*100.0/t) ;
  return 0 ;
}

/****************************************************************************//***                           CLASSE NSLibCurl                           ***/
/****************************************************************************/

CURL*        (FAR* NSLibCurl::pCurlEasyInit)( void ) = NULL ;
CURLcode     (FAR* NSLibCurl::pCurlEasySetopt)( CURL *curl, CURLoption option, ... ) = NULL ;
CURLcode     (FAR* NSLibCurl::pCurlEasyPerform)( CURL *curl ) = NULL ;
void         (FAR* NSLibCurl::pCurlEasyReset)( CURL *curl ) = NULL ;
void         (FAR* NSLibCurl::pCurlEasyCleanup)( CURL *curl ) = NULL ;
CURLcode     (FAR* NSLibCurl::pCurlEasyGetInfo)( CURL *curl, CURLINFO info, ... ) = NULL ;
const char*  (FAR* NSLibCurl::pCurlEasyStrerror)( CURLcode ) = NULL ;
CURLcode     (FAR* NSLibCurl::pCurlGlobalInit)( long flags ) = NULL ;
void         (FAR* NSLibCurl::pCurlGlobalCleanup)( void ) = NULL ;
CURLFORMcode (FAR* NSLibCurl::pCurlFormAdd)(struct curl_httppost **httppost, struct curl_httppost **last_post, ...) = NULL ;
void         (FAR* NSLibCurl::pCurlFormFree)(struct curl_httppost *form) = NULL ;
curl_slist*  (FAR* NSLibCurl::pCurlSlistAppend)(struct curl_slist *, const char *) = NULL ;
void         (FAR* NSLibCurl::pCurlSlistFreeAll)(struct curl_slist *) = NULL ;
// Constructeur
NSLibCurl::NSLibCurl()
{
try
{
	pCurlEasyHandle = 0 ;

  szUrl[0]        = '\0' ;
	szLogin[0]      = '\0' ;
	szPassword[0]   = '\0' ;
  szLogAndPass[0] = '\0' ;

	pLibcurlModule = new TModule("libcurl-4.dll", TRUE) ;
	if ((TModule*) NULL == pLibcurlModule)
		erreur("can't load libcurl-4.dll", standardError, 0) ;
  else
	{
  	(FARPROC) pCurlEasyInit      = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_EASY_INIT)) ;
    (FARPROC) pCurlEasySetopt    = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_EASY_SETOPT)) ;
    (FARPROC) pCurlEasyPerform   = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_EASY_PERFORM)) ;
    (FARPROC) pCurlEasyReset     = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_EASY_RESET)) ;
    (FARPROC) pCurlEasyCleanup   = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_EASY_CLEANUP)) ;
    (FARPROC) pCurlEasyGetInfo   = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_EASY_GETINFO)) ;
    (FARPROC) pCurlEasyStrerror  = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_EASY_STRERROR)) ;
    (FARPROC) pCurlGlobalInit    = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_GLOBAL_INIT)) ;
    (FARPROC) pCurlGlobalCleanup = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_GLOBAL_CLEANUP)) ;
    (FARPROC) pCurlFormAdd       = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_FORMADD)) ;
    (FARPROC) pCurlFormFree      = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_FORMFREE)) ;
    (FARPROC) pCurlSlistAppend   = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_SLIST_APPEND)) ;
    (FARPROC) pCurlSlistFreeAll  = pLibcurlModule->GetProcAddress(MAKEINTRESOURCE(CURL_SLIST_FREE_ALL)) ;
	}

/*
	HMODULE cl = NULL;
  if(NULL == (cl = LoadLibrary("libcurl-4.dll")))
  {
  	::MessageBox(NULL, "I can't load libcurl","ERROR", 0);
    return;
  }
  else
	{
  	(FARPROC) pCurlEasyInit      = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_EASY_INIT)) ;
    (FARPROC) pCurlEasySetopt    = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_EASY_SETOPT)) ;
    (FARPROC) pCurlEasyPerform   = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_EASY_PERFORM)) ;
    (FARPROC) pCurlEasyReset     = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_EASY_RESET)) ;
    (FARPROC) pCurlEasyCleanup   = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_EASY_CLEANUP)) ;
    (FARPROC) pCurlEasyGetInfo   = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_EASY_GETINFO)) ;
    (FARPROC) pCurlEasyStrerror  = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_EASY_STRERROR)) ;
    (FARPROC) pCurlGlobalInit    = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_GLOBAL_INIT)) ;
    (FARPROC) pCurlGlobalCleanup = ::GetProcAddress(cl, MAKEINTRESOURCE(CURL_GLOBAL_CLEANUP)) ;
	}
*/
}
catch (TXInvalidModule e)
{
	string sErreur = string("Exception NSLibCurl ctor : ") + e.why() ;
	erreur(sErreur.c_str(), standardError, 0) ;
  pLibcurlModule = 0 ;
}
catch (...)
{
	erreur("Exception NSLibCurl ctor", standardError, 0) ;
}
}

// destructeurNSLibCurl::~NSLibCurl()
{
	if (pLibcurlModule)
		delete pLibcurlModule ;
}

bool
NSLibCurl::bIsReady()
{
	if ((NULL == pCurlEasyHandle) ||
	    (NULL == pCurlEasyInit) ||
      (NULL == pCurlEasySetopt) ||
	    (NULL == pCurlEasyPerform) ||
      (NULL == pCurlEasyReset) ||
      (NULL == pCurlEasyCleanup) ||
      (NULL == pCurlEasyGetInfo) ||
      (NULL == pCurlEasyStrerror) ||
      (NULL == pCurlGlobalInit) ||
      (NULL == pCurlGlobalCleanup)
     )
		return false ;

	return true ;
}

bool
NSLibCurl::initCurlOptParams(string sCurlParamsFile)
{
	if (false == NsFileExists(sCurlParamsFile))
		return true ;

  ifstream inFile ;
	// on ouvre le fichier de configuration
  inFile.open(sCurlParamsFile.c_str()) ;
  if (!inFile)
    return false ;

	string sLine ;
  // on lit les paramètres de backup
	while (!inFile.eof())
	{
		getline(inFile, sLine) ;

    size_t iLineLength = strlen(sLine.c_str()) ;
    if ((iLineLength > 2) && (string("//") != string(sLine, 0, 2)))
		{
    	string sLeftParam  = string("") ;
      string sRightParam = string("") ;
      //
      // get "Left + blank or tabs + Right"
      //
      size_t i = 0 ;
      while ((i < iLineLength) && (' ' != sLine[i]) && ('\t' != sLine[i]))
				sLeftParam += sLine[i++] ;
			while ((i < iLineLength) && ((' ' == sLine[i]) || ('\t' == sLine[i])))
				i++ ;

			while ((i < iLineLength) && ('\n' != sLine[i]))
				sRightParam += sLine[i++] ;

    	if ((string("") != sLeftParam) && (string("") != sRightParam))
      	aCurlOptParams[sLeftParam] = sRightParam ;
    }
	}

  inFile.close() ;

	return true ;
}

void
NSLibCurl::init()
{
	if ((NULL == pCurlEasyInit) || (NULL == pCurlGlobalInit))
		return ;

	(*pCurlGlobalInit)( CURL_GLOBAL_ALL ) ;
	pCurlEasyHandle = (*pCurlEasyInit)() ;

  initCurlOptParams("curlParams.dat") ;
}

void
NSLibCurl::cleanup()
{
	if ((NULL == pCurlEasyHandle) || (NULL == pCurlEasyCleanup))
		return ;

	(*pCurlEasyCleanup)(pCurlEasyHandle) ;
	pCurlEasyHandle = 0 ;

	(*pCurlGlobalCleanup)() ;
}

bool
NSLibCurl::fileDownload(string sServerFileName, string sDownloadFileName, bool bProgressBar, UnaFunctor<int>* pProgressBarSetter)
{
	if ((false == bIsReady()) || (szUrl[0] == '\0'))
		return false ;

	setCurlOptParams() ;

	char szFileUrl[2048] ;
  strcpy(szFileUrl, szUrl) ;
  strcat(szFileUrl, sServerFileName.c_str()) ;

	// First, check if it is not just about moving a file
	if (strlen(szFileUrl) > 3)
	{
		if (((szFileUrl[0] == '\\') && (szFileUrl[1] == '\\')) ||
        ((szFileUrl[1] == ':') && (szFileUrl[2] == '\\')))
			return ::CopyFile(szFileUrl, sDownloadFileName.c_str(), FALSE) ;
	}

	// URL
	(*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_URL, szFileUrl) ;

	// Login and Password
	if (szLogin[0] != '\0')
  {
		strcpy(szLogAndPass, szLogin) ;
    strcat(szLogAndPass, ":") ;
    strcat(szLogAndPass, szPassword) ;
    (*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_USERPWD, szLogAndPass) ;
  }

	// what to do with returned data
	(*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_WRITEFUNCTION, downloadFile) ;

  XferInfo info ;
  bool bInfoInited = info.init(sDownloadFileName, true) ;
  if (false == bInfoInited)
		return false ;

  // set the write function's user-data (state data)
	(*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_WRITEDATA, &info) ;


  // Progress bar
	if ((true == bProgressBar) && (NULL != pProgressBarSetter))
  {
  	(*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_NOPROGRESS, FALSE);
    (*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_PROGRESSFUNCTION, progress_func) ;
    (*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_PROGRESSDATA, pProgressBarSetter) ;
  }

	// action!

	const CURLcode rc = (*pCurlEasyPerform)(pCurlEasyHandle) ;

	if (CURLE_OK == rc)
  {
  	sLastCurlError = string("") ;
		return true ;
	}

	sLastCurlError = string((*pCurlEasyStrerror)(rc)) ;
  return false ;
}

void
NSLibCurl::setCurlOptParams()
{
	if (aCurlOptParams.empty())
    return ;

  map<string, string>::iterator iter ;

  for (iter = aCurlOptParams.begin() ; iter != aCurlOptParams.end() ; iter++)
  {
  	string sOption = (*iter).first ;
    string sParam  = (*iter).second ;

		CURLoption curlOption ;
    bool       bCurlOptionSet = false ;

    if      (string("CURLOPT_URL")                  == sOption) curlOption = CURLOPT_URL ;
    else if (string("CURLOPT_PROXY")                == sOption) curlOption = CURLOPT_PROXY ;
    else if (string("CURLOPT_PROXYPORT")            == sOption) curlOption = CURLOPT_PROXYPORT ;
    else if (string("CURLOPT_PROXYTYPE")            == sOption) curlOption = CURLOPT_PROXYTYPE ;
    else if (string("CURLOPT_HTTPPROXYTUNNEL")      == sOption) curlOption = CURLOPT_HTTPPROXYTUNNEL ;
    else if (string("CURLOPT_INTERFACE")            == sOption) curlOption = CURLOPT_INTERFACE ;
    else if (string("CURLOPT_LOCALPORT")            == sOption) curlOption = CURLOPT_LOCALPORT ;
    else if (string("CURLOPT_LOCALPORTRANGE")       == sOption) curlOption = CURLOPT_LOCALPORTRANGE ;
    else if (string("CURLOPT_DNS_CACHE_TIMEOUT")    == sOption) curlOption = CURLOPT_DNS_CACHE_TIMEOUT ;
    else if (string("CURLOPT_DNS_USE_GLOBAL_CACHE") == sOption) curlOption = CURLOPT_DNS_USE_GLOBAL_CACHE ;
    else if (string("CURLOPT_BUFFERSIZE")           == sOption) curlOption = CURLOPT_BUFFERSIZE ;
    else if (string("CURLOPT_PORT")                 == sOption) curlOption = CURLOPT_PORT ;
    else if (string("CURLOPT_TCP_NODELAY")          == sOption) curlOption = CURLOPT_TCP_NODELAY ;
    else if (string("CURLOPT_NETRC")                == sOption)
    {
    	curlOption = CURLOPT_NETRC ;
      if      (string("CURL_NETRC_OPTIONAL") == sParam) (*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_NETRC, CURL_NETRC_OPTIONAL) ;
    	else if (string("CURL_NETRC_IGNORED")  == sParam) (*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_NETRC, CURL_NETRC_IGNORED) ;
      else if (string("CURL_NETRC_REQUIRED") == sParam) (*pCurlEasySetopt)(pCurlEasyHandle, CURLOPT_NETRC, CURL_NETRC_REQUIRED) ;
      bCurlOptionSet = true ;
    }
    else if (string("CURLOPT_NETRC_FILE")           == sOption) curlOption = CURLOPT_NETRC_FILE ;
    else if (string("CURLOPT_USERPWD")              == sOption) curlOption = CURLOPT_USERPWD ;
    else if (string("CURLOPT_PROXYUSERPWD")         == sOption) curlOption = CURLOPT_PROXYUSERPWD ;
    else if (string("CURLOPT_HTTPAUTH")             == sOption) curlOption = CURLOPT_HTTPAUTH ;
    else if (string("CURLOPT_PROXYAUTH")            == sOption) curlOption = CURLOPT_PROXYAUTH ;

    if (false == bCurlOptionSet)
    {
    	size_t iParamLength = strlen(sParam.c_str()) ;

    	if ((iParamLength > 5) && (string("long ") == string(sParam, 0, 5)))
      {
      	sParam = string(sParam, 5, iParamLength - 5) ;
        (*pCurlEasySetopt)(pCurlEasyHandle, curlOption, atol(sParam.c_str())) ;
      }
      else if ((iParamLength > 4) && (string("int ") == string(sParam, 0, 4)))
      {
      	sParam = string(sParam, 4, iParamLength - 4) ;
        (*pCurlEasySetopt)(pCurlEasyHandle, curlOption, atoi(sParam.c_str())) ;
      }
      else if ((iParamLength > 5) && (string("bool ") == string(sParam, 0, 5)))
      {
      	sParam = string(sParam, 5, iParamLength - 5) ;
        sParam = pseumaj(sParam) ;

        if ((string("FALSE") == sParam) || (string("0") == sParam))
        	(*pCurlEasySetopt)(pCurlEasyHandle, curlOption, false) ;
        else
        	(*pCurlEasySetopt)(pCurlEasyHandle, curlOption, true) ;
      }
      else
      	(*pCurlEasySetopt)(pCurlEasyHandle, curlOption, sParam.c_str()) ;
    }
	}
}

/****************************************************************************/
/***                           CLASSE NSLibCurl                           ***/
/****************************************************************************/

XferInfo::XferInfo()
{
	_bytesTransferred = 0 ;
	_invocations = 0 ;

  _sFileName = string("") ;

	_stream = 0 ;
}

XferInfo::~XferInfo()
{
	close() ;
}

bool
XferInfo::init(string sFile, bool bForceReplace)
{
	if (string("") == sFile)
		return false ;

	_sFileName = sFile ;

	if (false == bForceReplace)
	{
		bool bExist = NsFileExists(_sFileName) ;
    if (true == bExist)
    {
    	string sWarningMsg = "Le fichier " + _sFileName + " existe déjà, voulez-vous le remplacer ?" ;
    	int retVal = ::MessageBox(0, sWarningMsg.c_str(), "Téléchargement", MB_YESNO) ;
      if (retVal == IDNO)
      	return false ;
    }
	}

	/*
	_outFile.open(_sFileName.c_str(), ios::trunc) ;
	if (!_outFile)
		return false ;
	*/

  _stream = fopen(_sFileName.c_str(), "wb") ;

	return true ;
}

void
XferInfo::write(const char* input, int size)
{
	_bytesTransferred += size ;
  _invocations++ ;

	if (!_outFile)
		return ;

	for (int i = 0; i < size; i++)
		_outFile.put(input[i]) ;
}

void
XferInfo::close()
{
	if (!_stream)
		return ;
	fclose(_stream) ;

	/*
	if (!_outFile)
		return ;
	_outFile.close() ;
	*/
}

