//-------------------------------------------------------------------------//                   HEADER FILE : NSREST.H
//
//                   Rest interface with LibCurl
//
//             Initial release : August 29 2007 (PA)
//             Final release   : August 29 2007 (PA)
//-------------------------------------------------------------------------
#ifndef __NSREST_H
#define __NSREST_H

#ifndef __NSCURL_H
  class NSLibCurl ;
#endif

#ifndef CURL_EXTERN
  class CURL ;
#endif

#include "nsoutil\nsexport.h"

/** struct to contain the key and value for URL attributes or Body data */
struct Var
{
  public:

    Var(const string& k, const string& v)
    {
      _sKey   = k ;
      _sValue = v ;
    }

    Var(const Var& rv)
    {
      _sKey   = rv._sKey ;
      _sValue = rv._sValue ;
    }

    string getKey()   { return _sKey ; }
    string getValue() { return _sValue ; }

    void   setKey(string sK)   { _sKey   = sK ; }
    void   setValue(string sV) { _sValue = sV ; }

  protected:

    string _sKey ;
    string _sValue ;
};
typedef struct Var Var ;

class _NSOUTILCLASSE Rest
{
  public:

    /**
     * Constructor
     * @param id Twilio account SID
     * @param token Twilio account token
     */
    Rest()
    {
      _sBuffer = string("") ;
    }

    /**
     * HTTP request
     * @param path URL path
     * @param method HTTP method: "GET", "POST", "PUT", "DELETE"
     * @param vars URL attributes or POST attributes
     * @return HTTP response
     */
    string request(const string& path, const string& method, const vector<Var>& vars) ;

    /**
     * Prefix path with Bam URL
     * @param path URL path
     * @return full URL
     */
    string build_uri(const string& path) const ;

    /**
     * Build a full Bam URL by addition of vars
     * @param path URL path
     * @return full URL
     */
    string add_vars_to_uri(const string& url, const vector<Var>& vars) const ;
    string add_escaped_vars_to_uri(NSLibCurl* pLibCurl, CURL *curl, const string& url, const vector<Var>& vars) const ;

  private:

    /** Response buffer */
    string _sBuffer ;

    /** API URL and REST path - set in cpp */
    static const string API_URL ;
    static const string API_REST ;

    /**
     * HTTP GET request
     * @param url HTTP request URL
     * @param vars URL attributes
     * @return HTTP response
     */
    string get(const string& url, const vector<Var>& vars) ;
    /**
     * HTTP POST request
     * @param url HTTP request URL
     * @param vars POST attributes
     * @return HTTP response
     */
    string post(const string& url, const vector<Var>& vars) ;
    /**
     * HTTP PUT request
     * @param url HTTP request URL
     * @param filename File to read data from
     * @return HTTP response
    */
    string put(const string& url, const string& filename) ;
    /**
     * HTTP DELETE request
     * @param url HTTP request URL
     * @return HTTP response
    */
    string tdelete(const string& url) ;
};

#endif 		// __NSREST_H

