//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#ifndef __NSLDVDRUG_H
# define __NSLDVDRUG_H

# if defined(_DANSDLL)
#  define _CRDOCCLASSE __import
# endif

class NSLdvDocumentBase ;
class NSLdvGoal ;
class NSConcern ;
class ArrayGoals ;
class NSLdvPubli ;
class NSFrameInformation ;
class NewConcernInfo ;

#ifndef NSDocumentHisto
  class NSDocumentHisto ;
#endif

#ifndef __NSBDMDRIVER_H
  class NsSelectableDrug ;
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

#include "nsldv/nsldvexport.h"

class NSLdvDrugPhase ;

class _NSLDVCLASSE NSLdvDrugTake : public NSRoot
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

    double getDoseAsDouble() ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

class _NSLDVCLASSE NSLdvDrugCycle : public NSRoot
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

/**
 * A given mode of prescription from starting date to ending date
 */
class _NSLDVCLASSE NSLdvDrugPhase : public NSRoot
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

    double          getPhaseDose() const ;

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

/**
 * Global program (may have multiple phases) for a given drug
 */
class _NSLDVCLASSE NSLdvDrug : public NSRoot
{
  public:

    NSLdvDocumentBase*  _pDoc ;

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

    // GoalInfoArray*      _pWorstJalons ;  // Worst goals

    NSLdvDrug(NSContexte *pCtx, NSLdvDocumentBase* pDocum, LDVFRAME iFrame) ;
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

    NsSelectableDrug* getBamDrug()                     { return _pBamDrug ; }
    void              setBamDrug(NsSelectableDrug* pD) { _pBamDrug = pD ; }

    bool            isNarcotic() ;

    void            setTitlesFromLexique() ;

    void            initPhases(NSPatPathoArray* pTree, PatPathoIter iter) ;
    NSLdvDrugPhase* getPhaseFromNode(string sNode) ;
    NSLdvDrugPhase* getCurrentActivePhase() ;
    NSLdvDrugPhase* getLastActivePhase() ;
    bool            getCurrentActivePhasePpt(NSPatPathoArray *pPPT) ;

    // void            goalAdded(NSLdvGoal *pGoal) ;
    // void            goalModified(NSLdvGoal *pGoal) ;

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

    bool            initBamDrug() ;

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

    NsSelectableDrug* _pBamDrug ;

    static long lObjectCount ;
} ;

typedef vector<NSLdvDrug *>        ArrayDrugs ;
typedef ArrayDrugs::iterator       drugsIter ;
typedef ArrayDrugs::const_iterator drugsConstIter ;

class _NSLDVCLASSE ArrayLdvDrugs : public ArrayDrugs
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

/**
 * An array of pointers to NSLdvDrug objects not managed here (not deleted by destructor)
 */
class _NSLDVCLASSE ArrayCopyDrugs : public ArrayDrugs
{
  public:

    // Constructeurs - Destructeur
    ArrayCopyDrugs() : ArrayDrugs() {}
    ArrayCopyDrugs(const ArrayCopyDrugs& rv) ;
    ~ArrayCopyDrugs() ;

    void   vider() ;

    ArrayCopyDrugs& operator=(const ArrayCopyDrugs& src) ;
} ;

#endif // __NSLDVDRUG_H

