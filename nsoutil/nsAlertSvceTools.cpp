// NSMailSvce.CPP : Incoming messages processing
////////////////////////////////////////////////////////////
// #include <cstring.h>
// #include <stdio.h>// #include <assert.h>// #include <vcl/controls.hpp>// #include <commctrl.h>// #include <owl/treewind.h>
// #include <vcl.h>// #pragma hdrstop// #include "ActiveX.h"

#include "nautilus\nsAlertSvce.h"
#include "nsoutil\nsAlertSvceTools.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "nsbb\nslistwind.h"#include "nautilus\nsepicap.h"#include "nautilus\ns_html.h"#include "nautilus\nsdocview.h"#include "nautilus\nsbrowse.h"#include "nssavoir\nsFileCaptur.h"#include "nsldv\nsldvuti.h"#include "partage\nsdivfile.h"#include "nsldv\nsldvdrug.h"#include "nsepisod\nsToDo.h"// #include "nautilus\psapi.h"#include "nsoutil\nsoutil.h"#include "nsoutil\nsAlertBox.rh" /****************** classe NSImportWindow **************************/
DEFINE_RESPONSE_TABLE1(NSAlertListWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_LBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  NS_LW_COLUMNCLICK(LVNColumnclick),
END_RESPONSE_TABLE ;

NSAlertListWindow::NSAlertListWindow(NSContexte* pCtx, NSAlertServiceWindow *parent, int id, int x, int y, int w, int h, TModule *module)
                 :TListWindow((TWindow *) parent, id, x, y, w, h, module),
                  NSRoot(pCtx)
{
  _pParentWindow = parent ;
  _iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

	_iPreviousSelection = -1 ;
  _bNaturallySorted   = true ;
}

NSAlertListWindow::~NSAlertListWindow()
{
	if (_pImages)
		delete _pImages ;
}

void
NSAlertListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES) ;
  TListWindow::SetupWindow() ;

	// HINSTANCE hInstModule = *GetApplication() ;
  // HINSTANCE hInstModule = pNSResModule->GetHandle() ;

	_pImages = new OWL::TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR4, 15, 5) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, DRUGINFO)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, DRUGALERT1)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, DRUGALERT2)) ;
  _pImages->Add(OWL::TBitmap(*pNSResModule, DRUGALERT3)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, DRUGALERT4)) ;
	SetImageList(*_pImages, TListWindow::State) ;

  SetupColumns() ;

  loadMessages() ;

  sortByAlert() ;

  selectFirstMessage() ;
}

void
NSAlertListWindow::Reload()
{
  loadMessages() ;
  sortByAlert() ;
  selectFirstMessage() ;
}

void
NSAlertListWindow::loadMessages()
{
  _aAlerts.vider() ;

  // Get drugs array
  //
  ArrayCopyDrugs* pDrugs = _pParentWindow->getDrugs() ;
  if (((ArrayCopyDrugs*) NULL == pDrugs) || pDrugs->empty())
    return ;

  // Get the alerts, that are inside the Bam drugs, inside the drugs
  //
  for (drugsIter itDrug = pDrugs->begin() ; pDrugs->end() != itDrug ; itDrug++)
  {
    NsSelectableDrug* pSelectableDrug = (*itDrug)->getBamDrug() ;
    if (pSelectableDrug)
    {
      NsDrugAlertArray* pAlerts = pSelectableDrug->getAlerts() ;
      if (pAlerts && (false == pAlerts->empty()))
        _aAlerts.addTo(pAlerts) ;
    }
  }
}

