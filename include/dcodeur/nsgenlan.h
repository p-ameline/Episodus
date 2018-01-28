#ifndef __NSGENLAN_H
#define __NSGENLAN_H

#include "dcodeur/nsgen.h"
#include "dcodeur/nsdkd.h"
// #include "nssavoir/nspatho.h"

#ifndef __linux__
#if defined(_DKD_DLL)
	#define _NSDKDCLASSE __export
#else
	#define _NSDKDCLASSE __import
#endif
#endif

class SentenceBlocks
{
  public :

    SentenceBlocks() ;
    SentenceBlocks(const SentenceBlocks& rv) ;

    ~SentenceBlocks() ;

    SentenceBlocks& operator=(const SentenceBlocks& src) ;

    // Getters
    //
    string getSujet()        const { return _sSujet ; }
    string getAttSujet()     const { return _sAttSujet ; }
    GENRE  getGenreSujet()   const { return _iGenreSujet ; }

    string getVerb()         const { return _sVerbe ; }

    string getPhraseAdverb() const { return _sPhraseAdverbe ; }
    string getModifAdv()     const { return _sAdverbeModificateur ; }
    string getModifAttAdv()  const { return _sAdverbeModificateurAtt ; }

    string getCOD()          const { return _sCOD ; }
    string getAttCOD()       const { return _sAttCOD ; }
    GENRE  getGenreCOD()     const { return _iGenreCOD ; }

    int    getThreshold()    const { return _iComplLevelThreshold ; }
    bool   isAffirmative()   const { return _bPhraseAffirmative ; }

    // Setters
    //
    void   setSujet(const string sS)       { _sSujet = sS ; }
    void   setAttSujet(const string sAS)   { _sAttSujet = sAS ; }
    void   setGenreSujet(const GENRE iGS)  { _iGenreSujet = iGS ; }

    void   setVerb(const string sV)        { _sVerbe = sV ; }
    void   addToVerb(const string sV)      { _sVerbe += sV ; }

    void   setPhraseAdverb(const string s) { _sPhraseAdverbe = s ; }
    void   addPhraseAdverb(const string s) { _sPhraseAdverbe += s ; }
    void   setModifAdv(const string sMA)   { _sAdverbeModificateur = sMA ; }
    void   setModifAttAdv(const string sM) { _sAdverbeModificateurAtt = sM ; }

    void   setCOD(const string sC)         { _sCOD = sC ; }
    void   addToCOD(const string sC)       { _sCOD += sC ; }
    void   setAttCOD(const string sAC)     { _sAttCOD = sAC ; }
    void   addToAttCOD(const string sAC)   { _sAttCOD += sAC ; }
    void   setGenreCOD(const GENRE iGC)    { _iGenreCOD = iGC ; }

    void   setThreshold(const int iT)      { _iComplLevelThreshold = iT ; }
    void   setAffirmative(bool bA)         { _bPhraseAffirmative = bA ; }

  protected :

    int    _iComplLevelThreshold ;

    string _sSujet ;
    string _sAttSujet ;
    GENRE  _iGenreSujet ;

    string _sVerbe ;

    string _sPhraseAdverbe ;
    string _sAdverbeModificateur ;
    string _sAdverbeModificateurAtt ;

    string _sCOD ;
    string _sAttCOD ;
    GENRE  _iGenreCOD ;

    bool   _bPhraseAffirmative ;
};

