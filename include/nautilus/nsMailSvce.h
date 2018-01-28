// NSBROWSE.H : Définition des objets dialogues contenant ie4/////////////////////////////////////////////////////////////
#if !defined(__NSMAILSVCE_H)#define __NSMAILSVCE_H
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
class NSNewMailObject
{
  private:

    static long lObjectCount ;

  public:

    NSNewMailObject(NSContexte* pCtx) ;
    ~NSNewMailObject() ;

    NSNewMailObject(const NSNewMailObject& rv) ;

    NSNewMailObject& operator=(const NSNewMailObject& src) ;

    void   initForUnknownPerson() ;
    void   initFromCapture(NSCaptureArray* pCapture) ;
    string getDisplayName() ;
    string getCurrentPatientDisplayName() ;

    bool   createWorkflowFile(string sFileName) ;
    bool   initFromWorkflowFile(string sFileName) ;
    void   initValuePairInformation(string sChapter, string sTitle, string sValue) ;

    size_t getProcessErrorLevel() ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    string sFileName ;
    string sWorkflowFileName ;

    ImportedFileType iType ;

    string sReceptionDate ;

    string sPatientName ;
    string sPatientSurname ;
    string sPatientBirth ;
    string sExamName ;
    string sExamDate ;
    string sExamCode ;
    bool   bExamVisible ;
    string sSender ;
    string sReceiver ;

    NSPatPathoArray demographicInformation ;
    NSPatPathoArray clinicalInformation ;

    string sProcessLog ;

    enum MAIL_ALERT_LEVEL { mailNoAlert = 0, mailUndefinedAlertLevel,
                            mailAlertLevel1, mailAlertLevel2, mailDangerAlert } ;

    MAIL_ALERT_LEVEL iAlertLevel ;
    string sAlertMessage ;

    enum MAIL_WORKFLOW { mailUnknowState = 0, mailNew, mailNotNew, mailRead } ;

    MAIL_WORKFLOW iWorkflowState ;

    NSContexte* pContexte ;
} ;

//
// Objet "Boite de dialogue" utilisé pour montrer la progression du
// chargement des messages
//
class NSMsgLoadProgressDlg : public NSUtilDialog
{
  public:

    OWL::TGauge*  pNewProgressBar ;
    OWL::TStatic* pNewText ;
    OWL::TGauge*  pOldProgressBar ;
    OWL::TStatic* pOldText ;
    OWL::TButton* pStopButton ;

    bool bKeepLoading ;

    NSMsgLoadProgressDlg(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
    ~NSMsgLoadProgressDlg() ;

    void SetupWindow() ;

    void resetNewGauge() { pNewProgressBar->SetValue(0) ; }
    void resetOldGauge() { pOldProgressBar->SetValue(0) ; }

    void fixePositionNew(int iPos) { pNewProgressBar->SetValue(iPos) ; }
    void fixePositionOld(int iPos) { pOldProgressBar->SetValue(iPos) ; }

    bool keepLoading() { return bKeepLoading ; }

    void stopPressed() ;

	DECLARE_RESPONSE_TABLE(NSMsgLoadProgressDlg) ;
};

typedef vector<NSNewMailObject*>  NSNewMailObjectArray ;
typedef NSNewMailObjectArray::iterator NSNewMailObjectIter ;
typedef NTArray<NSNewMailObject>  NSNewMailObjectsArray ;

bool mailObjSortByAlertInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByAlertSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByNameInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByNameSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByExamDateInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByExamDateSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByErrorInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByErrorSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByTypeInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;
bool mailObjSortByTypeSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2) ;

//---------------------------------------------------------------------------//// Classes////---------------------------------------------------------------------------
class NSMailServiceWindow ;
class NSMailGadgetWindow : public TGadgetWindow, public NSRoot
{
  public:

    NSMailGadgetWindow(NSContexte* pCtx, TWindow* parent, bool bAddButtons = false, const char far* title = 0 ) ;
    /* NSGadgetWindow(TWindow* parent = 0, TTileDirection direction = Horizontal,
          OWL::TFont* font = new TGadgetWindowFont, TModule* module = 0);  */
    ~NSMailGadgetWindow() ;

    void                CmChildOK() ;
    void                CmChildHelp() ;
    void                CmChildCancel() ;

    void                EvSetFocus(HWND hWndLostFocus) ;

