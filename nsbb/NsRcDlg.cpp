//**************************************************************************//  NSRCDLG.CPP
//**************************************************************************
#include <iostream.h>
#include <typeinfo.h>
#include <string.h>

#include <owl/owlpch.h>
#include <owl/edit.h>
#include <owl/listbox.h>

//#include <regexp.h>
#include <cstring.h>
#include <bwcc.h>

#include "nautilus\nssuper.h"
#include "nsbb\nsbb.h"
#include "partage\nsglobal.h"
#include "partage\ns_timer.h"
#include "nsbb\nsarc.h"
#include "nsbb\nsrcdlg.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsbbtran.h"
#include "nssavoir\ns_fic.h"
#include "nssavoir\nsgraphe.h"
#include "dcodeur\decoder.h"

const int ID_RCList     = 0x700;
const int ID_VoirList   = 0x701;
const int ID_RCE        = 0x702;
const int ID_PD_Group   = 0x703;
const int ID_PD_A       = 0x704;
const int ID_PD_B       = 0x705;
const int ID_PD_C       = 0x706;
const int ID_PD_D       = 0x707;
const int ID_PD_Z       = 0x708;

// Table de réponses aux messages
DEFINE_RESPONSE_TABLE1(NSRCDialog, NSDialog)
    EV_LBN_DBLCLK       (ID_RCList,    dblClickRC),
    EV_LBN_SELCHANGE    (ID_RCList,    selChangeRC),
    EV_LBN_DBLCLK       (ID_VoirList,  dblClickVA),
    EV_LBN_SELCHANGE    (ID_VoirList,  selChangeVA),
    EV_WM_CHAR,
    EV_WM_TIMER,
END_RESPONSE_TABLE ;

//---------------------------------------------------------------------------
//  Constructeur de NSRCDialog à partir d'un rcID
//---------------------------------------------------------------------------
#ifdef __OB1__
NSRCDialog::NSRCDialog(NSContexte *pCtx, TWindow* AParent, TResId ResourceID, TResId ConfResID, BBItem* pItem,
                    TModule* pResModule, NsMultiDialog* pControlMulti)            :NSDialog(pCtx, AParent, ResourceID, ConfResID, pItem, pResModule, pControlMulti)#elseNSRCDialog::NSRCDialog(NSContexte *pCtx, TWindow* AParent, TResId ResourceID, TResId ConfResID, BBItem* pItem,                    TModule* pResModule, NsMultiDialog* pControlMulti, bool initFromBbk)            :NSDialog(pCtx, AParent, ResourceID, ConfResID, pItem, pResModule, pControlMulti, initFromBbk)#endif{
    sCurrentLex         = "" ;
    sCurrentArchetype   = "" ;

    pRCEdit     = 0 ;
    pRCList     = 0 ;
    pRCE        = 0 ;
    pVoirAussi  = 0 ;

    initRCArray() ;

    bSettedUp   = false ;

    iLargeurColonne = 165 ;
    iHauteurColonne = 0 ;
    iMinimalHeight  = 0 ;

    pFiche      = new NSPatPathoArray(pCtx->getSuperviseur()) ;
}

//---------------------------------------------------------------------------
//  Constructeur de NSRCDialog avec création dynamique de contrôles
//---------------------------------------------------------------------------
#ifdef __OB1__
NSRCDialog::NSRCDialog(NSContexte *pCtx, TWindow* AParent, BBItem* pItem, TModule* pResModule, NsMultiDialog* pControlMulti)            :NSDialog(pCtx, AParent, pItem, pResModule, pControlMulti)
#else
NSRCDialog::NSRCDialog(NSContexte *pCtx, TWindow* AParent, BBItem* pItem, TModule* pResModule, NsMultiDialog* pControlMulti, bool initFromBbk)
            :NSDialog(pCtx, AParent, pItem, pResModule, pControlMulti, initFromBbk)
#endif
{
    sCurrentLex         = "" ;
    sCurrentArchetype   = "" ;

    pRCEdit     = 0 ;
    pRCList     = 0 ;
    pRCE        = 0 ;
    pVoirAussi  = 0 ;

    initRCArray() ;

    bSettedUp   = false ;

    iLargeurColonne = 165 ;
    iHauteurColonne = 0 ;
    iMinimalHeight  = 0 ;

    pFiche      = new NSPatPathoArray(pCtx->getSuperviseur()) ;
}

//---------------------------------------------------------------------------
//  Constructeur de NSRCDialog avec création dynamique de contrôles
//---------------------------------------------------------------------------
NSRCDialog::NSRCDialog(NSContexte *pCtx, BBItem* pItem, TWindow* AParent)
#ifdef __OB1__
            :NSDialog(pCtx, AParent, pItem, 0, 0)
#else
            :NSDialog(pCtx, AParent, pItem, 0, 0, false)
