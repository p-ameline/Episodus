//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1991, 1995 by Borland International, All Rights Reserved
//
//   Object windows bitmap scrolling app resource defines
//----------------------------------------------------------------------------

#if !defined(__NS_BMPV_H)
#define 		 __NS_BMPV_H


#include <owl\docmanag.h>
#include <owl\docview.h>

#include <owl/pch.h>
#include <owl/applicat.h>
#include <owl/dc.h>
#include <owl/printer.h>
#include <owl/menu.h>
#include <owl/dibitmap.h>
#include <owl/framewin.h>
#include <owl/scroller.h>
#include <owl/opensave.h>
#include <owl/clipview.h>
#include <owl/inputdia.h>
#include <string.h>
#include <stdio.h>
#include <dir.h>

#include "nautilus\nsbmpv.rh"
#include "nautilus\nsdocref.h"

DIAG_DEFINE_GROUP_INIT(OWL_INI, BmpView, 1, 0);

#define USE_DDB     // Use a DDB, else blts the DIB directly to the screen
// #define PAINT_ICON  // define to paint the icon from the bitmap

// static const char AppName[] = "BitmapView";

// #if defined(_DANSBMPDLL)
//		#define _NSBMPVCLASS __export
// #else
//		#define _NSBMPVCLASS __import
// #endif


// Classe Document Bitmap : contient un fichier Bitmap
// ----------------------------------------------------------------------------

#define _NSBMPVCLASS _USERCLASS

// Classe document ttx

class  NSBmpDocument : public NSRefDocument
{
	public:
   	string		sBitmap;

      NSBmpDocument(TDocument *parent, NSDocumentInfo *pDocumentInfo,
      					NSDocumentInfo* pDocHtmlInfo, NSContexte *pCtx);

      NSBmpDocument(TDocument *parent = 0, NSContexte *pCtx = 0);

   	~NSBmpDocument() {}

      // Entrée-Sortie de fichier texte
      void Sauvegarde();
      bool Ouverture(char *fBmp);

      // Méthodes virtuelles de TDocument
      bool Open(int mode, const char far *path = 0);
      bool Close();
      bool Commit(bool force = false);
      bool Revert(bool clear = false);
      bool IsOpen();
};

//
// class TWaitCursor
// ~~~~~ ~~~~~~~~~~~
// General wait cursor wrapper. Changes cursor to an hourglass during its scope
// ----------------------------------------------------------------------------

class  TWaitCursor {
  public:
    TWaitCursor()
    :
      HCursor(::SetCursor(::LoadCursor(0, IDC_WAIT)))
    {
    }

   ~TWaitCursor()
    {
      ::SetCursor(HCursor);
    }
  private:
    HCURSOR HCursor;
};

//
// class TWindowPrintout
// ~~~~~ ~~~~~~~~~~~~~~~
class TWindowPrintout : public TPrintout {
  public:
    TWindowPrintout(const char* title, TWindow* window);

    void GetDialogInfo(int& minPage, int& maxPage,
                       int& selFromPage, int& selToPage);
    void PrintPage(int page, TRect& rect, unsigned flags);
    void SetBanding(bool b)
    {
      Banding = b;
    }
    bool HasPage(int pageNumber)
    {
      return pageNumber == 1;
    }

  protected:
    TWindow* Window;
    bool     Scale;
};

//
// class NSBmpView
// ~~~~~ ~~~~~~~~~~~~~~
// A bitmap displaying window derived from TClipboardViewer to
// facilitate receiving of clipboard change notifications. Could mix it in if
// an additional base was desired.
// ----------------------------------------------------------------------------

class  NSBmpView : public TView,
										 virtual public TWindow,
										 public TClipboardViewer {
  public:
    bool      ScreenPalettized;  // True if the screen is a palettized device
    TPalette* DefPalette;        // Palette to use for DIBs w/o color tables
    TBrush*   BkgndBrush;        // Window background brush
    long      Rop;               // Bitmap -> screen raster op

    bool      Fit;               // Stretch bitmap to fit?
    bool      AutoClipView;      // Clipboard auto-display mode
    bool      PaintFromDDB;      // Paint DDB to screen, vs. DIB
    bool      PaintIcon;         // Paint icon from DIB, vs. from resource

    string    FileName;
    TDib*     Dib;
    TBitmap*  Bitmap;
    HBITMAP   MemHandle;
    TPalette* Palette;
    int       PixelWidth;
    int       PixelHeight;
    uint16    Colors;

    TPrinter* pPrinter;

    NSBmpView(NSBmpDocument& doc, TWindow *parent = 0);
   ~NSBmpView();

    // Redéfiniton des fonctions virtuelles de TView
    static LPCSTR	StaticName() 	{ return "Bitmap Viewer"; }
    const char*	GetViewName() 	{ return StaticName(); }
    TWindow*		GetWindow();
    bool 			CanClose();  

    // Remplissage de la vue
    void 			SetupWindow();
    void 			FillViewData();

  protected:

    NSBmpDocument 	*pDocBmp;		// Pointeur sur document BMP
    // TControlBar		*pToolBar;		// Barre d'outils

  	 // Fonctions de notification au document
    bool		  VnCommit(bool force);
    bool		  VnRevert(bool clear);

    // Fonctions de réponse aux messages de commande
    void 	  CmPublier();
    void 	  CmComposer();

    void      CmFileOpen();
    void      CmRead1();
    void      CmRead2();
    void      CmReadBad();
    void      CmMemHandle();
    void      CmMemHandleRefresh();
    void		  CmFilePrint();
    void      CmCopy();
    void      CmPaste();
    void      CmFit();
    void      CmAutoClipView();
    void      CmPaintFromDDB();
    void      CmPaintIcon();
    void      CeCopy(TCommandEnabler& ce);
    void      CePaste(TCommandEnabler& ce);
    void      CeFit(TCommandEnabler& ce);
    void      CeAutoClipView(TCommandEnabler& ce);
    void      CePaintFromDDB(TCommandEnabler& ce);
    void      CePaintIcon(TCommandEnabler& ce);

    void      Paint(TDC&, bool erase, TRect&);
    void      EvSize(uint sizeType, TSize&);

    void      EvPaletteChanged(HWND hWndPalChg);
    bool      EvQueryNewPalette();
    void      EvDrawClipboard();
    void      EvDestroy();

    bool      UpdatePalette(bool alwaysRepaint);
    void      AdjustScroller();
    void      SetCaption(const char*);
    void      SetupFromDib(TDib* dib);
    bool      LoadBitmapFile(const char*);
    bool      LoadBitmapResource(uint16 resId);
    bool      LoadMemHandle(HBITMAP hBitmap);

  DECLARE_RESPONSE_TABLE(NSBmpView);
};

#if defined __NS_BMPV_CPP
	DEFINE_DOC_TEMPLATE_CLASS(NSBmpDocument,NSBmpView,BmpDlgViewTmpl);
#else
	typedef TDocTemplateT<NSBmpDocument,NSBmpView> BmpDlgViewTmpl;
#endif

#endif		// __NS_BMPV_H

