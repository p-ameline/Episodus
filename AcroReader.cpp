//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AcroReader.h"
#include "nautilus\nsAcroRead.h"
#include "nautilus\nssuper.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AcroPDFLib_OCX"
#pragma resource "*.dfm"

TAcrobatProxy *AcrobatProxy ;

//---------------------------------------------------------------------------
__fastcall TAcrobatProxy::TAcrobatProxy(TComponent* Owner)
	                       :TForm(Owner)
{
  pView = (NSAcrobatView*) 0 ;
}

// Constructeur Nautilus
TAcrobatProxy::TAcrobatProxy(HWND Parent, NSAcrobatView* View)
              :TForm(Parent)
{
	pView = View ;
}

//---------------------------------------------------------------------------
void __fastcall TAcrobatProxy::FormCreate(TObject *Sender)
{
  Control->Align = alClient ;
}
//---------------------------------------------------------------------------
void __fastcall TAcrobatProxy::FormClose(TObject *Sender, TCloseAction &Action)
{
  Action = caFree ;
}
//---------------------------------------------------------------------------
void __fastcall TAcrobatProxy::BeforeDestruction(void)
{
  string sTrace = string("TAcrobatProxy BeforeDestruction") ;
  pView->_pDoc->pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  /*
  wchar_t buff[1024] ;
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "", -1, buff, sizeof(buff)) ;
  Control->LoadFile(buff) ;
  */
}
//---------------------------------------------------------------------------

void __fastcall TAcrobatProxy::AcroError(TObject *Sender)
{
  if ((NSAcrobatView*) NULL == pView)
    return ;

  string sTrace = string("TAcrobatProxy Error") ;
  pView->_pDoc->pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trWarning) ;
}
//---------------------------------------------------------------------------

