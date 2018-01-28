/*//////////////////////////////////////////////////////*/
/*														*/
/*	Created by:	Dominique Sauquet						*/
/*  Created on:	May 2003								*/
/*														*/
/*//////////////////////////////////////////////////////*/

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/personLus.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

#include "pilot\NautilusPilot.hpp"
#include "nsbb\nsmanager.h"
#include "nssavoir\nspatlnk.h"
// #include "nsbb\tagNames.h"
#include "nsbb\nspatpat.h"

//tools services
string NautilusPilot::SERV_AGENTS_TEST                  = "agentsTest" ;
string NautilusPilot::SERV_GET_AGENT_LIST               = "getAgentList" ;
string NautilusPilot::SERV_GET_NEEDED_AGENT_LIST        = "getAllNeededAgentList" ;
string NautilusPilot::SERV_GET_TRAIT_LIST               = "getTraitList" ;

string NautilusPilot::SERV_SET_PARAMETERS               = "setEpisodusParameters" ;

string NautilusPilot::SERV_USER_LIST                    = "userList" ;
string NautilusPilot::SERV_USER_LIST_WITH_ROLES         = "userListWithRoles" ;
string NautilusPilot::SERV_LOGIN                        = "login" ;
string NautilusPilot::SERV_PATIENT_LIST                 = "patientList" ;
//patient list in group server
string NautilusPilot::SERV_GROUP_PATIENT_LIST           = "searchGroupPersonsFromTraits" ;
string NautilusPilot::SERV_OBJECT_LIST             	    = "searchObjectFromPartialTraits" ;
string NautilusPilot::SERV_OBJECT_LIST_WITH_TRAITS      = "getObjectListWithTraits" ;
string NautilusPilot::SERV_SEARCH_PATIENT               = "searchPatient" ;
//open group patient
string NautilusPilot::SERV_OPEN_GROUP_PATIENT           = "openPatientData" ;
//create group patient for epiPump
string NautilusPilot::SERV_CREATE_GROUP_PATIENT         = "createGroupPatient" ;
//modify group patient for epiPump
string NautilusPilot::SERV_MODIFY_GROUP_PATIENT         = "modifyGroupPerson" ;


string NautilusPilot::SERV_SEARCH_PATIENT_FROM_TRAITS   = "searchPatientFromTraits" ;
string NautilusPilot::SERV_CREATE_PATIENT               = "createPatient" ;
string NautilusPilot::SERV_CREATE_CORESPONDENT          = "createCorrespondent" ;
string NautilusPilot::SERV_CREATE_OBJECT                = "createObject" ;
string NautilusPilot::SERV_MODIFY_OBJECT                = "modifyObject" ;
string NautilusPilot::SERV_SEARCH_OBJECT                = "searchObjectFromTraits" ;
string NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID        = "searchObjectFromId" ;
string NautilusPilot::SERV_SEARCH_OBJECT_HAVING_TRAITS  = "searchAllObjectsHavingTraits" ;
string NautilusPilot::SERV_CREATE_USER                  = "createUser" ;
string NautilusPilot::SERV_MODIFY_USER                  = "modifyUser" ;
string NautilusPilot::SERV_MODIFY_PERSON                = "modifyPerson" ;
string NautilusPilot::SERV_MODIFY_PERSON_TRAITS         = "modifyPersonTraits" ;
string NautilusPilot::SERV_MODIFY_PERSON_ROLE           = "modifyPersonRole" ;
string NautilusPilot::SERV_MODIFY_GRAPH_PERSON          = "modifyGraphPerson" ;
string NautilusPilot::SERV_SEARCH_PERSON                = "searchPersons" ;
string NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS      = "searchPersonsFromTraits" ;
string NautilusPilot::SERV_READ_GRAPH_ADMIN             = "readTreesInGraph" ;
string NautilusPilot::SERV_READ_ADRESS_GRAPH            = "readTreesInObjectGraph" ;
string NautilusPilot::SERV_UNLOCK                       = "unlockPatient" ;
string NautilusPilot::SERV_UNLOCK_ALL_PATIENTS          = "unlockAllPatients" ;
string NautilusPilot::SERV_USER_PROPERTIES_LIST         = "getUserProperties" ;
string NautilusPilot::SERV_PERSON_TRAITS_LIST           = "getPersonTraits" ;

//syncronizing services
string NautilusPilot::SERV_USER_IMPORT              = "importUser" ;
string NautilusPilot::SERV_CORRESPONDENT_IMPORT     = "importCorrespondent" ;
string NautilusPilot::SERV_CORRESPONDENT_IMPORT_ID  = "importCorrespondantWithId" ;
string NautilusPilot::SERV_PATIENT_IMPORT           = "importPatient" ;
//import group patient
string NautilusPilot::SERV_GROUP_PATIENT_IMPORT     = "importGroupPatient" ;


string NautilusPilot::SERV_CREATE_IMPORTED_PATIENT  = "createImportedPatient" ;
//create imported group patient
string NautilusPilot::SERV_CREATE_IMPORTED_GROUP_PATIENT = "createImportedGroupPatient" ;

string NautilusPilot::SERV_CREATE_IMPORTED_CORRESP  = "createImportedCorrespondent" ;
string NautilusPilot::SERV_CREATE_IMPORTED_USER     = "createImportedUser" ;
string NautilusPilot::SERV_GET_PERSON_INFORMATION   = "getPersonInformation" ;
string NautilusPilot::SERV_UPDATE_ALL_LDV_OBJECTS   = "importNewObjectsData" ;
string NautilusPilot::SERV_EXPORT_DATA              = "synchroLocalToCollective" ;
string NautilusPilot::SERV_IMPORT_DATA              = "readNewCollectiveData" ;
string NautilusPilot::SERV_ADD_MANDATE              = "addHealthTeamMembre" ;

//merging services
string NautilusPilot::SERV_MERGE_PATIENT                    = "mergePatient" ;
string NautilusPilot::SERV_OPEN_PATIENT_DATA_FROM_TRAITS    = "openPatientDataFromTraits" ;

// #define OBJECT_ID_LEN  PAT_NSS_LEN + DOC_CODE_DOCUM_LEN

jclass    NautilusPilot::classNautilusPilot   = NULL ;
jclass    NautilusPilot::classNautilusGraph   = NULL ;
jclass    NautilusPilot::classNode            = NULL ;
jclass    NautilusPilot::classMapping         = NULL ;
jclass    NautilusPilot::classDOMElement      = NULL ;
jclass    NautilusPilot::classStr             = NULL ;

jmethodID NautilusPilot::midReadGraphService  = NULL ;
jmethodID NautilusPilot::midReadGraph         = NULL ;
jmethodID NautilusPilot::midWriteGraph        = NULL ;
jmethodID NautilusPilot::midChildrenElements  = NULL ;
jmethodID NautilusPilot::midInvoke            = NULL ;
jmethodID NautilusPilot::midSimpInvoke        = NULL ;
jmethodID NautilusPilot::midNewGraph          = NULL ;
jmethodID NautilusPilot::midGetGraphId        = NULL ;
jmethodID NautilusPilot::midGetTreeId         = NULL ;

jmethodID NautilusPilot::midGetCurrentStep    = NULL ;
jmethodID NautilusPilot::midGetCurrentService = NULL ;

jmethodID NautilusPilot::midModify            = NULL ;

jfieldID NautilusPilot::jFidMnemo             = NULL ;
jfieldID NautilusPilot::jFidWarning           = NULL ;
jfieldID NautilusPilot::jFidStack             = NULL ;
jfieldID NautilusPilot::jFidStep							= NULL ;

long NSBasicAttribute::lObjectCount = 0 ;
long NSBasicAttributeArray::lObjectCount = 0 ;
long NSPersonsAttributesArray::lObjectCount = 0 ;

/*
unsigned char* ASCIItoUNICODE (unsigned char ch)
{
  unsigned char Val[2] ;
  if ((ch < 192) && (ch != 168) && (ch != 184))
  {
    Val[0] = 0 ;
    Val[1] = ch ;
    return Val ;
  }
  if (ch == 168)
  {
    Val[0] = 208 ;
    Val[1] = 129 ;
    return Val ;
  }
  if (ch == 184)
  {
    Val[0] = 209 ;
    Val[1] = 145 ;
    return Val ;
  }
  if (ch < 240)
  {
    Val[0] = 208 ;
    Val[1] = ch - 48 ;
    return Val ;
  }
  if (ch < 249)
  {
    Val[0] = 209 ;
    Val[1] = ch - 112 ;
    return Val ;
  }
  return NULL ;
}

unsigned int* ConvertString (unsigned char *string)
{
  unsigned int size = 0 ;
  while (0 != string[size++]) ;

  unsigned int *NewString = (unsigned int*)malloc(sizeof(unsigned int)*2*size-1) ;
  NewString[0] = 2 * size - 1 ;

  size = 0 ;
  while (string[size] != 0)
  {
    unsigned char* Uni = ASCIItoUNICODE(string[size]) ;
    NewString[2*size+1] = Uni[0] ;
    NewString[2*size+2] = Uni[1] ;
    size++ ;
  }

  return NewString ;
}
*/

/*
char* jstringToWindows(JNIEnv *jenv, jstring jstr)
{
  int length = jenv->GetStringLength(jstr) ;

  const jchar jcstr = * jenv->GetStringChars(jstr, 0) ;

  char *rtn = (char *) malloc(length * 2 + 1) ;

  int size = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) jcstr, length, rtn (length * 2 +1), NULL, NULL);
  if (size <= 0)
    return NULL ;

  jenv->ReleaseStringChars(jstr, jcstr) ;

  rtn[size] = 0 ;

  Return rtn ;
}
*/

// -----------------------------------------------------------------------------
// NautilusPilot
// -----------------------------------------------------------------------------
NautilusPilot::NautilusPilot(NSSuper* pSuper)
              :Pilot(), NSSuperRoot(pSuper)
//====================================================================
{
  _localJEnv      = (JNIEnv*) 0 ;
  
  _bJEnvOnDuty    = false ;
  _bServiceOnDuty = false ;
  _bPilotOnDuty   = false ;

	JNIEnv* jenv = JavaSystem::jenv ;
  if ((JNIEnv*) NULL == jenv)
  {
    fprintf(stderr, "NautilusPilot ctor : jenv is null. Exiting...\n") ;
    return ;
  }

	//=== execute method init of the NautilusPilot class
	jmethodID jmidNewNautPilot = jenv->GetMethodID(classNautilusPilot, "<init>", "(Lpilot/Pilot;)V");
	if ((0 == jmidNewNautPilot) || jenv->ExceptionCheck())
  {
    fprintf(stderr, "NautilusPilot ctor : Cannot locate the NautilusGraph constructor method. Exiting...\n");
    return ;
  }

	jNautilusPilot = jenv->NewObject(classNautilusPilot, jmidNewNautPilot, newPilot) ;
}

NautilusPilot::~NautilusPilot()
//=============================
{
}

JNIEnv*
NautilusPilot::StartPilotMethod()
{
  // Wait for services to be free
  //
  while (_bServiceOnDuty || _bPilotOnDuty)
    _pSuper->Delay(10) ;

  _bPilotOnDuty = true ;

  JNIEnv* jenv = GetJniEnv() ;

  if ((JNIEnv*) NULL == jenv)
  {
    _bPilotOnDuty = false ;
    return (JNIEnv*) 0 ;
  }

	return jenv ;
}

bool
NautilusPilot::EndPilotMethod(JNIEnv* jenv)
{
  bool bRelease = ReleaseJniEnv(jenv) ;
  _bPilotOnDuty = false ;
  return bRelease ;
}

// Get JNIEnv pointer... and maybe wait until it gets free
//
JNIEnv*
NautilusPilot::GetJniEnv()
{
  // Wait for pipe to be free
  //
   while (_bJEnvOnDuty)
    _pSuper->Delay(10) ;

  _bJEnvOnDuty = true ;

  if (_localJEnv)
    return _localJEnv ;

  return JavaSystem::jenv ;
}

// Return false if an exception occured
//
bool
NautilusPilot::ReleaseJniEnv(JNIEnv* jenv)
{
  // Convention: no pointer means no exception
  //
  if ((JNIEnv*) NULL == jenv)
  {
    _bJEnvOnDuty = false ;
    return true ;
  }

  bool bCallWasOk = (false == JniExceptionCheck(jenv)) ;

  _bJEnvOnDuty = false ;

  return bCallWasOk ;
}

// Returns true if an exception occured
//
bool
NautilusPilot::JniExceptionCheck(JNIEnv* jenv)
{
  // Convention: no pointer means no exception
  //
  if ((JNIEnv*) NULL == jenv)
    return false ;

  if (JNI_FALSE == jenv->ExceptionCheck())
    return false ;

  jenv->ExceptionDescribe() ;
  jenv->ExceptionClear() ;

  return true ;
}

//
// Previously (java 1.4), we wrote something like :
//     jstring jServiceName  = jenv->NewStringUTF(sServiceName.c_str()) ;
//
// But when switching to Java 1.6 this function no longer managed non standard
// chars correctly. I.e. "£SPID1" from C++ became "£SPID" on the Java side
//
jstring
NautilusPilot::WindowsTojstring(JNIEnv* jenv, const char * str)
{
  if (((const char *) NULL == str) || ((JNIEnv*) NULL == jenv))
    return (jstring) 0 ;

  jstring rtn = (jstring) 0 ;

  int slen = strlen(str) ;

  if (0 == slen)
    rtn = jenv->NewStringUTF(str) ;
  else
  {
    int length = MultiByteToWideChar(CP_ACP, 0, (LPCSTR) str, slen, NULL, 0) ;

    unsigned short *buffer = new unsigned short[length * 2 + 1] ;

    if (MultiByteToWideChar(CP_ACP, 0, (LPCSTR) str, slen, (LPWSTR) buffer, length) > 0)
      rtn = jenv->NewString((jchar *) buffer, length) ;

    delete[] buffer ;
  }

  return rtn ;
}

jstring
NautilusPilot::WinStringTojstring(JNIEnv* jenv, const string sStr)
{
  return WindowsTojstring(jenv, sStr.c_str()) ;
}

int
NautilusPilot::Init()
//=======================
{
try
{
  Trace(string("Entering NautilusPilot::Init\n")) ;

	JNIEnv* jenv = JavaSystem::jenv ;

  //=== find the classes
  classNautilusPilot = jenv->FindClass("nautilus/NautilusPilot") ;
  if ((0 == classNautilusPilot) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the nautilus/NautilusPilot class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the nautilus/NautilusPilot class. Exiting...\n") ;
    return -1 ;
	}

  classNautilusGraph = jenv->FindClass("nautilus/NautilusGraph") ;
  if ((0 == classNautilusGraph) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the nautilus/NautilusGraph class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the nautilus/NautilusGraph class. Exiting...\n") ;
    return -2 ;
	}

  classNode = jenv->FindClass("graphServer/Node") ;
  if ((0 == classNode) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the graphServer/Node class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the graphServer/Node class. Exiting...\n") ;
    return -3 ;
	}

  classMapping = jenv->FindClass("graphServer/Mapping") ;
  if ((0 == classMapping) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the graphServer/Mapping class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the graphServer/Mapping class. Exiting...\n") ;
    return -4 ;
  }

  classDOMElement = jenv->FindClass("domlightwrapper/DOMElement") ;
  if ((0 == classDOMElement) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the domlightwrapper/DOMElement class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the domlightwrapper/DOMElement class. Exiting...\n") ;
    return -5 ;
	}

  midChildrenElements = jenv->GetMethodID(classDOMElement, "getChildrenElements",
                "(Ljava/lang/String;)[Ldomlightwrapper/DOMElement;") ;
  if ((0 == midChildrenElements) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the DOMElement getChildrenElements class. Exiting...\n") ;
    Trace(sMsg) ;
  	// erreur("Cannot locate the DOMElement getChildrenElements method. Exiting...\n", standardError) ;
    return -6 ;
	}

  //=== get method id for the NautilusPilot.readGraph method
  midReadGraph =  jenv->GetMethodID(classNautilusPilot, "readGraph",
                    "(Ljava/lang/String;)Lnautilus/NautilusGraph;") ;

	midReadGraphService = jenv->GetMethodID(classNautilusPilot, "readGraph",
                          "(Ljava/lang/String;Ljava/lang/String;)Lnautilus/NautilusGraph;") ;
	if ((0 == midReadGraph) || (0 == midReadGraphService) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot readGraph class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusPilot readGraph method. Exiting...\n") ;
    return -7 ;
	}

  //=== get method id for the NautilusPilot.writeGraph method

  midWriteGraph = jenv->GetMethodID(classNautilusPilot, "writeGraph",
                    "(Lnautilus/NautilusGraph;)[LgraphServer/Mapping;") ;
	if ((0 == midWriteGraph) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot writeGraph class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusPilot writeGraph method. Exiting...\n") ;
    return -8 ;
	}

  //=== Get method id for the NautilusPilot.invokeService method
  //
  midInvoke = jenv->GetMethodID(classNautilusPilot, "invokeService",
                    "(Ljava/lang/String;Lnautilus/NautilusGraph;[Ljava/lang/String;)[LgraphServer/Mapping;") ;

	if ((0 == midInvoke) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot invokeService class. Exiting...\n") ;
    Trace(sMsg) ;
		// fprintf(stderr, "Cannot locate the NautilusPilot invokeService method. Exiting...\n") ;
    return -9 ;
	}

	midSimpInvoke = jenv->GetMethodID(classNautilusPilot, "invokeService",
            "(Ljava/lang/String;[Ljava/lang/String;)Lnautilus/NautilusGraph;") ;
	if ((0 == midSimpInvoke) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot invokeService class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusPilot invokeService method. Exiting...\n") ;
    return -10 ;
	}

  // Get ID of _mnemo String variable
  //
  jFidMnemo = jenv->GetFieldID(classNautilusPilot, "_mnemo", "Ljava/lang/String;") ;
  if ((0 == jFidMnemo) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot _mnemo variable. Exiting...\n") ;
    Trace(sMsg) ;
  	return -11 ;
  }

  // Get ID of _stack String variable
  //
	jFidStack = jenv->GetFieldID(classNautilusPilot, "_stack", "Ljava/lang/String;") ;
  if ((0 == jFidStack) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot stack class. Exiting...\n") ;
    Trace(sMsg) ;
  	return -12 ;
  }

  // Get ID of _stepError String variable
  //
	jFidStep = jenv->GetFieldID(classNautilusPilot, "_stepError", "Ljava/lang/String;") ;
  if ((0 == jFidStep) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot stepError class. Exiting...\n") ;
    Trace(sMsg) ;
  	return -13 ;
  }

  // Get ID of _warningMessage String variable
  //
	jFidWarning = jenv->GetFieldID(classNautilusPilot, "_warningMessage", "Ljava/lang/String;") ;
  if ((0 == jFidWarning) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot warningMessage class. Exiting...\n") ;
    Trace(sMsg) ;
  	return -14 ;
  }

	//=== get method id for the NautilusGraph constructor method
  midNewGraph = jenv->GetMethodID(classNautilusGraph, "<init>",
                                    "(SLjava/lang/String;IIII)V") ;
	if ((0 == midNewGraph) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusGraph constructor method. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusGraph constructor method. Exiting...\n") ;
    return -15 ;
	}

	//=== get method id for the NautilusGraph getTreeId method
  midGetTreeId = jenv->GetMethodID(classNautilusGraph, "getTreeId",
                                    "(I)Ljava/lang/String;") ;
	if ((0 == midGetTreeId) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusGraph getTreeId method. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusGraph getTreeId method. Exiting...\n") ;
    return -16 ;
	}

  //=== get method id for the NautilusGraph getGraphId method
  midGetGraphId = jenv->GetMethodID(classNautilusGraph, "getGraphId",
                                    "()Ljava/lang/String;") ;
	if ((0 == midGetGraphId) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusGraph getGraphId method. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusGraph getGraphId method. Exiting...\n") ;
    return -17 ;
	}

	classStr = jenv->FindClass("java/lang/String") ;
  if ((0 == classStr) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the java/lang/String class. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the java/lang/String class. Exiting...\n") ;
    return -18 ;
	}

	//=== execute method init of the NautilusPilot class
	jmethodID midInit = jenv->GetStaticMethodID(classNautilusPilot, "init", "()V") ;
	if ((0 == midInit) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusPilot init method. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusPilot init method. Exiting...\n") ;
    return -19 ;
	}

  Trace(string("Calling the NautilusPilot init java method\n")) ;

	jenv->CallStaticObjectMethod(classNautilusPilot, midInit) ;

  //=== get method id for the NautilusGraph getGraphId method
  midGetCurrentStep = jenv->GetMethodID(classNautilusPilot, "getCurrentStepName",
                                    "()Ljava/lang/String;") ;
  if ((0 == midGetCurrentStep) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusGraph getCurrentStepName method. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusGraph getCurrentStepName method. Exiting...\n") ;
    return -20 ;
	}

  //=== get method id for the NautilusGraph getGraphId method
  midGetCurrentService = jenv->GetMethodID(classNautilusPilot, "getCurrentServiceName",
                                    "()Ljava/lang/String;") ;
  if ((0 == midGetCurrentService) || jenv->ExceptionCheck())
  {
    string sMsg = string("Cannot locate the NautilusGraph getCurrentServiceName method. Exiting...\n") ;
    Trace(sMsg) ;
  	// fprintf(stderr, "Cannot locate the NautilusGraph getCurrentServiceName method. Exiting...\n") ;
    return -21 ;
	}

  midModify = jenv->GetMethodID(classNautilusPilot, "invokeService",
                "(Ljava/lang/String;Lnautilus/NautilusGraph;[Ljava/lang/String;)[LgraphServer/Mapping;") ;
	if ((0 == midModify) || jenv->ExceptionCheck())
	{
    string sMsg = string("Cannot locate the NautilusPilot invokeService (2) method. Exiting...\n") ;
    Trace(sMsg) ;
  	erreur("Cannot locate the NautilusPilot invokeService (2) method.",standardError) ;
		return -22 ;
	}

  Trace(string("Leaving NautilusPilot::Init\n")) ;

	return 0 ;
}
catch (...)
{
  string sErrorText = string("Exception NautilusPilot::Init.") ;
  Trace(sErrorText) ;
  erreur(sErrorText.c_str(), standardError) ;
	return -14 ;  // FIXME should be -23 ?
}
}

void
NautilusPilot::traceAndDisplayError(string sText)
{
  if (string("") == sText)
    return ;

  _pSuper->trace(&sText, 1, NSSuper::trError) ;
  erreur(sText.c_str(), standardError) ;
}

void
NautilusPilot::Trace(const string &sMsg)
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

int
NautilusPilot::jni_getNbTrees(JNIEnv* jenv, jobject jGraph)
//=============================================================
{
try
{
  if ((NULL == jGraph) || ((JNIEnv*) NULL == jenv))
  	return -1 ;

	// === Get method id for the NautilusGraph.getNumberOfTrees method
  //
	jmethodID mid = jenv->GetMethodID(classNautilusGraph, "getNumberOfTrees", "()I") ;
	if ((NULL == mid) || jenv->ExceptionCheck())
	{
		traceAndDisplayError(string("Cannot locate the NautilusGraph getNumberOfTrees method. Exiting...")) ;
		return -1 ;
	}

  // === Call method
  //
  int iResult = jenv->CallIntMethod(jGraph, mid) ;

  if (jenv->ExceptionCheck())
    return -1 ;

  return iResult ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getNbTrees")) ;
	return -1 ;
}
}

