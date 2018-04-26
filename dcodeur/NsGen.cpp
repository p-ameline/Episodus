#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

#include "nssavoir/nspatho.h"
#include "nssavoir/nsfilgd.h"
#include "dcodeur/nsgen.h"
#include "dcodeur/nsphrase.h"
#include "dcodeur/nsdkd.h"

long NSGenerateur::lObjectCount = 0 ;
long NSGenComplement::lObjectCount = 0 ;

// -------------------------------------------------------------------------
// ---------------------- METHODES DE generateur ---------------------------
// -------------------------------------------------------------------------
NSGenerateur::NSGenerateur(NSContexte* pCtx, NsProposition* pPropos, string sLangue)
             :NSRoot(pCtx)
{
  // pPhraseur->nettoye();
  _pCCArray     = new NSGenereCompArray(pCtx, this) ;
  _pProposition = pPropos ;
  initialise() ;

  _sPhrase         = string("") ;
  sLang            = sLangue ;
  _pPh             = (NSPhraseur*) 0 ;
  _iGeneratedLevel = levelUnknown ;

  if (_pProposition && (NsProposition::isPhraseur == _pProposition->iObjectType))
    _pPh = (static_cast<NSPhraseur*>(_pProposition->pProposition)) ;
}

NSGenerateur::NSGenerateur(const NSGenerateur& rv)
             :NSRoot(rv.pContexte)
{
  _pCCArray = new NSGenereCompArray(*(rv._pCCArray)) ;
  initialise() ;
  _sPhrase = string("") ;
  copieTout(&rv) ;
}

NSGenerateur::~NSGenerateur()
{
  delete _pCCArray ;

  if (_pAdjEpitheteAvPos)
      delete _pAdjEpitheteAvPos;
  if (_pAdjEpitheteAvNeg)
      delete _pAdjEpitheteAvNeg;
  if (_pAdjEpitheteApPos)
      delete _pAdjEpitheteApPos;
  if (_pAdjEpitheteApNeg)
      delete _pAdjEpitheteApNeg;
  if (_pCompNomPos)
      delete _pCompNomPos;
  if (_pCompNomNeg)
      delete _pCompNomNeg;
  if (_pAdverbePos)
      delete _pAdverbePos;
  if (_pAdverbeNeg)
      delete _pAdverbeNeg;
  if (_pCODPos)
      delete _pCODPos;
  if (_pCODNeg)
      delete _pCODNeg;
  if (_pAttSujetPos)
      delete _pAttSujetPos;
  if (_pAttSujetNeg)
      delete _pAttSujetNeg;
  if (_pAttCODPos)
      delete _pAttCODPos;
  if (_pAttCODNeg)
      delete _pAttCODNeg;
}

void
NSGenerateur::initialise()
{
  _pAdjEpitheteAvPos = 0;
  _pAdjEpitheteAvNeg = 0;
  _pAdjEpitheteApPos = 0;
  _pAdjEpitheteApNeg = 0;
  _pCompNomPos       = 0;
  _pCompNomNeg       = 0;
  _pAdverbePos       = 0;
  _pAdverbeNeg       = 0;
  _pCODPos           = 0;
  _pCODNeg           = 0;
  _pAttSujetPos      = 0;
  _pAttSujetNeg      = 0;
  _pAttCODPos        = 0;
  _pAttCODNeg        = 0;

  _pCCArray->initialise();
}

void
NSGenerateur::reinitialise()
{
  if (_pAdjEpitheteAvPos)
      delete _pAdjEpitheteAvPos;
  if (_pAdjEpitheteAvNeg)
      delete _pAdjEpitheteAvNeg;
  if (_pAdjEpitheteApPos)
      delete _pAdjEpitheteApPos;
  if (_pAdjEpitheteApNeg)
      delete _pAdjEpitheteApNeg;
  if (_pCompNomPos)
      delete _pCompNomPos;
  if (_pCompNomNeg)
      delete _pCompNomNeg;
  if (_pAdverbePos)
      delete _pAdverbePos;
  if (_pAdverbeNeg)
      delete _pAdverbeNeg;
  if (_pCODPos)
      delete _pCODPos;
  if (_pCODNeg)
      delete _pCODNeg;
  if (_pAttSujetPos)
      delete _pAttSujetPos;
  if (_pAttSujetNeg)
      delete _pAttSujetNeg;
  if (_pAttCODPos)
      delete _pAttCODPos;
  if (_pAttCODNeg)
      delete _pAttCODNeg;

  initialise();
}

