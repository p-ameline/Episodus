//
//                  Programme secondaire DKENTETE
//
//                  Fabrication de l'entête du CR
//
//             Début d'écriture :  7 Novembre 1990 (PA)
//             Dernière version :  1 Novembre 1993 (PA)
//
#include <owl\olemdifr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring.h>

#ifdef _INCLUS
  #include "partage\nsglobal.h"
  #include "nsbb\nsutidlg.h"
#endif

#include "partage\nsperson.h"
#include "nssavoir\nspatho.h"

#include "partage\nsdivfct.h"
#include "dzz\dzzglob.h"
#include "dzz\nsdzz.h"

#include "dzz\dzzdecod.h"

//--------------------------------------------------------------------------
//  Function 	 : void entete(string* pCRString)
//
//  Description : Fabrique l'entête du C-R
//
//	 ARGUMENTS 	 :
//
//	 Retour	  	 :	Rien
//--------------------------------------------------------------------------
//  Créé le 14/11/1988  Dernière mise à jour le 26/08/1994
void
decodage::entete(gereDate* pDateExam)
{
	//
	// Facilite l'écriture
	//
  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;

  string sDatex = string(pDateExam->getDate(), 0, 8) ;

	//
	// Titre de l'examen
	//
  NSPathologData Data ;
  string sLexique = decodage::_sExamen ;
  bool bTrouve = pContexte->getDico()->trouvePathologData(_sLangue, &sLexique, &Data);
  if (false == bTrouve)
  {
    Recupere() ;
    return ;
  }

  // Set title
  //
  string sTitre = string("") ;
  Data.donneLibelleAffiche(&sTitre) ;

  for (size_t i = 0; i < strlen(sTitre.c_str()); i++)
    sTitre[i] = pseumaj(sTitre[i]) ;

  setDcodeur(sTitre) ;

  // Set document date
  //
  addToDcodeur(string(" du ") + donne_date(sDatex, _sLangue)) ;

	//
	// Intitulé du patient
	//
  string sDateNaiss = pPatEnCours->donneNaissance() ;

  signed int age = -1 ;
  if (string("00000000") != sDateNaiss)
    age = donne_age(sDatex, sDateNaiss) ;

  string sIntitulePat = string("") ;
	donne_intitule_patient(&sIntitulePat, age, pPatEnCours->estFeminin(), false, _sLangue) ;

	addToDcodeur(string(" pour ") + sIntitulePat) ;
  addToDcodeur(pPatEnCours->getNom()) ;
  stripDcodeur(stripRight, ' ') ;
  addToDcodeur(string(" ")) ;
	addToDcodeur(pPatEnCours->getPrenom()) ;
	stripDcodeur(stripRight, ' ') ;

	metPhrase("2", "2"/*, 1*/) ;

	//
	// Birthdate and age
	//
	if (string("00000000") != sDateNaiss)
	{
		if (pPatEnCours->estFeminin())
			setDcodeur(string("Née le ")) ;
		else
			setDcodeur(string("Né le ")) ;
		addToDcodeur(donne_date(sDateNaiss, _sLangue)) ;
		//if (age != -1)
		//{
		addToDcodeur(" (") ;

		addToDcodeur(donne_intitule_age(sDatex, sDateNaiss)) ;
		addToDcodeur(")") ;
      //}

    metPhrase("", ""/*, 1*/) ;
	}

	setDcodeur(string("")) ;
}

