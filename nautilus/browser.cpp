//----------------------------------------------------------------------------
//
// TBrowserControl: encapsulates the Browser control by deriving from both
// TOleWindow and the Automation proxy class.
//
//----------------------------------------------------------------------------
#include <owl/pch.h>
#pragma hrdstop

#include <owl/docview.rh>
#include <owl/editfile.rh>
#include <owl/controlb.h>
#include <owl/buttonga.h>

#include <cstring.h>
#include <stdio.h>

#include "mshtmhst.h"

#include "browser.h"
#include "browsapp.h"
#include "dhtml.h"

#define CM_ONCLICKED 500

//---------------------------------------------------------------------------
//
//Helper classes for toolbar edit box:
//
//---------------------------------------------------------------------------

void  TEditGadgetEnabler::SetText(const char far* txt)
{
   char oldtxt[256];
	TEdit* edit = dynamic_cast<TEdit*>(Gadget->GetControl());
   edit->GetText(oldtxt, 256);
   if(strcmp(txt, oldtxt) != 0)
   	edit->SetText(txt);
}


void TEditGadget::CommandEnable()
{
	if (Window->GetHandle() && (GetFocus() != GetControl()->HWindow))
		Window->HandleMessage(WM_COMMAND_ENABLE, 0, TParam2(&TEditGadgetEnabler(*Window, this)));
}


DEFINE_RESPONSE_TABLE1(TNotifyEdit, TEdit)
	EV_WM_KEYDOWN,
END_RESPONSE_TABLE;


void TNotifyEdit::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if(key == VK_RETURN)
   {
      text = new char[GetTextLen() + 1];
      GetText(text, GetTextLen() + 1);
   	Parent->HandleMessage(WM_COMMAND, GetId(), 0);
   }
   else
   	TEdit::EvKeyDown(key, repeatCount, flags);
}

TAPointer<char> TNotifyEdit::text = 0;

const char* TNotifyEdit::Text()
{
	return (const char*)text;
}



//----------------------------------------------------------------------------


DEFINE_RESPONSE_TABLE1(TBrowserControl, TOleWindow)
  EV_OC_VIEWSETSITERECT,
  EV_WM_SIZE,
  EV_COMMAND(ID_FORMULA, FormulaChange),
  EV_COMMAND_ENABLE(ID_FORMULA, CeFormula),
  EV_OC_VIEWGETSITERECT,
  EV_COMMAND(CM_FILEPRINT, CmPrint),
  EV_COMMAND(CM_FILEPRINTERSETUP, CmPrintSetup),
  EV_COMMAND_ENABLE(CM_EDITCOPY, CeEditCopy),
  EV_COMMAND(CM_EDITCOPY, CmEditCopy),
  EV_COMMAND(CM_ONCLICKED, CmOnButtonClicked),
END_RESPONSE_TABLE;

DEFINE_AUTOCLASS(TBrowserControl)
  EXPOSE_METHOD(bnclicked, TAutoVoid, "OnButtonClicked", "Button Clicked Notification", 0)
  EXPOSE_APPLICATION(TBrowserApp, "Application", "Application object", 0)
END_AUTOCLASS(TBrowserControl, tfCanCreate, "BrowserControl", "BrowserControl Document Object", 0)


TBrowserControl::TBrowserControl(TWindow* parent, const char far* fileName, TModule* module)
: TOleWindow(parent, module)
{
	if (fileName)
		strcpy(FileData.FileName, fileName);
	else
		strcpy(FileData.FileName, "");
	FileData.Flags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
   FileData.SetFilter(string(*GetModule(), IDS_DOCFILEFILTER).c_str());

   count = 1;
}

TBrowserControl::~TBrowserControl()
{
}

//
// Perform normal SetupWindow, plus create ocx object and bind to its IDispatch
//
void TBrowserControl::SetupWindow()
{
	TOleWindow::SetupWindow();
   GUID id = IID_CoWebBrowser;
   ocx = InsertControl(id);                 // insert web browser
   Bind(ocx->GetCtlDispatch());             // bind to its IDispatch
   if(*FileData.FileName)
   	OpenURL(FileData.FileName);
   else                                     // open our default URL
   {
      char buf[256];
      char file[256];
      GetApplication()->GetModuleFileName(file, 256);
      sprintf(buf, "res://%s/SAMPLE", file);
      OpenURL(buf);
   }
   init = false;
   pthisDisp = GetDispatchOfObject(*this, TObjectDescriptor::Quiet);
}

