#include <dir.h>#include <string.h>
#include <cstring.h>
#include <stdio.h>

#include "nsoutil\nsarcedit.h"#include "nssavoir\ns_fic.h"
#include "partage\nsdivfct.h"
#include "nautilus\nautilus.rh"
#include "nsbb\nsbb.rh"
#include "nsbb\nstrnode.rh"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsoutil.rh"
#include "nsbb\nsbbitem.h"

//***************************************************************************
//
//  Méthodes de NSArcEditeur
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSArcEditeur, NSUtilDialog)
    EV_COMMAND(IDC_AE_OPEN, CmArchetype),
    EV_COMMAND(IDC_AE_SAVE, CmSauvegarde),
    EV_COMMAND(IDC_AE_NEW, CmNouveau),
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSArcEditeur::NSArcEditeur(TWindow* pere, NSContexte* pCtx)
             :NSUtilDialog(pere, pCtx, "IDD_ARCEDIT", pNSResModule)
{
try
{
    pArcName = new TEdit(this, IDC_AE_ARCNAME);
    bIsModif = false;
    sFileName = "";
    pPatPathoArray = new NSPatPathoArray(pContexte);
    pBigBoss = new NSSmallBrother(pContexte, pPatPathoArray);
    pNSTreeWindow = new NSArcEditTree(this, pCtx, IDC_AE_TVIEW, pNSResModule);
    initArchetype();
}
catch (const exception& e)
{
	string sExept = "Exception NSArcEditeur ctor " + string(e.what());
	erreur(sExept.c_str(), standardError, 0);
}
catch (...)
{
	erreur("Exception NSArcEditeur ctor.", standardError, 0);
}
}

NSArcEditeur::~NSArcEditeur()
{
    delete pArcName;
    delete pNSTreeWindow;
    delete pBigBoss;
    delete pPatPathoArray;
}

void
NSArcEditeur::SetupWindow()
{
    NSUtilDialog::SetupWindow();
}

void
NSArcEditeur::initArchetype()
{
    pBigBoss->pBBItem = new BBItem(pBigBoss);
    BBItemData* pDo = pBigBoss->pBBItem->pDonnees;
    //strcpy(pDo->nomDialogue, "TEXTELIB");
    // indispensable dans activer :
    strcpy(pDo->ouvreDialogue, "A");
    strcpy(pDo->fils, "");
    strcpy(pDo->decalageNiveau, "+00+00");
    //strcpy(pDo->fichierDialogue, "NSBB");
    pNSTreeWindow->pBBitemNSTreeWindow = pBigBoss->pBBItem;
    pBigBoss->pBBItem->pParseur = new nsarcParseur(pContexte);
    pBigBoss->pBBItem->pParseur->pArchetype = new Carchetype("", "<items></items>", NULL, pContexte);
    pBigBoss->pBBItem->pParseur->pArchetype->parser();
}

void
NSArcEditeur::CmArchetype()
{

    int     pos;
    ValIter ival;

	if (bIsModif)
	{
  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    int idRet = MessageBox("Attention un Archétype est déjà ouvert et a été modifié. Voulez-vous le sauvegarder ?", sCaption.c_str(), MB_YESNO) ;
    if (idRet == IDYES)
    	CmSauvegarde() ;
	}

	char path[1024] ;
	strcpy(path, (pContexte->PathName("IXML")).c_str()) ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,                                  "Tous fichiers (*.XML)|*.xml|",
                                  0, path, "XML") ;

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;
	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;
	string sFileName = string(initData.FileName) ;
	//
	// Remise à zéro
	//
	if (pBigBoss->pBBItem)
	{
		delete pBigBoss->pBBItem ;
		pBigBoss->pBBItem = NULL ;
	}

	pBigBoss->pBBItem = new BBItem(pBigBoss) ;
	BBItemData* pDo = pBigBoss->pBBItem->pDonnees ;
	//strcpy(pDo->nomDialogue, "TEXTELIB");
	// indispensable dans activer :
	strcpy(pDo->ouvreDialogue, "A") ;
	strcpy(pDo->fils, sFileName.c_str()) ;
	strcpy(pDo->decalageNiveau, "+00+00") ;
	//strcpy(pDo->fichierDialogue, "NSBB");

	pBigBoss->pBBItem->pParseur = new nsarcParseur(pContexte) ;

	if (pBigBoss->pBBItem->pParseur->open(sFileName))
	{
  	// on développe tout l'archetype à partir du BBItem root
    pBigBoss->pBBItem->creerArchetype(pBigBoss->pBBItem->pParseur->pArchetype->getRootItem(), false) ;

    // on renseigne les champs définissant le dialogue
    // (ces champs seront utilisés par BBItem::activer dans creerDialogue()
    //  qui ne sera appelée que pour le BBItem Root)
    if (pBigBoss->pBBItem->pParseur->pArchetype->getDialog() != NULL)
    {
    	string sNomDialog = pBigBoss->pBBItem->pParseur->pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_NOM) ;
      strcpy(pBigBoss->pBBItem->szNomDlg, sNomDialog.c_str()) ;
      string sNomDll = pBigBoss->pBBItem->pParseur->pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_DLL) ;
      strcpy(pBigBoss->pBBItem->pDonnees->fichierDialogue, sNomDll.c_str()) ;
    }

    pNSTreeWindow->pBBitemNSTreeWindow = pBigBoss->pBBItem ;

    int iLigne = 0 ;
    pNSTreeWindow->ammorcerTree(pBigBoss->pBBItem, &iLigne, 0, NULL) ;
	}
	else
		return ;

	pArcName->SetText(sFileName.c_str()) ;
}

void
NSArcEditeur::CmSauvegarde()
{
    if ((!pBigBoss->pBBItem->pParseur) || (!(pBigBoss->pBBItem->pParseur->pArchetype)))
        return;
    Carchetype* pArchetype = pBigBoss->pBBItem->pParseur->pArchetype;

    //
    // Nouveau fichier
    //
    if (sFileName == "")
    {
        char    path[256];
        strcpy(path,(pContexte->PathName("IXML")).c_str());
        TOpenSaveDialog::TData initData(OFN_PATHMUSTEXIST,
   											"Tous fichiers (*.XML)|*.xml|",
                                            0, path, "XML");

        TFileSaveDialog* pSaveDlg = new TFileSaveDialog(pContexte->GetMainWindow(), initData);
        if (pSaveDlg->Execute() != IDOK)        {
            delete pSaveDlg;
            return;
        }

        sFileName = string(initData.FileName);
        pArcName->SetText(sFileName.c_str());
        delete pSaveDlg;
    }

    if (sFileName == "")
        return;

    ofstream outFile;
	outFile.open(sFileName.c_str());
    if (!outFile)
    {
        erreur("Impossible de créer le fichier référentiel à sauvegarder.", standardError, 0);
        return;
    }

    outFile << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" ?>" << '\n';

    pArchetype->inscritFichier(&outFile, 0, "archetype");

	outFile.close();

    bIsModif = false;
}

