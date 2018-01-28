// nsagadlg.cpp : dialogues des AGA des fiches Fact// RS Sept 98
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <classlib\date.h>
#include <classlib\time.h>
#include <owl\eventhan.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
// #include "nautilus\nautilus.rh"
#include "nsdn\nsintrad.h"
#include "nscompta\nscompta.rh"
#include "nscompta\nsagadlg.h"
#include "nscompta\nsfsedlg.h"
#include "nsbb\tagNames.h"

//***************************************************************************
//  							Méthodes de NSAgaDocument
//***************************************************************************

// Constructeur NSAgaDocument
////////////////////////////////////////////////////////////////
NSAgaDocument::NSAgaDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
										NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
              :NSNoyauDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, true)
{
	// Initialisation des donnees
  pCriteres = new NSCriteres ;
  pTotaux		= new NSEncaissData ;
  pPartiels	= new NSEncaissData ;
	pAgaArray = new NSAgaArray ;
  nbAga 		= 0 ;
  pFseArray = new NSFse16Array ;
  nbPrest		= 0 ;
	pVar 		  = new NSVarCompta(pCtx) ;
  bImprimer = false ;
}

NSAgaDocument::NSAgaDocument(TDocument *parent, NSContexte *pCtx)
              :NSNoyauDocument(parent, pCtx)
{
	// Initialisation des donnees
	pCriteres = new NSCriteres ;
	pTotaux		= new NSEncaissData ;
	pPartiels	= new NSEncaissData ;
	pAgaArray = new NSAgaArray ;
	nbAga 		= 0 ;
	pFseArray = new NSFse16Array ;
	nbPrest		= 0 ;
	pVar 		  = new NSVarCompta(pCtx) ;
	bImprimer = false ;
}

NSAgaDocument::~NSAgaDocument(){
  delete pVar ;
  delete pFseArray ;
  delete pAgaArray ;
  delete pPartiels ;
  delete pTotaux ;
  delete pCriteres ;
}

// Ouverture du document////////////////////////////////////////////////////////////////
bool NSAgaDocument::Open(NSCriteres* pCritAga, bool bCumul)
{
	*pCriteres = *pCritAga ;
	bAgaCumules = bCumul ;
	if (!InitAgaArray())
		return false ;

	CalculeTotaux() ;

	return true ;
}

// Fermeture du document////////////////////////////////////////////////////////////////bool NSAgaDocument::Close()
{
	return true ;
}

/*
//
// 	InitFse1610Array : Charge les fiches 1610 dans NSFseArray
//
bool
NSAgaDocument::InitFse1610Array(string sNumCompt)
{
	string		sNumFse1610 = sNumCompt + string("    ") ;
	NSFse1610* 	pFse1610 = new NSFse1610(pContexte) ;

	pFse1610->lastError = pFse1610->open() ;
	if (pFse1610->lastError != DBIERR_NONE)	{
  	erreur("Erreur à l'ouverture de la base Fse1610.", standardError, pFse1610->lastError) ;
    delete pFse1610;    return false;
	}

    pFse1610->lastError = pFse1610->chercheClef(&sNumFse1610,
                                                "",
                                                0,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK);

    if (pFse1610->lastError == DBIERR_BOF)	// cas fichier vide
    {
   	    pFse1610->close();
        delete pFse1610;
        return true;		// le tableau est vide
    }

    if ((pFse1610->lastError != DBIERR_NONE) && (pFse1610->lastError != DBIERR_EOF))
    {
   	    erreur("Erreur à la recherche d'une fiche Fse1610.", standardError, pFse1610->lastError);
        pFse1610->close();
        delete pFse1610;
        return false;
    }

    while (pFse1610->lastError != DBIERR_EOF)
    {
   	    pFse1610->lastError = pFse1610->getRecord();
        if (pFse1610->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la lecture d'une fiche Fse1610.", standardError, pFse1610->lastError);
            pFse1610->close();
            delete pFse1610;
            return false;
        }

        // condition d'arret
        if (!(string(pFse1610->pDonnees->numcompt) == sNumCompt))
            break;

        // on remplit le tableau
        pFseArray->push_back(new NSBlocFse16(pFse1610));
        nbPrest++;

        // ... on passe au composant suivant
        pFse1610->lastError = pFse1610->suivant(dbiWRITELOCK);
        if ((pFse1610->lastError != DBIERR_NONE) && (pFse1610->lastError != DBIERR_EOF))
        {
      	    erreur("Erreur d'acces à une fiche Fse1610.", standardError, pFse1610->lastError);
            pFse1610->close();
            delete pFse1610;
            return false;
        }
    } // fin du while (recherche des composants images)

    // on ferme la base Fse1610
    pFse1610->lastError = pFse1610->close();
    if (pFse1610->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur de fermeture du fichier Fse1610.", standardError, pFse1610->lastError);
        delete pFse1610;
        return false;
    }

    delete pFse1610;
    return true;
}

//// 	InitFse1620Array : Charge les fiches 1620 dans NSFseArray
//
bool
NSAgaDocument::InitFse1620Array(string sNumCompt)
{
    string 		sNumFse1620 = sNumCompt + string("    ");
	NSFse1620* 	pFse1620 = new NSFse1620(pContexte);

    pFse1620->lastError = pFse1620->open();
    if (pFse1620->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base Fse1620.",standardError,0);
        delete pFse1620;
        return false;
    }

    pFse1620->lastError = pFse1620->chercheClef(&sNumFse1620,                                                "",                                                0,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK);

    if (pFse1620->lastError == DBIERR_BOF)	// cas fichier vide
    {
   	    pFse1620->close();
        delete pFse1620;
        return true;		// le tableau est vide
    }

    if ((pFse1620->lastError != DBIERR_NONE) && (pFse1620->lastError != DBIERR_EOF))
    {
   	    erreur("Erreur à la recherche d'une fiche Fse1620.", standardError, pFse1620->lastError);
        pFse1620->close();
        delete pFse1620;
        return false;
    }

    while (pFse1620->lastError != DBIERR_EOF)
    {   	    pFse1620->lastError = pFse1620->getRecord();
        if (pFse1620->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la lecture d'une fiche Fse1620.", standardError, pFse1620->lastError);
            pFse1620->close();
            delete pFse1620;
            return false;
        }

        // condition d'arret
        if (!(string(pFse1620->pDonnees->numcompt) == sNumCompt))
            break;

        // on remplit le tableau
        pFseArray->push_back(new NSBlocFse16(pFse1620));
        nbPrest++;

        // ... on passe au composant suivant
        pFse1620->lastError = pFse1620->suivant(dbiWRITELOCK);
        if ((pFse1620->lastError != DBIERR_NONE) && (pFse1620->lastError != DBIERR_EOF))
        {
      	    erreur("Erreur d'acces à une fiche Fse1620.", standardError, pFse1620->lastError);
            pFse1620->close();
            delete pFse1620;
            return false;
        }
    } // fin du while (recherche des composants images)

    // on ferme la base Fse1620
    pFse1620->lastError = pFse1620->close();
    if (pFse1620->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur de fermeture du fichier Fse1620.", standardError, pFse1620->lastError);
        delete pFse1620;
        return false;
    }

    delete pFse1620;
    return true;
}

//
// 	InitFse1630Array : Charge les fiches 1630 dans NSFseArray
//
bool
NSAgaDocument::InitFse1630Array(string sNumCompt)
{
    string 		sNumFse1630 = sNumCompt + string("    ");
	NSFse1630* 	pFse1630 = new NSFse1630(pContexte);

    pFse1630->lastError = pFse1630->open();
    if (pFse1630->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base Fse1630.", standardError, 0) ;
        delete pFse1630;
        return false;
    }

    pFse1630->lastError = pFse1630->chercheClef(&sNumFse1630,
                                                "",
                                                0,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK);

    if (pFse1630->lastError == DBIERR_BOF)	// cas fichier vide
    {
   	    pFse1630->close();
        delete pFse1630;
        return true;		// le tableau est vide
    }

    if ((pFse1630->lastError != DBIERR_NONE) && (pFse1630->lastError != DBIERR_EOF))
    {
   	    erreur("Erreur à la recherche d'une fiche Fse1630.", standardError, pFse1630->lastError);
        pFse1630->close();
        delete pFse1630;
        return false;
    }

    while (pFse1630->lastError != DBIERR_EOF)
    {
   	    pFse1630->lastError = pFse1630->getRecord();
        if (pFse1630->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la lecture d'une fiche Fse1630.", standardError, pFse1630->lastError);
            pFse1630->close();
            delete pFse1630;
            return false;
        }

        // condition d'arret
        if (!(string(pFse1630->pDonnees->numcompt) == sNumCompt)) break;

        // on remplit le tableau
        pFseArray->push_back(new NSBlocFse16(pFse1630));
        nbPrest++;

        // ... on passe au composant suivant
        pFse1630->lastError = pFse1630->suivant(dbiWRITELOCK);
        if ((pFse1630->lastError != DBIERR_NONE) && (pFse1630->lastError != DBIERR_EOF))
        {
      	    erreur("Erreur d'acces à une fiche Fse1630.", standardError, pFse1630->lastError);
            pFse1630->close();
            delete pFse1630;
            return false;
        }
    } // fin du while (recherche des composants images)

    // on ferme la base Fse1630
    pFse1630->lastError = pFse1630->close();
    if (pFse1630->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur de fermeture du fichier Fse1630.", standardError, pFse1630->lastError);
        delete pFse1630;
        return false;
    }

    delete pFse1630;
    return true;
}*/
boolNSAgaDocument::ChercheActes(string sNumCompt)
{
	if (NULL == pFseArray)
		return false ;

	pFseArray->vider() ;
	nbPrest = 0 ;

	NSComptInfo comptInfo ;
  strcpy(comptInfo._Donnees.numcompt, sNumCompt.c_str()) ;

  // on remplit le tableau
	if (false == comptInfo.InitFse1610Array(pFseArray, &nbPrest, pContexte))
		return false ;

	if (false == comptInfo.InitFse1620Array(pFseArray, &nbPrest, pContexte))		return false ;

	if (false == comptInfo.InitFse1630Array(pFseArray, &nbPrest, pContexte))		return false ;

	if (false == comptInfo.InitCCAMArray(pFseArray, &nbPrest, pContexte))
		return false ;

  if (false == comptInfo.InitLibreArray(pFseArray, &nbPrest, pContexte))
		return false ;
	if (pFseArray->empty())		return true ;

	// on trie par numéro de prestation
	sort(pFseArray->begin(), pFseArray->end(), blocInferieur) ;

	return true ;
}

