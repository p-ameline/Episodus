//---------------------------------------------------------------------------//    NS_ODBC.CPP
//
//  ODBC Connector
//
//  1ère version : PA 14 dec 2015   Dernière modif : 14 dec 2015
//---------------------------------------------------------------------------

#include <sstream>

#include "ns_sgbd\ns_odbc.h"
#include "nautilus\nssuper.h"
#include "partage\ole_utils.h"

NSOdbcConnector::NSOdbcConnector(NSSuper* pSuper, std::string sConnectorName)
                :NSSuperRoot(pSuper)
{
  _bReady = false ;

  _EnvHandle  = NULL ;
  _ConHandle  = NULL ;
  _StmtHandle = NULL ;

	// Allocate An Environment Handle
  //
	_rcAlloc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_EnvHandle) ;

  if (SQL_SUCCESS != _rcAlloc)
  {
    std::string sErrorMsg = std::string("ODBC Connector: Cannot allocate an Environment Handle") ;
    _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;
    return ;
  }

	// Set The ODBC Application Version To 3.x
  //
  _rcAlloc = SQLSetEnvAttr(_EnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER) ;

  if (false == SQL_SUCCEEDED(_rcAlloc))
  {
    std::string sErrorMsg = std::string("ODBC Connector: failed setting the ODBC application version to 3.x") ;
    _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;
    return ;
  }

	// Allocate A Connection Handle
  //
  _rcAlloc = SQLAllocHandle(SQL_HANDLE_DBC, _EnvHandle, &_ConHandle) ;

  if (false == SQL_SUCCEEDED(_rcAlloc))
  {
    std::string sErrorMsg = std::string("ODBC Connector: Cannot allocate a Connection Handle") ;
    _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;
    return ;
  }

  if (std::string("") == sConnectorName)
    return ;

  // Connect To The Database
  //
  _rcAlloc = SQLConnect(_ConHandle, (unsigned char*) sConnectorName.c_str(), SQL_NTS, (SQLCHAR *) "", SQL_NTS, (SQLCHAR *) "", SQL_NTS) ;

  // SQLCHAR     outstr[1024] ;
  // SQLSMALLINT outstrlen ;
  // _rcAlloc = SQLDriverConnect(_ConHandle, NULL, (unsigned char*) sConnectorName.c_str(), SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_COMPLETE) ;

  if (SQL_SUCCEEDED(_rcAlloc))
  {
    // std::string sMsg = std::string("ODBC Connector: Succesfuly connected to ") + sConnectorName + std::string(" with connection string ")  + std::string((char*) outstr) ;
    std::string sMsg = std::string("ODBC Connector: Succesfuly connected to ") + sConnectorName ;
    _pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

    if (SQL_SUCCESS_WITH_INFO == _rcAlloc)
      extract_error("ODBC driver reported the following diagnostics:", _ConHandle, SQL_HANDLE_DBC, NSSuper::trDetails) ;
  }
  else
  {
    std::string sErrorMsg = std::string("ODBC Connector: Cannot connect to ") + sConnectorName ;
    _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;
    extract_error("ODBC driver reported the following errors during connexion:", _ConHandle, SQL_HANDLE_DBC, NSSuper::trError) ;

    TraceOdbcSources() ;

    return ;
  }

  _bReady = true ;

  std::string sMsg = string("ODBC Connector: Succesfuly connected to ") + sConnectorName ;
  _pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;
}

NSOdbcConnector::~NSOdbcConnector()
{
  // Free The Statement Handle
  if (NULL != _StmtHandle)
    SQLFreeHandle(SQL_HANDLE_STMT, _StmtHandle) ;

  // Disconnect From The Northwind Sample Database
  if (NULL != _ConHandle)
    SQLDisconnect(_ConHandle) ;

  // Free The Connection Handle
  if (NULL != _ConHandle)
    SQLFreeHandle(SQL_HANDLE_DBC, _ConHandle) ;
	// Free The Environment Handle
  if (NULL != _EnvHandle)
		SQLFreeHandle(SQL_HANDLE_ENV, _EnvHandle) ;
}

