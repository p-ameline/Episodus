#include <algorithm>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

#include "dcodeur/nsgen.h"
#include "dcodeur/nsgenlan.h"
#include "dcodeur/nsphrase.h"
#include "dcodeur/nsdkd.h"
#include "nsbb/nsbbtran.h"
#include "nssavoir/nspatho.h"
#include "nssavoir/nsfilgd.h"

bool
GenComplementSortByPriorityInf(NSGenComplement *pCompl1, NSGenComplement *pCompl2)
{
	return (pCompl1->getPriority() < pCompl2->getPriority()) ;
}

bool
GenComplementSortByPrioritySup(NSGenComplement *pCompl1, NSGenComplement *pCompl2)
{
	return (pCompl1->getPriority() > pCompl2->getPriority()) ;
}

// -------------------------------------------------------------------------
// ---------------------- METHODES DE generateurFr -------------------------
// -------------------------------------------------------------------------

NSGenerateurFr::NSGenerateurFr(NSContexte* pCtx, NsProposition* pPropos, string sLangue)
               :NSGenerateur(pCtx, pPropos, sLangue)
{
  iTempsVerbe  = tempsPresentFr ;
  iAspectVerbe = aspectMoment ;
}

NSGenerateurFr::NSGenerateurFr(const NSGenerateurFr& rv)
               :NSGenerateur(rv)
{
  iTempsVerbe  = rv.iTempsVerbe;
  iAspectVerbe = rv.iAspectVerbe;
}

NSGenerateurFr::~NSGenerateurFr()
{
}

NSGenerateurFr&
NSGenerateurFr::operator=(const NSGenerateurFr& rv)
{
	if (&rv == this)
		return *this ;

	reinitialise() ;

	copieTout(&rv) ;

	iTempsVerbe  = rv.iTempsVerbe ;
	iAspectVerbe = rv.iAspectVerbe ;

	return *this ;
}

// classeTout() : permet de remplir les arrays du generateur a partir de celles
// du phraseur. Les arrays du generateur separent les elements a sens positif
// (dont la certitude est superieure ou egale a WCE50) et les elements a sens
// negatif (dont la certitude est strictement inferieure a WCE50).

void
NSGenerateurFr::classeTout()
{
try
{
  reinitialise() ;

  // En francais, les adjectifs ayant une certitude sont toujours places apres le nom.
  //
  // Les adjectifs ont deja ete tries en avant et apres, donc on ne reverifie pas
  // qu'il s'agit bien d'adjectifs du lexique.
  //
  dispatchArray(&(_pPh->adjEpitheteAv), &_pAdjEpitheteAvPos, &_pAdjEpitheteAvNeg) ;
  if (_pAdjEpitheteAvPos)
    _pAdjEpitheteAvPos->setParameters(&(_pPh->adjEpitheteAv)) ;
  if (_pAdjEpitheteAvNeg)
    _pAdjEpitheteAvNeg->setParameters(&(_pPh->adjEpitheteAv)) ;

  dispatchArray(&(_pPh->adjEpitheteAp), &_pAdjEpitheteApPos, &_pAdjEpitheteApNeg) ;
  if (_pAdjEpitheteApPos)
    _pAdjEpitheteApPos->setParameters(&(_pPh->adjEpitheteAp)) ;
  if (_pAdjEpitheteApNeg)
    _pAdjEpitheteApNeg->setParameters(&(_pPh->adjEpitheteAp)) ;

  dispatchArray(&(_pPh->compNom), &_pCompNomPos, &_pCompNomNeg, isNoun) ;
  if (_pCompNomPos)
    _pCompNomPos->setParameters(&(_pPh->compNom)) ;
  if (_pCompNomNeg)
    _pCompNomNeg->setParameters(&(_pPh->compNom)) ;

  dispatchArray(&(_pPh->adverbe), &_pAdverbePos, &_pAdverbeNeg, isAdverb) ;
  if (_pAdverbePos)
    _pAdverbePos->setParameters(&(_pPh->adverbe)) ;
  if (_pAdverbeNeg)
    _pAdverbeNeg->setParameters(&(_pPh->adverbe)) ;

  dispatchArray(&(_pPh->COD), &_pCODPos, &_pCODNeg, isNoun) ;
  if (_pCODPos)
    _pCODPos->setParameters(&(_pPh->COD)) ;
  if (_pCODNeg)
    _pCODNeg->setParameters(&(_pPh->COD)) ;

  dispatchArray(&(_pPh->AttSujet), &_pAttSujetPos, &_pAttSujetNeg) ;
  if (_pAttSujetPos)
    _pAttSujetPos->setParameters(&(_pPh->AttSujet)) ;
  if (_pAttSujetNeg)
    _pAttSujetNeg->setParameters(&(_pPh->AttSujet)) ;

  dispatchArray(&(_pPh->AttCOD), &_pAttCODPos, &_pAttCODNeg) ;
  if (_pAttCODPos)
    _pAttCODPos->setParameters(&(_pPh->AttCOD)) ;
  if (_pAttCODNeg)
    _pAttCODNeg->setParameters(&(_pPh->AttCOD)) ;

  _pCCArray->classeTout() ;
}
catch (...)
{
	erreur("Exception NSGenerateurFr::classeTout", standardError) ;
}
}

// generePhrase(int iStyle) : methode principale de generation de la phrase.

