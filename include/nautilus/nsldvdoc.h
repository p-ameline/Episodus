//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSLDVDOC_H
# define __NSLDVDOC_H

# if defined(_DANSDLL)
#  define _CRDOCCLASSE __import
# endif

class NSLdvDocument ;
class NSLdvGoal ;
class NSConcern ;
class ArrayGoals ;
class NSLdvPubli ;
class NSFrameInformation ;
class NewConcernInfo ;

#ifndef NSDocumentHisto
  class NSDocumentHisto ;
#endif

# include "nautilus/nsldv_vars.h"
# include "nautilus/nsannexe.h"
# include "nautilus/nstxstyl.h"
# include "nautilus/nsdocref.h"
# include "nautilus/nsrechd2.h"
# include "nautilus/nscr_ama.h"
# include "nautilus/nsldvgoalinfo.h"
# include "nsepisod/nsldvuti.h"
# include "nsepisod/eptables.h"
# include "nsepisod/nsCoopChart.h"
# include "ns_ob1/BB1types.h"
# include "ns_ob1/OB1Node.h"
# include "nsbb/nsattval.h"
# include "nsbb/nspatpat.h"
# include "nssavoir/nsvarray.h"
# include "nsdn/nsframedocs.h"

// Index de gravité d'une préoccupation pendant une tranche de temps
// Index (severity, risk level) of a healt concern for a period of time

class NSConcernModifier
{
  protected:

    static long lObjectCount ;

  public:

    NVLdVTemps  tDateHeureDeb ;
    NVLdVTemps  tDateHeureFin ;

    int         iSeverite ;
    int         iStatFonct ;
    int         iRisque ;

    string      sRefNode ;

    NSConcern   *pConcern ;

    NSConcernModifier(NSConcern     *pThisConcern,
                      NVLdVTemps    tDateTimeDeb,
                      NVLdVTemps    tDateTimeFin,
                      int           iSeverity = 0,
                      int           iFonction = 0,
                      int           iRisk     = 0) ;
    NSConcernModifier(const NSConcernModifier& rv) ;
    ~NSConcernModifier() ;

    void               setNode(string sNode) { sRefNode = sNode ; }

