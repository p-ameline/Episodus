//---------------------------------------------------------------------------
#define ActiveXH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <OleCtrls.hpp>
#include "SHDocVw_OCX.h"

class NSVisualView ;

//---------------------------------------------------------------------------
{
	__published:	// Composants g�r�s par l'EDI

		TWebBrowser*    Control ;

		void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
                                            TShiftState Shift, int X, int Y) ;
		void __fastcall ControlNavigateComplete2(TObject *Sender,
                                            LPDISPATCH pDisp, void* /* TVariant* */ URL) ;
  void __fastcall NavigateError(TObject *Sender, LPDISPATCH pDisp,
          TVariant *URL, TVariant *Frame, TVariant *StatusCode,
          TOLEBOOL *Cancel);
	private:	// D�clarations de l'utilisateur

	public:		// D�clarations de l'utilisateur
		NSVisualView*   pView ;
		__fastcall      TWebProxy(TComponent* Owner) ;
		void __fastcall RetrieveURL(AnsiString URL) ;
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#endif