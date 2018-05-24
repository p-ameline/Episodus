#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

#include "nssavoir\nspatho.h"
#include "nssavoir\nsfilgd.h"
#include "dcodeur\nsphrase.h"
#include "dcodeur\nsphrobj.h"
#include "dcodeur\nsgenlan.h"

long NSPhraseObjet::lBaseObjectCount = 0 ;

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSPhraseLesion --------------------------
// -------------------------------------------------------------------------
NSPhraseObjet::NSPhraseObjet(NSContexte* pCtx, int iDecoType, string sLangue)
              :decodageBase(pCtx, sLangue)
{
  _iDcType = iDecoType ;

  lBaseObjectCount++ ;
}

NSPhraseObjet::NSPhraseObjet(decodageBase* pBase, int iDecoType, string sLang)
              :decodageBase(pBase)
{
  _iDcType = iDecoType ;
  _sLangue = sLang ;

  lBaseObjectCount++ ;
}

NSPhraseObjet::NSPhraseObjet(const NSPhraseObjet& rv)
              :decodageBase(rv.pContexte, rv._sLangue)
{
  _Objet 	 = rv._Objet ;
  _iDcType = rv._iDcType ;

  lBaseObjectCount++ ;
}

NSPhraseObjet::~NSPhraseObjet()
{
  lBaseObjectCount-- ;
}

void
NSPhraseObjet::ammorce()
{
  _Objet = *(*(getitDcode())) ;
}

void
NSPhraseObjet::decode(int /* colonne */)
{
  return ;
}

NSPhraseObjet&
NSPhraseObjet::operator=(const NSPhraseObjet& src)
{
  if (&src == this)
    return *this ;

	_Objet 	 = src._Objet ;
  _iDcType = src._iDcType ;

  return *this ;
}

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSPhraseLesion --------------------------
// -------------------------------------------------------------------------
NSPhraseLesion::NSPhraseLesion(NSContexte* pCtx, int iDecoType, string sLangue)
               :NSPhraseObjet(pCtx, iDecoType, sLangue)
{
}

NSPhraseLesion::NSPhraseLesion(decodageBase* pBase, int iDecoType, string sLangue)
               :NSPhraseObjet(pBase, iDecoType, sLangue)
{
}

NSPhraseLesion::~NSPhraseLesion()
{
}

void
NSPhraseLesion::decode(int colonne)
{                             // 0LOCA1|0FORM1|VEPAI1|0ASPV1
  NSPhraseTemporel* pDate = 0 ;

  int refCol = getCol() ;
  int refCol2 ;

  while ((getCol() > colonne) && iBon())
	{
    // Aspect
    if 	    (getSt() == "0ASPV")
    {
      Avance() ;
      refCol2 = getCol() ;

      while ((getCol() > refCol) && iBon())
      {
        if 	  ((getSt())[0] == 'A')
        {
          NSPhraseOrgane* pLoca = new NSPhraseOrgane(this) ;
          pLoca->ammorce() ;
          Avance() ;
          pLoca->decode(refCol2) ;
          Aspect.push_back(pLoca) ;
        }
        else
          Recupere() ;
      }
    }
    // Dimension
    // Localisation
    else if (getSt() == "0LOCA")
    {
      Avance() ;
      refCol2 = getCol() ;

      while ((getCol() > refCol) && iBon())
      {
        if 	  ((getSt())[0] == 'A')
        {
          NSPhraseOrgane* pLoca = new NSPhraseOrgane(this) ;
          pLoca->ammorce() ;
          Avance() ;
          pLoca->decode(refCol2) ;
          Localisations.push_back(pLoca) ;
        }
        else
          Recupere() ;
      }
    }
    else if ((getSt())[0] == 'A')
    {
      NSPhraseOrgane* pLoca = new NSPhraseOrgane(this) ;
      pLoca->ammorce() ;
      Avance() ;
      pLoca->decode(refCol) ;
      Localisations.push_back(pLoca) ;
    }
    // Date d'ouverture
    else if (getSt() == "KOUVR")
    {
      Avance() ;

      if (NULL == pDate)
        pDate = new NSPhraseTemporel(this) ;

      //
      // On ne sait pas s'il y aura un max, donc on paramètre
      // pour une phrase du type "à partir du"
      //
      pDate->iTypeTps    = TpsDate ;
      pDate->iFormeTps   = TpsInterval ;
      pDate->iRepererTps = TpsFutur ;

      pDate->decode(refCol, true) ;
    }
    // Date de fermeture
    else if (getSt() == "KFERM")
    {
      Avance() ;

      if (NULL == pDate)
        pDate = new NSPhraseTemporel(this) ;

      //
      // Date passée d'un évènement ponctuel
      //
      pDate->iTypeTps    = TpsDate ;
      pDate->iRepererTps = TpsFutur ;

      if (string("") != pDate->ValeurMin.getLexique())
      {
        // Date de début == Date de fin / Starting date == Ending date
        if ((pDate->ValeurMin.getComplement() == pDate->ValeurMax.getComplement()) &&
                    (pDate->ValeurMin.getUnit() == pDate->ValeurMax.getUnit()))
          pDate->iFormeTps = TpsInstant ;
        else
          pDate->iFormeTps = TpsInterval ;

        pDate->bMinNow = false ;
      }
      else
      {
        pDate->iFormeTps = TpsInterval ;
        pDate->bMinNow   = true ;
      }

      pDate->decode(refCol, false) ;
    }
		else
      Recupere() ;
  }

  if (pDate)
    Temporalite.push_back(pDate) ;

	return ;
}

void
NSPhraseLesion::metPhrase()
{
  NSPathologData Data ;
  bool     trouve ;

  _pPhraseur->initialise() ;
  setDcodeur(string("")) ;
  //
  // La première phase consiste à annoncer l'existence de la lésion,
  // sous forme d'un COD (ex, "Il existe un polype")
  //
  NSPhraseMot Lesion(_Objet.getDataTank(), pContexte) ;
  Lesion.setArticle(NSPhraseMot::articleIndefini) ;
  //
  // Dans l'ordre, on décrit l'aspect, la dimension puis la localisation
  // (ex, "Il existe un polype sessile, de 3 mm, au niveau du sigmoïde"
  //
  bool bLesionComplement = false ;
  //
  // L'aspect est un complément du nom de la lésion
  //
  if (false == Aspect.empty())
  {
    if (false == bLesionComplement)
    {
      Lesion.initComplement() ;
      bLesionComplement = true ;
    }
    iterPhraseObj i = Aspect.begin() ;
    for ( ; Aspect.end() != i ; i++)
    {
      string sLexique = (*i)->_Objet.getLexique() ;
      trouve = pContexte->getDico()->trouvePathologData(_sLangue, &sLexique, &Data) ;
      if (trouve)
      {
        if (Data.estAdjectif())
          Lesion.getOrCreateFirstComplementPhr()->adjEpithete.push_back(new NSPhraseMot((*i)->_Objet.getDataTank(), pContexte)) ;
      }
    }
  }
  //
  // La dimension est également un complément du nom de la lésion
  //

  //
  // La localisation est un épithète si c'est un adjectif, un complément de
  // lieu si c'est un nom
  //
  if (false == Localisations.empty())
  {
    iterPhraseObj i = Localisations.begin() ;
    for ( ; Localisations.end() != i ; i++)
    {
      string sLexique = (*i)->_Objet.getLexique() ;
      trouve = pContexte->getDico()->trouvePathologData(_sLangue, &sLexique, &Data) ;
      if (trouve)
      {
        if      (Data.estAdjectif())
        {
          if (false == bLesionComplement)
          {
            Lesion.initComplement() ;
            bLesionComplement = true ;
          }
          Lesion.getOrCreateFirstComplementPhr()->adjEpithete.push_back(new NSPhraseMot((*i)->_Objet.getDataTank(), pContexte)) ;
        }
        else if (Data.estNom())
          _pPhraseur->CCLieu.push_back(new NSPhraseMot((*i)->_Objet.getDataTank(), pContexte)) ;
      }
    }
  }

  //
  // Si la date a été saisie
  //
  if (false == Temporalite.empty())
  {
    if (false == bLesionComplement)
    {
      Lesion.initComplement() ;
      bLesionComplement = true ;
    }
    NSPhraseMot donnee(pContexte) ;

    iterPhraseObj i = Temporalite.begin() ;
    for ( ; Temporalite.end() != i ; i++)

/*
    {
      NSPhraseTemporel* pTp = (NSPhraseTemporel*)(*i) ;

      donnee.metAZero() ;
      donnee.setLexique(string(pTp->Objet.pDonnees->lexique)) ;

      donnee.setComplement(string(pTp->ValeurMin.pDonnees->complement)) ;
      donnee.setUnite(pTp->ValeurMin.getUnit()) ;

      donnee.setformat(string(pTp->ValeurMin.pDonnees->lexique)) ;
      Lesion.pComplement->CCTemps.push_back(new NSPhraseMot(donnee)) ;
    }
*/

    {
      NSPhraseTemporel* pTp = (NSPhraseTemporel*)(*i) ;

      NSPhraseMotTime* pDate = new NSPhraseMotTime(pContexte) ;
      pTp->initPhraseMotTime(pDate) ;

      Lesion.getOrCreateFirstComplementPhr()->CCTemps.push_back(pDate) ;
    }
  }

  //
  // La localisation est complément de lieu de la phrase
  //
  _pPhraseur->COD.push_back(new NSPhraseMot(Lesion)) ;

  if (_pGenerateur->genereProposition(dcTiret))
    setDcodeur(_pGenerateur->getPropositionPhrase()) ;
}

NSPhraseLesion::NSPhraseLesion(const NSPhraseLesion& rv)
               :NSPhraseObjet(rv.pContexte)
{
  _Objet 			    = rv._Objet ;
  _iDcType 		    = rv._iDcType ;

  Aspect          = rv.Aspect ;
  Dimensions      = rv.Dimensions ;
  Localisations   = rv.Localisations ;
  Etiologies      = rv.Etiologies ;
  Evoquant        = rv.Evoquant ;
  Semiologie      = rv.Semiologie ;

  SousLesions     = rv.SousLesions ;

  Traitements     = rv.Traitements ;
  CAS             = rv.CAS ;

  Temporalite     = rv.Temporalite ;
  Garbage         = rv.Garbage ;
}

