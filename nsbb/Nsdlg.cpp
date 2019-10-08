//**************************************************************************//  NOM du FICHIER : NSDLG.CPP
//
//  Implémentation des méthodes de :
//     NSDialog
//     NSDialogModal
//     NSDialogModeless
//
//  Auteur : KRZISCH Ph.
// Date de création : juin 93             mise à jour : mai 94
//**************************************************************************
#include <iostream.h>
#include <typeinfo.h>
#include <string.h>

#include <owl/owlpch.h>

//#include <regexp.h>
#include <cstring.h>
#include <bwcc.h>

#include <owl/module.h>
#include <owl/edit.h>
#include <owl/static.h>
#include <owl/radiobut.h>
#include <owl/checkbox.h>
#include <owl/groupbox.h>
#include <owl/listbox.h>
#include <owl/combobox.h>
#include <owl/scrollba.h>
#include <owl/validate.h>

#include "partage\nsglobal.h"

#include "nssavoir\nspathor.h"

//#include "nsutil.h"
#include "partage\nsapp.h"
#include "nsutil\nscache.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsutil\nsutil.h"
#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nshistdo.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsedilex.h"
#include "nsbb\nsedit.h"
#include "nsbb\nsdico.h"
#include "nsbb\nsbouton.h"
#include "nsbb\nschkbox.h"
#include "nsbb\nsradiob.h"
#include "nsbb\nsgroupe.h"
#include "nsbb\nsonglet.h"
#include "nsbb\nscombo.h"
#include "nsbb\nslistwind.h"
#include "nsbb\nstooltip.h"

#include "nsbb\nsbb.h"
#include "nsbb\nsbb_msg.h"
#include "nsbb\nsbb.rh"
#include "nsbb\nsarc.h"
#include "partage\ns_vector.h"
#include "partage\ns_timer.h"
#include "nsbb\ns_multi.h"
#include "dcodeur\nsgenlan.h"
#include "dcodeur\decoder.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nscqvue.h"
#include "nssavoir\nsconver.h"
#include "nsbb\nsPaneSplitter.h"
#include "nsbb\nsdefArch.h"
#include "nsepisod\nsToDo.h"

#include "ns_ob1\Interface.h"
#include "ns_ob1\InterfaceForKs.h"
#include "ns_ob1\BB1Task.h"

#include "nsbb\nsbb.rh"
#include "ns_ob1\OB1.rh"
#include "nautilus\nautilus.rh"

const char  codeMARK    = 'O';

const string     NULLSTRING        = string("") ;
int				    	 NULLINT           = 0 ;
Voidptr			     NULLPTR           = NULL ;
PatPathoIter     *NULLPATPATHOITER = (PatPathoIter *) NULL;
NSPatPathoArray	 *NULLPATHO        = (NSPatPathoArray *) NULL ;
PathsList		     *NULLPATHS        = (PathsList *) NULL ;
BB1Object				 *NULLOBJECT       = (BB1Object *) NULL ;
NautilusQuestion *NULLLNAUTQUEST   = (NautilusQuestion *) NULL ;
NautilusEvent		 *NULLNAUTILUSEVENT	= (NautilusEvent *) NULL ;
NSSearchStruct	 *NULLSEARCHSTRUCT  = (NSSearchStruct *) NULL ;

//#include "nsbb\nsconclu.h"

//#include "kebis.h"

//void strAnalyse(const char* input, const char* separateurs,
//					 CacheArray& elts, int nb=1);
//void    strAnalyse(Pchar input, Pchar separateurs, CacheArray& elts, int nb = 1);
//void    strAnalyse(Pchar input, Pchar separateurs, Pchar elts[], int nb = 1);//bool    ptrInit(char** strPtr, int length, char car = ' ');
//bool    ptrInit(char** strPtr, const char* strInit);

// Définition des expressions régulières
//const TRegexp MWord("[A-Z][a-z]+");   // Mot commençant par une majuscule
//const TRegexp mWord("[a-z]+");        // Mot en minuscule
//const TRegexp Naturel("[0-9]+");      // Nombre naturel
//const TRegexp Entier("(+|-)[0-9]+");  // Nombre entier

//---------------------------------------------------------------------------
//  Configuration des boîtes de dialogue
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
const char szSepare[]= "/;";  // Chaîne des séparateurs utilisés pour
										// la configuration des boîtes de Dialogue
typedef void* Pvoid;
HINSTANCE hInstance;

//***************************************************************************
// Implémentation des méthodes NSDialog
//***************************************************************************
//$$ ID du contrôle NSBitmap
#define DIALOG_1  1

// Table de réponses aux messages
DEFINE_RESPONSE_TABLE1(NSDialog, TDialog)
	EV_COMMAND(IDCANCEL,       CmCancel),
	EV_COMMAND(IDOK,           CmOk),
	EV_COMMAND(IDRETOUR,       CmRetour),
	EV_COMMAND(IDSUITE,        CmSuite),
	EV_COMMAND(IDHELPWWW,      CmHelp),
	EV_COMMAND(IDHELP,         CmHelp),
	EV_COMMAND(IDTREEPASTE,    CmTreePaste),
	EV_COMMAND(IDCONCLUSION,   CmConclusion),
	EV_COMMAND(IDBBKCALCUL,    CmBbkCalcul),  EV_COMMAND(IDM_BBK_EVENT,  CmBbkEvent),  EV_COMMAND(IDM_BBK_UPDATE, CmBbkUpdate),  EV_COMMAND(IDNOFREETXT,    CmWarnForNoFreeText),	// EV_WM_MOVE,	// EV_WM_WINDOWPOSCHANGED,
	EV_WM_ACTIVATE,
	EV_WM_NCACTIVATE,
	EV_WM_SETFOCUS,
	EV_WM_WINDOWPOSCHANGING,
	EV_WM_LBUTTONDOWN,
  EV_WM_TIMER,
  EV_WM_CTLCOLOR,
END_RESPONSE_TABLE ;

//---------------------------------------------------------------------------
//  Function: NSDialog::NSDialog(TWindow* AParent, TResId ResourceID,
//                               TResId ConfigResID, BBItem* pItem)
//
//  Arguments:
//            AParent       -> Pointeur sur l'objet Parent
//            ResourceID    -> ID de la ressource
//            Config5ResID  -> ID de la ressource de configuration
//			  pItem			-> Pointeur sur le BBItem créateur
//  Description:
//            Constructeur de NSDialog qui appelle le constructeur de
//            TDialog.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
NSDialog::NSDialog( NSContexte    *pCtx,
                    TWindow       *AParent,
                    TResId        ResourceID,
                    TResId        ConfResID,
                    BBItem        *pItem,
                    TModule       *pResModule,                    NsMultiDialog *pControlMulti)         :TDialog(AParent, ResourceID, pResModule), NSRoot(pCtx){
	_ResID            = ResourceID ;
	_ConfigResID      = ConfResID ;
	_bPOMRDialog      = false ;	init(pItem, pControlMulti) ;
	_bMereMUEView     = false ;
	_bActivateMUEView = true ;
  _bOkActivated     = false ;
  _bBlackboardStillAtWork = false ;
  _iOngletLevel     = 0 ;
  _iLastAction      = actionOther ;

  _pNSToolTip       = (NSToolTip*) 0 ;
  _pToolTip         = new TTooltip(this) ;
  _pToolTip->Attr.Style |= TTS_BALLOON ;
}

NSDialog::NSDialog( NSContexte    *pCtx,
                    TWindow       *AParent,
                    BBItem        *pItem,
                    TModule       * /*pResModule */,
                    NsMultiDialog *pControlMulti)
         :TDialog(AParent, "DLG_BASE", pNSDLLModule), NSRoot(pCtx){
	_ResID       = 0 ;
	_ConfigResID = 0 ;
	_bPOMRDialog = false ;
	init(pItem, pControlMulti) ;

	_bMereMUEView           = false ;
	_bActivateMUEView       = true ;
  _bOkActivated           = false ;
  _bBlackboardStillAtWork = false ;
  _iOngletLevel           = 0 ;
  _iLastAction            = actionOther ;

  _pNSToolTip = (NSToolTip*) 0 ;
  _pToolTip   = new TTooltip(this) ;
  _pToolTip->Attr.Style |= TTS_BALLOON ;
}

