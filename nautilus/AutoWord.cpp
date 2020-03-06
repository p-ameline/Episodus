//---------------------------------------------------------------------------#include <vcl.h>
#pragma hdrstop

#include "AutoWord.h"
#include "nsbb\nsutidlg.h"
#include "nautilus\nsttx.h"

//---------------------------------------------------------------------------#pragma package(smart_init)
#pragma resource "*.dfm"
TAutoWordForm *AutoWordForm;
//---------------------------------------------------------------------------

__fastcall TAutoWordForm::TAutoWordForm(TComponent* Owner)    : TForm(Owner)
{
}

//---------------------------------------------------------------------------void __fastcall TAutoWordForm::ArchiverButtonClick(TObject *Sender)
{
  pView->CmArchiver() ;
}

//---------------------------------------------------------------------------void __fastcall TAutoWordForm::PublierButtonClick(TObject *Sender)
{
  pView->CmPublier() ;
}

//---------------------------------------------------------------------------void __fastcall TAutoWordForm::ComposerButtonClick(TObject *Sender)
{
  pView->CmComposer() ;
}

//---------------------------------------------------------------------------void __fastcall TAutoWordForm::EditerButtonClick(TObject *Sender)
{
  pView->CmEditer() ;
}

//---------------------------------------------------------------------------void __fastcall TAutoWordForm::FormulesButtonClick(TObject *Sender)
{
  pView->CmFormules() ;
}

//---------------------------------------------------------------------------void __fastcall TAutoWordForm::QuitterButtonClick(TObject *Sender)
{
  pView->CmQuitter() ;
}

//---------------------------------------------------------------------------void __fastcall TAutoWordForm::VisualiserButtonClick(TObject *Sender)
{
  pView->CmVisualiser() ;
}

//---------------------------------------------------------------------------

void __fastcall TAutoWordForm::LettreTypeButtonClick(TObject *Sender)
{
  pView->CmFileOpen() ;
}
//---------------------------------------------------------------------------

