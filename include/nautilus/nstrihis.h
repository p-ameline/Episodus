//  Traitement de la fiche historique
#ifndef __NSTRIHIS_H#define __NSTRIHIS_H

class NSDocumentInfo ;
class NsFindOtherPatientDialog ;
class ClasseStringVector ;

#include "nsbb\nstrewi.h"#include "nsbb\nstrnode.h"#include "nautilus\nshistdo.h"
#include "nsbb\nsbbsmal.h"

bool tri(NSDocumentInfo* s, NSDocumentInfo* b);
//--------------------------------------------------------------------------// NSTreeHistorique dérive de ma classe  NSTreeWindow et permet l'affichage
// des documents dans la fiche historique
//--------------------------------------------------------------------------

class NSTreeHistorique : public NSTreeWindow{
	public:

  	OWL::TImageList* _ImagesHisto ;    NSHISTODocument* _pDoc ;
    string           _sImportance ;
    NSSmallBrother*  _pBigBoss ;
    NSPatPathoArray  _NSPatPathoArray ;
    //constructeurs  Destructeur    NSTreeHistorique(TWindow* parent, NSContexte* pCtx, int id, int x, int y, int w, int h,
                	 NSHISTODocument* pDocu, TModule* module = 0, TStyle style = twsNone) ;
    virtual ~NSTreeHistorique() ;

        // Surcharges des opérateurs		NSTreeHistorique& operator=(const NSTreeHistorique& src) ;

    //méthodes    void SetupWindow() ;

    void             AfficheDocument() ;
    void             AjouteDocument(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray* pNSPatPathoArray,
                            NSNoyauDocument* pNouveauDocument = 0) ;
    void             DateCreation(NSDocumentHisto* pNSDocumentInfo) ;
    void             Interet(NSTreeNode* pNSTreeNode) ;
		void             DecomposeChaine(string* pChaine, ClasseStringVector* pVect, string separateur ) ;
    iterNSTreeNode   TrouveNoeud(string codeDocBrut) ;

    NSTreeNode*      getNodeForPoint(NS_CLASSLIB::TPoint point) ;
    NSTreeNode*      getSelectedNode() ;
    NSDocumentHisto* getDocumentForNode(NSTreeNode* pNSTreeNode) ;
    NSDocumentHisto* getSelectedDocument() ;

    void AfficheInteret(NSTreeNode* pNSTreeNodePere) ;
    // Fonction AfficheInteretMaximum surchargée (venant de NSTreeWindow)    // pour éviter que les documents de l'historique affichent un intérêt
    void AfficheInteretMaximum(NSTreeNode* pNSTreeNode) ;
    // idem dans l'autre sens (expand)
    void AfficheInteretReel(NSTreeNode* pNSTreeNode) ;

    void SetLabelNode(NSTreeNode* pNSTreeNode, NSDocumentHisto* pDocHisto) ;    void AffichePatho() ;
    bool isDisplayableDoc(NSDocumentHisto* pDocumentHisto) ;
    bool isCsDoc(NSDocumentHisto* pDocumentHisto) ;
    bool isCrDoc(NSDocumentHisto* pDocumentHisto) ;
    void GetDisplayablePathoForCr(NSDocumentHisto* pDocumentHisto, string sImportance, NSPatPathoArray* pPatPatho) ;
    void EnlevePatho() ;
    void AfficheIcone(int iCategorie, NSTreeNode* pNSDocNode) ;
    void EnleverDocument(NSDocumentInfo* pNSDocumentInfo) ;
    void VisualiserPatho(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray* pNSPatPathoArray) ;
    void VisualiserDocument(NSTreeNode* pNSDocNode) ;
    void FermetureDocument(NSDocumentInfo* pDocumentInfo) ;
    void AutoriserOuverture(NSDocumentInfo* pDocument) ;
    void AutoriserEdition(NSDocumentInfo* pDocument) ;
    void OuvertureDocument(NSTreeNode* pNSDocNode) ;
    void EditionDocument(NSTreeNode* pNSDocNode) ;
    bool ExisteDocumentOriginal(NSDocumentInfo* pDocument) ;
    void OuvertureDocumentOriginal(NSDocumentInfo* pDocument) ;
    void OuvertureDocument(string sDocID) ;

    void EvLButtonDblClk(uint, NS_CLASSLIB::TPoint&) ;    void EvTvKeyDown(TTwKeyDownNotify& nhmr) ;
    void EvKeyUp(uint key, uint repeatcount, uint flags) ;
    void EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point) ;
    void EvHScroll(uint scrollCode, uint thumbPos, HWND hWndCtl) ;
    void EvVScroll(uint scrollCode, uint thumbPos, HWND hWndCtl) ;
    void EvSetFocus(HWND hWndLostFocus) ;
    void EvClose() ;
    void SelChanged() ;

    void Ouvrir() ;    void Editer() ;
    void OpenOutside() ;
    void ToutOuvrir() ;
    void Oter() ;
    void Ajouter(NSDocumentInfo* pDocumentInfo) ;
    void Proprietes() ;
    void Parametres() ;
    void Detruire() ;
    void XmlExport() ;
    void Transfert() ;
    void Reassign() ;
    void CreateFrom() ;
    void SwitchVisibleManagementTrees() ;
    void TransfertStart(NsFindOtherPatientDialog* pDialog) ;

    int  donneIndexInteret(char cInteret) ;

    bool isManagementDoc(string sDocRoot) ;

    bool areManagementTreesVisible()        { return _bVisibleManagementTrees ; }
    void setVisibleManagementTrees(bool bV) { _bVisibleManagementTrees = bV ; }

    string getImportance()          { return _sImportance ; }
    void   setImportance(string sI) { _sImportance = sI ; }

  protected:

    bool _bVisibleManagementTrees ;

	DECLARE_RESPONSE_TABLE(NSTreeHistorique) ;};

class NSTitre : public TDialog, public NSRoot{
    public:

        OWL::TEdit*	pTitre;	//valeur exacte        string		sTitre;
        TWindow* 	AParent;
        int		   	ressource;

		NSTitre(TWindow* pere, TResId res, NSContexte* pCtx, string sTitre );		~NSTitre();

        void CmOk();        void CmCancel();
        void SetupWindow();

    DECLARE_RESPONSE_TABLE(NSTitre);};

//
// Dialog box used to find another patient
//
class NsFindOtherPatientDialog : public NSUtilDialog
{
	public:

		OWL::TStatic*	_pNom ;
		OWL::TStatic*	_pPrenom ;

    OWL::TStatic*	_pNomTxt ;
		OWL::TStatic*	_pPrenomTxt ;

		OWL::TButton*	_pLookForPatient ;
		OWL::TButton*	_pReassignDoc ;

		string _sPatCode ;

    NSTreeHistorique* _pHistoTree ;

    NSPatInfo*        _pPatEncours ;

		NsFindOtherPatientDialog(TWindow* pere, NSContexte* pCtx, NSTreeHistorique* pHistoTree) ;
		~NsFindOtherPatientDialog() ;

		void SetupWindow() ;

    void GetPatient() ;
		void Reassign() ;

	DECLARE_RESPONSE_TABLE(NsFindOtherPatientDialog) ;
};

#endif