void
NSGenerateur::copieTout(const NSGenerateur* rv)
{
  if (rv->_pProposition)
  {
    _pProposition = new NsProposition(*(rv->_pProposition)) ;
    if (_pProposition->iObjectType == NsProposition::isPhraseur)
      _pPh = (static_cast<NSPhraseur*>(_pProposition->pProposition)) ;
  }
  else
    _pProposition = (NsProposition*) 0 ;

  _sPhrase = rv->_sPhrase ;
  sLang    = rv->sLang ;

  _iGeneratedLevel = rv->_iGeneratedLevel ;

  if (rv->_pAdjEpitheteAvPos)
    _pAdjEpitheteAvPos = new NSPhraseMotArray(*(rv->_pAdjEpitheteAvPos)) ;
  else
    _pAdjEpitheteAvPos = (NSPhraseMotArray*) 0 ;

  if (rv->_pAdjEpitheteAvNeg)
    _pAdjEpitheteAvNeg = new NSPhraseMotArray(*(rv->_pAdjEpitheteAvNeg));
  else
    _pAdjEpitheteAvNeg = (NSPhraseMotArray*) 0 ;

  if (rv->_pAdjEpitheteApPos)
    _pAdjEpitheteApPos = new NSPhraseMotArray(*(rv->_pAdjEpitheteApPos));
  else
    _pAdjEpitheteApPos = (NSPhraseMotArray*) 0 ;

  if (rv->_pAdjEpitheteApNeg)
    _pAdjEpitheteApNeg = new NSPhraseMotArray(*(rv->_pAdjEpitheteApNeg));
  else
    _pAdjEpitheteApNeg = (NSPhraseMotArray*) 0 ;

  if (rv->_pCompNomPos)
    _pCompNomPos = new NSPhraseMotArray(*(rv->_pCompNomPos));
  else
    _pCompNomPos = (NSPhraseMotArray*) 0 ;

  if (rv->_pCompNomNeg)
    _pCompNomNeg = new NSPhraseMotArray(*(rv->_pCompNomNeg));
  else
    _pCompNomNeg = (NSPhraseMotArray*) 0 ;

  if (rv->_pAdverbePos)
    _pAdverbePos = new NSPhraseMotArray(*(rv->_pAdverbePos));
  else
    _pAdverbePos = (NSPhraseMotArray*) 0 ;

  if (rv->_pAdverbeNeg)
    _pAdverbeNeg = new NSPhraseMotArray(*(rv->_pAdverbeNeg));
  else
    _pAdverbeNeg = (NSPhraseMotArray*) 0 ;

  if (rv->_pCODPos)
    _pCODPos = new NSPhraseMotArray(*(rv->_pCODPos));
  else
    _pCODPos = (NSPhraseMotArray*) 0 ;

  if (rv->_pCODNeg)
    _pCODNeg = new NSPhraseMotArray(*(rv->_pCODNeg));
  else
    _pCODNeg = (NSPhraseMotArray*) 0 ;

  if (rv->_pAttSujetPos)
    _pAttSujetPos = new NSPhraseMotArray(*(rv->_pAttSujetPos));
  else
    _pAttSujetPos = (NSPhraseMotArray*) 0 ;

  if (rv->_pAttSujetNeg)
    _pAttSujetNeg = new NSPhraseMotArray(*(rv->_pAttSujetNeg));
  else
    _pAttSujetNeg = (NSPhraseMotArray*) 0 ;

  if (rv->_pAttCODPos)
    _pAttCODPos = new NSPhraseMotArray(*(rv->_pAttCODPos));
  else
    _pAttCODPos = (NSPhraseMotArray*) 0 ;

  if (rv->_pAttCODNeg)
    _pAttCODNeg = new NSPhraseMotArray(*(rv->_pAttCODNeg));
  else
    _pAttCODNeg = (NSPhraseMotArray*) 0 ;

  *_pCCArray = *(rv->_pCCArray) ;
}

bool
NSGenerateur::genereProposition(DCODETYPE iStyle, NsProposition* pPropos)
{
  NsProposition* pCurrentProp ;
  if (pPropos)
    pCurrentProp = pPropos ;
  else
    pCurrentProp = _pProposition ;

  if ((NsProposition*) NULL == pCurrentProp)
    return false ;
try
{
  if (NULL == pCurrentProp->pProposition)
    return false ;

  pCurrentProp->setPhrase(string("")) ;
  pCurrentProp->sLang = sLang ;

  //
  // Proposition isolee : on se contente de la decoder
  // Lone proposition : we just generate it
  //
  if (NsProposition::isPhraseur == pCurrentProp->iObjectType)
  {
    _pPh = (static_cast<NSPhraseur*>(pCurrentProp->pProposition)) ;
    bool bResult = generePhrase(iStyle) ;
    if (bResult)
      pCurrentProp->setPhrase(_sPhrase) ;
    return bResult ;
  }
  //
  // Propositions multiples : on decode, puis on assemble
  // Multiple propositions : we generate each one, then we build the sentence
  //
  if (NsProposition::isPropositionArray == pCurrentProp->iObjectType)
  {
    NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pCurrentProp->pProposition)) ;
    if (((NSPropositionArray*) NULL == pPropArray) || pPropArray->empty())
    {
      string sErrorText = string("NSGenerateur::genereProposition: null or empty propositions array.") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trWarning) ;
      return true ;
    }
    for (iterProposition i = pPropArray->begin() ; pPropArray->end() != i ; i++)
    {
      bool bResult = genereProposition(iStyle, *i) ;
      if (false == bResult)
        return false ;
    }
    return assembleProposition(iStyle, pCurrentProp) ;
  }
  return false ;
}
catch (...)
{
	erreur("Exception NSGenerateurFr::generePhrase.", standardError) ;
  return false ;
}
}

bool
NSGenerateur::ajouteMot(NSPhraseMotArray** ppMotArray, NSPhraseMot* pMot)
{
try
{
	if (NULL == pMot)
  	return true ;

	if (NULL == *ppMotArray)
  	*ppMotArray = new NSPhraseMotArray() ;

	(*ppMotArray)->push_back(pMot) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSGenerateur::ajouteMot.", standardError) ;
	return false ;
}
}

