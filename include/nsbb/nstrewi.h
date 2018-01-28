#ifndef __NSTREWI_H#define __NSTREWI_H

class ChampEdition ;
class NSDate ;
class NSEditDico ;
class PositionDragAndDrop ;

class NSPatPathoArray ;
class NSTreeNodeArray ;
class VecteurChaine ;
class NSConcern ;

class NSEncyclo ;
class NSCsVue ;
class NSCV ;

class NSTreeNode ;
class NSControle ;
class BBItemData ;
#ifndef __NSBBITEM_H
class BBItem ;
class BBFilsItem ;
#endif
class Message ;
class NSCutPaste ;
#ifndef __NSPATPAT_H__
class NSCutPastTLArray ;
#endif

class NSSmallBrother ;
class NSPatPathoInfo ;
class HistoryConnector ;
class NSVectFatheredPatPathoArray ;
class VecteurString ;

#include <owl/treewind.h>#include <owl/listwind.h>#include <owl/imagelst.h>
#include <winuser.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsbb\nsexport.h"
#include "nsbb\nstrnode.h"// #include "nsbb\nsdico.h"
// #include "nsbb\nspatpat.h"
// #include "nautilus\nsldvdoc.h"
#include "nssavoir\nsencycl.h"
#include "nssavoir\nsconver.h"
#include "nsbb\nsbb_glo.h"

#define TEXTE_LIBRE_LEN		40
//// Handler for child ID notifications that are handled at the child
//
// No arguments are passed, i.e: void method()
//

/*#define EV_NOTIFY_AT_CHILD(notifyCode, method)\  {notifyCode, UINT_MAX, (TAnyDispatcher)::v_Dispatch,\
   (TMyPMF)v_Sig(&TMyClass::method)} */

//// Notification handled by parent with handler expecting TTwDispInfoNotify& and
// returning a bool.    i.e. bool method(TTwDispInfoNotify& nmHdr)
//
#define EV_TVN_BEGINLABELEDIT(id, method) EV_TV_DISPINFO_NOTIFY_BOOL(id, TVN_BEGINLABELEDIT, method)

#define NS_TV_DISPINFO_NOTIFY_BOOL(notifyCode, method)\  {notifyCode, UINT_MAX, (TAnyDispatcher)::B_LPARAM_Dispatch,\
  (TMyPMF)b_TV_DISPINFO_NOTIFY_Sig(&TMyClass::method)}
#define NS_TVN_BEGINLABELEDIT(method) NS_TV_DISPINFO_NOTIFY_BOOL(TVN_BEGINLABELEDIT, method)

// bool method(TTwNotify& nmHdr)
//
#define NS_EV_TREEWIND_NOTIFY_BOOL(notifyCode, method)\  {notifyCode, UINT_MAX, (TAnyDispatcher)::B_LPARAM_Dispatch,\
  (TMyPMF)b_TREEWIND_NOTIFY_Sig(&TMyClass::method)}

#define NS_TVN_BEGINRDRAG(method) NS_EV_TREEWIND_NOTIFY_BOOL(TVN_BEGINRDRAG, method)
#define NS_TVN_BEGINDRAG(method) NS_EV_TREEWIND_NOTIFY_BOOL(TVN_BEGINDRAG, method)

#define NS_TVN_ITEMEXPANDING(method) NS_EV_TREEWIND_NOTIFY_BOOL(TVN_ITEMEXPANDING, method)
#define NS_TVN_ITEMEXPANDED(method)  NS_EV_TREEWIND_NOTIFY_BOOL(TVN_ITEMEXPANDED, method)

// void method(TTwKeyDownNotify& nhmr)
//
#define NS_TV_KEYDOWN(method)\  {TVN_KEYDOWN, UINT_MAX, (TAnyDispatcher)::v_LPARAM_Dispatch,\
  (TMyPMF)v_TV_KEYDOWN_NOTIFY_Sig(&TMyClass::method)}

