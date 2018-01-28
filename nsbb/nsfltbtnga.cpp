#include <owl/celarray.h>
#include <owl/gadgetwi.h>
#include <owl/uihelper.h>
#include <winsys/uimetric.h>

#include "nsbb\nsexport.h"
#include "nsbb\nsfltbtnga.h"
#include "nsbb\toolhlpr.h"
// #include "xpctlbar.h"

const long RopPSDPxax = 0x00B8074AL;
const long RopDSPDxax = 0x00E20746L;

// NSFlatButtonGadget implementation
//
NSFlatButtonGadget::NSFlatButtonGadget(TResId resId, int id, TType type,
                                     bool enabled, TState state,
                                     bool sharedGlyph)
                   :TButtonGadget(resId, id, type, enabled, state, sharedGlyph),
                    _bMouseIsIn(false), _pDib((TDib*) 0),
                    hotColor(193, 210, 238), selectColor(152, 181, 226)
{
}

NSFlatButtonGadget::NSFlatButtonGadget(TDib* pDib, int id, TType type,
                                       bool enabled, TState state)
                   :TButtonGadget(0, id, type, enabled, state, false /* shared */),
                    _bMouseIsIn(false)
{
  if (pDib)
  {
    _pDib = new TDib(*pDib) ;
    _pDib->MapUIColors( TDib::MapFace | TDib::MapText | TDib::MapShadow | TDib::MapHighlight ) ;
  }
}

NSFlatButtonGadget::~NSFlatButtonGadget()
{
  if (_pDib)
    delete _pDib ;
}

TDib*
NSFlatButtonGadget::GetGlyphDib()
{
  if (_pDib)
    return _pDib ;

  return TButtonGadget::GetGlyphDib() ;
}

//
// ReleaseGlyphDib().  Called by OWL to delete the dib returned by
// GetGlyphDib().  Since the dib is created in the constructor it
// is not deleted here.
//
void
NSFlatButtonGadget::ReleaseGlyphDib(TDib* pDib)
{
  if (NULL == _pDib)
    TButtonGadget::ReleaseGlyphDib(pDib) ;
}

//
// Build the CelArray member using the resource bitmap as the base glyph
// CelArray may contain an existing cel array that should be deleted if
// replaced.
//
// The CelArray and glyph painting can work in one of the following ways:
//   1. ResId ctor is used, ...., glyph states are cached in this CelArray
//   2. ResId ctor is used, CelArray holds single glyph and paints state on
//      the fly
//   3. ResId ctor is used, glyph is added to Window's CelArray and paints
//      state on the fly
//   4. glyphIndex ctor is used, uses Window's CelArray glyph and paints state
//      on the fly
//
void
NSFlatButtonGadget::BuildCelArray()
{
  PRECONDITION(Window);

  TDib*  glyph = GetGlyphDib();

  // Case 4, no resource of our own--using a cel in our Window's CelArray
  //
  if (!glyph)
    return;


  // Case 3, add (or replace old) our glyph to our Window's CelArray
  //
  TSize glyphSize(glyph->Width(), glyph->Height());
  TCelArray& celArray = Window->GetCelArray(glyph->Width(), glyph->Height());

  if (SharingGlyph && (glyphSize == celArray.CelSize())) {
    if (GlyphIndex >= 0)
      celArray.Replace(GlyphIndex, *glyph);
    else
      GlyphIndex = celArray.Add(*glyph);
  }
  else {

    // Case 2, build a CelArray containing only the normal glyph state
    //
    delete CelArray;
    CelArray = new TCelArray(*glyph, 1); //CelsTotal);
    GlyphIndex = 0;
    SharingGlyph = false;
  }

  ReleaseGlyphDib(glyph);

  // Case 1, for compatibility for now...
  //
}

void
NSFlatButtonGadget::Paint(TDC& dc)
{
  // Make sure our style matches our state, then paint our gadget borders
  //
  BorderStyle = (Pressed || State == Down) ? ButtonDn : ButtonUp;
  PaintBorder(dc);

  // Determine which CelArray to paint from, ours or our Window's
  //
  TCelArray& celArray = CelArray ? *CelArray : Window->GetCelArray();

  // Get the inner rect to use as the button face
  //
  TRect faceRect;
  GetInnerRect(faceRect);

  // Calculate the source rect from the celarray
  //
  TRect srcRect(celArray.CelRect(GlyphIndex));

  // The dest point of the glyph is relative to the face rect
  //
  TPoint dstPt(BitmapOrigin - faceRect.TopLeft());

  // Paint the button face
  //
  PaintFace(celArray, srcRect, dc, faceRect, dstPt);
}

