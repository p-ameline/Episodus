//----------------------------------------------------------------------------
//                  Ligne de vie - Vue du modèle Document/Vue
//----------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <algorithm>
#ifndef _ENTERPRISE_DLL
  #include <owl\pch.h>
#endif

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
#else
  #include "nautilus\nssuper.h"
#endif

#include "nsepisod\nsldvstructs.h"

// -----------------------------------------------------------------
//                       NewConcernInfo
// -----------------------------------------------------------------

NewConcernInfo::NewConcernInfo(NSSuper* pSuper)
               :NSSuperRoot(pSuper), _pptDetails(pSuper)
{
  _iFrame    = ldvframeNotSpecified ;
  _sLexique  = string("") ;
  _sTL       = string("") ;
  _sDateDeb  = string("") ;
  _sDateFin  = string("") ;
  _iSeverity = 0 ;
  _sCocCode  = string("") ;
  _sCimCode  = string("") ;
}

NewConcernInfo::NewConcernInfo(const NewConcernInfo& rv)
               :NSSuperRoot(rv._pSuper), _pptDetails(rv._pptDetails)
{
  _iFrame     = rv._iFrame ;
  _sLexique   = rv._sLexique ;
  _sTL        = rv._sTL ;
  _sDateDeb   = rv._sDateDeb ;
  _sDateFin   = rv._sDateFin ;
  _iSeverity  = rv._iSeverity ;
  _sCocCode   = rv._sCocCode ;
  _sCimCode   = rv._sCimCode ;
}

NewConcernInfo::~NewConcernInfo()
{
}

NewConcernInfo&
NewConcernInfo::operator=(const NewConcernInfo& src)
{
  if (this == &src)
  	return *this ;

  _iFrame     = src._iFrame ;
  _sLexique   = src._sLexique ;
  _sTL        = src._sTL ;
  _sDateDeb   = src._sDateDeb ;
  _sDateFin   = src._sDateFin ;
  _iSeverity  = src._iSeverity ;
  _sCocCode   = src._sCocCode ;
  _sCimCode   = src._sCimCode ;
  _pptDetails = src._pptDetails ;

  return *this ;
}

