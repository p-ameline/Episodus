long APICcrypter2(char* filename, char* Username,char* keyFolderPath,char* fieldto,char* fieldcc,char* fieldbcc,long verbose);
// fieldto, fieldcc et fiedbcc contiennent les adresses emails des destinataires autorisés du messages
// uniquement le début du email est utilisé, c'est à dire ce qui précède le @
// par exemple, si le champ to contient 2 destinataires "maurice@test.com, marcel@test.com", l'argument
// fieldto devra contenir "maurice,marcel"
// Le fichier crypté se voit ajouté .API à la fin de son nom
// idem crypter mais pouvant fournir un path vers le dossier des clefs

long APICdecrypter2(char* filename,char* keyFolderPath,long verbose);
// idem decrypter mais pouvant fournir un path vers le dossier des clefs

long APICcrypterMem2(char* donnees,char** resultat, char* Username, char* keyFolderPath,char* fieldto,char* fieldcc,char* fieldbcc);
// idem crypterMem mais pouvant fournir un path vers le dossier des clefs

long APICdecrypterMem2(char* donnees,char** resultat,char* keyFolderPath);
// idem decrypterMem mais pouvant fournir un path vers le dossier des clefs


long APITestDLL(char* donnees, char** resultat); // retourne la chaine envoyée en entrée/
long APITestDLL2(char* donnees); // retourne 100
long APITestDLL3(char* donnees); // retourne la longueur de la chaine envoyée en entrée/


long APICrypterPieceJointe2(char* fileNameSourceFile,char* fileNameDestFile,char* userKey,char* keypath,long format,long verbose);
//crypte une future piËce jointe fileNameSourceFile dans un fichier de destination fileNameDestFile devant avoir une extension .apz
//keypath est le chemin d'accËs au dossier "Clefs", sur PC : "C:\\apicrypt\\"
//userkey : clef ‡ utiliser pour crypter sans l'extension .xxx o˘ xxx est un nombre correspondant ‡ annÈe - 1900
//format = 0 : non compressÈe, d'autres formats seront disponibles plus tard.

long APIDecrypterPJ2(char* fileNameSourceFile,char* pathDestFiles,char* keypath,long verbose);
//dÈcrypte une ou plusieurs piËces jointes contenues dans un .apz 
//fileNameSourceFile = fichier .apz ‡ traiter
//pathDestFiles = dossier d'arrivÈe pour les piËces jointes dÈcryptÈes doit se terminer par \ impÈrativement
//keypath est le chemin d'accËs au dossier "Clefs", sur PC : "C:\\apicrypt\\"







// fonctions obsoletes, conservees pour des raisons de compatibilite uniquement.

long APICcrypter(char* filename, char* Username,char* fieldto,char* fieldcc,char* fieldbcc);
// fieldto, fieldcc et fiedbcc contiennent les adresses emails des destinataires autorisés du messages
// uniquement le début du email est utilisé, c'est à dire ce qui précède le @
// par exemple, si le champ to contient 2 destinataires "maurice@test.com, marcel@test.com", l'argument
// fieldto devra contenir "maurice,marcel"
// Le fichier crypté se voit ajouté .API à la fin de son nom

long APICdecrypter(char* filename);
// le fichier décrypté se voit ajouter .txt à la fin de son nom.

long APICcrypterMem(char* donnees,char** resultat, char* Username,char* fieldto,char* fieldcc,char* fieldbcc);
long APICdecrypterMem(char* donnees,char** resultat);

// utilisé pour crypter des fichiers sans la couche binascii
long APICcrypterbinaire(char* filename, char* Username, char* keyFolderPath,long verbose);
long APICdecrypterbinaire(char* filename, char* keyFolderPath,long verbose);
long APIBeep(void);

//réservé aux tests
long APICcrypter2Light(char* filename, char* Username,char* keyFolderPath);