NSPhraseLesion&
NSPhraseLesion::operator=(const NSPhraseLesion& src)
{
  if (&src == this)
    return *this ;

	_Objet 			    = src._Objet ;
  _iDcType 		    = src._iDcType ;

  Aspect          = src.Aspect ;
  Dimensions      = src.Dimensions ;
  Localisations   = src.Localisations ;
  Etiologies      = src.Etiologies ;
  Evoquant        = src.Evoquant ;
  Semiologie      = src.Semiologie ;

  SousLesions     = src.SousLesions ;

  Traitements     = src.Traitements ;
  CAS             = src.CAS ;

  Temporalite     = src.Temporalite ;
  Garbage         = src.Garbage ;

  return *this ;
}

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSPhraseGeste --------------------------
// -------------------------------------------------------------------------
NSPhraseGeste::NSPhraseGeste(NSContexte* pCtx, int iDecoType, string sLangue)
              :NSPhraseObjet(pCtx, iDecoType, sLangue)
{
}

NSPhraseGeste::NSPhraseGeste(decodageBase* pBase, int iDecoType, string sLangue)
              :NSPhraseObjet(pBase, iDecoType, sLangue)
{
}

NSPhraseGeste::~NSPhraseGeste()
{
}

void
NSPhraseGeste::decode(int colonne)
{
  int refCol = getCol() ;

  while ((getCol() > colonne) && iBon())
	{
    // Date
    if 	    (getSt() == "KCHIR")
    {
      NSPhraseTemporel* pDate = new NSPhraseTemporel(this) ;
      pDate->ammorce() ;
      Avance() ;
      //
      // Past date for a ponctual event
      // Date passée d'un évènement ponctuel
      //
      pDate->iTypeTps    = TpsDate ;
      pDate->iFormeTps   = TpsInstant ;
      pDate->iRepererTps = TpsPast ;

      pDate->decode(refCol) ;
      Temporalite.push_back(pDate) ;
    }
    else
      Recupere() ;
  }
	return ;
}

void
NSPhraseGeste::metPhrase()
{
  NSPathologData Data ;

  _pPhraseur->initialise() ;
  setDcodeur(string("")) ;
  //
  // La première phase consiste à annoncer l'existence du geste,
  // sous forme d'un COD
  //
  NSPhraseMot Geste(_Objet.getDataTank(), pContexte) ;
  Geste.setArticle(NSPhraseMot::articleIndefini) ;
  //
  //
  bool bLesionComplement = false ;
  //
  // Si la date a été saisie
  //
  if (false == Temporalite.empty())
  {
    if (false == bLesionComplement)
    {
      Geste.initComplement() ;
      bLesionComplement = true ;
    }
    NSPhraseMot donnee(pContexte) ;

    iterPhraseObj i = Temporalite.begin() ;
    for ( ; Temporalite.end() != i ; i++)
    {
      NSPhraseTemporel* pTp = (NSPhraseTemporel*)(*i) ;

      donnee.metAZero() ;
      donnee.setLexique(pTp->_Objet.getLexique()) ;

      donnee.setComplement(pTp->ValeurMin.getComplement()) ;
      donnee.setUnite(pTp->ValeurMin.getUnit()) ;

      donnee.setFormat(pTp->ValeurMin.getLexique()) ;
      Geste.getOrCreateFirstComplementPhr()->CCTemps.push_back(new NSPhraseMot(donnee)) ;
    }
  }
  _pPhraseur->COD.push_back(new NSPhraseMot(Geste)) ;
  if (_pGenerateur->genereProposition(dcTiret))
    setDcodeur(_pGenerateur->getPropositionPhrase()) ;
}

NSPhraseGeste::NSPhraseGeste(const NSPhraseGeste& rv)
              :NSPhraseObjet(rv.pContexte)
{
	_Objet      = rv._Objet ;
  _iDcType    = rv._iDcType ;

  Temporalite = rv.Temporalite ;
  Garbage     = rv.Garbage ;
}

NSPhraseGeste&
NSPhraseGeste::operator=(const NSPhraseGeste& src)
{
  if (&src == this)
    return *this ;

	_Objet      = src._Objet ;
  _iDcType    = src._iDcType ;

  Temporalite = src.Temporalite ;
  Garbage     = src.Garbage ;

  return *this ;
}

// -------------------------------------------------------------------------
// ------------------ METHODES DE NSPhrasePrescript ------------------------
// -------------------------------------------------------------------------
NSPhrasePrescript::NSPhrasePrescript(NSContexte* pCtx, int iDecoType, string sLangue)
              	  :NSPhraseObjet(pCtx, iDecoType, sLangue), _Forme((decodageBase*)this), _Event((decodageBase*)this), _Dates(pCtx)
{
  _iQuantitePhases  = 0 ;
  _sNonSubstituable = string("") ;
}

NSPhrasePrescript::NSPhrasePrescript(decodageBase* pBase, int iDecoType, string sLangue)
				          :NSPhraseObjet(pBase, iDecoType, sLangue), _Forme((decodageBase*)this), _Event((decodageBase*)this), _Dates(pBase->pContexte)
{
  _iQuantitePhases  = 0 ;
  _sNonSubstituable = string("") ;
}

NSPhrasePrescript::~NSPhrasePrescript()
{
}

void
NSPhrasePrescript::decode(int colonne)
{
try
{
  int refCol = getCol() ;
  // int refCol2 ;

  while ((getCol() > colonne) && iBon())
	{
    // Date d'ouverture
    if      (getSt() == "KOUVR")
    {
      Avance() ;

      _Dates.initMin(*(getitDcode())) ;

      //
      // On ne sait pas s'il y aura un max, donc on paramètre
      // pour une phrase du type "à partir du"
      //
      _Dates.setTypeTps(TpsDate) ;
      _Dates.setFormeTps(TpsInterval) ;
      _Dates.setRepererTps(TpsFutur) ;

      Avance() ;
    }
    // Date de fermeture
    else if (getSt() == "KFERM")
    {
      Avance() ;
      //
      // Date passée d'un évènement ponctuel
      //
      _Dates.initMax(*(getitDcode())) ;

      _Dates.setTypeTps(TpsDate) ;
      _Dates.setRepererTps(TpsFutur) ;

      if (string("") != _Dates.getValeurMin())
      {
        // Date de début == Date de fin / Starting date == Ending date
        if ((_Dates.getValeurMin() == _Dates.getValeurMax()) &&
                    (_Dates.getUniteMin() == _Dates.getUniteMax()))
          _Dates.setFormeTps(TpsInstant) ;
        else
          _Dates.setFormeTps(TpsInterval) ;

        _Dates.setMinNow(false) ;
      }
      else
      {
        _Dates.setFormeTps(TpsInterval) ;
        _Dates.setMaxNow(true) ;
      }

      Avance() ;
    }
    else if (getSt() == "KEVEI")
    {
      Avance() ;

      while ((getCol() > refCol) && iBon())
      {
        _Event.ammorce() ;
        Avance() ;
      }
    }
    // Forme (par exemple "comprimé")
    else if (getSt() == "0MEDF")
    {
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        _Forme.ammorce() ;
        Avance() ;
      }
    }
    // Type d'administration
    else if (getSt() == "0ADMI")
    {
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        NSPhraseObjet Type(this) ;
        Type.ammorce() ;
        _Administration.push_back(new NSPhraseObjet(Type)) ;
        Avance() ;
      }
    }
    // Voie d'administration
    else if (getSt() == "0VADM")
    {
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        NSPhraseObjet Type(this) ;
        Type.ammorce();
        //VoieAdministration.push_back(pType);
        Avance() ;
      }
    }
    // Phases
    else if (getSt() == "KPHAT")
    {
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        NSPhrasePhase* pPhase = new NSPhrasePhase(this) ;
        pPhase->pPrescript    = this ;
        pPhase->decode(refCol) ;

        if (iBon())
        {
          _iQuantitePhases++ ;
          _Phases.push_back(pPhase) ;
        }
        else
          delete pPhase ;
      }
    }
    // Détails
    else if (getSt() == "0DETA")
    {
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        if (getSt() == "£??")
        {
          NSPhraseMot* pTL = new NSPhraseMot((*(getitDcode()))->getDataTank(), pContexte) ;
          Avance() ;
          _Details.push_back(pTL) ;
        }
        else
          Recupere() ;
      }
    }
    // Texte libre
    else if (getSt() == "#TLI#")
    {
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        NSPhraseObjet* pTL = new NSPhraseObjet(this) ;
        pTL->ammorce() ;
        _FreeText.push_back(pTL) ;
        Avance() ;
      }
    }
    else if (getSt() == "£C;")
    {
      NSPhraseObjet* pTL = new NSPhraseObjet(this) ;
      pTL->ammorce() ;
      _FreeText.push_back(pTL) ;
      Avance() ;
    }
    // Admin informations: skipped so far
    //
    else if (string("LADMI") == getSt())
    {
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        if (string("LSUBS/WCE00") == getSt())
        {
          Avance() ;
          _sNonSubstituable = string("Ne pas substituer.") ;
        }
        else
          Avance() ;
      }
    }
    //
    // Id of proposal this prescription was created from (inside a referential)
    //
    else if (string("£RE") == getSt())
      Avance() ;
    else
      Recupere() ;
  }

	return ;
}
catch (...)
{
	erreur("Exception NSPhrasePrescript::decode", standardError) ;
}
}

