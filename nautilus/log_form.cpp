//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "log_form.h"
#include "nautilus\nsbrowse.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)// #pragma link "SHDocVw_TLB"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"

TWebLog *WebLog ;

//---------------------------------------------------------------------------__fastcall TWebLog::TWebLog(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------void __fastcall TWebLog::ClipButtonClick(TObject *Sender)
{
  pLog->CmClipCopy() ;
}

//---------------------------------------------------------------------------void __fastcall TWebLog::CloseButtonClick(TObject *Sender)
{
  pLog->CmFermer() ;
}

//---------------------------------------------------------------------------
void __fastcall TWebLog::ImpButtonClick(TObject *Sender)
{
  pLog->CmImprimer() ;
}
//---------------------------------------------------------------------------

void __fastcall TWebLog::NavigateErrorEvent(TObject *Sender,
      LPDISPATCH pDisp, TVariant *URL, TVariant *Frame,
      TVariant *StatusCode, TOLEBOOL *Cancel)
{
  int iStatusCode = int(*StatusCode) ;
  AnsiString sUrl = AnsiString(*URL) ;

  if (NULL != pLog)
    pLog->NavigateErrorEvent(iStatusCode, string(sUrl.c_str())) ;
}
//---------------------------------------------------------------------------

