// -----------------------------------------------------------------------------// nsgoalview.cpp
// Vue Document/Vues de gestion des objectifs Doc/View View for goal management
// -----------------------------------------------------------------------------
// FLP - Janvier/Février 2004
// PA  - Juillet 2003
// -----------------------------------------------------------------------------

#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

#include <owl/uihelper.h>
#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>

#include "nautilus\nssuper.h"#include "partage\nsdivfct.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsldvvar.h"
#include "nautilus\nsldvgoal.h"
#include "nautilus\nsFollowUpView.h"
#include "nautilus\nautilus.rh"
#include "nsbb\nspanesplitter.h"
#include "nsbb\nsattvaltools.h"
#include "nautilus\nsldvvue.rh"
#include "nsepisod\objectif_viewer.h"

NSFollowUpElement::NSFollowUpElement(NSContexte *pCtx, NSLdvGoal* pTheGoal)
                  :NSRoot(pCtx), ElementsArray(pCtx)
{
	pGoal = pTheGoal ;

	if (pTheGoal)
	{
		sLexique = pTheGoal->getLexique() ;

    string sLang = pContexte->getUserLanguage() ;
		pContexte->getDico()->donneShortLibelle(sLang, &sLexique, &sColumnLabel) ;
	}

  bSortOrderByDateInf = true ;
}

NSFollowUpElement::~NSFollowUpElement()
{
}

bool
NSFollowUpElement::initIterToBegin()
{
	if (true == ElementsArray.empty())
		return false ;

	currentIter = ElementsArray.begin() ;

  return true ;
}

bool
NSFollowUpElement::initIterToEnd()
{
	if (true == ElementsArray.empty())
		return false ;

	currentIter = ElementsArray.end() ;
  currentIter-- ;

  return true ;
}

bool
NSFollowUpElement::nextIter()
{
	if (ElementsArray.end() == currentIter)
		return false ;

	currentIter++ ;
  if (ElementsArray.end() == currentIter)
	{
		currentIter-- ;
		return false ;
	}

	return true ;
}

bool
NSFollowUpElement::previousIter()
{
	if (ElementsArray.begin() == currentIter)
		return false ;

	currentIter-- ;

	return true ;
}

void
NSFollowUpElement::sortArray()
{
	if (true == bSortOrderByDateInf)
		sortArrayByDateInf() ;
	else
		sortArrayByDateSup() ;
}

void
NSFollowUpElement::sortArrayByDateInf()
{
	sort(ElementsArray.begin(), ElementsArray.end(), sortByDateInf) ;
}

void
NSFollowUpElement::sortArrayByDateSup()
{
	sort(ElementsArray.begin(), ElementsArray.end(), sortByDateSup) ;
}

string
NSFollowUpElement::getCurrentIterDate()
{
	return (*currentIter)->getDate() ;
}

void
NSFollowUpElement::insertBlankDate(string sNewDate)
{
	// bookmark currentIter
	//
  NSHistoryValueManagement* pCurrentElement = 0 ;

	if (NULL != currentIter)
  	pCurrentElement = *currentIter ;

	NSHistoryValueManagement* pNewElement = new NSHistoryValueManagement() ;
  pNewElement->setDate(sNewDate) ;
  ElementsArray.push_back(pNewElement) ;
  sortArray() ;

	// reinit currentIter
  //
	if (NULL != pCurrentElement)
		for (currentIter = ElementsArray.begin() ; (ElementsArray.end() != currentIter) && (pCurrentElement != *currentIter) ; currentIter++) ;
}

void
NSFollowUpElement::setLexique(string sNewLex, bool bUpdateLabel)
{
	sLexique = sNewLex ;

	if (true == bUpdateLabel)
	{
		string sLang = pContexte->getUserLanguage() ;
		pContexte->getDico()->donneShortLibelle(sLang, &sLexique, &sColumnLabel) ;
	}
}

// -----------------------------------------------------------------------------
//
// Class NSGoalView
//
// -----------------------------------------------------------------------------

const int ID_GoalList = 0x100 ;
// Table de réponses de la classe NSEpisodView

