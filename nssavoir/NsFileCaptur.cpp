// -----------------------------------------------------------------------------// NsCaptur.cpp// -----------------------------------------------------------------------------
// Implémentation des méthodes de capture
// 1ère version : PA Septembre 2001    Dernière modif : Septembre 2001
// -----------------------------------------------------------------------------
#include "nautilus\nssuper.h"
#include "nautilus\nsepicap.h"
#include "partage\nsdivfct.h"
#include "nssavoir\nsfilecaptur.h"
#include "nssavoir\nssavoir.h"
#include "nsepisod\nsclasser.h"
#include "nsbb\nsbbtran.h"

NSCaptureFromHPRIM::NSCaptureFromHPRIM(NSContexte* pCtx)
                   :NSRoot(pCtx)
{
	sFileName    = string("") ;
	sFileContent = string("") ;
}

NSCaptureFromHPRIM::~NSCaptureFromHPRIM()
{
}

/*
bool
NSCaptureFromHPRIM::importHPRIM2(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT)
{
	if (sFileName == string(""))
		return false ;

	ifstream inFile ;
	string sLine ;
	string sText = "" ;

	inFile.open(sFileName.c_str());	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return false ;
	}

  parseHPRIM1header(&inFile, pCaptureArray, pPPT) ;

  NSEpiFlechiesDB flechiesDB(pContexte) ;

  bool bFinFound = false ;
  bool bFinFichierFound = false ;

  bool bValueWelcomed = false ;

  bool bLabTagFound = false ;

  // On saute tout jusqu'à la balise LAB
  //
  while (!inFile.eof() && !bLabTagFound)
	{
  	getline(inFile, sLine) ;

    string sStripLine = sLine ;
    strip(sStripLine, stripBoth) ;

    if (sStripLine == string("****LAB****"))
    	bLabTagFound = true ;
	}

  // no LAB tag : not HPRIM2
  //
  if (!bLabTagFound)
  {
  	inFile.close() ;
    return false ;
  }

  string sConcept   = string("") ;
  string sResType   = string("") ;
	string sNormality = string("") ;
  string sResStatus = string("") ;
  string sValue     = string("") ;
  string sUnit      = string("") ;
  string sNormInf   = string("") ;
  string sNormSup   = string("") ;
  string sValue2    = string("") ;
  string sUnit2     = string("") ;
  string sNormInf2  = string("") ;
  string sNormSup2  = string("") ;

	while (!inFile.eof())
	{
  	getline(inFile, sLine) ;

    string sStripLine = sLine ;
    strip(sStripLine, stripBoth) ;

    if (sStripLine == string("****FIN****"))
    	bFinFound = true ;
    else if (sStripLine == string("****FINFICHIER****"))
    	bFinFichierFound = true ;
    else if ((sStripLine != string("")) && (strlen(sStripLine.c_str()) > 4))
    {
      NSPatPathoArray PPTnum(pContexte) ;

    	// text: we try to analyze it
      //
      if (string(sStripLine, 0, 4) == string("TEX|"))
      {
      	sStripLine = string(sStripLine, 4, strlen(sStripLine.c_str()) - 4) ;

    		// What is it ?
      	string sPattern = string("") ;

      	// Title ?
      	string sResult = flechiesDB.getCodeLexiq(sStripLine, 'T') ;
  			if (sResult == string(""))
      		sResult = flechiesDB.getCodeLexiq(sStripLine, 'G') ;
      	if (sResult == string(""))
      		sResult = flechiesDB.getCodeLexiq(sStripLine, 'Z') ;

      	// value
      	if (sResult == string(""))
      	{
      		analysedCapture aCapt(pContexte, sStripLine, 0) ;
      		ParseElemArray aRawParseResult ;
      		ParseElemArray aSemanticParseResult ;
      		aCapt.getSemanticParseElemArray(&aRawParseResult, &aSemanticParseResult) ;
        	//
        	// We only use the information if it has been fully recognized
        	// elsewhere, it can lead to much stupid behaviour
        	//
        	bool bFullSuccess ;
      		sPattern = aCapt.getNumPattern(&aRawParseResult, &aSemanticParseResult, &bFullSuccess) ;
        	if (sPattern != string(""))
          {
          	// We try to get the concept here, because sometimes, the RES
            // information only contains sender specific codes
            //
          	string sResult = aCapt.getElementByTagInNumPattern(sPattern, 'V') ;
            if (sResult != string(""))
            	sConcept = sResult ;

        		if (!bFullSuccess)
          		bValueWelcomed = false ;
          	else
          	{
        			if (sConcept != "")
          			bValueWelcomed = true ;
          	}
        	}
        }

      	int iCol = 0 ;
      	if (sResult != string(""))
      		iCol = 0 ;
      	else
      	{
      		if (!PPTnum.empty())
        	{
        		size_t posValue = sPattern.find("[V") ;
          	if (posValue != NPOS)
          		iCol = 0 ;
          	else
          		iCol = 1 ;
        	}
      	}

    		// Pushing the text
      	//
    		Message theMessage("", "", "", "A", "", "", "") ;
      	theMessage.SetTexteLibre(sStripLine) ;
      	pPPT->ajoutePatho("£?????", &theMessage, iCol) ;
      }
      //
      // RES: structured part
      //
      else if (string(sStripLine, 0, 4) == string("RES|"))
      {
      	// RES-01 Type de segment RES
				// RES-02 Libellé de l'analyse         En texte clair
				// RES-03 Code de l'analyse Défini par le médecin ou selon codification retenue par la commission technique HPRIM
				// RES-04 Type de résultat             A pour numérique, N pour numérique, C pour code
				// RES-05 Résultat 1                   Les résultats numériques doivent être transmis sans espace ni virgule.
        //                                     Le séparateur décimal est le point.
        //                                     Un résultat numérique peut être précédé de son signe + ou -
				// RES-06 Unité 1                      Les unités sont exprimées en système MKSA
				// RES-07 Valeur normale inférieure 1
				// RES-08 Valeur normale supérieure 1
				// RES-09 Indicateur d'anormalité      L  inférieur à la normale
				//                                     H  supérieur à la normale
				//                                     LL inférieur à la valeur panique basse
				//                                     HH supérieur à la valeur panique haute
				//                                     N  normal
				// RES-10 Statut du résultat           F  validé
        //                                     R  non validé
				//                                     C  modifié, corrigé
				// RES-11 Résultat 2                   Ce  champ et les 3 suivants permettent de communiquer le résultat dans une deuxième unité
				// RES-12 Unité 2
				// RES-13 Valeur normale inférieure 2
				// RES-14 Valeur normale supérieure 2

      	sStripLine = string(sStripLine, 4, strlen(sStripLine.c_str()) - 4) ;

        int    iNumInfo = 0 ;
        int    iNumSerie = 0 ;

        while (sStripLine != string(""))
        {
        	size_t posPipe = sStripLine.find("|") ;

        	if (posPipe > 0)
          {
          	string sResData ;

          	if (posPipe == NPOS)
            	sResData = sStripLine ;
            else
          		sResData = string(sStripLine, 0, posPipe) ;

            string sResult = string("") ;

            switch (iNumInfo)
            {
              case 0 :
              	sResult = flechiesDB.getCodeLexiq(sResData, 'V') ;
                if (sResult != string(""))
                	sConcept = sResult ;
                break ;
              case 1 :
              	sResult = flechiesDB.getCodeLexiq(sResData, 'V') ;
                if (sResult != string(""))
                	sConcept = sResult ;
                break ;
              case 2 :
              	sResType = sResData ;
                break ;
              case 3 :
              	sValue = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                break ;
              case 4 :
              	sResult = flechiesDB.getCodeLexiq(sResData, '2') ;
                if (sResult != string(""))
                	sUnit = sResult ;
                break ;
              case 5 :
              	sNormInf = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                break ;
              case 6 :
              	sNormSup = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                break ;
              case 7 :
              	sNormality = sResData ;
                break ;
              case 8 :
              	sResStatus = sResData ;
                break ;
              case 9 :
              	sValue2 = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                break ;
              case 10 :
              	sResult = flechiesDB.getCodeLexiq(sResData, '2') ;
                if (sResult != string(""))
                	sUnit2 = sResult ;
                break ;
              case 11 :
              	sNormInf2 = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                break ;
              case 12 :
              	sNormSup2 = getEpisodusNumValueFromRawCapturedNum(sResData) ;
                break ;
          	}

          	if (posPipe < strlen(sStripLine.c_str()) - 1)
          		sStripLine = string(sStripLine, posPipe + 1, strlen(sStripLine.c_str()) - posPipe - 1) ;
            else
            	sStripLine = string("") ;
          }
          //
          // Empty entry
          //
          else
          {
						sStripLine = string(sStripLine, 1, strlen(sStripLine.c_str()) - 1) ;
            if      (iNumInfo == 4)
            	sUnit  = "200001" ;
            else if (iNumInfo == 10)
            	sUnit2 = "200001" ;
          }

          iNumInfo++ ;
        }

        if (sConcept != string(""))
        {
        	pPPT->ajoutePatho(sConcept, 1, 1) ;

          bool bValuesFound = false ;

        	if ((sValue != string("")) && (sUnit != string("")))
          {
          	bValuesFound = true ;

          	Message theMessage("", "", "", "A", "", "", "") ;
      			theMessage.SetComplement(sValue) ;
            theMessage.SetUnit(sUnit) ;
            pPPT->ajoutePatho("£N0;03", &theMessage, 2, 1) ;

          	if (sNormInf != string(""))
          	{
          		pPPT->ajoutePatho("VNOMI1", 2, 1) ;

            	Message theMessage("", "", "", "A", "", "", "") ;
      				theMessage.SetComplement(sNormInf) ;
            	theMessage.SetUnit(sUnit) ;
            	pPPT->ajoutePatho("£N0;03", &theMessage, 3, 1) ;
          	}
          	if (sNormSup != string(""))
          	{
          		pPPT->ajoutePatho("VNOMS1", 2, 1) ;

            	Message theMessage("", "", "", "A", "", "", "") ;
      				theMessage.SetComplement(sNormSup) ;
            	theMessage.SetUnit(sUnit) ;
            	pPPT->ajoutePatho("£N0;03", &theMessage, 3, 1) ;
          	}
          }
          if ((sValue2 != string("")) && (sUnit2 != string("")))
          {
          	bValuesFound = true ;

          	Message theMessage("", "", "", "A", "", "", "") ;
      			theMessage.SetComplement(sValue2) ;
            theMessage.SetUnit(sUnit2) ;
            pPPT->ajoutePatho("£N0;03", &theMessage, 2, 1) ;

          	if (sNormInf2 != string(""))
          	{
          		pPPT->ajoutePatho("VNOMI1", 2, 1) ;

            	Message theMessage("", "", "", "A", "", "", "") ;
      				theMessage.SetComplement(sNormInf2) ;
            	theMessage.SetUnit(sUnit2) ;
            	pPPT->ajoutePatho("£N0;03", &theMessage, 3, 1) ;
          	}
          	if (sNormSup2 != string(""))
          	{
          		pPPT->ajoutePatho("VNOMS1", 2, 1) ;

            	Message theMessage("", "", "", "A", "", "", "") ;
      				theMessage.SetComplement(sNormSup2) ;
            	theMessage.SetUnit(sUnit2) ;
            	pPPT->ajoutePatho("£N0;03", &theMessage, 3, 1) ;
          	}
          }
          if (bValuesFound)
          {
          	PatPathoIter iterPere = pPPT->end() ;
          	iterPere-- ;
          	pPPT->InserePatPathoFille(iterPere, &PPTnum) ;
          }
        }

      	sConcept   = string("") ;
        sResType   = string("") ;
				sNormality = string("") ;
				sResStatus = string("") ;
				sValue     = string("") ;
				sUnit      = string("") ;
				sNormInf   = string("") ;
				sNormSup   = string("") ;
				sValue2    = string("") ;
				sUnit2     = string("") ;
				sNormInf2  = string("") ;
				sNormSup2  = string("") ;
      }
    }
  }

	inFile.close() ;

  if (!bFinFound || !bFinFichierFound)
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("hprimManagement", "endOfFileMarkupsNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
  }

  return true ;
}
*/

