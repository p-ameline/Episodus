//-------------------------------------------------------------------------
//                    BIBLIOTHEQUE : NSDKD.H
//
//                     Fonctions utilitaires des decodeurs
//
//             Debut d'ecriture : 30 Juin 1988 (PA)
//             Derniere version : 28 Juillet 1994 (PA)
//-------------------------------------------------------------------------
#ifndef __NSDKD_H
#define __NSDKD_H

#include <vector>

class NSPatPathoArray ;
class NSPatPathoInfo ;
class NSPathologData ;
typedef std::vector<NSPatPathoInfo*>::iterator PatPathoIter ;

class NSPhraseMot ;
class NSPhraseur ;
class NsProposition ;
class NSGenerateur ;

class NSCim10Info ;
class NSCim10 ;
class NSCcamInfo ;
class NSCcam ;

class NSMaterielInfo ;

class NSPersonInfo ;

#ifndef __linux__
  #ifndef _ENTERPRISE_DLL
#include <owl/treewind.h>
#include <owl/imagelst.h>
#include <owl\dialog.h>
  #endif
#endif

#ifndef _ENTERPRISE_DLL
#include "partage/ns_vector.h"
typedef std::vector<OWL::TTreeNode*>   VectorTTreeNode ;
typedef VectorTTreeNode::iterator IterTTreeNode ;
#include "partage/NTArray.h"
typedef NTArray<OWL::TTreeNode>   VectTTreeNode ;
#endif

# include "nautilus/nscr_ama.h"
# include "nsbb/nspatpat.h"
# include "partage/nsdivfct.h"

#ifndef __linux__
#if defined(_DKD_DLL)
	#define _NSDKDCLASSE __export
#else
	#define _NSDKDCLASSE __import
#endif
#endif

//
// Enumeration des types de fenetres
//
enum AIDEDECODTYPE { iOk = 0, iAnnule, iCorrige, iErreur, iAutomatique };

// enum GENRE { genreMS = 1, genreFS, genreMP, genreFP, genreIT};

#ifndef __linux__
class _NSDKDCLASSE decodageBase ;
#else
class decodageBase ;
#endif

/*
class VectTTreeNode : public VectorTTreeNode
{
    public:

        VectTTreeNode() ;
        ~VectTTreeNode();
        VectTTreeNode(VectTTreeNode& src);

        void vider();
};   */

#ifndef __linux__
class _NSDKDCLASSE AdjIntens
#else
class AdjIntens
#endif
{
  public:

	  string _sAdjectif ;
    string _sIntensite ;

    AdjIntens() ;
};

/***************************************************************************/

#ifndef __linux__
  #ifndef _ENTERPRISE_DLL
class AideDecode : public TDialog, public NSRoot
{
  public:

    TWindow* 		     _Aparent ;
    TTreeWindow* 	   _pTreeWind ;
    OWL::TImageList* _pImages ;
    decodageBase*	   _pDecodageBase ;

    AideDecode(TWindow*, decodageBase* pDecode) ;
    ~AideDecode() ;

    void Cmok() ;
    void CmCorrige() ;
    void CmAnnule() ;
    void CmAutomatique() ;
    void SetupWindow() ;

    string buildLabel(PatPathoIter iterTReeView) ;

  DECLARE_RESPONSE_TABLE(AideDecode) ;
};
  #endif // #ifndef _ENTERPRISE_DLL
#endif // #ifndef __linux__

/***************************************************************************/

#ifndef __linux__
class _NSDKDCLASSE gereDate : public NSSuperRoot
#else
class gereDate : public NSSuperRoot
#endif
{
  protected:

    static long lObjectCount ;

  public :

    gereDate(NSSuper* pSuper, string sLangue) ;
    gereDate(const gereDate&) ;
    ~gereDate() { lObjectCount-- ; }

    gereDate& operator =(const gereDate&) ;

    void    initialise() ;

    void    setDate(string* nouvDate)  { _sDate      = *nouvDate ; }
    void    setFormat(string* nouvFmt) { _sFormatage = *nouvFmt ; }
    void    setUnite(string* nouvUnit) { _sUnite     = *nouvUnit ; }

    string  getDate()       { return _sDate ; }
    string  getUnit()       { return _sUnite ; }
    string  getFormat()     { return _sFormatage ; }
    string  getLibelle()    { return _sLibelle ; }

    bool    estVide()       { return (string("") == _sDate) ; }

    void    donne_date_breve(string* pMessage, string* pIntro) ;
    void    donne_date_breve_for_french(string* pMessage, string* pIntro) ;
    void    donne_date_breve_for_english(string* pMessage, string* pIntro) ;

    void    donne_date_claire(string* pMessage, string* pIntro) ;
    string  donne_mois(string mois) ;
    string  donne_mois(int iMois) ;

    void    setLang(string sLng) { _sLang = sLng ; }
    string  getLang()            { return _sLang ; }

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    //protected :

    string _sDate ;
    string _sFormatage ;

    string _sUnite ;
    string _sLibelle ;
    string _sLang ;
} ;

