// --------------------------------------------------------------------------// NSTEAMDOCVIEW.CPP		Document/Vues Health Team
// --------------------------------------------------------------------------
// PA - janvier 2007
// --------------------------------------------------------------------------

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsbb\nsPaneSplitter.h"
#include "nsbb\nslistwind.h"
#include "nsbb\tagNames.h"
#include "nsdn\nsdocnoy.h"
#include "nssavoir\GraphicHealthTeam.h"
#include "nssavoir\nsHealthTeamInterface.h"
#include "nsepisod\nsToDo.h"

#include "nautilus\nsTeamDocView.h"
#include "nautilus\nshistdo.h"

#include "partage\ns_timer.h"

#include "nssavoir\HealthTeam.rh"

// --------------------------------------------------------------------------
//
//					METHODES DE NSHealthTem Document / Vue
//
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
//
// Class NSHealthTeamDocument
//
// --------------------------------------------------------------------------

// Constructeur

NSHealthTeamDocument::NSHealthTeamDocument(TDocument *parent, NSContexte *pCtx, NSHealthTeam *pTeam)
                     :TDocument(parent), NSRoot(pCtx)
{
try
{
	SetTitle("Health Team") ;

	_pHealthTeam = pTeam ;
}
catch (...)
{
	erreur("Exception NSHealthTeamDocument ctor.", standardError, 0) ;
}
}

NSHealthTeamDocument::~NSHealthTeamDocument(){
	// never do delete pHealthTeam, it comes from NSPatientChoisi
}

bool
NSHealthTeamDocument::savePatpatho()
{
	NSPatPathoArray patPatho(pContexte->getSuperviseur()) ;
  DocumentIter iterDoc = pContexte->getPatient()->getDocHis()->DonnePrevPatPathoDocument("LEQUI1", &patPatho, NULL) ;
  if (iterDoc == NULL)
		return false ;

	NSNoyauDocument noyau(0, 0, 0, pContexte, false, true) ;

	(noyau._pDocInfo) = new NSDocumentInfo(pContexte) ; // *((*iterDoc)->pDonnees) ;

  NSDocumentData InfoData ;
  (*iterDoc)->initFromData(&InfoData) ;
	noyau._pDocInfo->setData(&InfoData) ;
  
	noyau.setPatPatho(_pHealthTeam->getPatho()) ;

	noyau.enregistrePatPatho() ;

  return true ;
}

void
NSHealthTeamDocument::invalidateViews(string sReason, string sReference)
{
	//
	// Valeurs possibles de sReason - Possible values for sReason
	//
	// MEMBER_NEW  MEMBER_REMOVED MEMBER_POS_CHANGED
	//
  // sReference can be ROSACE or LIST
  //
  TView* pView = GetViewList() ;
  if (NULL == pView)
  	return ;
	do
	{
		// Rosace
		//
		NSHealthTeamRosaceView* pRosaceView = TYPESAFE_DOWNCAST(pView, NSHealthTeamRosaceView) ;
		if (pRosaceView && (sReference == string("ROSACE")))
    {
    	RosaceManager* pRosaceManager = pRosaceView->getManager() ;
      *(_pHealthTeam->getPatho()) = *(pRosaceManager->getPatho()) ;
    }
    else
    {
    	// List
			//
			NSHealthTeamListView* pListView = TYPESAFE_DOWNCAST(pView, NSHealthTeamListView) ;
			if (pListView && (sReference == string("LIST")))
    	{
    		NSHealthTeamManager* pTeamManager = pListView->getManager() ;
      	*(_pHealthTeam->getPatho()) = *(pTeamManager->getPatho()) ;
    	}
		}

  	pView = NextView(pView) ;
	}
	while (pView) ;

  // First, record changes in health team document
  //
  if (false == savePatpatho())
		return ;

  _pHealthTeam->initFromPatho() ;

  // Then update views
  //
	pView = GetViewList() ;
  if (NULL == pView)
  	return ;

	do
	{
		// Rosace
		//
		NSHealthTeamRosaceView* pRosaceView = TYPESAFE_DOWNCAST(pView, NSHealthTeamRosaceView) ;
		if (pRosaceView)
			pRosaceView->reloadView(sReason, sReference) ;
    else
    {
    	// Vue secondaire
      //
      NSHealthTeamListView* pListView = TYPESAFE_DOWNCAST(pView, NSHealthTeamListView) ;
      if (pListView)
      	pListView->reloadView(sReason, sReference) ;
    }

		pView = NextView(pView) ;
	}
	while (pView) ;
}

// --------------------------------------------------------------------------
//
// Class NSHealthTeamRosaceView
//
// --------------------------------------------------------------------------

const int ID_Rosace = 0x100 ;

// Table de réponses de la classe NSHealthTeamRosaceView

DEFINE_RESPONSE_TABLE2(NSHealthTeamRosaceView, TView, TWindow)
    EV_VN_ISWINDOW,
    EV_WM_SIZE,
    EV_WM_MOVE,
    EV_WM_PAINT,
    EV_WM_WINDOWPOSCHANGED,
    EV_WM_SETFOCUS,
    EV_WM_MOUSEMOVE,
    EV_WM_DESTROY,END_RESPONSE_TABLE;

// Constructeur

