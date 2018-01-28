//---------------------------------------------------------------------------
#define LettreH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <cstring.h>

class PubliCorrespDialog;
class NSRefDocument;
class NSTtxDocument;

//---------------------------------------------------------------------------
class TLettreType : public TForm
{
__published:	// Composants g�r�s par l'EDI

	TPanel *Panel;
	TButton *AnnulButton;
	TRichEdit *RichEdit;
	TButton *SelectButton;
  TButton *CopyButton;
  TButton *CutButton;
  TButton *PasteButton;
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall AnnulButtonClick(TObject *Sender);
  void __fastcall SelectButtonClick(TObject *Sender);
  void __fastcall CopyButtonClick(TObject *Sender);
  void __fastcall CutButtonClick(TObject *Sender);
  void __fastcall PasteButtonClick(TObject *Sender);
  void __fastcall RichEditMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FormCreate(TObject *Sender);

private:	// D�clarations de l'utilisateur

public:		// D�clarations de l'utilisateur
	PubliCorrespDialog* pDlg ;
	NSTtxDocument*		pDocTtx ;

	__fastcall TLettreType(TComponent* Owner, PubliCorrespDialog* pere,

	__fastcall TLettreType(TComponent* Owner) ;

  void   SetText(string sTexte) ;
  void   OuvreLettre(bool bReadOnly = false) ;
  void   OuvreLettreRTF(char* fichier) ;
  string InitTitre() ;
};

//---------------------------------------------------------------------------
extern PACKAGE TLettreType *LettreType;
//---------------------------------------------------------------------------
#endif