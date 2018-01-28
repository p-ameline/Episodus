//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ImpImg.h"
#include "nautilus\nsbrowse.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
// #pragma link "SHDocVw_TLB"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"

TImportImg *ImportImg;

//---------------------------------------------------------------------------
__fastcall TImportImg::TImportImg(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TImportImg::QuitButtonClick(TObject *Sender)
{
  pWin->CmQuitter() ;
}
//---------------------------------------------------------------------------

void __fastcall TImportImg::ImportButtonClick(TObject *Sender)
{
  pWin->CmImporterTout() ;
}
//---------------------------------------------------------------------------


void __fastcall TImportImg::ControlNavigateComplete2(TObject *Sender,
      LPDISPATCH pDisp, TVariant *URL)
{
  pWin->NavigateComplete() ;
}
//---------------------------------------------------------------------------

void __fastcall TImportImg::NavigateErrorEvent(TObject *Sender,
      LPDISPATCH pDisp, TVariant *URL, TVariant *Frame,
      TVariant *StatusCode, TOLEBOOL *Cancel)
{
  int iStatusCode = int(*StatusCode) ;
  AnsiString sUrl = AnsiString(*URL) ;

  if (NULL != pWin)
    pWin->NavigateErrorEvent(iStatusCode, string(sUrl.c_str())) ;
}
//---------------------------------------------------------------------------

