//---------------------------------------------------------------------------#include <vcl.h>
#pragma hdrstop

#include "Compos.h"
#include "nautilus\nsbrowse.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
// #pragma link "SHDocVw_TLB"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"

TWebCompos *WebCompos ;

//---------------------------------------------------------------------------
__fastcall TWebCompos::TWebCompos(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------void __fastcall TWebCompos::OKButtonClick(TObject *Sender)
{
	pView->CmOk() ;
}

//---------------------------------------------------------------------------void __fastcall TWebCompos::CancelButtonClick(TObject *Sender)
{
	pView->CmCancel() ;
}

//---------------------------------------------------------------------------
void __fastcall TWebCompos::NavigateErrorEvent(TObject *Sender,
      LPDISPATCH pDisp, TVariant *URL, TVariant *Frame,
      TVariant *StatusCode, TOLEBOOL *Cancel)
{
  int iStatusCode = int(*StatusCode) ;
  AnsiString sUrl = AnsiString(*URL) ;

  if (NULL != pView)
    pView->NavigateErrorEvent(iStatusCode, string(sUrl.c_str())) ;
}
//---------------------------------------------------------------------------