#ifndef __linux__
class _NSDKDCLASSE NSGenerateurFr : public NSGenerateur
#else
class NSGenerateurFr : public NSGenerateur
#endif
{
  public :

    enum GENRENOM { declinaisonSingulier = 0, declinaisonPluriel } ;
    enum GENREADJ { declinaisonMS = 0, declinaisonFS, declinaisonMP, declinaisonFP } ;

    enum TEMPSVERBEFR { tempsPasseFr = 1, tempsPresentFr, tempsFuturFr } ;
    enum ASPECTVERBEFR { aspectMoment = 1, aspectAccompli, aspectProspectif } ;

    NSGenerateurFr(NSContexte* pCtx, NsProposition* pPropos, string sLangue) ;
    NSGenerateurFr(const NSGenerateurFr& rv) ;

    ~NSGenerateurFr() ;

    NSGenerateurFr& operator=(const NSGenerateurFr& src) ;

    void    classeTout();

    bool    assembleProposition(DCODETYPE iStyle, NsProposition* pPropos = (NsProposition*) 0) ;

    void    donneLibelleAffiche(string* pLibelle, NSPathologData* pData, const GENRE iGenre = genreNull) ;

    bool    prepareVerbe(SentenceBlocks* pSentenceBlocks, DCODETYPE iStyle) ;
    bool    prepareSujet(SentenceBlocks* pSentenceBlocks, DCODETYPE iStyle) ;
    bool    prepareCOD(SentenceBlocks* pSentenceBlocks, DCODETYPE iStyle) ;

    string  donneNomComplet(NSPhraseMot* pMot, NSPhraseMot::TYPEARTICLE iArticle, DCODETYPE iStyle, const GERECERTITUDE iGereCertitude = avecCertitude, GENERATEDLEVEL iGeneratedLevel = levelUnknown) ;
    bool    genereNomComplet(NSPhraseMot* pMot, NSPhraseMot::TYPEARTICLE iArticle, DCODETYPE iStyle, const GERECERTITUDE iGereCertitude = avecCertitude) ;

    string  donneAdjectifComplet(NSPhraseMot* pMot, const GENRE iGenre = genreNull, const GERECERTITUDE iGereCertitude = avecCertitude, bool bInAffirmativeSequence = false) ;
    bool    genereAdjectifComplet(NSPhraseMot* pMot, const GENRE iGenre = genreNull, const GERECERTITUDE iGereCertitude = avecCertitude, bool bInAffirmativeSequence = false) ;

    string  donneAdverbeComplet(NSPhraseMot* pMot, const GERECERTITUDE iGereCertitude = avecCertitude) ;
    bool    genereAdverbeComplet(NSPhraseMot* pMot, const GERECERTITUDE iGereCertitude = avecCertitude) ;

    string  donneCertitude(string sCertitude, bool* pCommenceParVoyelleOuH = (bool*) 0, bool bInAffirmativeSequence = false) ;

    string  donnePhraseComplement(NSPhraseMot* pLiaison,
                                 NSPhraseMot* pPreposition,
                                 NSPhraseMotArray* pCC, bool* bSucces,
                                 DCODETYPE iStyle) ;
    string  donnePhraseChiffree(NSPhraseMot* pLiaison,
                               NSPhraseMot* pPreposition,
                               NSPhraseMotArray* pCC, bool* bSucces) ;
    string  getLabelForNum(NSPhraseMot* pWord) ;

    bool    donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj = (NSPhraseMot*) 0) ;
    bool    donnePremierAdv(NSPhraseur* pComp, NSPhraseMot* pMotAdv = (NSPhraseMot*) 0) ;

    string  decodeNum(gereNum* pNum, bool* bSucces) ;
    string  decodeTime(NSPhraseMotTime* pTime, bool* bSucces) ;
    string  decodeCycle(NSPhraseMotTimeCycle* pCycle, bool* bSucces) ;

    string  donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) ;
    string  donneArticleDefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) ;
    string  donneArticlePartitif(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) ;
    string  donneAdjectifPossessif(NSPhraseMot* pMot, bool bPluriel) ;
    string  getAdjectifPossessif(GENRE iGenreNom, GENRE iGenrePossesseur) ;
    string  getAdjectifDemonstratif(GENRE iGenreNom) ;
    string  donnePronomPersonnel(GENRE iGenre, NSPhraseur::VBPERSO iVbPersonne, string sFonction) ;

    void    etFinal(string *type, string *type1, const string sSeparator = string(" et ")) ;

    string  donneParticipePresent(NSPathologData* pPathoData) ;
    string  donneParticipePasse(NSPhraseMot* pPhraseMot, const GENRE iGenre) ;

    void    mapTempsVerbe() ;
    void    donneVerbe(string* principal, string* auxilliaire) ;
    string  donneVerbeTerminaison(int groupe, string ssgroupe) ;
    bool    donneVerbeClasse(NSPhraseMot* pPhraseMot, string* psTransitivite,
                          string* psConjugaison, string* psActEtat) ;
    string  donneAuxilliaire(string* auxilliaire, int iVbTemps, int iVbAspect,
                            int iVbMode, int iVbPersonne, int iVbNegation) ;

    bool    traitePrepositionCC(NSPhraseMotArray* pCompC, NSPhraseMot* pPreposition = (NSPhraseMot*) 0, NSPhraseur::VBPREPOCTX iContexte = NSPhraseur::prepUndefined) ;
    bool    modifiePreposition(NSPhraseMot* pMot, int iArticle, bool bPluriel) ;

    string  postTraitement(string *sEntree) ;

  protected :

    int  iTempsVerbe ;
    int  iAspectVerbe ;

    bool generePhrase(DCODETYPE iStyle) ;
    bool generePhrasePrincipale(SentenceBlocks* pSB, DCODETYPE iStyle) ;
    bool generePhraseRelative(SentenceBlocks* pSB, DCODETYPE iStyle) ;

    bool buildActiveSentenceWithSubject(SentenceBlocks* pSB) ;
    bool buildActiveSentenceWithoutSubject(SentenceBlocks* pSB) ;
    bool buildPassiveSentence(SentenceBlocks* pSB) ;
    bool buildBulletSentence(SentenceBlocks* pSB) ;

    bool buildActiveVerbalGroup(SentenceBlocks* pSB) ;
    bool buildPassiveVerbalGroup(SentenceBlocks* pSB) ;

    void writeComplements(int iThreshold, string sBefore, string sBeforeNegation, string sAfter, const bool bAboveThreshold = false) ;

    string getParticipePresent(string sVerb, bool bAffirmative) ;
};

