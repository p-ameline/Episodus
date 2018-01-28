//---------------------------------------------------------------------------#include <vcl.h>
#pragma hdrstop

#include "Import.h"
#include "nautilus\nsbrowse.h"
#include "nautilus\nssuper.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
// #pragma link "SHDocVw_TLB"
#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"

TWebImport *WebImport;

//---------------------------------------------------------------------------
__fastcall TWebImport::TWebImport(TComponent* Owner)
    : TForm(Owner)
{
}

TWebImport::TWebImport(HWND Parent, NSImportWindow* Win)
           :TForm(Parent)
{
  pImport = Win ;
}

__fastcall TWebImport::~TWebImport()
{
  
}

void TWebImport::Close()
{
  TForm::Close() ;
}

//---------------------------------------------------------------------------void __fastcall TWebImport::ImpButtonClick(TObject *Sender)
{
	pImport->CmImporter() ;
}

//---------------------------------------------------------------------------void __fastcall TWebImport::RefButtonClick(TObject *Sender)
{
	pImport->CmReferencer() ;
}

//---------------------------------------------------------------------------void __fastcall TWebImport::AnnButtonClick(TObject *Sender)
{
	pImport->CmAnnuler() ;
}

//---------------------------------------------------------------------------void __fastcall TWebImport::CaptButtonClick(TObject *Sender)
{
	pImport->CmCapturer() ;
}

//---------------------------------------------------------------------------

void __fastcall TWebImport::NavigateErrorEvent(TObject *Sender,
      LPDISPATCH pDisp, TVariant *URL, TVariant *Frame,
      TVariant *StatusCode, TOLEBOOL *Cancel)
{
  int iStatusCode = int(*StatusCode) ;
  AnsiString sUrl = AnsiString(*URL) ;

  if (pImport)
    pImport->NavigateErrorEvent(iStatusCode, string(sUrl.c_str())) ;
}
//---------------------------------------------------------------------------

