//----------------------------------------------------------------------------
// Style utilisés pour le traitement du texte (Cadre, Paragraphe, Bordures)
//
// PA Septembre 94
//----------------------------------------------------------------------------
#ifndef __NSTXSTAR_H
#define __NSTXSTAR_H

#include <owl\color.h>
#include <owl\gdiobjec.h>

#include "nautilus\nstxstyl.h"#include "partage\ns_vector.h"#include "partage\NTArray.h"

//
// Objets mis en place pour pouvoir travailler sur des Array de Style
//
typedef vector <NSFont*> NSFontsArray;
typedef NSFontsArray::iterator FontIter;typedef NTArray<NSFont>  NSFontArray;
 /*
class  NSFontArray : public NSFontsArray
{
	public :

	// Constructeurs
	NSFontArray() : NSFontsArray() {}
	NSFontArray( NSFontArray& rv);
   // Opérateur =
	NSFontArray& operator=( NSFontArray src);
   int 			 operator==( NSFontArray& o);
	// Destructeur
	~NSFontArray();

   void vider();
};           */

typedef vector <NSStyleCadre*> NSStylCadrArray;
typedef NSStylCadrArray::iterator StylCadrIter;typedef NTArray<NSStyleCadre> NSStyleCadreArray;
/*
class  NSStyleCadreArray : public NSStylCadrArray
{
public :
	// Constructeurs
	NSStyleCadreArray() : NSStylCadrArray() {}
	NSStyleCadreArray( NSStyleCadreArray& rv);
   // Opérateur =
	NSStyleCadreArray& operator=( NSStyleCadreArray src);
   int 			  		 operator==( NSStyleCadreArray& o);
	// Destructeur
	~NSStyleCadreArray();

   void vider();
};     */

typedef vector <NSCadreDecor*> NSCadreDecoArray;
typedef NSCadreDecoArray::iterator CadreDecoIter;
typedef NTArray<NSCadreDecor>    NSCadreDecorArray;
/*
class  NSCadreDecorArray : public NSCadreDecoArray
{
	public :

	// Constructeurs
	NSCadreDecorArray() : NSCadreDecoArray() {}
	NSCadreDecorArray( NSCadreDecorArray& rv);
	// Destructeur
	~NSCadreDecorArray();
   // Opérateur =
	NSCadreDecorArray& operator=( NSCadreDecorArray src);
   int 			  		 operator==( NSCadreDecorArray& o);

   void vider();
};       */

//typedef vector <NSStyleParagraphe*> NSStylParaArray;
typedef vector <NSStyleParagraphe*> NSStylParaArray;
typedef NSStylParaArray::iterator NSStylParaArrayIter;
typedef NTArray<NSStyleParagraphe>  NSStyleParagrapheArray;
/*
class  NSStyleParagrapheArray : public NSStylParaArray
{
	public :

	// Constructeurs
	NSStyleParagrapheArray() : NSStylParaArray() {}
	NSStyleParagrapheArray( NSStyleParagrapheArray& rv);
   // Opérateur =
	NSStyleParagrapheArray& operator=( NSStyleParagrapheArray src);
   int 			  		      operator==( NSStyleParagrapheArray& o);
	// Destructeur
	~NSStyleParagrapheArray();

   void vider();
};    */

//typedef TArrayAsVector<NSPage> NSStylPageArray;
typedef vector <NSPage*> NSStylPageArray;
typedef NSStylPageArray::iterator NSStylPageArrayIter;
typedef NTArray<NSPage > NSPageArray;
/*
class  NSPageArray : public NSStylPageArray
{
	public :

	// Constructeurs
	NSPageArray() : NSStylPageArray() {}
	NSPageArray(NSPageArray& rv);
   // Opérateur =
	NSPageArray& operator=(NSPageArray src);
   int 			 operator==(NSPageArray& o);
	// Destructeur
	~NSPageArray();

   void vider();
};   */

#endif // __NSTXSTAR_H
