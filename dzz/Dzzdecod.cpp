//  +-----------------------------------------------------------------+
//  |                   Programme secondaire DZZDECOD                 |
//  |                                                                 |
//  |          Oriente le décodage vers les principaux chapitres      |
//  |                                                                 |
//  |             Début d'écriture : 14 Janvier 2001 (PA)             |
//  |             Dernière version : 14 Janvier 2001 (PA)             |
//  +-----------------------------------------------------------------+
#include <owl\olemdifr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring.h>

#include "nssavoir\nssavoir.h"
#include "nssavoir\nspatho.h"
#include "nsbb\nstlibre.h"
#include "dzz\dzzdecod.h"
#include "dzz\dzzglob.h"
#include "dzz\dzzspeci.h"
#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nsdn\nsdocum.h"

//  +-----------------------------------------------------------------+
//  ¦              Biopsies, Textes libres, Traitements               ¦
//  +-----------------------------------------------------------------+
//  Créé le 20/03/1989 Dernière mise à jour 20/03/1989
void
decodage::BioLibre(int colonne, string decDeb, string decFin, int sautLigne)
{
	// int  		 i, j, k, l;
  // unsigned int iNum;
	// char 		 decal[3];

  int refCol = getCol();

	while ((getCol() > colonne) && iBon())
	{
  	if 	  (getSt() ==	"#TLI#")
    {
    	// NSTlibre NSTlibre(pContexte) ;
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
      	if (getSt() == "£??")
        {
					// bool bOK = NSTlibre.RecupereTexte(pContexte->getPatient()->getNss(), *getCpl(), sDcodeur()) ;
					Avance() ;
          // if (bOK)
          	metPhrase(decDeb, decFin, sautLigne) ;
				}
        else
        {
          setDcodeurFromLexique(getStL(), _sLangue) ;
          Avance() ;
          addToDcodeur(string(".")) ;
					metPhrase(decDeb, decFin, sautLigne) ;
        }
			}
		}
    else
    	Recupere() ;
	}

	return ;
}

//  +-----------------------------------------------------------------+
//  ¦                   Lésion sans description                       ¦
//  +-----------------------------------------------------------------+
//  Créé le 19/01/1989 Dernière mise à jour 19/01/1989
void
decodage::lesionVide(string phrase, int colonne)
{
	while (getCol() > colonne)
  	BioLibre(colonne) ;

  debutPhrase() ;
  addToDcodeur(phrase) ;
  addToDcodeur(string(".")) ;
  metPhrase() ;
  finPhrase() ;
  return ;
}

decGeneral::decGeneral(NSContexte* pCtx)
           :decodage(pCtx)
{
}

decGeneral::decGeneral(decodageBase* pBase)
           :decodage(pBase)
{
}

//  +-----------------------------------------------------------------+
//  ¦             Décode code[], comlib[] et biop[]                   ¦
//  +-----------------------------------------------------------------+
//  Créé le 10/11/1988 Dernière mise à jour 23/06/1992
void
decGeneral::decode()
{
  if (decodage::_pDocument)
  {
    gereDate dateExam(pContexte->getSuperviseur(), _sLangue) ;
    string sCreationDate = decodage::_pDocument->_sDateCreation ;
    dateExam.setDate(&sCreationDate) ;
    entete(&dateExam) ;
  }

  int refCol = getCol() ;

	while ((getCol() >= refCol) && iBon())
	{
  	if 	    ((getSt() == "GPALD") || (getSt() == "GPCOD"))
    {
    	NSPathologData Data ;
      string sLex = getStL() ;
      bool bTrouve = pContexte->getDico()->trouvePathologData(_sLangue, &sLex, &Data) ;
      if (false == bTrouve)
      {
      	Recupere() ;
        return ;
      }
      string sTitre = "" ;
      Data.donneLibelleAffiche(&sTitre) ;
      sTitre[0] = pseumaj(sTitre[0]) ;
      ajLL() ;
      Avance() ;
      decPrescription Prescri(this) ;
      Prescri.setLibelle(sTitre) ;
      Prescri.decode(refCol) ;
    }
    else if (((getSt())[0] == '_') || ((getSt())[0] == 'I') ||
             ((getSt())[0] == 'N') || ((getSt())[0] == 'O') ||
             ((getSt())[0] == 'G'))
    {
    	decSpecialite Specia(this, dcTiret) ;
      Specia.decode(refCol) ;
      Specia.donnePhrase() ;
    	setDcodeur(string("")) ;
      metPhrase() ;
    }
		else
    	Recupere() ;
	}

	if (false == iBon())
		return ;

	return ;
}

/*  +-----------------------------------------------------------------+  */
/*  ¦                  Décode le "time stamp"                         ¦  */
/*  +-----------------------------------------------------------------+  */
/*  Créé le 06/09/1991 Dernière mise à jour 06/09/1991                   */
void
decGeneral::versi(int colonne)
{
	gereDate dateExam(pContexte->getSuperviseur(), _sLangue) ;

  string codeOper = string("") ;
  string contexte = string("") ;

  int refCol = getCol() ;

  while ((getCol() > colonne) && iBon())
	{
  	if 	    (getSt() == "KCHIR")
    {
    	Avance() ;
      donneDate(refCol, &dateExam) ;
    }
    else if (getSt() == "DOPER")
    {
    	codeOper = getCpl() ;
      Avance() ;
    }
  }

  entete(&dateExam) ;

  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;

  string sDateNaiss = pPatEnCours->donneNaissance() ;
  if (string("00000000") != sDateNaiss)
  {
  	if (contexte != "")
    	setDcodeur(string("(") + contexte + string(") ")) ;

    if (pPatEnCours->estFeminin())
      addToDcodeur(string("Née le ")) ;
    else
    	addToDcodeur(string("Né le ")) ;

    addToDcodeur(donne_date(sDateNaiss, _sLangue)) ;
    //if (age != -1)
    //{
    addToDcodeur(string(" (")) ;
    string sDateExamen = dateExam.getDate() ;
    addToDcodeur(donne_intitule_age(sDateExamen, sDateNaiss)) ;
    addToDcodeur(string(")")) ;
    //}
  }
  else
  	setDcodeur(contexte) ;

  if (string("") != getDcodeur())
  	metPhrase("", ""/*, 1*/) ;

  return ;
}

