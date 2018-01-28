#include <string.h>#include <cstring.h>
#include <stdio.h>
#include <sysutils.hpp>

#include "nautilus\nssuper.h"
#include "nsoutil\nsfilgui.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsoutil.rh"
#include "nssavoir\nsguide.h"
#include "nssavoir\nspatbas.h"
#include "nsbb\nsednum.h"
#include "partage\nsdivfile.h"

// *********************************
// Définition de la classe NSElement
// *********************************

NSElement::NSElement(){
  _sCode     = string("") ;
  _sLibelle  = string("") ;
  _sDecalage = string("") ;
  _iResult   = -1 ;
}

NSElement::NSElement(string sCode, string sLibelle, string sDecalage)
{
	_sCode     = sCode ;
	_sLibelle  = sLibelle ;
	_sDecalage = sDecalage ;
	_iResult   = -1 ;
}

NSElement::~NSElement(){
}

NSElement::NSElement(const NSElement& rv)
{
  _sCode     = rv._sCode ;
  _sLibelle  = rv._sLibelle ;
  _sDecalage = rv._sDecalage ;
  _iResult   = rv._iResult ;
}

NSElement&
NSElement::operator=(const NSElement& src)
{
	if (this == &src)
		return *this ;

  _sCode     = src._sCode ;
  _sLibelle  = src._sLibelle ;
  _sDecalage = src._sDecalage ;
  _iResult   = src._iResult ;

	return *this ;
}

intNSElement::operator==(const NSElement& o)
{
  if ((_sCode     == o._sCode) &&
      (_sLibelle  == o._sLibelle) &&
      (_sDecalage == o._sDecalage))
		return 1 ;
  else
		return 0 ;
}

// **********************************
// Définition de la classe NSEltArray
// **********************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSEltArray::NSEltArray(const NSEltArray& rv) : NSEltVector()
{
	if (false == rv.empty())
		for (NSEltConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSElement(**i)) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSEltArray::vider()
{
	if (false == empty())
		for (NSEltIter i = begin() ; end() != i ; )
    {
    	delete *i ;
    	erase(i) ;
    }
}

NSEltArray::~NSEltArray(){
	vider() ;
}

NSEltArray&
NSEltArray::operator=(const NSEltArray& src)
{
	if (this == &src)
		return *this ;

  vider() ;

  if (false == src.empty())
		for (NSEltConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSElement(**i)) ;

	return *this ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSModifFilGuideDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSModifFilGuideDialog, NSUtilDialog)    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_BN_CLICKED(IDC_MODFG_SEQUENCE, CmTouteSequence),
    EV_BN_CLICKED(IDC_MODFG_ELEMENT, CmToutElement),
    EV_BN_CLICKED(IDC_MODFG_PLUR1, CmMajCode),
    EV_BN_CLICKED(IDC_MODFG_PLUR2, CmMajCode),
    EV_BN_CLICKED(IDC_MODFG_PLURSANS, CmMajCode),
    EV_BN_CLICKED(IDC_MODFG_CERT00, CmMajCode),
    EV_BN_CLICKED(IDC_MODFG_CERT25, CmMajCode),
    EV_BN_CLICKED(IDC_MODFG_CERT50, CmMajCode),
    EV_BN_CLICKED(IDC_MODFG_CERT75, CmMajCode),
    EV_BN_CLICKED(IDC_MODFG_CERT100, CmMajCode),
    EV_BN_CLICKED(IDC_MODFG_CERTSANS, CmMajCode),
END_RESPONSE_TABLE;

NSModifFilGuideDialog::NSModifFilGuideDialog(TWindow* pere, string code, NSContexte* pCtx,                                                TModule* mod, bool bDialoguePere)
                         :NSUtilDialog(pere, pCtx, "IDD_MODFG", mod)
{
    pEdit           = new NSUtilLexique(pContexte, this, IDC_MODFG_EDIT, pContexte->getDico());
    pTouteSequence  = new TRadioButton(this, IDC_MODFG_SEQUENCE);
    pToutElement    = new TRadioButton(this, IDC_MODFG_ELEMENT);

    // GroupBox pour la certitude
    pCertitude      = new TGroupBox(this, IDC_MODFG_CERTGROUP);
    pCertSans       = new TRadioButton(this, IDC_MODFG_CERTSANS, pCertitude);
    pCert00         = new TRadioButton(this, IDC_MODFG_CERT00, pCertitude);
    pCert25         = new TRadioButton(this, IDC_MODFG_CERT25, pCertitude);
    pCert50         = new TRadioButton(this, IDC_MODFG_CERT50, pCertitude);
    pCert75         = new TRadioButton(this, IDC_MODFG_CERT75, pCertitude);
    pCert100        = new TRadioButton(this, IDC_MODFG_CERT100, pCertitude);

    // GroupBox pour le pluriel
    pPluriel        = new TGroupBox(this, IDC_MODFG_PLURGROUP);
    pPlur1          = new TRadioButton(this, IDC_MODFG_PLUR1, pPluriel);
    pPlur2          = new TRadioButton(this, IDC_MODFG_PLUR2, pPluriel);
    pSingulier      = new TRadioButton(this, IDC_MODFG_PLURSANS, pPluriel);

    sCode = code;
    bPere = bDialoguePere;
}

NSModifFilGuideDialog::~NSModifFilGuideDialog()
{
    delete pEdit;
    delete pTouteSequence;
    delete pToutElement;
    delete pCertSans;
    delete pCert00;
    delete pCert25;
    delete pCert50;
    delete pCert75;
    delete pCert100;
    delete pCertitude;
    delete pPlur1;
    delete pPlur2;
    delete pSingulier;
    delete pPluriel;
}

void
NSModifFilGuideDialog::SetupWindow()
{
	string sText ;

	NSUtilDialog::SetupWindow() ;

  if (sCode == "")
  {
    pSingulier->Check() ;
    pCertSans->Check() ;
    pEdit->setLabel("") ;
    pEdit->SetFocus() ;
    return ;
  }
  else if (sCode == "~????1")
  {
    CmToutElement() ;
    return ;
  }
  else if (sCode == "~~***1")
  {
    CmTouteSequence() ;
    return ;
  }
  else if (sCode[0] == '~')
    sText = string("Joker (") + string(sCode, 3, 2) + string(")") ;
  else
    sText = "" ;

  // on doit initialiser le pluriel et la certitude
  size_t pos1, pos2 ;
  string sRacine ;
  string sCompl ;

  pos1 = sCode.find('/') ;
  if (pos1 != string::npos)
  {
    sRacine = string(sCode, 0, pos1) ;
    pCertSans->Check() ;
    pSingulier->Check() ;

    while (pos1 != string::npos)
    {
      pos2 = sCode.find('/', pos1+1) ;
      if (pos2 != string::npos)
      	sCompl = string(sCode, pos1+1, pos2-pos1-2) ;
      else
      	sCompl = string(sCode, pos1+1, strlen(sCode.c_str())-pos1-2) ;

      if (sCompl == "")
      {
        erreur("Cet élément comporte un attribut vide.", standardError, 0) ;
      }
      else if (sCompl[0] == '$')
      {
        erreur("Cet élément comporte un attribut complément.", standardError, 0) ;
      }
      else if (sCompl == "WPLUR")
      {
        pSingulier->Uncheck() ;
        pPlur1->Check() ;
      }
      else if (sCompl == "WPLUS")
      {
        pSingulier->Uncheck() ;
        pPlur2->Check() ;
      }
      else if (string(sCompl, 0, 3) == "WCE")
      {
        sCompl = string(sCompl, 3, 2) ;
        pCertSans->Uncheck() ;

        if (sCompl == "00")
        	pCert00->Check() ;
        else if (sCompl == "25")
        	pCert25->Check() ;
        else if (sCompl == "50")
        	pCert50->Check() ;
        else if (sCompl == "75")
        	pCert75->Check() ;
        else if (sCompl == "A0")
        	pCert100->Check() ;
        else
        	erreur("Cet élément comporte un attribut de certitude incorrect.", standardError, 0) ;
      }
      else
        erreur("Cet élément comporte un attribut inconnu.", standardError, 0) ;

      pos1 = pos2 ;
    }
  }
  else
  {
    sRacine = sCode ;
    pCertSans->Check() ;
    pSingulier->Check() ;
  }

  pEdit->setLabel(sRacine, sText) ;
  pEdit->SetFocus() ;
}

void
NSModifFilGuideDialog::UncheckAll()
{
    pCert00->Uncheck();
    pCert25->Uncheck();
    pCert50->Uncheck();
    pCert75->Uncheck();
    pCert100->Uncheck();
    pCertSans->Uncheck();
    pPlur1->Uncheck();
    pPlur2->Uncheck();
    pSingulier->Uncheck();
}

voidNSModifFilGuideDialog::CmTouteSequence()
{
	if (bPere)
  {
  	if (pTouteSequence->GetCheck() == BF_CHECKED)
    {
    	pTouteSequence->Uncheck() ;
      pEdit->setLabel("") ;
    }
    else
    {
    	pTouteSequence->Check() ;
      pToutElement->Uncheck() ;
      UncheckAll() ;
      pSingulier->Check() ;
      pCertSans->Check() ;
      pEdit->setLabel("~~***1", "Toute Séquence (**)") ;
    }
  }

  pEdit->SetFocus() ;
}

voidNSModifFilGuideDialog::CmToutElement()
{
    if (bPere)
    {
        if (pToutElement->GetCheck() == BF_CHECKED)
        {
            pToutElement->Uncheck();
            pEdit->setLabel("");
        }
        else
        {
            pToutElement->Check();
            pTouteSequence->Uncheck();
            UncheckAll();
            pSingulier->Check();
            pCertSans->Check();
            pEdit->setLabel("~????1", "Tout élément (??)");
        }
    }

    pEdit->SetFocus();
}

voidNSModifFilGuideDialog::CmMajCode()
{
    string sCodelex = pEdit->getCode();

    if ((pToutElement->GetCheck() == BF_CHECKED) ||
        (pTouteSequence->GetCheck() == BF_CHECKED) ||
        (sCodelex == "") || (sCodelex == "£?????"))
    {
        // on laisse dans l'état : sans certitude et sans pluriel
        UncheckAll();
        pCertSans->Check();
        pSingulier->Check();
    }

    pEdit->SetFocus();
}

voidNSModifFilGuideDialog::CmOk()
{
    string sCompl1, sCompl2;

    sCode = pEdit->getCode();
    if (sCode == "£?????")
    {
        erreur("Attention vous ne pouvez pas insérer du texte libre...", standardError, 0) ;
        return;
    }

    // Complément Pluriel
    if (pPlur1->GetCheck() == BF_CHECKED)
        sCompl1 = "/WPLUR1";
    else if (pPlur2->GetCheck() == BF_CHECKED)
        sCompl1 = "/WPLUS1";
    else
        sCompl1 = "";

    // Complément Certitude
    if (pCert00->GetCheck() == BF_CHECKED)
        sCompl2 = "/WCE001";
    else if (pCert25->GetCheck() == BF_CHECKED)
        sCompl2 = "/WCE251";
    else if (pCert50->GetCheck() == BF_CHECKED)
        sCompl2 = "/WCE501";
    else if (pCert75->GetCheck() == BF_CHECKED)
        sCompl2 = "/WCE751";
    else if (pCert100->GetCheck() == BF_CHECKED)
        sCompl2 = "/WCEA01";
    else
        sCompl2 = "";

    // note : on laisse la possibilité d'avoir un code vide en sortie
    sCode = sCode + sCompl1 + sCompl2;

    TDialog::CmOk();
}

void
NSModifFilGuideDialog::CmCancel()
{
    TDialog::CmCancel();
}

// -----------------------------------------------------------------
//
//  Méthodes de NSListFGWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListFGWindow, TListWindow)
    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Function: NSListFGWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
//
//  Arguments:	  les modKeys et le point clické
//
//  Description: Fonction de réponse au click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListFGWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TListWindow::EvLButtonDown(modKeys, point);

    TLwHitTestInfo info(point);
    HitTest(info);

    if (info.GetFlags() & LVHT_ONITEM)
    {
        if (iRes == IDC_LW_PERE)
            pDlg->CmModPere();
        else if (iRes == IDC_LW_FILS)
            pDlg->CmModFils();
    }
}

//---------------------------------------------------------------------------
//  Function: NSListFGWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListFGWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)
   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSEditFilGuideDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSEditFilGuideDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_COMMAND(IDC_EDIT_CHOIXGD, ChoixGroupe),

    EV_COMMAND(IDC_HAUT_PERE, CmHautPere),
    EV_COMMAND(IDC_BAS_PERE, CmBasPere),
    EV_COMMAND(IDC_INS_PERE, CmInsPere),
    EV_COMMAND(IDC_MOD_PERE, CmModPere),
    EV_COMMAND(IDC_DEL_PERE, CmDelPere),

    EV_COMMAND(IDC_HAUT_FILS, CmHautFils),
    EV_COMMAND(IDC_BAS_FILS, CmBasFils),
    EV_COMMAND(IDC_INS_FILS, CmInsFils),
    EV_COMMAND(IDC_MOD_FILS, CmModFils),
    EV_COMMAND(IDC_DEL_FILS, CmDelFils),
END_RESPONSE_TABLE;

NSEditFilGuideDialog::NSEditFilGuideDialog(TWindow* pere, NSContexte* pCtx,
                                            string codeGroupe, string codeFilGuide)
                         :NSUtilDialog(pere, pCtx, "IDD_EDITFG", pNSResModule)
{
    pEditComment   = new NSUtilEdit(pContexte, this, IDC_EDIT_COMMENT, BB_COMMENTAIRE_LEN);
    pEditGroupe    = new TStatic(this, IDC_EDIT_CHOIXGD);
    pListePere     = new NSListFGWindow(this, IDC_LW_PERE);
    pListeFils     = new NSListFGWindow(this, IDC_LW_FILS);
    pPereArray     = new NSEltArray;
    pFilsArray     = new NSEltArray;
    nbPere = 0;
    nbFils = 0;

    sCodeGroupe = codeGroupe;
    sCodeFilGuide = codeFilGuide;
    sCheminPere = "";

    // si le groupe n'est pas précisé, on autorise le choix d'un groupe
    if (sCodeGroupe == "")
        bChoixGroupe = true;
    else
        bChoixGroupe = false;
}

NSEditFilGuideDialog::~NSEditFilGuideDialog()
{
    delete pEditComment;
    delete pEditGroupe;
    delete pListePere;
    delete pListeFils;
    delete pPereArray;
    delete pFilsArray;
}

void
NSEditFilGuideDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow();

    InitListePere();
    InitListeFils();
    AfficheNomGroupe();

    if (sCodeFilGuide != "")
    {
        if (ChargeFilGuide())
        {
            AfficheListePere();
            AfficheListeFils();
        }
        else
            erreur("Impossible de charger le fil guide recherché.",standardError, 0) ;
    }
    else
    {
        AfficheListePere();
        AfficheListeFils();
    }
}

voidNSEditFilGuideDialog::InitListePere()
{
	TListWindColumn colElt("Père", 500, TListWindColumn::Left, 0);
  	pListePere->InsertColumn(0, colElt);
}

void
NSEditFilGuideDialog::InitListeFils()
{
	TListWindColumn colFils("Fils", 500, TListWindColumn::Left, 0);
  	pListeFils->InsertColumn(0, colFils);
}

void
NSEditFilGuideDialog::AfficheListePere()
{
  pListePere->DeleteAllItems() ;

	for (int i = nbPere - 1; i >= 0; i--)
  {
    char elt[1024] ;
    sprintf(elt, "%s", (((*pPereArray)[i])->getLabel()).c_str()) ;

    TListWindItem Item(elt, 0) ;
    pListePere->InsertItem(Item) ;
  }

  pListePere->SetFocus() ;
}

void
NSEditFilGuideDialog::AfficheListeFils()
{
  pListeFils->DeleteAllItems() ;

  for (int i = nbFils - 1; i >= 0; i--)
  {
    char elt[1024] ;
    sprintf(elt, "%s", (((*pFilsArray)[i])->getLabel()).c_str()) ;

    TListWindItem Item(elt, 0) ;
    pListeFils->InsertItem(Item) ;
  }

  pListeFils->SetFocus() ;
}


