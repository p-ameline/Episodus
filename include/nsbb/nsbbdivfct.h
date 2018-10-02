#ifndef __NSBBDIVFCT_H#define __NSBBDIVFCT_H

class NSDialog ;
class NSDlgFonction ;
class BBItem ;
class BBFilsItem ;
class NSPatPathoArray ;
class NSDocumentHisto ;
class NSTreeWindow ;

#include <cstring.h>
#include <bwcc.h>
// #include "nsbb\nsbbitem.h"
// #include "nsbb\nsbouton.h"

#include "nsldv\nsLdv_Vars.h"
#include "nsbb\nsbb_dlg.h"

class _NSBBCLASSE HistoryConnector
{
	public :

    enum CONNECTIONTYPE { isUndefined = 0, isConcern, isSynthesis } ;

    // constructeurs
    HistoryConnector() ;
    HistoryConnector(const HistoryConnector& src) ;

    ~HistoryConnector() ;

    HistoryConnector& operator=(const HistoryConnector& src) ;
    int               operator==(const HistoryConnector& o) ;

    CONNECTIONTYPE getConnectionType()                   { return _iConnectionType ; }
    void           setConnectionType(CONNECTIONTYPE iCT) { _iConnectionType = iCT ; }

    string         getElementPath()              { return _sElementPath ; }
    void           setElementPath(string sP)     { _sElementPath = sP ; }

    string         getItemLabel()                { return _sItemLabel ; }
    void           setItemLabel(string sI)       { _sItemLabel = sI ; }

    string         getDestinationPath()          { return _sDestinationPath ; }
    void           setDestinationPath(string sD) { _sDestinationPath = sD ; }

    string         getHistoryPath()              { return _sHistoryPath ; }
    void           setHistoryPath(string sD)     { _sHistoryPath = sD ; }

    bool           isUnique()                    { return _bIsUnique ; }
    void           setUnique(bool bIU)           { _bIsUnique = bIU ; }

    string         getPath() ;

    void             addSubPath(string sPath) ;
    vector<string*> *getSubPathArray() { return &_aSubPaths ; }
    void             emptySubPath() ;
    bool             isInSubPath(string sSubPath) ;

  protected :

    CONNECTIONTYPE   _iConnectionType ;
    bool             _bIsUnique ;

    string           _sElementPath ;
    string           _sItemLabel ;

    string           _sDestinationPath ;

    string           _sHistoryPath ;

    vector<string*>  _aSubPaths ;
};

typedef vector<HistoryConnector*>        HistoryConnectorVector ;
typedef HistoryConnectorVector::iterator HistoryConnectorIter ;
typedef NTArray<HistoryConnector>        HistoryConnectorArray ;

HistoryConnector* _NSBBCLASSE GetHistoryConnectorForPath(NSDlgFonction *pNSFct, HistoryConnectorArray *pHCa) ;

void        ActiveChampEdit(BBFilsItem* fils, string sVal) ;
BBFilsItem* RetournerValeurEdit(BBItem* pItem, string sEtiquette, string* pValeurEdit, bool bFromRoot, bool bSemanticSearch) ;
BBFilsItem* RetournerValeurEdit(BBItem* pItem, string sEtiquette, string* pValeurEdit, bool bSemanticSearch) ;
void        RetournerDateMax(BBItem* pItem, string* pDateMax) ;

NSControle* RechercheFctApres(NSControleVector* pNSCtrl, NSDlgFonction *pNSFct, string sFctName) ;
NSControle* RechercheFctAvant(NSControleVector* pNSCtrl, NSDlgFonction *pNSFct, string sFctName) ;

BBFilsItem* _NSBBCLASSE TrouverFilsAyantBonneEtiquette(BBItem* pere, string sEtiquette) ;
string      _NSBBCLASSE FindValueInRootChapter(BBItem* pRacine, string sChapter, string sValueString) ;
BBFilsItem* _NSBBCLASSE FindElementInPathFromRootChapter(BBItem* pRacine, string sChapter) ;
string      _NSBBCLASSE FindValueInPathFromRootChapter(BBItem* pRacine, string sChapter, string sValueString) ;

