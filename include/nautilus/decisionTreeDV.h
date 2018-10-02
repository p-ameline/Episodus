// -----------------------------------------------------------------------------
// header des vues nscqvue.h
// -----------------------------------------------------------------------------
#if !defined(__DECISIONTREEDV_H)
# define __DECISIONTREEDV_H

#include <owl\docview.h>#include <owl\mdichild.h>#include <owl\splitter.h>#include <owl\controlb.h>#include <owl\button.h>#include <owl\checkbox.h>#include <owl\radiobut.h>#include <owl\groupbox.h>class NSSmallBrother ;class Cbalise ;
class CguidelineStep ;
class CdecisionStep ;
class CactionStep ;
class CbranchStep ;
class CsynchronizationStep ;
class CpatientStateStep ;
class nsGuidelineParseur ;
class NSGuidelineControlVector ;
class NSGuidelineControl ;
class NSDecisionTreeView ;
class BB1BBInterfaceForKs ;
#include "nsbb\nsMView.h"
#include "nautilus\nsdocref.h"
#include "nsbb\nsbb_glo.h"
#include "partage\nsdivfct.h"

enum ACTIVATIONSTATE { isBlank, isGreen, isRed } ;

class DTslot
{
  public:

    DTslot() ;
    DTslot(CguidelineStep *pStep, string sSlotId) ;
    ~DTslot() ;

    DTslot(const DTslot& rv) ;
    DTslot& operator=(const DTslot& src) ;

    void addControl(NSGuidelineControl *pGuidelineControl) ;
    void removeControl(NSGuidelineControl *pGuidelineControl) ;
    void setState(ACTIVATIONSTATE iState) ;

    CguidelineStep     *getStep()                         { return _pGuidelineStep ; }
    void                setStep(CguidelineStep *pStep)    { _pGuidelineStep = pStep ; }

    // NSGuidelineControlVector *getInterfaceControler()                              { return _pInterfaceControler ; }
    // void                     setInterfaceControler(NSGuidelineControlVector *pElt) { _pInterfaceControler = pElt ; }

    string              getSlotId() { return _sSlotId ; }
    string              getTitle(string sLang) ;

    void                setControl(NSGuidelineControl* pControl) ;

  protected:

    NSGuidelineControlVector* _pInterfaceControler ;

    CguidelineStep*     _pGuidelineStep ;
    string              _sSlotId ;
} ;

typedef vector<DTslot*>              DTslotVector ;
typedef DTslotVector::iterator       DTslotIter ;
typedef DTslotVector::const_iterator DTslotConstIter ;
typedef NTArray<DTslot>              DTslotArray ;

class DTlink
{
  public:

    DTlink() ;
    DTlink(DTslot* pSlotFrom, string sSlotIdTo, string sLinkId) ;
    ~DTlink() ;

    DTlink(const DTlink& rv) ;
    DTlink& operator=(const DTlink& src) ;

    void addControl(NSGuidelineControl *pGuidelineControl) ;
    void removeControl(NSGuidelineControl *pGuidelineControl) ;

    void setState(ACTIVATIONSTATE iState) ;

    // NSGuidelineControlVector* getInterfaceControler()                               { return _pInterfaceControler ; }
    // void                      setInterfaceControler(NSGuidelineControlVector *pElt) { _pInterfaceControler = pElt ; }

    DTslot*             getSlotFrom()               { return _pSlotFrom ; }
    void                setSlotFrom(DTslot* pSlot)  { _pSlotFrom = pSlot ; }

    string              getIdStepFrom()             { return _sIdStepFrom ; }
    void                setIdStepFrom(string sId)   { _sIdStepFrom = sId ; }

    DTslot*             getSlotTo()                 { return _pSlotTo ; }
    void                setSlotTo(DTslot* pSlot)    { _pSlotTo = pSlot ; }

    string              getIdStepTo()               { return _sIdStepTo ; }
    void                setIdStepTo(string sId)     { _sIdStepTo = sId ; }

    Cbalise*            getInBetween()              { return _pInBetween ; }
    void                setInBetween(Cbalise* pBa)  { _pInBetween = pBa ; }

    string              getLinkId()                 { return _sLinkId ; }
    void                setLinkId(string sId)       { _sLinkId = sId ; }