bool
NSCaptureFromHPRIM::importHPRIM2(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT, int* iAlertLevel, string* pLogMessage, bool *pbValidHeader, string &sRootExam)
{
	if (string("") == sFileName)
		return false ;

	ifstream inFile ;
	string sLine ;
	string sText = "" ;

	inFile.open(sFileName.c_str());	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return false ;
	}

  // Parse header
  //
  string sLabName = string("") ;
  string sDocName = string("") ;
  bool bHeaderValidity = parseHPRIM1header(&inFile, pCaptureArray, pPPT, pLogMessage, &sLabName, &sDocName) ;
  if ((bool*) NULL != pbValidHeader)
    *pbValidHeader = bHeaderValidity ;

  // Parse text
  //
  bool bLabTagFound = false ;
  /* bool bGoodParsing = */ parseHPRIM1text(&inFile, pPPT, &bLabTagFound, sRootExam) ;

  // no LAB tag : not HPRIM2
  //
  if (false == bLabTagFound)
  {
  	inFile.close() ;
    return false ;
  }

  NSEpiFlechiesDB flechiesDB(pContexte) ;

  bool bFinFound = false ;
  bool bFinFichierFound = false ;

  string sConcept   = string("") ;
  string sResType   = string("") ;
	string sNormality = string("") ;
  string sResStatus = string("") ;
  string sValue     = string("") ;
  string sUnit      = string("") ;
  string sNormInf   = string("") ;
  string sNormSup   = string("") ;
  string sValue2    = string("") ;
  string sUnit2     = string("") ;
  string sNormInf2  = string("") ;
  string sNormSup2  = string("") ;

  string sLogText   = string("") ;

  PatPathoIter iterReference = NULL ;

  bool bTextTagExist = false ;
  bool bTextFound    = false ;

	while (!inFile.eof())
	{
  	getline(inFile, sLine) ;

    string sStripLine = sLine ;
    strip(sStripLine, stripBoth) ;

    if      (string("****FIN****") == sStripLine)
    	bFinFound = true ;
    else if (string("****FINFICHIER****") == sStripLine)
    	bFinFichierFound = true ;
    else if ((string("") != sStripLine) && (strlen(sStripLine.c_str()) > 4))
    {
      NSPatPathoArray PPTnum(pContexte->getSuperviseur()) ;

    	// text: we try to analyze it
      //
      if (string(sStripLine, 0, 4) == string("TEX|"))
      {
      	bTextTagExist = true ;

      	sStripLine = string(sStripLine, 4, strlen(sStripLine.c_str()) - 4) ;
        strip(sStripLine, stripBoth) ;

        iterReference = findStringInPatPatho(sStripLine, pPPT) ;
        if ((NULL != iterReference) && (pPPT->end() != iterReference))
        	bTextFound = true ;
      }
      //
      // RES: structured part
      //
      else if (string(sStripLine, 0, 4) == string("RES|"))
      {
        HPRIM2ResEntry hprim2entry(string("")) ;
        if (hprim2entry.initFromResLine(sStripLine))
        {
          // Processing label
          //
          string sResult = flechiesDB.getCodeLexiq(hprim2entry._sLabel, 'V') ;
          if (string("") != sResult)
            sConcept = sResult ;
          else
          {
            sLogText = pContexte->getSuperviseur()->getText("fileImport", "warningUnknownConcept") ;
            *pLogMessage += sLogText + string(": ") + hprim2entry._sLabel + string("\r\n") ;
          }

          // Processing code
          //
          sResult = string("") ;
          // Try to find a lab specific code
          if (string("") != sLabName)
            sResult = flechiesDB.getCodeLexiq(sLabName + string(".") + hprim2entry._sCode, 'V') ;
          if (string("") == sResult)
            sResult = flechiesDB.getCodeLexiq(hprim2entry._sCode, 'V') ;
          if (string("") != sResult)
            sConcept = sResult ;
          else
          {
            sLogText = pContexte->getSuperviseur()->getText("fileImport", "warningUnknownConcept") ;
            *pLogMessage += sLogText + string(": ") + hprim2entry._sCode + string("\r\n") ;
          }

          sResType = hprim2entry._sResultType ;
          sValue   = getEpisodusNumValueFromRawCapturedNum(hprim2entry._sResult1) ;

          // Processing unit
          //
          if (string("") == hprim2entry._sUnit1)
            sUnit = string("200001") ;
          else
          {
            sResult = flechiesDB.getCodeLexiq(hprim2entry._sUnit1, '2') ;
            if (string("") != sResult)
              sUnit = sResult ;
            else
            {
              sLogText = pContexte->getSuperviseur()->getText("fileImport", "warningUnknownUnit") ;
              *pLogMessage += sLogText + string(": ") + hprim2entry._sUnit1 + string("\r\n") ;
            }
          }

          sNormInf = getEpisodusNumValueFromRawCapturedNum(hprim2entry._sLowerN1) ;
          sNormSup = getEpisodusNumValueFromRawCapturedNum(hprim2entry._sUpperN1) ;

          sNormality = hprim2entry._sAbnormal ;
          if (((string("LL") == sNormality) || (string("HH") == sNormality)) && (*iAlertLevel < 2))
            *iAlertLevel = 2 ;
          else if (((string("L") == sNormality) || (string("H") == sNormality)) && (*iAlertLevel < 1))
            *iAlertLevel = 1 ;

          sResStatus = hprim2entry._sStatus ;

          // Second information set
          //
          sValue2 = getEpisodusNumValueFromRawCapturedNum(hprim2entry._sResult2) ;

          if (string("") == hprim2entry._sUnit2)
            sUnit2 = string("200001") ;
          else
          {
            sResult = flechiesDB.getCodeLexiq(hprim2entry._sUnit2, '2') ;
            if (string("") != sResult)
              sUnit2 = sResult ;
            else
            {
              sLogText = pContexte->getSuperviseur()->getText("fileImport", "warningUnknownSecondUnit") ;
              *pLogMessage += sLogText + string(": ") + hprim2entry._sUnit2 + string("\r\n") ;
            }
          }

          sNormInf2 = getEpisodusNumValueFromRawCapturedNum(hprim2entry._sLowerN2) ;
          sNormSup2 = getEpisodusNumValueFromRawCapturedNum(hprim2entry._sUpperN2) ;
        }

        if (string("") != sConcept)
        {
        	PPTnum.ajoutePatho(sConcept, 0) ;

          bool bValuesFound = false ;

        	if ((string("") != sValue) && (string("") != sUnit))
          {
          	bValuesFound = true ;

          	Message theMessage ;
      			theMessage.SetComplement(sValue) ;
            theMessage.SetUnit(sUnit) ;
            PPTnum.ajoutePatho("£N0;03", &theMessage, 1) ;

            if ((string("0") == sNormInf) && (string("0") == sNormSup))
            {
            	sNormInf = string("") ;
              sNormSup = string("") ;
            }

          	if (string("") != sNormInf)
          	{
          		PPTnum.ajoutePatho("VNOMI1", 1) ;

            	Message theMessage ;
      				theMessage.SetComplement(sNormInf) ;
            	theMessage.SetUnit(sUnit) ;
            	PPTnum.ajoutePatho("£N0;03", &theMessage, 2) ;
          	}
          	if (string("") != sNormSup)
          	{
          		PPTnum.ajoutePatho("VNOMS1", 1) ;

            	Message theMessage ;
      				theMessage.SetComplement(sNormSup) ;
            	theMessage.SetUnit(sUnit) ;
            	PPTnum.ajoutePatho("£N0;03", &theMessage, 2) ;
          	}
          }
          else
        	{
        		sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorMissingValueOrUnit") ;
          	*pLogMessage += sLogText + string("\r\n") ;
        	}

          if ((string("") != sValue2) && (string("") != sUnit2))
          {
          	bValuesFound = true ;

          	Message theMessage ;
      			theMessage.SetComplement(sValue2) ;
            theMessage.SetUnit(sUnit2) ;
            PPTnum.ajoutePatho("£N0;03", &theMessage, 1) ;

            if ((string("0") == sNormInf2) && (string("0") == sNormSup2))
            {
            	sNormInf2 = string("") ;
              sNormSup2 = string("") ;
            }

          	if (string("") != sNormInf2)
          	{
          		PPTnum.ajoutePatho("VNOMI1", 1) ;

            	Message theMessage ;
      				theMessage.SetComplement(sNormInf2) ;
            	theMessage.SetUnit(sUnit2) ;
            	PPTnum.ajoutePatho("£N0;03", &theMessage, 2) ;
          	}
          	if (string("") != sNormSup2)
          	{
          		PPTnum.ajoutePatho("VNOMS1", 1) ;

            	Message theMessage ;
      				theMessage.SetComplement(sNormSup2) ;
            	theMessage.SetUnit(sUnit2) ;
            	PPTnum.ajoutePatho("£N0;03", &theMessage, 2) ;
          	}
          }
          else
        	{
        		sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorMissingSecondValueOrUnit") ;
          	*pLogMessage += sLogText + string("\r\n") ;
        	}

          if (bValuesFound)
          {
          	if (NULL == iterReference)
            	iterReference = pPPT->end() ;

            // If the same concept is already there, we kill it
            //
            string sConceptSens = pContexte->getDico()->donneCodeSens(&sConcept) ;
            PatPathoIter iterConcept = pPPT->begin() ;
            for ( ; pPPT->end() != iterConcept ; iterConcept++)
            	if ((*iterConcept)->getLexiqueSens() == sConceptSens)
              	break ;

            // Same concept found: merging
            //
						if (pPPT->end() != iterConcept)
            {
            	NSPatPathoArray pptValues(pContexte->getSuperviseur()) ;
            	PPTnum.ExtrairePatPatho(PPTnum.begin(), &pptValues) ;

              pPPT->mergePatpatho(&pptValues, &iterConcept) ;
            }
            // Same concept not found: adding
            //
            else
            {
            	// If reference is at end, we take the last father as reference
            	//
          		if (pPPT->end() == iterReference)
            	{
          			iterReference-- ;
              	// int iRefColEnd = (*iterReference)->getColonne() ;
              	while ((iterReference != pPPT->begin()) &&
                     ((*iterReference)->getColonne() != 0))
              		iterReference-- ;
            	}
              // Inserting the structured values
            	//
          		pPPT->InserePatPathoFille(iterReference, &PPTnum) ;
            }

            if (false == bTextTagExist)
            	iterReference = NULL ;
          }
        }
        else
        {
        	sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorUnknownConcept") ;
          *pLogMessage += sLogText + string("\r\n") ;
        }

      	sConcept   = string("") ;
        sResType   = string("") ;
				sNormality = string("") ;
				sResStatus = string("") ;
				sValue     = string("") ;
				sUnit      = string("") ;
				sNormInf   = string("") ;
				sNormSup   = string("") ;
				sValue2    = string("") ;
				sUnit2     = string("") ;
				sNormInf2  = string("") ;
				sNormSup2  = string("") ;
      }
    }
  }

	inFile.close() ;

  if ((false == bFinFound) || (false == bFinFichierFound))
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("hprimManagement", "endOfFileMarkupsNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
  }

  return true ;
}

