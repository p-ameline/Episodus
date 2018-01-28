#include <dir.h>#include <string.h>
#include <cstring.h>
#include <stdio.h>

#include "nsoutil\nsrefedit.h"#include "nsoutil\nssavary.h"#include "nsbb\nsbbsmal.h"#include "nsbb\nstrewi.h"#include "nsbb\nsattval.h"#include "nsbb\nsarc.h"#include "nsoutil\nsoutil.h"#include "nsoutil\nsoutil.rh"/*#include "nssavoir\ns_fic.h"
#include "partage\nsdivfct.h"
#include "nautilus\nautilus.rh"
*/


/*
#define ID_TREE 100
#define ID_LIST 101

DEFINE_RESPONSE_TABLE1(NSsmEditeur, TPaneSplitter)    EV_LVN_GETDISPINFO(ID_LIST, LvnGetDispInfo),
    EV_COMMAND(CM_ENREGISTRE, CmEnregistre),
    EV_WM_CLOSE,
END_RESPONSE_TABLE;

NSsmEditeur::NSsmEditeur(TWindow* parent, NSContexte* pCtx, string sLexiquePilot)            :TPaneSplitter(parent), NSRoot(pCtx)
{
  string sCodeSens ;
  pContexte->getDico()->donneCodeSens(&sLexiquePilot, &sCodeSens) ;
  sLexiquePilote = sCodeSens ;
  pPatPathoArray = new NSPatPathoArray(pCtx->getSuperviseur()) ;
  initPatPatho() ;
  pSavoirArray = new NSSavoirArray ;
  initSavoir() ;

  pBigBoss = new NSSmallBrother(pCtx, pPatPathoArray) ;
  Attr.Style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN) ;
  //  // Create pNSTreeWindowow
  //
  pNSTreeWindow = new NSTreeWindow(this, pCtx, ID_TREE, 0, 0, 0, 0) ;
  pNSTreeWindow->Attr.Style   |= WS_VISIBLE | TVS_HASLINES |
                            TVS_HASBUTTONS | TVS_LINESATROOT ;
  pNSTreeWindow->Attr.ExStyle |= WS_EX_CLIENTEDGE ;

  //  // Create pListWindow
  //
  pListWind = new NSPaneListWindow(this, ID_LIST, 10, 10, 100, 100) ;
  pListWind->Attr.Style   |= LVS_REPORT | LVS_SORTASCENDING | LVS_EDITLABELS ;
  pListWind->Attr.ExStyle |= WS_EX_CLIENTEDGE ;
}

////
//
NSsmEditeur::~NSsmEditeur(){
  delete pBigBoss ;
  delete pSavoirArray ;
  delete pPatPathoArray ;

  pNSTreeWindow->LibereBBitemLanceur() ;
  delete pNSTreeWindow ;
  delete pListWind ;
}

//
//
//
void
NSsmEditeur::SetupWindow()
{
    // Initialize base class
    //
    //
    //mettre sLexiquePilote à vide pour ne pas écraser la patpatho dans
    //lanceConsult
    //
    if (!pPatPathoArray->empty())
        pBigBoss->lanceConsult("", pNSTreeWindow) ;
    else
  	    pBigBoss->lanceConsult(sLexiquePilote, pNSTreeWindow) ;

    TPaneSplitter::SetupWindow() ;
    pNSTreeWindow->SetupWindow() ;
    SplitPane(pNSTreeWindow, pListWind, psVertical) ;

    //dispatcher la patpatho
    pNSTreeWindow->DispatcherPatPatho(pPatPathoArray, 0, 0, "") ;

    if (!pBigBoss->getPatPatho()->empty())
		pBigBoss->MetTitre() ;

    // Initialize pListWind
    //
    TListWindColumn Lien("Lien sémantique", 100) ;
    pListWind->InsertColumn(0, Lien) ;
    TListWindColumn Equivalent("Equivalent sémantique", 150) ;
    pListWind->InsertColumn(1, Equivalent) ;

    // Remplis la ListWind
    //
    AfficheListe() ;
}

//
//
//
void
NSsmEditeur::CleanupWindow()
{
    // Cleanup the base class
    //
    TPaneSplitter::CleanupWindow() ;
}

void
NSsmEditeur::EvClose()
{
    TPaneSplitter::EvClose();
}

voidNSsmEditeur::initPatPatho()
{
    pPatPathoArray->vider() ;
    // char chAffiche[200], dateAffiche[20] ;
    //
	// Création d'un objet d'accès au fichier CR
	//
	NSSavFiche* pCR = new NSSavFiche(pContexte) ;
	//
	// Ouverture du fichier
	//
	pCR->lastError = pCR->open() ;
	if (pCR->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier de données.", 0, pCR->lastError) ;
		delete pCR ;
		return ;
	}
    //
    // Recherche de la première fiche
    //
    string cle = sLexiquePilote + string(BASE_LOCALISATION_LEN, ' ') ;

    pCR->lastError = pCR->chercheClef(  &cle,
                                        "",
                                        0,
                                        keySEARCHGEQ,
                                        dbiWRITELOCK) ;
    if (pCR->lastError != DBIERR_NONE)
	{
        erreur("Il n'existe pas de fiche pour cet élément.", 0, pCR->lastError) ;
        pCR->close() ;
		delete pCR ;
		return ;
	}
    pCR->lastError = pCR->getRecord() ;
    if (pCR->lastError != DBIERR_NONE)
	{
		erreur("Le fichier de données est défectueux.", 0, pCR->lastError) ;
        pCR->close();
		delete pCR;
		return;
	}
    if (strcmp(sLexiquePilote.c_str(), pCR->pDonnees->sens) != 0)
    {
        erreur("Il n'existe pas de fiche pour cet élément.", 0, pCR->lastError);
        pCR->close();
		delete pCR;
		return;
    }
    //
    // On avance dans le fichier tant que les fiches trouvées appartiennent
    // à ce compte rendu
    //
    while ((pCR->lastError != DBIERR_EOF) &&
   		 (strcmp(sLexiquePilote.c_str(), pCR->pDonnees->sens) == 0))
    {
        pPatPathoArray->push_back(new NSPatPathoInfo(pCR));
        pCR->lastError = pCR->suivant(dbiWRITELOCK);
        if ((pCR->lastError != DBIERR_NONE) && (pCR->lastError != DBIERR_EOF))
        {
            erreur("Pb d'acces a la fiche suivante.", 0, pCR->lastError);
      	    pCR->close();
			delete pCR;
			return;
		}

        if (pCR->lastError != DBIERR_EOF)
        {
   		    pCR->lastError = pCR->getRecord();
   		    if (pCR->lastError != DBIERR_NONE)
			{
                erreur("Le fichier de données est défectueux.", 0, pCR->lastError);
      		    pCR->close();
				delete pCR;
				return;
            }
        }
    }
    //
    // Fermeture du fichier
    //
    pCR->close();
    delete pCR;

	return;
}

voidNSsmEditeur::CmEnregistre()
{
   pNSTreeWindow->okFermerDialogue();
   pNSTreeWindow->EnregistrePatPatho(pBigBoss);
   if (!pBigBoss->getPatPatho()->empty())
		pBigBoss->MetTitre();
	enregPatPatho();
}

boolNSsmEditeur::enregPatPatho()
{
    bool existeInfo = true;
	//
	// Création d'un objet d'accès au fichier CR
	//
	NSSavFiche* pCR = new NSSavFiche(pContexte);
	//
	// Ouverture du fichier
	//
	pCR->lastError = pCR->open();
	if (pCR->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier de fiches.", 0, pCR->lastError);
		delete pCR;
		return false;
	}
    //
	// On efface d'abord les anciennes données
	//
    string cle = sLexiquePilote + string(BASE_LOCALISATION_LEN, ' ');
    bool effacer = true;
    while (effacer)
    {
   	    effacer = false;
        pCR->lastError = pCR->chercheClef(&cle,
                                            "",
                                            0,
                                            keySEARCHGEQ,
                                            dbiWRITELOCK);
        if (pCR->lastError == DBIERR_NONE)
        {
      	    pCR->lastError = pCR->getRecord();
            if (pCR->lastError == DBIERR_NONE)
         	    if (strcmp(sLexiquePilote.c_str(), pCR->pDonnees->sens) == 0)
            	    effacer = true;
        }
        if (effacer)
        {
      	    pCR->lastError = pCR->deleteRecord();
            if (pCR->lastError != DBIERR_NONE)
         	    effacer = false;
        }
    }
    //
    // Stockage de la PatPathoArray dans la base
    //
    for (PatPathoIter iJ = pPatPathoArray->begin(); iJ != pPatPathoArray->end(); iJ++)
    {
   	    *((NSPathoBaseData*)pCR->pDonnees) = *((NSPathoBaseData*)(*iJ)->pDonnees);
        strcpy(pCR->pDonnees->sens, sLexiquePilote.c_str());

        pCR->lastError = pCR->appendRecord();
        if (pCR->lastError != DBIERR_NONE)
        {
      	    erreur("Erreur à la sauvegarde des données.", 0, pCR->lastError);
            delete pCR;
            return false;
        }
    }
    //
    // Fermeture de la base
    //
    pCR->close();
    delete pCR;
    return existeInfo;
}

voidNSsmEditeur::initSavoir()
{
	pSavoirArray->vider();
    // char chAffiche[200], dateAffiche[20];
    //
	// Création d'un objet d'accès au fichier CR
	//
	NSSavoir* pSavoir = new NSSavoir(pContexte);
	//
	// Ouverture du fichier
	//
	pSavoir->lastError = pSavoir->open();
	if (pSavoir->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du réseau sémantique.", 0, pSavoir->lastError);
		delete pSavoir;
		return;
	}
    //
    // Recherche de la première fiche
    //
    CURProps curProps;
    pSavoir->lastError = DbiGetCursorProps(pSavoir->PrendCurseur(), curProps);

    Byte* pIndexRec = new Byte[curProps.iRecBufSize];
    memset(pIndexRec, 0, curProps.iRecBufSize);

    string sLienBlanc = string(SAVOIR_LIEN_LEN, ' ');
    DbiPutField(pSavoir->PrendCurseur(), SAVOIR_LIEN_FIELD, pIndexRec,(Byte*)(sLienBlanc.c_str()));
    DbiPutField(pSavoir->PrendCurseur(), SAVOIR_QUALIFIE_FIELD , pIndexRec, (Byte*)(sLexiquePilote.c_str()));

    pSavoir->lastError = pSavoir->chercheClefComposite("FLECHE",
														NODEFAULTINDEX,
                                          				keySEARCHGEQ,
                                          				dbiWRITELOCK,
                                          				pIndexRec);

    if (pSavoir->lastError != DBIERR_NONE)
	{
        erreur("Il n'existe pas de fiche sémantique pour cet élément.", 0, pSavoir->lastError);
        pSavoir->close();
        delete[] pIndexRec;
		delete pSavoir;
		return;
    }
    pSavoir->lastError = pSavoir->getRecord();
    if (pSavoir->lastError != DBIERR_NONE)
	{
        erreur("Le fichier de réseau sémantique est défectueux.", 0, pSavoir->lastError);
        pSavoir->close();
        delete[] pIndexRec;
		delete pSavoir;
		return;
    }
    if (strcmp(sLexiquePilote.c_str(), pSavoir->pDonnees->qualifie) != 0)
    {
        erreur("Il n'existe pas de fiche sémantique pour cet élément.", 0, pSavoir->lastError);
        pSavoir->close();
        delete[] pIndexRec;
		delete pSavoir;
		return;
    }
    //
    // On avance dans le fichier tant que les fiches trouvées appartiennent
    // à ce compte rendu
    //
    while ((pSavoir->lastError != DBIERR_EOF) &&
   		 (strcmp(sLexiquePilote.c_str(), pSavoir->pDonnees->qualifie) == 0))
	{
        pSavoirArray->push_back(new NSSavoirInfo(pSavoir));
        pSavoir->lastError = pSavoir->suivant(dbiWRITELOCK);
        if ((pSavoir->lastError != DBIERR_NONE) && (pSavoir->lastError != DBIERR_EOF))
		{
            erreur("Pb d'acces a la fiche suivante du réseau sémantique.", 0, pSavoir->lastError);
      	    pSavoir->close();
            delete[] pIndexRec;
			delete pSavoir;
			return;
        }

        if (pSavoir->lastError != DBIERR_EOF)
        {
   		    pSavoir->lastError = pSavoir->getRecord();
   		    if (pSavoir->lastError != DBIERR_NONE)
			{
                erreur("Le fichier de données du réseau sémantique est défectueux.", 0, pSavoir->lastError);
      		    pSavoir->close();
                delete[] pIndexRec;
				delete pSavoir;
				return;
            }
        }
    }
    //
    // Fermeture du fichier
    //
    pSavoir->close();
    delete[] pIndexRec;
    delete pSavoir;

	return;
}

//---------------------------------------------------------------------------//  Description: Affiche le tableau de patients dans la liste
//---------------------------------------------------------------------------
void
NSsmEditeur::AfficheListe()
{
    pListWind->DeleteAllItems();
    for (SavInfoIter iJ = pSavoirArray->begin(); iJ != pSavoirArray->end(); iJ++)    {
        string sEtiquette;
        string sQualificatif = (*iJ)->pDonnees->lien;
        string sQualifie		= (*iJ)->pDonnees->qualifiant;
        donneRelation(&sQualificatif, &sQualifie, &sEtiquette);
   	    TListWindItem Item(sEtiquette.c_str(), 0);
        pListWind->InsertItem(Item);
    }
}

//
// Callback notification to handle additional column information
// for each item.
// Dans ce cas, ajoute le qualifiant
//
void
NSsmEditeur::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
  	const int BufLen = 255;
  	static char buffer[BufLen];
  	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;

  	int index = dispInfoItem.GetIndex();  	string sQualif 	= "";
  	string sLibelle = "";
  	int 	iGenre = 0;

  	// Affiche les informations en fonction de la colonne
  	switch (dispInfoItem.GetSubItem())
  	{
  		case 1: 	// qualifiant
            string sLang = "";
            if ((pContexte) && (pContexte->getUtilisateur()))
                sLang = pContexte->getUtilisateur()->donneLang();

      	    sQualif = string(((*pSavoirArray)[index])->pDonnees->qualifiant) + string("1");
        	pContexte->getDico()->donneLibelle(sLang, &sQualif, &sLibelle, NSDico::affiche, &iGenre);
        	sprintf(buffer, "%s", sLibelle.c_str());
        	dispInfoItem.SetText(buffer);
        	break;
  	}
}

void
NSsmEditeur::donneRelation(string* pQualificatif, string* pQualifie, string* pEtiquette)
{
	char cQualif = (*pQualifie)[0];
    char cPilote = sLexiquePilote[0];

    if (*pQualificatif == "ES")
    {
   	    *pEtiquette = "est un";
        return;
    }

    if (*pQualificatif == "ME")
    {
   	    switch (cPilote)
   	    {
   		    case 'V' :
      		    switch (cQualif)
         	    {
         		    case '2' : *pEtiquette = "se mesure en"; break;
         	    }
      		    break;
            case 'G' :
      		    switch (cQualif)
         	    {
         		    case 'V' : *pEtiquette = "sert à mesurer"; break;
         	    }
      		    break;
            case 'O' :
      		    switch (cQualif)
         	    {
         		    case 'G' : *pEtiquette = "instrument de mesure de"; break;
         	    }
      		    break;
            case 'B' :
      		    switch (cQualif)
         	    {
         		    case 'V' : *pEtiquette = "sert à mesurer"; break;
         	    }
      		    break;
        }
        return;
    }

    if (*pQualificatif == "AT")
    {
   	    switch (cPilote)
   	    {
   		    case 'P' :
      		    switch (cQualif)
         	    {
         		    case 'A' : *pEtiquette = "atteint"; break;
                    case 'H' : *pEtiquette = "atteint"; break;
         	    }
      		    break;
            case 'G' :
      		    switch (cQualif)
         	    {
         		    case 'P' : *pEtiquette = "s'applique à"; break;
                    case 'A' : *pEtiquette = "s'effectue sur"; break;
         	    }
      		    break;
            case 'A' :
      		    switch (cQualif)
         	    {
                    case 'A' : *pEtiquette = "est une partie de"; break;
         	    }
      		    break;
        }
        return;
    }
}
*/

