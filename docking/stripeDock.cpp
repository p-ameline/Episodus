// ****************************************************************************
// ****************************************************************************

#include <owl\owlpch.h>
#pragma hdrstop

#include <owl/uihelper.h>  // for TUIBorder edge painting
#include <owl/textgadg.h>
#include <owl/glyphbtn.h>  

#include "nautilus\nssuper.h"
#include "nsepisod\nsToDo.h"
#include "nsbb\ns_skins.h"

#include "docking\stripeDock.h"
#include "docking\stripes.rh"
#include "docking\stripes_icons.rc"

//#define CTXHELP

#ifdef  CTXHELP
#include "CtxHelpM.h"
#endif


// ******************************* constants ***********************************

const GripperDistSize = 3; // Distance from gripper to gripper and size of gripper
const FloatMarginsX   = 4; // Left and Right Margins, FloatingSlip
const FloatMarginsY   = 3; // Top and Bottom Margins, FloatingSlip
const EdgeMaginsX     = 5; // Left and Right Margins, EdgeSlip
const EdgeMaginsY     = 5; // Top and Bottom Margins, EdgeSlip

// ******************************* helper functions ***************************

void Draw3dRect(HDC hdc, const ClassLib::TRect& rect, COLORREF clrTopLeft, COLORREF clrBottomRight)
// MFC style function, that draw the frame of a 3d rect with given colors
{
	if (hdc == 0)
  	return ;

	Draw3dRect(hdc, rect.left, rect.top, rect.Width(), rect.Height(), clrTopLeft, clrBottomRight) ;
}

void Draw3dRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	if (hdc == 0)
  	return ;

	FillSolidRect(hdc, x, y, cx - 1, 1, clrTopLeft) ;
	FillSolidRect(hdc, x, y, 1, cy - 1, clrTopLeft) ;
	FillSolidRect(hdc, x + cx, y, -1, cy, clrBottomRight) ;
	FillSolidRect(hdc, x, y + cy, cx, -1, clrBottomRight) ;
}

void FillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr)
// MFC style function, that draws a filled rect. Is this faster???
{
	if (hdc == 0)
  	return ;

	::SetBkColor(hdc, clr) ;
	ClassLib::TRect rect(x, y, x + cx, y + cy) ;
	::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, 0, 0, 0) ;
}

// ************************* NsStripe *************************

DEFINE_RESPONSE_TABLE1(NsStripe, TDockableControlBarEx)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(CM_STRIPES_CLOSE, CmOk),
  EV_COMMAND(CM_STRIPES_URL,   CmHelp),
  EV_COMMAND_ENABLE(CM_STRIPES_CLOSE, CmEnableButton),
  EV_COMMAND_ENABLE(CM_STRIPES_URL,   CmEnableButton),
END_RESPONSE_TABLE;


// default constructor
//
NsStripe::NsStripe(NSContexte *pCtx, string sText, string sURL, TWindow* parent,
                   STRIPETYPE iStripeType,
                   TTileDirection direction,
                   TFont* font, TModule* module)
         :TDockableControlBarEx(parent, direction, font, module)
{
  pContexte   = pCtx ;
  _sText      = sText ;
  _sUrl       = sURL ;
  _stripeType = iStripeType ;

  initColors() ;

  ClassLib::TRect appRect = pContexte->GetMainWindow()->GetWindowRect() ;
  Attr.W = appRect.Width() ;
  // ToggleRect  = TRect(-1, -1, appRect.Width(), 10) ;

  _bFullWidth = true ;

  _pTextGadget = new NsStripeTextGadget(this, IDG_MESSAGE, TGadget::None, TTextGadget::Center, 10, 0) ;
  _pTextGadget->WideAsPossible = true ;
  Insert(*_pTextGadget) ;
}

// constructor that set additional the Attr.id for context help and OLE 2,
// title for the caption, and the flag for a default controlbar
//
NsStripe::NsStripe(NSContexte *pCtx, uint id, string sText, string sURL,
                   const char* title,
                   bool _Default, TWindow* parent,
                   TTileDirection direction, TFont* font, TModule* module)
         :TDockableControlBarEx(id, title, _Default, parent, direction, font, module)
{
  pContexte   = pCtx ;
  _sText      = sText ;
  _sUrl       = sURL ;

  _bFullWidth = true ;

  initColors() ;

  _pTextGadget = new NsStripeTextGadget(this, IDG_MESSAGE, TGadget::None, TTextGadget::Center, 10, 0) ;
  _pTextGadget->WideAsPossible = true ;
  Insert(*_pTextGadget) ;
}

