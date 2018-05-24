//  +-----------------------------------------------------------------+
//  �               PROGRAMME SECONDAIRE : DZZSpeci.cpp               �
//  �                                                                 �
//  �              D�codage Prescription d'une sp�cialit�             �
//  �                                                                 �
//  �             D�but d'�criture :  7 Novembre 1990 (PA)            �
//  �             Derni�re version : 25 Avril 1994 (PA)               �
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

decPrescription::decPrescription(NSContexte* pCtx)
                :decodage(pCtx)
{
	sLibelle = string("") ;
}

decPrescription::decPrescription(decodageBase* pBase)
                :decodage(pBase)
{
	sLibelle = string("") ;
}

//  +-----------------------------------------------------------------+  */
//  �             D�code code[], comlib[] et biop[]                   �  */
//  +-----------------------------------------------------------------+  */
//  Cr�� le 14/01/2001 Derni�re mise � jour 14/01/2001                   */
void
decPrescription::decode(int colonne)
{
	if (string("") != sLibelle)
  {
  	setDcodeur(string("")) ;
    metPhrase() ;
    setDcodeur(sLibelle) ;
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
    	decSpecialite Specia(this, dcTiret) ;
      Specia.decode(refCol) ;
			Specia.donnePhrase() ;

      setDcodeur(string("")) ;
      metPhrase() ;
    }
		else
    	Recupere() ;
  }
	return ;
}

//  +-----------------------------------------------------------------+
//  �     D�codage d'une prescription de sp�cialit� pharmaceutique    �
//  +-----------------------------------------------------------------+
//  Cr�� le 07/11/1990 Derni�re mise � jour 22/07/1992

decSpecialite::decSpecialite(NSContexte* pCtx, int iDecodeType)
              :decLesion(pCtx, iDecodeType), _DkdPrescript(pCtx, dcTiret)
{
}

decSpecialite::decSpecialite(decodageBase* pBase, int iDecodeType)
              :decLesion(pBase, iDecodeType), _DkdPrescript(pBase, dcTiret)
{
}

void
decSpecialite::decode(int /* colonne */)
{
	_DkdPrescript.ammorce() ;

	// Affichage du nom de la sp�cialit�
	//
	ajLL() ;

  NSPathologData Data ;
  string sLex = getStL() ;
  bool bTrouve = pContexte->getDico()->trouvePathologData(_sLangue, &sLex, &Data) ;
  if (false == bTrouve)
  {
  	Recupere() ;
    return ;
  }

  _iCompteur++ ;
  string sDrugNb = IntToString(_iCompteur) + string(". ") ;

  setDcodeurFromData(&Data) ;
	if (getDcodeur() != string(""))
	{
		setPseumajForFirstChar() ;
    sDrugNb += getDcodeur() ;

    if (pContexte->getEpisodus() && pContexte->getEpisodus()->addVirtualDrug())
    {
      string sVirtualDrug = getVirtualDrug(sLex) ;
      if (string("") != sVirtualDrug)
        sDrugNb += string(" (") + sVirtualDrug + string(")") ;
    }

    setDcodeur(sDrugNb) ;
    metPhrase("4", "4") ;
  }

	setDcodeur(string("")) ;
	int refCol = getCol() ;
	Avance() ;

	_DkdPrescript.decode(refCol) ;

	return ;
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
}

