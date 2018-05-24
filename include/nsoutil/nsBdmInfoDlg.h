// -----------------------------------------------------------------------------
// nsBdmInfoDlg.h
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Ligne de vie - Drugs database control dialogs
// -----------------------------------------------------------------------------
// PA  - april 2018
// ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Programme protégé par une licence Open Source GPL
// -----------------------------------------------------------------------------

#ifndef __NSBDMINFODLG_H
#define __NSBDMINFODLG_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\slider.h>
#include <owl\gauge.h>
#include <owl\listwind.h>

#include "partage\ns_vector.h"
#include "partage\NTArray.h"
#include "nsoutil/nsexport.h"
#include "nsbb\nsednum.h"
#include "partage\NTArray.h"
#include "nsbb\nslistwind.h"
#include "nssavoir\NsBdmDriver.h"
#include "nsbb\NTFunctor.h"

class NSGenericBdmInfoDlg ;
class NSBdmMultiInfoDlg ;

class NSBdmInfoListWindow : public NSSkinableListWindow
{
  public:

    // constructors / destructor
    NSBdmInfoListWindow() ;
    NSBdmInfoListWindow(NSContexte* pCtx, NSGenericBdmInfoDlg *parent, int iResId, TModule* module = (TModule*) 0) ;
    NSBdmInfoListWindow(NSContexte* pCtx, NSGenericBdmInfoDlg *parent, int id, int x, int y, int w, int h, TModule* module = (TModule*) 0) ;
    ~NSBdmInfoListWindow() ;

    void    SetupToolBar() ;
    void    SetupWindow() ;
    void    SetupColumns() ;
    void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    void    fillList(NsXmlEntriesArray* paXmlEntriesArray) ;

    NSGenericBdmInfoDlg* getDrugInfoDlg() { return _pParentDlg ; }

    bool    bSetupToolBar ;

  protected:

    int                  _iRes ;
    NSGenericBdmInfoDlg* _pParentDlg ;

    void InsertItems(const string sLabel, const string sValue, int iLine) ;

  DECLARE_RESPONSE_TABLE(NSBdmInfoListWindow) ;
} ;

class NSBdmInfoTitlesListBox : public OWL::TListBox
{
  public:

    // constructors / destructor
    NSBdmInfoTitlesListBox() ;
    NSBdmInfoTitlesListBox(NSBdmMultiInfoDlg *parent, int iResId, TModule* module = (TModule*) 0) ;
    NSBdmInfoTitlesListBox(NSBdmMultiInfoDlg *parent, int id, int x, int y, int w, int h, TModule* module = (TModule*) 0) ;
    ~NSBdmInfoTitlesListBox() ;

    void SetupToolBar() ;
    void SetupWindow() ;
    void SetupColumns() ;

    void fillList(NsXmlBlocksArray* paBlocks) ;

  protected:

    NSBdmMultiInfoDlg* _pParentDlg ;

    NsXmlBlocksArray*  _paBlocks ;

    void CmSelChanged() ;

  DECLARE_RESPONSE_TABLE(NSBdmInfoTitlesListBox) ;
} ;

class _NSOUTILCLASSE NSBdmInfoButton : public OWL::TButton, public NSRoot
{
  public:

    NSBdmInfoButton(NSContexte *pCtx, NSGenericBdmInfoDlg* parent, int resId) ;
    NSBdmInfoButton(NSContexte *pCtx, NSGenericBdmInfoDlg* parent, int resId,
                    const char far* text, int X, int Y, int W, int H,
                    bool isDefault = false, TModule* module = 0) ;
    ~NSBdmInfoButton() ;

    // child id notification handled at the child
    //
    void BNClicked() ;  // BN_CLICKED
    void SetupWindow() ;

    void Update(const string sUrl, const string sTitle) ;

    char far* GetClassName() ;

    string getUrl() const            { return _sUrl ; }
    void   setUrl(const string sU)   { _sUrl = sU ; }

    string getTitle() const          { return _sTitle ; }
    void   setTitle(const string sT) { _sTitle = sT ; }

  protected:

    string               _sUrl ;
    string               _sTitle ;
    NSGenericBdmInfoDlg* _pDrugInfoDlg ;

	DECLARE_RESPONSE_TABLE(NSBdmInfoButton) ;
};