bool
NSGenerateur::ajouteCopieMot(NSPhraseMotArray** ppMotArray, const NSPhraseMot* pMot)
{
try
{
  if ((NSPhraseMot*) NULL == pMot)
    return true ;

  if ((NSPhraseMotArray*) NULL == *ppMotArray)
    *ppMotArray = new NSPhraseMotArray() ;

  //
  // NSPhraseMotTime ?
  //
  const NSPhraseMotTime* pDate = dynamic_cast<const NSPhraseMotTime*>(pMot) ;
  if (pDate)
  {
    NSPhraseMotTime *pNewDate = new NSPhraseMotTime(*pDate) ;
    (*ppMotArray)->push_back(pNewDate) ;
  }
  else
  {
    const NSPhraseMotTimeCycle* pCycle = dynamic_cast<const NSPhraseMotTimeCycle*>(pMot) ;
    if (pCycle)
    {
      NSPhraseMotTimeCycle *pNewCycle = new NSPhraseMotTimeCycle(*pCycle) ;
      (*ppMotArray)->push_back(pNewCycle) ;
    }
    else
      (*ppMotArray)->push_back(new NSPhraseMot(*pMot)) ;
  }

  return true ;
}
catch (...)
{
	erreur("Exception NSGenerateur::ajouteCopieMot.", standardError) ;
	return false ;
}
}

bool
NSGenerateur::terminePhrase()
{
  if (string("") != _sPhrase)
  {
    _sPhrase[0] = pseumaj(_sPhrase[0]) ;
    _sPhrase += string(".") ;
  }
  return true ;
}

bool
NSGenerateur::termineProposition(NsProposition* pPropos)
{
  string sPhrase = string("") ;

  if (pPropos)
    sPhrase = pPropos->getPhrase() ;
  else
    sPhrase = _pProposition->getPhrase() ;

  if (string("") != sPhrase)
  {
    sPhrase[0] = pseumaj(sPhrase[0]) ;
    sPhrase += string(".") ;
  }

  if (pPropos)
    pPropos->setPhrase(sPhrase) ;
  else
    _pProposition->setPhrase(sPhrase) ;

  return true ;
}

bool
NSGenerateur::CommenceParVoyelle(const string& sLib)
{
  if (string("") == sLib)
    return false ;

	switch (sLib[0])
  {
    case 'È' ://200
    case 'É' :
    case 'Ê' :
    case 'Ë' :
    case 'é' :
    case 'e' :
    case 'E' :
    case 'ë' :
    case 'è' :
    case 'ê' :
    case 'À' :
    case 'Á' :
    case 'a' :
    case 'A' :
    case 'Â' :
    case 'Ã' :
    case 'Ä' :
    case 'Å' : //197
    case 'Æ' :
    case 'à' : //226
    case 'â' :
    case 'ã' :
    case 'ä' :
    case 'å' :
    case 'æ' :
    case 'á' :
    case 'Ì' :
    case 'Í' :
    case 'Î' :
    case 'Ï' :
    case 'í' :
    case 'i' :
    case 'I' :
    case 'î' :
    case 'ï' :
    case 'Ü' ://220
    case 'Û' :
    case 'Ú' :
    case 'Ù' : //216
    case 'ú' :
    case 'û' :
    case 'u' :
    case 'U' :
    case 'ü' :
    case 'ù' :
    case 'Õ' : //213
    case 'Ö' :
    case 'Ô' :
    case 'o' :
    case 'O' :
    case 'Ó' :
    case 'Ò' :
    case 'ò' : //242
    case 'ó' :
    case 'ô' :
    case 'õ' :
    case 'ö' :
    case 'Ý' :
    case 'y' :
    case 'Y' :
    case 'ÿ' :  return true ;
  }
  
  return false ;
}

//  Unstack from: 2 "solid"          to: ""
//                1 "nice"               "solid"
//                  "great, pure"        "great, pure, nice"
//
//  It is legal to have type2 == "" in order to "purge" the stack, for example
//  to end up with "great, pure, nice, solid"
//
void
NSGenerateur::etDuMilieu(string *type, string *type1, string *type2, const string sSepar)
{
	if (((string*) NULL == type) || ((string*) NULL == type1) || ((string*) NULL == type2))
		return ;

  if (string("") != *type1)
  {
	  if (string("") != *type)
		  *type += sSepar ;
	  *type += *type1 ;
  }

	*type1 = *type2 ;
	*type2 = string("") ;
}

void
NSGenerateur::contracting(string *pSentence, string sFrom, string sTo)
{
  if (((string *) NULL == pSentence) || (string("") == *pSentence) || (string("") == sFrom))
    return ;

  size_t posit1 = pSentence->find(sFrom) ;
  if (NPOS == posit1)
    return ;

  size_t iFromSize = strlen(sFrom.c_str()) ;

  while (NPOS != posit1)
  {
    if (0 == posit1)
      pSentence->replace(0, iFromSize, sTo) ;
    else if ((' ' == (*pSentence)[posit1-1]) || (' ' == sFrom[0]))
      pSentence->replace(posit1, iFromSize, sTo) ;

    posit1 = pSentence->find(sFrom, posit1 + 1) ;
  }
}

