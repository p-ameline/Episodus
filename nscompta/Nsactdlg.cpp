// nsimpdlg.cpp : dialogues des impayes// RS Octobre 98
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <classlib\date.h>
#include <classlib\time.h>
#include <owl\eventhan.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsadmiwd.h"
#include "partage\nsperson.h"
#include "nautilus\nspatdlg.h"	// pour le code prescript
#include "nscompta\nscompta.rh"
#include "nscompta\nsfsedlg.h"   // pour le tri des tableaux Fse
#include "nscompta\nsactdlg.h"
#include "nsbb\tagNames.h"

//***************************************************************************
//  							Méthodes de NSActDocument
//***************************************************************************

// Constructeur NSActDocument
////////////////////////////////////////////////////////////////
NSActDocument::NSActDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
										NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
              :NSNoyauDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, true)
{
	// Initialisation des donnees
  pCriteres 	   = new NSMultiCriteres ;
  pFseArray 	   = new NSFse16Array ;
  nbPrest			   = 0 ;
  pFactArray	   = new NSFactArray ;
  nbFact			   = 0 ;
  pTPArray		   = new NSTPArray ;
  nbTP				   = 0 ;
	pActArray 	   = new NSListActArray ;
  nbAct 			   = 0 ;
  pExamArray	   = new NSExamArray ;
  nbExam			   = 0 ;
  pKCodeArray	   = new NSKCodeArray ;
  nbKCode			   = 0 ;
  pCCAMCodeArray = new NSCCAMCodeArray ;
  nbCCAMCode	   = 0 ;
  pLibreCodeArray = new NSCCAMCodeArray ;
  nbLibreCode	   = 0 ;
  pTotauxArray   = new NSTotauxArray ;
  nbTotaux		   = 0 ;
	pVar 				   = new NSVarCompta(pCtx) ;
  bImprimer		   = false ;
}

NSActDocument::NSActDocument(TDocument *parent, NSContexte *pCtx)              :NSNoyauDocument(parent, pCtx)
{
	// Initialisation des donnees
  pCriteres 	   = new NSMultiCriteres ;
  pFseArray 	   = new NSFse16Array ;
  nbPrest			   = 0 ;
  pFactArray	   = new NSFactArray ;
  nbFact			   = 0 ;
  pTPArray		   = new NSTPArray ;
  nbTP				   = 0 ;
	pActArray 	   = new NSListActArray ;
  nbAct 			   = 0 ;
  pExamArray	   = new NSExamArray ;  nbExam			   = 0 ;
  pKCodeArray	   = new NSKCodeArray ;
  nbKCode			   = 0 ;
  pCCAMCodeArray = new NSCCAMCodeArray ;
  nbCCAMCode	   = 0 ;
  pLibreCodeArray = new NSCCAMCodeArray ;
  nbLibreCode	   = 0 ;
  pTotauxArray   = new NSTotauxArray ;
  nbTotaux		   = 0 ;
	pVar 				   = new NSVarCompta(pCtx) ;
  bImprimer		   = false ;
}

NSActDocument::~NSActDocument()
{
	delete pVar ;
  delete pTotauxArray ;
  delete pCCAMCodeArray ;
  delete pKCodeArray ;
  delete pExamArray ;
  delete pActArray ;
  delete pTPArray ;
  delete pFactArray ;
  delete pFseArray ;
  delete pCriteres ;
}

// Ouverture du document
////////////////////////////////////////////////////////////////
bool
NSActDocument::Open(int mode, LPCSTR /* path */)
{
	bool bAvecPat ;
	if (false == LanceCriteres())   	return false ;

	if (0 == mode)		bAvecPat = true ;  else  	bAvecPat = false ;	if (false == InitActArray(bAvecPat))
   	return false ;

	return true ;}

// Fermeture du document
////////////////////////////////////////////////////////////////
bool NSActDocument::Close()
{
	return true ;
}

bool
NSActDocument::LanceCriteres()
{
	NSMultiCriteresDialog* pCriteresDlg =
   	new NSMultiCriteresDialog(pContexte->GetMainWindow(), pContexte);

	if (pCriteresDlg->Execute() != IDOK)
	{
   	delete pCriteresDlg ;
    return false ;
  }
  *pCriteres = *(pCriteresDlg->pCriteres) ;

	delete pCriteresDlg ;
	return true ;
}

