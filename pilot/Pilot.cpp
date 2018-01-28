/*//////////////////////////////////////////////////////*/
/*														*/
/*	Created by:	Dominique Sauquet						*/
/*  Created on:	May 2003								*/
/*														*/
/*//////////////////////////////////////////////////////*/

# include <fstream.h>

#include "pilot\Pilot.hpp"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Pilot
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

jclass Pilot::classPilot = NULL ;

// Definir des constantes de retour
// static dans Pilot.hpp
int Pilot::Init(const char* config)
//=================================
{
  Trace(string("Entering Pilot init\n")) ;

	JNIEnv* jenv = JavaSystem::jenv ;
	if ((JNIEnv*) NULL == jenv)
		return -4 ;

  Trace(string("Before jenv->FindClass\n")) ;

	classPilot = jenv->FindClass("pilot/Pilot") ;

	if (0 == classPilot)
	{
    string sMsg = string("Cannot locate the pilot/Pilot class. Exiting...\n") ;
    Trace(sMsg) ;
		fprintf(stderr, sMsg.c_str()) ;
    return -1 ;
	}

  Trace(string("Before jenv->GetStaticMethodID1\n")) ;

  //=== method Pilot.setParametersFromFile
  jmethodID mid = jenv->GetStaticMethodID(classPilot, "setParametersFromFile", "(Ljava/lang/String;)V") ;
  if (0 == mid)
  {
    string sMsg = string("Cannot locate the setParametersFromFile method. Exiting...\n") ;
    Trace(sMsg) ;
    fprintf(stderr, sMsg.c_str()) ;
    return -2 ;
  }
  jstring jdescr = jenv->NewStringUTF(config) ;
  jenv->CallStaticVoidMethod(classPilot, mid, jdescr) ;
  jenv->DeleteLocalRef(jdescr) ;

  Trace(string("Calling GetStaticMethodID for method \"init\"\n")) ;

	//=== method Pilot.init
	jmethodID midInit = jenv->GetStaticMethodID(classPilot, "init", "()I") ;
	if (0 == midInit)
	{
    string sMsg = string("Cannot locate the init method. Exiting...\n") ;
    Trace(sMsg) ;
  	fprintf(stderr, sMsg.c_str()) ;
    return -3 ;
	}

  Trace(string("Calling method \"init\"\n")) ;

	jint nbAgent = jenv->CallStaticIntMethod(classPilot, midInit) ;

  Trace(string("Leaving Pilot init\n")) ;

	return nbAgent ;
}

int Pilot::AddServices(const char* serviceFile)
//=============================================
{
	JNIEnv* jenv = JavaSystem::jenv ;

	//=== method Pilot.addServices
  jmethodID mid = jenv->GetStaticMethodID(classPilot, "addServices", "(Ljava/lang/String;)Z") ;
  if (0 == mid)
  {
    string sMsg = string("Cannot locate the addServices method. Exiting...\n") ;
    Trace(sMsg) ;
    fprintf(stderr, sMsg.c_str()) ;
    return 0 ;
  }
  jstring jdescr = jenv->NewStringUTF(serviceFile) ;
  jenv->CallStaticVoidMethod(classPilot, mid, jdescr) ;
	jenv->DeleteLocalRef(jdescr) ;

  //=== method Pilot.getNbServices
  mid = jenv->GetStaticMethodID(classPilot, "getNbServices", "()I") ;
  if (0 == mid)
  {
    fprintf(stderr, "Cannot locate the getNbServices method. Exiting...\n") ;
    return 0 ;
  }

  return jenv->CallStaticIntMethod(classPilot, mid) ;
}

