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

// -------------------------------------------------------------------------
// ---------------------- METHODES DE generateurEn -------------------------
// -------------------------------------------------------------------------

NSGenerateurEn::NSGenerateurEn(NSContexte* pCtx, NsProposition* pPropos, string sLangue)
               :NSGenerateur(pCtx, pPropos, sLangue)
{
}

NSGenerateurEn::NSGenerateurEn(const NSGenerateurEn& rv)
               :NSGenerateur(rv)
{
}

NSGenerateurEn::~NSGenerateurEn()
{
}

NSGenerateurEn&
NSGenerateurEn::operator=(const NSGenerateurEn& rv)
{
	if (&rv == this)
		return *this ;

	reinitialise() ;
  copieTout(&rv) ;

	return *this ;
}

void
NSGenerateurEn::classeTout()
{
try
{
	iterPhraseMot  iterMots;
	NSPathologData Data;

	reinitialise() ;

  // Les adjectifs ayant une certitude sont toujours places apres le nom.

  if (false == _pPh->adjEpitheteAv.empty())
  {
    for (iterMots = _pPh->adjEpitheteAv.begin(); iterMots != _pPh->adjEpitheteAv.end(); iterMots++)
    {
      // Les adjectifs ont deja ete tries en avant et apres, donc on ne reverifie pas
      // qu'il s'agit bien d'adjectifs du lexique.

      string sCertitude = (*iterMots)->getCertitude() ;

      if (sCertitude != "")
      {
          if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
              ajouteCopieMot(&_pAdjEpitheteApNeg, *iterMots);
          else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50") && (string(sCertitude, 0, 5) != "WCEA0"))
              ajouteCopieMot(&_pAdjEpitheteApPos, *iterMots);
          else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) == "WCEA0"))
              ajouteCopieMot(&_pAdjEpitheteAvPos, *iterMots);
      }
      else
          ajouteCopieMot(&_pAdjEpitheteAvPos, *iterMots);
    }
  }

  if (false == _pPh->adjEpitheteAp.empty())
  {
    for (iterMots = _pPh->adjEpitheteAp.begin(); iterMots != _pPh->adjEpitheteAp.end(); iterMots++)
    {
        // Les adjectifs ont deja ete tries en avant et apres, donc on ne reverifie pas
        // qu'il s'agit bien d'adjectifs du lexique.

        string sCertitude = (*iterMots)->getCertitude() ;

        if (sCertitude != "")
        {
            if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                ajouteCopieMot(&_pAdjEpitheteApNeg, *iterMots);
            else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                ajouteCopieMot(&_pAdjEpitheteApPos, *iterMots);
        }
        else
            ajouteCopieMot(&_pAdjEpitheteApPos, *iterMots);
    }
  }

  if (false == _pPh->compNom.empty())
  {
    for (iterMots = _pPh->compNom.begin(); iterMots != _pPh->compNom.end(); iterMots++)
    {
      string sLexique = (*iterMots)->getLexique() ;

      if (false == (*iterMots)->estTexteLibre())
      {
        bool bFound = getPathologData(sLexique, Data) ;
        if (false == bFound)
          return ;
        if (false == Data.estNom())
          return ;
      }
      string sCertitude = (*iterMots)->getCertitude() ;

      if (sCertitude != "")
      {
        if ((string(sCertitude, 0, 3) == string("WCE")) && (string(sCertitude, 0, 5) < string("WCE50")))
          ajouteCopieMot(&_pCompNomNeg, *iterMots) ;
        else if ((string(sCertitude, 0, 3) == string("WCE")) && (string(sCertitude, 0, 5) >= string("WCE50")))
          ajouteCopieMot(&_pCompNomPos, *iterMots) ;
      }
      else
        ajouteCopieMot(&_pCompNomPos, *iterMots);
    }
  }

    if (false == _pPh->adverbe.empty())
    {
        for (iterMots = _pPh->adverbe.begin(); iterMots != _pPh->adverbe.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getLexique() ;
            if (false == (*iterMots)->estTexteLibre())
            {
                bool trouve = getPathologData(sLexique, Data) ;
                if (!trouve)
                    return;
                if (!Data.estAdverbe())
                    return;
            }

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&_pAdverbeNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&_pAdverbePos, *iterMots);
            }
            else
                ajouteCopieMot(&_pAdverbePos, *iterMots);
        }
    }

    if (false == _pPh->COD.empty())
    {
        for (iterMots = _pPh->COD.begin(); iterMots != _pPh->COD.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getCertitude() ;

            if (false == (*iterMots)->estTexteLibre())
            {
                bool trouve = getPathologData(sLexique, Data) ;
                if (!trouve)
                    return;
                if (!Data.estNom())
                    return;
            }

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&_pCODNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&_pCODPos, *iterMots);
            }
            else
                ajouteCopieMot(&_pCODPos, *iterMots);
        }
    }

    if (false == _pPh->AttSujet.empty())
    {
        for (iterMots = _pPh->AttSujet.begin(); iterMots != _pPh->AttSujet.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getLexique() ;

            if (!((*iterMots)->estTexteLibre()))
            {
                bool trouve = getPathologData(sLexique, Data) ;
                if (!trouve)
                    return;
            }

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&_pAttSujetNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&_pAttSujetPos, *iterMots);
            }
            else
                ajouteCopieMot(&_pAttSujetPos, *iterMots);
        }
    }

    if (false == _pPh->AttCOD.empty())
    {
        for (iterMots = _pPh->AttCOD.begin(); iterMots != _pPh->AttCOD.end(); iterMots++)
        {
            string sLexique = (*iterMots)->getLexique() ;

            if (!((*iterMots)->estTexteLibre()))
            {
                bool trouve = getPathologData(sLexique, Data) ;
                if (!trouve)
                    return;
            }

            string sCertitude = (*iterMots)->getCertitude() ;

            if (sCertitude != "")
            {
                if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) < "WCE50"))
                    ajouteCopieMot(&_pAttCODNeg, *iterMots);
                else if ((string(sCertitude, 0, 3) == "WCE") && (string(sCertitude, 0, 5) >= "WCE50"))
                    ajouteCopieMot(&_pAttCODNeg, *iterMots);
            }
            else
                ajouteCopieMot(&_pAttCODNeg, *iterMots);
        }
    }

    _pCCArray->classeTout();
}
catch (...)
{
	erreur("Exception NSGenerateurEn::classeTout", standardError) ;
}
}

void
NSGenerateurEn::donneLibelleAffiche(string* pLibelle, NSPathologData* pData, GENREVERBE iGenre)
{
  if (NULL == pLibelle)
    return ;

  // En anglais, cette methode sert pour les noms et les verbes.
  // Les noms peuvent etre singuliers ou pluriel.
  // Les verbes peuvent etre au participe present, au preterite ou au participe
  // passe.

  int iDeclinaison = 0 ;
  *pLibelle = string("") ;

  if (NULL == pData)
    return ;

  if (pData->estVerbe())
  {
    switch (iGenre)
    {
      case declinaisonBase        : iDeclinaison = declinaisonBase ;        break ;
      case declinaison3Personne   : iDeclinaison = declinaison3Personne ;   break ;
      case declinaisonPartPresent : iDeclinaison = declinaisonPartPresent ; break ;
      case declinaisonPreterite   : iDeclinaison = declinaisonPreterite ;   break ;
      case declinaisonPartPasse   : iDeclinaison = declinaisonPartPasse ;   break ;
      default                     : iDeclinaison = 0 ;
    }

    pData->donneLibelleAffiche(pLibelle, iDeclinaison) ;
  }
  else
    pData->donneLibelleAffiche(pLibelle) ;
}

void
NSGenerateurEn::donneLibelleAffiche(string* pLibelle, NSPathologData* pData, GENRE iGenre)
{
  if (NULL == pLibelle)
    return ;

  // En anglais, cette methode sert pour les noms et les verbes.
  // Les noms peuvent etre singuliers ou pluriel.
  // Les verbes peuvent etre au participe present, au preterite ou au participe
  // passe.

  *pLibelle = string("") ;

  if (NULL == pData)
    return ;

  if (pData->estNom())
  {
    if ((genreMP == iGenre) || (genreFP == iGenre) || (genreNP == iGenre))
      pData->donneLibelleAffiche(pLibelle, declinaisonPluriel) ;
    else
      pData->donneLibelleAffiche(pLibelle, declinaisonSingulier) ;
  }
  else
    pData->donneLibelleAffiche(pLibelle) ;
}