DEFINE_RESPONSE_TABLE1(NSFollowUpView, NSMUEView)
  EV_VN_ISWINDOW,
//  EV_LVN_GETDISPINFO(ID_GoalList, DispInfoListe),
  EV_LVN_COLUMNCLICK(ID_GoalList, LVNColumnclick),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
END_RESPONSE_TABLE ;


// Constructeur
NSFollowUpView::NSFollowUpView(NSLdvDocument &doc, string sConcern)
               :NSLDVView(doc.pContexte, &doc, 0, string("GoalsFollowUp"), string("LdvDoc"), sConcern), followUpValues(pContexte)
{
try
{
  pLdVDoc = &doc ;
  pListeWindow = new NSFollowUpPropertyWindow(this, ID_GoalList, 0, 0, 0, 0) ;

  iSortedColumn = 1 ;
  bNaturallySorted = true ; // in order to have it naturally sorted

	initMUEViewMenu("menubar_goal") ;

  pToolBar = 0 ;
  bSetupToolBar = true ;

  initCurrentGoals() ;

  setViewName() ;
}
catch (...)
{
  erreur("Exception NSFollowUpView ctor.", standardError, 0) ;
}
}

// Destructeur
NSFollowUpView::~NSFollowUpView()
{
}

void
NSFollowUpView::setViewName()
{
	sViewName = pContexte->getSuperviseur()->getText("followUpManagement", "followUpBoard") ;

  addConcernTitle() ;
}

void
NSFollowUpView::CmNouveau()
{
   /* NSPatPathoArray* pPathArray = new NSPatPathoArray(pLdVDoc->pContexte);
    ObjectifViewerDlg* objDlg = new ObjectifViewerDlg((TWindow*)this, pLdVDoc->pContexte, true, pPathArray);
    objDlg->Execute();

    delete objDlg ;   */

}

void
NSFollowUpView::CmModifier()
{
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow*
NSFollowUpView::GetWindow()
{
  return (TWindow *) this ;
}

// Appel de la fonction de remplissage de la vue
void
NSFollowUpView::SetupWindow()
{
	NSMUEView::SetupWindow() ;

	Parent->SetCaption(sViewName.c_str()) ;

	SetupColumns() ;
	AfficheListe() ;

	// we want to have a list sorted by severity at the first opening
	// sortByColumn(1) ;
}

// -----------------------------------------------------------------------------
// Initialisation des colonnes de la ListWindow
// -----------------------------------------------------------------------------
void
NSFollowUpView::SetupColumns()
{
	string sNameOb0 = pContexte->getSuperviseur()->getText("generalLanguage", "date") ;
  pListeWindow->InsertColumn( 0, TListWindColumn((char*)sNameOb0.c_str(), 120, TListWindColumn::Left,    0)) ;

  if (followUpIndexes.empty())
    return ;

  NSFollowUpElementIter fuIter = followUpIndexes.begin() ;
  for (int i = 1 ;  followUpIndexes.end() != fuIter; fuIter++, i++)
		pListeWindow->InsertColumn(i, TListWindColumn((char*)(*fuIter)->sColumnLabel.c_str(),80, TListWindColumn::Right, i)) ;
}

void
NSFollowUpView::initCurrentGoals()
{
try
{
  string ps2 = "NSFollowUpView::initCurrentGoals: begin" ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubSteps) ;

	followUpValues.vider() ;
  followUpIndexes.vider() ;

	if (string("") == sPreoccup)
  {
    ps2 = "NSFollowUpView::initCurrentGoals: no concern, leaving" ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubSteps) ;
		return ;
  }

  NSFrameInformationArray *pFrames = pLdVDoc->getFrames() ;
  if (NULL == pFrames)
  {
    ps2 = "NSFollowUpView::initCurrentGoals: no frame, leaving" ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubSteps) ;
    return ;
  }

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    NSFrameInformation *pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;

	  ArrayGoals* pArrayOfGoals = pFrameInfo->getGoalsArray() ;
    if (pArrayOfGoals && (false == pArrayOfGoals->empty()))
    {
	    // Enumerating goals
	    //
	    for (ArrayGoalIter goalIt = pArrayOfGoals->begin() ;
                                pArrayOfGoals->end() != goalIt ; goalIt++)
		    if ((*goalIt)->getConcern() == sPreoccup)
    	    processGoal(*goalIt) ;
    }
  }

  synchronizeElements() ;

  ps2 = "NSFollowUpView::initCurrentGoals: done, leaving" ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubSteps) ;
}
catch (...)
{
	erreur("Exception NSFollowUpView::initCurrentGoals.", standardError, 0) ;
}
}

