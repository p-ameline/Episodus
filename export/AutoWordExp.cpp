//---------------------------------------------------------------------------#include <vcl.h>
#pragma hdrstop

#include "AutoWordExp.h"
#include "export\html2word.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TAutoWordFormEx *AutoWordFormEx;
//---------------------------------------------------------------------------__fastcall TAutoWordFormEx::TAutoWordFormEx(TComponent* Owner)
    : TForm(Owner)
{
}

