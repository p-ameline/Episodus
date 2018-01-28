//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "webTips.h"
#include "nsbb\nsTipBrowser.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)// #pragma link "SHDocVw_TLB"
#pragma link "SHDocVw_OCX"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"

TWebTip *WebTip ;

//---------------------------------------------------------------------------__fastcall TWebTip::TWebTip(TComponent* Owner)
    : TForm(Owner)
{
	BorderStyle = bsToolWindow ;
  FormStyle   = fsStayOnTop ;
}

//---------------------------------------------------------------------------void __fastcall TWebTip::CloseButtonClick(TObject* /* Sender */)
{
  pTip->CmFermer() ;
}

//---------------------------------------------------------------------------
