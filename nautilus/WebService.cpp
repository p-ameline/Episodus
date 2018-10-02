//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "WebService.h"
#include "nautilus\nsbrowseSvce.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TWebServiceForm *WebServiceForm;
//---------------------------------------------------------------------------
__fastcall TWebServiceForm::TWebServiceForm(TComponent* Owner)
	                         :TForm(Owner)
{
  pWebServicesWindow = (NSWebServiceWindow*) 0 ;
}

//---------------------------------------------------------------------------
void __fastcall TWebServiceForm::ActionButtonClick(TObject *Sender)
{
	if (pWebServicesWindow)
		pWebServicesWindow->Action() ;
}
//---------------------------------------------------------------------------

void __fastcall TWebServiceForm::pagePrevious(TObject *Sender)
{
	Control->GoBack() ;
}
//---------------------------------------------------------------------------

void __fastcall TWebServiceForm::pageNext(TObject *Sender)
{
	Control->GoForward() ;
}
//---------------------------------------------------------------------------

void __fastcall TWebServiceForm::pageReload(TObject *Sender)
{
	Control->Refresh() ;
}
//---------------------------------------------------------------------------

void __fastcall TWebServiceForm::loadStop(TObject *Sender)
{
	Control->Stop() ;
}
//---------------------------------------------------------------------------

void __fastcall TWebServiceForm::NavigateErrorEvent(TObject *Sender,
      LPDISPATCH pDisp, TVariant *URL, TVariant *Frame,
      TVariant *StatusCode, TOLEBOOL *Cancel)
{
  int iStatusCode = int(*StatusCode) ;
  AnsiString sUrl = AnsiString(*URL) ;

  if (pWebServicesWindow)
    pWebServicesWindow->NavigateErrorEvent(iStatusCode, string(sUrl.c_str())) ;
}
//---------------------------------------------------------------------------