  protected:

    NSGuidelineControlVector* _pInterfaceControler ;

    DTslot*             _pSlotFrom ;
    string              _sIdStepFrom ;

    DTslot*             _pSlotTo ;
    string              _sIdStepTo ;

    Cbalise*            _pInBetween ;

    string              _sLinkId ;
} ;

typedef vector<DTlink*>              DTlinkVector ;
typedef DTlinkVector::iterator       DTlinkIter ;
typedef DTlinkVector::const_iterator DTlinkConstIter ;
typedef NTArray<DTlink>              DTlinkArray ;

class DTtoken
{
  public:

    enum TOKENSTATE { isFree, isWaiting, isBlocked, isToDelete } ;

    DTtoken() ;
    ~DTtoken() ;

    DTtoken(const DTtoken& rv) ;
    DTtoken& operator=(const DTtoken& src) ;

    DTslot* getCurrentSlot()              { return _pCurrentSlot ; }
    void    setCurrentSlot(DTslot* pSlot) ;

    string  getPreviousSlotId()           { return _sPreviousSlotId ; }
    void    setPreviousSlotId(string sId) { _sPreviousSlotId = sId ; }

    TOKENSTATE getTokenState()              { return _iState ; }
    void       setTokenState(TOKENSTATE iS) { _iState = iS ; }

    int     getTokenId()                  { return _iTokenId ; }
    string  getTokenIdAsString()          { return IntToString(_iTokenId) ; }
    void    setTokenId(int iTokenId)      { _iTokenId = iTokenId ; }

  protected:

    DTslot*        _pCurrentSlot ;
    string         _sPreviousSlotId ;
    vector<string> _aFatherSlotsStack ;
    TOKENSTATE     _iState ;
    int            _iTokenId ;
} ;

typedef vector<DTtoken*>        DTtokenVector ;
typedef DTtokenVector::iterator DTtokenIter ;
typedef NTArray<DTtoken>        DTtokenArray ;

enum GUIDELINEWNDTYPE { isUndefinedCtrl, isActionConcernCheckbox,
                        isDecisionButton, isDecisionRadioButton,
                        isGuidelineStatic, isSlotGroupBox, isLinkGroupBox,
                        isStandardGroup } ;

//--------------------------------------------------------------------------
// Structure qui sert � lier les contr�les � leur homologue BB
//--------------------------------------------------------------------------
struct NSGuidelineControl : public NSRoot
{
	private :

  	string              _sIdentity ;  // Control
    DTslot*             _pSlot ;	    // Controled slot
    DTlink*             _pLink ;	    // Controled link
    void *              _pNSCtrl ;    // Interface element
    string              _sFctName ;	  // Fonction name
    bool                _bVisible ;
    string              _sHelpText ;
    GUIDELINEWNDTYPE		_iType ;	    // Control's type
    NSDecisionTreeView *_pView ;

  public :

  	// Constructeurs, Destructeur
    NSGuidelineControl(NSContexte* pCtx) ;
    NSGuidelineControl(NSContexte* pCtx, DTslot* pSlot, string sIdentity, string sFctName) ;
    NSGuidelineControl(NSContexte* pCtx, DTlink* pLink, string sIdentity) ;
    NSGuidelineControl(const NSGuidelineControl& src) ;
    ~NSGuidelineControl() ;

    bool    canWeClose() ;
    HWND    getHWND() ;

    // activation du contr�le dans un �tat donn�
    void    activeControle(ACTIVATIONSTATE iState) ;
    void    prepareControle() ;
    void    executeKillFocusBehaviour() ;
    void    SetFocus() ;

    // Surcharges des op�rateurs
    NSGuidelineControl& operator=(const NSGuidelineControl& src) ;
    int                 operator==(const NSGuidelineControl& x) ;

    string              getIdentity()                      { return _sIdentity ; }
    void                setIdentity(string ident)          { _sIdentity = ident ; }

    string              getFunctionName()                  { return _sFctName ; }
    void                setFunctionName(string sFctName)   { _sFctName = sFctName ; }

    string              getHelpText()                      { return _sHelpText ; }
    void                setHelpText(string sHT)            { _sHelpText = sHT ; }

