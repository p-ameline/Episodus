#ifndef __ARCHEDIT_DLG_H#define __ARCHEDIT_DLG_H

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\radiobut.h>

class NSArchEditDocument ;
class NSUtilLexique ;

#include "nsepisod\flechiesDB.h"
#include "nsbb\nsutidlg.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsArchEditDialog : public NSUtilDialog
{
  public:

    OWL::TRadioButton* _pEditFile ;
    OWL::TRadioButton* _pNewFile ;

    NsArchEditDialog(TWindow* pere, NSContexte* pCtx) ;
    ~NsArchEditDialog() ;

    void SetupWindow() ;

    void CmEditFile() ;
    void CmNewFile() ;

    string getFileToOpen() { return _sFileToOpen ; }

  protected:

    string _sFileToOpen ;

  DECLARE_RESPONSE_TABLE(NsArchEditDialog) ;
} ;

//
// Dialog for archetype params
//
class NsArchParamsDialog : public NSUtilDialog
{
  public:

    OWL::TRadioButton* _pEditFile ;
    OWL::TRadioButton* _pNewFile ;

    NsArchParamsDialog(TWindow* pere, NSContexte* pCtx, NSArchEditDocument* pDoc) ;
    ~NsArchParamsDialog() ;

    void SetupWindow() ;

    void CmOk() ;

    string getArchetypeId()             { return _sArchetypeId ; }
    void   setArchetypeId(string sId)   { _sArchetypeId = sId ; }

    string getReferentialId()           { return _sReferentialId ; }
    void   setReferentialId(string sId) { _sReferentialId = sId ; }

    string getConcern()                 { return _sConcernConcept ; }
    void   setConcern(string sCo)       { _sConcernConcept = sCo ; }

    string getDimension()               { return _sRoot ; }
    void   setDimension(string sDi)     { _sRoot = sDi ; }

    bool   getAutoCreate()              { return _bAutoCreate ; }
    void   setAutoCreate(bool bAC)      { _bAutoCreate = bAC ; }

  protected:

    void updateArchetypeIdEdit() ;
    void updateReferentialIdEdit() ;
    void updateConcernEdit() ;
    void updateDimensionsRadioButtons() ;
    void updateAutoCreateRadioButtons() ;

    void updateArchetypeIdFromEdit() ;
    void updateReferentialIdFromEdit() ;
    void updateConcernFromEdit() ;
    void updateDimensionsFromRadioButtons() ;
    void updateAutoCreateFromRadioButtons() ;

    string _sArchetypeId ;
    string _sReferentialId ;
    string _sConcernConcept ;
    string _sRoot ;
    bool   _bAutoCreate ;

    OWL::TGroupBox*    _pIdGroup ;
    OWL::TEdit*        _pIdEdit ;
    OWL::TStatic*      _pIdTxt ;
    OWL::TGroupBox*    _pReferentialGroup ;
    OWL::TEdit*        _pReferentialEdit ;
    OWL::TStatic*      _pReferentialTxt ;
    OWL::TGroupBox*    _pCreateConcernGroup ;
    NSUtilLexique*     _pConceptEdit ;
    OWL::TStatic*      _pConceptTxt ;
    OWL::TGroupBox*    _pUniverseGroup ;
    OWL::TRadioButton* _pHealthRelated ;
    OWL::TRadioButton* _pSocialRelated ;
    OWL::TRadioButton* _pRiskRelated ;
    OWL::TGroupBox*    _pAutocreateGroup ;
    OWL::TRadioButton* _pAutoCreateYes ;
    OWL::TRadioButton* _pAutoCreateNo ;

    NSArchEditDocument* _pDoc ;

  DECLARE_RESPONSE_TABLE(NsArchParamsDialog) ;
} ;

//
// Dialog box for setting caption and rect
//
class NsCaptionAndRectEditDialog : public NSUtilDialog
{
  public:

    OWL::TGroupBox*    _pCaptionGroup ;
    OWL::TStatic*      _pCaption_text ;
    OWL::TEdit*        _pCaption_edit ;

    OWL::TGroupBox*    _pPosAndSizeGroup ;
    OWL::TStatic*      _pX_text ;
    OWL::TEdit*        _pX_edit ;
    OWL::TStatic*      _pY_text ;
    OWL::TEdit*        _pY_edit ;
    OWL::TStatic*      _pW_text ;
    OWL::TEdit*        _pW_edit ;
    OWL::TStatic*      _pH_text ;
    OWL::TEdit*        _pH_edit ;

    NsCaptionAndRectEditDialog(TWindow* pere, NSContexte* pCtx, TResId resID, NSArchEditDocument* pDoc) ;
    ~NsCaptionAndRectEditDialog() ;

    void SetupWindow() ;
    void CmOk() ;

    int    getX() { return _iX ; }
    int    getY() { return _iY ; }
    int    getW() { return _iW ; }
    int    getH() { return _iH ; }

    void   setX(int iX) { _iX = iX ; }
    void   setY(int iY) { _iY = iY ; }
    void   setW(int iW) { _iW = iW ; }
    void   setH(int iH) { _iH = iH ; }

    NS_CLASSLIB::TRect getRect() ;
    void   setRect(NS_CLASSLIB::TRect tRect) ;

    string getCaption()           { return _sCaption ; }
    void   setCaption(string sCp) { _sCaption = sCp ; }

  protected:

    void     updateSizeEdits() ;
    void     updateCaptionFromEdit() ;
    void     updateSizeFromEdits() ;

    string   getText(OWL::TEdit* pEdit) ;
    int      getIntFromText(OWL::TEdit* pEdit) ;

    int      _iX ;
    int      _iY ;
    int      _iW ;
    int      _iH ;

    string   _sCaption ;

    NSArchEditDocument* _pDoc ;

  DECLARE_RESPONSE_TABLE(NsCaptionAndRectEditDialog) ;
} ;

//
// Generic dialog box for Control properties setting
//
class NsControlEditDialog : public NsCaptionAndRectEditDialog
{
  public:

    enum CTRLTYPE { isUndefined, isEdit, isBtn, isRadioBtn, isCheckBox,
                    isTreeWindow, isListView, isStatic, isGroupBox } ;

    OWL::TGroupBox*    _pCtrlTypeGroup ;

    OWL::TGroupBox*    _pProperties ;
    OWL::TButton*      _pExtensions ;

    NsControlEditDialog(TWindow* pere, NSContexte* pCtx, TResId resID, NSArchEditDocument* pDoc) ;
    ~NsControlEditDialog() ;

    void SetupWindow() ;

    void CmOk() ;

    string getClass()           { return _sClass ; }
    void   setClass(string sCl) { _sClass = sCl ; }

    uint32 getStyle()           { return _iStyle ; }
    void   setStyle(uint32 sSt) { _iStyle = sSt ; }

  protected:

    // Functions that must be provided by derived classes
    //
    virtual void updateControTypeRadioButtons() = 0 ;
    virtual void updatePropertyCheckBoxes() = 0;
    virtual void updateClassFromRadioButtons() = 0 ;
    virtual void updateStyleFromCheckBoxes() = 0 ;

    CTRLTYPE getControlType() ;
    void     setClassFromType(CTRLTYPE iType) ;

    string   _sClass ;
    uint32   _iStyle ;

  DECLARE_RESPONSE_TABLE(NsControlEditDialog) ;
} ;

//
// Dialog box for Dynamic Control properties setting
//
class NsDynamicControlEditDialog : public NsControlEditDialog
{
  public:

    OWL::TRadioButton* _pButton ;
    OWL::TRadioButton* _pCheckBox ;
    OWL::TRadioButton* _pRadioButton ;
    OWL::TRadioButton* _pEdit ;
    OWL::TRadioButton* _pTree ;