void
NSFollowUpView::processGoal(NSLdvGoal* pGoal)
{
	if (NULL == pGoal)
		return ;

	// Already processed?
	//
  if (false == followUpIndexes.empty())
	{
  	NSFollowUpElementIter fuIter = followUpIndexes.begin() ;
  	for ( ;  followUpIndexes.end() != fuIter; fuIter++)
    	if (pGoal == (*fuIter)->pGoal)
				return ;
	}

	if (string("") == pGoal->getLexique())
		return ;

  string ps2 = "NSFollowUpView::processGoal: processing goal " + pGoal->getLexique() ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;

  string sGoalConcept = pGoal->getLexique() ;

	// Numerical value
	//
	if ('V' == sGoalConcept[0])
	{
  	string sGoalSens ;
    pContexte->getDico()->donneCodeSens(&sGoalConcept, &sGoalSens) ;

  	// Check if this value has already been processed
		//
    NSFollowUpElementIter fuIter = followUpIndexes.begin() ;
    for ( ; followUpIndexes.end() != fuIter; fuIter++)
    {
    	string sFuSens ;
      pContexte->getDico()->donneCodeSens(&((*fuIter)->sLexique), &sFuSens) ;

      if (sFuSens == sGoalSens)
      	break ;
    }
    if (followUpIndexes.end() != fuIter)
    {
      ps2 = "NSFollowUpView::processGoal: goal already processed, leaving." ;
      pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;
    	return ;
    }

  	// Get history for this value
    //
  	NSHistoryValManagementArray histoManager(pContexte) ;

		histoManager.ChargeHistoryValueItem(pGoal->getLexique()) ;
		histoManager.ChargeCaptureValueItem(pGoal->getLexique()) ;
		if (histoManager.empty())
    {
      ps2 = "NSFollowUpView::processGoal: no value for this goal, leaving." ;
      pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;
			return ;
    }

    // Create a new Follow Up element
    //
    NSFollowUpElement* pFUelt = new NSFollowUpElement(pContexte, pGoal) ;

    NSHistoryValManagementIter histoIter = histoManager.begin() ;
    for (;  histoManager.end() != histoIter; )
    {
    	pFUelt->ElementsArray.push_back(*histoIter) ;
      histoManager.erase(histoIter) ;
    }

    followUpIndexes.push_back(pFUelt) ;
  }
  //
  // Drug and treatment
  //
  else if (('_' == sGoalConcept[0]) || ('I' == sGoalConcept[0]) || ('N' == sGoalConcept[0]))
  {
  	// Find all drugs related to this goal
    //
    string  sConceptSens ;
		pContexte->getDico()->donneCodeSens(&sGoalConcept, &sConceptSens) ;

    // ps2 = "NSFollowUpView::processGoal: get semantic sons." ;
    // pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

    // VectString VSEquivalent ;
		// pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sConceptSens, "ES", &VSEquivalent, "ENVERS") ;

    NSFrameInformationArray *pFrames = pLdVDoc->getFrames() ;
    if (NULL == pFrames)
      return ;

    for (int i = 0 ; i < FRAMECOUNT ; i++)
    {
      NSFrameInformation *pFrameInfo = pFrames->getFrameInformation(getFrameForIndex(i)) ;
      if (pFrameInfo)
      {
        ps2 = "NSFollowUpView::processGoal: get treatment array for frame " + getLabelForFrame(getFrameForIndex(i)) ;
        pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

        ArrayLdvDrugs *pLdvDrugs = pFrameInfo->getTreatmentsArray() ;
        if (pLdvDrugs && (false == pLdvDrugs->empty()))
        {
          drugsIter itDrugs = pLdvDrugs->begin() ;
          for ( ; itDrugs != pLdvDrugs->end() ; itDrugs++)
          {
            string sDrugLexique = (*itDrugs)->getLexique() ;
            string sDrugSens ;
            pContexte->getDico()->donneCodeSens(&sDrugLexique, &sDrugSens) ;

            VectString VSEquivalent ;
            pContexte->getSuperviseur()->getFilGuide()->TousLesVrais(sDrugSens, "ES", &VSEquivalent, "FLECHE") ;

            if (VSEquivalent.contains(sConceptSens))
            {
              ps2 = "NSFollowUpView::processGoal: proper drug found " + sDrugLexique ;
              pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

              bool bNewElement = true ;
              NSFollowUpElement* pFUelt = 0 ;

              // Check if this drug has already been created
              //
              if (false == followUpIndexes.empty())
              {
                NSFollowUpElementIter fuIter = followUpIndexes.begin() ;
                for ( ; followUpIndexes.end() != fuIter; fuIter++)
                {
                  string sFuSens ;
                  pContexte->getDico()->donneCodeSens(&((*fuIter)->sLexique), &sFuSens) ;

                  if (sFuSens == sDrugSens)
                    break ;
                }
                if (followUpIndexes.end() != fuIter)
                {
                  bNewElement = false ;
                  pFUelt = *fuIter ;
                }
              }
              if (NULL == pFUelt)
              {
                pFUelt = new NSFollowUpElement(pContexte, pGoal) ;
                pFUelt->setLexique(sDrugLexique) ;
              }

              // Fill the Follow Up element with treatment's phases
              //
              if (false == (*itDrugs)->_aPhases.empty())
              {
                ps2 = "NSFollowUpView::processGoal: filling follow up element with treatment's phases." ;
                pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

                NSLdvDrugPhaseIter phaseIter = (*itDrugs)->_aPhases.begin() ;
                for ( ; (*itDrugs)->_aPhases.end() != phaseIter ; phaseIter++)
                {
                  NSHistoryValueManagement *pHistoryValue = new NSHistoryValueManagement() ;
                  pHistoryValue->setLabel(sDrugLexique) ;
                  pHistoryValue->setDate((*phaseIter)->_tDateOuverture.donneDateHeure()) ;
                  pHistoryValue->setDateFin((*phaseIter)->_tDateFermeture.donneDateHeure()) ;
                  pHistoryValue->setValue((*phaseIter)->_sTitreCourt) ;

                  pFUelt->ElementsArray.push_back(pHistoryValue) ;
                }
              }
              // Add the Follow Up element to the Index
              //
              if (true == bNewElement)
                followUpIndexes.push_back(pFUelt) ;
            }
          }
        }
      }
    }
  }
  ps2 = "NSFollowUpView::processGoal: done, leaving." ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;
}

