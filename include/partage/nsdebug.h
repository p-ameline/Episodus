//-----------------------------------------------------------------//  Macro à utiliser comme assert() mais pour WINDOWS
//  N'interrompt pas le programme
//-----------------------------------------------------------------
#ifndef NDEBUG

#define WinAssert(exp)\{\
  if(!(exp))\
    {\
     char szBuffer[40]; \
     sprintf(szBuffer, "Fichier %s, Ligne %d , Variable impliquée : %s",\
	     __FILE__, __LINE__, #exp);\
     MessageBox(NULL, szBuffer,\
		"Assertion non vérifiée",\
		MB_OK|MB_ICONHAND);\
     } }

//-----------------------------------------------------------------//  Macro à utiliser comme assert() mais pour WINDOWS
//  Interrompt le programme en cas d'erreur
//-----------------------------------------------------------------
#define WinAssertStop(exp)\
{\
  if(!(exp))\
    {\
     char szBuffer[40]; \
     sprintf(szBuffer, "Fichier : %s, Ligne : %d, Variable impliquée : %s ",\
	     __FILE__, __LINE__, #exp);\
     if(IDABORT == MessageBox(NULL, szBuffer,  \
		"Assertion non vérifiée",      \
		MB_ABORTRETRYIGNORE|MB_ICONHAND);\
       FatalExit(-1);}}

#else
#define WinAssert(exp)#define WinAssertStop(exp)

#endif