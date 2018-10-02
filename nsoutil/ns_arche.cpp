//---------------------------------------------------------------------------//    NS_ARCHE.CPP
//    PA   juillet 95
//  Implémentation des objets materiel
//---------------------------------------------------------------------------
#include <owl\applicat.h>
#include <owl\decmdifr.h>
#include <owl\docmanag.h>
#include <owl\olemdifr.h>

#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring.h>

#include "partage\nsglobal.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsoutil\nsoutil.h"

#include "nsoutil\ns_arche.h"
#include "nsoutil\ns_arche.rh"

#include "nssavoir\nsgraphe.h"
#include "pilot\Pilot.hpp"
#include "pilot\JavaSystem.hpp"
#include "nsbb\tagNames.h"

// -----------------------------------------------------------------
//
//  Méthodes de ArchetypesListDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ArchetypesListDialog, NSUtilDialog)
	EV_COMMAND(IDOK,     CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_LVN_GETDISPINFO(IDC_LM_LW, LvnGetDispInfo),
  EV_LVN_COLUMNCLICK(IDC_LM_LW, LVNColumnclick),
END_RESPONSE_TABLE;

ArchetypesListDialog::ArchetypesListDialog(TWindow* pere, NSContexte* pCtx, int iType, TModule* /* mod */)
                    :NSUtilDialog(pere, pCtx, "IDD_LISTARCH", pNSResModule)
{
try
{
	_iType = iType ;

	_pList = new NSArcheListWindow(this, IDC_LM_LW) ;
  
	_iSelectedArchetype = -1 ;
  _bNaturallySorted   = false ;
	_iSortedColumn      = 0 ;
}
catch (...)
{
	erreur("Exception ArchetypesListDialog ctor.", standardError, 0) ;
}
}

ArchetypesListDialog::~ArchetypesListDialog()
{
	delete _pList ;
}

voidArchetypesListDialog::SetupWindow()
{
	// fichiers d'aide
  sHindex = "" ;
  sHcorps = "" ;
	NSUtilDialog::SetupWindow() ;
  InitListe() ;

	if (InitArray())
  	AfficheListe() ;
}

// Initialisation du tableau depuis la base
bool
ArchetypesListDialog::InitArray()
{
try
{
	_aArray.vider() ;

	// on récupère la liste des matériels : actifs et non-actifs
	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrList ;

  string sTraitName = string("") ;
  if      (NSArcManager::archetype    == _iType)
		sTraitName = "_0ARCH_0ARID" ;
	else if (NSArcManager::referentiel  == _iType)
		sTraitName = "_0REFE_0ARID" ;
  else if (NSArcManager::decisionTree == _iType)
		sTraitName = "_0ARDE_0ARID" ;

	AttrList.push_back(new NSBasicAttribute(sTraitName, "")) ;
	AttrList.push_back(new NSBasicAttribute(RESEARCH, CONTAIN_RESEARCH)) ;

	bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST.c_str(), &ObjList, &AttrList) ;
	if (!res)
		return false ;

	if (ObjList.empty())
  	return true ;

  NSDataGraph Graph(pContexte->getSuperviseur(), graphObject) ;

	for (NSPersonsAttributeIter k = ObjList.begin() ; k != ObjList.end() ; k++)
  {
  	string sOIDS = (*k)->getAttributeValue(OIDS) ;

		NSBasicAttributeArray AttrArray ;
		AttrArray.push_back(new NSBasicAttribute(OBJECT, sOIDS)) ;
    res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), &Graph, &AttrArray) ;
    if (!res)
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") ;
      if (sErrorText == "")
      	sErrorText = string("Echec service : Impossible de récupérer un objet dans la base") ;
      sErrorText += string(" (") + sOIDS + string(")") ;
      erreur(sErrorText.c_str(), standardError, 0) ;
    }
    else
    {
    	string sTypeLex ;
    	string sType ;

      if (_iType == NSArcManager::archetype)
      {
      	sTypeLex = string("0ARCH1") ;
    		sType = string("0ARCH") ;
    	}
      else
      {
      	sTypeLex = string("0REFE1") ;
    		sType = string("0REFE") ;
      }

      NSDataTreeIter iterTree ;
      if (Graph.getTrees()->ExisteTree(sTypeLex, pContexte->getSuperviseur(), &iterTree))
      {
      	string sFichArc = ArchetypeGetFile(pContexte->getSuperviseur(), *iterTree, sType) ;
        string sNomArc  = ArchetypeGetID(pContexte->getSuperviseur(), *iterTree, sType) ;

        (*iterTree)->setModelName(sNomArc) ;
        (*iterTree)->setModelFileName(sFichArc) ;
        _aArray.push_back(new NSDataTree(*(*iterTree))) ;
      }
    }
  }

	return true ;
}
catch (...)
{
	erreur("Exception ArchetypesListDialog::InitArray.", standardError, 0) ;
	return false ;
}
}

voidArchetypesListDialog::InitListe()
{
	TListWindColumn colID("Identifiant", 170, TListWindColumn::Left, 0) ;
  _pList->InsertColumn(0, colID) ;
  TListWindColumn colFile("Fichier", 230, TListWindColumn::Left, 1) ;
  _pList->InsertColumn(1, colFile) ;
}

