//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSCR_ANA_H

#include <classlib\arrays.h>

//
// D�finition de NSLigneArray (Array de NSLignes(s))
//
//typedef TArrayAsVector<NSLigne> NSLignesArray;

typedef vector<NSLigne*> NSLignesArray;
typedef NSLignesArray::iterator NSLignesArrayIter;
/*
class  NSLigneArray : public NSLignesArray
{
public :
	// Constructeurs
	NSLigneArray() : NSLignesArray() {}
	NSLigneArray(NSLigneArray& rv);
	// Destructeur
	virtual ~NSLigneArray();
	// Op�rateur =
	NSLigneArray& operator=(NSLigneArray src);
   void vider();
};    */

//
// D�finition de NSRectArray (Array de TRect(s))
//
//typedef TArrayAsVector<TRect> NSRectsArray;

typedef vector<NS_CLASSLIB::TRect*> NSRectsArray;
typedef NSRectsArray::iterator NSRectsArrayIter;
/*
class  NSRectArray : public NSRectsArray
{
public :
	// Constructeurs
	NSRectArray() : NSRectsArray() {}
	NSRectArray(NSRectArray& rv);
	// Destructeur
	virtual ~NSRectArray();
	// Op�rateur =
	NSRectArray& operator=(NSRectArray src);
   void vider();
};  */


#endif // __NSCR_ANA_H