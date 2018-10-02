//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSLDVDOCBASE_H
# define __NSLDVDOCBASE_H

# if defined(_DANSDLL)
#  define _CRDOCCLASSE __import
# endif

class NSLdvDocumentBase ;
class NSLdvGoal ;
class NSLdvDrug ;
class NSConcern ;
class ArrayGoals ;
class ArrayLdvDrugs ;
class ArrayCopyDrugs ;
class NSLdvPubli ;
class NSFrameInformation ;
class NewConcernInfo ;
class NSLdvViewBase ;

#ifndef NSDocumentHisto
  class NSDocumentHisto ;
#endif

#ifndef __NSBDMDRIVER_H
  class NsSelectableDrug ;
  class NsSelectableDrugArray ;
#endif

# include "nsldv/nsldv_vars.h"
# include "ns_sgbd/nsannexe.h"
# include "dcodeur/nstxstyl.h"
# include "nautilus/nsdocref.h"
# include "nautilus/nsrechd2.h"
# include "dcodeur/nscr_ama.h"
# include "nsldv/nsldvgoalinfo.h"
# include "nsldv/nsldvuti.h"
# include "nsepisod/eptables.h"
# include "nsepisod/nsCoopChart.h"
# include "ns_ob1/BB1types.h"
# include "ns_ob1/OB1Node.h"
# include "nsbb/nsattval.h"
# include "nsbb/nspatpat.h"
# include "nssavoir/nsvarray.h"
# include "nsdn/nsframedocs.h"

# include "nsldv/nsldvexport.h"

// Index de gravit� d'une pr�occupation pendant une tranche de temps
// Index (severity, risk level) of a healt concern for a period of time

class _NSLDVCLASSE NSConcernModifier
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

// d�finition d'un array repr�sentant la fen�tre de capture
typedef vector<NSConcernModifier*>    ArrayModifier ;
typedef ArrayModifier::iterator       ArrayModifierIter ;
typedef ArrayModifier::const_iterator ArrayModifierConstIter ;

class _NSLDVCLASSE ArrayPbModifier : public ArrayModifier
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

class _NSLDVCLASSE NSConcern : public NSRoot
{
  public:

    NSLdvDocumentBase       *_pDoc ;

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
    string              _sPrimoPb ;       // Probl�me p�re - Originating problem
    string              _sCocCode ;
    string              _sCimCode ;

    ArrayPbModifier     _aModificateurs ;

    GoalInfoArray       _aWorstJalons ;  // Worst goals

    NSConcern(NSContexte *pCtx, NSLdvDocumentBase *pDocum, LDVFRAME iFrame) ;
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

// d�finition d'un array repr�sentant la fen�tre de capture
typedef vector<NSConcern *>     ArrayPb ;
typedef ArrayPb::iterator       ArrayConcernIter ;
typedef ArrayPb::const_iterator ArrayConcernConstIter ;

class _NSLDVCLASSE ArrayConcern : public ArrayPb
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

// Partie d'un objet sp�cifique � un probl�me
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

class _NSLDVCLASSE ArraySsObjet : public ArraySsObj
{
  public:

	  NSLdvDocumentBase   *pDoc ;

    // Constructeurs - Destructeur
    ArraySsObjet(NSLdvDocumentBase *pDocum) : ArraySsObj(), pDoc(pDocum) {}
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

    string          _sTitre ;             // Libell�
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

class _NSLDVCLASSE ArrayObjets : public ArrayObj
{
  public:

    NSLdvDocumentBase   *pDoc ;

    // Constructeurs - Destructeur
    ArrayObjets(NSLdvDocumentBase *pDocum) : ArrayObj(), pDoc(pDocum) {}
    ArrayObjets(const ArrayObjets& rv) ;
    ~ArrayObjets() ;

    void       vider() ;
    void       initialiser() ;

    NSLdvObjet *getObjet(string sRef) ;
    void       removeObjet(string sRef) ;

    ArrayObjets& operator=(const ArrayObjets& src) ;
} ;

class _NSLDVCLASSE NSFrameInformation : public NSRoot
{
  public:

    NSFrameInformation(NSContexte *pCtx, NSLdvDocumentBase *pDoc, LDVFRAME iFrame, NSFrameDocuments *pFrameDocs) ;
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

    NSLdvDocumentBase*   getDoc()             { return _pDoc ; }

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

    NSLdvDocumentBase    *_pDoc ;

    static long lObjectCount ;