bool
NSGenerateurEn::generePhrase(DCODETYPE iStyle)
{
  reinitialise() ;
  _sPhrase = string("") ;

  string   sLexique;
  string   sPluriel;
  string   sCertitude;
  string   sInter1 = "";
  string   sInter2 = "";
  bool     bSucces;

  // La phrase est-elle affirmative ou negative ?
  // true pour phrase affirmative, false pour phrase negative.

  bool    bPhraseAffirmative = true ;

  if (NSPhraseur::negationOui == _pPh->iVbNegation)
    bPhraseAffirmative = false ;

  string  sAdverbeModificateur    = string(" ") ;
  string  sAdverbeModificateurAtt = string(" ") ;

  iterPhraseMot  iterMots;
  NSPathologData Data;

  string sVerbe = "";

  // Pour les adverbes du verbe

  string sPhraseAdverbe = "";

  string sPhrasePos = "";
  string sPhraseNeg = "";
  string sInterPos1 = "";
  string sInterPos2 = "";
  string sInterNeg1 = "";
  string sInterNeg2 = "";

  // Avant de classer les complements, on leur met la preposition lorsqu'il y
  // en a une d'imposee par le mot complete.
  // Le verbe peut imposer une preposition au COI ainsi qu'au COS.
  // Il faut traiter les prepositions des complements circonstanciels.

  if (false == _pPh->Verbe.empty())
    traitePostposition(*(_pPh->Verbe.begin())) ;

  traitePrepositionCC(&_pPh->CCLieu,    &_pPh->PrepositionLieu,    NSPhraseur::prepLieu);
  traitePrepositionCC(&_pPh->CCTemps,   &_pPh->PrepositionTemps,   NSPhraseur::prepTemps);
  traitePrepositionCC(&_pPh->CCManiere, &_pPh->PrepositionManiere, NSPhraseur::prepManiere);
  traitePrepositionCC(&_pPh->CCMoyen,   &_pPh->PrepositionMoyen,   NSPhraseur::prepMoyen);
  traitePrepositionCC(&_pPh->CCCause,   &_pPh->PrepositionCause,   NSPhraseur::prepCause);
  traitePrepositionCC(&_pPh->CCBut,     &_pPh->PrepositionBut,     NSPhraseur::prepBut);
  traitePrepositionCC(&_pPh->CCType,    &_pPh->PrepositionType,    NSPhraseur::prepType);
  traitePrepositionCC(&_pPh->CCChiffre, &_pPh->PrepositionChiffre, NSPhraseur::prepChiffre);

  // Ensuite on classe.
  classeTout();

  // Adverbes du verbe. (stockes dans le complement du verbe.)
  //
  if (false == (_pPh->Verbe).empty())
  {
    if ((*(_pPh->Verbe.begin()))->getFirstComplementPhr())
    {
      NSPhraseur* pCompVerbe = (*(_pPh->Verbe.begin()))->getFirstComplementPhr() ;

      if (false == (pCompVerbe->adverbe).empty())
      {
        NsProposition Propos(pContexte, &pCompVerbe, NsProposition::notSetType, NsProposition::notSetConjonct) ;
        NSGenerateurEn GeneAdverbe(pContexte, &Propos, sLang) ;
        GeneAdverbe.classeTout() ;

        if (GeneAdverbe._pAdverbePos)
        {
          if (false == (GeneAdverbe._pAdverbePos)->empty())
          {
            for (iterMots  = (GeneAdverbe._pAdverbePos)->begin() ;
                      (GeneAdverbe._pAdverbePos)->end() != iterMots ;
                                                             iterMots++)
            {
              if (false == (*iterMots)->estTexteLibre())
                sInterPos2 = donneAdverbeComplet(*iterMots) ;
              else
                sInterPos2 = (*iterMots)->getTexteLibre() ;

              etDuMilieu(&sPhrasePos, &sInterPos1, &sInterPos2) ;
            }
            etFinal(&sPhrasePos, &sInterPos1) ;

            sPhraseAdverbe += sPhrasePos ;
          }
        }

        if (GeneAdverbe._pAdverbeNeg)
        {
          if (false == (GeneAdverbe._pAdverbeNeg)->empty())
          {
            // s'il existe des adverbes a sens positif.
            if (string("") != sPhrasePos)
            {
              for (iterMots = (GeneAdverbe._pAdverbeNeg)->begin() ;
                       (GeneAdverbe._pAdverbeNeg)->end() != iterMots ;
                          iterMots++)
              {
                if (false == (*iterMots)->estTexteLibre())
                  sInterNeg2 = donneAdverbeComplet(*iterMots) ;
                else
                  sInterNeg2 = (*iterMots)->getTexteLibre() ;

                etDuMilieu(&sPhraseNeg, &sInterNeg1, &sInterNeg2) ;
              }
              etFinal(&sPhraseNeg, &sInterNeg1) ;

              sPhraseAdverbe += string(" mais ") + sPhraseNeg ;
            }

            // s'il n'y a pas d'adverbe a sens positif, la phrase devient
            // negative.
            //
            else
            {
              bPhraseAffirmative = false ;

              // On prend l'adverbe modificateur du premier adverbe du COD negatif

              bool bPremier = true;
              sInter1 = "";
              sInter2 = "";
              for (iterMots = (GeneAdverbe._pAdverbeNeg)->begin() ;
                       (GeneAdverbe._pAdverbeNeg)->end() != iterMots ;
                                                                iterMots++)
              {
                sInterNeg2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle, sansCertitude) ;

                sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;

                if (string("") != sCertitude)
                {
                  if (bPremier)
                  {
                    bPremier = false ;
                    if (sCertitude != "")
                    {
                      if (string(sCertitude, 0 ,5) == "WCE00")
                        sAdverbeModificateur = string(" ") ;
                      else if ((string(sCertitude, 0 ,5) > "WCE00") &&
                               (string(sCertitude, 0 ,5) < "WCE50"))
                        sAdverbeModificateur = " probably ";
                    }
                  }
                  else
                  {
                    if (string("") != sCertitude)
                    {
                      if (string(sCertitude, 0 ,5) == "WCE00")
                        sInterNeg2 = string("not ") + sInter2 ;
                      else if ((string(sCertitude, 0 ,5) > "WCE00") &&
                               (string(sCertitude, 0 ,5) < "WCE50"))
                        sInterNeg2 = string("probably not ") + sInter2 ;
                    }
                  }
                }
                etDuMilieu(&sPhraseNeg, &sInterNeg1, &sInterNeg2) ;
              }
              etFinal(&sPhraseNeg, &sInterNeg1) ;

              sPhraseAdverbe = sPhraseNeg ;
            }
          }
        }
      }
    }
  }

  //
  // PREPARATION DES COMPOSANTS
  //

  //
  // Sujet
  //
  string sSujet      = string("") ;
  GENRE  iGenreSujet = genreNull ;
  string sAttSujet   = string("") ;

  if (!(prepareSujet(&sSujet, &iGenreSujet, &sAttSujet, &sAdverbeModificateur, &bPhraseAffirmative, iStyle)))
    return false ;

  // On traite le COD.
  // On met d'abord les complements positifs, puis les negatifs, avec "but not de" entre.
  // S'il n'y a que des negatifs, la phrase devient negative.
  //

  string sCOD = "";
  GENRE  iGenreCOD = genreNull ;
  string sAttCOD = "";

  if (!(prepareCOD(&sCOD, &iGenreCOD, &sAttCOD, &sAdverbeModificateur, &bPhraseAffirmative, iStyle)))
    return false ;

  //
  // Complements : quand, ou, comment, avec quoi, pourquoi, dans quel but
  //

  _pCCArray->donnePhrase(iStyle) ;

  //
  // Cas de la phrase principale
  //
  if (phrasePrincipale == _pPh->iPhraseType)
  {
    if (dcPhrase == iStyle)
    {
      //
      // Cas classique : FORME ACTIVE
      //
      if (NSPhraseur::formePassive != _pPh->iForme)
      {
        //
        //     P
        //   /  \
        //  GN  GV
        //
        //
        if (string("") != sSujet)
        {
          if (NSPhraseur::sujetNoRepeat == _pPh->iTypeSujet)
          {
            NSPhraseur::VBPERSO iPersonVb ;
            if ((genreFP == iGenreSujet) || (genreMP == iGenreSujet) || (genreNP == iGenreSujet))
              iPersonVb = NSPhraseur::pers3P ;
            else
              iPersonVb = NSPhraseur::pers3S ;
            _sPhrase = donnePronomPersonnel(iGenreSujet, iPersonVb, STR_SUJET) ;
          }
          else if (NSPhraseur::sujetRemove != _pPh->iTypeSujet)
            _sPhrase = sSujet ;

          // Cas d'une vraie phrase (iStyle == dcPhrase)
          //
          //
          //                P
          //              /  \         \
          //             GN  GV        GN prep
          //                / \
          //               V  GN (COD)
          //   ex : My daughter has driven the car on the highway.
          //
          // Il n'est pas possible d'avoir à la fois un COD et un attribut
          // du sujet (qui appartiennent tous deux au GV)

          if ((sCOD != "") && (sAttSujet != ""))
            return false ;

          // L'existence d'un COD impose celle d'un verbe
          if ((sCOD != "") && (_pPh->Verbe.empty()))
            return false ;

          // S'il existe un attribut et pas de verbe, on impose
          // le verbe être
          if ((sAttSujet != "") && (_pPh->Verbe.empty()))
          {
            NSPatPathoData PathoData ;
            PathoData.setLexique("4ETRE1") ;
            _pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;
          }

          if ((iGenreSujet == genreMP) || (iGenreSujet == genreFP) || (iGenreSujet == genreNP))
            _pPh->iVbPersonne = NSPhraseur::pers3P ;
          else
            _pPh->iVbPersonne = NSPhraseur::pers3S ;

          // On traite au passage le cas ou la phrase est negative

          string sPrincipal, sAuxilliaire;
          donneVerbe(&sPrincipal, &sAuxilliaire);
          if (sAuxilliaire != "")
          {
            if (false == bPhraseAffirmative)
            {
              sVerbe += sAuxilliaire + sAdverbeModificateur + string("not") ;

              if (sPhraseAdverbe != "")
                sVerbe += string(" ") + sPhraseAdverbe ;

              if (sPrincipal != "")
                sVerbe += string(" ") + sPrincipal ;
            }
            else
            {
              sVerbe += sAuxilliaire ;
              if (sPrincipal != "")
                sVerbe += string(" ") + sPrincipal ;

              if (sPhraseAdverbe != "")
                sVerbe += string(" ") + sPhraseAdverbe ;
            }
          }
          else
          {
            // La phrase negative en anglais comporte necessairement
            // un auxilliaire.

            if (false == bPhraseAffirmative)
              return false ;
            else
              sVerbe += sPrincipal ;

            if (sPhraseAdverbe != "")
              sVerbe += string(" ") + sPhraseAdverbe ;
          }

          _sPhrase += string(" ") + sVerbe;

          if (sCOD != "")
            _sPhrase += string(" ") + sCOD ;
          if (sAttCOD != "")
            _sPhrase += string(" ") + sAttCOD ;
          if (sAttSujet != "")
            _sPhrase += string(" ") + sAttSujet ;
        }
        else // Pas de sujet.
        {
          //
          // On s'occupe du verbe
          //
          if (false == _pPh->Verbe.empty())
          {
            // Creation d'un sujet virtuel "there" pour les verbes d'etat.
            // Pour les verbes d'action, on passe a la voix passive.
            //
            // On mange des pommes.
            // -> Apples are eaten.

            string sTransitivite;
            string sActEtat;

            if (!donneVerbeClasse(*(_pPh->Verbe.begin()), &sTransitivite, &sActEtat))
                return false;

            switch (_pPh->iVbType)
            {
              case NSPhraseur::vbTypeAction :
              {
                NsProposition Propos(pContexte, &_pPh, NsProposition::notSetType, NsProposition::notSetConjonct) ;
                NSGenerateurEn GenePassif(pContexte, &Propos, sLang) ;
                GenePassif._pPh->iForme = NSPhraseur::formePassive ;
                bool result = GenePassif.genereProposition(dcPhrase) ;
                _sPhrase = GenePassif.getPropositionPhrase() ;
                return result ;
              }
              case NSPhraseur::vbTypeEtat :
              {
                _sPhrase += string("there");

                _pPh->iVbPersonne = NSPhraseur::pers3S;

                string sPrincipal, sAuxilliaire;
                donneVerbe(&sPrincipal, &sAuxilliaire);

                // Phrase affirmative
                //
                if (bPhraseAffirmative)
                {
                  if (sAuxilliaire != "")
                    sVerbe = string(" ") + sAuxilliaire;
                  if (sPrincipal != "")
                    sVerbe += string(" ") + sPrincipal;

                  if (sPhraseAdverbe != "")
                    sVerbe += string(" ") + sPhraseAdverbe;
                }

                // Phrase negative
                //
                else
                {
                  if (sAuxilliaire != "")
                  {
                    sVerbe += string(" ") + sAuxilliaire + sAdverbeModificateur + string("not") ;

                    if (sPhraseAdverbe != "")
                      sVerbe += string(" ") + sPhraseAdverbe ;

                    if (sPrincipal != "")
                      sVerbe += string(" ") + sPrincipal ;
                  }

                  // La phrase anglaise negative comporte toujours un
                  // auxilliaire.
                  else
                    return false ;
                }
                _sPhrase += sVerbe ;
                break ;
              }
            }
          }
          //
          // Ni verbe, ni sujet -> phrase sur le mode "There is"
          //
          else if (string("") != sCOD)
          {
            NSPatPathoData PathoData ;
            PathoData.setLexique("4ETRE1") ;
            _pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

            _sPhrase += string("there") ;
            _pPh->iVbPersonne = NSPhraseur::pers3S ;

            string sPrincipal, sAuxilliaire ;
            donneVerbe(&sPrincipal, &sAuxilliaire) ;
            if (bPhraseAffirmative)
            {
              if (sAuxilliaire != "")
                sVerbe = string(" ") + sAuxilliaire ;
              if (sPrincipal != "")
                sVerbe += string(" ") + sPrincipal ;
            }

            // Phrase negative
            //
            else
            {
              if (sAuxilliaire != "")
              {
                sVerbe += string(" ") +  sAuxilliaire + sAdverbeModificateur + string("not") ;
                if (sPrincipal != "")
                  sVerbe += string(" ") + sPrincipal ;
              }
              else
                return false ;
            }
            _sPhrase += sVerbe ;
            if (sCOD != "")
              _sPhrase += string(" ") + sCOD ;
            if (sAttCOD != "")
              _sPhrase += string(" ") + sAttCOD ;
          }
        }
      }
      //
      // FORME PASSIVE
      //
      // Le sujet et le COD inversent leur rôle ; le temps est donne par
      // le verbe etre, le verbe est au participe passe
      //
      // Ex The cat eats the mouse. -> The mouse is eaten by the cat.
      //
      else
      {
        // Il faut forcement un COD, sinon on se retrouve sans sujet
        if (sCOD == "")
          return false;
        // Il faut forcement un verbe
        if (_pPh->Verbe.empty())
          return false;

        if (_pPh->iTypeSujet == NSPhraseur::sujetNoRepeat)
        {
          NSPhraseur::VBPERSO iPersonVb ;
          if ((genreFP == iGenreCOD) || (genreMP == iGenreCOD) || (genreNP == iGenreCOD))
            iPersonVb = NSPhraseur::pers3P ;
          else
            iPersonVb = NSPhraseur::pers3S ;
          _sPhrase = donnePronomPersonnel(iGenreSujet, iPersonVb, STR_SUJET) ;
        }
        else if (NSPhraseur::sujetRemove != _pPh->iTypeSujet)
          _sPhrase = sCOD ;

        //
        // On prend le verbe au participe passe, c'est le verbe etre
        // qui est conjugue
        //
        string sVb = donneParticipePasse(*(_pPh->Verbe.begin()), iGenreCOD);

        // On ne peut pas faire de phrase passive avec des verbes qui
        // n'ont pas de participe passe comme "to can".

        if (string("") == sVb)
          return false ;

        _pPh->Verbe.vider() ;
        NSPatPathoData PathoData ;
        PathoData.setLexique("4ETRE1") ;
        _pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;

        if ((iGenreSujet == genreMP) || (iGenreSujet == genreFP) || (iGenreSujet == genreNP))
          _pPh->iVbPersonne = NSPhraseur::pers3P ;
        else
          _pPh->iVbPersonne = NSPhraseur::pers3S ;

        string sPrincipal, sAuxilliaire ;
        donneVerbe(&sPrincipal, &sAuxilliaire) ;

        // Phrase affirmative
        //
        if (bPhraseAffirmative)
        {
            if (sAuxilliaire != "")
                sVerbe = string(" ") + sAuxilliaire;
            if (sPrincipal != "")
                sVerbe += string(" ") + sPrincipal;
        }

        // Phrase negative
        //
        else
        {
          if (sAuxilliaire != "")
          {
            sVerbe += string(" ") + sAuxilliaire + sAdverbeModificateur + string("not");
            if (sPrincipal != "")
              sVerbe += string(" ") + sPrincipal ;
          }
          else
            return false ;
        }

        if (bPhraseAffirmative)
        {
          _sPhrase += sVerbe + string(" ") + sVb ;

          if (sPhraseAdverbe != "")
            _sPhrase += string(" ") + sPhraseAdverbe ;
        }
        else
        {
          _sPhrase += sVerbe ;

          if (sPhraseAdverbe != "")
            _sPhrase += string(" ") + sPhraseAdverbe ;

          _sPhrase += string(" ") + sVb ;
        }

        // I've found your propositions interesting.
        // -> Your propositions have been found interesting.
        //
        if (sAttCOD != "")
          _sPhrase += string(" ") + sAttCOD ;

        //
        // S'il y a un sujet, on ajoute "by" devant.
        // Il peut ne pas y avoir de sujet, par exemple
        // "The car has been stolen by someone."
        //
        if (sSujet != "")
          _sPhrase += string(" by ") + sSujet;
      }
    }

    else if (dcTiret == iStyle)
    {
      // C'est comme la forme passive, sauf qu'on enleve le verbe etre, et
      // qu'on enleve les articles du COD.

      // Deux cas de figure :
      // - "vraie phrase" et il faut un COD
      // - enumeration de valeurs chiffrees

      // Il faut forcement un COD, ou des valeurs chiffrees
      if ((sCOD == "") && (_pPh->CCChiffre.empty()))
        return false;

      // S'il y a un COD mais pas de verbe, c'est juste une enumeration.
      if (sCOD != "")
        _sPhrase = sCOD;

      if (false == _pPh->Verbe.empty())
      {
        // Il faut forcement un COD, sinon on se retrouve sans sujet
        if (sCOD == "")
          return false;

        //
        // On prend le verbe au participe passe
        //
        string sVb = donneParticipePasse(*(_pPh->Verbe.begin()), iGenreCOD) ;

        if (sVb == "")
          return false ;

        // Si la phrase est negative, on ajoute "not", ou "absolutely not" etc...
        // juste avant le participe passe.

        if (false == bPhraseAffirmative)
        {
          _sPhrase += string(" ") + sAdverbeModificateur + string("not ");

          if (sPhraseAdverbe != "")
            _sPhrase += sPhraseAdverbe += string(" ");

          _sPhrase += sVb;
        }
        else
        {
          _sPhrase += string(" ") + sVb ;

          if (sPhraseAdverbe != "")
            _sPhrase += string(" ") + sPhraseAdverbe ;
        }

        // Propostions found interesting by Jean.
        //
        if (sAttCOD != "")
          _sPhrase += string(" ") + sAttCOD ;

        //
        // S'il y a un sujet, on ajoute "par" devant.
        // Il peut ne pas y avoir de sujet, par exemple
        // "La vesicule est etudiee en decubitus dorsal"
        //
        if (sSujet != "")
          _sPhrase += string(" by ") + sSujet ;
      }
    }
  }
  //
  // Complements
  //
  // Penser a pouvoir forcer l'ordre (importance) des Complements

  if (false == _pCCArray->empty())
  {
    for (GenCompIter itCompl = _pCCArray->begin(); itCompl != _pCCArray->end(); itCompl++)
    {
      if ((*itCompl)->getPositif() != string(""))
      {
        _sPhrase += " " + (*itCompl)->getPositif() ;
        if ((*itCompl)->getNegatif() != string(""))
          _sPhrase += string(" but not ") + (*itCompl)->getNegatif() ;
      }
      else if ((*itCompl)->getNegatif() != string(""))
        _sPhrase += string(" not ") + (*itCompl)->getNegatif() ;
    }
  }

  //
  // Cas de la subordonnee relative.
  //
  if (_pPh->iPhraseType == phraseRelative)
  {
    if (dcPhrase == iStyle)
    {
      //
      // Cas classique : FORME ACTIVE
      //
      if (_pPh->iForme != NSPhraseur::formePassive)
      {
        //
        //     P
        //   /  \
        //  GN  GV
        //
        //
        if (sSujet != "")
        {
          // Le pronom qui introduit la subordonnee relative depend du genre
          // du sujet. Si celui-ci est un homme (ou une femme), la proposition
          // est introduite par "who". Si c'est un objet (genre neutre), elle
          // est introduite par "which".

          if ((iGenreSujet == genreMS) || (iGenreSujet == genreMP) ||
              (iGenreSujet == genreFS) || (iGenreSujet == genreFP))
            _sPhrase = string(", who") ;

          if ((iGenreSujet == genreNS) || (iGenreSujet == genreNP))
            _sPhrase = string(", which") ;

          // Cas d'une vraie phrase (iStyle == dcPhrase)
          //
          //
          //                P
          //              /  \         \
          //             GN  GV        GN prep
          //                / \
          //               V  GN (COD)
          //   ex : My daughter who has driven the car on the highway.
          //
          // Il n'est pas possible d'avoir à la fois un COD et un attribut
          // du sujet (qui appartiennent tous deux au GV)

          if ((sCOD != "") && (sAttSujet != ""))
              return false;

          // L'existence d'un COD impose celle d'un verbe
          if ((sCOD != "") && (_pPh->Verbe.empty()))
              return false;

          // S'il existe un attribut et pas de verbe, on impose
          // le verbe être
          if ((sAttSujet != "") && (_pPh->Verbe.empty()))
          {
            NSPatPathoData PathoData ;
            PathoData.setLexique("4ETRE1") ;
            _pPh->Verbe.push_back(new NSPhraseMot(&PathoData, pContexte)) ;
          }

          if ((iGenreSujet == genreMP) || (iGenreSujet == genreFP)
              || (iGenreSujet == genreNP))
            _pPh->iVbPersonne = NSPhraseur::pers3P;
          else
            _pPh->iVbPersonne = NSPhraseur::pers3S;

          // On traite au passage le cas ou la phrase est negative

          string sPrincipal, sAuxilliaire;
          donneVerbe(&sPrincipal, &sAuxilliaire);
          if (sAuxilliaire != "")
          {
            if (false == bPhraseAffirmative)
            {
              sVerbe += sAuxilliaire + sAdverbeModificateur + string("not") ;

              if (sPhraseAdverbe != "")
                sVerbe += string(" ") + sPhraseAdverbe ;

              if (sPrincipal != "")
                sVerbe += string(" ") + sPrincipal ;
            }

            else
            {
              sVerbe += sAuxilliaire;
              if (sPrincipal != "")
                  sVerbe += string(" ") + sPrincipal;

              if (sPhraseAdverbe != "")
                  sVerbe += string(" ") + sPhraseAdverbe;
            }
          }

          else
          {
            // La phrase negative en anglais comporte necessairement
            // un auxilliaire.

            if (false == bPhraseAffirmative)
                return false ;
            else
                sVerbe += sPrincipal ;

            if (string("") != sPhraseAdverbe)
                sVerbe += string(" ") + sPhraseAdverbe ;
          }

          _sPhrase += string(" ") + sVerbe ;

          if (sCOD != "")
            _sPhrase += string(" ") + sCOD ;
          if (sAttCOD != "")
            _sPhrase += string(" ") + sAttCOD ;
          if (sAttSujet != "")
            _sPhrase += string(" ") + sAttSujet ;
        }

        else // Pas de sujet.
        {
          // Impossible dans le cadre d'une subordonnee relative.
          return false ;
        }
      }
      //
      // FORME PASSIVE
      //
      // On ne la traite pas encore
      else
      {
        return false ;
      }
    }
    else if (dcTiret == iStyle)
    {
      return false ;
    }
  }

  // A la fin, on rajoute la phrase qui se place apres les " : ".
  //
  if (_pPh->pDeuxPoints)
  {
    NsProposition Propos(pContexte, &(_pPh->pDeuxPoints), NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneDeuxPoints(pContexte, &Propos, sLang) ;
    GeneDeuxPoints.genereProposition(iStyle) ;
    _sPhrase += string(": ") + GeneDeuxPoints.getPropositionPhrase() ;
  }

  // Elimination des eventuels blancs initiaux
  strip(_sPhrase, stripLeft);
  //while ((strlen(sPhrase.c_str()) > 0) && (sPhrase[0] == ' '))
  //	sPhrase = string(sPhrase, 1, strlen(sPhrase.c_str())-1);

  _sPhrase = postTraitement(&_sPhrase);

  return true ;
}