//***************************************************************************
//
//  		  Méthodes de NSProposEdit//
//***************************************************************************
NSProposEdit::NSProposEdit(Cproposition* prop)
{
  init() ;

  if ((Cproposition*) NULL == prop)
    return ;

  _sName  = prop->getStringAttribute(ATTRIBUT_PROP_NOM) ;
  _sGroup = prop->getStringAttribute(ATTRIBUT_PROP_GROUPE) ;

  Valeur_array *pValArray = prop->getValArray() ;
  if ((Valeur_array*) NULL == pValArray)
    return ;

  for (ValIter ival = pValArray->begin() ; pValArray->end() != ival ; ival++)
  {
    if ((*ival)->getLabel() == LABEL_VALIDITE)
    {
      // en principe il n'y a qu'une seule clause validite (sans attributs)
      // on récupère la string de validite telle quelle
      _sValidity = (*ival)->getValue() ;
    }
    else if ((*ival)->getLabel() == LABEL_TREE)
    {
      Ctree* pCtree = dynamic_cast<Ctree*>((*ival)->getObject()) ;
      if (pCtree)
      {
        _aTreeArray.push_back(new NSPatPathoArray(*(pCtree->pPatPathoArray))) ;
        _iNbTrees++ ;
      }
    }
  }
}