bool
NSCaptureFromHPRIM::importHPRIM1(NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT, string* pLogMessage, bool *pbValidHeader, string &sRootExam)
{
	if (string("") == sFileName)
		return false ;

	ifstream inFile ;
	string sLine ;
	string sText = "" ;

	inFile.open(sFileName.c_str());	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return false ;
	}

  bool bHeaderValidity = parseHPRIM1header(&inFile, pCaptureArray, pPPT, pLogMessage) ;
  if (NULL != pbValidHeader)
    *pbValidHeader = bHeaderValidity ;

  bool bLabReached ;
  bool bGoodParsing = parseHPRIM1text(&inFile, pPPT, &bLabReached, sRootExam) ;

	inFile.close() ;

  if (!bGoodParsing)
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("hprimManagement", "endOfFileMarkupsNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
  }

  return true ;
}

bool
NSCaptureFromHPRIM::importTextContent(NSCaptureArray* pCaptureArray)
{
  if (NULL == pCaptureArray)
    return false ;

  ifstream inFile ;
	inFile.open(sFileName.c_str());
	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return false ;
	}

  string sLine ;
	string sText = "" ;
  while (false == inFile.eof())
	{
    getline(inFile, sLine) ;
    if (string("") != sText)
      sText += string(" ") ;
    sText += sLine ;
  }

  return true ;
}