#endif
{
    sCurrentLex         = "" ;
    sCurrentArchetype   = "" ;

    pRCEdit     = 0 ;
    pRCList     = 0 ;
    pRCE        = 0 ;
    pVoirAussi  = 0 ;

    initRCArray() ;

    bSettedUp   = false ;

    iLargeurColonne = 165 ;
    iHauteurColonne = 0 ;
    iMinimalHeight  = 0 ;

    pFiche      = new NSPatPathoArray(pCtx->getSuperviseur()) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSRCDialog::~NSRCDialog()
{
	Desactive() ;

    delete pRCEdit ;
    delete pRCList ;
    delete pRCE ;
    delete pVoirAussi ;

    delete pFiche ;
}

//---------------------------------------------------------------------------
//  SetupWindow()
//---------------------------------------------------------------------------
void
NSRCDialog::SetupWindow()
{
try
{
	NSSuper *pSuper = pContexte->getSuperviseur() ;

  string ps = string("Entrée dans SetupWindow de NSRCDialog") ;
  pSuper->trace(&ps, 1) ;

  if (sCurrentLex == "")
  {
  	if (false == aRCs.empty())
    	sCurrentLex = (*(aRCs.begin()))->sLexique ;
  }
  if (sCurrentLex != "")
  {
  	donneFiche() ;

    donneArchetype(sCurrentLex) ;
    if (sCurrentArchetype != "")
    	switchArchetype(sCurrentArchetype) ;
  }

  // ... puis on ajoute les composants spécifiques
  // ... then we add the specific components
  if (!bSettedUp)
  {
  	pRCGroup        = new TGroupBox(this, -1, "&RC", 4, 15, 250, 200) ;
    pRCList         = new TListBox(this, ID_RCList, 4, 35, 250, 190, pNSDLLModule) ;
    pVAGroup        = new TGroupBox(this, -1, "&Voir aussi", 4, 220, 250, 200) ;
    pVoirAussi      = new TListBox(this, ID_VoirList, 4, 240, 250, 190, pNSDLLModule) ;
    pRCEGroup       = new TGroupBox(this, -1, "RC&E", 4, 425, 250, 90) ;
    pRCE            = new TListBox(this, ID_RCE, 4, 445, 250, 80, pNSDLLModule) ;

    pPosDiagGroup   = new TGroupBox(this, ID_PD_Group, "&Position diagnostique", 4, 525, 250, 100) ;
    pPosDiag[0] = new NSRadioButton(pContexte, this, ID_PD_A, "&A Symptôme", 8, 550, 80, 12, pPosDiagGroup) ;
    pPosDiag[1] = new NSRadioButton(pContexte, this, ID_PD_A+1, "&B Syndrome", 8, 566, 80, 12, pPosDiagGroup) ;
    pPosDiag[2] = new NSRadioButton(pContexte, this, ID_PD_A+2, "&C Tableau de maladie", 100, 550, 130, 12, pPosDiagGroup) ;
    pPosDiag[3] = new NSRadioButton(pContexte, this, ID_PD_A+3, "&D Diagnostic certifié", 100, 566, 130, 12, pPosDiagGroup) ;
    pPosDiag[4] = new NSRadioButton(pContexte, this, ID_PD_A+4, "&Z Non morbide", 8, 598, 160, 12, pPosDiagGroup) ;

    iMinimalHeight = 400 ;
  }

  // D'abord, on laisse la NSDialog se construire...
  // First, we let the NSDialog build itself...
  //
  NSDialog::SetupWindow() ;

  // Préparation de la fonction d'énumération
  WNDENUMPROC lpEnumDlgFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) NSRCDialog::RCDlgConfigCtrl, hInstance) ;

  // On passe comme paramètre de EnumChildWindows LPARAM(this), l'adresse de
  // this puisque dans les fonctions static on ne peut pas récupérer le
  // pointeur this.
  EnumChildWindows(HWindow, lpEnumDlgFunc, LPARAM((TWindow *)this)) ;

  Cdialogbox* pDialogBox = 0 ;
  NS_CLASSLIB::TRect dlgSizeRect(0, 0, iLargeurColonne, iHauteurColonne) ;

  if (_pBBItem->getParseur())
  {
  	string sLang = pContexte->getUserLanguage() ;

    pDialogBox = _pBBItem->getArchetypeDialog(sLang) ;

    if (pDialogBox)
    {
    	dlgSizeRect = dlgSizeRect.InflatedBy(pDialogBox->getW() / 2, pDialogBox->getH() / 2) ;
      if (dlgSizeRect.Height() < iMinimalHeight)
      	dlgSizeRect.SetWH(dlgSizeRect.Left(), dlgSizeRect.Top(), dlgSizeRect.Width(), iMinimalHeight) ;

      MapDialogRect(dlgSizeRect) ;
    }
  }

  TDialog::SetupWindow() ;

  //
  // Redimentionnement
  //
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect clientRect ;
  GetClientRect(clientRect) ;
  //
  // On compare le clientRect réel avec les dimensions souhaitées,
  // et on modifie le WindowRect en conséquence
  //
  int nouvWindowWidth  = dlgRect.Width()  + (dlgSizeRect.Width()  - clientRect.Width()) ;
  int nouvWindowHeight = dlgRect.Height() + (dlgSizeRect.Height() - clientRect.Height()) ;
  MoveWindow(dlgRect.left, dlgRect.top, nouvWindowWidth, nouvWindowHeight, true) ;

  if (!bSettedUp)
  {
  	initRCList() ;
    pRCList->SetFocus() ;
  }
  initVoirAussi() ;
  initRCE() ;
  initPosDiag() ;
  actifRCList() ;

  bSettedUp   = true ;

  ps = string("Sortie de SetupWindow de NSRCDialog") ;
  pSuper->trace(&ps, 1) ;
}
catch (...)
{
	erreur("Exception NSRCDialog::SetupWindow", standardError, 0) ;
  return ;
}
}

