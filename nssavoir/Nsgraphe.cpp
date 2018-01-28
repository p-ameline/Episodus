// -----------------------------------------------------------------------------
// nsgraphe.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.18 $
// $Author: pameline $
// $Date: 2015/05/01 21:51:39 $
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// FLP - aout 2003 - ajout des liens pour les préoccupations
// ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//
//  		Implémentation des méthodes du graphe entre noeuds d'arbres
//
// -----------------------------------------------------------------------------

// #include <stdio.h>
// #include <classlib\filename.h>
// #include <sysutils.hpp>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsglobalLus.h"
  #include "partage/nsdivfile.h"
  #include "enterpriseLus/personLus.h"
#else
  #include "partage\nsglobal.h"
  #include "partage\nsdivfct.h"
  #include "partage\nsdivfile.h"
  #include "nautilus\nssuper.h"
  #include "nsbb\nstlibre.h"
  #include "nsbb\ns_objs.h"
#endif

#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsfilgd.h"
#include "nssavoir\nspatbas.h"
#include "nsbb\nspatpat.h"

#include "nsbb\nsbbtran.h"

#include "pilot\Pilot.hpp"
#include "pilot\NautilusPilot.hpp"
#include "pilot\JavaSystem.hpp"#include "nsbb\tagNames.h"
// -----------------------------------------------------------------------------
// Implémentation des méthodes NSRootLink
// -----------------------------------------------------------------------------
NSRootLink::NSRootLink()
{
}

NSRootLink::~NSRootLink()
{
}

/*

// -----------------------------------------------------------------------------
// Retourne le code qui correspond à une relation
// Return the code that represents that link
// -----------------------------------------------------------------------------
string
NSRootLink::donneString(NODELINKTYPES iRelation)
{
	switch (iRelation)
	{
		case problemRelatedTo       : return "PB" ;
		case problemContactElement  : return "PE" ;
		case soapRelatedTo          : return "SO" ;
		case problemGoals	        	: return "PG" ;
    case goalOpener             : return "GO" ;
    case goalReseter            : return "GR" ;
    case goalAlarm              : return "GA" ;
    case goalCloser             : return "GC" ;
		case archetype	            : return "AR" ;
    case refCreator	            : return "RC" ;
    case referentialOf          : return "RR" ;
		case drugOf	                : return "DG" ;
    case treatmentOf	        	: return "TT" ;
    case personHealthIndex      : return "HH" ;
    case personSynthesis        : return "HS" ;
    case personAdminData        : return "HA" ;
    case personDocument         : return "HD" ;
    case personFolderLibrary    : return "HF" ;
    case personHealthProData    : return "HP" ;
    case personHealthTeam				: return "HT" ;
    case docData                : return "ZA" ;
    case docFolder              : return "ZF" ;
    case docPilot              	: return "ZP" ;
    case personContribution     : return "HC" ;
    case contribElement         : return "ZC" ;
    case docComposition         : return "ZO" ;
    case compositionTag         : return "ZT" ;
    case processWaitingFor      : return "QW" ;
    case processResultFrom      : return "QR" ;
    case objectIn               : return "OI" ;
    case hiddenBy               : return "MH" ;
    case contributionAdded      : return "CA" ;
    case contributionModified   : return "CM" ;
    case contributionOpened			: return "CO" ;
    case copyOf									: return "YO" ;
		case fonctionalUnitStay     : return "JR" ;
		case stayContribution       : return "JC" ;
		case personIdentifiers      : return "HI" ;
  }

  return "" ;
}


// -----------------------------------------------------------------------------
// Retourne la relation qui correspond à un code
// Return the link represented by that code
// -----------------------------------------------------------------------------
NSRootLink::NODELINKTYPES
NSRootLink::donneRelation(string sRelation)
{
	if (strlen(sRelation.c_str()) < 2)
  	return badLink ;

	switch (sRelation[0])
  {
  	// ARCHETYPE related
    case 'A' :  switch (sRelation[1])
    						{
                	case 'R'	: return archetype ;
                }
                break ;

    // CONTRIBUTION related
    case 'C' :	switch (sRelation[1])
    						{
                	case 'A'	: return contributionAdded ;
                  case 'M'	: return contributionModified ;
                  case 'O'	: return contributionOpened ;
                }
                break ;

    // DRUG related
    case 'D' :  switch (sRelation[1])
    						{
    							case 'G'	: return drugOf ;
    						}
    						break ;

    // GOAL related
    case 'G' :  switch (sRelation[1])
    						{
                  case 'A'	: return goalAlarm ;
                  case 'C'	: return goalCloser ;
                	case 'O'	: return goalOpener ;
                  case 'R'	: return goalReseter ;
                }
                break ;

    // HUMAN/PERSON related
    case 'H' :  switch (sRelation[1])
    						{
                  case 'A'	: return personAdminData ;
                	case 'D'	: return personDocument ;
                  case 'C'	: return personContribution ;
                  case 'F'	: return personFolderLibrary ;
                  case 'H'	: return personHealthIndex ;
                  case 'I'  : return personIdentifiers ;
                  case 'P'	: return personHealthProData ;
                  case 'S'	: return personSynthesis ;
                  case 'T'	: return personHealthTeam ;
                }
                break ;

    // "séjour" related
    case 'J' :  switch (sRelation[1])
    						{
                	case 'C'  : return stayContribution ;
                  case 'R'  : return fonctionalUnitStay ;
                }
                break ;

    // internal MANAGEMENT related
    case 'M' :  switch (sRelation[1])
    						{
                	case 'H'	: return hiddenBy ;
                }
                break ;

    // Object related
    case 'O' :	switch (sRelation[1])
    						{
                	case 'I'	: return objectIn ;
                }
                break ;

    // POMR related
    case 'P' :  switch (sRelation[1])
    						{
                	case 'B'	: return problemRelatedTo ;
                  case 'E'	: return problemContactElement ;
                  case 'G'	: return problemGoals ;
                }
                break ;

    // Prescription related (P is already used, so we use Q here
    case 'Q'  : switch (sRelation[1])
    						{
                  case 'R'	: return processResultFrom ;
                	case 'W'	: return processWaitingFor ;
                }
                break ;

    // REFERENCE related
    case 'R' :  switch (sRelation[1])
    						{
                	case 'C'	: return refCreator ;
                  case 'R'	: return referentialOf ;
                }
                break ;

    // SOAP related
    case 'S' :  switch (sRelation[1])
    						{
                	case 'O'	: return soapRelatedTo ;
                }
                break ;

    // TREATMENT related
    case 'T' :  switch (sRelation[1])
    						{
                	case 'T'	: return treatmentOf ;
                }
                break ;

    // miscellanous
    case 'Y' :  switch (sRelation[1])
    						{
                	case 'O'	: return copyOf ;
                }
                break ;

    // DOCUMENT related
    case 'Z' :  switch (sRelation[1])
    						{
                	case 'A'	: return docData ;
                  case 'C'	: return contribElement ;
                  case 'F'	: return docFolder ;
                  case 'O'	: return docComposition ;
                  case 'P'	: return docPilot ;
                  case 'T'	: return compositionTag ;
                }
                break ;
	}

	return badLink ;
}

*/

