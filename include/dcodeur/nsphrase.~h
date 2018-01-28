#ifndef __NSPHRASE_H
#define __NSPHRASE_H

class NSPhraseur ;
class NSPhraseurArray ;
class NSGenerateur ;
class NSPatPathoData ;
class NSPatPathoInfo ;
class NSPathologData ;

class gereNum ;
class gereDate ;
class gereHeure ;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "partage/ns_vector.h"
#include "nssavoir/nspatbas.h"
#include "partage/NTArray.h"

#ifndef __linux__
#if defined(_DKD_DLL)
	#define _NSDKDCLASSE __export
#else
	#define _NSDKDCLASSE __import
#endif
#endif

enum PHRTYPE { phrasePrincipale = 1, phraseComplement, phraseRelative} ;

//
// Enumeration des modes de decodage
//
enum DCODETYPE { dcUndefined, dcPhrase, dcTiret } ;

#ifndef __linux__
class _NSDKDCLASSE NSPhraseMot : public NSRoot
#else
class NSPhraseMot : public NSRoot
#endif
{
  public :

  enum TYPEARTICLE    { articleSans = 1, articleDefini, articleIndefini, articlePartitif} ;

  enum FORCEPOSITION  { posStandard = 1, posAvant, posApres } ;
  enum COMPOSERLINK   { linkUndefined = 1, linkNone, linkSpace, linkMinus } ;

  enum NUMFORME       { numTiret = 1, numSimple, numRelative } ;

  enum NAMECMPLTYPE   { cpltUndefined = 1, cpltNature, cpltAppartenance, cpltUsage } ;
  enum COMPLEMENTMNGT { complementNormal = 1, complementAppartenanceNoRepeat } ;

  protected :

    // Les preposition "non traduisibles" comme EN decubitus ou A velo
    // n'ont pas d'interet a etre traitees avec le lexique puisqu'elles
    // sont artificielles
    string _sPreposition ;
    //
    // "dans la rue" mais "en decubitus"
    //

    // Le mot peut-être modifié en mot composé avec un modificateur comme
    // sus, sous, retro, etc. Exemple sous-rénal.
    //
    string        _sComposer ;
    FORCEPOSITION _iComposerPos ;
    COMPOSERLINK  _iComposerLink ;

    // On a une string pour les textes libres.
    string _sTexteLibre ;

    TYPEARTICLE _avecArticle ;

    //
    // Pour forcer la position d'un adjectif par rapport au nom.

    FORCEPOSITION _posForcee ;

    //
    // Variables de stockage des valeurs
    //
    // char lexique[BASE_LEXIQUE_LEN + 1] ;
    // char complement[BASE_COMPLEMENT_LEN + 1] ;
    // char certitude[BASE_CERTITUDE_LEN + 1] ;
    // char pluriel[BASE_PLURIEL_LEN + 1] ;

    string _sLexique ;
    string _sComplement ;
    string _sCertitude ;
    string _sPluriel ;

    // Pour les valeurs chiffrees
    // char format[BASE_LEXIQUE_LEN + 1] ;
    // char methode[BASE_LEXIQUE_LEN + 1] ;
    // char unite[BASE_LEXIQUE_LEN + 1] ;

    string _sFormat ;
    string _sMethode ;
    string _sUnite ;

    NUMFORME _numForme ;

    // Typologie de ce mot si il est complément du nom
    //
    // cpltNature :       un banc de bois
    // cpltAppartenance : le banc de la cuisine, la hauteur de l'anévrysme
    // cpltUsage :        la planche à repasser
    //
    NAMECMPLTYPE _iTypeAsComplement ;

    // Compléments de ce nom
    //
    // Complement normal ou remplacé par un article possessif ("la voiture de Pierre" ou "sa voiture")
    COMPLEMENTMNGT _iComplementManagement ;

    NSPhraseurArray* _pComplements ;

    // Priorite de cet element dans la phrase
    //
    int _iPriorite ;

    // By default, this information is not set, it becomes instanciated
    // once function getLexiqueData has been called 
    //
    NSPathologData* _pLexiqueData ;
    string          _sLangForData ;

    // Category in order to group common elements in enumerations
    // Formerly it was automatically postives versus negatives, but it could
    // be more logical to group physiological vers pathological, for example
    //
    string          _sEnumCategory ;

    static long lBaseObjectCount ;

  public :

    NSPhraseMot(NSContexte* pCtx, string sEtiquette = string("")) ;
    NSPhraseMot(const NSPhraseMot& rv) ;
    // NSPhraseMot(NSPathoBaseData* pPathoBaseElement, NSContexte* pCtx);
    NSPhraseMot(const NSPatPathoData* pPatPathoElement, NSContexte* pCtx) ;
    virtual ~NSPhraseMot() ;

    void InitFromEtiquette(const string sEtiquette) ;
    void initFromNum(const gereNum* pNum) ;

    void setPreposition(const string sPrep)     { _sPreposition = sPrep ; }
    void setComposer(const string sComposer)    { _sComposer = sComposer ; }
    void setCompPos(const FORCEPOSITION iCP)    { _iComposerPos = iCP ; }
    void setCompLnk(const COMPOSERLINK iCL)     { _iComposerLink = iCL ; }
    void setArticle(const TYPEARTICLE iArticle) { _avecArticle = iArticle ; }
    void setLexique(const string sLexi)         { _sLexique = sLexi ; }
    void setComplement(const string sCompl)     { _sComplement = sCompl ; }
    void setCertitude(const string sCert)       { _sCertitude = sCert ; }
    void setPluriel(const string sPlur)         { _sPluriel = sPlur ; }
    void setFormat(const string sForm)          { _sFormat = sForm ; }
    void setMethode(const string sMetho)        { _sMethode = sMetho ; }
    void setUnite(const string sUnit)           { _sUnite = sUnit ; }
    void setPosition(const FORCEPOSITION iPos)  { _posForcee = iPos ; }
    void setTexteLibre(const string sTexte)     { _sTexteLibre = sTexte ; }
    void setNumForme(const NUMFORME iforme)     { _numForme = iforme ; }
    void setPriority(const int iPrior)          { _iPriorite = iPrior ; }
    void setCplmtMgnt(const COMPLEMENTMNGT iM)  { _iComplementManagement = iM ; }
    void setTypeAsCplmt(const NAMECMPLTYPE iT)  { _iTypeAsComplement = iT ; }
    void setEnumCategory(const string sEC)      { _sEnumCategory = sEC ; }

    string         getPreposition() const  { return _sPreposition ; }
    string         getComposer() const     { return _sComposer ; }
    FORCEPOSITION  getCompPos() const      { return _iComposerPos ; }
    COMPOSERLINK   getCompLnk() const      { return _iComposerLink ; }
    TYPEARTICLE    getArticle() const      { return _avecArticle ; }
    string         getLexique() const      { return _sLexique ; }
    string         getComplement() const   { return _sComplement ; }
    string         getCertitude() const    { return _sCertitude ; }
    string         getPluriel() const      { return _sPluriel ; }
    string         getFormat() const       { return _sFormat ; }
    string         getMethode() const      { return _sMethode ; }
    string         getUnite() const        { return _sUnite ; }
    FORCEPOSITION  getPosition() const     { return _posForcee ; }
    string         getTexteLibre() const   { return _sTexteLibre ; }
    int            getNumForme() const     { return _numForme ; }
    int            getPriority() const     { return _iPriorite ; }
    COMPLEMENTMNGT getCplmtMgnt() const    { return _iComplementManagement ; }
    NAMECMPLTYPE   getTypeAsCplmt() const  { return _iTypeAsComplement ; }
    string         getEnumCategory() const { return _sEnumCategory ; }

    string         getConcept() const      { return (string("") != _sLexique) ? _sLexique : _sUnite ; }

    void             addComplementPhr(NSPhraseur* pCompl, PHRTYPE iPhraseType = phraseComplement) ;
    NSPhraseurArray* getComplementPhr() { return _pComplements ; }
    NSPhraseur*      getFirstComplementPhr() ;
    NSPhraseur*      getOrCreateFirstComplementPhr() ;
    void             setComplementPhr(NSPhraseurArray* pPA) { _pComplements = pPA ; }

    bool           estTexteLibre() const ;

    string       forceNombre(NSGenerateur* pGener) ;

    NSPhraseMot& operator=(const NSPhraseMot& src) ;
    int          operator==(const NSPhraseMot& o) ;

    virtual void metAZero() ;
    void         initComplement() ;

    bool         estVide() const ;

    NSPathologData* getLexiqueData(const string sLang) ;

    static long getNbInstance()  { return lBaseObjectCount ; }
    static void initNbInstance() { lBaseObjectCount = 0 ; }
};

