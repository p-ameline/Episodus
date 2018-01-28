
#include "partage\nsglobal.h"
#include "nautilus\nsaddfunctions.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsbb\nsMView.h"
#include "nsepisod\nsToDo.h"

#include <owl/listwind.h>
#include "nautilus\nautilus.rh"
#include "nautilus\ns_functions.rh"
//***********************************************************************//
//							Classe NSAddFunctions
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSAddFunctions, TDialog)
	EV_COMMAND(IDOK,     CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

NSAddFunctions::NSAddFunctions(	TWindow* parent,
                                NSContexte* pCtx,
                                TModule* module)
               :TDialog(parent, IDD_INTROFUNCT, module), NSRoot(pCtx)
{
	pFctList = new NSFunctionsListWindow(this, pContexte, LIST_FUNCT);
}


NSAddFunctions::~NSAddFunctions()
{
	delete pFctList ;
}

void
NSAddFunctions::SetupWindow()
{
	TDialog::SetupWindow() ;
}

void
NSAddFunctions::CmOk()
{
  TDialog::CmOk() ;
  return ;

/*
  if (pFctList->pFctNameList->empty())
	{
  	TDialog::CmOk() ;
		return ;
	}

	int index = pFctList->IndexItemSelect() ;
	if (index == -1)
	{
  	string sErrMsg = pContexte->getSuperviseur()->getText("functionManagement", "YouMustSelectAFunction") ;
		erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  int i = 0 ;
	NSFunctionsManagementIter iterFunct= pFctList->pFctNameList->begin() ;
  for(iterFunct; ((iterFunct != pFctList->pFctNameList->end()) && (i!=index)); iterFunct++)
  	i++ ;

  if (iterFunct != pFctList->pFctNameList->end())
		pFctList->newWindow((*iterFunct)->getFonction()) ;

	TDialog::CmOk() ;
*/
}

void
NSAddFunctions::CmCancel()
{
	Destroy(IDCANCEL) ;
}

//***********************************************************************//
//							Classe NSIntroModifFunctions
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSIntroModifFunctions, TDialog)
	EV_COMMAND(IDOK,     CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSIntroModifFunctions::NSIntroModifFunctions(  TWindow* parent, NSContexte* pCtx,
                                                NSFunctionsManagement* pParam,
                                                NSCorrespondentsNamesArray *pCorrespName,
                                                TModule* module)
	:	TDialog(parent, IDD_NEWFUNCT, module),
		NSRoot(pCtx)
{
	pParamInt = pParam ;
	pNameList = pCorrespName ;
	pPropList = new NSSplitListWindow(this, pContexte, LIST_NEWFUNCT, pParamInt, pNameList) ;
	pEditFct  = new TEdit(this, IDC_EDITFCT) ;

}


NSIntroModifFunctions::~NSIntroModifFunctions()
{
	delete pPropList;
    delete pEditFct;
}

void
NSIntroModifFunctions::SetupWindow()
{
    TDialog::SetupWindow();
    if(pParamInt)
        pEditFct->SetText((pParamInt->getNomFctUtilisateur()).c_str());
}

void
NSIntroModifFunctions::CmOk()
{
    TDialog::CmOk();
}


void
NSIntroModifFunctions::CmCancel()
{
	Destroy(IDCANCEL);
}

// --------------------------------------------------------------------------
//              Classe NSFunctionsListWindow
// --------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSFunctionsListWindow, TListWindow)
	EV_WM_SETFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_WM_LBUTTONDOWN,
  EV_WM_LBUTTONDBLCLK,  //double click
  NS_LVN_GETDISPINFO(LvnGetDispInfo),
  NS_LW_COLUMNCLICK(LVNColumnclick),
  EV_COMMAND(IDC_MODCOMB, CmModifEnreg),
  EV_COMMAND(IDC_DELCOMB, CmSupprEnreg),
  EV_COMMAND(IDC_FCTPARAMS, CmModifParam),
END_RESPONSE_TABLE;

NSFunctionsListWindow::NSFunctionsListWindow(TWindow* parent, NSContexte* pCtx, int resourceId,
                           TModule* module)
                      :TListWindow(parent, resourceId, module), NSRoot(pCtx), aWindsPropLoc(pCtx)
{
	iSortedColumn = -1 ;

	pFctNameList = new NSFunctionsManagementArray() ;   //liste de fonctions
	pDialog = parent ;
	Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
	Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}

NSFunctionsListWindow::~NSFunctionsListWindow()
{
	delete pFctNameList ;
}

void
NSFunctionsListWindow::EvSetFocus(HWND hWndLostFocus)
{
}

void
NSFunctionsListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point ;

	TPopupMenu Menu ;

	string sModiComp = pContexte->getSuperviseur()->getText("functionManagement", "ModifyAComplementaryFunction") ;
  string sDeleComp = pContexte->getSuperviseur()->getText("functionManagement", "DeleteAComplementaryFunction") ;
  string sModiPara = pContexte->getSuperviseur()->getText("functionManagement", "ModifyParameters") ;

	Menu.AppendMenu(MF_STRING, IDC_MODCOMB, sModiComp.c_str()) ;
	Menu.AppendMenu(MF_STRING, IDC_DELCOMB, sDeleComp.c_str()) ;
  Menu.AppendMenu(MF_STRING, IDC_FCTPARAMS, sModiPara.c_str()) ;

	ClientToScreen(lp) ;
	Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
}

void
NSFunctionsListWindow::EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;
	int index = HitTest(info) ;
	if (index == -1)
		return ;
	TListWindow::EvLButtonDown(modkeys, point) ;
	SetItemState(index, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSFunctionsListWindow::newWindow(string sWndMother)
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;
	NSToDoTask* pTask = new NSToDoTask ;
	pTask->setWhatToDo(string("OpenNewWindow")) ;
	pTask->setParam1(sWndMother) ;

	pTask->sendMe(pSuper) ;
}