// We have to complete all lists in order to have the same dates everywhere
//
void
NSFollowUpView::synchronizeElements()
{
	if (true == followUpIndexes.empty())
		return ;

	string sMaxDate = string("00000000000000") ;

	NSFollowUpElementIter FUiter = followUpIndexes.begin() ;
  for (; FUiter != followUpIndexes.end() ; FUiter++)
	{
  	(*FUiter)->bSortOrderByDateInf = false ;
  	(*FUiter)->sortArray() ;
  	(*FUiter)->initIterToBegin() ;
    string sDate = (*FUiter)->getCurrentIterDate() ;

    if (sMaxDate < sDate)
    	sMaxDate = sDate ;
	}

	NVLdVTemps tpsMaxDate ;
  tpsMaxDate.initFromDate(sMaxDate) ;

  bool bKeepOn = true ;

  while (true == bKeepOn)
  {
  	// We use a NVLdVTemps object in order to be able to compare
    // days-only and day-time dates
    //
    NVLdVTemps tpsNextMaxDate ;
    tpsNextMaxDate.init() ;

		NSFollowUpElementIter FUiter = followUpIndexes.begin() ;
  	for (; FUiter != followUpIndexes.end() ; FUiter++)
		{
    	NVLdVTemps tpsCurrentDate ;

    	string sDate = (*FUiter)->getCurrentIterDate() ;
      tpsCurrentDate.initFromDate(sDate) ;

    	if (!(tpsCurrentDate == tpsMaxDate))
      {
      	(*FUiter)->insertBlankDate(tpsMaxDate.donneDateHeure()) ;
        if ((tpsNextMaxDate < tpsCurrentDate) && (tpsCurrentDate < tpsMaxDate))
        	tpsNextMaxDate = tpsCurrentDate ;
      }
      else
      	if ((*FUiter)->nextIter())
        {
        	sDate = (*FUiter)->getCurrentIterDate() ;
          tpsCurrentDate.initFromDate(sDate) ;

          if (tpsNextMaxDate < tpsCurrentDate)
    				tpsNextMaxDate = tpsCurrentDate ;
        }
		}
    if (true == tpsNextMaxDate.estVide())
    	bKeepOn = false ;
    else
    	tpsMaxDate = tpsNextMaxDate ;
  }
}

