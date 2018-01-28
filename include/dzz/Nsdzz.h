//----------------------------------------------------------------------------// Fichier d'entête commun à tous les décodeurs (pas uniquement dke :-)
//----------------------------------------------------------------------------
#ifndef __NSDZZ_H
#define __NSDZZ_H

class NSDocumentData ;

#include "nautilus\nssuper.h"
#include "nsbb\nspatpat.h"
#include "nautilus\nscr_ama.h"

int _export dcodeKE(char Commande, NSPatPathoArray far *pPatPathoArray,
                    char far *Fichier, NSContexte far *pCtx,
                    NSCRPhraseArray far *pPhra, NSDocumentData far *pDocument);

bool _export dcodePart(char Commande, NSPatPathoArray far *pPPtArray,
                    char far *Fichier, NSContexte far *pCtx,
                    NSCRPhraseArray far *pPhra, string far *pFonction,
                    int iDecoType);

void entete(string FAR *pCRString);
void decode(void);

#ifdef _MAIN
	#ifndef __MAIN
		extern TModule *pNSDLLModule;
	#else
		NSContexte* pContexte;		static TModule *pResMod;
		TModule *pNSDLLModule;
	#endif
#else
	extern TModule *pNSDLLModule;
#endif

#endif  // #define __NSDKE_H