void
NSAlertListWindow::processSelectionChange()
{
	// Don't do that, because if an entry is removed, iCurrentSelection is -1
	int iCurrentSelection = IndexItemSelect() ;
	if (iCurrentSelection < 0)
		return ;

	// Don't do that, because if an entry is removed, select index doesn't change
	//if (iPreviousSelection == iCurrentSelection)
 	//	return ;

	_iPreviousSelection = iCurrentSelection ;

  NsDrugAlert* pSelectedAlert = _aAlerts[_iPreviousSelection] ;

  // Ask the view to display
	displayMessage() ;

/*
	// on construit un html temporaire dans le répertoire FPER
	string sPathHtml = pContexte->PathName("FPER") ;

	NSHtmlTXT html(toImporter, 0, sFileName.c_str(), pContexte) ;

	// on trouve le nom du fichier temporaire à visualiser
  string sNomSeul = string("mailBox") ;
	string sHtml = html.nomSansDoublons(sPathHtml, sNomSeul, "htm") ;
	sHtml = sPathHtml + sHtml ;

	// on passe la template des documents TXT
	string sTemplateHtml = pContexte->PathName("NTPL") + string("doctxt.htm") ;

  string sTypeNautilus = string("ZTTXT") ;
	// generation du fichier html (dans le repertoire FPER)
	if (!html.genereHtml(sTemplateHtml, sHtml, sDirectory, pSelectedMail->sFileName, sTypeNautilus))
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "errorCreatingImportHtmlFile") ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return ;
	}

	// Ask the view to navigate
	if (NULL == pParentWindow->getViewer())
  	return ;

	pParentWindow->getViewer()->Navigate(sHtml) ;
*/
}

