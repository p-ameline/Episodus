// --------------------------------------------------------------------------// NSVENUEDOCVIEW.CPP		Document/Vues Venues management
// --------------------------------------------------------------------------
// PA - march 2010
// --------------------------------------------------------------------------

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsbb\nsPaneSplitter.h"

#include "nsdn\nsdocum.h"
#include "nsdn\nsdocdlg.h"
#include "nautilus\nsdocref.h"
#include "nssavoir\nscaptur.h"

#include "nautilus\nsVenueDocView.h"

// --------------------------------------------------------------------------
//
//					METHODES DE NSVenues Document / Vue
//
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
//
// Class NSVenuesDocument
//
// --------------------------------------------------------------------------

// Constructeur

NSVenuesDocument::NSVenuesDocument(TDocument *parent, NSContexte *pCtx)
                 :TDocument(parent), NSRoot(pCtx)
{
try
{
	SetTitle("Venues") ;

  if ((NSPatientChoisi*) NULL == pContexte->getPatient())
    return ;

  NSPersonGraphManager* pGraph = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraph)
    return ;

  pGraph->GetStayArray(pContexte, &_aVenuesArray) ;
}
catch (...)
{
	erreur("Exception NSVenuesDocument ctor.", standardError, 0) ;
}
}

NSVenuesDocument::~NSVenuesDocument(){
  emptyVenuesArray() ;
}

bool
NSVenuesDocument::saveVenue(NSSejourInfo *pSejourInfo)
{
  return true ;
}

bool
NSVenuesDocument::addNewVenue(NSSejourInfo *pVenue)
{
  if ((NSSejourInfo*) NULL == pVenue)
    return false ;

  NSPatPathoArray PPT(pContexte->getSuperviseur(), graphPerson) ;

  if (false == pVenue->buildPatPatho(&PPT))
    return false ;

  NSRefDocument NewVenue(0, pContexte) ;
  NewVenue.setReadOnly(false) ;
  NewVenue.setPatPatho(&PPT) ;

  string sLang = string("") ;
  if (pContexte)
  	sLang = pContexte->getUserLanguage() ;

  string sRootCode = (*(PPT.begin()))->getLexique() ;

	string sLibelleDoc ;
  pContexte->getDico()->donneLibelle(sLang, &sRootCode, &sLibelleDoc) ;
  if (string("") != sLibelleDoc)
    sLibelleDoc[0] = pseumaj(sLibelleDoc[0]) ;

  if (false == NewVenue.Referencer("ZCS00", sLibelleDoc,
                                    string(""), string(""),  // file and path
                                    false, false,
                                    string(""),              // doc ID
                                    NSRootLink::fonctionalUnitStay))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("venuesWindows", "venueCreationFailed") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
	}

  // On enregistre la patpatho
	bool bSavePpt = NewVenue.enregistrePatPatho() ;
	if (false == bSavePpt)
		return false ;

  reloadVenuesArray() ;

  invalidateViews("", "") ;

  return true ;
}

bool
NSVenuesDocument::updateExistingVenue(NSSejourInfo *pVenue)
{
  if ((NSSejourInfo*) NULL == pVenue)
    return false ;

  // Get previous patpatho
  //
  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pGraphManager)
    return false ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  NSPatPathoArray previousPPT(pSuper, graphPerson) ;

  string sRosace ;
  bool bGotTree = pGraphManager->getTree(pVenue->getTreeID(), &previousPPT, &sRosace) ;
  if (false == bGotTree)
  {
		string sErrorText = pSuper->getText("graphAndTreeErrors", "noTreeWithThisIdInTheGraph") + string(" (") + pVenue->getTreeID() + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  // Apply changes
  //
  NSPatPathoArray newPPT(pSuper, graphPerson) ;
  newPPT = previousPPT ;

  pVenue->updatePatPatho(&newPPT) ;

  // If nothing changed... nothing to do
  //
  if (newPPT == previousPPT)
    return true ;

  // Record changes
  //
  NSVectPatPathoArray VectUpdate ;
	string sTreeID      = string("") ;
  string sCodeDocMeta = string("") ;
	VectUpdate.push_back(new NSPatPathoArray(newPPT)) ;

	pGraphManager->updateTrees(pContexte, &VectUpdate, &sTreeID, &sCodeDocMeta, pidsPatient) ;

	if (string("") == sTreeID)
	{
  	string sErrorText = pSuper->getText("documentManagementErrors", "failedToSave") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return false ;
	}

  reloadVenuesArray() ;
  invalidateViews(string("EDITED")) ;

  return true ;
}

