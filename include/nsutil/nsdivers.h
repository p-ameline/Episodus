//-------------------------------------------------------------------------
//                    BIBLIOTHEQUE : NSDIVERS.H
//
//            Fonctions de traitement de comptes rendus
//
//             Début d'écriture : 30 Juin 1988 (PA)
//             Dernière version : 17 Août 1994 (PA)
//-------------------------------------------------------------------------

#if !defined(__NSDIVERS_H)
#define __NSDIVERS_H

#include "partage\ns_vector.h"

//UINT valide_dernier(NSPatient* pPatient, unsigned char *message, UINT *reparable);
UINT valide_cr(string* pCRString, char *message, UINT *reparable);

#if defined(_NSUTILDLL)
	#define _CLASSEUTIL __export
#else
	#define _CLASSEUTIL __import
#endif

//
// Classe NSString
//

class _CLASSEUTIL NSString
{
public :
	string donnee;
   NSString(string s);
   NSString(NSString& rv);
   ~NSString();
   NSString& operator=(NSString src);
};

//
// Définition de NSStringArray (Array de string)
//

typedef vector<NSString*> NSStringVector;
typedef NSStringVector::iterator NSStringIter;

class _CLASSEUTIL NSStringArray : public NSStringVector
{
public :
	// Constructeurs
	NSStringArray() : NSStringVector() {}
	NSStringArray(NSStringArray& rv);
	// Destructeur
	~NSStringArray();

   NSStringArray& operator=(NSStringArray src);
   void vider();
};

#endif