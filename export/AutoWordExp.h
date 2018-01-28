//---------------------------------------------------------------------------#ifndef AutoWordHExp
#define AutoWordHExp
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

class NSAutoWordViewExp ;

//---------------------------------------------------------------------------
class TAutoWordFormEx : public TForm
{__published:	// IDE-managed Components

private:	// User declarations
public:		// User declarations
    NSAutoWordViewExp* pView;    __fastcall TAutoWordFormEx(TComponent* Owner);
               TAutoWordFormEx(HWND Parent, NSAutoWordViewExp* View): TForm(Parent)                { pView = View; }
};

//---------------------------------------------------------------------------extern PACKAGE TAutoWordFormEx *AutoWordFormEx;
//---------------------------------------------------------------------------

#endif