#ifndef __linux__
class _NSDKDCLASSE gereDateFr : public gereDate
#else
class gereDateFr : public gereDate
#endif
{
  public :

    gereDateFr(NSSuper* pSuper, string sLangue):gereDate(pSuper, sLangue) {}
    gereDateFr(const gereDateFr&) ;
    ~gereDateFr() {}

    gereDateFr& operator=(const gereDateFr&) ;

    enum TYPEDATE { DateNormal = 1, DateDeb, DateFin} ;

    void   donne_date_breve(string* pMessage, string* pIntro, int iType = DateNormal) ;
    void   donne_date_breve_for_date(string* pMessage, string* pIntro, int iType) ;

    void   donne_date_claire(string* pMessage, string* pIntro, int iType = DateNormal) ;
    void   donne_date_claire_for_date(string* pMessage, string* pIntro, int iType) ;

    string donne_mois(string mois) ;
    string donne_mois(int iMois) ;
};

#ifndef __linux__
class _NSDKDCLASSE gereNumFr : public gereNum
#else
class gereNumFr : public gereNum
#endif
{
  public :

    gereNumFr(NSSuper* pSuper, string sLangue):gereNum(pSuper, sLangue) {}
    gereNumFr(const gereNumFr&) ;
    ~gereNumFr() {}

    gereNumFr& operator=(const gereNumFr&) ;

    string getLabel() const ;
};