void
NSArcEditeur::CmNouveau()
{
}

void
NSArcEditeur::CmOk()
{
    if (bIsModif)
    {
        string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
        int idRet = MessageBox("L'Archétype a été modifié. Voulez-vous le sauvegarder ?", sCaption.c_str(), MB_YESNO);
        if (idRet == IDYES)
            CmSauvegarde();
    }

    TDialog::CmOk();
}

void
NSArcEditeur::CmCancel()
{
    TDialog::CmCancel();
}

//***************************************************************************
//
//  Méthodes de NSArcEditTree
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSArcEditTree, NSTreeWindow)
    EV_WM_RBUTTONDOWN,
    EV_COMMAND(IDC_CONTRAINTE,  CmEditContrainte),
    EV_COMMAND(IDC_CONTROLE,    CmEditControle),
END_RESPONSE_TABLE;

NSArcEditTree::NSArcEditTree(TWindow* parent,NSContexte* pCtx,
                            int id, int x, int y, int w, int h,
                            TStyle style, TModule* module)
              :NSTreeWindow(parent, pCtx, id, x, y, w, h, style, module)
{
}

NSArcEditTree::NSArcEditTree(TWindow* parent, NSContexte* pCtx, int resourceId,
                           TModule* module)
              :NSTreeWindow(parent, pCtx, resourceId, module)
{
}

NSArcEditTree::~NSArcEditTree()
{
}

void
NSArcEditTree::ammorcerTree(BBItem* pItem, int* piLigne, int iColonne, NSTreeNode* pFatherNode)
{
    if ((!pItem) || (pItem->aBBItemFils.empty()))
   	    return;

    BBiter FilsIt = pItem->aBBItemFils.begin();
    for ( ; FilsIt != pItem->aBBItemFils.end(); FilsIt++)
    {
        if ((*FilsIt)->pCitem)
        {
            string sEtiquette   = "";
            string sCertitude   = "";
            string sPluriel     = "";
            string sComplement  = "";
            string sFormat      = "";

            //
            // Parsing de la chaine qui représente le noeud
            // Parsing of the string that describe the node
            //
            string sCode = (*FilsIt)->pCitem->getCode();
            string sBloc = "";
            size_t pos = sCode.find('/');
            if (pos == NPOS)
                sEtiquette = sCode;
            else
            {
                sEtiquette  = string(sCode, 0, pos);
                if (strlen(sCode.c_str()) > pos + 1)
                    sCode   = string(sCode, pos+1, strlen(sCode.c_str()) - pos -1);

                while (sCode != "")
                {
                    if (sCode[0] == '$')
                    {
                        if ((strlen(sCode.c_str()) > 1) && (strlen(sCode.c_str()) <= 16))
                            sComplement = sCode;
                        else
                            sComplement = string(sCode, 1, 15);
                        sCode = "";
                    }
                    else
                    {
                        pos = sCode.find('/');
                        if (pos == NPOS)
                        {
                            sBloc   = sCode;
                            sCode   = "";
                        }
                        else
                        {
                            sBloc   = string(sCode, 0, pos);
                            if (strlen(sCode.c_str()) > pos + 1)
                                sCode = string(sCode, pos+1, strlen(sCode.c_str()) - pos -1);
                        }
                        if (strlen(sBloc.c_str()) == BASE_LEXIQUE_LEN)
                        {
                            if      (string(sBloc, 0, 3) == string("WCE"))
                                sCertitude  = sBloc;
                            else if (string(sBloc, 0, 3) == string("WPL"))
                                sPluriel    = sBloc;
                            else if (string(sBloc, 0, 1) == string("£"))
                                sFormat     = sBloc;
                        }
                    }
                }
            }
            //
            // Création du noeud
            // Creating the node
            //
            TTreeNode* pAmorce;

            if (pItem == pBBitemNSTreeWindow)
            {
                TTreeNode root = GetRoot();
                pAmorce = new TTreeNode(*this, TVI_FIRST);
   		        *pAmorce = root.AddChild(*pAmorce);
            }
            else
            {
                pAmorce = new TTreeNode(*this, "");
                if (pFatherNode->VectFrereLie.empty()) //pas de frères liés
                    *pAmorce = pFatherNode->InsertChild(*pAmorce, TTreeNode::Last);
                else
                {
                    NSTreeNode*	pNSTreeFrere = pFatherNode->VectFrereLie.back(); //dérnier élément de VectFrereLie crée son frère
		            *pAmorce = pNSTreeFrere->InsertChild(*pAmorce, TTreeNode::Last);
                }
            }

      	    NSTreeNode* pNewNSTreeNode = new NSTreeNode(*pAmorce, pContexte);
            (*piLigne)++;
      	    pNewNSTreeNode->setLigne(*piLigne);
     	 	pNewNSTreeNode->setColonne(iColonne);

      	    pNewNSTreeNode->pere = pFatherNode;
      	    pNewNSTreeNode->FrerePilote = 0;

            pNewNSTreeNode->SetLexique(sEtiquette);
	        pNewNSTreeNode->SetCertitude(sCertitude);
	        pNewNSTreeNode->SetComplement(sComplement);
	        pNewNSTreeNode->SetPluriel(sPluriel);

            //
            // Connection du noeud à son BBFilsItem
            // Connecting the node to its BBFilsItem
            //

            // Création du contrôle
            // Creating the control
            pNewNSTreeNode->pControle = new NSControle();
            // Connection du contrôle au transfert du BBFilsItem
            // Connecting the control to the BBFilsItem's transfert structure
            pNewNSTreeNode->pControle->setTransfert((*FilsIt)->pTransfert);
		    (*FilsIt)->pTransfert->referenceControle(pNewNSTreeNode->pControle);
            // Connection du contrôle au noeud
            // Connecting the control to the node
            pNewNSTreeNode->pControle->setControle(dynamic_cast<void*>(pNewNSTreeNode));
            pNewNSTreeNode->pControle->setType(pNewNSTreeNode->donneType());
            pNewNSTreeNode->pControle->setNSDialog(pBBitemNSTreeWindow->pNSDialog);
            pNewNSTreeNode->pContexte = pContexte;
      	    pNewNSTreeNode->estPropose = false;
   		    //pNewNSTreeNode->SetImageIndex(6);
      	    //pNewNSTreeNode->SetSelectedImageIndex(6,  true);

            setNodeLabel(pNewNSTreeNode);
      	    pNewNSTreeNode->SetText(pNewNSTreeNode->sLabel.c_str(), true);

      	    pNodeArray->push_back(pNewNSTreeNode);

            //
            // Lancement itératif sur les fils
            //
            if (!((*FilsIt)->VectorFils.empty()))
            {
                BBiterFils iter = (*FilsIt)->VectorFils.begin();
                for ( ; iter != (*FilsIt)->VectorFils.end(); iter++)                    ammorcerTree(*iter, piLigne, iColonne+1, pNewNSTreeNode);
            }
        }
    }
}