voidNSAgaDocument::SommeActes(string& sActes)
{
	char prest[255] ;
	char cCoeff[255], cCode[255] ;
	int  divis, coeff ;
	bool bPremier = true ;

	sActes = string("") ;
	for (int i = 0 ; i < nbPrest ; i++)	{
    NSBlocFse16 *pBlocFse = (*pFseArray)[i] ;

		switch (pBlocFse->getTypePrest())
		{
			case NSBlocFse16::bloc1610 :

				strcpy(cCode, pBlocFse->get1610()->_Donnees.code) ;
				divis = atoi(pBlocFse->get1610()->_Donnees.divis) ;
				coeff = atoi(pBlocFse->get1610()->_Donnees.coeff) * divis ;

				if ((coeff % 100) == 0)					sprintf(cCoeff, "%d", coeff/100) ;
				else
					sprintf(cCoeff, "%d,%02d", coeff/100, coeff%100) ;

				if (1 == divis)					sprintf(prest, "%s %s", cCode, cCoeff) ;
				else
					sprintf(prest, "%s %s/%d", cCode, cCoeff, divis) ;

				break ;
			case NSBlocFse16::bloc1620 :
				sprintf(prest, "%s", pBlocFse->get1620()->_Donnees.code_ifd) ;        break ;

			case NSBlocFse16::bloc1630 :
      	sprintf(prest, "%s", pBlocFse->get1630()->_Donnees.code_ik) ;				break ;

      case NSBlocFse16::blocCcam :

      	strcpy(cCode, pBlocFse->getCcam()->_Donnees.code) ;
        if ('\0' != pBlocFse->getCcam()->_Donnees.pourcent[0])
        	coeff = atoi(pBlocFse->getCcam()->_Donnees.pourcent) ;
        else
      		coeff = 100 ;
        if (100 == coeff)        	sprintf(prest, "%s", cCode) ;        else        {        	sprintf(prest, "%s*%02d", cCode, coeff) ;          strcat(prest, "%") ;        }				break ;

      case NSBlocFse16::blocLibre :

      	strcpy(cCode, pBlocFse->getLibre()->_Donnees.code) ;
        if ('\0' != pBlocFse->getLibre()->_Donnees.pourcent[0])
        	coeff = atoi(pBlocFse->getLibre()->_Donnees.pourcent) ;
        else
      		coeff = 100 ;
        if (100 == coeff)        	sprintf(prest, "%s", cCode) ;        else        {        	sprintf(prest, "%s*%02d", cCode, coeff) ;          strcat(prest, "%") ;        }				break ;

      case NSBlocFse16::blocMat :

        break ;

			default:
				erreur("Type de prestation erronné dans la liste des prestations", standardError, 0) ;		}
		if (false == bPremier)    	sActes += string(" + ") ;
    else
			bPremier = false ;

		sActes += string(prest) ;	}
}

