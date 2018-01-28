//---------------------------------------------------------------------------
#ifndef WebServiceH
#define WebServiceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SHDocVw_OCX.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <OleCtrls.hpp>
#include <ToolWin.hpp>
#include <string>

class NSWebServiceWindow ;

//---------------------------------------------------------------------------
class TWebServiceForm : public TForm
{
__published:	// IDE-managed Components
	TWebBrowser *Control;
	TToolBar *ServicesToolBar;
	TBitBtn *ActionButton;
	TButton *Previous;
	TButton *Next;
	TButton *Reload;
	TButton *Stop;
	void __fastcall ActionButtonClick(TObject *Sender);
	void __fastcall pagePrevious(TObject *Sender);
	void __fastcall pageNext(TObject *Sender);
	void __fastcall pageReload(TObject *Sender);
	void __fastcall loadStop(TObject *Sender);
  void __fastcall NavigateErrorEvent(TObject *Sender, LPDISPATCH pDisp,
          TVariant *URL, TVariant *Frame, TVariant *StatusCode,
          TOLEBOOL *Cancel);
private:	// User declarations
public:		// User declarations

	NSWebServiceWindow* pWebServicesWindow ;

	__fastcall TWebServiceForm(TComponent* Owner) ;

  TWebServiceForm(HWND Parent, NSWebServiceWindow* Win): TForm(Parent)
                { pWebServicesWindow = Win ; }
};
//---------------------------------------------------------------------------
extern PACKAGE TWebServiceForm *WebServiceForm;
//---------------------------------------------------------------------------
#endif
