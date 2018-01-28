#ifndef __NSLDVGOAL_H__
# define __NSLDVGOAL_H__

# include <vector.h>
# include <string.h>

# include "nautilus\nsldvdoc.h"

// -----------------------------------------------------------------------------
// Objectif instancié dans le dossier patients
// -----------------------------------------------------------------------------

class	NSLdvGoal : public NSRoot
{
  protected:

    string      	  _sLexique ;
    string      	  _sTitre ;
    string      	  _sConcern ;
    string      	  _sReference ;

    static long lObjectCount ;

  public:

	  string          _sGoalID ;
	  string          _sReferentialID ;

    string          _sCertitude ;          //presence ou l'absence de l'objectif
    string          _sComplementText ;     //en texte libre

    enum NSGOALTYPE   { medicament=0, biology, traitement, exam, other };
    enum TYPESRYTHMES { ponctuel = 1, cyclic, permanent } ; // (durée ?)
    int             _iRythme ;

  //  bool            bCyclic ;

    double          _dDelaiDebutAutorise ;   // délai de début autorisé
    string          _sUniteDebutAutorise ;
    double          _dDelaiDebutConseille ;  // délai de début conseillé
    string          _sUniteDebutConseille ;
    double          _dDelaiDebutIdeal ;      // délai de début idéal
    string          _sUniteDebutIdeal ;
    double          _dDelaiDebutIdealMax ;   // délai de début idéal maximum
    string          _sUniteDebutIdealMax ;
    double          _dDelaiDebutConseilMax ; // délai de début conseillé maximum
    string          _sUniteDebutConseilMax ;
    double          _dDelaiDebutCritique ;   // délai de début critique
    string          _sUniteDebutCritique ;

    string          _sDateDebutAutorise ;    // date de début autorisé
    string          _sDateDebutConseille ;   // date de début conseillé
    string          _sDateDebutIdeal ;       // date de début idéal
    string          _sDateDebutIdealMax ;    // date de début idéal maximum
    string          _sDateDebutConseilMax ;  // date de début conseillé maximum
    string          _sDateDebutCritique ;    // date de début critique

    bool            _bValue ;
    bool            _bValMinAutorise ;       // valeur min autorisé active ?
    double          _dValMinAutorise ;       // valeur min autorisé
    string          _sUniteValMinAutorise ;  // unité de la valeur min autorisé
    bool            _bValMinConseille ;      // valeur min conseillé active ?
    double          _dValMinConseille ;      // valeur min conseillé
    string          _sUniteValMinConseille ; // unité de la valeur min conseillé
    bool            _bValMinIdeal ;          // valeur min idéal active ?
    double          _dValMinIdeal ;          // valeur min idéal
    string          _sUniteValMinIdeal ;     // unité de la valeur min idéal
    bool            _bValMaxIdeal ;          // valeur max idéal active ?
    double          _dValMaxIdeal ;          // valeur max idéal
    string          _sUniteValMaxIdeal ;     // unité de la valeur max idéal
    bool            _bValMaxConseille ;      // valeur max conseillé active ?
    double          _dValMaxConseille ;      // valeur max conseillé
    string          _sUniteValMaxConseille ; // unité de la valeur max conseillé
    bool            _bValMaxAutorise ;       // valeur max autorisé active ?
    double          _dValMaxAutorise ;       // valeur max autorisé
    string          _sUniteValMaxAutorise ;  // unité valeur max autorisé

    NVLdVTemps  	  _tDateOuverture ;        // Date théorique d'ouverture
    NVLdVTemps  	  _tDateFermeture ;        // Date théorique de fermeture
    string          _sOpenEventNode ;        // Noeud de l'événement d'ouverture
    string          _sCloseEventNode ;       // Noeud de l'événement de fermeture

    NVLdVTemps  	  _tOuvertLe ;             // Date effective d'ouverture
    double          _dValueOuverture ;       // valeur à l'ouverture
    string          _sValueOuverture ;
    string          _sUnitOuverture ;
    bool            _bValueOuverture ;
    NVLdVTemps  	  _tFermeLe ;              // Date effective de fermeture
    double          _dValueFermeture ;       // valeur à la fermeture
    string          _sValueFermeture ;
    string          _sUnitFermeture ;
    bool            _bValueFermeture ;
    bool						_isSelected ;    				//objectif selectionné pour l'ordonnance (si medicament) ou process view

    NSLdvGoal(NSContexte *pCtx, NSLdvDocument *pDocum, LDVFRAME iFrame) ;
    NSLdvGoal(const NSLdvGoal& rv) ;
    ~NSLdvGoal() ;

    void            init(string sOpenDate, string sCloseDate, string sOpenEvent, string sCloseEvent) ;
    void            init() ;
    void            initJalons() ;
    void            initPostDateInterJalons(NSLdvGoalInfo* pRefJalon, NVLdVTemps tpsNextJalonDate) ;
    void            initInterDelayJalons(NSLdvGoalInfo* pRefJalon, NVLdVTemps tpsNextJalonDate) ;
    void            initMetaJalons() ;
    void            initConcern(NSLdvDocument *pDocLdv) ;
    void            initFromPatho(NSPatPathoArray *pPPt, PatPathoIter *pIter) ;
    void            getPathoValue(NSPatPathoArray *pPPt, PatPathoIter *pIter, string &sValue, string &sUnit, string &sFormat) ;

    double          getMeanDelaiInSecond() ;
    double          getDelaiInSecond(double iDelai, string sUnite) ;

    string          getNodeDate (string sNode) ;
    bool            getNodeValue(string sNode, string *sValue, double *iValue) ;
    string          getNodeUnit (string sNode) ;

