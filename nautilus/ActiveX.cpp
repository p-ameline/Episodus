//---------------------------------------------------------------------------#include <vcl.h>
#pragma hdrstop

#include "ActiveX.h"#include "nautilus\nsvisual.h"

//---------------------------------------------------------------------------#pragma package(smart_init)
// #pragma link "SHDocVw_TLB"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"

TWebProxy *WebProxy ;
//---------------------------------------------------------------------------__fastcall TWebProxy::TWebProxy(TComponent* Owner)
                     :TForm(Owner)
{
  pView = 0 ;
}

// Constructeur NautilusTWebProxy::TWebProxy(HWND Parent, NSVisualView* View): TForm(Parent)
{
	pView = View ;

	// CoInitialize(NULL);
	// Control = new TWebBrowser(Parent);
	// Control->Align = alClient;
}

//---------------------------------------------------------------------------void __fastcall TWebProxy::RetrieveURL(AnsiString URL)
{
	wchar_t buff[100] ;

	// SetStatusText("Retrieving document " + URL + "...");try
{
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, URL.c_str(), -1, buff, sizeof(buff)) ;
  Control->Navigate(buff, 0, 0, 0, 0) ;
}
catch (EOleException* e)
{
	ShowMessage(e->Message) ;
}
}
//---------------------------------------------------------------------------

void __fastcall TWebProxy::FormMouseDown(TObject *Sender,                        TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button == mbRight)
    {
    }
}
//---------------------------------------------------------------------------

void __fastcall TWebProxy::ControlNavigateComplete2(TObject* Sender,                                            LPDISPATCH pDisp, void* /* TVariant* */ URL)
{
  if (pView)
    pView->NavigateComplete() ;
}
//---------------------------------------------------------------------------


void __fastcall TWebProxy::NavigateError(TObject *Sender, LPDISPATCH pDisp,
      TVariant *URL, TVariant *Frame, TVariant *StatusCode,
      TOLEBOOL *Cancel)
{
  int iStatusCode = int(*StatusCode) ;
  AnsiString sUrl = AnsiString(*URL) ;

  if (pView)
    pView->NavigateErrorEvent(iStatusCode, string(sUrl.c_str())) ;
}
//---------------------------------------------------------------------------

