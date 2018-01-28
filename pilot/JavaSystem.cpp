/*//////////////////////////////////////////////////////*/
/*														*/
/*	Created by:	Dominique Sauquet						*/
/*  Created on:	May 2003								*/
/*														*/
/*//////////////////////////////////////////////////////*/

#include "pilot/JavaSystem.hpp"
#include <string>
#include <fstream>
// #include <owl\module.h>

#ifdef _ENTERPRISE_DLL
  #include "pilot/NautilusPilot.hpp"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "nspr.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  JavaSystem
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

JNIEnv*  JavaSystem::jenv     = (JNIEnv*)  0 ;
JavaVM*  JavaSystem::_jvm     = (JavaVM*)  0 ;
TModule* JavaSystem::_pJvmDll = (TModule*) 0 ;

static jint JNICALL vfprintfHook(FILE * /*fp*/, const char *format, va_list args)
{
	char buf[1024] ;
	_vsnprintf(buf, sizeof(buf), format, args) ;

	ofstream outFile ;
	outFile.open("traceNau.inf", ios::app) ;
	if (!outFile)
		return 1 ;

	outFile << string(buf) ;
  outFile << string("\n") ;

	outFile.close() ;

	return 1 ;
}

/*
* The path must contain the directory of jvm.dll
*#define JNI_OK           0                 // success
#define JNI_ERR          (-1)              //unknown error
#define JNI_EDETACHED    (-2)              // thread detached from the VM
#define JNI_EVERSION     (-3)              // JNI version error
#define JNI_ENOMEM       (-4)              // not enough memory
#define JNI_EEXIST       (-5)              // VM already created
#define JNI_EINVAL       (-6)              // invalid arguments

*/
int JavaSystem::Init(JniManager* pManager, const char* pSzClassPath,
					           const char* pSzLibPath, const char* pSzOptions, bool verbose)
