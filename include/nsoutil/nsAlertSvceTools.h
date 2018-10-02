// NSALERTSVCE.H : Display drugs alerts/////////////////////////////////////////////////////////////
#if !defined(__NSALERTSVCETOOLS_H)#define __NSALERTSVCETOOLS_H
// # include <owl/button.h># include <owl/layoutwi.h>
# include <owl/panespli.h>
# include <owl/listwind.h>
# include <owl/edit.h>
# include <owl/treewind.h>
# include <owl/mdichild.h>
// # include <owl/control.h>
# include <owl/gadgetwi.h># include <owl/imagelst.h>// # include <owl/gauge.h>// # include <owl/radiobut.h>// # include <owl/combobox.h>
#include <winuser.h>

#include "nssavoir\nsBdmDriver.h"
#include "partage\ns_vector.h"#include "partage\NTArray.h"
#include "nsbb\nsutidlg.h"
#include "nssavoir\nscaptur.h"
#include "nssavoir\nsFileCaptur.h"
#include "nautilus\nsbrowseSvce.h"
#include "nsldv\nsldvdrug.h"

#include "nsoutil/nsexport.h"

class NSContexte ;/**
 * An array of pointers to NSLdvDrug objects not managed here (not deleted by destructor)
 */
class _NSOUTILCLASSE ArrayCopyDrugAlerts : public NsDrugAlertsVector
{
  public:

    // Constructeurs - Destructeur
    ArrayCopyDrugAlerts() : NsDrugAlertsVector() {}
    ArrayCopyDrugAlerts(const ArrayCopyDrugAlerts& rv) ;
    ~ArrayCopyDrugAlerts() ;

    void vider() ;
    void addTo(const NsDrugAlertArray* pSrc) ;

    ArrayCopyDrugAlerts& operator=(const ArrayCopyDrugAlerts& src) ;
} ;

bool mailObjSortByAlertInf(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByAlertSup(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByNameInf(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByNameSup(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByExamDateInf(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByExamDateSup(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByErrorInf(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByErrorSup(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByTypeInf(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;
bool mailObjSortByTypeSup(NsDrugAlert *pObj1, NsDrugAlert *pObj2) ;

//---------------------------------------------------------------------------//// Classes////---------------------------------------------------------------------------
class NSAlertServiceWindow ;
class NSAlertGadgetWindow : public TGadgetWindow, public NSRoot
{
  public:

    NSAlertGadgetWindow(NSContexte* pCtx, TWindow* parent, bool bAddButtons = false, const char far* title = 0 ) ;
    ~NSAlertGadgetWindow() ;

    void                CmChildOK() ;
    void                CmChildHelp() ;
    void                CmChildCancel() ;

    void                EvSetFocus(HWND hWndLostFocus) ;

    OWL::TButton        *pCloseBtn ;
    OWL::TButton        *pHelp ;

    void                activateParent() ;

  protected:

    void                  SetupWindow() ;
    NSAlertServiceWindow* _pPaneSplitter ;

	DECLARE_RESPONSE_TABLE(NSAlertGadgetWindow) ;
} ;

class _NSOUTILCLASSE NSAlertListWindow : public TListWindow, public NSRoot
{
  public:

    // constructors / destructor
    NSAlertListWindow() ;
    NSAlertListWindow(NSContexte* pCtx, NSAlertServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
    ~NSAlertListWindow() ;

    // Loading mails list
    void    loadMessages() ;
    void    getNewMessagesFromBufferDirectory(string sExtension = string("txt")) ;
    void    initMailList(string sExtension = string("txt")) ;
    void    initMailFile(HANDLE hFile, string sPrefix, string sSuffix) ;
    bool    analizeWorkflowFile(NsDrugAlert* pMailObj) ;
    bool    analizeMailFile(NsDrugAlert* pMailObj, bool bHeaderOnly = false) ;
    bool    analizeHtmlFile(NsDrugAlert* pMailObj, bool bHeaderOnly = false) ;
    void    analizeHtmlHeader(NSCaptureArray* pCaptureArray, string* pParamStr, ImportedFileType* piType) ;
    void    checkClinicalAlert(NsDrugAlert* pMailObj) ;
    void    sendToArchives(NsDrugAlert* pMailObj) ;
    void    removeEntry(NsDrugAlert* pMailObj) ;
    void    displayMessage() ;

    // void    reinitMailList() ;

    // réponse aux événements
    bool    VnIsWindow(HWND hWnd) ;
    void    EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void    EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void    EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void    EvSetFocus(HWND hWndLostFocus) ;
    void    EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt) ;
    void    DispInfoListe(TLwDispInfoNotify& dispInfo) ;

    void    sortAgain() ;
    void    LVNColumnclick(TLwNotify& lwn) ;
    void    changeSortOrder(int iCol) ;
    void    changeAlertSortOrder()    { changeSortOrder(0) ; }
    void    changeNameSortOrder()     { changeSortOrder(1) ; }
    void    changeExamDateSortOrder() { changeSortOrder(4) ; }
    void    changeErrorSortOrder()    { changeSortOrder(3) ; }
    void    changeTypeSortOrder()     { changeSortOrder(2) ; }
    void    sortByAlert() ;
    void    sortByName() ;
    void    sortByExamDate() ;
    void    sortByType() ;
    void    sortByError() ;

    int     IndexItemSelect() ;

    void    SetupToolBar() ;
    void    SetupWindow() ;
    void    SetupColumns() ;
    void    Reload() ;
    void    AfficheListe() ;
    void    EvKillFocus(HWND hWndGetFocus) ;

    void    processSelectionChange() ;
    void    selectFirstMessage() ;

    void    CmSetDocParams() ;

    bool    bSetupToolBar ;

    NSAlertServiceWindow* _pParentWindow ;
    ArrayCopyDrugAlerts   _aAlerts ;

    string                _sBufferDirectory ;
    string                _sDirectory ;
    string                _sArchivesDirectory ;
    int                   _iRes ;

  protected:

    int                   _iPreviousSelection ;
    bool                  _bLogOn ;

    bool                  _bNaturallySorted ;
    int                   _iSortedColumn ;

    OWL::TImageList*      _pImages ;

    bool                  _bMustRestartFileProcess ;

  DECLARE_RESPONSE_TABLE(NSAlertListWindow) ;
} ;

class _NSOUTILCLASSE NSTxtAlertViewerWindow : public OWL::TEdit, public NSRoot
{
  public:

    // constructors / destructor
    NSTxtAlertViewerWindow(NSContexte* pCtx, NSAlertServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
    ~NSTxtAlertViewerWindow() ;

    void SetupWindow() ;
    void displayText(string* pTxtContent, string* pTxtDetail) ;

    NSAlertServiceWindow* _pParentWindow ;

 // DECLARE_RESPONSE_TABLE(NSTxtAlertViewerWindow) ;
} ;

class _NSOUTILCLASSE NSAlertElementsViewerWindow : public OWL::TTreeWindow, public NSRoot
{
  public:

    // constructors / destructor
    NSAlertElementsViewerWindow(NSContexte* pCtx, NSAlertServiceWindow *parent, int id, int x, int y, int w, int h) ;
    ~NSAlertElementsViewerWindow() ;

    void SetupWindow() ;
    void displayInformation(const NSPrescriptionCheckingMessage* pCheckInfo) ;

    NSAlertServiceWindow* _pParentWindow ;

  // DECLARE_RESPONSE_TABLE(NSAlertElementsViewerWindow) ;
} ;


#endif  // __NSALERTSVCETOOLS_H

// fin de nsAlertSvceTools.h///////////////////////////////////////////////////////////////

