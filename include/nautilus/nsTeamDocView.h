// --------------------------------------------------------------------------// NSTEAMDOCVIEW.H		Document/Vues Capture Health Team
// --------------------------------------------------------------------------
// PA Janvier 2007
// --------------------------------------------------------------------------

#if !defined(__NS_TEAMDOCVIEW_H)#define __NS_TEAMDOCVIEW_H

#include <owl\docmanag.h>#include <owl\docview.h>

#include "nssavoir\nsHealthTeam.h"#include "nssavoir\nsHealthTeamInterface.h"#include "nsbb\nsMView.h"#include "nsbb\nsPaneSplitter.h"
//
//  ------------------  DOCUMENT  ------------------
//

class NSHealthTeamDocument : public TDocument, public NSRoot{
	public:

		NSHealthTeamDocument(TDocument *parent, NSContexte *pCtx = 0, NSHealthTeam *pTeam = 0) ;		~NSHealthTeamDocument() ;

    bool savePatpatho() ;

		// M�thodes virtuelles de TDocument    bool    Open(int mode, const char far *path = 0)   { return true ; }    bool    Close()                                    { return true ; }

		bool    IsOpen()                                   { return true ; }    void    invalidateViews(string sReason, string sReference = string("")) ;
    NSHealthTeam* getHealthTeam()                      { return _pHealthTeam ; }

	protected:
		NSHealthTeam*  _pHealthTeam ;};
//
//  ------------------  ROSACE VIEW  ------------------
//

class NSHealthTeamRosaceView : public NSMUEView{
	public:

		// Constructeur - Destructeur		NSHealthTeamRosaceView(NSHealthTeamDocument& doc, TWindow *parent = 0) ;
		~NSHealthTeamRosaceView() ;

    void CmOk() {}
    void CmCancel() {}
		// Red�finiton des fonctions virtuelles de TView		static LPCSTR   StaticName() { return "NSHealthTeamRosaceView" ; }
		const char      *GetViewName() { return StaticName() ; }
		TWindow         *GetWindow();
		bool            CanClose();           // de TEditFile
		bool            SetDocTitle(const char far* docname, int index)
		{
			return OWL::TWindow::SetDocTitle(docname, index) ;
		}
    void            setViewName() ;

    void            EvDestroy();    void            SetWindowPosit();
    void            RecordWindowPosit();

		void            EvSetFocus(HWND hWndLostFocus);		void            EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point);
                        // Fonctions de r�ponse aux evts
		void            EvSize(uint sizeType, ClassLib::TSize &size);
		void            EvMove(ClassLib::TPoint& clientOrigin);
		void            EvWindowPosChanged(WINDOWPOS far& windowPos);
		void            EvPaint();
		void            gotMoved();

    void            reloadView(string sReason, string sReference = string("")) ;

		NSHealthTeamDocument* getDoc()     { return _pTeamDoc ; }    RosaceManager*        getManager() { return _pRosaceManager ; }
	protected:
		// Variables
		NSHealthTeamDocument* _pTeamDoc ; // Pointeur sur document soap    RosaceManager*        _pRosaceManager ;    RosaceDrawer*         _pRosaceDrawer ;
    bool bSetupToolBar ;
		// Remplissage de la vue    void FillViewData() ;
    void SetupWindow() ;
    void SetupToolBar() ;
    void SetupTxtBar() ;
    void InitFormatFont() ;

		// view event handlers		bool VnIsWindow(HWND hWnd) ;

		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;		void EvChar(uint key, uint repeatCount, uint flags) ;

	DECLARE_RESPONSE_TABLE(NSHealthTeamRosaceView) ;};

//
//  ------------------  LIST VIEW  ------------------
//

class NSHealthTeamListWindow ;

class NSHealthTeamListView : public NSMUEView{ public:

	// Constructeur - Destructeur
  NSHealthTeamListView(NSHealthTeamDocument& doc, TWindow *parent = 0) ;
  ~NSHealthTeamListView() ;

  void CmAdd() ;
  void CmFct1() ;  // Add record keeper
  void CmChange() ;
  void CmClose() ;
  void CmModifyPerson() ;
  void CmSwitchActiveOnly() ;
  void CmSwitchColumnsExtent() ;

  void CmOk() {}
  void CmCancel() {}

  // Red�finiton des fonctions virtuelles de TView et TRichEdit
  static LPCSTR   StaticName() { return "NSHealthTeamListView" ; }
  //const char      *GetViewName() {return StaticName();}
  const char      *GetViewName() { return _sViewName.c_str() ; }
  TWindow         *GetWindow();
  bool            SetDocTitle(const char far* docname, int index)
                  { return OWL::TWindow::SetDocTitle(docname, index) ; }
  void            setViewName() ;
  bool            CanClose() ;           

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

  void   AddCorresp(bool IsTraitant = false) ;

  void   reloadView(string sReason, string sReference = string("")) ;
  void   initCurentMembers() ;

  NSHealthTeamManager* getManager()     { return &_tManager ; }
  bool                 isExtendedMode() { return _bExtendedColumns ; }

 protected:

	// Remplissage de la vue
  void SetupWindow() ;
  void SetupTxtBar() ;
  void InitFormatFont() ;
  void SetupToolBar() ;

  void DispInfoListe(TLwDispInfoNotify& dispInfo) ;
  void LVNColumnclick(TLwNotify& lwn) ;

  void refreshHealthTeamDoc() ;
  void removeClosedMandates() ;
  void removePatientAndLocalTeam() ;

  void changeSortOrder(int iOrder) ;
  void sortByName() ;
  void sortByJob() ;
	void sortBySpec() ;
	void sortByDistance() ;
	void sortByAngle() ;
  void sortByEnding() ;
  void sortByBegining() ;

  void getInfosFromTraits(string sPersonId, int iIndex) ;
  void EvTimer(uint timerId) ;

  NSMemberMandatePairArray _aMembers ; 			// list member mandat
  bool                     _bActiveOnly ;   // just active mandats are displayed

  // Variables
  NSHealthTeamDocument*    _pTeamDoc ;  NSHealthTeamListWindow*  _pListeWindow ;  OWL::TControlBar*        _pToolBar ;	        // Barre d'outils

  bool                     _bSetupToolBar ;

  bool                     _bExtendedColumns ;
  int                      _iColumnNb ;
  bool                     _bNaturallySorted ;
  int                      _iSortedColumn ;

  NSHealthTeamManager      _tManager ;

  bool                     _bLocalMode ;

  NSGadgetButton* 				 _pBtNew ;
  NSGadgetButton* 				 _pBtNewTrt ;
	NSGadgetButton*          _pBtEdit ;
  NSGadgetButton*          _pBtClose ;

	DECLARE_RESPONSE_TABLE(NSHealthTeamListView) ;
};

//
// Objet "ListWindow" avec gestion du double-click
//
class NSHealthTeamListWindow : public TListWindow
{
	public:

		NSHealthTeamListView* _pView ;    int                   _iRes ;
    NSHealthTeamListWindow(NSHealthTeamListView* pere, int resId, int x, int y, int w,                                                int h, TModule* module = 0) ;

    ~NSHealthTeamListWindow() {}    void SetupWindow() ;
    void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point);
    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    void EvKeyDown(uint key, uint repeatCount, uint flags) ;    void EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt) ;    int  IndexItemSelect() ;

    void EvSetFocus(HWND hWndLostFocus) ;
    void EvKillFocus(HWND hWndGetFocus) ;

	DECLARE_RESPONSE_TABLE(NSHealthTeamListWindow) ;};

#endif		// __NS_TEAMDOCVIEW_H