NSHealthTeamRosaceView::NSHealthTeamRosaceView(NSHealthTeamDocument& doc, TWindow *parent)
                       :NSMUEView(doc.pContexte, &doc, parent, "HealthTeamRosace", "HealthTeamDoc")
  // NSMUEView(NSContexte* pCtx, TDocument* pDoc, TPaneSplitter *pPaneSplitter = 0, TWindow *parent = 0, string sFunction = "", string sPreoccu = "");
{
try
{
	_pTeamDoc = &doc ;

  _pRosaceManager = new RosaceManager(pContexte, _pTeamDoc->getHealthTeam()) ;
  _pRosaceDrawer  = new RosaceDrawer(pContexte, this, ID_Rosace, "", 0, 0, 0, 0, _pRosaceManager) ;

  initMUEViewMenu("menubar_healthTeam") ;

	bSetupToolBar = true ;

	setViewName() ;
}
catch (...)
{
	erreur("Exception NSHealthTeamRosaceView ctor.", standardError, 0) ;
}
}

// Destructeur
NSHealthTeamRosaceView::~NSHealthTeamRosaceView(){
  string ps = string("Deleting HealthTeamRosaceView.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  delete _pRosaceManager ;
  delete _pRosaceDrawer ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)

TWindow*
NSHealthTeamRosaceView::GetWindow()
{
	return (TWindow*) this ;
}

void
NSHealthTeamRosaceView::setViewName()
{
	_sViewName = pContexte->getSuperviseur()->getText("HealthTeamWindows", "healthTeamRosace") ;

  addConcernTitle() ;
}

// Appel de la fonction de remplissage de la vue
void
NSHealthTeamRosaceView::SetupWindow()
{
try
{
	if (_bFirstSetup)
		_pRosaceDrawer->MoveWindow(GetClientRect(), true) ;

	TWindow::SetupWindow() ;
  _pRosaceDrawer->SetupWindow() ;

  string sDocTitle = _sViewName = pContexte->getSuperviseur()->getText("HealthTeamWindows", "healthTeamRosace") ;
	TWindow::SetDocTitle(sDocTitle.c_str(), 0) ;

	_bFirstSetup = false ;
}
catch (...)
{
	erreur("Exception NSHealthTeamRosaceView::SetupWindow.", standardError, 0) ;
}
}

// Fonction CanClose : Appel de CanClose du document

bool
NSHealthTeamRosaceView::CanClose()
{
	TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;

	pMyApp->FlushControlBar() ;
	bSetupToolBar = false ;

	return true ;
}

bool
NSHealthTeamRosaceView::VnIsWindow(HWND hWnd)
{
	return HWindow == hWnd ;
}

void
NSHealthTeamRosaceView::EvDestroy()
{
}

// fonction permettant de prendre toute la taille de TWindow par la Rosace

void
NSHealthTeamRosaceView::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TWindow::EvSize(sizeType, size) ;
  _pRosaceDrawer->MoveWindow(GetClientRect(), true) ;
  _pRosaceDrawer->SetupWindow() ;
}

voidNSHealthTeamRosaceView::EvMove(ClassLib::TPoint& clientOrigin)
{
	TWindow::EvMove(clientOrigin) ;
}

voidNSHealthTeamRosaceView::EvWindowPosChanged(WINDOWPOS far& windowPos)
{
	TWindow::EvWindowPosChanged(windowPos) ;
}

voidNSHealthTeamRosaceView::gotMoved()
{
}

voidNSHealthTeamRosaceView::EvPaint(){
	TWindow::EvPaint() ;
}

// fonction EVSetFocusvoid
NSHealthTeamRosaceView::EvSetFocus(HWND hWndLostFocus)
{
  NSMUEView::EvSetFocus(hWndLostFocus) ;

	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;

	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}

	_pPaneSplitter->SetFrameTitle(getFunction(), _sViewName) ;	pContexte->setAideIndex("");	pContexte->setAideCorps("epi_soaping.htm");
}

voidNSHealthTeamRosaceView::SetupToolBar()
{
try
{
	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	// pMyApp->cb2->Insert(*new TButtonGadget(CM_EPISO_TRANSMIT, CM_EPISO_TRANSMIT, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->LayoutSession() ;
}
catch (...)
{
	erreur("Exception NSHealthTeamRosaceView::SetupToolBar.", standardError, 0) ;
}
}

void
NSHealthTeamRosaceView::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
}

void
NSHealthTeamRosaceView::reloadView(string sReason, string sReference)
{
	_pRosaceManager->reset(_pTeamDoc->getHealthTeam()) ;

	Invalidate() ;
}

// --------------------------------------------------------------------------
//
// Class NSHealthTeamListView
//
// --------------------------------------------------------------------------

bool
sortMMPByTypeSup(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getMandate()->getType() > pM2->getMandate()->getType()) ;
}

bool
sortMMPByTypeInf(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getMandate()->getType() < pM2->getMandate()->getType()) ;
}

bool
sortMMPByProfessionSup(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getProfessionLabel() > pM2->getProfessionLabel()) ;
}

bool
sortMMPByProfessionInf(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getProfessionLabel() < pM2->getProfessionLabel()) ;
}

bool
sortMMPBySpecialitySup(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getSpecialityLabel() > pM2->getSpecialityLabel()) ;
}

bool
sortMMPBySpecialityInf(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getSpecialityLabel() < pM2->getSpecialityLabel()) ;
}

bool
sortMMPByDistanceSup(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getDistance() > pM2->getDistance()) ;
}

bool
sortMMPByDistanceInf(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getDistance() < pM2->getDistance()) ;
}