//
// Dispatch an array of NSPhraseMot into two arrays of homogeneous elements
// Typically affirmative/negative or pathological/physiological
//
// Can also dispatch only elements which are of proper type
//
void
NSGenerateur::dispatchArray(const NSPhraseMotArray* pMots, NSPhraseMotArray** ppYesArray, NSPhraseMotArray** ppNoArray, WORDTYPE iWordType)
{
  if (((NSPhraseMotArray*) NULL == pMots) || (pMots->empty()))
    return ;

  // Default grouping is affirmative/negative
  //
  if (string("") == pMots->getEnumerationOrder())
  {
    dispatchArrayYesNo(pMots, ppYesArray, ppNoArray, iWordType) ;
    return ;
  }

  string sGroupLabel1 = string("") ;
  string sGroupLabel2 = string("") ;

  splitString(pMots->getEnumerationOrder(), &sGroupLabel1, &sGroupLabel2, '/') ;

  for (iterConstPhraseMot iterMots = pMots->begin() ; pMots->end() != iterMots ; iterMots++)
  {
    bool bFound = isProperType(*iterMots, iWordType) ;

    if (bFound)
    {
      if ((*iterMots)->getEnumCategory() == sGroupLabel1)
        ajouteCopieMot(ppYesArray, *iterMots) ;
      else
        ajouteCopieMot(ppNoArray, *iterMots) ;
    }
  }
}

//
// Dispatch an array of NSPhraseMot into an array of affirmative elements
// and an array of negative elements
//
// Can also dispatch only elements which are of proper type
//
void
NSGenerateur::dispatchArrayYesNo(const NSPhraseMotArray* pMots, NSPhraseMotArray** ppYesArray, NSPhraseMotArray** ppNoArray, WORDTYPE iWordType)
{
  if (((NSPhraseMotArray*) NULL == pMots) || (pMots->empty()))
    return ;

  for (iterConstPhraseMot iterMots = pMots->begin() ; pMots->end() != iterMots ; iterMots++)
  {
    bool bFound = isProperType(*iterMots, iWordType) ;

    if (bFound)
      dispatchYesNo(*iterMots, ppYesArray, ppNoArray, pMots->getGroupType()) ;
  }
}

//
// Dispatch a NSPhraseMot into the array of affirmative elements
// or the array of negative elements
//
void
NSGenerateur::dispatchYesNo(const NSPhraseMot* pMot, NSPhraseMotArray** ppYesArray, NSPhraseMotArray** ppNoArray, NSPhraseMotArray::TYPEGRPENUM iGroupingMode)
{
  if ((NSPhraseMot*) NULL == pMot)
    return ;

  string sCertitude = pMot->getCertitude() ;
  if ((string("") != sCertitude) && (NSPhraseMotArray::groupMixedWithNon != iGroupingMode))
  {
    if      ((string(sCertitude, 0, 3) == "WCE") &&
             (string(sCertitude, 0, 5) < "WCE50"))
      ajouteCopieMot(ppNoArray, pMot) ;

    else if ((string(sCertitude, 0, 3) == "WCE") &&
             (string(sCertitude, 0, 5) >= "WCE50"))
      ajouteCopieMot(ppYesArray, pMot) ;
  }
  else
    ajouteCopieMot(ppYesArray, pMot) ;
}

bool
NSGenerateur::isProperType(const NSPhraseMot* pMot, WORDTYPE iWordType) const
{
  if (isAny == iWordType)
    return true ;

  if ((NSPhraseMot*) NULL == pMot)
    return false ;

  if (pMot->estTexteLibre())
    return true ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
  {
    string sErrorText = string("NSGenerateur::dispatchArray discarding ") + pMot->getLexique() + string(" which cannot be found in Lexique for langage ") + sLang ;
    pSuper->trace(&sErrorText, 1, NSSuper::trWarning) ;
    return false ;
  }

  string sDiscard = string("") ;

  switch(iWordType)
  {
    case isNoun :
    {
      if (false == pData->estNom())
        sDiscard = string("a noun") ;
      break ;
    }
    case isAdjective :
    {
      if (false == pData->estAdjectif())
        sDiscard = string("an adjective") ;
      break ;
    }
    case isAdverb :
    {
      if (false == pData->estAdverbe())
        sDiscard = string("an adverb") ;
      break ;
    }
    case isVerb :
    {
      if (false == pData->estVerbe())
        sDiscard = string("a verb") ;
      break ;
    }
    case isInvar :
    {
      if (false == pData->estInvariable())
        sDiscard = string("an invariable") ;
      break ;
    }
  }

  if (string("") != sDiscard)
  {
    string sErrorText = string("NSGenerateurFr::classeTout discarding ") + pMot->getLexique() + string(" because it is not ") + sDiscard + string(".") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trWarning) ;
    return false ;
  }

  return true ;
}