// -----------------------------------------------------------------------------
// Fonction  : TrouveLibelle(string sCode, string& sLibelle, bool bPere)
// En entrée : un code lexique ou un ensemble de codes lexiques séparés par '/'
// En sortie : le libellé ou l'ensemble de libellés correspondant
// -----------------------------------------------------------------------------
void
NSEditFilGuideDialog::TrouveLibelle(string sCode, string& sLibelle, bool bPere)
{
  size_t pos1, pos2 ;
  bool bContinue = true ;
  string sCode1, sLibelle1 ;
  string sCode2, sLibelle2 ;

  // on remet à "" sLibelle
  sLibelle = "" ;

  pos1 = 0 ;
  pos2 = sCode.find('/') ;
  if (pos2 != string::npos)
  {
    while (bContinue)
    {
      if (pos2 != string::npos)
        sCode1 = string(sCode, pos1, pos2 - pos1) ;
      else
        sCode1 = string(sCode, pos1, strlen(sCode.c_str()) - pos1) ;

      TrouveLibelle(sCode1, sLibelle1, bPere) ;
      if (sLibelle == "")
        sLibelle = sLibelle1 ;
      else
        sLibelle += string("/") + sLibelle1 ;

      if ((pos2 == string::npos) || (pos2 == (strlen(sCode.c_str()) - 1)))
        bContinue = false ;
      else
      {
        pos1 = pos2 + 1 ;
        pos2 = sCode.find('/', pos1) ;
      }
    }
    return ;
  }

  string sLang = string("") ;
  if (pContexte)
  	sLang = pContexte->getUserLanguage() ;

  // on traite d'abord les jokers (codes peres)
  if (bPere && (sCode == "~????1"))
    sLibelle = "Tout élément (??)" ;
  else if (bPere && (sCode == "~~***1"))
    sLibelle = "Toute séquence (**)" ;
  else if (bPere && (sCode != "") && (sCode[0] == '~'))
    sLibelle = string("Joker (") + string(sCode,3,2) + string(")") ;
  else    // cas des codes commun aux fils et aux peres (codes lexiques)
  {
    if (sCode == "")
      sLibelle = "[Vide]" ;
    else
    {
      if (strlen(sCode.c_str()) == BASE_LEXI_LEN)
      {
        sCode2 = sCode ;
        sLibelle2 = "" ;
        if (pContexte->getDico()->donneLibelle(sLang, &sCode2, &sLibelle2))
        {
          // pour les codes système
          if (sLibelle2 == "")
          {
            pContexte->getDico()->donneLibelleLexique(sLang, &sCode2, &sLibelle2) ;

            // pour les cas non traités
            if (sLibelle2 == "")
              sLibelle = sCode ;
            else
              sLibelle = sLibelle2 ;
          }
          else // cas normal
          	sLibelle = sLibelle2 ;
        }
        else // cas d'erreur : code introuvable
        	sLibelle = sCode ;
      }
      else // cas d'erreur : code trop long
      	sLibelle = sCode ;
    }
  }
}

// -----------------------------------------------------------------------------
// Fonction    : ParsingChemin(string sChem, bool bPere)
// En entrée   : un chemin de codes lexiques ou de codes sens et un booléen
// Description : charge l'array des codes père ou fils avec les éléments du chemin
// -----------------------------------------------------------------------------
void
NSEditFilGuideDialog::ParsingChemin(string sChem, bool bPere)
{
  string sCode, sCodeElt ;
  string sLibelle, sLibelleElt ;
  size_t pos1, pos2 ;
  char sep ;
  bool bVide = true ;
  bool bParsing = true ;

  if (bPere)
    sep = '/' ;
  else
    sep = '|' ;

  sCodeElt = "" ;
  sLibelleElt = "" ;

  pos1 = 0 ;
  pos2 = sChem.find(sep) ;

  while (bParsing)
  {
    sCode = "" ;
    sLibelle = "" ;

    if (pos2 != string::npos)
      sCode = string(sChem, pos1, pos2 - pos1) ;
    else
      sCode = string(sChem, pos1, strlen(sChem.c_str()) - pos1) ;

    // Chaque sElement récupéré peut être un code sens
    // => on le transforme en code lexique
    if ((sCode != "") && (strlen(sCode.c_str()) < BASE_LEXI_LEN))
    	pContexte->getDico()->donneCodeComplet(sCode) ;

    TrouveLibelle(sCode, sLibelle, bPere) ;

    // cas des attributs
    if ((sCode[0] == '$') || (string(sCode, 0, 3) == "WCE") || (string(sCode, 0, 4) == "WPLU"))
    {
      // on stocke alors le code et le libelle
      // dans l'élément en cours
      if (bVide)
      {
        sCodeElt = sCode ;
        sLibelleElt = sLibelle ;
        bVide = false ;
      }
      else
      {
        sCodeElt += string("/") + sCode ;
        sLibelleElt += string("/") + sLibelle ;
      }
    }
    else
    {
      if (!bVide)
      {
        if (bPere)
        {
          pPereArray->push_back(new NSElement(sCodeElt, sLibelleElt)) ;
          nbPere++ ;
        }
        else
        {
          pFilsArray->push_back(new NSElement(sCodeElt, sLibelleElt)) ;
          nbFils++ ;
        }
      }
      sCodeElt = sCode ;
      sLibelleElt = sLibelle ;
      bVide = false ;
    }

    // condition d'arret : on traite aussi le cas du '/' à la fin du chemin
    if ((pos2 == string::npos) || (pos2 == (strlen(sChem.c_str()) - 1)))
    {
      bParsing = false ;
    }
    else
    {
      pos1 = pos2 + 1 ;
      pos2 = sChem.find(sep, pos1) ;
    }
  }

  // on traite le dernier élément (éventuellement : chemin vide)
  if (bPere)
  {
    pPereArray->push_back(new NSElement(sCodeElt, sLibelleElt)) ;
    nbPere++ ;
  }
  else
  {
    pFilsArray->push_back(new NSElement(sCodeElt, sLibelleElt)) ;
    nbFils++ ;
  }
}

bool
NSEditFilGuideDialog::ChargeFilGuide()
{
  string sChemin;
  string sFils;
  string sCode;

  //
  // On récupère le fil guide de code sCodeFilGuide
  // Création d'un BBFiche
  //
  BBFiche dbBBFiche(pContexte->getSuperviseur()) ;
  dbBBFiche.lastError = dbBBFiche.open() ;
  if (dbBBFiche.lastError != DBIERR_NONE)
  {
    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, dbBBFiche.lastError) ;
    return false ;
  }

  // On cherche les fils guides de groupe sCode
  sCode = sCodeGroupe + sCodeFilGuide ;
  dbBBFiche.lastError = dbBBFiche.chercheClef(&sCode,
                                        "",
                                        0,
                                        keySEARCHEQ,
                                        dbiWRITELOCK) ;

  if ((dbBBFiche.lastError != DBIERR_NONE) && (dbBBFiche.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, dbBBFiche.lastError) ;
    dbBBFiche.close() ;
    return false ;
  }
  else if (dbBBFiche.lastError == DBIERR_RECNOTFOUND)
  {
    erreur("Impossible de trouver le fil guide recherché dans la base des guides.", standardError, 0) ;
    dbBBFiche.close() ;
    return false;
  }

  // on est sur le bon fil guide
  dbBBFiche.lastError = dbBBFiche.getRecord() ;
  if (dbBBFiche.lastError != DBIERR_NONE)
  {
    erreur("Erreur à la lecture d'une fiche Guides.", standardError, dbBBFiche.lastError) ;
    dbBBFiche.close() ;
    return false ;
  }

  // On affiche le champ commentaire
  pEditComment->SetText((static_cast<BBItemData*>(dbBBFiche.getData()))->commentaire);

  // On récupère ici le chemin du fil guide et ses fils
  sChemin = dbBBFiche.getSemanticPath();
  sFils = string((static_cast<BBItemData*>(dbBBFiche.getData()))->fils);

  // on charge d'abord le tableau Pere
  ParsingChemin(sChemin, true);

  // on charge maintenant le tableau Fils
  ParsingChemin(sFils, false);

  // on ferme la base des guides
  dbBBFiche.lastError = dbBBFiche.close() ;
  if (dbBBFiche.lastError != DBIERR_NONE)
    erreur("Erreur de fermeture de la base des Guides.", standardError, dbBBFiche.lastError) ;

  return true ;
}

// ************** Controles de la boite Pere
void
NSEditFilGuideDialog::CmBasPere()
{
    int eltChoisi = pListePere->IndexItemSelect();

    if (eltChoisi == -1)
    {
        erreur("Vous devez sélectionner un élément à déplacer.", standardError, 0) ;
        return;
    }

    if (eltChoisi == (nbPere - 1))
        return;

    NSElement* pElement = new NSElement(*((*pPereArray)[eltChoisi]));

    *((*pPereArray)[eltChoisi]) = *((*pPereArray)[eltChoisi + 1]);
    *((*pPereArray)[eltChoisi + 1]) = *pElement;

    AfficheListePere();
    pListePere->SetSel(eltChoisi + 1, true);

    delete pElement;}
voidNSEditFilGuideDialog::CmHautPere()
{
    int eltChoisi = pListePere->IndexItemSelect();

    if (eltChoisi == -1)
    {
        erreur("Vous devez sélectionner un élément à déplacer.", standardError, 0) ;
        return;
    }

    if (eltChoisi == 0)
        return;

    NSElement* pElement = new NSElement(*((*pPereArray)[eltChoisi]));

    *((*pPereArray)[eltChoisi]) = *((*pPereArray)[eltChoisi - 1]);
    *((*pPereArray)[eltChoisi - 1]) = *pElement;

    AfficheListePere();
    pListePere->SetSel(eltChoisi - 1, true);
    delete pElement;
}

void
NSEditFilGuideDialog::CmInsPere()
{
    // Insertion en queue d'un nouvel élément
    NSModifFilGuideDialog* pModDlg =
        new NSModifFilGuideDialog(this, "", pContexte, pNSResModule);

    if (pModDlg->Execute() == IDOK)
    {
        string sCode = pModDlg->sCode;        string sLibelle;
        TrouveLibelle(sCode, sLibelle, true);
        pPereArray->push_back(new NSElement(sCode, sLibelle));
        nbPere++;
        AfficheListePere();
    }

    delete pModDlg;
}

voidNSEditFilGuideDialog::CmModPere()
{
  int eltChoisi = pListePere->IndexItemSelect() ;

  if (-1 == eltChoisi)  {
    erreur("Vous devez sélectionner un élément à modifier.", standardError, 0) ;
    return ;
  }

  NSElement element = *((*pPereArray)[eltChoisi]) ;

  NSModifFilGuideDialog* pModDlg =
        new NSModifFilGuideDialog(this, element.getCode(), pContexte, pNSResModule) ;

  if (pModDlg->Execute() == IDOK)
  {
    string sCode = pModDlg->sCode ;
    string sLibelle ;

    TrouveLibelle(sCode, sLibelle, true) ;

    element.setCode(sCode) ;
    element.setLabel(sLibelle) ;
    *((*pPereArray)[eltChoisi]) = element ;
    AfficheListePere() ;
  }

  delete pModDlg ;
}

void
NSEditFilGuideDialog::CmDelPere()
{
  int eltChoisi = pListePere->IndexItemSelect() ;
  if (-1 == eltChoisi)
  {
    erreur("Vous devez sélectionner un élément à détruire.", standardError, 0) ;
    return ;
  }

  int j = 0 ;
  for (NSEltIter i = pPereArray->begin() ; pPereArray->end() != i ; i++, j++)
  {
    if (j == eltChoisi)
    {
      delete *i ;
      pPereArray->erase(i) ;
      nbPere-- ;
      break ;
    }
  }

  AfficheListePere() ;
}

// ******************** Controles de la boite fils

void
NSEditFilGuideDialog::CmBasFils()
{
  int eltChoisi = pListeFils->IndexItemSelect() ;
  if (-1 == eltChoisi)
  {
    erreur("Vous devez sélectionner un élément à déplacer.", standardError, 0) ;
    return ;
  }

  if (nbFils - 1 == eltChoisi)
    return ;

  NSElement element(*((*pFilsArray)[eltChoisi])) ;

  *((*pFilsArray)[eltChoisi])     = *((*pFilsArray)[eltChoisi + 1]) ;
  *((*pFilsArray)[eltChoisi + 1]) = element ;

  AfficheListeFils() ;
  pListeFils->SetSel(eltChoisi + 1, true) ;
}

voidNSEditFilGuideDialog::CmHautFils()
{
  int eltChoisi = pListeFils->IndexItemSelect() ;
  if (eltChoisi == -1)
  {
    erreur("Vous devez sélectionner un élément à déplacer.", standardError, 0) ;
    return ;
  }

  if (0 == eltChoisi)    return ;

  NSElement element(*((*pFilsArray)[eltChoisi])) ;
  *((*pFilsArray)[eltChoisi]) = *((*pFilsArray)[eltChoisi - 1]) ;  *((*pFilsArray)[eltChoisi - 1]) = element ;

  AfficheListeFils() ;
  pListeFils->SetSel(eltChoisi - 1, true) ;}

void
NSEditFilGuideDialog::CmInsFils()
{
  // Insertion en queue d'un nouvel élément
  NSModifFilGuideDialog* pModDlg =
        new NSModifFilGuideDialog(this, "", pContexte, pNSResModule, false) ;
  if (pModDlg->Execute() == IDOK)  {
    string sCode = pModDlg->sCode ;
    string sLibelle ;
    TrouveLibelle(sCode, sLibelle, false) ;

    pFilsArray->push_back(new NSElement(sCode, sLibelle)) ;    nbFils++ ;

    AfficheListeFils() ;  }
  delete pModDlg ;
}

voidNSEditFilGuideDialog::CmModFils()
{
  int eltChoisi = pListeFils->IndexItemSelect() ;

  if (-1 == eltChoisi)  {
    erreur("Vous devez sélectionner un élément à modifier.", standardError, 0) ;
    return ;
  }

  NSElement element = *((*pFilsArray)[eltChoisi]) ;
  NSModifFilGuideDialog* pModDlg =        new NSModifFilGuideDialog(this, element.getCode(), pContexte, pNSResModule, false) ;
  if (pModDlg->Execute() == IDOK)
  {
    string sCode = pModDlg->sCode ;

    string sLibelle ;    TrouveLibelle(sCode, sLibelle, false);

    element.setCode(sCode) ;    element.setLabel(sLibelle) ;

    *((*pFilsArray)[eltChoisi]) = element ;
    AfficheListeFils() ;  }

  delete pModDlg ;}

voidNSEditFilGuideDialog::CmDelFils()
{
  int eltChoisi = pListeFils->IndexItemSelect() ;

  if (-1 == eltChoisi)
  {    erreur("Vous devez sélectionner un élément à détruire.", standardError, 0) ;
    return ;
  }
  int j = 0 ;
  for (NSEltIter i = pFilsArray->begin() ; pFilsArray->end() != i ; i++, j++)  {
    if (j == eltChoisi)    {
      delete *i ;
      pFilsArray->erase(i) ;
      nbFils-- ;
      break ;
    }
  }

  AfficheListeFils() ;
}

voidNSEditFilGuideDialog::ChoixGroupe(){
  if (false == bChoixGroupe)
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    MessageBox("Le groupe n'est pas modifiable.", sCaption.c_str(), MB_OK) ;
    return ;
  }

  // Choix du groupe où placer ce fil guide (en mode utilOnly)
  NSChoixGdDialog* pChoixGdDlg = new NSChoixGdDialog(this, pContexte, true) ;

  if (pChoixGdDlg->Execute() == IDOK)
  {
    int index = pChoixGdDlg->GroupChoisi;
    if (-1 == index)
    {
      delete pChoixGdDlg ;
      return ;
    }

    NSGroupGdInfo* pGroupeChoisi = (pChoixGdDlg->GroupArray)[index] ;
    if (pGroupeChoisi)
      sCodeGroupe = string(pGroupeChoisi->_Donnees.groupe) ;
    AfficheNomGroupe() ;
  }

  delete pChoixGdDlg ;
}

