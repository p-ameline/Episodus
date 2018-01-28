#ifndef __NSINTERFACE_H

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

// View for interfaces with material devices
{
  public :

    NSInterfaceView(NSInterfaceDocument& doc, TWindow *parent = 0) ;

    // fonction virtuelles de TView
    const char*   GetViewName() { return StaticName() ; }
    TWindow*      GetWindow()   { return (TWindow*) this ; }

    void PerformCreate(int menuOrId);
    void SetupWindow();
    void EvClose();
    bool CanClose();
    void CmQuitter() ;
    void FillStartupInfo(LPSTARTUPINFO psi);

    void CmCreatePatient() ;
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

    void   SetDocTitle(string sTitle)  { _sDocTitle = sTitle ; }
    string GetDocTitle()   { return _sDocTitle ; }

		// M�thodes virtuelles de TDocument
    bool OpenModule() ;
		bool Close() ;

		bool IsOpen() ;
#endif  // __NS_INTERFACE_H