void
NSFlatButtonGadget::PaintBorder(TDC& dc)
{
  if ((GetEnabled() && _bMouseIsIn) || GetButtonState())
  {
    TRect rect(0, 0, Bounds.Width(), Bounds.Height()) ;

    FillSolidRect(dc, rect, (Pressed ? selectColor : hotColor)) ;

    Draw3dRect(dc, rect, TColor::SysActiveCaption, TColor::SysActiveCaption) ;
  }
}

void
NSFlatButtonGadget::BuildMask(TDC& maskDC, const TPoint& maskDst,
                             const TSize& maskSize, TDC& glyphDC,
                             const TRect& glyphRect)
{
  maskDC.PatBlt(0, 0, maskSize.cx, maskSize.cy, BLACKNESS) ;

  // We use Sys3dFace as the mask color
  //
  TColor bkColor = glyphDC.SetBkColor(TColor::Sys3dFace) ;
  maskDC.BitBlt(maskDst.x, maskDst.y, maskSize.cx, maskSize.cy,
                glyphDC, glyphRect.left, glyphRect.top, SRCCOPY) ;
  glyphDC.SetBkColor(bkColor) ;
}

void
NSFlatButtonGadget::DitherBackground(TDC& dc, TDC& maskDC, const TRect& dstRect)
{
  THatch8x8Brush brush(THatch8x8Brush::Hatch11F1,
                       TColor::Sys3dFace,
                       TColor::Sys3dHilight) ;
  dc.SelectObject(brush) ;

  TColor txtColor = dc.SetTextColor(TColor::Black) ;
  TColor bkColor  = dc.SetBkColor  (TColor::White) ;

  dc.BitBlt(dstRect, maskDC, TPoint(0, 0), RopDSPDxax) ;

  dc.SetTextColor(txtColor) ;
  dc.SetBkColor  (bkColor) ;

  dc.RestoreBrush() ;
}

void
NSFlatButtonGadget::PaintFace(TBitmap& glyph, TRect& srcRect, TDC& dc,
                             TRect& faceRect, TPoint& dstPoint)
{
  // Select our glyph in a working memory dc
  //
  TMemoryDC memDC(glyph) ;
  memDC.SetTextColor(TColor::Black) ;
  memDC.SetBkColor  (TColor::White) ;

  // Calculate the glyph's destination rect
  //
  TRect dstRect(faceRect.TopLeft() + dstPoint, srcRect.Size()) ;
  dstRect &= faceRect ;

  // Build the mask
  //
  TBitmap* mask = new TBitmap(faceRect.Width(), faceRect.Height(), 1, 1, 0) ;
  TMemoryDC maskDC(*mask) ;
  BuildMask(maskDC, dstPoint, faceRect.Size(), memDC, srcRect) ;

  // Draw according the specified state
  //
  if (!GetEnabled())
  {
    // Inactivate mask - convert the highlight color to 1's on existing mask
    //
    memDC.SetBkColor(TColor::Sys3dHilight) ;
    maskDC.BitBlt(srcRect, memDC, TPoint(0, 0), SRCPAINT) ;

    // Gray the image using the standard button shadow color
    //
    dc.SelectObject(TBrush(TColor::Sys3dShadow)) ;

    TColor bkColor = dc.SetBkColor(TColor::Sys3dHilight) ;

    dc.BitBlt(dstRect, maskDC, dstPoint, RopPSDPxax) ;

    dc.SetBkColor(bkColor) ;

    dc.RestoreBrush() ;
  }
  else
  {
    // Do a transparent blit using the mask
    //
    const long DSTCOPY = 0x00AA0029 ;  // Undocumented ROP

    dc.MaskBlt(dstRect, memDC, srcRect.TopLeft(), *mask, dstPoint,
               MAKEROP4(DSTCOPY, SRCCOPY)) ;
  }

  // Do any filling determined by the state arguments
  //
  if (State == Indeterminate)
    // Dither the background everywhere except where the glyph is
    //
    DitherBackground(dc, maskDC, faceRect) ;

  maskDC.RestoreBitmap() ;

  delete mask ;
}