int
NautilusPilot::jni_getNbLinks(JNIEnv* jenv, jobject jGraph)
//=============================================================
{
try
{
  if ((NULL == jGraph) || ((JNIEnv*) NULL == jenv))
  	return -1 ;

	//=== Get method id for the NautilusGraph.getNumberOfLinks method
  //
	jmethodID mId = jenv->GetMethodID(classNautilusGraph, "getNumberOfLinks", "()I") ;
	if ((NULL == mId) || JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("Cannot locate the NautilusGraph getNumberOfLinks method. Exiting...")) ;
		return -1 ;
	}

  //=== Get method id for the NautilusGraph.getNumberOfLinks method
  //
	int iResult = jenv->CallIntMethod(jGraph, mId) ;

  if (jenv->ExceptionCheck())
  {
    traceAndDisplayError(string("Failed calling the NautilusGraph getNumberOfLinks method. Exiting...")) ;
    return -1 ;
  }

  return iResult ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getNbLinks")) ;
	return -1 ;
}
}

int
NautilusPilot::jni_getNbModels(JNIEnv* jenv, jobject jGraph)
//==============================================================
{
try
{
  if ((NULL == jGraph) || ((JNIEnv*) NULL == jenv))
  	return -1 ;

	//=== Get method id for the NautilusGraph.getNumberOfModels method
  //
	jmethodID mid = jenv->GetMethodID(classNautilusGraph, "getNumberOfModels", "()I") ;
	if ((NULL == mid) || JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("Cannot locate the NautilusGraph getNumberOfModels method. Exiting...")) ;
		return -1 ;
	}

  //=== Call method
  //
	int iResult = jenv->CallIntMethod(jGraph, mid) ;

  if (JniExceptionCheck(jenv))
  {
		traceAndDisplayError(string("Failed calling the NautilusGraph getNumberOfModels method. Exiting...")) ;
		return -1 ;
	}

  return iResult ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getNbModels")) ;
	return -1 ;
}
}

int
NautilusPilot::jni_getNbRights(JNIEnv* jenv, jobject jGraph)
//==============================================================
{
try
{
  if ((NULL == jGraph) || ((JNIEnv*) NULL == jenv))
  	return -1 ;

  //=== Get method id for the NautilusGraph.getNumberOfRights method
  //
	jmethodID mid = jenv->GetMethodID(classNautilusGraph, "getNumberOfRights", "()I") ;
	if ((0 == mid) || JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("Cannot locate the NautilusGraph getNumberOfRights method. Exiting...")) ;
		return -1 ;
	}

  //=== Call method
  //
	int iResult = jenv->CallIntMethod(jGraph, mid) ;

  if (JniExceptionCheck(jenv))
  {
		traceAndDisplayError(string("Failed calling the NautilusGraph getNumberOfRights method. Exiting...")) ;
		return -1 ;
	}

  return iResult ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getNbRights")) ;
	return -1 ;
}
}

string
NautilusPilot::jni_getAttribute(JNIEnv* jenv, jobject jGraph, char* method, int i)
//======================================================================
{
try
{
  if ((NULL == method) || (NULL == jGraph) || ((JNIEnv*) NULL == jenv))
  	return string("") ;

	//=== Get method id for the NautilusGraph method
  //
	jmethodID mid = jenv->GetMethodID(classNautilusGraph, method, "(I)Ljava/lang/String;") ;
	if ((NULL == mid) || JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("NautilusPilot::jni_getAttribute : Cannot locate the NautilusGraph method. Exiting...")) ;
		return string("") ;
	}

	jstring jResult = reinterpret_cast<jstring>(jenv->CallObjectMethod(jGraph, mid, i)) ;
  if ((NULL == jResult) || JniExceptionCheck(jenv))
  {
  	traceAndDisplayError(string("NautilusPilot::jni_getAttribute : CallObjectMethod failed. Exiting...")) ;
  	return string("") ;
  }

	char *result = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
  jenv->DeleteLocalRef(jResult) ;

  string sResult = string("") ;

  if (result)
  {
    sResult = string(result) ;
    delete[] result ;
  }

	return sResult ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getAttribute")) ;
	return NULL ;
}
}

string
NautilusPilot::jni_getGraphId(JNIEnv* jenv, jobject jGraph)
//================================================================
{
try
{
  if ((NULL == jGraph) || ((JNIEnv*) NULL == jenv))
  	return string("") ;

	//=== execute NautilusGraph.getGraphId method
  //
	jstring jResult = reinterpret_cast<jstring>(jenv->CallObjectMethod(jGraph, midGetGraphId)) ;
  if ((NULL == jResult) || JniExceptionCheck(jenv))
  {
    traceAndDisplayError(string("NautilusPilot::jni_getGraphId : CallObjectMethod failed. Exiting...")) ;
    return string("") ;
  }

	char *result = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
	jenv->DeleteLocalRef(jResult) ;

	if ((char*) NULL != result)
	{
		string id = string(result) ;
		delete[] result ;
		return id ;
	}

	return string("") ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getGraphId")) ;
	return string("") ;
}
}

/*   Return curent step name being executed
*/
string NautilusPilot::getCurrentStepName()
//========================================
{
try
{
  JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
    return string("") ;

	//=== execute NautilusGraph.getGraphId method
	// jstring jResult = (jstring) jenv->CallObjectMethod(jNautilusPilot, midGetCurrentStep) ;
  jstring jResult = reinterpret_cast<jstring>(jenv->CallObjectMethod(jNautilusPilot, midGetCurrentStep)) ;
  if ((NULL == jResult) || JniExceptionCheck(jenv))
  {
    ReleaseJniEnv() ;
    fprintf(stderr, "NautilusPilot::getCurrentStepName : CallObjectMethod failed. Exiting...\n") ;
    return string("") ;
  }

	char *result = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
	jenv->DeleteLocalRef(jResult) ;

  ReleaseJniEnv(jenv) ;

	if ((char*) NULL != result)
	{
		string mes = string(result) ;
		delete[] result ;
		return mes ;
	}

	return string("") ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getCurrentStepName")) ;
	return string("") ;
}
}

/*
*   Return curent step name being executed
*/
string NautilusPilot::getCurrentServiceName()
//=======================================================
{
try
{
  JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
    return string("") ;

	//=== execute NautilusGraph.getGraphId method
	jstring jResult = reinterpret_cast<jstring>(jenv->CallObjectMethod(jNautilusPilot, midGetCurrentService)) ;
  if ((NULL == jResult) || JniExceptionCheck(jenv))
  {
    ReleaseJniEnv() ;
    fprintf(stderr, "NautilusPilot::midGetCurrentService : CallObjectMethod failed. Exiting...\n") ;
    return string("") ;
  }

	char *result = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
	jenv->DeleteLocalRef(jResult) ;

  ReleaseJniEnv(jenv) ;

	if ((char*) NULL != result)
	{
		string mes = string(result) ;
		delete[] result ;
		return mes ;
	}

	return string("") ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getCurrentServiceName")) ;
	return string("") ;
}
}

/**
*  Return curent step user name being executed.
* User name is defined in localisation.dat. The service name is chapter name.
*/
string
NautilusPilot::getCurrentStepUserName(string stepName, string serviceName)
//==============================================================================
{
  if ((string("") == stepName) || (string("") == serviceName))
    return string("") ;

  string sText = _pSuper->getText(serviceName, stepName) ;
  if (string("") == sText) //stepName message not found in the localisation file
    sText = stepName ;     //keep the stepName

  return sText ;
}

/**
*  Return curent step user name being executed.
* User name is defined in localisation.dat. The service name is chapter name.
*/
string
NautilusPilot::getCurrentStepUserName()
//============================================
{
  string stepName    = getCurrentStepName() ;
  string serviceName = getCurrentServiceName() ;

  if ((string("") == stepName) || (string("") == serviceName))
    return string("") ;

  string sText = _pSuper->getText(serviceName, stepName) ;
  if (string("") == sText) //stepName message not found in the localisation file
    sText = stepName ;     //keep the stepName

  return sText ;
}

string
NautilusPilot::jni_getTreeId(JNIEnv* jenv, jobject jGraph, int i)
//======================================================================
{
try
{
  if ((NULL == jGraph) || ((JNIEnv*) NULL == jenv))
		return string("") ;

	//=== execute NautilusGraph.getTreeId method
	jstring jResult = reinterpret_cast<jstring>(jenv->CallObjectMethod(jGraph, midGetTreeId, i)) ;
  if ((NULL == jResult) || JniExceptionCheck(jenv))
  {
  	traceAndDisplayError(string("NautilusPilot::jni_getTreeId : CallObjectMethod failed. Exiting...")) ;
  	return string("") ;
  }

	char *result = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
	jenv->DeleteLocalRef(jResult) ;

	if ((char*) NULL != result)
	{
		string id = string(result) ;
		delete[] result ;
		return id ;
	}

	return string("") ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getTreeId")) ;
	return string("") ;
}
}

/*
jobject NautilusPilot::jni_getTree(jobject jGraph, int i)
//=====================================================================
{
try
{
  if (NULL == jGraph)
		return NULL ;

  JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
		return NULL ;

	//=== Get method id for the NautilusGraph.getTree method
  //
	jmethodID mid = jenv->GetMethodID(classNautilusGraph, "getTree", "(I)LgraphServer/Tree;") ;
	if ((0 == mid) || JniExceptionCheck(jenv))
	{
    ReleaseJniEnv() ;
		fprintf(stderr, "Cannot locate the NautilusGraph method. Exiting...\n") ;
		return NULL ;
	}

  //=== Call method
  //
	jobject resultObj = jenv->CallObjectMethod(jGraph, mid, i) ;

  if (false == ReleaseJniEnv(jenv))
    return (jobject) 0 ;

  return resultObj ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getTree")) ;
	return NULL ;
}
}
*/

jobjectArray
NautilusPilot::jni_getTreeNodes(JNIEnv* jenv, jobject jGraph, int i)
//===============================================================================
{
try
{
  if (((jobject) NULL == jGraph) || ((JNIEnv*) NULL == jenv))
		return (jobjectArray) 0 ;

	//=== Get method id for the NautilusGraph.getTreeNodes method
  //
	jmethodID mid = jenv->GetMethodID(classNautilusGraph, "getTreeNodes", "(I)[LgraphServer/Node;") ;
	if ((0 == mid) || JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("jni_getTreeNodes: Cannot locate the NautilusGraph method. Exiting...")) ;
		return (jobjectArray) 0 ;
	}

  jobjectArray resultObjArray = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jGraph, mid, i)) ;
  if (JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("jni_getTreeNodes: Failed calling the NautilusGraph method. Exiting...")) ;
		return (jobjectArray) 0 ;
	}

  return resultObjArray ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getTreeNodes")) ;
	return NULL ;
}
}

string
NautilusPilot::jni_getNodeField(JNIEnv* jenv, char* fieldName, jobject aNode)
//=================================================================================
{
try
{
  if ((NULL == fieldName) || (NULL == aNode) || ((JNIEnv*) NULL == jenv))
		return string("") ;

	//=== Get method id for the NautilusGraph.getTreeId method
  //
	jfieldID jFid = jenv->GetFieldID(classNode, fieldName, "Ljava/lang/String;") ;
	if (JniExceptionCheck(jenv) || (NULL == jFid))
  {
  	traceAndDisplayError(string("NautilusPilot::jni_getNodeField : GetFieldID failed. Exiting...")) ;
		return string("") ;
  }

	jstring jResult = reinterpret_cast<jstring>(jenv->GetObjectField(aNode, jFid)) ;
	if (JniExceptionCheck(jenv) || (NULL == jResult))
  {
		traceAndDisplayError(string("NautilusPilot::jni_getNodeField : GetObjectField failed. Exiting...")) ;
		return string("") ;
  }

	char *result = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
  jenv->DeleteLocalRef(jResult) ;

	string sResult = string("") ;

  if (result)
  {
    sResult = string(result) ;
    delete[] result ;
  }

	return sResult ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getNodeField")) ;
	return NULL ;
}
}

char**
NautilusPilot::jni_getMappingField(JNIEnv* jenv, char* fieldName,
                                        jobjectArray jMappings, jint nbMaps)
//==========================================================================
{
try
{
  if ((NULL == fieldName) || (NULL == jMappings) || (nbMaps <= 0) || ((JNIEnv*) NULL == jenv))
		return (char**) 0 ;

	//=== Get field id for a field of the graphServer.Mapping class
  //
	jfieldID jFid = jenv->GetFieldID(classMapping, fieldName, "Ljava/lang/String;") ;
  if (JniExceptionCheck(jenv) || (NULL == jFid))
	{
		traceAndDisplayError(string("NautilusPilot::jni_getMappingField : GetFieldID failed. Exiting...")) ;
		return (char**) 0 ;
	}

	char** attributes = new char*[nbMaps] ;
	for (int i = 0 ; i < nbMaps ; i++)
	{
		jobject obj = jenv->GetObjectArrayElement(jMappings, i) ;
		if ((NULL != obj) && (false == JniExceptionCheck(jenv)))
		{
			jstring jResult = reinterpret_cast<jstring>(jenv->GetObjectField(obj, jFid)) ;
			if ((NULL != jResult) && (false == JniExceptionCheck(jenv)))
      {
				attributes[i] = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
        jenv->DeleteLocalRef(jResult) ; //add by non jnisien
      }
      else
        traceAndDisplayError(string("NautilusPilot::jni_getMappingField : GetObjectField failed.")) ;

      jenv->DeleteLocalRef(obj) ;
		}
    else
      traceAndDisplayError(string("NautilusPilot::jni_getMappingField : GetObjectArrayElement failed.")) ;
	}

	return attributes ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::jni_getMappingField")) ;
	return (char**) 0 ;
}
}

const char*
NautilusPilot::getTextError(JNIEnv* jenv, string sChap, bool bNotFound)
//=====================================================================
{
  if ((JNIEnv*) NULL == jenv)
    return "" ;

try
{
	jstring jMnemo = reinterpret_cast<jstring>(jenv->GetObjectField(jNautilusPilot, jFidMnemo)) ;
	if ((NULL != jMnemo)    &&         // mnemo error information
  		(string(JavaSystem::NewStringWithSpecialCharacters(jMnemo)) != "") )
	{
		// jsize size = (jsize)jenv->GetStringUTFLength(jMnemo) ;
		string sMnemo = string(JavaSystem::NewStringWithSpecialCharacters(jMnemo)) ;
    string sErrorText = _pSuper->getText(sChap, sMnemo) ;
		if (sErrorText == "" )    //mnemo error not found in the localisation file
			sErrorText = sMnemo ;   //keep the mnemo

    return sErrorText.c_str() ;
	}

  // Try to get stack information
  jstring jStack = reinterpret_cast<jstring>(jenv->GetObjectField(jNautilusPilot, jFidStack)) ;
  if (NULL != jStack)
  {
    return JavaSystem::NewStringWithSpecialCharacters(jStack) ;
  }

  // Try to get a step error information
  jstring jStep = reinterpret_cast<jstring>(jenv->GetObjectField(jNautilusPilot, jFidStep)) ;
  if (NULL != jStep)
  {
    return JavaSystem::NewStringWithSpecialCharacters(jStep) ;
  }

	if (false == bNotFound)
		return (_pSuper->getText(sChap, "nonInstancedError")).c_str() ;
	else
		return "" ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getTextError")) ;
	return NULL ;
}
}

/*
    if 0 -- MAPPING_RETURN
     1 -- DOMELEMENT_RETURN
     2 -- GRAPH_RETURN
used from createPatient*/
int
NautilusPilot::getReturnType(JNIEnv* jenv)
//=========================================
{
try
{
	if ((JNIEnv*) NULL == jenv)
		return -1 ;

	jfieldID jFid = jenv->GetFieldID(classNautilusPilot, "_returnDataType", "I") ;
  if (JniExceptionCheck(jenv))
    return -1 ;

	int iRes = (jint)jenv->GetIntField(jNautilusPilot, jFid) ;
  if (JniExceptionCheck(jenv))
    return -1 ;

	switch (iRes)
	{
  	case 0:
    	return MAPPING_RETURN ;
		case 1:
			return DOMELEMENT_RETURN ;
		case 2:
			return GRAPH_RETURN ;
		case 3:
			return EMPTY_RETURN ;
    case 4:
      return WARNING_RETURN ;
	};

	return -1 ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getReturnType")) ;
	return -1 ;
}
}

string
NautilusPilot::getTextWarning(JNIEnv* jenv, string sChap)
//=======================================================
{
try
{
	if ((JNIEnv*) NULL == jenv)
		return string("") ;

	jstring jWarning = reinterpret_cast<jstring>(jenv->GetObjectField(jNautilusPilot, jFidWarning)) ;
	if ((NULL != jWarning) && (false == JniExceptionCheck(jenv))) // warnings
  {
    string sWarning = string(JavaSystem::NewStringWithSpecialCharacters(jWarning)) ;
		return _pSuper->getText(sChap, sWarning) ;
  }

  //return "nonInstancedWarning" ;
  return _pSuper->getText(sChap, "nonInstancedWarning") ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getTextWarning")) ;
	return string("") ;
}
}

// groupeTag = la balise resultat qui groupe plusieures personnes ou objects (ex. list)
//balise = la balise qui delimite un groupe (ex. person, object)
void
NautilusPilot::transformResultFieldInAttributesArray(JNIEnv* jenv, NSPersonsAttributesArray* pList,
                                            string groupeTag, string balise)
//=================================================================================================
{
  if (((NSPersonsAttributesArray*) NULL == pList) || ((JNIEnv*) NULL == jenv))
		return ;

try
{
	jmethodID midResult = jenv->GetMethodID(classNautilusPilot, "getResultExec",
                                    "(Ljava/lang/String;)Ldomlightwrapper/DOMElement;") ;

	if ((NULL == midResult) || JniExceptionCheck(jenv))
	{
		string sErrorText = _pSuper->getText("pilotManagement", "cannotGetJniMethodId") + string(" (fct : getResultExec)") ;
    traceAndDisplayError(sErrorText) ;
		return ;
	}

  jstring jName = WinStringTojstring(jenv, groupeTag) ;

	jobject jObject = jenv->CallObjectMethod(jNautilusPilot, midResult, jName) ;
	if ((NULL == jObject) || JniExceptionCheck(jenv))
  {
    string sError = string("NautilusPilot::transformResultFieldInAttributesArray : CallObjectMethod failed") ;
    _pSuper->trace(&sError, 1, NSSuper::trError) ;
    jenv->DeleteLocalRef(jName) ;
		return ;
  }

	jstring jNodeName = WinStringTojstring(jenv, balise) ;

	jobjectArray jDomElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jObject, midChildrenElements, jNodeName)) ;
  if ((NULL == jDomElems) || JniExceptionCheck(jenv))
	{
  	traceAndDisplayError(string("NautilusPilot::transformResultFieldInAttributesArray : CallObjectMethod failed")) ;
    jenv->DeleteLocalRef(jNodeName) ;
	  jenv->DeleteLocalRef(jName) ;
		return ;
	}

	jenv->DeleteLocalRef(jNodeName) ;
	jenv->DeleteLocalRef(jName) ;

	int count = jenv->GetArrayLength(jDomElems) ;
  if (JniExceptionCheck(jenv))
	{
  	traceAndDisplayError(string("NautilusPilot::transformResultFieldInAttributesArray : GetArrayLength failed")) ;
		return ;
	}

	if (string("*") == balise)
	{
		NSBasicAttributeArray* pIDAttributes = new NSBasicAttributeArray() ;
		for (int i = 0 ; i < count ; i++)
		{
			jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
			if ((NULL != jNodeElem) && (false == JniExceptionCheck(jenv)))
			{
        string sTagName = getTagName(jenv, jNodeElem) ;

				if ((string("") == sTagName) /*||(getText(jNodeElem) == "")*/ )
        {
        	delete pIDAttributes ;
        	return ;
        }

				NSBasicAttribute* pAttribute = new NSBasicAttribute() ;
				pAttribute->setBalise(sTagName) ;
				pAttribute->setText(getText(jenv, jNodeElem)) ;

				pIDAttributes->push_back(pAttribute) ;

				jenv->DeleteLocalRef(jNodeElem) ;
			}
      else
      {
      	traceAndDisplayError(string("NautilusPilot::transformResultFieldInAttributesArray : GetObjectArrayElement failed")) ;
      }
    }
		pList->push_back(pIDAttributes) ;
	}
	else
		for (int i = 0 ; i < count ; i++)
		{
			jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
			if (jNodeElem)
			{
				NSBasicAttributeArray* pIDAttributes = new NSBasicAttributeArray() ;

				DOMTreeToBasicAttribute(jenv, pIDAttributes, jNodeElem) ;
        if (pIDAttributes)
					pList->push_back(pIDAttributes) ;

				jenv->DeleteLocalRef(jNodeElem) ;
			}
      else
      {
      	traceAndDisplayError(string("NautilusPilot::transformResultFieldInAttributesArray : GetObjectArrayElement failed")) ;
      }
		}
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::transformResultFieldInAttributesArray")) ;
}
}

bool NautilusPilot::toBeChanged(string id)
//========================================
{
  if (string("") == id)
    return false ;

  if ('#' == id[0])
    return true ;

  return false ;
}

bool NautilusPilot::toBeChangedForComposedId(string id)
//=====================================================
{
  if (string("") == id)
    return false ;

  //in case of Graph of type Person : the test means "new person or new document"
  //in case of Graph of type Object : the test means "new object" (condition id[PAT_NSS_LEN] always false)

  if (('#' == id[0]) || ('#' == id[PAT_NSS_LEN]))     //for a patient
    return true ;
  else
    if (('$' == id[0]) && ('#' == id[1]))            //for a object
      return true ;

  return false ;
}

/*
bool
NautilusPilot::readGraph(NSDataGraph* pGraph, string sServiceName)
//=================================================================
{
try
{
  JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
  {
		traceAndDisplayError(string("NautilusPilot::readGraph : jenv is null. Exiting...")) ;
    return false ;
  }

	//=== execute a NautilusGraph.readGraph method
  //
	jstring jId = WinStringTojstring(pGraph->getGraphID()) ;

	jobject jGraph ;
	if (string("") != sServiceName)
	{
		jstring jServiceName = WinStringTojstring(sServiceName) ;
		// jGraph = jenv->CallObjectMethod(jNautilusPilot, midReadGraphService, jId, jServiceName) ;
    jGraph = CallObjectMethod(jNautilusPilot, midReadGraphService, jId, jServiceName) ;
	}
	else
		// jGraph = jenv->CallObjectMethod(jNautilusPilot, midReadGraph, jId) ;
    jGraph = CallObjectMethod(jNautilusPilot, midReadGraph, jId) ;

	if ((NULL == jGraph) || JniExceptionCheck(jenv))
  {
  	traceAndDisplayError(string("NautilusPilot::readGraph : CallObjectMethod failed. Exiting...")) ;
    return false ;
	}

	// build a C graph from the java graph object
	//===========================================
	buildGraphForC(jenv, jGraph, pGraph) ;

	jenv->DeleteLocalRef(jGraph) ;
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::readGraph")) ;
	return false ;
}
}
*/