void
NsStripe::initColors()
{
  _clrMain    = TColor::Sys3dFace ;
  _clrHilight = TColor::Sys3dHilight ;
  _clrShadow  = TColor::Sys3dShadow ;

  nsSkin* pSkin ;

  switch(_stripeType)
  {
    case isInformation :

      pSkin = pContexte->getSkins()->donneSkin(string("stripeInformation")) ;
      if (pSkin)
        _clrMain    = *(pSkin->getBackColor()) ;
      else
        _clrMain    = TColor(0, 128, 0) ;

      pSkin = pContexte->getSkins()->donneSkin(string("stripeInformationHilight")) ;
      if (pSkin)
        _clrHilight = *(pSkin->getBackColor()) ;
      else
        _clrHilight = TColor::LtGreen ;

      pSkin = pContexte->getSkins()->donneSkin(string("stripeInformationShadow")) ;
      if (pSkin)
        _clrShadow  = *(pSkin->getBackColor()) ;
      else
        _clrShadow  = TColor::Sys3dShadow ;

      break ;

    case isWarning :

      pSkin = pContexte->getSkins()->donneSkin(string("stripeWarning")) ;
      if (pSkin)
        _clrMain    = *(pSkin->getBackColor()) ;
      else
        _clrMain    = TColor(128, 128, 0) ;

      pSkin = pContexte->getSkins()->donneSkin(string("stripeWarningHilight")) ;
      if (pSkin)
        _clrHilight = *(pSkin->getBackColor()) ;
      else
        _clrHilight = TColor::LtYellow ;

      pSkin = pContexte->getSkins()->donneSkin(string("stripeWarningShadow")) ;
      if (pSkin)
        _clrShadow  = *(pSkin->getBackColor()) ;
      else
        _clrShadow  = TColor::Sys3dShadow ;

      break ;

    case isAlert :

      pSkin = pContexte->getSkins()->donneSkin(string("stripeAlert")) ;
      if (pSkin)
        _clrMain    = *(pSkin->getBackColor()) ;
      else
        _clrMain    = TColor(128, 0, 0) ;

      pSkin = pContexte->getSkins()->donneSkin(string("stripeAlertHilight")) ;
      if (pSkin)
        _clrHilight = *(pSkin->getBackColor()) ;
      else
        _clrHilight = TColor::LtRed ;

      pSkin = pContexte->getSkins()->donneSkin(string("stripeAlertShadow")) ;
      if (pSkin)
        _clrShadow  = *(pSkin->getBackColor()) ;
      else
        _clrShadow  = TColor::Sys3dShadow ;

      break ;
  }
}

NsStripe::~NsStripe()
{
  Destroy(IDCANCEL) ;
}

void
NsStripe::LayoutSession()
{
  ClassLib::TRect appRect = pContexte->GetMainWindow()->GetWindowRect() ;
  Attr.W = appRect.Width() ;

  TDockableControlBarEx::LayoutSession() ;
}

void
NsStripe::CmEnableButton(TCommandEnabler& tce)
{
  tce.Enable(true); // or false, if you have a need to disable the button
}

void
NsStripe::SetupWindow()
{
  TDockableControlBarEx::SetupWindow() ;
}

void
NsStripe::addUtilityGadgets()
{
  // _pCloseButton = new NsStripeCloseButton(this, CM_STRIPES_CLOSE, "", 0, 0, 0, 0) ;
  // _pCloseButton->Create() ;
  // Insert(*new TControlGadget(*_pCloseButton)) ;

  // Insert(*new NsCloseButtonGadget(this, CM_STRIPES_CLOSE, CM_STRIPES_CLOSE, TButtonGadget::Command)) ;

  if (string("") != _sUrl)
  {
    Insert(*new TSeparatorGadget) ;
    Insert(*new TButtonGadget(CM_STRIPES_URL, CM_STRIPES_URL, TButtonGadget::Command)) ;
  }
  Insert(*new TSeparatorGadget) ;
  Insert(*new TButtonGadget(CM_STRIPES_CLOSE, CM_STRIPES_CLOSE, TButtonGadget::Command)) ;
}

