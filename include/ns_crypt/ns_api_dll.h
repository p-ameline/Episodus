//-------------------------------------------------------------------------//                   HEADER FILE : NS_API_DLL.H
//
//                   Interface with Apicrypt
//
//             Initial release : November 17 2010 (PA)
//             Final release   : November 17 2010 (PA)
//-------------------------------------------------------------------------
#ifndef __NS_API_DLL_H
#define __NS_API_DLL_H

#include <owl\module.h>

#include "ns_crypt\apicerr.h"
#include "nsbb\NTFunctor.h"

// dllapi Functions
//

#define APIC_APIC_DISPOSE_PTR        1  // APICApicDisposePtr
#define APIC_CRYPTER_3               4  // APICcrypter3
#define APIC_CRYPTER_MEM_2           6  // APICcrypterMem2
#define APIC_DECRYPTER_3            12  // APICdecrypter3
#define APIC_DECRYPTER_MEM_2        14  // APICdecrypterMem2
#define API_CHEMIN_PAR_DEFAUT       17  // APICheminParDefaut
#define API_CLEF_PAR_DEFAUT         18  // APIClefParDefaut
#define API_CRYPTER_PIECE_JOINTE_2  20  // APICrypterPieceJointe2
// #define APIDecrypterMem4  21
#define API_DECRYPTER_PJ            22  // APIDecrypterPJ
// #define APIGenererAccuse  23

#if defined(_APICRYPT_DLL)
	#define _APICLASSE __export
#else
	#define _APICLASSE __import
#endif

class _APICLASSE NSApicrypt
{
	public :

		TModule* pApiDllModule ;

		NSApicrypt() ;
		~NSApicrypt() ;

    string getMailText(NSContexte far *pCtx, string sRawText, string sEMail, string sCorrespId, NSPatInfo *pPatInfo, NSNoyauDocument *pDoc) ;
    string getHprimText(NSContexte far *pCtx, string sRawText, string sEMail, string sCorrespId, NSPatInfo *pPatInfo, NSNoyauDocument *pDoc) ;

    long   CrypteMessages(NSContexte far *pCtx, string far *pMessage, string far *pAdresse) ;
    long   CrypteFichiers(NSContexte far *pCtx, string far *pNomFich, string far *pAdresse) ;    long   DecrypteMessages(NSContexte far *pCtx, string far *pMessage, string far *pAdresse) ;    long   DecrypteFichiers(NSContexte far *pCtx, string far *pNomFich, string far *pAdresse) ;    static void   donneAdresseUtile(const string sAdresseLongue, string& sAdresseUtile) ;    static string getMailName(const string sEMail) ;    static string getMailProvider(const string sEMail) ;    static bool   isApicryptAddress(const string sEMail) ;    string getHprimHeader(NSContexte far *pCtx, string sEMail, string sCorrespId, NSPatInfo *pPatInfo, NSNoyauDocument *pDoc) ;    string getHprimHeaderAddress(NSContexte far *pCtx, NSPatInfo *pPatInfo) ;    string getHprimHeaderDate(string sNauDate) ;    string getHprimText(string sHtmlText) ;    string getHprimError(long lErrorCode) ;    // Funtions pointers    //

    // cryptage de texte en memoire :
    //

    // idem crypterMem mais pouvant fournir un path vers le dossier des clefs
    static long (FAR* pAPICcrypterMem2)(char* donnees, char** resultat, char* Username, char* keyFolderPath, char* fieldto, char* fieldcc, char* fieldbcc) ;
    // idem decrypterMem mais pouvant fournir un path vers le dossier des clefs
    static long (FAR* pAPICdecrypterMem2)(char* donnees, char** resultat, char* keyFolderPath) ;


    // cryptage de texte via fichier :
    //

    // idem crypter2 mais pouvant fournir le chemin du fichier d'arrivee des donnees cryptees
    static long (FAR* pAPICcrypter3)(char* filename, char* targetfilename, char* Username, char* keyFolderPath, char* fieldto, char* fieldcc, char* fieldbcc, long verbose) ;
    // idem decrypter2 mais pouvant fournir le nom du fichier de destination
    static long (FAR* pAPICdecrypter3)(char* filename, char* targetfilename, char* keyFolderPath, long verbose) ;

    // utilitaires windows :
    //
    static void (FAR* pAPICheminParDefaut)(char* keypath) ;
    static void (FAR* pAPIClefParDefaut)(char* keypath, char* userkey) ;

    // pour desallouer la memoire d'un block cree par une fonction apicrypt
    static void (FAR* pAPICApicDisposePtr)(char* data) ;

    // cryptage de texte via fichier :
    //

    // crypte n fichiers
    //
    // fileNameSourceFile (chemins separes par des "|" ex: "./test.jpg|/volumes/data/test.doc" ou avec "\" si sur pc ou ":" si os9)
    // fileNameDestFile fichier de destination devant avoir une extension .apz
    // keypath est le chemin d'acces au dossier "Clefs", sur PC : "C:\\apicrypt\\"
    // userkey : clef a utiliser pour crypter sans l'extension .xxx ou xxx est un nombre correspondant a annee - 1900
    // dests : contient la concatenation de tous les destinataires separes par des virgules, ex : jean.dupont,maurice.durant
    // format = 0 : non compressee
    // format = 2 : comression LZO si autorisee dans local.h
    //
    static TypErr (FAR* pAPICrypterPieceJointe2)(char* fileNameSourceFile, char* fileNameDestFile, char* userKey, char* keypath, char* dests, long format, long verbose) ;

    // decrypte une ou plusieurs pieces jointes contenues dans un .apz
    //
    // fileNameSourceFile = fichier .apz a traiter
    // pathDestFiles = dossier d'arrivee pour les pieces jointes decryptees doit se terminer par "\" imperativement sur pc ou "/" sous unix ou ":" sur os9
    // keypath est le chemin d'acces au dossier "Clefs", sur PC : "C:\\apicrypt\\"
    //
    static TypErr (FAR* pAPIDecrypterPJ)(char* fileNameSourceFile, char* pathDestFiles, char* keypath, long verbose) ;
} ;

#endif 		// __NS_API_DLL_H