bool
NSActDocument::InitFactArray(string sNumCompt)
{
	// on récupère la cle
	string sNumFact =  sNumCompt + string("  ") ;

	NSFact Fact(pContexte->getSuperviseur()) ;

	pFactArray->vider() ;
  nbFact = 0 ;

  Fact.lastError = Fact.open() ;
  if (Fact.lastError != DBIERR_NONE)
  {
  	erreur("Erreur à l'ouverture de la base FACT.", standardError, Fact.lastError) ;
    return false ;
	}

	Fact.lastError = Fact.chercheClef(&sNumFact,
      														  "",
																    0,
																    keySEARCHGEQ,
																    dbiWRITELOCK) ;

	if (Fact.lastError == DBIERR_BOF)	// cas fichier vide
  {
  	Fact.close() ;
    return true ;		// le tableau est vide
  }

	if ((Fact.lastError != DBIERR_NONE) && (Fact.lastError != DBIERR_EOF))  {
  	erreur("Erreur à la recherche d'une fiche fact.", standardError, Fact.lastError) ;
    Fact.close() ;
    return false ;
	}

	while (Fact.lastError != DBIERR_EOF)
  {
  	Fact.lastError = Fact.getRecord() ;
    if (Fact.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      return false ;
    }

    // condition d'arret
    if (Fact._Donnees.getNumCompt() != sNumCompt)
    	break ;

    // on remplit le tableau
    pFactArray->push_back(new NSFactInfo(&Fact)) ;
    nbFact++ ;

    // ... on passe au composant suivant
    Fact.lastError = Fact.suivant(dbiWRITELOCK) ;
    if ((Fact.lastError != DBIERR_NONE) && (Fact.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      return false ;
    }
  } // fin du while

	// on ferme la base  Fact.lastError = Fact.close() ;
  if (Fact.lastError != DBIERR_NONE)
  {
  	erreur("Erreur de fermeture du fichier FACT.", standardError, Fact.lastError) ;
    return false ;
  }

	// on trie les fact par date	sort(pFactArray->begin(), pFactArray->end(), factAnterieure) ;

	return true ;
}

bool
NSActDocument::InitTPArray(string sNumCompt)
{
	// on récupère la cle
	string sNumTPayant = sNumCompt + string("  ") ;

	NSTPayant TPayant(pContexte->getSuperviseur()) ;

	pTPArray->vider() ;
  nbTP = 0 ;

	TPayant.lastError = TPayant.open() ;
  if (TPayant.lastError != DBIERR_NONE)
  {
  	erreur("Erreur à l'ouverture de la base TPayant.", standardError, TPayant.lastError) ;
    return false ;
  }

	TPayant.lastError = TPayant.chercheClef(&sNumTPayant,
      														        "",
                                          0,
																          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

	if (TPayant.lastError == DBIERR_BOF)	// cas fichier vide
  {
  	TPayant.close() ;
    return true ;		// le tableau est vide
  }

  if ((TPayant.lastError != DBIERR_NONE) && (TPayant.lastError != DBIERR_EOF))
  {
  	erreur("Erreur à la recherche d'une fiche TPayant.", standardError, TPayant.lastError);
    TPayant.close() ;
    return false ;
	}

  while (TPayant.lastError != DBIERR_EOF)
  {
  	TPayant.lastError = TPayant.getRecord();
    if (TPayant.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche TPayant.", standardError, TPayant.lastError) ;
      TPayant.close() ;
      return false ;
    }

    // condition d'arret
    if (string(TPayant._Donnees.numcompt) != sNumCompt)
    	break ;

    // on remplit le tableau
    pTPArray->push_back(new NSTPayantInfo(&TPayant)) ;
    nbTP++ ;

    // ... on passe au composant suivant
    TPayant.lastError = TPayant.suivant(dbiWRITELOCK);
    if ((TPayant.lastError != DBIERR_NONE) && (TPayant.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche TPayant.", standardError, TPayant.lastError) ;
      TPayant.close() ;
      return false ;
    }
  } // fin du while

  // on ferme la base
  TPayant.lastError = TPayant.close() ;
  if (TPayant.lastError != DBIERR_NONE)
  {
  	erreur("Erreur de fermeture du fichier TPayant.", standardError, TPayant.lastError);
    return false ;
	}

	return true ;
}

boolNSActDocument::CalculeTotaux(NSComptInfo* pComptInfo)
{
	if ((NSComptInfo*) NULL == pComptInfo)
		return false ;

	string sNumCompt = pComptInfo->_Donnees.getNumCompt() ;

	if (false == InitFactArray(sNumCompt))
  {
  	erreur("Erreur à l'initialisation du tableau des fiches Fact", standardError, 0) ;
    return false ;
  }

  if (false == InitTPArray(sNumCompt))
  {
  	erreur("Erreur à l'initialisation du tableau des fiches Tiers-Payant", standardError, 0) ;
    return false ;
  }

  int iDuLoc    = atoi(pComptInfo->_Donnees.duFranc) ;
  int iDuEuro   = atoi(pComptInfo->_Donnees.duEuro) ;
  int iPaidLoc  = atoi(pComptInfo->_Donnees.payeFranc) ;
  int iPaidEuro = atoi(pComptInfo->_Donnees.payeEuro) ;

	totaux._iTotalLoc  += iDuLoc ;
  totaux._iTotalEuro += iDuEuro ;
  totaux.depassLoc   += atoi(pComptInfo->_Donnees.depassFranc);
  totaux.depassEuro  += atoi(pComptInfo->_Donnees.depassEuro);
  totaux.paieLoc     += iPaidLoc ;
  totaux.paieEuro    += iPaidEuro ;
  totaux.impayeLoc   += iDuLoc  - iPaidLoc ;
  totaux.impayeEuro  += iDuEuro - iPaidEuro ;

  if (pComptInfo->isDepass())
  {
    totaux._iTotalAvecDepassLoc  += iDuLoc ;
		totaux._iTotalAvecDepassEuro += iDuEuro ;
  }

  if (false == pFactArray->empty())
	{
  	for (NSFactIter i = pFactArray->begin() ; pFactArray->end() != i ; i++)
		{
      int paieLoc, paieEuro ;

    	if (!strcmp((*i)->_Donnees.unite, "LOC"))
      {
      	paieLoc  = atoi((*i)->_Donnees.montant) ;
        paieEuro = pVar->getEuroFromLocal(paieLoc) ;
      }
      else
      {
      	paieEuro = atoi((*i)->_Donnees.montant) ;
        paieLoc  = pVar->getLocalFromEuro(paieEuro) ;
      }

      if (!strcmp((*i)->_Donnees.mode_paie, "E"))
      {
      	totaux.espLoc  += paieLoc ;
        totaux.espEuro += paieEuro ;
      }
      else if (!strcmp((*i)->_Donnees.mode_paie, "C"))
      {
      	totaux.chqLoc  += paieLoc ;
        totaux.chqEuro += paieEuro ;
      }
      else if (!strcmp((*i)->_Donnees.mode_paie, "V"))
      {
      	totaux.virLoc  += paieLoc ;
        totaux.virEuro += paieEuro ;
      }
      else if (!strcmp((*i)->_Donnees.mode_paie, "B"))
      {
      	totaux.cbLoc  += paieLoc ;
        totaux.cbEuro += paieEuro ;
      }
    }
	}

	int resteDuLoc  = 0 ;
  int resteDuEuro = 0 ;

	if (false == pTPArray->empty())
	{
    for (NSTPIter i = pTPArray->begin() ; pTPArray->end() != i ; i++)
    {
      int iResteDu = atoi((*i)->_Donnees.reste_du) ;

    	if (!strcmp((*i)->_Donnees.monnaie, "LOC"))
      {
      	resteDuLoc  += iResteDu ;
        resteDuEuro += pVar->getEuroFromLocal(iResteDu) ;
      }
      else
      {
      	resteDuEuro += iResteDu ;
        resteDuLoc  += pVar->getLocalFromEuro(iResteDu) ;
      }
    }
	}

  totaux.impTPLoc     += resteDuLoc ;
  totaux.impTPEuro    += resteDuEuro ;

  totaux.impAutreLoc  += iDuLoc  - iPaidLoc  - resteDuLoc ;
  totaux.impAutreEuro += iDuEuro - iPaidEuro - resteDuEuro ;

  totaux._iNbTotal++ ;

  if (pComptInfo->isDepass())
    totaux._iNbTotalAvecDepass++ ;

	return true ;
}

void
NSActDocument::InitTotauxArray()
{
	NSTotauxData data;

  data.sLibelle    = string("Total") ;
  data.montantLoc  = totaux._iTotalLoc ;
  data.montantEuro = totaux._iTotalEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Total avec dépassement") ;
  data.montantLoc  = totaux._iTotalAvecDepassLoc ;
  data.montantEuro = totaux._iTotalAvecDepassEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

	data.sLibelle    = string("Dépassement") ;
  data.montantLoc  = totaux.depassLoc ;
  data.montantEuro = totaux.depassEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Paiement") ;
  data.montantLoc  = totaux.paieLoc ;
  data.montantEuro = totaux.paieEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Espèces") ;
  data.montantLoc  = totaux.espLoc ;
  data.montantEuro = totaux.espEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Chèques") ;
  data.montantLoc  = totaux.chqLoc ;
  data.montantEuro = totaux.chqEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Virements") ;
  data.montantLoc  = totaux.virLoc ;
  data.montantEuro = totaux.virEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Carte bancaire") ;
  data.montantLoc  = totaux.cbLoc ;
  data.montantEuro = totaux.cbEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Impayés") ;
  data.montantLoc  = totaux.impayeLoc ;
  data.montantEuro = totaux.impayeEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Impayés Tiers-payant") ;
  data.montantLoc  = totaux.impTPLoc ;
  data.montantEuro = totaux.impTPEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Impayés Autres") ;
  data.montantLoc  = totaux.impAutreLoc ;
  data.montantEuro = totaux.impAutreEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;
}

boolNSActDocument::CherchePatient(string sNumSS, string& sNomPatient, NSPersonInfo& patInfo)
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
  patInfo = NSPersonInfo(tempInfo) ;
	return true ;}
bool
NSActDocument::ChercheActes(string sNumCompt)
{
	if ((NSFse16Array*) NULL == pFseArray)
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

void
NSActDocument::SommeActes(string& sActes)
{
	char prest[255] ;
	char cCode[255] ;
	char cCoeff[255] ;
	int  divis, coeff ;
	bool bPremier = true ;

	sActes = string("") ;

	for (int i = 0; i < nbPrest; i++)
	{
    NSBlocFse16 *pBlocFse = (*pFseArray)[i] ;

		switch (pBlocFse->getTypePrest())
		{
    	case NSBlocFse16::bloc1610 :
      {
      	NSFse1610Info* p1610 = pBlocFse->get1610() ;

        if ((NSFse1610Info*) NULL != p1610)
        {
      		strcpy(cCode, p1610->_Donnees.code) ;

        	divis = atoi(p1610->_Donnees.divis) ;
        	coeff = atoi(p1610->_Donnees.coeff) * divis ;

        	if ((coeff % 100) == 0)
        		sprintf(cCoeff, "%d", coeff/100) ;
        	else
        		sprintf(cCoeff, "%d,%02d", coeff/100, coeff%100) ;

        	if (divis == 1)
        		sprintf(prest, "%s %s", cCode, cCoeff) ;
        	else
        		sprintf(prest, "%s %s/%d", cCode, cCoeff, divis) ;
        }
        break ;
      }

      case NSBlocFse16::bloc1620 :
      	sprintf(prest, "%s", pBlocFse->get1620()->_Donnees.code_ifd) ;
        break ;

      case NSBlocFse16::bloc1630 :
      	sprintf(prest, "%s", pBlocFse->get1630()->_Donnees.code_ik) ;
				break ;

      case NSBlocFse16::blocCcam :
      {
        NSFseCCAMInfo* pCCAM = pBlocFse->getCcam() ;

        if ((NSFseCCAMInfo*) NULL != pCCAM)
        {
      		strcpy(prest, pCCAM->_Donnees.code) ;

          if (pCCAM->_Donnees.pourcent[0] != '\0')
          {
          	coeff = atoi(pCCAM->_Donnees.pourcent) ;

        		if ((coeff % 100) == 0)
            {
        			sprintf(cCoeff, "%d", coeff) ;
              strcat(prest, " ") ;
              strcat(prest, cCoeff) ;
              strcat(prest, "%") ;
            }
          }
        }
        break ;
      }

      case NSBlocFse16::blocLibre :
      {
        NSFseLibreInfo* pLibre = pBlocFse->getLibre() ;

        if ((NSFseLibreInfo*) NULL != pLibre)
        {
      		strcpy(prest, pLibre->_Donnees.code) ;

          if (pLibre->_Donnees.pourcent[0] != '\0')
          {
          	coeff = atoi(pLibre->_Donnees.pourcent) ;

        		if ((coeff % 100) == 0)
            {
        			sprintf(cCoeff, "%d", coeff) ;
              strcat(prest, " ") ;
              strcat(prest, cCoeff) ;
              strcat(prest, "%") ;
            }
          }
        }
        break ;
      }

      case NSBlocFse16::blocMat :
        break ;

      default:
      	erreur("Type de prestation erronné dans la liste des prestations", standardError, 0) ;
        return ;
    }

		if (false == bPremier)
    	sActes += string(" + ") ;
    else
    	bPremier = false ;

    sActes += string(prest) ;
	}
}

int
NSActDocument::EstDansExamArray(string sCode, NSExamArray* pArray)
{
	if (((NSExamArray*) NULL == pArray) || (pArray->empty()))
		return -1 ;

	NSExamIter i ;
	int 			 j ;

	for (i = pArray->begin(), j = 0 ; pArray->end() != i ; i++, j++)
		if ((*i)->sCodeExam == sCode)
    	return j ;

	return -1 ;
}

intNSActDocument::EstDansKCodeArray(string sKCode, NSKCodeArray* pArray)
{
	if ((!pArray) || (pArray->empty()))
		return -1 ;

	NSKCodeIter i ;
	int 			  j ;

	for (i = pArray->begin(), j = 0; i != pArray->end(); i++, j++)
  	if ((*i)->sKCode == sKCode)
    	return j ;

	return -1 ;
}

int
NSActDocument::EstDansCCAMCodeArray(string sCCAMCode, NSCCAMCodeArray* pArray)
{
	if (((NSCCAMCodeArray*) NULL == pArray) || (pArray->empty()))
		return -1 ;

	NSCCAMCodeIter i = pArray->begin() ;

	for (int j = 0 ; pArray->end() != i ; i++, j++)
  	if ((*i)->getCode() == sCCAMCode)
    	return j ;

	return -1 ;
}

voidNSActDocument::InitKCodes(NSComptInfo* pComptInfo)
{
	int 		j, k;
  double      coeff = 0.0, quantite = 0.0;
  string 		sCode, sCodeExam;
  NSKCodeData kcodeData;
  NSExamData  examData;

	sCodeExam = pComptInfo->_Donnees.getExam() + pComptInfo->_Donnees.getSynonym() ;

	j = EstDansExamArray(sCodeExam, pExamArray);

	if (j >= 0)
  	(*pExamArray)[j]->nbExam += 1;
	else
	{
  	examData.sCodeExam = sCodeExam;
    examData.nbExam = 1;

    pExamArray->push_back(new NSExamData(examData));
    nbExam++;
  }

  // on reprend j pour pouvoir insérer les KCodes
  //
	j = EstDansExamArray(sCodeExam, pExamArray) ;

	for (NSFse16Iter i = pFseArray->begin() ; pFseArray->end() != i ; i++)
  {
  	switch ((*i)->getTypePrest())
    {
    	case NSBlocFse16::bloc1610 :
      	sCode = string((*i)->get1610()->_Donnees.code) ;
        coeff = atof((*i)->get1610()->_Donnees.coeff) / 100 ;
        quantite = coeff * atoi((*i)->get1610()->_Donnees.quantite) ;
        break ;

      case NSBlocFse16::bloc1620 :
      	sCode = string((*i)->get1620()->_Donnees.code_ifd) ;
        quantite = atoi((*i)->get1620()->_Donnees.quantite) ;
        break ;

      case NSBlocFse16::bloc1630 :
      	sCode = string((*i)->get1630()->_Donnees.code_ik) ;
        quantite = atoi((*i)->get1630()->_Donnees.nbre_km) ;
        break ;

      case NSBlocFse16::blocCcam :
      	sCode = (*i)->getCcam()->getCode() ;
        break ;

      case NSBlocFse16::blocLibre :
      	sCode = (*i)->getLibre()->getCode() ;
        break ;

      case NSBlocFse16::blocMat :
        break ;
    }

    if (((*i)->getTypePrest() == NSBlocFse16::bloc1610) ||
        ((*i)->getTypePrest() == NSBlocFse16::bloc1620) ||
        ((*i)->getTypePrest() == NSBlocFse16::bloc1630))
    {
    	k = EstDansKCodeArray(sCode, &((*pExamArray)[j]->aKCodeArray)) ;

    	if (k >= 0)
    		((*pExamArray)[j]->aKCodeArray)[k]->occur += quantite ;
    	else
    	{
    		kcodeData.sKCode = sCode ;
      	kcodeData.occur = quantite ;

      	((*pExamArray)[j]->aKCodeArray).push_back(new NSKCodeData(kcodeData)) ;
    	}

    	k = EstDansKCodeArray(sCode, pKCodeArray) ;

    	if (k >= 0)
    		(*pKCodeArray)[k]->occur += quantite ;
    	else
    	{
    		kcodeData.sKCode = sCode ;
      	kcodeData.occur = quantite ;

      	pKCodeArray->push_back(new NSKCodeData(kcodeData)) ;
      	nbKCode++ ;
    	}
    }
    else if ((*i)->getTypePrest() == NSBlocFse16::blocCcam)
    {
    	sCode = string((*i)->getCcam()->_Donnees.code) ;

      int iPourcent = 100 ;
      if ((*i)->getCcam()->_Donnees.pourcent[0] != '\0')
      	iPourcent = atoi((*i)->getCcam()->_Donnees.pourcent) ;
      double dQuantity = double(iPourcent) / 100 ;

      int iSomme = 0 ;
      if ((*i)->getCcam()->_Donnees.montant_e[0] != '\0')
      	iSomme = atoi((*i)->getCcam()->_Donnees.montant_e) ;
      double dSomme = double(iSomme) / 100 ;

      k = EstDansKCodeArray(sCode, &((*pExamArray)[j]->aKCodeArray)) ;

    	if (k >= 0)
    		((*pExamArray)[j]->aKCodeArray)[k]->occur += dQuantity ;
    	else
    	{
    		kcodeData.sKCode = sCode ;
      	kcodeData.occur = dQuantity ;

      	((*pExamArray)[j]->aKCodeArray).push_back(new NSKCodeData(kcodeData)) ;
    	}

      k = EstDansCCAMCodeArray(sCode, pCCAMCodeArray) ;

      if (k >= 0)
      {
      	(*pCCAMCodeArray)[k]->addToNbre(dQuantity) ;
        (*pCCAMCodeArray)[k]->addToSum(dSomme) ;
      }
      else
    	{
      	NSCCAMCodeData* pCCAMCodeData = new NSCCAMCodeData ;

        pCCAMCodeData->setCode(sCode) ;
   			pCCAMCodeData->setLabel(string("")) ;
   			pCCAMCodeData->setNbre(dQuantity) ;
   			pCCAMCodeData->setSum(dSomme) ;

      	pCCAMCodeArray->push_back(pCCAMCodeData) ;
      	nbCCAMCode++ ;
    	}
    }
    else if ((*i)->getTypePrest() == NSBlocFse16::blocLibre)
    {
    	sCode = string((*i)->getLibre()->_Donnees.code) ;

      int iPourcent = 100 ;
      if ((*i)->getLibre()->_Donnees.pourcent[0] != '\0')
      	iPourcent = atoi((*i)->getLibre()->_Donnees.pourcent) ;
      double dQuantity = double(iPourcent) / 100 ;

      int iSomme = 0 ;
      if ((*i)->getLibre()->_Donnees.montant_e[0] != '\0')
      	iSomme = atoi((*i)->getLibre()->_Donnees.montant_e) ;
      double dSomme = double(iSomme) / 100 ;

      k = EstDansKCodeArray(sCode, &((*pExamArray)[j]->aKCodeArray)) ;

    	if (k >= 0)
    		((*pExamArray)[j]->aKCodeArray)[k]->occur += dQuantity ;
    	else
    	{
    		kcodeData.sKCode = sCode ;
      	kcodeData.occur = dQuantity ;

      	((*pExamArray)[j]->aKCodeArray).push_back(new NSKCodeData(kcodeData)) ;
    	}

      k = EstDansCCAMCodeArray(sCode, pLibreCodeArray) ;

      if (k >= 0)
      {
      	(*pLibreCodeArray)[k]->addToNbre(dQuantity) ;
        (*pLibreCodeArray)[k]->addToSum(dSomme) ;
      }
      else
    	{
      	NSCCAMCodeData* pCCAMCodeData = new NSCCAMCodeData ;

        pCCAMCodeData->setCode(sCode) ;
   			pCCAMCodeData->setLabel(string("")) ;
   			pCCAMCodeData->setNbre(dQuantity) ;
   			pCCAMCodeData->setSum(dSomme) ;

      	pLibreCodeArray->push_back(pCCAMCodeData) ;
      	nbLibreCode++ ;
    	}
    }
  }
}

boolNSActDocument::SelectionCriteres(NSCompt* pCompt)
{
  if ((NSCompt*) NULL == pCompt)
    return false ;

	if (pCriteres->bActesPerso)
		if (pCompt->getOperator() != pContexte->getUtilisateur()->getNss())
			return false ;

	if (string("") != pCriteres->sCodeExamen)
  	if (pCompt->getExam() != pCriteres->sCodeExamen)
    	return false ;

	if (string("") != pCriteres->sCodePrescript)
  	if (pCompt->getPrescriber() != pCriteres->sCodePrescript)
    	return false ;

	if ((1 == pCriteres->iImpayes) && (pCompt->isPayed()))		return false ;

	if ((2 == pCriteres->iImpayes) && (false == pCompt->isPayed()))
  	return false ;

	if ((1 == pCriteres->iContexte) && (string("") != pCompt->getContext()))
  	return false ;

	if ((2 == pCriteres->iContexte) && (string("NEXTE") != pCompt->getContext()))
  	return false ;

	if ((4 == pCriteres->iContexte) && (string("NHOST") != pCompt->getContext()))
  	return false ;

  if ((8 == pCriteres->iContexte) && (string("NAMBU") != pCompt->getContext()))
  	return false ;

	return true ;}
bool
NSActDocument::InitActArray(bool bAvecPatient)
{
	NSCompt       Compt(pContexte->getSuperviseur()) ;
  NSListActData ActData(pContexte) ;

  string			  sNumSS, sNomLong ;
  string 			  sCodeExam, sLibExam, sActes ;

  NSPersonInfo  patInfo(pContexte->getSuperviseur()) ;

  pActArray->vider() ;
  nbAct    = 0 ;

  Compt.lastError = Compt.open() ;
  if (Compt.lastError != DBIERR_NONE)
  {
  	erreur("Erreur à l'ouverture de la base Compt.", standardError, Compt.lastError) ;
    return false ;
  }

  // on cherche les Compt par date
  Compt.lastError = Compt.chercheClef(&(pCriteres->sDate1),
                                            "DATE_COMPT",
                                            NODEFAULTINDEX,
                                            keySEARCHGEQ,
                                            dbiWRITELOCK);

	if (Compt.lastError == DBIERR_BOF)	// cas fichier vide
  {
  	Compt.close() ;
    return true ;		// le tableau est vide
  }

  if ((Compt.lastError != DBIERR_NONE) && (Compt.lastError != DBIERR_EOF))
  {
  	erreur("Erreur à la recherche d'une fiche Compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    return false ;
  }

  string sLang = pContexte->getUserLanguage() ;

	CptaSearchProgressDialog progessDialog(pContexte->GetMainWindow(), pContexte) ;
	progessDialog.Create() ;
	progessDialog.Show(SW_SHOW) ;

  while (Compt.lastError != DBIERR_EOF)
  {
  	pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

  	if (progessDialog.isStopped())
    {
    	pCriteres->bInterruptedProcess = true ;
    	break ;
    }

  	Compt.lastError = Compt.getRecord() ;
    if (Compt.lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la lecture d'une fiche Compt.", standardError, Compt.lastError) ;
      Compt.close() ;
      return false ;
    }

    progessDialog.ClearText() ;
    progessDialog.SetNewDate(Compt._Donnees.getDate()) ;    progessDialog.SetNewCode(Compt._Donnees.getNumCompt()) ;    progessDialog.SetAck("Lecture OK.") ;

    // cas des fiches compt représentant des recettes et non des actes
    // dans ce cas le nss patient est vide et on passe au suivant    if (strcmp(Compt._Donnees.patient, "") != 0)    {    	// condition d'arret
      if (string(Compt._Donnees.date) > (pCriteres->sDate2))
      	break ;

      if (SelectionCriteres(&Compt))      {
        NSComptInfo ComptInfo(&Compt) ;

      	ActData.metAZero();

        if (bAvecPatient)
        {
        	if (false == CherchePatient(Compt._Donnees.getPatientId(), sNomLong, patInfo))          {
            // Compt.close() ;
            // return false ;
            string ps = string("Impossible de retrouver le patient ") + Compt._Donnees.getPatientId() + string(" rattaché à la fiche Compt ") + Compt._Donnees.getNumCompt() ;
            erreur(ps.c_str(), standardError, 0) ;
            pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
          }
        }

        if (false == ChercheActes(Compt._Donnees.getNumCompt()))        {
          string ps = string("Impossible de retrouver les actes rattachés à la fiche Compt ") + Compt._Donnees.getNumCompt() ;
          erreur(ps.c_str(), standardError, 0) ;
          pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
          // Compt.close() ;
          // return false ;
        }

        SommeActes(sActes) ;
        // incrémente les tableaux de K-codes        InitKCodes(&ComptInfo) ;

        // incrémente les totaux        CalculeTotaux(&ComptInfo) ;

        sCodeExam = Compt._Donnees.getExam() + Compt._Donnees.getSynonym() ;        pContexte->getDico()->donneLibelle(sLang, &sCodeExam, &sLibExam) ;

        // initialisation des données communes        strcpy(ActData.numCompt, Compt._Donnees.numcompt);
        strcpy(ActData.dateCompt, Compt._Donnees.date);
        if (bAvecPatient)
        	strcpy(ActData.nomPatient, sNomLong.c_str());
        strcpy(ActData.libExam, sLibExam.c_str());
        strcpy(ActData.actes, sActes.c_str());

        if (bAvecPatient)
        	ActData.patInfo = patInfo;
        pActArray->push_back(new NSListActData(ActData)) ;        nbAct++ ;
      }
    }

    // ... on passe à la fiche Compt suivante
    Compt.lastError = Compt.suivant(dbiWRITELOCK) ;
    if ((Compt.lastError != DBIERR_NONE) && (Compt.lastError != DBIERR_EOF))    {
    	erreur("Erreur d'acces à une fiche Compt.", standardError, Compt.lastError) ;
      Compt.close() ;
      return false ;
    }
  } // fin du while

  // on ferme la base COMPT
  Compt.lastError = Compt.close();
  if (Compt.lastError != DBIERR_NONE)
  	erreur("Erreur de fermeture du fichier Compt.", standardError, Compt.lastError) ;

	return true ;
}

//***************************************************************************//  							Méthodes de NSListActWindow
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSListActWindow, TListWindow)
   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

void
NSListActWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Function: NSListActWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListActWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
		pDlg->CmModifAct() ;
}

//---------------------------------------------------------------------------//  Function: NSListActWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListActWindow::IndexItemSelect()
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

//***************************************************************************//
//  Méthodes de NSListActDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSListActDialog, NSUtilDialog)	EV_LV_DISPINFO_NOTIFY(IDC_LA_LW, LVN_GETDISPINFO, DispInfoListeAct),
  EV_COMMAND(IDC_LA_IMPRIMER, CmImprimer),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//// Constructeur
//
NSListActDialog::NSListActDialog(TWindow* pere, NSContexte* pCtx, NSActDocument* pActDoc)
                :NSUtilDialog(pere, pCtx, "IDD_LISTACTES", pNSResModule)
{
	pDoc      = pActDoc ;

  pLibelle  = new TStatic(this, IDC_LA_LIBELLE) ;
	pListeAct = new NSListActWindow(this, IDC_LA_LW) ;
	pVar      = new NSVarCompta(pCtx) ;
}

//// Destructeur
//
NSListActDialog::~NSListActDialog()
{
	delete pLibelle ;
	delete pListeAct ;
	delete pVar ;
}

//// Fonction SetupWindow
//
void
NSListActDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	string sLibelle = pDoc->pCriteres->getSummaryString(pContexte) ;
  pLibelle->SetText(sLibelle.c_str()) ;

	InitListeAct() ;
	AfficheListeAct() ;
}