// -----------------------------------------------------------------------------
// Retourne le code qui correspond à une relation
// Return the code that represents that link
// -----------------------------------------------------------------------------
string
NSRootLink::donneString(NODELINKTYPES iRelation)
{
	switch (iRelation)
	{
  	case problemRelatedTo            : return string("0PRO1") ;
		case problemContactElement       : return string("0PREL") ;
		case soapRelatedTo               : return string("0PRCL") ;
		case problemGoals	               : return string("0OBJE") ;
		case goalOpener                  : return string("0OJOP") ;
		case goalReseter                 : return string("0OJRE") ;
		case goalAlarm                   : return string("0OJAL") ;
		case goalCloser	                 : return string("0OJCL") ;
		case archetype	                 : return string("0ARCH") ;
		case refCreator		               : return string("0DOCR") ;
		case guidelineOf			           : return string("0ARCR") ;
    case referentialOf			         : return string("0RECR") ;
		case drugOf					             : return string("N0000") ;
		case treatmentOf						     : return string("GTRAI") ;
    case prescriptionElement         : return string("0SOA3") ;
		case personHealthIndex           : return string("ZPOMR") ;
    case personSocialIndex           : return string("ZPSOC") ;
		case personSynthesis				     : return string("ZSYNT") ;
    case personIndexExtension        : return string("0EXIX") ;
		case personAdminData             : return string("ZADMI") ;
		case personDocument              : return string("ZDOCU") ;
		case personFolderLibrary	       : return string("0LIBC") ;
		case personHealthProData	     	 : return string("DPROS") ;
    case personRiskManager           : return string("ORISK") ;
		case personHealthTeam		     		 : return string("LEQUI") ;
		case docData					     		   : return string("ZDATA") ;
		case docFolder                   : return string("0CHEM") ;
		case docPilot                    : return string("OSERV") ;
		case personContribution          : return string("LCTRI") ;
		case contribElement              : return string("") ; // non utilisé
    case indexConcerns               : return string("0PRO1") ;
    case indexGoals                  : return string("0OBJE") ;
		case docComposition              : return string("ZPRES") ;
		case compositionTag              : return string("0TAGC") ;
		case processWaitingFor           : return string("0PRWA") ;
		case processResultFrom           : return string("0PRRE") ;
		case objectIn                    : return string("OCOMP") ;
		case hiddenBy                    : return string("") ; // non utilisé
		case contributionAdded           : return string("0CTCR") ;
		case contributionModified        : return string("0CTMO") ;
		case contributionOpened          : return string("0CTCO") ;
		case copyOf                      : return string("0COPY") ;
		case fonctionalUnitStay          : return string("LSEJO") ;
		case stayContribution            : return string("0CTSE") ;
		case personIdentifiers           : return string("0LIBI") ;
    case OCRizedDocument             : return string("ZDOCR") ;
    case semantizedDocument          : return string("ZDSEM") ;
    case viewOfDocument              : return string("ZDODE") ;
    case externViewOfDocument        : return string("ZDINT") ;
    case structuredVersionOfDocument : return string("ZDSTR") ;
    case letterOfDocument            : return string("ZLETT") ;
  }

  return string("") ;
}


// -----------------------------------------------------------------------------
// Retourne la relation qui correspond à un code
// Return the link represented by that code
// -----------------------------------------------------------------------------
NSRootLink::NODELINKTYPES
NSRootLink::donneRelation(string sRelation)
{
	if (strlen(sRelation.c_str()) != 5)
  	return badLink ;

	if (string("0PRO1") == sRelation) return problemRelatedTo ;
	if (string("0PREL") == sRelation)	return problemContactElement ;
	if (string("0PRCL") == sRelation)	return soapRelatedTo ;
	if (string("0OBJE") == sRelation)	return problemGoals ;
	if (string("0OJOP") == sRelation)	return goalOpener	;
	if (string("0OJRE") == sRelation)	return goalReseter ;
	if (string("0OJAL") == sRelation)	return goalAlarm ;
	if (string("0OJCL") == sRelation)	return goalCloser	;
	if (string("0ARCH") == sRelation)	return archetype ;
	if (string("0DOCR") == sRelation)	return refCreator	;
	if (string("0ARCR") == sRelation)	return guidelineOf ;
  if (string("0RECR") == sRelation)	return referentialOf ;
	if (string("N0000") == sRelation)	return drugOf	;
	if (string("GTRAI") == sRelation)	return treatmentOf ;
  if (string("0SOA3") == sRelation)	return prescriptionElement ;
	if (string("ZPOMR") == sRelation)	return personHealthIndex ;
  if (string("ZPSOC") == sRelation)	return personSocialIndex ;
	if (string("ZSYNT") == sRelation)	return personSynthesis ;
  if (string("0EXIX") == sRelation)	return personIndexExtension ;
	if (string("ZADMI") == sRelation)	return personAdminData ;
	if (string("ZDOCU") == sRelation)	return personDocument	;
	if (string("0LIBC") == sRelation)	return personFolderLibrary ;
	if (string("DPROS") == sRelation)	return personHealthProData ;
  if (string("ORISK") == sRelation)	return personRiskManager ;
	if (string("LEQUI") == sRelation)	return personHealthTeam	;
	if (string("ZDATA") == sRelation)	return docData ;
	if (string("0CHEM") == sRelation)	return docFolder ;
	if (string("OSERV") == sRelation)	return docPilot ;
  if (string("0PRO1") == sRelation)	return indexConcerns ;
  if (string("0OBJE") == sRelation)	return indexGoals ;
	if (string("LCTRI") == sRelation)	return personContribution ;
	if (string("ZPRES") == sRelation)	return docComposition ;
	if (string("0TAGC") == sRelation)	return compositionTag ;
	if (string("0PRWA") == sRelation)	return processWaitingFor ;
	if (string("0PRRE") == sRelation)	return processResultFrom ;
	if (string("OCOMP") == sRelation)	return objectIn ;
	if (string("0CTCR") == sRelation)	return contributionAdded ;
	if (string("0CTMO") == sRelation)	return contributionModified ;
	if (string("0CTCO") == sRelation)	return contributionOpened ;
	if (string("0COPY") == sRelation)	return copyOf ;
	if (string("LSEJO") == sRelation)	return fonctionalUnitStay ;
	if (string("0CTSE") == sRelation)	return stayContribution ;
	if (string("0LIBI") == sRelation)	return personIdentifiers ;
  if (string("ZDOCR") == sRelation)	return OCRizedDocument ;
  if (string("ZDSEM") == sRelation)	return semantizedDocument ;
  if (string("ZDODE") == sRelation)	return viewOfDocument ;
  if (string("ZDINT") == sRelation) return externViewOfDocument ;
  if (string("ZDSTR") == sRelation) return structuredVersionOfDocument ;
  if (string("ZLETT") == sRelation)	return letterOfDocument ;

	return badLink ;
}

///////////////////////////////////////////////////////////////
//
// Classe NSLinkManager
//
///////////////////////////////////////////////////////////////

// Constructeur et destructeur
NSLinkManager::NSLinkManager(NSSuper* pSuper, NSDataGraph* pDataGraph)
              :NSRootLink(), NSSuperRoot(pSuper)
{
    pGraph = pDataGraph;
    setTypeGraphe(pGraph->getGraphType());
}

NSLinkManager::~NSLinkManager()
{
}

NSLinkManager::NSLinkManager(const NSLinkManager& rv)
              :NSRootLink(), NSSuperRoot(rv._pSuper)
{
  pGraph = rv.pGraph ;
  setTypeGraphe(pGraph->getGraphType()) ;
}

NSLinkManager&
NSLinkManager::operator=(const NSLinkManager& src)
{
	if (&src == this)
  	return *this ;

	pGraph = src.pGraph ;
  setTypeGraphe(pGraph->getGraphType()) ;

	return *this ;
}

// savoir si sCode1 et sCode2 sont liés par sRelation
// note : le lien inverse est donné par sensCle = "ENVERS"
bool
NSLinkManager::VraiOuFaux(string sNoeud1, NODELINKTYPES iRelation, string sNoeud2, DBITBLNAME sensCle)
{
	if ((string("") == sNoeud1) || (string("") == sNoeud2))
		return false ;

	bool bOk = false ;

	VecteurString VecteurString ;
	TousLesVrais(sNoeud1, iRelation, &VecteurString, sensCle) ;

	// sNoeud2 fait il partie de pVecteurString ?
	// does sNoeud2 belong to pVecteurString ?

	if ((false == VecteurString.empty()) &&
			(VecteurString.ItemDansUnVecteur(sNoeud2)))
		bOk = true ;

	return bOk ;
}