//---------------------------------------------------------------------------
//  Initialisation de la liste des RC
//---------------------------------------------------------------------------
void
NSRCDialog::initRCList()
{
    pRCList->ClearList() ;

    if (aRCs.empty())
        return ;

    RCElemIter itRCs = aRCs.begin() ;
    for ( ; itRCs != aRCs.end() ; itRCs++)
        pRCList->AddString((*itRCs)->sLibelle.c_str()) ;
}

void
NSRCDialog::donneFiche()
{
try
{
    pFiche->vider() ;

    if (sCurrentLex == "")
        return ;

    NSContexte* pCtx = _pBBItem->_pBigBoss->pContexte ;

    string sCodeSens ;
    pCtx->getDico()->donneCodeSens(&sCurrentLex, &sCodeSens) ;

    NSFicheManager* pFichMan = new NSFicheManager(pCtx->getSuperviseur()) ;
    bool bSuccess = pFichMan->getFiche(sCodeSens, pFiche) ;
    delete pFichMan ;

    if (!bSuccess)
        pFiche->vider() ;
}
catch (...)
{
	erreur("Exception (NSRCDialog::donneFiche).", standardError, 0);
}
}

void
NSRCDialog::donneArchetype(string sLexique)
{
try
{
    sCurrentArchetype = "" ;

    NSSuper *pSuper = pContexte->getSuperviseur() ;

    //recherche sémantique
    VecteurRechercheSelonCritere* pVecteurSelonCritere = new VecteurRechercheSelonCritere(GUIDE) ;
    string sCodeSens ;
    NSDico::donneCodeSens(&sLexique, &sCodeSens) ;
    pVecteurSelonCritere->AjouteEtiquette(sCodeSens) ;

    string sLocalisation = _pBBItem->_sLocalisation ;
    pSuper->getFilGuide()->chercheChemin(&sLocalisation ,
                             pVecteurSelonCritere, NSFilGuide :: compReseau) ;

    bool        trouve ;
    BBItemData* pDonnees = new BBItemData ;
    pVecteurSelonCritere->SetData(sCodeSens, &trouve, pDonnees) ;
    string sEtiquette ;
    if (trouve)
    {
        if (pDonnees->ouvreDialogue[0] == 'A')
            sCurrentArchetype = string(pDonnees->fils) ;
    }

    delete pDonnees ;
    delete pVecteurSelonCritere ;
}
catch (...)
{
	erreur("Exception NSRCDialog::donneArchetype", standardError, 0) ;
    return ;
}
}

void
NSRCDialog::switchArchetype(string _sArchetype)
{
try
{
  detruitControles() ;

  Message msg ;
  _pBBItem->DestructionManuelle(&msg) ;
  _pBBItem->_aBBItemFils.vider() ;
  _aNSCtrls.vider() ;

  // Préparation de la fonction d'énumération qui remet en place
  WNDENUMPROC lpEnumDlgFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) NSRCDialog::RCDlgUnconfigCtrl, hInstance) ;
  EnumChildWindows(HWindow, lpEnumDlgFunc, LPARAM((TWindow *)this)) ;

  if (_pBBItem->getParseur())
  {
    delete _pBBItem->getParseur() ;
    _pBBItem->setParseur((nsarcParseur*) 0) ;
  }

  if (string("") == _sArchetype)
    return ;

  string sArchetypeFile = pContexte->getSuperviseur()->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::archetype, _sArchetype) ;
  if (string("") == sArchetypeFile)
    return ;

  nsarcParseur *pNewParseur = new nsarcParseur(pContexte->getSuperviseur()) ;

  _pBBItem->setParseur(pNewParseur) ;
  if (pNewParseur->open(sArchetypeFile))
  {
    // on développe tout l'archetype à partir du BBItem root
    //
    _pBBItem->setItemOpenArchetype(true) ;

    _pBBItem->setDialogName(string("")) ;
    _pBBItem->setItemDialogFile(string("")) ;
    _pBBItem->_sIdArchetype = pNewParseur->getArchetype()->getName() ;

    //
    // Penser à transmettre "false" pour la paramètre Consultation, sinon
    // la patpatho n'est pas dispatchée
    //
    _pBBItem->creerArchetype(pNewParseur->getArchetype()->getRootItem(), false) ;
    // pBBItem->DispatcherPatPathoIteratif() ;
  }
  return ;
}
catch (...)
{
	erreur("Exception NSRCDialog::switchArchetype", standardError, 0) ;
  return ;
}
}