voidNSListActDialog::InitListeAct()
{
	TListWindColumn colDate("Date", 80, TListWindColumn::Left, 0) ;
	pListeAct->InsertColumn(0, colDate) ;
	TListWindColumn colNomPat("Nom - Prénom", 100, TListWindColumn::Left, 1) ;
	pListeAct->InsertColumn(1, colNomPat) ;
	TListWindColumn colCode("Code", 50, TListWindColumn::Left, 2) ;
	pListeAct->InsertColumn(2, colCode) ;
	TListWindColumn colExam("Examen", 160, TListWindColumn::Left, 3) ;
	pListeAct->InsertColumn(3, colExam) ;
	TListWindColumn colActes("Actes", 150, TListWindColumn::Left, 4) ;
	pListeAct->InsertColumn(4, colActes) ;
}

voidNSListActDialog::AfficheListeAct()
{
	pListeAct->DeleteAllItems() ;

	string sLang = pContexte->getUserLanguage() ;

	for (int i = pDoc->nbAct - 1; i >= 0; i--)
	{
   	string sDateExam = donne_date(((*(pDoc->pActArray))[i])->dateCompt, sLang) ;
   	TListWindItem Item(sDateExam.c_str(), 0) ;
    pListeAct->InsertItem(Item) ;
	}
}

