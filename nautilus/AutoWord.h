//---------------------------------------------------------------------------
#define AutoWordH

//---------------------------------------------------------------------------
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

class NSAutoWordView;
//---------------------------------------------------------------------------
{
  __published:	// IDE-managed Components

    Stdctrls::TButton *ArchiverButton;
    Stdctrls::TButton *EditerButton;
    Stdctrls::TButton *FormulesButton;
    Stdctrls::TButton *QuitterButton;
    Stdctrls::TButton *LettreType;
    void __fastcall ArchiverButtonClick(TObject *Sender);
    void __fastcall EditerButtonClick(TObject *Sender);
    void __fastcall FormulesButtonClick(TObject *Sender);
  private:	// User declarations
  public:		// User declarations
    NSAutoWordView* pView;
    __fastcall TAutoWordForm(TComponent* Owner);
//---------------------------------------------------------------------------
extern PACKAGE TAutoWordForm *AutoWordForm;
//---------------------------------------------------------------------------
#endif