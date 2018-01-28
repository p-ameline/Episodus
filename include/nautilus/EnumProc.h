//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __ENUMPROC_H__
#define __ENUMPROC_H__

/*
********************
EnumProc.h
********************
*/

#include <windows.h>

#include <tlhelp32.h>
#include <vdmdbg.h>

typedef BOOL (CALLBACK *PROCENUMPROC)(DWORD, WORD, LPSTR, LPARAM) ;

BOOL WINAPI EnumProcs(PROCENUMPROC lpProc, LPARAM lParam) ;

// ToolHelp Function Pointers.
HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD,DWORD) ;
BOOL (WINAPI *lpfProcess32First)(HANDLE,LPPROCESSENTRY32) ;
BOOL (WINAPI *lpfProcess32Next)(HANDLE,LPPROCESSENTRY32) ;

// PSAPI Function Pointers.
BOOL (WINAPI *lpfEnumProcesses)(DWORD *, DWORD cb, DWORD *);
BOOL (WINAPI *lpfEnumProcessModules)(HANDLE, HMODULE *, DWORD, LPDWORD);
DWORD (WINAPI *lpfGetModuleFileNameEx)(HANDLE, HMODULE, LPTSTR, DWORD);

// VDMDBG Function Pointers.
INT (WINAPI *lpfVDMEnumTaskWOWEx)(DWORD, TASKENUMPROCEX  fp, LPARAM);


// ToolHelp Function.
//HANDLE WINAPI  CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
//BOOL WINAPI    Process32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
//BOOL WINAPI    Process32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);

// PSAPI Function.
/*
typedef BOOL (WINAPI * PFNENUMPROCESSES)(
    DWORD * lpidProcess,
    DWORD   cb,
    DWORD * cbNeeded
    );

typedef BOOL (WINAPI * PFNENUMPROCESSMODULES)(
    HANDLE hProcess,
    HMODULE *lphModule,
    DWORD cb,
    LPDWORD lpcbNeeded
    );

typedef DWORD (WINAPI * PFNGETMODULEFILENAMEEXA)(
    HANDLE hProcess,
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize
    );

    HMODULE hModPSAPI = 0;
    PFNENUMPROCESSES EnumProcesses = 0;
    PFNENUMPROCESSMODULES EnumProcessModules = 0;
    PFNGETMODULEFILENAMEEXA GetModuleFileNameExA = 0;
    */
/*
BOOL WINAPI    EnumProcesses(DWORD *, DWORD cb, DWORD *);
BOOL WINAPI    EnumProcessModules(HANDLE, HMODULE *, DWORD, LPDWORD);
DWORD WINAPI   GetModuleFileNameExA(HANDLE, HMODULE, LPTSTR, DWORD);
*/
// VDMDBG Function.
//INT WINAPI     VDMEnumTaskWOWEx(DWORD, TASKENUMPROCEX  fp, LPARAM);


#endif // __ENUMPROC_H__