//================================================================
{
  NSSuper* pSuper = pManager->getSuper() ;

	string ps = string("JavaSystem::Init : begin") ;
	pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

#ifndef _ENTERPRISE_DLL
  //
  // First, make certain that error/warning messages won't be hidden by the
  // splash screen
  //
  HWND hWnd = 0 ;

  TSplashWindow* pSplashWin = pSuper->getSplashWindow() ;
  if (pSplashWin)
    hWnd = pSplashWin->GetHandle() ;

	//
  // Before calling JNI_CreateJavaVM, we make sure that there is a dll and
  // that the proper method is accessible
  //
  ps = string("JavaSystem::Init : loading jvm.dll") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  if (_pJvmDll)
    delete _pJvmDll ;

  _pJvmDll = (TModule*) 0 ;

  // Try to load the dll at the specified path
  //
  if (pSzLibPath)
  {
	  string sDll = string(pSzLibPath) + string("\\") + string("jvm.dll") ;

    ps = string("JavaSystem::Init : Trying to load ") + sDll ;
    pSuper->trace(&ps, 1, NSSuper::trError) ;

	  _pJvmDll = new TModule(sDll.c_str(), true, false /*don't throw an exception if can't load*/) ;
    if (((TModule*) NULL == _pJvmDll) || (0 == _pJvmDll->GetHandle()))
    {
      if (_pJvmDll)
      {
        delete _pJvmDll ;
        _pJvmDll = (TModule*) 0 ;
      }
      ps = string("JavaSystem::Init : Failed to load ") + sDll ;
      pSuper->trace(&ps, 1, NSSuper::trError) ;
    }
  }

  // If not loaded already, try to load the dll with no specified path
  //
  if (((TModule*) NULL == _pJvmDll) || (0 == _pJvmDll->GetHandle()))
  {
    string sDll = string("jvm.dll") ;
    ps = string("JavaSystem::Init : Trying to load ") + sDll ;
    pSuper->trace(&ps, 1, NSSuper::trError) ;

    _pJvmDll = new TModule(sDll.c_str(), true, false /*don't throw an exception if can't load*/) ;
  }

  // If it failed, we have to surrender
  //
	if (((TModule*) NULL == _pJvmDll) || (0 == _pJvmDll->GetHandle()))
  {
  	ps = string("JavaSystem::Init : Problem loading VM Java library (jvm.dll)") ;
    pSuper->trace(&ps, 1, NSSuper::trError) ;
		erreur(ps.c_str(), standardError, 0, hWnd) ;
    return -1 ;
	}

  ps = string("JavaSystem::Init : jvm.dll loaded (") + string(_pJvmDll->GetName()) + string(")") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // Get JNI_CreateJavaVM function
  //
  jint (JNICALL *JNI_CreateJavaVM)(JavaVM **pvm, void **penv, void *args) ;
  JNI_CreateJavaVM = (jint (JNICALL *)(JavaVM **, void **, void *)) _pJvmDll->GetProcAddress("JNI_CreateJavaVM") ;
  if (NULL == JNI_CreateJavaVM)
  {
  	ps = string("JavaSystem::Init : Could not find the JNI_CreateJavaVM function in the specified .DLL") ;
		pSuper->trace(&ps, 1, NSSuper::trError) ;
		erreur(ps.c_str(), standardError, 0, hWnd) ;
    return -1 ;
  }

#else  // #ifndef _ENTERPRISE_DLL

	//
  // Before calling JNI_CreateJavaVM, we make sure that there is a dll and
  // that the proper method is accessible
  //
  ps = string("JavaSystem::Init : loading jvm.dll") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  PRLibrary *jvmDll = 0 ;
  PRStatus  dllStatus ;

  if (pSzLibPath)
  {
	  string sDll = string(pSzLibPath) + string("\\") + string("jvm.dll") ;

    ps = string("JavaSystem::Init : Trying to load ") + sDll ;
    pSuper->trace(&ps, 1, NSSuper::trError) ;

	  jvmDll = PR_LoadLibrary(sDll.c_str()) ;
    if ((PRLibrary *) NULL == jvmDll)
    {
      ps = string("JavaSystem::Init : Failed to load ") + sDll ;
      pSuper->trace(&ps, 1, NSSuper::trError) ;
    }
  }
  if ((PRLibrary *) NULL == jvmDll)
  {
    string sDll = string("jvm.dll") ;
    ps = string("JavaSystem::Init : Trying to load ") + sDll ;
    pSuper->trace(&ps, 1, NSSuper::trError) ;

    jvmDll = PR_LoadLibrary(sDll.c_str()) ;
  }
	if ((PRLibrary *) NULL == jvmDll)
  {
  	ps = string("JavaSystem::Init : Problem loading VM Java library (jvm.dll)") ;
    pSuper->trace(&ps, 1, NSSuper::trError) ;
		erreur(ps.c_str(), standardError) ;
    return -1 ;
	}

  // ps = string("JavaSystem::Init : jvm.dll loaded (") + string(jvmDll->GetName()) + string(")") ;
  ps = string("JavaSystem::Init : jvm.dll loaded, finding JNI_CreateJavaVM function") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  jint (JNICALL *JNI_CreateJavaVM)(JavaVM **pvm, void **penv, void *args) ;
  JNI_CreateJavaVM = (jint (JNICALL *)(JavaVM **, void **, void *)) PR_FindSymbol(jvmDll, "JNI_CreateJavaVM") ;
  if (NULL == JNI_CreateJavaVM)
  {
  	ps = string("JavaSystem::Init : Could not find the JNI_CreateJavaVM function in the specified .DLL") ;
		pSuper->trace(&ps, 1, NSSuper::trError) ;
		erreur(ps.c_str(), standardError) ;

    PR_UnloadLibrary(jvmDll) ;

    return -1 ;
  }
#endif // #ifndef _ENTERPRISE_DLL

  //
  // Options initialization
  //

  // Default values for min and max heap memory
  //
  string sXms = string("-Xms64m") ;
	string sXmx = string("-Xmx256m") ;

  VectString aOptions ;
  if (pSzOptions)
  {
    aOptions.fillFromString(string(pSzOptions), string(" ")) ;

    if (false == aOptions.empty())
    {
      // If Xms is specified, it superseedes default values
      //
      string sMin = aOptions.containsStartWith(string("-Xms")) ;
      if (string("") != sMin)
        sXms = string("") ;

      string sMax = aOptions.containsStartWith(string("-Xmx")) ;
      if (string("") != sMax)
        sXmx = string("") ;
    }
  }

	JavaVMInitArgs vm_args ;
	JavaVMOption* options = new JavaVMOption[8 + aOptions.size()] ;
	//JavaVMOption options[4];
	int optionsNb = 0;

  if (string("") != sXms)
	  options[optionsNb++].optionString = (char*) sXms.c_str() ;
  if (string("") != sXmx)
	  options[optionsNb++].optionString = (char*) sXmx.c_str() ;

  if (false == aOptions.empty())
    for (IterString it = aOptions.begin() ; aOptions.end() != it ; it++)
      options[optionsNb++].optionString = (char*) (*it)->c_str() ;

  if (pSzClassPath)
  {
  	ps = string("- classPath : ") + string(pSzClassPath) ;

    int len = strlen(pSzClassPath) ;
	  char* classP = new char[30+len] ;
	  sprintf(classP, "-Djava.class.path=%s", pSzClassPath) ;

    options[optionsNb++].optionString = classP ;
  }
  else
  	ps = string("- classPath : NULL") ;
	pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

  if (pSzLibPath)
  {
  	ps = string("- libPath : ") + string(pSzLibPath) ;

    int len = strlen(pSzLibPath) ;
	  char* libP = new char[30+len] ;
	  sprintf(libP, "-Djava.library.path=%s", pSzLibPath) ;

    options[optionsNb++].optionString = libP ;
  }
  else
  	ps = string("- libPath : NULL") ;
	pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

  options[optionsNb].optionString = "vfprintf" ;
	options[optionsNb++].extraInfo = vfprintfHook ;

	if (verbose)
		options[optionsNb++].optionString = "-verbose:jni" ;
  //options[optionsNb++].optionString = "-Djava.compiler=NONE";

	vm_args.version  = JNI_VERSION_1_4 ;
	vm_args.options  = options ;
	vm_args.nOptions = optionsNb ;
	vm_args.ignoreUnrecognized = JNI_TRUE ;

  // Trace options
  //
  ps = string("JavaSystem::Init : ready to call JNI_CreateJavaVM with ") + IntToString(optionsNb) + string(" options;") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  for (int i = 0 ; i < optionsNb ; i++)
  {
    string sOpt = string(options[i].optionString) ;
    pSuper->trace(&sOpt, 1, NSSuper::trDetails) ;
  }

  //
  // Now we can call JNI_CreateJavaVM
  //
  ps = string("JavaSystem::Init : calling JNI_CreateJavaVM") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // Doc: The JNI_CreateJavaVM() function loads and initializes a Java VM and
  //      returns a pointer to the JNI interface pointer.
  //
  jint err = JNI_CreateJavaVM(&_jvm, (void**) &jenv, &vm_args) ;
  if (err < 0)
  {
  	ps = string("JavaSystem::Init : JNI_CreateJavaVM failed") ;
    switch (err)
    {
    	case -1 : ps += string(" (unknown error)") ; break ;
      case -2 : ps += string(" (thread detached from the VM)") ; break ;
      case -3 : ps += string(" (JNI version error)") ; break ;
      case -4 : ps += string(" (not enough memory)") ; break ;
      case -5 : ps += string(" (VM already created)") ; break ;
      case -6 : ps += string(" (invalid arguments)") ; break ;
      default : ps += string(" (unknown return error)") ; break ;
    }
		pSuper->trace(&ps, 1, NSSuper::trError) ;
#ifndef _ENTERPRISE_DLL
		erreur(ps.c_str(), standardError, 0, hWnd) ;
#else
    erreur(ps.c_str(), standardError) ;
#endif
    delete[] options ;
    return err ;
  }
  else
  {
  	ps = string("JavaSystem::Init : JNI_CreateJavaVM succeeded") ;
		pSuper->trace(&ps, 1, NSSuper::trDetails) ;
  }

  delete[] options ;

  ps = string("JavaSystem::Init : Loaded JVM version is ") ;
  jint iVer = jenv->GetVersion() ;

  switch(iVer)
  {
    case JNI_VERSION_1_1 : ps += string("1.1") ; break ;
    case JNI_VERSION_1_2 : ps += string("1.2") ; break ;
    case JNI_VERSION_1_4 : ps += string("1.4") ; break ;
    case JNI_VERSION_1_6 : ps += string("1.6") ; break ;
    default              : ps += string("unknown") ;
  }
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  return err ;
}

