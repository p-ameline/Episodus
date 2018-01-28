
#include "nsbb\NSLabel.h"

DEFINE_RESPONSE_TABLE1(NSLabel, TStatic)
EV_WM_PAINT,
END_RESPONSE_TABLE ;


NSLabel::NSLabel(TWindow* parent, int id, const char far* title, int x, int y, int w, int h, uint textLimit , TModule* module  )
: TStatic(parent,id,title, x, y,  w,  h, textLimit, module )
{
}


NSLabel::NSLabel(TWindow* parent, int resourceId, uint textLimit, TModule* module ) :TStatic(parent, resourceId,textLimit, module)
{
}

void    NSLabel::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
  int temp = 0;
}

void    NSLabel::EvPaint()
{

  TPaintDC dc(*this); // DEvice context
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint;
  BOOL eras                = dc.Ps.fErase;
  HDC   hdrecp                = dc.Ps.hdc;

  TFont font("Times Roman", 20, 8);
  TDC useref(hdrecp);
  useref.SelectObject(font);
  useref.SetTextColor(NS_CLASSLIB::TColor::LtRed);
  NS_CLASSLIB::TPoint pointd(0,0);

  char lztxt[1024];
  GetText(lztxt, 1023);
 // SIZE lpsi;   cx et cy
    // ::GetTextExtentPoint32((hdrecp) , lztxt,  longueru du texte, &lpsi);
  useref.TextOut(pointd,lztxt);
  useref.RestoreFont();
}


NSLabel::~NSLabel()
{
}
