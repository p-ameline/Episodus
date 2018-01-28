//---------------------------------------------------------------------------#ifndef ImportH
#define ImportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include "SHDocVw_OCX.h"

class NSImportWindow ;
//---------------------------------------------------------------------------class TWebImport : public TForm
{

__published:	// Composants gérés par l'EDI
  TPanel *Panel ;

  TButton *ImpButton ;
  TButton *RefButton ;
  TButton *AnnButton ;
  TButton *CaptButton ;

  TWebBrowser *Control ;

  void __fastcall ImpButtonClick(TObject *Sender);  void __fastcall RefButtonClick(TObject *Sender);
  void __fastcall AnnButtonClick(TObject *Sender);
	void __fastcall CaptButtonClick(TObject *Sender);  void __fastcall NavigateErrorEvent(TObject *Sender, LPDISPATCH pDisp,
          TVariant *URL, TVariant *Frame, TVariant *StatusCode,
          TOLEBOOL *Cancel);
private:	// Déclarations de l'utilisateurpublic:		// Déclarations de l'utilisateur
  NSImportWindow* pImport ;
  __fastcall TWebImport(TComponent* Owner);             TWebImport(HWND Parent, NSImportWindow* Win);

  __fastcall ~TWebImport() ;

  void Close() ;
};

//---------------------------------------------------------------------------extern PACKAGE TWebImport *WebImport;
//---------------------------------------------------------------------------

#endif