    // void loadSubTrees() ;
    // void loadSubTreesForDataType(string sDocRoot, NSDocumentHistoArray *pTank, NSRootLink::NODELINKTYPES iRelation) ;

    NSDocumentHisto* findNodeInArray(string sNodeId, NSDocumentHistoArray* pDocsArray) ;
} ;

// VECTEUR DE PATPATHO
typedef vector<NSFrameInformation*>	NSVectFrameInformation ;
typedef NSVectFrameInformation::iterator       FrameInfoIter ;
typedef NSVectFrameInformation::const_iterator FrameInfoConstIter ;

class _NSLDVCLASSE NSFrameInformationArray : public NSVectFrameInformation
{
  public:

    // Constructeurs
    NSFrameInformationArray(NSLdvDocumentBase *pDoc) ;
    NSFrameInformationArray(const NSFrameInformationArray& rv) ;

    // Destructeur
    virtual ~NSFrameInformationArray() ;
    void vider() ;

    void                openFrameInformation(LDVFRAME iFrame) ;
    void                initConcernsForFrameInformation(LDVFRAME iFrame) ;
    void                initTreatmentsForFrameInformation(LDVFRAME iFrame) ;
    void                initGoalsForFrameInformation(LDVFRAME iFrame) ;

    NSFrameInformation* getFrameInformation(LDVFRAME iFrame) ;

    // Op�rateurs
    NSFrameInformationArray& operator=(const NSFrameInformationArray& src) ;

 protected:

    NSLdvDocumentBase *_pDoc ;
} ;

class _NSLDVCLASSE NSLdvDocumentBase : public TDocument, public NSRoot
{
  protected:

    ArrayObjets               _aObjets ;
    ArraySsObjet              _aSousObjets ;
    NSQualityOfLifeChartArray _aQoLCharts ;

    NSFrameInformationArray aFrameIndexes ;

  public:

    enum LDVSERVICESERROR { ldvseNoError = 0,       ldvseUndefined,
                            ldvseNothingToDo,
                            ldvseInsuficientRights, ldvseCanceledByUser,
                            ldvseInvalidData,       ldvseInterfaceError
                          } ;

    string          _sLangue ;

    NSLdvDocumentBase(TDocument *parent, NSContexte *pCtx = 0) ;
    NSLdvDocumentBase(const NSLdvDocumentBase& rv) ;
    virtual ~NSLdvDocumentBase() ;

    NSLdvDocumentBase& operator=(const NSLdvDocumentBase& src) ;

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
    LDVSERVICESERROR DrugCreatePrescriptionForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs, bool bSecured) ;
    LDVSERVICESERROR DrugCreateNoNamePrescriptionForSelectionService(TWindow* pCallingView, VecteurString* pDrugsRefs, bool bSecured) ;
    LDVSERVICESERROR DrugSwitchToDciForSelectionService(TWindow* pCallingView, ArrayCopyDrugs* pDrugs) ;

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
    // removed void             Publish(NSLdvPubli *pPubliDriver, bool bCorrespPatient = true) ;
    bool             TemplateCompo(string typeDoc, string& tempDoc, string& enteteDoc) ;
    // remove bool             GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation, NSLdvPubli *pPubliDriver, string sAdresseCorresp = "", string sPathDest = "") ;

    // ajouter une m�thode qui r�cup�re des objets � partir de la basketlist de NSEpisodus
    // faire une m�thode
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

    // Removed NSDocumentHisto* getStorageDocument(LDVFRAME iFrame, NSRootLink::NODELINKTYPES iRelation) ;
    // Removed NSDocumentHisto* getConcernsStorageDocument(LDVFRAME iFrame) { return getStorageDocument(iFrame, NSRootLink::indexConcerns) ; }
    // Removed NSDocumentHisto* getDrugsStorageDocument(LDVFRAME iFrame)    { return getStorageDocument(iFrame, NSRootLink::drugOf) ; }
    // Removed NSDocumentHisto* getGoalsStorageDocument(LDVFRAME iFrame)    { return getStorageDocument(iFrame, NSRootLink::indexGoals) ; }

    virtual bool    getStorageDocumentsIds(VecteurString *pIdsVector, LDVFRAME iFrame, NSRootLink::NODELINKTYPES iRelation) = 0 ;
            bool    getStorageDocumentsIds(VecteurString *pIdsVector, string sFrameIndexMeta, NSRootLink::NODELINKTYPES iRelation) ;