void
NSFunctionsListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	if (pFctNameList->empty())
		return ;

	EvLButtonDown(modKeys, point) ;
	TLwHitTestInfo info(point) ;

	HitTest(info) ;	if (info.GetFlags() & LVHT_ONITEM)
  {
  	int index = IndexItemSelect() ;
    if (index == -1)
    {
    	string sErrMsg = pContexte->getSuperviseur()->getText("functionManagement", "YouMustSelectAFunction") ;
			erreur(sErrMsg.c_str(), warningError, 0) ;
      return ;
    }
    int i = 0 ;
    NSFunctionsManagementIter iterFunct= pFctNameList->begin() ;
    for(iterFunct; ((iterFunct != pFctNameList->end()) && (i!=index)); iterFunct++)
    	i++ ;
    if (iterFunct != pFctNameList->end())
    	newWindow((*iterFunct)->getFonction()) ;
	}
  TDialog* pParent = TYPESAFE_DOWNCAST(pDialog, TDialog) ;
  pParent->CmCancel() ;
}

void
NSFunctionsListWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

	SetupColumns();

	string sLang = pContexte->getUserLanguage() ;
	initFunctionNameListe(sLang, pContexte->PathName("FGLO")) ;
	initComponentsListe(sLang, pContexte->PathName("FGLO")) ;
	AfficheListe() ;
	TListWindow::SetupWindow() ;
}

void
NSFunctionsListWindow::SetupColumns()
{
	NS_CLASSLIB:: TRect  rectList ;

	string sFunction = pContexte->getSuperviseur()->getText("functionManagement", "function") ;
  string sCompleme = pContexte->getSuperviseur()->getText("functionManagement", "complementaryFunction") ;
  string sOpening  = pContexte->getSuperviseur()->getText("functionManagement", "openingMode") ;
  string sHotKey   = pContexte->getSuperviseur()->getText("functionManagement", "hotKey") ;

  InsertColumn(0, TListWindColumn((char far*) sFunction.c_str(), 100)) ;
  InsertColumn(1, TListWindColumn((char far*) sCompleme.c_str(), 250)) ;
  InsertColumn(2, TListWindColumn((char far*) sOpening.c_str(),  100)) ;
  InsertColumn(3, TListWindColumn((char far*) sHotKey.c_str(),    50)) ;
}

int
NSFunctionsListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0; i < count; i++)  {
  	if (GetItemState(i, LVIS_SELECTED))    {
    	index = i ;
      break ;
    }
  }

  return index ;}


void
NSFunctionsListWindow::initFunctionNameListe(string sUserLang, string sPersoDirectory)
{
/*
    ifstream    inFile1, inFile2 ;

	string      line ;
	string      sData = "" ;
	string      sNomAttrib ;
	string      sNomComplement ;
  	size_t      i = 0 ;
	string      sFichier ;

	if (sUserLang != "")
	{
        sFichier = sPersoDirectory + string("localisation_") + sUserLang + string(".dat") ;
		inFile1.open(sFichier.c_str()) ;
	}
	else
	{
		inFile1.open("perso\\localisation.dat") ;
		if (!inFile1)
			return ;
	}

	if (!inFile1)
	{
		sFichier = sPersoDirectory + string("localisation.dat") ;
		inFile2.open(sFichier.c_str()) ;
		if (!inFile2)
		{
			erreur("Impossible d'ouvrir le fichier localisation.dat.", 0, 0) ;
			return ;
		}

		while (!inFile2.eof())
		{
			getline(inFile2,line) ;
			if (line != "")
				sData += line + "\n" ;
		}

		inFile2.close() ;
	}
	else
	{
		while (!inFile1.eof())
		{
			getline(inFile1,line) ;
			if (line != "")
				sData += line + "\n" ;
		}

		inFile1.close() ;
	}

	// boucle de chargement des attributs

    int pos = sData.find("[");
    i = pos ;
    while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))
        i++;
    i++;
	while (i < strlen(sData.c_str()))
	{
		sNomAttrib = "" ;
		sNomComplement = "";

        while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))
			sNomAttrib += sData[i++] ;
		while ((strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))
			i++ ;

		while ((strlen(sData.c_str())) && (sData[i] != '\t') && (sData[i] != '\n'))
			sNomComplement += sData[i++] ;
        while ((i < strlen(sData.c_str())) && ((sData[i] == '\n')||(sData[i] == ' ') || (sData[i] == '\t')))
            i++;
        NSFunctionsManagement *pElemManag = new NSFunctionsManagement() ;
        pElemManag->setFonction(sNomAttrib);
        pElemManag->setNomFctUtilisateur(sNomComplement);
        pFctNameList->push_back(pElemManag) ;

 	}
	return ;
*/
}

