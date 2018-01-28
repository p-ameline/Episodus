//----------------------------------------------------------------------------// CAPTURE D'IMAGES//----------------------------------------------------------------------------
#ifndef __IDAPI_PROXY_H#define __IDAPI_PROXY_H
#include <owl/module.h>

#define DBI_INIT                 2
#define DBI_EXIT                 3
#define DBI_OPEN_DATABASE        4
#define DBI_CLOSE_DATABASE       5
#define DBI_DEBUGLAYER_OPTIONS 508

bool initIdapiProxy() ;
bool closeIdapiProxy() ;

#endif  // __IDAPI_PROXY_H

