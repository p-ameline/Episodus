//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1991, 1995 by Borland International, All Rights Reserved
//
//   Example program that shows Dibs, Bitmaps and Palettes in a scrolling
//   Window. Also uses diagnostics to trace thru some routines
//----------------------------------------------------------------------------

// NSBMPV.CPP		Document/Vues Afficheur Bitmap
// ------------------------------------------------
// Rémi SPAAK - Juin 1997
//----------------------------------------------------------------------------

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <fstream.h>
#include <owl/oleview.h>

#define __NS_BMPV_CPP

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\nsbmpv.h"

#include "nautilus\nautilus.rh"

TWindowPrintout::TWindowPrintout(const char* title, TWindow* window)
:
  TPrintout(title)
{
  Window = window;
  Scale = true;
}

void
TWindowPrintout::PrintPage(int, TRect& rect, unsigned)
{
  // Conditionally scale the DC to the window so the printout will
  // resemble the window
  //
  int    prevMode;
  TSize  oldVExt, oldWExt;
  if (Scale) {
    prevMode = DC->SetMapMode(MM_ISOTROPIC);
    TRect windowSize = Window->GetClientRect();
    DC->SetViewportExt(PageSize, &oldVExt);
    DC->SetWindowExt(windowSize.Size(), &oldWExt);
    DC->IntersectClipRect(windowSize);
    DC->DPtoLP(rect, 2);
  }

  // Call the window to paint itself
  Window->Paint(*DC, false, rect);

  // Restore changes made to the DC
  if (Scale) {
    DC->SetWindowExt(oldWExt);
    DC->SetViewportExt(oldVExt);
    DC->SetMapMode(prevMode);
  }
}

// Do not enable page range in the print dialog since only one page is
// available to be printed
//
void
TWindowPrintout::GetDialogInfo(int& minPage, int& maxPage,
                               int& selFromPage, int& selToPage)
{
  minPage = 0;
  maxPage = 0;
  selFromPage = selToPage = 0;
}

// Constructeur NSBmpDocument
////////////////////////////////////////////////////////////////
NSBmpDocument::NSBmpDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
										NSDocumentInfo* pDocHtmlInfo, NSSuper* pSup)
				  :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pSup)
{
	sBitmap = "";
	Open(0, "");
}

NSBmpDocument::NSBmpDocument(TDocument *parent, NSSuper *pSup)
				  :NSRefDocument(parent,pSup)
{
	sBitmap = "";
	Open(0, "");
}

// Ouverture du document
////////////////////////////////////////////////////////////////
bool NSBmpDocument::Open(int /*mode*/, LPCSTR path)
{
  if (!IsDirty())
  {
	 // if (path)
	 //	SetDocPath(path);
	 SetDirty(FALSE);
  }
  return TRUE;
}

// Fermeture du document
////////////////////////////////////////////////////////////////
bool NSBmpDocument::Close()
{
  if (TDocument::Close())
	 return FALSE;

  sBitmap = "";
  return TRUE;
}

// Fonction Commit
////////////////////////////////////////////////////////////////
bool NSBmpDocument::Commit(bool force)
{
  if (!IsDirty() && !force)
	 return TRUE;
  SetDirty(FALSE);
  return TRUE;
}

// Fonction Revert
////////////////////////////////////////////////////////////////
bool NSBmpDocument::Revert(bool clear)
{
  if (!TDocument::Revert(clear))
	 return FALSE;
  if (!clear)
	 Open(0);
  return TRUE;
}

// Indique si document ouvert
////////////////////////////////////////////////////////////////
bool NSBmpDocument::IsOpen()
{
  return (sBitmap != "");
}