void
NSFunctionsListWindow::initComponentsListe(string sUserId, string sPersoDirectory, bool bFromFile)
{
	string sIndic = pContexte->getUtilisateur()->getNss() ;

	//les proprietes des toutes les fenetre
  if (bFromFile)
  	aWindsPropLoc.initWindsChild(sIndic, pContexte->PathName("FGLO")) ;
  else
  	aWindsPropLoc = *(pContexte->getUtilisateur()->getWindowPropertyArray()) ;

	if (aWindsPropLoc.empty())
  	return ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	pFctNameList->vider() ;
  ArrayWPIter iterProp = aWindsPropLoc.begin() ;
  for ( ; iterProp != aWindsPropLoc.end(); iterProp++)
  {
  	NSFunctionsManagement *pElemManag = new NSFunctionsManagement() ;
    pElemManag->setFonction((*iterProp)->getFunction()) ;
    string sLib = pSuper->getText("functions", (*iterProp)->getFunction()) ;
    if (sLib != "")
    	pElemManag->setNomFctUtilisateur(sLib) ;
    else
    	pElemManag->setNomFctUtilisateur((*iterProp)->getFunction()) ;

    string sCompWind = pElemManag->getNomFctUtilisateur() ;

  	//les proprietes d'une fenetre composée
    NSWindowProperty* pProp = aWindsPropLoc.getProperty((*iterProp)->getFunction()) ;
    //les parametres de splitting
    ArraySplitWindow *pParamSplit = pProp->getSplit() ;
    pElemManag->setListSplit(pProp->getSplit()) ;
    if (!pParamSplit->empty())
    {
    	for (ArraySWIter iterSplit = pParamSplit->begin(); (iterSplit != pParamSplit->end()); iterSplit++)
      {
      	string sLib = pSuper->getText("functions", (*iterSplit)->getChildWindow()) ;
        if (sLib != "")
        	sCompWind = sCompWind + " - " + sLib ;
      }
    }
    pElemManag->setComplement(sCompWind) ;
    pElemManag->setActivity((*iterProp)->getActivity()) ;
    pElemManag->setHotKey((*iterProp)->getHotKey()) ;

    pFctNameList->push_back(pElemManag) ;
	}

/*
    for (NSFunctionsManagementIter iterFunct = pFctNameList->begin(); (iterFunct != pFctNameList->end()); iterFunct++)
    {
        string sFct = (*iterFunct)->getFonction() ;
        ArrayWPIter iterProp = aWindsPropLoc.begin();
        string sCompWind = (*iterFunct)->getNomFctUtilisateur();      //on ajoute la premiere mere
        for (iterProp; ((iterProp != aWindsPropLoc.end()) && ((*iterProp)->sFunction != sFct)); iterProp++) ;
        if (iterProp != aWindsPropLoc.end())
        {
              //les proprietes d'une fenetre composée
            NSWindowProperty* pProp = aWindsPropLoc.getProperty(sFct);
            //les parametres de splitting
            ArraySplitWindow *pParamSplit = pProp->paramSplit;
            (*iterFunct)->setListSplit(pProp->paramSplit);
            if (!pParamSplit->empty())
            {
                for (ArraySWIter iterSplit = pParamSplit->begin(); (iterSplit != pParamSplit->end()); iterSplit++)
                {
                    //on cherche le nom utilisateur pour chaque complement
                    NSFunctionsManagementIter iterFct = pFctNameList->begin() ;
                    for(iterFct;((iterFct != pFctNameList->end())&&((*iterFct)->getFonction() != (*iterSplit)->getChildWindow()));iterFct++) ;
                    if (iterFct != pFctNameList->end())
                        sCompWind = sCompWind + " - " + (*iterFct)->getNomFctUtilisateur();
                }
            }
        }
        (*iterFunct)->setComplement(sCompWind);
    } */
}

void
NSFunctionsListWindow::AfficheListe()
{
	DeleteAllItems() ;

	if (pFctNameList->empty())
		return ;

  NSFunctManArray ::reverse_iterator itReverse ;
  itReverse = pFctNameList->rbegin() ;

  while (itReverse != pFctNameList->rend())
  {
  	string sNomFct = (*itReverse)->getNomFctUtilisateur() ;
    if (sNomFct != "")
    {
    	TListWindItem Item(sNomFct.c_str() , 0) ;
      InsertItem(Item) ;
    }
    itReverse++ ;
  }
}

void
NSFunctionsListWindow::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;

  int i = 0 ;
  NSFunctionsManagementIter iterFunct= pFctNameList->begin() ;
  for(iterFunct; ((iterFunct != pFctNameList->end()) && (i < index)); iterFunct++)
  	i++ ;
  if ((iterFunct == pFctNameList->end()) || (i != index))
		return ;

	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
  	case 1 : // complément
    {
    	string sComp = (*iterFunct)->getComplement() ;
      dispInfoItem.SetText(sComp.c_str(), strlen(sComp.c_str())) ;
      break ;
    }

		case 2 : // opening mode
    {
    	string sText = string("") ;
    	switch ((*iterFunct)->getActivity())
      {
      	case NSWindowProperty::notAvailable :
        	sText = pContexte->getSuperviseur()->getText("functionManagement", "notAvailable") ;
          break ;
        case NSWindowProperty::available :
        	sText = pContexte->getSuperviseur()->getText("functionManagement", "available") ;
          break ;
        case NSWindowProperty::openedWithPatient :
        	sText = pContexte->getSuperviseur()->getText("functionManagement", "openedWithPatient") ;
          break ;
        case NSWindowProperty::blackboardedWithPatient :
        	sText = pContexte->getSuperviseur()->getText("functionManagement", "blackboardedWithPatient") ;
          break ;
      }
      dispInfoItem.SetText(sText.c_str(), strlen(sText.c_str())) ;
      break ;
		}

    case 3 : // Hot key
    {
    	char szText[2] ;
    	szText[0] = (*iterFunct)->getHotKey() ;
      szText[1] = '\0' ;
      dispInfoItem.SetText(szText, strlen(szText)) ;
      break ;
    }
  }
}

void
NSFunctionsListWindow::LVNColumnclick(TLwNotify& lwn)
{
	switch ( lwn.iSubItem )
	{
		case 0 :
    	sortByLibelle() ;
      break ;
	}
}

void
NSFunctionsListWindow::sortByLibelle()
{
}

void
NSFunctionsListWindow::EnregData(NSFunctionsManagement* pParamsDevelop)
{
  NSUtilisateurChoisi* pUser = pContexte->getUtilisateur() ;
  if ((NSUtilisateurChoisi*) NULL == pUser)
    return ;

	if (false == pUser->isEnregWin())
  	return ;

  NSWindowProperty NewPropWind ;

  NewPropWind.setActivity(pParamsDevelop->getActivity()) ;
  NewPropWind.setHotKey(pParamsDevelop->getHotKey()) ;

  // *(pNewPropWind->paramSplit) = *(pParamsDevelop->getListSplit());
  ArraySplitWindow* pSplit = new ArraySplitWindow(* pParamsDevelop->getListSplit()) ;
  NewPropWind.setSplit(pSplit) ;
  NewPropWind.setFunction(pParamsDevelop->getFonction()) ;
  NewPropWind.setX(-1) ;
  NewPropWind.setY(-1) ;
  NewPropWind.setW(-1) ;
  NewPropWind.setH(-1) ;
  // dans la methode saveWindowProperty on met directement le pointeur dans le vector
  //donc il faut pas l'effacer!!!!
  // bool bNewFct = false ;

	string sUserId = pUser->getNss() ;
	NSWindowProperty* pWinPropFct = pUser->getWindowProperty(pParamsDevelop->getFonction()) ;
  if (NULL == pWinPropFct)
  {
  	// bNewFct = true ;
    pUser->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &NewPropWind, false) ;
    pWinPropFct = pUser->getWindowProperty(pParamsDevelop->getFonction()) ;
  }
  ArraySplitWindow* pParamsSplitting = pParamsDevelop->getListSplit() ;
  if (NULL == pParamsSplitting)
  	return ;

  pWinPropFct->resetSplitList() ;

  if (false == pParamsSplitting->empty())
  {
  	ArraySWIter iterSplit = pParamsSplitting->begin() ;
  	for( ; pParamsSplitting->end() != iterSplit ;  iterSplit++)
    	pWinPropFct->addSplit((*iterSplit)->getMotherWindow(), (*iterSplit)->getChildWindow(), (*iterSplit)->getSplitDirection(), (*iterSplit)->getPercent()) ;
  }

  pUser->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &NewPropWind, false) ;
  pUser->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &NewPropWind, true) ;

  // if (!bNewFct)
  // 	delete pNewPropWind ;
  string sLang = pContexte->getUserLanguage() ;
  initComponentsListe(sLang, pContexte->PathName("FGLO"), false) ;
  AfficheListe() ;
}

