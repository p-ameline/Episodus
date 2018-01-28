//---------------------------------------------------------------------------#ifndef _OLE_UTILS_H
#define _OLE_UTILS_H

#include "partage\nsdivfct.h"

// Get a STL string from a BSTR
//
string BSTRtoSTLstring(BSTR bstrText)
{
try
{
	string sResult = string("") ;

  if (NULL == bstrText)
  	return sResult ;

	// Convert the text from Unicode to ANSI
	char *ansistr ;
	int lenW = ::SysStringLen(bstrText) ;
	int lenA = ::WideCharToMultiByte(CP_ACP, 0, bstrText, lenW, 0, 0, NULL, NULL) ;
	if (lenA <= 0)
  	return sResult ;

	ansistr = new char[lenA + 1] ; // allocate a final null terminator as well
	::WideCharToMultiByte(CP_ACP, 0, bstrText, lenW, ansistr, lenA, NULL, NULL) ;
	ansistr[lenA] = 0 ; // Set the null terminator yourself

	sResult = string(ansistr) ;

  delete[] ansistr ;

  return sResult ;
}
catch (...)
{
  erreur("Exception BSTRtoSTLstring.", standardError, 0) ;
  return string("") ;
}
}

//
// Take an ANSI char string & return a wide string in a new'd wchar_t[] buffer
//
wchar_t* stringToWchart(string sSrc)
{
try
{
  size_t size = MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), strlen(sSrc.c_str()), 0, 0) ;
  wchar_t* dst = new wchar_t[size + 1] ;

  size = MultiByteToWideChar(CP_ACP, 0, sSrc.c_str(), strlen(sSrc.c_str()), dst, size) ;
  dst[size] = 0 ;

  return dst ;
}
catch (...)
{
  erreur("Exception stringToWchart.", standardError, 0) ;
  return (wchar_t*) 0 ;
}
}

#endif  // _OLE_UTILS_H

