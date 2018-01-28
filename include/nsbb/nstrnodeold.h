#ifndef __NSTRNODE_H
#define __NSTRNODE_H

#include <owl\owlpch.h>
#include <owl\edit.h>
#include <owl/treewind.h>
#include <owl/button.h>
#include <owl/menu.h>

#include "nsbb\nStlibre.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nsdico.h"
#include "nsbb\nspardlg.h"
#include "nsbb\nsedcon.h"

#include "partage\ns_vector.h"
#define TEXTE_LIBRE_LEN		40
//
// Handler for child ID notifications that are handled at the child
//
// No arguments are passed, i.e: void method()
//
/*#define EV_NOTIFY_AT_CHILD(notifyCode, method)\
  {notifyCode, UINT_MAX, (TAnyDispatcher)::v_Dispatch,\
   (TMyPMF)v_Sig(&TMyClass::method)} */

//
// Notification handled by parent with handler expecting TTwDispInfoNotify& and
// returning a bool.    i.e. bool method(TTwDispInfoNotify& nmHdr)
//
#define NS_TV_DISPINFO_NOTIFY_BOOL(notifyCode, method)\
  {notifyCode, UINT_MAX, (TAnyDispatcher)::B_LPARAM_Dispatch,\
  (TMyPMF)b_TV_DISPINFO_NOTIFY_Sig(&TMyClass::method)}
#define NS_TVN_BEGINLABELEDIT(method) NS_TV_DISPINFO_NOTIFY_BOOL(TVN_BEGINLABELEDIT, method)

#define NS_EV_TREEWIND_NOTIFY_BOOL(notifyCode, method)\
  {notifyCode, UINT_MAX, (TAnyDispatcher)::B_LPARAM_Dispatch,\
  (TMyPMF)b_TREEWIND_NOTIFY_Sig(&TMyClass::method)}
#define NS_TVN_ITEMEXPANDING(method) NS_EV_TREEWIND_NOTIFY_BOOL(TVN_ITEMEXPANDING, method)


#define NS_TV_KEYDOWN(method)\
  {TVN_KEYDOWN, UINT_MAX, (TAnyDispatcher)::v_LPARAM_Dispatch,\
  (TMyPMF)v_TV_KEYDOWN_NOTIFY_Sig(&TMyClass::method)}

//-------------------------------------------------------------------------
//		VecteurChaine est un vecteur de string
//-------------------------------------------------------------------------
class  Parametre;
typedef vector<string*> EquiItem;
typedef EquiItem::iterator IterString;
class VecteurChaine : public EquiItem
{
	public:
	   VecteurChaine() ;
   	~VecteurChaine();
   	VecteurChaine(VecteurChaine& src);
   	// Surcharge de l'opérateur d'affectation
		VecteurChaine& operator=(VecteurChaine src);
   	void vider();
};

//-------------------------------------------------------------------------
//		classe    NSTreeNode
//-------------------------------------------------------------------------

class	NSTreeNode;

typedef vector<NSTreeNode*> NSNSTreeNodeArray;
typedef NSNSTreeNodeArray::iterator iterNSTreeNode;

class NSTreeNodeArray : public  NSNSTreeNodeArray
{
  public :
		// Constructeurs
		NSTreeNodeArray(): NSNSTreeNodeArray() {}
		NSTreeNodeArray(NSTreeNodeArray& rv);
		// Destructeur
		virtual ~NSTreeNodeArray();
		NSTreeNodeArray& NSTreeNodeArray::operator=(NSTreeNodeArray src);
      void vider();
      void EnleverElement(NSTreeNode* pNSTreeNodeSuppr);
      bool Contient(string sEtqiuette);
};

class NSFilsTreeNodeArray : public NSTreeNodeArray
{
 	public :
   		// Constructeurs
		NSFilsTreeNodeArray() : NSTreeNodeArray() {}
		NSFilsTreeNodeArray(NSFilsTreeNodeArray& rv);
		// Destructeur
		virtual ~NSFilsTreeNodeArray();
		NSFilsTreeNodeArray& NSFilsTreeNodeArray::operator=(NSFilsTreeNodeArray src);
      void vider();
};

class NSFrereTreeNodeArray : public NSTreeNodeArray
{
 	public :
   		// Constructeurs
		NSFrereTreeNodeArray(): NSTreeNodeArray() {}
		NSFrereTreeNodeArray(NSFrereTreeNodeArray& rv);
		// Destructeur
		virtual ~NSFrereTreeNodeArray();
		NSFrereTreeNodeArray& NSFrereTreeNodeArray::operator=(NSFrereTreeNodeArray src);
      void vider();
};



class _TYPEDECLASSE  NSTreeNode : public TTreeNode, public NSRoot
{
   public:

      NSControle* 		pControle;
      uint					nMaxInput;
      string 				sIdentite; //étiquette
      string				sInteret;
      string				sPluriel;
      string				sCertitude;
      string 				sLocalisation; // Position dans l'arbre
      Parametre*			pParametre;
	   char 					szType[25]; // Type de contrôle EDIT
      NSTreeNode*			pere;	//pere créateur du NSTreeNode
      NSFilsTreeNodeArray	VectFils; //vecteur des fils d'un NSTreeNode
      NSFrereTreeNodeArray	VectFrereLie; //vecteur des frères liés à un NSTreeNode
      NSTreeNode*			FrerePilote;	//frère contenant le texte libre
      bool					Absence; //absence ou non de la pathologie
      bool					estPropose; //élément du fil guide proposé mais non validé
      bool					CreateurChampEdit;	//élément recevant une valeur chiffrée
      string				sContenuBrut, sContenuTransfert, Type;
      string				sLabel; 	//	label du NSTreeNode : exp grippe
      string 				code;   	//	texte libre : code de la fiche contenant le texte dans ce noeud
      string 				sTexteGlobal; //contient le texte libre global de ce NStreeNode

      NSTreeNode(TTreeWindow& treeWind, NSSuper* pSup, HTREEITEM hItem = TVI_ROOT);
      NSTreeNode(const TTreeNode& treeNode, TNextCode nextCode, NSSuper* pSup);
      NSTreeNode(const TTreeNode& treeNode, NSSuper* pSup);
      //copie
   	NSTreeNode(NSTreeNode& src);
      virtual ~NSTreeNode();
     // Surcharges des opérateurs
		NSTreeNode& operator=(NSTreeNode src);
      //
      // methodes
      //
      void activeControle(int activation, Message* pMessage = 0)  ;
      uint Transferer(TTransferDirection direction, int* pActif, Message* pMessage = 0);
      uint TransfererFinal(TTransferDirection direction, int* pActif, Message* pMessage = 0);

		int donneType() { return(isTreeNode); }
      void DicoKillFocus(string* pContenu, string NewType);
      void ctrlNotification();
      void MettreAJourLigne(int Decalageligne); //decalage des lignes de Decalageligne
      bool Descendant(NSTreeNode* pNSTreeNode);
      bool estFictif();
      bool estLie();
      bool estFictifPur();
      void RecupereParametre();
      void MettreMonEtiquetteAJour();
      int  getLigne() 	 { return ligne; }
      int  getColonne()  { return colonne; }

      void setLigne(int Ligne) 	{ligne = Ligne; }
      void setColonne(int Colonne) { colonne = Colonne; }

      private:

      int 			ligne;  	//numéro de la ligne du NSTreeNode
      int 			colonne; //numéro de la colonne du NSTreeNode


};

//--------------------------------------------------------------------------
// classe  NSTreeWindow
//--------------------------------------------------------------------------

const int TreeEditId = 102;

class _TYPEDECLASSE NSTreeWindow : public TTreeWindow, public NSRoot
{
   public:

      NSTreeNodeArray* 	pNodeArray;
      NSControle* 	  	pControle;
      NSEditDico*	  	  	pEditDico;
      NSTreeNode* 	  	pNSTreeNodeEnCours;
      TWindow* 	     	Aparent;
      ChampEdition* 		pChampEdition;
      BBItem*          	pBBitemNSTreeWindow; //BBItem créateur de la NSTreeWindow
      TImageList*  		Images;
      TTwHitTestInfo*   pHitTestInfo;

      //constructeurs  Destructeur
   	NSTreeWindow(TWindow* parent, NSSuper*	pSuper, int id, int x, int y, int w, int h,
                	 TStyle style = twsNone, TModule* module = 0);
		NSTreeWindow(TWindow* parent, NSSuper*	pSuper, int resourceId = 0, TModule* module = 0);
      ~NSTreeWindow();
      //copie
      NSTreeWindow(NSTreeWindow& src);
      // Surcharges des opérateurs
		NSTreeWindow& operator=(NSTreeWindow src);

      //méthodes
      NSTreeNode* GetNSTreeNode(TTreeNode* pTTreeNode); //à partir du hitem d'un TTreeNode on retourne le NSTreeNode
      														//correspondant

      NSTreeNode* GetNSTreeNode(HTREEITEM TTitem);
      void activeControle(int activation, Message* pMessage);
      void EvDelete();
		HWND EditLabel(HTREEITEM item);
      void EvLButtonDown(uint modKeys, TPoint& ptClick);
      bool TvnBeginLabelEdit(TTwDispInfoNotify& nmHdr);
      bool TvnItemExpanDing(TTwNotify& nmHdr);
   	void EvSetFocus(HWND hWndLostFocus);
      void EvKillFocus(HWND hWndGetFocus);
      void DicoGetTexteLibre(string* pContenu);
      void DicoGetCodeLexique();
      int  donneType() { return(isTreeWindow); }
      void SetupWindow();
      void SelChanged();
      void DispatcherPatPatho(NSPatPathoArray*	ContenuPatpatho);
      void RepartirPatPatho(TTreeNode* pNoeud, NSPatPathoArray*	ContenuPatpatho, NSTreeNode* pNSTreeNode,
											int* DecalageLigneFils);