void
NSDialog::init(BBItem* pItem, NsMultiDialog* pControlMulti)
{
try
{
	_pBBItem = pItem ;

	// Si le dialogue est dynamique, on ne se préoccupe pas de DLL
	if (0 != _ResID)
		pNSResModule = pItem->_pBigBoss->TrouverModule(pItem->getItemDialogFile()) ;
	else
		pNSResModule = (TModule*) 0 ;

	_sControleur   = string("") ;
	_sConfigEdit   = string("") ;
	_sConfigOnglet = string("") ;
	_sConfigHelp   = string("") ;
  _sConfigTips   = string("") ;
	_sResInfo      = string("") ;	canCloseOk = false ;	// pDefRegions	  = 0;
	_pControleurMulti = pControlMulti ;
	for (int i = 0; i < 20; i++)
		_pConfigCache[i] = 0 ;
	_pGroupCurr		   = (OWL::TGroupBox*) 0 ;
	_iNbrCtrl        = 0 ;
	//$$ Temporaire
	hInstance = GetModule()->GetInstance() ;

	_sHelpBodyUrl   = string("") ;
	_sHelpIndexUrl  = string("") ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;
	if (pSuper)
	{
		pSuper->setAideIndex("") ;
		pSuper->setAideCorps("zz_generique.htm") ;
	}

	if (0 != _ResID)
		initialiseRessources(_ResID, _ConfigResID) ;
	// else if ((pBBItem == pBBItem->pBigBoss->pBBItem) && (pBBItem->ouvreArchetype()))
	else if (_pBBItem->ouvreArchetype())
		initialiseRessourcesArchetype() ;

	if (pControlMulti)
		pControlMulti->referenceDialogue(this) ;

	_pView       = (TWindowView*) 0 ;
	_bCanMove    = true ;
}
catch (...)
{
	erreur("Exception NSDialog::init.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSDialog::~NSDialog()
{
	Desactive() ;

  if (_pToolTip)
    delete _pToolTip ;
  if (_pNSToolTip)
    delete _pNSToolTip ;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void NSDialog::Desactive()
{
	if (false == _bActif)
		return ;

	for (int i = 0 ; i < 20 ; i++)	{
		if (_pConfigCache[i])
		{
			delete _pConfigCache[i] ;
			_pConfigCache[i] = (NSDialogEdit*) 0 ;
		}
	}

    /*
	if (pDefRegions)	{
		delete pDefRegions;
		pDefRegions = 0;
	}
    */    /*	if (isOnglet)	{
	}
    */
	if (false == _aGroups.empty())
  {
  	vector<OWL::TGroupBox*>::iterator BBGrpIter = _aGroups.begin() ;
    for ( ; _aGroups.end() != BBGrpIter ; )
    {
    	delete *BBGrpIter ;
      _aGroups.erase(BBGrpIter) ;
    }
  }

	_bActif = false ;

  _aNSCtrls.vider() ;
}

//---------------------------------------------------------------------------//  Function: NSDialog::SetupWindow()
//
//  Arguments:
//	    AUCUN
//  Description:
//	    Sert à configurer tous les contrôles si nécessaire.
//          Initialise également la PvoidArray qui va servir
//          à mémoriser l'adresse de tous les emplacements de mémoire
//          servant au transfert de données des différents contrôles de
//          la boîte de dialogue. Ce vecteur est la propriété de BigBrother
//          puisque celui-ci est le seul à connaître le moment opportun de
//          destruction de l'objet de transfert.
//          (Cela pose un problème d'encapsulation C++)
//  Returns:
//	    RIEN
//---------------------------------------------------------------------------
void
NSDialog::SetupWindow()
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string ps = string("Entrée dans SetupWindow de NSDialog") ;
  pSuper->trace(&ps, 1) ;

  // En cas de perte du focus, on cache l'éventuelle fenêtre de pilotage
  // de description multiple pour éviter sa sélection à partir d'une fenêtre
  // fille de cette boite de dialogue
  TWindow   *pMere = Parent ;
  NSDialog  *pDialog = TYPESAFE_DOWNCAST(pMere, NSDialog) ;
  if (pDialog)
  {
    if (pDialog->_pControleurMulti)
      pDialog->_pControleurMulti->Show(SW_HIDE) ;
  }

  if (_pControleurMulti)
  {
    _pControleurMulti->setPosition(this) ;
    // donner un titre aux (pBBItem->pBBFilsPere->VectorFils.size())
    // boîtes pBBItem->pBBFilsPere->VectorFils.size() = nombre de
    // boîtes multidialogue
    _pControleurMulti->setTitre(this, _pBBItem->_pBBFilsPere->VectorFils.size()) ;
  }

  // Si le dialogue est statique, on énumère ses contrôles pour créer les
  // bons objets
  if (0 != _ResID)
  {
    WNDENUMPROC lpEnumDlgFunc ;

    // Enumération et configuration des contrôles

    // Initialisation des variables incrémentées dans l'énumération
    _iIndexCtrl = 0 ;
    _iIndexEdit = 0 ;

    pSuper->afficheStatusMessage("Connexion des contrôles") ;

    // Préparation de la fonction d'énumération
    lpEnumDlgFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) NSDialog::DlgConfigCtrl, hInstance) ;

    // On passe comme paramètre de EnumChildWindows LPARAM(this), l'adresse de
    // this puisque dans les fonctions static on ne peut pas récupérer le
    // pointeur this.
    EnumChildWindows(HWindow, lpEnumDlgFunc, LPARAM((TWindow *)this)) ;

    TDialog::SetupWindow() ;

    // Dynamic caption?
    char szCaptionHeading[7] ;
    if (GetWindowText(szCaptionHeading, 7))
    {
      if (0 == strcmp("$AUTO$", szCaptionHeading))
        SetDynamicCaption() ;
    }
  }
  // else if ((pBBItem == pBBItem->pBigBoss->pBBItem) && (pBBItem->ouvreArchetype()))
  else if (_pBBItem->ouvreArchetype())
    CreerControlesArchetype() ;

  // Si le dialogue est dynamique, on crée à la main ses contrôles
  else
    CreerControles() ;


	// Appel obligatoire au SetupWindow() de l'ancêtre


	// Il est inutile de lancer ForEach avant TDialog::SetupWindow, car ChildList
  // n'est pas encore instancié

	// Marche ForEach((TActionFunc)(&(NSDialog::DlgConfigCtrl2)), (void *)((TWindow *)this)) ;

	// Lance les éventuelles fonctions d'initialisation des contrôles
	initialiseControles();

  // Lorsqu'on initialise à partir du blackboard en mod "SetupWindow" par default
	if (_pBBItem->_KsInterface.getInitFromBbk())
    initControlesFromBbk() ;

  // vérification qu'il y a quelquechose dans les variables de la capture
  if (false == pContexte->getEpisodus()->CaptureArray.empty())
  {
    initControlesFromCapture() ;
    // pSuper->pEpisodus->CaptureArray.vider() ;
  }

	// Initialise les contrôles NAUTILUS en fonction du contexte Big Brother
	rafraichitControles() ;

  // It's time for the BBItem to cut bad branches: Ordinary "Fils Guides" that
  // got connected to leaves but don't open a dialog. They tend to cause system
  // hanging
  //
	if (false == _pBBItem->ouvreArchetype())
		_pBBItem->cutBadDialogBranches(this) ;

	//$$ Temporaire
	// ConfigShow() ;
  pSuper->afficheStatusMessage("") ;

  if (_pView)
  {
    if (pContexte->getPredi())
    {
      ps = string("SetupWindow de NSDialog, partie spécifique à DPIO") ;
      pSuper->trace(&ps, 1) ;

      NS_CLASSLIB::TRect dlgRect ;
      GetWindowRect(dlgRect) ;

      int iWidth  = dlgRect.Width() ;
      int iHeigth = dlgRect.Height() ;

      int iX = pContexte->getPredi()->_iCurrentX ;
      int iY = pContexte->getPredi()->_iCurrentY ;

      SetWindowPos(0, iX, iY, iWidth, iHeigth, SWP_NOZORDER);

      pContexte->getPredi()->_iCurrentX += pContexte->getPredi()->_iDeltaPosX ;
      pContexte->getPredi()->_iCurrentY += pContexte->getPredi()->_iDeltaPosY ;
    }
  }

  _pToolTip->Activate() ;

  ps = string("Sortie de SetupWindow de NSDialog") ;
  pSuper->trace(&ps, 1) ;
}

//*** acc key handling *****

// In order to have tooltips operate, we have to use Create, as a modeless
// dialog, then to use BeginModal in order to have application behave as
// if dialog was modal (freeze other windows while this one is open)
//
int NSDialog::Execute()
{
try
{
  _IsModalDialog = true ;

  if (TDialog::Create())
  {
    Show(SW_SHOW) ;

    TWindow* notBlockedWindow = (TWindow*) 0 ;
    NSDico* pDico = pContexte->getDico() ;
    if (pDico && pDico->getDicoDialog())
      notBlockedWindow = pDico->getDicoDialog() ;

    int iResult = pContexte->getSuperviseur()->getApplication()->BeginModal(this, notBlockedWindow, MB_TASKMODAL) ;

    Parent->SetFocus() ;

    return iResult ;
  }
  else
    return -1 ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSDialog::Execute : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
  return -1 ;
}
catch(...)
{
	erreur("Exception NSDialog::Execute.", standardError, 0) ;
  return -1 ;
}
}

void NSDialog::Destroy (int ret)
{
  if (_IsModalDialog)
  {
    pContexte->getSuperviseur()->getApplication()->EndModal(IDCANCEL) ;
    _IsModalDialog = false ;
  }
  TDialog::Destroy(ret) ;
}

bool
NSDialog::PreProcessMsg(MSG &msg)
{
  if (_pToolTip && _pToolTip->IsWindow())
    _pToolTip->RelayEvent(msg) ;

  return TDialog::PreProcessMsg(msg) ;
}

//---------------------------------------------------------------------------
//  Function: 		NSDialog::initialiseRessources()
//
//  Arguments:		AUCUN
//
//  Description:	Initialise les descripteurs de ressources situés dans
//						le fichier '_INIT'
//
//  Returns:		true si OK, false en cas d'erreur
//---------------------------------------------------------------------------
bool
NSDialog::initialiseRessources(TResId ResourceID, TResId ConfigResID)
{
  //
	// Une NSDialog possède TOUJOURS une ressource de configuration
	//
	if (ConfigResID == 0)
	{
		char nomConfigRes[BB_NOM_DIALOGUE_LEN + 5];
		strcpy(nomConfigRes, ResourceID);
		strcat(nomConfigRes, "_INIT");
		ConfigResID = nomConfigRes;
	}
	if (LoadConfigResource(ConfigResID))
	{		//
		// Analyse de la ressource de configuration
		//
		AnalyseRes() ;

		//
		// Création de la matrice de configuration pour le contrôleur
		//
		if (string("") != _sControleur)
		{
			AnalyseCtrl(_sControleur.c_str(), "|:") ;
			_sControleur = string("") ;
		}

		//
		// Création de la matrice de configuration pour les contrôles EDIT
		//
		if (string("") != _sConfigEdit)
		{
			AnalyseEdit(_sConfigEdit.c_str(), szSepare) ;
			_sConfigEdit = string("") ;
		}

    //
		// Création de la matrice de configuration pour les onglets
		//
		if (string("") != _sConfigOnglet)
		{
			AnalyseOnglet(_sConfigOnglet.c_str(), szSepare) ;
			_sConfigOnglet = string("") ;
		}

    //
		// Initialisation de l'aide en ligne
		//
		if (string("") != _sConfigHelp)
		{
      //MessageBox("Entrée dans AnalyseHelp", "", MB_ICONEXCLAMATION);
			AnalyseHelp(_sConfigHelp.c_str(), szSepare) ;
			_sConfigHelp = string("") ;
		}
    else
      AnalyseHelp("", szSepare) ;

    //
		// Initialisation des tooltips
		//
		if (string("") != _sConfigTips)
		{
			AnalyseTips(_sConfigTips.c_str(), "|:") ;
			_sConfigTips = string("") ;
		}

		// Création de la matrice de configuration des régions d'un bitmap
		// pBitmapRegion = new DefRegion[ ]

		// Création de la matrice de transfert
		// pTransferBuf = new NSTransferArray;
		// SetTransferBuffer(dynamic_cast<void*>(pTransferBuf));

		_sResInfo = string("") ;
	}
	else
  {
		// Message d'erreur
    return false ;
	}
  _bActif = true ;
  return true ;
}

//---------------------------------------------------------------------------
//  Function: 		NSDialog::initialiseRessourcesArchetype()
//
//  Arguments:		AUCUN
//
//  Description:	Initialise les descripteurs de ressources situés dans
//						le fichier '_INIT'
//
//  Returns:		true si OK, false en cas d'erreur
//---------------------------------------------------------------------------
bool
NSDialog::initialiseRessourcesArchetype()
{
	if (((BBItem*) NULL == _pBBItem) || ((nsarcParseur*) NULL == _pBBItem->getParseur()))
		return false ;

	//
	// Une NSDialog possède TOUJOURS une ressource de configuration
	//
  string sLang = pContexte->getUserLanguage() ;

	Cdialogbox* pDialogBox = _pBBItem->getArchetypeDialog(sLang) ;
	if ((Cdialogbox*) NULL == pDialogBox)
		return false ;

	// Crcdata* pRCData = pDialogBox->getRCData();

	_sResInfo = string(pDialogBox->getValueRCData()) ;

	//	// Analyse de la ressource de configuration
	//
	AnalyseRes() ;

	//	// Création de la matrice de configuration pour le contrôleur
	//
	if (string("") != _sControleur)
	{
		AnalyseCtrl(_sControleur.c_str(), "|:") ;
		_sControleur = string("") ;
	}

  //  // Création de la matrice de configuration pour les contrôles EDIT
  //
  if (string("") != _sConfigEdit)
  {
    AnalyseEdit(_sConfigEdit.c_str(), szSepare) ;
    _sConfigEdit = string("") ;
  }

  //  // Création de la matrice de configuration pour les onglets
  //
  if (string("") != _sConfigOnglet)
  {
    AnalyseOnglet(_sConfigOnglet.c_str(), szSepare) ;
    _sConfigOnglet = string("") ;
  }

  //  // Initialisation de l'aide en ligne
  //
  if (string("") != _sConfigHelp)
  {
    //MessageBox("Entrée dans AnalyseHelp", "", MB_ICONEXCLAMATION);
    AnalyseHelp(_sConfigHelp.c_str(), szSepare) ;
    _sConfigHelp = string("") ;
  }
  else
    AnalyseHelp("", szSepare) ;

  //
  // Initialisation des tooltips
  //
  if (string("") != _sConfigTips)
  {
    AnalyseTips(_sConfigTips.c_str(), szSepare) ;
    _sConfigTips = string("") ;
  }

  // Création de la matrice de configuration des régions d'un bitmap  // pBitmapRegion = new DefRegion[ ]

  // Création de la matrice de transfert  // pTransferBuf = new NSTransferArray;
  // SetTransferBuffer(dynamic_cast<void*>(pTransferBuf));

  _sResInfo = string("") ;
  _bActif = true ;  return true ;}
void
NSDialog::AnalyseCtrl(const char *input, const char *separateurs)
{
try
{
	if ((NULL == input) || (NULL == separateurs))
		return ;

  size_t posit, pos ;
  char  separe[2] ;
  string vide   = string(" ") ;
  string sCopie = string(input) ;
  string sIdentite ;

  separe[0] = separateurs[0] ;
  separe[1] = '\0' ;
  size_t posApres = sCopie.find(separe[0]) ;
  size_t posAvant = 0 ;
	while ((posApres != string::npos) && (posApres < strlen(sCopie.c_str())))
	{
		NSDialogCtrl* pConfigCt = new NSDialogCtrl() ;
		pConfigCt->setIdentite(string(sCopie, posAvant, posApres - posAvant)) ;

    sIdentite = pConfigCt->getIdentite() ;
    posit     = sIdentite.find(separateurs[1]) ;

    if ((string::npos != posit) && (posit < strlen(sIdentite.c_str())))
    {
      pConfigCt->setIdentite(string(sIdentite, 0, posit)) ;
      pConfigCt->setDlgFonction(string(sIdentite, posit + 1, strlen(sIdentite.c_str()))) ;
    }

    pos = (pConfigCt->getIdentite()).find(vide) ;
    if (string::npos != pos)
      pConfigCt->setIdentite(string("")) ;

    _aConfigCtrls.push_back(pConfigCt) ;

    posAvant = posApres + 1 ;
		posApres = sCopie.find(separe[0], posApres + 1) ;
  }
}
catch (...)
{
  erreur("Exception NSDialog::AnalyseCtrl.", standardError, 0) ;
}
}

void
NSDialog::AnalyseEdit(const char *input, const char *separateurs)
{
try
{
	if (!input || !separateurs)
		return ;

  int 	i = 0 ;
	char  *szCur ;
	char  *copie ;

  copie = new char[strlen(input) + 1] ;
	strcpy(copie, input) ;

	szCur = strtok(copie, separateurs) ;

  while (szCur != NULL)
	{
		_pConfigCache[i] = new NSDialogEdit ;
		strcpy(_pConfigCache[i]->szType, szCur) ;

		szCur = strtok(NULL, separateurs) ;
		if (szCur != NULL)
		{
			_pConfigCache[i]->nMaxInput = atoi(szCur) ;
			szCur = strtok(NULL, separateurs) ;
		}
		i++ ;
	}
  delete[] copie ;
}catch (...)
{
  erreur("Exception NSDialog::AnalyseEdit.", standardError, 0) ;
}
}

void
NSDialog::AnalyseOnglet(const string &sInput, const string &sSeparateurs)
{
  if ((string("") == sInput) || (string("") == sSeparateurs))
    return ;

  if (strlen(sSeparateurs.c_str()) < 2)
    return ;

  char mainSepar      = sSeparateurs[0] ;
  char secondarySepar = sSeparateurs[1] ;
try
{
  int iIndex = 1 ;

  size_t iPosMain  = sInput.find(mainSepar) ;
  size_t iPosStart = 0 ;

  NSDialogOnglet* pConfigCacheTab = (NSDialogOnglet*) 0 ;

  while (NPOS != iPosMain)
  {
    string sData = string(sInput, iPosStart, iPosMain - iPosStart) ;

    iPosStart = iPosMain + 1 ;
    iPosMain  = sInput.find(mainSepar, iPosStart) ;

    if (1 == iIndex)
    {
      pConfigCacheTab = new NSDialogOnglet ;
      pConfigCacheTab->sOngletActif = sData ;
      iIndex++ ;
    }
    else if (2 == iIndex)
    {
      size_t iPosSeco      = sData.find(secondarySepar) ;
      size_t iPosSecoStart = 0 ;
      while (NPOS != iPosSeco)
      {
        pConfigCacheTab->addTabText(string(sData, iPosSecoStart, iPosSeco - iPosSecoStart)) ;

        iPosSecoStart = iPosSeco + 1 ;
        iPosSeco      = sData.find(secondarySepar, iPosSecoStart) ;
      }

      size_t iDataLen = strlen(sData.c_str()) ;
      if (iPosSecoStart < iDataLen)
        pConfigCacheTab->addTabText(string(sData, iPosSecoStart, iDataLen - iPosSecoStart)) ;

      _aConfigTabs.push_back(new NSDialogOnglet(*pConfigCacheTab)) ;

      iIndex = 1 ;
      delete pConfigCacheTab ;
      pConfigCacheTab = (NSDialogOnglet*) 0 ;
    }
  }

/*
  char  *szCur ;
	char  *copie ;

  copie = new char[strlen(input) + 1] ;
	strcpy(copie, input) ;

  // La chaîne de configuration est de la forme :
  // "Mitrale/Mitrale;Aortique;Aorte;Pulmonaire;Tricuspide;Cavités/"
  // cad
  // "OngletActif/Onglet1;Onglet2;Onglet3/"

  NSDialogOnglet* pConfigCacheTab = new NSDialogOnglet ;

	// strtok place un \0 à la fin de l'élément si il est trouvé
	szCur = strtok(copie, separateurs) ;
  if (szCur != NULL)
  {
    pConfigCacheTab->sOngletActif = string(szCur) ;
    szCur = strtok(NULL, separateurs) ;
  }


	while (NULL != szCur)
	{
    if ('\0' != *szCur)
      pConfigCacheTab->addTabText(string(szCur)) ;

    szCur = strtok(NULL, separateurs) ;
  }

  _aConfigTabs.push_back(pConfigCacheTab) ;

	delete[] copie ;
*/
}
catch (...)
{
  erreur("Exception NSDialog::AnalyseOnglet.", standardError, 0) ;
}
}

void
NSDialog::AnalyseHelp(const char *input, const char *separateurs)
{
	//
	//
  Creferences* pRef = _pBBItem->getArchetypeReference() ;
  if (pRef)  {    Chead* pHead = pRef->getHead() ;    if (pHead)    {      string sUrl = pHead->getHelpUrl();      if (sUrl != "")      {        _sHelpBodyUrl = sUrl ;
        return ;
      }
    }
  }

	if ((!input) || (input[0] == '\0'))
		return ;

  // La chaîne de configuration est de la forme :
  // "Index/Corps/"

  char   separe[2] ;
  string vide = " " ;
  string sCopie = string(input) ;
  string sIdentite ;

  separe[0] = separateurs[0] ;
  separe[1] = '\0' ;

  size_t posApres = sCopie.find(separe[0]) ;  size_t posAvant = 0 ;

  if (posApres == NPOS)    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  if (posApres == posAvant)
    _sHelpIndexUrl = "" ;
  else
    _sHelpIndexUrl = string(sCopie, posAvant, posApres - posAvant) ;

  pSuper->setAideIndex(_sHelpIndexUrl) ;

  posAvant = posApres + 1 ;
  posApres = sCopie.find(separe[0], posApres + 1) ;

  if (posApres == posAvant)
    _sHelpBodyUrl = "" ;
  else if (posApres == NPOS)
    _sHelpBodyUrl = string(sCopie, posAvant, strlen(sCopie.c_str()) - posAvant) ;
  else
    _sHelpBodyUrl = string(sCopie, posAvant, posApres - posAvant) ;

  pSuper->setAideCorps(_sHelpBodyUrl) ;
}

void
NSDialog::AnalyseTips(const char *input, const char *separateurs)
{
	if ((NULL == input) || (NULL == separateurs) || (strlen(separateurs) < 2))
		return ;

  string sCopie = string(input) ;

  size_t posApres = sCopie.find(separateurs[0]) ;
  size_t posAvant = 0 ;
	while ((posApres != string::npos) && (posApres < strlen(sCopie.c_str())))
	{
		string sBlock = string(sCopie, posAvant, posApres - posAvant) ;
    size_t posit  = sBlock.find(separateurs[1]) ;

    if ((string::npos != posit) && (posit < strlen(sBlock.c_str())))
    {
      string sIdentite = string(sBlock, 0, posit) ;
      string sHelpText = string(sBlock, posit + 1, strlen(sBlock.c_str())) ;

      NSDialogCtrl* pDlgCtrl = _aConfigCtrls.getDialogCtrl(sIdentite) ;
      if (pDlgCtrl)
        pDlgCtrl->setHelpText(sHelpText) ;
    }

    posAvant = posApres + 1 ;
		posApres = sCopie.find(separateurs[0], posApres + 1) ;
  }
}

//---------------------------------------------------------------------------//  Function:   bool NSDialog::LoadConfigResource()
//
//  Arguments:
//
//  Description:
//    Charge la ressource de configuration associée à la boîte de dialogue.
//    La ressource associée à une boîte de dialogue se présente de la manière
//    suivante dans le fichier RES :
// MYRESSOURCE_1 MYRESSOURCE
// {
//  "EDITTEXT"                               // Configuration des contrôles EDIT
//  "C/34;D/12;C/4;C/7;C/3;C/10"
//  "NSBITMAP"                               // Configuration des régions d'un
//  300                                      // NSBitmap
// "Tête:103,10/58,76/102,109/115,98"
// "Cou:103,10/58,76/84,136/0,107/0,68"
// "Epaule:0,107/58,76/98,155/0,164"
// "Sternum:0,164/98,155/0,205/84,214"
// }
//     Mais dans la mémoire :
//"EDITTEXT/C/34;D/12;C/4;C/7;C/3;C/10/NSBITMAP/300/Tête:103,10/58,76/102,109/
// 115,98/Cou:103,10/58,76/84,136/0,107/0,68/Epaule:0,107/58,76/98,155/0,164/
// Sternum:0,164/98,155/0,205/84,214"
//
//  Returns:
//      true   si aucun incident n'est survenu
//      false  sinon
//  REMARQUE :
//   Ce type de ressource est partageable par toutes les instances de
//   l'application.
//---------------------------------------------------------------------------
string* NSDialog::LoadConfigResource(TResId resId, const char far* resType)
{
try
{
  HRSRC   resHdl ;
	HGLOBAL glbHdl ;
	//
	// Chargement en mémoire de la ressource
	//
	if ((resHdl = pNSResModule->FindResource(resId, resType)) != 0)
  {
    if ((glbHdl = pNSResModule->LoadResource(resHdl)) != 0)
    {
      _sResInfo = string((char*)LockResource(glbHdl)) ;
#if !defined __WIN32__
			//
			// Débloquage de la ressource et libération
			//
			GlobalUnlock(glbHdl) ;
#endif
    }
#if !defined __WIN32__
		FreeResource(glbHdl) ;
#endif
  }
	return &_sResInfo ;
}
catch (...)
{
  erreur("Exception NSDialog::LoadConfigResource.", standardError, 0) ;
  return (string*) 0 ;
}
}


// -----------------------------------------------------------------------------// Function     : bool NSDialog::AnalyseRes()// Arguments    : AUCUN
// Description  : Analyse la string de configuration.
//                Détermine la chaîne de configuration pour les contrôles EDIT
//                et crée des DefRegionArray pour chacun des "NSBitmap" déclarés
//                dans pesInfo.
// Returns      : RIEN
// -----------------------------------------------------------------------------
bool
NSDialog::AnalyseRes()
{
try
{
  string sConfigBitmap, sConfigEdit, sConfigControle;

	size_t nouvPos ;     // Positions
	int	 i;
	//
	// Si la chaine de configuration est vide, on sort immédiatement
	//
	if (string("") == _sResInfo)
		return true ;
  //
  // Si la chaine contient une fin explicite, on tronque pResInfo
  //
  size_t posFin = _sResInfo.find(string("FINRCDAT")) ;
  if (NPOS != posFin)
    _sResInfo = string(_sResInfo, 0, posFin) ;

  //
	// Prise de la position des divers éléments
	//
  size_t len = strlen(_sResInfo.c_str()) ;

  int	 nbPos = 6 ;
  size_t pos[6] ;
	pos[0] = _sResInfo.find(string("NSBITMAP")) ;
	pos[1] = _sResInfo.find(string("EDITTEXT")) ;
	pos[2] = _sResInfo.find(string("NSCONTRO")) ;
	pos[3] = _sResInfo.find(string("NSONGLET")) ;
  pos[4] = _sResInfo.find(string("NSAIDELI")) ;
  pos[5] = _sResInfo.find(string("NSHLPTXT")) ;
	//
	// Configuration des NSBITMAP
	//
	/*if (pos[0] != NPOS)
	{
        size_t ext;

		for (nouvPos = len, i = 0; i < nbPos; i++)
			if ((pos[i] > pos[0]) && (pos[i] < nouvPos))
				nouvPos = pos[i];
		sConfigBitmap = (*pResInfo)(pos[0]+8, nouvPos-pos[0]-8);
		//
		// Analyse de sConfigBitmap
		//
		if (!sConfigBitmap.is_null())
		{
			//
			// ID du bitmap
			//
			nouvPos = sConfigBitmap.find(Naturel, &ext, 0);
			BitmapID = atoi(string(sConfigBitmap(nouvPos, ext)).c_str());
			//
			// $$ Compte le nombre de régions
			//
			nouvPos = 0;
         int nbReg;
			for (nbReg = 0;
					(nouvPos = sConfigBitmap.find(MWord, &ext, nouvPos)) != NPOS;
					nbReg++, nouvPos += ext);
			//
			// Construction de la matrice d'initialisation des régions
			//
			nouvPos = 0;
			pDefRegions = new DefRegionArray;

			for (i = 0; i < nbReg ; i++)
			{
				nouvPos = sConfigBitmap.find(MWord, &ext, nouvPos);
				if ((len = sConfigBitmap.find(MWord, &ext, nouvPos+ext)) != NPOS)
					pDefRegions->push_back(new DefRegion(string(sConfigBitmap(nouvPos, len-nouvPos))));
				else
				{
					len = sConfigBitmap.length();
					pDefRegions->push_back(new DefRegion(string(sConfigBitmap(nouvPos, len))));
				}
				nouvPos = len;
			}
		}
	} */
	//
	// Configuration pour les contrôles Edit
	//
  if (NPOS != pos[1])
	{
    for (nouvPos = len, i = 0; i < nbPos; i++)
      if ((pos[i] > pos[1]) && (pos[i] < nouvPos))
        nouvPos = pos[i] ;
    _sConfigEdit = string(_sResInfo, pos[1]+8, nouvPos-pos[1]-8) ;
  }
	//
	// Configuration pour le contrôleur
	//
	if (NPOS != pos[2])
	{
		for (nouvPos = len, i = 0; i < nbPos; i++)
			if ((pos[i] > pos[2]) && (pos[i] < nouvPos))
				nouvPos = pos[i] ;
		_sControleur = string(_sResInfo, pos[2]+8, nouvPos-pos[2]-8) ;
	}
	//
	// Configuration pour les onglets
	//
	if (NPOS != pos[3])
	{
		for (nouvPos = len, i = 0; i < nbPos; i++)
			if ((pos[i] > pos[3]) && (pos[i] < nouvPos))
				nouvPos = pos[i] ;
    _sConfigOnglet = string(_sResInfo, pos[3]+8, nouvPos-pos[3]-8) ;
	}
  //
	// Configuration pour l'aide en ligne
	//
	if (NPOS != pos[4])
	{
		for (nouvPos = len, i = 0; i < nbPos; i++)
			if ((pos[i] > pos[4]) && (pos[i] < nouvPos))
				nouvPos = pos[i] ;
    _sConfigHelp = string(_sResInfo, pos[4]+8, nouvPos-pos[4]-8) ;
    // MessageBox(pConfigHelp->c_str(), "Chaine d'aide", MB_ICONEXCLAMATION);
	}
  //
	// Configuration pour les tooltips
	//
	if (NPOS != pos[5])
	{
		for (nouvPos = len, i = 0; i < nbPos; i++)
			if ((pos[i] > pos[5]) && (pos[i] < nouvPos))
				nouvPos = pos[i] ;
    _sConfigTips = string(_sResInfo, pos[5]+8, nouvPos-pos[5]-8) ;
	}
	return true ;
}
catch (...)
{
  erreur("Exception NSDialog::AnalyseRes().", standardError, 0) ;
  return true ;
}
}
//---------------------------------------------------------------------------//  Function:  NSDialog::DlgConfigCtrl(HWND hWnd, LPARAM lParam)
//
//  Arguments:
//	     hWnd    -> handle du contrôle enfant
//	     lParam  -> lParam de la structure MESSAGE
//  Description:
//	    - Sert à créer tous les objets OWL correspondants aux objets
//      d'interface (contrôles) définis dans la ressource boîte de dialogue.
//     - Crée les buffers de transfert pour chaque objet OWL dans une
//      NSTranferArray.
//     - Utilise les info. stockées dans pConfigCache sous la forme
//      une lettre pour le type de saisie et un nombre pour le nombre
//      de caractères à saisir pour créer des validateurs et des limiteurs
//      de saisie pour les contrôles EDIT.
//      Incrémentation de nIndexCurr.
//      (ex :   )
//  REMARQUE :
//     Comme dans les fonction static il n'est pas possible d'accéder à this,
//     on récupére celui-ci dans lParam
//  Returns:
//	     true  pour que l'énumération puisse continuer.
//---------------------------------------------------------------------------
bool FAR PASCAL _export NSDialog::DlgConfigCtrl(HWND hWnd, LPARAM lParam)
{
try
{
	int  nIndex ;                    // Position dans *pConfigCache
	char szType[25] ;                // Type de contrôle EDIT
	char szEditDateType[2] ;         //type de date du controle EDIT
	char szClassName[30],           // Nom de la classe du contrôle
		  szBuffer[80] ;            // Utilisée pour les fonctions WINDOWS
	memset(szType,	    0, 25) ;
	memset(szClassName, 0, 30) ;
	memset(szBuffer,    0, 80) ;

	string  typeCtrl  = string("") ;  // Type de contrôle
	HWND    hDlg      = NULL ;        // Handle de la boîte de dialogue Parent.
	UINT    nMaxInput = 255 ;         // Nombre maximal de caractères à saisir
	//
	// Adresse de l'objet boîte de dialogue courante
	//
	TWindow* pWnd = reinterpret_cast<TWindow*>(lParam) ;
	NSDialog* pDlgCurrent = dynamic_cast<NSDialog*>(pWnd) ;

	if (((NSDialog*)       NULL == pDlgCurrent) ||
      ((BBItem*)         NULL == pDlgCurrent->_pBBItem) ||
      ((NSSmallBrother*) NULL == pDlgCurrent->_pBBItem->_pBigBoss))
		return true ;

	// NSContexte* pContexte = pDlgCurrent->pBBItem->pBigBoss->pContexte ;
  NSContexte* pContexte = pDlgCurrent->pContexte ;
  if ((NSContexte*) NULL == pContexte)
	  return true ;

	string sLang = pContexte->getUserLanguage() ;

	//
	// Obtention de l'ID du contrôle et de son type WINDOWS
	//
	int ctrlID  = ::GetDlgCtrlID(hWnd);
	::GetClassName(hWnd, (LPSTR) szClassName, 30);
	//
	// Obtention du handle de la boîte de Dialogue
	//
	hDlg = ::GetParent(hWnd);

	//
  // Initialisation de Groupe et Identite
  //
  nIndex = pDlgCurrent->_iIndexCtrl ;

  if (pDlgCurrent->_aConfigCtrls.size() <= (unsigned int) nIndex)
  	return true ;

	NSDialogCtrl* pCfgCtrl = (pDlgCurrent->_aConfigCtrls)[nIndex] ;
  if ((NSDialogCtrl*) NULL == pCfgCtrl)
  	return true ;

	string sIdentite 	  = pCfgCtrl->getIdentite() ;
	string sDlgFonction = pCfgCtrl->getDlgFonction() ;
  string sHelpText    = pCfgCtrl->getHelpText() ;

	// -------------------------------------------------------------------
	//                              TreeView
	// -------------------------------------------------------------------
	if (!strcmp(szClassName,"SysTreeView32"))
	{
    NSTreeWindow* pNSTreeWindow = new NSTreeWindow(pDlgCurrent, pContexte, ctrlID) ;

    NSControle* pControl = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
    pControl->setControle(dynamic_cast<void*>(pNSTreeWindow)) ;
    pControl->setType(pNSTreeWindow->donneType()) ;
    pControl->setNSDialog(pDlgCurrent) ;
    pControl->setHelpText(sHelpText) ;
    
    pNSTreeWindow->setControl(pControl) ;
    pDlgCurrent->referenceControle(pControl) ;

    if ((pControl->getTransfert()) &&
                (pControl->getTransfert()->getFilsItem()))
    {
      BBFilsItem* pSonItem    = pControl->getTransfert()->getFilsItem() ;
      BBItem*     pFatherItem = pSonItem->getItemFather() ;
      pNSTreeWindow->pBBitemNSTreeWindow = pFatherItem ;

      // BBFilsItem*	pCtrlBBFilsItem = pNSTreeWindow->pControle->getTransfert()->pBBFilsItem ;
      // if (!(pCtrlBBFilsItem->VectorFils.empty()))
      //    pNSTreeWindow->pBBitemNSTreeWindow = *(pCtrlBBFilsItem->VectorFils.begin()) ;

      //
      // Créer le deuxième BBItem (GCONS le fils en cas de consultation par exemple)
      //
      /* int creer = */ pFatherItem->developperConsultation(pSonItem) ;
    }

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                                 Edit
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "Edit"))
  {
    string sIden = "" ;
    //
    // Contrôle relié à un BBFisItem / This control is linked to a BBFilsItem
    //
    if (sIdentite != "")
    {
      //NSEdit qui bosse avec le lexique
      if ((sIdentite.find(string("£C;")) != NPOS) || (sIdentite.find(string("/£C;")) != NPOS))
      {
        NSEditLexique* pNSEdit = new NSEditLexique(pDlgCurrent,
                                                   pContexte,
                                                   ctrlID,
                                                   pContexte->getDico(),
                                                   nMaxInput+1) ;
        pNSEdit->setFreeTextEnabled(true) ;
        pNSEdit->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
        pNSEdit->pControle->setControle(dynamic_cast<void*>(pNSEdit)) ;
        pNSEdit->pControle->setType(pNSEdit->donneType()) ;
        pNSEdit->pControle->setNSDialog(pDlgCurrent) ;
        pNSEdit->pControle->setHelpText(sHelpText) ;
        pDlgCurrent->referenceControle(pNSEdit->pControle) ;
      }
      //
      // Champ edit qui ouvre le lexique et refuse les textes libres
      //
      else if (sIdentite.find(string("£CX")) != NPOS)      {
        NSEditLexique* pNSEdit = new NSEditLexique(pDlgCurrent,
                                                   pContexte,
                                                   ctrlID,
                                                   pContexte->getDico(),
                                                   nMaxInput+1) ;
        pNSEdit->setFreeTextEnabled(false) ;
        pNSEdit->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
        pNSEdit->pControle->setControle(dynamic_cast<void*>(pNSEdit)) ;
        pNSEdit->pControle->setType(pNSEdit->donneType()) ;
        pNSEdit->pControle->setNSDialog(pDlgCurrent) ;
        pNSEdit->pControle->setHelpText(sHelpText) ;
        pDlgCurrent->referenceControle(pNSEdit->pControle) ;
      }
      //
      // Champ edit qui n'ouvre pas le lexique et aliment Complement
      //
      else if (sIdentite.find(string("£CC")) != NPOS)      {        NSEditNoLex* pEdit ;        strcpy(szType, "C") ;        string sLen = string(sIdentite, 3, 3) ;        int iLen = atoi(sLen.c_str()) ;        pEdit = new NSEditNoLex(pContexte, pDlgCurrent, ctrlID, szType, sLang, iLen) ;        pEdit->setControl(new NSControle(pContexte, pDlgCurrent->_pBBItem/*, cGroupe*/,																		sIdentite, sDlgFonction)) ;

        pEdit->getControl()->setControle(dynamic_cast<void*>(pEdit)) ;        pEdit->getControl()->setType(pEdit->donneType()) ;
        pEdit->getControl()->setNSDialog(pDlgCurrent) ;
        pEdit->getControl()->setHelpText(sHelpText) ;
        pDlgCurrent->referenceControle(pEdit->getControl()) ;

        //        // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
        //

        pDlgCurrent->_iIndexEdit++ ;      }      //      // Champ edit qui n'ouvre pas le lexique et alimente Texte libre      //      else if (sIdentite.find(string("£CL")) != NPOS)      {        NSEditNoLex* pEdit ;        strcpy(szType, "L") ;        string sLen = string(sIdentite, 3, 3) ;        int iLen = atoi(sLen.c_str()) ;        pEdit = new NSEditNoLex(pContexte, pDlgCurrent, ctrlID, szType, sLang, iLen) ;        pEdit->setControl(new NSControle(pContexte, pDlgCurrent->_pBBItem/*, cGroupe*/,																		sIdentite, sDlgFonction)) ;

        pEdit->getControl()->setControle(dynamic_cast<void*>(pEdit)) ;        pEdit->getControl()->setType(pEdit->donneType()) ;
        pEdit->getControl()->setNSDialog(pDlgCurrent) ;
        pEdit->getControl()->setHelpText(sHelpText) ;
        pDlgCurrent->referenceControle(pEdit->getControl()) ;

        //        // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
        //

        pDlgCurrent->_iIndexEdit++ ;      }      else
      {
        //
        // Index du contrôle
        //
        nIndex = pDlgCurrent->_iIndexEdit ;
        //
        // Type de saisie dans le contrôle
        //

        string sTypeDate = string("") ;
        //
        //trouver l
        //
        size_t pos = sIdentite.find(string("£")) ;
        if (NPOS != pos)
        {
          string sLivre = string(sIdentite, pos + 1, 1) ;
          strcpy(szType, sLivre.c_str()) ;

          sTypeDate = string(sIdentite, pos + 2, 1) ;
          strcpy(szEditDateType, sTypeDate.c_str()) ;
        }

        //
        // Création de l'objet OWL correspondant à l'objet d'interface
        //
        NSEdit* pEdit = (NSEdit*) 0 ;
        WNDTYPE	iEditType = isEdit ;
        switch (szType[0])
        {
          case codeMARK :
            pEdit = new NSEdit(pContexte, pDlgCurrent, ctrlID, szType, sLang) ;
            break ;
          case nbMARK :
            pEdit = new NSEdit(pContexte, pDlgCurrent, ctrlID, szType, sLang) ;
            break ;
          case charMARK :
            pEdit = new NSEdit(pContexte, pDlgCurrent, ctrlID, szType, sLang) ;
            break ;
          case dateMARK :
            //NSEditDate*
            pEdit = new NSEditDate(pContexte, pDlgCurrent, ctrlID, szType, sLang, sTypeDate[0]) ;
            iEditType = isEditDate ;
            break ;
          case dateHeureMARK :
            pEdit = new NSEditDateHeure(pContexte, pDlgCurrent, ctrlID, szType, sLang, sTypeDate[0]) ;
            iEditType = isEditDateHeure ;
            break ;
          case heureMARK :
            pEdit = new NSEditHeure(pContexte, pDlgCurrent, ctrlID, szType, sLang) ;
            break ;
          default :
            string sErrMess = string("Unknown Edit type : ") + string(1, szType[0]) ;
            erreur(sErrMess.c_str(), standardError, 0) ;
        }
        if (pEdit)
        {
          pEdit->setControl(new NSControle(pContexte, pDlgCurrent->_pBBItem/*, cGroupe*/,
                                                    sIdentite, sDlgFonction)) ;
          pEdit->getControl()->setControle(dynamic_cast<void*>(pEdit)) ;
          // pEdit->pControle->setType(pEdit->donneType());  // doesn't work, always return a isEdit type
          pEdit->getControl()->setType(iEditType) ;
          pEdit->getControl()->setNSDialog(pDlgCurrent) ;
          pEdit->getControl()->setHelpText(sHelpText) ;
          pDlgCurrent->referenceControle(pEdit->getControl()) ;
          //
          // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
          //
          pDlgCurrent->_iIndexEdit++ ;
        }
      }
    }
    // Contrôle non relié à un BBFisItem
    else
    {
      // On ne le crée que s'il est attaché à une fonction
      if (string("") != sDlgFonction)
      {
        NSEdit* pEdit = new NSEdit(pContexte, pDlgCurrent, ctrlID, "", sLang) ;
        pEdit->setControl(new NSControle(pContexte, pDlgCurrent->_pBBItem/*, cGroupe*/,
                                                    sIdentite, sDlgFonction)) ;
        pEdit->getControl()->setControle(dynamic_cast<void*>(pEdit)) ;
        pEdit->getControl()->setType(pEdit->donneType()) ;
        pEdit->getControl()->setNSDialog(pDlgCurrent) ;
        pEdit->getControl()->setHelpText(sHelpText) ;
        pDlgCurrent->referenceControle(pEdit->getControl()) ;
        //
        // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
        //
        pDlgCurrent->_iIndexEdit++ ;
      }
    }

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }
  //
  // NSBitmap (Bitmap interactif)
  //----------------------------
  // $$ Pour l'instant ne participe pas au transfert
/*
  if (!strcmp(szClassName, "NSBitmap"))  {        NSBitmap* pNSBitmap = new NSBitmap(static_cast<TWindow*>(pDlgCurrent), ctrlID,
															pDlgCurrent->BitmapID,	pDlgCurrent->defRegions());
        pNSBitmap->pControle    = new NSControle(pDlgCurrent->pBBItem, sIdentite, sDlgFonction);
        pNSBitmap->pControle->setControle(dynamic_cast<void*>(pNSBitmap));
        pNSBitmap->pControle->setType(pNSBitmap->donneType());
        pNSBitmap->pControle->setNSDialog(pDlgCurrent);
        pDlgCurrent->referenceControle(pNSBitmap->pControle);
  }
*/

  // -------------------------------------------------------------------  //                                  Static
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "Static"))
  {
    if ((-1 != ctrlID) && (0xFFFF != ctrlID))
    {
      // Récupération de la longueur du texte du contrôle
      nMaxInput = ::GetDlgItemText(hDlg, ctrlID, szBuffer, nMaxInput) ;
      // Création de l'objet OWL correspondant à l'objet d'interface
      //new TStatic(pDlgCurrent, ctrlID, nMaxInput+1);
      NSStatic* pNSStatic  = new NSStatic(pContexte, static_cast<TWindow*>(pDlgCurrent), ctrlID) ;
      pNSStatic->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
      pNSStatic->pControle->setControle(dynamic_cast<void*>(pNSStatic)) ;
      pNSStatic->pControle->setType(pNSStatic->donneType()) ;
      pNSStatic->pControle->setNSDialog(pDlgCurrent) ;
      pNSStatic->pControle->setHelpText(sHelpText) ;
      pDlgCurrent->referenceControle(pNSStatic->pControle) ;
    }

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                       Button  (autre que OK et Cancel)
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "Button") &&
       (IDOK != ctrlID) && (IDCANCEL != ctrlID))
  {
    // Détermination du style
    int nIndex = ::GetWindowLong(hWnd, GWL_STYLE) ;

    //    // Contrôle RadioButton
    //
    if (((nIndex& BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)||
					            ((nIndex& BS_RADIOBUTTON) == BS_RADIOBUTTON))
    {      NSRadioButton* pRaBut = new NSRadioButton(pContexte, pDlgCurrent, ctrlID, pDlgCurrent->_pGroupCurr) ;
      pRaBut->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
      pRaBut->pControle->setControle(dynamic_cast<void*>(pRaBut)) ;
      pRaBut->pControle->setType(pRaBut->donneType()) ;
      pRaBut->pControle->setNSDialog(pDlgCurrent) ;
      pRaBut->pControle->setHelpText(sHelpText) ;
      pDlgCurrent->referenceControle(pRaBut->pControle) ;
    }
    //
    // Contrôle CheckBox
    //
    else if (((nIndex& BS_CHECKBOX) == BS_CHECKBOX)||
						        ((nIndex& BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX))
    {
      //TCheckBox* pChkBox = new TCheckBox(pDlgCurrent, ctrlID, pDlgCurrent->pGroupCurr);
      //pControle->pCtrl = dynamic_cast<void*>(pChkBox);
      //pControle->sTypeCtrl = "CheckBox";
    }
    //
    // Contrôle Button
    //
    else
    {
      NSButton* pButt = new NSButton(pContexte, pDlgCurrent, ctrlID);
      pButt->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, /*cGroupe,*/
																		sIdentite, sDlgFonction) ;
      pButt->pControle->setControle(dynamic_cast<void*>(pButt)) ;
      pButt->pControle->setType(pButt->donneType()) ;
      pButt->pControle->setNSDialog(pDlgCurrent) ;
      pButt->pControle->setHelpText(sHelpText) ;
      pDlgCurrent->referenceControle(pButt->pControle) ;
    }

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                          Boîte de liste
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "Listbox"))
  {
    //TListBox* pListB = new TListBox(pDlgCurrent, ctrlID);
    //pControle->pCtrl 		= dynamic_cast<void*>(pListB);
    //pControle->sTypeCtrl = szClassName;

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                              Combobox
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "Combobox"))
  {
    // is it a classification or not ?
    bool bClassif = false ;
    VectString aVecteurString ;
    NSSuper* pSuper = pContexte->getSuperviseur() ;
    pSuper->getFilGuide()->TousLesVrais("0CODE", "ES", &aVecteurString, "ENVERS") ;    if (!(aVecteurString.empty()))    {      string sIdentSens ;      NSDico::donneCodeSens(&sIdentite, &sIdentSens) ;      if (aVecteurString.contains(sIdentSens))        bClassif = true ;    }

    if (bClassif)
    {
      NSComboClassif* pComboB = new NSComboClassif(pContexte, pDlgCurrent, ctrlID) ;

      pComboB->pControle->setControle(dynamic_cast<void*>(pComboB)) ;
      pComboB->pControle->setType(pComboB->donneType()) ;
      pComboB->pControle->setNSDialog(pDlgCurrent) ;
      pComboB->pControle->setHelpText(sHelpText) ;
      pDlgCurrent->referenceControle(pComboB->pControle) ;

      //pControle->pCtrl 		= dynamic_cast<void*>(pComboB);
      //pControle->sTypeCtrl = szClassName;

      pDlgCurrent->_iIndexCtrl++ ;
      return true ;
    }
    else
    {
      NSComboSemantique* pComboB = new NSComboSemantique(pContexte, pDlgCurrent, ctrlID) ;

      pComboB->pControle->setControle(dynamic_cast<void*>(pComboB)) ;
      pComboB->pControle->setType(pComboB->donneType()) ;
      pComboB->pControle->setNSDialog(pDlgCurrent) ;
      pComboB->pControle->setHelpText(sHelpText) ;
      pDlgCurrent->referenceControle(pComboB->pControle) ;

      //pControle->pCtrl 		= dynamic_cast<void*>(pComboB);
      //pControle->sTypeCtrl = szClassName;

      pDlgCurrent->_iIndexCtrl++ ;
      return true ;
    }
  }

  // -------------------------------------------------------------------
  //                                  Group
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "Group"))
  {
    // pDlgCurrent->iIndexGroupe++;
    //pDlgCurrent->pGroupCurr = new TGroupBox(pDlgCurrent, ctrlID);

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                              ScrollBar
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "ScrollBar"))
  {
    // Création de l'objet OWL correspondant à l'objet d'interface
    new TScrollBar(pDlgCurrent, ctrlID) ;
    typeCtrl = "ScrollBar" ;

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                      Bouton BORLAND
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "BorBtn")
                && (ctrlID != IDOK)
                && (ctrlID != IDCANCEL)
                && (ctrlID != IDRETOUR)
                && (ctrlID != IDSUITE)
                && (ctrlID != IDHELP)
                && (ctrlID != IDHELPWWW)
                && (ctrlID != IDHELPNEW)
                && (ctrlID != IDTREEPASTE)
                && (ctrlID != IDCONCLUSION)
                && (ctrlID != IDBBKCALCUL)               )
  {
    // Détermination du style
    int nIndex = ::GetWindowLong(hWnd, GWL_STYLE) ;

    if ( !((nIndex& BBS_BITMAP) == BBS_BITMAP) )
    {
      NSButton* pButt = new NSButton(pContexte, pDlgCurrent, ctrlID) ;
      pButt->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
      pButt->pControle->setControle(dynamic_cast<void*>(pButt)) ;
      pButt->pControle->setType(pButt->donneType()) ;
      pButt->pControle->setNSDialog(pDlgCurrent) ;
      pButt->pControle->setHelpText(sHelpText) ;
      pDlgCurrent->referenceControle(pButt->pControle) ;
    }

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                          CheckBox BORLAND
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "BorCheck"))
  {
    NSCheckBox* pChkBox = new NSCheckBox(pContexte, pDlgCurrent, ctrlID, pDlgCurrent->_pGroupCurr) ;
    pChkBox->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
    pChkBox->pControle->setControle(dynamic_cast<void*>(pChkBox)) ;
    pChkBox->pControle->setType(pChkBox->donneType()) ;
    pChkBox->pControle->setNSDialog(pDlgCurrent) ;
    pChkBox->pControle->setHelpText(sHelpText) ;
    pDlgCurrent->referenceControle(pChkBox->pControle) ;

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                          RadioBouton BORLAND
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "BorRadio"))
  {
    NSRadioButton* pRaBut = new NSRadioButton(pContexte, pDlgCurrent, ctrlID, pDlgCurrent->_pGroupCurr) ;
    pRaBut->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
    pRaBut->pControle->setControle(dynamic_cast<void*>(pRaBut)) ;
    pRaBut->pControle->setType(pRaBut->donneType()) ;
    pRaBut->pControle->setNSDialog(pDlgCurrent) ;
    pRaBut->pControle->setHelpText(sHelpText) ;
    pDlgCurrent->referenceControle(pRaBut->pControle) ;

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                                  Onglets
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "SysTabControl32"))
  {
    NSOnglet* pOnglet = new NSOnglet(pContexte, pDlgCurrent, ctrlID) ;
    pOnglet->setLevel(pDlgCurrent->getIncrementedOngletsLevel()) ;

    NSControle* pControl = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
    pControl->setControle(dynamic_cast<void*>(pOnglet)) ;
    pControl->setType(pOnglet->donneType()) ;
    pControl->setNSDialog(pDlgCurrent) ;
    pControl->setHelpText(sHelpText) ;

    pOnglet->setControl(pControl) ;
    pDlgCurrent->referenceControle(pControl) ;

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }

  // -------------------------------------------------------------------
  //                          BorShade BORLAND
  // -------------------------------------------------------------------
  if (!strcmp(szClassName, "BorShade"))
  {
    // Détermination du style
    int nIndex = ::GetWindowLong(hWnd, GWL_STYLE) ;
    //
    // Contrôle Group
    //
    if (((nIndex& BSS_RGROUP) == BSS_RGROUP) ||
                            ((nIndex& WS_GROUP) == WS_GROUP))
    {
      if (string("") != sIdentite)
      {
        NSGroupBox* pGrpBox = new NSGroupBox(pContexte, pDlgCurrent, ctrlID) ;
        pGrpBox->pControle = new NSControle(pContexte, pDlgCurrent->_pBBItem, sIdentite, sDlgFonction) ;
        pGrpBox->pControle->setControle(dynamic_cast<void*>(pGrpBox)) ;
        pGrpBox->pControle->setType(pGrpBox->donneType()) ;
        pGrpBox->pControle->setHelpText(sHelpText) ;
        pDlgCurrent->referenceControle(pGrpBox->pControle) ;
        pDlgCurrent->_pGroupCurr = (OWL::TGroupBox*)pGrpBox ;
      }
      else
      {
        OWL::TGroupBox* pGroupBox = new OWL::TGroupBox(pDlgCurrent, ctrlID) ;
        pDlgCurrent->_aGroups.push_back(pGroupBox) ;
        pDlgCurrent->_pGroupCurr = pGroupBox ;
      }
    }

    pDlgCurrent->_iIndexCtrl++ ;
    return true ;
  }
  //
  // Incrémentation du compteur
  //
  pDlgCurrent->_iIndexCtrl++ ;

  return true ;
}
catch (...)
{
  erreur("Exception NSDialog::DlgConfigCtrl.", standardError, 0) ;
  return false ;
}
}

