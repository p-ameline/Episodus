#ifndef __NSINTERFACE_H#define __NSINTERFACE_H

class NSInterfaceDocument ;

class NSInterfaceButton : public OWL::TButton
{
  public:

    NSInterfaceButton(TWindow* parent, int resId) ;
    NSInterfaceButton(TWindow* parent, int resId, const char far* text,
                 int X, int Y, int W, int H, bool isDefault = false,
                 TModule* module = 0) ;
    ~NSInterfaceButton() ;

    char far* GetClassName() ;

    void SetupWindow() ;
};

// View for interfaces with material devices//class NSInterfaceView : public TWindowView, public NSRoot
{
  public :

    NSInterfaceView(NSInterfaceDocument& doc, TWindow *parent = 0) ;    ~NSInterfaceView() ;

    // fonction virtuelles de TView    static LPCSTR StaticName()  { return "NSInterfaceView" ; }
    const char*   GetViewName() { return StaticName() ; }
    TWindow*      GetWindow()   { return (TWindow*) this ; }

    void PerformCreate(int menuOrId);    // void MakeVisible();
    void SetupWindow();
    void EvClose();
    bool CanClose();
    void CmQuitter() ;
    void FillStartupInfo(LPSTARTUPINFO psi);

    void CmCreatePatient() ;    void CmStoreExternalId() ;    void CmStartNewExam() ;
    void CmGetPatientResults() ;
    void CmParams() ;

  protected :

    NSInterfaceDocument* _pDoc ;

    NSInterfaceButton* _pCreatePatientButton ;
    NSInterfaceButton* _pStoreExternalButton ;
    NSInterfaceButton* _pStartNewExamButton ;
    NSInterfaceButton* _pGetPatientResultsButton ;
    NSInterfaceButton* _pParamsButton ;

    int  _iWindowsWidth ;
    int  _iWindowsHeight ;

    bool _bBusy ;

    string getStoreExernalButtonText() ;

  DECLARE_RESPONSE_TABLE(NSInterfaceView) ;
};

class NSInterfaceDocument : public TDocument, public NSRoot
{
	public:

		NSInterfaceDocument(TDocument *parent, NSContexte *pCtx = 0) ;
		~NSInterfaceDocument() {}

    void   SetDocTitle(string sTitle)  { _sDocTitle = sTitle ; }		string GetDocExt() ;
    string GetDocTitle()   { return _sDocTitle ; }

		// Méthodes virtuelles de TDocument		bool Open(int mode, const char far *path = 0) ;
    bool OpenModule() ;
		bool Close() ;

		bool IsOpen() ;    bool StartAction(TWindow *pere, string sActionName, string &sActionResult, NSVectPatPathoArray *pPptResult) ;    void importDocuments(NSVectPatPathoArray *pPatPatho, string sFileName) ;    void queryInterfaceIEP() ;    void storeInterfaceIEP(string sPreviousValue) ;    void openRemoteDocument(string sURI) ;    void   setInterfaceIEP(string sIEP) { _sInterfaceIEP = sIEP ; }    string getInterfaceIEP()            { return _sInterfaceIEP ; }    bool   isBusy() { return _bBusy ; }  protected :    string _sDocTitle ;    string _sModuleName ;    bool   _TestOk ;    string _sPrefix ;    string _sInterfaceIEP ;    bool   _bBusy ;    TModule* _pInterfaceModule ;    void   initInterfacesList(map<string, string> *paInterfaces, map<string, string> *paPseudoProtocols) ;    void   SetDocTitle() ;    void   recordTree(NSPatPathoArray *pPatPatho) ;    void   importBothDocuments(NSPatPathoArray *pPatPatho, string sFileName) ;    bool   importFileDocument(NSRefDocument *pRefDoc, string sDocTitle, string sFileName, NSRefDocument *pMasterDoc, NSRootLink::NODELINKTYPES masterLink) ;    bool   importExternalLink(NSRefDocument *pRefDoc, string sDocType, string sDocTitle, string sFileName, NSRefDocument *pMasterDoc) ;};
#endif  // __NS_INTERFACE_H

