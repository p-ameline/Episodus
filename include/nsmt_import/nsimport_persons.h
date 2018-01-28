//---------------------------------------------------------------------------
#ifndef __NSIMPORT_PERSONS_H
#define __NSIMPORT_PERSONS_H

class nsarcParseur ;
class NSHealthTeamManager ;

#include "nsmt_import\nsimport_transcode.h"
#include "nsbb\nsmanager.h"
#include "nsbb\nsattval.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nscaptur.h"

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

class NsImportPersonsDialog ;

class NSImport : public NSRoot
{
 public:

  enum TRANSCODETYPE { patient = 0, utilisateur, correspondant, document, healthteam } ;

  NSLocalChapterArray     _aParams ;
  ArrayEpisodusModelData  _aCaptureModels ;
  NSCaptureFormatArray    _aCaptureFormat ;

  NSPersonGraphManager*   _pGraphPerson ;
  NSAddressGraphManager*  _pGraphAdr ;
  string                  _sContribution ;

  string                  _sObjectIDLocalTeam ;

  NSArcNodeArray*         _pLibArc ;
  string                  _sImportFile ;
  NsImportPersonsDialog*  _pCheckDialog ;

  NSImport(NSContexte* pCtx) ;
  ~NSImport() ;

  bool    readParamsFromFile() ;
  bool    readCaptureModels() ;

  void    CreateAllPatients() ;
  void    CreateAllCorresps() ;
  void    CreateAllLetters() ;

  bool Referencer(NSPatInfo* pCurrentPerson, string typeDoc, string sPersonId,
                  NSDocumentInfo* pDocInfo, NSPatPathoArray* pPatPathoArray,
                  string sFileName, bool bDocVisible, string sCodeDoc,
                  int typeLink) ;

  string  ValueType(TRANSCODETYPE type) ;
  string  DonneNouveauType(char* type) ;
  bool    EnregTranscode(TRANSCODETYPE type, string sOldCode, string sNewCode) ;
  bool    ExisteTranscode(TRANSCODETYPE type, string sOldCode, string& sNewCode) ;
  void	  TraceGlobals(string sNss) ;
  void	  LogTransfert(string trace) ;

  string         getParam(string sChapter, string sCode) ;
  EpisodusModel* getCaptureModel(string sModelName) ;
  bool           initLocalTeam() ;
};

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsvImportDialog : public NSUtilDialog
{
	public:

    OWL::TCheckBox*	_pImportPatients ;
    OWL::TCheckBox*	_pImportCorresps ;
    OWL::TCheckBox*	_pImportLetters ;

    NSImport*       _pImport ;

    NsvImportDialog(TWindow* pere, NSContexte* pCtx, NSImport* pImport) ;
    ~NsvImportDialog() ;

    void SetupWindow() ;

    //void CmOk();
    //void CmCancel();

    void lancePatients() ;
    void lanceCorresps() ;
    void lanceLetters() ;

	DECLARE_RESPONSE_TABLE(NsvImportDialog) ;
};

//
//
class NSImportPerson : public NSRoot
{
 public:

  EpisodusModel* _pCaptureModel ;
  NSImport*      _pImport ;

  nsarcParseur*  _pParseurForAdminArchetype ;

  NsImportPersonsDialog* pCheckDialog ;

  NSImportPerson(NSContexte* pCtx, NSImport* pImport) ;
  ~NSImportPerson() ;

  bool    CreateAllPatients() ;
  bool    CreatePatient(string sLine) ;
  bool    CreateGraphPatient(NSCaptureArray* pCapturedInfo, string sIPP) ;
  bool    createGraphRoot() ;

  bool    createRootDocuments() ;
  bool    createFolderLibrary() ;
  bool    createRootIndex() ;
  bool    createRootSynthesis() ;
  bool    createIdsRepository(string sIPP) ;

  bool    createHealthTeam(NSCaptureArray* pCapturedInfo) ;
  bool    addCorrespsToHealthTeam(NSHealthTeamManager* pHTManager, NSCaptureArray* pCapturedInfo) ;
  bool    addCapturedToHealthTeam(NSHealthTeamManager* pHTManager, NSCaptureArray* pCapturedInfos) ;
  bool    addThisCorrespToHealthTeam(NSHealthTeamManager* pHTManager, string sCorrespID) ;
  bool    createGraphCorresp(NSCaptureArray* pCapturedInfos, string* psNewId) ;

  bool    CreateContribution() ;

  bool    fillArchetypedPpt(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo, Carchetype* pArchetype) ;
  bool    fillSonNodes(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo, Citem* pFather, string sFatherPath) ;

  void          openDemographicArchetype() ;
  nsarcParseur* getCorrespArchetype() ;

  string  removeHealthTeamRoot(string* pCapturedPath) ;
};

class NSImportCorresp : public NSRoot
{
 public:

  EpisodusModel* _pCaptureModel ;
  NSImport*      _pImport ;

  nsarcParseur*  _pParseurForAdminArchetype ;

  NsImportPersonsDialog* _pCheckDialog ;

  NSImportCorresp(NSContexte* pCtx, NSImport* pImport) ;
  ~NSImportCorresp() ;

  bool    CreateAllCorresps() ;
  bool    CreateCorresp(string sLine) ;
  bool    CreateGraphCorresp(NSCaptureArray* pCapturedInfo, string sIPP) ;
  bool    createGraphRoot() ;

  void          openDemographicArchetype() ;
  nsarcParseur* getCorrespArchetype() ;
} ;

class NSImportLetter : public NSRoot
{
 public:

  NSImport*  _pImport ;
  string     _sCurrentPatId ;
  NSPatInfo* _pCurrentPerson ;

  NsImportPersonsDialog* _pCheckDialog ;

  NSImportLetter(NSContexte* pCtx, NSImport* pImport) ;
  ~NSImportLetter() ;

  bool    CreateAllLetters() ;
  bool    CreateLetter(string sLine) ;
  bool    OpenPat(string sID) ;
} ;

//
// Objet "boite de dialogue" utilisé pour contrôler l'exécution
//
class NsImportPersonsDialog : public NSUtilDialog
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

		NsImportPersonsDialog(TWindow* pere, NSContexte* pCtx) ;
		~NsImportPersonsDialog() ;

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

	DECLARE_RESPONSE_TABLE(NsImportPersonsDialog) ;
};

#endif    // __NSIMPORT_PERSONS_H