void
ArchetypesListDialog::AfficheListe()
{
	_pList->DeleteAllItems() ;

  if (_aArray.empty())
		return ;

  size_t nbMat = _aArray.size() ;
	for (int i = nbMat - 1; i >= 0 ; i--)
	{
  	string sID = (_aArray[i])->getModelName() ;
    TListWindItem Item(sID.c_str(), 0) ;
    _pList->InsertItem(Item) ;
	}
}

void
ArchetypesListDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;
  string sFileName ;

	string sLang = "";
  if ((pContexte) && (pContexte->getUtilisateur()))
  	sLang = pContexte->getUtilisateur()->donneLang() ;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne
	switch (dispInfoItem.GetSubItem())
  {
  	case 1: 	// Nom de fichier
    	sFileName = (_aArray[index])->getModelFileName() ;
      dispInfoItem.SetText(sFileName.c_str()) ;
      break ;
	}
}

void
ArchetypesListDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
    case 0  : sortByID() ;   break ;
    case 1  : sortByFile() ; break ;
  }
}

void
ArchetypesListDialog::swapSortOrder(int iColumn)
{
  if (_iSortedColumn == iColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = iColumn ;
    _bNaturallySorted = true ;
  }
}

void
ArchetypesListDialog::sortByID()
{
	swapSortOrder(0) ;

  if (_aArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aArray.begin(), _aArray.end(), ArchetypeSortByIDInf) ;
  else
    sort(_aArray.begin(), _aArray.end(), ArchetypeSortByIDSup) ;

  AfficheListe() ;
}

void
ArchetypesListDialog::sortByFile()
{
	swapSortOrder(0) ;

  if (_aArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aArray.begin(), _aArray.end(), ArchetypeSortByFileInf) ;
  else
    sort(_aArray.begin(), _aArray.end(), ArchetypeSortByFileSup) ;

  AfficheListe() ;
}

void
ArchetypesListDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

voidArchetypesListDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSArcheListWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSArcheListWindow, TListWindow)
    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSArcheListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point);

	HitTest(info);

	// if (info.GetFlags() & LVHT_ONITEM)
	//	pDlg->CmModifier() ;
}

//---------------------------------------------------------------------------
//  Description: Retourne l'index du premier item sélectionné
//---------------------------------------------------------------------------
int
NSArcheListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0; i < count; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break;
    }

	return index ;
}

// -----------------------------------------------------------------
//
//  Méthodes de RefInfoStorage
//
// -----------------------------------------------------------------

RefInfoStorage::RefInfoStorage()
{
  sObjectId = string("") ;
  sRefId    = string("") ;
  sLabel    = string("") ;
  sType     = string("") ;
  sDomain   = string("") ;
}

RefInfoStorage::RefInfoStorage(RefInfoStorage& rv)
{
  sObjectId = rv.sObjectId ;
  sRefId    = rv.sRefId ;
  sLabel    = rv.sLabel ;
  sType     = rv.sType ;
  sDomain   = rv.sDomain ;
}

RefInfoStorage&
RefInfoStorage::operator=(RefInfoStorage src)
{
  if (this == &src)
    return *this ;

  sObjectId = src.sObjectId ;
  sRefId    = src.sRefId ;
  sLabel    = src.sLabel ;
  sType     = src.sType ;
  sDomain   = src.sDomain ;

  return *this ;
}

bool RefInfoSortByLabelInf(RefInfoStorage *pStorage1, RefInfoStorage *pStorage2)
{
  if ((NULL == pStorage1) || (NULL == pStorage2))
    return false ;

  // return (strcmp(pStorage1->sLabel.c_str(), pStorage2->sLabel.c_str()) > 0) ;

  return (pseumaj(pStorage1->sLabel) > pseumaj(pStorage2->sLabel)) ;
}

bool RefInfoSortByLabelSup(RefInfoStorage *pStorage1, RefInfoStorage *pStorage2)
{
	if ((NULL == pStorage1) || (NULL == pStorage2))
    return false ;

  // return (strcmp(pStorage1->sLabel.c_str(), pStorage2->sLabel.c_str()) < 0) ;

  return (pseumaj(pStorage1->sLabel) < pseumaj(pStorage2->sLabel)) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de ReferentialChoiceDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ReferentialChoiceDialog, NSUtilDialog)
	EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_LVN_GETDISPINFO(IDC_LM_LW, LvnGetDispInfo),
  EV_LVN_COLUMNCLICK(IDC_LM_LW, LVNColumnclick),
  EV_LVN_ITEMCHANGED(IDC_LM_LW, SelChanged),
  EV_LVN_GETDISPINFO(IDC_LM_LW_T, LvnGetDispInfoTrt),
  EV_LVN_COLUMNCLICK(IDC_LM_LW_T, LVNColumnclickTrt),
  EV_LVN_ITEMCHANGED(IDC_LM_LW_T, SelChangedTrt),
END_RESPONSE_TABLE;

