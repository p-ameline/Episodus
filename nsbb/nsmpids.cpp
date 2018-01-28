// -----------------------------------------------------------------------------
// nsmpids.cpp// -----------------------------------------------------------------------------
// Base contenant l'identifiant patient dans le modele MUE
// -----------------------------------------------------------------------------
// $Revision: 1.19 $
// $Author: pameline $
// $Date: 2016/12/29 16:22:29 $
// -----------------------------------------------------------------------------
// FLP - octobre 2004
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#include "nsbb\nsmpids.h"
#include "partage\nsdivfct.h"
#include "nsbb\nsmanager.h"
#include "nautilus\nssuper.h"
#include "nsbb\nsarc.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsdefArch.h"
#include "nsbb\nspardlg.h"
#include "nssavoir\nsgraphe.h"
#include "nsepisod\nsToDo.h"

#include "pilot\NautilusPilot.hpp"
#include "nsbb\tagNames.h"

//***************************************************************************
// Implémentation des méthodes NSPids
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSPidsData::NSPidsData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSPidsData::NSPidsData()
{
	metAZero();
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsData::NSPidsData(NSPidsData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPidsData::NSPidsData(const NSPidsData& rv)
{
	strcpy(nss,								rv.nss);
    strcpy(rootdoc,                         rv.rootdoc);
	strcpy(nom,								rv.nom);
	strcpy(prenom,							rv.prenom);
	strcpy(code,							rv.code);
	strcpy(sexe,							rv.sexe);
	strcpy(naissance,						rv.naissance);
    strcpy(login,                           rv.login);
    strcpy(passwd,                          rv.passwd);
    strcpy(nb_exemp,                        rv.nb_exemp);
    strcpy(messagerie,                      rv.messagerie);
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsData::operator=(NSPidsData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSPidsData&
NSPidsData::operator=(const NSPidsData& src)
{
	if (this == &src)
		return *this ;

	strcpy(nss, 	  					src.nss);
    strcpy(rootdoc,                     src.rootdoc);
	strcpy(nom, 	  					src.nom);
	strcpy(prenom, 	  					src.prenom);
	strcpy(code,   	  					src.code);
	strcpy(sexe,      					src.sexe);
	strcpy(naissance, 					src.naissance);
    strcpy(login,                       src.login);
    strcpy(passwd,                      src.passwd);
    strcpy(nb_exemp,                    src.nb_exemp);
    strcpy(messagerie,                  src.messagerie);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsData::operator==(NSPidsData src)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSPidsData::operator==(const NSPidsData& o)
{
	if ((strcmp(nss, 		  							o.nss) 							== 0) &&
      (strcmp(rootdoc,                            o.rootdoc)                      == 0) &&
			(strcmp(nom, 		  						o.nom) 							== 0) &&
			(strcmp(prenom, 	  						o.prenom) 						== 0) &&
			(strcmp(code,   	  						o.code) 						== 0) &&
			(strcmp(sexe,      							o.sexe) 						== 0) &&
			(strcmp(naissance, 							o.naissance) 					== 0)
			)
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Function:    NSPidsData::metAZero()
//
//  Description: Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSPidsData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(nss,       					0,	PIDS_NSS_LEN + 1);
    memset(rootdoc,                     0,  PIDS_ROOTDOC_LEN + 1);
	memset(nom,       					0, 	PIDS_NOM_LEN + 1);
	memset(prenom,    					0, 	PIDS_PRENOM_LEN + 1);
	memset(code,      					0, 	PIDS_CODE_LEN + 1);
	memset(sexe,      					0, 	PIDS_SEXE_LEN + 1);
	memset(naissance, 					0, 	PIDS_NAISSANCE_LEN + 1);
    memset(login,                       0,  PIDS_LOGIN_LEN + 1);
    memset(passwd,                      0,  PIDS_PASSWD_LEN + 1);
    memset(nb_exemp,                    0,  PIDS_NB_EXEMP_LEN + 1);
    memset(messagerie,                  0,  PIDS_MESSAGERIE_LEN + 1);
}

//---------------------------------------------------------------------------
//  Function:    NSPidsData::metABlanc()
//
//  Description: Met à blanc les variables de la fiche
//---------------------------------------------------------------------------
void
NSPidsData::metABlanc()
{
	//
	// Met les champs de données à blanc
	//
	memset(nss,       					' ', 	PIDS_NSS_LEN);
    memset(rootdoc,                     ' ',    PIDS_ROOTDOC_LEN);
	memset(nom,       					' ', 	PIDS_NOM_LEN);
	memset(prenom,    					' ', 	PIDS_PRENOM_LEN);
	memset(code,      					' ', 	PIDS_CODE_LEN);
	memset(sexe,      					' ', 	PIDS_SEXE_LEN);
	memset(naissance, 					' ', 	PIDS_NAISSANCE_LEN);
    memset(login,                       ' ',    PIDS_LOGIN_LEN);
    memset(passwd,                      ' ',    PIDS_PASSWD_LEN);
    memset(nb_exemp,                    ' ',    PIDS_NB_EXEMP_LEN + 1);
    memset(messagerie,                  ' ',    PIDS_MESSAGERIE_LEN + 1);
}

/*****************************************************************************/
// 							METHODES DE NSPids
/*****************************************************************************/

//---------------------------------------------------------------------------
//  Function:    NSPids::NSPids(NSSuper* pSuper)
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSPids::NSPids(NSContexte* pCtx, PIDSTYPE iType)
       :NSPidsInfo(pCtx), NSFiche(pCtx->getSuperviseur())
{
	iTypePids = iType ;
}

//---------------------------------------------------------------------------
//  Function:  NSPids::~NSPids()
//
//  Arguments:
//
//  Description:
//             Destructeur.
//             Ferme proprement une classe de ce type.
//  Returns:
//             Aucun
//---------------------------------------------------------------------------
NSPids::~NSPids()
{
}

//---------------------------------------------------------------------------
//  Function:  NSPids::alimenteFiche()
//
//  Arguments:
//
//  Description:
//             Transfère le contenu de pRecBuff dans les variables de
//             la fiche
//  Returns:
//             Rien
//---------------------------------------------------------------------------
void NSPids::alimenteFiche()
{
	alimenteChamp(pDonnees->nss,       PIDS_NSS_FIELD,		 		PIDS_NSS_LEN);
  alimenteChamp(pDonnees->rootdoc,   PIDS_ROOTDOC_FIELD,         PIDS_ROOTDOC_LEN);
	alimenteChamp(pDonnees->nom,       PIDS_NOM_FIELD,		 		PIDS_NOM_LEN);
	alimenteChamp(pDonnees->prenom, 	 PIDS_PRENOM_FIELD,	 		PIDS_PRENOM_LEN);
	alimenteChamp(pDonnees->code, 		 PIDS_CODE_FIELD,		 	PIDS_CODE_LEN);
	alimenteChamp(pDonnees->sexe,      PIDS_SEXE_FIELD,		 	PIDS_SEXE_LEN);
	alimenteChamp(pDonnees->naissance, PIDS_NAISSANCE_FIELD,	    PIDS_NAISSANCE_LEN);
  if (iTypePids == pidsUtilisat)
  {
    alimenteChamp(pDonnees->login, 			PIDS_LOGIN_FIELD,		 	PIDS_LOGIN_LEN);
    alimenteChamp(pDonnees->passwd,	        PIDS_PASSWD_FIELD,	        PIDS_PASSWD_LEN);
  }
}

//---------------------------------------------------------------------------
//  Function:  NSPids::videFiche()
//
//  Description: Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void NSPids::videFiche()
{
	videChamp(pDonnees->nss, 			PIDS_NSS_FIELD,		 		PIDS_NSS_LEN);
    videChamp(pDonnees->rootdoc,        PIDS_ROOTDOC_FIELD,         PIDS_ROOTDOC_LEN);
	videChamp(pDonnees->nom, 			PIDS_NOM_FIELD,		 		PIDS_NOM_LEN);
	videChamp(pDonnees->prenom, 		PIDS_PRENOM_FIELD,	 		PIDS_PRENOM_LEN);
	videChamp(pDonnees->code, 			PIDS_CODE_FIELD,		 	PIDS_CODE_LEN);
	videChamp(pDonnees->sexe, 			PIDS_SEXE_FIELD,		 	PIDS_SEXE_LEN);
	videChamp(pDonnees->naissance,	    PIDS_NAISSANCE_FIELD,	    PIDS_NAISSANCE_LEN);
    if (iTypePids == pidsUtilisat)
    {
        videChamp(pDonnees->login, 			PIDS_LOGIN_FIELD,		 	PIDS_LOGIN_LEN);
	    videChamp(pDonnees->passwd,	        PIDS_PASSWD_FIELD,	        PIDS_PASSWD_LEN);
    }
}

//---------------------------------------------------------------------------
//  Function:  NSPids::open()
//
//  Arguments:
//
//  Description:
//             Ouvre la table primaire et les index secondaires
//  Returns:
//             PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult NSPids::open()
{
    char tableName[255];
    if (iTypePids == pidsPatient)        strcpy(tableName, "NSPIDS.DB");    else if (iTypePids == pidsCorresp)        strcpy(tableName, "NSPIDSCOR.DB");
    else
        strcpy(tableName, "NSPIDSUTI.DB");

	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
    return (lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSPids::close()
//
//  Arguments:
//
//  Description:
//             Ferme la table primaire et l'index secondaire
//  Returns:
//             PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult NSPids::close()
{
	//
	// Appelle la fonction close() de la classe de base pour fermer
	// l'index primaire
	//
	lastError = NSFiche::close();

  return (lastError);
}

// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSPidsInfo
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Fonction    : NSPidsInfo::NSPidsInfo(NSSuper* pSuper)
// Description : Constructeur avec superviseur (pour les accès aux bases)
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPidsInfo::NSPidsInfo(NSContexte* pCtx)
           :NSRoot(pCtx)
{
	// Crée l'objet de données
	pDonnees 	  		= new NSPidsData() ;
  pGraphPerson    = new NSPersonGraphManager(pContexte->getSuperviseur()) ;
  pGraphAdr       = new NSAddressGraphManager(pContexte->getSuperviseur()) ;
  sFonction       = "" ;
  sChez           = "" ;
}

// -----------------------------------------------------------------------------
// Fonction    : NSPidsInfo::NSPidsInfo(NSPids *)
// Description : Constructeur à partir d'un NSPids
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPidsInfo::NSPidsInfo(NSPids *pPids)
           :NSRoot(pPids->NSPidsInfo::pContexte)
{
	// Crée l'objet de données
	pDonnees 	   		= new NSPidsData() ;
  pGraphPerson    = new NSPersonGraphManager(pContexte->getSuperviseur()) ;
  pGraphAdr       = new NSAddressGraphManager(pContexte->getSuperviseur()) ;
  sFonction       = "" ;
  sChez           = "" ;

	// Copie les données du NSPids
	*pDonnees 	   	= *(pPids->pDonnees) ;
}


// -----------------------------------------------------------------------------
// Fonction    : NSPidsInfo::NSPidsInfo(NSBasicAttributeArray *pAttribute)
// Description : Constructeur avec NSBasicAttributeArray (pour les accès aux bases a l'aide du pilot)
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPidsInfo::NSPidsInfo(NSBasicAttributeArray *pAttribute, NSContexte *pCtx)
           :NSRoot(pCtx)
{
	// Crée l'objet de données
	pDonnees 	  	= new NSPidsData() ;
  pGraphPerson  = new NSPersonGraphManager(pContexte->getSuperviseur()) ;
  pGraphAdr     = new NSAddressGraphManager(pContexte->getSuperviseur()) ;
  sFonction     = "" ;
  sChez         = "" ;

  NSBasicAttributeIter iter = pAttribute->begin() ;
  for(iter ; pAttribute->end() != iter ; iter++)
  {
    if ((*iter)->getBalise() == FIRST_NAME)
      strcpy(pDonnees->prenom, ((*iter)->getText()).c_str()) ;
    else if ((*iter)->getBalise() == LAST_NAME)
      strcpy(pDonnees->nom, ((*iter)->getText()).c_str()) ;
    else if ((*iter)->getBalise() == BIRTHDATE)
      strcpy(pDonnees->naissance, ((*iter)->getText()).c_str()) ;
    else if ((*iter)->getBalise() == SEX)
      strcpy(pDonnees->sexe, ((*iter)->getText()).c_str()) ;
    else if ((*iter)->getBalise() == PIDS)
      strcpy(pDonnees->nss, ((*iter)->getText()).c_str()) ;
    else if ((*iter)->getBalise() ==  LOGIN)
      strcpy(pDonnees->login, ((*iter)->getText()).c_str()) ;
  }
}

// -----------------------------------------------------------------------------
// Fonction    : NSPidsInfo::~NSPidsInfo()
// Description : Destructeur
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPidsInfo::~NSPidsInfo()
{
	delete pDonnees ;
  delete pGraphPerson ;
  delete pGraphAdr ;
  // pas de delete du pSuper
}


// -----------------------------------------------------------------------------
// Fonction    : NSPidsInfo::NSPidsInfo(NSPidsInfo& rv)
// Description : Constructeur copie
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPidsInfo::NSPidsInfo(const NSPidsInfo& rv)
           :NSRoot(rv.pContexte)
{
	// Crée l'objet de données
	pDonnees 	   	= new NSPidsData(*(rv.pDonnees)) ;
  pGraphPerson  = new NSPersonGraphManager(*(rv.pGraphPerson)) ;
  pGraphAdr     = new NSAddressGraphManager(*(rv.pGraphAdr)) ;
  sFonction     = rv.sFonction ;
  sChez         = rv.sChez ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsInfo::operator=(NSPidsInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSPidsInfo& NSPidsInfo::operator=(const NSPidsInfo& src)
{
	if (this == &src)
		return *this ;

	*pDonnees 	   = *(src.pDonnees) ;
	*pGraphPerson  = *(src.pGraphPerson) ;
	*pGraphAdr     = *(src.pGraphAdr) ;
	sFonction      = src.sFonction ;
	sChez          = src.sChez ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsInfo::operator==(NSPidsInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSPidsInfo::operator==(const NSPidsInfo& o)
{
	int egal = 1 ;

	if (!(*pDonnees == *(o.pDonnees)))
		egal = 0 ;

  return egal ;
}

// --------------------------------------------------------------------------
// ------------------------ METHODES DE NSPidsArray ----------------------
// --------------------------------------------------------------------------

NSPidsArray::NSPidsArray(const NSPidsArray& rv)
            :NSPidsVector()
{
try
{
  if (false == rv.empty())
    for (NSPidsConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSPidsInfo(*(*i))) ;
}
catch (const exception& e)
{
  string sExept = "Exception NSPidsArray copy ctor " + string(e.what());
  erreur(sExept.c_str(), standardError, 0);
}
catch (...)
{
  erreur("Exception NSPidsArray copy ctor.", standardError, 0);
}
}

voidNSPidsArray::vider()
{
  if (empty())
    return ;

  for (NSPidsIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSPidsArray::~NSPidsArray()
{
	vider() ;
}

NSPidsArray&
NSPidsArray::operator=(const NSPidsArray& src)
{
	if (this == &src)
		return *this ;

try
{
	vider() ;

	if (false == src.empty())		for (NSPidsConstIter i = src.begin() ; src.end() != i ; i++)			push_back(new NSPidsInfo(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSPidsArray (= operator).", standardError, 0) ;
	return *this;
}
}

int
NSPidsArray::operator==(const NSPidsArray& o)
{
try
{
  if (empty())  {    if (o.empty())      return 1 ;    else      return 0 ;  }  int egal = 1 ;  NSPidsIter      i = begin() ;  NSPidsConstIter j = o.begin() ;  while ((end() != i) && (o.end() != j))  {    if (!((*(*i)) == (*(*j))))    {      egal = 0 ;      break ;    }    i++ ;    j++ ;  }  if (egal)  {    if ((end() != i) || (o.end() != j))      egal = 0 ;  }  return egal ;
}
catch (const exception& e)
{
    string sExept = "Exception NSPidsArray (== operator) " + string(e.what());
    erreur(sExept.c_str(), standardError, 0);
    return 0;
}
catch (...)
{
    erreur("Exception NSPidsArray (== operator).", standardError, 0);
    return 0;
}
}

//***************************************************************************
// Implémentation des méthodes du NSPersonManager
//***************************************************************************

NSPersonInfo::NSPersonInfo(NSSuper* pSuper, NSBasicAttributeArray* pAttribute)
             :NSSuperRoot(pSuper)
{
  _pContexte = (NSContexte*) 0 ;

	initValues() ;

	if (((NSBasicAttributeArray*) NULL == pAttribute) || (pAttribute->empty()))
		return ;

	NSBasicAttributeIter iter = pAttribute->begin() ;
	for ( ; pAttribute->end() != iter ; iter++)
	{
    if      ((*iter)->getBalise() == FIRST_NAME)
    	_sPrenom = (*iter)->getText() ;
    else if ((*iter)->getBalise() == LAST_NAME)
    	_sNom = (*iter)->getText() ;
    else if ((*iter)->getBalise() == BIRTHDATE)
    	_sNaissance = (*iter)->getText() ;
    else if ((*iter)->getBalise() == SEX)
    	_sSexe = (*iter)->getText() ;
    else if ((*iter)->getBalise() == PIDS)
    	_sPersonID = (*iter)->getText() ;
    else if ((*iter)->getBalise() == LOGIN)
    	_sLogin = (*iter)->getText() ;
    else if ((*iter)->getBalise() == ROLE)
    	_sRoles = (*iter)->getText() ;
    else if ((*iter)->getBalise() == CITY_PRO)
    	_sVille = (*iter)->getText() ;
    else if ((*iter)->getBalise() == APPOINTMENT_DATE)
    	_sReconvocDate = (*iter)->getText() ;
    else if ((*iter)->getBalise() == LOCKED)
    	_sBlocked = (*iter)->getText() ;
    else if ((*iter)->getBalise() == ADELI_ID)
    	_sAdeli = (*iter)->getText() ;
	}
}

NSPersonInfo::NSPersonInfo(NSSuper* pSuper, NSContexte* pCtx, string sNss, PIDSTYPE iTypePids)
             :NSSuperRoot(pSuper)
{
  _pContexte = pCtx ;

	initValues() ;

	_sPersonID = sNss ;

	NSPatPathoArray PatPathoData(_pSuper) ;

	_typePids = iTypePids ;

	bool _bVerbose = false ;
	if (pidsPatient == iTypePids)
		_bVerbose = true ;

	// on lance un PersonGraphManager pour récupérer les données
	NSPersonGraphManager GraphManager(_pSuper) ;
	if (GraphManager.getGraphAdmin(_sPersonID, _bVerbose, iTypePids, &PatPathoData))
	{
  	NSPatPathoArray PatPathoAdmin(_pSuper, graphPerson) ;
		NSPatPathoArray PatPathoPDS(_pSuper, graphPerson) ;

    string sCode ;
    string sCivil ;
    string sSex ;
    string sActiveJob ;

    GraphManager.partDualPatho(&PatPathoData, &PatPathoAdmin, &PatPathoPDS) ;
    GraphManager.ChargeDonneesAdmin(_pContexte, &PatPathoAdmin, _sNom, _sPrenom, sCode, sSex, _sNaissance, _sCivilite, _sPhone, _sMobile, _sMail) ;
    GraphManager.ChargeDonneesPDS(_pContexte, &PatPathoPDS, _sMetier, _sSpecialite, sActiveJob, _sOrientation, sCivil, _sTitre, _sVille, _sAdeli, _sRpps) ;

    // Professionnal civility, if any, replaces regular one
    //
    if (string("") != sCivil)
      _sCivilite = sCivil ;

    if      (string("1") == sSex)
      metMasculin() ;
    else if (string("2") == sSex)
      metFeminin() ;

    updateCalculatedValues(&GraphManager, iTypePids) ;
  }
}

// Ici on fournit une patpatho et on crée le graphe à l'aide du pilote
NSPersonInfo::NSPersonInfo(NSContexte* pContexte, NSPatPathoArray* pPatPathoData, PIDSTYPE iTypePids)
             :NSSuperRoot(pContexte->getSuperviseur())
{
  _pContexte = pContexte ;
  _typePids  = iTypePids ;

  // on lance un PersonGraphManager pour récupérer les données
  //
  NSPersonGraphManager GraphManager(_pSuper) ;

  string sSex       = string("") ;
  string sCode      = string("") ;
  string sCivil     = string("") ;
  string sActiveJob = string("") ;

  if ((pPatPathoData) && (false == pPatPathoData->empty()))
  {
  	NSPatPathoArray PatPathoAdmin(_pSuper, graphPerson) ;
		NSPatPathoArray PatPathoPDS(_pSuper, graphPerson) ;

    GraphManager.partDualPatho(pPatPathoData, &PatPathoAdmin, &PatPathoPDS) ;
    GraphManager.ChargeDonneesAdmin(_pContexte, &PatPathoAdmin, _sNom, _sPrenom, sCode, sSex, _sNaissance, _sCivilite, _sPhone, _sMobile, _sMail) ;
    GraphManager.ChargeDonneesPDS(_pContexte, &PatPathoPDS, _sMetier, _sSpecialite, sActiveJob, _sOrientation, sCivil, _sTitre, _sVille, _sAdeli, _sRpps) ;

    if      (string("1") == sSex)
      metMasculin() ;
    else if (string("2") == sSex)
      metFeminin() ;
	}

	if (string("") != sCivil)
  	_sCivilite = sCivil ;

	// Sélection des traits à envoyer au pilote
	string sNewPids = string(1, INMEMORY_CHAR) + string(PAT_NSS_LEN - 1, '0') ;
  NSBasicAttributeArray AttrList ;

  switch (iTypePids)
  {
  	case pidsPatient :
    	break ;
    case pidsCorresp :
    	AttrList.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;

      // AttrList.push_back(new NSBasicAttribute(FIRST_NAME, sPrenom)) ;
      // AttrList.push_back(new NSBasicAttribute(LAST_NAME, sNom)) ;
      // AttrList.push_back(new NSBasicAttribute(CITY_PRO, sVille)) ;
      AttrList.push_back(new NSBasicAttribute(PIDS, sNewPids)) ;
      AttrList.push_back(new NSBasicAttribute(PERSON, sNewPids)) ;
      GraphManager.setInfoPids(&AttrList) ;

      GraphManager.setAdminAttrArray(NSPersonGraphManager::attribsInit, _sNom, _sPrenom, sSex, _sNaissance) ;
      GraphManager.setPDSAttrArray(NSPersonGraphManager::attribsInit, _sMetier, _sSpecialite, sActiveJob, _sOrientation, _sTitre, _sVille, _sAdeli) ;

      break ;
    case pidsUtilisat :
    	break ;
  }

	if (GraphManager.setGraphAdmin(pContexte, pPatPathoData, iTypePids))
  	// on récupère le personID issu du graphe
    _sPersonID = GraphManager.getPersonID() ;
  else
  	erreur("La création de la personne a échoué.", standardError, 0) ;

  updateCalculatedValues(&GraphManager, iTypePids) ;
}

NSPersonInfo::~NSPersonInfo()
{
}

NSPersonInfo::NSPersonInfo(const NSPersonInfo& rv)
             :NSSuperRoot(rv._pSuper), _VectData(rv._VectData)
{
  _pContexte     = rv._pContexte ;
	_sPersonID     = rv._sPersonID ;
  _sNom          = rv._sNom ;
  _sPrenom       = rv._sPrenom ;
  _sLang         = rv._sLang ;
  _sSexe         = rv._sSexe ;
  _sNaissance    = rv._sNaissance ;
  _sLogin        = rv._sLogin ;
  _sMetier       = rv._sMetier ;
  _sSpecialite   = rv._sSpecialite ;
  _sOrientation  = rv._sOrientation ;
  _sActivite     = rv._sActivite ;
  _sAdeli        = rv._sAdeli ;
  _sRpps         = rv._sRpps ;
  _sTitre        = rv._sTitre ;
  _sCivilite     = rv._sCivilite ;
  _sRoles        = rv._sRoles ;
	_sVille        = rv._sVille ;
  _sPhone        = rv._sPhone ;
  _sMobile       = rv._sMobile ;
  _sMail         = rv._sMail ;
	_sReconvocDate = rv._sReconvocDate ;
	_sBlocked      = rv._sBlocked ;

  _typePids      = rv._typePids ;

  _sChez         = rv._sChez ;
  _sObjectAdr    = rv._sObjectAdr ;
  _sMainAdr      = rv._sMainAdr ;
}

NSPersonInfo&
NSPersonInfo::operator=(const NSPersonInfo& src)
{
	if (this == &src)
		return *this ;

	_sPersonID     = src._sPersonID ;
  _sNom          = src._sNom ;
  _sPrenom       = src._sPrenom ;
  _sLang         = src._sLang ;
  _sSexe         = src._sSexe ;
  _sNaissance    = src._sNaissance ;
  _sLogin        = src._sLogin ;
  _sMetier       = src._sMetier ;
  _sSpecialite   = src._sSpecialite ;
  _sOrientation  = src._sOrientation ;
  _sActivite     = src._sActivite ;
  _sAdeli        = src._sAdeli ;
  _sRpps         = src._sRpps ;
  _sTitre        = src._sTitre ;
  _sCivilite     = src._sCivilite ;
  _sRoles        = src._sRoles ;
	_sVille        = src._sVille ;
  _sPhone        = src._sPhone ;
  _sMobile       = src._sMobile ;
  _sMail         = src._sMail ;
	_sReconvocDate = src._sReconvocDate ;
	_sBlocked      = src._sBlocked ;

  _typePids      = src._typePids ;

  _sChez         = src._sChez ;
  _sObjectAdr    = src._sObjectAdr ;
  _sMainAdr      = src._sMainAdr ;

  _VectData      = src._VectData ;

  return *this ;
}

int
NSPersonInfo::operator==(const NSPersonInfo& o)
{
	if (_sPersonID == o._sPersonID)
		return 1 ;

	return 0 ;
}

NSPersonGraphManager*
NSPersonInfo::getPersonGraph()
{
	NSPersonGraphManager *pGraphManager = new NSPersonGraphManager(_pSuper) ;
  if (pGraphManager->getGraphAdmin(_sPersonID, _typePids))
  	return pGraphManager ;

  return (NSPersonGraphManager*) 0 ;
}

string
NSPersonInfo::getMainAdr(PIDSTYPE iTypePids, string sLanguage)
{
	if (string("") != _sMainAdr)
  	return _sMainAdr ;

  NSPersonGraphManager PersonManager(_pSuper) ;
  if (string("") == _sObjectAdr)
  {
  	if (false == PersonManager.getGraphAdmin(_sPersonID, iTypePids))
      return "" ;

    if (false == PersonManager.trouveObjectAdrPrinc(iTypePids, _sObjectAdr, _sChez))
    {
    	_sMainAdr = _sCivilite + string("\r\n") + PersonManager.trouveLibLongForInGraphAdr(iTypePids) ;
      return _sMainAdr ;
    }
  }

  if (string("") != _sObjectAdr)
  {
  	NSAddressGraphManager AddressManager(_pSuper) ;
    if (AddressManager.getGraphAdr(_sObjectAdr))
    {
    	_sMainAdr = _sCivilite + string("\r\n") ;
      if (_sChez != "")
      	_sMainAdr += _sChez + string("\r\n") ;
      _sMainAdr += AddressManager.trouveLibLongAdr(_pContexte, sLanguage) ;
    }
    else
    	_sMainAdr = string("") ;
  }
  else
  	_sMainAdr = _sCivilite + string("\r\n") + PersonManager.trouveLibLongForInGraphAdr(iTypePids) ;

  return _sMainAdr ;
}

void
NSPersonInfo::updateCalculatedValues(NSPersonGraphManager* pPersonGraphManager, PIDSTYPE iTypePids)
{
  // Calcul de la civilité

  if (string("") != _sCivilite)
  {
    _sCivilite[0] = pseumaj(_sCivilite[0]) ;
    _sCivilite = _sCivilite + string(" ") + _sPrenom + string(" ") + _sNom ;
  }
  else if (string("") != _sTitre)
  {
    _sTitre[0] = pseumaj(_sTitre[0]) ;
    _sCivilite = _sTitre + string(" ") + _sPrenom + string(" ") + _sNom ;
  }
  else
    _sCivilite = _sPrenom + string(" ") + _sNom ;

  if (NULL == pPersonGraphManager)
    return ;

  _sMainAdr = _sCivilite + string("\r\n") + pPersonGraphManager->trouveLibLongForInGraphAdr(iTypePids) ;
}

void
NSPersonInfo::initValues()
{
	_sPersonID     = string("") ;
	_sNom          = string("") ;
	_sPrenom       = string("") ;
	_sLang         = string("") ;
	_sSexe         = string("") ;
	_sNaissance    = string("") ;
	_sLogin        = string("") ;

	_sMetier       = string("") ;
	_sSpecialite   = string("") ;
	_sOrientation  = string("") ;
	_sActivite     = string("") ;
  _sAdeli        = string("") ;
  _sRpps         = string("") ;
	_sTitre        = string("") ;
	_sCivilite     = string("") ;
	_sRoles        = string("") ;
	_sVille        = string("") ;
  _sPhone        = string("") ;
  _sMobile       = string("") ;
  _sMail         = string("") ;
	_sReconvocDate = string("") ;
	_sBlocked      = string("") ;

	_sChez         = string("") ;
	_sObjectAdr    = string("") ;
	_sMainAdr      = string("") ;

	_typePids      = pidsPatient ;
}

string
NSPersonInfo::getNomLong() const
{
	string sNomLong = string("") ;

	sNomLong += _sNom ;
  if (string("") != _sPrenom)
		sNomLong += string(" ") + _sPrenom ;

	return sNomLong ;
}

// --------------------------------------------------------------------------
// ------------------------ METHODES DE NSPersonArray ----------------------
// --------------------------------------------------------------------------

NSPersonArray::NSPersonArray(const NSPersonArray& rv)
              :NSPersonVector(), NSSuperRoot(rv._pSuper)
{
try
{
	if (false == rv.empty())
  	for (NSPersonConstIter i = rv.begin() ; rv.end() != i ; i++)
    	push_back(new NSPersonInfo(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSPersonArray copy ctor.", standardError, 0) ;
}
}

voidNSPersonArray::vider()
{
	if (empty())
  	return ;

  for (NSPersonIter i = begin() ; end() != i ; )
  {
  	delete *i ;
    erase(i) ;
  }
}

NSPersonArray::~NSPersonArray()
{
	vider() ;
}

NSPersonArray&
NSPersonArray::operator=(const NSPersonArray& src)
{
	if (this == &src)
		return *this ;

try
{
	vider() ;

  if (false == src.empty())  	for (NSPersonConstIter i = src.begin() ; src.end() != i ; i++)    	push_back(new NSPersonInfo(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSPersonArray (= operator).", standardError, 0) ;
  return *this ;
}
}

int
NSPersonArray::operator==(const NSPersonArray& o)
{
try
{
  if (empty())  {    if (o.empty())      return 1 ;    else      return 0 ;  }  int egal = 1 ;  NSPersonIter      i = begin() ;  NSPersonConstIter j = o.begin() ;  while ((end() != i) && (o.end() != j))  {    if (!((*(*i)) == (*(*j))))    {      egal = 0 ;      break ;    }    i++ ;    j++ ;  }  if (egal)  {    if ((end() != i) || (o.end() != j))      egal = 0 ;  }	return egal ;
}
catch (...)
{
	erreur("Exception NSPersonArray (== operator).", standardError, 0) ;
  return 0 ;
}
}

NSPersonInfo*
NSPersonArray::lookForPersonInArray(string sID)
{
  if ((string("") == sID) || (true == empty()))
  	return (NSPersonInfo*) 0 ;

  for (NSPersonIter i = begin() ; end() != i ; i++)
    if ((*i)->_sPersonID == sID)
      return (*i) ;

  return (NSPersonInfo*) 0 ;
}

NSPersonInfo*
NSPersonArray::lookForRppsInArray(string sRPPS)
{
  if ((string("") == sRPPS) || (true == empty()))
  	return (NSPersonInfo*) 0 ;

  for (NSPersonIter i = begin() ; end() != i ; i++)
    if ((*i)->getRpps() == sRPPS)
      return (*i) ;

  return (NSPersonInfo*) 0 ;
}

NSPersonInfo*
NSPersonArray::lookForAdeliInArray(string sAdeli)
{
  if ((string("") == sAdeli) || (true == empty()))
  	return (NSPersonInfo*) 0 ;

  for (NSPersonIter i = begin() ; end() != i ; i++)
    if ((*i)->getAdeli() == sAdeli)
      return (*i) ;

  return (NSPersonInfo*) 0 ;
}

NSPersonInfo*
NSPersonArray::getPerson(NSContexte *pContexte, string sID, PIDSTYPE iTypePids, GETPERSONMODE iGetPersonMode)
{
	if (string("") == sID)
  	return (NSPersonInfo*) 0 ;

  // First check if this person is already known
  //
  NSPersonInfo* pPerson = lookForPersonInArray(sID) ;

  if (pPerson)
    return pPerson ;

  // This person was not found in array
  //

  // If in "justCheck" mode, then return NULL
  //
  if (justCheck == iGetPersonMode)
    return (NSPersonInfo*) 0 ;

  // If in "asynchronous" mode, then ask PilotProxy to do the job
  //
  if (asynchronous == iGetPersonMode)
  {
    string sMsg = string("NSPersonArray::getPerson, asking for person ") + sID + string(" in asynchronous mode") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

	  NSToDoTask* pTask = new NSToDoTask ;
	  pTask->setWhatToDo(string("GetPersonAsynch")) ;
    pTask->setParam1(sID) ;

    string sTypePidsAsString = string("") ;
    switch (iTypePids)
    {
      case pidsPatient      : sTypePidsAsString = string("pidsPatient") ;      break ;
      case pidsCorresp      : sTypePidsAsString = string("pidsCorresp") ;      break ;
      case pidsUtilisat     : sTypePidsAsString = string("pidsUtilisat") ;     break ;
      case pidsPatientGroup : sTypePidsAsString = string("pidsPatientGroup") ; break ;
      case pidsAll          : sTypePidsAsString = string("pidsAll") ;          break ;
    }
    pTask->setParam2(sTypePidsAsString) ;

	  pTask->sendMe(_pSuper) ;

    return (NSPersonInfo*) 0 ;
  }

  // Get this person from server
  //
  NSPersonInfo* pNewPerson = new NSPersonInfo(_pSuper, pContexte, sID, iTypePids) ;
  push_back(pNewPerson) ;
  return pNewPerson ; // back() ;
}

NSPersonInfo*
NSPersonArray::getPersonFromRpps(NSContexte *pContexte, string sID, GETPERSONMODE iGetPersonMode)
{
  if (string("") == sID)
    return (NSPersonInfo*) 0 ;

  string sMsg = string("NSPersonArray::getPersonFromRpps, asking for person whose RPPS is ") + sID + string(" in memory.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  NSPersonInfo* pPerson = lookForRppsInArray(sID) ;
  if (pPerson)
    return pPerson ;

  sMsg = string("NSPersonArray::getPersonFromRpps, asking for person whose RPPS is ") + sID + string(" in database, using the index.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  pPerson = getPersonFromIndexedProId(pContexte, sID, string("LRPPS")) ;
  if (pPerson)
    return pPerson ;

  sMsg = string("NSPersonArray::getPersonFromRpps, asking for person whose RPPS is ") + sID + string(" in database.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  return getPersonFromProId(pContexte, sID, string("LRPPS"), iGetPersonMode) ;
}

NSPersonInfo*
NSPersonArray::getPersonFromAdeli(NSContexte *pContexte, string sID, GETPERSONMODE iGetPersonMode)
{
  NSPersonInfo* pPerson = lookForAdeliInArray(sID) ;
  if (pPerson)
    return pPerson ;

  pPerson = getPersonFromIndexedProId(pContexte, sID, string("LADEL")) ;
  if (pPerson)
    return pPerson ;

  return getPersonFromProId(pContexte, sID, string("LADEL"), iGetPersonMode) ;
}

NSPersonInfo*
NSPersonArray::getPersonFromIndexedProId(NSContexte *pContexte, string sID, string sIdCode)
{
  NSPersonsAttributesArray AttsList ;
  NSBasicAttributeArray AttrArray ;

  // Check that there is a single person with this ADELI_ID
  //
  AttrArray.push_back(new NSBasicAttribute(ROLE, CORRESP_ROLE)) ;
  AttrArray.push_back(new NSBasicAttribute(ADELI_ID, sID)) ;

  bool bListOk = _pSuper->getPilot()->personList((NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str(), &AttsList, &AttrArray) ;

  if ((false == bListOk) || AttsList.empty())
    return (NSPersonInfo*) 0 ;

  if (AttsList.size() > 1)
  {
    string sWarning = string("getPersonFromIndexedProId: Multiple persons with professionnal identifer = ") + sID + string(" ; opening first") ;
    _pSuper->trace(&sWarning, 1, NSSuper::trWarning) ;
  }

  NSPersonsAttributeIter iterPerson = AttsList.begin() ;

  string sPIDS = (*iterPerson)->getAttributeValue(PIDS) ;

  string sMsg = string("") ;

  NSPersonInfo* pFoundAndOk = getPersonByPidsFromProId(pContexte, sID, sIdCode, sPIDS) ;
  if (pFoundAndOk)
  {
    sMsg = string("getPersonFromIndexedProId: Correspondant found: ") + sPIDS ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
    return pFoundAndOk ;
  }

  sMsg = string("getPersonFromIndexedProId: Correspondant not found, leaving") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  return (NSPersonInfo*) 0 ;
}

NSPersonInfo*
NSPersonArray::getPersonFromProId(NSContexte *pContexte, string sID, string sIdCode, GETPERSONMODE /* iGetPersonMode */)
{
  NSPersonsAttributesArray AttsList ;
  NSBasicAttributeArray AttrArray ;

  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, ROLE)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, CITY_PRO)) ;

  AttrArray.push_back(new NSBasicAttribute(ROLE,  CORRESP_ROLE)) ;

  // Get list of all corresps
  //
  string sMsg = string("getPersonFromProId: Getting the list of all correspondants") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  bool bListOk = _pSuper->getPilot()->personList((NautilusPilot::SERV_SEARCH_PERSON).c_str(), &AttsList, &AttrArray) ;

  if ((false == bListOk) || AttsList.empty())
    return (NSPersonInfo*) 0 ;

  for (NSPersonsAttributeIter iterPerson = AttsList.begin() ; AttsList.end() != iterPerson ; iterPerson++)
  {
    string sPIDS = (*iterPerson)->getAttributeValue(PIDS) ;

    NSPersonInfo* pFoundAndOk = getPersonByPidsFromProId(pContexte, sID, sIdCode, sPIDS) ;
    if (pFoundAndOk)
      return pFoundAndOk ;
  }

  pContexte->getSuperviseur()->afficheStatusMessage(string("")) ;

  sMsg = string("getPersonFromProId: Correspondant not found, leaving") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  return (NSPersonInfo*) 0 ;
}

NSPersonInfo*
NSPersonArray::getPersonByPidsFromProId(NSContexte *pContexte, string sID, string sIdCode, string sPids)
{
  string sMsg = string("Looking for corresp ") + sPids + string(" in memory") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  NSPersonInfo* pAlreadyThere = lookForPersonInArray(sPids) ;
  if (pAlreadyThere)
  {
    // We don't do this because there is only one index (currently attached
    // to the Adeli) and we want it to be usable for RPPS
    //
    // if (((string("LRPPS") == sIdCode) && (pAlreadyThere->getRpps()  == sID)) ||
    //    ((string("LADEL") == sIdCode) && (pAlreadyThere->getAdeli() == sID)))
    //
    if ((pAlreadyThere->getRpps() == sID) || (pAlreadyThere->getAdeli() == sID))
    {
      pContexte->getSuperviseur()->afficheStatusMessage(string("")) ;

      sMsg = string("Corresp found in memory") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      return pAlreadyThere ;
    }
  }
  else
  {
    // We get the person... and while we got her, we add her to the buffer
    //
    pContexte->getSuperviseur()->afficheStatusMessage(string("Load corresp.") + sPids) ;

    sMsg = string("Looking for corresp ") + sPids + string(" in database") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    NSPersonInfo *pPerson = new NSPersonInfo(_pSuper, pContexte, sPids, pidsCorresp) ;
    push_back(pPerson) ;

    // We don't do this because there is only one index (currently attached
    // to the Adeli) and we want it to be usable for RPPS
    //
    // if (((string("LRPPS") == sIdCode) && (pPerson->getRpps() == sID)) ||
    //     ((string("LADEL") == sIdCode) && (pPerson->getAdeli() == sID)))
    //
    if ((pPerson->getRpps() == sID) || (pPerson->getAdeli() == sID))
    {
      pContexte->getSuperviseur()->afficheStatusMessage(string("")) ;

      sMsg = string("Corresp found in database") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      return pPerson ;
    }
  }

  return (NSPersonInfo*) 0 ;
}

NSPersonInfo*
NSPersonArray::createPerson(NSContexte *pContexte, PIDSTYPE iTypePids)
{
  NSPatPathoArray PatPathoData(_pSuper, graphPerson) ;

  NSSmallBrother  BigBoss(pContexte, &PatPathoData) ;
  BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;

#ifdef __OB1__
	BB1BBInterfaceForKs InterfaceForKs(-1, _pSuper->getReferalProfessionnalArchetypeId(), "", false) ;
#endif
  // Ici on lance une boite de dialogue modale
  switch (iTypePids)
  {
    case pidsPatient	:	break ;
    case pidsCorresp	:
#ifdef __OB1__
    	BigBoss.lanceBbkArchetypeInDialog(_pSuper->getReferalProfessionnalArchetypeId(), 0, 0, &InterfaceForKs, true) ;
#else
    	BigBoss.lanceBbkArchetypeInDialog(_pSuper->getReferalProfessionnalArchetypeId(), 0, 0, true) ;
#endif
      break ;
    case pidsUtilisat	:	break ;
  }

  // on teste le code de retour du dialogue, qui est stocké dans le
  // BBItem root créé par le pBigBoss.
  if (0 == BigBoss.getBBItem()->getRetourDlg())     // CmOK
  {
    // on enregistre la patpatho du pBigBoss, qui contient les nouvelles données
    // si elle n'est pas vide.
    if (false == BigBoss.getPatPatho()->empty())
    {
      PatPathoData = *(BigBoss.getPatPatho()) ;

      NSPersonInfo *pNewPerson = new NSPersonInfo(pContexte, &PatPathoData, iTypePids) ;
      push_back(pNewPerson) ;
      return back() ;
    }
  }
  return (NSPersonInfo*) 0 ;
}

bool
NSPersonArray::modifyPerson(NSContexte *pContexte, string sID, string sRoles, PIDSTYPE iTypePids)
{
	// Modify "as a correspondant"
  //
	if (pidsCorresp == iTypePids)
  {
  	// Is she also a Patient?
    //
  	size_t iPatientPos = sRoles.find(PATIENT_ROLE) ;
    //
    // If not also a Patient, do as usual
    //
    if (NPOS == iPatientPos)
    {
    	NSPatPathoArray PatPathoData(_pSuper, graphPerson) ;
  		NSPersonGraphManager GraphManager(_pSuper) ;
  		bool bVerbose = false ;
      if (GraphManager.getGraphAdmin(sID, bVerbose, iTypePids, &PatPathoData))
      {
      	GraphManager.setAttributeValue(PIDS, sID) ;
        GraphManager.setAttributeValue(PERSON, GraphManager.getPersonID()) ;
				GraphManager.setAttributeValue(OPERATOR, pContexte->getUtilisateurID()) ;
				GraphManager.setAttributeValue(CONSOLE,  string(_pSuper->getConsole())) ;
				// GraphManager.setAttributeValue(INSTANCE, string(szInstance)) ;

      	NSSmallBrother BigBoss(pContexte, &PatPathoData) ;
    		BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
#ifdef __OB1__
				BB1BBInterfaceForKs InterfaceForKs(-1, _pSuper->getReferalProfessionnalArchetypeId(), "", false) ;
				BigBoss.lanceBbkArchetypeInDialog(_pSuper->getReferalProfessionnalArchetypeId(), 0, 0, &InterfaceForKs, true) ;
#else
				BigBoss.lanceBbkArchetypeInDialog(_pSuper->getReferalProfessionnalArchetypeId(), 0, 0, true) ;
#endif
      	// on teste le code de retour du dialogue, qui est stocké dans le BBItem
    		// root créé par le pBigBoss.
    		if (0 == BigBoss.getBBItem()->getRetourDlg())     // CmOK
    		{
      		// on enregistre la patpatho du pBigBoss, qui contient les nouvelles données
      		// si elle n'est pas vide.
      		if (false == BigBoss.getPatPatho()->empty())
      		{
        		PatPathoData = *(BigBoss.getPatPatho()) ;
            if (false == GraphManager.setBothAttributes(pContexte, &PatPathoData, NSPersonGraphManager::attribsUpdate))
            	return false ;

            string sContrib = createContribution(pContexte, &GraphManager, iTypePids) ;

            // pUtilInfo->pGraphPerson->setInfoPids(&AttrList) ;
        		if (false == GraphManager.setGraphAdmin(pContexte, &PatPathoData, pidsCorresp))
        			return false ;

            closeContribution(pContexte, &GraphManager, sContrib) ;

            // Get an updated NSPersonInfo
            //
            NSPersonInfo ModifiedPerson(_pSuper, pContexte, sID, iTypePids) ;

            // Update the array
            //
            NSPersonInfo* pUnmodifiedPerson = lookForPersonInArray(sID) ;
            if (pUnmodifiedPerson)
              *pUnmodifiedPerson = ModifiedPerson ;
            else
              push_back(new NSPersonInfo(ModifiedPerson)) ;

        		return true ;
      		}
    		}
			}
    }
    //
    // If also a Patient, ask what part to edit
    //
    else
    {
    	whatToEditDialog sayWhot(pContexte->GetMainWindow(), sRoles, pContexte) ;
      int idRet = sayWhot.Execute() ;
      if (IDOK != idRet)
      	return true ;

      NSPatPathoArray PatPathoData(_pSuper, graphPerson) ;
  		NSPersonGraphManager GraphManager(_pSuper) ;
  		bool bVerbose = false ;
      if (false == GraphManager.getGraphAdmin(sID, bVerbose, iTypePids, &PatPathoData))
      	return false ;

      NSPatPathoArray PatPathoPerso(_pSuper, graphPerson) ;
      NSPatPathoArray PatPathoPro(_pSuper, graphPerson) ;

      if (false == PatPathoData.empty())
      	GraphManager.partDualPatho(&PatPathoData, &PatPathoPerso, &PatPathoPro) ;

      if (sayWhot.isPersonalDataActive())
      {
      	NSSmallBrother BigBoss(pContexte, &PatPathoPerso) ;
    		BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
        string sArchetypeId = _pSuper->getDemographicArchetypeId() ;
#ifdef __OB1__
				BB1BBInterfaceForKs InterfaceForKs(-1, sArchetypeId, "", false) ;
				BigBoss.lanceBbkArchetypeInDialog(sArchetypeId, 0, 0, &InterfaceForKs, true) ;
#else
				BigBoss.lanceBbkArchetypeInDialog(sArchetypeId, 0, 0, true) ;
#endif

				if ((0 == BigBoss.getBBItem()->getRetourDlg()) &&
            (*(BigBoss.getPatPatho()) != PatPathoPerso))
        	PatPathoPerso = *(BigBoss.getPatPatho()) ;
      }
      if (sayWhot.isProfessionalDataActive())
      {
      	NSSmallBrother BigBoss(pContexte, &PatPathoPro) ;
    		BigBoss.setMotherWindow(pContexte->GetMainWindow()) ;
        string sArchetypeId = _pSuper->getReferalProfessionnalEditArchetypeId() ;
#ifdef __OB1__
				BB1BBInterfaceForKs InterfaceForKs(-1, sArchetypeId, "", false) ;
				BigBoss.lanceBbkArchetypeInDialog(sArchetypeId, 0, 0, &InterfaceForKs, true) ;
#else
				BigBoss.lanceBbkArchetypeInDialog(sArchetypeId, 0, 0, true) ;
#endif

				if ((0 == BigBoss.getBBItem()->getRetourDlg()) &&
            (*(BigBoss.getPatPatho()) != PatPathoPro))
        	PatPathoPro = *(BigBoss.getPatPatho()) ;
      }

      // Concatenating both patPatho
      //
      NSPatPathoArray newPatPathoData(_pSuper, graphPerson) ;
      GraphManager.buildDualPatho(&newPatPathoData, &PatPathoPerso, &PatPathoPro) ;

      if (newPatPathoData != PatPathoData)
      {
        string sContrib = createContribution(pContexte, &GraphManager, iTypePids) ;

      	// pUtilInfo->pGraphPerson->setInfoPids(&AttrList) ;
        bool bRet = GraphManager.setGraphAdmin(pContexte, &newPatPathoData, pidsCorresp) ;

        closeContribution(pContexte, &GraphManager, sContrib) ;

        if (false == bRet)
        	return false ;

        // Get an updated NSPersonInfo
        //
        NSPersonInfo ModifiedPerson(pContexte, &PatPathoData, iTypePids) ;

        // Update the array
        //
        NSPersonInfo* pUnmodifiedPerson = lookForPersonInArray(sID) ;
        if (pUnmodifiedPerson)
          *pUnmodifiedPerson = ModifiedPerson ;
        else
          push_back(new NSPersonInfo(ModifiedPerson)) ;

        return true ;
      }
    }
  }

	return false ;
}

string
NSPersonArray::createContribution(NSContexte *pContexte, NSPersonGraphManager* pGraphManager, PIDSTYPE iTypePids)
{
try
{
  if (NULL == pGraphManager)
    return string("") ;

	string sDateJour = donne_date_duJour() + donne_heure() ;
	string sPidsUtil = pContexte->getUtilisateurID() ;

	// Construction de la patpatho de la nouvelle contribution
	NSPatPathoArray PatPathoArray(_pSuper) ;

	PatPathoArray.ajoutePatho("LCTRI1", 0) ;

  Message Msg ;

	// Utilisateur
	PatPathoArray.ajoutePatho("HUTIL1", 1) ;
	Msg.SetComplement(sPidsUtil.c_str()) ;
	PatPathoArray.ajoutePatho("£SPID1", &Msg, 2) ;

  Msg.Reset() ;

	// Date d'ouverture
	PatPathoArray.ajoutePatho("KOUVR1", 1) ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(sDateJour) ;
	PatPathoArray.ajoutePatho("£T0;19", &Msg, 2) ;

	//
	// On crée la liaison de la contribution avec le document root	//	// La contribution initiale est enregistrée dans ::createPatient	string sNodeRoot     = pGraphManager->getRootTree() ;	string sContribution = pGraphManager->setTree(&PatPathoArray, "") ;	NSLinkManager Link(_pSuper, pGraphManager->getDataGraph()) ;	Link.etablirLien(sNodeRoot, NSRootLink::personContribution, sContribution) ;  // pour les contributions ultérieures on update directement  NSVectPatPathoArray VectU ;  VectU.push_back(new NSPatPathoArray(PatPathoArray)) ;
  bool bContribCreated = pGraphManager->updateTrees(pContexte, &VectU, &sContribution, 0, iTypePids) ;  if (false == bContribCreated)    return string("") ;	return sContribution ;}
catch (...)
{
	erreur("Exception NSPersonArray::createContribution.", standardError, 0) ;
	return string("") ;
}
}

bool
NSPersonArray::closeContribution(NSContexte *pContexte, NSPersonGraphManager* pGraphManager, string sContribution)
{
try
{
  if (NULL == pGraphManager)
    return false ;

  string sDateJour = donne_date_duJour() + donne_heure() ;

  //
  // on charge l'arbre qui correspond à la contribution en cours
  //
  NSPatPathoArray PatPathoArray(_pSuper) ;
  string sRosace = "" ;
  if (false == pGraphManager->getTree(sContribution, &PatPathoArray, &sRosace))
    return false ;

  NSPatPathoArray PatPatho(_pSuper) ;
  PatPatho.ajoutePatho("KFERM1", 0) ;
  Message	Msg ;
  Msg.SetUnit("2DA021") ;
  Msg.SetComplement(sDateJour) ;
  PatPatho.ajoutePatho("£T0;19", &Msg, 1) ;

  PatPathoArray.InserePatPatho(PatPathoArray.end(), &PatPatho, 1) ;

  NSVectPatPathoArray VectU ;
  VectU.push_back(new NSPatPathoArray(PatPathoArray)) ;
  string sNewTreeId ;
  if (false == pGraphManager->updateTrees(pContexte, &VectU, &sNewTreeId))
    return false ;

  return true ;
}
catch (...)
{
	erreur("Exception NSPersonArray::closeContribution.", standardError, 0) ;
  return false ;
}
}

// fin du fichier nsmpids.cpp

