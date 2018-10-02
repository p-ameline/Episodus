#ifndef __NSMDIFRAMEBASE_H__

# include <owl\mdi.h>
// # include <owl\controlb.h>
// # include <owl\statusba.h>
// # include <owl\printer.h>
// # include <owl\docking.h>
// # include <owl\controlg.h>
// # include <owl\buttonga.h>
// # include <owl\menu.h>
// # include <owl\splashwi.h>

class NSInPersonsChild ;
class NSMDIClient ;

// # include "docking\dockinge.h"
// # include "docking\stripeDock.h"
// # include "partage\nsdivfct.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

// # include "nsbb\nsMdiChild.h"
// # include "nsepisod\nsToDo.h"

# include "nsldv/nsldvexport.h"

class _NSLDVCLASSE NSMDIFrameBase : public TDecoratedMDIFrame, public NSRoot
{

    enum TIMERIMPORTTYPES { isScan = 0, isImage } ;

    bool       bEndSession;

    NSMDIFrameBase(NSContexte* pCtx, const char far *title, TResId menuResId, TMDIClient &clientWnd = *new TMDIClient, bool trackMenuSelection = false, OWL::TModule* module = 0) ;
    virtual ~NSMDIFrameBase() ;
    void SetupWindow() ;
    void EvTimer(uint timerId) ;
    void CloseStripes() ;

#endif // !__NSMDIFRAMEBASE_H__