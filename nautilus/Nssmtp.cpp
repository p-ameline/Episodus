//---------------------------------------------------------------------------#include <vcl.h>
#pragma hdrstop

#include "Nssmtp.h"#include "scktcomp.hpp"
#include "nautilus\nschoisi.h"      // fait un include de nsdocref.h
#include "nautilus\ns_html.h"       // pour ConvertRTFDialog
#include "nautilus\nsdocref.h"       
#include "nautilus\nssuper.h"       // pour NSContexte

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TControlSMTP *ControlSMTP;

//---------------------------------------------------------------------------
__fastcall TControlSMTP::TControlSMTP(TComponent* Owner)
                        :TForm(Owner)
{
  bInterrupt = false ;
  bAttendre  = false ;
  bConnected = false ;
  bApicrypt  = false ;
}

__fastcall TControlSMTP::TControlSMTP(TComponent* Owner, NSContexte* ctx,                            NSRefDocument* doc, NSMailParams* mail) : TForm(Owner)
{
  // on ne fait pas de copie : tous les pointeurs sont résidents
  pContexte   = ctx ;
  pDoc        = doc ;
  pMail       = mail ;
  encours     = "(vide)" ;

  bInterrupt  = false ;
  bAttendre   = false ;
  bConnected  = false ;
  bApicrypt   = false ;

  StatusBar->SimplePanel = true ;
  StatusBarServeur->SimplePanel = true ;
}

__fastcall TControlSMTP::~TControlSMTP()
{
    // ne pas deleter les pointeurs !
}

void __fastcall TControlSMTP::AnnulButtonClick(TObject *Sender)
{
  bInterrupt = true ;
}

