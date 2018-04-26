/*
** Copyright Nautilus, (10/9/2004)
** fabrice.le_perru@nautilus-info.com
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/

/*
 * Inspired by Twilio REST C++ from Laurent Luce
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <fcntl.h>
#include <sys/stat.h>
// #include <unistd.h>

#include <curl/nscurl.h>
// #if LIBCURL_VERSION_NUM < 0x071507
// #include <curl/types.h>
// #endif

using namespace std;

// #include "Utils.h"
#include "curl/nsRest.h"

const string Rest::API_URL  = string("http://api.vidal.fr") ;
const string Rest::API_REST = string("/rest/api") ;

/**
 * HTTP request
 * @param path URL path
 * @param method HTTP method: "GET", "POST", "PUT", "DELETE"
 * @param vars URL attributes or POST attributes
 * @return HTTP response
 */
string Rest::request(const string& path, const string& method, const vector<Var>& vars)
{
  string sResponse ;

  if (path.length() <= 0)
    throw "Path is empty";

  // Get an uppercase version of the method string
  //
  string m = method ;

  transform(m.begin(), m.end(), m.begin(), ::toupper) ;

  if ((string("GET") != m) && (string("POST") != m)
        && (string("PUT") != m) && (string("DELETE") != m))
  {
    throw "Invalid m parameter";
  }

  // Build the full URL
  //
  string url = build_uri(path) ;

  // Call the proper function
  //
  if      (string("GET") == m)
    sResponse = get(url, vars) ;

  else if (string("POST") == m)
    sResponse = post(url, vars) ;

  else if (string("PUT") == m)
    sResponse = put(url, vars[0].getValue()) ;

  else if (string("DELETE") == m)
    sResponse = tdelete(url) ;

  return sResponse ;
}

/**
 * Curl write function
 * @param data data received from server
 * @param size data size is size * nmemb
 * @param nmemb data size is size * nmemb
 * @param buffer
 */
static int writer(char *data, size_t size, size_t nmemb, string* psBuffer)
{
  int result = 0 ;
  if (psBuffer)
  {
    psBuffer->append(data, size * nmemb);
    result = size * nmemb ;
  }
  return result ;
}

/**
 * Curl read callback function
 * @param ptr pointer to storage
 * @param size data size is size * nmemb
 * @param nmemb data size is size * nmemb
 * @param userdata read from stream
 */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode = fread((FILE*)ptr, size, nmemb, (FILE*)stream) ;
  return retcode ;
}

/**
 * HTTP GET request
 * @param url HTTP request URL
 * @param vars URL attributes or POST attributes
 * @return HTTP response
 */
string Rest::get(const string& url, const vector<Var>& vars)
{
  NSLibCurl libCurl ;
  if ((NULL == libCurl.pCurlEasyInit)    ||
      (NULL == libCurl.pCurlEasySetopt)  ||
      (NULL == libCurl.pCurlEasyPerform) ||
      (NULL == libCurl.pCurlEasyCleanup))
		return string("") ;

  string u = add_vars_to_uri(url, vars) ;

  _sBuffer = string("HTTP get error") ;

  CURL *curl = (*libCurl.pCurlEasyInit)() ;
  if(curl)
  {
    //url = curl_easy_escape(curl, url.c_str(), url.length());
    string sAuth = string("") ;
    // if ((string("") != tid) || (string("") != ttoken))
    //  sAuth = tid + string(":") + ttoken ;

    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_URL, u.c_str()) ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC) ;
    if (string("") != sAuth)
      (*libCurl.pCurlEasySetopt)(curl, CURLOPT_USERPWD, sAuth.c_str()) ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_WRITEFUNCTION, writer) ;

    _sBuffer = string("") ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_WRITEDATA, &_sBuffer) ;

    CURLcode res = (*libCurl.pCurlEasyPerform)(curl) ;

    (*libCurl.pCurlEasyCleanup)(curl) ;
    if (CURLE_OK == res)
      return _sBuffer ;
  }

  return _sBuffer ;
}

/**
 * HTTP POST request
 * @param url HTTP request URL
 * @param vars POST attributes
 * @return HTTP response
*/
string Rest::post(const string& url, const vector<Var>& vars)
{
  NSLibCurl libCurl ;
  if ((NULL == libCurl.pCurlGlobalInit)  ||
      (NULL == libCurl.pCurlEasyInit)    ||
      (NULL == libCurl.pCurlEasySetopt)  ||
      (NULL == libCurl.pCurlEasyPerform) ||
      (NULL == libCurl.pCurlEasyCleanup))
		return string("") ;

  struct curl_httppost *formpost = NULL ;
  struct curl_httppost *lastptr = NULL ;
  static const char buf[] = "Expect:" ;

  (*libCurl.pCurlGlobalInit)(CURL_GLOBAL_ALL) ;

  // Post data
  for (unsigned int i = 0 ; i < vars.size() ; i++)
  {
    (*libCurl.pCurlFormAdd)(&formpost,
                            &lastptr,
                            CURLFORM_COPYNAME, vars[i].getKey().c_str(),
                            CURLFORM_COPYCONTENTS, vars[i].getValue().c_str(),
                            CURLFORM_END) ;
  }

  _sBuffer = string("HTTP post error") ;

  CURL *curl = (*libCurl.pCurlEasyInit)() ;
  struct curl_slist *headerlist = (*libCurl.pCurlSlistAppend)(headerlist, buf) ;

  if (curl)
  {
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_URL, url.c_str()) ;

    // string sAuth = tid + string(":") + ttoken ;
    string sAuth = string("") ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC) ;
    if (string("") != sAuth)
      (*libCurl.pCurlEasySetopt)(curl, CURLOPT_USERPWD, sAuth.c_str()) ;

    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_HTTPHEADER, headerlist) ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_HTTPPOST, formpost) ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_WRITEFUNCTION, writer) ;
    _sBuffer = string("") ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_WRITEDATA, &_sBuffer) ;

    CURLcode res = (*libCurl.pCurlEasyPerform)(curl) ;

    (*libCurl.pCurlEasyCleanup)(curl) ;
    (*libCurl.pCurlFormFree)(formpost) ;
    (*libCurl.pCurlSlistFreeAll)(headerlist) ;
    (*libCurl.pCurlGlobalCleanup)() ;

    if (CURLE_OK == res)
      return _sBuffer ;
  }

  return _sBuffer ;
}