class gereNum ;

#ifndef __linux__
class _NSDKDCLASSE numStorage
#else
class numStorage
#endif
{
  public :

    numStorage(NSSuper* pSuper, string sLangue) ;
    numStorage(const numStorage&) ;
    ~numStorage() ;

    void     initToBlank() ;
    void     init(const numStorage* pSrc) ;

    void     setNum(const string sNum)  { _sNum = sNum ; }
    void     setNumInf(const string sN) { _sNumInf = sN ; }
    void     setNumSup(const string sN) { _sNumSup = sN ; }
    void     setValue(const double dV)  { _dValue = dV ; }
    void     setValInf(const double dV) { _dInfValue = dV ; }
    void     setValSup(const double dV) { _dSupValue = dV ; }
    void     setNormale(const gereNum* pNorm) ;
    void     setNormInf(const gereNum* pNorm) ;
    void     setNormSup(const gereNum* pNorm) ;
    void     setFormat(const string sF) { _sFormatage = sF ; }
    void     setUnit(const string sU)   { _sUnite = sU ; }
    void     setInfEgal(const bool bIE) { _bInfEgal = bIE ; }
    void     setSupEgal(const bool bSE) { _bSupEgal = bSE ; }
    void     setIsExact(const bool bIE) { _bExact = bIE ; }
    void     setHasInf(const bool bHI)  { _bInf = bHI ; }
    void     setHasSup(const bool bHS)  { _bSup = bHS ; }

    void     initValue(const string sRawData) ;
    void     initInfValue(const string sRawData) ;
    void     initSupValue(const string sRawData) ;

    string   getNum()      const { return _sNum ; }
    string   getNumInf()   const { return _sNumInf ; }
    string   getNumSup()   const { return _sNumSup ; }
    gereNum* getNormale()  const { return _pNormale ; }
    gereNum* getNormInf()  const { return _pNormInf ; }
    gereNum* getNormSup()  const { return _pNormSup ; }
    string   getFormat()   const { return _sFormatage ; }
    string   getUnit()     const { return _sUnite ; }
    double   getValue()    const { return _dValue ; }
    double   getInfValue() const { return _dInfValue ; }
    double   getUppValue() const { return _dSupValue ; }
    bool     isExact()     const { return _bExact ; }
    bool     hasInf()      const { return _bInf ; }
    bool     hasSup()      const { return _bSup ; }
    bool     isInfEgal()   const { return _bInfEgal ; }
    bool     isSupEgal()   const { return _bSupEgal ; }

    numStorage& operator=(const numStorage&) ;

    static void strToDble(const string sNum, string &sBonNum, double &dValue) ;

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected :

    string  _sNum, _sNumInf, _sNumSup ;
    double  _dValue, _dInfValue, _dSupValue ;
    bool    _bExact, _bInf, _bSup ;
    bool    _bInfEgal, _bSupEgal ;

    string  _sFormatage ;
    string  _sUnite ;

	  gereNum* _pNormale ;
    gereNum* _pNormInf ;
    gereNum* _pNormSup ;

    static long lObjectCount ;
} ;

typedef vector<numStorage*>              VectorNumStorage ;
typedef VectorNumStorage::iterator       NumStorageIter ;
typedef VectorNumStorage::const_iterator NumStorageConstIter ;
typedef NTArray<numStorage>              VectNumStorage ;