void
NSVenuesDocument::emptyVenuesArray()
{
  if (_aVenuesArray.empty())
    return ;

  NSSejourIter itVenue = _aVenuesArray.begin() ;
  for ( ; _aVenuesArray.end() != itVenue ; )
  {
    delete *itVenue ;
    _aVenuesArray.erase(itVenue) ;
	}
}

void
NSVenuesDocument::reloadVenuesArray()
{
  emptyVenuesArray() ;

  if ((NSPatientChoisi*) NULL == pContexte->getPatient())
    return ;

  NSPersonGraphManager* pGraph = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraph)
    return ;

  pGraph->GetStayArray(pContexte, &_aVenuesArray) ;
}

bool
NSVenuesDocument::updateVenuesFromCapture(NSCaptureArray* pCapt)
{
  if ((NSCaptureArray*) NULL == pCapt)
    return false ;

  if (pCapt->empty())
    return true ;

  string sVenueId = string("") ;
  pCapt->trouveChemin(string("LSEJO1/LNUSE1"), &sVenueId) ;

  if (string("") == sVenueId)
    return true ;

  NSSejourInfo* pVenue = getVenueForId(sVenueId) ;
  if ((NSSejourInfo*) NULL == pVenue)
    return createVenueFromCapture(pCapt) ;

  return updateSelectedVenueFromCapture(pVenue, pCapt) ;
}

bool
NSVenuesDocument::createVenueFromCapture(NSCaptureArray* pCapt)
{
  if (((NSCaptureArray*) NULL == pCapt) || pCapt->empty())
    return false ;

  NSSejourInfo Venue ;
  if (false == Venue.initFromCapture(pCapt))
    return false ;

  return addNewVenue(&Venue) ;
}

bool
NSVenuesDocument::updateSelectedVenueFromCapture(NSSejourInfo *pVenue, NSCaptureArray* pCapt)
{
  if (((NSCaptureArray*) NULL == pCapt) || ((NSSejourInfo*) NULL == pVenue))
    return false ;

  if (pCapt->empty())
    return false ;

  // Make a copy before update
  //
  NSSejourInfo Venue = *pVenue ;

  // Update from capture
  //
  if (false == pVenue->initFromCapture(pCapt))
    return false ;

  // Compare original vs copy
  //
  if (pVenue->hasSameContent(Venue))
    return true ;

  // Register changes
  //
  return updateExistingVenue(pVenue) ;
}

NSSejourInfo*
NSVenuesDocument::getVenueForId(string sVenueId)
{
  if (_aVenuesArray.empty())
    return (NSSejourInfo*) 0 ;

  NSSejourIter itVenue = _aVenuesArray.begin() ;
  for ( ; _aVenuesArray.end() != itVenue ; itVenue++)
    if ((*itVenue)->getNumero() == sVenueId)
      return *itVenue ;

  return (NSSejourInfo*) 0 ;
}

void
NSVenuesDocument::invalidateViews(string sReason, string sReference)
{
	//
	// Valeurs possibles de sReason - Possible values for sReason
	//
	// MEMBER_NEW  MEMBER_REMOVED MEMBER_POS_CHANGED
	//
  // sReference can be ROSACE or LIST
  //
  TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
  	return ;

	do
	{
		// Rosace
		//
		NSVenuesView* pVenuesView = TYPESAFE_DOWNCAST(pView, NSVenuesView) ;
		if (pVenuesView)
			pVenuesView->reloadView(sReason, sReference) ;

		pView = NextView(pView) ;
	}
	while (pView) ;
}

// --------------------------------------------------------------------------
//
// Class NSVenuesView
//
// --------------------------------------------------------------------------

bool
sortVenueByIDSup(NSSejourInfo *pM1, NSSejourInfo *pM2)
{
	if (((NSSejourInfo*) NULL == pM1) || ((NSSejourInfo*) NULL == pM2))
		return false ;
	return (pM1->getNumero() > pM2->getNumero()) ;
}