boolNSAgaDocument::ChercheNumSS(string sNumCompt, string& sNumSS)
{
	NSCompt* pCompt = new NSCompt(pContexte->getSuperviseur()) ;

	sNumSS = "" ;

	pCompt->lastError = pCompt->open();
	if (pCompt->lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ouverture de la base Compt.db", standardError, pCompt->lastError) ;
    delete pCompt ;
    return false ;
	}

	pCompt->lastError = pCompt->chercheClef(&sNumCompt,
                                            "",
                                            0,
                                            keySEARCHEQ,
                                            dbiWRITELOCK) ;

	if ((pCompt->lastError != DBIERR_NONE) && (pCompt->lastError != DBIERR_RECNOTFOUND))
	{
  	erreur("Erreur à la recherche dans la base Compt.db", standardError, pCompt->lastError) ;
    pCompt->close() ;
    delete pCompt ;
    return false ;
	}

	if (pCompt->lastError != DBIERR_RECNOTFOUND)
	{
  	pCompt->lastError = pCompt->getRecord();
    if (pCompt->lastError != DBIERR_NONE)
    {
    	erreur("Erreur à lecture dans la base Compt.db", standardError, pCompt->lastError) ;
      pCompt->close() ;
			delete pCompt ;
			return false ;    }

    // on fabrique le nom long et on le stocke dans la string
    sNumSS = pCompt->_Donnees.getPatientId() ;
  }
  else
  {
  	erreur("La fiche compt rattachée à votre paiement n'existe plus dans la base Compt.db", standardError, 0) ;
    pCompt->close() ;
    delete pCompt ;
    return false ;
	}

  pCompt->lastError = pCompt->close() ;
  if (pCompt->lastError != DBIERR_NONE)
  	erreur("Erreur à la fermeture de la base Compt.db", standardError, pCompt->lastError) ;

	delete pCompt ;
	return true ;
}

boolNSAgaDocument::ChercheLibelle(string sNumCompt, string& sLibelle){	NSRecettes* pRec = new NSRecettes(pContexte->getSuperviseur()) ;
	sLibelle = "" ;
	pRec->lastError = pRec->open() ;	if (pRec->lastError != DBIERR_NONE)	{  	erreur("Erreur à l'ouverture de la base Recettes.db", standardError, pRec->lastError) ;    delete pRec ;    return false ;	}
	pRec->lastError = pRec->chercheClef(&sNumCompt,                                            "",                                            0,                                            keySEARCHEQ,                                            dbiWRITELOCK) ;
	if ((pRec->lastError != DBIERR_NONE) && (pRec->lastError != DBIERR_RECNOTFOUND))	{  	erreur("Erreur à la recherche dans la base Recettes.db", standardError, pRec->lastError) ;    pRec->close() ;    delete pRec ;    return false ;	}
  if (pRec->lastError != DBIERR_RECNOTFOUND)  {  	pRec->lastError = pRec->getRecord() ;    if (pRec->lastError != DBIERR_NONE)    {    	erreur("Erreur à lecture dans la base Recettes.db", standardError, pRec->lastError) ;      pRec->close() ;			delete pRec ;			return false ;    }
    // on récupere le libelle    sLibelle = string(pRec->pDonnees->libelle) ;	}  else  {  	erreur("La fiche Recettes correspondante n'existe plus dans la base Recettes.db", standardError, 0) ;    pRec->close() ;    delete pRec ;    return false ;	}
  pRec->lastError = pRec->close() ;  if (pRec->lastError != DBIERR_NONE)  	erreur("Erreur à la fermeture de la base Recettes.db", standardError, pRec->lastError) ;
	delete pRec ;	return true ;}
bool
NSAgaDocument::ChercheEcriture(string sNumEcriture, NSEcritureArray* pEcritureArray)
{
	// on récupère la cle
	string      sCleEcriture =  sNumEcriture + string("  ") ;
	NSEcriture* pEcriture = new NSEcriture(pContexte->getSuperviseur()) ;

	pEcriture->lastError = pEcriture->open() ;	if (pEcriture->lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ouverture de la base Ecriture.", standardError, 0) ;
    delete pEcriture ;
    return false ;
	}

	pEcriture->lastError = pEcriture->chercheClef(&sCleEcriture,                                                "",
                                                0,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK) ;

	if (pEcriture->lastError == DBIERR_BOF)	// cas fichier vide	{
    pEcriture->close();
    delete pEcriture;
    return false;		// le tableau est vide
	}

	if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))	{
    erreur("Erreur à la recherche d'une fiche Ecriture.", standardError, pEcriture->lastError);
    pEcriture->close();
    delete pEcriture;
    return false;
	}

	while (pEcriture->lastError != DBIERR_EOF)	{
  	pEcriture->lastError = pEcriture->getRecord();
    if (pEcriture->lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture d'une fiche Ecriture.", standardError, pEcriture->lastError);
      pEcriture->close();
      delete pEcriture;
      return false;
  	}

    // condition d'arret    if (!(string(pEcriture->pDonnees->numero) == sNumEcriture))
    	break ;

    // on remplit le tableau    pEcritureArray->push_back(new NSEcritureInfo(pEcriture)) ;

    // ... on passe au suivant    pEcriture->lastError = pEcriture->suivant(dbiWRITELOCK);
    if ((pEcriture->lastError != DBIERR_NONE) && (pEcriture->lastError != DBIERR_EOF))
    {
      erreur("Erreur d'acces à une fiche Ecriture.", standardError, pEcriture->lastError);
      pEcriture->close();
      delete pEcriture;
      return false;
    }
	} // fin du while (recherche des écritures)

	// on ferme la base	pEcriture->lastError = pEcriture->close() ;
	if (pEcriture->lastError != DBIERR_NONE)
  	erreur("Erreur de fermeture du fichier Ecriture.", standardError, pEcriture->lastError) ;

	delete pEcriture ;
	return true ;
}