void FixerDateDuJour(NSDlgFonction *pNSFct) ;
void AdaptLastingTime(NSDlgFonction *pNSFct) ;
void FixLastingTime(NSDlgFonction *pNSFct) ;
void AdaptEnding(NSDlgFonction *pNSFct, bool bForceEnding = false) ;
void FixNoEnding(NSDlgFonction *pNSFct) ;
void CancelEnding(NSDlgFonction *pNSFct) ;
void FixCaption(NSDlgFonction *pNSFct) ;
void CreatePreoccup(NSDlgFonction *pNSFct) ;
void SynchroNumValues(NSDlgFonction *pNSFct) ;
void RemplirDateMax(NSDlgFonction *pNSFct) ;
void CalcSC(NSDlgFonction *pNSFct) ;
void GetIPP(NSDlgFonction *pNSFct) ;
void GetLocalIPP(NSDlgFonction *pNSFct, bool bManual) ;
string GetNewIPP(NSContexte* pContexte) ;
void CalcIMC(NSDlgFonction *pNSFct) ;
void _NSBBCLASSE CalcIPS(NSDlgFonction *pNSFct) ;
void _NSBBCLASSE CalcIPSLeft(NSDlgFonction *pNSFct) ;
void _NSBBCLASSE CalcIPSRight(NSDlgFonction *pNSFct) ;
void CalcIPSSide(NSDlgFonction *pNSFct, string sAnkle, string sInferiorMember, string sBodySide) ;
string GetLeftArmSystolicArterialPressure(NSDlgFonction *pNSFct) ;
string GetRightArmSystolicArterialPressure(NSDlgFonction *pNSFct) ;
string GetArmSystolicArterialPressure(NSDlgFonction *pNSFct, string sArm, string sSide) ;
string GetMaxSystolicArterialPressure(NSDlgFonction *pNSFct) ;
void CalcWeigthGain(NSDlgFonction *pNSFct) ;
void CalcVCHAL(NSDlgFonction *pNSFct) ;
void CalcVDATA(NSDlgFonction *pNSFct) ;
void CalcFriedwaldGpl(NSDlgFonction *pNSFct) ;
void CalcMicroAlb24(NSDlgFonction *pNSFct) ;
void CalcConsoSel(NSDlgFonction *pNSFct) ;
void CalcCockroftMGPL(NSDlgFonction *pNSFct) ;
void CalcCockroftMMPL(NSDlgFonction *pNSFct) ;
void CalcMdrdMGPL(NSDlgFonction *pNSFct) ;
void CalcMdrdMMPL(NSDlgFonction *pNSFct) ;
void CalcBilirubineLibre(NSDlgFonction *pNSFct) ;
void EditAddress(NSDlgFonction *pNSFct) ;
void ModifyAddress(NSDlgFonction *pNSFct) ;
void EditAddressButton(NSDlgFonction *pNSFct) ;
void ChoosePatAddress(NSDlgFonction *pNSFct) ;
void ChooseCorAddress(NSDlgFonction *pNSFct) ;
void ChooseAddress(NSDlgFonction *pNSFct) ;
void EditCorresp(NSDlgFonction *pNSFct) ;
void ShowCorresp(NSDlgFonction *pNSFct) ;
void EditRefCorresp(NSDlgFonction *pNSFct) ;
void ShowRefCorresp(NSDlgFonction *pNSFct) ;
void PidsAdrSite(NSDlgFonction *pNSFct) ;
void PidsAdrVoie(NSDlgFonction *pNSFct) ;
void PidsAdrZip(NSDlgFonction *pNSFct) ;
void ChangeToLowCaps(NSDlgFonction *pNSFct) ;
void PidsZipFillCity(NSDlgFonction *pNSFct) ;
bool getCityFromZip(string sZipCode, NSPatPathoArray* pCityPpt, NSDialog* pMotherDialog, NSContexte* pContexte) ;
bool getCityFromZipTxt(string sZipCode, string &sCityName, TWindow* pMotherDialog, NSContexte* pContexte) ;
void SurfOnTheMap(NSDlgFonction *pNSFct) ;
void ShowQrCode(NSDlgFonction *pNSFct) ;
void Classifying(NSDlgFonction *pNSFct, bool bOneTopic = false) ;
void initFromCurrentHealthProblems(NSDlgFonction *pNSFct) ;
void initFromCurrentSocialProblems(NSDlgFonction *pNSFct) ;
void initFromCurrentRisks(NSDlgFonction *pNSFct) ;
void initFromCurrentFrameConcerns(NSDlgFonction *pNSFct, LDVFRAME iFrame) ;
void initFromCurrentConcerns(NSDlgFonction *pNSFct) ;
void GetPrescriptionCrossover(NSDlgFonction *pNSFct) ;
void SetPrescriptionCrossover(NSDlgFonction *pNSFct) ;

void SetGestationDDR(NSDlgFonction *pNSFct) ;
void SetGestationDatesDDR(NSDlgFonction *pNSFct) ;
void SetGestationDatesEcho(NSDlgFonction *pNSFct) ;
void SetGestationDates(NSDlgFonction *pNSFct) ;

void _NSBBCLASSE CheckSurgicalHistory(NSDlgFonction *pNSFct, string sCutConcept = string("QANTC")) ;
void _NSBBCLASSE CheckSurgicalHistoryChapter(NSDlgFonction *pNSFct, string sCutConcept = string("QANTC")) ;
void _NSBBCLASSE CheckFamilyHistory(NSDlgFonction *pNSFct, string sCutConcept = string("QANTF")) ;
void _NSBBCLASSE CheckFamilyHistoryChapter(NSDlgFonction *pNSFct, string sCutConcept = string("QANTF")) ;
void _NSBBCLASSE RecordInSynthesis(NSDlgFonction *pNSFct, string sContextPath, string sCutConcept = string("")) ;
void _NSBBCLASSE RecordInSynthesis2(NSDlgFonction *pNSFct, HistoryConnector* pHC) ;
void             SaveSynthesis(NSTreeWindow* pTreeWin, NSDocumentHisto* pSynthDoc) ;
string           getModelNodeFromDoc(NSPatPathoInfo* pCopyNode, string sModelDocId, NSContexte *pCtx, VecteurString *pNodes = (VecteurString*) 0) ;

void _NSBBCLASSE CheckChronicConcern(NSDlgFonction *pNSFct) ;
void _NSBBCLASSE CheckNotChronicConcern(NSDlgFonction *pNSFct) ;
void _NSBBCLASSE RecordChronicConcernInHealthIndex(NSDlgFonction *pNSFct) ;

void InitTitleOfMaterial(NSDlgFonction *pNSFct) ;
void SelectMaterial(NSDlgFonction *pNSFct) ;
void InitTitleOfPerson(NSDlgFonction *pNSFct) ;
void SelectPerson(NSDlgFonction *pNSFct) ;

static bool FAR PASCAL SetStaticCaption(HWND hWnd, LPARAM lParam) ;

#endif

