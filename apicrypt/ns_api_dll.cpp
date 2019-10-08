//---------------------------------------------------------------------------#include <stdio.h>
#include <sysutils.hpp>		// needed for FileExists

#include "partage\nsglobal.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
#include "nsbb\nspatpat.h"
#include "nsdn\nsdocnoy.h"
#include "nautilus\nssuper.h"
#include "ns_crypt\apicerr.h"
#include "ns_crypt\ns_api_dll.h"

/****************************************************************************//***                           CLASSE NSApicrypt                          ***/
/****************************************************************************/

long   (FAR* NSApicrypt::pAPICcrypterMem2)       (char* donnees, char** resultat, char* Username, char* keyFolderPath, char* fieldto, char* fieldcc, char* fieldbcc) = NULL ;
long   (FAR* NSApicrypt::pAPICdecrypterMem2)     (char* donnees, char** resultat, char* keyFolderPath) = NULL ;
long   (FAR* NSApicrypt::pAPICcrypter3)          (char* filename, char* targetfilename, char* Username, char* keyFolderPath, char* fieldto, char* fieldcc, char* fieldbcc, long verbose) = NULL ;
long   (FAR* NSApicrypt::pAPICdecrypter3)        (char* filename, char* targetfilename, char* keyFolderPath, long verbose) = NULL ;
void   (FAR* NSApicrypt::pAPICheminParDefaut)    (char* keypath) = NULL ;
void   (FAR* NSApicrypt::pAPIClefParDefaut)      (char* keypath, char* userkey) = NULL ;
void   (FAR* NSApicrypt::pAPICApicDisposePtr)    (char* data) = NULL ;
TypErr (FAR* NSApicrypt::pAPICrypterPieceJointe2)(char* fileNameSourceFile, char* fileNameDestFile, char* userKey, char* keypath, char* dests, long format, long verbose) = NULL ;
TypErr (FAR* NSApicrypt::pAPIDecrypterPJ)        (char* fileNameSourceFile, char* pathDestFiles, char* keypath, long verbose) = NULL ;

// Constructeur
NSApicrypt::NSApicrypt()
{
try
{
	pApiDllModule = new TModule("dllapi.dll", TRUE) ;
	if (NULL == pApiDllModule)
		erreur("can't load dllapi.dll", standardError, 0) ;
  else
	{
  	(FARPROC) pAPICcrypterMem2        = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(APIC_CRYPTER_MEM_2)) ;
    (FARPROC) pAPICdecrypterMem2      = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(APIC_DECRYPTER_MEM_2)) ;
    (FARPROC) pAPICcrypter3           = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(APIC_CRYPTER_3)) ;
    (FARPROC) pAPICdecrypter3         = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(APIC_DECRYPTER_3)) ;
    (FARPROC) pAPICheminParDefaut     = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(API_CHEMIN_PAR_DEFAUT)) ;
    (FARPROC) pAPIClefParDefaut       = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(API_CLEF_PAR_DEFAUT)) ;
    (FARPROC) pAPICApicDisposePtr     = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(APIC_APIC_DISPOSE_PTR)) ;
    // (FARPROC) pAPIClefParDefaut       = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(CURL_GLOBAL_INIT)) ;
    (FARPROC) pAPICrypterPieceJointe2 = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(API_CRYPTER_PIECE_JOINTE_2)) ;
    (FARPROC) pAPIDecrypterPJ         = pApiDllModule->GetProcAddress(MAKEINTRESOURCE(API_DECRYPTER_PJ)) ;
	}
}
catch (TXInvalidModule e)
{
	string sErreur = string("Exception NSApicrypt ctor : ") + e.why() ;
	erreur(sErreur.c_str(), standardError, 0) ;
  pApiDllModule = 0 ;
}
catch (...)
{
	erreur("Exception NSApicrypt ctor", standardError, 0) ;
}
}

// destructeurNSApicrypt::~NSApicrypt()
{
	if (pApiDllModule)
		delete pApiDllModule ;
}

