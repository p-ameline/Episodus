// -----------------------------------------------------------------------------
// Nsldvdrug.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.29 $
// $Author: pameline $
// $Date: 2014/10/31 15:01:29 $
// -----------------------------------------------------------------------------
// Ligne de Vie document
// -----------------------------------------------------------------------------
// FLP - aout 2003
// ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//           Ligne de vie - Document du modèle Document/Vue
// -----------------------------------------------------------------------------

#include <owl\filedoc.h>
#include <iostream.h>
#include <cstring.h>

#include "nssavoir\nsBdmDriver.h"
#include "partage\nsdivfct.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsdecode.h"
#include "nsbb\nsbb.h"
#include "nautilus\nsrechd2.h"
#include "dcodeur\nsdkd.h"
#include "dcodeur\decoder.h"
#include "dcodeur\nsphrase.h"
#include "dcodeur\nsgen.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nstrihis.h"
#include "nssavoir\nsgraphe.h"
#include "nautilus\nscsdoc.h"
#include "dcodeur\nsdkd.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nstlibre.h"
#include "nsbb\nsmedicdlg.h"
#include "nsbb\nsarc.h"
#include "nsldv\nsldvstructs.h"
#include "nssavoir\nsrightsmanager.h"
#include "nsldv\nsldvdrug.h"
#include "nsldv\nsldvvue_base.h"
#include "nsldv\nsldvgoal.h"
#include "nautilus\nsdrugview.h"
#include "nautilus\nsgoalview.h"
#include "nautilus\nsprocessview.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nscompub.h"
#include "nautilus\ns_html.h"
#include "nsoutil\ns_arche.h"
#include "nsoutil\ibdm.h"

#include "nautilus\nssoapview.h"

#include "ns_ob1\TypedVal.h"
#include "ns_ob1\NautilusType.h"
#include "ns_ob1\BB1Task.h"
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\BB1KS.h"
#include "ns_ob1\Interface.h"

const string drugPhaseSepar    = " | ";
const char   drugMMSSepar      = ':';
const char   drugMMSUpSepar    = '^';
const char   drugMMSBNoonSepar = '[';
const char   drugMMSANoonSepar = ']';
const char   drugMMSDownSepar  = '_';

long NSLdvDrugTake::lObjectCount = 0 ;
long NSLdvDrugCycle::lObjectCount = 0 ;
long NSLdvDrugPhase::lObjectCount = 0 ;
long NSLdvDrug::lObjectCount = 0 ;

//-----------------------------------------------------------------------//
//          					   Classe NSLdvDrugTake                            //
//-----------------------------------------------------------------------//

NSLdvDrugTake::NSLdvDrugTake(NSContexte *pCtx)
              :NSRoot(pCtx)
{
	_sNbDoses       = string("") ;
	_sAdminType     = string("") ;
	_sAdminLocation = string("") ;

  lObjectCount++ ;
}

NSLdvDrugTake::NSLdvDrugTake(const NSLdvDrugTake& rv)
              :NSRoot(rv.pContexte)
{
	_sNbDoses       = rv._sNbDoses ;
  _tpsClose       = rv._tpsClose ;
  _sAdminType     = rv._sAdminType ;
  _sAdminLocation = rv._sAdminLocation ;
  _tpsAdminHour   = rv._tpsAdminHour ;

  lObjectCount++ ;
}

NSLdvDrugTake::~NSLdvDrugTake()
{
  lObjectCount-- ;
}

void
NSLdvDrugTake::initFromTree(NSPatPathoArray* pTree, PatPathoIter iterSource)
{
	if ((NSPatPathoArray*) NULL == pTree)
		return ;

	PatPathoIter iter = iterSource ;

	int iColBase = (*iter)->getColonne() ;

	while ((pTree->end() != iter) && ((*iter)->getColonne() >= iColBase))
	{
		if ((*iter)->getColonne() == iColBase)
    {
    	string sSens = (*iterSource)->getLexiqueSens() ;

    	if (sSens == "VNBDO")
      {
      	iter++ ;
        if (pTree->end() != iter)
        {
					string sTemp ;
					string sUnite ;
					string sFormat ;
					string sValeur ;

          if (pContexte->getDico()->CodeCategorie((*iter)->getLexique()) == string(1, '£'))
          {
          	sFormat = (*iter)->getLexique() ;
            sValeur = (*iter)->getComplement() ;
            sUnite  = (*iter)->getUnit() ;

            std::string sNumSeparator = pContexte->getSuperviseur()->getText("0localInformation", "decimalSeparator") ;

            size_t i = sValeur.find(".") ;
  					if (i == NPOS)
  						_sNbDoses = sValeur ;
  					else
  					{
            	string sEntier = string("") ;
              string sDecima = string("") ;

  						if (i > 0)
    						sEntier = string(sValeur, 0, i) ;
              else
              	sEntier = string("0") ;
    					sDecima = string(sValeur, i+1, strlen(sValeur.c_str())-i-1) ;

    					_sNbDoses = sEntier + sNumSeparator + sDecima ;
  					}
          }
          iter++ ;
        }
      }
      else if (sSens == "KFERM")
      {
      	iter++ ;
        if (pTree->end() != iter)
        {
        	string sDate = (*iter)->getComplement() ;
          _tpsClose.initFromDateHeure(sDate) ;
          iter++ ;
        }
      }
      else if (sSens == "0ADMI")
      {
      	iter++ ;
        if (pTree->end() != iter)
        {
        	_sAdminType = (*iter)->getLexique() ;
          iter++ ;
        }
      }
      else if (sSens == "0VADM")
      {
      	iter++ ;
        if (pTree->end() != iter)
        {
        	_sAdminLocation = (*iter)->getLexique() ;
          iter++ ;
        }
      }
      else
      {
      	string sSens = (*iter)->getLexiqueSens() ;
        if (sSens == string("£H0"))
        {
        	string sHour = (*iter)->getComplement() ;
          string sDH   = string("00000000") + sHour ;
          _tpsAdminHour.initFromDateHeure(sDH) ;
      		iter++ ;
        }
        else
        	iter++ ;
      }
    }
    else
    	iter++ ;
  }
}

double
NSLdvDrugTake::getDoseAsDouble()
{
  return StringToDouble(_sNbDoses) ;
}

NSLdvDrugTake&
NSLdvDrugTake::operator=(const NSLdvDrugTake& src)
{
  if (this == &src)
		return *this ;

	_sNbDoses       = src._sNbDoses ;
  _tpsClose       = src._tpsClose ;
  _sAdminType     = src._sAdminType ;
  _sAdminLocation = src._sAdminLocation ;
  _tpsAdminHour   = src._tpsAdminHour ;

	return *this ;
}

//-----------------------------------------------------------------------//
//          					   Classe NSLdvDrugCycle                           //
//-----------------------------------------------------------------------//

NSLdvDrugCycle::NSLdvDrugCycle(NSContexte *pCtx, NSLdvDrugPhase* pParent)
               :NSRoot(pCtx)
{
	_sTitre      = string("") ;
	_sTitreCourt = string("") ;

  _dDailyDose  = 0 ;

  _pParentPhase = pParent ;

  lObjectCount++ ;
}

NSLdvDrugCycle::NSLdvDrugCycle(const NSLdvDrugCycle& rv)
               :NSRoot(rv.pContexte)
{
	_sTitre       = rv._sTitre ;
	_sTitreCourt  = rv._sTitreCourt ;

  _dDailyDose   = rv._dDailyDose ;

  _pParentPhase = rv._pParentPhase ;

  lObjectCount++ ;
}

NSLdvDrugCycle::~NSLdvDrugCycle()
{
  lObjectCount-- ;
}