int
NSOdbcConnector::ExecuteRequest(std::string sRequest, std::vector<ResultLine>* pColData)
{
  if (false == isReady())
    return 1 ;

  if ((std::string("") == sRequest) || ((std::vector<ResultLine>*) NULL == pColData))
    return 1 ;

  // Allocate An SQL Statement Handle
  //
  SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, _ConHandle, &_StmtHandle) ;

  if (false == SQL_SUCCEEDED(ret))
  {
    std::string sErrorMsg = std::string("NSOdbcConnector::ExecuteRequest: Cannot allocate a Statement Handle") ;
    _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;
    return 1 ;
  }

  // Execute the command
  //
  ret = Execute(sRequest) ;

  if (false == SQL_SUCCEEDED(ret))
    return 1 ;

  std::string sErrorMsg = std::string("NSOdbcConnector::ExecuteRequest: Request succeeded: ") + sRequest ;
  _pSuper->trace(&sErrorMsg, 1, NSSuper::trDetails) ;

  // Get the number of columns in result set
  //
  SQLSMALLINT iColumnsCount ;
  ret = GetColCount(&iColumnsCount) ;

  if (false == SQL_SUCCEEDED(ret))
    return 1 ;

  // Get columns types
  //
  // From msdn:
  // An application typically calls SQLDescribeCol after a call to
  // SQLPrepare and before or after the associated call to SQLExecute.
  // An application can also call SQLDescribeCol after a call to SQLExecDirect.
  //
  ret = GetColDescription(iColumnsCount) ;

  if (false == SQL_SUCCEEDED(ret))
    return 1 ;

  // Bind columns to a data collection array
  //
  ResultSetColBind** ppColumnsData = new ResultSetColBind*[iColumnsCount] ;
  ret = BindColumns(ppColumnsData) ;

  if (false == SQL_SUCCEEDED(ret))
    return 1 ;

  // Get information in result set
  //
  ret = FetchData(ppColumnsData, pColData, iColumnsCount) ;

  if (false == SQL_SUCCEEDED(ret))
    return 1 ;

  // Free The Statement Handle
  //
  freeStmtHandle() ;

  // Delete binding objects
  //
  for (int i = 0 ; i < iColumnsCount ; i++)
    if (ppColumnsData[i])
      delete ppColumnsData[i] ;

  delete[] ppColumnsData ;

  return 0 ;
}

// Get the data for one column and return the info in the form
// of a std::string.  The ODBC driver will make all necessary
// data conversions from whatever type the data is in the database
// to SQL_CHAR.  You could make this function more comples by
// getting the return type as it appears in the database then constructing
// a VARIANT object to hold the data.
SQLRETURN
NSOdbcConnector::GetColData(int colnum, string& str)
{
	SQLCHAR buf[255] = {0} ;
  SQLRETURN ret = SQLGetData(_StmtHandle, colnum, SQL_CHAR, buf, sizeof(buf), NULL) ;

	if (SQL_SUCCEEDED(ret))
		str = reinterpret_cast<char*>(buf) ;

	return ret ;
}

//
// Define The ShowResults() Member Function
SQLRETURN
NSOdbcConnector::GetResultset()
{
/*
	// Get all column description
	DescribeColumns();
	// erase anything that's in the colData vector
	colData.clear();
	// fetch a row from the resultset
	while (SQLFetch(_StmtHandle) == SQL_SUCCESS)
	{
		// vector of strings to hold the column data
		vector<string> col;
		string data;
		// column counter
		int i = 1;
		// get the data for each column and add it to
		// the col vector
		while( GetColData(i, data) == SQL_SUCCESS)
		{
			col.push_back(data);
			++i; // increment the column number
		}
		// add column data to the colData vector
		colData.push_back(col);
	}
*/
	return SQL_SUCCESS;
}

// From http://www.easysoft.com/developer/languages/c/odbc_tutorial.html
//
void
NSOdbcConnector::extract_error(std::string sMsg, SQLHANDLE handle, SQLSMALLINT type, NSSuper::TRACETYPE iTraceType)
{
  _pSuper->trace(&sMsg, 1, iTraceType) ;

  SQLINTEGER  i = 0;
  SQLINTEGER  native;
  SQLCHAR     state[6] ;
  SQLCHAR     text[SQL_MAX_MESSAGE_LENGTH] ;
  SQLSMALLINT len;
  SQLRETURN	  ret;

  do
  {
    ret = SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &len) ;
    if (SQL_SUCCEEDED(ret))
    {
      // printf("%s:%ld:%ld:%s", state, i, native, text) ;
      std::string sErrorMsg = std::string((char*) text) ;
      _pSuper->trace(&sErrorMsg, 1, iTraceType) ;
    }
  }
  while (SQL_SUCCEEDED(ret)) ;
}