NSProposEdit::NSProposEdit()
{
  init() ;
}

NSProposEdit::~NSProposEdit()
{
}

NSProposEdit::NSProposEdit(const NSProposEdit& rv)
{
  _sName      = rv._sName ;
  _sGroup     = rv._sGroup ;
  _sValidity  = rv._sValidity ;
  _aTreeArray = rv._aTreeArray ;
  _iNbTrees   = rv._iNbTrees ;
}

void
NSProposEdit::init()
{
  _sName     = string("") ;
  _sGroup    = string("") ;
  _sValidity = string("") ;

  _iNbTrees  = 0 ;
}

NSProposEdit&
NSProposEdit::operator=(const NSProposEdit& src)
{
  if (&src == this)
		return *this ;

  _sName      = src._sName ;
  _sGroup     = src._sGroup ;
  _sValidity  = src._sValidity ;
  _aTreeArray = src._aTreeArray ;
  _iNbTrees   = src._iNbTrees ;

  return *this ;
}

// Classe NSProposEditArray
////////////////////////////////////////////////////

//---------------------------------------------------------------------------
//  Constructeur copie//---------------------------------------------------------------------------
NSProposEditArray::NSProposEditArray(const NSProposEditArray& rv)                  :NSProposEditVector(){  if (false == rv.empty())    for (NSProposEditConstIter i = rv.begin() ; rv.end() != i ; i++)      push_back(new NSProposEdit(**i)) ;}
NSProposEditArray&
NSProposEditArray::operator=(const NSProposEditArray& src)
{  if (&src == this)		return *this ;  if (false == src.empty())    for (NSProposEditConstIter i = src.begin() ; src.end() != i ; i++)      push_back(new NSProposEdit(**i)) ;  return *this ;}