void
NSArcEditTree::setNodeLabel(NSTreeNode* pNode)
{
    string sEtiquette   = pNode->getEtiquette();
    string sPluriel     = pNode->getPluriel();
    string sCertitude   = pNode->getCertitude();
    string sComplement  = pNode->sContenuTransfert;

    string sLibelleTrouve;
    pContexte->getSuperviseur()->getDico()->donneLibelle(sLang, &sEtiquette, &sLibelleTrouve);

    if (sCertitude != "")
    {
        if      (string(sCertitude, 0, 5) == "WCEA0")
            sLibelleTrouve += string(" (oui)");
        else if (string(sCertitude, 0, 5) == "WCE75")
            sLibelleTrouve += string(" (probable)");
        else if (string(sCertitude, 0, 5) == "WCE50")
            sLibelleTrouve += string(" (possible)");
        else if (string(sCertitude, 0, 5) == "WCE25")
            sLibelleTrouve += string(" (improbable)");
        else if (string(sCertitude, 0, 5) == "WCE00")
            sLibelleTrouve += string(" (non)");
        else if (string(sCertitude, 0, 5) == "WCEZZ")
            sLibelleTrouve += string(" (inconnu)");
    }

    //
    // Contraintes
    //
    bool    bOblig      = false;
    string  sFilsMin    = "";
    string  sFilsMax    = "";

    Citem* pCitem = 0;
    if ((pNode->pControle) && (pNode->pControle->getTransfert()) &&
                            (pNode->pControle->getTransfert()->pBBFilsItem))
    pCitem = pNode->pControle->getTransfert()->pBBFilsItem->pCitem;
    if (pCitem)
    {
        for (ValIter ival = pCitem->vect_val.begin(); ival != pCitem->vect_val.end(); ival++)
        {
            if ((*ival)->sLabel == LABEL_CONTRAINTE)
            {                // on initialise les valeurs de la contrainte trouvée                // si elle est de type "exist" ou "needed"                Ccontrainte* pCcont = dynamic_cast<Ccontrainte*>((*ival)->pObject);
                string sTypeCont = pCcont->getStringAttribute(ATTRIBUT_CONTR_TYPE);

                if (sTypeCont == string(VAL_ATTR_CONTR_TYPE_NEEDED))
                {
                    bOblig = true;
                }
                else if (sTypeCont == string(VAL_ATTR_CONTR_TYPE_EXIST))
                {
                    sFilsMin = pCcont->getStringAttribute(ATTRIBUT_CONTR_MIN);
                    sFilsMax = pCcont->getStringAttribute(ATTRIBUT_CONTR_MAX);
                }
            }
        }
    }
    if ((bOblig) || (sFilsMin != "") || (sFilsMax != ""))
    {
        string sContrainte = "";
        if (bOblig)
            sContrainte = "++++";
        if (sFilsMin != "")
        {
            if (sContrainte != "")
                sContrainte += "/";
            sContrainte += string("++") + sFilsMin + "|";
        }

        sLibelleTrouve = sContrainte + string(" ") + sLibelleTrouve;
    }

    //
    // Controle
    //
    if (pCitem)
    {
        string sTexte = pCitem->getText();
        string sControl = pCitem->getControl();
        if ((sTexte != "") || (sControl != ""))
        {
            string sTextCtrl = "";
            if (sControl == VAL_ATTR_ITEM_CTRL_ACTIF)
                sTextCtrl = "Actif";
            if (sTexte != "")
            {
                if (sTextCtrl != "")
                    sTextCtrl += " -> ";
                sTextCtrl += sTexte;
            }
            if (sTextCtrl != "")
                sLibelleTrouve += string(" >> ") + sTextCtrl;
        }
    }

    pNode->sLabel = sLibelleTrouve;
    pNode->SetText(sLibelleTrouve.c_str());
}

void
NSArcEditTree::metAjour(NSTreeNode* pNSTreeNode)
{
    setNodeLabel(pNSTreeNode);

    string sNodePath = pNSTreeNode->getEtiquette();

    if (pNSTreeNode->getPluriel() != "")
        sNodePath += string("/") + pNSTreeNode->getPluriel();
    if (pNSTreeNode->getCertitude() != "")
        sNodePath += string("/") + pNSTreeNode->getCertitude();
    if (pNSTreeNode->sContenuTransfert != "")
        sNodePath += string("/$") + pNSTreeNode->sContenuTransfert;

    Citem* pChgCitem = pNSTreeNode->pControle->getTransfert()->pBBFilsItem->pCitem;
    pChgCitem->setCode(sNodePath);

    // Mettre à jour l'indicateur bIsModif
    NSArcEditeur* pArcEdit = dynamic_cast<NSArcEditeur*>(Parent);
    if (pArcEdit)
        pArcEdit->bIsModif = true;
}

