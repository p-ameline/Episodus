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
// #include "nautilus\nsrechdl.h"
#include "partage\nsdivfct.h"
#include "nautilus\nspatdlg.h"
#include "nautilus\nsanxary.h"
#include "nsdn\nsdocum.h"
#include "nsdn\nsframedocs.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsmanager.h"
#include "nsbb\nsbbtran.h"
#include "nssavoir\nspatho.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsHealthTeam.h"
#include "nssavoir\nsguide.h"
#include "nautilus\nsTeamDocView.h"
#include "nautilus\nshistdo.h"
#include "dcodeur\nsgen.h"

# include "partage\ns_search.h"
#ifndef __NSPERSON_H
# include "partage\nsperson.h"
#endif

#include "pilot\NautilusPilot.hpp"
#include "nsbb\tagNames.h"

long NSPatInfo::lObjectCount = 0 ;
long NSCorrespondantInfo::lObjectCount = 0 ;

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
  init() ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::NSPatientData(NSPatientData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPatientData::NSPatientData(const NSPatientData& rv)
              :NSRoot(rv.pContexte)
{
  initFromPatientData(&rv) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::operator=(NSPatientData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSPatientData&
NSPatientData::operator=(const NSPatientData& src)
{
	if (this == &src)
		return *this ;

  initFromPatientData(&src) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatientData::operator==(NSPatientData src)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSPatientData::operator==(const NSPatientData& o)
{
  if ((_sNss           == o._sNss)           &&
      (_sNom           == o._sNom)           &&
      (_sPrenom        == o._sPrenom)        &&
      (_sNomJeuneFille == o._sNomJeuneFille) &&
      (_sCode          == o._sCode)          &&
      (_sConvoc        == o._sConvoc)        &&
      (_sSexe          == o._sSexe)          &&
      (_sAdresse       == o._sAdresse)       &&
      (_sNaissance     == o._sNaissance)     &&
      (_sTelepor       == o._sTelepor)       &&
      (_sTelebur       == o._sTelebur)       &&
      (_sSitfam        == o._sSitfam)
      )
      return 1 ;
  else
      return 0 ;
}

void
NSPatientData::init()
{
  _sNss           = string("") ;
  _sNom           = string("") ;
  _sPrenom        = string("") ;
  _sCode          = string("") ;
  _sNomJeuneFille = string("") ;
  _sIpp           = string("") ;
  _sMergedWith    = string("") ;
  _sConvoc        = string("") ;
  _sSexe          = string("") ;
  _sAdresse       = string("") ;
  _sNaissance     = string("") ;
  _sTelepor       = string("") ;
  _sTelebur       = string("") ;
  _sSitfam        = string("") ;
  _sLang          = string("") ;
  _sCivilite      = string("") ;
  _sNomLong       = string("") ;
  _sLockedConsole = string("") ;
  _sPatientType   = string("") ;
}

void
NSPatientData::initFromPatientData(const NSPatientData *pOther)
{
  if ((NSPatientData*) NULL == pOther)
    return ;

  _sNss           = pOther->_sNss ;
  _sNom           = pOther->_sNom ;
  _sPrenom        = pOther->_sPrenom ;
  _sCode          = pOther->_sCode ;
  _sConvoc        = pOther->_sConvoc ;
  _sSexe          = pOther->_sSexe ;
  _sAdresse       = pOther->_sAdresse ;
  _sNaissance     = pOther->_sNaissance ;
  _sTelepor       = pOther->_sTelepor ;
  _sTelebur       = pOther->_sTelebur ;
  _sSitfam        = pOther->_sSitfam ;
  _sLang          = pOther->_sLang ;
  _sCivilite      = pOther->_sCivilite ;
  _sNomLong       = pOther->_sNomLong ;
  _sIpp           = pOther->_sIpp ;
  _sMergedWith    = pOther->_sMergedWith ;
  _sLockedConsole = pOther->_sLockedConsole ;
  _sPatientType   = pOther->_sPatientType ;
  _sNomJeuneFille = pOther->_sNomJeuneFille ;
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

	if ((string("") != _sNaissance) && (string("00000000") != _sNaissance))
	{
		string sDateJour = donne_date_duJour() ;
		int iAge = donne_age(sDateJour, _sNaissance) ;

		if ((iAge >= 0) && (iAge <= 14))
			bEnfant = true ;
	}

	if (string("fr") == sLang)
	{
		if (bEnfant)
			_sNomLong = string("Enfant ") ;
		else
		{
			if ('2' == _sSexe[0])
				_sNomLong = string("Mme ") ;
			else
				_sNomLong = string("M. ") ;
		}
	}

	if (string("en") == sLang)
	{
		if (bEnfant)
			_sNomLong = string("Child ");
		else
		{
			if ('2' == _sSexe[0])
				_sNomLong = string("Ms ");
			else
				_sNomLong = string("Mr ");
		}
	}

	_sNomLong += _sNom + string(" ") + _sPrenom ;
}

string
NSPatientData::fabriqueCivilite(bool /* bShort */, string sLang) const
{
	if (string("") == sLang)
		sLang = pContexte->getUserLanguage() ;

	if (string("") == _sCivilite)
    return string("") ;

  string sLibelleTrouve ;
  pContexte->getDico()->donneLibelle(sLang, &_sCivilite, &sLibelleTrouve) ;
  if (string("") == sLibelleTrouve)
    return string("") ;

  sLibelleTrouve += string(" ") ;
  
  return sLibelleTrouve ;
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
			nbenf += 55 ;
			sitfam[1] = (char) (nbenf) ;
			sitfam[2] = '\0' ;
		}
		else // cas nbenf > 35
		{
			sitfam[1] = '*' ;
      sitfam[2] = '\0' ;
    }

	_sSitfam = string(sitfam) ;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::getNbEnfants()
//
//  Description: retourne le nb d'enfants d'après sitfam[1]
//---------------------------------------------------------------------------
int NSPatientData::getNbEnfants() const
{
	char sitfam[3] ;
	strcpy(sitfam, _sSitfam.c_str()) ;

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
string NSPatientData::donneSitFam(string sLang) const
{
	string sSitFam ;

	if (string("") == sLang)
		sLang = pContexte->getUserLanguage() ;

	switch (_sSitfam[0])
	{
		case 'C' :	sSitFam = "Célibataire" ;
								break;

		case 'V' :	if (estMasculin())
									sSitFam = "Veuf" ;
								else
									sSitFam = "Veuve" ;
            		break;

		case 'B' :	if (estMasculin())
									sSitFam = "Concubin" ;
								else
									sSitFam = "Concubine" ;
            		break;

		case 'M' :	if (estMasculin())
									sSitFam = "Marié" ;
								else
									sSitFam = "Mariée" ;
								break;

		case 'D' :	if (estMasculin())
									sSitFam = "Divorcé" ;
								else
									sSitFam = "Divorcée" ;
								break;

		default :		sSitFam = "" ;
	}

  return sSitFam ;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneNbEnfants()
//
//  Description: Retourne le nombre d'enfants
//---------------------------------------------------------------------------
string NSPatientData::donneNbEnfants() const
{
	int iNbEnf = getNbEnfants() ;

	// cas nb enfants trop grand
	if (iNbEnf < 0)
		return string("nombreux enfants") ;

	// cas nb enfants entre 0 et max (max fixé à 35 pour l'instant !)
  if (1 == iNbEnf)
    return string("1 enfant") ;

  if (iNbEnf > 1)
    return IntToString(iNbEnf) + string(" enfants") ;

	return string("") ;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneDateNaiss()
//
//  Description: Retourne une string jj/mm/aaaa contenant la date de naissance
//---------------------------------------------------------------------------
string
NSPatientData::donneDateNaiss(string sLang) const
{
  if (string("") == _sNaissance)
		return string("") ;

  string sFormat = pContexte->getSuperviseur()->getText("0localInformation", "dateFormat", sLang, pContexte) ;

  return getFormatedDate(_sNaissance, sLang, sFormat) ;
}

//---------------------------------------------------------------------------
//  Function:    NSPatientData::donneTitre()
//
//  Description: Retourne une string correspondant à nom_long s'il existe
//               sinon refabrique le nom_long
//---------------------------------------------------------------------------
string NSPatientData::donneTitre(string sLang)
{
	if (string("") == sLang)
		sLang = pContexte->getUserLanguage() ;

	if (string("") == _sNomLong)
		fabriqueNomLong() ;

	return _sNomLong ;
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
          :NSRoot(pCtx),
           _Donnees(pCtx),
           _GraphPerson(pCtx->getSuperviseur()),
           _GraphAdr(pCtx->getSuperviseur())
{
  _pFramesDocuments = (NSFrameDocumentsArray*) 0 ;
  _sChez            = string("") ;
  _pDocHis          = (NSHISTODocument*) 0 ;
  _pHealthTeam      = (NSHealthTeam*) 0 ;

  lObjectCount++ ;
}

// -----------------------------------------------------------------------------
// Fonction    : NSPatInfo::NSPatInfo(NSBasicAttributeArray* pAttribute)
// Description : Constructeur avec NSBasicAttributeArray (pour les accès aux bases a l'aide du pilot)
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPatInfo::NSPatInfo(NSBasicAttributeArray* pAttribute, NSContexte* pCtx)
          :NSRoot(pCtx),
           _Donnees(pCtx),
           _GraphPerson(pCtx->getSuperviseur()),
           _GraphAdr(pCtx->getSuperviseur())
{
  _pFramesDocuments = (NSFrameDocumentsArray*) 0 ;
  _sChez            = string("") ;

  _pDocHis          = (NSHISTODocument*) 0 ;

  _pHealthTeam      = (NSHealthTeam*) 0 ;

  NSBasicAttributeIter iter = pAttribute->begin() ;
  for ( ; pAttribute->end() != iter ; iter++)
  {
    string sTag        = (*iter)->getBalise() ;
    string sTagContent = (*iter)->getText() ;

    if      (FIRST_NAME == sTag)
    	_Donnees._sPrenom = sTagContent ;
    else if (LAST_NAME == sTag)
    	_Donnees._sNom = sTagContent ;
    else if (BIRTHDATE == sTag)
    	_Donnees._sNaissance = sTagContent ;
    else if (SEX == sTag)
    	_Donnees._sSexe = sTagContent ;
    else if (PIDS == sTag)
    	_Donnees._sNss = sTagContent ;
    else if (IPP == sTag)
    	_Donnees._sIpp = sTagContent ;
    else if (MERGE == sTag)
    	_Donnees._sMergedWith = sTagContent ;
    else if (LOCKED == sTag)
    	_Donnees._sLockedConsole = sTagContent ;
    else if (APPOINTMENT_DATE == sTag)
    	_Donnees._sConvoc = sTagContent ;
    else if (PERSONTYPE == sTag)
    	_Donnees._sPatientType = sTagContent ;
  }

  lObjectCount++ ;
}

// -----------------------------------------------------------------------------
// Fonction    : NSPatInfo::NSPatInfo(NSPatient*)
// Description : Constructeur à partir d'un NSPatient
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPatInfo::NSPatInfo(NSPids *pPids)
          :NSRoot(pPids->NSPidsInfo::pContexte),
           _Donnees(pPids->NSPidsInfo::pContexte),
           _GraphPerson(pPids->NSPidsInfo::pContexte->getSuperviseur()),
           _GraphAdr(pPids->NSPidsInfo::pContexte->getSuperviseur())
{
  _pFramesDocuments = (NSFrameDocumentsArray*) 0 ;
  _sChez            = string("") ;

  _pDocHis          = (NSHISTODocument*) 0 ;

  _pHealthTeam      = (NSHealthTeam*) 0 ;

	// Copie les données du NSPids
  _Donnees._sNss       = string(pPids->pDonnees->nss) ;
  _Donnees._sNom       = string(pPids->pDonnees->nom) ;
  _Donnees._sPrenom    = string(pPids->pDonnees->prenom) ;
  _Donnees._sCode      = string(pPids->pDonnees->code) ;
  _Donnees._sSexe      = string(pPids->pDonnees->sexe) ;
  _Donnees._sNaissance = string(pPids->pDonnees->naissance) ;
  _Donnees.fabriqueNomLong() ;

  lObjectCount++ ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::~NSPatInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPatInfo::~NSPatInfo()
{
  lObjectCount-- ;

// Not possible here yet because of linking dependencies
//
//  if (NULL != pDocHis)
//    delete pDocHis ;

  if (_pHealthTeam)
		delete _pHealthTeam ;

  if (_pFramesDocuments)
    delete _pFramesDocuments ;
}

// -----------------------------------------------------------------------------
// Fonction    : NSPatInfo::NSPatInfo(NSPatInfo& rv)
// Description : Constructeur copie
// Retour      : Rien
// -----------------------------------------------------------------------------
NSPatInfo::NSPatInfo(const NSPatInfo& rv)
          :NSRoot(rv.pContexte),
           _Donnees(rv._Donnees),
           _CorrespArray(rv._CorrespArray),
           _GraphPerson(rv._GraphPerson),
           _GraphAdr(rv._GraphAdr)
{
try
{
  _sChez         = rv._sChez ;

  _pDocHis       = (NSHISTODocument*) 0 ;
  _pHealthTeam   = (NSHealthTeam*) 0 ;

  if ((NSFrameDocumentsArray*) NULL == rv._pFramesDocuments)
    _pFramesDocuments = (NSFrameDocumentsArray*) 0 ;
  else
    _pFramesDocuments = new NSFrameDocumentsArray(*(rv._pFramesDocuments)) ;

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception NSPatInfo copy ctor", standardError, 0) ;
}
}

bool
NSPatInfo::open(const string sNss, bool bLock)
{
  if (false == getGraph(sNss, bLock))
    return false ;

  NSPatPathoArray PatPathoAdmin(pContexte->getSuperviseur()) ;

  string sAdminDataTreeID = getAdminData(&PatPathoAdmin) ;

  if (string("") == sAdminDataTreeID)
  {
    string sMsg = string("Il est impossible de trouver l'arbre administratif dans le graphe.") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trError) ;
    erreur(sMsg.c_str(), standardError, 0) ;
    return false ;
  }

  setNss(string(sAdminDataTreeID, 0, PIDS_NSS_LEN)) ;

  ChargeDonneesPatient(&PatPathoAdmin) ;

  return true ;
}

bool
NSPatInfo::getGraph(const string sNss, bool /* bLock */)
{
try
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

	NSBasicAttributeArray LocalAttrArray ;
	LocalAttrArray.push_back(new NSBasicAttribute(PERSON,   sNss)) ;
	LocalAttrArray.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;
  LocalAttrArray.push_back(new NSBasicAttribute(CONSOLE,  pSuper->getConsoleString())) ;
  LocalAttrArray.push_back(new NSBasicAttribute(INSTANCE, pSuper->getInstanceString())) ;

  NSPersonsAttributesArray PatientList ;

  string ps = string("Calling Pilot service \"searchPatient\"") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	// bool res = pSuper->getPilot()->searchPatient(NautilusPilot::SERV_OPEN_PATIENT_DATA_FROM_TRAITS.c_str(), pPatRech->getGraphPerson()->getDataGraph(), &PatientList, &LocalAttrArray) ;
  bool res = pSuper->getPilot()->searchPatient(NautilusPilot::SERV_SEARCH_PATIENT.c_str(), _GraphPerson.getDataGraph(), &PatientList, &LocalAttrArray) ;

	if (false == res)
	{
  	pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
  	string tempMessage = pSuper->getPilot()->getWarningMessage() ;
    string tempError   = pSuper->getPilot()->getErrorMessage() ;
    if (string("") != tempMessage)
    {
      pSuper->trace(&tempMessage, 1, NSSuper::trWarning) ;
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
    }
    if (string("") != tempError)
    {
      pSuper->trace(&tempError, 1, NSSuper::trError) ;
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
    }
    return false ;
	}

  ps = string("Pilot service \"searchPatient\" ended") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // Check that patient graph is not empty and valid
  //
  if (((NSDataGraph*) NULL == _GraphPerson.getDataGraph()) ||
    	(false == _GraphPerson.graphPrepare()))
  {
    if ((NSDataGraph*) NULL == _GraphPerson.getDataGraph())
      ps = string("Empty graph.") ;
    else
      ps = string("Failure of the graphPrepare function.") ;
    pSuper->trace(&ps, 1, NSSuper::trWarning) ;
		return false ;
  }

  ps = string("Patient's graph prepared") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // Address "locking" and "read only" issues
  //
  _GraphPerson.setNeedUnlock(false) ;
	_GraphPerson.setReadOnly(true) ;

  if (false == PatientList.empty())
	{
		string sIsLocked = PatientList.getAttributeValue(TO_UNLOCK) ;
		if (string("ok") == sIsLocked)
			_GraphPerson.setNeedUnlock(true) ;
		string sOperationType	= PatientList.getAttributeValue(OPERATION_TYPE) ;
		if (string("readWrite") == sOperationType)
			_GraphPerson.setReadOnly(false) ;
	}

  if (true == _GraphPerson.getReadOnly())
  {
    string sLockingConsoleId = PatientList.getAttributeValue(LOCKED) ;
    string sWarnText = string("") ;
		if (string("") != sLockingConsoleId)
      sWarnText = pSuper->getText("privilegeManagement", "patientFileIsAlreadyLocked") + string(" (") + sLockingConsoleId + string(")") ;
    else
      sWarnText = pSuper->getText("privilegeManagement", "patientFileIsReadOnly") ;

    string sCaption = pSuper->getAppName() ;
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sWarnText.c_str(), sCaption.c_str(), MB_OK) ;
  }

  ps = string("Locking and read-only issues adressed") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	_GraphPerson.setInfoPids(&LocalAttrArray) ;
	_GraphPerson.getDataGraph()->setLastTree() ;

  return true ;
}
catch (...)
{
	erreur("Exception NSPatInfo::open.", standardError, 0) ;
	return false ;
}
}

bool
NSPatInfo::initFromPersonInfo(NSPersonInfo *pPersInfo)
{
  if ((NSPersonInfo*) NULL == pPersInfo)
    return false ;

  setNss(pPersInfo->getNss()) ;
  setNom(pPersInfo->getNom()) ;
  setPrenom(pPersInfo->getPrenom()) ;
  setCode(string("")) ;
  setNomJF(string("")) ;
  setIpp(string("")) ;
  setMergedWith(string("")) ;
  setConvoc(pPersInfo->getConvoc()) ;
  setSexe(string("")) ;
  setAdresse(string("")) ;
  setNaissance(pPersInfo->getNaissance()) ;
  setTelePor(string("")) ;
  setTeleBur(string("")) ;
  setSitfam(string("")) ;
  setLang(pPersInfo->getLang()) ;
  setCivilite(string("")) ;
  setNomLong(pPersInfo->getNomLong()) ;
  setLockedCons(string("")) ;

  return true ;
}

void
NSPatInfo::fabriqueNomLong(string sLang)
{
	if (string("") == sLang)
		sLang = pContexte->getUserLanguage() ;

	if (initGraphs())
	{
  	// on fabrique dans ce cas le nom long à partir du graphe
    string sCivilite ;
    if (_GraphPerson.trouveCivilite(pContexte, sCivilite, sLang))
    	_Donnees._sNomLong = sCivilite ;
    else
    	_Donnees.fabriqueNomLong(sLang) ;
  }
  else
  	// on fabrique le nom long à partir des pDonnees
    _Donnees.fabriqueNomLong(sLang) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::initGraphs()
//  Description:	Initialise les graphes du patient.
//  Retour:			true->les graphes sont initialisés false->echec
//---------------------------------------------------------------------------
bool
NSPatInfo::initGraphs()
{
  return true ;
}

//---------------------------------------------------------------------------
//  bloquer()
//
//  Efface le patient du fichier de blocage
//---------------------------------------------------------------------------
void
NSPatInfo::debloquer()
{
try
{
	if (false == getADebloquer())
		return ;

	NSBasicAttributeArray BAttrArray ;
  BAttrArray.push_back(new NSBasicAttribute(CONSOLE,	pContexte->getSuperviseur()->getConsoleString())) ;
  BAttrArray.push_back(new NSBasicAttribute(INSTANCE,	pContexte->getSuperviseur()->getInstanceString())) ;
  BAttrArray.push_back(new NSBasicAttribute(PERSON,		getNss())) ;
  if (false == pContexte->getSuperviseur()->getPilot()->unlockPatient(NautilusPilot::SERV_UNLOCK.c_str(), &BAttrArray))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "unlockError") ;
    return ;
  }

	setADebloquer(false) ;
}
catch (...)
{
	erreur("Exception debloquer.", standardError, 0) ;
}
}

bool
NSPatInfo::ChercheChemin(string sChemin, NSSearchStruct* pSearchStruct, bool reinit_structure)
{	if (((NSHISTODocument*) NULL == _pDocHis) || (_pDocHis->getVectDocument()->empty()))		return false ;  if ((NSSearchStruct*) NULL == pSearchStruct)    return false ;  if (true == reinit_structure)   // reinit the research struct    pSearchStruct->reinit() ;  if (string("") == sChemin)    return true ;  string sTrace = string("NSPatInfo::ChercheChemin Looking for path ") + sChemin ;  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;  std::string sCheminPatho = sChemin ;  // ComposeNoeudsChemin(sChemin, sCheminPatho) ;  //  // Recherche dans la synthèse  //  SearchInSynthesis(sChemin, pSearchStruct) ;	//	// Recherche dans les Index de santé - Searching in health indexes	//  SearchInIndexes(sChemin, pSearchStruct) ;  //  // Recherche dans les autres documents  //  sTrace = string("NSPatInfo::ChercheChemin Searching in documents for path ") + sChemin ;  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;  size_t iDocsCounter        = 0 ;  size_t iScannedDocsCounter = 0 ;  size_t iScannedMetaCounter = 0 ;  DocumentIter DocItEnd = _pDocHis->getVectDocument()->end() ;  // Preparing the documents iterator end	for (DocumentIter DocIt = _pDocHis->getVectDocument()->begin() ; DocIt != DocItEnd ; DocIt++)	{    iDocsCounter++ ;		string sDateDoc = (*DocIt)->getDateDoc() ;  // Recupere la date du document		if ((string("") != pSearchStruct->_sEndingDate) && (sDateDoc < pSearchStruct->_sEndingDate)) // La date finale est passe			break ;		// LA seule facon d'etre sur quon a les n plus recents est de verifier qu'on a n reponses		// plus recents que le documents en cours		if ((pSearchStruct->_iNbNodes > 0) && (pSearchStruct->getFoundNodes()->getNunberOfResultBeforeDate(sDateDoc) >= pSearchStruct->_iNbNodes))   // si la date du document est superieur a la derniere date			break ;    // Is this document a Frame index    //    bool bIsIndex     = IsDocumentAFrameIndex(*DocIt) ;    bool bIsSynthesis = IsDocumentASynthesis(*DocIt) ;    if ((false == bIsSynthesis) && (false == bIsIndex))    {      // This document is based on a (not empty) patpatho      //      NSPatPathoInfo* pRootNode = (*DocIt)->getRootPathoInfo() ;      if (pRootNode)      {        bool bValidDocument = true ;        bool bMandatoryDocument = false ;        if (string("") != pSearchStruct->_sMandatoryPath)        {          bMandatoryDocument = true ;          if (false == pSearchStruct->_bMandatoryForYesOrNo)          {            if (false == (*DocIt)->CheminDansPatpatho(pSearchStruct->_sMandatoryPath, string(1, cheminSeparationMARK)))              bValidDocument = false ;          }          // Mandatory path must be tested for both yes and no          else          {            if ((false == (*DocIt)->CheminDansPatpatho(pSearchStruct->_sMandatoryPath + string(1, intranodeSeparationMARK) + string("WCEA0"), string(1, cheminSeparationMARK))) &&                (false == (*DocIt)->CheminDansPatpatho(pSearchStruct->_sMandatoryPath + string(1, intranodeSeparationMARK) + string("WCE00"), string(1, cheminSeparationMARK))))              bValidDocument = false ;          }        }        if (bValidDocument)        {          iScannedDocsCounter++ ;          SearchInDocument(sChemin, pSearchStruct, *DocIt) ;          if (bMandatoryDocument)            break ;        }      }      //      // This document is not based on a (not empty) patpatho but is described by      // a meta      //      else      {        NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;        (*DocIt)->initFromMeta(&PptMeta) ;        if (false == PptMeta.empty())        {          // on vérifie au préalable qu'il existe un chapitre "données de suivi"          PatPathoConstIter pptItBegin = PptMeta.ChercherItem("9SUIV") ;          if ((PptMeta.end() != pptItBegin) && (NULL != pptItBegin))          {            string sDateDoc = donne_date_duJour() ;            iScannedMetaCounter++ ;            PatPathoIter pptIt ;            while (PptMeta.CheminDansPatpatho(sChemin, string(1, cheminSeparationMARK), &pptIt, &pptItBegin))            {              std::string info_date = PptMeta.getNodeDate(pptIt) ; // recupere l'information sur la date              if (std::string("") == info_date)                info_date = sDateDoc ;              if ((NULL != pptIt) && (PptMeta.end() != pptIt))              {                string sRefDate = string("") ;                if (isValidRegularNode(&PptMeta, pptIt, pSearchStruct, sDateDoc, sRefDate))                  pSearchStruct->getFoundNodes()->insert(sRefDate, std::string((*pptIt)->getNode())) ;              }              if ((NULL == pptIt) || (PptMeta.end() == pptIt))                break ;              // pptItBegin = pptIt++ ;              pptItBegin = ++pptIt ;              if (PptMeta.end() == pptItBegin)                break ;            }          }        }      }    }	}	pSearchStruct->create_result() ;  sTrace = string("NSPatInfo::ChercheChemin Leaving search for path ") + sChemin ;  sTrace += string(" (for ") + IntToString(iDocsCounter) + string(" documents : ") + IntToString(iScannedDocsCounter) + string(" scanned and ") + IntToString(iScannedMetaCounter) + string(" metas)") ;  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;	return true ;}

bool
NSPatInfo::SearchInSynthesis(string sChemin, NSSearchStruct *pSearchStruct)
{
  if ((NSSearchStruct*) NULL == pSearchStruct)
    return false ;

  string sTrace = string("NSPatInfo::SearchInSynthesis Looking for path ") + sChemin ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  NSDocumentHisto* pSynthDoc = GetSynthesisDocument() ;

  if ((NSDocumentHisto*) NULL == pSynthDoc)
  {    sTrace = string("NSPatInfo::SearchInSynthesis Looking for path ") + sChemin + string(" - synthesis not found, leaving") ;    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;    return true ;  }

  // Synthesis is supposed to be dated as today
  //  string sDateDoc = donne_date_duJour() ;

  bool bResult = pSynthDoc->SearchInDocument(sChemin, pSearchStruct, sDateDoc) ;

  sTrace = string("NSPatInfo::SearchInSynthesis Leaving search for path ") + sChemin ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  return bResult ;
}

//
// Recherche dans les Index de santé - Searching in health indexes//
bool
NSPatInfo::SearchInIndexes(string sChemin, NSSearchStruct *pSearchStruct)
{
  if (((NSSearchStruct*) NULL == pSearchStruct) || (string("") == sChemin))
    return false ;

  string sTrace = string("NSPatInfo::SearchInIndexes Looking for path ") + sChemin ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  // Health indexes are supposed to be dated as today
  //  string sDateDoc = donne_date_duJour() ;
  ClasseStringVector aPathElements ;
  DecomposeChaine(&sChemin, &aPathElements, string(1, cheminSeparationMARK)) ;
  if (aPathElements.empty())
    return false ;

  string sPathRoot = (*(aPathElements.begin()))->getItem() ;

  for (int iF = 0 ; iF < FRAMECOUNT ; iF++)  {    LDVFRAME iFrame = getFrameForIndex(iF) ;    NSFrameDocuments* pFrameDocs = _pFramesDocuments->getFrameDocuments(iFrame) ;    if (pFrameDocs)    {      NSDocumentHisto* pIndexDoc = pFrameDocs->getIndexDocument() ;      if (pIndexDoc)      {        string sRootFrame     = getRootForFrame(iFrame) ;        string sRootFrameSens = string("") ;        pContexte->getDico()->donneCodeSens(&sRootFrame, &sRootFrameSens) ;        // Get the Patpatho for this index        //        NSPatPathoArray* pPatho_index = pIndexDoc->getPatPathoPointer() ;        if (pPatho_index && (false == pPatho_index->empty()))        {          PatPathoConstIter pptItBegin = pPatho_index->begin() ;          // First case: the searched for pathway starts with ZPOMR -> standard search          //          if (sPathRoot == sRootFrameSens)          {            // First, look inside index            //            string sTrace = string("NSPatInfo::SearchInIndexes Looking into Index Tree for path ") + sChemin ;            pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;            PatPathoIter pptIt = pPatho_index->begin() ;    // Get an iterator            while (pPatho_index->CheminDansPatpatho(sChemin, string(1, cheminSeparationMARK), &pptIt, &pptItBegin))            {              if ((NULL != pptIt) && (pPatho_index->end() != pptIt))              {                string sRefDate = string("") ;                if (isValidIndexNode(pPatho_index, pptIt, pSearchStruct, sRefDate))                  pSearchStruct->getFoundNodes()->insert(sRefDate, std::string((*pptIt)->getNode())) ;              }              if ((NULL == pptIt) || (pPatho_index->end() == pptIt))                break ;              // pptItBegin = pptIt++ ;              pptItBegin = ++pptIt ;              if (pPatho_index->end() == pptItBegin)                break ;            }            //            // Then look inside extensions            //            iterClassString eltsIt = aPathElements.begin() ;            eltsIt++ ;            if (aPathElements.end() != eltsIt)            {              string sSecondElement = (*eltsIt)->getItem() ;              // Build path without the "ZPOMR" root              //              string sPostRootPath = sSecondElement ;              eltsIt++ ;              while (aPathElements.end() != eltsIt)              {                sPostRootPath += string(1, cheminSeparationMARK) + (*eltsIt)->getItem() ;                eltsIt++ ;              }              string sTrace = string("NSPatInfo::SearchInIndexes Looking into Extension Trees for path ") + sPostRootPath ;              pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;              // If the second element is a chapter, then just look for in              // specific extensions... if not, all extensions must be scanned              //              if      (string("0PRO1") == sSecondElement)                SearchInIndexExtensions(sPostRootPath, pFrameDocs->getConcernDocs(), pSearchStruct) ;              else if (string("0OBJE") == sSecondElement)                SearchInIndexExtensions(sPostRootPath, pFrameDocs->getGoalDocs(), pSearchStruct) ;              else if (string("N0000") == sSecondElement)                SearchInIndexExtensions(sPostRootPath, pFrameDocs->getTreatmentDocs(), pSearchStruct) ;              else              {                SearchInIndexExtensions(sPostRootPath, pFrameDocs->getConcernDocs(), pSearchStruct) ;                SearchInIndexExtensions(sPostRootPath, pFrameDocs->getGoalDocs(), pSearchStruct) ;                SearchInIndexExtensions(sPostRootPath, pFrameDocs->getTreatmentDocs(), pSearchStruct) ;              }            }          }          // Second case, the search is not oriented toward the health index, then take          // care of not searching in the "risk" or "goal" section unless explicitly          // specified          //          // If this frame has a "RootLabel" (like the risk), don't even search into          // the "concerns" section          //          else          {            PatPathoConstIter pptItCol_1 = pPatho_index->ChercherPremierFils(pptItBegin) ;            bool bSearchInGoals = false ;            if (string("0OBJE") == sPathRoot)              bSearchInGoals = true ;            bool bSearchInRisk = false ;            if (string("ORISK") == sPathRoot)              bSearchInRisk = true ;            sTrace = string("NSPatInfo::SearchInIndexes Searching in main index") ;            pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;            PatPathoIter pptIt = pPatho_index->begin() ;            // Looking inside index            // Just look in Goals and Risk if explicitely specified            // Just look in Concern if no RootLabel for this frame (Risk has a label frame)            //            while ((NULL != pptItCol_1) && (pPatho_index->end() != pptItCol_1))            {              string sChapter = (*pptItCol_1)->getLexiqueSens() ;              if (((string("0OBJE") != sChapter) || bSearchInGoals) &&                  ((string("ORISK") != sChapter) || bSearchInRisk) &&                  ((string("0PRO1") != sChapter) || (string("") == getRootLabelForFrame(iFrame))))              {                PatPathoConstIter pptItBegin = pptItCol_1 ;                // pptItBegin : search must return path located beyond this node                // pptItCol_1 : search must occur inside this node's sons subtree                //                while (pPatho_index->CheminDansPatpatho(sChemin, string(1, cheminSeparationMARK), &pptIt, &pptItBegin, &pptItCol_1))                {                  if ((NULL != pptIt) && (pPatho_index->end() != pptIt))                  {                    string sRefDate = string("") ;                    if (isValidIndexNode(pPatho_index, pptIt, pSearchStruct, sRefDate))                      pSearchStruct->getFoundNodes()->insert(sRefDate, std::string((*pptIt)->getNode())) ;                  }                  if ((NULL == pptIt) || (pPatho_index->end() == pptIt))                    break ;                  // pptItBegin = pptIt++ ;                  pptItBegin = ++pptIt ;                  if ((pPatho_index->end() == pptItBegin) || ((*pptItBegin)->getColonne() == 1))                    break ;                }              }              if (pPatho_index->end() != pptItCol_1)                pptItCol_1 = pPatho_index->ChercherFrereSuivant(pptItCol_1) ;            }            // Look inside extensions            //            sTrace = string("NSPatInfo::SearchInIndexes Searching in index extensions") ;            pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;            if      (bSearchInGoals)              SearchInIndexExtensions(sChemin, pFrameDocs->getGoalDocs(), pSearchStruct) ;            else if (bSearchInRisk)              SearchInIndexExtensions(sChemin, pFrameDocs->getTreatmentDocs(), pSearchStruct) ;            else              SearchInIndexExtensions(sChemin, pFrameDocs->getConcernDocs(), pSearchStruct) ;          }        }      }    }  }

  sTrace = string("NSPatInfo::SearchInIndexes Leaving search for path ") + sChemin ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  return true ;
}

bool
NSPatInfo::SearchInIndexExtensions(string sChemin, vector<NSDocumentHisto*> *pConcernDocs, NSSearchStruct *pSearchStruct)
{
  string sTrace = string("NSPatInfo::SearchInIndexExtensions Entering for path ") + sChemin ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  if ((string("") == sChemin) || (NULL == pConcernDocs) || (NULL == pSearchStruct))
  {
    sTrace = string("NSPatInfo::SearchInIndexExtensions Bad parameters, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return false ;
  }

  if (pConcernDocs->empty())
  {
    sTrace = string("NSPatInfo::SearchInIndexExtensions: No extension found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    return true ;
  }

  // Health indexes are supposed to be dated as today
  //  string sDateDoc = donne_date_duJour() ;
  for (DocumentIter docIt = pConcernDocs->begin() ; pConcernDocs->end() != docIt ; docIt++)
  {
    NSPatPathoArray PathoExtension(pContexte->getSuperviseur()) ;
    (*docIt)->initFromPatPatho(&PathoExtension) ;

    if (false == PathoExtension.empty())    {      PatPathoConstIter pptItBegin = PathoExtension.begin() ;      PatPathoIter      pptIt      = PathoExtension.begin() ;   // Get an iterator      while (PathoExtension.CheminDansPatpatho(sChemin, string(1, cheminSeparationMARK), &pptIt, &pptItBegin))      {        if ((NULL != pptIt) && (PathoExtension.end() != pptIt))        {          string sRefDate = string("") ;          if (isValidIndexNode(&PathoExtension, pptIt, pSearchStruct, sRefDate))            pSearchStruct->getFoundNodes()->insert(sDateDoc, std::string((*pptIt)->getNode())) ;        }        if ((NULL == pptIt) || (PathoExtension.end() == pptIt))          break ;        // pptItBegin = pptIt++ ;        pptItBegin = ++pptIt ;        if (PathoExtension.end() == pptItBegin)          break ;      }
    }
  }

  sTrace = string("NSPatInfo::SearchInIndexExtensions: Leaving") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  return true ;
}

// Check if a node (whose path is ok) is valid for NSSearchStruct
//
bool
NSPatInfo::isValidIndexNode(NSPatPathoArray *pPPT, PatPathoIter pptIter, NSSearchStruct *pSearchStruct, string &sRefDate)
{
  string sDateDoc = donne_date_duJour() ;

  return isValidRegularNode(pPPT, pptIter, pSearchStruct, sDateDoc, sRefDate) ;
}

// Check if a node from index tree (whose path is ok) is valid for NSSearchStruct
//
bool
NSPatInfo::isValidRegularNode(NSPatPathoArray *pPPT, PatPathoConstIter pptIter, NSSearchStruct *pSearchStruct, string sDateDoc, string &sRefDate)
{
  if (((NSPatPathoArray *) NULL == pPPT) || (NULL == pptIter) || (pPPT->end() == pptIter))
    return false ;

  if (NULL == pSearchStruct)
    return true ;

  // Check Episode dates
  //  if (NSSearchStruct::episodeUndefined != pSearchStruct->getEpisodeStatus())  {    string sOpeningDate = pPPT->getNodeOpenDate(pptIter) ;    string sClosingDate = pPPT->getNodeCloseDate(pptIter) ;    if (false == pSearchStruct->accept_episode(sOpeningDate, sClosingDate))      return false ;  }  // Check archetype  //  if ((string("") != pSearchStruct->getMandatoryArchetype()))  {    string sArchetypeId = pPPT->getDirectArchetypeId(pptIter) ;    if (sArchetypeId != pSearchStruct->getMandatoryArchetype())      return false ;  }

  // Check peremption date
  //
  string sDate = pPPT->getNodeDate(pptIter) ;
  if (string("") == sDate)  {    if (false == pSearchStruct->accept_date(sDateDoc))      return false ;    sRefDate = sDateDoc ;  }  else  {    if (false == pSearchStruct->accept_date(sDate))      return false ;    sRefDate = sDate ;  }
  return true ;
}

bool
NSPatInfo::SearchInDocument(string sChemin, NSSearchStruct *pSearchStruct, NSDocumentHisto* pDocument)
{
  if (((NSSearchStruct*)  NULL == pSearchStruct) ||
      ((NSDocumentHisto*) NULL == pDocument))
    return false ;

  return pDocument->SearchInDocument(sChemin, pSearchStruct) ;
}

bool
NSPatInfo::IsDocumentAFrameIndex(NSDocumentHisto* pDocument)
{
  if (NULL == pDocument)
    return false ;

  string name     = pDocument->getContent() ;
  string nameSens = string("") ;  pContexte->getDico()->donneCodeSens(&name, &nameSens) ;  // For extension trees  //  if ((string("0PRO1") == nameSens) ||      (string("0OBJE") == nameSens) ||      (string("N0000") == nameSens))    return true ;  // Is this document a Frame index  //  for (int iF = 0 ; iF < FRAMECOUNT ; iF++)  {    string sRootFrame     = getRootForFrame(getFrameForIndex(iF)) ;    string sRootFrameSens = string("") ;    pContexte->getDico()->donneCodeSens(&sRootFrame, &sRootFrameSens) ;    if (sRootFrameSens == nameSens)      return true ;  }

  return false ;
}

bool
NSPatInfo::IsDocumentASynthesis(NSDocumentHisto* pDocument)
{
  if (NULL == pDocument)
    return false ;

  string name     = pDocument->getContent() ;
  string nameSens = string("") ;  pContexte->getDico()->donneCodeSens(&name, &nameSens) ;
  return (string("ZSYNT") == nameSens) ;
}

string
NSPatInfo::getQueryPathModifiers(string& sQueryPath)
{
  size_t posModifs = sQueryPath.find(":") ;
  if (string::npos == posModifs)
    return string("") ;

  string sModifiers = string("") ;
  if (posModifs < strlen(sQueryPath.c_str()) - 1)
    sModifiers = string(sQueryPath, posModifs+1, strlen(sQueryPath.c_str())-posModifs-1) ;

  sQueryPath = string(sQueryPath, 0, posModifs) ;

  return sModifiers ;
}

bool
NSPatInfo::getReadOnly()
{
  return _GraphPerson.getReadOnly() ;
}

bool
NSPatInfo::getADebloquer()
{
  return _GraphPerson.getNeedUnlock() ;
}

void
NSPatInfo::setReadOnly(bool bRO)
{
  _GraphPerson.setReadOnly(bRO) ;
}

void
NSPatInfo::setADebloquer(bool bAD)
{
  _GraphPerson.setNeedUnlock(bAD) ;
}

// Returns tree ID if found, string("") if not
//
string
NSPatInfo::getAdminData(NSPatPathoArray* pPpt)
{
  return getSingletonTree(NSRootLink::personAdminData, pPpt) ;
}

// Returns tree ID if found, string("") if not
//
string
NSPatInfo::getSingletonTree(NSRootLink::NODELINKTYPES iType, NSPatPathoArray* pPpt)
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return string("") ;

  NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
  if ((NSLinkManager*) NULL == pGraphe)
    return string("") ;

  // Get label tree ID
  //
  NSDataGraph* pDataGraph = _GraphPerson.getDataGraph() ;
  if ((NSDataGraph*) NULL == pDataGraph)
    return string("") ;

  string sCodeDocRoot = pDataGraph->getGraphID() ;
  VecteurString aVecteurString ;
  pGraphe->TousLesVrais(sCodeDocRoot, iType, &aVecteurString) ;

  if (aVecteurString.empty())
  {
    string ps = string("getSingletonTree for ") + pGraphe->donneString(iType) + string(" failed because no node was found for this link.") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
    return string("") ;
  }

  // Get data tree ID
  //
  string sCodeDocLabel = *(*(aVecteurString.begin())) ;
  aVecteurString.vider() ;
  pGraphe->TousLesVrais(sCodeDocLabel, NSRootLink::docData, &aVecteurString) ;

  if (aVecteurString.empty())
  {
    string ps = string("getSingletonTree for ") + pGraphe->donneString(iType) + string(" failed because no link to data tree was found for this link.") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
    return string("") ;
  }

  string sCodeDocData = *(*(aVecteurString.begin())) ;

  string sRosace = string("") ;
  if (false == _GraphPerson.getTree(sCodeDocData, pPpt, &sRosace))
  {
    string ps = string("getSingletonTree for ") + pGraphe->donneString(iType) + string(" failed because no data tree was found for this link.") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
    return string("") ;
  }

  return sCodeDocData ;
}

NSDocumentHisto*
NSPatInfo::GetDocument(string sRootSens)
{
  if (string("") == sRootSens)
    return (NSDocumentHisto*) 0 ;

  if (((NSHISTODocument*) NULL == _pDocHis) || (_pDocHis->getVectDocument()->empty()))
		return (NSDocumentHisto*) 0 ;

  NSDocHistoArray* pVectDocuments = _pDocHis->getVectDocument() ;
  if (((NSDocHistoArray*) NULL == pVectDocuments) || (pVectDocuments->empty()))
		return (NSDocumentHisto*) 0 ;

  DocumentReverseIter iterReverseDocEnd = pVectDocuments->rend() ;
	for (DocumentReverseIter iterReverseDoc = pVectDocuments->rbegin() ; iterReverseDoc != iterReverseDocEnd ; iterReverseDoc++)
	{
    NSPatPathoInfo* pRootNode = (*iterReverseDoc)->getRootPathoInfo() ;

    if (pRootNode)
    {
      string sDocRootSens = pRootNode->getLexiqueSens() ;

      if (sDocRootSens == sRootSens)
        return *iterReverseDoc ;
		}
	}

  return (NSDocumentHisto*) 0 ;
}

void
NSPatInfo::GetDocPatho(NSPatPathoArray *pPatho, string sRootSens)
{
  if ((NSPatPathoArray*) NULL == pPatho)
		return ;

  pPatho->vider() ;

  NSDocumentHisto* pDoc = GetDocument(sRootSens) ;
  if (pDoc)
    pDoc->initFromPatPatho(pPatho) ;
}

// -----------------------------------------------------------------------------
// Renvoie la patpatho fille du noeud
// -----------------------------------------------------------------------------
// ajout fab - 2003/08/13 -- on cherche également dans la patpatho META
// -----------------------------------------------------------------------------
bool
NSPatInfo::DonneSousArray(string sNoeud, NSPatPathoArray *pPPT)
{  if ((NSPatPathoArray*) NULL == pPPT)    return false ;	if ((NULL == _pDocHis) || (_pDocHis->getVectDocument()->empty()))		return false ;  if (strlen(sNoeud.c_str()) != PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + PPD_NOEUD_LEN)    return false ;	string sDocument = string(sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;	DocumentIter DocIt = _pDocHis->getVectDocument()->begin() ;	for ( ; DocIt != _pDocHis->getVectDocument()->end() ; DocIt++)	{    NSPatPathoArray IterPPT(pContexte->getSuperviseur()) ;    (*DocIt)->initFromPatPatho(&IterPPT) ;		if (false == IterPPT.empty())		{			PatPathoIter pptIt = IterPPT.begin() ;      if ((*pptIt)->getDoc() == sDocument)      {        for ( ; (IterPPT.end() != pptIt) && ((*pptIt)->getNode() != sNoeud) ; pptIt++)          ;        if (IterPPT.end() != pptIt)        {          IterPPT.ExtrairePatPatho(pptIt, pPPT) ;          return true ;        }      }      else      {        NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;        (*DocIt)->initFromMeta(&PptMeta) ;        if (false == PptMeta.empty())        {          PatPathoIter pptMetaIter = PptMeta.begin() ;          if ((*pptMetaIter)->getDoc() == sDocument)          {            for ( ; (PptMeta.end() != pptMetaIter) && ((*pptMetaIter)->getNode() != sNoeud) ; pptMetaIter++)              ;            if (PptMeta.end() != pptMetaIter)            {              PptMeta.ExtrairePatPatho(pptMetaIter, pPPT) ;              return true ;            }          }        }      }		}	}	return false ;}
// -----------------------------------------------------------------------------
// Renvoie une patpatho qui contient le noeud, sa fille et les même données
// pour les frères du noeud qui lui sont identiques
// -----------------------------------------------------------------------------
// ajout fab - 2003/08/13 -- on cherche également dans la patpatho META
// -----------------------------------------------------------------------------
bool
NSPatInfo::DonneArray(string sNoeud, NSPatPathoArray *pPPT)
{  if ((NSPatPathoArray*) NULL == pPPT)    return false ;	if ((NULL == _pDocHis) || (_pDocHis->getVectDocument()->empty()))		return false ;      if (strlen(sNoeud.c_str()) != PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + PPD_NOEUD_LEN)    return false ;	string sDocument = string(sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;	DocumentIter DocIt = _pDocHis->getVectDocument()->begin() ;	for ( ; DocIt != _pDocHis->getVectDocument()->end() ; DocIt++)	{    NSPatPathoArray* pIterPPT = (*DocIt)->getPatPathoPointer() ;		if (pIterPPT && (false == pIterPPT->empty()))		{			PatPathoIter pptIt = pIterPPT->begin() ;      if ((*pptIt)->getDoc() == sDocument)      {        for ( ; (pIterPPT->end() != pptIt) && ((*pptIt)->getNode() != sNoeud) ; pptIt++)          ;        if (pIterPPT->end() != pptIt)        {          pIterPPT->ExtrairePatPathoFreres(pptIt, pPPT) ;          return true ;        }      }      else      {        NSPatPathoArray* pPptMeta = (*DocIt)->getMetaPointer() ;        if (pPptMeta && (false == pPptMeta->empty()))        {          PatPathoIter pptMetaIter = pPptMeta->begin() ;          if ((*pptMetaIter)->getDoc() == sDocument)          {            for ( ; (pPptMeta->end() != pptMetaIter) && ((*pptMetaIter)->getNode() != sNoeud) ; pptMetaIter++)              ;            if (pPptMeta->end() != pptMetaIter)            {              pPptMeta->ExtrairePatPathoFreres(pptMetaIter, pPPT) ;              return true ;            }          }        }      }		}	}	return false ;}
//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::initCorrespArray()
//  Description:	Recherche les correspondants du patient.
//  Retour:			true->le tableau des corresp est initialisé false->echec
//---------------------------------------------------------------------------
bool
NSPatInfo::initCorrespArray(NSPatPathoArray* pPatPathoArray)
{
	if (((NSPatPathoArray*) NULL == pPatPathoArray) || (pPatPathoArray->empty()))
  	return false ;

  _CorrespArray.vider() ;

  //
  // On part du principe que les données qui nous intéressent sont dans un
  // sous chapitre LCTAC (contacts)
  //
  // We suppose that the values we need are in a sub-chapter LCTAC (contact)
  //
  string sType ;

  string sFonction = string("") ;
  string sCode     = string("") ;

  string sTemp     = string("") ;

  PatPathoIter iter = pPatPathoArray->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sElemLex = (*iter)->getLexique() ;
    string sSens    = string("") ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    // Chapitre "contacts" / Contact chapter
    if (string("LCTAC") == sSens)
    {
      int iColContact = (*iter)->getColonne() ;
      iter++ ;

      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColContact))
      {
        sElemLex = (*iter)->getLexique() ;
        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

        // Liste des correspondants
        if (string("LCORR") == sSens)
        {
          iter++ ;
          while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColContact + 1))
          {
            sElemLex = (*iter)->getLexique() ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

            // Correspondant : comprend fonction et person_id
            if (string("DCORR") == sSens)
            {
              iter++ ;
              bool   bCorresp  = false ;
              string sFonction = string("") ;
              string sCode     = string("") ;

              while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColContact + 2))
              {
                sElemLex = (*iter)->getLexique() ;
                pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

                // Correspondant : comprend fonction et person_id
                if (string("LFONC") == sSens)
                  sFonction = pPatPathoArray->getFreeTextValue(&iter) ;

                else if (string("HHUMA") == sSens)
                {
                  iter++ ;
                  while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColContact + 3))
                  {
                    // on cherche ici un identifiant
                    sElemLex = (*iter)->getLexique() ;
                    if (string(sElemLex, 0, 5) == string("£SPID"))
                    {
                      sCode    = (*iter)->getComplement() ;
                      bCorresp = true ;
                    }
                    iter++ ;
                  }
                }
                else
                  iter++ ;
              }

              if (bCorresp)
              {
                NSPidsInfo PidsInfo(pContexte) ;
                initCorrespPIDS(&PidsInfo, sCode) ;
                PidsInfo.sFonction = sFonction ;

                _CorrespArray.push_back(new NSPidsInfo(PidsInfo)) ;
              }
            }
            else
              iter++ ;
          }
        }
        else
          iter++ ;
      }
    }
    else
      iter++ ;
  }

	return true ;
}

bool
NSPatInfo::getFoldersPatho(NSPatPathoArray* pPatPathoArray)
{
  if ((NSPatPathoArray*) NULL == pPatPathoArray)
    return false ;

  pPatPathoArray->vider() ;

  if (_pDocHis && _pDocHis->getLibChem())
  {
    _pDocHis->getLibChem()->initFromPatPatho(pPatPathoArray) ;
    return true ;
  }

  string sDocRoot = _GraphPerson.getRootTree() ;
  NSLinkManager* pGraphe = _GraphPerson.getLinkManager() ;
  VecteurString aVecteurString ;
  pGraphe->TousLesVrais(sDocRoot, NSRootLink::personFolderLibrary, &aVecteurString) ;
  if (aVecteurString.empty())
    return false ;

  string sCodeDocLibChem = *(*(aVecteurString.begin())) ;
  NSDocumentInfo docInf(sCodeDocLibChem, pContexte, &_GraphPerson) ;
  docInf.LoadMetaAndData(&_GraphPerson) ;
  docInf.DonnePatPatho(pPatPathoArray, &_GraphPerson) ;

  return true ;
}

bool
NSPatInfo::getFoldersArray(NSChemiseArray* pChemisesArray)
{
  if ((NSChemiseArray*) NULL == pChemisesArray)
    return false ;

  NSPatPathoArray FoldersArray(pContexte->getSuperviseur()) ;

  if (false == getFoldersPatho(&FoldersArray))
    return false ;

  if (FoldersArray.empty())
    return false ;

  return pChemisesArray->initFromPatpatho(&FoldersArray) ;
}

bool
NSPatInfo::CreeContribution(bool bInitiale)
{
try
{
	string sDateJour = donne_date_duJour() + donne_heure() ;
	string sPidsUtil = pContexte->getUtilisateurID() ;

	// Construction de la patpatho de la nouvelle contribution
	NSPatPathoArray PatPathoArray(pContexte->getSuperviseur()) ;

	PatPathoArray.ajoutePatho("LCTRI1", 0) ;

  Message Msg ;

	// Utilisateur
	PatPathoArray.ajoutePatho("HUTIL1", 1) ;
	Msg.SetComplement(sPidsUtil.c_str()) ;
	PatPathoArray.ajoutePatho("£SPID1", &Msg, 2) ;

  // Numéro de rendez-vous
  NSCaptureArray* pCapt = &(pContexte->getEpisodus()->newCaptureArray) ;
  string sNumRdv = string("") ;
  pCapt->trouveChemin(string("LCTRI1/LNUMR1"), &sNumRdv) ;
  if (string("") != sNumRdv)
  {
    PatPathoArray.ajoutePatho("LNUMR1", 1) ;
    Msg.Reset() ;
	  Msg.SetComplement(sNumRdv.c_str()) ;
	  PatPathoArray.ajoutePatho("£CC000", &Msg, 2) ;
  }

	// Date d'ouverture
	PatPathoArray.ajoutePatho("KOUVR1", 1) ;
  Msg.Reset() ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(sDateJour) ;
	PatPathoArray.ajoutePatho("£T0;19", &Msg, 2) ;

	//
	// On crée la liaison de la contribution avec le document root	//	// La contribution initiale est enregistrée dans ::createPatient	string sNodeRoot = _GraphPerson.getRootTree() ;	_sContribution = _GraphPerson.setTree(&PatPathoArray, "") ;	NSLinkManager Link(pContexte->getSuperviseur(), _GraphPerson.getDataGraph()) ;	Link.etablirLien(sNodeRoot, NSRootLink::personContribution, _sContribution) ;	if (false == bInitiale)	{		// pour les contributions ultérieures on update directement		NSVectPatPathoArray VectU ;		VectU.push_back(new NSPatPathoArray(PatPathoArray)) ;
		bool bContribCreated = _GraphPerson.updateTrees(pContexte, &VectU, &_sContribution) ;		if (false == bContribCreated)		{			_sContribution = string("") ;			return false ;		}		// sContribution = pGraphPerson->pDataGraph->getLastContribution();	}	return true ;}
catch (...)
{
	erreur("Exception NSPatInfo::CreeContribution.", standardError, 0) ;
	return false;
}
}

bool
NSPatInfo::FermeContribution()
{
try
{
  string sDateJour = donne_date_duJour() + donne_heure() ;

  //
  // on charge l'arbre qui correspond à la contribution en cours
  //
  NSPatPathoArray PatPathoArray(pContexte->getSuperviseur()) ;
  string sRosace = string("") ;
  if (false == _GraphPerson.getTree(_sContribution, &PatPathoArray, &sRosace))
    return false ;

  NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
  PatPatho.ajoutePatho("KFERM1", 0) ;
  Message	Msg ;
  Msg.SetUnit("2DA021") ;
  Msg.SetComplement(sDateJour) ;
  PatPatho.ajoutePatho("£T0;19", &Msg, 1) ;

  PatPathoArray.InserePatPatho(PatPathoArray.end(), &PatPatho, 1) ;

  NSVectPatPathoArray VectU ;
  VectU.push_back(new NSPatPathoArray(PatPathoArray)) ;
  string sNewTreeId ;
  if (false == _GraphPerson.updateTrees(pContexte, &VectU, &sNewTreeId))
    return false ;

  _sContribution = string("") ;

  return true ;
}
catch (...)
{
	erreur("Exception NSPatInfo::FermeContribution.", standardError, 0) ;
	return false ;
}
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

// Update NSPatInfo from a patpatho
//
void
NSPatInfo::ChargeDonneesPatient(NSPatPathoArray* pPatPathoArray)
{
try
{
  initIPP() ;

  if (((NSPatPathoArray*) NULL == pPatPathoArray) || (pPatPathoArray->empty()))
    return ;
  //
  // On part du principe que les données qui nous intéressent sont dans un
  // sous chapitre LIDET (identité)
  // On pourrait imaginer prendre les premières valeurs "nom", "prénom"...
  // rencontrées
  //
  // We suppose that the values we need are in a sub-chapter LIDET (identity)
  // We could imagine we take the first encoutered "1st name", "2nd name"... values
  //

  PatPathoIter iter = pPatPathoArray->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sSens = (*iter)->getLexiqueSens() ;

    // Chapitre "identité" / Identity chapter
    //
    if (string("LIDET") == sSens)
    {
      int iColIdentity = (*iter)->getColonne() ;
      iter++ ;

      string sNom    = string("") ;
      string sPrenom = string("") ;
      string sNomJF  = string("") ;
      string sCode   = string("") ;

      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity))
      {
        sSens = (*iter)->getLexiqueSens() ;

        // Nom du patient
        // Patient's name
        if ((sSens == string("LNOM0")) && (string("") == sNom))
        {
          sNom = pPatPathoArray->getFreeTextValue(&iter) ;

          if (string("") != sNom)
            _Donnees._sNom = sNom ;
        }
        // Prénom du patient
        // Patient's second name
        else if ((string("LNOM2") == sSens) && (string("") == sPrenom))
        {
          sPrenom = pPatPathoArray->getFreeTextValue(&iter) ;

          if (string("") != sPrenom)
            _Donnees._sPrenom = sPrenom ;
        }
        // Nom de jeune fille du patient
        // Patient's maiden name
        else if ((string("LNOM1") == sSens) && (string("") == sNomJF))
        {
          sNomJF = pPatPathoArray->getFreeTextValue(&iter) ;

          if (string("") != sNomJF)
            _Donnees._sNomJeuneFille = sNomJF ;
        }
        // Code patient
        // Patient's code
        else if ((string("LCOD0") == sSens) && (string("") == sCode))
        {
          sCode = pPatPathoArray->getFreeTextValue(&iter) ;

          if (string("") != sCode)
            _Donnees._sCode = sCode ;
        }
        // Sexe du patient
        // Patient's sex
        else if (string("LSEXE") == sSens)
        {
          iter++ ;
          while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity+1))
          {
            // on cherche ici un code lexique
            string sElemLex = (*iter)->getLexique() ;
            string sTemp    = string("") ;
            pContexte->getDico()->donneCodeSens(&sElemLex, &sTemp) ;

            if      (string("HMASC") == sTemp)
              _Donnees.metMasculin() ;
            else if (string("HFEMI") == sTemp)
              _Donnees.metFeminin() ;

            iter++ ;

            while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity + 2))
            {
              sElemLex = string((*iter)->getLexique()) ;
              pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

              // Civilité
              if (string("HCIVO") == sSens)
              {
                iter++ ;
                while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity + 3))
                {
                  // on cherche ici un code lexique pour un libelle
                  string sCodeLex = string((*iter)->getLexique()) ;
                  _Donnees._sCivilite = sCodeLex ;
                  iter++ ;
                }
              }
              else
                iter++ ;
            }
          }
        }
        else if (string("KNAIS") == sSens)
        {
          iter++ ;
          while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity+1))
          {
            string sUnite  = (*iter)->getUnitSens() ;
            string sFormat = (*iter)->getLexiqueSens() ;
            string sValeur = (*iter)->getComplement() ;

            if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
              _Donnees._sNaissance = sValeur ;

            iter++ ;
          }
        }
        else
          iter++ ;
      }
    }
    else
      iter++ ;
  }
}
catch (...)
{
	erreur("Exception NSPatInfo::ChargeDonneesPatient.", standardError, 0) ;
}
}