int JavaSystem::Init(JniManager* pManager, const char* fileName, bool verbose)
//===============================================================
{
  NSSuper* pSuper = pManager->getSuper() ;

#ifndef _ENTERPRISE_DLL
	//
  // First, make certain that error/warning messages won't be hidden by the
  // splash screen
  //
  HWND hWnd = 0 ;

  if (pSuper->getApplication())
  {
  	TSplashWindow* pSplashWin = pSuper->getApplication()->getSplashWindow() ;
    if (pSplashWin)
    	hWnd = pSplashWin->GetHandle() ;
  }
#endif

	FILE *file;
  string ps ;

	if ((NULL == fileName) || ('\0' == fileName[0]))
  {
  	string sErrorText = string("JavaSystem::Init: no params file specified. Exiting...") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
#ifndef _ENTERPRISE_DLL
		erreur(sErrorText.c_str(), standardError, 0, hWnd) ;
#else
    erreur(sErrorText.c_str(), standardError) ;
#endif
    return 1 ;
  }
  if ((file = fopen(fileName, "r")) == NULL)
  {
  	string sErrorText = string("JavaSystem::Init : Impossible to open ")
    											 + string(fileName) ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
#ifndef _ENTERPRISE_DLL
		erreur(sErrorText.c_str(), standardError, 0, hWnd) ;
#else
    erreur(sErrorText.c_str(), standardError) ;
#endif
		return 1 ;
  }

	static char * delim  = "= \t\n" ;
  static char * delim2 = "\t\n" ;
  char *rc ;
	char linebuffer[2000] ;   //big enough for classPath definition
	char *linebufferp ;

	char* classPath = NULL ;
	char* libPath   = NULL ;
  char* options   = NULL ;

	while (true)
	{
		rc = fgets(linebuffer, sizeof(linebuffer), file) ;
		if ((char *)NULL == rc)
			break ;

		if ((linebufferp = (char *)strtok(&linebuffer[0], delim)) == NULL)
			continue;   //read nextline

		if (*linebufferp == '#' ||
		    *linebufferp == 0   || *linebufferp == ' ')
			continue;   //read nextline

		if (0 == strcmp(linebufferp, "libPath"))
		{
			if ((linebufferp = (char *)strtok(NULL, delim2)) == NULL)
				continue ;
      while ((' ' == *linebufferp) || ('=' == *linebufferp))
        linebufferp++ ;

			libPath = new char[strlen(linebufferp)+1] ;
			strcpy(libPath, linebufferp) ;
		}

		if (0 == strcmp(linebufferp, "classPath"))
		{
			if ((linebufferp = (char *)strtok(NULL, delim2)) == NULL)
				continue ;
      while ((' ' == *linebufferp) || ('=' == *linebufferp))
        linebufferp++ ;

			classPath = new char[strlen(linebufferp)+1] ;
			strcpy(classPath, linebufferp) ;
		}

    if (0 == strcmp(linebufferp, "options"))
		{
			if ((linebufferp = (char *)strtok(NULL, delim2)) == NULL)
				continue ;
      while ((' ' == *linebufferp) || ('=' == *linebufferp))
        linebufferp++ ;

			options = new char[strlen(linebufferp)+1] ;
			strcpy(options, linebufferp) ;
		}
	}

	fclose(file) ;

  if (libPath)
    ps = string("JavaSystem::Init : Init with libpath : ") + string(libPath) ;
  else
    ps = string("JavaSystem::Init : libPath is NULL") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  if (classPath)
    ps = string("JavaSystem::Init : Init with classPath : ") + string(classPath) ;
  else
    ps = string("JavaSystem::Init : classPath is NULL") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  if (options)
    ps = string("JavaSystem::Init : Init with jvm params : ") + string(options) ;
  else
    ps = string("JavaSystem::Init : not specific jvm params") ;
	pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	int stat = Init(pManager, classPath, libPath, options, verbose) ;

  if (classPath)
	  delete[] classPath ;
  if (libPath)
	  delete[] libPath ;
  if (options)
	  delete[] options ;

	return stat ;
}

