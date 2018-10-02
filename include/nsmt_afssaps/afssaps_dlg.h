#ifndef __AFSSAPS_DLG_H#define __AFSSAPS_DLG_H

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>

class NSPathoArray ;

#include "nsbb\nsutidlg.h"
#include "nsbb\nsednum.h"
#include "nssavoir\nspatho.h"
#include "nsmt_afssaps\afssapsMaster.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsAfssapsDialog : public NSUtilDialog
{
  public:

    AfssapsMaster*    _pMaster ;

    OWL::TCheckBox*	  _pProcessCis ;
    OWL::TCheckBox*	  _pProcessCisGener ;
    OWL::TCheckBox*	  _pProcessDci ;

    NsAfssapsDialog(TWindow* pere, NSContexte* pCtx, AfssapsMaster* pMaster) ;
    ~NsAfssapsDialog() ;

    void SetupWindow() ;

    void CmOk() ;
    //void CmCancel();

    void startCis() ;
    void startCisGener() ;
    void startDci() ;

  DECLARE_RESPONSE_TABLE(NsAfssapsDialog) ;
} ;

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsFindDrugDialog : public NSUtilDialog
{
  public:

    string         _sLabel ;
    NSPathoArray*  _pRecords ;

    OWL::TListBox* _pLibelles ;
    OWL::TStatic*	 _pAmmorce ;

    char           _szSelectedLabel[PATHO_LIBELLE_LEN + 1] ;
    bool           _bIsSelected ;

    string         _sOther ;
    bool           _bStopAfterThisOne ;

    NsFindDrugDialog(TWindow* parent, NSContexte* pCtx, string *pLabel, NSPathoArray* pRecords) ;
    ~NsFindDrugDialog() ;

    void SetupWindow() ;

    void CmOk() ;
    void CmCancel() ;
    void CmOther() ;
    void CmStop() ;
    void dblClick() ;

    bool mustStopAfterThisOne() { return _bStopAfterThisOne ; }

    string getSelectedDrug() ;

  DECLARE_RESPONSE_TABLE(NsFindDrugDialog) ;
} ;

//---------------------------------------------------------------
//fixer le type de document
//---------------------------------------------------------------
class NSTransTermeLexique : public TDialog, public NSRoot
{
	public :

		NSUtilLexique* _pNewDrug ;
		string* 		   _pTypeDocum ;
    OWL::TStatic*  _pOldDrug ;
    string         _sReferenceText ;

		NSTransTermeLexique(TWindow* pere, NSContexte* pCtx, string* pTypeDocument,                        string sRefText, TModule* module = 0) ;
		~NSTransTermeLexique() ;

		void CmOk() ;		void CmCancel() ;
		void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(NSTransTermeLexique) ;};

#endif  // __VIDALCD_DLG_H