SQLRETURN
NSOdbcConnector::Execute(string sRequest)
{
  if ((string("") == sRequest) || (NULL == _StmtHandle))
    return SQL_ERROR ;

  // Version Introduced: ODBC 1.0 Standards Compliance: ISO 92
  //
  // SQLExecDirect executes a preparable statement, using the current values of
  // the parameter marker variables if any parameters exist in the statement.
  //
  // SQLExecDirect is the fastest way to submit an SQL statement for one-time execution.
  //
  SQLRETURN ret = SQLExecDirect(_StmtHandle, (SQLCHAR*) sRequest.c_str(), SQL_NTS) ;

  if (false == SQL_SUCCEEDED(ret))
  {
    std::string sErrorMsg = std::string("NSOdbcConnector Request failed: ") + sRequest ;
    _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;

    extract_error("ODBC connector reported the following errors during request:", _StmtHandle, SQL_HANDLE_STMT, NSSuper::trError) ;

    freeStmtHandle() ;
  }

  return ret ;
}

// Get the number of rows modified/inserted/deleted by a query.
//
SQLRETURN
NSOdbcConnector::GetRowCount(SQLINTEGER *iRowsCount)
{
  if (NULL == _StmtHandle)
    return SQL_ERROR ;

  // Version Introduced: ODBC 1.0 Standards Compliance: ISO 92
  //
  // SQLRowCount returns the number of rows affected by an UPDATE, INSERT, or
  // DELETE statement; an SQL_ADD, SQL_UPDATE_BY_BOOKMARK, or
  // SQL_DELETE_BY_BOOKMARK operation in SQLBulkOperations; or an SQL_UPDATE or
  // SQL_DELETE operation in SQLSetPos.
  //
  SQLRETURN ret = SQLRowCount(_StmtHandle, iRowsCount) ;

  if (false == SQL_SUCCEEDED(ret))
  {
    std::string sErrorMsg = std::string("NSOdbcConnector Cannot get the number of rows in result set") ;
    _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;

    extract_error("ODBC driver reported the following errors when getting the number of rows in result set:", _StmtHandle, SQL_HANDLE_STMT, NSSuper::trError) ;

    freeStmtHandle() ;
  }
  else
  {
    std::string sMsg = std::string("ODBC request returned ") + LongToString(*iRowsCount) + std::string(" rows") ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
  }

  return ret ;
}

// Get the number of columns in result set.
//
SQLRETURN
NSOdbcConnector::GetColCount(SQLSMALLINT *iColumnsCount)
{
  if (NULL == _StmtHandle)
    return SQL_ERROR ;

  // Version Introduced: ODBC 1.0 Standards Compliance: ISO 92
  //
  // SQLNumResultCols returns the number of columns in a result set.
  //
  SQLRETURN ret = SQLNumResultCols(_StmtHandle, iColumnsCount) ;

  if (false == SQL_SUCCEEDED(ret))
  {
    std::string sErrorMsg = std::string("NSOdbcConnector Cannot get the number of columns in result set") ;
    _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;

    extract_error("ODBC driver reported the following errors when getting the number of columns in result set:", _StmtHandle, SQL_HANDLE_STMT, NSSuper::trError) ;

    freeStmtHandle() ;
  }
  else
  {
    std::string sMsg = std::string("The result set contains ") + IntToString(*iColumnsCount) + std::string(" columns") ;
    _pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
  }

  return ret ;
}

// Get the description for all the columns in the resultset.
//
SQLRETURN
NSOdbcConnector::GetColDescription(int iColumnsCount)
{
  if (NULL == _StmtHandle)
    return SQL_ERROR ;

  _aColsDesc.clear() ;

  if (0 == iColumnsCount)
    return SQL_SUCCESS ;

  std::string sMsg = string("") ;

  for (int i = 0 ; i < iColumnsCount ; i++)
  {
    ColDescription c ;
	  c.colNumber = i + 1 ;

    // Version Introduced: ODBC 1.0 Standards Compliance: ISO 92
    //
    // SQLDescribeCol returns the result descriptor — column name, type,
    // column size, decimal digits, and nullability — for one column in the
    // result set. This information also is available in the fields of the IRD.
    //
    SQLRETURN ret = SQLDescribeCol(_StmtHandle, c.colNumber,
		                               c.colName, sizeof(c.colName), &c.nameLen,
		                               &c.dataType, &c.colSize,
                                   &c.decimalDigits, &c.nullable) ;

    if (false == SQL_SUCCEEDED(ret))
    {
      std::string sErrorMsg = std::string("NSOdbcConnector Cannot get the description for a column in result set") ;
      _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;

      extract_error("ODBC driver reported the following errors when getting the description for a column in result set:", _StmtHandle, SQL_HANDLE_STMT, NSSuper::trError) ;

      freeStmtHandle() ;

      return ret ;
    }

    _aColsDesc.push_back(c) ;

    sMsg += string("\"") + string((char*) c.colName) + string("\" (") + GetDataTypeAsString(c.dataType) + string(", ") + IntToString(c.colSize) + string(") ") ;
  }

  _pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	return SQL_SUCCESS ;
}

