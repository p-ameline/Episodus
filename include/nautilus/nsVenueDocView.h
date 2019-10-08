// --------------------------------------------------------------------------// NSTEAMDOCVIEW.H		Document/Vues Capture Health Team
// --------------------------------------------------------------------------
// PA Janvier 2007
// --------------------------------------------------------------------------

#if !defined(__NS_VENUEDOCVIEW_H)#define __NS_VENUEDOCVIEW_H

#include <owl\docmanag.h>#include <owl\docview.h>

class NSSejourInfo ;

#include "nsbb\nsMView.h"
#include "nsbb\nsPaneSplitter.h"
//
//  ------------------  DOCUMENT  ------------------
//

class NSVenuesDocument : public TDocument, public NSRoot{
	public:

		NSVenuesDocument(TDocument *parent, NSContexte *pCtx = 0) ;		~NSVenuesDocument() ;

    bool saveVenue(NSSejourInfo *pVenue) ;
    bool addNewVenue(NSSejourInfo *pVenue) ;
    bool updateExistingVenue(NSSejourInfo *pVenue) ;

		// M�thodes virtuelles de TDocument    bool    Open(int mode, const char far *path = 0)   { return true ; }    bool    Close()                                    { return true ; }

		bool    IsOpen()                                   { return true ; }    void    invalidateViews(string sReason, string sReference = string("")) ;    void    emptyVenuesArray() ;    void    reloadVenuesArray() ;
    bool    updateVenuesFromCapture(NSCaptureArray* pCapt) ;
    bool    createVenueFromCapture(NSCaptureArray* pCapt) ;
    bool    updateSelectedVenueFromCapture(NSSejourInfo *pVenue, NSCaptureArray* pCapt) ;

    NSSejourInfo* getVenueForId(string sVenueId) ;

    vector<NSSejourInfo*>* getVenuesArray() { return &_aVenuesArray ; }

	protected:
    vector<NSSejourInfo*> _aVenuesArray ;};
//
//  ------------------  LIST VIEW  ------------------
//

class NSVenuesListWindow ;

class NSVenuesView : public NSMUEView{ public:

	// Constructeur - Destructeur
  NSVenuesView(NSVenuesDocument& doc, TWindow *parent = 0) ;
  ~NSVenuesView() ;

  void CmAdd() ;
  void CmChange() ;
  void CmClose() ;

  void CmOk() {}
  void CmCancel() {}

  // Red�finiton des fonctions virtuelles de TView et TRichEdit
  static LPCSTR   StaticName() { return "NSVenueView" ; }
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

  // Fonctions de r�ponse aux evts
  void   EvSize(uint sizeType, ClassLib::TSize &size) ;
  void   EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
  void   EvRButtonDownOut(uint modkeys, NS_CLASSLIB::TPoint& point) ;
  void   EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
  void   EvChar(uint key, uint repeatCount, uint flags) ;
  void   EvSetFocus(HWND hWndLostFocus) ;
  void   focusFct() ;

  void   reloadView(string sReason, string sReference = string("")) ;

 protected:

	// Remplissage de la vue
  void SetupWindow() ;
  void SetupTxtBar() ;
  void InitFormatFont() ;
  void SetupToolBar() ;

  void DispInfoListe(TLwDispInfoNotify& dispInfo) ;
  void LVNColumnclick(TLwNotify& lwn) ;

  void changeSortOrder(int iOrder) ;
  void sortByVenueId() ;
  void sortByUnit() ;
  void sortByDateEnding() ;
  void sortByDateBegining() ;

  // Variables

  NSVenuesDocument*    _pVenuesDoc ;

  NSVenuesListWindow*  _pListeWindow ;  OWL::TControlBar*    _pToolBar ;	        // Barre d'outils

  bool                 _bSetupToolBar ;

  bool                 _bNaturallySorted ;
  int                  _iSortedColumn ;

  NSGadgetButton* 		 _pBtNew ;
	NSGadgetButton*      _pBtEdit ;
  NSGadgetButton*      _pBtClose ;

	DECLARE_RESPONSE_TABLE(NSVenuesView) ;
};

//
// Objet "ListWindow" avec gestion du double-click
//
class NSVenuesListWindow : public TListWindow
{
	public:

		NSVenuesView* _pView ;    int           _iRes ;
    NSVenuesListWindow(NSVenuesView* pere, int resId, int x, int y, int w,                                                int h, TModule* module = 0) ;

    ~NSVenuesListWindow() {}    void SetupWindow() ;
    void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvKeyDown(uint key, uint repeatCount, uint flags) ;    void EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt) ;    int  IndexItemSelect() ;

    void EvSetFocus(HWND hWndLostFocus) ;
    void EvKillFocus(HWND hWndGetFocus) ;

	DECLARE_RESPONSE_TABLE(NSVenuesListWindow) ;};

#endif		// __NS_VENUEDOCVIEW_H