bool
sortVenueByIDInf(NSSejourInfo *pM1, NSSejourInfo *pM2)
{
	if (((NSSejourInfo*) NULL == pM1) || ((NSSejourInfo*) NULL == pM2))
		return false ;
	return (pM1->getNumero() < pM2->getNumero()) ;
}

bool
sortVenueByUnitSup(NSSejourInfo *pM1, NSSejourInfo *pM2)
{
	if (((NSSejourInfo*) NULL == pM1) || ((NSSejourInfo*) NULL == pM2))
		return false ;
	return (pM1->getUnit() > pM2->getUnit()) ;
}

bool
sortVenueByUnitInf(NSSejourInfo *pM1, NSSejourInfo *pM2)
{
	if (((NSSejourInfo*) NULL == pM1) || ((NSSejourInfo*) NULL == pM2))
		return false ;
	return (pM1->getUnit() < pM2->getUnit()) ;
}

bool
sortVenueByBeginDateSup(NSSejourInfo *pM1, NSSejourInfo *pM2)
{
	if (((NSSejourInfo*) NULL == pM1) || ((NSSejourInfo*) NULL == pM2))
		return false ;
	return (pM1->getBeginDate() > pM2->getBeginDate()) ;
}

bool
sortVenueByBeginDateInf(NSSejourInfo *pM1, NSSejourInfo *pM2)
{
	if (((NSSejourInfo*) NULL == pM1) || ((NSSejourInfo*) NULL == pM2))
		return false ;
	return (pM1->getBeginDate() < pM2->getBeginDate()) ;
}

bool
sortVenueByEndDateSup(NSSejourInfo *pM1, NSSejourInfo *pM2)
{
	if (((NSSejourInfo*) NULL == pM1) || ((NSSejourInfo*) NULL == pM2))
		return false ;
	return (pM1->getEndDate() > pM2->getEndDate()) ;
}

bool
sortVenueByEndDateInf(NSSejourInfo *pM1, NSSejourInfo *pM2)
{
	if (((NSSejourInfo*) NULL == pM1) || ((NSSejourInfo*) NULL == pM2))
		return false ;
	return (pM1->getEndDate() < pM2->getEndDate()) ;
}

const int ID_HTList = 0x100 ;

const int CM_VENUE_NEW    = 0x110 ;
const int CM_VENUE_EDIT   = 0x111 ;
const int CM_VENUE_DELETE = 0x112 ;

// Table de réponses de la classe NSHealthTeamListView

DEFINE_RESPONSE_TABLE1(NSVenuesView, NSMUEView)
  EV_VN_ISWINDOW,
  EV_LVN_GETDISPINFO(ID_HTList, DispInfoListe),
  EV_LVN_COLUMNCLICK(ID_HTList, LVNColumnclick),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_VENUE_NEW,    CmAdd),
  EV_COMMAND(CM_VENUE_EDIT,   CmChange),
  EV_COMMAND(CM_VENUE_DELETE, CmClose),
END_RESPONSE_TABLE ;

// Constructeur
NSVenuesView::NSVenuesView(NSVenuesDocument& doc, TWindow *parent)
             :NSMUEView(doc.pContexte, &doc, parent, "VenuesList", "VenuesDoc")
{
try
{
  _pVenuesDoc    = &doc ;
  _pListeWindow  = new NSVenuesListWindow(this, ID_HTList, 0, 0, 0, 0) ;

	initMUEViewMenu("menubar_healthTeam") ;

  _pToolBar      = 0 ;
  _bSetupToolBar = true ;

  _iSortedColumn = -1 ;

	// string sText = pContexte->getSuperviseur()->getText("HealthTeamWindows", "newCorrespondent") ;
	// _pBtNew   = new NSGadgetButton(this, CM_GENERAL_ADD, sText.c_str(), 0, 0, 300, 20) ;
  _pBtNew   = 0 ;

  // sText = pContexte->getSuperviseur()->getText("HealthTeamWindows", "alterCorrespondent") ;
	// _pBtEdit  = new NSGadgetButton(this, CM_MEMBER_EDIT, sText.c_str(), 0, 0, 50, 15) ;
  _pBtEdit  = 0 ;

  // sText = pContexte->getSuperviseur()->getText("HealthTeamWindows", "removeCorrespondent") ;
	// _pBtClose = new NSGadgetButton(this, CM_GENERAL_CLOSE, sText.c_str(), 0, 0, 300, 20) ;
  _pBtClose = 0 ;

	setViewName() ;
}
catch (...)
{
  erreur("Exception NSHealthTeamListView ctor.", standardError, 0) ;
}
}