// Sauve fichier texte/RTF du document
////////////////////////////////////////////////////////////////
void NSBmpDocument::Sauvegarde()
{
	int i, j;
	char nomFichier[13], *nomLibre;
	ofstream outFile;
	//
	// Si il existe, prise du nom de fichier de la fiche document
	//
	i = 1;
	if (pDocInfo != 0)
		i = pDocInfo->pDonnees->donneNomDos(nomFichier);
	//
	// Sinon, prise du premier nom de fichier libre
	//
	if (i == 1)
	{
		nomLibre = 0;
		i = DonneNomFichier(pSuper->pUtilisateur->pPatient->pDonnees->nss,
								  "BMP", "", &nomLibre);
		if (i == 0)
		{
			strcpy(nomFichier, nomLibre);
			delete[] nomLibre;
		}
		else
		{
			if (nomLibre != 0)
				delete[] nomLibre;
			return;
		}
		//
		// Création d'une fiche Document
		//
		if (pDocInfo == 0)
		{
			NSDocument* pNewDocument = new NSDocument(pSuper);
			pNewDocument->lastError = pNewDocument->open();
			if (pNewDocument->lastError != DBIERR_NONE)
			{
				erreur("Impossible d'ouvrir le fichier des Documents.", 0, pNewDocument->lastError);
				delete pNewDocument;
				return;
			}
			pNewDocument->pDonnees->metABlanc();
			char code[DOC_CODE_DOCUM_LEN + 1];
			char nom[DOC_NOM_LEN + 1];
			char creation[DOC_CREATION_LEN + 1];
			//
			// Niveau de sécurité pour l'accès : Celui de l'utilisateur
			strcpy(pNewDocument->pDonnees->acces,
										pSuper->pUtilisateur->pDonnees->groupe);
			//
			// Créateur : Utilisateur en cours
			strcpy(pNewDocument->pDonnees->createur,
										pSuper->pUtilisateur->getszNss());
			//
			// Type de document : Texte Nautilus au format RTF
			strcpy(pNewDocument->pDonnees->type, "IFBMP");
			//
			// Mise du nom de fichier à sa place
			pNewDocument->pDonnees->rangeNomDos(nomFichier);
			//
			// Intêret 5 = moyen
			strcpy(pNewDocument->pDonnees->interet, "5");
			//
			// Date de creation = date du jour
			donne_date_duJour(creation);
			strcpy(pNewDocument->pDonnees->creation, creation);
			//
			// Demande de création et référencement du document
			//
			if (!(pNewDocument->CreeDocument()))
			{
				pNewDocument->close();
				delete pNewDocument;
				return;
			}
			else
			{
				pNewDocument->close();
				pDocInfo = new NSDocumentInfo(pNewDocument);
				delete pNewDocument;
			}
		}
	}
	//
	// Ouverture du fichier
	//
	outFile.open(nomFichier);
	//
	// Si la tentative a échoué, ouverture du fichier standard
	//
	if (!outFile)
		return;
	for (i = 0; i < sBitmap.length(); i++)
		outFile.put(char(sBitmap[i]));
	outFile.close();
}

// Ouvre fichier texte/RTF du document
////////////////////////////////////////////////////////////////
bool NSBmpDocument::Ouverture(char *fBmp)
{
	int i;
	char nomFichier[13];
	ifstream inFile;
	//
	// Mise à vide de la chaine texte
	//
	sBitmap = "";
	//
	// Si il existe, prise du nom de fichier de la fiche document
	//
	i = 1;
	if (pDocInfo != 0)
		i = pDocInfo->pDonnees->donneNomDos(nomFichier);
	//
	// Sinon, on sort
	//
	if (i == 1)
		return false;
	//
	// Ouverture du fichier et prise de sBitmap
	//
	inFile.open(nomFichier);
	if (!inFile)
		return false;
	sBitmap.read_file(inFile);
	inFile.close();
   strcpy(fBmp,nomFichier);
   return true;
}

// Instanciation de la classe modele de document
////////////////////////////////////////////////////////////////
BmpDlgViewTmpl BmpTmpl("BMP Format",
							  "*.bmp",
								0,
							  "BMP",
								dtAutoOpen|dtAutoDelete|dtUpdateDir|dtFileMustExist);


