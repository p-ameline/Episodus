//---------------------------------------------------------------------------
#ifndef ExamHistoH
#define ExamHistoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>

class NSVisualView ;

//---------------------------------------------------------------------------
class TExamHistoForm : public TForm
{
__published:	// IDE-managed Components
  TBitBtn *Previous;
  TBitBtn *Next;
  TWebBrowser *Control;
  TPanel *Pannel;
  void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall NavigateError(TObject *Sender, LPDISPATCH pDisp,
          TVariant *URL, TVariant *Frame, TVariant *StatusCode,
          TOLEBOOL *Cancel);
  void __fastcall ControlNavigateComplete2(TObject *Sender,
          LPDISPATCH pDisp, void* /* TVariant* */ URL);
  void __fastcall openPrevious(TObject *Sender);
  void __fastcall openNext(TObject *Sender);
private:	// User declarations
public:		// User declarations

  NSVisualView*   pView ;

  __fastcall      TExamHistoForm(TComponent* Owner) ;                  TExamHistoForm(HWND Parent, NSVisualView* View) ;
  void __fastcall RetrieveURL(AnsiString URL) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TExamHistoForm *ExamHistoForm;
//---------------------------------------------------------------------------
#endif
 