ImportedFileType
NSCaptureFromHPRIM::importHPRIMheader(NSCaptureArray* pCaptureArray, string* pLogMessage)
{
	if (sFileName == string(""))
		return FILETYPE_UNKNOWN ;

	ifstream inFile ;
	string sLine ;
	string sText = "" ;

	inFile.open(sFileName.c_str());	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return FILETYPE_UNKNOWN ;
	}

  NSPatPathoArray* pPPT = NULL ;
	bool bGoodHeader = parseHPRIM1header(&inFile, pCaptureArray, pPPT, pLogMessage) ;

  if (!bGoodHeader)
  {
  	inFile.close() ;
    return FILETYPE_UNKNOWN ;
	}

	bool bFinFound        = false ;
  bool bFinFichierFound = false ;
  bool bLabTagFound     = false ;
  bool bMultiFile       = false ;

	while (!inFile.eof())
	{
  	getline(inFile, sLine) ;

    string sStripLine = sLine ;
    strip(sStripLine, stripBoth) ;

    if (sStripLine == string("****FIN****"))
    	bFinFound = true ;
    else if (sStripLine == string("****FINFICHIER****"))
    	bFinFichierFound = true ;
    else if (sStripLine == string("****LAB****"))
    	bLabTagFound = true ;
    else
    {
    	if ((sStripLine != string("")) && bFinFound)
      	bMultiFile = true ;
    }
	}
  inFile.close() ;

  if (bFinFound && bFinFichierFound)
  {
  	if (bLabTagFound)
    {
    	if (bMultiFile)
      	return FILETYPE_HPRIM2_MULTI ;
      else
    		return FILETYPE_HPRIM2 ;
    }
    else
    {
    	if (bMultiFile)
      	return FILETYPE_HPRIM1_MULTI ;
      else
      	return FILETYPE_HPRIM1 ;
    }
  }
  // Since header is good, we cannot return FILETYPE_UNKNOWN 
	return FILETYPE_HPRIM1 ;
}

string
NSCaptureFromHPRIM::getHPRIMBody()
{
	if (sFileName == string(""))
		return string("") ;

	ifstream inFile ;
	string sLine ;
	string sText = "" ;

	inFile.open(sFileName.c_str());	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return string("") ;
	}

  // Skip header
  //
  size_t iNumLine = 0 ;
  while (!inFile.eof() && (iNumLine < HPRIMHEADER_LINESNB))
  {
  	getline(inFile, sLine) ;
    iNumLine++ ;
  }

	// Get body
  //
  string sResult = string("") ;

  bool bContinue = true ;
  while (!inFile.eof() && bContinue)
	{
  	getline(inFile, sLine) ;

    string sStripLine = sLine ;
    strip(sStripLine, stripBoth) ;

    if ((sStripLine == string("****FIN****")) ||
        (sStripLine == string("****FINFICHIER****")) ||
        (sStripLine == string("****LAB****")))
    	bContinue = false ;
    else
    	sResult += sLine + string("\n") ;
	}

  inFile.close() ;

  return sResult ;
}

