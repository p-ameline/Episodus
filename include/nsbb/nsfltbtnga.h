#if !defined(TOOL_FLTBTNGA_H)
#define TOOL_FLTBTNGA_H

#if !defined(OWL_BUTTONGA_H)
# include <owl/buttonga.h>
#endif
#if !defined(OWL_GADGET_H)
# include <owl/gadget.h>
#endif
#if !defined(WINSYS_GEOMETRY_H)
# include <winsys/geometry.h>
#endif

//
// class NSFlatButtonGadget
// ~~~~~ ~~~~~~~~~~~~~~~~~
//
class _NSBBCLASSE NSFlatButtonGadget : public TButtonGadget
{
  public:

    NSFlatButtonGadget(TResId resId, int id, TType type = Command,
                      bool enabled = false, TState state = Up,
                      bool sharedGlyph = false) ;

    NSFlatButtonGadget(TDib* pDib, int id, TType type = Command,
                      bool enabled = false, TState state = Up) ;

    ~NSFlatButtonGadget() ;

  protected:

    void Paint      (TDC& dc) ;
    void PaintBorder(TDC& dc) ;

    void MouseEnter(uint modKeys, ClassLib::TPoint& point) ;
    void MouseLeave(uint modKeys, ClassLib::TPoint& point) ;

    TDib* GetGlyphDib() ;
    void  ReleaseGlyphDib(TDib*) ;
    void  BuildCelArray() ;

  private:

    bool  _bMouseIsIn ;
    TDib* _pDib ;

    NS_CLASSLIB::TColor hotColor ;
    NS_CLASSLIB::TColor selectColor ;

    void BuildMask(TDC& maskDC, const ClassLib::TPoint& maskDst, const ClassLib::TSize& maskSize,
                   TDC& glyphDC, const ClassLib::TRect& glyphRect) ;
    void DitherBackground(TDC& dc, TDC& maskDC, const ClassLib::TRect& dstRect);
    void PaintFace(OWL::TBitmap& glyph, ClassLib::TRect& srcRect, TDC& dc, ClassLib::TRect& faceRect,
                   ClassLib::TPoint& dstPoint) ;
};

//
// class NSFlatSeparatorGadget
// ~~~~~ ~~~~~~~~~~~~~~~~~~~~
//
class _NSBBCLASSE NSFlatSeparatorGadget : public TGadget
{
  public:

    NSFlatSeparatorGadget(int w = 0, int h = 0, int id = 0) ;

    void Inserted() ;

  protected:

    void Paint(TDC& dc) ;

    virtual void PaintHorizontally(TDC& dc) ;
    virtual void PaintVertically  (TDC& dc) ;

  private:

    int width ;
    int height ;

    void UpdateBounds(int w, int h) ;
};

//
// class NSFlatGripperGadget
// ~~~~~ ~~~~~~~~~~~~~~~~~~
//
class _NSBBCLASSE NSFlatGripperGadget : public NSFlatSeparatorGadget
{
  public:

    NSFlatGripperGadget(int w = 0, int h = 0, int id = 0) ;

  protected:

    void MouseEnter(uint modKeys, ClassLib::TPoint& point) ;
    void MouseLeave(uint modKeys, ClassLib::TPoint& point) ;

    void LButtonDown(uint modKeys, ClassLib::TPoint& point) ;

    void PaintHorizontally(TDC& dc) ;
    void PaintVertically  (TDC& dc) ;
};

#endif  // TOOL_FLTBTNGA_H
