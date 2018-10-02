// NSBROWSE.CPP : Dialogues de composition et de publication
////////////////////////////////////////////////////////////
#include <cstring.h>
#include <stdio.h>#include <assert.h>/** Includes spécifiques capture **/#include <string.h>#include <malloc.h>#include <windows.h>#include <wingdi.h>#include <mshtml.h>///////////////////////////////////
#include <owl\clipboar.h>

#include "WebService.h"#include "nautilus\nsbrowseSvce.h"#include "nautilus\nsbrowse.h"#include "nsbb\nsednum.h"#include "nautilus\nsepicap.h"#include "partage\ole_utils.h"#include "nautilus\nsvisual.h"
/****************** classe NSImportWindow **************************/
DEFINE_RESPONSE_TABLE1(NSWebServiceWindow, TWindow)
   EV_WM_CLOSE,
END_RESPONSE_TABLE;

NSWebServiceWindow::NSWebServiceWindow(TWindow* parent, string sFirstURL, NSContexte* pCtx)
                   :TWindow(parent)
{
	sHtml        = "" ;
	sStartingURL = sFirstURL ;
	pContexte    = pCtx ;
	bCanClose    = false ;

  Form         = NULL ;
}

NSWebServiceWindow::~NSWebServiceWindow()
{
	// Delete de la Form
  if (NULL != Form)
  {
		delete Form ;
		// CoUninitialize() ;
  }
}

void
NSWebServiceWindow::EvClose()
{
	TWindow::EvClose() ;
}

//
// Override a TWindow virtual to create the HWND directly.
// NSImportWindow and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSWebServiceWindow::PerformCreate(int /*menuOrId*/)
{
	//CoInitialize(NULL) ;
	Form = new TWebServiceForm(Parent->GetHandle(), this) ;
	Form->Visible = false ;
	Form->ParentWindow = Parent->HWindow ;
	SetHandle(Form->Handle) ;
	::SetParent(Forms::Application->Handle, pContexte->GetMainWindow()->HWindow) ;

	// on navigue vers l'URL du service
	Navigate(sStartingURL) ;
}

bool
NSWebServiceWindow::IsTabKeyMessage(MSG &msg)
{
	if (GetCapture() == NULL)
	{
		if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
    {
    	if (msg.wParam == VK_TAB)
      {
      	SendMessage(CN_BASE + msg.message, msg.wParam, msg.lParam) ;
        return true ;
      }
    }
  }

	return false ;
}

//// Let the form process keystrokes in its own way.  Without
// this method, you can't tab between control on the form.
//
bool
NSWebServiceWindow::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;
	if (!result)
		result = TWindow::PreProcessMsg(msg) ;

	return result ;
}

