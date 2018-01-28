//---------------------------------------------------------------------------
//     NSANXARY.H
//
//  Contient les definitions des arrays d'objets annexes de NAUTILUS
//
//  Ce code est separe de NSANNEXE car il n'est pas possible de declarer
//  une Array comme Import
//
//  1ere version : PA juillet 94   Dernière modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSANXARY_H
#define __NSANXARY_H

// #include <classlib\arrays.h>

class NSDocumentInfo ;
class NSChemiseInfo ;

// #include "nautilus\nsannexe.h"
// #include "nsdn\nsdocum.h"

#include "partage/ns_vector.h"
#include "partage/NTArray.h"

//
// Définition de NSDocumentArray (Array de NSDocument(s))
//
typedef vector<NSDocumentInfo*>  NSDocInfoArray ;
typedef NSDocInfoArray::iterator DocInfoIter ;
typedef NTArray<NSDocumentInfo>  NSDocumentArray ;

/*
class NSDocumentArray : public NSDocInfoArray
{
	public :
	// Constructeurs
	NSDocumentArray() : NSDocInfoArray() {}
	NSDocumentArray(NSDocumentArray& rv);
	// Destructeur
	virtual ~NSDocumentArray();
	void vider();
};*/

class NSPatPathoArray ;

//
// Définition de NSChemiseArray (Array de NSChemise(s))
//
typedef vector<NSChemiseInfo*> NSChemInfoArray;
typedef NSChemInfoArray::iterator ChemInfoIter;
typedef NTArray<NSChemiseInfo> ChemisesArray ;

#ifndef __linux__
  #if defined(_DANSNSDNDLL)
    #define _CLASSENSDN __export
  #else
    #define _CLASSENSDN __import
  #endif
class _CLASSENSDN NSChemiseArray : public ChemisesArray
#else
class NSChemiseArray : public ChemisesArray
#endif
{
  public :

    bool initFromPatpatho(NSPatPathoArray *pFoldersPpt) ;
};

//
// Définition de NSArmoireArray (Array de NSArmoire(s))
//
/*
typedef vector<NSArmoireInfo*> NSArmoiInfoArray;
typedef NSArmoiInfoArray::iterator ArmoiInfoIter;

class NSArmoireArray : public NSArmoiInfoArray
{

	public :

	// Constructeurs
	NSArmoireArray() : NSArmoiInfoArray() {}
	NSArmoireArray(NSArmoireArray& rv);
	// Destructeur
	virtual ~NSArmoireArray();
	void vider();
};
*/

//
// Définition de NSChemDocArray (Array de NSChemDoc(s))
//

/*

typedef vector<NSChemiseInfo*> NSChemDocInfoArray;
typedef NSChemDocInfoArray::iterator ChemDocInfoIter;


class NSChemDocArray : public NSChemDocInfoArray
{
	public :

	// Constructeurs
	NSChemDocArray() : NSChemDocInfoArray() {}
	NSChemDocArray(NSChemiseArray& rv);
	// Destructeur
	virtual ~NSChemDocArray();
	void vider();
};

*/

//---------------------------------------------------------------------------

#endif    // __NSANXARY_H