//---------------------------------------------------------------------------//  Destructeur//---------------------------------------------------------------------------void
NSProposEditArray::vider(){  if (false == empty())    for (NSProposEditIter i = begin() ; end() != i ; )    {      delete *i ;      erase(i) ;    }}
NSProposEditArray::~NSProposEditArray(){	vider() ;}

//***************************************************************************
//
//  		  Méthodes de NSListPropWindow//
//***************************************************************************
DEFINE_RESPONSE_TABLE1(NSListPropWindow, TListWindow)   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Function: NSListPropWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------

voidNSListPropWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TLwHitTestInfo info(point);

    HitTest(info);
    if (info.GetFlags() & LVHT_ONITEM)        pDlg->CmEditProp();
}

//---------------------------------------------------------------------------//  Function: NSListPropWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------

intNSListPropWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;}

//***************************************************************************//
//  Méthodes de NSRefEditeur
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSRefEditeur, NSUtilDialog)
    EV_LV_DISPINFO_NOTIFY(IDC_RE_LVIEW, LVN_GETDISPINFO, DispInfoListeProp),
    EV_COMMAND(IDC_RE_NEWREF, CmReferentiel),
    EV_COMMAND(IDC_RE_PROPHAUT, CmHaut),
    EV_COMMAND(IDC_RE_PROPBAS, CmBas),
    EV_COMMAND(IDC_RE_ADDPROP, CmAjouter),
    EV_COMMAND(IDC_RE_DELPROP, CmSupprimer),
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSRefEditeur::NSRefEditeur(TWindow* pere, NSContexte* pCtx)
             :NSUtilDialog(pere, pCtx, "IDD_REFEDIT", pNSResModule)
{
  _pRefName  = new TEdit(this, IDC_RE_REFNAME) ;
  _pListProp = new NSListPropWindow(this, IDC_RE_LVIEW) ;
  _pParseur  = (nsrefParseur*) 0 ;
  _bIsModif  = false ;
  _nbProp    = 0 ;
}