void
NSPhrasePrescript::metPhrase(string decDeb, string decFin, int sautLigne)
{
  _pPhraseur->initialise() ;
  setDcodeur(string("")) ;

  if (string("") != _Event._Objet.getLexique())
  {
    NSPhraseur* pPhraEvent = new NSPhraseur(pContexte) ;
    pPhraEvent->iPhraseType = phraseComplement ;

    NsProposition PropEvent(pContexte, &pPhraEvent, NsProposition::principale, NsProposition::notSetConjonct) ;

    pPhraEvent->CCHypoth.push_back(new NSPhraseMot(_Event._Objet.getDataTank(), pContexte)) ;
    if (_pGenerateur->genereProposition(dcTiret, &PropEvent))
    {
      setDcodeur(PropEvent.getPhrase()) ;
      if (getDcodeur() != string(""))
      {
        setPseumajForFirstChar() ;
        addToDcodeur(string(" :")) ;
        decodageBase::metPhrase(decDeb, decFin, sautLigne) ;
      }
    }
  }

  if (_Phases.empty())
  {
    metPhraseFreeText(decDeb, decFin, sautLigne) ;
    return ;
  }

  iterPhraseObj iterPhrase = _Phases.begin() ;
  for ( ; _Phases.end() != iterPhrase ; iterPhrase++)
  {
    NSPhrasePhase* pPhase = dynamic_cast<NSPhrasePhase*>(*iterPhrase) ;
    if (pPhase)
    {
      pPhase->metPhrase() ;
      addToDcodeur(_pGenerateur->getPropositionPhrase()) ;
      decodageBase::metPhrase(decDeb, decFin, sautLigne) ;
    }
  }

  metPhraseFreeText(decDeb, decFin, sautLigne) ;

  if (string("") != _sNonSubstituable)
  {
    setDcodeur(_sNonSubstituable) ;
    decodageBase::metPhrase(decDeb, decFin, sautLigne) ;
  }

/*
    //
    // La première phase consiste à annoncer le mode d'administration,
    // sous forme d'un COD (ex, "réaliser une injection par voie IV")
    //
    //
    // Si le type d'administration a été saisi, c'est lui le COD
    // la forme est alors son complément de nom
    //
    if (!(Administration.empty()))
    {
    	iterPhraseObj i = Administration.begin();
        for (; i != Administration.end(); i++)
        	pPhraseur->COD.push_back(new NSPhraseMot((*i)->Objet.pDonnees, pContexte));
    }
    // Pas de type d'administration : c'est la forme qui devient COD
    //
    else
    {
        // pPhraseur->COD.push_back(new NSPhraseMot(Forme.Objet.pDonnees, pContexte));
        //
        if (!(PrisesJour.empty()))
        {
            // NSPhraseMot* pMotCOD = new NSPhraseMot(pContexte) ;
            // pMotCOD->initComplement() ;
            for (iterPhraseObj itPrise = PrisesJour.begin() ; itPrise != PrisesJour.end() ; itPrise++)
            {
                NSPhrasePrise* pPrise = static_cast<NSPhrasePrise*>(*itPrise);
                if (pPrise)
   	            {
                    NSPhraseMot* pUnePrise = new NSPhraseMot(Forme.Objet.pDonnees, pContexte) ;
                    pUnePrise->initComplement() ;
                    pUnePrise->pComplement->adjNumeralCardinal = NSPhraseMot(pPrise->nbDose.pDonnees, pContexte) ;
                    pUnePrise->pComplement->CCTemps.push_back(new NSPhraseMot(pPrise->Moment.pDonnees, pContexte)) ;
                    pPhraseur->COD.push_back(pUnePrise) ;
                }
            }
        }
    }

    // Les dates sont-elles précisées
    if (((Dates.valeurMin)[0] != '\0') || ((Dates.valeurMax)[0] != '\0'))
        pPhraseur->CCTemps.push_back(new NSPhraseMotTime(Dates)) ;

    if (pGenerateur->genereProposition(dcTiret))
    {
    	*sDcodeur() = pGenerateur->getPropositionPhrase() ;
        decodageBase::metPhrase(decDeb, decFin, sautLigne) ;

        if (!(Details.empty()))
        {
            for (iterPhraseMot itDet = Details.begin() ; itDet != Details.end() ; itDet++)
            {
                *sDcodeur() = (*itDet)->sTexteLibre;
                decodageBase::metPhrase(decDeb, decFin, sautLigne) ;
            }
        }
    }
*/
}

void
NSPhrasePrescript::metPhraseFreeText(string decDeb, string decFin, int sautLigne)
{
  if (_FreeText.empty())
    return ;

  iterPhraseObj iterPhrase = _FreeText.begin() ;
  for ( ; _FreeText.end() != iterPhrase ; iterPhrase++)
  {
    string sCodeLexique = (*iterPhrase)->_Objet.getLexique() ;
    string sSens ;
    pContexte->getDico()->donneCodeSens(&sCodeLexique, &sSens) ;

    if ((sSens == "£??") || (sSens == "£C;"))
    {
      setDcodeur((*iterPhrase)->_Objet.getTexteLibre()) ;
      decodageBase::metPhrase(decDeb, decFin, sautLigne) ;
    }
    else
    {
      setDcodeurFromLexique(sCodeLexique, _sLangue) ;
      if (string("") != getDcodeur())
      {
        setPseumajForFirstChar() ;
        addToDcodeur(string(".")) ;
      }
      decodageBase::metPhrase(decDeb, decFin, sautLigne) ;
    }
  }
}

NSPhrasePrescript::NSPhrasePrescript(const NSPhrasePrescript& rv)
              	  :NSPhraseObjet(rv.pContexte), _Forme(rv._Forme), _Event(rv._Event), _Dates(rv._Dates)
{
  _Objet 			      = rv._Objet ;
  _iDcType 		      = rv._iDcType ;

  _Forme            = rv._Forme ;
  _Dates            = rv._Dates ;

  _Administration   = rv._Administration ;
  _Event            = rv._Event ;

  _iQuantitePhases  = rv._iQuantitePhases ;
  _Phases           = rv._Phases ;

  _Garbage          = rv._Garbage ;
  _Details          = rv._Details ;
  _FreeText         = rv._FreeText ;

  _sNonSubstituable = rv._sNonSubstituable ;
}

NSPhrasePrescript&
NSPhrasePrescript::operator=(const NSPhrasePrescript& src)
{
  if (this == &src)
  	return *this ;

  _Objet 			      = src._Objet ;
  _iDcType 		      = src._iDcType ;

	_Forme            = src._Forme ;
  _Dates            = src._Dates ;

  _Administration   = src._Administration ;
  _Event            = src._Event ;

  _iQuantitePhases  = src._iQuantitePhases ;
  _Phases           = src._Phases ;

  _Garbage          = src._Garbage ;
  _Details          = src._Details ;
  _FreeText         = src._FreeText ;

  _sNonSubstituable = src._sNonSubstituable ;

  return *this ;
}

// -------------------------------------------------------------------------
// ------------------ METHODES DE NSPhrasePhase ------------------------
// -------------------------------------------------------------------------

NSPhrasePhase::NSPhrasePhase(NSContexte* pCtx, int iDecoType, string sLangue)
              :NSPhraseObjet(pCtx, iDecoType, sLangue), phrDuree((NSPhrasePhase*)this), DatePhase(pCtx), DateRenouv(pCtx)
{
	isDuration      = false ;
	iNbRenouv       = 0 ;
	iQuantiteCycles = 0 ;
	pPrescript      = (NSPhrasePrescript*) 0 ;
}

NSPhrasePhase::NSPhrasePhase(decodageBase* pBase, int iDecoType, string sLangue)
              :NSPhraseObjet(pBase, iDecoType, sLangue), phrDuree(pBase), DatePhase(pBase->pContexte), DateRenouv(pBase->pContexte)
{
  isDuration      = false ;
  iNbRenouv       = 0 ;
  iQuantiteCycles = 0 ;
  pPrescript      = (NSPhrasePrescript*) 0 ;
}

NSPhrasePhase::~NSPhrasePhase()
{
}