bool
NSCaptureFromHPRIM::parseHPRIM1header(ifstream* pInFile, NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT, string* pLogMessage, string* pLabName, string* pDocName)
{
	if (!pInFile || !pCaptureArray)
		return false ;

	string sLang = "" ;
	if (pContexte->getUtilisateur())
		sLang = pContexte->getUtilisateur()->donneLang() ;

	// HPRIM Format
	// line 1 Patient ID (num or name + surname)
	//      2 Patient Familly name
	//      3 Patient name
	//      4 Adresse 1
	//      5 Adresse 2
	//      6 Code Postal / Ville
	//      7 Date Naissance
	//      8 No SS
	//      9 Code ?
	//     10 Date Examen
	//     11 Identifiant Labo
	//     12 Identifiant Médecin

  string asHeaderTable[HPRIMHEADER_LINESNB] ;
  size_t iCurrentSlotToFill = 0 ;

  for (int i = 0 ; i < HPRIMHEADER_LINESNB; i++)
  	asHeaderTable[i] = string("") ;

	// First step: loading header table 12 lines
  //
  // Notice: if we encouter a ****FIN**** we empty the header table and start
  //         filling it from line 0
  //
  string sLine ;

	string sLogText = pContexte->getSuperviseur()->getText("fileImport", "analysingHprimHeader") ;
  *pLogMessage += sLogText + string("\r\n") ;

  bool bSatisfied = false ;

  while (!pInFile->eof() && (!bSatisfied))
	{
    // Process a file line
    //
  	getline(*pInFile, sLine) ;

    strip(sLine, stripBoth) ;

    // Finding a FIN -> reseting headers lines
    //
    if (sLine == string("****FIN****"))
    {
			for (int i = 0 ; i < HPRIMHEADER_LINESNB; i++)
				asHeaderTable[i] = string("") ;
    	iCurrentSlotToFill = 0 ;
    }
    else if (sLine == string("****FINFICHIER****"))
    {
    	sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorFinFichierTagEncountered") ;
			*pLogMessage += sLogText + string("\r\n") ;
    	return false ;
    }
    else
    {
    	asHeaderTable[iCurrentSlotToFill] = sLine ;
      iCurrentSlotToFill++ ;
    }

    // If header table is full, we check if it sounds good
    //
    if (iCurrentSlotToFill >= HPRIMHEADER_LINESNB)
    {
    	// If first line is empty, then check if
      //
    	if (asHeaderTable[0] == string(""))
      {
      	if ((asHeaderTable[1] == string("")) ||
            ((asHeaderTable[9] != string("")) && (asHeaderTable[9].find("/") == string::npos)))
        {
        	for (int i = 0 ; i < HPRIMHEADER_LINESNB - 1; i++)
						asHeaderTable[i] = asHeaderTable[i+1] ;
          iCurrentSlotToFill = HPRIMHEADER_LINESNB - 1 ;
        }
      	else
        	bSatisfied = true ;
      }
      else
      	bSatisfied = true ;
    }
	}

  // Second step: process the header table
  //
  for (int i = 0; i < HPRIMHEADER_LINESNB; i++)
	{
  	if (asHeaderTable[i] != string(""))
    {
    	string sPathString      = string("") ;
    	string sUnitString      = string("") ;
    	string sClassifString   = string("") ;
    	string sInformationDate = string("") ;

    	switch(i)
      {
      	case 1 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM01", asHeaderTable[i], FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimSurname") ;
          *pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string("\r\n") ;
          break ;
        case 2 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM21", asHeaderTable[i], FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimFirstName") ;
          *pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string("\r\n") ;
          break ;
        case 3 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LCOOR1/LADRE1/LADL11", asHeaderTable[i], FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimAddressFirstLine") ;
          *pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string("\r\n") ;
          break ;
        case 4 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LCOOR1/LADRE1/LADL21", asHeaderTable[i], FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimAddressSecondLine") ;
          *pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string("\r\n") ;
          break ;
        case 5 :
        	{
          	size_t blkPos = asHeaderTable[i].find(" ") ;
            if (blkPos != NPOS)
            {
            	string sZip  = string(asHeaderTable[i], 0, blkPos) ;
              string sCity = string(asHeaderTable[i], blkPos + 1, strlen(asHeaderTable[i].c_str()) - blkPos - 1) ;
              strip(sCity, stripBoth) ;
              pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LCOOR1/LADRE1/LVILL1/LZIP01", sZip, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
              sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimZipCode") ;
							*pLogMessage += sLogText + string(": ") + sZip + string("\r\n") ;
              pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LCOOR1/LADRE1/LVILL1/LCOMU1", sCity, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
              sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimCity") ;
							*pLogMessage += sLogText + string(": ") + sCity + string("\r\n") ;
            }
          }
          break ;
        //
        // Birthdate
        //
        case 6 :
        	{
          	string sDate = string("") ;

            string sBirthDate = asHeaderTable[i] ;

            if (strlen(sBirthDate.c_str()) >= 5)
            {                                      
              string sMonth = string(sBirthDate, 3, 2) ;

              // Warning : when the date comes from the french NIR, the month can indicate
              //           that this date is not known exactly :
              //           Le mois de naissance peut aussi être compris entre 30 et 42,
              //           entre 50 et 99 ou égal à 20 ou 99 ; pour les personnes dont
              //           on ne connaît pas le mois de naissance du fait d’absence
              //           de registre d’état civil dans certains pays étrangers.
              //
              int iMonth = atoi(sMonth.c_str()) ;
              if (iMonth > 12)
              {
            	  if ((iMonth == 20) || (iMonth == 99) || ((iMonth >= 30) && (iMonth <= 42)) || ((iMonth >= 50) && (iMonth <= 99)))
                {
              	  sBirthDate[3] = '0' ;
                  sBirthDate[4] = '1' ;

                  sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimBirthDateAdapted") ;
								  *pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string(" -> ") + sBirthDate + string("\r\n") ;
							  }
              }
            }
          	//
          	// JJ/MM/AA
            //
          	if (strlen(sBirthDate.c_str()) == 8)
            {
          		NVLdVTemps currentDate ;
            	currentDate.takeTime() ;
            	string sCurrentDate = currentDate.donneDate() ;
            	string sTreshold = string(sCurrentDate, 2, 2) ;

            	sDate = getDateFromHPRIM1date(sBirthDate, sTreshold) ;
            }
            //
            // JJ/MM/AAAA
            //
            else if (strlen(sBirthDate.c_str()) == 10)
            	donne_date_inverse(sBirthDate, sDate, sLang) ;

            if (sDate != string(""))
            {
            	pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/KNAIS1", sDate, FromMailBox, sClassifString, "2DA011", sInformationDate)) ;
              sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimBirthDate") ;
							*pLogMessage += sLogText + string(": ") + sDate + string("\r\n") ;
            }
            else
            {
            	sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorInvalidBirthDate") ;
							*pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string("\r\n") ;
            	return false ;
            }
          }
          break ;
        //
        // NSS (get gender from it)
        //
        case 7 :
        {
        	string sNIR    = string("") ;
          string sNIRkey = string("") ;

          size_t blkPos = asHeaderTable[i].find(" ") ;
          if (blkPos != NPOS)
          {
          	sNIR    = string(asHeaderTable[i], 0, blkPos) ;
            sNIRkey = string(asHeaderTable[i], blkPos + 1, strlen(asHeaderTable[i].c_str()) - blkPos - 1) ;
            strip(sNIRkey, stripBoth) ;
          }
          else
          	sNIR = asHeaderTable[i] ;

          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LFRAN1/LFRAB1", sNIR, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimNir") ;
          *pLogMessage += sLogText + string(": ") + sNIR + string("\r\n") ;

          if      (sNIR[0] == '1')
          	pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LSEXE1", "HMASC2", FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
          else if (sNIR[0] == '2')
          	pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LSEXE1", "HFEMI2", FromMailBox, sClassifString, sUnitString, sInformationDate)) ;


          if (sNIRkey != string(""))
          {
          	pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LFRAN1/LFRAV1", sNIRkey, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
            sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimNirKey") ;
          	*pLogMessage += sLogText + string(": ") + sNIR + string("\r\n") ;
          }
        }
        	break ;

        //
        // Document's date (warning, it can be something like "M-5128-23/10/06")
        //
        // No (understandable) document date means bad header
        //
        case 9 :
        	{
          	// First, find the date inside the string
            size_t iPos = asHeaderTable[i].find("/") ;
            if (iPos == string::npos)
            {
            	sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorDocumentDateWithoutSlash") ;
							*pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string("\r\n") ;
            	return false ;
            }

            size_t iPos2 = asHeaderTable[i].find("/", iPos + 1) ;
            if (iPos2 != iPos + 3)
            {
            	sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorDocumentDateIllSeparatedSlash") ;
							*pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string("\r\n") ;
            	return false ;
            }

            size_t iLength = strlen(asHeaderTable[i].c_str()) ;
            if ((iPos + 6 <= iLength))
            {
            	string sRawDate = string(asHeaderTable[i], iPos-2, 8) ;
              //
              // 8 or 10 digits date means that 8 first digit are a valid date
              //
          		string sDate = getDateFromHPRIM1date(sRawDate, "") ;
              if (sDate == string(""))
              {
              	sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorDocumentDateIsInvalid") ;
								*pLogMessage += sLogText + string(": ") + sRawDate +  string("\r\n") ;
              	return false ;
              }

              // Check if it is not a 10 digits date
              if ((iPos + 8 <= iLength) && (isdigit(asHeaderTable[i][iPos + 6])) && (isdigit(asHeaderTable[i][iPos + 7])))
              {
              	sRawDate = string(asHeaderTable[i], iPos-2, 10) ;
            		donne_date_inverse(sRawDate, sDate, sLang) ;
              }

            	if (sDate != string(""))
            	{
              	if (NULL != pPPT)
              	{
            			pPPT->ajoutePatho("KCHIR2", 0) ;
									Message theMessage ;
    							theMessage.SetUnit("2DA011") ;
									theMessage.SetComplement(sDate) ;
									pPPT->ajoutePatho("£D0;10", &theMessage, 1) ;
              	}
              	else
                {
              		pCaptureArray->ajouter(new NSCapture(pContexte, "KCHIR2", sDate, FromMailBox, sClassifString, "2DA011", sInformationDate)) ;
                  sLogText = pContexte->getSuperviseur()->getText("fileImport", "hprimDocumentDate") ;
          				*pLogMessage += sLogText + string(": ") + sDate + string("\r\n") ;
          			}
            	}
              else
              {
              	sLogText = pContexte->getSuperviseur()->getText("fileImport", "errorDocumentDateWithoutSlash") ;
								*pLogMessage += sLogText + string(": ") + asHeaderTable[i] + string("\r\n") ;
              	return false ;
              }
            }
          }
        //
        // Lab's name
        //
        case 10 :
          	if (NULL != pLabName)
            	*pLabName = asHeaderTable[i] ;
          	break ;
        //
        // Doctor's name
        //
        case 12 :
          	if (NULL != pDocName)
            	*pDocName = asHeaderTable[i] ;
          	break ;
      }
    }
	}
  sLogText = pContexte->getSuperviseur()->getText("fileImport", "goodHprimHeader") ;
  *pLogMessage += sLogText + string("\r\n") ;
  return true ;
}

