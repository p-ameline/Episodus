#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage/nsdivfct.h"
  #include "nautilus/nssuper.h"
#endif

#include "nssavoir/nspatho.h"
#include "nssavoir/nsfilgd.h"
#include "dcodeur/nsphrase.h"
#include "dcodeur/nsgen.h"
#include "dcodeur/nsdkd.h"
#include "nsbb/nsbbtran.h"

long NSPhraseMot::lBaseObjectCount = 0 ;
long NSPhraseur::lBaseObjectCount = 0 ;
long NsProposition::lObjectCount = 0 ;

// -------------------------------------------------------------------------
// ---------------------- METHODES DE phraseur -----------------------------
// -------------------------------------------------------------------------
NSPhraseur::NSPhraseur(NSContexte* pCtx)
           :NSRoot(pCtx), LiaisonLieu(pCtx), PrepositionLieu(pCtx),
                          LiaisonTemps(pCtx), PrepositionTemps(pCtx),
                          LiaisonManiere(pCtx), PrepositionManiere(pCtx),
                          LiaisonMoyen(pCtx), PrepositionMoyen(pCtx),
                          LiaisonCause(pCtx), PrepositionCause(pCtx),
                          LiaisonBut(pCtx), PrepositionBut(pCtx),
                          LiaisonType(pCtx), PrepositionType(pCtx),
                          LiaisonHypoth(pCtx), PrepositionHypoth(pCtx),
                          LiaisonChiffre(pCtx), PrepositionChiffre(pCtx),
                          adjNumeralCardinal(pCtx), adjNumeralOrdinal(pCtx)
{
  initialise() ;
}

NSPhraseur::NSPhraseur(const NSPhraseur& rv)
           :NSRoot(rv.pContexte),
            LiaisonLieu(rv.pContexte),
            PrepositionLieu(rv.pContexte),
            LiaisonTemps(rv.pContexte),
            PrepositionTemps(rv.pContexte),
            LiaisonManiere(rv.pContexte),
            PrepositionManiere(rv.pContexte),
            LiaisonMoyen(rv.pContexte),
            PrepositionMoyen(rv.pContexte),
            LiaisonCause(rv.pContexte),
            PrepositionCause(rv.pContexte),
            LiaisonBut(rv.pContexte),
            PrepositionBut(rv.pContexte),
            LiaisonType(rv.pContexte),
            PrepositionType(rv.pContexte),
            LiaisonHypoth(rv.pContexte),
            PrepositionHypoth(rv.pContexte),
            LiaisonChiffre(rv.pContexte),
            PrepositionChiffre(rv.pContexte),
            adjNumeralCardinal(rv.pContexte),
            adjNumeralOrdinal(rv.pContexte)
{
try
{
  copieTout(&rv) ;

  pParentheses  = (NSPhraseur*) 0 ;
  pDeuxPoints   = (NSPhraseur*) 0 ;

  if (rv.pParentheses)
    pParentheses = new NSPhraseur(*(rv.pParentheses)) ;

  if (rv.pDeuxPoints)
    pDeuxPoints = new NSPhraseur(*(rv.pDeuxPoints)) ;
}
catch (...)
{
	erreur("Exception NSPhraseur copy ctor.", standardError) ;
}
}

//--------------------------------------------------------------------------
// DESTRUCTEUR
// -------------------------------------------------------------------------

NSPhraseur::~NSPhraseur()
{
  if (pParentheses)
    delete pParentheses ;

  if (pDeuxPoints)
    delete pDeuxPoints ;
}

NSPhraseur&
NSPhraseur::operator=(const NSPhraseur& src)
{
try
{
	if (this == &src)
		return *this ;

	if (pParentheses)
  {
  	delete pParentheses ;
    pParentheses = (NSPhraseur*) 0 ;
  }

  if (pDeuxPoints)
  {
  	delete pDeuxPoints ;
    pDeuxPoints = (NSPhraseur*) 0 ;
  }

  initialise() ;
  copieTout(&src) ;

	if (src.pParentheses)
  	pParentheses = new NSPhraseur(*(src.pParentheses)) ;

	if (src.pDeuxPoints)
  	pDeuxPoints = new NSPhraseur(*(src.pDeuxPoints)) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSPhraseur = operator.", standardError) ;
	return *this ;
}
}

void
NSPhraseur::initialise()
{
  iForme         = formeActive ;
  iPhraseType    = phrasePrincipale ;
  _iRelativeType = dcUndefined ;

  Sujet.vider() ;

  Verbe.vider() ;
  iVbType     = vbTypeNotInit ;
  iVbEtat     = vbEtatNotInit ;
  iVbTemps    = tempsPresent ;
  iVbAspect   = aspectPonctuel ;
  iVbMode     = modeIndicatif ;
  iVbPersonne = pers3S ;
  iVbNegation = negationNon ;

  iTypeSujet  = sujetNormal ;

  adjEpithete.vider() ;
  adjEpitheteAv.vider() ;
  adjEpitheteAp.vider() ;

  compNom.vider() ;
  adverbe.vider() ;

  COD.vider() ;
  COI.vider() ;
  COS.vider() ;
  AttSujet.vider() ;
  AttCOD.vider() ;

  pParentheses      = (NSPhraseur*) 0 ;
  iParenthesesStyle = dcUndefined ;

  LiaisonLieu.metAZero() ;
  PrepositionLieu.metAZero() ;
  iTypeLocalisation = locUndefined ;
  CCLieu.vider() ;
  LiaisonTemps.metAZero() ;
  PrepositionTemps.metAZero() ;
  CCTemps.vider() ;
  LiaisonManiere.metAZero() ;
  PrepositionManiere.metAZero() ;
  CCManiere.vider() ;
  LiaisonMoyen.metAZero() ;
  PrepositionMoyen.metAZero() ;
  CCMoyen.vider() ;
  LiaisonCause.metAZero() ;
  PrepositionCause.metAZero() ;
  CCCause.vider() ;
  LiaisonBut.metAZero() ;
  PrepositionBut.metAZero() ;
  CCBut.vider() ;
  LiaisonType.metAZero() ;
  PrepositionType.metAZero() ;
  CCType.vider() ;
  LiaisonHypoth.metAZero() ;
  PrepositionHypoth.metAZero() ;
  CCHypoth.vider() ;
  LiaisonChiffre.metAZero() ;
  PrepositionChiffre.metAZero() ;
  CCChiffre.vider() ;

  adjNumeralOrdinal.metAZero() ;
  adjNumeralCardinal.metAZero() ;
  adjNonQualificatif = adjectifSans ;

  pDeuxPoints = (NSPhraseur*) 0 ;
}