    GUIDELINEWNDTYPE    getType()                          { return _iType ; }
    void                setType(GUIDELINEWNDTYPE iTyp)     { _iType = iTyp ; }

    NSDecisionTreeView* getView()                          { return _pView ; }
    void                setView(NSDecisionTreeView* pView) { _pView = pView ; }

    void*               getControl()                       { return _pNSCtrl ; }
    void                setControl(void* pCtrl)            { _pNSCtrl = pCtrl ; }

    bool                isVisible()                        { return _bVisible ; }
    void                setVisible(bool bVisib)            { _bVisible = bVisib; }

    DTslot*             getSlot() { return _pSlot ;	}
    DTlink*             getLink() { return _pLink ; }

    bool        initOnSetupWindow() ;  //filling on setup
    bool        initOnFocus() ;        //filling on focus
    bool        initOnTimer() ;        //filling by OB1 over time
    bool        isToFilling(string str) ;

    string      getPath() ;
    string      cutPath(string *psPath, string sCutElement = string(""), bool bInclude = true) ;
    bool        getPathForBlackboard(string* psPathForBB) ;
    bool        getEndValidityDate(string* psValidityDate) ;

    bool        isEmpty() ;
};

typedef vector<NSGuidelineControl*> NSGuidelineControlVectorCtrl ;
typedef NSGuidelineControlVectorCtrl::iterator       iterNSGuidelineControl ;
typedef NSGuidelineControlVectorCtrl::const_iterator iterNSGuidelineControlConst ;

class NSGuidelineControlVector : public NSGuidelineControlVectorCtrl
{
	public :

  	// Constructeurs
    NSGuidelineControlVector() : NSGuidelineControlVectorCtrl() {}
    NSGuidelineControlVector(const NSGuidelineControlVector& rv) ;

    // Destructeur
    ~NSGuidelineControlVector() ;
    NSGuidelineControlVector& operator=(const NSGuidelineControlVector& src) ;

    void vider() ;
    void emptyWithoutDelete() ;
    void eraseControl(NSGuidelineControl *pGuidelineControl) ;
};

struct NSGuidelineInterfaceElement : public NSRoot
{
  public :

    NSGuidelineInterfaceElement(NSContexte* pCtx) ;
    virtual ~NSGuidelineInterfaceElement() ;

    void            setState(ACTIVATIONSTATE iState) { _iActivationState = iState ; }
    ACTIVATIONSTATE getState()                       { return _iActivationState ; }

    NSGuidelineControl* getControl()                          { return _pControl ; }
    virtual void        setControl(NSGuidelineControl* pCtrl) { _pControl = pCtrl ; }

  protected:

    NSGuidelineControl* _pControl ;
    ACTIVATIONSTATE     _iActivationState ;
} ;

class NSDecisionButton : public OWL::TButton, public NSGuidelineInterfaceElement
{
  public:

    string      sContenuBrut, sContenuTransfert, Type ;

    NSDecisionButton(NSContexte *pCtx, TWindow* parent, int resId) ;
    NSDecisionButton(NSContexte *pCtx, TWindow* parent, int resId, const char far* text,
                 int X, int Y, int W, int H, bool isDefault = false,
                 TModule* module = 0) ;
    ~NSDecisionButton() ;

    void activeControle(ACTIVATIONSTATE iState) ;

    GUIDELINEWNDTYPE donneType() { return(isDecisionButton) ; }
    bool             canWeClose() ;
    char far*        GetClassName() ;

    // child id notification handled at the child
    //
    void BNClicked() ;  // BN_CLICKED
    void SetupWindow() ;

    void setControl(NSGuidelineControl* pControl) ;

    string getQualityOfLife()           { return _sQualityOfLifeId ; }
    void   setQualityOfLife(string sAr) { _sQualityOfLifeId = sAr ; }

    string getArchetype()               { return _sArchetypeId ; }
    void   setArchetype(string sAr)     { _sArchetypeId = sAr ; }

    bool getActivation()                { return _bActiveToi ; }
    void setActivation(bool bActiv)     { _bActiveToi = bActiv ; }

  protected:

    bool   _bActiveToi ;	// activate control or just set focus
    string _sArchetypeId ;
    string _sQualityOfLifeId ;