void
NSPhrasePhase::decode(int colonne)
{
try
{
	NSPhraseTemporel* pTemps = 0 ;

	int refCol = getCol();

	while ((getCol() > colonne) && iBon())
	{
  	// Date de début de la phase
    if      (getSt() == "KOUVR")
    {
      Avance();

      DatePhase.initMin(*(getitDcode())) ;

      //
      // On ne sait pas s'il y aura un max, dont on paramètre
      // pour une phrase du type "à partir du"
      //
      DatePhase.setTypeTps(TpsDate) ;
      DatePhase.setFormeTps(TpsInterval) ;
      DatePhase.setRepererTps(TpsFutur) ;

      Avance();
    }
    // Date de fin de la phase
    else if (getSt() == "KFERM")
    {
      Avance() ;

      DatePhase.initMax(*(getitDcode())) ;

      Avance() ;
    }
  	// Cycle(s)
    else if (getSt() == "KCYTR")
    {
    	Avance() ;
      // int refCol2 = getCol() ;
      while ((getCol() > refCol) && iBon())
      {
      	NSPhraseCycle* pCycle = new NSPhraseCycle(this) ;
        pCycle->decode(refCol) ;

        if (iBon())
        {
        	iQuantiteCycles++ ;
          Cycles.push_back(pCycle) ;
        }
        else
        	delete pCycle ;
      }
    }
    // Durée de la phase
    else if (getSt() == "VDURE")
    {
    	phrDuree.ammorce() ;
      isDuration = true ;
      phrDuree.setTypeTps(TpsDuration) ;
      phrDuree.setFormeTps(TpsInterval) ;

      Avance() ;

      phrDuree.decode(refCol, false, TpsDuration) ;
    }
    // Nombre de renouvellements posibles
    else if (getSt() == "VRENO")
    {
    	Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        NSPatPathoInfo PPTinfo = *(*(getitDcode())) ;
        gereNum numNbEvent(pContexte->getSuperviseur(), _sLangue) ;
        NSPatPathoInfo nbEventInfo ;

        string sLexique = PPTinfo.getLexique() ;
        if ((string("") != sLexique) && ('£' == sLexique[0]))
        {
        	nbEventInfo = PPTinfo ;
          Avance() ;
        }
        else
        	Recupere() ;

        string sEventCplmt = nbEventInfo.getComplement() ;
        string sEventUnit  = nbEventInfo.getUnit() ;
        string sEventFrmt  = nbEventInfo.getLexique() ;
        numNbEvent.instancier(&sEventCplmt, &sEventUnit, &sEventFrmt) ;

        iNbRenouv = int(numNbEvent.getValeur()) ;
      }
    }
    // renouvellement
    else if (getSt() == "GRENT")
    {
      int refCol2 = getCol() ;
      Avance() ;

      while ((getCol() > refCol) && iBon())
      {
        if (getSt() == "VDURE")
        {
          phrDuree.ammorce() ;
          isDuration = true ;
          phrDuree.setTypeTps(TpsDuration) ;
          phrDuree.setFormeTps(TpsInterval) ;

          int refCol3 = getCol() ;

          Avance() ;

          phrDuree.decode(refCol3, false, TpsDuration) ;
        }
        // Nombre de renouvellements posibles
        else if (getSt() == "VRENO")
        {
          Avance() ;
          while ((getCol() > refCol2) && iBon())
          {
            NSPatPathoInfo PPTinfo = *(*(getitDcode())) ;
            gereNum numNbEvent(pContexte->getSuperviseur(), _sLangue) ;
            NSPatPathoInfo nbEventInfo ;

            string sLexique = PPTinfo.getLexique() ;
            if ((string("") != sLexique) && ('£' == sLexique[0]))
            {
            	nbEventInfo = PPTinfo ;
              Avance() ;
            }
            else
            	Recupere() ;

            string sEventCplmt = nbEventInfo.getComplement() ;
            string sEventUnit  = nbEventInfo.getUnit() ;
            string sEventFrmt  = nbEventInfo.getLexique() ;
            numNbEvent.instancier(&sEventCplmt, &sEventUnit, &sEventFrmt) ;

            iNbRenouv = int(numNbEvent.getValeur()) ;
          }
        }
        // Date d'ouverture
        else if (getSt() == "KOUVR")
        {
          Avance() ;

          DateRenouv.initMin(*(getitDcode())) ;

          //
          // On ne sait pas s'il y aura un max, dont on paramètre
          // pour une phrase du type "à partir du"
          //
          DateRenouv.setTypeTps(TpsDate) ;
          DateRenouv.setFormeTps(TpsInterval) ;
          DateRenouv.setRepererTps(TpsFutur) ;

          Avance() ;
        }
        else
            Recupere() ;
      }
    }
    else
    	Recupere() ;
	}
  if (pTemps)
	{
  	// Temporalite.push_back(new NSPhraseTemporel(*pTemps));
    delete pTemps ;
	}
	return ;
}
catch (...)
{
	erreur("Exception NSPhrasePhase::decode", standardError) ;
}
}

void
NSPhrasePhase::metPhrase()
{
try
{
  NSPathologData Data ;

  _pPhraseur->initialise() ;
  setDcodeur(string("")) ;

  NsProposition Propos(pContexte) ;

  NSPropositionArray* pPropArray = new NSPropositionArray() ;

  if (false == Cycles.empty())
  {
    iterPhraseObj iterCycle = Cycles.begin() ;
    for ( ; Cycles.end() != iterCycle ; iterCycle++)
    {
      NSPhraseCycle* pCycle = dynamic_cast<NSPhraseCycle*>(*iterCycle) ;
      // NSPhraseCycle* pCycle = TYPESAFE_DOWNCAST(*iterCycle, NSPhraseCycle) ;
      if (pCycle)
      {
        // Cycle circadien
        //
        if (false == pCycle->Cycle_circadien.empty())
        {
          NsProposition* pPrCyCir = new NsProposition(pContexte) ;
          NSPropositionArray* pPropCyCirArray = new NSPropositionArray() ;
          pPrCyCir->pProposition  = pPropCyCirArray ;
          pPrCyCir->iObjectType   = NsProposition::isPropositionArray ;

          iterPhraseObj iterCircadien = pCycle->Cycle_circadien.begin() ;
          for ( ; pCycle->Cycle_circadien.end() != iterCircadien ; iterCircadien++)
          {
            NSPhrasePrise* pPrise = dynamic_cast<NSPhrasePrise*>(*iterCircadien) ;
            // NSPhrasePrise* pPrise = TYPESAFE_DOWNCAST(*iterCircadien, NSPhrasePrise) ;
            if (pPrise)
            {
              NSPhraseur* pPhraCycle = new NSPhraseur(pContexte) ;
              NSPhraseMot* pMot = new NSPhraseMot(pPrescript->_Forme._Objet.getDataTank(), pContexte) ;
              pMot->initComplement() ;
              pMot->getOrCreateFirstComplementPhr()->adjNumeralCardinal = NSPhraseMot(pPrise->nbDose.getDataTank(), pContexte) ;
              // NSPhraseMot* pMot = new NSPhraseMot(pPrise->nbDose.pDonnees, pContexte) ;
              pPhraCycle->COD.push_back(pMot) ;
              if (string("") != pPrise->Moment.getLexique())
                pPhraCycle->CCTemps.push_back(new NSPhraseMot(pPrise->Moment.getDataTank(), pContexte)) ;
              if (false == pPrise->Temporalite.estVide())
              {
                NSPhraseMotTime *pTime = new NSPhraseMotTime(pPrise->Temporalite) ;
                pPhraCycle->CCTemps.push_back(pTime) ;
              }
              if (false == pPrise->TempoCycle.estVide())
              {
                NSPhraseMotTimeCycle* pMotCycle = new NSPhraseMotTimeCycle(pContexte) ;
                pPrise->TempoCycle.initCycle(pMotCycle) ;
                pPhraCycle->CCTemps.push_back(pMotCycle) ;
              }

              pPropCyCirArray->push_back(new NsProposition(pContexte, &pPhraCycle, NsProposition::principale, NsProposition::notSetConjonct)) ;
            }
          }

          pPropArray->push_back(pPrCyCir) ;
        }
        // Rythme
        //
        if (false == pCycle->Rythme.empty())
        {
          NsProposition* pPrRythm = new NsProposition(pContexte) ;
          NSPropositionArray* pPropRythmArray = new NSPropositionArray() ;
          pPrRythm->pProposition      = pPropRythmArray ;
          pPrRythm->iObjectType       = NsProposition::isPropositionArray ;
          pPrRythm->iPropositionType  = NsProposition::principale ;

          iterPhraseObj iterRythme = pCycle->Rythme.begin() ;
          for ( ; pCycle->Rythme.end() != iterRythme ; iterRythme++)
          {
            NSPhraseTempoCycle* pRythme = dynamic_cast<NSPhraseTempoCycle*>(*iterRythme) ;
            // NSPhrasePrise* pPrise = TYPESAFE_DOWNCAST(*iterCircadien, NSPhrasePrise) ;
            if (pRythme)
            {
              NSPhraseur* pPhraRythme = new NSPhraseur(pContexte) ;
              pPhraRythme->iPhraseType = phraseComplement ;
              NsProposition* pProposRythme = new NsProposition(pContexte, &pPhraRythme, NsProposition::notSetType, NsProposition::notSetConjonct) ;

              NSPhraseMotTimeCycle* pMotCycle = new NSPhraseMotTimeCycle(pContexte) ;
              pRythme->initCycle(pMotCycle) ;
              pPhraRythme->CCTemps.push_back(pMotCycle) ;

              pPropRythmArray->push_back(pProposRythme) ;
            }
            else
            {
              NSPhraseur* pPhraRythme = new NSPhraseur(pContexte) ;
              pPhraRythme->iPhraseType = phraseComplement ;
              NsProposition* pProposRythme = new NsProposition(pContexte, &pPhraRythme, NsProposition::notSetType, NsProposition::notSetConjonct) ;

              pPhraRythme->CCTemps.push_back(new NSPhraseMot((*iterRythme)->_Objet.getDataTank(), pContexte)) ;

              pPropRythmArray->push_back(pProposRythme) ;
            }
          }

          pPropArray->push_back(pPrRythm) ;
        }
      }
      //if (!((*iterCycle)->Rythme.empty()))
      //{
      //}
    }
  }

  if (isDuration)
  {
    NSPhraseur* pPhraDuree      = new NSPhraseur(pContexte) ;
    NsProposition* pProposDuree = new NsProposition(pContexte, &pPhraDuree, NsProposition::notSetType, NsProposition::notSetConjonct) ;

    NSPhraseMotTime* pDate = new NSPhraseMotTime(pContexte) ;

    pDate->initDuree(&(phrDuree.ValeurDuree)) ;
    pPhraDuree->CCTemps.push_back(pDate) ;

    if (iNbRenouv > 0)
    {
      NSPhraseMot* pPhraseMot = new NSPhraseMot(pContexte) ;
      pPhraseMot->setLexique(string("VRENO2")) ;
      char szNbre[33] ;
      itoa(iNbRenouv, szNbre, 10) ;
      pPhraseMot->setComplement(string(szNbre)) ;
      pPhraseMot->setFormat(string("£N0;02")) ;
      pPhraseMot->setUnite(string("2FOIS1")) ;      pPhraseMot->setNumForme(NSPhraseMot::numTiret) ;      pPhraDuree->CCChiffre.push_back(pPhraseMot) ;    }

    pPhraDuree->iPhraseType = phraseComplement ;
    pPropArray->push_back(pProposDuree) ;
  }

  Propos.pProposition = pPropArray ;
  Propos.iObjectType  = NsProposition::isPropositionArray ;

  //
  // La première phase consiste à annoncer le mode d'administration,
  // sous forme d'un COD (ex, "réaliser une injection par voie IV")
  //
  //
  // Si la date a été saisie
  //
  // if (!(Administration.empty()))
  // {
  // 	iterPhraseObj i = Administration.begin();
  //    for (; i != Administration.end(); i++)
  //    	pPhraseur->COD.push_back(new NSPhraseMot((*i)->Objet.pDonnees, pContexte));
  // }
  if (_pGenerateur->genereProposition(dcTiret, &Propos))
  {
    //*sDcodeur() = pGenerateur->getPropositionPhrase() ;
    _pGenerateur->termineProposition(&Propos) ;
    setDcodeur(Propos.getPhrase()) ;
  }
}
catch (...)
{
	erreur("Exception NSPhrasePhase::metPhrase", standardError) ;
}
}

