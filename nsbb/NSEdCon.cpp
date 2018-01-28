#include <string.h>#include <cstring.h>

#ifdef _INCLUS
  #include "partage\Nsglobal.h"
  #include "nsbb\nsutidlg.h"
#endif

#include "nsbb\nstrnode.h"
#include "nsbb\nsedcon.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsednum.h"
#include "nsbb\nsbb.rh"
#include "nsbb\nslistwind.h"

/**********************************************************************/
//  permettre l'édition de champ numérique pour un NSTreeNode
/**********************************************************************/

#ifdef _INCLUS
DEFINE_RESPONSE_TABLE1(ChampEdition, NSDialogWrapper)
#else
DEFINE_RESPONSE_TABLE1(ChampEdition, TDialog)
#endif
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(IDC_VALSTOCK , Statique),
  EV_COMMAND(IDC_RECADYNAM, Dynamique),
END_RESPONSE_TABLE;

ChampEdition::ChampEdition(TWindow* parent, TResId ressource, int MaxEntree,
                           NSContexte* pCtx, NS_CLASSLIB::TRect rectEdit,
                           TModule* module, bool bCal)
             :TDialog(parent, ressource, module), NSRoot(pCtx)
{
	sValeurExacte = "" ;
	sValeurSup    = "" ;
	sValeurInf    = "" ;
	sUnite   	  = "" ;
	sTypeCalcul	  = "" ; //recalcul dynamique
	bCalcul		  = bCal ; //cacher  pStock et pDynamique si pas de calcul dans convert.db
	MaxInPut 	  = MaxEntree ;
	AParent 	  = parent ;
	rectEdition   = rectEdit ;
	//
	// Création de tous les "objets de contrôle"
	//
	pValeurExacte	= new NSHistoryEdit(this, IDC_EDITION, pContexte, "", "", "") ;
	pValeurSup 	  = new TEdit(this, IDC_SUP) ;
	pValeurInf 	  = new TEdit(this, IDC_INF) ;
	pUnite 		  	= new NSUtilLexique(pContexte, this, IDC_UNITE, pContexte->getDico()) ;
	pStock		  	= new TRadioButton(this, IDC_VALSTOCK) ; //valeur stockée
	pDynamique	  = new TRadioButton(this, IDC_RECADYNAM) ;// recalculée
}

