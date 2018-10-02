#ifndef __NSMDIFRAME_H__# define __NSMDIFRAME_H__

# include <owl\mdi.h># include <owl\decmdifr.h>
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
{  public:
    NSPrinter* Printer;

    NSMDIFrame(NSContexte* pCtx, const char far *title, TResId menuResId, TMDIClient &clientWnd = *new TMDIClient, bool trackMenuSelection = false, OWL::TModule* module = 0) ;
    ~NSMDIFrame() ;
    void SetupWindow() ;    bool EvQueryEndSession() ;
    void EvTimer(uint timerId) ;
    void EvEndSession(bool endSession, bool logOff /* used only by Win95 */) ;    void CmToDo() ;    void ExecuteToDo(NSToDoTask *pToDo) ;    NSMDIClient*      GetMdiClientWindow()   { return TYPESAFE_DOWNCAST(ClientWnd, NSMDIClient) ; }    NSBasicMDIClient* GetBasicClientWindow() { return TYPESAFE_DOWNCAST(ClientWnd, NSBasicMDIClient) ; }    HWND              GetPopupMenuWindowHandle() ;    void CheckForAutomaticImport(TIMERIMPORTTYPES iImportType) ;    void ToDo_Archetype(NSToDoTask* pToDo) ;    void ToDo_KS_Archetype(NSToDoTask* pToDo) ;    void ToDo_KS_DecisionTree(NSToDoTask* pToDo) ;    void ToDo_FermeArchetype(NSToDoTask* pToDo) ;    void ToDo_CreerBibArch(NSToDoTask* pToDo) ;    void ToDo_CreerBibChem(NSToDoTask* pToDo) ;    void ToDo_FermeDPIO(NSToDoTask* pToDo) ;    void ToDo_ActivateMUEView(NSToDoTask* pToDo) ;    void ToDo_NewPreoccup(NSToDoTask* pToDo) ;    void ToDo_NewPreoccupProposal(NSToDoTask* pToDo) ;    void ToDo_Message(NSToDoTask* pToDo, NsStripe::STRIPETYPE iStripeType) ;    void ToDo_NewPreoccupFromNode(NSToDoTask* pToDo) ;    void ToDo_NewDrugFromNode(NSToDoTask* pToDo) ;    void ToDo_ChangePreoccupFromNode(NSToDoTask* pToDo) ;    void ToDo_OpenNewWindow(NSToDoTask* pToDo) ;    void ToDo_ExportDocForDeletedCompt(NSToDoTask* pToDo) ;    void ToDo_GetPersonAsynch(NSToDoTask* pToDo) ;    void ToDo_OpenDocument(NSToDoTask* pToDo) ;    void ToDo_EditDocument(NSToDoTask* pToDo) ;    void ToDo_ExportDocument(NSToDoTask* pToDo) ;    void openWhoIsThereWindow(string sInPatientsFileDesc) ;    void closeWhoIsThereWindow() ;    NSInPersonsChild* getInPersonsChild()                       { return _pInPersonsChild ; }    void              setInPersonsChild(NSInPersonsChild* pIPC) { _pInPersonsChild = pIPC ; } DECLARE_RESPONSE_TABLE(NSMDIFrame) ;} ;

#endif // !__NSMDIFRAMEBASE_H__