bool
NSGenerateurFr::generePhrase(DCODETYPE iStyle)
{
try
{
  reinitialise() ;
  _sPhrase = string("") ;

  SentenceBlocks sentenceBlocks ;

  string sLexique   = string("") ;
  string sPluriel   = string("") ;
  string sCertitude = string("") ;
  string sInter2    = string("") ;

  // La phrase est-elle affirmative ou negative ?
  // true pour phrase affirmative, false pour phrase negative.
  //
  if (NSPhraseur::negationOui == _pPh->iVbNegation)
    sentenceBlocks.setAffirmative(false) ;

  NSPathologData Data ;

  // Avant de classer les compléments, on leur met la preposition lorsqu'il y
  // en a une d'imposée par le mot complété.
  // Le verbe peut imposer une préposition au COI ainsi qu'au COS.
  // Il faut traiter les prépositions des complements circonstanciels.

  if      (false == _pPh->Verbe.empty())
    traitePostposition(*(_pPh->Verbe.begin())) ;
  else if (false == _pPh->Sujet.empty())
  {
    if ((phrasePrincipale == _pPh->iPhraseType) &&
        (NSPhraseur::formePassive != _pPh->iForme) &&
        (false == _pPh->CCChiffre.empty()))
    {
      // S'il existe un complément de nombre et pas de verbe...
      //
      iterPhraseMot itNb = _pPh->CCChiffre.begin() ;

      if (string("") == (*itNb)->getLexique())
      {
        // si le nombre n'a pas de concept, on impose le verbe "être" et
        // on ajoute la préposition "de" (le poids est de 70 kg)
        //
        _pPh->Verbe.push_back(new NSPhraseMot(pContexte, string("4ETRE1"))) ;
        _pPh->PrepositionChiffre.setPreposition(string("de")) ;
      }
      else
      {
        // si le nombre a un concept, on impose le verbe "avoir"
        // (l'enfant a un poids de 70 kg)
        //
        _pPh->Verbe.push_back(new NSPhraseMot(pContexte, string("4AVOI1"))) ;
      }

      // for (iterPhraseMot itNb = pPh->CCChiffre.begin() ; pPh->CCChiffre.end() != itNb ; itNb++)
      //  if (string("") == (*itNb)->getPreposition())
      //    (*itNb)->setPreposition(string("de")) ;
    }
  }

  traitePrepositionCC(&(_pPh->CCLieu),     &(_pPh->PrepositionLieu),    NSPhraseur::prepLieu) ;
  traitePrepositionCC(&(_pPh->CCTemps),    &(_pPh->PrepositionTemps),   NSPhraseur::prepTemps) ;
  traitePrepositionCC(&(_pPh->CCManiere),  &(_pPh->PrepositionManiere), NSPhraseur::prepManiere) ;
  traitePrepositionCC(&(_pPh->CCMoyen),    &(_pPh->PrepositionMoyen),   NSPhraseur::prepMoyen) ;
  traitePrepositionCC(&(_pPh->CCCause),    &(_pPh->PrepositionCause),   NSPhraseur::prepCause) ;
  traitePrepositionCC(&(_pPh->CCBut),      &(_pPh->PrepositionBut),     NSPhraseur::prepBut) ;
  traitePrepositionCC(&(_pPh->CCType),     &(_pPh->PrepositionType),    NSPhraseur::prepType) ;
  traitePrepositionCC(&(_pPh->CCHypoth),   &(_pPh->PrepositionHypoth),  NSPhraseur::prepHypoth) ;
  traitePrepositionCC(&(_pPh->CCChiffre),  &(_pPh->PrepositionChiffre), NSPhraseur::prepChiffre) ;

  // Ensuite on classe.
  classeTout() ;

  //
  // PREPARATION DES COMPOSANTS
  //

  // On transforme la gestion du verbe d'internationale a francaise.
  //

  mapTempsVerbe() ;

  // Dans tous les composants, on peut avoir des caracteres libres. Ceux-ci ont
  // le code "£C;020". Leur lexique est ce qu'il faut afficher.

  // Adverbes du verbe. (stockés dans le complément du verbe.)

  if (false == prepareVerbe(&sentenceBlocks, iStyle))
    return false ;

  //
  // Sujet
  //
  if (false == prepareSujet(&sentenceBlocks, iStyle))
    return false ;

  // On traite le COD.
  // On met d'abord les complements positifs, puis les négatifs, avec "mais pas de" entre.
  // S'il n'y a que des négatifs, la phrase devient négative.
  //
  if (false == prepareCOD(&sentenceBlocks, iStyle))
    return false ;

  //
  // Compléments : quand, où, comment, avec quoi, pourquoi, dans quel but
  //
  sentenceBlocks.setThreshold(101) ;

  _pCCArray->donnePhrase(iStyle) ;
  if (false == _pCCArray->empty())
    sort(_pCCArray->begin(), _pCCArray->end(), GenComplementSortByPrioritySup) ;

  // Cas de la phrase principale
  //
  if (phrasePrincipale == _pPh->iPhraseType)
  {
    if (false == generePhrasePrincipale(&sentenceBlocks, iStyle))
      return false ;
  }

  //
  // Compléments
  //
  // Penser à pouvoir forcer l'ordre (importance) des Compléments.
  //
  // Ceci peut être réalisé en utilisant l'int iPriorite de NSGenComplement.
  //
  writeComplements(sentenceBlocks.getThreshold(), string(" "), string(" mais pas "), string("")) ;

  //
  // Cas de la subordonnée relative
  //
  // La subordonnée relative complète un nom ou un groupe nominal appartenant
  // à la principale. Elle fait partie des expansions du nom au même titre que
  // le complément du nom et l'adjectif.

  if (phraseRelative == _pPh->iPhraseType)
  {
    if (false == generePhraseRelative(&sentenceBlocks, iStyle))
      return false ;
  }

  // Remarquons qu'on ne traite pas les compléments dans le cas d'une subordonnée
  // relative.
  // Ceux-ci sont traites dans genereNomComplet, car ils se placent devant la
  // subordonnee relative.

  // A la fin, on rajoute la phrase qui se place apres les " : ".

  if (_pPh->pDeuxPoints)
  {
    NsProposition  Propos(pContexte, &(_pPh->pDeuxPoints), NsProposition::deuxPoints, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneDeuxPoints(pContexte, &Propos, sLang) ;
    GeneDeuxPoints.genereProposition(iStyle) ;
    _sPhrase += string(" : ") + GeneDeuxPoints.getPropositionPhrase() ;
  }

  if (_pPh->pParentheses)
  {
    NsProposition Propos(pContexte, &(_pPh->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneParentheses(pContexte, &Propos, sLang) ;

    DCODETYPE dcParenthese = _pPh->iParenthesesStyle ;
    if (dcUndefined == dcParenthese)
      dcParenthese = iStyle ;

    GeneParentheses.genereProposition(dcParenthese) ;
    _sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
  }

  // Elimination des éventuels blancs initiaux
  strip(_sPhrase, stripLeft) ;

  _sPhrase = postTraitement(&_sPhrase) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSGenerateurFr::generePhrase.", standardError) ;
	return false ;
}
}

bool
NSGenerateurFr::generePhrasePrincipale(SentenceBlocks* pSB, DCODETYPE iStyle)
{
try
{
  if ((SentenceBlocks*) NULL == pSB)
    return false ;

  // Phrase classique (pas en mode "tiret")
  //
  if (dcPhrase == iStyle)
  {
    //
    // Cas classique : FORME ACTIVE
    //
    if (NSPhraseur::formePassive != _pPh->iForme)
    {
      // On commence par inscrire les compléments de priorité >= 90
      // "au niveau du côlon droit, il existe un polype"
      //
      writeComplements(90, string(""), string("pas "), string(", "), true) ;
      pSB->setThreshold(90) ;

      //
      //     P
      //   /  |
      //  GN  GV
      //
      //
      if (string("") != pSB->getSujet())
        buildActiveSentenceWithSubject(pSB) ;

      else // Pas de sujet.
        buildActiveSentenceWithoutSubject(pSB) ;
    }
    //
    // FORME PASSIVE
    //
    else
      buildPassiveSentence(pSB) ;
  }
  else if (dcTiret == iStyle)
    buildBulletSentence(pSB) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSGenerateurFr::generePhrasePrincipale.", standardError) ;
	return false ;
}
}

bool
NSGenerateurFr::generePhraseRelative(SentenceBlocks* pSB, DCODETYPE iStyle)
{
try
{
  if ((SentenceBlocks*) NULL == pSB)
    return false ;

  if (dcPhrase == iStyle)
  {
    //
    // Cas classique : FORME ACTIVE
    //
    if (NSPhraseur::formePassive != _pPh->iForme)
    {
      //
      //     P
      //   /  |
      //  GN  GV
      //
      //
      if (string("") != pSB->getSujet())
      {
        if (NSPhraseur::sujetNoRepeat == _pPh->iTypeSujet)
          _sPhrase = string("qui") ;
        else
          _sPhrase = string("dont ") + pSB->getSujet() ;

        // Cas d'une vraie phrase (iStyle == dcPhrase)
        //
        //
        //                P
        //              /  |         |
        //             GN  GV        GN prep
        //                / |
        //               V  GN (COD)
        //   ex : Ma fille qui a conduit la voiture sur l'autoroute
        //        Ma fille dont la voiture prend le virage
        //
        buildActiveVerbalGroup(pSB) ;
      }
      // Pas de sujet. Impossible, puisqu'on a normalement rempli
      // la case sujet avant d'appeler generePhrase.
      //
      else
        return false ;
    }
    //
    // FORME PASSIVE
    //
    // Ca donne :
    //
    // La personne par qui le drame est arrivé.
    //
    else
    {
      _sPhrase = string("par qui ") ;

      if (false == buildPassiveVerbalGroup(pSB))
        return false ;
    }
  }

  else if (dcTiret == iStyle)
  {
    // Impossible de faire une phrase "à tiret" à partir d'une phrase comme
    // "Je vois l'enfant qui prend la balle"
    // Par contre une phrase comme "il existe une plaque qui induit une sténose"
    // peut devenir "plaque induisant une sténose"
    //
    // if (NSPhraseur::sujetNoRepeat == _pPh->iTypeSujet)
    //  return false ;

    // Plusieurs cas...
    // 1) Avec verbe être -> comme la forme active, en enlevant le verbe être
    //    Ex : Un anévrysme dont le siège est sous-rénal
    //                       -> Un anévrysme de siège sous-rénal
    //
    //
    // Cas classique : FORME ACTIVE
    //
    if (NSPhraseur::formePassive != _pPh->iForme)
    {
      //
      //     P
      //   /  |
      //  GN  GV
      //
      //
      if (string("") != pSB->getSujet())
      {
        if (NSPhraseur::sujetNoRepeat != _pPh->iTypeSujet)
        {
          if (CommenceParVoyelle(pSB->getSujet()))
            _sPhrase = string("d'") + pSB->getSujet() ;
          else
            _sPhrase = string("de ") + pSB->getSujet() ;
        }

        // Cas d'une vraie phrase (iStyle == dcPhrase)
        //
        //
        //                P
        //              /  |         |
        //             GN  GV        GN prep
        //                / |
        //               V  GN (COD)
        //   ex : Ma fille qui a conduit la voiture sur l'autoroute
        //        Ma fille dont la voiture prend le virage
        //
        // Il n'est pas possible d'avoir à la fois un COD et un attribut
        // du sujet (qui appartiennent tous deux au GV)
        //                       Cet aspect
        //                          /  |
        //              est atypique   évoque un kyste
        //
        if ((string("") != pSB->getCOD()) && (string("") != pSB->getAttSujet()))
          return false ;

        // L'existence d'un COD impose celle d'un verbe
        if ((string("") != pSB->getCOD()) && (_pPh->Verbe.empty()))
          return false ;

        // S'il existe un attribut et pas de verbe, on impose
        // le verbe être
        if ((string("") != pSB->getAttSujet()) && (_pPh->Verbe.empty()))
        {
          NSPatPathoData PathoData ;
          PathoData.setLexique(string("4ETRE1")) ;
          _pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;
        }

        // Le verbe est précisé
        //
        if (false == _pPh->Verbe.empty())
        {
          // On traite au passage le cas où la phrase est négative

          string sVb = (*(_pPh->Verbe.begin()))->getLexique() ;
          //
          // Pas verbe être
          //
          if (string("4ETRE1") != sVb)
          {
            string sVbLabel = getParticipePresent(sVb, pSB->isAffirmative()) ;
            pSB->addToVerb(sVbLabel) ;

            if (string("") != pSB->getPhraseAdverb())
              pSB->addToVerb(string(" ") + pSB->getPhraseAdverb()) ;
          }
          // Le verbe être disparaît
          //
          else
            pSB->setVerb(string("")) ;
        }

        if (string("") != pSB->getVerb())
          _sPhrase += string(" ") + pSB->getVerb() ;

        if (string("") != pSB->getCOD())
          _sPhrase += string(" ") + pSB->getCOD() ;
        if (string("") != pSB->getAttCOD())
          _sPhrase += string(" ") + pSB->getAttCOD() ;
        if (string("") != pSB->getAttSujet())
          _sPhrase += string(" ") + pSB->getAttSujet() ;

        return true ;
      }
      // Pas de sujet. Impossible, puisqu'on a normalement rempli
      // la case sujet avant d'appeler generePhrase.
      //
      else
        return false ;
    }

    return false ;
  }

  return true ;
}
catch (...)
{
	erreur("Exception NSGenerateurFr::generePhraseRelative.", standardError) ;
	return false ;
}
}

//
//     P
//   /  |
//  GN  GV
//
//
bool
NSGenerateurFr::buildActiveSentenceWithSubject(SentenceBlocks* pSB)
{
  if ((SentenceBlocks*) NULL == pSB)
    return false ;

  // If subject is not to be repeated, it is replaced by a pronom personnel
  // Example : "Le garçon joue avec la balle" -> "Il joue avec la balle"
  //
  if (NSPhraseur::sujetNoRepeat == _pPh->iTypeSujet)
  {
    NSPhraseur::VBPERSO iPersonVb ;

    if (isPlural(pSB->getGenreSujet()))
      iPersonVb = NSPhraseur::pers3P ;
    else
      iPersonVb = NSPhraseur::pers3S ;

    _sPhrase += donnePronomPersonnel(pSB->getGenreSujet(), iPersonVb, STR_SUJET) ;
  }

  // If subject is not to be fully removed (usually in case of several propositions)
  // Example : "Il joue avec la balle et il regarde en l'air" -> "Il joue avec la balle et regarde en l'air"
  //
  else if (NSPhraseur::sujetRemove != _pPh->iTypeSujet)
    _sPhrase += pSB->getSujet() ;

  // Cas d'une vraie phrase (iStyle == dcPhrase)
  //
  //
  //                P
  //              /  |         |
  //             GN  GV        GN prep
  //                / |
  //               V  GN (COD)
  //   ex : Ma fille a conduit la voiture sur l'autoroute
  //
  buildActiveVerbalGroup(pSB) ;

  return true ;
}

bool
NSGenerateurFr::buildActiveSentenceWithoutSubject(SentenceBlocks* pSB)
{
  if ((SentenceBlocks*) NULL == pSB)
    return false ;

  //
  // On s'occupe du verbe
  //
  if (false == _pPh->Verbe.empty())
  {
    // Création d'un sujet virtuel : "on" pour les
    // verbes d'action et "il" pour les verbes d'état

    string sTransitivite ;
    string sConjugaison ;
    string sActEtat ;

    if (false == donneVerbeClasse(*(_pPh->Verbe.begin()), &sTransitivite, &sConjugaison, &sActEtat))
      return false ;

    switch (_pPh->iVbType)
    {
      case NSPhraseur::vbTypeAction : _sPhrase += string("on") ; break ;
      case NSPhraseur::vbTypeEtat   : _sPhrase += string("il") ; break ;
    }
    _pPh->iVbPersonne = NSPhraseur::pers3S ;

    string sPrincipal, sAuxilliaire ;
    donneVerbe(&sPrincipal, &sAuxilliaire) ;

    // Phrase affirmative
    //
    if (pSB->isAffirmative())
    {
      if (string("") != sAuxilliaire)
        pSB->addToVerb(string(" ") + sAuxilliaire) ;
      if (sPrincipal != "")
        pSB->addToVerb(string(" ") + sPrincipal) ;

      if (string("") != pSB->getPhraseAdverb())
        pSB->addToVerb(string(" ") + pSB->getPhraseAdverb()) ;
    }
    // Phrase négative
    //
    else
    {
      if (string("") != sAuxilliaire)
      {
        if (CommenceParVoyelle(sAuxilliaire))
          pSB->addToVerb(string(" n'") + sAuxilliaire + pSB->getModifAdv() + string("pas")) ;
        else
          pSB->addToVerb(string(" ne ") + sAuxilliaire + pSB->getModifAdv() + string("pas")) ;

        if (string("") != pSB->getPhraseAdverb())
          pSB->addToVerb(string(" ") + pSB->getPhraseAdverb()) ;

        if (string("") != sPrincipal)
          pSB->addToVerb(string(" ") + sPrincipal) ;
      }
      else
      {
        // On verifie si le verbe commence par une voyelle ou un h muet
        if (CommenceParVoyelle(sPrincipal))
          pSB->addToVerb(string(" n'") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
        else
        {
          NSPhraseMot* pVerb = *(_pPh->Verbe.begin()) ;

          NSPathologData* pData = pVerb->getLexiqueData(sLang) ;
          if ((NSPathologData*) NULL == pData)
            return false ;

          if (pData->chercheGrammaire(VALEUR_DE_H) == H_MUET)
            pSB->addToVerb(string(" n'") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
          else
            pSB->addToVerb(string(" ne ") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
        }

        if (string("") != pSB->getPhraseAdverb())
          pSB->addToVerb(string(" ") + pSB->getPhraseAdverb()) ;
      }
    }
    _sPhrase += pSB->getVerb() ;
    if (string("") != pSB->getCOD())
      _sPhrase += string(" ") + pSB->getCOD() ;
    if (string("") != pSB->getAttCOD())
      _sPhrase += string(" ") + pSB->getAttCOD() ;
  }
  //
  // Ni verbe, ni sujet -> phrase sur le mode "il existe" ou "on note"
  //
  else if (string("") != pSB->getCOD())
  {
    NSPatPathoData PathoData ;
    PathoData.setLexique("4EXIS1") ;
    _pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

    _sPhrase += string("il") ;
    _pPh->iVbPersonne = NSPhraseur::pers3S ;

    string sPrincipal, sAuxilliaire ;
    donneVerbe(&sPrincipal, &sAuxilliaire) ;

    if (pSB->isAffirmative())
    {
      if (string("") != sAuxilliaire)
        pSB->addToVerb(string(" ") + sAuxilliaire) ;
      if (string("") != sPrincipal)
        pSB->addToVerb(string(" ") + sPrincipal) ;
    }
    // Phrase négative
    //
    else
    {
      if (string("") != sAuxilliaire)
      {
        if (CommenceParVoyelle(sAuxilliaire))
          pSB->addToVerb(string(" n'") + sAuxilliaire + pSB->getModifAdv() + string("pas")) ;
        else
          pSB->addToVerb(string(" ne ") + sAuxilliaire + pSB->getModifAdv() + string("pas")) ;

        if (string("") != sPrincipal)
          pSB->addToVerb(string(" ") + sPrincipal) ;
      }
      else
      {
        // On vérifie si le verbe commence par une voyelle ou un h muet
        if (CommenceParVoyelle(sPrincipal))
          pSB->addToVerb(string(" n'") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
        else
        {
          NSPhraseMot* pVerb = *(_pPh->Verbe.begin()) ;

          NSPathologData* pData = pVerb->getLexiqueData(sLang) ;
          if ((NSPathologData*) NULL == pData)
            return false ;

          if (pData->chercheGrammaire(VALEUR_DE_H) == H_MUET)
            pSB->addToVerb(string(" n'") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
          else
            pSB->addToVerb(string(" ne ") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
        }
      }
    }

    _sPhrase += pSB->getVerb() ;
    if (string("") != pSB->getCOD())
      _sPhrase += string(" ") + pSB->getCOD() ;
    if (string("") != pSB->getAttCOD())
      _sPhrase += string(" ") + pSB->getAttCOD() ;
  }

  return true ;
}

//
// FORME PASSIVE
//
// Le sujet et le COD inversent leur rôle ; le temps est donné par
// le verbe être, le verbe est au participe passé
//
// Ex Jean construit la maison -> La maison est construite par Jean
//
bool
NSGenerateurFr::buildPassiveSentence(SentenceBlocks* pSB)
{
  if ((SentenceBlocks*) NULL == pSB)
    return false ;

  if (false == buildPassiveVerbalGroup(pSB))
    return false ;

  //
  // S'il y a un sujet, on ajoute "par" devant.
  // Il peut ne pas y avoir de sujet, par exemple
  // "La vésicule est étudiée en decubitus dorsal"
  //
  if (string("") != pSB->getSujet())
    _sPhrase += string(" par ") + pSB->getSujet() ;

  return true ;
}

// C'est comme la forme passive, sauf qu'on enlève le verbe être, et
// qu'on enlève les articles du COD
// "La lésion est traitée par laser" -> "Lésion traitée par laser"
//
// Autre cas de figure, il y a un sujet et on met le verbe au participe présent
// "La plaque représente 35% de la surface" ->
//                                  "plaque représentant 35% de la surface"
//
// Enfin, si ce n'est pas une : "vraie phrase" avec un COD ou un sujet, il peut
// s'agir d'une énumération de valeurs chiffrées
//
bool
NSGenerateurFr::buildBulletSentence(SentenceBlocks* pSB)
{
  if ((SentenceBlocks*) NULL == pSB)
    return false ;

  // Il faut forcément un COD, ou des valeurs chiffrées
  // if ((sCOD == "") && (pPh->CCChiffre.empty()))
  //     return false;

  // S'il y a un COD mais pas de verbe, c'est juste une énumeration.
  //
  if (string("") != pSB->getCOD())
  {
    _sPhrase = pSB->getCOD() ;

    // Cas COD + attribut du COD : [Il existe une] Pression artérielle augmentée
    //
    if (_pPh->Verbe.empty() && (string("") != pSB->getAttCOD()))
      _sPhrase += string(" ") + pSB->getAttCOD() ;
  }

  if (_pPh->Verbe.empty())
  {
    // Cas sujet + attribut du sujet : Taille des reins [est] normale
    //
    if ((string("") != pSB->getSujet()) && (string("") != pSB->getAttSujet()))
    {
      _sPhrase = pSB->getSujet() + string(" ") + pSB->getAttSujet() ;
      return true ;
    }
    else
      return true ;
  }

  bool bPassiveMode = true ;

  // Il faut forcément un COD, sinon on se retrouve sans sujet
  //
  // Alternative, "la plaque représente 35% de la surface" devient
  //                        "plaque représentant 35% de la surface"
  //
  if (string("") == pSB->getCOD())
  {
    if (string("") == pSB->getSujet())
      return false ;

    _sPhrase = pSB->getSujet() ;

    bPassiveMode = false ;
  }

  //
  // On prend le verbe au participe passé
  //
  string sVb = string("") ;
  if (bPassiveMode)
    sVb = donneParticipePasse(*(_pPh->Verbe.begin()), pSB->getGenreCOD()) ;
  else
    sVb = getParticipePresent((*(_pPh->Verbe.begin()))->getLexique(), pSB->isAffirmative()) ;

  // Si la phrase est négative, on ajoute "pas", ou "absolument pas" etc...
  // juste avant le participe passé.

  if (false == pSB->isAffirmative())
  {
    _sPhrase += pSB->getModifAdv() + string("pas ") ;

    if (string("") != pSB->getPhraseAdverb())
      _sPhrase += pSB->getPhraseAdverb() += string(" ") ;

    _sPhrase += sVb ;
  }
  else
  {
    _sPhrase += string(" ") + sVb ;

    if (string("") != pSB->getPhraseAdverb())
      _sPhrase += string(" ") + pSB->getPhraseAdverb() ;
  }

  if ((false == bPassiveMode) && (string("") != pSB->getCOD()))
    _sPhrase += string(" ") + pSB->getCOD() ;

  // Propositions trouvées intéressantes par Jean.
  //
  if (string("") != pSB->getAttCOD())
    _sPhrase += string(" ") + pSB->getAttCOD() ;

  // S'il y a un sujet, on ajoute "par" devant.
  // Il peut ne pas y avoir de sujet, par exemple
  // "Vésicule étudiée en decubitus dorsal"
  //
  if (bPassiveMode && (string("") != pSB->getSujet()))
    _sPhrase += string(" par ") + pSB->getSujet() ;

  return true ;
}

//                GV
//              /  |
//             V   GN (COD)
//
bool
NSGenerateurFr::buildActiveVerbalGroup(SentenceBlocks* pSB)
{
  if ((SentenceBlocks*) NULL == pSB)
    return false ;

  // Il n'est pas possible d'avoir à la fois un COD et un attribut
  // du sujet (qui appartiennent tous deux au GV)
  //                       Cet aspect
  //                          /  |
  //              est atypique   évoque un kyste
  //
  if ((string("") != pSB->getCOD()) && (string("") != pSB->getAttSujet()))
    return false ;

  // L'existence d'un COD impose celle d'un verbe
  //
  if ((string("") != pSB->getCOD()) && (_pPh->Verbe.empty()))
    return false ;

  // S'il existe un attribut et pas de verbe, on impose
  // le verbe être
  if ((string("") != pSB->getAttSujet()) && (_pPh->Verbe.empty()))
  {
    NSPatPathoData PathoData ;
    PathoData.setLexique("4ETRE1") ;
    _pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;
  }

  if (isPlural(pSB->getGenreSujet()))
    _pPh->iVbPersonne = NSPhraseur::pers3P ;
  else
    _pPh->iVbPersonne = NSPhraseur::pers3S ;

  // On traite au passage le cas où la phrase est négative

  string sPrincipal, sAuxilliaire ;
  donneVerbe(&sPrincipal, &sAuxilliaire) ;

  if (string("") != sAuxilliaire)
  {
    if (false == pSB->isAffirmative())
    {
      // L'auxilliaire (être ou avoir) ne commence jamais par un h.
      if (CommenceParVoyelle(sAuxilliaire))
        pSB->addToVerb(string("n'") + sAuxilliaire + pSB->getModifAdv() + string("pas")) ;
      else
        pSB->addToVerb(string("ne ") + sAuxilliaire + pSB->getModifAdv() + string("pas")) ;

      if (string("") != pSB->getPhraseAdverb())
        pSB->addToVerb(string(" ") + pSB->getPhraseAdverb()) ;

      if (string("") != sPrincipal)
        pSB->addToVerb(string(" ") + sPrincipal) ;
    }
    else
    {
      pSB->addToVerb(sAuxilliaire) ;

      if (string("") != sPrincipal)
        pSB->addToVerb(string(" ") + sPrincipal) ;

      if (string("") != pSB->getPhraseAdverb())
        pSB->addToVerb(string(" ") + pSB->getPhraseAdverb()) ;
    }
  }
  else
  {
    if (false == pSB->isAffirmative())
    {
      // On vérifie si le verbe commence par une voyelle ou un h muet.
      if (CommenceParVoyelle(sPrincipal))
        pSB->addToVerb(string("n'") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
      else
      {
        NSPhraseMot* pVerb = *(_pPh->Verbe.begin()) ;

        NSPathologData* pData = pVerb->getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pData)
          return false ;

        if (pData->chercheGrammaire(VALEUR_DE_H) == H_MUET)
          pSB->addToVerb(string("n'") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
        else
          pSB->addToVerb(string("ne ") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
      }
    }
    else
      pSB->addToVerb(sPrincipal) ;

    if (string("") != pSB->getPhraseAdverb())
      pSB->addToVerb(string(" ") + pSB->getPhraseAdverb()) ;
  }

  _sPhrase += string(" ") + pSB->getVerb() ;

  if (string("") != pSB->getCOD())
    _sPhrase += string(" ") + pSB->getCOD() ;
  if (string("") != pSB->getAttCOD())
    _sPhrase += string(" ") + pSB->getAttCOD() ;
  if (string("") != pSB->getAttSujet())
    _sPhrase += string(" ") + pSB->getAttSujet() ;

  return true ;
}

//                P
//              /  |
//             S   V
//           (COD)
//
bool
NSGenerateurFr::buildPassiveVerbalGroup(SentenceBlocks* pSB)
{
  if ((SentenceBlocks*) NULL == pSB)
    return false ;

  // Il faut forcément un COD, sinon on se retrouve sans sujet
  if (string("") == pSB->getCOD())
    return false ;

  // Il faut forcément un verbe
  if (_pPh->Verbe.empty())
    return false ;

  // La vésicule est étudiée -> Elle est étudiée
  //
  if (NSPhraseur::sujetNoRepeat == _pPh->iTypeSujet)
  {
    NSPhraseur::VBPERSO iPersonVb ;

    if (isPlural(pSB->getGenreCOD()))
      iPersonVb = NSPhraseur::pers3P ;
    else
      iPersonVb = NSPhraseur::pers3S ;

    _sPhrase = donnePronomPersonnel(pSB->getGenreSujet(), iPersonVb, STR_SUJET) ;
  }
  else if (NSPhraseur::sujetRemove != _pPh->iTypeSujet)
    _sPhrase = pSB->getCOD() ;

  // On prend le verbe au participe passé, c'est le verbe être
  // qui est conjugué
  //
  // Il faut necessairement un verbe.

  if (_pPh->Verbe.empty())
    return false ;

  string sVb = donneParticipePasse(*(_pPh->Verbe.begin()), pSB->getGenreCOD()) ;

  _pPh->Verbe.vider() ;
  NSPatPathoData PathoData ;
  PathoData.setLexique("4ETRE1") ;
  _pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

  if (isPlural(pSB->getGenreCOD()))
    _pPh->iVbPersonne = NSPhraseur::pers3P ;
  else
    _pPh->iVbPersonne = NSPhraseur::pers3S ;

  string sPrincipal, sAuxilliaire ;
  donneVerbe(&sPrincipal, &sAuxilliaire) ;

  // Phrase affirmative

  // On traite les adverbes.
  // Si la phrase est négative, on les met entre le verbe être et
  // l'auxilliaire.
  //
  if (pSB->isAffirmative())
  {
    if (string("") != sAuxilliaire)
      pSB->addToVerb(string(" ") + sAuxilliaire) ;
    if (string("") != sPrincipal)
      pSB->addToVerb(string(" ") + sPrincipal) ;
  }
  // Phrase négative
  //
  else
  {
    if (string("") != sAuxilliaire)
    {
      if (CommenceParVoyelle(sAuxilliaire))
        pSB->addToVerb(string(" n'") + sAuxilliaire + pSB->getModifAdv() + string("pas")) ;
      else
        pSB->addToVerb(string(" ne ") + sAuxilliaire + pSB->getModifAdv() + string("pas")) ;

      if (string("") != sPrincipal)
        pSB->addToVerb(string(" ") + sPrincipal) ;
    }
    else
    {
      // On verifie si le verbe commence par une voyelle ou un h muet
      if (CommenceParVoyelle(sPrincipal))
        pSB->addToVerb(string(" n'") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
      else
      {
        NSPhraseMot* pVerb = *(_pPh->Verbe.begin()) ;

        NSPathologData* pData = pVerb->getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pData)
          return false ;

        if (pData->chercheGrammaire(VALEUR_DE_H) == H_MUET)
          pSB->addToVerb(string(" n'") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
        else
          pSB->addToVerb(string(" ne ") + sPrincipal + pSB->getModifAdv() + string("pas")) ;
      }
    }
  }

  if (pSB->isAffirmative())
  {
    _sPhrase += pSB->getVerb() + string(" ") + sVb ;

    if (string("") != pSB->getPhraseAdverb())
      _sPhrase += string(" ") + pSB->getPhraseAdverb() ;
  }
  else
  {
    _sPhrase += pSB->getVerb() ;

    if (string("") != pSB->getPhraseAdverb())
      _sPhrase += string(" ") + pSB->getPhraseAdverb() ;

    _sPhrase += string(" ") + sVb ;
  }

  // Après le verbe, on met l'attribut du COD :
  //
  //    J'ai trouve vos propositions interessantes
  // -> Vos propositions ont été trouvées interessantes par moi.
  //
  if (string("") != pSB->getAttCOD())
    _sPhrase += string(" ") + pSB->getAttCOD() ;

  return true ;
}

void
NSGenerateurFr::writeComplements(int iThreshold, string sBefore, string sBeforeNegation, string sAfter, const bool bAboveThreshold)
{
  if (_pCCArray->empty())
    return ;

  for (GenCompIter itCompl = _pCCArray->begin(); itCompl != _pCCArray->end(); itCompl++)
  {
    if (((true  == bAboveThreshold) && ((*itCompl)->getPriority() >= iThreshold)) ||
        ((false == bAboveThreshold) && ((*itCompl)->getPriority() < iThreshold)))
    {
      if ((*itCompl)->getPositif() != string(""))
        _sPhrase += sBefore + (*itCompl)->getPositif() + sAfter ;
      if ((*itCompl)->getNegatif() != string(""))
        _sPhrase += sBeforeNegation + (*itCompl)->getNegatif() + sAfter ;
    }
  }
}

bool
NSGenerateurFr::assembleProposition(DCODETYPE /* iStyle */, NsProposition* pPropos)
{
  NsProposition* pCurrentProp ;
  if (pPropos)
    pCurrentProp = pPropos ;
  else
    pCurrentProp = _pProposition ;

  if ((NsProposition*) NULL == pCurrentProp)
    return false ;

  if (NsProposition::isPropositionArray != pCurrentProp->iObjectType)
    return true ;

  NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pCurrentProp->pProposition)) ;
  if (pPropArray->empty())
    return true ;

  iterProposition itPr = pPropArray->begin() ;
  pCurrentProp->setPhrase((*itPr)->getPhrase()) ;

  itPr++ ;

  while (pPropArray->end() != itPr)
  {
    NsProposition* pItProp = *itPr ;
    itPr++ ;

    if (string("") != pItProp->getPhrase())
    {
      switch (pItProp->iPropositionType)
      {
        case NsProposition::principale :
          if (pPropArray->end() != itPr)
            pCurrentProp->addToPhrase(string(", ") + pItProp->getPhrase()) ;
          else
            pCurrentProp->addToPhrase(string(" et ") + pItProp->getPhrase()) ;
          break ;
        //
        // Completives
        //
        case NsProposition::completiveQue :                          // SCQ : Je veux que tu reviennes
          pCurrentProp->addToPhrase(string(" que ") + pItProp->getPhrase()) ;
          break ;
        case NsProposition::completiveInfinitive :                   // SCI : J'entends les chats miauler
          pCurrentProp->addToPhrase(pItProp->getPhrase()) ;
          break ;
        case NsProposition::completiveInterrogative :                // SCN : Je voudrais savoir pourquoi tu ne veux pas
          pCurrentProp->addToPhrase(string(" pourquoi ") + pItProp->getPhrase()) ;
          break ;
        //
        // Circonstancielles
        //
        // - Temporelle
        case NsProposition::circonstancielleTemporelleAnterieure :   // SCTA : Je le verrai avant qu'il chante
          switch (pItProp->iConjonctionType)
          {
            case NsProposition::SCTA_AvantQue :        // ---X---|
              pCurrentProp->addToPhrase(string(" avant que ") + pItProp->getPhrase()) ;
              break ;
            case NsProposition::SCTA_JusteAvantQue :   // ------X|
              pCurrentProp->addToPhrase(string(" juste avant que ") + pItProp->getPhrase()) ;
              break ;
            case NsProposition::SCTA_JusquAceQue :     // --XXXXX|
              pCurrentProp->addToPhrase(string(" jusqu'à ce que ") + pItProp->getPhrase()) ;
              break ;
            default :
              pCurrentProp->addToPhrase(string(" avant que ") + pItProp->getPhrase()) ;
          }
          break ;
        case NsProposition::circonstancielleTemporelleSimultanee :   // SCTS : Je chantais au moment où il arriva
          pCurrentProp->addToPhrase(string(" au moment où ") + pItProp->getPhrase()) ;
          break ;
        case NsProposition::circonstancielleTemporellePosterieure :  // SCTP : Je chanterai après que vous serez parti
          switch (pItProp->iConjonctionType)
          {
            case NsProposition::SCTP_ApresQue :          // ---|---X
              pCurrentProp->addToPhrase(string(" après que ") + pItProp->getPhrase()) ;
              break ;
            case NsProposition::SCTP_DesQue :            // ---|X---
              pCurrentProp->addToPhrase(string(" dès que ") + pItProp->getPhrase()) ;
              break ;
            case NsProposition::SCTP_DepuisQue :         // ---|XXXX
              pCurrentProp->addToPhrase(string(" depuis que ") + pItProp->getPhrase()) ;
              break ;
            default :
              pCurrentProp->addToPhrase(string(" avant que ") + pItProp->getPhrase()) ;
          }
          break ;
        // - Consécutive -> résultat de l'action
        case NsProposition::circonstancielleConsecutive :            // SCR : Il fait trop froid pour que je chante
          pCurrentProp->addToPhrase(string(" pour que ") + pItProp->getPhrase()) ;
          break ;
        // - Causale -> cause de l'action
        case NsProposition::circonstancielleCausale :                // SCC : Il a réussi parce qu'il chantait bien
          pCurrentProp->addToPhrase(string(" parce que ") + pItProp->getPhrase()) ;
          break ;
        // - Concessive -> relation inattendue
        case NsProposition::circonstancielleConcessive :             // SCS : Il a échoué bien qu'il chanta bien
          pCurrentProp->addToPhrase(string(" bien que ") + pItProp->getPhrase()) ;
          break ;
        // - Finale -> but de l'action
        case NsProposition::circonstancielleFinale :                 // SCF : Il chante fort pour qu'on l'entende bien
          pCurrentProp->addToPhrase(string(" afin que ") + pItProp->getPhrase()) ;
          break ;
        // - Comparative -> comparaison
        case NsProposition::circonstancielleComparativeProportion :  // SCCP : Vous chanterez d'autant plus fort que vous serez bien échauffé
          switch (pItProp->iConjonctionType)
          {
            case NsProposition::SCCP_AutantQuantite :    // suivant, selon, à mesure
                pCurrentProp->addToPhrase(string(" à mesure que ") + pItProp->getPhrase()) ;
                break ;
            case NsProposition::SCCP_Synchrone :         // au fur et à mesure
                pCurrentProp->addToPhrase(string(" au fur à mesure que ") + pItProp->getPhrase()) ;
                break ;
            case NsProposition::SCCP_PlusQuantite :      // d'autant plus
                pCurrentProp->addToPhrase(string(" d'autant plus que ") + pItProp->getPhrase()) ;
                break ;
            case NsProposition::SCCP_PlusVite :          // d'autant plus vite
                pCurrentProp->addToPhrase(string(" d'autant plus vite que ") + pItProp->getPhrase()) ;
                break ;
            case NsProposition::SCCP_MoinsQuantite :      // d'autant moins
                pCurrentProp->addToPhrase(string(" d'autant moins que ") + pItProp->getPhrase()) ;
                break ;
            case NsProposition::SCCP_MoinsVite :          // d'autant moins vite
                pCurrentProp->addToPhrase(string(" d'autant moins vite que ") + pItProp->getPhrase()) ;
                break ;
          }
          break ;
        case NsProposition::circonstancielleComparativeEgalite :     // SCCE : Vous chantez aussi fort que moi
          switch (pItProp->iConjonctionType)
          {
            case NsProposition::SCCE_Aspect :            // tel que, de même que, si ... que
              pCurrentProp->addToPhrase(string(" tel que ") + pItProp->getPhrase()) ;
              break ;
            case NsProposition::SCCE_Quantite :          // autant que, aussi que
              pCurrentProp->addToPhrase(string(" autant que ") + pItProp->getPhrase()) ;
              break ;
            case NsProposition::SCCE_Temps :             // tant que
              pCurrentProp->addToPhrase(string(" tant que ") + pItProp->getPhrase()) ;
              break ;
          }
          break ;
        case NsProposition::circonstancielleComparativeRessemblance : // SCCR : Tu me regardes comme me regardait ta mère
          pCurrentProp->addToPhrase(string(" comme ") + pItProp->getPhrase()) ;
          break ;
        // - Conditionnelle -> condition
        case NsProposition::circonstancielleConditionnelleHypothese : // SCHH : il mange s'il a faim, il mangera s'il a faim
          pCurrentProp->addToPhrase(string(" si ") + pItProp->getPhrase()) ;
          break ;
        case NsProposition::circonstancielleConditionnelleImaginee :  // SCHH : il mangerait s'il avait faim
          pCurrentProp->addToPhrase(string(" si ") + pItProp->getPhrase()) ;
          break ;
        default :
          pCurrentProp->addToPhrase(string(", ") + pItProp->getPhrase()) ;
      }
    }
  }
  return true ;
}

// donneLibelleAffiche : sert à faire les bons appels à donneLibelleAffiche de
// NSPathologData.

void
NSGenerateurFr::donneLibelleAffiche(string* pLibelle, NSPathologData* pData, const GENRE iGenre)
{
  // En francais, cette methode ne sert que pour les noms et les adjectifs.
  // Ceux-ci peuvent etre MS, MP, FS ou FP.

  int iDeclinaison = 0 ;
  *pLibelle = string("") ;

  if ((NSPathologData*) NULL == pData)
    return ;

  if (pData->estNom())
  {
    if (isPlural(iGenre))
      pData->donneLibelleAffiche(pLibelle, declinaisonPluriel) ;
    else
      pData->donneLibelleAffiche(pLibelle, declinaisonSingulier) ;

    return ;
  }
  else if (pData->estAdjectif())
  {
    switch (iGenre)
    {
      case genreMP : iDeclinaison = declinaisonMP ; break ;
      case genreFP : iDeclinaison = declinaisonFP ; break ;
      case genreMS : iDeclinaison = declinaisonMS ; break ;
      case genreFS : iDeclinaison = declinaisonFS ; break ;
      default      : iDeclinaison = 0 ;
    }

    pData->donneLibelleAffiche(pLibelle, iDeclinaison) ;
    return ;
  }
  else
    pData->donneLibelleAffiche(pLibelle) ;
}

// Elaborating natural language string for adverbs
//
// If adverbs are all in negative form, the verb is switched to negative mode
//
// Take care never to write "*pbPhraseAffirmative = true" since adverbs mode
// is not enough for this - sentence may have been declared as negative globally
//
bool
NSGenerateurFr::prepareVerbe(SentenceBlocks* pSentenceBlocks, DCODETYPE iStyle)
{
  if ((_pPh->Verbe).empty())
    return true ;

  string          sCertitude;
  iterPhraseMot   iterMots;
  NSPathologData  Data;

  string sPhrasePos = string("") ;
  string sPhraseNeg = string("") ;
  string sInterPos1 = string("") ;
  string sInterPos2 = string("") ;
  string sInterNeg1 = string("") ;
  string sInterNeg2 = string("") ;

  NSPhraseMot* pFirstVerb = *(_pPh->Verbe.begin()) ;
  if ((NSPhraseMot*) NULL == pFirstVerb)
    return true ;

  NSPhraseurArray* pComplements = pFirstVerb->getComplementPhr() ;
  if (((NSPhraseurArray*) NULL == pComplements) || pComplements->empty())
    return true ;

  for (phraseurIter CpltIt = pComplements->begin() ;
                              pComplements->end() != CpltIt ; CpltIt++)
  {
    NSPhraseur* pCompVerbe = *CpltIt ;

    if (false == (pCompVerbe->adverbe).empty())
    {
      NsProposition Propos(pContexte, &pCompVerbe, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
      NSGenerateurFr GeneAdverbe(pContexte, &Propos, sLang) ;
      GeneAdverbe.classeTout() ;

      if (GeneAdverbe._pAdverbePos)
      {
        if (false == (GeneAdverbe._pAdverbePos)->empty())
        {
          for (iterMots  = (GeneAdverbe._pAdverbePos)->begin() ;
               iterMots != (GeneAdverbe._pAdverbePos)->end() ;
               iterMots++)
          {
            if (false == (*iterMots)->estTexteLibre())
              sInterPos2 = donneAdverbeComplet(*iterMots) ;
            else
              sInterPos2 = (*iterMots)->getTexteLibre() ;

            etDuMilieu(&sPhrasePos, &sInterPos1, &sInterPos2) ;
          }
          etFinal(&sPhrasePos, &sInterPos1) ;

          pSentenceBlocks->addPhraseAdverb(sPhrasePos) ;
        }
      }

      if (GeneAdverbe._pAdverbeNeg)
      {
        if (false == (GeneAdverbe._pAdverbeNeg)->empty())
        {
          // s'il existe des adverbes à sens positif.
          //
          if (string("") != sPhrasePos)
          {
            for (iterMots  = (GeneAdverbe._pAdverbeNeg)->begin() ;
                 iterMots != (GeneAdverbe._pAdverbeNeg)->end() ;
                 iterMots++)
            {
              if (false == (*iterMots)->estTexteLibre())
                sInterNeg2 = donneAdverbeComplet(*iterMots) ;
              else
                sInterNeg2 = (*iterMots)->getTexteLibre() ;

              etDuMilieu(&sPhraseNeg, &sInterNeg1, &sInterNeg2) ;
            }
            etFinal(&sPhraseNeg, &sInterNeg1) ;

            pSentenceBlocks->addPhraseAdverb(string(" mais ") + sPhraseNeg) ;
          }

          // s'il n'y a pas d'adverbe a sens positif,
          // la phrase devient négative.
          //
          else
          {
            pSentenceBlocks->setAffirmative(false) ;

            // On prend l'adverbe modificateur du premier adverbe
            // du COD négatif

            bool bPremier = true;
            for (iterMots  = (GeneAdverbe._pAdverbeNeg)->begin() ;
                    (GeneAdverbe._pAdverbeNeg)->end() != iterMots ;
                        iterMots++)
            {
              sInterNeg2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle, sansCertitude) ;
              if (strlen((*iterMots)->getCertitude().c_str()) >= 5)
                sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;
              else
                sCertitude = string("") ;

              if (string("") != sCertitude)
              {
                if (bPremier)
                {
                  bPremier = false ;
                  if (string("") != sCertitude)
                  {
                    if (string("WCE00") == sCertitude)
                      pSentenceBlocks->setModifAdv(string(" ")) ;
                    else if ((sCertitude > string("WCE00")) &&
                             (sCertitude < string("WCE50")))
                      pSentenceBlocks->setModifAdv(string(" probablement ")) ;
                  }
                }
                else
                {
                  if (string("") != sCertitude)
                  {
                    if (string("WCE00") == sCertitude)
                      sInterNeg2 = string("pas ") /* + sInter2*/ ;
                    else if ((sCertitude > string("WCE00")) &&
                             (sCertitude < string("WCE50")))
                      sInterNeg2 = string("probablement pas ") /* + sInter2*/ ;
                  }
                }
              }
              etDuMilieu(&sPhraseNeg, &sInterNeg1, &sInterNeg2) ;
            }
            etFinal(&sPhraseNeg, &sInterNeg1) ;

            pSentenceBlocks->setPhraseAdverb(sPhraseNeg) ;
          }
        }
      }
    }
  }
  return true ;
}

bool
NSGenerateurFr::prepareSujet(SentenceBlocks* pSentenceBlocks, DCODETYPE iStyle)
{
  if ((SentenceBlocks*) NULL == pSentenceBlocks)
    return false ;

  string   sCertitude = string("") ;
  GENRE    iGenre     = genreNull ;

  string   sAdverbeModificateurAtt = string(" ") ;

  //
  // Le sujet doit être un nom - Subject must be a noun
  //
  if (false == _pPh->Sujet.empty())
  {
    string sSubject = string("") ;
    string sInter1  = string("") ;
    string sInter2  = string("") ;

    // On itère sur tous les sujets
    //
    for (iterPhraseMot iterMots = _pPh->Sujet.begin() ; _pPh->Sujet.end() != iterMots ; iterMots++)
    {
      string sLexique = (*iterMots)->getLexique() ;
      string sPluriel = (*iterMots)->getPluriel() ;

      NSPathologData Data ;

      // A propos de textes libres.
      // On donne le genre MS à tous les textes libres, par défaut.
      // On ne peut pas traiter grammaticalement les textes libres.

      if (false == (*iterMots)->estTexteLibre())
      {
        NSPathologData* pPathologData = (*iterMots)->getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pPathologData)
          return false ;

        Data = *pPathologData ;

        if (false == Data.estNom())
          return false ;
        Data.donneGenre(&iGenre) ;

        // Récupération du genre (composition du genre récupéré avec
        // le genre déjà existant)
        //
        if (string("") != sPluriel)
          Data.donneGenrePluriel(&iGenre) ;

        string sForceNbr = (*iterMots)->forceNombre(this) ;

        if      (FORCE_PLUR == sForceNbr)
          Data.donneGenrePluriel(&iGenre) ;
        else if (FORCE_SING == sForceNbr)
          Data.donneGenreSingulier(&iGenre) ;
      }
      else
        iGenre = genreMS ;

      // Premier genre attribué, prendre celui du sujet en cours
      // Attributing a gender for first time, take subject's gender
      //
      if (genreNull == pSentenceBlocks->getGenreSujet())
        pSentenceBlocks->setGenreSujet(iGenre) ;

      // Genre déjà attribué (donc déjà pluriel ou passe au pluriel) ; adapter selon celui du sujet en cours
      // Gender already set (hence already plural or switch to plural) ; adapt it from current subject
      //
      else
      {
        if (isFeminin(pSentenceBlocks->getGenreSujet()) && (Data.estFeminin()))
          pSentenceBlocks->setGenreSujet(genreFP) ;
        else
          pSentenceBlocks->setGenreSujet(genreMP) ;
      }

      // L'article dépend du statut d'objet réel ou abstrait
      // Pour un texte libre, on va chercher son libellé dans la table
      // des textes libres.

      if (false == (*iterMots)->estTexteLibre())
      {
        if (((string("") == sPluriel) && (Data.estReel())) || (string(sPluriel, 0, 5) == "WPLUS"))
        {
          if (dcTiret != iStyle)
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle) ;
          else
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle) ;
        }
        else if (((string("") == sPluriel) && (false == Data.estReel())) || (string(sPluriel, 0, 5) == "WPLUR"))
          sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle) ;
      }
      else
        sInter2 = (*iterMots)->getTexteLibre() ;

      etDuMilieu(&sSubject, &sInter1, &sInter2);
    }

    etFinal(&sSubject, &sInter1) ;

    pSentenceBlocks->setSujet(sSubject) ;
  }

  // L'attribut du sujet est un adjectif qualificatif qui s'accorde avec le sujet
  //
  // ex : La valve est dilatée
  //
  // L'attribut doit être un adjectif
  //
  // On met d'abord les attributs positifs, puis les négatifs, avec "mais pas" entre (sauf dans le cas groupSimple).
  // S'il n'y a que des négatifs, la phrase devient négative.
  //
  // ex : La valve n'est pas dilatée.
  // 		  La valve est dilatée, mais pas détruite.
  //
  string sInter1 = string("") ;
  string sInter2 = string("") ;

  // Attributs sur le mode positif
  //
  string sAttSujetPos = string("") ;
  if (_pAttSujetPos && (false == _pAttSujetPos->empty()))
  {
    if (_pPh->Sujet.empty())
      return false ;

    bool bNegativeInAffirmativeSequence = (NSPhraseMotArray::groupMixedWithNon == _pAttSujetPos->getGroupType()) ;

    for (iterPhraseMot iterMots = _pAttSujetPos->begin() ; _pAttSujetPos->end() != iterMots ; iterMots++)
    {
      if (false == (*iterMots)->estTexteLibre())
        // On impose le genre du sujet
        sInter2 = donneAdjectifComplet(*iterMots, pSentenceBlocks->getGenreSujet(), avecCertitude, bNegativeInAffirmativeSequence) ;
      else
        sInter2 = (*iterMots)->getTexteLibre() ;

      etDuMilieu(&sAttSujetPos, &sInter1, &sInter2) ;
    }

    if ((_pPh->AttSujet.getGroupType() != NSPhraseMotArray::groupSimple) ||
        ((NSPhraseMotArray*) NULL == _pAttSujetNeg) || _pAttSujetNeg->empty())
      etFinal(&sAttSujetPos, &sInter1) ;
    else
    {
      etDuMilieu(&sAttSujetPos, &sInter1, &sInter2) ;
      etDuMilieu(&sAttSujetPos, &sInter1, &sInter2) ;
    }

    sInter1 = string("") ;
    sInter2 = string("") ;
  }

  // Attributs sur le mode negatif
  //
  string sAttSujetNeg = string("") ;
  if (_pAttSujetNeg && (false == _pAttSujetNeg->empty()))
  {
    if (_pPh->Sujet.empty())
      return false ;

    bool bActiveVerb = false ;
    if ((_pPh->AttSujet.getGroupType() == NSPhraseMotArray::groupSimple) &&
        (string("") != sAttSujetPos))
      bActiveVerb = true ;

    bool bPremier = true ;

    for (iterPhraseMot iterMots = _pAttSujetNeg->begin() ; _pAttSujetNeg->end() != iterMots ; iterMots++)
    {
      // On impose le genre du sujet
      sInter2 = donneAdjectifComplet(*iterMots, pSentenceBlocks->getGenreSujet(), sansCertitude) ;

      sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;

      // Premier attribut de l'énumération
      //
      if (bPremier)
      {
        bPremier = false ;
        if (string("") != sCertitude)
        {
          if (sCertitude == "WCE00")
            sAdverbeModificateurAtt = string(" ") ;
          else if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
            sAdverbeModificateurAtt = string(" probablement ") ;
        }
      }
      else
        sInter2 = donneCertitude(sCertitude, (bool*) 0, bActiveVerb) + string(" ") + sInter2 ;

      etDuMilieu(&sAttSujetNeg, &sInter1, &sInter2) ;
    }
    etFinal(&sAttSujetNeg, &sInter1) ;
  }

  // Pas d'attribut du sujet positif
  //
  if (string("") == sAttSujetPos)
  {
    // Mais un attribut du sujet négatif
    //
    if (string("") != sAttSujetNeg)
    {
      if (pSentenceBlocks->isAffirmative())
      {
        pSentenceBlocks->setAffirmative(false) ;
        pSentenceBlocks->setAttSujet(sAttSujetNeg) ;
        pSentenceBlocks->setModifAttAdv(sAdverbeModificateurAtt) ;
      }
      else
      {
        // il y a déjà un adverbe du verbe qui rend la phrase négative.
        string sNewAttSujet = sAdverbeModificateurAtt + string("pas ") + sAttSujetNeg ;
        // on enlève les blancs initaux.
        strip(sNewAttSujet, stripLeft) ;

        pSentenceBlocks->setAttSujet(sNewAttSujet) ;
      }
    }
  }
  // Il existe un attribut du sujet positif
  //
  else
  {
    string sNewAttSujet = sAttSujetPos ;

    // Il existe également un attribut du sujet négatif
    //
    // Usuellement la phrase est du type "La valve est dilatée, mais pas détruite.".
    //
    // Mais si la négation est "physiologique", il ne faut pas mettre "mais pas"
    // par exemple, "Il est fonctionnel et non dilaté".
    // C'est ce qui est indiqué par le modificateur "groupSimple"
    //
    if (string("") != sAttSujetNeg)
    {
      if (_pPh->AttSujet.getGroupType() != NSPhraseMotArray::groupSimple)
        sNewAttSujet += string(" mais") + sAdverbeModificateurAtt + string ("pas ") + sAttSujetNeg ;
      else
      {
        if (_pAttSujetNeg->size() > 1)
          sNewAttSujet += string(",") ;
        else
          sNewAttSujet += string(" et") ;
        sNewAttSujet += sAdverbeModificateurAtt + string ("non ") + sAttSujetNeg ;
      }
    }
    pSentenceBlocks->setAttSujet(sNewAttSujet) ;
  }

  return true ;
}

bool
NSGenerateurFr::prepareCOD(SentenceBlocks* pSentenceBlocks, DCODETYPE iStyle)
{
  if ((SentenceBlocks*) NULL == pSentenceBlocks)
    return false ;

  pSentenceBlocks->setCOD(string("")) ;
  pSentenceBlocks->setAttCOD(string("")) ;
  pSentenceBlocks->setGenreCOD(genreNull) ;

  string sPluriel = string("") ;

  // D'abord on va récuperer le genre du COD

  if (false == _pPh->COD.empty())
  {
    for (iterPhraseMot iterMots = _pPh->COD.begin() ; _pPh->COD.end() != iterMots ; iterMots++)
    {
      string sLexique = (*iterMots)->getLexique() ;

      sPluriel = (*iterMots)->getPluriel() ;

      GENRE iGenre = genreNull ;

      NSPathologData Data ;

      if (false == (*iterMots)->estTexteLibre())
      {
        NSPathologData* pPathologData = (*iterMots)->getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pPathologData)
          return false ;

        Data = *pPathologData ;

        if (false == Data.estNom())
          return false ;

        // Récupération du genre (composition du genre récupéré avec
        // le genre déjà existant)
        Data.donneGenre(&iGenre) ;
        if (string("") != sPluriel)
          Data.donneGenrePluriel(&iGenre) ;

        string sForceNbr = (*iterMots)->forceNombre(this) ;
        if      (FORCE_PLUR == sForceNbr)
          Data.donneGenrePluriel(&iGenre) ;
        else if (FORCE_SING == sForceNbr)
          Data.donneGenreSingulier(&iGenre) ;
      }
      else
        iGenre = genreMS ;

      if (genreNull == pSentenceBlocks->getGenreCOD())
        pSentenceBlocks->setGenreCOD(iGenre) ;
      else
      {
        if (isFeminin(pSentenceBlocks->getGenreCOD()) && (Data.estFeminin()))
          pSentenceBlocks->setGenreCOD(genreFP) ;
        else
          pSentenceBlocks->setGenreCOD(genreMP) ;
      }
    }
  }

  // On traite ensuite successivement les noms du COD à certitude positive, puis ceux
  // à certitude negative.
  // Pour une dcTiret, il ne faut pas mettre d'article dans le COD.

  string sCODPos = string("") ;

  if (_pCODPos)
  {
    if (false == _pCODPos->empty())
    {
      string sInter1 = string("") ;
      string sInter2 = string("") ;

      for (iterPhraseMot iterMots = _pCODPos->begin() ; _pCODPos->end() != iterMots ; iterMots++)
      {
        if (dcPhrase == iStyle)
        {
          if (false == (*iterMots)->estTexteLibre())
          {
            if (string("") == sPluriel)
              sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle, avecCertitude, levelCOD) ;

            else if (string("WPLUS") == string(sPluriel, 0, 5))
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle, avecCertitude, levelCOD) ;
            else if (string("WPLUR") == string(sPluriel, 0, 5))
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle, avecCertitude, levelCOD) ;
          }
          else
            sInter2 = (*iterMots)->getTexteLibre() ;
        }

        else if (dcTiret == iStyle)
        {
          if (false == (*iterMots)->estTexteLibre())
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle, avecCertitude, levelCOD) ;
          else
            sInter2 = (*iterMots)->getTexteLibre() ;
        }

        etDuMilieu(&sCODPos, &sInter1, &sInter2) ;
      }
      etFinal(&sCODPos, &sInter1) ;

      pSentenceBlocks->addToCOD(sCODPos) ;
    }
  }

  string sCODNeg = string("") ;

  if (_pCODNeg)
  {
    if (false == _pCODNeg->empty())
    {
      if ((string("") != sCODPos) || (false == pSentenceBlocks->isAffirmative()))
      {
        string sInter1 = string("") ;
        string sInter2 = string("") ;

        for (iterPhraseMot iterMots = _pCODNeg->begin() ; _pCODNeg->end() != iterMots ; iterMots++)
        {
          if (dcPhrase == iStyle)
          {
            if (string("") == sPluriel)
              sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle, avecCertitude, levelCOD) ;
            else if (string(sPluriel, 0, 5) == "WPLUS")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle, avecCertitude, levelCOD) ;
            else if (string(sPluriel, 0, 5) == "WPLUR")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle, avecCertitude, levelCOD) ;
          }
          else if (dcTiret == iStyle)
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle, avecCertitude, levelCOD) ;

          etDuMilieu(&sCODNeg, &sInter1, &sInter2) ;
        }
        etFinal(&sCODNeg, &sInter1) ;

        pSentenceBlocks->addToCOD(string(" mais ") + sCODNeg) ;
      }
      //
      // Pas de COD à sens affirmatif
      //
      else
      {
        pSentenceBlocks->setAffirmative(false) ;

        // On prend l'adverbe modificateur du premier nom du COD négatif

        bool bPremier = true ;
        string sInter1 = string("") ;
        string sInter2 = string("") ;

        for (iterPhraseMot iterMots = _pCODNeg->begin() ; _pCODNeg->end() != iterMots ; iterMots++)
        {
          if (dcPhrase == iStyle)
          {
            if (string("") == sPluriel)
              sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle, sansCertitude, levelCOD) ;
            else if (string(sPluriel, 0, 5) == "WPLUS")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle, sansCertitude, levelCOD) ;
            else if (string(sPluriel, 0, 5) == "WPLUR")
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle, sansCertitude, levelCOD) ;

            string sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;

            if (string("") != sCertitude)
            {
              // It is the first COD in the list (ex, "fièvre" dans "pas de fièvre, ni de prurit")
              //
              if (bPremier)
              {
                bPremier = false ;
                if (string("") != sCertitude)
                {
                  if (string(sCertitude, 0 ,5) == string("WCE00"))
                    pSentenceBlocks->setModifAdv(string(" ")) ;
                  else if ((string(sCertitude, 0 ,5) > string("WCE00")) &&
                           (string(sCertitude, 0 ,5) < string("WCE50")))
                    pSentenceBlocks->setModifAdv(string(" probablement ")) ;
                }
              }
              else
              {
                if (string("") != sCertitude)
                {
                  if (string(sCertitude, 0 ,5) == string("WCE00"))
                    // sInter2 = string("pas ") + sInter2 ;
                    sInter2 = string("ni ") + sInter2 ;
                  else if ((string(sCertitude, 0 ,5) > string("WCE00")) &&
                           (string(sCertitude, 0 ,5) < string("WCE50")))
                    sInter2 = string("ni probablement ") + sInter2 ;
                }
              }
            }
          }

          else if (dcTiret == iStyle)
            // sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle, avecCertitude, levelCOD) ;
            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle, avecCertitude, levelCOD) ;

          etDuMilieu(&sCODNeg, &sInter1, &sInter2) ;
        }
        // etFinal(&sCODNeg, &sInter1) ; // ni, ni (pas ni et ni)
        etDuMilieu(&sCODNeg, &sInter1, &sInter1) ;

        pSentenceBlocks->setCOD(sCODNeg) ;
      }
    }
  }

  // Attribut du COD (se place après les adjectifs qualificatifs qualifiant le COD).

  string sAttCODPos = string("") ;

  if (_pAttCODPos)
  {
    if (false == _pAttCODPos->empty())
    {
      if (_pPh->COD.empty())
        return false ;

      string sInter1 = string("") ;
      string sInter2 = string("") ;

      // In "groupMixedWithNon", positive and negative are in the same flow
      //
      bool bNegativeInAffirmativeSequence = (NSPhraseMotArray::groupMixedWithNon == _pAttCODPos->getGroupType()) ;

      for (iterPhraseMot iterMots = _pAttCODPos->begin() ; _pAttCODPos->end() != iterMots ; iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
          // On impose le genre du COD
          sInter2 = donneAdjectifComplet(*iterMots, pSentenceBlocks->getGenreCOD(), avecCertitude, bNegativeInAffirmativeSequence) ;
        else
          sInter2 = (*iterMots)->getTexteLibre() ;

        etDuMilieu(&sAttCODPos, &sInter1, &sInter2);
      }
      etFinal(&sAttCODPos, &sInter1);
    }
  }

  string sAttCODNeg = string("") ;
  if (_pAttCODNeg)
  {
    if (false == _pAttCODNeg->empty())
    {
      if (_pPh->COD.empty())
        return false ;

      string sInter1 = string("") ;
      string sInter2 = string("") ;

      for (iterPhraseMot iterMots = _pAttCODNeg->begin() ; _pAttCODNeg->end() != iterMots ; iterMots++)
      {
        // On impose le genre du COD
        sInter2 = donneAdjectifComplet(*iterMots, pSentenceBlocks->getGenreCOD()) ;

        etDuMilieu(&sAttCODNeg, &sInter1, &sInter2) ;
      }
      etFinal(&sAttCODNeg, &sInter1);
    }
  }

  if (string("") == sAttCODPos)
  {
    if (string("") != sAttCODNeg)
      pSentenceBlocks->setAttCOD(sAttCODNeg) ;
  }
  else
  {
    pSentenceBlocks->setAttCOD(sAttCODPos) ;
    if (string("") != sAttCODNeg)
      pSentenceBlocks->addToAttCOD(string(" mais ") + sAttCODNeg) ;
  }

  return true ;
}