ReferentialChoiceDialog::ReferentialChoiceDialog(TWindow* pere, NSContexte* pCtx, LDVFRAME iFrame, string sType, SORTTYPE sortType, string sConcept, TModule* /* mod */)
                        :NSUtilDialog(pere, pCtx, "IDD_LISTARCH", pNSResModule)
{
try
{
	_sType     = sType ;
  _sortType  = sortType ;
  _sConcept  = sConcept ;
  _iFrame    = iFrame ;

	_pList    = new NSRefChoiceListWindow(this, IDC_LM_LW) ;
  _pListTrt = new NSRefChoiceListWindow(this, IDC_LM_LW_T) ;

  _bNaturallySorted    = false ;
	_iSortedColumn       = 0 ;
  _bNaturallySortedTrt = false ;
  _iSortedColumnTrt    = 0 ;
  
  _pSelectedRef        = 0 ;

  _bUseSemanticNetwork = false ;
}
catch (...)
{
	erreur("Exception ReferentialChoiceDialog ctor.", standardError, 0) ;
}
}

ReferentialChoiceDialog::~ReferentialChoiceDialog()
{
	delete _pList ;
  delete _pListTrt ;
}

voidReferentialChoiceDialog::SetupWindow()
{
	// fichiers d'aide
  sHindex = "" ;
  sHcorps = "" ;
	NSUtilDialog::SetupWindow() ;
  InitListe() ;

	if (InitArray())
  {
  	// AfficheListe() ;
    _bNaturallySorted = true ;
    _bNaturallySortedTrt = true ;
    sortByName() ;
    sortByNameTrt() ;
  }

  _pList->SetFocus() ;
}

// Initialisation du tableau depuis la base
bool
ReferentialChoiceDialog::InitArray()
{
try
{
	_aArray.vider() ;
  _aArrayTrt.vider() ;

  VectString aVecteurString ;

  aVecteurString.push_back(new string(_sConcept)) ;
  if (_bUseSemanticNetwork)
  {
    NSSuper* pSuper = pContexte->getSuperviseur() ;
    pSuper->getFilGuide()->TousLesVrais(_sConcept, "ES", &aVecteurString, "FLECHE") ;
  }

  string sFrameRootSens = string("") ;
  string sFrameRoot     = getRootLabelForFrame(_iFrame) ;
  if (string("") != sFrameRoot)
    pContexte->getDico()->donneCodeSens(&sFrameRoot, &sFrameRootSens) ;

  for (IterString iIter = aVecteurString.begin() ; aVecteurString.end() != iIter ; iIter++)
  {
	  // on récupère la liste des matériels : actifs et non-actifs
	  NSPersonsAttributesArray ObjList ;
	  NSBasicAttributeArray    AttrList ;

	  AttrList.push_back(new NSBasicAttribute(string("_0REFE_LDOMA"), _sType)) ;
    if (string("") != **iIter)
      AttrList.push_back(new NSBasicAttribute(string("_0REFE_LTYPA"), **iIter)) ;
    if (string("") != sFrameRootSens)
      AttrList.push_back(new NSBasicAttribute(string("_0REFE_LFRME"), sFrameRootSens)) ;

	  bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST_WITH_TRAITS.c_str(), &ObjList, &AttrList) ;
	  if (res && (false == ObjList.empty()))
	  {
	    for (NSPersonsAttributeIter k = ObjList.begin() ; ObjList.end() != k ; k++)
      {
        RefInfoStorage* pStorage = new RefInfoStorage() ;

        pStorage->sObjectId = (*k)->getAttributeValue(OIDS) ;
        pStorage->sRefId    = (*k)->getAttributeValue(string("_0REFE_0ARID")) ;
        pStorage->sLabel    = (*k)->getAttributeValue(string("_0REFE_LNOMA")) ;
        pStorage->sType     = (*k)->getAttributeValue(string("_0REFE_LTYPA")) ;
        pStorage->sDomain   = (*k)->getAttributeValue(string("_0REFE_LDOMA")) ;

        storeInList(pStorage) ;
      }
    }
  }

	return true ;
}
catch (...)
{
	erreur("Exception ReferentialChoiceDialog::InitArray.", standardError, 0) ;
	return false ;
}
}

void
ReferentialChoiceDialog::storeInList(RefInfoStorage* pStorage)
{
  if (NULL == pStorage)
    return ;

  switch(_sortType)
  {
    case sortNone :
      _aArray.push_back(pStorage) ;
      break ;

    case sortDrugs :
      // just drugs, not treatments ('N') since the other column is for "treatment of..."
      if ((string("") != pStorage->sType) && (pStorage->sType[0] != 'I') && (pStorage->sType[0] != '_'))
        _aArray.push_back(pStorage) ;
      else
        _aArrayTrt.push_back(pStorage) ;
      break ;
  }
}