voidNSListActDialog::DispInfoListeAct(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255 ;
	static char buffer[BufLen] ;
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;

	int index = dispInfoItem.GetIndex();

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
   	case 1: // nom long patient

    	sprintf(buffer, "%s", ((*(pDoc->pActArray))[index])->nomPatient) ;
      dispInfoItem.SetText(buffer) ;
      break ;

    case 2: // code nautilus patient

    	sprintf(buffer, "%s", ((*(pDoc->pActArray))[index])->patInfo.getNss().c_str()) ;
      dispInfoItem.SetText(buffer) ;      break ;

    case 3: // examen

    	sprintf(buffer, "%s", ((*(pDoc->pActArray))[index])->libExam) ;
      dispInfoItem.SetText(buffer) ;
      break ;

   	case 4: // actes

    	sprintf(buffer, "%s", ((*(pDoc->pActArray))[index])->actes) ;
      dispInfoItem.SetText(buffer) ;
      break ;

	} // fin du switch
}

voidNSListActDialog::CmModifAct()
{
	int index = pListeAct->IndexItemSelect() ;
	if (index == -1)
	{
  	erreur("Vous devez sélectionner une fiche Act.", warningError, 0) ;
    return ;
	}

	string sNumCompt = string(((*(pDoc->pActArray))[index])->numCompt) ;

  NSPersonInfo    PatInfo((*(pDoc->pActArray))[index]->patInfo) ;
  NSComptaPatient compta(pContexte, &PatInfo) ;

  if (false == compta.EditCompt(sNumCompt, this))
    return ;

/*
	NSCompt Compt(pContexte->getSuperviseur()) ;

	Compt.lastError = Compt.open() ;
	if (Compt.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ouverture de la base Compt.", standardError, Compt.lastError) ;
    return ;
	}

	Compt.lastError = Compt.chercheClef(&sNumCompt,
	                                    "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if (Compt.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à la recherche d'une fiche compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    return ;
	}

	Compt.lastError = Compt.getRecord() ;
	if (Compt.lastError != DBIERR_NONE)
	{
		erreur("Erreur à lecture du fichier Compt.db", standardError, Compt.lastError) ;
    Compt.close() ;
    return ;
	}

	// on lance la modif des infos récupérées
	NSPersonInfo PatInfo((*(pDoc->pActArray))[index]->patInfo) ;

	CreerFicheComptDialog* pComptDlg = new CreerFicheComptDialog(this, pContexte, &PatInfo, false) ;

	*(pComptDlg->pData) = *(Compt.pDonnees) ;

	if ((pComptDlg->Execute()) == IDCANCEL)
	{
  	Compt.close() ;
    delete pComptDlg ;
    return ;
	}

	// on stocke les donnees du dialogue dans les Data
	*(Compt.pDonnees) = *(pComptDlg->pData) ;

	Compt.lastError = Compt.modifyRecord(TRUE) ;
	if (Compt.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à la modification de la fiche compt.", standardError, Compt.lastError) ;
    Compt.close() ;
    delete pComptDlg ;
    return ;
	}

	Compt.lastError = Compt.close() ;
	if (Compt.lastError != DBIERR_NONE)
  	erreur("Erreur à la fermeture de la base Compt.db.", standardError, Compt.lastError) ;

	// on enregistre les autres donnees sous le meme numcompt
	pComptDlg->EnregDonneesCompt(sNumCompt) ;

	delete pComptDlg ;*/
	// on recharge le tableau depuis la base car on a pu modifier la fiche Compt.
	pDoc->InitActArray(true) ;
	AfficheListeAct() ;
}

voidNSListActDialog::CmImprimer()
{
	pDoc->bImprimer = true ;
	CmOk() ;
}

voidNSListActDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

voidNSListActDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

//***************************************************************************//
//  Méthodes de NSSomActDialog
//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSSomActDialog, NSUtilDialog)
	EV_LV_DISPINFO_NOTIFY(IDC_SOMACTES_EXAMENS, LVN_GETDISPINFO, DispInfoListeExam),
	EV_LV_DISPINFO_NOTIFY(IDC_SOMACTES_TOTAUX, LVN_GETDISPINFO, DispInfoListeTotaux),
	EV_LV_DISPINFO_NOTIFY(IDC_SOMACTES_KCODES, LVN_GETDISPINFO, DispInfoListeKCode),
  EV_COMMAND(IDC_SOMACTES_IMPRIMER, CmImprimer),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//// Constructeur
//
NSSomActDialog::NSSomActDialog(TWindow* pere, NSContexte* pCtx, NSActDocument* pActDoc)
							 :NSUtilDialog(pere, pCtx, "IDD_SOMACTES", pNSResModule)
{
	pDoc = pActDoc ;

  pLibelle      = new TStatic(this, IDC_SOMACTES_LIBELLE) ;
	pListeExam 		= new TListWindow(this, IDC_SOMACTES_EXAMENS) ;
	pListeTotaux 	= new TListWindow(this, IDC_SOMACTES_TOTAUX) ;
	pListeKCode 	= new TListWindow(this, IDC_SOMACTES_KCODES) ;
  pCounts       = new TStatic(this, IDC_TOTAL_COMPTE) ;
  pInfoDepass   = new TStatic(this, IDC_TOTAL_DEPASS) ;
	pVar          = new NSVarCompta(pCtx) ;
}

//// Destructeur
//
NSSomActDialog::~NSSomActDialog()
{
	delete pLibelle ;
	delete pListeExam ;
	delete pListeTotaux ;
	delete pListeKCode ;
  delete pCounts ;
  delete pInfoDepass ;
	delete pVar ;
}

//
// Fonction SetupWindow
//
void
NSSomActDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	pDoc->InitTotauxArray() ;

	InitListeExam() ;
	AfficheListeExam() ;
  InitListeTotaux() ;
  AfficheListeTotaux() ;
  InitListeKCode() ;
  AfficheListeKCode() ;

  string sLibelle = pDoc->pCriteres->getSummaryString(pContexte) ;
  pLibelle->SetText(sLibelle.c_str()) ;

  DisplayCounts() ;
  DisplayDepass() ;
}

void
NSSomActDialog::InitListeExam()
{
	TListWindColumn colExam("Examen", 120, TListWindColumn::Left, 0) ;
	pListeExam->InsertColumn(0, colExam) ;
	TListWindColumn colNombre("Nombre", 50, TListWindColumn::Left, 1) ;
  pListeExam->InsertColumn(1, colNombre) ;
  TListWindColumn colCode("Code", 250, TListWindColumn::Left, 2) ;
	pListeExam->InsertColumn(2, colCode) ;
}

void
NSSomActDialog::AfficheListeExam()
{
	char cLibExam[255] = "" ;
	string sCodeExam, sLibExam ;

	pListeExam->DeleteAllItems() ;

	string sLang = pContexte->getUserLanguage() ;

	for (int i = pDoc->nbExam - 1 ; i >= 0 ; i--)
	{
  	sCodeExam = ((*(pDoc->pExamArray))[i])->sCodeExam ;
    pContexte->getDico()->donneLibelle(sLang, &sCodeExam, &sLibExam) ;
    strcpy(cLibExam, sLibExam.c_str()) ;
    TListWindItem Item(cLibExam, 0) ;
    pListeExam->InsertItem(Item) ;
	}
}