/*void
NSFunctionsListWindow::CmCreerEnreg()
{
    NSFunctionsManagementIter iterFunct= pFctNameList->begin();
    NSCorrespondentsNamesArray *pCorrespName = new NSCorrespondentsNamesArray();
    for(iterFunct; (iterFunct != pFctNameList->end()) ; iterFunct++)
    {
        NSCorrespondentsNames *pNames = new NSCorrespondentsNames();
        pNames->setFonction((*iterFunct)->getFonction());
        pNames->setNomFctUtilisateur((*iterFunct)->getNomFctUtilisateur());
        pCorrespName->push_back(pNames);
    }

    NSFunctionsManagement* pParamsDevelop = new NSFunctionsManagement();

    NSIntroModifFunctions *pEnregDlg = new NSIntroModifFunctions(this, pContexte, pParamsDevelop, pCorrespName );
    if (pEnregDlg->Execute() != IDOK)
    {
        delete pParamsDevelop ;
        return ;
        delete pCorrespName;
    }
    delete pEnregDlg;
    delete pCorrespName;
    EnregData(pParamsDevelop) ;

}  */


void
NSFunctionsListWindow::CmModifEnreg()
{
	int index = IndexItemSelect() ;

  if (index == -1)  {
  	string sErrMsg = pContexte->getSuperviseur()->getText("functionManagement", "YouMustSelectAFunction") ;
		erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  int i = 0 ;
  NSFunctionsManagementIter iterFunct= pFctNameList->begin() ;
  for(iterFunct; ((iterFunct != pFctNameList->end()) && (i!=index)); iterFunct++)
  	i++ ;
  NSFunctionsManagement* pParamsDevelop ;
  if (iterFunct != pFctNameList->end())
  	pParamsDevelop = (*iterFunct) ;
  iterFunct= pFctNameList->begin() ;

  NSCorrespondentsNamesArray CorrespName ;
  for (iterFunct; iterFunct != pFctNameList->end() ; iterFunct++)
  {
  	NSCorrespondentsNames *pNames = new NSCorrespondentsNames() ;
    pNames->setFonction((*iterFunct)->getFonction()) ;
    pNames->setNomFctUtilisateur((*iterFunct)->getNomFctUtilisateur()) ;
    CorrespName.push_back(pNames) ;
  }

  NSIntroModifFunctions *pEnregDlg = new NSIntroModifFunctions(this, pContexte, pParamsDevelop, &CorrespName) ;
  if (pEnregDlg->Execute() == IDOK)
  	EnregData(pParamsDevelop) ;
  delete pEnregDlg ;
}

void
NSFunctionsListWindow::CmSupprEnreg()
{
	int index = IndexItemSelect() ;
  if (index == -1)
  {
		string sErrMsg = pContexte->getSuperviseur()->getText("functionManagement", "YouMustSelectAFunction") ;
		erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  ArrayWinProp aCopyWindsProp = *(pContexte->getUtilisateur()->getWindowPropertyArray()) ;

  if ((pFctNameList->empty()) || (aCopyWindsProp.empty()))
  	return ;

  int i = 0 ;
  NSFunctionsManagementIter iterFunct = pFctNameList->begin() ;
  for( ; (pFctNameList->end() != iterFunct) && (i < index) ; iterFunct++)
  	i++ ;
  string sFct = (*iterFunct)->getFonction() ;
  ArrayWPIter iterProp = aCopyWindsProp.begin() ;
  for ( ; (aWindsPropLoc.end() != iterProp) && ((*iterProp)->getFunction() != sFct) ; iterProp++) ;
  if (aCopyWindsProp.end() != iterProp)
  {
  	delete (*iterProp) ;
    aCopyWindsProp.erase(iterProp) ;
    (*iterProp) = aCopyWindsProp.back() ;
  }

  *(pContexte->getUtilisateur()->getWindowPropertyArray()) = aCopyWindsProp ;

  string sLang = pContexte->getUserLanguage() ;
  initComponentsListe(sLang, pContexte->PathName("FGLO"), false) ;
  AfficheListe() ;
}

void
NSFunctionsListWindow::CmModifParam()
{
	int index = IndexItemSelect() ;

  if (-1 == index)  {
  	string sErrMsg = pContexte->getSuperviseur()->getText("functionManagement", "YouMustSelectAFunction") ;
		erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  int i = 0 ;
  NSFunctionsManagementIter iterFunct = pFctNameList->begin() ;
  for( ; (pFctNameList->end() != iterFunct) && (i != index) ; iterFunct++, i++) ;
  if (pFctNameList->end() == iterFunct)
  	return ;

  NSFunctionsManagement* pParamsDevelop = (*iterFunct) ;

  NSWindowProperty::FCTACTIVITY iActivity = pParamsDevelop->getActivity() ;
  char                          cHotKey   = pParamsDevelop->getHotKey() ;
  string                        sModifier = pParamsDevelop->getModifiers() ;

  NSParamsModifFunctions *pEnregDlg = new NSParamsModifFunctions(this, pContexte, &iActivity, &cHotKey, &sModifier) ;
  if (pEnregDlg->Execute() == IDOK)
  {
  	pParamsDevelop->setActivity(iActivity) ;
  	pParamsDevelop->setHotKey(cHotKey) ;
    pParamsDevelop->setModifiers(sModifier) ;

    bool bMustSave = pContexte->getUtilisateur()->isEnregWin() ;
    pContexte->getUtilisateur()->setEnregWin(true) ;
  	EnregData(pParamsDevelop) ;
    pContexte->getUtilisateur()->setEnregWin(bMustSave) ;
  }
  delete pEnregDlg ;
}

bool
NSFunctionsListWindow::canWeClose()
{
	return true ;
}


//------------------------------------------------------------------------------
//                      classe NSSplitListWindow
//------------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSSplitListWindow, TListWindow)
    EV_WM_SETFOCUS,
    EV_WM_RBUTTONDOWN,
  	EV_WM_LBUTTONDBLCLK,  //double click
    NS_LVN_GETDISPINFO(LvnGetDispInfo),
    EV_COMMAND(IDC_NEWWIND, CmCreerEnreg),
    EV_COMMAND(IDC_MODWIND, CmModifEnreg),
    EV_COMMAND(IDC_DELWIND, CmSupprEnreg),
END_RESPONSE_TABLE;

NSSplitListWindow::NSSplitListWindow(TWindow* parent, NSContexte* pCtx, int resourceId,
                           NSFunctionsManagement* pParam, NSCorrespondentsNamesArray *pCorrespName,
                           TModule* module)
             :TListWindow(parent, resourceId, module), NSRoot(pCtx)
{

    iSortedColumn = -1 ;
    pDialog = parent;
    if(pParam)
        pParamInit = pParam;
    pListNames =  pCorrespName;
   
    Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS;
    Attr.ExStyle |= WS_EX_NOPARENTNOTIFY;

}

NSSplitListWindow::~NSSplitListWindow()
{

}

void
NSSplitListWindow::EvSetFocus(HWND hWndLostFocus)
{
}

void
NSSplitListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
    // création d'un menu popup
    NS_CLASSLIB::TPoint lp = point;

    TPopupMenu *menu = new TPopupMenu();

    menu->AppendMenu(MF_STRING, IDC_NEWWIND, "Créer");
    menu->AppendMenu(MF_STRING, IDC_MODWIND, "Modifier");
  	menu->AppendMenu(MF_STRING, IDC_DELWIND, "Supprimer");

    ClientToScreen(lp);
  	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow);
  	delete menu;
}

void
NSSplitListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    TLwHitTestInfo info(point);

    HitTest(info);
    if (info.GetFlags() & LVHT_ONITEM)        CmModifEnreg();}

