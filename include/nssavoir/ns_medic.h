//---------------------------------------------------------------------------//     NS_MEDIC.H
//
//  	Implémentation des protocoles médicamenteux
//---------------------------------------------------------------------------
#ifndef __NS_MEDIC_H
#define __NS_MEDIC_H

#include <cstring.h>
#include "nssavoir\nspatbas.h"
#include "nsbb\nspatpat.h"
#include "ns_sgbd\nsfiche.h"

#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

//
// Taille des champs du fichier PRESCRIP
// les autres champs sont dans nspatbas.h
//
#define PRESC_CODE_LEN   		  11

//
// Indice des champs du fichier PRESCRIP
//
#define PRESC_CODE_FIELD   			1
#define PRESC_LOCALISATION_FIELD 	2
#define PRESC_TYPE_FIELD  		 	3
#define PRESC_LEXIQUE_FIELD      	4
#define PRESC_COMPLEMENT_FIELD   	5
#define PRESC_CERTITUDE_FIELD    	6
#define PRESC_INTERET_FIELD  	 	7
#define PRESC_PLURIEL_FIELD      	8

//
// Taille des champs du fichier PROTOCOL
// les autres champs sont dans nspatbas.h
//
#define PROTO_TYPE_LEN   	   	5
#define PROTO_UTILISATEUR_LEN	3
#define PROTO_CODE_LEN   	 	3
#define PROTO_LIBELLE_LEN      30
#define PROTO_SCENARIO_LEN     	5

//
// Indice des champs du fichier PROTOCOL
//
#define PROTO_TYPE_FIELD  	  	1
#define PROTO_UTILISATEUR_FIELD	2
#define PROTO_CODE_FIELD   		3
#define PROTO_LIBELLE_FIELD   	4
#define PROTO_SCENARIO_FIELD   	5

//---------------------------------------------------------------------------
//  Classe NSPrescription
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSPrescriptionData : public NSPathoBaseData
{
  	public :

	  	//
	  	// Variables de stockage des valeurs
	  	//
	  	char code[PRESC_CODE_LEN + 1];

	  	NSPrescriptionData();
	  	NSPrescriptionData(NSPrescriptionData& rv);
     	~NSPrescriptionData();

     	void metAZero();

	  	NSPrescriptionData&	operator=(NSPrescriptionData src);
	  	int 			  	operator==(const NSPrescriptionData& o);
};


//
//  Classe NSPrescriInfo (destinée à être stockée dans une Array)
//
class _CLASSELEXI NSPrescriInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSPrescriptionData* pDonnees;

		NSPrescriInfo();
		NSPrescriInfo(NSPrescriInfo& rv);
    ~NSPrescriInfo();

		NSPrescriInfo& operator=(NSPrescriInfo src);
		int operator == ( const NSPrescriInfo& o );
};

//---------------------------------------------------------------------------
//  Classe NSProtocole
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSProtocoleData
{
	public :

  	//
		// Variables de stockage des valeurs
		//
		// char type[PROTO_TYPE_LEN + 1];
		// char utilisateur[PROTO_UTILISATEUR_LEN + 1];
		// char code[PROTO_CODE_LEN + 1];
		// char libelle[PROTO_LIBELLE_LEN + 1];
		// char scenario[PROTO_SCENARIO_LEN + 1];

    string sObjectID ;
    char   type[BASE_LEXI_LEN] ;
    string sLibelle ;

		NSProtocoleData();
		NSProtocoleData(NSProtocoleData& rv);
		~NSProtocoleData();

		void metAZero();

		NSProtocoleData& operator=(NSProtocoleData src);
		int 			  	   operator==(const NSProtocoleData& o);
};

//
//  Classe NSProtocoleInfo (destinée à être stockée dans une Array)
//
class _CLASSELEXI NSProtocoleInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSProtocoleData* pDonnees ;
    NSPatPathoArray* pPatPathoArray ;

		NSProtocoleInfo() ;
		NSProtocoleInfo(NSProtocoleInfo& rv) ;
    ~NSProtocoleInfo() ;

    bool initialiseDepuisObjet(NSContexte* pCtx, string sObjId) ;

		string           getType()    { return string(pDonnees->type) ; }
    string           getLibelle() { return pDonnees->sLibelle ; }
    NSPatPathoArray* getPptPtr()  { return pPatPathoArray ; }

		NSProtocoleInfo& operator=(NSProtocoleInfo src) ;
		int operator == ( const NSProtocoleInfo& o ) ;
};

//---------------------------------------------------------------------------
#endif    // __NS_MEDIC_H