#ifndef __linux__
class _NSDKDCLASSE gereNum : public NSSuperRoot
#else
class gereNum : public NSSuperRoot
#endif
{
  public :

    enum EXTENSIONS { RIEN = 0, NORMALES, STYLE, SENTENCE } ;

    gereNum(NSSuper* pSuper, string sLangue) ;
    gereNum(const gereNum&) ;
    ~gereNum() ;

    gereNum& operator=(const gereNum&) ;

    string  donneLibelleUnite(NSContexte *pContexte, const int iIndice = 0, const bool bWithSpacer = false) ;

    void    initialise() ;
    void    instancier(const string* pNum, const string* pUnit, const string* pFrmt, const int iIndice = 0) ;

    void    setNum(const string* nouvNum, const int iIndice = 0) ;
    void    setUnite(const string* nouvUnit, const int iIndice = 0) ;
    void    setFormt(const string* nouvFrmt, const int iIndice = 0) ;
    void    setVal(const double nouVal, const int iIndice = 0) ;

    bool    setNormale(const gereNum* pNorm, int iIndice = 0) ;
    bool    setNormInf(const gereNum* pNorm, int iIndice = 0) ;
    bool    setNormSup(const gereNum* pNorm, int iIndice = 0) ;

    string  getNum(int iIndice = 0) const ;
    string  getNumInf(int iIndice = 0) const ;
    string  getNumSup(int iIndice = 0) const ;
    string  getNum(NSContexte *pContexte, const string sFormatage, const EXTENSIONS eExtensions = RIEN, const int iIndice = 0) const ;
    string  getNormale(int iIndice = 0) const ;

    gereNum* getNorm(int iIndice = 0) const ;
    gereNum* getNormInf(int iIndice = 0) const ;
    gereNum* getNormSup(int iIndice = 0) const ;

    double  getValeur(int iIndice = 0) const ;
    double  getValeurInf(int iIndice = 0) const ;
    double  getValeurSup(int iIndice = 0) const ;

    bool    estExact(int iIndice = 0) const ;
    bool    estInf(int iIndice = 0) const ;
    bool    estSup(int iIndice = 0) const ;
    bool    estInfEgal(int iIndice = 0) const ;
    bool    estSupEgal(int iIndice = 0) const ;

    string  getUnite(int iIndice = 0) const ;
    string  getFormat(int iIndice = 0) const ;

    bool    estVide(int iIndice = 0) const ;

    void    setLang(const string sLng) { _sLang = sLng ; }
    string  getLang() const            { return _sLang ; }

    gereDate* getDate() const          { return _pDateRef ; }
    void      setDate(gereDate* pDate) ;

    numStorage* getStorageFromIndice(const int iIndice) const ;
    numStorage* getStorageFromIndiceOrCreate(const int iIndice) ;

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected :

 	  VectNumStorage _aValues ;
    gereDate*      _pDateRef ;
    string         _sLang ;

    static long lObjectCount ;
};

#ifndef __linux__
class _NSDKDCLASSE gereHeure : public NSSuperRoot
#else
class gereHeure : public NSSuperRoot
#endif
{
  protected:

    static long lObjectCount ;

  public :

    gereHeure(NSSuper* pSuper, string sLangue) ;
    gereHeure(const gereHeure&) ;
    ~gereHeure() { lObjectCount-- ; }

    gereHeure& operator=(const gereHeure&);

    void   initialise();

    void   setHeure(string* nouvDate) { _sHeure     = *nouvDate ; }
    void   setFormat(string* nouvFmt) { _sFormatage = *nouvFmt ; }
    void   setUnite(string* nouvUnit) { _sUnite 		= *nouvUnit ; }

    string getHeure()   { return _sHeure ; }
    string getLibelle() { return _sLibelle ; }
    string getFormat()  { return _sFormatage ; }
    string getUnit()    { return _sUnite ; }

    bool   estVide()    { return (string("") == _sHeure) ; }

    void   donne_heure(string* pMessage) ;

    void   setLang(string sLng) { _sLang = sLng ; }
    string getLang()            { return _sLang ; }

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    //protected :

    string _sHeure ;
    string _sFormatage ;

    string _sUnite ;
    string _sLibelle ;
    string _sLang ;
};