bool
NSGenerateurEn::assembleProposition(DCODETYPE /* iStyle */, NsProposition* pPropos)
{
  NsProposition* pCurrentProp ;
  if (pPropos)
    pCurrentProp = pPropos ;
  else
    pCurrentProp = _pProposition ;

  NSPropositionArray* pPropArray = (static_cast<NSPropositionArray*>(pCurrentProp->pProposition)) ;
  if (pPropArray->empty())
    return true ;

  iterProposition i = pPropArray->begin() ;
  pCurrentProp->setPhrase((*i)->getPhrase()) ;

  while (pPropArray->end() != i)
  {
    NsProposition* pItProp = *i ;
    i++ ;

    if (string("") != (*i)->getPhrase())
    {
      switch ((*i)->iPropositionType)
      {
        case NsProposition::principale :
          if (i != pPropArray->end())
            pCurrentProp->addToPhrase(string(", ") + (*i)->getPhrase()) ;
          else
            pCurrentProp->addToPhrase(string(" and ") + (*i)->getPhrase()) ;
          break ;
        //
        // Completives
        //
        case NsProposition::completiveQue :   // SCQ : Je veux que tu reviennes
            pCurrentProp->addToPhrase(string(" that ") + (*i)->getPhrase()) ;
            break ;
        case NsProposition::completiveInfinitive : // SCI : J'entends les chats miauler
            pCurrentProp->addToPhrase((*i)->getPhrase()) ;
            break ;
        case NsProposition::completiveInterrogative :  // SCN : Je voudrais savoir pourquoi tu ne veux pas
            pCurrentProp->addToPhrase(string(" why ") + (*i)->getPhrase()) ;
            break ;
        //
        // Circonstancielles
        //
        // - Temporelle
        case NsProposition::circonstancielleTemporelleAnterieure :   // SCTA : Je le verrai avant qu'il chante
          switch ((*i)->iConjonctionType)
          {
            case NsProposition::SCTA_AvantQue :        // ---X---|
              pCurrentProp->addToPhrase(string(" before ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCTA_JusteAvantQue :   // ------X|
              pCurrentProp->addToPhrase(string(" just before ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCTA_JusquAceQue :     // --XXXXX|
              pCurrentProp->addToPhrase(string(" until ") + (*i)->getPhrase()) ;
              break ;
            default :
              pCurrentProp->addToPhrase(string(" before ") + (*i)->getPhrase()) ;
          }
          break ;
        case NsProposition::circonstancielleTemporelleSimultanee :   // SCTS : Je chantais au moment où il arriva
          pCurrentProp->addToPhrase(string(" at the moment when ") + (*i)->getPhrase()) ;
          break ;
        case NsProposition::circonstancielleTemporellePosterieure :  // SCTP : Je chanterai après que vous serez parti
          switch ((*i)->iConjonctionType)
          {
            case NsProposition::SCTP_ApresQue :          // ---|---X
              pCurrentProp->addToPhrase(string(" after ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCTP_DesQue :            // ---|X---
              pCurrentProp->addToPhrase(string(" as soons as ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCTP_DepuisQue :         // ---|XXXX
              pCurrentProp->addToPhrase(string(" since ") + (*i)->getPhrase()) ;
              break ;
            default :
              pCurrentProp->addToPhrase(string(" before ") + (*i)->getPhrase()) ;
          }
          break ;
        // - Consecutive -> resultat de l'action
        case NsProposition::circonstancielleConsecutive :            // SCR : Il fait trop froid pour que je chante
          pCurrentProp->addToPhrase(string(" for ") + (*i)->getPhrase()) ;
          break ;
        // - Causale -> cause de l'action
        case NsProposition::circonstancielleCausale :                // SCC : Il a reussi parce qu'il chantait bien
          pCurrentProp->addToPhrase(string(" because ") + (*i)->getPhrase()) ;
          break ;
        // - Concessive -> relation inattendue
        case NsProposition::circonstancielleConcessive :             // SCS : Il a echoue bien qu'il chanta bien
            pCurrentProp->addToPhrase(string(" even if ") + (*i)->getPhrase()) ;
            break ;
        // - Finale -> but de l'action
        case NsProposition::circonstancielleFinale :                 // SCF : Il chante fort pour qu'on l'entende bien
            pCurrentProp->addToPhrase(string(" in order to ") + (*i)->getPhrase()) ;
            break ;
        // - Comparative -> comparaison
        case NsProposition::circonstancielleComparativeProportion :  // SCCP : Vous chanterez d'autant plus fort que vous serez bien echauffe
          switch ((*i)->iConjonctionType)
          {
            case NsProposition::SCCP_AutantQuantite :    // suivant, selon, à mesure
              pCurrentProp->addToPhrase(string(" as much as ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCCP_Synchrone :         // au fur et à mesure
              pCurrentProp->addToPhrase(string(" at the same time ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCCP_PlusQuantite :      // d'autant plus
              pCurrentProp->addToPhrase(string(" as much as ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCCP_PlusVite :          // d'autant plus vite
              pCurrentProp->addToPhrase(string(" the faster as ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCCP_MoinsQuantite :      // d'autant moins
              pCurrentProp->addToPhrase(string(" as less as ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCCP_MoinsVite :          // d'autant moins vite
              pCurrentProp->addToPhrase(string(" the slower as ") + (*i)->getPhrase()) ;
              break ;
          }
          break ;
        case NsProposition::circonstancielleComparativeEgalite :     // SCCE : Vous chantez aussi fort que moi
          switch ((*i)->iConjonctionType)
          {
            case NsProposition::SCCE_Aspect :            // tel que, de même que, si ... que
              pCurrentProp->addToPhrase(string(" as ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCCE_Quantite :          // autant que, aussi que
              pCurrentProp->addToPhrase(string(" as much as ") + (*i)->getPhrase()) ;
              break ;
            case NsProposition::SCCE_Temps :             // tant que
              pCurrentProp->addToPhrase(string(" until ") + (*i)->getPhrase()) ;
              break ;
          }
          break ;
        case NsProposition::circonstancielleComparativeRessemblance : // SCCR : Tu me regardes comme me regardait ta mère
          pCurrentProp->addToPhrase(string(" in the same way as ") + (*i)->getPhrase()) ;
          break ;
        // - Conditionnelle -> condition
        case NsProposition::circonstancielleConditionnelleHypothese : // SCHH : il mange s'il a faim, il mangera s'il a faim
          pCurrentProp->addToPhrase(string(" if ") + (*i)->getPhrase()) ;
          break ;
        case NsProposition::circonstancielleConditionnelleImaginee :  // SCHH : il mangerait s'il avait faim
          pCurrentProp->addToPhrase(string(" if ") + (*i)->getPhrase()) ;
          break ;
        default :
          pCurrentProp->addToPhrase(string(", ") + (*i)->getPhrase()) ;
      }
    }
  }
  return true ;
}

bool
NSGenerateurEn::prepareSujet(string* psSujet, GENRE* piGenreSujet, string* psAttSujet, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle)
{
  bool     trouve;
  string   sLexique;
  string   sPluriel;
  string   sCertitude;
  string   sInter1 = "";
  string   sInter2 = "";
  GENRE    iGenre;

  string   sAdverbeModificateurAtt = " ";

  iterPhraseMot  iterMots;
  NSPathologData Data;

  if (false == _pPh->Sujet.empty())
  {
      sInter1 = "";
      sInter2 = "";
      for (iterMots = _pPh->Sujet.begin(); iterMots != _pPh->Sujet.end(); iterMots++)
      {
          sLexique = (*iterMots)->getLexique() ;

          if (false == (*iterMots)->estTexteLibre())
          {
              trouve = getPathologData(sLexique, Data) ;
              sPluriel = (*iterMots)->getPluriel() ;
              if (!trouve)
                  return false;
              if (!(Data.estNom()))
                  return false;
              Data.donneGenre(&iGenre);

              // Recuperation du genre (composition du genre recupere avec
              // le genre dejà existant)

              if (sPluriel != "")
                  Data.donneGenrePluriel(&iGenre);

              string sForceNbr = (*iterMots)->forceNombre(this);

              if (sForceNbr == FORCE_PLUR)
                  Data.donneGenrePluriel(&iGenre);
              else if (sForceNbr == FORCE_SING)
                  Data.donneGenreSingulier(&iGenre);
          }
          else
              iGenre = genreMS;

          if (genreNull == *piGenreSujet)
              *piGenreSujet = iGenre;
          else
          {
              // En anglais, le genre n'a aucune importance au pluriel.
              // On met donc par defaut le genre MP au pluriel.
              // En fait, on compose quand meme les genres, de maniere a ce que
              // l'on puisse choisir le bon pronom pour introduire les subordonnees
              // relatives.

              if (((*piGenreSujet == genreNS) || (*piGenreSujet == genreNP)) &&
                   (Data.estNeutre()))
                  *piGenreSujet = genreNP;

              else if (((*piGenreSujet == genreFS) || (*piGenreSujet == genreFP) ||
                      (*piGenreSujet == genreNS) || (*piGenreSujet == genreNP))
                     && (Data.estFeminin()))
                  *piGenreSujet = genreFP;
              else
                  *piGenreSujet = genreMP;
          }

          // L'article depend du statut d'objet reel ou abstrait

          if (((sPluriel == "") && (Data.estReel())) || (string(sPluriel, 0, 5) == "WPLUS"))
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle) ;
          else if (((sPluriel == "") && (!Data.estReel())) || (string(sPluriel, 0, 5) == "WPLUR"))
              sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle) ;

          etDuMilieu(psSujet, &sInter1, &sInter2);
      }
      etFinal(psSujet, &sInter1);
  }
  // L'attribut du sujet est un adjectif qualificatif qui s'accorde avec le sujet
  //
  // ex : La valve est dilatee
  //
  // L'attribut doit être un adjectif
  //
  // On met d'abord les attributs positifs, puis les negatifs, avec "but not" entre.
  // S'il n'y a que des negatifs, la phrase devient negative.
  //

  string sAttSujet = "";

  string sAttSujetPos = "";
  if (_pAttSujetPos)
  {
      if (false == _pAttSujetPos->empty())
      {
          if (_pPh->Sujet.empty())
              return false;
          sInter1 = "";
          sInter2 = "";
          for (iterMots = _pAttSujetPos->begin(); iterMots != _pAttSujetPos->end(); iterMots++)
          {
              if (!((*iterMots)->estTexteLibre()))
              {
                  // On impose le genre du sujet
                  sInter2 = donneAdjectifComplet(*iterMots, *piGenreSujet);
              }
              else
                  sInter2 = (*iterMots)->getTexteLibre() ;

              etDuMilieu(&sAttSujetPos, &sInter1, &sInter2);
          }
          etFinal(&sAttSujetPos, &sInter1);
      }
  }

  string sAttSujetNeg = "";
  if (_pAttSujetNeg)
  {
      if (!(_pAttSujetNeg->empty()))
      {
          if (_pPh->Sujet.empty())
              return false;
          sInter1 = "";
          sInter2 = "";
          bool bPremier = true;

          for (iterMots = _pAttSujetNeg->begin(); iterMots != _pAttSujetNeg->end(); iterMots++)
          {
              // On impose le genre du sujet
              sInter2 = donneAdjectifComplet(*iterMots, genreNull, sansCertitude);

              sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;

              if (bPremier)
              {
                  bPremier = false;
                  if (sCertitude != "")
                  {
                      if (sCertitude == "WCE00")
                          sAdverbeModificateurAtt = " ";
                      else if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
                          sAdverbeModificateurAtt = " probably ";
                  }
              }
              else
              {
                  if (sCertitude != "")
                  {
                      if (sCertitude == "WCE00")
                          sInter2 = string("not ") + sInter2;
                      else if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
                          sInter2 = string("probably not ") + sInter2;
                  }
              }

              etDuMilieu(&sAttSujetNeg, &sInter1, &sInter2);
          }
          etFinal(&sAttSujetNeg, &sInter1);
      }
  }

  if (sAttSujetPos == "")
  {
      if (sAttSujetNeg != "")
      {
          if (*pbPhraseAffirmative)
          {
              *pbPhraseAffirmative = false ;
              *psAttSujet = sAttSujetNeg;
              *psAdverbeModificateur = sAdverbeModificateurAtt;
          }
          else
          {
              // il y a deja un adverbe du verbe qui rend la phrase negative.
              *psAttSujet = sAdverbeModificateurAtt + string("but ") + sAttSujetNeg;
              // on enlève les blancs initaux.
              strip(*psAttSujet, stripLeft);
          }
      }
  }
  else
  {
      *psAttSujet = sAttSujetPos;
      if (sAttSujetNeg != "")
          *psAttSujet += string(" but") + sAdverbeModificateurAtt + string("not ") + sAttSujetNeg ;
  }

  return true ;
}

bool
NSGenerateurEn::prepareCOD(string* psCOD, GENRE* piGenreCOD, string* psAttCOD, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle)
{
  bool     trouve ;
  string   sLexique ;
  string   sPluriel ;
  string   sCertitude ;
  string   sInter1 = "" ;
  string   sInter2 = "" ;
  GENRE    iGenre ;

  iterPhraseMot  iterMots ;
  NSPathologData Data ;

  *psCOD      = "" ;
  *psAttCOD   = "" ;
  *piGenreCOD = genreNull ;

    //
    // On met d'abord les complements positifs, puis les negatifs, avec "but not de" entre.
    // S'il n'y a que des negatifs, la phrase devient negative.
    //

    // D'abord on va recuperer le genre du COD

    if (false == _pPh->COD.empty())
    {
        for (iterMots = _pPh->COD.begin(); iterMots != _pPh->COD.end(); iterMots++)
        {
            sLexique = (*iterMots)->getLexique() ;

            if (false == (*iterMots)->estTexteLibre())
            {
                sPluriel = (*iterMots)->getPluriel() ;
                trouve = getPathologData(sLexique, Data) ;
                if (!trouve)
                    return false ;
                if (false == Data.estNom())
                    return false;
                // Recuperation du genre (composition du genre recupere avec
                // le genre deja existant)
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);

                string sForceNbr = (*iterMots)->forceNombre(this);
                if (sForceNbr == FORCE_PLUR)
                    Data.donneGenrePluriel(&iGenre);
                else if (sForceNbr == FORCE_SING)
                    Data.donneGenreSingulier(&iGenre);
            }
            else
                iGenre = genreMS;

            if (genreNull == *piGenreCOD)
                *piGenreCOD = iGenre;
            else
            {
                if (((*piGenreCOD == genreNS) || (*piGenreCOD == genreNP)) &&
                     (Data.estNeutre()))
                    *piGenreCOD = genreNP;

                else if (((*piGenreCOD == genreFS) || (*piGenreCOD == genreFP) ||
                	   	  (*piGenreCOD == genreNS) || (*piGenreCOD == genreNP))
                     	 && (Data.estFeminin()))
                    *piGenreCOD = genreFP;
                else
                    *piGenreCOD = genreMP;
            }
        }
    }

    // On traite ensuite successivement les non du COD a certitude positive, puis ceux
    // a certitude negative.
    // Pour une dcTiret, il ne faut pas mettre d'article dans le COD.

    string sCODPos = "";

    if (_pCODPos)
    {
        if (!(_pCODPos->empty()))
        {
            sInter1 = "";
            sInter2 = "";
            for (iterMots = _pCODPos->begin(); iterMots != _pCODPos->end(); iterMots++)
            {
                if (dcPhrase == iStyle)
                {
                    if (!((*iterMots)->estTexteLibre()))
                    {
                        if (sPluriel == "")
                            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle) ;

                        else if (string(sPluriel, 0, 5) == "WPLUR")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle) ;
                        else if (string(sPluriel, 0, 5) == "WPLUS")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle) ;
                    }
                    else
                        sInter2 = (*iterMots)->getTexteLibre() ;
                }

                else if (dcTiret == iStyle)
                {
                    if (!((*iterMots)->estTexteLibre()))
                    {
                        sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle);
                    }
                    else
                        sInter2 = (*iterMots)->getTexteLibre() ;
                }

                etDuMilieu(&sCODPos, &sInter1, &sInter2);
            }
            etFinal(&sCODPos, &sInter1);
            *psCOD += sCODPos ;
        }
    }

    string sCODNeg = "";

    if (_pCODNeg)
    {
        if (!(_pCODNeg->empty()))
        {
            if ((sCODPos != "") || (!(*pbPhraseAffirmative)))
            {
                sInter1 = "";
                sInter2 = "";
                for (iterMots = _pCODNeg->begin(); iterMots != _pCODNeg->end(); iterMots++)
                {
                    if (dcPhrase == iStyle)
                    {
                        if (sPluriel == "")
                            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle) ;

                        else if (string(sPluriel, 0, 5) == "WPLUR")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle) ;
                        else if (string(sPluriel, 0, 5) == "WPLUS")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle) ;
                    }

                    else if (dcTiret == iStyle)
                        sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle) ;

                    etDuMilieu(&sCODNeg, &sInter1, &sInter2);
                }
                etFinal(&sCODNeg, &sInter1);
                *psCOD += string(" but ") + sCODNeg;
            }

            // Pas de COD a sens affirmatif
            else
            {
                *pbPhraseAffirmative = false;

                // On prend l'adverbe modificateur du premier nom du COD negatif

                bool bPremier = true;
                sInter1 = "";
                sInter2 = "";
                for (iterMots = _pCODNeg->begin(); iterMots != _pCODNeg->end(); iterMots++)
                {
                    if (dcPhrase == iStyle)
                    {
                        if (sPluriel == "")
                            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle, sansCertitude);

                        else if (string(sPluriel, 0, 5) == "WPLUR")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle, sansCertitude);
                        else if (string(sPluriel, 0, 5) == "WPLUS")
                            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle, sansCertitude);

                        sCertitude = string((*iterMots)->getCertitude(), 0, 5);

                        if (sCertitude != "")
                        {
                            if (bPremier)
                            {
                                bPremier = false;
                                if (sCertitude != "")
                                {
                                    if (string(sCertitude, 0 ,5) == "WCE00")
                                        *psAdverbeModificateur = " ";
                                    else if ((string(sCertitude, 0 ,5) > "WCE00") && (string(sCertitude, 0 ,5) < "WCE50"))
                                        *psAdverbeModificateur = " probably ";
                                }
                            }
                            else
                            {
                                if (sCertitude != "")
                                {
                                    if (string(sCertitude, 0 ,5) == "WCE00")
                                        sInter2 = string("not ") + sInter2;
                                    else if ((string(sCertitude, 0 ,5) > "WCE00") && (string(sCertitude, 0 ,5) < "WCE50"))
                                        sInter2 = string("probably not ") + sInter2;
                                }
                            }
                        }
                    }

                    etDuMilieu(&sCODNeg, &sInter1, &sInter2);
                }
                etFinal(&sCODNeg, &sInter1);
                *psCOD = sCODNeg;
            }
        }
    }

    // Attribut du COD (se place apres les adjectifs qualificatifs qualifiant le COD).

    string sAttCODPos = "";

    if (_pAttCODPos)
    {
        if (!(_pAttCODPos->empty()))
        {
            if (_pPh->COD.empty())
                return false;
            sInter1 = "";
            sInter2 = "";
            for (iterMots = _pAttCODPos->begin(); iterMots != _pAttCODPos->end(); iterMots++)
            {
                 if (!((*iterMots)->estTexteLibre()))
                {
                    // On impose le genre du COD
                    sInter2 = donneAdjectifComplet(*iterMots, *piGenreCOD);
                }
                else
                    sInter2 = (*iterMots)->getTexteLibre() ;

                etDuMilieu(&sAttCODPos, &sInter1, &sInter2);
            }
            etFinal(&sAttCODPos, &sInter1);
        }
    }

    string sAttCODNeg = "";
    if (_pAttCODNeg)
    {
        if (!(_pAttCODNeg->empty()))
        {
            if (_pPh->COD.empty())
                return false;
            sInter1 = "";
            sInter2 = "";
            for (iterMots = _pAttCODNeg->begin(); iterMots != _pAttCODNeg->end(); iterMots++)
            {
                // On impose le genre du COD
                sInter2 = donneAdjectifComplet(*iterMots);

                etDuMilieu(&sAttCODNeg, &sInter1, &sInter2);
            }
            etFinal(&sAttCODNeg, &sInter1);
        }
    }

    if (sAttCODPos == "")
    {
        if (sAttCODNeg != "")
        {
            *psAttCOD = sAttCODNeg;
        }
    }
    else
    {
        *psAttCOD = sAttCODPos;
        if (sAttCODNeg != "")
            *psAttCOD += string(" but ") + sAttCODNeg ;
    }
    return true;
}

string
NSGenerateurEn::donneNomComplet(NSPhraseMot* pMot, NSPhraseMot::TYPEARTICLE iArticle, DCODETYPE iStyle, GERECERTITUDE iGereCertitude)
{
  string sLibel = "" ;

  NSPathologData Data ;

  iterPhraseMot iterMots ;

  string sCertitude;

  string sLexique = pMot->getLexique() ;
  string sPluriel = pMot->getPluriel() ;

  bool bPluriel = false ;
  string sPrep = "" ;

  bool bFound = getPathologData(sLexique, Data) ;
  if (false == bFound)
    return string("") ;

  // Recuperation du genre (composition du genre recupere avec
  // le genre deja existant)
  //
  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (string("") != sPluriel)
    Data.donneGenrePluriel(&iGenre) ;

  // On regarde si le nombre est force.

  string sForceNbr = pMot->forceNombre(this);

  if (sForceNbr == FORCE_PLUR)
    Data.donneGenrePluriel(&iGenre);
  else if (sForceNbr == FORCE_SING)
    Data.donneGenreSingulier(&iGenre);

  //
  donneLibelleAffiche(&sLibel, &Data, iGenre);

  NSPhraseur* pCplmt = pMot->getFirstComplementPhr() ;
  if (pCplmt)
  {
    NsProposition Propos(pContexte, pCplmt, NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneCpl(pContexte, &Propos, sLang) ;
    traitePostposition(pMot) ;

    // Le complement du nom est necessairement introduit par une preposition.
    // Donc si on n'a pas de preposition, on en met une par defaut : "of".
    if (false == pCplmt->compNom.empty())
    {
      for (iterMots = pCplmt->compNom.begin();
           iterMots != pCplmt->compNom.end();
           iterMots++)
      {
        if (false == (*iterMots)->estTexteLibre())
        {
          if ((*iterMots)->getPreposition() == "")
              (*iterMots)->setPreposition("of") ;
        }
        else
        {
          if ((*iterMots)->getPreposition() == "")
              (*iterMots)->setTexteLibre(string("of ") + (*iterMots)->getTexteLibre()) ;
        }
      }
    }

    if (GeneCpl.genereNomComplet(pMot, iArticle, iStyle, iGereCertitude))
      sLibel = GeneCpl.getTempoPhrase() ;
  }
  else
  {
    if (iGereCertitude == avecCertitude)
    {
      sCertitude = string(pMot->getCertitude(), 0, 5) ;
      string sCertitudeEcrit = donneCertitude(sCertitude) ;

      if (sCertitudeEcrit != "")
        sLibel = sCertitudeEcrit + string(" ") + sLibel ;
    }

    if ((iGenre == genreMP) || (iGenre == genreFP) || (iGenre == genreNP))
      bPluriel = true ;

    if      (iArticle == NSPhraseMot::articleDefini)
      sLibel = donneArticleDefini(pMot, bPluriel) + sLibel;
    else if (iArticle == NSPhraseMot::articleIndefini)
      sLibel = donneArticleIndefini(pMot, bPluriel) + sLibel;
    else if (iArticle == NSPhraseMot::articlePartitif)
      sLibel = donneArticlePartitif(pMot, bPluriel) + sLibel;

    // la preposition
    sPrep = Data.chercheGrammaire(PREPOSITION) ;

    if (sPrep != "")
    {
      if (pMot->getPreposition() != "")
        pMot->setPreposition(pMot->getPreposition() + string(" ")) ;
      pMot->setPreposition(pMot->getPreposition() + string(sPrep, 1, strlen(sPrep.c_str()) - 1)) ;
    }
    else if (pMot->getPreposition() != "")
      sLibel = pMot->getPreposition() + string(" ") + sLibel ;
  }

  return sLibel ;
}

bool
NSGenerateurEn::genereNomComplet(NSPhraseMot* pMot, NSPhraseMot::TYPEARTICLE iArticle, DCODETYPE iStyle, GERECERTITUDE iGereCertitude)
{
  _sPhrase = string("") ;

  reinitialise() ;

  NSPhraseur* pComp = pMot->getFirstComplementPhr() ;
  string   sInter1 = "";
  string   sInter2 = "";
  GENRE    iGenreNom;
  string   sCertitude = "";
  string   sCertitudeNom = "";

  NSPathologData Data ;
  NSPathologData* pPremierAdjData = 0 ;

  if (false == (pComp->adjEpithete).empty())
    pComp->classeAdjectif(this);

  traitePrepositionCC(&_pPh->CCLieu,    &_pPh->PrepositionLieu,    NSPhraseur::prepLieu) ;
  traitePrepositionCC(&_pPh->CCTemps,   &_pPh->PrepositionTemps,   NSPhraseur::prepTemps) ;
  traitePrepositionCC(&_pPh->CCManiere, &_pPh->PrepositionManiere, NSPhraseur::prepManiere) ;
  traitePrepositionCC(&_pPh->CCMoyen,   &_pPh->PrepositionMoyen,   NSPhraseur::prepMoyen) ;
  traitePrepositionCC(&_pPh->CCCause,   &_pPh->PrepositionCause,   NSPhraseur::prepCause) ;
  traitePrepositionCC(&_pPh->CCBut,     &_pPh->PrepositionBut,     NSPhraseur::prepBut) ;
  traitePrepositionCC(&_pPh->CCType,    &_pPh->PrepositionType,    NSPhraseur::prepType) ;
  traitePrepositionCC(&_pPh->CCChiffre, &_pPh->PrepositionChiffre, NSPhraseur::prepChiffre) ;

  classeTout() ;

  //
  // Prise des informations sur le nom à completer
  //
  string sLexique = pMot->getLexique() ;
  string sPluriel = pMot->getPluriel() ;
  bool bFound = getPathologData(sLexique, Data) ;
  if (false == bFound)
    return false ;
  if (false == Data.estNom())
    return false ;
  //
  // Recuperation du genre (composition du genre recupere avec
  // le genre deja existant)
  Data.donneGenre(&iGenreNom);
  if (sPluriel != "")
      Data.donneGenrePluriel(&iGenreNom);

  // On regarde si le nombre est force.

  string sForceNbr = pMot->forceNombre(this);

  if (sForceNbr == FORCE_PLUR)
      Data.donneGenrePluriel(&iGenreNom);
  else if (sForceNbr == FORCE_SING)
      Data.donneGenreSingulier(&iGenreNom);

  string sNom = string("") ;

  donneLibelleAffiche(&sNom, &Data, iGenreNom) ;

  //
  // Complements : quand, ou, comment, avec quoi, pourquoi, dans quel but
  //
  _pCCArray->donnePhrase(iStyle) ;

  // Expansions du groupe nominal
  //
  // Cas 1 Adjectif epithete     le gros garcon
  //       le plus pres du nom, dans adjEpithete
  //
  // Cas 2 Complement du nom     le garcon aux yeux bleus
  //       dans les complements circonstanciels
  //
  // Cas 3 Subordonnee relative  le garcon qui a pris le ballon
  //       suppose qu'il y ait un verbe
  //

  // On traite d'abord les adjectifs epithètes
  //

  iterPhraseMot  iterMotsAdj;
  NSPathologData DataAdj;

  string sInterAvPos1 = "";
  string sInterAvPos2 = "";
  string sPhraseAvPos = "";

  string sInterApPos1 = "";
  string sInterApPos2 = "";
  string sPhraseApPos = "";

  string sInterApNeg1 = "";
  string sInterApNeg2 = "";
  string sPhraseApNeg = "";

  string sPhraseAv = "";
  string sPhraseAp = "";

  // On traite les epithetes qui se placent avant le nom.
  // Ils ont tous une certitude "WCEA0", ou bien pas de certitude.
  // En fait, *_pAdjEpitheteAvNeg est toujours vide.

  if (_pAdjEpitheteAvPos)
  {
      if (!(_pAdjEpitheteAvPos->empty()))
      {
          for (iterMotsAdj = _pAdjEpitheteAvPos->begin();
               iterMotsAdj != _pAdjEpitheteAvPos->end();
               iterMotsAdj++)
          {
              // L'epithete s'accorde avec le nom qu'il qualifie.
              if (!((*iterMotsAdj)->estTexteLibre()))
                  sInterAvPos2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom);

              else
                  sInterAvPos2 = (*iterMotsAdj)->getTexteLibre() ;

              etDuMilieu(&sPhraseAvPos, &sInterAvPos1, &sInterAvPos2);
          }
          etFinal(&sPhraseAvPos, &sInterAvPos1);
      }
  }

  // On traite les adjectifs qui se placent apres le nom.
  // D'abord les affirmatifs, puis les negatifs.
  // Si on a des positifs et des negatifs, on met "but not" entre les deux.
  // Si on n'a que des negatifs, on met juste "pas".

  if (_pAdjEpitheteApPos)
  {
      if (!(_pAdjEpitheteApPos->empty()))
      {
          for (iterMotsAdj = _pAdjEpitheteApPos->begin();
               iterMotsAdj != _pAdjEpitheteApPos->end();
               iterMotsAdj++)
          {
              // L'epithete s'accorde avec le nom qu'il qualifie.
              if (!((*iterMotsAdj)->estTexteLibre()))
                  sInterApPos2 = donneAdjectifComplet(*iterMotsAdj, iGenreNom);

              else
                  sInterApPos2 = (*iterMotsAdj)->getTexteLibre() ;

              etDuMilieu(&sPhraseApPos, &sInterApPos1, &sInterApPos2);
          }
          etFinal(&sPhraseApPos, &sInterApPos1);
      }
  }

  if (_pAdjEpitheteApNeg)
  {
      if (!(_pAdjEpitheteApNeg->empty()))
      {
          for (iterMotsAdj = _pAdjEpitheteApNeg->begin();
               iterMotsAdj != _pAdjEpitheteApNeg->end();
               iterMotsAdj++)
          {
              // L'epithete s'accorde avec le nom qu'il qualifie.

              sInterApNeg2 = donneAdjectifComplet(*iterMotsAdj);

              etDuMilieu(&sPhraseApNeg, &sInterApNeg1, &sInterApNeg2);
          }
          etFinal(&sPhraseApNeg, &sInterApNeg1);
      }
  }

  sPhraseAv = sPhraseAvPos;
  sPhraseAp = sPhraseApPos;

  if (sPhraseApNeg != "")
  {
      if (sPhraseApPos != "")
          sPhraseAp += string(" but ") + sPhraseApNeg;
      else
          sPhraseAp = sPhraseApNeg;
  }

  // On traite ensuite les complements du nom.
  // Considerons pour simplifier qu'il ne s'agit que de noms.
  // "Il y a suspicion d'endocartite."
  // Le complement du nom est le plus souvent introduit pâr la preposition "de".

  iterPhraseMot  iterMotsCompNom;
  NSPathologData DataCompNom;

  string sInterCompNomPos1 = "";
  string sInterCompNomPos2 = "";
  string sCompNomPos = "";

  string sInterCompNomNeg1 = "";
  string sInterCompNomNeg2 = "";
  string sCompNomNeg = "";

  string sCompNom = "";

  if (_pCompNomPos)
  {
    if (false == _pCompNomPos->empty())
    {
      for (iterMotsCompNom = _pCompNomPos->begin();
           iterMotsCompNom != _pCompNomPos->end();
           iterMotsCompNom++)
      {
        sLexique = (*iterMotsCompNom)->getLexique() ;
        if (false == (*iterMotsCompNom)->estTexteLibre())
        {
          bFound = getPathologData(sLexique, DataCompNom) ;
          if (false == bFound)
            return false ;

          sCertitude = string((*iterMotsCompNom)->getCertitude(), 0, 5) ;

          sInterCompNomPos2 = donneCertitude(sCertitude) + donneNomComplet(*iterMotsCompNom, (*iterMotsCompNom)->getArticle(), iStyle, sansCertitude) ;
        }
        else
          sInterCompNomPos2 = (*iterMotsCompNom)->getTexteLibre() ;

        etDuMilieu(&sCompNomPos, &sInterCompNomPos1, &sInterCompNomPos2);
      }
      etFinal(&sCompNomPos, &sInterCompNomPos1);
    }
  }

  if (_pCompNomNeg)
  {
    if (false == _pCompNomNeg->empty())
    {
      for (iterMotsCompNom = _pCompNomNeg->begin();
           iterMotsCompNom != _pCompNomNeg->end();
           iterMotsCompNom++)
      {
        sLexique = (*iterMotsCompNom)->getLexique() ;
        bFound = getPathologData(sLexique, DataCompNom) ;
        if (false == bFound)
          return false ;

        sCertitude = string((*iterMotsCompNom)->getCertitude(), 0, 5);
        sInterCompNomNeg2 = donneCertitude(sCertitude) + donneNomComplet(*iterMotsCompNom, (*iterMotsCompNom)->getArticle(), iStyle, sansCertitude);

        etDuMilieu(&sCompNomNeg, &sInterCompNomNeg1, &sInterCompNomNeg2);
      }
      etFinal(&sCompNomNeg, &sInterCompNomNeg1);
    }
  }

  sCompNom = sCompNomPos ;

  if (string("") != sCompNomNeg)
  {
    if (string("") != sCompNomPos)
      sCompNom += string(" but ") + sCompNomNeg ;
    else
      sCompNom = sCompNomNeg ;
  }

  string sCertitudeEcrit = donneCertitude(sCertitudeNom);

  if (string("") != sCertitudeEcrit)
  {
    _sPhrase = sCertitudeEcrit ;

    if (sPhraseAv != "")
      _sPhrase += string(" ") + sPhraseAv;
    if (sNom != "")
      _sPhrase += string(" ") + sNom;
    if (sCompNom != "")
      _sPhrase += string(" ") + sCompNom;
    if (sPhraseAp != "")
      _sPhrase += string(" ") + sPhraseAp;
  }
  else
  {
    _sPhrase = sPhraseAv;

    if (sPhraseAv != "")
      _sPhrase += string(" ");

    _sPhrase += sNom;

    if (sCompNom != "")
      _sPhrase += string(" ") + sCompNom;

    if (sPhraseAp != "")
      _sPhrase += string(" ") + sPhraseAp;
  }


  // Traitons les subordonnees circonstancielles.

  if (false == _pCCArray->empty())
  {
    for (GenCompIter itCompl = _pCCArray->begin(); itCompl != _pCCArray->end(); itCompl++)
    {
      if ((*itCompl)->getPositif() != string(""))
      {
        _sPhrase += " " + (*itCompl)->getPositif() ;
        if ((*itCompl)->getNegatif() != string(""))
          _sPhrase += string(" but not ") + (*itCompl)->getNegatif() ;
      }
      else if ((*itCompl)->getNegatif() != string(""))
        _sPhrase += string(" not ") + (*itCompl)->getNegatif() ;
    }
  }

  // On traite d'abord la preposition
  string sPrep = Data.chercheGrammaire(PREPOSITION) ;

  if (string("") != sPrep)
  {
    if (pMot->getPreposition() != "")
      pMot->setPreposition(pMot->getPreposition() + string(" ")) ;
    pMot->setPreposition(pMot->getPreposition() + string(sPrep, 1, strlen(sPrep.c_str()) - 1)) ;
  }

  // Traitement de l'article

  bool bPluriel = false ;

  if ((iGenreNom == genreMP) || (iGenreNom == genreFP) || (iGenreNom == genreNP))
    bPluriel = true ;

  if      (iArticle == NSPhraseMot::articleDefini)
    _sPhrase = donneArticleDefini(pMot, bPluriel, sansCertitude) + _sPhrase ;
  else if (iArticle == NSPhraseMot::articleIndefini)
    _sPhrase = donneArticleIndefini(pMot, bPluriel, sansCertitude) + _sPhrase ;
  else if (iArticle == NSPhraseMot::articlePartitif)
    _sPhrase = donneArticlePartitif(pMot, bPluriel, sansCertitude) + _sPhrase ;

  if (pMot->getPreposition() != "")
    _sPhrase = pMot->getPreposition() + string(" ") + _sPhrase ;

  if (iGereCertitude == avecCertitude)
  {
    sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude) ;

    if (sCertitudeEcrit != "")
      _sPhrase = sCertitudeEcrit + string(" ") + _sPhrase ;
  }

  if (pComp->pParentheses)
  {
    NsProposition Propos(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneParentheses(pContexte, &Propos, sLang) ;
    GeneParentheses.genereProposition(dcPhrase);
    _sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
  }

  // On indique que le nom est complete par une subordonnee relative en donnant
  // à iPhraseType la valeur phraseRelative.
  // Dans Nautilus, on considerera que l'antecedent a toujours valeur de sujet
  // relativement a la subordonnee relative. Celle-ci est donc toujours introduite
  // par le mot de liaison "who" dans le cas d'un antecedent humain, et "which" dans le
  // cas d'un antecedent inanime.
  // On met l'antecedent dans l'array Sujet du complement, pour permettre a generePhrase
  // de mettre le verbe a la bonne personne.

  if (pComp->iPhraseType == phraseRelative)
  {
    pComp->Sujet.vider() ;
    pComp->Sujet.push_back(new NSPhraseMot(*pMot)) ;
    NsProposition Propos(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneRelative(pContexte, &Propos, sLang) ;
    GeneRelative.genereProposition(dcPhrase) ;
    _sPhrase += GeneRelative.getPropositionPhrase() ;
  }

  return true ;
}


string
NSGenerateurEn::donneAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre, GERECERTITUDE iGereCertitude)
{
  NSPathologData  DataAdj ;

  string sLexique = pMot->getLexique() ;
  bool bFound = getPathologData(sLexique, DataAdj) ;
  if (false == bFound)
    return string("") ;

  if (false == DataAdj.estAdjectif())
    return string("") ;

  string sLibelle = string("") ;
  donneLibelleAffiche(&sLibelle, &DataAdj) ;

  // L'adjectif peut etre precede d'adverbes.

  if (pMot->getFirstComplementPhr())
  {
    NSPhraseur* pComp = pMot->getFirstComplementPhr() ;
    NsProposition Propos(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneCpl(pContexte, &Propos, sLang) ;
    if (GeneCpl.genereAdjectifComplet(pMot, iGenre, iGereCertitude))
      sLibelle = GeneCpl.getTempoPhrase() ;
  }

  if (avecCertitude == iGereCertitude)
  {
    string sCertitude = string(pMot->getCertitude(), 0, 5) ;
    sLibelle = donneCertitude(sCertitude) + sLibelle;
  }

  return sLibelle ;
}

bool
NSGenerateurEn::genereAdjectifComplet(NSPhraseMot* pMot, GENRE /* iGenre */, GERECERTITUDE iGereCertitude)
{
  _sPhrase = string("") ;

  reinitialise() ;

  NSPhraseur* pComp = pMot->getFirstComplementPhr() ;
  string   sLexique = "" ;

  NSPathologData Data;

  classeTout();

  //
  // Prise des informations sur l'adverbe à completer
  //
  sLexique = pMot->getLexique() ;
  bool bFound = getPathologData(sLexique, Data) ;
  if (false == bFound)
    return false ;
  if (false == Data.estAdjectif())
    return false ;

  string sAdjectif = string("") ;

  donneLibelleAffiche(&sAdjectif, &Data);
  if (sAdjectif == "")
      return false;

  // L'adjectif peut etre complete par des adverbes.

  iterPhraseMot  iterMotsComp;
  NSPathologData DataComp;

  string sInterCompPos1 = "";
  string sInterCompPos2 = "";
  string sCompPos = "";

  string sInterCompNeg1 = "";
  string sInterCompNeg2 = "";
  string sCompNeg = "";

  string sComp = "";

  if (_pAdverbePos)
  {
    if (!(_pAdverbePos->empty()))
    {
      for (iterMotsComp  = _pAdverbePos->begin();
           iterMotsComp != _pAdverbePos->end();
           iterMotsComp++)
      {
        sLexique = (*iterMotsComp)->getLexique() ;

        if (false == (*iterMotsComp)->estTexteLibre())
        {
          bFound = getPathologData(sLexique, DataComp) ;
          if (false == bFound)
            return false ;

          sInterCompPos2 = donneAdverbeComplet(*iterMotsComp) ;
        }
        else
          sInterCompPos2 = (*iterMotsComp)->getTexteLibre() ;


        etDuMilieu(&sCompPos, &sInterCompPos1, &sInterCompPos2);
      }
      etFinal(&sCompPos, &sInterCompPos1);
    }
  }

  if (_pAdverbeNeg)
  {
    if (false == _pAdverbeNeg->empty())
    {
      for (iterMotsComp  = _pAdverbeNeg->begin();
           iterMotsComp != _pAdverbeNeg->end();
           iterMotsComp++)
      {
        sLexique = (*iterMotsComp)->getLexique() ;
        bFound = getPathologData(sLexique, DataComp) ;
        if (false == bFound)
          return false ;

        sInterCompNeg2 = donneAdverbeComplet(*iterMotsComp) ;

        etDuMilieu(&sCompNeg, &sInterCompNeg1, &sInterCompNeg2) ;
      }
      etFinal(&sCompNeg, &sInterCompNeg1) ;
    }
  }

  sComp = sCompPos;

  if (sCompNeg != "")
  {
    if (sCompPos != "")
      sComp += string(" but ") + sCompNeg ;
    else
      sComp = sCompNeg ;
  }

  _sPhrase = sComp;

  if (sComp != "")
      _sPhrase += string(" ");
  _sPhrase += sAdjectif;

  // Tout a la fin, on met le complement entre parentheses.

  if (pComp->pParentheses)
  {
    NsProposition Propos(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneParentheses(pContexte, &Propos, sLang) ;
    GeneParentheses.genereProposition(dcPhrase) ;
    _sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
  }

  if (iGereCertitude == avecCertitude)
  {
    string sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude) ;

    if (string("") != sCertitudeEcrit)
      _sPhrase = sCertitudeEcrit + string(" ") + _sPhrase ;
  }

  return true ;
}

string
NSGenerateurEn::donneAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude)
{
  if ((NSPhraseMot*) NULL == pMot)
    return string("") ;

  NSPathologData Data ;

  bool bFound = getPathologData(pMot->getLexique(), Data) ;
  if (false == bFound)
    return string("") ;
  if (false == Data.estAdverbe())
    return string("") ;

  string sLibel = string("") ;
  donneLibelleAffiche(&sLibel, &Data) ;

  if (pMot->getFirstComplementPhr())
  {
    NsProposition Propos(pContexte, pMot->getFirstComplementPhr(), NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneCpl(pContexte, &Propos, sLang) ;
    if (GeneCpl.genereAdverbeComplet(pMot))
      sLibel = GeneCpl.getTempoPhrase() ;
  }

  if (iGereCertitude == avecCertitude)
  {
    string sCertitude = string(pMot->getCertitude(), 0, 5) ;
    string sCertitudeEcrit = donneCertitude(sCertitude) ;

    if (string("") != sCertitudeEcrit)
      sLibel = sCertitudeEcrit + string(" ") + sLibel ;
  }

  return sLibel ;
}

bool
NSGenerateurEn::genereAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE /* iGereCertitude */)
{
  _sPhrase = string("") ;

  reinitialise() ;

  bool     trouve ;
  NSPhraseur* pComp = pMot->getFirstComplementPhr() ;
  string   sLexique = "" ;

  NSPathologData Data;

  classeTout();

  //
  // Prise des informations sur l'adverbe à completer
  //
  sLexique = pMot->getLexique() ;
  trouve = getPathologData(sLexique, Data) ;
  if (false == trouve)
    return false ;
  if (false == Data.estAdverbe())
    return false ;

  string sAdverbe = string("") ;

  donneLibelleAffiche(&sAdverbe, &Data);
  if (sAdverbe == "")
    return false ;

  // L'adverbe peut etre complete par des adverbes.

  iterPhraseMot  iterMotsComp;
  NSPathologData DataComp;

  string sInterCompPos1 = "";
  string sInterCompPos2 = "";
  string sCompPos = "";

  string sInterCompNeg1 = "";
  string sInterCompNeg2 = "";
  string sCompNeg = "";

  string sComp = "";

  if (_pAdverbePos)
  {
    if (false == _pAdverbePos->empty())
    {
      for (iterMotsComp  = _pAdverbePos->begin();
           iterMotsComp != _pAdverbePos->end();
           iterMotsComp++)
      {
        sLexique = (*iterMotsComp)->getLexique() ;

        if (false == (*iterMotsComp)->estTexteLibre())
        {
          trouve = getPathologData(sLexique, DataComp) ;
          if (false == trouve)
            return false ;

          sInterCompPos2 = donneAdverbeComplet(*iterMotsComp);
        }
        else
          sInterCompPos2 = (*iterMotsComp)->getTexteLibre() ;

        etDuMilieu(&sCompPos, &sInterCompPos1, &sInterCompPos2) ;
      }
      etFinal(&sCompPos, &sInterCompPos1);
    }
  }

  if (_pAdverbeNeg)
  {
    if (false == _pAdverbeNeg->empty())
    {
      for (iterMotsComp  = _pAdverbeNeg->begin();
           iterMotsComp != _pAdverbeNeg->end();
           iterMotsComp++)
      {
        sLexique = (*iterMotsComp)->getLexique() ;
        trouve = getPathologData(sLexique, DataComp) ;
        if (false == trouve)
          return false ;

        sInterCompNeg2 = donneAdverbeComplet(*iterMotsComp) ;

        etDuMilieu(&sCompNeg, &sInterCompNeg1, &sInterCompNeg2) ;
      }
      etFinal(&sCompNeg, &sInterCompNeg1) ;
    }
  }

  sComp = sCompPos;

  if (sCompNeg != "")
  {
    if (sCompPos != "")
      sComp += string(" but ") + sCompNeg;
    else
      sComp = sCompNeg;
  }

  _sPhrase = sComp ;

  if (string("") != sComp)
    _sPhrase += string(" ") ;
  _sPhrase += sAdverbe ;

  // Tout a la fin, on met le complement entre parentheses.

  if (pComp->pParentheses)
  {
    NsProposition Propos(pContexte, &(pComp->pParentheses), NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneParentheses(pContexte, &Propos, sLang) ;
    GeneParentheses.genereProposition(dcPhrase) ;
    _sPhrase += string(" (") + GeneParentheses.getPropositionPhrase() + string(")") ;
  }

  return true ;
}

string
NSGenerateurEn::donneCertitude(string sCertitude, bool* pCommenceParVoyelleOuH, bool bInAffirmativeSequence)
{
  // pCommenceParVoyelleOuH sert au cas ou la certitude peut imposer une
  // apostrophe. On ne le modifie pas au cas ou la ceritude

  if (string("") == sCertitude)
    return string("") ;

  if (string("WCE00") == sCertitude)
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false;
    return string("not");
  }
  if ((sCertitude > "WCE00") && (sCertitude < "WCE50"))
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false;
    return string("probably not");
  }
  if (string(sCertitude, 0 ,5) == "WCE50")
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false;
    return string("perhaps");
  }
  if ((string(sCertitude, 0 ,5) > "WCE50") && (string(sCertitude, 0 ,5) < "WCE90"))
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false;
    return string("probably");
  }
  if ((string(sCertitude, 0 ,5) >= "WCE90") && (string(sCertitude, 0 ,5) < "WCEA0"))
  {
    if (pCommenceParVoyelleOuH)
      *pCommenceParVoyelleOuH = false;
    return string("certainly");
  }

  return string("") ;
}

string
NSGenerateurEn::donnePhraseComplement(NSPhraseMot* pLiaison,
                                      NSPhraseMot* pPreposition,
                                      NSPhraseMotArray* pCC,
                                      bool* bSucces,
                                      DCODETYPE iStyle)
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

  bool     trouve ;
  GENRE    iGenre ;

  NSPathologData Data ;

  for (iterPhraseMot iterMots = pCC->begin() ; pCC->end() != iterMots ; iterMots++)
  {
    string sInter2 = string("") ;

    // Est-ce une donnee chiffree ?
    string sFormat = (*iterMots)->getFormat() ;

    // Est-ce une structure de type NSPhraseMotTime ?
    // Is it a NSPhraseMotTime object
    NSPhraseMotTime* pDate = dynamic_cast<NSPhraseMotTime*>(*iterMots) ;
    if (pDate)
    {
      string sComplement ;
      string sUnite ;
      if (string("") != pDate->getValeurMin())
      {
        sComplement = pDate->getValeurMin() ;
        sUnite      = pDate->getUniteMin() ;
        sFormat     = pDate->getFormatMin() ;

        gereDateFr DateMin(pContexte->getSuperviseur(), sLang) ;
        DateMin.setDate(&sComplement) ;
        DateMin.setFormat(&sFormat) ;

        string sUniteSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
        DateMin.setUnite(&sUniteSens) ;

        string sLibelMin, sIntroMin ;

        if (string("") != pDate->getValeurMax())
        {
          DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateFr::DateDeb);

          sComplement = pDate->getValeurMax() ;
          sUnite      = pDate->getUniteMax() ;
          sFormat     = pDate->getFormatMax() ;

          gereDateFr DateMax(pContexte->getSuperviseur(), sLang);
          DateMax.setDate(&sComplement);
          DateMax.setFormat(&sFormat);

          string sUniteSens = string("") ;
          pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens);
          DateMax.setUnite(&sUniteSens);

          string sLibelMax, sIntroMax ;

          DateMax.donne_date_claire(&sLibelMax, &sIntroMax, gereDateEn::DateFin) ;

          sInter2 = sIntroMin + sLibelMin + string(" ") + sIntroMax + sLibelMax ;
        }
        else if (pDate->isMaxNow())
        {
          DateMin.donne_date_claire(&sLibelMin, &sIntroMin, gereDateEn::DateDeb);
          sInter2 = " since" + sIntroMin + sLibelMin;
        }
        else
        {
          DateMin.donne_date_claire(&sLibelMin, &sIntroMin);
          sInter2 = string("starting ") + sIntroMin + sLibelMin;
        }
      }
      else if (string("") != pDate->getValeurMax())
      {
        sComplement = pDate->getValeurMax() ;
        sUnite      = pDate->getUniteMax() ;
        sFormat     = pDate->getFormatMax() ;

        gereDateFr DateMax(pContexte->getSuperviseur(), sLang) ;
        DateMax.setDate(&sComplement) ;
        DateMax.setFormat(&sFormat) ;

        string sUniteSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
        DateMax.setUnite(&sUniteSens) ;

        string sLibelMax, sIntroMax ;

        if (pDate->isMinNow())
        {
          DateMax.donne_date_claire(&sLibelMax, &sIntroMax, gereDateEn::DateFin);
          sInter2 = " until" + sIntroMax + sLibelMax;
        }
        else
        {
          DateMax.donne_date_claire(&sLibelMax, &sIntroMax);
          sInter2 = sIntroMax + sLibelMax;
        }
      }
    }
    else if (string("") == sFormat)
    {
      string sLexique = (*iterMots)->getLexique() ;
      string sPluriel = (*iterMots)->getPluriel() ;
      if (false == (*iterMots)->estTexteLibre())
      {
        trouve = getPathologData(sLexique, Data) ;
        if (!trouve)
          return "" ;
        if (!(Data.estNom()))
          return "" ;

        // L'article du statut d'objet reel ou abstrait
        //
        if ((*iterMots)->getArticle() != NSPhraseMot::articleSans)
        {
          if (sPluriel == "")
            sInter2 = donneNomComplet(*iterMots, (*iterMots)->getArticle(), iStyle, sansCertitude);
          else if (string(sPluriel, 0, 5) == "WPLUR")
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleDefini, iStyle, sansCertitude);
          else if (string(sPluriel, 0, 5) == "WPLUS")
            sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleIndefini, iStyle, sansCertitude);
        }
        else
          sInter2 = donneNomComplet(*iterMots, NSPhraseMot::articleSans, iStyle, sansCertitude);

        string sCertitude = string((*iterMots)->getCertitude(), 0, 5) ;
        string sCertitudeEcrit = donneCertitude(sCertitude) ;

        if (sCertitudeEcrit != "")
          sInter2 = sCertitudeEcrit + string(" ") + sInter2 ;
      }
      else
        sInter2 = (*iterMots)->getTexteLibre() ;
    }
    else
    {
      //
      // Date
      //
      if      ((sFormat[0] == '£') &&
               ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK)))
      {
        string sComplement = (*iterMots)->getComplement() ;
        string sUnite      = (*iterMots)->getUnite() ;

        gereDateEn Date(pContexte->getSuperviseur(), sLang);
        Date.setDate(&sComplement);
        Date.setFormat(&sFormat);

        string sUniteSens = "";
        pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens);
        Date.setUnite(&sUniteSens);

        string sLibel, sIntro;

        Date.donne_date_claire(&sLibel, &sIntro);
        sInter2 = sIntro + sLibel;
      }
    }
    etDuMilieu(&sPhr, &sInter1, &sInter2);
  }
  etFinal(&sPhr, &sInter1);

  // Traitement de la preposition globale
  string sLexique = pPreposition->getLexique() ;
  if (string("") != sLexique)
  {
    string sPluriel = pPreposition->getPluriel() ;
    bool bFound = getPathologData(sLexique, Data) ;
    if (false == bFound)
      return string("") ;

    Data.donneGenre(&iGenre) ;
    if (string("") != sPluriel)
      Data.donneGenrePluriel(&iGenre) ;
    string sPrep = string("") ;
    donneLibelleAffiche(&sPrep, &Data, iGenre) ;

    sPhr = sPrep + string(" ") + sPhr ;
  }

  // Traitement de l'element de liaison
  sLexique = pLiaison->getLexique() ;
  if (string("") != sLexique)
  {
    string sPluriel = pLiaison->getPluriel() ;
    bool bFound = getPathologData(sLexique, Data) ;
    if (false == bFound)
      return string("") ;

    if (Data.estVerbe())
    {
      string sPartPresent = donneParticipePresent(&Data) ;
      sPhr = sPartPresent + string(" ") + sPhr ;
    }
    else
    {
      Data.donneGenre(&iGenre) ;
      if (string("") != sPluriel)
        Data.donneGenrePluriel(&iGenre) ;
      string sLink = string("") ;
      donneLibelleAffiche(&sLink, &Data, iGenre) ;
      sPhr = sLink + string(" ") + sPhr ;
    }
  }

  if (bSucces)
    *bSucces = true ;

  return sPhr ;
}