//--------------------------------------------------------------------------
// classe  NSTreeWindow
//--------------------------------------------------------------------------

const int TreeEditIdAsConst = 102 ;
class _NSBBCLASSE NSTreeWindow : public TTreeWindow, public NSRoot
{
	public:

    bool                 bIniting ;             // En cours d'initialisation
    bool                 _bLoosingFocus ;       // Focus goes away, stop editing

		NSTreeNodeArray      _NodeArray ;           //vecteur des noeuds(NSTreeNode)
    bool                 ReadOnly ;
    bool                 AfficheCopieCollee ;   //afficher le contenu du copie collée
                                                //à l'ouverture d'un document
    bool                 bLanceEdit ;	          //savoir si on perd le focus au profit du pEDitDico
    bool                 bPaint ;    	          //pour ignorer les killFocus tant que
                                                //la treeview n'a pas été peinte
    bool                 bEditionDico ;         // pour savoir si le pEditDico en cours est en édition
    bool                 bCurseurTexte ;        // pour controler le type de curseur
    bool                 bForceEdit ;           // Permet l'édition des éléments codés    string               skinName ;             // nom de la fenetre de skin    NSControle*          _pControle ;
    NSEditDico*	  	  	 _pEditDico ;           //champ édit pour accéder au lexique
    NS_CLASSLIB::TRect   rectEditDico ;         // rectangle d'edition du pEditDico
    NSTreeNode*          pNSTreeNodeEnCours ;   //le NSTreeNode en cours
    TWindow*             Aparent ;
    bool                 bDirty ;               //le contenu de cet TreeView a changé ou non
    ChampEdition*        pChampEdition ;        //boîte de dialogue des valeurs chiffrées
    NSDate*              pChampDate ;           //boîte de dialogue des dates et heures
    PositionDragAndDrop* pPositionDragAndDrop ; //position drag and drop et copie
    bool                 ModifierTexteLibre ;   //modification de texte libre en cours
    bool                 bDispatcherPatpatho ;  //autorise à dispatcher la patpatho globale
    BBItem*              pBBitemNSTreeWindow ;  //BBItem créateur de la NSTreeWindow
    OWL::TImageList*     Images ;

    string               sLang ;                // Langue d'affichage

    TTwHitTestInfo*      pHitTestInfo ;
    // NSEncyclo*           pNSEncyclo ;           // pointeur sur la base Encyclop.db
    // NSCV*                pNSCV ;                // base convert.db
    NSEncyclo            Encyclo ;                 // base Encyclop.db
    NSCV                 Converter ;               // base convert.db

		//constructeurs  Destructeur
    NSTreeWindow(TWindow* parent, NSContexte* pCtx, int id, int x, int y,
                     int w, int h, TStyle style = twsNone, TModule* module = 0) ;
		NSTreeWindow(TWindow* parent, NSContexte* pCtx, int resourceId = 0,
                     TModule* module = 0) ;
    virtual ~NSTreeWindow() ;
    //copie
    NSTreeWindow(const NSTreeWindow& src) ;
    // Surcharges des opérateurs
		NSTreeWindow& operator=(const NSTreeWindow& src) ;

    void openDbTables() ;
    void closeDbTables() ;

    //méthodes
    NSTreeNode* GetNSTreeNode(OWL::TTreeNode* pTTreeNode) ; //à partir du hitem d'un TTreeNode on retourne le NSTreeNode
      														//correspondant

    NSTreeNode* GetNSTreeNode(HTREEITEM TTitem) ;
    NSTreeNode* GetNSTreeNode(string sNodeID) ;
    void        okFermerDialogue() ;
    void        LibereBBitemLanceur() ;
    void        activeControle(int activation, Message* pMessage) ;
    void        SupprimerTousNoeuds() ;

		HWND EditLabel(HTREEITEM item) ;