// donneNomComplet : méthode qui sert à fabriquer un nom, accompagné de tous ses
// compléments. On peut lui demander de ne pas gérer la certitude du nom, ce qui
// est utile notamment dans les subordonnées circonstancielles :
//
// ex : Il existe un polype au niveau de la bouche mais pas de la langue.
//
// Dans ce cas, la certitude se place avant la préposition, donc ne doit pas
// être gérée par donneNomComplet.

string
NSGenerateurFr::donneNomComplet(NSPhraseMot* pMot, NSPhraseMot::TYPEARTICLE iArticle, DCODETYPE iStyle, GERECERTITUDE iGereCertitude, GENERATEDLEVEL iGeneratedLevel)
{
  string sLibel = string("") ;
  NSPathologData Data ;

  iterPhraseMot iterMots ;

  string sCertitude = pMot->getCertitude() ;
  if ((string("") != sCertitude) && (strlen(sCertitude.c_str()) > 5))
    sCertitude = string(sCertitude, 0, 5) ;

  string sLexique = pMot->getLexique() ;
  string sConcept = pMot->getConcept() ;
  string sPluriel = pMot->getPluriel() ;
  GENRE iGenre ;
  bool bPluriel = false ;
  string sPrep = string("") ;

  NSPathologData* pPathologData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pPathologData)
  {
    string sWarningTxt = string("NSGenerateurFr::donneNomComplet: cannot find \"") + sConcept + string("\" in Lexique.") ;
    pContexte->getSuperviseur()->trace(&sWarningTxt, 1, NSSuper::trWarning) ;
    return string("") ;
  }

  Data = *pPathologData ;

  if (false == Data.estNom())
  {
    string sWarningTxt = string("NSGenerateurFr::donneNomComplet: \"") + sConcept + string("\" is not a noon in Lexique.") ;
    pContexte->getSuperviseur()->trace(&sWarningTxt, 1, NSSuper::trWarning) ;
    return string("") ;
  }
  // Récupération du genre (composition du genre récupéré avec
  // le genre déjà existant)
  Data.donneGenre(&iGenre) ;
  if (string("") != sPluriel)
    Data.donneGenrePluriel(&iGenre) ;

  // On regarde si le nombre est forcé.

  string sForceNbr = pMot->forceNombre(this) ;

  if      (FORCE_PLUR == sForceNbr)
    Data.donneGenrePluriel(&iGenre) ;
  else if (FORCE_SING == sForceNbr)
    Data.donneGenreSingulier(&iGenre) ;

  //
  if (string("") != sLexique)
    donneLibelleAffiche(&sLibel, &Data, iGenre) ;
  else
    sLibel = getLabelForNum(pMot) ;

  agglutinate(pMot, sLibel) ;

  bool bDontRepeatNoonCompl = false ;

  NSPhraseurArray* pComplements = pMot->getComplementPhr() ;

  if (pComplements && (false == pComplements->empty()))
  {
    for (phraseurIter CpltIt = pComplements->begin() ;
                          pComplements->end() != CpltIt ; CpltIt++)
    {
      NSPhraseur* pComplement = *CpltIt ;

      NsProposition Propos(pContexte, pComplement, NsProposition::notSetType, NsProposition::notSetConjonct) ;
      NSGenerateurFr GeneCpl(pContexte, &Propos, sLang) ;
      GeneCpl.setGeneratdLevel(iGeneratedLevel) ;
      traitePostposition(pMot) ;

      // Le complément du nom est nécessairement introduit par une préposition.
      // Donc si on n'a pas de préposition, on en met une par défaut :
      // En général                     : "de" (ex : "un signe de sténose")
      // Si lésion complétée par examen : "à"  (ex : "sténose à l'échographie"
      //
      if (false == pComplement->compNom.empty())
      {
        for (iterMots = pComplement->compNom.begin() ;
                   pComplement->compNom.end() != iterMots ;
                        iterMots++)
        {
          if (NSPhraseMot::complementAppartenanceNoRepeat == (*iterMots)->getCplmtMgnt())
            bDontRepeatNoonCompl = true ;

          if (false == (*iterMots)->estTexteLibre())
          {
            if ((*iterMots)->getPreposition() == string(""))
            {
              // Appartenance : "l'anévrysme de l'aorte"
              if (NSPhraseMot::cpltAppartenance == (*iterMots)->getTypeAsCplmt())
              {
                string sPrep = string("de ") + donneArticleDefini(*iterMots, false) ;
                (*iterMots)->setPreposition(sPrep) ;
              }
              else
              {
                // Si lésion complétée par examen : "à"  (ex : "sténose à l'échographie"
                if (pPathologData->isHealthProblem() &&
                     (*iterMots)->getLexiqueData(string("fr")) &&
                     (*iterMots)->getLexiqueData(string("fr"))->isProcedure())
                  (*iterMots)->setPreposition(string("à")) ;
                else
                  (*iterMots)->setPreposition(string("de")) ;
              }
            }
          }
          else
          {
            if ((*iterMots)->getPreposition() == string(""))
              (*iterMots)->setTexteLibre(string("de ") + (*iterMots)->getTexteLibre()) ;
          }
        }
      }

      if (false == bDontRepeatNoonCompl)
      {
        DCODETYPE iComplementStyle = iStyle ;
        if ((phraseRelative == pComplement->iPhraseType) &&
                (dcUndefined != pComplement->_iRelativeType))
          iComplementStyle = pComplement->_iRelativeType ;

        GERECERTITUDE iGeneCert = iGereCertitude ;
        if ((string("") != sCertitude) && (sCertitude < "WCE50"))
          iGeneCert = sansCertitude ;

        if (GeneCpl.genereNomComplet(pMot, iArticle, iComplementStyle, iGeneCert))
        {
          sLibel = GeneCpl.getTempoPhrase() ;

          if ((avecCertitude == iGereCertitude) &&
                         (string("") != sCertitude) && (sCertitude < "WCE50"))
            sLibel = donneCertitude(sCertitude) + string(" ") + sLibel ;

          return sLibel ;  // genereNomComplet treats all complements,
        }                  // no need to keep on iterating
      }
      // Complément du nom non répété, exemple :
      //      La hauteur de l'anévrisme est de 10 mm -> Sa hauteur est de 10 mm
      else
      {
        string sAdjPos = donneAdjectifPossessif(pMot, false) ;
        sLibel = sAdjPos + sLibel ;
      }
    }
  }
  //
  // Pas de complément
  //
  else
  {
    // On traite d'abord la préposition
    sPrep = Data.chercheGrammaire(PREPOSITION);

    if (string("") != sPrep)
    {
      if (pMot->getPreposition() != string(""))
        pMot->setPreposition(pMot->getPreposition() + string(" ")) ;
      pMot->setPreposition(pMot->getPreposition() + string(sPrep, 1, strlen(sPrep.c_str()) - 1)) ;
    }

    if (isPlural(iGenre))
      bPluriel = true ;

    bool modifie = modifiePreposition(pMot, iArticle, bPluriel) ;

    if      (NSPhraseMot::articleDefini == iArticle)
      sLibel = donneArticleDefini(pMot, bPluriel) + sLibel ;
    else if (NSPhraseMot::articleIndefini == iArticle)
    {
      // A la forme négative, l'article indéfini est remplacé par un
      // article partitif
      // ex : il existe un polype -> il n'existe pas de polype
//            if ((iGereCertitude == avecCertitude) && (sCertitude < "WCE50"))
      if ((string("") != sCertitude) && (sCertitude < "WCE50"))
        sLibel = donneArticlePartitif(pMot, bPluriel) + sLibel ;
      else
        sLibel = donneArticleIndefini(pMot, bPluriel) + sLibel ;
    }
    else if (NSPhraseMot::articlePartitif == iArticle)
      sLibel = donneArticlePartitif(pMot, bPluriel) + sLibel ;

    if (iGereCertitude == avecCertitude)
    {
      string sCertitudeEcrit = donneCertitude(sCertitude) ;

      if (string("") != sCertitudeEcrit)
        sLibel = sCertitudeEcrit + string(" ") + sLibel ;
    }

    if (modifie)
      sLibel = pMot->getPreposition() + sLibel ;
    else if (pMot->getPreposition() != string(""))
      sLibel = pMot->getPreposition() + string(" ") + sLibel ;
  }

  return sLibel ;
}


