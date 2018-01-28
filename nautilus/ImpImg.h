//---------------------------------------------------------------------------
#ifndef ImpImgH
#define ImpImgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include "SHDocVw_OCX.h"

class NSImpImgWindow;

//---------------------------------------------------------------------------

class TImportImg : public TForm
{
    __published:	// Composants gérés par l'EDI
        TPanel *Panel;
        TButton *QuitButton;
        TWebBrowser *Control;
        TButton *ImportButton;
        void __fastcall QuitButtonClick(TObject *Sender);
        void __fastcall ImportButtonClick(TObject *Sender);
        void __fastcall ControlNavigateComplete2(TObject *Sender,
          LPDISPATCH pDisp, TVariant *URL);
  void __fastcall NavigateErrorEvent(TObject *Sender, LPDISPATCH pDisp,
          TVariant *URL, TVariant *Frame, TVariant *StatusCode,
          TOLEBOOL *Cancel);

    private:	// Déclarations de l'utilisateur
    public:		// Déclarations de l'utilisateur
        NSImpImgWindow* pWin;

        __fastcall TImportImg(TComponent* Owner);
                   TImportImg(HWND Parent, NSImpImgWindow* Win): TForm(Parent)
                    { pWin = Win; }
};

//---------------------------------------------------------------------------
extern PACKAGE TImportImg *ImportImg;
//---------------------------------------------------------------------------

#endif