bool
NSAgaDocument::CherchePatient(string sNumSS, string& sNomPatient, NSPersonInfo& patInfo)
{
  NSPersonsAttributesArray AttsList ;

  NSBasicAttributeArray AttrArray ;
  AttrArray.push_back(new NSBasicAttribute(PERSON, sNumSS)) ;

  const char* serviceName = (NautilusPilot::SERV_PERSON_TRAITS_LIST).c_str() ;

	bool listOk = pContexte->getSuperviseur()->getPilot()->getUserProperties(serviceName, &AttsList, &AttrArray) ;
  if ((false == listOk) || AttsList.empty())
    return false ;

  NSPersonsAttributeIter iterPerson = AttsList.begin() ;

  NSBasicAttributeArray* pAttribute = *iterPerson ;
  if ((NULL == pAttribute) || pAttribute->empty())
    return false ;

  NSPersonInfo tempInfo(pContexte->getSuperviseur(), pAttribute) ;

// Previous method, that loads whole admin graph
/*
	NSPersonInfo tempInfo(pContexte, sNumSS, pidsPatient) ;
*/

	sNomPatient = string("") ;

	// on fabrique le nom long et on le stocke dans la string

	string sLang = pContexte->getUserLanguage() ;

	sNomPatient = tempInfo.getCivilite() ;  if (string("") == sNomPatient)    sNomPatient = tempInfo.getNomLong() ;
  patInfo = NSPersonInfo(tempInfo) ;	return true ;
}
boolNSAgaDocument::InitAgaArray()
{
	string		sOperateur = "" ;

	NSAgaData AgaData(pContexte) ;
	string		sNumSS = "", sNomLong = "" ;
	string    sLibelle = "" ;
	string		sActes ;

	string		      sNumEcriture ;
	NSEcritureArray EcritureArray ;

	NSPersonInfo patInfo(pContexte->getSuperviseur()) ;
	pAgaArray->vider() ;	nbAga = 0 ;

  NSFact Fact(pContexte->getSuperviseur()) ;

	Fact.lastError = Fact.open();
	if (Fact.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ouverture de la base FACT.", standardError, Fact.lastError) ;
    return false ;
	}

	// on cherche les fact par date  //  string sDate1 = pCriteres->getDateAga1() ;	Fact.lastError = Fact.chercheClef(&sDate1,
                                    "DATE_FACT",
                                    NODEFAULTINDEX,
                                    keySEARCHGEQ,
                                    dbiWRITELOCK) ;

	if (Fact.lastError == DBIERR_BOF)	// cas fichier vide
	{
  	Fact.close() ;
    return true ;		// le tableau est vide
  }

  if ((Fact.lastError != DBIERR_NONE) && (Fact.lastError != DBIERR_EOF))
  {
  	erreur("Erreur à la recherche d'une fiche fact.", standardError, Fact.lastError) ;
    Fact.close() ;
    return false ;
  }

  bool bCptsLibsOpen = false ;

  NScptsLibs cptsLibs(pContexte->getSuperviseur()) ;
	cptsLibs.lastError = cptsLibs.open() ;
  if (cptsLibs.lastError != DBIERR_NONE)
  	erreur("Erreur à l'ouverture de la base cptsLibs.", standardError, cptsLibs.lastError) ;
  else
  	bCptsLibsOpen = true ;

	if (pCriteres->isActesPerso())  	sOperateur = pContexte->getUtilisateur()->getNss() ;

	while (Fact.lastError != DBIERR_EOF)
	{
  	Fact.lastError = Fact.getRecord() ;
    if (Fact.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      if (bCptsLibsOpen)
      	cptsLibs.close() ;
      return false ;
    }

    // condition d'arret
    if (false == bAgaCumules)
    {
    	if (Fact.getPaidDate() > pCriteres->getDateAga1())      	break ;    }    else    {    	if (Fact.getPaidDate() > pCriteres->getDateAga2())      	break ;    }
    // on remplit le tableau (éventuellement on trie sur l'operateur)    if (false == pCriteres->isActesPerso())
    	sOperateur = Fact.getOperator() ;

    if (sOperateur == Fact.getOperator())
    {
    	if ((false == bAgaCumules) || (atoi(Fact._Donnees.montant) > 0))      {
      	AgaData.metAZero() ;        sActes = "" ;
        ChercheNumSS(Fact.getNumCompt(), sNumSS) ;
        if (string("") != sNumSS)
        {
        	CherchePatient(sNumSS, sNomLong, patInfo) ;          ChercheActes(Fact.getNumCompt()) ;          SommeActes(sActes) ;          strcpy(AgaData.nomPatient, sNomLong.c_str()) ;          AgaData.patInfo = patInfo ;        }        else        {        	EcritureArray.vider() ;          sNumEcriture = string("C") + Fact.getNumCompt() ;
          if (!ChercheEcriture(sNumEcriture, &EcritureArray))          	erreur("Impossible de récupérer les écritures correspondant à une dépense", standardError, 0);          else        	{          	for (NSEcritureIter i = EcritureArray.begin() ; EcritureArray.end() != i ; i++)            {
            	// on prend la première ligne d'écriture au débit
              // (en principe l'array ne contient que deux lignes d'écriture actuellement)
              if (!strcmp((*i)->pDonnees->sens, "C"))
              {
              	AgaData.ecriture = *((*i)->pDonnees) ;

                if (bCptsLibsOpen)
                {
                	string sCompte = (*i)->pDonnees->compte ;
                  cptsLibs.lastError = cptsLibs.chercheClef(&sCompte,
                                                            "",
                                                            0,
                                                            keySEARCHEQ,
                                                            dbiWRITELOCK) ;
                  if (cptsLibs.lastError == DBIERR_NONE)	// cas fichier vide
                  {
                  	cptsLibs.lastError = cptsLibs.getRecord() ;
                    if (cptsLibs.lastError != DBIERR_NONE)
                    	erreur("Erreur à la lecture d'une fiche cptsLibs.", standardError, cptsLibs.lastError) ;
                    else
                    	AgaData.cpt_lib = *(cptsLibs.pDonnees) ;
                  }
                }
              }
            }          }          ChercheLibelle(Fact.getNumCompt(), sLibelle) ;          strcpy(AgaData.libelle, sLibelle.c_str()) ;        }        strcpy(AgaData.numCompt, Fact._Donnees.numcompt) ;
        strcpy(AgaData.actes,    sActes.c_str()) ;
        strcpy(AgaData.montant,  Fact._Donnees.montant) ;
        strcpy(AgaData.unite,    Fact._Donnees.unite) ;
        strcpy(AgaData.modePaie, Fact._Donnees.mode_paie) ;

        if (bAgaCumules)
        {
        	string sLang = pContexte->getUserLanguage() ;
          string sDatePaie = donne_date(Fact.getPaidDate(), sLang) ;
          strcpy(AgaData.datePaie, sDatePaie.c_str()) ;
        }

        pAgaArray->push_back(new NSAgaData(AgaData));        nbAga++;      }    }

    // ... on passe au composant suivant    Fact.lastError = Fact.suivant(dbiWRITELOCK);
    if ((Fact.lastError != DBIERR_NONE) && (Fact.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      if (bCptsLibsOpen)
      	cptsLibs.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

	// on ferme la base CARTE_SV2
	Fact.lastError = Fact.close() ;
	if (Fact.lastError != DBIERR_NONE)
		erreur("Erreur de fermeture du fichier FACT.", standardError, Fact.lastError) ;

  if (bCptsLibsOpen)
		cptsLibs.close() ;

	return true ;
}

voidNSAgaDocument::CalculeTotaux()
{
  int montant;

  pTotaux->paieLoc = 0;   pPartiels->paieLoc = 0;
  pTotaux->paieEuro = 0;  pPartiels->paieEuro = 0;
  pTotaux->espLoc = 0;    pPartiels->espLoc = 0;
  pTotaux->espEuro = 0;   pPartiels->espEuro = 0;
  pTotaux->chqLoc = 0;    pPartiels->chqLoc = 0;
  pTotaux->chqEuro = 0;   pPartiels->chqEuro = 0;
  pTotaux->virLoc = 0;    pPartiels->virLoc = 0;
  pTotaux->virEuro = 0;   pPartiels->virEuro = 0;
  pTotaux->cbLoc = 0;     pPartiels->cbLoc = 0;
  pTotaux->cbEuro = 0;    pPartiels->cbEuro = 0;

	for (NSAgaIter i = pAgaArray->begin() ; pAgaArray->end() != i ; i++)
	{
  	montant = atoi((*i)->montant) ;

    if (!strcmp((*i)->modePaie, "E"))
    {
      if (!strcmp((*i)->unite, "LOC"))
      {
        pTotaux->espLoc    += montant ;
        pTotaux->espEuro   += pVar->getEuroFromLocal(montant) ;
        pPartiels->espLoc  += montant ;
      }
      else
      {
        pTotaux->espEuro   += montant ;
        pTotaux->espLoc    += pVar->getLocalFromEuro(montant) ;
        pPartiels->espEuro += montant ;
      }
    }
    else if (!strcmp((*i)->modePaie, "C"))
    {
    	if (!strcmp((*i)->unite, "LOC"))
      {
      	pTotaux->chqLoc     += montant;
        pTotaux->chqEuro    += pVar->getEuroFromLocal(montant) ;
        pPartiels->chqLoc   += montant;
      }
      else
      {
      	pTotaux->chqEuro    += montant;
        pTotaux->chqLoc     += pVar->getLocalFromEuro(montant) ;
        pPartiels->chqEuro  += montant;
      }
    }
    else if (!strcmp((*i)->modePaie, "V"))
    {
      if (!strcmp((*i)->unite, "LOC"))
      {
        pTotaux->virLoc     += montant;
        pTotaux->virEuro    += pVar->getEuroFromLocal(montant) ;
        pPartiels->virLoc   += montant;      }
      else
      {
        pTotaux->virEuro    += montant;
        pTotaux->virLoc     += pVar->getLocalFromEuro(montant) ;
        pPartiels->virEuro  += montant;
      }
    }
    else if (!strcmp((*i)->modePaie, "B"))
    {
    	if (!strcmp((*i)->unite, "LOC"))
      {
      	pTotaux->cbLoc      += montant;        pTotaux->cbEuro     += pVar->getEuroFromLocal(montant) ;
        pPartiels->cbLoc    += montant;
      }
      else
      {
      	pTotaux->cbEuro     += montant;
        pTotaux->cbLoc      += pVar->getLocalFromEuro(montant) ;
        pPartiels->cbEuro   += montant;
      }
    }

    if (!strcmp((*i)->unite, "LOC"))
    {
      pTotaux->paieLoc    += montant;
      pTotaux->paieEuro   += pVar->getEuroFromLocal(montant) ;
      pPartiels->paieLoc  += montant;
    }
    else
    {
      pTotaux->paieEuro   += montant;
      pTotaux->paieLoc    += pVar->getLocalFromEuro(montant) ;
      pPartiels->paieEuro += montant;    }
	}
}

//***************************************************************************//  							Méthodes de NSListAgaWindow
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSListAgaWindow, TListWindow)	EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Function: NSListAgaWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListAgaWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;
	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)		pDlg->CmModifAga() ;
}