NSPhrasePhase::NSPhrasePhase(const NSPhrasePhase& rv)
              :NSPhraseObjet(rv.pContexte), phrDuree(rv.pContexte), DatePhase(rv.pContexte), DateRenouv(rv.pContexte)
{
  _Objet 			    = rv._Objet ;
  _iDcType 		    = rv._iDcType ;

  phrDuree        = rv.phrDuree ;
  DatePhase       = rv.DatePhase ;
  iNbRenouv       = rv.iNbRenouv ;
  DateRenouv      = rv.DateRenouv ;

  iQuantiteCycles = rv.iQuantiteCycles ;
  Cycles          = rv.Cycles ;
}

NSPhrasePhase&
NSPhrasePhase::operator=(const NSPhrasePhase& src)
{
	if (this == &src)
		return *this ;

	_Objet 			    = src._Objet ;
	_iDcType 		    = src._iDcType ;

	phrDuree        = src.phrDuree ;
	DatePhase       = src.DatePhase ;
  DateRenouv      = src.DateRenouv ;
	iNbRenouv       = src.iNbRenouv ;

	iQuantiteCycles = src.iQuantiteCycles ;
	Cycles          = src.Cycles ;

	return *this ;
}

// -------------------------------------------------------------------------
// ------------------ METHODES DE NSPhraseCycle ------------------------
// -------------------------------------------------------------------------

NSPhraseCycle::NSPhraseCycle(NSContexte* pCtx, int iDecoType, string sLangue)
              :NSPhraseObjet(pCtx, iDecoType, sLangue)
{
}

NSPhraseCycle::NSPhraseCycle(decodageBase* pBase, int iDecoType, string sLangue)
              :NSPhraseObjet(pBase, iDecoType, sLangue)
{
}

void
NSPhraseCycle::decode(int colonne)
{
try
{
	// NSPhraseTemporel* pTemps = 0 ;
	NSPatPathoInfo PPTinfo ;

	int refCol = getCol() ;

	while ((getCol() > colonne) && iBon())
	{
  	// Rythme circadien
    if      (getSt() == "KRYTH")
    {
    	Avance() ;
      int refCol2 = getCol() ;
      while ((getCol() > refCol) && iBon())
      {
      	if (getSt() == "KHHMM")
        {
        	Avance() ;

          NSPhrasePrise* pPrise = new NSPhrasePrise(this) ;
          pPrise->decode(refCol2) ;

          if (iBon())
          	Cycle_circadien.push_back(pPrise) ;
          else
          	delete pPrise ;
        }
        else if (getSt() == "KCYCI")
        {
        	Avance() ;
          int refCol3 = getCol() ;

          while ((getCol() > refCol2) && iBon())
          {
          	if (getSt() == "KRYLI")
            {
            	Avance() ;
              NSPhraseTempoCycle* pTmpCycle = new NSPhraseTempoCycle(this) ;
              pTmpCycle->iTypeCycle = NSPhraseTempoCycle::Libre ;
              pTmpCycle->decode(refCol3) ;
              if (iBon())
              {
              	NSPhrasePrise* pPrise = new NSPhrasePrise(this) ;
                pPrise->TempoCycle = *pTmpCycle ;
                pPrise->decode(refCol2) ;
                if (iBon())
                	Cycle_circadien.push_back(pPrise) ;
                else
                	delete pPrise ;
              }
              delete pTmpCycle ;
            }
            else if (getSt() == "KRYRE")
            {
            	Avance() ;
              NSPhraseTempoCycle* pTmpCycle = new NSPhraseTempoCycle(this) ;
              pTmpCycle->iTypeCycle = NSPhraseTempoCycle::Regulier ;
              pTmpCycle->decode(refCol3) ;
              if (iBon())
              {
              	NSPhrasePrise* pPrise = new NSPhrasePrise(this) ;
                pPrise->TempoCycle = *pTmpCycle ;
                pPrise->decode(refCol2) ;
                if (iBon())
                	Cycle_circadien.push_back(pPrise) ;
                else
                	delete pPrise ;
              }
              delete pTmpCycle ;
            }
            else
            	Recupere() ;
          }
        }
        else
        {
        	//
          PPTinfo = *(*(getitDcode())) ;
          string sCodeLexique = PPTinfo.getLexique() ;
          string sSens ;
          pContexte->getDico()->donneCodeSens(&sCodeLexique, &sSens) ;

          Avance() ;

          // Moment de la journée
          //
          if ((pContexte->getSuperviseur()->getFilGuide()->VraiOuFaux(sSens, string("ES"), "KMOME")) ||
              (pContexte->getSuperviseur()->getFilGuide()->VraiOuFaux(sSens, string("ES"), "KEVEQ")))
          {
          	NSPhrasePrise* pPrise = new NSPhrasePrise(this) ;
            pPrise->Moment = PPTinfo ;
            pPrise->decode(refCol2) ;

            if (iBon())
            	Cycle_circadien.push_back(pPrise) ;
            else
            	delete pPrise ;
          }
        }
      }
    }
    // Rythme de cure
    else if (getSt() == "KRYTP")
    {
    	Avance() ;
      int refCol2 = getCol() ;
      while ((getCol() > refCol) && iBon())
      {
      	if (getSt() == "KRYLI")
        {
        	Avance() ;
          NSPhraseTempoCycle* pTmpCycle = new NSPhraseTempoCycle(this) ;
          pTmpCycle->iTypeCycle = NSPhraseTempoCycle::Libre ;

          pTmpCycle->decode(refCol2) ;
          if (iBon())
          	Rythme.push_back(pTmpCycle) ;
          else
          	delete pTmpCycle ;
        }
        else if (getSt() == "KRYRE")
        {
        	Avance() ;
          NSPhraseTempoCycle* pTmpCycle = new NSPhraseTempoCycle(this) ;
          pTmpCycle->iTypeCycle = NSPhraseTempoCycle::Regulier ;

          pTmpCycle->decode(refCol2) ;
          if (iBon())
          	Rythme.push_back(pTmpCycle) ;
          else
          	delete pTmpCycle ;
        }
        else
        {
        	PPTinfo = *(*(getitDcode())) ;
          string sSens = PPTinfo.getLexiqueSens() ;

          // For Day 1, day 2, day 3
          if (string("2DAT0") == sSens)
          {
          	int refCol3 = getCol() ;

            NSPhraseObjet* pJour = new NSPhraseObjet(this) ;
            pJour->ammorce() ;

          	Avance() ;

            if (iBon())
      				while ((getCol() > refCol3) && iBon())
              {
              	if (getSt() == "VNUMT")
                {
                	Avance() ;
                  while ((getCol() > refCol3 + 1) && iBon())
            			{
                  	NSPatPathoInfo PPTinfo = *(*(getitDcode())) ;
                		gereNum numNbEvent(pContexte->getSuperviseur(), _sLangue) ;
                		NSPatPathoInfo nbEventInfo ;

                    string sLexique = PPTinfo.getLexique() ;
                    if ((string("") != sLexique) && ('£' == sLexique[0]))
                		{
                    	nbEventInfo = PPTinfo ;
                    	Avance() ;
                		}
                		else
                    	Recupere() ;

                		string sEventCplmt  = nbEventInfo.getComplement() ;
                		string sEventUnit   = nbEventInfo.getUnit() ;
                		string sEventFrmt   = nbEventInfo.getLexique() ;
                		numNbEvent.instancier(&sEventCplmt, &sEventUnit, &sEventFrmt) ;

                		// iNbRenouv = int(numNbEvent.getValeur()) ;
                  }
                }
              }

            if (iBon())
            	Rythme.push_back(pJour) ;
            else
            	delete pJour ;
          }
          // For Day 1, day 2, day 3
          else if (sSens == string("9VOID"))
          {
          	Avance() ;
          }
          // for day(s) of the week
          else if (pContexte->getSuperviseur()->getFilGuide()->VraiOuFaux(sSens, string("ES"), "KJSEM"))
          {
          	NSPhraseObjet* pJour = new NSPhraseObjet(this) ;
            pJour->ammorce() ;

            Avance() ;

            if (iBon())
            	Rythme.push_back(pJour) ;
            else
            	delete pJour ;
          }
          else
          	Recupere() ;
        }
      }
    }
    else
    	Recupere() ;
  }
	return ;
}
catch (...)
{
	erreur("Exception NSPhraseCycle::decode", standardError) ;
}
}