    OWL::TCheckBox*    _pVisible ;
    OWL::TCheckBox*    _pDisabled ;
    OWL::TCheckBox*    _pGroup ;
    OWL::TCheckBox*    _pTabStop ;

    NsDynamicControlEditDialog(TWindow* pere, NSContexte* pCtx, NSArchEditDocument* pDoc) ;
    ~NsDynamicControlEditDialog() ;

    void SetupWindow() ;

  protected:

    void     updateControTypeRadioButtons() ;
    void     updatePropertyCheckBoxes() ;

    void     updateClassFromRadioButtons() ;
    void     updateStyleFromCheckBoxes() ;

  DECLARE_RESPONSE_TABLE(NsDynamicControlEditDialog) ;
} ;

//
// Dialog box for Static Control properties setting
//
class NsStaticControlEditDialog : public NsControlEditDialog
{
  public:

    OWL::TRadioButton* _pTypeStatic ;
    OWL::TRadioButton* _pTypeGroup ;

    OWL::TCheckBox*    _pVisible ;
    OWL::TCheckBox*    _pDisabled ;

    NsStaticControlEditDialog(TWindow* pere, NSContexte* pCtx, NSArchEditDocument* pDoc) ;
    ~NsStaticControlEditDialog() ;

    void SetupWindow() ;

  protected:

    void     updateControTypeRadioButtons() ;
    void     updatePropertyCheckBoxes() ;

    void     updateClassFromRadioButtons() ;
    void     updateStyleFromCheckBoxes() ;

  DECLARE_RESPONSE_TABLE(NsStaticControlEditDialog) ;
} ;

//
// Dialog box for Static Control properties setting
//
class NsWindowEditDialog : public NsCaptionAndRectEditDialog
{
  public:

    NsWindowEditDialog(TWindow* pere, NSContexte* pCtx, NSArchEditDocument* pDoc) ;
    ~NsWindowEditDialog() ;

    void SetupWindow() ;

  protected:

  DECLARE_RESPONSE_TABLE(NsWindowEditDialog) ;
} ;

//
// Dialog box for Static Control properties setting
//
class NsSelectEditNodeDialog : public NSUtilDialog
{
  public:

    OWL::TGroupBox*    _pGroupNum ;
    OWL::TRadioButton* _pEditNum ;
    OWL::TStatic*      _pUnit ;
    NSUtilLexique*     _pConceptEdit ;
    OWL::TGroupBox*    _pGroupHour ;
    OWL::TRadioButton* _pEditDateHour ;
    OWL::TRadioButton* _pEditDate ;
    OWL::TRadioButton* _pEditHour ;
    OWL::TGroupBox*    _pGroupAlpha ;
    OWL::TRadioButton* _pEditAlpha ;
    OWL::TRadioButton* _pEditAlphaLex ;
    OWL::TRadioButton* _pEditAlphaLexOnly ;

    NsSelectEditNodeDialog(TWindow* pere, NSContexte* pCtx) ;
    ~NsSelectEditNodeDialog() ;

    void SetupWindow() ;
    void CmOk() ;

    void   setUnit(string sUnit)   { _sUnit = sUnit ; }
    string getUnit()               { return _sUnit ; }

    void   setLabel(string sLabel) { _sLabel = sLabel ; }
    string getLabel()              { return _sLabel ; }

  protected:

    string _sUnit ;
    string _sLabel ;

    void initControlsFromUnit() ;
    void initUnitFromControls() ;

    void resetControls() ;

    void CmNumChecked() ;
    void CmDateHourChecked() ;
    void CmDateChecked() ;
    void CmRadioChecked() ;
    void CmAlphaChecked() ;
    void CmAlphaLexChecked() ;
    void CmAlphaLexOnlyChecked() ;

  DECLARE_RESPONSE_TABLE(NsSelectEditNodeDialog) ;
} ;

#endif  // __ARCHEDIT_DLG_H