enum TIME_EVENT { TpsTypeUndefined = 0, TpsDuration, TpsDate } ;  // Dure 2h ou Dans 2h
enum TIME_FORM  { TpsFormUndefined = 0, TpsInstant, TpsInterval } ; //
enum TIME_FRAME { TpsFrameUndefined = 0, TpsPresent, TpsPast, TpsFutur } ;

#ifndef __linux__
class _NSDKDCLASSE NSPhraseMotTime : public NSPhraseMot
#else
class NSPhraseMotTime : public NSPhraseMot
#endif
{
  public :

    //
    // Duree ou Date : pendant 2 heures vs dans 2 heures
    // Duration or date : lasting 2 hours vs starting 2 hours from now
    //
    // Pour une date precise, alors mettre valeurMin == valeurMax
    // For a sharp date, then set valeurMin == valeurMax
    //
    TIME_EVENT _iTypeTps ;

    //
    // Temps ponctuel ou intervalle de temps
    // Point or Duration
    //
    TIME_FORM _iFormeTps ;

    //
    // Now, in the past or in the future
    //
    TIME_FRAME _iRepererTps ;

    //
    // Time position related to an event (event = lexique)
    //
    enum TIME_POSITION { TpsPositionNotDefined = 0,
                         EventFarBefore, EventBefore, EventJustBefore,
                         EventDuring,
                         EventJustAfter, EventAfter, EventFarAfter,
                         EventFarFrom } ;
    TIME_POSITION _iEventTps ;

    //
    // Min time
    //
    string _sValeurMin ;   // Valeur min - Min Value
    string _sUniteMin ;    // Unite min  - Min value unit
    string _sFormatMin ;
    //
    // Le temps min est "maintenant"    : de maintenant a plus tard
    // Min time is "now"                : from now to max time
    //
    bool _bMinNow ;

    //
    // Max time
    //
    string _sValeurMax ;   // Valeur max  - Max Value
    string _sUniteMax ;    // Unite max   - Max value unit
    string _sFormatMax ;
    //
    // Le temps max est "maintenant"    : d'une date a maintenant
    // Max time is "now"                : from min time to now
    //
    bool _bMaxNow ;

    //
    // Duree (3 mois...)
    //
    string _sValeurDuree ; // Valeur max  - Max Value
    string _sUniteDuree ;  // Unite max   - Max value unit
    string _sFormatDuree ;

    NSPhraseMotTime(NSContexte* pCtx) ;
    NSPhraseMotTime(const NSPhraseMotTime& rv) ;
    // NSPhraseMot(NSPathoBaseData* pPathoBaseElement, NSContexte* pCtx);
    NSPhraseMotTime(const NSPatPathoData* pPatPathoElement, NSContexte* pCtx) ;
    ~NSPhraseMotTime() ;

    NSPhraseMotTime& operator=(const NSPhraseMotTime& src) ;
    int              operator==(const NSPhraseMotTime& o) ;

    void metAZero() ;
    void setSharpDate() ;
    bool minEgalMax() ;
    bool estVide() ;

    TIME_EVENT    getTypeTps() const    { return _iTypeTps ;    }
    TIME_FORM     getFormeTps() const   { return _iFormeTps ;   }
    TIME_FRAME    getRepererTps() const { return _iRepererTps ; }
    TIME_POSITION getEventTps() const   { return _iEventTps ;   }

    string getValeurMin() const { return _sValeurMin ; }
    string getUniteMin() const  { return _sUniteMin ;  }
    string getFormatMin() const { return _sFormatMin ; }

    bool   isMinNow() const     { return _bMinNow ; }

    string getValeurMax() const { return _sValeurMax ; }
    string getUniteMax() const  { return _sUniteMax ;  }
    string getFormatMax() const { return _sFormatMax ; }

    bool   isMaxNow() const     { return _bMaxNow ; }

    string getValeurDuree() const { return _sValeurDuree ; }
    string getUniteDuree() const  { return _sUniteDuree ;  }
    string getFormatDuree() const { return _sFormatDuree ; }

    void   initDuree(NSPatPathoInfo* pPatho) ;
    void   initMin(NSPatPathoInfo* pPatho) ;
    void   initMax(NSPatPathoInfo* pPatho) ;

    void   initDuree(gereHeure* pHeure) ;
    void   initMin(gereHeure* pHeure) ;
    void   initMax(gereHeure* pHeure) ;

    void   initMin(gereDate* pDate) ;
    void   initMax(gereDate* pDate) ;

    void   setTypeTps(const TIME_EVENT iTypeTps)   { _iTypeTps    = iTypeTps ;  }
    void   setFormeTps(const TIME_FORM iFormeTps)  { _iFormeTps   = iFormeTps ; }
    void   setRepererTps(const TIME_FRAME iReTps)  { _iRepererTps = iReTps ;    }
    void   setEventTps(const TIME_POSITION iEvTps) { _iEventTps   = iEvTps ;    }

    void   setMaxNow(const bool bMN)               { _bMaxNow     = bMN ;       }
    void   setMinNow(const bool bMN)               { _bMinNow     = bMN ;       }

  protected:

    void   initTime(NSPatPathoInfo* pPatho, string &sValue, string &sUnit, string &sFormat) ;
    void   initTime(gereHeure* pHeure, string &sValue, string &sUnit, string &sFormat) ;
    void   initTime(gereDate* pDate, string &sValue, string &sUnit, string &sFormat) ;
};

