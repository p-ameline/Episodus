//---------------------------------------------------------------------------#ifndef AutoWordH
#define AutoWordH

//---------------------------------------------------------------------------#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

class NSAutoWordView;
//---------------------------------------------------------------------------class TAutoWordForm : public TForm
{
  __published:	// IDE-managed Components

    Stdctrls::TButton *ArchiverButton;    Stdctrls::TButton *PublierButton;
    Stdctrls::TButton *EditerButton;
    Stdctrls::TButton *FormulesButton;
    Stdctrls::TButton *QuitterButton;
    Stdctrls::TButton *LettreType;  TImage *Image1;
    void __fastcall ArchiverButtonClick(TObject *Sender);    void __fastcall PublierButtonClick(TObject *Sender);    void __fastcall ComposerButtonClick(TObject *Sender);
    void __fastcall EditerButtonClick(TObject *Sender);
    void __fastcall FormulesButtonClick(TObject *Sender);    void __fastcall QuitterButtonClick(TObject *Sender);    void __fastcall VisualiserButtonClick(TObject *Sender);    void __fastcall LettreTypeButtonClick(TObject *Sender);
  private:	// User declarations
  public:		// User declarations
    NSAutoWordView* pView;
    __fastcall TAutoWordForm(TComponent* Owner);               TAutoWordForm(HWND Parent, NSAutoWordView* View): TForm(Parent)                { pView = View; }};
//---------------------------------------------------------------------------
extern PACKAGE TAutoWordForm *AutoWordForm;
//---------------------------------------------------------------------------
#endif