//return the admin ou Dpros graph
//input data : id person + arrays (link="HA"; link="HP")
bool
NautilusPilot::readGraphAdmin(string sServiceName, NSDataGraph* pGraph,
																							NSBasicAttributeArray *pAttrArray)
//==============================================================================
{
  if (((NSDataGraph*) NULL == pGraph) || ((NSBasicAttributeArray*) NULL == pAttrArray))
  	return false ;

  JNIEnv* jenv = (JNIEnv*) 0 ;

try
{
	jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
  {
		traceAndDisplayError(string("NautilusPilot::readGraphAdmin : jenv is null. Exiting...")) ;
    return false ;
  }

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if ((jobjectArray) NULL == strArray)
  {
    EndPilotMethod(jenv) ;
  	traceAndDisplayError(string("NautilusPilot::readGraphAdmin : createJObjectArray failed. Exiting...")) ;
		return false ;
  }

	// === execute a NautilusGraph.readGraph method
  //
	jstring jServiceName = WinStringTojstring(jenv, sServiceName) ;

	jobject jGraph = jenv->CallObjectMethod(jNautilusPilot, midSimpInvoke, jServiceName, strArray) ;

	if (((jobject) NULL == jGraph) || JniExceptionCheck(jenv))
	{
    EndPilotMethod(jenv) ;
  	traceAndDisplayError(string("NautilusPilot::readGraphAdmin : CallObjectMethod failed. Exiting...")) ;
		return false ;
  }

	// build a C graph from the java graph object
	//===========================================
  //
  buildGraphForC(jenv, jGraph, pGraph) ;

	jenv->DeleteLocalRef(jGraph) ;

  EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  EndPilotMethod(jenv) ;
  traceAndDisplayError(string("Exception NautilusPilot::readGraphAdmin")) ;
	return false ;
}
}

/* imput data  patient ancien + new patient
                console + instance
    return merge Graph
*/
bool
NautilusPilot::mergePatient(string sServiceName, NSDataGraph* pGraph, NSBasicAttributeArray *pAttrArray)
//=========================================================================================================
{
	if ((NULL == pGraph) || (NULL == pAttrArray))
  	return false ;

	JNIEnv* jenv = StartPilotMethod() ;
	if ((JNIEnv*) NULL == jenv)
	{
		traceAndDisplayError(string("NautilusPilot::mergepatient : jenv is null. Exiting...")) ;
    return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
	if (NULL == strArray)
	{
    EndPilotMethod(jenv) ;
		traceAndDisplayError(string("NautilusPilot::mergepatient : createJObjectArray failed. Exiting...")) ;
		return false ;
	}

	//=== Execute a NautilusGraph.readGraph method
  //
	jstring jServiceName = WinStringTojstring(jenv, sServiceName) ;

	jobject jGraph = jenv->CallObjectMethod(jNautilusPilot, midSimpInvoke, jServiceName, strArray) ;

	if ((NULL == jGraph) && (WARNING_RETURN == getReturnType(jenv))) //WARNING_RETURN
	{
  	warningMessage = getTextWarning(jenv, string("NTIERS")) ;
    EndPilotMethod(jenv) ;
    return false ;
	}
  if ((NULL == jGraph) || JniExceptionCheck(jenv))
	{
    EndPilotMethod(jenv) ;
  	traceAndDisplayError(string("NautilusPilot::mergepatient : method call failed. Exiting...")) ;
    return false ;
	}

	// build a C graph from the java graph object
	//===========================================
	buildGraphForC(jenv, jGraph, pGraph) ;

	jenv->DeleteLocalRef(jGraph) ;

  EndPilotMethod(jenv) ;

	return true ;
}

//return adresse graph
//input data : id from an adresse object  + arrays (link="IN")  + direction
//( FORWORD = 0; BACK = 1; DOUBLE_SENS = 2;)
bool
NautilusPilot::readAdressGraph(string sServiceName, NSDataGraph* pGraph, NSBasicAttributeArray *pAttrArray)
//=========================================================================================================
{
  if ((NULL == pGraph) || (NULL == pAttrArray))
  	return false ;

  JNIEnv* jenv = (JNIEnv*) 0 ;

try
{
	jenv = StartPilotMethod() ;
	if ((JNIEnv*) NULL == jenv)
	{
		traceAndDisplayError(string("NautilusPilot::readAdressGraph : jenv is null. Exiting...")) ;
    return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
	if (NULL == strArray)
	{
    EndPilotMethod(jenv) ;
  	traceAndDisplayError(string("NautilusPilot::readAdressGraph : createJObjectArray failed. Exiting...")) ;
    return false ;
	}

	//=== execute a NautilusGraph.readGraph method
  //
	jstring jServiceName = WinStringTojstring(jenv, sServiceName) ;

	jobject jGraph = jenv->CallObjectMethod(jNautilusPilot, midSimpInvoke, jServiceName, strArray) ;

	if ((NULL == jGraph) || JniExceptionCheck(jenv))
	{
  	traceAndDisplayError(string("NautilusPilot::readAdressGraph : CallObjectMethod failed. Exiting...")) ;

    jenv->DeleteLocalRef(strArray) ;
	  jenv->DeleteLocalRef(jServiceName) ;

    EndPilotMethod(jenv) ;
    return false ;
	}

  jenv->DeleteLocalRef(strArray) ;
	jenv->DeleteLocalRef(jServiceName) ;

	// build a C graph from the java graph object
	//===========================================
	buildGraphForC(jenv, jGraph, pGraph) ;

	jenv->DeleteLocalRef(jGraph) ;

  EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  EndPilotMethod(jenv) ;
  traceAndDisplayError(string("Exception NautilusPilot::readAdressGraph")) ;
	return false ;
}
}

bool NautilusPilot::writeGraph(NSDataGraph* pGraph)
//==================================================
{
  if ((NSDataGraph*) NULL == pGraph)
	{
		traceAndDisplayError(string("NautilusPilot::writeGraph : no graph, exiting")) ;
		return false ;
	}

  JNIEnv* jenv = (JNIEnv*) 0 ;

try
{
	jenv = StartPilotMethod() ;
	if ((JNIEnv*) NULL == jenv)
	{
		traceAndDisplayError(string("NautilusPilot::writeGraph : jenv is null. Exiting...")) ;
    return false ;
	}

	int nbTrees  = 0 ;
	int nbLinks  = 0 ;
	int nbModels = 0 ;
	int nbRights = 0 ;

	jobject jGraph = buildGraphForJava(jenv, pGraph, nbTrees, nbLinks, nbModels, nbRights) ;
	if ((0 == nbTrees) || (NULL == jGraph))
	{
  	jenv->DeleteLocalRef(jGraph) ;
    EndPilotMethod(jenv) ;
    return false ;
	}

	jobjectArray jMappings = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, midWriteGraph, jGraph)) ;

  if ((NULL == jMappings) || JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("NautilusPilot::writeGraph : CallObjectMethod failed. Exiting...")) ;

    jenv->DeleteLocalRef(jGraph) ;
    jenv->DeleteLocalRef(jMappings) ;

    EndPilotMethod(jenv) ;
    return false ;
	}

	jenv->DeleteLocalRef(jGraph) ;

	applyMappings(jenv, jMappings, pGraph, nbTrees, nbLinks, nbModels, nbRights) ;
	jenv->DeleteLocalRef(jMappings) ;

  EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  EndPilotMethod(jenv) ;
  traceAndDisplayError(string("Exception NautilusPilot::writeGraph")) ;
	return false ;
}
}

//used for patient reseach
bool NautilusPilot::invokeService(const char* serviceName, NSDataGraph* pGraph,
                                    NSBasicAttributeArray *pAttrArray, JNIEnv* jenv)
//=====================================================================================
{
try
{
	if (((const char*)             NULL == serviceName) ||
      ((NSDataGraph*)            NULL == pGraph) ||
      ((NSBasicAttributeArray *) NULL == pAttrArray))
  	return false ;

  if (false == _bServiceOnDuty)
  {
	  jenv = StartPilotMethod() ;
	  if ((JNIEnv*) NULL == jenv)
	  {
  	  traceAndDisplayError(string("NautilusPilot::invokeService : jenv is null. Exiting...")) ;
      return false ;
	  }
  }

  // Locate the invokeService method into NautilusPilot.java
  //
  // (NautilusGraph invokeService(String serviceName, String[] data))
  //
  jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeService",
      	    "(Ljava/lang/String;[Ljava/lang/String;)Lnautilus/NautilusGraph;") ;

  // Not found
  //
	if ((0 == mid) || JniExceptionCheck(jenv))
	{
    if (false == _bServiceOnDuty)
      EndPilotMethod(jenv) ;
		traceAndDisplayError(string("Cannot locate the NautilusPilot invokeService method.")) ;
		return false ;
	}

  // Create search traits as Java objects
  //
	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
	if ((jobjectArray) NULL == strArray)
	{
    if (false == _bServiceOnDuty)
      EndPilotMethod(jenv) ;
  	traceAndDisplayError(string("NautilusPilot::invokeService : createJObjectArray failed. Exiting...")) ;
    return false ;
	}

	pGraph->graphReset() ;

	// NautilusGraph = invokeService(Pilot pilot, String serviceName, String[] data)
	//==============================================================================
  //
	jstring jServName = WindowsTojstring(jenv, serviceName) ;

  // Execute the invokeService method from NautilusPilot.java
  //
	jobject jGraph = jenv->CallObjectMethod(jNautilusPilot, mid, jServName, strArray) ;

  jenv->DeleteLocalRef(jServName) ;
  jenv->DeleteLocalRef(strArray) ;

  if ((NULL == jGraph) && (WARNING_RETURN == getReturnType(jenv))) //WARNING_RETURN
	{
    if (false == _bServiceOnDuty)
      EndPilotMethod(jenv) ;
  	warningMessage = getTextWarning(jenv, string("NTIERS")) ;
    return false ;
	}
  if ((0 == mid) || JniExceptionCheck(jenv))
	{
    if (false == _bServiceOnDuty)
      EndPilotMethod(jenv) ;
		traceAndDisplayError(string("Cannot locate the NautilusPilot invokeService method.")) ;
		return false ;
	}
	if (jenv->ExceptionOccurred())
	{
    if (false == _bServiceOnDuty)
      EndPilotMethod(jenv) ;
  	errorMessage = string(getTextError(jenv, "NTIERS", true)) ;
		return false ;
	}

	// build a C graph from the java graph object
	//===========================================
	if (jGraph)
	{
  	buildGraphForC(jenv, jGraph, pGraph) ;
    jenv->DeleteLocalRef(jGraph) ;
	}

  if (false == _bServiceOnDuty)
    EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::invokeService")) ;
	return false ;
}
}

bool NautilusPilot::searchPatient(const char* serviceName, NSDataGraph* pGraph,
                    NSPersonsAttributesArray *pList, NSBasicAttributeArray *pAttrArray)
//=====================================================================================
{
  if (((const char*)                NULL == serviceName) ||
      ((NSDataGraph*)               NULL == pGraph) ||
      ((NSPersonsAttributesArray *) NULL == pList) ||
      ((NSBasicAttributeArray *)    NULL == pAttrArray))
  	return false ;

  JNIEnv* jenv = (JNIEnv*) 0 ;

try
{
	jenv = StartPilotMethod() ;
	if ((JNIEnv*) NULL == jenv)
	{
  	traceAndDisplayError(string("NautilusPilot::searchPatient : jenv is null. Exiting...")) ;
    return false ;
	}

  _bServiceOnDuty = true ;

  bool bServiceResult = invokeService(serviceName, pGraph, pAttrArray, jenv) ;

  _bServiceOnDuty = false ;

	if (false == bServiceResult)
  {
    EndPilotMethod(jenv) ;
		return false ;
  }

	transformResultFieldInAttributesArray(jenv, pList, "status", "*") ;

  EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  EndPilotMethod(jenv) ;
  traceAndDisplayError(string("Exception NautilusPilot::searchPatient")) ;
	return false ;
}
}

bool
NautilusPilot::initAgentList()
{
try
{
  JNIEnv* jenv = StartPilotMethod() ;
	if ((JNIEnv*) NULL == jenv)
	{
  	traceAndDisplayError(string("NautilusPilot::initAgentList : jenv is null. Exiting...")) ;
    return false ;
	}

  NSPersonsAttributesArray AgentsList ;
	resourceList(jenv, NautilusPilot::SERV_GET_AGENT_LIST.c_str(), &AgentsList) ;

  if (AgentsList.empty())
  	return false ;

  NSPersonsAttributeIter iterAgent = AgentsList.begin() ;
  for ( ; AgentsList.end() != iterAgent ; iterAgent++)
  {
  	string agentName = string((*iterAgent)->getAttributeValue("name")) ;
			//string err = string((*iter)->getAttributeValue("textError")) ;
    NSAgentStatus *pAgent = new NSAgentStatus(agentName, "") ;

    string sProp = string((*iterAgent)->getAttributeValue("properties"))  ;
    if (string("") != sProp)
    	pAgent->setAgentProperties(sProp) ;
    VecteurString *pServList = new VecteurString() ;
    //getServiceForAgent(agentName.c_str(), pServList ) ;
    pAgent->setServiceList(pServList)  ;

    _AvailableAgentList.push_back(pAgent) ;
	}

	markIncorrectAgents(jenv) ;

  EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::initAgentList")) ;
	return false ;
}
}

bool
NautilusPilot:: markIncorrectAgents(JNIEnv* jenv)
//===================================
{
  if ((JNIEnv*) NULL == jenv)
		return false ;

try
{
	NSPersonsAttributesArray ErrorList ;
	bool res = agentsTest(jenv, SERV_AGENTS_TEST.c_str(), &ErrorList) ;
	if (false == res)
		return false ;

  if (ErrorList.empty())
    return true ;

  for (NSPersonsAttributeIter iter = ErrorList.begin() ; ErrorList.end() != iter ; iter++)
  {
    string sAgentName = string((*iter)->getAttributeValue("agent")) ;
    NSAgentStatus* pAgent = _AvailableAgentList.selectAgent(sAgentName) ;

    if (pAgent)
    {
      string err = string((*iter)->getAttributeValue("textError")) ;
      pAgent->setError(err) ;
      pAgent->setInactive() ;
    }
  }

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::markIncorrectAgents")) ;
	return false ;
}
}

// utilisé pour les tests des agents
bool
NautilusPilot::agentsTest(JNIEnv* jenv, const char* serviceName, NSPersonsAttributesArray *pErrorList)
//======================================================================================
{
	if ((NULL == serviceName) || ((JNIEnv*) NULL == jenv))
		return false ;
try
{
  if (_AvailableAgentList.empty())
  	return true ;

  NSBasicAttributeArray AttrArray ;

  for (NSAgentStatusIter iterAgent = _AvailableAgentList.begin() ; _AvailableAgentList.end() != iterAgent ; iterAgent++)
  	AttrArray.push_back(new NSBasicAttribute((*iterAgent)->getAgentName(), string("true"))) ;

  jobjectArray strArray = createJObjectArray(jenv, &AttrArray) ;
  if (NULL == strArray)
  {
  	fprintf(stderr, "NautilusPilot::invokeService : createJObjectArray failed. Exiting...");
    return false ;
  }

	jmethodID midAg = jenv->GetMethodID(classNautilusPilot, "agentsTest",
                    "(Ljava/lang/String;[Ljava/lang/String;)[Ldomlightwrapper/DOMElement;");
  if ((0 == midAg) || jenv->ExceptionCheck())
	{
  	string ps2 = string("NautilusPilot::agentsTest method. Exiting...") ;
		_pSuper->trace(&ps2, 1, NSSuper::trError) ;
		return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jobjectArray jResultTrees = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, midAg, jServName, strArray)) ;
  jenv->DeleteLocalRef(jServName) ;
  jenv->DeleteLocalRef(strArray) ;

	if (jenv->ExceptionOccurred())
	{
  	string sErrorText = getTextError(jenv) ;
		erreur(sErrorText.c_str(), standardError) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		jenv->ExceptionDescribe() ;
		return false ;
	}
	if (NULL == jResultTrees)
		return true ;

	int count = jenv->GetArrayLength(jResultTrees) ;
	for (int i = 0 ; i < count ; i++)
	{
		jobject jErrorElem = jenv->GetObjectArrayElement(jResultTrees, i) ;
    if (NULL != jErrorElem)
    {
    	NSBasicAttributeArray* pErrorAttributes = new NSBasicAttributeArray() ;
      DOMTreeToBasicAttribute(jenv, pErrorAttributes, jErrorElem) ;
      jenv->DeleteLocalRef(jErrorElem) ;
      if (NULL != pErrorList)
      	pErrorList->push_back(pErrorAttributes) ;
    }
    else
    {
    	string ps = string("NautilusPilot::errorList : GetObjectArrayElement failed.") ;
      _pSuper->trace(&ps, 1, NSSuper::trError) ;
    }
  }
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::agentsTest")) ;
	return false ;
}
}

bool
NautilusPilot::testServiceStatus(string sServiceName)
{
	VecteurString *pAgents = _PropertiesServicesList.getAgentListForService(sServiceName) ;
  if ((NULL == pAgents) || (pAgents->empty()))
		return true ;

	for (EquiItemIter iterAgent = pAgents->begin() ; pAgents->end() != iterAgent ; iterAgent++)
		if (_AvailableAgentList.selectAgent(**iterAgent)->isInactive())
    	return false ;

	return true ;
}

bool
NautilusPilot::traitError(string sServiceName)
{
  VecteurString *pAgents = _PropertiesServicesList.getAgentListForService(sServiceName) ;

  string sError = string("") ;
  if ((NULL == pAgents) || (pAgents->empty()))
  	sError = string("nonInstancedWarning") ;
  else
  {
		for (EquiItemIter iterAgent= pAgents->begin() ; pAgents->end() != iterAgent ; iterAgent++)
  	{
    	NSAgentStatus *pAgent = _AvailableAgentList.selectAgent(**iterAgent) ;
  		if (pAgent && (pAgent->isInactive()))
  		{
    		string sErr = _AvailableAgentList.selectAgent(**iterAgent)->getError() ;
      	string sErrTxt = _pSuper->getText("JavaException", _AvailableAgentList.selectAgent(**iterAgent)->getError()) ;
      	if (NULL == strstr(sError.c_str(), sErrTxt.c_str()))
      		sError = sError + "\n" + sErrTxt ;
  		}
  	}
	}
  sError = sError + "\n " + _pSuper->getText("JavaException", "msgContinue") ;
 	int returnVal = MessageBox(0, sError.c_str(), "Message Nautilus", MB_YESNO) ;
  if (IDYES == returnVal)
  	return true ;

  return false ;
}

bool
NautilusPilot::traitError(int agentType)
{
  //VecteurString *pAgents = pPropertiesServicesList->getAgentListForService( sServiceName );
  string sError = "" ;
  if (_AvailableAgentList.empty())
  	sError = "nonInstancedWarning" ;
	else
  {
		for (NSAgentStatusIter iterAgent = _AvailableAgentList.begin() ; _AvailableAgentList.end() != iterAgent ; iterAgent++)
  	{
   		if ((*iterAgent) && ((*iterAgent)->isInactive()) && ((*iterAgent)->getAgentType() == agentType))
  		{
    		string sErr = (*iterAgent)->getError() ;
      	string sErrTxt = _pSuper->getText("JavaException", (*iterAgent)->getError()) ;
      	if (NULL == strstr(sError.c_str(), sErrTxt.c_str()))
      		sError = sError + "\n" + sErrTxt ;
  		}
  	}
	}
  if (string("") == sError)
  	sError = _pSuper->getText("JavaException", "versionProblem") ;

	sError = sError + "\n " +  _pSuper->getText("JavaException", "msgContinue") ;
  int returnVal = MessageBox(0, sError.c_str(), "Message Nautilus", MB_YESNO) ;
  if (IDYES == returnVal)
  	return true ;

  return false ;
  //erreur(sError.c_str(), standardError, 0) ;
}

bool
NautilusPilot::isOperationalLdVAgent()
{
	if (_AvailableAgentList.empty())
		return false ;

  bool foundAgent = false ;

  for (NSAgentStatusIter iterAgent = _AvailableAgentList.begin() ; _AvailableAgentList.end() != iterAgent ; iterAgent++)
  {
   /*	if( ((*iterAgent)->isLdvTypeAgent() ) && ((*iterAgent)->isInactive()))
    	return false ;  */
		if ((*iterAgent)->isLdvTypeAgent())
    {
    	foundAgent = true ;
      if ((*iterAgent)->isInactive())
      	return false ;
    }
  }

	return foundAgent ;
}

bool
NautilusPilot::isOperationalLocalAgent()
{
	if (_AvailableAgentList.empty())
		return false ;

	for (NSAgentStatusIter iterAgent = _AvailableAgentList.begin() ; _AvailableAgentList.end() != iterAgent ; iterAgent++)
  	if ((*iterAgent)->isLocalTypeAgent() && (*iterAgent)->isInactive())
    	return false ;

	return true ;
}

//utilisé pour les tests des agents
bool
NautilusPilot::getServiceForAgent(const char* agentName, VecteurString *pServList)
//=================================================================================
{
	if (NULL == agentName)
		return false ;
try
{
	JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
	{
  	string ps2 = string("NautilusPilot::agentsTest : jenv is null. Exiting...") ;
		_pSuper->trace(&ps2, 1, NSSuper::trError) ;
    return false ;
	}

	jmethodID midAg = jenv->GetMethodID(classNautilusPilot, "getServiceList",
                    "(Ljava/lang/String;)[Ljava/lang/String;");
	if ((0 == midAg) || jenv->ExceptionCheck())
	{
  	string ps2 = string("NautilusPilot::getServiceList method. Exiting...") ;
		_pSuper->trace(&ps2, 1, NSSuper::trError) ;
		return false ;
	}

	jstring jAgentName = WindowsTojstring(jenv, agentName) ;
	jobjectArray jArrayString = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, midAg, jAgentName)) ;
  jenv->DeleteLocalRef(jAgentName) ;
  if (jenv->ExceptionOccurred())
	{
  	string sErrorText = getTextError(jenv) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		jenv->ExceptionDescribe() ;
  	return false ;
  }

	if ((NULL == jArrayString) && (WARNING_RETURN == getReturnType(jenv)))
  {
  	jstring jWarning = (jstring)jenv->GetObjectField(jNautilusPilot, jFidWarning) ;
    if (NULL != jWarning)
    {
    	// warnings
      string sWarning = string(JavaSystem::NewStringWithSpecialCharacters(jWarning)) ;
      if (string("serviceNotFound") == sWarning)
      {
      	jenv->DeleteLocalRef(jArrayString) ;
        return true ;
      }
  	}
  }

	jint jsize = jenv->GetArrayLength(jArrayString) ;
	for (int i = 0 ; i < jsize ; i++)
	{
  	jstring jElem = (jstring)jenv->GetObjectArrayElement(jArrayString, i) ;
    if (NULL != jElem)
    {
      if (pServList)
    	  pServList->AddString(JavaSystem::NewStringWithSpecialCharacters(jElem)) ;
      jenv->DeleteLocalRef(jElem) ;
    }
    else
    {
    	string ps2 = string("NautilusPilot::invokeService(char*, char*, VecteurString&) : GetObjectArrayElement failed.") ;
      _pSuper->trace(&ps2, 1, NSSuper::trError) ;
    }
	}
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getServiceForAgent")) ;
	return false ;
}
}