// mettre tous les items liés à sCode1 par sRelation dans pVecteurString ou pLinkArray
// note : le lien inverse est donné par sensCle = "ENVERS"
void
NSLinkManager::TousLesVrais(string sNoeud, NODELINKTYPES iRelation, VecteurString *pVecteurString, DBITBLNAME sensCle)
{
	if ((string("") == sNoeud) || ((VecteurString *) NULL == pVecteurString))
		return ;

  if (true == pGraph->getLinks()->empty())
    return ;

	// Recherche du code qui correspond à ce lien
	string sLien = donneString(iRelation) ;
	if (string("") == sLien)
		return ;

	for (NSPatLinkIter i = pGraph->getLinks()->begin(); pGraph->getLinks()->end() != i ; i++)
	{
  	if (((*i)->getLien() == sLien) &&
        (((strcmp(sensCle, "FLECHE") == 0) && ((*i)->getQualifie() == sNoeud)) ||
         ((strcmp(sensCle, "ENVERS") == 0) && ((*i)->getQualifiant() == sNoeud)))
       )
    {
    	// Vérifier que l'élément n'appartient pas au vecteur
      // Checking if we have not already found that node
      if ((strcmp(sensCle, "FLECHE") == 0) &&
          (pVecteurString->ItemDansUnVecteur((*i)->getQualifiant()) == false))
      	pVecteurString->AddString((*i)->getQualifiant()) ;

      if ((strcmp(sensCle, "ENVERS") == 0) &&
          (pVecteurString->ItemDansUnVecteur((*i)->getQualifie()) == false))
      	pVecteurString->AddString((*i)->getQualifie()) ;
    }
  }
}

void
NSLinkManager::TousLesVraisDocument(string sDocRoot, NODELINKTYPES iRelation, VecteurString *pVecteurString, DBITBLNAME sensCle)
{
	if ((string("") == sDocRoot) || (NULL == pVecteurString))
		return ;

	TousLesVrais(sDocRoot, iRelation, pVecteurString, sensCle) ;

  string sMeta = giveMetaDataRoot(sDocRoot) ;

  TousLesVrais(sMeta, iRelation, pVecteurString, sensCle) ;
}

void
NSLinkManager::TousLesVrais(string sNoeud, NODELINKTYPES iRelation, NSPatLinkArray *pLinkArray, DBITBLNAME sensCle)
{
	if ((NULL == pLinkArray) || (string("") == sNoeud))
		return ;

  if (true == pGraph->getLinks()->empty())
    return ;

	// Recherche du code qui correspond à ce lien
	string sLien = donneString(iRelation) ;
	if (string("") == sLien)
		return ;

  VecteurString VecteurString ;

  for (NSPatLinkIter i = pGraph->getLinks()->begin(); pGraph->getLinks()->end() != i ; i++)
  {
  	if (((*i)->getLien() == sLien) &&
        (((strcmp(sensCle, "FLECHE") == 0) && ((*i)->getQualifie() == sNoeud)) ||
         ((strcmp(sensCle, "ENVERS") == 0) && ((*i)->getQualifiant() == sNoeud))))
    {
    	// Vérifier que l'élément n'appartient pas au vecteur
			// Checking if we have not already found that node
			if ((strcmp(sensCle, "FLECHE") == 0) &&
					(VecteurString.ItemDansUnVecteur((*i)->getQualifiant()) == false))
      {
				VecteurString.AddString((*i)->getQualifiant()) ;
        pLinkArray->push_back(new NSPatLinkInfo(*(*i))) ;
      }

			if ((strcmp(sensCle, "ENVERS") == 0) &&
					(VecteurString.ItemDansUnVecteur((*i)->getQualifie()) == false))
      {
      	VecteurString.AddString((*i)->getQualifie()) ;
        pLinkArray->push_back(new NSPatLinkInfo(*(*i))) ;
      }
    }
  }
}

// mettre tous les items liés par iRelation dans pNoeudParam et leurs noeuds liés dans pNoeudResult
void
NSLinkManager::TousLesVrais(string sTreeID, NODELINKTYPES iRelation, VecteurString *pNoeudParam, VecteurString *pNoeudResult, DBITBLNAME sensCle)
{
	if ((string("") == sTreeID) || (NULL == pNoeudParam) || (NULL == pNoeudResult))
		return ;

  if (true == pGraph->getLinks()->empty())
    return ;

  if (strlen(sTreeID.c_str()) != OBJECT_ID_LEN)
  {
  	erreur("Erreur de format du treeID dans NSLinkManager::TousLesVrais(...)", standardError) ;
    return ;
  }

	// Recherche du code qui correspond à ce lien
	string sLien = donneString(iRelation) ;
	if (sLien == "")
		return ;

  size_t lenTree = OBJECT_ID_LEN ;

  for (NSPatLinkIter i = pGraph->getLinks()->begin() ; pGraph->getLinks()->end() != i ; i++)
  {
  	if (((*i)->getLien() == sLien) &&
        (((strcmp(sensCle, "FLECHE") == 0) && (string((*i)->getQualifie(), 0, lenTree) == sTreeID)) ||
         ((strcmp(sensCle, "ENVERS") == 0) && (string((*i)->getQualifiant(), 0, lenTree) == sTreeID))))
    {
    	if (strcmp(sensCle, "FLECHE") == 0)
      {
				pNoeudResult->AddString((*i)->getQualifiant()) ;
        pNoeudParam->AddString((*i)->getQualifie()) ;
      }

			if (strcmp(sensCle, "ENVERS") == 0)
    	{
				pNoeudResult->AddString((*i)->getQualifie()) ;
        pNoeudParam->AddString((*i)->getQualifiant()) ;
      }
    }
  }
}

void
NSLinkManager::TousLesVrais(string sPatientID, NSPatLinkArray *pLinkArray, DBITBLNAME sensCle)
{
	if ((string("") == sPatientID) || (NULL == pLinkArray))
		return ;

  if (true == pGraph->getLinks()->empty())
    return ;

	if (strlen(sPatientID.c_str()) != PAT_NSS_LEN)
  {
  	erreur("Erreur de format du PatientID dans NSLinkManager::TousLesVrais(...)", standardError) ;
    return ;
  }

	// string sLien = string("  ");

  VecteurString VecteurString ;

  size_t lenNss = PAT_NSS_LEN ;

  for (NSPatLinkIter i = pGraph->getLinks()->begin(); pGraph->getLinks()->end() != i; i++)
  {
  	if (((strcmp(sensCle, "FLECHE") == 0) && (string((*i)->getQualifie(), 0, lenNss) == sPatientID)) ||
        ((strcmp(sensCle, "ENVERS") == 0) && (string((*i)->getQualifiant(), 0, lenNss) == sPatientID)))
    {
    	// Vérifier que l'élément n'appartient pas au vecteur
			// Checking if we have not already found that node
			if ((strcmp(sensCle, "FLECHE") == 0) &&
          (VecteurString.ItemDansUnVecteur((*i)->getQualifiant()) == false))
      {
				VecteurString.AddString((*i)->getQualifiant()) ;
        pLinkArray->push_back(new NSPatLinkInfo(*(*i))) ;
      }

			if ((strcmp(sensCle, "ENVERS") == 0) &&
          (VecteurString.ItemDansUnVecteur((*i)->getQualifie()) == false))
      {
      	VecteurString.AddString((*i)->getQualifie()) ;
        pLinkArray->push_back(new NSPatLinkInfo(*(*i))) ;
      }
    }
  }
}

bool
NSLinkManager::etablirLien(string sQualifie, NODELINKTYPES iRelation, string sQualifiant)
{
	// Recherche du code qui correspond à ce lien
  //
	string sLien = donneString(iRelation) ;
	if (string("") == sLien)
		return false ;

  return etablirLien(sQualifie, sLien, sQualifiant) ;
}