// Destructeur
NSVenuesView::~NSVenuesView()
{
  string ps = string("Deleting VenuesView.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

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
NSVenuesView::setViewName()
{
	_sViewName = pContexte->getSuperviseur()->getText("venuesWindows", "venuesManagement") ;

  addConcernTitle() ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow
*NSVenuesView::GetWindow()
{
  return (TWindow *) this ;
}

// Appel de la fonction de remplissage de la vuevoid
NSVenuesView::SetupWindow()
{
  NSMUEView::SetupWindow() ;
  Parent->SetCaption(_sViewName.c_str()) ;

  SetupColumns() ;
  AfficheListe() ;
}

void
NSVenuesView::CmAdd()
{
try
{
  NSSejourInfo Venue ;

  CreerSejourDialog *pNewVenueDlg = new CreerSejourDialog((TWindow *)this, pContexte, &Venue, pContexte->getSuperviseur()->getDefaultFctUnit()) ;
  int	iDialogReturn = pNewVenueDlg->Execute() ;
  delete pNewVenueDlg ;

  if (IDOK != iDialogReturn)
  	return ;

  _pVenuesDoc->addNewVenue(&Venue) ;
}
catch (...)
{
	erreur("Exception NSVenuesView::CmAdd.", standardError, 0) ;
}
}

void
NSVenuesView::CmChange()
{
  if (NULL == pContexte->getPatient())
  {
		string sErrorText = pContexte->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

  int iSelectedIndex = _pListeWindow->IndexItemSelect() ;

  if (iSelectedIndex < 0)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

  //
  // Get the venue information
  //
  vector<NSSejourInfo*>* pVenuesArray = _pVenuesDoc->getVenuesArray() ;
  if ((vector<NSSejourInfo*>*) NULL == pVenuesArray)
    return ;

  NSSejourInfo Venue = *((*pVenuesArray)[iSelectedIndex]) ;

  //
  // Ask for changes
  //
  CreerSejourDialog *pNewVenueDlg = new CreerSejourDialog((TWindow *)this, pContexte, &Venue, Venue.getUnit()) ;
  int	iDialogReturn = pNewVenueDlg->Execute() ;
  delete pNewVenueDlg ;

  if (IDOK != iDialogReturn)
  	return ;

  _pVenuesDoc->updateExistingVenue(&Venue) ;
}

void
NSVenuesView::CmClose()
{
}

void
NSVenuesView::reloadView(string sReason, string sReference)
{
  AfficheListe() ;
}

// Initialisation des colonnes de la ListWindowvoid
NSVenuesView::SetupColumns()
{
	string sID    = pContexte->getSuperviseur()->getText("venuesWindows", "venueIdentifier") ;
  string sFU    = pContexte->getSuperviseur()->getText("venuesWindows", "functionalUnit") ;
	string sBegin = pContexte->getSuperviseur()->getText("venuesWindows", "beginDate") ;
  string sEnd   = pContexte->getSuperviseur()->getText("venuesWindows", "endDate") ;

  _pListeWindow->InsertColumn(0, TListWindColumn((char*)sID.c_str(),    300, TListWindColumn::Left, 0)) ;
  _pListeWindow->InsertColumn(1, TListWindColumn((char*)sFU.c_str(),     50, TListWindColumn::Left, 1)) ;
  _pListeWindow->InsertColumn(2, TListWindColumn((char*)sBegin.c_str(), 120, TListWindColumn::Left, 2)) ;
  _pListeWindow->InsertColumn(3, TListWindColumn((char*)sEnd.c_str(),   120, TListWindColumn::Left, 3)) ;
}

// Affichage des éléments de la listevoid
NSVenuesView::AfficheListe()
{
	_pListeWindow->DeleteAllItems() ;

  vector<NSSejourInfo*>* pVenues = _pVenuesDoc->getVenuesArray() ;
  if ((NULL == pVenues) || (pVenues->empty()))
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NSSejourReverseIter itVenue = pVenues->rbegin() ;
  for ( ; pVenues->rend() != itVenue ; itVenue++)
  {
    TListWindItem Item(((*itVenue)->getNumero()).c_str(), 0) ;
    _pListeWindow->InsertItem(Item) ;
	}
}

void
NSVenuesView::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  vector<NSSejourInfo*>* pVenues = _pVenuesDoc->getVenuesArray() ;
  if ((NULL == pVenues) || (pVenues->empty()))
    return ;

  const int       BufLen = 255 ;
  static char     buffer[BufLen] ;
  buffer[0] = '\0' ;

  TListWindItem   &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

	string sText = string("") ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
  	case 1  : // unit
  		sText = ((*pVenues)[index])->getUnit() ;
      break ;

    case 2  : // date début
    {
    	NVLdVTemps tpsBegin ;
      tpsBegin.initFromDate(((*pVenues)[index])->getBeginDate()) ;
      sText = (tpsBegin.donneFormattedDateHeure(pContexte, sLang)).c_str() ;
    }
      break ;

    case 3  : // date fin    {      NVLdVTemps tpsEnd ;      tpsEnd.initFromDateHeure(((*pVenues)[index])->getEndDate()) ;      if (tpsEnd.estNoLimit())      	sText = pContexte->getSuperviseur()->getText("HealthTeamWindows", "notimelimit") ;      else
      {
      	tpsEnd.initFromDate(((*pVenues)[index])->getEndDate()) ;
        sText = (tpsEnd.donneFormattedDateHeure(pContexte, sLang)).c_str() ;
      }
    }      break ;
  }

  sprintf(buffer, "%s", sText.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
NSVenuesView::LVNColumnclick(TLwNotify& lwn)
{
	changeSortOrder(lwn.iSubItem) ;

  switch ( lwn.iSubItem )
  {
    case 0  : sortByVenueId() ;      break ;
    case 1  : sortByUnit() ;         break ;
    case 2  : sortByDateEnding() ;   break ;
    case 3  : sortByDateBegining() ; break ;
  }
}

void
NSVenuesView::changeSortOrder(int iOrder)
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
NSVenuesView::sortByVenueId()
{
  vector<NSSejourInfo*>* pVenues = _pVenuesDoc->getVenuesArray() ;
  if ((NULL == pVenues) || (pVenues->empty()))
    return ;

  if (_bNaturallySorted)
    sort(pVenues->begin(), pVenues->end(), sortVenueByIDInf) ;
  else
    sort(pVenues->begin(), pVenues->end(), sortVenueByIDSup) ;

  AfficheListe() ;
}

void
NSVenuesView::sortByUnit()
{
  vector<NSSejourInfo*>* pVenues = _pVenuesDoc->getVenuesArray() ;
  if ((NULL == pVenues) || (pVenues->empty()))
    return ;

  if (_bNaturallySorted)
    sort(pVenues->begin(), pVenues->end(), sortVenueByUnitInf) ;
  else
    sort(pVenues->begin(), pVenues->end(), sortVenueByUnitSup) ;

  AfficheListe() ;
}

void
NSVenuesView::sortByDateEnding()
{
  vector<NSSejourInfo*>* pVenues = _pVenuesDoc->getVenuesArray() ;
  if ((NULL == pVenues) || (pVenues->empty()))
    return ;

  if (_bNaturallySorted)
    sort(pVenues->begin(), pVenues->end(), sortVenueByEndDateInf) ;
  else
    sort(pVenues->begin(), pVenues->end(), sortVenueByEndDateSup) ;

  AfficheListe() ;
}

void
NSVenuesView::sortByDateBegining()
{
  vector<NSSejourInfo*>* pVenues = _pVenuesDoc->getVenuesArray() ;
  if ((NULL == pVenues) || (pVenues->empty()))
    return ;

  if (_bNaturallySorted)
    sort(pVenues->begin(), pVenues->end(), sortVenueByBeginDateInf) ;
  else
    sort(pVenues->begin(), pVenues->end(), sortVenueByBeginDateSup) ;

  AfficheListe() ;
}

boolNSVenuesView::VnIsWindow(HWND hWnd){
  return (HWindow == hWnd) ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindowvoid
NSVenuesView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
	_pListeWindow->MoveWindow(GetClientRect(), true) ;
}

// fonction EVSetFocusvoid
NSVenuesView::EvSetFocus(HWND hWndLostFocus)
{
	NSMUEView::EvSetFocus(hWndLostFocus);

  focusFct() ;

	_pListeWindow->SetFocus() ;
}

// fonction Click droit : menu contextuel
void
NSVenuesView::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point ;

	TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sEdit  = pSuper->getText("venuesWindows", "alterVenue") ;
	string sNew   = pSuper->getText("venuesWindows", "newVenue") ;
	string sDel   = pSuper->getText("venuesWindows", "removeVenue") ;

  int	index = _pListeWindow->IndexItemSelect() ;
	if (index != -1)
	{
		menu->AppendMenu(MF_STRING, CM_VENUE_EDIT,   sEdit.c_str()) ;
		menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, CM_VENUE_NEW,   sNew.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, CM_VENUE_DELETE, sDel.c_str()) ;
	}
	else
	{
		menu->AppendMenu(MF_STRING, CM_VENUE_NEW, sNew.c_str()) ;
	}

	ClientToScreen(lp) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
	delete menu ;
}

void
NSVenuesView::EvRButtonDownOut(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
  NS_CLASSLIB::TPoint lp = point ;

  TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sNew = pSuper->getText("venuesWindows", "newVenue") ;
  menu->AppendMenu(MF_STRING, CM_VENUE_NEW, sNew.c_str()) ;

  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}

void
NSVenuesView::focusFct()
{
	activateMUEViewMenu() ;

  TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
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
NSVenuesView::SetupToolBar()
{
/*
	TMyApp  *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->FlushControlBar() ;

	pMyApp->cb2->Insert(*new TButtonGadget(CM_GENERAL_ADD,   CM_GENERAL_ADD,   TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(CM_GENERAL_CLOSE, CM_GENERAL_CLOSE, TButtonGadget::Command)) ;

  pMyApp->cb2->LayoutSession() ;
*/

	_pPaneSplitter->FlushControlBar() ;

	// FlushControlBar destroys the buttons... we need to create them after
	//
  string sText = pContexte->getSuperviseur()->getText("venuesWindows", "newVenue") ;
	_pBtNew   = new NSGadgetButton(this, CM_GENERAL_ADD, sText.c_str(), 0, 0, 300, 20) ;
  sText = pContexte->getSuperviseur()->getText("venuesWindows", "removeVenue") ;
	_pBtClose = new NSGadgetButton(this, CM_GENERAL_CLOSE, sText.c_str(), 0, 0, 300, 20) ;

  _pPaneSplitter->Insert(*new TControlGadget(*_pBtNew)) ;
	_pPaneSplitter->Insert(*new TSeparatorGadget) ;
  _pPaneSplitter->Insert(*new TControlGadget(*_pBtClose)) ;

  _pPaneSplitter->LayoutSession() ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSHealthTeamListWindow//
// -----------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(NSVenuesListWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  // EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSVenuesListWindow::NSVenuesListWindow(NSVenuesView *parent, int id, int x, int y, int w, int h, TModule *module)
                   :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  _pView = parent ;
  _iRes  = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

void
NSVenuesListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSVenuesListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  	_pView->CmChange() ;
  else
  	_pView->CmAdd() ;
}

void
NSVenuesListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    _pView->EvRButtonDown(modkeys, point) ;
  }
  else
  	_pView->EvRButtonDownOut(modkeys, point) ;
}

void
NSVenuesListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
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
NSVenuesListWindow::IndexItemSelect()
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
NSVenuesListWindow::EvSetFocus(HWND hWndLostFocus)
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
NSVenuesListWindow::EvKillFocus(HWND hWndGetFocus)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
  SetTextBkColor(0x00ffffff) ;
  Invalidate();
}

