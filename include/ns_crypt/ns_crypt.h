//----------------------------------------------------------------------------// Fichier d'entête commun à tous les crypteurs
//----------------------------------------------------------------------------
#ifndef __NS_CRYPT_H
#define __NS_CRYPT_H

#include <owl\module.h>
#include "nautilus\nssuper.h"

long _export CrypteMessages(NSContexte far *pCtx, string far *pMessage,
							string far *pAdresse, string far *pClef);

long _export CrypteFichiers(NSContexte far *pCtx, string far *pNomFich,							string far *pAdresse, string far *pClef);

long _export DecrypteMessages(NSContexte far *pCtx, string far *pMessage,							  string far *pAdresse, string far *pClef);

long _export DecrypteFichiers(NSContexte far *pCtx, string far *pNomFich,							  string far *pAdresse, string far *pClef);

#ifdef _MAIN
	#ifndef __MAIN
		extern TModule *pNSDLLModule;
	#else
		NSContexte* pContexte;
		static TModule *pResMod;
		TModule *pNSDLLModule;
	#endif
#else
	extern TModule *pNSDLLModule;
#endif

#endif  // #define __NS_CRYPT_H
