// NSCsvParser.CPP : Csv file processing
////////////////////////////////////////////////////////////
#include <cstring.h>
#include <stdio.h>#include <assert.h>#include "nsepisod\nsCsvParser.h"

long NSCsvField::lObjectCount = 0 ;
long NSCsvRecord::lObjectCount = 0 ;
long NSCsvParser::lObjectCount = 0 ;
/****************** classe NSCsvInformation **************************/
NSCsvField::NSCsvField(string sValue, int iCol)
{
  lObjectCount++ ;

  _sValue  = sValue ;
  _iColumn = iCol ;
}

NSCsvField::~NSCsvField()
{
  lObjectCount-- ;
}

NSCsvField::NSCsvField(const NSCsvField& rv)
{
  lObjectCount++ ;

  _sValue  = rv._sValue ;
  _iColumn = rv._iColumn ;
}

NSCsvField&
NSCsvField::operator=(const NSCsvField& src)
{
  if (&src == this)
		return *this ;

  _sValue  = src._sValue ;
  _iColumn = src._iColumn ;

  return *this ;
}

/****************** classe NSCsvLine ******************/

NSCsvRecord::NSCsvRecord()
{
  lObjectCount++ ;
}

NSCsvRecord::~NSCsvRecord()
{
  lObjectCount-- ;
}

NSCsvRecord::NSCsvRecord(const NSCsvRecord& rv)
{
  lObjectCount++ ;

  _aFields = rv._aFields ;
}

void
NSCsvRecord::AddField(string sValue, int iCol)
{
  _aFields.push_back(new NSCsvField(sValue, iCol)) ;
}

NSCsvRecord&
NSCsvRecord::operator=(const NSCsvRecord& src)
{
  if (&src == this)
		return *this ;

  _aFields = src._aFields ;

  return *this ;
}

/****************** classe NSCsvParserManager ******************/

NSCsvParserManager::NSCsvParserManager()
{
  _sField             = string("") ;

  _bInsideDoubleQuote = false ;
  _bWasDoubleQuote    = false ;
  _bWasCrOrLf         = false ;
  _bNeedASeparator    = false ;

  _iRecordNumber      = 0 ;
  _iFieldNumber       = 0 ;

  _pCurrentRecord    = (NSCsvRecord*) 0 ;
}

NSCsvParserManager::~NSCsvParserManager()
{
  if (_pCurrentRecord)
    delete _pCurrentRecord ;
}

void
NSCsvParserManager::newCurrentRecord()
{
  if (_pCurrentRecord)
    delete _pCurrentRecord ;

  _pCurrentRecord = new NSCsvRecord() ;
}

void
NSCsvParserManager::resetCurrentRecord()
{
  if (_pCurrentRecord)
    delete _pCurrentRecord ;
    
  _pCurrentRecord = (NSCsvRecord*) 0 ;
}

/****************** classe NSCsvParser ******************/

NSCsvParser::NSCsvParser(NSSuper* pSuper)
            :NSSuperRoot(pSuper)
{
  _pHeader = (NSCsvRecord*) 0 ;

  lObjectCount++ ;
}

NSCsvParser::~NSCsvParser()
{
  if (_pHeader)
    delete _pHeader ;

  lObjectCount-- ;
}

NSCsvParser::NSCsvParser(const NSCsvParser& rv)
            :NSSuperRoot(rv._pSuper)
{
  lObjectCount++ ;

  _aRecords   = rv._aRecords ;

  _bHasHeader = rv._bHasHeader ;
  _cSeparator = rv._cSeparator ;

  _pHeader = (NSCsvRecord*) 0 ;
  if (rv._pHeader)
    _pHeader = new NSCsvRecord(*(rv._pHeader)) ;
}