string
NSGenerateurEn::donnePhraseChiffree(NSPhraseMot* pLiaison,
                                    NSPhraseMot* pPreposition,
                                    NSPhraseMotArray* pCC,
                                    bool* bSucces)
{
  string sInter1 = "";
  string sInter2 = "";

  string sPhr = "";

  bool     trouve ;
  string   sLexique ;
  string   sPluriel ;
  GENRE    iGenre ;

  iterPhraseMot  iterMots;
  NSPathologData Data;

  if (false == pCC->empty())
  {
    for (iterMots = pCC->begin(); iterMots != pCC->end(); iterMots++)
    {
            // Traitement du complement, en fonction de son format
            //
            string sComplement = (*iterMots)->getComplement() ;

            string sUnite  = (*iterMots)->getUnite() ;
            string sFormat = (*iterMots)->getFormat() ;

            string sValeur = "";
            //
            // Valeur numerique   ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK)))
            //
            if (sFormat[0] == '£')
            {
                if (sFormat[1] == nbMARK)
                {
                    gereNum Num(pContexte->getSuperviseur(), sLang);
                    Num.instancier(&sComplement, &sUnite, &sFormat);
                    bool bSucces;
                    sValeur = decodeNum(&Num, &bSucces);

                    // On traite le cas ou la valeur chiffree est du type :
                    // "dans 3 jours", "il y a 3 jours", "pendant 3 jours",
                    // "il y a 2 ans pendant 3 jours"...

                    if (sLexique == "KDATE1")
                    {
                        // Cas ou il s'agit d'un evenement passe ponctuel :
                        // "3 days ago".
                        sValeur += string(" ago");
                    }
                    else if (sLexique == "KDATD1")
                    {
                        // Cas ou il s'agit d'un evenement futur ponctuel :
                        // "in 3 days".
                        sValeur = string("in ") + sValeur;
                    }
                }
                //
                // Date
                //
                else if ((sFormat[1] == dateMARK) || (sFormat[1] == dateHeureMARK))
                {
                    string sMessage = "" ;
                    string sIntro = "" ;

                    gereDateEn Date(pContexte->getSuperviseur(), sLang);
                    Date.setUnite(&sUnite);
                    Date.setFormat(&sFormat);
                    Date.setDate(&sComplement);
                    Date.donne_date_claire(&sMessage, &sIntro);
                    sValeur = sIntro + sMessage;
                }
                //
                // Heure
                //
                else if (sFormat[1] == heureMARK)
                {
                    string sMessage = "";

                    gereHeure Heure(pContexte->getSuperviseur(), sLang);
                    Heure.setUnite(&sUnite);
                    Heure.setFormat(&sFormat);
                    Heure.setHeure(&sComplement);
                    Heure.donne_heure(&sMessage);
                    sValeur = sMessage;
                }
            }

            sLexique = (*iterMots)->getLexique() ;
            if (sLexique != "")
            {
                trouve = getPathologData(sLexique, Data) ;
                if (!trouve)
                {
                    *bSucces = false;
                    return "";
                }
                // Recuperation du genre (composition du genre recupere avec
                // le genre deja existant)
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);
                //
                donneLibelleAffiche(&sInter2, &Data, iGenre);
            }

            string sMethode = (*iterMots)->getMethode() ;
            string sLibelMethode = "";

            if (sMethode != "")
            {
                trouve = getPathologData(sMethode, Data) ;
                if (!trouve)
                {
                    *bSucces = false;
                    return "";
                }
                // Recuperation du genre (composition du genre recupere avec
                // le genre deja existant)
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);
                //
                donneLibelleAffiche(&sLibelMethode, &Data, iGenre);
            }

            if (sInter2 != "")
                sInter2 += " ";
            sInter2 += sValeur;
            if (sLibelMethode != "")
                sInter2 += " (" + sLibelMethode + ")";

            etDuMilieu(&sPhr, &sInter1, &sInter2);
        }
        etFinal(&sPhr, &sInter1);

        // Traitement de la preposition
        sLexique = pPreposition->getLexique() ;
        if (sLexique != "")
        {
            sPluriel = pPreposition->getPluriel() ;
            trouve = getPathologData(sLexique, Data) ;
            if (!trouve)
                return "";
            Data.donneGenre(&iGenre);
            if (sPluriel != "")
                Data.donneGenrePluriel(&iGenre);
            donneLibelleAffiche(&sInter2, &Data, iGenre);
            sPhr = sInter2 + " " + sPhr;
        }
        // Traitement de l'element de liaison
        sLexique = pLiaison->getLexique() ;
        if (sLexique != "")
        {
            sPluriel = pLiaison->getPluriel() ;
            trouve = getPathologData(sLexique, Data) ;
            if (!trouve)
                return "";
            if (Data.estVerbe())
            {
                sInter2 = donneParticipePresent(&Data);
                sPhr = sInter2 + " " + sPhr;
            }
            else
            {
                Data.donneGenre(&iGenre);
                if (sPluriel != "")
                    Data.donneGenrePluriel(&iGenre);
                donneLibelleAffiche(&sInter2, &Data, iGenre);
                sPhr = sInter2 + " " + sPhr;
            }
        }
    }
    return sPhr;
}