NSRefEditeur::~NSRefEditeur()
{
  delete _pRefName ;
  delete _pListProp ;
  if (_pParseur)
    delete _pParseur ;
}

void
NSRefEditeur::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  InitListeProp() ;
}

void
NSRefEditeur::InitListeProp()
{
  TListWindColumn colName("Nom", 280, TListWindColumn::Left, 0) ;
  _pListProp->InsertColumn(0, colName) ;

  TListWindColumn colGroup("Groupe", 200, TListWindColumn::Left, 1) ;  _pListProp->InsertColumn(1, colGroup) ;
}

void
NSRefEditeur::AfficheListeProp()
{
  _pListProp->DeleteAllItems() ;

  for (int i = _nbProp - 1; i >= 0; i--)  {
    TListWindItem Item(((_aPropArray[i])->getName()).c_str(), 0) ;
    _pListProp->InsertItem(Item) ;
  }
}

void
NSRefEditeur::DispInfoListeProp(TLwDispInfoNotify& dispInfo)
{
  const int 	    BufLen = 255 ;
  static char     buffer[BufLen] ;
  TListWindItem&  dispInfoItem = *(TListWindItem*)&dispInfo.item ;

  int index = dispInfoItem.GetIndex() ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case 1: // groupe
      strcpy(buffer, ((_aPropArray[index])->getGroup()).c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;
  }
}

void
NSRefEditeur::EcrireTree(NSPatPathoArray* pPatPathoArray, string& sOut)
{
  if ((NSPatPathoArray*) NULL == pPatPathoArray)
    return ;

  // on prend pour l'instant un arbre d'objectif par défaut
  sOut += string("<") + string(LABEL_TREE) + string(" localisation = \"ZPOMR/0OBJE\">\n") ;
  int colPrec = -1 ;

  for (PatPathoIter i = pPatPathoArray->begin() ; pPatPathoArray->end() != i ; i++)
  {
    int col = (*i)->getColonne() ;
    for (int k = 0 ; k < col ; k++)
      sOut += string("\t") ;

    // fermer les blocs précédents
    if (col <= colPrec)
    {
      for (int k = colPrec ; k >= col ; k--)
      {
        for (int j = 0 ; j < k ; j++)
          sOut += string("\t") ;

        sOut += string("</") + string(LABEL_NODE) + string(">\n") ;
      }
    }

    sOut += string("<") + string(LABEL_NODE) + string(" ") ;

    // donnees obligatoires
    sOut += string(ATTRIBUT_NODE_LOC) + string(" = \"") + (*i)->getLocalisation() + string("\" ") ;
    if (string("") != (*i)->getType())
      sOut += string(ATTRIBUT_NODE_TYPE) + string(" = \"") + (*i)->getType() + string("\" ") ;
    sOut += string(ATTRIBUT_NODE_LEXIQUE) + string(" = \"") + texteHtml((*i)->getLexique()) + string("\" ") ;

    // textes libres ou complement
    if (string("£?????") == (*i)->getLexique())
    {
      sOut += string(ATTRIBUT_NODE_COMPLEMENT) + string(" = \"") ;
      string sTexteLibre = (*i)->getTexteLibre() ;
      sOut += texteHtml(sTexteLibre) + string("\" ") ;
    }
    else if (string("") != (*i)->getComplement())
    {
      sOut += string(ATTRIBUT_NODE_COMPLEMENT) + string(" = \"") ;
      sOut += texteHtml((*i)->getComplement()) + string("\" ") ;
    }

    // donnees annexes
    if (string("") != (*i)->getCertitude())
    {
      sOut += string(ATTRIBUT_NODE_CERTITUDE) + string(" = \"") ;
      sOut += texteHtml((*i)->getCertitude()) + string("\" ") ;
    }

    if (string("") != (*i)->getInteret())
    {
      sOut += string(ATTRIBUT_NODE_INTERET) + string(" = \"") ;
      sOut += texteHtml((*i)->getInteret()) + string("\" ") ;
    }

    if (string("") != (*i)->getPluriel())
    {
      sOut += string(ATTRIBUT_NODE_PLURIEL) + string(" = \"") ;
      sOut += texteHtml((*i)->getPluriel()) + string("\" ") ;
    }

    sOut += string(">\n") ;

    colPrec = col ;
  }

  // fermer les derniers blocs jusqu'à la colonne 0
  for (int k = colPrec ; k >= 0 ; k--)
  {
    for (int j = 0 ; j < k ; j++)
      sOut += string("\t") ;

    sOut += string("</") + string(LABEL_NODE) + string(">\n") ;
  }

  sOut += string("</") + string(LABEL_TREE) + string(">\n") ;
}