void
NSRCDialog::initRCArray()
{
try
{
    aRCs.vider() ;

    NSSuper*    pSuper      = pContexte->getSuperviseur() ;

    // VectString aVecteurString ;
    // pSuper->pFilGuide->TousLesVrais("ERC00", "E0", &aVecteurString, "ENVERS") ;    VecteurString aVecteurString ;    pSuper->getFilGuide()->chercheEquivalent("ERC00", &aVecteurString, "E0", "ENVERS") ;

    if (aVecteurString.empty())
        return ;

    string sLang = pContexte->getUserLanguage() ;

    int iNumCode = 0 ;
    // IterString itCodes = aVecteurString.begin() ;
    EquiItemIter itCodes = aVecteurString.begin() ;
    for ( ; itCodes != aVecteurString.end() ; itCodes++)
    {
        string sCode = **itCodes + string("1") ;

        string sLibelleTrouve;
        pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleTrouve);

        aRCs.push_back(new NSRCElement(iNumCode, sCode, sLibelleTrouve)) ;

        iNumCode++ ;
    }
}
catch (...)
{
	erreur("Exception NSRCDialog::initRCArray", standardError, 0);
    return ;
}
}

void
NSRCDialog::initVoirAussi()
{
  pVoirAussi->ClearList() ;
  aRCAussi.vider() ;

  if (pFiche->empty())
    return ;

  PatPathoIter itPpt ;
  bool bVA = pFiche->CheminDansPatpatho("ERC001/0VOIR1", string(1, cheminSeparationMARK), &itPpt) ;

  if (false == bVA)
    return ;

  string sLang = pContexte->getUserLanguage() ;

  int iNumCode = 0 ;
  int iRefCol  = (*itPpt)->getColonne() ;
  itPpt++ ;
  while ((itPpt != pFiche->end()) && ((*itPpt)->getColonne() > iRefCol))
  {
    string sCode = (*itPpt)->getLexique() ;

    string sLibelleTrouve ;
    pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleTrouve) ;

    aRCAussi.push_back(new NSRCElement(iNumCode, sCode, sLibelleTrouve)) ;

    itPpt++ ;
    iNumCode++ ;
  }

  if (aRCAussi.empty())
    return ;

  RCElemIter itRCs = aRCAussi.begin() ;
  for ( ; aRCAussi.end() != itRCs ; itRCs++)
    pVoirAussi->AddString((*itRCs)->sLibelle.c_str()) ;
}

void
NSRCDialog::actifRCList()
{
    if (sCurrentLex == "")
        return ;

    //
    // On pourrait utiliser SetSelString, mais il n'existe pas de sélection exacte
    // We could use SetSelString but there is no exact comparison
    //

    RCElemIter itSelected = aRCs.findByCode(sCurrentLex) ;
    if ((itSelected == NULL) || (itSelected == aRCs.end()))
        return ;

    int iPosit = pRCList->FindExactString((*itSelected)->sLibelle.c_str(), 0) ;
    if (iPosit >= 0)
        pRCList->SetSelIndex(iPosit) ;
}

void
NSRCDialog::initRCE()
{
  pRCE->ClearList() ;
  aRCE.vider() ;

  if (pFiche->empty())
      return ;

  PatPathoIter itPpt ;
  bool bVA = pFiche->CheminDansPatpatho("ERC001/0RCE01", string(1, cheminSeparationMARK), &itPpt) ;

  if (!bVA)
      return ;

  string sLang = pContexte->getUserLanguage() ;

  NSPatPathoArray aTempPatho(pContexte->getSuperviseur()) ;
  GlobalDkd Dcode(pContexte, sLang, "", &aTempPatho) ;
  string sLibelleTrouve ;

  int iNumCode = 0 ;
  int iRefCol  = (*itPpt)->getColonne() ;
  itPpt++ ;
  while ((itPpt != pFiche->end()) && ((*itPpt)->getColonne() > iRefCol))
  {
      string sCode = (*itPpt)->getLexique() ;

      aTempPatho.vider() ;
      aTempPatho.push_back(new NSPatPathoInfo(**itPpt)) ;
      Dcode.decodeNoeud("", true) ;
      sLibelleTrouve = Dcode.getDcodeur() ;

      // aRCE.push_back(new NSRCElement(iNumCode, sCode, sLibelleTrouve)) ;
      aRCE.push_back(new string(sLibelleTrouve)) ;

      itPpt++ ;
      iNumCode++ ;
  }

  if (aRCE.empty())
    return ;

  IterString itRCE = aRCE.begin() ;
  for ( ; itRCE != aRCE.end() ; itRCE++)
      pRCE->AddString((*itRCE)->c_str()) ;

  //RCElemIter itRCs = aRCE.begin() ;
  //for ( ; itRCs != aRCE.end() ; itRCs++)
  //    pRCE->AddString((*itRCs)->sLibelle.c_str()) ;
}

