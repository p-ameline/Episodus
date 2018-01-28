//---------------------------------------------------------------------------
#ifndef __NSIMPORT_UPGRADE_H
#define __NSIMPORT_UPGRADE_H

// #include "nsmt_version\nsv_person.h"
// #include "nsmt_version\nsv_transcode.h"
// #include "nsmt_version\nsv_medic.h"
#include "nsbb\nsmanager.h"
#include "nssavoir\nsgraphe.h"

class NsvCheckTransferDialog;

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsImportDialog : public NSUtilDialog
{
	public:

    OWL::TCheckBox*	  pPatients ;

    NSVUpgrade*       pUpgrade ;

    NsImportDialog(TWindow* pere, NSContexte* pCtx) ;
    ~NsImportDialog() ;

    void SetupWindow() ;

    //void CmOk();
    //void CmCancel();

    void lancePatients() ;

	DECLARE_RESPONSE_TABLE(NsImportDialog) ;
};

//
// Objet "boite de dialogue" utilisé pour contrôler l'exécution
//
class NsiCheckTransferDialog : public NSUtilDialog
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

		NsiCheckTransferDialog(TWindow* pere, NSVUpgrade::TRANSCODETYPE type, NSContexte* pCtx) ;
		~NsiCheckTransferDialog() ;

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

	DECLARE_RESPONSE_TABLE(NsiCheckTransferDialog) ;
};

#endif