	DECLARE_RESPONSE_TABLE(NSDecisionButton) ;
};

class NSActionConcernCheckBox : public OWL::TCheckBox, public NSGuidelineInterfaceElement
{
  public:

    // Constructeur et destructeur
    NSActionConcernCheckBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, OWL::TGroupBox* group, bool bStandardStyle = false, string sBackColor = string("")) ;
    NSActionConcernCheckBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far* title, int x,
                      int y, int w, int h, OWL::TGroupBox *group = 0, bool bStandardStyle = false, string sBackColor = string(""),
                      TModule* module = 0) ;
    ~NSActionConcernCheckBox() ;

    void    SetupWindow() ;
    bool    canWeClose() ;

    bool    isEmpty() { return _bIsEmpty ; }

    void    activeControle(ACTIVATIONSTATE iState) ;
    void    EvKeyUp(uint key, uint repeatcount, uint flags) ;
    TResult EvSetBackColor(TParam1 param1, TParam2 param2) ;

    char far* GetClassName() ;

    GUIDELINEWNDTYPE donneType() { return(isActionConcernCheckbox) ; }

    bool getActivation()            { return _bActiveToi ; }
    void setActivation(bool bActiv) { _bActiveToi = bActiv ; }

    void setControl(NSGuidelineControl* pControl) ;

    // child id notification handled at the child
    //
    void    BNClicked() ;  // BN_CLICKED

  protected:

    void   emptyConcernInformation() ;

    bool   _bActiveToi ;	// activate control or just set focus
    bool   _bTitle ;     // Le titre d�pendant de la langue a-t-il d�j� �t� donn�

    bool   _bIsEmpty ;  // Never been activated (checked or unchecked) by user
    bool   _bStandardStyle ;
    string _sBackColor ;

    string _sConcernCode ;
    string _sConcernFrame ;
    int    _iConcernSeverity ;

    NSDecisionTreeView *_pDTView ;

	DECLARE_RESPONSE_TABLE(NSActionConcernCheckBox) ;
} ;

class NSDecisionRadioButton : public OWL::TRadioButton, public NSGuidelineInterfaceElement
{
	public:

    NSDecisionRadioButton(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, OWL::TGroupBox* group) ;
    NSDecisionRadioButton(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far* title, int x,
                          int y, int w, int h, OWL::TGroupBox *group = 0,
                          TModule* module = 0) ;
    ~NSDecisionRadioButton() ;

    void      GetWindowClass(WNDCLASS far& wc) ;
    char far* GetClassName() ;
    void      SetupWindow() ;
    bool      canWeClose() ;

    void      activeControle(ACTIVATIONSTATE iState) ;
    void      EvKeyUp(uint key, uint repeatcount, uint flags) ;
    TResult   EvSetBackColor(TParam1 param1, TParam2 param2) ;

    void      Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void      EvPaint() ;
    void      PaintGroup(TDC& dc) ;
    void      PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    void      PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect) ;

    bool      isEmpty() { return _bIsEmpty ; }

    bool      getActivation()             { return _bActiveToi ; }
    void      setActivation(bool bActiv)  { _bActiveToi = bActiv ; }

    GUIDELINEWNDTYPE donneType() { return (isDecisionRadioButton) ; }

    // child id notification handled at the child
    //
    void        BNClicked() ;  // BN_CLICKED

  protected:

    bool _bActiveToi ;	// activate control or just set focus
    bool _bTitle ;     // Le titre d�pendant de la langue a-t-il d�j� �t� donn�

    bool _bIsEmpty ; // Never been activated (checked or unchecked) by user

    NSDecisionTreeView *_pDTView ;

	DECLARE_RESPONSE_TABLE(NSDecisionRadioButton) ;
};

class NSGuidelineStatic : public OWL::TStatic, public NSGuidelineInterfaceElement
{
	public:

    TWindow* 	parentEdit ;

    // Constructeur et destructeur
    NSGuidelineStatic(NSContexte* pCtx, NSDecisionTreeView* parent, int resId) ;
    NSGuidelineStatic(NSContexte* pCtx, NSDecisionTreeView* parent, int resId, const char far* title,
                    int x, int y, int w, int h,
                    uint textLimit = 0, TModule* module = 0) ;
    ~NSGuidelineStatic() ;

