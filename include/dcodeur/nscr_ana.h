//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSCR_ANA_H
#include <classlib\arrays.h>
//

typedef vector<NSLigne*> NSLignesArray ;
typedef NSLignesArray::iterator NSLignesArrayIter ;
//
// D�finition de NSRectArray (Array de TRect(s))
//

typedef vector<NS_CLASSLIB::TRect*> NSRectsArray ;
typedef NSRectsArray::iterator      NSRectsArrayIter ;
#endif // __NSCR_ANA_H