void
NSLdvDrugCycle::setTitleFromTree(NSPatPathoArray* pTree, PatPathoIter iterSource, bool bLong, bool bShort)
{
	if ((NSPatPathoArray*) NULL == pTree)
		return ;

	string sSens = (*iterSource)->getLexiqueSens() ;
  if (string("KCYTR") != sSens)
  	return ;

	PatPathoIter iter = iterSource ;

	int iColBase = (*iter)->getColonne() ;
  iter++ ;

	while ((pTree->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		if ((*iter)->getColonne() == iColBase+1)
    {
    	sSens = (*iter)->getLexiqueSens() ;

    	if (string("KRYTH") == sSens)
      {
      	setTitleForCircadian(pTree, iter, bLong, bShort) ;
        // return ;
        iter++ ;
      }
    	else if (string("KRYTP") == sSens)
    	{
      	setTitleForNonCircadian(pTree, iter, bLong, bShort) ;
        // return ;
        iter++ ;
    	}
      else
      	iter++ ;
    }
    else
    	iter++ ;
  }
}

/*
void
NSLdvDrugCycle::setTitleForCircadian(NSPatPathoArray* pTree, PatPathoIter iterSource, bool bLong, bool bShort)
{
	if (!pTree)
		return ;

	string sSens = (*iterSource)->getLexiqueSens(pContexte) ;
  if (sSens != "KRYTH")
  	return ;

	PatPathoIter iter = iterSource ;

	int iColBase = (*iter)->getColonne() ;
  iter++ ;

	while ((iter != pTree->end()) && ((*iter)->getColonne() > iColBase))
	{
		if ((*iter)->getColonne() == iColBase+1)
    {
    	sSens = (*iter)->getLexiqueSens(pDoc->pContexte) ;

      if (sSens == std::string("KREVE"))
      {
      	iter++ ;
    	}
      else
      	iter++ ;
    }
    else
    	iter++ ;
  }
}
*/

void
NSLdvDrugCycle::setTitleForCircadian(NSPatPathoArray* pTree, PatPathoIter iterSource, bool bLong, bool bShort)
{
	if (((NSPatPathoArray*) NULL == pTree) || (NULL == iterSource) || (NULL == *iterSource) || (pTree->end() == iterSource))
		return ;

	string sSens = (*iterSource)->getLexiqueSens() ;
  if (string("KRYTH") != sSens)
  	return ;

  int iColBase = (*iterSource)->getColonne() ;

	PatPathoIter iter = iterSource ;
  iter++ ;

  string sCircadian[7] ;
  for (int i = 0 ; i < 7 ; i++)
  	sCircadian[i] = string("0") ;

  string sLang = pContexte->getUserLanguage() ;

  _dDailyDose = 0 ;

	while ((pTree->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		if ((*iter)->getColonne() == iColBase+1)
    {
    	sSens = (*iter)->getLexiqueSens() ;

      int iPosDose = -1 ;

      if      (sSens == std::string("KREVE"))
      	iPosDose = 0 ;
      else if (sSens == std::string("KMATI"))
      	iPosDose = 1 ;
    	else if (sSens == std::string("KMIDI"))
      	iPosDose = 2 ;
      else if (sSens == std::string("KGOUT"))
      	iPosDose = 3 ;
    	else if (sSens == std::string("KSOIR"))
      	iPosDose = 4 ;
    	else if (sSens == std::string("KCOUC"))
      	iPosDose = 5 ;
    	else if (sSens == std::string("KNOCT"))
      	iPosDose = 6 ;
      else if (sSens == std::string("VNBDO"))
      {
      	iter++ ;
        string sCompl = (*iter)->getComplement() ;

        if (bShort)
        {
        	string sShortCircadianCode = string("KRYTH3") ;
        	string sShortCircadianLib ;
  				pContexte->getDico()->donneLibelle(sLang, &sShortCircadianCode, &sShortCircadianLib) ;
          _sTitreCourt += string(" ") + sCompl + sShortCircadianLib ;
        }
				if (bLong)
        {
        	string sLongCircadianCode = string("KRYTH2") ;
        	string sLongCircadianLib ;
  				pContexte->getDico()->donneLibelle(sLang, &sLongCircadianCode, &sLongCircadianLib) ;
          _sTitre += string(" : ") + sCompl + string(" ") + sLongCircadianLib ;
        }
        return ;
      }
      //
      // Take at fixed time
      //
      else if (sSens == std::string("KHHMM"))
      {
      	iter++ ;
      	if (pTree->end() != iter)
        {
        	NSLdvDrugTake drugTake(pContexte) ;
          drugTake.initFromTree(pTree, iter) ;

        	if (drugTake._sNbDoses != string(""))
          	_sTitreCourt += string(" ") + drugTake._sNbDoses +
                     string("-") + drugTake._tpsAdminHour.donneFormattedHeure(pContexte, sLang) ;
        }
      }
      //
      // Cycle
      //
      else if (sSens == std::string("KCYCI"))
      {
        int iCycleCol = (*iter)->getColonne() ;

      	iter++ ;
      	while ((pTree->end() != iter) && ((*iter)->getColonne() > iCycleCol))
        {
          sSens = (*iter)->getLexiqueSens() ;

          // Irregular cycle
          //
          if      (std::string("KRYLI") == sSens)
          {
            CircBaseFree freeCycle((NSMedicCycleGlobal*) 0) ;
            if (freeCycle.ParseTree(iter, pTree->end()))
            {
              _sTitreCourt = freeCycle.getQuantity() + string("/") ;

              string sFreqUnit = freeCycle.getFreqUnit() ;
              string sLabel    = string("") ;
              pContexte->getDico()->donneLibelle(sLang, &sFreqUnit, &sLabel) ;

              _sTitreCourt += freeCycle.getFrequency() + string(" ") + sLabel ;

              _dDailyDose += freeCycle.getTakeCountPerDay(pContexte) ;
            }
          }
          // Regular cycle à la "take every X time units"
          //
          else if (std::string("KRYRE") == sSens)
          {
            CircBaseRegular regularCycle((NSMedicCycleGlobal*) 0) ;
            if (regularCycle.ParseTree(iter, pTree->end()))
            {
              _sTitreCourt = regularCycle.getQuantity() + string("/") ;

              string sFreqUnit = regularCycle.getFreqUnit() ;
              string sLabel    = string("") ;
              pContexte->getDico()->donneLibelle(sLang, &sFreqUnit, &sLabel) ;

              _sTitreCourt += regularCycle.getFrequency() + string(" ") + sLabel ;

              _dDailyDose += regularCycle.getTakeCountPerDay(pContexte) ;
            }
          }
          else
    	      iter++ ;
        }
      }
      else
      	iter++ ;

    	if (iPosDose >= 0)
      {
      	iter++ ;
        NSLdvDrugTake drugTake(pContexte) ;
        drugTake.initFromTree(pTree, iter) ;
        if (string("") != drugTake._sNbDoses)
        {
          string sDose = drugTake._sNbDoses ;
        	sCircadian[iPosDose] = sDose ;

          _dDailyDose += drugTake.getDoseAsDouble() ;
        }
    	}
    }
    else
    	iter++ ;
  }

  // Building the text
  //

  // Day periods ?
  int iFirst ;
  for (iFirst = 0 ; (iFirst < 7) && (sCircadian[iFirst] == string("0")) ; iFirst++) ;
  if (iFirst < 7)
  {
  	if (bShort)
    {
    	string sDrugMMSSepar = string(1, drugMMSSepar) ;

  		// is it more complex than MMSC
    	if ((sCircadian[0] != string("0")) || (sCircadian[3] != string("0")) || (sCircadian[6] != string("0")))
      {
      	string sDrugMMSUpSepar    = string(1, drugMMSUpSepar) ;
        string sDrugMMSBNoonSepar = string(1, drugMMSBNoonSepar) ;
        string sDrugMMSANoonSepar = string(1, drugMMSANoonSepar) ;
        string sDrugMMSDownSepar  = string(1, drugMMSDownSepar) ;

    		_sTitreCourt += sDrugMMSUpSepar    + sCircadian[0] + sDrugMMSSepar      + sCircadian[1] +
      	                sDrugMMSBNoonSepar + sCircadian[2] + sDrugMMSANoonSepar + sCircadian[3] +
                        sDrugMMSSepar      + sCircadian[4] + sDrugMMSSepar      + sCircadian[5] +
                        sDrugMMSDownSepar  + sCircadian[6] ;
      }
      // MMSC
      else if (sCircadian[5] != string("0"))
      	_sTitreCourt += sCircadian[1] + sDrugMMSSepar + sCircadian[2] +
                        sDrugMMSSepar + sCircadian[4] + sDrugMMSSepar + sCircadian[5] ;
      else
      	_sTitreCourt += sCircadian[1] + sDrugMMSSepar + sCircadian[2] +
                        sDrugMMSSepar + sCircadian[4] ;
    }
  }
}

void
NSLdvDrugCycle::setTitleForNonCircadian(NSPatPathoArray* pTree, PatPathoIter iterSource, bool bLong, bool bShort)
{
  if (((NSPatPathoArray*) NULL == pTree) || (NULL == iterSource) || (NULL == *iterSource) || (pTree->end() == iterSource))
		return ;

	string sSens = (*iterSource)->getLexiqueSens() ;
  if (string("KRYTP") != sSens)
  	return ;

  int iColBase = (*iterSource)->getColonne() ;

	PatPathoIter iter = iterSource ;
  iter++ ;

  string sLang = pContexte->getUserLanguage() ;

  size_t iDaysCountPerWeek = 0 ;

  string sRythmForDay = string("") ;

	while ((pTree->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		if ((*iter)->getColonne() == iColBase+1)
    {
    	sSens = (*iter)->getLexiqueSens() ;

      int iPosDose = -1 ;

      // Enumeration of days of week
      //
      if ((std::string("KJLUN") == sSens) || (std::string("KJMAR") == sSens) ||
          (std::string("KJMER") == sSens) || (std::string("KJJEU") == sSens) ||
          (std::string("KJVEN") == sSens) || (std::string("KJSAM") == sSens) ||
          (std::string("KJDIM") == sSens))
      {
        iDaysCountPerWeek++ ;

        string sCode  = (*iter)->getLexiqueSens() ;
        string sLabel = string("") ;
        pContexte->getDico()->donneLibelle(sLang, &sCode, &sLabel) ;

        if (string("") != sRythmForDay)
          sRythmForDay += string(",") ;

        sRythmForDay += string(sLabel, 0, 2) ;
      }
      // Irregular cycle
      //
      else if (std::string("KRYLI") == sSens)
      {
        CircBaseFree freeCycle((NSMedicCycleGlobal*) 0) ;
        if (freeCycle.ParseTree(iter, pTree->end()))
        {
          sRythmForDay = freeCycle.getQuantity() + string("/") ;

          string sFreqUnit = freeCycle.getFreqUnit() ;
          string sLabel    = string("") ;
          pContexte->getDico()->donneLibelle(sLang, &sFreqUnit, &sLabel) ;

          sRythmForDay += freeCycle.getFrequency() + string(" ") + sLabel ;

          // _dDailyDose += freeCycle.getTakeCountPerDay(pContexte) ;
        }
        else
          iter++ ;
      }
      // Regular cycle à la "take every X time units"
      //
      else if (std::string("KRYRE") == sSens)
      {
        CircBaseRegular regularCycle((NSMedicCycleGlobal*) 0) ;
        if (regularCycle.ParseTree(iter, pTree->end()))
        {
          sRythmForDay = regularCycle.getQuantity() + string("/") ;

          string sFreqUnit = regularCycle.getFreqUnit() ;
          string sLabel    = string("") ;
          pContexte->getDico()->donneLibelle(sLang, &sFreqUnit, &sLabel) ;

          sRythmForDay += regularCycle.getFrequency() + string(" ") + sLabel ;

          // _dDailyDose += regularCycle.getTakeCountPerDay(pContexte) ;
        }
        else
          iter++ ;
      }
      else
    	  iter++ ;
    }
    else
      iter++ ;
  }

  if (string("") == sRythmForDay)
    return ;

  if (string("") != _sTitreCourt)
    _sTitreCourt += string(" ") ;

  _sTitreCourt += sRythmForDay ;
}

NSLdvDrugCycle&
NSLdvDrugCycle::operator=(const NSLdvDrugCycle& src)
{
	if (this == &src)
		return *this ;

	_sTitre       = src._sTitre ;
	_sTitreCourt  = src._sTitreCourt ;

  _dDailyDose   = src._dDailyDose ;

  _pParentPhase = src._pParentPhase ;

  return *this ;
}

//-----------------------------------------------------------------------//
//          					   Classe NSLdvDrugPhase                           //
//-----------------------------------------------------------------------//

NSLdvDrugPhase::NSLdvDrugPhase(NSContexte *pCtx, NSLdvDrug* pParent)
               :NSRoot(pCtx)
{
	_sReference  = string("") ;

	_sTitre      = string("") ;
  _sTitreCourt = string("") ;

  _sIntakeUnitLib      = string("") ;
	_sIntakeUnitShortLib = string("") ;

  _pParentDrug = pParent ;

	initIntakeUnits() ;

  lObjectCount++ ;
}

NSLdvDrugPhase::NSLdvDrugPhase(const NSLdvDrugPhase& rv)
               :NSRoot(rv.pContexte)
{
	_sReference          = rv._sReference ;

	_sTitre              = rv._sTitre ;
  _sTitreCourt         = rv._sTitreCourt ;

  _sIntakeUnitLib      = rv._sIntakeUnitLib ;
  _sIntakeUnitShortLib = rv._sIntakeUnitShortLib ;

  _tDateOuverture      = rv._tDateOuverture ;
	_tDateFermeture      = rv._tDateFermeture ;

  _aCycles             = rv._aCycles ;
  _aPrescriptionRects  = rv._aPrescriptionRects ;

  _pParentDrug         = rv._pParentDrug ;

  lObjectCount++ ;
}

NSLdvDrugPhase::~NSLdvDrugPhase()
{
  lObjectCount-- ;
}

void
NSLdvDrugPhase::setTitlesFromCycles(bool bLong, bool bShort)
{
	_sTitre      = string("") ;
	_sTitreCourt = string("") ;

  if (_aCycles.empty())
		return ;

	NSLdvDrugCycleIter itCycle = _aCycles.begin() ;
  for ( ; _aCycles.end() != itCycle ; itCycle++)
  {
  	if (_aCycles.begin() != itCycle)
    {
    	_sTitre      += string(" - ") ;
			_sTitreCourt += drugPhaseSepar ;
    }
    _sTitre      += (*itCycle)->_sTitre ;
  	_sTitreCourt += (*itCycle)->_sTitreCourt ;
  }

  compressShortTitle() ;
}

void
NSLdvDrugPhase::compressShortTitle()
{
	return ;

/*
	// If it is a multi-cycle phase, we take out the 0s
  //
	size_t iPipePos = sTitreCourt.find("|") ;
  if (iPipePos == string::npos)
		return ;

	string sDecimalSeparator = pContexte->getSuperviseur()->getText("0localInformation", "decimalSeparator") ;
  if (sDecimalSeparator == string(""))
  	sDecimalSeparator = string(",") ;

  string sDigitGroupSeparator = pContexte->getSuperviseur()->getText("0localInformation", "digitGroupSeparator") ;

	bool bPreviousCharIsNonZeroUnit = false ;
	for (size_t iCurPos = 0 ; iCurPos < strlen(sTitreCourt.c_str()) ; )
	{
  	if (sTitreCourt[iCurPos] == '0')
    {
    	if (!bPreviousCharIsNonZeroUnit)
      {
      	bool bZeroKilled = true ;

      	if (iCurPos == strlen(sTitreCourt.c_str()) - 1)
        	sTitreCourt = string(sTitreCourt, 0, strlen(sTitreCourt.c_str()) - 1) ;
        else if (sTitreCourt[iCurPos+1] != sDecimalSeparator[0])
        {
      		if (iCurPos == 0)
      			sTitreCourt = string(sTitreCourt, 1, strlen(sTitreCourt.c_str()) - 1) ;
        	else
        		sTitreCourt = string(sTitreCourt, 0, iCurPos) + string(sTitreCourt, iCurPos + 1, strlen(sTitreCourt.c_str()) - iCurPos - 1) ;
        }
        else
        {
        	bZeroKilled = false ;
        	iCurPos++ ;
        }

        // If we killed a '0' we may have to kill the separator next to it
        //
				if (bZeroKilled && (iCurPos < strlen(sTitreCourt.c_str())))
        {
        	if (string(sTitreCourt, iCurPos, strlen(drugPhaseSepar.c_str())) == drugPhaseSepar)
          	bPreviousCharIsNonZeroUnit = false ;
          else if (isdigit(sTitreCourt[iCurPos]))
      			bPreviousCharIsNonZeroUnit = true ;
          else
          {
          	if (iCurPos == strlen(sTitreCourt.c_str()) - 1)
        			sTitreCourt = string(sTitreCourt, 0, strlen(sTitreCourt.c_str()) - 1) ;
      			if (iCurPos == 0)
      				sTitreCourt = string(sTitreCourt, 1, strlen(sTitreCourt.c_str()) - 1) ;
        		else
        			sTitreCourt = string(sTitreCourt, 0, iCurPos) + string(sTitreCourt, iCurPos + 1, strlen(sTitreCourt.c_str()) - iCurPos - 1) ;
          }
        }
      }
      else
      	iCurPos++ ;
    }
    else
    {
    	if ((sTitreCourt[iCurPos] == sDecimalSeparator[0]) ||
          (bPreviousCharIsNonZeroUnit && (sDigitGroupSeparator != string("")) && (sTitreCourt[iCurPos] == sDigitGroupSeparator[0])) ||
          isdigit(sTitreCourt[iCurPos]))
      	bPreviousCharIsNonZeroUnit = true ;
      else
      	bPreviousCharIsNonZeroUnit = false ;

    	iCurPos++ ;
    }
  }
*/
}

string
NSLdvDrugPhase::getShortestLibForIntakeUnit()
{
	if ((NSLdvDrug*) NULL == _pParentDrug)
		return string("") ;

	string sLexiCode = _pParentDrug->_sIntakeUnit ;
  string sLangue   = _pParentDrug->_pDoc->_sLangue ;
  string sReturn   = string("") ;

  pContexte->getDico()->donneShortLibelle(sLangue, &sLexiCode, &sReturn) ;

  return sReturn ;
}

void
NSLdvDrugPhase::initIntakeUnits()
{
	if (((NSLdvDrug*) NULL == _pParentDrug) || (string("") == _pParentDrug->_sIntakeUnit))
		return ;

	_sIntakeUnitShortLib = getShortestLibForIntakeUnit() ;
	pContexte->getDico()->donneLibelle(_pParentDrug->_pDoc->_sLangue, &(_pParentDrug->_sIntakeUnit), &_sIntakeUnitLib) ;
}

/**
 * Get the global dose (daily dose * days count)
 */
double
NSLdvDrugPhase::getPhaseDose() const
{
  if (_aCycles.empty())
    return 0 ;

  unsigned long lDaysCount = _tDateFermeture.daysBetween(_tDateOuverture) ;
  if (0 == lDaysCount)
    return 0 ;

  double dPhaseDose = 0 ;

  for (NSLdvDrugCycleConstIter it = _aCycles.begin() ; _aCycles.end() != it ; it++)
    dPhaseDose += lDaysCount * (*it)->_dDailyDose ;

  return dPhaseDose ;
}

NSLdvDrugPhase&
NSLdvDrugPhase::operator=(const NSLdvDrugPhase& src)
{
	if (&src == this)
		return *this ;

	_sReference          = src._sReference ;

	_sTitre              = src._sTitre ;
  _sTitreCourt         = src._sTitreCourt ;

  _sIntakeUnitLib      = src._sIntakeUnitLib ;
  _sIntakeUnitShortLib = src._sIntakeUnitShortLib ;

  _tDateOuverture      = src._tDateOuverture ;
	_tDateFermeture      = src._tDateFermeture ;

  _aCycles             = src._aCycles ;
  _aPrescriptionRects  = src._aPrescriptionRects ;

  _pParentDrug         = src._pParentDrug ;

	return *this ;
}

//-----------------------------------------------------------------------//
//          						    Classe NSLdvDrug                             //
//-----------------------------------------------------------------------//

NSLdvDrug::NSLdvDrug(NSContexte* pCtx, NSLdvDocumentBase* pDocum, LDVFRAME iFrame)
          :NSRoot(pCtx)
{
  _iFrame = iFrame ;

	_pDoc    = pDocum ;

	_tDateOuverture.init() ;
	_tDateFermeture.init() ;

  _sTitre      = string("") ;
  _sTitreCourt = string("") ;
	_sReference  = string("") ;
	_sLexique    = string("") ;
  _sCiCode     = string("") ;
  _sAtcCode    = string("") ;
  _sIntakeUnit = string("") ;
  _sFreeText   = string("") ;
  _bALD        = false ;

  _sLatestPrescriptionDoc  = string("") ;
  _sLatestPrescriptionDate = string("") ;

  _bToBePrescribed = false ;

  // _pWorstJalons = new GoalInfoArray() ;

  _pBamDrug = (NsSelectableDrug*) 0 ;

  lObjectCount++ ;
}

NSLdvDrug::NSLdvDrug(const NSLdvDrug& rv)
          :NSRoot(rv.pContexte)
{
  _iFrame         = rv._iFrame ;
	_pDoc           = rv._pDoc ;

	_sReference     = rv._sReference ;
	_sLexique       = rv._sLexique ;
  _sAtcCode       = rv._sAtcCode ;
  _sCiCode        = rv._sCiCode ;

	_sTitre         = rv._sTitre ;
  _sTitreCourt    = rv._sTitreCourt ;
	_tDateOuverture = rv._tDateOuverture ;
	_tDateFermeture = rv._tDateFermeture ;

  _sIntakeUnit    = rv._sIntakeUnit ;
  _aPhases        = rv._aPhases ;

  _sFreeText      = rv._sFreeText ;
  _bALD           = rv._bALD ;

	_aConcerns      = rv._aConcerns ;

  _sLatestPrescriptionDoc  = rv._sLatestPrescriptionDoc ;
  _sLatestPrescriptionDate = rv._sLatestPrescriptionDate ;

  _bToBePrescribed = rv._bToBePrescribed ;

/*
  if (rv._pWorstJalons)
    _pWorstJalons = new GoalInfoArray(*(rv._pWorstJalons)) ;
  else
    _pWorstJalons = (GoalInfoArray*) 0 ;
*/

  if (rv._pBamDrug)
    _pBamDrug = new NsSelectableDrug(*(rv._pBamDrug)) ;
  else
    _pBamDrug = (NsSelectableDrug*) 0 ;

  lObjectCount++ ;
}

NSLdvDrug::~NSLdvDrug()
{
/*
  if (_pWorstJalons)
	  delete _pWorstJalons ;
*/

  if (_pBamDrug)
    delete _pBamDrug ;

  lObjectCount-- ;
}

/*
void
NSLdvDrug::goalAdded(NSLdvGoal *pGoal)
{
	if ((NSLdvGoal*) NULL == pGoal)
		return ;

	_pDoc->goalAdded(pGoal, _pWorstJalons) ;
}

void
NSLdvDrug::goalModified(NSLdvGoal *pGoal)
{
	//
	// On reconstruit entièrement worstJalons
	// worstJalons is completely rebuilt
	//
	_pWorstJalons->vider() ;

  NSFrameInformationArray *pFrames = _pDoc->getFrames() ;
  if ((NSFrameInformationArray*) NULL == pFrames)
    return ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      ArrayGoals* pLdvGoals = pFrameInfo->getGoalsArray() ;
      if (pLdvGoals && (false == pLdvGoals->empty()))
		  {
	      // On passe en revue tous les objectifs
	      //
	      for (ArrayGoalIter goalIt = pLdvGoals->begin() ;
                                        pLdvGoals->end() != goalIt ; goalIt++)
		      if ((*goalIt)->getConcern() == _sReference)
    	      goalAdded(*goalIt) ;
      }
    }
  }
}
*/

NSLdvDrug&
NSLdvDrug::operator=(const NSLdvDrug& src)
{
	if (this == &src)
		return *this ;

  _iFrame          = src._iFrame ;
	_pDoc            = src._pDoc ;

	_sTitre          = src._sTitre ;
  _sTitreCourt     = src._sTitreCourt ;
	_tDateOuverture  = src._tDateOuverture ;
	_tDateFermeture  = src._tDateFermeture ;

	_sReference      = src._sReference ;
	_sLexique        = src._sLexique ;
  _sAtcCode        = src._sAtcCode ;
  _sCiCode         = src._sCiCode ;

  _sLatestPrescriptionDoc  = src._sLatestPrescriptionDoc ;
  _sLatestPrescriptionDate = src._sLatestPrescriptionDate ;

  _bToBePrescribed = src._bToBePrescribed ;

  _sIntakeUnit     = src._sIntakeUnit ;
  _aPhases         = src._aPhases ;

  _sFreeText       = src._sFreeText ;
  _bALD            = src._bALD ;

	_aConcerns       = src._aConcerns ;

/*
  _pWorstJalons->vider() ;
  if (src._pWorstJalons && (false == src._pWorstJalons->empty()))
  	*_pWorstJalons = *(src._pWorstJalons) ;
*/

  if (_pBamDrug)
    delete _pBamDrug ;

  if (src._pBamDrug)
    _pBamDrug = new NsSelectableDrug(*(src._pBamDrug)) ;
  else
    _pBamDrug = (NsSelectableDrug*) 0 ;

	return *this ;
}

bool
NSLdvDrug::bIsLinkedConcern(string sConcern)
{
	if (_aConcerns.empty())
		return false ;

	for (EquiItemIter i = _aConcerns.begin() ; _aConcerns.end() != i ; i++)
		if (*(*i) == sConcern)
			return true ;

	return false ;
}

void
NSLdvDrug::sendFocusEvent()
{
	sendEvent(FOCUS) ;
}

void
NSLdvDrug::sendActivationEvent()
{
	sendEvent(ACTIVATE) ;
}

void
NSLdvDrug::sendEvent(EventType iEvent)
{
  if ((NSLdvDocument*) NULL == _pDoc)
    return ;

  NSFrameInformationArray *pFrames = _pDoc->getFrames() ;
  if ((NSFrameInformationArray*) NULL == pFrames)
    return ;

  NSPatPathoArray EventPatho(pContexte->getSuperviseur()) ;

  PatPathoIter     pptIter  = 0 ;
  NSPatPathoArray* pPtIndex = (NSPatPathoArray*) 0 ;

  for (int i = 0 ; (i < FRAMECOUNT) && ((NSPatPathoArray*) NULL == pPtIndex) ; i++)
  {
    NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
    if (pFrameInfo)
    {
      NSDocumentHisto* pDocument = pFrameInfo->findNode(_sReference) ;
      if (pDocument)
      {
        NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;
        pDocument->initFromPatPatho(&DocPatho) ;

        if (false == DocPatho.empty())
		    {
          pptIter = DocPatho.ChercherNoeud(_sReference) ;
          if (pptIter && (DocPatho.end() != pptIter))
            EventPatho = DocPatho ;
        }
      }
    }
  }
  if (EventPatho.empty())
    return ;

	NautilusEvent nsEvent(&EventPatho, pptIter, iEvent, false) ;
  pContexte->getBBinterface()->addNautilusEvent(&nsEvent) ;
}

void
NSLdvDrug::initPhases(NSPatPathoArray* pTree, PatPathoIter iterSource)
{
	if ((NSPatPathoArray*) NULL == pTree)
		return ;

	string sSens = (*iterSource)->getLexiqueSens() ;
  if (string("KPHAT") != sSens)
  	return ;

	NSLdvDrugPhase* pPhase = new NSLdvDrugPhase(pContexte, this) ;

  pPhase->setNoeud((*iterSource)->getNode()) ;

  bool bOpeningDateIsSpecified = false ;
  bool bClosingDateIsSpecified = false ;
  // bool bDurationHasBeenSet     = false ;

	// Set default opening date : drug opening date if no previous phase exists
  //                            the previous phase ending date if there is one
	//
	NVLdVTemps tPeriodOpen ;
  if (_aPhases.empty())
  	tPeriodOpen = _tDateOuverture ;
  else
  	tPeriodOpen = _aPhases.back()->_tDateFermeture ;

  NVLdVTemps tPeriodStart = tPeriodOpen ;
  NVLdVTemps tPeriodClose ;

	PatPathoIter iter = iterSource ;

	int iColBase = (*iter)->getColonne() ;
  iter++ ;

	while ((pTree->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		if ((*iter)->getColonne() == iColBase+1)
    {
    	sSens = (*iter)->getLexiqueSens() ;

      // Dates
      //
      if ((string("KOUVR") == sSens) || (string("KFERM") == sSens))
      {
      	iter++ ;
        int iLigneBase = (*iter)->getLigne() ;

        string sUnite  = string("") ;
        string sFormat = string("") ;
        string sValeur = string("") ;
        while ((pTree->end() != iter) &&
                      ((*iter)->getLigne() == iLigneBase))
        {
        	if (pContexte->getDico()->CodeCategorie((*iter)->getLexique()) == string(1, '£'))
          {
          	sFormat = (*iter)->getLexiqueSens() ;
            sValeur = (*iter)->getComplement() ;
            sUnite  = (*iter)->getUnitSens() ;
            break ;
          }
          iter++ ;
        }

        // sFormat est du type £D0;03
        if ((string("") != sFormat) && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (string("") != sValeur))
        {
        	if ((sUnite == "2DA01") || (sUnite == "2DA02"))
          {
          	if (sSens == "KOUVR")
            {
            	tPeriodOpen.initFromDate(sValeur) ;
              bOpeningDateIsSpecified = true ;
            }
            else if (sSens == "KFERM")
            {
              tPeriodClose.initFromDate(sValeur) ;
              bClosingDateIsSpecified = true ;
            }
          }
        }
      }
      // Periods
      //
    	else if ((sSens == "VDURE") || (sSens == "VRENO"))
      {
      	iter++ ;

        // bDurationHasBeenSet = true ;

        string sUnitDureeCycle = (*iter)->getUnit() ;
				int    iDureeCycle     = atoi(((*iter)->getComplement()).c_str()) ;

        iter++ ;

        NVLdVTemps tPeriodEnd = tPeriodStart ;
        tPeriodEnd.ajouteTemps(iDureeCycle, sUnitDureeCycle, pContexte) ;

        if (bClosingDateIsSpecified && (tPeriodEnd > tPeriodClose))
        	tPeriodEnd = tPeriodClose ;
        if (bOpeningDateIsSpecified && (tPeriodStart < tPeriodOpen))
        	tPeriodStart = tPeriodOpen ;

        if (tPeriodStart > tPeriodClose)
        {
        	NVLdVRect* pPhaseRect = new NVLdVRect(NULL) ;
        	pPhaseRect->setLeft(tPeriodStart) ;
        	pPhaseRect->setRight(tPeriodEnd) ;
        	pPhase->_aPrescriptionRects.push_back(pPhaseRect) ;
        }

        tPeriodStart = tPeriodEnd ;
      }
    	else if (sSens == "KCYTR")
    	{
      	NSLdvDrugCycle* pCycle = new NSLdvDrugCycle(pContexte, pPhase) ;
      	pCycle->setTitleFromTree(pTree, iter, true, true) ;
        pPhase->_aCycles.push_back(pCycle) ;
      	iter++ ;
    	}
      //
      // Prescription renewed
      //
      else if (sSens == "GRENT")
    	{
        iter++ ;

        NVLdVTemps tRenewPeriodStart ;
        NVLdVTemps tRenewPeriodEnd ;

        NVLdVTemps tRenewPeriodOpen ;
        NVLdVTemps tRenewPeriodClose ;

        while ((pTree->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
	      {
          if ((*iter)->getColonne() == iColBase + 2)
          {
            sSens = (*iter)->getLexiqueSens() ;

            // Dates (KFERM is not supposed to exist, but lets suppose it can)
            //
            if ((string("KOUVR") == sSens) || (string("KFERM") == sSens))
            {
              iter++ ;

              string sUnite  = string("") ;
              string sFormat = string("") ;
              string sValeur = string("") ;
              while ((pTree->end() != iter) &&
                            ((*iter)->getColonne() == iColBase + 3))
              {
                if (pContexte->getDico()->CodeCategorie((*iter)->getLexique()) == string(1, '£'))
                {
                  sFormat = (*iter)->getLexiqueSens() ;
                  sValeur = (*iter)->getComplement() ;
                  sUnite  = (*iter)->getUnitSens() ;
                  break ;
                }
                iter++ ;
              }

              // sFormat est du type £D0;03
              if ((string("") != sFormat) && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (string("") != sValeur))
              {
                if ((sUnite == "2DA01") || (sUnite == "2DA02"))
                {
                  if      (string("KOUVR") == sSens)
                    tRenewPeriodOpen.initFromDate(sValeur) ;
                  else if (string("KFERM") == sSens)
                    tRenewPeriodClose.initFromDate(sValeur) ;
                }
              }
            }
            // Periods
            //
            else if ((string("VDURE") == sSens) || (string("VRENO") == sSens))
            {
              iter++ ;

              // bDurationHasBeenSet = true ;

              string sUnitDureeCycle = (*iter)->getUnit() ;
              int    iDureeCycle     = atoi(((*iter)->getComplement()).c_str()) ;

              iter++ ;

              if (tRenewPeriodStart.estVide())
              {
                tRenewPeriodStart = tRenewPeriodOpen ;
                tRenewPeriodEnd   = tRenewPeriodStart ;
              }

              tRenewPeriodEnd.ajouteTemps(iDureeCycle, sUnitDureeCycle, pContexte) ;
            }
            else
              iter++ ;
          }
          else
            iter++ ;
        }
        if ((false == tRenewPeriodClose.estVide()) && (tRenewPeriodEnd > tRenewPeriodClose))
          tRenewPeriodEnd = tRenewPeriodClose ;
        if ((false == tRenewPeriodOpen.estVide()) && (tRenewPeriodStart < tRenewPeriodOpen))
          tRenewPeriodStart = tRenewPeriodOpen ;

        if (tRenewPeriodStart > tRenewPeriodClose)
        {
          NVLdVRect* pPhaseRect = new NVLdVRect(NULL) ;
          pPhaseRect->setLeft(tRenewPeriodStart) ;
          pPhaseRect->setRight(tRenewPeriodEnd) ;
          pPhase->_aPrescriptionRects.push_back(pPhaseRect) ;
        }

        // Add the renewed period to the prescription period
        //
        if (false == bClosingDateIsSpecified)
        {
          tPeriodClose = tRenewPeriodEnd ;
          bClosingDateIsSpecified = true ;
        }
        else if (tRenewPeriodEnd > tPeriodClose)
          tPeriodClose = tRenewPeriodEnd ;
    	}
      else
      	iter++ ;
    }
    else
    	iter++ ;
  }

	pPhase->_tDateOuverture = tPeriodOpen ;
	pPhase->_tDateFermeture = tPeriodStart ;

  if (true == bClosingDateIsSpecified)
  {
  	pPhase->_tDateFermeture = tPeriodClose ;

    NVLdVRect* pPhaseRect = new NVLdVRect(NULL) ;
    pPhaseRect->setLeft(pPhase->_tDateOuverture) ;
		pPhaseRect->setRight(pPhase->_tDateFermeture) ;
    pPhase->_aPrescriptionRects.push_back(pPhaseRect) ;
  }

  pPhase->setTitlesFromCycles(true, true) ;

  // If the phase extends beyond the prescription scope, it means it has been
  // renewed. In this case, we have to update the closing date accordingly
  //
  if (pPhase->_tDateFermeture > _tDateFermeture)
    _tDateFermeture = pPhase->_tDateFermeture ;

  _aPhases.push_back(pPhase) ;
}

NSLdvDrugPhase*
NSLdvDrug::getCurrentActivePhase()
{
	if (_aPhases.empty())
		return (NSLdvDrugPhase*) 0 ;

	// récupération de la date du jour
  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

	NSLdvDrugPhaseIter itPhase = _aPhases.begin() ;
  for ( ; _aPhases.end() != itPhase ; itPhase++)
		if ((tpsNow > (*itPhase)->_tDateOuverture) && (tpsNow < (*itPhase)->_tDateFermeture))
    	return *itPhase ;

	return (NSLdvDrugPhase*) 0 ;
}

NSLdvDrugPhase*
NSLdvDrug::getLastActivePhase()
{
	if (_aPhases.empty())
		return (NSLdvDrugPhase*) 0 ;

  // If there is a current phase, then take it
  //
  NSLdvDrugPhase* pLastActivePhase = getCurrentActivePhase() ;
  if (pLastActivePhase)
    return pLastActivePhase ;

  // If there is no current phase, take the preceding one

	// récupération de la date du jour
  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

	NSLdvDrugPhaseIter itPhase = _aPhases.begin() ;
  for ( ; _aPhases.end() != itPhase ; itPhase++)
  {
		if (tpsNow > (*itPhase)->_tDateOuverture)
    {
      if ((NSLdvDrugPhase*) NULL == pLastActivePhase)
        pLastActivePhase = *itPhase ;
      else if (pLastActivePhase->_tDateFermeture < (*itPhase)->_tDateFermeture)
        pLastActivePhase = *itPhase ;
    }
  }

	return pLastActivePhase ;
}

NSLdvDrugPhase*
NSLdvDrug::getPhaseFromNode(string sNode)
{
	if (_aPhases.empty())
		return (NSLdvDrugPhase*) 0 ;

	NSLdvDrugPhaseIter itPhase = _aPhases.begin() ;
  for ( ; _aPhases.end() != itPhase ; itPhase++)
		if ((*itPhase)->getNoeud() == sNode)
    	return *itPhase ;

	return (NSLdvDrugPhase*) 0 ;
}

/*
string
NSLdvDrug::getPrescriptionDate()
{
  NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

  string sNodeMedic = getNoeud() ;

  NSFrameInformation* pFrame = _pDoc->getFrameForNode(sNodeMedic) ;
  if ((NSFrameInformation*) NULL == pFrame)
    return string("") ;

  ArrayLdvDrugs* pDrugs = pFrame->getTreatmentsArray() ;
  if ((ArrayLdvDrugs*) NULL == pDrugs)
    return string("") ;

  string sNodeRenew = pDrugs->getLastRenewNode(sNodeMedic) ;
  if ((string("") != sNodeRenew) && (sNodeRenew != sNodeMedic))
    sNodeMedic = sNodeRenew ;

  // on récupère le lien éventuel sur un traitement
  VecteurString aTreatments ;
  pGraphe->TousLesVrais(sNodeMedic, NSRootLink::treatmentOf, &aTreatments) ;
  if (aTreatments.empty())
    return string("") ;

  // on récupère le premier noeud pour avoir le code document ordonnance
  string sNodeOrdo = *(*(aTreatments.begin())) ;
  string sDocOrdo  = string(sNodeOrdo, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;

  // on recherche le pDocHis correspondant dans l'historique
  // et on affiche la date du document (en général date de création)
  if ((pContexte->getPatient()) && (pContexte->getPatient()->getDocHis()))
  {
    NSDocHistoArray* pVectDocument = pContexte->getPatient()->getDocHis()->getVectDocument() ;
    if (pVectDocument && (false == pVectDocument->empty()))
    {
      DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sDocOrdo) ;
      if (NULL != iterDoc)
        return (*iterDoc)->getDateDoc() ;
    }
  }

  return string("") ;
}
*/

bool
NSLdvDrug::isNarcotic()
{
  if (_pBamDrug)
    return _pBamDrug->isNarcotic() ;

  return false ;
}

void
NSLdvDrug::setTitlesFromLexique()
{
	_pDoc->pContexte->getDico()->donneLibelle(_pDoc->_sLangue, &_sLexique, &_sTitre) ;

  // Cut to the first unit
  //
  NSEpiFlechiesDB flechiesDB(pContexte) ;

  bool bKeepSearching = true ;
  size_t iBlankPos = _sTitre.find(" ") ;
  size_t iPrevPos  = 0 ;
  while (bKeepSearching && (NPOS != iBlankPos))
  {
  	string sWord = string(_sTitre, iPrevPos, iBlankPos - iPrevPos) ;
  	string sUnitCode = flechiesDB.getCodeLexiq(sWord, '2') ;
    if (string("") != sUnitCode)
    	bKeepSearching = false ;
    else if (iBlankPos < strlen(_sTitre.c_str()) - 1)
    {
    	iBlankPos++ ;
      while ((' ' == _sTitre[iBlankPos]) && (iBlankPos < strlen(_sTitre.c_str()) - 1))
      	iBlankPos++ ;

      iPrevPos = iBlankPos ;
      iBlankPos = _sTitre.find(" ", iPrevPos) ;
    }
  }

  if (NPOS == iBlankPos)
  	_sTitreCourt = _sTitre ;
  else
		_sTitreCourt = string(_sTitre, 0, iBlankPos) ;

  string(_sTitreCourt, stripBoth) ;
}

/*
bool
NSLdvDrug::modifyPhaseDate(string sPhaseNode, string sDateType, NVLdVTemps tNewDate)
{
	NSLdvDrugPhase* pPhase = getPhaseFromNode(sPhaseNode) ;
	if ((NSLdvDrugPhase*) NULL == pPhase)
		return (NSLdvDrugPhase*) 0 ;

	return modifyPhaseDate(pPhase, sDateType, tNewDate) ;
}

bool
NSLdvDrug::modifyPhaseDate(NSLdvDrugPhase* pPhase, string sDateType, NVLdVTemps tNewDate)
{
	if ((NSLdvDrugPhase*) NULL == pPhase)
		return false ;

	return pPhase->modifyDate(sDateType, tNewDate) ;
}
*/

NVLdVTemps
NSLdvDrug::getPhaseDate(string sPhaseNode, string sDateType)
{
	NSLdvDrugPhase* pPhase = getPhaseFromNode(sPhaseNode) ;
	if ((NSLdvDrugPhase*) NULL == pPhase)
  {
  	NVLdVTemps tUndefined ;
		tUndefined.init() ;
		return tUndefined ;
	}

	return getPhaseDate(pPhase, sDateType) ;
}

NVLdVTemps
NSLdvDrug::getPhaseDate(NSLdvDrugPhase* pPhase, string sDateType)
{
	if ((NSLdvDrugPhase*) NULL == pPhase)
	{
		NVLdVTemps tUndefined ;
		tUndefined.init() ;
		return tUndefined ;
	}

	string sSens ;
	pContexte->getDico()->donneCodeSens(&sDateType, &sSens) ;

  if      (string("KOUVR") == sSens)
		return pPhase->_tDateOuverture ;
	else if (string("KFERM") == sSens)
		return pPhase->_tDateFermeture ;

	NVLdVTemps tUndefined ;
  tUndefined.init() ;
  return tUndefined ;
}

bool
NSLdvDrug::addNewPhase(NSPatPathoArray* pPhaseTree)
{
	return false ;
}

bool
NSLdvDrug::addNewActivePhase(NSPatPathoArray* pPhaseTree)
{
	return false ;
}

/*
bool
NSLdvDrug::createXmlTree(Ctree* pTreeObject, string sTargetLocalisation)
{
	if ((Ctree*) NULL == pTreeObject)
		return false ;

	// Setting the "localisation" attribute
  //
  if (string("") == sTargetLocalisation)
		sTargetLocalisation = string("ZPOMR/N0000") ;

  string sLoc = pTreeObject->getStringAttribute(ATTRIBUT_TREE_LOC) ;
  if (string("") != sLoc)
  {
  	if (sTargetLocalisation != sLoc)
    	pTreeObject->setStringAttribute(ATTRIBUT_TREE_LOC, sTargetLocalisation) ;
  }
  else
  	pTreeObject->addAttribute(ATTRIBUT_TREE_LOC, sTargetLocalisation) ;

	//
  // Adding Root nodes and asking them to create their sons recursively
  //

	// Looking for this drug in the health index document
  //
	NSPatPathoArray* pPatho ;
	PatPathoIter iter = _pDoc->donneDrug(this, &pPatho) ;
  // not found !
	if ((NULL == iter) || (pPatho->end() == iter))
  	return false ;

	NSPatPathoArray drugPatho(pContexte->getSuperviseur()) ;
  pPatho->ExtrairePatPathoWithRoot(iter, &drugPatho) ;

  // First step: remove referential Ids
  //
  PatPathoIter RefIdIter ;
  bool bIsID = drugPatho.CheminDansPatpatho(string("£RE"), string(1, cheminSeparationMARK), &RefIdIter) ;
  while (true == bIsID)
  {
    drugPatho.SupprimerItem(RefIdIter) ;
    bIsID = drugPatho.CheminDansPatpatho(string("£RE"), string(1, cheminSeparationMARK), &RefIdIter) ;
  }

	int iCol = 0 ;

	// Find opening
  //
  PatPathoIter iter2 = drugPatho.begin() ;
  iter2++ ;
  PatPathoIter iterClose = 0 ;

  int iTotalDurationValueYears  = 0 ;
  int iTotalDurationValueMonths = 0 ;
  int iTotalDurationValueDays   = 0 ;

  while (drugPatho.end() != iter2)
  {
    if (iCol + 1 == (*iter2)->getColonne())
    {
    	string sSens = (*iter2)->getLexiqueSens() ;

      // Opening date : switch to "now"
      //
      if      (string("KOUVR") == sSens)
      {
      	iter2++ ;
        if (drugPatho.end() != iter2)
        {
        	sSens = (*iter2)->getLexiqueSens() ;
          if ((string("£D0") == sSens) || (string("£T0") == sSens))
          	(*iter2)->setComplement("$KAUJO+000000") ;
        }
      }
      else if (string("KFERM") == sSens)
      {
      	iter2++ ;
        if (drugPatho.end() != iter2)
        {
        	sSens = (*iter2)->getLexiqueSens() ;
          if ((string("£D0") == sSens) || (string("£T0") == sSens))
          	iterClose = iter2 ;
        }
      }
      else if (string("KPHAT") == sSens)
      {
      	int iColPhase = (*iter2)->getColonne() ;
      	iter2++ ;

        PatPathoIter iterPhaseClose = 0 ;

        int iPhaseDurationValueYears  = 0 ;
				int iPhaseDurationValueMonths = 0 ;
				int iPhaseDurationValueDays   = 0 ;

        while ((drugPatho.end() != iter2) && ((*iter2)->getColonne() > iColPhase))
        {
        	sSens = (*iter2)->getLexiqueSens() ;

          // Opening date : switch to "now"
      		//
      		if      (string("KOUVR") == sSens)
      		{
      			iter2++ ;
        		if (drugPatho.end() != iter2)
        		{
        			sSens = (*iter2)->getLexiqueSens() ;
          		if ((string("£D0") == sSens) || (string("£T0") == sSens))
              {
              	string sDeltaTps = donne_deltaTemps(iTotalDurationValueYears, iTotalDurationValueMonths, iTotalDurationValueDays) ;
          			(*iter2)->setComplement(string("$KAUJO+") + sDeltaTps) ;
              }
        		}
      		}
      		else if (string("KFERM") == sSens)
      		{
      			iter2++ ;
        		if (drugPatho.end() != iter2)
        		{
        			sSens = (*iter2)->getLexiqueSens() ;
          		if ((string("£D0") == sSens) || (string("£T0") == sSens))
          			iterPhaseClose = iter2 ;
        		}
      		}
          else if (string("VDURE") == sSens)
      		{
      			iter2++ ;
        		if (drugPatho.end() != iter2)
        		{
        			sSens = (*iter2)->getLexiqueSens() ;
          		if (string("£N0") == sSens)
              {
                string sValue = (*iter2)->getComplement() ;
                int    iPhaseDurationValue = atoi(sValue.c_str()) ;
								string sPhaseDurationUnit  = (*iter2)->getUnitSens() ;

                // Year
            		if      (string("2DAT3") == sPhaseDurationUnit)
                	iPhaseDurationValueYears  = iPhaseDurationValue ;
            		// Month
            		else if (string("2DAT2") == sPhaseDurationUnit)
                	iPhaseDurationValueMonths = iPhaseDurationValue ;
                // Week
            		else if (string("2DAT1") == sPhaseDurationUnit)
                	iPhaseDurationValueDays   = 7 * iPhaseDurationValue ;
            		// Day
            		else if (string("2DAT0") == sPhaseDurationUnit)
                	iPhaseDurationValueDays   = iPhaseDurationValue ;
              }
        		}
      		}
          iter2++ ;
        }
        iTotalDurationValueYears  = iPhaseDurationValueYears ;
				iTotalDurationValueMonths = iPhaseDurationValueMonths ;
				iTotalDurationValueDays   = iPhaseDurationValueDays ;

        if (NULL != iterPhaseClose)
        {
        	string sDeltaTps = donne_deltaTemps(iTotalDurationValueYears, iTotalDurationValueMonths, iTotalDurationValueDays) ;
          (*iterPhaseClose)->setComplement(string("$KAUJO+") + sDeltaTps) ;
        }
      }
    }
    if (drugPatho.end() != iter2)
  		iter2++ ;
  }
  if (iterClose)
  {
  	string sDeltaTps = donne_deltaTemps(iTotalDurationValueYears, iTotalDurationValueMonths, iTotalDurationValueDays) ;
    (*iterClose)->setComplement(string("$KAUJO+") + sDeltaTps) ;
  }

	*(pTreeObject->pPatPathoArray) = drugPatho ;

	return pTreeObject->initFromPatPatho() ;
}
*/

/**
 * Initialize the Bam drug object by calling the Bam
 */
bool
NSLdvDrug::initBamDrug()
{
  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if ((NSBdmDriver*) NULL == pDriver)
    return false ;

  if (string("") == _sCiCode)
  {
    InterfaceBdm bdm(pContexte) ;

    string sLexiqueSens = string("") ;
    pContexte->getDico()->donneCodeSens(&_sLexique, &sLexiqueSens) ;
    if (bdm.isCodeLexiMedInDB(sLexiqueSens))
      _sCiCode = bdm.pBdm->getCodeCIP() ;
  }

  if (string("") == _sCiCode)
    return false ;

  if ((NsSelectableDrug*) NULL == _pBamDrug)
    _pBamDrug = new NsSelectableDrug() ;

  _pBamDrug->setPrescribedNodeId(_sReference) ;

  if (pDriver->getDrugInformation(_pBamDrug, &_sCiCode))
    return true ;

  delete _pBamDrug ;
  _pBamDrug = (NsSelectableDrug*) 0 ;

  return false ;
}

//-----------------------------------------------------------------------//
//				   		 Classe ArrayLdvDrugs                            //
//-----------------------------------------------------------------------//

ArrayLdvDrugs::ArrayLdvDrugs(const ArrayLdvDrugs& rv)
              :ArrayDrugs()
{
try
{
	_pFrame = rv._pFrame ;

	if (false == rv.empty())
		for (drugsConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSLdvDrug(*(*i))) ;
}
catch (...)
{
	erreur("Exception ArrayLdvDrugs copy ctor.", standardError, 0) ;
}
}

void
ArrayLdvDrugs::vider()
{
	if (empty())
		return ;

	for (drugsIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

ArrayLdvDrugs::~ArrayLdvDrugs()
{
	vider() ;
}

void
ArrayLdvDrugs::initialiser()
{
  if ((NSFrameInformation*) NULL == _pFrame)
    return ;

  NSPatPathoArray PatPathoIndex(_pFrame->pContexte->getSuperviseur()) ;
  _pFrame->initFromPatPathoIndex(&PatPathoIndex) ;

  if (PatPathoIndex.empty())
    return ;

  initDrugsFromPatho(&PatPathoIndex) ;

  NSDocumentHistoArray* pVectDoc = _pFrame->getTreatmentDocs() ;
  if (((NSDocumentHistoArray*) NULL == pVectDoc) || pVectDoc->empty())
    return ;

  for (DocumentIter i = pVectDoc->begin() ; pVectDoc->end() != i ; i++)
    if (*i)
    {
      NSPatPathoArray DocPatho(_pFrame->pContexte->getSuperviseur()) ;
      (*i)->initFromPatPatho(&DocPatho) ;

      initDrugsFromPatho(&DocPatho) ;
    }
}

void
ArrayLdvDrugs::initDrugsFromPatho(NSPatPathoArray* pPpt)
{
  if (((NSPatPathoArray*) NULL == pPpt) || pPpt->empty())
    return ;

  // On cherche le chapitre "traitement"
	PatPathoIter iter = pPpt->ChercherItem("N00001") ;
	if (((PatPathoIter) NULL == iter) || (pPpt->end() == iter))
    return ;

  int iColBase = (*iter)->getColonne() ;

  iter++ ;
  if (((PatPathoIter) NULL == iter) || (pPpt->end() == iter))
    return ;

  loadDrugs(pPpt, iter, iColBase) ;
}

void
ArrayLdvDrugs::reinit()
{
  if ((NSFrameInformation*) NULL == _pFrame)
    return ;

  NSPatPathoArray PptIndex(_pFrame->pContexte->getSuperviseur()) ;
  _pFrame->initFromPatPathoIndex(&PptIndex) ;

  if (PptIndex.empty())
    return ;

  reinitDrugsFromPatho(&PptIndex) ;

  NSDocumentHistoArray* pVectDoc = _pFrame->getTreatmentDocs() ;
  if (((NSDocumentHistoArray*) NULL == pVectDoc) || pVectDoc->empty())
    return ;

  for (DocumentIter i = pVectDoc->begin() ; pVectDoc->end() != i ; i++)
    if (*i)
    {
      NSPatPathoArray DocPatho(_pFrame->pContexte->getSuperviseur()) ;
      (*i)->initFromPatPatho(&DocPatho) ;

      reinitDrugsFromPatho(&DocPatho) ;
    }
}

void
ArrayLdvDrugs::reinitDrugsFromPatho(NSPatPathoArray* pPpt)
{
  if ((NULL == pPpt) || pPpt->empty())
    return ;

  // On cherche le chapitre "traitement"
	PatPathoIter iter = pPpt->ChercherItem("N00001") ;
	if (((PatPathoIter) NULL == iter) || (pPpt->end() == iter))
    return ;

  int iColBase = (*iter)->getColonne() ;

  iter++ ;
  if (((PatPathoIter) NULL == iter) || (pPpt->end() == iter))
    return ;

  reloadDrugs(pPpt, iter, iColBase) ;
}

void
ArrayLdvDrugs::reloadDrugs(NSPatPathoArray* pPtIndex, PatPathoIter iter, int iColBase, bool bJustOne)
{
try
{
	if (((NSPatPathoArray*) NULL == pPtIndex) || (pPtIndex->empty()))
  	return ;

	// NSPatPathoArray* pPtIndex = pDoc->pPathoPOMRIndex;

	NSLdvDrug* pDrug = (NSLdvDrug*) 0 ;

	bool bTourner = true ;

	while ((bTourner) && (pPtIndex->end() != iter) &&
         ((*iter)->getColonne() > iColBase))
	{
		// Drug
    //
		if ((*iter)->getColonne() == iColBase + 1)
		{
      // If a drug was already being processed, then add it to the array
      //
			if (pDrug)
			{
				bool    b2Add = true ;
        drugsIter drIter ;

				if (false == empty())
				{
					for (drIter = begin() ; end() != drIter ; drIter++)
          {
						if ((*drIter)->getNoeud() == pDrug->getNoeud())
            {
							b2Add = false ;
              break ;
            }
          }
				}

				if (b2Add)
				{
					NSLdvDrug* pDg = new NSLdvDrug(*pDrug) ;
          pDg->initBamDrug() ;
					push_back(pDg) ;
				}
				else // on remet à jour le medicament
				{
        	*(*drIter) = *pDrug ;
        }

        // Mise à jour des vues - Refresh the views
        /*
        TView* pView = pDoc->GetViewList();
        do
        {
            NSLdvView* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView);
            //if (pLdvView)
            //	pLdvView->addProb(pPb) ;

            pView = pDoc->NextView(pView);
        }
        while (pView);
        */

				delete pDrug ;
				pDrug = (NSLdvDrug*) 0 ;
			}

      // Processing the new drug
      //
      string sCodeLex = (*iter)->getLexique() ;
      string sCodeSensDrug ;
    	_pFrame->pContexte->getDico()->donneCodeSens(&sCodeLex, &sCodeSensDrug) ;

      if (bJustOne)
      	bTourner = false ;

      //
      // Médicament caché -- Hidden drug
      //
      if (string("90000") == sCodeSensDrug)
      {
      	iter++ ;
      	while ((pPtIndex->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
        	iter++ ;
      }
      //
      // Médicament visible -- Visible drug
      //
      else
      {
        pDrug = new NSLdvDrug(_pFrame->pContexte, _pFrame->getDoc(), _pFrame->getFrame()) ;

        // Libellé
        pDrug->setLexique(sCodeLex) ;

        if (string("£?????") != sCodeLex)
          pDrug->setTitlesFromLexique() ;
        // Texte libre - Free text
        else
        	pDrug->_sTitre = (*iter)->getTexteLibre() ;

        // Noeud
        pDrug->setNoeud((*iter)->getNode()) ;

        iter++ ;

        // Drug's parameters
        // Paramètres du médicament
        //
        while ((pPtIndex->end() != iter) &&
                ((*iter)->getColonne() > iColBase + 1))
        {
          string sSens = (*iter)->getLexiqueSens() ;

          if ((*iter)->getColonne() == iColBase+2)
          {
            // Dates
            if ((string("KOUVR") == sSens) || (string("KFERM") == sSens))
            {
              iter++ ;
              int iLigneBase = (*iter)->getLigne() ;
              // gereDate* pDate = new gereDate(pContexte);
              string sUnite  = string("") ;
              string sFormat = string("") ;
              string sValeur = string("") ;
              while ((pPtIndex->end() != iter) &&
                      ((*iter)->getLigne() == iLigneBase))
              {
                if (_pFrame->pContexte->getDico()->CodeCategorie((*iter)->getLexique()) == string(1, '£'))
                {
                  sFormat = (*iter)->getLexiqueSens() ;
                  sValeur = (*iter)->getComplement() ;
                  sUnite  = (*iter)->getUnitSens() ;
                  break ;
                }
								iter++ ;
              }

              // sFormat est du type £D0;03
              if ((string("") != sFormat) &&
                  (('D' == sFormat[1]) || ('T' == sFormat[1])) &&
                  (string("") != sValeur))
              {
                if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
                {
                	if      (string("KOUVR") == sSens)
                    pDrug->_tDateOuverture.initFromDate(sValeur) ;
                  else if (string("KFERM") == sSens)
                  	pDrug->_tDateFermeture.initFromDate(sValeur) ;
                }
              }
            }
            // Unité de prise
            //
            else if (string("0MEDF") == sSens)
            {
            	iter++ ;
              if ((iter != pPtIndex->end()) && ((*iter)->getColonne() > iColBase + 2))
              	pDrug->_sIntakeUnit = (*iter)->getLexique() ;
            }
            // Phases
            //
            else if (string("KPHAT") == sSens)
            {
            	pDrug->initPhases(pPtIndex, iter) ;
              iter++ ;
              while ((iter != pPtIndex->end()) &&
                     ((*iter)->getColonne() > iColBase + 2))
              	iter++ ;
            }
            else if (string("£C;") == sSens)
            {
              pDrug->_sFreeText = (*iter)->getTexteLibre() ;
              iter++ ;
            }
            else if (string("LADMI") == sSens)
            {
              int iColBaseAdmin = (*iter)->getColonne() ;
              iter++ ;
              while ((pPtIndex->end() != iter) && ((*iter)->getColonne() > iColBaseAdmin))
              {
                std::string tempAdm = (*iter)->getLexiqueSens() ;
                if ("LBARZ" == tempAdm)
                  pDrug->_bALD = true ;
                iter++ ;
              }
            }
            else if (string("6ATC0") == sSens)
            {
              pDrug->setAtcCode((*iter)->getComplement()) ;
              iter++ ;
            }
            else if ((string("6CIS0") == sSens) ||
                     (string("6CIP0") == sSens) ||
                     (string("6CIP7") == sSens) ||
                     (string("6CIPT") == sSens))
            {
              pDrug->setCiCode((*iter)->getComplement()) ;
              iter++ ;
            }
            else
              iter++ ;
          }
          else
            iter++ ;
        }
        if (pDrug->_tDateFermeture.estVide())
        	pDrug->_tDateFermeture.setNoLimit() ;
  		}
		}
		else
			iter++ ;
	}

	if (pDrug)
	{
    bool    b2Add = true ;
    drugsIter drIter ;

    if (false == empty())
    {
    	for (drIter = begin() ; end() != drIter ; drIter++)
      {
      	if ((*drIter)->getNoeud() == pDrug->getNoeud())
        {
        	b2Add = false ;
          break ;
        }
      }
    }

    if (b2Add)
    {
    	NSLdvDrug *pDg = new NSLdvDrug(*pDrug) ;
      pDg->initBamDrug() ;
      push_back(pDg) ;
    }
    else // on remet à jour le medicament
    {
    	*(*drIter) = *pDrug ;
    }

    // Mise à jour des vues - Refresh the views
    /*
    TView* pView = pDoc->GetViewList();
    do
    {
        NSLdvView* pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView);
        //if (pLdvView)
        //	pLdvView->addProb(pPb) ;

        pView = pDoc->NextView(pView);
    }
    while (pView);
    */

    delete pDrug ;
    pDrug = (NSLdvDrug*) 0 ;
	}
}
catch (...)
{
	erreur("Exception ArrayLdvDrugs::reloadDrugs.", standardError, 0) ;
  return ;
}
}

bool
ArrayLdvDrugs::deleteDrug(NSLdvDrug* pDrug)
{
	if (empty())
		return false ;

	for (drugsIter i = begin() ; end() != i ; i++)
	{
		if (*i == pDrug)
		{
			delete pDrug ;
			erase(i) ;
			return true ;
		}
	}

  return false ;
}

bool
ArrayLdvDrugs::deleteDrug(string sRef)
{
	if (empty())
		return false ;

	for (drugsIter i = begin() ; end() != i ; i++)
	{
		if ((*i)->getNoeud() == sRef)
		{
			delete *i ;
			erase(i) ;
			return true ;
		}
	}

  return false ;
}

bool
ArrayLdvDrugs::getRenewPatPatho(NSPatPathoArray* pPPT, string sNodeRenew)
{
	if (((NSPatPathoArray*) NULL == pPPT) || (string("") == sNodeRenew))
  	return false ;

  if ((NSFrameInformation*) NULL == _pFrame)
    return false ;

  NSDocumentHisto* pDocum = _pFrame->findNode(sNodeRenew) ;
  if ((NSDocumentHisto*) NULL == pDocum)
    return false ;

  NSPatPathoArray PptIndex(_pFrame->pContexte->getSuperviseur()) ;
  pDocum->initFromPatPatho(&PptIndex) ;

  if (PptIndex.empty())
    return false ;

	// On cherche le chapitre "traitement"
	PatPathoIter pptIt = PptIndex.ChercherItem("N00001") ;

	if (((PatPathoIter) NULL == pptIt) || (PptIndex.end() == pptIt))
  	return false ;   // cas aucun traitement déjà effectué

  // on note la colonne de base
  int iColBase = (*pptIt)->getColonne() ;

  // on se place sur le noeud renouvelement
  pptIt = PptIndex.ChercherNoeud(sNodeRenew) ;

	if (((PatPathoIter) NULL == pptIt) || (PptIndex.end() == pptIt))
  	return false ;   // cas d'erreur : noeud introuvable

	string sNodePhaseID = string("") ;
	string sNodeRenewID = string(sNodeRenew, strlen(sNodeRenew.c_str()) - PPD_NOEUD_LEN, PPD_NOEUD_LEN) ;
	bool bFirstPhase = true ;

  // on remonte en arrière jusqu'au noeud médicament
  // on retient le NodeID de la phase englobante au passage
  // Remarque : on retient ici uniquement le nodeID (valeur du noeud sans treeID)
  // car on compare ensuite cette valeur dans une nouvelle patpatho qui n'a pas de treeID
  while ((*pptIt)->getColonne() > iColBase+1)
  {
  	string sElemLex = (*pptIt)->getLexiqueSens() ;
    if (bFirstPhase && (sElemLex == "KPHAT"))
    {
    	sNodePhaseID = (*pptIt)->getNodeID() ;
      bFirstPhase = false ;
    }

    pptIt-- ;
  }

  if ((string("") == sNodePhaseID) || ((*pptIt)->getColonne() != iColBase+1))
  	return false ;   // cas d'erreurs : phase ou médicament introuvables

  string sCodeMedic = (*pptIt)->getLexique() ;
  string sNodeMedic = (*pptIt)->getNode() ;

  // on constitue la patpatho du médicament
  NSPatPathoArray SousPPT(_pFrame->pContexte->getSuperviseur()) ;
  _pFrame->pContexte->getPatient()->DonneSousArray(sNodeMedic, &SousPPT) ;

  pPPT->vider();
  // on reconstitue la patpatho à partir du noeud
  pPPT->ajoutePatho(sCodeMedic, 0) ;
  // Insertion en queue (iter doit être ici egal à end())
  pPPT->InserePatPatho(pPPT->end(), &SousPPT, 1);

  // on va maintenant "faire le ménage" dans cette patpatho :
  // - on supprime toutes les phases sauf celle qui correspond à sNodePhase
  // - dans cette phase, on supprime la durée, le nombre de renouvelements
  //   et tous les renouvelements sauf celui qui correspond à sNodeRenew
  pptIt = pPPT->begin() ;
  pptIt++ ;

  while (pPPT->end() != pptIt)
  {
    string sSens = (*pptIt)->getLexiqueSens() ;
    string sCurrentNodeID = (*pptIt)->getNodeID() ;

    if (string("KPHAT") == sSens)
    {
    	if (sCurrentNodeID != sNodePhaseID)
      {
      	pPPT->SupprimerFils(pptIt) ;
        pPPT->SupprimerItem(pptIt) ;
      }
      else
      {
      	// on se trouve ici dans la phase à traiter
        // on parcours la phase pour faire les suppressions nécessaires
        int iColPhase = (*pptIt)->getColonne() ;
        pptIt++ ;

        while ((pptIt != pPPT->end()) && ((*pptIt)->getColonne() > iColPhase))
        {
        	sSens = (*pptIt)->getLexiqueSens() ;
          sCurrentNodeID = (*pptIt)->getNodeID() ;

          if ((*pptIt)->getColonne() == iColPhase + 1)
          {
          	// Durée : on la supprime
            if (sSens == "VDURE")
            {
            	pPPT->SupprimerFils(pptIt) ;
              pPPT->SupprimerItem(pptIt) ;
            }
            else if (sSens == "VRENO")
            {
            	pPPT->SupprimerFils(pptIt) ;
              pPPT->SupprimerItem(pptIt) ;
            }
            else if (sSens == "GRENT")
            {
            	if (sCurrentNodeID != sNodeRenewID)
              {
              	pPPT->SupprimerFils(pptIt) ;
                pPPT->SupprimerItem(pptIt) ;
              }
              else
              	pptIt++ ;
            }
            else
            	pptIt++ ;
          }
          else
          	pptIt++ ;
        } // fin du while sur la phase
    	} // fin du if sur le node de la phase
    }
    else
    	pptIt++ ;
  }

	return true ;
}

string
ArrayLdvDrugs::getLastRenewNode(string sNodeMedic)
{
  if ((NSFrameInformation*) NULL == _pFrame)
    return string("") ;

  NSDocumentHisto* pDocum = _pFrame->findNode(sNodeMedic) ;
  if ((NSDocumentHisto*) NULL == pDocum)
    return string("") ;

  NSPatPathoArray* pPptIndex = pDocum->getPatPathoPointer() ;
  if (((NSPatPathoArray*) NULL == pPptIndex) || pPptIndex->empty())
    return string("") ;

	// on se place sur le noeud médicament
	PatPathoIter pptIt = pPptIndex->ChercherNoeud(sNodeMedic) ;

	if (((PatPathoIter) NULL == pptIt) || (pPptIndex->end() == pptIt))
  	return string("") ;   // cas d'erreur : noeud introuvable

  string sLastRenewNode = sNodeMedic ;

	// on note la colonne de base
	int iColBase = (*pptIt)->getColonne() ;
	pptIt++ ;

	while ((pPptIndex->end() != pptIt) && ((*pptIt)->getColonne() > iColBase))
	{
    string sSens = (*pptIt)->getLexiqueSens() ;
    string sCurrentNode = (*pptIt)->getNode() ;

    if (sSens == "KPHAT")
    {
    	// on se trouve ici dans la phase à traiter
      // on parcours la phase noter le noeud renouvelement s'il existe
      int iColPhase = (*pptIt)->getColonne() ;
      pptIt++ ;

      while ((pPptIndex->end() != pptIt) && ((*pptIt)->getColonne() > iColPhase))
      {
      	sSens = (*pptIt)->getLexiqueSens() ;
        sCurrentNode = (*pptIt)->getNode() ;

        if ((*pptIt)->getColonne() == iColPhase + 1)
        {
        	if (sSens == "GRENT")
          {
          	sLastRenewNode = sCurrentNode ;
            pptIt++ ;
          }
          else
          	pptIt++ ;
        }
        else
        	pptIt++ ;
      } // fin du while sur la phase
    }
    else
    	pptIt++ ;
  }

	return sLastRenewNode ;
}

void
ArrayLdvDrugs::loadDrugs(NSPatPathoArray* pPtIndex, PatPathoIter iter, int iColBase, bool bJustOne)
{
try
{
  if (((NSPatPathoArray*) NULL == pPtIndex) || (pPtIndex->empty()))
  	return ;

	// NSPatPathoArray* pPtIndex = pDoc->pPathoPOMRIndex ;
	NSLdvDrug* pDrug = (NSLdvDrug*) 0 ;

	NSLinkManager* pGraphe = _pFrame->pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
	bool bTourner = true ;

	while ((bTourner) && (pPtIndex->end() != iter) &&
					((*iter)->getColonne() > iColBase))
	{
		// Problème
		if ((*iter)->getColonne() == iColBase+1)
		{
    	if (bJustOne)
      	bTourner = false ;

			if (pDrug)
			{
        pDrug->initBamDrug() ;
				push_back(new NSLdvDrug(*pDrug)) ;
				delete pDrug ;
				pDrug = (NSLdvDrug*) 0 ;
			}

      string sCodeLex      = (*iter)->getLexique() ;
      string sCodeSensDrug = (*iter)->getLexiqueSens() ;

      //
      // Médicament caché -- Hidden drug
      //
      if (string("90000") == sCodeSensDrug)
      {
      	iter++ ;
      	while ( (iter != pPtIndex->end()) &&
                ((*iter)->getColonne() > iColBase + 1))
        	iter++ ;
      }
      //
      // Médicament visible -- Visible drug
      //
      else
      {
        pDrug = new NSLdvDrug(_pFrame->pContexte, _pFrame->getDoc(), _pFrame->getFrame()) ;

        // Libellé
        pDrug->setLexique(sCodeLex) ;

        if (string("£?????") != sCodeLex)
        	pDrug->setTitlesFromLexique() ;
          // pDoc->pContexte->getDico()->donneLibelle(pDoc->sLangue, &sCodeLex, &(pDrug->sTitre));

        // Texte libre - Free text
        else
        	pDrug->_sTitre = (*iter)->getTexteLibre() ;

        // Noeud
        string sNoeud = (*iter)->getNode() ;
        pDrug->setNoeud(sNoeud) ;

        iter++ ;

        // Paramètres du médicament
        while ((pPtIndex->end() != iter) &&
                ((*iter)->getColonne() > iColBase + 1))
        {
          string sSens = (*iter)->getLexiqueSens() ;

          if ((*iter)->getColonne() == iColBase+2)
          {
            // Dates
            if ((string("KOUVR") == sSens) || (string("KFERM") == sSens))
            {
              iter++ ;
              int iLigneBase = (*iter)->getLigne() ;
              // gereDate* pDate = new gereDate(pContexte);
              string sUnite  = string("") ;
              string sFormat = string("") ;
              string sValeur = string("") ;
              while (	(iter != pPtIndex->end()) &&
                      ((*iter)->getLigne() == iLigneBase))
              {
                if (_pFrame->pContexte->getDico()->CodeCategorie((*iter)->getLexique()) == string(1, '£'))
                {
                  sFormat = (*iter)->getLexiqueSens() ;
                  sValeur = (*iter)->getComplement() ;
                  sUnite  = (*iter)->getUnitSens() ;
                  break ;
                }
                iter++ ;
              }

              // sFormat est du type £D0;03
              if ((string("") != sFormat) && (('D' == sFormat[1]) || ('T' == sFormat[1])) &&
                          (string("") != sValeur))
              {
                if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
                {
                  if (string("KOUVR") == sSens)
                    pDrug->_tDateOuverture.initFromDate(sValeur);
                  else
                    if (string("KFERM") == sSens)
                      pDrug->_tDateFermeture.initFromDate(sValeur);
                }
              }
            }
            // Unité de prise
            //
            else if (string("0MEDF") == sSens)
            {
            	iter++ ;
              if ((pPtIndex->end() != iter) && ((*iter)->getColonne() > iColBase + 2))
              	pDrug->_sIntakeUnit = (*iter)->getLexique() ;
            }
            // Phases
            //
            else if (string("KPHAT") == sSens)
            {
            	pDrug->initPhases(pPtIndex, iter) ;
              iter++ ;
              while ((pPtIndex->end() != iter) &&
                      ((*iter)->getColonne() > iColBase + 2))
               	iter++ ;
            }
            else if (string("£C;") == sSens)
            {
              pDrug->_sFreeText = (*iter)->getTexteLibre() ;
              iter++ ;
            }
            else if (string("LADMI") == sSens)
            {
              int iColBaseAdmin = (*iter)->getColonne() ;
              iter++ ;
              while ((pPtIndex->end() != iter) && ((*iter)->getColonne() > iColBaseAdmin))
              {
                std::string tempAdm = (*iter)->getLexiqueSens() ;
                if (string("LBARZ") == tempAdm)
                  pDrug->_bALD = true ;
                iter++ ;
              }
            }
            else if (string("6ATC0") == sSens)
            {
              pDrug->setAtcCode((*iter)->getComplement()) ;
              iter++ ;
            }
            else if ((string("6CIS0") == sSens) ||
                     (string("6CIP0") == sSens) ||
                     (string("6CIP7") == sSens) ||
                     (string("6CIPT") == sSens))
            {
              pDrug->setCiCode((*iter)->getComplement()) ;
              iter++ ;
            }
            else
              iter++ ;
          }
          else
            iter++ ;
        }
        if (pDrug->_tDateFermeture.estVide())
        	pDrug->_tDateFermeture.setNoLimit() ;

        // Ce médicament est-il lié à des problèmes de santé ?
        // Is this drug linked to a health problem ?
        pGraphe->TousLesVrais(pDrug->getNoeud(), NSRootLink::drugOf,
                                                   &(pDrug->_aConcerns)) ;
			}
		}
		else
			iter++ ;
	}

	if (pDrug)
  {
    pDrug->initBamDrug() ;
		push_back(pDrug) ;
  }
}
catch (...)
{
	erreur("Exception ArrayLdvDrugs::loadDrugs.", standardError, 0) ;
	return;
}
}

NSLdvDrug*
ArrayLdvDrugs::getDrug(string sRef)
{
	if ((string("") == sRef) || (empty()))
		return (NSLdvDrug*) 0 ;

	for (drugsIter i = begin() ; end() != i ; i++)
  	if ((*i)->getNoeud() == sRef)
    	return *i ;

	return (NSLdvDrug*) 0 ;
}

ArrayLdvDrugs&
ArrayLdvDrugs::operator=(const ArrayLdvDrugs& src)
{
try
{
	if (this == &src)
		return *this ;

	_pFrame = src._pFrame ;

	vider() ;

	if (false == src.empty())
		for (drugsConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSLdvDrug(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayLdvDrugs (=).", standardError, 0) ;
	return *this;
}
}

//-----------------------------------------------------------------------//
//				   		 Classe ArrayCopyDrugs                           //
//		   Array de copies de pointeurs : ne jamais faire de delete      //
//-----------------------------------------------------------------------//

ArrayCopyDrugs::ArrayCopyDrugs(const ArrayCopyDrugs& rv)
	             :ArrayDrugs()
{
try
{
	if (false == rv.empty())
		for (drugsConstIter i = rv.begin(); rv.end() != i ; i++)
			push_back(*i) ;
}
catch (...)
{
	erreur("Exception ArrayCopyDrugs ctor.", standardError, 0) ;
}
}

void
ArrayCopyDrugs::vider()
{
	if (empty())
		return ;

	for (drugsIter i = begin(); end() != i ; )
		erase(i) ;
}

ArrayCopyDrugs::~ArrayCopyDrugs()
{
	vider() ;
}

ArrayCopyDrugs&
ArrayCopyDrugs::operator=(const ArrayCopyDrugs& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (drugsConstIter i = src.begin(); src.end() != i ; i++)
			push_back(*i) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayCopyDrugs (=).", standardError, 0) ;
	return *this ;
}
}