SQLRETURN
NSOdbcConnector::BindColumns(ResultSetColBind** pColumnsData)
{
  if ((NULL == _StmtHandle) || ((ResultSetColBind**) NULL == pColumnsData))
    return SQL_ERROR ;

  if (_aColsDesc.empty())
    return SQL_SUCCESS ;

  int iNbColsToBind = _aColsDesc.size() ;

  vector<ColDescription>::iterator it = _aColsDesc.begin() ;

  for (int i = 0 ; _aColsDesc.end() != it ; it++, i++)
  {
    pColumnsData[i] = new ResultSetColBind ;

    pColumnsData[i]->iColumnNumber = i + 1 ;

    //  Note that the size is count of bytes (for Unicode).
    //
    // All ODBC functions that take SQLPOINTER use count of bytes ;
    // all functions that take only strings use count of characters.
    //
    switch((*it).dataType)
    {
      case SQL_CHAR :
      case SQL_VARCHAR :

        pColumnsData[i]->iTargetType         = SQL_C_CHAR ;
        pColumnsData[i]->pCharTargetValuePtr = (SQLCHAR *) malloc ((*it).colSize + 1) ;
        pColumnsData[i]->iBufferLength       = ((*it).colSize + 1) * sizeof(SQLCHAR) ;

        BindCol(pColumnsData[i], pColumnsData[i]->pCharTargetValuePtr) ;

        break ;

      case SQL_INTEGER :

        pColumnsData[i]->iTargetType         = SQL_C_LONG ;
        pColumnsData[i]->pLongTargetValuePtr = (SQLINTEGER *) malloc ((*it).colSize + 1) ;
        pColumnsData[i]->iBufferLength       = ((*it).colSize + 1) * sizeof(SQLINTEGER) ;

        BindCol(pColumnsData[i], pColumnsData[i]->pLongTargetValuePtr) ;

        break ;

      case SQL_WCHAR :
      case SQL_WVARCHAR :

        pColumnsData[i]->iTargetType          = SQL_C_TCHAR ;
        pColumnsData[i]->iBufferLength        = ((*it).colSize + 1) * sizeof(WCHAR) ;
        pColumnsData[i]->pWCharTargetValuePtr = (WCHAR*) malloc(((*it).colSize + 1) * sizeof(WCHAR)) ;

        BindCol(pColumnsData[i], pColumnsData[i]->pWCharTargetValuePtr) ;
    }

/*
    switch((*it).dataType)
    {
      case SQL_CHAR :

        pColumnsData[i]->pTargetValuePtr = new SQLCHAR[(*it).colSize] ; // new SQLCHAR[512] ;
        BindCol(pColumnsData[i]) ;

        break ;

      case SQL_VARCHAR :

        pColumnsData[i]->pTargetValuePtr = new SQLCHAR[(*it).colSize] ; // new SQLCHAR[512] ;
        BindCol(pColumnsData[i]) ;

        break ;

     case SQL_VARCHAR:

             ColumnsData[i] = new SQLCHAR**[*RowCount];
             for(int l=0;l<*RowCount;l++)
             {
                 ((SQLCHAR**)ColumnsData[i])[l] = new SQLCHAR[512];
             }

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_VARCHAR,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_LONGVARCHAR:

             ColumnsData[i] = new SQLCHAR**[*RowCount];
             for(int l=0;l<*RowCount;l++)
             {
                 ((SQLCHAR**)ColumnsData[i])[l] = new SQLCHAR[512];
             }

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_LONGVARCHAR,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_WCHAR:

             ColumnsData[i] = new SQLWCHAR**[*RowCount];
             for(int l=0;l<*RowCount;l++)
             {
                 ((SQLWCHAR**)ColumnsData[i])[l] = new SQLWCHAR[512];
             }

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_WCHAR,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_WVARCHAR:

             ColumnsData[i] = new SQLWCHAR**[*RowCount];
             for(int l=0;l<*RowCount;l++)
             {
                 ((SQLWCHAR**)ColumnsData[i])[l] = new SQLWCHAR[512];
             }

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_WVARCHAR,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_WLONGVARCHAR:

             ColumnsData[i] = new SQLWCHAR**[*RowCount];
             for(int l=0;l<*RowCount;l++)
             {
                 ((SQLWCHAR**)ColumnsData[i])[l] = new SQLWCHAR[512];
             }

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_WLONGVARCHAR,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_DECIMAL:

             ColumnsData[i] = new SQL_NUMERIC_STRUCT[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_DECIMAL,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_NUMERIC:

             ColumnsData[i] = new SQL_NUMERIC_STRUCT[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_NUMERIC,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_SMALLINT:

             ColumnsData[i] = new SQLSMALLINT[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_SMALLINT,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_INTEGER:

            ColumnsData[i] = new SQLINTEGER[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_INTEGER,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_REAL:

             ColumnsData[i] = new SQLREAL[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_REAL,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_FLOAT:

             ColumnsData[i] = new SQLREAL[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_FLOAT,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_DOUBLE:

             ColumnsData[i] = new SQLDOUBLE[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_DOUBLE,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_BIT:

             ColumnsData[i] = new SQLCHAR[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_BIT,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_TINYINT:

             ColumnsData[i] = new SQLSCHAR[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_TINYINT,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_BIGINT:

             ColumnsData[i] = new SQLBIGINT[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_BIGINT,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_BINARY:

             ColumnsData[i] = new SQLCHAR**[*RowCount];
             for(int l=0;l<*RowCount;l++)
             {
                 ((SQLCHAR**)ColumnsData[i])[l] = new SQLCHAR[512];
             }

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_BINARY,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_TYPE_DATE:

            ColumnsData[i] = new SQL_DATE_STRUCT[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

//>>>>>>>>>>>>>>>
             SQLBindCol(hStmt,i+1,SQL_TYPE_DATE,ColumnsData[i],512,rowLengths[i]);
             break;

     case SQL_TYPE_TIME:

             ColumnsData[i] = new SQL_TIME_STRUCT[*RowCount];

             rowLengths[i] = new SQLINTEGER[*RowCount];

             SQLBindCol(hStmt,i+1,SQL_TYPE_TIME,ColumnsData[i],512,rowLengths[i]);
             break;

      default:
        break ;
    }
*/
  }

  return SQL_SUCCESS ;
}

