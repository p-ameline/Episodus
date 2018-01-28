//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
//#include <windows.h>
#ifndef __NS_HKFCT_H
#define __NS_HKFCT_H

#ifdef _NSHOOKDLL

void   _export InstalleHook(char *sWindowTitle);
void   _export LibereHook();

#else

void   InstalleHook(char *sWindowTitle);
void   LibereHook();

#endif

#endif
