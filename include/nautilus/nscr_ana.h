//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSCR_ANA_H#define __NSCR_ANA_H

#include <classlib\arrays.h>#include "nautilus\nscr_anx.h"#include "partage\NTArray.h"

//
// Définition de NSLigneArray (Array de NSLignes(s))
//
//typedef TArrayAsVector<NSLigne> NSLignesArray;

typedef vector<NSLigne*> NSLignesArray;
typedef NSLignesArray::iterator NSLignesArrayIter;typedef NTArray<NSLigne>    NSLigneArray;
/*
class  NSLigneArray : public NSLignesArray
{
public :
	// Constructeurs
	NSLigneArray() : NSLignesArray() {}
	NSLigneArray(NSLigneArray& rv);
	// Destructeur
	virtual ~NSLigneArray();
	// Opérateur =
	NSLigneArray& operator=(NSLigneArray src);
   void vider();
};    */

//
// Définition de NSRectArray (Array de TRect(s))
//
//typedef TArrayAsVector<TRect> NSRectsArray;

typedef vector<NS_CLASSLIB::TRect*> NSRectsArray;
typedef NSRectsArray::iterator NSRectsArrayIter;typedef NTArray<NS_CLASSLIB::TRect>  NSRectArray;
/*
class  NSRectArray : public NSRectsArray
{
public :
	// Constructeurs
	NSRectArray() : NSRectsArray() {}
	NSRectArray(NSRectArray& rv);
	// Destructeur
	virtual ~NSRectArray();
	// Opérateur =
	NSRectArray& operator=(NSRectArray src);
   void vider();
};  */


#endif // __NSCR_ANA_H
