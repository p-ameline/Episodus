////  NSSmallBrother
//
#ifndef __NSBBSMAL_H
#define __NSBBSMAL_H

class NSPathoArray ;
class NSMagicView ;
class NSDialog ;
class BBFilsItem ;
class NSBBMUEView ;
class HistoryConnector ;

#include <cstring.h>
#include <list.h>
#include <owl\module.h>
#include <owl\docview.h>

#include "nssavoir\nsguide.h"
#include "nsbb\nspatpat.h"
#include "nautilus\nsrechd2.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\nspathoLus.h"
#else
  #include "nssavoir\nspatho.h"
#endif

#include "nsbb\nsExport.h"

#ifdef __OB1__
  #include "ns_ob1\InterfaceForKs.h"
#endif

struct NSModuleInstance
{
  public :

    TModule* _pTModule ;
    int      _iInstanceCount ;

    ~NSModuleInstance() ;
};

// Classe de stockage d'une DLL
// Elle possède un pointeur sur TModule et une chaine contenant
// le libelle fichier dialogue dialogue
class _NSBBCLASSE NSModuleDialogue
{
  public :

    //Constructeur
    NSModuleDialogue() ;
    NSModuleDialogue(const NSModuleDialogue& o) ;

    //Destructeur
    ~NSModuleDialogue() ;

    string 	 getLabel()             { return _sLibelle ; }
    void  	 setLabel(string sL)    { _sLibelle = sL ; }

    TModule* getModule() ;
    void     setModule(TModule* pM) ;

    NSModuleDialogue& operator=(const NSModuleDialogue& src) ;

  protected :

    string 	          _sLibelle ;    // [BB_FICHIER_DIALOGUE_LEN + 1] ; //BB KE_
    NSModuleInstance* _pModule ;

    void freeModule() ;
    void getModule(NSModuleInstance* pModuleInstance) ;
};

typedef vector<NSModuleDialogue>       NSTModuleArray ;
typedef NSTModuleArray::iterator       iterNSTModuleArray ;
typedef NSTModuleArray::const_iterator iterNSTModuleArrayConst ;

class _NSBBCLASSE NSResModuleArray : public NSTModuleArray
{
  public :

    // Constructeurs
    NSResModuleArray() : NSTModuleArray() {}
    NSResModuleArray(const NSResModuleArray& rv) ;

    // Destructeur
    ~NSResModuleArray() ;
    NSResModuleArray& operator=(const NSResModuleArray& src) ;
    void vider() ;
};

//----------------------------------------------------------------------
// permet de mémoriser les valeurs des champs Edit introduites les boîtes
// de calcul
//----------------------------------------------------------------------
class _NSBBCLASSE classCalcul
{
  public:

    string sLocalisation ;
    string sContenu ;

    classCalcul(string sLoca = string(""), string sCont = string("")) ;
    classCalcul(const classCalcul& src) ;
    classCalcul& operator=(const classCalcul& src) ;
    bool         operator==(const classCalcul& o) ;
};

typedef vector<classCalcul>             vectclassCalcul ;
typedef vectclassCalcul::iterator       iterStringCalcul ;
typedef vectclassCalcul::const_iterator iterStringCalculConst ;

class _NSBBCLASSE ClasseCalculVector : public vectclassCalcul
{
  public:

    ClasseCalculVector() ;
    ~ClasseCalculVector() ;
    ClasseCalculVector(const ClasseCalculVector& src) ;
    // Surcharge de l'opérateur d'affectation
    ClasseCalculVector& operator=(const ClasseCalculVector &src) ;
    void vider() ;
};

//----------------------------------------------------------------------
// Classe d'instructions de navigation dans le noyau
//----------------------------------------------------------------------
class _NSBBCLASSE BBCmdMessage
{
  public:

    int    iCmd ;
    string sMessage ;

    BBCmdMessage(int cmd = 0, string message = string("")) ;
    BBCmdMessage(const BBCmdMessage& src) ;

    BBCmdMessage& operator=(const BBCmdMessage& src) ;
    bool          operator==(const BBCmdMessage& o) ;

    string getMessage() const { return sMessage ; }
    int    getCmd()     const { return iCmd ;     }
};

typedef list<BBCmdMessage*>          vectbbcmdmsg ;
typedef vectbbcmdmsg::iterator       iterBBCmdMsg ;
typedef vectbbcmdmsg::const_iterator iterBBCmdMsgConst ;

class _NSBBCLASSE BBCmdMsgVector : public vectbbcmdmsg
{
  public:

    BBCmdMsgVector() ;
    ~BBCmdMsgVector() ;
    BBCmdMsgVector(const BBCmdMsgVector& src) ;
    // Surcharge de l'opérateur d'affectation
    BBCmdMsgVector& operator=(const BBCmdMsgVector& src) ;
    void vider() ;
};

class NSTreeWindow;
class _NSBBCLASSE BBItem;
class _NSBBCLASSE nsarcParseur;
class _NSBBCLASSE nsGuidelineParseur ;

class _NSBBCLASSE NSSmallBrother : public NSRoot
{
	public :

		TWindow* 		        _pFenetreMere ;

    NSResModuleArray    _aResModuleArray ;
    NSPatPathoArray*    _pPatPathoArray ;
    NSPatPathoArray     _TmpPPtArray ;
    BBItem*             _pBBItem ;           // BBItem père de tous les autres
#ifndef __OB1__
    bool                _bInitFromBbk ;
#endif

		ClasseCalculVector	_VectorCalcul ;
    BBCmdMsgVector      _aCmdMsgArray ;

    NSMagicView* 		    _pNSSmallVue ;    char 				        _contexteModule ;
    string 			        _sLexiqueModule ;    // [MODU_LEXIQUE_LEN+1]
    string 			        _sNoeudModule ;      // [PPD_NOEUD_LEN+1]

    BBFiche*       		  _pBBFiche ;

    bool 				        _bCorrection ;
    string 				      _sCheminCorrection ;
    string 				      _sPositionConclusion ;

    string              _sFichierSauveRoot ;
    int                 _iSeuilSauve ;        // Niveau de BBItem en dessous
                                             // duquel on enregistre
    int                 _iNumSauve ;          // Numéro de sauvegarde en cours
    int                 _iNumMaxSauve ;       // Numéro max (pour redo)

#ifdef __OB1__
    NSSmallBrother(NSContexte* pCtx, NSPatPathoArray* pArray, NSMagicView* pMagVue = 0) ;
#else
		NSSmallBrother(NSContexte* pCtx, NSPatPathoArray* pArray, NSMagicView* pMagVue = 0, bool initFromBbk = false) ;
#endif
    ~NSSmallBrother() ;
    NSSmallBrother(NSSmallBrother& src) ;

    void        AjouteVecteur(string sLocalisation , string sContenu) ;
    void        RemplirVecteur(string sLocalisation, string sContenu) ;
    string      TrouverElement(string sLocalisation) ;

    bool        lanceModule() ;
    bool        lanceScript() ;
    bool        lanceScript(NSTreeWindow* pTreeWind) ;
#ifdef __OB1__
    NSDialog*   lanceBbkArchetypeInDialog(string sArchetype, nsarcParseur* pParseur, BBFilsItem* pFilsPere, BB1BBInterfaceForKs* pKsInter = NULL, bool bModal = false) ;
    bool        lanceBbkArchetypeInView(string sArchetype, nsarcParseur* pParseur, BBFilsItem* pFilsPere, NSBBMUEView* pView, BB1BBInterfaceForKs* pKsInter = NULL) ;
    // bool        lanceBbkDecisionTreeInView(string sArchetype, nsGuidelineParseur* pParseur, BBFilsItem* pFilsPere, NSBBMUEView* pView, BB1BBInterfaceForKs* pKsInter = NULL) ;
#else
		NSDialog*   lanceBbkArchetype(string sArchetype, nsarcParseur* pParseur, BBFilsItem* pFilsPere, bool bModal = false, NSBBMUEView* pView = NULL) ;
#endif
    bool        fermeBbkArchetype(int iReturn, TWindow* view) ;
    void        canCloseArchetype(bool bCanClose, bool bCanCloseDPIO = true) ;
    bool        ouvreResModule() ;
    bool        ouvreScript() ;
    void        MetTitre() ;  			// ajoute le titre au compte rendu :  exemple GECHY
    void        EnleverTitre() ;
    bool        corrigeModule(string* pLocLes) ;
    bool        SupprimerLigneCR(string* pLocalisation) ;
    // méthode (param : libelle) qui retourne un pointeur sur le module(pTModule) de l'élément si libelle
    // existe
    TModule*    TrouverModule(string sLibelle, bool bCreateIfMissing = true) ;

    void        InjectHistoryElements() ;
    void        InjectHistoryElementsFromConcern(HistoryConnector *pHistConn) ;
    void        InjectHistoryElementsFromSynthesis(HistoryConnector *pHistConn) ;
    void        CleanHistoryPatpatho(NSPatPathoArray* pHistoPpt, HistoryConnector *pHistConn) ;

