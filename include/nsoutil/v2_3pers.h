//---------------------------------------------------------------------------
//     V2_3PERS.H
//
//  Contient les définitions des objets PATIENT et UTILISATEUR de NAUTILUS
//
//---------------------------------------------------------------------------
#ifndef __V2_3PERS_H
#define __V2_3PERS_H

#include "partage\nsperson.h"

//---------------------------------------------------------------------------
//  Classe V2_3Patient
//      ANCETRE : NSPatient
//---------------------------------------------------------------------------
/*class V2_3Patient
{
	public :
	  	//
	  	// Variables de stockage des anciennes valeurs
	  	//
   	char V2nss[7];
	  	char V2dernier[15];
	  	char V2nom[26];
	  	char V2prenom[26];
	  	char V2convoc[21];
	  	char V2adresse[51];
	  	char V2ville[26];
	  	char V2medic[6][4];
	  	char V2medinom[26];
	  	char V2medidres[26];
	  	char V2mediville[26];
	  	char V2naissance[9];
	  	char V2telephone[16];
	  	char V2code[21];

      V2_3Patient(NSSuper* pSup) { pSuper = pSup; }
	  	DBIResult importe();

      NSSuper* pSuper;
}; */

//---------------------------------------------------------------------------
//  Classe V2_3Utilisateur
//---------------------------------------------------------------------------
class V2_3Utilisateur
{

	public :

   	//
	  	// Variables de stockage des anciennes valeurs
	  	//
	  	char V2indice[4];
	  	char V2code[7];
	  	char V2nom[26];
	  	char V2prenom[26];
	  	char V2adresse[51];
	  	char V2ville[26];
	  	char V2gparam[14];
	  	char V2cparam[10];
	  	char V2pparam[10];
	  	char V2wparam[7];
	  	char V2lparam[7];
	  	char V2dercod[9];
	  	char V2securite[21];

      V2_3Utilisateur(NSContexte* pCtx = 0);
	  	DBIResult importe();

      NSContexte* pContexte;
};

//---------------------------------------------------------------------------
//  Classe V2_3Correspondant
//---------------------------------------------------------------------------
/*class V2_3Correspondant : public NSCorrespondant
{
  	public :

   	//
	  	// Variables de stockage des valeurs
	  	//
	  	char V2code[4];
	  	char V2nom[26];
	  	char V2prenom[26];
	  	char V2adresse[51];
	  	char V2ville[26];
	  	char V2docteur[2];
	  	char V2telephone[16];
	  	char V2nb_exemp[3];
	  	char V2messagerie[36];

      V2_3Correspondant(NSSuper* pSup = 0);
	  	DBIResult importe();

      NSSuper* pSuper;
};*/

//---------------------------------------------------------------------------
//  Classe V2_3CompteRendu
//---------------------------------------------------------------------------
/*class V2_3CompteRendu : public NSCompteRendu
{
  public :

	  //
	  // Variables de stockage des valeurs
	  //
	  unsigned char V2nss[9];
	  unsigned char V2groupe[5][256];

	  DBIResult importe();

};*/
//---------------------------------------------------------------------------
#endif    // __V2_3PERS_H