void
NsStripe::Paint(TDC& dc, bool erase, ClassLib::TRect& rect)
{
  // When docked, this paints an etched border just
  // inside the client area of the control bar
  // TDockableControlBar::Paint(dc, erase, rect) ;

  FillSolidRect(dc, rect.X(), rect.Y(), rect.Width(), rect.Height(), _clrMain) ;
  // RedrawGadgets(dc, rect) ;
  PaintGadgets(dc, erase, rect) ;

  TDockingSlip* Slip = TYPESAFE_DOWNCAST(Parent, TDockingSlip) ;
  if (Slip && Slip->GetLocation() != alNone)
  {
    TWindowDC MyDC(*this) ;
    ClassLib::TRect ThisClientRect = GetClientRect() ;
    TUIBorder(ThisClientRect, TUIBorder::EdgeEtched, TUIBorder::Left).Paint(MyDC) ;
    TUIBorder(ThisClientRect, TUIBorder::EdgeEtched, TUIBorder::Right).Paint(MyDC) ;
    TUIBorder(ThisClientRect, TUIBorder::EdgeEtched, TUIBorder::Bottom).Paint(MyDC) ;
    TUIBorder(ThisClientRect, TUIBorder::EdgeEtched, TUIBorder::Top).Paint(MyDC) ;

    THarborEx* harbor = TYPESAFE_DOWNCAST(GetHarbor(), THarborEx) ;
    if (harbor->IsDrawGripper())
    {
	    ClassLib::TRect gripperRect = ThisClientRect ;

      if (Slip->GetLocation() == alTop || Slip->GetLocation() == alBottom)
      {
        gripperRect.Inflate(-GripperDistSize-1, -GripperDistSize) ;
        gripperRect.right = gripperRect.left + GripperDistSize ;
        Draw3dRect(MyDC, gripperRect, _clrHilight, _clrShadow) ;
        gripperRect.Offset(GripperDistSize, 0) ;
        Draw3dRect(MyDC, gripperRect, _clrHilight, _clrShadow) ;
      }
      else
      {
        gripperRect.Inflate(-GripperDistSize, -GripperDistSize-1) ;
        gripperRect.bottom = gripperRect.top + GripperDistSize ;
        Draw3dRect(MyDC, gripperRect, _clrHilight, _clrShadow) ;
        gripperRect.Offset(0, GripperDistSize) ;
        Draw3dRect(MyDC, gripperRect, _clrHilight, _clrShadow) ;
      }
    }
  }
}

void
NsStripe::RedrawGadgets(TDC& dc, ClassLib::TRect& rect)
{
  TGadget* pGadget = FirstGadget() ;
  while (pGadget)
  {
    RedrawGadget(pGadget, dc, rect) ;
    pGadget = NextGadget(*pGadget) ;
  }
}

void
NsStripe::RedrawGadget(TGadget* pGadget, TDC& dc, ClassLib::TRect& rect)
{
  if (NULL == pGadget)
    return ;

  NsStripeTextGadget* pTextGadget = TYPESAFE_DOWNCAST(pGadget, NsStripeTextGadget) ;
  if (pTextGadget)
    pTextGadget->Paint(dc) ;
}

void
NsStripe::CmOk()
{
  THarborEx* pHarbor = pContexte->getSuperviseur()->getApplication()->getHarbor() ;
  if (pHarbor)
    pHarbor->Remove(*this) ;

  Destroy(IDOK) ;
}

void
NsStripe::CmHelp()
{
}

//
// Set the text for the message bar by puting it in the message text gadget
//
void
NsStripe::SetText(string sText)
{
  SetMessageText(IDG_MESSAGE, sText.c_str()) ;
}

//
// Set the text for the text gadget with a given ID
//
void
NsStripe::SetMessageText(int id, const char* text)
{
  NsStripeTextGadget* tg = TYPESAFE_DOWNCAST(GadgetWithId(id), NsStripeTextGadget) ;
  if (tg)
    tg->SetText(text);
}

//
// When the gadget window receives a WM_COMMAND_ENABLE message, it is likely
// from a gadget or control within a TControlGadget. Reroute to the command
// target.
//
void
NsStripe::EvCommandEnable(TCommandEnabler& ce)
{
  // Default to TWindow's implementation if the above routing fails
  //
  TWindow::EvCommandEnable(ce);
}

// ************************* NsPreoccupStripe *************************

DEFINE_RESPONSE_TABLE1(NsPreoccupStripe, NsStripe)
END_RESPONSE_TABLE;


