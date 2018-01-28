
#include "ns_ob1\NSKS.h"

NSKS::NSKS()
{
  pKSModule = 0 ;
  sLabel    = string("") ;
}

NSKS::~NSKS()
{
#ifdef _ENTERPRISE_DLL
  if (pKSModule)
    PR_UnloadLibrary(pKSModule) ;
#else
  if (pKSModule)
    delete pKSModule ;
#endif
} ;