#ifndef __linux__
class _NSDKDCLASSE NSPhraseMotTimeCycle : public NSPhraseMot
#else
class NSPhraseMotTimeCycle : public NSPhraseMot
#endif
{
	public :

		//
    // Cycle regulier ou libre :
    // Regulier : |XXX_____|XXX_____|XXX_____ : duree cycle + duree action
    // Libre    : |XX__XX__|XX_XX___|_XX__XX_ : duree cycle + duree action + nb actions dans un cycle
    //
    // Regular or free cycle :
    // Regular : |XXX_____|XXX_____|XXX_____ : cycle duration + action duration
    // Free    : |XX__XX__|XX_XX___|_XX__XX_ : cycle duration + action duration + nb of actions in a cycle
    //
    enum CYCLETYPE { CycNotInit = 0, CycRegular, CycFree } ;
    CYCLETYPE _iCycleType ;

    //
    // Cycle duration time
    //
    string _sCycleDurationValue ;
    string _sCycleDurationUnit ;
    string _sCycleDurationFormat ;
    //
    // Action duration time
    //
    string _sActionDurationValue ;
    string _sActionDurationUnit ;
    string _sActionDurationFormat ;
    //
    // Action duration time
    //
    string _sNumberOfAction ;

    NSPhraseMotTimeCycle(NSContexte* pCtx) ;
    NSPhraseMotTimeCycle(const NSPhraseMotTimeCycle& rv) ;
    // NSPhraseMot(NSPathoBaseData* pPathoBaseElement, NSContexte* pCtx);
    NSPhraseMotTimeCycle(const NSPatPathoData* pPatPathoElement, NSContexte* pCtx) ;
    ~NSPhraseMotTimeCycle() ;

    NSPhraseMotTimeCycle& operator=(const NSPhraseMotTimeCycle& src) ;
    int                   operator==(const NSPhraseMotTimeCycle& o) ;

    void metAZero() ;
    bool estVide() ;

    string getCycleDurationValue()   { return _sCycleDurationValue ;   }
    string getCycleDurationUnit()    { return _sCycleDurationUnit ;    }
    string getCycleDurationFormat()  { return _sCycleDurationFormat ;  }

    CYCLETYPE getCycleType()         { return _iCycleType ; }

    string getActionDurationValue()  { return _sActionDurationValue ;  }
    string getActionDurationUnit()   { return _sActionDurationUnit ;   }
    string getActionDurationFormat() { return _sActionDurationFormat ; }

    string getNumberOfAction()       { return _sNumberOfAction ;       }

    void   setCycleDurationValue(string sCD)  { _sCycleDurationValue  = sCD ; }
    void   setCycleDurationUnit(string sCD)   { _sCycleDurationUnit   = sCD ; }
    void   setCycleDurationFormat(string sCD) { _sCycleDurationFormat = sCD ; }

    void   setCycleType(CYCLETYPE iCT)        { _iCycleType           = iCT ; }

    void   setActionDurationValue(string sAD)  { _sActionDurationValue  = sAD ; }
    void   setActionDurationUnit(string sAD)   { _sActionDurationUnit   = sAD ; }
    void   setActionDurationFormat(string sAD) { _sActionDurationFormat = sAD ; }

    void   setNumberOfAction(string sNoA)      { _sNumberOfAction     = sNoA ; }
};

