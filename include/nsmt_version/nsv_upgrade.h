//---------------------------------------------------------------------------
#ifndef __NSV_UPGRADE_H
#define __NSV_UPGRADE_H

class NSMoralPersonManager ;
class NsvCheckTransferDialog ;
class NSCompt ;

#include "nsmt_version\nsv_person.h"
#include "nsmt_version\nsv_transcode.h"
#include "nsmt_version\nsv_medic.h"
#include "nsbb\nsmanager.h"
#include "nssavoir\nsgraphe.h"
// #include "nautilus\nsanxary.h"

class NSCvtVarTag
{
	public :

		string index ;
    string codeDoc ;

    NSCvtVarTag(string ind, string code) { index = ind ; codeDoc = code ; }
    ~NSCvtVarTag() {}
};

typedef NTArray<NSCvtVarTag> NSCvtVarTagArray;
typedef std::vector<NSCvtVarTag*>::iterator CvtVarTagIter;

class NSVUpgrade : public NSRoot
{
 public:
 	enum TRANSCODETYPE { patient = 0, utilisateur, correspondant, adresse, materiel, protocole, chemin, healthteam, comptaCompt, comptaFact, comptaFSE1610, comptaFSECCAM, comptaTiersPay, comptaDepenses, comptaCptsLibs, medicament, templates };

  NSPersonGraphManager*   pGraphPerson ;
  NSAddressGraphManager*  pGraphAdr ;
  string                  sContribution ;
  NSVChemiseArray*        pChemArray ;
  NSDocumentArray*        pDocumArray ;
  NSDocumentArray*        pCompoArray ;

  string				          sObjectIDLocalTeam ;
/*
  NSPatPathoArray*        pEquiMed;
  NSPatPathoArray*        pHealthTeam;
  string                  sOIDSEquiMed;
*/
  NSArcNodeArray*         pLibArc ;
  string                  sPathChemins ;
  NsvCheckTransferDialog* pCheckDialog ;

  NSVPatPaDatArray*       pDonneesArray ;
  NSVPatPaLocArray*       pLocalisArray ;
  NSArcNodeArray*         pArcNodeArray ;

  // Array des supports
  NSSupportsArray         aSupportsArray ;

  NSVUpgrade(NSContexte* pCtx) ;
  ~NSVUpgrade() ;

  string  ValueType(TRANSCODETYPE type) ;
  bool    CreateCorresp() ;
  bool    CreateGraphCorresp(NSVersionCorrespondantInfo* pCorInfo) ;
  bool    CreateUsers() ;
  bool    CreateGraphUtil(NSVersionUtiliInfo* pUtilInfo, NSMoralPersonManager *pMPManager) ;
  void    CreateUserFiles(string sUserID) ;

  bool    CreatePatients() ;
  bool    CreateGraphPatient(NSVersionPatInfo* pPatInfo) ;
  bool    CreateChemises(string sNss, NSPatPathoArray* pPathoChem) ;
  bool    FindPreviousFolders(string sNss, bool bStopOnError = false) ;
  bool    CreateDocuments(string sNss, string sPersonId) ;
  bool    CreateLienChemise(NSDocumentInfo* pDocInfo) ;
  bool    CreatePresentation(NSDocumentInfo* pHtmlInfo) ;
  bool    CreateContribution() ;
  string  getCivility(NSVersionPatInfo* pPatInfo) ;
  bool    getOldPatientFromNss(NSVersionPatInfo *pOldPatient, string sNss) ;

  bool    ModifyPatients() ;
  bool    ModifyExistingPatient(NSVersionPatInfo* pPatInfo, string sPids) ;
  bool    OpenExistingPatientGraph(NSBasicAttributeArray *pAttrArray) ;
  bool    RepairPartlyCreatedPatient(string sNss) ;

  bool    ModifyCorresps() ;
  bool    setCorrespMailAddress(string sOldCode, string sNewMail) ;