void
NSAlertListWindow::selectFirstMessage()
{
	if (_aAlerts.empty())
    return ;

  size_t iIndexNumber = 0 ;

  SetItemState(iIndexNumber, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;
  EnsureVisible(iIndexNumber, false) ;

  // Invalidate() ;

  processSelectionChange() ;
}

void
NSAlertListWindow::SetupColumns()
{
	string sTitle    = pContexte->getSuperviseur()->getText("drugAlertWindow", "title") ;
  string sType     = pContexte->getSuperviseur()->getText("drugAlertWindow", "alertType") ;
  string sCategory = pContexte->getSuperviseur()->getText("drugAlertWindow", "alertCategory") ;
  string sMessage  = pContexte->getSuperviseur()->getText("drugAlertWindow", "alertMessage") ;

  InsertColumn(0, TListWindColumn("!", 20, TListWindColumn::Left, 0)) ;
  InsertColumn(1, TListWindColumn((char*)sTitle.c_str(), 300, TListWindColumn::Left, 1)) ;
  InsertColumn(2, TListWindColumn((char*)sType.c_str(), 50, TListWindColumn::Left, 2)) ;
  InsertColumn(3, TListWindColumn((char*)sCategory.c_str(), 50, TListWindColumn::Left, 3)) ;
  InsertColumn(4, TListWindColumn((char*)sMessage.c_str(), 300, TListWindColumn::Left, 4)) ;
}

// Affichage des éléments de la liste
void
NSAlertListWindow::AfficheListe()
{
  string sTrace = string("NSAlertListWindow::AfficheListe: Entering.") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	DeleteAllItems() ;

  if (_aAlerts.empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NsDrugAlertReverseIter itrAlert = _aAlerts.rbegin() ;
  do
  {
    string sPatName = (*itrAlert)->getProductLabel() ;
    TListWindItem Item(sPatName.c_str(), 0) ;

    // Col 0
    //
    switch ((*itrAlert)->getSeverity())
    {
/*
    	case NsDrugAlert::severityInfo :
      	Item.SetStateImage(0) ;
        break ;
*/
      case NsDrugAlert::severityLevel1 :
      	Item.SetStateImage(1) ;
        break ;
      case NsDrugAlert::severityLevel2 :
      	Item.SetStateImage(2) ;
        break ;
      case NsDrugAlert::severityLevel3 :
      	Item.SetStateImage(3) ;
        break ;
      case NsDrugAlert::severityLevel4 :
      	Item.SetStateImage(4) ;
        break ;
    }
    InsertItem(Item) ;

    itrAlert++ ;
	}
  while (itrAlert != _aAlerts.rend()) ;

  int i = 0 ;
	for (NsDrugAlertIter itAlert = _aAlerts.begin() ; _aAlerts.end() != itAlert ; i++, itAlert++)
	{
    // Col 1
    //
    // string sProductName = (*itAlert)->getProductLabel() ;
    string sProductName = (*itAlert)->getTitle() ;
    TListWindItem Item1(sProductName.c_str(), 1) ;
    Item1.SetIndex(i) ;
    Item1.SetSubItem(1) ;
    SetItem(Item1) ;

    // Col 2
    //
    string sAlertType = string("") ;

    switch ((*itAlert)->getSeverity())
    {
    	case NsDrugAlert::severityInfo :
      	sAlertType = pContexte->getSuperviseur()->getText("drugAlertWindow", "typeInfo") ;
        break ;
      case NsDrugAlert::severityLevel1 :
      	sAlertType = pContexte->getSuperviseur()->getText("drugAlertWindow", "typeAlert1") ;
        break ;
      case NsDrugAlert::severityLevel2 :
      	sAlertType = pContexte->getSuperviseur()->getText("drugAlertWindow", "typeAlert2") ;
        break ;
      case NsDrugAlert::severityLevel3 :
      	sAlertType = pContexte->getSuperviseur()->getText("drugAlertWindow", "typeAlert3") ;
        break ;
      case NsDrugAlert::severityLevel4 :
      	sAlertType = pContexte->getSuperviseur()->getText("drugAlertWindow", "typeAlert4") ;
        break ;
    }

    TListWindItem Item2(sAlertType.c_str(), 2) ;
    Item1.SetIndex(i) ;
    Item1.SetSubItem(2) ;
    SetItem(Item2) ;

    // Col 3
    //
/*
    TListWindItem Item3("", 3) ;
    szType[0] = '\0' ;

    size_t iErrorLevel = (*itMail)->getProcessErrorLevel() ;
    if (iErrorLevel > 0)
    {
    	Item3.SetImageIndex(9) ;
      strcpy(szType, "~") ;
    }

    Item3.SetText(szType) ;
    Item3.mask = LVIF_IMAGE | LVIF_TEXT ;
    Item3.SetIndex(i) ;
    Item3.SetSubItem(3) ;
    SetItem(Item3) ;
*/
    // Col 4
    //
    string sContent = texteCourant((*itAlert)->getContent()) ;

    TListWindItem Item4(sContent.c_str(), 4) ;

    Item4.SetIndex(i) ;
    Item4.SetSubItem(4) ;
    SetItem(Item4) ;
	}

  sTrace = string("NSAlertListWindow::AfficheListe: Exiting.") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
}

void
NSAlertListWindow::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  const int       BufLen = 255 ;
  static char     buffer[BufLen] ;
  TListWindItem   &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  // char            szDate[25] ;
  // char            szDateDisplay[25] ;
  // char            szHour[25] ;
  string          sHourDisplay ;
  buffer[0] = '\0' ;

  // int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

/*
  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case 1  :
    {
    	string sPatName = (_aAlerts[index])->getDisplayName() ;
    	dispInfoItem.SetText(sPatName.c_str()) ;
      break ;
    }

  	case 2  :
    {
    	switch ((_aAlerts[index])->iType)
    	{
    		case FILETYPE_HPRIM1 :
    		case FILETYPE_HPRIM1_MULTI :
      		dispInfoItem.SetImageIndex(0) ;
        	break ;
      	case FILETYPE_HPRIM2 :
    		case FILETYPE_HPRIM2_MULTI :
      		dispInfoItem.SetImageIndex(1) ;
        	break ;
      	case FILETYPE_UNKNOWN :
      		dispInfoItem.SetImageIndex(2) ;
        	break ;
    	}
      dispInfoItem.mask = LVIF_IMAGE ;
      break ;
    }

    case 3  :
    {
    	string sProcess = (_aAlerts[index])->sProcessLog ;
      if (sProcess != string(""))
      {
    		size_t pos = sProcess.find("Error") ;
      	if (pos != NPOS)
      		dispInfoItem.SetImageIndex(10) ;
      }
      break ;
    }

    case 4  : // date début
    	strcpy(szDate, (_aAlerts[index])->sExamDate.c_str()) ;
      donne_date(szDate, szDateDisplay, sLang) ;

      dispInfoItem.SetText(szDateDisplay) ;
      break ;

*/
/*
    case 2  : // date fichier
    {
    	strcpy(szDate, (_aAlerts[index])->sReceptionDate.c_str()) ;

      // Get hour
      int i ;
      for (i = 0; i < 6; i++)
      	szHour[i] = szDate[i+8] ;
      szHour[i] = '\0' ;
      donne_heure(szHour, sHourDisplay) ;

      // Get date
      szDate[8] = '\0' ;
      donne_date(szDate, szDateDisplay, sLang) ;

      strcat(szDateDisplay, " ") ;
      strcat(szDateDisplay, sHourDisplay.c_str()) ;

      dispInfoItem.SetText(szDateDisplay) ;
      break ;
    }
*/
//  }
}

void
NSAlertListWindow::displayMessage()
{
	if (((NSAlertServiceWindow*)   NULL == _pParentWindow) ||
      ((NSTxtAlertViewerWindow*) NULL == _pParentWindow->getTxtViewer()))
  	return ;

	NsDrugAlert* pSelectedAlert = _aAlerts[_iPreviousSelection] ;
  if ((NsDrugAlert*) NULL == pSelectedAlert)
    return ;

  string sAlertContent = pSelectedAlert->getContent() ;
  string sContentType  = pseumaj(pSelectedAlert->getContentType()) ;
  string sTxtContent   = texteCourant(sAlertContent) ;

  string sAlertDetail  = pSelectedAlert->getDetail() ;
  string sDetailType   = pseumaj(pSelectedAlert->getDetailType()) ;
  string sTxtDetail    = texteCourant(sAlertDetail) ;

  if (((string("TEXT/HTML") == sDetailType) || (string("TEXT/HTML") == sContentType)) &&
            ((sAlertDetail != sTxtDetail) || (sAlertContent != sTxtContent)))
  {
    // NSHtmlAlertViewerWindow* pHtmlViewer = _pParentWindow->getHtmlViewer() ;
    // if ((NSHtmlAlertViewerWindow*) NULL != pHtmlViewer)
    //   pHtmlViewer->display(sAlertDetail) ;
    //
    // _pParentWindow->displayHtml(sAlertDetail) ;
    //
    NSToDoTask* pTask = new NSToDoTask ;
    pTask->setWhatToDo(string("DrugAlertDisplayHtml")) ;
    pTask->setParam1(sAlertContent) ;
    pTask->setParam2(sAlertDetail) ;
    pTask->sendMe(pContexte->getSuperviseur()) ;
  }
  else
  {
    // NSTxtAlertViewerWindow* pTxtViewer = _pParentWindow->getTxtViewer() ;
    // if ((NSTxtAlertViewerWindow*) NULL != pTxtViewer)
    //   pTxtViewer->displayText(&sAlertDetail) ;
    NSToDoTask* pTask = new NSToDoTask ;
    pTask->setWhatToDo(string("DrugAlertDisplayTxt")) ;
    pTask->setParam1(sAlertContent) ;
    pTask->setParam2(sAlertDetail) ;
    pTask->sendMe(pContexte->getSuperviseur()) ;
  }
}

/*
void
NSAlertListWindow::reinitMailList()
{
	_aAlerts.vider() ;
	initMailList() ;
  sortAgain() ;
  AfficheListe() ;
}
*/

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSAlertListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  processSelectionChange() ;

  //if (info.GetFlags() & LVHT_ONITEM)  //	pView->CmModifier() ;
  //else
  //	pView->CmNouveau() ;
}

