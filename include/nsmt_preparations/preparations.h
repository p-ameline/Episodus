//---------------------------------------------------------------------------//     preparations_dlg.h
//     PA   octobre 2010
//  Contient les définitions des objets de Préparations
//---------------------------------------------------------------------------
#ifndef __PREPARATIONS_H
#define __PREPARATIONS_H

class NSContexte ;

#include "partage\ns_vector.h"

//---------------------------------------------------------------------------
//  Classe Materiel
//      ANCETRE : NSFiche
//---------------------------------------------------------------------------
//
// Objet données des Objets Materiel
//
class NSPreparationData
{
	public :

  	string sObjectID ;
    //
    // Variables de stockage des valeurs
    //
    string sName ;
    string sProduct1 ;
    string sDose1 ;
    string sUnit1 ;
    string sProduct2 ;
    string sDose2 ;
    string sUnit2 ;
    bool   bActif ;

    string sAutomaticLabel ;

    void metAZero() ;

    NSPreparationData() { metAZero() ; }
    NSPreparationData(const NSPreparationData& rv) ;

    NSPreparationData& operator=(const NSPreparationData& src) ;
    int                operator==(const NSPreparationData& o) ;
};

//
// Objet "Info" correspondant à Materiel
// (destiné à être utilisé dans les containers)
//
class NSPreparationInfo
{
	public:

		NSPreparationData* pDonnees ;

    // NSPatPathoArray* pPatPathoArray ;
    bool initialiseDepuisObjet(NSContexte* pCtx, string sObjId, string actif = "") ;

		NSPreparationInfo() ;
    ~NSPreparationInfo() ;
		NSPreparationInfo(const NSPreparationInfo& rv) ;

    string donneComplement() ;
    bool initialiseDepuisComplement(string sComplement) ;

    string getObjectID() { return pDonnees->sObjectID ; }
    string getName()     { return pDonnees->sName ; }
    string getProduct1() { return pDonnees->sProduct1 ; }
    string getDose1()    { return pDonnees->sDose1 ; }
    string getUnit1()    { return pDonnees->sUnit1 ; }
    string getProduct2() { return pDonnees->sProduct2 ; }
    string getDose2()    { return pDonnees->sDose2 ; }
    string getUnit2()    { return pDonnees->sUnit2 ; }

    void   setObjectID(string sId) { pDonnees->sObjectID = sId ; }
    void   setName(string sNa)     { pDonnees->sName     = sNa ; }

		NSPreparationInfo& operator=(const NSPreparationInfo& src) ;
		int                operator==(const NSPreparationInfo& o) ;
};

//
// Définition de NSMaterielArray (Array de NSMateriel(s))
//
typedef vector<NSPreparationInfo*>         NSPrepaInfoArray ;
typedef NSPrepaInfoArray::iterator         PrepasIter ;
typedef NSPrepaInfoArray::const_iterator   PrepasConstIter ;
typedef NSPrepaInfoArray::reverse_iterator PrepasReverseIter ;

class NSPreparationsArray : public NSPrepaInfoArray
{
	public :

		// Constructeurs
		NSPreparationsArray() : NSPrepaInfoArray() {}
		NSPreparationsArray(const NSPreparationsArray& rv) ;
      	//
		// Destructeur
		virtual ~NSPreparationsArray() ;
    void vider() ;
};

#endif    // __PREPARATIONS_H

