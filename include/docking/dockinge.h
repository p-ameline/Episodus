// ****************************************************************************// Copyright (C) 1998 by Dieter Windau
// All rights reserved
//
// DockingEx.h:  header file
// Version:      1.0
// Date:         12.07.1998
// Author:       Dieter Windau
//
// Freeware OWL classes that extents the dockable system
//
// You are free to use/modify this code but leave this header intact.
// May not be sold for profit.
//
// Tested with Borland C++ 5.02, OWL 5.02 under Windows NT 4.0 SP3 but I think
// the class would work with Windows 95 too.
// This file is provided "as is" with no expressed or implied warranty.
// Use at your own risk.
//
// This code is based on BordDock classes by Alan Chambers, Birmingham, England.
// (aka Arnold the Aardvark - nom de news)
//   EMail: myrddin@foxholly.demon.co.uk
//   Web:   http://www.foxholly.demon.co.uk/OWL.htm
//
// The persistent storage of toolbar positions for use in the next session is
// based on TMyToolbar class of Christopher Kohlhoff. Very special thanks.
//   Web: http://www.tenermerx.com/owlhow/
//
// Please send me bug reports, bug fixes, enhancements, requests, etc., and
// I'll try to keep this in next versions:
//   EMail: dieter.windau@usa.net
//   Web:   http://www.members.aol.com/SoftEngage
// ****************************************************************************

#ifndef __DOCKINGEX_H#define __DOCKINGEX_H

#ifndef   OWL_DOCKING_H#include <owl/docking.h>
#endif
#ifndef   CLASSLIB_ARRAYS_H
#include <classlib/arrays.h>
#endif

#include "nsbb/nsexport.h"

// ****************** Forward declaration *************************************
struct TGridLine;
class THarborEx;

// *********************** helper functions ***********************************

// MFC style function, that draw the frame of a 3d rect with given colors
void Draw3dRect(HDC hdc, const NS_CLASSLIB::TRect& rect,
	COLORREF clrTopLeft, COLORREF clrBottomRight);
void Draw3dRect(HDC hdc, int x, int y, int cx, int cy,
	COLORREF clrTopLeft, COLORREF clrBottomRight);

// MFC style function, that draws a filled rect. Is this faster???
void FillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr);

// ****************** TDockableControlBarEx ***********************************

class _NSBBCLASSE TDockableControlBarEx : public TDockableControlBar
{
  public:

    TDockableControlBarEx(TWindow* parent = 0,                          TTileDirection direction = Horizontal,
                          OWL::TFont* font = new TGadgetWindowFont,
                          OWL::TModule* module = 0);

    // default constructor
    TDockableControlBarEx(uint id,                          const char* title,
                          bool _Default = true,
                          TWindow* parent = 0,
                          TTileDirection direction = Horizontal,
                          OWL::TFont* font = new TGadgetWindowFont,
                          OWL::TModule* module = 0);

    // constructor that set additional the Attr.id for context help and OLE 2,    // title for the caption, and the flag for a default controlbar.

    virtual ~TDockableControlBarEx();

    void Hide();
    void Show();
    TResult WindowProc(uint msg, TParam1 p1, TParam2 p2);

    void SavePosition();

    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& rect);
    // Overridden to paint the etched toolbar border and the gripper

    void ToggleSlip();
    bool IsDefault() { return Default; }    void SetDefault(bool b) { Default = b; }

  protected:
    void EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point);    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point);

    void CmOk();    void CmCancel();

  protected:

    NS_CLASSLIB::TRect  Rect;           // Last position. Save when hide    TAbsLocation        Location;       // Last location. Save when hide
    NS_CLASSLIB::TRect  ToggleRect;     // position when toggle the slip view
    TAbsLocation        ToggleLocation; // location when toggle the slip view
    bool                Dockable;       // false, if the controlbar is not dockable
    bool                Default;        // true, if the controlbar is a default bar.
                                 // You can't delete or rename a default bar.

   	DECLARE_RESPONSE_TABLE(TDockableControlBarEx);
};

// ************************ TDCBData ******************************************
// class that encapsulate relevant TDockableToolBarEx dataclass TDCBData
{
public:  TDockableControlBarEx* DCB;
  TDockingSlip*          Slip; // if DCB is not visible this is 0
  NS_CLASSLIB::TRect     Rect; // in screen coordinates
  char*                  Title;

public:

  TDCBData()    { DCB=0; Slip=0; Rect=NS_CLASSLIB::TRect(0,0,0,0); Title = 0; }
  TDCBData(const TDCBData& data)
    { *this = data;  }
  ~TDCBData()
    { delete[] Title; }

  TDCBData& operator =(const TDCBData& d);

	bool operator ==(const TDCBData& d) const
    { return memcmp(this, &d, sizeof(d)) == 0 ? true : false; }
	bool operator !=(const TDCBData& d) const
    { return !(*this == d); }

  bool operator < (const TDCBData& d) const
    { return strcmp(Title, d.Title) < 0 ? true : false; }
  bool operator > (const TDCBData& d) const
    { return strcmp(Title, d.Title) > 0 ? true : false; }
};