//utilisé pour la liste des utilisateurs
bool NautilusPilot::invokeService(const char* serviceName, const char* tagName, VecteurString& result)
//====================================================================================================
{
	if (((const char*) NULL == serviceName) || ((const char*) NULL == tagName))
		return false ;

  /*  if(!pAvailableAgentList->isAvailableService(string(serviceName)))
        return false ;     */
try
{
	JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
	{
		string ps2 = string("NautilusPilot::searchPatient : jenv is null. Exiting...") ;
		_pSuper->trace(&ps2, 1, NSSuper::trError) ;
		return false ;
	}

	//=== get method id for the NautilusPilot.invokeService method
	jmethodID mid2Invoke = jenv->GetMethodID(classNautilusPilot, "invokeService",
                    "(Ljava/lang/String;Ljava/lang/String;)[Ljava/lang/String;") ;
	if ((0 == mid2Invoke) || jenv->ExceptionCheck())
	{
  	string ps2 = string("NautilusPilot::invokeService(char*, char*, VecteurString&) : Cannot locate the NautilusPilot invokeService method. Exiting...") ;
		_pSuper->trace(&ps2, 1, NSSuper::trError) ;
		return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jstring jTagName  = WindowsTojstring(jenv, tagName) ;

	jobjectArray jArrayString = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, mid2Invoke, jServName, jTagName)) ;

	jenv->DeleteLocalRef(jServName) ;
	jenv->DeleteLocalRef(jTagName) ;

	if (jenv->ExceptionOccurred())
	{
		// erreur("Erreur execution : invoke service",standardError, 0,0);
    string sErrorText = getTextError(jenv) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		jenv->ExceptionDescribe() ;
		return false ;
	}

	if (jArrayString)
	{
		jint jsize = jenv->GetArrayLength(jArrayString) ;
		for (int i = 0 ; i < jsize ; i++)
		{
			jstring jElem = (jstring)jenv->GetObjectArrayElement(jArrayString, i) ;
      if (jElem)
      {
      	result.AddString(JavaSystem::NewStringWithSpecialCharacters(jElem)) ;
        jenv->DeleteLocalRef(jElem) ;
      }
      else
      {
      	string ps2 = string("NautilusPilot::invokeService(char*, char*, VecteurString&) : GetObjectArrayElement failed.") ;
        _pSuper->trace(&ps2, 1, NSSuper::trError) ;
      }
    }
	}
	jenv->DeleteLocalRef(jArrayString) ;
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::invokeService")) ;
	return false ;
}
}

//used for userList
bool
NautilusPilot::personsList(const char* serviceName, string resultTagName, NSPersonsAttributesArray *pAttsList, char* first, ...)
//==============================================================================================================================
{
	if ((NULL == serviceName) || (NULL == pAttsList))
  	return false ;

try
{
  JNIEnv* jenv = StartPilotMethod() ;
	if ((JNIEnv*) NULL == jenv)
	{
  	string ps2 = string("NautilusPilot::personsList : jenv is null. Exiting...") ;
		traceAndDisplayError(ps2) ;
    return false ;
	}

	jobjectArray strArray = NULL ;
//	char debug[512];
	if (NULL != first)
	{
		va_list marker ;

		//=== first loop, we compute size
		int size = 0 ;
		va_start(marker, first) ;
		while( true )
		{
			size++ ;
			if( !va_arg( marker, char*) )
				break ;
		}
		va_end(marker) ;

		//jclass classStr = jenv->FindClass("java/lang/String");
		strArray = jenv->NewObjectArray(size, classStr, NULL) ;
    if ((NULL == strArray) || jenv->ExceptionCheck())
		{
      EndPilotMethod(jenv) ;
			string ps2 = string("NautilusPilot::personsList : NewObjectArray failed. Exiting...") ;
			_pSuper->trace(&ps2, 1, NSSuper::trError) ;
			return false ;
		}

		//=== second loop, we build a Java String Array with the strings
		char* data = new char[80] ;
		strcpy(data, first) ;
		va_start(marker, first) ;
		for (int i = 0; i < size; i++)
		{
			jstring jstr = WindowsTojstring(jenv, data) ;
			jenv->SetObjectArrayElement(strArray, i, jstr) ;
      if (jenv->ExceptionOccurred())
			{
				// erreur("Erreur execution : invoke service",standardError, 0,0);
        string sErrorText = getTextError(jenv) ;
        _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
				jenv->ExceptionDescribe() ;
        EndPilotMethod(jenv) ;
        delete[] data ;
				return false ;
			}

			jenv->DeleteLocalRef(jstr) ;

			data = va_arg( marker, char*) ;
		}
    delete[] data ;

		va_end(marker) ;
	}

	jmethodID mid2Invoke = jenv->GetMethodID(classNautilusPilot, "invokeService2String",
                    "(Ljava/lang/String;Ljava/lang/String;)Ldomlightwrapper/DOMElement;") ;

	if ((NULL == mid2Invoke) || jenv->ExceptionCheck())
	{
    EndPilotMethod(jenv) ;
  	string ps2 = string("NautilusPilot::personsList : Cannot locate the NautilusPilot invokeService2String method. Exiting...") ;
    _pSuper->trace(&ps2, 1, NSSuper::trError) ;
		return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jstring jTagName  = WinStringTojstring(jenv, resultTagName) ;

	jobject jDOMElem = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, mid2Invoke, jServName, jTagName)) ;

	if (jenv->ExceptionOccurred())
	{
		//erreur("Erreur execution : invoke service personsList",standardError,0, 0);
    string sErrorText = getTextError(jenv) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		jenv->ExceptionDescribe() ;
    EndPilotMethod(jenv) ;
		return false ;
	}

	jenv->DeleteLocalRef(jServName) ;
	jenv->DeleteLocalRef(jTagName) ;
	if (NULL == jDOMElem)
	{
		string sErrorText = getTextError(jenv) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
    EndPilotMethod(jenv) ;
		return false ;
	}

	jstring jNodeName = WinStringTojstring(jenv, resultTagName) ;

	jobjectArray jDomElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jDOMElem, midChildrenElements, jNodeName)) ;
	jenv->DeleteLocalRef(jNodeName) ;
	if (NULL == jDomElems)
	{
		string ps2 = string("NautilusPilot::personsList : CallObjectMethod failed. Exiting...") ;
		_pSuper->trace(&ps2, 1, NSSuper::trError) ;
    EndPilotMethod(jenv) ;
		return false ;
	}

	int count = jenv->GetArrayLength(jDomElems) ;
	for (int i = 0; i < count; i++)
	{
  	jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
		if (NULL != jNodeElem)
		{
    	NSBasicAttributeArray* pElemAttributes = new NSBasicAttributeArray() ;
      DOMTreeToBasicAttribute(jenv, pElemAttributes, jNodeElem) ;
      pAttsList->push_back(pElemAttributes) ;
      jenv->DeleteLocalRef(jNodeElem) ;
    }
		else
    {
    	string ps2 = string("NautilusPilot::personsList : GetObjectArrayElement failed.") ;
			_pSuper->trace(&ps2, 1, NSSuper::trError) ;
    }
  }

	jenv->DeleteLocalRef(jDomElems) ;

  EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::personsList")) ;
	return false ;
}
}

jobject
NautilusPilot::CallObjectMethod(jobject obj, jmethodID methodID, ...)
//===================================================================================
{
try
{
  JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
    return (jobject) 0 ;

  va_list argPtr ;
  va_start(argPtr, methodID) ;

  jobject resObj = jenv->CallObjectMethodV(obj, methodID, argPtr) ;

  if (false == ReleaseJniEnv(jenv))
    return (jobject) 0 ;

  return resObj ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::CallObjectMethod")) ;
	return (jobject) 0 ;
}
}

jboolean
NautilusPilot::CallBooleanMethod(jobject obj, jmethodID methodID, ...)
//===================================================================================
{
try
{
  JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
    return JNI_FALSE ;

  va_list argPtr ;
  va_start(argPtr, methodID) ;

  jboolean bResult = jenv->CallBooleanMethodV(obj, methodID, argPtr) ;

  if (false == ReleaseJniEnv(jenv))
    return JNI_FALSE ;

  return bResult ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::CallBooleanMethod")) ;
	return JNI_FALSE ;
}
}

void
NautilusPilot::CallVoidMethod(jobject obj, jmethodID methodID, ...)
//===================================================================================
{
try
{
  JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
    return ;

  va_list argPtr ;
  va_start(argPtr, methodID) ;

  jenv->CallVoidMethodV(obj, methodID, argPtr) ;

  ReleaseJniEnv(jenv) ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::CallVoidMethod")) ;
}
}

void NautilusPilot::buildGraphForC(JNIEnv* jenv, jobject jGraph, NSDataGraph* pGraph)
//===================================================================================
{
	if ((NSDataGraph *) NULL == pGraph)
  	return ;

try
{
	pGraph->getTrees()->vider() ;
	pGraph->getLinks()->vider() ;
	pGraph->getArchs()->vider() ;
	pGraph->getRights()->vider() ;

	if (((JNIEnv*) NULL == jenv) || ((jobject) NULL == jGraph))
		return ;

	int nbTrees  = jni_getNbTrees(jenv, jGraph) ;
	int nbLinks  = jni_getNbLinks(jenv, jGraph) ;
	int nbModels = jni_getNbModels(jenv, jGraph) ;
	int nbRights = jni_getNbRights(jenv, jGraph) ;

	// char* result = NULL ;

	//=== create C++ models
	//=====================
	for (int i = 0 ; i < nbModels ; i++)
	{
		NSArcLink* pmodel = new NSArcLink(_pSuper) ;

    pmodel->setObject(jni_getAttribute(jenv, jGraph, "getModelObject", i)) ;
    pmodel->setNode(jni_getAttribute(jenv,   jGraph, "getModelNode", i)) ;
    pmodel->setType(jni_getAttribute(jenv,   jGraph, "getModelType", i)) ;
    pmodel->setMOId(jni_getAttribute(jenv,   jGraph, "getModelModelObject", i)) ;

		pGraph->getArchs()->push_back(pmodel) ;
	}

	//=== create C++ rights
	//=====================
	for (int i = 0 ; i < nbRights ; i++)
	{
  	NSNodeRight* pRight = new NSNodeRight() ;

    pRight->setNode(jni_getAttribute(jenv, jGraph, "getRightNode", i)) ;
    pRight->setRight(jni_getAttribute(jenv, jGraph, "getRightRight", i)) ;

		pGraph->getRights()->push_back(pRight) ;
	}

	//------------------------------------------------------------------------------
	//=== create C++ links
	//====================
	for (int i = 0 ; i < nbLinks ; i++)
	{
  	NSPatLinkInfo* plink = new NSPatLinkInfo() ;

    plink->setQualifie(jni_getAttribute(jenv, jGraph, "getLinkQualified", i)) ;
    plink->setQualifiant(jni_getAttribute(jenv, jGraph, "getLinkQualifier", i)) ;
    plink->setLien(jni_getAttribute(jenv, jGraph, "getLinkLink", i)) ;

		pGraph->getLinks()->push_back(plink) ;
	}

	string treeId = jni_getGraphId(jenv, jGraph) ;
	pGraph->setGraphID(treeId) ;

	//=== create C++ trees
	//====================
	for (int i = 0 ; i < nbTrees ; i++)
	{
		treeId = jni_getTreeId(jenv, jGraph, i) ;

		NSDataTree* pDataTree = new NSDataTree(_pSuper, treeId, graphPerson) ;

    // To optimise this step, we fill pDataTree from inside instead of building
    // a patpatho and use setPatPatho
    //
    NSPatPathoArray* pPatPathoArray = pDataTree->getPatPathoPointer() ;

		// NSPatPathoArray PatPathoArray(_pSuper) ;

		jobjectArray jnodes = jni_getTreeNodes(jenv, jGraph, i) ;
		if ((jobjectArray) NULL == jnodes)
    {
			traceAndDisplayError(string("NautilusPilot::buildGraphForC : jni_getTreeNodes failed. exiting...")) ;
			return ;
    }

		jsize len = jenv->GetArrayLength(jnodes) ;
		//loop on the nodes of a tree and fill pDataTree->pPathPathoArray
		for (int j = 0 ; j < len ; j++)
		{
			jobject jnode = jenv->GetObjectArrayElement(jnodes, j) ;

			if (jnode)
			{
				NSPatPathoInfo* pPatPathoInfo = new NSPatPathoInfo() ;
				pPatPathoInfo->setTreeID(treeId) ;

        pPatPathoInfo->setNodeID(jni_getNodeField(jenv,       "NODE",         jnode)) ;
        pPatPathoInfo->setLexique(jni_getNodeField(jenv,      "LEXICON",      jnode)) ;
        pPatPathoInfo->setComplement(jni_getNodeField(jenv,   "COMPLEMENT",   jnode)) ;
        pPatPathoInfo->setUnit(jni_getNodeField(jenv,         "UNIT",         jnode)) ;
        pPatPathoInfo->setCertitude(jni_getNodeField(jenv,    "CERTITUDE",    jnode)) ;
        pPatPathoInfo->setInteret(jni_getNodeField(jenv,      "INTEREST",     jnode)) ;
        pPatPathoInfo->setPluriel(jni_getNodeField(jenv,      "PLURAL",       jnode)) ;
        pPatPathoInfo->setVisible(jni_getNodeField(jenv,      "VISIBLE",      jnode)) ;
        pPatPathoInfo->setType(jni_getNodeField(jenv,         "TYPE",         jnode)) ;
        pPatPathoInfo->setTexteLibre(jni_getNodeField(jenv,   "FREE_TEXT",    jnode)) ;
        pPatPathoInfo->setLocalisation(jni_getNodeField(jenv, "LOCALISATION", jnode)) ;

				jenv->DeleteLocalRef(jnode) ;

				// PatPathoArray.ajouteElement(pPatPathoInfo) ;
        pPatPathoArray->ajouteElement(pPatPathoInfo) ;
      }
    }
		jenv->DeleteLocalRef(jnodes) ;

    // Better do it now and not take the risk it is never done
    //
    pDataTree->sortPPTByLocalisation() ;

		// pDataTree->setPatPatho(&PatPathoArray, true) ;

		pGraph->getTrees()->push_back(pDataTree) ;
	}
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::buildGraphForC")) ;
}
}

jobject
NautilusPilot::buildGraphForJava(JNIEnv* jenv, NSDataGraph* pGraph,
                    int& nbTrees, int& nbLinks, int&  nbModels, int& nbRights)
//=============================================================================
{
	if (((NSDataGraph*) NULL == pGraph) || ((JNIEnv*) NULL == jenv))
  	return (jobject) NULL ;

try
{
	jstring jRootId = WinStringTojstring(jenv, pGraph->getGraphID()) ;
	nbTrees  = pGraph->getTrees()->size() ;
	nbLinks  = pGraph->getLinks()->size() ;
	nbModels = pGraph->getArchs()->size() ;
	nbRights = pGraph->getRights()->size() ;

	//dom replace graphPerson by pGraph->getType !!
	jobject jGraph = jenv->NewObject(classNautilusGraph, midNewGraph,
                          graphPerson, jRootId, nbTrees, nbLinks, nbModels, nbRights) ;
  if (JniExceptionCheck(jenv) || (NULL == jGraph))
  {
  	string ps = string("NautilusPilot::buildGraphForJava : NewObject failed. exiting...");
		_pSuper->trace(&ps, 1, NSSuper::trError) ;
    return (jobject) 0 ;
  }
	int i ;

	//=== build a graph for the java side
	//======================================
	//loop on the trees (pGraph->aTrees)
	//==================================
	if (nbTrees > 0)
	{
		//=== get method id for the NautilusGraph.createLink method
	  jmethodID midT = jenv->GetMethodID(classNautilusGraph, "createTree",
                        "(ILjava/lang/String;I)V");
    if (JniExceptionCheck(jenv) || (NULL == midT))
  	{
  		string ps = string("NautilusPilot::buildGraphForJava : GetMethodID failed for createTree. exiting...");
			_pSuper->trace(&ps, 1, NSSuper::trError) ;
      return (jobject) 0 ;
  	}

		jmethodID midN = jenv->GetMethodID(classNautilusGraph, "createNode",
                        "(IILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V") ;
		if (JniExceptionCheck(jenv) || (NULL == midN))
  	{
  		string ps = string("NautilusPilot::buildGraphForJava : GetMethodID failed for createNode. exiting...");
			_pSuper->trace(&ps, 1, NSSuper::trError) ;
      return (jobject) 0 ;
  	}

		NSDataTreeIter treeIter = pGraph->getTrees()->begin() ;
		i = 0 ;
		while (pGraph->getTrees()->end() != treeIter)     //loop on the trees
    {                                            //=================
			NSDataTree* treeInfo = *treeIter ;

			jstring jTreeId = WinStringTojstring(jenv, treeInfo->getTreeID()) ;
			int     nbNodes = treeInfo->size() ;

			jenv->CallVoidMethod(jGraph, midT, i, jTreeId, nbNodes) ;
      if (nbNodes > 0)
			{
				NSPatPathoArray PatPathoArray(_pSuper) ;
				treeInfo->getRawPatPatho(&PatPathoArray, true) ;
				if (PatPathoArray.empty())
        {
        	string ps = string("NautilusPilot::buildGraphForJava : Empty patpatho. exiting...") ;
					_pSuper->trace(&ps, 1, NSSuper::trError) ;
        	return (jobject) 0 ;
        }
        PatPathoIter patIter = PatPathoArray.begin() ;
        int j = 0 ;
        while (PatPathoArray.end() != patIter)  //loop on the nodes
        {                                          //=================
        	NSPatPathoInfo* patPathoInfo = *patIter ;

/* No longer works for Java 1.6 ("£SPID1" becomes "£SPID")
          jstring jNodeId       = jenv->NewStringUTF( patPathoInfo->getNodeID().c_str() ) ;
          jstring jType         = jenv->NewStringUTF( patPathoInfo->getType().c_str() ) ;
          jstring jLexique      = jenv->NewStringUTF( patPathoInfo->getLexique().c_str() ) ;
          jstring jComplement   = jenv->NewStringUTF( patPathoInfo->getComplement().c_str() ) ;
          jstring jCertitude    = jenv->NewStringUTF( patPathoInfo->getCertitude().c_str() ) ;
          jstring jPluriel      = jenv->NewStringUTF( patPathoInfo->getPluriel().c_str() ) ;
          jstring jUnit         = jenv->NewStringUTF( patPathoInfo->getUnit().c_str() ) ;
          jstring jFreeText     = jenv->NewStringUTF( patPathoInfo->getTexteLibre().c_str() ) ;
          jstring jLocalisation = jenv->NewStringUTF( patPathoInfo->getLocalisation().c_str() ) ;
          jstring jVisible      = jenv->NewStringUTF( patPathoInfo->getVisible().c_str() ) ;
          jstring jInteret      = jenv->NewStringUTF( patPathoInfo->getInteret().c_str() ) ;
*/

          jstring jNodeId       = WinStringTojstring(jenv, patPathoInfo->getNodeID()) ;
          jstring jType         = WinStringTojstring(jenv, patPathoInfo->getType()) ;
          jstring jLexique      = WinStringTojstring(jenv, patPathoInfo->getLexique()) ;
          jstring jComplement   = WinStringTojstring(jenv, patPathoInfo->getComplement()) ;
          jstring jCertitude    = WinStringTojstring(jenv, patPathoInfo->getCertitude()) ;
          jstring jPluriel      = WinStringTojstring(jenv, patPathoInfo->getPluriel()) ;
          jstring jUnit         = WinStringTojstring(jenv, patPathoInfo->getUnit()) ;
          jstring jFreeText     = WinStringTojstring(jenv, patPathoInfo->getTexteLibre()) ;
          jstring jLocalisation = WinStringTojstring(jenv, patPathoInfo->getLocalisation()) ;
          jstring jVisible      = WinStringTojstring(jenv, patPathoInfo->getVisible()) ;
          jstring jInteret      = WinStringTojstring(jenv, patPathoInfo->getInteret()) ;

          jenv->CallVoidMethod(jGraph, midN, i, j, jTreeId, jNodeId,
                               jType, jLexique, jComplement, jCertitude,
                               jPluriel, jUnit, jFreeText, jLocalisation,
                               jVisible, jInteret) ;

          jenv->DeleteLocalRef(jNodeId) ;
          jenv->DeleteLocalRef(jType) ;
          jenv->DeleteLocalRef(jLexique) ;
          jenv->DeleteLocalRef(jComplement) ;
          jenv->DeleteLocalRef(jCertitude) ;
          jenv->DeleteLocalRef(jPluriel) ;
          jenv->DeleteLocalRef(jUnit) ;
          jenv->DeleteLocalRef(jFreeText) ;
          jenv->DeleteLocalRef(jLocalisation) ;
          jenv->DeleteLocalRef(jVisible) ;
          jenv->DeleteLocalRef(jInteret) ;

          patIter++ ;
          j++ ;
        }     //end of while
      }       //end of if( nbNodes > 0 )

      jenv->DeleteLocalRef(jTreeId) ;

      treeIter++ ;
      i++ ;
    }
  }

  //loop on the links (pGraph->aLinks)
  //==================================
  if (nbLinks > 0)
  {
  	//=== get method id for the NautilusGraph.createLink method
	  jmethodID mid = jenv->GetMethodID(classNautilusGraph, "createLink",
      "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V") ;
        //dom  "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;[LgraphServer/PointOfView;)V") ;
    if (JniExceptionCheck(jenv) || (NULL == mid))
  	{
  		string ps = string("NautilusPilot::buildGraphForJava : GetMethodID failed for createLink. exiting...");
			_pSuper->trace(&ps, 1, NSSuper::trError) ;
      return (jobject) 0 ;
  	}

    jmethodID midPoint = jenv->GetMethodID(classNautilusGraph, "createPointOfView",
                "(IILjava/lang/String;Ljava/lang/String;)V");
    if (JniExceptionCheck(jenv) || (NULL == midPoint))
  	{
  		string ps = string("NautilusPilot::buildGraphForJava : GetMethodID failed for createPointOfView. exiting...");
			_pSuper->trace(&ps, 1, NSSuper::trError) ;
      return (jobject) 0 ;
  	}

    if (false == pGraph->getLinks()->empty())
    {
    	NSPatLinkIter linkIter = pGraph->getLinks()->begin() ;
      i = 0 ;
      while (pGraph->getLinks()->end() != linkIter)
      {
      	NSPatLinkInfo* linkInfo = *linkIter ;

        jstring jQualifie   = WinStringTojstring(jenv, linkInfo->getQualifie()) ;
        jstring jLien       = WinStringTojstring(jenv, linkInfo->getLien()) ;
        jstring jQualifiant = WinStringTojstring(jenv, linkInfo->getQualifiant()) ;

    //dom    jenv->CallVoidMethod(jGraph, mid, i, jQualifie, jLien, jQualifiant, jPoint);
        jenv->CallVoidMethod(jGraph, mid, i, jQualifie, jLien, jQualifiant) ;

        jenv->DeleteLocalRef(jQualifie) ;
        jenv->DeleteLocalRef(jLien) ;
        jenv->DeleteLocalRef(jQualifiant) ;

        jstring jempty = WindowsTojstring(jenv, "") ;
        int j = 0 ;    //in the future j can be incremented to add a point of view
        jenv->CallVoidMethod(jGraph, midPoint, i, j, jempty, jempty) ;

        jenv->DeleteLocalRef(jempty) ;

        /*
        jobject jPoint = jenv->NewObject(classNautilusGraph, midPoint, i, j, "", "");
        if (jPoint == NULL)
  			{
  				ps = string("NautilusPilot::buildGraphForJava : NewObject failed. exiting...");
					pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
          return NULL ;
  			}
        jenv->CallVoidMethod(jGraph, mid, i, jQualifie, jLien, jQualifiant, jPoint);
        */

    //dom    jenv->DeleteLocalRef(jPoint);

        linkIter++ ;
        i++ ;
      }
    }
  }

	//loop on the models (pGraph->aArchs)
	//===================================
	if (nbModels > 0)
	{
		//=== get method id for the NautilusGraph.createModel method
	  jmethodID mid = jenv->GetMethodID(classNautilusGraph, "createModel",
        "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V") ;
    if (JniExceptionCheck(jenv) || (NULL == mid))
  	{
  		string ps = string("NautilusPilot::buildGraphForJava : GetMethodID failed for createModel. exiting...") ;
			_pSuper->trace(&ps, 1, NSSuper::trError) ;
      return (jobject) 0 ;
  	}

		if (false == pGraph->getArchs()->empty())
		{
    	NSArcLinkIter modelIter = pGraph->getArchs()->begin() ;
      i = 0 ;
      while (pGraph->getArchs()->end() != modelIter)
      {
      	NSArcLink* modelInfo = *modelIter ;
        //manage object, node, type, name
        jstring jObject      = WinStringTojstring(jenv, modelInfo->getObject()) ;
        jstring jNode        = WinStringTojstring(jenv, modelInfo->getNode()) ;
        jstring jType        = WinStringTojstring(jenv, modelInfo->getType()) ;
        jstring jModelObject = WinStringTojstring(jenv, modelInfo->getMOId()) ;

        jenv->CallVoidMethod(jGraph, mid, i, jObject, jNode, jType, jModelObject) ;

        jenv->DeleteLocalRef(jObject) ;
        jenv->DeleteLocalRef(jNode) ;
        jenv->DeleteLocalRef(jType) ;
        jenv->DeleteLocalRef(jModelObject) ;

        modelIter++ ;
        i++ ;
      }
    }
  }

   //------------------------------------------------------------------
  //loop on the rights (pGraph->aRights)
  //==================================
  if (nbRights > 0)
  {
		//=== get method id for the NautilusGraph.createLink method
	  jmethodID mid = jenv->GetMethodID(classNautilusGraph, "createRight",
                                  "(ILjava/lang/String;Ljava/lang/String;)V") ;
    if (JniExceptionCheck(jenv) || (NULL == mid))
  	{
  		string ps = string("NautilusPilot::buildGraphForJava : GetMethodID failed for createRight. exiting...") ;
			_pSuper->trace(&ps, 1, NSSuper::trError) ;
      return (jobject) 0 ;
  	}

    if (false == pGraph->getRights()->empty())
    {
    	NSNodeRightIter rightIter = pGraph->getRights()->begin() ;
      i = 0 ;
			while (pGraph->getRights()->end() != rightIter)
      {
      	NSNodeRight* rightInfo = *rightIter ;

        jstring jNode  = WinStringTojstring(jenv, rightInfo->getNode()) ;
        jstring jRight = WinStringTojstring(jenv, rightInfo->getRight()) ;
        // jsize str_size = jenv->GetStringUTFLength(jRight) ;

        jenv->CallVoidMethod(jGraph, mid, i, jNode, jRight) ;

        jenv->DeleteLocalRef(jNode) ;
        jenv->DeleteLocalRef(jRight) ;

        rightIter++ ;
        i++ ;
      }
    }
  }

	return jGraph ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::buildGraphForJava")) ;
  return (jobject) 0 ;
}
}

