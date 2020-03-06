//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "NsIndySmtp.h"

#include "nautilus\nschoisi.h"      // fait un include de nsdocref.h
#include "nautilus\ns_html.h"       // pour ConvertRTFDialog
#include "nautilus\nsdocref.h"       
#include "nautilus\nssuper.h"       // pour NSContexte

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdMessageClient"
#pragma link "IdSMTP"
#pragma link "IdTCPClient"
#pragma link "IdTCPConnection"
#pragma resource "*.dfm"

TIndySMTP *IndySMTP;

//---------------------------------------------------------------------------
__fastcall TIndySMTP::TIndySMTP(TComponent* Owner)
                     :TForm(Owner)
{
  initData() ;
  initInterface() ;
}

__fastcall TIndySMTP::TIndySMTP(TComponent* Owner, NSContexte* ctx,
                            NSRefDocument* doc, NSMailParams* mail) : TForm(Owner)
{
  // on ne fait pas de copie : tous les pointeurs sont résidents
  pContexte   = ctx ;
  pDoc        = doc ;
  pMail       = mail ;
  encours     = "(vide)" ;

  initData() ;
  initInterface() ;
}

__fastcall TIndySMTP::~TIndySMTP()
{
  delete Message ;
}

void
TIndySMTP::initData()
{
  _bInterrupt     = false ;
  _bAttendre      = false ;
  _bConnected     = false ;
  _bAuthenticated = false ;
  _bApicrypt      = false ;

  Message = new TIdMessage(this) ;
}

void
TIndySMTP::initInterface()
{
  ConnectedCB->Checked = false ;
  SendingCB->Checked   = false ;
  SentCB->Checked      = false ;

  FromText->Caption    = "" ;
  HostName->Caption    = "" ;
  UserText->Caption    = "" ;
  FromText->Caption    = "" ;
  TextTo->Caption      = "" ;
  MessageText->Caption = "" ;
}

//---------------------------------------------------------------------------
void __fastcall TIndySMTP::OnSmtpConnected(TObject *Sender)
{
  ConnectedCB->Checked = true ;
}
//---------------------------------------------------------------------------


void __fastcall TIndySMTP::OnSmtpWorkBegin(TObject *Sender,
      TWorkMode AWorkMode, const int AWorkCountMax)
{
  SendingCB->Checked = true ;
}
//---------------------------------------------------------------------------

void __fastcall TIndySMTP::OnSmtpWorkEnd(TObject *Sender, TWorkMode AWorkMode)
{
  SentCB->Checked = true ;

  string sMessage = string("SMTPSuccess Message sent to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSubDetails) ;

  MessageText->Caption = "Message sent" ;

  _bAttendre = false ;
}
//---------------------------------------------------------------------------

void TIndySMTP::Connect()
{
try
{
  char cPort[80] ;
  itoa(pMail->getPortSmtp(), cPort, 10) ;

  string sUrlSmtp  = pMail->getUrlSmtp() ;
  string sUserSmtp = pMail->getUserSmtp() ;

  HostName->Caption = sUrlSmtp.c_str() ;
  UserText->Caption = sUserSmtp.c_str() ;
  FromText->Caption = (pMail->getFrom()).c_str() ;

  MessageText->Caption = "Connexion..." ;

  SMTP->Host        = AnsiString(sUrlSmtp.c_str()) ;
  SMTP->Port        = pMail->getPortSmtp() ;
  SMTP->Username    = AnsiString(sUserSmtp.c_str()) ;

  SMTP->AuthenticationType = atNone ;
  if (pMail->isPassNeeded())
  {
    SMTP->AuthenticationType = atLogin ;
    SMTP->Password           = AnsiString((pMail->getPassSmtp()).c_str()) ;
  }

  int iTimeOut = 60000 ;
  SMTP->Connect(iTimeOut) ;

  _bConnected = SMTP->Connected() ;

  // Send will also call Authenticate before transmission of an E-Mail message,
  // if the SMTP session has not been authenticated.
  //
  // if (pMail->isPassNeeded())
  //   _bAuthenticated = SMTP->Authenticate() ;

  string sMessage = string("SMTP Connected to ") + pMail->getUrlSmtp() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSubDetails) ;

  MessageText->Caption = "Connexion OK" ;
}
catch (EIdAlreadyConnected& e)
{
  string sMessage = string("Déjà connecté au serveur SMTP ") + pMail->getUrlSmtp() + string(" : ") + string(e.Message.c_str()) ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trWarning) ;
}
/*
catch (ESockError& e)
{
	string sMessage = string("Connexion impossible au serveur SMTP ") + pMail->sUrlSMTP + string(" : ") + string(e.Message.c_str()) ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  erreur(sMessage.c_str(), standardError, 0) ;
}
catch (ESocketError& e)
{
	string sMessage = string("Connexion impossible au serveur SMTP ") + pMail->sUrlSMTP + string(" : ") + string(e.Message.c_str()) ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  erreur(sMessage.c_str(), standardError, 0) ;
}
*/
catch (...)
{
  string sMessage = string("Connexion impossible au serveur SMTP ") + pMail->getUrlSmtp() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
	erreur(sMessage.c_str(), standardError, 0) ;
}
}