//---------------------------------------------------------------------------
//  Function: NSListAgaWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListAgaWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0; i < count; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;
}

//***************************************************************************
//
//  Méthodes de NSListAgaDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSListAgaDialog, NSUtilDialog)
	EV_LV_DISPINFO_NOTIFY(IDC_AGA_LW, LVN_GETDISPINFO, DispInfoListeAga),
  EV_LV_DISPINFO_NOTIFY(IDC_AGA_LOC, LVN_GETDISPINFO, DispInfoListeLoc),
	EV_LV_DISPINFO_NOTIFY(IDC_AGA_EURO, LVN_GETDISPINFO, DispInfoListeEuro),
  EV_LV_DISPINFO_NOTIFY(IDC_AGA_GLOBAL, LVN_GETDISPINFO, DispInfoListeGlobal),
  EV_COMMAND(IDC_AGA_IMPRIMER, CmImprimer),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSListAgaDialog::NSListAgaDialog(TWindow* pere, NSContexte* pCtx, NSAgaDocument* pAgaDoc)
							 : NSUtilDialog(pere, pCtx, "IDD_AGA", pNSResModule)
{
	pDoc				   = pAgaDoc ;

	pTotauxArray	 = new NSTotauxArray ;
  nbTotaux			 = 0 ;
  pPartielsArray = new NSTotauxArray ;
  nbPartiels		 = 0 ;

  pListeLoc 		= new TListWindow(this, IDC_AGA_LOC) ;
  pListeEuro	 	= new TListWindow(this, IDC_AGA_EURO) ;
  pListeGlobal 	= new TListWindow(this, IDC_AGA_GLOBAL) ;
  pMonnaie			= new TStatic(this, IDC_AGA_MONNAIE) ;

  pListeAga 		= new NSListAgaWindow(this, IDC_AGA_LW) ;
  pVar          = new NSVarCompta(pCtx) ;
}

//
// Destructeur
//
NSListAgaDialog::~NSListAgaDialog()
{
	delete pTotauxArray ;
  delete pPartielsArray ;
  delete pListeLoc ;
  delete pListeEuro ;
  delete pListeGlobal ;
  delete pMonnaie ;
  delete pListeAga ;
  delete pVar ;
}

//// Fonction SetupWindow
//
void
NSListAgaDialog::SetupWindow()
{
	string sTitre ;
  string sDateAga1 = string("") ;
  string sDateAga2 = string("") ;
	NSUtilDialog::SetupWindow() ;

	string sLang = pContexte->getUserLanguage() ;

	sDateAga1 = donne_date(pDoc->pCriteres->getDateAga1(), sLang) ;  if (pDoc->bAgaCumules)
    sDateAga2 = donne_date(pDoc->pCriteres->getDateAga2(), sLang) ;

	// on récupère le titre du document et on l'affiche  if (pDoc->bAgaCumules)  {  	if (pDoc->pCriteres->getDateAga1() == pDoc->pCriteres->getDateAga2())    	sTitre = string("Liste des Recettes du ") + sDateAga1 ;    else    	sTitre = string("Liste des Recettes du ") + sDateAga1 + string(" au ") + sDateAga2 ;  }  else  	sTitre = string("Liste des AGA du ") + sDateAga1 ;
	SetCaption(sTitre.c_str()) ;
	pMonnaie->SetCaption((pVar->getSigle()).c_str()) ;
  InitListeAga() ;  AfficheListeAga() ;  InitTotauxArrays() ;
  InitListeLoc() ;  AfficheListeLoc() ;  InitListeEuro() ;  AfficheListeEuro() ;  InitListeGlobal() ;  AfficheListeGlobal() ;}
voidNSListAgaDialog::InitListeAga(){	TListWindColumn colNomPat("Nom - Prénom / Libelle", 300, TListWindColumn::Left, 0) ;
	pListeAga->InsertColumn(0, colNomPat) ;
	TListWindColumn colActes("Actes", 100, TListWindColumn::Left, 1) ;
	pListeAga->InsertColumn(1, colActes) ;
	TListWindColumn colMontant("Montant", 80, TListWindColumn::Right, 2) ;
	pListeAga->InsertColumn(2, colMontant) ;
	TListWindColumn colModePaie("Mode", 50, TListWindColumn::Left, 3) ;
	pListeAga->InsertColumn(3, colModePaie) ;
	if (pDoc->bAgaCumules)	{  	TListWindColumn colDatePaie("Date", 80, TListWindColumn::Left, 4) ;    pListeAga->InsertColumn(4, colDatePaie) ;	}}