bool
sortMMPByAngleSup(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getAngle() > pM2->getAngle()) ;
}

bool
sortMMPByAngleInf(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getAngle() < pM2->getAngle()) ;
}

bool
sortMMPByBeginDateSup(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getBeginDate() > pM2->getBeginDate()) ;
}

bool
sortMMPByBeginDateInf(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getBeginDate() < pM2->getBeginDate()) ;
}

bool
sortMMPByEndDateSup(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getEndDate() > pM2->getEndDate()) ;
}

bool
sortMMPByEndDateInf(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getEndDate() < pM2->getEndDate()) ;
}

bool
sortMMPByNameSup(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getName() > pM2->getName()) ;
}

bool
sortMMPByNameInf(NSMemberMandatePair *pM1, NSMemberMandatePair *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getName() < pM2->getName()) ;
}

const int ID_HTList = 0x100 ;

const int CM_MEMBER_NEW    = 0x110 ;
const int CM_MEMBER_EDIT   = 0x111 ;
const int CM_MEMBER_DELETE = 0x112 ;
const int CM_COLUMN_SWITCH = 0x113 ;
const int CM_MODIFY_PERSON = 0x114 ;

// Table de réponses de la classe NSHealthTeamListView

DEFINE_RESPONSE_TABLE1(NSHealthTeamListView, NSMUEView)
  EV_VN_ISWINDOW,
  EV_LVN_GETDISPINFO(ID_HTList, DispInfoListe),
  EV_LVN_COLUMNCLICK(ID_HTList, LVNColumnclick),
  EV_WM_TIMER,
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_GENERAL_ADD,   CmAdd),
  EV_COMMAND(CM_GENERAL_FCT1,  CmFct1),
  EV_COMMAND(CM_MEMBER_EDIT,   CmChange),
  EV_COMMAND(CM_GENERAL_CLOSE, CmClose),
  EV_COMMAND(CM_MODIFY_PERSON, CmModifyPerson),
  EV_COMMAND(CM_COLUMN_SWITCH, CmSwitchColumnsExtent),
END_RESPONSE_TABLE ;

// Constructeur
NSHealthTeamListView::NSHealthTeamListView(NSHealthTeamDocument& doc, TWindow *parent)
                     :NSMUEView(doc.pContexte, &doc, parent, "HealthTeamList", "HealthTeamDoc"),
                      _tManager(doc.getHealthTeam())
{
try
{
  _pTeamDoc         = &doc ;
  _pListeWindow     = new NSHealthTeamListWindow(this, ID_HTList, 0, 0, 0, 0) ;

  _bActiveOnly      = true ;

	initMUEViewMenu("menubar_healthTeam") ;

  _pToolBar         = (OWL::TControlBar*) 0 ;
  _bSetupToolBar    = true ;

  _bExtendedColumns = false ;
  _iSortedColumn    = -1 ;
  _bLocalMode       = true ;

	// string sText = pContexte->getSuperviseur()->getText("HealthTeamWindows", "newCorrespondent") ;
	// _pBtNew   = new NSGadgetButton(this, CM_GENERAL_ADD, sText.c_str(), 0, 0, 300, 20) ;
  _pBtNew           = (NSGadgetButton*) 0 ;
  _pBtNewTrt        = (NSGadgetButton*) 0 ;

  // sText = pContexte->getSuperviseur()->getText("HealthTeamWindows", "alterCorrespondent") ;
	// _pBtEdit  = new NSGadgetButton(this, CM_MEMBER_EDIT, sText.c_str(), 0, 0, 50, 15) ;
  _pBtEdit          = (NSGadgetButton*) 0 ;

  // sText = pContexte->getSuperviseur()->getText("HealthTeamWindows", "removeCorrespondent") ;
	// _pBtClose = new NSGadgetButton(this, CM_GENERAL_CLOSE, sText.c_str(), 0, 0, 300, 20) ;
  _pBtClose         = (NSGadgetButton*) 0 ;

	setViewName() ;
}
catch (...)
{
  erreur("Exception NSHealthTeamListView ctor.", standardError, 0) ;
}
}

