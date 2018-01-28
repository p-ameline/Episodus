//**************************************************************************
//
//  NSAPP.H
//
//  KRZISCH Ph. mai 93
//
//  Mise en place de NSApp qui doit gérer la mémoire utilisée par toute
//  application NAUTILUS.
//  Mise en place de NSLibrary
//**************************************************************************

#ifndef __NSAPP_H
#define __NSAPP_H

//#include "nswParam.h"
#include "partage\nsdebug.h"

/*#ifndef __STDTEMPL_H
#include <stdtempl.h>
#endif
*/
#include <stdio.h>
#include <owl\applicat.h>
#include <new.h>
#include <stdarg.h>

#ifndef __SYS\STAT_H
#include <sys\stat.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "nsbb\nsdlg.h"

typedef BOOL (*NOMEMHANDLER)(WORD );
const int MAXSTRLEN = 255;

//--------------------------------------------------------------------------
// Déclaration de NSAPP
// Toute application NAUTILUS doit dériver de cette classe car elle
// comporte un contrôle de la mémoire utilisée.
//--------------------------------------------------------------------------

class NSApp : public TApplication
{
  private:
  //NSDialogModeless* dialogs;    // Liste des dialogues non-modaux actifs
  BOOL useWININI;               // Indicateur d'utilisation de WINI.INI
  NOMEMHANDLER noMemHandler;    //
  int inNoMemHandler;

  public:
  char scratch[1024];           // Buffer à la disposition de tout le monde
  static long nObjBytes;        // Nombre de bytes alloués
  static int  nObj;             // Nombre d'objets alloués

 // Constructeur
  NSApp(const char far* AName): TApplication(AName) {}
  NSApp(const char far* AName, HINSTANCE AnInstance, HINSTANCE APrevInstance,
		  const char far* ACmdLine, int ACmdShow);

 // Destructeur
  ~NSApp() {}

 // Surcharge d'opérateurs
  void _FAR *operator new(size_t size);
  void operator delete(void _FAR *ptr);

 // Divers
  NOMEMHANDLER setNoMemHandler(NOMEMHANDLER fn);
  static BOOL DfltNoMemHandler(WORD size);
  int showStats(BOOL cancel = FALSE);

 //static void showAllStats();                   // Non implémentée
  //void addDialog(NSDialogModeless* dlg);
  //void removeDialog(NSDialogModeless* dlg);

 // Configuration de l'application
  void makeIniFileName(char* buf);
  int getProfile(const char* key, char* buf, int len, const char* dflt="",
					  const char* app=NULL);
  int getProfile(const char* key, int dflt=0, const char* app=NULL);
  BOOL setProfile(const char* key, const char* val, const char* app=NULL);
  BOOL setProfile(const char* key, int val, const char*app=NULL);

  private:
  BOOL callNoMemHandler(WORD size);
};
//--------------------------------------------------------------------------
// Déclaration de NSLib
// Utilisée pour une librairie (DLL)
//--------------------------------------------------------------------------
class NSLibrary : public TModule {
  public:
  NSLibrary(const char far* libname);
  ~NSLibrary() { if(HInstance) FreeLibrary(HInstance); }
};
#ifdef __DLL__
//#define App(*NSApp::GetApplication())
#else
extern NSApp App;
#endif
#endif