void
NSDialog::activateParent()
{
	if ((TWindowView*) NULL == _pView)
	{
		TWindow::BringWindowToTop() ;
		return ;
	}
	NSMUEView* pMueView = TYPESAFE_DOWNCAST(_pView, NSMUEView) ;
	if ((NSMUEView*) NULL == pMueView)
	{
		TWindow::BringWindowToTop() ;
		return;
	}
	pMueView->activateParent() ;
}

void
NSDialog::checkFreeText()
{
  NSButton* pStandardButton = (NSButton*) 0 ;
  NSButton* pCheckedButton  = (NSButton*) 0 ;

  // Check that a IDTL_ACTIF control exists
  //
  getFreeTextButtons(&pStandardButton, &pCheckedButton) ;
  if (((NSButton*) NULL == pCheckedButton) || ((NSButton*) NULL == pStandardButton))
    return ;

  pStandardButton->Show(SW_HIDE) ;
  pCheckedButton->Show(SW_SHOW) ;
}

void
NSDialog::uncheckFreeText()
{
  NSButton* pStandardButton = (NSButton*) 0 ;
  NSButton* pCheckedButton  = (NSButton*) 0 ;

  // Check that a IDTL_ACTIF control exists
  //
  getFreeTextButtons(&pStandardButton, &pCheckedButton) ;
  if (((NSButton*) NULL == pCheckedButton) || ((NSButton*) NULL == pStandardButton))
    return ;

  pStandardButton->Show(SW_SHOW) ;
  pCheckedButton->Show(SW_HIDE) ;
}

void
NSDialog::getFreeTextButtons(NSButton** ppStandardButton, NSButton** ppCheckedButton)
{
  if (((NSButton**) NULL == ppStandardButton) || ((NSButton**) NULL == ppCheckedButton))
    return ;

  *ppStandardButton = (NSButton*) 0 ;
  *ppCheckedButton  = (NSButton*) 0 ;

  if (_aNSCtrls.empty())
  	return ;

  for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
  {
    WNDTYPE iType = (*i)->getType() ;
    if (isBtn == iType)
    {
      NSButton* pButton = static_cast<NSButton*>((*i)->getControle()) ;
      if (pButton)
      {
        if      (pButton->GetId() == IDTL)
          *ppStandardButton = pButton ;
        else if (pButton->GetId() == IDTL_ACTIF)
          *ppCheckedButton  = pButton ;
      }
    }
  }
}

void
NSDialog::DlgConfigCtrl2(TWindow* /* pWnd */, void* /* lParam */)
//void DlgConfigCtrl2(TWindow* pWnd, void* lParam)
{
/*
	int  nIndex;                   // Position dans *pConfigCache
	char szType[25];             	 // Type de contrôle EDIT
	char szClassName[15],          // Nom de la classe du contrôle
  szBuffer[80],             // Utilisée pour les fonctions WINDOWS
	szLen[5],						 // Taille à saisir pour un Edit
	cGroupe;
	string typeCtrl = "";          // Type de contrôle
	string sIdentite, sDlgFonction;
	HWND hDlg;                     // Handle de la boîte de dialogue Parent.
	UINT  nMaxInput = 255;         // Nombre maximal de caractères à saisir
	void* pTransferBufCtrl = NULL; // adresse du buffer de transfert

	// Adresse de l'objet boîte de dialogue courante
	//NSDialog* pDlgCurrent = this;
	TWindow* pWind = reinterpret_cast<TWindow*>(lParam);
	NSDialog* pDlgCurrent = dynamic_cast<NSDialog*>(pWind);

	// Obtention de l'ID du contrôle et de son type WINDOWS
	int ctrlID  = pWnd->GetDlgCtrlID();
	::GetClassName(pWnd->HWindow, (LPSTR) szClassName, 15);
	pDlgCurrent->MessageBox(szClassName, "DIALOG CONFIG", MB_ICONEXCLAMATION);
	//szClassName = ((TDialog*)pWnd)->GetClassName();

	// Obtention du handle de la boîte de Dialogue
	//hDlg   = ::GetParent(hWnd);

	// Initialisation de Groupe et Identite
	nIndex = pDlgCurrent->iIndexCtrl;
	// string sCtrl = (*(pDlgCurrent->pConfigCtrl))[nIndex].getItem(0);
	// cGroupe = sCtrl[0];
	// sIdentite = (*(pDlgCurrent->pConfigCtrl))[nIndex].getItem(1);
	cGroupe = pDlgCurrent->pConfigCtrl[nIndex]->cGroupe;
	sIdentite = pDlgCurrent->pConfigCtrl[nIndex]->sIdentite;
	sDlgFonction = pDlgCurrent->pConfigCtrl[nIndex]->sDlgFonction;

	// Traitement suivant le type de contrôle rencontré
	// BVSP : Contrôle Onglets

	if (!strcmp(szClassName, "SaxTabs"))
	{
		delete pWnd;
		NSOnglet* pOnglet = new NSOnglet(pDlgCurrent, ctrlID);
		pOnglet->pControle = new NSControle(pDlgCurrent->pBBItem, cGroupe,
																		sIdentite, sDlgFonction);
		pOnglet->pControle->pNSCtrl   = dynamic_cast<void*>(pOnglet);
		pOnglet->pControle->iType     = pOnglet->donneType();
		pOnglet->pControle->pNSDialog = pDlgCurrent;
		pDlgCurrent->referenceControle(pOnglet->pControle);
	}

	// Incrémentation du compteur
	pDlgCurrent->iIndexCtrl++;

	return;
  */
}

// -----------------------------------------------------------------------------
// Function     : NSDialog::DisplayBlank(void)
// Arguments    : AUCUN
// Description  : Permet d'afficher des champs blancs
// Returns      : RIEN
// -----------------------------------------------------------------------------
void NSDialog::BlankDisplay(int id)
{
  SetDlgItemText(id, "") ;
}

// -----------------------------------------------------------------------------
// demander à la boîte de dialogue mère d'activer le controle
// suivant pControle sinon le premier
// -----------------------------------------------------------------------------
void
NSDialog::ActiveControlSuivant(NSControle* pControle)
{
	if (false == _aNSCtrls.empty())
	{
  	// Find current control
    //
		iterNSControle i = _aNSCtrls.begin() ;
		for ( ; (_aNSCtrls.end() != i) && (*i != pControle); i++) ;
		if (_aNSCtrls.end() != i)
			i++ ;  //suivant

		// Find first enabled control
    //
		while (_aNSCtrls.end() != i)
    {
    	if ((TControl*) NULL != (*i)->getControle())
      {
      	TControl* pCtrl = (TControl*) (*i)->getControle() ;
      	if (true == pCtrl->IsWindowEnabled())
        	break ;
      }
    	i++ ;
    }

		if (_aNSCtrls.end() != i)
		{
			(*i)->SetFocus() ;
			return ;
		}
	}
	//
	// Donner le focus au bouton Ok s'il existe sinon fermer la boîte
	//
	if (Parent->ChildWithId(IDOK))
		Parent->ChildWithId(IDOK)->SetFocus() ;
	else
		CmOk() ;

    //	(*(pNSCtrl->begin()))->SetFocus();
}

//---------------------------------------------------------------------------// demander à la boîte de dialogue mère d'activer le controle
// précédent pControle sinon le dernier
//---------------------------------------------------------------------------
void
NSDialog::ActiveControlPrecedent(NSControle* pControle)
{
	if (_aNSCtrls.empty())
		return ;

	iterNSControle i ;
	for (i = _aNSCtrls.begin() ; (_aNSCtrls.end() != i) && (*i != pControle) ; i++) ;
	if (_aNSCtrls.end() != i)
	{
		if (_aNSCtrls.begin() == i)
			(_aNSCtrls.back())->SetFocus() ;
		else
		{
			i-- ;  //précéedent
			(*i)->SetFocus() ;
		}
	}
}

//---------------------------------------------------------------------------
//  Function:		NSDialog::referenceControle(void* pNSControle)
//
//  Arguments:		pNSControle : pointeur (au type void) sur le controle
//
//  Description:	Sert à ajouter ce pointeur à la liste des objets de
//						contrôle (NSButton, NSCheckBox...) créés par NSDialog.
//						Cette liste sert, entre autres, à pouvoir les détruires
//						à la fermeture de la boite de dialogue.
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSDialog::referenceControle(NSControle* pNSControle)
{
	_aNSCtrls.push_back(pNSControle) ;
}