typedef vector<NSPhraseMot*> NSPhraMotArray ;
typedef NSPhraMotArray::iterator       iterPhraseMot ;
typedef NSPhraMotArray::const_iterator iterConstPhraseMot ;

#ifndef __linux__
class _NSDKDCLASSE NSPhraseMotArray : public NSPhraMotArray
#else
class NSPhraseMotArray : public NSPhraMotArray
#endif
{
  public :

    // Elements to be grouped:
    //   - are simply grouped together
    //   - groups are separated by "but"
    //
    enum TYPEGRPENUM  { groupUnspecified = 0, groupSimple, groupBut, groupMixedWithNon } ;
    enum TYPEGRPSEPA  { groupSeparComaAnd = 0, groupSeparNone } ;

    // Constructeurs
    NSPhraseMotArray() ;
    NSPhraseMotArray(const NSPhraseMotArray& rv) ;
    // Destructeur
    virtual ~NSPhraseMotArray() ;
    NSPhraseMotArray& operator=(const NSPhraseMotArray& src) ;
    //
    void vider() ;

    void initPreposition() ;

    TYPEGRPENUM getGroupType() const        { return _iGroupType ; }
    TYPEGRPSEPA getGroupSeparator() const   { return _iGroupSeparator ; }
    bool        isUsualSeparator() const    { return groupSeparComaAnd == _iGroupSeparator ; }
    string      getEnumerationOrder() const { return _sEnumerationOrder ; }

    void setGroupType(const TYPEGRPENUM iGT)      { _iGroupType        = iGT ; }
    void setGroupSeparator(const TYPEGRPSEPA iGS) { _iGroupSeparator   = iGS ; }
    void setEnumerationOrder(const string sEO)    { _sEnumerationOrder = sEO ; }

    void setParameters(const NSPhraseMotArray* pModel) ;

  protected :

    // How concepts should be grouped depending on their _sEnumCategory,
    // for example "physio/patho" (the separator being the '/' char)
    //
    TYPEGRPENUM _iGroupType ;
    TYPEGRPSEPA _iGroupSeparator ;
    string      _sEnumerationOrder ;
};