bool
NSLinkManager::etablirLien(string sQualifie, string sLink, string sQualifiant)
{
try
{
	if ((string("") == sQualifie) || (string("") == sLink) || (string("") == sQualifiant))
		return false ;

	if ((strlen(sQualifie.c_str())   > PATLINK_QUALIFIE_LEN) ||
			(strlen(sQualifiant.c_str()) > PATLINK_QUALIFIANT_LEN) ||
      (strlen(sLink.c_str())      != BASE_SENS_LEN))
		return false ;

  NSPatLinkInfo* pLinkInfo = new NSPatLinkInfo() ;

  pLinkInfo->setQualifie(sQualifie) ;
  pLinkInfo->setLien(sLink) ;
	pLinkInfo->setQualifiant(sQualifiant) ;

  // on signale le lien pour sauvegarder le document attaché à la fermeture du patient
  //
  pLinkInfo->setDirty(true) ;

    /********* La transaction n'est gérée que pour les enregistrements en base
    if ((strncmp(pLinkInfo->pDonnees->qualifie, "_------", PAT_NSS_LEN) == 0) &&
        (strncmp(pLinkInfo->pDonnees->qualifiant, "_------", PAT_NSS_LEN) == 0))
        strcpy(pLinkInfo->pDonnees->tranNew, (pContexte->getTransactionSysteme()).c_str());
    else
        strcpy(pLinkInfo->pDonnees->tranNew, (pContexte->getTransaction()).c_str());
    *********************************************************************************/

  pGraph->getLinks()->push_back(pLinkInfo) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSLinkManager::etablirLien.", standardError) ;
  return false ;
}
}

// This function establishes a link to the meta-data tree instead of the data tree
//
bool
NSLinkManager::etablirLienDocument(string sQualifie, NODELINKTYPES iRelation, string sQualifiant)
{
	if ((string("") == sQualifie) || (string("") == sQualifiant))
		return false ;

	string sNewQualifie   = sQualifie ;
  string sNewQualifiant = sQualifiant ;

	if (strlen(sQualifie.c_str()) < PATLINK_QUALIFIE_LEN)
  {
  	string sMetaQualifie = giveMetaDataRoot(sQualifie) ;
    if (sMetaQualifie != string(""))
    	sNewQualifie = sMetaQualifie ;
  }

  if (strlen(sQualifiant.c_str()) < PATLINK_QUALIFIE_LEN)
  {
  	string sMetaQualifiant = giveMetaDataRoot(sQualifiant) ;
    if (sMetaQualifiant != string(""))
    	sNewQualifiant = sMetaQualifiant ;
  }

	return etablirLien(sNewQualifie, iRelation, sNewQualifiant) ;
}

bool
NSLinkManager::detruireTousLesLiens(string sQualifie, NODELINKTYPES iRelation)
{
  if (true == pGraph->getLinks()->empty())
    return true ;

	if (string("") == sQualifie)
		return false ;

	if (strlen(sQualifie.c_str()) > PATLINK_QUALIFIE_LEN)
		return false ;

	// Recherche du code qui correspond à ce lien
	string sLien = donneString(iRelation) ;
	if (string("") == sLien)
		return false ;

  // on détruit tous les liens qualifie + relation (sens "FLECHE" et "ENVERS")
  for (NSPatLinkIter i = pGraph->getLinks()->begin(); pGraph->getLinks()->end() != i; )
  {
  	if (((*i)->getLien() == sLien) &&
        (((*i)->getQualifie() == sQualifie) || ((*i)->getQualifiant() == sQualifie)))
    {
    	delete (*i) ;
      pGraph->getLinks()->erase(i) ;
    }
    else
    	i++ ;
  }

  return true ;
}

bool
NSLinkManager::detruireTousLesLiensDocument(string sDocRoot, NODELINKTYPES iRelation)
{
	if (sDocRoot == "")
		return false ;

	if (!detruireTousLesLiens(sDocRoot, iRelation))
		return false ;

	string sMetaRoot = giveMetaDataRoot(sDocRoot) ;
  if (sMetaRoot == string(""))
  	return true ;

  return detruireTousLesLiens(sMetaRoot, iRelation) ;
}


bool
NSLinkManager::detruireTousLesLiens(string sNoeud)
{
	if (sNoeud == "")
		return false ;

  if (true == pGraph->getLinks()->empty())
    return true ;

	// on détruit tous les liens où sNoeud est qualifie ou qualifiant
	for (NSPatLinkIter i = pGraph->getLinks()->begin(); pGraph->getLinks()->end() != i ; )
	{
		if (strlen(sNoeud.c_str()) == PATLINK_QUALIFIE_LEN)
		{
			if (((*i)->getQualifie() == sNoeud) ||
                ((*i)->getQualifiant() == sNoeud))
			{
				delete (*i) ;
				pGraph->getLinks()->erase(i) ;
			}
			else
				i++ ;
		}
		else if (strlen(sNoeud.c_str()) == OBJECT_ID_LEN)
		{
			if (((*i)->getQualifie() == sNoeud) ||
                ((*i)->getQualifiant() == sNoeud))
			{
				delete (*i) ;
				pGraph->getLinks()->erase(i) ;
			}
			else
				i++ ;
		}
		else
			i++ ;
	}

	return true ;
}

bool
NSLinkManager::detruireTousLesLiens()
{
	pGraph->getLinks()->vider() ;
  return true ;
}

bool
NSLinkManager::detruireLien(string sQualifie, NODELINKTYPES iRelation, string sQualifiant)
{
  if (true == pGraph->getLinks()->empty())
    return true ;

	// ---------------------------------------------------------------------------
	// les paramètres sont-ils correct ?
	// ---------------------------------------------------------------------------

	if (string("") == sQualifie)
		return false ;

	if (strlen(sQualifie.c_str()) > PATLINK_QUALIFIE_LEN)
		return false ;

	// Recherche du code qui correspond à ce lien
	string sLien = donneString(iRelation) ;
	if (sLien == "")
		return false ;

	if (string("") == sQualifiant)
		return false ;

	if (strlen(sQualifiant.c_str()) > PATLINK_QUALIFIANT_LEN)
		return false ;

	// on détruit tous les liens qualifie + relation + qualifiant
	for (NSPatLinkIter i = pGraph->getLinks()->begin(); pGraph->getLinks()->end() != i; )
	{
		if (((*i)->getLien()       == sLien) &&
        ((*i)->getQualifie()   == sQualifie) &&
        ((*i)->getQualifiant() == sQualifiant))
		{
    	delete (*i) ;
      pGraph->getLinks()->erase(i) ;
    }
    else
    	i++ ;
  }

	return true ;
}

bool
NSLinkManager::existeLien(string sQualifie, NODELINKTYPES iRelation, string sQualifiant)
{
  if (true == pGraph->getLinks()->empty())
    return false ;

	// ---------------------------------------------------------------------------
	// les paramètres sont-ils correct ?
	// ---------------------------------------------------------------------------

	if (string("") == sQualifie)
		return false ;

	if (strlen(sQualifie.c_str()) > PATLINK_QUALIFIE_LEN)
		return false ;

	// Recherche du code qui correspond à ce lien
	string sLien = donneString(iRelation) ;
	if (string("") == sLien)
		return false ;

	if (string("") == sQualifiant)
  	return false ;

	if (strlen(sQualifiant.c_str()) > PATLINK_QUALIFIANT_LEN)
  	return false ;

	bool trouve = false ;

	// on cherche un lien qualifie + relation + qualifiant
	for (NSPatLinkIter i = pGraph->getLinks()->begin(); pGraph->getLinks()->end() != i; i++)
	{
		if (((*i)->getLien()       == sLien) &&
        ((*i)->getQualifie()   == sQualifie) &&
        ((*i)->getQualifiant() == sQualifiant))
    {
    	trouve = true ;
      break ;
    }
	}

	return trouve ;
}

bool
NSLinkManager::swapLiens(string sOldNode, NSPatPathoInfo* pNewPpt)
{
  if ((string("") == sOldNode) || (NULL == pNewPpt) || (NULL == pGraph))
    return false ;

  NSPatLinkArray* pLinks = pGraph->getLinks() ;
  if ((NULL == pLinks) || (true == pLinks->empty()))
    return true ;

  // Qualifie - flêche -> Qualifiant
  //

  for (NSPatLinkIter i = pLinks->begin() ; pLinks->end() != i ; i++)
	{
    // We also reconnect links like contributionAdded and contributionModified
    // to somehow "track" history
    //
    if ((*i)->getQualifie() == sOldNode)
    {
      NSRootLink::NODELINKTYPES iLkType = donneRelation((*i)->getLien()) ;
      pNewPpt->addTemporaryLink((*i)->getQualifiant(), iLkType, dirFleche) ;
    }

    if ((*i)->getQualifiant() == sOldNode)
    {
      NSRootLink::NODELINKTYPES iLkType = donneRelation((*i)->getLien()) ;
      pNewPpt->addTemporaryLink((*i)->getQualifie(), iLkType, dirEnvers) ;
    }
  }

	return true ;
}