voidNSSomActDialog::DispInfoListeExam(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 1024 ;
	static char buffer[BufLen] ;

	char			  cCodeExam[80] ;
	int			    occur ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

  NSExamData *pExamData = (*(pDoc->pExamArray))[index] ;

	switch (dispInfoItem.GetSubItem())
	{
  	case 1: // nombre d'examens

    	sprintf(buffer, "%d", pExamData->nbExam) ;
      break ;

    case 2: // codes liés aux examens

    	strcpy(buffer, "") ;

      for (NSKCodeIter i = pExamData->aKCodeArray.begin() ;
         					           pExamData->aKCodeArray.end() != i ;
                                                              i++)
      {
      	occur = (*i)->occur * 100 ;
        if ((occur%100) == 0)
					sprintf(cCodeExam, " %s(%d)", ((*i)->sKCode).c_str(), occur/100) ;
        else
        	sprintf(cCodeExam, " %s(%d,%02d)", ((*i)->sKCode).c_str(), occur/100, occur%100) ;

        strcat(buffer, cCodeExam) ;
      }

      break ;
   } // fin du switch

   dispInfoItem.SetText(buffer) ;
}

void
NSSomActDialog::InitListeTotaux()
{
	char cSigle[50] ;

	strcpy(cSigle, pVar->getSigle().c_str()) ;

	TListWindColumn colValeur("Valeur", 100, TListWindColumn::Left, 0) ;
	pListeTotaux->InsertColumn(0, colValeur) ;
	TListWindColumn colLoc(cSigle, 80, TListWindColumn::Right, 1) ;
	pListeTotaux->InsertColumn(1, colLoc) ;
	TListWindColumn colEuro("Euros", 80, TListWindColumn::Right, 2) ;
	pListeTotaux->InsertColumn(2, colEuro) ;
}

voidNSSomActDialog::AfficheListeTotaux()
{
	char 	 cValeur[255] = "" ;

	pListeTotaux->DeleteAllItems() ;

	for (int i = pDoc->nbTotaux - 1 ; i >= 0 ; i--)
	{
  	string sValeur = ((*(pDoc->pTotauxArray))[i])->sLibelle ;
    strcpy(cValeur, sValeur.c_str()) ;
   	TListWindItem Item(cValeur, 0) ;
    pListeTotaux->InsertItem(Item) ;
	}
}

void
NSSomActDialog::DispInfoListeTotaux(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255;
  static char buffer[BufLen];

  int			montant ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  // Affiche les informations en fonction de la colonne

  switch (dispInfoItem.GetSubItem())
  {
    case 1: // montant en monnaie locale

      montant = ((*(pDoc->pTotauxArray))[index])->montantLoc ;
      sprintf(buffer, "%d,%02d", montant/100, montant%100) ;
      break ;

    case 2: // montant en Euros

      montant = ((*(pDoc->pTotauxArray))[index])->montantEuro ;
      sprintf(buffer, "%d,%02d", montant/100, montant%100) ;
      break ;
  } // fin du switch

  dispInfoItem.SetText(buffer) ;
}

voidNSSomActDialog::InitListeKCode()
{
	TListWindColumn colTotal("Total", 80, TListWindColumn::Left, 0) ;
  pListeKCode->InsertColumn(0, colTotal) ;
	TListWindColumn colNombre("Nombre", 300, TListWindColumn::Left, 1) ;
  pListeKCode->InsertColumn(1, colNombre) ;
}

void
NSSomActDialog::AfficheListeKCode()
{
	char cTotal[255] = "" ;

	pListeKCode->DeleteAllItems() ;

  for (int i = pDoc->nbCCAMCode - 1 ; i >= 0 ; i--)
  {
  	string sKCode = ((*(pDoc->pCCAMCodeArray))[i])->getCode() ;
    sprintf(cTotal, "%s", sKCode.c_str()) ;
   	TListWindItem Item(cTotal, 0) ;
    pListeKCode->InsertItem(Item) ;
	}

  for (int i = pDoc->nbLibreCode - 1 ; i >= 0 ; i--)
  {
  	string sKCode = ((*(pDoc->pLibreCodeArray))[i])->getCode() ;
    sprintf(cTotal, "%s", sKCode.c_str()) ;
   	TListWindItem Item(cTotal, 0) ;
    pListeKCode->InsertItem(Item) ;
	}

	for (int i = pDoc->nbKCode - 1; i >= 0; i--)
  {
  	string sKCode = ((*(pDoc->pKCodeArray))[i])->sKCode ;
    sprintf(cTotal, "Nombre de %s", sKCode.c_str()) ;
   	TListWindItem Item(cTotal, 0) ;
    pListeKCode->InsertItem(Item) ;
	}
}

void
NSSomActDialog::DispInfoListeKCode(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255 ;
	static char buffer[BufLen] ;
  static char subBuffer[BufLen] ;

	int iKCodeSize = pDoc->pKCodeArray->size() ;
  int iCCAMSize  = pDoc->pCCAMCodeArray->size() ;
	int occur ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

  NSKCodeData    *pKCodeData = (NSKCodeData*) 0 ;
  NSCCAMCodeData *pCcamData  = (NSCCAMCodeData*) 0 ;

  if      (index < iKCodeSize)
    pKCodeData = (*(pDoc->pKCodeArray))[index] ;
  else if (index < iKCodeSize + iCCAMSize)
    pCcamData  = (*(pDoc->pCCAMCodeArray))[index-iKCodeSize] ;
  else
    pCcamData  = (*(pDoc->pLibreCodeArray))[index-iKCodeSize-iCCAMSize] ;

	switch (dispInfoItem.GetSubItem())
	{
		case 1: // nombre d'occurences du code

    	if (index < iKCodeSize)
      {
    		occur = pKCodeData->occur * 100 ;
    		if ((occur%100) == 0)
       		sprintf(buffer, "%d", occur/100) ;
    		else
      		sprintf(buffer, "%d,%02d", occur/100, occur%100) ;
      }
      else
      {
      	occur = pCcamData->getNbre() * 100 ;

        if ((occur%100) == 0)
       		sprintf(buffer, "%d", occur/100) ;
    		else
      		sprintf(buffer, "%d,%02d", occur/100, occur%100) ;

        strcat(buffer, " (") ;

   			occur = pCcamData->getSum() * 100 ;

        if ((occur%100) == 0)
       		sprintf(subBuffer, "%d", occur/100) ;
    		else
      		sprintf(subBuffer, "%d,%02d", occur/100, occur%100) ;

        strcat(buffer, subBuffer) ;
        strcat(buffer, " €)") ;
      }

      dispInfoItem.SetText(buffer) ;
      break ;
	} // fin du switch
}

void
NSSomActDialog::DisplayCounts()
{
  string sLabel = pDoc->totaux.GetCountsSentence() ;
  pCounts->SetText(sLabel.c_str()) ;
}

void
NSSomActDialog::DisplayDepass()
{
  string sLabel = pDoc->totaux.GetDepassSentence() ;
  pInfoDepass->SetText(sLabel.c_str()) ;
}

voidNSSomActDialog::CmImprimer(){
	pDoc->bImprimer = true ;
	CmOk() ;
}

void
NSSomActDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

void
NSSomActDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

//***************************************************************************
//  							Méthodes de NSEncaissDocument
//***************************************************************************

// Constructeur NSEncaissDocument////////////////////////////////////////////////////////////////
NSEncaissDocument::NSEncaissDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
										NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx)
                  :NSNoyauDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, true)
{
  // Initialisation des donnees
  pCriteres  = new NSMultiCriteres ;
  pTotaux    = new NSEncaissData ;
  pPartiels  = new NSEncaissData ;
  pFactArray = new NSFactArray ;
  nbFact     = 0 ;
	pVar       = new NSVarCompta(pCtx) ;
  bImprimer  = false ;
}

NSEncaissDocument::NSEncaissDocument(TDocument *parent, NSContexte *pCtx)
                  :NSNoyauDocument(parent, pCtx)
{
	// Initialisation des donnees
  pCriteres  = new NSMultiCriteres ;
  pTotaux    = new NSEncaissData ;
  pPartiels  = new NSEncaissData ;
  pFactArray = new NSFactArray ;
  nbFact     = 0 ;
  pVar       = new NSVarCompta(pCtx) ;
  bImprimer  = false ;
}

NSEncaissDocument::~NSEncaissDocument(){
  delete pVar ;
  delete pTotaux ;
  delete pPartiels ;
  delete pFactArray ;
  delete pCriteres ;
}

// Ouverture du document
////////////////////////////////////////////////////////////////
bool NSEncaissDocument::Open(int /* mode */, LPCSTR /* path */)
{
	if (false == LanceCriteres())
   	return false;

	if (false == InitFactArray())
   	return false;

  CalculeTotaux() ;

  return true ;
}

// Fermeture du document
////////////////////////////////////////////////////////////////
bool
NSEncaissDocument::Close()
{
  return true;
}

bool
NSEncaissDocument::LanceCriteres()
{
	NSFactCriteresDialog* pCriteresDlg =
   	      new NSFactCriteresDialog(pContexte->GetMainWindow(), pContexte) ;

  if (pCriteresDlg->Execute() != IDOK)
  {
    delete pCriteresDlg ;
    return false ;
  }

  *pCriteres = *(pCriteresDlg->pCriteres) ;

  delete pCriteresDlg ;
  return true ;
}

boolNSEncaissDocument::SelectionCriteres(NSFact* pFact)
{
  if ((NSFact*) NULL == pFact)
    return false ;

	if (pCriteres->bActesPerso)
		if (pFact->getOperator() != pContexte->getUtilisateur()->getNss())
			return false ;

	if (pCriteres->sCodeOrga == "#A") // tous + patients   	return true ;

	if (pCriteres->sCodeOrga == "#B") // tous - patients	{
  	if (string("") == pFact->getPayor())
    	return false ;
	}
  else if (pCriteres->sCodeOrga == "#C") // patients seulement
  {
  	if (string("") != pFact->getPayor())
    	return false ;
  }
	else
	{
  	if (pFact->getPayor() != pCriteres->sCodeOrga)
    	return false ;
	}

	return true ;}