// default constructor
//
NsPreoccupStripe::NsPreoccupStripe(NSContexte *pCtx, string sPreoccup,
                                   string sDate, string sText, string sURL,
                                   TWindow* parent, STRIPETYPE iStripeType,
                                   TTileDirection direction,
                                   TFont* font, TModule* module)
                 :NsStripe(pCtx, sText, sURL, parent, iStripeType, direction, font, module)
{
  _sText         = sText ;
  _sPreoccup     = sPreoccup ;
  _sStartingDate = sDate ;
  _pCreateButton = 0 ;
  _sUrl          = sURL ;
}

NsPreoccupStripe::~NsPreoccupStripe()
{
  if (_pCreateButton)
    delete _pCreateButton ;

  Destroy(IDCANCEL) ;
}

void
NsPreoccupStripe::SetupWindow()
{
  NsStripe::SetupWindow() ;

  string sLangue = pContexte->getUserLanguage() ;
  SetCaption("NsPreoccupStripe") ;
  SetText(_sText) ;

  // Insert(*new NsStripeTextGadget(this, ID_STRIPES_TEXT, TGadget::Recessed, TTextGadget::Center, strlen(_sText.c_str()), _sText.c_str())) ;
  Insert(*new TSeparatorGadget) ;

  if (string("") != _sPreoccup)
  {
    string sCreateTxt = pContexte->getSuperviseur()->getText("stripes", "createConcern") ;

    string sConcernLabel ;
    pContexte->getDico()->donneLibelle(sLangue, &_sPreoccup, &sConcernLabel) ;

    string sButtonText = sCreateTxt + string(" ") + sConcernLabel ;

    _pCreateButton = new NsStripeButton(this, CM_STRIPES_ACTION, sButtonText.c_str(), 0, 0, 50, 15) ;
    _pCreateButton->SetDesiredSize() ;
    _pCreateButton->Create() ;

    Insert(*new TControlGadget(*_pCreateButton)) ;
  }

  // if (string("") != _sUrl)
  //  Insert(*new TButtonGadget(CM_STRIPES_URL, CM_STRIPES_URL, TButtonGadget::Command)) ;
  // Insert(*new TSeparatorGadget) ;

  addUtilityGadgets() ;

  // Insert(*new TButtonGadget(CM_STRIPES_CLOSE, CM_STRIPES_CLOSE, TButtonGadget::Command)) ;
}

void
NsPreoccupStripe::buttonCheck(int iButtonId)
{
  if (CM_STRIPES_ACTION == iButtonId)
  {
    NSToDoTask* pTask = new NSToDoTask ;

		pTask->setWhatToDo(string("NewPreoccup")) ;
		pTask->setParam1(_sPreoccup) ;
		pTask->setParam2(_sStartingDate) ;

		pTask->sendMe(pContexte->getSuperviseur()) ;

    CmOk() ;
  }
}

// ************************* NsAlertStripe *************************

DEFINE_RESPONSE_TABLE1(NsAlertStripe, NsStripe)
END_RESPONSE_TABLE;

// default constructor
//
NsAlertStripe::NsAlertStripe(NSContexte *pCtx, string sText, string sURL, int iEmergencyLevel,
                             TWindow* parent, STRIPETYPE iStripeType,
                             TTileDirection direction,
                             TFont* font, TModule* module)
              :NsStripe(pCtx, sText, sURL, parent, iStripeType, direction, font, module)
{
  _sText           = sText ;
  _sUrl            = sURL ;
  _iEmergencyLevel = iEmergencyLevel ;
}

NsAlertStripe::~NsAlertStripe()
{
  Destroy(IDCANCEL) ;
}

void
NsAlertStripe::SetupWindow()
{
  SetCaption("NsAlertStripe") ;
  SetText(_sText) ;

  addUtilityGadgets() ;
}

void
NsAlertStripe::buttonCheck(int iButtonId)
{
}

// ************************* NsStripeTextGadget *************************

NsStripeTextGadget::NsStripeTextGadget(NsStripe* pStripe, int id,
                                       TBorderStyle borderStyle,
                                       TAlign align, uint numChars,
                                       const char far* text, TFont* font)
                   :TTextGadget(id, borderStyle, align, numChars, text, font)
{
  _pStripe = pStripe ;
}

NsStripeTextGadget::~NsStripeTextGadget()
{
}

void
NsStripeTextGadget::Invalidate()
{
  TTextGadget::Invalidate() ;
}