// Destructeur
NSHealthTeamListView::~NSHealthTeamListView()
{
  string ps = string("Deleting HealthTeamListView.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  if (_pListeWindow)
    delete _pListeWindow ;

	// Done by OWL's garbage collector
/*
	if (NULL != _pBtNew)
		delete _pBtNew ;
  if (NULL != _pBtEdit)
		delete _pBtEdit ;
	if (NULL != _pBtClose)
		delete _pBtClose ;
*/
}

void
NSHealthTeamListView::setViewName()
{
	_sViewName = pContexte->getSuperviseur()->getText("HealthTeamWindows", "healthTeamMandateList") ;

  addConcernTitle() ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow
*NSHealthTeamListView::GetWindow()
{
  return (TWindow *) this ;
}

// Appel de la fonction de remplissage de la vuevoid
NSHealthTeamListView::SetupWindow()
{
  NSMUEView::SetupWindow() ;
  Parent->SetCaption(_sViewName.c_str()) ;

  SetupColumns() ;
  initCurentMembers() ;
  AfficheListe() ;
}

bool
NSHealthTeamListView::CanClose()
{
	return NSMUEView::CanClose() ;
}

void
NSHealthTeamListView::CmAdd()
{
  AddCorresp(false) ;
}

void
NSHealthTeamListView::CmFct1()
{
  AddCorresp(true) ;
}

void
NSHealthTeamListView::AddCorresp(bool IsTraitant)
{
try
{
	bool bMandateVerbose = false ;

	NSHealthTeamMember *pMember ;
  string sPids = "" ;

  NSTPersonListDialog indep((TWindow *)this, pidsCorresp, true, pContexte, 0, true) ;
  int	iDialogReturn = indep.Execute() ;
  if (iDialogReturn != IDOK)
  	return ;

  if (indep.bCreer)
  {
    NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->createPerson(pContexte, pidsCorresp) ;
    if (pPersonInfo)
    {
      sPids = pPersonInfo->getNss() ;  // On recupre le personne id
      pMember = new NSHealthTeamMember(sPids) ;
      if (pMember->isValid())
        _tManager.addMember(pMember, NSHealthTeamMember::person) ; // On ajoute le membre
    }
    else
    	return ;
  }
  else
  {
    NSPersonInfo *temp = indep.pPersonSelect ;
    if (temp)
    {
      sPids = temp->getNss() ;
      if (string("") != sPids)
      {
        pMember = new NSHealthTeamMember(sPids) ;
        if (pMember->isValid())
          _tManager.addMember(pMember, NSHealthTeamMember::person) ;
      }
    }
    else
    {
      // TODO
      // Rajouter l'erreur
    }
  }

  NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPids, pidsCorresp) ;
  if (pPersonInfo)
  {
  	string sMemberName = pPersonInfo->getCivilite() ;
    // NSMemberMandatePair* man = new NSMemberMandatePair(pContexte, pMember, new NSHealthTeamMandate()) ;
    // man->setName(sMemberName) ;

    NSHealthTeamMandate *pMandate = new NSHealthTeamMandate() ;

    // Pre-init the mandate
    pMandate->autoInitMandate(pPersonInfo) ;
    if (IsTraitant)
      pMandate->setAsARecordKeeper() ;

		if (bMandateVerbose)
		{
    	HealthTeamMandateInterface test(this, pContexte, sMemberName, pMandate) ;
    	test.Execute() ;
		}

    _tManager.addMandate(pMember, pMandate) ;
  }

  refreshHealthTeamDoc() ;
}
catch (...)
{
	erreur("Exception NSHealthTeamListView::CmNouveau.", standardError, 0) ;
}
}

void
NSHealthTeamListView::CmChange()
{
	int index = _pListeWindow->IndexItemSelect() ;
  if ((index < 0) || (index >= (int) _aMembers.size()))
		return ;

	NSMemberMandatePair* pDel = _aMembers[index] ;

  if (NULL == pDel)
    return ;

  _tManager.modMandate(pContexte, pDel->getMember(), pDel->getMandate()) ;
  refreshHealthTeamDoc() ;
}

void
NSHealthTeamListView::CmClose()
{
  int iCount = _pListeWindow->GetItemCount() ;
  if (0 == iCount)
    return ;

	int index = _pListeWindow->IndexItemSelect() ;
  if ((index < 0) || (index >= (int) _aMembers.size()))
  {
    string sWarningMsg = pContexte->getSuperviseur()->getText("HealthTeamWindows", "mustSelectCorrespondent") ;
		erreur(sWarningMsg.c_str(), warningError, 0) ;
		return ;
  }

  int iRemovedCount = 0 ;

	for (int i = 0 ; i < iCount ; i++)
  	if (_pListeWindow->GetItemState(i, LVIS_SELECTED))
    {
      NSMemberMandatePair* pDel = _aMembers[i] ;
      if (pDel)
      {
        bool bClose = false ;

        // Ask for confirmation
        //
        string sCivility = string("") ;
        string sPersonId = pDel->getMember()->getID() ;
        NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPersonId, pidsCorresp, NSPersonArray::asynchronous) ;
        if (pPersonInfo)
          sCivility = pPersonInfo->getCivilite() ;

        if (string("") == sCivility)
          bClose = true ;
        else
        {
          string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
          string sMessage = string("Souhaitez-vous vraiment ôter ") + sCivility + string(" de la liste des correspondants de ce patient ?") ;
          int ret = MessageBox(sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;
          if (IDYES == ret)
        	  bClose = true ;
        }

        if (bClose)
        {
          iRemovedCount++ ;
          _tManager.closeMandate(pContexte, pDel->getMember(), pDel->getMandate()) ;
        }
      }
    }

  if (iRemovedCount > 0)
    refreshHealthTeamDoc() ;
}

void
NSHealthTeamListView::CmModifyPerson()
{
	int index = _pListeWindow->IndexItemSelect() ;
  if ((index < 0) || (index >= (int) _aMembers.size()))
		return ;

	NSMemberMandatePair* pMMP = _aMembers[index] ;
  if (NULL == pMMP)
    return ;

  NSHealthTeamMember* pHTM = pMMP->getMember() ;
  if (NULL == pHTM)
    return ;

  string sPids = pHTM->getID() ;

  NSPersonInfo* pPerson = pContexte->getPersonArray()->lookForPersonInArray(sPids) ;
  if (NULL == pPerson)
    return ;

  string sRoles = pPerson->getRoles() ;

  pContexte->getPersonArray()->modifyPerson(pContexte, sPids, sRoles, pidsCorresp) ;

  refreshHealthTeamDoc() ;
}

void
NSHealthTeamListView::refreshHealthTeamDoc()
{
	NSToDoTask* pTask = new NSToDoTask ;

  pTask->setWhatToDo(string("RefreshHealthTeamInformation")) ;
  pTask->setParam1(string("MEMBER_NEW")) ;
  pTask->setParam2(string("LIST")) ;

  pTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSHealthTeamListView::reloadView(string sReason, string sReference)
{
	initCurentMembers() ;
  AfficheListe() ;
}

void
NSHealthTeamListView::initCurentMembers()
{
  string ps2 = "NSHealthTeamListView::initCurentMembers : begin" ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;

	NSHealthTeamMemberArray* pMembers = _tManager.getMembers() ;
	_aMembers.initFromHealthTeam(pMembers, pContexte, false) ;

  if (_bActiveOnly)
		removeClosedMandates() ;

	if (_bLocalMode)
		removePatientAndLocalTeam() ;

  ps2 = "NSHealthTeamListView::initCurentMembers : end" ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;
}

void
NSHealthTeamListView::removeClosedMandates()
{
	if (_aMembers.empty())
		return ;

	NVLdVTemps tpsNow ;
	tpsNow.takeTime() ;

	NSMemberMandatePairIter itMbrs = _aMembers.begin() ;
  for ( ; _aMembers.end() != itMbrs ; )
  {
  	NVLdVTemps tpsEnd ;
    tpsEnd.initFromDateHeure((*itMbrs)->getEndDate()) ;
    if (tpsEnd <= tpsNow)
    {
    	delete *itMbrs ;
      _aMembers.erase(itMbrs) ;
    }
    else
    	itMbrs++ ;
  }
}

void
NSHealthTeamListView::removePatientAndLocalTeam()
{
	if (_aMembers.empty())
		return ;

	NSMemberMandatePairIter itMbrs = _aMembers.begin() ;
  for ( ; _aMembers.end() != itMbrs ; )
  {
  	NSHealthTeamMember *pMember = (*itMbrs)->getMember() ;

    if ((pMember->getID() == pContexte->getPatient()->getNss()) ||
        (pMember->getType() == NSHealthTeamMember::team))
    {
    	delete *itMbrs ;
      _aMembers.erase(itMbrs) ;
    }
    else
    	itMbrs++ ;
  }
}

// Initialisation des colonnes de la ListWindowvoid
NSHealthTeamListView::SetupColumns()
{
	string sMemberName = pContexte->getSuperviseur()->getText("HealthTeamWindows", "member") ;
  string sJob        = pContexte->getSuperviseur()->getText("HealthTeamWindows", "work") ;
	string sSpeciality = pContexte->getSuperviseur()->getText("HealthTeamWindows", "speciality") ;
  string sCity       = pContexte->getSuperviseur()->getText("generalLanguage", "city") ;

	if (true == _bExtendedColumns)
	{
    string sDistance   = pContexte->getSuperviseur()->getText("HealthTeamWindows", "distance") ;
    string sAngle      = pContexte->getSuperviseur()->getText("HealthTeamWindows", "angle") ;
    string sBeginDate  = pContexte->getSuperviseur()->getText("HealthTeamWindows", "begindate") ;
    string sEndDate    = pContexte->getSuperviseur()->getText("HealthTeamWindows", "enddate") ;

    _pListeWindow->InsertColumn(0, TListWindColumn((char*)sJob.c_str(),         80, TListWindColumn::Left, 0)) ;
    _pListeWindow->InsertColumn(1, TListWindColumn((char*)sMemberName.c_str(), 300, TListWindColumn::Left, 1)) ;
    _pListeWindow->InsertColumn(2, TListWindColumn((char*)sSpeciality.c_str(), 120, TListWindColumn::Left, 2)) ;
    _pListeWindow->InsertColumn(3, TListWindColumn((char*)sDistance.c_str(),    20, TListWindColumn::Left, 3)) ;
    _pListeWindow->InsertColumn(4, TListWindColumn((char*)sAngle.c_str(),       60, TListWindColumn::Left, 4)) ;
    _pListeWindow->InsertColumn(5, TListWindColumn((char*)sBeginDate.c_str(),  140, TListWindColumn::Left, 5)) ;
    _pListeWindow->InsertColumn(6, TListWindColumn((char*)sEndDate.c_str(),    140, TListWindColumn::Left, 6)) ;

    _iColumnNb = 7 ;
	}
  else
	{
    _pListeWindow->InsertColumn(0, TListWindColumn((char*)sJob.c_str(),         80, TListWindColumn::Left, 0)) ;
    _pListeWindow->InsertColumn(1, TListWindColumn((char*)sMemberName.c_str(), 300, TListWindColumn::Left, 1)) ;
    _pListeWindow->InsertColumn(2, TListWindColumn((char*)sSpeciality.c_str(), 120, TListWindColumn::Left, 2)) ;
    _pListeWindow->InsertColumn(3, TListWindColumn((char*)sCity.c_str(),       150, TListWindColumn::Left, 3)) ;

    _iColumnNb = 4 ;
  }
}

// Affichage des éléments de la listevoid
NSHealthTeamListView::AfficheListe()
{
	_pListeWindow->DeleteAllItems() ;

  if (_aMembers.empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NSMemberMandatePairRIter itMbrs = _aMembers.rbegin() ;
  for ( ; _aMembers.rend() != itMbrs ; itMbrs++)
  {
    string sPersonId = (*itMbrs)->getMember()->getID() ;

    // Start asynchronous request to get this person
    //
    NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPersonId, pidsCorresp, NSPersonArray::asynchronous) ;
    if (pPersonInfo)
      (*itMbrs)->setName(pPersonInfo->getCivilite()) ;
    else
      (*itMbrs)->setName(string("->") + sPersonId) ;

    TListWindItem Item(((*itMbrs)->getProfessionLabel()).c_str(), 0) ;
    _pListeWindow->InsertItem(Item) ;
	}

  SetTimer(ID_TEAMTIMER, 500) ;
}

void
NSHealthTeamListView::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  TListWindItem &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  NSMemberMandatePair *pMbMdPair = _aMembers[index] ;
  if ((NSMemberMandatePair*) NULL == pMbMdPair)
    return ;

	string sLang = pContexte->getUserLanguage() ;

	string sText = string("") ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case 1  : // civility
    {
  		sText = pMbMdPair->getName() ;

      // If this person has not been received yet, we just get her traits
      // in order to display basic information
      //
      if ((strlen(sText.c_str()) > 2) && (string("->") == string(sText, 0, 2)))
      {
        // NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson((_aMembers[index])->getMember()->getID(), pidsCorresp) ;
        // (_aMembers[index])->setName(pPersonInfo->sCivilite) ;

        string sNameFromTraits = pMbMdPair->getNameFromTraits() ;
        if (string("") == sNameFromTraits)
        {
          getInfosFromTraits(pMbMdPair->getMember()->getID(), index) ;

          // If it failed, make sure it won't do it endlessly
          if (string("") == pMbMdPair->getNameFromTraits())
            pMbMdPair->setNameFromTraits(string(sText, 2, strlen(sText.c_str())-2) + string(" (?)")) ;
        }

        sText = pMbMdPair->getNameFromTraits() ;
      }
      else if (string("") == sText)
      {
        sText = string("->") + pMbMdPair->getMember()->getID() + string(" (?)") ;
        pMbMdPair->setName(sText) ;
      }

      break ;
    }
  	// case 1  : // job
  	// 	sText = (_aMembers[index])->getProfessionLabel() ;
    //  break ;

  	case 2  : // speciality
    	sText = pMbMdPair->getSpecialityLabel() ;
      break ;

  	case 3  : // distance
    	if (true == _bExtendedColumns)
    		sText = pMbMdPair->getDistanceTxt() ;
      else
      {
        NSHealthTeamMember* pHTM = pMbMdPair->getMember() ;
        if (pHTM)
        {
          string sPids  = pHTM->getID() ;
          NSPersonInfo* pPerson = pContexte->getPersonArray()->lookForPersonInArray(sPids) ;
          if (pPerson)
            sText = pPerson->getVille() ;
        }
      }

      break ;

  	case 4  : // angle
    	sText = pMbMdPair->getStringAngle() ;
      break ;

    case 5  : // date début
    {
    	NVLdVTemps tpsBegin ;
      tpsBegin.initFromDateHeure(pMbMdPair->getBeginDate()) ;
      sText = (tpsBegin.donneFormattedDateHeure(pContexte, sLang)).c_str() ;
    }
      break ;

    case 6  : // date fin    {      NVLdVTemps tpsEnd ;      tpsEnd.initFromDateHeure(pMbMdPair->getEndDate()) ;      if (tpsEnd.estNoLimit())      	sText = pContexte->getSuperviseur()->getText("HealthTeamWindows", "notimelimit") ;      else
      {
      	tpsEnd.initFromDateHeure((_aMembers[index])->getEndDate()) ;
        sText = (tpsEnd.donneFormattedDateHeure(pContexte, sLang)).c_str() ;
      }
    }      break ;
  }

  static char buffer[255] ;
  buffer[0] = '\0' ;

  sprintf(buffer, "%s", sText.c_str()) ;
  dispInfoItem.SetText(buffer) ;

  // Writing this leads to strange truncations in list's columns
  // dispInfoItem.SetText(sText.c_str()) ;
}