voidNSListAgaDialog::AfficheListeAga()
{
	pListeAga->DeleteAllItems() ;

	for (int i = pDoc->nbAga - 1; i >= 0; i--)	{
  	if (strcmp(((*(pDoc->pAgaArray))[i])->nomPatient, "") != 0)
    {
    	TListWindItem Item(((*(pDoc->pAgaArray))[i])->nomPatient, 0) ;      pListeAga->InsertItem(Item) ;    }
    else
    {
    	TListWindItem Item(((*(pDoc->pAgaArray))[i])->libelle, 0) ;
      pListeAga->InsertItem(Item) ;
    }
	}}

voidNSListAgaDialog::DispInfoListeAga(TLwDispInfoNotify& dispInfo)
{
	const int      BufLen = 255 ;
	static char    buffer[BufLen] ;
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int            montant ;

	int index = dispInfoItem.GetIndex() ;
	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
   	case 1: // actes

     sprintf(buffer, "%s", ((*(pDoc->pAgaArray))[index])->actes);
     dispInfoItem.SetText(buffer);
     break;

   	case 2: // montant

    	montant = atoi(((*(pDoc->pAgaArray))[index])->montant) ;
      sprintf(buffer, "%d,%02d ", montant/100, montant%100) ;

      if (!strcmp(((*(pDoc->pAgaArray))[index])->unite, "LOC"))
      	strcat(buffer, (pVar->getSigle()).c_str()) ;
      else
      	strcat(buffer, "€") ;

      dispInfoItem.SetText(buffer) ;
      break ;

		case 3: 	// mode de paiement

    	sprintf(buffer, "%s", ((*(pDoc->pAgaArray))[index])->modePaie) ;
      dispInfoItem.SetText(buffer) ;
      break ;
		case 4: 	// date de paiement    	sprintf(buffer, "%s", ((*(pDoc->pAgaArray))[index])->datePaie) ;
      dispInfoItem.SetText(buffer) ;
      break ;

   } // fin du switch
}

voidNSListAgaDialog::InitTotauxArrays()
{
	NSTotauxData data ;

  pTotauxArray->vider() ;
  nbTotaux = 0 ;

  data.sLibelle = "Paiement" ;
  data.montantLoc = pDoc->pTotaux->paieLoc ;
  data.montantEuro = pDoc->pTotaux->paieEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle = "Espèces" ;
  data.montantLoc = pDoc->pTotaux->espLoc ;
  data.montantEuro = pDoc->pTotaux->espEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle = "Chèques" ;
  data.montantLoc = pDoc->pTotaux->chqLoc ;
  data.montantEuro = pDoc->pTotaux->chqEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle = "Virements" ;
  data.montantLoc = pDoc->pTotaux->virLoc ;
  data.montantEuro = pDoc->pTotaux->virEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle = "Carte bancaire" ;
  data.montantLoc = pDoc->pTotaux->cbLoc ;
  data.montantEuro = pDoc->pTotaux->cbEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  pPartielsArray->vider() ;
  nbPartiels = 0 ;

  data.sLibelle = "Paiement" ;
  data.montantLoc = pDoc->pPartiels->paieLoc ;
  data.montantEuro = pDoc->pPartiels->paieEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;

	data.sLibelle = "Espèces" ;
  data.montantLoc = pDoc->pPartiels->espLoc ;
  data.montantEuro = pDoc->pPartiels->espEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;

  data.sLibelle = "Chèques" ;
  data.montantLoc = pDoc->pPartiels->chqLoc ;
  data.montantEuro = pDoc->pPartiels->chqEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;

  data.sLibelle = "Virements" ;
  data.montantLoc = pDoc->pPartiels->virLoc ;
  data.montantEuro = pDoc->pPartiels->virEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;

  data.sLibelle = "Carte bancaire" ;
  data.montantLoc = pDoc->pPartiels->cbLoc ;
  data.montantEuro = pDoc->pPartiels->cbEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;
}

voidNSListAgaDialog::InitListeLoc()
{
	char cSigle[50] ;
	strcpy(cSigle, (pVar->getSigle()).c_str()) ;

	TListWindColumn colValeur("Valeur", 80, TListWindColumn::Left, 0) ;
	pListeLoc->InsertColumn(0, colValeur) ;
	TListWindColumn colLoc(cSigle, 80, TListWindColumn::Right, 1) ;
	pListeLoc->InsertColumn(1, colLoc) ;
}

void
NSListAgaDialog::AfficheListeLoc()
{
	char 	 cValeur[255] = "" ;
	string sValeur ;

	pListeLoc->DeleteAllItems() ;

	for (int i = nbPartiels - 1; i >= 0; i--)
	{
  	sValeur = ((*(pPartielsArray))[i])->sLibelle ;
    strcpy(cValeur, sValeur.c_str()) ;
   	TListWindItem Item(cValeur, 0) ;
    pListeLoc->InsertItem(Item) ;
	}
}

void
NSListAgaDialog::DispInfoListeLoc(TLwDispInfoNotify& dispInfo)
{
	const int      BufLen = 255 ;
	static char    buffer[BufLen] ;
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int            montant ;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // montant en monnaie locale

  		montant = ((*(pPartielsArray))[index])->montantLoc ;
      sprintf(buffer, "%d,%02d", montant/100, montant%100) ;
      dispInfoItem.SetText(buffer) ;
      break ;

	} // fin du switch
}

voidNSListAgaDialog::InitListeEuro()
{
	TListWindColumn colValeur("Valeur", 80, TListWindColumn::Left, 0) ;
	pListeEuro->InsertColumn(0, colValeur) ;
	TListWindColumn colEuro("Euros", 80, TListWindColumn::Right, 1) ;
	pListeEuro->InsertColumn(1, colEuro) ;
}

void
NSListAgaDialog::AfficheListeEuro()
{
	char 		cValeur[255] = "" ;
	string 	sValeur ;

	pListeEuro->DeleteAllItems() ;

	for (int i = nbPartiels - 1; i >= 0; i--)
	{
  	sValeur = ((*(pPartielsArray))[i])->sLibelle ;
    strcpy(cValeur, sValeur.c_str()) ;
   	TListWindItem Item(cValeur, 0) ;
    pListeEuro->InsertItem(Item) ;
	}
}