/*
bool
NSCaptureFromHPRIM::parseHPRIM1header(ifstream* pInFile, NSCaptureArray* pCaptureArray, NSPatPathoArray* pPPT)
{
	if (!pInFile || !pCaptureArray)
		return false ;

	string sLang = "" ;
	if (pContexte->getUtilisateur())
		sLang = pContexte->getUtilisateur()->donneLang() ;

	// HPRIM Format
	// line 1 Patient ID (num or name + surname)
	//      2 Patient Familly name
	//      3 Patient name
	//      4 Adresse 1
	//      5 Adresse 2
	//      6 Code Postal / Ville
	//      7 Date Naissance
	//      8 No SS
	//      9 Code ?
	//     10 Date Examen
	//     11 Identifiant Labo
	//     12 Identifiant Médecin

	// Loading header
  //
  size_t iNumLine = 0 ;
  string sLine ;

  while (!pInFile->eof() && (iNumLine < HPRIMHEADER_LINESNB))
	{
  	getline(*pInFile, sLine) ;

    string sPathString      = string("") ;
    string sUnitString      = string("") ;
    string sClassifString   = string("") ;
    string sInformationDate = string("") ;

    strip(sLine, stripBoth) ;

    if (sLine != string(""))
    {
    	switch(iNumLine)
      {
      	case 1 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM01", sLine, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        case 2 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM21", sLine, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        case 3 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LCOOR1/LADRE1/LADL11", sLine, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        case 4 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LCOOR1/LADRE1/LADL21", sLine, sClassifString, sUnitString, sInformationDate)) ;
          break ;
        case 5 :
        	{
          	size_t blkPos = sLine.find(" ") ;
            if (blkPos != NPOS)
            {
            	string sZip  = string(sLine, 0, blkPos) ;
              string sCity = string(sLine, blkPos + 1, strlen(sLine.c_str()) - blkPos - 1) ;
              strip(sCity, stripBoth) ;
              pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LCOOR1/LADRE1/LVILL1/LZIP01", sZip, sClassifString, sUnitString, sInformationDate)) ;
              pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LCOOR1/LADRE1/LVILL1/LCOMU1", sCity, sClassifString, sUnitString, sInformationDate)) ;
            }
          }
          break ;
        //
        // Birthdate
        //
        case 6 :
        	{
          	string sDate = string("") ;
          	//
          	// JJ/MM/AA
            //
          	if (strlen(sLine.c_str()) == 8)
            {
          		NVLdVTemps currentDate ;
            	currentDate.takeTime() ;
            	string sCurrentDate = currentDate.donneDate() ;
            	string sTreshold = string(sCurrentDate, 2, 2) ;

            	sDate = getDateFromHPRIM1date(sLine, sTreshold) ;
            }
            //
            // JJ/MM/AAAA
            //
            else if (strlen(sLine.c_str()) == 10)
            	donne_date_inverse(sLine, sDate, sLang) ;

            if (sDate != string(""))
            	pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/KNAIS1", sDate, sClassifString, "2DA011", sInformationDate)) ;
          }
          break ;
        //
        // NSS (get gender from it)
        //
        case 7 :
          pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LFRAN1/LFRAB1", sLine, sClassifString, sUnitString, sInformationDate)) ;
          if      (sLine[0] == '1')
          	pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LSEXE1", "HMASC2", sClassifString, sUnitString, sInformationDate)) ;
          else if (sLine[0] == '2')
          	pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LSEXE1", "HFEMI2", sClassifString, sUnitString, sInformationDate)) ;
          break ;
        //
        // Document's date
        //
        case 9 :
        	{
          	string sDate = string("") ;

          	// JJ/MM/AA
          	if (strlen(sLine.c_str()) == 8)
            	sDate = getDateFromHPRIM1date(sLine, "") ;
            // JJ/MM/AAAA
            else if (strlen(sLine.c_str()) == 10)
            	donne_date_inverse(sLine, sDate, sLang) ;

            M-5128-23/10/06

            if (sDate != string(""))
            {
              if (NULL != pPPT)
              {
            		pPPT->ajoutePatho("KCHIR2", 0) ;
								Message theMessage("", "", "", "A", "", "", "") ;
    						theMessage.SetUnit("2DA011") ;
								theMessage.SetComplement(sDate) ;
								pPPT->ajoutePatho("£D0;10", &theMessage, 1, 1) ;
              }
              else
              	pCaptureArray->ajouter(new NSCapture(pContexte, "KCHIR2", sDate, sClassifString, "2DA011", sInformationDate)) ;
            }
          }
      }
    }
    iNumLine++ ;
  }
  return true ;
}
*/

bool
NSCaptureFromHPRIM::parseHPRIM1text(ifstream* pInFile, NSPatPathoArray* pPPT, bool* pbStoppedOnLab, string &sRootExam)
{
	if (!pInFile || (NULL == pPPT))
		return false ;

	NSEpiFlechiesDB flechiesDB(pContexte) ;

  bool bLabFound = false ;
  bool bFinFound = false ;
  bool bFinFichierFound = false ;

  bool bValueWelcomed = false ;

  string sLine ;

	while (!(pInFile->eof()) && (false == bLabFound))
	{
  	getline(*pInFile, sLine) ;

    string sStripLine = sLine ;
    strip(sStripLine, stripBoth) ;

    if      (string("****FIN****") == sStripLine)
    	bFinFound = true ;
    else if (string("****FINFICHIER****") == sStripLine)
    	bFinFichierFound = true ;
    else if (string("****LAB****") == sStripLine)
    	bLabFound = true ;
    else if (string("") != sStripLine)
    {
      // We have better remove the '*' char, because it sometimes means "abnormal" value
      //
      size_t iStarCharPos = sStripLine.find("*") ;
      while (iStarCharPos != NPOS)                         
      {
      	sStripLine = string(sStripLine, 0, iStarCharPos) + string(sStripLine, iStarCharPos+1, strlen(sStripLine.c_str()) - iStarCharPos - 1) ;
      	iStarCharPos = sStripLine.find("*") ;
      }
      strip(sStripLine, stripBoth) ;

    	// What is it ?
      string sPattern = string("") ;

      // Title ?
      string sResult = flechiesDB.getCodeLexiq(sStripLine, 'T') ;
  		if (string("") == sResult)
      	sResult = flechiesDB.getCodeLexiq(sStripLine, 'G') ;
      if (string("") == sResult)
      	sResult = flechiesDB.getCodeLexiq(sStripLine, 'Z') ;

      if (string("") == sRootExam)
      {
        if (string("") != sResult)
          sRootExam = sResult ;
        else
          sRootExam = sFindFirstExamInSentence(sStripLine, &flechiesDB) ;
      }

      NSPatPathoArray PPTnum(pContexte->getSuperviseur()) ;
      bValueWelcomed = false ;

      // value
      if (string("") == sResult)
      {
      	analysedCapture aCapt(pContexte, sStripLine, (NSCaptureArray*) 0, FromMailBox) ;
      	ParseElemArray aRawParseResult ;
      	ParseElemArray aSemanticParseResult ;
      	aCapt.getSemanticParseElemArray(&aRawParseResult, &aSemanticParseResult) ;
        //
        // We only use the information if it has been fully recognized
        // otherwise it can lead to much stupid behaviour
        //
        bool bFullSuccess ;
      	sPattern = aCapt.getNumPattern(&aRawParseResult, &aSemanticParseResult, &bFullSuccess) ;
        if (string("") != sPattern)
        {
        	if (false == bFullSuccess)
          	bValueWelcomed = false ;
          else
          {
        		aCapt.numPatternToTree(&sPattern, &PPTnum) ;

            // If there are 2 values with same unit (or second with no unit)
            // then the second one is a "previous value"; we must delete it
            //
            if (false == PPTnum.empty())
            {
            	PatPathoIter iterPPT = PPTnum.begin() ;    // concept
              PatPathoIter iFirstData = PPTnum.ChercherPremierFils(iterPPT) ;
              if ((NULL != iFirstData) && (PPTnum.end() != iFirstData))
              {
              	if ((*iFirstData)->getLexiqueSens() == string("£N0"))
                {
                	string sUnitSens = (*iFirstData)->getUnitSens() ;
                  PatPathoIter iBrothData = PPTnum.ChercherFrereSuivant(iFirstData) ;
                  while ((NULL != iBrothData) && (PPTnum.end() != iBrothData))
                  {
                  	if ((*iBrothData)->getLexiqueSens() == string("£N0"))
                    {
                    	string sBrothUnitSens = (*iBrothData)->getUnitSens() ;
                      if ((sBrothUnitSens == sUnitSens) ||
                          (sBrothUnitSens == string("")) ||
                          (sBrothUnitSens == string("20000")))
                      {
                      	PPTnum.SupprimerItem(iBrothData) ;
                        iBrothData = PPTnum.ChercherFrereSuivant(iFirstData) ;
                      }
                      else
                      	iBrothData = PPTnum.ChercherFrereSuivant(iBrothData) ;
                    }
                    else
                      iBrothData = PPTnum.ChercherFrereSuivant(iBrothData) ;
                  }
                }
              }
            }

            size_t posValue = sPattern.find("[V") ;

            // Warning: if there is a concept alone, don't store structured data
            //
          	if ((posValue != NPOS) && (PPTnum.size() > 1))
          		bValueWelcomed = true ;
          }
        }
      }

      int iCol = 0 ;
      if (string("") != sResult)
      	iCol = 0 ;
      else
      {
      	if (false == PPTnum.empty())
        {
        	size_t posValue = sPattern.find("[V") ;
          if (posValue != NPOS)
          	iCol = 0 ;
          else
          	iCol = 1 ;
        }
      }

    	// Pushing the text
      //
    	Message theMessage ;
      theMessage.SetTexteLibre(sLine) ;
      pPPT->ajoutePatho("£?????", &theMessage, iCol) ;

      if (string("") != sResult)
      	pPPT->ajoutePatho(sResult, iCol+1) ;
      else
      {
      	if (bValueWelcomed && (false == PPTnum.empty()))
        {
        	PatPathoIter iterPere = pPPT->end() ;
          iterPere-- ;
          // int iRefColEnd = (*iterPere)->getColonne() ;
          while ((iterPere != pPPT->begin()) &&
                 ((*iterPere)->getColonne() != 0))
          	iterPere-- ;

          size_t posValue = sPattern.find("[V") ;
          if (NPOS == posValue)
          {
          	while ((iterPere != pPPT->begin()) && ((*iterPere)->getLexique() == string("£?????")))
            	iterPere-- ;
            while ((iterPere != pPPT->begin()) && (((*iterPere)->getLexique())[0] != 'V'))
            	iterPere-- ;
          }

        	pPPT->InserePatPathoFille(iterPere, &PPTnum) ;
        }
      }
    }
  }

  if (pbStoppedOnLab)
  {
  	if (bLabFound)
    	*pbStoppedOnLab = true ;
    else
    	*pbStoppedOnLab = false ;
  }

  if (!bFinFound && bLabFound)
  	return false ;

	return true ;
}