string
NSApicrypt::getMailText(NSContexte far *pCtx, string sRawText, string sEMail, string sCorrespId, NSPatInfo *pPatInfo, NSNoyauDocument *pDoc)
{
  if (((NSContexte*) NULL == pCtx) || (string("") == sRawText) ||
      (string("") == sEMail) || (string("") == sCorrespId) ||
      ((NSPatInfo*) NULL == pPatInfo) || ((NSNoyauDocument*) NULL == pDoc))
  {
    string sTrace = string("NSApicrypt::getMailText Bad parameters.") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return string("") ;
  }

  string sHprimText = getHprimText(pCtx, sRawText, sEMail, sCorrespId, pPatInfo, pDoc) ;
  if (string("") == sHprimText)
  {
    string sTrace = string("NSApicrypt::getMailText Empty hprim text.") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return string("") ;
  }

  bool bUseApicryptProxy = pCtx->getUtilisateur()->getMailApicrypt()->mustUseApiProxy() ;
  if (bUseApicryptProxy)
  {
    string sTrace = string("Sending non crypted Apicrypt message because parameter UseApiProxy says we are behind a crypting proxy") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    return sHprimText ;
  }

  long lResult = CrypteMessages(pCtx, &sHprimText, &sEMail) ;
  if (0 != lResult)
  {
    string sTrace = string("NSApicrypt::getMailText Message crypting failed") ;

    string sApicryptError = getHprimError(lResult) ;
    if (string("") != sApicryptError)
      sTrace += string(" (") + sApicryptError + string(").") ;
    else
      sTrace += string(".") ;

    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return string("") ;
  }
  else if (string("") == sHprimText)
  {
    string sTrace = string("NSApicrypt::getMailText Message crypting failed (no error but empty result).") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
  }

  return sHprimText ;
}

string
NSApicrypt::getHprimText(NSContexte far *pCtx, string sRawText, string sEMail, string sCorrespId, NSPatInfo *pPatInfo, NSNoyauDocument *pDoc)
{
  if (((NSContexte*) NULL == pCtx) || (string("") == sRawText) ||
      (string("") == sEMail) || (string("") == sCorrespId) ||
      ((NSPatInfo*) NULL == pPatInfo) || ((NSNoyauDocument*) NULL == pDoc))
    return string("") ;

  string sMailName = string("") ;
  donneAdresseUtile(sEMail, sMailName) ;

  string sHprimHeader = getHprimHeader(pCtx, sMailName, sCorrespId, pPatInfo, pDoc) ;
  if (string("") == sHprimHeader)
  {
    string sTrace = string("NSApicrypt::getHprimText Empty hprim text.") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return string("") ;
  }

  string sHprimText = sHprimHeader + sRawText ;

  sHprimText += string("****FIN****") + string(NEWLINE) ;
  sHprimText += string("****FINFICHIER****") + string(NEWLINE) ;

  return sHprimText ;
}

long
NSApicrypt::CrypteMessages(NSContexte far *pCtx, string far *pMessage, string far *pAdresse)
{
  if (((NSContexte*) NULL == pCtx) ||
      ((string*) NULL == pMessage) || (string("") == *pMessage) ||
      ((string*) NULL == pAdresse) || (string("") == *pAdresse))
    return -1 ;

  NSMailParams* pMailApicrypt = pCtx->getUtilisateur()->getMailApicrypt() ;

  string sApicryptUserName = pMailApicrypt->getApiUser() ;
  if (string("") == sApicryptUserName)
  {
    string sTrace = string("NSApicrypt::CrypteMessages Empty Apicrypt user name.") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return -1 ;
  }

  string sApicryptKeyDir = pMailApicrypt->getApiKeyDir() ;
  if (string("") == sApicryptKeyDir)
  {
    string sTrace = string("NSApicrypt::CrypteMessages Empty Apicrypt key dir.") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return -1 ;
  }
  else
  {
    string sTrace = string("Apicrypt key dir: ") + sApicryptKeyDir ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  }

  string sDestinataire ;
  donneAdresseUtile(*pAdresse, sDestinataire) ;

  char* resultat ;
  long resulterr = (*pAPICcrypterMem2)((char*)(pMessage->c_str()),
                                       &resultat,
                                       (char*)(sApicryptUserName.c_str()),
                                       (char*)(sApicryptKeyDir.c_str()),
                                       (char*)(sDestinataire.c_str()),
                                       "",
                                       "") ;
  if (kNoErr != resulterr)
    return resulterr ;

  *pMessage = string(resultat) ;

  return 0 ;
}