SQLRETURN
NSOdbcConnector::BindCol(ResultSetColBind* pColData, SQLPOINTER pTargetValuePtr)
{
  if (((ResultSetColBind*) NULL == pColData) || (NULL == pTargetValuePtr))
    return SQL_ERROR ;

  // Version Introduced: ODBC 1.0 Standards Compliance: ISO 92
  //
  // SQLBindCol binds application data buffers to columns in the result set.
  //
  return SQLBindCol(_StmtHandle,
                    pColData->iColumnNumber,
                    pColData->iTargetType,
                    pTargetValuePtr,
                    pColData->iBufferLength,
                    &(pColData->iReturnedStrLen)) ;
}

SQLRETURN
NSOdbcConnector::FetchData(ResultSetColBind** pColumnsData, std::vector<ResultLine>* pColData, SQLSMALLINT iColumnsCount)
{
  if ((NULL == _StmtHandle) || ((std::vector<ResultLine>*) NULL == pColData) ||
                                   ((ResultSetColBind**) NULL == pColumnsData))
    return SQL_ERROR ;

  bool bRemaining = true ;

  while (bRemaining)
  {
    // Version Introduced: ODBC 1.0 Standards Compliance: ISO 92
    //
    // SQLFetch fetches the next rowset of data from the result set and returns
    // data for all bound columns.
    //
    SQLRETURN ret = SQLFetch(_StmtHandle) ;

    // No more rows
    //
    if (SQL_NO_DATA == ret)
    {
      bRemaining = false ;

      string sResult = string("NSOdbcConnector::FetchData No more row") ;
      _pSuper->trace(&sResult, 1, NSSuper::trSubDetails) ;
    }
    //
    //
    else
    {
      if (false == SQL_SUCCEEDED(ret))
      {
        std::string sErrorMsg = std::string("NSOdbcConnector Error during SQLFetch") ;
        _pSuper->trace(&sErrorMsg, 1, NSSuper::trError) ;

        extract_error("ODBC driver reported the following errors when calling SQLFetch:", _StmtHandle, SQL_HANDLE_STMT, NSSuper::trError) ;

        // Unbind all columns
        //
        SQLFreeStmt(_StmtHandle, SQL_UNBIND) ;

        SQLCancel(_StmtHandle) ;
        freeStmtHandle() ;

        return ret ;
      }

      ResultLine newLine ;

      string sResult = string("") ;

      for (size_t j = 0 ; j < iColumnsCount ; j++)
      {
        std::string sData = std::string("") ;

        switch(_aColsDesc[j].dataType)
        {
          case SQL_CHAR :
          case SQL_VARCHAR :

            if (pColumnsData[j]->pCharTargetValuePtr)
              sData = std::string(pColumnsData[j]->pCharTargetValuePtr) ;
            break ;

          case SQL_INTEGER :

            if (pColumnsData[j]->pLongTargetValuePtr)
              sData = LongToString(*(pColumnsData[j]->pLongTargetValuePtr)) ;
            break ;

          case SQL_WCHAR :
          case SQL_WVARCHAR :

            if (pColumnsData[j]->pWCharTargetValuePtr)
              sData = BSTRtoSTLstring(pColumnsData[j]->pWCharTargetValuePtr) ;

            break ;
        }

        newLine.aColumns.push_back(sData) ;

        sResult += string("\"") + sData + string("\" ") ;
      }

      pColData->push_back(newLine) ;

      _pSuper->trace(&sResult, 1, NSSuper::trSubDetails) ;

      if (SQL_SUCCESS_WITH_INFO == ret)
        extract_error("ODBC additional info:", _StmtHandle, SQL_HANDLE_STMT, NSSuper::trDetails) ;
    }
  }

  // Unbind all columns
  //
  SQLFreeStmt(_StmtHandle, SQL_UNBIND) ;

  return SQL_SUCCESS ;
}