voidNSFollowUpView::reloadView(string sReason){  initCurrentGoals() ;  AfficheListe() ;}// Affichage des éléments de la liste
void
NSFollowUpView::AfficheListe()
{
  pListeWindow->DeleteAllItems() ;

  if (followUpIndexes.empty())
    return ;

	string sLang = pContexte->getUserLanguage() ;

	// char szDate[20] ;
  //

	NSFollowUpElementIter FUiter = followUpIndexes.begin() ;
  bool bKeepOn = (*FUiter)->initIterToEnd() ;

  while (true == bKeepOn)
	{
		string sItemDate = (*FUiter)->getCurrentIterDate() ;

    NVLdVTemps tpsCurrentDate ;
		tpsCurrentDate.initFromDate(sItemDate) ;

    char szBuffer[255] ;
    strcpy(szBuffer, tpsCurrentDate.donneFormattedDateHeure(pContexte, sLang).c_str()) ;
    pListeWindow->InsertItem(szBuffer) ;

    bKeepOn = (*FUiter)->previousIter() ;
  }

  // Initing column by column
  //
  int iColIndex = 1 ;

  FUiter = followUpIndexes.begin() ;
  for ( ; followUpIndexes.end() != FUiter ; FUiter++, iColIndex++)
  {
    if (false == (*FUiter)->initIterToBegin())
      continue ;

    int iLineIndex = 0 ;

    bool bKeepOnForCol = true ;
    while (true == bKeepOnForCol)
	  {
      NSHistoryValueManagement* pHistoElt = *((*FUiter)->currentIter) ;

      string sLabel = pHistoElt->getValue() ;
      if ((string("") != sLabel) && (string("") != pHistoElt->getUnit()))
      {
        string sUnit = pHistoElt->getUnit() ;
        pContexte->getDico()->donneCodeComplet(sUnit) ;
        string sLibUnit ;
        pContexte->getDico()->donneLibelle(sLang, &sUnit, &sLibUnit) ;
        sLabel += string(" ") + sLibUnit ;
      }

      TListWindItem Item(sLabel.c_str(), iColIndex) ;
      Item.SetIndex(iLineIndex) ;
      Item.SetSubItem(iColIndex) ;
      pListeWindow->SetItem(Item) ;

      bKeepOnForCol = (*FUiter)->nextIter() ;

      iLineIndex++ ;
    }
  }
}

void
NSFollowUpView::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  const int       BufLen = 255 ;
  static char     szBuffer[BufLen] ;
  TListWindItem   &dispInfoItem = *(TListWindItem *) &dispInfo.item ;
  // char            szDate[20] ;
  szBuffer[0] = '\0' ;

  int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

	int iColumn = dispInfoItem.GetSubItem() ;

	if (0 == iColumn)
	{
  	NSFollowUpElement*        pFuElt    = *(followUpIndexes.begin()) ;
		NSHistoryValueManagement* pHistoElt = pFuElt->ElementsArray[index] ;

		NVLdVTemps tpsCurrentDate ;
		tpsCurrentDate.initFromDate(pHistoElt->getDate()) ;
    strcpy(szBuffer, tpsCurrentDate.donneFormattedDateHeure(pContexte, sLang).c_str()) ;
  }
  else
  {
  	NSFollowUpElement*        pFuElt    = followUpIndexes[iColumn - 1] ;
		NSHistoryValueManagement* pHistoElt = pFuElt->ElementsArray[index] ;

    string sLabel = pHistoElt->getValue() ;

    if ((string("") != sLabel) && (string("") != pHistoElt->getUnit()))
    {
      string sUnit = pHistoElt->getUnit() ;
      pContexte->getDico()->donneCodeComplet(sUnit) ;
      string sLibUnit ;
      pContexte->getDico()->donneLibelle(sLang, &sUnit, &sLibUnit) ;
      sLabel += string(" ") + sLibUnit ;
    }

    strcpy(szBuffer, sLabel.c_str()) ;
  }

	dispInfoItem.SetText(szBuffer) ;
}

