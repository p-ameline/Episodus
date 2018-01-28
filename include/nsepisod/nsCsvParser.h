// NSCSVPARSER.H ://///////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// Copyright Philippe Ameline, 2014
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#if !defined(__NSCSVPARSER_H)#define __NSCSVPARSER_H
#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif

#include "nautilus\nssuper.h"
#include "partage\ns_vector.h"
#include "partage\NTArray.h"

// A value located at a given column
//
class _EPISODUS NSCsvField{
  private:

    static long lObjectCount ;

  public:

    NSCsvField(string sValue = string(""), int iCol = -1) ;
    ~NSCsvField() ;

    NSCsvField(const NSCsvField& rv) ;

    NSCsvField& operator=(const NSCsvField& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    string getValue() { return _sValue ;  }
    int    getCol()   { return _iColumn ; }

  protected:

    string _sValue ;
    int    _iColumn ;
} ;

typedef vector<NSCsvField*>        NSCsvFieldVector ;
typedef NSCsvFieldVector::iterator NSCsvFieldIter ;
typedef NTArray<NSCsvField>        NSCsvFieldArray ;

// An array of values located at a given column
//
class _EPISODUS NSCsvRecord
{
  private:

    static long lObjectCount ;

  public:

    NSCsvRecord() ;
    ~NSCsvRecord() ;

    NSCsvRecord(const NSCsvRecord& rv) ;

    NSCsvRecord& operator=(const NSCsvRecord& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    void AddField(string sValue, int iCol) ;

    NSCsvFieldArray* getFields() { return &_aFields ; }
    bool             isEmpty()   { return _aFields.empty() ; }

  protected:

    NSCsvFieldArray _aFields ;
} ;

typedef vector<NSCsvRecord*>        NSCsvRecordVector ;
typedef NSCsvRecordVector::iterator NSCsvRecordIter ;
typedef NTArray<NSCsvRecord>        NSCsvRecordArray ;

class NSCsvParserManager
{
  public:

    NSCsvParserManager() ;
    ~NSCsvParserManager() ;

    string getField()              { return _sField ; }
    void   emptyField()            { _sField = string("") ; }
    void   addToField(char cEntry) { _sField += string(1, cEntry) ; }

    bool   isInsideDoubleQuote()   { return _bInsideDoubleQuote ; }
    bool   wasDoubleQuote()        { return _bWasDoubleQuote ; }
    bool   wasCrOrLf()             { return _bWasCrOrLf ; }
    bool   needASeparator()        { return _bNeedASeparator ; }

    void   setInsideDoubleQuote(bool bDQ) { _bInsideDoubleQuote = bDQ ; }
    void   setWasDoubleQuote(bool bWDQ)   { _bWasDoubleQuote    = bWDQ ; }
    void   setWasCrOrLf(bool bWCOL)       { _bWasCrOrLf         = bWCOL ; }
    void   setNeedASeparator(bool bNAS)   { _bNeedASeparator    = bNAS ; }

    int    getRecordNumber()       { return _iRecordNumber ; }
    void   incrementRecordNumber() { _iRecordNumber++ ; }

    int    getFieldNumber()        { return _iFieldNumber ; }
    void   resetFieldNumber()      { _iFieldNumber = 0 ; }
    void   incrementFieldNumber()  { _iFieldNumber++ ; }

    NSCsvRecord* getCurrentRecord()    { return _pCurrentRecord ; }
    void         newCurrentRecord() ;
    void         resetCurrentRecord() ;
    void         detachCurrentRecord() { _pCurrentRecord = (NSCsvRecord*) 0 ; }

    void         recordCurrentField() { _pCurrentRecord->AddField(_sField, _iFieldNumber) ; }

  protected:

    string _sField ;

    bool   _bInsideDoubleQuote ;
    bool   _bWasDoubleQuote ;
    bool   _bWasCrOrLf ;
    bool   _bNeedASeparator ;

    int    _iRecordNumber ;
    int    _iFieldNumber ;

    NSCsvRecord* _pCurrentRecord ;
} ;

class _EPISODUS NSCsvParser : public NSSuperRoot
{
  private:

    static long lObjectCount ;

  public:

    NSCsvParser(NSSuper* pSuper) ;
    ~NSCsvParser() ;

    NSCsvParser(const NSCsvParser& rv) ;

    bool parse(string sFileName, bool bStopWhenError, bool bVerbose = true) ;
    bool addChar(char cEntry, NSCsvParserManager* pParseManager, bool bStopWhenError, bool bVerbose = true) ;
    void addRecord(NSCsvParserManager* pParseManager) ;

    NSCsvRecordArray* getRecords() { return &_aRecords ; }

    NSCsvParser& operator=(const NSCsvParser& src) ;

    void setHasHeader(bool bHH) { _bHasHeader = bHH ; }
    void setSeparator(char cS)  { _cSeparator = cS ;  }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSCsvRecordArray _aRecords ;

    bool             _bHasHeader ;
    char             _cSeparator ;

    NSCsvRecord*     _pHeader ;
} ;

#endif  // __NSCSVPARSER_H

// fin de nsCsvParser.h///////////////////////////////////////////////////////////////

