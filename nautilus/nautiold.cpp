//----------------------------------------------------------------------------
//   Nautilus -- nautilus.cpp
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\decmdifr.h>
#include <owl\dialog.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\statusba.h>
#include <owl\docmanag.h>
#include <owl\docview.h>
#include <owl\olemdifr.h>
#include <owl\oleview.h>
#include <owl\oledoc.h>
#include <stdlib.h>
#include <string.h>
//#include <owl\vbxctl.h>
#include <idapi.h>
#include <time.h>
#include <owl\printer.h>

#define __MAIN
#define __NAUTILUS_CPP

#include "nautilus\nautilus.h"
#include "nautilus\nssuper.h"
#include "partage\nsglobal.h"
#include <owl\docview.rc>
#include <owl\mdi.rh>
// #include "nsrechdl.h"
#include "nautilus\nautilus.rh"
#include "nautilus\nsresour.h"
#include "nsbb\nsbb.h"
#include "nautilus\nscrvue.h"
#include "nautilus\nscsvue.h"
// #include "nsdocvue.h"

DEFINE_APP_DICTIONARY(AppDictionary);
static TPointer<TOcRegistrar> Registrar;
// TOcRegistrar* Registrar;
// Registration
//
REGISTRATION_FORMAT_BUFFER(200)

BEGIN_REGISTRATION(AppReg)
	REGDATA(clsid,   "{5E4BD340-8ABC-101B-A23B-CE4E85D07ED2}")
	REGDATA(appname, "Nautilus True Server")
END_REGISTRATION

DEFINE_RESPONSE_TABLE1(TMyApp, TApplication)
  EV_OWLVIEW(dnCreate, EvNewView),
  EV_OWLVIEW(dnClose,  EvCloseView),
  EV_WM_DROPFILES,
  EV_COMMAND(CM_NEWDOC,  CmNewDoc),
  //EV_COMMAND(CM_CONSULTATION,  CmNewConsultation),//consultation
  EV_COMMAND(CM_NEWPAT,  CmNewPat),
  EV_COMMAND(CM_ABOUT, 	 CmAbout),
  EV_COMMAND(CM_CHEMISE, CmChemise),
  EV_COMMAND(CM_NEWTTXT, CmNewTtext),
  EV_COMMAND(CM_NEWCR, 	 CmNewCr),
  EV_COMMAND(CM_NEWMAGIC,CmNewMagic),
  EV_COMMAND(CM_NEWTTABL, CmNewTmpl),
  EV_COMMAND(CM_EXIT, CmExit),
END_RESPONSE_TABLE;

TMyApp::TMyApp()
		 :TApplication(::AppReg["appname"], ::Module, &::AppDictionary)
{
	pNSSuperviseur->pNSApplication = this;
   SetCmdShow(SW_MAXIMIZE);
}

// destructeur
TMyApp::~TMyApp()
{
	if (pNSSuperviseur)
   {
		delete pNSSuperviseur;
      pNSSuperviseur = 0;
   }

   frame = 0;
   sb = 0;
   cb = 0;

   // if (!(prendClient())->CloseChildren())
   // 	return false;
}

