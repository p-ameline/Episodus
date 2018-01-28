//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
//#include <windows.h>

#ifndef __NS_ODBC_H
#define __NS_ODBC_H

# if defined(_DANSSGBDDLL)
#  define _CLASSESGBD __export
# else
#  define _CLASSESGBD __import
# endif

# include <string>
# include <cstring>

# include <windows.h>

# include <sql.h>
# include <sqltypes.h>
# include <sqlext.h>

# include <vector>

# ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
# else
  #include "partage/nsglobal.h"
# endif

# include "nautilus\nssuper.h"

struct ColDescription
{
  SQLSMALLINT colNumber ;
  SQLCHAR     colName[80] ;
  SQLSMALLINT nameLen ;
  SQLSMALLINT dataType ;
  // SQLULEN     colSize ;
  SQLUINTEGER colSize ;
  SQLSMALLINT decimalDigits ;
  SQLSMALLINT nullable ;
} COLDESC ;

struct _CLASSESGBD ResultLine
{
  std::vector<std::string> aColumns ;
} ;

class ResultSetColBind
{
  public:

    SQLUSMALLINT iColumnNumber ;
    SQLSMALLINT  iTargetType ;

    // SQLPOINTER   pTargetValuePtr ;
    CHAR*        pCharTargetValuePtr ;
    WCHAR*       pWCharTargetValuePtr ;
    SQLINTEGER*  pLongTargetValuePtr ;

    SQLINTEGER   iBufferLength ;
    SQLINTEGER   iReturnedStrLen ;

    ResultSetColBind() ;
    ~ResultSetColBind() ;
} ;

#ifndef __linux__
class _CLASSESGBD NSOdbcConnector : public NSSuperRoot
#else
class NSOdbcConnector : public NSSuperRoot
#endif
{
  // Attributes
  public:

    // vector<ColDescription> cols;
    // vector< vector<string> > colData;

  // Operations
  public:

    NSOdbcConnector(NSSuper* pSuper, std::string sConnectorName) ;
    ~NSOdbcConnector() ;

    bool isReady() { return _bReady ; }

    // int       ExecuteRequest(std::string sRequest, std::vector<std::vector<std::string>>* pColData) ;
    int       ExecuteRequest(std::string sRequest, std::vector<ResultLine>* pColData) ;

    SQLRETURN GetResultset() ;
    void      DescribeColumns() ;

    void      TraceOdbcSources() ;

  private:

    SQLHANDLE _EnvHandle ;
    SQLHANDLE _ConHandle ;
    SQLHANDLE _StmtHandle ;
    SQLRETURN _rcAlloc ;

    bool      _bReady ;

    vector<ColDescription> _aColsDesc ;

    void extract_error(std::string sMsg, SQLHANDLE handle, SQLSMALLINT type, NSSuper::TRACETYPE iTraceType) ;

    SQLRETURN Execute(std::string sRequest) ;
    SQLRETURN GetRowCount(SQLINTEGER *iRowCount) ;
    SQLRETURN GetColCount(SQLSMALLINT *iColumnsCount) ;
    SQLRETURN GetColDescription(int iColumnsCount) ;
    SQLRETURN BindColumns(ResultSetColBind** pColumnsData) ;
    SQLRETURN BindCol(ResultSetColBind* pColData, SQLPOINTER pTargetValuePtr) ;
    SQLRETURN FetchData(ResultSetColBind** pColumnsData, std::vector<ResultLine>* pColData, SQLSMALLINT iColumnsCount) ;

    SQLRETURN GetColData(int colnum, std::string& str) ;

    void      freeStmtHandle() ;

    // std::string ToNarrow(const wchar_t *s, char dfault = '?', const std::locale& loc = std::locale()) ;
    std::string ToNarrow(const wchar_t *s) ;

    std::string GetDataTypeAsString(const SQLSMALLINT iDataType) ;
};

#endif   // __NS_ODBC_H