  bool    CreateMateriels() ;
  bool    CreateProtocols() ;
  void    createNTiersProtocol(NSVProtocole*) ;
  bool    CreateTemplates() ;
  void    createNTiersTemplate(NSVTemplate*) ;
  bool    CreateChemins() ;
  bool    createIdsRepository(string sIPP) ;

  bool    MoveDocuments(string sOldNss, string sNewPid) ;
  bool    UpdateChemises(string sOldNss, string sNewPid) ;

  void    automaticFusion() ;
  void    fusionOuCreation(string sOldNSS) ;
  string  getPatientWithSameTraits(string sNom, string sPrenom, string sNaiss) ;
  bool    isPersonWithSameTraits(string sRole, string sNom, string sPrenom) ;

  void    CommitChanges(string sPid) ;

  bool	  RemplirPatients() ;
  bool    TransfertCompta() ;
  bool    TransfertComptaCompt() ;
  bool    ChercheNumCompt(string& sNumCompt, NSCompt *pCompt) ;
  bool    TransfertComptaFact() ;
  bool    TransfertComptaDepense() ;
  bool    TransfertComptaCptsLibs() ;
  bool    TransfertComptaFse1610() ;
  bool    TransfertComptaFseCCAM() ;
  bool    TransfertComptaTiersPay() ;
  bool    RedresseComptaTiersPay() ;
  bool    TransfertComptaDoublons() ;
  bool    getNewPatientFromNewCompt(string sNumCompt, string& sPatNss) ;
  bool    TransfertReste() ;
  bool    ChargeSupportsArray(NSContexte* pCtx) ;
  bool    ChercheSupportsInfo(string sCodeSupport, NSSupportsInfo* pInfo, NSContexte* pCtx) ;
  string  PathName(string sCodeChemin, NSContexte* pCtx) ;
//  void    CreateHealthTeam(string sPersonId);
//  void    AjouteMembreEquiMed(string sPersonId);
  string  DonneNouveauType(char* type) ;
  bool    Referencer(string typeDoc, string sPersonId, NSDocumentInfo* pDocInfo, NSPatPathoArray* pPatPathoArray,
                      bool bDocVisible = true, string sCodeDoc = "", int typeLink = NSRootLink::personDocument) ;
  bool    EnregTranscode(TRANSCODETYPE type, string sOldCode, string sNewCode) ;
  bool    ExisteTranscode(TRANSCODETYPE type, string sOldCode, string& sNewCode) ;
  bool    ExisteReverseTranscode(TRANSCODETYPE type, string &sOldCode, string sNewCode) ;
  bool    ChargePatPatho(NSDocumentInfo* pDocInfo, NSPatPathoArray* pPatPathoArray) ;
  bool    MUEPatPatho(NSPatPathoArray* pPatPathoArray) ;
  bool    ChargeLibArc() ;
  string  donneOldLinkString(NSRootLink::NODELINKTYPES iRelation) ;
  void    TousLesVrais(string sTreeID, NSRootLink::NODELINKTYPES iRelation, VecteurString *pNoeudParam, VecteurString *pNoeudResult, DBITBLNAME sensCle = "FLECHE") ;
  void	  TraceGlobals(string sNss) ;
  void	  LogTransfert(string trace) ;
  string  getNewNodeFromOldNode(string sOldNode, string sNewTree) ;
  bool    copyFile(string sPathName, string sExistingFileName, string sNewFileName, bool bVerbose = true) ;

  void    CreateCheckDialog(TRANSCODETYPE iType) ;
  void    DestroyCheckDialog() ;
};

//
// Objet "Boite de dialogue" utilis� pour s�lectionner l'utilitaire
//
class NsvConvertDialog : public NSUtilDialog
{
	public:

