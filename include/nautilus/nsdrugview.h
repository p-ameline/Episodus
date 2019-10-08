// --------------------------------------------------------------------------// NSDRUGVIEW.H		Document/Vues Capture Données Utilisateur
// --------------------------------------------------------------------------
// PA - Juillet 2003
// --------------------------------------------------------------------------
#if !defined(__NSDRUGVIEW_H)
#define __NSDRUGVIEW_H

#include <owl\docmanag.h>#include <owl\docview.h>
#include <owl\mdichild.h>
#include <owl\decframe.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\applicat.h>
#include <owl\richedpr.h>
#include <owl\choosefo.h>
#include <owl\listwind.h>

#include "nautilus\nsepicap.h"

#include "nautilus\nsdocref.h"
#include "nautilus\nautilus.rh"
#include "nsbb\nsmview.h"
#include "nsbb\nslistWind.h"
#include "nautilus\nsldvdoc.h"
#include "nsldv\nsldvvar.h"
#include "nsldv\nsldvdrug.h"

#define MAXCARS 100000      // Limite du texte pour RichEdit

#define _NSDRUGVIEWCLASS _USERCLASS

unsigned long getDuration(NSLdvDrug* pLdvDrug) ;

class NSDrugsPropertyWindow;

class NSDrugView : public NSLDVView{ public:

	ArrayCopyDrugs _aCurrentDrugs ;

	// Constructeur - Destructeur
  NSDrugView(NSLdvDocument& doc, string sConcern = string("")) ;
  ~NSDrugView() ;

  void CmAdd() ;
  void CmAddWithCode(string sLexiCode) ;
  void CmContinue() ;
  void CmChange() ;
  void CmModifPoso() ;
  void CmClose() ;
  void CmSuppress() ;
  void CmPrevious() ;
  void CmAll() ;
  void CmFct1() ; // ordo
	void CmFct3() ; // protocol
	void CmFct4() ; // ordo sur sélection
  void CmFct5() ; // new guideline file from selection
  void CmFct6() ; // add selection to extisting guideline file
  void CmFct7() ; // drugs history
  void CmFct8() ; // ordo DCI
  void CmFct9() ; // drugs alerts
  void CmRights() ;
  void CmPublish() ;
  void CmHistory() ;
  void CmAlerts() ;
  void CmSwitchToVD() ;
  void CmDisplayAll() ;

  void CmOk() {}
  void CmCancel() {}

  // Redéfiniton des fonctions virtuelles de TView et TRichEdit
  static LPCSTR   StaticName() { return "NSDrugView" ; }
  //const char      *GetViewName() {return StaticName();}
  const char      *GetViewName() { return _sViewName.c_str() ; }
  TWindow         *GetWindow();
  bool            SetDocTitle(const char far* docname, int index)
                  { return OWL::TWindow::SetDocTitle(docname, index) ; }
  void            setViewName() ;

  void            SetupColumns() ;
  void            AfficheListe() ;

  // view event handlers
  bool 	          VnIsWindow(HWND hWnd) ;

  // Fonctions de réponse aux evts
  void   EvSize(uint sizeType, ClassLib::TSize &size) ;
  void   EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
  void   EvRButtonDownOut(uint modkeys, NS_CLASSLIB::TPoint& point) ;
  void   EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void   EvChar(uint key, uint repeatCount, uint flags) ;
  void   EvSetFocus(HWND hWndLostFocus) ;
  void   focusFct() ;

  string donneContexte(string sContexte) ;
  void   initCurentDrugs() ;

  void   reloadView(string sReason) ;

  // void						autoAddInDrugView(string sConcern, NSPatPathoArray *pPPT);
  void   autoAddInDrugView(string sConcern) ;

  bool   isSortedOnPrescribe() { return (1 == _iSortedColumn) ; }
  void   sortByPrescribe() ;

  void   addToPrescribeList(string sNodeId) ;
  void   removeFromPrescribeList(string sNodeId) ;
  bool   isInsidePrescribeList(string sNodeId) ;

  void   setDrugPrescribeStatus(NSLdvDrug *pDrug, bool bToPrescribe) ;
  void   adaptDrugToPrescribeList(NSLdvDrug *pDrug) ;

  void   openAlertWindow(NSPrescriptionCheckingMessage* pMsg) ;
  void   closeAlertWindow() ;
  void   checkByBdm(bool bForceOpenAlert = false) ;

  void   switchToDci(NSLdvDrug *pDrug) ;

 protected:

	// Remplissage de la vue
  void   SetupWindow() ;
  void   SetupTxtBar() ;
  void   InitFormatFont() ;
  void   SetupToolBar() ;

  void   DispInfoListe(TLwDispInfoNotify& dispInfo) ;
  void   LVNColumnclick(TLwNotify& lwn) ;

  void   sortByName() ;
  void   sortByAdmin() ;
  void   sortByDuration() ;
  void   sortByEnding() ;
  void   sortByBegining() ;
  void   sortByPrescrEnd() ;
  void   sortByPrescrDate() ;

  string getDrugRefToModify(string sWarningMsg = string("")) ;

  void   initCurrentList() ;
  void   markAddedDrugs() ;

  void   fillCheckingMessage(NSPrescriptionCheckingMessage* pCM) ;
  void   fillCheckingMessageBasics(NSPrescriptionCheckingMessage* pCM) ;
  void   fillCheckingMessageProblems(NSPrescriptionCheckingMessage* pCM) ;
  void   fillCheckingMessageAllergies(NSPrescriptionCheckingMessage* pCM) ;
  void   fillCheckingMessageDrugs(NSPrescriptionCheckingMessage* pCM) ;

  // Variables
  NSLdvDocument*         _pLdVDoc ;	// Pointeur sur document EPISOD  NSDrugsPropertyWindow* _pListeWindow ;  OWL::TControlBar*      _pToolBar ;	        // Barre d'outils
  VecteurString          _aToPrescribe ;
  VecteurString          _aPreviousDrugs ;
  bool                   _bOnlyActive ;       // Should ony active drugs be displayed

  bool                   _bSetupToolBar ;

  bool                   _bNaturallySorted ;
  int                    _iSortedColumn ;

  bool                   _isBeingUpgraded ;

	DECLARE_RESPONSE_TABLE(NSDrugView) ;
};

/*
class DrugTemplate : public DrugViewTmpl
{
    public:

	    string sConcern ;

	    // Constructeurs - Destructeur
	    DrugTemplate(string sConcernNode) ;
} ;
*/

//
// Objet "ListWindow" avec gestion du double-click (pour les médicaments à modifier)
//
class NSDrugsPropertyWindow : public NSSkinableListWindow
{
	public:

		NSDrugView* _pView ;    int         _iRes ;
    NSDrugsPropertyWindow(NSDrugView* pere, NSContexte* pCtx, int resId,                                                int x, int y, int w,                                                int h, TModule* module = 0) ;

    ~NSDrugsPropertyWindow() ;    void SetupWindow() ;
    void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvKeyDown(uint key, uint repeatCount, uint flags) ;    void EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;    void EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt) ;    int  IndexItemSelect() ;

    void EvSetFocus(HWND hWndLostFocus) ;
    void EvKillFocus(HWND hWndGetFocus) ;

    void switchPrescribeImage(int iItem, bool bToPrescribe) ;

  protected:

    OWL::TImageList* _pImages ;

	DECLARE_RESPONSE_TABLE(NSDrugsPropertyWindow) ;};

#endif		// __NSDRUGVIEW_H