boolNSEncaissDocument::InitFactArray()
{
  NSFact Fact(pContexte->getSuperviseur()) ;

  pFactArray->vider() ;
  nbFact = 0 ;

  Fact.lastError = Fact.open() ;
  if (DBIERR_NONE != Fact.lastError)
  {
    erreur("Erreur à l'ouverture de la base Fact.", standardError, Fact.lastError) ;
    return false ;
  }

  // on cherche les Fact par date
  Fact.lastError = Fact.chercheClef(&(pCriteres->sDate1),
      														  "DATE_FACT",
																    NODEFAULTINDEX,
																    keySEARCHGEQ,
																    dbiWRITELOCK) ;

  if (Fact.lastError == DBIERR_BOF)	// cas fichier vide
  {
    Fact.close() ;
    return true;		// le tableau est vide
  }

  if ((DBIERR_NONE != Fact.lastError) && (DBIERR_EOF != Fact.lastError))
  {
    erreur("Erreur à la recherche d'une fiche Fact.", standardError, Fact.lastError) ;
    Fact.close() ;
    return false ;
  }

  while (DBIERR_EOF != Fact.lastError)
  {
    Fact.lastError = Fact.getRecord() ;
    if (DBIERR_NONE != Fact.lastError)
    {
      erreur("Erreur à la lecture d'une fiche Fact.", standardError, Fact.lastError) ;
      Fact.close() ;      return false ;
    }

    // condition d'arret    if (Fact.getPaidDate() > (pCriteres->sDate2))
      break ;

    if (SelectionCriteres(&Fact))
    {
      pFactArray->push_back(new NSFactInfo(&Fact)) ;
      nbFact++ ;
    }

    // ... on passe à la fiche Fact suivante
    Fact.lastError = Fact.suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != Fact.lastError) && (DBIERR_EOF != Fact.lastError))
    {
      erreur("Erreur d'acces à une fiche Fact.", standardError, Fact.lastError) ;
      Fact.close() ;
      return false ;
    }
  } // fin du while

  // on ferme la base Fact
  Fact.lastError = Fact.close() ;
  if (DBIERR_NONE != Fact.lastError)
    erreur("Erreur de fermeture du fichier Fact.", standardError, Fact.lastError) ;

  return true ;
}

voidNSEncaissDocument::CalculeTotaux()
{
  int montant ;

	for (NSFactIter i = pFactArray->begin() ; pFactArray->end() != i ; i++)
  {
    montant = atoi((*i)->_Donnees.montant) ;

    if (!strcmp((*i)->_Donnees.mode_paie, "E"))
    {
      if (!strcmp((*i)->_Donnees.unite, "LOC"))
      {
        pTotaux->espLoc   += montant ;
        pTotaux->espEuro  += pVar->getEuroFromLocal(montant) ;
        pPartiels->espLoc += montant ;
      }
      else
      {
        pTotaux->espEuro   += montant ;
        pTotaux->espLoc    += pVar->getLocalFromEuro(montant) ;
        pPartiels->espEuro += montant ;
      }
    }
    else if (!strcmp((*i)->_Donnees.mode_paie, "C"))
    {
      if (!strcmp((*i)->_Donnees.unite, "LOC"))
      {
        pTotaux->chqLoc     += montant ;
        pTotaux->chqEuro    += pVar->getEuroFromLocal(montant) ;
        pPartiels->chqLoc   += montant ;
      }
      else
      {
        pTotaux->chqEuro    += montant ;
        pTotaux->chqLoc     += pVar->getLocalFromEuro(montant) ;
        pPartiels->chqEuro  += montant ;
      }
    }
    else if (!strcmp((*i)->_Donnees.mode_paie, "V"))
    {
      if (!strcmp((*i)->_Donnees.unite, "LOC"))
      {
        pTotaux->virLoc     += montant ;
        pTotaux->virEuro    += pVar->getEuroFromLocal(montant) ;
        pPartiels->virLoc   += montant ;
      }
      else
      {
        pTotaux->virEuro    += montant ;
        pTotaux->virLoc     += pVar->getLocalFromEuro(montant) ;
        pPartiels->virEuro  += montant ;
      }
    }
    else if (!strcmp((*i)->_Donnees.mode_paie, "B"))
    {
      if (!strcmp((*i)->_Donnees.unite, "LOC"))
      {
        pTotaux->cbLoc      += montant ;
        pTotaux->cbEuro     += pVar->getEuroFromLocal(montant) ;
        pPartiels->cbLoc    += montant ;
      }
      else
      {
        pTotaux->cbEuro     += montant ;
        pTotaux->cbLoc      += pVar->getLocalFromEuro(montant) ;
        pPartiels->cbEuro   += montant ;
      }
    }

    if (!strcmp((*i)->_Donnees.unite, "LOC"))
    {
      pTotaux->paieLoc    += montant ;
      pTotaux->paieEuro   += pVar->getEuroFromLocal(montant) ;
      pPartiels->paieLoc  += montant ;
    }
    else
    {
      pTotaux->paieEuro   += montant ;
      pTotaux->paieLoc    += pVar->getLocalFromEuro(montant) ;
      pPartiels->paieEuro += montant ;
    }
  }
}

//***************************************************************************
//
//  Méthodes de NSSomEncaissDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSSomEncaissDialog, NSUtilDialog)
  EV_LV_DISPINFO_NOTIFY(IDC_SE_LOC, LVN_GETDISPINFO, DispInfoListeLoc),
  EV_LV_DISPINFO_NOTIFY(IDC_SE_EURO, LVN_GETDISPINFO, DispInfoListeEuro),
  EV_LV_DISPINFO_NOTIFY(IDC_SE_GLOBAL, LVN_GETDISPINFO, DispInfoListeGlobal),
  EV_COMMAND(IDC_SE_IMPRIMER, CmImprimer),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSSomEncaissDialog::NSSomEncaissDialog(TWindow* pere, NSContexte* pCtx, NSEncaissDocument* pEncaissDoc)
							 : NSUtilDialog(pere, pCtx, "IDD_SOMENCAISS", pNSResModule)
{
	pDoc				   = pEncaissDoc ;

	pTotauxArray	 = new NSTotauxArray ;
	nbTotaux			 = 0 ;
	pPartielsArray = new NSTotauxArray ;
	nbPartiels		 = 0 ;

  pLibelle       = new TStatic(this, IDC_SE_LIBELLE) ;
	pListeLoc 		 = new TListWindow(this, IDC_SE_LOC) ;
	pListeEuro	 	 = new TListWindow(this, IDC_SE_EURO) ;
	pListeGlobal 	 = new TListWindow(this, IDC_SE_GLOBAL) ;
	pMonnaie			 = new TStatic(this, IDC_SE_MONNAIE) ;
	pVar           = new NSVarCompta(pCtx) ;
}

//// Destructeur
//
NSSomEncaissDialog::~NSSomEncaissDialog()
{
	delete pLibelle ;
	delete pTotauxArray ;
  delete pPartielsArray ;
  delete pListeLoc ;
  delete pListeEuro ;
  delete pListeGlobal ;
  delete pMonnaie ;
  delete pVar ;
}

//
// Fonction SetupWindow
//
void
NSSomEncaissDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	pMonnaie->SetCaption(pVar->getSigle().c_str()) ;

	InitTotauxArrays() ;

	InitListeLoc() ;
  AfficheListeLoc() ;
  InitListeEuro() ;
  AfficheListeEuro() ;
  InitListeGlobal() ;
  AfficheListeGlobal() ;

	string sLibelle = pDoc->pCriteres->getSummaryString(pContexte) ;
  pLibelle->SetText(sLibelle.c_str()) ;
}

void
NSSomEncaissDialog::InitTotauxArrays()
{
  NSTotauxData data ;

  data.sLibelle    = string("Paiement") ;
  data.montantLoc  = pDoc->pTotaux->paieLoc ;
  data.montantEuro = pDoc->pTotaux->paieEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Espèces") ;
  data.montantLoc  = pDoc->pTotaux->espLoc ;
  data.montantEuro = pDoc->pTotaux->espEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Chèques") ;
  data.montantLoc  = pDoc->pTotaux->chqLoc ;
  data.montantEuro = pDoc->pTotaux->chqEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Virements") ;
  data.montantLoc  = pDoc->pTotaux->virLoc ;
  data.montantEuro = pDoc->pTotaux->virEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Carte bancaire") ;
  data.montantLoc  = pDoc->pTotaux->cbLoc ;
  data.montantEuro = pDoc->pTotaux->cbEuro ;
  pTotauxArray->push_back(new NSTotauxData(data)) ;
  nbTotaux++ ;

  data.sLibelle    = string("Paiement") ;  data.montantLoc  = pDoc->pPartiels->paieLoc ;  data.montantEuro = pDoc->pPartiels->paieEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;

  data.sLibelle    = string("Espèces") ;
  data.montantLoc  = pDoc->pPartiels->espLoc ;
  data.montantEuro = pDoc->pPartiels->espEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;

  data.sLibelle    = string("Chèques") ;
  data.montantLoc  = pDoc->pPartiels->chqLoc ;
  data.montantEuro = pDoc->pPartiels->chqEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;

  data.sLibelle    = string("Virements") ;
  data.montantLoc  = pDoc->pPartiels->virLoc ;
  data.montantEuro = pDoc->pPartiels->virEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;

  data.sLibelle    = string("Carte bancaire") ;
  data.montantLoc  = pDoc->pPartiels->cbLoc ;
  data.montantEuro = pDoc->pPartiels->cbEuro ;
  pPartielsArray->push_back(new NSTotauxData(data)) ;
  nbPartiels++ ;
}

void
NSSomEncaissDialog::InitListeLoc()
{
	char cSigle[50] ;
  strcpy(cSigle, pVar->getSigle().c_str()) ;

  TListWindColumn colValeur("Valeur", 80, TListWindColumn::Left, 0) ;
  pListeLoc->InsertColumn(0, colValeur) ;
	TListWindColumn colLoc(cSigle, 80, TListWindColumn::Right, 1) ;
  pListeLoc->InsertColumn(1, colLoc) ;
}

void
NSSomEncaissDialog::AfficheListeLoc()
{
  pListeLoc->DeleteAllItems() ;

  for (int i = nbPartiels - 1; i >= 0; i--)
  {
    string sValeur = ((*(pPartielsArray))[i])->sLibelle ;
   	TListWindItem Item(sValeur.c_str(), 0) ;
    pListeLoc->InsertItem(Item) ;
  }
}

voidNSSomEncaissDialog::DispInfoListeLoc(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255;
   static char buffer[BufLen];
   TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;
   int 			index;
   int			montant;

   index = dispInfoItem.GetIndex();

   // Affiche les informations en fonction de la colonne

   switch (dispInfoItem.GetSubItem())
   {
   	case 1: // montant en monnaie locale

      	montant = ((*(pPartielsArray))[index])->montantLoc;
         sprintf(buffer, "%d,%02d", montant/100, montant%100);
         dispInfoItem.SetText(buffer);
         break;
   } // fin du switch
}

void
NSSomEncaissDialog::InitListeEuro()
{
	TListWindColumn colValeur("Valeur", 80, TListWindColumn::Left, 0) ;
  pListeEuro->InsertColumn(0, colValeur) ;
	TListWindColumn colEuro("Euros", 80, TListWindColumn::Right, 1) ;
  pListeEuro->InsertColumn(1, colEuro) ;
}

void
NSSomEncaissDialog::AfficheListeEuro()
{
  pListeEuro->DeleteAllItems() ;

	for (int i = nbPartiels - 1; i >= 0; i--)
  {
   	string sValeur = ((*(pPartielsArray))[i])->sLibelle ;
   	TListWindItem Item(sValeur.c_str(), 0) ;
    pListeEuro->InsertItem(Item) ;
  }
}