    bool    canWeClose() ;
    void    SetupWindow() ;

    GUIDELINEWNDTYPE donneType() { return(isGuidelineStatic) ; }

    void    EvSetFocus(HWND hWndLostFocus) ;
    void    activeControle(ACTIVATIONSTATE iState) ;

    void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void    EvPaint() ;
    void    PaintGroup(TDC& dc) ;
    void    PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    void    PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect) ;

    bool    getActivation()            { return _bActiveToi ; }
    void    setActivation(bool bActiv) { _bActiveToi = bActiv ; }

  protected:

    bool                _bActiveToi ; // activate control or just set focus
    NSDecisionTreeView *_pDTView ;

	DECLARE_RESPONSE_TABLE(NSGuidelineStatic) ;
};

class NSSlotGroupBox : public OWL::TGroupBox, public NSGuidelineInterfaceElement
{
  public:

    // Constructeur et destructeur    NSSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId) ;
    NSSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;    virtual ~NSSlotGroupBox() ;

    void    SetupWindow() ;

    void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void    EvPaint() ;

    void         PaintGroup(TDC& dc) ;
    virtual void PaintBackground(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    virtual void PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    void         PaintRoundFrame(TDC& dc, NS_CLASSLIB::TRect& rect, int iRadius, int iPenWidth) ;
    virtual void PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect) ;

    void    SelectionChanged(int controlId) ;

    GUIDELINEWNDTYPE donneType() { return(isSlotGroupBox) ; }

    // child id notification handled at the child
    //
    void    BNClicked() ;  // BN_CLICKED
    bool    canWeClose() ;
    void    activeControle(ACTIVATIONSTATE iState) ;

    bool    getActivation()            { return _bActiveToi ; }
    void    setActivation(bool bActiv) { _bActiveToi = bActiv ; }

  protected:

    bool                _bActiveToi ; // activate control or just set focus
    NSDecisionTreeView *_pDTView ;

  DECLARE_RESPONSE_TABLE(NSSlotGroupBox) ;
};

class NSActionSlotGroupBox : public NSSlotGroupBox
{
  public:

    // Constructeur et destructeur    NSActionSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId) ;
    NSActionSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;    ~NSActionSlotGroupBox() ;

    void    SetupWindow() ;

    void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void    EvPaint() ;
    void    PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    void    PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect) ;

    // child id notification handled at the child
    //
    void    BNClicked() ;  // BN_CLICKED

  DECLARE_RESPONSE_TABLE(NSActionSlotGroupBox) ;
};

class NSDecisionSlotGroupBox : public NSSlotGroupBox
{
  public:

    // Constructeur et destructeur    NSDecisionSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId) ;
    NSDecisionSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;    ~NSDecisionSlotGroupBox() ;

    void    SetupWindow() ;

    void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void    EvPaint() ;
    void    PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    void    PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect) ;

    // child id notification handled at the child
    //
    void    BNClicked() ;  // BN_CLICKED

  DECLARE_RESPONSE_TABLE(NSDecisionSlotGroupBox) ;
};

class NSDecisionStepSlotGroupBox : public NSSlotGroupBox
{
  public:

    // Constructeur et destructeur    NSDecisionStepSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId) ;
    NSDecisionStepSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;    ~NSDecisionStepSlotGroupBox() ;

    void    SetupWindow() ;

    void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void    EvPaint() ;
    void    PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    void    PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect) ;

    // child id notification handled at the child
    //
    void    BNClicked() ;  // BN_CLICKED

  DECLARE_RESPONSE_TABLE(NSDecisionStepSlotGroupBox) ;
};

class NSSynchroSlotGroupBox : public NSSlotGroupBox
{
  public:

    // Constructeur et destructeur    NSSynchroSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId) ;
    NSSynchroSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;    ~NSSynchroSlotGroupBox() ;

    void    SetupWindow() ;

    void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void    EvPaint() ;
    void    PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    void    PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect) ;

    // child id notification handled at the child
    //
    void    BNClicked() ;  // BN_CLICKED

  DECLARE_RESPONSE_TABLE(NSSynchroSlotGroupBox) ;
};

