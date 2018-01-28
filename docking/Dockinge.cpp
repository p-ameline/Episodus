// ****************************************************************************
// Copyright (C) 1998 by Dieter Windau
// All rights reserved
//
// DockingEx.cpp: implementation file
// Version:       1.0
// Date:          12.07.1998
// Author:        Dieter Windau
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

#include <owl\owlpch.h>
#pragma hdrstop

#include <owl/uihelper.h>  // for TUIBorder edge painting

#include "docking\dockinge.h"

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

void Draw3dRect(HDC hdc, const TRect& rect,
	COLORREF clrTopLeft, COLORREF clrBottomRight)
// MFC style function, that draw the frame of a 3d rect with given colors
{
	if (hdc == 0)
  	return;

	Draw3dRect(hdc, rect.left, rect.top, rect.Width(),
		rect.Height(), clrTopLeft, clrBottomRight);
}

void Draw3dRect(HDC hdc, int x, int y, int cx, int cy,
	COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	if (hdc == 0)
  	return;

	FillSolidRect(hdc, x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(hdc, x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(hdc, x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(hdc, x, y + cy, cx, -1, clrBottomRight);
}

void FillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr)
// MFC style function, that draws a filled rect. Is this faster???
{
	if (hdc == 0)
  	return;

	::SetBkColor(hdc, clr);
	TRect rect(x, y, x + cx, y + cy);
	::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, 0, 0, 0);
}

// ****************** TDockableControlBarEx ***********************************

DEFINE_RESPONSE_TABLE1(TDockableControlBarEx, TDockableControlBar)
	EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(IDOK, CmOk),
  EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

TDockableControlBarEx::TDockableControlBarEx(TWindow* parent,
	TTileDirection direction, TFont* font, TModule* module):
  TDockableControlBar(parent, direction, font, module)
// default constructor
{
	// SetMargins(TMargins(TMargins::Pixels,0,0,0,0));
	// Margins are setting in TEdgeSlipEx::DockableInsert()
	// and TFloatingSlipEx::DockableInsert()
  Attr.Id = 0;
  Default = true;

  // no default position
  ToggleRect     = TRect(-1,-1,-1,-1);
  ToggleLocation = alTop;
}

TDockableControlBarEx::TDockableControlBarEx(uint id, const char* title,
	bool _Default, TWindow* parent, TTileDirection direction, TFont* font,
  TModule* module):
  TDockableControlBar(parent, direction, font, module)
// constructor that set additional the Attr.id for context help and OLE 2,
// title for the caption, and the flag for a default controlbar
{
  Attr.Id = id;
  SetCaption(title ? title : "");
  Default = _Default;

  // no default position
  ToggleRect     = TRect(-1,-1,-1,-1);
  ToggleLocation = alTop;
}

TDockableControlBarEx::~TDockableControlBarEx()
{
  Destroy(IDCANCEL);
}

void TDockableControlBarEx::Hide()
{
  THarborEx* harbor = TYPESAFE_DOWNCAST(GetHarbor(), THarborEx);
  if (harbor)
  {
    harbor->Remove(*this);
  }
}

void TDockableControlBarEx::Show()
{
  THarborEx* harbor = TYPESAFE_DOWNCAST(GetHarbor(), THarborEx);
  if (harbor)
  {
	 	harbor->Insert(*this, Location, &Rect.TopLeft());
//  	harbor->Move(*this, Location, &Rect.TopLeft());
  }
}

TResult TDockableControlBarEx::WindowProc(uint msg, TParam1 p1, TParam2 p2)
{
  if (msg == WM_SHOWWINDOW && !p1) // we are being hidden
	  SavePosition();
  return TDockableControlBar::WindowProc(msg, p1, p2);
}

void TDockableControlBarEx::SavePosition()
{
  // get the actual absolute location of the toolbar
  //
  TDockingSlip* dockingSlip = TYPESAFE_DOWNCAST(Parent, TDockingSlip);
  TAbsLocation actLocation = (dockingSlip ? dockingSlip->GetLocation() : alNone);

  // Location changed
  //
  if (actLocation != Location)
  {
    // if the actual location is equal with the saved toggled slip location
    //
    if ((actLocation == alNone && ToggleLocation == alNone) ||
        (actLocation != alNone && ToggleLocation != alNone))
    {
      ToggleRect = Rect;
      ToggleLocation = Location;
    }
    Location = actLocation;
  }

  // get the rectangle of the toolbar
  //
  TFloatingSlipEx* floatingSlip = TYPESAFE_DOWNCAST(Parent, TFloatingSlipEx);
  if (floatingSlip)
    floatingSlip->GetWindowRect(Rect);
  else
    GetRect(Rect);
}

void TDockableControlBarEx::Paint(TDC& dc, bool erase, TRect& rect)
{
  // When docked, this paints an etched border just
  // inside the client area of the control bar
  TDockableControlBar::Paint(dc, erase, rect);
  TDockingSlip* Slip = TYPESAFE_DOWNCAST(Parent, TDockingSlip);
  if (Slip && Slip->GetLocation() != alNone)
  {
    TWindowDC MyDC(*this);
    TRect ThisClientRect = GetClientRect();
    TUIBorder(ThisClientRect, TUIBorder::EdgeEtched, TUIBorder::Left).Paint(MyDC);
    TUIBorder(ThisClientRect, TUIBorder::EdgeEtched, TUIBorder::Right).Paint(MyDC);
    TUIBorder(ThisClientRect, TUIBorder::EdgeEtched, TUIBorder::Bottom).Paint(MyDC);
    TUIBorder(ThisClientRect, TUIBorder::EdgeEtched, TUIBorder::Top).Paint(MyDC);

    THarborEx* harbor = TYPESAFE_DOWNCAST(GetHarbor(), THarborEx);
    if (harbor->IsDrawGripper())
    {
	    TRect gripperRect = ThisClientRect;
      if (Slip->GetLocation() == alTop || Slip->GetLocation() == alBottom)
      {
        gripperRect.Inflate(-GripperDistSize-1, -GripperDistSize);
        gripperRect.right = gripperRect.left + GripperDistSize;
        Draw3dRect(MyDC, gripperRect, TColor::Sys3dHilight, TColor::Sys3dShadow);
        gripperRect.Offset(GripperDistSize, 0);
        Draw3dRect(MyDC, gripperRect, TColor::Sys3dHilight, TColor::Sys3dShadow);
      }
      else
      {
        gripperRect.Inflate(-GripperDistSize, -GripperDistSize-1);
        gripperRect.bottom = gripperRect.top + GripperDistSize;
        Draw3dRect(MyDC, gripperRect, TColor::Sys3dHilight, TColor::Sys3dShadow);
        gripperRect.Offset(0, GripperDistSize);
        Draw3dRect(MyDC, gripperRect, TColor::Sys3dHilight, TColor::Sys3dShadow);
      }
    }
  }
}

void TDockableControlBarEx::EvLButtonDown(uint modKeys, TPoint& point)
{
#ifdef CTXHELP
  TCtxHelpFileManager* ctxHelpM = TYPESAFE_DOWNCAST(GetApplication(),
  	TCtxHelpFileManager);
  if (ctxHelpM && ctxHelpM->IsContextHelp())
  {
    TGadget* gadget = Capture ? Capture : GadgetFromPoint(point);
    if (gadget != 0 && gadget->GetBorderStyle() != TGadget::None)
    {
      // click in a gadget: call help with gadget id > 0
      //
      int HelpId = gadget->GetId();
      if (HelpId > 0)
	      ctxHelpM->WinHelp(HelpId);
    }
    else
    {
      // click in a seperator or the gripper: call help with Attr.Id != 0
      //
      if (Attr.Id != 0)
      	ctxHelpM->WinHelp(Attr.Id);
    }
  }
	else
#endif
		TDockableControlBar::EvLButtonDown(modKeys, point);
}

void TDockableControlBarEx::ToggleSlip()
{
  // Check that we're parented to the right window. In OLE server situation,
  // the toolbar could have been reparented to another HWND [i.e. container's
  // window]
  //
  TWindow* w = GetParentO();
  if (w && ::GetParent(*this) != w->GetHandle())
    return;

  SavePosition();
  bool ToggleSlips = false;
  TAbsLocation setLoc;
  if (TYPESAFE_DOWNCAST(w, TEdgeSlipEx) && Location != alNone)
  {
    // Change from EdgeSlip to FloatingSlip
    //
    setLoc         = alNone;
    ToggleLocation = Location;
    ToggleSlips    = true;
  }
  else
  {
    if (TYPESAFE_DOWNCAST(w, TFloatingSlipEx) && Location == alNone && ToggleLocation != alNone)
    {
      // Change from FloatingSlip to EdgeSlip
      //
      setLoc         = ToggleLocation;
      ToggleLocation = alNone;
      ToggleSlips    = true;
    }
  }

  if (ToggleSlips)
  {
    // get and set the rect before toggle the slips
    //
    TRect setRect;
    bool noDefaultPos = false;
    if (ToggleRect == TRect(-1,-1,-1,-1))
      noDefaultPos = true;
    else
      setRect = ToggleRect;
    ToggleRect = Rect;

    THarbor* harbor = GetHarbor();
    if (harbor)
    {
      harbor->Remove(*this);
      harbor->Insert(*this, setLoc, noDefaultPos ? 0 : &setRect.TopLeft());
    }
  }
}

void TDockableControlBarEx::EvLButtonDblClk(uint modKeys, TPoint& point)
{
  // Check that the DBLClk position is in a visible enabled Gadget
  // an the Dockable flag is set true
  //
  if (ShouldBeginDrag(point))
  {
    ToggleSlip();
    return;
  }
  TDockableControlBar::EvLButtonDblClk(modKeys, point);
}

void TDockableControlBarEx::CmOk()
{
  ::SendMessage((HWND)GetFocus(), WM_KEYDOWN, VK_RETURN, MAKELPARAM(1, 0));
}

void TDockableControlBarEx::CmCancel()
{
  ::SendMessage((HWND)GetFocus(), WM_KEYDOWN, VK_ESCAPE, MAKELPARAM(1, 0));
}

// ****************** TDCBData ************************************************

TDCBData& TDCBData::operator =(const TDCBData& d)
{
	DCB   = d.DCB;
  Slip  = d.Slip;
  Rect  = d.Rect;
  Title = strnewdup(d.Title);
  return *this;
}

// ****************** THarborEx ***********************************************

THarborEx::THarborEx(TDecoratedFrame& df,
  bool leftSlip, bool topSlip, bool rightSlip, bool bottomSlip):
  THarbor(df),
  DecoratedFrame(df),
  DCBData(5,0,5)  
// If you create one of the four TEdgeSlips in THarbor before you insert
// decorations in TDecoratedFrame all the TDockable's in THarbor insert near
// the main window borders. Some examples:
// - if you don't create top and left Slip here e.g. a ruler is not near the client window
// - if you create bottom slip here e.g. a statusbar is not always at bottom
{
  ShowTooltips = true;  // show tooltips for all dockable control bars
  WithAccel = true;     // show accelerator key in tooltips
  DrawGripper = true;   // draw gripper for all edge slips
  HintMode = TGadgetWindow::PressHints;
  
  TEdgeSlip* EdgeSlip;

  // Create the top slip
  //
  if (topSlip)
  {
	  EdgeSlip = ConstructEdgeSlip(DecoratedFrame, alTop);
  	SetEdgeSlip(alTop, EdgeSlip);
	  DecoratedFrame.Insert(*EdgeSlip, (TDecoratedFrame::TLocation)alTop);
  }

  // Create the left slip
  //
  if (leftSlip)
  {
	  EdgeSlip = ConstructEdgeSlip(DecoratedFrame, alLeft);
	  SetEdgeSlip(alLeft, EdgeSlip);
  	DecoratedFrame.Insert(*EdgeSlip, (TDecoratedFrame::TLocation)alLeft);
  }

  // Create the right slip
  //
  if (rightSlip)
  {
	  EdgeSlip = ConstructEdgeSlip(DecoratedFrame, alRight);
  	SetEdgeSlip(alRight, EdgeSlip);
	  DecoratedFrame.Insert(*EdgeSlip, (TDecoratedFrame::TLocation)alRight);
  }

  // Create the bottom slip
  //
  if (bottomSlip)
  {
	  EdgeSlip = ConstructEdgeSlip(DecoratedFrame, alBottom);
  	SetEdgeSlip(alBottom, EdgeSlip);
	  DecoratedFrame.Insert(*EdgeSlip, (TDecoratedFrame::TLocation)alBottom);
  }
}

THarborEx::~THarborEx()
{
  Destroy(IDCANCEL);
}

TEdgeSlip* THarborEx::ConstructEdgeSlip(TDecoratedFrame& df, TAbsLocation location)
{
  // This is NOT a typesafe cast, but should be OK since public members are
  // exactly the same as for TEdgeSlip.
  //
  return (TEdgeSlip*)(new TEdgeSlipEx(df, location));
}

TFloatingSlip* THarborEx::ConstructFloatingSlip(TDecoratedFrame& df,
  	int x, int y, TWindow* dockableWindow)
{
  return TYPESAFE_DOWNCAST(new TFloatingSlipEx(
  	&df, x, y, dockableWindow), TFloatingSlip);
}

void THarborEx::SetHintMode(TGadgetWindow::THintMode hintMode)
{
  if (HintMode != hintMode)
  {
  	HintMode = hintMode;
    for (uint i=0; i<DCBData.GetItemsInContainer(); i++)
    {
      // change the hint mode for all visible and hidden dockable controlbars
      //
      TDockableControlBarEx* dcb = DCBData[i]->DCB;
      if (dcb && dcb->GetHandle())
	      dcb->SetHintMode(HintMode);
    }
  }
}

void THarborEx::SetDrawGripper(bool drawGripper)
{
  if (DrawGripper != drawGripper)
  {
    DrawGripper = drawGripper;
//D:   GetDCBData();
    for (uint i=0; i<DCBData.GetItemsInContainer(); i++)
    {
      // if the docking slip is a valid TEdgeSlip and has a valid non static
      // dockable controlbar as parent
      //
      TDockableControlBarEx* cb = DCBData[i]->DCB;
      TEdgeSlipEx* edgeSlip = TYPESAFE_DOWNCAST(DCBData[i]->Slip, TEdgeSlipEx);
      if (edgeSlip && edgeSlip->GetHandle() && cb && cb->GetHandle())
      {
        // set the margins for the gripper (invisible)
        //
        cb->ShowWindow(SW_HIDE);
        edgeSlip->SetGripperMargins(cb);
        cb->ShowWindow(SW_SHOWNA);
      }
    }
  }
}

int THarborEx::PostGetHintText(uint /*id*/, char* buf, int /*size*/, THintText hintType)
// Retrieves the hint text associated with a particular Id
// To be called from TDecoratedFrame GetHintText(...)
// If ShowTooltips == false, the tooltips will be deleted
// If WithAccel == false, the accelerator keys defined by resource in style:
// " (key)" will be hidden.
{
  if (buf)
  {
	  uint len = strlen(buf);
    if (ShowTooltips == false && len > 0 && hintType == htTooltip)
    {
      *buf = 0;
      return 0;
    }
    if (WithAccel == false && len > 0 && hintType == htTooltip)
    {
      char* c = strchr(buf, '(');
      if (c != 0)
      {
        *(c-1) = 0; // -1 for space before the '('
        return strlen(buf);
      }
    }
    return len;
  }
  return 0;
}

void THarborEx::GetDCBData()
{
  DCBData.Flush();

  // search in all child windows of TDecoratedFrame valid TDockingSlip windows
  //
  TRect rect = TRect(0,0,0,0);
  TWindow* first = DecoratedFrame.GetFirstChild();
  if (first)
  {
    TWindow* child = first;
    do
    {
      TDockableControlBarEx* cb;
      TDockingSlip* dockSlip = TYPESAFE_DOWNCAST(child, TDockingSlip);
      TEdgeSlipEx* edgeSlip = TYPESAFE_DOWNCAST(child, TEdgeSlipEx);
      TFloatingSlipEx* floatSlip = TYPESAFE_DOWNCAST(child, TFloatingSlipEx);

      if (dockSlip && edgeSlip && edgeSlip->GetHandle())
      {
        TWindow* edgeFirst = edgeSlip->GetFirstChild();
        if (edgeFirst)
        {
          TWindow* edgeChild = edgeFirst;
          do
          {
  	        cb = TYPESAFE_DOWNCAST(edgeChild, TDockableControlBarEx);
		        InsertDCBData(cb, dockSlip);
			      edgeChild = edgeChild->Next();
          }
					while (edgeChild != edgeFirst);
		    }
      }
      else if (dockSlip && floatSlip && floatSlip->GetHandle())
      {
			  cb = TYPESAFE_DOWNCAST(floatSlip->GetFirstChild(), TDockableControlBarEx);
        InsertDCBData(cb, dockSlip);
      }
      child = child->Next();
    }
    while (child != first);
  }

  first = GetFirstChild();
  if (first)
  {
    TWindow* child = first;
    do
    {
			TDockableControlBarEx* cb = TYPESAFE_DOWNCAST(child, TDockableControlBarEx);
      InsertDCBData(cb, 0);
      child = child->Next();
    }
    while (child != first);
  }
}

void THarborEx::InsertDCBData(TDockableControlBarEx* cb, TDockingSlip* slip)
{
	TRect rect;
  if (cb && cb->GetHandle())
  {
   	cb->GetRect(rect);
    int len = cb->GetWindowTextLength();
   	char* title = new char[len+1]; // +1 for 0 terminator
    if (len > 0)
      cb->GetWindowText(title, len+1);
    else
      *title=0;
    TDCBData* data = new TDCBData();
    data->DCB  = cb;
    data->Slip = slip;
    data->Rect = rect;
    data->Title = title;
    DCBData.Add(data);
  }
}

// ****************** TGridItem ***********************************************

TGridItem::TGridItem(TWindow* Window, TAbsLocation Location)
{
  // Initialise data
  Dockable = Window;
  TRect WindowRect = Dockable->GetWindowRect();
  // All the workings are to be in client coordinates of the edge slip
  ::MapWindowPoints(0, *(Window->Parent), (TPoint*)&WindowRect, 2);

  if (Location == alTop || Location == alBottom)
  {
    LeftEdge = WindowRect.left;
    TopEdge  = WindowRect.top;      // Only needed to find grid line
    Width    = WindowRect.Width();
    Height   = WindowRect.Height();
  }
  else
  {
    // Transpose the coordinates for internal calculations
    LeftEdge = WindowRect.top;
    TopEdge  = WindowRect.left;     // Only needed to find grid line
    Width    = WindowRect.Height();
    Height   = WindowRect.Width();
  }

  GridLine = 0;
  NextItem = 0;
  PrevItem = 0;
}

void TGridItem::ArrangeLeft()
// Recursive function tries to push dockables which are overlapped to the left.
// It is not possible to push them beyond the left edge of the slip and this
// leads to packing against the left edge.
{
  // Can only be called where there is a following item
  // Push this item to the immediate left of the following item
  LeftEdge = NextItem->LeftEdge - Width + 2;
  if (PrevItem)
  {
    if(LeftEdge < PrevItem->LeftEdge + PrevItem->Width - 2)
    {
      // Where this grid item overlaps the previous one, TRY to push the
      // previous one leftwards
      PrevItem->ArrangeLeft(); // This function recurses
      if(LeftEdge < PrevItem->LeftEdge + PrevItem->Width - 2)
      {
        // Where the previous item was prevented from moving far enough,
        // push this item to the right of it
        LeftEdge = PrevItem->LeftEdge + PrevItem->Width - 2;
      }
    }
  }
  else if (LeftEdge < -2)
  {
    // Where this is the first grid item, ensure it does not stick off the left
    LeftEdge = -2;
  }
}

void TGridItem::ArrangeRight(bool Limit, int SlipWidth)
// Recursive function tries to push dockables which are overlapped to the right.
// Where Limit is true, it is not possible to push them beyond the right edge of
// the slip and this leads to packing against the right edge. If Limit is false,
// items are allowed to stick out past the right edge, or even be pushed wholly
// beyond it.
{
  // Can only be called where there is a preceding item
  // Push this item to the immediate left of the preceding item
  LeftEdge = PrevItem->LeftEdge + PrevItem->Width - 2;
  if (NextItem)
  {
    if(LeftEdge + Width - 2 > NextItem->LeftEdge)
    {
      // Where this grid item overlaps the next one, TRY to push the
      // next one rightwards
      NextItem->ArrangeRight(Limit, SlipWidth); // This function recurses
      if(LeftEdge + Width - 2 > NextItem->LeftEdge)
      {
        // Where the next item was prevented from moving far enough,
        // push this item to the left of it
        LeftEdge = NextItem->LeftEdge - Width + 2;
      }
    }
  }
  else if (LeftEdge + Width - 2 > SlipWidth && Limit)
  {
    // Where this is the last grid item, ensure it does not stick off the right
    LeftEdge = SlipWidth - Width + 2;
  }
}

void TGridItem::ArrangePrecedingItems()
// Function called to move overlapped dockabes to the left of an anchor (this).
// The anchor is moved to the right if necessary.
{
  if (PrevItem)
  {
    if(LeftEdge < PrevItem->LeftEdge + PrevItem->Width - 2)
    {
      // Where this grid item overlaps the previous one, TRY to push the
      // previous one leftwards
      PrevItem->ArrangeLeft(); // This function recurses
      if(LeftEdge < PrevItem->LeftEdge + PrevItem->Width - 2)
      {
        // Where the previous item was prevented from moving far enough,
        // push this item to the right of it
        LeftEdge = PrevItem->LeftEdge + PrevItem->Width - 2;
      }
    }
  }
  else if (LeftEdge < -2)
  {
    // Where this is the first grid item, ensure it does not stick off the left
    LeftEdge = -2;
  }
}

void TGridItem::ArrangeFollowingItems(int SlipWidth)
// Function called to move overlapped dockabes to the right of an anchor (this).
// The function first tries to ensure packing against the right edge, but
// tries again without packing if the anchor cannot move far enough leftwards.
// The anchor is moved to the left or right if necessary.
{
  if (NextItem)
  {
    if(LeftEdge + Width - 2 > NextItem->LeftEdge)
    {
      // Where this grid item overlaps the next one, TRY to push the
      // next one rightwards
      NextItem->ArrangeRight(true, SlipWidth); // This function recurses
      if(LeftEdge + Width - 2 > NextItem->LeftEdge)
      {
        // Where the next item was prevented from moving far enough,
        // TRY to push this item to the left of it
        ArrangeLeft();
        if (LeftEdge + Width - 2 > NextItem->LeftEdge)
        {
          // Where this grid item still overlaps the next one, push the next one
          // rightwards
          NextItem->ArrangeRight(false, SlipWidth); // This function recurses
          // Is this part superfluous/counterproductive?
        }
      }
    }
  }
  else if (LeftEdge + Width - 2 > SlipWidth)
  {
    // Where this is the last grid item, ensure it does not stick off the right
    LeftEdge = SlipWidth - Width + 2;
    // This may result in the item sticking off the right edge
    ArrangePrecedingItems();
  }
}

// ****************** TGridLine ***********************************************

TGridLine::TGridLine(uint32 Top)
{
  TopEdge = Top;
  Height = 0;
  FirstItem = 0;
  NextLine = 0;
  PrevLine = 0;
}

TGridLine::~TGridLine()
{
  // CodeGuard loves me!
  TGridItem* Temp;
  while (FirstItem)
  {
    Temp = FirstItem->NextItem;
    delete FirstItem;
    FirstItem = Temp;
  }
}

TGridItem* TGridLine::LayoutGridItems(TGridItem* Anchor, int SlipWidth)
{
  // We should check that Anchor is in this gridline
  if (!Anchor)
    Anchor = FirstItem;

  // Confirm that the item is in the list
  TGridItem* IterItem = FirstItem;
  while (IterItem && IterItem != Anchor)
  {
    IterItem = IterItem->NextItem;
  }

  // Only do something if the item is in the list
  if (IterItem)
  {
    // Layout the dockables on this gridline so there are no overlaps
    Anchor->ArrangePrecedingItems();
    Anchor->ArrangeFollowingItems(SlipWidth);

    // Return a pointer to the first grid item pushed wholly off the right edge
    // of the slip
    TGridItem* FirstExcess;
    TGridItem* Temp = FirstItem;
    while (Temp && Temp->LeftEdge < SlipWidth)
    {
      Temp = Temp->NextItem;
    }
    FirstExcess = Temp;
    return FirstExcess;
  }
  return 0;
}

void TGridLine::ComputeHeight()
{
  int TempHeight = 0;
  // Height of the tallest grid item
  TGridItem* TempItem = FirstItem;
  while (TempItem)
  {
    TempHeight = max(TempItem->Height, TempHeight);
    TempItem = TempItem->NextItem;
  }
  Height = TempHeight;
}

void TGridLine::InsertGridItem(TGridItem* NewItem)
{
  if (!FirstItem)
  {
    FirstItem = NewItem;
  }
  else
  {
    TGridItem* InsertAfter = 0;
    TGridItem* InsertBefore = 0;
    TGridItem* IterNext = FirstItem;
    // Loop to sort position by LeftEdge
    while (IterNext)
    {
      if (NewItem->LeftEdge > IterNext->LeftEdge)
      {
        InsertAfter = IterNext;
      }
      else if (!InsertBefore)
      {
        InsertBefore = IterNext;
      }

      IterNext = IterNext->NextItem;
    }

    if (InsertAfter)
    {
      NewItem->PrevItem = InsertAfter;
      InsertAfter->NextItem = NewItem;
    }
    else
      FirstItem = NewItem;

    if (InsertBefore)
    {
      NewItem->NextItem = InsertBefore;
      InsertBefore->PrevItem = NewItem;
    }
    else
      NewItem->NextItem = 0; // Make sure the list is terminated properly
  }

  NewItem->GridLine = this;
  NewItem->TopEdge  = TopEdge;
  ComputeHeight();  // Can't remember why I added this...
}

void TGridLine::RemoveGridItem(TGridItem* NewItem)
{
  // Confirm that the item is in the list
  TGridItem* IterItem = FirstItem;
  while (IterItem && IterItem != NewItem)
  {
    IterItem = IterItem->NextItem;
  }

  // Only do something if the item is in the list
  if (IterItem)
  {
    if (NewItem->PrevItem)
    {
      NewItem->PrevItem->NextItem = NewItem->NextItem;
    }
    else
    {
      // The first item is being removed
      // Omitting this line caused the item in
      // question to be deleted twice - Bang!
      FirstItem = NewItem->NextItem;
    }
    if (NewItem->NextItem)
    {
      NewItem->NextItem->PrevItem = NewItem->PrevItem;
    }
    NewItem->PrevItem = 0;
    //NewItem->NextItem = 0; I'll be needing this later
    NewItem->GridLine = 0;
  }
}

// ****************** TGridLayout *********************************************

TGridLayout::TGridLayout(TAbsLocation Loc)
{
  FirstLine = 0;
  Parent = 0;
  Dockable = 0;
  DockableItem = 0;
  Location = Loc;
}

void TGridLayout::CreateLists(TWindow* parent, TWindow* dockable)
{
  Parent = parent;     // The edge slip
  Dockable = dockable; // The just dropped dockable child of the edge slip

  TWindow* FirstChild = Parent->GetFirstChild();
  if (FirstChild)
  {
    TWindow* ChildWindow = FirstChild;
    do
    {
      TGridItem* NewItem = new TGridItem(ChildWindow, Location);
      if (ChildWindow == Dockable)
      {
        // Don't insert the current item into the linked lists yet
        DockableItem = NewItem;
      }
      else
      {
        // Insert the item into the linked lists
        InsertGridItem(NewItem);
      }

      ChildWindow = ChildWindow->Next();
    }
    while (ChildWindow != FirstChild); // Does Next() cycle?
  }
}

TGridLine* TGridLayout::InsertDockableItem()
{
  // Need some logic to determine which existing grid line it should be in
  // or whether a new grid line should be inserted

  if (!DockableItem)
    return 0;

  TGridLine* DockableLine;

  if (FirstLine)
  {
    // Find first grid line with TopEdge greater than that of the new item
    TGridLine* IterPrev = 0;
    TGridLine* IterNext = FirstLine;
    while (IterNext && IterNext->TopEdge < DockableItem->TopEdge)
    {
      IterPrev = IterNext;
      IterNext = IterNext->NextLine;
    }
    // IterNext points to the first line with TopEdge >= DockableItem->TopEdge

    // Find the comparator
    int CompareTop;
    if (IterNext)
      CompareTop = IterNext->TopEdge;
    else
      CompareTop = IterPrev->TopEdge + IterPrev->Height - 2;

    // Decide which grid line to use based on degree of overlap of the new
    // dockable with the existing gridlines.
    // I'm sure this could be done lots better, but it does the trick quite
    // well.
    if (CompareTop < DockableItem->TopEdge + (DockableItem->Height - 1) / 3)
    {
      // Insert it on the next line - this will force a new line if no NextLine
      DockableItem->TopEdge = CompareTop;
      DockableLine = InsertGridItem(DockableItem);
    }
    else if (CompareTop < DockableItem->TopEdge + (2 * (DockableItem->Height - 1)) / 3)
    {
      // Force a new line to be inserted below
      DockableItem->TopEdge = CompareTop - 1;
      DockableLine = InsertGridItem(DockableItem);
    }
    else if (IterPrev)
    {
      CompareTop = IterPrev->TopEdge + IterPrev->Height - 2;
      if (CompareTop > DockableItem->TopEdge + (2 * (DockableItem->Height - 1)) / 3)
      {
        // Pull onto previous line
        DockableItem->TopEdge = IterPrev->TopEdge;
        DockableLine = InsertGridItem(DockableItem);
      }
      else
      {
        // Force a new line to be inserted below previous line
        DockableItem->TopEdge = IterPrev->TopEdge + 1;
        DockableLine = InsertGridItem(DockableItem);
      }
    }
    else
    {
      // Force a new line to be inserted above
      DockableItem->TopEdge = CompareTop - 1;
      DockableLine = InsertGridItem(DockableItem);
    }
  }
  else
  {
    DockableItem->TopEdge = -2;
    DockableLine = InsertGridItem(DockableItem);
  }
  return DockableLine;
}

TGridLayout::~TGridLayout()
{
  // CodeGuard loves me!
  TGridLine* Temp;
  while (FirstLine)
  {
    Temp = FirstLine->NextLine;
    delete FirstLine;
    FirstLine = Temp;
  }
}

int TGridLayout::LayoutGridLines()
{
  int iGlobalWidth = Parent->GetClientRect().Width() ;

  // Vertical layout - sets top edge mainly
  int CumulativeTop  = -2 ;
  int iLineHeight    = 0 ;
  int CumulativeLeft = 0 ;

  TGridLine* IterLine = FirstLine ;
  while (IterLine)
  {
    IterLine->ComputeHeight() ;

    TWindow* pDockableForLine = IterLine->FirstItem->Dockable ;
    if (pDockableForLine)
    {
      int iLeft = pDockableForLine->GetClientRect().Left() ;

      // Same line
      //
      if (iLeft >= CumulativeLeft)
      {
        iLineHeight      = max(iLineHeight, IterLine->Height) ;
        IterLine->Height = iLineHeight ;
        CumulativeLeft   = pDockableForLine->GetClientRect().Right() ;
      }
      // New line
      //
      else
      {
        CumulativeTop = CumulativeTop + iLineHeight - 2;
        iLineHeight   = IterLine->Height ;
      }
    }

    IterLine->TopEdge = CumulativeTop ;

    IterLine = IterLine->NextLine;
  }

  CumulativeTop += iLineHeight ;

  // Return the required height/width of the slip, taking all the gridlines
  // into consideration. The sides are slightly different to the top and
  // bottom because the etched edge slip borders are oriented differently
  // relative to the gridlines. CumulativeTop should not be negative - it
  // screws up the 3D client edge of the main client window.
  if (Location == alTop || Location == alBottom || CumulativeTop == -2)
    CumulativeTop += 2;
  return CumulativeTop;
}

/* Previous implementation

int TGridLayout::LayoutGridLines()
{
  // Vertical layout - sets top edge mainly
  int CumulativeTop = -2;
  TGridLine* IterLine = FirstLine;
  while (IterLine)
  {
    IterLine->TopEdge = CumulativeTop;
    IterLine->ComputeHeight();
    CumulativeTop = CumulativeTop + IterLine->Height - 2;

    IterLine = IterLine->NextLine;
  }

  // Return the required height/width of the slip, taking all the gridlines
  // into consideration. The sides are slightly different to the top and
  // bottom because the etched edge slip borders are oriented differently
  // relative to the gridlines. CumulativeTop should not be negative - it
  // screws up the 3D client edge of the main client window.
  if (Location == alTop || Location == alBottom || CumulativeTop == -2)
    CumulativeTop += 2;
  return CumulativeTop;
}
*/

void TGridLayout::LayoutDockables()
// This function iterates through the grid items and implements the results
// of all our calculations.
{
  TPoint ItemPoint;
  TPoint DockPoint;

  // Iterate through the grid lines
  TGridLine* IterLine = FirstLine;
  while (IterLine)
  {
    // Iterate through the grid items in the current grid line
    TGridItem* IterItem = IterLine->FirstItem;
    while (IterItem)
    {
      // Do something to the dockables
      // Make a point from the left of the dockable and the top of its grid line
      if (Location == alTop || Location == alBottom)
      {
        ItemPoint = TPoint(IterItem->LeftEdge, IterLine->TopEdge);
      }
      else
      {
        // Swap the points back over for the real window
        ItemPoint = TPoint(IterLine->TopEdge, IterItem->LeftEdge);
      }

      // Find the current top left point of the dockable
      DockPoint = TPoint(IterItem->Dockable->GetWindowRect().TopLeft());
      ::MapWindowPoints(0, *Parent, &DockPoint, 1);
      // Compare with the current top left point of the dockable
      if (ItemPoint != DockPoint)
      {
        IterItem->Dockable->SetWindowPos(0, ItemPoint.x, ItemPoint.y, 0, 0,
                                         SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
      }

      IterItem = IterItem->NextItem;
    }
    IterLine = IterLine->NextLine;
  }
}

void TGridLayout::InsertGridLine(TGridLine* NewLine)
{
  if (!FirstLine)
  {
    FirstLine = NewLine;
  }
  else
  {
    TGridLine* InsertAfter = 0;
    TGridLine* InsertBefore = 0;
    TGridLine* IterNext = FirstLine;
    // Loop to sort position by LeftEdge
    while (IterNext)
    {
      if (NewLine->TopEdge > IterNext->TopEdge)
      {
        InsertAfter = IterNext;
      }
      else if (!InsertBefore)
      {
        InsertBefore = IterNext;
      }

      IterNext = IterNext->NextLine;
    }

    if (InsertAfter)
    {
      NewLine->PrevLine = InsertAfter;
      InsertAfter->NextLine = NewLine;
    }
    else
      FirstLine = NewLine;

    if (InsertBefore)
    {
      NewLine->NextLine = InsertBefore;
      InsertBefore->PrevLine = NewLine;
    }
  }
}

TGridLine* TGridLayout::InsertGridItem(TGridItem* NewItem)
{
  NewItem->GridLine = 0; // Just to make sure
  NewItem->NextItem = 0; // In case it was removed from another line

  // Find which gridline the item should be inserted into
  TGridLine* IterLine = FirstLine;
  while (IterLine)
  {
    if (NewItem->TopEdge == IterLine->TopEdge)
    {
      IterLine->InsertGridItem(NewItem);
    }
    IterLine = IterLine->NextLine;
  }

  if (!NewItem->GridLine) // No grid line in which to insert it
  {
    // Create a new gridline for the item
    TGridLine* NewLine = new TGridLine(NewItem->TopEdge);
    InsertGridLine(NewLine);
    NewLine->InsertGridItem(NewItem);
  }

  // Return the grid line into which the item was inserted
  return NewItem->GridLine;
}

// ***************************** TFloatingSlipEx ******************************

DEFINE_RESPONSE_TABLE1(TFloatingSlipEx, TFloatingSlip)
  EV_WM_NCLBUTTONDOWN,
  EV_WM_LBUTTONDOWN,
  EV_WM_NCLBUTTONDBLCLK,
END_RESPONSE_TABLE;

TFloatingSlipEx::TFloatingSlipEx(TWindow* parent, int x, int y, TWindow* clientWnd,
  bool shrinkToClient, int captionHeight, bool popupPalette, TModule* module):
  TFloatingSlip(parent, x, y, clientWnd, shrinkToClient, captionHeight,
    popupPalette, module)
{
}

TFloatingSlipEx::~TFloatingSlipEx()
{
}

void TFloatingSlipEx::DockableInsert(TDockable& dockable, const TPoint* topLeft,
  TRelPosition position, TDockable* relDockable)
{
  TFloatingSlip::DockableInsert(dockable, topLeft, position, relDockable);

  TDockableControlBarEx* cb = TYPESAFE_DOWNCAST(dockable.GetWindow(),
  	TDockableControlBarEx);
  if (cb)
  {
    //DW: Set the Margins
    //
    cb->SetMargins(TMargins(TMargins::Pixels,
    	FloatMarginsX, FloatMarginsX, FloatMarginsY, FloatMarginsY));
  }
}

void TFloatingSlipEx::EvNCLButtonDown(uint hitTest, TPoint& point)
{
#ifdef CTXHELP
  TCtxHelpFileManager* ctxHelpM = TYPESAFE_DOWNCAST(GetApplication(),
  	TCtxHelpFileManager);
  if (ctxHelpM && ctxHelpM->IsContextHelp())
  {
    // click in the titlebar of floating slip: call help with Attr.Id != 0
    //
    TDockableControlBarEx* cb = TYPESAFE_DOWNCAST(GetWindow(),
    	TDockableControlBarEx);
    if (cb && cb->Attr.Id != 0)
		  ctxHelpM->WinHelp(cb->Attr.Id);
  }
  else
#endif
    TFloatingSlip::EvNCLButtonDown(hitTest, point);
}

void TFloatingSlipEx::EvLButtonDown(uint hitTest, TPoint& point)
{
#ifdef CTXHELP
  TCtxHelpFileManager* ctxHelpM = TYPESAFE_DOWNCAST(GetApplication(),
  	TCtxHelpFileManager);
  if (ctxHelpM && ctxHelpM->IsContextHelp())
  {
    // click in the margings of floating slip: call help with Attr.Id != 0
    //
    TDockableControlBarEx* cb = TYPESAFE_DOWNCAST(GetWindow(),
    	TDockableControlBarEx);
    if (cb && cb->Attr.Id != 0)
		  ctxHelpM->WinHelp(cb->Attr.Id);
  }
  else
#endif
    TFloatingSlip::EvLButtonDown(hitTest, point);
}

void TFloatingSlipEx::EvNCLButtonDblClk(uint modKeys, TPoint& point)
{
  TDockableControlBarEx* cb = TYPESAFE_DOWNCAST(GetWindow(),
  	TDockableControlBarEx);
 	if (cb)
    cb->ToggleSlip();
  TFloatingSlip::EvNCLButtonDblClk(modKeys, point);
}

// ****************** TEdgeSlipEx *********************************************

DEFINE_RESPONSE_TABLE1(TEdgeSlipEx, TWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_NCCALCSIZE,
  EV_WM_NCPAINT,
  EV_WM_ERASEBKGND,
  EV_WM_PARENTNOTIFY,
  EV_WM_WINDOWPOSCHANGING,
END_RESPONSE_TABLE;

TEdgeSlipEx::TEdgeSlipEx(TDecoratedFrame& parent, TAbsLocation location, TModule* module)
:
  TWindow(&parent, "EdgeSlipEx", module),
  Location(location),
  GridType(Location == alTop || Location == alBottom ? YCoord : XCoord)
{
  SetBkgndColor(TColor::Sys3dFace);
  Attr.Style = (WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
  InsertingDockable = false;
}

TEdgeSlipEx::~TEdgeSlipEx()
{
  Destroy(IDCANCEL);
}

void TEdgeSlipEx::SetGripperMargins(TDockableControlBarEx* cb)
// Set the margins for the gripper
{
  if (cb)
  {
    THarborEx* harbor = TYPESAFE_DOWNCAST(cb->GetHarbor(), THarborEx);
    if (harbor)
    {
      int gripperSize = 3*GripperDistSize;
      if (Location == alTop || Location == alBottom)
      {
        cb->SetMargins(TMargins(TMargins::Pixels,
          (harbor->IsDrawGripper()) ? gripperSize+EdgeMaginsX : EdgeMaginsX,
          EdgeMaginsX,
          EdgeMaginsY,
          EdgeMaginsY));
      }
      else
      {
        cb->SetMargins(TMargins(TMargins::Pixels,
          EdgeMaginsX,
          EdgeMaginsX,
          (harbor->IsDrawGripper()) ? gripperSize+EdgeMaginsY : EdgeMaginsY,
          EdgeMaginsY));
      }
    }
  }
}

void TEdgeSlipEx::DockableInsert(TDockable& dockable, const TPoint* topLeft,
  TRelPosition position, TDockable* relDockable)
// Similar to TEdgeSlip
{
  // Get dockable's window & hide it in case we have to toss it around a bit
  // Reparent the window to the edge slip
  TWindow* dockableWindow = dockable.GetWindow();
  dockableWindow->ShowWindow(SW_HIDE);
  dockableWindow->SetParent(this);

  //DW: Set the margins for the Gripper
  //
  TDockableControlBarEx* cb = TYPESAFE_DOWNCAST(dockableWindow,
  	TDockableControlBarEx);
  SetGripperMargins(cb);

  // Let window know it was docked...
  if (dockableWindow->IsWindow())
    dockableWindow->SendMessage(WM_OWLWINDOWDOCKED, TParam1(position),
                                TParam2((TDockingSlip*)this));

  // Make sure that the dockable is oriented the right way--horizontal layout
  // for Y-gridded slips
  InsertingDockable = true;  // Prevent EvParentNotify from doing anything
  dockable.Layout(GridType == YCoord ? alTop : alLeft);
  InsertingDockable = false;

  DockableMove(dockable, topLeft, position, relDockable);
  dockableWindow->ShowWindow(SW_SHOWNA);
}

void TEdgeSlipEx::DockableMove(TDockable& dockable, const TPoint* topLeft,
  TRelPosition /*position*/, TDockable* /*relDockable*/)
{
  TWindow* dockableWindow = dockable.GetWindow();

  if (topLeft)
  {
    // Move the window into position relative to its parent (this)
    ::MapWindowPoints(0, *this, (TPoint*)topLeft, 1);
    dockableWindow->SetWindowPos(0, topLeft->x, topLeft->y, 0, 0,
                 SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
  }
  else
  {
    // A stand-in until a better method of approximation is used
    dockableWindow->SetWindowPos(0, -2, -2, 0, 0,
                 SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
  }

  // Now do the magic which shuffles all the dockables about
  ArrangeAllDockables(dockableWindow);
}

void TEdgeSlipEx::DockableRemoved(const TRect& /*orgRect*/)
{
  // Do the magic which shuffles all the dockables about
  // Should just close up empty gridlines
  ArrangeAllDockables(0);
}

void TEdgeSlipEx::ArrangeAllDockables(TWindow* NewDockable)
{
  TGridLayout GridLayout(Location);
  GridLayout.CreateLists(this, NewDockable);

  TGridLine* DockableLine = GridLayout.InsertDockableItem();
  if (DockableLine)
  {
    int SlipWidth;
    if (Location == alTop || Location == alBottom)
    {
      // This gave zero if the slip was just created - first toolbar positioned
      // always at the far left, not the drop position.
      SlipWidth = GetWindowRect().Width();

      //SlipWidth = max(Parent->GetClientRect().Width(), 0);
    }
    else
    {
      // This gave zero if the slip was just created - first toolbar positioned
      // always at the top, not the drop position.
      SlipWidth = max(GetWindowRect().Height() - 4, 0); // Compensate for borders

      //TFrameWindow* Frame = TYPESAFE_DOWNCAST(Parent, TFrameWindow);
      // This makes no allowance for the possible increase if the new toolbar was
      // dragged from the top or bottom slips, or for the offset if the new
      // toolbar was dragged from the top slip.
      //SlipWidth = max(Frame->GetClientWindow()->GetClientRect().Height() - 4, 0);
    }

    TGridItem* OverItem = DockableLine->LayoutGridItems(GridLayout.DockableItem, SlipWidth);
    if (OverItem)
    {
      // This will deal with wrapping items pushed off the right edge.
      // Assumes we only need to wrap at most one new grid line.
      TGridItem* IterItem = OverItem;
      while (IterItem)
      {
        TGridItem* IterNext = IterItem->NextItem;

        DockableLine->RemoveGridItem(IterItem);
        IterItem->TopEdge += 1;  // Force a new line to be inserted
        IterItem->LeftEdge = -2; // Jam over to the left side
        GridLayout.InsertGridItem(IterItem);

        IterItem = IterNext;
      }
      TGridLine* ExtraLine = OverItem->GridLine;
      // Layout the new line
      ExtraLine->LayoutGridItems(OverItem, SlipWidth);
    }
  }
  int SlipHeight = GridLayout.LayoutGridLines();

  // Translate the calculations to actual window positions
  GridLayout.LayoutDockables();

  // Alter the size of the slip itself if necessary
  LayoutSlip(SlipHeight);
}

void TEdgeSlipEx::LayoutSlip(int Height)
{
  // This resizes the edge slip by modifying the layout metrics held by its
  // parent frame
  bool SizeChanged;
  if (Location == alTop || Location == alBottom)
  {
    SizeChanged = Height != Attr.H;
    Attr.H = Height;
  }
  else
  {
    SizeChanged = Height != Attr.W;
    Attr.W = Height;
  }

  if (SizeChanged)
  {
    TDecoratedFrame* Frame = TYPESAFE_DOWNCAST(Parent, TDecoratedFrame);
    TLayoutMetrics Metrics;
    Frame->GetChildLayoutMetrics(*this, Metrics);
    Frame->SetChildLayoutMetrics(*this, Metrics);
    Frame->Layout();
  }
}

TAbsLocation TEdgeSlipEx::GetLocation() const
// Copied more or less from TEdgeSlip implemenation
{
  return Location;
}

TResult TEdgeSlipEx::EvCommand(uint id, THandle hWndCtl, uint notifyCode)
// Copied more or less from TEdgeSlip implemenation
{
  if (notifyCode == 0 && Parent)
    return Parent->EvCommand(id, hWndCtl, notifyCode);

  return TWindow::EvCommand(id, hWndCtl, notifyCode);
}

void TEdgeSlipEx::EvCommandEnable(TCommandEnabler& commandEnabler)
// Copied more or less from TEdgeSlip implemenation
{
  if (Parent)
  {
    // Already being processed?
    if (!commandEnabler.IsReceiver(Parent->GetHandle()))
    {
      // No, so forward it up to our parent
      commandEnabler.SetReceiver(Parent->GetHandle());
      Parent->EvCommandEnable(commandEnabler);
    }
  }
}

void TEdgeSlipEx::SetupWindow()
{
  TWindow::SetupWindow();

  // Give an initial position to all the child windows
  // This part may not be required if user wants to set positions
  // directly via Attr
  TWindow* FirstChild = GetFirstChild();
  if (FirstChild)
  {
    TWindow* ChildWindow = FirstChild;
    do
    {
      // A stand-in until a better method of approximation is used
      ChildWindow->SetWindowPos(0, -2, -2, 0, 0,
                                   SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER);
      ChildWindow = ChildWindow->Next();
    }
    while (ChildWindow != FirstChild); // Does Next() cycle?
  }

  TGridLayout GridLayout(Location);
  GridLayout.CreateLists(this, 0); //No Anchor
  int SlipWidth;
  if (Location == alTop || Location == alBottom)
    SlipWidth = GetWindowRect().Width();
  else
    SlipWidth = max(GetWindowRect().Height() - 4, 0); // Compensate for borders

  // This spaces the grid lines vertically to ensure there is space to insert
  // new lines if necessary
  GridLayout.LayoutGridLines();

  TGridLine* DockableLine = GridLayout.FirstLine;
  // Cycle through the grid lines to lay them all out
  while (DockableLine)
  {
    TGridItem* OverItem = DockableLine->LayoutGridItems(0, SlipWidth);
    TGridLine* ExtraLine = DockableLine;
    while (OverItem)
    {
      // This will deal with wrapping items pushed off the right edge
      // Loops in case of multiple wraps
      TGridItem* IterItem = OverItem;
      while (IterItem)
      {
        TGridItem* IterNext = IterItem->NextItem;

        ExtraLine->RemoveGridItem(IterItem);
        IterItem->TopEdge += 1;  // Force a new line to be inserted
        IterItem->LeftEdge = -2; // Jam over to the left side
        GridLayout.InsertGridItem(IterItem);

        IterItem = IterNext;
      }
      // Change to the newly created line
      ExtraLine = OverItem->GridLine;
      // Layout the new line
      OverItem = ExtraLine->LayoutGridItems(OverItem, SlipWidth);
    }
    // Not the best scheme since newly inserted lines will be laid out twice.
    DockableLine = DockableLine->NextLine;
  }
  int SlipHeight = GridLayout.LayoutGridLines();

  // Translate the calculations to actual window positions
  GridLayout.LayoutDockables();

  // Alter the size of the slip itself if necessary
  LayoutSlip(SlipHeight);
}

void TEdgeSlipEx::EvLButtonDown(uint modKeys, TPoint& point)
// Copied more or less from TEdgeSlip implemenation
{
  TWindow* cw = GetWindowPtr(ChildWindowFromPoint(point));

  // Only allow immediate children of the docking window to be clicked on.
  if (cw && cw->Parent == this)
  {
    // Is the mouseDown in a area where we can move the docked window?
    TPoint childPt = point;
    MapWindowPoints(*cw, &childPt, 1);
    TDockable* d = TYPESAFE_DOWNCAST(cw, TDockable);
    if (d && d->ShouldBeginDrag(childPt))
    {
      MapWindowPoints(0, &point, 1);
      if (Harbor && Harbor->DockDraggingBegin(*d, point, Location, this))
      {
        return;          // Successfully started
      }
    }
  }
  TWindow::EvLButtonDown(modKeys, point);
}

void TEdgeSlipEx::EvLButtonDblClk(uint /*modKeys*/, TPoint& /*point*/)
// Copied more or less from TEdgeSlip implemenation
{
  Parent->HandleMessage(WM_OWLSLIPDBLCLK);
}

uint TEdgeSlipEx::EvNCCalcSize(bool calcValidRects, NCCALCSIZE_PARAMS far& calcSize)
// Copied more or less from TEdgeSlip implemenation
{
  // Why is this function required?
  // Does it affect the interpretation of the client coordinates?
  // Yes - this appears to be the case.

  uint ret = TWindow::EvNCCalcSize(calcValidRects, calcSize);
  if (IsIconic())
    return ret;

  // Only add in space if this slip is not shrunk to nothing
  if (calcSize.rgrc[0].bottom - calcSize.rgrc[0].top > 0)
  {
    if (!(Attr.Style & WS_BORDER))
    {
      if (Location != alBottom)
        calcSize.rgrc[0].top += 2;
      if (Location != alTop)
        calcSize.rgrc[0].bottom -= 2;
    }
  }
  return 0;
}

void TEdgeSlipEx::EvNCPaint(HRGN)
// Copied more or less from TEdgeSlip implemenation
{
  // Non-3d style
  if (Attr.Style & WS_BORDER)
  {
    DefaultProcessing();
  }
  // Use 3-d style
  else
  {
    // Paint etched line along the top for left, top & right slips, and along
    // the bottom for bottom, left & right slips to separate from the menubar,
    // statusbar & each other.
    TWindowDC dc(*this);
    int height(GetWindowRect().Height());
    if (Location != alBottom)
      TUIBorder(TRect(0,0,9999,2), TUIBorder::EdgeEtched, TUIBorder::Top).Paint(dc);
    if (Location != alTop)
      TUIBorder(TRect(0,height-2,9999,height), TUIBorder::EdgeEtched, TUIBorder::Bottom).Paint(dc);
  }
}

bool TEdgeSlipEx::EvEraseBkgnd(HDC hDC)
// Copied more or less from TEdgeSlip implemenation
{
  TWindow::EvEraseBkgnd(hDC);  // Let TWindow erase everything
  return true;
}

void TEdgeSlipEx::EvParentNotify(uint event, uint /*childHandleOrX*/, uint /*childIDOrY*/)
// Copied more or less from TEdgeSlip implemenation
{
  if (event == WM_SIZE && !InsertingDockable)
    ArrangeAllDockables(0); // Can cause double Layout of frame window
  else
    DefaultProcessing();
}

void TEdgeSlipEx::EvWindowPosChanging(WINDOWPOS far& windowPos)
// Copied more or less from TEdgeSlip implemenation
{
  //if (!(windowPos.flags & SWP_NOSIZE))
  //{
  //  CompressParallel(windowPos.cx);
  //}
  TWindow::EvWindowPosChanging(windowPos);
}