void
NSOdbcConnector::TraceOdbcSources()
{
  std::string sMsg = string("Available ODBC sources:") ;
  _pSuper->trace(&sMsg, 1, NSSuper::trWarning) ;

  SQLSMALLINT dsn_ret ;
  SQLSMALLINT desc_ret ;
  SQLRETURN   ret ;

  char dsn[256] ;
  char desc[256] ;

  SQLUSMALLINT direction = SQL_FETCH_FIRST ;

  while (SQL_SUCCEEDED(ret = SQLDataSources(_EnvHandle, direction, dsn, sizeof(dsn), &dsn_ret, desc, sizeof(desc), &desc_ret)))
  {
    direction = SQL_FETCH_NEXT ;

    std::string sMsg = string(dsn) ;
    if ('\0' != desc[0])
      sMsg += std::string(" (") + string(desc) + std::string(")") ;
    _pSuper->trace(&sMsg, 1, NSSuper::trWarning) ;

    // if (ret == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
  }
}

void
NSOdbcConnector::freeStmtHandle()
{
  if (NULL == _StmtHandle)
    return ;

  SQLFreeHandle(SQL_HANDLE_STMT, _StmtHandle) ;
  _StmtHandle = 0 ;
}

std::string
NSOdbcConnector::ToNarrow(const wchar_t *s)
{
/*
  std::ostringstream stm ;
  while (L'\0' != *s) {
    stm << std::use_facet<std::ctype<wchar_t>>(loc).narrow(*s++, dfault) ;
  }
  return stm.str() ;
*/

  std::wstring ws(s) ;
  std::string str(ws.begin(), ws.end()) ;
  return str ;
}