      void EvTvKeyDown(TTwKeyDownNotify& nhmr);//touche clavier
      void Modifier();//modifier un noeud

      NSTreeNode* TrouverGrandFrere(NSTreeNode* pNSTreeNode, int ligne);
      NSTreeNode* TrouverPetitFrere(NSTreeNode* pNSTreeNode, int ligne);
      NSTreeNode* TrouverPetitFrereFilsRoot(NSTreeNode* pNSTreeNode, int ligne);
      NSTreeNode* TrouverGrandFrereFilsRoot(NSTreeNode* pNSTreeNode, int ligne);

      void MiseAjourNSTreeNode( NSTreeNode* pNSTreeNode/*, string codeTexteLibre*/); //MAJ de pNSTreeNode
      void ReMiseAjourNSTreeNode(NSTreeNode* pNSTreeNode,
                                 string pStringCode, string codeTexteLibre); //MAJ d'un nstreenode qui exsite
      void AjouteElementFictif(NSTreeNode* pNSTreeNode, TTreeNode* Noeud, bool CreerFilsChampEdit); //ajouter un NSTreeNode et un TTreeNode fictif
      void AjouteFilsFictif(NSTreeNode* pNSTreeNode, TTreeNode* pTTreeNode, int ligne, int colonne, string sChaineRecuperee,
                             string sIdentite, bool estPropose, bool CreerFilsChampEdit);
      void DetruitFils(int *cardinal, NSTreeNode* pNSTreeNode); //détruire des NSTreeNodes et des TTreeNodes fictifs
      void AjouteFrereFictif(NSTreeNode*  pNSTreeNode, TTreeNode* Noeud, int ligne , int colonne, string sFaconAjout = "Fin" );
      void GetMaxCoordonnees(NSTreeNode* pNSTreeNode, int *ligne, int *colonne); //donne les coordonnees maximales pour les fils
      void CreerBBItem(NSTreeNode* pNSTreeNode, NSTreeNode* pNSTreeNodePetitFrere/*, BBItemData* pDonnees*/);
      NSTreeNode* TrouverPere(int ligne, int colonne);
      void CreerFrere(NSTreeNode*  pNSTreeFrere, int ligne , int colonne);
      void CreerNSTreeNode(NSTreeNode* pNewNSTreeNode, int ligne, int colonne,
      							string sIdentite, string sLabel, string sCodeTexteLibre, string sTexteLibre);

      void AjouteFrereFictifTexteLibre(NSTreeNode* pNSTreeNode,
      					int ligne,	int colonne ,  string sTexteLibre);
   	void RepartirTexteLibre(NSTreeNode* pNSTreeNode, VecteurChaine* VecteurTexte, string* pTexteLibre, int* tailleVecteur);
      void NStreeNodeSuivant();
      void NStreeNodePrcedent();
      void InsereAvant();
      void InsereApres();
      void DecomposeChaine(string* pChaine, VecteurChaine* pVect );
      void EvRButtonDown(uint modkeys, TPoint& point);
      void ReconstruireFrere(NSTreeNode* pNSTreeNode);
      void ReconstruireFils(NSTreeNode* pNSTreeNode);
      void Absence();
      void Presence();
      void Parametres();
      void RecupereValeurChiffree(NSTreeNode* pNSTreeNode);
      void DecomposeChaine(VecteurChaine* pVect, string sChaine);
      void EvLButtonDblClk(uint, TPoint&);
      void developper(NSTreeNode* pNSTreeNode , TTreeNode* Noeud, int* DecalageLigneFils,
							int ligne, int colonne, bool CreerFilsChampEdit, bool bRechercheFilGuide = false);
      bool RechercheFilGuide(NSTreeNode* pNSTreeNode, BBItemData* pDonnees, BBFilsItem* pBBfils);               
      void UpdateNewNStreeNode(NSTreeNode* pNewNSTreeNode, int ligneFils, int colonneFils,
                    NSTreeNode* pNSTreeNodePere, NSPatPathoData* pDonnees );
      void SupprimerFils(NSTreeNode* pNSTreeNode, int* NbNoeudSupprime, bool* PatpthoNonVide);              


		DECLARE_RESPONSE_TABLE(NSTreeWindow);
};

#endif // __NSTRNODE_H
