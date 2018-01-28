#include <bwcc.h>
#include "partage\Nsglobal.h"

#include "nssavoir\nspathor.h"
#include "nsbb\nsutiDlg.h"#include "nsbb\nsmediccontrols.h"#include "nsbb\nsbb.rh"#include "nautilus\nssuper.h"
// -------------------------------------------------------------------------// -------------------- METHODES DE NSVectControl ------------------------
//    							vecteur de TControls
// -------------------------------------------------------------------------
NSVectControl::NSVectControl()
		          :NSVecteurControl()
{}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVectControl::NSVectControl(const NSVectControl& rv)
              :NSVecteurControl()
{
	if (false == rv.empty())
		for (NSControlConstIter it = rv.begin() ; rv.end() != it ; it++)
			push_back(new TControl(AParent, 0, ressource, 0, 0, 0, 0, module)) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSVectControl::vider()
{
	if (false == empty())
		for (NSControlIter i = begin() ; i != end() ; )
			erase(i) ;
}

NSVectControl::~NSVectControl(){
	vider() ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSVectControl&
NSVectControl::operator=(const NSVectControl& src)
{
	if (this == &src)
		return *this ;

	//
	// Effacement des éléments déjà contenus dans le vecteur destination
	//
	vider() ;

	//
	// Copie et insertion des éléments de la source
	//
  if (false == src.empty())
		for (NSControlConstIter i = src.begin() ; src.end() != i ; i++)
   		push_back(new TControl(AParent, 0, ressource, 0, 0, 0, 0, module)) ;

	return *this ;
}

//*********************************************************************// 								classe NSUtilDialog
//*********************************************************************

#ifdef _INCLUSDEFINE_RESPONSE_TABLE1(NSUtilDialog, NSDialogWrapper)
#else
DEFINE_RESPONSE_TABLE1(NSUtilDialog, TDialog)
#endif
    EV_WM_KEYDOWN,
    EV_COMMAND(IDOK,     CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_COMMAND(CM_HELP,  CmHelp),
    EV_COMMAND(IDHELP,   CmHelp),
    EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NSUtilDialog::NSUtilDialog(TWindow* parent, NSContexte* pCtx, TResId resId, TModule* module)
#ifdef _INCLUS
             :NSDialogWrapper(pCtx, parent, resId, module)
#else
             :TDialog(parent, resId, module), NSRoot(pCtx)
#endif
{
  pNSVectControl = new NSVectControl ;
  ressource      = resId ;
  AParent        = parent ;

  sHindex        = string("") ;
  sHcorps        = string("") ;

  hAccelTable    = LoadAccelerators(pCtx->getSuperviseur()->getApplication()->GetInstance(), MAKEINTRESOURCE(IDA_BB)) ;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NSUtilDialog::~NSUtilDialog()
{
	if (pNSVectControl)
	{
		delete pNSVectControl ;
		pNSVectControl = 0 ;
	}
}

void
NSUtilDialog::ReferenceControl(TControl* pTControl)
{
	pNSVectControl->push_back(pTControl) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSUtilDialog::NSUtilDialog(const NSUtilDialog& src)
#ifdef _INCLUS
             :NSDialogWrapper(src.pContexte, src.AParent, src.ressource)
#else
             :TDialog(src.AParent, src.ressource), NSRoot(src.pContexte)
#endif
{
  pNSVectControl = src.pNSVectControl ;
  AParent        = src.AParent ;
  ressource      = src.ressource ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSUtilDialog&
NSUtilDialog::operator=(const NSUtilDialog& src)
{
  if (this == &src)
		return *this ;

  pNSVectControl = src.pNSVectControl ;
  AParent        = src.AParent ;
  ressource      = src.ressource ;

	return *this ;
}

//
//
//
static bool isItThisChild2(TWindow* win, void* id)
{
  return (win->GetId() == *(int*)id) ;
}

static bool isItTheGoodId(HWND hWnd, LPARAM lParam)
{
	//
	// Adresse de l'objet boîte de dialogue courante
	//
	TWindow* pWnd = reinterpret_cast<TWindow*>(lParam) ;
	NSUtilDialog* pDlgCurrent = dynamic_cast<NSUtilDialog*>(pWnd) ;
	if (NULL == pDlgCurrent)
		return true ;

  //
	// Obtention de l'ID du contrôle et de son type WINDOWS
	//
	int ctrlID  = ::GetDlgCtrlID(hWnd);

  if (ctrlID == pDlgCurrent->iIdToFind)
  {
    pDlgCurrent->hWndResult = hWnd ;
    return false ;
  }

  return true ;
}

TWindow*
NSUtilDialog::ChildWithId(int id)
{
  // FirstThat only iterates among OWL objects
  TWindow* pOwlObject = ((TWindow*)this)->FirstThat(isItThisChild2, (void*)&id) ;
  if (pOwlObject)
    return pOwlObject ;

  hWndResult = 0 ;
  iIdToFind  = id ;

  WNDENUMPROC lpEnumDlgFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) isItTheGoodId, hInstance) ;
  EnumChildWindows(HWindow, lpEnumDlgFunc, LPARAM((TWindow *)this)) ;

  if (0 == hWndResult)
    return 0 ;

  TWindow* pNonOwlObject = new TWindow(hWndResult) ;
  return pNonOwlObject ;
}

//
//
//
static bool isItThisChildHwd(TWindow* win, void* hWnd)
{
  return (win->HWindow == *(HWND*)hWnd) ;
}

TWindow*
NSUtilDialog::ChildWithHandle(HWND hWnd)
{
  return ((TWindow*)this)->FirstThat(isItThisChildHwd, (void*)&hWnd) ;
}

/*void EnumerWindows(TWindow* pWinCurr, void* pWinds)
{
    TControl* pContrl = TYPESAFE_DOWNCAST(pWinCurr, TControl) ;

    if (!pContrl)
      return ;
    NSVectControl*  pVectControls = (NSVectControl *)pWinds ;
    NSControlIter  iter;
    if (!pVectControls->empty())
      for (iter = pVectControls->begin(); (iter != pVectControls->end() && (*iter) != pContrl); iter++);
    if ((pVectControls->empty())||(iter != pVectControls->end()))
      pVectControls->push_back(pContrl) ;

}   */

/*
   // Préparation de la fonction d'énumération
    lpEnumDlgFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) NSDialog::DlgConfigCtrl, hInstance) ;

    // On passe comme paramètre de EnumChildWindows LPARAM(this), l'adresse de
    // this puisque dans les fonctions static on ne peut pas récupérer le
    // pointeur this.
    EnumChildWindows(HWindow, lpEnumDlgFunc, LPARAM((TWindow *)this)) ;
*/

static void EnumerWindows(TWindow* wControl, void* lParam)
{
	TControl* pContrl = TYPESAFE_DOWNCAST(wControl, TControl) ;
  if (NULL == pContrl)
    return ;

  NSVectControl* pVectControls = (NSVectControl*)(lParam);
 // NSVectControl* pVectControls = reinterpret_cast<NSVectControl*>(lParam);

  NSControlIter iter ;
  if (false == pVectControls->empty())
    for (iter = pVectControls->begin() ; (pVectControls->end() != iter) && ((*iter) != pContrl); iter++) ;

  if (pVectControls->empty() || (pVectControls->end() == iter))
    pVectControls->push_back(pContrl) ;
}

void
NSUtilDialog::SearchControls()
{
   // lpEnumDlgFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) NSDialog::DlgConfigCtrl, hInstance) ;
   //WNDENUMPROC lpEnumDlgFunc = (WNDENUMPROC) MakeProcInstance((FARPROC)NSUtilDialog::EnumWindows, hInstance) ;

  // EnumChildWindows(this->GetHandle(), NSUtilDialog::EnumerWindows, LPARAM(pNSVectControl)) ;
  //EnumChildWindows(this->HWindow, NSUtilDialog::EnumerWindows, LPARAM(pNSVectControl)) ;
	ForEach((TActionFunc)EnumerWindows, (void*)pNSVectControl);
}

/*static void count(TWindow* child, void* i)
{
  int* n = (int*)i;
  if (child) (*n)++;
}
void
NSUtilDialog::SearchControls(){
 	 int i = 0;
 	 ForEach((TActionFunc)count, (void*)&i );
 	// return i;
}   */



/*int FAR PASCAL NSUtilDialog::EnumerWindows(HWND hWnd, LPARAM lParam)
{

  TWindow wControl(hWnd);
  // TWindow *wControl = new TWindow(hWnd);
  TControl* pContrl = TYPESAFE_DOWNCAST(&wControl, TControl) ;
  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(&wControl, NSUtilDialog) ;

  //delete  wControl;
  /*if (pNSUtilDialog)
    return 1;
  else
    if (!pContrl)
    {
       delete pNSUtilDialog;
      return 1;

    }

  NSVectControl* pVectControls = reinterpret_cast<NSVectControl*>(lParam);
  if (!pVectControls)
      return 0;
  NSControlIter  iter;
  if (!pVectControls->empty())
      for (iter = pVectControls->begin(); (iter != pVectControls->end() && (*iter) != pContrl); iter++);
  if ((pVectControls->empty())||(iter != pVectControls->end()))
      pVectControls->push_back(pContrl) ;
  delete  pContrl;
  return 1;
}   */


bool
NSUtilDialog::ValidActiveControl(TControl *pControl)
{
   char szClassName[30] = "" ;
   pControl->GetClassName(szClassName, 30);
  // string sClassName = string(szClassName);

   if ((!strcmp(szClassName, "Static"))||(!strcmp(szClassName, "ScrollBar"))
            || (!strcmp(szClassName, "msctls_updown32")))
    return false;

   if (!strcmp(szClassName, "Button"))
   {
      //on elimine les buttons inactifs WS_DISABLED
      uint32  styleButton = pControl->GetStyle();
      uint32  styleGris = styleButton|WS_DISABLED;
      if (styleButton == styleGris)
        return false;
      else
        return true;
   }
   //pour les controls inactifs
   /*uint32  styleControl = pControl->GetStyle();
   uint32  styleHide = styleControl|SW_HIDE;
   if (styleControl == styleHide)
      return false;
   else
      return true;   */

   return true;

}
bool
NSUtilDialog::SelectTextActiveControl(TControl* pTControle)
{
  char szClassName[30] = "" ;
  pTControle->GetClassName(szClassName, 30) ;
  if (!strcmp(szClassName, "Edit"))
  {
    TEdit* pEdit = (TEdit*)pTControle ;
    if (!pEdit)
      return false ;

    pEdit->SetSelection(0, pEdit->GetTextLen()) ;
  }
  return true ;
}

//------------------------------------------------------------------------
//demander à la boîte de dialogue mère d'activer le controle
//suivant pControle sinon le premier
//------------------------------------------------------------------------
void
NSUtilDialog::ActiveControlSuivant(TControl* pTControle)
{
	pNSVectControl->vider() ;
	SearchControls() ;

	if (pNSVectControl->empty())
		return ;

  NSControlIter i = pNSVectControl->begin() ;
	for ( ; (pNSVectControl->end() != i) && (*i != pTControle); i++) ;
	if (pNSVectControl->end() != i)
		i++ ;  //suivant

	while ((pNSVectControl->end() != i) && (!ValidActiveControl(*i)))
		i++ ;

	if (pNSVectControl->end() != i)
	{
		(*i)->SetFocus() ;
    SelectTextActiveControl(*i) ;
	}
   /* char far* szClassName = "";
    (*i)->GetClassName(szClassName, 30);
    if(!strcmp(szClassName, "Edit"))
    {
      TControl* pContrl = TYPESAFE_DOWNCAST(&wControl, TControl) ;
      (*i)->SetSelection(0, (*i)->GetTextLen()) ;
    } */
	else //donner le focus au bouton Ok s'il existe sinon fermer la boîte
	{
  	if (ChildWithId(IDOK))
    	ChildWithId(IDOK)->SetFocus() ;
    else
    	CmOk() ;
	}

    //	(*(pNSVectControl->begin()))->SetFocus();
}

voidNSUtilDialog::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TDialog::EvKeyDown(key, repeatCount, flags) ;
}

bool
NSUtilDialog::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  if (NULL == hAccelTable)
    return false ;

  bool bTranslated = ::TranslateAccelerator(GetHandle(), hAccelTable, &msg) ;
  if (bTranslated)
    return true ;

  return false ;
}

void
NSUtilDialog::CmOk()
{
  // On vérifie qu'un edit n'était pas en cours de saisie
  // Le Return n'envoie pas de KillFocus et appelle directement CmOk
  //
  killControlFocusOnReturnClose() ;

	TDialog::CmOk() ;
}

voidNSUtilDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

void
NSUtilDialog::CmHelp()
{
  if (pContexte && pContexte->getSuperviseur())
  {
    NSSuper* pSuper = pContexte->getSuperviseur() ;

  	pSuper->setAideIndex(sHindex) ;
    pSuper->setAideCorps(sHcorps) ;
    pSuper->NavigationAideEnLigne() ;
  }
}

void
NSUtilDialog::EvSetFocus(HWND hWndLostFocus)
{
	//
	// On remet la fenetre de saisie dans le Lexique au premier plan
	// We bring back the Lexique window on top
	//
	NSDico* pDico = pContexte->getDico() ;
	if (NULL == pDico)
	{
		TDialog::EvSetFocus(hWndLostFocus) ;
		return ;
	}
	ChoixPathoDialog* pDicoDlg = pDico->getDicoDialog() ;
  //
  // ATTENTION :  si elle était cachée, le BringWindowToTop entraine  //              un comportement erratique  // WARNING :    if the window was hidden, BringWindowToTop can  //              make the system unstable  //
	if (pDicoDlg && (pDicoDlg->EstOuvert()))
		pDicoDlg->BringWindowToTop() ;

	TDialog::EvSetFocus(hWndLostFocus) ;
}

void
NSUtilDialog::setTextForControl(int iControlId, string sTextChapter, string sTextId)
{
	TWindow* pControl = ChildWithId(iControlId) ;
  if (NULL == pControl)
		return ;

	string sText = pContexte->getSuperviseur()->getText(sTextChapter, sTextId) ;

  pControl->SetCaption(sText.c_str()) ;
}

void
NSUtilDialog::killControlFocusOnReturnClose()
{
  // The goal, here, is to test if the focus is still in a control when
  // entering CmOk. This situation occurs when <Enter> key is pressed. In
  // this case, the control gets the killfocus message after CmOk is processed
  // and cannot update its information. We decide to call its killfocus method
  // here.

  THandle hFocusWnd = GetFocus() ;

  // If the focus is not inside the dialog, there is nothing to do
  //
  if (false == IsChild(hFocusWnd))
    return ;

  // If focus is already set to OK button, then there is nothing to do either
  //
  TWindow* pOkButton = ChildWithId(IDOK) ;
  if ((NULL == pOkButton) || (pOkButton->GetHandle() == hFocusWnd))
    return ;

  TWindow* pFocusedChild = ChildWithHandle(hFocusWnd) ;
  if (NULL == pFocusedChild)
    return ;

  NSUtilEdit* pEditChild = 0 ;

try
{
  pEditChild = dynamic_cast<NSUtilEdit*>(pFocusedChild) ;
  if (NULL == pEditChild)
    return ;
}
catch (...)
{
  return ;
}

try
{
  //
  //
  NSEditNum* pEditNumChild = dynamic_cast<NSEditNum*>(pEditChild) ;
  if (NULL != pEditNumChild)
  {
    // Doing this would start an infinite loop by calling CmOk
    // pEditNumChild->EvKillFocus(pOkButton->HWindow) ;
    pEditNumChild->EvKillFocus(0) ;
    return ;
  }
}
catch (...)
{
}
  pEditChild->EvKillFocus(0) ;
}

void
NSUtilDialog::GetWindowClass(WNDCLASS& wndClass)
{
  TResId dlgClass = WC_DIALOG ;

#if defined(BI_COMP_BORLANDC)

  // TApplication* pApplication = GetApplication() ;
  TApplication* pApplication = GetApplication() ;

  if (pApplication && (pApplication->BWCCEnabled()))
  {
    TModule* pBwccModule = pApplication->GetBWCCModule() ;
    if (pBwccModule && pBwccModule->GetHandle())
    {
      pApplication->GetBWCCModule()->Register(*GetModule()) ;
      dlgClass = BORDLGCLASS ;
    }
  }
#endif

  if (!TUser::GetClassInfo(0, dlgClass, &wndClass))
    GetModule()->GetClassInfo(dlgClass, &wndClass) ;

  wndClass.lpszClassName = GetClassName() ;  wndClass.hInstance = *GetModule() ;   //!CQ Win32 only? (B50.26561)
}

char far*
NSUtilDialog::GetClassName()
{
#if defined(BI_COMP_BORLANDC)
  return BORDLGCLASS ;
#else
  return (char far*) WC_DIALOG ;
#endif
}

TApplication*
NSUtilDialog::GetApplication()
{
  if ((NULL == pContexte) || (NULL == pContexte->getSuperviseur()))
    return (TApplication*) 0 ;

  return pContexte->getSuperviseur()->getApplication() ;
}

/******************************************************************************/
//
//
//  NSDialogWrapper
//
//
/******************************************************************************/

#ifdef _INCLUS   //*************************//

NSDialogWrapper::NSDialogWrapper(NSContexte* pCtx, TWindow* parent, TResId resId,            TModule* module)
            : TDialog(parent, resId, module), NSRoot(pCtx), pContexte(pCtx),
            pModule(module)
{
}

void
NSDialogWrapper::GetWindowClass(WNDCLASS& wndClass)
{
    TResId dlgClass;

    #if defined(BI_COMP_BORLANDC)
    if (pContexte->getSuperviseur()->BWCCOn) {
        //::MessageBox(0, "Appel de GetBWCCModule()->Register", "", MB_OK);
        if (pContexte->getSuperviseur()->GetBWCCModule())
            pContexte->getSuperviseur()->GetBWCCModule()->Register(*GetModule());
        else
            //::MessageBox(0, "BWCCModule est NULL", "", MB_OK);
        dlgClass = BORDLGCLASS;
    }
    else
    #endif
        dlgClass = WC_DIALOG;

    //::MessageBox(0, "Appel de GetClassInfo", "", MB_OK);
    if (!TUser::GetClassInfo(0, dlgClass, &wndClass))
        GetModule()->GetClassInfo(dlgClass, &wndClass);

    //::MessageBox(0, "Fin de GetWindowClass", "", MB_OK);
    wndClass.lpszClassName = GetClassName();
    wndClass.hInstance = *GetModule();   //!CQ Win32 only? (B50.26561)
}

//
// Registers the TWindow's MS-Windows, if not already registered
//
bool
NSDialogWrapper::Register()
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

  //::MessageBox(0, "Lancement de la procédure de registration", "", MB_OK);

  // If the wndclass is not yet registered, call GetWindowClass() to let our
  // derived class fill in the appropriate info. If the class is still then not
  // registered, then make it so.
  //
  if (!gc && !GetModule()->GetClassInfo(GetClassName(), &windowClass)) {
    //::MessageBox(0, "Appel de GetWindowClass", "", MB_OK);
    GetWindowClass(windowClass);
    WNDCLASS dummy;
    if (!GetModule()->GetClassInfo(GetClassName(), &dummy))
    {
      //::MessageBox(0, "Appel de ::RegisterClass", "", MB_OK);
      return ::RegisterClass(&windowClass);
    }
  }

  return true;
}

char far*
NSDialogWrapper::GetClassName()
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
NSDialogWrapper::RegisterChildObjects()
{
  if (FirstThat(NSRegisterFails))
    TXWindow::Raise(this, IDS_CHILDREGISTERFAIL);
}

extern TDialog* _OWLDATA DlgCreationWindow;

int
NSDialogWrapper::Execute()
{
  PRECONDITION(GetHandle() == 0);

  //::MessageBox(0, "Entrée dans NSDialogWrapper::Execute()", "", MB_OK);

  IsModal = true;

  if (!Register())
    TXWindow::Raise(this, IDS_CLASSREGISTERFAIL);

  DlgCreationWindow = this;

  //::MessageBox(0, "Avant RegisterChildObjects", "", MB_OK);
  // Register all the dialog's child objects (for custom control support)
  //
  RegisterChildObjects();

  //::MessageBox(0, "Avant DoExecute", "", MB_OK);

  int retValue = DoExecute();
//  GetApplication()->ResumeThrow();

  // DoEcecute returns -1 if it could not create the dialog box
  //
  if (retValue == -1)
    TXWindow::Raise(this, IDS_WINDOWEXECUTEFAIL);

  //::MessageBox(0, "Sortie de NSDialogWrapper::Execute()", "", MB_OK);

  return retValue;
}

#endif // _INCLUS