void
NSRCDialog::initPosDiag()
{
    uint32 styleNormal = BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP ;
    uint32 styleGrayed = styleNormal | WS_DISABLED ;

    for (int i = 0; i < 5; i++)
    {
        pPosDiag[i]->TWindow::SetStyle(styleGrayed) ;
        pPosDiag[i]->Invalidate() ;
        pPosDiag[i]->SetCheck(BF_UNCHECKED) ;
        pPosDiag[i]->Show(SW_HIDE) ;
    }

    if (pFiche->empty())
    {
        Invalidate() ;
        return ;
    }

    PatPathoIter itPpt ;
    bool bVA = pFiche->CheminDansPatpatho("ERC001/0POSD1", string(1, cheminSeparationMARK), &itPpt) ;

    if (!bVA)
    {
        Invalidate() ;
        return ;
    }

    int iRefCol     = (*itPpt)->getColonne() ;
    itPpt++ ;
    while ((itPpt != pFiche->end()) && ((*itPpt)->getColonne() > iRefCol))
    {
        string sCode = (*itPpt)->getLexique() ;

        string sCodeSens ;
        pContexte->getDico()->donneCodeSens(&sCode, &sCodeSens) ;

        if      (sCodeSens == "JPODA")
        {
            pPosDiag[0]->TWindow::SetStyle(styleNormal) ;
            pPosDiag[0]->Invalidate() ;
            pPosDiag[0]->Show(SW_SHOW) ;
        }
        else if (sCodeSens == "JPODB")
        {
            pPosDiag[1]->TWindow::SetStyle(styleNormal) ;
            pPosDiag[1]->Invalidate() ;
            pPosDiag[1]->Show(SW_SHOW) ;
        }
        else if (sCodeSens == "JPODC")
        {
            pPosDiag[2]->TWindow::SetStyle(styleNormal) ;
            pPosDiag[2]->Invalidate() ;
            pPosDiag[2]->Show(SW_SHOW) ;
        }
        else if (sCodeSens == "JPODD")
        {
            pPosDiag[3]->TWindow::SetStyle(styleNormal) ;
            pPosDiag[3]->Invalidate() ;
            pPosDiag[3]->Show(SW_SHOW) ;
        }
        else if (sCodeSens == "JPODZ")
        {
            pPosDiag[4]->TWindow::SetStyle(styleNormal) ;
            pPosDiag[4]->Invalidate() ;
            pPosDiag[4]->Show(SW_SHOW) ;
        }

        itPpt++ ;
    }
    Invalidate() ;
}

void
NSRCDialog::dblClickRC()
{
    KillTimer(ID_RCTIMER) ;

    int iSelectedRC = (int) pRCList->GetSelIndex();
    if (iSelectedRC < 0)
        return ;

    char szSelected[256] ;
    int iLibSize = pRCList->GetSelString(szSelected, 255) ;
    if (iLibSize < 2)
        return ;

    RCElemIter itSelected = aRCs.findByLib(string(szSelected)) ;
    if ((itSelected == NULL) || (itSelected == aRCs.end()))
        return ;

    sCurrentLex = (*itSelected)->sLexique ;

    if (sCurrentLex != "")
        changeRC(sCurrentLex) ;
}

void
NSRCDialog::dblClickVA()
{
    KillTimer(ID_RCTIMER) ;

    int iSelectedRC = (int) pVoirAussi->GetSelIndex();
    if (iSelectedRC < 0)
        return ;

    char szSelected[256] ;
    int iLibSize = pVoirAussi->GetSelString(szSelected, 255) ;
    if (iLibSize < 2)
        return ;

    RCElemIter itSelected = aRCAussi.findByLib(string(szSelected)) ;
    if ((itSelected == NULL) || (itSelected == aRCs.end()))
        return ;

    sCurrentLex = (*itSelected)->sLexique ;

    if (sCurrentLex != "")
        changeRC(sCurrentLex) ;

    pRCList->SetFocus() ;
}

void
NSRCDialog::selChangeRC()
{
    KillTimer(ID_RCTIMER) ;
    bRcSelected = true ;
    SetTimer(ID_RCTIMER, 300) ;
}

void
NSRCDialog::selChangeVA()
{
    KillTimer(ID_RCTIMER) ;
    bRcSelected = false ;
    SetTimer(ID_RCTIMER, 300) ;
}

void
NSRCDialog::changeRC(string sLexique)
{
    if (sCurrentLex == "")
        return ;

    donneArchetype(sCurrentLex) ;
    if (sCurrentArchetype == "")
        return ;

    donneFiche() ;
    initVoirAussi() ;
    initRCE() ;
    actifRCList() ;

    switchArchetype(sCurrentArchetype) ;
    SetupWindow() ;
    Invalidate() ;
}

void
NSRCDialog::addCurrentRcToHistory()
{
  aRcHistory.AddString(sCurrentLex) ;
}