void
NSPhraseur::copieTout(const NSPhraseur* pSrc)
{
  iPhraseType         = pSrc->iPhraseType ;
  _iRelativeType      = pSrc->_iRelativeType ;
  iForme              = pSrc->iForme ;
  Sujet               = pSrc->Sujet ;
  iTypeSujet          = pSrc->iTypeSujet ;
  //
  Verbe               = pSrc->Verbe ;
  iVbType             = pSrc->iVbType ;
  iVbEtat             = pSrc->iVbEtat ;
  iVbTemps            = pSrc->iVbTemps ;
  iVbAspect           = pSrc->iVbAspect ;
  iVbMode             = pSrc->iVbMode ;
  iVbPersonne         = pSrc->iVbPersonne ;
  iVbNegation         = pSrc->iVbNegation ;
  //
  adjEpithete         = pSrc->adjEpithete ;
  adjEpitheteAv       = pSrc->adjEpitheteAv ;
  adjEpitheteAp       = pSrc->adjEpitheteAp ;
  adjNumeralOrdinal   = pSrc->adjNumeralOrdinal ;
  adjNumeralCardinal  = pSrc->adjNumeralCardinal ;
  adjNonQualificatif  = pSrc->adjNonQualificatif ;

  // Complement du nom
  compNom             = pSrc->compNom ;

  // Adverbes
  adverbe             = pSrc->adverbe ;

  // Complements essentiels
  COD                 = pSrc->COD ;
  COI                 = pSrc->COI ;
  COS                 = pSrc->COS ;
  AttSujet            = pSrc->AttSujet ;
  AttCOD              = pSrc->AttCOD ;
  // Complements circonstanciels
  iTypeLocalisation   = pSrc->iTypeLocalisation ;
  LiaisonLieu         = pSrc->LiaisonLieu ;
  PrepositionLieu     = pSrc->PrepositionLieu ;
  CCLieu              = pSrc->CCLieu ;
  LiaisonTemps        = pSrc->LiaisonTemps ;
  PrepositionTemps    = pSrc->PrepositionTemps ;
  CCTemps             = pSrc->CCTemps ;
  LiaisonManiere      = pSrc->LiaisonManiere ;
  PrepositionManiere  = pSrc->PrepositionManiere ;
  CCManiere           = pSrc->CCManiere ;
  LiaisonMoyen        = pSrc->LiaisonMoyen ;
  PrepositionMoyen    = pSrc->PrepositionMoyen ;
  CCMoyen             = pSrc->CCMoyen ;
  LiaisonCause        = pSrc->LiaisonCause ;
  PrepositionCause    = pSrc->PrepositionCause ;
  CCCause             = pSrc->CCCause ;
  LiaisonBut          = pSrc->LiaisonBut ;
  PrepositionBut      = pSrc->PrepositionBut ;
  CCBut               = pSrc->CCBut ;
  LiaisonType         = pSrc->LiaisonType ;
  PrepositionType     = pSrc->PrepositionType ;
  CCType              = pSrc->CCType ;
  LiaisonHypoth       = pSrc->LiaisonHypoth ;
  PrepositionHypoth   = pSrc->PrepositionHypoth ;
  CCHypoth            = pSrc->CCHypoth ;

  LiaisonChiffre      = pSrc->LiaisonChiffre ;
  PrepositionChiffre  = pSrc->PrepositionChiffre ;
  CCChiffre           = pSrc->CCChiffre ;

  iParenthesesStyle   = pSrc->iParenthesesStyle ;
}

void
NSPhraseur::initParentheses()
{
try
{
	if (pParentheses)
  	pParentheses->initialise() ;
	else
  	pParentheses = new NSPhraseur(pContexte) ;

	pParentheses->iPhraseType = phraseComplement ;
}
catch (...)
{
	erreur("Exception NSPhraseur::initParentheses.", standardError) ;
}
}

void
NSPhraseur::initDeuxPoints()
{
try
{
	if (pDeuxPoints)
  	pDeuxPoints->initialise() ;
	else
  	pDeuxPoints = new NSPhraseur(pContexte) ;
}
catch (...)
{
	erreur("Exception NSPhraseur::initDeuxPoints.", standardError) ;
}
}

//--------------------------------------------------------------------------
// Le nettoyeur de prepositions
//--------------------------------------------------------------------------

void
NSPhraseur::nettoye()
{
  adjEpithete.initPreposition() ;
  adjEpitheteAv.initPreposition() ;
  adjEpitheteAp.initPreposition() ;
  adjNumeralOrdinal.metAZero() ;
  adjNumeralCardinal.metAZero() ;
  adjNonQualificatif = adjectifSans ;
  compNom.initPreposition() ;
  adverbe.initPreposition() ;
  COD.initPreposition() ;
  COI.initPreposition() ;
  COS.initPreposition() ;
  AttSujet.initPreposition() ;
  AttCOD.initPreposition() ;
  iTypeLocalisation = locUndefined ;
  CCLieu.initPreposition() ;
  CCTemps.initPreposition() ;
  CCManiere.initPreposition() ;
  CCMoyen.initPreposition() ;
  CCCause.initPreposition() ;
  CCBut.initPreposition() ;
  CCType.initPreposition() ;
  CCHypoth.initPreposition() ;
  CCChiffre.initPreposition() ;

  if (pDeuxPoints)
    pDeuxPoints->nettoye() ;
  if (pParentheses)
    pParentheses->nettoye() ;
}

void
NSPhraseur::classeAdjectif(NSGenerateur* pGener)
{
try
{
  if ((NSGenerateur*) NULL == pGener)
    return ;

  adjEpitheteAv.vider() ;
  adjEpitheteAp.vider() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  if (adjEpithete.empty())
    return ;

  adjEpitheteAv.setParameters(&adjEpithete) ;
  adjEpitheteAp.setParameters(&adjEpithete) ;

  for (iterPhraseMot iterMots = adjEpithete.begin() ; adjEpithete.end() != iterMots ; iterMots++)
  {
    string sLexique = (*iterMots)->getLexique() ;

    if (false == (*iterMots)->estTexteLibre())
    {
      NSPathologData Data ;
      bool bFound = pContexte->getDico()->trouvePathologData(pGener->getLang(), &sLexique, &Data) ;
      if (false == bFound)
      {
        string sErrorText = string("NSPhraseur::classeAdjectif discarding ") + sLexique + string(" which cannot be found in Lexique for langage ") + pGener->getLang() ;
        pSuper->trace(&sErrorText, 1, NSSuper::trWarning) ;
      }
      if (false == Data.estAdjectif())
      {
        string sErrorText = string("NSPhraseur::classeAdjectif discarding ") + sLexique + string(" because an epithete must be an adjective.") ;
        pSuper->trace(&sErrorText, 1, NSSuper::trWarning) ;
        bFound = false ;
      }

      if (bFound)
      {
        string sPos = Data.chercheGrammaire(ADJ_POSITION) ;

        if      ((*iterMots)->getPosition() == NSPhraseMot::posAvant)
          adjEpitheteAv.push_back(new NSPhraseMot(*(*iterMots))) ;
        else if ((*iterMots)->getPosition() == NSPhraseMot::posApres)
          adjEpitheteAp.push_back(new NSPhraseMot(*(*iterMots))) ;
        else
        {
          if (AV_NOM == sPos)
            adjEpitheteAv.push_back(new NSPhraseMot(*(*iterMots))) ;
          else if ((AP_NOM == sPos) || (string("") == sPos))
            adjEpitheteAp.push_back(new NSPhraseMot(*(*iterMots))) ;
        }
      }
    }
    //
    // On met les adjectifs libres apres le nom.
    //
    else
      adjEpitheteAp.push_back(new NSPhraseMot(*(*iterMots))) ;
  }
}
catch (...)
{
  erreur("Exception NSPhraseur::classeAdjectif.", standardError) ;
}
}