PatPathoIter
NSCaptureFromHPRIM::findStringInPatPatho(string sRef, NSPatPathoArray* pPPT)
{
	if ((string("") == sRef) || (NULL == pPPT) || (pPPT->empty()))
		return NULL ;

	PatPathoIter itPPT = pPPT->begin() ;
  for ( ; (pPPT->end() != itPPT) && ((*itPPT)->getTexteLibre() != sRef) ; itPPT++)
  	;

	return itPPT ;
}

bool
NSCaptureFromHPRIM::explodeMultipleMessagesFile()
{
	if (string("") == sFileName)
		return false ;

	// First step: parse file name

  string sFileDirectory = string("") ;
  string sLocalFileName = string("") ;
  string sFileExtension = string("") ;

  size_t pos1 = sFileName.find_last_of(".") ;
  if (NPOS == pos1)
  	sLocalFileName = sFileName ;
  else
  {
  	sLocalFileName = string(sFileName, 0, pos1) ;
    sFileExtension = string(sFileName, pos1+1, strlen(sFileName.c_str())-1) ;
  }

  pos1 = sLocalFileName.find_last_of("\\") ;
  if (NPOS != pos1)
  {
  	sFileDirectory = string(sLocalFileName, 0, pos1+1) ;
    sLocalFileName = string(sLocalFileName, pos1+1, strlen(sLocalFileName.c_str())-1) ;
  }

  // Open master file for reading

	ifstream inFile ;

  inFile.open(sFileName.c_str());
	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sFileName ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return false ;
	}

	string sLine ;
	string sText = "" ;

  getline(inFile, sLine) ;

  bool bKeepOnRunning = true ;  while (bKeepOnRunning && !inFile.eof())  {    // First, skip empty lines and detect end tag    string sStripLine = sLine ;    strip(sStripLine, stripBoth) ;

/*  Don't do that because empty lines may be usefull at the begining of the message

    while ((sStripLine == string("")) && !inFile.eof())
    {
    	getline(inFile, sLine) ;
    	sStripLine = sLine ;    	strip(sStripLine, stripBoth) ;
    }
*/

    // Here, we are at end of file without reaching the end tag
    //
    if (inFile.eof())
    {
    	inFile.close() ;
    	return false ;
    }

		if (string("****FINFICHIER****") == sStripLine)
    {
    	inFile.close() ;
    	return true ;
    }

  	// Starting the new message; first create a new file    string sNewFileName = sFileDirectory + nomSansDoublons(sFileDirectory, sLocalFileName, sFileExtension) ;    ofstream outFile ;  	outFile.open(sNewFileName.c_str());
  	if (!outFile)
  	{
    	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") ;
      sErrorText += string(" ") + sNewFileName ;
    	erreur(sErrorText.c_str(), standardError, 0, 0) ;
    	inFile.close() ;
    	return false ;
  	}    bool bGotEndOfMessageTag = false ;
  	while (!inFile.eof() && (!bGotEndOfMessageTag))
  	{
      outFile << (sLine + string("\n")) ;

      strip(sLine, stripBoth) ;
    	if (sLine == string("****FIN****"))
      {
      	outFile << (string("****FINFICHIER****") + string("\n")) ;
        bGotEndOfMessageTag = true ;
        getline(inFile, sLine) ;
      }
      else if (sLine == string("****FINFICHIER****"))
      {
      	outFile << (string("****FINFICHIER****") + string("\n")) ;
        bGotEndOfMessageTag = true ;
      }
      else
      	getline(inFile, sLine) ;
		}

    outFile.close() ;
  }

  inFile.close() ;

  return false ;
}

string
NSCaptureFromHPRIM::getDateFromHPRIM1date(string sHPRIMdate, string s2kTreshold)
{
	if (strlen(sHPRIMdate.c_str()) != 8)
		return string("") ;

	if (('/' != sHPRIMdate[2]) || ('/' != sHPRIMdate[5]))
		return string("") ;

  string sYear  = string(sHPRIMdate, 6, 2) ;
  string sMonth = string(sHPRIMdate, 3, 2) ;
  string sDay   = string(sHPRIMdate, 0, 2) ;
  string sCent  = string("20") ;

  // Year ?
  if (string("") == s2kTreshold)
  	sCent = string("20") ;
  else
  {
  	int iTreshold = atoi(s2kTreshold.c_str()) ;
    int iYear     = atoi(sYear.c_str()) ;
    if (iYear > iTreshold)
    	sCent = string("19") ;
    else
    	sCent = string("20") ;
  }

  string sDateFr = sDay + string("/") + sMonth + string("/") + sCent + sYear ;

  string sReturnDate = string("") ;
  donne_date_inverse(sDateFr, sReturnDate, "fr") ;
  return sReturnDate ;
}