//prevent the control from changing its size:
bool TBrowserControl::EvOcViewSetSiteRect(TRect far* rect)
{
  TOleWindow::EvOcViewSetSiteRect(rect);
  return false;
}

// keep the control at full window size
void TBrowserControl::EvSize(uint sizeType, TSize& size)
{
   TOleWindow::EvSize(sizeType, size);
   TRect r = GetClientRect();
	ocx->SetLeft(0);
   ocx->SetTop(0);
   ocx->SetWidth(r.Width());
   ocx->SetHeight(r.Height());
}

// navigate to the URL:
void TBrowserControl::FormulaChange()
{
	const char* text = TNotifyEdit::Text();
   OpenURL(text);
}

// get our URL:
void TBrowserControl::CeFormula(TCommandEnabler& ce)
{
   string s(LocationURL());
	ce.SetText(s.c_str());
}

// helper function to
// make it easier to navigate to a URL:
void TBrowserControl::OpenURL(const char* URL, long flags, const char* frame, SafeArray<unsigned char> s)
{
   TAutoVal args[3];
   push_arg(args[0], flags);
   push_arg(args[1], TAutoString(frame));
   push_arg(args[2], static_cast<SafeArrayBase&>(s));
   Navigate(URL, args, args+1, args+2, 0);
}

//
// IOleCommandTarget functions:
//
void TBrowserControl::CmPrint()
{
   IDispatch* pdisp = Document();
   IOleCommandTarget* command;
   pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command);
   if(command)
   {
      command->Exec(NULL, OLECMDID_PRINT, OLECMDEXECOPT_PROMPTUSER, NULL, NULL);
      command->Release();
   }
   pdisp->Release();
}

void TBrowserControl::CmPrintSetup()
{
   IDispatch* pdisp = Document();
   IOleCommandTarget* command;
   pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command);
   if(command)
   {
      command->Exec(NULL, OLECMDID_PAGESETUP, OLECMDEXECOPT_PROMPTUSER, NULL, NULL);
      command->Release();
   }
   pdisp->Release();
}

void TBrowserControl::CeEditCopy(TCommandEnabler& tce)
{
   if(IsBound())
   {
      IDispatch* pdisp = Document();
      if(pdisp == 0)
      {
         tce.Enable(false);
         return;
      }
      IOleCommandTarget* command;
      pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command);
      if(command)
      {
         OLECMD id;
         id.cmdID = OLECMDID_COPY;
         command->QueryStatus(NULL, 1, &id, NULL);
         tce.Enable(id.cmdf & OLECMDF_ENABLED);
         command->Release();
      }
      pdisp->Release();
   }
   else
      tce.Enable(false);
}