void
NSEditFilGuideDialog::AfficheNomGroupe()
{
  if (string("") == sCodeGroupe)
    return ;

  // on vérifie l'existence du code groupe dans la base.
  // s'il existe, on affiche son nom sur le bouton de choix
  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (Group.lastError != DBIERR_NONE)
  {
    erreur("Impossible d'ouvrir la base GroupGd.db", standardError, Group.lastError) ;
    return ;
  }

  Group.lastError = Group.chercheClef(&sCodeGroupe, "", 0, keySEARCHEQ, dbiWRITELOCK) ;
  if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return ;
  }
  else if (Group.lastError == DBIERR_NONE)  // cas où le code existe
  {
    Group.lastError = Group.getRecord() ;
    if (Group.lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture dans la base GroupGd.db", standardError, Group.lastError) ;
      Group.close() ;
      return ;
    }

    pEditGroupe->SetText(Group._Donnees.libelle) ;
  }
  else // le code n'existe pas
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    MessageBox("Code groupe incorrect. Vous devez choisir un autre groupe.", sCaption.c_str(), MB_OK) ;
    sCodeGroupe = string("") ;
  }

  // on ferme la base
  Group.lastError = Group.close() ;
  if (Group.lastError != DBIERR_NONE)
    erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, Group.lastError) ;
}

void
NSEditFilGuideDialog::CmOk()
{
  string sBoutChemin ;
  string sCodeCompose ;
  string sCodeLexique ;
  string sCodeUtile ;
  string sCode ;
  size_t pos1, pos2 ;
  bool bContinue ;
  string sFils = "" ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  if (sCodeGroupe == "")
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    MessageBox("Vous devez choisir un groupe pour pouvoir enregistrer votre fil guide.", sCaption.c_str(), MB_OK) ;
    return ;
  }

  // On constitue la string sChemin à partir de l'array Père
  for (NSEltIter i = pPereArray->begin() ; i != pPereArray->end() ; i++)
  {
    sCodeCompose = (*i)->getCode() ;

    sBoutChemin = "" ;
    bContinue = true ;
    pos1 = 0 ;
    pos2 = sCodeCompose.find('/') ;

    while (bContinue)
    {
      if (pos2 != string::npos)
        sCodeLexique = string(sCodeCompose, pos1, pos2 - pos1) ;
      else
        sCodeLexique = string(sCodeCompose, pos1, strlen(sCodeCompose.c_str()) - pos1) ;

      NSDico::donneCodeSens(&sCodeLexique, &sCodeUtile) ;
      if (sBoutChemin != "")
        sBoutChemin += "/" ;
      sBoutChemin += sCodeUtile ;

      // condition d'arret : on traite aussi le cas du '/' à la fin du chemin
      if ((pos2 == string::npos) || (pos2 == (strlen(sCodeCompose.c_str()) - 1)))
      {
        bContinue = false ;
      }
      else
      {
        pos1 = pos2 + 1 ;
        pos2 = sCodeCompose.find('/', pos1) ;
      }
    }

    if (sCheminPere != "")
      sCheminPere += "/" ;
    sCheminPere += sBoutChemin ;
  }

  // On constitue la string sFils à partir de l'array Fils
  for (NSEltIter i = pFilsArray->begin() ; i != pFilsArray->end() ; i++)
  {
    if (sFils != "")
    	sFils += "|" ;
    sFils += (*i)->getCode() ;
  }

  //
  // Création d'un BBFiche
  //
  BBFiche* pBBFiche = new BBFiche(pContexte->getSuperviseur()) ;
  pBBFiche->lastError = pBBFiche->open() ;
  if (pBBFiche->lastError != DBIERR_NONE)
  {
    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, pBBFiche->lastError) ;
    delete pBBFiche ;
    return ;
  }

  if (sCodeFilGuide != "")
  {
    // cas de la modification
    sCode = sCodeGroupe + sCodeFilGuide ;
    pBBFiche->lastError = pBBFiche->chercheClef(&sCode, "", 0, keySEARCHEQ, dbiWRITELOCK) ;
    if ((pBBFiche->lastError != DBIERR_NONE) && (pBBFiche->lastError != DBIERR_RECNOTFOUND))
    {
      erreur("Erreur à la recherche dans le fichier des Guides.", standardError, pBBFiche->lastError) ;
      pBBFiche->close() ;
      delete pBBFiche ;
      return ;
    }
    else if (pBBFiche->lastError == DBIERR_RECNOTFOUND)
    {
      erreur("Le fil guide à modifier n'existe pas dans la base des guides.", standardError, pBBFiche->lastError);
      pBBFiche->close();
      delete pBBFiche;
      return;
    }

    pBBFiche->lastError = pBBFiche->getRecord();
    if (pBBFiche->lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture d'une fiche Guides.", standardError, pBBFiche->lastError) ;
      pBBFiche->close();
      delete pBBFiche;
      return;
    }

    // on caste à cause de la structure de BBFiche (voir nsguide.cpp)
    string sCommentaire = pEditComment->GetText(BB_COMMENTAIRE_LEN) ;

    strcpy((static_cast<BBItemData*>(pBBFiche->getData()))->chemin,      sCheminPere.c_str()) ;
    strcpy((static_cast<BBItemData*>(pBBFiche->getData()))->commentaire, sCommentaire.c_str()) ;
    strcpy((static_cast<BBItemData*>(pBBFiche->getData()))->fils,        sFils.c_str());

    pBBFiche->lastError = pBBFiche->modifyRecord(TRUE);
    if (pBBFiche->lastError != DBIERR_NONE)
    {
        erreur("Erreur à la modification du fil guide.", standardError, pBBFiche->lastError) ;
        pBBFiche->close();
        delete pBBFiche;
        return;
    }
  }
    else
    {
        // cas de la création d'un nouveau fil guide
        // on cherche d'abord s'il existe des fils guide utilisateur
        // appartenant au même groupe
        string sCodePrecedent = "000000";
        string sCompteur;

        sCode = sCodeGroupe + string(CH_CODE_LEN, ' ');
        pBBFiche->lastError = pBBFiche->chercheClef(&sCode,
                                                "",
                                                0,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK);

        if ((pBBFiche->lastError != DBIERR_NONE) && (pBBFiche->lastError != DBIERR_EOF))
        {
            erreur("Erreur à la recherche dans le fichier des Guides.", standardError, pBBFiche->lastError) ;
            pBBFiche->close();
            delete pBBFiche;
            return;
        }

        // on cherche le dernier fil guide du groupe pour incrémenter le code
        while (pBBFiche->lastError != DBIERR_EOF)
        {
            pBBFiche->lastError = pBBFiche->getRecord();
            if (pBBFiche->lastError != DBIERR_NONE)
            {
                erreur("Erreur à la lecture d'une fiche Guides.", standardError, pBBFiche->lastError) ;
                pBBFiche->close();
                delete pBBFiche;
                return;
            }

            // Condition d'arret
            if (pBBFiche->getGroupID() != sCodeGroupe)
                break;

            sCodePrecedent = pBBFiche->getID() ;

            pBBFiche->lastError = pBBFiche->suivant(dbiWRITELOCK);
            if ((pBBFiche->lastError != DBIERR_NONE) && (pBBFiche->lastError != DBIERR_EOF))
            {
                erreur("Erreur d'accès à la fiche Guides suivante.", standardError, pBBFiche->lastError) ;
                pBBFiche->close();
                delete pBBFiche;
                return;
            }
        } // fin du while

        sCompteur = sCodePrecedent;
        if (!(pBBFiche->IncrementeCode(&sCompteur)))
        {
            erreur("Erreur à l'attribution du code pour le nouveau fil guide.", standardError, 0) ;
            pBBFiche->close();
            delete pBBFiche;
            return;
        }

        sCodeFilGuide = sCompteur;

        // on copie les valeurs dans le nouveau fil guide
        // on caste à cause de la structure de BBFiche (voir nsguide.cpp)
        string sCommentaire = pEditComment->GetText(BB_COMMENTAIRE_LEN) ;
        
        BBItemData* pItemData = static_cast<BBItemData*>(pBBFiche->getData()) ;
        if (pItemData)
        {
          pItemData->metAZero() ;
          pItemData->setID(sCodeFilGuide) ;
          pItemData->setGroupID(sCodeGroupe) ;
          pItemData->setSemanticPath(sCheminPere) ;
          pItemData->setLabel(sCommentaire) ;
          pItemData->setOpenDialog(false) ;
          pItemData->setLevelShift(string("+01+01")) ;
          pItemData->setSonsList(sFils) ;

          pBBFiche->lastError = pBBFiche->appendRecord();
          if (pBBFiche->lastError != DBIERR_NONE)
          {
            erreur("Erreur à l'ajout du nouveau fil guide.", standardError, pBBFiche->lastError) ;
            pBBFiche->close();
            delete pBBFiche;
            return;
          }
        }
    } // fin du else (création)

	pBBFiche->lastError = pBBFiche->close() ;
	if (pBBFiche->lastError != DBIERR_NONE)
		erreur("Erreur de fermeture de la base des Guides.", standardError, pBBFiche->lastError) ;

	delete pBBFiche ;

  TDialog::CmOk() ;
}

void
NSEditFilGuideDialog::CmCancel()
{
	TDialog::CmCancel() ;
}


/*****************************************************************************/
/***                    GESTION DES FICHIERS DE FILS GUIDES                ***/
/*****************************************************************************/

NSGuidesManager::NSGuidesManager(NSContexte* pCtx)                :NSRoot(pCtx){	sFile = string("") ;	init() ;}
NSGuidesManager::~NSGuidesManager()
{
}

void
NSGuidesManager::setImportFile(string sNewFile)
{
	sFile = sNewFile ;
	init() ;
}

void
NSGuidesManager::init()
{
	bValidHeader = false ;

	sFileVersion = string("") ;
	sGroup       = string("") ;
	sLabel       = string("") ;
	sGroupDate   = string("") ;
	sUser        = string("") ;

	if (string("") != sFile)
		readHeader() ;
}

bool
NSGuidesManager::doesGroupExist(string sCode, bool& bError, string& sDate)
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (Group.lastError != DBIERR_NONE)
  {
  	string sErrLabel = pSuper->getText("filsGuidesManagement", "cannotOpenFilsGuidesGroupsTable") ;
    erreur(sErrLabel.c_str(), standardError, Group.lastError) ;
    bError = true ;
    return false ;
  }

  Group.lastError = Group.chercheClef(&sCode, "", 0, keySEARCHEQ, dbiWRITELOCK) ;  if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_RECNOTFOUND))
  {
  	string sErrLabel = pSuper->getText("filsGuidesManagement", "searchErrorInGuidesGroupsTable") ;
    erreur(sErrLabel.c_str(), standardError, Group.lastError) ;
    Group.close() ;
    bError = true ;
    return false ;
  }
  else if (Group.lastError == DBIERR_NONE)  // cas où le code existe
  {
    Group.lastError = Group.getRecord() ;
    if (Group.lastError != DBIERR_NONE)
    {
    	string sErrLabel = pSuper->getText("filsGuidesManagement", "readErrorInGuidesGroupsTable") ;
    	erreur(sErrLabel.c_str(), standardError, Group.lastError) ;
      Group.close() ;
      bError = true ;
      return true ;
    }

    string sLang = pContexte->getUserLanguage() ;

    sDate = donne_date(Group._Donnees.date, sLang) ;
  }
  else // le code n'existe pas
  {
    sDate = string("") ;
    Group.lastError = Group.close() ;
    bError = false ;
    return false ;
  }

  // on ferme la base
  Group.lastError = Group.close() ;
  bError = false ;
  return true ;
}

//---------------------------------------------------------------------------
//  Description: Remove the group from Fils Guides groups' file
//               Remove all Fils Guides from the table
//               Returns false in case of database error
//---------------------------------------------------------------------------
bool
NSGuidesManager::deleteGroup(string sGroupName)
{
	bool bSuccess = deleteGuidesFromGroup(sGroupName) ;
  if (true == bSuccess)
  	bSuccess == unreferenceGroup(sGroupName) ;

	return bSuccess ;
}

bool
NSGuidesManager::deleteGuidesFromGroup(string sGroupName)
{
	if (string("") == sGroupName)
		return false ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

  //
  // Création d'un BBFiche
  //
  BBFiche Fiche(pContexte->getSuperviseur()) ;
  Fiche.lastError = Fiche.open() ;
  if (Fiche.lastError != DBIERR_NONE)
  {
		string sErrLabel = pSuper->getText("filsGuidesManagement", "cannotOpenFilsGuidesTable") ;
		erreur(sErrLabel.c_str(), standardError, Fiche.lastError) ;
    return false ;
  }

  // Looking for Fils Guides of this group to delete them  //  string sCodeFG = sGroupName + string(CH_CODE_LEN, ' ') ;
	Fiche.lastError = Fiche.chercheClef(&sCodeFG,
                                      "",
                                      0,
                                      keySEARCHGEQ,
                                      dbiWRITELOCK) ;

  if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_EOF))
  {
  	string sErrLabel = pSuper->getText("filsGuidesManagement", "searchErrorInGuidesTable") ;
		erreur(sErrLabel.c_str(), standardError, Fiche.lastError) ;
    Fiche.close() ;
		return false ;
  }

  // on boucle pour détruire tous les fils guides de type sCode
  while (Fiche.lastError != DBIERR_EOF)
  {
  	Fiche.lastError = Fiche.getRecord() ;
    if (Fiche.lastError != DBIERR_NONE)
    {
    	string sErrLabel = pSuper->getText("filsGuidesManagement", "readErrorInGuidesTable") ;
			erreur(sErrLabel.c_str(), standardError, Fiche.lastError) ;
      Fiche.close() ;
      return false ;
    }

    // Condition d'arret
    if (Fiche.getData()->getGroupID() != sGroupName)
    	break ;


    Fiche.lastError = Fiche.deleteRecord() ;    if (Fiche.lastError != DBIERR_NONE)    {
    	string sErrLabel = pSuper->getText("filsGuidesManagement", "deleteErrorInGuidesTable") ;
			erreur(sErrLabel.c_str(), standardError, Fiche.lastError) ;
      Fiche.close() ;
      return false ;
    }

    Fiche.lastError = Fiche.suivant(dbiWRITELOCK);
    if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_EOF))
    {
    	string sErrLabel = pSuper->getText("filsGuidesManagement", "errorAccessingNextGuideInTable") ;
			erreur(sErrLabel.c_str(), standardError, Fiche.lastError) ;
      Fiche.close() ;
      return false ;
    }
	}

	Fiche.lastError = Fiche.close() ;

	return true ;
}

bool
NSGuidesManager::unreferenceGroup(string sGroupName)
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	NSGroupGd Group(pContexte->getSuperviseur()) ;
	Group.lastError = Group.open() ;
	if (Group.lastError != DBIERR_NONE)
	{
		string sErrLabel = pSuper->getText("filsGuidesManagement", "cannotOpenFilsGuidesGroupsTable") ;
    erreur(sErrLabel.c_str(), standardError, Group.lastError) ;
    return false ;
	}

	Group.lastError = Group.chercheClef(&sGroupName,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

	if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_RECNOTFOUND))
	{
		string sErrLabel = pSuper->getText("filsGuidesManagement", "searchErrorInGuidesGroupsTable") ;
    erreur(sErrLabel.c_str(), standardError, Group.lastError) ;
    Group.close() ;
    return false ;
	}
	else if (Group.lastError == DBIERR_NONE)  // cas où le code existe
  {
		Group.lastError = Group.deleteRecord() ;
    if (Group.lastError != DBIERR_NONE)
    {
    	string sErrLabel = pSuper->getText("filsGuidesManagement", "deleteErrorInGuidesGroupsTable") ;
    	erreur(sErrLabel.c_str(), standardError, Group.lastError) ;
      Group.close() ;
      return false ;
    }
  }
  else // le code n'existe pas
  {
		string sErrLabel = pSuper->getText("filsGuidesManagement", "thisGroupDoesNotExistInGroupsTable") ;
    erreur(sErrLabel.c_str(), standardError, 0) ;
	}

	// on ferme la base
	Group.lastError = Group.close() ;

	return true ;
}


