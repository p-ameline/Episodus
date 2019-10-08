//----------------------------------------------------------------------------
//   Programme protégé par une licence Open Source GPL
//   NAUTILUS - www.nautilus-info.com
//----------------------------------------------------------------------------
#ifndef __NSLDVVUE_H
# define __NSLDVVUE_H

// # include <owl\owlpch.h>
// # include <owl\dc.h>
// # include <owl\inputdia.h>
// # include <owl\chooseco.h>
// # include <owl\gdiobjec.h>
// # include <owl\docmanag.h>
// # include <owl\filedoc.h>
// # include <owl\listbox.h>
// # include <owl\draglist.h>
// # include <owl\imagelst.h>
// # include <classlib\arrays.h>

// # include "nsbb\nstip.h"
// # include "nsbb\nslistwind.h"
// # include "nautilus\nsldvdoc.h"
// # include "nsldv\nsldvuti.h"
// # include "nsldv\nsldvvar.h"
// # include "nsldv\nsldvvuetoons.h"
# include "nsldv\nsldvvue_base.h"

// # include "nsldv\nsldvgoal.h"
// # include "partage\ns_timer.h"

// # include "nsldv\nsldvtoons.h"

// #define ICONWIDTH 16

// class NSLdvWindowToon ;
// class NSLdvChronoLine ;
// class NSLdvCommandPannel ;

class NSLdvView : public NSLdvViewBase
{
  protected:

    static long lObjectCount ;

  public:

    NSLdvView(NSLdvDocument& doc, TWindow* parent = (TWindow*) 0) ;
    NSLdvView(const NSLdvView& rv) ;
    ~NSLdvView() ;

    void EvSetFocus(THandle hWndLostFocus /* may be 0 */) ;

    void CmNewPreoccup() ;
    void CmNewRisk() ;
    void CmNewSocialConcern() ;

    void CmAddObjectifs(NSConcern* pConcern, LDVFRAME iFrame) ;
    void CmProject(NSConcern* pConcern, LDVFRAME iFrame, string sKsName, string sKsArchetype) ;

    void CmEvolPreoccup(LDVFRAME iFrame) ;
    void CmEvolPreoType(NSConcern* pConcern, LDVFRAME iFrame) ;

    NSLdvDocument* getDoc() { return TYPESAFE_DOWNCAST(_pLdVDoc, NSLdvDocument) ; }

    NSLdvView&     operator=(const NSLdvView& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

 DECLARE_RESPONSE_TABLE(NSLdvView) ;
};

#endif // __NSLDVVUE_H

