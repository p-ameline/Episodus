//---------------------------------------------------------------------------#ifndef NssmtpH
#define NssmtpH
//---------------------------------------------------------------------------

#include <Classes.hpp>#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <NMsmtp.hpp>
#include <Psock.hpp>
#include <cstring.h>

class NSContexte;
class NSRefDocument;
class NSMailParams;
//---------------------------------------------------------------------------
class TControlSMTP : public TForm{
__published:	// Composants gérés par l'EDI

    TPanel      *Panel;    TNMSMTP     *SMTP;
    TStatusBar  *StatusBar;
    TEdit       *EditHost;
    TLabel      *Label1;
    TEdit       *EditPort;
    TLabel      *Label2;
    TEdit       *EditUser;
    TLabel      *Label3;
    TEdit       *EditExp;
    TLabel      *Label4;
    TEdit       *EditDest;
    TLabel      *Label5;
    TButton     *AnnulButton;
    TStatusBar  *StatusBarServeur;

    void __fastcall AnnulButtonClick(TObject *Sender);
    void __fastcall SMTPConnect(TObject *Sender);
    void __fastcall SMTPConnectionFailed(TObject *Sender);
    void __fastcall SMTPInvalidHost(bool &handled);
    void __fastcall SMTPDisconnect(TObject *Sender);
    void __fastcall SMTPConnectionRequired(bool &handled);
    void __fastcall SMTPSendStart(TObject *Sender);
    void __fastcall SMTPFailure(TObject *Sender);
    void __fastcall SMTPSuccess(TObject *Sender);
    void __fastcall SMTPStatus(TComponent *Sender, AnsiString Status);
    void __fastcall SMTPAuthentificationFailed(bool &Handled);  void __fastcall SMTPRecipientNotFound(AnsiString Recipient);
  private:	// Déclarations de l'utilisateur
    bool        bInterrupt;    bool        bAttendre;
    bool        bApicrypt;

  public:		// Déclarations de l'utilisateur
    NSContexte*     pContexte;    NSRefDocument*  pDoc;
    NSMailParams*   pMail;
    AnsiString      encours;

    bool            bConnected;

    __fastcall TControlSMTP(TComponent* Owner);
    __fastcall TControlSMTP(TComponent* Owner, NSContexte* ctx,
                            NSRefDocument* doc, NSMailParams* mail);
    __fastcall ~TControlSMTP();

    void    Connect();    void    Disconnect();
    bool    Verify();
    void    ComposeMessage(string sFromAdr, string sToAdr, string sObjet, string sCodeLettre);
    void    ComposeSimpleMessage(string sFromAdr, string sToAdr, string sObjet, string sMessage);
    void    AttacheMessage(string sFileName);
    void    SendMail();
    bool    Continuer();
    bool    Attendre();
    bool    SetApicryptMode() { bApicrypt = true ; return true ; }
};

//---------------------------------------------------------------------------
extern PACKAGE TControlSMTP *ControlSMTP;
//---------------------------------------------------------------------------
#endif
