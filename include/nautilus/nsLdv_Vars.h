//---------------------------------------------------------------------------//     NSLDV_VARS.H
//
//  Contient les définitions des objets annexes de NAUTILUS
//
//  1ere version : PA juillet 94   Dernière modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSLDV_VARS_H
#define __NSLDV_VARS_H

#include "nssavoir\nsRootLink.h"

#define FRAMECOUNT 3

enum LDVFRAME         { ldvframeNotSpecified = -1,
                          ldvframeHealth = 0,
                          ldvframeRisk,
                          ldvframeSocial
                        } ;

int getIndexForFrame(LDVFRAME iFrame)
{
  switch(iFrame)
  {
    case ldvframeHealth : return 0 ;
    case ldvframeRisk   : return 1 ;
    case ldvframeSocial : return 2 ;
  }
  return -1 ;
}

LDVFRAME getFrameForIndex(int iIndex)
{
  switch(iIndex)
  {
    case 0 : return ldvframeHealth ;
    case 1 : return ldvframeRisk ;
    case 2 : return ldvframeSocial ;
  }
  return ldvframeNotSpecified ;
}

string getRootForFrame(LDVFRAME iFrame)
{
  switch(iFrame)
  {
    case ldvframeHealth : return string("ZPOMR1") ;
    case ldvframeRisk   : return string("ORISK1") ;
    case ldvframeSocial : return string("ZPSOC1") ;
  }
  return string("") ;
}

// For lines naming (for example to add, "risk of" for risk frame lines)
//
string getRootLabelForFrame(LDVFRAME iFrame)
{
  switch(iFrame)
  {
    case ldvframeHealth : return string("") ;
    case ldvframeRisk   : return string("ORISK1") ;
    case ldvframeSocial : return string("") ;
  }
  return string("") ;
}

string getLabelForFrame(LDVFRAME iFrame)
{
  switch(iFrame)
  {
    case ldvframeHealth : return string("healthIndex") ;
    case ldvframeRisk   : return string("riskFollowup") ;
    case ldvframeSocial : return string("socialIndex") ;
  }
  return string("") ;
}

NSRootLink::NODELINKTYPES getLinkForFrame(LDVFRAME iFrame)
{
  switch(iFrame)
  {
    case ldvframeHealth : return NSRootLink::personHealthIndex ;
    case ldvframeRisk   : return NSRootLink::personRiskManager ;
    case ldvframeSocial : return NSRootLink::personSocialIndex ;
  }
  return NSRootLink::badLink ;
}

//---------------------------------------------------------------------------
#endif    // __NSANNEXE_H
