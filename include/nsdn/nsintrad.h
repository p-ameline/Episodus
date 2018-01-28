// nsintradb.h//////////////////////////////////////////////////////////////////////////
// Header des bases intranet - Rémi SPAAK Aout 1997
//////////////////////////////////////////////////////////////////////////

#ifndef __NSINTRAD_H
#define __NSINTRAD_H

#if defined(_DANSNSDNDLL)
	#define _CLASSENSDN __export
#else
	#define _CLASSENSDN __import
#endif

// #include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <owl\listbox.h>
#include "partage\nsperson.h"   // pour le code patient
#include "nssavoir\nspatbas.h"  // pour BASE_LEXI_LEN#include "nautilus\nsannexe.h"#include "nautilus\nstxstyl.h"#include "nsbb\ns_objs.h"	// pour OBJECT_ID_LEN
// tailles et indices des champs de FORMULE.DB///////////////////////////////////////////////

#define FORM_CODE_LEN       3
#define FORM_TYPE_LEN       1
#define FORM_TEXTE1_LEN		255
#define FORM_TEXTE2_LEN		255
#define FORM_TEXTE3_LEN		255
#define FORM_TEXTE4_LEN		255

#define FORM_CODE_FIELD		  1
#define FORM_TYPE_FIELD		  2
#define FORM_TEXTE1_FIELD	  3
#define FORM_TEXTE2_FIELD	  4
#define FORM_TEXTE3_FIELD	  5
#define FORM_TEXTE4_FIELD	  6

////////////////////////////////////////////////////////////////////////////////
// DEFINITION DES CLASSES D'ACCES AUX BASES INTRANET
////////////////////////////////////////////////////////////////////////////////

// Base TEMPLATE.DB
///////////////////

class _CLASSENSDN NSTemplateData
{
	public :
    	//
	  	// Variables de stockage des valeurs
	  	//

		string sCode ;
		string sType ;
		string sUser ;
    string sConsole ;
		string sRoot ;
		string sCompo ;
		string sDefaut ;
		string sEnTete ;
		string sFichier ;
		string sLibelle ;

		NSTemplateData();
		NSTemplateData(const NSTemplateData& rv);

		NSTemplateData&	operator=(const NSTemplateData& src);
		int 		   	    operator==(const NSTemplateData& o);
};

//
// Objet contenant uniquement un pointeur sur les donnees//
class _CLASSENSDN NSTemplateInfo
{
	public :

		//
    // Objet qui contient les données
    //
    NSTemplateData* pDonnees ;
    string			    sNumCons ;

		NSTemplateInfo() ;
		NSTemplateInfo(NSBasicAttributeArray *pTmplArray) ;
		NSTemplateInfo(const NSTemplateInfo& rv) ;
    ~NSTemplateInfo() ;

    void setCode(string sCod)    { pDonnees->sCode    = sCod ; }
		void setType(string sTyp)    { pDonnees->sType    = sTyp ; }
		void setUser(string sUti)    { pDonnees->sUser    = sUti ; }
    void setConsole(string sCsl) { pDonnees->sConsole = sCsl ; }
		void setRoot(string sRoo)    { pDonnees->sRoot    = sRoo ; }
		void setCompo(string sCo)    { pDonnees->sCompo   = sCo ; }
		void setDefaut(string sD)    { pDonnees->sDefaut  = sD ; }
		void setEnTete(string sE)    { pDonnees->sEnTete  = sE ; }
		void setFichier(string sF)   { pDonnees->sFichier = sF ; }
		void setLibelle(string sL)   { pDonnees->sLibelle = sL ; }

    string getCode()    { return pDonnees->sCode ; }
		string getType()    { return pDonnees->sType ; }
		string getUser()    { return pDonnees->sUser ; }
    string getConsole() { return pDonnees->sConsole ; }
		string getRoot()    { return pDonnees->sRoot ; }
		string getCompo()   { return pDonnees->sCompo ; }
		string getDefaut()  { return pDonnees->sDefaut ; }
		string getEnTete()  { return pDonnees->sEnTete ; }
		string getFichier() { return pDonnees->sFichier ; }
		string getLibelle() { return pDonnees->sLibelle ; }

    NSTemplateInfo& operator=(const NSTemplateInfo& src) ;
		int             operator == ( const NSTemplateInfo& o ) ;
};