#ifndef __linux__
class _NSDKDCLASSE gereCode : public NSRoot
#else
class gereCode : public NSRoot
#endif
{
  protected:

    static long lObjectCount ;

  public :

    gereCode(NSContexte* pCtx, string sLangue) ;
    gereCode(const gereCode&) ;
    ~gereCode() { lObjectCount-- ; }

    gereCode& operator=(const gereCode&) ;

    void   initialise() ;

    void   setCode(string* nouvCode) { sCode = *nouvCode ; }
    void   setClassification(string* nouvClass) { sClassification = *nouvClass ; }

    string getCode()    { return sCode ; }
    string getLibelle() { return sCode ; }

    bool   estVide()    { return sCode == "" ; }

    void   donne_code(string* pMessage, bool bAvecCode = true);

    void   setLang(string sLng) { sLang = sLng ; }
    string getLang()            { return sLang ; }

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    //protected :

    string sCode ;
    string sClassification ;
    string sLang ;
};

//
// Phrases de decodage : le decodage en memoire permet un tri dynamique
//
#ifndef __linux__
class _NSDKDCLASSE NSDkdPhrase
#else
class NSDkdPhrase
#endif
{
  protected:

    static long lObjectCount ;

  public:

    int     categorie ;      // titre, sstitre, phrase ordinaire...
    string  sTexte ;

    string  sEtiquette ;     // pour le tri ulterieur

    string  decDeb ;
    string  decFin ;
    int     sautLigne ;
    string  locLesion ;

    NSDkdPhrase() ;
    NSDkdPhrase(const NSDkdPhrase& rv) ;
    ~NSDkdPhrase() ;
    NSDkdPhrase& operator=(const NSDkdPhrase& src) ;
    int operator == (const NSDkdPhrase& o) ;
    int operator != (const NSDkdPhrase& o) { return !(*this==o) ; }

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

typedef vector<NSDkdPhrase*> NSDkdPhrArray ;
typedef NSDkdPhrArray::iterator       NSDkdPhrArrayIter ;
typedef NSDkdPhrArray::const_iterator NSDkdPhrArrayConstIter ;
#ifndef __linux__
typedef NTArray<NSDkdPhrase> _NSDKDCLASSE NSDkdPhraseArray ;
#else
typedef NTArray<NSDkdPhrase> NSDkdPhraseArray ;
#endif

/*
class _NSDKDCLASSE NSDkdPhraseArray : public NSDkdPhrArray
{
    public :

	    NSDkdPhraseArray() : NSDkdPhrArray() {}
	    NSDkdPhraseArray(NSDkdPhraseArray& rv) ;

	    virtual ~NSDkdPhraseArray() ;

	    NSDkdPhraseArray& operator=(NSDkdPhraseArray src) ;
        void vider() ;
};  */

//-----------------------------------------------------------------
// classe decodage
//
// Classe de base des classes de decodage
//-----------------------------------------------------------------
#ifndef __linux__
class _NSDKDCLASSE dkdNoyau : public NSRoot
#else
class dkdNoyau : public NSRoot
#endif
{
  protected:

    NSPatPathoArray*  _pPatPathoArray ;
    PatPathoIter      _itDcode, _itMDcode ;
    string			      _sCodeLexique, _sComplement ;

    string			      _sLocLesion ;
    string			      _sLocPath ;

    string 			      _sCodeLexUtil ;
    bool			        _bCodeUtilValid ;
    string 			      _sCodeLexDecal, _sComplDecal ;

    NSDkdPhraseArray* _pDkdPhrases ;

    NSCRPhraLesArray* _pPhraLes ;
    NSCRPhraseArray*  _pPhrases ;
    string 			      _sDcodeur ;
    int 	  		      _iBon, _iCorrigeAnnule ;
    bool			        _bRecupFenetre ;
    bool              _bDansFichier ;

#ifndef __linux__
	  HANDLE 			      _hDcode ;
#endif

    static long       lObjectCount ;

  public:

    dkdNoyau(NSContexte* pCtx) ;
    ~dkdNoyau() ;

    void ouvreFichier(char *fichier) ;
    void fermeFichier() ;

    string getNodeAsString(PatPathoIter it = NULL) const ;

    string  getDcodeur() const                 { return _sDcodeur ; }
    // string* getDcodeurPointer()                { return &_sDcodeur ; }
    void    setDcodeur(const string sC)        { _sDcodeur =  sC ; }
    void    addToDcodeur(const string sC)      { _sDcodeur += sC ; }
    void    addSentenceToDcodeur(const string sC) ;

    string* getCodeLexDecalPointer()          { return &_sCodeLexDecal ; }
    void    setCodeLexDecal(const string sC)  { _sCodeLexDecal = sC ; }

    NSPatPathoArray* getPatPathoArray() const    { return _pPatPathoArray ; }
    void setPatPathoArray(NSPatPathoArray* pPPt) { _pPatPathoArray = pPPt ; }

    string getCodeLexique() const             { return _sCodeLexique ; }
    void   setCodeLexique(const string sCL)   { _sCodeLexique = sCL ; }
    void   addToCodeLexique(const string sCL) { _sCodeLexique += sCL ; }

    void   refreshCodeLexUtil() ;
    string getCodeLexUtil() const             {	return _sCodeLexUtil ; }
    void   setCodeLexUtil(const string sCLU)  { _sCodeLexUtil = sCLU ; }

    void   refreshComplement()              { _sComplement = (*_itDcode)->getComplement() ; }
    string getComplement() const            { return _sComplement ; }
    void   setComplement(const string sCP)  { _sComplement = sCP ; }

    string getLocLesion() const             { return _sLocLesion ; }
    void   setLocLesion(const string sLL)   { _sLocLesion = sLL ; }
    void   addLocLesion(const string sLL)   { _sLocLesion += sLL ; }

    string getLocPath()   const             { return _sLocPath ; }
    void   setLocPath(const string sLP)     { _sLocPath = sLP ; }
    void   addLocPath(const string sLP)     { _sLocPath += sLP ; }

    void              setDkA(NSDkdPhraseArray* pDkA) { _pDkdPhrases = pDkA ; }
    NSDkdPhraseArray* getDkA() const                 { return _pDkdPhrases ; }

    void              setPLA(NSCRPhraLesArray* pPLA) { _pPhraLes = pPLA ; }
    NSCRPhraLesArray* getPLA() const                 { return _pPhraLes ; }

    void              setPhA(NSCRPhraseArray* pPhA)  { _pPhrases = pPhA ; }
    NSCRPhraseArray*  getPhA() const                 { return _pPhrases ; }

    bool	            isCodeUtilValid() const        { return _bCodeUtilValid ; }
    void	            setCodeUtilValid(bool bUV)     { _bCodeUtilValid = bUV ;  }

#ifndef __linux__
	  HANDLE getHandler() const               { return _hDcode ; }
    void   setHandler(HANDLE hH)            { _hDcode = hH ; }
#endif

    int 	 isGood()                         { return _iBon ; }
    void	 setGood(const int iG)            { _iBon = iG ; }

    NSPatPathoArray* getPatPathoArray()     { return _pPatPathoArray ; }

    void   setCorrAnnu(const int valeur)    { _iCorrigeAnnule = valeur ; }
    int    getCorrAnnu() const              { return _iCorrigeAnnule ; }

    void   setRecupFen(const bool valeur)   { _bRecupFenetre = valeur ; }
    bool   getRecupFen() const              { return _bRecupFenetre ; }

    void   setDsFichier(const bool valeur)  { _bDansFichier = valeur ; }
    bool   getDsFichier() const             { return _bDansFichier ; }

    PatPathoIter getitDcode() const          { return _itDcode ; }
    void         setitDcode(PatPathoIter it) { _itDcode = it ; }
    void         nextItDcode()               { if (_pPatPathoArray->end() != _itDcode) _itDcode++ ; }
    void         previousItDcode()           { _itDcode-- ; }
    boolean      isItDcodeAtEnd() const      { return _pPatPathoArray->end() == _itDcode ; }

    PatPathoIter getitMDcode() const          { return _itMDcode ; }
    void         setitMDcode(PatPathoIter it) { _itMDcode = it ; }
    void         nextItMDcode()               { if (_pPatPathoArray->end() != _itMDcode) _itMDcode++ ; }
    void         previousItMDcode()           { _itMDcode-- ; }
    boolean      isItMDcodeAtEnd() const      { return _pPatPathoArray->end() == _itMDcode ; }

    void   MetMarque() { _itMDcode = _itDcode ; }
    void   VaMarque() ;

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

//-----------------------------------------------------------------
// classe decodage
//
// Classe de base des classes de decodage
//-----------------------------------------------------------------
#ifndef __linux__
class _NSDKDCLASSE decodageBase : public NSRoot
#else
class decodageBase : public NSRoot
#endif
{
  private:

	  dkdNoyau*      _pNoyau ;
    bool	         _bCreateur ;

    static long    lObjectCount ;

  protected:

    NSPhraseur*    _pPhraseur ;
    string         _sLangue ;
    NSGenerateur*  _pGenerateur ;
    NsProposition* _pPropos ;

  public:

    // Methodes
    //
    decodageBase(NSContexte* pCtx, const string sLangue = string("fr")) ;

    // Constructeur copie : duplique le noyau
    decodageBase(const decodageBase& rv) ;
    // Constructeur filiation : partage le meme noyau
    decodageBase(const decodageBase* ptr) ;

    virtual ~decodageBase() ;

    NSGenerateur*     createGenerator(NsProposition* pProp) ;

    // string*           sDcodeur()       const { return _pNoyau->getDcodeurPointer() ; }
    string            getDcodeur()     const  { return _pNoyau->getDcodeur() ; }
    void              setDcodeur(const string sD)   { _pNoyau->setDcodeur(sD) ;   }
    void              addToDcodeur(const string sD) { _pNoyau->addToDcodeur(sD) ; }
    void              addSentenceToDcodeur(const string sC) { _pNoyau->addSentenceToDcodeur(sC) ; }

    void              setPPtArray(NSPatPathoArray* pPPtArray) ;
    NSPatPathoArray*  getPPtArray()    const { return _pNoyau->getPatPathoArray() ; }

    void              setDkA(NSDkdPhraseArray* pDkA) { _pNoyau->setDkA(pDkA) ; }
    NSDkdPhraseArray* getDkA()         const { return _pNoyau->getDkA() ; }

    void              setPLA(NSCRPhraLesArray* pPLA) { _pNoyau->setPLA(pPLA) ; }
    NSCRPhraLesArray* getPLA()         const { return _pNoyau->getPLA() ; }

    void              setPhA(NSCRPhraseArray* pPhA) { _pNoyau->setPhA(pPhA) ; }
    NSCRPhraseArray*  getPhA()         const { return _pNoyau->getPhA() ; }

    void              set_iBon(const int valeur) { _pNoyau->setGood(valeur) ; }
    int               iBon()           const { return _pNoyau->isGood() ; }

    void              setCorrAnnu(const int valeur) { _pNoyau->setCorrAnnu(valeur) ; }
    int               getCorrAnnu()    const { return _pNoyau->getCorrAnnu() ; }

    void              setRecupFen(const bool valeur) { _pNoyau->setRecupFen(valeur) ; }
    bool              getRecupFen()    const { return _pNoyau->getRecupFen() ; }

    void              setDsFichier(const bool valeur) { _pNoyau->setDsFichier(valeur) ; }
    bool              getDsFichier()   const { return _pNoyau->getDsFichier() ; }

    PatPathoIter      getitDcode()     const { return _pNoyau->getitDcode() ; }
    PatPathoIter      getitMDcode()    const { return _pNoyau->getitMDcode() ; }
    void              setitMDcode(PatPathoIter it) { _pNoyau->setitMDcode(it) ; }

    NSPhraseur*       getPhraseur()    const { return _pPhraseur ; }
    string            getLang()        const { return _sLangue ; }
    NSGenerateur*     getGenerateur()  const { return _pGenerateur ; }
    NsProposition*    getProposition() const { return _pPropos ; }

    void			        Avance(const int nbPositions = 1) ;
    void			        Recule(const int nbPositions = 1) ;
    void			        MetMarque() ;
    void			        VaMarque() ;
    boolean           isAtEnd() const        { return _pNoyau->isItDcodeAtEnd() ; }

    void			        Suivant() ;
    void			        Precedent() ;

    string		 	      getSt() const ; // Code utile
    string			      getStL() const { return _pNoyau->getCodeLexique() ; } // Code complet

    void		 	        getCert(string* pCert) const ; // Certitude
    void		 	        getPlur(string* pPlur) const ; // Pluriel

    string			      getCert()       const { return (*(_pNoyau->getitDcode()))->getCertitude() ; }
    string			      getPlur()       const { return (*(_pNoyau->getitDcode()))->getPluriel() ; }
    string			      getLexique()    const { return (*(_pNoyau->getitDcode()))->getLexique() ; }
    string			      getTexteLibre() const { return (*(_pNoyau->getitDcode()))->getTexteLibre() ; }

    string*			      getSt(int decal) ;  // Code utile
    string*			      getStL(int decal) ; // Code complet

    int 			        getCol() const ;

    string 			      getCpl() ;

    void			        ouvreFichier(const char *fichier) ;
    void			        fermeFichier() ;
    void 			        metPhrase(const string decDeb = string(""), const string decFin = string(""), const int sautLigne = 0) ;
    bool 			        metUnCar(const char cCar) ;
    void     		      vidangeDkdPhrases() ;

    void 			        etDuMilieu(string *type, string *type1, string *type2, const string entre) ;
    void 			        etFinal(string *type, string *type1, const string fin) ;
    void 			        ajoutSeparateur(string *phrase, unsigned int niveau) ;

    void			        donneDimension(int colonne, gereNum* pNum) ;
    void			        donneDate(int colonne, gereDate* pDate) ;
    void			        donneHeure(int colonne, gereHeure* pHeure) ;
    void			        donneCode(int colonne, gereCode* pCode) ;

    void    		      getGereDate(gereDate** ppDate) ;

#ifndef _ENTERPRISE_DLL
    DBIResult		      getMateriel(NSMaterielInfo* pMatInfo) ;
    DBIResult		      getCIM10(NSCim10Info* pMatInfo, NSCim10* pMat = (NSCim10*) 0) ;
    DBIResult		      getCcam(NSCcamInfo* pCcamInfo, NSCcam* pCcam = (NSCcam*) 0) ;
#endif

    bool              getCorresp(NSPersonInfo* pPersonInfo) ;
    // DBIResult		getCorresp(NSCorrespondantInfo* pCorInfo);

    //void			  donneCarLibre(string* carLibre);
    void 			        donneCarLibre(int colonne, string* carLibre, GENRE* pGenre = 0) ;

    void			        initialiseIterateurs() ;

    void 			        ajLL() ;

    void 			        Recupere() ;

    bool   			      CommenceParVoyelle(const string* pLib) const ;
    int  				      donneCertitude(const string sCert) const ;
    //
    string			      intenseAdjectif(AdjIntens* pAdj1, AdjIntens* pAdj2) ;

    //
    bool			        isTexteLibre() const ;
    bool			        isChampLibre() const ;
    bool			        isDimension() const ;
    bool			        isDate() const ;
    bool			        isHeure() const ;
    bool			        isCode() const ;
    bool			        isComplementAvecUnite(const char cCategorie) const ;
    bool			        isComplementSansUnite(const char cCategorie) const ;

    void              setDcodeurFromLexique(string sCodeLexique, string sLang = string("")) ;
    void              setDcodeurFromData(NSPathologData* pData) ;
    void              setPseumajForFirstChar() ;
    void              stripDcodeur(STRIPTYPE n, char c = ' ') ;

    // Natural language generation macros
    //
    string createSentenceForNum(const string sLabel, const gereNum* pNum, bool bReady = true) ;
    string createSentenceForNum(const NSPhraseMot* pSubject, const gereNum* pNum, bool bReady = true) ;
    string createSentenceForNumRelatedTo(const string sRelatedTo, const string sLabel, const gereNum* pNum, bool bReady = true, bool bDontShowRelator = false) ;

    string createSentenceForAttribute(const string sLabel, const string sAttribute, bool bReady = true) ;
    string createSentenceForAttribute(const NSPhraseMot* pSubject, const string sAttribute, bool bReady = true) ;

    string createSentenceForAttributeRelatedTo(const string sRelatedTo, const string sLabel, const string sAttribute, bool bReady = true, bool bDontShowRelator = false) ;
    string createSentenceForAttributeRelatedTo(const NSPhraseMot* pRelatedTo, const NSPhraseMot* pSubject, const string sAttribute, bool bReady = true, bool bDontShowRelator = false) ;

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

#endif 		// __DIVDCODE_H