bool
NSGuidesManager::readHeader()
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	ifstream inFile;
	inFile.open(sFile.c_str()) ;
	if (!inFile)
  {
  	string sErrLabel = pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrLabel += string(" (") + sFile + string(")") ;
  	erreur(sErrLabel.c_str(), standardError, 0) ;
    return false ;
  }

	string sParam[6] = {"","","","","",""} ;

  // on récupère le header du fichier groupe
  if (!inFile.eof())
	{
		string sHeader ;
		getline(inFile, sHeader) ;

		if (string("") != sHeader)
  	{
			size_t i = 0, k = 0 ;

			while (i < strlen(sHeader.c_str()))
			{
				if ('\"' == sHeader[i])
					k++ ;
				else
				{
    			if (k < 6)
      			sParam[k] += sHeader[i] ;
      		else
      			break ;      	}

      	i++ ;
    	}
      if (5 == k)
				bValidHeader = true ;
  	}
  }
  inFile.close() ;

	if (false == bValidHeader)
	{
  	string sErrLabel = pSuper->getText("fileErrors", "corruptedFile") ;
    sErrLabel += string(" (") + sFile + string(")") ;
  	erreur(sErrLabel.c_str(), standardError, 0) ;
    return false ;
	}

	sFileVersion = sParam[0] ;
	sGroup       = sParam[2] ;
	sLabel       = sParam[3] ;
	sGroupDate   = sParam[4] ;
	sUser        = sParam[5] ;

	return true ;
}

bool
NSGuidesManager::importGroup(string sFileCompletePath)
{
	if (string("") != sFileCompletePath)
		setImportFile(sFileCompletePath) ;

	if ((string("") == sFile) || (false == bValidHeader))
		return false ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	ifstream inFile;
	inFile.open(sFile.c_str()) ;
	if (!inFile)
  {
  	string sErrLabel = pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrLabel += string(" (") + sFile + string(")") ;
  	erreur(sErrLabel.c_str(), standardError, 0) ;
    return false ;
  }
  if (inFile.eof())
  {
  	inFile.close() ;
		return false ;
	}

	// Skip header
	//
	string sHeader ;
	getline(inFile, sHeader) ;

  if (inFile.eof())
	{
		inFile.close() ;
		return false ;
	}

	//	// Create records in Fils Guides table
	//
	BBFiche Fiche(pContexte->getSuperviseur()) ;
	Fiche.lastError = Fiche.open() ;

	if (Fiche.lastError != DBIERR_NONE)	{
  	string sErrLabel = pSuper->getText("filsGuidesManagement", "cannotOpenFilsGuidesTable") ;
		erreur(sErrLabel.c_str(), standardError, Fiche.lastError) ;
		inFile.close() ;
		return false ;
	}

	string sData, sLine ;
  string sParam[18] ;

	while (!inFile.eof())	{
		getline(inFile, sLine) ;
		if (string("") != sLine)
		{
			for (int i = 0 ; i < 18 ; i++)
				sParam[i] = string("") ;

      size_t i = 0, k = 0 ;
      while (i < strlen(sLine.c_str()))
      {
        if ('\"' == sLine[i])
          k++ ;
        else
        {
          if (k < 18)
            sParam[k] += sLine[i] ;
          else
            break ;        }

        i++ ;
      }

      if (18 == k)
			{
        BBItemData* pItemData = static_cast<BBItemData*>(Fiche.getData()) ;
        if (pItemData)
        {
				  pItemData->metAZero() ;

          pItemData->setID(sParam[0]) ;          pItemData->setGroupID(sParam[1]) ;
          pItemData->setSemanticPath(sParam[2]) ;
          pItemData->setLabel(sParam[3]) ;
          pItemData->setDialogFile(sParam[4]) ;
          pItemData->setDialogName(sParam[5]) ;
          pItemData->setOpenDialog(sParam[6]) ;
          pItemData->setFctFile(sParam[7]) ;
          pItemData->setFctName(sParam[8]) ;
          pItemData->setLevelShift(sParam[9]) ;
          pItemData->setSonsList(sParam[10]) ;
          pItemData->setSonsRules(sParam[11]) ;
          pItemData->setAutomatic(sParam[12]) ;
          pItemData->setEmptyActivation(sParam[13]) ;
          pItemData->setUnicity(sParam[14]) ;
          pItemData->setNeeded(sParam[15]) ;
          pItemData->setSortRules(sParam[16]) ;
          pItemData->setValidityCase(sParam[17]) ;

          Fiche.lastError = Fiche.appendRecord() ;

          if (Fiche.lastError != DBIERR_NONE)          {
            string sErrLabel = pSuper->getText("filsGuidesManagement", "addErrorInGuidesTable") ;
            erreur(sErrLabel.c_str(), standardError, Fiche.lastError) ;
            Fiche.close() ;
            inFile.close() ;
            return false ;
          }
        }
			}
		}
	}

	inFile.close() ;

	Fiche.lastError = Fiche.close() ;  return true ;}

bool
NSGuidesManager::referenceGroup()
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	NSGroupGd Group(pContexte->getSuperviseur()) ;

	Group.lastError = Group.open() ;	if (Group.lastError != DBIERR_NONE)
	{
  	string sErrLabel = pSuper->getText("filsGuidesManagement", "cannotOpenFilsGuidesGroupsTable") ;
    erreur(sErrLabel.c_str(), standardError, Group.lastError) ;
    return false ;
	}

	strcpy(Group._Donnees.groupe,  sGroup.c_str()) ;	strcpy(Group._Donnees.libelle, sLabel.c_str()) ;
	strcpy(Group._Donnees.date,    sGroupDate.c_str()) ;
	strcpy(Group._Donnees.util,    sUser.c_str()) ;

	Group.lastError = Group.appendRecord() ;	if (Group.lastError != DBIERR_NONE)
	{
  	string sErrLabel = pSuper->getText("filsGuidesManagement", "addErrorInGuidesGroupsTable") ;
    erreur(sErrLabel.c_str(), standardError, Group.lastError) ;
    Group.close() ;
    return false ;
	}

	Group.lastError = Group.close() ;	return true ;
}
/***************************************************************************************//***                        GESTION DES GROUPES DE FILS GUIDES                       ***/
/***************************************************************************************/

DEFINE_RESPONSE_TABLE1(NSGroupGdDialog, NSUtilDialog)
	EV_CHILD_NOTIFY(IDC_GROUPGD_IMPORT, BN_CLICKED, CmImport),
	EV_CHILD_NOTIFY(IDC_GROUPGD_EXPORT, BN_CLICKED, CmExport),
	EV_CHILD_NOTIFY(IDC_GROUPGD_DESTROY, BN_CLICKED, CmDestroy),
	EV_CHILD_NOTIFY(IDC_GROUPGD_LISTFG, BN_CLICKED, CmGestion),
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSGroupGdDialog::NSGroupGdDialog(TWindow* pere, NSContexte* pCtx)
				  :NSUtilDialog(pere, pCtx, "IDD_GROUPGD", pNSResModule)
{
	pImport  = new TButton(this, IDC_GROUPGD_IMPORT) ;
	pExport  = new TButton(this, IDC_GROUPGD_EXPORT) ;
	pDestroy = new TButton(this, IDC_GROUPGD_DESTROY) ;
	pGestion = new TButton(this, IDC_GROUPGD_LISTFG) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSGroupGdDialog::~NSGroupGdDialog()
{
	//
	// Suppression de tous les objets
	//
	delete pImport ;
	delete pExport ;
	delete pDestroy ;
	delete pGestion ;
}

//---------------------------------------------------------------------------//  Fonction :   NSGroupGdDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NSGroupGdDialog::SetupWindow()
{
	// fichiers d'aide
	sHindex = "" ;
	sHcorps = "Fils_guides.html" ;
	NSUtilDialog::SetupWindow() ; 
}

// -----------------------------------------------------------------------------
// Fonction    : NSGroupGdDialog::ExisteCode(string sCode, bool& bExiste, string& sDate)
// Description : Vérifie l'existence d'un code dans la base des groupes
//               Si le code existe, renvoie la date associée
//               Retourne false en cas d'erreur de base, true sinon
//---------------------------------------------------------------------------
bool
NSGroupGdDialog::ExisteCode(string sCode, bool& bExiste, string& sDate)
{
	string sLang = pContexte->getUserLanguage() ;

  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (Group.lastError != DBIERR_NONE)
  {
   	erreur("Impossible d'ouvrir la base GroupGd.db", standardError, Group.lastError) ;
    return false ;
  }

  Group.lastError = Group.chercheClef(&sCode, "", 0, keySEARCHEQ, dbiWRITELOCK) ;  if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return false ;
  }
  if (Group.lastError == DBIERR_NONE)  // cas où le code existe
  {
    Group.lastError = Group.getRecord() ;
    if (Group.lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture dans la base GroupGd.db", standardError, Group.lastError) ;
      Group.close() ;
      return false ;
    }

    sDate = donne_date(Group._Donnees.date, sLang) ;
    bExiste = true ;
  }
  else // le code n'existe pas
  {
    sDate = string("") ;
    bExiste = false ;
  }

  // on ferme la base
  Group.lastError = Group.close() ;
  if (Group.lastError != DBIERR_NONE)
    erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, Group.lastError) ;

  return true ;
}


//---------------------------------------------------------------------------
//  Fonction :   NSGroupGdDialog::ExisteLibelle(string sLibelle, bool& bExiste, string& sDate)
//
//  Description: Vérifie l'existence d'un libelle dans la base des groupes
//               Si le libelle existe, renvoie la date associée
//               Retourne false en cas d'erreur de base, true sinon
//---------------------------------------------------------------------------
bool
NSGroupGdDialog::ExisteLibelle(string sLibelle, bool& bExiste, string& sDate)
{
	char libelle1[80], libelle2[80] ;

	string sLang = pContexte->getUserLanguage() ;

  strcpy(libelle1, sLibelle.c_str()) ;
  ote_blancs(libelle1) ;
  pseumaj(libelle1) ;
  bExiste = false ;

  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (Group.lastError != DBIERR_NONE)
  {
  	erreur("Impossible d'ouvrir la base GroupGd.db", standardError, Group.lastError) ;
    return false ;
  }

	Group.lastError = Group.debut(dbiWRITELOCK) ;
  if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_BOF))
	{
		erreur("Erreur de positionnement dans la base GroupGd.db", standardError, Group.lastError) ;
		Group.close() ;
		return false ;
	}

	if (Group.lastError == DBIERR_BOF)
	{
  	// cas de la base vide : on renvoie true en laissant bExiste à false
    Group.close() ;
		return true ;
  }

	do
	{
  	Group.lastError = Group.getRecord() ;
		if (Group.lastError != DBIERR_NONE)
		{
			erreur("Erreur de lecture dans la base GroupGd.db", standardError, Group.lastError) ;
			Group.close() ;
			return false ;
		}

    strcpy(libelle2, Group._Donnees.libelle) ;
    ote_blancs(libelle2) ;
    pseumaj(libelle2) ;

    // Si le libelle existe
    if (!strcmp(libelle1, libelle2))
    {
      sDate = donne_date(Group._Donnees.date, sLang) ;
      bExiste = true ;
      break ;
    }
    // on se positionne sur le groupe suivant
    Group.lastError = Group.suivant(dbiWRITELOCK) ;
		if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'accès à la fiche groupe suivante.", standardError, Group.lastError);
      Group.close() ;
			return false;
    }
  }
	while (Group.lastError != DBIERR_EOF);

	Group.lastError = Group.close() ;
  if (Group.lastError != DBIERR_NONE)
		erreur("Erreur de fermeture de la base GroupGd.db", standardError, Group.lastError) ;

	return true ;
}

//---------------------------------------------------------------------------//  Fonction :   NSGroupGdDialog::DetruireGroupe(string sCode)
//
//  Description: Enleve le groupe de code donné de la base des groupes
//               et les fils guides liés au groupe de GUIDE.Db
//               Retourne false en cas d'erreur de base, true sinon
//---------------------------------------------------------------------------
bool
NSGroupGdDialog::DetruireGroupe(string sCode)
{
  //
  // Création d'un BBFiche
  //
  BBFiche cBBFiche(pContexte->getSuperviseur()) ;
  cBBFiche.lastError = cBBFiche.open() ;
  if (cBBFiche.lastError != DBIERR_NONE)
  {
    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, cBBFiche.lastError) ;
    return false ;
  }

  // On cherche les fils guides de groupe sCode  // pour pouvoir les supprimer  string sCodeFG = sCode + string(CH_CODE_LEN, ' ') ;
  cBBFiche.lastError = cBBFiche.chercheClef(&sCodeFG,
                                      "",
                                      0,
                                      keySEARCHGEQ,
                                      dbiWRITELOCK) ;

  if ((cBBFiche.lastError != DBIERR_NONE) && (cBBFiche.lastError != DBIERR_EOF))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, cBBFiche.lastError) ;
    cBBFiche.close() ;
    return false ;
  }

  // on boucle pour détruire tous les fils guides de type sCode
  while (cBBFiche.lastError != DBIERR_EOF)
  {
    cBBFiche.lastError = cBBFiche.getRecord() ;
    if (cBBFiche.lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture d'une fiche Guides.", standardError, cBBFiche.lastError) ;
      cBBFiche.close() ;
      return false ;
    }

    // Condition d'arret
    if (cBBFiche.getGroupID() != sCode)
      break ;


    cBBFiche.lastError = cBBFiche.deleteRecord() ;    if (cBBFiche.lastError != DBIERR_NONE)    {
      erreur("Erreur à la destruction d'une fiche Guides.", standardError, cBBFiche.lastError) ;
      cBBFiche.close() ;
      return false ;
    }

    cBBFiche.lastError = cBBFiche.suivant(dbiWRITELOCK) ;
    if ((cBBFiche.lastError != DBIERR_NONE) && (cBBFiche.lastError != DBIERR_EOF))
    {
      erreur("Erreur d'accès à la fiche Guides suivante.", standardError, cBBFiche.lastError) ;
      cBBFiche.close() ;
      return false ;
    }
  }

  cBBFiche.lastError = cBBFiche.close() ;
  if (cBBFiche.lastError != DBIERR_NONE)
    erreur("Erreur de fermeture de la base des Guides.", standardError, cBBFiche.lastError) ;

  // on enlève maintenant le groupe associé de la base des groupes
  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (Group.lastError != DBIERR_NONE)
  {
    erreur("Impossible d'ouvrir la base GroupGd.db", standardError, Group.lastError) ;
    return false ;
  }

  Group.lastError = Group.chercheClef(&sCode,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

  if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return false ;
  }

  if (Group.lastError == DBIERR_NONE)  // cas où le code existe
  {
    Group.lastError = Group.deleteRecord() ;
    if (Group.lastError != DBIERR_NONE)
    {
      erreur("Erreur à la suppression dans la base GroupGd.db", standardError, Group.lastError) ;
      Group.close() ;
      return false ;
    }
  }
  else // le code n'existe pas
    erreur("Le groupe comportant le code spécifié n'existe pas dans GroupGd.db", standardError, 0) ;

  // on ferme la base
  Group.lastError = Group.close() ;
  if (Group.lastError != DBIERR_NONE)
    erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, Group.lastError) ;

  return true ;
}