class NSLinkGroupBox : public OWL::TGroupBox, public NSGuidelineInterfaceElement
{
  public:

    // Constructeur et destructeur    NSLinkGroupBox(NSContexte *pCtx, TWindow* parent, int resId) ;
    NSLinkGroupBox(NSContexte *pCtx, TWindow* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;    ~NSLinkGroupBox() ;

    void    SetupWindow() ;

    GUIDELINEWNDTYPE donneType() { return(isLinkGroupBox) ; }

    // child id notification handled at the child
    //
    bool    canWeClose() ;
    void    activeControle(ACTIVATIONSTATE iState) ;

    void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void    EvPaint() ;
    void    PaintGroup(TDC& dc) ;
    void    PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect) ;
    void    PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect) ;

    char far* GetClassName() ;

    bool    getActivation()            { return _bActiveToi ; }
    void    setActivation(bool bActiv) { _bActiveToi = bActiv ; }

  protected:

    bool _bActiveToi ;	// activate control or just set focus
    bool _bLinkActivated ;

  DECLARE_RESPONSE_TABLE(NSLinkGroupBox) ;
};

class NSDecisionTreeDocument : public TDocument, public NSRoot
{
	public:

	 	NSDecisionTreeDocument(TDocument* parent = 0, NSContexte* pCtx = 0, string sArc = "",
              int iTypeFormulaire = 0, string sDefaultArc = "",
              bool bDefArch = false) ;
#ifdef __OB1__
    NSDecisionTreeDocument(TDocument* parent, NSContexte* pCtx, BB1BBInterfaceForKs* pKSdesc) ;
#endif
	 	NSDecisionTreeDocument(const NSDecisionTreeDocument& rv) ;
	 	~NSDecisionTreeDocument() ;
	 	NSDecisionTreeDocument& operator=(const NSDecisionTreeDocument& src) ;

		bool	GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
    						string sAdresseCorresp = "", string sPathDest = "") ;

		string  InitIntitule() ;

	 	// M�thodes de gestion des formulaires
		bool    ouvreArchetype() ;
	 	bool    enregistre(bool bVerbose = true, bool bSoaping = true) ;
		string  donneTexte() ;
		void    genereHtmlView() ;
		void    remplaceTagsChemin(string& sHtml) ;
    void    openReferential() ;

	 	// implementation des m�thodes virtuelles de TDocument
	 	bool   Open(int mode, const char far* path=0) ;
	 	bool   Close() ;
	 	bool   IsOpen() { return true ; }
	 	void   SetTitle(LPCSTR title) ;
		// void   SetDocName(char *nom);

    string findTitle() ;

    CguidelineStep*        getGuidelineStepFromId(string sStepId) ;

    string                 getArchetype()  { return _sArchetype ; }
    nsGuidelineParseur*    getParseur()    { return _pParseur ; }
    string                 getSemDocType() { return _sSemDocType ; }
    LDVFRAME               getFrame()      { return _iFrame ; }
    bool                   getParsingOk()  { return _bParsingOk ; }
    string                 getHelpUrl() ;

    void createSlots() ;
    void createSlotAndLinksForDecision(CdecisionStep *pDecision) ;
    void createSlotAndLinkForAction(CactionStep *pAction) ;
    void createSlotAndLinksForBranch(CbranchStep *pBranch) ;
    void createSlotAndLinkForSynchronization(CsynchronizationStep *pSynchro) ;
    void createSlotAndLinkForPatientState(CpatientStateStep *pPatientState) ;
    
    void completeLinks() ;

    void run(HWND interfaceHandle) ;
    bool runOneCycle() ;
    bool moveToken(DTtoken *pToken, DTslot* pToSlot) ;
    void activateSlot(DTslot* pToSlot) ;
    void startToken(DTslot* pToSlot, DTslot* pFromSlot) ;
    bool processSynchro(DTslot *pSlot) ;
    int  Interprete(string sReq, size_t& cc, DTslot *pSlot) ;
    bool carAutorise(char c, bool debut = false) ;
    bool isTokenOnSlot(string sOriginSlotName, DTslot *pSlot) ;

    void activateLink(string sLinkId, ACTIVATIONSTATE iState) ;

    DTslot* getSlotById(string sSlotId) ;
    DTlink* getLinkById(string sLinkId) ;

    string  buildLinkId(string sFrom, string sTo) ;

  protected:

    string                 _sSemDocType ;
    LDVFRAME               _iFrame ;

    DTslotArray            _aSlots ;
    DTlinkArray            _aLinks ;
    DTtokenArray           _aTokens ;

    nsGuidelineParseur*    _pParseur ;
		string                 _sArchetype ;
    string                 _sDefaultArchetype ;
    bool                   _bForceDefaultArch ;
