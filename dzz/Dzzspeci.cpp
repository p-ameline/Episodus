//  +-----------------------------------------------------------------+
//  ¦               PROGRAMME SECONDAIRE : DZZSpeci.cpp               ¦
//  ¦                                                                 ¦
//  ¦              Décodage Prescription d'une spécialité             ¦
//  ¦                                                                 ¦
//  ¦             Début d'écriture :  7 Novembre 1990 (PA)            ¦
//  ¦             Dernière version : 25 Avril 1994 (PA)               ¦
//  +-----------------------------------------------------------------+
#include <owl\olemdifr.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring.h>
#include <math.h>

#include "dcodeur\nsphrobj.h"

#include "dzz\dzzspeci.h"
#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsepicap.h"
#include "nssavoir\nspatho.h"

decPrescription::decPrescription(NSContexte* pCtx, bool bAllLettersSentences)
                :decodage(pCtx)
{
	_sLibelle             = string("") ;
  _bAllLettersSentences = bAllLettersSentences ;
}

decPrescription::decPrescription(decodageBase* pBase, bool bAllLettersSentences)
                :decodage(pBase)
{
	_sLibelle             = string("") ;
  _bAllLettersSentences = bAllLettersSentences ;
}

//  +-----------------------------------------------------------------+  */
//  ¦             Décode code[], comlib[] et biop[]                   ¦  */
//  +-----------------------------------------------------------------+  */
//  Créé le 14/01/2001 Dernière mise à jour 14/01/2001                   */
void
decPrescription::decode(int colonne)
{
	if (string("") != _sLibelle)
  {
  	setDcodeur(string("")) ;
    metPhrase() ;
    setDcodeur(_sLibelle) ;
    metPhrase("3", "3"/*, 1*/) ;
    setDcodeur(string("")) ;
    metPhrase() ;
  }

  int refCol = getCol() ;

	while ((getCol() > colonne) && iBon())
	{
  	if (((getSt())[0] == '_') || ((getSt())[0] == 'I') ||
        ((getSt())[0] == 'N') || ((getSt())[0] == 'O') ||
        ((getSt())[0] == 'G'))
    {
    	decSpecialite Specia(this, _bAllLettersSentences, dcTiret) ;
      Specia.decode(refCol) ;
			Specia.donnePhrase() ;

      setDcodeur(string("")) ;
      metPhrase() ;
    }
		else
    	BioLibre(colonne) ;
  }
}

//  +-----------------------------------------------------------------+
//  ¦     Décodage d'une prescription de spécialité pharmaceutique    ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 22/07/1992

decSpecialite::decSpecialite(NSContexte* pCtx, bool bAllLettersSentences, int iDecodeType)
              :decLesion(pCtx, iDecodeType), _DkdPrescript(pCtx, dcTiret, string("fr"), bAllLettersSentences)
{
  _bAllLettersSentences = bAllLettersSentences ;
}

decSpecialite::decSpecialite(decodageBase* pBase, bool bAllLettersSentences, int iDecodeType)
              :decLesion(pBase, iDecodeType), _DkdPrescript(pBase, dcTiret, string("fr"), bAllLettersSentences)
{
  _bAllLettersSentences = bAllLettersSentences ;
}

