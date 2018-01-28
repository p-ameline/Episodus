//---------------------------------------------------------------------------#ifndef LogFormH
#define LogFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include "SHDocVw_OCX.h"

class NSLogWindow ;

//---------------------------------------------------------------------------

class TWebLog : public TForm{
__published:	// Composants gérés par l'EDI

    TPanel *Panel;    TButton *ClipButton;
    TButton *CloseButton;
    TButton *ImpButton;
    TWebBrowser *Control;
    void __fastcall ClipButtonClick(TObject *Sender);
    void __fastcall CloseButtonClick(TObject *Sender);
    void __fastcall ImpButtonClick(TObject *Sender);
  void __fastcall NavigateErrorEvent(TObject *Sender, LPDISPATCH pDisp,
          TVariant *URL, TVariant *Frame, TVariant *StatusCode,
          TOLEBOOL *Cancel);

private:	// Déclarations de l'utilisateur

public:		// Déclarations de l'utilisateur
    NSLogWindow* pLog;
    __fastcall TWebLog(TComponent* Owner);               TWebLog(HWND Parent, NSLogWindow* Win): TForm(Parent)
                { pLog = Win; }
};

//---------------------------------------------------------------------------
extern PACKAGE TWebLog *WebLog;
//---------------------------------------------------------------------------
#endif