bool
ReferentialChoiceDialog::getObjectTree(NSDataTree* pDataTree)
{
  if (NULL == pDataTree)
    return false ;

  string sOIDS = pDataTree->getModelFileName() ;
  if (string("") == sOIDS)
    return true ;

  NSDataGraph Graph(pContexte->getSuperviseur(), graphObject) ;

  NSBasicAttributeArray AttrArray ;
  AttrArray.push_back(new NSBasicAttribute(OBJECT, sOIDS)) ;
  bool res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), &Graph, &AttrArray) ;
  if (false == res)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") ;
    if (sErrorText == "")
      sErrorText = string("Echec service : Impossible de récupérer un objet dans la base") ;
    sErrorText += string(" (") + sOIDS + string(")") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

  string sTypeLex = string("0REFE1") ;

  NSDataTreeIter iterTree ;
  if (Graph.getTrees()->ExisteTree(sTypeLex, pContexte->getSuperviseur(), &iterTree))
  {
    string sFichArc = ArchetypeGetFile(pContexte->getSuperviseur(), *iterTree, string("0REFE")) ;
    string sNomArc  = ArchetypeGetID(pContexte->getSuperviseur(), *iterTree, string("0REFE")) ;

    (*iterTree)->setModelName(sNomArc) ;
    (*iterTree)->setModelFileName(sFichArc) ;

    *pDataTree = **iterTree ;
  }

  return true ;
}

voidReferentialChoiceDialog::InitListe()
{
	string sTitleText = pContexte->getSuperviseur()->getText("referentialSelection", "referentialTitle") ;

	TListWindColumn colID((char*)sTitleText.c_str(), 400, TListWindColumn::Left, 0) ;
  _pList->InsertColumn(0, colID) ;
  TListWindColumn colID2((char*)sTitleText.c_str(), 400, TListWindColumn::Left, 0) ;
  _pListTrt->InsertColumn(0, colID) ;
}

void
ReferentialChoiceDialog::AfficheListe()
{
	_pList->DeleteAllItems() ;

  if (false == _aArray.empty())
  {
    size_t nbMat = _aArray.size() ;
	  for (int i = nbMat - 1; i >= 0 ; i--)
	  {
      TListWindItem Item(_aArray[i]->sLabel.c_str(), 0) ;
      _pList->InsertItem(Item) ;
	  }
  }
}

void
ReferentialChoiceDialog::AfficheListeTrt()
{
	_pListTrt->DeleteAllItems() ;

  if (false == _aArrayTrt.empty())
  {
    size_t nbMat = _aArrayTrt.size() ;
	  for (int i = nbMat - 1; i >= 0 ; i--)
	  {
      TListWindItem Item(_aArrayTrt[i]->sLabel.c_str(), 0) ;
      _pListTrt->InsertItem(Item) ;
	  }
  }
}

void
ReferentialChoiceDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;
  string sFileName ;

	string sLang = "";
  if ((pContexte) && (pContexte->getUtilisateur()))
  	sLang = pContexte->getUtilisateur()->donneLang() ;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne
	switch (dispInfoItem.GetSubItem())
  {
  	case 1: 	// Nom de fichier
    	sFileName = (_aArray[index])->sLabel.c_str() ;
      dispInfoItem.SetText(sFileName.c_str()) ;
      break ;
	}
}

void
ReferentialChoiceDialog::LvnGetDispInfoTrt(TLwDispInfoNotify& dispInfo)
{
  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;
  string sFileName ;

	string sLang = "";
  if ((pContexte) && (pContexte->getUtilisateur()))
  	sLang = pContexte->getUtilisateur()->donneLang() ;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne
	switch (dispInfoItem.GetSubItem())
  {
  	case 1: 	// Nom de fichier
    	sFileName = (_aArrayTrt[index])->sLabel.c_str() ;
      dispInfoItem.SetText(sFileName.c_str()) ;
      break ;
	}
}

void
ReferentialChoiceDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
    case 0  : sortByName() ; break ;
  }
}

void
ReferentialChoiceDialog::LVNColumnclickTrt(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
    case 0  : sortByNameTrt() ; break ;
  }
}

void
ReferentialChoiceDialog::SelChanged(TLwNotify& lwn)
{
  if (LVIF_STATE == lwn.uChanged)
    unselectAllTrt() ;
}

void
ReferentialChoiceDialog::SelChangedTrt(TLwNotify& lwn)
{
  if (LVIF_STATE == lwn.uChanged)
    unselectAll() ;
}

void
ReferentialChoiceDialog::swapSortOrder(int iColumn)
{
  if (_iSortedColumn == iColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = iColumn ;
    _bNaturallySorted = true ;
  }
}

void
ReferentialChoiceDialog::swapSortOrderTrt(int iColumn)
{
  if (_iSortedColumnTrt == iColumn)
  {
    if (_bNaturallySortedTrt)
      _bNaturallySortedTrt = false ;
    else
      _bNaturallySortedTrt = true ;
  }
  else
  {
    _iSortedColumnTrt = iColumn ;
    _bNaturallySortedTrt = true ;
  }
}

