// codes d'erreurs renvoyés par les différentes classes apicrypt

#ifndef __Crypto__
#define __Crypto__
#endif

#ifndef __CryptErr__
#define __CryptErr__
typedef	long TypErr;

const long kNoErr=0;		// ok
const long kFNFErr=-43;	// file not found
const long kFileClosed=-2;	// fichier fermé

const long kCRCErr=-3;	// CRCErr lors de asciibin....
const long kPbErr=-4;	// pb lors du décrypt....

const long kFrameTooShort=-4; // Frame too short : pendant un décryptage, si paquet plus petit que le minium possible !
const long KeyNotFoundErr=-5; // clef de décryptage not found.
const long kBadKeyHeader=-6; // header de clef corrompu !
const long kMemErr=-7; 	// pb de mémoire ou de clef en lecture seule
const long kOffsetOutOfRangeErr=-8; 	// offset clef out of range
const long kFrameTooLong=-9; // Frame too long : pendant un décryptage, si paquet plus long possible !
const long kAttachmentToBig=-10; 	// pièce jointe trop grosse pour être traitee !
const long kHdlNil=-11; 	// handle a decrypter = nil
const long kEncapsulationFailed=-12; 	// l'encapsulation d'une pièce jointe en format transport apicrypt a échoué
const long kOtherErr=-1;	//toutes les autres erreurs
const long kNonApicryptedAttachement=-11;
const long kDEJATRANSITE=-12;//tentative de readressage d'un email crypte reçu
#endif