// genereNomComplet : méthode qui sert à générer un nom lorsque celui-ci a des compléments.
//
// iGereCertitude = sansCertitude signifie que la certitude ne doit pas être
//                                prise en compte, par exemple parce que la
//                                négation est gérée par le verbe, par exemple
//                                "il n'existe pas de sténose"
//
bool
NSGenerateurFr::genereNomComplet(NSPhraseMot* pMot, NSPhraseMot::TYPEARTICLE iArticle, DCODETYPE iStyle, GERECERTITUDE iGereCertitude)
{
  _sPhrase = string("") ;

  reinitialise() ;

  if ((NSPhraseMot*) NULL == pMot)
    return false ;

  NSPhraseurArray* pCompls = pMot->getComplementPhr() ;
  if (pCompls && (false == pCompls->empty()))
  {
    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
      if (false == ((*ComplIt)->adjEpithete).empty())
        (*ComplIt)->classeAdjectif(this) ;
  }

  traitePrepositionCC(&_pPh->CCLieu,    &_pPh->PrepositionLieu,    NSPhraseur::prepLieu) ;
  traitePrepositionCC(&_pPh->CCTemps,   &_pPh->PrepositionTemps,   NSPhraseur::prepTemps) ;
  traitePrepositionCC(&_pPh->CCManiere, &_pPh->PrepositionManiere, NSPhraseur::prepManiere) ;
  traitePrepositionCC(&_pPh->CCMoyen,   &_pPh->PrepositionMoyen,   NSPhraseur::prepMoyen) ;
  traitePrepositionCC(&_pPh->CCCause,   &_pPh->PrepositionCause,   NSPhraseur::prepCause) ;
  traitePrepositionCC(&_pPh->CCBut,     &_pPh->PrepositionBut,     NSPhraseur::prepBut) ;
  traitePrepositionCC(&_pPh->CCType,    &_pPh->PrepositionType,    NSPhraseur::prepType) ;
  traitePrepositionCC(&_pPh->CCHypoth,  &_pPh->PrepositionHypoth,  NSPhraseur::prepHypoth) ;
  traitePrepositionCC(&_pPh->CCChiffre, &_pPh->PrepositionChiffre, NSPhraseur::prepChiffre) ;

  classeTout() ;

  //
  // Prise des informations sur le nom à compléter
  //
  NSPathologData Data ;

  string sLexique = pMot->getLexique() ;
  string sConcept = pMot->getConcept() ;
  string sPluriel = pMot->getPluriel() ;

  NSPathologData* pPathologData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pPathologData)
  {
    string sWarningTxt = string("NSGenerateurFr::genereNomComplet: cannot find \"") + sConcept + string("\" in Lexique.") ;
    pContexte->getSuperviseur()->trace(&sWarningTxt, 1, NSSuper::trWarning) ;
    return false ;
  }

  Data = *pPathologData ;

  if (false == Data.estNom())
  {
    string sWarningTxt = string("NSGenerateurFr::genereNomComplet: \"") + sConcept + string("\" is not a noon in Lexique.") ;
    pContexte->getSuperviseur()->trace(&sWarningTxt, 1, NSSuper::trWarning) ;
    return false ;
  }

  // Récupération du genre (composition du genre récupéré avec
  // le genre déjà existant)
  //
  GENRE iGenreNom ;
  Data.donneGenre(&iGenreNom) ;
  if (string("") != sPluriel)
    Data.donneGenrePluriel(&iGenreNom) ;

  // On regarde si le nombre est force.

  string sForceNbr = pMot->forceNombre(this) ;

  if      (sForceNbr == FORCE_PLUR)
    Data.donneGenrePluriel(&iGenreNom) ;
  else if (sForceNbr == FORCE_SING)
    Data.donneGenreSingulier(&iGenreNom) ;

  string sNom = string("") ;

  if (string("") != sLexique)
    donneLibelleAffiche(&sNom, &Data, iGenreNom) ;
  else
    sNom = getLabelForNum(pMot) ;
  agglutinate(pMot, sNom) ;

  string sCertitudeNom = string(pMot->getCertitude(), 0, 5) ;

  //
  // Compléments : quand, où, comment, avec quoi, pourquoi, dans quel but
  //
  _pCCArray->donnePhrase(iStyle) ;

  // Expansions du groupe nominal
  //
  // Cas 1 Adjectif épithète     le gros garçon
  //       le plus près du nom, dans adjEpithete
  //
  // Cas 2 Complément du nom     le garçon aux yeux bleus
  //       dans les compléments circonstanciels
  //
  // Cas 3 Subordonnée relative  le garçon qui a pris le ballon
  //       suppose qu'il y ait un verbe
  //

  // On traite d'abord les adjectifs épithètes
  //
  string sPhraseAvPos = string("") ;
  string sPhraseApPos = string("") ;
  string sPhraseApNeg = string("") ;

  // On traite les epithetes qui se placent avant le nom.
  // Ils ont tous une certitude "WCEA0", ou bien pas de certitude.
  // En fait, *_pAdjEpitheteAvNeg est toujours vide.

  if (_pAdjEpitheteAvPos && (false == _pAdjEpitheteAvPos->empty()))
  {
    string sInterAvPos1 = string("") ;
    string sInterAvPos2 = string("") ;

    // In "groupMixedWithNon", positive and negative are in the same flow
    //
    bool bNegativeInAffirmativeSequence = (NSPhraseMotArray::groupMixedWithNon == _pAdjEpitheteAvPos->getGroupType()) ;

    for (iterPhraseMot iterMotsAdj = _pAdjEpitheteAvPos->begin() ;
             _pAdjEpitheteAvPos->end() != iterMotsAdj ;
                   iterMotsAdj++)
    {
      // L'epithete s'accorde avec le nom qu'il qualifie.
      if (false == (*iterMotsAdj)->estTexteLibre())
        sInterAvPos2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom, avecCertitude, bNegativeInAffirmativeSequence) ;
      else
        sInterAvPos2 = (*iterMotsAdj)->getTexteLibre() ;

      if (_pAdjEpitheteAvPos->isUsualSeparator())
        etDuMilieu(&sPhraseAvPos, &sInterAvPos1, &sInterAvPos2) ;
      else
        etDuMilieu(&sPhraseAvPos, &sInterAvPos1, &sInterAvPos2, string(" ")) ;
    }
    if (_pAdjEpitheteAvPos->isUsualSeparator())
      etFinal(&sPhraseAvPos, &sInterAvPos1) ;
    else
      etFinal(&sPhraseAvPos, &sInterAvPos1, string(" ")) ;
  }

  // On traite les adjectifs qui se placent apres le nom.
  //
  // Cas classique, d'abord les affirmatifs, puis les negatifs.
  // Si on a des positifs et des negatifs, on met "mais pas" entre les deux.
  // Si on n'a que des negatifs, on met juste "pas".
  //
  // Cas groupMixedWithNon on mélange tout et on met "non" au lieu de "pas",
  // par exemple "thrombus non occlusif"

  bool bGroupeSimple = (_pAdjEpitheteApPos && (_pAdjEpitheteApPos->getGroupType() == NSPhraseMotArray::groupSimple)) ;

  string sInterAp1 = string("") ;
  string sInterAp2 = string("") ;

  if (_pAdjEpitheteApPos && (false == _pAdjEpitheteApPos->empty()))
  {
    bool bNegativeInAffirmativeSequence = (NSPhraseMotArray::groupMixedWithNon == _pAdjEpitheteApPos->getGroupType()) ;

    for (iterPhraseMot iterMotsAdj = _pAdjEpitheteApPos->begin();
            _pAdjEpitheteApPos->end() != iterMotsAdj ;
                iterMotsAdj++)
    {
      // L'epithete s'accorde avec le nom qu'il qualifie.
      if (false == (*iterMotsAdj)->estTexteLibre())
        sInterAp2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom, avecCertitude, bNegativeInAffirmativeSequence) ;
      else
        sInterAp2 = (*iterMotsAdj)->getTexteLibre() ;

      if (_pAdjEpitheteApPos->isUsualSeparator())
        etDuMilieu(&sPhraseApPos, &sInterAp1, &sInterAp2) ;
      else
        etDuMilieu(&sPhraseApPos, &sInterAp1, &sInterAp2, string(" ")) ;
    }

    if ((false == bGroupeSimple) || ((NSPhraseMotArray*) NULL == _pAdjEpitheteApNeg))
    {
      if (_pAdjEpitheteApPos->isUsualSeparator())
        etFinal(&sPhraseApPos, &sInterAp1) ;
      else
        etFinal(&sPhraseApPos, &sInterAp1, string(" ")) ;
      sInterAp1 = string("") ;
    }
  }

  if (_pAdjEpitheteApNeg)
  {
    // Si groupe simple, on continue l'énumération
    //
    if (bGroupeSimple)
      sPhraseApNeg = sPhraseApPos ;

    for (iterPhraseMot iterMotsAdj = _pAdjEpitheteApNeg->begin() ;
              _pAdjEpitheteApNeg->end() != iterMotsAdj ;
                    iterMotsAdj++)
    {
      // L'epithete s'accorde avec le nom qu'il qualifie.

      sInterAp2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom) ;

      if (_pAdjEpitheteApNeg->isUsualSeparator())
        etDuMilieu(&sPhraseApNeg, &sInterAp1, &sInterAp2) ;
      else
        etDuMilieu(&sPhraseApNeg, &sInterAp1, &sInterAp2, string(" ")) ;
    }
    if (_pAdjEpitheteApNeg->isUsualSeparator())
      etFinal(&sPhraseApNeg, &sInterAp1) ;
    else
      etFinal(&sPhraseApNeg, &sInterAp1, string(" ")) ;

    // Si groupe simple, on continue l'énumération
    //
    if (bGroupeSimple)
    {
      sPhraseApPos = sPhraseApNeg ;
      sPhraseApNeg = string("") ;
    }
  }

  string sPhraseAv = sPhraseAvPos ;
  string sPhraseAp = sPhraseApPos ;

  if (string("") != sPhraseApNeg)
  {
    if (string("") != sPhraseApPos)
      sPhraseAp += string(" mais ") + sPhraseApNeg ;
    else
      sPhraseAp = sPhraseApNeg ;
  }

  // On traite ensuite les complements du nom.
  // Considerons pour simplifier qu'il ne s'agit que de noms.
  // "Il y a suspicion d'endocartite."
  // Le complement du nom est le plus souvent introduit par la preposition "de".

  string sCompNomPos = string("") ;
  string sCompNomNeg = string("") ;

  string sCertitude = string("") ;

  if (_pCompNomPos && (false == _pCompNomPos->empty()))
  {
    string sInterCompNomPos1 = string("") ;
    string sInterCompNomPos2 = string("") ;

    for (iterPhraseMot iterMotsCompNom = _pCompNomPos->begin() ;
           _pCompNomPos->end() != iterMotsCompNom ;
             iterMotsCompNom++)
    {
      sLexique = (*iterMotsCompNom)->getLexique() ;

      if (false == (*iterMotsCompNom)->estTexteLibre())
      {
        NSPathologData* pPathologData = (*iterMotsCompNom)->getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pPathologData)
          return false ;

        NSPathologData DataCompNom = *pPathologData ;

        sCertitude = string((*iterMotsCompNom)->getCertitude(), 0, 5) ;

        sInterCompNomPos2 = donneCertitude(sCertitude) + donneNomComplet(*iterMotsCompNom, (*iterMotsCompNom)->getArticle(), iStyle, sansCertitude) ;
      }
      else
        sInterCompNomPos2 = (*iterMotsCompNom)->getTexteLibre() ;

      etDuMilieu(&sCompNomPos, &sInterCompNomPos1, &sInterCompNomPos2) ;
    }
    etFinal(&sCompNomPos, &sInterCompNomPos1) ;
  }

  if (_pCompNomNeg && (false == _pCompNomNeg->empty()))
  {
    string sInterCompNomNeg1 = string("") ;
    string sInterCompNomNeg2 = string("") ;

    for (iterPhraseMot iterMotsCompNom = _pCompNomNeg->begin() ;
            _pCompNomNeg->end() != iterMotsCompNom ;
               iterMotsCompNom++)
    {
      NSPathologData* pPathologData = (*iterMotsCompNom)->getLexiqueData(sLang) ;
      if ((NSPathologData*) NULL == pPathologData)
        return false ;

      sCertitude = string((*iterMotsCompNom)->getCertitude(), 0, 5) ;
      sInterCompNomNeg2 = donneCertitude(sCertitude) + donneNomComplet(*iterMotsCompNom, (*iterMotsCompNom)->getArticle(), iStyle, sansCertitude) ;

      etDuMilieu(&sCompNomNeg, &sInterCompNomNeg1, &sInterCompNomNeg2) ;
    }
    etFinal(&sCompNomNeg, &sInterCompNomNeg1) ;
  }

  string sCompNom = sCompNomPos ;

  if (string("") != sCompNomNeg)
  {
    if (string("") != sCompNomPos)
      sCompNom += string(" mais ") + sCompNomNeg ;
    else
      sCompNom = sCompNomNeg ;
  }

  string sCertitudeEcrit = donneCertitude(sCertitudeNom);

  if ((string("") != sCertitudeEcrit) && (avecCertitude == iGereCertitude))
  {
    _sPhrase = sCertitudeEcrit ;

    if (string("") != sPhraseAv)
      _sPhrase += string(" ") + sPhraseAv ;
    if (string("") != sNom)
      _sPhrase += string(" ") + sNom ;
    //
    // Modif 17/12/14 epithetes before complements
    //
    if (string("") != sPhraseAp)
      _sPhrase += string(" ") + sPhraseAp ;
    if (string("") != sCompNom)
      _sPhrase += string(" ") + sCompNom ;
/*
    if (string("") != sPhraseAp)
      _sPhrase += string(" ") + sPhraseAp ;
*/
  }
  else
  {
    _sPhrase = sPhraseAv ;

    if (string("") != sPhraseAv)
      _sPhrase += string(" ") ;

    _sPhrase += sNom ;

    //
    // Modif 17/12/14 epithetes before complements
    // Ex "Les segments visualisés de l'artère carotide droite"
    //
    if (string("") != sPhraseAp)
      _sPhrase += string(" ") + sPhraseAp ;

    if (string("") != sCompNom)
      _sPhrase += string(" ") + sCompNom ;

/*
    if (string("") != sPhraseAp)
      _sPhrase += string(" ") + sPhraseAp ;
*/
  }

  // Traitons les subordonnees circonstancielles.

  if (false == _pCCArray->empty())
  {
    for (GenCompIter itCompl = _pCCArray->begin() ; _pCCArray->end() != itCompl ; itCompl++)
    {
      if ((*itCompl)->getPositif() != string(""))
      {
        _sPhrase += string(" ") + (*itCompl)->getPositif() ;
        if ((*itCompl)->getNegatif() != string(""))
          _sPhrase += string(" mais pas ") + (*itCompl)->getNegatif() ;
      }
      else if ((*itCompl)->getNegatif() != string(""))
        _sPhrase += string(" pas ") + (*itCompl)->getNegatif() ;
    }
  }

  bool bPluriel = isPlural(iGenreNom) ;

  // On traite les adjectifs numéraux
  // char NumCard[BASE_COMPLEMENT_LEN + 1];
  // strcpy(NumCard, pComp->adjNumeralCardinal.complement) ;

  bool bExistNumeralCardinal = false ;
  if (pCompls && (false == pCompls->empty()))
  {
    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
      if (string("") != (*ComplIt)->adjNumeralCardinal.getComplement())
      {
        _sPhrase = (*ComplIt)->adjNumeralCardinal.getComplement() + string(" ") + _sPhrase ;
        bExistNumeralCardinal = true ;
      }
  }

  // On traite les adjectifs non qualificatifs (possessifs, démonstratifs...)
  //
  bool bIsAdjNonQualif = false ;

  if (pCompls && (false == pCompls->empty()))
  {
    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
      if (NSPhraseur::adjectifSans != (*ComplIt)->adjNonQualificatif)
      {
        switch((*ComplIt)->adjNonQualificatif)
        {
          case NSPhraseur::adjectifPossessif :
            // TODO : shouldn't be (iGenreNom, iGenreNom)
            _sPhrase = getAdjectifPossessif(iGenreNom, iGenreNom) + string(" ") + _sPhrase ;
            bIsAdjNonQualif = true ;
            break ;
          case NSPhraseur::adjectifDemonstratif :
            _sPhrase = getAdjectifDemonstratif(iGenreNom) + string(" ") + _sPhrase ;
            bIsAdjNonQualif = true ;
            break ;
        }
      }
  }

  // NSPhraseMot      adjNumeralOrdinal ;

  // On traite d'abord la preposition
  //
  string sPrep = Data.chercheGrammaire(PREPOSITION) ;

  if (string("") != sPrep)
  {
    if (pMot->getPreposition() != string(""))
      pMot->setPreposition(pMot->getPreposition() + string(" ")) ;
    pMot->setPreposition(pMot->getPreposition() + string(sPrep, 1, strlen(sPrep.c_str()) - 1)) ;
  }

  bool modifie = modifiePreposition(pMot, iArticle, bPluriel) ;

  // On traite l'article

  // Remarque : en francais, un article ne commence jamais par un "h".

  if (false == bIsAdjNonQualif)  // avoid "la sa maison" or "la cette maison"
  {
    if      (NSPhraseMot::articleDefini == iArticle)
      _sPhrase = donneArticleDefini(pMot, bPluriel, sansCertitude) + _sPhrase ;
    //
    // Pas d'article indéfini lorsqu'il existe un adjectif numéral cardinal
    // On peut écrire "les 3 maisons sont vertes" mais pas "des 3 maisons sont vertes"
    //
    else if ((NSPhraseMot::articleIndefini == iArticle) && (false == bExistNumeralCardinal))
    {
      if ((sCertitudeNom != "") && (sCertitudeNom < "WCE50") /* && (iGereCertitude == avecCertitude) */)
        _sPhrase = donneArticlePartitif(pMot, bPluriel, sansCertitude) + _sPhrase ;
      else
        _sPhrase = donneArticleIndefini(pMot, bPluriel, sansCertitude) + _sPhrase ;
    }
    else if (NSPhraseMot::articlePartitif == iArticle)
      _sPhrase = donneArticlePartitif(pMot, bPluriel, sansCertitude) + _sPhrase ;
  }

  // Apres l'article, on traite la preposition.

  string sPreposition = pMot->getPreposition() ;

  if (modifie)
    _sPhrase = sPreposition + _sPhrase ;
  else if (string("") != sPreposition)
  {
    if ('\'' != sPreposition[strlen(sPreposition.c_str())-1])
      sPreposition += string(" ") ;
    _sPhrase = sPreposition + _sPhrase ;
  }

  if (avecCertitude == iGereCertitude)
  {
    sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude);

    if (string("") != sCertitudeEcrit)
      _sPhrase = sCertitudeEcrit + string(" ") + _sPhrase ;
  }

  // Tout a la fin, on met le complement entre parentheses.

  if (pCompls && (false == pCompls->empty()))
  {
    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
      if ((*ComplIt)->pParentheses)
      {
        NsProposition Propos(pContexte, &((*ComplIt)->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
        NSGenerateurFr GeneParentheses(pContexte, &Propos, sLang) ;

        DCODETYPE dcParenthese = (*ComplIt)->iParenthesesStyle ;
        if (dcUndefined == dcParenthese)
          dcParenthese = iStyle ;

        GeneParentheses.genereProposition(dcParenthese) ;
        _sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
      }
  }

  // On indique que le nom est complete par une subordonnee relative en donnant
  // à iPhraseType la valeur phraseRelative.
  //
  // Dans Nautilus, on considerera 2 cas :
  //
  // 1) la relative n'a pas de sujet : "le garçon qui a pris le ballon"
  //    Alors, c'est l'antécédent qui a valeur de sujet relativement à la
  //    subordonnée relative. Celle-ci est donc toujours introduite par le mot
  //    de liaison "qui".
  //    On met l'antécédent dans l'array Sujet du complement, pour permettre à
  //    generePhrase de mettre le verbe à la bonne personne.
  //
  // 2) la relative a un sujet : "le garçon dont le ballon est rouge"
  //    La subordonnée relative est alors introduite par le mot
  //    de liaison "dont".
  //
  if (pCompls && (false == pCompls->empty()))
  {
    string sRelatives = string("") ;
    string sRelative1 = string("") ;

    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
    {
      if (phraseRelative == (*ComplIt)->iPhraseType)
      {
        if ((*ComplIt)->Sujet.empty())
        {
          // Si on ne supprime pas le complément, on tourne en rond
          NSPhraseMot* pMotSujet = new NSPhraseMot(*pMot) ;
          if (pMotSujet->getComplementPhr())
          {
            // Si il existe un adjectif numéral cardinal, penser à forcer le pluriel
            //
            if ((string("") == pMotSujet->getPluriel()) &&
                (FORCE_PLUR == pMotSujet->forceNombre(this)))
              pMotSujet->setPluriel(string("WPLUS1")) ;

            delete pMotSujet->getComplementPhr() ;
            pMotSujet->setComplementPhr((NSPhraseurArray*) 0) ;
          }
          (*ComplIt)->Sujet.push_back(pMotSujet) ;
          (*ComplIt)->iTypeSujet = NSPhraseur::sujetNoRepeat ; // Sujet matérialisé par "qui"
        }

        // If "dcPhrase" or "dcTiret" has not been defined for the relative, we
        // set it according to the principal
        //
        if (dcUndefined == (*ComplIt)->_iRelativeType)
          (*ComplIt)->_iRelativeType = iStyle ;

        NsProposition Propos(pContexte, ComplIt, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
        NSGenerateurFr GeneRelative(pContexte, &Propos, sLang) ;
        GeneRelative.genereProposition((*ComplIt)->_iRelativeType) ;

        if (string("") != sRelatives)
          sRelatives += string(", ") ;

        // sRelatives += GeneRelative.getPropositionPhrase() ;
        string sRelative2 = GeneRelative.getPropositionPhrase() ;
        etDuMilieu(&sRelatives, &sRelative1, &sRelative2) ;
      }
    }
    if (string("") != sRelative1)
    {
      etFinal(&sRelatives, &sRelative1) ;
      _sPhrase += string(" ") + sRelatives ;
    }
  }

  return true ;
}

// donneAdjectifComplet : pareil que pour donneNomComplet, sauf que c'est pour
// générer des adjectifs.

string
NSGenerateurFr::donneAdjectifComplet(NSPhraseMot* pMot, const GENRE iGenre, const GERECERTITUDE iGereCertitude, bool bInAffirmativeSequence)
{
  if ((NSPhraseMot*) NULL == pMot)
    return string("") ;

  // On retrouve l'élément dans le Lexique
  //
  NSPathologData* pDataAdj = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pDataAdj)
    return string("") ;

  if (false == pDataAdj->estAdjectif())
    return string("") ;

  string sLibelle = string("") ;

  donneLibelleAffiche(&sLibelle, pDataAdj, iGenre) ;
  agglutinate(pMot, sLibelle) ;

  // L'adjectif peut etre précédé d'adverbes.

  NSPhraseurArray* pCompls = pMot->getComplementPhr() ;
  if (pCompls && (false == pCompls->empty()))
  {
    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
    {
      NSPhraseur* pComp = *ComplIt ;
      NsProposition Propos(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
      NSGenerateurFr GeneCpl(pContexte, &Propos, sLang) ;
      if (GeneCpl.genereAdjectifComplet(pMot, iGenre, iGereCertitude))
        sLibelle = GeneCpl.getTempoPhrase() ;
    }
  }
  else
  {
    if (avecCertitude == iGereCertitude)
    {
      string sCertitude = string(pMot->getCertitude(), 0, 5) ;
      string sCertitudeEcrit = donneCertitude(sCertitude, (bool*) 0, bInAffirmativeSequence) ;

      if (string("") != sCertitudeEcrit)
        sLibelle = sCertitudeEcrit + string(" ") + sLibelle ;
    }
  }

  return sLibelle ;
}

bool
NSGenerateurFr::genereAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre, GERECERTITUDE iGereCertitude, bool bInAffirmativeSequence)
{
  _sPhrase = string("") ;

  reinitialise() ;

  NSPhraseurArray* pCompls = pMot->getComplementPhr() ;

  classeTout() ;

  //
  // Prise des informations sur l'adverbe à compléter
  //
  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return false ;
  if (false == pData->estAdjectif())
    return false ;

  string sAdjectif = string("") ;
  donneLibelleAffiche(&sAdjectif, pData, iGenre) ;
  if (string("") == sAdjectif)
    return false ;
  agglutinate(pMot, sAdjectif) ;

  // L'adjectif peut etre complété par des adverbes.

  iterPhraseMot  iterMotsComp ;
  NSPathologData DataComp ;

  string sInterCompPos1 = string("") ;
  string sInterCompPos2 = string("") ;
  string sCompPos = string("") ;

  string sInterCompNeg1 = string("") ;
  string sInterCompNeg2 = string("") ;
  string sCompNeg = string("") ;

  string sComp = string("") ;

  if (_pAdverbePos)
  {
    if (false == _pAdverbePos->empty())
    {
      for (iterMotsComp  = _pAdverbePos->begin() ;
                _pAdverbePos->end() != iterMotsComp ;
                                            iterMotsComp++)
      {
        if (false == (*iterMotsComp)->estTexteLibre())
        {
          NSPathologData* pAdvData = (*iterMotsComp)->getLexiqueData(sLang) ;
          if ((NSPathologData*) NULL == pAdvData)
            return false ;

          sInterCompPos2 = donneAdverbeComplet(*iterMotsComp) ;
        }
        else
          sInterCompPos2 = (*iterMotsComp)->getTexteLibre() ;

        etDuMilieu(&sCompPos, &sInterCompPos1, &sInterCompPos2) ;
      }
      etFinal(&sCompPos, &sInterCompPos1) ;
    }
  }

  if (_pAdverbeNeg)
  {
    if (false == _pAdverbeNeg->empty())
    {
      for (iterMotsComp  = _pAdverbeNeg->begin() ;
                   _pAdverbeNeg->end() != iterMotsComp ;
                                                iterMotsComp++)
      {
        NSPathologData* pAdvData = (*iterMotsComp)->getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pAdvData)
          return false ;

        sInterCompNeg2 = donneAdverbeComplet(*iterMotsComp) ;

        etDuMilieu(&sCompNeg, &sInterCompNeg1, &sInterCompNeg2) ;
      }
      etFinal(&sCompNeg, &sInterCompNeg1) ;
    }
  }

  sComp = sCompPos ;

  if (string("") != sCompNeg)
  {
    if (string("") != sCompPos)
      sComp += string(" mais ") + sCompNeg ;
    else
      sComp = sCompNeg ;
  }

  _sPhrase = sComp ;

  if (string("") != sComp)
    _sPhrase += string(" ") ;
  _sPhrase += sAdjectif ;

  // Tout a la fin, on met le complément entre parentheses.

  if (pCompls && (false == pCompls->empty()))
  {
    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
    {
      if ((*ComplIt)->pParentheses)
      {
        NsProposition Propos(pContexte, &((*ComplIt)->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
        NSGenerateurFr GeneParentheses(pContexte, &Propos, sLang) ;
        GeneParentheses.genereProposition(dcPhrase) ;
        _sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
      }
    }
  }

  // Le complément de l'adjectif peut être :
  //
  // - un nom                 : la plaque est responsable d'une sténose
  // - un pronom              : je suis content pour elle
  // - un verbe à l'infinitif : je suis incapable de mentir
  // - un adverbe             : ce travail est vraiment difficile
  // - une proposition        : il était si content qu'il le disait à tous
  //
  if (pCompls && (false == pCompls->empty()))
  {
    string sRelatives = string("") ;
    string sRelative1 = string("") ;

    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
    {
      // Proposition
      //
      if (phraseRelative == (*ComplIt)->iPhraseType)
      {
        if ((*ComplIt)->Sujet.empty())
        {
          // Si on ne supprime pas le complément, on tourne en rond
          NSPhraseMot* pMotSujet = new NSPhraseMot(*pMot) ;
          if (pMotSujet->getComplementPhr())
          {
            delete pMotSujet->getComplementPhr() ;
            pMotSujet->setComplementPhr((NSPhraseurArray*) 0) ;
          }
          (*ComplIt)->Sujet.push_back(pMotSujet) ;
          (*ComplIt)->iTypeSujet = NSPhraseur::sujetNoRepeat ; // Sujet matérialisé par "qui"
        }

        // If "dcPhrase" or "dcTiret" has not been defined for the relative, we
        // set it according to the principal
        //
        // if (dcUndefined == (*ComplIt)->_iRelativeType)
        //   (*ComplIt)->_iRelativeType = iStyle ;

        NsProposition Propos(pContexte, ComplIt, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
        NSGenerateurFr GeneRelative(pContexte, &Propos, sLang) ;
        GeneRelative.genereProposition((*ComplIt)->_iRelativeType) ;

        if (string("") != sRelatives)
          sRelatives += string(", ") ;

        string sRelative2 = GeneRelative.getPropositionPhrase() ;
        etDuMilieu(&sRelatives, &sRelative1, &sRelative2) ;
      }
      //
      // Nom "la plaque est responsable d'une sténose"
      // On qualifie avec la préposition "de"
      //
      else if (false == (*ComplIt)->Sujet.empty())
      {
        string sNouns  = string("") ;
        string sInter1 = string("") ;

        for (iterPhraseMot iterMots = (*ComplIt)->Sujet.begin() ; (*ComplIt)->Sujet.end() != iterMots ; iterMots++)
        {
          string sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), dcPhrase, sansCertitude) ;
          etDuMilieu(&sNouns, &sInter1, &sInter2) ;
        }
        etFinal(&sNouns, &sInter1) ;

        if (string("") != sNouns)
          _sPhrase += string(" de ") + sNouns ;
      }
    }
    if (string("") != sRelative1)
    {
      etFinal(&sRelatives, &sRelative1) ;
      _sPhrase += string(" ") + sRelatives ;
    }
  }

  if (iGereCertitude == avecCertitude)
  {
    string sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude, (bool*) 0, bInAffirmativeSequence) ;

    if (string("") != sCertitudeEcrit)
      _sPhrase = sCertitudeEcrit + string(" ") + _sPhrase ;
  }

  return true ;
}