void
NSArcEditTree::MiseAjourNSTreeNode(NSTreeNode* pNSTreeNode)
{
    if (pNSTreeNode->sIdentite == "")
   	    return;

try
{
    pNSTreeNode->pControle  = new NSControle(pBBitemNSTreeWindow , pNSTreeNode->sIdentite, "");
    pNSTreeNode->pControle->setControle(dynamic_cast<void*>(pNSTreeNode));
    pNSTreeNode->pControle->setType(pNSTreeNode->donneType());
    pNSTreeNode->pControle->setNSDialog(pBBitemNSTreeWindow->pNSDialog);
    pNSTreeNode->pContexte = pContexte;

    int colonne = pNSTreeNode->getColonne();
    int ligne = pNSTreeNode->getLigne();
    //retrouver pNSTreeNode dans pNodeArray
    iterNSTreeNode EnCours = pNodeArray->begin(); // pNSTreeNode

    for (; (EnCours != pNodeArray->end()) && ((*EnCours) != pNSTreeNode); EnCours++);

    if (EnCours != pNodeArray->end())
    {
        NSTreeNode* pNSTreePere = pNSTreeNode->pere;
        if (pNSTreePere)
        {
      	    NSTreeNode* pNSTreePetitFrere = TrouverPetitFrere(pNSTreeNode, ligne);

            if (pNSTreePetitFrere)
         	    CreerBBItem(pNSTreeNode, pNSTreePetitFrere);
            else
                CreerBBItem(pNSTreeNode, 0);
        }
        else
        {
            //chercher le 1er NSTreeNode de colonne = 1 et après pNSTreeNode
            NSTreeNode* pNSTreePetitFrere = TrouverPetitFrereFilsRoot(pNSTreeNode, ligne);
            if (pNSTreePetitFrere)
         	    CreerBBItem(pNSTreeNode, pNSTreePetitFrere);
            else
	            CreerBBItem(pNSTreeNode, 0);
        }
    }
}
catch (...)
{
	erreur("Exception NSTreeWindow::MiseAjourNSTreeNode.", standardError, 0);
}
}

void
NSArcEditTree::ReMiseAjourNSTreeNode(NSTreeNode* pNSTreeNode,
                                 string pStringCode, string sComplement)
{
    NSTreeWindow::ReMiseAjourNSTreeNode(pNSTreeNode, pStringCode, sComplement);
    metAjour(pNSTreeNode);
}

void
NSArcEditTree::CreerBBItem(NSTreeNode* pNSTreeNode, NSTreeNode* pNSTreeNodePetitFrere)
{
    ValIter ival;
    Citem* pCitemPere;

    if (pNSTreeNode->pere == 0)
    {
        // on crée à la racine (dans BBItem root)
        if (pNSTreeNodePetitFrere)
            pBBitemNSTreeWindow->CreerFilsManuel(pNSTreeNode->pControle, pNSTreeNodePetitFrere->pControle);
        else
            pBBitemNSTreeWindow->CreerFilsManuel(pNSTreeNode->pControle, 0);
    }
    else
        NSTreeWindow::CreerBBItem(pNSTreeNode, pNSTreeNodePetitFrere);

    BBFilsItem* pNewFils    = pNSTreeNode->pControle->getTransfert()->pBBFilsItem;
    BBFilsItem* pPereFils   = pNewFils->pPere->pBBFilsPere;

    // Si le pere (le BBItem Root) n'a aucun pBBFilsPere
    if (pPereFils == NULL)
    {
        // on initialise le pCitemPere avec l'item root de l'archetype (ITEMS)
        pCitemPere = pBBitemNSTreeWindow->pParseur->pArchetype->getRootItem();
    }
    else // sinon on prend le pCitem du pBBFilsPere
        pCitemPere = pPereFils->pCitem;

    string sNodePath = pNSTreeNode->getEtiquette();

    if (pNSTreeNode->getPluriel() != "")
        sNodePath += string("/") + pNSTreeNode->getPluriel();
    if (pNSTreeNode->getCertitude() != "")
        sNodePath += string("/") + pNSTreeNode->getCertitude();
    if (pNSTreeNode->sContenuTransfert != "")
        sNodePath += string("/$") + pNSTreeNode->sContenuTransfert;

    string sNodeLabel = string(ATTRIBUT_ITEM_CODE) + string(" = \"") + sNodePath + string("\"");

    CValeur* pValeur = new CValeur(LABEL_ITEM, "", sNodeLabel, pCitemPere);
    //
    // Il faut insérer au bon endroit au lieu de le faire à la fin
    //
    if ((pNSTreeNodePetitFrere) && (pNSTreeNodePetitFrere->pControle) &&
        (pNSTreeNodePetitFrere->pControle->getTransfert()) &&
        (pNSTreeNodePetitFrere->pControle->getTransfert()->pBBFilsItem) &&
        (pNSTreeNodePetitFrere->pControle->getTransfert()->pBBFilsItem->pCitem))
    {
        Citem* pCitemPetitFrere = pNSTreeNodePetitFrere->pControle->getTransfert()->pBBFilsItem->pCitem;

        for (ival = pCitemPere->vect_val.begin(); ival != pCitemPere->vect_val.end(); ival++)
        {
            if ((*ival)->sLabel == LABEL_ITEM)
            {                // on doit insérer la valeur avant le petit frère                Citem* pCitem = dynamic_cast<Citem*>((*ival)->pObject);

                if (pCitem == pCitemPetitFrere)
                {
                    pCitemPere->vect_val.insert(ival, pValeur);
                    break;
                }
            }
        }
    }
    else
        pCitemPere->vect_val.push_back(pValeur);

    pValeur->pObject = new Citem(sNodeLabel, "", pCitemPere, pContexte);
    CAttributs* pAttr = new CAttributs(ATTRIBUT_ITEM_CODE, sNodePath);
    pValeur->pObject->vect_attr.push_back(pAttr);

    pNewFils->pCitem = dynamic_cast<Citem*>(pValeur->pObject);

    // Mettre à jour l'indicateur bIsModif
    NSArcEditeur* pArcEdit = dynamic_cast<NSArcEditeur*>(Parent);
    if (pArcEdit)
        pArcEdit->bIsModif = true;
}

void
NSArcEditTree::Absence()
{
    TTreeNode noeud = GetSelection();
    NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud);
    if (!pNSTreeNode)
   	    return;
    NSTreeWindow::Absence();
    metAjour(pNSTreeNode);
}

void
NSArcEditTree::Presence()
{
    TTreeNode noeud = GetSelection();
    NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud);
    if (!pNSTreeNode)
   	    return;
    NSTreeWindow::Presence();
    metAjour(pNSTreeNode);
}