void NautilusPilot::applyMappings(JNIEnv* jenv, jobjectArray jMappings, NSDataGraph* pGraph,
                                        int nbTrees, int nbLinks, int nbModels, int nbRights)
//===========================================================================================
{
	if (((NSDataGraph*) NULL == pGraph) || ((jobjectArray) NULL == jMappings) || ((JNIEnv*) NULL == jenv))
  	return ;

try
{
	jint iNbMaps = jenv->GetArrayLength(jMappings) ;

	if (iNbMaps <= 0)
    return ;

  //build C++ version of the mappings
  //=================================
  char** tempNodes     = jni_getMappingField(jenv, "temporaryNODE_ID",   jMappings, iNbMaps) ;
  char** tempObjects   = jni_getMappingField(jenv, "temporaryOBJECT_ID", jMappings, iNbMaps) ;

  char** storedNodes   = jni_getMappingField(jenv, "storedNODE_ID",   jMappings, iNbMaps) ;
  char** storedObjects = jni_getMappingField(jenv, "storedOBJECT_ID", jMappings, iNbMaps) ;

  string sGraphId = pGraph->getGraphID() ;
  if (toBeChangedForComposedId(sGraphId))
  {
    string sNewGraphId = getMappedIdForDoc(sGraphId, iNbMaps, tempObjects, storedObjects) ;
    if (string("") != sNewGraphId)
      pGraph->setGraphID(sNewGraphId) ;

    pGraph->setLastTree(string("")) ;
  }

  //loop on the trees (pGraph->aTrees)
  //==================================
  //
  NSDataTreeArray* pTrees = pGraph->getTrees() ;

  if ((nbTrees > 0) && pTrees && (false == pTrees->empty()))
  {
    NSDataTreeIter treeIter = pTrees->begin() ;
    while (pTrees->end() != treeIter)           //loop on the trees
    {                                           //=================
      NSDataTree* pTreeInfo = *treeIter ;
      string sIdDoc = pTreeInfo->getTreeID() ;

      //in case of Graph of type Person : the test means "new person or new document"
      //in case of Graph of type Object : the test means "new object"
      if (toBeChangedForComposedId(sIdDoc))
      {
        string sNewTreeId = getMappedIdForDoc(sIdDoc, iNbMaps, tempObjects, storedObjects) ;
        if (string("") != sNewTreeId)
        {
          pTreeInfo->setTreeID(sNewTreeId) ;
          sIdDoc = sNewTreeId ;
        }
      }

      //loop on the nodes
      //=================
      NSPatPathoArray PatPathoArray(_pSuper) ;
      pTreeInfo->getRawPatPatho(&PatPathoArray, true) ;

      if (false == PatPathoArray.empty())
      {
        PatPathoIter patIter = PatPathoArray.begin() ;
        while (PatPathoArray.end() != patIter)
        {
          NSPatPathoInfo* patPathoInfo = *patIter ;
          string sTempNode = patPathoInfo->getNodeID() ;

          if (toBeChanged(sTempNode)) //Node is temporary
          {
            string sNewNodeId = getMappedIdForNode(sTempNode, iNbMaps, tempNodes, storedNodes, sIdDoc, storedObjects) ;
            if (string("") != sNewNodeId)
            {
              patPathoInfo->setTreeID(sIdDoc) ;
              patPathoInfo->setNodeID(sNewNodeId) ;
            }
          } //end of if node is temporary

          patIter++ ;
        }   // end of loop on nodes
      }     // enf of if there are nodes

      pTreeInfo->setPatPatho(&PatPathoArray, true) ;

      treeIter++ ;
    }   // end of loop on trees
  }     // end of if( nbTrees > 0 )

  // loop on the links (pGraph->aLinks)
  //===================================
  //
  NSPatLinkArray* pLinks = pGraph->getLinks() ;

  if ((nbLinks > 0) && pLinks && (false == pLinks->empty()))
  {
    NSPatLinkIter linkIter = pLinks->begin() ;
    while (pLinks->end() != linkIter)
    {
      NSPatLinkInfo* linkInfo = *linkIter ;

      //=== modify "qualifie"
      //
      string sQualifie = linkInfo->getQualifie() ;

      string sNewCompositeId = getMappedIdForComposite(sQualifie, iNbMaps, tempNodes, storedNodes, tempObjects, storedObjects) ;
      if (string("") != sNewCompositeId)
        linkInfo->setQualifie(sNewCompositeId) ;

      //=== modify "qualifiant"
      //
      sQualifie = linkInfo->getQualifiant() ;

      sNewCompositeId = getMappedIdForComposite(sQualifie, iNbMaps, tempNodes, storedNodes, tempObjects, storedObjects) ;
      if (string("") != sNewCompositeId)
        linkInfo->setQualifiant(sNewCompositeId) ;

      linkIter++ ;
    }   // end of loop on links
  }     // end of if( nbLinks > 0 )

  // Loop on the models (pGraph->aArchs)
  //====================================
  //
  NSArcLinkArray* pArchs = pGraph->getArchs() ;

  if ((nbModels > 0) && pArchs && (false == pArchs->empty()))
  {
    NSArcLinkIter modelIter = pArchs->begin() ;
    while (pArchs->end() != modelIter)
    {
      NSArcLink* modelInfo = *modelIter ;

      //=== modify "object"
      //
      string sObjectId = modelInfo->getObject() ;

      if (toBeChangedForComposedId(sObjectId))
      {
        string sNewObjectId = getMappedIdForDoc(sObjectId, iNbMaps, tempObjects, storedObjects) ;
        if (string("") != sNewObjectId)
        {
          sObjectId = sNewObjectId ;
          modelInfo->setObject(sNewObjectId) ;
        }
      }

      //=== modify "node"
      //
      string sNodeId = modelInfo->getNode() ;

      if (toBeChanged(sNodeId))
      {
        string sNewNodeId = getMappedIdForNode(sNodeId, iNbMaps, tempNodes, storedNodes, sObjectId, storedObjects) ;
        if (string("") != sNewNodeId)
          modelInfo->setNode(sNewNodeId) ;
      }

      //=== modify "model_object_id"
      //=== No need to transform with mappings - The modelObject is not temporary

      modelIter++ ;
    }   // end of loop on models
  }     // end of if( nbModels > 0 )

  //------------------------------------------------------------------------------
  // Loop on the rights (pGraph->aRights)
  //=====================================
  //
  NSNodeRightArray* pRights = pGraph->getRights() ;

  if ((nbRights > 0) && pRights && (false == pRights->empty()))
  {
    NSNodeRightIter rightIter = pRights->begin() ;
    while (pRights->end() != rightIter)
    {
      NSNodeRight* rightInfo = *rightIter ;

      //=== modify "object"
      string sNode = rightInfo->getNode() ;

      string sNewCompositeId = getMappedIdForComposite(sNode, iNbMaps, tempNodes, storedNodes, tempObjects, storedObjects) ;
      if (string("") != sNewCompositeId)
        rightInfo->setNode(sNewCompositeId) ;

      rightIter++ ;
    }   // end of loop on rights
  }     // end of if( nbRigts > 0 )

  //------------------------------------------------------------------------------

  //delete the mappings variables
  //loop on the mappings to set the new values of trees and nodes
  for( int i = 0 ; i < iNbMaps ; i++)
  {
    delete[] tempNodes[i] ;
    delete[] tempObjects[i] ;

    delete[] storedNodes[i] ;
    delete[] storedObjects[i] ;
  }
  delete[] tempNodes ;
  delete[] tempObjects ;

  delete[] storedNodes ;
  delete[] storedObjects ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::applyMappings")) ;
}
}

string
NautilusPilot::getMappedIdForDoc(string sPreviousId, int iNbMaps, char** pTempObjects, char** pStoredObjects)
{
  if (((char**) NULL == pTempObjects) ||
      ((char**) NULL == pStoredObjects) ||
       (string("") == sPreviousId) || (iNbMaps <= 0))
    return string("") ;

  for (int i = 0 ; i < iNbMaps ; i++)
    if (string(pTempObjects[i]) == sPreviousId)
      return string(pStoredObjects[i]) ;

  return string("") ;
}

string
NautilusPilot::getMappedIdForNode(string sPreviousId, int iNbMaps, char** pTempNodes, char** pStoredNodes, string sDocId, char** pStoredObjects)
{
  if (((char**) NULL == pTempNodes) ||
      ((char**) NULL == pStoredNodes) ||
      ((char**) NULL == pStoredObjects) || (string("") == sPreviousId) ||
                                           (string("") == sDocId) ||
                                           (iNbMaps <= 0))
    return string("") ;

  for (int i = 0 ; i < iNbMaps ; i++)
    if ((string(pStoredObjects[i]) == sDocId) &&
        (string(pTempNodes[i])     == sPreviousId))
      return string(pStoredNodes[i]) ;

  return string("") ;
}

string
NautilusPilot::getMappedIdForComposite(string sPreviousId, int iNbMaps, char** pTempNodes, char** pStoredNodes, char** pTempObjects, char** pStoredObjects)
{
  if ((NULL == pTempNodes) || (NULL == pStoredNodes) || (NULL == pTempObjects) || (NULL == pStoredObjects) || (string("") == sPreviousId) || (iNbMaps <= 0))
    return string("") ;

  // Get object Id and node Id
  //
  size_t iCompositeLen = strlen(sPreviousId.c_str()) ;
  if (iCompositeLen < OBJECT_ID_LEN)
    return string("") ;

  string sObjectId = string(sPreviousId, 0, OBJECT_ID_LEN) ;

  string sNodeId   = string("") ;
  if (iCompositeLen > OBJECT_ID_LEN)
    sNodeId = string(sPreviousId, OBJECT_ID_LEN, iCompositeLen - OBJECT_ID_LEN) ;

  // Apply mappings
  //
  if (toBeChangedForComposedId(sObjectId))
    sObjectId = getMappedIdForDoc(sObjectId, iNbMaps, pTempObjects, pStoredObjects) ;

  if (toBeChanged(sNodeId))
    sNodeId = getMappedIdForNode(sNodeId, iNbMaps, pTempNodes, pStoredNodes, sObjectId, pStoredObjects) ;

  return sObjectId + sNodeId ;
}

int
NautilusPilot::nbOfChildren(JNIEnv* jenv, jobject jTree, string sNodeName)
{
try
{
	if ((NULL == jTree) || ((JNIEnv*) NULL == jenv))
  	return -1 ;

	//=== get method id for the DOMElement getNbChildren method
	jmethodID midNoOfChildrens = jenv->GetMethodID(classDOMElement, "getNbChildren",
                "(Ljava/lang/String;)I") ;
	if (JniExceptionCheck(jenv) || (0 == midNoOfChildrens))
	{
		traceAndDisplayError(string("Cannot locate the DOMElement getNbChildren method. Exiting...")) ;
		return -1 ;
	}

	jstring jNodeName = WinStringTojstring(jenv, sNodeName) ;
	int iResult = reinterpret_cast<jint>(jenv->CallObjectMethod(jTree, midNoOfChildrens, jNodeName)) ;
  if (JniExceptionCheck(jenv))
	{
    jenv->DeleteLocalRef(jNodeName) ;
		traceAndDisplayError(string("DOMElement getNbChildren method failed. Exiting...")) ;
		return -1 ;
	}

	jenv->DeleteLocalRef(jNodeName) ;

	return iResult ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::nbOfChildren")) ;
  return -1 ;
}
}

string
NautilusPilot::getText(JNIEnv* jenv, jobject jTree)
{
try
{
	if (((jobject) NULL == jTree) || ((JNIEnv*) NULL == jenv))
		return string("") ;

	//=== Get method id for the DOMElement getFirstText method
  //
	jmethodID midText = jenv->GetMethodID(classDOMElement, "getFirstText",
                                                      "()Ljava/lang/String;") ;
	if ((0 == midText) || jenv->ExceptionCheck())
	{
  	erreur("Cannot locate the DOMElement getFirstText method. Exiting...\n", standardError) ;
    return string("") ;
	}

	jstring jResult = reinterpret_cast<jstring>(jenv->CallObjectMethod(jTree, midText)) ;
  string sRes = JavaSystem::NewStringObjWithSpecialCharacters(jResult) ;
  jenv->DeleteLocalRef(jResult) ;

  return sRes ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getText")) ;
  return NULL ;
}
}

string
NautilusPilot::getTagName(JNIEnv* jenv, jobject jTree)
{
try
{
	if (((jobject) NULL == jTree) || ((JNIEnv*) NULL == jenv))
		return string("") ;

	//=== Get method id for the DOMElement getFirstText method
  //
	jmethodID midTag = jenv->GetMethodID(classDOMElement, "getTagName", "()Ljava/lang/String;") ;
	if ((0 == midTag) || jenv->ExceptionCheck())
	{
		erreur("Cannot locate the DOMElement getTagName method. Exiting...\n", standardError) ;
		return string("") ;
	}

	jstring jResult = reinterpret_cast<jstring>(jenv->CallObjectMethod(jTree, midTag)) ;

	string sRes = JavaSystem::NewStringObjWithSpecialCharacters(jResult) ;
	jenv->DeleteLocalRef(jResult) ;

	return sRes ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getTagName")) ;
  return string("") ;
}
}

void
NautilusPilot::DOMTreeToBasicAttribute(JNIEnv* jenv, NSBasicAttributeArray* pAttrElem, jobject jTree, string sNodeName)
{
try
{
	if ((NULL == pAttrElem) || (NULL == jTree) || ((JNIEnv*) NULL == jenv))
  	return ;

	jstring jNodeName = WinStringTojstring(jenv, sNodeName) ;
	jobjectArray jDomElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jTree, midChildrenElements, jNodeName, NULL)) ;

	if (JniExceptionCheck(jenv) || ((jobjectArray) NULL == jDomElems))
  {
    // Don't use traceAndDisplayError because displaying stops all
		// traceAndDisplayError(string("NautilusPilot::DOMTreeToBasicAttribute : CallObjectMethod failed. Exiting...")) ;
    string sText = string("NautilusPilot::DOMTreeToBasicAttribute : CallObjectMethod failed. Exiting...") ;
    _pSuper->trace(&sText, 1, NSSuper::trError) ;
    jenv->DeleteLocalRef(jNodeName) ;
    jenv->DeleteLocalRef(jDomElems) ;
		return ;
	}

  jenv->DeleteLocalRef(jNodeName) ;

	jsize count = jenv->GetArrayLength(jDomElems) ;
	for (int i = 0 ; i < count ; i++)
	{
    jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
    if (NULL != jNodeElem)
    {
    	if ((getTagName(jenv, jNodeElem) == string("")) /* || (getText(jNodeElem) == "")*/)
      {
      	jenv->DeleteLocalRef(jDomElems) ;
        jenv->DeleteLocalRef(jNodeElem) ;
        return ;
      }

      NSBasicAttribute* pAttribute = new NSBasicAttribute() ;
      pAttribute->setBalise(getTagName(jenv, jNodeElem)) ;
      pAttribute->setText(getText(jenv, jNodeElem));
      pAttrElem->push_back(pAttribute) ;
      jenv->DeleteLocalRef(jNodeElem) ;
    }
	}

  jenv->DeleteLocalRef(jDomElems) ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::DOMTreeToBasicAttribute")) ;
}
}

//used from patient list
//used from all persons list
//used from person list from traits
bool
NautilusPilot::personList(const char* serviceName, NSPersonsAttributesArray *pPatList, NSBasicAttributeArray *pAttrArray)
{
	if ((NULL == serviceName) ||
      ((NSPersonsAttributesArray*) NULL == pPatList) ||
      ((NSBasicAttributeArray*)    NULL == pAttrArray))
  	return false ;

try
{
  JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    traceAndDisplayError(sErrorText) ;
		return false ;
	}

	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "personOrObjectList",
      "(Ljava/lang/String;[Ljava/lang/String;)Ldomlightwrapper/DOMElement;") ;
	if (JniExceptionCheck(jenv) || (0 == mid))
  {
    EndPilotMethod(jenv) ;
    string sErrorText = _pSuper->getText("NTIERS", "personListMethodNotFound") ;
    traceAndDisplayError(sErrorText) ;
		return false ;
	}

  jobjectArray strArray  = createJObjectArray(jenv, pAttrArray) ;
	jstring      jServName = WindowsTojstring(jenv, serviceName) ;

	jobject jDOMElem = jenv->CallObjectMethod(jNautilusPilot, mid, jServName, strArray) ;

	if (JniExceptionCheck(jenv) || ((jobject) NULL == jDOMElem))
	{
    if (getReturnType(jenv) == WARNING_RETURN)
    {
      jstring jWarning = (jstring)jenv->GetObjectField(jNautilusPilot, jFidWarning) ;
      if (jWarning != NULL )
      {
        // warnings
        string sWarning = string(JavaSystem::NewStringWithSpecialCharacters(jWarning)) ;
        if (sWarning == "noData")
        {
          EndPilotMethod(jenv) ;
          jenv->DeleteLocalRef(jDOMElem) ;
          return true ;
        }
        else
        {
          EndPilotMethod(jenv) ;
          warningMessage = _pSuper->getText("NTIERS", sWarning) ;
          jenv->DeleteLocalRef(jDOMElem) ;
          return false ;
        }
      }
    }
    else if (getTextError(jenv) != "")
    {
      errorMessage = getTextError(jenv) ;
      jenv->DeleteLocalRef(jDOMElem) ;
      EndPilotMethod(jenv) ;
      return false ;
    }
    else
    {
      jenv->DeleteLocalRef(jDOMElem) ;
      EndPilotMethod(jenv) ;
      return true ;
    }
	}

  jenv->DeleteLocalRef(strArray) ;
  jenv->DeleteLocalRef(jServName) ;

  jstring jNodeName = WindowsTojstring(jenv, "person") ;
  jobjectArray jDomElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jDOMElem, midChildrenElements, jNodeName)) ;
  jenv->DeleteLocalRef(jNodeName) ;
  if ((jobjectArray) NULL == jDomElems)
  {
    jenv->DeleteLocalRef(jDomElems) ;
    EndPilotMethod(jenv) ;
    return true ;
  }

  int count = jenv->GetArrayLength(jDomElems) ;
  for (int i = 0 ; i < count ; i++)
  {
    jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
    if (jNodeElem != NULL)
    {
      NSBasicAttributeArray* pPersonAttributes = new NSBasicAttributeArray() ;
      DOMTreeToBasicAttribute(jenv, pPersonAttributes, jNodeElem) ;
      jenv->DeleteLocalRef(jNodeElem) ;
      pPatList->push_back(pPersonAttributes) ;
    }
    else
    {
      string ps = string("NautilusPilot::personList : GetObjectArrayElement failed.") ;
      _pSuper->trace(&ps, 1, NSSuper::trError) ;
    }
  }

  jenv->DeleteLocalRef(jDomElems) ;

  EndPilotMethod(jenv) ;

  return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::personList")) ;
  return false ;
}
}

bool
NautilusPilot::getUserProperties(const char* serviceName, NSPersonsAttributesArray *pPatList,
                                                    NSBasicAttributeArray *pAttrArray)
//====================================================================================
{
  if (!serviceName || !pPatList || !pAttrArray)
    return false ;

  return personList(serviceName, pPatList, pAttrArray) ;
}

//used from all object list
bool
NautilusPilot::objectList(const char* serviceName, NSPersonsAttributesArray *pObjectList,
                                                    NSBasicAttributeArray *pAttrArray)
