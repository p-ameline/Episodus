//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
//#include <windows.h>
#ifndef __NS_HOOK_H
#define __NS_HOOK_H

#include <bwcc.h>

int MouseHookMessage;

void   logHookMessage(string sSpyMessage);

LRESULT WINAPI NSHookProc(int nCode, WPARAM wParam, LPARAM lParam);

/*LRESULT WINAPI HookProc(int nCode, WPARAM wParam, LPARAM lParam);
extern "C" void __stdcall _export HookMouse();
extern "C" void __stdcall _export UnhookMouse(); */

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

#endif