//-------------------------------------------------------------------------
// We'll use a timer message to update our drag image instead of MouseMove
// messages.
//-------------------------------------------------------------------------
void
NSRCDialog::EvTimer(uint id)
{
  	if (id != ID_RCTIMER)
        return ;

    if (bRcSelected)
  	    dblClickRC() ;
    else
        dblClickVA() ;
}


void
NSRCDialog::EvChar(uint key, uint repeatCount, uint flags)
{
    if (key == VK_SPACE)
        return ;

    EvChar(key, repeatCount, flags) ;
}

bool FAR PASCAL _export NSRCDialog::RCDlgConfigCtrl(HWND hWnd, LPARAM lParam)
{
  //
	// Adresse de l'objet boîte de dialogue courante
	//
	TWindow* pWnd = reinterpret_cast<TWindow*>(lParam);
	NSRCDialog* pDlgCurrent = dynamic_cast<NSRCDialog*>(pWnd);

  if (NULL == pDlgCurrent)
    return true ;

  NSContexte *pContexte = pDlgCurrent->pContexte ;

  if (((pDlgCurrent->pRCList)     && (hWnd == pDlgCurrent->pRCList->HWindow)) ||
      ((pDlgCurrent->pVoirAussi)  && (hWnd == pDlgCurrent->pVoirAussi->HWindow)) ||
      ((pDlgCurrent->pRCE)        && (hWnd == pDlgCurrent->pRCE->HWindow)) ||
      ((pDlgCurrent->pRCGroup)    && (hWnd == pDlgCurrent->pRCGroup->HWindow)) ||
      ((pDlgCurrent->pVAGroup)    && (hWnd == pDlgCurrent->pVAGroup->HWindow)) ||
      ((pDlgCurrent->pRCEGroup)   && (hWnd == pDlgCurrent->pRCEGroup->HWindow)) ||
      ((pDlgCurrent->pPosDiagGroup) && (hWnd == pDlgCurrent->pPosDiagGroup->HWindow)))
    return true ;

  if ((pDlgCurrent->pPosDiag[0]) && (hWnd == pDlgCurrent->pPosDiag[0]->HWindow))
  {
        //pDlgCurrent->pPosDiag[0]->SetCheck(BF_UNCHECKED) ;
        pDlgCurrent->pPosDiag[0]->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, "0POSD1/JPODA1", "") ;
        pDlgCurrent->pPosDiag[0]->pControle->setControle(dynamic_cast<void*>(pDlgCurrent->pPosDiag[0])) ;
        pDlgCurrent->pPosDiag[0]->pControle->setType(pDlgCurrent->pPosDiag[0]->donneType()) ;
        pDlgCurrent->pPosDiag[0]->pControle->setNSDialog(pDlgCurrent) ;
        pDlgCurrent->referenceControle(pDlgCurrent->pPosDiag[0]->pControle) ;
        return true ;
    }
    if ((pDlgCurrent->pPosDiag[1]) && (hWnd == pDlgCurrent->pPosDiag[1]->HWindow))
    {
        //pDlgCurrent->pPosDiag[1]->SetCheck(BF_UNCHECKED) ;
        pDlgCurrent->pPosDiag[1]->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, "0POSD1/JPODB1", "") ;
        pDlgCurrent->pPosDiag[1]->pControle->setControle(dynamic_cast<void*>(pDlgCurrent->pPosDiag[1])) ;
        pDlgCurrent->pPosDiag[1]->pControle->setType(pDlgCurrent->pPosDiag[1]->donneType()) ;
        pDlgCurrent->pPosDiag[1]->pControle->setNSDialog(pDlgCurrent) ;
        pDlgCurrent->referenceControle(pDlgCurrent->pPosDiag[1]->pControle) ;
        return true ;
    }
    if ((pDlgCurrent->pPosDiag[2]) && (hWnd == pDlgCurrent->pPosDiag[2]->HWindow))
    {
        //pDlgCurrent->pPosDiag[2]->SetCheck(BF_UNCHECKED) ;
        pDlgCurrent->pPosDiag[2]->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, "0POSD1/JPODC1", "") ;
        pDlgCurrent->pPosDiag[2]->pControle->setControle(dynamic_cast<void*>(pDlgCurrent->pPosDiag[2])) ;
        pDlgCurrent->pPosDiag[2]->pControle->setType(pDlgCurrent->pPosDiag[2]->donneType()) ;
        pDlgCurrent->pPosDiag[2]->pControle->setNSDialog(pDlgCurrent) ;
        pDlgCurrent->referenceControle(pDlgCurrent->pPosDiag[2]->pControle) ;
        return true ;
    }
    if ((pDlgCurrent->pPosDiag[3]) && (hWnd == pDlgCurrent->pPosDiag[3]->HWindow))
    {
        //pDlgCurrent->pPosDiag[3]->SetCheck(BF_UNCHECKED) ;
        pDlgCurrent->pPosDiag[3]->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, "0POSD1/JPODD1", "") ;
        pDlgCurrent->pPosDiag[3]->pControle->setControle(dynamic_cast<void*>(pDlgCurrent->pPosDiag[3])) ;
        pDlgCurrent->pPosDiag[3]->pControle->setType(pDlgCurrent->pPosDiag[3]->donneType()) ;
        pDlgCurrent->pPosDiag[3]->pControle->setNSDialog(pDlgCurrent) ;
        pDlgCurrent->referenceControle(pDlgCurrent->pPosDiag[3]->pControle) ;
        return true ;
    }
    if ((pDlgCurrent->pPosDiag[4]) && (hWnd == pDlgCurrent->pPosDiag[4]->HWindow))
    {
        //pDlgCurrent->pPosDiag[4]->SetCheck(BF_UNCHECKED) ;
        pDlgCurrent->pPosDiag[4]->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, "0POSD1/JPODZ1", "") ;
        pDlgCurrent->pPosDiag[4]->pControle->setControle(dynamic_cast<void*>(pDlgCurrent->pPosDiag[4])) ;
        pDlgCurrent->pPosDiag[4]->pControle->setType(pDlgCurrent->pPosDiag[4]->donneType()) ;
        pDlgCurrent->pPosDiag[4]->pControle->setNSDialog(pDlgCurrent) ;
        pDlgCurrent->referenceControle(pDlgCurrent->pPosDiag[4]->pControle) ;
        return true ;
    }

    NS_CLASSLIB::TRect dialogRect = pWnd->GetWindowRect() ;

    RECT wndRect ;
    ::GetWindowRect(hWnd, &wndRect) ;

    NS_CLASSLIB::TRect dlgDeltaRect(0, 0, pDlgCurrent->iLargeurColonne, pDlgCurrent->iHauteurColonne) ;
    pDlgCurrent->MapDialogRect(dlgDeltaRect) ;

    // long lX = wndRect.left + dlgDeltaRect.Width() ;
    // long lY = wndRect.top + dlgDeltaRect.Height() ;

    NS_CLASSLIB::TPoint ptHautGauche(wndRect.left + dlgDeltaRect.Width(), wndRect.top + dlgDeltaRect.Height()) ;
    pDlgCurrent->ScreenToClient(ptHautGauche) ;

    ::MoveWindow(hWnd, ptHautGauche.X(), ptHautGauche.Y(), wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, false) ;