bool
NSGenerateurEn::donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj)
{
  NSPathologData PremierAdjData;
  NSPhraseMot PremierAdj(pContexte);
  bool trouve;
  string sLexiqueAdj = string("") ;

  if (pCompAdj)
  {
    NsProposition Propos(pContexte, &pCompAdj, NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneCompl(pContexte, &Propos, sLang) ;
    pCompAdj->classeAdjectif(this) ;
    GeneCompl.classeTout() ;

    if (GeneCompl._pAdjEpitheteAvPos)
    {
      if (false == (GeneCompl._pAdjEpitheteAvPos)->empty())
      {
        PremierAdj = *(*((GeneCompl._pAdjEpitheteAvPos)->begin())) ;
        sLexiqueAdj = PremierAdj.getLexique() ;

        // Si jamais le premier adjectif est un texte libre, on l'ignore,
        // vu qu'on est incapable de le traiter grammaticalement.
        if (PremierAdj.estTexteLibre())
            return false ;

        trouve = getPathologData(sLexiqueAdj, PremierAdjData) ;
        if (false == trouve)
          return false ;
        if (pMotAdj)
          *pMotAdj = PremierAdj ;
        return true ;
      }
    }
    else if (GeneCompl._pAdjEpitheteAvNeg)
    {
      if (false == (GeneCompl._pAdjEpitheteAvNeg)->empty())
      {
        PremierAdj = *(*((GeneCompl._pAdjEpitheteAvNeg)->begin())) ;
        sLexiqueAdj = PremierAdj.getLexique() ;

        if (PremierAdj.estTexteLibre())
            return false ;

        trouve = getPathologData(sLexiqueAdj, PremierAdjData) ;
        if (false == trouve)
          return false ;
        if (pMotAdj)
          *pMotAdj = PremierAdj ;
        return true ;
      }
    }
  }
  return false ;
}

bool
NSGenerateurEn::donnePremierAdv(NSPhraseur* pComp, NSPhraseMot* pMotAdv)
{
  NSPathologData PremierAdvData;
  NSPhraseMot PremierAdv(pContexte);
  NSPhraseMot PremierAdvRec(pContexte);
  bool trouve;
  string sLexiqueAdv = "";

  if (pComp)
  {
    NsProposition Propos(pContexte, &pComp, NsProposition::notSetType, NsProposition::notSetConjonct) ;
    NSGenerateurEn GeneCompl(pContexte, &Propos, sLang) ;
    pComp->classeAdjectif(this) ;
    GeneCompl.classeTout() ;

    if (GeneCompl._pAdverbePos)
    {
      if (false == (GeneCompl._pAdverbePos)->empty())
      {
        PremierAdv = *(*((GeneCompl._pAdverbePos)->begin())) ;
        sLexiqueAdv = PremierAdv.getLexique() ;
        trouve = getPathologData(sLexiqueAdv, PremierAdvData);
        if (false == trouve)
          return false ;
        if (PremierAdv.getFirstComplementPhr())
        {
          if (donnePremierAdv(PremierAdv.getFirstComplementPhr(), &PremierAdvRec))
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
        sLexiqueAdv = PremierAdv.getLexique() ;
        trouve = getPathologData(sLexiqueAdv, PremierAdvData) ;
        if (false == trouve)
          return false ;
        if (PremierAdv.getFirstComplementPhr())
        {
          if (donnePremierAdv(PremierAdv.getFirstComplementPhr(), &PremierAdvRec))
          {
            *pMotAdv = PremierAdvRec ;
            return true ;
          }
        }
        if (pMotAdv)
          *pMotAdv = PremierAdv;
        return true;
      }
  }
  return false ;
}

string
NSGenerateurEn::decodeNum(gereNum* pNum, bool* bSucces)
{
    string sLibelle = "";
    *bSucces = true;

    if ((pNum->estExact()) || (pNum->estInf()) || (pNum->estSup()))
    {
        bool bPluriel = false;

        if (pNum->estExact())
        {
            sLibelle = pNum->getNum(pContexte, string("")) ;
            if (pNum->getValeur() > 1)
                bPluriel = true;
        }
        else
        {
            if ((pNum->estInf()) && (pNum->estSup()))
            {
                sLibelle = "between " + pNum->getNumInf() + " and " + pNum->getNumSup();
            }
            else if (pNum->estInf())
            {
                sLibelle = "more than " + pNum->getNumInf();
            }

            else if (pNum->estSup())
            {
                sLibelle = "less than " + pNum->getNumSup();
            }

            if ((pNum->getValeurInf() > 1) || (pNum->getValeurSup() > 1))
                bPluriel = true;
        }
        //
        // Libelle de l'unite de l'unite
        //
        if (sLibelle != "")
        {
            string sUniteLibel = pNum->donneLibelleUnite(pContexte) ;

            if (sUniteLibel != "")
                sLibelle += string(" ") + sUniteLibel;
        }
    }
    return sLibelle;
}

string
NSGenerateurEn::donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude)
{
  bool bModifieArticle = false;

  NSPhraseMot    MotAdj(pContexte);
  NSPhraseMot    MotAdv(pContexte);
  NSPathologData DataAdj;
  NSPathologData DataAdv;
  NSPathologData Data;

  string sLexiqueAdj = "";
  string sLexiqueAdv = "";

  string sCertitudeNom = "";
  string sCertitudeAdj = "";
  string sCertitudeAdv = "";

  string sLexique = pMot->getLexique() ;

  bool trouve = getPathologData(sLexique, Data) ;
  if (false == trouve)
    return string("") ;

  if (false == Data.estNom())
    return "" ;

  // Il n'y a pas d'article indefini au pluriel en anglais
  //
  if ((Data.estNom()) && bPluriel)
    return string(" ") ;

  GENRE  iGenreNom ;
  string sLibelle = string("") ;

  Data.donneGenre(&iGenreNom) ;
  donneLibelleAffiche(&sLibelle, &Data, iGenreNom) ;

  if (CommenceParVoyelle(sLibelle))
    bModifieArticle = true ;

  // On traite le cas des noms commencant par h.
  // Si le h est muet, l'article est an.
  // Sinon, l'article est a.
  // Cette information est stockee dans le libelle, en commentaire grammatical
  // {| / / }.
  // h : le h est muet.
  // H : le h est prononce.
  //
  if (('h' == sLibelle[0]) || ('H' == sLibelle[0]))
  {
    if (Data.chercheGrammaire(VALEUR_DE_H) == H_MUET)
      bModifieArticle = true;
  }

  if (pMot->getFirstComplementPhr())
  {
    bool bPremierAdjExiste = donnePremierAdj(pMot->getFirstComplementPhr(), &MotAdj);
    sLexiqueAdj = MotAdj.getLexique() ;

    if (bPremierAdjExiste)
    {
      sCertitudeAdj = string(MotAdj.getCertitude(), 0, 5) ;

      if (sCertitudeAdj != "")
      {
        // Le premier adjectif est precede d'une certitude, donc on ne
        // s'occupe pas de la premiere lettre de l'adjectif.
        //
        donneCertitude(sCertitudeAdj, &bModifieArticle);
      }
      else if (MotAdj.getFirstComplementPhr())
      {
        bool bPremierAdvExiste = donnePremierAdv(MotAdj.getFirstComplementPhr(), &MotAdv);
        sLexiqueAdv = MotAdv.getLexique() ;

        if (bPremierAdvExiste)
        {
          sCertitudeAdv = string(MotAdv.getCertitude() , 0, 5);

          if (sCertitudeAdv != "")
          {
            // Le premier adverbe est precede d'une certitude, donc on ne
            // s'occupe pas de la premiere lettre de l'adverbe.
            //
            donneCertitude(sCertitudeAdv, &bModifieArticle);
          }
          else
          {
            // On regarde si l'adverbe commence par une voyelle ou par un
            // h muet.
            string sLibelleAdv = "";
            donneLibelleAffiche(&sLibelleAdv, &DataAdv);

            if (CommenceParVoyelle(sLibelleAdv))
              bModifieArticle = true;

            else if ((sLibelleAdv[0] == 'h') || (sLibelleAdv[0] == 'H'))
            {
              if (DataAdv.chercheGrammaire(VALEUR_DE_H) == H_MUET)
                bModifieArticle = true;
            }
          }
        }
        else
        {
          // On regarde si l'adjectif commence par une voyelle ou par un
          // h muet.
          string sLibelleAdj = "";
          donneLibelleAffiche(&sLibelleAdj, &DataAdj);

          if (CommenceParVoyelle(sLibelleAdj))
            bModifieArticle = true;

          else if ((sLibelleAdj[0] == 'h') || (sLibelleAdj[0] == 'H'))
          {
            if (DataAdj.chercheGrammaire(VALEUR_DE_H) == H_MUET)
              bModifieArticle = true ;
          }
        }
      }
    }
  }

  // Si on gere la certitude du nom, celle-ci prend le dessus sur le reste.

  if (iGereCertitude == avecCertitude)
    donneCertitude(sCertitudeNom, &bModifieArticle) ;

  if (bModifieArticle)
    return string("an ") ;
  else
    return string("a ") ;
}

string
NSGenerateurEn::donneArticleDefini(NSPhraseMot* pMot, bool /* bPluriel */, GERECERTITUDE /* iGereCertitude */)
{
  if ((NSPhraseMot*) NULL == pMot)
    return string("") ;

  NSPathologData Data ;

  bool trouve = getPathologData(pMot->getLexique(), Data) ;
  if (false == trouve)
    return string("") ;

  if (false == Data.estNom())
    return string("") ;

  return string("the ") ;
}

string
NSGenerateurEn::donneArticlePartitif(NSPhraseMot* /* pMot */, bool /* bPluriel */, GERECERTITUDE /* iGereCertitude */)
{
  return string("") ;
}

string
NSGenerateurEn::donnePronomPersonnel(GENRE iGenre, NSPhraseur::VBPERSO /* iVbPersonne */, string /* sFonction */)
{
  switch (iGenre)
  {
    case genreMS : return string("he") ;
    case genreFS : return string("she") ;
    case genreNS : return string("it") ;
    case genreMP : return string("they") ;
    case genreFP : return string("they") ;
    case genreNP : return string("they") ;
    default      : return "" ;
  }
}

void
NSGenerateurEn::etFinal(string *type, const string *type1, const string sSeparator, bool bUseDefaultIfEmpty)
{
  if (((string*) NULL == type) || ((string*) NULL == type1))
    return ;

	if (string("") == *type1)
		return ;

	if (string("") != *type)
  {
    if ((string("") == sSeparator) && bUseDefaultIfEmpty)
      *type += string(" and ") ;
    else
      *type += sSeparator ;
  }

	*type += *type1 ;

	return ;
}

string
NSGenerateurEn::donneParticipePresent(NSPathologData* pPathoData)
{
  string sPartPresent = string("") ;
  donneLibelleAffiche(&sPartPresent, pPathoData, declinaisonPartPresent) ;
  return sPartPresent ;
}

string
NSGenerateurEn::donneParticipePasse(NSPhraseMot* pPhraseMot, GENRE /* iGenre */)
{
  if ((NSPhraseMot*) NULL == pPhraseMot)
    return string("") ;

  NSPathologData Data;

  bool bFound = getPathologData(pPhraseMot->getLexique(), Data) ;
  if (false == bFound)
    return string("") ;
  if (false == Data.estVerbe())
    return string("") ;

  string sPartPasse = string("") ;
  donneLibelleAffiche(&sPartPasse, &Data, declinaisonPartPasse) ;

  return sPartPasse ;
}

string
NSGenerateurEn::donnePreterite(NSPhraseMot* pPhraseMot)
{
  if ((NSPhraseMot*) NULL == pPhraseMot)
    return string("") ;

  NSPathologData Data ;
  bool bFound = getPathologData(pPhraseMot->getLexique(), Data) ;
  if (false == bFound)
    return string("") ;
  if (false == Data.estVerbe())
    return string("") ;

  string sPreterite = string("") ;
  donneLibelleAffiche(&sPreterite, &Data, declinaisonPreterite) ;

  return sPreterite ;
}

void
NSGenerateurEn::donneVerbe(string* principal, string* auxilliaire)
{
  if ((NULL == principal) || (NULL == auxilliaire))
    return ;

  *principal   = string("") ;
  *auxilliaire = string("") ;

  if (_pPh->Verbe.empty())
    return ;

    string   sLexique;
    NSPathologData Data;

  NSPhraseMot* pVerbe = *(_pPh->Verbe.begin()) ;
  string sVb = (*(_pPh->Verbe.begin()))->getLexique() ;

  string sVerbe ;
  pContexte->getDico()->donneCodeSens(&sVb, &sVerbe) ;

  bool trouve = getPathologData(sVb, Data) ;

  switch (_pPh->iVbAspect)
  {
    case NSPhraseur::aspectPonctuel :
    {
      switch (_pPh->iVbTemps)
      {
        case NSPhraseur::tempsPasseRevolu :
        case NSPhraseur::tempsPasseActuel : // preterite
        {
          if (sVb == "4ETRE1")
          {
            switch (_pPh->iVbPersonne)
            {
              case NSPhraseur::pers3S : *auxilliaire = "was" ;  return ;
              case NSPhraseur::pers1S : *auxilliaire = "was" ;  return ;
              default                 : *auxilliaire = "were" ; return ;
            }
          }
          else if (sVb == "4AVOI1")
          {
            switch (_pPh->iVbPersonne)
            {
              case NSPhraseur::pers3S : *auxilliaire = "had" ; return ;
              case NSPhraseur::pers1S : *auxilliaire = "had" ; return ;
              default                 : *auxilliaire = "had" ; return ;
            }
          }
          else
            *principal = donnePreterite(pVerbe) ;
        }
        case NSPhraseur::tempsPresent : // present simple
        {
          if (sVb == "4ETRE1")
          {
            switch (_pPh->iVbPersonne)
            {
              case NSPhraseur::pers1S : *auxilliaire = "am" ;  return ;
              case NSPhraseur::pers3S : *auxilliaire = "is" ;  return ;
              default                 : *auxilliaire = "are" ; return ;
            }
          }
          else if (sVb == "4AVOI1")
          {
            switch (_pPh->iVbPersonne)
            {
              case NSPhraseur::pers1S : *auxilliaire = "have" ; return ;
              case NSPhraseur::pers3S : *auxilliaire = "has" ;  return ;
              default                 : *auxilliaire = "have" ; return ;
            }
          }
          else
          {
            switch (_pPh->iVbPersonne)
            {
              case NSPhraseur::pers3S :
              {
                donneLibelleAffiche(principal, &Data, declinaison3Personne) ;
                break ;
              }
              default :
                donneLibelleAffiche(principal, &Data, declinaisonBase) ;
            }
          }
        }
        case NSPhraseur::tempsFuturLointain :
        case NSPhraseur::tempsFuturProche   : // futur
        {
          donneLibelleAffiche(principal, &Data, declinaisonBase) ;
          switch (_pPh->iVbPersonne)
          {
            case NSPhraseur::pers1S :
            case NSPhraseur::pers3S :
            {
              *auxilliaire = "shall" ;
              return ;
            }
            default :
            {
              *auxilliaire = "will" ;
              return ;
            }
          }
        }
        default : return ;
      }
    }
    case NSPhraseur::aspectIntervalle :
    {
      switch (_pPh->iVbTemps)
      {
        case NSPhraseur::tempsPasseRevolu : // present perfect
        {
          *principal = donneParticipePasse(pVerbe);
          switch (_pPh->iVbPersonne)
          {
            case NSPhraseur::pers3S : *auxilliaire = "has" ;  return ;
            default                 : *auxilliaire = "have" ; return ;
          }
        }
        case NSPhraseur::tempsPasseActuel : // present perfect progressif
        {
          *principal = string("been ") + donneParticipePresent(&Data) ;
          switch (_pPh->iVbPersonne)
          {
            case NSPhraseur::pers3S : *auxilliaire = "has" ;  return ;
            default                 : *auxilliaire = "have" ; return ;
          }
        }
        case NSPhraseur::tempsPresent : // present progressif
        {
          *principal = donneParticipePresent(&Data) ;
          switch (_pPh->iVbPersonne)
          {
            case NSPhraseur::pers1S : *auxilliaire = "am" ;  return ;
            case NSPhraseur::pers3S : *auxilliaire = "is" ;  return ;
            default                 : *auxilliaire = "are" ; return ;
          }
        }
        case NSPhraseur::tempsFuturProche :
        case NSPhraseur::tempsFuturLointain : // futur progressif
        {
          *principal   = string("be ") + donneParticipePresent(&Data) ;
          *auxilliaire = "will" ;
        }
        default : return;
      }
    }
    default : return;
  }
}

bool
NSGenerateurEn::donneVerbeClasse(NSPhraseMot* pPhraseMot, string* psTransitivite, string* psActEtat)
{
  // En anglais, le verbe est du type : etudier {|T/A}
  // T   pour Transitif, I intransitif
  // A   pour verbe d'Action,
  // E verbe d'etat (ER reel "to be", EA apparent "to look", ED dure "to stay"
  //                 EC change "to become")

  if (((NSPhraseMot*) NULL == pPhraseMot) || ((string*) NULL == psTransitivite)
                                          || ((string*) NULL == psActEtat))
    return false ;

  *psTransitivite = string("") ;
  *psActEtat      = string("") ;

  string sLexique = (*(_pPh->Verbe.begin()))->getLexique() ;
  NSPathologData Data ;

  bool bFound = getPathologData(sLexique, Data) ;
  if (false == bFound)
    return false ;

  if (false == Data.estVerbe())
    return false ;

  string sTransitif   = Data.chercheGrammaire("T") ;
  string sIntransitif = Data.chercheGrammaire("I") ;
  string sEtat        = Data.chercheGrammaire("E") ;
  string sAction      = Data.chercheGrammaire("A") ;

  // Un verbe ne peut pas etre a la fois d'action et d'etat, ni a la fois
  // transitif et intransitif(ou au moins dans le lexique).
  // Dans les deux cas, un verbe est soit l'un soit l'autre.

  if ((sTransitif != "") && (sIntransitif != ""))
    return false ;

  if ((sAction != "") && (sEtat != ""))
    return false ;

  if ((sTransitif == "") && (sIntransitif == ""))
    return false ;

  if ((sAction == "") && (sEtat == ""))
    return false ;

  if (string("") != sTransitif)
    *psTransitivite = sTransitif ;

  if (string("") != sIntransitif)
    *psTransitivite = sIntransitif ;

  if (string("") != sAction)
    *psActEtat = sAction ;

  if (string("") != sEtat)
    *psActEtat = sEtat ;

  if (string("") != *psActEtat)
  {
    if      (((*psActEtat)[0] == 'A') || ((*psActEtat)[0] == 'a'))
      _pPh->iVbType = NSPhraseur::vbTypeAction ;
    else if (((*psActEtat)[0] == 'E') || ((*psActEtat)[0] == 'e'))
      _pPh->iVbType = NSPhraseur::vbTypeEtat ;
  }

  return true ;
}

bool
NSGenerateurEn::traitePrepositionCC(NSPhraseMotArray* pCompC, NSPhraseMot* pPreposition, NSPhraseur::VBPREPOCTX iContexte)
{
  if (pPreposition)
  {
    if (pPreposition->getLexique() != "")
    {
      traitePostposition(pPreposition, pCompC) ;
      return true ;
    }
  }

  // Si on n'a pas specifie de preposition, il faut au moins avoir specifie un contexte.

  if (iContexte == 0)
    return false ;

  // Si on a specifie un contexte, on met une preposition par defaut en fonction du
  // contexte.

  // Pour le lieu, on met "au niveau de".
  if (iContexte == NSPhraseur::prepLieu)
  {
    (_pPh->PrepositionLieu).setLexique("1AUNI1") ;
    return traitePrepositionCC(pCompC, &(_pPh->PrepositionLieu)) ;
  }

    // Pour le temps, on met..., on met quoi au fait?

    // Pour la maniere, on met "avec"
    else if (iContexte == NSPhraseur::prepManiere)
    {
        (_pPh->PrepositionManiere).setLexique("0AVEC1");
        return traitePrepositionCC(pCompC, &(_pPh->PrepositionManiere));
    }

    // Pour la cause, mettre "a cause de".
    // Pour le but, mettre "pour".

    // pour le type, on met "a type de"

    else if (iContexte == NSPhraseur::prepType)
    {
        (_pPh->PrepositionType).setLexique("ATYPEDE");
        return traitePrepositionCC(pCompC, &(_pPh->PrepositionManiere));
    }

    return false;
}


string
NSGenerateurEn::postTraitement(string *sEntree)
{
  if (string("") == *sEntree)
    return string("") ;

  string sSortie = *sEntree ;

  // Contraction de la forme "can not" en "cannot"
  //
  size_t posit1 = sSortie.find("can not ") ;
  while (NPOS != posit1)
  {
    if (0 == posit1)
      sSortie.replace(0, 8, "cannot ") ;
    else if (sSortie[posit1-1] == ' ')
      sSortie.replace(posit1, 8, "cannot ");

    posit1 = sSortie.find("can not ", posit1+1) ;
  }

  return sSortie ;
}

//
// ////////////////  OBJET  gereDateEn   /////////////////
//

//
//  constructeur copie
//
gereDateEn::gereDateEn(const gereDateEn& src)
           :gereDate(src._pSuper, src._sLang)
{
  gereDate(*this) = (gereDate) src ;
}

//
//  operateur d'affectation
//
gereDateEn&
gereDateEn::operator=(const gereDateEn& src)
{
  if (this == &src)
		return *this ;

  gereDate(*this) = (gereDate) src ;

  return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//
void
gereDateEn::donne_date_breve(string* pMessage, string* pIntro, int iType)
{
    *pMessage = "";
    *pIntro 	 = "";
  	//
  	// Date au format AAAAMMJJ
	//
  	if (_sUnite == "2DA01")
  	{
        // Date vide
        if (_sDate == "00000000")
      	    return;
        // Juste AAAA -> "en 2001"
        else if (string(_sDate, 4, 4) == "0000")
        {
      	    *pMessage = string(_sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "in ";
                                    break;
                case DateDeb :      *pIntro = "from ";
                                    break;
                case DateFin :      *pIntro = "to ";
                                    break;
            }
        }
        // MM/AAAA -> "en juillet 2001"
        else if (string(_sDate, 6, 2) == "00")
        {
      	    *pMessage = donne_mois(string(_sDate, 4, 2)) + " " + string(_sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "in ";
                                    break;
                case DateDeb :      *pIntro = "from ";
                                    break;
                case DateFin :      *pIntro = "to ";
                                    break;
            }
        }
        // JJ/MM/AAAA -> "le 25/07/2001"
        else
        {
      	    *pMessage = string(_sDate, 6, 2) + "/" + string(_sDate, 4, 2) + "/" + string(_sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "on the ";
                                    break;
                case DateDeb :      *pIntro = "from the ";
                                    break;
                case DateFin :      *pIntro = "to the ";
                                    break;
            }
        }
    }
    //
  	// Date au format AAAAMMJJhhmmss
	//
  	if (_sUnite == "2DA02")
  	{
        // Date vide
        if (_sDate == "00000000")
      	    return;
        // Juste AAAA -> "en 2001"
        else if (string(_sDate, 4, 4) == "0000")
        {
      	    *pMessage = string(_sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "in ";
                                    break;
                case DateDeb :      *pIntro = "from ";
                                    break;
                case DateFin :      *pIntro = "to ";
                                    break;
            }
        }
        // MM/AAAA -> "en juillet 2001"
        else if (string(_sDate, 6, 2) == "00")
        {
      	    *pMessage = donne_mois(string(_sDate, 4, 2)) + " " + string(_sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "in ";
                                    break;
                case DateDeb :      *pIntro = "from ";
                                    break;
                case DateFin :      *pIntro = "to ";
                                    break;
            }
        }
        // JJ/MM/AAAA -> "le 25/07/2001"
        else
        {
      	    *pMessage = string(_sDate, 6, 2) + "/" + string(_sDate, 4, 2) + "/" + string(_sDate, 0, 4);
            switch (iType)
            {
                case DateNormal :   *pIntro = "on the ";
                                    break;
                case DateDeb :      *pIntro = "from the ";
                                    break;
                case DateFin :      *pIntro = "to the ";
                                    break;
            }
            string sHeure = "" ;
            // gestion de l'heure
            if      (strlen(_sDate.c_str()) == 10)
                sHeure = string(_sDate, 8, 2) + string("h") ;
            else if (strlen(_sDate.c_str()) == 12)
                sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) ;
            else if (strlen(_sDate.c_str()) == 14)
                sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;

            if (sHeure != "")
                *pMessage += string(" at ") + sHeure ;
        }
    }
    //
  	// Heure au format HHmm ou HH
	//
    else if ((_sUnite == "2HE01") || (_sUnite == "2HE00"))
    {
        if (_sUnite == "2HE01")
            *pMessage = string(_sDate, 0, 2) + string(":") + string(_sDate, 2, 2) ;
        if (_sUnite == "2HE00")
            *pMessage = _sDate + string(":") ;

        if ((*pMessage)[0] == '0')
            *pMessage = string(*pMessage, 1, strlen(pMessage->c_str()) - 1) ;

        switch (iType)
        {
            case DateNormal :
                *pIntro = "at " ;
                break;
            case DateDeb :
                *pIntro = "from " ;
                break;
            case DateFin :
                *pIntro = "to " ;
                break;
        }
    }
  	return;
}

//  +-----------------------------------------------------------------+
//  ¦           Donne la date au format "July 12th 1994"            ¦
//  +-----------------------------------------------------------------+
//
void
gereDateEn::donne_date_claire(string* pMessage, string* pIntro, int iType)
{
    unsigned int i;

    *pMessage = "";
    *pIntro 	 = "";
  	//
  	// Date au format AAAAMMJJ
	//
  	if (_sUnite == "2DA01")
  	{
        if (strlen(_sDate.c_str()) < 8)
            return;

        switch (iType)
        {
            case DateNormal :   *pIntro = "the ";
                                break;
            case DateDeb :      *pIntro = "from the ";
                                break;
            case DateFin :      *pIntro = "to the ";
                                break;
        }
		//
		// Jour
		//
		if (isdigit(_sDate[6]) && isdigit(_sDate[7]))
		{
			if (_sDate[6] == '0')
			{
                if (_sDate[7] != 0)
                {
				    if (_sDate[7] == '1')
					    *pMessage = "1st ";
				    else if (_sDate[7] == '2')
				    {
					    *pMessage = "2nd ";
				    }
                    else if (_sDate[7] == '3')
				    {
					    *pMessage = "3rd ";
				    }
                    else
                        *pMessage = _sDate[7];
                        *pMessage += "th ";
                }
                else
                {
                    switch (iType)
                    {
                        case DateNormal :   *pIntro = "in ";
                                            break;
                        case DateDeb :      *pIntro = "from ";
                                            break;
                        case DateFin :      *pIntro = "to ";
                                            break;
                    }
                }
			}
			else
			{
				*pMessage = _sDate[6];
				*pMessage += _sDate[7];
				*pMessage += "th ";
			}
		}
		else
			*pMessage = "?? ";
		//
		// Mois
		//
        string sMois = donne_mois(string(_sDate, 4, 2));
        if (sMois != "")
      	    *pMessage += sMois + " " + *pMessage + " ";
        else
      	    *pMessage = "?? ";
        //
    // Annee
		//
		for (i = 0; i < 4; i++)
		{
			if (isdigit(_sDate[i]))
				*pMessage += _sDate[i];
			else
				*pMessage += '?';
		}
    }
    //
  	// Date au format AAAAMMJJhhmmss
	//
  	else if (_sUnite == "2DA02")
  	{
        if (strlen(_sDate.c_str()) < 8)
            return;

        string sHeure = "" ;

        switch (iType)
        {
            case DateNormal :   *pIntro = "the ";
                                break;
            case DateDeb :      *pIntro = "from the ";
                                break;
            case DateFin :      *pIntro = "to the ";
                                break;
        }
		//
		// Jour
		//
		if (isdigit(_sDate[6]) && isdigit(_sDate[7]))
		{
			if (_sDate[6] == '0')
			{
                if (_sDate[7] != 0)
                {
				    if (_sDate[7] == '1')
					    *pMessage = "1st ";
				    else if (_sDate[7] == '2')
				    {
					    *pMessage = "2nd ";
				    }
                    else if (_sDate[7] == '3')
				    {
					    *pMessage = "3rd ";
				    }
                    else
                        *pMessage = _sDate[7];
                        *pMessage += "th ";
                }
                else
                {
                    switch (iType)
                    {
                        case DateNormal :   *pIntro = "in ";
                                            break;
                        case DateDeb :      *pIntro = "from ";
                                            break;
                        case DateFin :      *pIntro = "to ";
                                            break;
                    }
                }
			}
			else
			{
				*pMessage = _sDate[6];
				*pMessage += _sDate[7];
				*pMessage += "th ";
			}
            // gestion de l'heure
            if      (strlen(_sDate.c_str()) == 10)
                sHeure = string(_sDate, 8, 2) ;
            else if (strlen(_sDate.c_str()) == 12)
                sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) ;
            else if (strlen(_sDate.c_str()) == 14)
                sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;
		}
		else
			*pMessage = "?? ";
		//
		// Mois
		//
        string sMois = donne_mois(string(_sDate, 4, 2));
        if (sMois != "")
      	    *pMessage += sMois + " " + *pMessage + " ";
        else
      	    *pMessage = "?? ";
        //
    // Annee
		//
		for (i = 0; i < 4; i++)
		{
			if (isdigit(_sDate[i]))
				*pMessage += _sDate[i];
			else
				*pMessage += '?';
		}

        if (sHeure != "")
            *pMessage += string(" at ") + sHeure ;
    }
    //
  	// Heure au format HHmm ou HH
	//
    else if ((_sUnite == "2HE01") || (_sUnite == "2HE00"))
    {
        if (_sUnite == "2HE01")
            *pMessage = string(_sDate, 0, 2) + string("h") + string(_sDate, 2, 2) ;
        if (_sUnite == "2HE00")
            *pMessage = _sDate + string("h") ;

        if ((*pMessage)[0] == '0')
            *pMessage = string(*pMessage, 1, strlen(pMessage->c_str()) - 1) ;

        switch (iType)
        {
            case DateNormal :
                *pIntro = "at " ;
                break;
            case DateDeb :
                *pIntro = "from " ;
                break;
            case DateFin :
                *pIntro = "to " ;
                break;
        }
    }
	return;
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libelle du mois à partir de "MM"              ¦
//  +-----------------------------------------------------------------+
//  Cree le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDateEn::donne_mois(string mois)
{
    if (strlen(mois.c_str()) < 2)
   	    return "";
    if ((!(isdigit(mois[0]))) || (!(isdigit(mois[1]))))
   	    return "";
    int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]);

    return donne_mois(i);
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libelle du mois à partir d'un int             ¦
//  +-----------------------------------------------------------------+
//  Cree le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDateEn::donne_mois(int iMois)
{
    switch (iMois)
    {
   	    case  1 : return "January";
		case  2 : return "February";
		case  3 : return "March";
		case  4 : return "April";
		case  5 : return "May";
		case  6 : return "June";
		case  7 : return "July";
		case  8 : return "August";
		case  9 : return "September";
		case 10 : return "Octobre";
		case 11 : return "Novembre";
		case 12 : return "December";
    }
    return "";
}











