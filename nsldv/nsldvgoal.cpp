// #include "nautilus\nsanxary.h"
#include "nsdn\nsdochis.h"
#include "nsldv\nsldvgoal.h"

#include "nsbb\nsbbtran.h"
#include "nsbb\nstlibre.h"
#include "nautilus\nssuper.h"
#include "nautilus\nshistdo.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsconver.h"

long NSLdvGoalInfo::lObjectCount = 0 ;
long NSDelayZone::lObjectCount = 0 ;
long NSDateZone::lObjectCount = 0 ;
long NSValueZone::lObjectCount = 0 ;
long NSLdvGoal::lObjectCount = 0 ;

// -----------------------------------------------------------------------------
// fonction globale de tri :
//			Permet de trier les NSLdvGoalInfo par ordre de date
// -----------------------------------------------------------------------------
bool
infGoalInfo(NSLdvGoalInfo *s, NSLdvGoalInfo *b)
{
  return (s->tpsInfo < b->tpsInfo) ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSLdvGoalInfo
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// NSLdvGoalInfo(string code, string value, string unit)
// -----------------------------------------------------------------------------
NSLdvGoalInfo::NSLdvGoalInfo()
{
  sGoalReference  = string("") ;

  iTypeJalon      = jalonNotype ;
  iTypeJalonEvent = jalonNoEvent ;

  iTimeLevel      = None ;
  iValueLevel     = None ;
  iLevel          = None ;

  sEventNode      = string("") ;
  tpsInfo.init() ;
  tpsClosed.setNoLimit() ;

	sCode           = string("") ;
  dValue          = 0 ;
	sValue	        = string("") ;
	sUnit           = string("") ;
	sFormat	        = string("") ;

  lObjectCount++ ;
}

// -----------------------------------------------------------------------------
// NSLdvGoalInfo(NSLdvGoalInfo& rv)
// -----------------------------------------------------------------------------
NSLdvGoalInfo::NSLdvGoalInfo(const NSLdvGoalInfo& rv)
{
  sGoalReference  = rv.sGoalReference ;

	sCode           = rv.sCode ;
  dValue          = rv.dValue ;
	sValue	        = rv.sValue ;
	sUnit           = rv.sUnit ;
	sFormat         = rv.sFormat ;

  tpsInfo         = rv.tpsInfo ;
  tpsClosed       = rv.tpsClosed ;

  iLevel          = rv.iLevel ;
  iTimeLevel      = rv.iTimeLevel ;
  iValueLevel     = rv.iValueLevel ;

  sEventNode      = rv.sEventNode ;
  iTypeJalon      = rv.iTypeJalon ;
  iTypeJalonEvent = rv.iTypeJalonEvent ;

  lObjectCount++ ;
}

// -----------------------------------------------------------------------------
// ~NSLdvGoalInfo()
// -----------------------------------------------------------------------------
NSLdvGoalInfo::~NSLdvGoalInfo()
{
  lObjectCount-- ;
}

// -----------------------------------------------------------------------------
// NSLdvGoalInfo& operator=(NSLdvGoalInfo src)
// -----------------------------------------------------------------------------
NSLdvGoalInfo&
NSLdvGoalInfo::operator=(const NSLdvGoalInfo& src)
{
  if (this == &src)
		return *this ;

  sGoalReference  = src.sGoalReference ;

	sCode           = src.sCode ;
  dValue          = src.dValue ;
	sValue	        = src.sValue ;
	sUnit 	        = src.sUnit ;
	sFormat         = src.sFormat ;

  tpsInfo         = src.tpsInfo ;
  tpsClosed       = src.tpsClosed ;

  iLevel          = src.iLevel ;
  iTimeLevel      = src.iTimeLevel ;
  iValueLevel     = src.iValueLevel ;

  sEventNode      = src.sEventNode ;
  iTypeJalon      = src.iTypeJalon ;
  iTypeJalonEvent = src.iTypeJalonEvent ;

	return (*this) ;
}

void
NSLdvGoalInfo::computeLevel()
{
  if (iValueLevel > iTimeLevel)
    iLevel = iValueLevel ;
  else
    iLevel = iTimeLevel ;
}

NSLdvGoalInfo::JALONSLEVELS
NSLdvGoalInfo::getPreviousColor(NSLdvGoalInfo::JALONSLEVELS iColorLevel)
{
	switch(iColorLevel)
	{
		case AProuge : return APjaune ;
		case APjaune : return APvert ;
		case APvert  : return Bleu ;
		case Bleu    : return AVvert ;
		case AVvert  : return AVjaune ;
		case AVjaune : return AVrouge ;
		case AVrouge : return None ;
		case None    : return None ;
		default      : return None ;
	}
}

NSLdvGoalInfo::JALONSLEVELS
NSLdvGoalInfo::getNextColor(NSLdvGoalInfo::JALONSLEVELS iColorLevel)
{
	switch(iColorLevel)
	{
		case AProuge : return None ;
		case APjaune : return AProuge ;
		case APvert  : return APjaune ;
		case Bleu    : return APvert ;
		case AVvert  : return Bleu ;
		case AVjaune : return AVvert ;
		case AVrouge : return AVjaune ;
		case None    : return None ;
		default      : return None ;
	}
}

// -----------------------------------------------------------------------------
//
// Méthodes de GoalInfoArray
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// GoalInfoArray()
// -----------------------------------------------------------------------------
GoalInfoArray::GoalInfoArray()
              :GoalInfoVector()
{
}

// -----------------------------------------------------------------------------
// GoalInfoArray(GoalInfoArray& rv)
// -----------------------------------------------------------------------------
GoalInfoArray::GoalInfoArray(const GoalInfoArray& rv)
{
try
{
	if (false == rv.empty())
		for (GoalInfoConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSLdvGoalInfo(*(*i))) ;
}
catch (...)
{
	erreur("Exception GoalInfoArray copy ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// ~GoalInfoArray()
// -----------------------------------------------------------------------------
GoalInfoArray::~GoalInfoArray()
{
	vider() ;
}

// -----------------------------------------------------------------------------
// GoalInfoArray& operator=(GoalInfoArray src)
// -----------------------------------------------------------------------------
GoalInfoArray&
GoalInfoArray::operator=(const GoalInfoArray& src)
{
try
{
	if (this == &src)
		return (*this) ;

	vider() ;

	if (false == src.empty())
		for (GoalInfoConstIter goalInfoIter = src.begin() ; src.end() != goalInfoIter ; goalInfoIter++)
			push_back(new NSLdvGoalInfo(**goalInfoIter)) ;

	return (*this) ;
}
catch (...)
{
	erreur("Exception GoalInfoArray (=).", standardError, 0) ;
	return *this;
}
}

// -----------------------------------------------------------------------------
// void	vider()
// -----------------------------------------------------------------------------
void
GoalInfoArray::vider()
{
	if (empty())
		return ;

	for (GoalInfoIter i = begin() ; end() != i ; )
	{
    if (*i)
		  delete (*i) ;
		erase(i) ;
	}
}

NSLdvGoalInfo*
GoalInfoArray::getClosingJalon(NSLdvGoalInfo *pJalon)
{
	if (empty())
		return (NSLdvGoalInfo*) 0 ;

  GoalInfoIter i = begin() ;
  // On retrouve le jalon initial
  for ( ; (end() != i) && (*i != pJalon) ; i++)
    ;
  if (end() == i)
    return (NSLdvGoalInfo*) 0 ;

  GoalInfoIter j = getClosingJalon(i) ;

  if (j == end())
    return (NSLdvGoalInfo*) 0 ;

  return *j ;
}

GoalInfoIter
GoalInfoArray::getClosingJalon(GoalInfoIter iJalon)
{
  if (empty())
    return end() ;

  string sGoalRef = (*iJalon)->sGoalReference ;

  GoalInfoIter j = iJalon ;
  j++ ;

  for ( ; (j != end()) && ((*j)->sGoalReference != sGoalRef) ; j++)
    ;

  return j ;
}

/*
// -----------------------------------------------------------------------------
// void	initialiser()
// -----------------------------------------------------------------------------
void
GoalInfoArray::initialiser()
{
}


// -----------------------------------------------------------------------------
// void		loadGoalInfo(PatPathoIter iter, int iColBase)
// -----------------------------------------------------------------------------
void
GoalInfoArray::loadGoalInfo(PatPathoIter iter, int iColBase)
{
}


// -----------------------------------------------------------------------------
// void		reinit()
// -----------------------------------------------------------------------------
void
GoalInfoArray::reinit()
{
}


// -----------------------------------------------------------------------------
// void        reloadGoalInfo(PatPathoIter iter, int iColBase)
// -----------------------------------------------------------------------------
void
GoalInfoArray::reloadGoalInfo(PatPathoIter iter, int iColBase)
{
}
*/


// -----------------------------------------------------------------------------
//
// Méthodes de NSLdvGoal
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// NSLdvGoal(NSContexte *pCtx)
// -----------------------------------------------------------------------------
NSLdvGoal::NSLdvGoal(NSContexte *pCtx, NSLdvDocumentBase *pDocum, LDVFRAME iFrame)
          :NSRoot(pCtx)
{
try
{
  _pDoc                   = pDocum ;

	_sLexique                = string("") ;
	_sTitre                  = string("") ;
	_sCertitude              = string("") ;
	_sComplementText         = string("") ;
	_tDateOuverture.init() ;
	_dValueOuverture         = 0 ;
	_sValueOuverture         = string("") ;
	_sUnitFermeture          = string("") ;
	_bValueOuverture         = false ;
	_tDateFermeture.init() ;
	_dValueFermeture         = 0 ;
	_sValueFermeture         = string("") ;
	_sUnitFermeture          = string("") ;
	_bValueFermeture         = false ;
	_tOuvertLe.init() ;
	_tFermeLe.init() ;

	_sOpenEventNode          = string("") ;
	_sCloseEventNode         = string("") ;

	_sConcern                = string("") ;
	_sReference              = string("") ;

	_iRythme                 = NSLdvGoal::ponctuel ;

  _iFrame                 = iFrame ;

	_dDelaiDebutAutorise     = -1 ;
	_sUniteDebutAutorise     = string("") ;
	_dDelaiDebutConseille    = -1 ;
	_sUniteDebutConseille    = string("") ;
	_dDelaiDebutIdeal        = -1 ;
	_sUniteDebutIdeal        = string("") ;
	_dDelaiDebutIdealMax     = -1 ;
	_sUniteDebutIdealMax     = string("") ;
	_dDelaiDebutConseilMax   = -1 ;
	_sUniteDebutConseilMax   = string("") ;
	_dDelaiDebutCritique     = -1 ;
	_sUniteDebutCritique     = string("") ;

	_sDateDebutAutorise      = string("") ;
	_sDateDebutConseille     = string("") ;
	_sDateDebutIdeal         = string("") ;
	_sDateDebutIdealMax      = string("") ;
	_sDateDebutConseilMax    = string("") ;
	_sDateDebutCritique      = string("") ;

	_bValue                  = false ;

	_bValMinAutorise         = false ;
	_dValMinAutorise         = 0 ;
	_sUniteValMinAutorise    = string("") ;
	_bValMinConseille        = false ;
	_dValMinConseille        = 0 ;
	_sUniteValMinConseille   = string("") ;
	_bValMinIdeal            = false ;
	_dValMinIdeal            = 0 ;
	_sUniteValMinIdeal       = string("") ;
	_bValMaxIdeal            = false ;
	_dValMaxIdeal            = 0 ;
	_sUniteValMaxIdeal       = string("") ;
	_bValMaxConseille        = false ;
	_dValMaxConseille        = 0 ;
	_sUniteValMaxConseille   = string("") ;
	_bValMaxAutorise         = false ;
	_dValMaxAutorise         = 0 ;
	_sUniteValMaxAutorise    = string("") ;
  _isSelected              = false ;

  lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSLdvGoal ctor.", standardError, 0) ;
}
}

int
NSLdvGoal::getGoalType()
{
	char firstCh = _sLexique[0] ;
  switch(firstCh)
  {
  	case 'I':
			return medicament ;
    case '_' :
    	return medicament ;
    case 'G' :
    	return exam ;
    case 'N' :
    	return traitement ;
    case 'T' :
    	return biology ;
    case 'V' :
    	//test if sLexique EST un examen biologique
    	string sCodeSens ;
    	pContexte->getDico()->donneCodeSens(&_sLexique, &sCodeSens) ;
      if (pContexte->getSuperviseur()->getFilGuide()->VraiOuFaux(sCodeSens, "ES", "VREBI1"))
        return biology ;
  } ;

  return other ;
}

void
NSLdvGoal::init(string sOpenDate, string sCloseDate, string sOpenEvent, string sCloseEvent)
{
  if (string("") != sOpenDate)
    _tDateOuverture.initFromDate(sOpenDate) ;
  if (string("") != sCloseDate)
    _tDateFermeture.initFromDate(sCloseDate) ;

  _sOpenEventNode  = sOpenEvent ;
  _sCloseEventNode = sCloseEvent ;

  init() ;
}

void
NSLdvGoal::init()
{
try
{
  // L'événement est-il ouvert ?
  // Does event opened ?

  // S'il n'y a pas d'événement d'ouverture, l'ouverture est mécanique
  // If there is no opening event, the goal opens automatically
  if (_sOpenEventNode == "")
  {
    // Ce n'est pas gênant que l'objectif s'ouvre dans le futur
    // The goal can open in the futur - no problem for that
    if (false == _tDateOuverture.estVide())
      _tOuvertLe = _tDateOuverture ;
  }

  // S'il y a un événement d'ouverture, l'ouverture ne se fait que si l'événement a eu lieu
  // If there is an opening event, the goal only opens if the event occured
  else
  {
    NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

    VecteurString vecteurString ;
    pGraphe->TousLesVrais(_sReference, NSRootLink::goalOpener, &vecteurString) ;

    if (false == vecteurString.empty())
    {
      EquiItemIter openIter = vecteurString.begin() ;
      string       sDateDoc = getNodeDate(*(*openIter)) ;

      double dValue ;
      string sValue ;
      if (getNodeValue(*(*openIter), &sValue, &dValue))
      {
        _sUnitOuverture  = getNodeUnit(*(*openIter)) ;
        _dValueOuverture = dValue ;
        _sValueOuverture = sValue ;
        _bValueOuverture = true ;
        _bValue          = true ;
      }

      if (string("") != sDateDoc)
        _tOuvertLe.initFromDate(sDateDoc) ;
    }
  }

  // L'événement a-t-il une date de fermeture ?
  // Does event have a closing date ?

  if (_tOuvertLe.estVide())
    return ;

  NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

  VecteurString vectString ;
  pGraphe->TousLesVrais(_sReference, NSRootLink::goalCloser, &vectString, "ENVERS") ;

  if (false == vectString.empty())
  {
    EquiItemIter closeIter = vectString.begin() ;
    string       sDateDoc  = getNodeDate(*(*closeIter)) ;

    double       dValue ;
    string       sValue ;
    if (getNodeValue(*(*closeIter), &sValue, &dValue))
    {
      _sUnitFermeture  = getNodeUnit(*(*closeIter)) ;
      _dValueFermeture = dValue ;
      _sUnitFermeture  = sValue ;
      _bValueFermeture = true ;
      _bValue          = true ;
    }

    if (sDateDoc != "")
      _tFermeLe.initFromDate(sDateDoc) ;
  }

  // S'il n'y a pas d'événement de fermeture, la fermeture est mécanique
  // If there is no closing event, the goal closes automatically
  if ((_tFermeLe.estVide()) && (string("") == _sCloseEventNode))
  {
    // Ce n'est pas gênant que l'objectif s'ouvre dans le futur
    // The goal can open in the futur - no problem for that
    if (false == _tDateFermeture.estVide())
      _tFermeLe = _tDateFermeture ;
  }

  // Check that this goal doesn't exist after it's concern closing date
  //
  if (string("") != _sConcern)
  {
    if (_pDoc)
    {
      LDVFRAME iFrame = ldvframeNotSpecified ;
      NSConcern* pConcern = _pDoc->getConcern(_sConcern, iFrame) ;
      if (pConcern && (false == pConcern->_tDateFermeture.estNoLimit()))
        if ((_tFermeLe.estVide()) || (_tFermeLe > pConcern->_tDateFermeture))
          _tFermeLe = pConcern->_tDateFermeture ;
    }
  }
}
catch (...)
{
	erreur("Exception NSLdvGoal::init().", standardError, 0) ;
}
}
//==============================================================================

void
NSLdvGoal::getADateTree(string sVal, string sCode, NSPatPathoArray* pPatho, int colonne)
{
	if ((string("") == sVal) || (NULL == pPatho))
		return ;

	pPatho->ajoutePatho(sCode, colonne) ;
	colonne++ ;
	pPatho->ajoutePatho("KDAT01", colonne) ;
	colonne++ ;
	Message Msg ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(sVal) ;
	pPatho->ajoutePatho("£T0;19", &Msg, colonne) ;
}

void
NSLdvGoal::getAllDatesTree(NSPatPathoArray* pPatho, int colonne)
{
  if ((_sDateDebutIdeal     == "") && (_sDateDebutIdealMax   == "") &&
      (_sDateDebutConseille == "") && (_sDateDebutConseilMax == "") &&
      (_sDateDebutAutorise  == "") && (_sDateDebutCritique   == "") )
    return ;

  pPatho->ajoutePatho("KMOD01", colonne) ;
  colonne++;
  getADateTree(_sDateDebutIdeal,      "KMOD31", pPatho, colonne) ;
  getADateTree(_sDateDebutIdealMax,   "KMOD41", pPatho, colonne) ;
  getADateTree(_sDateDebutConseille,  "KMOD21", pPatho, colonne) ;
  getADateTree(_sDateDebutConseilMax, "KMOD51", pPatho, colonne) ;
  getADateTree(_sDateDebutAutorise,   "KMOD11", pPatho, colonne) ;
  getADateTree(_sDateDebutCritique,   "KMOD61", pPatho, colonne) ;
}

void
NSLdvGoal::getAPeriodTree(double dVal, string unit, string sCode, NSPatPathoArray* pPatho, int colonne)
{
	if ((dVal <= 0) || (unit == ""))
		return ;

	string sVal =	DoubleToString(&dVal, -1, -1) ;

	pPatho->ajoutePatho(sCode, colonne) ;

	colonne++ ;
	pPatho->ajoutePatho("VDURE1", colonne) ;

	colonne++ ;
	Message Msg ;
	Msg.SetUnit(unit) ;
	Msg.SetComplement(sVal) ;
	pPatho->ajoutePatho("£N0;03", &Msg, colonne) ;
}

void
NSLdvGoal::getAllPeriodesTree(NSPatPathoArray* pPatho, int colonne)
{
	if ((_dDelaiDebutIdeal     < 0) && (_dDelaiDebutIdealMax   < 0) &&
		  (_dDelaiDebutConseille < 0) && (_dDelaiDebutConseilMax < 0) &&
		  (_dDelaiDebutAutorise  < 0) && (_dDelaiDebutCritique   < 0))
		return ;

	pPatho->ajoutePatho("KMOD01", colonne) ;
	colonne++;

	getAPeriodTree(_dDelaiDebutIdeal,      _sUniteDebutIdeal,      "KMOD31", pPatho, colonne) ;
	getAPeriodTree(_dDelaiDebutIdealMax,   _sUniteDebutIdealMax,   "KMOD41", pPatho, colonne) ;
	getAPeriodTree(_dDelaiDebutConseille,  _sUniteDebutConseille,  "KMOD21", pPatho, colonne) ;
	getAPeriodTree(_dDelaiDebutConseilMax, _sUniteDebutConseilMax, "KMOD51", pPatho, colonne) ;
	getAPeriodTree(_dDelaiDebutAutorise,   _sUniteDebutAutorise,   "KMOD11", pPatho, colonne) ;
	getAPeriodTree(_dDelaiDebutCritique,   _sUniteDebutCritique,   "KMOD61", pPatho, colonne) ;
}

void
NSLdvGoal::getAValueTree(double dVal, string unit, string sCode, NSPatPathoArray* pPatho, int colonne)
{
	if ((dVal <= 0) || (string("") == unit))
		return ;

	string sVal =	DoubleToString(&dVal, -1, -1) ;

	pPatho->ajoutePatho(sCode, colonne) ;

	colonne++ ;

	Message CodeMsg  ;
	CodeMsg.SetUnit(unit) ;
	CodeMsg.SetComplement(sVal) ;
	pPatho->ajoutePatho("£N0;03", &CodeMsg, colonne) ;
}

void
NSLdvGoal::getAllValuesTree(NSPatPathoArray* pPatho, int colonne)
{
  if ((NSPatPathoArray*) NULL == pPatho)
    return ;

	if ((_dValMinIdeal     <= 0) && (_dValMaxIdeal     <= 0) &&
      (_dValMinConseille <= 0) && (_dValMaxConseille <= 0) &&
      (_dValMinAutorise  <= 0) && (_dValMaxAutorise  <= 0) )
		return ;

	pPatho->ajoutePatho("0ENVV1", colonne) ;
  colonne++ ;
  getAValueTree(_dValMinIdeal,     _sUniteValMinIdeal,     "VMIN01", pPatho, colonne) ;
  getAValueTree(_dValMaxIdeal,     _sUniteValMaxIdeal,     "VMAX01", pPatho, colonne) ;
  getAValueTree(_dValMinConseille, _sUniteValMinConseille, "VMIN11", pPatho, colonne) ;
  getAValueTree(_dValMaxConseille, _sUniteValMaxConseille, "VMAX11", pPatho, colonne) ;
  getAValueTree(_dValMinAutorise,  _sUniteValMinAutorise,  "VMIN21", pPatho, colonne) ;
  getAValueTree(_dValMaxAutorise,  _sUniteValMaxAutorise,  "VMAX21", pPatho, colonne) ;
}

bool
NSLdvGoal::insertDate(string sCode, NVLdVTemps tTemps, NSPatPathoArray* pPatho, int colonne)
{
	if ((tTemps.estVide()) || (string("") == sCode) || (NULL == pPatho))
		return false ;

	pPatho->ajoutePatho(sCode, colonne) ;

  colonne++ ;

	Message Msg ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(tTemps.donneDateHeure()) ;
	pPatho->ajoutePatho("£T0;19", &Msg, colonne) ;

	return true ;
}

bool
NSLdvGoal::insertEvent(string sCode, string sEvent, NSPatPathoArray* pPatho, int colonne)
{
	if ((string("") == sEvent)|| (string("") == sCode) || (NULL == pPatho))
		return false ;

	pPatho->ajoutePatho(sCode, colonne) ;
	colonne++ ;
	pPatho->ajoutePatho(sEvent, colonne) ;

	return true ;
}

void
NSLdvGoal::setGoalPatho(NSPatPathoArray* pPatho)
{
	if (NULL == pPatho)
		return ;

	int colonne = 0 ;
	if (string("") != _sCertitude)
	{
		Message Msg ;
    Msg.SetCertitude(_sCertitude) ;
    pPatho->ajoutePatho(_sLexique, &Msg, colonne) ;
	}
	else
		pPatho->ajoutePatho(_sLexique, colonne) ;

	colonne++ ;
  int iColBase = colonne ;

	if (string("") != _sComplementText)
	{
		pPatho->ajoutePatho("LNOMA1", colonne) ;
    colonne++ ;
    Message Msg ;
    Msg.SetTexteLibre(_sComplementText) ;
    pPatho->ajoutePatho("£?????", &Msg, 2) ;
    colonne++ ;
	}

  colonne = iColBase ;

	bool res = insertDate("KOUVR1", _tDateOuverture, pPatho, colonne) ;
	if (false == res)
		res = insertEvent("KEVOU1", _sOpenEventNode, pPatho, colonne) ;
	if (false == res)
		return ;

	switch(_iRythme)
	{
  	case ponctuel :
    	getAllDatesTree(pPatho, colonne) ;
      getAllValuesTree (pPatho, colonne) ;
      break ;
    case cyclic :
    	pPatho->ajoutePatho("1CYCL1", colonne) ;
      getAllValuesTree(pPatho, colonne) ;
      getAllPeriodesTree(pPatho, colonne) ;
      break ;
    case permanent :
    	pPatho->ajoutePatho("KPERM1", colonne) ;
      getAllValuesTree (pPatho, colonne) ;
      break ;
	}

  if (false == _tDateFermeture.estNoLimit())
	  res = insertDate("KFERM1", _tDateFermeture, pPatho, colonne) ;
	if (string("") != _sCloseEventNode)
		res = insertEvent("KEVFE1", _sCloseEventNode, pPatho, colonne) ;
}
//==============================================================================

void
NSLdvGoal::initFromPatho(NSPatPathoArray *pPPt, PatPathoIter *pIter)
{
  if ((NSPatPathoArray*) NULL == pPPt)
    return ;

  if ((NULL == pIter) || (NULL == *pIter) || (pPPt->end() == *pIter))
    return ;

  // Libellé
  string sCodeLex = (**pIter)->getLexique() ;
  int    iBaseCol = (**pIter)->getColonne() ;

  string sLangue = pContexte->getUserLanguage() ;

  _sLexique = sCodeLex ;

  if (string("£?????") != sCodeLex)
    pContexte->getDico()->donneLibelle(sLangue, &sCodeLex, &_sTitre) ;
  // Texte libre - Free text
  else
  	_sTitre = (**pIter)->getTexteLibre() ;

  // Noeud
  _sReference = (**pIter)->getNode() ;
  _sCertitude = (**pIter)->getCertitude() ;
  (*pIter)++ ;

  // Paramètres du problème
  while ((pPPt->end() != *pIter) && ((**pIter)->getColonne() > iBaseCol))
  {
    string sElemLex = (**pIter)->getLexique() ;
    string sSens ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    if ((**pIter)->getColonne() == iBaseCol + 1)
    {
      bool	bIncrIter = true ;

			// Identifiant d'objectif au sein du référentiel
      // Goal ID inside the Referential
      if (string("£RE") == sSens)
      	_sGoalID = (**pIter)->getComplement() ;

      // Dates d'ouverture et de fermeture
      else if (bIncrIter && ((string("KOUVR") == sSens) || (string("KFERM") == sSens)))
      {
        string sUnit   = string("") ;
        string sFormat = string("") ;
        string sValue  = string("") ;

        getPathoValue(pPPt, pIter, sValue, sUnit, sFormat) ;

        // sFormat est du type £D0;03
        if ((string("") != sFormat) && (('D' == sFormat[1]) || ('T' == sFormat[1])) && (string("") != sValue))
        {
          if ((string("2DA01") == sUnit) || (string("2DA02") == sUnit))
          {
            if      (string("KOUVR") == sSens)
              _tDateOuverture.initFromDate(sValue) ;
            else if (string("KFERM") == sSens)
              _tDateFermeture.initFromDate(sValue) ;
          }
        }
        bIncrIter = false ;
      }

      // cyclique
      else if (string("1CYCL") == sSens)
      {
        (*pIter)++ ;
        bIncrIter = false ;
        _iRythme  = NSLdvGoal::cyclic ;
      }

      // permanent
      else if (sSens == "KPERM")
      {
        (*pIter)++ ;
        bIncrIter = false ;
        _iRythme  = NSLdvGoal::permanent ;
      }
      else if (sSens == "LNOMA")
      {
				(*pIter)++ ;
        if (((**pIter)->getColonne() == iBaseCol + 2) && ((**pIter)->getLexique() == "£?????"))
          _sComplementText = (**pIter)->getTexteLibre() ;
        else
          bIncrIter = false ;
      }
      // événement d'ouverture
      else if (sSens == "KEVOU")
      {
        (*pIter)++ ;
        if ((**pIter)->getColonne() == iBaseCol + 2)
          _sOpenEventNode = (**pIter)->getLexique() ;
        else
          bIncrIter = false ;
      }

      // événement de fermeture
      else if (sSens == "KEVFE")
      {
        (*pIter)++ ;
        if ((**pIter)->getColonne() == iBaseCol + 2)
          _sCloseEventNode = (**pIter)->getLexique() ;
        else
          bIncrIter = false ;
      }

      // moment de début
      // starting moment
      else if (sSens == "KMOD0")
      {
        (*pIter)++ ;

        string sMoment     = "" ;
        string sMomentSens = "" ;

        int iColBase = (**pIter)->getColonne() ;

        // Type de moment (autorisé, idéal...)
        while ((*pIter != pPPt->end()) && ((**pIter)->getColonne() == iColBase))
        {
          sMoment = (**pIter)->getLexique() ;
          pContexte->getDico()->donneCodeSens(&sMoment, &sMomentSens) ;

          if ((sMomentSens != "")         &&
              (	(sMomentSens == "KMOD1")  ||
                (sMomentSens == "KMOD2")  ||
                (sMomentSens == "KMOD3")  ||
                (sMomentSens == "KMOD4")  ||
                (sMomentSens == "KMOD5")  ||
                (sMomentSens == "KMOD6")))
          {
            (*pIter)++ ;

            // Date fixe ou durée depuis le démarrage du cycle
            while ((pPPt->end() != *pIter) && ((**pIter)->getColonne() > iColBase))
            {
              string sTypeMomentSens = (**pIter)->getLexiqueSens() ;

              if ((sTypeMomentSens == "VDURE") || (sTypeMomentSens == "KDAT0"))
              {
                string sUnit   = string("") ;
                string sFormat = string("") ;
                string sValue  = string("") ;

                getPathoValue(pPPt, pIter, sValue, sUnit, sFormat) ;

                // Date précise
                if ((string("") != sFormat) && (('D' == sFormat[1]) || ('T' == sFormat[1])) && (string("") != sValue))
                {
                  switch (sMomentSens[4])
                  {
                    case '1'	:   _sDateDebutAutorise    = sValue ;  // moment de début autorisé
                                  break ;
                    case '2'	:   _sDateDebutConseille   = sValue ;  // moment de début conseillé
                                  break ;
                    case '3'	:   _sDateDebutIdeal       = sValue ;  // moment de début idéal
                                  break ;
                    case '4'	:	  _sDateDebutIdealMax    = sValue ;  // moment de début idéal maximum
                                  break ;
                    case '5'	:	  _sDateDebutConseilMax  = sValue ; // moment de début conseillé maximum
                                  break ;
                    case '6'	:	  _sDateDebutCritique    = sValue ; // moment de début critique
                                  break ;
                  }
                }

                // Durée
                else if ((string("") != sFormat) && ('N' == sFormat[1]) && (string("") != sValue) && (string("") != sUnit))
                {
                  double dvalue = StringToDouble(sValue) ;

                  switch (sMomentSens[4])
                  {
                    case '1'	:   _dDelaiDebutAutorise = dvalue ;   // moment de début autorisé
                                  _sUniteDebutAutorise = sUnit ;
                                  break ;
                    case '2'	:   _dDelaiDebutConseille = dvalue ;  // moment de début conseillé
                                  _sUniteDebutConseille = sUnit ;
                                  break ;
                    case '3'	:   _dDelaiDebutIdeal = dvalue ;      // moment de début idéal
                                  _sUniteDebutIdeal = sUnit ;
                                  break ;
                    case '4'	:	  _dDelaiDebutIdealMax = dvalue ;   // moment de début idéal maximum
                                  _sUniteDebutIdealMax = sUnit ;
                                  break ;
                    case '5'	:	  _dDelaiDebutConseilMax = dvalue ; // moment de début conseillé maximum
                                  _sUniteDebutConseilMax = sUnit ;
                                  break ;
                    case '6'	:	  _dDelaiDebutCritique = dvalue ;   // moment de début critique
                                  _sUniteDebutCritique = sUnit ;
                                  break ;
                  }
                }
              }
              else
                (*pIter)++ ;
            }
          }
          else
            (*pIter)++ ;
        }
        bIncrIter = false ;
      }

      // moment de fin
      else if (sSens == "KMOF0")
      {
        (*pIter)++ ;

        string 	sMoment     = "" ;
        string  sMomentSens	= "" ;

        int iColBase = (**pIter)->getColonne() ;

        while ((*pIter != pPPt->end()) && ((**pIter)->getColonne() == iColBase))
        {
          sMoment = (**pIter)->getLexique() ;
          pContexte->getDico()->donneCodeSens(&sMoment, &sMomentSens) ;

          if ((sMomentSens != "") &&
              (	(sMomentSens == "KMOF1") ||
                (sMomentSens == "KMOF2") ||
                (sMomentSens == "KMOF3") ||
                (sMomentSens == "KMOF4") ||
                (sMomentSens == "KMOF5") ||
                (sMomentSens == "KMOF6")))
          {
            string sUnit   = string("") ;
            string sFormat = string("") ;
            string sValue  = string("") ;

            getPathoValue(pPPt, pIter, sValue, sUnit, sFormat) ;

            if ((string("") != sFormat) && (('D' == sFormat[1]) || ('T' == sFormat[1]) || ('N' == sFormat[1])) && (string("") != sValue))
            {
              switch (sMomentSens[4])
              {
                case '1'	: // moment de fin autorisé						(sMomentSens == "KMOF1")
                case '2'	: // moment de fin conseillé					(sMomentSens == "KMOF2")
                case '3'	: // moment de fin idéal							(sMomentSens == "KMOF3")
                case '4'	:	// moment de fin idéal maximum			(sMomentSens == "KMOF4")
                case '5'	:	// moment de fin conseillé maximum	(sMomentSens == "KMOF5")
                case '6'	:	// moment de fin critique						(sMomentSens == "KMOF6")
                //pGoal->pGoalInfos->push_back(new NSLdvGoalInfo(sMomentSens, sValeur, sUnite, sFormat)) ;
                break ;
              }
            }
          }
        }
        bIncrIter = false ;
      }

      // durée
      else if (sSens == "VDURE")
      {
        (*pIter)++ ;

        string 	sDuree      = "" ;
        string  sDureeSens	= "" ;

        int iColBase = (**pIter)->getColonne() ;

        while ((*pIter != pPPt->end()) && ((**pIter)->getColonne() == iColBase))
        {
          sDuree = (**pIter)->getLexique() ;
          pContexte->getDico()->donneCodeSens(&sDuree, &sDureeSens) ;

          if ((sDureeSens != "") &&
              (	(sDureeSens == "VDUR1") ||
                (sDureeSens == "VDUR2") ||
                (sDureeSens == "VDUR3") ||
                (sDureeSens == "VDUR4") ||
                (sDureeSens == "VDUR5") ||
                (sDureeSens == "VDUR6")))
          {
            string sUnit   = string("") ;
            string sFormat = string("") ;
            string sValue  = string("") ;

            getPathoValue(pPPt, pIter, sValue, sUnit, sFormat) ;

            if ((string("") != sFormat) && (('D' == sFormat[1]) || ('T' == sFormat[1]) || ('N' == sFormat[1])) && (string("") != sValue))
            {
              switch (sDureeSens[4])
              {
                case '1'	: // durée|s| minimale autorisée	(sDureeSens == "VDUR1")
                case '2'	: // durée|s| minimale conseillée	(sDureeSens == "VDUR2")
                case '3'	: // durée|s| idéale minimale			(sDureeSens == "VDUR3")
                case '4'	: // durée|s| idéale maximale			(sDureeSens == "VDUR4")
                case '5'	: // durée|s| maximale conseillée	(sDureeSens == "VDUR5")
                case '6'	: // durée|s| maximale autorisée	(sDureeSens == "VDUR6")
                //pGoal->pGoalInfos->push_back(new NSLdvGoalInfo(sDureeSens, sValeur, sUnite, sFormat)) ;
                break ;
              }
            }
          }
        }
        bIncrIter = false ;
      }

      // enveloppe de valeur
      else if (sSens == "0ENVV")
      {
        (*pIter)++ ;

        string 	sValue      = "" ;
        string  sValueSens	= "" ;

        int iColBase = (**pIter)->getColonne() ;

        while ((*pIter != pPPt->end()) && ((**pIter)->getColonne() == iColBase))
        {
          sValue = (**pIter)->getLexique() ;
          pContexte->getDico()->donneCodeSens(&sValue, &sValueSens) ;

          if ((sValueSens != "") &&
              ( (sValueSens == "VMIN0") ||
                (sValueSens == "VMIN1") ||
                (sValueSens == "VMIN2") ||
                (sValueSens == "VMAX0") ||
                (sValueSens == "VMAX1") ||
                (sValueSens == "VMAX2")))
          {
            string sUnit   = string("") ;
            string sFormat = string("") ;
            string sValue  = string("") ;

            getPathoValue(pPPt, pIter, sValue, sUnit, sFormat) ;

            if ((string("") != sFormat) && (('D' == sFormat[1]) || ('T' == sFormat[1]) || ('N' == sFormat[1])) && (string("") != sValue))
            {
              double dValue = StringToDouble(sValue) ;

              if ((sValueSens[2] == 'I') && (sValueSens[3] == 'N'))   // VMINx
              {
                switch (sValueSens[4])
                {
                  case '0'  :   _dValMinIdeal          = dValue ;
                                _sUniteValMinIdeal     = sUnit ;
                                _bValMinIdeal          = true ;
                                _bValue                = true ;
                                break ;

                  case '1'	:   _dValMinConseille      = dValue ;
                                _sUniteValMinConseille = sUnit ;
                                _bValMinConseille      = true ;
                                _bValue                = true ;
                                break ;

                  case '2'	:   _dValMinAutorise       = dValue ;
                                _sUniteValMinAutorise  = sUnit ;
                                _bValMinAutorise       = true ;
                                _bValue                = true ;
                                break ;

                  //pGoal->pGoalInfos->push_back(new NSLdvGoalInfo(sDureeSens, sValeur, sUnite, sFormat)) ;
                }
              }

              if ((sValueSens[2] == 'A') && (sValueSens[3] == 'X'))   // VMAXx
              {
                switch (sValueSens[4])
                {
                  case '0'  :   _dValMaxIdeal          = dValue ;
                                _sUniteValMaxIdeal     = sUnit ;
                                _bValMaxIdeal          = true ;
                                _bValue                = true ;
                                break ;

                  case '1'	:   _dValMaxConseille      = dValue ;
                                _sUniteValMaxConseille = sUnit ;
                                _bValMaxConseille      = true ;
                                _bValue                = true ;
                                break ;

                  case '2'	:   _dValMaxAutorise       = dValue ;
                                _sUniteValMaxAutorise  = sUnit ;
                                _bValMaxAutorise       = true ;
                                _bValue                = true ;
                                break ;

                  //pGoal->pGoalInfos->push_back(new NSLdvGoalInfo(sDureeSens, sValeur, sUnite, sFormat)) ;
                }
              }

            }
          }
        } // end while
        bIncrIter = false ;
      }

      if (bIncrIter)
        (*pIter)++ ;
    }
    else
      (*pIter)++ ;
  } // Fin de l'énumération des paramètres de l'objectif

	initPresetIntervals() ;
}

void
NSLdvGoal::getPathoValue(NSPatPathoArray *pPPt, PatPathoIter *pIter, string &sValue, string &sUnit, string &sFormat)
{
  sValue  = string("") ;
  sUnit   = string("") ;
  sFormat = string("") ;

  if ((NSPatPathoArray*) NULL == pPPt)
    return ;

  if ((NULL == pIter) || (NULL == *pIter) || (pPPt->end() == *pIter))
    return ;

  int iBaseCol = (**pIter)->getColonne() ;

  (*pIter)++ ;

  while ((pPPt->end() != *pIter) && ((**pIter)->getColonne() > iBaseCol))
  {
    string sFrt = (**pIter)->getLexiqueSens() ;
    if ((string("") != sFrt) && ('£' == sFrt[0]))
    {
      sFormat = sFrt ;
      sUnit   = (**pIter)->getUnitSens() ;
      sValue  = (**pIter)->getComplement() ;

      (*pIter)++ ;

      return ;
    }

    (*pIter)++ ;
  }
}

void
NSLdvGoal::initJalons()
{
try
{
	_aJalons.vider() ;

	// Si pas ouvert, on sort
	if (_tOuvertLe.estVide())
		return ;

	// Si refusé (fermé avant (ou en même temps) que son ouverture, on sort
	if ((false == _tFermeLe.estVide()) && (_tOuvertLe >= _tFermeLe))
		return ;

	string sDateOuvert = _tOuvertLe.donneDate() ;
	NSLdvGoalInfo::JALONSLEVELS iLastValueLevel = NSLdvGoalInfo::None ;

	// Premier jalon : ouverture correspond à tOuvertLe
  //
	NSLdvGoalInfo *pOpenInfo   = new NSLdvGoalInfo() ;

	pOpenInfo->sGoalReference  = _sReference ;
	pOpenInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonOuverture ;
	pOpenInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;

	// Si l'objectif n'est pas cyclique, on détermine la couleur du jalon
	// d'ouverture, en fonction des dates de début
	//                          AVrouge = 1, AVjaune, AVvert, Bleu, APvert, APjaune, AProuge
	if (NSLdvGoal::ponctuel == _iRythme)
	{
		// TimeLevel
    pOpenInfo->iTimeLevel = getColorForPonctualDate(sDateOuvert) ;

		// ValueLevel
		if (_bValueOuverture)
			pOpenInfo->iValueLevel = getValueLevel(_sValueOuverture, _sUnitOuverture, &iLastValueLevel) ;
	}

	if (string("") != _sOpenEventNode)
		pOpenInfo->sEventNode = _sOpenEventNode ;

	pOpenInfo->tpsInfo      = _tOuvertLe ;
	pOpenInfo->tpsClosed    = _tOuvertLe ;
	pOpenInfo->sValue       = _sValueOuverture ;
	pOpenInfo->dValue       = _dValueOuverture ;
	pOpenInfo->sUnit        = _sUnitOuverture ;

	// dans le cas d'un objectif permanent, on suppose qu'à l'ouverture l'objectif
	// n'est pas rempli ; si le premier reseter est <= ouverture, on le remet à
	// bleu plus bas
	//
	// in the case of a permanent goal, we suppose that the goal is not realized
	// at opening
	//
	if (NSLdvGoal::permanent == _iRythme)
		pOpenInfo->iTimeLevel = NSLdvGoalInfo::AProuge ;

	pOpenInfo->computeLevel() ;

	_aJalons.push_back(pOpenInfo) ;

	// Objectif cyclique, on va de reseter en reseter
	// Cyclic goal : we go from one reseter event to the other
	if (NSLdvGoal::cyclic == _iRythme)
	{
    // Récupération des reseters
    // Asking for this goal's reseter events
		NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

    VecteurString aVecteurString ;
    pGraphe->TousLesVrais(_sReference, NSRootLink::goalReseter, &aVecteurString, "ENVERS") ;

    VecteurString aVecteurDates ;

    if (false == aVecteurString.empty())
    {
      // On remplace les noeuds par leur date
      EquiItemIter strIter = aVecteurString.begin() ;
      for ( ; aVecteurString.end() != strIter ; strIter++)
      {
        string sDateDoc = getNodeDate(*(*strIter)) ;
        // sDateDoc = Date ou ""
        aVecteurDates.AddString(sDateDoc) ;
      }

      // On crée les jalons de reset, par ordre chronologique
      while (false == aVecteurDates.empty())
      {
        string sDateMin           = string("ZZZZ") ;
        EquiItemIter dateIter     = aVecteurDates.begin() ;
        EquiItemIter theDateIter  = aVecteurDates.begin() ;
        EquiItemIter noeudIter    = aVecteurString.begin() ;
        EquiItemIter theNoeudIter = aVecteurString.begin() ;

        for ( ; aVecteurDates.end() != dateIter ; dateIter++, noeudIter++)
        {
          if (*(*dateIter) < sDateMin)
          {
            sDateMin     = *(*dateIter) ;
            theDateIter  = dateIter ;
            theNoeudIter = noeudIter ;
          }
        }
        if (string("ZZZZ") == sDateMin)
          break ;

        NSLdvGoalInfo *pNewInfo = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference  = _sReference ;
        pNewInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonCycle ;
        pNewInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel      = NSLdvGoalInfo::AVrouge ;

        double dValue ;
        string sValue ;
        string sUnit ;
        if (getNodeValue(*(*theNoeudIter), &sValue, &dValue))
        {
          sUnit                 = getNodeUnit(*(*theNoeudIter)) ;
          pNewInfo->iValueLevel = getValueLevel(sValue, sUnit, &iLastValueLevel) ;
        }

        pNewInfo->computeLevel() ;

        pNewInfo->sEventNode        = *(*theNoeudIter) ;
        pNewInfo->tpsInfo.initFromDate(sDateMin) ;
        // L'événement est considéré comme ponctuel
        pNewInfo->tpsClosed.initFromDate(sDateMin) ;

        pNewInfo->dValue            = dValue ;
        pNewInfo->sValue            = sValue ;
        pNewInfo->sUnit             = sUnit ;

        _aJalons.push_back(pNewInfo) ;

        delete *theNoeudIter ;
        aVecteurString.erase(theNoeudIter) ;

        delete *theDateIter ;
        aVecteurDates.erase(theDateIter) ;
      }
    }
  }

  // objectif permanent, utilisé dans le médicament, ce que l'on veut traduire
  // par un objectif de ce type de rythme est que lorsqu'un objectif consiste à
  // prescrire un médicament, ce médicament doit être prescrit en "permanence"
  //
  // Permanent Goal, it is used in drug oriented goal, this type of goals is
  // used when we want a goal that consists of a drug administration, the drug
  // which has to be prescript, must be present "permanently" in the current
  // patient's drug prescription
  if (NSLdvGoal::permanent == _iRythme)
  {
    // Récupération des reseters et des alarms
    // Asking for this goal's reseter and alarm events
    NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

    // Getting reset nodes IDs
    //
    VecteurString aVSReset ;
    pGraphe->TousLesVrais(_sReference, NSRootLink::goalReseter, &aVSReset, "ENVERS") ;
    //
    // Gettind reset nodes dates
    //
    VecteurString aVDateReset ;
    if (false == aVSReset.empty())
    {
      EquiItemIter resetIter = aVSReset.begin() ;
      for ( ; aVSReset.end() != resetIter ; resetIter++)
      {
        string sDateDoc = getNodeDate(*(*resetIter)) ;
        // sDateDoc = Date ou ""
        aVDateReset.AddString(sDateDoc) ;
      }
    }

    // Getting alarm nodes IDs
    //
    VecteurString aVSAlarm ;
    pGraphe->TousLesVrais(_sReference, NSRootLink::goalAlarm,   &aVSAlarm, "ENVERS") ;
    //
    // Getting alarm nodes dates
    //
    VecteurString aVDateAlarm ;
    if (false == aVSAlarm.empty())
    {
      EquiItemIter alarmIter = aVSAlarm.begin() ;
      for ( ; aVSAlarm.end() != alarmIter ; alarmIter++)
      {
        string sDateDoc = getNodeDate(*(*alarmIter)) ;
        // sDateDoc = Date ou ""
        aVDateAlarm.AddString(sDateDoc) ;
      }
    }

    if ((false == aVSReset.empty()) || (false == aVSAlarm.empty()))
    {
      int iResetCpt = 0 ;

      // On crée les jalons, par ordre chronologique
      // Creating "jalons", in chronological order
      //
      while ((false == aVDateReset.empty()) || (false == aVDateAlarm.empty()))
      {
        string        sDateMin      = "ZZZZ" ;
        EquiItemIter  theDateIter   = NULL ;
        EquiItemIter  theNodeIter   = NULL ;
        bool          bResetNode    = false ;
        bool          bAlarmNode    = false ;

        // on recherche le plus ancien dans les reseters
        // searching in reseters the older one
        if (false == aVDateReset.empty())
        {
          EquiItemIter dateResetIter = aVDateReset.begin() ;
          EquiItemIter nodeResetIter = aVSReset.begin() ;

          for ( ; aVDateReset.end() != dateResetIter ; dateResetIter++, nodeResetIter++)
          {
            if (*(*dateResetIter) < sDateMin)
            {
              sDateMin    = *(*dateResetIter) ;
              theDateIter = dateResetIter ;
              theNodeIter = nodeResetIter ;
              bAlarmNode  = false ;
              bResetNode  = true ;
            }
          }
        }

        // on recherche si un alarm n'est pas plus ancien
        // searching in alarms an older one
        if (false == aVDateAlarm.empty())
        {
          EquiItemIter dateAlarmIter = aVDateAlarm.begin() ;
          EquiItemIter nodeAlarmIter = aVSAlarm.begin() ;

          for ( ; aVDateAlarm.end() != dateAlarmIter ; dateAlarmIter++, nodeAlarmIter++)
          {
            if (*(*dateAlarmIter) < sDateMin)
            {
              sDateMin    = *(*dateAlarmIter) ;
              theDateIter = dateAlarmIter ;
              theNodeIter = nodeAlarmIter ;
              bResetNode  = false ;
              bAlarmNode  = true ;
            }
          }
        }

        // si le plus ancien trouvé est un reseters on incrémente le compteur
        // si c'est un alarm, on le décremente
        if      (bResetNode)
          iResetCpt++ ;
        else if (bAlarmNode)
          iResetCpt-- ;

        // Eventuellement, on remet à jour pOpenInfo->iTimeLevel
        //
        if (sDateMin <= pOpenInfo->tpsInfo.donneDateHeure())
        {
          if (iResetCpt > 0)
            pOpenInfo->iTimeLevel  = NSLdvGoalInfo::Bleu ;
          else
            pOpenInfo->iTimeLevel  = NSLdvGoalInfo::AProuge ;
          pOpenInfo->computeLevel() ;
        }

        if (string("ZZZZ") == sDateMin)
          break ;

        NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = _sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonCycle ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;

        if (iResetCpt > 0)
          pNewInfo->iTimeLevel  = NSLdvGoalInfo::Bleu ;
        else
          pNewInfo->iTimeLevel  = NSLdvGoalInfo::AProuge ;

        double dValue ;
        string sValue ;
        string sUnit ;
        if (getNodeValue(*(*theNodeIter), &sValue, &dValue))
        {
          sUnit                 = getNodeUnit(*(*theNodeIter)) ;
          pNewInfo->iValueLevel = getValueLevel(sValue, sUnit, &iLastValueLevel) ;
        }

        pNewInfo->computeLevel() ;

        pNewInfo->sEventNode        = *(*theNodeIter) ;
        pNewInfo->tpsInfo.initFromDate(sDateMin) ;
        // L'événement est considéré comme ponctuel
        pNewInfo->tpsClosed.initFromDate(sDateMin) ;

        pNewInfo->dValue            = dValue ;
        pNewInfo->sValue            = sValue ;
        pNewInfo->sUnit             = sUnit ;

        _aJalons.push_back(pNewInfo) ;

        if (bResetNode)
        {
          // le noeud appartient à la liste des noeuds reseter
          // current node is part of reseter node list
          delete (*theNodeIter) ;
          aVSReset.erase(theNodeIter) ;

          delete (*theDateIter) ;
          aVDateReset.erase(theDateIter) ;
        }
        else if (bAlarmNode)
        {
          // le noeud appartient à la liste des noeuds alarm
          // current node is part of alarm node list
          delete (*theNodeIter) ;
          aVSAlarm.erase(theNodeIter) ;

          delete (*theDateIter) ;
          aVDateAlarm.erase(theDateIter) ;
        }
      }
    }
/*
    else
    {
      NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

      pNewInfo->sGoalReference    = sReference ;
      pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonCycle ;
      pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;

      pNewInfo->iTimeLevel        = NSLdvGoalInfo::AProuge ;

      int           iValue ;
      string        sValue ;
      string        sUnit ;

      pNewInfo->computeLevel() ;

      pNewInfo->sEventNode        = NULL ;
      pNewInfo->tpsInfo.initFromDate(sDateMin) ;
      // L'événement est considéré comme ponctuel
      pNewInfo->tpsClosed.initFromDate(sDateMin) ;

      pNewInfo->iValue            = iValue ;
      pNewInfo->sValue            = sValue ;
      pNewInfo->sUnit             = sUnit ;

      pJalons->push_back(pNewInfo) ;
    }
*/
	}

  // Dernier jalon : fermeture
  if (false == _tFermeLe.estVide())
  {
    NSLdvGoalInfo *pCloseInfo   = new NSLdvGoalInfo() ;

    pCloseInfo->sGoalReference  = _sReference ;
    pCloseInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonFermeture ;
    pCloseInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;
    if (string("") != _sCloseEventNode)
      pCloseInfo->sEventNode    = _sCloseEventNode ;
    pCloseInfo->tpsInfo         = _tFermeLe ;
    pCloseInfo->tpsClosed       = _tFermeLe ;

    pCloseInfo->dValue          = _dValueFermeture ;
    pCloseInfo->sValue          = _sValueFermeture ;
    pCloseInfo->sUnit           = _sUnitFermeture ;

    _aJalons.push_back(pCloseInfo) ;
  }

  // On trie les jalons (ils devraient déjà l'être, mais...)
  sort(_aJalons.begin(), _aJalons.end(), infGoalInfo) ;

  // Jalons intermédiaires
	if (_aJalons.empty())
		return ;

  // Tour de passe-passe intermédiaire : si un jalon précède le jalon de
  // début, il devient jalon de début
  GoalInfoIter itGoals = _aJalons.begin() ;
  if (NSLdvGoalInfo::jalonOuverture != (*itGoals)->iTypeJalonEvent)
  {
    (*itGoals)->iTypeJalonEvent = NSLdvGoalInfo::jalonOuvreCycle ;
    itGoals++ ;

    // On supprime le jalon d'ouverture
    for ( ; (_aJalons.end() != itGoals) && (NSLdvGoalInfo::jalonOuverture != (*itGoals)->iTypeJalonEvent) ; itGoals++)
      ;

    if (_aJalons.end() != itGoals)
    {
      delete *itGoals ;
      _aJalons.erase(itGoals) ;
    }
  }

	// Objectif ponctuel : on pose les jalons intermédiaires après
  // la date d'ouverture, et jusqu'à l'éventuelle date de fermeture
  //
  if (NSLdvGoal::ponctuel == _iRythme)
	{
  	// Recherche du jalon d'ouverture - Looking for opening bookmark
    //
  	GoalInfoIter itGoalsBegin = _aJalons.begin() ;
  	for ( ; (_aJalons.end() != itGoalsBegin) &&
           (NSLdvGoalInfo::jalonOuverture != (*itGoalsBegin)->iTypeJalonEvent) ;
  					itGoalsBegin++) ;
		if (_aJalons.end() != itGoalsBegin)
		{
			// Recherche du jalon de fermeture - Looking for closing bookmark
    	//
			itGoals = _aJalons.begin() ;
  		for ( ; (_aJalons.end() != itGoals) &&
              (NSLdvGoalInfo::jalonFermeture != (*itGoals)->iTypeJalonEvent) ;
  					itGoals++) ;

    	if (_aJalons.end() != itGoals)
      	initPostDateInterJalons(*itGoalsBegin, (*itGoals)->tpsInfo) ;
			else
      {
      	NVLdVTemps tpsInfinite ;
        tpsInfinite.setNoLimit() ;
        initPostDateInterJalons(*itGoalsBegin, tpsInfinite) ;
			}
		}
	}

	if (NSLdvGoal::cyclic == _iRythme)
	{
  	// Recherche du jalon d'ouverture - Looking for opening bookmark
    //
  	itGoals = _aJalons.begin() ;
  	for ( ; (_aJalons.end() != itGoals) &&
            (NSLdvGoalInfo::jalonOuverture != (*itGoals)->iTypeJalonEvent) ;
  					itGoals++) ;
		if (_aJalons.end() != itGoals)
		{
    	NSLdvGoalInfo* pRefJalon = *itGoals ;

      itGoals++ ;

      for ( ; (_aJalons.end() != itGoals) &&
          ((*itGoals)->iTypeJalonEvent != NSLdvGoalInfo::jalonIntermediaire) &&
          ((*itGoals)->iTypeJalonEvent != NSLdvGoalInfo::jalonFermeture) ; itGoals++)
      {
      	initInterDelayJalons(pRefJalon, (*itGoals)->tpsInfo) ;
        pRefJalon = *itGoals ;
      }

      if ((itGoals != _aJalons.end()) &&
          ((*itGoals)->iTypeJalonEvent == NSLdvGoalInfo::jalonFermeture))
    		initInterDelayJalons(pRefJalon, (*itGoals)->tpsInfo) ;
      else
      {
      	NVLdVTemps tpsInfinite ;
        tpsInfinite.setNoLimit() ;
        initInterDelayJalons(pRefJalon, tpsInfinite) ;
			}
		}
	}

/*
  itGoals = pJalons->begin() ;
  for ( ; (itGoals != pJalons->end()) &&
          ((*itGoals)->iTypeJalonEvent != NSLdvGoalInfo::jalonIntermediaire) &&
          ((*itGoals)->iTypeJalonEvent != NSLdvGoalInfo::jalonFermeture) ; itGoals++)
  {
    // Les dates fixes sont créées par rapport au jalon d'ouverture
    if ((*itGoals)->iTypeJalonEvent == NSLdvGoalInfo::jalonOuverture)
    {

      // création des jalons intermédiaires situés entre ce jalon et la date d'ouverture
      // we create the intermediate bookmarks situated between this one and the opening date

      // Jalon Début autorisé

      // La date d'ouverture de l'objectif est plus ancienne que la date de début autorisé
      //
      if ((sDateDebutAutorise != "") && (sDateDebutAutorise > sDateOuvert))
      {
        tpsBuff.initFromDate(sDateDebutAutorise) ;

        if (tpsBuff <= (*itGoals)->tpsInfo)
        {
          (*itGoals)->iTimeLevel      = NSLdvGoalInfo::AVjaune ;
          (*itGoals)->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          (*itGoals)->computeLevel() ;
        }
        else
        {
          NSLdvGoalInfo *pCloseInfo   = new NSLdvGoalInfo() ;

          pCloseInfo->sGoalReference  = sReference ;
          pCloseInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonIntermediaire ;
          pCloseInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;
          pCloseInfo->iTimeLevel      = NSLdvGoalInfo::AVjaune ;
          pCloseInfo->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          pCloseInfo->dValue          = (*itGoals)->dValue ;
          pCloseInfo->sValue          = (*itGoals)->sValue ;
          pCloseInfo->sUnit           = (*itGoals)->sUnit ;
          pCloseInfo->tpsInfo         = tpsBuff ;

          pCloseInfo->computeLevel() ;

          pJalons->push_back(pCloseInfo) ;
        }
      }

      // Jalon Début conseillé
      if ((sDateDebutConseille != "") && (sDateDebutConseille > sDateOuvert))
      {
        tpsBuff.initFromDate(sDateDebutConseille) ;

        if (tpsBuff <= (*itGoals)->tpsInfo)
        {
          (*itGoals)->iTimeLevel      = NSLdvGoalInfo::AVvert ;
          (*itGoals)->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          (*itGoals)->computeLevel() ;
        }
        else
        {
          NSLdvGoalInfo *pCloseInfo   = new NSLdvGoalInfo() ;

          pCloseInfo->sGoalReference  = sReference ;
          pCloseInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonIntermediaire ;
          pCloseInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;
          pCloseInfo->iTimeLevel      = NSLdvGoalInfo::AVvert ;
          pCloseInfo->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          pCloseInfo->dValue          = (*itGoals)->dValue ;
          pCloseInfo->sValue          = (*itGoals)->sValue ;
          pCloseInfo->sUnit           = (*itGoals)->sUnit ;
          pCloseInfo->tpsInfo         = tpsBuff ;

          pCloseInfo->computeLevel() ;

          pJalons->push_back(pCloseInfo) ;
        }
      }

      // Jalon Début idéal
      if ((sDateDebutIdeal != "") && (sDateDebutIdeal > sDateOuvert))
      {
        tpsBuff.initFromDate(sDateDebutIdeal) ;

        if (tpsBuff <= (*itGoals)->tpsInfo)
        {
          (*itGoals)->iTimeLevel      = NSLdvGoalInfo::Bleu ;
          (*itGoals)->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          (*itGoals)->computeLevel() ;
        }
        else
        {
          NSLdvGoalInfo *pCloseInfo   = new NSLdvGoalInfo() ;

          pCloseInfo->sGoalReference  = sReference ;
          pCloseInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonIntermediaire ;
          pCloseInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;
          pCloseInfo->iTimeLevel      = NSLdvGoalInfo::Bleu ;
          pCloseInfo->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          pCloseInfo->dValue          = (*itGoals)->dValue ;
          pCloseInfo->sValue          = (*itGoals)->sValue ;
          pCloseInfo->sUnit           = (*itGoals)->sUnit ;
          pCloseInfo->tpsInfo         = tpsBuff ;

          pCloseInfo->computeLevel() ;

          pJalons->push_back(pCloseInfo) ;
        }
      }

      // Jalon Début idéal maximum
      if ((sDateDebutIdealMax != "") && (sDateDebutIdealMax > sDateOuvert))
      {
        tpsBuff.initFromDate(sDateDebutIdealMax) ;

        if (tpsBuff > (*itGoals)->tpsInfo)
        {
          NSLdvGoalInfo *pCloseInfo   = new NSLdvGoalInfo() ;

          pCloseInfo->sGoalReference  = sReference ;
          pCloseInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonIntermediaire ;
          pCloseInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;
          pCloseInfo->iTimeLevel      = NSLdvGoalInfo::APvert ;
          pCloseInfo->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          pCloseInfo->dValue          = (*itGoals)->dValue ;
          pCloseInfo->sValue          = (*itGoals)->sValue ;
          pCloseInfo->sUnit           = (*itGoals)->sUnit ;
          pCloseInfo->tpsInfo         = tpsBuff ;

          pCloseInfo->computeLevel() ;

          pJalons->push_back(pCloseInfo) ;
        }
      }

      // Jalon Début conseillé maximum
      if ((sDateDebutConseilMax != "") && (sDateDebutConseilMax > sDateOuvert))
      {
        tpsBuff.initFromDate(sDateDebutConseilMax) ;

        if (tpsBuff > (*itGoals)->tpsInfo)
        {
          NSLdvGoalInfo *pCloseInfo   = new NSLdvGoalInfo() ;

          pCloseInfo->sGoalReference  = sReference ;
          pCloseInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonIntermediaire ;
          pCloseInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;
          pCloseInfo->iTimeLevel      = NSLdvGoalInfo::APjaune ;
          pCloseInfo->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          pCloseInfo->dValue          = (*itGoals)->dValue ;
          pCloseInfo->sValue          = (*itGoals)->sValue ;
          pCloseInfo->sUnit           = (*itGoals)->sUnit ;
          pCloseInfo->tpsInfo         = tpsBuff ;

          pCloseInfo->computeLevel() ;

          pJalons->push_back(pCloseInfo) ;
        }
      }

      // Jalon Début critique
      if ((sDateDebutCritique != "")  && (sDateDebutCritique > sDateOuvert))
      {
        tpsBuff.initFromDate(sDateDebutCritique) ;

        if (tpsBuff > (*itGoals)->tpsInfo)
        {
          NSLdvGoalInfo *pCloseInfo   = new NSLdvGoalInfo() ;

          pCloseInfo->sGoalReference  = sReference ;
          pCloseInfo->iTypeJalonEvent = NSLdvGoalInfo::jalonIntermediaire ;
          pCloseInfo->iTypeJalon      = NSLdvGoalInfo::jalonDebut ;
          pCloseInfo->iTimeLevel      = NSLdvGoalInfo::AProuge ;
          pCloseInfo->iValueLevel     = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
          pCloseInfo->dValue          = (*itGoals)->dValue ;
          pCloseInfo->sValue          = (*itGoals)->sValue ;
          pCloseInfo->sUnit           = (*itGoals)->sUnit ;
          pCloseInfo->tpsInfo         = tpsBuff ;

          pCloseInfo->computeLevel() ;

          pJalons->push_back(pCloseInfo) ;
        }
      }
    }

    // Durées
    bool bNextGoal = false ;
    GoalInfoIter itNextGoals = itGoals ;
    if (itNextGoals != pJalons->end())
    {
      itNextGoals++ ;
      if ((itNextGoals != pJalons->end()) && ((*itNextGoals)->iTypeJalonEvent != NSLdvGoalInfo::jalonIntermediaire))
        bNextGoal = true ;
    }

    // Jalon Début autorisé
    if (dDelaiDebutAutorise > 0)
    {
      tpsBuff = (*itGoals)->tpsInfo ;
      tpsBuff.ajouteTemps(int(dDelaiDebutAutorise), sUniteDebutAutorise, pContexte);

      if ((!bNextGoal) || (bNextGoal && ((*itNextGoals)->tpsInfo > tpsBuff)))
      {
        NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonIntermediaire ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel        = NSLdvGoalInfo::AVjaune ;
        pNewInfo->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
        pNewInfo->dValue            = (*itGoals)->dValue ;
        pNewInfo->sValue            = (*itGoals)->sValue ;
        pNewInfo->sUnit             = (*itGoals)->sUnit ;
        pNewInfo->tpsInfo           = tpsBuff ;

        pNewInfo->computeLevel() ;

        pJalons->push_back(pNewInfo) ;
      }
    }
    else if (dDelaiDebutAutorise == 0)
    {
      (*itGoals)->iTimeLevel        = NSLdvGoalInfo::AVjaune ;
      (*itGoals)->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
      (*itGoals)->computeLevel() ;
    }

    // Jalon Début conseillé
    if (dDelaiDebutConseille > 0)
    {
      tpsBuff = (*itGoals)->tpsInfo ;
      tpsBuff.ajouteTemps(int(dDelaiDebutConseille), sUniteDebutConseille, pContexte) ;

      if ((!bNextGoal) || (bNextGoal && ((*itNextGoals)->tpsInfo > tpsBuff)))
      {
        NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonIntermediaire ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel        = NSLdvGoalInfo::AVvert ;
        pNewInfo->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
        pNewInfo->dValue            = (*itGoals)->dValue ;
        pNewInfo->sValue            = (*itGoals)->sValue ;
        pNewInfo->sUnit             = (*itGoals)->sUnit ;
        pNewInfo->tpsInfo           = tpsBuff ;

        pNewInfo->computeLevel() ;

        pJalons->push_back(pNewInfo) ;
      }
    }
    else if (dDelaiDebutConseille == 0)
    {
      (*itGoals)->iTimeLevel        = NSLdvGoalInfo::AVvert ;
      (*itGoals)->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
      (*itGoals)->computeLevel() ;
    }

    // Jalon Début idéal
    if (dDelaiDebutIdeal > 0)
    {
      tpsBuff = (*itGoals)->tpsInfo ;
      tpsBuff.ajouteTemps(int(dDelaiDebutIdeal), sUniteDebutIdeal, pContexte);

      if ((!bNextGoal) || (bNextGoal && ((*itNextGoals)->tpsInfo > tpsBuff)))
      {
        NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonIntermediaire ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel        = NSLdvGoalInfo::Bleu ;
        pNewInfo->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
        pNewInfo->dValue            = (*itGoals)->dValue ;
        pNewInfo->sValue            = (*itGoals)->sValue ;
        pNewInfo->sUnit             = (*itGoals)->sUnit ;
        pNewInfo->tpsInfo           = tpsBuff ;

        pNewInfo->computeLevel() ;

        pJalons->push_back(pNewInfo) ;
      }
    }
    else if (dDelaiDebutIdeal == 0)
    {
      (*itGoals)->iTimeLevel        = NSLdvGoalInfo::Bleu ;
      (*itGoals)->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
      (*itGoals)->computeLevel() ;
    }

    // Jalon Début idéal maximum
    if (dDelaiDebutIdealMax > 0)
    {
      tpsBuff = (*itGoals)->tpsInfo ;
      tpsBuff.ajouteTemps(int(dDelaiDebutIdealMax), sUniteDebutIdealMax, pContexte);

      if ((!bNextGoal) || (bNextGoal && ((*itNextGoals)->tpsInfo > tpsBuff)))
      {
        NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonIntermediaire ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel        = NSLdvGoalInfo::APvert ;
        pNewInfo->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
        pNewInfo->dValue            = (*itGoals)->dValue ;
        pNewInfo->sValue            = (*itGoals)->sValue ;
        pNewInfo->sUnit             = (*itGoals)->sUnit ;
        pNewInfo->tpsInfo           = tpsBuff ;

        pNewInfo->computeLevel() ;

        pJalons->push_back(pNewInfo) ;
      }
    }

    // Jalon Début conseillé maximum
    if (dDelaiDebutConseilMax > 0)
    {
      tpsBuff = (*itGoals)->tpsInfo ;
      tpsBuff.ajouteTemps(int(dDelaiDebutConseilMax), sUniteDebutConseilMax, pContexte);

      if ((!bNextGoal) || (bNextGoal && ((*itNextGoals)->tpsInfo > tpsBuff)))
      {
        NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonIntermediaire ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel        = NSLdvGoalInfo::APjaune ;
        pNewInfo->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
        pNewInfo->dValue            = (*itGoals)->dValue ;
        pNewInfo->sValue            = (*itGoals)->sValue ;
        pNewInfo->sUnit             = (*itGoals)->sUnit ;
        pNewInfo->tpsInfo           = tpsBuff ;

        pNewInfo->computeLevel() ;

        pJalons->push_back(pNewInfo) ;
      }
    }

    // Jalon Début critique
    if (dDelaiDebutCritique > 0)
    {
      tpsBuff = (*itGoals)->tpsInfo ;
      tpsBuff.ajouteTemps(int(dDelaiDebutCritique), sUniteDebutCritique, pContexte);

      if ((!bNextGoal) || (bNextGoal && ((*itNextGoals)->tpsInfo > tpsBuff)))
      {
        NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonIntermediaire ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel        = NSLdvGoalInfo::AProuge ;
        pNewInfo->iValueLevel       = getValueLevel((*itGoals)->sValue, (*itGoals)->sUnit, &iLastValueLevel) ;
        pNewInfo->dValue            = (*itGoals)->dValue ;
        pNewInfo->sValue            = (*itGoals)->sValue ;
        pNewInfo->sUnit             = (*itGoals)->sUnit ;
        pNewInfo->tpsInfo           = tpsBuff ;

        pNewInfo->computeLevel() ;

        pJalons->push_back(pNewInfo) ;
      }
    }
  }
*/

  // On trie les jalons
  sort(_aJalons.begin(), _aJalons.end(), infGoalInfo) ;

  // On initialise la date de fermeture des jalons
  GoalInfoIter closingIt ;
  for (GoalInfoIter jalonIt = _aJalons.begin() ; _aJalons.end() != jalonIt ; jalonIt++)
  {
    // On initialise la date de fermeture du jalon
    closingIt = _aJalons.getClosingJalon(jalonIt) ;

    if (_aJalons.end() != closingIt)
      (*jalonIt)->tpsClosed = (*closingIt)->tpsInfo ;
    else
      (*jalonIt)->tpsClosed.setNoLimit() ;
  }

  // Initialisation des Meta-jalons
  initMetaJalons() ;
}
catch (...)
{
	erreur("Exception NSLdvGoal::initJalons.", standardError, 0) ;
}
}

void
NSLdvGoal::initPostDateInterJalons(NSLdvGoalInfo* pRefJalon, NVLdVTemps tpsNextJalonDate)
{
  if ((NSLdvGoalInfo*) NULL == pRefJalon)
    return ;

	// Si la date de référence est déjà AProuge, on n'a pas de jalons
  // intermédiaires au delà
	//
	NSLdvGoalInfo::JALONSLEVELS iLevel = getColorForPonctualDate(pRefJalon->tpsInfo.donneDateHeure()) ;
	if ((NSLdvGoalInfo::None == iLevel) || (NSLdvGoalInfo::AProuge == iLevel))
		return ;

	iLevel = pRefJalon->getNextColor(iLevel) ;

	bool bTourner = true ;
	while (bTourner)
	{
  	NSDateZone* pDateZone = _aDateZones.getZone(iLevel) ;
    if (pDateZone)
		{
    	NVLdVTemps tpsLow  = pDateZone->getLowDate() ;
      NVLdVTemps tpsHigh = pDateZone->getHighDate() ;
      if (tpsLow > tpsNextJalonDate)
				return ;

      if (tpsLow >= pRefJalon->tpsInfo)
      {
      	NSLdvGoalInfo::JALONSLEVELS iLastValueLevel ;

      	NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = _sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonIntermediaire ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel        = iLevel ;
        pNewInfo->iValueLevel       = getValueLevel(pRefJalon->sValue, pRefJalon->sUnit, &iLastValueLevel) ;
        pNewInfo->dValue            = pRefJalon->dValue ;
        pNewInfo->sValue            = pRefJalon->sValue ;
        pNewInfo->sUnit             = pRefJalon->sUnit ;
        pNewInfo->tpsInfo           = tpsLow ;

        pNewInfo->computeLevel() ;

        _aJalons.push_back(pNewInfo) ;
      }
		}
    iLevel = pRefJalon->getNextColor(iLevel) ;
    if (iLevel == NSLdvGoalInfo::None)
    	bTourner = false ;
	}
}

void
NSLdvGoal::initInterDelayJalons(NSLdvGoalInfo* pRefJalon, NVLdVTemps tpsNextJalonDate)
{
  if (NULL == pRefJalon)
    return ;

	NSLdvGoalInfo::JALONSLEVELS iLevel = NSLdvGoalInfo::AVrouge ;

	bool bTourner = true ;
	while (bTourner)
	{
  	NSDelayZone* pDelayZone = _aDelayZones.getZone(iLevel) ;
    if (pDelayZone)
		{
  		NVLdVTemps tpsBuff = pRefJalon->tpsInfo ;
      tpsBuff.ajouteTemps(int(pDelayZone->dLowDelay), pDelayZone->sLowDelayUnit, pContexte) ;
      if (tpsBuff <= tpsNextJalonDate)
      {
      	NSLdvGoalInfo::JALONSLEVELS iLastValueLevel ;

      	NSLdvGoalInfo *pNewInfo     = new NSLdvGoalInfo() ;

        pNewInfo->sGoalReference    = _sReference ;
        pNewInfo->iTypeJalonEvent   = NSLdvGoalInfo::jalonIntermediaire ;
        pNewInfo->iTypeJalon        = NSLdvGoalInfo::jalonDebut ;
        pNewInfo->iTimeLevel        = iLevel ;
        pNewInfo->iValueLevel       = getValueLevel(pRefJalon->sValue, pRefJalon->sUnit, &iLastValueLevel) ;
        pNewInfo->dValue            = pRefJalon->dValue ;
        pNewInfo->sValue            = pRefJalon->sValue ;
        pNewInfo->sUnit             = pRefJalon->sUnit ;
        pNewInfo->tpsInfo           = tpsBuff ;

        pNewInfo->computeLevel() ;

        _aJalons.push_back(pNewInfo) ;
      }
		}
    iLevel = pRefJalon->getNextColor(iLevel) ;
    if (iLevel == NSLdvGoalInfo::None)
    	bTourner = false ;
	}
}

void
NSLdvGoal::initMetaJalons()
{
try
{
  if (_aJalons.empty())
    return ;

  // Variables qui permettent de savoir quelle est la pire couleur en cours
  // et jusqu'à quand elle est d'actualité
  NVLdVTemps tpsMaxTimeForColor ;
  tpsMaxTimeForColor.init() ;
  int iColorLevel = -1 ;

  // On énumère tous les jalons
  for (GoalInfoIter jalonIt = _aJalons.begin() ; _aJalons.end() != jalonIt ; )
  {
    // Si le nouveau jalon est plus grave que le plus grave du moment
    // ça devient le plus grave
    //
    // ATTENTION : on ne tient pas compte des jalons "instantanés"
    //              (moment d'ouverture = moment de fermeture)
    //
    if (((*jalonIt)->iLevel > iColorLevel) && (!((*jalonIt)->tpsClosed == (*jalonIt)->tpsInfo)))
    {
      iColorLevel = (*jalonIt)->iLevel ;

      // On recherche la date de péremption du jalon
      tpsMaxTimeForColor = (*jalonIt)->tpsClosed ;

      // On vérifie qu'il n'y a pas d'autres jalons plus graves ou aussi
      // grave mais plus long à la même date
      GoalInfoIter refIt = jalonIt ;
      jalonIt++ ;
      while ((_aJalons.end() != jalonIt) && ((*jalonIt)->tpsInfo == (*refIt)->tpsInfo))
      {
        if      ((*jalonIt)->iLevel > iColorLevel)
          refIt = jalonIt ;
        else if (((*jalonIt)->iLevel == iColorLevel) && ((*jalonIt)->tpsClosed > tpsMaxTimeForColor))
        {
          tpsMaxTimeForColor = (*jalonIt)->tpsClosed ;
          refIt = jalonIt ;
        }
        jalonIt++ ;
      }

      // On peut maintenant créer le nouveau meta-jalon
      NSLdvGoalInfo *pMetaJalon = new NSLdvGoalInfo(**refIt) ;
      _aMetaJalons.push_back(pMetaJalon) ;
    }

    // Sinon, on regarde si le nouveau jalon est au delà de tpsMaxTimeForColor
    else if ((*jalonIt)->tpsInfo >= tpsMaxTimeForColor)
    {
      // Le jalon en cours est le premier candidat à la succession
      GoalInfoIter refIt  = jalonIt ;
      iColorLevel         = (*jalonIt)->iLevel ;
      tpsMaxTimeForColor  = (*jalonIt)->tpsClosed ;

      // On doit passer en revue tous les jalons plus anciens ou de
      // même date pour voir lequel est susceptible d'imposer sa couleur
      GoalInfoIter jIt = _aJalons.begin() ;
      for ( ; (_aJalons.end() != jIt) && ((*jIt)->tpsInfo <= (*jalonIt)->tpsInfo) ; jIt++)
      {
        if ((*jIt)->tpsClosed > (*jalonIt)->tpsInfo)
        {
          // Postulant à imposer sa couleur
          if (((*jIt)->iLevel > iColorLevel)    ||
              (((*jIt)->iLevel == iColorLevel)  &&
              ((*jIt)->tpsClosed > tpsMaxTimeForColor)))
          {
            refIt = jIt ;
            iColorLevel         = (*jIt)->iLevel ;
            tpsMaxTimeForColor  = (*jIt)->tpsClosed ;
          }
        }
      }

      // On peut maintenant créer le nouveau meta-jalon, avec
      // la date actuelle comme date de départ
      NSLdvGoalInfo *pMetaJalon = new NSLdvGoalInfo(**refIt) ;
      pMetaJalon->tpsInfo = (*jalonIt)->tpsInfo ;
      _aMetaJalons.push_back(pMetaJalon) ;

      jalonIt = jIt ;
    }
    else
      jalonIt++ ;
  }

  // On trie les meta-jalons
  sort(_aMetaJalons.begin(), _aMetaJalons.end(), infGoalInfo) ;

  // On initialise la date de fermeture des meta-jalons
  GoalInfoIter closingIt ;
  for (GoalInfoIter jalonIt = _aMetaJalons.begin() ; _aMetaJalons.end() != jalonIt ; jalonIt++)
  {
    // On initialise la date de fermeture du jalon
    closingIt = _aMetaJalons.getClosingJalon(jalonIt) ;

    if (_aMetaJalons.end() != closingIt)
      (*jalonIt)->tpsClosed = (*closingIt)->tpsInfo ;
    else
      (*jalonIt)->tpsClosed.setNoLimit() ;
  }

  // je ne comprends pas pourquoi on faisait ce test, je l'enlève
  // don't understand why we test this condition, so I get it out
/*
  if (!bValue)
    return ;
*/

  // les meta-jalons ont été créés, il faut maintenant créer un méta-jalon qui
  // correspond à l'instant "now".

  // pour cela on parcourt la liste jusqu'à se trouver à l'emplacement dans la
  // liste où l'on doit rajouter le meta-jalon "now"
  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  bool  bInTheFuture = false ;

  for (GoalInfoIter jalonIt = _aMetaJalons.begin() ; _aMetaJalons.end() != jalonIt ; jalonIt++)
  {
    if (((*jalonIt)->tpsClosed >= tpsNow) && (false == bInTheFuture))
    {
      if ((*jalonIt)->tpsClosed > tpsNow)
      {
        // on doit créer un nouveau jalon
        NSLdvGoalInfo   *pNewJalon = new NSLdvGoalInfo(**jalonIt) ;
        pNewJalon->tpsClosed  = tpsNow ;
        (*jalonIt)->tpsInfo   = tpsNow ;
        jalonIt = _aMetaJalons.insert(jalonIt, pNewJalon) ;
      }
      (*jalonIt)->iTypeJalonEvent = NSLdvGoalInfo::jalonNow ;
      bInTheFuture = true ;
    }

    // on veut modifier les jalons dans le futur pour prendre le iLevel prend la
    // valeur de iTimeLevel
    // on ne prend pas en compte la valeur pour la représentation dans le futur
    else if (bInTheFuture)
      (*jalonIt)->iLevel = (*jalonIt)->iTimeLevel ;
  }
}
catch (...)
{
	erreur("Exception NSLdvGoal::initMetaJalons.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Prévenir la préoccupation à laquelle est rattachée cet objectif que
// l'objectif existe
// -----------------------------------------------------------------------------
void
NSLdvGoal::initConcern(NSLdvDocumentBase *pDocLdv)
{
  if ((NSLdvDocumentBase*) NULL == pDocLdv)
    return ;

  pDocLdv->addGoalToConcern(this) ;
}

double
NSLdvGoal::getMeanDelaiInSecond()
{
  int     iNbDelais   = 0 ;
  double  dMeanDelai  = 0 ;

  if ((_dDelaiDebutConseille > 0) && (_sUniteDebutConseille != ""))
  {
    dMeanDelai += getDelaiInSecond(_dDelaiDebutConseille, _sUniteDebutConseille) ;
    iNbDelais++ ;
  }
  if ((_dDelaiDebutIdeal > 0) && (_sUniteDebutIdeal != ""))
  {
    dMeanDelai += getDelaiInSecond(_dDelaiDebutIdeal, _sUniteDebutIdeal) ;
    iNbDelais++ ;
  }
  if ((_dDelaiDebutIdealMax > 0) && (_sUniteDebutIdealMax != ""))
  {
    dMeanDelai += getDelaiInSecond(_dDelaiDebutIdealMax, _sUniteDebutIdealMax) ;
    iNbDelais++ ;
  }
  if ((_dDelaiDebutConseilMax > 0) && (_sUniteDebutConseilMax != ""))
  {
    dMeanDelai += getDelaiInSecond(_dDelaiDebutConseilMax, _sUniteDebutConseilMax) ;
    iNbDelais++ ;
  }

  if (iNbDelais > 0)
    return (dMeanDelai / double(iNbDelais)) ;
  else
    return 0 ;
}

double
NSLdvGoal::getDelaiInSecond(double iDelai, string sUnite)
{
  string sUnitConcept ;
  pContexte->getDico()->donneCodeSens(&sUnite, &sUnitConcept) ;

  if (sUnitConcept == "2SEC0")
    return iDelai ;
  if (sUnitConcept == "2MINU")
    return double(60) * iDelai ;
  if (sUnitConcept == "2HEUR")
    return double(3600) * iDelai ;
  if (sUnitConcept == "2DAT0")
    return double(86400) * iDelai ;
  if (sUnitConcept == "2DAT1")
    return getDelaiInSecond(double(7) * iDelai, "2DAT01") ;
  if (sUnitConcept == "2DAT2")
    return getDelaiInSecond(double(30) * iDelai, "2DAT01") ;
  if (sUnitConcept == "2DAT2")
    return getDelaiInSecond(double(365) * iDelai, "2DAT01") ;

  return double(0) ;
}

string
NSLdvGoal::getNodeDate(string sNode)
{
  string  sDocReference   = string(sNode, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;
  string  sNodeReference  = string(sNode, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN, strlen(sNode.c_str()) - (PAT_NSS_LEN + DOC_CODE_DOCUM_LEN)) ;
  string  sNodeDate       = string("") ;

  // Recherche du document qui contient ce noeud
  // Looking for the document that contains this node
  NSHISTODocument* pHistory = pContexte->getPatient()->getDocHis() ;
  if (NULL == pHistory)
    return string("") ;

  NSDocHistoArray* pVectDocument = pHistory->getVectDocument() ;
	if ((NULL == pVectDocument) || pVectDocument->empty())
    return string("") ;

  DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sDocReference) ;
  // Found
  if (NULL == iterDoc)
    return string("") ;

  // we save the document's date, but we try to search if there is a date in the
  // sons of the Value
  sNodeDate = (*iterDoc)->getDateDoc() ;

  // now we search in the sub-tree if we have a date
  PatPathoIter iterPat, iterEnd ;

  NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;

	if ((*iterDoc)->getCodeDocMeta() == sDocReference)
  {
    (*iterDoc)->initFromMeta(&PptMeta) ;

    iterPat = PptMeta.ChercherNoeud(sNode) ;
    iterEnd = PptMeta.end() ;
  }
  else
  {
    (*iterDoc)->initFromPatPatho(&PptMeta) ;

    iterPat = PptMeta.ChercherNoeud(sNode) ;
    iterEnd = PptMeta.end() ;
  }
  // the reseter can be anything... don't test ((*iterPat)->pDonnees->lexique == sLexique)
  if (iterPat && (iterPat != iterEnd))
  {
    int iColonne = (*iterPat)->getColonne() ;

    string sCodeLex = (*iterPat)->getLexique() ;
    string sType    = pContexte->getDico()->CodeCategorie(sCodeLex) ;

    string sDateLexSens = string("KOUVR") ;
    if      (string("V") == sType)
      sDateLexSens = string("KDARE") ;
    else if (string("G") == sType)
      sDateLexSens = string("KCHIR") ;

    iterPat++ ;

    while ((iterPat != iterEnd)  &&
                ((*iterPat)->getColonne() > iColonne))
    {
      bool bDateReference    = false ;
      int  iColDateReference = (*iterPat)->getColonne() ;

      string sNodeLex = (*iterPat)->getLexique() ;
      string sNodeSens ;
      pContexte->getDico()->donneCodeSens(&sNodeLex, &sNodeSens) ;

      if (sNodeSens == sDateLexSens)
      {
        bDateReference    = true ;
        iColDateReference = (*iterPat)->getColonne() ;
        iterPat++ ;
      }

      // bool bNextIsDate = false ;
      while (bDateReference && (iterPat != iterEnd) &&
                            ((*iterPat)->getColonne() > iColDateReference))
      {
        string sLexique = (*iterPat)->getLexique() ;

        if ((sLexique[0] == '£') &&
             ((sLexique[1] == dateMARK) || (sLexique[1] == dateHeureMARK)) &&
                    (((*iterPat)->getUnit())[0] == '2'))
        {
          sNodeDate = (*iterPat)->getComplement() ;
          break ;
        }
        iterPat++ ;
      }

      if (false == bDateReference)
        iterPat++ ;
    }
  }

  return (sNodeDate) ;
}

NSLdvGoalInfo::JALONSLEVELS
NSLdvGoal::getValueLevel(string sValue, string sUnit, NSLdvGoalInfo::JALONSLEVELS *iLastLevel)
{
	if (string("") == sValue)
		return NSLdvGoalInfo::None ;

  double dValue = StringToDouble(sValue) ;

  NSLdvGoalInfo::JALONSLEVELS iLevelValue = _aValueZones.getColor(dValue, sUnit) ;

  if (NSLdvGoalInfo::None != iLevelValue)
	{
  	(*iLastLevel) = iLevelValue ;
		return iLevelValue ;
	}

	if (NSLdvGoalInfo::None != (*iLastLevel))
    iLevelValue = (*iLastLevel) ;
  else
    iLevelValue = NSLdvGoalInfo::None ;

	(*iLastLevel) = iLevelValue ;
	return iLevelValue ;
}

bool
NSLdvGoal::getNodeValue(string sNode, string *sValue, double *dValue)
{
	(*sValue) = "" ;
	(*dValue) = 0 ;

	string sCategorie = pContexte->getDico()->CodeCategorie(_sLexique) ;
	if (string("V") != sCategorie)
		return false ;

	string  sDocReference   = string(sNode, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;
	string  sNodeReference  = string(sNode, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN, strlen(sNode.c_str()) - (PAT_NSS_LEN + DOC_CODE_DOCUM_LEN)) ;

	// Recherche du document qui contient ce noeud
	// Looking for the document that contains this node
	NSHISTODocument *pHistory = pContexte->getPatient()->getDocHis() ;
  if (NULL == pHistory)
    return false ;

  NSDocHistoArray* pVectDocument = pHistory->getVectDocument() ;
	if ((NULL == pVectDocument) || pVectDocument->empty())
  	return false ;

	DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sDocReference) ;

	// Found
	if (NULL == iterDoc)
		return false ;

  NSPatPathoArray IterPPT(pContexte->getSuperviseur()) ;
  (*iterDoc)->initFromPatPatho(&IterPPT) ;

  if (IterPPT.empty())
    return false ;

	// recherche de la valeur contenue dans ce noeud
	// Looking for the Value contained in this node
  //
	PatPathoIter iterPat = IterPPT.ChercherNoeud(sNode) ;
	if ((NULL == iterPat) || (IterPPT.end() == iterPat))
    return false ;

  if ((*iterPat)->getLexique() != _sLexique)
    return false ;

  int iColonne = (*iterPat)->getColonne() ;

  iterPat++ ;
  while ((IterPPT.end() != iterPat) && ((*iterPat)->getColonne() > iColonne))
  {
    // no need to use ifdef _MUE because we test the same thing in the two cases
    // the only one difference is that in _MUE version the Value is in the first son
    // and in the !_MUE version the Value is the son of the son (so in iterPat++)
    string sLexique = (*iterPat)->getLexique() ;
    if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
      (*sValue) = (*iterPat)->getComplement() ;
      (*dValue) = StringToDouble(*sValue) ;
      return true ;
    }

    iterPat++ ;
  }

	return false ;
}

string
NSLdvGoal::getNodeUnit(string sNode)
{
  string  sObjReference   = string(sNode, 0,                                PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;
  string  sNodeReference  = string(sNode, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN, strlen(sNode.c_str()) - (PAT_NSS_LEN + DOC_CODE_DOCUM_LEN)) ;

  // Recherche du document qui contient ce noeud
  // Looking for the document that contains this node
  NSHISTODocument *pHistory = pContexte->getPatient()->getDocHis() ;
  if (NULL == pHistory)
    return string("") ;

  NSDocHistoArray* pVectDocument = pHistory->getVectDocument() ;
	if ((NULL == pVectDocument) || pVectDocument->empty())
  	return string("") ;

  DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sObjReference) ;
  // Found
  if (NULL == iterDoc)
    return string("") ;

  NSPatPathoArray IterPPT(pContexte->getSuperviseur()) ;
  (*iterDoc)->initFromPatPatho(&IterPPT) ;

  if (IterPPT.empty())
    return string("") ;

  // recherche de l'unité contenue dans ce noeud
  // Looking for the Unit of the Value contained in this node
  //
  PatPathoIter iterPat = IterPPT.ChercherNoeud(sNode) ;
  if ((NULL == iterPat) || (IterPPT.end() == iterPat))
    return string("") ;

  if ((*iterPat)->getLexique() != _sLexique)
    return string("") ;

  int iColonne = (*iterPat)->getColonne() ;

  iterPat++ ;
  while ((IterPPT.end() != iterPat) && ((*iterPat)->getColonne() > iColonne))
  {
    string sLexique = (*iterPat)->getLexique() ;
    if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
      string  sUnite = "" ;
      string  sTemp = (*iterPat)->getUnit() ;
      pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
      return sUnite ;
    }
    iterPat++ ;
  }

  return string("") ;
}

void
NSLdvGoal::initPresetIntervals()
{
	initPresetDelayIntervals() ;
	initPresetDateIntervals() ;
	initPresetValueIntervals() ;
}

void
NSLdvGoal::initPresetDelayIntervals()
{
  NSLdvGoalInfo::JALONSLEVELS iUpColor   = NSLdvGoalInfo::None ;
  NSLdvGoalInfo::JALONSLEVELS iDownColor = NSLdvGoalInfo::None ;

  if ((_dDelaiDebutIdealMax >= 0) || (_dDelaiDebutIdeal >= 0))
  {
		iUpColor   = NSLdvGoalInfo::Bleu ;
		iDownColor = NSLdvGoalInfo::Bleu ;
  }

	// Delays
	//
	if (_dDelaiDebutIdealMax >= 0)
	{
		_aDelayZones.setHighValues(NSLdvGoalInfo::Bleu, _dDelaiDebutIdealMax, _sUniteDebutIdealMax, true) ;
    _aDelayZones.setLowValues(NSLdvGoalInfo::APvert, _dDelaiDebutIdealMax, _sUniteDebutIdealMax, false) ;

		iUpColor = NSLdvGoalInfo::APvert ;
	}
	if (_dDelaiDebutIdeal >= 0)
	{
  	_aDelayZones.setLowValues(NSLdvGoalInfo::Bleu, _dDelaiDebutIdeal, _sUniteDebutIdeal, true) ;
    _aDelayZones.setHighValues(NSLdvGoalInfo::AVvert, _dDelaiDebutIdeal, _sUniteDebutIdeal, false) ;

		iDownColor = NSLdvGoalInfo::AVvert ;
	}
	if (_dDelaiDebutConseilMax >= 0)
	{
		if (iUpColor == NSLdvGoalInfo::None)
    	iUpColor = NSLdvGoalInfo::APvert ;

		_aDelayZones.setHighValues(iUpColor, _dDelaiDebutConseilMax, _sUniteDebutConseilMax, true) ;
    _aDelayZones.setLowValues(NSLdvGoalInfo::APjaune, _dDelaiDebutConseilMax, _sUniteDebutConseilMax, false) ;

    iUpColor = NSLdvGoalInfo::APjaune ;
	}
	if (_dDelaiDebutConseille >= 0)
	{
  	if (iDownColor == NSLdvGoalInfo::None)
    	iDownColor = NSLdvGoalInfo::AVvert ;

    _aDelayZones.setLowValues(iDownColor, _dDelaiDebutConseille, _sUniteDebutConseille, true) ;
    _aDelayZones.setHighValues(NSLdvGoalInfo::AVjaune, _dDelaiDebutConseille, _sUniteDebutConseille, false) ;

    iDownColor = NSLdvGoalInfo::AVjaune ;
	}
	if (_dDelaiDebutCritique >= 0)
	{
  	if (iUpColor == NSLdvGoalInfo::None)
    	iUpColor = NSLdvGoalInfo::APjaune ;

    _aDelayZones.setHighValues(iUpColor, _dDelaiDebutCritique, _sUniteDebutCritique, true) ;
    _aDelayZones.setLowValues(NSLdvGoalInfo::AProuge, _dDelaiDebutCritique, _sUniteDebutCritique, false) ;

    iUpColor = NSLdvGoalInfo::AProuge ;
	}
	if (_dDelaiDebutAutorise >= 0)
	{
  	if (iDownColor == NSLdvGoalInfo::None)
    	iDownColor = NSLdvGoalInfo::AVjaune ;

    _aDelayZones.setLowValues(iDownColor, _dDelaiDebutAutorise, _sUniteDebutAutorise, true) ;
    _aDelayZones.setHighValues(NSLdvGoalInfo::AVrouge, _dDelaiDebutAutorise, _sUniteDebutAutorise, false) ;

    iDownColor = NSLdvGoalInfo::AVrouge ;
	}
}

void
NSLdvGoal::initPresetDateIntervals()
{
  NSLdvGoalInfo::JALONSLEVELS iUpColor   = NSLdvGoalInfo::None ;
  NSLdvGoalInfo::JALONSLEVELS iDownColor = NSLdvGoalInfo::None ;

  if ((string("") != _sDateDebutIdeal) || (string("") != _sDateDebutIdealMax))
  {
		iUpColor   = NSLdvGoalInfo::Bleu ;
		iDownColor = NSLdvGoalInfo::Bleu ;
  }

	// Delays
	//
	if (string("") != _sDateDebutIdealMax)
	{
  	_aDateZones.setHighValues(NSLdvGoalInfo::Bleu, _sDateDebutIdealMax, true) ;
    _aDateZones.setLowValues(NSLdvGoalInfo::APvert, _sDateDebutIdealMax, false) ;

		iUpColor = NSLdvGoalInfo::APvert ;
	}
	if (string("") != _sDateDebutIdeal)
	{
		_aDateZones.setLowValues(NSLdvGoalInfo::Bleu, _sDateDebutIdeal, true) ;
    _aDateZones.setHighValues(NSLdvGoalInfo::AVvert, _sDateDebutIdeal, false) ;

		iDownColor = NSLdvGoalInfo::AVvert ;
	}
	if (string("") != _sDateDebutConseilMax)
	{
  	if (iUpColor == NSLdvGoalInfo::None)
    	iUpColor = NSLdvGoalInfo::APvert ;

		_aDateZones.setHighValues(iUpColor, _sDateDebutConseilMax, true) ;
    _aDateZones.setLowValues(NSLdvGoalInfo::APjaune, _sDateDebutConseilMax, false) ;

    iUpColor = NSLdvGoalInfo::APjaune ;
	}
	if (string("") != _sDateDebutConseille)
	{
  	if (iDownColor == NSLdvGoalInfo::None)
    	iDownColor = NSLdvGoalInfo::AVvert ;

    _aDateZones.setLowValues(iDownColor, _sDateDebutConseille, true) ;
    _aDateZones.setHighValues(NSLdvGoalInfo::AVjaune, _sDateDebutConseille, false) ;

    iDownColor = NSLdvGoalInfo::AVjaune ;
	}
	if (string("") != _sDateDebutCritique)
	{
  	if (iUpColor == NSLdvGoalInfo::None)
    	iUpColor = NSLdvGoalInfo::APjaune ;

    _aDateZones.setHighValues(iUpColor, _sDateDebutCritique, true) ;
    _aDateZones.setLowValues(NSLdvGoalInfo::AProuge, _sDateDebutCritique, false) ;

    iUpColor = NSLdvGoalInfo::AProuge ;
	}
	if (string("") != _sDateDebutAutorise)
	{
  	if (iDownColor == NSLdvGoalInfo::None)
    	iDownColor = NSLdvGoalInfo::AVjaune ;

    _aDateZones.setLowValues(iDownColor, _sDateDebutAutorise, true) ;
    _aDateZones.setHighValues(NSLdvGoalInfo::AVrouge, _sDateDebutAutorise, false) ;

    iDownColor = NSLdvGoalInfo::AVrouge ;
	}
}

void
NSLdvGoal::initPresetValueIntervals()
{
  NSLdvGoalInfo::JALONSLEVELS iUpColor   = NSLdvGoalInfo::None ;
  NSLdvGoalInfo::JALONSLEVELS iDownColor = NSLdvGoalInfo::None ;

  if (_bValMaxIdeal || _bValMinIdeal)
  {
		iUpColor   = NSLdvGoalInfo::Bleu ;
		iDownColor = NSLdvGoalInfo::Bleu ;
  }

	// Delays
	//
	if (_bValMaxIdeal)
	{
  	_aValueZones.setHighValues(NSLdvGoalInfo::Bleu, _dValMaxIdeal, _sUniteValMaxIdeal, true) ;
    _aValueZones.setLowValues(NSLdvGoalInfo::APvert, _dValMaxIdeal, _sUniteValMaxIdeal, false) ;

		iUpColor = NSLdvGoalInfo::APvert ;
	}
	if (_bValMinIdeal)
	{
  	_aValueZones.setLowValues(NSLdvGoalInfo::Bleu, _dValMinIdeal, _sUniteValMinIdeal, true) ;
    _aValueZones.setHighValues(NSLdvGoalInfo::AVvert, _dValMinIdeal, _sUniteValMinIdeal, false) ;

		iDownColor = NSLdvGoalInfo::AVvert ;
	}
	if (_bValMaxConseille)
	{
  	if (iUpColor == NSLdvGoalInfo::None)
    	iUpColor = NSLdvGoalInfo::APvert ;

		_aValueZones.setHighValues(iUpColor, _dValMaxConseille, _sUniteValMaxConseille, true) ;
    _aValueZones.setLowValues(NSLdvGoalInfo::APjaune, _dValMaxConseille, _sUniteValMaxConseille, false) ;

    iUpColor = NSLdvGoalInfo::APjaune ;
	}
	if (_bValMinConseille)
	{
  	if (iDownColor == NSLdvGoalInfo::None)
    	iDownColor = NSLdvGoalInfo::AVvert ;

    _aValueZones.setLowValues(iDownColor, _dValMinConseille, _sUniteValMinConseille, true) ;
    _aValueZones.setHighValues(NSLdvGoalInfo::AVjaune, _dValMinConseille, _sUniteValMinConseille, false) ;

    iDownColor = NSLdvGoalInfo::AVjaune ;
	}
	if (_bValMaxAutorise)
	{
  	if (iUpColor == NSLdvGoalInfo::None)
    	iUpColor = NSLdvGoalInfo::APjaune ;

    _aValueZones.setHighValues(iUpColor, _dValMaxAutorise, _sUniteValMaxAutorise, true) ;
    _aValueZones.setLowValues(NSLdvGoalInfo::AProuge, _dValMaxAutorise, _sUniteValMaxAutorise, false) ;

    iUpColor = NSLdvGoalInfo::AProuge ;
	}
	if (_bValMinAutorise)
	{
  	if (iDownColor == NSLdvGoalInfo::None)
    	iDownColor = NSLdvGoalInfo::AVjaune ;

    _aValueZones.setLowValues(iDownColor, _dDelaiDebutAutorise, _sUniteDebutAutorise, true) ;
    _aValueZones.setHighValues(NSLdvGoalInfo::AVrouge, _dDelaiDebutAutorise, _sUniteDebutAutorise, false) ;

    iDownColor = NSLdvGoalInfo::AVrouge ;
	}
}

NSLdvGoalInfo::JALONSLEVELS
NSLdvGoal::getColorForPonctualDate(string sDate)
{
	return _aDateZones.getColor(sDate) ;
}

void
NSLdvGoal::getValuedGoalFromIndice(int iIndice, bool *pbIsGoal, double *pdGoalValue, string *psGoalUnit)
{
	switch (iIndice)
  {
  	case 0 :
    	*pbIsGoal    = _bValMinAutorise ;
      *pdGoalValue = _dValMinAutorise ;
      *psGoalUnit  = _sUniteValMinAutorise ;
      break ;
    case 1 :
    	*pbIsGoal    = _bValMinConseille ;
      *pdGoalValue = _dValMinConseille ;
      *psGoalUnit  = _sUniteValMinConseille ;
      break ;
    case 2 :
    	*pbIsGoal    = _bValMinIdeal ;
      *pdGoalValue = _dValMinIdeal ;
      *psGoalUnit  = _sUniteValMinIdeal ;
      break ;
    case 3 :
    	*pbIsGoal    = _bValMaxIdeal ;
      *pdGoalValue = _dValMaxIdeal ;
      *psGoalUnit  = _sUniteValMaxIdeal ;
      break ;
    case 4 :
    	*pbIsGoal    = _bValMaxConseille ;
      *pdGoalValue = _dValMaxConseille ;
      *psGoalUnit  = _sUniteValMaxConseille ;
      break ;
    case 5 :
    	*pbIsGoal    = _bValMaxAutorise ;
      *pdGoalValue = _dValMaxAutorise ;
      *psGoalUnit  = _sUniteValMaxAutorise ;
      break ;
    default :
    	*pbIsGoal    = false ;
      *pdGoalValue = double(0) ;
      *psGoalUnit  = string("") ;
      break ;
  }
}

bool
NSLdvGoal::canBeFullyConverted(string sUnit)
{
	bool   bIsDefined ;
  double dValue ;
  string sValueUnit ;

  // First, check if this unit is already goals' values unit
  //
  int i ;
	for (i = 0 ; i < 6; i++)
  {
  	getValuedGoalFromIndice(i, &bIsDefined, &dValue, &sValueUnit) ;
  	if (bIsDefined && (sValueUnit != sUnit))
    	break ;
  }
  if (i == 6)
		return true ;

  // Second, check if concepts are all alike
  //
  string sUnitSens ;
	pContexte->getDico()->donneCodeSens(&sUnit, &sUnitSens) ;

	for (i = 0 ; i < 6; i++)
  {
  	getValuedGoalFromIndice(i, &bIsDefined, &dValue, &sValueUnit) ;
  	if (bIsDefined)
    {
    	string sValueUnitSens ;
			pContexte->getDico()->donneCodeSens(&sValueUnit, &sValueUnitSens) ;

      if (sValueUnitSens != sUnitSens)
    		break ;
    }
  }
  if (i == 6)
		return true ;

	// If we are there, it means that units are really different, we must check
  // if we can convert from one to the other
  //

  NSCV NsCv(pContexte->getSuperviseur()) ;

  DBIResult result = NsCv.open() ;
	if (result != DBIERR_NONE)
    return false ;

	for (i = 0 ; i < 6; i++)
  {
  	getValuedGoalFromIndice(i, &bIsDefined, &dValue, &sValueUnit) ;
  	if (bIsDefined)
    {
    	bool bCvtSuccess = NsCv.ConvertirUnite(&dValue, sUnit, sValueUnit, _sLexique) ;
      if (!bCvtSuccess)
    		break ;
    }
  }

  NsCv.close() ;

  if (i == 6)
		return true ;

	return false ;
}

bool
NSLdvGoal::createXmlTree(NSLdvDocumentBase *pDocLdv, Ctree* pTreeObject, string sTargetLocalisation, bool bRemoveTitle)
{
  if (((NSLdvDocumentBase*) NULL == pDocLdv) || ((Ctree*) NULL == pTreeObject))
		return false ;

	// Setting the "localisation" attribute
  //
  if (string("") == sTargetLocalisation)
		sTargetLocalisation = string("ZPOMR/0OBJE") ;

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
	PatPathoIter iter = pDocLdv->donneGoal(this, &pPatho) ;
  // not found !
	if (((PatPathoIter) NULL == iter) || (pPatho->end() == iter))
  	return false ;

	NSPatPathoArray goalPatho(pContexte->getSuperviseur()) ;
  pPatho->ExtrairePatPathoWithRoot(iter, &goalPatho) ;

  // First step: remove referential Ids
  //
  PatPathoIter RefIdIter ;
  bool bIsID = goalPatho.CheminDansPatpatho(string("£RE"), string(1, cheminSeparationMARK), &RefIdIter) ;
  while (true == bIsID)
  {
    goalPatho.SupprimerItem(RefIdIter) ;
    bIsID = goalPatho.CheminDansPatpatho(string("£RE"), string(1, cheminSeparationMARK), &RefIdIter) ;
  }

  //
  // If asked, remove the title (for example, inside the referential, the title is in the header, not in the tree)
  //
  if (bRemoveTitle)
  {
    bool bIsID = goalPatho.CheminDansPatpatho(string("LNOMA"), string(1, cheminSeparationMARK), &RefIdIter) ;
    while (true == bIsID)
    {
      goalPatho.SupprimerItem(RefIdIter) ;
      bIsID = goalPatho.CheminDansPatpatho(string("LNOMA"), string(1, cheminSeparationMARK), &RefIdIter) ;
    }
  }

	int iCol = 0 ;

	// Find opening
  //
  PatPathoIter iter2 = goalPatho.begin() ;
  iter2++ ;
  PatPathoIter iterClose = 0 ;

  int iTotalDurationValueYears  = 0 ;
  int iTotalDurationValueMonths = 0 ;
  int iTotalDurationValueDays   = 0 ;

  while (goalPatho.end() != iter2)
  {
    if (iCol + 1 == (*iter2)->getColonne())
    {
    	string sSens = (*iter2)->getLexiqueSens() ;

      // Opening date : switch to "now"
      //
      if      (string("KOUVR") == sSens)
      {
      	iter2++ ;
        if (goalPatho.end() != iter2)
        {
        	sSens = (*iter2)->getLexiqueSens() ;
          if ((string("£D0") == sSens) || (string("£T0") == sSens))
          	(*iter2)->setComplement("$KAUJO+000000") ;
        }
      }
      else if (string("KFERM") == sSens)
      {
      	iter2++ ;
        if (goalPatho.end() != iter2)
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

        while ((goalPatho.end() != iter2) && ((*iter2)->getColonne() > iColPhase))
        {
        	sSens = (*iter2)->getLexiqueSens() ;

          // Opening date : switch to "now"
      		//
      		if      (string("KOUVR") == sSens)
      		{
      			iter2++ ;
        		if (goalPatho.end() != iter2)
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
        		if (goalPatho.end() != iter2)
        		{
        			sSens = (*iter2)->getLexiqueSens() ;
          		if ((string("£D0") == sSens) || (string("£T0") == sSens))
          			iterPhaseClose = iter2 ;
        		}
      		}
          else if (string("VDURE") == sSens)
      		{
      			iter2++ ;
        		if (goalPatho.end() != iter2)
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
    if (goalPatho.end() != iter2)
  		iter2++ ;
  }
  if (NULL != iterClose)
  {
  	string sDeltaTps = donne_deltaTemps(iTotalDurationValueYears, iTotalDurationValueMonths, iTotalDurationValueDays) ;
    (*iterClose)->setComplement(string("$KAUJO+") + sDeltaTps) ;
  }

	*(pTreeObject->pPatPathoArray) = goalPatho ;

	return pTreeObject->initFromPatPatho() ;
}

// -----------------------------------------------------------------------------
// NSLdvGoal(NSLdvGoal& rv)
// -----------------------------------------------------------------------------
NSLdvGoal::NSLdvGoal(const NSLdvGoal& rv)
          :NSRoot(rv.pContexte)
{
try
{
  _pDoc                    = rv._pDoc ;

	_sLexique                = rv._sLexique ;
	_sTitre                  = rv._sTitre ;
	_sConcern                = rv._sConcern ;
	_sReference              = rv._sReference ;
	_sCertitude              = rv._sCertitude ;
	_sComplementText         = rv._sComplementText ;

	_tDateOuverture 	       = rv._tDateOuverture ;
	_tDateFermeture          = rv._tDateFermeture ;
	_tOuvertLe               = rv._tOuvertLe ;
	_tFermeLe                = rv._tFermeLe ;
	_sOpenEventNode          = rv._sOpenEventNode ;
	_sCloseEventNode         = rv._sCloseEventNode ;

	_dValueOuverture         = rv._dValueOuverture ;
	_sValueOuverture         = rv._sValueOuverture ;
	_sUnitOuverture          = rv._sUnitOuverture ;
	_bValueOuverture         = rv._bValueOuverture ;
	_dValueFermeture         = rv._dValueFermeture ;
	_sValueFermeture         = rv._sValueFermeture ;
	_sUnitFermeture          = rv._sUnitFermeture ;
	_bValueFermeture         = rv._bValueFermeture ;

	_iRythme                 = rv._iRythme ;

  _iFrame                  = rv._iFrame ;

	_dDelaiDebutAutorise     = rv._dDelaiDebutAutorise ;
	_sUniteDebutAutorise     = rv._sUniteDebutAutorise ;
	_dDelaiDebutConseille    = rv._dDelaiDebutConseille ;
	_sUniteDebutConseille    = rv._sUniteDebutConseille ;
	_dDelaiDebutIdeal        = rv._dDelaiDebutIdeal ;
	_sUniteDebutIdeal        = rv._sUniteDebutIdeal ;
	_dDelaiDebutIdealMax     = rv._dDelaiDebutIdealMax ;
	_sUniteDebutIdealMax     = rv._sUniteDebutIdealMax ;
	_dDelaiDebutConseilMax   = rv._dDelaiDebutConseilMax ;
	_sUniteDebutConseilMax   = rv._sUniteDebutConseilMax ;
	_dDelaiDebutCritique     = rv._dDelaiDebutCritique ;
	_sUniteDebutCritique     = rv._sUniteDebutCritique ;

	_sDateDebutAutorise      = rv._sDateDebutAutorise ;
	_sDateDebutConseille     = rv._sDateDebutConseille ;
	_sDateDebutIdeal         = rv._sDateDebutIdeal ;
	_sDateDebutIdealMax      = rv._sDateDebutIdealMax ;
	_sDateDebutConseilMax    = rv._sDateDebutConseilMax ;
	_sDateDebutCritique      = rv._sDateDebutCritique ;

	_bValue                  = rv._bValue ;

	_bValMinAutorise         = rv._bValMinAutorise ;
	_dValMinAutorise         = rv._dValMinAutorise ;
	_sUniteValMinAutorise    = rv._sUniteValMinAutorise ;
	_bValMinConseille        = rv._bValMinConseille ;
	_dValMinConseille        = rv._dValMinConseille ;
	_sUniteValMinConseille   = rv._sUniteValMinConseille ;
	_bValMinIdeal            = rv._bValMinIdeal ;
	_dValMinIdeal            = rv._dValMinIdeal ;
	_sUniteValMinIdeal       = rv._sUniteValMinIdeal ;
	_bValMaxIdeal            = rv._bValMaxIdeal ;
	_dValMaxIdeal            = rv._dValMaxIdeal ;
	_sUniteValMaxIdeal       = rv._sUniteValMaxIdeal ;
	_bValMaxConseille        = rv._bValMaxConseille ;
	_dValMaxConseille        = rv._dValMaxConseille ;
	_sUniteValMaxConseille   = rv._sUniteValMaxConseille ;
	_bValMaxAutorise         = rv._bValMaxAutorise ;
	_dValMaxAutorise         = rv._dValMaxAutorise ;
	_sUniteValMaxAutorise    = rv._sUniteValMaxAutorise ;
	_isSelected							 = rv._isSelected ;

	_aJalons	               = rv._aJalons ;
	_aMetaJalons	           = rv._aMetaJalons ;

	_aDelayZones 						 = rv._aDelayZones ;
	_aDateZones              = rv._aDateZones ;
	_aValueZones             = rv._aValueZones ;

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception NSLdvGoal copy tor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// ~NSLdvGoal()
// -----------------------------------------------------------------------------

NSLdvGoal::~NSLdvGoal()
{
  lObjectCount-- ;
}

// -----------------------------------------------------------------------------
// NSLdvGoal&	operator=(NSLdvGoal& src)
// -----------------------------------------------------------------------------
NSLdvGoal&
NSLdvGoal::operator=(const NSLdvGoal& rv)
{
  if (this == &rv)
    return *this ;

try
{
  _pDoc                    = rv._pDoc ;

  _sLexique                = rv._sLexique ;
	_sTitre                  = rv._sTitre ;
  _sConcern                = rv._sConcern ;
	_sReference              = rv._sReference ;
  _sCertitude              = rv._sCertitude ;
  _sComplementText         = rv._sComplementText ;

	_tDateOuverture 	       = rv._tDateOuverture ;
	_tDateFermeture          = rv._tDateFermeture ;
  _tOuvertLe               = rv._tOuvertLe ;
  _tFermeLe                = rv._tFermeLe ;
  _sOpenEventNode          = rv._sOpenEventNode ;
  _sCloseEventNode         = rv._sCloseEventNode ;

  _dValueOuverture         = rv._dValueOuverture ;
  _sValueOuverture         = rv._sValueOuverture ;
  _sUnitOuverture          = rv._sUnitOuverture ;
  _bValueOuverture         = rv._bValueOuverture ;
  _dValueFermeture         = rv._dValueFermeture ;
  _sValueFermeture         = rv._sValueFermeture ;
  _sUnitFermeture          = rv._sUnitFermeture ;
  _bValueFermeture         = rv._bValueFermeture ;

  _iRythme                 = rv._iRythme ;

  _iFrame                  = rv._iFrame ;

  _dDelaiDebutAutorise     = rv._dDelaiDebutAutorise ;
  _sUniteDebutAutorise     = rv._sUniteDebutAutorise ;
  _dDelaiDebutConseille    = rv._dDelaiDebutConseille ;
  _sUniteDebutConseille    = rv._sUniteDebutConseille ;
  _dDelaiDebutIdeal        = rv._dDelaiDebutIdeal ;
  _sUniteDebutIdeal        = rv._sUniteDebutIdeal ;
  _dDelaiDebutIdealMax     = rv._dDelaiDebutIdealMax ;
  _sUniteDebutIdealMax     = rv._sUniteDebutIdealMax ;
  _dDelaiDebutConseilMax   = rv._dDelaiDebutConseilMax ;
  _sUniteDebutConseilMax   = rv._sUniteDebutConseilMax ;
  _dDelaiDebutCritique     = rv._dDelaiDebutCritique ;
  _sUniteDebutCritique     = rv._sUniteDebutCritique ;

  _sDateDebutAutorise      = rv._sDateDebutAutorise ;
  _sDateDebutConseille     = rv._sDateDebutConseille ;
  _sDateDebutIdeal         = rv._sDateDebutIdeal ;
  _sDateDebutIdealMax      = rv._sDateDebutIdealMax ;
  _sDateDebutConseilMax    = rv._sDateDebutConseilMax ;
  _sDateDebutCritique      = rv._sDateDebutCritique ;

  _bValue                  = rv._bValue ;

  _bValMinAutorise         = rv._bValMinAutorise ;
  _dValMinAutorise         = rv._dValMinAutorise ;
  _sUniteValMinAutorise    = rv._sUniteValMinAutorise ;
  _bValMinConseille        = rv._bValMinConseille ;
  _dValMinConseille        = rv._dValMinConseille ;
  _sUniteValMinConseille   = rv._sUniteValMinConseille ;
  _bValMinIdeal            = rv._bValMinIdeal ;
  _dValMinIdeal            = rv._dValMinIdeal ;
  _sUniteValMinIdeal       = rv._sUniteValMinIdeal ;
  _bValMaxIdeal            = rv._bValMaxIdeal ;
  _dValMaxIdeal            = rv._dValMaxIdeal ;
  _sUniteValMaxIdeal       = rv._sUniteValMaxIdeal ;
  _bValMaxConseille        = rv._bValMaxConseille ;
  _dValMaxConseille        = rv._dValMaxConseille ;
  _sUniteValMaxConseille   = rv._sUniteValMaxConseille ;
  _bValMaxAutorise         = rv._bValMaxAutorise ;
  _dValMaxAutorise         = rv._dValMaxAutorise ;
  _sUniteValMaxAutorise    = rv._sUniteValMaxAutorise ;

	_aJalons	               = rv._aJalons ;
	_aMetaJalons	           = rv._aMetaJalons ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSLdvGoal (=).", standardError, 0) ;
	return *this;
}
}

bool
NSLdvGoal::estIdentique(NSLdvGoal* pModel)
{
	if (NULL == pModel)
		return false ;

  if ((_sLexique               == pModel->_sLexique)                &&
      (_sCertitude             == pModel->_sCertitude)              &&
      (_sComplementText        == pModel->_sComplementText)         &&
      (_iRythme                == pModel->_iRythme)                 &&
      (_dDelaiDebutAutorise    == pModel->_dDelaiDebutAutorise)     &&
      (_sUniteDebutAutorise    == pModel->_sUniteDebutAutorise)     &&
      (_dDelaiDebutConseille   == pModel->_dDelaiDebutConseille)    &&
      (_sUniteDebutConseille   == pModel->_sUniteDebutConseille)    &&
      (_dDelaiDebutIdeal       == pModel->_dDelaiDebutIdeal)        &&
      (_sUniteDebutIdeal       == pModel->_sUniteDebutIdeal)        &&
      (_dDelaiDebutIdealMax    == pModel->_dDelaiDebutIdealMax)     &&
      (_sUniteDebutIdealMax    == pModel->_sUniteDebutIdealMax)     &&
      (_dDelaiDebutConseilMax  == pModel->_dDelaiDebutConseilMax)   &&
      (_sUniteDebutConseilMax  == pModel->_sUniteDebutConseilMax)   &&
      (_dDelaiDebutCritique    == pModel->_dDelaiDebutCritique)     &&
      (_sUniteDebutCritique    == pModel->_sUniteDebutCritique)     &&
      (_sDateDebutAutorise     == pModel->_sDateDebutAutorise)      &&
      (_sDateDebutConseille    == pModel->_sDateDebutConseille)     &&
      (_sDateDebutIdeal        == pModel->_sDateDebutIdeal)         &&
      (_sDateDebutIdealMax     == pModel->_sDateDebutIdealMax)      &&
      (_sDateDebutConseilMax   == pModel->_sDateDebutConseilMax)    &&
      (_sDateDebutCritique     == pModel->_sDateDebutCritique)      &&

      (_bValue                 == pModel->_bValue)                 &&
      (_iFrame                 == pModel->_iFrame)                 &&

      (_bValMinAutorise        == pModel->_bValMinAutorise)         &&
      (_dValMinAutorise        == pModel->_dValMinAutorise)         &&
      (_sUniteValMinAutorise   == pModel->_sUniteValMinAutorise)    &&
      (_bValMinConseille       == pModel->_bValMinConseille)        &&
      (_dValMinConseille       == pModel->_dValMinConseille)        &&
      (_sUniteValMinConseille  == pModel->_sUniteValMinConseille)   &&
      (_bValMinIdeal           == pModel->_bValMinIdeal)            &&
      (_dValMinIdeal           == pModel->_dValMinIdeal)            &&
      (_sUniteValMinIdeal      == pModel->_sUniteValMinIdeal)       &&
      (_bValMaxIdeal           == pModel->_bValMaxIdeal)            &&
      (_dValMaxIdeal           == pModel->_dValMaxIdeal)            &&
      (_sUniteValMaxIdeal      == pModel->_sUniteValMaxIdeal)       &&
      (_bValMaxConseille       == pModel->_bValMaxConseille)        &&
      (_dValMaxConseille       == pModel->_dValMaxConseille)        &&
      (_sUniteValMaxConseille  == pModel->_sUniteValMaxConseille)   &&
      (_bValMaxAutorise        == pModel->_bValMaxAutorise)         &&
      (_dValMaxAutorise        == pModel->_dValMaxAutorise)         &&
      (_sUniteValMaxAutorise   == pModel->_sUniteValMaxAutorise)    &&

      //(tDateOuverture       == pModel->tDateOuverture)          &&
      //(tDateFermeture       == pModel->tDateFermeture)          &&
      (_sOpenEventNode         == pModel->_sOpenEventNode)          &&
      (_sCloseEventNode        == pModel->_sCloseEventNode))
    return true ;

  return false ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de ArrayGoals
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// ArrayGoals(ArrayGoals& rv)
// -----------------------------------------------------------------------------
ArrayGoals::ArrayGoals(const ArrayGoals& rv)
           :VectGoal()
{
try
{
	if (false == rv.empty())
		for (ArrayGoalConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSLdvGoal(*(*i))) ;

	_pDoc = rv._pDoc ;
}
catch (...)
{
  erreur("Exception ArrayGoals copy ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// ~ArrayGoals()
// -----------------------------------------------------------------------------
ArrayGoals::~ArrayGoals()
{
	vider() ;
}

// -----------------------------------------------------------------------------
// void		vider()
// -----------------------------------------------------------------------------
void
ArrayGoals::vider()
{
	if (empty())
		return ;

	for (ArrayGoalIter i = begin() ; end() != i ; )
	{
    if (*i)
		  delete *i ;
		erase(i) ;
	}
}

// -----------------------------------------------------------------------------
// void		initialiser()
// -----------------------------------------------------------------------------
void
ArrayGoals::initialiser()
{
  // If this array is connected to a frame, then load it from frame's tree
  //
  if (_pFrame)
  {
    NSPatPathoArray FramePPT(_pDoc->pContexte->getSuperviseur()) ;
    _pFrame->initFromPatPathoIndex(&FramePPT) ;

    initGoalsFromPatho(&FramePPT, _pFrame->getFrame()) ;

    NSDocumentHistoArray* pVectDoc = _pFrame->getGoalDocs() ;
    if (((NSDocumentHistoArray*) NULL == pVectDoc) || pVectDoc->empty())
      return ;

    for (DocumentIter i = pVectDoc->begin() ; pVectDoc->end() != i ; i++)
      if (*i)
      {
        NSPatPathoArray DocPPT(_pDoc->pContexte->getSuperviseur()) ;
        (*i)->initFromPatPatho(&DocPPT) ;

	      initGoalsFromPatho(&DocPPT, _pFrame->getFrame()) ;
      }

    return ;
  }

  // If this array is not connected to a frame, we load goals from all frames
  //
  if (_pDoc)
  {
    for (int i = 0 ; i < FRAMECOUNT ; i++)
    {
      LDVFRAME iFrame = getFrameForIndex(i) ;
      NSFrameInformation *pFrame = _pDoc->getFrames()->getFrameInformation(iFrame) ;
      if (pFrame)
      {
        NSPatPathoArray FramePPT(_pDoc->pContexte->getSuperviseur()) ;
        pFrame->initFromPatPathoIndex(&FramePPT) ;

        initGoalsFromPatho(&FramePPT, pFrame->getFrame()) ;

        NSDocumentHistoArray* pVectDoc = pFrame->getGoalDocs() ;
        if ((NULL == pVectDoc) || pVectDoc->empty())
          return ;

        for (DocumentIter i = pVectDoc->begin() ; pVectDoc->end() != i ; i++)
          if (*i)
          {
            NSPatPathoArray DocPPT(_pDoc->pContexte->getSuperviseur()) ;
            (*i)->initFromPatPatho(&DocPPT) ;

	          initGoalsFromPatho(&DocPPT, pFrame->getFrame()) ;
          }
      }
    }
  }
}

/*
void
ArrayGoals::initHealthGoals()
{
  if (NULL == pDoc)
		return ;

	NSPatPathoArray *pPtIndex = pDoc->getPOMRIndex() ;
  if ((NULL == pPtIndex) || pPtIndex->empty())
    return ;

	initGoalsFromPatho(pPtIndex, ldvframeHealth) ;
}

void
ArrayGoals::initRiskGoals()
{
  if (NULL == pDoc)
		return ;

	NSPatPathoArray *pPtIndex = pDoc->getRiskIndex() ;
  if ((NULL == pPtIndex) || pPtIndex->empty())
    return ;

	initGoalsFromPatho(pPtIndex, ldvframeRisk) ;
}

void
ArrayGoals::initSocialGoals()
{
  if (NULL == pDoc)
		return ;

	NSPatPathoArray *pPtIndex = pDoc->getSocialIndex() ;
  if ((NULL == pPtIndex) || pPtIndex->empty())
    return ;

	initGoalsFromPatho(pPtIndex, ldvframeSocial) ;
}
*/

void
ArrayGoals::initGoalsFromPatho(NSPatPathoArray *pPtIndex, LDVFRAME iFrame)
{
  if ((NULL == pPtIndex) || pPtIndex->empty())
    return ;

	// Looking for the "Goals" chapter
  //
	PatPathoIter iter = pPtIndex->ChercherItem("0OBJE1") ;

	if ((NULL == iter) || (pPtIndex->end() == iter))
    return ;

  int iColBase = (*iter)->getColonne() ;

  iter++ ;
  if (pPtIndex->end() == iter)
    return ;

  loadGoals(iter, iColBase, pPtIndex, iFrame) ;
}

// -----------------------------------------------------------------------------
// void		loadGoals(PatPathoIter iter, int iColBase)
// -----------------------------------------------------------------------------
void
ArrayGoals::loadGoals(PatPathoIter iter, int iColBase, NSPatPathoArray* pPtIndex, LDVFRAME iFrame, bool bJustOne, bool bNewGoal)
{
try
{
  if ((NULL == pPtIndex) || pPtIndex->empty())
    return ;

	NSLdvGoal *pGoal = (NSLdvGoal*) 0 ;

	bool bTourner = true ;

	while ((bTourner) && (pPtIndex->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		// Problème
		if ((*iter)->getColonne() == iColBase + 1)
		{
    	if (bJustOne)
      	bTourner = false ;

			if (pGoal)
			{
				push_back(new NSLdvGoal(*pGoal)) ;
				delete pGoal ;
				pGoal = (NSLdvGoal*) 0 ;

				NSLdvGoal* pLatestGoal = back() ;
        pLatestGoal->init() ;
        pLatestGoal->initJalons() ;
        pLatestGoal->initConcern(_pDoc) ;
			}

      string sCodeLex = (*iter)->getLexique() ;
      string sCodeSensGoal ;
    	_pDoc->pContexte->getDico()->donneCodeSens(&sCodeLex, &sCodeSensGoal) ;

      //
      // Objectif caché -- Hidden health goal
      //
      if (sCodeSensGoal == "90000")
      {
      	iter++ ;
      	while ( (iter != pPtIndex->end()) &&
                ((*iter)->getColonne() > iColBase + 1))
        	iter++ ;
      }
      //
      // Objectif visible -- Visible health goal
      //
      else
      {
        pGoal = new NSLdvGoal(_pDoc->pContexte, _pDoc, iFrame) ;
        pGoal->initFromPatho(pPtIndex, &iter) ;

        // Cet objectif est-il raccordé à une préoccupation (nsconcern) ?
        // Is this goal attached to a health issue ?

        NSLinkManager* pGraphe = _pDoc->pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

        VecteurString VectString ;
        pGraphe->TousLesVrais(pGoal->getNode(), NSRootLink::problemGoals, &VectString) ;
        if (false == VectString.empty())
          pGoal->setConcern(*(*(VectString.begin()))) ;
  		}
		}
		else
			iter++ ;
	}

	if (pGoal)
	{
		push_back(new NSLdvGoal(*pGoal)) ;
		delete pGoal ;
		pGoal = (NSLdvGoal*) 0 ;

		NSLdvGoal* pLatestGoal = back() ;
		pLatestGoal->init() ;
		pLatestGoal->initJalons() ;
		pLatestGoal->initConcern(_pDoc) ;
	}
}
catch (...)
{
	erreur("Exception ArrayGoals::loadGoals.", standardError, 0) ;
	return ;
}
}

// -----------------------------------------------------------------------------
// void    reinit()
// -----------------------------------------------------------------------------
void
ArrayGoals::reinit()
{
  vider() ;

  initialiser() ;

/*
  if (NULL == pDoc)
		return ;

	NSPatPathoArray* pPtIndex = pDoc->getPOMRIndex() ;
  if ((NULL == pPtIndex) || pPtIndex->empty())
    return ;

	// On cherche le chapitre "objectifs de santé"
	PatPathoIter iter = pPtIndex->ChercherItem("0OBJE1") ;

	if ((iter != NULL) && (iter != pPtIndex->end()))
	{
		int iColBase = (*iter)->getColonne() ;
		iter++ ;

		vider() ;

		loadGoals(iter, iColBase) ;
	}
*/
}

// -----------------------------------------------------------------------------
//  Un nouvel objectif va être ajouté : on gère ses répercutions sur les objectifs
//  déjà en place
//
//  A new goal is to be added : we manage it's relationships with other goals
// -----------------------------------------------------------------------------
void
ArrayGoals::initNewGoal(NSLdvGoal *pNewGoal)
{
try
{
  if ((empty()) || (NULL == pNewGoal))
    return ;

  NSSuper *pSuper = _pDoc->pContexte->getSuperviseur() ;

  string ps = string("Entering ArrayGoals::initNewGoal for goal \"") + pNewGoal->getTitle() + string("\"") ;
  pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;

  // Goal that never opens
  // Objectif qui ne s'ouvre jamais
  //
  if ((string("") == pNewGoal->_sOpenEventNode) &&
                                (pNewGoal->_tDateOuverture.estVide()))
  {
    ps = string("Leaving ArrayGoals::initNewGoal (never opening goal).") ;
    pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;
    return ;
  }

  //
  // Are opening conditions fulfiled?
  // On regarde si les conditions d'ouverture sont réalisées
  //
  NVLdVTemps tNow ;
  tNow.takeTime() ;

  // Closing date already in the past, leaving
  // Date de fermeture déjà atteinte : on sort
  //
  if ((false == pNewGoal->_tDateFermeture.estVide()) &&
                                          (pNewGoal->_tDateFermeture < tNow))
  {
    ps = string("Leaving ArrayGoals::initNewGoal (goal opening date in past).") ;
    pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;
    return ;
  }

  string sNewConcept ;
  _pDoc->pContexte->getDico()->donneCodeSens(&(pNewGoal->getLexique()), &sNewConcept) ;

  NSPatientChoisi* pPatient = _pDoc->pContexte->getPatient() ;
  if (NULL == pPatient)
  {
    ps = string("Leaving ArrayGoals::initNewGoal (no patient).") ;
    pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;
    return ;
  }

  NSHISTODocument* pHistory = pPatient->getDocHis() ;

  // Is there already an opening event to take into account?
  // On cherche s'il existe un événement
  //
  if (string("") != pNewGoal->_sOpenEventNode)
  {
    if ((NULL == pHistory) || (pHistory->getVectDocument()->empty()))
    {
      ps = string("Leaving ArrayGoals::initNewGoal (empty history).") ;
      pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;
      return ;
    }

    string      sGoalOpener = string("") ;
    NVLdVTemps  tDateDoc ;
    tDateDoc.init() ;

    DocumentIter iterDoc = pHistory->getVectDocument()->begin() ;
		while (pHistory->getVectDocument()->end() != iterDoc)
		{
      NSPatPathoInfo* pRootPathoInfo = (*iterDoc)->getRootPathoInfo() ;
      if (pRootPathoInfo)
      {
        string sRootConcept ;
        string sLexique = pRootPathoInfo->getLexique() ;
        _pDoc->pContexte->getDico()->donneCodeSens(&sLexique, &sRootConcept) ;

        if (sRootConcept == sNewConcept)
        {
          string sDate = (*iterDoc)->getDateDoc() ;
          if (string("") != sDate)
          {
            NVLdVTemps tDateOfDoc ;
            tDateOfDoc.initFromDate(sDate) ;
            if ((tDateOfDoc > tDateDoc) && (tDateOfDoc > pNewGoal->_tDateOuverture))
            {
              tDateDoc    = tDateOfDoc ;
              sGoalOpener = pRootPathoInfo->getNode() ;
            }
          }
        }
      }
      iterDoc++ ;
		}

    if (string("") != sGoalOpener)
    {
      NSLinkManager* pGraphe = pPatient->getGraphPerson()->getLinkManager() ;

      pGraphe->etablirLien(sGoalOpener, NSRootLink::goalOpener, pNewGoal->getNode()) ;
      pNewGoal->_tOuvertLe = tDateDoc ;
    }
    else
    {
      ps = string("Leaving ArrayGoals::initNewGoal (no opening event found).") ;
      pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;
      return ;
    }
  }
  else
  {
    if (pNewGoal->_tDateOuverture <= tNow)
    {
      ps = string("Leaving ArrayGoals::initNewGoal: opening event found and goal now considered opened.") ;
      pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;
      pNewGoal->_tOuvertLe = pNewGoal->_tDateOuverture ;
    }
  }

  //
  // On regarde si d'autres objectifs concernent le même concept
  //
  for (ArrayGoalIter itGoal = begin() ; end() != itGoal ; itGoal++)
	{
    string sPreviousConcept ;
    _pDoc->pContexte->getDico()->donneCodeSens(&((*itGoal)->getLexique()), &sPreviousConcept) ;

    if (sPreviousConcept == sNewConcept)
    {
      // Le nouvel objectif est cyclique
      if (NSLdvGoal::cyclic == pNewGoal->_iRythme)
      {
        // Si l'ancien est cyclique, le plus fréquent cache l'autre
        if (NSLdvGoal::cyclic == (*itGoal)->_iRythme)
        {
          double dNewDelai = pNewGoal->getMeanDelaiInSecond() ;
          double dOldDelai = (*itGoal)->getMeanDelaiInSecond() ;
          // Nouveau moins fréquent : on ne le garde que s'il
          // démarre plus tôt
          if (dNewDelai >= dOldDelai)
          {
            // if ()
          }
        }
      }
    }
  }
  ps = string("Leaving ArrayGoals::initNewGoal.") ;
  pSuper->trace(&ps, 1, NSSuper::trSubSteps) ;
}
catch (...)
{
	erreur("Exception ArrayGoals::initNewGoal", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// void   	reloadGoals(PatPathoIter iter, int iColBase)
// -----------------------------------------------------------------------------
void
ArrayGoals::reloadGoals(PatPathoIter iter, int iColBase, NSPatPathoArray* pPtIndex)
{
try
{
  if (NULL == _pFrame)
		return ;

  if ((NULL == pPtIndex) || pPtIndex->empty())
    return ;

	NSLdvGoal *pGoal = (NSLdvGoal*) 0 ;

	while ((pPtIndex->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		// Problème
		if ((*iter)->getColonne() == iColBase + 1)
		{
			if (pGoal)
			{
				push_back(new NSLdvGoal(*pGoal)) ;
				delete pGoal ;
				pGoal = 0 ;
			}

      string sCodeLex = (*iter)->getLexique() ;
      string sCodeSensGoal ;
    	_pFrame->pContexte->getDico()->donneCodeSens(&sCodeLex, &sCodeSensGoal) ;

      //
      // Objectif caché -- Hidden health goal
      //
      if (sCodeSensGoal == "90000")
      {
      	iter++ ;
      	while ( (iter != pPtIndex->end()) &&
                ((*iter)->getColonne() > iColBase + 1))
        	iter++ ;
      }
      //
      // Objectif visible -- Visible health goal
      //
      else
      {
        pGoal = new NSLdvGoal(_pDoc->pContexte, _pDoc, _pFrame->getFrame()) ;

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // bidouille pour que ca marche
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        pGoal->_tDateFermeture.setNoLimit() ;

        // Libellé
        pGoal->setLexique(sCodeLex) ;

        if (string("£?????") != sCodeLex)
        {
          string sTitle = string("") ;
          _pFrame->pContexte->getDico()->donneLibelle(_pDoc->_sLangue, &sCodeLex, &sTitle) ;
          pGoal->setTitle(sTitle) ;
        }
        // Texte libre - Free text
        else
        	pGoal->setTitle((*iter)->getTexteLibre()) ;

        // Noeud
        string sNoeud = (*iter)->getNode() ;
        pGoal->setNode(sNoeud) ;

        iter++ ;

        // Paramètres d'objectif
        while ((iter != pPtIndex->end()) && ((*iter)->getColonne() > iColBase + 1))
        {
          string sElemLex = (*iter)->getLexique() ;
          string sSens ;
          _pFrame->pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

          if ((*iter)->getColonne() == iColBase + 2)
          {
            // Dates d'ouverture et de fermeture
            if ((sSens == "KOUVR") || (sSens == "KFERM"))
            {
              iter++;
              int iLigneBase = (*iter)->getLigne() ;
              // gereDate* pDate = new gereDate(pContexte) ;
              string sUnite  = "" ;
              string sFormat = "" ;
              string sValeur = "" ;
              string sTemp   = "" ;
              while ((pPtIndex->end() != iter) && ((*iter)->getLigne() == iLigneBase))
              {
                string sLexique = (*iter)->getLexique() ;
                if ((string("") != sLexique) && ('£' == sLexique[0]))
                {
                  sTemp   = (*iter)->getLexique() ;
                  _pFrame->pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
                  sValeur = (*iter)->getComplement() ;
                  sTemp   = (*iter)->getUnit() ;
                  _pFrame->pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
                  break ;
                }
                iter++ ;
              }

              // sFormat est du type £D0;03
              if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) && (sValeur != ""))
              {
                if ((sUnite == "2DA01") || (sUnite == "2DA02"))
                {
                  if (string("KOUVR") == sSens)
                    pGoal->_tDateOuverture.initFromDate(sValeur) ;
                  else
                    if (string("KFERM") == sSens)
                      pGoal->_tDateFermeture.initFromDate(sValeur) ;
                }
              }
            }
            else
              iter++ ;
          }
          else
            iter++ ;
        }

        /*
        // Cette préoccupation est-elle l'évolution d'une autre ?
        // Is this health concern the evolution of a previous one ?
        VecteurString   *pVecteurString = new VecteurString() ;
        pContexte->getGraphe()->TousLesVrais(sNoeud, NSGraphe::problemRelatedTo, pVecteurString) ;
        if (!pVecteurString->empty())
          pConcern->sPrimoPb =  *(*(pVecteurString->begin())) ;
        delete pVecteurString ;
        */
  		}
		}
		else
			iter++ ;
	}

	if (pGoal)
	{
		push_back(new NSLdvGoal(*pGoal)) ;
		delete pGoal ;
		pGoal = 0 ;
	}
}
catch (...)
{
	erreur("Exception ArrayGoals::reloadConcerns.", standardError, 0) ;
	return ;
}
}

// -----------------------------------------------------------------------------
// void    deleteGoal(NSLdvGoal *pGoal)
// -----------------------------------------------------------------------------
void
ArrayGoals::deleteGoal(NSLdvGoal *pGoal)
{
	if ((NULL == pGoal) || empty())
		return ;

	for (ArrayGoalIter i = begin() ; end() != i ; i++)
	{
		if (*i == pGoal)
		{
			delete pGoal ;
			erase(i) ;
			return ;
		}
	}
}

GoalInfoArray*
ArrayGoals::giveBaddestJalons(string sConcern, NVLdVRect* pRect)
{
  if (empty())
    return (GoalInfoArray*) 0 ;

  GoalInfoArray* pGIarray = new GoalInfoArray() ;

  //
  // Etape 1 :    On entasse tous les jalons de tous les objectifs
  //              de cette préoccupation qui sont en partie dans le rectangle
  //
  for (ArrayGoalIter i = begin() ; end() != i ; i++)
  {
		if ((*i)->getConcern() == sConcern)
    {
      GoalInfoArray* pJalons = (*i)->getMetaJalons() ;
      if (pJalons && (false == pJalons->empty()))
      {
        GoalInfoIter itGoals = pJalons->begin() ;
        for ( ; (pJalons->end() != itGoals) && ((*itGoals)->tpsInfo < pRect->getRight()) ; itGoals++)
          if ((*itGoals)->tpsClosed > pRect->getLeft())
            pGIarray->push_back(new NSLdvGoalInfo(**itGoals)) ;
      }
    }
  }

  if (pGIarray->empty())
  {
    delete pGIarray ;
    return (GoalInfoArray*) 0 ;
  }

  //
  // Etape 2 : On trie par ordre chronologique
  //
  sort(pGIarray->begin(), pGIarray->end(), infGoalInfo) ;

  //
  // Etape 3 : On ne garde que les pires pour une période donnée
  //
  int        iLevel = 0 ;
  NVLdVTemps tpsEndLevel = pRect->getLeft() ;

  for (GoalInfoIter i = pGIarray->begin() ; pGIarray->end() != i ; )
  {
    if (((*i)->iLevel > iLevel) || ((*i)->tpsInfo > tpsEndLevel))
    {
      iLevel      = (*i)->iLevel ;
      tpsEndLevel = (*i)->tpsClosed ;
      i++ ;
    }
    else
    {
      delete *i ;
      pGIarray->erase(i) ;
    }
  }

  return pGIarray ;
}

// -----------------------------------------------------------------------------
// NSLdvGoal		*getGoal(string sRef)
// -----------------------------------------------------------------------------
NSLdvGoal*
ArrayGoals::getGoal(string sRef)
{
	if (empty())
		return (NSLdvGoal*) 0 ;

	for (ArrayGoalIter i = begin() ; end() != i ; i++)
		if ((*i)->getNode() == sRef)
			return *i ;

	return (NSLdvGoal*) 0 ;
}

// -----------------------------------------------------------------------------
// NSLdvGoal		*makeBaddestProjection()
// -----------------------------------------------------------------------------
// cette fonction créé un NSLdvGoal qui est la projection de tous les objectifs
// contenu dans l'ArrayGoals qui reflète la pire situation à chaque instant.
// -----------------------------------------------------------------------------

NSLdvGoal*
ArrayGoals::makeBaddestProjection()
{
	if (empty())
		return NULL ;

	// pour l'instant on renvoie le premier, mais il faudra renvoyer un nouvel
	// objectif qui reflétera le pire état de chaque objectif

	for (ArrayGoalIter i = begin() ; end() != i ; i++)
		return (*i) ;

	return (NSLdvGoal*) 0 ;
}

// -----------------------------------------------------------------------------
// ArrayGoals& operator=(ArrayGoals src)
// -----------------------------------------------------------------------------
ArrayGoals&
ArrayGoals::operator=(const ArrayGoals& src)
{
try
{
  if (this == &src)
  	return *this ;

	vider() ;

	if (false == src.empty())
		for (ArrayGoalConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSLdvGoal(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayGoals (=).", standardError, 0) ;
	return *this ;
}
}

///////////////////////////// NSDelayZone

// Constructor
NSDelayZone::NSDelayZone(NSLdvGoalInfo::JALONSLEVELS iColour, double dLow, string sLow, bool bLow, double dHigh, string sHigh, bool bHigh)
{
	dLowDelay      = dLow ;
	sLowDelayUnit  = sLow ;
	bLowIncluded   = bLow ;

	dHighDelay     = dHigh ;
	sHighDelayUnit = sHigh ;
	bHighIncluded  = bHigh ;

	iLevel         = iColour ;

  lObjectCount++ ;
}

// Copy constructor
NSDelayZone::NSDelayZone(const NSDelayZone& rv)
{
	dLowDelay      = rv.dLowDelay ;
	sLowDelayUnit  = rv.sLowDelayUnit ;
	bLowIncluded   = rv.bLowIncluded ;

	dHighDelay     = rv.dHighDelay ;
	sHighDelayUnit = rv.sHighDelayUnit ;
	bHighIncluded  = rv.bHighIncluded ;

	iLevel         = rv.iLevel ;

  lObjectCount++ ;
}

// destructor
NSDelayZone::~NSDelayZone()
{
  lObjectCount-- ;
}

// is this delay inside this zone
bool
NSDelayZone::isInside(double dValue, string sUnit)
{
	if (dLowDelay >= 0)
	{
		if (sUnit != sLowDelayUnit)
			return false ;
    if (bLowIncluded && (dValue < dLowDelay))
			return false ;
		if (!bLowIncluded && (dValue <= dLowDelay))
			return false ;
	}
	if (dHighDelay >= 0)
	{
		if (sUnit != sHighDelayUnit)
			return false ;
    if (bHighIncluded && (dValue > dHighDelay))
			return false ;
		if (!bLowIncluded && (dValue >= dHighDelay))
			return false ;
	}
  return true ;
}

// = operator
NSDelayZone&
NSDelayZone::operator=(const NSDelayZone& src)
{
  if (this == &src)
  	return *this ;

	dLowDelay      = src.dLowDelay ;
	sLowDelayUnit  = src.sLowDelayUnit ;
	bLowIncluded   = src.bLowIncluded ;

	dHighDelay     = src.dHighDelay ;
	sHighDelayUnit = src.sHighDelayUnit ;
	bHighIncluded  = src.bHighIncluded ;

	iLevel         = src.iLevel ;

  return *this ;
}

///////////////////////////// NSDelayZoneArray

// Constructor
NSDelayZoneArray::NSDelayZoneArray()
                 :NSDelayZoneVector()
{
}

// Copy constructor
NSDelayZoneArray::NSDelayZoneArray(const NSDelayZoneArray& rv)
{
try
{
	if (false == rv.empty())
		for (NSDelayZoneConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSDelayZone(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSDelayZoneArray copy ctor.", standardError, 0) ;
}
}

// Destructor
NSDelayZoneArray::~NSDelayZoneArray()
{
	vider() ;
}

NSLdvGoalInfo::JALONSLEVELS
NSDelayZoneArray::getColor(double dValue, string sUnit)
{
	if (false == empty())
		for (NSDelayZoneIter i = begin() ; end() != i ; i++)
			if ((*i)->isInside(dValue, sUnit))
				return (*i)->iLevel ;

	return NSLdvGoalInfo::None ;
}

void
NSDelayZoneArray::setLowValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, double dLow, string sLow, bool bLow)
{
	NSDelayZone* pZone = getZone(iColorZone) ;
	if (pZone)
	{
		pZone->dLowDelay     = dLow ;
		pZone->sLowDelayUnit = sLow ;
		pZone->bLowIncluded  = bLow ;
    return ;
	}

	push_back(new NSDelayZone(iColorZone, dLow, sLow, bLow, -1, "", false)) ;
}

void
NSDelayZoneArray::setHighValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, double dHigh, string sHigh, bool bHigh)
{
	NSDelayZone* pZone = getZone(iColorZone) ;
	if (pZone)
	{
		pZone->dHighDelay     = dHigh ;
		pZone->sHighDelayUnit = sHigh ;
		pZone->bHighIncluded  = bHigh ;
    return ;
	}

	push_back(new NSDelayZone(iColorZone, -1, "", false, dHigh, sHigh, bHigh)) ;
}

NSDelayZone*
NSDelayZoneArray::getZone(NSLdvGoalInfo::JALONSLEVELS iColorZone)
{
	if (false == empty())
		for (NSDelayZoneIter i = begin() ; end() != i ; i++)
			if ((*i)->iLevel == iColorZone)
				return *i ;

	return (NSDelayZone*) 0 ;
}

// = operator
NSDelayZoneArray&
NSDelayZoneArray::operator=(const NSDelayZoneArray& src)
{
try
{
  if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (NSDelayZoneConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSDelayZone(**i)) ;

	return (*this) ;
}
catch (...)
{
	erreur("Exception NSDelayZoneArray (=).", standardError, 0) ;
	return *this;
}
}

// Empty the array
void
NSDelayZoneArray::vider()
{
	if (empty())
		return ;

	for (NSDelayZoneIter i = begin() ; end() != i ; )
	{
    if (*i)
		  delete (*i) ;
		erase(i) ;
	}
}

///////////////////////////// NSDateZone

// Constructor
NSDateZone::NSDateZone(NSLdvGoalInfo::JALONSLEVELS iColour, string sLow, bool bLow, string sHigh, bool bHigh)
{
	sLowDate       = sLow ;
	bLowIncluded   = bLow ;

	sHighDate      = sHigh ;
	bHighIncluded  = bHigh ;

	iLevel         = iColour ;

  lObjectCount++ ;
}

// Copy constructor
NSDateZone::NSDateZone(const NSDateZone& rv)
{
	sLowDate       = rv.sLowDate ;
	bLowIncluded   = rv.bLowIncluded ;

	sHighDate      = rv.sHighDate ;
	bHighIncluded  = rv.bHighIncluded ;

	iLevel         = rv.iLevel ;

  lObjectCount++ ;
}

// destructor
NSDateZone::~NSDateZone()
{
  lObjectCount-- ;
}

// is this delay inside this zone
bool
NSDateZone::isInside(string sDate)
{
	if (sLowDate != "")
	{
    if (bLowIncluded && (sDate < sLowDate))
			return false ;
		if (!bLowIncluded && (sDate <= sLowDate))
			return false ;
	}
	if (sHighDate != "")
	{
    if (bHighIncluded && (sDate > sHighDate))
			return false ;
		if (!bLowIncluded && (sDate >= sHighDate))
			return false ;
	}
  return true ;
}

NVLdVTemps
NSDateZone::getLowDate()
{
	NVLdVTemps tps ;
	if (sLowDate != "")
  	tps.initFromDate(sLowDate) ;
  else
  	tps.init() ;
  return tps ;
}

NVLdVTemps
NSDateZone::getHighDate()
{
	NVLdVTemps tps ;
	if (sLowDate != "")
		tps.initFromDate(sHighDate) ;
	else
		tps.setNoLimit() ;
  return tps ;
}

// = operator
NSDateZone&
NSDateZone::operator=(const NSDateZone& src)
{
  if (this == &src)
		return *this ;

	sLowDate       = src.sLowDate ;
	bLowIncluded   = src.bLowIncluded ;

	sHighDate      = src.sHighDate ;
	bHighIncluded  = src.bHighIncluded ;

	iLevel         = src.iLevel ;

  return *this ;
}

///////////////////////////// NSDelayZoneArray

// Constructor
NSDateZoneArray::NSDateZoneArray()
                :NSDateZoneVector()
{
}

// Copy constructor
NSDateZoneArray::NSDateZoneArray(const NSDateZoneArray& rv)
{
try
{
	if (false == rv.empty())
		for (NSDateZoneConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSDateZone(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSDateZoneArray copy ctor.", standardError, 0) ;
}
}

// Destructor
NSDateZoneArray::~NSDateZoneArray()
{
	vider() ;
}

NSLdvGoalInfo::JALONSLEVELS
NSDateZoneArray::getColor(string sDate)
{
	if (!(empty()))
		for (NSDateZoneIter i = begin() ; i != end() ; i++)
			if ((*i)->isInside(sDate))
				return (*i)->iLevel ;

	return NSLdvGoalInfo::None ;
}

void
NSDateZoneArray::setLowValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, string sLow, bool bLow)
{
	NSDateZone* pZone = getZone(iColorZone) ;
	if (pZone)
	{
		pZone->sLowDate      = sLow ;
		pZone->bLowIncluded  = bLow ;
    return ;
	}

	push_back(new NSDateZone(iColorZone, sLow, bLow, "", false)) ;
}

void
NSDateZoneArray::setHighValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, string sHigh, bool bHigh)
{
	NSDateZone* pZone = getZone(iColorZone) ;
	if (pZone)
	{
		pZone->sHighDate      = sHigh ;
		pZone->bHighIncluded  = bHigh ;
    return ;
	}

	push_back(new NSDateZone(iColorZone, "", false, sHigh, bHigh)) ;
}

NSDateZone*
NSDateZoneArray::getZone(NSLdvGoalInfo::JALONSLEVELS iColorZone)
{
	if (false == empty())
		for (NSDateZoneIter i = begin() ; i != end() ; i++)
			if ((*i)->iLevel == iColorZone)
				return *i ;

	return NULL ;
}

// = operator
NSDateZoneArray&
NSDateZoneArray::operator=(const NSDateZoneArray& src)
{
  if (this == &src)
		return *this ;

try
{
	vider() ;

	if (false == src.empty())
		for (NSDateZoneConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSDateZone(**i)) ;

	return (*this) ;
}
catch (...)
{
	erreur("Exception NSDateZoneArray (=).", standardError, 0) ;
	return *this;
}
}

// Empty the array
void
NSDateZoneArray::vider()
{
	if (empty())
		return ;

	for (NSDateZoneIter i = begin() ; end() != i ; )
	{
		delete (*i) ;
		erase(i) ;
	}
}

///////////////////////////// NSValueZone

// Constructor
NSValueZone::NSValueZone(NSLdvGoalInfo::JALONSLEVELS iColour, bool bIsLow, double dLow, string sLow, bool bLow, bool bIsHigh, double dHigh, string sHigh, bool bHigh)
{
	bLowExist      = bIsLow ;
	dLowValue      = dLow ;
	sLowValueUnit  = sLow ;
	bLowIncluded   = bLow ;

	bHighExist     = bIsHigh ;
	dHighValue     = dHigh ;
	sHighValueUnit = sHigh ;
	bHighIncluded  = bHigh ;

	iLevel         = iColour ;

  lObjectCount++ ;
}

// Copy constructor
NSValueZone::NSValueZone(const NSValueZone& rv)
{
	bLowExist      = rv.bLowExist ;
	dLowValue      = rv.dLowValue ;
	sLowValueUnit  = rv.sLowValueUnit ;
	bLowIncluded   = rv.bLowIncluded ;

	bHighExist     = rv.bHighExist ;
	dHighValue     = rv.dHighValue ;
	sHighValueUnit = rv.sHighValueUnit ;
	bHighIncluded  = rv.bHighIncluded ;

	iLevel         = rv.iLevel ;

  lObjectCount++ ;
}

// destructor
NSValueZone::~NSValueZone()
{
  lObjectCount-- ;
}

// is this delay inside this zone
bool
NSValueZone::isInside(double dValue, string sUnit)
{
	if (bLowExist)
	{
		if (sUnit != sLowValueUnit)
			return false ;
    if (bLowIncluded && (dValue < dLowValue))
			return false ;
		if (!bLowIncluded && (dValue <= dLowValue))
			return false ;
	}
	if (bHighExist)
	{
		if (sUnit != sHighValueUnit)
			return false ;
    if (bHighIncluded && (dValue > dHighValue))
			return false ;
		if (!bLowIncluded && (dValue >= dHighValue))
			return false ;
	}
  return true ;
}

// = operator
NSValueZone&
NSValueZone::operator=(const NSValueZone& src)
{
  if (this == &src)
		return *this ;

  bLowExist      = src.bLowExist ;
	dLowValue      = src.dLowValue ;
	sLowValueUnit  = src.sLowValueUnit ;
	bLowIncluded   = src.bLowIncluded ;

	bHighExist     = src.bHighExist ;
	dHighValue     = src.dHighValue ;
	sHighValueUnit = src.sHighValueUnit ;
	bHighIncluded  = src.bHighIncluded ;

	iLevel         = src.iLevel ;

  return *this ;
}

///////////////////////////// NSValueZoneArray

// Constructor
NSValueZoneArray::NSValueZoneArray()
                 :NSValueZoneVector()
{
}

// Copy constructor
NSValueZoneArray::NSValueZoneArray(const NSValueZoneArray& rv)
{
try
{
	if (false == rv.empty())
		for (NSValueZoneConstIter i = rv.begin() ; i != rv.end() ; i++)
			push_back(new NSValueZone(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSValueZoneArray copy ctor.", standardError, 0) ;
}
}

// Destructor
NSValueZoneArray::~NSValueZoneArray()
{
	vider() ;
}

NSLdvGoalInfo::JALONSLEVELS
NSValueZoneArray::getColor(double dValue, string sUnit)
{
	if (!(empty()))
		for (NSValueZoneIter i = begin() ; i != end() ; i++)
			if ((*i)->isInside(dValue, sUnit))
				return (*i)->iLevel ;

	return NSLdvGoalInfo::None ;
}

void
NSValueZoneArray::setLowValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, double dLow, string sLow, bool bLow)
{
	NSValueZone* pZone = getZone(iColorZone) ;
	if (pZone)
	{
  	pZone->bLowExist     = true ;
		pZone->dLowValue     = dLow ;
		pZone->sLowValueUnit = sLow ;
		pZone->bLowIncluded  = bLow ;
    return ;
	}

	push_back(new NSValueZone(iColorZone, true, dLow, sLow, bLow, false, -1, "", false)) ;
}

void
NSValueZoneArray::setHighValues(NSLdvGoalInfo::JALONSLEVELS iColorZone, double dHigh, string sHigh, bool bHigh)
{
	NSValueZone* pZone = getZone(iColorZone) ;
	if (pZone)
	{
		pZone->bHighExist     = true ;
		pZone->dHighValue     = dHigh ;
		pZone->sHighValueUnit = sHigh ;
		pZone->bHighIncluded  = bHigh ;
    return ;
	}

	push_back(new NSValueZone(iColorZone, false, -1, "", false, true, dHigh, sHigh, bHigh)) ;
}

NSValueZone*
NSValueZoneArray::getZone(NSLdvGoalInfo::JALONSLEVELS iColorZone)
{
	if (!(empty()))
		for (NSValueZoneIter i = begin() ; i != end() ; i++)
			if ((*i)->iLevel == iColorZone)
				return *i ;

	return NULL ;
}

// = operator
NSValueZoneArray&
NSValueZoneArray::operator=(const NSValueZoneArray& src)
{
  if (this == &src)
		return *this ;

try
{
	vider() ;

	if (false == src.empty())
		for (NSValueZoneConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSValueZone(**i)) ;

	return (*this) ;
}
catch (...)
{
	erreur("Exception NSValueZoneArray (=).", standardError, 0) ;
	return *this;
}
}

// Empty the array
void
NSValueZoneArray::vider()
{
	if (empty())
		return ;

	for (NSValueZoneIter i = begin() ; i != end() ; )
	{
		delete (*i) ;
		erase(i) ;
	}
}

