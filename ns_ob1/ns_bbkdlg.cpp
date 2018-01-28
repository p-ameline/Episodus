
#include <owl\dialog.h>

#include "nsbb\nsutidlg.h"
#include "nautilus\nssuper.h"

#include "ns_ob1\ns_bbk.h"
#include "ns_ob1\ns_bbkdlg.h"

#include "ns_ob1\nautilus-bbk.h"

#include "ns_ob1\ns_bbk.rh"

#include "nsbb\nstabwindow.h"
#ifdef _NSVIDAL
# include "ns_vidal\vidal_commun.h"
#endif


#include "nsbb\nstabwindow.h"


DEFINE_RESPONSE_TABLE1(NSBBKDlg, NSUtilDialog)
	EV_CHILD_NOTIFY(IDC_INITBBK,	BN_CLICKED, CmInitBBK),
	EV_CHILD_NOTIFY(IDC_GO1CYCLE,	BN_CLICKED, CmGoOneCycle),
	EV_CHILD_NOTIFY(IDC_GO,				BN_CLICKED, CmGo),
	EV_CHILD_NOTIFY(IDC_STOPBBK,	BN_CLICKED, CmFinishBBK),
	EV_CHILD_NOTIFY(IDC_DPIO,			BN_CLICKED,	CmDPIO),
	EV_CHILD_NOTIFY(IDC_VIDAL,		BN_CLICKED,	CmVidal),
	EV_COMMAND(IDOK,			CmOk),
	EV_COMMAND(IDCANCEL,	CmCancel),
	EV_COMMAND(IDHELP,		CmHelp),
END_RESPONSE_TABLE;


NSBBKDlg::NSBBKDlg(TWindow* parent , NSContexte* pCtx, TModule *module)
	: NSUtilDialog(parent, pCtx, "IDD_BBK", pNSResModule)
{
	pInit				= new TButton(this, IDC_INITBBK) ;
	pGoOneCycle = new TButton(this, IDC_GO1CYCLE) ;
	pGo 				= new TButton(this, IDC_GO) ;
	pFinish 		= new TButton(this, IDC_STOPBBK) ;
	pVidal			= new TButton(this, IDC_VIDAL) ;
}


NSBBKDlg::~NSBBKDlg()
{
}


void		NSBBKDlg::CmOk()
{
	TDialog::CmOk() ;
}


void		NSBBKDlg::CmCancel()
{
	TDialog::CmCancel() ;
}


void		NSBBKDlg::CmHelp()
{
}


void		NSBBKDlg::SetupWindow()
{
	TDialog::SetupWindow() ;
}


void		NSBBKDlg::CmInitBBK()
{
	reinitBlackboard(pContexte) ;
}


void		NSBBKDlg::CmGoOneCycle()
{
	goOneCycleBlackboard(pContexte) ;
}


void		NSBBKDlg::CmGo()
{
	goNCyclesBlackboard(pContexte) ;
}


void		NSBBKDlg::CmFinishBBK()
{
	//closeBlackboard(pContexte) ;
}


void		NSBBKDlg::CmDPIO()
{
   DPIO(pContexte) ;
  /***********************************************
  NSWindowsArray  *pWindowsArray = new NSWindowsArray() ;

  TWindow         *pWinToAdd1     = new TWindow(NULL, "title1") ;
  TButton *pButton  = new TButton(pWinToAdd1, 100, "test", 15, 15, 100, 100) ;
  pWindowsArray->push_back(pWinToAdd1) ;

  TWindow         *pWinToAdd2     = new TWindow(NULL, "title2") ;
  TButton *pButton2  = new TButton(pWinToAdd2, 100, "testons", 15, 15, 100, 100) ;
  pWindowsArray->push_back(pWinToAdd2) ;

  NSTabWindow     *pTabWindow = new NSTabWindow(pContexte->GetMainWindow(), "toto", pWindowsArray) ;

  pTabWindow->Execute() ;
  // we have not to delete TDialogs and the NSWindowsArray because NSTabWindow delete them
  //  delete pWinToAdd1 ;
  //  delete pWinToAdd2 ;
  //  delete pWindowsArray ;
  delete pTabWindow ;
  ****************************************************/
}


void  	NSBBKDlg::CmVidal()
{
#ifdef _NSVIDAL
	VidalMaster	*pVidalMaster = new VidalMaster(pContexte) ;
	pVidalMaster->doAll() ;
//	pVidalMaster->verifCodeLexiq() ;
	delete pVidalMaster ;
#endif
}

