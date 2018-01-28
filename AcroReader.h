//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "AcroPDFLib_OCX.h"
#include <OleCtrls.hpp>

class NSAcrobatView ;

//---------------------------------------------------------------------------
class TAcrobatProxy : public TForm
{
__published:	// IDE-managed Components
  TAcroPDF *Control;
	void __fastcall FormCreate(TObject *Sender) ;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action) ;
  void __fastcall BeforeDestruction(void) ;
  void __fastcall AcroError(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAcrobatProxy(TComponent* Owner) ;
  TAcrobatProxy(HWND Parent, NSAcrobatView* View) ;

	NSAcrobatView* pView ;
};
//---------------------------------------------------------------------------
extern PACKAGE TAcrobatProxy *AcrobatProxy;
//---------------------------------------------------------------------------
#endif
