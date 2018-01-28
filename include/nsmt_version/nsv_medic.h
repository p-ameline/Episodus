//---------------------------------------------------------------------------//     NS_MEDIC.H
//
//  	Implémentation des protocoles médicamenteux
//---------------------------------------------------------------------------
#ifndef __NSV_MEDIC_H
#define __NSV_MEDIC_H

#include <cstring.h>
#include "nssavoir\nspatbas.h"
#include "ns_sgbd\nsfiche.h"

//
// Taille des champs du fichier PRESCRIP
// les autres champs sont dans nspatbas.h
//
#define PRESC_CODE_LEN   		  11

//
// Indice des champs du fichier PRESCRIP
//
#define PRESC_CODE_FIELD   			  1
#define PRESC_LOCALISATION_FIELD 	2
#define PRESC_TYPE_FIELD  		 	  3
#define PRESC_LEXIQUE_FIELD      	4
#define PRESC_COMPLEMENT_FIELD   	5
#define PRESC_CERTITUDE_FIELD    	6
#define PRESC_INTERET_FIELD  	 	  7
#define PRESC_PLURIEL_FIELD      	8

//
// Taille des champs du fichier PROTOCOL
// les autres champs sont dans nspatbas.h
//
#define PROTO_TYPE_LEN   	   	    5
#define PROTO_UTILISATEUR_LEN	    3
#define PROTO_CODE_LEN            3
#define PROTO_LIBELLE_LEN        30
#define PROTO_SCENARIO_LEN     	  5

//
// Indice des champs du fichier PROTOCOL
//
#define PROTO_TYPE_FIELD  	  	  1
#define PROTO_UTILISATEUR_FIELD   2
#define PROTO_CODE_FIELD          3
#define PROTO_LIBELLE_FIELD   	  4
#define PROTO_SCENARIO_FIELD   	  5

//---------------------------------------------------------------------------
//  Classe NSPrescription
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSVPrescriptionData : public NSPathoBaseData
{
	public :

		//
		// Variables de stockage des valeurs
		//
		char code[PRESC_CODE_LEN + 1] ;

		NSVPrescriptionData() ;
		NSVPrescriptionData(NSVPrescriptionData& rv) ;
		~NSVPrescriptionData() ;

		void metAZero() ;

		NSVPrescriptionData& operator=(NSVPrescriptionData src) ;
		int                  operator==(const NSVPrescriptionData& o) ;
};

class NSVPrescription : public NSFiche
{
	public :

		//
		// Objet qui contient les données
    //
    NSVPrescriptionData* pDonnees ;

    NSVPrescription(NSSuper* pSuper) ;
    NSVPrescription(NSVPrescription& rv) ;
    ~NSVPrescription() ;

		void alimenteFiche() ;
    void videFiche() ;    DBIResult open(string pathbase) ;
    DBIResult getPatRecord() ;

    virtual bool Create() ;
    virtual bool Modify() ;

    NSVPrescription& operator=(NSVPrescription src) ;
    int              operator==(const NSVPrescription& o) ;
};

//
//  Classe NSPrescriInfo (destinée à être stockée dans une Array)
//
class NSVPrescriInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSVPrescriptionData* pDonnees ;

		NSVPrescriInfo() ;
    NSVPrescriInfo(NSVPrescription*) ;
		NSVPrescriInfo(NSVPrescriInfo& rv) ;
    ~NSVPrescriInfo() ;

		NSVPrescriInfo& operator=(NSVPrescriInfo src) ;
		int operator == ( const NSVPrescriInfo& o ) ;
};

//---------------------------------------------------------------------------
//  Classe NSProtocole
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSVProtocoleData
{
	public :

		//
		// Variables de stockage des valeurs
		//
		char type[PROTO_TYPE_LEN + 1] ;
		char utilisateur[PROTO_UTILISATEUR_LEN + 1] ;
		char code[PROTO_CODE_LEN + 1] ;
		char libelle[PROTO_LIBELLE_LEN + 1] ;
		char scenario[PROTO_SCENARIO_LEN + 1] ;

		NSVProtocoleData() ;
		NSVProtocoleData(NSVProtocoleData& rv) ;
		~NSVProtocoleData() ;

		void metAZero() ;

		NSVProtocoleData& operator=(NSVProtocoleData src) ;
		int 			  	   operator==(const NSVProtocoleData& o) ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class NSVProtocole : public NSFiche
{
	public :

		//
    // Objet qui contient les données
    //
    NSVProtocoleData* pDonnees ;

    NSVProtocole(NSSuper* pSuper) ;
    NSVProtocole(NSVProtocole& rv) ;
    ~NSVProtocole() ;

    void alimenteFiche() ;
    void videFiche() ;
    DBIResult open(string pathbase) ;
    DBIResult getPatRecord() ;

    virtual bool Create() ;
    virtual bool Modify() ;

    NSVProtocole& operator=(NSVProtocole src) ;
    int operator == ( const NSVProtocole& o ) ;
};

//
//  Classe NSVProtocoleInfo (destinée à être stockée dans une Array)
//
class NSVProtocoleInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSVProtocoleData* pDonnees ;

		NSVProtocoleInfo() ;
    NSVProtocoleInfo(NSVProtocole*) ;
		NSVProtocoleInfo(NSVProtocoleInfo& rv) ;
    ~NSVProtocoleInfo() ;

		NSVProtocoleInfo& operator=(NSVProtocoleInfo src) ;
		int operator == ( const NSVProtocoleInfo& o ) ;
};

//---------------------------------------------------------------------------
#endif    // __NSV_MEDIC_H

