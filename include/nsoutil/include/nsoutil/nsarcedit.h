#if !defined(NSARCEDIT_H)#define NSARCEDIT_H

#include <owl\edit.h>
#include "nsbb\nsednum.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsarc.h"#include "nsoutil\nssavary.h"
#include "nsoutil\nsexport.h"

class NSArcEditTree : public NSTreeWindow{
    public:

        NSArcEditTree(TWindow* parent, NSContexte* pCtx, int id, int x, int y,
                     int w, int h, TStyle style = twsNone, TModule* module = 0);
		NSArcEditTree(TWindow* parent, NSContexte* pCtx, int resourceId = 0,
                     TModule* module = 0);
        ~NSArcEditTree();

        void    ammorcerTree(BBItem* pItem, int* piLigne, int iColonne, NSTreeNode* pFatherNode);
        void    setNodeLabel(NSTreeNode* pNode);
        string  getNodePath(NSTreeNode* pNode);
        void    metAjour(NSTreeNode* pNSTreeNode);

        void    MiseAjourNSTreeNode(NSTreeNode* pNSTreeNode);
        void    ReMiseAjourNSTreeNode(NSTreeNode* pNSTreeNode,
                                        string pStringCode, string sComplement);
        void    CreerBBItem(NSTreeNode* pNSTreeNode, NSTreeNode* pNSTreeNodePetitFrere);

        void    Absence();
        void    Presence();
        void    Parametres();
        void    FixeCertitude(int iCert);

        void    EvDelete();
        void    EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point);
        void    CmEditContrainte();
        void    CmEditControle();

    DECLARE_RESPONSE_TABLE(NSArcEditTree);
};

//// Classe NSRefEditeur : Editeur de référentiels//
class _NSOUTILCLASSE NSArcEditeur : public NSUtilDialog
{
    public:

        OWL::TEdit*         pArcName;
        string              sFileName;
        NSArcEditTree* 		pNSTreeWindow;
        NSPatPathoArray* 	pPatPathoArray;
        NSSmallBrother* 	pBigBoss;
        bool                bIsModif;

        NSArcEditeur(TWindow* pere, NSContexte* pCtx);
        ~NSArcEditeur();

        void SetupWindow();
        void initArchetype();
        void CmArchetype();
        void CmSauvegarde();
        void CmNouveau();
        void CmOk();
        void CmCancel();

    DECLARE_RESPONSE_TABLE(NSArcEditeur);
};

class NSContListDialog : public NSUtilDialog
{
    public:

        OWL::TEdit*         pLabel;
        OWL::TGroupBox*     pOblig;
        OWL::TRadioButton*  pObligOui;
        OWL::TRadioButton*  pObligNon;
        NSEditNum*          pMin;
        NSEditNum*          pMax;

        Citem*              pCitemPere;
        string              sLabel;
        bool                bOblig;
        bool                bIsNeeded;
        string              sFilsMin;
        string              sFilsMax;
        bool                bIsExist;

        NSContListDialog(TWindow* pere, string sLabelNode, Citem* pCitemNode, NSContexte* pCtx);
        ~NSContListDialog();

        void SetupWindow();
        void CmOk();
        void CmCancel();

    DECLARE_RESPONSE_TABLE(NSContListDialog);
};

class NSControlTextDialog : public NSUtilDialog
{
    public:

        OWL::TEdit*         pLabel;
        OWL::TGroupBox*     pActif;
        OWL::TRadioButton*  pActifOui;
        OWL::TRadioButton*  pActifNon;
        OWL::TEdit*         pTexte;

        Citem*              pCitem;
        string              sLabel;
        bool                bActif;
        string              sTexte;

        NSControlTextDialog(TWindow* pere, string sLabelNode, Citem* pCitemNode, NSContexte* pCtx);
        ~NSControlTextDialog();

        void SetupWindow();
        void CmOk();
        void CmCancel();

    DECLARE_RESPONSE_TABLE(NSControlTextDialog);
};

#endif