void
NSDialog::CreerControles()
{
try
{
  string sLibel ;

  string sLang = pContexte->getUserLanguage() ;

  int iBoxHeight = 10 ;    // hauteur du bouton
  int iBoxWidth  = 200 ;   // largeur du bouton
  int iBoxInterv = 3 ;     // intervalle entre deux boutons
  int iBoxTop    = 4 ;     // haut du 1er bouton à partir du haut du groupbox
  int iBoxLeft   = 5 ;     // gauche des boutons à partir de la gauche du groupbox

  int iGroupTop  = 3 ;     // haut du groupbox
  int iGroupLeft = 2 ;     // gauche du groupbox

  int iSeparLeft = 3 ;     // gauche du séparateur
  int iGB_Separ  = 3 ;     // intervalle entre le bas du groupbox et le séparateur
  int iSepar_Btn = 8 ;     // intervalle entre le séparateur et le bouton

  int iBtnHeight = 25 ;    // hauteur d'un bouton
  int iBtnWidth  = 43 ;    // hauteur d'un bouton
  int iBtnLeft   = 3 ;     // gauche du premier bouton

  int iBtn_bas   = 3 ;     // intervalle entre le bas du bouton et le bas de la boite

  int  iNbBoxes = 0 ;
  bool bTexteLibre = false ;

  // Combien de boites à cocher faut-il créer ?
  // Existe-t-il un texte libre ?
  //
  if (false == _pBBItem->_aBBItemFils.empty())
  {
    for (BBiter it = _pBBItem->_aBBItemFils.begin() ;
                                    _pBBItem->_aBBItemFils.end() != it ; it++)
    {
      if ((*it)->getItemLabel() == "#####1")
        bTexteLibre = true ;
      else
        iNbBoxes++ ;
    }
  }

  int iGroupWidth = (2 * iBoxLeft) + iBoxWidth ;
  int iTotalWidth = (2 * iGroupLeft) + iGroupWidth ;

  int iGroupHeight ;
  if (iNbBoxes > 0)
    iGroupHeight = (2 * iBoxTop) + (iNbBoxes * iBoxHeight) +
                        ((iNbBoxes-1) * iBoxInterv) ;
  else
    iGroupHeight = (2 * iBoxTop) ;

  int iTotalHeight = iGroupTop + iGroupHeight + iGB_Separ + iSepar_Btn +
                        iBtnHeight + iBtn_bas ;

  // On fixe la taille de la boite de dialogue
  //
  NS_CLASSLIB::TRect dlgSizeRect(0, 0, iTotalWidth, iTotalHeight) ;
  MapDialogRect(dlgSizeRect) ;

  // On crée le groupe
  //
  NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(iGroupLeft, iGroupTop,
                                 iGroupLeft + iGroupWidth,
                                 iGroupTop + iGroupHeight) ;
  MapDialogRect(cvtRect) ;

  TGroupBox* pGroup = new TGroupBox(this, -1, "",
                                      cvtRect.Left(), cvtRect.Top(),
                                      cvtRect.Width(), cvtRect.Height(),
                                      /* TModule* */ 0) ;
  pGroup->Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;

  // On crée les cases à cocher
  //
  if (false == _pBBItem->_aBBItemFils.empty())
  {
    int iTopOfBox = iGroupTop + iBoxTop ;

    GlobalDkd dCode(pContexte, sLang) ;

    int iID = 200 ;
    for (BBiter it = _pBBItem->_aBBItemFils.begin();
                                      _pBBItem->_aBBItemFils.end() != it ; it++)
    {
      if ((*it)->getItemLabel() != "#####1")
      {
        // Détermination du libellé du bouton

        Message Msg("");
        Msg.InitFromEtiquette((*it)->getItemLabel()) ;
        string sMixte = Msg.GetLexique() ;
        if (string("") != Msg.GetComplement())
          sMixte += string(1, intranodeSeparationMARK) + Msg.GetComplement() ;
        if (string("") != Msg.GetPluriel())
          sMixte += string(1, intranodeSeparationMARK) + Msg.GetPluriel() ;
        if (string("") != Msg.GetCertitude())
          sMixte += string(1, intranodeSeparationMARK) + Msg.GetCertitude() ;

        GENRE iGenreObj ;
        int   iCertObj ;
        sLibel = dCode.decodeMot(&sMixte, genreNull, &iGenreObj, &iCertObj) ;

        /*
        Message* pMsg = new Message("");
        pMsg->InitFromEtiquette((*i)->sEtiquette);
        pContexte->getDico()->trouvePathologData(sLang, &(pMsg->GetLexique()), &Data);
        Data.donneGenre(&iGenre);
        if (pMsg->GetPluriel() != "")
            Data.donneGenrePluriel(&iGenre);
        pGene->donneLibelleAffiche(&sLibel, &Data, iGenre);
        delete pMsg;
        */
        sLibel[0] = pseumaj(sLibel[0]) ;

        cvtRect = NS_CLASSLIB::TRect(iGroupLeft + iBoxLeft,
                                     iTopOfBox,
                                     iGroupLeft + iBoxLeft + iBoxWidth,
                                     iTopOfBox + iBoxHeight) ;
        MapDialogRect(cvtRect) ;

        if ((*it)->FilsProlongeable)
        {
          // Checkbox or Edit?
          //
          string sEditFormat = string("") ;
          string sEditUnit   = string("") ;
          string sEditLabel  = string("") ;
          if (false == (*it)->VectorFils.empty())
          {
            BBItem* pSonItem = *((*it)->VectorFils.begin()) ;
            if (pSonItem->_aBBItemFils.size() == 1)
            {
              BBFilsItem* pSonFilsItem = *(pSonItem->_aBBItemFils.begin()) ;
              sEditLabel = pSonFilsItem->getItemLabel() ;
              Message Msg ;
              Msg.InitFromEtiquette(sEditLabel) ;
              sEditUnit = Msg.GetUnit() ;
              if (string("") != sEditUnit)
                sEditFormat = Msg.GetLexique() ;
            }
          }

          if ((string("") == sEditUnit) || (string("") == sEditFormat))
          {
            NSCheckBox* pChkBx = new NSCheckBox(pContexte, this, iID,
                                              sLibel.c_str(),
                                              cvtRect.Left(), cvtRect.Top(),
                                              cvtRect.Width(), cvtRect.Height(),
                                              /*pGroup*/ 0, 0) ;
            pChkBx->pControle = new NSControle(pContexte, _pBBItem, (*it)->getItemLabel(), "") ;
            pChkBx->pControle->setControle(dynamic_cast<void*>(pChkBx)) ;
            pChkBx->pControle->setType(pChkBx->donneType()) ;
            pChkBx->pControle->setNSDialog(this) ;
            referenceControle(pChkBx->pControle) ;
          }
          else
          {
            NSStatic* pNSStatic  = new NSStatic(pContexte, this, iID, sLibel.c_str(),
                                                cvtRect.Left(), cvtRect.Top(),
                                              cvtRect.Width(), cvtRect.Height()) ;
            pNSStatic->pControle = new NSControle(pContexte, _pBBItem, (*it)->getItemLabel(), "") ;
            pNSStatic->pControle->setControle(dynamic_cast<void*>(pNSStatic)) ;
            pNSStatic->pControle->setType(pNSStatic->donneType()) ;
            pNSStatic->pControle->setNSDialog(this) ;
            referenceControle(pNSStatic->pControle) ;

            iID++ ;

            string sDisplayedUnit = string("") ;

            NSEdit* pEdit = (NSEdit*) 0 ;
            WNDTYPE	iEditType = isEdit ;

            string sLivre = string(sEditFormat, 1, 1) ;
            char szType[2] ;
            strcpy(szType, sLivre.c_str()) ;

            int iEditMargin = 90 ;
            int iEditXPos   = cvtRect.Left() + cvtRect.Width() - iEditMargin ;
            int iEditWidth  = 30 ;

            switch (sEditFormat[1])
            {
              case codeMARK :
                pEdit = new NSEdit(pContexte, this, iID, szType, "",
                                     iEditXPos, cvtRect.Top(), iEditWidth,
                                     cvtRect.Height(), sLang) ;
                break ;
              case nbMARK :
                {
                  pEdit = new NSEdit(pContexte, this, iID, szType, "",
                                     iEditXPos, cvtRect.Top(), iEditWidth,
                                     cvtRect.Height(), sLang) ;
                  string sUnitSens = string("") ;
                  NSDico::donneCodeSens(&sEditUnit, &sUnitSens) ;
                  if (string("20000") != sUnitSens)
                    sDisplayedUnit = sEditUnit ;
                }
                break ;
              case charMARK :
                pEdit = new NSEdit(pContexte, this, iID, szType, "",
                                     iEditXPos, cvtRect.Top(), iEditWidth,
                                     cvtRect.Height(), sLang) ;
                break ;
              case dateMARK :
                //NSEditDate*
                pEdit = new NSEditDate(pContexte, this, iID, szType, "",
                                     iEditXPos, cvtRect.Top(), iEditWidth,
                                     cvtRect.Height(), sLang, sEditFormat[2]) ;
                iEditType = isEditDate ;
                break ;
              case dateHeureMARK :
                pEdit = new NSEditDateHeure(pContexte, this, iID, szType, "",
                                     iEditXPos, cvtRect.Top(), iEditWidth,
                                     cvtRect.Height(), sLang, sEditFormat[2]) ;
                iEditType = isEditDateHeure ;
                break ;
              case heureMARK :
                pEdit = new NSEditHeure(pContexte, this, iID, szType, "",
                                     iEditXPos, cvtRect.Top(), iEditWidth,
                                     cvtRect.Height(), sLang) ;
                break ;
              default :
                string sErrMess = string("Unknown Edit type : ") + sEditFormat ;
                erreur(sErrMess.c_str(), standardError, 0) ;
            }
            if (pEdit)
            {
              string sItemLabel = (*it)->getItemLabel() + string(1, cheminSeparationMARK) + sEditLabel ;

              pEdit->setControl(new NSControle(pContexte, *((*it)->VectorFils.begin()),
                                                    sEditLabel, "")) ;
              pEdit->getControl()->setControle(dynamic_cast<void*>(pEdit)) ;
              // pEdit->pControle->setType(pEdit->donneType());  // doesn't work, always return a isEdit type
              pEdit->getControl()->setType(iEditType) ;
              pEdit->getControl()->setNSDialog(this) ;
              referenceControle(pEdit->getControl()) ;
              //
              // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
              //
              _iIndexEdit++ ;

              if (string("") != sDisplayedUnit)
              {
                iID++ ;

                string sUnitLib = string("") ;
                int iUnitWidth = iEditMargin - iEditWidth - 2 ;
                pContexte->getDico()->donneLibelle(sLang, &sDisplayedUnit, &sUnitLib) ;
                /* NSStatic* pStatic = */ new NSStatic(pContexte, this, iID,
                                            sUnitLib.c_str(),
                                            iEditXPos + iEditWidth + 2,
                                            cvtRect.Top(), iUnitWidth, cvtRect.Height()) ;
              }
            }
          }
        }
        else
        {
          NSRadioButton* pRaBut = new NSRadioButton(pContexte, this, iID,
                                            sLibel.c_str(),
                                            cvtRect.Left(), cvtRect.Top(),
                                            cvtRect.Width(), cvtRect.Height(),
                                            /*pGroup*/ 0, 0) ;
          pRaBut->pControle = new NSControle(pContexte, _pBBItem, (*it)->getItemLabel(), "") ;
          pRaBut->pControle->setControle(dynamic_cast<void*>(pRaBut)) ;
          pRaBut->pControle->setType(pRaBut->donneType()) ;
          pRaBut->pControle->setNSDialog(this) ;
          referenceControle(pRaBut->pControle) ;
        }

        iTopOfBox += iBoxHeight + iBoxInterv ;
        iID++ ;
      }
    }
  }

  // On crée le séparateur
  //
  int iSepareTop = iGroupHeight + iGroupTop + iGB_Separ ;
  cvtRect = NS_CLASSLIB::TRect(iSeparLeft, iSepareTop,
                                 iSeparLeft + iTotalWidth - (2 * iSeparLeft),
                                 iSepareTop + 3) ;
  MapDialogRect(cvtRect) ;
  TGroupBox* pDip = new TGroupBox(this, -1, "",
                                    cvtRect.Left(), cvtRect.Top(),
                                    cvtRect.Width(), cvtRect.Height(),
                                    /*TModule**/ 0) ;
  pDip->Attr.Style |= BSS_HDIP ;

  // On crée les boutons
  //
  int iNbBtn ;
  if (bTexteLibre)
    iNbBtn = 4 ;
  else
    iNbBtn = 3 ;

  int iBtnInterv = (iTotalWidth - (2 * iBtnLeft) - (iNbBtn * iBtnWidth)) / (iNbBtn - 1) ;

  int iBtnTopPos  = iSepareTop + iSepar_Btn ;
  int iBtnLeftPos = iBtnLeft ;

  cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos,
                                 iBtnLeftPos + iBtnWidth,
                                 iBtnTopPos + iBtnHeight) ;
  MapDialogRect(cvtRect) ;
  /*TButton* pBtOK =*/ new TButton(this, IDOK, "", cvtRect.Left(), cvtRect.Top(),
                                 cvtRect.Width(), cvtRect.Height(),
                                 /*isDefault*/ false, 0) ;
  iBtnLeftPos += iBtnWidth + iBtnInterv ;

  cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos,
                                 iBtnLeftPos + iBtnWidth,
                                 iBtnTopPos + iBtnHeight) ;
  MapDialogRect(cvtRect) ;
  /*TButton* pBtCn =*/ new TButton(this, IDCANCEL, "", cvtRect.Left(), cvtRect.Top(),
                                 cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
  iBtnLeftPos += iBtnWidth + iBtnInterv ;

  if (bTexteLibre)
  {
    cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos,
                                 iBtnLeftPos + iBtnWidth,
                                 iBtnTopPos + iBtnHeight) ;
    MapDialogRect(cvtRect) ;

    if (false == _pBBItem->_aBBItemFils.empty())
    {
      for (BBiter it = _pBBItem->_aBBItemFils.begin() ;
                                   _pBBItem->_aBBItemFils.end() != it ; it++)
      {
        if ((*it)->getItemLabel() == string("#####1"))
        {
          NSButton* pBtTla = new NSButton(pContexte, this, IDTL_ACTIF, "",
                                         cvtRect.Left(), cvtRect.Top(),
                                         cvtRect.Width(), cvtRect.Height(),
                                         /*isDefault*/ false, (TModule*) 0) ;
          pBtTla->pControle = new NSControle(pContexte, _pBBItem, (*it)->getItemLabel(), "") ;
          pBtTla->pControle->setControle(dynamic_cast<void*>(pBtTla)) ;
          pBtTla->pControle->setType(pBtTla->donneType()) ;
          pBtTla->pControle->setNSDialog(this) ;
          referenceControle(pBtTla->pControle) ;

          NSButton* pBtTl = new NSButton(pContexte, this, IDTL, "",
                                         cvtRect.Left(), cvtRect.Top(),
                                         cvtRect.Width(), cvtRect.Height(),
                                         /*isDefault*/ false, (TModule*) 0) ;
          pBtTl->pControle = new NSControle(pContexte, _pBBItem, (*it)->getItemLabel(), "") ;
          pBtTl->pControle->setControle(dynamic_cast<void*>(pBtTl)) ;
          pBtTl->pControle->setType(pBtTl->donneType()) ;
          pBtTl->pControle->setNSDialog(this) ;
          referenceControle(pBtTl->pControle) ;
        }
      }
    }

    iBtnLeftPos += iBtnWidth + iBtnInterv ;
  }

  cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos,
                                 iBtnLeftPos + iBtnWidth,
                                 iBtnTopPos + iBtnHeight) ;
  MapDialogRect(cvtRect) ;
  /*TButton* pBtHl =*/ new TButton(this, IDHELP, "", cvtRect.Left(), cvtRect.Top(),
                                 cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
  iBtnLeftPos += iBtnWidth + iBtnInterv ;

  //
  // Titre de la boite de dialogue
  //
  if (_pBBItem->_pBBFilsPere)
    SetDynamicCaption() ;

  //
  // Appel du SetupWindow() de TDialog
  //
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
  MoveWindow(dlgRect.left, dlgRect.top, nouvWindowWidth, nouvWindowHeight) ;
}
catch (...)
{
	erreur("Exception lors de la création dynamique de dialogue.", standardError, 0) ;
}
}