void
NSPatInfo::initIPP()
{
  NSPatPathoArray pptIdent(pContexte->getSuperviseur()) ;
	if (false == _GraphPerson.getLibIDsPpt(&pptIdent))
    return ;

  string sIPP ;
  if (true == _GraphPerson.IPPEnCours(&pptIdent, string(""), &sIPP))
    setIpp(sIPP) ;
}

string
NSPatInfo::getIPPEnCours(string sSite, string* psOuvre, string* psFerme)
{
	if (psOuvre)
		*psOuvre = string("") ;
  if (psFerme)
		*psFerme = string("") ;

	// Get IDs Library
	//
  NSPatPathoArray pptIdent(pContexte->getSuperviseur()) ;
	if (false == _GraphPerson.getLibIDsPpt(&pptIdent))
		return string("") ;

	string sCurrentIpp ;
	bool bExistIPP = _GraphPerson.IPPEnCours(&pptIdent, sSite, &sCurrentIpp, psOuvre, psFerme) ;
	if (false == bExistIPP)
  	return string("") ;

	return sCurrentIpp ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::operator=(NSPatInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSPatInfo&
NSPatInfo::operator=(const NSPatInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees 	    = src._Donnees ;
	_CorrespArray = src._CorrespArray ;
	pContexte     = src.pContexte ;

	_GraphPerson  = src._GraphPerson ;
	_GraphAdr     = src._GraphAdr ;
	_sChez        = src._sChez ;

/*
  if (NULL != pDocHis)
  {
    delete pDocHis ;
    pDocHis = 0 ;
  }
  if (NULL != src.pDocHis)
    pDocHis = new NSHISTODocument(*(src.pDocHis)) ;
*/

  if (_pHealthTeam)
  {
    delete _pHealthTeam ;
    _pHealthTeam = (NSHealthTeam*) 0 ;
  }
  if (src._pHealthTeam)
    _pHealthTeam = new NSHealthTeam(*(src._pHealthTeam)) ;

  if (_pFramesDocuments)
  {
    delete _pFramesDocuments ;
    _pFramesDocuments = (NSFrameDocumentsArray*) 0 ;
  }
  if (src._pFramesDocuments)
    _pFramesDocuments = new NSFrameDocumentsArray(*(src._pFramesDocuments)) ;

	return *this ;
}

string
NSPatInfo::fabriqueAgeLabel(string sLang)
{
  if (string("") == _Donnees._sNaissance)
    return string("") ;

  NVLdVTemps tNaissance ;
  if (false == tNaissance.initFromDate(_Donnees._sNaissance))
    return string("") ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;

  unsigned long lDaysOfLife = tNow.daysBetween(tNaissance) ;

  if (lDaysOfLife < 3)
    return fabriqueAgeLabelHours(sLang) ;
  if (lDaysOfLife < 90)
    return fabriqueAgeLabelDays(sLang) ;
  if (lDaysOfLife < 700)
    return fabriqueAgeLabelMonths(sLang) ;

  return fabriqueAgeLabelYears(sLang) ;
}

string
NSPatInfo::fabriqueAgeLabelYears(string sLang)
{
  if (string("") == _Donnees._sNaissance)
    return string("") ;

  NSDico* pDico = pContexte->getDico() ;
  if (NULL == pDico)
    return string("") ;

  string sCurrentDate = donne_date_duJour() ;

  int iCurrentAge = donne_age(sCurrentDate, _Donnees._sNaissance) ;
  if (iCurrentAge <= 0)
    return string("") ;

  // Get the lib of "year(s)"
  //
  string sCodeLexAn = string("2DAT33") ;
  NSPathologData Data ;
  if (false == pDico->trouvePathologData(sLang, &sCodeLexAn, &Data))
    return string("") ;

  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (iCurrentAge > 1)
    Data.donneGenrePluriel(&iGenre) ;
  string sLibel ;
  NSGenerateur* pGene = pDico->getGenerateur() ;
  pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

  string sTitre = IntToString(iCurrentAge) + string(" ") + sLibel ;

  return sTitre ;
}

string
NSPatInfo::fabriqueAgeLabelMonths(string sLang)
{
  if (string("") == _Donnees._sNaissance)
    return string("") ;

  NSDico* pDico = pContexte->getDico() ;
  if (NULL == pDico)
    return string("") ;

  string sCurrentDate = donne_date_duJour() ;

  int iCurrentAge = donne_age_mois(sCurrentDate, _Donnees._sNaissance) ;
  if (iCurrentAge <= 0)
    return string("") ;

  // Get the lib of "month(s)"
  //
  string sCodeLexAn = string("2DAT21") ;
  NSPathologData Data ;
  if (false == pDico->trouvePathologData(sLang, &sCodeLexAn, &Data))
    return string("") ;

  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (iCurrentAge > 1)
    Data.donneGenrePluriel(&iGenre) ;
  string sLibel ;
  NSGenerateur* pGene = pDico->getGenerateur() ;
  pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

  string sTitre = IntToString(iCurrentAge) + string(" ") + sLibel ;

  return sTitre ;
}

string
NSPatInfo::fabriqueAgeLabelDays(string sLang)
{
  NVLdVTemps tNaissance ;
  if (false == tNaissance.initFromDate(_Donnees._sNaissance))
    return string("") ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;

  unsigned long lDaysOfLife = tNow.daysBetween(tNaissance) ;
  if (lDaysOfLife <= (unsigned long)0)
    return string("") ;

  NSDico* pDico = pContexte->getDico() ;
  if (NULL == pDico)
    return string("") ;

  // Get the lib of "day(s)"
  //
  string sCodeLexAn = string("2DAT01") ;
  NSPathologData Data ;
  if (false == pDico->trouvePathologData(sLang, &sCodeLexAn, &Data))
    return string("") ;

  int iCurrentAge = int(lDaysOfLife) ;

  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (iCurrentAge > 1)
    Data.donneGenrePluriel(&iGenre) ;
  string sLibel ;
  NSGenerateur* pGene = pDico->getGenerateur() ;
  pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

  string sTitre = IntToString(iCurrentAge) + string(" ") + sLibel ;

  return sTitre ;
}

string
NSPatInfo::fabriqueAgeLabelHours(string sLang)
{
  return string("") ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPatInfo::operator==(NSPatInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSPatInfo::operator==(const NSPatInfo& o)
{
	int egal = 1 ;

	if (!(_Donnees      == o._Donnees))
		egal = 0 ;
	if (!(_CorrespArray == o._CorrespArray))
		egal = 0 ;

  return egal ;
}

//***************************************************************************
//
// Fonction globale StringTitre (utilisée par NSUtilisateur et NSCorrespondant)
// -> Renvoie la string titre en fonction du type (caractère)
//
//***************************************************************************

string StringTitre(NSContexte* pContexte, string sTypeTitre, string sLang)
{
	string titre("") ;

	if (string("") == sTypeTitre)
		return titre ;

	pContexte->getDico()->donneLibelle(sLang, &sTypeTitre, &titre) ;
  titre[0] = pseumaj(titre[0]) ;
  titre += string(" ") ;

/*
  if ((string("") == sLang) || (string("fr") == string(sLang, 0, 2)))
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
*/

	return titre ;
}

//***************************************************************************
//
// Fonction globale StringAvantTitre (utilisée par NSUtilisateur et NSCorrespondant)
// -> Renvoie la string avant titre en fonction du type (caractère)
//
//***************************************************************************

string StringAvantTitre(string sTypeTitre, string sLang)
{
	string avant("") ;

	if (string("") == sTypeTitre)
		return avant ;

	if ((string("") == sLang) || (string("fr") == string(sLang, 0, 2)))
	{
  	if ((string("HDOCT1") == sTypeTitre) ||
        (string("HPROF1") == sTypeTitre))
    	avant = string("au") ;
		else
    	avant = string("à") ;
	}
  else if (string("en") == string(sLang, 0, 2))
  {
  	avant = string("to") ;
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
NSUtilisateurData::NSUtilisateurData(const NSUtilisateurData& rv)
{
	_sNss            = rv._sNss ;
	_sLogin          = rv._sLogin ;
	_sCode           = rv._sCode ;
	_sSexe           = rv._sSexe ;
	_sNom            = rv._sNom ;
	_sPrenom         = rv._sPrenom ;
	_sType           = rv._sType ;
	_sMessagerie     = rv._sMessagerie ;
	_sLang           = rv._sLang ;
	_sMetier         = rv._sMetier ;
	_sSpecialite     = rv._sSpecialite ;
	_sVille          = rv._sVille ;
  _sProfessionalId = rv._sProfessionalId ;
  _sRpps           = rv._sRpps ;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSUtilisateurData::metAZero()
{
	_sNss            = string("") ;
	_sLogin          = string("") ;
	_sCode           = string("") ;
	_sSexe           = string("") ;
	_sNom            = string("") ;
	_sPrenom         = string("") ;
	_sType           = string("") ;
	_sMessagerie     = string("") ;
	_sLang           = string("") ;
	_sMetier         = string("") ;
	_sSpecialite     = string("") ;
	_sVille          = string("") ;
  _sProfessionalId = string("") ;
  _sRpps           = string("") ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::operator=(NSUtilisateurData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSUtilisateurData&
NSUtilisateurData::operator=(const NSUtilisateurData& src)
{
	if (this == &src)
		return *this ;

	_sNss            = src._sNss ;
	_sLogin          = src._sLogin ;
	_sCode           = src._sCode ;
	_sSexe           = src._sSexe ;
	_sNom            = src._sNom ;
	_sPrenom         = src._sPrenom ;
	_sType           = src._sType ;
	_sMessagerie     = src._sMessagerie ;
	_sLang           = src._sLang ;
	_sMetier         = src._sMetier ;
	_sSpecialite     = src._sSpecialite ;
	_sVille          = src._sVille ;
  _sProfessionalId = src._sProfessionalId ;
  _sRpps           = src._sRpps ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::operator==(NSUtilisateurData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSUtilisateurData::operator==(const NSUtilisateurData& o)
{
	return (_sNss == o._sNss) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::donneTitre()
//  Description:	renvoie le titre de l'utilisateur en fonction des donnees
//  Retour:			la string contenant le titre
//---------------------------------------------------------------------------
string
NSUtilisateurData::donneTitre(NSContexte* pContexte, string sLang)
{
	string titre("") ;

	if ((string("") == sLang) || (string("fr") == string(sLang, 0, 2)))
	{
  	if ((string("HDOCT1") == _sType) ||
        (string("HPROF1") == _sType))
			titre = string("le ") ;
	}

	titre += StringTitre(pContexte, _sType, sLang) ;

	if (string("") != _sPrenom)
  	titre += _sPrenom + string(" ") ;

	titre += _sNom ;

  return titre ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::donneAvantTitre()
//  Description:	renvoie l'avant-titre (au ou à) de l'utilisateur en fonction des donnees
//  Retour:			la string contenant l'avant-titre
//---------------------------------------------------------------------------

string
NSUtilisateurData::donneAvantTitre(string sLang)
{
	return StringAvantTitre(_sType, sLang) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtilisateurData::donneSignature()
//  Description:	renvoie le titre de l'utilisateur en fonction des donnees
//  Retour:			la string contenant le titre signature
//---------------------------------------------------------------------------

string
NSUtilisateurData::donneSignature(NSContexte* pContexte, string sLang)
{
	string titre("") ;

	titre = StringTitre(pContexte, _sType, sLang) ;

	if (string("") != _sPrenom)
  	titre += _sPrenom + string(" ") ;

	titre += _sNom ;

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
            :NSCorrespondantInfo(pCtx)
{
	_sTitre                = string("") ;
	_sCivilProf            = string("") ;
	_sCivil                = string("") ;

	_sLogin                = string("") ;
	_sPasswd               = string("") ;
	_sUserType             = string("") ;
	_sPasswordType         = string("") ;
	_sDatePasswordCreation = string("") ;
	_sValidityDuration     = string("") ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtiliInfo::NSUtiliInfo(NSBasicAttributeArray* pAttribute)
//  Description:	Constructeur avec NSBasicAttributeArray
//                  (pour les accès aux bases a l'aide du pilot)
//  Retour:			Rien
//---------------------------------------------------------------------------
NSUtiliInfo::NSUtiliInfo(NSBasicAttributeArray* pAttribute, NSContexte* pCtx)
            :NSCorrespondantInfo(pCtx)
{
  _sTitre                = string("") ;
  _sCivilProf            = string("") ;
  _sCivil                = string("") ;
  _sLogin                = string("") ;
  _sPasswd               = string("") ;
  _sUserType             = string("") ;
  _sPasswordType         = string("") ;
  _sDatePasswordCreation = string("") ;
  _sValidityDuration     = string("") ;

  if ((pAttribute) && (false == pAttribute->empty()))
  {
		NSBasicAttributeIter iter = pAttribute->begin() ;

		for (iter ; pAttribute->end() != iter ; iter++)
		{
    	if      ((*iter)->getBalise() == FIRST_NAME)
    		_Donnees._sPrenom = (*iter)->getText() ;
    	else if ((*iter)->getBalise() == LAST_NAME)
    		_Donnees._sNom    = (*iter)->getText() ;
    	else if((*iter)->getBalise() == SEX)
    		_Donnees._sSexe   = (*iter)->getText() ;
    	else if((*iter)->getBalise() == PIDS)
    		_Donnees._sNss    = (*iter)->getText() ;
    	else if((*iter)->getBalise() == TITLE_CODE)
    	{
    		_Donnees._sType   = (*iter)->getText() ;
/*
    	string sCivilite = (*iter)->getText() ;

      if      (sCivilite == string("HDOCT1"))
      	pDonnees->_sType = string("O") ;
      else if (sCivilite == string("HPROF1"))
      	pDonnees->_sType = string("P") ;
      else if (sCivilite == string("HMOND1"))
      	pDonnees->_sType = string("1") ;
      else if (sCivilite == string("HMADD1"))
      	pDonnees->_sType = string("2") ;
      else if (sCivilite == string("HMONF1"))
      	pDonnees->_sType = string("R") ;
      else if (sCivilite == string("HMADP1"))
      	pDonnees->_sType = string("M") ;
      else if (sCivilite == string("HMONP1"))
      	pDonnees->_sType = string("A") ;
      else if (sCivilite == string("HMADR1"))
      	pDonnees->_sType = string("B") ;
      else if (sCivilite == string("HMADE1"))
      	pDonnees->_sType = string("C") ;
*/
    	}
    	else if ((*iter)->getBalise() == PROFESSION)
    		_Donnees._sMetier     = (*iter)->getText() ;
    	else if ((*iter)->getBalise() == SPECIALITY)
    		_Donnees._sSpecialite = (*iter)->getText() ;
    	else if ((*iter)->getBalise() == CITY_PRO)
    		_Donnees._sVille      = (*iter)->getText() ;
		}
	}
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSUtiliInfo::NSUtiliInfo(const NSUtiliInfo& rv)
            :NSCorrespondantInfo(rv.pContexte)
{
  _Donnees     = rv._Donnees ;
  _GraphPerson = rv._GraphPerson ;

  _sTitre                  = rv._sTitre ;
  _sCivilProf              = rv._sCivilProf ;
  _sCivil                  = rv._sCivil ;
  _sLogin                  = rv._sLogin ;
  _sPasswd                 = rv._sPasswd ;
  _sUserType               = rv._sUserType ;
  _sPasswordType           = rv._sPasswordType ;
  _sDatePasswordCreation   = rv._sDatePasswordCreation ;
  _sValidityDuration       = rv._sValidityDuration ;
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSUtilisateurChoisi*
//---------------------------------------------------------------------------
NSUtiliInfo::NSUtiliInfo(NSUtilisateurChoisi* pUtilChoisi)
						:NSCorrespondantInfo(pUtilChoisi->NSUtiliInfo::pContexte)
{
  _Donnees     = *(pUtilChoisi->getData()) ;
  _GraphPerson = pUtilChoisi->_GraphPerson ;

  _sTitre                = pUtilChoisi->_sTitre ;
  _sCivilProf            = pUtilChoisi->_sCivilProf ;
  _sCivil                = pUtilChoisi->_sCivil ;
  _sLogin                = pUtilChoisi->_sLogin ;
  _sPasswd               = pUtilChoisi->_sPasswd ;
  _sUserType             = pUtilChoisi->_sUserType ;
  _sPasswordType         = pUtilChoisi->_sPasswordType ;
  _sDatePasswordCreation = pUtilChoisi->_sDatePasswordCreation ;
  _sValidityDuration     = pUtilChoisi->_sValidityDuration ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSUtiliInfo::~NSUtiliInfo()
{
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
NSUtiliInfo::operator=(const NSUtiliInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees 	   = src._Donnees ;
	_GraphPerson = src._GraphPerson ;

  _sTitre                = src._sTitre ;
  _sCivilProf            = src._sCivilProf ;
  _sCivil                = src._sCivil ;
  _sLogin                = src._sLogin ;
  _sPasswd               = src._sPasswd ;
  _sUserType             = src._sUserType ;
  _sPasswordType         = src._sPasswordType ;
  _sDatePasswordCreation = src._sDatePasswordCreation ;
  _sValidityDuration     = src._sValidityDuration ;

  pContexte = src.pContexte ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSUtiliInfo::operator==(NSUtiliInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSUtiliInfo::operator==(const NSUtiliInfo& o)
{
	int egal = 1 ;

	if (!(_Donnees == o._Donnees))
  	egal = 0 ;

	return egal ;
}

bool
NSUtiliInfo::pwdNeedChange()
{
	if      ("T" == _sPasswordType)
		return true ;
	else if ("F" == _sPasswordType)
		return false ;
	else if ("J" == _sPasswordType)
	{
    // Calcul de la date du jour et de la date d'expiration
    //
    string sDateJour       = donne_date_duJour() ;
    string sDateExpiration = _sDatePasswordCreation ;

    for (int i = 0; i < atoi(_sValidityDuration.c_str()); i++)
    	incremente_date(sDateExpiration) ;

    if (sDateExpiration < sDateJour)
    	return true ;

    return false ;
	}

	return false ;
}

bool
NSUtiliInfo::changePassword()
{
	::MessageBox(0, "Votre mot de passe a expiré. Vous devez saisir un nouveau mot de passe.", "Message Nautilus", MB_OK) ;

    /********************************************
    // modification d'un code utilisateur
    // on ne contrôle pas l'ancien mot de passe pour l'administrateur
    // Saisie du nouveau mot de passe
    string sDateJour= donne_date_duJour() ;

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

string
NSUtiliInfo::donneSignature(NSContexte* pContexte, string sLang)
{
  string sTitre = StringTitre(pContexte, _sTitre, sLang) ;

	if (string("") != getPrenom())
  	sTitre += getPrenom() + string(" ") ;

	sTitre += getNom() ;

  return sTitre ;
}

//***************************************************************************
//
// Implémentation des méthodes NSCorrespondantData
//
//***************************************************************************

/*
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
	_sNss        = string("") ;
  _sCode       = string("") ;
  _sNom        = string("") ;
  _sPrenom     = string("") ;
  _sSexe       = string("") ;
  _sAdress     = string("") ;
  _sDocteur    = string("") ;
  _sTelePor    = string("") ;
  _sTeleBur    = string("") ;
  _sNbExemp    = string("") ;
  _sMessagerie = string("") ;
  _sFonction   = string("") ;
  _sActif      = string("") ;
 	_sLang       = string("") ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantData::NSCorrespondantData(NSCorrespondantData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCorrespondantData::NSCorrespondantData(NSCorrespondantData& rv)
{
	_sNss        = rv._sNss ;
  _sCode       = rv._sCode ;
  _sNom        = rv._sNom ;
  _sPrenom     = rv._sPrenom ;
  _sSexe       = rv._sSexe ;
  _sAdress     = rv._sAdress ;
  _sDocteur    = rv._sDocteur ;
  _sTelePor    = rv._sTelePor ;
  _sTeleBur    = rv._sTeleBur ;
  _sNbExemp    = rv._sNbExemp ;
  _sMessagerie = rv._sMessagerie ;
  _sFonction   = rv._sFonction ;
  _sActif      = rv._sActif ;
 	_sLang       = rv._sLang ;
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

	_sNss        = src._sNss ;
  _sCode       = src._sCode ;
  _sNom        = src._sNom ;
  _sPrenom     = src._sPrenom ;
  _sSexe       = src._sSexe ;
  _sAdress     = src._sAdress ;
  _sDocteur    = src._sDocteur ;
  _sTelePor    = src._sTelePor ;
  _sTeleBur    = src._sTeleBur ;
  _sNbExemp    = src._sNbExemp ;
  _sMessagerie = src._sMessagerie ;
  _sFonction   = src._sFonction ;
  _sActif      = src._sActif ;
 	_sLang       = src._sLang ;

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
	string titre("") ;

	titre = StringTitre(docteur, sLang) ;

	if (string("") != _sPrenom)
		titre = titre + _sPrenom + string(" ") ;

	titre += _sNom ;

	return titre ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantData::donneAvantTitre()
//  Description:	renvoie l'avant-titre (au ou à) du corresp en fonction des donnees
//  Retour:			la string contenant l'avant-titre
//---------------------------------------------------------------------------
string
NSCorrespondantData::donneAvantTitre(string sLang)
{
  return StringAvantTitre(docteur, sLang) ;
}
*/

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
                    :NSRoot(pCtx),
                     _GraphPerson(pCtx->getSuperviseur())
{
  lObjectCount++ ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::~NSCorrespondantInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCorrespondantInfo::~NSCorrespondantInfo()
{
  lObjectCount-- ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::NSCorrespondantInfo(NSCorrespondantInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCorrespondantInfo::NSCorrespondantInfo(const NSCorrespondantInfo& rv)
                    :NSRoot(rv.pContexte),
                     _Donnees(rv._Donnees),
                     _GraphPerson(rv._GraphPerson)
{
  lObjectCount++ ;
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
NSCorrespondantInfo::operator=(const NSCorrespondantInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees     = src._Donnees ;
	_GraphPerson = src._GraphPerson ;
	pContexte    = src.pContexte ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCorrespondantInfo::operator==(NSCorrespondantInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int
NSCorrespondantInfo::operator==(const NSCorrespondantInfo& o)
{
	int egal = 1 ;

	if (!(_Donnees == o._Donnees))
		egal = 0 ;

	return egal ;
}

bool
NSCorrespondantInfo::GetDataFromGraph(NSPatPathoArray* pPPT)
{
	string       sElemLex, sSens, sType, sTemp ;
	string       lang = "" ;

	NSPatPathoArray PatPathoArray(pContexte->getSuperviseur()) ;

	if (pPPT)
	{
		if (pPPT->empty())
			return false ;

		PatPathoArray = *pPPT ;
	}
	else
	{
		// on initialise la patpatho à partir du graphe
    //
    // Warning getRawPatPatho doesn't get Archetypes information
    //
		NSDataTreeIter iterTree ;
		NSDataGraph* pDataGraph = _GraphPerson.getDataGraph() ;

    if (pDataGraph->getTrees()->ExisteTree("ZADMI1", pContexte->getSuperviseur(), &iterTree))
    	(*iterTree)->getRawPatPatho(&PatPathoArray) ;
    else
    {
      string sMsg = string("Impossible de trouver l'arbre [ZADMI1] dans le graphe administratif.") ;
      erreur(sMsg.c_str(), standardError, 0) ;
      return false ;
    }

    if (pDataGraph->getTrees()->ExisteTree("DPROS1", pContexte->getSuperviseur(), &iterTree))
    {
    	NSPatPathoArray PatPathoPDS(pContexte->getSuperviseur()) ;
      (*iterTree)->getRawPatPatho(&PatPathoPDS) ;
      PatPathoArray.InserePatPatho(PatPathoArray.end(), &PatPathoPDS, 0, true) ;
    }
  }

	if (PatPathoArray.empty())
		return false ;

	//=============================================================================
	//il faut recuperer la langue d'utilisateur de la base ou de l'interface
	//Voir dans la racine du DPROS!!!!
	//=============================================================================
  string sLang = pContexte->getUserLanguage() ;

	string sNom    = "" ,  sPrenom = "" , sSexe   = "" ;
	string sCivilite = "", sEMail = "";
	string sMetier = "",   sSpecialite = "", sVille = string("") ;

	PatPathoIter iter = PatPathoArray.begin() ;
	int iColBase = (*iter)->getColonne() ;

	while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() >= iColBase))
	{
  	sSens = (*iter)->getLexiqueSens() ;

    if (string("ZADMI") == sSens)
    {
    	iter++ ;

      while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
      {
      	sSens = (*iter)->getLexiqueSens() ;

        // Chapitre "identité" / Identity chapter
        if (string("LIDET") == sSens)
        {
        	int iColIdentity = (*iter)->getColonne() ;
          iter++ ;

          while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColIdentity))
          {
          	sSens = (*iter)->getLexiqueSens() ;

            // Nom de l'utilisateur
            if ((string("LNOM0") == sSens) && (string("") == sNom))
            {
              sNom = PatPathoArray.getFreeTextValue(&iter) ;

              if (string("") != sNom)
              	_Donnees._sNom = sNom ;
            }
            // Prénom de l'utilisateur
            else if ((string("LNOM2") == sSens) && (sPrenom == ""))
            {
              sPrenom = PatPathoArray.getFreeTextValue(&iter) ;

              if (string("") != sPrenom)
              	_Donnees._sPrenom = sPrenom ;
            }
            // Sexe de l'utilisateur
            else if (string("LSEXE") == sSens)
            {
            	iter++ ;
              while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColIdentity+1))
              {
              	// on cherche ici un code lexique
                sTemp = (*iter)->getLexiqueSens() ;

                if      (string("HMASC") == sTemp)
                	_Donnees.metMasculin() ;
                else if (string("HFEMI") == sTemp)
                	_Donnees.metFeminin() ;

                iter++ ;
              }
            }
            else
            	iter++ ;
          }
        }
        else
        	iter++ ;
      } // while
    } // if (sSens == string("ZADMI"))
    else if (string("DPROS") == sSens)
    {
    	iter++ ;

      while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
      {
      	sSens = (*iter)->getLexiqueSens() ;

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
        lang = "fr" ;

        // Chapitre "Compétences" / Competence chapter
        if (string("LCOMP") == sSens)
        {
        	int iColCompetences = (*iter)->getColonne() ;
          iter++ ;

          while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColCompetences))
          {
          	sSens = (*iter)->getLexiqueSens() ;

            // metier
            if ((string("LMETI") == sSens) && (string("") == sMetier))
            {
            	// on cherche ici un edit lexique (ayant comme fils un code lexique)
              iter++ ;
              sElemLex = (*iter)->getLexique() ;
              if (string("") != sElemLex)
              	if (string(sElemLex, 0, 3) == string("£C;"))
                {
                	iter++ ;
                  while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColCompetences+2))
                  {
                  	// on cherche ici un code lexique pour un libelle
                    string sCode = (*iter)->getLexique() ;
                    pContexte->getDico()->donneLibelle(sLang, &sCode, &sMetier) ;

                    iter++ ;
                  }
                }
                else
                {
                	sMetier = (*iter)->getLexique() ;
                  if (PatPathoArray.end() != iter)
                  	iter++ ;
                }
              else
              	iter++ ;

              if (string("") != sMetier)
              	_Donnees._sMetier = sMetier ;
            }
            // Specilité de l'utilisateur
            else if ((string("LSPEC") == sSens) && (string("") == sSpecialite))
            {
            	// on cherche ici un edit lexique (ayant comme fils un code lexique)
              iter++ ;
              sElemLex = (*iter)->getLexique() ;
              if (string("") != sElemLex)
              	if (string(sElemLex, 0, 3) == string("£C;"))
                {
                	iter++ ;
                  while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColCompetences+2))
                  {
                  	// on cherche ici un code lexique pour un libelle
                    string sCode = (*iter)->getLexique() ;
                    pContexte->getDico()->donneLibelle(sLang, &sCode, &sSpecialite) ;

                    iter++ ;
                  }
                }
              	else
              	{
              		sSpecialite = (*iter)->getLexique() ;
                	if (PatPathoArray.end() != iter)
                		iter++ ;
              	}
            	else    //si "" on avanse
              	iter++ ;

              if (string("") != sSpecialite)
              	_Donnees._sSpecialite = sSpecialite ;
            }
            else
            	iter++ ;
          }
 				}
        // Chapitre "civilité"
        else if (string("HCIVO") == sSens)
        {
        	int iColCivilite = (*iter)->getColonne() ;
          iter++ ;
          while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColCivilite))
          {
          	// on cherche ici un code lexique pour un libelle
            sCivilite = (*iter)->getLexique() ;
            // pContexte->getDico()->donneLibelle(sLang, &sCodeLex, &sCivilite) ;

            iter++ ;
          }

          if (sCivilite != "")
          {
          	_Donnees._sType = sCivilite ;
/*
          	if (sCivilite == string("HDOCT1"))
            	pDonnees->_sType = string("O") ;
            else if (sCivilite == string("HPROF1"))
            	pDonnees->_sType = string("P") ;
            else if (sCivilite == string("HMOND1"))
            	pDonnees->_sType = string("1") ;
            else if (sCivilite == string("HMADD1"))
            	pDonnees->_sType = string("2") ;
            else if (sCivilite == string("HMONF1"))
            	pDonnees->_sType = string("R") ;
            else if (sCivilite == string("HMADP1"))
            	pDonnees->_sType = string("M") ;
            else if (sCivilite == string("HMONP1"))
            	pDonnees->_sType = string("A") ;
            else if (sCivilite == string("HMADR1"))
            	pDonnees->_sType = string("B") ;
            else if (sCivilite == string("HMADE1"))
            	pDonnees->_sType = string("C") ;
*/
          }
        }

        // Identifiers
        else if (string("LIPRO") == sSens)
				{
          int iColPros = (*iter)->getColonne() ;
					iter++;
					while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColPros))
					{
						sSens = (*iter)->getLexiqueSens() ;

						// Adeli
						if (sSens == string("LADEL"))
						{
            	int iColId = (*iter)->getColonne() ;
							iter++ ;
              while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColId))
					    {
						    _Donnees._sProfessionalId = (*iter)->getComplement() ;
						    iter++ ;
					    }
            }
            // Rpps
						else if (sSens == string("LRPPS"))
						{
            	int iColId = (*iter)->getColonne() ;
							iter++ ;
              while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColId))
					    {
						    _Donnees._sRpps = (*iter)->getComplement() ;
						    iter++ ;
					    }
            }
            else
              iter++ ;
					}
				}

        // Chapitre "lieu d'exercice"
        else if (sSens == string("ULIEX"))
        {
        	int iColLiex = (*iter)->getColonne() ;
          iter++ ;

          while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColLiex))
          {
          	sSens = (*iter)->getLexiqueSens() ;

            // Adresse mail
            if ((string("LMAIL") == sSens) && (string("") == sEMail))
            {
              sEMail = PatPathoArray.getFreeTextValue(&iter) ;

              if (sEMail != "")
              	_Donnees._sMessagerie = sEMail ;
            }
            // Adresse
						else if (sSens == string("LADRE"))
						{
            	int iColAdr = (*iter)->getColonne() ;
							iter++ ;
              while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColAdr))
							{
								sSens = (*iter)->getLexiqueSens() ;

								// Adresse
								if (sSens == string("LVILL"))
								{
                	int iColVille = (*iter)->getColonne() ;
									iter++ ;
              		while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColVille))
									{
										sSens = (*iter)->getLexiqueSens() ;

										// Adresse
										if (sSens == string("LCOMU"))
                    {
                      sVille = PatPathoArray.getFreeTextValue(&iter) ;

                      if (string("") != sVille)
                      	_Donnees._sVille = sVille ;
                    }
                    else
            					iter++ ;
                  }
                }
                else
            			iter++ ;
              }
            }
            else
            	iter++ ;
          } // while
        }
        else
        	iter++ ;
 			} // while
		} // else if (sSens == string("DPROS"))
		else
			iter++ ;
	} // while de base

	return true ;
}

//////////////////////// fin de nsperson.cpp //////////////////////////////////

