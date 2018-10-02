//---------------------------------------------------------------------------
#ifndef __NSV_UPGRADE_H
#define __NSV_UPGRADE_H

class NsvCheckTransferDialog ;
class NSMsgLoadProgressDlg ;
class NSRecupPatientData ;
class NSRecupCorrespData ;

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
  string                  sContribution ;

  string				          sObjectIDLocalTeam ;
  string                  sPathChemins ;
  NsvCheckTransferDialog* pCheckDialog ;
  NSMsgLoadProgressDlg*   _pProgressDlg ;

  FILE*                   _inFile ;
  unsigned char           _cChar ;
  int                     _iCharIdx ;

  NSVUpgrade(NSContexte* pCtx) ;
  ~NSVUpgrade() ;

  bool   CreatePatients() ;
  bool   ExportPatient(NSRecupPatientData* pPatInfo) ;

  bool   CreateCorresps() ;
  bool   ExportCorresp(NSRecupCorrespData* pCorrespInfo) ;

  bool   ReferenceLettres() ;
  bool   ReferenceLettresAndSubdirectories(string sPath) ;
  bool   ReferenceLettresInDirectory(string sPath) ;
  bool   ReferenceDirectoriesInDirectory(string sPath) ;
  bool   ExportLettreData(string sFileName, string sCreationDate) ;
  size_t countFilesInDirectory(char* szSearchString) ;
  string GetLastWriteTime(HANDLE hFile) ;

  string GetDataFromFile() ;

  bool   CreateChemins() ;

  bool   IsMobilePhone(string sText) ;
  bool   IsMail(string sText) ;
  bool   IsPhoneNumber(string sText) ;
};

//
// Objet "Boite de dialogue" utilis� pour s�lectionner l'utilitaire
//
class NsvConvertDialog : public NSUtilDialog
{
	public:

    OWL::TCheckBox*	pChemins ;
    OWL::TCheckBox*	pCorresp ;
    OWL::TCheckBox*	pPatients ;
    OWL::TCheckBox*	pLettres ;

    NSVUpgrade*     pUpgrade ;

    NsvConvertDialog(TWindow* pere, NSContexte* pCtx) ;
    ~NsvConvertDialog() ;

    void SetupWindow() ;

    //void CmOk();
    //void CmCancel();

    void lanceChemins() ;

    void lanceCorresp() ;
    void lancePatients() ;
    void lanceLettres() ;

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

//
// Objet "Boite de dialogue" utilis� pour montrer la progression du
// chargement des messages
//
class NSMsgLoadProgressDlg : public NSUtilDialog
{
  public:

    OWL::TGauge*  pProgressBar ;
    OWL::TStatic* pText ;
    OWL::TButton* pStopButton ;

    bool bKeepLoading ;

    NSMsgLoadProgressDlg(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
    ~NSMsgLoadProgressDlg() ;

    void SetupWindow() ;

    void resetGauge()           { pProgressBar->SetValue(0) ; }
    void fixePosition(int iPos) { pProgressBar->SetValue(iPos) ; }

    void setText(string sTxt)   { pText->SetCaption(sTxt.c_str()) ; }

    bool keepLoading() { return bKeepLoading ; }

    void stopPressed() ;

	DECLARE_RESPONSE_TABLE(NSMsgLoadProgressDlg) ;
} ;

#endif