    bool       TvnBeginLabelEdit(TTwDispInfoNotify& nmHdr) ;
    bool       TvnItemExpanDing(TTwNotify& nmHdr) ;
    bool       TvnItemExpanded(TTwNotify& nmHdr) ;

    // Response functions for handling dragging
    //
    void       EvLButtonDown( uint modKeys, NS_CLASSLIB::TPoint& pt ) ;
    void       EvLButtonUp( uint modKeys, NS_CLASSLIB::TPoint& pt ) ;
    void       EvTimer( uint id ) ;
    bool       EvTvnBeginDrag(TTwNotify& twn) ;

    void       SynchroInformationForPath(string sPath, NSPatPathoArray* pPpt, string separator = string(1, cheminSeparationMARK)) ;
    void       SynchroInformationForPath(string sPath, string sItemLabel, string sDocID, NSVectFatheredPatPathoArray* pTransPatpatho, HistoryConnector* pHC, string separator = string(1, cheminSeparationMARK)) ;
    string     getModelNodeFromDoc(NSPatPathoInfo* pCopyNode, string sModelDocId, VecteurString *pNodes) ;

    void       CopyBranch(NSTreeNode* pNodeDrop, string sCopie, NSCutPaste* pCP = 0) ;
    void       DragAvant(NSTreeNode*  pNodeDrop, string sCopie, NSCutPaste* pCP = 0) ;
    void       DragApres(NSTreeNode*  pNodeDrop, string sCopie, NSCutPaste* pCP = 0) ;
    void       DragFils (NSTreeNode*  pNodeDrop, string sCopie, NSCutPaste* pCP = 0) ;

    void       SynchronizeNode(NSTreeNode* pNodeDrop, NSCutPaste* pCP) ;
    void       SynchroMarkNodes(NSTreeNode* pRefNode) ;
    void       synchroNode(NSTreeNode* pRefNode, NSPatPathoArray* pPpt) ;
    void       SynchroDeleteNodes(NSTreeNode* pRefNode) ;

    //copie collée coupée
    void       CopierNode(NSTreeNode*  pNSTreeNodeDrag) ;
    void       CouperNode(NSTreeNode*  pNSTreeNodeDrag) ;
    void       CollerNode(NSTreeNode*  pNSTreeNodeDrag) ;

    // fonctions de la table de réponse
    void       CopyNode() ;
    void       CutNode() ;
    void       PasteNode() ;

    void       EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void       EvHScroll(uint scrollCode, uint thumbPos, HWND hWndCtl) ;
    void       EvVScroll(uint scrollCode, uint thumbPos, HWND hWndCtl) ;
    void       Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& rect) ;
    void       EvPaint() ;
    HBRUSH     EvCtlColor(HDC hdc, HWND hwndchild, UINT ctltype) ;
    void       SetDirty(bool bEtat) ;
    bool       GetDirty() ;
    void       EvSetFocus(HWND hWndLostFocus) ;
    void       Initialise(HWND hWndLostFocus) ;
    void       CreeNoeudFictif() ;
    void       EvKillFocus(HWND hWndGetFocus) ;
    TResult    DicoLostFocus(TParam1 param1, TParam2 param2) ;
    void       KillDico() ;
    void       DicoGetTexteLibre(string* pContenu) ;
    void       DicoGetCodeLexique() ;
    string     DicoGetText() ;
    void       EntreeEditionDico() ;
    void       SortieEditionDico() ;
    WNDTYPE    donneType() { return(isTreeWindow) ; }

    void       adaptEditToTreenode() ;

    void       SetupWindow() ;

    void       skinSwitchOn(string sSkinName) ;
    void       skinSwitchOff(string sSkinName) ;
    void       SelChanged() ;
    void       DispatcherPatPatho(const NSPatPathoArray* ContenuPatpatho,
                                  NSTreeNode* pNSTreeNodeLanceur,
                                  int* DecalageLigneFils,
                                  string sHowToInsert,
                                  string sExpand = string(""),
                                  NSCutPastTLArray* pTLArray = (NSCutPastTLArray*) 0) ;

    void       SetNodesIdFromPatho(NSPatPathoArray* pContentPatpatho) ;

    void       EvTvKeyDown(TTwKeyDownNotify& nhmr) ; //touche clavier
    void       EvKeyUp(uint key, uint repeatcount, uint flags) ;
    void       Modifier() ; //modifier un noeud

    NSTreeNode* TrouverGrandFrere(NSTreeNode* pNSTreeNode, int ligne) ;
    NSTreeNode* TrouverPetitFrere(NSTreeNode* pNSTreeNode, int ligne) ;
    NSTreeNode* TrouverPetitFrereFilsRoot(NSTreeNode* pNSTreeNode, int ligne) ;
    NSTreeNode* TrouverGrandFrereFilsRoot(NSTreeNode* pNSTreeNode, int ligne) ;
    NSTreeNode* GetNodeForPath(string sPath, string separator = string(1, cheminSeparationMARK)) ;