void
NSAlertListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    processSelectionChange() ;    NsDrugAlert* pSelectedAlert = _aAlerts[indexItem] ;  }  else
		TListWindow::EvRButtonDown(modkeys, point) ;
}

void
NSAlertListWindow::EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TListWindow::EvLButtonDown(modkeys, point) ;

	processSelectionChange() ;}
void
NSAlertListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
/*
	if      (key == VK_DELETE)
  	pView->CmSupprimer() ;
  else if (key == VK_INSERT)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	pView->CmModifier() ;
    else
    	pView->CmNouveau() ;
  }
  else if (key == VK_TAB)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	pView->setFocusToPrevSplitterView() ;
    else
    	pView->setFocusToNextSplitterView() ;
  }
  else
*/
  	TListWindow::EvKeyDown(key, repeatCount, flags) ;

	processSelectionChange() ;
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSAlertListWindow::IndexItemSelect()
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
NSAlertListWindow::sortAgain()
{
	switch (_iSortedColumn)
  {
  	case 0 : sortByName() ; break ;
  	case 1 : sortByAlert() ; break ;
	}
}

void
NSAlertListWindow::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
		case 0  :
    	changeAlertSortOrder() ;
    	sortByAlert() ;
      break ;
    case 1  :
    	changeNameSortOrder() ;
    	sortByName() ;
      break ;
  }
}