// According to http://en.wikipedia.org/wiki/Comma-separated_values a CSV file
// should have:
//
// - DOS-style lines that end with (CR/LF) characters (optional for the last line)
// - An optional header record (there is no sure way to detect whether it is present, so care is required when importing).
// - Each record "should" contain the same number of comma-separated fields.
// - Any field may be quoted (with double quotes).
// - Fields containing a line-break, double-quote, and/or commas should be quoted. (If they are not, the file will likely be impossible to process correctly).
// - A (double) quote character in a field must be represented by two (double) quote characters.
//
// Take special care that a line-break can existe inside a record if this
// record is quoted
//
bool
NSCsvParser::parse(string sFileName, bool bStopWhenError, bool bVerbose)
{
  if (string("") == sFileName)
    return false ;

  ifstream inFile ;
  inFile.open(sFileName.c_str()) ;
	if (!inFile)
	{
    string sErrorText = _pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sFileName ;

    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    if (bVerbose)
      erreur(sErrorText.c_str(), standardError, 0) ;

		return false ;
	}

  NSCsvParserManager parseManager ;
  parseManager.newCurrentRecord() ;

  // while (!inFile.eof()) Don't do this because iostream doesn't know it's at
  //                       the end of the file until it tries to read that first
  //                       character past the end of the file.

  int cEntry ;

  while ((cEntry = inFile.get()) != EOF)
	{
    bool bIsOk = addChar(cEntry, &parseManager, bStopWhenError, bVerbose) ;
    if ((false == bIsOk) && bStopWhenError)
      return false ;
	}

  if (false == parseManager.wasCrOrLf())
  {
    if ((parseManager.getRecordNumber() > 0) || (string("") != parseManager.getField()))
    {
      parseManager.recordCurrentField() ;

      addRecord(&parseManager) ;
    }
  }

	inFile.close() ;

  return true ;
}

bool
NSCsvParser::addChar(char cEntry, NSCsvParserManager* pParseManager, bool bStopWhenError, bool bVerbose)
{
  if ((NSCsvParserManager*) NULL == pParseManager)
    return false ;

  // Inside a double quote, previous char was a quote, this one is not
  //                  -> We are no longer inside a quoted area
  //
  if (pParseManager->isInsideDoubleQuote() && pParseManager->wasDoubleQuote() && ('\"' != cEntry))
  {
    pParseManager->setInsideDoubleQuote(false) ;
    pParseManager->setWasDoubleQuote(false) ;
    pParseManager->setNeedASeparator(true) ;
  }

  // Inside a quoted field, all characters are added except first double
  // quote in a row of two (yes, even CR and LF are added)
  //
  if (pParseManager->isInsideDoubleQuote())
  {
    if (('\"' == cEntry) && (false == pParseManager->wasDoubleQuote()))
      pParseManager->setWasDoubleQuote(true) ;
    else
      pParseManager->addToField(cEntry) ;

    return true ;
  }

  if (pParseManager->wasCrOrLf() && ('\n' != cEntry) && ('\r' != cEntry))
    pParseManager->setWasCrOrLf(false) ;

  // Not in a quoted area and encountering a quote -> we entered a quoted area
  //
  if ('\"' == cEntry)
  {
    if (pParseManager->needASeparator())
      return false ;

    pParseManager->setInsideDoubleQuote(true) ;
    pParseManager->setWasDoubleQuote(false) ;    // This quote can't count

    return true ;
  }

  if (cEntry == _cSeparator)
  {
    pParseManager->recordCurrentField() ;
    pParseManager->incrementFieldNumber() ;
    pParseManager->emptyField() ;

    pParseManager->setNeedASeparator(false) ;

    return true ;
  }

  if (('\n' == cEntry) || ('\r' == cEntry))
  {
    pParseManager->setNeedASeparator(false) ;

    if (false == pParseManager->wasCrOrLf())
    {
      pParseManager->recordCurrentField() ;
      pParseManager->emptyField() ;

      pParseManager->setWasCrOrLf(true) ;

      addRecord(pParseManager) ;

      pParseManager->newCurrentRecord() ;
    }

    return true ;
  }

  if (pParseManager->needASeparator())
    return false ;

  pParseManager->addToField(cEntry) ;

  return true ;
}

void
NSCsvParser::addRecord(NSCsvParserManager* pParseManager)
{
  if ((NSCsvParserManager*) NULL == pParseManager)
    return ;

  _pSuper->getApplication()->PumpWaitingMessages() ;

  NSCsvRecord* pCurrentRecord = pParseManager->getCurrentRecord() ;
  if ((NSCsvRecord*) NULL == pCurrentRecord)
    return ;

  if ((0 == pParseManager->getRecordNumber()) && _bHasHeader)
    _pHeader = pCurrentRecord ;
  else
    _aRecords.push_back(pCurrentRecord) ;

  pParseManager->detachCurrentRecord() ;

  pParseManager->incrementRecordNumber() ;
  pParseManager->resetFieldNumber() ;
}

NSCsvParser&
NSCsvParser::operator=(const NSCsvParser& src)
{
  if (&src == this)
		return *this ;

  _aRecords   = src._aRecords ;

  _bHasHeader = src._bHasHeader ;
  _cSeparator = src._cSeparator ;

  if (_pHeader)
    delete _pHeader ;
  _pHeader = (NSCsvRecord*) 0 ;
  if (src._pHeader)
    _pHeader = new NSCsvRecord(*(src._pHeader)) ;

  return *this ;
}

//////////////////////// ending nsCsvParser.cpp ////////////////////////
