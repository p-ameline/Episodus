//----------------------------------------------------------------------------// Style utilisés pour le traitement du texte (Cadre, Paragraphe, Bordures)
//
// PA Septembre 94
//----------------------------------------------------------------------------
#ifndef __NSTXSTAR_H
#define __NSTXSTAR_H

#include <owl\color.h>
#include <owl\gdiobjec.h>

#include "dcodeur\nstxstyl.h"#include "partage\ns_vector.h"// #include "partage\NTArray.h"
#if defined(_DKD_DLL)	#define _NSDKDCLASSE __export
#else
	#define _NSDKDCLASSE __import
#endif
//
// Objets mis en place pour pouvoir travailler sur des Array de Style
//
typedef vector <NSFont*>            NSFontsArray ;
typedef NSFontsArray::iterator      FontIter ;
typedef NTArray<NSFont>             NSFontArray ;typedef vector <NSStyleCadre*>      NSStylCadrArray ;typedef NSStylCadrArray::iterator   StylCadrIter ;typedef NTArray<NSStyleCadre>       NSStyleCadreArray ;
typedef vector <NSCadreDecor*>      NSCadreDecoArray ;
typedef NSCadreDecoArray::iterator  CadreDecoIter ;
typedef NTArray<NSCadreDecor>       NSCadreDecorArray ;

typedef vector <NSStyleParagraphe*> NSStylParaArray ;
typedef NSStylParaArray::iterator   NSStylParaArrayIter ;
typedef NTArray<NSStyleParagraphe>  NSStyleParagrapheArray ;

typedef vector <NSPage*>            NSStylPageArray ;
typedef NSStylPageArray::iterator   NSStylPageArrayIter ;
typedef NTArray<NSPage>             NSPageArray ;

#endif // __NSTXSTAR_H