void
NSAlertListWindow::changeSortOrder(int iCol)
{
	if (_iSortedColumn == iCol)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = iCol ;
    _bNaturallySorted = true ;
  }
}

void
NSAlertListWindow::sortByAlert()
{
  if (_aAlerts.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aAlerts.begin(), _aAlerts.end(), mailObjSortByAlertSup) ;
  else
    sort(_aAlerts.begin(), _aAlerts.end(), mailObjSortByAlertInf) ;

  AfficheListe() ;
}

void
NSAlertListWindow::sortByName()
{
  if (_aAlerts.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aAlerts.begin(), _aAlerts.end(), mailObjSortByNameInf) ;
  else
    sort(_aAlerts.begin(), _aAlerts.end(), mailObjSortByNameSup) ;

  AfficheListe() ;
}

void
NSAlertListWindow::EvSetFocus(HWND hWndLostFocus)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
  SetTextBkColor(0x00fff0f0) ;
  Invalidate();

  // pView->focusFct() ;

  int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSAlertListWindow::EvKillFocus(HWND hWndGetFocus)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
  SetTextBkColor(0x00ffffff) ;
  Invalidate() ;
}

// -----------------------------------------------------------------------------
//
//   NSAlertElementsViewerWindow//
// -----------------------------------------------------------------------------
NSAlertElementsViewerWindow::NSAlertElementsViewerWindow(NSContexte* pCtx, NSAlertServiceWindow *parent, int id, int /*x*/, int /*y*/, int /*w*/, int /*h*/)
                            :OWL::TTreeWindow(parent, id, 0, 0, 0, 0), NSRoot(pCtx)
{
	_pParentWindow = parent ;

  // TWindow::ModifyStyle(uint32 offBits, uint32 onBits, uint swpFlags)
  //
  ModifyStyle(0, TVS_HASLINES) ;
}

NSAlertElementsViewerWindow::~NSAlertElementsViewerWindow()
{
}

void
NSAlertElementsViewerWindow::SetupWindow()
{
  OWL::TTreeWindow::SetupWindow() ;
}

