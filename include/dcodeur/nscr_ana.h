//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSCR_ANA_H#define __NSCR_ANA_H
#include <classlib\arrays.h>#include "dcodeur\nscr_anx.h"#include "partage\NTArray.h"
//// Définition de NSLigneArray (Array de NSLignes(s))//

typedef vector<NSLigne*> NSLignesArray ;
typedef NSLignesArray::iterator NSLignesArrayIter ;typedef NTArray<NSLigne>        NSLigneArray ;
//
// Définition de NSRectArray (Array de TRect(s))
//

typedef vector<NS_CLASSLIB::TRect*> NSRectsArray ;
typedef NSRectsArray::iterator      NSRectsArrayIter ;typedef NTArray<NS_CLASSLIB::TRect> NSRectArray ;
#endif // __NSCR_ANA_H