#ifndef __linux__
class _NSDKDCLASSE NSGenerateurEn : public NSGenerateur
#else
class NSGenerateurEn : public NSGenerateur
#endif
{
  public :

    enum GENRENOM { declinaisonSingulier = 0, declinaisonPluriel } ;
    enum GENREVERBE { declinaisonBase = 0, declinaison3Personne,
                      declinaisonPartPresent, declinaisonPreterite,
                      declinaisonPartPasse } ;

    NSGenerateurEn(NSContexte* pCtx, NsProposition* pPropos, string sLangue) ;
    NSGenerateurEn(const NSGenerateurEn& rv) ;

    ~NSGenerateurEn() ;

    NSGenerateurEn& operator=(const NSGenerateurEn& src) ;

    void    classeTout() ;

    bool    assembleProposition(DCODETYPE iStyle, NsProposition* pPropos = (NsProposition*) 0) ;

    void    donneLibelleAffiche(string* pLibelle, NSPathologData* pData, const GENRE iGenre = genreNull) ;
    void    donneLibelleAffiche(string* pLibelle, NSPathologData* pData, const GENREVERBE iGenre) ;

    bool    prepareVerbe(string* psPhraseAdverbe, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle) ;
    bool    prepareSujet(string* psSujet, GENRE* piGenreSujet, string* psAttSujet, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle) ;
    bool    prepareCOD(string* psCOD, GENRE* piGenreCOD, string* psAttCOD, string* psAdverbeModificateur, bool* pbPhraseAffirmative, DCODETYPE iStyle) ;

    string  donneNomComplet(NSPhraseMot* pMot, NSPhraseMot::TYPEARTICLE iArticle, DCODETYPE iStyle, GERECERTITUDE iGereCertitude = avecCertitude) ;
    bool    genereNomComplet(NSPhraseMot* pMot, NSPhraseMot::TYPEARTICLE iArticle, DCODETYPE iStyle, GERECERTITUDE iGereCertitude = avecCertitude) ;

    string  donneAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre = genreNull, GERECERTITUDE iGereCertitude = avecCertitude) ;
    bool    genereAdjectifComplet(NSPhraseMot* pMot, GENRE iGenre = genreNull, GERECERTITUDE iGereCertitude = avecCertitude) ;

    string  donneAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude = avecCertitude) ;
    bool    genereAdverbeComplet(NSPhraseMot* pMot, GERECERTITUDE iGereCertitude = avecCertitude) ;

    bool    donnePremierAdv(NSPhraseur* pComp, NSPhraseMot* pMotAdv = (NSPhraseMot*) 0) ;

    string  donneCertitude(string sCertitude, bool* pCommenceParVoyelleOuH = (bool*) 0, bool bInAffirmativeSequence = false) ;

    string  donnePhraseComplement(NSPhraseMot* pLiaison,
                                 NSPhraseMot* pPreposition,
                                 NSPhraseMotArray* pCC, bool* bSucces,
                                 DCODETYPE iStyle) ;
    string  donnePhraseChiffree(NSPhraseMot* pLiaison,
                               NSPhraseMot* pPreposition,
                               NSPhraseMotArray* pCC, bool* bSucces) ;

    bool    donnePremierAdj(NSPhraseur* pCompAdj, NSPhraseMot* pMotAdj = 0) ;

    string  decodeNum(gereNum* pNum, bool* bSucces) ;
    string  decodeTime(NSPhraseMotTime* pTime, bool* bSucces) ;
    string  decodeCycle(NSPhraseMotTimeCycle* pCycle, bool* bSucces) ;

    string  donneArticleIndefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) ;
    string  donneArticleDefini(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) ;
    string  donneArticlePartitif(NSPhraseMot* pMot, bool bPluriel, GERECERTITUDE iGereCertitude = avecCertitude) ;
    string  donnePronomPersonnel(GENRE iGenre, NSPhraseur::VBPERSO iVbPersonne, string sFonction) ;

    void    etFinal(string *type, string *type1, const string sSeparator = string(" and ")) ;

    string  donneParticipePresent(NSPathologData* pPathoData) ;
    string  donneParticipePasse(NSPhraseMot* pPhraseMot, GENRE iGenre = genreNull) ;
    string  donnePreterite(NSPhraseMot* pPhraseMot) ;
    void    donneVerbe(string* principal, string* auxilliaire) ;
    bool    donneVerbeClasse(NSPhraseMot* pPhraseMot, string* psTransitivite, string* psActEtat) ;

    bool    traitePrepositionCC(NSPhraseMotArray* pCompC, NSPhraseMot* pPreposition = (NSPhraseMot*) 0, NSPhraseur::VBPREPOCTX iContexte = NSPhraseur::prepUndefined) ;

    string  postTraitement(string *sEntree) ;

  protected :

    bool    generePhrase(DCODETYPE iStyle) ;
};

#ifndef __linux__
class _NSDKDCLASSE gereDateEn : public gereDate
#else
class gereDateEn : public gereDate
#endif
{
  public :

    gereDateEn(NSSuper* pSuper, string sLangue):gereDate(pSuper, sLangue) {}
    gereDateEn(const gereDateEn&) ;
    ~gereDateEn() {}

    gereDateEn& operator=(const gereDateEn&) ;

    enum TYPEDATE { DateNormal = 1, DateDeb, DateFin} ;

    void    donne_date_breve(string* pMessage, string* pIntro, int iType = DateNormal) ;
    void    donne_date_claire(string* pMessage, string* pIntro, int iType = DateNormal) ;
    string  donne_mois(string mois) ;
    string  donne_mois(int iMois) ;
};

#ifndef __linux__
class _NSDKDCLASSE gereNumEn : public gereNum
#else
class gereNumEn : public gereNum
#endif
{
  public :

    gereNumEn(NSSuper* pSuper, string sLangue):gereNum(pSuper, sLangue) {}
    gereNumEn(const gereNumFr&) ;
    ~gereNumEn() {}

    gereNumEn& operator=(const gereNumEn&) ;

    string getLabel() ;
};

#endif  // __NSGENLAN_H