DEFINE_RESPONSE_TABLE3(NSBmpView, TView, TWindow, TClipboardViewer)

  EV_VN_COMMIT,
  EV_VN_REVERT,

  EV_COMMAND(CM_COMPOSE, CmComposer),
  EV_COMMAND(CM_IMPRIME, CmPublier),

  EV_COMMAND(CM_FILEOPEN, CmFileOpen),
  EV_COMMAND(CM_READ1, CmRead1),
  EV_COMMAND(CM_READ2, CmRead2),
  EV_COMMAND(CM_READBAD, CmReadBad),
  EV_COMMAND(CM_MEMHANDLE, CmMemHandle),
  EV_COMMAND(CM_MEMHANDLEREFRESH, CmMemHandleRefresh),
  EV_COMMAND(CM_FILEPRINT, CmFilePrint),
  EV_COMMAND(CM_EDITCOPY, CmCopy),
  EV_COMMAND(CM_EDITPASTE, CmPaste),
  EV_COMMAND(CM_FIT, CmFit),
  EV_COMMAND(CM_AUTOCLIPVIEW, CmAutoClipView),
  EV_COMMAND(CM_PAINTFROMDDB, CmPaintFromDDB),
  EV_COMMAND(CM_PAINTICON, CmPaintIcon),
  EV_COMMAND_ENABLE(CM_EDITCOPY, CeCopy),
  EV_COMMAND_ENABLE(CM_EDITPASTE, CePaste),
  EV_COMMAND_ENABLE(CM_FIT, CeFit),
  EV_COMMAND_ENABLE(CM_AUTOCLIPVIEW, CeAutoClipView),
  EV_COMMAND_ENABLE(CM_PAINTFROMDDB, CePaintFromDDB),
  EV_COMMAND_ENABLE(CM_PAINTICON, CePaintIcon),

  EV_WM_SIZE,
  EV_WM_PALETTECHANGED,
  EV_WM_QUERYNEWPALETTE,
  EV_WM_DRAWCLIPBOARD,
  EV_WM_DESTROY,

END_RESPONSE_TABLE;

// FONCTIONS DE LA CLASSE BMPVIEW
///////////////////////////////////////////////////////////////////////////////
//
// Constructor for a TBmpViewWindow, sets scroll styles and constructs
// the Scroller object.  Also sets the Rop based on whether the display
// is monochrome (two-color) or polychrome.
//

// NSTtxView::NSTtxView(NSTtxDocument& doc, TWindow* parent) :
// TView(doc),pDocTtx(&doc),TRichEdit(parent,IDRichEd,"",0,0,0,0)


NSBmpView::NSBmpView(NSBmpDocument& doc, TWindow *parent)
:
  TView(doc),
  TWindow(parent,0,0),   // avant TWindow(0,0,0)
  pDocBmp(&doc),
  TClipboardViewer()
{

  Attr.Style |= WS_BORDER | WS_VSCROLL | WS_HSCROLL;
  Attr.ExStyle |= WS_EX_CLIENTEDGE;
  Attr.AccelTable = IDA_BMPVIEW;
  SetViewMenu(new TMenuDescr(BITMAPVIEW,1,1,1,-1,0,0));

  pPrinter = new TPrinter(GetApplication());

  BkgndBrush = new TBrush(TColor::SysWindow);
  Scroller = new TScroller(this, 1, 1, 200, 200);

  Fit = false;
  AutoClipView = false;
  PaintFromDDB = true;
  PaintIcon = false;

  Dib = 0;
  Bitmap = 0;
  MemHandle = 0;
  Palette = 0;

  // If the screen is a palettized device, then we'll need a halftone palette
  // for dibs with > 8 bpp
  //
  TScreenDC screenDC;
  if (screenDC.GetDeviceCaps(RASTERCAPS) & RC_PALETTE) {
    ScreenPalettized = true;
#if defined(BI_PLAT_WIN32)
    DefPalette = new THalftonePalette;
#else
    DefPalette = new TPalette((HPALETTE)::GetStockObject(DEFAULT_PALETTE));
#endif
  }
  else {
    ScreenPalettized = false;
    DefPalette = 0;
  }

  // Raster op to use when blitting to the screen
  //
  Rop = SRCCOPY;

  SetCaption(0);
  SetBkgndColor(TColor::Transparent);  // Don't pre-erase image background
}

//
//
//
NSBmpView::~NSBmpView()
{
  delete Bitmap;
  if (Palette != DefPalette)
    delete Palette;
  delete DefPalette;
  delete Dib;
  delete BkgndBrush;
  delete pPrinter;
}

TWindow*
NSBmpView::GetWindow()
{
	return (TWindow *) this;
}