void TMyApp::InitMainWindow()
{
	// Si on est appelé en temps que serveur OLE, on ne crée pas la
	// fenêtre principale
	// if (GetRegistrar().IsOptionSet(TOcCmdLine::Embedding))

	if (GetRegistrar().IsOptionSet(amEmbedding))
		nCmdShow = SW_HIDE;

	// Construction de la "Frame" NAUTILUS
	frame = new NSMDIFrame("NAUTILUS", 0, *(Client = new TMDIClient(this)),TRUE, this);
	frame->SetOcApp(OcApp);

	// Construction de la barre de statut
	sb = new TStatusBar(frame, TGadget::Recessed);

	// Construction de la barre de contrôle
	cb = new TControlBar(frame);
	cb->Insert(*new TButtonGadget(CM_NEWDOC, 		CM_NEWDOC, 	 	TButtonGadget::Command));

   //cb->Insert(*new TButtonGadget(CM_CONSULTATION, 	CM_CONSULTATION, 	 	TButtonGadget::Command));

	cb->Insert(*new TButtonGadget(CM_NEWPAT, 		CM_NEWPAT, 	 	TButtonGadget::Command));
	cb->Insert(*new TSeparatorGadget);
	cb->Insert(*new TButtonGadget(CM_CHEMISE, 	CM_CHEMISE,  	TButtonGadget::Command));
	cb->Insert(*new TSeparatorGadget);
	cb->Insert(*new TButtonGadget(CM_NEWTTXT, 	CM_NEWTTXT,  	TButtonGadget::Command));
	cb->Insert(*new TButtonGadget(CM_NEWTTABL, 	CM_NEWTTABL, 	TButtonGadget::Command));
   // Fonction d'appel des comptes-rendus pour le modele serveur OLE
	// cb->Insert(*new TButtonGadget(CM_NEWCR, 		CM_FILENEW, 	 	TButtonGadget::Command));
	cb->Insert(*new TButtonGadget(CM_NEWCR, 		CM_NEWCR, 	 	TButtonGadget::Command));
	cb->Insert(*new TButtonGadget(CM_NEWMAGIC, 	CM_NEWMAGIC, 	TButtonGadget::Command));
	cb->Insert(*new TSeparatorGadget);
   cb->Insert(*new TButtonGadget(CM_ENREGISTRE, CM_ENREGISTRE, TButtonGadget::Command));
   cb->Insert(*new TButtonGadget(CM_COMPOSE, 	CM_COMPOSE,  	TButtonGadget::Command));
   cb->Insert(*new TButtonGadget(CM_IMPRIME, 	CM_IMPRIME,  	TButtonGadget::Command));
	cb->Insert(*new TSeparatorGadget);
	cb->Insert(*new TButtonGadget(CM_ABOUT, 	CM_ABOUT, 	 TButtonGadget::Command));
	cb->SetHintMode(TGadgetWindow::EnterHints);

	// Définition de l'identifiant de la barre d'outils. Nécessaire à la fusion
	// OLE des barres
	cb->Attr.Id = IDW_TOOLBAR;

	// Insertion des barres (statut et contrôle) dans la frame
	frame->Insert(*sb, TDecoratedFrame::Bottom);
	frame->Insert(*cb, TDecoratedFrame::Top);

	// Définition de la fenêtre principale et de son menu
	SetMainWindow(frame);
   //TMenuDescr* pMenu = new TMenuDescr(IDM_MDICMNDS,1,0,0,0,1,1);
	//GetMainWindow()->SetMenuDescr(*pMenu);
   GetMainWindow()->SetMenuDescr(TMenuDescr(IDM_MDICMNDS));
   GetMainWindow()->Attr.Style |= WS_MAXIMIZE;

	// Mise en place du gestionnaire de documents
	// SetDocManager(new TDocManager(dmMDI | dmMenu | dmNoRevert, this));
   SetDocManager(new TDocManager(dmMDI, this));
}

void
TMyApp::InitInstance()
{
	TApplication::InitInstance();
   TApplication::EnableBWCC();
	GetMainWindow()->DragAcceptFiles(TRUE);
  	if (pNSSuperviseur->pUtilisateur == 0)
		pNSSuperviseur->AppelUtilisateur();
	if (pNSSuperviseur->pUtilisateur == 0)
   {
   	GetMainWindow()->CloseWindow();
      if (!GetMainWindow()->GetHandle())
      	Condemn(GetMainWindow());
   }
}

void
TMyApp::EvDropFiles(TDropInfo dropInfo)
{
	int fileCount = dropInfo.DragQueryFileCount();
	for (int index = 0; index < fileCount; index++) {
		int fileLength = dropInfo.DragQueryFileNameLen(index)+1;
		char* filePath = new char [fileLength];
		dropInfo.DragQueryFile(index, filePath, fileLength);
		TDocTemplate* tpl = GetDocManager()->MatchTemplate(filePath);
		if (tpl)
			tpl->CreateDoc(filePath);
		delete filePath;
      filePath = 0 ;
	}
	dropInfo.DragFinish();
}