virtual void    MiseAjourNSTreeNode(NSTreeNode* pNSTreeNode/*, string codeTexteLibre*/) ; //MAJ de pNSTreeNode
virtual void    ReMiseAjourNSTreeNode(NSTreeNode* pNSTreeNode,
                                 string pStringCode, string codeTexteLibre) ; //MAJ d'un nstreenode qui exsite
		void        DetruitFils(int *cardinal, NSTreeNode* pNSTreeNode) ; //détruire des NSTreeNodes et des TTreeNodes fictifs
    void        AjouteFrereFictifAvant(NSTreeNode*  pNSTreeNode, OWL::TTreeNode* Noeud, int ligne , int colonne, string sFaconAjout = "Fin" ) ;
    void        GetMaxCoordonnees(NSTreeNode* pNSTreeNode, int *ligne, int *colonne, bool bJustReal = false) ; //donne les coordonnees maximales pour les fils
virtual void    CreerBBItem(NSTreeNode* pNSTreeNode, NSTreeNode* pNSTreeNodePetitFrere/*, BBItemData* pDonnees*/);
		NSTreeNode* TrouverPere(int ligne, int colonne) ;
    void        AjouteFrereFictifApres(NSTreeNode*  pNSTreeFrere, int ligne , int colonne) ;
    void        TuerNoeudFictif() ;

    void        UpdateCommandInformation(NSPatPathoInfo* pPptInfo, NSTreeNode* pNSTreeNode = (NSTreeNode*) 0) ;

        //Creétion de frère et de fils
        void AjouteFrereFictifTexteLibre(NSTreeNode* pNSTreeNode,
      					int ligne,	int colonne ,  string sTexteLibre, bool bLie = true) ;

        void AutoriserCreationFils(NSTreeNode* pNSTreeNode,OWL::TTreeNode* Noeud) ;
        void AutoriserCreationFrere(NSTreeNode* pNSTreeNode) ;
        void AjouteElementFictif(NSTreeNode* pNSTreeNode, OWL::TTreeNode* Noeud, bool CreerFilsChampEdit, bool bDateHeure) ; //ajouter un NSTreeNode et un TTreeNode fictif
        void AjouteFilsFictif(NSTreeNode* pNSTreeNode, OWL::TTreeNode* pTTreeNode, int ligne, int colonne,
                             string sIdentite, bool estPropose, bool CreerFilsChampEdit, bool bDateHeure) ;
        void CreerNSTreeNode(NSTreeNode* pNewNSTreeNode, int ligne, int colonne,
      							string sIdentite, string sLabel, string sCodeTexteLibre) ;

   	    void RepartirTexteLibre(NSTreeNode* pNSTreeNode, VecteurChaine* VecteurTexte, string* pTexteLibre, int* tailleVecteur) ;
        void NStreeNodeSuivant(bool bThenSortieEditionDico = false) ;
        void NStreeNodePrcedent() ;
        void InsereAvant() ;
        void InsereApres() ;
        void InsertTlInEdit() ;
        void DecomposeChaineTL(string* pChaine, VecteurChaine* pVect ) ;
        void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
        void ReconstruireFrere(NSTreeNode* pNSTreeNode) ;
        void ReconstruireFils(NSTreeNode* pNSTreeNode) ;