bool ArchetypeSortByIDInf(NSDataTree *pTree1, NSDataTree *pTree2)
{
  if ((NULL == pTree1) || (NULL == pTree2))
    return false ;

	NSSuper* pSuper = pTree1->_pSuper ;
	string sTitle1 = ArchetypeGetProperty(pSuper, pTree1, string("0REFE"), string("LNOMA")) ;
  pseumaj(&sTitle1) ;
  string sTitle2 = ArchetypeGetProperty(pSuper, pTree2, string("0REFE"), string("LNOMA")) ;
  pseumaj(&sTitle2) ;

  return (strcmp(sTitle1.c_str(), sTitle2.c_str()) > 0) ;
}

bool ArchetypeSortByIDSup(NSDataTree *pTree1, NSDataTree *pTree2)
{
  if ((NULL == pTree1) || (NULL == pTree2))
    return false ;

	NSSuper* pSuper = pTree1->_pSuper ;
	string sTitle1 = ArchetypeGetProperty(pSuper, pTree1, string("0REFE"), string("LNOMA")) ;
  pseumaj(&sTitle1) ;
  string sTitle2 = ArchetypeGetProperty(pSuper, pTree2, string("0REFE"), string("LNOMA")) ;
  pseumaj(&sTitle2) ;

  return (strcmp(sTitle1.c_str(), sTitle2.c_str()) < 0) ;
}

void
ReferentialChoiceDialog::sortByName()
{
	swapSortOrder(0) ;

  if (_aArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aArray.begin(), _aArray.end(), RefInfoSortByLabelInf) ;
  else
    sort(_aArray.begin(), _aArray.end(), RefInfoSortByLabelSup) ;

  AfficheListe() ;
}

void
ReferentialChoiceDialog::sortByNameTrt()
{
	swapSortOrderTrt(0) ;

  if (_aArrayTrt.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aArrayTrt.begin(), _aArrayTrt.end(), RefInfoSortByLabelInf) ;
  else
    sort(_aArrayTrt.begin(), _aArrayTrt.end(), RefInfoSortByLabelSup) ;

  AfficheListeTrt() ;
}

void
ReferentialChoiceDialog::CmOk()
{
  _pSelectedRef = getSelected() ;

	NSUtilDialog::CmOk() ;
}

voidReferentialChoiceDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

RefInfoStorage*
ReferentialChoiceDialog::getSelected()
{
  RefInfoStorage* pSelected = getSelectedIn1() ;
  if ((RefInfoStorage*) NULL != pSelected)
    return pSelected ;

  pSelected = getSelectedIn2() ;
  if ((RefInfoStorage*) NULL != pSelected)
    return pSelected ;

  return (RefInfoStorage*) 0 ;
}

RefInfoStorage*
ReferentialChoiceDialog::getSelectedIn1()
{
  int count = _pList->GetItemCount() ;
  if (count <= 0)
    return 0 ;

	for (int i = 0; i < count; i++)
  	if (_pList->GetItemState(i, LVIS_SELECTED))
      return _aArray[i] ;

	return (RefInfoStorage*) 0 ;
}

RefInfoStorage*
ReferentialChoiceDialog::getSelectedIn2()
{
  int count = _pListTrt->GetItemCount() ;
  if (count <= 0)
    return 0 ;

	for (int i = 0; i < count; i++)
  	if (_pListTrt->GetItemState(i, LVIS_SELECTED))
      return _aArrayTrt[i] ;

	return (RefInfoStorage*) 0 ;
}


string
ReferentialChoiceDialog::getSelectedObjectId()
{
  if (NULL != _pSelectedRef)
    return _pSelectedRef->sObjectId ;

  return string("") ;
}

string
ReferentialChoiceDialog::getSelectedRefId()
{
  if (NULL != _pSelectedRef)
    return _pSelectedRef->sRefId ;

  return string("") ;
}

void
ReferentialChoiceDialog::unselectAll()
{
  int count = _pList->GetItemCount() ;
  if (count <= 0)
    return ;

	for (int i = 0; i < count; i++)
  	if (_pList->GetItemState(i, LVIS_SELECTED))
      _pList->SetItemState(i, 0, LVIS_SELECTED) ;
}

void
ReferentialChoiceDialog::unselectAllTrt()
{
  int count = _pListTrt->GetItemCount() ;
  if (count <= 0)
    return ;

	for (int i = 0; i < count; i++)
  	if (_pListTrt->GetItemState(i, LVIS_SELECTED))
      _pListTrt->SetItemState(i, 0, LVIS_SELECTED) ;
}

/*
NSDataTree*
ReferentialChoiceDialog::getSelectedRef()
{
	if ((-1 == _iSelectedReferential) || (true == _aArray.empty()))
		return 0 ;

	return _aArray[_iSelectedReferential] ;
}
*/

// -----------------------------------------------------------------
//
//  Méthodes de NSArcheListWindow
//
// -----------------------------------------------------------------

#define CM_REF_OPEN 101
#define CM_REF_EDIT 102

DEFINE_RESPONSE_TABLE1(NSRefChoiceListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_REF_OPEN, CmOpen),
  EV_COMMAND(CM_REF_EDIT, CmEdit),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSRefChoiceListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
		pDlg->CmOk() ;
}

