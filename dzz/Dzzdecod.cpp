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
  	if 	  (isTexteLibre())
    {
    	Avance() ;
      while ((getCol() > refCol) && iBon())
      {
      	if (getSt() == "£??")
        {
        	setDcodeur(getTexteLibre()) ;
          Avance() ;
          metPhrase(decDeb, decFin, sautLigne) ;
        }
        else
        {
          setDcodeurFromLexique(getStL(), _sLangue) ;
          Avance() ;
          if (getDcodeur() != string(""))
          {
          	setPseumajForFirstChar() ;
            addToDcodeur(string(".")) ;
          }
					metPhrase(decDeb, decFin, sautLigne) ;
        }
      }
    }
    else
    	Recupere() ;
	}
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
  // Is it necessary to change all numbers by their denomination in letters,
  // as needed for prescriptions of narcotics
  //
  bool bAllLettersSentences = false ;

  if (getSt() == string("ZORDS"))
    bAllLettersSentences = true ;

  if (decodage::_pDocument)
  {
    gereDate dateExam(pContexte->getSuperviseur(), _sLangue) ;
    string sCreationDate = decodage::_pDocument->_sDateCreation ;
    dateExam.setDate(&sCreationDate) ;
    entete(&dateExam) ;
  }

  // Prescription lines count
  //
  int iPrescriptionLineCount = countLines() ;

  setDcodeur(string("")) ;
  metPhrase("", ""/*, 1*/) ;

  setDcodeur(string("Cette ordonnance comporte ")) ;
  if (1 == iPrescriptionLineCount)
    addToDcodeur(string("une ligne de prescription.")) ;
  else
  {
    string sAllCharText = getAllCharsText(IntToString(iPrescriptionLineCount) + string(" lignes de prescription."), ',', ' ', 0) ;
    addToDcodeur(sAllCharText) ;
  }
  metPhrase("", ""/*, 1*/) ;

  //
  //
  int refCol = getCol() ;

	while ((getCol() >= refCol) && iBon())
	{
    // Concept "ALD" or "ACD"
    //
  	if 	    ((getSt() == string("GPALD")) || (getSt() == string("GPCOD")))
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
      decPrescription Prescri(this, bAllLettersSentences) ;
      Prescri.setLibelle(sTitre) ;
      Prescri.decode(refCol) ;
    }
    // Drug
    //
    else if (((getSt())[0] == '_') || ((getSt())[0] == 'I') ||
             ((getSt())[0] == 'N') || ((getSt())[0] == 'O') ||
             ((getSt())[0] == 'G'))
    {
    	decSpecialite Specia(this, bAllLettersSentences, dcTiret) ;
      Specia.decode(refCol) ;
      Specia.donnePhrase() ;
    	setDcodeur(string("")) ;
      metPhrase() ;
    }
    // Allergy
    //
    else if (getSt() == string("EALMD"))
    {
    	int refCol2 = getCol() ;

      _pPhraseur->initialise() ;
      _pGenerateur->reinitialise() ;

      NSPhraseMot concept((*(getitDcode()))->getDataTank(), pContexte) ;

      Avance() ;

      if (getCol() > refCol2)
        _pPhraseur->pDeuxPoints = new NSPhraseur(pContexte) ;

      int iAllerCount = 0 ;

	    while ((getCol() > refCol2) && iBon())
	    {
        NSPhraseMot allergy((*(getitDcode()))->getDataTank(), pContexte) ;
        _pPhraseur->pDeuxPoints->addCOD(&allergy) ;
        Avance() ;
        iAllerCount++ ;
      }

      if (iAllerCount > 0)
      {
        if (iAllerCount > 1)
          concept.setPluriel(string("WPLUS1")) ;

        _pPhraseur->addCOD(new NSPhraseMot(concept)) ;

        _pGenerateur->genereProposition(dcTiret) ;
	      if (_pGenerateur->getPropositionPhrase() != string(""))
	      {
  	      _pGenerateur->termineProposition() ;
          setDcodeur(_pGenerateur->getPropositionPhrase()) ;
          metPhrase("", ""/*, 1*/) ;
        }
      }
    }
		else
    	BioLibre(refCol) ;
	}

	if (false == iBon())
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
}

/**
 *  Count the prescription lines
 */
int
decGeneral::countLines()
{
  MetMarque() ;

  int iCount = 0 ;
  int refCol = getCol() ;

	while ((getCol() >= refCol) && iBon())
	{
  	if 	    ((getSt() == string("GPALD")) || (getSt() == string("GPCOD")))
      Avance() ;

    // Allergies, take care not to take into account
    //
    else if (getSt() == string("EALMD"))
    {
      int refCol2 = getCol() ;
      Avance() ;
      while ((getCol() > refCol2) && iBon())
        Avance() ;
    }
    else if (((getSt())[0] == '_') || ((getSt())[0] == 'I') ||
             ((getSt())[0] == 'N') || ((getSt())[0] == 'O') ||
             ((getSt())[0] == 'G'))
    {
      iCount++ ;

      int refCol2 = getCol() ;
      Avance() ;
      while ((getCol() > refCol2) && iBon())
        Avance() ;
    }
		else
    	Avance() ;
	}

  VaMarque() ;

  return iCount ;
}