//---------------------------------------------------------------------------
//  Fonction :   NSGroupGdDialog::CodeDispo(string& sCode)
//
//  Description: Recherche le premier code disponible parmi
//               les fils guides utilisateurs
//               Retourne false en cas d'erreur de base, true sinon
//---------------------------------------------------------------------------
bool
NSGroupGdDialog::CodeDispo(string& sCode)
{
    string sCodePrec, sCodeSuiv = "001";

    // On doit d'abord se placer après les groupes de
    // fils guides système (en $XX)
    NSGroupGd* pGroup = new NSGroupGd(pContexte->getSuperviseur());
    pGroup->lastError = pGroup->open();
    if (pGroup->lastError != DBIERR_NONE)
    {
        erreur("Impossible d'ouvrir la base GroupGd.db", standardError, pGroup->lastError);
        delete pGroup;
        return false;
    }

    pGroup->lastError = pGroup->debut(dbiWRITELOCK);
   	if ((pGroup->lastError != DBIERR_NONE) && (pGroup->lastError != DBIERR_BOF))
	{
		erreur("Erreur de positionnement dans la base GroupGd.db", standardError, pGroup->lastError);
		pGroup->close();
		delete pGroup;
		return false;
	}

    if (pGroup->lastError == DBIERR_BOF)
    {
        // cas de la base vide
        sCode = sCodeSuiv;
        pGroup->close();
		delete pGroup;
		return true;
    }

   	do
   	{
   		pGroup->lastError = pGroup->getRecord();
		if (pGroup->lastError != DBIERR_NONE)
		{
			erreur("Erreur de lecture dans la base GroupGd.db", standardError, pGroup->lastError);
			pGroup->close();
			delete pGroup;
			return false;
		}

        // condition d'arret
        if (pGroup->_Donnees.groupe[0] != '$')
            break;

      	// on se positionne sur le groupe suivant
      	pGroup->lastError = pGroup->suivant(dbiWRITELOCK);
		if ((pGroup->lastError != DBIERR_NONE) && (pGroup->lastError != DBIERR_EOF))
      	{
			erreur("Erreur d'accès à la fiche groupe suivante.", standardError, pGroup->lastError);
         	pGroup->close();
			delete pGroup;
			return false;
      	}

    }
   	while (pGroup->lastError != DBIERR_EOF);

    // on boucle jusqu'à trouver le premier trou
    // dans les codes groupe utilisateur
    while (pGroup->lastError != DBIERR_EOF)
    {
        sCodePrec = string(pGroup->_Donnees.groupe);
        sCodeSuiv = sCodePrec;
        pGroup->IncrementeCode(&sCodeSuiv);

        // on se positionne sur le groupe suivant
      	pGroup->lastError = pGroup->suivant(dbiWRITELOCK);
		if ((pGroup->lastError != DBIERR_NONE) && (pGroup->lastError != DBIERR_EOF))
      	{
			erreur("Erreur d'accès à la fiche groupe suivante.", standardError, pGroup->lastError);
         	pGroup->close();
			delete pGroup;
			return false;
      	}

        if (pGroup->lastError != DBIERR_EOF)
        {
            pGroup->lastError = pGroup->getRecord();
		    if (pGroup->lastError != DBIERR_NONE)
		    {
			    erreur("Erreur de lecture dans la base GroupGd.db", standardError, pGroup->lastError);
			    pGroup->close();
			    delete pGroup;
			    return false;
		    }

            // condition d'arret : il existe un trou
            if (string(pGroup->_Donnees.groupe) > sCodeSuiv)
                break;
        }
    } // fin while

    // on renvoie le dernier code libre
    sCode = sCodeSuiv;

    // on ferme la base
    pGroup->lastError = pGroup->close();
    if (pGroup->lastError != DBIERR_NONE)
        erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, pGroup->lastError);

    delete pGroup;
    return true;
}
voidNSGroupGdDialog::CmImport()
{
  char 	path[256];
  string  sGroupe;
  string  sLibelle;
  string  sDateGroupe, sDateBase;
  string  sUtil;
  ifstream inFile;
  string   header, line;
  string   sParam[6] = {"","","","","",""};
  bool     bInterne;

  // on choisi d'abord le répertoire par défaut d'importation (IHTM)
  strcpy(path,(pContexte->PathName("IHTM")).c_str());

  TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
                      "Tous fichiers (*.NSG)|*.nsg|",
                                          0,path,"NSG");

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

  if (IDOK != iUserChoice)
		return ;

  string sFileName = string(initData.FileName);

  inFile.open(sFileName.c_str());
	if (!inFile)
  {
  	erreur("Impossible d'ouvrir le fichier groupe à importer.", standardError, 0) ;
    return ;
  }

  // on récupère le header du fichier groupe
  if (!inFile.eof())
  	getline(inFile, header) ;
  else
  {
  	erreur("Le fichier groupe est vide.", standardError, 0) ;
    inFile.close() ;
    return ;
  }

	if (header != "")
  {
  	size_t i = 0, k = 0 ;

    while (i < strlen(header.c_str()))
    {
    	if (header[i] == '\"')
      	k++ ;
      else
      {
      	if (k < 6)
        	sParam[k] += header[i] ;
        else
        {        	erreur("Le header du fichier groupe est incorrect.", standardError, 0) ;
          return ;
        }
      }

      i++ ;
    }
  }
  else
  {
  	erreur("Le header du fichier groupe est vide.", standardError, 0) ;
    inFile.close() ;
    return ;
  }

  sGroupe = sParam[2] ;
  sLibelle = sParam[3] ;
  sDateGroupe = sParam[4] ;
  sUtil = sParam[5] ;

  if ('$' != sGroupe[0])  {
    int retVal ;
    bool bExiste = false ;

    // On vérifie s'il existe un groupe utilisateur de même nom    // Si c'est le cas, on interrompt l'importation.
    if (!ExisteLibelle(sLibelle, bExiste, sDateBase))
    {
      erreur("L'importation est interrompue.", standardError, 0) ;
      inFile.close() ;
      return ;
    }

    if (bExiste)
    {
      string sMsg = "Il existe déjà un groupe de même libellé, daté du " + sDateBase ;
      erreur(sMsg.c_str(), standardError, 0) ;
      inFile.close() ;
      return ;
    }

    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Ce groupe de fils guides a-t-il été créé sur votre site ?", sCaption.c_str(), MB_YESNO) ;
    if (retVal == IDYES)
      bInterne = true ;
    else
      bInterne = false ;

    if (false == CodeDispo(sGroupe))
    {
      erreur("Impossible de trouver un nouveau code pour le groupe à importer.", standardError, 0) ;
      inFile.close() ;
      return ;
    }
  }
  else
  {
    bool bExiste = false ;

    // On vérifie s'il existe un groupe systeme de même code
    // Si c'est le cas, soit on le détruit et on importe,
    // soit on interrompt l'importation.
    if (!ExisteCode(sGroupe, bExiste, sDateBase))
    {
      erreur("L'importation est interrompue.", standardError, 0) ;
      inFile.close() ;
      return ;
    }

    if (bExiste)
    {
      int retVal ;
      string sMsg = "Ce groupe système existe déjà à la date du " + sDateBase + ". Voulez-vous le remplacer ?" ;

      string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
      retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMsg.c_str(), sCaption.c_str(), MB_YESNO) ;
      if (retVal == IDYES)
      {
        if (!DetruireGroupe(sGroupe))
        {
          erreur("L'importation a échoué.", standardError, 0) ;
          inFile.close() ;
          return ;
        }
      }
      else
        return ;
    }
    bInterne = false ;
  }

  // Controle de la version
  if (sParam[0] == "1.0")
  {
    string sData ;
    //
    // Création d'un BBFiche
    //
    BBFiche cBBFiche(pContexte->getSuperviseur()) ;    cBBFiche.lastError = cBBFiche.open() ;
    if (cBBFiche.lastError != DBIERR_NONE)
    {
      erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, cBBFiche.lastError) ;
      inFile.close() ;
      return ;
    }

    while (!inFile.eof())
    {
      getline(inFile, line) ;
      if (string("") != line)
      {
        size_t pos1 = 0 ;
        BBItemData* pItemData = static_cast<BBItemData*>(cBBFiche.getData()) ;
        pItemData->metAZero() ;
        size_t pos2 = line.find_first_of('\"', pos1) ;
        string sFieldData = string(line, pos1, pos2 - pos1) ;
        pItemData->setID(sFieldData) ;

        size_t index = 0 ;
        while (index < 17)
        {
          pos1 = pos2 + 1 ;
          pos2 = line.find_first_of('\"', pos1) ;

          sFieldData = string(line, pos1, pos2 - pos1) ;

          switch(index)
          {
            case  0 : pItemData->setGroupID(sGroupe) ;            break ;
            case  1 : pItemData->setSemanticPath(sFieldData) ;    break ;
            case  2 : pItemData->setLabel(sFieldData) ;           break ;
            case  3 : pItemData->setDialogFile(sFieldData) ;      break ;
            case  4 : pItemData->setDialogName(sFieldData) ;      break ;
            case  5 : pItemData->setOpenDialog(sFieldData) ;      break ;
            case  6 : pItemData->setFctFile(sFieldData) ;         break ;
            case  7 : pItemData->setFctName(sFieldData) ;         break ;
            case  8 : pItemData->setLevelShift(sFieldData) ;      break ;
            case  9 : pItemData->setSonsList(sFieldData) ;        break ;
            case 10 : pItemData->setSonsRules(sFieldData) ;       break ;
            case 11 : pItemData->setAutomatic(sFieldData) ;       break ;
            case 12 : pItemData->setEmptyActivation(sFieldData) ; break ;
            case 13 : pItemData->setUnicity(sFieldData) ;         break ;
            case 14 : pItemData->setNeeded(sFieldData) ;          break ;
            case 15 : pItemData->setSortRules(sFieldData) ;       break ;
            case 16 : pItemData->setValidityCase(sFieldData) ;    break ;
          }

          index++ ;
        }

        cBBFiche.lastError = cBBFiche.appendRecord() ;
        if (cBBFiche.lastError != DBIERR_NONE)
        {
          erreur("Erreur à l'ajout du nouveau fil guide.", standardError, cBBFiche.lastError) ;
          cBBFiche.close() ;
          inFile.close() ;
          return ;
        }
      }
    }

    cBBFiche.lastError = cBBFiche.close() ;
    if (cBBFiche.lastError != DBIERR_NONE)
      erreur("Erreur de fermeture de la base des Guides.", standardError, cBBFiche.lastError) ;
  }
  else
  {
    erreur("La version de ce groupe de fils guides n'est pas gérée.",standardError, 0) ;
    inFile.close() ;
    return;
  }

  inFile.close() ;

  // on ajoute maintenant le groupe associé à la base des groupes
  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (Group.lastError != DBIERR_NONE)
  {
    erreur("Impossible d'ouvrir la base GroupGd.db", standardError, Group.lastError) ;
    return ;
  }

  strcpy(Group._Donnees.groupe,  sGroupe.c_str()) ;
  strcpy(Group._Donnees.libelle, sLibelle.c_str()) ;
  strcpy(Group._Donnees.date,    sDateGroupe.c_str()) ;
  strcpy(Group._Donnees.util,    sUtil.c_str()) ;

  Group.lastError = Group.appendRecord() ;
  if (Group.lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ajout dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return ;
  }

  // on ferme la base
  Group.lastError = Group.close() ;
  if (Group.lastError != DBIERR_NONE)
    erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, Group.lastError) ;

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Importation du groupe de fils guides terminée.", sCaption.c_str(), MB_OK) ;
}

void
NSGroupGdDialog::CmExport()
{
  string sOut = "";
  string sCodeGroupe;
  int    nbfg;
  ofstream outFile;
  NSChoixGdDialog* pChoixGdDlg = new NSChoixGdDialog(this, pContexte);

  if (pChoixGdDlg->Execute() != IDOK)
  {
    delete pChoixGdDlg ;
    return ;
  }

  int index = pChoixGdDlg->GroupChoisi ;
  if (-1 == index)
  {
    delete pChoixGdDlg ;
    return ;
  }

  NSGroupGdInfo GroupeChoisi = *((pChoixGdDlg->GroupArray)[index]) ;

  delete pChoixGdDlg ;

  // On écrit d'abord le header dans sOut
  sOut += string("1.0")           + string("\"") ;   // Numéro de version
  sOut += string("    ")          + string("\"") ;  // CRC (pour + tard)
  sOut += GroupeChoisi.getGroup() + string("\"") ;
  sOut += GroupeChoisi.getLabel() + string("\"") ;
  sOut += GroupeChoisi.getDate()  + string("\"") ;
  sOut += GroupeChoisi.getUser()  + string("\n") ;

  sCodeGroupe = GroupeChoisi.getGroup() ;

  //
  // On écrit maintenant tous les fils guides appartenant à ce groupe
  // Création d'un BBFiche
  //
  BBFiche Fiche(pContexte->getSuperviseur()) ;
  Fiche.lastError = Fiche.open() ;
  if (DBIERR_NONE != Fiche.lastError)
  {
    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, Fiche.lastError) ;
    return ;
  }

  // On cherche les fils guides de groupe sCode  // pour pouvoir les exporter
  string sCodeFG = sCodeGroupe + string(CH_CODE_LEN, ' ');
  Fiche.lastError = Fiche.chercheClef(&sCodeFG,
                                      "",
                                      0,
                                      keySEARCHGEQ,
                                      dbiWRITELOCK) ;

  if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_EOF))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return ;
  }

  // on boucle pour exporter tous les fils guides de type sCodeGroupe
  nbfg = 0 ;

  while (Fiche.lastError != DBIERR_EOF)  {
    Fiche.lastError = Fiche.getRecord() ;
    if (Fiche.lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture d'une fiche Guides.", standardError, Fiche.lastError) ;
      Fiche.close() ;
      return ;
    }

    // Condition d'arret : on n'est plus sur le bon code groupe
    if (Fiche.getGroupID() != sCodeGroupe)
      break ;

    BBItemData* pItemData = static_cast<BBItemData*>(Fiche.getData()) ;

    // On écrit ici les données du fil guide dans sOut
    sOut += pItemData->getID()                + string("\"") ;
    sOut += pItemData->getGroupID()           + string("\"") ;
    sOut += pItemData->getSemanticPath()      + string("\"") ;
    sOut += pItemData->getLabel()             + string("\"") ;
    sOut += pItemData->getDialogFile()        + string("\"") ;
    sOut += pItemData->getDialogName()        + string("\"") ;
    sOut += string(pItemData->ouvreDialogue)  + string("\"") ;
    sOut += pItemData->getFctFile()           + string("\"") ;
    sOut += pItemData->getFctName()           + string("\"") ;
    sOut += pItemData->getLevelShift()        + string("\"") ;
    sOut += pItemData->getSonsList()          + string("\"") ;
    sOut += pItemData->getSonsRules()         + string("\"") ;
    sOut += string(pItemData->automatique)    + string("\"") ;
    sOut += string(pItemData->actif_vide)     + string("\"") ;
    sOut += string(pItemData->unicite_lesion) + string("\"") ;
    sOut += string(pItemData->impose)         + string("\"") ;
    sOut += pItemData->getSortRules()         + string("\"") ;
    sOut += pItemData->getValidityCase()      + string("\"") ;
    sOut += string("\n") ;

    nbfg++ ;

    // On passe au fil guide suivant
    Fiche.lastError = Fiche.suivant(dbiWRITELOCK);
    if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_EOF))
    {
      erreur("Erreur d'accès à la fiche Guides suivante.", standardError, Fiche.lastError) ;
      Fiche.close() ;
      return ;
    }
  }

  Fiche.lastError = Fiche.close() ;
  if (Fiche.lastError != DBIERR_NONE)
    erreur("Erreur de fermeture de la base des Guides.", standardError, Fiche.lastError) ;

  if (0 == nbfg)
  {
    erreur("Aucun fil guide n'existe avec le code groupe spécifié", standardError, 0) ;
    return ;
  }

  NSNomGdDialog* pNomGdDlg = new NSNomGdDialog(this, pContexte->PathName("IHTM"), "", "nsg", pContexte) ;
  if (pNomGdDlg->Execute() != IDOK)
  {
    erreur("L'exportation du groupe est annulée", standardError, 0) ;
    delete pNomGdDlg ;
    return ;
  }

  // on peut enregistrer sous le nom de fichier proposé
  string sFichier = pNomGdDlg->sFichier ;

  delete pNomGdDlg ;

  outFile.open(sFichier.c_str()) ;
  if (outFile)
  {
    for (size_t i=0; i < strlen(sOut.c_str()); i++)
      outFile.put(sOut[i]) ;

    outFile.close() ;
  }
  else
  {
    erreur("Impossible de créer le fichier à exporter", standardError, 0) ;
    return ;
  }

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Exportation du groupe de fils guides terminée.", sCaption.c_str(), MB_OK) ;
}