bool
NSGenerateur::getPathologData(const string &sLexique, NSPathologData &Data)
{
  bool bFound = pContexte->getDico()->trouvePathologData(sLang, &sLexique, &Data) ;
  if (bFound)
    return true ;

  string sErrorText = string("NSGenerateur: can't find ") + sLexique + string(" in the Lexique.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;

  return false ;
}

bool
NSGenerateur::agglutinate(NSPhraseMot* pMot, string& sLabel)
{
  if ((NSPhraseMot*) NULL == pMot)
    return false ;

  string sComposer = pMot->getComposer() ;
  if ((string("") == sLabel) || (string("") == sComposer))
    return true ;

  NSPathologData DataComposer ;
  bool bFound = getPathologData(sComposer, DataComposer) ;
  if (false == bFound)
    return false ;

  string sComposerLabel = string("") ;
  donneLibelleAffiche(&sComposerLabel, &DataComposer) ;
  if (string("") == sComposerLabel)
    return false ;

  string sSeparator = string("") ;
  switch(pMot->getCompLnk())
  {
    case NSPhraseMot::linkSpace : sSeparator = string(" ") ; break ;
    case NSPhraseMot::linkMinus : sSeparator = string("-") ; break ;
  }

  switch(pMot->getCompPos())
  {
    case NSPhraseMot::posAvant :
      sLabel = sComposerLabel + sSeparator + sLabel ;
      break ;
    case NSPhraseMot::posApres :
      sLabel += sSeparator + sComposerLabel ;
      break ;
  }

  return true ;
}

// traitePostpositon sert a remplir la string sPreposition des noms qui completent
// un nom qui impose une preposition a ses suivants.
//
// Antecedents de pericardite et d'endocardite.
//
// pSpecifique sert dans le cas ou l'on veut mettre des prepositions liees au mot
// pMot dans une array donnee, mais qui ne figure pas dans pComplement.
//
bool
NSGenerateur::traitePostposition(NSPhraseMot* pMot, NSPhraseMotArray* pSpecifique)
{
  if (false == traitePostpositionGeneric(pMot, pSpecifique))
    return false ;
  return traitePostpositionLocation(pMot) ;
}

// traitePostpositionGeneric traite POSTPOSITION, destiné aux compléments de
// nom (si le concept est un nom) ou aux COD/COI (si le concept est un verbe)
//
bool
NSGenerateur::traitePostpositionGeneric(NSPhraseMot* pMot, NSPhraseMotArray* pSpecifique)
{
  if ((NSPhraseMot*) NULL == pMot)
    return false ;

  bool bSuccess ;
  string sPostposition = getPostposition(pMot, string(POSTPOSITION), &bSuccess) ;
  if (string("") == sPostposition)
    return bSuccess ;

  // Si pMot est un nom, il peut imposer une preposition a son complement du nom.
  //
  if (pMot->getFirstComplementPhr())
  {
    NSPhraseur* pComp = pMot->getFirstComplementPhr() ;

    if (false == pComp->compNom.empty())
    {
      for (iterPhraseMot iterMots = pComp->compNom.begin() ; pComp->compNom.end() != iterMots ; iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
          (*iterMots)->setPreposition(sPostposition) ;
        else
        {
          // On rajoute la preposition dans le libelle du texte libre.
          (*iterMots)->setPreposition(sPostposition);
          (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                             + (*iterMots)->getTexteLibre()) ;
        }
      }
    }
  }

  // Si pMot est un verbe, il peut imposer une preposition au COI ou au COS.
  //
  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if (pData && pData->estVerbe())
  {
    if (false == _pPh->COI.empty())
    {
      for (iterPhraseMot iterMots = _pPh->COI.begin() ; _pPh->COI.end() != iterMots ; iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
          (*iterMots)->setPreposition(sPostposition) ;

        else
        {
          // On rajoute la preposition dans le libelle du texte libre.
          (*iterMots)->setPreposition(sPostposition) ;
          (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                            + (*iterMots)->getTexteLibre()) ;
        }
      }
    }

    if (false == _pPh->COS.empty())
    {
      for (iterPhraseMot iterMots = _pPh->COS.begin() ; _pPh->COS.end() != iterMots ; iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
          (*iterMots)->setPreposition(sPostposition) ;

        else
        {
          // On rajoute la preposition dans le libelle du texte libre.
          (*iterMots)->setPreposition(sPostposition) ;
          (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                             + (*iterMots)->getTexteLibre()) ;
        }
      }
    }
  }

  if (pSpecifique)
  {
    if (false == pSpecifique->empty())
    {
      for (iterPhraseMot iterMots = pSpecifique->begin() ; pSpecifique->end() != iterMots ; iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
          (*iterMots)->setPreposition(sPostposition) ;

        else
        {
          // On rajoute la preposition dans le libelle du texte libre.
          (*iterMots)->setPreposition(sPostposition) ;
          (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                          + (*iterMots)->getTexteLibre()) ;
        }
      }
    }
  }

  return true ;
}

// traitePostpositionLocation traite PREP4LOC, destiné aux compléments
// circonstanciels de lieu (ex : angioplastie (DE l'artère) et non "à l'artère")
//
bool
NSGenerateur::traitePostpositionLocation(NSPhraseMot* pMot)
{
  if ((NSPhraseMot*) NULL == pMot)
    return false ;

  // No location complement -> nothing to do
  //
  NSPhraseur* pComp = pMot->getFirstComplementPhr() ;
  if (((NSPhraseur*) NULL == pComp) || pComp->CCLieu.empty())
    return true ;

  bool bSuccess ;
  string sPostposition = getPostposition(pMot, string(PREP4LOC), &bSuccess) ;
  if (string("") == sPostposition)
    return bSuccess ;

  for (iterPhraseMot iterMots = pComp->CCLieu.begin() ; pComp->CCLieu.end() != iterMots ; iterMots++)
  {
    if (false == (*iterMots)->estTexteLibre())
      (*iterMots)->setPreposition(sPostposition) ;
    else
    {
      // On rajoute la preposition dans le libelle du texte libre.
      (*iterMots)->setPreposition(sPostposition);
      (*iterMots)->setTexteLibre(sPostposition + string(" ")
                                         + (*iterMots)->getTexteLibre()) ;
    }
  }

  return true ;
}

string
NSGenerateur::getPostposition(NSPhraseMot* pMot, const string sPostPosType, bool* pbSuccess) const
{
  if (pbSuccess)
    *pbSuccess = false ;

  if ((NSPhraseMot*) NULL == pMot)
    return string("") ;

  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return string("") ;

  if (pbSuccess)
    *pbSuccess = true ;

  // Premier cas, postposition générique
  //
  string sPostpositionBrute = pData->chercheGrammaire(sPostPosType) ;
  if (string("") == sPostpositionBrute)
    return string("") ;

  // La postposition est la string sPostpositionBrute privee du radical initial.
  //
  size_t iTagLen = strlen(sPostPosType.c_str()) ;
  if (strlen(sPostpositionBrute.c_str()) == iTagLen)
    return string("") ;

  return string(sPostpositionBrute, iTagLen, strlen(sPostpositionBrute.c_str()) - iTagLen) ;
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSGenComplement ------------------------
// -------------------------------------------------------------------------
NSGenComplement::NSGenComplement(NSContexte* pCtx, NSGenerateur* pGener)
                :NSRoot(pCtx)
{
  _pGenerateur = pGener ;

  _sType       = string("") ;
  _sPositif    = string("") ;
  _sNegatif    = string("") ;
  _pMotsPos    = (NSPhraseMotArray*) 0 ;
  _pMotsNeg    = (NSPhraseMotArray*) 0 ;
  _iPriorite   = 50 ;
}

NSGenComplement::NSGenComplement(const NSGenComplement& rv)
                :NSRoot(rv.pContexte)
{
try
{
  _sType        = rv._sType ;
  _sPositif     = rv._sPositif ;
  _sNegatif     = rv._sNegatif ;
  _iPriorite    = rv._iPriorite ;
  _pGenerateur  = rv._pGenerateur ;

  if (rv._pMotsPos)
    _pMotsPos = new NSPhraseMotArray(*(rv._pMotsPos)) ;
  else
    _pMotsPos = (NSPhraseMotArray*) 0 ;
  if (rv._pMotsNeg)
    _pMotsNeg = new NSPhraseMotArray(*(rv._pMotsNeg)) ;
  else
    _pMotsNeg = (NSPhraseMotArray*) 0 ;
}
catch (...)
{
	erreur("Exception NSGenComplement copy ctor.", standardError) ;
}
}

NSGenComplement::~NSGenComplement()
{
	if (_pMotsPos)
  	delete _pMotsPos ;
	if (_pMotsNeg)
  	delete _pMotsNeg ;
}

void
NSGenComplement::donnePhrElements(NSPhraseMot** ppLiaison, NSPhraseMot** ppPreposition, NSPhraseMotArray** ppMots)
{
  *ppLiaison     = (NSPhraseMot*) 0 ;
  *ppPreposition = (NSPhraseMot*) 0 ;
  *ppMots        = (NSPhraseMotArray*) 0 ;

  if ((NSGenerateur*) NULL == _pGenerateur)
    return ;

  NSPhraseur* pPhra = _pGenerateur->getCurrentPhraseur() ;

  if ((NSPhraseur*) NULL == pPhra)
    return ;

  if      (string(STR_CCLIEU) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonLieu) ;
    *ppPreposition = &(pPhra->PrepositionLieu) ;
    *ppMots        = &(pPhra->CCLieu) ;
  }
  else if (string(STR_CCTEMPS) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonTemps) ;
    *ppPreposition = &(pPhra->PrepositionTemps) ;
    *ppMots        = &(pPhra->CCTemps) ;
  }
  else if (string(STR_CCMANIERE) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonManiere) ;
    *ppPreposition = &(pPhra->PrepositionManiere) ;
    *ppMots        = &(pPhra->CCManiere) ;
  }
  else if (string(STR_CCMOYEN) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonMoyen) ;
    *ppPreposition = &(pPhra->PrepositionMoyen) ;
    *ppMots        = &(pPhra->CCMoyen) ;
  }
  else if (string(STR_CCCHIFFRE) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonChiffre) ;
    *ppPreposition = &(pPhra->PrepositionChiffre) ;
    *ppMots        = &(pPhra->CCChiffre) ;
  }
  else if (string(STR_CCBUT) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonBut) ;
    *ppPreposition = &(pPhra->PrepositionBut) ;
    *ppMots        = &(pPhra->CCBut) ;
  }
  else if (string(STR_CCCAUSE) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonCause) ;
    *ppPreposition = &(pPhra->PrepositionCause) ;
    *ppMots        = &(pPhra->CCCause) ;
  }
  else if (string(STR_CCTYPE) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonType) ;
    *ppPreposition = &(pPhra->PrepositionType) ;
    *ppMots        = &(pPhra->CCType) ;
  }
  else if (string(STR_CCHYPOTH) == _sType)
  {
    *ppLiaison     = &(pPhra->LiaisonHypoth) ;
    *ppPreposition = &(pPhra->PrepositionHypoth) ;
    *ppMots        = &(pPhra->CCHypoth) ;
  }
}