// donneAdverbeComplet : pareil que pour les noms et les adjectifs, mais pour les adverbes.

string
NSGenerateurFr::donneAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude)
{
  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return string("") ;

  if (false == pData->estAdverbe())
    return string("") ;

  string sLibel = string("") ;
  donneLibelleAffiche(&sLibel, pData) ;

  NSPhraseurArray* pCompls = pMot->getComplementPhr() ;
  if (pCompls && (false == pCompls->empty()))
  {
    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
    {
      NsProposition Propos(pContexte, *ComplIt, NsProposition::notSetType, NsProposition::notSetConjonct) ;
      NSGenerateurFr GeneCpl(pContexte, &Propos, sLang) ;
      if (GeneCpl.genereAdverbeComplet(pMot, iGereCertitude))
        sLibel = GeneCpl.getTempoPhrase() ;
    }
  }

  if (iGereCertitude == avecCertitude)
  {
    string sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude) ;

    if (sCertitudeEcrit != "")
      sLibel = sCertitudeEcrit + string(" ") + sLibel ;
  }

  return sLibel ;
}

bool
NSGenerateurFr::genereAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE /* iGereCertitude */)
{
  if ((NSPhraseMot*) NULL == pMot)
    return false ;

  _sPhrase = string("") ;

  reinitialise() ;

  NSPhraseurArray* pCompls = pMot->getComplementPhr() ;

  classeTout() ;

  //
  // Prise des informations sur l'adverbe à compléter
  //
  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return false ;

  if (false == pData->estAdverbe())
    return false ;

  string sAdverbe = string("") ;

  donneLibelleAffiche(&sAdverbe, pData) ;
  if (string("") == sAdverbe)
    return false ;

  // L'adverbe peut etre complété par des adverbes.

  string sInterCompPos1 = string("") ;
  string sInterCompPos2 = string("") ;
  string sCompPos = string("") ;

  string sInterCompNeg1 = string("") ;
  string sInterCompNeg2 = string("") ;
  string sCompNeg = string("") ;

  string sComp = string("") ;

  if (_pAdverbePos)
  {
    if (false == _pAdverbePos->empty())
    {
      for (iterPhraseMot iterMotsComp  = _pAdverbePos->begin();
               iterMotsComp != _pAdverbePos->end();
               iterMotsComp++)
      {
        if (false == (*iterMotsComp)->estTexteLibre())
        {
          NSPathologData* pDataComp = (*iterMotsComp)->getLexiqueData(sLang) ;
          if ((NSPathologData*) NULL == pDataComp)
            return false ;

          sInterCompPos2 = donneAdverbeComplet(*iterMotsComp) ;
        }
        else
          sInterCompPos2 = (*iterMotsComp)->getTexteLibre() ;

        etDuMilieu(&sCompPos, &sInterCompPos1, &sInterCompPos2) ;
      }
      etFinal(&sCompPos, &sInterCompPos1) ;
    }
  }

  if (_pAdverbeNeg)
  {
    if (false == _pAdverbeNeg->empty())
    {
      for (iterPhraseMot iterMotsComp  = _pAdverbeNeg->begin();
               iterMotsComp != _pAdverbeNeg->end();
               iterMotsComp++)
      {
        NSPathologData* pDataComp = (*iterMotsComp)->getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pDataComp)
          return false ;

        sInterCompNeg2 = donneAdverbeComplet(*iterMotsComp) ;

        etDuMilieu(&sCompNeg, &sInterCompNeg1, &sInterCompNeg2) ;
      }
      etFinal(&sCompNeg, &sInterCompNeg1) ;
    }
  }

  sComp = sCompPos;

  if (string("") != sCompNeg)
  {
    if (string("") != sCompPos)
      sComp += string(" mais ") + sCompNeg ;
    else
      sComp = sCompNeg ;
  }

  _sPhrase = sComp ;

  if (string("") != sComp)
    _sPhrase += string(" ") ;
  _sPhrase += sAdverbe ;

  // Tout a la fin, on met le complement entre parentheses.
  //
  if (pCompls && (false == pCompls->empty()))
  {
    for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
    {
      if ((*ComplIt)->pParentheses)
      {
        NsProposition Propos(pContexte, &((*ComplIt)->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
        NSGenerateurFr GeneParentheses(pContexte, &Propos, sLang) ;
        GeneParentheses.genereProposition(dcPhrase) ;
        _sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
      }
    }
  }

  return true ;
}


// donneCertitude : méthode qui donne le libellé de la certitude, et éventuellement
// qui indique si le libellé de la certitude commence par une voyelle ou un h muet.
//
// bInAffirmativeSequence == true si la phrase est affirmative et que, au lieu de
// "pas", il faut écrire "non".
// Exemple : l'artère n'est pas dilatée, ni compliante.
//           l'artère est dilatée et non compliante.
//
string
NSGenerateurFr::donneCertitude(string sCertitude, bool* pCommenceParVoyelleOuH, bool bInAffirmativeSequence)
{
  // pCommenceParVoyelleOuH sert au cas ou la certitude peut imposer une
  // apostrophe. On ne le modifie pas au cas ou la ceritude

  if (string("") == sCertitude)
    return string("") ;

  if (string("WCE00") == sCertitude)
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false ;

    if (false == bInAffirmativeSequence)
      return string("pas") ;
    else
      return string("non") ;
  }

  if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false ;

    return string("probablement pas") ;
  }

  if (string(sCertitude, 0 ,5) == "WCE50")
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false ;
    return string("peut-être") ;
  }

  if ((string(sCertitude, 0 ,5) > "WCE50") && (string(sCertitude, 0 ,5) < "WCE90"))
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false ;
    return string("probablement");
  }

  if ((string(sCertitude, 0 ,5) >= "WCE90") && (string(sCertitude, 0 ,5) < "WCEA0"))
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false ;
    return string("typiquement");
  }

  return string("") ;
}

// donnePhraseComplement : generation des complements circonstanciels.

string
NSGenerateurFr::donnePhraseComplement(NSPhraseMot* pLiaison,
                                      NSPhraseMot* pPreposition,
                                      NSPhraseMotArray* pCC,
                                      bool* bSucces, DCODETYPE iStyle)
{
  if (bSucces)
    *bSucces = false ;

  string sPhr = string("") ;

  if ((NSPhraseMotArray*) NULL == pCC)
    return sPhr ;

  if (pCC->empty())
  {
    if (bSucces)
      *bSucces = true ;

    return sPhr ;
  }

  string sInter1 = string("") ;

  GENRE iGenre ;
  bool  bNegation = false ;

  for (iterPhraseMot iterMots = pCC->begin() ; pCC->end() != iterMots ; iterMots++)
  {
    string sInter2 = string("") ;

    // If there is no complement, try to get a simmple label
    //
    NSPhraseurArray* pCplmt = (*iterMots)->getComplementPhr() ;
    if (((NSPhraseurArray*) NULL == pCplmt) || (pCplmt->empty()))
      sInter2 = getLabelForNum(*iterMots) ;

    //
    // Ni valeur chiffrée, ni date
    // Not a number, nor a date
    //
    if (string("") == sInter2)
    {
      string sConcept = (*iterMots)->getConcept() ;
      string sPluriel = (*iterMots)->getPluriel() ;

      if (false == (*iterMots)->estTexteLibre())
      {
        NSPathologData Data ;
        bool bFound = getPathologData(sConcept, Data) ;
        if (false == bFound)
        {
          string sWarningTxt = string("NSGenerateurFr::donnePhraseComplement: cannot find \"") + sConcept + string("\" in Lexique.") ;
          pContexte->getSuperviseur()->trace(&sWarningTxt, 1, NSSuper::trWarning) ;
          return string("") ;
        }
        if (false == Data.estNom())
        {
          string sWarningTxt = string("NSGenerateurFr::donnePhraseComplement: \"") + sConcept + string("\" is not a noon in Lexique.") ;
          pContexte->getSuperviseur()->trace(&sWarningTxt, 1, NSSuper::trWarning) ;
          return string("") ;
        }

        // L'article du statut d'objet reel ou abstrait

        if ((*iterMots)->getArticle() != NSPhraseMot::articleSans)
        {
          if (string("") == sPluriel)
            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle, sansCertitude) ;
          else if (string(sPluriel, 0, 5) == "WPLUR")
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle, sansCertitude) ;
          else if (string(sPluriel, 0, 5) == "WPLUS")
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle, sansCertitude) ;
        }
        else
          sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle, sansCertitude) ;

        // In case of negation, the "pas" is added globally at the begining
        // of complements enumaration: "pas au niveau du bulbe, ni du cardia"
        //
        string sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;
        if (string("WCE00") == sCertitude)
          bNegation = true ;
        else
        {
          string sCertitudeEcrit = donneCertitude(sCertitude) ;
          if (string("") != sCertitudeEcrit)
            sInter2 = sCertitudeEcrit + string(" ") + sInter2 ;
        }
      }
      else
        sInter2 = (*iterMots)->getTexteLibre() ;
    }

    if (bNegation)
      etDuMilieu(&sPhr, &sInter1, &sInter2, string(", ni ")) ;
    else
      etDuMilieu(&sPhr, &sInter1, &sInter2) ;
  }

  if (bNegation)
    etFinal(&sPhr, &sInter1, string(", ni ")) ;
  else
    etFinal(&sPhr, &sInter1) ;

  // Traitement de la préposition globale
  string sLexique = pPreposition->getLexique() ;
  if (string("") != sLexique)
  {
    string sPluriel = pPreposition->getPluriel() ;

    NSPathologData* pData = pPreposition->getLexiqueData(sLang) ;
    if ((NSPathologData*) NULL == pData)
      return string("") ;

    pData->donneGenre(&iGenre) ;
    if (string("") != sPluriel)
      pData->donneGenrePluriel(&iGenre) ;
    string sPrep = string("") ;
    donneLibelleAffiche(&sPrep, pData, iGenre) ;

    sPhr = sPrep + string(" ") + sPhr ;
  }

  // Traitement de l'élément de liaison
  //
  sLexique = pLiaison->getLexique() ;
  if (string("") != sLexique)
  {
    string sPluriel = pLiaison->getPluriel() ;

    NSPathologData* pData = pLiaison->getLexiqueData(sLang) ;
    if ((NSPathologData*) NULL == pData)
      return string("") ;

    if (pData->estVerbe())
    {
      string sPartPresent = donneParticipePresent(pData) ;
      sPhr = sPartPresent + string(" ") + sPhr ;
    }
    else
    {
      pData->donneGenre(&iGenre) ;
      if (string("") != sPluriel)
        pData->donneGenrePluriel(&iGenre) ;
      string sLink = string("") ;
      donneLibelleAffiche(&sLink, pData, iGenre) ;
      sPhr = sLink + string(" ") + sPhr ;
    }
  }

  if (bSucces)
    *bSucces = true ;

  return sPhr ;
}

string
NSGenerateurFr::donnePhraseChiffree(NSPhraseMot* pLiaison,
                                    NSPhraseMot* pPreposition,
                                    NSPhraseMotArray* pCC,
                                    bool* bSucces)
{
  string sPhr = string("") ;

  if ((NSPhraseMotArray*) NULL == pCC)
  {
    if (bSucces)
      *bSucces = false ;
    return string("") ;
  }

  if (pCC->empty())
  {
    if (bSucces)
      *bSucces = true ;
    return sPhr ;
  }

  string sInter1 = string("") ;
  string sPhraseSimple = string("") ;

  string sRelat1 = string("") ;
  string sPhraseRelative = string("") ;

  string sVerbe        = string("") ;
  string sVerbePluriel = string("") ;

  string sPluriel = string("") ;
  GENRE  iGenre ;

  for (iterPhraseMot iterMots = pCC->begin() ; pCC->end() != iterMots ; iterMots++)
  {
    string sMesure = string("") ;
    GENRE  iGenreMesure ;
    bool   bPluriel ;

    string sPrepOption = string("") ;

    string sLexique = (*iterMots)->getLexique() ;

    // It is perfectly allowed to have Lexique being empty for numerical values
    //
    NSPathologData* pData = (NSPathologData*) 0 ;

    if (string("") != sLexique)
    {
      pData = (*iterMots)->getLexiqueData(sLang) ;
      if ((NSPathologData*) NULL == pData)
      {
        if (bSucces)
          *bSucces = false ;
        return string("") ;
      }

      // Récupération du genre (composition du genre récupéré avec
      // le genre déjà existant)
      pData->donneGenre(&iGenreMesure) ;
      if (string("") != sPluriel)
        pData->donneGenrePluriel(&iGenreMesure) ;

      if ((string("") != sPluriel) || (pData->estPluriel()))
        bPluriel = true ;
      else
        bPluriel = false ;
      //
      donneLibelleAffiche(&sMesure, pData, iGenreMesure) ;
    }

    // Traitement du complement, en fonction de son format
    //
    string sComplement = (*iterMots)->getComplement() ;

    string sUnite  = (*iterMots)->getUnite() ;
    string sFormat = (*iterMots)->getFormat() ;

    string sValeur = string("") ;
    //
    // Valeur numérique
    //
    if (sFormat[0] == '£')
    {
      if (nbMARK == sFormat[1])
      {
        gereNum Num(pContexte->getSuperviseur(), sLang) ;
        Num.instancier(&sComplement, &sUnite, &sFormat) ;
        bool bSucces ;
        sValeur = decodeNum(&Num, &bSucces) ;

        // On traite le cas ou la valeur chiffree est du type :
        // "dans 3 jours", "il y a 3 jours", "pendant 3 jours",
        // "il y a 2 ans pendant 3 jours"...

        if (sLexique == "KDATE1")
        {
          // Cas ou il s'agit d'un evenement passe ponctuel :
          // "il y a 3 jours".
          sValeur = string("il y a ") + sValeur ;
          sVerbe        = string("était") ;
          sVerbePluriel = string("étaient") ;
        }
        else if (sLexique == string("KDATD1"))
        {
          // Cas ou il s'agit d'un evenement futur ponctuel :
          // "dans 3 jours".
          sValeur = string("dans ") + sValeur ;
          sVerbe        = string("sera") ;
          sVerbePluriel = string("seront") ;
        }
        else if (string("") != sLexique)
        {
          if (pData->estNom())
            sPrepOption = string("de") ;
          sVerbe        = string("est") ;
          sVerbePluriel = string("sont") ;
        }
      }
      //
      // Date
      //
      else if ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK))
      {
        string sMessage = string("") ;
        string sIntro   = string("") ;

        gereDateFr Date(pContexte->getSuperviseur(), sLang) ;
        Date.setUnite(&sUnite) ;
        Date.setFormat(&sFormat) ;
        Date.setDate(&sComplement) ;
        Date.donne_date_claire(&sMessage, &sIntro) ;
        sValeur = sIntro + sMessage ;
      }
      //
      // Heure
      //
      else if (sFormat[1] == heureMARK)
      {
        string sMessage = string("") ;

        gereHeure Heure(pContexte->getSuperviseur(), sLang) ;
        Heure.setUnite(&sUnite) ;
        Heure.setFormat(&sFormat) ;
        Heure.setHeure(&sComplement) ;
        Heure.donne_heure(&sMessage) ;
        sValeur = sMessage ;
      }
    }

    sLexique = (*iterMots)->getLexique() ;

    string sMethode      = (*iterMots)->getMethode() ;
    string sLibelMethode = string("") ;

    if (string("") != sMethode)
    {
      NSPathologData Data ;
      bool trouve = getPathologData(sMethode, Data) ;
      if (false == trouve)
      {
        *bSucces = false ;
        return string("") ;
      }
      // Récupération du genre (composition du genre récupéré avec
      // le genre déjà existant)
      Data.donneGenre(&iGenre);
      if (string("") != sPluriel)
        Data.donneGenrePluriel(&iGenre) ;
      //
      donneLibelleAffiche(&sLibelMethode, &Data, iGenre) ;
    }

    string sInter2 = string("") ;
    string sRelat2 = string("") ;

    //
    // Cas simple : une lésion d'une longueur de 3 mm
    //
    if ((*(iterMots))->getNumForme() == NSPhraseMot::numSimple)
    {
      if (string("") != sMesure)
      {
        sInter2 = string("de ") +
                                donneArticleIndefini(*iterMots, bPluriel) +
                                sMesure ;
      }
      if (string("") != sPrepOption)
        sInter2 += string(" ") + sPrepOption ;
      if (string("") != sInter2)
        sInter2 += string(" ") ;
      sInter2 += sValeur ;
      if (string("") != sLibelMethode)
        sInter2 += string(" (") + sLibelMethode + string(")") ;
    }
    //
    // Cas relative : une lésion dont la longueur est de 3 mm
    //
    else if ((*(iterMots))->getNumForme() == NSPhraseMot::numRelative)
    {
      sRelat2 = donneArticleDefini(*iterMots, bPluriel) + sMesure;
      if (bPluriel)
        sRelat2 += string(" ") + sVerbePluriel ;
      else
        sRelat2 += string(" ") + sVerbe ;
      if (string("") != sPrepOption)
        sRelat2 += string(" ") + sPrepOption ;
      sRelat2 += string(" ") ;
      sRelat2 += sValeur ;
      if (string("") != sLibelMethode)
        sRelat2 += string(" (") + sLibelMethode + string(")") ;
    }
    //
    // Cas tirets : longueur 3 mm
    //
    else
    {
      sInter2 = sMesure ;
      if (string("") != sInter2)
        sInter2 += string(" ") ;
      sInter2 += sValeur ;
      if (string("") != sLibelMethode)
        sInter2 += string(" (") + sLibelMethode + string(")") ;
    }

    // On peut avoir un complémente de phrase, par exemple "à 25 mm du thrombus"
    //
    NSPhraseurArray* pCompls = (*iterMots)->getComplementPhr() ;
    if (pCompls && (false == pCompls->empty()))
    {
      for (phraseurIter ComplIt = pCompls->begin() ; pCompls->end() != ComplIt ; ComplIt++)
      {
        NsProposition Propos(pContexte, ComplIt, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
        NSGenerateurFr GeneParentheses(pContexte, &Propos, sLang) ;
        GeneParentheses.genereProposition(dcPhrase) ;
        if (string("") != sInter2)
          sInter2 += string(" ") + GeneParentheses.getPropositionPhrase() ;
        if (string("") != sRelat2)
          sRelat2 += string(" ") + GeneParentheses.getPropositionPhrase() ;
      }
    }

    etDuMilieu(&sPhraseSimple,   &sInter1, &sInter2) ;
    etDuMilieu(&sPhraseRelative, &sRelat1, &sRelat2) ;
  }
  etFinal(&sPhraseSimple,   &sInter1) ;
  etFinal(&sPhraseRelative, &sRelat1) ;

  sPhr = sPhraseSimple;
  if (string("") != sPhraseRelative)
  {
    if (string("") != sPhr)
      sPhr += string(" ") ;

    // Eviter "La sténose est peu serrée : dont le pic de vitesse est de..."
    //
    if ((NsProposition::deuxPoints != _pProposition->iPropositionType) &&
        ((false == _pPh->COD.empty()) || (levelCOD == _iGeneratedLevel)))
      sPhr += string("dont ") ;

    sPhr += sPhraseRelative ;
  }

  // Traitement de la préposition
  //
  string sLexProp = pPreposition->getLexique() ;
  if (string("") != sLexProp)
  {
    sPluriel = pPreposition->getPluriel() ;

    NSPathologData* pDataProp = pPreposition->getLexiqueData(sLang) ;
    if ((NSPathologData*) NULL == pDataProp)
      return string("") ;

    pDataProp->donneGenre(&iGenre) ;
    if (string("") != sPluriel)
      pDataProp->donneGenrePluriel(&iGenre) ;
    string sPrep = string("") ;
    donneLibelleAffiche(&sPrep, pDataProp, iGenre) ;
    sPhr = sPrep + string(" ") + sPhr ;
  }
  else if (string("") != pPreposition->getPreposition())
    sPhr = pPreposition->getPreposition() + string(" ") + sPhr ;

  // Traitement de l'élément de liaison
  //
  string sLexLink = pLiaison->getLexique() ;
  if (string("") != sLexLink)
  {
    sPluriel = pLiaison->getPluriel() ;

    NSPathologData* pDataLink = pLiaison->getLexiqueData(sLang) ;
    if ((NSPathologData*) NULL == pDataLink)
      return string("") ;

    if (pDataLink->estVerbe())
    {
      string sLink = donneParticipePresent(pDataLink) ;
      sPhr = sLink + string(" ") + sPhr ;
    }
    else
    {
      pDataLink->donneGenre(&iGenre) ;
      if (string("") != sPluriel)
        pDataLink->donneGenrePluriel(&iGenre) ;
      string sLink = string("") ;
      donneLibelleAffiche(&sLink, pDataLink, iGenre) ;
      sPhr = sLink + string(" ") + sPhr ;
    }
  }

  *bSucces = true ;
  return sPhr ;
}