void
NSDialog::CreerControlesArchetype()
{
try
{
  if ((BBItem*) NULL == _pBBItem)
    return ;

	string sLang = pContexte->getUserLanguage() ;

  Cdialogbox *pDialogBox = _pBBItem->getArchetypeDialog(sLang) ;
  if ((Cdialogbox*) NULL == pDialogBox)
    return ;

  Ccontrol * pControl = pDialogBox->getFirstControl() ;
  if ((Ccontrol*) NULL == pControl)
    return ;

  // Crcdata* pRCData        = pDialogBox->getRCData();

  UINT  nMaxInput = 255 ;         // Nombre maximal de caractères à saisir

  // string sRCData = pRCData->getNSContro();
  string sRCData = pDialogBox->getStringRCData() ;

  NS_CLASSLIB::TRect cvtRect ;
  int prec = 0, suiv ;

	char szType[25] ;       // Type de contrôle EDIT
  char szEditDateType[2] ;         //type de date dans le controle EDIT (NSEditDate)
  // char szTypeControl[255] ;
  size_t posLex ;

  suiv = sRCData.find("|") ;

  while (pControl)
  {
    string sType     = pControl->getType() ;
    string sCoords   = pControl->getCoords() ;
    string sCaption  = pControl->getCaption() ;
    string sFilling  = pControl->getFilling() ;
    string sHelpText = pControl->getHelpText() ;
    int    ctrlID    = pControl->getRefId() ;

    // conversion du type en majuscules
    pseumaj(&sType) ;

    int coords[4] ;
    coords[0] = pControl->getX() ;
    coords[1] = pControl->getY() ;
    coords[2] = pControl->getW() ;
    coords[3] = pControl->getH() ;

    // Rectangle de conversion des coordonnées en unités de boite de dialogue
    cvtRect = NS_CLASSLIB::TRect(coords[0], coords[1], coords[0] + coords[2], coords[1] + coords[3]) ;

    MapDialogRect(cvtRect) ;

    coords[0] = cvtRect.Left() ;
    coords[1] = cvtRect.Top() ;
    coords[2] = cvtRect.Width() ;
    coords[3] = cvtRect.Height() ;

    string sDlgFonction = "" ;
    string sIdentite    = string(sRCData, prec, suiv - prec) ;

    if (string("") != sIdentite)
    {
      size_t fct_posit = sIdentite.find(':') ;
      if (string::npos != fct_posit)
      {
        if (fct_posit < strlen(sIdentite.c_str()))
          sDlgFonction = string(sIdentite, fct_posit + 1, strlen(sIdentite.c_str())) ;
        sIdentite = string(sIdentite, 0, fct_posit) ;
      }
    }

    // -------------------------------------------------------------------------
    // Traitement suivant le type de contrôle rencontré

    if (sType == string("SYSTREEVIEW32"))
    {
      // TreeView
      NSTreeWindow * pNSTreeWindow = new NSTreeWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
      pNSTreeWindow->Attr.Style = pControl->getIStyle() ;
      pNSTreeWindow->pBBitemNSTreeWindow = _pBBItem ;
      NSControle* pControl = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pControl->setControle(dynamic_cast<void *>(pNSTreeWindow)) ;
      pControl->setType(pNSTreeWindow->donneType()) ;
      pControl->setNSDialog(this) ;
      pControl->setFilling(sFilling) ;
      pControl->setHelpText(sHelpText) ;
      pNSTreeWindow->setControl(pControl) ;
      referenceControle(pControl) ;

      if ((pControl->getTransfert()) &&
                (pControl->getTransfert()->getFilsItem()))
      {
      	BBFilsItem* pSonItem    = pControl->getTransfert()->getFilsItem() ;
      	BBItem*     pFatherItem = pSonItem->getItemFather() ;
      	pNSTreeWindow->pBBitemNSTreeWindow = pFatherItem ;

        // BBFilsItem*	pCtrlBBFilsItem = pNSTreeWindow->pControle->getTransfert()->pBBFilsItem ;
        // if (!(pCtrlBBFilsItem->VectorFils.empty()))
        //    pNSTreeWindow->pBBitemNSTreeWindow = *(pCtrlBBFilsItem->VectorFils.begin()) ;

        //
    		// Créer le deuxième BBItem (GCONS le fils en cas de consultation par exemple)
    		//
        if (pFatherItem)
    		  /* int creer = */ pFatherItem->developperConsultation(pSonItem) ;
        /*
        if (!(pSonItem->PatPtahovide()))
        {
        	NSPatPathoArray* pPatPatho = *(pSonItem->getPatPatho()->begin()) ;
        	pNSTreeWindow->DispatcherPatPatho(pPatPatho, 0, 0, "") ;
        }
        */
      }
    }

    if (sType == string("SYSLISTVIEW32"))
    {
      // ListView
      if (sIdentite.find(string("LCADR")) != NPOS)
      {
        // Liste d'adresses
        NSAdrListWindow * pNSAdrListWindow = new NSAdrListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
        pNSAdrListWindow->Attr.Style = pControl->getIStyle() ;
        pNSAdrListWindow->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pNSAdrListWindow->pControle->setControle(dynamic_cast<void *>(pNSAdrListWindow)) ;
        pNSAdrListWindow->pControle->setType(pNSAdrListWindow->donneType()) ;
        pNSAdrListWindow->pControle->setHelpText(sHelpText) ;
        pNSAdrListWindow->pControle->setFilling(sFilling) ;
        pNSAdrListWindow->pControle->setNSDialog(this) ;

        // on ajoute ici un indicateur pour préciser que ce controle gère
        // lui-même la gestion multiple sans lancer de multi-dialogue
        pNSAdrListWindow->pControle->setGestionMultiple(true) ;
        referenceControle(pNSAdrListWindow->pControle) ;
      }
      else if (sIdentite.find(string("LCORR")) != NPOS)
      {
        NSCorListWindow * pNSCorListWindow = new NSCorListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
        pNSCorListWindow->Attr.Style = pControl->getIStyle() ;
        pNSCorListWindow->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pNSCorListWindow->pControle->setControle(dynamic_cast<void *>(pNSCorListWindow)) ;
        pNSCorListWindow->pControle->setType(pNSCorListWindow->donneType()) ;
        pNSCorListWindow->pControle->setFilling(sFilling) ;
        pNSCorListWindow->pControle->setHelpText(sHelpText) ;
        pNSCorListWindow->pControle->setNSDialog(this) ;
        // on ajoute ici un indicateur pour préciser que ce controle gère
        // lui-même la gestion multiple sans lancer de multi-dialogue
        pNSCorListWindow->pControle->setGestionMultiple(true) ;
        referenceControle(pNSCorListWindow->pControle) ;
      }
      else
      {
        string sItem ;
        size_t pos = sIdentite.find_last_of('/') ;
        if (pos != NPOS)
          sItem = string(sIdentite, pos + 1, strlen(sIdentite.c_str()) - pos - 1) ;
        else
          sItem = sIdentite ;

        if (sItem[0] == 'V')
        {
          NSHistorizedValListWindow * pNSHistoListWindow = new NSHistorizedValListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
          pNSHistoListWindow->Attr.Style = pControl->getIStyle() ;
          pNSHistoListWindow->setItemValue(sItem) ;
          pNSHistoListWindow->_pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
          pNSHistoListWindow->_pControle->setControle(dynamic_cast<void *>(pNSHistoListWindow)) ;
          pNSHistoListWindow->_pControle->setType(pNSHistoListWindow->donneType()) ;
          pNSHistoListWindow->_pControle->setFilling(sFilling) ;
          pNSHistoListWindow->_pControle->setHelpText(sHelpText) ;
          pNSHistoListWindow->_pControle->setNSDialog(this) ;

          // on ajoute ici un indicateur pour préciser que ce controle gère
          // lui-même la gestion multiple sans lancer de multi-dialogue
          pNSHistoListWindow->_pControle->setGestionMultiple(true) ;
          referenceControle(pNSHistoListWindow->_pControle) ;
        }
        else
        {
          NSHistorizedListWindow * pNSHistoListWindow = new NSHistorizedListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
          pNSHistoListWindow->Attr.Style = pControl->getIStyle() ;
          pNSHistoListWindow->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
          pNSHistoListWindow->pControle->setControle(dynamic_cast<void *>(pNSHistoListWindow)) ;
          pNSHistoListWindow->pControle->setType(pNSHistoListWindow->donneType()) ;
          pNSHistoListWindow->pControle->setFilling(sFilling) ;
          pNSHistoListWindow->pControle->setHelpText(sHelpText) ;
          pNSHistoListWindow->pControle->setNSDialog(this) ;

          // on ajoute ici un indicateur pour préciser que ce controle gère
          // lui-même la gestion multiple sans lancer de multi-dialogue
          pNSHistoListWindow->pControle->setGestionMultiple(true) ;
          referenceControle(pNSHistoListWindow->pControle) ;
        }
      }
    }

    if (sType == string("VALLISTVIEW32"))
    {
      // contrôle vallistview - from FLP
      NSHistorizedValListWindow * pNSHistoValWindow = new NSHistorizedValListWindow(this, pContexte, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
      pNSHistoValWindow->Attr.Style = pControl->getIStyle() ;
      pNSHistoValWindow->_pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pNSHistoValWindow->_pControle->setControle(dynamic_cast<void*>(pNSHistoValWindow)) ;
      pNSHistoValWindow->_pControle->setType(pNSHistoValWindow->donneType()) ;
      pNSHistoValWindow->_pControle->setFilling(sFilling);
      pNSHistoValWindow->_pControle->setHelpText(sHelpText) ;
      pNSHistoValWindow->_pControle->setNSDialog(this) ;
      // on ajoute ici un indicateur pour préciser que ce controle gère lui-même
      // la gestion multiple sans lancer de multi-dialogue
      pNSHistoValWindow->_pControle->setGestionMultiple(true) ;
      referenceControle(pNSHistoValWindow->_pControle) ;
    }

    if (sType == string("EDIT"))
    {
      // Contrôle Edit
      string sIden = "" ;
      if ((sIdentite.find(string("£C;")) != NPOS) || (sIdentite.find(string("/£C;")) != NPOS))
      {
        // NSEdit qui bosse avec le lexique
        NSEditLexique * pNSEdit = new NSEditLexique(this, pContexte, ctrlID, pContexte->getDico(), "", coords[0], coords[1], coords[2], coords[3], nMaxInput + 1) ;
        pNSEdit->setFreeTextEnabled(true) ;
        pNSEdit->Attr.Style = pControl->getIStyle() ;
        pNSEdit->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pNSEdit->pControle->setControle(dynamic_cast<void *>(pNSEdit)) ;
        pNSEdit->pControle->setType(pNSEdit->donneType()) ;
        pNSEdit->pControle->setFilling(sFilling) ;
        pNSEdit->pControle->setHelpText(sHelpText) ;
        pNSEdit->pControle->setNSDialog(this) ;
        referenceControle(pNSEdit->pControle) ;
      }
      else if (sIdentite.find(string("£CX")) != NPOS)
      {
        NSEditLexique * pNSEdit = new NSEditLexique(this, pContexte, ctrlID, pContexte->getDico(), "", coords[0], coords[1], coords[2], coords[3], nMaxInput + 1) ;
        pNSEdit->setFreeTextEnabled(false) ;
        pNSEdit->Attr.Style = pControl->getIStyle() ;
        pNSEdit->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pNSEdit->pControle->setControle(dynamic_cast<void *>(pNSEdit)) ;
        pNSEdit->pControle->setType(pNSEdit->donneType()) ;
        pNSEdit->pControle->setFilling(sFilling) ;
        pNSEdit->pControle->setHelpText(sHelpText) ;
        pNSEdit->pControle->setNSDialog(this) ;
        referenceControle(pNSEdit->pControle) ;
      }
      else if (sIdentite.find(string("£CC")) != NPOS)
      {        // Champ edit qui n'ouvre pas le lexique et aliment Complement        NSEditNoLex* pEdit = (NSEditNoLex*) 0 ;        strcpy(szType, "C0") ;        posLex = sIdentite.find(string("£CC")) ;        string sElemLex = string(sIdentite, posLex, BASE_LEXIQUE_LEN) ;        string sLen = string(sElemLex, 4, 2) ;        int iLen = atoi(sLen.c_str()) ;        pEdit = new NSEditNoLex(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang, iLen) ;        pEdit->Attr.Style = pControl->getIStyle() ;        pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction)) ;        pEdit->getControl()->setControle(dynamic_cast<void *>(pEdit)) ;
        pEdit->getControl()->setType(pEdit->donneType()) ;
        pEdit->getControl()->setFilling(sFilling) ;
        pEdit->getControl()->setHelpText(sHelpText) ;
        pEdit->getControl()->setNSDialog(this) ;
        referenceControle(pEdit->getControl()) ;

        // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.        _iIndexEdit++ ;
      }      else if (sIdentite.find(string("£CL")) != NPOS)      {        // Champ edit qui n'ouvre pas le lexique et alimente Texte libre        NSEditNoLex* pEdit = (NSEditNoLex*) 0 ;        posLex = sIdentite.find(string("£CL")) ;        string sElemLex = string(sIdentite, posLex, BASE_LEXIQUE_LEN) ;        strcpy(szType, string(sElemLex, 2, 2).c_str()) ;        string sLen = string(sElemLex, 4, 2) ;        int iLen = atoi(sLen.c_str()) ;        pEdit = new NSEditNoLex(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang, iLen) ;        pEdit->Attr.Style = pControl->getIStyle() ;        pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction)) ;        pEdit->getControl()->setControle(dynamic_cast<void *>(pEdit)) ;
        pEdit->getControl()->setType(pEdit->donneType()) ;
        pEdit->getControl()->setFilling(sFilling) ;
        pEdit->getControl()->setHelpText(sHelpText) ;
        pEdit->getControl()->setNSDialog(this) ;
        referenceControle(pEdit->getControl()) ;

        // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.        _iIndexEdit++ ;
      }      else
      {
        // Type de saisie dans le contrôle
        string sLivre ;
        string sTypeDate ;
        size_t pos = sIdentite.find(string("£")) ;
        if (pos != NPOS)
        {
          sLivre = string(sIdentite, pos + 1, 1) ;
          strcpy(szType, sLivre.c_str()) ;

          sTypeDate = string(sIdentite, pos + 2, 1) ;
          strcpy(szEditDateType, sTypeDate.c_str()) ;
        }

        // Création de l'objet OWL correspondant à l'objet d'interface
        NSEdit* pEdit = (NSEdit*) 0 ;
        WNDTYPE	iEditType = isEdit ;
        switch (szType[0])
        {
          case codeMARK       : pEdit = new NSEdit(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang) ;
                                break ;
          case nbMARK         : pEdit = new NSEdit(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang) ;
                                break ;
          case charMARK       : pEdit = new NSEdit(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang) ;
                                break ;
          case dateMARK       : // NSEditDate*
                                pEdit = new NSEditDate(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang, szEditDateType[0]) ;
                                iEditType = isEditDate ;
                                break ;
          case dateHeureMARK  : pEdit = new NSEditDateHeure(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang, szEditDateType[0]) ;
                                iEditType = isEditDateHeure ;
                                break ;
          case heureMARK      : pEdit = new NSEditHeure(pContexte, this, ctrlID, szType, "", coords[0], coords[1], coords[2], coords[3], sLang) ;
                                break ;
          default             : string sErrMess = string("Unknown Edit type : ") + string(1, szType[0]) ;
                                erreur(sErrMess.c_str(), standardError, 0) ;
        }

        if (pEdit)
        {
        	pEdit->Attr.Style = pControl->getIStyle() ;

          pEdit->setControl(new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction)) ;
          pEdit->getControl()->setControle(dynamic_cast<void *>(pEdit)) ;
          // pEdit->pControle->setType(pEdit->donneType()) ; // doesn't work, always set a isEdit type
          pEdit->getControl()->setType(iEditType) ;
          pEdit->getControl()->setFilling(sFilling) ;
          pEdit->getControl()->setHelpText(sHelpText) ;
          pEdit->getControl()->setNSDialog(this) ;
          referenceControle(pEdit->getControl()) ;

          // Incrémentation de iIndexEdit de 1 pour passer à l'Edit suivant.
          _iIndexEdit++ ;
        }
      }
    }

    // Static
    if ((sType == string("STATIC")) || (sType == string("BORSTATIC")))
    {
      // Création de l'objet OWL correspondant à l'objet d'interface
      NSStatic* pNSStatic = new NSStatic(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
      pNSStatic->Attr.Style = pControl->getIStyle() ;
      pNSStatic->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pNSStatic->pControle->setControle(dynamic_cast<void *>(pNSStatic)) ;
      pNSStatic->pControle->setType(pNSStatic->donneType()) ;
      pNSStatic->pControle->setFilling(sFilling) ;
      pNSStatic->pControle->setHelpText(sHelpText) ;
      pNSStatic->pControle->setNSDialog(this) ;
      referenceControle(pNSStatic->pControle) ;
    }

    if ((sType == string("BUTTON")) &&
          (ctrlID != IDOK) && (ctrlID != IDCANCEL) &&
          (ctrlID != IDB_OK) && (ctrlID != IDB_CANCEL))
    {
      // Contrôle Button (à voir pour OK et Cancel)

      // -----------------------------------------------------------------------
      // ATTENTION : Il faut penser à tester du plus grand au plus petit sinon
      // un groupbox répond oui à radiobouton (par exemple)
      // #define BS_PUSHBUTTON       0x00000000L
      // #define BS_DEFPUSHBUTTON    0x00000001L
      // #define BS_CHECKBOX         0x00000002L
      // #define BS_AUTOCHECKBOX     0x00000003L
      // #define BS_RADIOBUTTON      0x00000004L
      // #define BS_3STATE           0x00000005L
      // #define BS_AUTO3STATE       0x00000006L
      // #define BS_GROUPBOX         0x00000007L
      // #define BS_USERBUTTON       0x00000008L
      // #define BS_AUTORADIOBUTTON  0x00000009L

      // Détermination du style
      // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE);
      int nIndex = pControl->getIStyle();

      if ((nIndex & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)      {
        // Contrôle AutoRadioButton

        // Création de l'objet OWL correspondant à l'objet d'interface
        NSRadioButton * pRaBut = new NSRadioButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], _pGroupCurr) ;
        pRaBut->Attr.Style = pControl->getIStyle() ;
        pRaBut->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pRaBut->pControle->setControle(dynamic_cast<void *>(pRaBut)) ;
        pRaBut->pControle->setType(pRaBut->donneType()) ;
        pRaBut->pControle->setFilling(sFilling) ;
        pRaBut->pControle->setHelpText(sHelpText) ;
        pRaBut->pControle->setNSDialog(this) ;
        referenceControle(pRaBut->pControle) ;
      }
      else if ((nIndex & BS_GROUPBOX) == BS_GROUPBOX)
      {
        // Contrôle Groupbox
        if (sIdentite != "")
        {
          NSGroupBox * pGrpBox = new NSGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
          pGrpBox->Attr.Style = pControl->getIStyle() ;
          pGrpBox->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
          pGrpBox->pControle->setControle(dynamic_cast<void *>(pGrpBox)) ;
          pGrpBox->pControle->setType(pGrpBox->donneType()) ;
          pGrpBox->pControle->setFilling(sFilling) ;
          pGrpBox->pControle->setHelpText(sHelpText) ;
          pGrpBox->pControle->setNSDialog(this) ;
          referenceControle(pGrpBox->pControle) ;
          _pGroupCurr = (OWL::TGroupBox *)pGrpBox ;
        }
        else
        {
          OWL::TGroupBox* pGroupBox = new OWL::TGroupBox(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
          pGroupBox->Attr.Style = pControl->getIStyle() ;
          _aGroups.push_back(pGroupBox) ;
          _pGroupCurr = pGroupBox ;
        }
      }
      else if ((nIndex & BS_RADIOBUTTON) == BS_RADIOBUTTON)
      {        // Contrôle RadioButton

        // Création de l'objet OWL correspondant à l'objet d'interface
        NSRadioButton * pRaBut = new NSRadioButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], _pGroupCurr) ;
        pRaBut->Attr.Style = pControl->getIStyle() ;
        pRaBut->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pRaBut->pControle->setControle(dynamic_cast<void *>(pRaBut)) ;
        pRaBut->pControle->setType(pRaBut->donneType()) ;
        pRaBut->pControle->setFilling(sFilling) ;
        pRaBut->pControle->setHelpText(sHelpText) ;
        pRaBut->pControle->setNSDialog(this) ;
        referenceControle(pRaBut->pControle) ;
      }
      else if (((nIndex& BS_CHECKBOX) == BS_CHECKBOX) || ((nIndex& BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX))
      {
        // Contrôle CheckBox
        NSCheckBox * pChkBox = new NSCheckBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], _pGroupCurr) ;
        pChkBox->Attr.Style = pControl->getIStyle() ;
        pChkBox->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pChkBox->pControle->setControle(dynamic_cast<void *>(pChkBox)) ;
        pChkBox->pControle->setType(pChkBox->donneType()) ;
        pChkBox->pControle->setFilling(sFilling) ;
        pChkBox->pControle->setHelpText(sHelpText) ;
        pChkBox->pControle->setNSDialog(this) ;
        referenceControle(pChkBox->pControle) ;
      }
      else
      {
        // Contrôle Button (par défaut ne participe au transfert )
        NSButton * pButt = new NSButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
        pButt->Attr.Style = pControl->getIStyle() ;
        pButt->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pButt->pControle->setControle(dynamic_cast<void *>(pButt)) ;
        pButt->pControle->setType(pButt->donneType()) ;
        pButt->pControle->setFilling(sFilling) ;
        pButt->pControle->setHelpText(sHelpText) ;
        pButt->pControle->setNSDialog(this) ;
        referenceControle(pButt->pControle) ;
      }
    }
    else if (sType == string("BUTTON"))
    {
      TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
      pButt->Attr.Style = pControl->getIStyle() ;
    }

    if (sType == string("SCROLLBAR"))
    {
      // Contrôle ScrollBar
      int nIndex = pControl->getIStyle() ;
      if ((nIndex& WS_HSCROLL) == WS_HSCROLL)
      {        // Création de l'objet OWL correspondant à l'objet d'interface
        TScrollBar* pScroll = new TScrollBar(this, ctrlID, coords[0], coords[1], coords[2], coords[3], true) ;
        pScroll->Attr.Style = pControl->getIStyle() ;
      }
      else
      {
        // Création de l'objet OWL correspondant à l'objet d'interface
        TScrollBar* pScroll = new TScrollBar(this, ctrlID, coords[0], coords[1], coords[2], coords[3], false) ;
        pScroll->Attr.Style = pControl->getIStyle() ;
      }
      // typeCtrl = "ScrollBar" ;
    }

    // BORLAND : Contrôle Bouton
    if ((sType == string("BORBTN")) &&  (ctrlID != IDOK)          && (ctrlID != IDCANCEL)     &&
                                        (ctrlID != IDB_OK)        && (ctrlID != IDB_CANCEL)   &&
                                        (ctrlID != IDRETOUR)      && (ctrlID != IDSUITE)      &&
                                        (ctrlID != IDHELP)        && (ctrlID != IDHELPWWW)    &&
                                        (ctrlID != IDHELPNEW)     && (ctrlID != IDTREEPASTE)  &&
                                        (ctrlID != IDCONCLUSION)  && (ctrlID != IDBBKCALCUL))
    {
      // Détermination du style
      // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE);
      int nIndex = pControl->getIStyle() ;

      if (!((nIndex & BBS_BITMAP) == BBS_BITMAP))
      {
        // Contrôle RadioButton
        NSButton * pButt = new NSButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
        pButt->Attr.Style = pControl->getIStyle() ;
        pButt->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
        pButt->pControle->setControle(dynamic_cast<void *>(pButt)) ;
        pButt->pControle->setType(pButt->donneType()) ;
        pButt->pControle->setFilling(sFilling) ;
        pButt->pControle->setHelpText(sHelpText) ;
        pButt->pControle->setNSDialog(this) ;
        referenceControle(pButt->pControle) ;
      }
    }
    else if (sType == string("BORBTN"))
    {
      // For dialogs, buttons IDB_OK and IDB_CANCEL (that are not created for
      // windows) are visible and given the IDOK and IDCANCEL IDs
      //
      if      (IDB_OK == ctrlID)
        ctrlID = IDOK ;
      else if (IDB_CANCEL == ctrlID)
        ctrlID = IDCANCEL ;

      TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
      pButt->Attr.Style = pControl->getIStyle() ;
    }

    // BORLAND : Contrôle CheckBox
    if (sType == string("BORCHECK"))
    {
      NSCheckBox * pChkBox = new NSCheckBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], _pGroupCurr) ;
      pChkBox->Attr.Style = pControl->getIStyle() ;
      pChkBox->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pChkBox->pControle->setControle(dynamic_cast<void *>(pChkBox)) ;
      pChkBox->pControle->setType(pChkBox->donneType()) ;
      pChkBox->pControle->setFilling(sFilling) ;
      pChkBox->pControle->setHelpText(sHelpText) ;
      pChkBox->pControle->setNSDialog(this) ;
      referenceControle(pChkBox->pControle) ;
    }

    if (sType == string("BORRADIO"))
    {
      // BORLAND : Contrôle RadioBouton
      NSRadioButton * pRaBut = new NSRadioButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], _pGroupCurr) ;
      pRaBut->Attr.Style = pControl->getIStyle() ;
      pRaBut->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pRaBut->pControle->setControle(dynamic_cast<void *>(pRaBut)) ;
      pRaBut->pControle->setType(pRaBut->donneType()) ;
      pRaBut->pControle->setFilling(sFilling) ;
      pRaBut->pControle->setHelpText(sHelpText) ;
      pRaBut->pControle->setNSDialog(this) ;
      referenceControle(pRaBut->pControle) ;
    }

    if (sType == string("SYSTABCONTROL32"))
    {
      // BORLAND : Contrôle Onglets
      NSOnglet * pOnglet = new NSOnglet(pContexte, this, ctrlID, coords[0], coords[1], coords[2], coords[3]) ;
      pOnglet->Attr.Style = pControl->getIStyle() ;

      NSControle* pControl = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      pControl->setControle(dynamic_cast<void *>(pOnglet)) ;
      pControl->setType(pOnglet->donneType()) ;
      pControl->setFilling(sFilling) ;
      pControl->setHelpText(sHelpText) ;
      pControl->setNSDialog(this) ;

      pOnglet->setControl(pControl) ;
      referenceControle(pControl) ;
    }

    if (sType == string("COMBOBOX"))
    {
    	// is it a classification or not ?
      bool bClassif = false ;
      VectString aVecteurString ;
			NSSuper* pSuper = pContexte->getSuperviseur() ;
			pSuper->getFilGuide()->TousLesVrais("0CODE", "ES", &aVecteurString, "ENVERS") ;			if (false == aVecteurString.empty())			{				string sIdentSens ;				NSDico::donneCodeSens(&sIdentite, &sIdentSens) ;        if (aVecteurString.contains(sIdentSens))        	bClassif = true ;			}
      if (bClassif)
			{
      	// BORLAND : Contrôle Combobox
      	NSComboClassif * pComboB = new NSComboClassif(pContexte, this, ctrlID, coords[0], coords[1], coords[2], coords[3], pControl->getIStyle(), 0) ;
        pComboB->Attr.Style = pControl->getIStyle() ;
      	// NSComboClassif * pComboB = new NSComboClassif(this, ctrlID) ;
      	pComboB->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      	pComboB->pControle->setControle(dynamic_cast<void *>(pComboB)) ;
      	pComboB->pControle->setType(pComboB->donneType()) ;
      	pComboB->pControle->setFilling(sFilling) ;
        pComboB->pControle->setHelpText(sHelpText) ;
      	pComboB->pControle->setNSDialog(this) ;
      	referenceControle(pComboB->pControle) ;
      }
      else
      {
      	NSComboSemantique * pComboB = new NSComboSemantique(pContexte, this, ctrlID, coords[0], coords[1], coords[2], coords[3], pControl->getIStyle(), 0) ;
        pComboB->Attr.Style = pControl->getIStyle() ;
      	// NSComboClassif * pComboB = new NSComboClassif(this, ctrlID) ;
      	pComboB->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
      	pComboB->pControle->setControle(dynamic_cast<void *>(pComboB)) ;
      	pComboB->pControle->setType(pComboB->donneType()) ;
      	pComboB->pControle->setFilling(sFilling) ;
        pComboB->pControle->setHelpText(sHelpText) ;
      	pComboB->pControle->setNSDialog(this) ;
      	referenceControle(pComboB->pControle) ;
      }
    }

    // BORLAND : Contrôle BorShade
    if (sType == string("BORSHADE"))
    {
      // Détermination du style
      // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE) ;
      int nIndex = pControl->getIStyle() ;

      if (((nIndex & BSS_RGROUP) == BSS_RGROUP) || ((nIndex & WS_GROUP) == WS_GROUP))
      {
        // Contrôle Group
        if (sIdentite != "")
        {
          NSGroupBox * pGrpBox = new NSGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
          pGrpBox->Attr.Style = pControl->getIStyle() ;
          pGrpBox->pControle = new NSControle(pContexte, _pBBItem, sIdentite, sDlgFonction) ;
          pGrpBox->pControle->setControle(dynamic_cast<void *>(pGrpBox)) ;
          pGrpBox->pControle->setType(pGrpBox->donneType()) ;
          pGrpBox->pControle->setFilling(sFilling) ;
          pGrpBox->pControle->setHelpText(sHelpText) ;
          pGrpBox->pControle->setNSDialog(this) ;
          referenceControle(pGrpBox->pControle) ;
          _pGroupCurr = (OWL::TGroupBox *)pGrpBox ;
        }
        else
        {
          OWL::TGroupBox* pGroupBox = new OWL::TGroupBox(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
          pGroupBox->Attr.Style = pControl->getIStyle() ;
          // aGroups.push_back(pGroupBox) ;
          _pGroupCurr = pGroupBox ;
        }
      }
    }

/*
    if (ctrlID == IDBBKCALCUL)
    {
      // cas où le contrôle n'est pas un des contrôles précédents, mais que
      // c'est le contrôle qui lance le calcul par le Blackboard
      NSControle * pCtr = new NSControle(pBBItem, sIdentite, sDlgFonction) ;
      referenceControle(pCtr) ;
    }
*/

    // Incrémentation du compteur
    _iIndexCtrl++ ;
    prec = suiv + 1 ;
    suiv = sRCData.find("|", prec) ;
    pControl = pDialogBox->getNextControl(pControl) ;
  }

  // On fixe la taille de la boite de dialogue
  NS_CLASSLIB::TRect dlgSizeRect(0, 0, pDialogBox->getW(), pDialogBox->getH()) ;
  MapDialogRect(dlgSizeRect) ;

  // Appel du SetupWindow() de TDialog
  TDialog::SetupWindow() ;

  // On fixe la caption du dialogue
  string sDlgCaption = pDialogBox->getCaption() ;
  if (sDlgCaption[0] == '$')
  {
    if (sDlgCaption == string("$NAME"))
    {
      if (NULL != pContexte->getPatient())
        sDlgCaption = pContexte->getPatient()->getNomLong() ;
      else
        sDlgCaption = "" ;
    }
    else
      sDlgCaption = "" ;
  }
  SetCaption(sDlgCaption.c_str()) ;

  // Redimentionnement
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect clientRect ;
  GetClientRect(clientRect) ;

  // On compare le clientRect réel avec les dimensions souhaitées, et on modifie
  // le WindowRect en conséquence
  int nouvWindowWidth  = dlgRect.Width()  + (dlgSizeRect.Width()  - clientRect.Width()) ;
  int nouvWindowHeight = dlgRect.Height() + (dlgSizeRect.Height() - clientRect.Height()) ;
  MoveWindow(dlgRect.left, dlgRect.top, nouvWindowWidth, nouvWindowHeight) ;

  // Evaluation du rectangle utilisable à l'écran
  NS_CLASSLIB::TRect usableRect ;

  // Taille de l'écran - Screen size
  TScreenDC screenDC ;
  int iHorzRes = screenDC.GetDeviceCaps(HORZRES) ;
  int iVertRes = screenDC.GetDeviceCaps(VERTRES) ;

  usableRect = NS_CLASSLIB::TRect(dlgRect.TopLeft(), NS_CLASSLIB::TPoint(iHorzRes, iVertRes)) ;

  // TaskBar de Windows
  // RECT        rect ;
  APPBARDATA  AppBarData ;

  AppBarData.hWnd = ::FindWindow("Shell_TrayWnd", NULL) ;
  if (AppBarData.hWnd != 0)
  {
    AppBarData.cbSize = sizeof(AppBarData) ;
    int iResult = ::SHAppBarMessage(ABM_GETTASKBARPOS, &AppBarData) ;
    if (iResult)
    {
      switch (AppBarData.uEdge)
      {
        case ABE_BOTTOM : usableRect.bottom = AppBarData.rc.top ;
                          break ;
        case ABE_LEFT   : if (usableRect.Left() < AppBarData.rc.right)
                            usableRect.left = AppBarData.rc.right ;
                          break ;
        case ABE_RIGHT  : if (usableRect.Right() > AppBarData.rc.left)
                            usableRect.right = AppBarData.rc.left ;
                          break ;
        case ABE_TOP    : break ;
      }
    }
  }

  int newTop;
  if (nouvWindowHeight > usableRect.Height())
  {
    newTop = dlgRect.top - (nouvWindowHeight - usableRect.Height()) ;
  }

  if (newTop < 0)
    newTop = 0 ;

  if (newTop < dlgRect.top)
    MoveWindow(dlgRect.left, newTop, nouvWindowWidth, nouvWindowHeight) ;
}
catch (...)
{
  erreur("Exception NSDialog::CreerControlesArchetype.", standardError, 0) ;
}
}

void
NSDialog::SetDynamicCaption()
{
  if (((BBItem*) NULL == _pBBItem) || ((BBFilsItem*) NULL == _pBBItem->_pBBFilsPere))
  {
    SetCaption("") ;
    return ;
  }

  NSDico* pDico = pContexte->getDico() ;

  if (((NSDico*) NULL == pDico) || (NULL == pDico->getGenerateur()))
  {
    SetCaption("") ;
    return ;
  }

  string sLang = pContexte->getUserLanguage() ;

  Message Msg ;
  Msg.InitFromEtiquette(_pBBItem->_pBBFilsPere->getItemLabel()) ;

  NSPathologData Data ;
  pContexte->getDico()->trouvePathologData(sLang, &(Msg.GetLexique()), &Data) ;

  GENRE iGenre ;
  Data.donneGenre(&iGenre) ;
  if (string("") != Msg.GetPluriel())
    Data.donneGenrePluriel(&iGenre) ;

  NSGenerateur* pGene = pDico->getGenerateur() ;

  string sLibel = string("") ;
  pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;
  sLibel = pseumaj(sLibel) ;

  SetCaption(sLibel.c_str()) ;
}

// -----------------------------------------------------------------------------// Function     :	NSDialog::detruitControles()
// Arguments    :	Rien
// Description  :	Sert à détruire les objets de contrôle (NSButton, NSCheckBox)
//						    créés par NSDialog.
// Returns      :	Rien
// -----------------------------------------------------------------------------
voidNSDialog::detruitControles()
{
  _aNSCtrls.vider() ;
}