void
NSArcEditTree::Parametres()
{
    TTreeNode noeud = GetSelection();
    NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud);
    if (!pNSTreeNode)
   	    return;
    NSTreeWindow::Parametres();
    metAjour(pNSTreeNode);
}

void
NSArcEditTree::FixeCertitude(int iCert)
{
    TTreeNode noeud = GetSelection();
    NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud);
    if (!pNSTreeNode)
   	    return;
    NSTreeWindow::FixeCertitude(iCert);
    metAjour(pNSTreeNode);
}

void
NSArcEditTree::EvDelete()
{
    bool bPropose = false;

    TTreeNode NoeudSuppr = GetSelection();  				   //TTreeNode sélectionné
    NSTreeNode* pNSTreeNodeSuppr = GetNSTreeNode(&NoeudSuppr); //NSTreeNode correspondant

    if (!(pNSTreeNodeSuppr->estPropose))
    {
        Citem* pSupprCitem = pNSTreeNodeSuppr->pControle->getTransfert()->pBBFilsItem->pCitem;

        BBFilsItem* pSupprFils  = pNSTreeNodeSuppr->pControle->getTransfert()->pBBFilsItem;
        BBFilsItem* pPereFils   = pSupprFils->pPere->pBBFilsPere;

        Citem* pCitemPere;
        // Si le pere (le BBItem Root) n'a aucun pBBFilsPere
        if (pPereFils == NULL)
            // on initialise le pCitemPere avec l'item root de l'archetype (ITEMS)
            pCitemPere = pBBitemNSTreeWindow->pParseur->pArchetype->getRootItem();
        else // sinon on prend le pCitem du pBBFilsPere
            pCitemPere = pPereFils->pCitem;

        for (ValIter ival = pCitemPere->vect_val.begin(); ival != pCitemPere->vect_val.end(); ival++)
        {
            if ((*ival)->sLabel == LABEL_ITEM)
            {                // on doit insérer la valeur avant le petit frère                Citem* pCitem = dynamic_cast<Citem*>((*ival)->pObject);

                if (pCitem == pSupprCitem)
                {
                    delete (*ival);
                    pCitemPere->vect_val.erase(ival);
                    break;
                }
            }
        }
        // Mettre à jour l'indicateur bIsModif
        NSArcEditeur* pArcEdit = dynamic_cast<NSArcEditeur*>(Parent);
        if (pArcEdit)
            pArcEdit->bIsModif = true;
    }
    NSTreeWindow::EvDelete();
}

