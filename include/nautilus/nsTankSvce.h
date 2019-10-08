// NSBROWSE.H : Définition des objets dialogues contenant ie4/////////////////////////////////////////////////////////////
#if !defined(__NSTANKSVCE_H)#define __NSTANKSVCE_H
# include <owl/button.h># include <owl/layoutwi.h>
# include <owl/panespli.h>
# include <owl/listwind.h>
# include <owl/editfile.h>
# include <owl/mdichild.h>
# include <owl/control.h>
# include <owl/gadgetwi.h># include <owl/imagelst.h># include <owl/gauge.h># include <owl/radiobut.h># include <owl/combobox.h>

// #include "nautilus\nssuper.h"#include "partage\ns_vector.h"
#include "partage\NTArray.h"
#include "nsbb\nsutidlg.h"
#include "nssavoir\nscaptur.h"
#include "nssavoir\nsFileCaptur.h"
#include "nautilus\nsbrowseSvce.h"

class NSContexte ;class TWebProxy ;
class NSTankPerson
{
  private:

    static long lObjectCount ;

  public:

    NSTankPerson(NSContexte* pCtx) ;
    NSTankPerson(NSContexte* pCtx, const string sName, const string sSurname, const string sBirth, const string sFileName) ;
    ~NSTankPerson() ;

    NSTankPerson(const NSTankPerson& rv) ;

    NSTankPerson& operator=(const NSTankPerson& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    string getPatientName()    const { return _sPatientName ; }
    string getPatientSurname() const { return _sPatientSurname ; }
    string getPatientBirth()   const { return _sPatientBirth ; }
    string getXmlFileName()    const { return _sXmlFileName ; }

    void   setPatientName(const string sS)    { _sPatientName    = sS ; }
    void   setPatientSurname(const string sS) { _sPatientSurname = sS ; }
    void   setPatientBirth(const string sS)   { _sPatientBirth   = sS ; }
    void   setXmlFileName(const string sS)    { _sXmlFileName    = sS ; }

  protected:

    string _sPatientName ;
    string _sPatientSurname ;
    string _sPatientBirth ;
    string _sXmlFileName ;

    NSContexte* _pContexte ;
} ;

//
// Objet "Boite de dialogue" utilisé pour montrer la progression du
// chargement des messages
//
class NSTankLoadProgressDlg : public NSUtilDialog
{
  public:

    OWL::TGauge*  _pNewProgressBar ;
    OWL::TStatic* _pNewText ;
    OWL::TButton* _pStopButton ;

    bool          _bKeepLoading ;

    NSTankLoadProgressDlg(TWindow* pere, NSContexte* pCtx, TModule* module = (TModule*) 0) ;
    ~NSTankLoadProgressDlg() ;

    void SetupWindow() ;

    void resetNewGauge() { _pNewProgressBar->SetValue(0) ; }

    void fixePositionNew(int iPos) { _pNewProgressBar->SetValue(iPos) ; }

    bool keepLoading() { return _bKeepLoading ; }

    void stopPressed() ;

	DECLARE_RESPONSE_TABLE(NSTankLoadProgressDlg) ;
};

typedef vector<NSTankPerson*>       NSTankPersonArray ;
typedef NSTankPersonArray::iterator NSTankPersonIter ;
typedef NTArray<NSTankPerson>       NSTankPersonsArray ;

bool tankObjSortByNameInf(const NSTankPerson *pObj1, const NSTankPerson *pObj2) ;
bool tankObjSortByNameSup(const NSTankPerson *pObj1, const NSTankPerson *pObj2) ;
bool tankObjSortByBirthdateInf(const NSTankPerson *pObj1, const NSTankPerson *pObj2) ;
bool tankObjSortByBirthdateSup(const NSTankPerson *pObj1, const NSTankPerson *pObj2) ;

//---------------------------------------------------------------------------//// Classes////---------------------------------------------------------------------------
class NSTankServiceWindow ;
class NSTankGadgetWindow : public TGadgetWindow, public NSRoot
{
  public:

    NSTankGadgetWindow(NSContexte* pCtx, TWindow* parent, bool bAddButtons = false, const char far* title = 0 ) ;
    /* NSGadgetWindow(TWindow* parent = 0, TTileDirection direction = Horizontal,
          OWL::TFont* font = new TGadgetWindowFont, TModule* module = 0);  */
    ~NSTankGadgetWindow() ;

    void                CmChildOK() ;
    void                CmChildHelp() ;
    void                CmChildCancel() ;

    void                EvSetFocus(HWND hWndLostFocus) ;

    OWL::TButton        *pCloseBtn ;
    OWL::TButton        *pHelp ;

    void                activateParent() ;

  protected:

    void                 SetupWindow() ;
    NSTankServiceWindow* pPaneSplitter ;

	DECLARE_RESPONSE_TABLE(NSTankGadgetWindow) ;
} ;

class NSTankListWindow : public TListWindow, public NSRoot
{
  public:

    // constructors / destructor
    NSTankListWindow() ;
    NSTankListWindow(NSContexte* pCtx, NSTankServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
    ~NSTankListWindow() ;

    // Loading mails list
    void    loadPatients() ;
    void    loadPatient(const string sLine) ;

    void    displayMessage() ;

    void    reinitPatients() ;

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
    void    changeNameSortOrder()      { changeSortOrder(0) ; }
    void    changeBirthdateSortOrder() { changeSortOrder(1) ; }
    void    sortByName() ;
    void    sortByBirthdate() ;

    int     IndexItemSelect() ;

    void    SetupToolBar() ;
    void    SetupWindow() ;
    void    SetupColumns() ;
    void    AfficheListe() ;
    void    EvKillFocus(HWND hWndGetFocus) ;

    void    processSelectionChange() ;
    void    selectCurrentPatient() ;

    void    CmOpenPatient() ;
    void    CmImportTxtFile() ;
    void    CmImportTreeFile() ;
    void    CmImportBothFiles() ;
    void    CmSetDetailsMode() ;
    void    CmSetLogMode() ;
    void    CmSetBodyMode() ;
    void    CmExplodeMultipleMessage() ;
    void    CmDeleteMessage() ;

    void    CmSetDocParams() ;

    bool    bSetupToolBar ;

    NSTankServiceWindow* _pParentWindow ;
    NSTankPersonsArray   _aPatients ;

    int                   _iRes ;

  protected:

    int                   _iPreviousSelection ;
    bool                  _bDetailsOn ;
    bool                  _bLogOn ;

    bool                  _bNaturallySorted ;
    int                   _iSortedColumn ;

    bool                  _bMustRestartFileProcess ;

    NSTankLoadProgressDlg* _pProgressDlg ;

  DECLARE_RESPONSE_TABLE(NSTankListWindow) ;
} ;

class NSXmlTreeViewerWindow : public TTreeWindow, public NSRoot
{
  public:

    // constructors / destructor
    NSXmlTreeViewerWindow(NSContexte* pCtx, NSTankServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
    ~NSXmlTreeViewerWindow() ;

    void loadContent(const string sFileContent) ;

    NSTankServiceWindow* _pParentWindow ;
    string               _sFileName ;

    bool getLineContent(const string* psLine, string* psTag, string* psParams, string* psContent) ;

 // DECLARE_RESPONSE_TABLE(NSXmlTreeViewerWindow) ;
} ;

class NSTankServiceWindow : public TPaneSplitter, public NSRoot
{
  public:

    NSTankGadgetWindow* _pGadgetPanelWindow ;
    TMDIChild*          _pMDIChild ;

    // NSClientWindow(TWindow* parent, TSplitDirection splitDir, float percent = .50);
    NSTankServiceWindow(NSContexte* pCtx, TWindow* parent, const char far* title = 0, TModule* module = 0) ;
    ~NSTankServiceWindow() ;

    void  CmOk() ;
    void  CmHelp() ;
    void  CmCancel() ;

    NSTankListWindow*      getList()      { return _pTankListWindow ; }
    NSXmlTreeViewerWindow* getXmlViewer() { return _pXmlTreeWindow ; }

    bool  isSelectedPatientEqualToCurrentPatient(const NSTankPerson* pTankObject) const ;

    void  openPatient(NSTankPerson* pTankObject) ;
    bool  importXmlFile(NSTankPerson* pTankObject) ;

    bool  canProceedWithPatient(NSTankPerson* pTankObject) ;

    bool  ReplacePane(TWindow* target, TWindow* newPane, TPaneSplitter::TDelete dt) ;
    void  setXmlWindowPos() ;

  protected:

    void  EvClose() ;
    void  SetupWindow() ;
    void  EvSize(uint sizeType, ClassLib::TSize& size) ;

    NSTankListWindow*      _pTankListWindow ;
    NSXmlTreeViewerWindow* _pXmlTreeWindow ;

	// DECLARE_RESPONSE_TABLE(NSTankServiceWindow) ;
};
// Classe TMDIChild pour la redéfinition de EvClose
/////////////////////////////////////////////////////////
class NSTankServiceChild : public TMDIChild, public NSRoot
{
	public :

		NSTankServiceWindow* pClient ;

		NSTankServiceChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSTankServiceWindow* clientWnd = 0) ;
		~NSTankServiceChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSTankServiceChild) ;
};

// -----------------------------------------------------------------------------
//
// Objet "Boite de dialogue" utilisé pour préparer l'enregistrement d'un document
//
// -----------------------------------------------------------------------------
class PreEnregTankDocDialog : public NSUtilDialog
{
	public:

		NSTankPerson     *_pMailObject ;

		OWL::TEdit          *_pNomDoc ;
    NSUtilLexique       *_pType ;
		NSUtilEditDateHeure *_pDateCreation ;
		OWL::TRadioButton   *_pHistorique ;

		int        	  	     _creation ;

		PreEnregTankDocDialog(TWindow *, NSTankPerson *, NSContexte *pCtx) ;
		~PreEnregTankDocDialog() ;

		void            SetupWindow() ;

		void            CmOk() ;
		void            CmCancel() ;

	DECLARE_RESPONSE_TABLE(PreEnregTankDocDialog) ;
} ;

#endif  // __NSTANKSVCE_H

// fin de nsTankSvce.h///////////////////////////////////////////////////////////////