void
NSPhraseur::addGrammatical(const string sEtiquette, NSPhraseMotArray* pBlock)
{
  NSPhraseMot Element(pContexte) ;
  Element.InitFromEtiquette(sEtiquette) ;
  addGrammatical(&Element, pBlock) ;
}

void
NSPhraseur::addGrammatical(const NSPhraseMot* pElement, NSPhraseMotArray* pBlock)
{
  if (((NSPhraseMot*) NULL == pElement) || ((NSPhraseMotArray*) NULL == pBlock))
    return ;

  // NSPhraseMotTime ?
  //
  const NSPhraseMotTime* pDate = dynamic_cast<const NSPhraseMotTime*>(pElement) ;
  if (pDate)
  {
    NSPhraseMotTime *pTime = new NSPhraseMotTime(*pDate) ;
    pBlock->push_back(pTime) ;
    return ;
  }

  // NSPhraseMotTimeCycle ?
  //
  const NSPhraseMotTimeCycle* pCycle = dynamic_cast<const NSPhraseMotTimeCycle*>(pElement) ;
  if (pCycle)
  {
    NSPhraseMotTimeCycle *pTimeCycle = new NSPhraseMotTimeCycle(*pCycle) ;
    pBlock->push_back(pTimeCycle) ;
    return ;
  }

  pBlock->push_back(new NSPhraseMot(*pElement)) ;
}

//***************************************************************************
// Implementation des methodes NSPhraseMot
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par defaut
//---------------------------------------------------------------------------
NSPhraseMot::NSPhraseMot(NSContexte* pCtx, string sEtiquette)
            :NSRoot(pCtx)
{
  InitFromEtiquette(sEtiquette) ;

	_pComplements = (NSPhraseurArray*) 0 ;

  _pLexiqueData = (NSPathologData*) 0 ;
  _sLangForData = string("") ;
}