string
NSLinkManager::nextDocToSave()
{
  if (NULL == pGraph)
    return string("") ;

  NSPatLinkArray* pLinks = pGraph->getLinks() ;
  if ((NULL == pLinks) || (true == pLinks->empty()))
    return string("") ;

	string sNextDoc = string("") ;
  int    maxLinks = 0 ;

  VecteurString VecteurString ;

  // A FAIRE : Etablir une priorité en cas de lien d'un document vers l'index de santé
  // pour enregistrer le document de préférence à l'index de santé
  for (NSPatLinkIter i = pLinks->begin() ; pLinks->end() != i ; i++)
  {
  	if ((*i)->getDirty())
    {
    	string sDocum = string((*i)->getQualifie(), 0, OBJECT_ID_LEN) ;
      if (VecteurString.ItemDansUnVecteur(sDocum) == false)
      {
      	VecteurString.AddString(sDocum) ;
        int numLinks = numberDirtyLinksToDoc(sDocum) ;
        if (numLinks > maxLinks)
        {
        	sNextDoc = sDocum ;
          maxLinks = numLinks ;
        }
      }

      sDocum = string((*i)->getQualifiant(), 0, OBJECT_ID_LEN) ;
      if (VecteurString.ItemDansUnVecteur(sDocum) == false)
      {
      	VecteurString.AddString(sDocum) ;
        int numLinks = numberDirtyLinksToDoc(sDocum) ;
        if (numLinks > maxLinks)
        {
        	sNextDoc = sDocum ;
          maxLinks = numLinks ;
        }
      }
    }
  }

	return sNextDoc ;
}

int
NSLinkManager::numberDirtyLinksToDoc(string sTreeID)
{
  if (true == pGraph->getLinks()->empty())
    return 0 ;

	int cpt = 0 ;

	for (NSPatLinkIter i = pGraph->getLinks()->begin(); pGraph->getLinks()->end() != i; i++)
	{
  	if ((*i)->getDirty())
    {
    	if (string((*i)->getQualifie(), 0, OBJECT_ID_LEN) == sTreeID)
      	cpt += 1 ;

      if (string((*i)->getQualifiant(), 0, OBJECT_ID_LEN) == sTreeID)
      	cpt += 1 ;
    }
  }

	return cpt ;
}

string
NSLinkManager::giveMetaDataRoot(string sDocRoot)
{
	VecteurString aVecteurString ;
  TousLesVrais(sDocRoot, NSRootLink::docData, &aVecteurString, "ENVERS") ;
  if (aVecteurString.empty())
  	return string("") ;

	return **(aVecteurString.begin()) ;
}

///////////////////////////////////////////////////////////////
//
// Classe NSArcNode
//
///////////////////////////////////////////////////////////////

NSArcNode::NSArcNode(string sNode, string sArc, int iEtat)
{
  _sNode = sNode ;
  _sArc  = sArc ;
  _iEtat = iEtat ;
}

NSArcNode::~NSArcNode()
{
}

NSArcNode::NSArcNode(const NSArcNode& rv)
{
  _sNode = rv._sNode ;
  _sArc  = rv._sArc ;
  _iEtat = rv._iEtat ;
}

NSArcNode&
NSArcNode::operator=(const NSArcNode& src)
{
	if (&src == this)
  	return *this ;

	_sNode = src._sNode ;
  _sArc  = src._sArc ;
  _iEtat = src._iEtat ;

  return *this ;
}

// Classe NSArcNodeArray
//////////////////////////////////////////

//---------------------------------------------------------------------------
//  Constructeur copie//---------------------------------------------------------------------------
NSArcNodeArray::NSArcNodeArray(const NSArcNodeArray& rv)               :NSArcNodeVector(){try{	if (false == rv.empty())  	for (NSArcNodeConstIter i = rv.begin(); rv.end() != i ; i++)    	push_back(new NSArcNode(*(*i))) ;}catch (...)
{
	erreur("Exception NSArcNodeArray copy ctor.", standardError) ;
  return ;
}}
NSArcNodeArray&
NSArcNodeArray::operator=(const NSArcNodeArray& src)
{try{	if (&src == this)  	return *this ;	if (false == src.empty())  	for (NSArcNodeConstIter i = src.begin() ; src.end() != i ; i++)    	push_back(new NSArcNode(*(*i))) ;	return *this ;}catch (...)
{
	erreur("Exception NSArcNodeArray = operator.", standardError) ;
  return *this ;
}}

//---------------------------------------------------------------------------//  Destructeur//---------------------------------------------------------------------------void
NSArcNodeArray::vider(){	if (false == empty())		for (NSArcNodeIter i = begin() ; end() != i ; )		{    	delete *i ;      erase(i) ;    }}
NSArcNodeArray::~NSArcNodeArray(){	vider() ;}

///////////////////////////////////////////////////////////////
//
// Classe NSArcManager
//
///////////////////////////////////////////////////////////////

NSArcManager::NSArcManager(NSSuper* pSuper)
             :NSSuperRoot(pSuper)
{
	// NSSuper *pLocalSup = pContexte->getSuperviseur() ;
}

NSArcManager::NSArcManager(const NSArcManager& src)
             :NSSuperRoot(src._pSuper)
{
	_aModelArray = src._aModelArray ;
}

NSArcManager::~NSArcManager()
{
}

NSArcManager&
NSArcManager::operator=(const NSArcManager& src)
{
	if (this == &src)
  	return (*this) ;

	_aModelArray	= src._aModelArray ;

	return (*this) ;
}

bool
NSArcManager::OuvreLibrairie()
{
try
{
	erreur("Fonction NSArcManager::OuvreLibrairie non implémentée dans le mode N_TIERS.", standardError) ;
	return false ;
}
catch (...)
{
	erreur("Exception NSArcManager::OuvreLibrairie.", standardError) ;
	return false ;
}}

string
NSArcManager::DonneNomArchetypeLie(TYPEARC type, string sNode, NSPersonGraphManager* pGraphPerson)
{
try
{
  if ((NSPersonGraphManager *) NULL == pGraphPerson)
    return string("") ;

	// En N_TIERS sNodeArc représente le model_object_id
  NSDataGraph* pGraph = pGraphPerson->getDataGraph() ;
  if (pGraph->getArchs()->empty())
  	return string("") ;

	string sTypeLink ;
	if      (NSArcManager::archetype == type)
		sTypeLink = pGraphPerson->getLinkManager()->donneString(NSRootLink::archetype) ;
	else if (NSArcManager::referentiel == type)
		sTypeLink = pGraphPerson->getLinkManager()->donneString(NSRootLink::referentialOf) ;
  else if (NSArcManager::decisionTree == type)
		sTypeLink = pGraphPerson->getLinkManager()->donneString(NSRootLink::referentialOf) ;

  string sNodeArc = string("") ;

  for (NSArcLinkIter i = pGraph->getArchs()->begin(); pGraph->getArchs()->end() != i; i++)
  {
  	if (((*i)->getFullNode() == sNode) && ((*i)->getType() == sTypeLink))
    {
    	sNodeArc = (*i)->getMOId() ;
      break ;
    }
  }

  if (string("") == sNodeArc)
  	return string("") ;

	return DonneNomArchetypeDansLibrairie(type, sNodeArc) ;
}
catch (...)
{
	erreur("Exception NSArcManager::DonneNomArchetypeLie.", standardError) ;
  return "" ;
}
}