    // Removed NSDocumentHisto* newStorageDocument(NSDocumentHisto* pRootDoc, NSRootLink::NODELINKTYPES iRelation) ;

    NSDocumentHisto* getMoreRecentDocument(VecteurString* pDocumentsIds) ;

    int              getStorageDocumentCardinality(NSDocumentHisto* pDoc, string sChapter) ;

    // bool            newConcern(string sLexique, string *psNode, string sDateDeb, string sDateFin, int iGravite, string sCocCode, int iRisque, NSPatPathoArray* pDetails, string sTL = "") ;
    // bool            newRisk(string sLexique, string *psNode, string sDateDeb, string sDateFin, string sCocCode, NSPatPathoArray* pDetails, string sTL = "") ;
    // bool            newLine(LDVFRAME iFrame, string sLexique, string *psNode, string sDateDeb, string sDateFin, int iSeverity, string sCocCode, NSPatPathoArray* pDetails, string sTL = "") ;
    // Removed bool            newLine(LDVFRAME iFrame, string& sNode, NewConcernInfo *pConcernInfo) ;
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
    void            setDrugPrescriptionDate(NSLdvDrug* pDrug) ;

    // Risk frame
    //
    PatPathoIter    getRiskElement(string sNodeId, NSPatPathoArray **pPatho) ;

    // Social frame
    //
    PatPathoIter    getSocialElement(string sNodeId, NSPatPathoArray **pPatho) ;

    // Removed bool            connectObjectToConcern(LDVFRAME iFrame, string sCodeConcern, int iSeverity, string sNoeud, NSRootLink::NODELINKTYPES iRelation, bool bAutoCreate) ;
    bool            connectObjectToDrug(LDVFRAME iFrame, string sCodeDrug, int iSeverity, string sNoeud, NSRootLink::NODELINKTYPES iRelation, bool bAutoCreate) ;
    bool            showNewTree(const NSPatPathoArray *pPatPathoArray, string sDateDoc) ;
    void            showNewTreeToBbk(const NSPatPathoArray *pPatPathoArray, string sDateDoc) ;
    // Removed void            showNewDrug(const NSPatPathoArray *pPatPathoArray, PatPathoIter iter) ;
    bool            isALDDrug(string sNodeMedic) ;
    void            connectValuedNode(NSPatPathoArray *pPatPathoArray, PatPathoIter iter, string sGoalNode) ;
    void            showDrugProperty(NSLdvDrug *pDrug) ;
    bool            createXmlTree(NSLdvDrug *pDrug, Ctree* pTreeObject, string sTargetLocalisation) ;
    string          getPrescriptionDate(NSLdvDrug *pDrug) ;

    bool            getActiveBamCompatibleDrugs(NsSelectableDrugArray* paDrugsList) ;

    void            addGoalToConcern(NSLdvGoal *pAddedGoal) ;
    void            goalModified(NSLdvGoal *pModifiedGoal) ;
    void            reinitAllGoals() ;
    void            closeGoal(NSLdvGoal *pGoal, NVLdVTemps *pDateFin, NSPatPathoArray *pPatPathoMotif) ;
    void            closeGoal(NSLdvGoal *pGoal, NSPatPathoArray *pPatPathoMotif) ;  // Now
    void            goalAdded(NSLdvGoal* pGoal, GoalInfoArray *pWorstJalons) ;
    void            goalModifiedForConcern(NSLdvGoal* pGoal, NSConcern *pConcern) ;
    void            goalModifiedForDrug(NSLdvGoal* pGoal, NSLdvDrug *pDrug) ;

    // Removed void            invalidateViews(string sReason, string sReference = string("")) ;
    // Removed void            viewsReloadDrugToons() ;

    // Removed NSLdvView       *getLdvView(string sConcern = string("")) ;
    // Removed NSDrugView      *getDrugView(string sConcern) ;
    // Removed NSGoalView      *getGoalView(string sConcern) ;
    // Removed NSProcessView   *getProcessView(string sConcern) ;

    bool            openLdVObject(NSLdvObjet *pObj) ;
    bool            openLdVObjectsInArray(ArrayObjets *pObjects, NSLdvViewBase *pView) ;
    // implementation des m�thodes virtuelles de TDocument
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
} ;

ArrayGoals*
NSLdvDocumentBase::getGoals(LDVFRAME iFrame)
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

#endif // __NSLDVDOCBASE_H