void
NSHealthTeamListView::CmSwitchColumnsExtent()
{
	if (_bExtendedColumns)
		_bExtendedColumns = false ;
	else
  	_bExtendedColumns = true ;

	// Delete all of the columns
  //
	for (int i = 0 ; i < _iColumnNb ; i++)
  	_pListeWindow->DeleteColumn(0) ;

	SetupColumns() ;

  Invalidate() ;
}

void
NSHealthTeamListView::LVNColumnclick(TLwNotify& lwn)
{
	changeSortOrder(lwn.iSubItem) ;

  switch ( lwn.iSubItem )
  {
    case 0  : sortByName() ;      break ;
    case 1  : sortByJob() ;       break ;
    case 2  : sortBySpec() ;      break ;
    case 3  : sortByDistance() ;  break ;
    case 4  : sortByAngle() ;     break ;
    case 5  : sortByBegining() ;  break ;
    case 6  : sortByEnding() ;    break ;
  }
}

void
NSHealthTeamListView::changeSortOrder(int iOrder)
{
	if (_iSortedColumn == iOrder)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = iOrder ;
    _bNaturallySorted = true ;
  }
}

void
NSHealthTeamListView::sortByName()
{
  if (_aMembers.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByNameInf) ;
  else
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByNameSup) ;

  AfficheListe() ;
}