/*
    NS_CLASSLIB::TRect dialogRect = pWnd->GetWindowRect() ;

    RECT wndRect ;
    ::GetWindowRect(hWnd, &wndRect) ;

    NS_CLASSLIB::TRect dlgDeltaRect(0, 0, pDlgCurrent->iLargeurColonne, pDlgCurrent->iHauteurColonne) ;
    pDlgCurrent->MapDialogRect(dlgDeltaRect) ;

    long lX = wndRect.left - dialogRect.X() + dlgDeltaRect.Width() ;
    long lY = wndRect.top - dialogRect.Y() + dlgDeltaRect.Height() ;

    ::MoveWindow(hWnd, lX, lY, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, false) ;
*/

    return true ;
}

bool FAR PASCAL _export NSRCDialog::RCDlgUnconfigCtrl(HWND hWnd, LPARAM lParam)
{
    //
	// Adresse de l'objet boîte de dialogue courante
	//
	TWindow* pWnd = reinterpret_cast<TWindow*>(lParam);
	NSRCDialog* pDlgCurrent = dynamic_cast<NSRCDialog*>(pWnd);

    if (!pDlgCurrent)
        return true ;

    if (((pDlgCurrent->pRCList) && (hWnd == pDlgCurrent->pRCList->HWindow)) ||
        ((pDlgCurrent->pVoirAussi)  && (hWnd == pDlgCurrent->pVoirAussi->HWindow)) ||
        ((pDlgCurrent->pRCE)        && (hWnd == pDlgCurrent->pRCE->HWindow)) ||
        ((pDlgCurrent->pPosDiagGroup) && (hWnd == pDlgCurrent->pPosDiagGroup->HWindow)) ||
        ((pDlgCurrent->pRCGroup)    && (hWnd == pDlgCurrent->pRCGroup->HWindow)) ||
        ((pDlgCurrent->pVAGroup)    && (hWnd == pDlgCurrent->pVAGroup->HWindow)) ||
        ((pDlgCurrent->pRCEGroup)   && (hWnd == pDlgCurrent->pRCEGroup->HWindow)) ||
        ((pDlgCurrent->pPosDiag[0]) && (hWnd == pDlgCurrent->pPosDiag[0]->HWindow)) ||
        ((pDlgCurrent->pPosDiag[1]) && (hWnd == pDlgCurrent->pPosDiag[1]->HWindow)) ||
        ((pDlgCurrent->pPosDiag[2]) && (hWnd == pDlgCurrent->pPosDiag[2]->HWindow)) ||
        ((pDlgCurrent->pPosDiag[3]) && (hWnd == pDlgCurrent->pPosDiag[3]->HWindow)) ||
        ((pDlgCurrent->pPosDiag[4]) && (hWnd == pDlgCurrent->pPosDiag[4]->HWindow)))
        return true ;

    ::DestroyWindow(hWnd) ;
/*
    NS_CLASSLIB::TRect dialogRect = pWnd->GetWindowRect() ;

    RECT wndRect ;
    ::GetWindowRect(hWnd, &wndRect) ;

    long lX = wndRect.left - dialogRect.X() - pDlgCurrent->iLargeurColonne ;
    long lY = wndRect.top - dialogRect.Y() - pDlgCurrent->iHauteurColonne ;

    ::MoveWindow(hWnd, lX, lY, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, false) ;
*/

    return true ;
}

