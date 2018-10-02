// NSALERTSVCE.H : Display drugs alerts/////////////////////////////////////////////////////////////
#if !defined(__NSALERTSVCE_H)#define __NSALERTSVCE_H
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

class NSContexte ;
class TWebProxy ;class NSAlertGadgetWindow ;
class NSAlertListWindow ;
class NSTxtAlertViewerWindow ;
class NSAlertElementsViewerWindow ;

//---------------------------------------------------------------------------//// Classes////---------------------------------------------------------------------------
class NSAlertServiceWindow ;
class NSHtmlAlertViewerWindow : public TWindow, public NSRoot
{
  public:

    // constructors / destructor
    NSHtmlAlertViewerWindow(NSContexte* pCtx, NSAlertServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
    ~NSHtmlAlertViewerWindow() ;

    void PerformCreate(int menuOrId) ;
    void SetupWindow() ;    void display(const string sBodyText) ;    void Navigate() ;
    NSAlertServiceWindow* _pParentWindow ;
    string                _sBodyText ;

  protected:

    TWebProxy				      *_Form ;
 // DECLARE_RESPONSE_TABLE(NSHtmlAlertViewerWindow) ;
} ;

class NSAlertServiceWindow : public TPaneSplitter, public NSRoot
{
  public:

    NSAlertGadgetWindow* _pGadgetPanelWindow ;
    TMDIChild*           _pMDIChild ;

    // NSClientWindow(TWindow* parent, TSplitDirection splitDir, float percent = .50);
    NSAlertServiceWindow(NSContexte* pCtx, NSPrescriptionCheckingMessage* pCheckInfo, ArrayCopyDrugs* pDrugs, TWindow* parent, const char far* title = 0, TModule* module = 0) ;
    ~NSAlertServiceWindow() ;

    void  CmOk() ;
    void  CmHelp() ;
    void  CmCancel() ;

    NSAlertListWindow*           getList()       { return _pAlertListWindow ; }
    NSAlertElementsViewerWindow* getElements()   { return _pAlertElementsWindow ; }
    NSWebServiceWindow*          getViewer()     { return _pAlertViewWindow ; }
    NSTxtAlertViewerWindow*      getTxtViewer()  { return _pAlertTextWindow ; }
    NSHtmlAlertViewerWindow*     getHtmlViewer() { return _pAlertHtmlWindow ; }

    ArrayCopyDrugs*              getDrugs()      { return &_aDrugs ; }

    void  reloadElements(const NSPrescriptionCheckingMessage* pCheckInfo) ;
    void  reloadDrugs(ArrayCopyDrugs* pDrugs) ;

    void  openTextViewer() ;
    void  openHtmlViewer() ;
    bool  ReplacePane(TWindow* target, TWindow* newPane, TPaneSplitter::TDelete dt) ;
    void  setHtmlWindowPos() ;

    void  displayHtml(const string sAlertContent, const string sAlertDetail) ;
    void  displayTxt(const string sAlertContent, const string sAlertDetail) ;

  protected:

    void  EvClose() ;
    void  SetupWindow() ;
    void  EvSize(uint sizeType, ClassLib::TSize& size) ;

    void  loadDrugs(ArrayCopyDrugs* pDrugs) ;

    NSAlertListWindow*           _pAlertListWindow ;
    NSAlertElementsViewerWindow* _pAlertElementsWindow ;
    NSWebServiceWindow*          _pAlertViewWindow ;
    NSTxtAlertViewerWindow*      _pAlertTextWindow ;
    NSHtmlAlertViewerWindow*     _pAlertHtmlWindow ;

    TWindow*                     _pCurrentViewer ;

    ArrayCopyDrugs                _aDrugs ;
    NSPrescriptionCheckingMessage _CheckInfo ;

	// DECLARE_RESPONSE_TABLE(NSAlertServiceWindow) ;
};
// Classe TMDIChild pour la redéfinition de EvClose
/////////////////////////////////////////////////////////
class NSAlertServiceChild : public TMDIChild, public NSRoot
{
	public :

		NSAlertServiceWindow* pClient ;

		NSAlertServiceChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSAlertServiceWindow* clientWnd = 0) ;
		~NSAlertServiceChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSAlertServiceChild) ;
};

#endif  // __NSALERTSVCE_H

// fin de nsAlertSvce.h///////////////////////////////////////////////////////////////