voidNSGroupGdDialog::CmDestroy()
{
  NSChoixGdDialog* pChoixGdDlg = new NSChoixGdDialog(this, pContexte) ;

  if (pChoixGdDlg->Execute() != IDOK)
  {
    delete pChoixGdDlg ;
    return ;
  }

  int index = pChoixGdDlg->GroupChoisi ;
  if (index == -1)
  {
    delete pChoixGdDlg ;
    return ;
  }

  NSGroupGdInfo* pGroupeChoisi = (pChoixGdDlg->GroupArray)[index] ;
  string sCodeGroupe = pGroupeChoisi->getGroup() ;

  string sMsg = "Etes vous sûr de vouloir détruire le groupe ";
  if (sCodeGroupe[0] == '$')
    sMsg += "système : ";
  else
    sMsg += "utilisateur : ";
  sMsg += pGroupeChoisi->getLabel() ;
  sMsg += ", ainsi que tous les fils guides associés à ce groupe ?";

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  int retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMsg.c_str(), sCaption.c_str(), MB_YESNO) ;
  if (retVal == IDYES)
  {
    if (!DetruireGroupe(sCodeGroupe))
      erreur("La destruction de ce groupe a échoué.", standardError, 0) ;
    else
      ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Destruction terminée.", sCaption.c_str(), MB_OK) ;
  }

  delete pChoixGdDlg ;
}

void
NSGroupGdDialog::CmGestion()
{
  NSListChemDialog* pListChemDlg = new NSListChemDialog(this, pContexte, pNSResModule) ;
  pListChemDlg->Execute() ;
  delete pListChemDlg ;
}

voidNSGroupGdDialog::CmOk()
{
  NSUtilDialog::CmOk() ;
}

void
NSGroupGdDialog::CmCancel()
{
  NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSChoixGdDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSChoixGdDialog, NSUtilDialog)
  EV_CHILD_NOTIFY_AND_CODE(IDC_CHOIXGD_LIST, LBN_SELCHANGE, CmSelectGroup),
  EV_CHILD_NOTIFY_AND_CODE(IDC_CHOIXGD_LIST, LBN_DBLCLK, CmGroupDblClk),
  EV_BN_CLICKED(IDC_CHOIXGD_FGSYS, FilGuideSys),
  EV_BN_CLICKED(IDC_CHOIXGD_FGUTIL, FilGuideUtil),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSChoixGdDialog::NSChoixGdDialog(TWindow* pere, NSContexte* pCtx, bool utilOnly)
                :NSUtilDialog(pere, pCtx, "IDD_CHOIXGD", pNSResModule)
{
  GroupChoisi = -1 ;

  pGroupBox   = new OWL::TListBox(this, IDC_CHOIXGD_LIST) ;
  pFGGB       = new OWL::TGroupBox(this, IDC_CHOIXGD_GB) ;
  pFGSys      = new OWL::TRadioButton(this, IDC_CHOIXGD_FGSYS, pFGGB) ;
  pFGUtil     = new OWL::TRadioButton(this, IDC_CHOIXGD_FGUTIL, pFGGB) ;

  bUtilOnly   = utilOnly ;}

NSChoixGdDialog::~NSChoixGdDialog(){
  delete pGroupBox ;
  delete pFGGB ;
  delete pFGSys ;
  delete pFGUtil ;
}

voidNSChoixGdDialog::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  if (bUtilOnly)  {
    bFGSys = false ;
    pFGSys->Uncheck() ;
    pFGUtil->Check() ;
  }
  else
  {
    bFGSys = true ;
    pFGSys->Check() ;
    pFGUtil->Uncheck() ;
  }

  InitListe() ;}

boolNSChoixGdDialog::InitListe()
{
  //
	// Remplissage de GroupBox avec les libelles des fichiers de groupe
	//
	NSGroupGd Group(pContexte->getSuperviseur()) ;

	//	// Ouverture du fichier
	//
	Group.lastError = Group.open() ;

	if (DBIERR_NONE != Group.lastError)	{
    erreur("Erreur à l'ouverture de la base GroupGd.db.", standardError, Group.lastError) ;
		return false ;
	}

  Group.lastError = Group.debut(dbiWRITELOCK) ;
  if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_BOF))	{
		erreur("Erreur de positionnement dans la base GroupGd.db.", standardError, Group.lastError);
		Group.close() ;
		return false ;
	}

  if (Group.lastError == DBIERR_BOF)  {
    erreur("La base GroupGd est vide.", standardError, 0) ;
		Group.close() ;
		return false ;
  }

  do  {
    Group.lastError = Group.getRecord() ;

		if (DBIERR_NONE != Group.lastError)		{
			erreur("Erreur de lecture dans la base GroupGd.db.", standardError, Group.lastError) ;
			Group.close() ;
			return false ;
		}

    // On remplit la GroupBox et le GroupArray avec les groupes de fils guides    // de type utilisateur ou les groupes de fils guides système
    //
    if ((bFGSys && (Group._Donnees.groupe[0] == '$')) ||            ((!bFGSys) && (Group._Donnees.groupe[0] != '$')))
    {
      pGroupBox->AddString(Group._Donnees.libelle) ;
			GroupArray.push_back(new NSGroupGdInfo(&Group)) ;
    }

    // on se positionne sur la template suivante
    //
    Group.lastError = Group.suivant(dbiWRITELOCK) ;
		if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_EOF))    {
      erreur("Erreur d'accès au groupe suivant.", standardError, Group.lastError) ;
      Group.close() ;
			return false ;
    }
  }
  while (DBIERR_EOF != Group.lastError) ;

	Group.lastError = Group.close() ;
  if (DBIERR_NONE != Group.lastError)    erreur("Erreur de fermeture de la base des groupes.", standardError, Group.lastError) ;

  return true ;}

void NSChoixGdDialog::FilGuideSys(){
  if (bUtilOnly)
  {
    pFGSys->Uncheck() ;
    pFGUtil->Check() ;
    return ;
  }

  bFGSys = true ;
  pGroupBox->ClearList() ;

  GroupArray.vider() ;

  InitListe() ;
}

void NSChoixGdDialog::FilGuideUtil(){
  bFGSys = false ;
  pGroupBox->ClearList() ;

  GroupArray.vider() ;

  InitListe() ;
}

void NSChoixGdDialog::CmNewGroup(){
  if (bFGSys)
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    MessageBox("Fonction non disponible pour les groupes système.", sCaption.c_str(), MB_OK) ;
    return ;
  }

  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (DBIERR_NONE != Group.lastError)
  {
    erreur("Impossible d'ouvrir la base GroupGd.db", standardError, Group.lastError) ;
    return ;
  }

  Group.lastError = Group.fin(dbiWRITELOCK);
  if (DBIERR_NONE != Group.lastError)
  {
    erreur("Erreur au positionnement dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return ;
  }

  Group.lastError = Group.getRecord() ;
  if (DBIERR_NONE != Group.lastError)
  {
    erreur("Erreur à la lecture dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return ;
  }

  string sCode = Group.getGroup() ;

  if ('$' != sCode[0])
  {
    if (false == Group.IncrementeCode(&sCode))
    {
      erreur("Erreur à l'attribution d'un nouveau code groupe.", standardError, 0) ;
      Group.close();
      return;
    }
  }
  else    // cas du premier groupe utilisateur
    sCode = string("000") ;

  NSCreateGdDialog* pDlg = new NSCreateGdDialog(this, pContexte, pNSResModule) ;
  strcpy(pDlg->pData->groupe, sCode.c_str()) ;

  if (pDlg->Execute() == IDCANCEL)
  {
    delete pDlg;
    Group.close() ;
    return ;
  }

  // le code n'existe pas encore => on ajoute la fiche groupe
  Group._Donnees = *(pDlg->pData) ;

  delete pDlg ;

  Group.lastError = Group.appendRecord() ;
  if (DBIERR_NONE != Group.lastError)
  {
    erreur("Erreur à l'ajout dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return ;
  }

  // !!! on met à jour la GroupBox s'il s'agit d'un nouveau groupe utilisateur
  // ce test n'est plus nécéssaire mais on le garde pour mémoire

  sCode = Group.getGroup() ;

  if ('$' != sCode[0])
  {
    GroupArray.push_back(new NSGroupGdInfo(&Group)) ;

    pGroupBox->AddString(Group.getLabel().c_str()) ;
    pGroupBox->SetSelIndex(pGroupBox->GetCount() - 1) ;
    GroupChoisi = pGroupBox->GetSelIndex() ;
  }

  // on ferme la base
  Group.lastError = Group.close() ;
  if (DBIERR_NONE != Group.lastError)
    erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, Group.lastError) ;
}

void NSChoixGdDialog::CmModGroup(){
  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;

  if (-1 == GroupChoisi)
  {
    MessageBox("Vous devez sélectionner un groupe à modifier.", sCaption.c_str(), MB_OK) ;
    return ;
  }

  if (bFGSys)
  {
    MessageBox("Fonction non disponible pour les groupes système.", sCaption.c_str(), MB_OK) ;
    return ;
  }

  // on passe les data au dialogue de modification des groupes
  NSGroupGdInfo* pGroupeChoisi = GroupArray[GroupChoisi] ;
  NSCreateGdDialog* pDlg = new NSCreateGdDialog(this, pContexte, pNSResModule) ;
  *(pDlg->pData) = pGroupeChoisi->_Donnees ;

  if (pDlg->Execute() == IDCANCEL)
  {
    delete pDlg ;
    return ;
  }

  string sCode = string(pDlg->pData->groupe) ;

  // on regarde si le code existe bien
  // avant de modifier la nouvelle fiche groupe
  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (Group.lastError != DBIERR_NONE)
  {
    erreur("Impossible d'ouvrir la base GroupGd.db", standardError, Group.lastError) ;
    delete pDlg ;
    return ;
  }

  Group.lastError = Group.chercheClef(&sCode,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

  if ((Group.lastError != DBIERR_NONE) && (Group.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    delete pDlg ;
    return ;
  }
  else if (Group.lastError == DBIERR_RECNOTFOUND)  // cas où le code n'existe plus
  {
    erreur("Impossible de retrouver le groupe à modifier dans GroupGd.db", standardError, 0) ;
    Group.close() ;
    delete pDlg ;
    return ;
  }

  // on est sur le bon Record => on modifie la fiche groupe
  Group._Donnees = *(pDlg->pData) ;

  Group.lastError = Group.modifyRecord(TRUE) ;
  if (Group.lastError != DBIERR_NONE)
  {
    erreur("Erreur à la modification dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    delete pDlg ;
    return ;
  }

  // on ferme la base
  Group.lastError = Group.close() ;
  if (Group.lastError != DBIERR_NONE)
    erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, Group.lastError) ;

  delete pDlg ;
}

void NSChoixGdDialog::CmGroupDblClk(WPARAM /* Cmd */){
	GroupChoisi = pGroupBox->GetSelIndex() ;
	NSUtilDialog::CmOk() ;
}

void NSChoixGdDialog::CmSelectGroup(WPARAM /* Cmd */){
	//
	// Récupération de l'indice du groupe sélectionné
	//
	GroupChoisi = pGroupBox->GetSelIndex();
}

void NSChoixGdDialog::CmCancel(){
	GroupChoisi = -1;
	TDialog::CmCancel();
}

// -----------------------------------------------------------------//
//  Méthodes de NSNomGdDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSNomGdDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSNomGdDialog::NSNomGdDialog(TWindow* pere, string sDir, string sNom, string sExt, NSContexte* pCtx)
              :NSUtilDialog(pere, pCtx, "IDD_NOMGD", pNSResModule)
{
  sPath = sDir ;

  size_t pos = sNom.find('.') ;
  if (pos != NPOS)
  	sFichier = string(sNom, 0, pos) ;
  else
  	sFichier = sNom ;
  sExtension = sExt ;

  pText = new TStatic(this, IDC_NOMGD_TEXT) ;
  pNom  = new NSUtilEdit(pContexte, this, IDC_NOMGD_EDIT, 80) ;
}

NSNomGdDialog::~NSNomGdDialog(){
  delete pNom;
}

voidNSNomGdDialog::SetupWindow()
{
  char far lib[100] ;
  string sLib;

  NSUtilDialog::SetupWindow() ;
  pText->GetText(lib, 100);  sLib = string(lib) + string(" ") + sExtension;
  pText->SetText(sLib.c_str());
  string sFileName = sFichier + "." + sExtension;
  pNom->SetText(sFileName.c_str());
  pNom->SetFocus();
  // Le SetSelection ne marche pas...
  // pNom->SetSelection(0, strlen(sFichier.c_str()));
}

void NSNomGdDialog::CmOk(){
  string sName = pNom->GetText(80) ;
  if (string("") == sName)  {    erreur("Vous devez indiquer un nom de fichier.", standardError, 0) ;
    return ;
  }
  sFichier = sName ;
  char msg[255] ;
  sprintf(msg, "Le nom de fichier doit comporter l'extension .%s", sExtension.c_str());
  size_t pos = sFichier.find('.');
  if (NPOS == pos)
  {    erreur(msg, standardError, 0, GetHandle()) ;
    return ;
  }

  string sExt = string(sFichier, pos+1, strlen(sFichier.c_str())-pos-1) ;
  if (sExt != sExtension)  {    erreur(msg, standardError, 0, GetHandle()) ;    return ;
  }

  sFichier = sPath + sFichier ;
  if (NsFileExists(sFichier))  {    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
    int idRet = MessageBox("Ce fichier existe déjà. Voulez-vous le remplacer ?", sCaption.c_str(), MB_YESNO);
    if (IDNO == idRet)
      return ;  }

	NSUtilDialog::CmOk();}

void NSNomGdDialog::CmCancel(){
	NSUtilDialog::CmCancel();
}

/*****************************************************************************/
/******             OUTIL DE GESTION DES FILS GUIDES                    ******/
/*****************************************************************************/

// -----------------------------------------------------------------
//
//  Méthodes de NSListChemWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListChemWindow, TListWindow)
    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Function: NSListChemWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
//
//  Arguments:	  les modKeys et le point clické
//
//  Description: Fonction de réponse au click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListChemWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TListWindow::EvLButtonDown(modKeys, point);

    TLwHitTestInfo info(point);
    HitTest(info);

    if (info.GetFlags() & LVHT_ONITEM)
    {
        pDlg->CmModFG();
    }
}

//---------------------------------------------------------------------------
//  Function: NSListChemWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListChemWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)
   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSListChemDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListChemDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_CBN_SELCHANGE(IDC_LISTFG_GROUPE, EvGroupChange),
    EV_BN_CLICKED(IDC_LISTFG_NEW, CmNewFG),
    EV_BN_CLICKED(IDC_LISTFG_MOD, CmModFG),
    EV_BN_CLICKED(IDC_LISTFG_DUP, CmDupFG),
    EV_BN_CLICKED(IDC_LISTFG_DEL, CmDelFG),
    EV_BN_CLICKED(IDC_LISTFG_UP, CmHaut),
    EV_BN_CLICKED(IDC_LISTFG_DOWN, CmBas),
    EV_BN_CLICKED(IDC_LISTFG_NEWGP, CmNewGP),
    EV_BN_CLICKED(IDC_LISTFG_MODGP, CmModGP),
END_RESPONSE_TABLE;

NSListChemDialog::NSListChemDialog(TWindow* pere, NSContexte* pCtx, TModule* mod)
                         :NSUtilDialog(pere, pCtx, "IDD_LISTFG", mod)
{
    pGroupBox =     new TComboBox(this, IDC_LISTFG_GROUPE);
    pListeChem =    new NSListChemWindow(this, IDC_LISTFG_LW);
    pChemArray =    new NSEltArray;
    nbChem = 0;
    pGroupArray =   new NSGroupGdArray;
}

NSListChemDialog::~NSListChemDialog()
{
    delete pGroupBox;
    delete pListeChem;
    delete pChemArray;
    delete pGroupArray;
}

void
NSListChemDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow();

    InitListeChem();

    if (InitGroupBox())
    {
        GroupChoisi = 0;
        pGroupBox->SetSelIndex(0);

        if (InitChemArray())
            AfficheListeChem();
        else
            erreur("Impossible d'afficher le groupe initial.",standardError, 0) ;
    }
    else
        erreur("Impossible d'obtenir la liste des groupes.",standardError, 0) ;
}

bool
NSListChemDialog::InitGroupBox()
{
    //
	// Remplissage de GroupBox avec les libelles des fichiers de groupe
    // Note : pour ce dialogue, on n'autorise que les fils guides utilisateurs
	//
	NSGroupGd* pGroup = new NSGroupGd(pContexte->getSuperviseur());
	//
	// Ouverture du fichier
	//
	pGroup->lastError = pGroup->open();
	if (pGroup->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base GroupGd.db.", standardError, pGroup->lastError);
		delete pGroup;
		return false;
	}

   	pGroup->lastError = pGroup->debut(dbiWRITELOCK);
   	if ((pGroup->lastError != DBIERR_NONE) && (pGroup->lastError != DBIERR_BOF))
	{
		erreur("Erreur de positionnement dans la base GroupGd.db.", standardError, pGroup->lastError);
		pGroup->close();
		delete pGroup;
		return false;
	}

    if (pGroup->lastError == DBIERR_BOF)
    {
        erreur("La base GroupGd est vide.", standardError, 0) ;
		pGroup->close();
		delete pGroup;
		return false;
    }

   	do
   	{
   		pGroup->lastError = pGroup->getRecord();
		if (pGroup->lastError != DBIERR_NONE)
		{
			erreur("Erreur de lecture dans la base GroupGd.db.", standardError, pGroup->lastError);
			pGroup->close();
			delete pGroup;
			return false;
		}

      	// On remplit la GroupBox et le GroupArray avec les groupes de fils guides
        // de type utilisateur
      	if (pGroup->_Donnees.groupe[0] != '$')
      	{
      		pGroupBox->AddString(pGroup->_Donnees.libelle);
			pGroupArray->push_back(new NSGroupGdInfo(pGroup));
      	}

      	// on se positionne sur le groupe suivant
      	pGroup->lastError = pGroup->suivant(dbiWRITELOCK);
		if ((pGroup->lastError != DBIERR_NONE) && (pGroup->lastError != DBIERR_EOF))
      	{
			erreur("Erreur d'accès au groupe suivant.", standardError, pGroup->lastError);
         	pGroup->close();
			delete pGroup;
			return false;
      	}

    }
   	while (pGroup->lastError != DBIERR_EOF);

	pGroup->lastError = pGroup->close();
   	if (pGroup->lastError != DBIERR_NONE)
		erreur("Erreur de fermeture de la base des groupes.", standardError, pGroup->lastError);

	delete pGroup;
    return true;
}

void
NSListChemDialog::InitListeChem()
{
    TListWindColumn colChem("Chemin", 1000, TListWindColumn::Left, 0);
  	pListeChem->InsertColumn(0, colChem);
}
voidNSListChemDialog::AfficheListeChem()
{
  pListeChem->DeleteAllItems() ;
  for (int i = nbChem - 1 ; i >= 0 ; i--)  {
    char elt[1024] ;
    sprintf(elt, "%s", (((*pChemArray)[i])->getLabel()).c_str()) ;
    TListWindItem Item(elt, 0) ;
    pListeChem->InsertItem(Item) ;
  }
}

voidNSListChemDialog::EvGroupChange()
{
  GroupChoisi = pGroupBox->GetSelIndex() ;

  // On recalcule le tableau de chemins à afficher  if (InitChemArray())
    AfficheListeChem() ;
  else
    erreur("Impossible d'afficher la liste des chemins.",standardError, 0) ;
}


void
NSListChemDialog::TrouveLibElement(string sElement, string& sLibElement)
{
    size_t pos1, pos2;
    bool bContinue = true;
    bool bPremier = true;
    string sCode1, sLibelle1;
    string sCode2, sLibelle2;

    // on remet à "" sLibElement
    sLibElement = "";

    pos1 = 0;
    pos2 = sElement.find('/');
    if (pos2 != NPOS)
    {
        while (bContinue)
        {
            if (pos2 != NPOS)
                sCode1 = string(sElement, pos1, pos2-pos1);
            else
                sCode1 = string(sElement, pos1, strlen(sElement.c_str())-pos1);

            TrouveLibElement(sCode1, sLibelle1);
            if (bPremier)
            {
                sLibElement = sLibelle1;
                bPremier = false;
            }
            else
                sLibElement += string("/") + sLibelle1;

            if ((pos2 == NPOS) || (pos2 == (strlen(sElement.c_str())-1)))
                bContinue = false;
            else
            {
                pos1 = pos2+1;
                pos2 = sElement.find('/', pos1);
            }
        }

        return;
    }

    string sLang = "";
    if (pContexte) 
        sLang = pContexte->getUserLanguage();

    // on traite les jokers à part
    if (sElement == "")
        sLibElement = "";
    else if (sElement[0] == '~')
        sLibElement = string(sElement, 3, 2);
    else
    {
        sCode2 = sElement; sLibelle2 = "";
        if (strlen(sCode2.c_str()) < BASE_LEXI_LEN)        	pContexte->getDico()->donneCodeComplet(sCode2) ;
        if (pContexte->getDico()->donneLibelle(sLang, &sCode2, &sLibelle2))
        {
            // pour les codes système
            if (sLibelle2 == "")
            {
                pContexte->getDico()->donneLibelleLexique(sLang, &sCode2, &sLibelle2);

                // pour les cas non traités
                if (sLibelle2 == "")
                    sLibElement = sElement;
                else
                    sLibElement = sLibelle2;
            }
            else // cas normal
                sLibElement = sLibelle2;
        }
        else // cas d'erreur
            sLibElement = sElement;
    }
}
boolNSListChemDialog::InitChemArray()
{
  string sCode;
  string sChemin;
  string sLibelle;

  pChemArray->vider();
  nbChem = 0;

  NSGroupGdInfo* pGroupeChoisi = (*pGroupArray)[GroupChoisi];
  string sCodeGroupe = pGroupeChoisi->getGroup() ;

  //
  // On stocke les chemins des fils guides appartenant à ce groupe
  // Création d'un BBFiche
  //
  BBFiche Fiche(pContexte->getSuperviseur()) ;
  Fiche.lastError = Fiche.open() ;
  if (Fiche.lastError != DBIERR_NONE)
  {
    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, Fiche.lastError) ;
    return false ;
  }

  // On cherche les fils guides de groupe sCodeGroupe
  string sCodeFG = sCodeGroupe + string(CH_CODE_LEN, ' ');
  Fiche.lastError = Fiche.chercheClef(&sCodeFG,
                                      "",
                                      0,
                                      keySEARCHGEQ,
                                      dbiWRITELOCK);

  if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_EOF))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return false ;
  }

  // on boucle pour récupérer tous les fils guides de type sCodeGroupe
  while (Fiche.lastError != DBIERR_EOF)
  {
    Fiche.lastError = Fiche.getRecord() ;
    if (Fiche.lastError != DBIERR_NONE)
    {
      erreur("Erreur à la lecture d'une fiche Guides.", standardError, Fiche.lastError) ;
      Fiche.close() ;
      return false ;
    }

    // Condition d'arret : on n'est plus sur le bon code groupe
    if (Fiche.getGroupID() != sCodeGroupe)
      break ;

    // On récupère ici le chemin du fil guide (en clair)
    BBItemData* pItemData = static_cast<BBItemData*>(Fiche.getData()) ;
    if (pItemData)
    {
      sCode   = pItemData->getID() ;
      sChemin = pItemData->getSemanticPath() ;
    }
    sLibelle = string("") ;

    TrouveLibElement(sChemin, sLibelle) ;

    pChemArray->push_back(new NSElement(sCode, sLibelle)) ;
    nbChem++ ;

    // On passe au fil guide suivant
    Fiche.lastError = Fiche.suivant(dbiWRITELOCK) ;
    if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_EOF))
    {
      erreur("Erreur d'accès à la fiche Guides suivante.", standardError, Fiche.lastError) ;
      Fiche.close() ;
      return false ;
    }
  }

  Fiche.lastError = Fiche.close() ;
  if (Fiche.lastError != DBIERR_NONE)
    erreur("Erreur de fermeture de la base des Guides.", standardError, Fiche.lastError) ;

  return true ;
}

