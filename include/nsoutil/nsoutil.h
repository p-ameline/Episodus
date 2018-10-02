//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
//#include <windows.h>

#ifndef __NSOUTIL_H
#define __NSOUTIL_H

#include <bwcc.h>

#include "nsoutil/nsexport.h"

// class TWebProxy ;

void _export lancerOutils(NSContexte* pCtx);

// #if defined(_EXPORTDLL)
  #ifdef _MAIN
	  #ifndef __MAIN
		  extern TModule *pNSResModule;
	  #else
		  static TModule *pResMod;
		  TModule *pNSResModule;
	  #endif
  #else
	  extern TModule *pNSResModule;
  #endif
// #endif

// TWebProxy* _NSOUTILCLASSE ShowActiveXForm(Forms::TApplication* pMainApp, HWND Parent) ;

#endif