ChampEdition::~ChampEdition()
{
	delete pValeurExacte ;
	delete pValeurInf ;
	delete pValeurSup ;
	delete pUnite;
	delete pStock;
	delete pDynamique;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void
ChampEdition::CmOk()
{
  char far* edition1 = new char[MaxInPut + 1];
  pValeurExacte->GetText(edition1, MaxInPut + 1);
  sValeurExacte = string(edition1);
  delete[] edition1;

  char far* edition2 = new char[MaxInPut + 1];
  pValeurSup->GetText(edition2, MaxInPut + 1);
  sValeurSup = string(edition2);
  delete[] edition2;

  char far* edition3 = new char[MaxInPut + 1];
  pValeurInf->GetText(edition3, MaxInPut + 1);
  sValeurInf = string(edition3);
  delete[] edition3;

  if((sValeurExacte != "") && ((sValeurSup != "") || (sValeurInf != "")))
  {
  	erreur("Il faut choisir la valeur exacte ou l'intervalle", warningError, 0) ;
    pValeurExacte->SetFocus() ;
    return ;
  }

  if (pUnite->sCode == "£?????")
  {
  	pUnite->sCode = "" ;
    pUnite->sContenuTransfert = "" ;
  }
  sUnite = pUnite->sContenuTransfert ;

	if (pStock->GetCheck() == BF_CHECKED)
  	sTypeCalcul = "STOCK" ;
  else if (pDynamique->GetCheck() == BF_CHECKED)
  	sTypeCalcul = "DYNAMIQUE" ;

	CloseWindow(IDOK) ;
}

string
ChampEdition::getUnit()
{
	if (pUnite->sCode == "£?????")
		return string("") ;

	return pUnite->sCode ;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
ChampEdition::Statique()
{
	if (!bCalcul)
  	pStock->SetCheck(BF_UNCHECKED) ;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
ChampEdition::Dynamique()
{
	if (!bCalcul)
  	pDynamique->SetCheck(BF_UNCHECKED) ;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
ChampEdition::CmCancel()
{
	Destroy(IDCANCEL);
}

void
ChampEdition::SetupWindow()
{
	TDialog::SetupWindow() ;

	pUnite->SetText(sUnite.c_str()) ;

  pValeurExacte->SetText(sValeurExacte.c_str()) ;
  pValeurExacte->sItemValue = sEtiquette ;

  pValeurSup->SetText(sValeurSup.c_str()) ;
  pValeurInf->SetText(sValeurInf.c_str()) ;
  pUnite->sContenuTransfert = sUnite ;

  if(bCalcul)
  {
  	if (sTypeCalcul == "DYNAMIQUE") //dynamique
    	pDynamique->SetCheck(BF_CHECKED);
    else
    	pStock->SetCheck(BF_CHECKED);
	}

 	//
  //coordonnées de l'écran en pixels
  //
  TIC* pTic = new TIC("DISPLAY",0,0);
  int XScreen = pTic->GetDeviceCaps(HORZRES);
  int YScreen = pTic->GetDeviceCaps(VERTRES);
  delete pTic;

  // rect.left= rectEdit->left (Edit) par rapport à la boîte de dialogue
  // rectPere.left (la boîte de dialogue ) par rapport à l'écran
  // pTree->Attr.X

	//position du lexique
  NS_CLASSLIB::TRect rectBoiteMere = Parent->GetWindowRect(); //boîte mère
  NS_CLASSLIB::TRect rectChamp   = GetWindowRect();

  int x = rectEdition.left + rectBoiteMere.left + 50 ;
  int y = rectEdition.top ;

  if ((x + rectChamp.Width()) >= XScreen)
      x = rectEdition.left  - rectChamp.Width() - 5;

  if ((y + rectChamp.Height()) >= YScreen)
      y = YScreen - rectChamp.bottom + rectChamp.top ;

  SetWindowPos(0,x,y,rectChamp.Width(),rectChamp.Height(),SWP_NOZORDER) ;
}

void
ChampEdition::ValeurExacte()
{}

voidChampEdition::ValeurSuperieure()
{}

voidChampEdition::ValeurInferieure()
{}

void
ChampEdition::setUnit(string sNewUnit)
{
	string sLang = pContexte->getUserLanguage() ;

	if (strlen(sNewUnit.c_str()) == (BASE_LEXI_LEN - 1))
  	sNewUnit += "1";
    
  string sLabelUnite ;
	pContexte->getDico()->donneLibelle(sLang, &sNewUnit, &sLabelUnite) ;
  pUnite->sCode = sNewUnit ;
  sUnite = sLabelUnite ;

  pValeurExacte->sUnit = sNewUnit ;
}

//*******************************************************************//								TRAITEMENT DES DATES ET DES HEURES//*******************************************************************

DEFINE_RESPONSE_TABLE1(NSDate, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  //EV_WM_CLOSE,
END_RESPONSE_TABLE;

NSDate::NSDate(TWindow* parent, TResId ressource, NSContexte* pCtx, string sType,
                NS_CLASSLIB::TRect rectEdit, TModule* module)
       :TDialog(parent, ressource, module), NSRoot(pCtx)
{
	sValeur     = string("") ;
	AParent     = parent ;
	rectEdition = rectEdit ;
	bOkToClose  = false ;

	string sLang = pContexte->getUserLanguage() ;

	char szType[25] ;
	//
	// Création de tous les "objets de contrôle"
	//
	if (sType == "DATE") //date
	{
  	pDate  = new NSEditDate(pContexte, this, IDC_DATE, szType, sLang) ;
    pHeure = 0 ;
  }
  else if (sType == "HEURE") //heure
  {
  	pHeure = new NSEditHeure(pContexte, this, IDC_DATE, szType, sLang) ;
    pDate = 0 ;
  }
}

NSDate::~NSDate()
{
	if (pDate)
		delete pDate ;
	if (pHeure)
		delete pHeure ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void
NSDate::CmOk()
{
	if (pDate)
	{
    bool bCanLetItBe ;
    pDate->actionForKillFocus(bCanLetItBe) ;
  	pDate->donneTransfert() ;
    sValeur = pDate->getContenuTransfert() ;
	}
  else
	{
  	pHeure->donneTransfert() ;
    sValeur = pHeure->getContenuTransfert() ;
	}

	bOkToClose = true ;
	CloseWindow(IDOK) ;
}

void
NSDate::Nettoyer()
{
  sValeur = string("") ;
  if (pDate)
  {
    delete pDate ;
    pDate = 0 ;
  }
  if (pHeure)
  {
    delete pHeure ;
    pHeure = 0 ;
  }
  bOkToClose = false ;
}

void
NSDate::Initialise(string sUnite)
{
	string sLang = pContexte->getUserLanguage() ;

	char szType[25] ;
	if (sUnite == "2DA01") //date
	{
		pDate = new NSEditDate(pContexte, this, IDC_DATE, szType, sLang) ;
    pHeure = 0 ;
	}
	else if(sUnite == "2HE01") //heure
	{
		pHeure = new NSEditHeure(pContexte, this, IDC_DATE, szType, sLang) ;
		pDate = 0 ;
	}
}

//-------------------------------------------------------------------------//-------------------------------------------------------------------------
void
NSDate::CmCancel()
{
  bOkToClose = true ;
	Destroy(IDCANCEL) ;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
bool
NSDate::CanClose()
{
  if (false == bOkToClose)
    return false ;

  if (pDate)
  {
    pDate->donneTransfert() ;
    sValeur = pDate->getContenuTransfert() ;
  }
  else
  {
    pHeure->donneTransfert() ;
    sValeur = pHeure->getContenuTransfert() ;
  }
  return true ;
}

void
NSDate::SetupWindow()
{
  TDialog::SetupWindow() ;

  if (pDate)  {
    pDate->Clear() ;
    // pDate->SetText(sValeur.c_str()) ;
    pDate->setContenuTransfert(sValeur) ;
    pDate->donneBrut() ;
    pDate->SetText(pDate->getContenuBrut().c_str()) ;
    // if (pDate->sContenuBrut == "00/00/1900")
    // pDate->Insert(pDate->sContenuBrut.c_str()) ;
  }
  else
  {
    pHeure->Clear() ;
    pHeure->SetText(sValeur.c_str()) ;
    pHeure->setContenuTransfert(sValeur) ;
    pHeure->donneBrut() ;
    if (pHeure->getContenuBrut() == string("00:00"))
      pHeure->Insert(pHeure->getContenuBrut().c_str()) ;
  }

  //
  //coordonnées de l'écran en pixels  //
  TIC* pTic = new TIC("DISPLAY",0,0) ;
  int XScreen = pTic->GetDeviceCaps(HORZRES) ;
  int YScreen = pTic->GetDeviceCaps(VERTRES) ;
  delete pTic ;

  // rect.left= rectEdit->left (Edit) par rapport à la boîte de dialogue
  // rectPere.left (la boîte de dialogue ) par rapport à l'écran
  // pTree->Attr.X

	//position du lexique
  NS_CLASSLIB::TRect rectBoiteMere = Parent->GetWindowRect(); //boîte mère
  NS_CLASSLIB::TRect rectChamp = GetWindowRect();

  int x = rectEdition.left + rectBoiteMere.left + 50 ;
  int y = rectEdition.top ;

  if ((x + rectChamp.Width()) >= XScreen)
    x = rectEdition.left  - rectChamp.Width() - 5 ;

  if ((y + rectChamp.Height()) >= YScreen)
    y = YScreen - rectChamp.bottom + rectChamp.top ;

  SetWindowPos(0, x, y, rectChamp.Width(), rectChamp.Height(), SWP_NOZORDER) ;
}

/**********************************************************************/
//  position drag and drop et copie
/**********************************************************************/

DEFINE_RESPONSE_TABLE1(PositionDragAndDrop, TDialog)
  EV_COMMAND(IDC_AVANT, Avant),
  EV_COMMAND(IDC_APRES, Apres),
  EV_COMMAND(IDC_FILS,  Fils),
  EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

PositionDragAndDrop::PositionDragAndDrop(TWindow* parent, TResId ressource,
                                          NSContexte* pCtx, TModule* module)
				    :TDialog(parent, ressource, module), NSRoot(pCtx)
{
	//
	// Création de tous les "objets de contrôle"
	//
	pAvant = new TRadioButton(this, IDC_AVANT) ;
	pApres = new TRadioButton(this, IDC_APRES) ;
	pFils  = new TRadioButton(this, IDC_FILS) ;

  iDragType = dragNotSpecified ;
}

PositionDragAndDrop::~PositionDragAndDrop()
{
	delete pAvant ;
	delete pApres ;
	delete pFils ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
PositionDragAndDrop::Avant()
{
  iDragType = dragBefore ;
	CloseWindow(IDOK) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
PositionDragAndDrop::Apres()
{
  iDragType = dragAfter ;
	CloseWindow(IDOK) ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
PositionDragAndDrop::Fils()
{
  iDragType = dragAsSon ;
	CloseWindow(IDOK) ;
}

void
PositionDragAndDrop::SetupWindow()
{
	TDialog::SetupWindow() ;
	pAvant->SetCheck(BF_UNCHECKED) ;
	pApres->SetCheck(BF_UNCHECKED) ;
	pFils->SetCheck(BF_UNCHECKED) ;
}

//----------------------------------------------------------------------------
void
PositionDragAndDrop::CmOk()
{
	CloseWindow(IDOK) ;
}

//----------------------------------------------------------------------------
//                                NSHistoryEdit
//----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSHistoryEdit, TEdit)
    EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

NSHistoryEdit::NSHistoryEdit(ChampEdition* parent, int resId, NSContexte* pCtx, string sValue, string sValueUnit, string sValuePath)
              :TEdit(parent, resId, 0), NSRoot(pCtx)
{
	pPere        = parent ;

	sItemValue   = sValue ;
	sUnit        = sValueUnit ;
	sPath        = sValuePath ;
	bHistoryOpen = false ;
}

void
NSHistoryEdit::SetupWindow()
{
  TEdit::SetupWindow() ;
}

void
NSHistoryEdit::EvKeyDown(uint key, uint repeatcount, uint flags)
{
try
{
	if ((GetKeyState(VK_CONTROL) < 0) && ((key == 'H') || (key == 'h')) && (sItemValue != ""))
  {
  	bHistoryOpen = true ;

    sUnit = pPere->getUnit() ;

    string sResult = "" ;
    NSValListHistoryDialog* pDialogHistory = new NSValListHistoryDialog(this, pContexte, sItemValue, sPath, sUnit, &sResult) ;
    pDialogHistory->Execute() ;
    delete pDialogHistory ;

    bHistoryOpen = false ;

    if (sResult != "")
    	SetText(sResult.c_str()) ;
  }

	TEdit::EvKeyDown(key, repeatcount, flags) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSHistoryEdit::EvKeyDown : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
  return ;
}
catch (...)
{
	erreur("Exception NSHistoryEdit::EvKeyDown.", standardError, 0) ;
  return ;
}
}

//----------------------------------------------------------------------------
//                                   INCLUS
//----------------------------------------------------------------------------

#ifdef _INCLUS
void
ChampEdition::GetWindowClass(WNDCLASS& wndClass)
{
    TResId dlgClass;

    #if defined(BI_COMP_BORLANDC)
    if (pContexte->getSuperviseur()->BWCCOn) {
        pContexte->getSuperviseur()->GetBWCCModule()->Register(*GetModule());
        dlgClass = BORDLGCLASS;
    }
    else
    #endif
    dlgClass = WC_DIALOG;

    if (!TUser::GetClassInfo(0, dlgClass, &wndClass))
        GetModule()->GetClassInfo(dlgClass, &wndClass);

    wndClass.lpszClassName = GetClassName();
    wndClass.hInstance = *GetModule();   //!CQ Win32 only? (B50.26561)
}

//
// Registers the TWindow's MS-Windows, if not already registered
//
bool
ChampEdition::Register()
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
  string clsName = GetClassName();
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

  return true;}

char far*
ChampEdition::GetClassName()
{
#if defined(BI_COMP_BORLANDC)
  if (pContexte->getSuperviseur()->BWCCOn)
    return BORDLGCLASS;
  else
#endif
    return (char far*)WC_DIALOG;
}

static bool
NSRegisterFails(TWindow* win, void*)
{
/*  char clsName[31];
  win->GetClassName(clsName, 30);
  if( 0 == strcmp(clsName, "Group") )
    return false;
*/
  return !win->Register();
}

void
ChampEdition::RegisterChildObjects()
{
  if (FirstThat(NSRegisterFails))
    TXWindow::Raise(this, IDS_CHILDREGISTERFAIL);
}

extern TDialog* _OWLDATA DlgCreationWindow;

int
ChampEdition::Execute()
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

#endif   // _INCLUS