// ****************** THarborEx ***********************************************
class _NSBBCLASSE THarborEx : public THarbor{
  public:

    THarborEx(TDecoratedFrame& df,
    					bool leftSlip   = true,
              bool topSlip    = true,
              bool rightSlip  = true,
              bool bottomSlip = false) ;

		// If you create one of the four TEdgeSlips in THarbor before you insert
		// decorations in TDecoratedFrame all the TDockable's in THarbor insert near
		// the main window borders. Some examples:
		// - if you don't create top and left Slip here e.g. a ruler is not near the client window
		// - if you create bottom slip here e.g. a statusbar is not always at bottom
    virtual ~THarborEx() ;

    virtual TEdgeSlip* ConstructEdgeSlip(TDecoratedFrame& df, TAbsLocation location);
    // Factory function overridden to use TEdgeSlipEx in lace of TEdgeSlip

	  virtual TFloatingSlip* ConstructFloatingSlip(TDecoratedFrame& df,
  		int x, int y, TWindow* dockableWindow);
	  // Factory function overridden to use TFloatingSlipEx in lace of TFloatingSlip

    bool IsShowTooltips() { return ShowTooltips; }
    void SetShowTooltips(bool showTooltips) { ShowTooltips = showTooltips; }
    bool IsWithAccel() { return WithAccel; }
    void SetWithAccel(bool b) { WithAccel = b; }
    TGadgetWindow::THintMode GetHintMode() { return HintMode; }
    void SetHintMode(TGadgetWindow::THintMode HintMode);
    bool IsDrawGripper() { return DrawGripper; }
    void SetDrawGripper(bool drawGripper);
    // Get/set options for all dockable toolbars in harbor

    virtual int PostGetHintText(uint id, char* buf, int size, THintText hintType);
    // Retrieves the hint text associated with a particular Id
    // To be called from TDecoratedFrame GetHintText(...)
    // If ShowTooltips == false, the tooltips will be deleted
    // If WithAccel == false, the accelerator keys defined by resource in style:
    // " (key)" will be hidden.

  protected:

    void  GetDCBData();    void  InsertDCBData(TDockableControlBarEx* cb, TDockingSlip* slip);

  protected:

    TDecoratedFrame& DecoratedFrame;
    bool             ShowTooltips;  // show tooltips for all dockable control bars    bool             WithAccel;     // show accelerator key in tooltips
    bool             DrawGripper;   // draw gripper for all edge slips
    TGadgetWindow::THintMode HintMode;// hint mode for all dockable control bars

    TISArrayAsVector<TDCBData> DCBData;
};

// ****************** TFloatingSlipEx *****************************************
class _NSBBCLASSE TFloatingSlipEx: public TFloatingSlip{
public:
  TFloatingSlipEx(TWindow* parent, int x, int y, TWindow* clientWnd = 0,
    bool shrinkToClient = true, int captionHeight = DefaultCaptionHeight,
    bool popupPalette = true, OWL::TModule* module = 0);
  virtual ~TFloatingSlipEx();

	void DockableInsert(TDockable& dockable, const NS_CLASSLIB::TPoint* topLeft,
    TRelPosition position, TDockable* relDockable);

protected:
  void EvNCLButtonDown(uint hitTest, NS_CLASSLIB::TPoint& point);
  void EvLButtonDown(uint hitTest, NS_CLASSLIB::TPoint& point);
  void EvNCLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point);

  DECLARE_RESPONSE_TABLE(TFloatingSlipEx);
};

// ****************** TEdgeSlipEx *********************************************
class _NSBBCLASSE TEdgeSlipEx : public TWindow, public TDockingSlip
{
  public:
    TEdgeSlipEx(TDecoratedFrame& parent, TAbsLocation location, OWL::TModule* module = 0);
    virtual ~TEdgeSlipEx();

    void     SetGripperMargins(TDockableControlBarEx* cb);
		// Set the margins for the gripper

    // Overridden TWindow virtuals
    TResult  EvCommand(uint id, THandle hWndCtl, uint notifyCode);
    void     EvCommandEnable(TCommandEnabler& commandEnabler);
    void     SetupWindow();