virtual void Absence() ;
virtual void Presence() ;
virtual void Parametres() ;
virtual void NewPreoccup() ;
virtual void ChangePreoccup(NSConcern* pConcern) ;
virtual void NewPreoccupRC(NSPatPathoArray* ContenuPatpatho, string sArchetype) ;
virtual void EvolPreoccupRC(NSPatPathoArray* ContenuPatpatho, NSConcern* pConcern, string sArchetype) ;
virtual void ChangePreoccupRC(NSPatPathoArray* ContenuPatpatho, NSConcern* pConcern, string sArchetype) ;
virtual void ConnectToPreoccup() ;
        void Classifier() ;
        void FixeInteret(char KbKey) ;
virtual void FixeCertitude(int iCert) ;
        void FixeDroits() ;
        virtual void EditionFilGuide() ;
        virtual void Encyclop() ;
        virtual void Certificat() ;
        virtual int  donneIndexInteret(char cInteret) ;

        //valeurs chiffrées
        void   RecupereValeurChiffree(NSTreeNode* pNSTreeNode, bool bCalcul = false) ;
        void   RecupereUniteDateHeure(NSTreeNode* pNSTreeNode) ;
        string TrouverUnite(string sQualifie) ;
        string chercheClefComposite(string sQualifie, string sLien) ;

        void DecomposeChaineTL(VecteurChaine* pVect, string sChaine) ;
        void EvLButtonDblClk(uint, NS_CLASSLIB::TPoint&) ;
        void developper(NSTreeNode* pNSTreeNode , OWL::TTreeNode* Noeud, int* DecalageLigneFils,							int ligne, int colonne, bool CreerFilsChampEdit, bool bDateHeure,
                                            bool bRechercheFilGuide = false, bool bInit = false) ;
        bool RechercheFilGuide(NSTreeNode* pNSTreeNode, BBItemData* pDonnees, BBFilsItem* pBBfils) ;

        void Valider(bool bInit = false) ;
        void NodeRemoteValidation(NSTreeNode* pNSTreeNode) ;
        void AfficheLibelle() ;
        // affiche pour pNSTreeNode l'intêret le plus élevé
        // parmi ceux de ses descendants
        virtual void AfficheInteretMaximum(NSTreeNode* pNSTreeNode) ;
        // affcihe l'interet réel lié au pNSTreeNode
        virtual void AfficheInteretReel(NSTreeNode* pNSTreeNode) ;

        //suppression
        void TuerFilsFictif(NSTreeNode* ) ;
        void SupprimerNoeud(NSTreeNode* pNSTreeNodeSuppr) ;
        void SupprimerFils(NSTreeNode* pNSTreeNode, int* NbNoeudSupprime, bool* PatpthoNonVide) ;
virtual void EvDelete() ;

        void EnregistrePatPatho(NSSmallBrother* pBigBoss) ;

        bool canWeClose() ;

        bool isRealNode(NSTreeNode* pNSTreeNode) ;

    NSEditDico* getEditDico() const          { return _pEditDico ; }
    void        setEditDico(NSEditDico* pED) { _pEditDico = pED ;  }

    NSControle* getControl() const           { return _pControle ; }
    void        setControl(NSControle* pCt)  { _pControle = pCt ;  }

    void        setLoosingFocus(bool bLF)    { _bLoosingFocus = bLF ; }

    string      getHandleAsString() ;
    string      getEditDicoHandleAsString() ;

  protected:

    static int TreeEditId ;

	DECLARE_RESPONSE_TABLE(NSTreeWindow) ;
};

#endif