// -----------------------------------------------------------------------------
// Function     :	NSDialog::initControlesFromBbk()
// Description  :	Initialise les contrôles à partir du blackboard.
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
NSDialog::initControlesFromBbk(string sAction)
{
	// No use to init from bbk if patient is not opened
	//
	if ((NSPatientChoisi*) NULL == pContexte->getPatient())
		return ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;
  if ((BB1BBInterface*) NULL == pContexte->getBBinterface())
		return ;

  if (pContexte->getBBinterface())
	  pContexte->getBBinterface()->connectTokenToWindow(_pBBItem->_KsInterface.getTokenId(), HWindow) ;

	if (_aNSCtrls.empty())
  	return ;

  // L'archetype est par convention dans le champ fils du BBItem lanceur
  string sArchetype = string(_pBBItem->getItemSonsList()) ;

  pSuper->afficheStatusMessage("Interrogation du Blackboard...") ;

  // bool bAsynchronousQuestionsAsked = false ;

  for (iterNSControle i = _aNSCtrls.begin(); _aNSCtrls.end() != i; i++)  {  	if ((*i)->getTransfert() && ((*i)->isToFilling(sAction)))    {      string sCheminBBFilsItem = string("") ;    	(*i)->getPathForBlackboard(&sCheminBBFilsItem) ;      NSSearchStruct searchStruct = *((*i)->getSearchStruct()) ;
      // searchStruct._sStartingDatePattern = (*i)->getFillingEndPeriod() ;      // (*i)->getEndValidityDate(&(searchStruct._sStartingDate)) ;

      // on a ici un chemin non vide = une question à poser au blackboard
      NSPatPathoArray* pPatPathoArray = NULL ;
      string date_fils = string("") ;

      // on pose la question au blackboard
			int res = pContexte->getBBinterface()->precoche(sCheminBBFilsItem, &pPatPathoArray, HWindow, &date_fils, true, &searchStruct) ;
			if ((1 == res) && (pPatPathoArray) && (false == pPatPathoArray->empty()))
        // il y a une réponse du blackboard ==>
        // on transmet la patpatho au BBFilsItem via le pTransfert
        // On met à jour le contrôle
        //
				initControlFromBbkAnswer(*i, pPatPathoArray, &date_fils) ;

      else if ((*i)->initOnTimer())
      {
      	// bAsynchronousQuestionsAsked = true ;

        // If Edit control, tell him that it is waiting for BBK answer
        //
        if ((*i)->getType() == isEdit)
        {
          NSEdit* pNSEditControl = static_cast<NSEdit*>((*i)->getControle()) ;
          if (pNSEditControl)
            pNSEditControl->setWaitingForOb1(true) ;
        }

        // TypedVal typ(sCheminBBFilsItem) ;
      	// AskDeterministicQuestion *DPIOMessage = new AskDeterministicQuestion("From dialog", &typ, 10) ;
  			// pSuper->bbkToDo(pBBItem->pBigBoss->pContexte, 0, "AskDeterministicQuestion", "", "", DPIOMessage, true, NULL, false) ;
      }
      if (pPatPathoArray)
        delete pPatPathoArray ;
		}
  }  // if (bAsynchronousQuestionsAsked)  //	SetTimer(ID_OB1_TIMER, 1000) ;}void
NSDialog::initControlFromBbkAnswer(NSControle* pControl, NSPatPathoArray* pPatPathoArray, string* psDate)
{
	if ((NSControle*) NULL == pControl)
  	return ;
  if (((NSPatPathoArray*) NULL == pPatPathoArray) || pPatPathoArray->empty())
  	return ;

  pControl->initFromArray(pPatPathoArray) ;

  // If Edit control, tell him to stop waiting
  //
  if (pControl->getType() == isEdit)
  {
    NSEdit* pNSEditControl = static_cast<NSEdit*>(pControl->getControle()) ;
    if (pNSEditControl)
      pNSEditControl->setWaitingForOb1(false) ;
  }

  if ((string*) NULL == psDate)
		return ;

  //
  // Recherche d'un contrôle qui contiendrait la date de cette information
  // We look for another control whose subject would be this control's date
  //
  if (_aNSCtrls.empty())
    return ;

  iterNSControle k = _aNSCtrls.begin() ;
  for ( ; (_aNSCtrls.end() != k) && (*k != pControl) ; k++) ;
  if (_aNSCtrls.end() == k)
  	return ;

  string sIdentite = pControl->getIdentite() ;
  bool trouve = false ;

  k++ ;

  while (_aNSCtrls.end() != k)
  {
    if (((*k)->getFilling() == "D") && ((*k)->getPathControl() == sIdentite))
    {
      trouve = true ;
      break ;
    }
    k++ ;
  }

  if (false == trouve)
		return ;

  // on regarde si on a une date ou une date et heure
  string sDateIdent = (*k)->getIdentite() ;
  string typeDate = string("") ;
  size_t pos = sDateIdent.find_last_of("/£") ;
  if (string::npos != pos)
  	typeDate = string(sDateIdent, pos, 2) ;

  string date_fils = *psDate ;

  if ((typeDate == "£T") || (typeDate == "£D"))
  {
  	NVLdVTemps tpsObj ;
    tpsObj.initFromDate(date_fils) ;

    if (typeDate == "£T")
    	date_fils = tpsObj.donneDateHeure() ;
    else
    	date_fils = tpsObj.donneDate() ;
  }

  (*k)->getTransfert()->getTransfertMessage()->SetComplement(date_fils) ;
  (*k)->getTransfert()->getFilsItem()->Active() ; //rendre ce fils actif
  (*k)->prepareControle() ;
}

// -----------------------------------------------------------------------------
// Function     :	NSDialog::rafraichitControles()
// Description  :	Initialise les contrôles en fonction de l'état du BBItem.
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
NSDialog::rafraichitControles()
{
	if (_aNSCtrls.empty())
  	return ;

  for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
  	(*i)->prepareControle() ;
}

// -----------------------------------------------------------------------------
//  Function    : NSDialog::initialiseControles()
//  Description :	Initialisation des contrôles après TDialog::SetupWindow()
//						    - Lance l'éventuelle fonction d'initialisation des contrôles.
//						    - Initialise l'onglet
//  Returns     : Rien
// -----------------------------------------------------------------------------
void
NSDialog::initialiseControles()
{
	if (_aNSCtrls.empty())
		return ;

  size_t iTabIndex = 0 ;

	for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
	{
    // Si le contrôle est un onglet, on l'initialise
    //
    if ((*i)->getType() == isOnglet)
    {
      NSOnglet* pTab = static_cast<NSOnglet*>((*i)->getControle()) ;
      if (pTab && (iTabIndex < _aConfigTabs.size()))
    	  pTab->InitialiseTabs(_aConfigTabs[iTabIndex]) ;

      iTabIndex++ ;
    }

    // Initializing tooltip
    //
    if (string("") != (*i)->getHelpText())
    {
      TControl* pControl = static_cast<TControl*>((*i)->getControle()) ;
      if (pControl)
      {
        const TToolInfo toolInfo(*this, pControl->HWindow, (*i)->getHelpText().c_str()) ;
        _pToolTip->AddTool(toolInfo) ;
      }
    }

    // On lance l'éventuelle fonction d'initialisation
    //
		if ((*i)->getFonction())
    	(*i)->getFonction()->execute(NSDLGFCT_CREATION) ;
	}
}

void
NSDialog::initControlesFromCapture()
{
  if (pContexte->getEpisodus() == NULL)
    return ;

  NSCaptureArray* pCapt = &(pContexte->getEpisodus()->CaptureArray) ;
  if (pCapt->empty())
    return ;

  if (_aNSCtrls.empty())
		return ;

  // ici on en est
  for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
  {
    if ((*i)->getTransfert())
    {
      // on récupère le chemin du BBItem père du BBFilesItem associé au
      // NSControle via son transfert
      //
      BBFilsItem* pFilsItem = (*i)->getTransfert()->getFilsItem() ;
      Message*    pMessage  = (*i)->getTransfert()->getTransfertMessage() ;

      string sCheminBBFilsItem = string("") ;
      string sEtiq = string("") ;

      if (pFilsItem)
      {
      	// on récupère le chemin du BBItem père
      	sCheminBBFilsItem = pFilsItem->getItemFather()->_sLocalisation ;
      	// on récupère l'étiquette du fils
      	string  sEtiquette = pFilsItem->getItemLabel() ;
      	NSDico::donneCodeSens(&sEtiquette, &sEtiq) ;
      }

      for (CaptureIter captIter = pCapt->begin() ; pCapt->end() != captIter ; captIter++)
      {
      	if (string("") != (*captIter)->getChemin())
        {
          //
          // On regarde si le chemin de l'élément se termine par le chemin
          // de capture
          //
          string sCheminCapture ;
          string sPath = (*captIter)->getChemin() ;
          NSDico::donneCodeSens(&sPath, &sCheminCapture) ;

          // Attention, la détection automatique ajoute GCONS1, il faut le
          // supprimer
          if ((strlen(sCheminCapture.c_str()) > 6) && (string(sCheminCapture, 0, 6) == "GCONS/"))
            sCheminCapture = string(sCheminCapture, 6, strlen(sCheminCapture.c_str())-6) ;

          string sCheminCompare = string("") ;

          size_t iCaptLen = strlen(sCheminCapture.c_str()) ;
          size_t iBBLen   = strlen(sCheminBBFilsItem.c_str()) ;

          if (iBBLen >= iCaptLen)
              sCheminCompare = string(sCheminBBFilsItem, iBBLen - iCaptLen, iCaptLen) ;

          //if (sCheminCapture == sCheminBBFilsItem)
          if (sCheminCompare == sCheminCapture)
          {
            if      ('£' == sEtiq[0])
            {
              if (pMessage)
                pMessage->SetTexteLibre((*captIter)->getLibelle()) ;
              if (pFilsItem)
                pFilsItem->Active() ;
            }
            else if ('2' == sEtiq[0])
            {
              size_t iChemMark = sEtiq.find(cheminSeparationMARK) ;
              if (string::npos != iChemMark)
              {
                string  sUnit   = string(sEtiq, 0, iChemMark) ;
                string  sLexiq  = string(sEtiq, iChemMark + 1, strlen(sEtiq.c_str()) - iChemMark - 1) ;

                string sValeur = (*captIter)->getLibelle() ;

                // Conversion d'unités
                if ((string("") != (*captIter)->getUnit()) && (sUnit != (*captIter)->getUnit()))
                {
                  NSCV NsCv(pContexte->getSuperviseur()) ;
                  DBIResult Resultat = NsCv.open() ;
                  if (DBIERR_NONE != Resultat)
                  {
                    erreur("Erreur à l'ouverture de la base convert.", standardError, Resultat) ;
                    sValeur = string("") ;
                  }
                  else
                  {
                    double dVal = StringToDouble(sValeur) ;
                    if (NsCv.ConvertirUnite(&dVal, sUnit, (*captIter)->getUnit()))
                      sValeur = DoubleToString(&dVal, 0, 2) ;
                    else
                      sValeur = string("") ;

                    NsCv.close();
                  }
                }

                if (string("") != sValeur)
                {
                  if (pMessage)
                  {
                    pMessage->SetLexique(sLexiq) ;
                    pMessage->SetUnit(sUnit) ;
                    pMessage->SetComplement(sValeur) ;
                  }
                  if (pFilsItem)
                    pFilsItem->Active() ;
                }
              }
            }
            else
            {
              string sLabel ;
              string sLib = (*captIter)->getLibelle() ;
              NSDico::donneCodeSens(&sLib, &sLabel) ;
              if (sEtiq == sLabel)
                pFilsItem->Active() ;
            }
            (*i)->executeKillFocusBehaviour() ;
            (*i)->prepareControle() ;
            break ;
          }
        }
      }
    }
  }
}

void NSDialog::TestClick(WPARAM)
{
	::MessageBox(0, "Click sur un bouton", "Configuration", MB_OK);
}

void
NSDialog::ForceCanClose()
{
	canCloseOk = false ;
}

// -----------------------------------------------------------------------------// Function     : NSDialog::Canclose()// Arguments    : AUCUN
// Description  : Finit le sousclassement des contrôles EDIT pour laisser la
//                place propre
// Returns      : true
// -----------------------------------------------------------------------------
bool
NSDialog::CanClose()
{
    //
    // En cas de perte du focus, on cache l'éventuelle fenêtre de pilotage
    // de description multiple pour éviter sa sélection à partir d'une fenêtre
    // fille de cette boite de dialogue
    //
    if (canCloseOk)
    {
        canCloseOk = false;
        return false;
    }
    TWindow* pMere = Parent;
    NSDialog* pDialog = TYPESAFE_DOWNCAST(pMere, NSDialog);
    if (pDialog)
    {
        if (pDialog->_pControleurMulti)
        {
            pDialog->_pControleurMulti->Show(SW_SHOWNORMAL);
            pDialog->SetFocus();
        }
    }

    //
    // Si on "remplace" une CQVue, il faut fermer la vue
    //
    //if (pView)
    //    ::SendMessage(pView->GetHandle(), WM_CLOSE, 0, 0) ;

    return true;
}

//--------------------------------------------------------------------------// quand on selectionne une boite de dialogue
//	utilisée surtout pour les boîtes multidialogue
//--------------------------------------------------------------------------
void
NSDialog::EvActivate(uint active, bool minimized, HWND hwnd)
{
	string ps = string("Entrée dans EvActivate de NSDialog") ;	pContexte->getSuperviseur()->trace(&ps, 1) ;

	TWindow::EvActivate(active, minimized, hwnd) ;

	if (((WA_ACTIVE == active) || (WA_CLICKACTIVE == active)) && (!minimized))
	{
		if (_pControleurMulti)
			_pControleurMulti->setActive(this) ;

		if ((string("") != _sHelpBodyUrl) || (string("") != _sHelpIndexUrl))
		{
			NSSuper* pSuper = pContexte->getSuperviseur() ;
			if (pSuper)
			{
				pSuper->setAideIndex(_sHelpIndexUrl) ;
				pSuper->setAideCorps(_sHelpBodyUrl) ;
			}
			else
			{
				pSuper->setAideIndex("") ;
				pSuper->setAideCorps("zz_generique.htm") ;
			}
		}
	}

	if (NULL == _pView)
	{
  	ps = string("Sortie de EvActivate de NSDialog (view is Null)") ;
		pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
		return ;	}	NSMUEView* pMueView = TYPESAFE_DOWNCAST(_pView, NSMUEView) ;
	if (NULL == pMueView)
  {
  	ps = string("Sortie de EvActivate de NSDialog (MueView is Null)") ;
		pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
		return ;	}

	NSPaneSplitter* pPaneSplit = pMueView->getPaneSplitter() ;
	if ((NULL == pPaneSplit) || (NULL == pPaneSplit->_pMDIChild))
  {
  	ps = string("Sortie de EvActivate de NSDialog (Invalid PaneSplitter is Null)") ;
		pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
		return ;	}

	if (((active == WA_ACTIVE) || (active == WA_CLICKACTIVE)) && (!minimized))
	{
  	/****************************
		HWND hForeGround = ::GetForegroundWindow() ;
  	    if ((hForeGround == GetHandle()) || (hForeGround == pPaneSplit->pMDIChild->GetHandle()))
			return ;
    ******************************/

		if (_bActivateMUEView)
    {
      NSSuper* pSuper = pContexte->getSuperviseur() ;
      NSToDoTask* pTask = new NSToDoTask ;
      pTask->setWhatToDo(string("ActivateMUEView")) ;
      pTask->setPointer1((void*) pMueView, false) ;
      pTask->setPointer2((void*) this, false) ;

      pTask->sendMe(pSuper) ;
    }
    else
    	_bActivateMUEView = true ;

		// pPaneSplit->pMDIChild->PostMessage(WM_ACTIVATE, MAKEWPARAM(active, minimized), LPARAM(hwnd)) ;
	}
    /**********************************
	else
	{
  	    SetWindowPos(pPaneSplit->pMDIChild->GetHandle() ,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE	) ;
	}
    *************************************/

	ps = string("Sortie de EvActivate de NSDialog") ;
	pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}boolNSDialog::EvNCActivate(bool active){	TDialog::EvNCActivate(active) ;  if (active)		activateParent() ;  return true ;}voidNSDialog::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point){	TDialog::EvLButtonDown(modKeys, point) ;	if (NULL == _pView)		return ;	NSMUEView* pMueView = TYPESAFE_DOWNCAST(_pView, NSMUEView) ;
	if ((NSMUEView*) NULL == pMueView)
		return ;

	NSPaneSplitter* pPaneSplit = pMueView->getPaneSplitter() ;
	if (((NSPaneSplitter*) NULL == pPaneSplit) || (NULL == pPaneSplit->_pMDIChild))
		return ;

	// Send WM_PARENTNOTIFY to parent's MDIchild
  LPARAM wparam = MAKEWPARAM(WM_LBUTTONDOWN, 0) ;
  LPARAM lparam = MAKELPARAM(WORD(point.X()), WORD(point.Y())) ;
  pPaneSplit->_pMDIChild->PostMessage(WM_PARENTNOTIFY, wparam, lparam) ;
	// pPaneSplit->pMDIChild->PostMessage(WM_PARENTNOTIFY, MAKEWPARAM(WM_LBUTTONDOWN, 0), MAKELPARAM(point.X(), point.Y())) ;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void
NSDialog::CmHelp()
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;
	if (pSuper)
	{
		if ((string("") != _sHelpBodyUrl) || (string("") != _sHelpIndexUrl))
		{
			pSuper->setAideIndex(_sHelpIndexUrl) ;
			pSuper->setAideCorps(_sHelpBodyUrl) ;
		}
		else
		{
			pSuper->setAideIndex("") ;
			pSuper->setAideCorps("zz_generique.htm") ;
		}
	}

	pContexte->NavigationAideEnLigne() ;
}

voidNSDialog::CmTreePaste()
{
try
{
	string sLocalis = _pBBItem->_sLocalisation ;

	NSPatPathoArray PathoSynthese(pContexte->getSuperviseur()) ;

	bool continuer = true ;

	if ((pContexte->getPatient()) && (pContexte->getPatient()->getDocHis()))
	{
		NSDocHistoArray *pDocArray = pContexte->getPatient()->getDocHis()->getVectDocument() ;

		if (false == pDocArray->empty())
		{
			DocumentIter iterDoc = pDocArray->begin() ;
			while ((pDocArray->end() != iterDoc) && continuer)
			{
        NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
        (*iterDoc)->initFromPatPatho(&PPT) ;

        if (false == PPT.empty())
        {
				  PatPathoIter iter = PPT.begin() ;
				  if ((*iter)->getLexiqueSens() == string("ZSYNT"))
          {
        	  PathoSynthese = PPT ;
            continuer = false ;
          }
          else
        	  iterDoc++ ;
        }
        else
        	iterDoc++ ;
      }
    }
  }

	if (PathoSynthese.empty())
		return ;

	string sousChaine, sLocaCherche ;
	PatPathoIter IterPpt = PathoSynthese.begin() ;

/*
  bool bChercher = true;
  while (bChercher && (sLocalis != ""))
  {
    sLocaCherche = sLocalis ;
    IterPpt = pPathoSynthese->ChaineDansPatpatho(sLocaCherche, &sousChaine, string("|")) ;
        if (IterPpt == pPathoSynthese->end())
        {
        	size_t posSepare = sLocalis.find('/');
            if (posSepare == NPOS)
                sLocalis = "";
            else
                sLocalis = string(sLocalis, posSepare+1, strlen(sLocalis.c_str())-posSepare-1);
        }
        else
        	bChercher = false;
    } */
	sLocaCherche = sLocalis ;	size_t posSepare = sLocaCherche.find('/') ;
	while (posSepare != NPOS)
	{
		sLocaCherche = string(sLocaCherche, posSepare+1, strlen(sLocaCherche.c_str())-posSepare-1) ;
		posSepare = sLocaCherche.find('/') ;
	}
	IterPpt = PathoSynthese.ChaineDansPatpatho(sLocaCherche, &sousChaine, string("|")) ;

	if ((NULL == IterPpt) || (PathoSynthese.end() == IterPpt))
	{
		// delete pPathoSynthese;
		return ;
	}
	if (NULL == _pBBItem->_pPPTEnCours)
		return ;

	// On est sur l'élément, ce sont ses fils qui nous intéressent
	int iColRef = (*IterPpt)->getColonne() + 1 ;
	int iLigRef = (*IterPpt)->getLigne() + 1 ;

	// On vérifie qu'il existe bien des données à récupérer avant de
	// remettre à zéro le BBItem
	IterPpt++ ;
	if ((PathoSynthese.end() == IterPpt) || ((*IterPpt)->getColonne() < iColRef))
		return ;

	BBFilsItem*	pFilsPere =	_pBBItem->_pBBFilsPere ;
	if (NULL == pFilsPere)
		return ;

	NSVectFatheredPatPathoArray* pVect = pFilsPere->getPatPatho() ;
	if (pVect->empty())
		pVect->push_back(new NSFatheredPatPathoArray(pContexte->getSuperviseur(), 0, new NSPatPathoArray(pContexte->getSuperviseur()))) ;
	else
		(*(pVect->begin()))->getPatPatho()->vider() ;
	FatheredPatPathoIterVect iJ = pVect->begin() ;

	// pBBItem->RemetAZero();

	while ((PathoSynthese.end() != IterPpt) &&
           ((*IterPpt)->getColonne() >= iColRef))
	{
		NSPatPathoInfo* pPpt = new NSPatPathoInfo(*(*IterPpt)) ;
		pPpt->setColonne(pPpt->getColonne() - iColRef) ;
		pPpt->setLigne(pPpt->getLigne() - iLigRef) ;
		(*iJ)->getPatPatho()->push_back(pPpt) ;

		IterPpt++ ;
	}

	pFilsPere->Redeveloppe() ;
	//rafraichitControles();
	//Invalidate();
}
catch (...)
{
	erreur("Exception NSDialog::CmTreePaste.", standardError, 0) ;
}
}

void
NSDialog::CmWarnForNoFreeText()
{
  string sWarn = pContexte->getSuperviseur()->getText("lexiqueManagement", "freeTextNotAllowed") ;
  erreur(sWarn.c_str(), warningError, 0, this->HWindow) ;
}

//-------------------------------------------------------------------------
//  Function: 		void NSDialog::CmOk()
//
//  Arguments:		AUCUN
//  Description:	Répond à OK.
//  Returns:		RIEN
//---------------------------------------------------------------------------
void
NSDialog::CmOk()
{
	// If Ok already activated, do nothing
  //
	if (_bOkActivated)
		return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

	_bOkActivated = true ;

	if (_bMereMUEView)
		canCloseOk = true ;

	//
  // Les conditions de fermeture sont-elles remplies ?
  //
  if (false == _aNSCtrls.empty())
  	for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
    	if (false == (*i)->canWeClose())
      {
      	_bOkActivated = false ;
      	return ;
      }

	string sMessage ;
	// vérification des contraintes (SFMG RC)
	if (false == _pBBItem->verifConstraintItem(&sMessage))
	{
    string sWarningMsg = pSuper->getText("archetypesMessages", "someMandatoryInformationIsMissing") ;
  	erreur(sWarningMsg.c_str(), warningError, 0) ;
    _bOkActivated = false ;
    return ;
  }

	bool bRootArchetype = false ;
  if ((_pBBItem == _pBBItem->_pBigBoss->getBBItem()) && (_pBBItem->ouvreArchetype()))
  	bRootArchetype = true ;

  //
  // Déclenchement du calcul
  //
  if (bRootArchetype)
  	CmBbkCalcul() ;

	_pBBItem->donneRetourDlg(0) ;

	//
  // S'il s'agit de NSTreeWindow et d'un texte libre, alors enregistrer
  // ce texte
  //
  if (false == _aNSCtrls.empty())
  {
  	for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
    {
    	if ((*i)->getType() == isTreeWindow)
      {
      	NSTreeWindow* pNSTreeWindow = static_cast<NSTreeWindow*>((*i)->getControle());
        if (pNSTreeWindow && pNSTreeWindow->getEditDico())
        	pNSTreeWindow->getEditDico()->TextLibre() ;
      }
    }
  }

  // Attention : pour la fiche administrative, qui ne se ferme pas sur le OK
  // il ne faut pas détacher les contrôles
  bool bDetacheCtrls = true ;
  if (bRootArchetype)
  {
    NSSuper* pSuper = pContexte->getSuperviseur() ;

    string sArcName = _pBBItem->getArchetypeName() ;
    if (pSuper->getDemographicArchetypeId() == sArcName)
      bDetacheCtrls = false ;
  }

  executePreClosingFunctions() ;

  // Le BBItem donne-t-il l'autorisation de fermer ?
  // Does the BBItem allow this dialog to close ?
  //
	if (false == _pBBItem->okFermerDialogue(true, bDetacheCtrls))
  {
  	_bOkActivated = false ;
  	return ;
  }

  //
  // S'il s'agit de NSTreeWindow tuer pEDitDico
  //
  closeEditDico() ;

  //
  // Eventuel lancement de fonction
  //
  executeClosingFunctions() ;

  //
  // Si la boite de dialogue est un formulaire
  //
  if (bRootArchetype && (false == _pBBItem->_bModalDialog))
  {
  	// On signale la volonté de fermer ; c'est pBigBoss qui
    // fermera le questionnaire après éventuel enregistrement
    //
    /*bool bRet =*/ _pBBItem->_pBigBoss->fermeBbkArchetype(IDOK, _pView) ;

    return ;
  }

  if (_pControleurMulti)
  {
  	_bOkActivated = false ;
  	_pControleurMulti->idOk(this, _iLastAction) ;
    _iLastAction = actionOk ;
  }
  else
  {
  	if (CanClose())
    	CloseWindow(IDOK) ;
  }
}

void NSDialog::CmSuite(){
	CmOk() ;
}

//-------------------------------------------------------------------------//  Function: 		void NSDialog::CmRetour()
//
//  Arguments:		AUCUN
//  Description:	Répond au bouton de retour en arrière
//  Returns:		RIEN
//---------------------------------------------------------------------------
void NSDialog::CmRetour()
{
  _iLastAction = actionOther ;

	executePreClosingFunctions() ;

	_pBBItem->donneRetourDlg(-1);
	if (false == _pBBItem->okFermerDialogue(true))
		return ;

  //
  // S'il s'agit de NSTreeWindow tuer pEDitDico
  //
  closeEditDico() ;

  //
  // Eventuel lancement de fonction
  //
  executeClosingFunctions() ;

  if (CanClose())
    CloseWindow(IDOK) ;
}