longNSApicrypt::CrypteFichiers(NSContexte far *pCtx, string far *pNomFich, string far *pAdresse){  if ((NULL == pCtx) || (NULL == pNomFich) || (string("") == *pNomFich) ||                        (NULL == pAdresse) || (string("") == *pAdresse))    return -1 ;

  NSMailParams* pMailApicrypt = pCtx->getUtilisateur()->getMailApicrypt() ;

  string sApicryptUserName = pMailApicrypt->getApiUser() ;
  if (string("") == sApicryptUserName)
  {
    string sTrace = string("NSApicrypt::CrypteMessages Empty Apicrypt user name.") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return -1 ;
  }

  string sApicryptKeyDir = pMailApicrypt->getApiKeyDir() ;
  if (string("") == sApicryptKeyDir)
  {
    string sTrace = string("NSApicrypt::CrypteMessages Empty Apicrypt key dir.") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return -1 ;
  }
  else
  {
    string sTrace = string("Apicrypt key dir: ") + sApicryptKeyDir ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  }
  string sUtiliMail = pCtx->getUtilisateur()->getMail() ;  donneAdresseUtile(sUtiliMail, sUtiliMail) ;

  string sDestinataire ;
  donneAdresseUtile(*pAdresse, sDestinataire) ;

  //
  // Le fichier toto.txt, crypté s'appelle maintenant toto.txt.api
  //
  string sNouvNom = *pNomFich + string(".api") ;

  char* targetFileName ;

  long resulterr = (*pAPICcrypter3)((char*)(pNomFich->c_str()),
                                    (char*)(sNouvNom.c_str()),
                                    (char*)(sApicryptUserName.c_str()),
                                    (char*)(sApicryptKeyDir.c_str()),
                                    (char*)(sDestinataire.c_str()),
                                    "",
                                    "",
                                    long(0)) ;
  // if (kNoErr != resulterr)
    return resulterr ;
}longNSApicrypt::DecrypteMessages(NSContexte far *pCtx, string far *pMessage, string far *pAdresse){}long
NSApicrypt::DecrypteFichiers(NSContexte far *pCtx, string far *pNomFich, string far *pAdresse)
{
}

void
NSApicrypt::donneAdresseUtile(const string sAdresseLongue, string& sAdresseUtile)
{
  size_t posAt = sAdresseLongue.find('@') ;
  if (posAt)
    sAdresseUtile = string(sAdresseLongue, 0, posAt) ;
  else
    sAdresseUtile = sAdresseLongue ;
}

string
NSApicrypt::getMailName(const string sEMail)
{
  string sMailName = string("") ;
  donneAdresseUtile(sEMail, sMailName) ;
  return sMailName ;
}

string
NSApicrypt::getMailProvider(string sEMail)
{
  if (string("") == sEMail)
    return string("") ;

  size_t iMailLength = strlen(sEMail.c_str()) ;

  size_t iPosAt = sEMail.find('@') ;
  if ((NPOS == iPosAt) || (iPosAt == iMailLength - 1))
    return string("") ;

  return string(sEMail, iPosAt + 1, iMailLength - iPosAt -1) ;
}

bool
NSApicrypt::isApicryptAddress(const string sEMail)
{
  if (string("") == sEMail)
    return false ;

  // For debug mode only
  // if (string("philippe.ameline@free.fr") == sEMail)
  //  return true ;

  string sMailProvider = getMailProvider(sEMail) ;

  size_t iPos = sMailProvider.find("apicrypt.org") ;

  return (NPOS != iPos) ;
}

string
NSApicrypt::getHprimHeader(NSContexte far *pCtx, string sMailName, string sCorrespId, NSPatInfo *pPatInfo, NSNoyauDocument *pDoc)
{
  if (((NSContexte*) NULL == pCtx) || (string("") == sMailName) ||
      (string("") == sCorrespId) || ((NSPatInfo*) NULL == pPatInfo) ||
      ((NSNoyauDocument*) NULL == pDoc))
  {
    string sTrace = string("NSApicrypt::getHprimHeader Bad parameters.") ;
    pCtx->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return string("") ;
  }

  string sHeader = pPatInfo->getNss() + string(NEWLINE) ;
  sHeader += pPatInfo->getNom()    + string(NEWLINE) ;
  sHeader += pPatInfo->getPrenom() + string(NEWLINE) ;

  // Address, birthday and NSS
  //
  string sAddressHeader = getHprimHeaderAddress(pCtx, pPatInfo) ;
  if (string("") != sAddressHeader)
    sHeader += sAddressHeader ;
  else
    sHeader += string(NEWLINE) + string(NEWLINE) + string(NEWLINE) + string(NEWLINE) + string(NEWLINE) ;

  // Document code
  //
  sHeader += pDoc->_pDocInfo->getCodeDocMeta() + string(NEWLINE) ;

  // Document date
  //
  string sExamDate = pDoc->GetDateExamen(false) ;
  string sHprimExamDate = getHprimHeaderDate(sExamDate) ;

  sHeader += sHprimExamDate + string(NEWLINE) ;

  // Sender
  //
  NSUtilisateurChoisi *pUser = pCtx->getUtilisateur() ;
  string sUserId  = pUser->getNss() ;
  string sUserApi = pUser->getMailApicrypt()->getApiUser() ;

  sHeader += sUserId + string(11 - strlen(sUserId.c_str()), ' ') + getMailName(sUserApi) + string(NEWLINE) ;

  // Destination
  //
  sHeader += sCorrespId + string(11 - strlen(sCorrespId.c_str()), ' ') + sMailName + string(NEWLINE) ;

  return sHeader ;
}