bool
NSGenComplement::donnePhrase(DCODETYPE iStyle)
{
  bool bSucces ;

  NSPhraseMot*      pLiaison ;
  NSPhraseMot*      pPreposition ;
  NSPhraseMotArray* pMots ;

  donnePhrElements(&pLiaison, &pPreposition, &pMots) ;

  // TODO: the pb here is that we use donnePhraseChiffree only for
  //       "numerical value" complement (typically a dimension), but it should
  //       also get used for location complements of the kind
  //                                            "located at 25 mm from..."
  //       Means that calling donnePhraseChiffree versus donnePhraseComplement
  //       should be decided for each complement and not for the whole block
  //
  // Below a awfull hack
  bool bForceNumCompl = false ;
  if (_pMotsPos && (_pMotsPos->size() == 1))
  {
    NSPhraseMot* pMot = *(_pMotsPos->begin()) ;
    if ((pMot->getUnite() != string("")) && pMot->getComplementPhr() &&
                                (false == pMot->getComplementPhr()->empty()))
      bForceNumCompl = true ;
  }

  if ((STR_CCCHIFFRE == _sType) || bForceNumCompl)
  {
    if (_pMotsPos && (false == _pMotsPos->empty()))
    {
      _sPositif = _pGenerateur->donnePhraseChiffree(pLiaison, pPreposition,
                                                        _pMotsPos, &bSucces) ;
      if (false == bSucces)
        return false ;
    }

    if (_pMotsNeg && (false == _pMotsNeg->empty()))
    {
      _sNegatif = _pGenerateur->donnePhraseChiffree(pLiaison, pPreposition,
                                                        _pMotsNeg, &bSucces) ;
      if (false == bSucces)
        return false ;
    }
  }
  else
  {
    if (_pMotsPos && (false == _pMotsPos->empty()))
    {
      _sPositif = _pGenerateur->donnePhraseComplement(pLiaison, pPreposition,
                                                          _pMotsPos, &bSucces,
                                                          iStyle) ;
      if (false == bSucces)
        return false ;
    }

    if (_pMotsNeg && (false == _pMotsNeg->empty()))
    {
      _sNegatif = _pGenerateur->donnePhraseComplement(pLiaison, pPreposition,
                                                          _pMotsNeg, &bSucces,
                                                          iStyle) ;
      if (false == bSucces)
        return false ;
    }
  }
  return true ;
}