void
NSSplitListWindow::SetupWindow()
{
    ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

    SetupColumns();

    AfficheListe();
    TListWindow::SetupWindow();
}

void
NSSplitListWindow::SetupColumns()
{
    NS_CLASSLIB:: TRect  rectList ;

    InsertColumn(0, TListWindColumn("Fonction fenetre mere", 120));
    InsertColumn(1, TListWindColumn("Fonction fenetre fille", 110));
    InsertColumn(2, TListWindColumn("Direction", 90));
    InsertColumn(3, TListWindColumn("Pourcentage", 80));


}

int
NSSplitListWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)    {
   	    if (GetItemState(i, LVIS_SELECTED))        {
      	    index = i;
            break;
        }
    }

    return index;}


void
NSSplitListWindow::initListe()
{
   /* ArraySplitWindow* pSplitProps = pParamInit->getListSplit();
    if (pSplitProps)
        return;*/
   // for(ArraySWIter iterSplit = pSplitProps->begin(); iterSplit != pSplitProps->end(); iterSplit++)

}

void
NSSplitListWindow::AfficheListe()
{

	DeleteAllItems();    if (!pParamInit)
        return;
    ArraySplitWindow* pSplitProps = pParamInit->getListSplit();
    if ((!pSplitProps)||(pSplitProps->empty()))
        return;
    if(!pListNames)
        return;

    ArraySplitWindow ::reverse_iterator itReverse ;
    itReverse = pSplitProps->rbegin() ;

    while (itReverse != pSplitProps->rend())
    {
        string sFctName = (*itReverse)->getMotherWindow();
        if (sFctName != "")
        {
            NSCorrespondentsNamesIter iterNames = pListNames->begin();
            for(iterNames; ((iterNames != pListNames->end())&&((*iterNames)->getFonction() != sFctName)); iterNames++);
            string sUserFct = "";
            if (iterNames != pListNames->end())
            {
                sUserFct =  (*iterNames)->getNomFctUtilisateur();
                TListWindItem Item(sUserFct.c_str() , 0) ;
                InsertItem(Item);
            }
        }
        itReverse++ ;
    }
}

void
NSSplitListWindow::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
    TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;
    int index = dispInfoItem.GetIndex();

    if (!pParamInit)
        return;

    ArraySplitWindow* pSplitProps = pParamInit->getListSplit();
    if ((!pSplitProps)||(pSplitProps->empty()))
        return;
    if(!pListNames)
        return;
    ArraySWIter iterSWind= pSplitProps->begin();
    int i = 0;
    for(iterSWind; ((iterSWind != pSplitProps->end()) && (i<index)); iterSWind++)
        i++;
    string sChildWindName = "";
    TSplitDirection sDirection = psNone;
    float fPercent = 0.0;
    if ((iterSWind != pSplitProps->end()) && (i == index))
    {
        sChildWindName = (*iterSWind)->getChildWindow();
        sDirection = (*iterSWind)->getSplitDirection();
        fPercent = (*iterSWind)->getPercent();
    }

    // Affiche les informations en fonction de la colonne    switch (dispInfoItem.GetSubItem())
    {
        case 1: // fenetre fille

           {
            if (sChildWindName == "")
                return;

            NSCorrespondentsNamesIter iterNames = pListNames->begin();
            for(iterNames; ((iterNames != pListNames->end())&&((*iterNames)->getFonction() != sChildWindName)); iterNames++);
            string sUserFct = "";
            if (iterNames != pListNames->end())
            {
                sUserFct =  (*iterNames)->getNomFctUtilisateur();
                dispInfoItem.SetText(sUserFct.c_str(), strlen(sUserFct.c_str()));
            }
            }
            break;
        case 2: // direction
            {
                string sDir = "";
                if (sDirection == psHorizontal)
                    sDir = string("Horizontale ( - )") ;
                else
                    if (sDirection == psVertical)
                       sDir = string("Verticale ( | )") ;
                dispInfoItem.SetText(sDir.c_str(), strlen(sDir.c_str()));
            }
            break;
       case 3: // pourcentage
            {
               int dec, sign, ndig = 2;
               char *str;               str = fcvt(fPercent, ndig, &dec, &sign);
               dispInfoItem.SetText(str, strlen(str));
            }
            break;

    }
}