void
NSPhraseCycle::metPhrase()
{
  NSPathologData Data ;

  _pPhraseur->initialise() ;
  setDcodeur(string("")) ;

  NsProposition Propos(pContexte) ;
  NSPropositionArray* pPropArray = new NSPropositionArray() ;

  // Cycle circadien
  //
  if (false == Cycle_circadien.empty())
  {
    NsProposition* pPrCyCir = new NsProposition(pContexte) ;
    NSPropositionArray* pPropCyCirArray = new NSPropositionArray() ;
    pPrCyCir->pProposition  = pPropCyCirArray ;
    pPrCyCir->iObjectType   = NsProposition::isPropositionArray ;

    iterPhraseObj iterCircadien = Cycle_circadien.begin() ;
    for ( ; Cycle_circadien.end() != iterCircadien ; iterCircadien++)
    {
      NSPhrasePrise* pPrise = dynamic_cast<NSPhrasePrise*>(*iterCircadien) ;
      // NSPhrasePrise* pPrise = TYPESAFE_DOWNCAST(*iterCircadien, NSPhrasePrise) ;
      if (pPrise)
      {
        NSPhraseur* pPhraCycle = new NSPhraseur(pContexte) ;
        if (string("") != pPrise->Moment.getLexique())
          pPhraCycle->CCTemps.push_back(new NSPhraseMot(pPrise->Moment.getDataTank(), pContexte)) ;
        if (false == pPrise->Temporalite.estVide())
        {
          NSPhraseMotTime* pMotTime = new NSPhraseMotTime(pPrise->Temporalite) ;
          pPhraCycle->CCTemps.push_back(pMotTime) ;
        }
        if (false == pPrise->TempoCycle.estVide())
        {
          NSPhraseMotTimeCycle* pMotCycle = new NSPhraseMotTimeCycle(pContexte) ;
          pPrise->TempoCycle.initCycle(pMotCycle) ;
          pPhraCycle->CCTemps.push_back(pMotCycle) ;
        }

        pPropCyCirArray->push_back(new NsProposition(pContexte, &pPhraCycle, NsProposition::principale, NsProposition::notSetConjonct)) ;
      }
    }

    pPropArray->push_back(pPrCyCir) ;
  }
  // Rythme
  //
  if (false == Rythme.empty())
  {
    NsProposition* pPrRythm = new NsProposition(pContexte) ;
    NSPropositionArray* pPropRythmArray = new NSPropositionArray() ;
    pPrRythm->pProposition     = pPropRythmArray ;
    pPrRythm->iObjectType      = NsProposition::isPropositionArray ;
    pPrRythm->iPropositionType = NsProposition::principale ;

    iterPhraseObj iterRythme = Rythme.begin() ;
    for ( ; Rythme.end() != iterRythme ; iterRythme++)
    {
      NSPhraseTempoCycle* pRythme = dynamic_cast<NSPhraseTempoCycle*>(*iterRythme) ;
      // NSPhrasePrise* pPrise = TYPESAFE_DOWNCAST(*iterCircadien, NSPhrasePrise) ;
      if (pRythme)
      {
        NSPhraseur* pPhraRythme = new NSPhraseur(pContexte) ;
        pPhraRythme->iPhraseType = phrasePrincipale ;
        NsProposition* pProposRythme = new NsProposition(pContexte, &pPhraRythme, NsProposition::notSetType, NsProposition::notSetConjonct) ;

        NSPhraseMotTimeCycle* pMotCycle = new NSPhraseMotTimeCycle(pContexte) ;
        pRythme->initCycle(pMotCycle) ;
        pPhraRythme->CCTemps.push_back(pMotCycle) ;

        pPropRythmArray->push_back(pProposRythme) ;
      }
      else
      {
        NSPhraseur* pPhraRythme = new NSPhraseur(pContexte) ;
        pPhraRythme->iPhraseType = phrasePrincipale ;
        NsProposition* pProposRythme = new NsProposition(pContexte, &pPhraRythme, NsProposition::notSetType, NsProposition::notSetConjonct) ;

        pPhraRythme->COD.push_back(new NSPhraseMot((*iterRythme)->_Objet.getDataTank(), pContexte)) ;

        pPropRythmArray->push_back(pProposRythme) ;
      }
    }

    pPropArray->push_back(pPrRythm) ;
  }

  Propos.pProposition = pPropArray ;
  Propos.iObjectType  = NsProposition::isPropositionArray ;

  if (_pGenerateur->genereProposition(dcTiret, &Propos))
  {
    _pGenerateur->termineProposition(&Propos) ;
    setDcodeur(Propos.getPhrase()) ;
  }
}

NSPhraseCycle::NSPhraseCycle(const NSPhraseCycle& rv)
              :NSPhraseObjet(rv.pContexte)
{
  _Objet          = rv._Objet;
  _iDcType        = rv._iDcType;

  Cycle_circadien = rv.Cycle_circadien ;
  Rythme          = rv.Rythme ;
}

NSPhraseCycle&
NSPhraseCycle::operator=(const NSPhraseCycle& src)
{
  if (this == &src)
  	return *this ;

  _Objet 			    = src._Objet ;
  _iDcType 		    = src._iDcType ;

  Cycle_circadien = src.Cycle_circadien ;
  Rythme          = src.Rythme ;

  return *this ;
}

// -------------------------------------------------------------------------
// ------------------ METHODES DE NSPhrasePrise ------------------------
// -------------------------------------------------------------------------
NSPhrasePrise::NSPhrasePrise(NSContexte* pCtx, int iDecoType, string sLangue)
              :NSPhraseObjet(pCtx, iDecoType, sLangue), Temporalite(pCtx), TempoCycle(pCtx)
{
}

NSPhrasePrise::NSPhrasePrise(decodageBase* pBase, int iDecoType, string sLangue)
              :NSPhraseObjet(pBase, iDecoType, sLangue), Temporalite(pBase->pContexte), TempoCycle(pBase->pContexte)
{
}

void
NSPhrasePrise::decode(int colonne)
{
try
{
  NSPhraseTemporel* pTemps = 0 ;

  int refCol = getCol() ;

  while ((getCol() > colonne) && iBon())
	{
    // Nombre de doses
    if      (getSt() == "VNBDO")
    {
      Avance() ;

      NSPatPathoInfo PPTinfo ;

      while ((getCol() > refCol) && iBon())
      {
        //
        PPTinfo = *(*(getitDcode())) ;

        string sLexique = PPTinfo.getLexique() ;
        if ((string("") != sLexique) && ('£' == sLexique[0]))
        {
          nbDose = PPTinfo ;
          Avance() ;
        }
        else
          Recupere() ;
      }
    }
    // Date de fermeture
    else if (getSt() == "KFERM")
    {
      if (NULL == pTemps)
      {
        pTemps = new NSPhraseTemporel(this) ;
        pTemps->ammorce() ;
      }
      Avance() ;
      //
      // Date passée d'un évènement ponctuel
      //
      pTemps->iTypeTps    = TpsDate;
      pTemps->iFormeTps   = TpsInstant;
      pTemps->iRepererTps = TpsFutur;

      pTemps->decode(refCol, false);
    }
    // Type d'administration
    else if (getSt() == "0ADMI")
    {
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
        NSPhraseObjet* pType = new NSPhraseObjet(this) ;
        pType->ammorce() ;
        // Administration.push_back(new NSPhraseObjet(*pType));
        delete pType ;
        Avance() ;
      }
    }
    // Voie d'administration
    else if (getSt() == "0VADM")
    {
      Avance();
      while ((getCol() > refCol) && iBon())
      {
        NSPhraseObjet* pType = new NSPhraseObjet(this) ;
        pType->ammorce() ;
        // VoieAdministration.push_back(new NSPhraseObjet(*pType));
        delete pType ;
        Avance() ;
      }
    }
    else
    {
      gereHeure heure(pContexte->getSuperviseur(), _sLangue) ;
      donneHeure(colonne, &heure) ;

      if (false == heure.estVide())
      {
        Temporalite.initMin(&heure) ;
        Temporalite.setSharpDate() ;
        //
        Temporalite.setTypeTps(TpsDate) ;
        Temporalite.setFormeTps(TpsInstant) ;
        Temporalite.setRepererTps(TpsFutur) ;
      }
      else
        Recupere() ;
    }
  }
  if (pTemps)
    //Temporalite.push_back(new NSPhraseTemporel(*pTemps));
    delete pTemps ;

	return ;
}
catch (...)
{
	erreur("Exception NSPhrasePrise::decode", standardError) ;
}
}

void
NSPhrasePrise::metPhrase()
{
    NSPathologData Data;

    _pPhraseur->initialise();
    setDcodeur(string("")) ;
    //
    // La première phase consiste à annoncer le mode d'administration,
    // sous forme d'un COD (ex, "réaliser une injection par voie IV")
    //
    //
    // Si la date a été saisie
    //
    // if (!(Administration.empty()))
    // {
    // 	iterPhraseObj i = Administration.begin();
    //    for (; i != Administration.end(); i++)
    //    	pPhraseur->COD.push_back(new NSPhraseMot((*i)->Objet.pDonnees, pContexte));
    // }
    if (_pGenerateur->genereProposition(dcTiret))
    	setDcodeur(_pGenerateur->getPropositionPhrase()) ;
}

NSPhrasePrise::NSPhrasePrise(const NSPhrasePrise& rv)
              :NSPhraseObjet(rv.pContexte), Temporalite(rv.pContexte), TempoCycle(rv.pContexte)
{
  _Objet      = rv._Objet ;
  _iDcType    = rv._iDcType ;

  nbDose	   	= rv.nbDose ;
  ValeurDose  = rv.ValeurDose ;
  ObjetDose   = rv.ObjetDose ;

  Moment      = rv.Moment ;
  Temporalite = rv.Temporalite ;
  TempoCycle  = rv.TempoCycle ;
}

NSPhrasePrise&
NSPhrasePrise::operator=(const NSPhrasePrise& src)
{
  if (this == &src)
  	return *this ;

	_Objet      = src._Objet ;
  _iDcType    = src._iDcType ;

  nbDose	   	= src.nbDose ;
  ValeurDose  = src.ValeurDose ;
  ObjetDose   = src.ObjetDose ;

  Moment      = src.Moment ;
  Temporalite = src.Temporalite ;
  TempoCycle  = src.TempoCycle ;

  return *this ;
}