void
NSAlertElementsViewerWindow::displayInformation(const NSPrescriptionCheckingMessage* pCheckInfo)
{
  if (GetItemCount() > 0)
    DeleteAllItems() ;

	if ((NSPrescriptionCheckingMessage*) NULL == pCheckInfo)
		return ;

  OWL::TTreeNode rootNode = GetRoot() ;
  // OWL::TTreeNode rootNode(*this, (HTREEITEM) TVI_ROOT) ;

  // Patient information
  //
  string sPatInfoText = pContexte->getSuperviseur()->getText(string("drugAlertWindow"), string("patientInformation")) ;
  OWL::TTreeNode patientNode(*this, (const char far*) sPatInfoText.c_str(), 0, 0) ;
  patientNode = rootNode.AddChild(patientNode) ;

  NSPresCheckPatientIntformation* pPatientInfo = pCheckInfo->getPatientInformation() ;
  if (pPatientInfo)
  {
    // Patient's way of life
    //
    string sBasicInfoText = pContexte->getSuperviseur()->getText(string("drugAlertWindow"), string("riskInformation")) ;
    OWL::TTreeNode basicNode(*this, (const char far*) sBasicInfoText.c_str()) ;
    basicNode = patientNode.AddChild(basicNode) ;

    NSBdmEntryArray* pBasicInfo = pPatientInfo->getBasicInformation() ;
    if (pBasicInfo && (false == pBasicInfo->empty()))
    {
      for (NSBdmEntryConstIter it = pBasicInfo->begin() ; pBasicInfo->end() != it ; it++)
      {
        string sLabel = (*it)->getLabel() ;

        if (string("") == sLabel)
        {
          sLabel = pContexte->getSuperviseur()->getText(string("drugAlertWindow"), (*it)->getID()) ;
          if (string("") != sLabel)
            sLabel += string(" : ") + (*it)->getCode() ;
        }

        if (string("") != sLabel)
        {
          OWL::TTreeNode infoNode(*this, (const char far*) sLabel.c_str()) ;
          basicNode.AddChild(infoNode) ;
        }
      }

      basicNode.ExpandItem(TVE_EXPAND) ;
    }

    // Allergies
    //
    string AllergyText = pContexte->getSuperviseur()->getText(string("drugAlertWindow"), string("allergies")) ;
    OWL::TTreeNode allergyNode(*this, (const char far*) AllergyText.c_str()) ;
    allergyNode = patientNode.AddChild(allergyNode) ;

    NSBdmEntryArray* pAllergyInfo = pPatientInfo->getAllergies() ;
    if (pAllergyInfo && (false == pAllergyInfo->empty()))
    {
      for (NSBdmEntryConstIter it = pAllergyInfo->begin() ; pAllergyInfo->end() != it ; it++)
      {
      }
    }

    // Allergies aux molécules
    //
    string MoleculesText = pContexte->getSuperviseur()->getText(string("drugAlertWindow"), string("molecules")) ;
    OWL::TTreeNode moleculesNode(*this, (const char far*) MoleculesText.c_str()) ;
    moleculesNode = patientNode.AddChild(moleculesNode) ;

    NSBdmEntryArray* pMoleculesInfo = pPatientInfo->getMolecules() ;
    if (pMoleculesInfo && (false == pMoleculesInfo->empty()))
    {
      for (NSBdmEntryConstIter it = pMoleculesInfo->begin() ; pMoleculesInfo->end() != it ; it++)
      {
        OWL::TTreeNode moleculeNode(*this, (const char far*) (*it)->getLabel().c_str()) ;
        moleculesNode.AddChild(moleculeNode) ;
      }
      moleculesNode.ExpandItem(TVE_EXPAND) ;
    }

    // Pathologies
    //
    string PathologiesText = pContexte->getSuperviseur()->getText(string("drugAlertWindow"), string("pathologies")) ;
    OWL::TTreeNode pathologiesNode(*this, (const char far*) PathologiesText.c_str()) ;
    pathologiesNode = patientNode.AddChild(pathologiesNode) ;

    NSBdmEntryArray* pPathologiesInfo = pPatientInfo->getPathologies() ;
    if (pPathologiesInfo && (false == pPathologiesInfo->empty()))
    {
      for (NSBdmEntryConstIter it = pPathologiesInfo->begin() ; pPathologiesInfo->end() != it ; it++)
      {
        OWL::TTreeNode pathologyNode(*this, (const char far*) (*it)->getLabel().c_str()) ;
        pathologiesNode.AddChild(pathologyNode) ;
      }
      pathologiesNode.ExpandItem(TVE_EXPAND) ;
    }
  }
  patientNode.ExpandItem(TVE_EXPAND) ;

  // Prescription lines
  //
  string sPrescrLinesText = pContexte->getSuperviseur()->getText(string("drugAlertWindow"), string("prescriptionLines")) ;
  OWL::TTreeNode prescrNode(*this, (const char far*) sPrescrLinesText .c_str(), 0, 0) ;
  prescrNode = rootNode.AddChild(prescrNode) ;

  NSPresLineArray* pLines = pCheckInfo->getPrescriptionLines() ;
  if (pLines && (false == pLines->empty()))
  {
    for (NSPresLineIter it = pLines->begin() ; pLines->end() != it ; it++)
    {
      OWL::TTreeNode drugNode(*this, (const char far*) (*it)->getDrugLabel().c_str()) ;
      drugNode = prescrNode.AddChild(drugNode) ;

      if (string("") != (*it)->getDose())
      {
        string sDoseLabel = (*it)->getDose() + string(" ") + (*it)->getDoseUnitAsText(pContexte) + string(" ") + (*it)->getFrequencyTypeAsText(pContexte) ;
        OWL::TTreeNode drugDoseNode(*this, (const char far*) sDoseLabel.c_str()) ;
        drugNode.AddChild(drugDoseNode) ;
      }
      if (string("") != (*it)->getDuration())
      {
        string sDurationLabel = (*it)->getDuration() + string(" ") + (*it)->getDurationTypeAsText(pContexte) ;
        OWL::TTreeNode drugDurationNode(*this, (const char far*) sDurationLabel.c_str()) ;
        drugNode.AddChild(drugDurationNode) ;
      }
      drugNode.ExpandItem(TVE_EXPAND) ;
    }
  }
  prescrNode.ExpandItem(TVE_EXPAND) ;
}