#ifndef __linux__
class _NSDKDCLASSE NSPhraseur : public NSRoot
#else
class NSPhraseur : public NSRoot
#endif
{
  public :

    enum TYPEARTICLE  { articleSans = 1, articleDefini, articleIndefini} ;
    enum ADJNONQUALIF { adjectifSans = 1, adjectifPossessif, adjectifDemonstratif} ;

    // Qualification de la localisation (CC de lieu)
    // Caractéristique descriptive : la longueur "de la" jambe est de x cm
    // Localisation niveau : il existe un polype "au niveau du" côlon
    // Localisation à : il existe un ulcère "à" la jambe droite
    // Localisation de : il existe un ulcère à 10 cm "de" la jambe droite
    //
    enum TYPELOCALISATION { locUndefined = 0, locCaractDescr, locNiveau, locA, locFrom} ;

    // Phrase principale ou complement d'un nom
    // enum PHRTYPE { phrasePrincipale = 1, phraseComplement, phraseRelative} ;
    PHRTYPE   iPhraseType ;
    DCODETYPE _iRelativeType ;  // Un anévrysme dont l'origine est sus-rénale vs Un anévrysme d'origine sus-rénale

    // Forme active ou passive
    enum PHRFORME { formeActive = 1, formePassive } ;
    PHRFORME iForme ;

    NSPhraseMotArray Sujet ;

    // Sujet normal ou remplace par un article ou totalement supprimé
    enum SUJTYPE { sujetNormal = 1, sujetNoRepeat, sujetRemove } ;
    SUJTYPE iTypeSujet;

    NSPhraseMotArray Verbe ;
    // Caracteristiques du verbe

    enum VBTYPE { vbTypeNotInit = 0, vbTypeAction, vbTypeEtat} ;
    enum VBETAT { vbEtatNotInit = 0, vbEtatReel, vbEtatApparent, vbEtatDure, vbEtatChange} ;
    VBTYPE iVbType ;
    VBETAT iVbEtat ;

    // Emploi du verbe : temps, aspect, mode, personne, negation
    enum VBASPEC { aspectPonctuel = 1, aspectIntervalle} ;
    enum VBTEMPS { tempsPasseRevolu = 1, tempsPasseActuel,
                   tempsPresent, tempsFuturProche, tempsFuturLointain} ;
    enum VBMODE  { modeIndicatif = 1, modeSubjonctif, modeImperatif,
                   modeConditionnel, modeInfinitif, modeParticipe,
                   modeGerondif} ;
    enum VBPERSO { pers1S = 1, pers2S, pers3S, pers1P, pers2P, pers3P} ;
    enum VBNEGAT { negationOui = 1, negationNon} ;
    enum VBPREPOCTX { prepUndefined = 0, prepLieu = 1, prepTemps, prepManiere, prepMoyen,
              prepCause, prepBut, prepType, prepHypoth, prepChiffre} ;
    VBTEMPS iVbTemps ;
    VBASPEC iVbAspect ;
    VBMODE  iVbMode ;
    VBPERSO iVbPersonne ;
    VBNEGAT iVbNegation ;

    // Explications concernant un mot entre parentheses.

    NSPhraseur* pParentheses ;
    DCODETYPE   iParenthesesStyle ;

    // Complements essentiels
    NSPhraseMotArray    COD ;       // Complement d'Objet Direct
    NSPhraseMotArray    COI ;       // Complement d'Objet Indirect
    NSPhraseMotArray    COS ;       // Complement d'Objet Second
    NSPhraseMotArray    AttSujet ;  // Attribut du sujet
                                    // "L'homme paraissait fatigué"
                                    // "Sa démarche était lourde"
    NSPhraseMotArray    AttCOD ;    // Attribut du COD
                                    // "J'ai trouve vos propositions intéressantes"

    // Complements circonstanciels : quand, ou, comment, avec quoi, pourquoi,
    // dans quel but
    TYPELOCALISATION    iTypeLocalisation ;
    NSPhraseMot         LiaisonLieu, PrepositionLieu ;
    NSPhraseMotArray    CCLieu ;    // Complement circonstanciel de lieu

    NSPhraseMot         LiaisonTemps, PrepositionTemps ;
    NSPhraseMotArray    CCTemps ;   // Complement circonstanciel de temps

    NSPhraseMot         LiaisonManiere, PrepositionManiere ;
    NSPhraseMotArray    CCManiere ; // Complement circonstanciel de maniere
                                    // Il mange avec delicatesse

    NSPhraseMot         LiaisonMoyen, PrepositionMoyen ;
    NSPhraseMotArray    CCMoyen ;   // Complement circonstanciel de moyen
                                    // Il mange avec des couverts en argent

    NSPhraseMot         LiaisonCause, PrepositionCause ;
    NSPhraseMotArray    CCCause ;   // Complement circonstanciel de cause
                                    // Il tremble de peur

    NSPhraseMot         LiaisonBut, PrepositionBut ;
    NSPhraseMotArray    CCBut ;     // Complement circonstanciel de but
                                    // Il court pour maigrir

    NSPhraseMot         LiaisonType, PrepositionType ;
    NSPhraseMotArray    CCType ;    // Complement circonstanciel de type
                                    // Douleur thoracique a type de douleur angineuse.

    NSPhraseMot         LiaisonHypoth, PrepositionHypoth ;
    NSPhraseMotArray    CCHypoth ;  // Complement circonstanciel d'hypothese
                                    // A prendre en cas de douleur.

    NSPhraseMot         LiaisonChiffre, PrepositionChiffre ;
    NSPhraseMotArray    CCChiffre ; // Valeur chiffree + valeur normale

    // Complement de la phrase apres ":".
    //
    NSPhraseur*         pDeuxPoints ;

    //
    // ------------- Variables specifiques des complements --------------
    //
    NSPhraseMotArray    adjEpithete ;           // Epithete "Une belle journée"
    NSPhraseMotArray    adjEpitheteAv ;         // Epithete "Une belle journée"
    NSPhraseMotArray    adjEpitheteAp ;         // Epithete "Une maison rouge"
    NSPhraseMot         adjNumeralCardinal ;    // Adjectif numeral cardinal : "3 pommes"
    NSPhraseMot         adjNumeralOrdinal ;     // Adjectif numeral ordinal : "la troisième pomme"
    ADJNONQUALIF        adjNonQualificatif ;    // Adjectif possessif "Sa maison", démonstratif "Cette maison"

    // Complement du nom
    NSPhraseMotArray    compNom ;               // "Suspicion d'endocartite"

    // Adverbes qui peuvent completer un adjectif, un verbe ou un adverbe.

    NSPhraseMotArray    adverbe ;               // "un homme très grand", "ils marchent vite"

    //
    // -------------------------- Methodes --------------------------------
    //

    NSPhraseur(NSContexte* pCtx) ;
    NSPhraseur(const NSPhraseur& rv) ;
    virtual ~NSPhraseur() ;

    void addSubject(const string sSubject)       { addGrammatical(sSubject, &Sujet) ; }
    void addSubject(const NSPhraseMot* pSubject) { addGrammatical(pSubject, &Sujet) ; }

    void addAttSujet(const string sAttSujet)         { addGrammatical(sAttSujet, &AttSujet) ; }
    void addAttSujet(const NSPhraseMot* pAttSujet)   { addGrammatical(pAttSujet, &AttSujet) ; }

    void addVerb(const string sVerb)             { addGrammatical(sVerb, &Verbe) ; }
    void addVerb(const NSPhraseMot* pVerb)       { addGrammatical(pVerb, &Verbe) ; }

    void addCOD(const string sCOD)               { addGrammatical(sCOD, &COD) ; }
    void addCOD(const NSPhraseMot* pCOD)         { addGrammatical(pCOD, &COD) ; }

    void addAttCOD(const string sAttCOD)         { addGrammatical(sAttCOD, &AttCOD) ; }
    void addAttCOD(const NSPhraseMot* pAttCOD)   { addGrammatical(pAttCOD, &AttCOD) ; }

    void addAdjEpithete(const string sEpithete)         { addGrammatical(sEpithete, &adjEpithete) ; }
    void addAdjEpithete(const NSPhraseMot* pEpithete)   { addGrammatical(pEpithete, &adjEpithete) ; }
    void addAdjEpitheteAv(const string sEpithete)       { addGrammatical(sEpithete, &adjEpitheteAv) ; }
    void addAdjEpitheteAv(const NSPhraseMot* pEpithete) { addGrammatical(pEpithete, &adjEpitheteAv) ; }
    void addAdjEpitheteAp(const string sEpithete)       { addGrammatical(sEpithete, &adjEpitheteAp) ; }
    void addAdjEpitheteAp(const NSPhraseMot* pEpithete) { addGrammatical(pEpithete, &adjEpitheteAp) ; }

    void addCompNom(const string sCompNom)           { addGrammatical(sCompNom, &compNom) ; }
    void addCompNom(const NSPhraseMot* pCompNom)     { addGrammatical(pCompNom, &compNom) ; }

    void addCCLieu(const string sCCLieu)             { addGrammatical(sCCLieu, &CCLieu) ; }
    void addCCLieu(const NSPhraseMot* pCCLieu)       { addGrammatical(pCCLieu, &CCLieu) ; }

    void addCCTemps(const string sCCTemps)           { addGrammatical(sCCTemps, &CCTemps) ; }
    void addCCTemps(const NSPhraseMot* pCCTemps)     { addGrammatical(pCCTemps, &CCTemps) ; }

    void addCCChiffre(const string sCCChiffre)       { addGrammatical(sCCChiffre, &CCChiffre) ; }
    void addCCChiffre(const NSPhraseMot* pCCChiffre) { addGrammatical(pCCChiffre, &CCChiffre) ; }

    void addCCManiere(const string sCCManiere)       { addGrammatical(sCCManiere, &CCManiere) ; }
    void addCCManiere(const NSPhraseMot* pCCManiere) { addGrammatical(pCCManiere, &CCManiere) ; }

    void addCCMoyen(const string sCCMoyen)           { addGrammatical(sCCMoyen, &CCMoyen) ; }
    void addCCMoyen(const NSPhraseMot* pCCMoyen)     { addGrammatical(pCCMoyen, &CCMoyen) ; }

    void addAdverbe(const string sAdverbe)           { addGrammatical(sAdverbe, &adverbe) ; }
    void addAdverbe(const NSPhraseMot* pAdverbe)     { addGrammatical(pAdverbe, &adverbe) ; }

    // Pour mettre a zero toutes les prepositions.
    void nettoye() ;
    void initialise() ;
    void classeAdjectif(NSGenerateur* pGener) ; // Classe les adjectifs en avant et apres

    NSPhraseur& operator=(const NSPhraseur& src) ;

    static long getNbInstance()  { return lBaseObjectCount ; }
    static void initNbInstance() { lBaseObjectCount = 0 ; }

  protected:

    void addGrammatical(const string sEtiquette, NSPhraseMotArray* pBlock) ;
    void addGrammatical(const NSPhraseMot* pElement, NSPhraseMotArray* pBlock) ;

    void   copieTout(const NSPhraseur* pSrc) ;

    void   initParentheses() ;
    void   initDeuxPoints() ;

    static long lBaseObjectCount ;
};