//====================================================================================
{
	if ((NULL == serviceName) || (NULL == pObjectList) || (NULL == pAttrArray))
		return false ;

try
{
	JNIEnv* jenv = StartPilotMethod() ;
	if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "personOrObjectList",
      "(Ljava/lang/String;[Ljava/lang/String;)Ldomlightwrapper/DOMElement;");
	if ((0 == mid) || jenv->ExceptionCheck())
	{
    EndPilotMethod(jenv) ;
  	string sErrorText = _pSuper->getText("NTIERS", "objectListMethodNotFound") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jobject jDOMElem = jenv->CallObjectMethod(jNautilusPilot, mid, jServName, strArray) ;
	jenv->DeleteLocalRef(strArray) ;
	jenv->DeleteLocalRef(jServName) ;
	if (NULL == jDOMElem)
	{
  	if (getReturnType(jenv) == WARNING_RETURN)                //WARNING_RETURN
    	warningMessage = getTextWarning(jenv, "NTIERS") ;
    else
    	errorMessage = getTextError(jenv) ;
    jenv->DeleteLocalRef(jDOMElem) ;
    EndPilotMethod(jenv) ;
    return false ;
	}

  jstring jNodeName = WindowsTojstring(jenv, "object") ;
  jobjectArray jDomElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jDOMElem, midChildrenElements, jNodeName)) ;
  jenv->DeleteLocalRef(jNodeName) ;
  jenv->DeleteLocalRef(jDOMElem) ;
  if (jDomElems == NULL)
  {
  	string ps = string("NautilusPilot::objectList : CallObjectMethod failed for midChildrenElements. exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    jenv->DeleteLocalRef(jDomElems);
    EndPilotMethod(jenv) ;
    return true ;
	}

  int count = jenv->GetArrayLength(jDomElems) ;
  for (int i = 0 ; i < count ; i++)
  {
  	jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
    if (jNodeElem != NULL)
    {
    	NSBasicAttributeArray* pobjAttributes = new NSBasicAttributeArray();
      DOMTreeToBasicAttribute(jenv, pobjAttributes, jNodeElem);
      jenv->DeleteLocalRef(jNodeElem) ;
      pObjectList->push_back(pobjAttributes) ;
    }
    else
    {
    	string ps = string("NautilusPilot::personList : GetObjectArrayElement failed.") ;
      _pSuper->trace(&ps, 1, NSSuper::trError) ;
    }
	}

	jenv->DeleteLocalRef(jDomElems) ;

  EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::objectList")) ;
  return false ;
}
}

jobjectArray
NautilusPilot::createJObjectArray(JNIEnv* jenv, NSBasicAttributeArray *pAttrArray)
//=================================================================
{
try
{
	if (((NSBasicAttributeArray *) NULL == pAttrArray) || ((JNIEnv*) NULL == jenv))
		return (jobjectArray) 0 ;

	size_t size = pAttrArray->size() ;

	jobjectArray strArray = jenv->NewObjectArray(2 * size, classStr, NULL) ;
	if (((jobjectArray) NULL == strArray) || JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("NautilusPilot::createJObjectArray : NewObjectArray failed. Exiting...")) ;
    return (jobjectArray) NULL ;
	}

	if (pAttrArray->empty())
  	return strArray ;

	int i = 0 ;
	for (NSBasicAttributeIter iterAttr = pAttrArray->begin() ; pAttrArray->end() != iterAttr ; iterAttr++)
	{
		jstring jstr = WinStringTojstring(jenv, (*iterAttr)->getBalise()) ;
		jenv->SetObjectArrayElement(strArray, i, jstr) ;
		i++ ;
		jstring jstr2 = WinStringTojstring(jenv, (*iterAttr)->getText()) ;
		jenv->SetObjectArrayElement(strArray, i, jstr2) ;
		i++ ;
		jenv->DeleteLocalRef(jstr) ;
    jenv->DeleteLocalRef(jstr2) ;
	}

	return strArray ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::createJObjectArray")) ;
  return NULL ;
}
}

//----------------------------- create Person or Object ------------------------
//la liste des patients montée ici n'est pas utilisée actuellement
//iType = 0 person
//iType = 1 object
bool
NautilusPilot::createPersonOrObject(const char* serviceName, NSDataGraph* pGraph, NSPersonsAttributesArray *pList,
                                NSBasicAttributeArray *pAttrArray, int iType, bool forcedCreate)
//================================================================================================================
{
try
{
	if ((NULL == pAttrArray) || (NULL == pGraph) || (NULL == pList))
  	return false ;

	JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return NULL ;
	}

  int nbTrees  = 0 ;
  int nbLinks  = 0 ;
  int nbModels = 0 ;
  int nbRights = 0 ;

  // 1) build a graph for the java side
  //====================================
  jobject jGraph = buildGraphForJava(jenv, pGraph, nbTrees, nbLinks, nbModels, nbRights) ;
  if (0 == nbTrees)
  {
  	jenv->DeleteLocalRef(jGraph) ;
    EndPilotMethod(jenv) ;
    return false ;
  }

  if (forcedCreate)
  	// Le contenu n'a pas d'importance, c'est l'existence de la balise qui compte
  	pAttrArray->push_back(new NSBasicAttribute("force", "true")) ;

  jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;

  if (forcedCreate)
  	pAttrArray->eraseAttributeValue("force") ;

  jmethodID mid ;
  if (PERSON_TYPE == iType)
		mid = jenv->GetMethodID(classNautilusPilot, "createPerson",
                "(Lnautilus/NautilusGraph;Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/Object;") ;
  else
  	if (OBJECT_TYPE == iType)
    	mid = jenv->GetMethodID(classNautilusPilot, "createObject",
                "(Lnautilus/NautilusGraph;Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/Object;") ;
  if ((0 == mid) || jenv->ExceptionCheck())
  {
  	string sErrorText = "Cannot locate the NautilusPilot createPatient method." ;
  	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
    jenv->DeleteLocalRef(strArray) ;
    jenv->DeleteLocalRef(jGraph) ;
    EndPilotMethod(jenv) ;
		return false ;
	}

  jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jobjectArray jObjet = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, mid, jGraph, jServName, strArray)) ;
	jenv->DeleteLocalRef(strArray) ;
  jenv->DeleteLocalRef(jServName);
  jenv->DeleteLocalRef(jGraph) ;

	if (jenv->ExceptionOccurred())
	{
  	//erreur("Erreur execution: createPatient - service",standardError,0,0);
		string sErrorText = getTextError(jenv) ;
  	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
		jenv->ExceptionDescribe();
    jenv->DeleteLocalRef(jObjet);
    EndPilotMethod(jenv) ;
		return false ;
	}

	if ((NULL == jObjet) && (getReturnType(jenv) == WARNING_RETURN))
	{
		string sErrorText = getTextError(jenv) ;
  	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
    EndPilotMethod(jenv) ;
		return false ;
	}

  if (getReturnType(jenv) == DOMELEMENT_RETURN)    //DOMELEMENT_RETURN
  {
    jstring jNodeName ;
    switch(iType)
    {
      case  PERSON_TYPE :
        jNodeName = WindowsTojstring(jenv, "person") ;
        break ;
      case  OBJECT_TYPE :
        jNodeName = WindowsTojstring(jenv, "object") ;
        break ;
    }
    jobjectArray jDomElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jObjet, midChildrenElements, jNodeName)) ;
    jenv->DeleteLocalRef(jNodeName) ;
    int count = 0 ;
    if (NULL == jDomElems)
    {
      string ps = string("NautilusPilot::createPersonOrObject : CallObjectMethod failed. Exiting...") ;
      _pSuper->trace(&ps, 1, NSSuper::trError) ;
      jenv->DeleteLocalRef(jObjet);
      EndPilotMethod(jenv) ;
      return false ;
    }
    count = jenv->GetArrayLength(jDomElems) ;
    for (int i = 0 ; i < count ; i++)
    {
      jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
      NSBasicAttributeArray* pIDAttributes = new NSBasicAttributeArray() ;

      DOMTreeToBasicAttribute(jenv, pIDAttributes, jNodeElem) ;
      pList->push_back(pIDAttributes) ;
      jenv->DeleteLocalRef(jNodeElem) ;
    }
  }
  else
    if (getReturnType(jenv) == MAPPING_RETURN)             //MAPPING_RETURN
  	{
  		//si l'enregistement est effectué pour une personne on transfer les props de bloque.
    	if (PERSON_TYPE == iType)
        transformResultFieldInAttributesArray(jenv, pList, "status", "*") ;

      applyMappings(jenv, jObjet, pGraph, nbTrees, nbLinks, nbModels, nbRights) ;
    }

  jenv->DeleteLocalRef(jObjet) ;

  EndPilotMethod(jenv) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::createPersonOrObject")) ;
  return false ;
}
}

//-------------------------invokeService----------------------------------------
//utilisée pour la recherche des objets
bool
NautilusPilot::invokeService(const char* serviceName, NSDataGraph* pGraph,
                    NSPersonsAttributesArray *pObjList, NSBasicAttributeArray *pAttrArray)
//========================================================================================
{
try
{
	if (((NSBasicAttributeArray*) NULL == pAttrArray) || ((NSDataGraph*) NULL == pGraph))
		return false ;

	JNIEnv* jenv = GetJniEnv() ;

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeServiceReturnObject",
      "(Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/Object;");
	if ((0 == mid) || jenv->ExceptionCheck())
	{
    ReleaseJniEnv(jenv) ;
  	string sErrorText = "Cannot locate the NautilusPilot invokeService (3) method." ;
  	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
		return false;
	}

	jobjectArray jObject = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, mid, jServName, strArray)) ;
  jenv->DeleteLocalRef(jServName) ;
  jenv->DeleteLocalRef(strArray) ;
	if (jenv->ExceptionOccurred() )
	{
		//erreur("Erreur execution: createPatient - service",standardError,0,0);
		string sErrorText = getTextError(jenv) ;
  	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
    jenv->ExceptionDescribe() ;
    ReleaseJniEnv(jenv) ;
    return false ;
	}

	if (NULL == jObject)
	{
  	if (getReturnType(jenv) == WARNING_RETURN)
    {
    	warningMessage = getTextWarning(jenv, "NTIERS") ;
      ReleaseJniEnv(jenv) ;
      return false ;
    }
    errorMessage = getTextError(jenv) ;
    ReleaseJniEnv(jenv) ;
		return false ;
	}
	else
	{
		if (getReturnType(jenv) == DOMELEMENT_RETURN)    //DOMELEMNT_RETURN
		{
    	jstring jNodeName = WindowsTojstring(jenv, "object") ;
      jobjectArray jDomElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jObject, midChildrenElements, jNodeName)) ;
      jenv->DeleteLocalRef(jNodeName) ;
      int count = 0 ;
      if (NULL == jDomElems)
			{
      	string ps = string("NautilusPilot::invokeService : CallObjectMethod failed. Exiting...") ;
				_pSuper->trace(&ps, 1, NSSuper::trError) ;
        jenv->DeleteLocalRef(jDomElems) ;
        ReleaseJniEnv(jenv) ;
        return false ;
			}
      count = jenv->GetArrayLength(jDomElems) ;
      for (int i = 0 ; i < count ; i++)
      {
				jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
        if (jNodeElem)
        {
        	NSBasicAttributeArray* pIDAttributes = new NSBasicAttributeArray() ;

					DOMTreeToBasicAttribute(jenv, pIDAttributes, jNodeElem) ;
					pObjList->push_back(pIDAttributes) ;
          jenv->DeleteLocalRef(jNodeElem) ;
        }
        else
				{
					string ps = string("NautilusPilot::invokeService : CallObjectMethod failed.") ;
					_pSuper->trace(&ps, 1, NSSuper::trError) ;
				}
			}
      jenv->DeleteLocalRef(jDomElems) ;
		}
		else
			if (getReturnType(jenv) == GRAPH_RETURN)    //GRAPH_RETURN
			{
				// build a C graph from the java graph object
				//===========================================
				buildGraphForC(jenv, jObject, pGraph) ;
				jenv->DeleteLocalRef(jObject) ;
				transformResultFieldInAttributesArray(jenv, pObjList, "list", "object") ;
			}
	}

  ReleaseJniEnv(jenv) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::invokeService")) ;
  return false ;
}
}

bool
NautilusPilot::modifyPersonOrObject(const char* serviceName, NSDataGraph* pGraph,
                    NSPersonsAttributesArray *pList, NSBasicAttributeArray *pAttrArray)
//=====================================================================================
{
try
{
	string sMessage = string("NautilusPilot::modifyPersonOrObject -- begin") ;
	_pSuper->trace(&sMessage, 1, NSSuper::trDetails) ;

	if (((NSBasicAttributeArray*) NULL == pAttrArray) ||
                                           ((NSDataGraph*) NULL == pGraph))
		return false ;

	JNIEnv* jenv = StartPilotMethod() ;
	if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    traceAndDisplayError(sErrorText) ;
		return false ;
	}

	int nbTrees  = 0 ;
	int nbLinks  = 0 ;
	int nbModels = 0 ;
	int nbRights = 0 ;

	// 1) build a graph for the java side
	//====================================
	jobject jGraphIn = buildGraphForJava(jenv, pGraph, nbTrees, nbLinks, nbModels, nbRights) ;

	sMessage = string("NautilusPilot::modifyPersonOrObject -- Graph4Java build") ;
	_pSuper->trace(&sMessage, 1, NSSuper::trDetails) ;

	if (0 == nbTrees)
	{
		string sErrorText = "L'arbre de votre graph ne contient pas de données" ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
		jenv->DeleteLocalRef(jGraphIn) ;
    EndPilotMethod(jenv) ;
		return false ;
	}
  /*	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeService",
                "(Ljava/lang/String;Lnautilus/NautilusGraph;[Ljava/lang/String;)[LgraphServer/Mapping;");
	if (mid == 0)
	{
		erreur("Cannot locate the NautilusPilot invokeService (2) method.",standardError, 0, 0);
		return false;
	}  */

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
	jstring jServName = WindowsTojstring(jenv, serviceName) ;

	sMessage = string("NautilusPilot::modifyPersonOrObject -- JObjectArray created") ;
	_pSuper->trace(&sMessage, 1, NSSuper::trDetails) ;

	jobjectArray jGraph = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, midModify, jServName, jGraphIn, strArray)) ;

	sMessage = string("NautilusPilot::modifyPersonOrObject -- method called") ;
	_pSuper->trace(&sMessage, 1, NSSuper::trDetails) ;

	jenv->DeleteLocalRef(strArray) ;
	jenv->DeleteLocalRef(jServName) ;
	jenv->DeleteLocalRef(jGraphIn) ;
	if (jenv->ExceptionOccurred())
	{
    sMessage = string("NautilusPilot::modifyPersonOrObject -- an Exception occured") ;
    _pSuper->trace(&sMessage, 1, NSSuper::trError) ;

		//erreur( getTextError(jenv), standardError, 0, 0);
		string sErrorText = getTextError(jenv) ;
  	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
		jenv->DeleteLocalRef(jGraph) ;
    EndPilotMethod(jenv) ;
		return false ;
	}

	if ((jGraph == NULL) && (getReturnType(jenv) == WARNING_RETURN))        //WARNING_RETURN
	{
    sMessage = string("NautilusPilot::modifyPersonOrObject -- a warning was returned") ;
    _pSuper->trace(&sMessage, 1, NSSuper::trWarning) ;

		warningMessage = string(getTextWarning(jenv, "NTIERS")) ;
    EndPilotMethod(jenv) ;
		return false ;
	}

	applyMappings(jenv, jGraph, pGraph, nbTrees, nbLinks, nbModels, nbRights) ;

	sMessage = string("NautilusPilot::modifyPersonOrObject -- mappings applied") ;
	_pSuper->trace(&sMessage, 1, NSSuper::trDetails) ;

	jenv->DeleteLocalRef(jGraph) ;

	sMessage = string("NautilusPilot::modifyPersonOrObject -- get2call transformResultFieldInAttributesArray") ;
	_pSuper->trace(&sMessage, 1, NSSuper::trDetails) ;

	transformResultFieldInAttributesArray(jenv, pList) ;

  EndPilotMethod(jenv) ;

	sMessage = string("NautilusPilot::modifyPersonOrObject -- end") ;
	_pSuper->trace(&sMessage, 1, NSSuper::trDetails) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::modifyPersonOrObject")) ;
  return false ;
}
}

bool
NautilusPilot::modifyTraitsForPersonOrObject(const char* serviceName,
                    NSPersonsAttributesArray * /* pList */, NSBasicAttributeArray *pAttrArray)
//=====================================================================================
{
try
{
	string sMessage = string("NautilusPilot::modifyTraitsForPersonOrObject -- begin") ;
	_pSuper->trace(&sMessage, 1, NSSuper::trDetails) ;

	if (NULL == pAttrArray)
		return false ;

	JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
  {
		traceAndDisplayError(string("NautilusPilot::modifyTraitsForPersonOrObject : jenv is null. Exiting...")) ;
    return false ;
  }

	bool bServiceResult = invokeSimpleService(jenv, serviceName, pAttrArray) ;

  EndPilotMethod(jenv) ;

  return bServiceResult ;

/*
	JNIEnv* jenv = JavaSystem::jenv ;
	if (jenv == NULL)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(pAttrArray) ;
	jstring jServName = jenv->NewStringUTF( serviceName ) ;

	sMessage = string("NautilusPilot::modifyTraitsForPersonOrObject -- JObjectArray created") ;
	pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSteps) ;

	jobjectArray jGraph = (jobjectArray)jenv->CallObjectMethod(jNautilusPilot, midModify, jServName, NULL, strArray) ;

	sMessage = string("NautilusPilot::modifyTraitsForPersonOrObject -- method called") ;
	pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSteps) ;

	jenv->DeleteLocalRef(strArray) ;
	jenv->DeleteLocalRef(jServName) ;
	if (jenv->ExceptionOccurred())
	{
		//erreur( getTextError(jenv), standardError, 0, 0);
		string sErrorText = getTextError(jenv) ;
  	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError, 0, 0) ;
		jenv->DeleteLocalRef(jGraph) ;
		return false ;
	}

	if (getReturnType(jenv) == WARNING_RETURN)        //WARNING_RETURN
	{
		warningMessage = string(getTextWarning(jenv, "NTIERS")) ;
		return false ;
	}

	sMessage = string("NautilusPilot::modifyTraitsForPersonOrObject -- get2call transformResultFieldInAttributesArray") ;
	pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSteps) ;

	transformResultFieldInAttributesArray(jenv, pList) ;

	sMessage = string("NautilusPilot::modifyTraitsForPersonOrObject -- end") ;
	pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSteps) ;

	return true ;
*/
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::modifyTraitsForPersonOrObject")) ;
  return false ;
}
}

//-------------------------invokeService-----------------------------------------------
//utilisée pour login et pour la modification d'une personne
//invokeService return mapping. Il faut recuperer le DOMElement
bool
NautilusPilot::invokeService2ReturnElements(const char* serviceName, NSDataGraph* pGraph,
                    NSPersonsAttributesArray *pList, NSBasicAttributeArray *pAttrArray)
//=======================================================================================
{
try
{
	if ((NULL == serviceName) || (NULL == pGraph) || (NULL == pList) || (NULL == pAttrArray))
  	return false ;

	JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}
	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if (NULL == strArray)
	{
  	string ps = string("NautilusPilot::invokeService2ReturnElements : createJObjectArray failed. Exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jobject jGraph = NULL ;
	if (string(SERV_LOGIN) == string(serviceName))
	{
		/*classNautilusPilot, "invokeService",  "(Ljava/lang/String;[Ljava/lang/String;)Lnautilus/NautilusGraph;");    */
    jGraph = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, midSimpInvoke, jServName, strArray)) ;
	}
  else
  {
  	/*classNautilusPilot, "invokeService",  "(Ljava/lang/String;Lnautilus/NautilusGraph;[Ljava/lang/String;)[LgraphServer/Mapping;");*/
    jGraph = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, midInvoke, jServName, strArray)) ;
	}

	jenv->DeleteLocalRef(strArray) ;
	jenv->DeleteLocalRef(jServName) ;
	if (jenv->ExceptionOccurred() )
	{
		errorMessage = getTextError(jenv) ;
    jenv->DeleteLocalRef(jGraph) ;
		return false ;
	}

  if ((NULL == jGraph) && (WARNING_RETURN == getReturnType(jenv)))        //WARNING_RETURN
	{
  	warningMessage = string(getTextWarning(jenv, "NTIERS")) ;
    jenv->DeleteLocalRef(jGraph) ;
    return false ;
	}
  buildGraphForC(jenv, jGraph, pGraph) ;
  jenv->DeleteLocalRef(jGraph) ;

	transformResultFieldInAttributesArray(jenv, pList) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::invokeService2ReturnElements")) ;
  return false ;
}
}

