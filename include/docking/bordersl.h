////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//  Class definition for TBorderSlip (TWindow and TDockingSlip)
//  and associated structures.
//
//  Created by:
//  ===========
//  Alan Chambers, Birmingham, England.
//  e-mail: myrddin@foxholly.demon.co.uk
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if !defined(borderslip_h)
#define borderslip_h
#include <owl/docking.h>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class TBorderSlip : public TWindow, public TDockingSlip
{
  public:
    TBorderSlip(TDecoratedFrame& parent, TAbsLocation location, TModule* module = 0);
    virtual ~TBorderSlip();

    // Overridden TWindow virtuals
    TResult  EvCommand(uint id, THandle hWndCtl, uint notifyCode);
    void     EvCommandEnable(TCommandEnabler& commandEnabler);
    void     SetupWindow();

    // These are overriden for any window that wants to be notified on dockable
    // windows being moved or removed.
    void DockableInsert(TDockable& dockable, const TPoint* topLeft = 0,
      TRelPosition position = rpNone, TDockable* relDockable = 0);
    void DockableMove(TDockable& dockable, const TPoint* topLeft = 0,
      TRelPosition position = rpNone, TDockable* relDockable = 0);
    void DockableRemoved(const TRect& orgRect);
    TAbsLocation GetLocation() const;

  protected:
    // Response functions
    void     EvLButtonDown(uint modKeys, TPoint& point);
    void     EvLButtonDblClk(uint modKeys, TPoint& point);
    uint     EvNCCalcSize(bool calcValidRects, NCCALCSIZE_PARAMS far& calcSize);
    void     EvNCPaint(HRGN);
    bool     EvEraseBkgnd(HDC);
    void     EvParentNotify(uint event, uint childHandleOrX, uint childIDOrY);
    void     EvWindowPosChanging(WINDOWPOS far& windowPos);

    // Internal tiling functions and data
    void ArrangeAllDockables(TWindow* NewDockable);
    void LayoutSlip(int Height);
    TAbsLocation Location;
    TGridType    GridType;
    bool InsertingDockable; // To prevent parent notify layout during other layout

  DECLARE_RESPONSE_TABLE(TBorderSlip);
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Forward declaration
struct TGridLine;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Represents a single dockable in the slip
struct TGridItem
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


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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