// -----------------------------------------------------------------------------
//
//   NSTxtAlertViewerWindow//
// -----------------------------------------------------------------------------
NSTxtAlertViewerWindow::NSTxtAlertViewerWindow(NSContexte* pCtx, NSAlertServiceWindow *parent, int id, int /*x*/, int /*y*/, int /*w*/, int /*h*/, TModule* /*module*/)
                       :OWL::TEdit(parent, id, "", 0, 0, 0, 0, 0, true /* multiline */), NSRoot(pCtx)
{
	_pParentWindow = parent ;

  // TWindow::ModifyStyle(uint32 offBits, uint32 onBits, uint swpFlags)
  //
  ModifyStyle(WS_HSCROLL | ES_AUTOHSCROLL, /* ES_MULTILINE | */ WS_CHILD | WS_VSCROLL | ES_READONLY) ;
}

NSTxtAlertViewerWindow::~NSTxtAlertViewerWindow()
{
}

void
NSTxtAlertViewerWindow::SetupWindow()
{
  NS_CLASSLIB::TRect frmtRect ;
  GetRect(frmtRect) ;
  SetRectNP(frmtRect) ;
}

void
NSTxtAlertViewerWindow::displayText(string* pTxtContent, string* pTxtDetail)
{
	if ((string*) NULL == pTxtContent)
		return ;

  if ((string("") == *pTxtContent) && (((string*) NULL == pTxtDetail) ||
                                           (string("") == *pTxtDetail)))
  {
    SetText("") ;
    return ;
  }

  string sTxt = *pTxtContent ;

  if (((string*) NULL == pTxtDetail) || (string("") == *pTxtDetail))
  {
    SetText(sTxt.c_str()) ;
    return ;
  }

  string sNewLineForEdit = string("\r\n") ;

  sTxt += sNewLineForEdit + sNewLineForEdit + *pTxtDetail ;

  // Cut the text according to "-" (primary bullet points level)
  //
  size_t iPos = sTxt.find(". -") ;
  if (NPOS != iPos)
  {
    while (NPOS != iPos)
    {
      sTxt = string(sTxt, 0, iPos + 1) + sNewLineForEdit + string(sTxt, iPos+2, strlen(sTxt.c_str()) - iPos - 2) ;
      iPos = sTxt.find(". -", iPos + 2) ;
    }
  }

  // Cut the text according to ":." (secondary bullet points level)
  //
  iPos = sTxt.find(": .") ;
  if (NPOS != iPos)
  {
    while (NPOS != iPos)
    {
      sTxt = string(sTxt, 0, iPos + 1) + sNewLineForEdit + string("   ") + string(sTxt, iPos+2, strlen(sTxt.c_str()) - iPos - 2) ;
      iPos = sTxt.find(": .", iPos + 2) ;
    }
  }

  // Cut the text according to "." (secondary bullet points level)
  //
  iPos = sTxt.find(", .") ;
  if (NPOS != iPos)
  {
    while (NPOS != iPos)
    {
      sTxt = string(sTxt, 0, iPos + 1) + sNewLineForEdit + string("   ") + string(sTxt, iPos+2, strlen(sTxt.c_str()) - iPos - 2) ;
      iPos = sTxt.find(", .", iPos + 2) ;
    }
  }

	SetText(sTxt.c_str()) ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSNewMailObject//
// -----------------------------------------------------------------------------
bool
mailObjSortByAlertInf(NsDrugAlert *pObj1, NsDrugAlert *pObj2)
{
  if (((NsDrugAlert*) NULL == pObj1) || ((NsDrugAlert*) NULL == pObj2))
    return false ;

	return (pObj1->getSeverity() < pObj2->getSeverity()) ;
}

bool
mailObjSortByAlertSup(NsDrugAlert *pObj1, NsDrugAlert *pObj2)
{
  if (((NsDrugAlert*)NULL == pObj1) || ((NsDrugAlert*)NULL == pObj2))
    return false ;

	return (pObj1->getSeverity() > pObj2->getSeverity()) ;
}

bool
mailObjSortByNameInf(NsDrugAlert *pObj1, NsDrugAlert *pObj2)
{
  if (((NsDrugAlert*) NULL == pObj1) || ((NsDrugAlert*) NULL == pObj2))
    return false ;

  if (pObj1->getProductLabel() == pObj2->getProductLabel())
    return mailObjSortByAlertSup(pObj1, pObj2) ;

	return (pObj1->getProductLabel() < pObj2->getProductLabel()) ;
}

bool
mailObjSortByNameSup(NsDrugAlert *pObj1, NsDrugAlert *pObj2)
{
  if (((NsDrugAlert*) NULL == pObj1) || ((NsDrugAlert*) NULL == pObj2))
    return false ;

  if (pObj1->getProductLabel() == pObj2->getProductLabel())
    return mailObjSortByAlertSup(pObj1, pObj2) ;

	return (pObj1->getProductLabel() > pObj2->getProductLabel()) ;
}

/**
 *				   		  Classe ArrayCopyDrugAlerts
 *		   copy pointers array : never make any delete
 */

ArrayCopyDrugAlerts::ArrayCopyDrugAlerts(const ArrayCopyDrugAlerts& rv)
                    :NsDrugAlertsVector()
{
try
{
	if (false == rv.empty())
		for (NsDrugAlertConstIter i = rv.begin(); rv.end() != i ; i++)
			push_back(*i) ;
}
catch (...)
{
	erreur("Exception ArrayCopyDrugAlerts ctor.", standardError, 0) ;
}
}

void
ArrayCopyDrugAlerts::vider()
{
	if (empty())
		return ;

	for (NsDrugAlertIter i = begin(); end() != i ; )
		erase(i) ;
}

ArrayCopyDrugAlerts::~ArrayCopyDrugAlerts()
{
	vider() ;
}

ArrayCopyDrugAlerts&
ArrayCopyDrugAlerts::operator=(const ArrayCopyDrugAlerts& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (NsDrugAlertConstIter i = src.begin(); src.end() != i ; i++)
			push_back(*i) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ArrayCopyDrugAlerts (=).", standardError, 0) ;
	return *this ;
}
}

void
ArrayCopyDrugAlerts::addTo(const NsDrugAlertArray* pSrc)
{
try
{
  if (((NsDrugAlertArray*) NULL == pSrc) || pSrc->empty())
    return ;

  for (NsDrugAlertConstIter i = pSrc->begin() ; pSrc->end() != i ; i++)
    push_back(*i) ;
}
catch (...)
{
	erreur("Exception ArrayCopyDrugAlerts addTo", standardError, 0) ;
}
}

////////////////////////// fin de nsMailSvce.cpp ///////////////////////////////