// ------------------------------------------------------------------------
//                            NSRCElement
// ------------------------------------------------------------------------

NSRCElement::NSRCElement(int iOrd, string sLex, string sLibel)
{
  iOrder      = iOrd ;
  sLexique    = sLex ;
  sLibelle    = sLibel ;
}

NSRCElement::NSRCElement(const NSRCElement& rv)
{
  iOrder      = rv.iOrder ;
  sLexique    = rv.sLexique ;
  sLibelle    = rv.sLibelle ;
}

NSRCElement::~NSRCElement(){}
NSRCElement&
NSRCElement::operator=(const NSRCElement& src)
{
  if (this == &src)
    return *this ;

  iOrder      = src.iOrder ;
  sLexique    = src.sLexique ;
  sLibelle    = src.sLibelle ;

  return *this ;
}

int
NSRCElement::operator==(const NSRCElement& o)
{
  if ((iOrder     == o.iOrder)    &&
      (sLexique   == o.sLexique)  &&
      (sLibelle   == o.sLibelle))
    return 1 ;

  return 0 ;
}

// ------------------------------------------------------------------------
//                            NSRCElement
// ------------------------------------------------------------------------

ArrayRCElems::ArrayRCElems(const ArrayRCElems& rv)
{
try
{
	if (false == rv.empty())
		for (RCElemConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSRCElement(*(*i))) ;
}
catch (...)
{
  erreur("Exception ArrayRCElems.", standardError, 0) ;
}
}

ArrayRCElems::~ArrayRCElems()
{
    vider() ;
}

RCElemIter
ArrayRCElems::findByLib(string sLibel)
{
  if (empty())
    return (RCElemIter) NULL ;

  RCElemIter i = begin() ;
  for ( ; (end() != i) && ((*i)->sLibelle != sLibel); i++) ;

  return i ;
}

RCElemIter
ArrayRCElems::findByCode(string sCode)
{
  if ((empty()) || (sCode == ""))
    return (RCElemIter) NULL ;

  RCElemIter i = begin() ;
  for ( ; (end() != i) && ((*i)->sLexique != sCode) ; i++) ;

  return i ;
}

void
ArrayRCElems::vider()
{
  if (empty())
		return ;

	for (RCElemIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

ArrayRCElems&
ArrayRCElems::operator=(const ArrayRCElems& src)
{
try
{
  if (this == &src)
    return *this ;

  vider() ;

  if (false == src.empty())
		for (RCElemConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(new NSRCElement(*(*i))) ;

  return *this ;
}
catch (...)
{
  erreur("Exception ArrayRCElems=.", standardError, 0) ;
  return *this ;
}
}

// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSPosDiagButton, TRadioButton)
END_RESPONSE_TABLE ;

//
// Constructeurs
//
NSPosDiagButton::NSPosDiagButton(TWindow* parent, int resId, TGroupBox* group)
                :TRadioButton(parent, resId, group)
{
}

NSPosDiagButton::NSPosDiagButton(TWindow* parent, int resId, const char far* title,
                             int x, int y, int w, int h, TGroupBox *group,
                             TModule* module)
              :TRadioButton(parent, resId, title, x, y, w, h, group, module)
{
  //
  // Attention, le constructeur de TRadioButton employé ici attribue le style
  // BS_AUTORADIOBUTTON, que nous remplaçons par BS_RADIOBUTTON
  //
  Attr.Style    = (Attr.Style ^ BS_AUTORADIOBUTTON) | BS_RADIOBUTTON;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY;
}

NSPosDiagButton::~NSPosDiagButton()
{
}

char far*
NSPosDiagButton::GetClassName()
{
  return RADIO_CLASS;
}

void
NSPosDiagButton::GetWindowClass(WNDCLASS far& wc)
{
  TWindow::GetWindowClass(wc);
  // static TBrush yellow(TColor::LtYellow);
  // wc.hbrBackground = yellow;
  // wc.lpszClassName = RADIO_CLASS;
  // wc.style |= BS_RADIOBUTTON;
}

void
NSPosDiagButton::SetupWindow()
{
  TRadioButton::SetupWindow();
}

void
NSPosDiagButton::setOn()
{
  Attr.Style = Attr.Style ^ WS_DISABLED ;
}

void
NSPosDiagButton::setOff()
{
  Attr.Style = Attr.Style | WS_DISABLED ;
}