void
NSRefEditeur::CmReferentiel()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

	if (true == _bIsModif)
	{
		string sCaption = string("Message ") + pSuper->getAppName() ;
		int idRet = MessageBox("Attention un référentiel est déjà ouvert et a été modifié. Voulez-vous le sauvegarder ?", sCaption.c_str(), MB_YESNO);
		if (IDYES == idRet)
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

	_sFileName = string(initData.FileName) ;
	if (_pParseur)
		delete _pParseur ;

	_pParseur = new nsrefParseur(pSuper) ;

	if (false == _pParseur->open(_sFileName))
	{
		string sCaption = string("Message ") + pSuper->getAppName() ;
    MessageBox("Erreur de parsing du référentiel", sCaption.c_str(), MB_OK) ;
    return;
	}

  Creferentiel* pRef = _pParseur->getReferentiel() ;
  if ((Creferentiel*) NULL == pRef)
    return ;

	_aPropArray.vider() ;
	_nbProp = 0 ;

  Valeur_array *pValArray = pRef->getValArray() ;
  if ((Valeur_array*) NULL == pValArray)
    return ;

	if (false == pValArray->empty())
	{
		// on instancie l'ensemble des propositions
		for (ValIter ival = pValArray->begin() ; pValArray->end() != ival ; ival++)
		{
  		if ((*ival)->getLabel() == LABEL_PROPOSITION)
    	{    		Cproposition* pCprop = dynamic_cast<Cproposition*>((*ival)->getObject()) ;        if (pCprop)        {      	  _aPropArray.push_back(new NSProposEdit(pCprop)) ;      	  _nbProp++ ;        }    	}		}
	}

	AfficheListeProp() ;
	_pRefName->SetText(_sFileName.c_str()) ;
}

void
NSRefEditeur::CmEditProp()
{
  int propChoisie = _pListProp->IndexItemSelect() ;

  if (-1 == propChoisie)
  {
    erreur("Vous devez sélectionner une proposition à éditer.", standardError, 0) ;
    return ;
  }

  NSPropEditeur* pDlg = new NSPropEditeur(this, pContexte, _aPropArray[propChoisie]) ;
  int idRet = pDlg->Execute() ;

  if (IDOK == idRet)
  {
    if (false == _bIsModif)
      _bIsModif = pDlg->bIsModif ;

    if (_bIsModif)
      *(_aPropArray[propChoisie]) = *(pDlg->pPropos) ;

    AfficheListeProp() ;
  }

  delete pDlg ;
}

void
NSRefEditeur::CmHaut()
{
  int iPropChoisie = _pListProp->IndexItemSelect() ;

  if (-1 == iPropChoisie)
  {
    erreur("Vous devez sélectionner une proposition à déplacer.", standardError, 0) ;
    return ;
  }

  if (0 == iPropChoisie)
    return ;

  NSProposEdit propos = *(_aPropArray[iPropChoisie]) ;
  *(_aPropArray[iPropChoisie]) = *(_aPropArray[iPropChoisie-1]) ;
  *(_aPropArray[iPropChoisie-1]) = propos ;

  AfficheListeProp() ;

  _pListProp->SetFocus() ;
  _pListProp->SetSel(iPropChoisie-1, true) ;
  _pListProp->EnsureVisible(iPropChoisie-1, true) ;
}

void
NSRefEditeur::CmBas()
{
  int iPropChoisie = _pListProp->IndexItemSelect() ;

  if (-1 == iPropChoisie)
  {
    erreur("Vous devez sélectionner une proposition à déplacer.", standardError, 0) ;
    return ;
  }

  if (_nbProp - 1 == iPropChoisie)
    return ;

  NSProposEdit propos = *(_aPropArray[iPropChoisie]) ;
  *(_aPropArray[iPropChoisie]) = *(_aPropArray[iPropChoisie + 1]) ;
  *(_aPropArray[iPropChoisie+1]) = propos ;

  AfficheListeProp() ;

  _pListProp->SetFocus() ;
  _pListProp->SetSel(iPropChoisie+1, true) ;
  _pListProp->EnsureVisible(iPropChoisie+1, true) ;
}

void
NSRefEditeur::CmAjouter()
{
  // on prend une proposition vierge
  NSProposEdit prop ;

  NSPropEditeur* pDlg = new NSPropEditeur(this, pContexte, &prop) ;
  int idRet = pDlg->Execute() ;

  if (IDOK == idRet)
  {
    if (false == _bIsModif)
      _bIsModif = pDlg->bIsModif ;

    if (_bIsModif)
    {
      _aPropArray.push_back(new NSProposEdit(*(pDlg->pPropos))) ;
      _nbProp++ ;
    }

    AfficheListeProp() ;
  }

  delete pDlg ;
}

void
NSRefEditeur::CmSupprimer()
{
  int iPropChoisie = _pListProp->IndexItemSelect() ;
  if (-1 == iPropChoisie)
  {
    erreur("Vous devez sélectionner une proposition à supprimer.", standardError, 0) ;
    return ;
  }

  if (_aPropArray.empty())
    return ;

  int iIndex = 0 ;
  for (NSProposEditIter i = _aPropArray.begin() ; _aPropArray.end() != i ; i++)
  {
    if (iIndex == iPropChoisie)
    {
      delete (*i);
      _aPropArray.erase(i) ;
      _nbProp-- ;
      break ;
    }

    iIndex++ ;
  }

  AfficheListeProp() ;
}