void
NSHealthTeamListView::sortByJob()
{
  if (_aMembers.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByProfessionInf) ;
  else
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByProfessionSup) ;

  AfficheListe() ;
}

void
NSHealthTeamListView::sortBySpec()
{
	if (_aMembers.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMembers.begin(), _aMembers.end(), sortMMPBySpecialityInf) ;
  else
    sort(_aMembers.begin(), _aMembers.end(), sortMMPBySpecialitySup) ;

  AfficheListe() ;
}

void
NSHealthTeamListView::sortByDistance()
{
  if (_aMembers.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByDistanceInf) ;
  else
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByDistanceSup) ;

  AfficheListe() ;
}

void
NSHealthTeamListView::sortByAngle()
{
  if (_aMembers.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByAngleInf) ;
  else
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByAngleSup) ;

  AfficheListe() ;
}

void
NSHealthTeamListView::sortByEnding()
{
  if (_aMembers.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByEndDateInf) ;
  else
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByEndDateSup) ;

  AfficheListe() ;
}

void
NSHealthTeamListView::sortByBegining()
{
  if (_aMembers.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByBeginDateInf) ;
  else
    sort(_aMembers.begin(), _aMembers.end(), sortMMPByBeginDateSup) ;

  AfficheListe() ;
}

