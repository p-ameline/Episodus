//---------------------------------------------------------------------------//    NS_FIC.CPP
//    KRZISCH PH.   janvier 92
//  Implémentation des objets de gestion des fiches descriptives
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

//using namespace std;

#include "nautilus\nssuper.h"
#include "nautilus\nsrechdl.h"
#include "partage\nsdivfct.h"
#include "nsbb\nspatpat.h"
#include "nssavoir\ns_fic.h"

//***************************************************************************
// Implémentation des méthodes NSSavFiche
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSSavFicheData::NSSavFicheData()
{
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Fonction : NSSavFicheData::metAZero()
//  Met les champs de données à zéro
//---------------------------------------------------------------------------
void
NSSavFicheData::metAZero()
{
  _sSens = string("") ;
	NSPathoBaseData::metAZero() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSSavFicheData::NSSavFicheData(NSSavFicheData& rv)
               :NSPathoBaseData(rv)
{
  _sSens = rv._sSens ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSSavFicheData::~NSSavFicheData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSSavFicheData&
NSSavFicheData::operator=(NSSavFicheData src)
{
	if (this == &src)
		return *this ;

	_sSens = src._sSens ;

  NSPathoBaseData* pbdata1 = this ;
	NSPathoBaseData* pbdata2 = &src ;

	*pbdata1 = *pbdata2 ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSSavFicheData::operator == (const NSSavFicheData& o)
{
	if ((_sSens             == o._sSens) &&
		  (_sCodeLocalisation == o._sCodeLocalisation))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSSavFiche::NSSavFiche(NSSuper* pSuper)
           :NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSSavFicheData() ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSSavFiche::NSSavFiche(NSSavFiche& rv)
           :NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSSavFicheData() ;
	//
	// Copie les valeurs du NSSavFicheInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSSavFiche::~NSSavFiche()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Fonction : NSSavFiche::alimenteFiche()
//
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSSavFiche::alimenteFiche()
{
  char sens[FICHE_SENS_LEN + 1] ;
  char codeLocalisation[BASE_LOCALISATION_LEN + 1] ;
  char type[BASE_TYPE_LEN + 1] ;
  char lexique[BASE_LEXIQUE_LEN + 1] ;
  char complement[BASE_COMPLEMENT_LEN + 1] ;
  char unit[BASE_UNIT_LEN + 1] ;
  char certitude[BASE_CERTITUDE_LEN + 1] ;
  char interet[BASE_INTERET_LEN + 1] ;
  char pluriel[BASE_PLURIEL_LEN + 1] ;
  char visible[BASE_VISIBLE_LEN + 1] ;

	alimenteChamp(sens, 		        FICHE_SENS_FIELD, 			  FICHE_SENS_LEN) ;
	alimenteChamp(codeLocalisation, FICHE_LOCALISATION_FIELD, BASE_LOCALISATION_LEN) ;
	alimenteChamp(type, 		        FICHE_TYPE_FIELD, 			  BASE_TYPE_LEN) ;
	alimenteChamp(lexique, 	        FICHE_LEXIQUE_FIELD, 		  BASE_LEXIQUE_LEN) ;
	alimenteChamp(complement,       FICHE_COMPLEMENT_FIELD, 	BASE_COMPLEMENT_LEN) ;
  alimenteChamp(unit,             FICHE_UNIT_FIELD,         BASE_UNIT_LEN) ;
	alimenteChamp(certitude, 	      FICHE_CERTITUDE_FIELD, 	  BASE_CERTITUDE_LEN) ;
	alimenteChamp(interet, 	        FICHE_INTERET_FIELD, 		  BASE_INTERET_LEN) ;
	alimenteChamp(pluriel, 	        FICHE_PLURIEL_FIELD, 		  BASE_PLURIEL_LEN) ;

  pDonnees->setSens(sens) ;
  pDonnees->setLocalisation(codeLocalisation) ;
  pDonnees->setTypeString(type) ;
  pDonnees->setLexique(lexique) ;
  pDonnees->setComplement(complement) ;
  pDonnees->setUnit(unit) ;
  pDonnees->setCertitude(certitude) ;
  pDonnees->setInteret(interet) ;
  pDonnees->setPluriel(pluriel) ;
}

//---------------------------------------------------------------------------
//  Fonction : NSSavFiche::videFiche()
//
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSSavFiche::videFiche()
{
  char sens[FICHE_SENS_LEN + 1] ;
  char codeLocalisation[BASE_LOCALISATION_LEN + 1] ;
  char type[BASE_TYPE_LEN + 1] ;
  char lexique[BASE_LEXIQUE_LEN + 1] ;
  char complement[BASE_COMPLEMENT_LEN + 1] ;
  char unit[BASE_UNIT_LEN + 1] ;
  char certitude[BASE_CERTITUDE_LEN + 1] ;
  char interet[BASE_INTERET_LEN + 1] ;
  char pluriel[BASE_PLURIEL_LEN + 1] ;
  char visible[BASE_VISIBLE_LEN + 1] ;

  strcpy(sens, pDonnees->getSens().c_str()) ;
  strcpy(codeLocalisation, pDonnees->getLocalisation().c_str()) ;
  strcpy(type, pDonnees->getTypeString().c_str()) ;
  strcpy(lexique, pDonnees->getLexique().c_str()) ;
  strcpy(complement, pDonnees->getComplement().c_str()) ;
  strcpy(unit, pDonnees->getUnit().c_str()) ;
  strcpy(certitude, pDonnees->getCertitude().c_str()) ;
  strcpy(interet, pDonnees->getInteret().c_str()) ;
  strcpy(pluriel, pDonnees->getPluriel().c_str()) ;
  strcpy(visible, pDonnees->getVisible().c_str()) ;

	videChamp(sens, 		  	    FICHE_SENS_FIELD, 			  FICHE_SENS_LEN) ;
	videChamp(codeLocalisation, FICHE_LOCALISATION_FIELD, BASE_LOCALISATION_LEN) ;
	videChamp(type, 		  	    FICHE_TYPE_FIELD, 			  BASE_TYPE_LEN) ;
	videChamp(lexique, 	  	    FICHE_LEXIQUE_FIELD, 		  BASE_LEXIQUE_LEN) ;
	videChamp(complement,       FICHE_COMPLEMENT_FIELD, 	BASE_COMPLEMENT_LEN) ;
  videChamp(unit,             FICHE_UNIT_FIELD,         BASE_UNIT_LEN) ;
	videChamp(certitude, 	      FICHE_CERTITUDE_FIELD, 	  BASE_CERTITUDE_LEN) ;
	videChamp(interet, 	  	    FICHE_INTERET_FIELD, 		  BASE_INTERET_LEN) ;
	videChamp(pluriel, 	  	    FICHE_PLURIEL_FIELD, 		  BASE_PLURIEL_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSSavFiche::getRecord()
//
//  Prend l'enregistrement en cours et assigne la valeur des
//  champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSSavFiche::getPatRecord()
{
	//
	// La table est-elle ouverte ?
	//
	if (false == _isOpen)
		return(lastError = ERROR_TABLENOTOPEN) ;
	//
	// Appel de la classe de base pour récupérer l'enregistrement.
	//
	lastError = getDbiRecord(dbiWRITELOCK) ;

	return(lastError) ;
}

//---------------------------------------------------------------------------//  Fonction : NSSavFiche::open()
//
//  Ouvre la table primaire et les index secondaires
//
//  Retour :	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSSavFiche::open()
{
	char tableName[] = "NSM_FICHES.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL) ;
	return(lastError) ;
}

//---------------------------------------------------------------------------
//  Function:  NSSavFiche::Create()
//
//---------------------------------------------------------------------------
bool
NSSavFiche::Create()
{
	return true ;
}

//---------------------------------------------------------------------------
//  Function:  NSSavFiche::Modify()
//
//---------------------------------------------------------------------------
bool
NSSavFiche::Modify()
{
	return true ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSSavFiche&
NSSavFiche::operator=(NSSavFiche src)
{
	*pDonnees = *(src.pDonnees) ;
	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSSavFiche::operator == ( const NSSavFiche& o )
{
	return (*pDonnees == *(o.pDonnees)) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSSavFicheInfo::NSSavFicheInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSSavFicheInfo::NSSavFicheInfo()
{
    // Crée l'objet de données
	pDonnees = new NSSavFicheData() ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSSavFicheInfo::NSSavFicheInfo(NSSavFiche*)
//
//  Description:	Constructeur à partir d'un NSSavFiche
//---------------------------------------------------------------------------
NSSavFicheInfo::NSSavFicheInfo(NSSavFiche* pPatho)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSSavFicheData() ;
	//
	// Copie les valeurs du NSDocument
	//
	*pDonnees = *(pPatho->pDonnees) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSSavFicheInfo::NSSavFicheInfo(NSSavFicheInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSSavFicheData() ;
	//
	// Copie les valeurs du NSSavFicheInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSSavFicheInfo::~NSSavFicheInfo()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSSavFicheInfo&
NSSavFicheInfo::operator=(NSSavFicheInfo src)
{
	if (this == &src)
		return *this ;

	*pDonnees = *(src.pDonnees) ;
  
	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSSavFicheInfo::operator == ( const NSSavFicheInfo& o )
{
	return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
// Implémentation des méthodes NSFicheManager
//***************************************************************************

NSFicheManager::NSFicheManager(NSSuper* pSuper)
               :NSSuperRoot(pSuper)
{
	pCurseur = new NSSavFiche(pSuper) ;
	//
	// Ouverture du fichier
	//
	pCurseur->lastError = pCurseur->open() ;
	if (pCurseur->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier de fiches.", standardError, pCurseur->lastError) ;
		delete pCurseur ;
	}
}

NSFicheManager::NSFicheManager(NSFicheManager& rv)
               :NSSuperRoot(rv._pSuper)
{
	pCurseur = new NSSavFiche(_pSuper) ;
	//
	// Ouverture du fichier
	//
	pCurseur->lastError = pCurseur->open() ;
	if (pCurseur->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier de fiches.", standardError, pCurseur->lastError) ;
		delete pCurseur ;
	}
}

NSFicheManager::~NSFicheManager()
{
	if (pCurseur)
	{
		pCurseur->close() ;
		delete pCurseur ;
	}
}

bool
NSFicheManager::getFiche(string sSens, NSPatPathoArray* pPPTarray, bool bVerbose)
{
	if ((!pPPTarray) || (sSens == ""))
		return false ;

	pPPTarray->vider() ;

	//
	// Recherche de la première fiche
	//
	string cle = sSens + string(BASE_LOCALISATION_LEN, ' ') ;

	pCurseur->lastError = pCurseur->chercheClef(    &cle,
                                                    "",
                                                    0,
                                                    keySEARCHGEQ,
                                                    dbiWRITELOCK) ;
	if (pCurseur->lastError != DBIERR_NONE)
	{
  	if (bVerbose)
    	erreur("Il n'existe pas de fiche pour cet élément.", standardError, pCurseur->lastError) ;
		return false ;
	}

	pCurseur->lastError = pCurseur->getRecord() ;
	if (pCurseur->lastError != DBIERR_NONE)
	{
		erreur("Le fichier de fiches est défectueux.", standardError, pCurseur->lastError) ;
		return false ;
	}
	if (sSens != pCurseur->getSens())
	{
  	if (bVerbose)
    	erreur("Il n'existe pas de fiche pour cet élément.", standardError, pCurseur->lastError) ;
		return false ;
	}
  //
  // On avance dans le fichier tant que les fiches trouvées appartiennent
  // à ce compte rendu
  //
	while ((pCurseur->lastError != DBIERR_EOF) && (sSens == pCurseur->getSens()))
	{
  	pPPTarray->push_back(new NSPatPathoInfo(pCurseur)) ;
    pCurseur->lastError = pCurseur->suivant(dbiWRITELOCK) ;
    if ((pCurseur->lastError != DBIERR_NONE) && (pCurseur->lastError != DBIERR_EOF))
    {
    	erreur("Pb d'acces a la fiche suivante.", standardError, pCurseur->lastError) ;
			return false ;
		}

    if (pCurseur->lastError == DBIERR_NONE)
    {
    	pCurseur->lastError = pCurseur->getRecord() ;
      if (pCurseur->lastError != DBIERR_NONE)
			{
      	erreur("Le fichier de fiches est défectueux.", standardError, pCurseur->lastError) ;
				return false ;
      }
    }
  }
  return true ;
}

bool
NSFicheManager::setFiche(string sSens, NSPatPathoArray* pPPTarray)
{
	//
	// On efface d'abord les anciennes données
	//
	string cle = sSens + string(BASE_LOCALISATION_LEN, ' ') ;
	bool effacer = true ;
	while (effacer)
	{
		effacer = false ;
    pCurseur->lastError = pCurseur->chercheClef(&cle,
                                                    "",
                                                    0,
                                                    keySEARCHGEQ,
                                                    dbiWRITELOCK) ;
    if (pCurseur->lastError == DBIERR_NONE)
    {
    	pCurseur->lastError = pCurseur->getRecord() ;
      if (pCurseur->lastError == DBIERR_NONE)
      	if (sSens == pCurseur->getSens())
        	effacer = true ;
    }
    if (effacer)
    {
    	pCurseur->lastError = pCurseur->deleteRecord() ;
      if (pCurseur->lastError != DBIERR_NONE)
      	effacer = false ;
    }
  }

	if ((NULL == pPPTarray) || (pPPTarray->empty()))
  	return true ;

	//
	// Stockage de la PatPathoArray dans la base
	//
	for (PatPathoIter iJ = pPPTarray->begin() ; pPPTarray->end() != iJ ; iJ++)
	{
		*((NSPathoBaseData*)pCurseur->pDonnees) = *((NSPathoBaseData*)(*iJ)->getDataTank()) ;
    pCurseur->setSens(sSens) ;

    pCurseur->lastError = pCurseur->appendRecord() ;
    if (pCurseur->lastError != DBIERR_NONE)
  	{
    	erreur("Erreur à la sauvegarde des données.", standardError, pCurseur->lastError) ;
      return false ;
    }
  }
  //
  // Fermeture de la base
  //
  return true ;
}

void
NSFicheManager::updateFiche(NSPatPathoArray* pFiche, string sChapter, string sSubChapter, NSPatPathoArray* pPPTvalues)
{
	if (NULL == pFiche)
		return ;

	//
	// Fiche vide : on ajoute simplement
	//
	if (pFiche->empty())
	{
  	if ((NULL == pPPTvalues) || (pPPTvalues->empty()))
    	return ;

    int iCol = 0 ;
    if (sChapter != "")
    {
    	pFiche->ajoutePatho(sChapter, iCol++) ;
      if (sSubChapter != "")
      	pFiche->ajoutePatho(sSubChapter, iCol++) ;
      pFiche->InserePatPatho(pFiche->end(), pPPTvalues, iCol++) ;
    }
    return ;
  }
  //
  // Fiche non vide : on supprime les anciennes valeurs, puis on ajoute les nouvelles
  //
  if (string("") != sChapter)
  {
  	string sSepar = string("/") ;
    string sCodeSens ;
    NSDico::donneCodeSens(&sChapter, &sCodeSens) ;
    string sChem = sCodeSens ;
    if (string("") != sSubChapter)
    {
    	NSDico::donneCodeSens(&sSubChapter, &sCodeSens) ;
      sChem += sSepar + sCodeSens ;
    }
    string sLoca ;
    //
    // Recherche de la chaine...
    //
    PatPathoIter pptIter = pFiche->ChaineDansPatpatho(sChem, &sLoca, sSepar) ;
    //
    // Chaine entièrement trouvée
    //
    if ((pptIter) && (pFiche->end() != pptIter) && (sLoca == sChem))
    {
    	if ((pPPTvalues) && (false == pPPTvalues->empty()))
      {
      	pFiche->SupprimerFils(pptIter) ;
        pFiche->InserePatPathoFille(pptIter, pPPTvalues) ;
      }
      else
      	pFiche->SupprimerItem(pptIter) ;
    }
    else if (string("") != sSubChapter)
    {
    	if ((pPPTvalues) && (false == pPPTvalues->empty()))
      {
      	// On cherche le chapitre tout seul
        //
        NSDico::donneCodeSens(&sChapter, &sCodeSens) ;
        sChem = sCodeSens ;
        PatPathoIter pptIter = pFiche->ChaineDansPatpatho(sChem, &sLoca, sSepar) ;
        //
        // Chapitre trouvé
        //
        if ((pptIter) && (pFiche->end() != pptIter) && (sLoca == sChem))
        {
        	NSPatPathoArray NewPpt(_pSuper) ;
          NewPpt.ajoutePatho(sSubChapter, 0) ;
          NewPpt.InserePatPatho(NewPpt.end(), pPPTvalues, 1) ;
          pFiche->InserePatPathoFille(pptIter, &NewPpt) ;
        }
      }
    }
    //
    // Le chapitre n'existe pas et le sous-chapitre est vide
    //
    else if ((pPPTvalues) && (false == pPPTvalues->empty()))
    {
    	pFiche->ajoutePatho(sChapter, 0) ;
      pFiche->InserePatPatho(pFiche->end(), pPPTvalues, 1) ;
    }
  }
  else
  {
  	pFiche->vider() ;

    if ((NULL == pPPTvalues) || (pPPTvalues->empty()))
    	return ;

    *pFiche = *pPPTvalues ;
	}
}