#ifdef __OB1__
    BB1BBInterfaceForKs*   _pKSdescriptor ;
#endif
		int                    _iTypeDoc ;
		bool                   _bCreate ;
		bool                   _bParsingOk ;
		string                 _sHtmlView ;
};

/**
* View (doc/view) for Archetypes
* Vue (document/vue) pour les Archetypes
*/
class NSDecisionTreeView : public NSMUEView
{
	public:

		NSDecisionTreeDocument* _pDoc ; // Pointeur sur document formulaire

		OWL::TControlBar*       _pToolBar ;		    // Barre d'outils		bool			              _bSetupToolBar ;

		NSDecisionTreeView(NSDecisionTreeDocument& doc, TWindow *parent = 0) ;		~NSDecisionTreeView() ;

		void                PerformCreate(int menuOrId) ;
		void                SetupWindow() ;
    void                Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;
		void 			          lanceArchetype() ;    void                EvKeyDown(uint key, uint repeatCount, uint flags) ;    void                EvSysKeyDown(uint key, uint repeatCount, uint flags) ;    void                EvKeyUp(uint key, uint repeatCount, uint flags) ;    bool                PreProcessMsg(MSG &msg) ;		static LPCSTR       StaticName()    { return "NSCQWindowsView" ; }
		const char*         GetViewName()   { return StaticName() ; }
		TWindow*            GetWindow() ;
		void                setViewName() ;

		// Fonction d'initialisation		void 		            SetupToolBar() ;
		void		            EvSetFocus(THandle hWndLostFocus) ;
		void 		            EvClose() ;
		void                EvDestroy() ;
    TResult             EvSetBackColor(TParam1 param1, TParam2 param2) ;

    void                CmBbkUpdate() ;

    void                CreerControlesArchetype() ;
    TWindow*            CreateControl(Ccontrol *pControl, OWL::TGroupBox *pGroupCurr, int &hSysButton, string sLang) ;
    void                ResetControls() ;

		// Document notifications		bool 		            CanClose() ;		void 		            CmPublier() ;
		void 		            CmVisualiser() ;
		void 		            CmEnregistre() ;
    void                CmQuickSave() ;
		void                CmFileClose() ;
    void                CmHelp() ;
		void                CmOk() ;
    void                CmOkFinalStep() ;
    void                CmCancel() ;

    void                Compta() ;

    string              getIdentityFrom(string sIdentity) ;
    string              getIdentityTo(string sIdentity) ;

    NSGuidelineControlVector* getControlVectors() { return _pGuidelineControls ; }

    NS_CLASSLIB::TRect  getDialogRect()      { return _dlgRect ; }
    NS_CLASSLIB::TColor getBackgroundColor() { return _BackgroundColor ; }
    TDib               *getCheckedDib()      { return _pCheckedIconDib ; }

    Cdialogbox*         getDialogBox() ;

  protected:

    bool                _bCanClose ;
    bool                _bOkActivated ;
    bool                _bAcceptUpdate ;
    bool                _bNeedToRefresh ;  // a IDM_BBK_UPDATE event was received at a moment when it could not been processed
    NS_CLASSLIB::TRect  _dlgRect ;

    NSGuidelineControlVector *_pGuidelineControls ;

    NS_CLASSLIB::TColor _BackgroundColor ;
    TDib               *_pCheckedIconDib ;
    TTooltip           *_pToolTip ;

		// bool                PreProcessMsg(MSG& msg) ;

	DECLARE_RESPONSE_TABLE(NSDecisionTreeView) ;
} ;

#endif // __DECISIONTREEDV_H