void
NSListAgaDialog::DispInfoListeEuro(TLwDispInfoNotify& dispInfo)
{
	const int 	   BufLen = 255 ;
	static char    buffer[BufLen] ;
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int            montant ;

  int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // montant en monnaie locale

    	montant = ((*(pPartielsArray))[index])->montantEuro ;
      sprintf(buffer, "%d,%02d", montant/100, montant%100) ;
      dispInfoItem.SetText(buffer) ;
      break ;

	} // fin du switch
}
voidNSListAgaDialog::InitListeGlobal(){
	char cSigle[50] ;
  strcpy(cSigle, (pVar->getSigle()).c_str()) ;

	TListWindColumn colValeur("Valeur", 80, TListWindColumn::Left, 0) ;
	pListeGlobal->InsertColumn(0, colValeur) ;
	TListWindColumn colEuro("Euros", 80, TListWindColumn::Right, 1) ;
	pListeGlobal->InsertColumn(1, colEuro) ;
	TListWindColumn colLoc(cSigle, 80, TListWindColumn::Right, 2) ;
	pListeGlobal->InsertColumn(2, colLoc) ;
}

void
NSListAgaDialog::AfficheListeGlobal()
{
	char 		cValeur[255] = "" ;
	string 	sValeur ;

	pListeGlobal->DeleteAllItems() ;

	for (int i = nbTotaux - 1; i >= 0; i--)	{
   	sValeur = ((*(pTotauxArray))[i])->sLibelle ;
    strcpy(cValeur, sValeur.c_str()) ;
   	TListWindItem Item(cValeur, 0) ;
    pListeGlobal->InsertItem(Item) ;
	}
}

voidNSListAgaDialog::DispInfoListeGlobal(TLwDispInfoNotify& dispInfo)
{
	const int 	   BufLen = 255 ;
  static char    buffer[BufLen] ;
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int            montant ;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // montant en monnaie locale

      montant = ((*(pTotauxArray))[index])->montantEuro;
      sprintf(buffer, "%d,%02d", montant/100, montant%100);      dispInfoItem.SetText(buffer);
      break;

    case 2: // montant en Euros
      montant = ((*(pTotauxArray))[index])->montantLoc;      sprintf(buffer, "%d,%02d", montant/100, montant%100);
      dispInfoItem.SetText(buffer);
      break;
	} // fin du switch
}

