#if !defined(__NSARCEDITTREE_H)#define __NSARCEDITTREE_H

#include <owl\docview.h>

class NSEditNum ;

#include "nsbb\nstrewi.h"
#include "nsbb\nsutidlg.h"
#include "nsmt_archEdit\nsArcEditDoc.h"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
class NSArcEditTreeView : public TView, public NSTreeWindow
{
	public:

  	NSSmallBrother     *_pBigBoss ;
    NSPatPathoArray    *_pPatPathoArray ;
    // Pointeurs
    NSArchEditDocument *_pDoc ;			    // Pointeur sur document

    OWL::TControlBar   *_pToolBar ;		    // Barre d'outils    bool               _bSetupToolBar ;

    TMenuDescr         *_pMUEViewMenu ;
    HACCEL             _hAccelerator ;

    bool               _bIsModif ;

    NSArcEditTreeView(NSArchEditDocument& doc, TWindow* parent = 0) ;
	  ~NSArcEditTreeView() ;

    static LPCSTR     StaticName()  { return "NSArcEditTreeView" ; }
	  const char* 	    GetViewName() { return StaticName() ; }
    TWindow*	        GetWindow() ;

    // Fonction d'initialisation
	  void SetupWindow() ;
    void SetupToolBar() ;
    void SetWindowPosit() ;

    void setViewName() ;

    void CmEnregistre() ;

    void CmEditContrainte() ;
    void CmEditControle() ;
    void CmCreateEditNode() ;

    void startEditing() ;
    void ammorcerTree(BBItem* pItem, int* piLigne, int iColonne, NSTreeNode* pFatherNode) ;
    void setNodeLabel(NSTreeNode* pNode) ;
    void metAjour(NSTreeNode* pNSTreeNode) ;

    void MiseAjourNSTreeNode(NSTreeNode* pNSTreeNode) ;
    void ReMiseAjourNSTreeNode(NSTreeNode* pNSTreeNode,
                                  string pStringCode, string sComplement) ;
    void CreerBBItem(NSTreeNode* pNSTreeNode, NSTreeNode* pNSTreeNodePetitFrere) ;

    string getPathForNode(NSTreeNode* pNSTreeNode) ;
    string getLabelForNode(NSTreeNode* pNSTreeNode) ;
    string getExtendedPathForNode(NSTreeNode* pNSTreeNode) ;

    void SelChanged() ;
    void Absence() ;
    void Presence() ;
    void Parametres() ;
    void FixeCertitude(int iCert) ;

    void EvDelete() ;
    void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;

    void EvSize(uint sizeType, NS_CLASSLIB::TSize& size) ;
    void EvSysCommand(uint cmdType, NS_CLASSLIB::TPoint& point) ;
    void EvSetFocus(THandle hWndLostFocus) ;
    void EvClose() ;
    void EvDestroy() ;
    bool EvQueryEndSession() ;
    bool PreProcessMsg(MSG &msg) ;

    // Document notifications
    bool VnIsWindow(HWND hWnd) { return NSTreeWindow::HWindow == hWnd ; }
    bool CanClose();
    void CmFileClose();

    void initMUEViewMenu(string sMenuName) ;
    void activateMUEViewMenu() ;

  protected:

    string _sViewName ;

    string buildNodeString(NSTreeNode* pNSTreeNode) ;

  DECLARE_RESPONSE_TABLE(NSArcEditTreeView) ;
};

class NSContListDialog : public NSUtilDialog
{
  public:

    OWL::TEdit*         pLabel ;
    OWL::TGroupBox*     pOblig ;
    OWL::TRadioButton*  pObligOui ;
    OWL::TRadioButton*  pObligNon ;
    NSEditNum*          pMin ;
    NSEditNum*          pMax ;

    Citem*              pCitemPere ;
    string              sLabel ;
    bool                bOblig ;
    bool                bIsNeeded ;
    string              sFilsMin ;
    string              sFilsMax ;
    bool                bIsExist ;

    NSContListDialog(TWindow* pere, string sLabelNode, Citem* pCitemNode, NSContexte* pCtx) ;
    ~NSContListDialog() ;

    void SetupWindow() ;
    void CmOk() ;
    void CmCancel() ;

  DECLARE_RESPONSE_TABLE(NSContListDialog) ;
};

class NSControlTextDialog : public NSUtilDialog
{
  public:

    OWL::TEdit*         pLabel ;
    OWL::TGroupBox*     pActif ;
    OWL::TRadioButton*  pActifOui ;
    OWL::TRadioButton*  pActifNon ;
    OWL::TEdit*         pTexte ;

    Citem*              pCitem ;
    string              sLabel ;
    bool                bActif ;
    string              sTexte ;

    NSControlTextDialog(TWindow* pere, string sLabelNode, Citem* pCitemNode, NSContexte* pCtx) ;
    ~NSControlTextDialog() ;

    void SetupWindow() ;
    void CmOk() ;
    void CmCancel() ;

  DECLARE_RESPONSE_TABLE(NSControlTextDialog) ;
};

#endif // __NSARCEDITTREE_H

