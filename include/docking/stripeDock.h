// ****************************************************************************// ****************************************************************************

#ifndef __STRIPEDOCK_H#define __STRIPEDOCK_H

#include <owl/glyphbtn.h>
#include <owl/textgadg.h>
#include <owl/buttonga.h>  

class NsStripe ;
class NSContexte ;

// #include "nautilus/nssuper.h"
#include "docking/dockinge.h"
class NsStripeTextGadget : public TTextGadget {

  public:

    NsStripeTextGadget(NsStripe* pStripe, int id = 0,
                       TBorderStyle = None, TAlign = Left,
                       uint numChars = 10, const char far* text = 0,
                       OWL::TFont* font = 0 /*new TGadgetWindowFont*/);
   ~NsStripeTextGadget() ;

   void Invalidate() ;
   void Paint(TDC& dc) ;

  protected:

    NsStripe* _pStripe ;
} ;

class NsCloseButtonGadget : public TButtonGadget {

  public:

    NsCloseButtonGadget(NsStripe* pStripe,
                        TResId glyphResIdOrIndex,
                        int    id,
                        TType  type = Command,
                        bool   enabled = false, // initial state before cmd enabling
                        TState state = Up,
                        bool   sharedGlyph = false) ;
   ~NsCloseButtonGadget() ;

  protected:

    NsStripe* _pStripe ;
} ;

class NsStripeButton : public OWL::TButton
{
  public:

    NsStripeButton(NsStripe* pStripe, int resId, const char far* text,
                   int X, int Y, int W, int H, OWL::TFont* font = 0, bool isDefault = false,
                   TModule* module = 0) ;
    ~NsStripeButton() ;

    char far* GetClassName() ;

    // child id notification handled at the child
    //
    void BNClicked() ;  // BN_CLICKED
    void SetupWindow() ;
    void SetDesiredSize() ;

  protected:

    NsStripe*   _pStripe ;
    OWL::TFont* _pFont ;
    string      _sText ;

	DECLARE_RESPONSE_TABLE(NsStripeButton) ;
};

// ****************** NsStripe ***********************************

class NsStripe : public TDockableControlBarEx
{
  public:

    enum STRIPETYPE { isUndefined = 0, isInformation, isWarning, isAlert } ;

    // default constructor    NsStripe(NSContexte *pCtx, string sText, string sURL, TWindow* parent = 0,             STRIPETYPE iStripeType = isUndefined,             TTileDirection direction = Horizontal,
             OWL::TFont* font = new TGadgetWindowFont,
             OWL::TModule* module = 0) ;

    // constructor that set additional the Attr.id for context help and OLE 2,
    // title for the caption, and the flag for a default controlbar.
    NsStripe(NSContexte *pCtx, uint id, string sText, string sURL,             const char* title,
             bool _Default = true,
             TWindow* parent = 0,
             TTileDirection direction = Horizontal,
             OWL::TFont* font = new TGadgetWindowFont,
             OWL::TModule* module = 0) ;

    virtual ~NsStripe() ;

    // Overridden to paint the etched toolbar border and the gripper
    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& rect) ;
    void SetupWindow() ;
    void EvCommandEnable(TCommandEnabler& ce) ;
    void CmEnableButton(TCommandEnabler& tce) ;
    void CmOk() ;
    void CmHelp() ;

    void initColors() ;
    void addUtilityGadgets() ;

    void LayoutSession() ;
    void RedrawGadgets(TDC& dc, ClassLib::TRect& rect) ;
    void RedrawGadget(TGadget* pGadget, TDC& dc, ClassLib::TRect& rect) ;

    void SetText(string sText) ;
    void SetMessageText(int id, const char* text) ;

    STRIPETYPE getStripeType()               { return _stripeType ; }
    void       setStripeType(STRIPETYPE iST) { _stripeType = iST ; }

    COLORREF   getMainColor()    { return _clrMain    ; }
    COLORREF   getTextColor()    { return _clrText    ; }
    COLORREF   getHilightColor() { return _clrHilight ; }
    COLORREF   getShadowColor()  { return _clrShadow  ; }

    virtual void buttonCheck(int iButtonId) = 0 ;

  protected:

    NsStripeTextGadget  *_pTextGadget ;

    STRIPETYPE _stripeType ;
    NSContexte *pContexte ;

    string     _sText ;
    string     _sUrl ;

    COLORREF _clrMain    ;
    COLORREF _clrText    ;
    COLORREF _clrHilight ;
    COLORREF _clrShadow  ;

    bool     _bFullWidth ;

  DECLARE_RESPONSE_TABLE(NsStripe) ;
};

// *********************** NsPreoccupStripe ***********************

class NsPreoccupStripe : public NsStripe
{
  public:

    // default constructor
    NsPreoccupStripe(NSContexte *pCtx, string sPreoccup, string sDate,                     string sText, string sURL,                     TWindow* parent = 0,                     STRIPETYPE iStripeType = isUndefined,                     TTileDirection direction = Horizontal,
                     OWL::TFont* font = new TGadgetWindowFont,
                     OWL::TModule* module = 0) ;

    virtual ~NsPreoccupStripe() ;

    void SetupWindow() ;

    void buttonCheck(int iButtonId) ;

  protected:

    string          _sPreoccup ;
    string          _sStartingDate ;
    NsStripeButton *_pCreateButton ;

  DECLARE_RESPONSE_TABLE(NsPreoccupStripe) ;
};

// *********************** NsPreoccupStripe ***********************

class NsAlertStripe : public NsStripe
{
  public:

    // default constructor
    NsAlertStripe(NSContexte *pCtx, string sText, string sURL, int iEmergencyLevel,                  TWindow* parent = 0,                  STRIPETYPE iStripeType = isUndefined,                  TTileDirection direction = Horizontal,
                  OWL::TFont* font = new TGadgetWindowFont,
                  OWL::TModule* module = 0) ;

    virtual ~NsAlertStripe() ;

    void SetupWindow() ;

    void buttonCheck(int iButtonId) ;

  protected:

    int    _iEmergencyLevel ;

  DECLARE_RESPONSE_TABLE(NsAlertStripe) ;
};

#endif