// Appel de la fonction de remplissage de la vue
////////////////////////////////////////////////////////////////
void
NSBmpView::SetupWindow()
{
   TWindow::SetupWindow();
  	FillViewData();
}
// Remplissage de la vue////////////////////////////////////////////////////////////////
void
NSBmpView::FillViewData()
{
	char fBitmap[13] ;
  char chAffiche[200] ;
  char dateAffiche[20] ;

	if (pDocBmp->Ouverture(fBitmap))  {
  	LoadBitmapFile(fBitmap) ;
    strcpy(chAffiche, pDocBmp->pDocInfo->pDonnees->nom) ;
    ote_blancs(chAffiche) ;
    if (strcmp(pDocBmp->pDocInfo->pDonnees->creation, "        ") != 0)
		{
    	strcat(chAffiche, " du ") ;
      string sLang = "" ;			if (pContexte->getUtilisateur())
				sLang = pContexte->getUtilisateur()->donneLang() ;
			donne_date(pDocBmp->pDocInfo->pDonnees->creation, dateAffiche, sLang) ;			strcat(chAffiche, dateAffiche) ;
		}
    SetCaption(chAffiche) ;
  }
}

// Fonction CanClose : Appel de CanClose du document
////////////////////////////////////////////////////////////////
bool
NSBmpView::CanClose()
{
  return pDocBmp->CanClose();
}

bool
NSBmpView::VnCommit(bool /*force*/)
{
	return true;
}

bool
NSBmpView::VnRevert(bool /*clear*/)
{
	Invalidate();	// force un Paint
   return true;
}

//
// Build up a caption based on a filename, and set it into the title.
//
void
NSBmpView::SetCaption(const char* name)
{
  FileName = name ? name : "untitled";

  if (Parent) {
#if 0 // Old title format
    string caption = string(GetApplication()->GetName()) + " - " + FileName;
#else
    string caption = FileName + " - " + GetApplication()->GetName();
#endif
    Parent->SetCaption(caption.c_str());
  }
}

// Fonction de réponse à la commande Composer
////////////////////////////////////////////////////////////////
void
NSBmpView::CmComposer()
{
	pDocBmp->Composer();
}

// Fonction de réponse à la commande Imprimer (publier)
////////////////////////////////////////////////////////////////
void
NSBmpView::CmPublier()
{
	pDocBmp->Publier();
}

//
// Make a metafile & put it on the clipboard.
// Make a copy of each of the objects & place the copies on the clipboard
//
void
NSBmpView::CmCopy()
{
  TClipboard clipboard(*this);

  if (clipboard.EmptyClipboard()) {
    TMetaFileDC mfDC;
    mfDC.SetWindowExt(Dib->Size());
    mfDC.SetWindowOrg(0, 0);
    mfDC.SetDIBitsToDevice(TRect(0,0, PixelWidth, PixelHeight), TPoint(0,0), *Dib);
    TMetaFilePict mf(mfDC.Close(), AutoDelete);
    mf.ToClipboard(clipboard, MM_TEXT, TSize(PixelWidth, PixelHeight));

    if (Palette)
      TPalette(*Palette).ToClipboard(clipboard);
    TDib(*Dib).ToClipboard(clipboard);
    if (Bitmap)
      TBitmap(*Bitmap).ToClipboard(clipboard);
  }
}

void
NSBmpView::CeCopy(TCommandEnabler& ce)
{
  ce.Enable(Dib != 0);
}

