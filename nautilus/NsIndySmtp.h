//---------------------------------------------------------------------------
#ifndef NsIndySmtpH
#define NsIndySmtpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdMessageClient.hpp"
#include "IdSMTP.hpp"
#include "IdTCPClient.hpp"
#include "IdTCPConnection.hpp"
#include <ExtCtrls.hpp>

#include <cstring.h>

class NSContexte;
class NSRefDocument;
class NSMailParams;

//---------------------------------------------------------------------------
class TIndySMTP : public TForm
{
__published:	// IDE-managed Components
  TIdSMTP *SMTP;
  TPanel *SmtpParams;
  TLabel *HostLabel;
  TLabel *HostName;
  TLabel *UserLabel;
  TLabel *UserText;
  TPanel *MailParams;
  TLabel *FromLabel;
  TLabel *FromText;
  TLabel *LabelTo;
  TLabel *TextTo;
  TPanel *StatusPanel;
  TCheckBox *ConnectedCB;
  TCheckBox *SendingCB;
  TCheckBox *SentCB;
  TPanel *MessagePanel;
  TLabel *MessageText;
  void __fastcall OnSmtpConnected(TObject *Sender);
  void __fastcall OnSmtpWorkBegin(TObject *Sender, TWorkMode AWorkMode,
          const int AWorkCountMax);
  void __fastcall OnSmtpWorkEnd(TObject *Sender, TWorkMode AWorkMode);

  private:	// Déclarations de l'utilisateur

    bool        _bInterrupt;    bool        _bAttendre;
    bool        _bApicrypt;

    bool        _bConnected ;
    bool        _bAuthenticated ;

    TIdMessage* Message ;

    void initData() ;
    void initInterface() ;

    void InitMessage(const string sSender, const string sNameFrom,
                     const string sMailFrom, const string sReplyTo,
                     const string sToAdr, const string sObjet) ;

  public:		// Déclarations de l'utilisateur
    NSContexte*     pContexte ;    NSRefDocument*  pDoc ;
    NSMailParams*   pMail ;
    AnsiString      encours ;

  __fastcall TIndySMTP(TComponent* Owner);
  __fastcall TIndySMTP(TComponent* Owner, NSContexte* ctx, NSRefDocument* doc, NSMailParams* mail);
  __fastcall ~TIndySMTP() ;

    void Connect() ;
    void Disconnect() ;
    bool Verify() ;

    void ComposeMessage(const string sSender, const string sNameFrom,
                        const string sMailFrom, const string sReplyTo,
                        const string sToAdr, const string sObjet,
                        const string sCodeLettre) ;
    void ComposeSimpleMessage(const string sSender, const string sNameFrom,
                              const string sMailFrom, const string sReplyTo,
                              const string sToAdr, const string sObjet,
                              const string sMessage) ;
    void AttacheMessage(string sFileName) ;
    void ClearMessage() ;
    void SendMail() ;
    bool Continuer() ;
    bool Attendre() ;
    bool SetApicryptMode() { _bApicrypt = true ; return true ; }
    bool IsConnected()     { return _bConnected ; }
    bool IsAuthenticated() { return _bAuthenticated ; }
};
//---------------------------------------------------------------------------
extern PACKAGE TIndySMTP *IndySMTP;
//---------------------------------------------------------------------------
#endif
