//----------------------------------------------------------------------------
//
// PA Septembre 94
//----------------------------------------------------------------------------
#ifndef __NSTXSTAR_H
#define __NSTXSTAR_H

#include <owl\color.h>
#include <owl\gdiobjec.h>

#include "dcodeur\nstxstyl.h"
#if defined(_DKD_DLL)
#else
	#define _NSDKDCLASSE __import
#endif

// Objets mis en place pour pouvoir travailler sur des Array de Style
//
typedef vector <NSFont*>            NSFontsArray ;
typedef NSFontsArray::iterator      FontIter ;
typedef NTArray<NSFont>             NSFontArray ;

typedef NSCadreDecoArray::iterator  CadreDecoIter ;
typedef NTArray<NSCadreDecor>       NSCadreDecorArray ;

typedef vector <NSStyleParagraphe*> NSStylParaArray ;
typedef NSStylParaArray::iterator   NSStylParaArrayIter ;
typedef NTArray<NSStyleParagraphe>  NSStyleParagrapheArray ;

typedef vector <NSPage*>            NSStylPageArray ;
typedef NSStylPageArray::iterator   NSStylPageArrayIter ;
typedef NTArray<NSPage>             NSPageArray ;

#endif // __NSTXSTAR_H