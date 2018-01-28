//---------------------------------------------------------------------------
//       NSUTIL.H
//  KRZISCH Ph.   janvier 92
//  Fichier contenant les prototypes des fonctions Utiles
//
//---------------------------------------------------------------------------
#ifndef __NSUTIL_H
#define __NSUTIL_H

/*#ifndef __NSWPARAM_H
#include "..\include\nswparam.h"
#endif */

#ifndef __STDIO_H
#include <stdio.h>
#endif

#ifndef __CSTRING_H
#include <cstring.h>
#endif

#ifndef __STDARG_H
#include <stdarg.h>
#endif

#ifndef __WINDOWS_H
#include <windows.h>
#endif

#ifndef __NSCACHE_H
#include "nsutil\nscache.h"
#endif

#ifndef __NSTYPE_H
#include "partage\nstype.h"
#endif

#ifndef __NSGLOBAL_H
#include "partage\nsglobal.h"
#endif

#ifndef __NSHORLOG_H
#include "partage\nshorlog.h"
#endif

#ifdef _NSUTILDLL
//---------------------------------------------------------------------------
//  Prototypes des fonctions utilitaires
//---------------------------------------------------------------------------
int     _export wspf(char buffer[],char* fmt, ...);
//void    SetInternational(void);
int     _export Position(char* value, char* array[], int nbElts);
void _export strAnalyse(const char* input, const char* separateurs,
                CacheArray& elts, int nb=1);

int  _export   firstNoWhite(const char* s);
int  _export   lastNoWhite(const char* s);
int  _export   beginPad(const char* s, char* dest, int numBlanks);
int  _export   endPad(const char* s, char* dest, int numBlanks);
void _export   beginWhiteCut(char* s);
void _export   endWhiteCut(char* s);
int  _export   centerStr(const char* s, char* dest, int maxChar);
int  _export   leftStr(const char* s, char* dest, int maxChar);
int  _export   rightStr(const char* s, char* dest, int maxChar);

#else

//---------------------------------------------------------------------------
//  Prototypes des fonctions utilitaires
//---------------------------------------------------------------------------
int     wspf(char buffer[],char* fmt, ...);
//void    SetInternational(void);
int     Position(char* value, char* array[], int nbElts);
void strAnalyse(const char* input, const char* separateurs,
                CacheArray& elts, int nb=1);

//void    strAnalyse(Pchar input, Pchar separateurs, CacheArray& elts, int nb = 1);
//void    strAnalyse(Pchar input, Pchar separateurs, Pchar elts[], int nb = 1);
//BOOL    ptrInit(char** strPtr, int length, char car = ' ');
//BOOL    ptrInit(char** strPtr, const char* strInit);
int     firstNoWhite(const char* s);
int     lastNoWhite(const char* s);
int     beginPad(const char* s, char* dest, int numBlanks);
int     endPad(const char* s, char* dest, int numBlanks);
void    beginWhiteCut(char* s);
void    endWhiteCut(char* s);
int     centerStr(const char* s, char* dest, int maxChar);
int     leftStr(const char* s, char* dest, int maxChar);
int     rightStr(const char* s, char* dest, int maxChar);

#endif
//---------------------------------------------------------------------------
//  Prototypes des fonctions exportées
//---------------------------------------------------------------------------
LRESULT FAR PASCAL _export EditNbWndProc(HWND hWnd, WORD wMessage,
					 WORD wParam, LONG lParam);
LRESULT FAR PASCAL _export EditCharWndProc(HWND hWnd, WORD wMessage,
						WORD wParam, LONG lParam);
//---------------------------------------------------------------------------
//  Prototypes des fonctions de déclassement
//---------------------------------------------------------------------------
void InitSousClass(HWND hWnd, WORD id_Edit, FARPROC lpChangeProc);
void FiniSousClass(HWND hWnd, WORD id_Edit, FARPROC lpPrevProc);
//---------------------------------------------------------------------------
#endif   // __NSUTIL_H