string
NSArcManager::DonneNomArchetypeDansLibrairie(TYPEARC type, string sNodeArc)
{
try
{
	string sType    = string("") ;
  string sTypeLex = string("") ;

	if      (NSArcManager::archetype == type)
	{
		sTypeLex = string("0ARCH1") ;
		sType = string("0ARCH") ;
	}
	else if (NSArcManager::referentiel == type)
	{
		sTypeLex = string("0REFE1") ;
		sType = string("0REFE") ;
	}
  else if (NSArcManager::decisionTree == type)
  {
  	sTypeLex = string("0ARDE1") ;
    sType = string("0ARDE") ;
  }
	else
	{
		erreur("Type inconnu dans ::DonneNomArchetypeDansLibrairie.", standardError) ;
		return "" ;
	}

	// en N_TIERS on cherche d'abord en mémoire, sinon on fait un appel au pilote
	// Remarque : ici sNodeArc représente l'object_id du modele
	NSDataTreeIter iterTree ;
	if (_aModelArray.ExisteTreeID(sNodeArc, &iterTree))
  {
    string sModelName = (*iterTree)->getModelName() ;
    if (string("") != sModelName)
		  return sModelName ;
  }

	// ------------ Appel du pilote
	//
	NSDataGraph     Graph(_pSuper, graphObject) ;

	//
	//Il faut creer la variable sTraitName = "_" + nomObjet + "_" + typeobjet
	string sTraitName = string("") ;
  if      (NSArcManager::archetype    == type)
		sTraitName = "_0ARCH_0ARID" ;
	else if (NSArcManager::referentiel  == type)
		sTraitName = "_0REFE_0ARID" ;
  else if (NSArcManager::decisionTree == type)
		sTraitName = "_0ARDE_0ARID" ;

	NSBasicAttributeArray AttrArray ;
	AttrArray.push_back(new NSBasicAttribute(OBJECT, sNodeArc)) ;

	bool res = _pSuper->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(),
                                    &Graph, &AttrArray) ;
	if (false == res)
	{
		string sErrorText = _pSuper->getText("NTIERS", "modelNotFound") + string(" ") + sNodeArc ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return string("") ;
	}

	if (false == Graph.getTrees()->ExisteTree(sTypeLex, _pSuper, &iterTree))
    return string("") ;

  NSPatPathoArray* pPatPathoArray = (*iterTree)->getPatPathoPointer() ;
  if (((NSPatPathoArray*) NULL == pPatPathoArray) || pPatPathoArray->empty())
    return string("") ;

  PatPathoIter iter    = pPatPathoArray->begin() ;
	PatPathoIter iterEnd = pPatPathoArray->end() ;

	if ((NULL == iter) || (iter == iterEnd))
  {
		erreur("Le noeud archetype est incorrect dans ::DonneNomArchetypeDansLibrairie().", standardError) ;
		return string("") ;
	}

  string sElemLex = (*iter)->getLexique() ;
  string sSens    = string("") ;
  NSDico::donneCodeSens(&sElemLex, &sSens) ;

  if (sSens != sType)
  {
    erreur("Le noeud archetype est mal positionné dans ::DonneNomArchetypeDansLibrairie().", standardError) ;
    return string("") ;
  }

  int iColArc = (*iter)->getColonne() ;
  iter++ ;

  bool bNomTrouve = false ;

  string sNomArc  = string("") ;
  string sFichArc = string("") ;

  // on charge les données de l'archetype
  while ((iter != iterEnd) && ((*iter)->getColonne() > iColArc))
  {
    sElemLex = (*iter)->getLexique() ;
    NSDico::donneCodeSens(&sElemLex, &sSens) ;

    // identifiant (unique)
    if (sSens == string("0ARID"))
    {
      iter++ ;
      sNomArc = string("") ;

      while ((iter != iterEnd) && ((*iter)->getColonne() > iColArc+1))
      {
        // on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;

        if (sElemLex == string("£?????"))
        {
          sNomArc = (*iter)->getTexteLibre() ;
          bNomTrouve = true ;
        }

        iter++ ;
      }
    }
    // nom du fichier (unique)
    else if (sSens == string("0NFIC"))
    {
      iter++ ;

      while ((iter != iterEnd) && ((*iter)->getColonne() > iColArc+1))
      {
        // on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;

        if (sElemLex == string("£?????"))
          sFichArc = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    else
      iter++ ;
  }

  if (bNomTrouve)
  {
    (*iterTree)->setModelName(sNomArc) ;
    (*iterTree)->setModelFileName(sFichArc) ;
    _aModelArray.push_back(new NSDataTree(*(*iterTree))) ;

    return sNomArc ;
  }

	return string("") ;
}
catch (...)
{
	erreur("Exception NSArcManager::DonneNomArchetypeDansLibrairie.", standardError) ;
  return string("") ;
}
}

string
NSArcManager::DonneFichierArchetypeParNom(TYPEARC type, string sNomArc, bool bAvecChemin)
{
try
{
  string sType    = string("") ;
  string sTypeLex = string("") ;

	if      (NSArcManager::archetype == type)
  {
  	sTypeLex = string("0ARCH1") ;
    sType    = string("0ARCH") ;
  }
  else if (NSArcManager::referentiel == type)
  {
  	sTypeLex = string("0REFE1") ;
    sType    = string("0REFE") ;
  }
  else if (NSArcManager::decisionTree == type)
  {
  	sTypeLex = string("0ARDE1") ;
    sType    = string("0ARDE") ;
  }
  else
  {
  	erreur("Type inconnu dans ::DonneFichierArchetypeParNom.", standardError) ;
    return string("") ;
  }

	// en N_TIERS on cherche d'abord en mémoire, sinon on fait un appel au pilote
	// on doit parcourir l'array des modèles
	NSDataTreeIter iterTree ;
	string sPathName = string("") ;

	if (false == _aModelArray.empty())
		for (iterTree = _aModelArray.begin() ; _aModelArray.end() != iterTree ; iterTree++)
		{
			if ((*iterTree)->getModelName() == sNomArc)
			{
#ifndef _ENTERPRISE_DLL
				if (bAvecChemin)
					sPathName = _pSuper->PathName("IXML") + (*iterTree)->getModelFileName() ;
				else
#endif
					sPathName = (*iterTree)->getModelFileName() ;

				return sPathName ;
			}
		}

	// ------------ Appel du pilote
	NSDataGraph     Graph(_pSuper, graphObject) ;

	//Il faut creer la variable sTraitName = "_" + nomObjet + "_" + typeobjet
	string  sTraitName ;

	if      (NSArcManager::archetype    == type)
		sTraitName = "_0ARCH_0ARID" ;
	else if (NSArcManager::referentiel  == type)
		sTraitName = "_0REFE_0ARID" ;
  else if (NSArcManager::decisionTree == type)
		sTraitName = "_0ARDE_0ARID" ;

	// char szTraitName[13];
	// strcpy(szTraitName, sTraitName.c_str());

	//appel invokeService pour la recherche d'un graph

	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrList ;
	AttrList.push_back(new NSBasicAttribute(sTraitName, sNomArc)) ;

	bool res = _pSuper->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT.c_str(),
                                    &Graph, &ObjList, &AttrList) ;

	if (false == res)
	{
		string sErrorText = _pSuper->getText("NTIERS", "modelNotFound") ;
    sErrorText += string(" (") + sNomArc + string(")") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
  	return string("") ;
	}

	if (false == Graph.getTrees()->ExisteTree(sTypeLex, _pSuper, &iterTree))
  {
    string sErrorText = string("No tree found for model ") + sNomArc ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	return string("") ;
  }

  NSPatPathoArray* pPatPathoArray = (*iterTree)->getPatPathoPointer() ;

  if (((NSPatPathoArray*) NULL == pPatPathoArray) || pPatPathoArray->empty())
  {
    string sErrorText = string("Empty tree found for model ") + sNomArc ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur("Le noeud du modele est incorrect dans ::DonneFichierArchetypeParNom().", standardError) ;
		return string("") ;
	}

  PatPathoIter iter    = pPatPathoArray->begin() ;
  PatPathoIter iterEnd = pPatPathoArray->end() ;

  string sElemLex = (*iter)->getLexique() ;
  string sSens    = string("") ;
  NSDico::donneCodeSens(&sElemLex, &sSens);

  if (sSens != sType)
  {
  	erreur("Le noeud du modele est mal positionné dans ::DonneFichierArchetypeParNom().", standardError) ;
    return string("") ;
  }

  int iColArc = (*iter)->getColonne() ;
  iter++ ;

  string sFichArc = string("") ;

  // on charge les données de l'archetype
  while ((iter != iterEnd) && ((*iter)->getColonne() > iColArc))
  {
  	sElemLex = (*iter)->getLexique() ;
    NSDico::donneCodeSens(&sElemLex, &sSens) ;

    // nom du fichier (unique)
    if (sSens == string("0NFIC"))
    {
    	iter++ ;

      while ((iter != iterEnd) && ((*iter)->getColonne() > iColArc+1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;

        if (sElemLex == string("£?????"))
        	sFichArc = (*iter)->getTexteLibre() ;

        iter++;
      }
    }
    else
    	iter++ ;
  }

  (*iterTree)->setModelName(sNomArc) ;
  (*iterTree)->setModelFileName(sFichArc) ;
  _aModelArray.push_back(new NSDataTree(*(*iterTree))) ;

#ifndef _ENTERPRISE_DLL
  if (bAvecChemin)
  	sPathName = _pSuper->PathName("IXML") + (*iterTree)->getModelFileName() ;
  else
#endif
  	sPathName = (*iterTree)->getModelFileName() ;

  return sPathName ;
}
catch (...)
{
	erreur("Exception NSArcManager::DonneFichierArchetypeParNom.", standardError) ;
	return "" ;
}
}

bool
NSArcManager::ExisteFichierArchetype(string sFichArc, string sPathArc, string& /* sNomArc */, TYPEARC& /* type */)
{
try
{
  string sPathName = sPathArc ;

	if (sPathName[strlen(sPathName.c_str())-1] != '\\')
  	sPathName += string("\\") ;

  sPathName += sFichArc ;

  if (NsFileExists(sPathName))
  	return true ;

	return false ;
}
catch (...)
{
	erreur("Exception NSArcManager::ExisteFichierArchetype.", standardError) ;
	return true ;
}
}

string
NSArcManager::AjouteArchetype(NSContexte *pContexte, TYPEARC type, string sNomArc, string sFichArc, string sPathArc, string sType, string sCategory, string sLabel, string sFrame)
{
try
{
	string ps = string("-- Adding to the library - begin") ;
  _pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

  if (string("") == sNomArc)
  {
  	string ps = string("-- Bad parameters (empty ID) - exiting") ;
  	_pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
    return string("") ;
  }

  if (string("") == sFichArc)
  {
  	string ps = string("-- Bad parameters (empty File name) - exiting") ;
  	_pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
    return string("") ;
  }

	// On doit ici ajouter un nouvel archetype
	// En N_TIERS un archetype comprend identifiant et nom de fichier
	// (par convention tous les archetypes sont dans IXML. Ajout d'Url à voir + tard
	NSPatPathoArray PatPatho(_pSuper, graphObject) ;
	string sLexique ;
	string sObjectID = "" ;

	// Ajout d'un nouveau segment d'arbre
	if      (NSArcManager::archetype == type)
		sLexique = string("0ARCH1") ;
	else if (NSArcManager::referentiel == type)
		sLexique = string("0REFE1") ;
  else if (NSArcManager::decisionTree == type)
		sLexique = string("0ARDE1") ;
  else
  {
  	string ps = string("-- Bad parameters (type) - exiting") ;
  	_pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
  	return string("") ;
  }

	PatPatho.ajoutePatho(sLexique, 0) ;

	PatPatho.ajoutePatho("0ARID1", 1) ;
	Message Msg ;
	Msg.SetLexique("£?????") ;
	Msg.SetTexteLibre(sNomArc) ;
	PatPatho.ajoutePatho("£?????", &Msg, 2) ;

	PatPatho.ajoutePatho("0NFIC1", 1) ;
	Msg.Reset() ;
	Msg.SetLexique("£?????") ;
	Msg.SetTexteLibre(sFichArc) ;
	PatPatho.ajoutePatho("£?????", &Msg, 2) ;

  if (string("") != sLabel)
  {
  	// libellé
		PatPatho.ajoutePatho("LNOMA1", 1) ;
  	Msg.Reset() ;
    Msg.SetLexique("£?????") ;
		Msg.SetTexteLibre(sLabel) ;
		PatPatho.ajoutePatho("£?????", &Msg, 2) ;
  }

  if (string("") != sType)
  {
  	// libellé
		PatPatho.ajoutePatho("LTYPA1", 1) ;
    PatPatho.ajoutePatho(sType, 2) ;
  }

  if (string("") != sCategory)
  {
  	// libellé
		PatPatho.ajoutePatho("LDOMA1", 1) ;
    PatPatho.ajoutePatho(sCategory, 2) ;
  }

  if (string("") != sFrame)
  {
  	// libellé
		PatPatho.ajoutePatho("LFRME1", 1) ;
    PatPatho.ajoutePatho(sFrame, 2) ;
  }

	NSObjectGraphManager GraphObject(_pSuper) ;
	string sRootId = GraphObject.setTree(&PatPatho, "") ;
	GraphObject.setRootObject(sRootId) ;
	// Appel du pilote
	NSDataGraph* pGraph = GraphObject.getDataGraph() ;
	//pObectsList la liste d'objects qui correspondent a ces criteres
	NSPersonsAttributesArray ObjectsList ;
	string user = "_000000" ;
	if (pContexte && (pContexte->getUtilisateur()!= NULL))
		user = pContexte->getUtilisateurID() ;
	string sCodeSens ;
	NSDico::donneCodeSens(&sLexique, &sCodeSens) ;
	string sTraitName = string("_") + sCodeSens + "_" + string("0ARID") ;
	/* char szTraitName[12];
    strcpy(szTraitName,sTraitName.c_str()); */

	NSBasicAttributeArray AttrList ;
	AttrList.push_back(new NSBasicAttribute(sTraitName, sNomArc)) ;
	AttrList.push_back(new NSBasicAttribute("user",  user)) ;

  if (string("") != sType)
  {
  	string sTypeSens ;
		NSDico::donneCodeSens(&sType, &sTypeSens) ;
    string sTraitName = string("_") + sCodeSens + "_" + string("LTYPA") ;
    AttrList.push_back(new NSBasicAttribute(sTraitName, sTypeSens)) ;
  }
  // Ex : N0000 for treatment and 0OBJE for goal
  //
  if (string("") != sCategory)
  {
    string sCategorySens ;
		NSDico::donneCodeSens(&sCategory, &sCategorySens) ;
    string sTraitName = string("_") + sCodeSens + "_" + string("LDOMA") ;
    AttrList.push_back(new NSBasicAttribute(sTraitName, sCategorySens)) ;
  }
  if (string("") != sFrame)
  {
    string sFrameSens ;
		NSDico::donneCodeSens(&sFrame, &sFrameSens) ;
    string sTraitName = string("_") + sCodeSens + "_" + string("LFRME") ;
    AttrList.push_back(new NSBasicAttribute(sTraitName, sFrameSens)) ;
  }
  if (string("") != sLabel)
  {
    string sTraitName = string("_") + sCodeSens + "_" + string("LNOMA") ;
    AttrList.push_back(new NSBasicAttribute(sTraitName, sLabel)) ;
  }

	_pSuper->getPilot()->createPersonOrObject(NautilusPilot::SERV_CREATE_OBJECT.c_str(),
                        pGraph, &ObjectsList, &AttrList, OBJECT_TYPE, false) ;

	if (NULL == pGraph)
  {
  	string ps = string("-- The Pilot didn't create the new object - exiting") ;
  	_pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
		return "" ;
  }

	NSDataTreeIter iter ;
	if (pGraph->getTrees()->ExisteTree(sLexique, _pSuper, &iter))
	{
		sObjectID = (*iter)->getTreeID() ;
		_aModelArray.push_back(new NSDataTree(*(*iter))) ;

    ps = string("-- The Pilot has created the new object : ") + sObjectID ;
  	_pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
	}

  ps = string("-- Adding to the library - end") ;
  _pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

	return sObjectID ;
}
catch (...)
{
	erreur("Exception NSArcManager::AjouteArchetype.", standardError) ;
	return "" ;
}
}

void
NSArcManager::AjouteIdentifiantArchetype(TYPEARC /* type */, string /* sNomArc */, string /* sFichArc */)
{
try
{
	erreur("Fonction NSArcManager::AjouteIdentifiantArchetype non implémentée en mode N_TIERS", standardError) ;
}
catch (...)
{
	erreur("Exception NSArcManager::AjouteIdentifiantArchetype.", standardError) ;
	return ;
}
}

string
NSArcManager::DonneNoeudArchetype(TYPEARC type, string sNomArc)
{
try
{
  string sType, sTypeLex ;

	if      (NSArcManager::archetype == type)
	{
  	sTypeLex = string("0ARCH1") ;
    sType    = string("0ARCH") ;
  }
  else if (NSArcManager::referentiel == type)
  {
  	sTypeLex = string("0REFE1") ;
    sType    = string("0REFE") ;
  }
  else if (NSArcManager::decisionTree == type)
  {
  	sTypeLex = string("0ARDE1") ;
    sType    = string("0ARDE") ;
  }
  else
  {
  	erreur("Type inconnu dans ::DonneNoeudArchetype.", standardError) ;
    return string("") ;
  }

	// en N_TIERS on cherche d'abord en mémoire, sinon on fait un appel au pilote
	// on doit parcourir l'array des modèles
  //
	NSDataTreeIter iterTree ;
	if (false == _aModelArray.empty())
		for (iterTree = _aModelArray.begin() ; _aModelArray.end() != iterTree ; iterTree++)
		{
			if ((*iterTree)->getModelName() == sNomArc)
				return (*iterTree)->getTreeID() ;
		}

	// ------------ Appel du pilote
  //
	NSDataGraph     Graph(_pSuper, graphObject) ;
	NSPatPathoArray PatPathoArray(_pSuper, graphObject) ;

	string sTraitName = string("") ;
  if      (NSArcManager::archetype    == type)
		sTraitName = "_0ARCH_0ARID" ;
	else if (NSArcManager::referentiel  == type)
		sTraitName = "_0REFE_0ARID" ;
  else if (NSArcManager::decisionTree == type)
		sTraitName = "_0ARDE_0ARID" ;

	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrArray ;
	AttrArray.push_back(new NSBasicAttribute(sTraitName, sNomArc)) ;

	bool res = _pSuper->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT.c_str(),
                                    &Graph, &ObjList, &AttrArray);
	if (false == res)
	{
		string sErrorText = _pSuper->getText("NTIERS", "modelNotFound") ;
    sErrorText += string(" ") + sNomArc ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return string("") ;
	}

	if (false == Graph.getTrees()->ExisteTree(sTypeLex, _pSuper, &iterTree))
  	return string("") ;

	(*iterTree)->getRawPatPatho(&PatPathoArray) ;

  PatPathoIter iter    = PatPathoArray.begin() ;
  PatPathoIter iterEnd = PatPathoArray.end() ;

	if (iter == iterEnd)
  {
  	erreur("Le noeud archetype est incorrect dans ::DonneNoeudArchetype().", standardError) ;
		return string("") ;
	}

  string sSens = (*iter)->getLexiqueSens() ;

  if (sSens != sType)
  {
  	erreur("Le noeud archetype est mal positionné dans ::DonneNoeudArchetype().", standardError) ;
    return string("") ;
	}

  string sFichArc = string("") ;

	int iColArc = (*iter)->getColonne() ;
  iter++ ;

  // on charge les données de l'archetype
  while ((iter != iterEnd) && ((*iter)->getColonne() > iColArc))
  {
  	string sSens = (*iter)->getLexiqueSens() ;

    // nom du fichier (unique)
    if (string("0NFIC") == sSens)
    {
    	iter++ ;

      while ((iter != iterEnd) && ((*iter)->getColonne() > iColArc+1))
      {
      	// on cherche ici un texte libre
        string sElemLex = (*iter)->getLexique() ;

        if (sElemLex == string("£?????"))
        	sFichArc = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    else
    	iter++ ;
  }

	(*iterTree)->setModelName(sNomArc) ;
  (*iterTree)->setModelFileName(sFichArc) ;
  _aModelArray.push_back(new NSDataTree(*(*iterTree))) ;

  string rootTree = (*iterTree)->getTreeID() ;

	return rootTree ;
}
catch (...)
{
	erreur("Exception NSArcManager::DonneNoeudArchetype.", standardError) ;
	return "" ;
}
}

// -----------------------------------------------------------------------------
//
//  		Utility functions for Archetypes
//
// -----------------------------------------------------------------------------

string ArchetypeGetProperty(NSSuper *pSuper, NSDataTree *pTree, string sType, string sPropSens)
{
	if (((NSDataTree*) NULL == pTree) || ((NSSuper*) NULL == pSuper))
		return string("") ;

	NSPatPathoArray PatPathoArray(pSuper, graphObject) ;
  pTree->getRawPatPatho(&PatPathoArray) ;

  if (PatPathoArray.empty())
  	return string("") ;

  // Checking that the root is of the proper type
  //
	PatPathoIter iter = PatPathoArray.begin() ;
  string sSens = (*iter)->getLexiqueSens() ;

  if (sSens != sType)
  	return string("") ;

	int iColArc = (*iter)->getColonne() ;
  iter++ ;

  string sResult = string("") ;

  // on charge les données de l'archetype
  //
  while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColArc))
  {
  	sSens = (*iter)->getLexiqueSens() ;

    // nom du fichier (unique)
    if (sSens == sPropSens)
    {
    	iter++ ;

      while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColArc + 1))
      {
      	// on cherche ici un texte libre
        string sElemLex = (*iter)->getLexique() ;

        if (string("£?????") == sElemLex)
        	sResult = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    else
    	iter++ ;
  }

  return sResult ;
}