void
NSRefEditeur::CmSauvegarde()
{
  char szFileName[1024] ;
  _pRefName->GetText(szFileName, 255) ;

  if (string(szFileName) != _sFileName)
    _sFileName = string(szFileName);

    ofstream outFile;

  string sOut = string("") ;
  sOut += string("<") + string(LABEL_REFERENTIEL) + string(">\n") ;

  if (false == _aPropArray.empty())
    for (NSProposEditIter i = _aPropArray.begin() ; _aPropArray.end() != i ; i++)
    {
      sOut += string("<") + string(LABEL_PROPOSITION) + string(" ") ;
      sOut += string(ATTRIBUT_PROP_NOM) + string(" = \"") + texteHtml((*i)->getName()) + string("\"") ;

      if (string("") != (*i)->getGroup())
      {
        sOut += string(" ") + string(ATTRIBUT_PROP_GROUPE) + string(" = \"") ;
        sOut += texteHtml((*i)->getGroup()) + string("\"") ;
      }

      sOut += string(">\n") ;

      // clause de validité
      if (string("") != (*i)->getValidity())
      {
        sOut += string("<") + string(LABEL_VALIDITE) + string(">\n") ;
        sOut += (*i)->getValidity() + string("\n") ;
        sOut += string("</") + string(LABEL_VALIDITE) + string(">\n") ;
      }

      if (false == (*i)->getTrees()->empty())
      {
        for (PatPathoIterVect k = (*i)->getTrees()->begin() ; (*i)->getTrees()->end() != k ; k++)
          // (*k) est un pPatPathoArray
          if (false == (*k)->empty())
            EcrireTree(*k, sOut) ;

      }

      sOut += string("</") + string(LABEL_PROPOSITION) + string(">\n");
    }

  sOut += string("</") + string(LABEL_REFERENTIEL) + string(">\n");

  // écriture sur disque
  outFile.open(_sFileName.c_str());
  if (!outFile)
  {
    erreur("Impossible de créer le fichier référentiel à sauvegarder", standardError, 0) ;
    return;
  }

  for (size_t i = 0; i < strlen(sOut.c_str()); i++)
    outFile.put(sOut[i]) ;

	outFile.close() ;
}

void
NSRefEditeur::CmOk()
{
  if (_bIsModif)
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
    int idRet = MessageBox("Le référentiel a été modifié. Voulez-vous le sauvegarder ?", sCaption.c_str(), MB_YESNO) ;
    if (IDYES == idRet)
      CmSauvegarde() ;
  }

  TDialog::CmOk() ;
}

void
NSRefEditeur::CmCancel()
{
  TDialog::CmCancel() ;
}

//***************************************************************************
//
//  Méthodes de NSPropEditeur
//
//***************************************************************************

DEFINE_RESPONSE_TABLE1(NSPropEditeur, NSUtilDialog)
  EV_COMMAND(IDC_PE_NEXTTREE, CmSuivant),
  EV_COMMAND(IDC_PE_PREVTREE, CmPrecedent),
  EV_COMMAND(IDC_PE_ADDTREE, CmAjouter),
  EV_COMMAND(IDC_PE_DELTREE, CmSupprimer),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSPropEditeur::NSPropEditeur(TWindow* pere, NSContexte* pCtx, NSProposEdit* prop)
              :NSUtilDialog(pere, pCtx, "IDD_PROPEDIT", pNSResModule)
{
  pPropName  = new TEdit(this, IDC_PE_PROPNAME) ;
  pPropGroup = new TEdit(this, IDC_PE_GROUPNAME) ;
  pTreeNum   = new TEdit(this, IDC_PE_NUMTREE) ;
  pPropos    = new NSProposEdit(*prop) ;

  noTree     = 0 ;
  sPropName  = prop->getName() ;
  sPropGroup = prop->getGroup() ;
  bIsModif   = false ;

  // Attr.Style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
  pPatPathoArray = new NSPatPathoArray(pCtx->getSuperviseur()) ;
  pBigBoss       = (NSSmallBrother*) 0 ;
  pNSTreeWindow  = new NSTreeWindow(this, pCtx, IDC_PE_TVIEW, pNSResModule) ;
  // pNSTreeWindow->Attr.Style   |= WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT;  // pNSTreeWindow->Attr.ExStyle |= WS_EX_CLIENTEDGE;
}

NSPropEditeur::~NSPropEditeur()
{
  delete pPropName ;
  delete pPropGroup ;
  delete pTreeNum ;
  delete pPropos ;
  if (pBigBoss)
    delete pBigBoss ;
  delete pPatPathoArray ;

  pNSTreeWindow->LibereBBitemLanceur() ;  delete pNSTreeWindow ;
}

void
NSPropEditeur::SetupWindow()
{
  initPatPatho(0) ;

  TDialog::SetupWindow() ;

  pNSTreeWindow->DispatcherPatPatho(pPatPathoArray, 0, 0, "") ;

  pPropName->SetText(sPropName.c_str()) ;  pPropGroup->SetText(sPropGroup.c_str()) ;  char szIdTree[10] ;  sprintf(szIdTree, "1/%d", pPropos->getNbTrees()) ;  pTreeNum->SetText(szIdTree) ;}