		OWL::TCheckBox*	  pArchetypes ;
    OWL::TCheckBox*	  pUsers ;
    OWL::TCheckBox*	  pCorresp ;
    OWL::TCheckBox*	  pPatients ;
    OWL::TCheckBox*	  pMateriels ;
    OWL::TCheckBox*	  pProtocols ;
    OWL::TCheckBox*	  pTemplates ;
    OWL::TCheckBox*   pChemins ;
    OWL::TCheckBox*   pCompta ;
    OWL::TCheckBox*   pFusion ;
    OWL::TCheckBox*   pBadsList ;
    OWL::TCheckBox*   pDoublon ;
    OWL::TCheckBox*   pComptaDoublon ;
    OWL::TCheckBox*   pReprise ;
    OWL::TCheckBox*   pCreateHealthTeam ;
    OWL::TCheckBox*   pImportDocuments ;
    OWL::TCheckBox*   pRepriseCompt ;
    OWL::TCheckBox*   pRepriseFact ;
    OWL::TCheckBox*   pReprise1610 ;
    OWL::TCheckBox*   pRepriseCCAM ;
    OWL::TCheckBox*   pRepriseTiersPay ;
    OWL::TCheckBox*   pRepriseMail ;

    NSVUpgrade*       pUpgrade ;

    NsvConvertDialog(TWindow* pere, NSContexte* pCtx) ;
    ~NsvConvertDialog() ;

    void SetupWindow() ;

    //void CmOk();
    //void CmCancel();

    void lanceArchetypes() ;
    void lanceUsers() ;
    void lanceCorresp() ;
    void lancePatients() ;
    void lanceMateriels() ;
    void lanceProtocols() ;
    void lanceTemplates() ;
    void lanceChemins() ;
    void lanceCompta() ;
    void lanceFusion() ;
    void lanceBadsList() ;
    void lanceDoublon() ;
    void lanceComptaDoublon() ;
    void lanceReprise() ;
    void lanceCreateHealthTeam() ;
    void lanceDocByDoc() ;
    void lanceRepriseCompt() ;
    void lanceRepriseFact() ;
    void lanceReprise16() ;
    void lanceRepriseCCAM() ;
    void lanceRepriseTiersPay() ;
    void lanceRepriseMail() ;

	DECLARE_RESPONSE_TABLE(NsvConvertDialog) ;
};

//
// Objet "boite de dialogue" utilis� pour contr�ler l'ex�cution
//
class NsvCheckTransferDialog : public NSUtilDialog
{
	public :

		OWL::TStatic*	pOldCode ;
		OWL::TStatic*	pNewCode ;
		OWL::TStatic*	pAck ;

		OWL::TStatic*	pCntNSBasicAttribute ;
		OWL::TStatic*	pCntNSBasicAttributeArray ;
		OWL::TStatic*	pCntNSPersonsAttributesArray ;
    OWL::TStatic*	pCntNSDataTree ;
    OWL::TStatic*	pCntNSDataTreeArray ;
    OWL::TStatic*	pCntNSDataGraph ;
    OWL::TStatic*	pCntNSObjectGraphManager ;
    OWL::TStatic*	pCntNSPersonGraphManager ;
    OWL::TStatic*	pCntNSPatPathoInfo ;
    OWL::TStatic*	pCntNSPatPathoArray ;
    OWL::TStatic*	pCntNSVectPatPathoArray ;

		int    count;
		int    num;
		int    maxNum;
    int	   maxGarbage;
		string sMinPat;
		string sMaxPat;
		bool   bStop;

		NSVUpgrade::TRANSCODETYPE 	typeCode;

		NsvCheckTransferDialog(TWindow* pere, NSVUpgrade::TRANSCODETYPE type, NSContexte* pCtx) ;
		~NsvCheckTransferDialog() ;

		void SetupWindow() ;
		void SetOldCode(string sOldCode);
		void SetNewCode(string sNewCode);
		void SetAck(string sAck);
		void ClearText();
		void CheckCount();
		void FlushMemory();
		void StopProcess();
		bool isStopped();
		void SetStopLimit();

		void updateInstanceCounters() ;

	DECLARE_RESPONSE_TABLE(NsvCheckTransferDialog) ;
};

#endif