//
// Définition de NSTemplateArray (Array de NSTemplate)
//
typedef vector<NSTemplateInfo*>          NSTemplateVector ;
typedef NSTemplateVector::iterator       NSTemplateIter ;
typedef NSTemplateVector::const_iterator NSTemplateConstIter ;

class _CLASSENSDN NSTemplateArray : public NSTemplateVector
{
	public :

		// Constructeurs
		NSTemplateArray() : NSTemplateVector() {}
		NSTemplateArray(const NSTemplateArray& rv) ;
		// Destructeur
		virtual ~NSTemplateArray() ;

		NSTemplateArray& operator=(const NSTemplateArray& src) ;
		void vider() ;
};

//////////////////////////////////////////////////////////////////////////////// Base FORMULE.DB
//////////////////////////////////////////////////////////////////////////////

class _CLASSENSDN NSFormuleData
{
  public :

		//	  // Variables de stockage des valeurs
	  //
    char code[FORM_CODE_LEN + 1] ;
	 	char type[FORM_TYPE_LEN + 1] ;
    char texte1[FORM_TEXTE1_LEN + 1] ;
    char texte2[FORM_TEXTE2_LEN + 1] ;
    char texte3[FORM_TEXTE3_LEN + 1] ;
    char texte4[FORM_TEXTE4_LEN + 1] ;

	  NSFormuleData() ;
    NSFormuleData(const NSFormuleData& rv) ;

    void metAZero() ;

	  NSFormuleData& operator=(const NSFormuleData& src) ;
	  int 			     operator==(const NSFormuleData& o) ;
};

//// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
#ifndef _ENTERPRISE_DLL
class _CLASSENSDN NSFormule : public NSFiche
#else
class _CLASSENSDN NSFormule : public NSSuperRoot
#endif
{
  public :

	  //
	  // Variables de stockage des valeurs
	  //
	  NSFormuleData* pDonnees ;

	  NSFormule(NSSuper* pSuper) ;
	  ~NSFormule() ;

#ifndef _ENTERPRISE_DLL
	  DBIResult open() ;
#endif
	  void alimenteFiche() ;
	  void videFiche() ;
};

//// Objet contenant uniquement un pointeur sur les donnees
//
class _CLASSENSDN NSFormuleInfo
{
	public :

  	//
    // Objet qui contient les données
    //
    NSFormuleData* pDonnees ;

    NSFormuleInfo() ;
		NSFormuleInfo(const NSFormule*) ;
		NSFormuleInfo(const NSFormuleInfo& rv) ;
    ~NSFormuleInfo() ;

    NSFormuleInfo& operator=(const NSFormuleInfo& src) ;
		int            operator==(const NSFormuleInfo& o) ;

    string getCode()   { return string(pDonnees->code) ; }
	 	string getType()   { return string(pDonnees->type) ; }
    string getTexte1() { return string(pDonnees->texte1) ; }
    string getTexte2() { return string(pDonnees->texte2) ; }
    string getTexte3() { return string(pDonnees->texte3) ; }
    string getTexte4() { return string(pDonnees->texte4) ; }

    void   setCode(string sCo)   { strcpy(pDonnees->code, sCo.c_str()) ; }
    void   setType(string sTy)   { strcpy(pDonnees->type, sTy.c_str()) ; }
    void   setTexte1(string sTx) { strcpy(pDonnees->texte1, sTx.c_str()) ; }
    void   setTexte2(string sTx) { strcpy(pDonnees->texte2, sTx.c_str()) ; }
    void   setTexte3(string sTx) { strcpy(pDonnees->texte3, sTx.c_str()) ; }
    void   setTexte4(string sTx) { strcpy(pDonnees->texte4, sTx.c_str()) ; }
};

//
// Définition de NSFormuleArray (Array de NSFormule)
//
typedef vector<NSFormuleInfo*>          NSFormuleVector ;
typedef NSFormuleVector::iterator       NSFormuleIter ;
typedef NSFormuleVector::const_iterator NSFormuleConstIter ;

class _CLASSENSDN NSFormuleArray : public NSFormuleVector
{
	public :

		// Constructeurs
		NSFormuleArray() : NSFormuleVector() {}
		NSFormuleArray(const NSFormuleArray& rv) ;
		// Destructeur
		virtual ~NSFormuleArray() ;

   	NSFormuleArray& operator=(const NSFormuleArray& src) ;
   	void vider() ;
};

#endif // __NSINTRAD_H

///////////////////////////// fin de nsintradb.h ///////////////////////////////