//
// Paint the text gadget by painting gadget borders, & then painting text in
// the InnerRect. Empty or 0 text blanks the gadget.
//
void
NsStripeTextGadget::Paint(TDC& dc)
{
  PaintBorder(dc);

  ClassLib::TRect  innerRect;
  GetInnerRect(innerRect);
  int x = innerRect.left;
  int y = innerRect.top;

  if (Font == 0)
    dc.SelectObject(Window->GetFont());
  else
    dc.SelectObject(*Font);

  // Adjust the horizontal position for centered or right justified text
  //
  if (Text && TextLen && Align != Left)
  {
    ClassLib::TSize extent = dc.GetTextExtent(Text, TextLen);

    if (extent.cx < innerRect.Width()) {
      int  diff = innerRect.Width() - extent.cx;

      if (Align == Center)
        diff /= 2;

      x += diff;
    }
  }

  // Use ExtTextOut() to paint the text & background rectangle

  COLORREF clrBackground   = TColor::Sys3dFace ;
  COLORREF clrInactiveText = TColor::SysGrayText ;
  COLORREF clrActiveText   = TColor::SysBtnText ;

  switch(_pStripe->getStripeType())
  {
    case NsStripe::isInformation :
      clrBackground   = TColor(0, 128, 0) ;
      clrInactiveText = TColor::SysGrayText ;
      clrActiveText   = TColor::White ;
      break ;
    case NsStripe::isWarning :
      clrBackground   = TColor(128, 0, 0) ; ;
      clrInactiveText = TColor::SysGrayText ;
      clrActiveText   = TColor::White ;
      break ;
    case NsStripe::isAlert :
      clrBackground   = TColor(128, 0, 0) ;
      clrInactiveText = TColor::SysGrayText ;
      clrActiveText   = TColor::White ;
      break ;
  }

  //
  dc.SetBkColor(clrBackground);
  dc.SetTextColor(GetEnabled() ? clrActiveText : clrInactiveText);
  dc.ExtTextOut(x, y, ETO_CLIPPED | ETO_OPAQUE, &innerRect, Text, TextLen);
  dc.RestoreFont();
}

// ************************* NsStripeButton *************************

DEFINE_RESPONSE_TABLE1(NsStripeButton, TButton)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),END_RESPONSE_TABLE;

//
// Constructeur
//
NsStripeButton::NsStripeButton(NsStripe* pStripe, int resId,
                               const char far* text, int X, int Y, int W, int H,
                               TFont* font, bool isDefault, TModule* module)
               :TButton((TWindow*) pStripe, resId, text, X, Y, W, H, isDefault, module)
{
	DisableTransfer() ;
  _pStripe = pStripe ;
  _pFont   = font ;
  _sText   = string(text) ;
}

//
// Destructeur
//
NsStripeButton::~NsStripeButton()
{
}

void
NsStripeButton::SetupWindow()
{
  OWL::TButton::SetupWindow() ;
}

//
// Fonction utilisée lorsque le bouton est cliqué
//
void
NsStripeButton::BNClicked()
{
  if (_pStripe)
    _pStripe->buttonCheck(GetId()) ;
}

//
// Respond to the virtual call to let this gadget's Window know how big this
// text gadget wants to be based on the text size.
//
void
NsStripeButton::SetDesiredSize()
{
  bool bDeleteFont = false ;

  TFont* font = _pFont ;
  if (font == 0)
  {
    HFONT hSystemFont = (HFONT)GetStockObject(SYSTEM_FONT) ;
		LOGFONT systemFont ;
		::GetObject(hSystemFont, sizeof(LOGFONT), (void*)&systemFont) ;

    font = new TFont(&systemFont) ;

    bDeleteFont = true ;
  }

  if (NULL == font)
    return;

  Attr.W = font->GetTextExtent(_sText.c_str()).cx + 2 ;
  Attr.H = font->GetHeight() + 2 ;

  if (bDeleteFont)
    delete font ;
}

char far*
NsStripeButton::GetClassName()
{
  return "BUTTON" ;
}

// ************************* NsCloseButtonGadget *************************

NsCloseButtonGadget::NsCloseButtonGadget(NsStripe* pStripe,
                                         TResId glyphResIdOrIndex,
                                         int    id,
                                         TType  type,
                                         bool   enabled,
                                         TState state,
                                         bool   sharedGlyph)
                    :TButtonGadget(glyphResIdOrIndex, id, type, enabled, state, sharedGlyph)
{
  _pStripe = pStripe ;
}

NsCloseButtonGadget::~NsCloseButtonGadget()
{
}