boolNSListChemDialog::PermuteFG(int indexInf)
{
  BBItemInfo* fg1;
  BBItemInfo* fg2;
  string      sCodeFG, sCode;

  NSGroupGdInfo* pGroupeChoisi = (*pGroupArray)[GroupChoisi];
  string sCodeGroupe = pGroupeChoisi->getGroup() ;

  //
  // On enregistre les deux fils guide
  //
  BBFiche Fiche(pContexte->getSuperviseur()) ;
  Fiche.lastError = Fiche.open() ;
  if (DBIERR_NONE != Fiche.lastError)
  {
    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, Fiche.lastError) ;
    return false ;
  }

  sCodeFG = sCodeGroupe + ((*pChemArray)[indexInf])->getCode() ;
  Fiche.lastError = Fiche.chercheClef(&sCodeFG,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK);

  if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return false ;
  }
  else if (Fiche.lastError == DBIERR_RECNOTFOUND)
  {
    erreur("Le fil guide spécifié n'existe pas dans la base des guides.", standardError, 0) ;
    Fiche.close() ;
    return false ;
  }

  Fiche.lastError = Fiche.getRecord() ;
  if (Fiche.lastError != DBIERR_NONE)
  {
    erreur("Erreur à la lecture d'une fiche Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return false ;
  }

  fg1 = new BBItemInfo(&Fiche) ;
  sCode = fg1->getID() ;

  sCodeFG = sCodeGroupe + ((*pChemArray)[indexInf+1])->getCode() ;
  Fiche.lastError = Fiche.chercheClef(&sCodeFG,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK);

  if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return false ;
  }
  else if (Fiche.lastError == DBIERR_RECNOTFOUND)
  {
    erreur("Le fil guide spécifié n'existe pas dans la base des guides.", standardError, 0) ;
    Fiche.close() ;
    return false ;
  }

  Fiche.lastError = Fiche.getRecord() ;
  if (Fiche.lastError != DBIERR_NONE)
  {
    erreur("Erreur à la lecture d'une fiche Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return false ;
  }

  fg2 = new BBItemInfo(&Fiche) ;

  // on permute uniquement les codes avant de refaire un modifyRecord
  fg1->setID(fg2->getID()) ;
  fg2->setID(sCode) ;

  BBItemData* pItemData = static_cast<BBItemData*>(Fiche.getData()) ;
  if (pItemData)
    *pItemData = *(fg1->getData()) ;

  Fiche.lastError = Fiche.modifyRecord(TRUE);
  if (DBIERR_NONE != Fiche.lastError)
  {
    erreur("Erreur à la modification du fil guide.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return false ;
  }

  sCodeFG = sCodeGroupe + sCode;
  Fiche.lastError = Fiche.chercheClef(&sCodeFG,
                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK);

  if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return false ;
  }
  else if (Fiche.lastError == DBIERR_RECNOTFOUND)
  {
    erreur("Le fil guide spécifié n'existe pas dans la base des guides.", standardError, 0) ;
    Fiche.close() ;
    return false ;
  }

  pItemData = static_cast<BBItemData*>(Fiche.getData()) ;
  if (pItemData)
    *pItemData = *(fg2->getData()) ;

  Fiche.lastError = Fiche.modifyRecord(TRUE);
  if (DBIERR_NONE != Fiche.lastError)
  {
    erreur("Erreur à la modification du fil guide.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return false ;
  }
  Fiche.lastError = Fiche.close() ;  if (Fiche.lastError != DBIERR_NONE)
    erreur("Erreur de fermeture de la base des Guides.", standardError, Fiche.lastError) ;

  return true ;
}

voidNSListChemDialog::CmNewFG()
{
  NSGroupGdInfo* pGroupeChoisi = (*pGroupArray)[GroupChoisi];
  string sCodeGroupe = pGroupeChoisi->getGroup() ;

  NSEditFilGuideDialog* pEditFGDlg =
      new NSEditFilGuideDialog(this, pContexte, sCodeGroupe);

  if (pEditFGDlg->Execute() == IDOK)
  {
    string sCode = pEditFGDlg->sCodeFilGuide;
    string sLibelle = "";
    int lastIndex;
    TrouveLibElement(pEditFGDlg->sCheminPere, sLibelle);
    pChemArray->push_back(new NSElement(sCode, sLibelle));
    nbChem++;
    AfficheListeChem();
    lastIndex = pListeChem->GetItemCount() - 1;
    pListeChem->SetFocus();
    pListeChem->SetSel(lastIndex, true);
    pListeChem->EnsureVisible(lastIndex, true);
  }

  delete pEditFGDlg;
}

voidNSListChemDialog::CmModFG()
{
  NSGroupGdInfo* pGroupeChoisi = (*pGroupArray)[GroupChoisi] ;
  string sCodeGroupe = pGroupeChoisi->getGroup() ;

  int fgChoisi = pListeChem->IndexItemSelect() ;

  if (-1 == fgChoisi)
  {
    erreur("Vous devez sélectionner un fil guide à modifier.", standardError, 0) ;
    return ;
  }

  string sCodeFG = ((*pChemArray)[fgChoisi])->getCode() ;

  NSEditFilGuideDialog* pEditFGDlg =
        new NSEditFilGuideDialog(this, pContexte, sCodeGroupe, sCodeFG) ;

  if (pEditFGDlg->Execute() == IDOK)
  {
    string sLibelle = string("") ;
    TrouveLibElement(pEditFGDlg->sCheminPere, sLibelle) ;

    ((*pChemArray)[fgChoisi])->setLabel(sLibelle) ;

    AfficheListeChem() ;

    pListeChem->SetFocus() ;
    pListeChem->SetSel(fgChoisi, true) ;
    pListeChem->EnsureVisible(fgChoisi, true) ;
  }

  delete pEditFGDlg ;
}

void
NSListChemDialog::CmDupFG()
{
  int fgChoisi = pListeChem->IndexItemSelect() ;

  if (-1 == fgChoisi)
  {
    erreur("Vous devez sélectionner un fil guide à dupliquer.", standardError, 0) ;
    return ;
  }

  // cas de la duplication d'un fil guide
  // on commence par lire le fil guide source
  NSGroupGdInfo* pGroupeChoisi = (*pGroupArray)[GroupChoisi] ;
  string sCodeGroupe = pGroupeChoisi->getGroup() ;
  string sCodeFG     = sCodeGroupe + ((*pChemArray)[fgChoisi])->getCode() ;

  //  // Création d'un BBFiche  //  BBFiche Fiche(pContexte->getSuperviseur()) ;
  Fiche.lastError = Fiche.open() ;
  if (DBIERR_NONE != Fiche.lastError)
  {
    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, Fiche.lastError) ;
    return ;
  }

  Fiche.lastError = Fiche.chercheClef(&sCodeFG,                                      "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK) ;

  if ((DBIERR_NONE != Fiche.lastError) && (DBIERR_RECNOTFOUND != Fiche.lastError))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return ;
  }
  if (DBIERR_RECNOTFOUND == Fiche.lastError)
  {
    erreur("Le fil guide spécifié n'existe pas dans la base des guides.", standardError, 0) ;
    Fiche.close() ;
    return ;
  }

  Fiche.lastError = Fiche.getRecord() ;
  if (DBIERR_NONE != Fiche.lastError)
  {
    erreur("Erreur à la lecture d'une fiche Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return ;  }

  BBItemInfo fg1(&Fiche) ;

  // on cherche maintenant la première place disponible
  // pour insérer ce fil guide dans le même groupe
  string sCodePrecedent = fg1.getID() ;
  string sCompteur, sCode;

  while (DBIERR_EOF != Fiche.lastError)
  {
    sCompteur = sCodePrecedent ;

    if (false == Fiche.IncrementeCode(&sCompteur))
    {
      erreur("Erreur à l'attribution du code pour le nouveau fil guide.", standardError, 0) ;
      Fiche.close() ;
      return ;
    }

    sCode = sCodeGroupe + sCompteur;
    Fiche.lastError = Fiche.chercheClef(&sCode,
                                        "",
                                        0,
                                        keySEARCHGEQ,
                                        dbiWRITELOCK) ;

    if ((DBIERR_NONE != Fiche.lastError) && (DBIERR_EOF != Fiche.lastError))
    {
      erreur("Erreur à la recherche dans le fichier des Guides.", standardError, Fiche.lastError) ;
      Fiche.close() ;
      return ;
    }

    Fiche.lastError = Fiche.getRecord() ;
    if (DBIERR_NONE != Fiche.lastError)
    {
      erreur("Erreur à la lecture d'une fiche Guides.", standardError, Fiche.lastError) ;
      Fiche.close() ;
      return ;
    }

    // Condition d'arret : il existe un trou ou fin de groupe
    if ((Fiche.getGroupID() != sCodeGroupe) || (Fiche.getID() > sCompteur))
      break ;

    Fiche.lastError = Fiche.suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != Fiche.lastError) && (DBIERR_EOF != Fiche.lastError))
    {
      erreur("Erreur d'accès à la fiche Guides suivante.", standardError, Fiche.lastError) ;
      Fiche.close() ;
      return ;
    }

    Fiche.lastError = Fiche.getRecord() ;
    if (DBIERR_NONE != Fiche.lastError)
    {
      erreur("Erreur à la lecture d'une fiche Guides.", standardError, Fiche.lastError) ;
      Fiche.close() ;
      return ;
    }

    sCodePrecedent = Fiche.getID() ;
  } // fin du while

  // sCompteur correspond au premier code valide :
  // on le stocke dans fg1 et on stocke les donnees dans BBFiche
  fg1.setID(sCompteur) ;

  BBItemData* pItemData = static_cast<BBItemData*>(Fiche.getData()) ;
  if (pItemData)
    *pItemData = *(fg1.getData()) ;

  // on enregistre...
  Fiche.lastError = Fiche.appendRecord() ;
  if (DBIERR_NONE != Fiche.lastError)
  {
    erreur("Erreur à l'ajout du nouveau fil guide.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return ;
  }

  Fiche.lastError = Fiche.close() ;

  if (DBIERR_NONE != Fiche.lastError)
		erreur("Erreur de fermeture de la base des Guides.", standardError, Fiche.lastError) ;

  // on reporte, at last, la modif dans la liste
  // Note : dans la liste, l'élément sera inséré en queue
  // il faudra recharger le tableau pour l'avoir à sa place définitive
  NSElement ElementDuplique = *((*pChemArray)[fgChoisi]) ;
  ElementDuplique.setCode(sCompteur) ;

  pChemArray->push_back(new NSElement(ElementDuplique)) ;
  nbChem++ ;

  AfficheListeChem() ;
  int lastIndex = pListeChem->GetItemCount() - 1 ;
  pListeChem->SetFocus() ;
  pListeChem->SetSel(lastIndex, true) ;
  pListeChem->EnsureVisible(lastIndex, true) ;
}

voidNSListChemDialog::CmDelFG()
{

    NSEltIter i;
    int j;
    int fgChoisi = pListeChem->IndexItemSelect();

    if (fgChoisi == -1)
    {
        erreur("Vous devez sélectionner un fil guide à détruire.", standardError, 0) ;
        return;
    }

    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
    int retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Etes-vous sûr de vouloir détruire le fil guide sélectionné ?", sCaption.c_str(), MB_YESNO);
    if (retVal == IDNO)
        return;

    NSGroupGdInfo* pGroupeChoisi = (*pGroupArray)[GroupChoisi];
    string sCodeGroupe = pGroupeChoisi->getGroup() ;
    string sCodeFG = sCodeGroupe + ((*pChemArray)[fgChoisi])->getCode() ;

    //
    // Création d'un BBFiche
    //
    BBFiche* pBBFiche = new BBFiche(pContexte->getSuperviseur());
    pBBFiche->lastError = pBBFiche->open();
    if (pBBFiche->lastError != DBIERR_NONE)
    {
   	    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, pBBFiche->lastError) ;
        delete pBBFiche;
        return;
    }

    // On cherche les fils guides de groupe sCode
    // pour pouvoir les supprimer
    pBBFiche->lastError = pBBFiche->chercheClef(&sCodeFG,
                                        "",
                                        0,
                                        keySEARCHEQ,
                                        dbiWRITELOCK);

    if ((pBBFiche->lastError != DBIERR_NONE) && (pBBFiche->lastError != DBIERR_RECNOTFOUND))
    {
        erreur("Erreur à la recherche dans le fichier des Guides.", standardError, pBBFiche->lastError) ;
        pBBFiche->close();
        delete pBBFiche;
        return;
    }
    else if (pBBFiche->lastError == DBIERR_RECNOTFOUND)
    {
        erreur("Le fil guide spécifié n'existe pas dans la base des guides.", standardError, 0) ;
        pBBFiche->close();
        delete pBBFiche;
        return;
    }

    pBBFiche->lastError = pBBFiche->deleteRecord();
    if (pBBFiche->lastError != DBIERR_NONE)
    {
        erreur("Erreur à la destruction d'une fiche Guides.", standardError, pBBFiche->lastError) ;
        pBBFiche->close();
        delete pBBFiche;
        return;
    }

    pBBFiche->lastError = pBBFiche->close();
   	if (pBBFiche->lastError != DBIERR_NONE)
		erreur("Erreur de fermeture de la base des Guides.", standardError, pBBFiche->lastError) ;

	delete pBBFiche;

    // on enlève le fil guide de pChemArray
    for (i = pChemArray->begin(), j = 0; i != pChemArray->end(); i++, j++)
    {
        if (j == fgChoisi)
        {
            delete *i;
            pChemArray->erase(i);
            nbChem -= 1;
            break;
        }
    }

    AfficheListeChem();
}
voidNSListChemDialog::CmHaut()
{
  int fgChoisi = pListeChem->IndexItemSelect() ;
  if (-1 == fgChoisi)
  {
    erreur("Vous devez sélectionner un fil guide à déplacer.", standardError, 0) ;
    return ;  }
  if (0 == fgChoisi)    return ;
  string sChemin = ((*pChemArray)[fgChoisi])->getLabel() ;
  if (false == PermuteFG(fgChoisi-1))  {    erreur("Echec dans la permutation de deux fils guide.",standardError, 0) ;
    return ;
  }

  ((*pChemArray)[fgChoisi])->setLabel(((*pChemArray)[fgChoisi-1])->getLabel()) ;  ((*pChemArray)[fgChoisi-1])->setLabel(sChemin) ;

  AfficheListeChem() ;
  pListeChem->SetFocus() ;  pListeChem->SetSel(fgChoisi-1, true) ;
  pListeChem->EnsureVisible(fgChoisi-1, true) ;
}