void
NSFollowUpView::LVNColumnclick(TLwNotify& lwn)
{
	int iColumn = lwn.iSubItem ;

    // Si cette colonne était déjà triée, c'est qu'on souhaite la trier
    // dans l'ordre inverse
    //
    // If this column was alredy sorted, it means we want to sort it in
    // reverse order
    //
    if (iSortedColumn == iColumn)
	{
    	if (bNaturallySorted)
			bNaturallySorted = false ;
		else
			bNaturallySorted = true ;
	}
	else
		bNaturallySorted = true ;

  	sortByColumn(iColumn) ;
}


// -----------------------------------------------------------------------------
// fonction qui trie les items en fonction d'une colonne
// -----------------------------------------------------------------------------
void
NSFollowUpView::sortByColumn(int iColumn)
{
	iSortedColumn = iColumn ;

/*
	if (aGoals.empty())
		return ;

	switch (iColumn)
	{
		case 0  :
        	if (bNaturallySorted)
        		sort(aGoals.begin(), aGoals.end(), goalSortByNameInf) ;
            else
            	sort(aGoals.begin(), aGoals.end(), goalSortByNameSup) ;
            break ;

    	// -------------------------------------------------------------------------
    	// functions are not in the same order than the other because we want that
    	// in normal case we sort like the greatest severity is the first
    	case 1  :
        	if (bNaturallySorted)
            	sort(aGoals.begin(), aGoals.end(), goalSortByLevelSup) ;
            else
            	sort(aGoals.begin(), aGoals.end(), goalSortByLevelInf) ;
            break ;
    	// -------------------------------------------------------------------------

        case 2  : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByRythmeInf) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByRythmeSup) ;
                  break ;

        case 3  : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByAutoriseSup) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByAutoriseInf) ;
                  break ;

        case 4  : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByConseilleSup) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByConseilleInf) ;
                  break ;

        case 5  : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByIdealSup) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByIdealInf) ;
                  break ;

        case 6  : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByIdealMaxSup) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByIdealMaxInf) ;
                  break ;

        case 7  : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByConseilleMaxSup) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByConseilleMaxInf) ;
                  break ;

        case 8  : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByCriticalSup) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByCriticalInf) ;
                  break ;

        case 9  : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByPreviousSup) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByPreviousInf) ;
                  break ;

        case 10 : if (bNaturallySorted)
                    sort(aGoals.begin(), aGoals.end(), goalSortByPrevValueInf) ;
                  else
                    sort(aGoals.begin(), aGoals.end(), goalSortByPrevValueSup) ;
                  break ;
	}
*/

	AfficheListe() ;
}


boolNSFollowUpView::VnIsWindow(HWND hWnd){
  return (HWindow == hWnd) ;
}


// fonction permettant de prendre toute la taille de TWindow par la TListWindowvoid
NSFollowUpView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
  pListeWindow->MoveWindow(GetClientRect(), true) ;
}


// fonction EVSetFocusvoid
NSFollowUpView::EvSetFocus(HWND hWndLostFocus)
{
	activateMUEViewMenu() ;

  TMyApp    *pMyApp = pContexte->getSuperviseur()->getApplication() ;
  NSMUEView::EvSetFocus(hWndLostFocus) ;
  if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
    SetupToolBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }

  pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;
  pContexte->setAideIndex("") ;
  pContexte->setAideCorps("objectifs.htm") ;
}