void
NSSplitListWindow::CmCreerEnreg()
{
	NSSplittingWindowProperty NewProp ;
  NSCorrespondentsNamesArray CopyListName(*pListNames) ;
  NSMakeFunctions *pDlg = new NSMakeFunctions(pDialog, pContexte, &NewProp, &CopyListName) ;
  if (pDlg->Execute() != IDOK)
  {
  	delete pDlg ;
    return ;
  }
  if (!pParamInit)
  	return ;

	ArraySplitWindow* pSplitProps = pParamInit->getListSplit() ;
  pSplitProps->push_back(new NSSplittingWindowProperty(NewProp)) ;
  delete pDlg ;
  AfficheListe() ;
}

void
NSSplitListWindow::CmModifEnreg()
{
    int index = IndexItemSelect();

   	if (index == -1)   	{
   		erreur("Vous devez sélectionner un complément à modifier.", standardError, 0) ;
      	return;
   	}
    if (!pParamInit)
        return;

    //iNoCombinForModif = index;

    NSIntroModifFunctions *pDlg = TYPESAFE_DOWNCAST(pDialog, NSIntroModifFunctions);
    if(! pDlg)
        return;
  //  pDlg->setIndexModif(index);

    ArraySplitWindow* pSplitProps = pParamInit->getListSplit();
    if ((!pSplitProps)||(pSplitProps->empty()))
        return;
    if(!pListNames)
        return;
    ArraySWIter iterSWind= pSplitProps->begin();
    int i = 0;
    for(iterSWind; ((iterSWind != pSplitProps->end()) && (i<index)); iterSWind++)
        i++;
   // NSSplittingWindowProperty *pNewProp = new NSSplittingWindowProperty();
    if ((iterSWind != pSplitProps->end()) && (i == index))
    {
        NSMakeFunctions *pDlg = new NSMakeFunctions(pDialog,pContexte,(*iterSWind),pListNames);
        if (pDlg->Execute() != IDOK)
        {
        	delete pDlg ;
          return ;
        }
        if (!pParamInit)
            return;

        delete pDlg;
       // delete pNewProp;
        AfficheListe();
    }

}

void
NSSplitListWindow::CmSupprEnreg()
{
	int index = IndexItemSelect();
  if (index == -1)
  {
  	erreur("Vous devez sélectionner un complément à supprimer.", standardError, 0) ;
      	return;
   	}
    ArraySplitWindow* pSplitProps = pParamInit->getListSplit();
    if ((!pSplitProps)||(pSplitProps->empty()))
        return;
    ArraySWIter iterSWind= pSplitProps->begin();
    int i = 0;
    for(iterSWind; ((iterSWind != pSplitProps->end()) && (i<index)); iterSWind++)
        i++;
    if ((iterSWind != pSplitProps->end()) && (i == index))
    {
        delete (*iterSWind) ;
        pSplitProps->erase(iterSWind) ;
        (*iterSWind) = pSplitProps->back();
    }
     AfficheListe();
}


//***********************************************************************//
//							Classe NSMakeFunctions
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSMakeFunctions, TDialog)
	EV_COMMAND(IDOK,            CmOk),
    EV_COMMAND(IDCANCEL,	CmCancel),
END_RESPONSE_TABLE;

NSMakeFunctions::NSMakeFunctions ( TWindow* pere, NSContexte* pCtx,
                                    NSSplittingWindowProperty *pPropInt,
                                    NSCorrespondentsNamesArray *pCorrespName,
                                    TModule* module)
	:	TDialog(pere, IDD_INTROPROP, module),
		NSRoot(pCtx)
{
    //pPropWind = new NSSplittingWindowProperty(*pPropInt);
    if(!pPropInt)
        return;
    pPropWind = pPropInt;
    pNamesList =  pCorrespName;

    pEditMother = new NSComboBoxWindowsName(this,IDC_MOTHER, pNamesList);
    pEditChild = new NSComboBoxWindowsName(this,IDC_CHILD, pNamesList);
    pEditPercent = new TEdit(this,IDC_PERCENT);
    pVerticalButton = new TRadioButton (this, IDC_VERTICAL);
    pHorizontalButton = new TRadioButton (this, IDC_HORIZONTAL);

}


NSMakeFunctions::~NSMakeFunctions()
{
	delete pEditMother;
    delete pEditChild;
    delete pVerticalButton;
    delete pHorizontalButton;
    delete pEditPercent;
}

void
NSMakeFunctions::SetupWindow()
{
    TDialog::SetupWindow();
    if (!pPropWind)
        return;
    pEditMother->setCode(pPropWind->getMotherWindow());
    pEditChild->setCode(pPropWind->getChildWindow());
    if (pPropWind->getSplitDirection() == psHorizontal)
        pHorizontalButton->Check();
    else
        if (pPropWind->getSplitDirection() == psVertical)
            pVerticalButton->Check();
    float fPercent = pPropWind->getPercent();
    int dec, sign, ndig = 2;
    char *str;    str = fcvt(fPercent, ndig, &dec, &sign);
    if(fPercent != 1.0)
        pEditPercent->SetText(str);
}

bool
NSMakeFunctions::validInput()
{
  int  iBuffLen = pEditPercent->GetTextLen() ;
  char *pszBuff = new char[iBuffLen + 2] ;
  pEditPercent->GetText(pszBuff, iBuffLen + 1) ;
  string sPercent = string(pszBuff) ;
  delete[] pszBuff ;
  if ( (pEditMother->getSelCode() == "")   ||
       (pEditChild->getSelCode() == "")    ||
       (sPercent == "")  ||
       ((pHorizontalButton->GetCheck() == BF_UNCHECKED )&&
       (pVerticalButton->GetCheck() == BF_UNCHECKED  )) )
    return false ;
    
  return true ;
}