//-------------------------invokeService-----------------------------------------------
//utilisée pour import Ou modification object apres update avec le serveur collective
//invokeService return a graph. Il faut recuperer le DOMElement
bool
NautilusPilot::updateObjectList(const char* serviceName, NSDataGraph* pGraph, NSBasicAttributeArray *pAttrArray)
//=============================================================================================================
{
try
{
	if (!serviceName || !pGraph || !pAttrArray)
  	return false ;

	JNIEnv* jenv = GetJniEnv() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if (strArray == NULL)
	{
  	string ps = string("NautilusPilot::updateObjectList : createJObjectArray failed. Exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jmethodID mid;

    //modification d'une personne. Invoke service 1
    mid = jenv->GetMethodID(classNautilusPilot, "updateObjectList",
        "(Ljava/lang/String;[Ljava/lang/String;Lnautilus/NautilusGraph;)Ljava/lang/String;");
	if ((0 == mid) || jenv->ExceptionCheck())
	{
		string sErrorText = "Cannot locate the NautilusPilot updateObjectList method." ;
  	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	jstring jWarning = reinterpret_cast<jstring>(jenv->CallObjectMethod(jNautilusPilot, mid, jServName, strArray)) ;

	jenv->DeleteLocalRef(strArray) ;
	jenv->DeleteLocalRef(jServName) ;
	if (jenv->ExceptionOccurred())
	{
  	errorMessage = getTextError(jenv) ;
    jenv->ExceptionDescribe() ;
		return false ;
	}
	if (jWarning == NULL)
	{
		return true ;
  }
	else
	{
  	warningMessage = string(JavaSystem::NewStringWithSpecialCharacters(jWarning)) ;
    return false ;
	}
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::updateObjectList")) ;
  return false ;
}
}

//unlockPatient and  unlockAllPatients
bool
NautilusPilot::unlockPatient(const char* serviceName, NSBasicAttributeArray *pAttrArray)
{
	if (NULL == pAttrArray)
		return false ;

  JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
  {
		traceAndDisplayError(string("NautilusPilot::unlockPatient : jenv is null. Exiting...")) ;
    return false ;
  }

	bool bServiceResult = invokeSimpleService(jenv, serviceName, pAttrArray) ;

  EndPilotMethod(jenv) ;

  return bServiceResult ;
}

bool
NautilusPilot::setEpisodusVersion(string version)
{
	if (string("") == version)
		return false ;

  JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
  {
		traceAndDisplayError(string("NautilusPilot::unlockPatient : jenv is null. Exiting...")) ;
    return false ;
  }

	const char* serviceName = SERV_SET_PARAMETERS.c_str() ;
	NSBasicAttributeArray AttrArray ;
	AttrArray.push_back(new NSBasicAttribute("version", version)) ;

	bool bInvoke = invokeSimpleService(jenv, serviceName, &AttrArray) ;

  EndPilotMethod(jenv) ;

  return bInvoke ;
}

//modify PIDS server data
bool
NautilusPilot::modifyPIDSData(const char* serviceName, NSBasicAttributeArray *pAttrArray)
{
	if (NULL == pAttrArray)
		return false ;

  JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
  {
		traceAndDisplayError(string("NautilusPilot::modifyPIDSData : jenv is null. Exiting...")) ;
    return false ;
  }

	bool bServiceResult = invokeSimpleService(jenv, serviceName, pAttrArray) ;

  EndPilotMethod(jenv) ;

  return bServiceResult ;
}

bool
NautilusPilot::startAgent(string sAgentName)
{
try
{
	if (string("") == sAgentName)
  	return false ;

	JNIEnv* jenv = GetJniEnv() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

  string service = "doConnect" + sAgentName ;
  const char* serviceName = service.c_str() ;
  jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeSimpleService",
                            "(Ljava/lang/String;)Z") ;
	if ((0 == mid) || jenv->ExceptionCheck())
	{
		fprintf(stderr, "Cannot locate the NautilusPilot invokeSimpleService method.") ;
		return false ;
	}

	// jboolean jRes = reinterpret_cast<jboolean>(CallObjectMethod(jenv, jNautilusPilot, mid, jServName)) ;
  jboolean jRes = jenv->CallBooleanMethod(jNautilusPilot, mid, jServName) ;
	if (jenv->ExceptionOccurred() )
	{
		string sErrorText = getTextError(jenv) ;
  	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
		jenv->ExceptionDescribe() ;
		return false ;
	}

  jenv->DeleteLocalRef(jServName) ;
	bool bRes = (bool) jRes ;
	if ((false == bRes) && (getReturnType(jenv) == WARNING_RETURN)) // WARNING_RETURN
	{
  	jstring jWarning = (jstring)jenv->GetObjectField(jNautilusPilot, jFidWarning) ;
    string sWarning = _pSuper->getText("NTIERS", "nonInstancedWarning");
    if (NULL != jWarning)             // warnings
    	sWarning = string(JavaSystem::NewStringWithSpecialCharacters(jWarning)) ;

    _AvailableAgentList.setAgentError( sAgentName, sWarning) ;

    return false ;
	}

	return true ;
    //return invokeSimpleService(serviceName, pAttrArray);
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::startAgent")) ;
  return false ;
}
}

//utilise invokeSimpleService
bool
NautilusPilot::invokeSimpleService(JNIEnv* jenv, const char* serviceName, NSBasicAttributeArray *pAttrArray)
//===========================================================================================
{
try
{
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    traceAndDisplayError(sErrorText) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
	if (NULL == strArray)
	{
  	traceAndDisplayError(string("NautilusPilot::unlockPatient : createJObjectArray failed. Exiting...")) ;
    return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeSimpleService",
                            "(Ljava/lang/String;[Ljava/lang/String;)Z") ;
	if ((0 == mid) || JniExceptionCheck(jenv))
	{
		traceAndDisplayError(string("Cannot locate the NautilusPilot invokeSimpleService method.")) ;
    jenv->DeleteLocalRef(strArray) ;
		return false ;
	}

	// jboolean jRes = reinterpret_cast<jboolean>(CallObjectMethod(jenv, jNautilusPilot, mid, jServName, strArray)) ;
  jboolean jRes = jenv->CallBooleanMethod(jNautilusPilot, mid, jServName, strArray) ;
  if (JniExceptionCheck(jenv))
	{
		string sErrorText = getTextError(jenv) ;
    traceAndDisplayError(sErrorText) ;
		jenv->ExceptionDescribe() ;

    jenv->DeleteLocalRef(strArray) ;
    jenv->DeleteLocalRef(jServName) ;

		return false ;
	}

	jenv->DeleteLocalRef(strArray) ;
  jenv->DeleteLocalRef(jServName) ;

	bool bRes = (bool) jRes ;
	if (false == bRes)
	{
		string sErrorText = getTextError(jenv) ;
  	traceAndDisplayError(sErrorText) ;
		return false ;
	}
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::invokeSimpleService")) ;
  return false ;
}
}

//login, password, personId du nouveau membre, le graph equipe de santé
bool
NautilusPilot::addHealthTeamMember(const char* serviceName,
                NSBasicAttributeArray *pAttrArray, NSDataGraph* pGraph)
//=====================================================================
{
  if ((NULL == pAttrArray) || (NULL == pGraph))
    return false ;

  JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
  {
		traceAndDisplayError(string("NautilusPilot::addHealthTeamMember : jenv is null. Exiting...")) ;
    return false ;
  }

  bool bServiceResult = invokeAddGraphService(jenv, serviceName, pAttrArray, pGraph) ;

  EndPilotMethod(jenv) ;

  return bServiceResult ;
}

//utilise invokeAddGraphService
bool
NautilusPilot::invokeAddGraphService(JNIEnv* jenv, const char* serviceName,
                                NSBasicAttributeArray *pAttrArray, NSDataGraph* pGraph)
//======================================================================================
{
try
{
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    traceAndDisplayError(sErrorText) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
	if (strArray == NULL)
	{
  	string ps = string("NautilusPilot::invokeAddGraphService : createJObjectArray failed. Exiting...") ;
    traceAndDisplayError(ps) ;
    return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeAddGraphService",
                "(Ljava/lang/String;Lnautilus/NautilusGraph;[Ljava/lang/String;)Z") ;
	if ((0 == mid) || jenv->ExceptionCheck())
	{
		traceAndDisplayError(string("Cannot locate the NautilusPilot invokeSimpleService method.")) ;
		return false ;
	}

/*
    string ps2 = "NautilusPilot::writeGraph : start of buildGraphForJava" ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;
*/
  int nbTrees  = 0;
	int nbLinks  = 0;
	int nbModels = 0;
	int nbRights = 0;
	jobject jGraph = buildGraphForJava(jenv, pGraph, nbTrees, nbLinks, nbModels, nbRights);
	if ((nbTrees == 0) || (jGraph == NULL))
		return false ;

/*
    ps2 = "NautilusPilot::writeGraph : calling the Pilot" ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;
*/

	// jboolean jRes = reinterpret_cast<jboolean>(CallObjectMethod(jenv, jNautilusPilot, mid, jServName, jGraph, strArray)) ;
  jboolean jRes = jenv->CallBooleanMethod(jNautilusPilot, mid, jServName, jGraph, strArray) ;

	jenv->DeleteLocalRef(strArray) ;
  jenv->DeleteLocalRef(jServName) ;
	if (jenv->ExceptionOccurred())
	{
		string sErrorText = getTextError(jenv) ;
  	traceAndDisplayError(sErrorText) ;
		jenv->ExceptionDescribe() ;
		return false ;
	}

	bool bRes = (bool) jRes ;
	if (false == bRes)
	{
		string sErrorText = getTextError(jenv) ;
  	traceAndDisplayError(sErrorText) ;
		return false ;
	}
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::invokeAddGraphService")) ;
  return false ;
}
}

//return the person graph (complet or patial)
//input data :  person Role (P,U or C),
//              the input Traits (login + password for patient or user, adeliNb for corresp)
bool
NautilusPilot::importPerson(const char* serviceName, NSBasicAttributeArray *pAttrArray,
                                         NSDataGraph* pGraph )
//===================================================================================
{
	if (!serviceName || !pAttrArray || !pGraph)
		return false ;

	JNIEnv* jenv = GetJniEnv() ;
	if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    traceAndDisplayError(sErrorText) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if ((jobjectArray) NULL == strArray)
	{
  	traceAndDisplayError(string("NautilusPilot::importPerson : createJObjectArray failed. Exiting...")) ;
    return false ;
	}

	jstring jServiceName = WindowsTojstring(jenv, serviceName) ;
	jobject jGraph = jenv->CallObjectMethod(jNautilusPilot, midSimpInvoke, jServiceName, strArray) ;

  jenv->DeleteLocalRef(jServiceName) ;
  jenv->DeleteLocalRef(strArray) ;

	if (jenv->ExceptionOccurred())
	{
  	const char* text = getTextError(jenv, "NTIERS", true) ;
    if(text != NULL)
    	errorMessage = string(text) ;
    jenv->DeleteLocalRef(jGraph) ;
    pGraph->graphReset() ;
    return false ;
  }
  else
  	if ((jGraph == NULL) && ( getReturnType(jenv) == WARNING_RETURN))                //WARNING_RETURN
    {
    	warningMessage = getTextWarning(jenv, "NTIERS") ;
      jenv->DeleteLocalRef(jGraph) ;
      return false ;
    }
    else
    	if (jGraph == NULL)
      {
      	const char* text = getTextError(jenv, "NTIERS", true) ;
        if (text != NULL)
        	errorMessage = string(text) ;
        jenv->DeleteLocalRef(jGraph) ;
        pGraph->graphReset() ;
        return false ;
      }
	pGraph->graphReset() ;
	// build a C graph from the java graph object
	//===========================================
	buildGraphForC(jenv, jGraph, pGraph) ;

	jenv->DeleteLocalRef(jGraph) ;
	return true ;
}

bool
NautilusPilot::importNewDataGraphPerson(const char* serviceName,
                            NSBasicAttributeArray *pAttrArray, NSDataGraph* pGraph )
//===================================================================================
{
	if (!serviceName || !pAttrArray || !pGraph)
  	return false ;

	JNIEnv* jenv = GetJniEnv() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if (NULL == strArray)
	{
  	string ps = string("NautilusPilot::importNewDataGraphPerson : createJObjectArray failed. Exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    return false ;
	}

	//=== execute a NautilusGraph.readGraph method
	jobject jGraph ;
	jstring jServiceName = WindowsTojstring(jenv, serviceName) ;
	jGraph = jenv->CallObjectMethod(jNautilusPilot, midSimpInvoke, jServiceName, strArray) ;
  jenv->DeleteLocalRef(jServiceName);
  jenv->DeleteLocalRef(strArray);
	if (NULL == jGraph)
	{
		string sErrorText = getTextError(jenv) ;
		erreur(sErrorText.c_str(), standardError) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		return false ;
	}
	pGraph->graphReset() ;
	// build a C graph from the java graph object
	//===========================================
	buildGraphForC(jenv, jGraph, pGraph) ;

	jenv->DeleteLocalRef(jGraph) ;
	return true ;
}

bool
NautilusPilot::exportNewDataGraphPerson(const char* serviceName,
                                                    NSBasicAttributeArray *pAttrArray)
//====================================================================================
{
try
{
	if (!serviceName || ((NSBasicAttributeArray*) NULL == pAttrArray))
  	return false ;

	JNIEnv* jenv = GetJniEnv() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if (NULL == strArray)
	{
  	string ps = string("NautilusPilot::exportNewDataGraphPerson : createJObjectArray failed. Exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeSimpleService",
                            "(Ljava/lang/String;[Ljava/lang/String;)Z");
	if ((0 == mid) || jenv->ExceptionCheck())
	{
		fprintf(stderr, "Cannot locate the NautilusPilot invokeSimpleService method.") ;
		return false ;
	}

	// jboolean jRes = reinterpret_cast<jboolean>(CallObjectMethod(jenv, jNautilusPilot, mid, jServName, strArray)) ;
  jboolean jRes = jenv->CallBooleanMethod(jNautilusPilot, mid, jServName, strArray) ;

  jenv->DeleteLocalRef(jServName) ;
  jenv->DeleteLocalRef(strArray) ;
	if (jenv->ExceptionOccurred())
	{
		string sErrorText = getTextError(jenv) ;
		erreur(sErrorText.c_str(), standardError) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		jenv->ExceptionDescribe();
		return false;
	}
	bool bRes = (bool) jRes ;
	if ((false == bRes) && (getReturnType(jenv) == WARNING_RETURN)) //WARNING_RETURN
	{
  	warningMessage = string(getTextWarning(jenv, "NTIERS")) ;
    return false ;
	}
	if (false == bRes)
	{
		errorMessage = string(getTextError(jenv, "NTIERS", true)) ;
		return false ;
	}
	return true ;
}
catch (...)
{
	traceAndDisplayError(string("Exception NautilusPilot::exportNewDataGraphPerson")) ;
	return false ;
}
}

bool
NautilusPilot::getPersonInformation(const char* serviceName, NSBasicAttributeArray *pAttrArray,
                                         NSDataGraph* pGraph )
//===================================================================================
{
try
{
	if (!serviceName || !pAttrArray || !pGraph)
  	return false ;

	JNIEnv* jenv = GetJniEnv() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if (NULL == strArray)
	{
    string ps = string("NautilusPilot::getPersonInformation : createJObjectArray failed. Exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    return false ;
	}

	jstring jServiceName = WindowsTojstring(jenv, serviceName) ;
	jobject jGraph = jenv->CallObjectMethod(jNautilusPilot, midSimpInvoke, jServiceName, strArray) ;

	jenv->DeleteLocalRef(jServiceName) ;
  jenv->DeleteLocalRef(strArray) ;
  if (jenv->ExceptionOccurred() )
  {
  	const char* text = getTextError(jenv, "NTIERS", true) ;
    if (text != NULL)
    	errorMessage = string(text) ;
    jenv->DeleteLocalRef(jGraph) ;
    pGraph->graphReset() ;
    return false ;
	}
	else
    if ((jGraph == NULL) && ( getReturnType(jenv) == WARNING_RETURN))                //WARNING_RETURN
    {
    	warningMessage = getTextWarning(jenv, "NTIERS") ;
      jenv->DeleteLocalRef(jGraph) ;
      return false ;
    }

	pGraph->graphReset() ;
	// build a C graph from the java graph object
	//===========================================
	buildGraphForC(jenv, jGraph, pGraph) ;

	jenv->DeleteLocalRef(jGraph) ;
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::getPersonInformation")) ;
  return false ;
}
}

bool
NautilusPilot::createImportedPerson(const char* serviceName, NSDataGraph* pGraph, NSPersonsAttributesArray *pList,
                                NSBasicAttributeArray *pAttrArray)
//========================================================================================================
{
try
{
	if(!serviceName || !pAttrArray || !pGraph || !pList)
		return false ;

	JNIEnv* jenv = GetJniEnv() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	int nbTrees  = 0;
	int nbLinks  = 0;
	int nbModels = 0;
	int nbRights = 0;

	// 1) build a graph for the java side
	//====================================
	jobject jGraph = buildGraphForJava(jenv, pGraph, nbTrees, nbLinks, nbModels, nbRights);
  if (NULL == jGraph)
  {
  	string ps = string("NautilusPilot::createImportedPerson : buildGraphForJava failed. Exiting...") ;
  	_pSuper->trace(&ps, 1, NSSuper::trError) ;
  	jenv->DeleteLocalRef(jGraph);
  	return false ;
  }

	if (0 == nbTrees)
  {
  	string ps = string("NautilusPilot::createImportedPerson : no trees after buildGraphForJava. Exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    jenv->DeleteLocalRef(jGraph);
		return false ;
	}

	//  "(Ljava/lang/String;[Ljava/lang/String;)Z");
	// 2) execute a NautilusPilot.createPatient method
	//================================================

	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "createImportedPerson",
                "(Lnautilus/NautilusGraph;Ljava/lang/String;[Ljava/lang/String;)Lnautilus/NautilusGraph;") ;
	if ((0 == mid) || jenv->ExceptionCheck())
	{
		erreur("Cannot locate the NautilusPilot createPatient method.", standardError) ;
		return false ;
	}
  jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if (NULL == strArray)
	{
    string ps = string("NautilusPilot::createImportedPerson : createJObjectArray failed. Exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    return false ;
	}
  jstring jServName = WindowsTojstring(jenv, serviceName) ;

  jobjectArray jObject = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, mid, jGraph, jServName, strArray)) ;
	jenv->DeleteLocalRef(strArray) ;
  jenv->DeleteLocalRef(jServName) ;

	if (jenv->ExceptionOccurred())
	{
		//erreur("Erreur execution: createPatient - service",standardError,0,0);
		string sErrorText = getTextError(jenv) ;
		erreur(sErrorText.c_str(), standardError) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		jenv->ExceptionDescribe() ;
    jenv->DeleteLocalRef(jObject);
		return false ;
	}
	if(getReturnType(jenv) == GRAPH_RETURN)    //GRAPH_RETURN
	{
		// build a C graph from the java graph object
		//===========================================
		buildGraphForC(jenv, jObject, pGraph) ;
		jenv->DeleteLocalRef(jObject) ;
	//	transformResultFieldInAttributesArray(jenv, pList, "list", "person") ;
  	transformResultFieldInAttributesArray(jenv, pList, "status", "*") ;
	}
	else
		if (getReturnType(jenv) == EMPTY_RETURN)
		{
			jenv->DeleteLocalRef(jObject) ;
			return true ;          //for createImportedUser
		}
		else
		{
			string sErrorText = getTextError(jenv) ;
			erreur(sErrorText.c_str(), standardError) ;
    	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      jenv->DeleteLocalRef(jObject) ;
			return false ;
		}

	return true ;
}
catch (...)
{
	traceAndDisplayError(string("Exception NautilusPilot::createImportedPerson")) ;
	return false ;
}
}

void
NautilusPilot::addUnavailableAgent(string sAgentName, string error)
{
  _AvailableAgentList.push_back(new NSAgentStatus(sAgentName, error)) ;
}

void
NautilusPilot::addUnavailableAgent(string sAgentName)
{
  _AvailableAgentList.push_back(new NSAgentStatus(sAgentName, string(""))) ;
}

//par default tag==agent
bool
NautilusPilot::resourceList(JNIEnv* jenv, const char* serviceName,
                            NSPersonsAttributesArray *pAgentList, string tag)
//========================================================================================
{
	if (((const char*)               NULL == serviceName) ||
      ((NSPersonsAttributesArray*) NULL == pAgentList) ||
      ((JNIEnv*)                   NULL == jenv))
  	return false ;

try
{
	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "resourceList",
      "(Ljava/lang/String;)Ldomlightwrapper/DOMElement;") ;
	if (JniExceptionCheck(jenv) || (0 == mid))
	{
  	string sErrorText = _pSuper->getText("NTIERS", "resourceListNotFound") ;
    traceAndDisplayError(sErrorText) ;
		return false ;
	}

	jstring jServName = WindowsTojstring(jenv, serviceName) ;
	jobject jDOMElem = jenv->CallObjectMethod(jNautilusPilot, mid, jServName) ;

  jenv->DeleteLocalRef(jServName) ;
  if (jDOMElem == NULL)
  {
  	errorMessage = getTextError(jenv) ;
    jenv->DeleteLocalRef(jDOMElem) ;
    return false ;
	}

	// jstring jNodeName = jenv->NewStringUTF( "agent" );
  jstring jNodeName = WinStringTojstring(jenv, tag) ;
  jobjectArray jDomElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jDOMElem, midChildrenElements, jNodeName)) ;
  jenv->DeleteLocalRef(jNodeName) ;
  if (jDomElems == NULL)
  {
  	traceAndDisplayError(string("NautilusPilot::objectList : CallObjectMethod failed for midChildrenElements. exiting...")) ;
    return true ;
	}

  int count = jenv->GetArrayLength(jDomElems) ;
  for (int i = 0 ; i < count ; i++)
	{
  	jobject jNodeElem = jenv->GetObjectArrayElement(jDomElems, i) ;
    if ((jobject) NULL != jNodeElem)
    {
    	NSBasicAttributeArray* pAttributes = new NSBasicAttributeArray() ;
      DOMTreeToBasicAttribute(jenv, pAttributes, jNodeElem) ;
      jenv->DeleteLocalRef(jNodeElem) ;
      pAgentList->push_back(pAttributes) ;
    }
    else
    {
    	traceAndDisplayError(string("NautilusPilot::personList : GetObjectArrayElement failed.")) ;
    }
	}
	jenv->DeleteLocalRef(jDomElems) ;
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::resourceList")) ;
  return false ;
}
}

bool
NautilusPilot::setServiceList()
//==============================
{
try
{
  JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
  {
		traceAndDisplayError(string("NautilusPilot::setServiceList : jenv is null. Exiting...")) ;
    return false ;
  }

  NSPersonsAttributesArray ServiceList ;

	if (false == resourceList(jenv, SERV_GET_NEEDED_AGENT_LIST.c_str(), &ServiceList, "service"))
  {
    EndPilotMethod(jenv) ;
  	return false ;
  }

	if (ServiceList.empty())
  {
    EndPilotMethod(jenv) ;
  	return false ;
  }

  NSPersonsAttributeIter iter = ServiceList.begin() ;

  while (ServiceList.end() != iter)
  {
    string sServiceName = (*iter)->getAttributeValue("name") ;
    if (string("") != sServiceName)
    {
      NSPilotService *pService = new NSPilotService(sServiceName) ;
      //test sans valeur le nom est deja testé, mais.....
      if (false == (*iter)->empty())
      {
        VecteurString* pAgentList = pService->getAgentList() ;
        if (pAgentList)
        {
          NSBasicAttributeIter iterProps = (*iter)->begin() ;
          for ( ; (*iter)->end() != iterProps ; iterProps++)
            if (((*iterProps)->getText() != string("")) &&
                ((*iter)->getAttributeValue("agent") != string("")))
              pAgentList->AddString((*iterProps)->getText()) ;
        }
      }
      _PropertiesServicesList.push_back(pService) ;
    }
    iter++ ;
  }

  EndPilotMethod(jenv) ;

  return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::setServiceList")) ;
  return false ;
}
}

bool
NautilusPilot::traitList(JNIEnv* jenv, const char* serviceName, NSPersonsAttributesArray *pTraitList)
//====================================================================================
{
	if (((const char*)               NULL == serviceName) ||
      ((NSPersonsAttributesArray*) NULL == pTraitList) ||
      ((JNIEnv*)                   NULL == jenv))
  	return false ;

try
{
	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "resourceList",
                          "(Ljava/lang/String;)Ldomlightwrapper/DOMElement;") ;
	if (JniExceptionCheck(jenv) || (0 == mid))
	{
  	string sErrorText = _pSuper->getText("NTIERS", "resourceListNotFound") ;
    traceAndDisplayError(sErrorText) ;
		return false ;
	}

	jstring jServName = WinStringTojstring(jenv, serviceName) ;
  jobjectArray jDOMElems = reinterpret_cast<jobjectArray>(jenv->CallObjectMethod(jNautilusPilot, mid, jServName)) ;
  if (JniExceptionCheck(jenv) || (NULL == jDOMElems))
  {
  	errorMessage = getTextError(jenv) ;
    jenv->DeleteLocalRef(jServName) ;
    if (jDOMElems)
      jenv->DeleteLocalRef(jDOMElems) ;
    return false ;
	}

  jenv->DeleteLocalRef(jServName) ;

  int count = jenv->GetArrayLength(jDOMElems) ;
  for (int i = 0 ; i < count ; i++)
  {
  	jobject jNodeElem = jenv->GetObjectArrayElement(jDOMElems, i) ;
    if ((false == JniExceptionCheck(jenv)) && (NULL != jNodeElem))
    {
    	NSBasicAttributeArray* pAttributes = new NSBasicAttributeArray() ;
      DOMTreeToBasicAttribute(jenv, pAttributes, jNodeElem) ;
      jenv->DeleteLocalRef(jNodeElem) ;
      pTraitList->push_back(pAttributes) ;
    }
    else
    	traceAndDisplayError(string("NautilusPilot::traitList : GetObjectArrayElement failed.")) ;
  }

  jenv->DeleteLocalRef(jDOMElems) ;

	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::traitList")) ;
  return false ;
}
}