    NSPatPathoArray* getPatPatho()   { return _pPatPathoArray ; }
    NSPatPathoArray* getTmpPatho()   { return &_TmpPPtArray ; }
#ifndef __OB1__
    bool IsInitFromBbk()             { return _bInitFromBbk ; }
#endif
    bool EstFormulaire() ;

    string 			     getLexiqueModule()           { return _sLexiqueModule ; }
    void  			     setLexiqueModule(string sLM) { _sLexiqueModule = sLM ; }

    BBItem*          getBBItem()                  { return _pBBItem ; }
    int              getSeuilSauve()              { return _iSeuilSauve ; }

    TWindow* 		     getMotherWindow()            { return _pFenetreMere ; }
    void 		         setMotherWindow(TWindow* pM) { _pFenetreMere = pM ; }

    bool 				     isEditionMode()              { return _bCorrection ; }
    void 				     setEditionMode(bool bEM)     { _bCorrection = bEM ; }

    // méthode de param = libellé , si libelle existe ne fait rien, sinon créee un nouveau TModule et une
    // nouvelle fiche NSModuleDialogue qu'il ajoute au vecteur.
    bool CreerNouveauModule(string sLibelle) ;

    //******************************************
    // gestion de la conclusion
    //******************************************
    PatPathoIter ChercherItem(NSPatPathoArray* pPPT, string sItem) ;
    // void ajoutePatho(NSPatPathoArray* pPPT, string sItem, int ligne, int colonne);
    bool ConclusionAutomatique() ;    void preparePrevAndNewConc(NSPatPathoArray* pPreviousPPT, NSPatPathoArray* pNewPPT) ;    void arbitrateConcChanges(NSPatPathoArray* pPreviousPPT, NSPatPathoArray* pNewPPT) ;    bool areConcDifferent(NSPatPathoArray* pPreviousPPT, NSPatPathoArray* pNewPPT) ;
    void MettreAjourPatpatho(NSPatPathoArray* pPathDestination,											   NSPatPathoArray* pPathSource) ;
    void ConcatenerPatpatho(NSPatPathoArray* pPatpathoSource,
                                   NSPatPathoArray* pPatpathoDestination,
                                   bool bDecalage = true) ;

    void ConcatenerPatpatho(string sItem, NSPatPathoArray* pPatpathoSource,
                                   NSPatPathoArray* pPatpathoDestination,
                                   bool bDecalage = true) ;

    void ConclusionManuelle(NSPatPathoArray* pPatpathoSource,
                                   NSPatPathoArray* pPatpathoDestination) ;
    void Conclusion() ;
    void EditionTexteLibre() ;
    void Codage() ;

    void lanceConsult(string sTypeDocument, NSTreeWindow* pNSTreeWindow) ;
    void lance12(string sNoeud1, string sNoeud2) ;

    void ActiverPatpathoBBfils(NSPatPathoArray* pPatpathoActuelle,   											BBItem* pBBItem, PatPathoIter iterPere) ;
    bool RechercheSemantique(string sItem, string sLocalisation) ;

    bool CreateBBItemFromFiche() ;

    //
    // Gestion du décodage
    //
    bool Decode(string sLexPere, NSPatPathoArray* pPatpathoFils, string sChemin,
   							int nbStrRet, string** pDecodPtr) ;

    //
    // Gestion des messages
    //
    bool pileVide() { return _aCmdMsgArray.empty() ; }
    int  depile(BBCmdMessage* pCmdMsg = 0) ;
    void empile(BBCmdMessage* pCmdMsg, bool tuer = true) ;
    void reEmpile(BBCmdMessage* pCmdMsg, bool tuer = true) ;

    //
    // Gestion des sauvegardes temporaires et des undo
    //
    void repriseSauvegarde() ;
    void effacerSauvegarde() ;
    void sauvegarde() ;
    void reprise() ;
    void defaire() ;
    void refaire() ;

    void rapatriePatpatho(BBItem* pItem) ;
    void showBB() ;

    void   setUseAutoConc(bool bUAC) { _bUseAutoConc = bUAC ; }
    bool   getUseAutoConc()          { return _bUseAutoConc ; }

    void   setModuleName(string sMN) { _sModuleName = sMN ; }
    string getModuleName()           { return _sModuleName ; }

  protected:

    bool   _bUseAutoConc ;
    bool   _bMustDeleteTmpFile ;
    string _sModuleName ;
};

#endif