void
NSSomEncaissDialog::DispInfoListeEuro(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255 ;
  static char buffer[BufLen] ;
  int			    montant ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

   // Affiche les informations en fonction de la colonne

  switch (dispInfoItem.GetSubItem())
  {
    case 1: // montant en monnaie locale

      montant = ((*(pPartielsArray))[index])->montantEuro ;
      sprintf(buffer, "%d,%02d", montant / 100, montant % 100) ;
      dispInfoItem.SetText(buffer) ;
      break ;
  } // fin du switch
}

void
NSSomEncaissDialog::InitListeGlobal()
{
  char cSigle[50];
  strcpy(cSigle, pVar->getSigle().c_str()) ;

	TListWindColumn colValeur("Valeur", 80, TListWindColumn::Left, 0) ;
  pListeGlobal->InsertColumn(0, colValeur) ;
	TListWindColumn colLoc(cSigle, 80, TListWindColumn::Right, 1) ;
  pListeGlobal->InsertColumn(1, colLoc) ;
  TListWindColumn colEuro("Euros", 80, TListWindColumn::Right, 2) ;
  pListeGlobal->InsertColumn(2, colEuro) ;
}

void
NSSomEncaissDialog::AfficheListeGlobal()
{
  pListeGlobal->DeleteAllItems() ;

  for (int i = nbTotaux - 1 ; i >= 0 ; i--)
  {
    string sValeur = ((*(pTotauxArray))[i])->sLibelle ;
    TListWindItem Item(sValeur.c_str(), 0) ;
    pListeGlobal->InsertItem(Item) ;
  }
}

void
NSSomEncaissDialog::DispInfoListeGlobal(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255;
  static char buffer[BufLen];

  int			montant;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  // Affiche les informations en fonction de la colonne

  switch (dispInfoItem.GetSubItem())
  {
   	case 1: // montant en monnaie locale

      montant = ((*(pTotauxArray))[index])->montantLoc ;
      sprintf(buffer, "%d,%02d", montant/100, montant%100) ;
      break ;

    case 2: // montant en Euros

      montant = ((*(pTotauxArray))[index])->montantEuro ;
      sprintf(buffer, "%d,%02d", montant/100, montant%100) ;
      break ;
  } // fin du switch

  dispInfoItem.SetText(buffer) ;
}

void
NSSomEncaissDialog::CmImprimer()
{
  pDoc->bImprimer = true ;
  CmOk() ;
}

void
NSSomEncaissDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

voidNSSomEncaissDialog::CmCancel()
{
	NSUtilDialog::CmCancel();
}

//***************************************************************************
//
//  Méthodes de NSMultiCriteresDialog
//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSMultiCriteresDialog, NSUtilDialog)
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
   EV_BN_CLICKED(IDC_MC_PRESCRIPT, CmPrescript),
END_RESPONSE_TABLE;

//// Constructeur
//
NSMultiCriteresDialog::NSMultiCriteresDialog(TWindow* pere, NSContexte* pCtx)
							 : NSUtilDialog(pere, pCtx, "IDD_MULTICRITERES", pNSResModule)
{
	// Initialisation des donnees
	pCriteres 	= new NSMultiCriteres ;

	// Création de tous les "objets de contrôle"
	pDate1 	 		= new NSUtilEditDate(pContexte, this, IDC_MC_DATE1) ;
	pDate2			= new NSUtilEditDate(pContexte, this, IDC_MC_DATE2) ;
	pExamen			= new NSUtilLexique(pContexte, this, IDC_MC_EXAMEN, pCtx->getDico()) ;
	pPrescript	= new TStatic(this, IDC_MC_PRESCRIPT) ;
	pActes			= new TGroupBox(this, IDC_MC_ACTES) ;
	pActesPerso	= new TRadioButton(this, IDC_MC_ACTES_PERSO, pActes) ;
	pActesTous	= new TRadioButton(this, IDC_MC_ACTES_TOUS, pActes) ;
	pImp				= new TGroupBox(this, IDC_MC_IMP) ;
	pImpTous		= new TRadioButton(this, IDC_MC_IMP_TOUS, pImp) ;
	pImpImpayes	= new TRadioButton(this, IDC_MC_IMP_IMPAYES, pImp) ;
	pImpPayes		= new TRadioButton(this, IDC_MC_IMP_PAYES, pImp) ;
	pCtxt				= new TGroupBox(this, IDC_MC_CTXT) ;
	pCtxtTous		= new TRadioButton(this, IDC_MC_CTXT_TOUS, pCtxt) ;
	pCtxtNP		  = new TRadioButton(this, IDC_MC_CTXT_NP,   pCtxt) ;
  pCtxtExt		= new TRadioButton(this, IDC_MC_CTXT_EXT,  pCtxt) ;
  pCtxtAmbu		= new TRadioButton(this, IDC_MC_CTXT_AMBU, pCtxt) ;
	pCtxtHosp		= new TRadioButton(this, IDC_MC_CTXT_HOSP, pCtxt) ;
}

//// Destructeur
//
NSMultiCriteresDialog::~NSMultiCriteresDialog()
{
	delete pCriteres ;
	delete pDate1 ;
	delete pDate2 ;
	delete pExamen ;
	delete pPrescript ;
	delete pActes ;
	delete pActesPerso ;
	delete pActesTous ;
	delete pImp ;
	delete pImpTous ;
	delete pImpImpayes ;
	delete pImpPayes ;
	delete pCtxt ;
	delete pCtxtTous ;
	delete pCtxtNP ;
  delete pCtxtExt ;
  delete pCtxtAmbu ;
	delete pCtxtHosp ;
}

//// Fonction SetupWindow
//
void
NSMultiCriteresDialog::SetupWindow()
{
	// fichiers d'aide
	sHindex = "" ;	sHcorps = "Comptabilite.html" ;
	TDate	dateSys ;
	char	dateJour[9] = "" ;

	NSUtilDialog::SetupWindow() ;

	sprintf(dateJour, "%4d%02d%02d", (int)dateSys.Year(),
      				(int)dateSys.Month(), (int)dateSys.DayOfMonth()) ;

	pDate1->setDate(dateJour) ;
	pDate2->setDate(dateJour) ;

	pActesTous->Check() ;
	pImpTous->Check() ;
	pCtxtTous->Check() ;
}

voidNSMultiCriteresDialog::CmPrescript()
{
try
{
	string sTitre ;

  NSTPersonListDialog indep((TWindow *)this, pidsCorresp, false, pContexte, 0, true) ;
  int	iDialogReturn = indep.Execute() ;
  if (iDialogReturn != IDOK)
  	return ;

	NSPersonInfo *temp = indep.pPersonSelect ;
  if (string("") == temp->getNss())
		return ;

	string sPids = temp->getNss() ;
	pCriteres->sCodePrescript = sPids ;

	NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPids, pidsCorresp) ;
  if (NULL == pPersonInfo)
  	return ;

	sTitre = pPersonInfo->getNom() + " " + pPersonInfo->getPrenom() ;

  pPrescript->SetCaption(sTitre.c_str()) ;
	pCriteres->sTitrePrescript = sTitre ;

/*
	ChercheListeCorDialog* pListeCorDlg ;
	pListeCorDlg = new ChercheListeCorDialog(this, pContexte, pNSResModule) ;
	if ((pListeCorDlg->Execute() == IDOK) && (pListeCorDlg->CorrespChoisi >= 0))
	{
  	pCriteres->sCodePrescript = string(pListeCorDlg->pCorrespSelect->pDonnees->code) ;
    sTitre = pListeCorDlg->pCorrespSelect->pDonnees->donneTitre() ;
    pPrescript->SetCaption(sTitre.c_str()) ;
    pCriteres->sTitrePrescript = sTitre ;
	}
	delete pListeCorDlg ;
*/

}
catch (...)
{
	erreur("Exception NSMultiCriteresDialog::CmPrescript.", standardError, 0) ;
}
}

voidNSMultiCriteresDialog::CmOk()
{  string sDate ;
	pDate1->getDate(&sDate) ;

	if (string("") == sDate)
	{
		erreur("Vous devez saisir une date de début.", standardError, 0) ;
    return ;
	}
	pCriteres->sDate1 = sDate ;

	pDate2->getDate(&sDate) ;

	if (string("") == sDate)
	{
		erreur("Vous devez saisir une date de fin.", warningError, 0) ;
		return ;
	}
	pCriteres->sDate2 = sDate ;

	if ((pCriteres->sDate2) < (pCriteres->sDate1))
	{
		erreur("La date de début ne peut etre supérieure à la date de fin.", warningError, 0) ;
		return ;
	}

  char far cfTexte[7] = "" ;
	pExamen->GetText(cfTexte, 7) ;
	if (!strcmp(cfTexte, ""))
	{
		pCriteres->sCodeExamen = string("") ;
		pCriteres->sSynExamen  = string("") ;
	}
	else
	{
		string sCode = pExamen->getCode() ;
	  pCriteres->sCodeExamen = string(sCode, 0, 5) ;
		pCriteres->sSynExamen  = string(sCode, 5, 1) ;
	}

	if (pActesPerso->GetCheck() == BF_CHECKED)
		pCriteres->bActesPerso = true ;
	else if (pActesTous->GetCheck() == BF_CHECKED)
		pCriteres->bActesPerso = false ;
	else
	{
		erreur("Vous devez saisir un choix (Vos actes / Tous les actes).", warningError, 0) ;
		return ;
	}

	if (pImpTous->GetCheck() == BF_CHECKED)
		pCriteres->iImpayes = 0 ;
	else if (pImpImpayes->GetCheck() == BF_CHECKED)
		pCriteres->iImpayes = 1 ;
	else if (pImpPayes->GetCheck() == BF_CHECKED)
		pCriteres->iImpayes = 2 ;
	else
	{
		erreur("Vous devez choisir un critere pour les impayes.", warningError, 0) ;
		return  ;
	}

	if      (pCtxtTous->GetCheck() == BF_CHECKED)
		pCriteres->iContexte = 0 ;
	else if (pCtxtNP->GetCheck()   == BF_CHECKED)
		pCriteres->iContexte = 1 ;
  else if (pCtxtExt->GetCheck()  == BF_CHECKED)
		pCriteres->iContexte = 2 ;
	else if (pCtxtHosp->GetCheck() == BF_CHECKED)
		pCriteres->iContexte = 4 ;
	else if (pCtxtAmbu->GetCheck() == BF_CHECKED)
		pCriteres->iContexte = 8 ;
	else
	{
   	erreur("Vous devez choisir un contexte.", warningError, 0) ;
		return ;
	}

	TDialog::CmOk() ;
}

voidNSMultiCriteresDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

//***************************************************************************
//
//  Méthodes de NSFactCriteresDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSFactCriteresDialog, NSUtilDialog)
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//// Constructeur
//
NSFactCriteresDialog::NSFactCriteresDialog(TWindow* pere, NSContexte* pCtx)
							 : NSUtilDialog(pere, pCtx, "IDD_FACTCRITERES", pNSResModule)
{
	// Initialisation des donnees
	pCriteres   = new NSMultiCriteres ;

	// Création de tous les "objets de contrôle"
	pDate1 	 		= new NSUtilEditDate(pContexte, this, IDC_FC_DATE1) ;
	pDate2			= new NSUtilEditDate(pContexte, this, IDC_FC_DATE2) ;
	pActes			= new TGroupBox(this, IDC_FC_ACTES) ;
	pActesPerso = new TRadioButton(this, IDC_FC_ACTES_PERSO, pActes) ;
	pActesTous  = new TRadioButton(this, IDC_FC_ACTES_TOUS, pActes) ;
	pOrga				= new TComboBox(this, IDC_FC_ORGA) ;

	pCodeOrgaArray = new NSCodeOrgaArray ;
	nbCodeOrga		 = 0 ;
}

//
// Destructeur
//
NSFactCriteresDialog::~NSFactCriteresDialog()
{
	delete pCriteres ;
  delete pDate1 ;
  delete pDate2 ;
  delete pActes ;
  delete pActesPerso ;
  delete pActesTous ;
  delete pOrga ;
  delete pCodeOrgaArray ;
}

//
// Fonction SetupWindow
//
void
NSFactCriteresDialog::SetupWindow()
{
	// fichiers d'aide
  sHindex = "" ;
  sHcorps = "Comptabilite.html" ;
	TDate	dateSys ;
	char	dateJour[9] = "" ;

	NSUtilDialog::SetupWindow() ;

	sprintf(dateJour, "%4d%02d%02d", (int)dateSys.Year(),
      				(int)dateSys.Month(), (int)dateSys.DayOfMonth()) ;

	pDate1->setDate(dateJour) ;
	pDate2->setDate(dateJour) ;

	pActesTous->Check() ;

	// on charge la table des organismes
	if (!InitCodeOrgaArray())
	{
		erreur("Probleme au chargement du tableau des codes organisme.", standardError, 0) ;
		return ;
	}

	pOrga->SetSelIndex(0) ;
}

bool
NSFactCriteresDialog::InitCodeOrgaArray()
{
	NSCodeOrga      CodeOrga(pContexte->getSuperviseur()) ;
	NSCodeOrgaInfo* pInfoDebut = new NSCodeOrgaInfo ;

	strcpy(pInfoDebut->_Donnees.code, "#A") ;
	strcpy(pInfoDebut->_Donnees.lib_court, "Tous + patients") ;
	strcpy(pInfoDebut->_Donnees.lib_long, pInfoDebut->_Donnees.lib_court) ;
	pCodeOrgaArray->push_back(new NSCodeOrgaInfo(*pInfoDebut)) ;
	pOrga->AddString(pInfoDebut->_Donnees.lib_court) ;
	nbCodeOrga++ ;

	strcpy(pInfoDebut->_Donnees.code, "#B") ;
	strcpy(pInfoDebut->_Donnees.lib_court, "Tous - patients") ;
	strcpy(pInfoDebut->_Donnees.lib_long, pInfoDebut->_Donnees.lib_court) ;
	pCodeOrgaArray->push_back(new NSCodeOrgaInfo(*pInfoDebut)) ;
	pOrga->AddString(pInfoDebut->_Donnees.lib_court) ;
	nbCodeOrga++ ;

	strcpy(pInfoDebut->_Donnees.code, "#C") ;
	strcpy(pInfoDebut->_Donnees.lib_court, "Patients") ;
	strcpy(pInfoDebut->_Donnees.lib_long, pInfoDebut->_Donnees.lib_court) ;
	pCodeOrgaArray->push_back(new NSCodeOrgaInfo(*pInfoDebut)) ;
	pOrga->AddString(pInfoDebut->_Donnees.lib_court) ;
	nbCodeOrga++ ;

	CodeOrga.lastError = CodeOrga.open() ;
	if (CodeOrga.lastError != DBIERR_NONE)
	{
  	erreur("Erreur à l'ouverture de la base CodeOrga.", standardError, CodeOrga.lastError) ;
		return false ;
	}

	CodeOrga.lastError = CodeOrga.debut(dbiWRITELOCK) ;
	if ((CodeOrga.lastError != DBIERR_NONE) && (CodeOrga.lastError != DBIERR_EOF))
	{
		erreur("Erreur de positionnement dans le fichier CodeOrga.db.", standardError, CodeOrga.lastError) ;
		CodeOrga.close() ;
		return false ;
	}

	while (CodeOrga.lastError != DBIERR_EOF)
	{
  	CodeOrga.lastError = CodeOrga.getRecord() ;
		if (CodeOrga.lastError != DBIERR_NONE)
		{
    	erreur("Erreur à la lecture d'une fiche CodeOrga.", standardError, CodeOrga.lastError) ;
      CodeOrga.close() ;
      return false ;
    }

    // on remplit le tableau et la combobox
    pCodeOrgaArray->push_back(new NSCodeOrgaInfo(&CodeOrga)) ;
    pOrga->AddString(CodeOrga._Donnees.lib_court) ;
    nbCodeOrga++ ;

    // ... on passe au composant suivant
    CodeOrga.lastError = CodeOrga.suivant(dbiWRITELOCK) ;
    if ((CodeOrga.lastError != DBIERR_NONE) && (CodeOrga.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche CodeOrga.", standardError, CodeOrga.lastError) ;
      CodeOrga.close() ;
      return false ;
    }
	} // fin du while (recherche des composants images)

	// on ferme la base CODEORGA
	CodeOrga.lastError = CodeOrga.close() ;
	if (CodeOrga.lastError != DBIERR_NONE)
	{		erreur("Erreur de fermeture du fichier CodeOrga.", standardError, CodeOrga.lastError) ;
		return false ;
	}

	return true ;
}

void
NSFactCriteresDialog::CmOk()
{
	string sDate, sCode ;
	int    indexOrga ;

	pDate1->getDate(&sDate) ;

	if (sDate == "")
	{
  	erreur("Vous devez saisir une date de début.", warningError, 0) ;
    return ;
	}
	pCriteres->sDate1 = sDate ;

	pDate2->getDate(&sDate) ;

	if (sDate == "")
	{
  	erreur("Vous devez saisir une date de fin.", warningError, 0) ;
    return ;
	}
	pCriteres->sDate2 = sDate ;

	if ((pCriteres->sDate2) < (pCriteres->sDate1))
	{
  	erreur("La date de début ne peut etre supérieure à la date de fin.", warningError, 0) ;
    return ;
	}

	if (pActesPerso->GetCheck() == BF_CHECKED)
  	pCriteres->bActesPerso = true ;
	else if (pActesTous->GetCheck() == BF_CHECKED)
		pCriteres->bActesPerso = false ;
	else
	{
  	erreur("Vous devez saisir un choix (Vos actes / Tous les actes).", warningError, 0) ;
    return ;
	}

	indexOrga = pOrga->GetSelIndex() ;
	if (indexOrga >= 0)
	{
  	pCriteres->sCodeOrga     = string((*pCodeOrgaArray)[indexOrga]->_Donnees.code) ;
    pCriteres->sLibCourtOrga = string((*pCodeOrgaArray)[indexOrga]->_Donnees.lib_court) ;
    pCriteres->sLibLongOrga  = string((*pCodeOrgaArray)[indexOrga]->_Donnees.lib_long) ;
	}

	TDialog::CmOk() ;
}

voidNSFactCriteresDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

//***********************************************************************////							Classe NsvCheckTransferDialog
//***********************************************************************//

DEFINE_RESPONSE_TABLE1(CptaSearchProgressDialog, NSUtilDialog)	EV_COMMAND(IDC_CHECK_STOP, StopProcess),END_RESPONSE_TABLE;

CptaSearchProgressDialog::CptaSearchProgressDialog(TWindow* pere, NSContexte* pCtx)
                         :NSUtilDialog(pere, pCtx, "IDD_CPTA_SEARCH_PROCESS", pNSResModule)
{
	pCurrentDate = new TStatic(this, IDC_DISPLAY_DATE) ;
	pCurrentCode = new TStatic(this, IDC_DISPLAY_CODE) ;
	pAck = new TStatic(this, IDC_CHECK_ACK) ;

  count      = 0 ;
  num        = 0 ;
  maxNum     = 0 ;
  maxGarbage = 0 ;

  bStop = false ;
}

CptaSearchProgressDialog::~CptaSearchProgressDialog()
{
	delete pCurrentDate ;
	delete pCurrentCode ;
	delete pAck ;
}

void
CptaSearchProgressDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow();
  // centrage du dialogue à l'écran  //  NS_CLASSLIB::TRect rectDlg = GetWindowRect() ;//coordonnées par % à l'écran  NS_CLASSLIB::TRect rectMain = Parent->GetWindowRect() ;  //
  // ScreenToClient converts the screen coordinates
  // of a specified point on the screen to client coordinates.
  //  NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y());
  Parent->ScreenToClient(point);
  int X = point.X() ;
  int Y = point.Y() ;
  int W = rectDlg.Width() ;
  int H = rectDlg.Height() ;  X = (rectMain.Width())/2 - W/2 ;  Y = (rectMain.Height())/2 - H/2 ;  //  // fixer la nouvelle position
  //
  SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;
}

void
CptaSearchProgressDialog::SetNewDate(string sNewDate)
{
  string sLang     = pContexte->getUserLanguage() ;
	string sDateExam = donne_date(sNewDate, sLang) ;

	pCurrentDate->SetText(sDateExam.c_str()) ;
}

void
CptaSearchProgressDialog::SetNewCode(string sNewCode)
{
	pCurrentCode->SetText(sNewCode.c_str()) ;
}

void
CptaSearchProgressDialog::SetAck(string sAck)
{
	pAck->SetText(sAck.c_str()) ;
}

void
CptaSearchProgressDialog::ClearText()
{
	pCurrentDate->SetText("") ;
	pCurrentCode->SetText("") ;
	pAck->SetText("") ;
}

void
CptaSearchProgressDialog::StopProcess()
{
	bStop = true ;
}

bool
CptaSearchProgressDialog::isStopped()
{
	return bStop ;
}

// fin de nsactdlg.cpp
///////////////////////