void
NSMakeFunctions::CmOk()
{
  if (validInput())
  {
    pPropWind->setMotherWindow(string(pEditMother->getSelCode())) ;
    pPropWind->setChildWindow(string(pEditChild->getSelCode())) ;

    int  iBuffLen = pEditPercent->GetTextLen() ;
    char *pszBuff = new char[iBuffLen + 2] ;
    pEditPercent->GetText(pszBuff, iBuffLen + 1) ;
    //  string sPercent = string(pszBuff);

    int iPercent = atoi(pszBuff) ;
    float fPercent = float(iPercent) ;
    fPercent = fPercent/100 ;
    pPropWind->setPercent(fPercent) ;

    if (pHorizontalButton->GetCheck() > 0 )
      pPropWind->setSplitDirection(psHorizontal) ;
    else if(pVerticalButton->GetCheck() > 0 )
      pPropWind->setSplitDirection(psVertical) ;
    delete[] pszBuff ;
  }

  TDialog::CmOk() ;
}

void
NSMakeFunctions::CmCancel()
{
	Destroy(IDCANCEL) ;
}

//--------------------------------------------------------------------------
//Class NSHistorizedListWindow
//--------------------------------------------------------------------------
NSFunctionsManagement:: NSFunctionsManagement()
{
  sFonction = "" ;
  sComplement = "" ;
  sNomFctUtilisateur = "" ;
  //pListSplit = NULL;
  pListSplit = new ArraySplitWindow() ;
}

NSFunctionsManagement::~NSFunctionsManagement()
{
}

//---------------------------------------------------------------------------
//      class NSHistoryManagementArray
//---------------------------------------------------------------------------

NSFunctionsManagementArray::NSFunctionsManagementArray(NSFunctionsManagementArray& rv)
                          : NSFunctManArray()
{
try
{
	if (false == rv.empty())
		for (NSFunctionsManagementIter i = rv.begin(); rv.end() != i ; i++)
			push_back(*i) ;
}
catch (...)
{
	erreur("Exception NSFunctionsManagementArray ctor.", standardError, 0) ;
}
}


