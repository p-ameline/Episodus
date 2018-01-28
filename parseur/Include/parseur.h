#ifndef __NAUTILUS_H
#define __NAUTILUS_H

#include <owl\mdi.h>
#include <owl\decmdifr.h>
#include <owl\controlb.h>
#include <owl\statusba.h>
#include <owl\printer.h>
#include <owl\docking.h>
#include <owl\controlg.h>
#include <owl\buttonga.h>

#include "docking\dockinge.h"
#include "partage\nsdivfct.h"


class NSPrinter : public OWL::TPrinter {
  public :
	 NSPrinter(OWL::TApplication* app = 0);
	 ~NSPrinter();

	 bool Print(TWindow* parent, TPrintout& printout, bool prompt);
};

// class NSMDIFrame : public TOleMDIFrame

class NSMDIFrame : public TDecoratedMDIFrame
{
  public :
   	NSPrinter* Printer;
    bool       bEndSession;

	NSMDIFrame(const char far *title, TResId menuResId,
							 TMDIClient &clientWnd = *new TMDIClient,
                             bool trackMenuSelection = false,
							 TModule* module = 0);
	~NSMDIFrame();

    void SetupWindow();
    bool EvQueryEndSession();
    void EvEndSession(bool endSession, bool logOff /* used only by Win95 */);

    DECLARE_RESPONSE_TABLE(NSMDIFrame);
};

// class TMyApp : public OWL::TApplication, public TOcModule

class TMyApp : public OWL::TApplication
{
	public:

	//
	// Frame, Barre de statut et barre de contrôle
	//
    NSMDIFrame* 	 		frame;
    OWL::TStatusBar* 		sb;
    OWL::TControlBar* 	 	cb;

    TGadget*		 		GadgetArray[20];
    int				 		nbGadget;

    // pour les barres dockables...
    THarborEx*     			Harbor;		/* TBorderHarbor* */
    TDockableControlBarEx* 	cb1;		/* TBorderDockable* */
    TDockableControlBarEx* 	cb2;		/* TBorderDockable* */
    TDockableControlBarEx* 	cb3;		/* TBorderDockable* */

	 TMyApp();
    ~TMyApp();

    TMDIClient* 		prendClient() { return Client; }
    void				InsertControlBar(TButtonGadget* pGadget);
    void				InsertControlBar(TControlGadget* pGadget);
    void				InsertControlBar(TSeparatorGadget* pGadget);
    bool				FlushControlBar();
    bool                FlushEpisodusControlBar();
    void				SetToolBarWindow(TWindow* pWindow)
    					{	pToolBarWindow = pWindow;	}
    TWindow*      		GetToolBarWindow()
    					{  return pToolBarWindow;	}
    bool                SetFirstInstance();
    bool                GetNumInstance(int& numInst);
    bool				CanClose();
    int 				MessageLoop();
    void                DetruireFichiersTmp();

    void                logMessage(string);

  	protected:

   	TWindow*    pToolBarWindow;

    // Override methods of TApplication
	void InitInstance();
	void InitMainWindow();
//    bool ProcessMsg(MSG& msg);

	// Event handlers
	void EvNewView  (TView& view);
	void EvCloseView(TView& view);
	void EvDropFiles(TDropInfo dropInfo);
    void Tempo(double s);
	void CmOpenLdV();
	void CmChemise();
    void CmImporter();
    void CmImportImg();
    void CmGenerePannel();
	void CmNewTtext();
    void CmNewTmpl();
    void CmMailBox();
    void CmOutils();
    void CmGroupGd();
    void CmRequete();
    void CmResult();
    void CmMateriels();
    void CmReconvoquer();
	void CmAbout();
    void CmHelp();
    void CmCreatPat();
    void CmCreatDoc();
    void CmModifDoc();
    void CmCreatCor();
    void CmModifCor();
    void CmBureau();
	void CmNewPat();
	void CmNewDoc();
	void CmNewCr();
    void CmNewMagic();
    void CmNewConsult(); //consultation

    // comptabilité
    void CmCarteVitale();
    void CmFicheCompt();
    void CmSituation();
    void CmAga();
    void CmImpayes();
    void CmListeActes();
    void CmSommeActes();
    void CmSommeEncaiss();
    void CmSaisieTP();
    void CmFeuilleSecu();
    void CmExit();

    // Episodus
    void    CmEpisoStart();
    void    CmEpisoStop();
    void    CmEpisoLearn();
    void    IdcEpisodPhare();


//    void    logMessage(string);
    LRESULT CmMouseHook(WPARAM wParam, LPARAM lParam);
    int  CM_MOUSE_HOOK;

    void    EvHotKey(int idHotKey);
    int     CaptureHotKey;

	TMDIClient* Client;
	DECLARE_RESPONSE_TABLE(TMyApp);
};

bool
TMyApp::FlushControlBar()
{
   	if (GetToolBarWindow() != 0)
   	{
   		// on enlève les boutons de la control bar
   		TGadget* pGadget = cb2->FirstGadget();
   		TGadget* pGadgetRemoved;

   		while (pGadget)
   		{
      		pGadgetRemoved = cb2->Remove(*pGadget);

   			if (!pGadgetRemoved)
      		{
      			erreur("Erreur à la destruction d'un bouton dans la barre d'outils",0,0);
         		return false;
      		}
      		else delete pGadgetRemoved;

      		pGadget = cb2->FirstGadget();
   		}

   		//cb2->Insert(*new TSeparatorGadget);
   		cb2->LayoutSession();
   		SetToolBarWindow(0);

   		// Harbor->Remove(*cb2);
   	}

   	return true;
}


bool
TMyApp::FlushEpisodusControlBar()
{
   	if (GetToolBarWindow() != 0)
   	{
   		// on enlève les boutons de la control bar
   		TGadget* pGadget = cb3->FirstGadget();
   		TGadget* pGadgetRemoved;

   		while (pGadget)
   		{
      		pGadgetRemoved = cb3->Remove(*pGadget);

   			if (!pGadgetRemoved)
      		{
      			erreur("Erreur à la destruction d'un bouton dans la barre d'outils",0,0);
         		return false;
      		}
      		else delete pGadgetRemoved;

      		pGadget = cb3->FirstGadget();
   		}

   		//cb2->Insert(*new TSeparatorGadget);
   		cb3->LayoutSession();
   		SetToolBarWindow(0);

   		// Harbor->Remove(*cb2);
   	}

   	return true;
}

#endif
// __NAUTILUS_H
