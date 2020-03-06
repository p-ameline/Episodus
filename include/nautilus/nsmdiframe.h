#ifndef __NSMDIFRAME_H__

# include <owl\mdi.h>
# include <owl\controlb.h>
# include <owl\statusba.h>
# include <owl\printer.h>
# include <owl\docking.h>
# include <owl\controlg.h>
# include <owl\buttonga.h>
# include <owl\menu.h>
# include <owl\splashwi.h>

class NSInPersonsChild ;
class NSMDIClient ;

# include "docking\dockinge.h"
# include "docking\stripeDock.h"
# include "partage\nsdivfct.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "nsbb\nsMdiChild.h"
# include "nsepisod\nsToDo.h"

# include "nsldv/nsmdiframe_base.h"

// class NSMDIFrame : public TOleMDIFrame
class NSMDIFrame : public NSMDIFrameBase
{
    NSPrinter* Printer;

    NSMDIFrame(NSContexte* pCtx, const char far *title, TResId menuResId, TMDIClient &clientWnd = *new TMDIClient, bool trackMenuSelection = false, OWL::TModule* module = 0) ;
    ~NSMDIFrame() ;
    void SetupWindow() ;
    void EvTimer(uint timerId) ;
    void EvEndSession(bool endSession, bool logOff /* used only by Win95 */) ;

#endif // !__NSMDIFRAMEBASE_H__