string ArchetypeGetID(NSSuper *pSuper, NSDataTree *pTree, string sType)
{
	if (((NSDataTree*) NULL == pTree) || ((NSSuper*) NULL == pSuper))
		return string("") ;

	return ArchetypeGetProperty(pSuper, pTree, sType, string("0ARID")) ;
}

string ArchetypeGetFile(NSSuper *pSuper, NSDataTree *pTree, string sType)
{
	if (((NSDataTree*) NULL == pTree) || ((NSSuper*) NULL == pSuper))
		return string("") ;

	return ArchetypeGetProperty(pSuper, pTree, sType, string("0NFIC")) ;
}

bool
ArchetypeSortByIDInf(NSDataTree *pTree1, NSDataTree *pTree2)
{
  if (((NSDataTree*) NULL == pTree1) || ((NSDataTree*) NULL == pTree2))
    return false ;

  string sT1 = pTree1->getModelName() ;
  pseumaj(&sT1) ;
  string sT2 = pTree2->getModelName() ;
  pseumaj(&sT2) ;

	return (strcmp(sT1.c_str(), sT2.c_str()) > 0) ;
}

bool
ArchetypeSortByIDSup(NSDataTree *pTree1, NSDataTree *pTree2)
{
  if (((NSDataTree*) NULL == pTree1) || ((NSDataTree*) NULL == pTree2))
    return false ;

  string sT1 = pTree1->getModelName() ;
  pseumaj(&sT1) ;
  string sT2 = pTree2->getModelName() ;
  pseumaj(&sT2) ;

	return (strcmp(sT1.c_str(), sT2.c_str()) < 0) ;
}

bool
ArchetypeSortByFileInf(NSDataTree *pTree1, NSDataTree *pTree2)
{
  if (((NSDataTree*) NULL == pTree1) || ((NSDataTree*) NULL == pTree2))
    return false ;

  string sT1 = pTree1->getModelFileName() ;
  pseumaj(&sT1) ;
  string sT2 = pTree2->getModelFileName() ;
  pseumaj(&sT2) ;

	return (strcmp(sT1.c_str(), sT2.c_str()) > 0) ;
}

bool
ArchetypeSortByFileSup(NSDataTree *pTree1, NSDataTree *pTree2)
{
  if (((NSDataTree*) NULL == pTree1) || ((NSDataTree*) NULL == pTree2))
    return false ;

  string sT1 = pTree1->getModelFileName() ;
  pseumaj(&sT1) ;
  string sT2 = pTree2->getModelFileName() ;
  pseumaj(&sT2) ;

	return (strcmp(sT1.c_str(), sT2.c_str()) < 0) ;
}

// fin de nsgraphe.cpp
/////////////////////////////////////////////////