    // These are overriden for any window that wants to be notified on dockable
    // windows being moved or removed.
    void DockableInsert(TDockable& dockable, const NS_CLASSLIB::TPoint* topLeft = 0,
      TRelPosition position = rpNone, TDockable* relDockable = 0);
    void DockableMove(TDockable& dockable, const NS_CLASSLIB::TPoint* topLeft = 0,
      TRelPosition position = rpNone, TDockable* relDockable = 0);
    void DockableRemoved(const NS_CLASSLIB::TRect& orgRect);
    TAbsLocation GetLocation() const;

  protected:

    // Response functions    void     EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point);
    void     EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point);
    uint     EvNCCalcSize(bool calcValidRects, NCCALCSIZE_PARAMS far& calcSize);
    void     EvNCPaint(HRGN);
    bool     EvEraseBkgnd(HDC);
    void     EvParentNotify(uint event, uint childHandleOrX, uint childIDOrY);
    void     EvWindowPosChanging(WINDOWPOS far& windowPos);

    // Internal tiling functions
    void ArrangeAllDockables(TWindow* NewDockable);
    void LayoutSlip(int Height);

  protected:
    // Internal tiling data
    TAbsLocation Location;
    TGridType    GridType;
    bool InsertingDockable; // To prevent parent notify layout during other layout

  DECLARE_RESPONSE_TABLE(TEdgeSlipEx);
};

// ****************** TGridItem ***********************************************
// Represents a single dockable in the slipstruct TGridItem
{
  // Constructor
  TGridItem(TWindow* Window, TAbsLocation Location);

  // Tiling functions - used to eliminate overlaps along a gridline
  void ArrangePrecedingItems();
  void ArrangeFollowingItems(int SlipWidth);
  void ArrangeLeft();
  void ArrangeRight(bool Limit, int SlipWidth);

  // Data members
  TWindow*   Dockable;      // Dockable window which this item represents
  int        LeftEdge;      // Left or Top depending on orientation
  int        TopEdge;       // Top or Left depending on orientation
  int        Width;         // Width or Height depending on orientation
  int        Height;        // Height or Width depending on orientation
  TGridLine* GridLine;      // Used to retieve the top edge for this grid item
  TGridItem* NextItem;      // Pointer to next item on the same gridline
  TGridItem* PrevItem;      // Pointer to previous item on the same gridline
};

// ****************** TGridLine ***********************************************

// Represents a row of one or more dockables in the slip (linked list)
struct TGridLine
{
  // Constructor and destructor
  TGridLine(uint32 Top);
  ~TGridLine();           // Delete all the grid items in the linked list

  // Tiling functions
  void ComputeHeight();   // Find height of the tallest grid item
  TGridItem* LayoutGridItems(TGridItem* Anchor, int SlipWidth);  // Horizontal layout - sets left edges mainly

  // List management functions
  void InsertGridItem(TGridItem* NewItem);  // Insert into a sorted linked list
  void RemoveGridItem(TGridItem* NewItem);  // Remove from the linked list

  // Data members
  int        TopEdge;     // All grid items share this top edge
  int        Height;      // Height of the tallest grid item
  TGridItem* FirstItem;   // Start of the linked list of grid items
  TGridLine* NextLine;    // Pointer to the next gridline
  TGridLine* PrevLine;    // Pointer to the previous gridline
};

// ****************** TGridLayout *********************************************

// Represents all rows of dockables in the slip (linked list)
struct TGridLayout
{
  // Constructor and destructor
  TGridLayout(TAbsLocation Loc);
  ~TGridLayout();           // Delete all the grid lines in the linked list

  // Tiling functions
  void CreateLists(TWindow* parent, TWindow* dockable);  // Build linked lists
  int  LayoutGridLines();  // Vertical layout - sets top edge mainly
  void LayoutDockables();  // Move actual windows about

  // List management functions
  void InsertGridLine(TGridLine* NewLine);       // Insert into a sorted linked list
  TGridLine* InsertGridItem(TGridItem* NewItem); // Insert into a sorted linked list
  TGridLine* InsertDockableItem();               // Insert into a sorted linked list

  // Data members
  TGridLine* FirstLine;    // Start of the linked list of grid lines
  TWindow*   Parent;       // The TBorderSlip which owns this object
  TWindow*   Dockable;     // The toolbar just dropped on this edge slip
  TGridItem* DockableItem; // The grid item representing Dockable
  TAbsLocation Location;   // The position of the TBorderSlip
};

#endif

