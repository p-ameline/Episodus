#ifndef JAVA_SYSTEM_H
#define JAVA_SYSTEM_H

/*//////////////////////////////////////////////////////*/
/*														*/
/*	Created by:	Dominique Sauquet   					*/
/*  Created on:	May 2003   								*/
/*														*/
/*//////////////////////////////////////////////////////*/

#include <jni.h>
#include <vector>

# if defined(_DANSSGBDDLL)
#  define _CLASSESGBD __export
# else
#  define _CLASSESGBD __import
# endif

class JniManager ;

#include <owl\module.h>

#ifndef __linux__
class _CLASSESGBD JavaSystem
#else
class JavaSystem
#endif
/**************************/
{
  protected:

		static JavaVM*       _jvm ;
    static OWL::TModule* _pJvmDll ;

	public:

		static JNIEnv* jenv ;

		static int		Init(JniManager* pManager, const char* pSzClassPath,
						 						const char* pSzLibPath, const char* pSzOptions,
						 						bool verbose = false) ;
		static int		Init(JniManager* pManager, const char* fileName,
						 						bool verbose = false) ;
		static void		Close(JniManager* pManager) ;

		static char*       NewStringWithSpecialCharacters(jstring myText) ;
		static std::string NewStringObjWithSpecialCharacters(jstring myText) ;
};

#endif // JAVA_SYSTEM_H
