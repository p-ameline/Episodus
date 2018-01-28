//---------------------------------------------------------------------------
//     nsToDo.h
//
//  Objects used to send commands to core application 
//
//
//  PA Août 1993
//---------------------------------------------------------------------------

#ifndef __NSTODO_H__
# define __NSTODO_H__

#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif

# include <cstring.h>
# include <vector.h>

class NSSuper ;
class BB1BBInterfaceForKs ;

# include "partage\NTArray.h"
# include "nautilus\nsLdv_Vars.h"

class _EPISODUS NSInstanceCounter
{
  public:

    NSInstanceCounter() { _iInstanceCounter = 1 ; } ;

    size_t getCount() { return _iInstanceCounter ; }

    void   add()    { _iInstanceCounter++ ; }
    void   remove() { _iInstanceCounter > 0 ? _iInstanceCounter-- : _iInstanceCounter = 0 ; }

  protected:

    size_t _iInstanceCounter ;
} ;

class _EPISODUS NSConcernToDoInformation
{
  public:

 	  //! constructor
    NSConcernToDoInformation() ;
    //! copy constructor
    NSConcernToDoInformation(const NSConcernToDoInformation&) ;

    //! destructor
    ~NSConcernToDoInformation() ;

    //! operator =
    NSConcernToDoInformation& operator=(const NSConcernToDoInformation& src) ;

    LDVFRAME getFrame()        { return _frame ;         }
    string   getLexique()      { return _sLexique ;      }
    string   getStartingDate() { return _sStartingDate ; }
    string   getEndingDate()   { return _sEndingDate ;   }
    int      getSeverity()     { return _iSeverity ;     }
    bool     isVerbose()       { return _bVerbose ;      }

    string   getText()         { return _sText ;         }
    string   getUrl()          { return _sUrl ;          }

    void   setFrame(LDVFRAME Frame)    { _frame         = Frame ; }
    void   setLexique(string sLex)     { _sLexique      = sLex ;  }
    void   setStartingDate(string sSD) { _sStartingDate = sSD ;   }
    void   setEndingDate(string sED)   { _sEndingDate   = sED ;   }
    void   setSeverity(int iSV)        { _iSeverity     = iSV ;   }
    void   setVerbose(bool bVB)        { _bVerbose      = bVB ;   }

    void   setText(string sTxt)        { _sText         = sTxt ;  }
    void   setUrl(string sUrl)         { _sUrl          = sUrl ;  }

  protected:

    LDVFRAME _frame ;
    string   _sLexique ;
    string   _sStartingDate ;
    string   _sEndingDate ;
    bool     _bVerbose ;
    int      _iSeverity ;

    string   _sText ;
    string   _sUrl ;
} ;

//
// Main objects
//
class _EPISODUS NSToDoTask
{
  public:

 	  //! constructor
    NSToDoTask() ;
    //! copy constructor
    NSToDoTask(const NSToDoTask&) ;

    //! destructor
    ~NSToDoTask() ;

    void sendMe(NSSuper *pSuper) ;
    void sendBB1BBInterface(BB1BBInterfaceForKs* pKsDesc, NSSuper *pSuper, bool bSend, bool bDeleteKsDesc = false) ;
    void sendBB1BBDecisionTree(BB1BBInterfaceForKs* pKsDesc, NSSuper *pSuper, bool bSend, bool bDeleteKsDesc = false) ;
    void BbkAskUser(NSSuper *pSuper, string sArchetype, int iTypeArc) ;

    //! operator =
    NSToDoTask& operator=(const NSToDoTask& src) ;

    void initFromTask(const NSToDoTask *pSrc) ;
    void reinit() ;

    string getWhatToDo()  { return _sWhatToDo ; }
    string getParam1()    { return _sParam1 ;   }
    string getParam2()    { return _sParam2 ;   }

    void*  getPointer1()  { return _pPointer1 ; }
    void*  getPointer2()  { return _pPointer2 ; }

    void   setWhatToDo(string sWTD) { _sWhatToDo = sWTD ; }
    void   setParam1(string sP1)    { _sParam1   = sP1 ; }
    void   setParam2(string sP2)    { _sParam2   = sP2 ; }

    void   setPointer1(void* pPointer, bool bMustDelete) ;
    void   setPointer2(void* pPointer, bool bMustDelete) ;

  protected:

    string  _sWhatToDo ;
    string  _sParam1 ;
    string  _sParam2 ;

    void*   _pPointer1 ;
    NSInstanceCounter* _pP1InstanceCounter ;

    void*   _pPointer2 ;
    NSInstanceCounter* _pP2InstanceCounter ;

    void reinitPointer(void* pPointer, NSInstanceCounter* pCounter) ;
} ;

typedef vector<NSToDoTask *>    NSTaskVector ;
typedef NSTaskVector::iterator       TaskIter ;
typedef NSTaskVector::const_iterator TaskConstIter ;
typedef NTArray<NSToDoTask>     NSTaskArray ;

#endif // !__NSTODO_H__