//-----------------------------------------------------------------------
// click droit de la souris sur un noeud
//-----------------------------------------------------------------------
void
NSArcEditTree::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
//
    // Si on est en cours d'édition, et que le texte n'est pas vide, on ne
    // fait rien
    //
    if (bEditionDico)
    {        // on teste dans ce cas si le texte est vide
        // (cas où on ne doit pas empecher le click)
        int len = pEditDico->GetTextLen();
        char far* texte = new char[len + 1];
        pEditDico->GetText(texte, len + 1);
        string sTexte = string(texte);
        delete[] texte;

        if (sTexte != "")
        	return;

        SortieEditionDico();
    }

    NSSuper* pSuper = pContexte->getSuperviseur();
    NS_CLASSLIB::TPoint lp = point;
  	pHitTestInfo->pt = lp;

  	NSTreeNode* pNSTreeNodeSelection = 0;
  	pNSTreeNodeSelection = GetNSTreeNode(HitTest(pHitTestInfo));
  	if (!pNSTreeNodeSelection)
  		return;

    TPopupMenu *menu = new TPopupMenu();

  	pNSTreeNodeSelection->SelectItem(TVGN_CARET);
  	if (pNSTreeNodeSelection->estLie()) //NSTreeNode représentant la suite d'un texte libre
  	{
        menu->AppendMenu(MF_GRAYED, 0, "Insérer Avant	Inser");
  		menu->AppendMenu(MF_GRAYED, 0, "Insérer Après	Alt+Entrée");
        if (pNSTreeNodeSelection->sIdentite == "")
            menu->AppendMenu(MF_GRAYED, 0, "Modifier	Alt+M");
        else
      	    menu->AppendMenu(MF_STRING, IDC_MODIFIER, "Modifier	Alt+M");
        menu->AppendMenu(MF_GRAYED, 0, "Edition contrainte");
        menu->AppendMenu(MF_GRAYED, 0, "Edition contrôle");

  		menu->AppendMenu(MF_SEPARATOR, 0, 0);
        menu->AppendMenu(MF_GRAYED, 0, "Absence de ...  ");
        menu->AppendMenu(MF_GRAYED, 0, "Paramètres  ");
        menu->AppendMenu(MF_GRAYED, 0, "Fil guide  ");
        menu->AppendMenu(MF_GRAYED, 0, "Encyclopédie  ");

        menu->AppendMenu(MF_SEPARATOR, 0, 0);
  		menu->AppendMenu(MF_GRAYED, 0, "Supprimer	Suppr  ");
    }
  	else
  	{
        // Elément fictif ou texte libre non proposé
        //
        if (pNSTreeNodeSelection->estFictif() &&
            (!pNSTreeNodeSelection->estPropose))
        {
            menu->AppendMenu(MF_GRAYED, 0, "Insérer Avant	Inser");
  			menu->AppendMenu(MF_GRAYED, 0, "Insérer Après	Alt+Entrée");
  			menu->AppendMenu(MF_GRAYED, 0, "Modifier	Alt+M");
            menu->AppendMenu(MF_GRAYED, 0, "Edition contrainte");
            menu->AppendMenu(MF_GRAYED, 0, "Edition contrôle");

  			menu->AppendMenu(MF_SEPARATOR, 0, 0);
      	    menu->AppendMenu(MF_GRAYED, 0, "Absence de ...  ");
      	    menu->AppendMenu(MF_GRAYED, 0, "Paramètres  ");
            menu->AppendMenu(MF_GRAYED, 0, "Fil guide  ");
            menu->AppendMenu(MF_GRAYED, 0, "Encyclopédie  ");

      	    menu->AppendMenu(MF_SEPARATOR, 0, 0);
  			menu->AppendMenu(MF_GRAYED ,0, "Supprimer	Suppr  ");
        }
        else if (pNSTreeNodeSelection->estPropose)
        {
      	    menu->AppendMenu(MF_GRAYED, 0, "Insérer Avant	Inser");
  			menu->AppendMenu(MF_GRAYED, 0, "Insérer Après	Alt+Entrée");
  			menu->AppendMenu(MF_GRAYED, 0, "Modifier	Alt+M");
            menu->AppendMenu(MF_GRAYED, 0, "Edition contrainte");
            menu->AppendMenu(MF_GRAYED, 0, "Edition contrôle");

  			menu->AppendMenu(MF_SEPARATOR, 0, 0);
      	    menu->AppendMenu(MF_GRAYED, 0, "Absence de ...  ");
      	    menu->AppendMenu(MF_GRAYED, 0, "Paramètres  ");
            menu->AppendMenu(MF_GRAYED, 0, "Fil guide  ");
            menu->AppendMenu(MF_GRAYED, 0, "Encyclopédie  ");

      	    menu->AppendMenu(MF_SEPARATOR, 0, 0);
  			menu->AppendMenu(MF_STRING, IDC_DELETE, "Supprimer	Suppr  ");
        }
        else if (!(pNSTreeNodeSelection->sTexteGlobal == string(""))) //frère pilote
        {
	  		menu->AppendMenu(MF_STRING, IDC_INSERTFRERE, "Insérer Avant	Inser");
  			menu->AppendMenu(MF_STRING, IDC_INSERTELEMENTFICTIF, "Insérer Après	Alt+Entrée");
  			menu->AppendMenu(MF_STRING, IDC_MODIFIER, "Modifier	Alt+M");
            menu->AppendMenu(MF_STRING, IDC_CONTRAINTE, "Edition contrainte");
            menu->AppendMenu(MF_STRING, IDC_CONTROLE, "Edition contrôle");

  			menu->AppendMenu(MF_SEPARATOR, 0, 0);
      	    menu->AppendMenu(MF_GRAYED, 0, "Absence de ...  ");
      	    menu->AppendMenu(MF_GRAYED, 0, "Paramètres  ");
            menu->AppendMenu(MF_GRAYED, 0, "Fil guide  ");
            menu->AppendMenu(MF_GRAYED, 0, "Encyclopédie  ");

      	    menu->AppendMenu(MF_SEPARATOR, 0, 0);
  			menu->AppendMenu(MF_STRING, IDC_DELETE, "Supprimer	Suppr  ");
            menu->AppendMenu(MF_STRING, IDC_CUT, "Couper	Ctrl+X  ");
            menu->AppendMenu(MF_STRING, IDC_COPY, "Copier	Ctrl+C  ");

            if (pSuper->pBufCopie->empty())
                menu->AppendMenu(MF_GRAYED, 0, "Coller	Ctrl+V  ");
            else
                menu->AppendMenu(MF_STRING, IDC_PASTE, "Coller	Ctrl+V  ");
        }
        else
        {
      	    menu->AppendMenu(MF_STRING, IDC_INSERTFRERE, "Insérer Avant	Inser");
  			menu->AppendMenu(MF_STRING, IDC_INSERTELEMENTFICTIF, "Insérer Après	Alt+Entrée");
  			menu->AppendMenu(MF_STRING, IDC_MODIFIER, "Modifier	Alt+M");
            menu->AppendMenu(MF_STRING, IDC_CONTRAINTE, "Edition contrainte");
            menu->AppendMenu(MF_STRING, IDC_CONTROLE, "Edition contrôle");

  			menu->AppendMenu(MF_SEPARATOR, 0, 0);

            string sCertitude = pNSTreeNodeSelection->Certitude();
            if ((sCertitude == "ABSENCE") || (sCertitude == "PRESENCE"))
            {
      		    if (pNSTreeNodeSelection->Absence)
      			    menu->AppendMenu(MF_STRING, IDC_PRESENCE, "Présence de ...  ");
      		    else
      			    menu->AppendMenu(MF_STRING, IDC_ABSENT, "Absence de ...  ");
            }
            else
         	    menu->AppendMenu(MF_STRING, IDC_ABSENT, "Absence de ...  ");

      	    menu->AppendMenu(MF_STRING, IDC_PARAMETRES, "Paramètres  ");
            menu->AppendMenu(MF_STRING, IDC_FILGUIDE, "Fil guide  ");

            if (pNSTreeNodeSelection->bEncyclop)
         	    menu->AppendMenu(MF_STRING, IDC_ENCYCLOP, "Encyclopédie  ");
            else
                menu->AppendMenu(MF_GRAYED, 0, "Encyclopédie  ");

      	    menu->AppendMenu(MF_SEPARATOR, 0, 0);  			menu->AppendMenu(MF_STRING, IDC_DELETE, "Supprimer	Suppr  ");            menu->AppendMenu(MF_STRING, IDC_CUT, "Couper	Ctrl+X  ");
            menu->AppendMenu(MF_STRING, IDC_COPY, "Copier	Ctrl+C  ");
            if (pSuper->pBufCopie->empty())
                menu->AppendMenu(MF_GRAYED, 0, "Coller	Ctrl+V  ");
            else
                menu->AppendMenu(MF_STRING, IDC_PASTE, "Coller	Ctrl+V  ");

            menu->AppendMenu(MF_SEPARATOR, 0, 0);
  			menu->AppendMenu(MF_STRING, IDC_CLASSIF, "Classifier	Ctrl+J  ");
        }
    }
  	ClientToScreen(lp);
  	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
  	delete menu;
}