void
NSGenComplement::initialise()
{
  _sType     = string("") ;
  _sPositif  = string("") ;
  _sNegatif  = string("") ;
  _iPriorite = 0 ;

  if (_pMotsPos)
  {
    delete _pMotsPos ;
    _pMotsPos = (NSPhraseMotArray*) 0 ;
  }
  if (_pMotsNeg)
  {
    delete _pMotsNeg ;
    _pMotsNeg = (NSPhraseMotArray*) 0 ;
  }
}

void
NSGenComplement::classe()
{
  _iPriorite = 0 ;
  _sPositif  = string("") ;
  _sNegatif  = string("") ;
  if (_pMotsPos)
  {
    delete _pMotsPos ;
    _pMotsPos = (NSPhraseMotArray*) 0 ;
  }
  if (_pMotsNeg)
  {
    delete _pMotsNeg ;
    _pMotsNeg = (NSPhraseMotArray*) 0 ;
  }

  NSPhraseMot*      pLiaison ;
  NSPhraseMot*      pPreposition ;
  NSPhraseMotArray* pMots ;

  donnePhrElements(&pLiaison, &pPreposition, &pMots) ;

  if ((NSPhraseMotArray*) NULL == pMots)
    return ;

  for (iterPhraseMot iterMots = pMots->begin() ; pMots->end() != iterMots ; iterMots++)
  {
    string sLexique = (*iterMots)->getLexique() ;

    if ((string("") != sLexique) && (false == (*iterMots)->estTexteLibre()))
    {
      /*
      NSPathologData* pData = (*iterMots)->getLexiqueData(sLang) ;
      if ((NSPathologData*) NULL == pData)
        return ;
      */
      // if (false == pData->estNom())
      //    return;

      string sCertitude = (*iterMots)->getCertitude() ;

      if ((string(sCertitude, 0, 3) == string("WCE")) &&
          (string(sCertitude, 0, 5) < "WCE50"))
        _pGenerateur->ajouteCopieMot(&_pMotsNeg, *iterMots) ;
      else if (((string(sCertitude, 0, 3) == "WCE") &&
                (string(sCertitude, 0, 5) >= "WCE50")) ||
                                             (string("") == sCertitude))
        _pGenerateur->ajouteCopieMot(&_pMotsPos, *iterMots) ;
    }
    else
      _pGenerateur->ajouteCopieMot(&_pMotsPos, *iterMots) ;

    _iPriorite = max(_iPriorite, (*iterMots)->getPriority()) ;
  }
}

NSGenComplement&
NSGenComplement::operator=(const NSGenComplement& rv)
{
  if (&rv == this)
    return *this ;

  _sType       = rv._sType ;
  _sPositif    = rv._sPositif ;
  _sNegatif    = rv._sNegatif ;
  _iPriorite   = rv._iPriorite ;
  _pGenerateur = rv._pGenerateur ;

  if (_pMotsPos)
  {
    delete _pMotsPos ;
    _pMotsPos = (NSPhraseMotArray*) 0 ;
  }
  if (rv._pMotsPos)
    _pMotsPos = new NSPhraseMotArray(*(rv._pMotsPos)) ;

  if (_pMotsNeg)
  {
    delete _pMotsNeg ;
    _pMotsNeg = (NSPhraseMotArray*) 0 ;
  }
  if (rv._pMotsNeg)
    _pMotsNeg = new NSPhraseMotArray(*(rv._pMotsNeg)) ;

  return *this ;
}

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSGenereCompArray -----------------------
// -------------------------------------------------------------------------