void
NSFunctionsManagementArray :: vider()
{
	if (empty())
  	return ;

	for (NSFunctionsManagementIter i = begin(); end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSFunctionsManagementArray::~NSFunctionsManagementArray()
{
	vider() ;
}


//--------------------------------------------------------------------------
//              Classe NSCorrespondentsNames
//--------------------------------------------------------------------------
NSCorrespondentsNames:: NSCorrespondentsNames()
{
	sFonction = "" ;
  sNomFctUtilisateur = "" ;
}

NSCorrespondentsNames::~NSCorrespondentsNames()
{
}

//---------------------------------------------------------------------------
//      class NSHistoryManagementArray
//---------------------------------------------------------------------------

NSCorrespondentsNamesArray::NSCorrespondentsNamesArray(const NSCorrespondentsNamesArray& rv)
                           :NSCorArray()
{
try
{
	if (false == rv.empty())
  	for (NSCorrespondentsNamesConstIter It = rv.begin(); rv.end() != It ; It++)
    	push_back(new NSCorrespondentsNames(*(*It))) ;
}
catch (...)
{
	erreur("Exception NSCorrespondentsNamesArray ctor.", standardError, 0) ;
}
}

NSCorrespondentsNamesArray&
NSCorrespondentsNamesArray::operator=(const NSCorrespondentsNamesArray& src)
{
try
{
  if (&src == this)
		return *this ;

	vider() ;

  if (false == src.empty())
    for (NSCorrespondentsNamesConstIter It = src.begin(); src.end() != It ; It++)
    	push_back(new NSCorrespondentsNames(*(*It))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSCorrespondentsNamesArray = operator.", standardError, 0) ;
  return *this ;
}
}

void
NSCorrespondentsNamesArray :: vider()
{
	if (empty())
		return ;

	for (NSCorrespondentsNamesIter i = begin(); end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSCorrespondentsNamesArray::~NSCorrespondentsNamesArray()
{
  vider() ;
}


DEFINE_RESPONSE_TABLE1(NSComboBoxWindowsName, TComboBox)
   EV_WM_KEYUP,
   EV_WM_KEYDOWN,
   EV_WM_GETDLGCODE,
END_RESPONSE_TABLE;

NSComboBoxWindowsName::NSComboBoxWindowsName(TWindow *parent, int resId,
                            NSCorrespondentsNamesArray *pNames)
                        : TComboBox(parent, resId)
{
  pNamesList   = new NSCorrespondentsNamesArray() ;
  pDlgParent  = parent ;
  sSelectedName = "" ;

  if (NULL == pNames)
    return ;
  /*
  if (pVecteurString && !pVecteurString->empty())
  {
    for (EquiItemIter iter = pVecteurString->begin() ; iter != pVecteurString->end() ; iter++)
      pLexiqCodes->push_back(new string(**iter)) ;
  }
  */
  if (false == pNames->empty())
    for (NSCorrespondentsNamesIter iter = pNames->begin() ; iter != pNames->end() ; iter++)
      pNamesList->push_back(new NSCorrespondentsNames(**iter)) ;
}

NSComboBoxWindowsName::~NSComboBoxWindowsName()
{
  pNamesList->vider() ;
  delete pNamesList ;
}

void
NSComboBoxWindowsName::SetupWindow()
{
  TComboBox::SetupWindow() ;

  if (false == pNamesList->empty())
    for (NSCorrespondentsNamesIter iter = pNamesList->begin() ; pNamesList->end() != iter ; iter++)
      AddString(((*iter)->getNomFctUtilisateur()).c_str()) ;
}

string
NSComboBoxWindowsName::getSelCode()
{
  // récupétion de l'index de la sélection au sein du combobox
  int index = GetSelIndex() ;
  string sSel = string("") ;

  // récupération du code lexique
  if (false == pNamesList->empty())
  {
    NSCorrespondentsNamesIter iter ;
    int iIter ;

    for (iter = pNamesList->begin(), iIter = 0 ; (pNamesList->end() != iter) && (index != iIter) ; iter++, iIter++)
    ;
    if ((pNamesList->end() != iter) && (iIter == index))
      sSel = (*iter)->getFonction() ;
  }
  sSelectedName = sSel ;
  return sSel ;
}

void
NSComboBoxWindowsName::setCode(string sValue)
{
  if (false == pNamesList->empty())
  {
    NSCorrespondentsNamesIter iter ;
    int iIter ;

    for (iter = pNamesList->begin(), iIter = 0 ;
            (pNamesList->end() != iter) && ((*iter)->getFonction() != sValue) ;
                                                        iter++, iIter++)
    ;
    if ((pNamesList->end() != iter) && ((*iter)->getFonction() == sValue))
    {
        SetSelIndex(iIter) ;
        return ;
        // sSelectedName = (*iter)->getFonction();
    }
  }
  SetSelIndex(-1) ;
}

//***********************************************************************//
//							Classe NSParamsModifFunctions
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSParamsModifFunctions, TDialog)
	EV_COMMAND(IDOK,     CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSParamsModifFunctions::NSParamsModifFunctions(TWindow* pere,
                                               NSContexte* pCtx,
                                               NSWindowProperty::FCTACTIVITY* piActivity,
                                               char* pcHotKey, string* psMods, TModule* module)
	:	TDialog(pere, "IDD_FUNCTPARAM", module),
		NSRoot(pCtx)
{
	pActivite = new TGroupBox(this, IDC_PROPGROUP) ;
  pInactive = new TRadioButton(this, IDC_PROPI, pActivite) ;
  pActive   = new TRadioButton(this, IDC_PROPA, pActivite) ;
  pAuto     = new TRadioButton(this, IDC_PROPO, pActivite) ;
  pAutoBBk  = new TRadioButton(this, IDC_PROPB, pActivite) ;
  pHotKey   = new TGroupBox(this, IDC_ALTGROUP) ;
  pEditFct  = new TEdit(this, IDC_ALT) ;
  pModAlt   = new TRadioButton(this, IDC_MOD_ALT) ;
  pModCtrl  = new TRadioButton(this, IDC_MOD_CTRL) ;
  pModShift = new TRadioButton(this, IDC_MOD_SHIFT) ;
  pModWin   = new TRadioButton(this, IDC_MOD_WIN) ;

  _piActivity  = piActivity ;
  _pcHotKey    = pcHotKey ;
  _psModifiers = psMods ;

	if (NULL != psMods)
		_winProperty.setModifiers(*psMods) ;
}

NSParamsModifFunctions::~NSParamsModifFunctions()
{
	delete pActivite ;
  delete pInactive ;
  delete pActive ;
  delete pAuto ;
  delete pAutoBBk ;
  delete pHotKey ;
  delete pEditFct ;
  delete pModAlt ;
  delete pModCtrl ;
  delete pModShift ;
  delete pModWin ;
}

void
NSParamsModifFunctions::SetupWindow()
{
	TDialog::SetupWindow() ;

  string sText = pContexte->getSuperviseur()->getText("functionParamsManagement", "openingProperties") ;
  pActivite->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("functionParamsManagement", "notAvailable") ;
  pInactive->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("functionParamsManagement", "available") ;
  pActive->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("functionParamsManagement", "openedWithPatient") ;
  pAuto->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("functionParamsManagement", "blackboardedWithPatient") ;
  pAutoBBk->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("functionParamsManagement", "hotKeyParameters") ;
  pHotKey->SetCaption(sText.c_str()) ;

  pInactive->Uncheck() ;
  pActive->Uncheck() ;
  pAuto->Uncheck() ;
  pAutoBBk->Uncheck() ;

  if (_piActivity)
	{
  	switch(*_piActivity)
		{
  		case NSWindowProperty::notAvailable :
    		pInactive->Check() ;
      	break ;
    	case NSWindowProperty::available :
    		pActive->Check() ;
      	break ;
    	case NSWindowProperty::openedWithPatient :
    		pAuto->Check() ;
      	break ;
    	case NSWindowProperty::blackboardedWithPatient :
    		pAutoBBk->Check() ;
     	 	break ;
		}
	}

  char szText[2] ;
  if (_pcHotKey)
  	szText[0] = *_pcHotKey ;
  else
  	szText[0] = '\0' ;
  szText[1] = '\0' ;
  pEditFct->SetText(szText) ;

	pModAlt->Uncheck() ;
  pModCtrl->Uncheck() ;
  pModShift->Uncheck() ;
  pModWin->Uncheck() ;

	if (_winProperty.isInModifier(MOD_ALT))
		pModAlt->Check() ;
  if (_winProperty.isInModifier(MOD_CONTROL))
		pModCtrl->Check() ;
	if (_winProperty.isInModifier(MOD_SHIFT))
		pModShift->Check() ;
	if (_winProperty.isInModifier(MOD_WIN))
		pModWin->Check() ;
}

void
NSParamsModifFunctions::CmOk()
{
	if (NULL != _piActivity)
	{
		if      (pInactive->GetCheck() == BF_CHECKED)
    	*_piActivity = NSWindowProperty::notAvailable ;
    else if (pActive->GetCheck() == BF_CHECKED)
    	*_piActivity = NSWindowProperty::available ;
    else if (pAuto->GetCheck() == BF_CHECKED)
    	*_piActivity = NSWindowProperty::openedWithPatient ;
    else if (pAutoBBk->GetCheck() == BF_CHECKED)
    	*_piActivity = NSWindowProperty::blackboardedWithPatient ;
    else
  		*_piActivity = NSWindowProperty::undefined ;
  }

  if (NULL != _pcHotKey)
  {
  	char szText[2] ;
  	pEditFct->GetText(szText, 2 /*maxChars*/) ;
    *_pcHotKey = szText[0] ;
  }

	if (NULL != _psModifiers)
	{
  	_winProperty.resetModifiers() ;
    if (BF_CHECKED == pModAlt->GetCheck())
    	_winProperty.addToModifiers(MOD_ALT) ;
    if (BF_CHECKED == pModCtrl->GetCheck())
    	_winProperty.addToModifiers(MOD_CONTROL) ;
    if (BF_CHECKED == pModShift->GetCheck())
    	_winProperty.addToModifiers(MOD_SHIFT) ;
    if (BF_CHECKED == pModWin->GetCheck())
    	_winProperty.addToModifiers(MOD_WIN) ;

    *_psModifiers = _winProperty.getModifiers() ;
	}

	TDialog::CmOk() ;
}


void
NSParamsModifFunctions::CmCancel()
{
	Destroy(IDCANCEL) ;
}