void
NSArcEditTree::CmEditContrainte()
{
    TTreeNode NoeudSel = GetSelection();  				   //TTreeNode sélectionné
    NSTreeNode* pNSTreeNodeSel = GetNSTreeNode(&NoeudSel); //NSTreeNode correspondant

    if ((!pNSTreeNodeSel->pControle) || (!pNSTreeNodeSel->pControle->getTransfert()->pBBFilsItem->pCitem))
        return;

    Citem* pCitemNodeSel = pNSTreeNodeSel->pControle->getTransfert()->pBBFilsItem->pCitem;

    // Appel du dialogue d'édition des contraintes
    NSContListDialog* pEditContDlg = new NSContListDialog(this, pNSTreeNodeSel->sLabel, pCitemNodeSel, pContexte);
    if (pEditContDlg->Execute() == IDOK)
    {
        // 1er cas : cas des contraintes needed
        ////////////////////////////////////////

        // 1) Si la contrainte existait et qu'elle n'existe plus
        if ((pEditContDlg->bIsNeeded) && (!pEditContDlg->bOblig))
        {
            // on la supprime...
            for (ValIter ival = pCitemNodeSel->vect_val.begin(); ival != pCitemNodeSel->vect_val.end(); ival++)
            {
                if ((*ival)->sLabel == LABEL_CONTRAINTE)
                {                    // on initialise les valeurs de la contrainte trouvée                    // si elle est de type "exist" ou "needed"                    Ccontrainte* pCcont = dynamic_cast<Ccontrainte*>((*ival)->pObject);
                    string sTypeCont = pCcont->getStringAttribute(ATTRIBUT_CONTR_TYPE);

                    if (sTypeCont == string(VAL_ATTR_CONTR_TYPE_NEEDED))
                    {
                        delete (*ival);
                        pCitemNodeSel->vect_val.erase(ival);
                        break;
                    }
                }
            }
        }

        // 2) Si la contrainte n'existait pas mais qu'elle existe maintenant
        if ((!pEditContDlg->bIsNeeded) && (pEditContDlg->bOblig))
        {
            // on la rajoute...
            string sContLabel = string(ATTRIBUT_CONTR_TYPE) + string(" = \"") + string(VAL_ATTR_CONTR_TYPE_NEEDED) + string("\"");
            CValeur* pValeur = new CValeur(LABEL_CONTRAINTE, "", sContLabel, pCitemNodeSel);
            pCitemNodeSel->vect_val.push_back(pValeur);
            pValeur->pObject = new Ccontrainte(sContLabel, "", pCitemNodeSel);
            CAttributs* pAttr = new CAttributs(ATTRIBUT_CONTR_TYPE, VAL_ATTR_CONTR_TYPE_NEEDED);
            pValeur->pObject->vect_attr.push_back(pAttr);
        }

        // 2ème cas : cas des contraintes "exist"
        //////////////////////////////////////////

        // 1) Si la contrainte existait...
        if (pEditContDlg->bIsExist)
        {
            // on la supprime...
            for (ValIter ival = pCitemNodeSel->vect_val.begin(); ival != pCitemNodeSel->vect_val.end(); ival++)
            {
                if ((*ival)->sLabel == LABEL_CONTRAINTE)
                {                    // on initialise les valeurs de la contrainte trouvée                    // si elle est de type "exist" ou "needed"                    Ccontrainte* pCcont = dynamic_cast<Ccontrainte*>((*ival)->pObject);
                    string sTypeCont = pCcont->getStringAttribute(ATTRIBUT_CONTR_TYPE);

                    if (sTypeCont == string(VAL_ATTR_CONTR_TYPE_EXIST))
                    {
                        delete (*ival);
                        pCitemNodeSel->vect_val.erase(ival);
                        break;
                    }
                }
            }
        }

        // 2) Si la contrainte existe à présent...
        if ((pEditContDlg->sFilsMin != "") || (pEditContDlg->sFilsMax != ""))
        {
            // on la rajoute...
            string sContLabel = string(ATTRIBUT_CONTR_TYPE) + string(" = \"") + string(VAL_ATTR_CONTR_TYPE_EXIST) + string("\"");
            string sContMin, sContMax;

            if (pEditContDlg->sFilsMin != "")
            {
                sContMin = string(ATTRIBUT_CONTR_MIN) + string(" = \"") + pEditContDlg->sFilsMin + string("\"");
                sContLabel += string(" ") + sContMin;
            }

            if (pEditContDlg->sFilsMax != "")
            {
                sContMax = string(ATTRIBUT_CONTR_MAX) + string(" = \"") + pEditContDlg->sFilsMax + string("\"");
                sContLabel += string(" ") + sContMax;
            }

            CValeur* pValeur = new CValeur(LABEL_CONTRAINTE, "", sContLabel, pCitemNodeSel);
            pCitemNodeSel->vect_val.push_back(pValeur);
            pValeur->pObject = new Ccontrainte(sContLabel, "", pCitemNodeSel);

            CAttributs* pAttr = new CAttributs(ATTRIBUT_CONTR_TYPE, VAL_ATTR_CONTR_TYPE_EXIST);
            pValeur->pObject->vect_attr.push_back(pAttr);
            if (pEditContDlg->sFilsMin != "")
            {
                CAttributs* pAttrMin = new CAttributs(ATTRIBUT_CONTR_MIN, pEditContDlg->sFilsMin);
                pValeur->pObject->vect_attr.push_back(pAttrMin);
            }
            if (pEditContDlg->sFilsMax != "")
            {
                CAttributs* pAttrMax = new CAttributs(ATTRIBUT_CONTR_MAX, pEditContDlg->sFilsMax);
                pValeur->pObject->vect_attr.push_back(pAttrMax);
            }
        }
        // Mettre à jour l'indicateur bIsModif
        setNodeLabel(pNSTreeNodeSel);

        NSArcEditeur* pArcEdit = dynamic_cast<NSArcEditeur*>(Parent);
        if (pArcEdit)
            pArcEdit->bIsModif = true;
    }
}

void
NSArcEditTree::CmEditControle()
{
    TTreeNode NoeudSel = GetSelection();  				   //TTreeNode sélectionné
    NSTreeNode* pNSTreeNodeSel = GetNSTreeNode(&NoeudSel); //NSTreeNode correspondant

    if ((!pNSTreeNodeSel->pControle) || (!pNSTreeNodeSel->pControle->getTransfert()->pBBFilsItem->pCitem))
        return;

    Citem* pCitemNodeSel = pNSTreeNodeSel->pControle->getTransfert()->pBBFilsItem->pCitem;

    // Appel du dialogue d'édition des contraintes
    NSControlTextDialog* pEditContDlg = new NSControlTextDialog(this, pNSTreeNodeSel->sLabel, pCitemNodeSel, pContexte);
    if (pEditContDlg->Execute() == IDOK)
    {
        string sAncText = pCitemNodeSel->getText();
        string sAncCtrl = pCitemNodeSel->getControl();

        if (pEditContDlg->bActif)
            pCitemNodeSel->setControl(VAL_ATTR_ITEM_CTRL_ACTIF);
        else
            pCitemNodeSel->setControl(VAL_ATTR_ITEM_CTRL_NONE);

        pCitemNodeSel->setText(pEditContDlg->sTexte);

        // Mettre à jour l'indicateur bIsModif
        setNodeLabel(pNSTreeNodeSel);

        NSArcEditeur* pArcEdit = dynamic_cast<NSArcEditeur*>(Parent);
        if (pArcEdit)
            pArcEdit->bIsModif = true;
    }
}