NSGenereCompArray::NSGenereCompArray(NSContexte* pCtx, NSGenerateur* pGener)
                  :NSGenCompArray()
{
  pGenerateur = pGener ;
  pContexte   = pCtx ;
}

//--------------------------------------------------------------------------
//  Constructeur copie
//--------------------------------------------------------------------------
NSGenereCompArray::NSGenereCompArray(const NSGenereCompArray& rv)
                  :NSGenCompArray()
{
  pGenerateur = rv.pGenerateur ;
  pContexte   = rv.pContexte ;

try
{
  if (false == rv.empty())
    for (GenCompConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSGenComplement(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSGenereCompArray copy ctor.", standardError) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSGenereCompArray::vider()
{
  if (empty())
    return ;

  for (GenCompIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSGenereCompArray::~NSGenereCompArray()
{
	vider() ;
}

void
NSGenereCompArray::initialise()
{
  vider() ;

    /*if (empty())
        return true;

    for (GenCompIter i = begin(); i != end(); i++)
        (*i)->initialise(); */

  return ;
}

void
NSGenereCompArray::classeTout()
{
try
{
  vider() ;

  if ((NULL == pGenerateur) ||
                   ((NSPhraseur*) NULL == pGenerateur->getCurrentPhraseur()))
    return ;

  NSPhraseur* pPh = pGenerateur->getCurrentPhraseur() ;

  if (false == pPh->CCLieu.empty())
  {
    NSGenComplement* pCCLieu = new NSGenComplement(pContexte, pGenerateur) ;
    pCCLieu->setType(STR_CCLIEU) ;
    pCCLieu->classe() ;
    push_back(pCCLieu) ;
  }
  if (false == pPh->CCTemps.empty())
  {
    NSGenComplement* pCCTemps = new NSGenComplement(pContexte, pGenerateur) ;
    pCCTemps->setType(STR_CCTEMPS) ;
    pCCTemps->classe() ;
    push_back(pCCTemps) ;
  }
  if (false == pPh->CCManiere.empty())
  {
    NSGenComplement* pCCManiere = new NSGenComplement(pContexte, pGenerateur) ;
    pCCManiere->setType(STR_CCMANIERE) ;
    pCCManiere->classe() ;
    push_back(pCCManiere) ;
  }
  if (false == pPh->CCMoyen.empty())
  {
    NSGenComplement* pCCMoyen = new NSGenComplement(pContexte, pGenerateur) ;
    pCCMoyen->setType(STR_CCMOYEN) ;
    pCCMoyen->classe() ;
    push_back(pCCMoyen) ;
  }
  if (false == pPh->CCCause.empty())
  {
    NSGenComplement* pCCCause = new NSGenComplement(pContexte, pGenerateur) ;
    pCCCause->setType(STR_CCCAUSE) ;
    pCCCause->classe() ;
    push_back(pCCCause) ;
  }
  if (false == pPh->CCBut.empty())
  {
    NSGenComplement* pCCBut = new NSGenComplement(pContexte, pGenerateur) ;
    pCCBut->setType(STR_CCBUT) ;
    pCCBut->classe() ;
    push_back(pCCBut) ;
  }
  if (false == pPh->CCType.empty())
  {
    NSGenComplement* pCCType = new NSGenComplement(pContexte, pGenerateur) ;
    pCCType->setType(STR_CCTYPE) ;
    pCCType->classe() ;
    push_back(pCCType) ;
  }
  if (false == pPh->CCChiffre.empty())
  {
    NSGenComplement* pCCChiffre = new NSGenComplement(pContexte, pGenerateur) ;
    pCCChiffre->setType(STR_CCCHIFFRE) ;
    pCCChiffre->classe() ;
    push_back(pCCChiffre) ;
  }
  if (false == pPh->CCHypoth.empty())
  {
    NSGenComplement* pCCHypoth = new NSGenComplement(pContexte, pGenerateur) ;
    pCCHypoth->setType(STR_CCHYPOTH) ;
    pCCHypoth->classe() ;
    push_back(pCCHypoth) ;
  }
}
catch (...)
{
	erreur("Exception NSGenereCompArray::classeTout", standardError) ;
	return ;
}
}

NSGenComplement*
NSGenereCompArray::donneComplement(string sType)
{
  if (empty())
    return (NSGenComplement*) 0 ;

  for (GenCompIter i = begin() ; end() != i ; i++)
    if ((*i)->getType() == sType)
      return (*i) ;

  return (NSGenComplement*) 0 ;
}

bool
NSGenereCompArray::donnePhrase(DCODETYPE iStyle)
{
  if (empty())
    return true ;

  for (GenCompIter i = begin() ; end() != i ; i++)
    if (false == (*i)->donnePhrase(iStyle))
      return false ;

  return true ;
}

NSGenereCompArray&
NSGenereCompArray::operator=(const NSGenereCompArray& src)
{
	if (this == &src)
		return *this ;

	pGenerateur = src.pGenerateur ;
	pContexte   = src.pContexte ;

try
{
	vider() ;

	if (false == src.empty())
  	for (GenCompConstIter i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSGenComplement(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSGenereCompArray::operator=", standardError) ;
	return *this ;
}
}