    NSConcernModifier& operator=(const NSConcernModifier& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

// définition d'un array représentant la fenêtre de capture
typedef vector<NSConcernModifier *>   ArrayModifier ;
typedef ArrayModifier::iterator       ArrayModifierIter ;
typedef ArrayModifier::const_iterator ArrayModifierConstIter ;

class ArrayPbModifier : public ArrayModifier
{
  public:

    // Constructeurs - Destructeur
    ArrayPbModifier() : ArrayModifier() {}
    ArrayPbModifier(const ArrayPbModifier& rv) ;
    ~ArrayPbModifier() ;

    void             vider() ;
  //	void              initialiser(NSConcern* pConcern);
    void             addModifier(NSConcernModifier *pModifier) ;

    ArrayPbModifier& operator=(const ArrayPbModifier& src) ;
} ;

class NSConcern : public NSRoot
{
  public:

    NSLdvDocument       *_pDoc ;

    string              _sTitre ;

    string              _sSignificatif ;
    string              _sSeverite ;

    NVLdVTemps          _tDateOuverture ;
    NVLdVTemps          _tDateFermeture ;
    NVLdVTemps          _tDateDebut ;
    NVLdVTemps          _tDateAutoClose ;

    NSPatPathoArray     *_pPptDetails ;

    string              _sReference ;

    string              _sHealthIssue ;   // Health issue
    string              _sPrimoPb ;       // Problème père - Originating problem
    string              _sCocCode ;
    string              _sCimCode ;

    ArrayPbModifier     _aModificateurs ;

    GoalInfoArray       _aWorstJalons ;  // Worst goals

    NSConcern(NSContexte *pCtx, NSLdvDocument *pDocum, LDVFRAME iFrame) ;
    NSConcern(const NSConcern& rv) ;
    ~NSConcern() ;

    string              getNoeud()            { return _sReference ; }
    void                setNoeud(string sNd)  { _sReference = sNd ; }
    LDVFRAME            getIFrame()           { return _iFrame ; }
    string              getLexique()          { return _sLexique ; }
    void                setLexique(string sL) { _sLexique = sL ; }
    string              getTitle()            { return _sTitre ; }

    // int                 getLineNumber()                                           { return iLineNumber ; }
    // void                setLineNumber(int i)                                      { iLineNumber = i ; }

    void                goalAdded(NSLdvGoal *pGoal) ;
    void                goalModified(NSLdvGoal *pGoal) ;
    bool                isActiveConcern();

    int                 getMaxSeverityIndex() ;
    int                 getMinSeverityIndex() ;

    void                sendFocusEvent() ;
    void                sendActivationEvent() ;
    void                sendEvent(EventType iEvent) ;

    void                initTitle(NSPatPathoInfo* pPatho) ;

    NSQualityOfLifeChartArray* getQualityOfLifeCharts() { return &_aQualityOfLifeCharts ; }
    NSQualityOfLifeChart*      getQualityOfLifeChartAtTime(const NVLdVTemps& tTime) ;

    NSConcern&          operator=(const NSConcern& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    LDVFRAME        _iFrame ;
    string          _sLexique ;

    NSQualityOfLifeChartArray _aQualityOfLifeCharts ;

    static long lObjectCount ;
} ;

// définition d'un array représentant la fenêtre de capture
typedef vector<NSConcern *>     ArrayPb ;
typedef ArrayPb::iterator       ArrayConcernIter ;
typedef ArrayPb::const_iterator ArrayConcernConstIter ;

class ArrayConcern : public ArrayPb
{
  public:

    NSFrameInformation *_pFrame ;

    // Constructeurs - Destructeur
    ArrayConcern(NSFrameInformation *pFrame) : ArrayPb(), _pFrame(pFrame) {}
    ArrayConcern(const ArrayConcern& rv) ;
    ~ArrayConcern() ;

    void          vider() ;
    void          init(LDVFRAME iFrame) ;
    // void          initialiser() ;
    // void          initRisks() ;
    // void          initSocial() ;
    void          initFromPatho(NSPatPathoArray* pPpt) ;
    void          loadConcerns(PatPathoIter iter, int iColBase, NSPatPathoArray* pPpt) ;

    void          reinit() ;
    void          reinitFromPatho(NSPatPathoArray* pPpt) ;
    void          reloadConcerns(NSPatPathoArray* pPpt, PatPathoIter iter, int iColBase) ;

    void          deleteConcern(NSConcern *pConcern) ;

    NSConcern     *getConcern(string sRef) ;
    NSConcern     *getFils(NSConcern *pConcern) ;

    ArrayConcern& operator=(const ArrayConcern& src) ;
} ;

inline NSConcern*
ArrayConcern::getConcern(string sRef)
{
	if (empty())
		return (NSConcern*) 0 ;

	for (ArrayConcernIter i = begin() ; end() != i ; i++)
		if ((*i)->getNoeud() == sRef)
			return *i ;

	return (NSConcern*) 0 ;
}

// Partie d'un objet spécifique à un problème
class NSLdvSousObjet : public NSRoot
{
  protected:

    static long lObjectCount ;

  public:

    string     sTitre ;
    NVLdVTemps tDateHeureDebut ;
    NVLdVTemps tDateHeureFin ;

    string     sConcern ;
    string     sObject ;

    string     sReference ;

    NSLdvSousObjet(NSContexte *pCtx) ;
    NSLdvSousObjet(const NSLdvSousObjet& rv) ;
    ~NSLdvSousObjet() ;

    NSLdvSousObjet& operator=(const NSLdvSousObjet& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<NSLdvSousObjet *>   ArraySsObj ;
typedef ArraySsObj::iterator       ArraySsObjIter ;
typedef ArraySsObj::const_iterator ArraySsObjConstIter ;

class ArraySsObjet : public ArraySsObj
{
  public:

	  NSLdvDocument   *pDoc ;

    // Constructeurs - Destructeur
    ArraySsObjet(NSLdvDocument *pDocum) : ArraySsObj(), pDoc(pDocum) {}
    ArraySsObjet(const ArraySsObjet& rv) ;
    ~ArraySsObjet() ;

    void          vider() ;
    void          initialiser() ;
    void          initForConcern(NSConcern *pConcern) ;

    bool          initSmartSsObject(string sNoeud, NSConcern *pConcern) ;
    void          initStupidSsObject(string sNoeud, NSConcern *pConcern) ;

    ArraySsObjet& operator=(const ArraySsObjet& src) ;
} ;

class NSLdvObjet : public NSRoot
{
  public:

    string          _sTitre ;             // Libellé
    string          _sLexique ;           // Root code
    NVLdVTemps      _tDateHeureDebut ;
    NVLdVTemps      _tDateHeureFin ;

    string          _sConcern ;

    string          _sReference ;
    string          _sTypeDoc ;

    NSLdvObjet(NSContexte *pCtx) ;
    NSLdvObjet(const NSLdvObjet& rv) ;
    ~NSLdvObjet() ;

    NSLdvObjet&           operator=(const NSLdvObjet& src) ;

    string                getTitre()                        { return _sTitre ; }
    void                  setTitre(string sT)               { _sTitre = sT ; }
    string                getLexique()                      { return _sLexique ; }
    void                  setLexique(string sL)             { _sLexique = sL ; }
    NVLdVTemps            getDateHeureDebut()               { return _tDateHeureDebut ; }
    void                  setDateHeureDebut(NVLdVTemps tps) { _tDateHeureDebut = tps ; }
    void                  setDateHeureDebut(string sTps)    { _tDateHeureDebut.initFromDate(sTps) ; }
    NVLdVTemps            getDateHeureFin()                 { return _tDateHeureFin ; }
    void                  setDateHeureFin(NVLdVTemps tps)   { _tDateHeureFin = tps ; }
    void                  setDateHeureFin(string sTps)      { _tDateHeureFin.initFromDate(sTps) ; }

    string                getConcern()                      { return _sConcern ; }
    void                  setConcern(string sC)             { _sConcern = sC ; }

    string                getReference()                    { return _sReference ; }
    void                  setReference(string sR)           { _sReference = sR ; }
    string                getTypeDoc()                      { return _sTypeDoc ; }
    void                  setTypeDoc(string sTD)            { _sTypeDoc = sTD ; }

    NSQualityOfLifeChart* getQolChart()                            { return _pQolChart ; }
    void                  setQolChart(NSQualityOfLifeChart *pQolC) { _pQolChart = pQolC ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    NSQualityOfLifeChart *_pQolChart ;

    static long lObjectCount ;
} ;

typedef vector<NSLdvObjet *>     ArrayObj ;
typedef ArrayObj::iterator       ArrayObjIter ;
typedef ArrayObj::const_iterator ArrayObjConstIter ;

class ArrayObjets : public ArrayObj
{
  public:

    NSLdvDocument   *pDoc ;

    // Constructeurs - Destructeur
    ArrayObjets(NSLdvDocument *pDocum) : ArrayObj(), pDoc(pDocum) {}
    ArrayObjets(const ArrayObjets& rv) ;
    ~ArrayObjets() ;

    void       vider() ;
    void       initialiser() ;

    NSLdvObjet *getObjet(string sRef) ;
    void       removeObjet(string sRef) ;

    ArrayObjets& operator=(const ArrayObjets& src) ;
} ;

class NSLdvDrugPhase ;

class NSLdvDrugTake : public NSRoot
{
  protected:

    static long lObjectCount ;

  public:

    string     _sNbDoses ;
    NVLdVTemps _tpsClose ;
    string     _sAdminType ;
    string     _sAdminLocation ;
    NVLdVTemps _tpsAdminHour ;

    NSLdvDrugTake(NSContexte *pCtx) ;
    NSLdvDrugTake(const NSLdvDrugTake& rv) ;
    ~NSLdvDrugTake() ;

    void initFromTree(NSPatPathoArray* pCycleTree, PatPathoIter iter) ;

    NSLdvDrugTake& operator=(const NSLdvDrugTake& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

class NSLdvDrugCycle : public NSRoot
{
  protected:

    static long lObjectCount ;

  public:

    string          _sTitre ;
    string          _sTitreCourt ;

    double          _dDailyDose ;

    NSLdvDrugPhase* _pParentPhase ;

    NSLdvDrugCycle(NSContexte *pCtx, NSLdvDrugPhase* pParent) ;
    NSLdvDrugCycle(const NSLdvDrugCycle& rv) ;
    ~NSLdvDrugCycle() ;

    void setTitleFromTree(NSPatPathoArray* pCycleTree, PatPathoIter iter, bool bLong = true, bool bShort = true) ;

    void setTitleForCircadian(NSPatPathoArray* pCycleTree, PatPathoIter iter, bool bLong, bool bShort) ;
    void setTitleForCircadianSingleDose(NSPatPathoArray* pCycleTree, PatPathoIter iter, bool bLong, bool bShort) ;
    void setTitleForCircadianHours(NSPatPathoArray* pCycleTree, PatPathoIter iter, bool bLong, bool bShort) ;
    void setTitleForCircadianFree(NSPatPathoArray* pCycleTree, PatPathoIter iter, bool bLong, bool bShort) ;
    void setTitleForCircadianRegular(NSPatPathoArray* pCycleTree, PatPathoIter iter, bool bLong, bool bShort) ;
    void setTitleForCircadianUnegular(NSPatPathoArray* pCycleTree, PatPathoIter iter, bool bLong, bool bShort) ;

    void setTitleForNonCircadian(NSPatPathoArray* pCycleTree, PatPathoIter iter, bool bLong, bool bShort) ;

    NSLdvDrugCycle& operator=(const NSLdvDrugCycle& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<NVLdVRect*>              NVLdVRectVector ;
typedef NVLdVRectVector::iterator       NVLdVRectIter ;
typedef NVLdVRectVector::const_iterator NVLdVRectConstIter ;
typedef NTArray<NVLdVRect>              NVLdVRectArray ;

typedef vector<NSLdvDrugCycle*>              NSLdvDrugCycleVector ;
typedef NSLdvDrugCycleVector::iterator       NSLdvDrugCycleIter ;
typedef NSLdvDrugCycleVector::const_iterator NSLdvDrugCycleConstIter ;
typedef NTArray<NSLdvDrugCycle>              NSLdvDrugCycleArray ;

class NSLdvDrug ;

class NSLdvDrugPhase : public NSRoot
{
  protected:

    static long lObjectCount ;

  public:

    string              _sReference ;

    string              _sTitre ;
    string              _sTitreCourt ;

    string              _sIntakeUnitLib ;
    string              _sIntakeUnitShortLib ;

    NVLdVTemps          _tDateOuverture ;
    NVLdVTemps          _tDateFermeture ;

    NVLdVRectArray      _aPrescriptionRects ;
    NSLdvDrugCycleArray _aCycles ;

    NSLdvDrug*          _pParentDrug ;

    NSLdvDrugPhase(NSContexte *pCtx, NSLdvDrug* pParent) ;
    NSLdvDrugPhase(const NSLdvDrugPhase& rv) ;
    ~NSLdvDrugPhase() ;

    void            setTitlesFromCycles(bool bLong = true, bool bShort = true) ;
    void            compressShortTitle() ;
    string          getShortestLibForIntakeUnit() ;
    void            initIntakeUnits() ;

    string          getNoeud()             { return _sReference ; }
    void            setNoeud(string sNd)   { _sReference = sNd ; }

    bool            modifyDate(string sDateType, NVLdVTemps tNewDate) ;

    NSLdvDrugPhase& operator=(const NSLdvDrugPhase& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<NSLdvDrugPhase*>              NSLdvDrugPhaseVector ;
typedef NSLdvDrugPhaseVector::iterator       NSLdvDrugPhaseIter ;
typedef NSLdvDrugPhaseVector::const_iterator NSLdvDrugPhaseConstIter ;
typedef NTArray<NSLdvDrugPhase>              NSLdvDrugPhaseArray ;

// Médicaments
class NSLdvDrug : public NSRoot
{
  public:

    NSLdvDocument*      _pDoc ;

    string              _sTitre ;
    string              _sTitreCourt ;

    NVLdVTemps          _tDateOuverture ;
    NVLdVTemps          _tDateFermeture ;

    string              _sIntakeUnit ;

    NSLdvDrugPhaseArray _aPhases ;

    string              _sFreeText ;
    bool                _bALD ;

    //vecteur des ids noeud preocupation
    VecteurString       _aConcerns ;

    GoalInfoArray*      _pWorstJalons ;  // Worst goals

    NSLdvDrug(NSContexte *pCtx, NSLdvDocument* pDocum, LDVFRAME iFrame) ;
    NSLdvDrug(const NSLdvDrug& rv) ;
    ~NSLdvDrug() ;

    string          getNoeud()             { return _sReference ; }
    void            setNoeud(string sNd)   { _sReference = sNd ; }
    string          getLexique()           { return _sLexique ; }
    void            setLexique(string sLx) { _sLexique = sLx ; }
    string          getAtcCode()           { return _sAtcCode ; }
    void            setAtcCode(string sAC) { _sAtcCode = sAC ; }
    string          getCiCode()            { return _sCiCode ; }
    void            setCiCode(string sCC)  { _sCiCode = sCC ; }
    LDVFRAME        getIFrame()            { return _iFrame ; }

    string          getLatestPrescriptionDoc()           { return _sLatestPrescriptionDoc ; }
    void            setLatestPrescriptionDoc(string sL)  { _sLatestPrescriptionDoc = sL ; }
    string          getLatestPrescriptionDate()          { return _sLatestPrescriptionDate ; }
    void            setLatestPrescriptionDate(string sL) { _sLatestPrescriptionDate = sL ; }

    bool            isToPrescribe()          { return _bToBePrescribed ; }
    void            setToPrescribe(bool bTP) { _bToBePrescribed = bTP ; }

    void            setTitlesFromLexique() ;

    void            initPhases(NSPatPathoArray* pTree, PatPathoIter iter) ;
    NSLdvDrugPhase* getPhaseFromNode(string sNode) ;
    NSLdvDrugPhase* getCurrentActivePhase() ;
    NSLdvDrugPhase* getLastActivePhase() ;
    bool            getCurrentActivePhasePpt(NSPatPathoArray *pPPT) ;

    void            goalAdded(NSLdvGoal *pGoal) ;
    void            goalModified(NSLdvGoal *pGoal) ;

    bool            bIsLinkedConcern(string sConcern) ;

    void            sendFocusEvent() ;
    void            sendActivationEvent() ;
    void            sendEvent(EventType iEvent) ;

    bool            modifyPhaseDate(string sPhaseNode, string sDateType, NVLdVTemps tNewDate) ;
    bool            modifyPhaseDate(NSLdvDrugPhase* pPhase, string sDateType, NVLdVTemps tNewDate) ;
    NVLdVTemps      getPhaseDate(string sPhaseNode, string sDateType) ;
    NVLdVTemps      getPhaseDate(NSLdvDrugPhase* pPhase, string sDateType) ;

    string          getPrescriptionDate() ;

    bool            addNewPhase(NSPatPathoArray* pPhaseTree) ;
    bool            addNewActivePhase(NSPatPathoArray* pPhaseTree) ;

    bool            createXmlTree(Ctree* pTreeObject, string sTargetLocalisation = string("")) ;

    NSLdvDrug&      operator=(const NSLdvDrug& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    LDVFRAME    _iFrame ;

    string      _sReference ;
    string      _sLexique ;
    string      _sAtcCode ;
    string      _sCiCode ;

    string      _sLatestPrescriptionDoc ;
    string      _sLatestPrescriptionDate ;

    bool        _bToBePrescribed ;

    static long lObjectCount ;
} ;

typedef vector<NSLdvDrug *>        ArrayDrugs ;
typedef ArrayDrugs::iterator       drugsIter ;
typedef ArrayDrugs::const_iterator drugsConstIter ;

class ArrayLdvDrugs : public ArrayDrugs
{
  public:

    NSFrameInformation *_pFrame ;

    // Constructeurs - Destructeur
    ArrayLdvDrugs(NSFrameInformation *pFrame) : ArrayDrugs(), _pFrame(pFrame) {}
    ArrayLdvDrugs(const ArrayLdvDrugs& rv) ;
    ~ArrayLdvDrugs() ;

    void            vider() ;
    void            initialiser() ;
    void            initDrugsFromPatho(NSPatPathoArray* pPpt) ;
    void            loadDrugs(NSPatPathoArray *pPt, PatPathoIter iter, int iColBase, bool bJustOne = false) ;
    void            reinit() ;
    void            reinitDrugsFromPatho(NSPatPathoArray* pPpt) ;
    void            reloadDrugs(NSPatPathoArray *pPt, PatPathoIter iter, int iColBase, bool bJustOne = false) ;
    bool            deleteDrug(NSLdvDrug *pDrug) ;
    bool            deleteDrug(string sRef) ;
    bool            getRenewPatPatho(NSPatPathoArray* pPPT, string sNodeRenew) ;
    string          getLastRenewNode(string sNodeMedic) ;
    NSLdvDrug       *getDrug(string sRef) ;

    ArrayLdvDrugs&  operator=(const ArrayLdvDrugs& src) ;
} ;

class ArrayCopyDrugs : public ArrayDrugs
{
  public:

    // Constructeurs - Destructeur
    ArrayCopyDrugs() : ArrayDrugs() {}
    ArrayCopyDrugs(const ArrayCopyDrugs& rv) ;
    ~ArrayCopyDrugs() ;

    void   vider() ;

    ArrayCopyDrugs& operator=(const ArrayCopyDrugs& src) ;
} ;

class NSDrugView ;
class NSGoalView ;
class NSProcessView ;

class NSFrameInformation : public NSRoot
{
  public:

    NSFrameInformation(NSContexte *pCtx, NSLdvDocument *pDoc, LDVFRAME iFrame, NSFrameDocuments *pFrameDocs) ;
	  NSFrameInformation(const NSFrameInformation& rv) ;
	  ~NSFrameInformation() ;

    void initConcerns() ;
    void initTreatments() ;
    void initGoals() ;

	  NSFrameInformation& operator=(const NSFrameInformation& src) ;

    LDVFRAME         getFrame()           { return _iFrame ; }

    ArrayConcern*    getConcernsArray()   { return _pConcerns ; }
    ArrayGoals*      getGoalsArray()      { return _pGoals ; }
    ArrayLdvDrugs*   getTreatmentsArray() { return _pTreatments ; }

    NSDocumentHisto* getIndexDocument()   { return _pFrameDocs->getIndexDocument() ; }

    // NSPatPathoArray* getPatPathoIndex()   { return _pFrameDocs->getPatPathoIndex() ; }
    void             setPatPathoIndex(const NSPatPathoArray* pPpt)      { _pFrameDocs->setPatPathoIndex(pPpt) ; }
    void             initFromPatPathoIndex(NSPatPathoArray* pPpt) const { _pFrameDocs->initFromPatPathoIndex(pPpt) ; }

    NSDocumentHistoArray* getConcernDocs()     { return _pFrameDocs->getConcernDocs() ; }
    NSDocumentHistoArray* getTreatmentDocs()   { return _pFrameDocs->getTreatmentDocs() ; }
    NSDocumentHistoArray* getGoalDocs()        { return _pFrameDocs->getGoalDocs() ; }

    NSLdvDocument*   getDoc()             { return _pDoc ; }

    NSConcern*       getConcern(string sNode) ;
    NSLdvDrug*       getTreatment(string sNode) ;
    NSLdvGoal*       getGoal(string sNode) ;

    NSDocumentHisto* findNode(string sNodeId) { return _pFrameDocs->findNode(sNodeId) ; }

    void addConcernDocument(NSDocumentHisto* pDoc)   { _pFrameDocs->addConcernDocument(pDoc) ; }
    void addTreatmentDocument(NSDocumentHisto* pDoc) { _pFrameDocs->addTreatmentDocument(pDoc) ; }
    void addGoalsDocument(NSDocumentHisto* pDoc)     { _pFrameDocs->addGoalsDocument(pDoc) ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    LDVFRAME         _iFrame ;

    NSFrameDocuments *_pFrameDocs ;

    ArrayConcern     *_pConcerns ;
    ArrayGoals       *_pGoals ;
    ArrayLdvDrugs    *_pTreatments ;

    NSLdvDocument    *_pDoc ;

    static long lObjectCount ;

    // void loadSubTrees() ;
    // void loadSubTreesForDataType(string sDocRoot, NSDocumentHistoArray *pTank, NSRootLink::NODELINKTYPES iRelation) ;

    NSDocumentHisto* findNodeInArray(string sNodeId, NSDocumentHistoArray* pDocsArray) ;
} ;

// VECTEUR DE PATPATHO
typedef vector<NSFrameInformation*>	NSVectFrameInformation ;
typedef NSVectFrameInformation::iterator       FrameInfoIter ;
typedef NSVectFrameInformation::const_iterator FrameInfoConstIter ;

class _NSBBCLASSE NSFrameInformationArray : public NSVectFrameInformation
{
  public:

    // Constructeurs
    NSFrameInformationArray(NSLdvDocument *pDoc) ;
    NSFrameInformationArray(const NSFrameInformationArray& rv) ;

    // Destructeur
    virtual ~NSFrameInformationArray() ;
    void vider() ;

    void                openFrameInformation(LDVFRAME iFrame) ;
    void                initConcernsForFrameInformation(LDVFRAME iFrame) ;
    void                initTreatmentsForFrameInformation(LDVFRAME iFrame) ;
    void                initGoalsForFrameInformation(LDVFRAME iFrame) ;

    NSFrameInformation* getFrameInformation(LDVFRAME iFrame) ;

    // Opérateurs
    NSFrameInformationArray& operator=(const NSFrameInformationArray& src) ;

 protected:

    NSLdvDocument *_pDoc ;
} ;

class NSLdvDocument : public TDocument, public NSRoot
{
  protected:

    ArrayObjets               _aObjets ;
    ArraySsObjet              _aSousObjets ;
    NSQualityOfLifeChartArray _aQoLCharts ;

    NSFrameInformationArray aFrameIndexes ;

    // For printing purposes
    NSPrintControl  _printControl ;

    static long lObjectCount ;

  public:

    enum LDVSERVICESERROR { ldvseNoError = 0,       ldvseUndefined,
                            ldvseNothingToDo,
                            ldvseInsuficientRights, ldvseCanceledByUser,
                            ldvseInvalidData,       ldvseInterfaceError
                          } ;

    string          _sLangue ;

    NSPublication   *_pPubli ;

    NSLdvDocument(TDocument *parent, NSContexte *pCtx = 0) ;
    NSLdvDocument(const NSLdvDocument& rv) ;
    ~NSLdvDocument() ;

    NSLdvDocument& operator=(const NSLdvDocument& src) ;

    ArrayObjets               *getObjets()     { return &_aObjets ; }
    ArraySsObjet              *getSsObjets()   { return &_aSousObjets ; }
    NSQualityOfLifeChartArray *getQoLCharts()  { return &_aQoLCharts ; }

    NSFrameInformationArray *getFrames() { return &aFrameIndexes ; }

    ArrayConcern    *getConcerns(LDVFRAME iFrame) ;
    ArrayGoals      *getGoals(LDVFRAME iFrame) ;
    ArrayLdvDrugs   *getDrugs(LDVFRAME iFrame) ;
    // NSPatPathoArray *getFrameIndex(LDVFRAME iFrame) ;
    void             initFromFrameIndex(LDVFRAME iFrame, NSPatPathoArray* pPpt) const ;

    // NSPatPathoArray *getPOMRIndex()   { return _pPathoPOMRIndex ;   }
    // NSPatPathoArray *getRiskIndex()   { return _pPathoRiskIndex ;   }
    // NSPatPathoArray *getSocialIndex() { return _pPathoSocialIndex ; }

    NSConcern* getConcern(string sNode, LDVFRAME &iFrame) ;
    NSLdvDrug* getTreatment(string sNode, LDVFRAME &iFrame) ;
    NSLdvGoal* getGoal(string sNode, LDVFRAME &iFrame) ;

    NSFrameInformation* getFrameForNode(string sNode) ;
    LDVFRAME            getFrameTypeFromRoot(string sRootCode) ;

    void loadFrames() ;

    // Drug oriented services
    //
    LDVSERVICESERROR DrugNewService(TWindow* pCallingView, string sDrugLexiCode = string(""), VecteurString* pRelatedConcerns = NULL) ;
    LDVSERVICESERROR DrugFromProtocolService(TWindow* pCallingView, VecteurString* pRelatedConcerns = NULL) ;
    LDVSERVICESERROR DrugRenewService(TWindow* pCallingView, string sDrugNode) ;
    LDVSERVICESERROR DrugModifyService(TWindow* pCallingView, string sDrugNode) ;
    LDVSERVICESERROR DrugChangePosoService(TWindow* pCallingView, string sDrugNode) ;
    LDVSERVICESERROR DrugChangePosoService(TWindow* pCallingView, NSLdvDrug* pLdvDrug) ;
    LDVSERVICESERROR DrugStopService(TWindow* pCallingView, string sDrugNode) ;
    LDVSERVICESERROR DrugDeleteService(TWindow* pCallingView, string sDrugNode) ;
    LDVSERVICESERROR DrugCreatePrescriptionService(TWindow* pCallingView) ;
    LDVSERVICESERROR DrugCreatePrescriptionForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs) ;

    LDVSERVICESERROR DrugCreateProtocolForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs) ;
    LDVSERVICESERROR DrugAddToProtocolForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs) ;
    LDVSERVICESERROR DrugSelectProtocolForConcern(TWindow* pCallingView, string sRelatedConcern) ;

    // Goal oriented services
    //
    LDVSERVICESERROR GoalFromProtocolService(TWindow* pCallingView, LDVFRAME iFrame, VecteurString* pRelatedConcerns = NULL) ;
    LDVSERVICESERROR GoalSelectProtocolForConcern(TWindow* pCallingView, string sRelatedConcern) ;
    LDVSERVICESERROR GoalSelectProtocolForRisk(TWindow* pCallingView, string sRelatedConcern) ;
    LDVSERVICESERROR GoalSelectProtocolForSocialConcern(TWindow* pCallingView, string sRelatedConcern) ;

    LDVSERVICESERROR GoalCreateProtocolForSelectionService(TWindow* pCallingView, VecteurString* pGoalsRefs) ;
    LDVSERVICESERROR GoalAddToProtocolForSelectionService(TWindow* pCallingView, VecteurString* pGoalsRefs) ;

    // Time oriented services
    //
    LDVSERVICESERROR NewAppointmentService(TWindow* pCallingView) ;

    // ICPC oriented services
    //
    LDVSERVICESERROR IcpcDusoiPptFromValues(TWindow* pCallingView, NSPatPathoArray *pPatho, int iScore, int iSymptom, int iComplication, int iPrognosis, int iTreatability) ;
    LDVSERVICESERROR IcpcDusoiValuesFromPpt(TWindow* pCallingView, NSPatPathoArray *pPatho, int* piScore, int* pSymptom, int* pComplication, int* pPrognosis, int* pTreatability) ;

    // Blackboard oriented services
    //
    LDVSERVICESERROR GetProjectKsForLine(TWindow* pCallingView, std::vector<OB1NKS*> *kss, string sCode) ;

    //
    //
    void             Publish(NSLdvPubli *pPubliDriver, bool bCorrespPatient = true) ;
    bool             TemplateCompo(string typeDoc, string& tempDoc, string& enteteDoc) ;
    bool             GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation, NSLdvPubli *pPubliDriver, string sAdresseCorresp = "", string sPathDest = "") ;

    // ajouter une méthode qui récupère des objets à partir de la basketlist de NSEpisodus
    // faire une méthode
    void            addFromBasketList() ;

    void            addConcern  (LDVFRAME iFrame, string sTitle, string sDateDeb, string sDateFin, string sPb, string sRef, string sPbOrigine = "") ;
    void            addObjet    (string sTitle, string sDateDeb, string sDateFin, string sPb, string sRef, string sTypDoc, string sTypeContenu) ;
    void            removeObjet (string sRef) ;
    void            addSsObjet  (string sTitle, string sDateDeb, string sDateFin, string sPb, string sRef, string sObjet) ;
    void	          addGoal     (LDVFRAME iFrame, string sTitle, string sDateDeb, string sDateFin, string sPb, string sRef) ;
    void	          addDrug     (LDVFRAME iFrame, string sTitle, string sDateDeb, string sDateFin, string sPb, string sRef) ;

    void            addQualityOfLifeCharts(string sDate, NSPatPathoArray *pPatho) ;
    void            modifyQualityOfLifeCharts(string sDate, NSPatPathoArray *pPatho) ;
    void            addQualityOfLifeCharts(string sConcern, string sDate, NSPatPathoArray *pPatho) ;
    void            modifyQualityOfLifeCharts(string sConcern, string sDate, NSPatPathoArray *pPatho) ;

    NSQualityOfLifeChart* createQualityOfLifeChart(string sDate, NSPatPathoArray *pPatho) ;

    NSDocumentHisto* getStorageDocument(LDVFRAME iFrame, NSRootLink::NODELINKTYPES iRelation) ;
    NSDocumentHisto* getConcernsStorageDocument(LDVFRAME iFrame) { return getStorageDocument(iFrame, NSRootLink::indexConcerns) ; }
    NSDocumentHisto* getDrugsStorageDocument(LDVFRAME iFrame)    { return getStorageDocument(iFrame, NSRootLink::drugOf) ; }
    NSDocumentHisto* getGoalsStorageDocument(LDVFRAME iFrame)    { return getStorageDocument(iFrame, NSRootLink::indexGoals) ; }

    bool            getStorageDocumentsIds(VecteurString *pIdsVector, LDVFRAME iFrame, NSRootLink::NODELINKTYPES iRelation) ;
    bool            getStorageDocumentsIds(VecteurString *pIdsVector, string sFrameIndexMeta, NSRootLink::NODELINKTYPES iRelation) ;

    NSDocumentHisto* newStorageDocument(NSDocumentHisto* pRootDoc, NSRootLink::NODELINKTYPES iRelation) ;

    NSDocumentHisto* getMoreRecentDocument(VecteurString* pDocumentsIds) ;

    int              getStorageDocumentCardinality(NSDocumentHisto* pDoc, string sChapter) ;

    // bool            newConcern(string sLexique, string *psNode, string sDateDeb, string sDateFin, int iGravite, string sCocCode, int iRisque, NSPatPathoArray* pDetails, string sTL = "") ;
    // bool            newRisk(string sLexique, string *psNode, string sDateDeb, string sDateFin, string sCocCode, NSPatPathoArray* pDetails, string sTL = "") ;
    // bool            newLine(LDVFRAME iFrame, string sLexique, string *psNode, string sDateDeb, string sDateFin, int iSeverity, string sCocCode, NSPatPathoArray* pDetails, string sTL = "") ;
    bool            newLine(LDVFRAME iFrame, string& sNode, NewConcernInfo *pConcernInfo) ;
    bool            evolConcern(NSConcern *pPere, string sLexique, string sDateDeb, string sDateFin) ;

    bool            changeConcernDateDeb(NSConcern *pConcern, NVLdVTemps *pDateDeb) ;
    bool            changeConcernDateFin(NSConcern *pConcern, NVLdVTemps *pDateFin) ;
    bool            changeConcernModifier(NSConcern *pConcern, NSConcernModifier *pModifier, int iSev, int iRisk) ;
    bool            changeConcernType(LDVFRAME iFrame, NSConcern *pConcern, NSPatPathoInfo *pNode) ;
    void            showConcernProperty(NSConcern *pConcern) ;

    bool            evolveConcernModifier(LDVFRAME iFrame, NSConcern *pConcern, NVLdVTemps *pDateDeb, int iSev, int iRisk) ;

    bool            creeConcernSuite(LDVFRAME iFrame, NSConcern *pConcernPere, NSConcern *pConcernFils) ;

    bool            chgConcernType(string sLexique) ;
    bool            chgConcernDate(string sDateDeb, string sDateFin) ;

    bool            deleteConcern(LDVFRAME iFrame, NSConcern *pConcern) ;
    void            manageRights(LDVFRAME iFrame, TWindow* parent, string sNode) ;

    bool            updateIndexDocument(NSDocumentHisto* pDocument, bool bUpdateDisplay = true) ;
    // bool            updateGenericIndex(LDVFRAME iFrame) ;
    // bool            updateIndex()       { return updateGenericIndex(_pPathoPOMRIndex, string("ZPOMR")) ; }
    // bool            updateRiskIndex()   { return updateGenericIndex(_pPathoRiskIndex, string("ORISK")) ; }
    // bool            updateSocialIndex() { return updateGenericIndex(_pPathoSocialIndex, string("ZPSOC")) ; }

    // Generic
    //
    PatPathoIter    getElement(LDVFRAME iFrame, string sNodeId, NSPatPathoArray **ppPatho) ;

    // Health frame
    //
    PatPathoIter    donneElement(string sNodeId, NSPatPathoArray **pPatho) ;
    PatPathoIter    donnePreoccup(NSConcern *pConcern, NSPatPathoArray **pPatho) ;
    PatPathoIter    donneGoal(NSLdvGoal *pGoal, NSPatPathoArray **pPatho) ;
    PatPathoIter    donneDrug(NSLdvDrug *pDrug, NSPatPathoArray **pPatho) ;
    NSConcern*      donneOpenConcern(LDVFRAME iFrame, string sCodeSens) ;
    NSLdvDrug*      donneMostRecentOpenDrug(LDVFRAME iFrame, string sCodeSens) ;
    NSLdvDrug*      donneNextRecentOpenDrug(LDVFRAME iFrame, string sCodeSens, NSLdvDrug* pPreviousDrug) ;

    // Risk frame
    //
    PatPathoIter    getRiskElement(string sNodeId, NSPatPathoArray **pPatho) ;

    // Social frame
    //
    PatPathoIter    getSocialElement(string sNodeId, NSPatPathoArray **pPatho) ;

    bool            connectObjectToConcern(LDVFRAME iFrame, string sCodeConcern, int iSeverity, string sNoeud, NSRootLink::NODELINKTYPES iRelation, bool bAutoCreate) ;
    bool            connectObjectToDrug(LDVFRAME iFrame, string sCodeDrug, int iSeverity, string sNoeud, NSRootLink::NODELINKTYPES iRelation, bool bAutoCreate) ;
    void            showNewTree(const NSPatPathoArray *pPatPathoArray, string sDateDoc) ;
    void            showNewTreeToBbk(const NSPatPathoArray *pPatPathoArray, string sDateDoc) ;
    void            showNewDrug(const NSPatPathoArray *pPatPathoArray, PatPathoIter iter) ;
    bool            isALDDrug(string sNodeMedic) ;
    void            connectValuedNode(NSPatPathoArray *pPatPathoArray, PatPathoIter iter, string sGoalNode) ;
    void            showDrugProperty(NSLdvDrug *pDrug) ;

    void            addGoalToConcern(NSLdvGoal *pAddedGoal) ;
    void            goalModified(NSLdvGoal *pModifiedGoal) ;
    void            reinitAllGoals() ;
    void            closeGoal(NSLdvGoal *pGoal, NVLdVTemps *pDateFin, NSPatPathoArray *pPatPathoMotif) ;
    void            closeGoal(NSLdvGoal *pGoal, NSPatPathoArray *pPatPathoMotif) ;  // Now
    void            goalAdded(NSLdvGoal* pGoal, GoalInfoArray *pWorstJalons) ;
    void            goalModifiedForConcern(NSLdvGoal* pGoal, NSConcern *pConcern) ;
    void            goalModifiedForDrug(NSLdvGoal* pGoal, NSLdvDrug *pDrug) ;

    void            invalidateViews(string sReason, string sReference = string("")) ;
    void            viewsReloadDrugToons() ;

    NSLdvView       *getLdvView(string sConcern = string("")) ;
    NSDrugView      *getDrugView(string sConcern) ;
    NSGoalView      *getGoalView(string sConcern) ;
    NSProcessView   *getProcessView(string sConcern) ;

    bool            openLdVObject(NSLdvObjet *pObj) ;
    bool            openLdVObjectsInArray(ArrayObjets *pObjects, NSLdvView *pView) ;
    // implementation des méthodes virtuelles de TDocument
    bool   	        Open(int mode, const char far *path = 0) ;
  //	bool            Close() ;
  //	bool	          CanClose() ;
    // bool   	        IsOpen() { return (false == _pConcerns->empty()) ; }

    void						traiteSelectedObjectifs(string sConcern);
    //void 						transferInDrugView(string sConcern,  NSPatPathoArray *pPPT) ;
    void						transferInDrugView(string sConcern,  NSLdvGoal *pGoal, bool initLogMed) ;
    void						transferInProccess(string sConcern,  ArrayGoals *pGoals) ;

    void            initGoal(NSLdvGoal* pGoal) ;
    void            initGoalForDrug(NSLdvGoal* pGoal) ;
    void            initGoalForAct(NSLdvGoal* pGoal) ;
    void            initGoalForNumValue(NSLdvGoal* pGoal) ;

    void            findCocCode(bool bVerbose, string &sCocCode, string sLexCode, string sLabel, string sClassif, string sPostCase) ;

    // Getters for print params

    NSPrintControl* getPrintControler() { return &_printControl ; }

    string getNomDocHtml()     { return _printControl.getNomDocHtml() ; }
    int    getNbImpress()      { return _printControl.getNbImpress() ; }
    bool   getImprimerLettre() { return _printControl.getImprimerLettre() ; }
    string getCodeLettre()     { return _printControl.getCodeLettre() ; }
    int    getIndexCorresp()   { return _printControl.getIndexCorresp() ; }

    // Setters for print params

    void setNomDocHtml(string sNDH)  { _printControl.setNomDocHtml(sNDH) ; }
    void setNbImpress(int iNb)       { _printControl.setNbImpress(iNb) ; }
    void setImprimerLettre(bool bIL) { _printControl.setImprimerLettre(bIL) ; }
    void setCodeLettre(string sCod)  { _printControl.setCodeLettre(sCod) ; }
    void setIndexCorresp(int iIC)    { _printControl.setIndexCorresp(iIC) ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

ArrayGoals*
NSLdvDocument::getGoals(LDVFRAME iFrame)
{
  NSFrameInformation* pFrameInfo = aFrameIndexes.getFrameInformation(iFrame) ;
  if ((NSFrameInformation*) NULL == pFrameInfo)
    return (ArrayGoals*) 0 ;

  return pFrameInfo->getGoalsArray() ;
}

NSFrameInformation*
NSFrameInformationArray::getFrameInformation(LDVFRAME iFrame)
{
  if (empty())
    return (NSFrameInformation*) 0 ;

  for (FrameInfoIter i = begin() ; end() != i ; i++)
    if ((*i)->getFrame() == iFrame)
      return *i ;

  return (NSFrameInformation*) 0 ;
}

#endif // __NSLDVDOC_H

