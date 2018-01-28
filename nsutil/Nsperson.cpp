//---------------------------------------------------------------------------
//    NSPERSON.CPP
//    KRZISCH PH.   janvier 92
//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------

#include <owl\olemdifr.h>
#include <mem.h>
#include <stdio.h>
#include <string.h>
#include <cstring.h>

#include "nautilus\nssuper.h"
#include "nautilus\nsrechdl.h"
#include "partage\nsdivfct.h"
#include "nautilus\nspatdlg.h"
#include "nautilus\nsanxary.h"
#include "nsdn\nsdocum.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsmanager.h"

#ifndef __NSPERSON_H
# include "partage\nsperson.h"
#endif

#include "pilot\NautilusPilot.hpp"
#include "nsbb\tagNames.h"


//***************************************************************************
// Implémentation des méthodes NSPatient
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSPatientData::NSPatientData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSPatientData::NSPatientData(NSContexte* pCtx)
              :NSRoot(pCtx)
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::NSPatientData(NSPatientData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPatientData::NSPatientData(NSPatientData& rv)
              :NSRoot(rv.pContexte)
{
	sNss           = rv.sNss ;
	sNom           = rv.sNom ;
	sPrenom        = rv.sPrenom ;
	sCode          = rv.sCode ;
	sConvoc        = rv.sConvoc ;
	sSexe          = rv.sSexe ;
	sAdresse       = rv.sAdresse ;
	sNaissance     = rv.sNaissance ;
	sTelepor       = rv.sTelepor ;
	sTelebur       = rv.sTelebur ;
	sSitfam        = rv.sSitfam ;
	sLang          = rv.sLang ;
	sCivilite      = rv.sCivilite ;
	sNomLong       = rv.sNomLong ;
	sIpp           = rv.sIpp ;
	sMergedWith    = rv.sMergedWith ;
	sNomJeuneFille = rv.sNomJeuneFille ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::operator=(NSPatientData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSPatientData&
NSPatientData::operator=(NSPatientData src)
{
	if (this == &src)
		return *this ;

	sNss           = src.sNss ;
  sNom           = src.sNom ;
  sPrenom        = src.sPrenom ;
  sCode          = src.sCode ;
  sConvoc        = src.sConvoc ;
  sSexe          = src.sSexe ;
  sAdresse       = src.sAdresse ;
  sNaissance     = src.sNaissance ;
  sTelepor       = src.sTelepor ;
  sTelebur       = src.sTelebur ;
  sSitfam        = src.sSitfam ;
  sLang          = src.sLang ;
  sCivilite      = src.sCivilite ;
  sNomLong       = src.sNomLong ;
  sIpp           = src.sIpp ;
  sMergedWith    = src.sMergedWith ;
  sNomJeuneFille = src.sNomJeuneFille ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::operator==(NSPatientData src)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSPatientData::operator == ( NSPatientData& o )
{
  if ((sNss == o.sNss)                &&
      (sNom == o.sNom)                &&
      (sPrenom == o.sPrenom)          &&
      (sCode == o.sCode)              &&
      (sConvoc == o.sConvoc)          &&
      (sSexe == o.sSexe)              &&
      (sAdresse == o.sAdresse)        &&
      (sNaissance == o.sNaissance)    &&
      (sTelepor == o.sTelepor)        &&
      (sTelebur == o.sTelebur)        &&
      (sSitfam == o.sSitfam)
      )
      return 1 ;
  else
      return 0 ;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::fabriqueNomLong()
//
//  Description: Initialise nom_long
//
//  Note : on suppose que les données naissance, nom, prenom et sexe
//  sont deja initialisées
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void NSPatientData::fabriqueNomLong(string sLang)
{
	if (string("") == sLang)
		sLang = pContexte->getUserLanguage() ;

	// calcul de l'age du patient
	bool bEnfant = false ;
	char szDateJour[10] ;

	if ((string("") != sNaissance) && (string("00000000") != sNaissance))
	{
    NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;
    int iAge = donne_age(tpsNow.donneDate(), sNaissance) ;

/*
		char szNaissance[PAT_NAISSANCE_LEN + 1] ;
		strcpy(szNaissance, sNaissance.c_str()) ;
		donne_date_duJour(szDateJour) ;
		int age = donne_age(szDateJour, szNaissance) ;
*/

		if (age <= 14)
			bEnfant = true ;
	}

	if (string("fr") == sLang)
	{
		if (bEnfant)
			sNomLong = string("Enfant ") ;
		else
		{
			if (sSexe[0] == '2')
				sNomLong = string("Mme ") ;
			else
				sNomLong = string("M. ") ;
		}
	}

	if (string("en") == sLang)
	{
		if (bEnfant)
			sNomLong = string("Child ");
		else
		{
			if (sSexe[0] == '2')
				sNomLong = string("Ms ");
			else
				sNomLong = string("Mr ");
		}
	}

	sNomLong += sNom ;
	sNomLong += string(" ") ;
	sNomLong += sPrenom ;
}

string
NSPatientData::fabriqueCivilite(bool /* bShort */, string sLang)
{
	if (sLang == "")
		sLang = pContexte->getUtilisateur()->donneLang() ;

	if (sCivilite != "")
	{
		string sLibelleTrouve ;
    pContexte->getDico()->donneLibelle(sLang, &sCivilite, &sLibelleTrouve) ;
    if (sLibelleTrouve != "")
    {
    	sLibelleTrouve += string(" ") ;
      return sLibelleTrouve ;
    }
  }

	return string("") ;
}

/*
// cette fonction a été mise dans nsperson.h --> on en a besoin dans CalculAgeKS
//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneNaissance(char *dateNaiss)
//
//  Description: Retourne dans dateNaiss la date de naissance du patient
//					  au format AAAAMMJJ
//					  Si le champ existe mais n'a pas été renseigné,
//					  retourne "00000000"
//
//  Returns:     1 si OK, 0 si date de naissance n'est pas un champ du fichier
//---------------------------------------------------------------------------
int
NSPatientData::donneNaissance(char *dateNaiss)
{
	strcpy(dateNaiss, naissance);
	if ((dateNaiss[0] == '\0') ||
			(strcmp(dateNaiss, "        ") == 0) ||
			(strcmp(dateNaiss, "19000000") == 0))
		strcpy(dateNaiss, "00000000");
	return 1;
}
*/

//---------------------------------------------------------------------------
//  Function:    NSPatientData::setNbEnfants(int nbenf)
//
//  Description: ajoute le nb d'enfants à sitfam
//---------------------------------------------------------------------------
void NSPatientData::setNbEnfants(int nbenf)
{
	char sitfam[3] ;

	if (nbenf < 0)
	{
		erreur("Le nombre d'enfants est fixé à une valeur incorrecte.", standardError, 0) ;
		sitfam[1] = '0' ;
		sitfam[2] = '\0' ;
		return;
	}

	if (nbenf <= 9)
		sprintf(&sitfam[1], "%1d", nbenf);
	else
		if (nbenf <= 35)
		{
			// on représente la valeur sous forme d'un caractère entre 'A' et 'Z'
			nbenf += 55;
			sitfam[1] = (char) (nbenf);
			sitfam[2] = '\0';
		}
		else // cas nbenf > 35
		{
			sitfam[1] = '*';
            sitfam[2] = '\0';
        }

	sSitfam = string(sitfam) ;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::getNbEnfants()
//
//  Description: retourne le nb d'enfants d'après sitfam[1]
//---------------------------------------------------------------------------
int NSPatientData::getNbEnfants()
{
	char sitfam[3] ;
	strcpy(sitfam, sSitfam.c_str()) ;

	// on s'assure que sitfam[2] est NULL
	sitfam[2] = '\0';
	// on ne considère que le premier caractère
	char c;
	if (strlen(sitfam) == 2)
		c = sitfam[1];
	else
		return 0;

	if ((c >= '0') && (c <= '9'))
		return atoi(&sitfam[1]);
	else
		if ((c >= 'A') && (c <= 'Z'))
			return (((int) c) - 55);

	// cas nb d'enfants > 35
  return -1;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneSitFam()
//
//  Description: Retourne la string décrivant la situation familiale
//---------------------------------------------------------------------------
string NSPatientData::donneSitFam(string sLang)
{
	string sSitFam ;

	if (sLang == "")
		sLang = pContexte->getUtilisateur()->donneLang() ;

	switch (sSitfam[0])
	{
		case 'C' :	sSitFam = "Célibataire";
								break;

		case 'V' :	if (estMasculin())
									sSitFam = "Veuf";
								else
									sSitFam = "Veuve";
            		break;

		case 'B' :	if (estMasculin())
									sSitFam = "Concubin";
								else
									sSitFam = "Concubine";
            		break;

		case 'M' :	if (estMasculin())
									sSitFam = "Marié";
								else
									sSitFam = "Mariée";
								break;

		case 'D' :	if (estMasculin())
									sSitFam = "Divorcé";
								else
									sSitFam = "Divorcée";
								break;

		default :		sSitFam = "";
	}

  return sSitFam;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneNbEnfants()
//
//  Description: Retourne le nombre d'enfants
//---------------------------------------------------------------------------
string NSPatientData::donneNbEnfants()
{
	string sNbEnf;
	char cNbEnf[3];
	int nbenf = getNbEnfants();

	// cas nb enfants trop grand
	if (nbenf < 0)
		return string("nombreux enfants");

	// cas nb enfants entre 0 et max (max fixé à 35 pour l'instant !)
	switch (nbenf)
	{
		case 0 :	sNbEnf = "";
							break;
		case 1 :	sNbEnf = "1 enfant";
							break;
		default :	sprintf(cNbEnf, "%d", nbenf);
							sNbEnf = string(cNbEnf) + string(" enfants");
	}

	return sNbEnf;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneDateNaiss()
//
//  Description: Retourne une string jj/mm/aaaa contenant la date de naissance
//---------------------------------------------------------------------------
string NSPatientData::donneDateNaiss(string sLang)
{
	char jour[3];
	char mois[3];
	char annee[5];
	char dateNaiss[11];

	if (sLang == "")
		sLang = pContexte->getUtilisateur()->donneLang() ;

	if (sNaissance == "")
		return "" ;

	string sJour = string(sNaissance, 6, 2) ;
  strcpy(jour, sJour.c_str()) ;
  string sMois = string(sNaissance, 4, 2) ;
  strcpy(mois, sMois.c_str()) ;
  string sAnnee = string(sNaissance, 0, 4) ;
  strcpy(annee, sAnnee.c_str()) ;

	if (sLang == "en")
		sprintf(dateNaiss, "%s/%s/%s", mois, jour, annee) ;
	else
		sprintf(dateNaiss, "%s/%s/%s", jour, mois, annee) ;

	return string(dateNaiss) ;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneTitre()
//
//  Description: Retourne une string correspondant à nom_long s'il existe
//               sinon refabrique le nom_long
//---------------------------------------------------------------------------
string NSPatientData::donneTitre(string sLang)
{
	if (sLang == "")
		sLang = pContexte->getUtilisateur()->donneLang() ;

	if (sNomLong == "")
		fabriqueNomLong() ;

	return sNomLong ;
}

//***************************************************************************
//
// Implémentation des méthodes NSPatInfo
//
//***************************************************************************


// -----------------------------------------------------------------------------
// Fonction    : NSPatInfo::NSPatInfo(NSSuper* pSuper)
// Description : Constructeur avec superviseur (pour les accès aux bases)
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPatInfo::NSPatInfo(NSContexte *pCtx)
          :NSRoot(pCtx)
{
	// Crée l'objet de données
	pDonnees      = new NSPatientData(pContexte) ;

  pGraphPerson  = new NSPersonGraphManager(pContexte) ;
  pGraphAdr     = new NSAddressGraphManager(pContexte) ;
  sChez         = string("") ;
  pCorrespArray = new NSPidsArray ;
}


// -----------------------------------------------------------------------------
// Fonction    : NSPatInfo::NSPatInfo(NSBasicAttributeArray* pAttribute)
// Description : Constructeur avec NSBasicAttributeArray (pour les accès aux bases a l'aide du pilot)
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPatInfo::NSPatInfo(NSBasicAttributeArray* pAttribute, NSContexte* pCtx)
          :NSRoot(pCtx)
{
	// Crée l'objet de données
	pDonnees 	  	= new NSPatientData(pContexte) ;

  pCorrespArray 	= new NSPidsArray ;
  pGraphPerson    = new NSPersonGraphManager(pContexte) ;
  pGraphAdr       = new NSAddressGraphManager(pContexte) ;
  sChez           = "" ;

  NSBasicAttributeIter iter = pAttribute->begin() ;

  for(iter ; iter != pAttribute->end() ; iter++)
  {
    if      ((*iter)->getBalise() == FIRST_NAME)
    	pDonnees->sPrenom = (*iter)->getText() ;
    else if ((*iter)->getBalise() == LAST_NAME)
    	pDonnees->sNom = (*iter)->getText() ;
    else if ((*iter)->getBalise() == BIRTHDATE)
    	pDonnees->sNaissance = (*iter)->getText() ;
    else if ((*iter)->getBalise() == SEX)
    	pDonnees->sSexe = (*iter)->getText() ;
    else if ((*iter)->getBalise() == PIDS)
    	pDonnees->sNss = (*iter)->getText() ;
    else if ((*iter)->getBalise() == IPP)
        pDonnees->sIpp = (*iter)->getText() ;
    else if ((*iter)->getBalise() == MERGE)
    	pDonnees->sMergedWith = (*iter)->getText() ;
    else if ((*iter)->getBalise() ==  LOCKED)
    	pDonnees->sLockedConsole = (*iter)->getText() ;
  }
}


// -----------------------------------------------------------------------------
// Fonction    : NSPatInfo::NSPatInfo(NSPatient*)
// Description : Constructeur à partir d'un NSPatient
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPatInfo::NSPatInfo(NSPids *pPids)
          :NSRoot(pPids->NSPidsInfo::pContexte)
{
	// Crée l'objet de données
	pDonnees 	   	= new NSPatientData(pContexte) ;

	pCorrespArray 	= new NSPidsArray ;
  pGraphPerson    = new NSPersonGraphManager(pContexte) ;
  pGraphAdr       = new NSAddressGraphManager(pContexte) ;
  sChez           = "" ;

	// Copie les données du NSPids
  pDonnees->sNss = string(pPids->pDonnees->nss) ;
  pDonnees->sNom = string(pPids->pDonnees->nom) ;
  pDonnees->sPrenom = string(pPids->pDonnees->prenom) ;
  pDonnees->sCode = string(pPids->pDonnees->code) ;
  pDonnees->sSexe = string(pPids->pDonnees->sexe) ;
  pDonnees->sNaissance = string(pPids->pDonnees->naissance) ;
  pDonnees->fabriqueNomLong() ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::~NSPatInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPatInfo::~NSPatInfo()
{
	delete pDonnees ;
	delete pCorrespArray ;
	delete pGraphPerson ;
	delete pGraphAdr ;

  // pas de delete du pSuper
}


// -----------------------------------------------------------------------------
// Fonction    : NSPatInfo::NSPatInfo(NSPatInfo& rv)
// Description : Constructeur copie
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPatInfo::NSPatInfo(NSPatInfo& rv)
          :NSRoot(rv.pContexte)
{
try
{
	// Crée l'objet de données
	pDonnees 	   		= new NSPatientData(*(rv.pDonnees)) ;

  pGraphPerson    = new NSPersonGraphManager(*(rv.pGraphPerson)) ;
  pGraphAdr       = new NSAddressGraphManager(*(rv.pGraphAdr)) ;
  sChez           = rv.sChez ;
  pCorrespArray   = new NSPidsArray(*(rv.pCorrespArray)) ;
}
catch (...)
{
  erreur("Exception NSPatInfo copy ctor", standardError, 0) ;
}
}

void
NSPatInfo::fabriqueNomLong(string sLang)
{
	if (sLang == "")
		sLang = pContexte->getUtilisateur()->donneLang() ;

	if (initGraphs())
	{
  	// on fabrique dans ce cas le nom long à partir du graphe
    string sCivilite ;
    if (pGraphPerson->trouveCivilite(sCivilite, sLang))
    	pDonnees->sNomLong = sCivilite ;
    else
    	pDonnees->fabriqueNomLong(sLang) ;
  }
  else
  	// on fabrique le nom long à partir des pDonnees
    pDonnees->fabriqueNomLong(sLang) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::initGraphs()
//  Description:	Initialise les graphes du patient.
//  Retour:			true->les graphes sont initialisés false->echec
//---------------------------------------------------------------------------
bool NSPatInfo::initGraphs()
{
    return true;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::initCorrespArray()
//  Description:	Recherche les correspondants du patient.
//  Retour:			true->le tableau des corresp est initialisé false->echec
//---------------------------------------------------------------------------
bool NSPatInfo::initCorrespArray(NSPatPathoArray* pPatPathoArray)
{
	if ((!pPatPathoArray) || (pPatPathoArray->empty()))
  	return false ;

  pCorrespArray->vider() ;
  //
  // On part du principe que les données qui nous intéressent sont dans un
  // sous chapitre LCTAC (contacts)
  //
  // We suppose that the values we need are in a sub-chapter LCTAC (contact)
  //
  PatPathoIter    iter ;
  string          sElemLex, sSens, sType ;
  bool            bCorresp;

  string          sFonction = "" ;
  string          sCode     = "" ;

  string          sTemp   = "" ;

  iter = pPatPathoArray->begin();
  int iColBase = (*iter)->pDonnees->getColonne();
  iter++;

  while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColBase))
  {
      sElemLex = string((*iter)->pDonnees->lexique);
      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

      // Chapitre "contacts" / Contact chapter
      if (sSens == string("LCTAC"))
      {
          int iColContact = (*iter)->pDonnees->getColonne() ;
          iter++ ;

          while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact))
          {
              sElemLex = string((*iter)->pDonnees->lexique);
              pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

              // Liste des correspondants
              if (sSens == string("LCORR"))
              {
                  iter++;
                  while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact + 1))
                  {
                      sElemLex = string((*iter)->pDonnees->lexique);
                      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

                      // Correspondant : comprend fonction et person_id
                      if (sSens == string("DCORR"))
                      {
                          iter++;
                          bCorresp = false;
                          sFonction = "";
                          sCode = "";

                          while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact + 2))
                          {
                              sElemLex = string((*iter)->pDonnees->lexique);
                              pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

                              // Correspondant : comprend fonction et person_id
                              if (sSens == string("LFONC"))
                              {
                                  iter++;
                                  while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact + 3))
                                  {
                                      // on cherche ici un texte libre
                                      sElemLex = (*iter)->getLexique() ;
                                      if (string(sElemLex, 0, 3) == string("£CL"))
                                      {
                                          sFonction = (*iter)->pDonnees->getTexteLibre();
                                      }
                                      iter++;
                                  }
                              }
                              else if (sSens == string("HHUMA"))
                              {
                                  iter++;
                                  while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColContact + 3))
                                  {
                                      // on cherche ici un texte libre
                                      sElemLex = (*iter)->getLexique() ;
                                      if (string(sElemLex, 0, 5) == string("£SPID"))
                                      {
                                          sCode = string((*iter)->pDonnees->complement);
                                          bCorresp = true;
                                      }
                                      iter++;
                                  }
                              }
                              else
                                  iter++;
                          }

                          if (bCorresp)
                          {
                              NSPidsInfo* pPidsInfo = new NSPidsInfo(pContexte);
                              initCorrespPIDS(pPidsInfo, sCode);
                              pPidsInfo->sFonction = sFonction;

                              pCorrespArray->push_back(new NSPidsInfo(*pPidsInfo));
                              delete pPidsInfo;
                          }
                      }
                      else
                          iter++;
                  }
              }
              else
                  iter++;
          }
      }
      else
          iter++;
  }

	return true ;
}

bool
NSPatInfo::initCorrespPIDS(NSPidsInfo* pPidsInfo, string sPersonID)
{
	//
	// Création d'une fiche d'accès au fichier PIDS
	//
	NSPids* pPids = new NSPids(pContexte, pidsCorresp) ;
	//
	// Ouverture du fichier
	//
	pPids->lastError = pPids->open() ;
	if (pPids->lastError != DBIERR_NONE)
	{
		erreur("Il est impossible d'ouvrir le fichier des PIDS.", standardError, pPids->lastError) ;
		delete pPids ;
		return false ;
	}

    pPids->lastError = pPids->chercheClef(&sPersonID,
                                                "",
                                                0,
                                                keySEARCHEQ,
                                                dbiWRITELOCK);

    if (pPids->lastError != DBIERR_NONE)
    {
        erreur("Impossible de trouver le PIDS du correspondant en cours.", standardError, pPids->lastError) ;
        pPids->close() ;
        delete pPids;
        return false;
    }

    //
    // On récupère l'enregistrement
    //
    pPids->lastError = pPids->getRecord();

    if (pPids->lastError != DBIERR_NONE)
    {
        erreur("Le fichier des PIDS correspondants est endommagé.", standardError, pPids->lastError) ;
        pPids->close() ;
        delete pPids;
        return false;
    }

    // Chargement des données du nouveau PIDS (qui garde les mêmes NSS et RootDoc)
    strcpy(pPidsInfo->pDonnees->nss,        pPids->pDonnees->nss);
    strcpy(pPidsInfo->pDonnees->rootdoc,    pPids->pDonnees->rootdoc);
    strcpy(pPidsInfo->pDonnees->nom,        pPids->pDonnees->nom) ;
    strcpy(pPidsInfo->pDonnees->prenom,     pPids->pDonnees->prenom) ;
    strcpy(pPidsInfo->pDonnees->code,       pPids->pDonnees->code) ;
    strcpy(pPidsInfo->pDonnees->sexe,       pPids->pDonnees->sexe) ;
    strcpy(pPidsInfo->pDonnees->naissance,  pPids->pDonnees->naissance) ;

    pPids->close() ;
    delete pPids ;

    return true ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::operator=(NSPatInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSPatInfo& NSPatInfo::operator=(NSPatInfo src)
{
	if (this == &src)
		return *this ;

	*pDonnees 	   = *(src.pDonnees);
	*pCorrespArray = *(src.pCorrespArray);
	pContexte      = src.pContexte;

	*pGraphPerson  = *(src.pGraphPerson);
	*pGraphAdr     = *(src.pGraphAdr);
	sChez          = src.sChez;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::operator==(NSPatInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSPatInfo::operator == ( NSPatInfo& o )
{
	int egal = 1;

	if (!(*pDonnees 			== *(o.pDonnees)))
		egal = 0;
	if (!(*pCorrespArray 	== *(o.pCorrespArray)))
		egal = 0;

  return egal;
}

//***************************************************************************
//
// Fonction globale StringTitre (utilisée par NSUtilisateur et NSCorrespondant)
// -> Renvoie la string titre en fonction du type (caractère)
//
//***************************************************************************

string StringTitre(char* typeTitre, string sLang)
{
	string titre("");

	if ((typeTitre == NULL) || (typeTitre[0] == '\0'))
		return titre;

	if (sLang == "fr")
	{
		switch (typeTitre[0])
		{
			case 'O':	titre = string("Docteur ");
								break;

			case 'P':	titre = string("Professeur ");
								break;

			case '1':	titre = string("M. le Docteur ");
								break;

			case '2':	titre = string("Mme le Docteur ");
								break;

			case 'R':	titre = string("M. le Professeur ");
								break;

			case 'M':	titre = string ("Mme le Professeur ");
								break;
		}
	}
	else
		if (sLang == "en")
		{
			switch (typeTitre[0])
			{
				case 'O':	titre = string("Doctor ");
									break;

				case 'P':	titre = string("Professor ");
									break;

				case '1':	titre = string("M. the Doctor ");
									break;

				case '2':	titre = string("Mrs the Doctor ");
									break;

				case 'R':	titre = string("M. the Professor ");
									break;

				case 'M':	titre = string ("Mrs the Professor ");
									break;
				}
    }

	return titre;
}

//***************************************************************************
//
// Fonction globale StringAvantTitre (utilisée par NSUtilisateur et NSCorrespondant)
// -> Renvoie la string avant titre en fonction du type (caractère)
//
//***************************************************************************

string StringAvantTitre(char* typeTitre, string sLang)
{
	string avant("");

	if ((typeTitre == NULL) || (typeTitre[0] == '\0'))
		return avant;

	if (sLang == "fr")
	{
		switch (typeTitre[0])
		{
			case 'O':
			case 'P':	avant = string("au");
								break;

			default:	avant = string("à");
		}
	}

	return avant;
}

//***************************************************************************
//
// Implémentation des méthodes NSUtilisateurData
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSUtilisateurData::NSUtilisateurData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSUtilisateurData::NSUtilisateurData()
{
	metAZero();
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::NSUtilisateurData(NSUtilisateurData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSUtilisateurData::NSUtilisateurData(NSUtilisateurData& rv)
{
	strcpy(nss,   	   rv.nss) ;
	strcpy(login,   	 rv.login) ;
	strcpy(code,   	   rv.code) ;
	strcpy(sexe,   	   rv.sexe) ;
	strcpy(nom, 		   rv.nom) ;
	strcpy(prenom, 	   rv.prenom) ;
	strcpy(type, 	     rv.type) ;
	strcpy(messagerie, rv.messagerie) ;
	strcpy(lang,	     rv.lang) ;

	sMetier     = rv.sMetier ;
	sSpecialite = rv.sSpecialite ;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSUtilisateurData::metAZero()
{
	memset(nss,        0, UTI_NSS_LEN + 1);
	memset(login,      0, UTI_LOGIN_LEN + 1);
	memset(code,       0, UTI_CODE_LEN + 1);
	memset(sexe,       0, UTI_SEXE_LEN + 1);
	memset(nom,        0, UTI_NOM_LEN + 1);
	memset(prenom,     0, UTI_PRENOM_LEN + 1);
	memset(type,       0, UTI_TYPE_LEN + 1);
	memset(messagerie, 0, UTI_MESSAGERIE_LEN + 1);
	memset(lang, 	     0, UTI_LANG_LEN + 1);

	sMetier     = "" ;
	sSpecialite = "" ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::operator=(NSUtilisateurData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSUtilisateurData&
NSUtilisateurData::operator=(NSUtilisateurData src)
{
	if (this == &src)
		return *this ;

	strcpy(nss,   	   src.nss) ;
	strcpy(login,   	 src.login) ;
	strcpy(code,   	   src.code) ;
	strcpy(sexe,   	   src.sexe) ;
	strcpy(nom, 		   src.nom) ;
	strcpy(prenom, 	   src.prenom) ;
	strcpy(type, 	     src.type) ;
	strcpy(messagerie, src.messagerie) ;
	strcpy(lang,	     src.lang) ;

	sMetier     = src.sMetier ;
	sSpecialite = src.sSpecialite ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::operator==(NSUtilisateurData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSUtilisateurData::operator == ( NSUtilisateurData& o )
{
	if ((strcmp(nss, o.nss) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::donneTitre()
//  Description:	renvoie le titre de l'utilisateur en fonction des donnees
//  Retour:			la string contenant le titre
//---------------------------------------------------------------------------
string
NSUtilisateurData::donneTitre(string sLang)
{
	string titre("");

	if ((type[0] == 'O') || (type[0] == 'P'))
		titre = string("le ");

	titre = titre + StringTitre(type, sLang);

	if (strcmp(prenom, ""))
		titre = titre + string(prenom) + string(" ");

	titre += string(nom);

  return titre;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::donneAvantTitre()
//  Description:	renvoie l'avant-titre (au ou à) de l'utilisateur en fonction des donnees
//  Retour:			la string contenant l'avant-titre
//---------------------------------------------------------------------------

string
NSUtilisateurData::donneAvantTitre(string sLang)
{
	return StringAvantTitre(type, sLang) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::donneSignature()
//  Description:	renvoie le titre de l'utilisateur en fonction des donnees
//  Retour:			la string contenant le titre signature
//---------------------------------------------------------------------------

string
NSUtilisateurData::donneSignature(string sLang)
{
	string titre("") ;

	titre = StringTitre(type) ;

	if (strcmp(prenom, ""))
		titre = titre + string(prenom) + string(" ") ;

	titre += string(nom) ;

  return titre ;
}

//***************************************************************************
//
// Implémentation des méthodes NSUtiliInfo
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSUtiliInfo::NSUtiliInfo(NSContexte* pCtx)
            :NSRoot(pCtx)
{
	// Crée l'objet de données
	pDonnees   	 = new NSUtilisateurData() ;
	pGraphPerson = new NSPersonGraphManager(pContexte) ;

	sTitre     = "" ;
	sCivilProf = "" ;
	sCivil     = "" ;

	sLogin = "" ;
	sPasswd = "" ;
	sUserType = "" ;
	sPasswordType = "" ;
	sDatePasswordCreation = "" ;
	sValidityDuration = "" ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtiliInfo::NSUtiliInfo(NSBasicAttributeArray* pAttribute)
//  Description:	Constructeur avec NSBasicAttributeArray
//                  (pour les accès aux bases a l'aide du pilot)
//  Retour:			Rien
//---------------------------------------------------------------------------
NSUtiliInfo::NSUtiliInfo(NSBasicAttributeArray* pAttribute, NSContexte* pCtx): NSRoot(pCtx)
{
	// Crée l'objet de données
	pDonnees 	   = new NSUtilisateurData() ;
	pGraphPerson = new NSPersonGraphManager(pContexte) ;

  sTitre = "";
  sCivilProf = "";
  sCivil = "";
  sLogin = "";
  sPasswd = "";
  sUserType = "";
  sPasswordType = "";
  sDatePasswordCreation = "";
  sValidityDuration = "";

	NSBasicAttributeIter iter = pAttribute->begin() ;

	for (iter; iter != pAttribute->end(); iter++)
	{
    if((*iter)->getBalise() == FIRST_NAME)
    	strcpy(pDonnees->prenom, ((*iter)->getText()).c_str()) ;
    else if((*iter)->getBalise() == LAST_NAME)
    	strcpy(pDonnees->nom, ((*iter)->getText()).c_str()) ;
    else if((*iter)->getBalise() == SEX)
    	strcpy(pDonnees->sexe, ((*iter)->getText()).c_str()) ;
    else if((*iter)->getBalise() == PIDS)
    	strcpy(pDonnees->nss, ((*iter)->getText()).c_str()) ;
    else if((*iter)->getBalise() == TITLE_CODE)
    {
    	string sCivilite = (*iter)->getText() ;

      if (sCivilite == string("HDOCT1"))
      	strcpy(pDonnees->type, "O") ;
      else if (sCivilite == string("HPROF1"))
      	strcpy(pDonnees->type, "P") ;
      else if (sCivilite == string("HMOND1"))
      	strcpy(pDonnees->type, "1") ;
      else if (sCivilite == string("HMADD1"))
      	strcpy(pDonnees->type, "2") ;
      else if (sCivilite == string("HMONF1"))
      	strcpy(pDonnees->type, "R") ;
      else if (sCivilite == string("HMADP1"))
      	strcpy(pDonnees->type, "M") ;
      else if (sCivilite == string("HMONP1"))
      	strcpy(pDonnees->type, "A") ;
      else if (sCivilite == string("HMADR1"))
      	strcpy(pDonnees->type, "B") ;
      else if (sCivilite == string("HMADE1"))
      	strcpy(pDonnees->type, "C") ;
    }
    else if((*iter)->getBalise() == PROFESSION)
    	pDonnees->sMetier = (*iter)->getText() ;
    else if((*iter)->getBalise() == SPECIALITY)
    	pDonnees->sSpecialite = (*iter)->getText() ;
	}
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSUtiliInfo::NSUtiliInfo(NSUtiliInfo& rv)
            :NSRoot(rv.pContexte)
{
	//
	// Crée l'objet de données
	//
	pDonnees 	  	= new NSUtilisateurData() ;
	//
	// Copie les valeurs du NSUtiliInfo d'origine
	//
	*pDonnees     = *(rv.pDonnees) ;

	pGraphPerson  = new NSPersonGraphManager(*(rv.pGraphPerson)) ;

  sTitre                  = rv.sTitre ;
  sCivilProf              = rv.sCivilProf ;
  sCivil                  = rv.sCivil ;
  sLogin                  = rv.sLogin ;
  sPasswd                 = rv.sPasswd ;
  sUserType               = rv.sUserType ;
  sPasswordType           = rv.sPasswordType ;
  sDatePasswordCreation   = rv.sDatePasswordCreation ;
  sValidityDuration       = rv.sValidityDuration ;
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSUtilisateurChoisi*
//---------------------------------------------------------------------------
NSUtiliInfo::NSUtiliInfo(NSUtilisateurChoisi* pUtilChoisi)
						:NSRoot(pUtilChoisi->NSUtiliInfo::pContexte)
{
	//
	// Crée l'objet de données
	//
	pDonnees 	  = new NSUtilisateurData() ;
	//
	// Copie les valeurs du NSUtilisateur
	//
	*pDonnees     = *(pUtilChoisi->pDonnees) ;

	pGraphPerson = new NSPersonGraphManager(*(pUtilChoisi->pGraphPerson)) ;

  sTitre                  = pUtilChoisi->sTitre ;
  sCivilProf              = pUtilChoisi->sCivilProf ;
  sCivil                  = pUtilChoisi->sCivil ;
  sLogin                  = pUtilChoisi->sLogin ;
  sPasswd                 = pUtilChoisi->sPasswd ;
  sUserType               = pUtilChoisi->sUserType ;
  sPasswordType           = pUtilChoisi->sPasswordType ;
  sDatePasswordCreation   = pUtilChoisi->sDatePasswordCreation ;
  sValidityDuration       = pUtilChoisi->sValidityDuration ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSUtiliInfo::~NSUtiliInfo()
{
	delete pDonnees ;
	if (pGraphPerson)
		delete pGraphPerson ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtiliInfo::initAdresseInfo()
//  Description:	Recherche l'adresse de l'utilisateur.
//  Retour:			true->l'adresse est initialisée false->echec
//---------------------------------------------------------------------------
bool
NSUtiliInfo::initAdresseInfo()
{
	return true ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtiliInfo::operator=(NSUtiliInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSUtiliInfo&
NSUtiliInfo::operator=(NSUtiliInfo src)
{
	if (this == &src)
		return *this ;

	*pDonnees 	  = *(src.pDonnees) ;
	*pGraphPerson = *(src.pGraphPerson) ;

  sTitre                = src.sTitre ;
  sCivilProf            = src.sCivilProf ;
  sCivil                = src.sCivil ;
  sLogin                = src.sLogin ;
  sPasswd               = src.sPasswd ;
  sUserType             = src.sUserType ;
  sPasswordType         = src.sPasswordType ;
  sDatePasswordCreation = src.sDatePasswordCreation ;
  sValidityDuration     = src.sValidityDuration ;

  pContexte = src.pContexte ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtiliInfo::operator==(NSUtiliInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSUtiliInfo::operator == ( NSUtiliInfo& o )
{
	int egal = 1 ;

	if (!(*pDonnees == *(o.pDonnees)))
  	egal = 0 ;

	return egal ;
}

bool
NSUtiliInfo::pwdNeedChange()
{
	if (sPasswordType == "T")
		return true ;
	else if (sPasswordType == "F")
		return false ;
	else if (sPasswordType == "J")
	{
    // Calcul de la date du jour et de la date d'expiration
    char   szDateJour[10] ;
    string sDateExpiration ;

    donne_date_duJour(szDateJour) ;
    sDateExpiration = sDatePasswordCreation ;

    for (int i = 0; i < atoi(sValidityDuration.c_str()); i++)
    	incremente_date(sDateExpiration) ;

    if (sDateExpiration < string(szDateJour))
    	return true ;

    return false ;
	}

	return false ;
}

bool
NSUtiliInfo::changePassword()
{
	char szDateJour[10] ;
	donne_date_duJour(szDateJour) ;

	::MessageBox(0, "Votre mot de passe a expiré. Vous devez saisir un nouveau mot de passe.", "Message Nautilus", MB_OK) ;

    /********************************************
    // modification d'un code utilisateur
    // on ne contrôle pas l'ancien mot de passe pour l'administrateur
    // Saisie du nouveau mot de passe
    CodeUtilisateurDialog *pCodeDlg = new CodeUtilisateurDialog(this, "", pContexte) ;

    if (pCodeDlg->Execute() == IDCANCEL)
    {
        delete pCodeDlg ;
        return false;
    }

    sPasswd = pCodeDlg->sCodeUtil ;
    sDatePasswordCreation = string(szDateJour);
    delete pCodeDlg ;
    **********************************************/

	return true ;
}

//***************************************************************************
//
// Implémentation des méthodes NSCorrespondantData
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSCorrespondantData::NSCorrespondantData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSCorrespondantData::NSCorrespondantData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}


//---------------------------------------------------------------------------
//  Function:    NSCorrespondantData::metABlanc()
//
//  Description: Met à blanc les variables.
//---------------------------------------------------------------------------
void NSCorrespondantData::metAZero()
{
	memset(code,        0, 	COR_CODE_LEN + 1);
	memset(nom,         0, 	COR_NOM_LEN + 1);
	memset(prenom,      0, 	COR_PRENOM_LEN + 1);
	memset(sexe,        0, 	COR_SEXE_LEN + 1);
	memset(adresse,     0, 	COR_ADRESSE_LEN + 1);
	memset(docteur,     0, 	COR_DOCTEUR_LEN + 1);
	memset(telepor, 		0, 	COR_TELEPOR_LEN + 1);
	memset(telebur,	    0, 	COR_TELEBUR_LEN + 1);
	memset(nb_exemp,    0, 	COR_NB_EXEMP_LEN + 1);
	memset(messagerie,	0, 	COR_MESSAGERIE_LEN + 1);
	memset(fonction, 		0, 	COR_FONCTION_LEN + 1);
	memset(actif, 			0, 	COR_ACTIF_LEN + 1);
	memset(lang, 				0, 	COR_LANG_LEN + 1);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantData::NSCorrespondantData(NSCorrespondantData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCorrespondantData::NSCorrespondantData(NSCorrespondantData& rv)
{
	strcpy(code,   	    rv.code);
	strcpy(nom, 				rv.nom);
	strcpy(prenom, 	    rv.prenom);
	strcpy(sexe, 				rv.sexe);
	strcpy(adresse,     rv.adresse);
	strcpy(docteur, 		rv.docteur);
	strcpy(telepor, 		rv.telepor);
	strcpy(telebur,     rv.telebur);
	strcpy(nb_exemp, 		rv.nb_exemp);
	strcpy(messagerie,	rv.messagerie);
	strcpy(fonction,		rv.fonction);
	strcpy(actif,				rv.actif);
	strcpy(lang,				rv.lang);
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantData::operator=(NSCorrespondantData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSCorrespondantData&
NSCorrespondantData::operator=(NSCorrespondantData src)
{
	if (this == &src)
		return *this ;

	strcpy(code,   	    src.code);
	strcpy(nom, 				src.nom);
	strcpy(prenom, 	    src.prenom);
	strcpy(sexe, 				src.sexe);
	strcpy(adresse,     src.adresse);
	strcpy(docteur, 		src.docteur);
	strcpy(telepor, 		src.telepor);
	strcpy(telebur,     src.telebur);
	strcpy(nb_exemp, 		src.nb_exemp);
	strcpy(messagerie,	src.messagerie);
	strcpy(fonction,		src.fonction);
	strcpy(actif,				src.actif);
	strcpy(lang,				src.lang);

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantData::operator==(NSCorrespondantData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSCorrespondantData::operator == ( NSCorrespondantData& o )
{
	if ((strcmp(code,   	  	o.code) 			== 0) &&
			(strcmp(nom, 		  		o.nom) 				== 0) &&
			(strcmp(prenom, 	  	o.prenom) 		== 0) &&
			(strcmp(sexe, 	  		o.sexe) 			== 0) &&
			(strcmp(adresse,   		o.adresse) 		== 0) &&
			(strcmp(docteur,   		o.docteur) 		== 0) &&
			(strcmp(telepor,   		o.telepor) 		== 0) &&
			(strcmp(telebur,   		o.telebur) 		== 0) &&
			(strcmp(nb_exemp,  		o.nb_exemp) 	== 0) &&
			(strcmp(messagerie,		o.messagerie)	== 0) &&
			(strcmp(fonction,  		o.fonction)		== 0) &&
			(strcmp(actif,  	  	o.actif)			== 0)
			)
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantData::donneTitre()
//  Description:	renvoie le titre du correspondant en fonction des donnees
//  Retour:			la string contenant le titre
//---------------------------------------------------------------------------
string
NSCorrespondantData::donneTitre(string sLang)
{
	string titre("");

	titre = StringTitre(docteur, sLang);

	if (strcmp(prenom, ""))
		titre = titre + string(prenom) + string(" ");

	titre += string(nom);

	return titre;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantData::donneAvantTitre()
//  Description:	renvoie l'avant-titre (au ou à) du corresp en fonction des donnees
//  Retour:			la string contenant l'avant-titre
//---------------------------------------------------------------------------
string
NSCorrespondantData::donneAvantTitre(string sLang)
{
  return StringAvantTitre(docteur, sLang);
}

//***************************************************************************
//
// Implémentation des méthodes NSCorrespondantInfo
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::NSCorrespondantInfo(NSSuper* pSuper)
//  Description:	Constructeur avec superviseur pour l'accès aux bases
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCorrespondantInfo::NSCorrespondantInfo(NSContexte* pCtx)
                    :NSRoot(pCtx)
{
	// Crée l'objet de données
	pDonnees     = new NSUtilisateurData() ;
	pGraphPerson = new NSPersonGraphManager(pContexte) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::~NSCorrespondantInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCorrespondantInfo::~NSCorrespondantInfo()
{
	delete pDonnees ;
	delete pGraphPerson ;
	// pas de delete de pSuper
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::NSCorrespondantInfo(NSCorrespondantInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCorrespondantInfo::NSCorrespondantInfo(NSCorrespondantInfo& rv)
                    :NSRoot(rv.pContexte)
{
	//
	// Crée l'objet de données
	//
	pDonnees 	   = new NSUtilisateurData() ;
	pGraphPerson = new NSPersonGraphManager(pContexte) ;
	//
	// Copie les valeurs du NSCorrespondantInfo d'origine
	//
	*pDonnees 		= *(rv.pDonnees) ;
	*pGraphPerson = *(rv.pGraphPerson) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::initAdresseInfo()
//  Description:	Recherche l'adresse du Correspondant.
//  Retour:			true->l'adresse est initialisée false->echec
//---------------------------------------------------------------------------
bool NSCorrespondantInfo::initAdresseInfo()
{
	return false ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::operator=(NSCorrespondantInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSCorrespondantInfo&
NSCorrespondantInfo::operator=(NSCorrespondantInfo src)
{
	if (this == &src)
		return *this ;

	*pDonnees 	  = *(src.pDonnees) ;
	*pGraphPerson = *(src.pGraphPerson) ;
	pContexte     = src.pContexte ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::operator==(NSCorrespondantInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int
NSCorrespondantInfo::operator == ( NSCorrespondantInfo& o )
{
	int egal = 1 ;

	if (!(*pDonnees == *(o.pDonnees)))
		egal = 0 ;

	return egal ;
}

bool
NSCorrespondantInfo::GetDataFromGraph(NSPatPathoArray* pPPT)
{
	PatPathoIter    iter ;
	string          sElemLex, sSens, sType, sTemp ;
	string lang = "";

	NSPatPathoArray* pPatPathoArray = new NSPatPathoArray(pContexte);

	if (pPPT)
	{
		if (pPPT->empty())
		{
    	delete pPatPathoArray ;
			return false ;
		}

		*pPatPathoArray = *pPPT;
	}
	else
	{
		// on initialise la patpatho à partir du graphe
		NSDataTreeIter iterTree;
		NSDataGraph* pDataGraph = pGraphPerson->pDataGraph;

        if (pDataGraph->aTrees.ExisteTree("ZADMI1", pContexte, &iterTree))
        {
            (*iterTree)->getPatPatho(pPatPathoArray);
        }
        else
        {
            char msg[255];
            strcpy(msg, "Impossible de trouver l'arbre [ZADMI1] dans le graphe administratif.");
            erreur(msg, standardError, 0) ;
            return false;
        }

        if (pDataGraph->aTrees.ExisteTree("DPROS1", pContexte, &iterTree))
        {
            NSPatPathoArray* pPatPathoPDS = new NSPatPathoArray(pContexte);
            (*iterTree)->getPatPatho(pPatPathoPDS);
            pPatPathoArray->InserePatPatho(pPatPathoArray->end(), pPatPathoPDS, 0);
            delete pPatPathoPDS;
        }
    }

//=============================================================================
//il faut recuperer la langue d'utilisateur de la base ou de l'interface
//Voir dans la racine du DPROS!!!!
//=============================================================================
    string sLang  = pContexte->getUtilisateur()->donneLang() ;

    string sNom    = "" , sPrenom = "" , sSexe   = "" ;
    string sCivilite = "", sEMail = "";
    string sMetier = "", sSpecialite = "";

    iter = pPatPathoArray->begin();
    int iColBase = (*iter)->pDonnees->getColonne();

    while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() >= iColBase))
    {
        sElemLex = string((*iter)->pDonnees->lexique);
        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

        if (sSens == string("ZADMI"))
        {
            iter++;

            while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColBase))
            {
                sElemLex = string((*iter)->pDonnees->lexique);
                pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

                // Chapitre "identité" / Identity chapter
                if (sSens == string("LIDET"))
                {
                    int iColIdentity = (*iter)->pDonnees->getColonne() ;
                    iter++ ;

                    while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColIdentity))
                    {
                        sElemLex = string((*iter)->pDonnees->lexique);
                        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

                        // Nom de l'utilisateur
                        if ((sSens == string("LNOM0")) && (sNom == ""))
                        {
                            iter++;
                            while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColIdentity+1))
                            {
                                // on cherche ici un texte libre
                                sElemLex = (*iter)->getLexique() ;
                                if (string(sElemLex, 0, 3) == string("£CL"))
                                {
                                    sNom = (*iter)->pDonnees->getTexteLibre();
                                }
                                iter++;
                            }

                            if (sNom != "")
                                strcpy(pDonnees->nom, sNom.c_str());
                        }
                        // Prénom de l'utilisateur
                        else if ((sSens == string("LNOM2")) && (sPrenom == ""))
                        {
                            iter++;
                            while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColIdentity+1))
                            {
                                // on cherche ici un texte libre
                                sElemLex = (*iter)->getLexique() ;
                                if (string(sElemLex, 0, 3) == string("£CL"))
                                {
                                    sPrenom = (*iter)->pDonnees->getTexteLibre();
                                }
                                iter++;
                            }

                            if (sPrenom != "")
                                strcpy(pDonnees->prenom, sPrenom.c_str());
                        }
                        // Sexe de l'utilisateur
                        else if (sSens == string("LSEXE"))
                        {
                            iter++;
                            while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColIdentity+1))
                            {
                                // on cherche ici un code lexique
                                sElemLex = (*iter)->getLexique() ;
                                pContexte->getDico()->donneCodeSens(&sElemLex, &sTemp) ;

                                if      (sTemp == string("HMASC"))
                                    sSexe = "1";
                                else if (sTemp == string("HFEMI"))
                                    sSexe = "2";

                                iter++;
                            }

                            if (sSexe != "")
                                strcpy(pDonnees->sexe, sSexe.c_str());
                        }
                        else
                            iter++;
                    }
                }
                else
                    iter++;
            } // while
        } // if (sSens == string("ZADMI"))
        else if (sSens == string("DPROS"))
        {
            iter++;

            while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColBase))
            {
                sElemLex = string((*iter)->pDonnees->lexique);
                pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);
 /*=============================================================================
  A ajouter!!!!!
  - le code pour la langue d'utilisateur (non la langue maternelle)
                 if (sSens == string("........"))
                 {
                    sLang = string((*iter)->pDonnees->lexique) ;
                 }
                 else
 =============================================================================*/
                 //la langue par defaut = "fr"
                  lang = "fr";

                 // Chapitre "Compétences" / Competence chapter
                if (sSens == string("LCOMP"))
                {
                    int iColCompetences = (*iter)->pDonnees->getColonne() ;
                    iter++ ;

                    while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColCompetences))
                    {
                        sElemLex = string((*iter)->pDonnees->lexique);
                        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

                        // metier
                        if ((sSens == string("LMETI")) && (sMetier == ""))
                        {
                            // on cherche ici un edit lexique (ayant comme fils un code lexique)
                            iter++;
                            sElemLex = (*iter)->getLexique() ;
                            if( sElemLex != "")
                                if (string(sElemLex, 0, 3) == string("£C;"))
                                {
                                    iter++;
                                    while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColCompetences+2))
                                    {
                                        // on cherche ici un code lexique pour un libelle
                                        string sCode = string((*iter)->pDonnees->lexique) ;
                                        pContexte->getDico()->donneLibelle(sLang, &sCode, &sMetier) ;

                                        iter++;
                                    }
                                }
                                else
                                {
                                    sMetier = string((*iter)->pDonnees->lexique) ;
                                    if(iter != pPatPathoArray->end())
                                        iter++;
                                }

                            else
                                iter++;

                            if (sMetier != "")
                                pDonnees->sMetier = sMetier;
                        }
                        // Specilité de l'utilisateur
                        else if ((sSens == string("LSPEC")) && (sSpecialite == ""))
                        {
                            // on cherche ici un edit lexique (ayant comme fils un code lexique)
                            iter++;
                            sElemLex = (*iter)->getLexique() ;
                            if( sElemLex != "")
                                if (string(sElemLex, 0, 3) == string("£C;"))
                                {
                                    iter++;
                                    while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColCompetences+2))
                                    {
                                        // on cherche ici un code lexique pour un libelle
                                        string sCode = string((*iter)->pDonnees->lexique) ;
                                        pContexte->getDico()->donneLibelle(sLang, &sCode, &sSpecialite) ;

                                        iter++;
                                    }
                                }
                                else
                                {
                                    sSpecialite = string((*iter)->pDonnees->lexique) ;
                                    if(iter != pPatPathoArray->end())
                                        iter++;
                                }
                            else    //si "" on avanse
                                iter++;

                            if (sSpecialite != "")
                                pDonnees->sSpecialite = sSpecialite;
                        }
                        else
                            iter++;
                    }
                }
                // Chapitre "civilité"
                else if (sSens == string("HCIVO"))
                {
                    int iColCivilite = (*iter)->pDonnees->getColonne() ;
                    iter++;
                    while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColCivilite))
                    {
                        // on cherche ici un code lexique pour un libelle
                        sCivilite = string((*iter)->pDonnees->lexique) ;
                        // pContexte->getDico()->donneLibelle(sLang, &sCodeLex, &sCivilite) ;

                        iter++;
                    }

                    if (sCivilite != "")
                    {
                        if (sCivilite == string("HDOCT1"))
                            strcpy(pDonnees->type, "O");
                        else if (sCivilite == string("HPROF1"))
                            strcpy(pDonnees->type, "P");
                        else if (sCivilite == string("HMOND1"))
                            strcpy(pDonnees->type, "1");
                        else if (sCivilite == string("HMADD1"))
                            strcpy(pDonnees->type, "2");
                        else if (sCivilite == string("HMONF1"))
                            strcpy(pDonnees->type, "R");
                        else if (sCivilite == string("HMADP1"))
                            strcpy(pDonnees->type, "M");
                        else if (sCivilite == string("HMONP1"))
                            strcpy(pDonnees->type, "A");
                        else if (sCivilite == string("HMADR1"))
                            strcpy(pDonnees->type, "B");
                        else if (sCivilite == string("HMADE1"))
                            strcpy(pDonnees->type, "C");
                    }
                }
                // Chapitre "lieu d'exercice"
                else if (sSens == string("ULIEX"))
                {
                    int iColLiex = (*iter)->pDonnees->getColonne() ;
                    iter++ ;

                    while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColLiex))
                    {
                        sElemLex = string((*iter)->pDonnees->lexique);
                        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens);

                        // Nom de l'utilisateur
                        if ((sSens == string("LMAIL")) && (sEMail == ""))
                        {
                            iter++;
                            while ((iter != pPatPathoArray->end()) && ((*iter)->pDonnees->getColonne() > iColLiex+1))
                            {
                                // on cherche ici un texte libre
                                sElemLex = (*iter)->getLexique() ;
                                if (string(sElemLex, 0, 3) == string("£CL"))
                                {
                                    sEMail = (*iter)->pDonnees->getTexteLibre();
                                }
                                iter++;
                            }

                            if (sEMail != "")
                                strcpy(pDonnees->messagerie, sEMail.c_str());
                        }
                        else
                            iter++;
                    } // while
                }
                else
                    iter++;
            } // while
		} // else if (sSens == string("DPROS"))
		else
			iter++ ;
	} // while de base

	delete pPatPathoArray ;

	return true ;
}

//////////////////////// fin de nsperson.cpp //////////////////////////////////