void JavaSystem::Close(JniManager* /* pManager */)
//======================
{
	_jvm->DestroyJavaVM() ;

  if (_pJvmDll)
  {
    delete _pJvmDll ;
    _pJvmDll = (TModule*) 0 ;
  }
}

char* JavaSystem::NewStringWithSpecialCharacters(jstring myText)
//==============================================================
{
	// Using GetStringChars, ReleaseStringChars and NewString
	// GetStringChars get a UNICODE encoded string. This encoding IS supported by c++.
  // But we must do a little trick to get it right : the returned string seems to be shit when we prompt it,
  // because it's only one caracter long, but if we prompt it caracter by caracter, we see that all
  // the caracters are here, and rightly encoded. So we must make a copy of this string, by specifying
  // the fact that it is encoded in UNICODE. The copy we get is now fully usable.
  // Only bad thing : using NewString to send a string back to the java doesn't work.

  if (NULL == myText)
    return "" ;

	jboolean isCopy ;
	const    jchar *str = jenv->GetStringChars(myText, &isCopy) ;
  jsize    sz         = jenv->GetStringLength(myText) ;

  char *str2 = new char[sz+1] ;
	for(int i=0; i<sz; i++)
	  str2[i] = (char)str[i] ;
	str2[sz] = '\0' ;

  if (isCopy == JNI_TRUE)
    jenv->ReleaseStringChars(myText, str) ;
  return str2 ;
}

string JavaSystem::NewStringObjWithSpecialCharacters(jstring myText)
//==============================================================
{
  char* str = NewStringWithSpecialCharacters(myText) ;
  string sReturn = string(str) ;
  delete str ;
  return sReturn ;
}