// SetupToolBarvoid
NSFollowUpView::SetupToolBar()
{
try
{
	TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	// pMyApp->cb2->Insert(*new TButtonGadget(CM_GOAL_STOP, CM_GOAL_STOP, TButtonGadget::Command)) ;

	pMyApp->getSecondaryControlBar()->LayoutSession() ;
}
catch (...)
{
	erreur("Exception NSFollowUpView::SetupToolBar.", standardError, 0) ;
}
}


// -----------------------------------------------------------------------------
//
//  					Méthodes de NSDrugsPropertyWindow//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSFollowUpPropertyWindow, TListWindow)  EV_WM_SETFOCUS,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
END_RESPONSE_TABLE ;


NSFollowUpPropertyWindow::NSFollowUpPropertyWindow(NSFollowUpView *parent, int id, int x, int y, int w, int h, TModule *module)
                      :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
	pView   = parent ;
	iRes    = id ;
	Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
	//Attr.ExStyle |= WS_EX_NOPARENTNOTIFY | LVS_EX_FULLROWSELECT ;

	Images  = 0 ;
}

NSFollowUpPropertyWindow::~NSFollowUpPropertyWindow()
{
	if (Images)
		delete Images ;
}

void
NSFollowUpPropertyWindow::SetupWindow()
{
	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

	TListWindow::SetupWindow() ;

	HINSTANCE hInstModule = *GetApplication() ;

  Images = new TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR4, 15, 5) ;
  Images->Add(OWL::TBitmap(hInstModule, GOAL_EARLY_RED)) ;
  Images->Add(OWL::TBitmap(hInstModule, GOAL_EARLY_YELLOW)) ;
  Images->Add(OWL::TBitmap(hInstModule, GOAL_EARLY_GREEN)) ;
  Images->Add(OWL::TBitmap(hInstModule, GOAL_BLUE)) ;
  Images->Add(OWL::TBitmap(hInstModule, GOAL_GREEN)) ;
  Images->Add(OWL::TBitmap(hInstModule, GOAL_YELLOW)) ;
  Images->Add(OWL::TBitmap(hInstModule, GOAL_RED)) ;
  SetImageList(*Images, TListWindow::State) ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSFollowUpPropertyWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;
  //if (info.GetFlags() & LVHT_ONITEM)  //  pDlg->CmModifier();
}


void
NSFollowUpPropertyWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
try
{
	TListWindow::EvLButtonDown(modkeys,point) ;

	//int count = GetItemCount() ;
	TLwHitTestInfo info(point) ;

	//int index = HitTest(info) ;

	TPopupMenu *menu = new TPopupMenu() ;

	// We can't do that since we don't have a concern to attach the new goal
  //sTitle = pView->pContexte->getSuperviseur()->getText("goalsManagement", "goalCreate") ;
	//menu->AppendMenu(MF_STRING, CM_GOAL_NEW, sTitle.c_str());

  string sTitle = pView->pContexte->getSuperviseur()->getText("goalsManagement", "goalModify") ;
	menu->AppendMenu(MF_STRING, CM_GOAL_CHANGE, sTitle.c_str());

  sTitle = pView->pContexte->getSuperviseur()->getText("goalsManagement", "goalTraitSelobj") ;

 	sTitle = pView->pContexte->getSuperviseur()->getText("goalsManagement", "goalClose") ;
	menu->AppendMenu(MF_STRING, CM_GOAL_STOP, sTitle.c_str());

	ClientToScreen(point) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
	delete menu ;
}
catch (...)
{
	erreur("Exception NSGoalsPropertyWindow::EvRButtonDown.", standardError, 0) ;
}
}

void
NSFollowUpPropertyWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}


// -----------------------------------------------------------------------------//  Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSFollowUpPropertyWindow::IndexItemSelect()
{
  int count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)    if (GetItemState(i, LVIS_SELECTED))
    {
      index = i ;
      break ;
    }

  return index ;}

void
NSFollowUpPropertyWindow::EvSetFocus(HWND hWndLostFocus)
{
  pView->EvSetFocus(hWndLostFocus) ;
}