//---------------------------------------------------------------------------
//  Met les champs de donnees a zero
//---------------------------------------------------------------------------
void
NSPhraseMot::metAZero()
{
  _sPreposition  = string("") ;
  _sComposer     = string("") ;
  _iComposerPos  = posStandard ;
  _iComposerLink = linkUndefined ;
  _sTexteLibre   = string("") ;
  _avecArticle   = articleSans ;
  _posForcee     = posStandard ;
  _numForme      = numSimple ;

  _sLexique      = string("") ;
  _sComplement   = string("") ;
  _sCertitude    = string("") ;
  _sPluriel      = string("") ;

  _sFormat       = string("") ;
  _sMethode      = string("") ;
  _sUnite        = string("") ;

  _iComplementManagement = complementNormal ;
  _iTypeAsComplement     = cpltUndefined ;

  _iPriorite     = 50 ;  // standard priority

  _sEnumCategory = string("") ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseMot::NSPhraseMot(const NSPhraseMot& rv)
            :NSRoot(rv.pContexte)
{
try
{
	_sPreposition  = rv._sPreposition ;
  _sComposer     = rv._sComposer ;
  _iComposerPos  = rv._iComposerPos ;
  _iComposerLink = rv._iComposerLink ;
	_sTexteLibre   = rv._sTexteLibre ;
	_avecArticle   = rv._avecArticle ;
	_posForcee     = rv._posForcee ;
	_numForme      = rv._numForme ;

  _sLexique      = rv._sLexique ;
  _sComplement   = rv._sComplement ;
  _sCertitude    = rv._sCertitude ;
  _sPluriel      = rv._sPluriel ;

  _sFormat       = rv._sFormat ;
  _sMethode      = rv._sMethode ;
  _sUnite        = rv._sUnite ;

  _iPriorite     = rv._iPriorite ;

  _sEnumCategory = rv._sEnumCategory ;

	if (rv._pComplements)
  	_pComplements = new NSPhraseurArray(*(rv._pComplements)) ;
  else
  	_pComplements = (NSPhraseurArray*) 0 ;

  if (rv._pLexiqueData)
  {
    _pLexiqueData = new NSPathologData(*(rv._pLexiqueData)) ;
    _sLangForData = rv._sLangForData ;
  }
  else
  {
    _pLexiqueData = (NSPathologData*) 0 ;
    _sLangForData = string("") ;
  }

  _iComplementManagement = rv._iComplementManagement ;
  _iTypeAsComplement     = rv._iTypeAsComplement ;
}
catch (...)
{
	erreur("Exception NSPhraseMot copy ctor.", standardError) ;
}
}

/*
//---------------------------------------------------------------------------
//  Constructeur a partir d'un pointeur de PathoBase
//---------------------------------------------------------------------------
 NSPhraseMot::NSPhraseMot(NSPathoBaseData* pPathoBaseElement, NSContexte* pCtx)
            :NSRoot(pCtx)
{
	metAZero();

    strcpy(lexique,    pPathoBaseElement->lexique);
	strcpy(complement, pPathoBaseElement->complement);
	strcpy(certitude,  pPathoBaseElement->certitude);
	strcpy(pluriel,    pPathoBaseElement->pluriel);

    sTexteLibre = pPathoBaseElement->sTexteLibre;

    pComplement = 0;
}
*/

//---------------------------------------------------------------------------
//  Constructeur a partir d'un pointeur de PatPatho
//---------------------------------------------------------------------------

NSPhraseMot::NSPhraseMot(const NSPatPathoData* pPatPathoElement, NSContexte* pCtx)
            :NSRoot(pCtx)
{
	metAZero() ;

  _sLexique    = pPatPathoElement->getLexique() ;
	_sComplement = pPatPathoElement->getComplement() ;
	_sCertitude  = pPatPathoElement->getCertitude() ;
  _sPluriel    = pPatPathoElement->getPluriel() ;
  _sUnite      = pPatPathoElement->getUnit() ;

  _sTexteLibre = pPatPathoElement->getTexteLibre() ;

  _pComplements = (NSPhraseurArray*) 0 ;

  _pLexiqueData = (NSPathologData*) 0 ;
  _sLangForData = string("") ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPhraseMot::~NSPhraseMot()
{
  if (_pComplements)
    delete _pComplements ;

  if (_pLexiqueData)
    delete _pLexiqueData ;
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------
NSPhraseMot&
NSPhraseMot::operator=(const NSPhraseMot& src)
{
try
{
	if (this == &src)
		return *this ;

	metAZero() ;

  _sPreposition  = src._sPreposition ;
  _sComposer     = src._sComposer ;
  _iComposerPos  = src._iComposerPos ;
  _iComposerLink = src._iComposerLink ;
  _sTexteLibre   = src._sTexteLibre ;
  _avecArticle   = src._avecArticle ;
  _posForcee     = src._posForcee ;
  _numForme      = src._numForme ;

  _sLexique      = src._sLexique ;
  _sComplement   = src._sComplement ;
  _sCertitude    = src._sCertitude ;
  _sPluriel      = src._sPluriel ;

  _sFormat       = src._sFormat ;
  _sMethode      = src._sMethode ;
  _sUnite        = src._sUnite ;

  _iPriorite     = src._iPriorite ;

  _sEnumCategory = src._sEnumCategory ;

  if (_pComplements)
  {
  	delete _pComplements ;
    _pComplements = (NSPhraseurArray*) 0 ;
  }

  if (src._pComplements)
  	_pComplements = new NSPhraseurArray(*(src._pComplements)) ;

  if (_pLexiqueData)
  {
    delete _pLexiqueData ;
    _pLexiqueData = (NSPathologData*) 0 ;
  }

  if (src._pLexiqueData)
  {
  	_pLexiqueData = new NSPathologData(*(src._pLexiqueData)) ;
    _sLangForData = src._sLangForData ;
  }
  else
    _sLangForData = string("") ;

  _iComplementManagement = src._iComplementManagement ;
  _iTypeAsComplement     = src._iTypeAsComplement ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSPhraseMot = operator.", standardError) ;
	return *this ;
}
}

//---------------------------------------------------------------------------
//  Operateur de comparaison
//---------------------------------------------------------------------------
int
NSPhraseMot::operator==(const NSPhraseMot& o)
{
	if ((_sLexique    == o._sLexique)    &&
      (_sComplement == o._sComplement) &&
      (_sCertitude  == o._sCertitude)  &&
      (_sPluriel    == o._sPluriel)    &&
      (_sFormat     == o._sFormat)     &&
      (_sMethode    == o._sMethode)    &&
      (_sUnite      == o._sUnite)      &&
      (_sTexteLibre == o._sTexteLibre))
		return 1 ;
	else
		return 0 ;
}

void
NSPhraseMot::InitFromEtiquette(const string sEtiquette)
{
  metAZero() ;

  if (string("") == sEtiquette)
    return ;

  Message Msg ;
  Msg.InitFromEtiquette(sEtiquette) ;

  _sLexique    = Msg.GetLexique() ;
	_sComplement = Msg.GetComplement() ;
	_sCertitude  = Msg.GetCertitude() ;
  _sPluriel    = Msg.GetPluriel() ;
  _sUnite      = Msg.GetUnit() ;

  _sTexteLibre = Msg.GetTexteLibre() ;
}

void
NSPhraseMot::initFromNum(const gereNum* pNum)
{
  if ((gereNum*) NULL == pNum)
    return ;

  setComplement(pNum->getNum()) ;
  setUnite(pNum->getUnite()) ;
  setFormat(pNum->getFormat()) ;
}

void
NSPhraseMot::initComplement()
{
try
{
	if (_pComplements)
  	_pComplements->vider() ;
  else
  	_pComplements = new NSPhraseurArray() ;
}
catch (...)
{
	erreur("Exception NSPhraseMot::initComplement.", standardError) ;
}
}

void
NSPhraseMot::addComplementPhr(NSPhraseur* pCompl, PHRTYPE iPhraseType)
{
  if ((NSPhraseur*) NULL == pCompl)
    return ;

  if ((NSPhraseurArray*) NULL == _pComplements)
  	_pComplements = new NSPhraseurArray() ;

  pCompl->iPhraseType = iPhraseType ;

  _pComplements->push_back(pCompl) ;
}

bool
NSPhraseMot::estTexteLibre() const
{
  return (string("£?????") == _sLexique) ;
}

string
NSPhraseMot::forceNombre(NSGenerateur* pGener)
{
  if ((NSGenerateur*) NULL == pGener)
    return string("") ;

  iterPhraseMot  iterMots ;
  NSPathologData Data ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // On regarde la certitude. Dans le cas d'un nom, une certitude WCE00 force le
  // singulier.

  string sCertitude  = _sCertitude ;

  string sLexiqueNom = _sLexique ;
  bool bNounFound = pContexte->getDico()->trouvePathologData(pGener->getLang(), &sLexiqueNom, &Data) ;
  if (false == bNounFound)
  {
    string sErrorText = string("NSPhraseMot::forceNombre Fail bacause ") + sLexiqueNom + string(" cannot be found in Lexique for langage ") + pGener->getLang() ;
    pSuper->trace(&sErrorText, 1, NSSuper::trWarning) ;
    return string("") ;
  }

/*
  if (Data.estNom())
  {
    if (string("") != sCertitude)
    {
      if (string(sCertitude, 0, 5) == "WCE00")
        return FORCE_SING ;
    }
  }
*/

  // On regarde les complements

  if (_pComplements && (false == _pComplements->empty()))
  {
    for (phraseurIter CpltIt = _pComplements->begin() ;
                                  _pComplements->end() != CpltIt ; CpltIt++)
    {
      if ((*CpltIt)->adjNumeralCardinal.getComplement() != string(""))
      {
        gereNum Nnombre(pContexte->getSuperviseur(), pGener->getLang()) ;

        string sComplement = (*CpltIt)->adjNumeralCardinal.getComplement() ;
        string sUnite      = (*CpltIt)->adjNumeralCardinal.getUnite() ;
        string sFormat     = (*CpltIt)->adjNumeralCardinal.getFormat() ;

        Nnombre.instancier(&sComplement, &sUnite, &sFormat) ;

        if (Nnombre.estExact())
        {
          double dRetour = Nnombre.getValeur() ;
          if (dRetour < 2)
            return FORCE_SING ;
          return FORCE_PLUR ;
        }
      }

      // On verifie si les adjectifs forcent le nombre

      if (false == ((*CpltIt)->adjEpithete).empty())
      {
        for (iterMots = ((*CpltIt)->adjEpithete).begin() ;
                ((*CpltIt)->adjEpithete).end() != iterMots ;
                    iterMots++)
        {
          string sLexique = (*iterMots)->getLexique() ;
          bool bFound = pContexte->getDico()->trouvePathologData(pGener->getLang(), &sLexique, &Data) ;

          if (bFound)
          {
            string sRetour = Data.chercheGrammaire(FORCE_NBR) ;
            if (string("") != sRetour)
              return sRetour ;
          }
        }
      }
      // On regarde les CCChiffre

      if (false == ((*CpltIt)->CCChiffre).empty())
      {
        for (iterMots = ((*CpltIt)->CCChiffre).begin() ;
                ((*CpltIt)->CCChiffre).end() != iterMots ;
                    iterMots++)
        {
          string sLexique = (*iterMots)->getLexique() ;
          bool bFound = pContexte->getDico()->trouvePathologData(pGener->getLang(), &sLexique, &Data) ;

          if (bFound)
          {
            string sRetour = Data.chercheGrammaire(FORCE_NBR);
            if      (FORCE_PLUR == sRetour)
              return FORCE_PLUR ;
            else if (FORCE_SING == sRetour)
              return FORCE_SING ;
            else if (FORCE_X    == sRetour)
            {
              gereNum Nnombre(pContexte->getSuperviseur(), pGener->getLang()) ;

              string sComplement = (*CpltIt)->adjNumeralCardinal.getComplement() ;
              string sUnite      = (*CpltIt)->adjNumeralCardinal.getUnite() ;
              string sFormat     = (*CpltIt)->adjNumeralCardinal.getFormat() ;

              Nnombre.instancier(&sComplement, &sUnite, &sFormat);
              if (Nnombre.estExact())
              {
                double dRetour = Nnombre.getValeur() ;
                if (dRetour < 2)
                  return FORCE_SING ;
                return FORCE_PLUR ;
              }

              if (Nnombre.estInf())
              {
                double dRetour = Nnombre.getValeurInf() ;
                if (dRetour >= 2)
                  return FORCE_PLUR ;
              }

              if (Nnombre.estSup())
              {
                double dRetour = Nnombre.getValeurSup() ;
                if (dRetour >= 2)
                  return FORCE_PLUR ;
                else if (dRetour < 2)
                  return FORCE_SING ;
              }
            }
          }
        }
      }
    }
  }
  return string("") ;
}

NSPhraseur*
NSPhraseMot::getFirstComplementPhr()
{
  if (((NSPhraseurArray*) NULL == _pComplements) || _pComplements->empty())
    return (NSPhraseur*) 0 ;

  return *(_pComplements->begin()) ;
}

NSPhraseur*
NSPhraseMot::getOrCreateFirstComplementPhr()
{
  if ((NSPhraseurArray*) NULL == _pComplements)
    initComplement() ;

  NSPhraseur* pFirstCplt = getFirstComplementPhr() ;
  if (pFirstCplt)
    return pFirstCplt ;

  _pComplements->push_back(new NSPhraseur(pContexte)) ;

  return getFirstComplementPhr() ;
}

bool
NSPhraseMot::estVide() const
{
  if ((string("") == _sLexique) && ((NSPhraseurArray*) NULL == _pComplements))
    return true ;
  return false ;
}

NSPathologData*
NSPhraseMot::getLexiqueData(string sLang)
{
  if (_pLexiqueData && (sLang == _sLangForData))
    return _pLexiqueData ;

  if ((NSPathologData*) NULL == _pLexiqueData)
    _pLexiqueData = new NSPathologData() ;

  if (false == pContexte->getDico()->trouvePathologData(sLang, &_sLexique, _pLexiqueData))
  {
    delete _pLexiqueData ;
    _pLexiqueData = (NSPathologData*) 0 ;
    return (NSPathologData*) 0 ;
  }

  _sLangForData = sLang ;

  return _pLexiqueData ;
}

//***************************************************************************
// Implementation des methodes NSPhraseMotTime
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par defaut
//---------------------------------------------------------------------------
NSPhraseMotTime::NSPhraseMotTime(NSContexte* pCtx)
                :NSPhraseMot(pCtx)
{
	metAZero() ;
  _pComplements = (NSPhraseurArray*) 0 ;
}

//---------------------------------------------------------------------------
//  Met les champs de donnees a zero
//---------------------------------------------------------------------------
void
NSPhraseMotTime::metAZero()
{
  NSPhraseMot::metAZero() ;

  _sValeurMin   = string("") ;
  _sUniteMin    = string("") ;
  _sFormatMin   = string("") ;
  _sValeurMax   = string("") ;
  _sUniteMax    = string("") ;
  _sFormatMax   = string("") ;
  _sValeurDuree = string("") ;
  _sUniteDuree  = string("") ;
  _sFormatDuree = string("") ;

  _bMinNow = false ;
  _bMaxNow = false ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseMotTime::NSPhraseMotTime(const NSPhraseMotTime& rv)
                :NSPhraseMot((NSPhraseMot&)rv)
{
  _sValeurMin   = rv._sValeurMin ;
	_sUniteMin    = rv._sUniteMin ;
  _sFormatMin   = rv._sFormatMin ;
	_sValeurMax   = rv._sValeurMax ;
	_sUniteMax    = rv._sUniteMax ;
  _sFormatMax   = rv._sFormatMax ;
  _sValeurDuree = rv._sValeurDuree ;
  _sUniteDuree  = rv._sUniteDuree ;
  _sFormatDuree = rv._sFormatDuree ;

  _bMinNow = rv._bMinNow ;
  _bMaxNow = rv._bMaxNow ;
}

//---------------------------------------------------------------------------
//  Constructeur a partir d'un pointeur de PatPatho
//---------------------------------------------------------------------------

NSPhraseMotTime::NSPhraseMotTime(const NSPatPathoData* pPatPathoElement, NSContexte* pCtx)
                :NSPhraseMot(pPatPathoElement, pCtx)
{
  metAZero() ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPhraseMotTime::~NSPhraseMotTime()
{
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------
NSPhraseMotTime&
NSPhraseMotTime::operator=(const NSPhraseMotTime& src)
{
  if (this == &src)
		return *this ;

  NSPhraseMot* pMot = (NSPhraseMot*) this ;
  *pMot = (NSPhraseMot&) src ;

  _sValeurMin   = src._sValeurMin ;
	_sUniteMin    = src._sUniteMin ;
  _sFormatMin   = src._sFormatMin ;
	_sValeurMax   = src._sValeurMax ;
	_sUniteMax    = src._sUniteMax ;
  _sFormatMax   = src._sFormatMax ;
  _sValeurDuree = src._sValeurDuree ;
  _sUniteDuree  = src._sUniteDuree ;
  _sFormatDuree = src._sFormatDuree ;


  _bMinNow = src._bMinNow ;
  _bMaxNow = src._bMaxNow ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Operateur de comparaison
//---------------------------------------------------------------------------
int
NSPhraseMotTime::operator==(const NSPhraseMotTime& o)
{
  NSPhraseMot* pMot = (NSPhraseMot*) this ;
  if (!(*pMot == (NSPhraseMot&) o))
    return 0 ;

	if ((_sValeurMin   == o._sValeurMin)   &&
      (_sUniteMin    == o._sUniteMin)    &&
      (_sFormatMin   == o._sFormatMin)   &&
      (_sValeurMax   == o._sValeurMax)   &&
      (_sUniteMax    == o._sUniteMax)    &&
      (_sFormatMax   == o._sFormatMax)   &&
      (_sValeurDuree == o._sValeurDuree) &&
      (_sUniteDuree  == o._sUniteDuree)  &&
      (_sFormatDuree == o._sFormatDuree) &&
      (_bMinNow == o._bMinNow) &&
      (_bMaxNow == o._bMaxNow))
		return 1 ;
	else
		return 0 ;
}

void
NSPhraseMotTime::setSharpDate()
{
  _sValeurMax = _sValeurMin ;
  _sUniteMax  = _sUniteMin ;
  _sFormatMax = _sFormatMin ;
}

bool
NSPhraseMotTime::minEgalMax()
{
  if ((_sValeurMax == _sValeurMin) &&
      (_sUniteMax  == _sUniteMin)  &&
      (_sFormatMax == _sFormatMin))
    return true ;

  return false ;
}

bool
NSPhraseMotTime::estVide()
{
  if ((string("") == _sValeurMin) &&
      (string("") == _sValeurMax) &&
      (string("") == _sValeurDuree))
    return true ;
  return false ;
}

void
NSPhraseMotTime::initTime(NSPatPathoInfo* pPatho, string &sValue, string &sUnit, string &sFormat)
{
  if ((NSPatPathoInfo*) NULL == pPatho)
    return ;

  sValue  = pPatho->getComplement() ;
  sUnit   = pPatho->getUnit() ;
  sFormat = pPatho->getLexique() ;
}

void
NSPhraseMotTime::initDuree(NSPatPathoInfo* pPatho)
{
  initTime(pPatho, _sValeurDuree, _sUniteDuree, _sFormatDuree) ;
}

void
NSPhraseMotTime::initMin(NSPatPathoInfo* pPatho)
{
  initTime(pPatho, _sValeurMin, _sUniteMin, _sFormatMin) ;
}

void
NSPhraseMotTime::initMax(NSPatPathoInfo* pPatho)
{
  initTime(pPatho, _sValeurMax, _sUniteMax, _sFormatMax) ;
}

void
NSPhraseMotTime::initTime(gereDate* pDate, string &sValue, string &sUnit, string &sFormat)
{
  if ((gereDate*) NULL == pDate)
    return ;

  sValue  = pDate->getDate() ;
  sUnit   = pDate->getUnit() ;
  sFormat = pDate->getFormat() ;
}

void
NSPhraseMotTime::initMin(gereDate* pDate)
{
  initTime(pDate, _sValeurMin, _sUniteMin, _sFormatMin) ;
}

void
NSPhraseMotTime::initMax(gereDate* pDate)
{
  initTime(pDate, _sValeurMax, _sUniteMax, _sFormatMax) ;
}

void
NSPhraseMotTime::initTime(gereHeure* pHeure, string &sValue, string &sUnit, string &sFormat)
{
  if ((gereHeure*) NULL == pHeure)
    return ;

  sValue  = pHeure->getHeure() ;
  sUnit   = pHeure->getUnit() ;
  sFormat = pHeure->getFormat() ;
}

void
NSPhraseMotTime::initDuree(gereHeure* pHeure)
{
  initTime(pHeure, _sValeurDuree, _sUniteDuree, _sFormatDuree) ;
}

void
NSPhraseMotTime::initMin(gereHeure* pHeure)
{
  initTime(pHeure, _sValeurMin, _sUniteMin, _sFormatMin) ;
}

void
NSPhraseMotTime::initMax(gereHeure* pHeure)
{
  initTime(pHeure, _sValeurMax, _sUniteMax, _sFormatMax) ;
}

//***************************************************************************
// Implementation des methodes NSPhraseMotTimeCycle
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par defaut
//---------------------------------------------------------------------------
NSPhraseMotTimeCycle::NSPhraseMotTimeCycle(NSContexte* pCtx)
                     :NSPhraseMot(pCtx)
{
	metAZero() ;
  _pComplements = (NSPhraseurArray*) 0 ;
}

//---------------------------------------------------------------------------
//  Met les champs de donnees a zero
//---------------------------------------------------------------------------
void
NSPhraseMotTimeCycle::metAZero()
{
  NSPhraseMot::metAZero() ;

  _iCycleType = CycNotInit ;

  _sCycleDurationValue   = string("") ;
  _sCycleDurationUnit    = string("") ;
  _sCycleDurationFormat  = string("") ;

  _sActionDurationValue  = string("") ;
  _sActionDurationUnit   = string("") ;
  _sActionDurationFormat = string("") ;

  _sNumberOfAction       = string("") ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseMotTimeCycle::NSPhraseMotTimeCycle(const NSPhraseMotTimeCycle& rv)
                     :NSPhraseMot((NSPhraseMot&)rv)
{
  _iCycleType = rv._iCycleType ;

	_sCycleDurationValue   = rv._sCycleDurationValue ;
	_sCycleDurationUnit    = rv._sCycleDurationUnit ;
  _sCycleDurationFormat  = rv._sCycleDurationFormat ;

	_sActionDurationValue  = rv._sActionDurationValue ;
	_sActionDurationUnit   = rv._sActionDurationUnit ;
  _sActionDurationFormat = rv._sActionDurationFormat ;

  _sNumberOfAction       = rv._sNumberOfAction ;
}

//---------------------------------------------------------------------------
//  Constructeur a partir d'un pointeur de PatPatho
//---------------------------------------------------------------------------

NSPhraseMotTimeCycle::NSPhraseMotTimeCycle(const NSPatPathoData* pPatPathoElement, NSContexte* pCtx)
                     :NSPhraseMot(pPatPathoElement, pCtx)
{
	metAZero() ;
}


//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPhraseMotTimeCycle::~NSPhraseMotTimeCycle()
{
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------
NSPhraseMotTimeCycle&
NSPhraseMotTimeCycle::operator=(const NSPhraseMotTimeCycle& src)
{
  if (this == &src)
		return *this ;

  NSPhraseMot* pMot = (NSPhraseMot*) this ;
  *pMot = (NSPhraseMot&) src ;

  _iCycleType = src._iCycleType ;

	_sCycleDurationValue   = src._sCycleDurationValue ;
	_sCycleDurationUnit    = src._sCycleDurationUnit ;
  _sCycleDurationFormat  = src._sCycleDurationFormat ;

	_sActionDurationValue  = src._sActionDurationValue ;
	_sActionDurationUnit   = src._sActionDurationUnit ;
  _sActionDurationFormat = src._sActionDurationFormat ;

  _sNumberOfAction       = src._sNumberOfAction ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Operateur de comparaison
//---------------------------------------------------------------------------
int
NSPhraseMotTimeCycle::operator==(const NSPhraseMotTimeCycle& o)
{
  NSPhraseMot* pMot = (NSPhraseMot*) this ;
  if (!(*pMot == (NSPhraseMot&) o))
    return 0 ;

	if ((_sCycleDurationValue   == o._sCycleDurationValue)   &&
      (_sCycleDurationUnit    == o._sCycleDurationUnit)    &&
      (_sCycleDurationFormat  == o._sCycleDurationFormat)  &&
      (_sActionDurationValue  == o._sActionDurationValue)  &&
      (_sActionDurationUnit   == o._sActionDurationUnit)   &&
      (_sActionDurationFormat == o._sActionDurationFormat) &&
      (_sNumberOfAction       == o._sNumberOfAction)       &&
      (_iCycleType == o._iCycleType))
		return 1 ;
	else
		return 0 ;
}

// *************************************************************************
// -------------------- METHODES DE NSPhraseMotArray ------------------------
// **************************************************************************

NSPhraseMotArray::NSPhraseMotArray()
                 :NSPhraMotArray()
{
  _iGroupType        = groupUnspecified ;
  _iGroupSeparator   = groupSeparComaAnd ;
  _sEnumerationOrder = string("") ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseMotArray::NSPhraseMotArray(const NSPhraseMotArray& rv)
                 :NSPhraMotArray()
{
  _iGroupType        = rv._iGroupType ;
  _iGroupSeparator   = rv._iGroupSeparator ;
  _sEnumerationOrder = rv._sEnumerationOrder ;

try
{
  if (rv.empty())
    return ;

  for (iterConstPhraseMot i = rv.begin(); i != rv.end(); i++)
  {
    NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(*i) ;
    if ( pDate )
    {
      NSPhraseMotTime* pNewDate = new NSPhraseMotTime(*pDate) ;
      push_back(pNewDate) ;
    }
    else
    {
      NSPhraseMotTimeCycle* pCycle = dynamic_cast<NSPhraseMotTimeCycle*>(*i) ;
      if ( pCycle )
      {
        NSPhraseMotTimeCycle *pNewCycle = new NSPhraseMotTimeCycle(*pCycle) ;
        push_back(pNewCycle) ;
      }
      else
        push_back(new NSPhraseMot(*(*i))) ;
    }
  }
}
catch (...)
{
	erreur("Exception NSPhraseMotArray copy ctor.", standardError) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSPhraseMotArray::vider()
{
  if (empty())
    return ;

  for (iterPhraseMot i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSPhraseMotArray::~NSPhraseMotArray()
{
  vider() ;
}

void
NSPhraseMotArray::setParameters(const NSPhraseMotArray* pModel)
{
  if ((NSPhraseMotArray*) NULL == pModel)
    return ;

  _iGroupType        = pModel->_iGroupType ;
  _iGroupSeparator   = pModel->_iGroupSeparator ;
  _sEnumerationOrder = pModel->_sEnumerationOrder ;
}

//---------------------------------------------------------------------------
//  Operateur d'affectation
//---------------------------------------------------------------------------
NSPhraseMotArray&
NSPhraseMotArray::operator=(const NSPhraseMotArray& src)
{
try
{
	if (this == &src)
		return *this ;

  _iGroupType        = src._iGroupType ;
  _iGroupSeparator   = src._iGroupSeparator ;
  _sEnumerationOrder = src._sEnumerationOrder ;

	//
  // Effacement des elements dejà contenus dans le vecteur destination
  //
  vider() ;
	//
  // Copie et insertion des elements de la source
  //
  if (false == src.empty())
  {
  	for (iterConstPhraseMot i = src.begin() ; src.end() != i ; i++)
    {
    	NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(*i) ;
      if (pDate)
      {
        NSPhraseMotTime* pNewDate = new NSPhraseMotTime(*pDate) ;
      	push_back(pNewDate) ;
      }
      else
      {
      	NSPhraseMotTimeCycle* pCycle = dynamic_cast<NSPhraseMotTimeCycle*>(*i) ;
        if (pCycle)
        {
          NSPhraseMotTimeCycle* pNewCycle = new NSPhraseMotTimeCycle(*pCycle) ;
        	push_back(pNewCycle) ;
        }
        else
        	push_back(new NSPhraseMot(*(*i))) ;
      }
    }
  }

	return *this ;
}
catch (...)
{
	erreur("Exception NSPhraseMotArray = operator.", standardError) ;
	return *this ;
}
}

// Pour vider toutes les prepositions

void
NSPhraseMotArray::initPreposition()
{
  if (empty())
    return ;

  iterPhraseMot iterMots ;

  for (iterMots = begin() ; end() != iterMots ; iterMots++)
  {
    (*iterMots)->setPreposition(string("")) ;
    if (NULL != (*iterMots)->getComplementPhr())
      // (*iterMots)->getComplementPhr()->nettoye() ;
      (*iterMots)->getComplementPhr()->vider() ;
  }
}

// *************************************************************************
// --------------------- METHODES DE NsProposition -------------------------
// *************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NsProposition::NsProposition(NSContexte* pCtx, bool bInitArray)
              :NSRoot(pCtx)
{
  iPropositionType = notSetType ;
  iConjonctionType = notSetConjonct ;

  if (bInitArray)
  {
    iObjectType    = isPropositionArray ;
    pProposition   = new NSPropositionArray() ;
  }
  else
  {
    iObjectType    = notSetObjType ;
    pProposition   = 0 ;
  }
  bDeleteAtEnd     = true ;

  _sPhrase         = string("") ;
  sLang            = string("") ;

  pPhraDelAtEnd    = (NSPhraseur**) 0 ;
}

NsProposition::NsProposition(NSContexte* pCtx, NSPhraseur** pPhrase, PROPOSITIONTYPE iPropType, CONJONCTIONTYPE iConjonct, bool bDelAtEnd)
              :NSRoot(pCtx)
{
  iPropositionType = iPropType ;
  iConjonctionType = iConjonct ;

  iObjectType      = isPhraseur ;
  pProposition     = *pPhrase ;
  bDeleteAtEnd     = bDelAtEnd ;

  _sPhrase         = string("") ;
  sLang            = string("") ;

  if (bDeleteAtEnd)
    pPhraDelAtEnd = pPhrase ;
  else
    pPhraDelAtEnd = 0 ;
}

NsProposition::NsProposition(NSContexte* pCtx, NSPhraseur* pPhrase, PROPOSITIONTYPE iPropType, CONJONCTIONTYPE iConjonct)
              :NSRoot(pCtx)
{
  iPropositionType = iPropType ;
  iConjonctionType = iConjonct ;

  iObjectType      = isPhraseur ;
  pProposition     = pPhrase ;

  _sPhrase         = string("") ;
  sLang            = string("") ;

  bDeleteAtEnd     = false ;
  pPhraDelAtEnd    = 0 ;
}

NsProposition::NsProposition(const NsProposition& rv)
              :NSRoot(rv.pContexte)
{
try
{
  iPropositionType = rv.iPropositionType ;
  iConjonctionType = rv.iConjonctionType ;

  _sPhrase         = rv._sPhrase ;
  sLang            = rv.sLang ;

  pPhraDelAtEnd    = 0 ;

  if (rv.pProposition)
  {
    if (rv.bDeleteAtEnd)
    {
      if      (isPhraseur == rv.iObjectType)
      {
        NSPhraseur* pPhraseur = (static_cast<NSPhraseur*>(rv.pProposition)) ;
        pProposition = new NSPhraseur(*pPhraseur) ;
        iObjectType  = isPhraseur ;
      }
      else if (isPropositionArray == rv.iObjectType)
      {
        NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(rv.pProposition)) ;
        pProposition = new NSPropositionArray(*pPropArray) ;
        iObjectType  = isPropositionArray ;
      }
      bDeleteAtEnd = true ;
    }
    else
    {
      pProposition = rv.pProposition ;
      iObjectType  = rv.iObjectType ;
      bDeleteAtEnd = false ;
    }
  }
  else
  {
    iObjectType  = notSetObjType ;
    pProposition = 0 ;
    bDeleteAtEnd = true ;
  }
}
catch (...)
{
	erreur("Exception NsProposition copy ctor.", standardError) ;
}
}

NsProposition::~NsProposition()
{
	if (pProposition && bDeleteAtEnd)
  {
  	if      (isPhraseur == iObjectType)
    {
    	NSPhraseur* pPhraseur = (static_cast<NSPhraseur*>(pProposition)) ;
      if (pPhraseur)
        delete pPhraseur ;

      if (pPhraDelAtEnd)
      	*pPhraDelAtEnd = NULL ;
    }
    else if (isPropositionArray == iObjectType)
    {
    	NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pProposition)) ;
      if (pPropArray)
        delete pPropArray ;
      pPropArray = (NSPropositionArray*) 0 ;
    }
    pProposition = 0 ;
  }
}

void
NsProposition::addPropositionToArray(const NSPhraseur* pPhrase, const PROPOSITIONTYPE iPropType, const CONJONCTIONTYPE iConjonct)
{
  if (((NSPhraseur*) NULL == pPhrase) || (isPropositionArray != iObjectType))
    return ;

  NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pProposition)) ;
  if ((NSPropositionArray*) NULL == pPropArray)
    return ;

  NSPhraseur* pNewPhr = new NSPhraseur(*pPhrase) ;
  pPropArray->push_back(new NsProposition(pContexte, &pNewPhr, iPropType, iConjonct)) ;
}

NsProposition&
NsProposition::operator=(const NsProposition& src)
{
try
{
	if (this == &src)
		return *this ;

	iPropositionType = src.iPropositionType ;
  iConjonctionType = src.iConjonctionType ;

  _sPhrase         = src._sPhrase ;
  sLang            = src.sLang ;

  pPhraDelAtEnd    = (NSPhraseur**) 0 ;

  if (src.pProposition)
  {
  	if (src.bDeleteAtEnd)
    {
    	if      (isPhraseur == src.iObjectType)
      {
      	NSPhraseur* pPhraseur = (static_cast<NSPhraseur*>(src.pProposition)) ;
        pProposition = new NSPhraseur(*pPhraseur) ;
        iObjectType  = isPhraseur ;
      }
      else if (isPropositionArray == src.iObjectType)
      {
      	NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(src.pProposition)) ;
        pProposition = new NSPropositionArray(*pPropArray) ;
        iObjectType  = isPropositionArray ;
      }
      bDeleteAtEnd = true ;
    }
    else
    {
    	pProposition = src.pProposition ;
      iObjectType  = src.iObjectType ;
      bDeleteAtEnd = false ;
    }
  }
  else
  {
  	iObjectType  = notSetObjType ;
    pProposition = 0 ;
  }

	return *this ;
}
catch (...)
{
	erreur("Exception NsProposition = operator.", standardError) ;
	return *this ;
}
}

// *************************************************************************
// -------------------- METHODES DE NSPropositionArray ------------------------
// *************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
/*
NSPropositionArray::NSPropositionArray(NSPropositionArray& rv)
                   :NSPropositionVector()
{
try
{
    if (rv.empty())
        return;
    for (iterProposition i = rv.begin(); i != rv.end();i++)
   	    push_back(new NsProposition(*(*i)));
}
catch (...)
{
    erreur("Exception NSPropositionArray copy ctor.", 0, 0);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSPropositionArray::vider()
{
    if (empty())
        return;
    for (iterProposition i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSPropositionArray::~NSPropositionArray()
{
    vider();
}

//---------------------------------------------------------------------------
//  Operateur d'affectation
//---------------------------------------------------------------------------
NSPropositionArray&
NSPropositionArray::operator=(NSPropositionArray src)
{
try
{
    //
    // Effacement des elements deja contenus dans le vecteur destination
    //
    vider();
	//
    // Copie et insertion des elements de la source
    //
    if (!(src.empty()))
  	    for (iterProposition i = src.begin(); i != src.end(); i++)
   	        push_back(new NsProposition(*(*i)));
    return *this;
}
catch (...)
{
    erreur("Exception NSPropositionArray = operator.", 0, 0);
    return *this ;
}
} */


NSPhraseurArray::NSPhraseurArray(const NSPhraseurArray& rv)
{
  if (rv.empty())
    return ;

  for (phraseurConstIter i = rv.begin() ; rv.end() != i ; i++)
    push_back(new NSPhraseur(**i)) ;
}

NSPhraseurArray::~NSPhraseurArray()
{
  vider() ;
}

NSPhraseurArray&
NSPhraseurArray::operator=(const NSPhraseurArray &src)
{
  if (this == &src)
		return *this ;

  vider() ;

  if (src.empty())
    for (phraseurConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(new NSPhraseur(**i)) ;

  return *this ;
}

void
NSPhraseurArray::vider()
{
  if (empty())
    return ;

  for (phraseurIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