bool
NautilusPilot::runGarbageCollector()
//==================================
{
try
{
	JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
	{
		string sErrorText = _pSuper->getText("javaError", "NULLJVM") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

  string sMsg = string("Pilot: Calling garbage collector") ;
  _pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

	jstring jServName = WindowsTojstring(jenv, "runGarbageCollector") ;
	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeServiceWithoutParams",
                                        "(Ljava/lang/String;)Z");
	if ((0 == mid) || jenv->ExceptionCheck())
  {
  	string sErrorText = _pSuper->getText("NTIERS", "invokeServiceWithoutParams") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
    EndPilotMethod(jenv) ;
		return false ;
	}

	// jboolean jRes = reinterpret_cast<jboolean>(CallObjectMethod(jenv, jNautilusPilot, mid, jServName)) ;
  jboolean jRes = jenv->CallBooleanMethod(jNautilusPilot, mid, jServName) ;

  jenv->DeleteLocalRef(jServName) ;
  if (jenv->ExceptionOccurred() )
	{
		string sErrorText = getTextError(jenv) ;
		erreur(sErrorText.c_str(), standardError) ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		jenv->ExceptionDescribe() ;
    EndPilotMethod(jenv) ;
		return false ;
	}

  EndPilotMethod(jenv) ;

	return jRes ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::runGarbageCollector")) ;
  return false ;
}
}

bool
NautilusPilot::fusionPatient(NSBasicAttributeArray *pAttrArray)
//=============================================================
{
try
{
	JNIEnv* jenv = StartPilotMethod() ;
  if ((JNIEnv*) NULL == jenv)
	{
		errorMessage = _pSuper->getText("javaError", "NULLJVM") ;
		return false ;
	}

	jmethodID mid = jenv->GetMethodID(classNautilusPilot, "invokeSimpleServiceStringReturn",
                                        "(Ljava/lang/String;[Ljava/lang/String;)Ljava/lang/String;") ;
	if ((0 == mid) || jenv->ExceptionCheck())
  {
    errorMessage = _pSuper->getText("NTIERS", "invokeSimpleServiceStringReturn") ;
    EndPilotMethod(jenv) ;
		return false ;
	}

	jobjectArray strArray = createJObjectArray(jenv, pAttrArray) ;
  if (NULL == strArray)
	{
    string ps = string("NautilusPilot::createImportedPerson : createJObjectArray failed. Exiting...") ;
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    EndPilotMethod(jenv) ;
    return false ;
	}
  jstring jServName = WindowsTojstring(jenv, "fusionPatient") ;
	jstring jResult = reinterpret_cast<jstring>(jenv->CallObjectMethod(jNautilusPilot, mid, jServName, strArray)) ;
  jenv->DeleteLocalRef(strArray) ;
  jenv->DeleteLocalRef(jServName) ;
  if ((jenv->ExceptionOccurred() ) || (jResult == NULL))
	{
    errorMessage = getTextError(jenv) ;
		jenv->ExceptionDescribe() ;
    EndPilotMethod(jenv) ;
    return false ;
  }

	char *result = JavaSystem::NewStringWithSpecialCharacters(jResult) ;
  jenv->DeleteLocalRef(jResult) ;
  if (NULL != result)
  {
    string res = string(result) ;
    if (string("ok") != res)
    {
      errorMessage = res ;
      EndPilotMethod(jenv) ;
      return false ;
    }
  }
  EndPilotMethod(jenv) ;
	return true ;
}
catch (...)
{
  traceAndDisplayError(string("Exception NautilusPilot::fusionPatient")) ;
  return false ;
}
}

// -----------------------------------------------------------------------------
// class JniManager
// -----------------------------------------------------------------------------
JniManager::JniManager(NSSuper* pSuper)
           :NSSuperRoot(pSuper)
{
  _bJavaOk = false ;
}

JniManager::~JniManager()
{
}

bool
JniManager::InitialiseJava(){  //  // First, make certain that error/warning messages won't be hidden by the
  // splash screen
  //
  HWND hWnd = 0 ;
  TSplashWindow* pSplashWin = _pSuper->getSplashWindow() ;
  if (pSplashWin)
  {
    hWnd = pSplashWin->GetHandle() ;
	  _pSuper->sendMessageToSplash(string("Init java")) ;    _pSuper->setSplashPercentDone(10) ;  }	string ps = string("Java init - begin") ;	_pSuper->trace(&ps, 1, NSSuper::trSteps) ;	ps = string("Java init : opening file pilot.dat") ;	_pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;	ifstream inFile ;	inFile.open("pilot.dat") ;	if (!inFile)
	{
		string sErreur = _pSuper->getText("pilotManagement", "cannotOpenPilot.dat") ;
    _pSuper->trace(&sErreur, 1, NSSuper::trError) ;
		erreur(sErreur.c_str(), standardError, 0, hWnd) ;
    _bJavaOk = false ;
		return false ;
	}
	inFile.close() ;
	ps = string("Java init : JavaSystem::Init") ;	_pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;	//=== Pilot initialisations
	char* propertyFile = NULL ; //by default Pilot.properties
	if (JavaSystem::Init(this, "pilot.properties", false))
	{
		string sErreur = _pSuper->getText("pilotManagement", "cannotInitJavaMachine") ;
    _pSuper->trace(&sErreur, 1, NSSuper::trError) ;
		erreur(sErreur.c_str(), standardError, 0, hWnd) ;
    _bJavaOk = false ;
		return false ;	}

	ps = string("Java init : Pilot::Init") ;
	_pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;

	if (propertyFile)
  	ps = string("property file : ") + string(propertyFile) ;
	else
		ps = string("property file : NULL") ;
	_pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	int stat = Pilot::Init(propertyFile) ;
	if (stat < 0)
	{
  	string sErreur = _pSuper->getText("pilotManagement", "cannotInitPilot") ;
    _pSuper->trace(&sErreur, 1, NSSuper::trError) ;
		erreur(sErreur.c_str(), standardError, 0, hWnd) ;
  	switch (stat)
    {
      case -1  : ps = string("Cannot locate the pilot/Pilot class. Exiting....") ; break ;
      case -2  : ps = string("Cannot locate the setParametersFromFile method. Exiting...") ; break ;
      case -3  : ps = string("Cannot locate the init method. Exiting...") ; break ;
      case -4  : ps = string("jenv is NULL. Exiting...") ; break ;
      default  : ps = string("Unknown error.") ;
    }
    _pSuper->trace(&ps, 1, NSSuper::trError) ;
    _bJavaOk = false ;
		return false ;
	}

	char szInfo[33] ;
	itoa(stat, szInfo, 10) ;
	ps = string("Pilot init - nb of agents : ") + string(szInfo) ;
	_pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	//use defaultServiceDirectory
	stat = Pilot::AddServicesFromDirectory(NULL) ;
	if (!stat)
	{
		string sErreur = _pSuper->getText("pilotManagement", "cannotAddServices") ;
    _pSuper->trace(&sErreur, 1, NSSuper::trError) ;
		erreur(sErreur.c_str(), standardError, 0, hWnd) ;
	}	itoa(stat, szInfo, 10) ;	ps = string("Pilot init - nb of services : ") + string(szInfo) ;
	_pSuper->trace(&ps, 1, NSSuper::trDetails) ;
	ps = string("Java init : NautilusPilot::Init") ;
	_pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;

	stat = NautilusPilot::Init() ;
	if (stat)
	{
		itoa(stat, szInfo, 10) ;
		string sErreur = _pSuper->getText("pilotManagement", "cannotInitNautilusPilot") + string(" ") + szInfo ;
    _pSuper->trace(&sErreur, 1, NSSuper::trError) ;
		erreur(sErreur.c_str(), standardError, 0, hWnd) ;

		if ( stat < 0 )
		{
			string sErreur = _pSuper->getText("pilotManagement", "cannotInitPilot") ;
      _pSuper->trace(&sErreur, 1, NSSuper::trError) ;
      switch (stat)
      {
        case -1  : ps = string("Cannot locate the nautilus/NautilusPilot class. Exiting...") ; break ;
        case -2  : ps = string("Cannot locate the nautilus/NautilusGraph class. Exiting...") ; break ;
        case -3  : ps = string("Cannot locate the graphServer/Node class. Exiting...") ; break ;
        case -4  : ps = string("Cannot locate the graphServer/Mapping class. Exiting...") ; break ;
        case -5  : ps = string("Cannot locate the domlightwrapper/DOMElement class. Exiting...") ; break ;
        case -6  : ps = string("Cannot locate the NautilusPilot readGraph method. Exiting...") ; break ;
        case -7  : ps = string("Cannot locate the NautilusPilot writeGraph method. Exiting...") ; break ;
        case -8  : ps = string("Cannot locate the NautilusPilot invokeService method V1. Exiting...") ; break ;
        case -9  : ps = string("Cannot locate the NautilusPilot invokeService method V2. Exiting...") ; break ;
        case -10 : ps = string("Cannot locate the NautilusGraph constructor method. Exiting...") ; break ;
        case -11 : ps = string("Cannot locate the NautilusGraph getTreeId method. Exiting...") ; break ;
        case -12 : ps = string("Cannot locate the NautilusGraph getGraphId method. Exiting...") ; break ;
        case -13 : ps = string("Cannot locate the the java/lang/String class. Exiting...") ; break ;
        case -14 : ps = string("Cannot locate the NautilusPilot init method. Exiting...") ; break ;
        case -15 : ps = string("Cannot get FieldId for mnemo. Exiting...") ; break ;
        case -16 : ps = string("Cannot get FieldId for stack. Exiting...") ; break ;
        case -17 : ps = string("Cannot locate the DOMElement getChildrenElements method. Exiting...") ; break ;
        default  : ps = string("Unknown error.") ;
      }
      _pSuper->trace(&ps, 1, NSSuper::trError) ;

      erreur(sErreur.c_str(), standardError, 0, hWnd) ;
		}
	  _bJavaOk = false ;
		return false ;
	}
  _bJavaOk = true ;
	return true ;
}

void
JniManager::DestroyJava()
{
  string ps = string("Java destroy - begin") ;
	_pSuper->trace(&ps, 1, NSSuper::trSteps) ;

  _bJavaOk = false ;
  JavaSystem::Close(this) ;

  ps = string("Java destroy - done") ;
	_pSuper->trace(&ps, 1, NSSuper::trSteps) ;
}

bool
JniManager::ResetJava()
{
  string ps = string("Reseting Java") ;
	_pSuper->trace(&ps, 1, NSSuper::trSteps) ;

  DestroyJava() ;
  return InitialiseJava() ;
}

// -----------------------------------------------------------------------------
// class NSBasicAttributeArray
// -----------------------------------------------------------------------------
NSBasicAttributeArray::NSBasicAttributeArray(const NSBasicAttributeArray& rv)
                      :NSBasicAttributeVector()
{
try
{
	if (false == rv.empty())
		for (NSBasicAttributeCIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSBasicAttribute(**i)) ;

	lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSBasicAttributeArray copy ctor", standardError) ;
}
}

void
NSBasicAttributeArray::vider()
{
	if (empty())
		return ;

	for (NSBasicAttributeIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSBasicAttributeArray::~NSBasicAttributeArray()
{
	vider() ;
	lObjectCount-- ;
}

NSBasicAttributeArray&
NSBasicAttributeArray::operator=(const NSBasicAttributeArray& src)
{
try
{
	if (this == &src)
  	return (*this) ;

	vider() ;

	if (false == src.empty())
		for (NSBasicAttributeCIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSBasicAttribute(**i)) ;

	return (*this) ;
}
catch (...)
{
	erreur("Exception NSBasicAttributeArray = operator", standardError) ;
	return (*this) ;
}
}

string
NSBasicAttributeArray::getAttributeValue(string sBalise)
{
  NSBasicAttribute* pBalise = getAttribute(sBalise) ;
  if (pBalise)
		return pBalise->getText() ;

  return string("") ;
}

void
NSBasicAttributeArray::setAttributeValue(string sBalise, string sText)
{
try
{
  NSBasicAttribute* pBalise = getAttribute(sBalise) ;
  if (pBalise)
    pBalise->setText(sText) ;
  else
	  push_back(new NSBasicAttribute(sBalise, sText)) ;
}
catch (...)
{
	erreur("Exception NSBasicAttributeArray::setAttributeValue", standardError) ;
}
}

void
NSBasicAttributeArray::changeAttributeValue(string sBalise, string sText)
{
try
{
	if (false == empty())
	{
		NSBasicAttributeIter iter = begin() ;
		for	( ; ((end() != iter) && ((*iter)->getBalise() != sBalise)) ; iter++) ;

		if (end() != iter)
		{
    	// si la balise a été modifiée, on la conserve
      // sinon on la supprime
      if ((*iter)->getText() == sText)
      {
      	delete (*iter) ;
        erase(iter) ;
      }
      else
      	(*iter)->setText(sText) ;
			return ;
		}
	}

    // si la balise n'existait pas, on la rajoute
	push_back(new NSBasicAttribute(sBalise, sText)) ;
}
catch (...)
{
	erreur("Exception NSBasicAttributeArray::setAttributeValue", standardError) ;
}
}

void
NSBasicAttributeArray::eraseAttributeValue(string sBalise)
{
	if (empty())
		return ;

	NSBasicAttributeIter iter = begin() ;
	for	( ; ((end() != iter) && ((*iter)->getBalise() != sBalise)) ; iter++) ;

	if (end() != iter)
	{
  	delete (*iter) ;
  	erase(iter) ;
	}
}

NSBasicAttribute*
NSBasicAttributeArray::getAttribute(string sBalise)
{
  if (empty())
		return (NSBasicAttribute*) 0 ;

	NSBasicAttributeIter iter = begin() ;
	for	( ; ((end() != iter) && ((*iter)->getBalise() != sBalise)) ; iter++) ;

	if (end() == iter)
    return (NSBasicAttribute*) 0 ;

  return *iter ;
}

//----------------------- class NSPersonsAttributesArray ---------------------

NSPersonsAttributesArray::NSPersonsAttributesArray(const NSPersonsAttributesArray& rv)
                         :NSBasicAttributeArrayVector()
{
try
{
	if (false == rv.empty())
		for (NSPersonsAttributeConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSBasicAttributeArray(*(*i))) ;

	lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSPersonsAttributesArray copy ctor", standardError) ;
}
}

string
NSPersonsAttributesArray::getAttributeValue(string sBalise)
{
	if (empty())
		return string("") ;

	string sResult = "" ;
	for (NSPersonsAttributeIter i = begin(); (end() != i) && (string("") == sResult); i++)
		sResult = (*i)->getAttributeValue(sBalise) ;

	return sResult ;
}

void
NSPersonsAttributesArray::vider()
{
	if (empty())
		return ;

	for (NSPersonsAttributeIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSPersonsAttributesArray::~NSPersonsAttributesArray()
{
	vider() ;

	lObjectCount-- ;
}

NSPersonsAttributesArray&
NSPersonsAttributesArray::operator=(const NSPersonsAttributesArray& src)
{
try
{
  if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (NSPersonsAttributeConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSBasicAttributeArray(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSPersonsAttributesArray = operator", standardError) ;
	return *this ;
}
}

//----------------------------- class NSAgentStatus ----------------------------
NSAgentStatus::NSAgentStatus()
{
	sAgent 			 = string("") ;
  sError  		 = string("") ;
  sProperties  = string("") ;
  status 			 = NSAgentStatus::active ;
  pServiceList = new VecteurString() ;
}

NSAgentStatus::NSAgentStatus(string sAgentName, string sErr )
{
	sAgent 			 = sAgentName ;
	sError  		 = sErr ;
	sProperties  = string("") ;
	status 			 = NSAgentStatus::active ;
	pServiceList = new VecteurString() ;
}

NSAgentStatus::~NSAgentStatus()
{
	if (pServiceList)
  	delete pServiceList ;
}

void
NSAgentStatus::setServiceList(VecteurString* pVect)
{
  if (pServiceList)
  	delete pServiceList ;

  pServiceList = pVect ;
}

void
NSAgentStatus::addService(string service)
{
  if ((VecteurString*) NULL == pServiceList)
    pServiceList = new VecteurString() ;

  pServiceList->AddString(service) ;
}

bool
NSAgentStatus::isServiceInAgentList(string sServiceName)
{
  if (((VecteurString*) NULL == pServiceList) || pServiceList->empty())
    return false ;

  EquiItemIter iterStr = pServiceList->begin() ;
  for ( ; ((pServiceList->end() != iterStr) && ((**iterStr) != sServiceName)) ; iterStr++) ;
	if (pServiceList->end() != iterStr)
  	return true ;

  return false ;
}

bool
NSAgentStatus::isLdvTypeAgent()
{
	if (strstr(sProperties.c_str(), LDV_AGENT_TEXT) != NULL)
   	return true ;
  return false ;
}

bool
NSAgentStatus::isLocalTypeAgent()
{
  if ((sProperties == LOCAL_AGENT) ||
  		(sProperties == DIRECT_GROUP_AGENT) ||
      (sProperties == DIRECT_COLLECTIVE))
		return true ;

  return false ;
}

bool
NSAgentStatus::isUnknownTypeAgent()
{
  if ((false == isLocalTypeAgent()) ||
      (false == isLdvTypeAgent()))
    return true ;

  return false ;
}

int
NSAgentStatus::getAgentType()
{
	if (isLocalTypeAgent())
  	return local ;
  if (isLdvTypeAgent())
  	return LdV ;
  return unknown ;
}

//------------------------- class NSAgentStatusArray --------------------------

void
NSAgentStatusArray::vider()
{
	if (empty())
		return ;
	for (NSAgentStatusIter i = begin(); i != end(); )
	{
		delete *i ;
		erase(i) ;
	}
}

NSAgentStatusArray::~NSAgentStatusArray()
{
	vider() ;
}
bool
NSAgentStatusArray::isAvailableService(string sServiceName)
{
	if (empty())
		return true ;

	for (NSAgentStatusIter iterAgent = begin() ; end() != iterAgent ; iterAgent++)
	{
    VecteurString* pAgentServiceList = (*iterAgent)->getServiceList() ;

  	if (pAgentServiceList && (false == pAgentServiceList->empty()))
    {
    	EquiItemIter iterStr = pAgentServiceList->begin() ;
      for ( ; ((pAgentServiceList->end() != iterStr) && ((**iterStr) != sServiceName)) ; iterStr++) ;

      if (pAgentServiceList->end() != iterStr)
      	return false ;
    }
	}

	return true ;
}

string
NSAgentStatusArray::getAgentError(string sAgentName)
{
	if (empty())
		return string("") ;

  NSAgentStatusIter iterAgent = begin() ;
	for ( ; ((end() != iterAgent) && ((*iterAgent)->getAgentName() != sAgentName)) ; iterAgent++) ;

  if (end() != iterAgent)
    return (*iterAgent)->getError() ;

	return string("") ;
}

bool
NSAgentStatusArray::setAgentError(string sAgentName, string sErr)
{
	if (empty())
		return false ;
    
  NSAgentStatusIter iterAgent = begin() ;
	for ( ; ((end() != iterAgent) && ((*iterAgent)->getAgentName() != sAgentName)) ; iterAgent++) ;
  if (end() != iterAgent)
  {
  	(*iterAgent)->setError(sErr) ;
    (*iterAgent)->setInactive() ;
    return true ;
  }

  return false ;
}

NSAgentStatus*
NSAgentStatusArray::selectAgent(string sAgentName)
{
	if (empty())
		return (NSAgentStatus*) 0 ;

  NSAgentStatusIter iterAgent = begin() ;
	for ( ; ((end() != iterAgent) && ((*iterAgent)->getAgentName() != sAgentName)) ; iterAgent++) ;

  if (end() != iterAgent)
    return (*iterAgent) ;

	return (NSAgentStatus*) 0 ;
}

void
NSAgentStatusArray::addAgent(string sAgentName, string sErr, VecteurString *pVect)
{
	NSAgentStatus *pNewAgent = new NSAgentStatus(sAgentName, sErr) ;
  pNewAgent->setServiceList(pVect) ;

  push_back(pNewAgent) ;
}

void
NSAgentStatusArray::addAgent(string sAgentName, string sErr)
{
	push_back(new NSAgentStatus(sAgentName, sErr)) ;
}

bool
NSAgentStatusArray::addUnavailableServiceForAgent(string sAgent, string sServiceName)
{
	if (empty())
	{
  	VecteurString *pServList = new VecteurString() ;
    pServList->AddString(sServiceName) ;
    addAgent(sAgent, string(""), pServList) ;
    return true ;
	}

  NSAgentStatusIter iterAgent = begin() ;
	for ( ; ((end() != iterAgent) && ((*iterAgent)->getAgentName() != sAgent)) ; iterAgent++) ;

  if (end() != iterAgent)
  {
  	(*iterAgent)->addService(sServiceName) ;
    return true ;
	}

	return false ;
}

void
NSAgentStatusArray::deleteAgent(string sAgentName)
{
	if (empty())
		return ;

	for (NSAgentStatusIter iter = begin() ; end() != iter ; iter++)
	{
  	if ((*iter)->getAgentName() == sAgentName)
		{
    	delete *iter ;
      erase(iter) ;
      return ;
    }
	}
}

//*****************************services management*****************************

NSPilotService::NSPilotService()
{
	_sServiceName = string("") ;
}

NSPilotService::NSPilotService(string sServ)
{
	_sServiceName = sServ ;
}

NSPilotService::~NSPilotService()
{
}

bool
NSPilotService::isAgentNeededInService(string	sAgentName)
{
  if (_aAgentList.empty())
    return false ;

  EquiItemIter iterStr = _aAgentList.begin() ;
  for ( ; (_aAgentList.end() != iterStr) && (**iterStr != sAgentName) ; iterStr++) ;

	if (_aAgentList.end() != iterStr)
  	return true ;

  return false ;
}

//------------------------- class NSServiceStatusArray --------------------------

void
NSServiceStatusArray::vider()
{
	if (empty())
		return ;

	for (NSPilotServiceStatusIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSServiceStatusArray::~NSServiceStatusArray()
{
	vider() ;
}

VecteurString*
NSServiceStatusArray::getAgentListForService(string sName)
{
	if (empty())
  	return (VecteurString*) 0 ;

  NSPilotServiceStatusIter iter = begin() ;
  for ( ; ((end() != iter) && ((*iter)->getServiceName() != sName)) ; iter++ ) ;
  if (end() != iter)
  	return (*iter)->getAgentList() ;

  return (VecteurString*) 0 ;
}

//
// -------------------- NSBasicAttribute --------------------
//
NSBasicAttribute::NSBasicAttribute()
{
  _balise = string("") ;
  _text   = string("") ;
  lObjectCount++ ;
}

NSBasicAttribute::NSBasicAttribute(string sBalise, string sText)
{
  _balise = sBalise ;
  _text   = sText ;
  lObjectCount++ ;
}

NSBasicAttribute::NSBasicAttribute(const NSBasicAttribute& rv)
{
  _balise = rv._balise ;
  _text   = rv._text ;
  lObjectCount++ ;
}

NSBasicAttribute::~NSBasicAttribute()
{
  lObjectCount-- ;
}

NSBasicAttribute&
NSBasicAttribute::operator=(const NSBasicAttribute& src)
{
  if (this == &src)
  	return *this ;

  _balise = src._balise ;
  _text   = src._text ;

  return *this ;
}