//
// When a user selects edit.paste, get the data from the clipboard. This
// routine prefers CF_META over CF_DIB over CF_BITMAP
//
void
NSBmpView::CmPaste()
{
  TClipboard clipboard(*this);
  if (!clipboard)
    return;

  TPalette* newPal = 0;
  TDib*     newDib = 0;
  TBitmap*  newBitmap;

  // If there is a palette on the clipboard, get it & realize it
  //
  if (clipboard.IsClipboardFormatAvailable(CF_PALETTE)) {
    newPal = new TPalette(TPalette(clipboard));  // make a copy
    UpdatePalette(false);
  }

  // try DIB format 1st
  //
  if (clipboard.IsClipboardFormatAvailable(CF_DIB)) {
    newDib = new TDib(TDib(clipboard));        // make a copy
    newBitmap = new TBitmap(*newDib, newPal);  // newPal==0 is OK
  }
  // try metafile 2nd
  //
  else if (clipboard.IsClipboardFormatAvailable(CF_METAFILEPICT)) {
    if (!newPal)
      newPal = new TPalette((HPALETTE)GetStockObject(DEFAULT_PALETTE));
    newBitmap = new TBitmap(TMetaFilePict(clipboard), *newPal,
                            GetClientRect().Size());
  }
  // try bitmap (DDB) format 3rd
  //
  else if (clipboard.IsClipboardFormatAvailable(CF_BITMAP)) {
    newBitmap = new TBitmap(TBitmap(clipboard));     // make a copy
  }
  else
    return;  // Should never happen--this command is enabled by above tests

  // Got a bitmap & maybe more. Now keep it & setup things.
  //
  delete Bitmap;
  Bitmap = newBitmap;

  // Make a DIB from the bitmap if we didn't get one yet.
  //
  if (!newDib)
    newDib = new TDib(*newBitmap, newPal);
  delete Dib;
  Dib = newDib;

  // Either use the palette obtained above, or construct one from the
  // color table in the DIB
  //
  try {
    if (!newPal)
      newPal = new TPalette(*newDib);
  }
  catch (...) {
#if defined(BI_PLAT_WIN32)
    newPal = new TPalette(::CreateHalftonePalette(TScreenDC()), AutoDelete);
#else
    throw TXOwl("Cannot create palette from DIB");
#endif
//    newPal = new TPalette((HPALETTE)::GetStockObject(DEFAULT_PALETTE));
  }
  if (Palette != DefPalette)
    delete Palette;
  Palette = newPal;
  Palette->GetObject(Colors);

  PixelWidth  = Dib->Width();
  PixelHeight = Dib->Height();
  AdjustScroller();
  SetCaption("clipboard");

  // We'll paint the icon from the bitmap, or use the icon in the resource
  //
  if (PaintIcon)
    GetApplication()->GetMainWindow()->SetIcon(0, 0);
  else
    GetApplication()->GetMainWindow()->SetIcon(GetApplication(), GetApplication()->GetName());
}

//
//
//
void
NSBmpView::CePaste(TCommandEnabler& ce)
{
  TClipboard clipboard(*this);
  ce.Enable(
    clipboard && (
      clipboard.IsClipboardFormatAvailable(CF_METAFILEPICT) ||
      clipboard.IsClipboardFormatAvailable(CF_DIB) ||
      clipboard.IsClipboardFormatAvailable(CF_BITMAP)
    )
  );
}

//
// If either of the "Read bitmap" menu items is selected, then we read
// the bitmap resource with the ID of the menu item...
//
void
NSBmpView::CmRead1()
{
  if (LoadBitmapResource(CM_READ1))
    SetCaption("(Bitmap resource 1)");
}

//
//
//
void
NSBmpView::CmRead2()
{
  if (LoadBitmapResource(CM_READ2))
    SetCaption("(Bitmap resource 2)");
}

void
NSBmpView::CmReadBad()
{
  LoadBitmapFile("");   // force a failure for kicks
}

//
//
//
void
NSBmpView::CmMemHandle()
{
  char buf[32];
  sprintf(buf, "%0X", MemHandle);
  if (TInputDialog(this, "Memory Bitmap", "Enter handle", buf, sizeof buf).Execute() == IDOK) {
    HBITMAP handle;
    sscanf(buf, "%x", &handle);
    LoadMemHandle(handle);
  }
}

//
// Reload the bitmaps from the user supplied bitmap handle
//
void
NSBmpView::CmMemHandleRefresh()
{
  if (MemHandle)
    LoadMemHandle(MemHandle);
}

void
NSBmpView::CmFilePrint()          // Execute File:Print command
{
  if (pPrinter) {
    TWindowPrintout printout("BmpViewer Test", this);
    printout.SetBanding(true);
    pPrinter->Print(this, printout, true);
  }
}

//
// Toggle Fit member variable & adjust scroller as needed
//
void
NSBmpView::CmFit()
{
  Fit = !Fit;
  AdjustScroller();
}