void
NSFlatButtonGadget::MouseEnter(uint modKeys, TPoint& point)
{
  TButtonGadget::MouseEnter(modKeys, point) ;

  _bMouseIsIn = true ;

  Invalidate() ;
}

void
NSFlatButtonGadget::MouseLeave(uint modKeys, TPoint& point)
{
  _bMouseIsIn = false ;

  TGadget::Invalidate(true) ;

  TButtonGadget::MouseLeave(modKeys, point) ;
}

// TFlatSeparatorGadget implementation
//
NSFlatSeparatorGadget::NSFlatSeparatorGadget(int w, int h, int id)
:
 TGadget(id)
{
  ShrinkWrapWidth = ShrinkWrapHeight = false;

  SetEnabled(false);

  width  = (w ? w : TUIMetric::CxSizeFrame * 2);
  height = (h ? h : TUIMetric::CxSizeFrame * 5);

  Bounds = TRect(0, 0, width, height);
}

void
NSFlatSeparatorGadget::Inserted()
{
  BorderStyle = None;
}

void
NSFlatSeparatorGadget::Paint(TDC& dc)
{
  TGadgetWindow::TTileDirection direction = Window->GetDirection();

  if (direction == TGadgetWindow::Horizontal) {
    if (Bounds.Width() != width)
      UpdateBounds(width, height);

    PaintVertically(dc);
  }
  else if (direction == TGadgetWindow::Vertical) {
    if (Bounds.Width() != height)
      UpdateBounds(height, width);

    PaintHorizontally(dc);
  }
  else if (Bounds.Height() != 0 || Bounds.Width() != 0)
    UpdateBounds(0, 0);
}

void
NSFlatSeparatorGadget::PaintHorizontally(TDC& dc)
{
  TRect rect(0, 0,  Bounds.Width(), Bounds.Height() / 2 + 1);
  TUIBorder(rect, TUIBorder::EdgeEtched, TUIBorder::Bottom).Paint(dc);
}

void
NSFlatSeparatorGadget::PaintVertically(TDC& dc)
{
  TRect rect(0, 0,  Bounds.Width() / 2 + 1, Bounds.Height());
  TUIBorder(rect, TUIBorder::EdgeEtched, TUIBorder::Right).Paint(dc);
}

void
NSFlatSeparatorGadget::UpdateBounds(int w, int h)
{
  Bounds = TRect(0, 0, w, h);
  Window->LayoutSession();
  Window->Invalidate();
}

// TFlatGripperGadget implementation
//
NSFlatGripperGadget::NSFlatGripperGadget(int w, int h, int id)
                    :NSFlatSeparatorGadget(w, h, id)
{
  SetEnabled(true);
}

void
NSFlatGripperGadget::MouseEnter(uint modKeys, TPoint& point)
{
  NSFlatSeparatorGadget::MouseEnter(modKeys, point);

  Window->SetCursor(0, IDC_SIZEALL);
}

void
NSFlatGripperGadget::MouseLeave(uint modKeys, TPoint& point)
{
  Window->SetCursor(0, IDC_ARROW);

  NSFlatSeparatorGadget::MouseLeave(modKeys, point);
}

void
NSFlatGripperGadget::LButtonDown(uint modKeys, TPoint& point)
{
  Window->HandleMessage(WM_LBUTTONDOWN, modKeys,
                        MkParam2(int16(point.x), int16(point.y)));
}

void
NSFlatGripperGadget::PaintHorizontally(TDC& dc)
{
  TColor gripperColor = DarkenColor(TColor::Sys3dFace, 100);

  int w = Bounds.Width ();
  for (int x = 0; x < w; x += 4)
    FillSolidRect(dc, x, 1, 2, 2, gripperColor);
}

void
NSFlatGripperGadget::PaintVertically(TDC& dc)
{
  TColor gripperColor = DarkenColor(TColor::Sys3dFace, 100);

  int h = Bounds.Height();
  for (int y = 0; y < h; y += 4)
    FillSolidRect(dc, 1, y, 2, 2, gripperColor);
}