void TIndySMTP::Disconnect(){
  if (false == _bConnected)
    return ;

	MessageText->Caption = "Déconnexion..." ;
	SMTP->Disconnect() ;
}

boolTIndySMTP::Verify(){
	// astuce pour déclencher OnConnexionRequired
	// si on n'est pas connecté à internet
	return true ; // (SMTP->Verify((pMail->sUserSMTP).c_str())) ;
}

void
TIndySMTP::InitMessage(const string sSender, const string sNameFrom,                          const string sMailFrom, const string sReplyTo,                          const string sToAdr, const string sObjet){
  Message->From->Address              = AnsiString(sMailFrom.c_str()) ;
  Message->From->Name                 = AnsiString(sNameFrom.c_str()) ;
  Message->Sender->Address            = AnsiString(sSender.c_str()) ;

  Message->ReplyTo->Add()->Address    = AnsiString(sReplyTo.c_str()) ;

  Message->Recipients->Add()->Address = AnsiString(sToAdr.c_str()) ;

  Message->Subject                    = AnsiString(sObjet.c_str()) ;
}

voidTIndySMTP::ComposeMessage(const string sSender, const string sNameFrom,                          const string sMailFrom, const string sReplyTo,                          const string sToAdr, const string sObjet,                          const string sCodeLettre){
  NSDocumentInfo* 	pDocTtxInfo ;
  string					  sPath ;
  string					  sNomDoc ;
  char					    fichLettre[255] ;
  string            sFichLettre ;
  char					    msg[255] ;
  char far			 	  texte[2048] ;

  encours = AnsiString(sToAdr.c_str()) ;
  TextTo->Caption = encours ;

  InitMessage(sSender, sNameFrom, sMailFrom, sReplyTo, sToAdr, sObjet) ;

  // composition du Body en fonction du code lettre du document
  if (string("") != sCodeLettre)
  {
    NSDocumentInfo docInfo(sCodeLettre, pContexte, pContexte->getPatient()->getGraphPerson()) ;
    docInfo.InitDocumentBrut(&pDocTtxInfo) ;
    sPath = pContexte->PathName(pDocTtxInfo->getLocalis()) ;
    sFichLettre = sPath + pDocTtxInfo->getFichier() ;
    ConvertRTFDialog convertDlg(pContexte, pContexte->GetMainWindow(), sFichLettre.c_str()) ;
    convertDlg.Create() ;
    convertDlg.getRichEdit()->GetText(texte, 2048) ;

    Message->Body->SetText(texte) ;

    convertDlg.CmOk() ;
  }
  else
  {
    sNomDoc = pDoc->_pDocInfo->getDocName() ;

    if (string("") == sNomDoc)
    {    	sprintf(msg, "le document concernant %s %s.",              pContexte->getPatient()->getszNom(),
            pContexte->getPatient()->getszPrenom()) ;
    }
    else    {
    	sprintf(msg, "%s %s %s.",
                sNomDoc.c_str(),
              pContexte->getPatient()->getszNom(),
            pContexte->getPatient()->getszPrenom()) ;
    }
    Message->Body->Add("Veuillez trouver en pièces jointes :") ;
    Message->Body->Add(msg) ;
  }
}

void
TIndySMTP::ComposeSimpleMessage(const string sSender, const string sNameFrom,
                                const string sMailFrom, const string sReplyTo,
                                const string sToAdr, const string sObjet,
                                const string sMessage)
{
  encours = AnsiString(sToAdr.c_str()) ;
  TextTo->Caption = encours ;

  InitMessage(sSender, sNameFrom, sMailFrom, sReplyTo, sToAdr, sObjet) ;

  Message->Body->SetText((char*)sMessage.c_str()) ;
}

void TIndySMTP::AttacheMessage(string sFileName){
  TIdAttachment *attachment = new TIdAttachment(Message->MessageParts, AnsiString(sFileName.c_str())) ;
}

void TIndySMTP::ClearMessage()
{
  Message->Clear() ;
}

void TIndySMTP::SendMail(){
try
{
  if (false == _bConnected)
    return ;
  MessageText->Caption = "Envoi du message..." ;  _bAttendre = true ;

  SMTP->Send(Message) ;}catch (Exception& e){
	string sMessage = string("Erreur à l'envoi du message à ") + string(Message->Recipients->EMailAddresses.c_str()) + string(" : ") + string(e.Message.c_str()) ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  erreur(sMessage.c_str(), standardError, 0) ;
  _bAttendre = false ;
}
catch (...)
{
  string sMessage = string("Erreur à l'envoi du message à ") + string(Message->Recipients->EMailAddresses.c_str()) ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
	erreur(sMessage.c_str(), standardError, 0) ;
  _bAttendre = false ;
}}

bool TIndySMTP::Continuer(){
  return (false == _bInterrupt) ;
}

bool TIndySMTP::Attendre(){
  pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
  return (_bAttendre) ;
}

