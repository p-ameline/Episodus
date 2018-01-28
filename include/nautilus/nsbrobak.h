// NSBROWSE.H : Définition des objets dialogues contenant ie4
/////////////////////////////////////////////////////////////

#if !defined(__NSBROWSE_H)
#define __NSBROWSE_H

// nsie4 sert uniquement pour la classe TBrowserControl
// #include "nsdn\nsie4.h"

#include <owl\edit.h>
#include <owl\controlg.h>
#include "nautilus\msnet.h"
// #include "c:\progra~1\borland\cbuild~1\lib\shdocvw_tlb.h"

//---------------------------------------------------------------------------
//
// Classes controle browser :
//
//---------------------------------------------------------------------------

class NSRefDocument;

class TDIECompo : public TOleDialog
{
public:

   string 				fichCompo;
   TOcControl* 			Ocx;
   TIWebBrowser2Proxy 	Control;

   NSRefDocument* 		pDocBrut;

	TDIECompo(TWindow* parent, NSRefDocument* pDocMaitre, TModule* module = 0);
	~TDIECompo();

   void SetupWindow();
	bool Message(const char far *texte);
   void Navigate(string url, bool kill = false);
   void CmOk();
   void CmCancel();

   DECLARE_RESPONSE_TABLE(TDIECompo);
};

class NSPublication;

class TDIEPubli : public TOleDialog
{
public:

	NSPublication* 		pPubli;
   TOcControl* 			Ocx;
	TIWebBrowser2Proxy 	Control;

	TDIEPubli(TWindow* parent, NSPublication* pPub, TModule* module = 0);
	~TDIEPubli();

   void SetupWindow();
	bool Message(const char far *texte);
   void Print();
   void Navigate(string url);
   bool IsBusy();
   bool EvOcCtrlCustomEvent(TCtrlCustomEvent* pev);

   //StatusTextChange: Statusbar text changed.  [dispid 102]
	void StatusTextChange(TAutoString Text);

	//ProgressChange: Fired when download progress is updated.  [dispid 108]
	void ProgressChange(long Progress, long ProgressMax);

	//CommandStateChange: The enabled state of a command changed.  [dispid 105]
	void CommandStateChange(long Command, TBool Enable);

	//DownloadBegin: Download of a page started.  [dispid 106]
	void DownloadBegin();

	//DownloadComplete: Download of page complete.  [dispid 104]
	void DownloadComplete();

	//TitleChange: Document title changed.  [dispid 113]
	void TitleChange(TAutoString Text);

	//PropertyChange: Fired when the PutProperty method has been called.  [dispid 112]
	void PropertyChange(TAutoString szProperty);

	//BeforeNavigate2: Fired before navigate occurs in the given WebBrowser (window or frameset element). The processing of this navigation may be modified.  [dispid 250]
	void BeforeNavigate2(IDispatch* pDisp, TAutoVal URL, TAutoVal Flags, TAutoVal TargetFrameName, TAutoVal PostData, TAutoVal Headers, TBool* Cancel);

	//NewWindow2: A new, hidden, non-navigated WebBrowser window is needed.  [dispid 251]
	void NewWindow2(IDispatch** ppDisp, TBool* Cancel);

	//NavigateComplete2: Fired when the document being navigated to becomes visible and enters the navigation stack.  [dispid 252]
	void NavigateComplete2(IDispatch* pDisp, TAutoVal URL);

	//DocumentComplete: Fired when the document being navigated to reaches ReadyState_Complete.  [dispid 259]
	void DocumentComplete(IDispatch* pDisp, TAutoVal URL);

	//OnQuit: Fired when application is quiting.  [dispid 253]
	void OnQuit();

	//OnVisible: Fired when the window should be shown/hidden  [dispid 254]
	void OnVisible(TBool Visible);

	//OnToolBar: Fired when the toolbar  should be shown/hidden  [dispid 255]
	void OnToolBar(TBool ToolBar);

	//OnMenuBar: Fired when the menubar should be shown/hidden  [dispid 256]
	void OnMenuBar(TBool MenuBar);

	//OnStatusBar: Fired when the statusbar should be shown/hidden  [dispid 257]
	void OnStatusBar(TBool StatusBar);

	//OnFullScreen: Fired when fullscreen mode should be on/off  [dispid 258]
	void OnFullScreen(TBool FullScreen);

	//OnTheaterMode: Fired when theater mode should be on/off  [dispid 260]
	void OnTheaterMode(TBool TheaterMode);

   void CmOk();
   void CmCancel();

   DECLARE_RESPONSE_TABLE(TDIEPubli);
};

#endif

// fin de nsbrowse.h
///////////////////////////////////////////////////////////////