void
NSHealthTeamListView::getInfosFromTraits(string sPersonId, int iIndex)
{
  if (string("") == sPersonId)
    return ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string sTrace = string("Getting infos from traits for ") + sPersonId ;
  pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

  NSPersonsAttributesArray AttsList ;

  NSBasicAttributeArray AttrArray ;
  AttrArray.push_back(new NSBasicAttribute(PERSON, sPersonId)) ;

  const char* serviceName = (NautilusPilot::SERV_PERSON_TRAITS_LIST).c_str() ;

	bool listOk = pSuper->getPilot()->getUserProperties(serviceName, &AttsList, &AttrArray) ;
  if ((false == listOk) || AttsList.empty())
  {
    sTrace = string("Infos from traits for ") + sPersonId + string(" not found") ;
    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;
    return ;
  }

  NSPersonsAttributeIter iterPerson = AttsList.begin() ;

  NSBasicAttributeArray* pAttribute = *iterPerson ;
  if ((NULL == pAttribute) || pAttribute->empty())
  {
    sTrace = string("Infos from traits for ") + sPersonId + string(" are empty") ;
    pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;
    return ;
  }

  string sCivility = string("") ;

  NSBasicAttributeIter iter = pAttribute->begin() ;
	for ( ; pAttribute->end() != iter ; iter++)
	{
    if ((*iter)->getBalise() == FIRST_NAME)
    {
      if (string("") != sCivility)
        sCivility += string(" ") ;
      sCivility += (*iter)->getText() ;
    }
    else if ((*iter)->getBalise() == LAST_NAME)
    {
      if (string("") != sCivility)
        sCivility = (*iter)->getText() + string(" ") + sCivility ;
      else
        sCivility = (*iter)->getText() ;
    }
  }

  if (string("") != sCivility)
    (_aMembers[iIndex])->setNameFromTraits(string("- ") + sCivility) ;

  sTrace = string("Infos from traits for ") + sPersonId + string(" returns \"") + sCivility + string("\"") ;
  pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;
}

void
NSHealthTeamListView::EvTimer(uint timerId)
{
	if (ID_TEAMTIMER != timerId)
		return ;

  bool bAllTeamThere = true ;
  bool bGotNewMember = false ;

  NSMemberMandatePairRIter itMbrs = _aMembers.rbegin() ;
  for ( ; _aMembers.rend() != itMbrs ; itMbrs++)
  {
    // Ask the person in "justCheck" mode
    //
    NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, (*itMbrs)->getMember()->getID(), pidsCorresp, NSPersonArray::justCheck) ;
    if (NULL == pPersonInfo)
      bAllTeamThere = false ;
    else if (pPersonInfo->getCivilite() != (*itMbrs)->getName())
    {
      (*itMbrs)->setName(pPersonInfo->getCivilite()) ;
      bGotNewMember = true ;
    }
    // pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
	}

  if (bGotNewMember)
    Invalidate() ;

  if (bAllTeamThere)
    KillTimer(ID_TEAMTIMER) ;
}

boolNSHealthTeamListView::VnIsWindow(HWND hWnd){
  return (HWindow == hWnd) ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindowvoid
NSHealthTeamListView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
	_pListeWindow->MoveWindow(GetClientRect(), true) ;
}

// fonction EVSetFocusvoid
NSHealthTeamListView::EvSetFocus(HWND hWndLostFocus)
{
	NSMUEView::EvSetFocus(hWndLostFocus);

  focusFct() ;

	_pListeWindow->SetFocus() ;
}