string
NSGenerateurFr::getLabelForNum(NSPhraseMot* pWord)
{
  if ((NSPhraseMot*) NULL == pWord)
    return string("") ;

  // Est-ce une donnée chiffrée ?
  string sFormat = pWord->getFormat() ;

  // Est-ce une structure de type NSPhraseMotTime ?
  // Is it a NSPhraseMotTime object?
  //
  NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(pWord) ;
  if (pDate)
  {
    bool bSucces ;
    string sTimeLibel = decodeTime(pDate, &bSucces) ;
    if (bSucces)
      return sTimeLibel ;
  }

  NSPhraseMotTimeCycle* pCycle = dynamic_cast<NSPhraseMotTimeCycle*>(pWord) ;
  if (pCycle)
  {
    bool bSucces ;
    string sCycleLibel = decodeCycle(pCycle, &bSucces) ;
    if (bSucces)
      return sCycleLibel ;
  }

  //
  // Date "simple"
  // "Simple" date
  //
  if ('£' == sFormat[0])
  {
    string sComplement = pWord->getComplement() ;
    string sUnite      = pWord->getUnite() ;

    if ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK))
    {
      gereDateFr Date(pContexte->getSuperviseur(), sLang) ;
      Date.setDate(&sComplement) ;
      Date.setFormat(&sFormat) ;

      string sUniteSens = string("") ;
      pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
      Date.setUnite(&sUniteSens) ;

      string sLibel, sIntro ;

      Date.donne_date_claire(&sLibel, &sIntro) ;
      return sIntro + sLibel ;
    }

    gereNum Num(pContexte->getSuperviseur(), sLang) ;
    Num.instancier(&sComplement, &sUnite, &sFormat) ;
    bool bSucces ;
    return decodeNum(&Num, &bSucces) ;
  }

  return string("") ;
}

// donnePremierAdj : donne le premier des adjectifs qui completent un mot.
// ne le donne pas s'il s'agit d'un texte libre.

bool
NSGenerateurFr::donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj)
{
  NSPhraseMot PremierAdj(pContexte) ;

  if (pCompAdj)
  {
    NsProposition Propos(pContexte, &pCompAdj, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneCompl(pContexte, &Propos, sLang) ;
    pCompAdj->classeAdjectif(this) ;
    GeneCompl.classeTout() ;

    if (GeneCompl._pAdjEpitheteAvPos)
    {
      if (false == (GeneCompl._pAdjEpitheteAvPos)->empty())
      {
        PremierAdj = *(*((GeneCompl._pAdjEpitheteAvPos)->begin())) ;

        // Si jamais le premier adjectif est un texte libre, on l'ignore,
        // vu qu'on est incapable de le traiter grammaticalement.
        if (PremierAdj.estTexteLibre())
          return false ;

        NSPathologData* pPremierAdjData = PremierAdj.getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pPremierAdjData)
          return false ;

        if (pMotAdj)
          *pMotAdj = PremierAdj ;

        return true ;
      }
    }
    else if (GeneCompl._pAdjEpitheteAvNeg)
      if (false == (GeneCompl._pAdjEpitheteAvNeg)->empty())
      {
        PremierAdj = *(*((GeneCompl._pAdjEpitheteAvNeg)->begin())) ;

        if (PremierAdj.estTexteLibre())
          return false;

        NSPathologData* pPremierAdjData = PremierAdj.getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pPremierAdjData)
          return false ;

        if (pMotAdj)
          *pMotAdj = PremierAdj ;

        return true ;
      }
  }
  return false ;
}

// donnePremierAdj : donne le premier adverbe qui complète un mot.
// Ne renvoie rien s'il s'agit d'un texte libre.

bool
NSGenerateurFr::donnePremierAdv(NSPhraseur* pComp, NSPhraseMot* pMotAdv)
{
  NSPhraseMot PremierAdv(pContexte);
  NSPhraseMot PremierAdvRec(pContexte);

  if (pComp)
  {
    NsProposition Propos(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
    NSGenerateurFr GeneCompl(pContexte, &Propos, sLang) ;
    pComp->classeAdjectif(this) ;
    GeneCompl.classeTout() ;

    if (GeneCompl._pAdverbePos)
    {
      if (false == (GeneCompl._pAdverbePos)->empty())
      {
        PremierAdv = *(*((GeneCompl._pAdverbePos)->begin())) ;

        // Si jamais le premier adverbe est un texte libre, on l'ignore,
        // vu qu'on est incapable de le traiter grammaticalement.
        if (PremierAdv.estTexteLibre())
          return false ;

        NSPathologData* pPremierAdvData = PremierAdv.getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pPremierAdvData)
          return false ;

        NSPhraseurArray* pCompls = PremierAdv.getComplementPhr() ;
        if (pCompls && (false == pCompls->empty()))
        {
          NSPhraseur* pFirstCompl = *(pCompls->begin()) ;
          if (donnePremierAdv(pFirstCompl, &PremierAdvRec))
          {
            *pMotAdv = PremierAdvRec ;
            return true ;
          }
        }
        if (pMotAdv)
          *pMotAdv = PremierAdv ;
        return true ;
      }
    }
    else if (GeneCompl._pAdverbeNeg)
      if (false == (GeneCompl._pAdverbeNeg)->empty())
      {
        PremierAdv = *(*((GeneCompl._pAdverbeNeg)->begin())) ;

        if (PremierAdv.estTexteLibre())
          return false ;

        NSPathologData* pPremierAdvData = PremierAdv.getLexiqueData(sLang) ;
        if ((NSPathologData*) NULL == pPremierAdvData)
          return false ;

        NSPhraseurArray* pCompls = PremierAdv.getComplementPhr() ;
        if (pCompls && (false == pCompls->empty()))
        {
          NSPhraseur* pFirstCompl = *(pCompls->begin()) ;
          if (donnePremierAdv(pFirstCompl, &PremierAdvRec))
          {
            *pMotAdv = PremierAdvRec ;
            return true ;
          }
        }

        if (pMotAdv)
          *pMotAdv = PremierAdv ;
        return true;
      }
  }
  return false ;
}

// decodeNum : pour décoder les valeurs chiffrées.
//
string
NSGenerateurFr::decodeNum(gereNum* pNum, bool* bSucces)
{
  if (((gereNum*) NULL == pNum) || ((bool*) NULL == bSucces))
    return string("") ;

  string sLibelle = string("") ;
  *bSucces = true ;

  if ((pNum->estExact()) || (pNum->estInf()) || (pNum->estSup()))
  {
    if (pNum->estExact())
      sLibelle = pNum->getNum(pContexte, "") ;
    else
    {
      if ((pNum->estInf()) && (pNum->estSup()))
        sLibelle = "entre " + pNum->getNumInf() + " et " + pNum->getNumSup() ;
      else if (pNum->estInf())
        sLibelle = "plus de " + pNum->getNumInf() ;
      else if (pNum->estSup())
        sLibelle = "moins de " + pNum->getNumSup() ;
    }
    //
    // Libellé de l'unité de l'unité
    //
    if (string("") != sLibelle)
    {
      string sUniteLibel = pNum->donneLibelleUnite(pContexte) ;

      if (string("") != sUniteLibel)
        sLibelle += string(" ") + sUniteLibel ;
    }
  }

  return sLibelle ;
}

string
NSGenerateurFr::decodeTime(NSPhraseMotTime* pTime, bool* pbSucces)
{
  if ((NSPhraseMotTime*) NULL == pTime)
    return string("") ;

  string sInter2     = string("") ;

  string sComplement = string("") ;
  string sUnite      = string("") ;
  string sFormat     = string("") ;

  if (string("") != pTime->getValeurDuree())
  {
    sComplement = pTime->getValeurDuree() ;
    sUnite      = pTime->getUniteDuree() ;
    sFormat     = pTime->getFormatDuree() ;

    gereNum numDuree(pContexte->getSuperviseur(), sLang) ;
    numDuree.instancier(&sComplement, &sUnite, &sFormat) ;
    bool bIsOk ;
    string sDurationValue = decodeNum(&numDuree, &bIsOk) ;

    if (bIsOk)
      sInter2 += string(" pendant ") + sDurationValue ;
  }

  if (string("") != pTime->getValeurMin())
  {
    sComplement = pTime->getValeurMin() ;
    sUnite      = pTime->getUniteMin() ;
    sFormat     = pTime->getFormatMin() ;

    gereDateFr DateMin(pContexte->getSuperviseur(), sLang);
    DateMin.setDate(&sComplement);
    DateMin.setFormat(&sFormat);

    string sUniteSens = string("") ;
    pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens);
    DateMin.setUnite(&sUniteSens);

    string sLibelMin, sIntroMin;

    // Min = max : date ou heure pile
    if (pTime->minEgalMax())
    {
      DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateFr::DateNormal) ;
      sInter2 += sIntroMin + sLibelMin ;
    }
    else if (string("") != pTime->getValeurMax())
    {
      DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateFr::DateDeb) ;

      sComplement = pTime->getValeurMax() ;
      sUnite      = pTime->getUniteMax() ;
      sFormat     = pTime->getFormatMax() ;

      gereDateFr DateMax(pContexte->getSuperviseur(), sLang) ;
      DateMax.setDate(&sComplement) ;
      DateMax.setFormat(&sFormat) ;

      string sUniteSens = string("") ;
      pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
      DateMax.setUnite(&sUniteSens) ;

      string sLibelMax, sIntroMax ;

      DateMax.donne_date_claire(&sLibelMax, &sIntroMax, gereDateFr::DateFin) ;

      sInter2 += sIntroMin + sLibelMin + string(" ") + sIntroMax + sLibelMax ;
    }
    else if (pTime->isMaxNow())
    {
      DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateFr::DateDeb) ;
      sInter2 += " depuis" + sIntroMin + sLibelMin ;
    }
    else
    {
      DateMin.donne_date_claire(&sLibelMin, &sIntroMin) ;
      sInter2 += string("à partir de ") + sIntroMin + sLibelMin ;
    }
  }
  else if (string("") != pTime->getValeurMax())
  {
    sComplement = pTime->getValeurMax() ;
    sUnite      = pTime->getUniteMax() ;
    sFormat     = pTime->getFormatMax() ;

    gereDateFr DateMax(pContexte->getSuperviseur(), sLang) ;
    DateMax.setDate(&sComplement) ;
    DateMax.setFormat(&sFormat) ;

    string sUniteSens = string("") ;
    pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
    DateMax.setUnite(&sUniteSens) ;

    string sLibelMax, sIntroMax ;

    if (pTime->isMinNow())
    {
      DateMax.donne_date_claire(&sLibelMax, &sIntroMax, gereDateFr::DateFin) ;
      sInter2 += " jusqu'" + sIntroMax + sLibelMax ;
    }
    else
    {
      DateMax.donne_date_claire(&sLibelMax, &sIntroMax) ;
      sInter2 += sIntroMax + sLibelMax ;
    }
  }
  
  if (pbSucces)
    *pbSucces = true ;

  return sInter2 ;
}

string
NSGenerateurFr::decodeCycle(NSPhraseMotTimeCycle* pCycle, bool* bSucces)
{
  string sInter2 = string("") ;

  string sAComplement = pCycle->getActionDurationValue() ;
  string sAUnite      = pCycle->getActionDurationUnit() ;
  string sAFormat     = pCycle->getActionDurationFormat() ;

  gereNum numAction(pContexte->getSuperviseur(), sLang) ;
  numAction.instancier(&sAComplement, &sAUnite, &sAFormat) ;

  string sCComplement = pCycle->getCycleDurationValue() ;
  string sCUnite      = pCycle->getCycleDurationUnit() ;
  string sCFormat     = pCycle->getCycleDurationFormat() ;

  gereNum numDuree(pContexte->getSuperviseur(), sLang) ;
  numDuree.instancier(&sCComplement, &sCUnite, &sCFormat) ;

  //
  // Cycle régulier - Regular cycle
  //
  if (pCycle->getCycleType() == NSPhraseMotTimeCycle::CycRegular)
  {
    // Même unité de temps - Same time unit
    //
    if (pCycle->getCycleDurationUnit() == pCycle->getActionDurationUnit())
    {
      if (string("") != pCycle->getActionDurationValue())
      {
        bool bGood ;
        string sActionDurationValue = decodeNum(&numAction, &bGood) ;

        if (bGood)
        {
          sInter2 += sActionDurationValue ;

          if (string("") != pCycle->getCycleDurationValue())
          {
            string sCycleLength = numDuree.getNum() ;
            sInter2 += string(" sur ") + sCycleLength ;
          }
          *bSucces = true ;
          return sInter2 ;
        }
      }
    }
    //
    // Unités de temps différentes - Distinct time unit
    //
    else
    {
      if (string("") != pCycle->getActionDurationValue())
      {
        bool bGood ;
        string sActionDurationValue = decodeNum(&numAction, &bGood) ;

        if (bGood)
        {
          sInter2 += sActionDurationValue ;

          if (string("") != pCycle->getCycleDurationValue())
          {
            if ((numDuree.estExact()) && (numDuree.getValeur() == 1))
            {
                string sCycUnitLib = numDuree.donneLibelleUnite(pContexte) ;
                sInter2 += string(" par ") + sCycUnitLib ;
            }
            else
            {
              string sCycleLength = decodeNum(&numDuree, &bGood) ;

              NSPathologData Data ;
              bool trouve = getPathologData(sCUnite, Data) ;
              if (trouve)
              {
                GENRE iGenre ;
                Data.donneGenre(&iGenre);
                if (isFeminin(iGenre))
                  sInter2 += string(" toutes les ") ;
                else
                  sInter2 += string(" tous les ") ;
              }
              else
                sInter2 += string(" tous les ") ;

              sInter2 += sCycleLength ;
            }
          }
          *bSucces = true ;
          return sInter2 ;
        }
      }
    }
  }
  //
  // Cycle libre - Free cycle
  //
  if (pCycle->getCycleType() == NSPhraseMotTimeCycle::CycFree)
  {
    // Même unité de temps - Same time unit
    //
    if ((pCycle->getCycleDurationUnit() == pCycle->getActionDurationUnit()) &&
        (string("") != pCycle->getNumberOfAction()))
    {
      if (string("") != pCycle->getActionDurationValue())
      {
        bool bGood ;
        string sActionDurationValue = decodeNum(&numAction, &bGood) ;

        if (bGood)
        {
          sInter2 += sActionDurationValue ;

          if (string("") != pCycle->getCycleDurationValue())
          {
            string sCycleLength = numDuree.getNum() ;
            sInter2 += string(" sur ") + sCycleLength ;
          }
          *bSucces = true ;
          return sInter2 ;
        }
      }
    }
    //
    // Unités de temps différentes - Distinct time unit
    //
    else
    {
      if (string("") != pCycle->getNumberOfAction())
        sInter2 += pCycle->getNumberOfAction() + string(" fois ") ;

      // Durée d'action non nulle
      //
      if (string("") != pCycle->getActionDurationValue())
      {
        bool bGood ;
        string sActionDurationValue = decodeNum(&numAction, &bGood) ;

        if (bGood)
        {
          sInter2 += sActionDurationValue ;

          if (string("") != pCycle->getCycleDurationValue())
          {
            if ((numDuree.estExact()) && (numDuree.getValeur() == 1))
            {
              string sCycUnitLib = numDuree.donneLibelleUnite(pContexte) ;
              sInter2 += string(" par ") + sCycUnitLib ;
            }
            else
            {
              string sCycleLength = decodeNum(&numDuree, &bGood) ;

              NSPathologData Data ;
              bool trouve = getPathologData(sCUnite, Data);
              if (trouve)
              {
                GENRE iGenre ;
                Data.donneGenre(&iGenre) ;
                if (isFeminin(genreFP))
                  sInter2 += string(" toutes les ") ;
                else
                  sInter2 += string(" tous les ") ;
              }
              else
                sInter2 += string(" tous les ") ;

              sInter2 += sCycleLength ;
            }
          }
          *bSucces = true ;
          return sInter2 ;
        }
      }
      // Durée d'action nulle
      else if (string("") != pCycle->getCycleDurationValue())
      {
        if ((numDuree.estExact()) && (numDuree.getValeur() == 1))
        {
            string sCycUnitLib = numDuree.donneLibelleUnite(pContexte) ;
            sInter2 += string(" par ") + sCycUnitLib ;
        }
        else
        {
          bool bGood;
          string sCycleLength = decodeNum(&numDuree, &bGood) ;
          /*
          NSPathologData* pData = new NSPathologData;
          NSSuper*        pSuper = pContexte->getSuperviseur();
          bool trouve = pContexte->getDico()->trouvePathologData(sLang, &sCUnite, pData);
          if (trouve)
          {
              int iGenre;
              pData->donneGenre(&iGenre);
              if ((iGenre == genreFS) || (iGenre == genreFP))
                  sInter2 += string(" toutes les ") ;
              else
                  sInter2 += string(" tous les ") ;
          }
          else
              sInter2 += string(" tous les ") ;
          */
          sInter2 += string(" par intervalle de ") ;
          sInter2 += sCycleLength ;
        }
        *bSucces = true ;
        return sInter2 ;
      }
    }
  }

  *bSucces = false ;
  return sInter2 ;
}

string
NSGenerateurFr::donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE /* iGereCertitude */)
{
  if ((NSPhraseMot*) NULL == pMot)
    return string("") ;

  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return string("") ;

  if (false == pData->estNom())
    return string("") ;

  if (bPluriel)
    return string("des ") ;

  GENRE  iGenreNom ;
  string sLibelle = string("") ;
  pData->donneGenre(&iGenreNom) ;
  donneLibelleAffiche(&sLibelle, pData, iGenreNom) ;

  if (pData->estMasculin())
    return string("un ") ;
  if (pData->estFeminin())
    return string("une ");

  return string("") ;
}

