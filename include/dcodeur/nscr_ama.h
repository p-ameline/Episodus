//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------

#ifndef __NSCR_AMA_H
#define __NSCR_AMA_H

#include <vector>
#include "dcodeur/nscr_amx.h"

#if defined(_DKD_DLL)
	#define _NSDKDCLASSE __export
#else
	#define _NSDKDCLASSE __import
#endif

//
// Définition de NSCRPhraseArray (Array de NSCRPhrase(s))
//

typedef vector<NSCRPhrase*> NSCRPhrArray ;
typedef NSCRPhrArray::iterator       NSCRPhrArrayIter ;
typedef NSCRPhrArray::const_iterator NSCRPhrArrayConstIter ;

class _NSDKDCLASSE NSCRPhraseArray : public NSCRPhrArray
{
  public :

	  // Constructeurs
    NSCRPhraseArray() : NSCRPhrArray() {}
    NSCRPhraseArray(const NSCRPhraseArray& rv) ;

	  // Destructeur
    virtual ~NSCRPhraseArray() ;

    // Opérateur =
    NSCRPhraseArray& operator=(const NSCRPhraseArray& src) ;

    void vider() ;
};

#endif // __NSCR_ANA_H