// -------------------------------------------------------------------------
// ------------------ METHODES DE NSPhrasePrise ------------------------
// -------------------------------------------------------------------------
/*
NSPhrasePrise::NSPhrasePrise(NSContexte* pCtx, int iDecoType, string sLangue)
              :NSPhraseObjet(pCtx, iDecoType, sLangue), Temporalite(pCtx)
{
}

NSPhrasePrise::NSPhrasePrise(decodageBase* pBase, int iDecoType, string sLangue)
              :NSPhraseObjet(pBase, iDecoType, sLangue), Temporalite(pBase->pContexte)
{
}

void
NSPhrasePrise::decode(int colonne)
{
    NSPhraseTemporel* pTemps = 0 ;

    int refCol = getCol();

    while ((getCol() > colonne) && iBon())
	{
        // Date d'ouverture
        if      (getSt() == "KOUVR")
        {
            if (!pTemps)
            {
                pTemps = new NSPhraseTemporel(this);
                pTemps->ammorce();
            }
            Avance();
            //
            // Date passée d'un évènement ponctuel
            //
            pTemps->iTypeTps    = NSPhraseTemporel::TpsDate;
            pTemps->iFormeTps   = NSPhraseTemporel::TpsInstant;
            pTemps->iRepererTps = NSPhraseTemporel::TpsFutur;

            pTemps->decode(refCol, true);
        }
        // Date de fermeture
        else if (getSt() == "KFERM")
        {
            if (!pTemps)
            {
                pTemps = new NSPhraseTemporel(this);
                pTemps->ammorce();
            }
            Avance();
            //
            // Date passée d'un évènement ponctuel
            //
            pTemps->iTypeTps    = NSPhraseTemporel::TpsDate;
            pTemps->iFormeTps   = NSPhraseTemporel::TpsInstant;
            pTemps->iRepererTps = NSPhraseTemporel::TpsFutur;

            pTemps->decode(refCol, false);
        }
        // Type d'administration
        else if (getSt() == "0ADMI")
        {
            Avance();
            while ((getCol() > refCol) && iBon())
            {
            	NSPhraseObjet* pType = new NSPhraseObjet(this);
            	pType->ammorce();
            	Administration.push_back(new NSPhraseObjet(*pType));
            	delete pType;
                Avance();
            }
        }
        // Voie d'administration
        else if (getSt() == "0VADM")
        {
            Avance();
            while ((getCol() > refCol) && iBon())
            {
            	NSPhraseObjet* pType = new NSPhraseObjet(this);
            	pType->ammorce();
            	VoieAdministration.push_back(new NSPhraseObjet(*pType));
            	delete pType;
                Avance();
            }
        }
        else
            Recupere();
    }
    if (pTemps)
    {
        Temporalite.push_back(new NSPhraseTemporel(*pTemps));
        delete pTemps;
    }
	return;
}

void
NSPhrasePrise::metPhrase()
{
    NSPathologData Data;

    pPhraseur->initialise();
    *sDcodeur() = "";
    //
    // La première phase consiste à annoncer le mode d'administration,
    // sous forme d'un COD (ex, "réaliser une injection par voie IV")
    //
    //
    // Si la date a été saisie
    //
    // if (!(Administration.empty()))
    // {
    // 	iterPhraseObj i = Administration.begin();
    //    for (; i != Administration.end(); i++)
    //    	pPhraseur->COD.push_back(new NSPhraseMot((*i)->Objet.pDonnees, pContexte));
    // }
    if (pGenerateur->generePhrase(dcTiret))
    	*sDcodeur() = pGenerateur->getPhrase();
}

NSPhrasePrise::NSPhrasePrise(NSPhrasePrise& rv)
              :NSPhraseObjet(rv.pContexte), Temporalite(rv.pContexte)
{
	Objet       = rv.Objet ;
    iDcType     = rv.iDcType ;

    nbDose	   	= rv.nbDose ;
    ValeurDose  = rv.ValeurDose ;
    ObjetDose   = rv.ObjetDose ;

    Moment      = rv.Moment ;
    Temporalite = rv.Temporalite ;
}

NSPhrasePrise&
NSPhrasePrise::operator=(NSPhrasePrise src)
{
	Objet       = src.Objet ;
    iDcType     = src.iDcType ;

    nbDose	   	= src.nbDose ;
    ValeurDose  = src.ValeurDose ;
    ObjetDose   = src.ObjetDose ;

    Moment      = src.Moment ;
    Temporalite = src.Temporalite ;

    return *this;
}
*/

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSPhraseOrgane --------------------------
// -------------------------------------------------------------------------
NSPhraseOrgane::NSPhraseOrgane(NSContexte* pCtx, int iDecoType, string sLangue)
               :NSPhraseObjet(pCtx, iDecoType, sLangue)
{
}

NSPhraseOrgane::NSPhraseOrgane(decodageBase* pBase, int iDecoType, string sLangue)
               :NSPhraseObjet(pBase, iDecoType, sLangue)
{
}

NSPhraseOrgane::~NSPhraseOrgane()
{
}

void
NSPhraseOrgane::decode(int colonne)
{
  int refCol = getCol() ;

  while ((getCol() > colonne) && iBon())
	{
    // Date
    if 	    (getSt() == "KCHIR")
    {
      gereDate date(pContexte->getSuperviseur(), _sLangue) ;
      Avance() ;
      donneDate(refCol, &date) ;
      if (false == date.estVide())
      {
/*         	    pGeste->chir_date += date.getDate();
         	    string sMessage, sIntro;
         	    date.donne_date_breve(&sMessage, &sIntro);
				pGeste->chir_date_libelle = " " + sIntro + sMessage; */
      }
    }
    else
      Recupere() ;
  }
	return ;
}

void
NSPhraseOrgane::metPhrase()
{
  NSPathologData Data ;

  _pPhraseur->initialise() ;
  setDcodeur(string("")) ;
  //
  // La première phase consiste à annoncer l'existence de la lésion,
  // sous forme d'un COD (ex, "Il existe un polype")
  //
  NSPhraseMot Lesion(_Objet.getDataTank(), pContexte) ;
  Lesion.setArticle(NSPhraseMot::articleIndefini) ;
  //
  // Dans l'ordre, on décrit l'aspect, la dimension puis la localisation
  // (ex, "Il existe un polype sessile, de 3 mm, au niveau du sigmoïde"
  //
  /*bool bLesionComplement = false;
  //
  // L'aspect est un complément du nom de la lésion
  //
  if (!(Aspect.empty()))
    {
        if (!bLesionComplement)
        {
            Lesion.initComplement();
            bLesionComplement = true;
        }
        iterPhraseObj i = Aspect.begin();
        for (; i != Aspect.end(); i++)
        {
            string sLexique = string((*i)->Objet.pDonnees->lexique);
            trouve = pSuper->getDico()->trouvePathologData(&sLexique, &Data);
            if (trouve)
            {
                if (Data.estAdjectif())
                    Lesion.pComplement->adjEpithete.push_back(new NSPhraseMot((NSPathoBaseData*)(*i)->Objet.pDonnees, pContexte));
            }
        }
    } */
    //
    // La dimension est également un complément du nom de la lésion
    //

  //
  // La localisation est complément de lieu de la phrase
  //
  _pPhraseur->COD.push_back(new NSPhraseMot(Lesion)) ;
}

NSPhraseOrgane::NSPhraseOrgane(const NSPhraseOrgane& rv)
               :NSPhraseObjet(rv.pContexte)
{
  _Objet 			= rv._Objet;
  _iDcType 		= rv._iDcType;

  Dimensions	= rv.Dimensions;
  Lesions			= rv.Lesions;
  Aspect			= rv.Aspect;

  Temporalite = rv.Temporalite;
  Garbage     = rv.Garbage;
}

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSPhraseTemporel ------------------------
// -------------------------------------------------------------------------
NSPhraseTemporel::NSPhraseTemporel(NSContexte* pCtx, int iDecoType, string sLangue)
                 :NSPhraseObjet(pCtx, iDecoType, sLangue)
{
}

NSPhraseTemporel::NSPhraseTemporel(decodageBase* pBase, int iDecoType, string sLangue)
                 :NSPhraseObjet(pBase, iDecoType, sLangue)
{
}

NSPhraseTemporel::~NSPhraseTemporel()
{
}

void
NSPhraseTemporel::decode(int colonne, bool bMinValue, TIME_EVENT iEvnt)
{
  NSPatPathoInfo PPTinfo ;

  TIME_EVENT iEvntType = iTypeTps ;
  // Forcing
  if (TpsTypeUndefined != iEvnt)
    iEvntType = iEvnt ;

  while ((getCol() > colonne) && iBon())
  {
    //
    PPTinfo = *(*(getitDcode())) ;

    string sLexique = PPTinfo.getLexique() ;
    if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
      if (TpsDate == iEvntType)
      {
        if (bMinValue)
          ValeurMin = PPTinfo ;
        else
          ValeurMax = PPTinfo ;
      }
      else
        ValeurDuree = PPTinfo ;
      Avance();
    }
    else
      Recupere() ;
  }
	return ;
}

void
NSPhraseTemporel::metPhrase()
{
  NSPathologData Data ;

  _pPhraseur->initialise() ;

  NSPhraseMot* pMot = new NSPhraseMot(_Objet.getDataTank(), pContexte) ;

  _pPhraseur->COD.push_back(pMot) ;

  NSPhraseMotTime* pDate = new NSPhraseMotTime(pContexte) ;

  pDate->initMin(&ValeurMin) ;
  pDate->setMinNow(bMinNow) ;

  if (string("") != pDate->getFormatMin())
    pDate->setFormat(pDate->getFormatMin()) ;

  pDate->initMax(&ValeurMax) ;
  pDate->setMaxNow(bMaxNow) ;

  if (string("") != pDate->getFormatMax())
    pDate->setFormat(pDate->getFormatMax()) ;

  _pPhraseur->CCTemps.push_back(pDate) ;

  if (_pGenerateur->genereProposition(dcTiret))
    setDcodeur(_pGenerateur->getPropositionPhrase()) ;
}

void
NSPhraseTemporel::initPhraseMotTime(NSPhraseMotTime* pPMT)
{
  if ((NSPhraseMotTime*) NULL == pPMT)
    return ;

  pPMT->setTypeTps(iTypeTps) ;
  pPMT->setFormeTps(iFormeTps) ;
  pPMT->setRepererTps(iRepererTps) ;

  // Min time
  //
  pPMT->initMin(&ValeurMin) ;
  pPMT->setMinNow(bMinNow) ;

  //
  // Max time
  //
  pPMT->initMax(&ValeurMax) ;
  pPMT->setMaxNow(bMaxNow) ;

  //
  // Durée (3 mois...)
  //
  pPMT->initDuree(&ValeurDuree) ;
}