    NSLdvGoalInfo::JALONSLEVELS getValueLevel(string sValue, string sUnit, NSLdvGoalInfo::JALONSLEVELS *iLastLevel) ;

    NSLdvGoal&		  operator=(const NSLdvGoal& src) ;
    bool            estIdentique(NSLdvGoal *pModel) ;

    string          getCompositeID() { return _sReferentialID + string(1, '#') + _sGoalID ; }

    void            setReferentialID(string sReID) { _sReferentialID = sReID ; }
    string          getReferentialID()             { return _sReferentialID ; }

    string		      getNode()               { return _sReference ; }
    void        	  setNode(string sNd)     { _sReference = sNd ; }
    string		      getLexique()            { return _sLexique ; }
    void        	  setLexique(string sLx)  { _sLexique = sLx ; }
    string		      getTitle()              { return _sTitre ; }
    void        	  setTitle(string sTi)    { _sTitre = sTi ; }
    string		      getConcern()            { return _sConcern ; }
    void        	  setConcern(string sCn)  { _sConcern = sCn ; }

    GoalInfoArray*  getMetaJalons()         { return &_aMetaJalons ; }
    GoalInfoArray*  getJalons()             { return &_aJalons ; }

    LDVFRAME        getIFrame()             { return _iFrame ; }

    void            getADateTree(string sVal, string sCode, NSPatPathoArray* pPatho, int colonne) ;
    void            getAllDatesTree(NSPatPathoArray* pPatho, int colonne) ;
    void            getAPeriodTree(double dVal, string unit, string sCode, NSPatPathoArray* pPatho, int colonne) ;
    void            getAllPeriodesTree(NSPatPathoArray* pPatho, int iChildCol) ;
    void            getAValueTree(double dVal, string unit, string sCode, NSPatPathoArray* pPatho, int colonne) ;
    void            getAllValuesTree (NSPatPathoArray* pPatho, int iChildCol) ;
    bool            insertDate(string sCode, NVLdVTemps tTemps, NSPatPathoArray* pPatho, int colonne) ;
    bool            insertEvent(string sCode, string sEveniment, NSPatPathoArray* pPatho, int colonne) ;

    void            setGoalPatho(NSPatPathoArray* pPatho) ;

    int							getGoalType();
    void						selectObjectif()				{ _isSelected = true ; }
    void						unselectObjectif()			{ _isSelected = false ; }
    bool						isASelectedObj()				{ return _isSelected ; }

    void            initPresetIntervals() ;
    void            initPresetDelayIntervals() ;
    void            initPresetDateIntervals() ;
    void            initPresetValueIntervals() ;

    NSLdvGoalInfo::JALONSLEVELS getColorForPonctualDate(string sDate) ;

    bool            containsGoalsOnValue() { return _bValMinAutorise  || _bValMinConseille ||
                                                    _bValMinIdeal     || _bValMaxIdeal ||
                                                    _bValMaxConseille || _bValMaxAutorise ; }

    void            getValuedGoalFromIndice(int iIndice, bool *pbIsGoal, double *pdGoalValue, string *psGoalUnit) ;
    bool            canBeFullyConverted(string sUnit) ;

    bool            createXmlTree(NSLdvDocument *pDocLdv, Ctree* pTreeObject, string sTargetLocalisation = string(""), bool bRemoveTitle = false) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

	protected:

    GoalInfoArray   _aJalons ;
    GoalInfoArray   _aMetaJalons ;          // Baddest goals

    LDVFRAME        _iFrame ;    // belongs to Health, Risk or Social frame?
    NSLdvDocument  *_pDoc ;
	  //
	  // Pre-compiled coloured intervals for delays
	  //
    NSDelayZoneArray _aDelayZones ;
	  //
	  // Pre-compiled coloured intervals for starting date
	  //
	  NSDateZoneArray _aDateZones ;
	  //
	  // Pre-compiled coloured intervals for values
	  //
	  NSValueZoneArray _aValueZones ;
} ;

typedef	vector<NSLdvGoal *>	VectGoal ;
typedef	VectGoal::iterator	     ArrayGoalIter ;
typedef	VectGoal::const_iterator ArrayGoalConstIter ;

class ArrayGoals : public VectGoal
{
  public:

    ArrayGoals(NSFrameInformation *pFrame) : VectGoal(), _pFrame(pFrame), _pDoc(pFrame->getDoc()) {} ;
    ArrayGoals(NSLdvDocument *pDocum) : VectGoal(), _pDoc(pDocum), _pFrame(0) {} ;
    ArrayGoals(const ArrayGoals& rv) ;
    ~ArrayGoals() ;

    void	          vider() ;
    void	          initialiser() ;
    // void	          initHealthGoals() ;
    // void	          initRiskGoals() ;
    // void	          initSocialGoals() ;
    void	          initGoalsFromPatho(NSPatPathoArray *pPtIndex, LDVFRAME iFrame) ;
    void            loadGoals(PatPathoIter iter, int iColBase, NSPatPathoArray* pPpt, LDVFRAME iFrame, bool bJustOne = false, bool bNewGoal = false) ;
    void            reinit() ;
    void            reloadGoals(PatPathoIter iter, int iColBase, NSPatPathoArray* pPpt) ;

    void            initNewGoal(NSLdvGoal *pNewGoal) ;

    GoalInfoArray   *giveBaddestJalons(string sConcern, NVLdVRect* pRect);

    void            deleteGoal(NSLdvGoal *pGoal) ;
    NSLdvGoal		    *getGoal(string sRef) ;

    NSLdvGoal		    *makeBaddestProjection() ;

    ArrayGoals&     operator=(const ArrayGoals& src) ;

    NSFrameInformation *_pFrame ;
    NSLdvDocument      *_pDoc ;
} ;

#endif // __NSLDVGOAL_H__