void
TMyApp::EvNewView(TView& view)
{
	char nomVue[80];
   TRect zone;

	// Dans la version serveur OLE, il faut déterminer quelle est la
	// fenêtre parent (différent lorsqu'il est utilisé in situ - serveur
	// dans son conteneur - ou appelé d'une autre application)
	//
	TOleView* ov = TYPESAFE_DOWNCAST(&view, TOleView);
	if (view.GetDocument().IsEmbedded() && ov && !ov->IsOpenEditing())
	{
		TWindow* vw = view.GetWindow();
		vw->SetParent(TYPESAFE_DOWNCAST(GetMainWindow(), TOleFrame)
																		->GetRemViewBucket());
		vw->Create();
	}
	else
	{
      	TMDIChild* child = new TMDIChild(*Client, 0);

      	if (view.GetViewMenu())
      	{
      		child->SetMenuDescr(*view.GetViewMenu());
      	}

         // on récupère le nom de la vue à créer
         strcpy(nomVue, view.GetViewName());

      	child->Create();
      	child->SetClientWindow(view.GetWindow());
         child->GetClientRect(zone);
	}
   if (!ov || !ov->GetOcRemView())
		OcApp->SetOption(amEmbedding, false);


	// Code de la version non serveur OLE
	//
	//TMDIChild* child = new TVbxMDIChild(*Client, view.GetDocument().GetTitle(),
	//																			view.GetWindow());
	//if (view.GetViewMenu())
	//	child->SetMenuDescr(*view.GetViewMenu());
	//child->Create();
}

void
TMyApp::EvCloseView(TView& /*view*/)
{
	// Rustine : On rétablit pour l'instant le menu de la fenetre mere
	GetMainWindow()->SetMenuDescr(TMenuDescr(IDM_MDICMNDS));
}

void TMyApp::Tempo(double s)
{
	time_t time1,time2;

   time(&time1);
   do time(&time2);
   while (difftime(time2,time1) < s);
}

void TMyApp::CmAbout()
{
	TDialog(&TWindow(GetMainWindow()->GetCommandTarget()),
   																	IDD_NS_ABOUT).Execute();
}

void TMyApp::CmNewDoc()
{
	if (pNSSuperviseur->pUtilisateur != 0)
	{
		if (pNSSuperviseur->pUtilisateur->pPatient != 0)
      {
			delete pNSSuperviseur->pUtilisateur->pPatient;
         pNSSuperviseur->pUtilisateur->pPatient = 0 ;
      }
		delete pNSSuperviseur->pUtilisateur;
      pNSSuperviseur->pUtilisateur = 0;
	}
	pNSSuperviseur->AppelUtilisateur();
	if (pNSSuperviseur->pUtilisateur == 0)
		GetMainWindow()->CloseWindow();
}

void TMyApp::CmNewPat()
{
	if (pNSSuperviseur->pUtilisateur != 0)
		pNSSuperviseur->pUtilisateur->AppelPatient();
}

void TMyApp::CmChemise()
{
	if ((pNSSuperviseur->pUtilisateur == 0) ||
											(pNSSuperviseur->pUtilisateur->pPatient == 0))
		return;
	pNSSuperviseur->pUtilisateur->pPatient->CmChemise();
}

void TMyApp::CmNewTtext()
{
	if ((pNSSuperviseur->pUtilisateur == 0) ||
											(pNSSuperviseur->pUtilisateur->pPatient == 0))
		return;
	pNSSuperviseur->pUtilisateur->pPatient->CmNewTTxt();
}

