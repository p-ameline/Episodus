// -----------------------------------------------------------------------------
// NSLabel.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:03:17 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#ifndef __NSLABEL_H__
# define __NSLABEL_H__
# include <owl\tabctrl.h>
# include <owl\commctrl.h>
# include <owl\static.h>

class NSLabel : public TStatic
{
  public:
    NSLabel(TWindow* parent, int id, const char far* title, int x, int y, int w, int h, uint textLimit = 0, TModule* module = 0 );
    NSLabel(TWindow* parent, int resourceId, uint textLimit = 0, TModule* module = 0);
    ~NSLabel();

  public:
    void    Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre);
    void    EvPaint();

  DECLARE_RESPONSE_TABLE(NSLabel) ;
};

#endif