    OWL::TButton        *pCloseBtn ;
    OWL::TButton        *pHelp ;

    void                activateParent() ;

  protected:

    void                 SetupWindow() ;
    NSMailServiceWindow* pPaneSplitter ;

	DECLARE_RESPONSE_TABLE(NSMailGadgetWindow) ;
} ;

class NSMailListWindow : public TListWindow, public NSRoot
{
  public:

    // constructors / destructor
    NSMailListWindow() ;
    NSMailListWindow(NSContexte* pCtx, NSMailServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
    ~NSMailListWindow() ;

    // Loading mails list
    void    loadMessages() ;
    void    getNewMessagesFromBufferDirectory(string sExtension = string("txt")) ;
    size_t  countFilesInDirectory(char* szSearchString) ;
    void    initMailList(string sExtension = string("txt")) ;
    void    initMailFile(HANDLE hFile, string sPrefix, string sSuffix) ;
    bool    analizeWorkflowFile(NSNewMailObject* pMailObj) ;
    bool    analizeMailFile(NSNewMailObject* pMailObj, bool bHeaderOnly = false) ;
    bool    analizeHtmlFile(NSNewMailObject* pMailObj, bool bHeaderOnly = false) ;
    void    analizeHtmlHeader(NSCaptureArray* pCaptureArray, string* pParamStr, ImportedFileType* piType) ;
    void    checkClinicalAlert(NSNewMailObject* pMailObj) ;
    void    sendToArchives(NSNewMailObject* pMailObj) ;
    void    removeEntry(NSNewMailObject* pMailObj) ;
    void    displayMessage() ;

    void    reinitMailList() ;

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
    void    AfficheListe() ;
    void    EvKillFocus(HWND hWndGetFocus) ;

    void    processSelectionChange() ;
    void    selectCurrentPatientFirstMessage() ;

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

    NSMailServiceWindow*  pParentWindow ;
    NSNewMailObjectsArray aNewMails ;

    string                sBufferDirectory ;
    string                sDirectory ;
    string                sArchivesDirectory ;
    int                   iRes ;

  protected:

    int                   iPreviousSelection ;
    bool                  bDetailsOn ;
    bool                  bLogOn ;

    bool                  bNaturallySorted ;
    int                   iSortedColumn ;

    OWL::TImageList*      pImages ;

    bool                  bMustRestartFileProcess ;

    NSMsgLoadProgressDlg* pProgressDlg ;

  DECLARE_RESPONSE_TABLE(NSMailListWindow) ;
} ;

class NSTxtViewerWindow : public TEditFile, public NSRoot
{
  public:

    // constructors / destructor
    NSTxtViewerWindow(NSContexte* pCtx, NSMailServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
    ~NSTxtViewerWindow() ;

    void displayFile(string sFileCompleteName) ;
    void displayFileBody(string sFileCompleteName) ;
    void displayText(string* pTxt) ;

    NSMailServiceWindow*  pParentWindow ;
    string                sFileName ;

 // DECLARE_RESPONSE_TABLE(NSTxtViewerWindow) ;
} ;

class NSHtmlViewerWindow : public TWindow, public NSRoot
{
  public:

    // constructors / destructor
    NSHtmlViewerWindow(NSContexte* pCtx, NSMailServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module = 0) ;
    ~NSHtmlViewerWindow() ;

    void PerformCreate(int menuOrId) ;
    void SetupWindow() ;    void displayFile(string sFileCompleteName) ;    void Navigate() ;
    NSMailServiceWindow*  pParentWindow ;
    string                sFileName ;

  protected:

    TWebProxy				      *Form ;
 // DECLARE_RESPONSE_TABLE(NSTxtViewerWindow) ;
} ;

/*
class NSMailViewWindow : public TWindow, public NSRoot
{
	public :

    string 		  sUrl ; 		      // url de référence    string 		  sHtml ;         // fichier html à visualiser/imprimer

    NSMailViewWindow(NSContexte* pCtx, NSMailServiceWindow *parent = 0) ;
    ~NSMailViewWindow() ;

    // Redéfiniton des fonctions virtuelles de TView    static LPCSTR	StaticName()  { return "NSMailViewWindow" ; }
    const char*		GetViewName() { return StaticName() ; }
    TWindow*			GetWindow() ;
    bool					CanClose() ;

    // SetDocTitle et GetFileName    bool 					SetDocTitle(const char far* docname, int index)
      { return TWindow::SetDocTitle(docname, index) ; }

    void            EvClose() ;
    // Fonctions de la VisualView    void  PerformCreate(int menuOrId) ;
    void  SetURL(const char* _URL) ;
    const char* GetURL() ;

    // void SetParentProperty(HWND hWnd);    void  Navigate(string sUrlTarget) ;

    bool  GenereHtml() ;
		void  MakeVisible() ;

  protected:
  	void  SetupWindow() ;

  	bool  IsTabKeyMessage(MSG &msg) ;    bool  PreProcessMsg(MSG &msg) ;

  private:

  	char       URL[2048] ;    TWebProxy* Form ;

		NSMailServiceWindow *pParentWindow ;

  DECLARE_RESPONSE_TABLE(NSMailViewWindow) ;};
*/

class NSMailServiceWindow : public TPaneSplitter, public NSRoot
{
  public:

    NSMailGadgetWindow* pGadgetPanelWindow ;
    TMDIChild*          pMDIChild ;

    // NSClientWindow(TWindow* parent, TSplitDirection splitDir, float percent = .50);
    NSMailServiceWindow(NSContexte* pCtx, TWindow* parent, const char far* title = 0, TModule* module = 0) ;
    ~NSMailServiceWindow() ;

    void  CmOk() ;
    void  CmHelp() ;
    void  CmCancel() ;

    NSMailListWindow*   getList()       { return pMailListWindow ; }
    NSWebServiceWindow* getViewer()     { return pMailViewWindow ; }
    NSTxtViewerWindow*  getTxtViewer()  { return pMailTextWindow ; }
    NSHtmlViewerWindow* getHtmlViewer() { return pMailHtmlWindow ; }

    bool  isSelectedPatientEqualToCurrentPatient(NSNewMailObject* pMailObject) ;

    void  openPatient(NSNewMailObject* pMailObject) ;
    bool  importTxtDocument(NSNewMailObject* pMailObject) ;
    bool  importParsedDocument(NSNewMailObject* pMailObject) ;
    bool  importBothDocuments(NSNewMailObject* pMailObject) ;
    bool  explodeMultipleMessages(NSNewMailObject* pMailObject) ;

    bool  canProceedWithPatient(NSNewMailObject* pMailObject) ;

    void  openTextViewer() ;
    void  openHtmlViewer() ;
    bool  ReplacePane(TWindow* target, TWindow* newPane, TPaneSplitter::TDelete dt) ;
    void  setHtmlWindowPos() ;

  protected:

    void  EvClose() ;
    void  SetupWindow() ;
    void  EvSize(uint sizeType, ClassLib::TSize& size) ;

    NSMailListWindow*   pMailListWindow ;
    NSWebServiceWindow* pMailViewWindow ;
    NSTxtViewerWindow*  pMailTextWindow ;
    NSHtmlViewerWindow* pMailHtmlWindow ;

    TWindow*            pCurrentViewer ;

	// DECLARE_RESPONSE_TABLE(NSMailServiceWindow) ;
};
// Classe TMDIChild pour la redéfinition de EvClose
/////////////////////////////////////////////////////////
class NSMailServiceChild : public TMDIChild, public NSRoot
{
	public :

		NSMailServiceWindow* pClient ;

		NSMailServiceChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSMailServiceWindow* clientWnd = 0) ;
		~NSMailServiceChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSMailServiceChild) ;
};

// -----------------------------------------------------------------------------
//
// Objet "Boite de dialogue" utilisé pour préparer l'enregistrement d'un document
//
// -----------------------------------------------------------------------------
class PreEnregMailDialog : public NSUtilDialog
{
	public:

		NSNewMailObject     *_pMailObject ;

		OWL::TEdit          *_pNomDoc ;
    NSUtilLexique       *_pType ;
		NSUtilEditDateHeure *_pDateCreation ;
		OWL::TRadioButton   *_pHistorique ;

		int        	  	     _creation ;

		PreEnregMailDialog(TWindow *, NSNewMailObject *, NSContexte *pCtx) ;
		~PreEnregMailDialog() ;

		void            SetupWindow() ;

		void            CmOk() ;
		void            CmCancel() ;

	DECLARE_RESPONSE_TABLE(PreEnregMailDialog) ;
} ;

#endif  // __NSMAILSVCE_H

// fin de nsMailSvce.h///////////////////////////////////////////////////////////////