voidNSListAgaDialog::CmModifAga()
{
	int index = pListeAga->IndexItemSelect();

	if (index == -1)
	{
		erreur("Vous devez sélectionner une fiche AGA.", warningError, 0) ;
		return ;
	}

	string sNumCompt = string(((*(pDoc->pAgaArray))[index])->numCompt) ;
	NSCompt Compt(pContexte->getSuperviseur()) ;
	Compt.lastError = Compt.open() ;	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base Compt.", standardError, Compt.lastError) ;
    return ;
	}

	Compt.lastError = Compt.chercheClef(&sNumCompt,	                                    "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if (Compt.lastError != DBIERR_NONE)	{
		erreur("Erreur à la recherche d'une fiche compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    return ;
	}

	Compt.lastError = Compt.getRecord() ;	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à lecture du fichier Compt.db", standardError, Compt.lastError) ;
    Compt.close() ;
    return ;
	}

	// on lance la modif des infos récupérées	// si le nss est vide, il s'agit d'une fiche recette, sinon d'un acte pour un patient	if (strcmp(Compt._Donnees.patient, "") != 0)	{  	NSPersonInfo PatInfo((*(pDoc->pAgaArray))[index]->patInfo) ;    CreerFicheComptDialog ComptDlg(this, pContexte, &PatInfo, false, &(Compt._Donnees)) ;
    if ((ComptDlg.Execute()) == IDCANCEL)
    {
    	Compt.close() ;
      return ;
    }

    // on stocke les donnees du dialogue dans les Data    Compt._Donnees = *(ComptDlg.pData) ;

    Compt.lastError = Compt.modifyRecord(TRUE) ;    if (Compt.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la modification de la fiche compt.", standardError, Compt.lastError) ;
      Compt.close() ;
      return ;
    }

    Compt.lastError = Compt.close() ;    if (Compt.lastError != DBIERR_NONE)
    	erreur("Erreur à la fermeture de la base Compt.db.", standardError, Compt.lastError) ;

    // on enregistre les autres donnees sous le meme numcompt    ComptDlg.EnregDonneesCompt(sNumCompt) ;
  }
  else
  {
  	// on doit fermer Compt car le dialogue des recettes gère cette base de son cote
    Compt.lastError = Compt.close() ;
    if (Compt.lastError != DBIERR_NONE)
    	erreur("Erreur à la fermeture de la base Compt.db.", standardError, Compt.lastError) ;

    // Recherche de la fiche fact correspondante (en principe toujours facture '00')
    string sNumFact = sNumCompt + string(FACT_NUMERO_LEN, '0') ;

    NSFact Fact(pContexte->getSuperviseur()) ;
    Fact.lastError = Fact.open() ;

    if (Fact.lastError != DBIERR_NONE)    {
    	erreur("Erreur à l'ouverture de la base FACT.", standardError, Fact.lastError) ;      return ;
    }    Fact.lastError = Fact.chercheClef(&sNumFact,                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

    if ((Fact.lastError != DBIERR_NONE) && (Fact.lastError != DBIERR_RECNOTFOUND))    {    	erreur("Erreur à la recherche d'une fiche fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      return ;
    }

    if (Fact.lastError != DBIERR_RECNOTFOUND)    {
    	Fact.lastError = Fact.getRecord() ;

      if (Fact.lastError != DBIERR_NONE)      {
      	erreur("Erreur à la lecture d'une fiche fact.", standardError, Fact.lastError) ;
        Fact.close() ;
        return ;
      }
    }
    else
    {
    	erreur("La facture correspondant à votre recette est introuvable dans la base FACT.", standardError, 0) ;
      Fact.close();
      return ;
    }

    NSRecettesDlg RecDlg(this, pContexte) ;

    // on met les data à jour pour modification
    *(RecDlg.pData)     = Compt._Donnees ;
    *(RecDlg.pFactData) = Fact._Donnees ;
    strcpy(RecDlg.pRecettesListe->pDonnees->numcompt, Compt._Donnees.numcompt) ;
    strcpy(RecDlg.pRecettesListe->pDonnees->libelle, ((*(pDoc->pAgaArray))[index])->libelle) ;

    Fact.lastError = Fact.close() ;
    if (Fact.lastError != DBIERR_NONE)
    	erreur("Erreur à la fermeture de la base Fact.", standardError, Fact.lastError) ;

    if ((RecDlg.Execute()) == IDCANCEL)
      return ;
	}
	// on recharge le tableau depuis la base	// car on a pu modifier la fiche Compt.
	pDoc->InitAgaArray() ;
	pDoc->CalculeTotaux() ;

	AfficheListeAga() ;	InitTotauxArrays() ;
	AfficheListeLoc() ;
	AfficheListeEuro() ;
	AfficheListeGlobal() ;
}

voidNSListAgaDialog::CmImprimer()
{
	pDoc->bImprimer = true ;
	CmOk() ;
}

voidNSListAgaDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

voidNSListAgaDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

//***************************************************************************//
//  Méthodes de NSCritAgaDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSCritAgaDialog, NSUtilDialog)   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSCritAgaDialog::NSCritAgaDialog(TWindow* pere, NSContexte* pCtx)
							 : NSUtilDialog(pere, pCtx, "IDD_CRITERES", pNSResModule)
{
	// Initialisation des donnees
  pCriteres 	= new NSCriteres ;

  // Création de tous les "objets de contrôle"
  pDateAga1 	= new NSUtilEditDate(pContexte, this, IDC_CRIT_DATE1) ;
  pDateAga2   = new NSUtilEditDate(pContexte, this, IDC_CRIT_DATE2) ;
  pActes		  = new TGroupBox(this, IDC_CRIT_ACTES) ;
  pActesPerso	= new TRadioButton(this, IDC_CRIT_ACTESPERSO, pActes) ;
  pActesTous	= new TRadioButton(this, IDC_CRIT_ACTESTOUS, pActes) ;
}

//
// Destructeur
//
NSCritAgaDialog::~NSCritAgaDialog()
{
	delete pCriteres ;
  delete pDateAga1 ;
  delete pDateAga2 ;
  delete pActes ;
  delete pActesPerso ;
  delete pActesTous ;
}

//
// Fonction SetupWindow
//
void
NSCritAgaDialog::SetupWindow()
{
	// fichiers d'aide
  sHindex = "" ;  sHcorps = "Comptabilite.html" ;

	TDate			dateSys ;
	char			dateFact[9] = "" ;

	NSUtilDialog::SetupWindow() ;

  sprintf(dateFact, "%4d%02d%02d", (int)dateSys.Year(),
      				(int)dateSys.Month(), (int)dateSys.DayOfMonth()) ;

  pDateAga1->setDate(dateFact) ;
  pDateAga2->setDate(dateFact) ;

  pActesTous->Check() ;
}

void
NSCritAgaDialog::CmOk()
{
	string sDate ;

	pDateAga1->getDate(&sDate) ;
  if (string("") == sDate)
  {
  	erreur("Vous devez saisir une date de début.", standardError, 0) ;
    return ;
	}

	pCriteres->setDateAga1(sDate) ;

	pDateAga2->getDate(&sDate) ;
  if (string("") == sDate)
  {
  	erreur("Vous devez saisir une date de fin.", standardError, 0) ;
    return ;
	}

  pCriteres->setDateAga2(sDate) ;

	if      (pActesPerso->GetCheck() == BF_CHECKED)
		pCriteres->setActesPerso(true) ;
  else if (pActesTous->GetCheck()  == BF_CHECKED)
  	pCriteres->setActesPerso(false) ;
  else
  {
  	erreur("Vous devez saisir un choix (Vos actes / Tous les actes).", standardError, 0) ;
    return ;
	}

	TDialog::CmOk() ;
}

void
NSCritAgaDialog::CmCancel()
{
	TDialog::CmCancel();
}

//***************************************************************************
//
//  Méthodes de NSSaisieTPDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSSaisieTPDialog, NSUtilDialog)
  EV_COMMAND(IDC_STP_CHOIXTP, CmChoixTP),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSSaisieTPDialog::NSSaisieTPDialog(TWindow* pere, NSContexte* pCtx)
							 : NSUtilDialog(pere, pCtx, "IDD_SAISIETP", pNSResModule)
{
  // Création de tous les "objets de contrôle"
  pLibCourt = new NSUtilEdit2(pContexte, this, IDC_STP_LIBCOURT, ORGA_LIB_COURT_LEN) ;
  pLibLong  = new NSUtilEdit2(pContexte, this, IDC_STP_LIBLONG, ORGA_LIB_LONG_LEN) ;
  pAdresse  = new NSMultiEdit(pContexte, this, IDC_STP_ADRESSE, ORGA_ADRESSE_LEN) ;
}

//
// Destructeur
//
NSSaisieTPDialog::~NSSaisieTPDialog()
{
  delete pLibCourt ;
  delete pLibLong ;
  delete pAdresse ;
}

//
// Fonction SetupWindow
//
void
NSSaisieTPDialog::SetupWindow()
{
	// fichiers d'aide
  sHindex = "" ;  sHcorps = "Saisie_des_tiers_payants.html" ;
	NSUtilDialog::SetupWindow() ;

	pAdresse->FormatLines(true) ;
}

void
NSSaisieTPDialog::CmChoixTP()
{
}

void
NSSaisieTPDialog::CmOk()
{
	string sLibCourt, sLibLong, sAdresse ;
	int		 codeOrga ;

	pLibCourt->GetText(sLibCourt);
	if (sLibCourt == "")
	{
  	erreur("Vous devez saisir un libelle court", standardError, 0) ;
    return ;
	}

	pLibLong->GetText(sLibLong);
	if (sLibLong == "")
	{
  	erreur("Vous devez saisir un libelle long", standardError, 0) ;
    return ;
	}

	pAdresse->GetText(sAdresse);

	NSCodeOrga CodeOrga(pContexte->getSuperviseur()) ;

	CodeOrga.lastError = CodeOrga.open() ;
	if (CodeOrga.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ouverture de la base CodeOrga.", standardError, CodeOrga.lastError) ;
    return ;
	}

	CodeOrga.lastError = CodeOrga.fin(dbiWRITELOCK) ;
	if ((CodeOrga.lastError != DBIERR_NONE) && (CodeOrga.lastError != DBIERR_EOF))
	{
  	erreur("Erreur de positionnement dans le fichier CodeOrga.db.", standardError, CodeOrga.lastError) ;
    CodeOrga.close() ;
    return ;
	}

	if (CodeOrga.lastError == DBIERR_BOF) // la base est vide
  	strcpy(CodeOrga._Donnees.code, "00000000001") ;
	else
	{
		CodeOrga.lastError = CodeOrga.getRecord() ;
    if (CodeOrga.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche CodeOrga.", standardError, CodeOrga.lastError) ;
      CodeOrga.close() ;
      return ;
    }

    codeOrga = atoi(CodeOrga._Donnees.code) + 1 ;
    sprintf(CodeOrga._Donnees.code, "%011d", codeOrga) ;
	}

	strcpy(CodeOrga._Donnees.lib_court, sLibCourt.c_str()) ;
  strcpy(CodeOrga._Donnees.lib_long,  sLibLong.c_str()) ;
  strcpy(CodeOrga._Donnees.adresse,   sAdresse.c_str()) ;

	CodeOrga.lastError = CodeOrga.appendRecord() ;
	if (CodeOrga.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ajout d'une fiche CodeOrga.", standardError, CodeOrga.lastError) ;
    CodeOrga.close() ;
    return ;
	}

	// on ferme la base CODEORGA
	CodeOrga.lastError = CodeOrga.close() ;
  if (CodeOrga.lastError != DBIERR_NONE)
  {
  	erreur("Erreur de fermeture du fichier CodeOrga.", standardError, CodeOrga.lastError) ;
    return ;
	}

	TDialog::CmOk() ;
}

void
NSSaisieTPDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

// fin de nsagadlg.cpp
//////////////////////////////////////////////////////////