//
// The fit menu item is checked if the Fit member is true
//
void
NSBmpView::CeFit(TCommandEnabler& ce)
{
  ce.SetCheck(Fit ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

//
// Toggle AutoAutoClipView member variable
//
void
NSBmpView::CmAutoClipView()
{
  AutoClipView = !AutoClipView;
}

//
// Check AutoClipView according to flag
//
void
NSBmpView::CeAutoClipView(TCommandEnabler& ce)
{
  ce.SetCheck(AutoClipView ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

//
// Toggle PaintFromDDB member variable
//
void
NSBmpView::CmPaintFromDDB()
{
  PaintFromDDB = !PaintFromDDB;
  Invalidate();
}

//
// Check AutoClipView according to flag
//
void
NSBmpView::CePaintFromDDB(TCommandEnabler& ce)
{
  ce.SetCheck(PaintFromDDB ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

//
// Toggle PaintIcon member variable
//
void
NSBmpView::CmPaintIcon()
{
  PaintIcon = !PaintIcon;
}

//
// Check AutoClipView according to flag
//
void
NSBmpView::CePaintIcon(TCommandEnabler& ce)
{
  ce.SetCheck(PaintIcon ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

//
// If the the "Open..." menu item is selected, then we prompt the user
// for a new bitmap file.  If the user selects one and it is one that
// we can read, we display it in the window and change the window's
// caption to reflect the new bitmap file.
//
void
NSBmpView::CmFileOpen()
{
  static TOpenSaveDialog::TData data (
    OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_NOREADONLYRETURN,
    "Bitmap Files (*.BMP)|*.bmp|",
    0,
    "",
    "BMP"
  );
  if (TFileOpenDialog(this, data).Execute() == IDOK) {
    char fileTitle[MAXPATH];
    TOpenSaveDialog::GetFileTitle(data.FileName, fileTitle, MAXPATH);
    LoadBitmapFile(fileTitle);
    SetCaption(strlwr(fileTitle));
  }
}

//
// Adjust the Scroller range so that the the origin is the
// upper-most scrollable point and the corner is the
// bottom-most.
//
void
NSBmpView::AdjustScroller()
{
  TRect  clientRect = GetClientRect();

  // Only show scrollbars when image is larger than
  // the client area and we are not stretching to fit.
  //
  if (Fit) {
    Scroller->SetRange(0, 0);
  }
  else {
    TPoint Range(Max(PixelWidth-clientRect.Width(), 0),
                 Max(PixelHeight-clientRect.Height(), 0));
    Scroller->SetRange(Range.x, Range.y);
  }
  Scroller->ScrollTo(0, 0);
  if (!GetUpdateRect(clientRect, false))
    Invalidate(false);
}

//
// Reset scroller range.
//
void
NSBmpView::EvSize(uint SizeType, TSize& Size)
{
  TWindow::EvSize(SizeType, Size);
  if (SizeType != SIZEICONIC) {
    AdjustScroller();
    Invalidate(false);
  }
}

//
// Somebody changed the palette. If its not us, then we need to update.
//
void
NSBmpView::EvPaletteChanged(HWND hWndPalChg)
{
  if (hWndPalChg != GetHandle())
    UpdatePalette(false);    // pass false to UpdateColors() instead of repaint
}

//
// We need to re-realize the logical palette each time we regain the input
// focus
//
bool
NSBmpView::EvQueryNewPalette()
{
  return UpdatePalette(true);
}

//
//
//
void
NSBmpView::EvDrawClipboard()
{
  if (TClipboardViewer::DoDrawClipboard() == esComplete)
    return;
  if (AutoClipView)
    CmPaste();
}

//
//
//
void
NSBmpView::EvDestroy()
{
  if (TClipboardViewer::DoDestroy() == esComplete)
    return;
  TWindow::EvDestroy();
}

//
//
//
bool
NSBmpView::UpdatePalette(bool alwaysRepaint)
{
  TRACEX(BmpView, 2, "Enter UpdatePalette()");
  if (ScreenPalettized && Palette) {
    TClientDC clientDC(*this);
#if defined(BI_PLAT_WIN16)
    Palette->UnrealizeObject();
#endif
    TRACEX(BmpView, 2, "UpdatePalette::Palette: " << hex << (uint)(HPALETTE)*Palette);
    clientDC.SelectObject(*Palette, true); 
    bool needsUpdate = clientDC.RealizePalette() > 0;
    if (alwaysRepaint || needsUpdate)
      Invalidate(false);
//    else if (needsUpdate)
//      clientDC.UpdateColors(); // can use this, but our painting is faster
    TRACEX(BmpView, 2, "Leave UpdatePalette(true)");
    return true;
  }
  TRACEX(BmpView, 2, "Leave UpdatePalette(false)");
  return false;
}

//
// Responds to an incoming Paint message by redrawing the bitmap.
// The Scroller's BeginView method, which sets the viewport origin
// relative to the present scroll position, has been called by TWindow's
// EvPaint
// Note that we Invalidate() ourselves with false to avoid the background
// paint flicker. Thats why we use
//
// The code can use either the Bitmap member with Stretch- and Bit- Blts,
// or use the Dib member with Stretch- and Set- DIBits...
//
void
NSBmpView::Paint(TDC& dc, bool, TRect&)
{
  TRACEX(BmpView, 2, "Enter Paint()");
  TRect clientRect = GetClientRect();

  TRACEX(BmpView, 2, "Paint::BkBrush: " << hex << (uint)(HBRUSH)*BkgndBrush);
  dc.SelectObject(*BkgndBrush);

  TMemoryDC memoryDC(dc);  // Get ready to possibly use the DDB

  if (PaintFromDDB && Bitmap) {
    TRACEX(BmpView, 2, "Paint::Bitmap: " << hex << (uint)(HBITMAP)*Bitmap);
    memoryDC.SelectObject(*Bitmap);
//    dc.SetStretchBltMode(STRETCH_HALFTONE);
    dc.SetStretchBltMode(STRETCH_ORSCANS);
    if (Palette) {
      TRACEX(BmpView, 2, "Paint::Palette: " << hex << (uint)(HPALETTE)*Palette);
      dc.SelectObject(*Palette, false);
      dc.RealizePalette();
      memoryDC.SelectObject(*Palette, false);
      memoryDC.RealizePalette();  
    }
    if (Rop == SRCCOPY) {
      if (Colors == 2) {
        if (Palette) {
          PALETTEENTRY pe;
          Palette->GetPaletteEntry(0, pe);
          dc.SetTextColor(TColor(pe));
          Palette->GetPaletteEntry(1, pe);
          dc.SetBkColor(TColor(pe));
        }
        else {
          dc.SetBkColor(TColor(0,0,0));
          dc.SetTextColor(TColor(255,255,255));
        }
      }
    }
  }

  if (PaintFromDDB && Bitmap || !PaintFromDDB && Dib) {
    TRect imageRect(0,0, PixelWidth, PixelHeight);
    if (Parent->IsIconic()) {
      if (PaintFromDDB)
        dc.StretchBlt(Parent->GetClientRect(), memoryDC, imageRect, Rop);
      else
        dc.StretchDIBits(Parent->GetClientRect(), imageRect, *Dib, Rop);
    }
    else {
      clientRect += TPoint((int)Scroller->XPos, (int)Scroller->YPos);
      if (Fit) {
        // Stretch to fit
        //
        if (PaintFromDDB)
          dc.StretchBlt(clientRect, memoryDC, imageRect, Rop);
        else
          dc.StretchDIBits(clientRect, imageRect, *Dib);
      }
      else {
        // Let it clip
        //
        if (PaintFromDDB)
          dc.BitBlt(imageRect, memoryDC, TPoint(0,0), Rop);
        else
          dc.SetDIBitsToDevice(imageRect, TPoint(0,0), *Dib);

        // Clear borders here for no flicker
        //
        dc.PatBlt(TRect(TPoint(PixelWidth,0), clientRect.BottomRight()));
        dc.PatBlt(TRect(TPoint(0,PixelHeight), clientRect.BottomRight()));
      }
    }
  }
  else
    // Nothing to paint
    //
    dc.PatBlt(clientRect, PATCOPY);

  TRACEX(BmpView, 2, "Leave Paint()" << endl);
}

//
// Setup all of the bitmap related members given a new dib
//
void
NSBmpView::SetupFromDib(TDib* dib)
{
  TRACEX(BmpView, 1, "Enter SetupFromDib()");
  TRACEX(BmpView, 1, "SetupFromDib::Dib: " << hex << (uint)(HANDLE)*dib);

  delete Dib;
  Dib = dib;

  if (Palette != DefPalette)
    delete Palette;
  Palette = 0;
  try {
    Palette = new TPalette(*dib);
  }
  catch (...) {
//    Palette = new TPalette(::CreateHalftonePalette(TScreenDC()), AutoDelete);
    Palette = DefPalette;
  }

  TRACEX(BmpView, 1, "SetupFromDib::Palette: " << hex << (uint)(HPALETTE)*Palette);

  {
    TWaitCursor w;
    delete Bitmap;
    Bitmap = new TBitmap(*Dib, Palette);
    TRACEX(BmpView, 1, "SetupFromDib::Bitmap: " << hex << (uint)(HBITMAP)*Bitmap);
  }

  PixelWidth  = Dib->Width();
  PixelHeight = Dib->Height();

  UpdatePalette(true);
  AdjustScroller();

  // We'll paint the icon from the bitmap, or use the icon in the resource
  //
  if (PaintIcon)
    GetApplication()->GetMainWindow()->SetIcon(0, 0);
  else
    GetApplication()->GetMainWindow()->SetIcon(GetApplication(), GetApplication()->GetName());

  TRACEX(BmpView, 1, "Leave SetupFromDib()");
}

// 
// Test if the passed resource is a Windows 3.0 (or PM 1.x) DI bitmap
// and if so read it.
// Report errors if unable to do so. Adjust the Scroller to the new
// bitmap dimensions.
//
bool
NSBmpView::LoadBitmapResource(uint16 resId)
{
  TDib* newDib;
  try {
    newDib = new TDib(*GetModule(), TResId(resId));
  }
  catch (TXGdi) {
    MessageBox("Cannot access bitmap resource", GetApplication()->GetName(),
               MB_OK);
    return false;
  }
  SetupFromDib(newDib);
  return true;
}

//
// Test if the passed file is a Windows 3.0 DI (or PM 1.x) bitmap
// and if so read it.
// Report errors if unable to do so. Adjust the Scroller to the new
// bitmap dimensions.
//
bool
NSBmpView::LoadBitmapFile(const char* name)
{
  TDib* newDib;
  try {
    newDib = new TDib(name);
  }
  catch (TXGdi) {
    MessageBox("Cannot open bitmap file", GetApplication()->GetName(), MB_OK);
    return false;
  }
  SetupFromDib(newDib);
  return true;
}

//
//
//
bool
NSBmpView::LoadMemHandle(HBITMAP handle)
{
#if defined(USE_DDB)
#if defined(BI_PLAT_WIN32)
  // Reqest size of info. buffer to validate HBITMAP
  //
  if (::GetObject(handle, 0, 0)) {
#else
  if (::IsGDIObject(handle)) {
#endif
    TDib* newDib;
    try {
      newDib = new TDib(TBitmap(handle));
      SetupFromDib(newDib);
      MemHandle = handle;

      char buf[20];
      sprintf(buf, "hbm:%0X", MemHandle);
      SetCaption(buf);
      return true;
    }
    catch (TXGdi) {
      MessageBox("Cannot construct Dib from HBITMAP",
                 GetApplication()->GetName(), MB_OK);
    }
  }
  else
    MessageBox("Not a valid bitmap handle",
                GetApplication()->GetName(), MB_OK);
#endif
  return false;
}

/* ------------------------ CLASSE APPLICATION INITIALE -----------------------

//
// class TBmpViewApp
// ~~~~~ ~~~~~~~~~~~
class _USERCLASS TBmpViewApp : public TApplication {
  public:
    TBmpViewApp(const char far* name) : TApplication(name) {}
    void InitMainWindow() {
      TFrameWindow* frame = new TFrameWindow(0, Name, new TBmpViewWindow);
      frame->AssignMenu(Name);
      frame->SetIcon(this, Name);
      frame->Attr.AccelTable = IDA_BMPVIEW;
      SetMainWindow(frame);
    }
};
*/

//----------------------------------------------------------------------------

//
//
//
// int
// OwlMain(int /*argc*/, char* /*argv*/ [])
// {
//  return TBmpViewApp(AppName).Run();
// }
