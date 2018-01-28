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
  signed int age ;
  string	   intitulePat;
	//
	// Facilite l'écriture
	//
  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;

  string sDatex = pDateExam->getDate() ;

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

  string sTitre = string("") ;
  Data.donneLibelleAffiche(&sTitre) ;

  for (size_t i = 0; i < strlen(sTitre.c_str()); i++)
    sTitre[i] = pseumaj(sTitre[i]) ;

  setDcodeur(sTitre + string(" de ")) ;
	//
	// Intitulé du patient
	//
#ifndef _MUE
  if (!(pPatEnCours->donneNaissance(dateNaiss)))
    age = -1;
  else
    age = donne_age(datex, dateNaiss);

	donne_intitule_patient(&intitulePat, age, pPatEnCours->estFeminin(), false, sLangue);

	addToDcodeur(intitulePat;
  addToDcodeur(pPatEnCours->pDonnees->nom) ;
	strip(*sDcodeur(), stripRight, ' '); // sDcodeur()->strip(string::Trailing, ' ');
  addToDcodeur(string(" ")) ;
	addToDcodeur(pPatEnCours->pDonnees->prenom) ;
	strip(*sDcodeur(), stripRight, ' '); // sDcodeur()->strip(string::Trailing, ' ');
#else
  string sDateNaiss = pPatEnCours->donneNaissance() ;
  if (string("00000000") == sDateNaiss)
    age = -1;
  else
    age = donne_age(sDatex, sDateNaiss) ;

	donne_intitule_patient(&intitulePat, age, pPatEnCours->estFeminin(), false, _sLangue) ;

	addToDcodeur(intitulePat) ;
  addToDcodeur(pPatEnCours->getNom()) ;
  stripDcodeur(stripRight, ' ') ;
  addToDcodeur(string(" ")) ;
	addToDcodeur(pPatEnCours->getPrenom()) ;
	stripDcodeur(stripRight, ' ') ;
#endif

	metPhrase("2", "2"/*, 1*/) ;
	//
	// Hospitalisé ou externe
	//
	setDcodeur(string("")) ;
}