void TMyApp::CmNewTmpl()
{
	if ((pNSSuperviseur->pUtilisateur == 0) ||
											(pNSSuperviseur->pUtilisateur->pPatient == 0))
		return;
	pNSSuperviseur->pUtilisateur->pPatient->CmNewTmpl();
}

void TMyApp::CmNewCr()
{
	if ((pNSSuperviseur->pUtilisateur == 0) ||
											(pNSSuperviseur->pUtilisateur->pPatient == 0))
		return;
	pNSSuperviseur->pUtilisateur->pPatient->CmNewCr();
}


void TMyApp::CmNewConsultation()
{
/*	if ((pNSSuperviseur->pUtilisateur == 0) ||
											(pNSSuperviseur->pUtilisateur->pPatient == 0))
		return;
	pNSSuperviseur->pUtilisateur->pPatient->CmNewConsultation();
*/
}


void TMyApp::CmNewMagic()
{
	if ((pNSSuperviseur->pUtilisateur == 0) ||
											(pNSSuperviseur->pUtilisateur->pPatient == 0))
		return;
	pNSSuperviseur->pUtilisateur->pPatient->CmNewMagic();
}

/*void TMyApp::CmNewTtabl()
{
  pNSSuperviseur->pUtilisateur->pPatient->CreeTtabl();
} */


void TMyApp::CmExit()
{
   if (pNSSuperviseur)
   {
		delete pNSSuperviseur;
      pNSSuperviseur = 0;
   }

   prendClient()->CloseChildren();

   GetMainWindow()->CloseWindow();
}

/***************************************************************************/

int OwlMain(int /*argc*/, char* /*argv*/ [])
{
	// Initialisation nécessaire pour utiliser les contrôles VBX
	//TBIVbxLibrary vbxLib;
	//pNSSuperviseur->pVBXLib = &vbxLib;
   pNSSuperviseur->InitialiseDatabase();

	//pNSSuperviseur->pBigBoss = new NSSmallBrother(/*pNSSuperviseur*/);
	//
	// Fonctionnement avant OLE
	//pNSSuperviseur->pNSApplication = new TMyApp;
	//return pNSSuperviseur->pNSApplication->Run();

	try
	{
    	int retVal;
//      TPointer<TOcRegistrar> Registrar;
    	TOleInit initOle;

		Registrar = new TOcRegistrar(AppReg, TOleDocViewFactory<TMyApp>(),
							TApplication::GetCmdLine(), ::DocTemplateStaticHead);

		if (Registrar->IsOptionSet(amAnyRegOption))
			return 0;

		// If this is an exe server normal run, run the app now. Otherwise, wait
		// until our factory gets a call (don't call us, we'll call you)
		//

		return Registrar->Run();
	}
	catch (xmsg& x)
	{
		::MessageBox(0, x.why().c_str(), "Exception", MB_OK);
	}
	return -1;
}

NSMDIFrame::NSMDIFrame(const char far *title, TResId menuResId,
											TMDIClient &clientWnd,
											bool trackMenuSelection, TModule* module)
			  :TOleMDIFrame(title, menuResId, clientWnd,
											trackMenuSelection, module)
{
	Printer = new NSPrinter;
}

NSMDIFrame::~NSMDIFrame()
{
	delete Printer;
   Printer = 0;
}

NSPrinter::NSPrinter(TApplication* app)
{
  Data = new TPrintDialog::TData;
  Error = 0;
  Application = app ? app : ::GetApplicationObject();
  CHECK(Application);
  GetDefaultPrinter();
}

NSPrinter::~NSPrinter()
{
  delete Data;
  Data = 0;
}

#define SETCLEAR(flag, set, clear)((flag) = ((flag)&~(clear))|(set))
#define WM_SETNUMBER    WM_USER+100

//
// Abort procedure used during printing, called by windows. Returns true to
// continue the print job, false to cancel.
//
int CALLBACK __export
TPrinterAbortProc(HDC hDC, int code)
{
  GetApplicationObject()->PumpWaitingMessages();

  // UserAbortDC will have been set by the AbortDialog
  //
  if (TPrinter::GetUserAbort() == hDC || TPrinter::GetUserAbort() == HDC(-1)) {
    TPrinter::SetUserAbort(0);
    return false;
  }
  return code == 0 || code == SP_OUTOFDISK;
}