//-------------------------------------------------------------------------//  Function: 		void NSDialog::CmConclusion()
//
//  Arguments:		AUCUN
//  Description:	Répond au bouton de sortie par le bas
//  Returns:		RIEN
//---------------------------------------------------------------------------
void NSDialog::CmConclusion()
{
try
{
  _iLastAction = actionOther ;

	_pBBItem->donneRetourDlg(NSDLG_SORTIE_BAS) ;

	BBCmdMessage* pCmdMsg = new BBCmdMessage(NSDLG_SORTIE_BAS) ;
	for (int i = 0; i < 10; i++)
		_pBBItem->_pBigBoss->empile(pCmdMsg, false) ;
	delete pCmdMsg ;

  executePreClosingFunctions() ;

	if (false == _pBBItem->okFermerDialogue(true))
		return ;

  //
  // S'il s'agit de NSTreeWindow tuer pEDitDico
  //
  closeEditDico() ;

  //
  // Eventuel lancement de fonction
  //
  executeClosingFunctions() ;

  if (CanClose())
    CloseWindow(IDOK) ;
}
catch (...)
{
	erreur("Exception NSDialog::CmConclusion().", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Function    : void NSDialog::CmBbkCalcul()
// Arguments   : AUCUN
// Description : Répond au bouton de calcul via le blackboard
// Returns     : RIEN
// -----------------------------------------------------------------------------
void
NSDialog::CmBbkCalcul()
{
try
{
  _iLastAction = actionOther ;

	if (_aNSCtrls.empty())
		return ;

  string ps = string("Entering NSDialog::CmBbkCalcul") ;
	pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	bool trouve = false ;

  iterNSControle i = _aNSCtrls.begin() ;
	for ( ; _aNSCtrls.end() != i ; i++)
	{		TControl * pCtrl = static_cast<TControl *>((*i)->getControle()) ;		int idCtrl = pCtrl->GetDlgCtrlID() ;		if (idCtrl == IDBBKCALCUL)		{			trouve = true ;			break ;
		}
	}

	if (false == trouve)
		return ;

	SetCursor(pNSDLLModule, IDC_THINKING_CURSOR) ;

	// on enregistre les modifs de tous les fils du BBItem root
	_pBBItem->okFermerDialogue(true, false) ;

	NSPatPathoArray PatPathoArray(pContexte->getSuperviseur()) ;

	// L'archetype est par convention dans le champ fils du BBItem lanceur
	string sArchetype = _pBBItem->getItemSonsList() ;
	pSuper->afficheStatusMessage("Renseignement du Blackboard...") ;
	string sDejaRepondu = "" ;

  string sCheminBBFilsItem ;
	// Enumération de tous les contrôles situés avant le contrôle IDBBKCALCUL
	for (i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
	{    TControl * pCtrl = static_cast<TControl *>((*i)->getControle()) ;    int idCtrl = pCtrl->GetDlgCtrlID() ;    if (idCtrl == IDBBKCALCUL)      break ;    if (NULL != (*i)->getTransfert())    {      // on récupère le chemin du BBItem pere du BBFilsItem associé      // au NSControle via son pTransfert      sCheminBBFilsItem = (*i)->getPath() ;      sCheminBBFilsItem = (*i)->cutPath(&sCheminBBFilsItem, string("/0QUES/"), false) ;/*      sCheminBBFilsItem = (*i)->getTransfert()->pBBFilsItem->getItemFather()->sLocalisation ;      // on rajoute l'étiquette du fils      string sEtiq ;      string sEtiquette = (*i)->getTransfert()->pBBFilsItem->getItemLabel() ;      pSuper->getDico()->donneCodeSens(&sEtiquette, &sEtiq) ;
      sCheminBBFilsItem += string(1, cheminSeparationMARK) + sEtiq ;

      // on enleve le chemin jusqu'à "0QUES1"
      size_t pos = sCheminBBFilsItem.find("/0QUES/") ;      if (pos == string::npos)        continue ;

      sCheminBBFilsItem = string(sCheminBBFilsItem, pos + 7, strlen(sCheminBBFilsItem.c_str()) - pos - 7) ;
*/
      if (sCheminBBFilsItem == "")
        continue ;

      // Cas d'une valeur chiffrée sur un bouton : enlever ce qui suit /$
      size_t pos = sCheminBBFilsItem.find("/$") ;
      if (0 == pos)
        continue ;
      if (string::npos != pos)
        sCheminBBFilsItem = string(sCheminBBFilsItem, 0, pos) ;

      sCheminBBFilsItem = getRegularPath(sCheminBBFilsItem, cheminSeparationMARK, intranodeSeparationMARK) ;

      // on a ici un chemin non vide = une réponse à donner au blackboard
      PatPathoArray.vider() ;

      NSTransferInfo* pTransfert = (*i)->getTransfert() ;

      if ((*i)->getType() == isHistoryValListWindow)
      {
        // cas des listes d'elts multiples : on prend le pTransfert du père et
        // on récupère la premiere patpatho de ce transfert
        BBItem* pBBItemPere = pTransfert->getFilsItem()->getItemFather() ;
        if (pBBItemPere && pBBItemPere->_pBBFilsPere && (NULL != pBBItemPere->_pBBFilsPere->getItemTransfertData()))
        {
          NSTransferInfo *pTrans = pBBItemPere->_pBBFilsPere->getItemTransfertData() ;
          NSVectFatheredPatPathoArray* pFatheredArray = pTrans->getPatPatho() ;
          if (pFatheredArray && (false == pFatheredArray->empty()))
          {
            NSFatheredPatPathoArray* pFatheredElement = *(pFatheredArray->begin()) ;
            if (pFatheredElement && (false == pFatheredElement->getPatPatho()->empty()))
              PatPathoArray = *(pFatheredElement->getPatPatho()) ;
          }
        }
      }
      else
      {
        NSVectFatheredPatPathoArray* pFatheredArray = pTransfert->getPatPatho() ;
        if (pFatheredArray && (false == pFatheredArray->empty()) && (false == (*(pFatheredArray->begin()))->getPatPatho()->empty()))
        {
          NSFatheredPatPathoArray* pFatheredElement = *(pFatheredArray->begin()) ;
          PatPathoArray = *(pFatheredElement->getPatPatho()) ;
        }
        else if (pTransfert->getFilsItem()->isActif())
        {
          BBFilsItem* pFils = pTransfert->getFilsItem() ;
          BBItem*     pPere = pFils->getItemFather() ;
          if (pPere)
          {
            // pFils->getItemTransfertData()->setTmpTransfertMessage(pFils->getItemTransfertData()->getTransfertMessage()) ;
            pFils->getItemTransfertData()->setTmpActif(pFils->getItemTransfertData()->getActif()) ;

            // Modif PA 16/05/09
            // pPere->pTempPPT->vider() ;
            // pPere->AjouteTmpEtiquette(pFils) ;
            // PatPathoArray = *(pPere->pTempPPT) ;
            NSPatPathoArray TempPPT(pContexte->getSuperviseur()) ;
            pPere->AjouteTmpEtiquette(&TempPPT, pFils) ;
            PatPathoArray = TempPPT ;
          }
        }
      }

      // on donne les réponses au blackboard
      if (sCheminBBFilsItem != sDejaRepondu)
      {
				if ((pTransfert->getFilsItem()->VectorFils.empty()) && (PatPathoArray.size() > 1))
        {
        	// Insert the array as an answer to this control's path
          //
        	pContexte->getBBinterface()->insertAnswerOnBlackboard(sCheminBBFilsItem, &PatPathoArray, Undefined, HWindow) ;
          //
          // Insert leaves as elements with their own path
          //
        	insertLeavesOnBlackBoard(sCheminBBFilsItem, &PatPathoArray) ;
        }
        else
        	pContexte->getBBinterface()->insertAnswerOnBlackboard(sCheminBBFilsItem, &PatPathoArray, Undefined, HWindow) ;
      }
      // Evite qu'un bouton non clické remette à zéro ce que vient de mettre un bouton clické
      // To prevent a non clicked button to reset the value just set up by an activated button
      if (false == PatPathoArray.empty())
        sDejaRepondu = sCheminBBFilsItem ;
    }
	}

  iterNSControle iPostCalcul = i ;

/*
	// On libère le KS afin qu'il puisse effectuer les calculs
  string sKsName = pBBItem->KsInterface.getKsName() ;
  if (sKsName != "")
  	pSuper->getBBinterface()->driveKSfromDialog(sKsName, BB1BBInterface::ksFree) ;
*/

	string sErrMsg = pSuper->getText("bbkKsManagement", "blackboardInDuty") ;
	pSuper->afficheStatusMessage((char*)sErrMsg.c_str()) ;

	// This is the case when the KS is already running and waits for the user
  // to press Ok
  //
	if (_pBBItem->_KsInterface.getTokenId() > 0)
  {
		// On libère le token afin qu'il puisse lancer le KS afin d'effectuer les calculs
		pContexte->getBBinterface()->driveKSfromDialog(_pBBItem->_KsInterface.getTokenId(), BB1BBInterface::ksFree) ;

  	_bBlackboardStillAtWork = true ;

    waitForKS() ;
	}
  // This is the case when the KS will only start when results are asked
  // We ask the first question, then wait for the KS end of work signal
  //
  else
  {
  	bool bFirstCompute = true ;
		for (i = iPostCalcul ; _aNSCtrls.end() != i ; i++)
		{			if ((*i)->getTransfert())			{				// on récupère le chemin du BBItem pere du BBFilsItem associé				// au NSControle via son pTransfert      	//				sCheminBBFilsItem = getPathForBbkQuestion(i) ;				if (string("") != sCheminBBFilsItem)        {					// on a ici un chemin non vide = une question à poser au blackboard
					NSPatPathoArray *pPatPathoLocal = (NSPatPathoArray *) 0 ;

      		string sAnswerDate ;
					AnswerStatus::ANSWERSTATUS res = pContexte->getBBinterface()->getAnswer2Question(sCheminBBFilsItem, sArchetype, &pPatPathoLocal, sAnswerDate, bFirstCompute, true /* bUserIsWaiting */, HWindow) ;
					bFirstCompute = false ;
          if ((res == AnswerStatus::astatusProcessed) && (pPatPathoLocal != NULL) && (!(pPatPathoLocal->empty())))
					{
						// il y a une réponse du blackboard ==> on transmet la patpatho au
        		// BBFilsItem via le pTransfert et on met à jour le contrôle
            //
        		initControlFromBbkAnswer(*i, pPatPathoLocal, &sAnswerDate) ;
          }
          else if (res == AnswerStatus::astatusProcessing)
      			_bBlackboardStillAtWork = true ;
          if (pPatPathoLocal)
      			delete pPatPathoLocal ;
        }
      }
    }

    if (_bBlackboardStillAtWork)
    	waitForKS() ;
  }

  sErrMsg = pSuper->getText("bbkKsManagement", "displayingResults") ;
	pSuper->afficheStatusMessage((char*) sErrMsg.c_str()) ;

	// bool bFirstCompute = true ;

	for (i = iPostCalcul ; _aNSCtrls.end() != i ; i++)
	{		if ((*i)->getTransfert())		{			// on récupère le chemin du BBItem pere du BBFilsItem associé			// au NSControle via son pTransfert      //			sCheminBBFilsItem = getPathForBbkQuestion(i) ;			if (sCheminBBFilsItem == "")				continue ;

			// on a ici un chemin non vide = une question à poser au blackboard
			NSPatPathoArray * pPatPathoLocal = NULL ;

      string sAnswerDate ;
			AnswerStatus::ANSWERSTATUS res = pContexte->getBBinterface()->getAnswer2Question(sCheminBBFilsItem, sArchetype, &pPatPathoLocal, sAnswerDate, false /* bFirstCompute */, true /* bUserIsWaiting */, HWindow) ;
			// bFirstCompute = false ;
			if ((res == AnswerStatus::astatusProcessed) && (pPatPathoLocal != NULL) && (!(pPatPathoLocal->empty())))
			{
				// il y a une réponse du blackboard ==> on transmet la patpatho au
        // BBFilsItem via le pTransfert et on met à jour le contrôle

        initControlFromBbkAnswer(*i, pPatPathoLocal, &sAnswerDate) ;
      }
      else if (res == AnswerStatus::astatusProcessing)
      	_bBlackboardStillAtWork = true ;

      if (pPatPathoLocal)
      	delete pPatPathoLocal ;
		}
	}

/*
  while (bBlackboardStillAtWork)
	{
  	i = iPostCalcul ;
    bBlackboardStillAtWork = false ;

    for ( ; (i != pNSCtrl->end()) ; i++)
    {      if ((*i)->getTransfert())      {        // on récupère le chemin du BBItem pere du BBFilsItem associé        // au NSControle via son pTransfert        sCheminBBFilsItem = (*i)->getTransfert()->pBBFilsItem->pPere->sLocalisation ;        // on rajoute l'étiquette du fils        string sEtiq ;        string sEtiquette = (*i)->getTransfert()->pBBFilsItem->sEtiquette ;        pSuper->getDico()->donneCodeSens(&sEtiquette, &sEtiq) ;
        sCheminBBFilsItem += string(1, cheminSeparationMARK) + sEtiq ;

        // on enleve le chemin jusqu'à "0CALC1"
        size_t pos = sCheminBBFilsItem.find("/0CALC/") ;        if (pos == string::npos)          continue ;

        sCheminBBFilsItem = string(sCheminBBFilsItem, pos + 7, strlen(sCheminBBFilsItem.c_str()) - pos - 7) ;
        if (sCheminBBFilsItem == "")
          continue ;

        // on a ici un chemin non vide = une question à poser au blackboard
        NSPatPathoArray * pPatPathoLocal = NULL ;

        string sAnswerDate ;
        AnswerStatus::ANSWERSTATUS res = pSuper->getBBinterface()->getAnswer2Question(sCheminBBFilsItem, sArchetype, &pPatPathoLocal, sAnswerDate, false, true  bUserIsWaiting , HWindow) ;

        if ((res == AnswerStatus::astatusProcessed) && (pPatPathoLocal != NULL) && (!(pPatPathoLocal->empty())))
          initControlFromBbkAnswer(*i, pPatPathoLocal, &sAnswerDate) ;
        else if (res == AnswerStatus::astatusProcessing)
          bBlackboardStillAtWork = true ;

        if (pPatPathoLocal)
          delete pPatPathoLocal ;
      }
      pSuper->getApplication()->PumpWaitingMessages() ;
    }
	}
*/

  SetCursor(0, IDC_ARROW) ;

  sErrMsg = pSuper->getText("bbkKsManagement", "resultsDisplayed") ;
	pSuper->afficheStatusMessage((char*) sErrMsg.c_str()) ;

/*
	// On stoppe le KS après qu'il a effectué les calculs
  if (sKsName != "")
  	pSuper->getBBinterface()->driveKSfromDialog(sKsName, BB1BBInterface::ksHold) ;
*/

	// on lance ici le référentiel
	// le parseur appartient toujours au BBItem lanceur (cf NSBBSmall::lanceBbkArchetype)
	// string sNomRef = pBBItem->pParseur->pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_REF);
	// if (sNomRef != "")
	//     pSuper->BbkShowReferential(sNomRef, pContexte);
}
catch (...)
{
	erreur("Exception NSDialog::CmBbkCalcul().", standardError, 0) ;
}
}

string
NSDialog::getPathForBbkQuestion(iterNSControle iterCtrl)
{
	if ((NULL == iterCtrl) || (NULL == *iterCtrl) || (NULL == (*iterCtrl)->getTransfert()))
		return string("") ;

	// NSSuper* pSuper = pContexte->getSuperviseur() ;

	// on récupère le chemin du BBItem pere du BBFilsItem associé
	// au NSControle via son pTransfert  //  BBFilsItem* pFilsItem = (*iterCtrl)->getTransfert()->getFilsItem() ;  if ((NULL == pFilsItem) || (NULL == pFilsItem->getItemFather()))  	return string("") ;/*	string sCheminBBFilsItem = pFilsItem-><getItemFather()->sLocalisation ;	// on rajoute l'étiquette du fils  string sEtiq ;  string sEtiquette = pFilsItem->getItemLabel() ;	pSuper->getDico()->donneCodeSens(&sEtiquette, &sEtiq) ;
	sCheminBBFilsItem += string(1, cheminSeparationMARK) + sEtiq ;
*/

  string sCheminBBFilsItem = pFilsItem->getLocalisation() ;

	// on enleve le chemin jusqu'à "0CALC1"
	size_t pos = sCheminBBFilsItem.find("/0CALC/") ;	if (pos == string::npos)  	return sCheminBBFilsItem ;

	sCheminBBFilsItem = string(sCheminBBFilsItem, pos + 7, strlen(sCheminBBFilsItem.c_str()) - pos - 7) ;

  return sCheminBBFilsItem ;
}

void
NSDialog::waitForKS()
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	NVLdVTemps tpsBBKStart ;
	tpsBBKStart.takeTime() ;

	int iMaxTimeBeforeStop = 90 ;

	while (_bBlackboardStillAtWork)
	{
		pSuper->getApplication()->PumpWaitingMessages() ;

		NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;
    NVLdVTemps tpsCompare(tpsBBKStart) ;
    tpsCompare.ajouteSecondes(iMaxTimeBeforeStop) ;
    if (tpsCompare < tpsNow)
    {
    	string sWarningMessage = pSuper->getText("bbkKsManagement", "thisKsSeemsHanged") ;
      string sMsg = pSuper->getText("bbkKsManagement", "doYouWantToResetIt") ;
      sWarningMessage += string(1, '\n') + sMsg ;
      int rep = MessageBox(sWarningMessage.c_str(), "", MB_YESNO) ;
      if (rep == IDYES)
      	_bBlackboardStillAtWork = false ;
      else
      	tpsBBKStart.takeTime() ;
    }
  }
}

void
NSDialog::CmBbkEvent()
{
	_bBlackboardStillAtWork = false ;
}

void
NSDialog::insertLeavesOnBlackBoard(string sCheminPere, NSPatPathoArray* pPatPathoArray)
{
	if ((NULL == pPatPathoArray) || pPatPathoArray->empty())
		return ;

  if (pContexte->getBBinterface())
    pContexte->getBBinterface()->insertLeavesOnBlackBoard(sCheminPere, pPatPathoArray) ;

/*
	NSSuper *pSuper = pContexte->getSuperviseur() ;

  PatPathoIter iterSuiv ;

	for (PatPathoIter i = pPatPathoArray->begin(); i != pPatPathoArray->end(); i++)
  {
  	iterSuiv = i ;
    iterSuiv++ ;

    if ((iterSuiv == pPatPathoArray->end()) || ((*iterSuiv)->getColonne() <= (*i)->getColonne()))
    {
    	// on est sûr ici que iterElement est une feuille
      NSPatPathoArray* pPatPatho = new NSPatPathoArray(pContexte, graphPerson) ;
      pPatPatho->ajoutePatho(i, 0, 0) ;
      string sCheminLex = pPatPathoArray->donneCheminItem(i) ;
      if ((*i)->getUnit() != "")
      	sCheminLex += string(1, cheminSeparationMARK) + (*i)->getUnit() ;
      sCheminLex += string(1, cheminSeparationMARK) + (*i)->getLexique() ;
      string sCheminFils = "" ;
      string sElemLex, sCodeSens ;
      // on enlève d'abord la racine
      size_t pos = sCheminLex.find(string(1, cheminSeparationMARK)) ;
      if (pos != string::npos)
      	sCheminLex = string(sCheminLex, pos+1, strlen(sCheminLex.c_str())-pos-1) ;
      else
      	sCheminLex = "" ;

      while (sCheminLex != "")
      {
      	pos = sCheminLex.find(string(1, cheminSeparationMARK)) ;
        if (pos != string::npos)
        {
        	sElemLex = string(sCheminLex, 0, pos) ;
          pSuper->getDico()->donneCodeSens(&sElemLex, &sCodeSens) ;
          sCheminFils += string(1, cheminSeparationMARK) + sCodeSens ;
          sCheminLex = string(sCheminLex, pos+1, strlen(sCheminLex.c_str())-pos-1) ;
        }
        else
        {
        	sElemLex = sCheminLex ;
          pSuper->getDico()->donneCodeSens(&sElemLex, &sCodeSens) ;
          sCheminFils += string(1, cheminSeparationMARK) + sCodeSens ;
          sCheminLex = "" ;
        }
      }

      if (sCheminFils != "")
      	sCheminFils = sCheminPere + sCheminFils ;
      else
      	sCheminFils = sCheminPere ;

      sCheminFils = getRegularPath(sCheminFils, cheminSeparationMARK, intranodeSeparationMARK) ;

			pSuper->getBBinterface()->insertAnswerOnBlackboard(sCheminFils, pPatPatho, Undefined) ;
		}
	}
*/
}
//-------------------------------------------------------------------------
//  Function: 		void NSDialog::CmOkMessage(int iMessage)
//
//  Arguments:		AUCUN
//  Description:	Equivalent à OK, mais avec orientation de sortie.
//  Returns:		RIEN
//---------------------------------------------------------------------------
void NSDialog::CmOkMessage(int iMessage)
{
	executePreClosingFunctions() ;

	_pBBItem->donneRetourDlg(iMessage) ;
	if (false == _pBBItem->okFermerDialogue(true))
		return ;

	//
	// S'il s'agit de NSTreeWindow tuer pEDitDico
	//
  closeEditDico() ;

  //
  // Eventuel lancement de fonction
  //
  executeClosingFunctions() ;

	if (CanClose())
  {
    /*
    if (pView)
    {
        NSSuper* pSuper = pBBItem->pBigBoss->pContexte->getSuperviseur();
        NSToDoTask* pTask = new NSToDoTask ;
        pTask->sWhatToDo = "FermeDPIO" ;
        pTask->pPointer1 = (void*)pView ;

        pSuper->aToDo.push_back(pTask) ;

        pSuper->pNSApplication->GetMainWindow()->PostMessage(WM_COMMAND, IDM_TODO) ;
    }
    */
		CloseWindow(IDOK) ;	}
}
void
NSDialog::CloseWindow(int retVal)
{
/*
	if (NULL != pBBItem)
	{
  	NSSuper* pSuper = pBBItem->pBigBoss->pContexte->getSuperviseur() ;

		// On libère le KS afin qu'il puisse effectuer les calculs
  	string sKsName = pBBItem->KsInterface.getKsName() ;
  	if (sKsName != "")
  		pSuper->getBBinterface()->driveKSfromDialog(sKsName, true) ;
	}
*/
	TDialog::CloseWindow(retVal) ;

  if (pContexte->getBBinterface())
  {
    char szHandle[20] ;
    sprintf(szHandle, "%p", HWindow) ;
    string ps = string("NSDialog::CloseWindow disconnecting interface ") + string(szHandle) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    pContexte->getBBinterface()->DisconnectInterface(HWindow) ;
  }
}

void
NSDialog::closeEditDico()
{
	//
	// S'il s'agit de NSTreeWindow tuer pEDitDico
	//
	if (_aNSCtrls.empty())
		return ;

	for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
	{
		if ((*i)->getType() == isTreeWindow)
		{
    	NSTreeWindow* pNSTreeWindow = static_cast<NSTreeWindow*>((*i)->getControle()) ;
      if (pNSTreeWindow && pNSTreeWindow->getEditDico())
      {
        NSEditDico* pEditDico = pNSTreeWindow->getEditDico() ;
        if (pEditDico)
        {
      	  delete pEditDico ;
          pNSTreeWindow->setEditDico((NSEditDico*) 0) ;
        }
      }
    }
  }
}

void
NSDialog::executeClosingFunctions()
{
	if (_aNSCtrls.empty())
		return ;

	for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
		if ((*i)->getFonction())
    	(*i)->getFonction()->execute(NSDLGFCT_FERMETURE) ;
}

void
NSDialog::executePreClosingFunctions()
{
	if (_aNSCtrls.empty())
		return ;

	for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)
		if ((*i)->getFonction())
    	(*i)->getFonction()->execute(NSDLGFCT_PREFERME) ;
}

//------------------------------------------------------------------------// Cette fenêtre doit fermer ses fenêtres filles et petites filles
// si elles sont ouvertes avant de se refermer elle même
//------------------------------------------------------------------------
void
NSDialog::CmOkFermer()
{
	if (false == _pBBItem->_aBBItemFils.empty())
  {
  	for (BBiter it = _pBBItem->_aBBItemFils.begin() ;
                               _pBBItem->_aBBItemFils.end() != it ; it++)
    {    	if (false == (*it)->VectorFils.empty())
      {
      	for (BBiterFils jt = (*it)->VectorFils.begin() ;
                                      (*it)->VectorFils.end() != jt ; jt++)
        	if ((*jt)->getDialog())
          	(*jt)->getDialog()->CmOkFermer() ;
      }
    }
  }
  CmOk() ;
}

//------------------------------------------------------------------------//cette fenêtre doit fermer ses fenêtres filles et petites filles
//si elles sont ouvertes avant de se refermer elle même
//------------------------------------------------------------------------
void
NSDialog::CmCancelFermer()
{
	if (false == _pBBItem->_aBBItemFils.empty())
  {
  	for (BBiter it = _pBBItem->_aBBItemFils.begin() ;
                               _pBBItem->_aBBItemFils.end() != it ; it++)
    {
    	if (false == (*it)->VectorFils.empty())
      {
      	for (BBiterFils jt = (*it)->VectorFils.begin() ;
                                        (*it)->VectorFils.end() != jt ; jt++)
        	if ((*jt)->getDialog())
          	(*jt)->CmCancelFermer((*jt)->getDialog()) ;
      }
    }
  }
  _pBBItem->CmCancelFermer(this) ;
}

//-------------------------------------------------------------------------//  Function: void NSDialog::CmCancel()
//
//  Description: Répond à IDCancel.
//               Consulte BigBrother.
//---------------------------------------------------------------------------
void
NSDialog::CmCancel()
{
	if (_bBlackboardStillAtWork)
		_bBlackboardStillAtWork = false ;

	_pBBItem->donneRetourDlg(-1) ;
	_pBBItem->okFermerDialogue(false) ;
  _pBBItem->detacheControle() ;

	//
	// S'il s'agit de NSTreeWindow tuer pEDitDico
	//
  closeEditDico() ;

  if (pContexte->getBBinterface())
  {
    char szHandle[20] ;
    sprintf(szHandle, "%p", HWindow) ;
    string ps = string("NSDialog::CmCancel disconnecting interface ") + string(szHandle) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    pContexte->getBBinterface()->DisconnectInterface(HWindow) ;
  }

	//si dialogue non multiple, on ferme cette boite de dialogue, sinon c'est NsMultiDialog
	// qui s'en charge
	if ((NULL == _pBBItem->_pBBFilsPere) || (NULL == _pBBItem->_pBBFilsPere->pNsMultiDialog))
	{
		if (CanClose())
			Destroy(IDCANCEL) ;
	}

  if (_pControleurMulti)
  {
  	_bOkActivated = false ;
  	_pControleurMulti->idCancel(this, _iLastAction) ;
    _iLastAction = actionCancel ;
  }

	if (_pView)
	{
		NSSuper* pSuper = pContexte->getSuperviseur() ;
		NSToDoTask* pTask = new NSToDoTask ;
		pTask->setWhatToDo(string("FermeDPIO")) ;
		pTask->setPointer1((void*) _pView, false) ;

		pTask->sendMe(pSuper) ;
	}
}
voidNSDialog::EvSetFocus(THandle hWndLostFocus)
{
	//
  // On remet la fenetre de saisie dans le Lexique au premier plan
  // We bring back the Lexique window on top
  //
  if (_pBBItem && _pBBItem->_pBigBoss)
  {
    NSDico* pDico = pContexte->getDico() ;
    if (pDico)
    {
    	ChoixPathoDialog* pDicoDlg = pDico->getDicoDialog() ;      //      // ATTENTION :  si elle était cachée, le BringWindowToTop entraine      //              un comportement erratique      // WARNING :    if the window was hidden, BringWindowToTop can      //              make the system unstable      //      if (pDicoDlg && (pDicoDlg->EstOuvert()))
      	pDicoDlg->BringWindowToTop() ;
    }
  }
/*    if (pView)    {        NSMUEView* pMueView = TYPESAFE_DOWNCAST(pView, NSMUEView);        if (pMueView)
            pMueView->SetFocus() ;
        else
            pView->SetFocus() ;    }*/   /*   if (bInitFromBbk)        initControlesFromBbk("SetFocus") ;  */	activateParent() ;	TDialog::EvSetFocus(hWndLostFocus) ;}
void
NSDialog::EvWindowPosChanging(WINDOWPOS far& windowPos)
{
	if ((TWindowView*) NULL == _pView)
		return ;

  if (_bCanMove)
  	return ;

	if ((windowPos.x != 0) || (windowPos.y != 0) || (windowPos.cx != 0) || (windowPos.cy != 0))
  {
  	NS_CLASSLIB::TRect rectCli = Parent->GetClientRect() ;
    if ((windowPos.x < 0) || (windowPos.y < 0) ||
        (windowPos.x + windowPos.cx > rectCli.Width()) ||
        (windowPos.y + windowPos.cy > rectCli.Height()))
    	windowPos.flags = SWP_NOMOVE ;
  }
}

//
// Setting control's color
//
HBRUSH
NSDialog::EvCtlColor(HDC hdc, HWND hWndCtl, uint /* ctlType */)
{
  	//
	// S'il s'agit de NSTreeWindow tuer pEDitDico
	//
	if (_aNSCtrls.empty())
    return (HBRUSH)DefaultProcessing() ;

	for (iterNSControle iCtl = _aNSCtrls.begin() ; _aNSCtrls.end() != iCtl ; iCtl++)
	{
		if (((*iCtl)->getHWND() == hWndCtl) && ((*iCtl)->getType() == isEdit))
		{
      NSEdit* pNSEditWindow = static_cast<NSEdit*>((*iCtl)->getControle()) ;
      if (pNSEditWindow)
        return pNSEditWindow->getCtlColor(hdc) ;
    }
  }

  return (HBRUSH)DefaultProcessing() ;
}

void
NSDialog::EvTimer(uint id)
{
	// if      (id == ID_OB1_TIMER)
	//	checkPrecocheOnTimer() ;

	if (ID_OB1_TIMER2 == id)
		checkResultOnTimer() ;
}

void
NSDialog::CmBbkUpdate()
{
  checkPrecocheOnTimer() ;
}

void
NSDialog::checkPrecocheOnTimer()
{
	if (_aNSCtrls.empty())
		return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // L'archetype est par convention dans le champ fils du BBItem lanceur
  string sArchetype = _pBBItem->getItemSonsList() ;

  pSuper->afficheStatusMessage("Interrogation du Blackboard...") ;

  bool bSomeAnswerStillMissing = false ;

  for (iterNSControle i = _aNSCtrls.begin() ; _aNSCtrls.end() != i ; i++)  {  	if ((*i)->getTransfert() && ((*i)->isEmpty()) && ((*i)->initOnTimer()))
    {
    	string sCheminBBFilsItem ;
    	(*i)->getPathForBlackboard(&sCheminBBFilsItem) ;

      // on a ici un chemin non vide = une question à poser au blackboard
      NSPatPathoArray* pPatPathoArray = NULL ;

      string date_fils = string("") ;
      // string end_date  = string("") ;
      // (*i)->getEndValidityDate(&end_date) ;

      // on pose la question au blackboard
			int res = pContexte->getBBinterface()->precoche(sCheminBBFilsItem, &pPatPathoArray, HWindow, &date_fils) ;
			if ((res == 1) && pPatPathoArray && (false == pPatPathoArray->empty()))
        // il y a une réponse du blackboard ==>
        // on transmet la patpatho au BBFilsItem via le pTransfert
        // On met à jour le contrôle
        //
      	initControlFromBbkAnswer(*i, pPatPathoArray, &date_fils) ;
      else
      	bSomeAnswerStillMissing = true ;
    }
  }
  // if (!bSomeAnswerStillMissing)
  //	KillTimer(ID_OB1_TIMER) ;
}

void
NSDialog::checkResultOnTimer()
{
	
}

//-------------------------------------------------------------------------//  Function: LRESULT NSDialog::EvCorrect(WPARAM wParam, LPARAM lParam)
//
//  Arguments:
//	      wParam ->
//       lParam ->
//  Description:
//	      Le message WM_CORRECT est émis par les contrôles Edit
//            donc on veut vérifier la validité des saisies.
//            La responsabilité de la décision est laissée à la fenêtre
//            mère.
//  Returns:
//
//---------------------------------------------------------------------------
LRESULT NSDialog::EvCorrect(WPARAM /* wParam */, LPARAM /* lParam */)
{
  ::MessageBox(0, "Message WM_CORRECT", "Test", MB_OK);
  return NULL;
}

//-------------------------------------------------------------------------//  Function: void NSDialog::WM_Continue(WPARAM wParam, LPARAM lParam)
//
//  Arguments:
//        wParam ->
//        lParam ->
//  Description:
//	      Le message WM_CONTINUE est émis par les contrôles ou la
//            boîte de dialogue elle même.
//            Il doit générer une consultation de BigBrother qui donne la
//            marche à suivre.
//            Analyse le message dans lequel on doit trouver l'identité de
//            la fenêtre émettrice, et des informations complèmentaires
//            si nécessaire.
//            Consulte BigBrother.
//            Crée la fenêtre suivante.
//  Returns:
//
//---------------------------------------------------------------------------
LRESULT NSDialog::EvContinue(WPARAM /* wParam */, LPARAM /* lParam */)
{
	//::MessageBox(0, "Message WM_CONTINUE", "Configuration", MB_OK);
	return NULL;
}

/*voidNSDialog::EvMove(ClassLib::TPoint& clientOrigin)
{
}

voidNSDialog::EvWindowPosChanged(WINDOWPOS far& windowPos)
{

} */
static void count(TWindow* child, void* i)
{
  int* n = (int*)i;
  if (child) (*n)++;
}

int NSDialog::CountChild(){
	int i = 0;
	ForEach((TActionFunc)count, (void*)&i );
	return i;
}

void NSDialog::ConfigShow(){
	char p[255];
  int j;

// Affichage de la matrice d'initialisation// Analyse de la chaîne proposée
/*  strcpy(p, "");
  int j = pConfigCache->GetItemsInContainer();
  for(int i = 0; i < pConfigCache->GetItemsInContainer(); i++) {
	 CacheElement elt = (*pConfigCache)[i];
	 ChaineArray * list = elt.listItems;
	 strcat(p, (*list)[0].c_str());
	 strcat(p, "|");
	 strcat(p, (*list)[1].c_str());
	 strcat(p, "\n");
  }
  ::MessageBox(0, p, "Chaîne de configuration", MB_YESNO); */

// Matrice de transfert
/*  j =  pTransferBuf->GetItemsInContainer();
  itoa(j, p, 10);
  strcat(p, "buffers dans la matrice de transfert");
  ::MessageBox(0, p,"Buffer de transfert", MB_YESNO); */

  // Nombre d'objets OWL
  j = CountChild();
  itoa(j, p, 10);
  strcat(p,"enfants dans la ChildList");
  ::MessageBox(0, p, "Nombre d'enfants dans la ChildList", MB_YESNO);
}

void
NSDialog::GetWindowClass(WNDCLASS& wndClass)
{
    TResId dlgClass;

#ifndef _INCLUS
    #if defined(BI_COMP_BORLANDC)
    if (GetApplication()->BWCCEnabled()) {
        GetApplication()->GetBWCCModule()->Register(*GetModule());
        dlgClass = BORDLGCLASS;
    }
    else
    #endif
        dlgClass = WC_DIALOG;
#else
    #if defined(BI_COMP_BORLANDC)
    if (pContexte->getSuperviseur()->BWCCOn)
    {
    pContexte->getSuperviseur()->GetBWCCModule()->Register(*GetModule());
        dlgClass = BORDLGCLASS;
    }
    else
    #endif
        dlgClass = WC_DIALOG;
#endif

    if (!TUser::GetClassInfo(0, dlgClass, &wndClass))
        GetModule()->GetClassInfo(dlgClass, &wndClass);

    wndClass.lpszClassName = GetClassName();    wndClass.hInstance = *GetModule();   //!CQ Win32 only? (B50.26561)
}

//------------------------------------------------------------------------------//------------------------------------------------------------------------------bool
NSDialog::Create()
{
  _IsModalDialog = false ;

  if (this->IsWindow())
    return false ;

  OWL::TDialog::Create() ;

  return true ;
}
/******************************************************************************/
//
//  Wrappers pour TDialog
//
/******************************************************************************/

#ifdef _INCLUS
//// Registers the TWindow's MS-Windows, if not already registered
//
bool
NSDialog::Register()
{
  // Only check for globally registered classes if not under NT or WoW box,
  // since NT treats a 0 hInstance completely differently than Windows.
  //
#if defined(BI_PLAT_WIN32)
  static bool checkGlobal = !TSystem::IsNT();
#else
  static bool checkGlobal = !TSystem::IsWoW();
#endif

  WNDCLASS  windowClass;
  bool gc;
  if (checkGlobal)
    gc = TUser::GetClassInfo(0, GetClassName(), &windowClass) != 0;
  else
    gc = false;

  // If the wndclass is not yet registered, call GetWindowClass() to let our
  // derived class fill in the appropriate info. If the class is still then not
  // registered, then make it so.
  //
  if (!gc && !GetModule()->GetClassInfo(GetClassName(), &windowClass)) {
    GetWindowClass(windowClass);
    WNDCLASS dummy;
    if (!GetModule()->GetClassInfo(GetClassName(), &dummy))
      return ::RegisterClass(&windowClass);
  }

  return true;
}

char far*NSDialog::GetClassName()
{
#if defined(BI_COMP_BORLANDC)
  if (pContexte->getSuperviseur()->BWCCOn)
    return BORDLGCLASS;
  else
#endif
    return (char far*)WC_DIALOG;
}

static bool
NSDialogRegisterFails(TWindow* win, void*)
{
  return !win->Register();
}

void
NSDialog::RegisterChildObjects()
{
  if (FirstThat(NSDialogRegisterFails))
    TXWindow::Raise(this, IDS_CHILDREGISTERFAIL);
}

extern TDialog* _OWLDATA DlgCreationWindow;

int
NSDialog::Execute()
{
  PRECONDITION(GetHandle() == 0);

  IsModal = true;

  if (!Register())
    TXWindow::Raise(this, IDS_CLASSREGISTERFAIL);

  DlgCreationWindow = this;

  // Register all the dialog's child objects (for custom control support)
  //
  RegisterChildObjects();

  int retValue = DoExecute();
//  GetApplication()->ResumeThrow();

  // DoEcecute returns -1 if it could not create the dialog box
  //
  if (retValue == -1)
    TXWindow::Raise(this, IDS_WINDOWEXECUTEFAIL);

  return retValue;
}
#endif // _INCLUS

/*void NSMainWindow::WMContinue(RTMessage Msg)
{
  pNSSuperviseur->QueFaire();

  switch(pNSSuperviseur->Action()) {

	  case NSW_RIEN_FAIRE  : break;
	  case NSW_MODALE      : //
				 //************************************************
				 GetModule()->ExecDialog(pNSSuperviseur->pDlgEnCours());
				 PostMessage(this->HWindow, WM_CONTINUE, 0L, 0L);
				 break;
	  case NSW_NON_MODALE  : //
				 //************************************************
				 GetModule()->MakeWindow(pNSSuperviseur->pDlgEnCours());
				 PostMessage(this->HWindow, WM_CONTINUE, 0L, 0L);
				 break;
	  case NSW_FERMER      : ShutDownWindow();
						 break;
	  case NSW_TOUT_FERMER : break;
  }
}
*/

//---------------------------------------------------------------------------//  Function: NSBBFonction::NSBBFonction(string	sNomFonct, BBItem* pBBItemPere)
//
//  Arguments:		sNomFonct  	 -> Nom de la fonction
//                pBBItemPere  -> BBItem créateur de la fonction
//
//  Description:	Constructeur initié par un BBItem
//---------------------------------------------------------------------------
NSBBFonction::NSBBFonction(NSContexte* pCtx, string sNomFonct, BBItem* pBBItemPere)
             :NSRoot(pCtx)
{
	sNomFonction  = sNomFonct ;
	pBBItem 	    = pBBItemPere ;
	pBBPhraseItem = 0 ;

	iNbrCtrl = 0 ;
	for (int i = 0; i < 100; i++)
		pNSCtrl[i] = 0 ;
	//
	// Prise de l'adresse de la fonction NSBBFct dans la DLL de ressources
	// Cette fonction doit se trouver en 2ème position de la liste d'Export
	//

  if (NULL == pBBItem)
  	return ;

	pNSResModule = pBBItem->_pBigBoss->TrouverModule(pBBItem->getItemDialogFile()) ;
	if (pNSResModule)
		(FARPROC) pAdresseFct = pNSResModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
}

//---------------------------------------------------------------------------//  Function: NSBBFonction::NSBBFonction(string	sNomFonct, BBPhraseItem* pBBItemPere)
//
//  Arguments:		sNomFonct  	 -> Nom de la fonction
//                pBBItemPere  -> BBItem créateur de la fonction
//
//  Description:	Constructeur initié par un BBPhraseItem
//---------------------------------------------------------------------------
NSBBFonction::NSBBFonction(NSContexte* pCtx, string sNomFonct, BBPhraseItem* pBBItemPere)
             :NSRoot(pCtx)
{
	sNomFonction  = sNomFonct ;

	pBBItem 		  = 0 ;
	pBBPhraseItem = pBBItemPere ;

	iNbrCtrl = 0 ;
	for (int i = 0; i < 100; i++)
		pNSCtrl[i] = 0 ;

	//
	// Prise de l'adresse de la fonction NSBBFct dans la DLL de ressources
	// Cette fonction doit se trouver en 2ème position de la liste d'Export
	//
	(FARPROC) pAdresseFct = pNSResModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
}

//---------------------------------------------------------------------------
//  Function:    NSBBFonction::~NSBBFonction()
//
//  Description: Destructeur
//---------------------------------------------------------------------------
NSBBFonction::~NSBBFonction()
{
	for (int i = 0; i < iNbrCtrl; i++)
  	if (pNSCtrl[i])
			delete pNSCtrl[i] ;
}

//---------------------------------------------------------------------------
//  Function: NSBBFonction::referenceTransfert()
//
//  Arguments:		Aucun
//
//  Description:
//---------------------------------------------------------------------------
int
NSBBFonction::referenceTransfert()
{
	return 0 ;
}

//---------------------------------------------------------------------------
//  Function: NSBBFonction::execute(int iParam)
//
//  Arguments:	iParam 	  : paramètre qui précise l'action à effectuer
//									 (référencement, exécution de la fct...)
//					sEtiquette : position BB de l'appelant
//					pNSCtrl	  : pointeur sur un éventuel contrôle Windows
//
//  Description: 	Exécute la fonction
//---------------------------------------------------------------------------
bool
NSBBFonction::execute(int iParam, int* piValeur, string /* sEtiquette */, NSControle* /* pNSCtrl */)
{
	NSSmallBrother* pBigBrother ;

	if (pBBItem)
		pBigBrother = pBBItem->_pBigBoss ;
	else
		return false ;

	if (pAdresseFct)
		return ((*pAdresseFct)(this, pBigBrother, iParam, piValeur)) ;

	return false ;
}

//---------------------------------------------------------------------------//  Function: NSBBFonction::nouveauCtrl(char cGroupe, string sIdentite, string sDlgFct)
//
//  Description: 	Crée un objet NSControle
//---------------------------------------------------------------------------
int
NSBBFonction::nouveauCtrl(string sIdentite, string sDlgFct)
{
try
{
	if (pBBItem == 0)
		return 1 ;

	pNSCtrl[iNbrCtrl] = new NSControle(pContexte, pBBItem, sIdentite, sDlgFct) ;
	pNSCtrl[iNbrCtrl]->setType(isFunct) ;
	iNbrCtrl++ ;
	return 0 ;
}
catch (...)
{
	erreur("Exception NSBBFonction::nouveauCtrl.", standardError, 0) ;
	return 0 ;
}
}

int farajouteBBFonctionCtrl(NSBBFonction* pBBFct, char sIdentite, string sDlgFct)
{
	return pBBFct->nouveauCtrl(string(1,sIdentite), sDlgFct) ;
}

// --------------------------------------------------------------------------
// ---------------------- Méthodes de NSDialogCtrl --------------------------
// --------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSDialogCtrl::NSDialogCtrl()
{
	_sIdentite    = "" ;
	_sControle    = "" ;
	_sDlgFonction = "" ;
  _sHelpText    = "" ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDialogCtrl::NSDialogCtrl(const NSDialogCtrl& rv)
{
	_sIdentite    = rv._sIdentite ;
	_sControle    = rv._sControle ;
	_sDlgFonction = rv._sDlgFonction ;
  _sHelpText    = rv._sHelpText ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSDialogCtrl::~NSDialogCtrl()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSDialogCtrl&
NSDialogCtrl::operator=(const NSDialogCtrl& src)
{
	if (this == &src)
		return *this ;

	_sIdentite    = src._sIdentite ;
	_sControle    = src._sControle ;
	_sDlgFonction = src._sDlgFonction ;
  _sHelpText    = src._sHelpText ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur ==
//---------------------------------------------------------------------------
int
NSDialogCtrl::operator==(const NSDialogCtrl& o)
{
	if (_sIdentite == o._sIdentite)
		return 1 ;

  return 0 ;
}

// --------------------------------------------------------------------------
// ---------------------- Méthodes de NSDialogVector --------------------------
// --------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Description :	Constructeur copie
//  Retour :		Rien
//---------------------------------------------------------------------------
NSDialogVector::NSDialogVector(const NSDialogVector& rv)
               :NSDialogCtrlVector()
{
try
{
	if (false == rv.empty())
		for (constIterNSDialogCtrlVector it = rv.begin() ; rv.end() != it ; it++)
			push_back(new NSDialogCtrl(**it)) ;
}
catch (...)
{
	erreur("Exception NSDialogVector copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Description:	Destructeur
//---------------------------------------------------------------------------
void
NSDialogVector::vider()
{
	if (empty())
		return ;

	for (iterNSDialogCtrlVector i = begin() ; end() != i ; )
	{
		delete *i ;
    erase(i) ;
	}
}

NSDialogVector::~NSDialogVector()
{
	vider() ;
}

//---------------------------------------------------------------------------
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSDialogVector&
NSDialogVector::operator=(const NSDialogVector& src)
{
	if (this == &src)
		return *this ;

try
{
	vider() ;

	if (false == src.empty())
		for (constIterNSDialogCtrlVector it = src.begin() ; src.end() != it ; it++)
			push_back(new NSDialogCtrl(**it)) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSDialogVector (=).", standardError, 0) ;
	return *this ;
}
}

NSDialogCtrl*
NSDialogVector::getDialogCtrl(string sIdentite)
{
  if ((string("") == sIdentite) || empty())
    return (NSDialogCtrl*) 0 ;

  iterNSDialogCtrlVector it = begin() ;
  for ( ; (end() != it) && ((*it)->getIdentite() != sIdentite) ; it++) ;

  if (end() != it)
    return *it ;

  return (NSDialogCtrl*) 0 ;
}

// --------------------------------------------------------------------------// ---------------------- Méthodes de NSDialogEdit --------------------------
// --------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSDialogEdit::NSDialogEdit()
{
	szType[0] = '\0' ;
	nMaxInput = 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDialogEdit::NSDialogEdit(const NSDialogEdit& rv)
{
	strcpy(szType, rv.szType) ;
	nMaxInput = rv.nMaxInput ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSDialogEdit::~NSDialogEdit()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSDialogEdit&
NSDialogEdit::operator=(const NSDialogEdit& src)
{
	if (this == &src)
		return *this ;

	strcpy(szType, src.szType) ;
	nMaxInput = src.nMaxInput ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur ==
//---------------------------------------------------------------------------
int
NSDialogEdit::operator==(const NSDialogEdit& o)
{
	if ((strcmp(szType, o.szType) == 0) && (nMaxInput == o.nMaxInput))
		return 1 ;

	return 0 ;}

// --------------------------------------------------------------------------
// --------------------- Méthodes de NSDialogOnglet -------------------------
// --------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSDialogOnglet::NSDialogOnglet()
{
	nNbOnglets 	 = 0 ;
	sOngletActif = string("") ;
	for (int i = 0 ; i < 20 ; i++)
		pTexteOnglets[i] = (string*) 0 ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDialogOnglet::NSDialogOnglet(const NSDialogOnglet& rv)
{
try
{
	nNbOnglets 	 = rv.nNbOnglets ;
	sOngletActif = rv.sOngletActif ;

  for (int i = 0 ; i < 20 ; i++)
		pTexteOnglets[i] = (string*) 0 ;

 	for (int i = 0 ; i < nNbOnglets ; i++)
  	pTexteOnglets[i] = new string(*(rv.pTexteOnglets[i])) ;
}
catch (...)
{
	erreur("Exception NSDialogOnglet copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSDialogOnglet::~NSDialogOnglet()
{
	vider() ;
}

void
NSDialogOnglet::vider()
{
  for (int i = 0 ; i < nNbOnglets ; i++)
  	if (pTexteOnglets[i])
			delete pTexteOnglets[i] ;

  nNbOnglets = 0 ;
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSDialogOnglet&
NSDialogOnglet::operator=(const NSDialogOnglet& src)
{
	if (this == &src)
		return *this ;

try
{
	vider() ;

	nNbOnglets 	 = src.nNbOnglets ;
	sOngletActif = src.sOngletActif ;

 	for (int i = 0 ; i < nNbOnglets ; i++)
  	pTexteOnglets[i] = new string(*(src.pTexteOnglets[i])) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSDialogOnglet (=).", standardError, 0) ;
	return *this ;
}
}

//---------------------------------------------------------------------------//  Opérateur ==
//---------------------------------------------------------------------------
int
NSDialogOnglet::operator==(const NSDialogOnglet& o)
{
	if ((nNbOnglets != o.nNbOnglets) || (sOngletActif != o.sOngletActif))
  	return 0 ;

	for (int i = 0; i < nNbOnglets; i++)
  {
    if (((string*) NULL == pTexteOnglets[i]) ||
        ((string*) NULL == o.pTexteOnglets[i]))
      return 0 ;

  	if (*(pTexteOnglets[i]) != *(o.pTexteOnglets[i]))
    	return 0 ;
  }

	return 1 ;
}

void
NSDialogOnglet::addTabText(const string& sLabel)
{
  pTexteOnglets[nNbOnglets++] = new string(sLabel) ;
}