typedef vector<NSPhraseur*> NSPhraseurVector ;
typedef NSPhraseurVector::iterator       phraseurIter ;
typedef NSPhraseurVector::const_iterator phraseurConstIter ;

#ifndef __linux__
class _NSDKDCLASSE NSPhraseurArray : public NSPhraseurVector
#else
class NSPhraseurArray : public NSPhraseurVector
#endif
{
  public :

    // Constructeurs
    NSPhraseurArray() : NSPhraseurVector() {}
    NSPhraseurArray(const NSPhraseurArray& rv) ;
    // Destructeur
    virtual ~NSPhraseurArray() ;
    NSPhraseurArray& NSPhraseurArray::operator=(const NSPhraseurArray &src) ;
    //
    void vider() ;
};

//
// NsProposition can contain either a NSPhraseur* or a propositions array.
// The first case for a simple sentence, the second one for either several
// sentences à la "he does this, opens that and play with this" or for relatives
// for example "I see the boy who plays with the ball"
//
#ifndef __linux__
class _NSDKDCLASSE NsProposition : public NSRoot
#else
class NsProposition : public NSRoot
#endif
{
  protected:

    string      _sPhrase ;

    static long lObjectCount ;

  public :

    enum PROPOSITIONOBJECTTYPE { notSetObjType = 0, isPhraseur, isPropositionArray } ;

    enum PROPOSITIONTYPE
    {
      notSetType = 0,
      principale,
      //
      // Completives
      //
      completiveQue,                          // SCQ : Je veux que tu reviennes
      completiveInfinitive,                   // SCI : J'entends les chats miauler
      completiveInterrogative,                // SCN : Je voudrais savoir pourquoi tu ne veux pas
      //
      // Circonstancielles
      //
      // - Temporelle
      circonstancielleTemporelleAnterieure,   // SCTA : Je le verrai avant qu'il chante
      circonstancielleTemporelleSimultanee,   // SCTS : Je chantais au moment ou il arriva
      circonstancielleTemporellePosterieure,  // SCTP : Je chanterai apres que vous serez parti
      // - Consecutive -> resultat de l'action
      circonstancielleConsecutive,            // SCR : Il fait trop froid pour que je chante
      // - Causale -> cause de l'action
      circonstancielleCausale,                // SCC : Il a reussi parce qu'il chantait bien
      // - Concessive -> relation inattendue
      circonstancielleConcessive,             // SCS : Il a echoue bien qu'il chanta bien
      // - Finale -> but de l'action
      circonstancielleFinale,                 // SCF : Il chante fort pour qu'on l'entende bien
      // - Comparative -> comparaison
      circonstancielleComparativeProportion,  // SCCP : Vous chanterez d'autant plus fort que vous serez bien echauffe
      circonstancielleComparativeEgalite,     // SCCE : Vous chantez aussi fort que moi
      circonstancielleComparativeRessemblance,// SCCR : Tu me regardes comme me regardait ta mere
      // - Conditionnelle -> condition
      circonstancielleConditionnelleHypothese,// SCHH : il mange s'il a faim, il mangera s'il a faim
      circonstancielleConditionnelleImaginee, // SCHH : il mangerait s'il avait faim
      //
      // Relative : traitee en complement du nom
      // Participiale ? (pas de mot subordonnant : le travail acheve, ils partirent en vacance,
      //                  fonctionne comme une circonstancielle sans subordonnant ; peut introduire
      //                  des nuances de temps, cause, concession et condition)
      //

      //
      // Deux points
      //
      deuxPoints, // tout va bien : la vie est belle
    } ;

    enum CONJONCTIONTYPE
    {
      notSetConjonct = 0,
      SCTA_AvantQue,          // ---X---|
      SCTA_JusteAvantQue,     // ------X|
      SCTA_JusquAceQue,       // --XXXXX|
      SCTP_ApresQue,          // ---|---X
      SCTP_DesQue,            // ---|X---
      SCTP_DepuisQue,         // ---|XXXX
      SCCP_AutantQuantite,    // suivant, selon, a mesure
      SCCP_Synchrone,         // au fur et a mesure
      SCCP_PlusQuantite,      // d'autant plus
      SCCP_PlusVite,          // d'autant plus vite
      SCCP_MoinsQuantite,     // d'autant moins
      SCCP_MoinsVite,         // d'autant moins vite
      SCCE_Aspect,            // tel que, de meme que, si ... que
      SCCE_Quantite,          // autant que, aussi que
      SCCE_Temps,             // tant que
    } ;

    PROPOSITIONTYPE         iPropositionType ;
    CONJONCTIONTYPE         iConjonctionType ;

    PROPOSITIONOBJECTTYPE   iObjectType ;
    void*                   pProposition ;
    bool                    bDeleteAtEnd ;

    string                  sLang ;

    NSPhraseur**            pPhraDelAtEnd ;

    NsProposition(NSContexte* pCtx, bool bInitArray = false) ;
    NsProposition(NSContexte* pCtx, NSPhraseur** pPhrase, PROPOSITIONTYPE iPropType, CONJONCTIONTYPE iConjonct, bool bDelAtEnd = true) ;
    NsProposition(NSContexte* pCtx, NSPhraseur* pPhrase, PROPOSITIONTYPE iPropType, CONJONCTIONTYPE iConjonct) ;
    NsProposition(const NsProposition& rv) ;
    ~NsProposition() ;

    void addPropositionToArray(const NSPhraseur* pPhrase, const PROPOSITIONTYPE iPropType, const CONJONCTIONTYPE iConjonct) ;

    NsProposition& operator=(const NsProposition& src) ;

    string getPhrase() const             { return _sPhrase ; }
    void   setPhrase(const string sPh)   { _sPhrase = sPh ;  }
    void   addToPhrase(const string sPh) { _sPhrase += sPh ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

typedef vector<NsProposition*> NSPropositionVector ;
typedef NSPropositionVector::iterator       iterProposition ;
typedef NSPropositionVector::const_iterator iterConstProposition ;

#ifndef __linux__
typedef NTArray<NsProposition> _NSDKDCLASSE NSPropositionArray ;
#else
typedef NTArray<NsProposition> NSPropositionArray ;
#endif

/*
class _NSDKDCLASSE NSPropositionArray : public NSPropositionVector
{
    public :
	    // Constructeurs
	    NSPropositionArray() : NSPropositionVector() {}
	    NSPropositionArray(NSPropositionArray& rv) ;
	    // Destructeur
	    virtual ~NSPropositionArray() ;
	    NSPropositionArray& NSPropositionArray::operator=(NSPropositionArray src) ;
        //
        void vider() ;
};   */

#endif  // __NSPHRASE_H