bool
NSPrinter::Print(TWindow* parent, TPrintout& printout, bool prompt)
{
  PRECONDITION(parent);

  // Start from scratch
  //
  Error = 1;           // Positive 'Error' means print job is OK
  TPrintDC*   prnDC;
  HCURSOR hOrigCursor = ::SetCursor(::LoadCursor(0, IDC_WAIT));

  //
  // Get page range & selection range (if any) from document
  //
  int selFromPage;
  int selToPage;
  printout.GetDialogInfo(Data->MinPage, Data->MaxPage, selFromPage, selToPage);
  if (selFromPage)
    Data->Flags &= ~PD_NOSELECTION;
  else
    Data->Flags |= PD_NOSELECTION;
  if (Data->MinPage) {
    Data->Flags &= ~PD_NOPAGENUMS;
    if (Data->FromPage < Data->MinPage)
      Data->FromPage = Data->MinPage;
    else if (Data->FromPage > Data->MaxPage)
		Data->FromPage = Data->MaxPage;
    if (Data->ToPage < Data->MinPage)
      Data->ToPage = Data->MinPage;
    else if (Data->ToPage > Data->MaxPage)
      Data->ToPage = Data->MaxPage;
  }
  else
    Data->Flags |= PD_NOPAGENUMS;

  //
  // Create & execute a TPrintDialog (or derived) and have it return a usable
  // DC if prompt is enabled. If the dialog fails because the default printer
  // changed, clear our device information & try again.
  //
  if (prompt) {
	 SETCLEAR(Data->Flags, PD_RETURNDC, PD_RETURNDEFAULT|PD_PRINTSETUP);
    bool ok = ExecPrintDialog(parent);
    if (!ok && Data->Error == PDERR_DEFAULTDIFFERENT) {
		ClearDevice();
      ok = ExecPrintDialog(parent);
	 }
    if (!ok)
      return false;

    prnDC = Data->TransferDC();   // We now own the DC, let prnDC manage it
  }
  // Construct the DC directly if prompting was not enabled.
  //
  else {
    prnDC = new TPrintDC(Data->GetDriverName(), Data->GetDeviceName(),
                         Data->GetOutputName(), Data->GetDevMode());
  }
  if (!prnDC)
    THROW( TXPrinter() );

  //
  // Get the page size
  //
  PageSize.cx = prnDC->GetDeviceCaps(HORZRES);
  PageSize.cy = prnDC->GetDeviceCaps(VERTRES);
  printout.SetPrintParams(prnDC, PageSize);

  //
  // Create modeless abort dialog using strings
  //
  TWindow* abortWin;
  TRY {
	 abortWin = CreateAbortWindow(parent, printout);
  }
  CATCH( (...) {
    delete prnDC;
    prnDC = 0;
    RETHROW;
  })

  ::SetCursor(hOrigCursor);
  parent->EnableWindow(false);

  prnDC->SetAbortProc(TPrinterAbortProc);

  //
  // Only band if the user requests banding and the printer
  // supports banding
  //
  bool banding = printout.WantBanding() &&
                (prnDC->GetDeviceCaps(RASTERCAPS) & RC_BANDING);
  if (banding)
    //
    // Only use BANDINFO if supported.
    //
    UseBandInfo = ToBool(prnDC->QueryEscSupport(BANDINFO));

  else
    //
    // Set the banding rectangle to full page
    //
    BandRect.Set(0, 0, PageSize.cx, PageSize.cy);

  //
  // If more than one (uncollated) copy was requested, see if printer can
  // handle it for performance and user convenience.
  //
  int copiesPerPass = 1;
  if (!(Data->Flags & PD_COLLATE))
    prnDC->SetCopyCount(Data->Copies, copiesPerPass);

  //
  // Figure out which page range to use: Selection, Dialog's from/to,
  // whole doc range or all possible pages.
  //
  int fromPage;
  int toPage;
  if (prompt && (Data->Flags & PD_SELECTION) || selFromPage) {
    fromPage = selFromPage;
    toPage = selToPage;
  }
  else if (prompt && (Data->Flags & PD_PAGENUMS)) {
    fromPage = Data->FromPage;
    toPage = Data->ToPage;
  }
  else if (Data->MinPage) {
    fromPage = Data->MinPage;
    toPage = Data->MaxPage;
  }
  else {
    fromPage = 1;
	 toPage = INT_MAX;
  }

  //
  // Copies loop, one pass per block of document copies.
  //
  printout.BeginPrinting();
  for (int copies = Data->Copies;
       copies > 0 && Error > 0;
       copies -= copiesPerPass) {

    //
    // On last multi-copy pass, may need to adjust copy count
    //
    if (copiesPerPass > 1 && copies < copiesPerPass)
      prnDC->SetCopyCount(copies, copiesPerPass);

    //
    // Whole document loop, one pass per page
    //
    Flags = pfBoth;
    Error = prnDC->StartDoc(printout.GetTitle(), 0);  // get PD_PRINTTOFILE ?
	 printout.BeginDocument(fromPage, toPage, Flags);

    for (int pageNum = fromPage;
         Error > 0 && pageNum <= toPage && printout.HasPage(pageNum);
         pageNum++) {

		abortWin->SendDlgItemMessage(ID_PAGE, WM_SETNUMBER, pageNum);

      //
      // Begin the page by getting the first band or calling StartPage()
      //
      if (banding) {
        FirstBand = true;
        Error = prnDC->NextBand(BandRect);
      }
      else
        Error = prnDC->StartPage();

      //
      // Whole page loop, one pass per band (once when not banding)
      //
      while (Error > 0 && !BandRect.IsEmpty()) {
        //
        // [Manually call the abort proc between bands or pages]
        //
#if defined(MANUAL_ABORT_CALL)
        prnDC->QueryAbort();
#endif

        if (banding) {
          CalcBandingFlags(*prnDC);
          if (printout.WantForceAllBands() && (Flags & pfBoth) == pfGraphics)
            prnDC->SetPixel(TPoint(0, 0), 0);  // Some old drivers need this 
          prnDC->DPtoLP(BandRect, 2);
        }

		  printout.PrintPage(pageNum, BandRect, Flags);
        if (banding)
          Error = prnDC->NextBand(BandRect);
        else
          break;
      }

      //
      // EndPage (NEWFRAME) need only called if not banding
      //
      if (Error > 0 && !banding) {
        Error = prnDC->EndPage();
        if (Error == 0)    // a zero return here is OK for this call
          Error = 1;
      }

    }  // End of Whole Document-loop

    // Tell GDI the document is finished
    //
    if (Error > 0)
#if defined(MANUAL_ABORT_CALL)
      if (banding && (UserAbortDC == *prnDC || UserAbortDC == HDC(-1))
        prnDC->AbortDoc();
      else
        prnDC->EndDoc();
#else
      prnDC->EndDoc();
#endif

    printout.EndDocument();

  } // End of Copy-loop
  printout.EndPrinting();

  // Set the printer back to 1 copy
  //
  if (copiesPerPass > 1)
    prnDC->SetCopyCount(1, copiesPerPass);

  //
  // Re-enable parent and free allocated resources
  //
  parent->EnableWindow(true);
  abortWin->Destroy();
  delete abortWin;
  abortWin =0;
  //if (UserAbortDC == *prnDC)  // User tried to abort, but it was too late
  //  UserAbortDC = 0;
  delete prnDC;
  prnDC = 0;

  //
  // Report error if not already done so by printmgr
  //
  if (Error & SP_NOTREPORTED)
    ReportError(parent, printout);

  return Error > 0;
}