void TBrowserControl::CmEditCopy()
{
   IDispatch* pdisp = Document();
   IOleCommandTarget* command;
   pdisp->QueryInterface(IID_IOleCommandTarget, (void**)&command);
   if(command)
   {
      command->Exec(NULL, OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
      command->Release();
   }
   pdisp->Release();
}

//
// IDocHostUIHandler implimentation:
//
// in this case we use IDocHostUIHandler to provide our IDispatch
// interface to the Browser, allows HTML scripts to access the containing
// application via the "window.external" property.
//
HRESULT STDMETHODCALLTYPE TBrowserControl::QueryInterface(REFIID riid, void** ppvObj)
{
   if(riid == IID_IUnknown)
   {
      *ppvObj = static_cast<IUnknown*>(this);
      return S_OK;
   }
   if(riid == IID_IDocHostUIHandler)
   {
      *ppvObj = static_cast<IDocHostUIHandler*>(this);
      return S_OK;
   }
   *ppvObj = 0;
   return E_NOTIMPL;
}

unsigned long STDMETHODCALLTYPE TBrowserControl::AddRef()
{
   return ++count;
}

unsigned long STDMETHODCALLTYPE TBrowserControl::Release()
{
   return --count;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::ShowContextMenu(DWORD dwID, POINT __RPC_FAR *ppt,
                                 IUnknown __RPC_FAR *pcmdtReserved, IDispatch __RPC_FAR *pdispReserved)
{
   return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::GetHostInfo(DOCHOSTUIINFO __RPC_FAR *pInfo)
{
   pInfo->dwFlags = 0;
   pInfo->dwDoubleClick = 0;
   return S_OK;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::ShowUI(DWORD dwID, IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
                        IOleCommandTarget __RPC_FAR *pCommandTarget, IOleInPlaceFrame __RPC_FAR *pFrame,
                        IOleInPlaceUIWindow __RPC_FAR *pDoc)
{
   return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::HideUI()
{
   return S_OK;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::UpdateUI()
{
   return S_OK;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::EnableModeless(BOOL fEnable)
{
   return S_OK;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::OnDocWindowActivate(BOOL fActivate)
{
   return S_OK;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::OnFrameWindowActivate(BOOL fActivate)
{
   return S_OK;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::ResizeBorder(LPCRECT prcBorder,
               IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow)
{
   return S_OK;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::TranslateAccelerator(LPMSG lpMsg,
            const GUID __RPC_FAR *pguidCmdGroup, DWORD nCmdID)
{
   return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::GetOptionKeyPath(LPOLESTR __RPC_FAR *pchKey, DWORD dw)
{
   *pchKey = 0;
   return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::GetDropTarget(IDropTarget __RPC_FAR *pDropTarget,
                           IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
{
   *ppDropTarget = 0;
   return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::GetExternal(IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
   *ppDispatch = pthisDisp;
   pthisDisp->AddRef();
   return S_OK;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::TranslateUrl(DWORD dwTranslate, OLECHAR __RPC_FAR *pchURLIn,
                        OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
{
   *ppchURLOut = 0;
   return S_FALSE;
}

HRESULT STDMETHODCALLTYPE TBrowserControl::FilterDataObject(IDataObject __RPC_FAR *pDO,
                        IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
{
   *ppDORet = 0;
   return S_FALSE;
}



//
// See if we can close.
//
bool TBrowserControl::CanClose()
{
	bool result = TWindow::CanClose();
   //must put TOleWindow::CanClose at the end as it assumes that we ARE closing
	//and does all kinds of antisocial things.

   if(result)
   {
      // destroy the Browser control:
      Unbind();
      //TOleWindow::CanClose();
      pthisDisp->Release();
   }
	return result;
}

//
// Ambient property callbacks, mostly these do nothing
// override to change the Ambient properties.
//
bool TBrowserControl::EvOcAmbientGetBackColor(long* backColor)
{
   *backColor = TColor::SysWindow;
	return true;
}

bool TBrowserControl::EvOcAmbientGetForeColor(long* foreColor)
{
   *foreColor = TColor::SysWindowText;
	return true;
}

bool TBrowserControl::EvOcAmbientGetLocaleID(long* /*localeId*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientGetTextAlign(short* /*align*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientGetMessageReflect(bool* /*reflectMessage*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientGetUserMode(bool* userMode)
{
  *userMode = true;
  return true;
}

bool TBrowserControl::EvOcAmbientGetUIDead(bool* deadUI)
{
  *deadUI = false;
  return true;
}

bool TBrowserControl::EvOcAmbientGetShowGrabHandles(bool* showGrabHandles)
{
   *showGrabHandles = false;
	return true;
}

bool TBrowserControl::EvOcAmbientGetShowHatching(bool* showHatching)
{
   *showHatching = false;
	return true;
}

bool TBrowserControl::EvOcAmbientGetDisplayAsDefault(bool* /*displayAsDefault*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientGetSupportsMnemonics(bool* /*supportMnemonics*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientGetDisplayName(TString** /*name*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientGetScaleUnits(TString** /*units*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientGetFont(IDispatch** /*font*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetBackColor(long /*backColor*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetForeColor(long /*foreColor*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetLocaleID(long /*localeId*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetTextAlign(short /*align*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetMessageReflect(bool /*reflect*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetUserMode(bool /*userMode*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetUIDead(bool /*dead*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetShowGrabHandles(bool /*showHandles*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetShowHatching(bool /*showHatching*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetDisplayAsDefault(bool /*displayAsDefault*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetSupportsMnemonics(bool /*supportMnemonics*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetDisplayName(TString* /*name*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetScaleUnits(TString* /*units*/)
{
  return false;
}

bool TBrowserControl::EvOcAmbientSetFont(IDispatch* /*font*/)
{
  return false;
}

//
// Control Event Dispatches
//
bool TBrowserControl::EvOcCtrlClick(TCtrlEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcCtrlDblClick(TCtrlEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcCtrlMouseDown(TCtrlMouseEvent* pev)
{
  return false;
}

bool TBrowserControl::EvOcCtrlMouseUp(TCtrlMouseEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcCtrlMouseMove(TCtrlMouseEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcCtrlKeyDown(TCtrlKeyEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcCtrlKeyUp(TCtrlKeyEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcCtrlErrorEvent(TCtrlErrorEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcCtrlFocus(TCtrlFocusEvent* pev)
{
   if(GetActiveWindow() == HWindow)
      pev->GotFocus = true;
	return true;
}

bool TBrowserControl::EvOcCtrlPropertyChange(TCtrlPropertyEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcCtrlPropertyRequestEdit(TCtrlPropertyEvent* /*pev*/)
{
  return false;
}

bool TBrowserControl::EvOcViewGetSiteRect(TRect* rect)
{
   TOleWindow::EvOcViewGetSiteRect(rect);
   *rect = GetClientRect();
   return true;
}

//
// now come the custom event notifications:
//

void TBrowserControl::StatusTextChange(TAutoString Text)
{
}

void TBrowserControl::ProgressChange(long Progress, long ProgressMax)
{
}

void TBrowserControl::CommandStateChange(long Command, TBool Enable)
{
}

void TBrowserControl::DownloadBegin()
{
}

void TBrowserControl::DownloadComplete()
{
}

void TBrowserControl::TitleChange(TAutoString Text)
{
   SetDocTitle(Text, 0);
}

void TBrowserControl::PropertyChange(TAutoString szProperty)
{
}

void TBrowserControl::BeforeNavigate2(IDispatch* pDisp, TAutoVal URL, TAutoVal Flags, TAutoVal TargetFrameName, TAutoVal PostData, TAutoVal Headers, TBool* Cancel)
{
}

void TBrowserControl::NewWindow2(IDispatch** ppDisp, TBool* Cancel)
{
}

void TBrowserControl::NavigateComplete2(IDispatch* pDisp, TAutoVal URL)
{
}

void TBrowserControl::DocumentComplete(IDispatch* pDisp, TAutoVal URL)
{
   //
   // at this point we can install our UI hook because the document
   // object is now available, for some strange reason pDisp parameter
   // does not point to the document object.....
   //
   if(!IsBound())
      return;
   ICustomDoc* pdoc;
   IDispatch* pdisp = Document();
   if(pdisp)
   {
      pdisp->QueryInterface(IID_ICustomDoc, &((void*)pdoc));
      pdisp->Release();
      if(pdoc)
      {
         pdoc->SetUIHandler(static_cast<IDocHostUIHandler*>(this));
         pdoc->Release();
         init = true;
      }
   }
}

void TBrowserControl::OnQuit()
{
}

void TBrowserControl::OnVisible(TBool Visible)
{
}

void TBrowserControl::OnToolBar(TBool ToolBar)
{
}

void TBrowserControl::OnMenuBar(TBool MenuBar)
{
}

void TBrowserControl::OnStatusBar(TBool StatusBar)
{
}

void TBrowserControl::OnFullScreen(TBool FullScreen)
{
}

void TBrowserControl::OnTheaterMode(TBool TheaterMode)
{
}

bool TBrowserControl::EvOcCtrlCustomEvent(TCtrlCustomEvent* pev)
{
	switch(pev->Args->DispId)
	{
	case 102:
	{
		TAutoString arg0 = pev->Args->operator[](0);
		StatusTextChange(arg0);
		break;
	}
	case 108:
	{
		long arg0 = pev->Args->operator[](0);
		long arg1 = pev->Args->operator[](1);
		ProgressChange(arg0, arg1);
		break;
	}
	case 105:
	{
		long arg0 = pev->Args->operator[](0);
		TBool arg1 = pev->Args->operator[](1);
		CommandStateChange(arg0, arg1);
		break;
	}
	case 106:
	{
		DownloadBegin();
		break;
	}
	case 104:
	{
		DownloadComplete();
		break;
	}
	case 113:
	{
		TAutoString arg0 = pev->Args->operator[](0);
		TitleChange(arg0);
		break;
	}
	case 112:
	{
		TAutoString arg0 = pev->Args->operator[](0);
		PropertyChange(arg0);
		break;
	}
	case 250:
	{
		IDispatch* arg0 = pev->Args->operator[](0);
		TAutoVal arg1 = pev->Args->operator[](1);
		TAutoVal arg2 = pev->Args->operator[](2);
		TAutoVal arg3 = pev->Args->operator[](3);
		TAutoVal arg4 = pev->Args->operator[](4);
		TAutoVal arg5 = pev->Args->operator[](5);
		TBool* arg6 = pev->Args->operator[](6);
		BeforeNavigate2(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
		break;
	}
	case 251:
	{
		IDispatch** arg0 = ((VARIANT*)&(pev->Args->operator[](0)))->ppdispVal;
		TBool* arg1 = pev->Args->operator[](1);
		NewWindow2(arg0, arg1);
		break;
	}
	case 252:
	{
		IDispatch* arg0 = pev->Args->operator[](0);
		TAutoVal arg1 = pev->Args->operator[](1);
		NavigateComplete2(arg0, arg1);
		break;
	}
	case 259:
	{
		IDispatch* arg0 = pev->Args->operator[](0);
		TAutoVal arg1 = pev->Args->operator[](1);
		DocumentComplete(arg0, arg1);
		break;
	}
	case 253:
	{
		OnQuit();
		break;
	}
	case 254:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnVisible(arg0);
		break;
	}
	case 255:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnToolBar(arg0);
		break;
	}
	case 256:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnMenuBar(arg0);
		break;
	}
	case 257:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnStatusBar(arg0);
		break;
	}
	case 258:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnFullScreen(arg0);
		break;
	}
	case 260:
	{
		TBool arg0 = pev->Args->operator[](0);
		OnTheaterMode(arg0);
		break;
	}
	}
	return true;
}

void TBrowserControl::OnButtonClicked()
{
   //
   // known problems:
   // exceptions thrown from here, even when caught
   // tend to crash the app.
   //
   // it would probably be better to just post a message from here
   // and execute our code from the windows message loop, and not
   // from this callback.
   //
   PostMessage(WM_COMMAND, CM_ONCLICKED);
}


void TBrowserControl::CmOnButtonClicked()
{
   try
   {
      //
      // declare the proxy objects we want to use:
      //
      TIHTMLDocument2Proxy doc;                   // the HTML document
      TIHTMLElementCollectionProxy collection;    // the all collection
      TIHTMLSelectElementProxy input;             // the input control we are interested in
      TIHTMLOptionElementProxy option;            // the selected option on the input control

      doc.Bind(Document());                       // get the document object from the browser
      doc.all(collection);                        // get the all collection
                                                  // and then the item we want by name (id)
      input.Bind(collection.item(TAutoString("CommunicateText")));

      if(input.IsBound())                         // check we got the control OK
      {
         int i = input.selectedIndex();           // get the index of the selection
         option.Bind(input.item(i));              // get the selected object from its index
         TAutoString s = option.text();           // finally get the text of the selected item
         char buf[256];
         sprintf(buf, "the text entered was: \"%s\".", (const char*)s);
         MessageBox(buf, "Communication Succeeded", MB_OK);
      }
      else
      {
         // error couldn't find the DHTML item we wanted:
         MessageBox("Unable to find item \"CommunicateText\"", "Error", MB_OK);
      }
   }
   catch(xmsg& msg)
   {
      MessageBox(msg.why().c_str(), "Error", MB_OK);
   }
   catch(...)
   {
      MessageBox("Unhandled exception", "Error!!", MB_OK);
   }
}