/**
 * HTTP PUT request
 * @param url HTTP request URL
 * @param filename File to read data from
 * @return HTTP response
*/
string Rest::put(const string& url, const string& filename)
{
  NSLibCurl libCurl ;
  if ((NULL == libCurl.pCurlEasyInit)    ||
      (NULL == libCurl.pCurlEasySetopt)  ||
      (NULL == libCurl.pCurlEasyPerform) ||
      (NULL == libCurl.pCurlEasyCleanup))
		return string("") ;

  struct stat file_info;

  // get local file size
  // int hd = open(filename.c_str(), O_RDONLY);
  // fstat(hd, &file_info);
  // close(hd);

  FILE* hd_src = fopen(filename.c_str(), "rb");

  (*libCurl.pCurlGlobalInit)(CURL_GLOBAL_ALL) ;

  _sBuffer = string("HTTP put error") ;
  CURL *curl = (*libCurl.pCurlEasyInit)() ;

  if (curl)
  {
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_URL, url.c_str()) ;

    // string sAuth = tid + ":" + ttoken ;
    string sAuth = string("") ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC) ;
    if (string("") != sAuth)
      (*libCurl.pCurlEasySetopt)(curl, CURLOPT_USERPWD, sAuth.c_str()) ;

    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_READFUNCTION, read_callback) ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_UPLOAD, 1L) ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_READDATA, hd_src) ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_INFILESIZE_LARGE,
                      (curl_off_t)file_info.st_size) ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_WRITEFUNCTION, writer) ;
    _sBuffer = string("") ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_WRITEDATA, &_sBuffer) ;

    CURLcode res = (*libCurl.pCurlEasyPerform)(curl) ;

    (*libCurl.pCurlEasyCleanup)(curl) ;
    (*libCurl.pCurlGlobalCleanup)() ;
    if (CURLE_OK == res)
    {
      fclose(hd_src) ;
      return _sBuffer ;
    }
  }

  fclose(hd_src) ;
  return _sBuffer ;
}

/**
 * HTTP DELETE request
 * @param url HTTP request URL
 * @return HTTP response
*/
string Rest::tdelete(const string& url)
{
  NSLibCurl libCurl ;
  if ((NULL == libCurl.pCurlEasyInit)    ||
      (NULL == libCurl.pCurlEasySetopt)  ||
      (NULL == libCurl.pCurlEasyPerform) ||
      (NULL == libCurl.pCurlEasyCleanup))
		return string("") ;

  _sBuffer = string("HTTP delete error") ;

  CURL *curl = (*libCurl.pCurlEasyInit)() ;
  if (curl)
  {
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_URL, url.c_str()) ;

    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC) ;
    // string sAuth = tid + string(":") + ttoken ;
    string sAuth = string("") ;
    if (string("") != sAuth)
      (*libCurl.pCurlEasySetopt)(curl, CURLOPT_USERPWD, sAuth.c_str()) ;

    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_CUSTOMREQUEST, "DELETE") ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_WRITEFUNCTION, writer) ;
    _sBuffer = string("") ;
    (*libCurl.pCurlEasySetopt)(curl, CURLOPT_WRITEDATA, &_sBuffer) ;

    CURLcode res = (*libCurl.pCurlEasyPerform)(curl) ;

    (*libCurl.pCurlEasyCleanup)(curl) ;
    if (CURLE_OK == res)
      return _sBuffer ;
  }

  return _sBuffer ;
}

/**
 * Build a full URL
 * @param path URL path
 * @return full URL
 */
string Rest::build_uri(const string& sPath) const
{
  bool bAddRestPath = true ;

  // Don't need to add the rest path if path has it already at start
  //
  size_t iRestPathLen = strlen(API_REST.c_str()) ;
  if ((strlen(sPath.c_str()) > iRestPathLen) &&
                      (string(sPath, 0, iRestPathLen) == API_REST))
    bAddRestPath = false ;

  string sFullUrl = API_URL ;
  if (bAddRestPath)
    sFullUrl += API_REST ;

  // If path doesn't start with a '/', we must add it
  //
  if (string("") != sPath)
  {
    if ('/' != sPath[0])
      sFullUrl += string("/") ;
    sFullUrl += sPath ;
  }

  return sFullUrl ;
}

/**
 * Build a full Bam URL by addition of vars
 * @param path URL path
 * @return full URL
 */
string Rest::add_vars_to_uri(const string& url, const vector<Var>& vars) const
{
  string sQuery = string("") ;
  for (unsigned int i = 0 ; i < vars.size() ; i++)
  {
    if (string("") != sQuery)
      sQuery += string("&") ;
    sQuery += vars[i].getKey() + string("=") + vars[i].getValue() ;
  }

  string u = url ;
  if (string("") != sQuery)
  {
    if (u.find("?") == string::npos)
      u += string("?") ;
    else
      u += string("&") ;
    u += sQuery ;
  }

  return u ;
}

