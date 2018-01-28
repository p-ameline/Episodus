//---------------------------------------------------------------------------#ifndef WebTypsH
#define WebTypsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include "SHDocVw_OCX.h"

class NSWebTipWindow ;

//---------------------------------------------------------------------------

class TWebTip : public TForm{
__published:	// Composants gérés par l'EDI

    TPanel *Panel;
    TButton *CloseButton;
    TWebBrowser *Control;
    void __fastcall CloseButtonClick(TObject *Sender);

private:	// Déclarations de l'utilisateur

public:		// Déclarations de l'utilisateur
    NSWebTipWindow* pTip;
    __fastcall TWebTip(TComponent* Owner);
    TWebTip(HWND Parent, NSWebTipWindow* Win): TForm(Parent)
                { pTip = Win; }
};

//---------------------------------------------------------------------------
extern PACKAGE TWebTip *WebTip;
//---------------------------------------------------------------------------
#endif