//***************************************************************************
//
//  Méthodes de NSContListDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSContListDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSContListDialog::NSContListDialog(TWindow* pere, string sLabelNode, Citem* pCitemNode, NSContexte* pCtx)
             :NSUtilDialog(pere, pCtx, "IDD_CONTLIST", pNSResModule)
{
try
{
  pLabel    = new TEdit(this, IDC_CL_LABEL) ;
  pOblig    = new TGroupBox(this, IDC_CL_OBLIG) ;
  pObligOui = new TRadioButton(this, IDC_CL_OBLIG_OUI, pOblig) ;
  pObligNon = new TRadioButton(this, IDC_CL_OBLIG_NON, pOblig) ;
  pMin      = new NSEditNum(this, IDC_CL_FILS_MIN, 2, 0) ;
  pMax      = new NSEditNum(this, IDC_CL_FILS_MAX, 2, 0) ;

  sLabel     = sLabelNode ;
  pCitemPere = pCitemNode ;
  bOblig     = false ;
  bIsNeeded  = false ;
  sFilsMin   = "" ;
  sFilsMax   = "" ;
  bIsExist   = false ;
}
catch (...)
{
	erreur("Exception NSContListDialog ctor.", standardError, 0);
}
}

NSContListDialog::~NSContListDialog()
{
  delete pLabel ;
  delete pOblig ;
  delete pObligOui ;
  delete pObligNon ;
  delete pMin ;
  delete pMax ;
}

void
NSContListDialog::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  pLabel->SetText(sLabel.c_str()) ;

  for (ValIter ival = pCitemPere->vect_val.begin(); ival != pCitemPere->vect_val.end(); ival++)
  {
    if ((*ival)->sLabel == LABEL_CONTRAINTE)
    {      // on initialise les valeurs de la contrainte trouvée      // si elle est de type "exist" ou "needed"      Ccontrainte* pCcont = dynamic_cast<Ccontrainte*>((*ival)->pObject);
      string sTypeCont = pCcont->getStringAttribute(ATTRIBUT_CONTR_TYPE);

      if (sTypeCont == string(VAL_ATTR_CONTR_TYPE_NEEDED))
      {
        bOblig    = true ;
        bIsNeeded = true ;
      }
      else if (sTypeCont == string(VAL_ATTR_CONTR_TYPE_EXIST))
      {
        sFilsMin = pCcont->getStringAttribute(ATTRIBUT_CONTR_MIN);
        sFilsMax = pCcont->getStringAttribute(ATTRIBUT_CONTR_MAX);
        bIsExist = true;
      }
    }
  }

  if (bOblig)
  {
    pObligOui->Check();
    pObligNon->Uncheck();
  }
  else
  {
    pObligNon->Check();
    pObligOui->Uncheck();
  }

  if (sFilsMin != "")
    pMin->SetText(sFilsMin.c_str());

  if (sFilsMax != "")
    pMax->SetText(sFilsMax.c_str());
}

void
NSContListDialog::CmOk()
{
    char far szFilsMin[3];
    char far szFilsMax[3];

    if (pObligOui->GetCheck() == BF_CHECKED)
        bOblig = true;

    if (pObligNon->GetCheck() == BF_CHECKED)
        bOblig = false;

    pMin->GetText(szFilsMin, 3);
    sFilsMin = string(szFilsMin);

    pMax->GetText(szFilsMax, 3);
    sFilsMax = string(szFilsMax);

    NSUtilDialog::CmOk();
}

void
NSContListDialog::CmCancel()
{
    NSUtilDialog::CmCancel();
}

//***************************************************************************
//
//  Méthodes de NSControlTextDialog
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSControlTextDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSControlTextDialog::NSControlTextDialog(TWindow* pere, string sLabelNode, Citem* pCitemNode, NSContexte* pCtx)
                    :NSUtilDialog(pere, pCtx, "IDD_CONTROLTEXT", pNSResModule)
{
try
{
    pLabel      = new TEdit(this, IDC_CT_LABEL);
    pActif      = new TGroupBox(this, IDC_CT_ACTIF);
    pActifOui   = new TRadioButton(this, IDC_CT_ACTIF_OUI, pActif);
    pActifNon   = new TRadioButton(this, IDC_CT_ACTIF_NON, pActif);
    pTexte      = new TEdit(this, IDC_CT_TEXTE);

    sLabel      = sLabelNode;
    pCitem      = pCitemNode;
    bActif      = false;
    sTexte      = "";
}
catch (...)
{
	erreur("Exception NSControlTextDialog ctor.", standardError, 0);
}
}

NSControlTextDialog::~NSControlTextDialog()
{
    delete pLabel;
    delete pActif;
    delete pActifOui;
    delete pActifNon;
    delete pTexte;
}

void
NSControlTextDialog::SetupWindow()
{
    NSUtilDialog::SetupWindow();

    pLabel->SetText(sLabel.c_str());

    string sTexte = pCitem->getText();
    pTexte->SetText(sTexte.c_str());

    string sControl = pCitem->getControl();
    if (sControl == VAL_ATTR_ITEM_CTRL_ACTIF)
    {
        pActifOui->SetCheck(BF_CHECKED);
        pActifNon->SetCheck(BF_UNCHECKED);
    }
    else
    {
        pActifNon->SetCheck(BF_CHECKED);
        pActifOui->SetCheck(BF_UNCHECKED);
    }
}

void
NSControlTextDialog::CmOk()
{
    if (pActifOui->GetCheck() == BF_CHECKED)
        bActif = true;

    if (pActifNon->GetCheck() == BF_CHECKED)
        bActif = false;

    char far szTexte[255];
    pTexte->GetText(szTexte, 254);
    sTexte = string(szTexte);

    NSUtilDialog::CmOk();
}

void
NSControlTextDialog::CmCancel()
{
    NSUtilDialog::CmCancel();
}


// fin de nsarcedit.cpp
//////////////////////////////////////////