void
NSPropEditeur::initPatPatho(int idTree)
{
  pPatPathoArray->vider() ;

  if (pPropos->getNbTrees() > 0)
  {
    // récupérer l'arbre n°idTree de la proposition
    *pPatPathoArray = *((*(pPropos->getTrees()))[idTree]) ;

    // affichage de l'indice tree
    char szIdTree[10] ;
    itoa(idTree+1, szIdTree, 10) ;
    sprintf(szIdTree, "%d/%d", idTree+1, pPropos->getNbTrees()) ;
    pTreeNum->SetText(szIdTree) ;
  }
  else // pas d'arbre => patpatho vide
    pTreeNum->SetText("1/1") ;

  if (pBigBoss)
    delete pBigBoss ;
  pBigBoss = new NSSmallBrother(pContexte, pPatPathoArray) ;

  if (false == pPatPathoArray->empty())
  {
    // insérer la racine "objectif" fictive ("titre" de l'arbre)
    pBigBoss->setLexiqueModule(string("0OBJE1")) ;
    // strcpy(pBigBoss->noeudModule, "");
    pBigBoss->MetTitre() ;
    pBigBoss->lanceConsult(string(""), pNSTreeWindow) ;
    bWithTitle = true ;
  }
  else // cas édition patpatho vide : on passe un sLexiqueModule à lanceConsult
  {
    pBigBoss->lanceConsult(string("0OBJE1"), pNSTreeWindow) ;
    bWithTitle = false ;
  }
}

void
NSPropEditeur::rechargerPatPatho()
{
  // vider la patpatho affichée
  pNSTreeWindow->SortieEditionDico() ;
  if (pNSTreeWindow->getEditDico())
  {
    NSEditDico* pDico = pNSTreeWindow->getEditDico() ;
    delete pDico ;
    pNSTreeWindow->setEditDico((NSEditDico*) 0) ;
  }
  pNSTreeWindow->SupprimerTousNoeuds() ;
  pNSTreeWindow->LibereBBitemLanceur() ;
  pNSTreeWindow->DeleteAllItems() ;

  // recharger la nouvelle en fonction de l'index tree
  initPatPatho(noTree) ;
  pNSTreeWindow->SetupWindow() ;
  pNSTreeWindow->DispatcherPatPatho(pPatPathoArray, 0, 0, "") ;
}

void
NSPropEditeur::sauverPatPatho()
{
  // stocker les modifs de la patpatho
  pNSTreeWindow->okFermerDialogue() ;

  // if (bWithTitle)
  // retirer la racine fictive
  pBigBoss->EnleverTitre() ;

  // on sauvegarde l'arbre en cours
  if (pPropos->getNbTrees() > 0)
    *((*(pPropos->getTrees()))[noTree]) = *pPatPathoArray ;
  else
  {
    // on sauve le nouvel arbre de proposition
    pPropos->_aTreeArray.push_back(new NSPatPathoArray(*pPatPathoArray)) ;
    pPropos->_iNbTrees++ ;
    noTree = 0 ;
  }
}

void
NSPropEditeur::CmSuivant()
{
  sauverPatPatho() ;

  if (pPropos->getNbTrees() <= 1)
    return ;

  if (noTree < (pPropos->getNbTrees() - 1))
    noTree++ ;
  else
    noTree = 0 ;

  rechargerPatPatho() ;
}

void
NSPropEditeur::CmPrecedent()
{
  sauverPatPatho() ;

  if (pPropos->getNbTrees() <= 1)
    return ;

  if (noTree > 0)
    noTree-- ;
  else
    noTree = pPropos->getNbTrees() - 1 ;

  rechargerPatPatho() ;
}

void
NSPropEditeur::CmAjouter()
{
  if (pPropos->getNbTrees() == 0)
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
    MessageBox("Pour saisir le premier arbre de la proposition, clickez dans la fenetre d'édition. Pour les suivants, clickez d'abord sur Ajouter.", sCaption.c_str(), MB_OK) ;
    return ;
  }

  sauverPatPatho() ;

  pPatPathoArray->vider() ;
  pPropos->getTrees()->push_back(new NSPatPathoArray(*pPatPathoArray)) ;
  pPropos->_iNbTrees++ ;
  noTree = pPropos->getNbTrees() - 1 ;

  rechargerPatPatho() ;
}

void
NSPropEditeur::CmSupprimer()
{
  int index = 0 ;

  if (pPropos->getNbTrees() == 0)
    return ;

  NSVectPatPathoArray *pVectPath = pPropos->getTrees() ;
  if (((NSVectPatPathoArray*) NULL == pVectPath) || pVectPath->empty())

  for (PatPathoIterVect i = pVectPath->begin() ; pVectPath->end() != i ; i++)
  {
    if (index == noTree)
    {
      delete (*i) ;
      pVectPath->erase(i) ;

      pPropos->_iNbTrees--;
      if (noTree > 0)
        noTree-- ;
      break ;
    }

    index++ ;
  }

  pNSTreeWindow->okFermerDialogue() ;
  rechargerPatPatho() ;
}

void
NSPropEditeur::CmOk()
{
  char text[256] ;

  sauverPatPatho() ;

  pPropName->GetText(text, 255) ;
  if (string(text) != sPropName)
    pPropos->setName(string(text)) ;

  pPropGroup->GetText(text, 255) ;
  if (string(text) != sPropGroup)
    pPropos->setGroup(string(text)) ;

  bIsModif = true ;
  TDialog::CmOk() ;
}

void
NSPropEditeur::CmCancel()
{
  bIsModif = false ;
  TDialog::CmCancel() ;
}

// nsrefedit.cpp end
//////////////////////////////////////////