void
NSRefChoiceListWindow::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  NS_CLASSLIB::TPoint lp = point ;

	TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pDlg->pContexte->getSuperviseur() ;
	string sOpen = pSuper->getCapitalizedText("generalLanguage", "open") ;
  string sEdit = pSuper->getCapitalizedText("generalLanguage", "edit") ;

  menu->AppendMenu(MF_STRING, CM_REF_OPEN, sOpen.c_str()) ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, CM_REF_EDIT, sEdit.c_str()) ;

  ClientToScreen(lp) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
	delete menu ;
}

//---------------------------------------------------------------------------
//  Description: Retourne l'index du premier item sélectionné
//---------------------------------------------------------------------------
int
NSRefChoiceListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0; i < count; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break;
    }

	return index ;
}

void
NSRefChoiceListWindow::CmOpen()
{
}

void
NSRefChoiceListWindow::CmEdit()
{
}

// -----------------------------------------------------------------
//
//  Méthodes de ReferentialTitleDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ReferentialTitleDialog, NSUtilDialog)
	EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

ReferentialTitleDialog::ReferentialTitleDialog(TWindow* pere, NSContexte* pCtx, string* pTitle, string* pID, string* pConcept, string* pRoot, TModule* /* mod */)
                       :NSUtilDialog(pere, pCtx, "IDD_CREAT_REFERENTIAL", pNSResModule)
{
try
{
	_pTitle   = pTitle ;
	_pID      = pID ;
  _pConcept = pConcept ;
  _pRoot    = pRoot ;

	_pTitleEdit     = new TEdit(this, IDC_EDIT_TITLE) ;
	_pIDEdit        = new TEdit(this, IDC_EDIT_ID) ;
  _pTitleTxt      = new TStatic(this, IDC_TXT_TITLE) ;
	_pIDTxt         = new TStatic(this, IDC_TXT_ID) ;
  _pConceptEdit   = new NSUtilLexique(pContexte, this, IDC_EDIT_CONCEPT, pContexte->getDico()) ;
  _pConceptTxt    = new TStatic(this, IDC_TXT_CONCEPT) ;
  _pHealthRelated = new TRadioButton(this, IDC_HEALTH_CONCEPT)  ;
  _pSocialRelated = new TRadioButton(this, IDC_SOCIAL_CONCEPT)  ;
  _pRiskRelated   = new TRadioButton(this, IDC_RISK_CONCEPT)  ;
}
catch (...)
{
	erreur("Exception ReferentialTitleDialog ctor.", standardError, 0) ;
}
}

ReferentialTitleDialog::~ReferentialTitleDialog()
{
  delete _pTitleEdit ;
	delete _pIDEdit ;
  delete _pTitleTxt ;
	delete _pIDTxt ;
  delete _pConceptEdit ;
  delete _pConceptTxt ;
  delete _pHealthRelated ;
  delete _pSocialRelated ;
  delete _pRiskRelated ;
}

voidReferentialTitleDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  string sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "referentialCreation") ;
  SetCaption(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "title") ;
  _pTitleTxt->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "identifier") ;
  _pIDTxt->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "concept") ;
  _pConceptTxt->SetText(sText.c_str()) ;

  if ((NULL != _pTitle) && (string("") != *_pTitle))
		_pTitleEdit->SetText(_pTitle->c_str()) ;
  if ((NULL != _pID) && (string("") != *_pID))
		_pIDEdit->SetText(_pID->c_str()) ;
  if ((NULL != _pConcept) && (string("") != *_pConcept))
		_pConceptEdit->setLabel(*_pConcept) ;

  if (NULL == _pRoot)
  {
    _pRiskRelated->SetCheck(BF_GRAYED) ;
    _pHealthRelated->SetCheck(BF_GRAYED) ;
    _pSocialRelated->SetCheck(BF_GRAYED) ;
  }
  else
  {
    _pRiskRelated->SetCheck(BF_UNCHECKED) ;
    _pHealthRelated->SetCheck(BF_UNCHECKED) ;
    _pSocialRelated->SetCheck(BF_UNCHECKED) ;

    if      (getRootLabelForFrame(ldvframeRisk) == *_pConcept)
      _pRiskRelated->SetCheck(BF_CHECKED) ;
    else if (getRootLabelForFrame(ldvframeHealth) == *_pConcept)
      _pHealthRelated->SetCheck(BF_CHECKED) ;
    else if (getRootLabelForFrame(ldvframeSocial) == *_pConcept)
      _pSocialRelated->SetCheck(BF_CHECKED) ;
    else
      _pHealthRelated->SetCheck(BF_CHECKED) ;
  }

  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "health") ;
  _pHealthRelated->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "social") ;
  _pSocialRelated->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "risk") ;
  _pRiskRelated->SetCaption(sText.c_str()) ;
}

