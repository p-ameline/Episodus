#ifndef __VIDALCD_DLG_H#define __VIDALCD_DLG_H

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>

class NSPathoArray ;

#include "nssavoir\nspatho.h"
#include "nsmt_vidalCD\VidalMaster.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsVidalCdDialog : public NSUtilDialog
{
  public:

    VidalMaster*      _pMaster ;

    OWL::TCheckBox*	  _pDCMasterInit ;
    OWL::TCheckBox*	  _pAI ;
    OWL::TCheckBox*	  _pExcipients ;
    OWL::TCheckBox*	  _pDrugs ;

    NsVidalCdDialog(TWindow* pere, NSContexte* pCtx, VidalMaster* pMaster) ;
    ~NsVidalCdDialog() ;

    void SetupWindow() ;

    void CmOk() ;
    //void CmCancel();

    void initComponents() ;
    void startAI() ;
    void startExcipients() ;
    void startDrugs() ;

  DECLARE_RESPONSE_TABLE(NsVidalCdDialog) ;
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

    NsFindDrugDialog(TWindow* parent, NSContexte* pCtx, string *pLabel, NSPathoArray* pRecords) ;
    ~NsFindDrugDialog() ;

    void SetupWindow() ;

    void CmOk() ;
    void CmCancel() ;

    string getSelectedDrug() ;

  DECLARE_RESPONSE_TABLE(NsFindDrugDialog) ;
} ;

#endif  // __VIDALCD_DLG_H
