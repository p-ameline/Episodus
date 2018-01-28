
#include "local.h"
// cryptage de texte en memoire :

// idem crypterMem mais pouvant fournir un path vers le dossier des clefs
long APICcrypterMem2(char* donnees,char** resultat, char* Username, char* keyFolderPath,char* fieldto,char* fieldcc,char* fieldbcc);

// idem decrypterMem mais pouvant fournir un path vers le dossier des clefs
long APICdecrypterMem2(char* donnees,char** resultat,char* keyFolderPath);


// cryptage de texte via fichier :

// idem crypter2 mais pouvant fournir le chemin du fichier d'arrivee des donnees cryptees
long APICcrypter3(char* filename, char* targetfilename, char* Username,char* keyFolderPath,char* fieldto,char* fieldcc,char* fieldbcc,long verbose);

// idem decrypter2 mais pouvant fournir le nom du fichier de destination
long APICdecrypter3(char* filename,char* targetfilename,char* keyFolderPath,long verbose);

// utilitaires windows :
void APICheminParDefaut(char* keypath);
void APIClefParDefaut(char* keypath, char* userkey);

void APICApicDisposePtr(char* data);
// pour desallouer la memoire d'un block cree par une fonction apicrypt



void GetUserKeyName(char* userkey,char* chemin);
void APICheminParDefaut(char* keypath);
void APIClefParDefaut(char* keypath, char* userkey);
TypErr APICrypterPieceJointe2(char* fileNameSourceFile,char* fileNameDestFile,char* userKey,char* keypath,char* dests,long format,long verbose);
//crypte n fichiers fileNameSourceFile (chemins separes par des "|" ex: "./test.jpg|/volumes/data/test.doc" ou avec "\" si sur pc ou ":" si os9)
// dans un fichier de destination fileNameDestFile devant avoir une extension .apz
//keypath est le chemin d'accès au dossier "Clefs", sur PC : "C:\\apicrypt\\"
//userkey : clef à utiliser pour crypter sans l'extension .xxx où xxx est un nombre correspondant à année - 1900
//dests : contient la concatenation de tous les destinataires separes par des virgules, ex : jean.dupont,maurice.durant
//format = 0 : non compressee
//format = 2 : comression LZO si autorisee dans local.h

TypErr 	APIDecrypterPJ(char* fileNameSourceFile,char* pathDestFiles,char* keypath,long verbose);
//décrypte une ou plusieurs pièces jointes contenues dans un .apz 
//fileNameSourceFile = fichier .apz à traiter
//pathDestFiles = dossier d'arrivée pour les pièces jointes décryptées doit se terminer par "\" impérativement sur pc ou "/" sous unix ou ":" sur os9
//keypath est le chemin d'accès au dossier "Clefs", sur PC : "C:\\apicrypt\\"




// ---------------------obsolete, conserve pour raison de compatibilite ascendante

// utilisŽ pour crypter en mŽmoire des donnŽes sans la couche binascii
long APICcrypterbinaireMem2(char* SourceData,unsigned long SourceLen,char** data,unsigned long* datalen, char* Username, char* keyFolderPath,long verbose);

// idem decrypterMem2 mais pouvant fournir un path vers le dossier des clefs et dŽcryptant un block binaire en mŽmoire et non en fichier
long APICdecrypterbinaireMem2(char* SourceData,unsigned long SourceLen,char** data,unsigned long* datalen, char* keyFolderPath,long verbose);

// utilisŽ pour crypter des fichiers sans la couche binascii (objets de type pieces jointes par exemple)
long APICcrypterbinaire2(char* filename, char* targetfilename, char* Username, char* keyFolderPath,long verbose);
long APICdecrypterbinaire(char* filename, char* keyFolderPath,long verbose);


// utilisŽ pour crypter des fichiers sans la couche binascii (objets de type pieces jointes par exemple)
long APICcrypterbinaire(char* filename, char* Username, char* keyFolderPath,long verbose);

// idem "crypter" mais pouvant fournir un path vers le dossier des clefs
long APICcrypter2(char* filename, char* Username,char* keyFolderPath,char* fieldto,char* fieldcc,char* fieldbcc,long verbose);

long APICcrypter(char* filename, char* Username,char* fieldto,char* fieldcc,char* fieldbcc);
// fieldto, fieldcc et fiedbcc contiennent les adresses emails des destinataires autorisŽs du messages
// uniquement le dŽbut du email est utilisŽ, c'est ˆ dire ce qui prŽcde le @
// par exemple, si le champ to contient 2 destinataires "maurice@test.com, marcel@test.com", l'argument
// fieldto devra contenir "maurice,marcel"
// Le fichier cryptŽ se voit ajoutŽ .API ˆ la fin de son nom

long APICdecrypter(char* filename);
// le fichier dŽcryptŽ se voit ajouter .txt ˆ la fin de son nom.

long APICdecrypter2(char* filename,char* keyFolderPath,long verbose);
// idem decrypter mais pouvant fournir un path vers le dossier des clefs

long APICcrypterMem(char* donnees,char** resultat, char* Username,char* fieldto,char* fieldcc,char* fieldbcc);
long APICdecrypterMem(char* donnees,char** resultat);


TypErr  APICrypterPieceJointe(char* fileNameSourceFile,char* fileNameDestFile,char* userKey,char* keypath,long format,long verbose);
//crypte une future pièce jointe fileNameSourceFile dans un fichier de destination fileNameDestFile devant avoir une extension .apz
//keypath est le chemin d'accès au dossier "Clefs", sur PC : "C:\\apicrypt\\"
//userkey : clef à utiliser pour crypter sans l'extension .xxx où xxx est un nombre correspondant à année - 1900
//format = 0 : non compressee
//format = 2 : comression LZO si autorisee dans local.h