std::string
NSOdbcConnector::GetDataTypeAsString(const SQLSMALLINT iDataType)
{
  //
  // This is the value returned in the NAME and CREATE PARAMS column by a call
  // to SQLGetTypeInfo. The NAME column returns the designation —
  // for example, CHAR — whereas the CREATE PARAMS column returns a
  // comma-separated list of creation parameters such as precision, scale,
  // and length.
  //
  std::string sName = std::string("") ;
  std::string sDesc = std::string("") ;

  switch(iDataType)
  {
    case SQL_CHAR :
      sName = std::string("CHAR(n)") ;
      sDesc = std::string("Character string of fixed string length n.") ;
      return std::string("SQL_CHAR") ;

    case SQL_VARCHAR :
      sName = std::string("VARCHAR(n)") ;
      sDesc = std::string("Variable-length character string with a maximum string length n.") ;
      return std::string("SQL_VARCHAR") ;

    case SQL_LONGVARCHAR :
      sName = std::string("LONG VARCHAR") ;
      sDesc = std::string("Variable length character data. Maximum length is data source–dependent.") ;
      return std::string("SQL_LONGVARCHAR") ;

    case SQL_WCHAR :
      sName = std::string("WCHAR(n)") ;
      sDesc = std::string("Unicode character string of fixed string length n") ;
      return std::string("SQL_WCHAR") ;

    case SQL_WVARCHAR :
      sName = std::string("VARWCHAR(n)") ;
      sDesc = std::string("Unicode variable-length character string with a maximum string length n") ;
      return std::string("SQL_WVARCHAR") ;

    case SQL_WLONGVARCHAR :
      sName = std::string("LONGWVARCHAR") ;
      sDesc = std::string("Unicode variable-length character data. Maximum length is data source–dependent") ;
      return std::string("SQL_WLONGVARCHAR") ;

    case SQL_DECIMAL :
      sName = std::string("DECIMAL(p,s)") ;
      sDesc = std::string("Signed, exact, numeric value with a precision of at least p and scale s. (The maximum precision is driver-defined.) (1 <= p <= 15; s <= p).") ;
      return std::string("SQL_DECIMAL") ;

    case SQL_NUMERIC :
      sName = std::string("NUMERIC(p,s)") ;
      sDesc = std::string("Signed, exact, numeric value with a precision p and scale s  (1 <= p <= 15; s <= p).") ;
      return std::string("SQL_NUMERIC") ;

    case SQL_SMALLINT :
      sName = std::string("SMALLINT") ;
      sDesc = std::string("Exact numeric value with precision 5 and scale 0 (signed: –32,768 <= n <= 32,767, unsigned: 0 <= n <= 65,535).") ;
      return std::string("SQL_SMALLINT") ;

    case SQL_INTEGER :
      sName = std::string("INTEGER") ;
      sDesc = std::string("Exact numeric value with precision 10 and scale 0 (signed: –2[31] <= n <= 2[31] – 1, unsigned: 0 <= n <= 2[32] – 1).") ;
      return std::string("SQL_INTEGER") ;

    case SQL_REAL :
      sName = std::string("REAL") ;
      sDesc = std::string("Signed, approximate, numeric value with a binary precision 24 (zero or absolute value 10[–38] to 10[38]).") ;
      return std::string("SQL_REAL") ;

    case SQL_FLOAT :
      sName = std::string("FLOAT(p)") ;
      sDesc = std::string("Signed, approximate, numeric value with a binary precision of at least p. (The maximum precision is driver-defined.).") ;
      return std::string("SQL_FLOAT") ;

    case SQL_DOUBLE :
      sName = std::string("DOUBLE PRECISION") ;
      sDesc = std::string("Signed, approximate, numeric value with a binary precision 53 (zero or absolute value 10[–308] to 10[308]).") ;
      return std::string("SQL_DOUBLE") ;

    case SQL_BIT :
      sName = std::string("BIT") ;
      sDesc = std::string("Single bit binary data (The SQL_BIT data type has different characteristics than the BIT type in SQL-92).") ;
      return std::string("SQL_BIT") ;


    case SQL_TINYINT :
      sName = std::string("TINYINT") ;
      sDesc = std::string("Exact numeric value with precision 3 and scale 0 (signed: –128 <= n <= 127, unsigned: 0 <= n <= 255).") ;
      return std::string("SQL_TINYINT") ;

    case SQL_BIGINT :
      sName = std::string("BIGINT") ;
      sDesc = std::string("Exact numeric value with precision 19 (if signed) or 20 (if unsigned) and scale 0 (signed: –2[63] <= n <= 2[63] – 1, unsigned: 0 <= n <= 2[64] – 1).") ;
      return std::string("SQL_BIGINT") ;

    case SQL_BINARY :
      sName = std::string("BINARY(n)") ;
      sDesc = std::string("Binary data of fixed length n.") ;
      return std::string("SQL_BINARY") ;

    case SQL_VARBINARY :
      sName = std::string("VARBINARY(n)") ;
      sDesc = std::string("Variable length binary data of maximum length n. The maximum is set by the user.") ;
      return std::string("SQL_VARBINARY") ;

    case SQL_LONGVARBINARY :
      sName = std::string("LONG VARBINARY") ;
      sDesc = std::string("Variable length binary data. Maximum length is data source–dependent.") ;
      return std::string("SQL_LONGVARBINARY") ;

    case SQL_TYPE_DATE :
      sName = std::string("DATE") ;
      sDesc = std::string("Year, month, and day fields, conforming to the rules of the Gregorian calendar. (See Constraints of the Gregorian Calendar, later in this appendix.).") ;
      return std::string("SQL_TYPE_DATE") ;

    case SQL_TYPE_TIME :
      sName = std::string("TIME(p)") ;
      sDesc = std::string("Hour, minute, and second fields, with valid values for hours of 00 to 23, valid values for minutes of 00 to 59, and valid values for seconds of 00 to 61. Precision p indicates the seconds precision.") ;
      return std::string("SQL_TYPE_TIME") ;

    case SQL_TYPE_TIMESTAMP :
      sName = std::string("TIMESTAMP(p)") ;
      sDesc = std::string("Year, month, day, hour, minute, and second fields, with valid values as defined for the DATE and TIME data types.") ;
      return std::string("SQL_TYPE_TIMESTAMP") ;

/*
    case SQL_TYPE_UTCDATETIME :
      sName = std::string("UTCDATETIME") ;
      sDesc = std::string("Year, month, day, hour, minute, second, utchour, and utcminute fields. The utchour and utcminute fields have 1/10 microsecond precision.") ;
      return std::string("SQL_TYPE_UTCDATETIME") ;

    case SQL_TYPE_UTCTIME :
      sName = std::string("UTCTIME") ;
      sDesc = std::string("Hour, minute, second, utchour, and utcminute fields. The utchour and utcminute fields have 1/10 microsecond precision.") ;
      return std::string("SQL_TYPE_UTCTIME") ;
*/

    case SQL_INTERVAL_MONTH :
      sName = std::string("INTERVAL MONTH(p)") ;
      sDesc = std::string("Number of months between two dates; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_MONTH") ;

    case SQL_INTERVAL_YEAR :
      sName = std::string("INTERVAL YEAR(p)") ;
      sDesc = std::string("Number of years between two dates; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_YEAR") ;

    case SQL_INTERVAL_YEAR_TO_MONTH :
      sName = std::string("INTERVAL YEAR(p) TO MONTH") ;
      sDesc = std::string("Number of years and months between two dates; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_YEAR_TO_MONTH") ;

    case SQL_INTERVAL_DAY :
      sName = std::string("INTERVAL DAY(p)") ;
      sDesc = std::string("Number of days between two dates; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_DAY") ;

    case SQL_INTERVAL_HOUR :
      sName = std::string("INTERVAL HOUR(p)") ;
      sDesc = std::string("Number of hours between two date/times; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_HOUR") ;

    case SQL_INTERVAL_MINUTE :
      sName = std::string("INTERVAL MINUTE(p)") ;
      sDesc = std::string("Number of minutes between two date/times; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_MINUTE") ;

    case SQL_INTERVAL_SECOND :
      sName = std::string("INTERVAL SECOND(p,q)") ;
      sDesc = std::string("Number of seconds between two date/times; p is the interval leading precision and q is the interval seconds precision.") ;
      return std::string("SQL_INTERVAL_SECOND") ;

    case SQL_INTERVAL_DAY_TO_HOUR :
      sName = std::string("INTERVAL DAY(p) TO HOUR") ;
      sDesc = std::string("Number of days/hours between two date/times; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_DAY_TO_HOUR") ;

    case SQL_INTERVAL_DAY_TO_MINUTE :
      sName = std::string("INTERVAL DAY(p) TO MINUTE") ;
      sDesc = std::string("Number of days/hours/minutes between two date/times; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_DAY_TO_MINUTE") ;

    case SQL_INTERVAL_DAY_TO_SECOND :
      sName = std::string("INTERVAL DAY(p) TO SECOND(q)") ;
      sDesc = std::string("Number of days/hours/minutes/seconds between two date/times; p is the interval leading precision and q is the interval seconds precision.") ;
      return std::string("SQL_INTERVAL_DAY_TO_SECOND") ;

    case SQL_INTERVAL_HOUR_TO_MINUTE :
      sName = std::string("INTERVAL HOUR(p) TO MINUTE") ;
      sDesc = std::string("Number of hours/minutes between two date/times; p is the interval leading precision.") ;
      return std::string("SQL_INTERVAL_HOUR_TO_MINUTE") ;

    case SQL_INTERVAL_HOUR_TO_SECOND :
      sName = std::string("INTERVAL HOUR(p) TO SECOND(q)") ;
      sDesc = std::string("Number of hours/minutes/seconds between two date/times; p is the interval leading precision and q is the interval seconds precision.") ;
      return std::string("SQL_INTERVAL_HOUR_TO_SECOND") ;

    case SQL_INTERVAL_MINUTE_TO_SECOND :
      sName = std::string("INTERVAL MINUTE(p) TO SECOND(q)") ;
      sDesc = std::string("Number of minutes/seconds between two date/times; p is the interval leading precision and q is the interval seconds precision.") ;
      return std::string("SQL_INTERVAL_MINUTE_TO_SECOND") ;

/*
    case SQL_GUID :
      sName = std::string("GUID") ;
      sDesc = std::string("Fixed length GUID.") ;
      return std::string("SQL_GUID") ;
*/

    default :
      return std::string("Unknown type") ;
  }
}

ResultSetColBind::ResultSetColBind()
{
  iColumnNumber   = 0 ;
  iTargetType     = 0 ;
  iBufferLength   = 0 ;
  iReturnedStrLen = 0 ;

  pCharTargetValuePtr  = (CHAR*) 0 ;
  pWCharTargetValuePtr = (WCHAR*) 0 ;
}

ResultSetColBind::~ResultSetColBind()
{
  if (pCharTargetValuePtr)
    free(pCharTargetValuePtr) ;
  if (pWCharTargetValuePtr)
    free(pWCharTargetValuePtr) ;
}