typedef vector<NSBdmInfoButton*>                 NSBdmInfoButtonsVector ;
typedef NSBdmInfoButtonsVector::iterator         NSBdmInfoButtonIter ;
typedef NSBdmInfoButtonsVector::const_iterator   NSBdmInfoButtonConstIter ;
typedef NSBdmInfoButtonsVector::reverse_iterator NSBdmInfoButtonReverseIter ;
typedef NTArray<NSBdmInfoButton>                 NSBdmInfoButtonsArray ;

//
// Generic values + links dialog
//
class _NSOUTILCLASSE NSGenericBdmInfoDlg : public NSUtilDialog
{
  public:

    NSGenericBdmInfoDlg(TWindow* pView, NSContexte *pCtx, TResId resID) ;
    virtual ~NSGenericBdmInfoDlg() ;

    virtual void fillList() = 0 ;
    virtual void activatedInformation(int iIndex) = 0 ;

    bool isValidUrl(string sUrl) ;
    void openUrl(const string sUrl, const string sTitle) ;

  protected:

    NSBdmInfoListWindow*  _pInfoList ;
    NSBdmInfoButtonsArray _aButtons ;
    OWL::TButton*         _pOkBtn ;

    void CmOk() ;
    void SetupWindow() ;

    int  createButtons() ;
    void resizeForButtons(int iCurrentBtnTopPos) ;
    void relocateOkButton(int iCurrentBtnTopPos) ;

    virtual string            getTitle() = 0 ;
    virtual NsHtmlLinksArray* getLinks() = 0 ;

    int _iTopStart ;        // Top of first button
    int _iButtonHeight ;    // Button's height
    int _iLeftStart ;       // Left margin
    int _iButtonWidth ;     // Button's width
    int _iButtonSepar ;     // Interval between 2 buttons on a line
    int _iButtonInterval ;  // Interval between 2 lines
    int _iButtonsPerLine ;  // Number of buttons per line
    int _iTotalWidth ;      // Width of the buttons area
    int _iBorBtnHeight ;    // Height of the OK button
    int _iOkBtnWidth ;      // Width of the OK button

  DECLARE_RESPONSE_TABLE(NSGenericBdmInfoDlg) ;
} ;

//
// Looking for a drug in the BAM
//
class _NSOUTILCLASSE NSBdmInfoDlg : public NSGenericBdmInfoDlg
{
  public:

    // enum DRUGSEARCHBY { searchByUndefined = 0, searchBySubstance, searchByATC, searchByIndication } ;

    NSBdmInfoDlg(TWindow* pView, NSContexte *pCtx, NsXmlBlock* pBlock) ;
    ~NSBdmInfoDlg() ;

    void fillList() ;
    void openUrl(const string sUrl) ;

    void activatedInformation(int iIndex) ;

    NsXmlBlock* getInformationBlock() { return _pBlock ; }

  protected:

    NsXmlBlock* _pBlock ;

    void SetupWindow() ;

    string            getTitle() { return _pBlock->getTitle() ; }
    NsHtmlLinksArray* getLinks() { return _pBlock->getLinks() ; }

  DECLARE_RESPONSE_TABLE(NSBdmInfoDlg) ;
} ;

//
// Generic values + links dialog
//
class _NSOUTILCLASSE NSBdmMultiInfoDlg : public NSGenericBdmInfoDlg
{
  public:

    NSBdmMultiInfoDlg(TWindow* pView, NSContexte *pCtx, NsXmlBlocksArray* paBlocks, string sTitle) ;
    virtual ~NSBdmMultiInfoDlg() ;

    void CmSelChanged() ;

    void fillTitles() ;
    void fillList() ;
    void openUrl(const string sUrl) ;

    void activatedInformation(int iIndex) ;

    void   selectBlock(string sBlockId) ;
    string getSelectedBlockId() { return _sSelectedBlockId ; }

    string getTitle()           { return _sTitle ; }

  protected:

    NSBdmInfoTitlesListBox* _pTitlesList ;

    NsXmlBlocksArray*       _paBlocks ;
    string                  _sSelectedBlockId ;

    string                  _sTitle ;

    void SetupWindow() ;

    void updateButtons() ;
    void updateButtons(NsHtmlLinksArray* pLinks) ;
    void updateButtonsContents(NsHtmlLinksArray* pLinks) ;
    void removeButtons() ;

    NsHtmlLinksArray* getLinks() ;
    NsXmlBlock*       getSelectedBlock() ;

  DECLARE_RESPONSE_TABLE(NSBdmMultiInfoDlg) ;
} ;

#endif // __NSBDMINFODLG_H

