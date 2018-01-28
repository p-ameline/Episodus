/*++ BUILD Version: 0001    // Increment this if a change has global effects

Copyright (c) 1994-1999  Microsoft Corporation

Module Name:

    psapi.h

Abstract:

    Include file for APIs provided by PSAPI.DLL

Author:

    Richard Shupak   [richards]  06-Jan-1994

Revision History:

--*/

#ifndef _PSAPI_H_
#define _PSAPI_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LIST_MODULES_DEFAULT 0x0  // This is the default one app would get without any flag.
#define LIST_MODULES_32BIT   0x01  // list 32bit modules in the target process.
#define LIST_MODULES_64BIT   0x02  // list all 64bit modules. 32bit exe will be stripped off.

// list all the modules
#define LIST_MODULES_ALL   (LIST_MODULES_32BIT | LIST_MODULES_64BIT)


// --- Should be in WinBase.h

typedef struct _MEMORYSTATUSEX {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORDLONG ullTotalPhys;
    DWORDLONG ullAvailPhys;
    DWORDLONG ullTotalPageFile;
    DWORDLONG ullAvailPageFile;
    DWORDLONG ullTotalVirtual;
    DWORDLONG ullAvailVirtual;
    DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

WINBASEAPI
BOOL
WINAPI
GlobalMemoryStatusEx(
    LPMEMORYSTATUSEX lpBuffer
    );

// ---end Should be in WinBase.h

BOOL
WINAPI
EnumProcesses (
    DWORD * lpidProcess,
    DWORD cb,
    LPDWORD lpcbNeeded
    );

BOOL
WINAPI
EnumProcessModules(
    HANDLE hProcess,
    HMODULE *lphModule,
    DWORD cb,
    LPDWORD lpcbNeeded
    );

BOOL
WINAPI
EnumProcessModulesEx(
    HANDLE hProcess,
    HMODULE *lphModule,
    DWORD cb,
    LPDWORD lpcbNeeded,
    DWORD dwFilterFlag
    );

DWORD
WINAPI
GetModuleBaseNameA(
    HANDLE hProcess,
    HMODULE hModule,
    LPSTR lpBaseName,
    DWORD nSize
    );

DWORD
WINAPI
GetModuleBaseNameW(
    HANDLE hProcess,
    HMODULE hModule,
    LPWSTR lpBaseName,
    DWORD nSize
    );

#ifdef UNICODE
#define GetModuleBaseName  GetModuleBaseNameW
#else
#define GetModuleBaseName  GetModuleBaseNameA
#endif // !UNICODE


DWORD
WINAPI
GetModuleFileNameExA(
    HANDLE hProcess,
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize
    );

DWORD
WINAPI
GetModuleFileNameExW(
    HANDLE hProcess,
    HMODULE hModule,
    LPWSTR lpFilename,
    DWORD nSize
    );

#ifdef UNICODE
#define GetModuleFileNameEx  GetModuleFileNameExW
#else
#define GetModuleFileNameEx  GetModuleFileNameExA
#endif // !UNICODE


typedef struct _MODULEINFO {
    LPVOID lpBaseOfDll;
    DWORD SizeOfImage;
    LPVOID EntryPoint;
} MODULEINFO, *LPMODULEINFO;


BOOL
WINAPI
GetModuleInformation(
    HANDLE hProcess,
    HMODULE hModule,
    LPMODULEINFO lpmodinfo,
    DWORD cb
    );


BOOL
WINAPI
EmptyWorkingSet(
    HANDLE hProcess
    );

//
// Working set information structures. All non-specified bits are reserved.
//

typedef ULONG ULONG_PTR ; // Add PA 

#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4201)   // unnamed struct
#pragma warning(disable:4214)   // bit fields other than int

typedef union _PSAPI_WORKING_SET_BLOCK {
    ULONG_PTR Flags;
    struct {
        ULONG_PTR Protection : 5;
        ULONG_PTR ShareCount : 3;
        ULONG_PTR Shared : 1;
        ULONG_PTR Reserved : 3;
#if defined(_WIN64)
        ULONG_PTR VirtualPage : 52;
#else
        ULONG_PTR VirtualPage : 20;
#endif
    };
} PSAPI_WORKING_SET_BLOCK, *PPSAPI_WORKING_SET_BLOCK;

typedef struct _PSAPI_WORKING_SET_INFORMATION {
    ULONG_PTR NumberOfEntries;
    PSAPI_WORKING_SET_BLOCK WorkingSetInfo[1];
} PSAPI_WORKING_SET_INFORMATION, *PPSAPI_WORKING_SET_INFORMATION;

typedef union _PSAPI_WORKING_SET_EX_BLOCK {
    ULONG_PTR Flags;
    struct {
        ULONG_PTR Valid : 1;        // The following fields are valid only if this bit is set
        ULONG_PTR ShareCount : 3;
        ULONG_PTR Win32Protection : 11;
        ULONG_PTR Shared : 1;
        ULONG_PTR Node : 6;
        ULONG_PTR Locked : 1;
        ULONG_PTR LargePage : 1;
    };
} PSAPI_WORKING_SET_EX_BLOCK, *PPSAPI_WORKING_SET_EX_BLOCK;

typedef struct _PSAPI_WORKING_SET_EX_INFORMATION {
    PVOID VirtualAddress;
    PSAPI_WORKING_SET_EX_BLOCK VirtualAttributes;
} PSAPI_WORKING_SET_EX_INFORMATION, *PPSAPI_WORKING_SET_EX_INFORMATION;

#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4214)
#pragma warning(default:4201)
#endif

BOOL
WINAPI
QueryWorkingSet(
    HANDLE hProcess,
    PVOID pv,
    DWORD cb
    );

BOOL
WINAPI
QueryWorkingSetEx(
    HANDLE hProcess,
    PVOID pv,
    DWORD cb
    );

BOOL
WINAPI
InitializeProcessForWsWatch(
    HANDLE hProcess
    );

typedef struct _PSAPI_WS_WATCH_INFORMATION {
    LPVOID FaultingPc;
    LPVOID FaultingVa;
} PSAPI_WS_WATCH_INFORMATION, *PPSAPI_WS_WATCH_INFORMATION;

typedef struct _PSAPI_WS_WATCH_INFORMATION_EX {
    PSAPI_WS_WATCH_INFORMATION BasicInfo;
    ULONG_PTR FaultingThreadId;
    ULONG_PTR Flags;    // Reserved
} PSAPI_WS_WATCH_INFORMATION_EX, *PPSAPI_WS_WATCH_INFORMATION_EX;

BOOL
WINAPI
GetWsChanges(
    HANDLE hProcess,
    PPSAPI_WS_WATCH_INFORMATION lpWatchInfo,
    DWORD cb
    );

BOOL
WINAPI
GetWsChangesEx(
    HANDLE hProcess,
    PPSAPI_WS_WATCH_INFORMATION_EX lpWatchInfoEx,
    PDWORD cb
    );

DWORD
WINAPI
GetMappedFileNameW (
    HANDLE hProcess,
    LPVOID lpv,
    LPWSTR lpFilename,
    DWORD nSize
    );

DWORD
WINAPI
GetMappedFileNameA (
    HANDLE hProcess,
    LPVOID lpv,
    LPSTR lpFilename,
    DWORD nSize
    );

#ifdef UNICODE
#define GetMappedFileName  GetMappedFileNameW
#else
#define GetMappedFileName  GetMappedFileNameA
#endif // !UNICODE

BOOL
WINAPI
EnumDeviceDrivers (
    LPVOID *lpImageBase,
    DWORD cb,
    LPDWORD lpcbNeeded
    );


DWORD
WINAPI
GetDeviceDriverBaseNameA (
    LPVOID ImageBase,
    LPSTR lpFilename,
    DWORD nSize
    );

DWORD
WINAPI
GetDeviceDriverBaseNameW (
    LPVOID ImageBase,
    LPWSTR lpBaseName,
    DWORD nSize
    );

#ifdef UNICODE
#define GetDeviceDriverBaseName  GetDeviceDriverBaseNameW
#else
#define GetDeviceDriverBaseName  GetDeviceDriverBaseNameA
#endif // !UNICODE


DWORD
WINAPI
GetDeviceDriverFileNameA (
    LPVOID ImageBase,
    LPSTR lpFilename,
    DWORD nSize
    );

DWORD
WINAPI
GetDeviceDriverFileNameW (
    LPVOID ImageBase,
    LPWSTR lpFilename,
    DWORD nSize
    );

#ifdef UNICODE
#define GetDeviceDriverFileName  GetDeviceDriverFileNameW
#else
#define GetDeviceDriverFileName  GetDeviceDriverFileNameA
#endif // !UNICODE

// Structure for GetProcessMemoryInfo()

typedef struct _PROCESS_MEMORY_COUNTERS {
    DWORD cb;
    DWORD PageFaultCount;
    size_t PeakWorkingSetSize;
    size_t WorkingSetSize;
    size_t QuotaPeakPagedPoolUsage;
    size_t QuotaPagedPoolUsage;
    size_t QuotaPeakNonPagedPoolUsage;
    size_t QuotaNonPagedPoolUsage;
    size_t PagefileUsage;
    size_t PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS;
typedef PROCESS_MEMORY_COUNTERS *PPROCESS_MEMORY_COUNTERS;

#if (_WIN32_WINNT >= 0x0501)

typedef struct _PROCESS_MEMORY_COUNTERS_EX {
    DWORD cb;
    DWORD PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivateUsage;
} PROCESS_MEMORY_COUNTERS_EX;
typedef PROCESS_MEMORY_COUNTERS_EX *PPROCESS_MEMORY_COUNTERS_EX;

#endif

BOOL
WINAPI
GetProcessMemoryInfo(
    HANDLE Process,
    PPROCESS_MEMORY_COUNTERS ppsmemCounters,
    DWORD cb
    );

typedef struct _PERFORMANCE_INFORMATION {
    DWORD cb;
    size_t CommitTotal;
    size_t CommitLimit;
    size_t CommitPeak;
    size_t PhysicalTotal;
    size_t PhysicalAvailable;
    size_t SystemCache;
    size_t KernelTotal;
    size_t KernelPaged;
    size_t KernelNonpaged;
    size_t PageSize;
    DWORD HandleCount;
    DWORD ProcessCount;
    DWORD ThreadCount;
} PERFORMANCE_INFORMATION, *PPERFORMANCE_INFORMATION, PERFORMACE_INFORMATION, *PPERFORMACE_INFORMATION;

BOOL
WINAPI
GetPerformanceInfo (
    PPERFORMANCE_INFORMATION pPerformanceInformation,
    DWORD cb
    );

typedef struct _ENUM_PAGE_FILE_INFORMATION {
    DWORD cb;
    DWORD Reserved;
    size_t TotalSize;
    size_t TotalInUse;
    size_t PeakUsage;
} ENUM_PAGE_FILE_INFORMATION, *PENUM_PAGE_FILE_INFORMATION;

typedef BOOL (*PENUM_PAGE_FILE_CALLBACKW) (LPVOID pContext, PENUM_PAGE_FILE_INFORMATION pPageFileInfo, LPCWSTR lpFilename);

typedef BOOL (*PENUM_PAGE_FILE_CALLBACKA) (LPVOID pContext, PENUM_PAGE_FILE_INFORMATION pPageFileInfo, LPCSTR lpFilename);

BOOL
WINAPI
EnumPageFilesW (
    PENUM_PAGE_FILE_CALLBACKW pCallBackRoutine,
    LPVOID pContext
    );

BOOL
WINAPI
EnumPageFilesA (
    PENUM_PAGE_FILE_CALLBACKA pCallBackRoutine,
    LPVOID pContext
    );

#ifdef UNICODE
#define PENUM_PAGE_FILE_CALLBACK PENUM_PAGE_FILE_CALLBACKW
#define EnumPageFiles EnumPageFilesW
#else
#define PENUM_PAGE_FILE_CALLBACK PENUM_PAGE_FILE_CALLBACKA
#define EnumPageFiles EnumPageFilesA
#endif // !UNICODE

DWORD
WINAPI
GetProcessImageFileNameA (
    HANDLE hProcess,
    LPSTR lpImageFileName,
    DWORD nSize
    );

DWORD
WINAPI
GetProcessImageFileNameW (
    HANDLE hProcess,
    LPWSTR lpImageFileName,
    DWORD nSize
    );

#ifdef UNICODE
#define GetProcessImageFileName  GetProcessImageFileNameW
#else
#define GetProcessImageFileName  GetProcessImageFileNameA
#endif // !UNICODE

#ifdef __cplusplus
}
#endif

#endif