string
NSApicrypt::getHprimHeaderAddress(NSContexte far *pCtx, NSPatInfo *pPatInfo)
{
  if (((NSContexte*) NULL == pCtx) || ((NSPatInfo*) NULL == pPatInfo))
    return string("") ;

  // Get demographics tree
  //
  NSPersonGraphManager* pGraphPerson = pPatInfo->getGraphPerson() ;
  if (((NSPersonGraphManager*) NULL == pGraphPerson) || (NULL == pGraphPerson->getDataGraph()))
    return string("") ;

  NSPatPathoArray PatPathoArray(pCtx->getSuperviseur()) ;
  NSDataTreeIter iterTree ;

  if (false == pGraphPerson->getDataGraph()->getTrees()->ExisteTree("ZADMI1", pCtx->getSuperviseur(), &iterTree))
    return string("") ;

  (*iterTree)->getRawPatPatho(&PatPathoArray) ;
  if (PatPathoArray.empty())
    return string("") ;

  // Get address parts
  //
  string addressHeader = string("") ;

  PatPathoIter iter ;

  if (PatPathoArray.CheminDansPatpatho("ZADMI1/LCOOR1/LADRE1/LADL11/£CL000", string(1, cheminSeparationMARK), &iter))
    addressHeader += (*iter)->getTexteLibre() ;

  addressHeader += string(NEWLINE) ;

  if (PatPathoArray.CheminDansPatpatho("ZADMI1/LCOOR1/LADRE1/LADL21/£CL000", string(1, cheminSeparationMARK), &iter))
    addressHeader += (*iter)->getTexteLibre() ;

  addressHeader += string(NEWLINE) ;

  if (PatPathoArray.CheminDansPatpatho("ZADMI1/LCOOR1/LADRE1/LVILL1/LZIP01/£CL000", string(1, cheminSeparationMARK), &iter))
    addressHeader += (*iter)->getTexteLibre() ;

  if (PatPathoArray.CheminDansPatpatho("ZADMI1/LCOOR1/LADRE1/LVILL1/LCOMU1/£CL000", string(1, cheminSeparationMARK), &iter))
    addressHeader += string(" ") + (*iter)->getTexteLibre() ;

  addressHeader += string(NEWLINE) ;

  // Birthday
  //
  string sBirthday = pPatInfo->getNaissance() ;
  string sHprimBirthday = getHprimHeaderDate(sBirthday) ;

  addressHeader += sHprimBirthday + string(NEWLINE) ;

  // NSS
  //
  if (PatPathoArray.CheminDansPatpatho("ZADMI1/LFRAN1/LFRAB1/£CL000", string(1, cheminSeparationMARK), &iter))
    addressHeader += (*iter)->getTexteLibre() ;
  if (PatPathoArray.CheminDansPatpatho("ZADMI1/LFRAN1/LFRAV1/£CL000", string(1, cheminSeparationMARK), &iter))
    addressHeader += string(" ") + (*iter)->getTexteLibre() ;

  addressHeader += string(NEWLINE) ;

  return addressHeader ;
}

string
NSApicrypt::getHprimHeaderDate(string sDate)
{
  if ((string("") == sDate) || (strlen(sDate.c_str()) < 8))
    return string("") ;

  string sHprimDate = string(sDate, 6, 2) + string("/") +
                      string(sDate, 4, 2) + string("/") +
                      string(sDate, 0, 4) ;

  return sHprimDate ;
}

string
NSApicrypt::getHprimError(long lErrorCode)
{
  if  (kOtherErr   == lErrorCode)
    return string("") ;

  if  (kFNFErr     == lErrorCode)
    return string("file not found") ;
  if (kFileClosed == lErrorCode)
    return string("file closed") ;
  if (kCRCErr     == lErrorCode)
    return string("CRCErr during asciibin") ;
  if (kPbErr      == lErrorCode)
    return string("problem when uncrypting") ;

  if (kFrameTooShort == lErrorCode)
    return string("frame too short") ;
  if (kFrameTooLong  == lErrorCode)
    return string("frame too long") ;
  if (KeyNotFoundErr == lErrorCode)
    return string("clef de décryptage not found") ;
  if (kBadKeyHeader  == lErrorCode)
    return string("corrupted key header") ;
  if (kMemErr        == lErrorCode)
    return string("memory error") ;
  if (kOffsetOutOfRangeErr == lErrorCode)
    return string("key offset out of range") ;

  return string("") ;
}

