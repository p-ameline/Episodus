#ifndef __SCHILLERECG_DLG_H#define __SCHILLERECG_DLG_H

class NSUtilLexique ;

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\button.h>

#include "nsmi_schillerEcg\schillerEcg.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsSchillerEcgDialog : public NSUtilDialog
{
  public:

    OWL::TButton* _pCreatePatientButton ;
    OWL::TButton* _pNewRestingEcgButton ;
    OWL::TButton* _pNewStressEcgButton ;
    OWL::TButton* _pGetPatientResultsButton ;
    OWL::TButton* _pParamsButton ;

    NsSchillerEcgDialog(TWindow* pere, NSContexte* pCtx, NsSchillerEcgInformation* pEcgInfo) ;
    ~NsSchillerEcgDialog() ;

    void SetupWindow() ;

    void exportPatient() ;
    void newRestingEcg() ;
    void newStressEcg() ;
    void getResult() ;

  protected:

    NsSchillerEcgInformation* _pEcgInfo ;

  DECLARE_RESPONSE_TABLE(NsSchillerEcgDialog) ;
} ;

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NSSemaParamsDlg : public NSUtilDialog
{
  public:

    OWL::TStatic* _pExeNameTxt ;
    OWL::TStatic* _pLoginTxt ;
    OWL::TStatic* _pPasswordTxt ;
    OWL::TButton* _pLookForExe ;
    OWL::TEdit*   _pExeName ;
    OWL::TEdit*   _pLogin ;
    OWL::TEdit*   _pPassword ;
    OWL::TButton* _pTestIds ;

    NSSemaParamsDlg(TWindow* pere, NSContexte* pCtx, NsSchillerEcgInformation* pEcgInfo) ;
    ~NSSemaParamsDlg() ;

    void SetupWindow() ;

    void CmOk() ;
    void CmCancel() ;

    void   lookForExe() ;
    string lookForExeManually() ;
    string lookForExeAutomatically(string sFileName) ;

    void testIdentifiers() ;

    void findFileProgress(int i, string& tem) ;

  protected:

    NsSchillerEcgInformation* _pEcgInfo ;

    string _sSelectedDir ;

    string getExeName() ;
    string getLogin() ;
    string getPass() ;

  DECLARE_RESPONSE_TABLE(NSSemaParamsDlg) ;
} ;

//
// Objet "Boite de dialogue" utilisé pour fixer un identifiant
//
class NSSchillerGetIdDlg : public NSUtilDialog
{
  public:

    OWL::TStatic* _pIdTxt ;
    OWL::TEdit*   _pIdEdit ;

    NSSchillerGetIdDlg(TWindow* pere, NSContexte* pCtx, string* pId) ;
    ~NSSchillerGetIdDlg() ;

    void SetupWindow() ;

    void CmOk() ;
    void CmCancel() ;

  protected:

    string *_psId ;

  DECLARE_RESPONSE_TABLE(NSSchillerGetIdDlg) ;
} ;

#endif  // __SCHILLERECG_DLG_H