voidNSListChemDialog::CmBas()
{
  int fgChoisi = pListeChem->IndexItemSelect() ;

  if (-1 == fgChoisi)  {
    erreur("Vous devez sélectionner un fil guide à déplacer.", standardError, 0) ;
    return ;
  }

  if (nbChem - 1 == fgChoisi)    return ;

  string sChemin = ((*pChemArray)[fgChoisi])->getLabel() ;
  if (false == PermuteFG(fgChoisi))  {
    erreur("Echec dans la permutation de deux fils guide.",standardError, 0) ;
    return ;
  }

  ((*pChemArray)[fgChoisi])->setLabel(((*pChemArray)[fgChoisi+1])->getLabel()) ;  ((*pChemArray)[fgChoisi+1])->setLabel(sChemin) ;

  AfficheListeChem() ;
  pListeChem->SetFocus() ;  pListeChem->SetSel(fgChoisi+1, true) ;
  pListeChem->EnsureVisible(fgChoisi+1, true) ;
}


voidNSListChemDialog::CmNewGP()
{
  NSGroupGd Group(pContexte->getSuperviseur()) ;
  Group.lastError = Group.open() ;
  if (DBIERR_NONE != Group.lastError)
  {
    erreur("Impossible d'ouvrir la base GroupGd.db", standardError, Group.lastError) ;
    return ;
  }

  Group.lastError = Group.fin(dbiWRITELOCK);
  if (DBIERR_NONE != Group.lastError)
  {
    erreur("Erreur au positionnement dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return ;
  }

  Group.lastError = Group.getRecord() ;
  if (DBIERR_NONE != Group.lastError)
  {
    erreur("Erreur à la lecture dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return ;
  }

  string sCode = Group.getGroup() ;

  if ('$' != sCode[0])
  {
    if (false == Group.IncrementeCode(&sCode))
    {
      erreur("Erreur à l'attribution d'un nouveau code groupe.", standardError, 0) ;
      Group.close() ;
      return ;
    }
  }
  else    // cas du premier groupe utilisateur
    sCode = string("000") ;

  NSCreateGdDialog* pDlg = new NSCreateGdDialog(this, pContexte, pNSResModule) ;
  strcpy(pDlg->pData->groupe, sCode.c_str()) ;

  if (pDlg->Execute() == IDCANCEL)
  {
    delete pDlg ;
    Group.close() ;
    return ;
  }

  // le code n'existe pas encore => on ajoute la fiche groupe
  Group._Donnees = *(pDlg->pData) ;

  delete pDlg ;

  Group.lastError = Group.appendRecord() ;
  if (DBIERR_NONE != Group.lastError)
  {
    erreur("Erreur à l'ajout dans la base GroupGd.db", standardError, Group.lastError) ;
    Group.close() ;
    return ;
  }

  // !!! on met à jour la GroupBox s'il s'agit d'un nouveau groupe utilisateur
  // ce test n'est plus nécéssaire mais on le garde pour mémoire
  //
  sCode = Group.getGroup() ;

  if ('$' != sCode[0])
  {
    pGroupBox->AddString(Group.getLabel().c_str()) ;
    pGroupArray->push_back(new NSGroupGdInfo(&Group)) ;
    pGroupBox->SetSelIndex(pGroupBox->GetCount() - 1) ;    EvGroupChange() ;
  }

  // on ferme la base
  Group.lastError = Group.close() ;
  if (DBIERR_NONE != Group.lastError)
    erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, Group.lastError) ;
}

void
NSListChemDialog::CmModGP()
{
  // on passe les data au dialogue de modification des groupes
  NSGroupGdInfo* pGroupeChoisi = (*pGroupArray)[GroupChoisi] ;
  NSCreateGdDialog* pDlg = new NSCreateGdDialog(this, pContexte, pNSResModule) ;
  *(pDlg->pData) = pGroupeChoisi->_Donnees ;

  if (pDlg->Execute() == IDCANCEL)
  {
    delete pDlg ;
    return ;
  }

  string sCode = string(pDlg->pData->groupe) ;

  // on regarde si le code existe bien
  // avant de modifier la nouvelle fiche groupe
  NSGroupGd* pGroup = new NSGroupGd(pContexte->getSuperviseur()) ;
  pGroup->lastError = pGroup->open() ;
  if (pGroup->lastError != DBIERR_NONE)
  {
    erreur("Impossible d'ouvrir la base GroupGd.db", standardError, pGroup->lastError) ;
    delete pGroup ;
    delete pDlg ;
    return ;
  }

  pGroup->lastError = pGroup->chercheClef(&sCode, "", 0, keySEARCHEQ, dbiWRITELOCK) ;
  if ((pGroup->lastError != DBIERR_NONE) && (pGroup->lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans la base GroupGd.db", standardError, pGroup->lastError) ;
    pGroup->close() ;
    delete pGroup ;
    delete pDlg ;
    return ;
  }
  else if (pGroup->lastError == DBIERR_RECNOTFOUND)  // cas où le code n'existe plus
  {
    erreur("Impossible de retrouver le groupe à modifier dans GroupGd.db", standardError, 0) ;
    pGroup->close() ;
    delete pGroup ;
    delete pDlg ;
    return ;
  }

  // on est sur le bon Record => on modifie la fiche groupe
  pGroup->_Donnees = *(pDlg->pData) ;

  pGroup->lastError = pGroup->modifyRecord(TRUE) ;
  if (pGroup->lastError != DBIERR_NONE)
  {
    erreur("Erreur à la modification dans la base GroupGd.db", standardError, pGroup->lastError) ;
    pGroup->close() ;
    delete pGroup ;
    delete pDlg ;
    return ;
  }

  // on ferme la base
  pGroup->lastError = pGroup->close() ;
  if (pGroup->lastError != DBIERR_NONE)
  	erreur("Erreur à la fermeture dans la base GroupGd.db", standardError, pGroup->lastError) ;
  delete pGroup ;
  delete pDlg ;
}


void

NSListChemDialog::CmOk()
{
    NSUtilDialog::CmOk();
}

void
NSListChemDialog::CmCancel()
{
    NSUtilDialog::CmCancel();
}
// -----------------------------------------------------------------
//
//  Méthodes de NSCreateGdDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSCreateGdDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSCreateGdDialog::NSCreateGdDialog(TWindow* pere, NSContexte* pCtx, TModule* mod)
                    :NSUtilDialog(pere, pCtx, "IDD_CREATEGD", mod)
{
  // fichiers d'aide
  sHindex = "" ;
  sHcorps = "Utilisateur.html" ;

  // Initialisation des donnees  pData 		  = new NSGroupGdData ;
	// Création de tous les "objets de contrôle"	pCodeGd 	  = new NSUtilEdit(pContexte, this, IDC_CREATEGD_CODE, GRPGD_GROUPE_LEN) ;
	pLibelleGd  = new NSUtilEdit(pContexte, this, IDC_CREATEGD_LIBELLE, GRPGD_LIBELLE_LEN) ;
  pUtilGd		  = new NSUtilEdit(pContexte, this, IDC_CREATEGD_UTIL, GRPGD_UTIL_LEN) ;
  pDateGd		  = new NSUtilEditDate(pContexte, this, IDC_CREATEGD_DATE) ; // défaut 00/00/2000
}

NSCreateGdDialog::~NSCreateGdDialog(){
  delete pDateGd ;
  delete pCodeGd ;
  delete pLibelleGd ;
  delete pUtilGd ;
  delete pData ;
}

//----------------------------------------------------------------------------


void
NSCreateGdDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow();

    pCodeGd->SetText(pData->groupe);
    pLibelleGd->SetText(pData->libelle);
    pUtilGd->SetText(pData->util);
    pDateGd->setDate(string(pData->date));
}

//----------------------------------------------------------------------------

void
NSCreateGdDialog::CmOk()
{
  bool bCode = false ;
  bool bLib = false ;

  // en théorie, le code groupe ne peut pas changer, mais bon...
  if (strcmp(pData->groupe, ""))
    bCode = true ;

  string sLibelle = pLibelleGd->GetText(GRPGD_LIBELLE_LEN) ;
  strcpy(pData->libelle, sLibelle.c_str()) ;
  if (strcmp(pData->libelle, ""))
    bLib = true ;

  string sUtil = pUtilGd->GetText(GRPGD_UTIL_LEN) ;
  strcpy(pData->util, sUtil.c_str()) ;

  string sDateGd ;
  pDateGd->getDate(&sDateGd) ;
  strcpy(pData->date, sDateGd.c_str()) ;

  if (false == bCode)
    erreur("Le champ [Code groupe] est obligatoire.", standardError, 0, GetHandle()) ;
  else if (!bLib)
  	erreur("Le champ [Libellé groupe] est obligatoire.", standardError, 0, GetHandle()) ;
  else
  	NSUtilDialog::CmOk() ;
}


//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
NSCreateGdDialog::CmCancel()
{
    NSUtilDialog::CmCancel();
}

// fin de nsfilgui.cpp
/////////////////////////////////////////////////////////////////////////////