int Pilot::AddServicesFromDirectory(const char* serviceDir)
//=========================================================
{
  JNIEnv* jenv = JavaSystem::jenv;

  //=== method Pilot.addServicesFromDirectory
  jmethodID mid = jenv->GetStaticMethodID(classPilot, "addServicesFromDirectory", "(Ljava/lang/String;)Z") ;
  if (0 == mid)
  {
    string sMsg = string("Cannot locate the addServicesFromDirectory method. Exiting...\n") ;
    Trace(sMsg) ;
    fprintf(stderr, sMsg.c_str()) ;
    return 0 ;
  }
  jstring jdescr = jenv->NewStringUTF(serviceDir) ;
  jenv->CallStaticVoidMethod(classPilot, mid, jdescr) ;
	jenv->DeleteLocalRef(jdescr) ;

  //=== method Pilot.getNbServices
  mid = jenv->GetStaticMethodID(classPilot, "getNbServices", "()I") ;
  if (0 == mid)
  {
    fprintf(stderr, "Cannot locate the getNbServices method. Exiting...\n") ;
    return 0 ;
  }
  return jenv->CallStaticIntMethod(classPilot, mid) ;
}

int Pilot::InitFromResource(const char* resourceName)
//===================================================
{
  JNIEnv* jenv = JavaSystem::jenv ;

  //=== method Pilot.initFromResource
  jmethodID mid = jenv->GetStaticMethodID(classPilot, "initFromResource", "(Ljava/lang/String;)Z") ;
  if (0 == mid)
  {
    string sMsg = string("Cannot locate the initFromResource method. Exiting...\n") ;
    Trace(sMsg) ;
    fprintf(stderr, sMsg.c_str()) ;
    return 0 ;
  }
  jstring jresc = jenv->NewStringUTF(resourceName) ;
  jenv->CallStaticVoidMethod(classPilot, mid, jresc) ;
	jenv->DeleteLocalRef(jresc) ;

  //=== method Pilot.getNbServices
  mid = jenv->GetStaticMethodID(classPilot, "getNbServices", "()I") ;
  if (mid == 0)
  {
    fprintf(stderr, "Cannot locate the getNbServices method. Exiting...\n") ;
    return 0  ;
  }

  return jenv->CallStaticIntMethod(classPilot, mid) ;
}

void Pilot::Trace(const string &sMsg)
{
  if (string("") == sMsg)
    return ;

  ofstream outFile ;
  outFile.open("traceNau.inf", ios::app) ;
	if (!outFile)
		return ;

  struct  date dateSys ;
	struct  time heureSys ;
	char    msg[255] ;

	getdate(&dateSys) ;
	gettime(&heureSys) ;
	sprintf(msg, "%02d/%02d/%4d - %02d:%02d:%02d,%02d>", dateSys.da_day, dateSys.da_mon,
                dateSys.da_year, heureSys.ti_hour, heureSys.ti_min,
                heureSys.ti_sec, heureSys.ti_hund) ;

	outFile << string(msg) ;
  outFile << string("\t\t\t\t") ;
  outFile << sMsg ;

	outFile.close() ;
}

Pilot::Pilot()
//============
{
  JNIEnv* jenv = JavaSystem::jenv ;

  //=== create a Pilot instance
	jmethodID mid = jenv->GetMethodID(classPilot, "<init>", "()V") ;
	if (0 == mid)
  {
    fprintf(stderr, "Cannot locate the Pilot constructor method. Exiting...\n") ;
	  newPilot = NULL ;
  }
	else
	  newPilot = jenv->NewObject(classPilot, mid) ;
}

Pilot::~Pilot()
//=============
{
}

char* Pilot::Execute(char* inputData, bool ifDOMElement)
//======================================================
{
	JNIEnv* jenv = JavaSystem::jenv ;

	//=== execute a Pilot instance
	jmethodID mid = jenv->GetMethodID(classPilot, "execute", "(Ljava/lang/String;Z)Ljava/lang/String;") ;
	if (0 == mid)
  {
    fprintf(stderr, "Cannot locate the Pilot execute method. Exiting...\n") ;
    return NULL ;
  }

  jstring jdescr = jenv->NewStringUTF(inputData) ;
  jstring jResult = (jstring)jenv->CallObjectMethod(newPilot, mid, jdescr, ifDOMElement) ;

  char *result = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
  printf("Result exec : %s\n", result) ;
  jenv->DeleteLocalRef(jdescr) ;
  jenv->DeleteLocalRef(newPilot) ;      //??? jResult

	return result ;
}