NSPhraseTemporel::NSPhraseTemporel(const NSPhraseTemporel& rv)
                 :NSPhraseObjet(rv.pContexte)
{
	_Objet 		  = rv._Objet ;
  _iDcType 	  = rv._iDcType ;

  iTypeTps    = rv.iTypeTps ;
  iFormeTps   = rv.iFormeTps ;
  iRepererTps = rv.iRepererTps ;

  ValeurMin   = rv.ValeurMin ;
  bMinNow     = rv.bMinNow ;

  ValeurMax   = rv.ValeurMax ;
  bMaxNow     = rv.bMaxNow ;
}

NSPhraseTemporel&
NSPhraseTemporel::operator=(const NSPhraseTemporel& src)
{
  if (&src == this)
    return *this ;

	_Objet 		  = src._Objet ;
	_iDcType 	  = src._iDcType ;

	iTypeTps    = src.iTypeTps ;
	iFormeTps   = src.iFormeTps ;
	iRepererTps = src.iRepererTps ;

	ValeurMin   = src.ValeurMin ;
	bMinNow     = src.bMinNow ;

	ValeurMax   = src.ValeurMax ;
	bMaxNow     = src.bMaxNow ;

	return *this ;
}

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSPhraseTempoCycle ----------------------
// -------------------------------------------------------------------------
NSPhraseTempoCycle::NSPhraseTempoCycle(NSContexte* pCtx, int iDecoType, string sLangue)
                    :NSPhraseObjet(pCtx, iDecoType, sLangue)
{
}

NSPhraseTempoCycle::NSPhraseTempoCycle(decodageBase* pBase, int iDecoType, string sLangue)
                    :NSPhraseObjet(pBase, iDecoType, sLangue)
{
}

NSPhraseTempoCycle::~NSPhraseTempoCycle()
{
}

void
NSPhraseTempoCycle::decode(int colonne)
{
	int refCol = getCol() ;

	NSPatPathoInfo PPTinfo ;

	while ((getCol() > colonne) && iBon())
	{
		// Durée de l'événement
    //
    if      (getSt() == "KDURA")
    {
    	Avance() ;

      while ((getCol() > refCol) && iBon())
      {
      	PPTinfo = *(*(getitDcode())) ;

        string sLexique = PPTinfo.getLexique() ;
        if ((string("") != sLexique) && ('£' == sLexique[0]))
        {
        	eventDuration = PPTinfo ;
          Avance() ;
        }
        else
        	Recupere() ;
      }
    }
    // Durée du cycle
    //
    else if (getSt() == "KDURC")
    {
    	Avance() ;

      while ((getCol() > refCol) && iBon())
      {
      	PPTinfo = *(*(getitDcode())) ;

        string sLexique = PPTinfo.getLexique() ;
        if ((string("") != sLexique) && ('£' == sLexique[0]))
        {
        	cycleDuration = PPTinfo ;
          Avance() ;
        }
        else
        	Recupere() ;
      }
    }
    else if (getSt() == "VAINC")
    {
    	Avance() ;

      while ((getCol() > refCol) && iBon())
      {
        PPTinfo = *(*(getitDcode())) ;
        gereNum numNbEvent(pContexte->getSuperviseur(), _sLangue) ;
        NSPatPathoInfo nbEventInfo ;
        donneDimension(refCol, &numNbEvent) ;

        if ((iBon()) && (numNbEvent.getUnite() == "2FOIS1"))
            iEventNb = int(numNbEvent.getValeur()) ;

        /*
        if ((PPTinfo.pDonnees->lexique)[0] == '£')
        {
            nbEventInfo = PPTinfo ;
            Avance() ;
        }
        else
            Recupere() ;

        string sEventCplmt  = nbEventInfo.getComplement() ;
        string sEventUnit   = nbEventInfo.getUnit() ;
        string sEventFrmt   = nbEventInfo.getLexique() ;
        numNbEvent.instancier(&sEventCplmt, &sEventUnit, &sEventFrmt) ;
        */
      }
    }
    else
    	Recupere() ;
	}
	return ;
}

void
NSPhraseTempoCycle::metPhrase()
{
  NSPathologData Data ;

  _pPhraseur->initialise() ;

  if (_pGenerateur->genereProposition(dcTiret))
    setDcodeur(_pGenerateur->getPropositionPhrase()) ;
}

void
NSPhraseTempoCycle::initCycle(NSPhraseMotTimeCycle* pCycle)
{
	if ((NSPhraseMotTimeCycle*) NULL == pCycle)
		return ;

	if (iTypeCycle == Regulier)
		pCycle->setCycleType(NSPhraseMotTimeCycle::CycRegular) ;
	if (iTypeCycle == Libre)
		pCycle->setCycleType(NSPhraseMotTimeCycle::CycFree) ;

	pCycle->setCycleDurationValue(cycleDuration.getComplement()) ;
	pCycle->setCycleDurationFormat(cycleDuration.getLexique()) ;
	pCycle->setCycleDurationUnit(cycleDuration.getUnit()) ;

	pCycle->setActionDurationValue(eventDuration.getComplement()) ;
	pCycle->setActionDurationFormat(eventDuration.getLexique()) ;
	pCycle->setActionDurationUnit(eventDuration.getUnit()) ;

	StringToInt(pCycle->getNumberOfAction()) ;
}

bool
NSPhraseTempoCycle::estVide()
{
	if ((string("") == eventDuration.getLexique()) &&
      (string("") == cycleDuration.getLexique()))
		return true ;
	return false ;
}

NSPhraseTempoCycle::NSPhraseTempoCycle(const NSPhraseTempoCycle& rv)
                   :NSPhraseObjet(rv.pContexte)
{
	_Objet        = rv._Objet ;
  _iDcType      = rv._iDcType ;

  iTypeCycle    = rv.iTypeCycle ;

  eventDuration = rv.eventDuration ;
  cycleDuration = rv.cycleDuration ;
  iEventNb      = rv.iEventNb ;
}

NSPhraseTempoCycle&
NSPhraseTempoCycle::operator=(const NSPhraseTempoCycle& src)
{
  if (this == &src)
  	return *this ;

  _Objet 		    = src._Objet ;
  _iDcType 	    = src._iDcType ;

  iTypeCycle    = src.iTypeCycle ;

  eventDuration = src.eventDuration ;
  cycleDuration = src.cycleDuration ;
  iEventNb      = src.iEventNb ;

  return *this ;
}

// -------------------------------------------------------------------------
// ------------------- METHODES DE NSPhraseBiometrie -----------------------
// -------------------------------------------------------------------------
NSPhraseBiometrie::NSPhraseBiometrie(NSContexte* pCtx, int iDecoType, string sLangue)
                  :NSPhraseObjet(pCtx, iDecoType, sLangue)
{
}

NSPhraseBiometrie::NSPhraseBiometrie(decodageBase* pBase, int iDecoType, string sLangue)
                  :NSPhraseObjet(pBase, iDecoType, sLangue)
{
}

void
NSPhraseBiometrie::decode(int colonne)
{
  NSPatPathoInfo PPTinfo ;

  while ((getCol() > colonne) && iBon())
	{
    //
    PPTinfo = *(*(getitDcode())) ;

    string sLexique = PPTinfo.getLexique() ;
    if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
      Valeur = PPTinfo ;
      Avance() ;
    }
    else
      Recupere() ;
  }
  return ;
}

void
NSPhraseBiometrie::metPhrase()
{
  setDcodeur(string("")) ;

  NSPathologData Data ;

  _pPhraseur->initialise() ;

  NSPhraseMot* pMot = new NSPhraseMot(_Objet.getDataTank(), pContexte) ;
  pMot->setComplement(Valeur.getComplement()) ;
  pMot->setFormat(Valeur.getLexique()) ;
  pMot->setUnite(Valeur.getUnit()) ;
  pMot->setNumForme(NSPhraseMot::numTiret) ;

  _pPhraseur->CCChiffre.push_back(pMot) ;

  if (_pGenerateur->genereProposition(dcTiret))
    setDcodeur(_pGenerateur->getPropositionPhrase()) ;
}

NSPhraseBiometrie::NSPhraseBiometrie(const NSPhraseBiometrie& rv)
                  :NSPhraseObjet(rv.pContexte)
{
	_Objet 		   = rv._Objet ;
  _iDcType 	   = rv._iDcType ;

  Valeur       = rv.Valeur ;
  Normales     = rv.Normales ;

  Methode      = rv.Methode ;
  Localisation = rv.Localisation ;

  Temporalite  = rv.Temporalite ;
  Garbage      = rv.Garbage ;
}

NSPhraseBiometrie&
NSPhraseBiometrie::operator=(const NSPhraseBiometrie& src)
{
  if (this == &src)
  	return *this ;

  _Objet 		   = src._Objet ;
  _iDcType 	   = src._iDcType ;

  Valeur       = src.Valeur ;
  Normales     = src.Normales ;

  Methode      = src.Methode ;
  Localisation = src.Localisation ;

  Temporalite  = src.Temporalite ;
  Garbage      = src.Garbage ;

  return *this ;
}

// *************************************************************************
// -------------------- METHODES DE NSPhraseObjArray -----------------------
// *************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPhraseObjArray::NSPhraseObjArray(const NSPhraseObjArray& rv)
                 :NSPhraObjArray()
{
try
{
	if (rv.empty())
    return ;

  for (iterConstPhraseObj i = rv.begin() ; rv.end() != i ; i++)
    push_back(new NSPhraseObjet(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSPhraseObjArray copy ctor", standardError) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSPhraseObjArray::vider()
{
  if (empty())
    return ;

  for (iterPhraseObj i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSPhraseObjArray::~NSPhraseObjArray()
{
  vider() ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPhraseObjArray&
NSPhraseObjArray::operator=(const NSPhraseObjArray& src)
{
try
{
	if (this == &src)
  	return *this ;

	//
  // Effacement des éléments déjà contenus dans le vecteur destination
  //
  vider() ;

	//
  // Copie et insertion des éléments de la source
  //
	if (false == src.empty())
  	for (iterConstPhraseObj i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSPhraseObjet(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSPhraseObjArray = operator", standardError) ;
	return *this ;
}
}