string
NSGenerateurFr::donneArticleDefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude)
{
  if ((NSPhraseMot*) NULL == pMot)
    return string("") ;

  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return string("") ;

  if (false == pData->estNom())
    return string("") ;

  if (bPluriel)
    return string("les ") ;

  GENRE iGenreNom ;
  string sLibelle = string("") ;
  pData->donneGenre(&iGenreNom) ;

  donneLibelleAffiche(&sLibelle, pData, iGenreNom) ;

  bool bApostrophe = false ;

  // On commence par regarder si le nom commence par une voyelle ou par un h muet.
  //
  if (CommenceParVoyelle(sLibelle))
    bApostrophe = true ;

  if (('h' == sLibelle[0]) || ('H' == sLibelle[0]))
  {
    if (pData->chercheGrammaire(VALEUR_DE_H) == H_MUET)
      bApostrophe = true ;
  }

  NSPathologData DataAdj ;
  NSPathologData DataAdv ;

  string sCertitudeNom = string("") ;
  string sCertitudeAdv = string("") ;

  sCertitudeNom = string(pMot->getCertitude(), 0, 5) ;

  NSPhraseurArray* pCompls = pMot->getComplementPhr() ;
  if (pCompls && (false == pCompls->empty()))
  {
    NSPhraseur* pFirsCompl = *(pCompls->begin()) ;

    NSPhraseMot MotAdj(pContexte) ;

    bool bPremierAdjExiste = donnePremierAdj(pFirsCompl, &MotAdj) ;
    string sLexiqueAdj = MotAdj.getLexique() ;

    if (bPremierAdjExiste)
    {
      string sCertitudeAdj = string(MotAdj.getCertitude(), 0, 5) ;

      if (string("") != sCertitudeAdj)
      {
        // Le premier adjectif est precede d'une certitude, donc on ne
        // s'occupe pas de la premiere lettre de l'adjectif.

        donneCertitude(sCertitudeAdj, &bApostrophe) ;
      }
      else if (MotAdj.getFirstComplementPhr())
      {
        NSPhraseMot MotAdv(pContexte) ;

        bool bPremierAdvExiste = donnePremierAdv(MotAdj.getFirstComplementPhr(), &MotAdv) ;
        string sLexiqueAdv = MotAdv.getLexique() ;

        if (bPremierAdvExiste)
        {
          string sCertitudeAdv = string(MotAdv.getCertitude(), 0, 5) ;

          if (string("") != sCertitudeAdv)
          {
            // Le premier adverbe est precede d'une certitude, donc on ne
            // s'occupe pas de la premiere lettre de l'adverbe.

            donneCertitude(sCertitudeAdv, &bApostrophe) ;
          }
          else
          {
            // On regarde si l'adverbe commence par une voyelle ou par un
            // h muet.
            string sLibelleAdv = string("") ;
            donneLibelleAffiche(&sLibelleAdv, &DataAdv) ;

            if (CommenceParVoyelle(sLibelleAdv))
              bApostrophe = true ;

            else if (('h' == sLibelleAdv[0]) || ('H' == sLibelleAdv[0]))
            {
              if (DataAdv.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                bApostrophe = true ;
            }
          }
        }
        else
        {
          // On regarde si l'adjectif commence par une voyelle ou par un
          // h muet.
          string sLibelleAdj = string("") ;
          donneLibelleAffiche(&sLibelleAdj, &DataAdj, iGenreNom) ;

          if (CommenceParVoyelle(sLibelleAdj))
            bApostrophe = true ;

          else if (('h' == sLibelleAdj[0]) || ('H' == sLibelleAdj[0]))
          {
            if (DataAdj.chercheGrammaire(VALEUR_DE_H) == H_MUET)
              bApostrophe = true ;
          }
        }
      }
    }
  }

  // Si on gere la certitude du nom, celle-ci prend le dessus sur le reste.

  if (avecCertitude == iGereCertitude)
    donneCertitude(sCertitudeNom, &bApostrophe) ;

  if (bApostrophe)
    return string("l'") ;

  if (pData->estMasculin())
    return string("le ") ;
  if (pData->estFeminin())
    return string("la ") ;

  return string("") ;
}

string
NSGenerateurFr::donneArticlePartitif(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude)
{
  bool bApostrophe = false ;

  NSPhraseMot    MotAdj(pContexte) ;
  NSPhraseMot    MotAdv(pContexte) ;
  NSPathologData DataAdj ;
  NSPathologData DataAdv ;

  string sLexiqueAdj = string("") ;
  string sLexiqueAdv = string("") ;

  string sLexique = pMot->getLexique() ;

  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return string("") ;

  string sCertitudeNom = string("") ;
  string sCertitudeAdj = string("") ;
  string sCertitudeAdv = string("") ;

  sCertitudeNom = string(pMot->getCertitude(), 0, 5) ;

  if (false == pData->estNom())
    return string("") ;

  if (bPluriel)
    return string("des ") ;

  GENRE  iGenreNom ;
  string sLibelle = string("") ;

  pData->donneGenre(&iGenreNom) ;
  donneLibelleAffiche(&sLibelle, pData, iGenreNom) ;

  // On commence par regarder si le nom commence par une voyelle ou par un h muet.

  if (CommenceParVoyelle(sLibelle))
    bApostrophe = true ;

  if (('h' == sLibelle[0]) || ('H' == sLibelle[0]))
  {
    if (pData->chercheGrammaire(VALEUR_DE_H) == H_MUET)
      bApostrophe = true;
  }

  if (pMot->getComplementPhr())
  {
    bool bPremierAdjExiste = donnePremierAdj(pMot->getFirstComplementPhr(), &MotAdj) ;
    sLexiqueAdj = MotAdj.getLexique() ;

    if (bPremierAdjExiste)
    {
      sCertitudeAdj = string(MotAdj.getCertitude(), 0, 5) ;

      if (string("") != sCertitudeAdj)
      {
        // Le premier adjectif est precede d'une certitude, donc on ne
        // s'occupe pas de la premiere lettre de l'adjectif.

        donneCertitude(sCertitudeAdj, &bApostrophe) ;
      }
      else if (MotAdj.getFirstComplementPhr())
      {
        bool bPremierAdvExiste = donnePremierAdv(MotAdj.getFirstComplementPhr(), &MotAdv) ;
        sLexiqueAdv = MotAdv.getLexique() ;

        if (bPremierAdvExiste)
        {
          sCertitudeAdv = string(MotAdv.getCertitude(), 0, 5) ;

          // Le premier adverbe est precede d'une certitude, donc on ne
          // s'occupe pas de la premiere lettre de l'adverbe.
          //
          if (string("") != sCertitudeAdv)
            donneCertitude(sCertitudeAdv, &bApostrophe) ;
          else
          {
            // On regarde si l'adverbe commence par une voyelle ou par un
            // h muet.
            string sLibelleAdv = "";
            donneLibelleAffiche(&sLibelleAdv, &DataAdv) ;

            if (CommenceParVoyelle(sLibelleAdv))
              bApostrophe = true ;

            else if (('h' == sLibelleAdv[0]) || ('H' == sLibelleAdv[0]))
            {
              if (DataAdv.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                bApostrophe = true ;
            }
          }
        }
        else
        {
          // On regarde si l'adjectif commence par une voyelle ou par un
          // h muet.
          string sLibelleAdj = string("") ;
          donneLibelleAffiche(&sLibelleAdj, &DataAdj, iGenreNom) ;

          if (CommenceParVoyelle(sLibelleAdj))
            bApostrophe = true ;

          else if (('h' == sLibelleAdj[0]) || ('H' == sLibelleAdj[0]))
          {
            if (DataAdj.chercheGrammaire(VALEUR_DE_H) == H_MUET)
              bApostrophe = true ;
          }
        }
      }
    }
  }

  if ((string("") != sCertitudeNom) && (sCertitudeNom < string("WCE050")))
  {
    if (bApostrophe)
      return string("d'") ;
    else
      return string("de ") ;
  }

  // Si on gere la certitude du nom, celle-ci prend le dessus sur le reste.

  if (avecCertitude == iGereCertitude)
    donneCertitude(sCertitudeNom, &bApostrophe) ;

  if (bApostrophe)
    return string("de l'") ;

  if (pData->estMasculin())
    return string("du ") ;
  if (pData->estFeminin())
    return string("de la ") ;

  return string("") ;
}

string
NSGenerateurFr::getAdjectifPossessif(GENRE iGenreNom, GENRE iGenrePossesseur)
{
  switch(iGenrePossesseur)
  {
    case genreMS :
    case genreFS :
    case genreNS :

      switch(iGenreNom)
      {
        case genreMS : return string("son") ;
        case genreFS : return string("sa") ;
        case genreMP : return string("ses") ;
        case genreFP : return string("ses") ;
      }
      break ;

    case genreMP :
    case genreFP :
    case genreNP :

      switch(iGenreNom)
      {
        case genreMS :
        case genreFS : return string("leur") ;
        case genreMP :
        case genreFP : return string("leurs") ;
      }
      break ;
  }

  return string("") ;
}

string
NSGenerateurFr::getAdjectifDemonstratif(GENRE iGenreNom)
{
  switch(iGenreNom)
  {
    case genreMS : return string("ce") ;
    case genreFS : return string("cette") ;
    case genreMP : return string("ces") ;
    case genreFP : return string("ces") ;
  }
  return string("") ;
}

string
NSGenerateurFr::donneAdjectifPossessif(NSPhraseMot* pMot, bool bPluriel)
{
  if ((NSPhraseMot*) NULL == pMot)
    return string("") ;

  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return string("") ;

  if (false == pData->estNom())
    return string("") ;

  if (bPluriel || pData->estPluriel())
    return string("ses ") ;

  if (pData->estMasculin())
    return string("son ") ;
  if (pData->estFeminin())
    return string("sa ") ;

  return string("") ;
}

string
NSGenerateurFr::donnePronomPersonnel(const GENRE iGenre, const NSPhraseur::VBPERSO iVbPersonne, const string sFonction)
{
  //
  // Pronom personnel en tant que sujet de la phrase
  //
  if (STR_SUJET == sFonction)
  {
    switch (iVbPersonne)
    {
      case NSPhraseur::pers1S : return string("je") ;
      case NSPhraseur::pers2S : return string("tu") ;
      case NSPhraseur::pers3S :
        switch (iGenre)
        {
          case genreMS : return string("il") ;
          case genreFS : return string("elle") ;
          case genreNS : return string("on") ;
          default      : return string("") ;
        }
      case NSPhraseur::pers1P : return string("nous") ;
      case NSPhraseur::pers2P : return string("vous") ;
      case NSPhraseur::pers3P :
        switch (iGenre)
        {
          case genreMP : return string("ils") ;
          case genreFP : return string("elles") ;
          default      : return string("") ;
        }
    }
    return string("") ;
  }
  //
  // Pronom personnel en tant que COD de la phrase
  //
  if (STR_COD == sFonction)
  {
    switch (iVbPersonne)
    {
      case NSPhraseur::pers1S : return string("me") ;
      case NSPhraseur::pers2S : return string("te") ;
      case NSPhraseur::pers3S :
        switch (iGenre)
        {
          case genreMS : return string("le") ;
          case genreFS : return string("la") ;
          case genreNS : return string("en") ; // en , y
          default      : return string("") ;
        }
      case NSPhraseur::pers1P : return string("nous") ;
      case NSPhraseur::pers2P : return string("vous") ;
      case NSPhraseur::pers3P : return string("les") ;
    }
    return string("") ;
  }
  //
  // Pronom personnel en tant que COI de la phrase
  //
  if (STR_COI == sFonction)
  {
    switch (iVbPersonne)
    {
      case NSPhraseur::pers1S : return string("me") ;
      case NSPhraseur::pers2S : return string("te") ;
      case NSPhraseur::pers3S :
        switch (iGenre)
        {
          case genreMS : return string("lui") ;
          case genreFS : return string("lui") ;
          case genreNS : return string("en") ; // en , y
          default      : return string("") ;
        }
      case NSPhraseur::pers1P : return string("nous") ;
      case NSPhraseur::pers2P : return string("vous") ;
      case NSPhraseur::pers3P : return string("leur") ;
    }
    return string("") ;
  }
  //
  // Pronom personnel en tant que CC de lieu de la phrase
  //
  if (STR_CCLIEU == sFonction)
  {
    switch (iVbPersonne)
    {
      case NSPhraseur::pers3S : return string("en") ; // en , y
      case NSPhraseur::pers3P : return string("en") ; // en , y
    }
    return string("") ;
  }
  return string("") ;
}

void
NSGenerateurFr::etFinal(string *type, string *type1, const string sSeparator)
{
	if (string("") == *type1)
		return ;

	if (string("") != *type)
    *type += sSeparator ;

	*type += *type1 ;
}

string
NSGenerateurFr::donneParticipePresent(NSPathologData* pPathoData)
{
  string sLibelle = string("") ;
  pPathoData->donneLibelleAffiche(&sLibelle) ;

  // On recherche le type de conjugaison du verbe (son numero de Bescherell)
  // On ne peut pas le faire avec donneverbeclasse, vu qu'on n'a pas de pPhraseMot,
  // On le fait donc "a la main" avec chercheGrammaire.

  string sConjugaison = pPathoData->chercheGrammaire("B") ;

  if (string("") == sConjugaison)
    return string("") ;

  // Verbes du 1er groupe
  // Il y a les B08 et les B14 (verbes en GER -> GEANT) et les autres.

  if ((sConjugaison > "B05") && (sConjugaison < "B19"))
  {
    if ((string("B08") == sConjugaison) || (string("B14") == sConjugaison))
      return string(sLibelle, 0, strlen(sLibelle.c_str())-2) + string("eant") ;
    else
      return string(sLibelle, 0, strlen(sLibelle.c_str())-2) + string("ant") ;
  }

  // Verbes du 2e groupe ir - issant comme finir
  if (string("B19") == sConjugaison)
    return string(sLibelle, 0, strlen(sLibelle.c_str())-1) + string("ssant") ;

  // Verbes du 2e groupe haïr
  if (string("B20") == sConjugaison)
    return string(sLibelle, 0, strlen(sLibelle.c_str())-2) + string("ïssant") ;

  // Verbes en -uire comme cuire
  if (string("B82") == sConjugaison)
    return string(sLibelle, 0, strlen(sLibelle.c_str())-2) + string("sant") ;

  return sLibelle ; // ou "" ?
}

string
NSGenerateurFr::donneParticipePasse(NSPhraseMot* pPhraseMot, const GENRE iGenre)
{
  if ((NSPhraseMot*) NULL == pPhraseMot)
    return string("") ;

  string sVb    = pPhraseMot->getLexique() ;
  string sVerbe = string("") ;

  string sTransitivite, sConjugaison, sActEtat ;
  if (donneVerbeClasse(pPhraseMot, &sTransitivite, &sConjugaison, &sActEtat) == false)
    return string("") ;

  string sLibelle ;
  if (pContexte->getDico()->donneLibelle(sLang, &sVb, &sLibelle) == false)
    return string("") ;

  // Verbes du 1er groupe
  if ((sConjugaison > "B05") && (sConjugaison < "B19"))
  {
    sVerbe = string(sLibelle, 0, strlen(sLibelle.c_str())-2) + string("é") ;
    if (isFeminin(iGenre))
      sVerbe += string("e") ;
    if (isPlural(iGenre))
      sVerbe += string("s") ;
  }

  // Verbes du 2e groupe
  if ((sConjugaison > "B18") && (sConjugaison < "B21"))
  {
    sVerbe = string(sLibelle, 0, strlen(sLibelle.c_str())-1) ;
    if (isFeminin(iGenre))
      sVerbe += string("e") ;
    if (isPlural(iGenre))
      sVerbe += string("s") ;
  }

  return sVerbe ;
}

// mapTempsVerbe : passage du système "général" de classification des temps au
// système francais.
//
void
NSGenerateurFr::mapTempsVerbe()
{
  //
  // Passage du systeme "international" au systeme francais :
  // Temps : tempsPresent, tempsPasse, tempsFutur
  // Aspect : aspectMoment, aspectAccompli, aspectProspectif
  //

  switch (_pPh->iVbAspect)
  {
    case NSPhraseur::aspectPonctuel :
    {
      switch (_pPh->iVbTemps)
      {
        case NSPhraseur::tempsPasseRevolu :
        {
          iTempsVerbe  = tempsPasseFr ;
          iAspectVerbe = aspectAccompli ;
          break ;
        }
        case NSPhraseur::tempsPasseActuel :
        {
          iTempsVerbe  = tempsPasseFr ;
          iAspectVerbe = aspectMoment ;
          break ;
        }
        case NSPhraseur::tempsPresent :
        {
          iTempsVerbe = tempsPresentFr ;
          iAspectVerbe = aspectMoment ;
          break ;
        }
        case NSPhraseur::tempsFuturProche :
        case NSPhraseur::tempsFuturLointain :
        {
          iTempsVerbe = tempsFuturFr ;
          iAspectVerbe = aspectMoment ;
          break ;
        }
        default : return ;
      }
      break ;
    }
    case NSPhraseur::aspectIntervalle :
    {
      switch (_pPh->iVbTemps)
      {
        case NSPhraseur::tempsPasseRevolu :
        case NSPhraseur::tempsPasseActuel :
        {
          iTempsVerbe  = tempsPasseFr ;
          iAspectVerbe = aspectAccompli ;
          break ;
        }
        case NSPhraseur::tempsPresent :
        {
          iTempsVerbe = tempsPresentFr ;
          iAspectVerbe = aspectMoment ;
          break ;
        }
        case NSPhraseur::tempsFuturProche :
        case NSPhraseur::tempsFuturLointain :
        {
          iTempsVerbe = tempsFuturFr ;
          iAspectVerbe = aspectAccompli ;
          break ;
        }
        default : return ;
      }
      break ;
    }
    default : return ;
  }
}

void
NSGenerateurFr::donneVerbe(string* principal, string* auxilliaire)
{
  if (((string*) NULL == principal) || ((string*) NULL == auxilliaire))
    return ;

  *principal   = string("") ;
  *auxilliaire = string("") ;

  if (_pPh->Verbe.empty())
    return ;

  string sVb = (*(_pPh->Verbe.begin()))->getLexique() ;
  string sVerbe ;
  pContexte->getDico()->donneCodeSens(&sVb, &sVerbe) ;

  //
  // Verbe être
  //
  if (sVb == "4ETRE1")
  {
    switch (iTempsVerbe)
    {
      case tempsPasseFr   :
      {
        switch (iAspectVerbe)
        {
          case aspectAccompli :
          {
            switch (_pPh->iVbPersonne)
            {
              case NSPhraseur::pers1S : *auxilliaire = "ai"    ; *principal = "été" ; return ;
              case NSPhraseur::pers2S : *auxilliaire = "as"    ; *principal = "été" ; return ;
              case NSPhraseur::pers3S : *auxilliaire = "a"     ; *principal = "été" ; return ;
              case NSPhraseur::pers1P : *auxilliaire = "avons" ; *principal = "été" ; return ;
              case NSPhraseur::pers2P : *auxilliaire = "avez"  ; *principal = "été" ; return ;
              case NSPhraseur::pers3P : *auxilliaire = "ont"   ; *principal = "été" ; return ;
            }
          }
          case aspectMoment :
          {
            switch (_pPh->iVbPersonne)
            {
              case NSPhraseur::pers1S : *principal = "étais" ;   return ;
              case NSPhraseur::pers2S : *principal = "étais" ;   return ;
              case NSPhraseur::pers3S : *principal = "était" ;   return ;
              case NSPhraseur::pers1P : *principal = "étions" ;  return ;
              case NSPhraseur::pers2P : *principal = "étiez" ;   return ;
              case NSPhraseur::pers3P : *principal = "étaient" ; return ;
            }
          }
        }
        break;
      }
      case tempsPresentFr :
      {
        switch (_pPh->iVbPersonne)
        {
          case NSPhraseur::pers1S : *principal = "suis" ;   return ;
          case NSPhraseur::pers2S : *principal = "es" ;     return ;
          case NSPhraseur::pers3S : *principal = "est" ;    return ;
          case NSPhraseur::pers1P : *principal = "sommes" ; return ;
          case NSPhraseur::pers2P : *principal = "êtes" ;   return ;
          case NSPhraseur::pers3P : *principal = "sont" ;   return ;
        }
        break ;
      }
      case tempsFuturFr   :
      {
        switch (_pPh->iVbPersonne)
        {
          case NSPhraseur::pers1S : *principal = "serai" ;  return ;
          case NSPhraseur::pers2S : *principal = "seras" ;  return ;
          case NSPhraseur::pers3S : *principal = "sera" ;   return ;
          case NSPhraseur::pers1P : *principal = "serons" ; return ;
          case NSPhraseur::pers2P : *principal = "serez" ;  return ;
          case NSPhraseur::pers3P : *principal = "seront" ; return ;
        }
        break ;
      }
    }
  }
  //
  // Verbe avoir
  //
  else if (sVb == "4AVOI1")
  {
      switch (iTempsVerbe)
      {
          case tempsPasseFr   :
          {
              switch (iAspectVerbe)
              {
                  case aspectAccompli :
                  {
                      switch (_pPh->iVbPersonne)
                      {
                          case NSPhraseur::pers1S : *auxilliaire = "ai"    ; *principal = "eu";   return;
                          case NSPhraseur::pers2S : *auxilliaire = "as"    ; *principal = "eu";   return;
                          case NSPhraseur::pers3S : *auxilliaire = "a"     ; *principal = "eu";   return;
                          case NSPhraseur::pers1P : *auxilliaire = "avons" ; *principal = "eu";  return;
                          case NSPhraseur::pers2P : *auxilliaire = "avez"  ; *principal = "eu";   return;
                          case NSPhraseur::pers3P : *auxilliaire = "ont"   ; *principal = "eu"; return;
                      }
                  }
                  case aspectMoment :
                  {
                      switch (_pPh->iVbPersonne)
                      {
                          case NSPhraseur::pers1S : *principal = "avais";   return;
                          case NSPhraseur::pers2S : *principal = "avais";   return;
                          case NSPhraseur::pers3S : *principal = "avait";   return;
                          case NSPhraseur::pers1P : *principal = "avions";  return;
                          case NSPhraseur::pers2P : *principal = "aviez";   return;
                          case NSPhraseur::pers3P : *principal = "avaient"; return;
                      }
                  }
              }
              break;
          }
          case tempsPresentFr :
          {
              switch (_pPh->iVbPersonne)
              {
                  case NSPhraseur::pers1S : *principal = "ai";   return;
                  case NSPhraseur::pers2S : *principal = "as";     return;
                  case NSPhraseur::pers3S : *principal = "a";    return;
                  case NSPhraseur::pers1P : *principal = "avons"; return;
                  case NSPhraseur::pers2P : *principal = "avez";   return;
                  case NSPhraseur::pers3P : *principal = "ont";   return;
              }
              break;
          }
          case tempsFuturFr   :
          {
              switch (_pPh->iVbPersonne)
              {
                  case NSPhraseur::pers1S : *principal = "aurai";  return;
                  case NSPhraseur::pers2S : *principal = "auras";  return;
                  case NSPhraseur::pers3S : *principal = "aura";   return;
                  case NSPhraseur::pers1P : *principal = "aurons"; return;
                  case NSPhraseur::pers2P : *principal = "aurez";  return;
                  case NSPhraseur::pers3P : *principal = "auront"; return;
              }
              break;
          }
      }
  }
  //
  // Recherche du groupe
  //
  // Le verbe est du type : étudier {|T/B15/A}
  // T   pour Transitif
  // B15 pour groupe 15 du bescherell
  //
  string sTransitivite ;
  string sConjugaison ;
  string sActEtat ;

  if (false == donneVerbeClasse(*(_pPh->Verbe.begin()), &sTransitivite, &sConjugaison, &sActEtat))
    return ;

  string sLibelle ;
  if (pContexte->getDico()->donneLibelle(sLang, &sVb, &sLibelle) == false)
    return ;

  // Verbes standards du 1er groupe
  if (sConjugaison == "B06")
  {
    string sRadical = string(sLibelle, 0, strlen(sLibelle.c_str())-2) ;
    *principal = sRadical + donneVerbeTerminaison(1, "") ;
  }
  // Verbes en "ier" rien de particulier sauf iions et iiez à l'imparfait
  else if (sConjugaison == "B15")
  {
    string sRadical = string(sLibelle, 0, strlen(sLibelle.c_str())-2) ;
    *principal = sRadical + donneVerbeTerminaison(1, "") ;
  }
  // Verbes en "uire" rien de particulier sauf un s au passé et au
  //                  présent pluriel : il induisait - nous induisons
  else if (sConjugaison == "B82")
  {
    string sRadical = string(sLibelle, 0, strlen(sLibelle.c_str())-2) ;
    if      (iTempsVerbe == tempsPasseFr)
      *principal = sRadical + string("s") + donneVerbeTerminaison(3, "") ;
    else if ((iTempsVerbe == tempsPresentFr) &&
             ((_pPh->iVbPersonne == NSPhraseur::pers1P) ||
              (_pPh->iVbPersonne == NSPhraseur::pers2P) ||
              (_pPh->iVbPersonne == NSPhraseur::pers3P)))
      *principal = sRadical + string("s") + donneVerbeTerminaison(3, "") ;
    else
      *principal = sRadical + donneVerbeTerminaison(3, "") ;
  }
}

string
NSGenerateurFr::donneVerbeTerminaison(int groupe, string ssgroupe)
{
    // ssgroupe : 2 : finale "ont" : sont, ont, font, vont
    //            3 : (je, tu) peux, veux, vaux
    //            4 : verbes en "dre" sauf ceux en "indre" et "soudre"
    //            5 : assaillir, couvrir, cueillir, défaillir, offrir, ouvrir,
    //                souffrir, tressaillir

    switch (groupe)
    {
        // Verbes du 1er groupe
        //
        case 1 :
            switch (iTempsVerbe)
            {
                case tempsPasseFr   :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("ais");
                        case NSPhraseur::pers2S : return string("ais");
                        case NSPhraseur::pers3S : return string("ait");
                        case NSPhraseur::pers1P : return string("ions");
                        case NSPhraseur::pers2P : return string("iez");
                        case NSPhraseur::pers3P : return string("aient");
                    }
                    break;
                case tempsPresentFr :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("e");
                        case NSPhraseur::pers2S : return string("es");
                        case NSPhraseur::pers3S : return string("e");
                        case NSPhraseur::pers1P : return string("ons");
                        case NSPhraseur::pers2P : return string("ez");
                        case NSPhraseur::pers3P : return string("ent");
                    }
                    break;
                case tempsFuturFr   :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("erai");
                        case NSPhraseur::pers2S : return string("eras");
                        case NSPhraseur::pers3S : return string("era");
                        case NSPhraseur::pers1P : return string("erons");
                        case NSPhraseur::pers2P : return string("erez");
                        case NSPhraseur::pers3P : return string("eront");
                    }
                    break;
            }
            break;
        //
        // Verbe du 2eme groupe
        //
        case 2 :
            switch (iTempsVerbe)
            {
                case tempsPasseFr   :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("issais");
                        case NSPhraseur::pers2S : return string("issais");
                        case NSPhraseur::pers3S : return string("issait");
                        case NSPhraseur::pers1P : return string("issions");
                        case NSPhraseur::pers2P : return string("issiez");
                        case NSPhraseur::pers3P : return string("issaient");
                    }
                    break;
                case tempsPresentFr :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("is");
                        case NSPhraseur::pers2S : return string("is");
                        case NSPhraseur::pers3S : return string("it");
                        case NSPhraseur::pers1P : return string("issons");
                        case NSPhraseur::pers2P : return string("issez");
                        case NSPhraseur::pers3P : return string("issent");
                    }
                    break;
                case tempsFuturFr   :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("irai");
                        case NSPhraseur::pers2S : return string("iras");
                        case NSPhraseur::pers3S : return string("ira");
                        case NSPhraseur::pers1P : return string("irons");
                        case NSPhraseur::pers2P : return string("irez");
                        case NSPhraseur::pers3P : return string("iront");
                    }
                    break;
            }
            break;
        //
        // Verbe du 3eme groupe
        //
        case 3 :
            switch (iTempsVerbe)
            {
                case tempsPasseFr   :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("ais");
                        case NSPhraseur::pers2S : return string("ais");
                        case NSPhraseur::pers3S : return string("ait");
                        case NSPhraseur::pers1P : return string("ions");
                        case NSPhraseur::pers2P : return string("iez");
                        case NSPhraseur::pers3P : return string("aient");
                    }
                    break;
                case tempsPresentFr :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S :
                            if (ssgroupe == "3")
                                return string("x");
                            if (ssgroupe == "5")
                                return string("e");
                            return string("s");
                        case NSPhraseur::pers2S :
                            if (ssgroupe == "3")
                                return string("x");
                            if (ssgroupe == "5")
                                return string("es");
                            return string("s");
                        case NSPhraseur::pers3S :
                            if (ssgroupe == "4")
                                return string("d");
                            if (ssgroupe == "5")
                                return string("e");
                            return string("t");
                        case NSPhraseur::pers1P : return string("ons");
                        case NSPhraseur::pers2P : return string("ez");
                        case NSPhraseur::pers3P :
                            if (ssgroupe == "2")
                                return string("nt");
                            return string("ent");
                    }
                    break;
                case tempsFuturFr   :
                    switch (_pPh->iVbPersonne)
                    {
                        case NSPhraseur::pers1S : return string("rai");
                        case NSPhraseur::pers2S : return string("ras");
                        case NSPhraseur::pers3S : return string("ra");
                        case NSPhraseur::pers1P : return string("rons");
                        case NSPhraseur::pers2P : return string("rez");
                        case NSPhraseur::pers3P : return string("ront");
                    }
                    break;
            }
            break;
    }
    return string("");
}

bool
NSGenerateurFr::donneVerbeClasse(NSPhraseMot* /* pPhraseMot */, string* psTransitivite, string* psConjugaison, string* psActEtat)
{
  // En français, le verbe est du type : étudier {|T/B15/A}
  // T   pour Transitif, I intransitif
  // B15 pour groupe 15 du bescherell
  // A   pour verbe d'Action,
  // E   verbe d'état (ER réel "être", EA apparent "sembler", ED dure "reste"
  //                   EC change "devenir")

  if (psTransitivite)
    *psTransitivite = string("") ;
  if (psConjugaison)
    *psConjugaison  = string("") ;
  if (psActEtat)
    *psActEtat      = string("") ;

  if (_pPh->Verbe.empty())
    return false ;

  NSPhraseMot* pVerb = (*(_pPh->Verbe.begin())) ;
  if ((NSPhraseMot*) NULL == pVerb)
    return false ;

  NSPathologData* pData = pVerb->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return false ;

  if (false == pData->estVerbe())
    return false ;

  string sTransitif   = pData->chercheGrammaire("T") ;
  string sIntransitif = pData->chercheGrammaire("I") ;
  string sEtat        = pData->chercheGrammaire("E") ;
  string sAction      = pData->chercheGrammaire("A") ;

  // Un verbe ne peut pas etre a la fois d'action et d'etat, ni a la fois
  // transitif et intransitif(ou au moins dans le lexique).
  // Dans les deux cas, un verbe est soit l'un soit l'autre.

  if ((string("") != sTransitif) && (string("") != sIntransitif))
    return false ;

  if ((string("") != sAction) && (string("") != sEtat))
    return false ;

  if ((string("") == sTransitif) && (string("") == sIntransitif))
    return false ;

  if ((string("") == sAction) && (string("") == sEtat))
    return false ;

  if (psConjugaison)
  {
    *psConjugaison = pData->chercheGrammaire("B") ;
    if (string("") == *psConjugaison)
      return false ;
  }

  if (psTransitivite && (string("") != sTransitif))
    *psTransitivite = sTransitif ;

  if (psTransitivite && (string("") != sIntransitif))
    *psTransitivite = sIntransitif ;

  if (psActEtat && (string("") != sAction))
    *psActEtat = sAction ;

  if (psActEtat && (string("") != sEtat))
    *psActEtat = sEtat ;

  if (psActEtat && (string("") != *psActEtat))
  {
    if      (((*psActEtat)[0] == 'A') || ((*psActEtat)[0] == 'a'))
      _pPh->iVbType = NSPhraseur::vbTypeAction;
    else if (((*psActEtat)[0] == 'E') || ((*psActEtat)[0] == 'e'))
    {
      _pPh->iVbType = NSPhraseur::vbTypeEtat ;
      if (strlen(psActEtat->c_str()) > 1)
      {
        if      (((*psActEtat)[1] == 'R') || ((*psActEtat)[1] == 'r'))
          _pPh->iVbEtat = NSPhraseur::vbEtatReel;
        else if (((*psActEtat)[1] == 'A') || ((*psActEtat)[1] == 'a'))
          _pPh->iVbEtat = NSPhraseur::vbEtatApparent;
        else if (((*psActEtat)[1] == 'D') || ((*psActEtat)[1] == 'd'))
          _pPh->iVbEtat = NSPhraseur::vbEtatDure;
        else if (((*psActEtat)[1] == 'C') || ((*psActEtat)[1] == 'c'))
          _pPh->iVbEtat = NSPhraseur::vbEtatChange;
      }
    }
  }
  return true ;
}