string
NSCaptureFromHPRIM::sFindFirstExamInSentence(string sSentence, NSEpiFlechiesDB* pFlechies)
{
  if ((string("") == sSentence) || (NULL == pFlechies))
    return string("") ;

  string sCopy = sSentence ;

  strip(sCopy, stripBoth) ;
  if (string("") == sCopy)
    return string("") ;

  // First step, replace tabs with blanks
  for (size_t i = 0 ; i < strlen(sCopy.c_str()) ; i++)
    if ('\t' == sCopy[i])
      sCopy[i] = ' ' ;

  // First step, remove multiple blanks
  keepOnlyOne(sCopy, ' ') ;

  string sTest = pFlechies->getCodeLexiq(sCopy) ;
  if ((string("") != sTest) &&
      (('G' == sTest[0]) || ('T' == sTest[0]) || ('Z' == sTest[0])))
    return sTest ;

  // Second step, cut in pieces (a blank is a piece)
  vector<string> aPieces ;

  size_t iStart = 0 ;
  size_t iEnd   = 0 ;
  for ( ; iEnd < strlen(sCopy.c_str()) ; iEnd++)
  {
    switch(sCopy[iEnd])
    {
      case ' '  :
        aPieces.push_back(string(sCopy, iStart, iEnd - iStart)) ;
        aPieces.push_back(string(" ")) ;
        iStart = iEnd + 1 ;
        break ;
      case '\'' :
        aPieces.push_back(string(sCopy, iStart, iEnd - iStart + 1)) ;
        iStart = iEnd + 1 ;
        break ;
    }
  }
  aPieces.push_back(string(sCopy, iStart, strlen(sCopy.c_str()) - iStart)) ;

  size_t iPiecesNbToTest = aPieces.size() - 1 ;

  while (iPiecesNbToTest > 0)
  {
    vector<string>::iterator itStart = aPieces.begin() ;

    size_t iRemainingPieces = aPieces.size() - 1 ;
    while ((iRemainingPieces >= iPiecesNbToTest-1) && (aPieces.end() != itStart))
    {
      string sStringToTest = string("") ;

      vector<string>::iterator itNext = itStart ;
      for (size_t i = 0 ; i < iPiecesNbToTest ; i++)
        sStringToTest += *(itNext++) ;

      // No need to test strings that start with a blank or end with a blank
      //
      if ((' ' != sStringToTest[0]) && (' ' != sStringToTest[strlen(sStringToTest.c_str()) - 1]))
      {
        string sResult = pFlechies->getCodeLexiq(sStringToTest) ;
        if ((string("") != sResult) &&
            (('G' == sResult[0]) || ('T' == sResult[0]) || ('Z' == sResult[0])))
        {
          // Don't accept just "report"
          //
          if (string("ZCN001") != sResult)
            return sResult ;
        }
      }

      itStart++ ;
      iRemainingPieces-- ;
    }

    iPiecesNbToTest-- ;
  }

  return string("") ;
}

// -----------------------------------------------------------------------------
//                      HPRIM2ResEntry methods
// -----------------------------------------------------------------------------

HPRIM2ResEntry::HPRIM2ResEntry(string sResLine)
{
  initFromResLine(sResLine) ;
}

HPRIM2ResEntry::HPRIM2ResEntry(const HPRIM2ResEntry& rv)
{
  initFromResEntry(rv) ;
}

HPRIM2ResEntry::~HPRIM2ResEntry()
{
}

void
HPRIM2ResEntry::init()
{
  _sLabel      = string("") ;
  _sCode       = string("") ;
  _sResultType = string("") ;
  _sResult1    = string("") ;
  _sUnit1      = string("") ;
  _sLowerN1    = string("") ;
  _sUpperN1    = string("") ;
  _sAbnormal   = string("") ;
  _sStatus     = string("") ;
  _sResult2    = string("") ;
  _sUnit2      = string("") ;
  _sLowerN2    = string("") ;
  _sUpperN2    = string("") ;
}

// RES-01 Type de segment RES
// RES-02 Libellé de l'analyse         En texte clair
// RES-03 Code de l'analyse Défini par le médecin ou selon codification retenue par la commission technique HPRIM
// RES-04 Type de résultat             A pour numérique, N pour numérique, C pour code
// RES-05 Résultat 1                   Les résultats numériques doivent être transmis sans espace ni virgule.
//                                     Le séparateur décimal est le point.
//                                     Un résultat numérique peut être précédé de son signe + ou -
// RES-06 Unité 1                      Les unités sont exprimées en système MKSA
// RES-07 Valeur normale inférieure 1
// RES-08 Valeur normale supérieure 1
// RES-09 Indicateur d'anormalité      L  inférieur à la normale
//                                     H  supérieur à la normale
//                                     LL inférieur à la valeur panique basse
//                                     HH supérieur à la valeur panique haute
//                                     N  normal
// RES-10 Statut du résultat           F  validé
//                                     R  non validé
//                                     C  modifié, corrigé
// RES-11 Résultat 2                   Ce  champ et les 3 suivants permettent de communiquer le résultat dans une deuxième unité
// RES-12 Unité 2
// RES-13 Valeur normale inférieure 2
// RES-14 Valeur normale supérieure 2

bool
HPRIM2ResEntry::initFromResLine(string sResLine)
{
  init() ;

  if (strlen(sResLine.c_str()) < 5)
    return false ;

  if (string("RES|") != string(sResLine, 0, 4))
    return false ;

  // Removing initial RES|
  string sStripLine = string(sResLine, 4, strlen(sResLine.c_str()) - 4) ;
  strip(sStripLine, stripBoth) ;

  int iNumInfo = 0 ;

  while (string("") != sStripLine)
  {
    size_t posPipe = sStripLine.find("|") ;

    if (posPipe > 0)
    {
      string sResData ;

      if (posPipe == NPOS)
        sResData = sStripLine ;
      else
        sResData = string(sStripLine, 0, posPipe) ;

      strip(sResData, stripBoth) ;

      switch (iNumInfo)
      {
        case 0  : _sLabel      = sResData ; break ;
        case 1  : _sCode       = sResData ; break ;
        case 2  : _sResultType = sResData ; break ;
        case 3  : _sResult1    = sResData ; break ;
        case 4  : _sUnit1      = sResData ; break ;
        case 5  : _sLowerN1    = sResData ; break ;
        case 6  : _sUpperN1    = sResData ; break ;
        case 7  : _sAbnormal   = sResData ; break ;
        case 8  : _sStatus     = sResData ; break ;
        case 9  : _sResult2    = sResData ; break ;
        case 10 : _sUnit2      = sResData ; break ;
        case 11 : _sLowerN2    = sResData ; break ;
        case 12 : _sUpperN2    = sResData ; break ;
      }

      if (posPipe < strlen(sStripLine.c_str()) - 1)
        sStripLine = string(sStripLine, posPipe + 1, strlen(sStripLine.c_str()) - posPipe - 1) ;
      else
        sStripLine = string("") ;
    }
    //
    // Empty entry
    //
    else
      sStripLine = string(sStripLine, 1, strlen(sStripLine.c_str()) - 1) ;

    iNumInfo++ ;
  }

  return true ;
}

void
HPRIM2ResEntry::initFromResEntry(const HPRIM2ResEntry& src)
{
  _sLabel      = src._sLabel ;
  _sCode       = src._sCode ;
  _sResultType = src._sResultType ;
  _sResult1    = src._sResult1 ;
  _sUnit1      = src._sUnit1 ;
  _sLowerN1    = src._sLowerN1 ;
  _sUpperN1    = src._sUpperN1 ;
  _sAbnormal   = src._sAbnormal ;
  _sStatus     = src._sStatus ;
  _sResult2    = src._sResult2 ;
  _sUnit2      = src._sUnit2 ;
  _sLowerN2    = src._sLowerN2 ;
  _sUpperN2    = src._sUpperN2 ;
}

HPRIM2ResEntry&
HPRIM2ResEntry::operator=(const HPRIM2ResEntry& src)
{
  if (&src == this)
		return *this ;

  initFromResEntry(src) ;

  return *this ;
}

int
HPRIM2ResEntry::operator==(const HPRIM2ResEntry& o)
{
  if ((_sLabel      == o._sLabel)      &&
      (_sCode       == o._sCode)       &&
      (_sResultType == o._sResultType) &&
      (_sResult1    == o._sResult1)    &&
      (_sUnit1      == o._sUnit1)      &&
      (_sLowerN1    == o._sLowerN1)    &&
      (_sUpperN1    == o._sUpperN1)    &&
      (_sAbnormal   == o._sAbnormal)   &&
      (_sStatus     == o._sStatus)     &&
      (_sResult2    == o._sResult2)    &&
      (_sUnit2      == o._sUnit2)      &&
      (_sLowerN2    == o._sLowerN2)    &&
      (_sUpperN2    == o._sUpperN2))
    return 1 ;

  return 0 ;
}