//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPConnect(TObject *Sender)
{
  // Evénement appelé après Connect() lorsque la connexion est OK
  string sMessage = string("SMTPConnect for message sent to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSubDetails) ;
  StatusBar->SimpleText = "Connexion OK" ;
  bConnected = true ;
}

//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPConnectionFailed(TObject *Sender)
{
  // en réponse à Connect()
  string sMessage = string("SMTPConnectionFailed for message to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  StatusBar->SimpleText = "Echec de la connexion" ;
  bInterrupt = true ;
  bConnected = false ;
}

//---------------------------------------------------------------------------
void __fastcall TControlSMTP::SMTPAuthentificationFailed(bool &handled)
{
  string sMessage = string("SMTPAuthentificationFailed for message to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  StatusBar->SimpleText = "Echec de l'authentification" ;
  handled    = false ;
  bInterrupt = true ;
  bConnected = false ;
}

//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPInvalidHost(bool &handled)
{
  // en réponse à Connect()
  string sMessage = string("SMTPInvalidHost for message to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  StatusBar->SimpleText = "Nom d'hôte SMTP invalide" ;
  handled    = false ;
  bInterrupt = true ;
  bConnected = false ;
}

//---------------------------------------------------------------------------
void __fastcall TControlSMTP::SMTPRecipientNotFound(AnsiString Recipient)
{
  string sMessage = string("SMTPRecipientNotFound for message to ") + encours.c_str() + string(" for recipient ") + Recipient.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  StatusBar->SimpleText = "Destinataire non trouvé" ;
  bInterrupt = true ;
  bConnected = false ;
}

//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPDisconnect(TObject *Sender)
{
  string sMessage = string("SMTPDisconnect for message sent to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSubDetails) ;
  // Evénement appelé après Disconnect() lorsque la déconnexion est OK
  // if (StatusBar != 0)  -- à tester pour cet évènement --
  //      StatusBar->SimpleText = "Déconnecté."
  bConnected = false ;
}

//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPConnectionRequired(bool &handled)
{
  // en réponse à SendMail()
  string sMessage = string("SMTPConnectionRequired for message to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  StatusBar->SimpleText = "Connexion non établie." ;
  handled    = false ;
  bInterrupt = true ;
}

//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPSendStart(TObject *Sender)
{
  string sMessage = string("SMTPSendStart for message sent to ") + encours.c_str() ;

  if (bApicrypt)
  {
    SMTP->FinalHeader->Values["Content-Type"] = "text/plain; charset=\"iso-8859-1\" ; format=\"flowed\"" ;
    SMTP->FinalHeader->Values["Content-Transfer-Encoding"] = "7bit" ;

    sMessage = string(" with Apicrypt specific header") ;
  }
  
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSubDetails) ;
}

//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPFailure(TObject *Sender)
{
  string sMessage = string("SMTPFailure for message to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  ShowMessage("Impossible d'envoyer le message à l'adresse " + encours) ;
  bAttendre = false ;
}
//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPSuccess(TObject *Sender)
{
  string sMessage = string("SMTPSuccess Message sent to ") + encours.c_str() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSubDetails) ;
  StatusBar->SimpleText = "Message envoyé" ;
  bAttendre = false ;
}

//---------------------------------------------------------------------------void __fastcall TControlSMTP::SMTPStatus(TComponent *Sender, AnsiString Status)
{
  if (0 != StatusBarServeur)
    StatusBarServeur->SimpleText = Status ;}
//---------------------------------------------------------------------------
void TControlSMTP::Connect(){
try
{
  char cPort[80] ;

  itoa(pMail->getPortSmtp(), cPort, 10) ;

  EditHost->Text = (pMail->getUrlSmtp()).c_str() ;
  EditPort->Text = cPort ;
  EditUser->Text = (pMail->getUserSmtp()).c_str() ;
  EditExp->Text  = (pMail->getSender()).c_str() ;
  StatusBar->SimpleText = "Connexion..." ;

  SMTP->Host        = AnsiString((pMail->getUrlSmtp()).c_str()) ;
  SMTP->Port        = pMail->getPortSmtp() ;
  SMTP->UserID      = AnsiString((pMail->getUserSmtp()).c_str()) ;
  SMTP->ReportLevel = 3 ;
  SMTP->TimeOut     = 60000 ;
  // SMTP->EncodeType = uuMime ;
  SMTP->Connect() ;
}
catch (ESockError& e)
{
	string sMessage = string("Connexion impossible au serveur SMTP ") + pMail->getUrlSmtp() + string(" : ") + string(e.Message.c_str()) ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  erreur(sMessage.c_str(), standardError, 0) ;
}
catch (ESocketError& e)
{
	string sMessage = string("Connexion impossible au serveur SMTP ") + pMail->getUrlSmtp() + string(" : ") + string(e.Message.c_str()) ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  erreur(sMessage.c_str(), standardError, 0) ;
}
catch (...)
{
  string sMessage = string("Connexion impossible au serveur SMTP ") + pMail->getUrlSmtp() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
	erreur(sMessage.c_str(), standardError, 0) ;
}
}

void TControlSMTP::Disconnect(){
	StatusBar->SimpleText = "Déconnexion..." ;
	SMTP->Disconnect() ;
}

boolTControlSMTP::Verify(){
	// astuce pour déclencher OnConnexionRequired
	// si on n'est pas connecté à internet
	return (SMTP->Verify((pMail->getUserSmtp()).c_str())) ;
}

voidTControlSMTP::ComposeMessage(string sFromAdr, string sToAdr, string sObjet,                    string sCodeLettre)
{
  NSDocumentInfo* 	pDocTtxInfo ;
  string					  sPath ;
  string					  sNomDoc ;
  char					    fichLettre[255] ;
  string            sFichLettre ;
  char					    msg[255] ;
  char far			 	  texte[2048] ;

  encours = AnsiString(sToAdr.c_str()) ;
  EditDest->Text = encours ;

  SMTP->ClearParameters() ;
  SMTP->PostMessage->FromAddress = AnsiString(sFromAdr.c_str()) ;
  SMTP->PostMessage->ToAddress->Add(AnsiString(sToAdr.c_str())) ;
  SMTP->PostMessage->Subject = AnsiString(sObjet.c_str()) ;

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
    SMTP->PostMessage->Body->SetText(texte) ;
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
    SMTP->PostMessage->Body->Add("Veuillez trouver en pièces jointes :") ;
    SMTP->PostMessage->Body->Add(msg) ;
  }
}

void TControlSMTP::ComposeSimpleMessage(string sFromAdr, string sToAdr, string sObjet, string sMessage)
{
  encours = AnsiString(sToAdr.c_str()) ;
  EditDest->Text = encours ;

  SMTP->ClearParameters() ;
  SMTP->PostMessage->FromAddress = AnsiString(sFromAdr.c_str()) ;
  SMTP->PostMessage->ToAddress->Add(AnsiString(sToAdr.c_str())) ;
  SMTP->PostMessage->Subject = AnsiString(sObjet.c_str()) ;
  SMTP->PostMessage->Body->SetText((char*)sMessage.c_str()) ;
}

void TControlSMTP::AttacheMessage(string sFileName){
  SMTP->PostMessage->Attachments->Add(AnsiString(sFileName.c_str())) ;
}

void TControlSMTP::SendMail(){
try
{
  if (false == bConnected)
    return ;
  StatusBar->SimpleText = "Envoi du message..." ;  bAttendre = true ;

  SMTP->SendMail() ;}catch (Exception& e){
	string sMessage = string("Erreur à l'envoi du message à ") + SMTP->PostMessage->ToAddress->GetText() + string(" : ") + string(e.Message.c_str()) ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
  erreur(sMessage.c_str(), standardError, 0) ;
  bAttendre = false ;
}                                
catch (...)
{
  string sMessage = string("Erreur à l'envoi du message à ") + SMTP->PostMessage->ToAddress->GetText() ;
  pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
	erreur(sMessage.c_str(), standardError, 0) ;
  bAttendre = false ;
}}

bool TControlSMTP::Continuer(){
  return (false == bInterrupt) ;
}

bool TControlSMTP::Attendre(){
  return (bAttendre) ;
}

// fin de Nssmtp.cpp