// fonction Click droit : menu contextuel
void
NSHealthTeamListView::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point ;

	TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sEdit  = pSuper->getText("HealthTeamWindows", "editThisMandate") ;
	string sNew   = pSuper->getText("HealthTeamWindows", "newMandate") ;
  string sTrt   = pSuper->getText("HealthTeamWindows", "newRecordKeeper") ;
	string sDel   = pSuper->getText("HealthTeamWindows", "deleteThisMandate") ;
  string sModif = pSuper->getText("functionsLabels",   ".episodus:OuterModify") ;

  string sSwitchMode ;
  if (true == _bExtendedColumns)
  	sSwitchMode = pSuper->getText("HealthTeamWindows", "switchToSimpleMode") ;
  else
  	sSwitchMode = pSuper->getText("HealthTeamWindows", "switchToExtendedMode") ;

  int	index = _pListeWindow->IndexItemSelect() ;
	if (-1 != index)
	{
		// menu->AppendMenu(MF_STRING, CM_MEMBER_EDIT,   sEdit.c_str()) ;
  	menu->AppendMenu(MF_STRING, CM_GENERAL_CLOSE, sDel.c_str()) ;
		menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, CM_GENERAL_ADD,   sNew.c_str()) ;
    menu->AppendMenu(MF_STRING, CM_GENERAL_FCT1,  sTrt.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, CM_MODIFY_PERSON, sModif.c_str()) ;
	}
	else
  {
		menu->AppendMenu(MF_STRING, CM_GENERAL_ADD,  sNew.c_str()) ;
    menu->AppendMenu(MF_STRING, CM_GENERAL_FCT1, sTrt.c_str()) ;
  }

  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, CM_COLUMN_SWITCH, sSwitchMode.c_str()) ;

	ClientToScreen(lp) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
	delete menu ;
}

void
NSHealthTeamListView::EvRButtonDownOut(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
  NS_CLASSLIB::TPoint lp = point ;

  TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sNew = pSuper->getText("HealthTeamWindows", "newMandate") ;
  string sTrt = pSuper->getText("HealthTeamWindows", "newRecordKeeper") ;
  menu->AppendMenu(MF_STRING, CM_MEMBER_NEW, sNew.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_GENERAL_FCT1,  sTrt.c_str()) ;

  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}

void
NSHealthTeamListView::focusFct()
{
	activateMUEViewMenu() ;

  TMyApp  *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (_bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
    SetupToolBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }

  _pPaneSplitter->SetFrameTitle(getFunction(), _sViewName) ;
  pContexte->setAideIndex("") ;
  pContexte->setAideCorps("medicaments.htm") ;
}


// SetupToolBarvoid
NSHealthTeamListView::SetupToolBar()
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  // Make certain secondary control bar is empty
  //
	TMyApp  *pMyApp = pSuper->getApplication() ;
  pMyApp->FlushControlBar() ;
  pMyApp->getSecondaryControlBar()->LayoutSession() ;

  // Setup pane splitter's control bar
  //
	_pPaneSplitter->FlushControlBar() ;

	// FlushControlBar destroys the buttons... we need to create them after
	//
  string sText = pSuper->getText("HealthTeamWindows", "newCorrespondent") ;
	_pBtNew    = new NSGadgetButton(this, CM_GENERAL_ADD, sText.c_str(), 0, 0, 200, 20) ;
  sText = pSuper->getText("HealthTeamWindows", "newRecordKeeper") ;
	_pBtNewTrt = new NSGadgetButton(this, CM_GENERAL_FCT1, sText.c_str(), 0, 0, 200, 20) ;
  sText = pSuper->getText("HealthTeamWindows", "removeCorrespondent") ;
	_pBtClose  = new NSGadgetButton(this, CM_GENERAL_CLOSE, sText.c_str(), 0, 0, 200, 20) ;

  _pPaneSplitter->Insert(*new TControlGadget(*_pBtNew)) ;
  _pPaneSplitter->Insert(*new TControlGadget(*_pBtNewTrt)) ;
	// _pPaneSplitter->Insert(*new TSeparatorGadget) ;
  _pPaneSplitter->Insert(*new TControlGadget(*_pBtClose)) ;

  _pPaneSplitter->LayoutSession() ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSHealthTeamListWindow//
// -----------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(NSHealthTeamListWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  // EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSHealthTeamListWindow::NSHealthTeamListWindow(NSHealthTeamListView *parent, int id, int x, int y, int w, int h, TModule *module)
                       :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  _pView = parent ;
  _iRes  = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

void
NSHealthTeamListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSHealthTeamListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  	_pView->CmModifyPerson() ;
  else
  	_pView->CmAdd() ;
}

void
NSHealthTeamListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    _pView->EvRButtonDown(modkeys, point) ;
  }
  else
  	_pView->EvRButtonDownOut(modkeys, point) ;
}

void
NSHealthTeamListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if      (key == VK_DELETE)
  	_pView->CmClose() ;
  else if (key == VK_INSERT)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	_pView->CmChange() ;
    else
    	_pView->CmAdd() ;
  }
  else if (key == VK_TAB)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	_pView->setFocusToPrevSplitterView() ;
    else
    	_pView->setFocusToNextSplitterView() ;
  }
  else
  	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

/*
void
NSHealthTeamListWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt)
{
	NSContexte  *pContexte  = pView->pContexte ;
  NSSuper     *pSuper     = pContexte->getSuperviseur() ;

  if (pSuper->bDragAndDrop)
  {
  	pSuper->DragDrop->DragLeave( *this ) ;
    pSuper->DragDrop->EndDrag() ;
    delete pSuper->DragDrop ;
    pSuper->DragDrop = 0 ;
    pSuper->bDragAndDrop = false ;
  }
}
*/

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSHealthTeamListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

void
NSHealthTeamListWindow::EvSetFocus(HWND hWndLostFocus)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
  SetTextBkColor(0x00fff0f0) ;
  Invalidate();

  _pView->focusFct() ;

  int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSHealthTeamListWindow::EvKillFocus(HWND hWndGetFocus)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
  SetTextBkColor(0x00ffffff) ;
  Invalidate();
}