string
NSGenerateurFr::donneAuxilliaire(string* Auxilliaire, int iVbTemps, int /* iVbAspect */, int /* iVbMode */, int iVbPersonne, int /* iVbNegation */)
{
  if (string("4ETRE") == *Auxilliaire)
  {
    switch (iVbTemps)
    {
      case tempsPasseFr   :
        switch (iVbPersonne)
        {
          case NSPhraseur::pers1S : return string("étais") ;
          case NSPhraseur::pers2S : return string("étais") ;
          case NSPhraseur::pers3S : return string("était") ;
          case NSPhraseur::pers1P : return string("étions") ;
          case NSPhraseur::pers2P : return string("étiez") ;
          case NSPhraseur::pers3P : return string("étaient") ;
        }
        break ;
      case tempsPresentFr :
        switch (iVbPersonne)
        {
          case NSPhraseur::pers1S : return string("suis") ;
          case NSPhraseur::pers2S : return string("es") ;
          case NSPhraseur::pers3S : return string("est") ;
          case NSPhraseur::pers1P : return string("sommes") ;
          case NSPhraseur::pers2P : return string("êtes") ;
          case NSPhraseur::pers3P : return string("sont") ;
        }
        break ;
      case tempsFuturFr   :
        switch (iVbPersonne)
        {
          case NSPhraseur::pers1S : return string("serai") ;
          case NSPhraseur::pers2S : return string("seras") ;
          case NSPhraseur::pers3S : return string("sera") ;
          case NSPhraseur::pers1P : return string("serons") ;
          case NSPhraseur::pers2P : return string("serez") ;
          case NSPhraseur::pers3P : return string("seront") ;
        }
        break;
    }
  }
  return string("") ;
}

// traitePrepositionCC : méthode qui met les prépositions, spécifiquement pour les
// les compléments circonstanciels.

bool
NSGenerateurFr::traitePrepositionCC(NSPhraseMotArray* pCompC, NSPhraseMot* pPreposition, NSPhraseur::VBPREPOCTX iContexte)
{
  iterPhraseMot  iterMots;

  if (pPreposition)
  {
    if (pPreposition->getLexique() != string(""))
    {
      traitePostposition(pPreposition, pCompC) ;
      return true ;
    }
  }

  // Si on n'a pas specifie de preposition, il faut au moins avoir specifie un contexte.

  if (NSPhraseur::prepUndefined == iContexte)
    return false ;

  // Si on a specifie un contexte, on met une preposition par defaut en fonction du
  // contexte.

  // Pour le lieu, on met "à" en général et "de" en complément du nom :
  //
  // La PA à la cheville est de XX mmHg / La PA est de XX mmHG à la cheville
  // La longueur de la jambe est de 25 cm
  //
  if (NSPhraseur::prepLieu == iContexte)
  {
    if (pCompC)
    {
      if (false == pCompC->empty())
      {
        string sPrepos = string("à") ;
        if ((phraseComplement == _pPh->iPhraseType) ||
                             (NSPhraseur::locFrom == _pPh->iTypeLocalisation))
          sPrepos = string("de") ;

        for (iterMots = pCompC->begin(); pCompC->end() != iterMots ; iterMots++)
        {
          if (false == (*iterMots)->estTexteLibre())
          {
            NSPathologData* pPathologData = (*iterMots)->getLexiqueData(sLang) ;
            if (((NSPathologData*) NULL == pPathologData) ||
                   (string("") == pPathologData->chercheGrammaire(PREPOSITION)))
            {
              if (string("") == (*iterMots)->getPreposition())
                (*iterMots)->setPreposition(sPrepos) ;
              (*iterMots)->setArticle(NSPhraseMot::articleDefini) ;
            }
          }
          else
          {
            // On rajoute la preposition dans le libelle du texte libre.
            if (string("") == (*iterMots)->getPreposition())
              (*iterMots)->setPreposition(sPrepos) ;
            (*iterMots)->setTexteLibre(sPrepos + string(" ") + (*iterMots)->getTexteLibre()) ;
          }
        }
        return true ;
      }
    }
  }

  // Pour le temps, on met..., on met quoi au fait?

  // Pour la maniere, on met "avec"
  else if (NSPhraseur::prepManiere == iContexte)
  {
    (_pPh->PrepositionManiere).setLexique("0AVEC1") ;
    return traitePrepositionCC(pCompC, &(_pPh->PrepositionManiere)) ;
  }

  // Pour la cause, mettre "a cause de".
  // Pour le but, mettre "pour".

  // pour le type, on met "a type de"

  else if (NSPhraseur::prepType == iContexte)
  {
    (_pPh->PrepositionType).setLexique("0ATYP1") ;
    return traitePrepositionCC(pCompC, &(_pPh->PrepositionManiere)) ;
  }
  else if (NSPhraseur::prepHypoth == iContexte)
  {
    (_pPh->PrepositionHypoth).setLexique("KEVEI2") ;
    return traitePrepositionCC(pCompC, &(_pPh->PrepositionHypoth)) ;
  }

  return false ;
}


// modifie préposition : méthode qui contracte s'il le faut le "de" en "d'".

bool
NSGenerateurFr::modifiePreposition(NSPhraseMot* pMot, int iArticle, bool bPluriel)
{
  if ((NSPhraseMot*) NULL == pMot)
    return false ;

  string sLibelle    = string("") ;
  bool   bApostrophe = false ;
  GENRE  iGenreNom   = genreNull ;

  NSPhraseMot    MotAdj(pContexte) ;
  NSPhraseMot    MotAdv(pContexte) ;

  NSPathologData DataAdj ;
  NSPathologData DataAdv ;

  string sLexiqueAdj = string("") ;
  string sLexiqueAdv = string("") ;

  string sCertitudeAdj = string("") ;
  string sCertitudeAdv = string("") ;

  string sCertitudeNom = string(pMot->getCertitude(), 0, 5) ;

  NSPathologData* pData = pMot->getLexiqueData(sLang) ;
  if ((NSPathologData*) NULL == pData)
    return false ;

  pData->donneGenre(&iGenreNom) ;
  donneLibelleAffiche(&sLibelle, pData) ;

  // En francais, on n'a que le "de" à changer parfois en "d'".

  string sPreposition = pMot->getPreposition() ;

  // Soit "de", soit une preposition qui se termine par " de"
  //
  if ((string("de") != sPreposition) ||
      ((strlen(sPreposition.c_str()) > 2) &&
       (string(sPreposition, strlen(sPreposition.c_str())-3, 3) != " de")))
    return false ;

  if (NSPhraseMot::articleDefini == iArticle)
    return false ;

  if (NSPhraseMot::articleIndefini == iArticle)
  {
    if (false == bPluriel)
    {
      sPreposition.replace(strlen(sPreposition.c_str())-2, 2, "d'") ;
      pMot->setPreposition(sPreposition) ;
      return true ;
    }
    else
      return false ;
  }

  if (CommenceParVoyelle(sLibelle))
    bApostrophe = true ;


  if (('h' == sLibelle[0]) || ('H' == sLibelle[0]))
  {
    if (pData->chercheGrammaire(VALEUR_DE_H) == H_MUET)
      bApostrophe = true;
  }

  if (pMot->getComplementPhr())
  {
    bool bPremierAdjExiste = donnePremierAdj(pMot->getFirstComplementPhr(), &MotAdj) ;
    sLexiqueAdj = MotAdj.getLexique() ;

    if (bPremierAdjExiste)
    {
      sCertitudeAdj = string(MotAdj.getCertitude(), 0, 5) ;

      if (string("") != sCertitudeAdj)
      {
        // Le premier adjectif est precede d'une certitude, donc on ne
        // s'occupe pas de la premiere lettre de l'adjectif.

        donneCertitude(sCertitudeAdj, &bApostrophe);
      }
      else if (MotAdj.getFirstComplementPhr())
      {
        bool bPremierAdvExiste = donnePremierAdv(MotAdj.getFirstComplementPhr(), &MotAdv);
        sLexiqueAdv = MotAdv.getLexique() ;

        if (bPremierAdvExiste)
        {
          sCertitudeAdv = string(MotAdv.getCertitude(), 0, 5) ;

          if (string("") != sCertitudeAdv)
          {
            // Le premier adverbe est precede d'une certitude, donc on ne
            // s'occupe pas de la premiere lettre de l'adverbe.

            donneCertitude(sCertitudeAdv, &bApostrophe) ;
          }
          else
          {
            // On regarde si l'adverbe commence par une voyelle ou par un
            // h muet.
            string sLibelleAdv = string("") ;
            donneLibelleAffiche(&sLibelleAdv, &DataAdv) ;

            if (CommenceParVoyelle(sLibelleAdv))
              bApostrophe = true ;

            else if (('h' == sLibelleAdv[0]) || ('H' == sLibelleAdv[0]))
            {
              if (DataAdv.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                bApostrophe = true;
            }
          }
        }
        else
        {
          // On regarde si l'adjectif commence par une voyelle ou par un
          // h muet.
          string sLibelleAdj = string("") ;
          donneLibelleAffiche(&sLibelleAdj, &DataAdj, iGenreNom) ;

          if (CommenceParVoyelle(sLibelleAdj))
            bApostrophe = true ;

          else if (('h' == sLibelleAdj[0]) || ('H' == sLibelleAdj[0]))
          {
            if (DataAdj.chercheGrammaire(VALEUR_DE_H) == H_MUET)
              bApostrophe = true ;
          }
        }
      }
    }
  }

  if (bApostrophe)
  {
    sPreposition.replace(strlen(sPreposition.c_str())-2, 2, "d'") ;
    pMot->setPreposition(sPreposition) ;
    return true ;
  }

  return false ;
}

string
NSGenerateurFr::postTraitement(string *psEntree)
{
  if ((NULL == psEntree) || (string("") == *psEntree))
    return string("") ;

  string sSortie = *psEntree ;

  // Contraction de la forme "de le " en "du "
  //
  contracting(&sSortie, string("de le "), string("du ")) ;

  // Contraction de la forme "de l' " en "de l'"
  //
  contracting(&sSortie, string("de l' "), string("de l'")) ;

  // Contraction des formes "de les " ou "de des " en "des "
  //
  contracting(&sSortie, string("de les "), string("des ")) ;
  contracting(&sSortie, string("de des "), string("des ")) ;

  // Contraction de la forme "à les " en "aux " et "à le " en "au "
  //
  contracting(&sSortie, string("à le "), string("au ")) ;
  contracting(&sSortie, string("à les "), string("aux ")) ;

  // Contraction de "est non " en "n'est pas "
  //
  contracting(&sSortie, string("est non "), string("n'est pas ")) ;

  // Contraction de "de un " en "d'un " et de "de une " en "d'une "
  //
  contracting(&sSortie, string("de un "), string("d'un ")) ;
  contracting(&sSortie, string("de une "), string("d'une ")) ;

  // Gestion des compléments circonstanciels de moyen en mode négatif
  //
  contracting(&sSortie, string("avec pas de"), string("sans")) ;
  contracting(&sSortie, string("pas avec de"), string("sans")) ;

  return sSortie ;
}

string
NSGenerateurFr::getParticipePresent(string sVerb, bool bAffirmative)
{
  string sVbLabel = string("") ;

  if (string("4ETRE1") == sVerb)
    sVbLabel = string("étant") ;
  else
  {
    NSPathologData DataVb ;

    bool bFound = getPathologData(sVerb, DataVb) ;
    if (false == bFound)
      return string("") ;

    sVbLabel = donneParticipePresent(&DataVb) ;

    if (false == bAffirmative)
    {
      // L'auxilliaire (être ou avoir) ne commence jamais par un h.
      if (CommenceParVoyelle(sVbLabel))
        sVbLabel = string("n'") + sVbLabel ;
      else
        sVbLabel = string("ne ") + sVbLabel ;
    }
  }

  return sVbLabel ;
}

//
// ////////////////  OBJET  gereDateFr   /////////////////
//

//
//  constructeur copie
//
gereDateFr::gereDateFr(const gereDateFr& src)
		       :gereDate(src._pSuper, src._sLang)
{
  gereDate(*this) = (gereDate) src ;
}

//
//  opérateur d'affectation
//
gereDateFr&
gereDateFr::operator=(const gereDateFr& src)
{
  if (this == &src)
		return *this ;

  gereDate(*this) = (gereDate) src ;
  
  return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereDateFr::donne_date_breve(string* pMessage, string* pIntro, int iType)
{
  if ((NULL == pMessage) || (NULL == pIntro))
    return ;

  *pMessage = string("") ;
  *pIntro   = string("") ;

  // Date au format AAAAMMJJ
	//
  if (string("2DA01") == _sUnite)
    donne_date_breve_for_date(pMessage, pIntro, iType) ;

  //
  // Date au format AAAAMMJJhhmmss
	//
  else if (string("2DA02") == _sUnite)
  {
    donne_date_breve_for_date(pMessage, pIntro, iType) ;

    size_t iDateLen = strlen(_sDate.c_str()) ;

    if ((iDateLen <= 8) || (string("00") == string(_sDate, 6, 2)))
      return ;

    string sHeure = string("") ;

    // gestion de l'heure
    if      (10 == iDateLen)
      sHeure = string(_sDate, 8, 2) + string("h") ;
    else if (12 == iDateLen)
      sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) ;
    else if (14 == iDateLen)
      sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;

    if (string("") != sHeure)
      *pMessage += string(" à ") + sHeure ;
  }

  //
  // Heure au format HHmm ou HH
	//
  else if ((string("2HE01") == _sUnite) || (string("2HE00") == _sUnite))
  {
    if (string("2HE01") == _sUnite)
      *pMessage = string(_sDate, 0, 2) + string(":") + string(_sDate, 2, 2) ;
    if (string("2HE00") == _sUnite)
      *pMessage = _sDate + string(":") ;

    if ('0' == (*pMessage)[0])
      *pMessage = string(*pMessage, 1, strlen(pMessage->c_str()) - 1) ;

    switch (iType)
    {
      case DateNormal :
        *pIntro = string("à ") ;
        break ;
      case DateDeb :
        *pIntro = string("de ") ;
        break ;
      case DateFin :
        *pIntro = string("à ") ;
        break ;
    }
  }
}

void
gereDateFr::donne_date_breve_for_date(string* pMessage, string* pIntro, int iType)
{
  if ((NULL == pMessage) || (NULL == pIntro))
    return ;

  *pMessage = string("") ;
  *pIntro   = string("") ;

  if (strlen(_sDate.c_str()) < 8)
    return ;

  // Date vide
  if (string("00000000") == string(_sDate, 0, 8))
    return ;

  // Juste AAAA -> "en 2001"
  if (string(_sDate, 4, 4) == string("0000"))
  {
    *pMessage = string(_sDate, 0, 4) ;

    switch (iType)
    {
      case DateNormal :
        *pIntro = string("en ") ;
        break ;
      case DateDeb :
        *pIntro = string("de ") ;
        break ;
      case DateFin :
        *pIntro = string("à ") ;
        break ;
    }

    return ;
  }

  // MM/AAAA -> "en juillet 2001"
  //
  if (string(_sDate, 6, 2) == string("00"))
  {
    *pMessage = donne_mois(string(_sDate, 4, 2)) + string(" ") + string(_sDate, 0, 4) ;

    switch (iType)
    {
      case DateNormal :
        *pIntro = string("en ") ;
        break ;
      case DateDeb :
        *pIntro = string("du ") ;
        break ;
      case DateFin :
        *pIntro = string("au ") ;
        break ;
    }

    return ;
  }

  // JJ/MM/AAAA -> "le 25/07/2001"
  //
  *pMessage = string(_sDate, 6, 2) + string("/") + string(_sDate, 4, 2) + string("/") + string(_sDate, 0, 4) ;

  switch (iType)
  {
    case DateNormal :
      *pIntro = string("le ") ;
      break ;
    case DateDeb :
      *pIntro = string("du ") ;
      break ;
    case DateFin :
      *pIntro = string("au ") ;
      break ;
  }
}

//  +-----------------------------------------------------------------+
//  ¦           Donne la date au format "1er janvier 1994"            ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereDateFr::donne_date_claire(string* pMessage, string* pIntro, int iType)
{
  if (((string*) NULL == pMessage) || ((string*) NULL == pIntro))
    return ;

  *pMessage = string("") ;
  *pIntro   = string("") ;

  //
  // Date au format AAAAMMJJ
	//
  if (string("2DA01") == _sUnite)
    donne_date_claire_for_date(pMessage, pIntro, iType) ;

  // Date au format AAAAMMJJhhmmss
	//
  else if (string("2DA02") == _sUnite)
  {
    donne_date_claire_for_date(pMessage, pIntro, iType) ;

    size_t iDateLen = strlen(_sDate.c_str()) ;

    if ((iDateLen <= 8) || (string("00") == string(_sDate, 6, 2)))
      return ;

    string sHeure = string("") ;

    // gestion de l'heure
    //
    if      (10 == iDateLen)
      sHeure = string(_sDate, 8, 2) + string(" heures") ;
    else if (12 == iDateLen)
      sHeure = string(_sDate, 8, 2) + string(" heures ") + string(_sDate, 10, 2) ;
    else if (14 == iDateLen)
      sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;

    if (string("") != sHeure)
      *pMessage += string(" à ") + sHeure ;
  }

  // Heure au format HHmm ou HH
	//
  else if ((string("2HE01") == _sUnite) || (string("2HE00") == _sUnite))
  {
    if (string("2HE01") == _sUnite)
      *pMessage = string(_sDate, 0, 2) + string("h") + string(_sDate, 2, 2) ;
    if (string("2HE00") == _sUnite)
      *pMessage = _sDate + string("h") ;

    if ('0' == (*pMessage)[0])
      *pMessage = string(*pMessage, 1, strlen(pMessage->c_str()) - 1) ;

    switch (iType)
    {
      case DateNormal :
        *pIntro = string("à ") ;
        break ;
      case DateDeb :
        *pIntro = string("de ") ;
        break;
      case DateFin :
        *pIntro = string("à ") ;
        break;
    }
  }
}

void
gereDateFr::donne_date_claire_for_date(string* pMessage, string* pIntro, int iType)
{
  if (strlen(_sDate.c_str()) < 8)
      return ;

  switch (iType)
  {
    case DateNormal :
      *pIntro = string("le ") ;
      break ;
    case DateDeb :
      *pIntro = string("du ") ;
      break ;
    case DateFin :
      *pIntro = string("au ") ;
      break ;
  }
  //
  // Jour
  //
  if (isdigit(_sDate[6]) && isdigit(_sDate[7]))
  {
    if ('0' == _sDate[6])
    {
      if      ('1' == _sDate[7])
        *pMessage = string("1er ") ;
      else if ('0' != _sDate[7])
        *pMessage = string(1, _sDate[7]) + string(" ") ;
      else
      {
        switch (iType)
        {
          case DateNormal :
            *pIntro = string("en ") ;
            break ;
          case DateDeb :
            *pIntro = string("de ") ;
            break ;
          case DateFin :
            *pIntro = string("à ") ;
            break ;
        }
      }
    }
    else
      *pMessage = string(_sDate, 6, 2) + string(" ") ;
  }
  else
    *pMessage = string("?? ") ;

  // Mois
  //
  string sMonth = string(_sDate, 4, 2) ;
  if (string("00") != sMonth)
  {
    string sMois = donne_mois(string(_sDate, 4, 2)) ;
    if (string("") != sMois)
      *pMessage += sMois + string(" ") ;
/*
    else
      *pMessage = string("?? ") ;
*/
  }

  // Année
  //
  for (int i = 0 ; i < 4 ; i++)
  {
    if (isdigit(_sDate[i]))
      *pMessage += string(1, _sDate[i]) ;
    else
      *pMessage += string(1, '?') ;
  }
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir de "MM"              ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDateFr::donne_mois(string mois)
{
  if (strlen(mois.c_str()) < 2)
    return string("") ;

  if ((!(isdigit(mois[0]))) || (!(isdigit(mois[1]))))
    return string("") ;

  int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]) ;

  return donne_mois(i) ;
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir d'un int             ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDateFr::donne_mois(int iMois)
{
  switch (iMois)
  {
    case  1 : return string("janvier") ;
		case  2 : return string("février") ;
		case  3 : return string("mars") ;
		case  4 : return string("avril") ;
		case  5 : return string("mai") ;
		case  6 : return string("juin") ;
		case  7 : return string("juillet") ;
		case  8 : return string("août") ;
		case  9 : return string("septembre") ;
		case 10 : return string("octobre") ;
		case 11 : return string("novembre") ;
		case 12 : return string("décembre") ;
  }

  return string("") ;
}

//
// ////////////////  OBJET  gereNumFr   /////////////////
//

//
//  constructeur copie
//
gereNumFr::gereNumFr(const gereNumFr& src)
          :gereNum(src._pSuper, src._sLang)
{
  gereNum(*this) = (gereNum) src ;
}

//
//  opérateur d'affectation
//
gereNumFr&
gereNumFr::operator=(const gereNumFr& src)
{
  if (this == &src)
		return *this ;

  gereNum(*this) = (gereNum) src ;

  return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereNumFr::getLabel() const
{
  string sLibelle = string("") ;

/*
  if (estExact() || estInf() || estSup())
  {
    if (estExact())
      sLibelle = getNum(pContexte, "") ;
    else
    {
      if ((estInf()) && (estSup()))
        sLibelle = "entre " + getNumInf() + " et " + getNumSup() ;
      else if (estInf())
        sLibelle = "plus de " + getNumInf() ;
      else if (estSup())
        sLibelle = "moins de " + getNumSup() ;
    }

    //
    // Libellé de l'unité de l'unité
    //
    if (string("") != sLibelle)
    {
      string sUniteLibel = donneLibelleUnite(pContexte) ;

      if (string("") != sUniteLibel)
        sLibelle += string(" ") + sUniteLibel ;
    }
  }
*/

  return sLibelle ;
}

//
// ---------------- sentenceBlocks ----------------
//

SentenceBlocks::SentenceBlocks()
{
  _iComplLevelThreshold = 101 ;
  _bPhraseAffirmative   = true ;

  _sSujet      = string("") ;
  _sAttSujet   = string("") ;
  _iGenreSujet = genreNull ;

  _sVerbe      = string("") ;

  _sPhraseAdverbe          = string("") ;
  _sAdverbeModificateur    = string(" ") ;
  _sAdverbeModificateurAtt = string(" ") ;

  _sCOD        = string("") ;
  _sAttCOD     = string("") ;
  _iGenreCOD   = genreNull ;
}

SentenceBlocks::SentenceBlocks(const SentenceBlocks& rv)
{
  _iComplLevelThreshold = rv._iComplLevelThreshold ;
  _bPhraseAffirmative   = rv._bPhraseAffirmative ;

  _sSujet      = rv._sSujet ;
  _sAttSujet   = rv._sAttSujet ;
  _iGenreSujet = rv._iGenreSujet ;

  _sVerbe      = rv._sVerbe ;

  _sPhraseAdverbe          = rv._sPhraseAdverbe ;
  _sAdverbeModificateur    = rv._sAdverbeModificateur ;
  _sAdverbeModificateurAtt = rv._sAdverbeModificateurAtt ;

  _sCOD        = rv._sCOD ;
  _sAttCOD     = rv._sAttCOD ;
  _iGenreCOD   = rv._iGenreCOD ;
}

SentenceBlocks::~SentenceBlocks()
{
}

SentenceBlocks&
SentenceBlocks::operator=(const SentenceBlocks& src)
{
  if (&src == this)
		return *this ;

  _iComplLevelThreshold = src._iComplLevelThreshold ;
  _bPhraseAffirmative   = src._bPhraseAffirmative ;

  _sSujet      = src._sSujet ;
  _sAttSujet   = src._sAttSujet ;
  _iGenreSujet = src._iGenreSujet ;

  _sVerbe      = src._sVerbe ;

  _sPhraseAdverbe          = src._sPhraseAdverbe ;
  _sAdverbeModificateur    = src._sAdverbeModificateur ;
  _sAdverbeModificateurAtt = src._sAdverbeModificateurAtt ;

  _sCOD        = src._sCOD ;
  _sAttCOD     = src._sAttCOD ;
  _iGenreCOD   = src._iGenreCOD ;

  return *this ;
}