void
decSpecialite::decode(int /* colonne */)
{
	_DkdPrescript.ammorce() ;

	// Affichage du nom de la spécialité
	//
	ajLL() ;

  string sLex = getStL() ;

  NSPathologData Data ;
  bool bTrouve = pContexte->getDico()->trouvePathologData(_sLangue, &sLex, &Data) ;
  if (false == bTrouve)
  {
  	Recupere() ;
    return ;
  }

  _iCompteur++ ;
  string sDrugNb = IntToString(_iCompteur) + string(". ") ;

  string sDrugCaption = string("") ;

  setDcodeurFromData(&Data) ;
	if (getDcodeur() != string(""))
	{
		setPseumajForFirstChar() ;

    if (pContexte->getEpisodus() && pContexte->getEpisodus()->addVirtualDrug())
    {
      string sVirtualDrug = getVirtualDrug(sLex) ;
      if (string("") != sVirtualDrug)
      {
        string sMsg = string("Virtual drug found: ") + sVirtualDrug ;
        pContexte->getSuperviseur()->trace(&sMsg, 1) ;

        string sRootName = getDrugRootName(sLex, getDcodeur()) ;
        if (string("") == sRootName)
          sRootName = getDcodeur() ;

        sDrugCaption = sVirtualDrug + string(" (") + sRootName + string(")") ;
      }
      else
      {
        sDrugCaption = getDcodeur() ;

        string sMsg = string("Virtual drug not found") ;
        pContexte->getSuperviseur()->trace(&sMsg, 1) ;
      }
    }
    else
    {
      sDrugCaption = getDcodeur() ;

      string sMsg = string("Not in virtual drug mode") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1) ;
    }

    string sDecimalSeparator    = pContexte->getSuperviseur()->getText("0localInformation", "decimalSeparator") ;
    string sDigitGroupSeparator = pContexte->getSuperviseur()->getText("0localInformation", "digitGroupSeparator") ;

    if (_bAllLettersSentences)
      sDrugCaption = getAllCharsText(sDrugCaption, sDecimalSeparator[0], sDigitGroupSeparator[0], 0) ;

    setDcodeur(sDrugNb + sDrugCaption) ;
    metPhrase("4", "4") ;
  }

	setDcodeur(string("")) ;
	int refCol = getCol() ;
	Avance() ;

	_DkdPrescript.decode(refCol) ;
}

void
decSpecialite::donnePhrase()
{
  if (false == iBon())
    return ;

	_DkdPrescript.metPhrase("4", "4") ;
}

string
decSpecialite::getVirtualDrug(const string sSpeciality)
{
  return pContexte->getSuperviseur()->getFilGuide()->getVirtualDrug(sSpeciality, pContexte, true) ;

/*
  if (string("") == sSpeciality)
    return string("") ;

  // Get all elements such as sSpeciality - is a -> element
  //
  string sCodeSens = pContexte->getDico()->donneCodeSens(&sSpeciality) ;

  VecteurString aVecteurString ;
  pContexte->getSuperviseur()->getFilGuide()->chercheEquivalent(sCodeSens, &aVecteurString, string("ES")) ;

  if (aVecteurString.empty())
    return string("") ;

  // Find if there is a virtual in the list
  //
  EquiItemIter itCodes = aVecteurString.begin() ;
  for ( ; itCodes != aVecteurString.end() ; itCodes++)
  {
    if (sCodeSens != **itCodes)
    {
      string sLibelleTrouve = string("") ;
      if (pContexte->getDico()->isVirtualDrug(_sLangue, *itCodes, &sLibelleTrouve))
        return sLibelleTrouve ;
    }
  }

  return string("") ;
*/
}

/**
 *  Get the root drug name (the shortest one that is father of concept and a drug)
 */
string
decSpecialite::getDrugRootName(const string sDrugCode, const string sDrugLabel)
{
  if ((string("") == sDrugCode) || (string("") == sDrugLabel))
    return string("") ;

  // Get all elements such as sSpeciality - is a -> element
  //
  string sCodeSens = pContexte->getDico()->donneCodeSens(&sDrugCode) ;

  VecteurString aIsA ;
  pContexte->getSuperviseur()->getFilGuide()->chercheEquivalent(sCodeSens, &aIsA, string("ES")) ;

  if (aIsA.empty())
    return string("") ;

  size_t iDrugLabelLen = strlen(sDrugLabel.c_str()) ;
  string sCandidate    = sDrugLabel ;
  size_t iCandidateLen = iDrugLabelLen ;

  string sLang = pContexte->getUserLanguage() ;

  for (EquiItemIter it = aIsA.begin() ; aIsA.end() != it ; it++)
  {
    string sCompleteCode = **it ;
    pContexte->getDico()->donneCodeComplet(sCompleteCode) ;

    string sLabel = string("") ;

    if (pContexte->getDico()->isDrug(&sCompleteCode))
		  pContexte->getDico()->donneLibelle(sLang, &sCompleteCode, &sLabel) ;

	  pContexte->getSuperviseur()->trace(&sLabel, 1) ;

    if (string("") != sLabel)
    {
      size_t iLabelLen = strlen(sLabel.c_str()) ;
      if ((iLabelLen < iCandidateLen) && (string(sCandidate, 0, iLabelLen) == sLabel))
      {
        sCandidate    = sLabel ;
        iCandidateLen = iLabelLen ;
      }
    }
  }

  return sCandidate ;
}