void
ReferentialTitleDialog::CmOk()
{
	char text[256] ;

	if (NULL != _pTitle)
  {
		_pTitleEdit->GetText(text, 255) ;
    *_pTitle = string(text) ;
	}

  if (_pID)
  {
    _pIDEdit->GetText(text, 255) ;
    *_pID = string(text) ;
  }

  if (_pConcept)
  {
    // on récupère d'abord un éventuel élément lexique sélectionné par les flêches
    // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
    if (_pConceptEdit->getDico()->getDicoDialog()->EstOuvert())
    {
      _pConceptEdit->getDico()->getDicoDialog()->InsererElementLexique() ;
      return ;
    }

    // Ne pas accepter les textes libres
    if (_pConceptEdit->getCode() == string("£?????"))
    {
      string sWarningTxt = pContexte->getSuperviseur()->getText("lexiqueManagement", "freeTextNotAllowed") ;
      erreur(sWarningTxt.c_str(), warningError, 0, GetHandle()) ;
      _pConceptEdit->SetFocus() ;
      return ;
    }

    *_pConcept = _pConceptEdit->getCode() ;

    if (_pRoot)
    {
      if      (BF_CHECKED == _pHealthRelated->GetCheck())
        *_pRoot = getRootLabelForFrame(ldvframeHealth) ;
      else if (BF_CHECKED == _pSocialRelated->GetCheck())
        *_pRoot = getRootLabelForFrame(ldvframeSocial) ;
      else if (BF_CHECKED == _pRiskRelated->GetCheck())
        *_pRoot = getRootLabelForFrame(ldvframeRisk) ;
      else
        *_pRoot = string("") ;
    }
  }

	NSUtilDialog::CmOk() ;
}

voidReferentialTitleDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de ReferentialPropParamsDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ReferentialPropParamsDialog, NSUtilDialog)
	EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

ReferentialPropParamsDialog::ReferentialPropParamsDialog(TWindow* pere, NSContexte* pCtx, string* pTitle, string* pGroup, string* pHelp, string* pShort, string* pLong, string* pAutoCheck, string* pEvidenceLevel, string* pUncheckArchId, VecteurString* pGroupsLabels, TModule* /* mod */)
                            :NSUtilDialog(pere, pCtx, "IDD_EDIT_REF_PARAMS", pNSResModule)
{
try
{
	_pTitle         = pTitle ;
	_pGroup         = pGroup ;
  _pHelp          = pHelp ;
  _pAutoCheck     = pAutoCheck ;
  _pEvidenceLevel = pEvidenceLevel ;
  _pUncheckArchId = pUncheckArchId ;

  _pGroupsLabels  = pGroupsLabels ;

	if (NULL != pLong)
		_sLongLabel = *pLong ;
	else
  	_sLongLabel = string("") ;

	if (NULL != pShort)
		_sShortLabel = *pShort ;
	else
  	_sShortLabel = string("") ;

	_pTitleEdit   = new TEdit(this, IDC_EDIT_NAME) ;
	_pGroupEdit   = new TComboBox(this, IDC_EDIT_GROUP) ;
  _pHelpEdit    = new TEdit(this, IDC_EDIT_HELP) ;
  _pUncheckEdit = new TEdit(this, IDC_EDIT_UNCHCK) ;

  _pTitleTxt   = new TStatic(this, IDC_TXT_NAME) ;
	_pGroupTxt   = new TStatic(this, IDC_TXT_GROUP) ;
  _pHelpTxt    = new TStatic(this, IDC_TXT_HELP) ;
  _pUncheckTxt = new TStatic(this, IDC_TXT_UNCHCK) ;

  _pLongTxt   = new TStatic(this, IDC_TXT_LONG) ;
  _pShortTxt  = new TStatic(this, IDC_TXT_SHORT) ;

  _pAutoGroup = new TGroupBox(this, IDC_TXT_AUTOSEL) ;
  _pAutoYes   = new TRadioButton(this, IDR_AUTO_YES)  ;
  _pAutoNo    = new TRadioButton(this, IDR_AUTO_NO)  ;

  _pEvidenceGroup = new TGroupBox(this, IDC_TXT_LEVEL) ;
  _pEvidenceA = new TRadioButton(this, IDR_LEVEL_A)  ;
  _pEvidenceB = new TRadioButton(this, IDR_LEVEL_B)  ;
  _pEvidenceC = new TRadioButton(this, IDR_LEVEL_C)  ;
  _pEvidenceP = new TRadioButton(this, IDR_LEVEL_PRO)  ;
  _pEvidenceU = new TRadioButton(this, IDR_LEVEL_UNKNOWN)  ;
}
catch (...)
{
	erreur("Exception ReferentialPropParamsDialog ctor.", standardError, 0) ;
}
}

ReferentialPropParamsDialog::~ReferentialPropParamsDialog()
{
  delete _pTitleEdit ;
	delete _pGroupEdit ;
  delete _pHelpEdit ;
  delete _pUncheckEdit ;
  delete _pTitleTxt ;
	delete _pGroupTxt ;
  delete _pHelpTxt ;
  delete _pLongTxt ;
  delete _pShortTxt ;
  delete _pUncheckTxt ;

  delete _pAutoGroup ;
  delete _pAutoYes ;
  delete _pAutoNo ;

  delete _pEvidenceGroup ;
  delete _pEvidenceA ;
  delete _pEvidenceB ;
  delete _pEvidenceC ;
  delete _pEvidenceP ;
  delete _pEvidenceU ;
}

voidReferentialPropParamsDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  string sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "propositionEdit") ;
  SetCaption(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "propositionName") ;
  _pTitleTxt->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "propositionGroup") ;
  _pGroupTxt->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "helpUrl") ;
  _pHelpTxt->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "uncheckJustification") ;
  _pUncheckTxt->SetText(sText.c_str()) ;

  if ((NULL != _pTitle) && (string("") != *_pTitle))
		_pTitleEdit->SetText(_pTitle->c_str()) ;


  if (false == _pGroupsLabels->empty())
	  for (EquiItemIter itemsIter = _pGroupsLabels->begin() ; _pGroupsLabels->end() != itemsIter ; itemsIter++)
      _pGroupEdit->AddString((*itemsIter)->c_str()) ;

  if ((NULL != _pGroup) && (string("") != *_pGroup))
		_pGroupEdit->SetText(_pGroup->c_str()) ;

  if ((NULL != _pHelp) && (string("") != *_pHelp))
		_pHelpEdit->SetText(_pHelp->c_str()) ;
  if ((NULL != _pUncheckArchId) && (string("") != *_pUncheckArchId))
		_pUncheckEdit->SetText(_pUncheckArchId->c_str()) ;

	_pLongTxt->SetText(_sLongLabel.c_str()) ;
  _pShortTxt->SetText(_sShortLabel.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "automaticallyChecked") ;
  _pAutoGroup->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("generalLanguageforDialog", "yes") ;
  _pAutoYes->SetCaption(sText.c_str()) ;
  if ((NULL != _pAutoCheck) && (string("yes") == *_pAutoCheck))
    _pAutoYes->SetCheck(BF_CHECKED) ;
  sText = pContexte->getSuperviseur()->getText("generalLanguageforDialog", "no") ;
  _pAutoNo->SetCaption(sText.c_str()) ;
  if ((NULL != _pAutoCheck) && (string("no") == *_pAutoCheck))
    _pAutoNo->SetCheck(BF_CHECKED) ;

  _pEvidenceU->SetCheck(BF_CHECKED) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "evidenceLevel") ;
  _pEvidenceGroup->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "evidenceLevelA") ;
  _pEvidenceA->SetCaption(sText.c_str()) ;
  if ((NULL != _pEvidenceLevel) && (string("A") == *_pEvidenceLevel))
    _pEvidenceA->SetCheck(BF_CHECKED) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "evidenceLevelB") ;
  _pEvidenceB->SetCaption(sText.c_str()) ;
  if ((NULL != _pEvidenceLevel) && (string("B") == *_pEvidenceLevel))
    _pEvidenceB->SetCheck(BF_CHECKED) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "evidenceLevelC") ;
  _pEvidenceC->SetCaption(sText.c_str()) ;
  if ((NULL != _pEvidenceLevel) && (string("C") == *_pEvidenceLevel))
    _pEvidenceC->SetCheck(BF_CHECKED) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "evidenceLevelP") ;
  _pEvidenceP->SetCaption(sText.c_str()) ;
  if ((NULL != _pEvidenceLevel) && (string("P") == *_pEvidenceLevel))
    _pEvidenceP->SetCheck(BF_CHECKED) ;
  sText = pContexte->getSuperviseur()->getText("referentialCreationManagement", "unknownLevel") ;
  _pEvidenceU->SetCaption(sText.c_str()) ;
}

void
ReferentialPropParamsDialog::CmOk()
{
	char text[256] ;

	if (NULL != _pTitle)
  {
		_pTitleEdit->GetText(text, 255) ;
    *_pTitle = string(text) ;
	}

  if (NULL != _pGroup)
  {
    _pGroupEdit->GetText(text, 255) ;
    *_pGroup = string(text) ;
  }

  if (NULL != _pHelp)
  {
    _pHelpEdit->GetText(text, 255) ;
    *_pHelp = string(text) ;
  }

  if (NULL != _pUncheckArchId)
  {
    _pUncheckEdit->GetText(text, 255) ;
    *_pUncheckArchId = string(text) ;
  }

  if (NULL != _pAutoCheck)
  {
    if      (_pAutoYes->GetCheck() == BF_CHECKED)
      *_pAutoCheck = string("yes") ;
    else if (_pAutoNo->GetCheck()  == BF_CHECKED)
      *_pAutoCheck = string("no") ;
    else
      *_pAutoCheck = string("") ;
  }

  if (NULL != _pEvidenceLevel)
  {
    if      (_pEvidenceA->GetCheck() == BF_CHECKED)
      *_pEvidenceLevel = string("A") ;
    else if (_pEvidenceB->GetCheck() == BF_CHECKED)
      *_pEvidenceLevel = string("B") ;
    else if (_pEvidenceC->GetCheck() == BF_CHECKED)
      *_pEvidenceLevel = string("C") ;
    else if (_pEvidenceP->GetCheck() == BF_CHECKED)
      *_pEvidenceLevel = string("P") ;
    else
      *_pEvidenceLevel = string("") ;
  }

	NSUtilDialog::CmOk() ;
}

voidReferentialPropParamsDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