// Fonction MakeVisiblevoid
NSWebServiceWindow::MakeVisible()
{
  Form->Visible = true ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSWebServiceWindow::SetupWindow()
{
	TWindow::SetupWindow() ;

	ModifyStyle(WS_BORDER, WS_CHILD) ;
	Form->Show() ;
	MakeVisible() ;
}

// Fonction Navigate
////////////////////////////////////////////////////////////////
void
NSWebServiceWindow::Navigate(string sStartingURL)
{
	if (string("") == sStartingURL)
  {
    string ps = string("NSWebServiceWindow::Navigate with empty URL") ;
	  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

try
{
	Variant Flags(navNoReadFromCache) ;
	TVariant VFlags = Flags.operator TVariant() ;

  wchar_t buff[1024] ;
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sStartingURL.c_str(), -1, buff, sizeof(buff)) ;

	Form->Control->Navigate(buff, &VFlags) ;
}
catch (Exception &ex)
{
  string ps = string("NSWebServiceWindow::Navigate (") + string(ex.Message.c_str()) + string(")") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
}
catch (...)
{
  string ps = string("NSWebServiceWindow::Navigate Exception") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
  erreur(ps.c_str(), standardError, 0) ;
}
}

void
NSWebServiceWindow::NavigateErrorEvent(int iStatusCode, string sURL)
{
	string ps = string("Ev : NavigateError for URL \"") + sURL + string("\"") ;
  ps += string(" (") + getNavigateErrorShortMsg(iStatusCode) + string(")") ;
	pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;

  erreur(ps.c_str(), standardError, 0) ;
}

voidNSWebServiceWindow::CmAnnuler()
{
	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  int retVal = MessageBox("Voulez-vous vraiment annuler l'importation ?", sCaption.c_str(), MB_YESNO) ;

  if (retVal == IDYES)
  {
  	bCanClose = true ;
    CloseWindow() ;
	}
}

void
NSWebServiceWindow::Action()
{
	IDispatch* pdisp = Form->Control->Document ;
  if ((IDispatch*) NULL == pdisp)
		return ;

  sHtml = getHTMLsource(pdisp) ;

  pdisp->Release() ;

  if (sHtml == string(""))
  	return ;

	captureInformation() ;
}

string
NSWebServiceWindow::getHTMLsource(IDispatch* pDocument)
{
  IHTMLDocument2* pDoc = 0;
	HRESULT hr = pDocument->QueryInterface(IID_IHTMLDocument2, (void**)&pDoc) ;

  if (FAILED(hr))
    return string("") ;

  IHTMLElement* pBodyElem = 0 ;
	hr = pDoc->get_body(&pBodyElem);
	if (FAILED(hr))
    return string("") ;

  BSTR bstrHTMLText = 0 ;
  hr = pBodyElem->get_outerHTML(&bstrHTMLText) ;
  if (FAILED(hr))
  	return string("") ;

  // Convert the text from Unicode to ANSI
  string sResult = BSTRtoSTLstring(bstrHTMLText) ;

	::SysFreeString(bstrHTMLText) ;

  return sResult ;
}

void
NSWebServiceWindow::captureInformation()
{
	if (sHtml == string(""))
  	return ;

  // Get the list of episodus data

  string sTags = getDataFromTag(&sHtml, string("<!-- episodus"), string("-->")) ;

  // parse it

  strip(sTags, stripBoth) ;

  if (sTags == string(""))
  	return ;

  string sBeginDataTag = string("<data ") ;
  string sEndDataTag   = string("/>") ;

  size_t iBeginData = sTags.find(sBeginDataTag) ;
  if (iBeginData == NPOS)
  	return ;

  size_t iBeginDataTagSize = strlen(sBeginDataTag.c_str()) ;

  // Get the capture array

  NSEpisodus* pEpisodus = pContexte->getEpisodus() ;
  if ((NSEpisodus*) NULL == pEpisodus)
  	return ;

  NSCaptureArray* pNewCaptureArray = &(pEpisodus->newCaptureArray) ;

  bool bCapturedElements = false ;

  while (NPOS != iBeginData)
  {
    // Take a data tag container

  	size_t iEndData = sTags.find(sEndDataTag, iBeginData + iBeginDataTagSize) ;
    if (NPOS == iEndData)
    	break ;

    size_t iStartingDataPosition = iBeginData + iBeginDataTagSize ;
    string sDataString = string(sTags, iStartingDataPosition, iEndData - iStartingDataPosition) ;

    string sPathString      = string("") ;
    string sValueString     = string("") ;
    string sUnitString      = string("") ;
    string sClassifString   = string("") ;
    string sInformationDate = string("") ;

    // Parse it
    takeCapturedEntries(&sDataString, &sPathString, &sValueString, &sUnitString) ;

    // Put the capture information in the capture array
    if ((string("") != sPathString) && (string("") != sValueString))
    {
    	pNewCaptureArray->ajouter(new NSCapture(pContexte, sPathString, sValueString, FromOutside, sClassifString, sUnitString, sInformationDate)) ;
      bCapturedElements = true ;
    }

    iBeginData = sTags.find(sBeginDataTag, iEndData) ;
  }

  if (bCapturedElements)
  	pContexte->captureData(pNewCaptureArray) ;
}

void
NSWebServiceWindow::takeCapturedEntries(string *pEntry, string* pPath, string* pValue, string* pUnit)
{
	if (!pEntry || !pPath || !pValue || !pUnit)
  	return ;

  if (*pEntry == string(""))
  {
  	*pPath = *pValue = *pUnit = string("") ;
    return ;
  }

  *pPath  = getDataFromTag(pEntry, string("path='"), string("'")) ;
  *pValue = getDataFromTag(pEntry, string("value='"), string("'")) ;
  *pUnit  = getDataFromTag(pEntry, string("unit='"), string("'")) ;
}

string
NSWebServiceWindow::getDataFromTag(string *pEntry, string sBeginTag, string sEndTag)
{
  string sResult = string("") ;

	if (!pEntry || (sBeginTag == string("")) || (sEndTag == string("")))
  	return sResult ;

	size_t iBeginTagPos = pEntry->find(sBeginTag) ;
  if (iBeginTagPos == NPOS)
  	return sResult ;

  size_t iBeginTagSize = strlen(sBeginTag.c_str()) ;

	size_t iEndTagPos   = pEntry->find(sEndTag, iBeginTagPos + iBeginTagSize) ;
  if (iEndTagPos == NPOS)
  	return sResult ;

  size_t iStartingDataPosition = iBeginTagPos + iBeginTagSize ;
	sResult = string(*pEntry, iStartingDataPosition, iEndTagPos - iStartingDataPosition) ;

  return sResult ;
}

/****************** classe NSImportChild **************************/
DEFINE_RESPONSE_TABLE1(NSWebServiceChild, TMDIChild)
	 EV_WM_CLOSE,
	 EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;


NSWebServiceChild::NSWebServiceChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSWebServiceWindow* clientWnd)					        :TMDIChild(parent, title, clientWnd), NSRoot(pCtx)
{
	pClient = clientWnd;
}

NSWebServiceChild::~NSWebServiceChild()
{
}

voidNSWebServiceChild::EvClose()
{
	if (!pClient->bCanClose)
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
		int retVal = MessageBox("Voulez-vous vraiment fermer ?", sCaption.c_str(), MB_YESNO) ;

		if (retVal == IDYES)
			TWindow::EvClose() ;
	}
	else
		TWindow::EvClose() ;
}

////////////////////////// fin de nsbrowse.cpp ///////////////////////////////
