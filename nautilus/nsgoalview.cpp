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
#include "nautilus\nsgoalview.h"
#include "nautilus\nscompub.h"
#include "nautilus\nscqdoc.h"
#include "nautilus\nsCoopChartDV.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nautilus.rh"
#include "nsbb\nspanesplitter.h"
#include "nsbb\nsattvaltools.h"
#include "nautilus\nsldvvue.rh"
#include "nsepisod\objectif_viewer.h"
#include "nsepisod\nsToDo.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsfltbtnga.h"
#include "nssavoir\nsgraphe.h"

bool
goalSortByNameInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getName() < pObj2->getName()) ;
}

bool
goalSortByNameSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getName() > pObj2->getName()) ;
}

bool
goalSortByLevelInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
  return (pObj1->getLevel() < pObj2->getLevel()) ;
}

bool
goalSortByLevelSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
  return (pObj1->getLevel() > pObj2->getLevel()) ;
}

bool
goalSortByRythmeInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getRythme() < pObj2->getRythme()) ;
}

bool
goalSortByRythmeSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj2->getRythme() > pObj1->getRythme()) ;
}

bool
goalSortByAutoriseInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateAuto() < pObj2->getDateAuto()) ;
}

bool
goalSortByAutoriseSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateAuto() > pObj2->getDateAuto()) ;
}

bool
goalSortByConseilleInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateCons() < pObj2->getDateCons()) ;
}

bool
goalSortByConseilleSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
    if (!pObj1 || !pObj2)
        return false ;
	return (pObj1->getDateCons() > pObj2->getDateCons()) ;
}

bool
goalSortByIdealInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateIdeal() < pObj2->getDateIdeal()) ;
}

bool
goalSortByIdealSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateIdeal() > pObj2->getDateIdeal()) ;
}

bool
goalSortByIdealMaxInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateIdealMax() < pObj2->getDateIdealMax()) ;
}

bool
goalSortByIdealMaxSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateIdealMax() > pObj2->getDateIdealMax()) ;
}

bool
goalSortByConseilleMaxInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateConsMax() < pObj2->getDateConsMax()) ;
}

bool
goalSortByConseilleMaxSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateConsMax() > pObj2->getDateConsMax()) ;
}

bool
goalSortByCriticalInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateCrit() < pObj2->getDateCrit()) ;
}

bool
goalSortByCriticalSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDateCrit() > pObj2->getDateCrit()) ;
}

bool
goalSortByPreviousInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDatePrec() < pObj2->getDatePrec()) ;
}

bool
goalSortByPreviousSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getDatePrec() > pObj2->getDatePrec()) ;
}

bool
goalSortByPrevValueInf(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getValue() < pObj2->getValue()) ;
}

bool
goalSortByPrevValueSup(NSObjectif *pObj1, NSObjectif *pObj2)
{
  if (!pObj1 || !pObj2)
    return false ;
	return (pObj1->getValue() > pObj2->getValue()) ;
}

// -----------------------------------------------------------------------------
//
// Class NSGoalView
//
// -----------------------------------------------------------------------------

const int ID_GoalList = 0x100 ;
// Table de réponses de la classe NSEpisodView

DEFINE_RESPONSE_TABLE1(NSGoalView, NSMUEView)
  EV_VN_ISWINDOW,
  // EV_LVN_GETDISPINFO(ID_GoalList, DispInfoListe),
  EV_LVN_COLUMNCLICK(ID_GoalList, LVNColumnclick),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_COMMAND(CM_REFERENTIAL,     CmFct3),
  EV_COMMAND(CM_GOAL_NEW,        CmNouveau),
  EV_COMMAND(CM_GOAL_CHANGE,     CmModifier),
  EV_COMMAND(CM_GOAL_TRAIT,      CmTraitSelectedObj),
  EV_COMMAND(CM_GOAL_STOP,       CmClore),
  EV_COMMAND(CM_GOAL_VALIDATE,   CmFct4),
  EV_COMMAND(IDC_NEW_REF,        CmFct5),
  EV_COMMAND(IDC_ADD_TO_REF,     CmFct6),
  EV_COMMAND(CM_GOAL_INVALIDATE, CmFct7),
  EV_COMMAND(CM_IMPRIME,         CmPublish),
END_RESPONSE_TABLE ;


// Constructeur
NSGoalView::NSGoalView(NSLdvDocument &doc, string sConcern)
  : NSLDVView(doc.pContexte, &doc, 0, string("GoalManagement"), string("LdvDoc"), sConcern)
{
try
{
  pLdVDoc = &doc ;
  pListeWindow = new NSGoalsPropertyWindow(this, ID_GoalList, 0, 0, 0, 0) ;

  iSortedColumn = 1 ;
  bNaturallySorted = true ; // in order to have it naturally sorted

	initMUEViewMenu("menubar_goal") ;

  pToolBar = 0 ;
  bSetupToolBar = true ;

  sViewConcern = sConcern ;

  initCurrentGoals() ;

  setViewName() ;
}
catch (...)
{
  erreur("Exception NSGoalView ctor.", standardError, 0) ;
}
}


// Destructeur
NSGoalView::~NSGoalView()
{
}

void
NSGoalView::setViewName()
{
	sViewName = pContexte->getSuperviseur()->getText("goalsManagement", "goalViewTitle") ;

  addConcernTitle() ;
}

void
NSGoalView::CmNouveau()
{
/*
  NSLdvGoal ldvGoal(pLdVDoc->pContexte) ;
  ObjectifViewerDlg* objDlg = new ObjectifViewerDlg((TWindow*)this, pLdVDoc->pContexte, true, &ldvGoal) ;
  int iResult = objDlg->Execute() ;
  delete objDlg ;

  if (IDOK != iResult)
    return ;

  NSPatPathoArray pathArray(pLdVDoc->pContexte->getSuperviseur()) ;
	ldvGoal.setGoalPatho(&pathArray) ;

  if (true == pathArray.empty())
    return ;

  pContexte->getPatient()->CreeObjectif(&pathArray, "", "", "", NULL, "") ;

	initCurrentGoals() ;
	AfficheListe() ;
*/
}

void
NSGoalView::CmModifier()
{
try
{
	//  NSPatPathoArray* pPathArray = new NSPatPathoArray(pLdVDoc->pContexte);
	int index = pListeWindow->IndexItemSelect() ;

	if (index < 0)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "mustSelectGoalToModify") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}
    //NSObjectif* objMod = aGoals[index];

  LDVFRAME iFrame = ldvframeNotSpecified ;
	string sGoalNode = (aGoals[index])->getNode() ;

	NSLdvGoal *pLdvGoal = pLdVDoc->getGoal(sGoalNode, iFrame) ;
  if (NULL == pLdvGoal)
    return ;

	NSLdvGoal* pNewCopy = new NSLdvGoal(*pLdvGoal) ;
	ObjectifViewerDlg* objDlg = new ObjectifViewerDlg((TWindow*)this, pLdVDoc->pContexte, false, pNewCopy) ;
	int iOkOrCancel = objDlg->Execute() ;
	delete objDlg ;

  if (IDOK != iOkOrCancel)
    return ;

	//if unchanged goal -> exit
	if (pLdvGoal->estIdentique(pNewCopy))
		return ;

	NSPatPathoArray PathArray(pNewCopy->pContexte->getSuperviseur()) ;
	pNewCopy->setGoalPatho(&PathArray) ;

	pLdVDoc->closeGoal(pLdvGoal, NULL) ;

	//creer nouveau objectif
	pContexte->getPatient()->CreeObjectif(&PathArray, "", "", "", NULL, iFrame, true, pNewCopy->getNode()) ;

	initCurrentGoals() ;
	AfficheListe() ;
}
catch (...)
{
  erreur("Exception NSGoalView::CmModifier", standardError, 0) ;
}
}

void
NSGoalView::CmClore()
{
try
{
	if (!(pContexte->userHasPrivilege(NSContexte::modifyGoal, -1, -1, string(""), string(""), NULL, NULL)))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return ;
	}

	int index = pListeWindow->IndexItemSelect() ;

	if (index < 0 /* == -1 */)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "StopMustSelect") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

	NSPatPathoArray StopReason(pContexte->getSuperviseur()) ;

# ifdef __OB1__
	NSSmallBrother BigBoss(pContexte, &StopReason, 0) ;
#else
	NSSmallBrother BigBoss(pContexte, &StopReason, 0, true) ;
#endif

	BigBoss.setMotherWindow(this) ;
#ifdef _MUE
	string sArchID = string("admin.goal.close.1.0") ;
# ifdef __OB1__
	BB1BBInterfaceForKs InterfaceForKs(-1, sArchID, "", false) ;
	/* NSDialog *pClientWin = */ BigBoss.lanceBbkArchetypeInDialog(sArchID, 0, 0, &InterfaceForKs, true /*modal*/) ;
# else
	/* NSDialog *pClientWin = */ BigBoss.lanceBbkArchetype(sArchID, 0, 0, true /*modal*/) ;
# endif
#else
	/* NSDialog *pClientWin = */ BigBoss.lanceBbkArchetype("sfmg.dpio.test.fermeture.1.0", 0, 0, true /*modal*/) ;
#endif

	if (StopReason.empty())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "StopNeedReason") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

	string sGoalNode = (aGoals[index])->getNode() ;

  LDVFRAME iFrame = ldvframeNotSpecified ;
  NSLdvGoal  *pLdvGoal = pLdVDoc->getGoal(sGoalNode, iFrame) ;
  if (NULL == pLdvGoal)
    return ;

	pLdVDoc->closeGoal(pLdvGoal, &StopReason) ;

	initCurrentGoals() ;
	AfficheListe() ;

	return ;
}
catch (...)
{
	erreur("Exception NSGoalView::CmArreter.", standardError, 0) ;
}
}

void
NSGoalView::CmFct3()
{
/*
	VecteurString RelatedConcerns ;
  if (sPreoccup != string(""))
  	RelatedConcerns.push_back(new string(sPreoccup)) ;

	pLdVDoc->GoalFromProtocolService(this, &RelatedConcerns) ;
*/
}

// Validate goal
//
void
NSGoalView::CmFct4()
{
try
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  int index = pListeWindow->IndexItemSelect() ;
	if (index < 0)
	{
		string sErrorText = pSuper->getText("goalsManagement", "mustSelectGoal") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

  LDVFRAME iFrame = ldvframeNotSpecified ;
	string sGoalNode = (aGoals[index])->getNode() ;

	NSLdvGoal *pLdvGoal = pLdVDoc->getGoal(sGoalNode, iFrame) ;
  if (NULL == pLdvGoal)
  {
    string ps = string("Entering goal validation, but goal not found... leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
    erreur(ps.c_str(), standardError, 0) ;
    return ;
  }

  string ps = string("Entering goal validation for goal ") + pLdvGoal->getTitle() + string(" (") + pLdvGoal->getNode() + string(")") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  string sGoalLexique = pLdvGoal->getLexique() ;

  string sWhat  = string("") ;
  string sWhere = string("") ;
  pSuper->getWhereToFind(sGoalLexique, sWhat, sWhere) ;

  // Goal can be satisfied through an Archetype
  //
  if (string("Archetype") == sWhat)
  {
    string ps = string("Goal validation, opening archetype ") + sWhere ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

    NSToDoTask *pTask = new NSToDoTask() ;
    pTask->BbkAskUser(pContexte->getSuperviseur(), sWhere, NSCQDocument::standard) ;
    return ;
  }

  // Goal can be satisfied with a quality of life form
  //
  if (string("QoL") == sWhat)
  {
    string sConcernNode = string("") ;
    if (string("") != sViewConcern)
    {
      NSFrameInformation *pFrameInfo = pLdVDoc->getFrameForNode(sViewConcern) ;
      if (pFrameInfo)
      {
        LDVFRAME iFrame = pFrameInfo->getFrame() ;
        NSConcern *pConcern = pLdVDoc->getConcern(sViewConcern, iFrame) ;
        if (pConcern)
          sConcernNode = pConcern->getNoeud() ;
      }
    }

    string ps = string("Goal validation, opening QoL form ") + sWhere ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

    NSCoopChartDocument* pDocNewCoopChart = new NSCoopChartDocument(0, pContexte, sWhere, sConcernNode) ;
    NSDocViewManager dvManager(pContexte) ;
	  dvManager.createView(pDocNewCoopChart, "Coop Chart") ;

    return ;
  }

  // Drug or treatment, add it to Ligne de vie
  //
  if (pContexte->getDico()->isDrug(&sGoalLexique))
  {
    NSLdvDocument* pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
    if (pLdvDoc)
    {
      VecteurString aRelatedConcerns ;
      if (string("") != pLdvGoal->getConcern())
  	    aRelatedConcerns.AddString(pLdvGoal->getConcern()) ;

      pLdvDoc->DrugNewService(this, sGoalLexique, &aRelatedConcerns) ;

      return ;
    }
  }

  // Cannot find a way to satisfy the goal... simply create a note
  //

  ps = string("Goal validation, creating a dedicated note.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  NSNoyauDocument ValidationNote(0, 0, 0, pContexte, false, true) ;

  NSPatPathoArray PptNote(pContexte->getSuperviseur()) ;
  ValidationNote.initFromPatPatho(&PptNote) ;

  // First, build a patpatho
  //
  PptNote.ajoutePatho("ZCS001", 0) ;
  PptNote.ajoutePatho("0SOA21", 1) ;
  PptNote.ajoutePatho(pLdvGoal->getLexique(), 2) ;

  ValidationNote.setPatPatho(&PptNote) ;

  string sDocLabel = string("Validation objectif ") + pLdvGoal->getTitle() ;

  bool bExistInfo = ValidationNote.Referencer("ZCS00", sDocLabel, "", "", false /* bDocVisible */, false /* bVerbose */) ;
  if (false == bExistInfo)
  {
    ps = string("Goal validation: function Referencer failed, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
    return ;
  }

	// Recording patpatho
	bExistInfo = ValidationNote.enregistrePatPatho() ;
	if (false == bExistInfo)
	{
    ps = string("Goal validation: function enregistrePatPatho failed, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
		return ;
	}

  // Loading the tree in order to get nodes' Ids
  //
	bExistInfo = ValidationNote.chargePatPatho() ;
	if (false == bExistInfo)
	{
    ps = string("Goal validation: function chargePatPatho failed, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
		return ;
	}

  ValidationNote.initFromPatPatho(&PptNote) ;
  if (PptNote.empty())
	{
    ps = string("Goal validation: function chargePatPatho failed, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
		return ;
	}

  string sPath = string("ZCS001") + string(1, cheminSeparationMARK) + string("0SOA21") + string(1, cheminSeparationMARK) + pLdvGoal->getLexique() ;

  PatPathoIter iter = PptNote.begin() ;
  if (PptNote.CheminDansPatpatho(sPath, string(1, cheminSeparationMARK), &iter))
  {
    string sRootNode = (*iter)->getNode() ;

    NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

    // If reset relation has already been set, there is nothing to be done
    //
    if (pGraphe->existeLien(sRootNode, NSRootLink::goalReseter, pLdvGoal->getNode()))
      return ;

    pGraphe->etablirLien(sRootNode, NSRootLink::goalReseter, pLdvGoal->getNode()) ;
  }

  pLdVDoc->reinitAllGoals() ;
  reloadView("Goal validated") ;
}
catch (...)
{
	erreur("Exception NSGoalView validate goal", standardError, 0) ;
}
}

// Invalidate a goal
//
void
NSGoalView::CmFct7()
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  int index = pListeWindow->IndexItemSelect() ;
	if (index < 0)
	{
		string sErrorText = pSuper->getText("goalsManagement", "mustSelectGoal") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

try
{
  LDVFRAME iFrame = ldvframeNotSpecified ;
	string sGoalNode = (aGoals[index])->getNode() ;

	NSLdvGoal *pLdvGoal = pLdVDoc->getGoal(sGoalNode, iFrame) ;
  if (NULL == pLdvGoal)
  {
    string ps = string("Entering goal invalidation, but goal not found... leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
    erreur(ps.c_str(), standardError, 0) ;
    return ;
  }

  string ps = string("Entering goal invalidation for goal ") + pLdvGoal->getTitle() + string(" (") + pLdvGoal->getNode() + string(")") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  ps = string("Goal invalidation, creating a dedicated note.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  NSNoyauDocument ValidationNote(0, 0, 0, pContexte, false, true) ;

  NSPatPathoArray PptNote(pContexte->getSuperviseur()) ;
  ValidationNote.initFromPatPatho(&PptNote) ;

  // First, build a patpatho
  //
  PptNote.ajoutePatho("ZCS001", 0) ;
  PptNote.ajoutePatho("0SOA21", 1) ;

  Message Msg ;
  Msg.SetLexique(pLdvGoal->getLexique()) ;
  Msg.SetCertitude(string("WCE001")) ;
  PptNote.ajoutePatho(pLdvGoal->getLexique(), &Msg, 2) ;

  ValidationNote.setPatPatho(&PptNote) ;

  string sDocLabel = string("Invalidation objectif ") + pLdvGoal->getTitle() ;

  bool bExistInfo = ValidationNote.Referencer("ZCS00", sDocLabel, "", "", false /* bDocVisible */, false /* bVerbose */) ;
  if (false == bExistInfo)
  {
    ps = string("Goal invalidation: function Referencer failed, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
    return ;
  }

	// Recording patpatho
	bExistInfo = ValidationNote.enregistrePatPatho() ;
	if (false == bExistInfo)
	{
    ps = string("Goal invalidation: function enregistrePatPatho failed, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
		return ;
	}

  // Loading the tree in order to get nodes' Ids
  //
	bExistInfo = ValidationNote.chargePatPatho() ;
	if (false == bExistInfo)
	{
    ps = string("Goal invalidation: function chargePatPatho failed, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
		return ;
	}

  ValidationNote.initFromPatPatho(&PptNote) ;
  if (PptNote.empty())
	{
    ps = string("Goal invalidation: function chargePatPatho failed, leaving") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
		return ;
	}

  string sPath = string("ZCS001") + string(1, cheminSeparationMARK) + string("0SOA21") + string(1, cheminSeparationMARK) + pLdvGoal->getLexique() + string(1, nodeSeparationMARK) + string("WCE001") ;

  PatPathoIter iter = PptNote.begin() ;
  if (PptNote.CheminDansPatpatho(sPath, string(1, cheminSeparationMARK), &iter))
  {
    string sRootNode = (*iter)->getNode() ;

    NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

    // If reset relation has already been set, there is nothing to be done
    //
    if (pGraphe->existeLien(sRootNode, NSRootLink::goalAlarm, pLdvGoal->getNode()))
      return ;

    pGraphe->etablirLien(sRootNode, NSRootLink::goalAlarm, pLdvGoal->getNode()) ;
  }

  pLdVDoc->reinitAllGoals() ;
  reloadView("Goal invalidated") ;
}
catch (...)
{
	erreur("Exception NSGoalView invalidate goal", standardError, 0) ;
}
}

// Create new guideline file from selection
//
void
NSGoalView::CmFct5()
{
	VecteurString aRefStrings ;

	int count = pListeWindow->GetItemCount() ;

	for (int i = 0 ; i < count ; i++)  	if (pListeWindow->GetItemState(i, LVIS_SELECTED))
    	aRefStrings.AddString((aGoals[i])->getNode()) ;

  pLdVDoc->GoalCreateProtocolForSelectionService(this, &aRefStrings) ;
  AfficheListe() ;
}

// Add selection to existing guideline file
//
void
NSGoalView::CmFct6()
{
	VecteurString aRefStrings ;

	int count = pListeWindow->GetItemCount() ;

	for (int i = 0 ; i < count ; i++)  	if (pListeWindow->GetItemState(i, LVIS_SELECTED))
    	aRefStrings.AddString((aGoals[i])->getNode()) ;

  pLdVDoc->GoalAddToProtocolForSelectionService(this, &aRefStrings) ;
  AfficheListe() ;
}

void
NSGoalView::CmPublish()
{
  NSLdvPubli publiDriver ;
  publiDriver._iPrintType  = NSLdvPubli::printGoals ;
  publiDriver._pPrintArray = (void*) &aGoals ;

  pLdVDoc->Publish(&publiDriver) ;
}

// GetWindow renvoie this (à redéfinir car virtual dans TView)
TWindow
*NSGoalView::GetWindow()
{
  return (TWindow *) this ;
}


// Appel de la fonction de remplissage de la vuevoid
NSGoalView::SetupWindow()
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
NSGoalView::SetupColumns()
{
	string sNameOb0 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalName") ;
  string sNameOb1 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalColor") ;
  string sNameOb2 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalRythme") ;
  string sNameOb3 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalPermited") ;
  string sNameOb4 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalAdvised") ;
  string sNameOb5 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalPerfect") ;
  string sNameOb6 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalPerfectMax") ;
  string sNameOb7 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalAdvisedMax") ;
  string sNameOb8 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalCritical") ;
  string sNameOb9 = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalPrevious") ;
  string sNameObA = pContexte->getSuperviseur()->getText("goalsManagement", "colGoalPrevValue") ;

  pListeWindow->InsertColumn( 0,  TListWindColumn((char*)sNameOb0.c_str(), 150, TListWindColumn::Left,    0)) ;
  pListeWindow->InsertColumn( 1,  TListWindColumn((char*)sNameOb1.c_str(),  20, TListWindColumn::Center,  1)) ;
  pListeWindow->InsertColumn( 2,  TListWindColumn((char*)sNameOb2.c_str(),  65, TListWindColumn::Left,    2)) ;
  pListeWindow->InsertColumn( 3,  TListWindColumn((char*)sNameOb3.c_str(),  75, TListWindColumn::Left,    3)) ;
  pListeWindow->InsertColumn( 4,  TListWindColumn((char*)sNameOb4.c_str(),  75, TListWindColumn::Left,    4)) ;
  pListeWindow->InsertColumn( 5,  TListWindColumn((char*)sNameOb5.c_str(),  75, TListWindColumn::Left,    5)) ;
  pListeWindow->InsertColumn( 6,  TListWindColumn((char*)sNameOb6.c_str(),  75, TListWindColumn::Left,    6)) ;
  pListeWindow->InsertColumn( 7,  TListWindColumn((char*)sNameOb7.c_str(),  75, TListWindColumn::Left,    7)) ;
  pListeWindow->InsertColumn( 8,  TListWindColumn((char*)sNameOb8.c_str(),  75, TListWindColumn::Left,    8)) ;
  pListeWindow->InsertColumn( 9,  TListWindColumn((char*)sNameOb9.c_str(),  75, TListWindColumn::Left,    9)) ;
  pListeWindow->InsertColumn(10,  TListWindColumn((char*)sNameObA.c_str(),  75, TListWindColumn::Right,  10)) ;
}

void
NSGoalView::initCurrentGoals()
{
try
{
	aGoals.vider() ;

  for (int iFrame = 0 ; iFrame < FRAMECOUNT ; iFrame++)
  {
    ArrayGoals *pLdvGoals = pLdVDoc->getGoals(getFrameForIndex(iFrame)) ;
    if (pLdvGoals && (false == pLdvGoals->empty()))
    {
      for (ArrayGoalIter j = pLdvGoals->begin() ; pLdvGoals->end() != j ; j++)
      {
  	    NSObjectif *pObj = new NSObjectif ;

        if (InitObjectif(pObj, *j))
        {
    	    aGoals.push_back(pObj) ;
          //(*j)->selectObjectif();
        }
        else
    	    delete pObj ;
      }
	  }
  }
  sortByColumn(iSortedColumn) ;
}
catch (...)
{
	erreur("Exception NSGoalView::initCurrentGoals.", standardError, 0) ;
}
}

void
NSGoalView::CmTraitSelectedObj()
{
	pLdVDoc->traiteSelectedObjectifs(sViewConcern) ;
}

bool
NSGoalView::SelectedObjectifs()
{
  int count = pListeWindow->GetItemCount() ;
  bool selectionFound = false ;

  //on reinitialise les selections
  for (int iFrame = 0 ; iFrame < FRAMECOUNT ; iFrame++)
  {
    ArrayGoals *pLdvGoals = pLdVDoc->getGoals(getFrameForIndex(iFrame)) ;
    if (pLdvGoals && (false == pLdvGoals->empty()))
      for (ArrayGoalIter iter = pLdvGoals->begin(); iter != pLdvGoals->end(); iter++)
  	    (*iter)->unselectObjectif() ;
  }

  for (int iGoal = 0 ; iGoal < count ; iGoal++)  {
    // If this goal is selected
    //
    if (pListeWindow->GetItemState(iGoal, LVIS_SELECTED))
    {
      bool found = false ;

      for (int iFrame = 0 ; (iFrame < FRAMECOUNT) && (false == found) ; iFrame++)
      {
        ArrayGoals *pLdvGoals = pLdVDoc->getGoals(getFrameForIndex(iFrame)) ;
        if (pLdvGoals && (false == pLdvGoals->empty()))
        {
          ArrayGoalIter iter = pLdvGoals->begin() ;
          while ((pLdvGoals->end() != iter) && (false == found))
          {
      	    if ((*iter)->estIdentique(aGoals[iGoal]->_pCorrespGoal))
            {
        	    selectionFound = true ;
              found = true ;
              (*iter)->selectObjectif() ;
            }
            /* else
        	    (*iter)->unselectObjectif(); */
            iter++ ;
          }
        }
      }
    }
  }
  return selectionFound ;
}
bool
NSGoalView::InitObjectif(NSObjectif *pObj, NSLdvGoal *pGoal){	if ((NULL == pObj) || (NULL == pGoal))		return false ;  // Si on cherche les objectifs d'une préoccupation précise, on vérifie  // que cet objectif est concerné  if ((string("") != sPreoccup) && (sPreoccup != pGoal->getConcern()))    return false ;	pObj->_pCorrespGoal = pGoal ;  // cas d'un objectif futur non prévisible  if (pGoal->_tOuvertLe.estVide())    return false ;  NVLdVTemps tToday ;  tToday.takeTime() ;  // cas d'un objectif déjà fermé  if ((false == pGoal->_tFermeLe.estVide()) && (pGoal->_tFermeLe <= tToday))    return false ;  bool IsOpened = (tToday >= pGoal->_tOuvertLe) ;	if (string("") != pGoal->_sComplementText)		pObj->_sName = pGoal->_sComplementText ;	else		pObj->_sName = pGoal->getTitle() ;  pObj->_sNode   = pGoal->getNode() ;  // le titre dans Goal est en clair (pas de code lexique)  pObj->_sCode   = "" ;  pObj->_sValue  = "" ;  pObj->_iLevel  = 0 ;  pObj->_iRythme = pGoal->_iRythme ;  // Note : un objectif cyclique non ouvert (futur) se comporte comme  // un objectif ponctuel  if      ((pGoal->_iRythme == NSLdvGoal::ponctuel) || (!IsOpened))  {    InitDateObjectifPonctuel(pObj->_sDateAuto,      pGoal, pGoal->_sDateDebutAutorise,   pGoal->_dDelaiDebutAutorise,   pGoal->_sUniteDebutAutorise) ;    InitDateObjectifPonctuel(pObj->_sDateCons,      pGoal, pGoal->_sDateDebutConseille,  pGoal->_dDelaiDebutConseille,  pGoal->_sUniteDebutConseille) ;    InitDateObjectifPonctuel(pObj->_sDateIdeal,     pGoal, pGoal->_sDateDebutIdeal,      pGoal->_dDelaiDebutIdeal,      pGoal->_sUniteDebutIdeal) ;    InitDateObjectifPonctuel(pObj->_sDateIdealMax,  pGoal, pGoal->_sDateDebutIdealMax,   pGoal->_dDelaiDebutIdealMax,   pGoal->_sUniteDebutIdealMax) ;    InitDateObjectifPonctuel(pObj->_sDateConsMax,   pGoal, pGoal->_sDateDebutConseilMax, pGoal->_dDelaiDebutConseilMax, pGoal->_sUniteDebutConseilMax) ;    InitDateObjectifPonctuel(pObj->_sDateCrit,      pGoal, pGoal->_sDateDebutCritique,   pGoal->_dDelaiDebutCritique,   pGoal->_sUniteDebutCritique) ;    string sDateOuvert = pGoal->_tOuvertLe.donneDate() ;    if      ((pGoal->_sDateDebutCritique    != "")  && (pGoal->_sDateDebutCritique    <= sDateOuvert))      pObj->_iLevel = NSLdvGoalInfo::AProuge ;
    else if ((pGoal->_sDateDebutConseilMax  != "")  && (pGoal->_sDateDebutConseilMax  <= sDateOuvert))
      pObj->_iLevel = NSLdvGoalInfo::APjaune ;
    else if ((pGoal->_sDateDebutIdealMax    != "")  && (pGoal->_sDateDebutIdealMax    <= sDateOuvert))
      pObj->_iLevel = NSLdvGoalInfo::APvert ;
    else if ((pGoal->_sDateDebutIdeal       != "")  && (pGoal->_sDateDebutIdeal       <= sDateOuvert))
      pObj->_iLevel = NSLdvGoalInfo::Bleu ;
    else if ((pGoal->_sDateDebutConseille   != "")  && (pGoal->_sDateDebutConseille   <= sDateOuvert))
      pObj->_iLevel = NSLdvGoalInfo::AVvert ;
    else if ((pGoal->_sDateDebutAutorise    != "")  && (pGoal->_sDateDebutAutorise    <= sDateOuvert))
      pObj->_iLevel = NSLdvGoalInfo::AVjaune ;
    else
      pObj->_iLevel = NSLdvGoalInfo::AVrouge ;
  }
  else if (NSLdvGoal::cyclic == pGoal->_iRythme)  {    // -------------------------------------------------------------------------    // cas des objectifs cycliques déjà ouverts et non déjà fermés    // case in which cyclic goals are open but not yet closed    bool bContinue = true ;    bool bNow      = false ;    for (GoalInfoRIter i = pGoal->getMetaJalons()->rbegin() ; pGoal->getMetaJalons()->rend() != i ; i++)    {      if ((false == bContinue) && bNow)        break ;      if (((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonNow) && !bNow)      {        pObj->_iLevel = (*i)->iLevel ;        bNow          = true ;      }      if (((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonIntermediaire) && bContinue)      {        if      ((*i)->iTimeLevel == NSLdvGoalInfo::AProuge)          pObj->_sDateCrit      = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::APjaune)          pObj->_sDateConsMax   = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::APvert)          pObj->_sDateIdealMax  = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::Bleu)          pObj->_sDateIdeal     = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::AVvert)          pObj->_sDateCons      = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::AVjaune)          pObj->_sDateAuto      = (*i)->tpsInfo.donneDate() ;      }      else if ((((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonCycle) ||                ((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonOuvreCycle)) && bContinue)      {        pObj->_sDatePrec  = (*i)->tpsInfo.donneDate() ;        if ((*i)->getValue() != "")        {          char    szValue[256] ;          string  sLabelUnit ;          string  sCodeUnit = (*i)->sUnit + '1' ;          pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sCodeUnit, &sLabelUnit) ;          sprintf(szValue, "%s %s", (*i)->sValue.c_str(), sLabelUnit.c_str()) ;          pObj->_sValue = string(szValue) ;        }        bContinue = false ;      }      else if ((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonOuverture)        bContinue = false ;    }  }  else if (pGoal->_iRythme == NSLdvGoal::permanent)  {    // -------------------------------------------------------------------------    // cas des objectifs permanents déjà ouverts et non déjà fermés    // case in which permanent goals are open but not yet closed    bool  bContinue = true ;    bool  bNow      = false ;    for (GoalInfoRIter i = pGoal->getMetaJalons()->rbegin() ; pGoal->getMetaJalons()->rend() != i ; i++)    {      if (!bContinue && bNow)        break ;      if (((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonNow) && !bNow)      {        pObj->_iLevel = (*i)->iLevel ;        bNow          = true ;      }      if (((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonIntermediaire) && bContinue)      {        if      ((*i)->iTimeLevel == NSLdvGoalInfo::AProuge)          pObj->_sDateCrit      = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::APjaune)          pObj->_sDateConsMax   = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::APvert)          pObj->_sDateIdealMax  = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::Bleu)          pObj->_sDateIdeal     = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::AVvert)          pObj->_sDateCons      = (*i)->tpsInfo.donneDate() ;        else if ((*i)->iTimeLevel == NSLdvGoalInfo::AVjaune)          pObj->_sDateAuto      = (*i)->tpsInfo.donneDate() ;      }      else if ((((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonCycle) || ((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonOuvreCycle)) && bContinue)        bContinue = false ;      else if ((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonOuverture)        bContinue = false ;    }  }  return true ;}voidNSGoalView::InitDateObjectifPonctuel(string& sDateObj, NSLdvGoal *pGoal, string sDateJalon, double dDelaiJalon, string sUniteJalon)
{
  NVLdVTemps tpsBuff ;
  sDateObj = "" ;

  if      (sDateJalon != "")
    sDateObj = sDateJalon ;  else if (dDelaiJalon > 0)  {    if (pGoal->_sOpenEventNode == "")    {      tpsBuff.initFromDate(pGoal->_tDateOuverture.donneDate()) ;      tpsBuff.ajouteTemps(int(dDelaiJalon), sUniteJalon, pContexte) ;      sDateObj = tpsBuff.donneDate() ;    }    else if (false == pGoal->_tOuvertLe.estVide())    {      tpsBuff.initFromDate(pGoal->_tOuvertLe.donneDate()) ;      tpsBuff.ajouteTemps(int(dDelaiJalon), sUniteJalon, pContexte) ;      sDateObj = tpsBuff.donneDate() ;    }  }}voidNSGoalView::reloadView(string sReason){  initCurrentGoals() ;  AfficheListe() ;}// Affichage des éléments de la liste
void
NSGoalView::AfficheListe()
{
  pListeWindow->DeleteAllItems() ;

  if (aGoals.empty())
    return ;

  string sLang = pContexte->getUserLanguage() ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NSObjectifIter itGoal = aGoals.end() ;
  do
  {
    itGoal-- ;
    TListWindItem Item(((*itGoal)->getName()).c_str(), 0) ;
    switch ((*itGoal)->getLevel())
    {
        case NSLdvGoalInfo::AVrouge : Item.SetStateImage(0) ; break ;
        case NSLdvGoalInfo::AVjaune : Item.SetStateImage(1) ; break ;
        case NSLdvGoalInfo::AVvert  : Item.SetStateImage(2) ; break ;
        case NSLdvGoalInfo::Bleu    : Item.SetStateImage(3) ; break ;
        case NSLdvGoalInfo::APvert  : Item.SetStateImage(4) ; break ;
        case NSLdvGoalInfo::APjaune : Item.SetStateImage(5) ; break ;
        case NSLdvGoalInfo::AProuge : Item.SetStateImage(6) ; break ;
    }
    pListeWindow->InsertItem(Item) ;
  }
  while (itGoal != aGoals.begin()) ;

  string sCyclic = pContexte->getSuperviseur()->getText("goalsManagement", "cyclic") ;
  string sPerman = pContexte->getSuperviseur()->getText("goalsManagement", "permanent") ;
  string sOnce   = pContexte->getSuperviseur()->getText("goalsManagement", "once") ;

  int iLineIndex = 0 ;
	for (itGoal = aGoals.begin() ; itGoal != aGoals.end() ; iLineIndex++, itGoal++)
	{
    int iColIndex = 1 ;

    // State indicator

    char szState[2] ;
    strcpy(szState, "?") ;

    switch ((*itGoal)->getLevel())
    {
      case NSLdvGoalInfo::AVrouge : strcpy(szState, "r") ; break ;
      case NSLdvGoalInfo::AVjaune : strcpy(szState, "j") ; break ;
      case NSLdvGoalInfo::AVvert  : strcpy(szState, "v") ; break ;
      case NSLdvGoalInfo::Bleu    : strcpy(szState, "b") ; break ;
      case NSLdvGoalInfo::APvert  : strcpy(szState, "V") ; break ;
      case NSLdvGoalInfo::APjaune : strcpy(szState, "J") ; break ;
      case NSLdvGoalInfo::AProuge : strcpy(szState, "R") ; break ;
    }

    TListWindItem Item1(szState, iColIndex) ;
    Item1.SetIndex(iLineIndex) ;
    Item1.SetSubItem(iColIndex) ;
    pListeWindow->SetItem(Item1) ;

    iColIndex++ ;

    // Cycle type

    string sCycleType = string("") ;
    switch ((*itGoal)->getRythme())
    {
      case NSLdvGoal::cyclic    : sCycleType = sCyclic ; break ;
      case NSLdvGoal::permanent : sCycleType = sPerman ; break ;
      case NSLdvGoal::ponctuel  : sCycleType = sOnce ;   break ;
    }

    TListWindItem Item2(sCycleType.c_str(), iColIndex) ;
    Item2.SetIndex(iLineIndex) ;
    Item2.SetSubItem(iColIndex) ;
    pListeWindow->SetItem(Item2) ;

    iColIndex++ ;

    for (int iDateType = 0 ; iDateType < 7 ; iDateType++)
    {
      string sDate = string("") ;
      switch(iDateType)
      {
        case 0 : sDate = (*itGoal)->getDateAuto() ;     break ;
        case 1 : sDate = (*itGoal)->getDateCons() ;     break ;
        case 2 : sDate = (*itGoal)->getDateIdeal() ;    break ;
        case 3 : sDate = (*itGoal)->getDateIdealMax() ; break ;
        case 4 : sDate = (*itGoal)->getDateConsMax() ;  break ;
        case 5 : sDate = (*itGoal)->getDateCrit() ;     break ;
        case 6 : sDate = (*itGoal)->getDatePrec() ;     break ;
      }

      if ((string("") != sDate) && (strlen(sDate.c_str()) < 20))
        sDate = donne_date(sDate, sLang) ;

      TListWindItem ItemDate(sDate.c_str(), iColIndex) ;
      ItemDate.SetIndex(iLineIndex) ;
      ItemDate.SetSubItem(iColIndex) ;
      pListeWindow->SetItem(ItemDate) ;

      iColIndex++ ;
    }

    string sPreviousValue = (*itGoal)->getValue() ;

    TListWindItem ItemPrevValue(sPreviousValue.c_str(), iColIndex) ;
    ItemPrevValue.SetIndex(iLineIndex) ;
    ItemPrevValue.SetSubItem(iColIndex) ;
    pListeWindow->SetItem(ItemPrevValue) ;
	}

  Invalidate() ;
}

void
NSGoalView::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  const int   BufLen = 255 ;
  static char buffer[BufLen] ;
  string      sDate = string("") ;
  buffer[0] = '\0' ;

  TListWindItem &dispInfoItem = *(TListWindItem *) &dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    // niveau de sévérité
    case 1  : // TODO
              {
                switch ((aGoals[index])->getLevel())
                {
                  case NSLdvGoalInfo::AVrouge : strcpy(buffer, "r") ; dispInfoItem.SetText(buffer) ; break ;
                  case NSLdvGoalInfo::AVjaune : strcpy(buffer, "j") ; dispInfoItem.SetText(buffer) ; break ;
                  case NSLdvGoalInfo::AVvert  : strcpy(buffer, "v") ; dispInfoItem.SetText(buffer) ; break ;
                  case NSLdvGoalInfo::Bleu    : strcpy(buffer, "b") ; dispInfoItem.SetText(buffer) ; break ;
                  case NSLdvGoalInfo::APvert  : strcpy(buffer, "V") ; dispInfoItem.SetText(buffer) ; break ;
                  case NSLdvGoalInfo::APjaune : strcpy(buffer, "J") ; dispInfoItem.SetText(buffer) ; break ;
                  case NSLdvGoalInfo::AProuge : strcpy(buffer, "R") ; dispInfoItem.SetText(buffer) ; break ;
                }
              }
              break ;

    // date début
    case 2  : if      ((aGoals[index])->getRythme() == NSLdvGoal::cyclic)
                strcpy(buffer, "cyclique") ;
              else if ((aGoals[index])->getRythme() == NSLdvGoal::permanent)
                strcpy(buffer, "permanent") ;
              else if ((aGoals[index])->getRythme() == NSLdvGoal::ponctuel)
                strcpy(buffer, "ponctuel") ;
              else
                strcpy(buffer, "") ;
              dispInfoItem.SetText(buffer) ;
              break ;

    // date début autorisé
    case 3  : sDate = (aGoals[index])->getDateAuto() ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    // date début conseillé
    case 4  : sDate = (aGoals[index])->getDateCons() ;              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    // date début idéal
    case 5  : sDate = (aGoals[index])->getDateIdeal() ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    // date début ideal max
    case 6  : sDate = (aGoals[index])->getDateIdealMax() ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    // date début conseillé max
    case 7  : sDate = (aGoals[index])->getDateConsMax() ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    // date début critique
    case 8  : sDate = (aGoals[index])->getDateCrit() ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    // date précédent
    case 9  : sDate = (aGoals[index])->getDatePrec() ;
              if (string("") != sDate)
                sDate = donne_date(sDate, sLang) ;
              dispInfoItem.SetText(sDate.c_str()) ;
              break ;

    // valeur précédente
    case 10 : if ((aGoals[index])->getValue() != string(""))
              {
                strcpy(buffer, ((aGoals[index])->getValue()).c_str()) ;
                dispInfoItem.SetText(buffer) ;
              }
              break ;
  }
}


void
NSGoalView::LVNColumnclick(TLwNotify& lwn)
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
NSGoalView::sortByColumn(int iColumn)
{
	iSortedColumn = iColumn ;

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

	AfficheListe() ;
}

boolNSGoalView::VnIsWindow(HWND hWnd){
  return (HWindow == hWnd) ;
}


// fonction permettant de prendre toute la taille de TWindow par la TListWindowvoid
NSGoalView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
  pListeWindow->MoveWindow(GetClientRect(), true) ;
}


// fonction EVSetFocusvoid
NSGoalView::EvSetFocus(HWND hWndLostFocus)
{
	NSMUEView::EvSetFocus(hWndLostFocus);

  focusFct() ;

  pListeWindow->SetFocus() ;
}

void
NSGoalView::focusFct()
{
	activateMUEViewMenu() ;

  TMyApp    *pMyApp = pContexte->getSuperviseur()->getApplication() ;
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
NSGoalView::SetupToolBar()
{
try
{
	TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_GOAL_STOP, CM_GOAL_STOP, TButtonGadget::Command)) ;

	pMyApp->getSecondaryControlBar()->LayoutSession() ;
}
catch (...)
{
	erreur("Exception NSGoalView::SetupToolBar.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
//
//  					Méthodes de NSDrugsPropertyWindow//
// -----------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(NSGoalsPropertyWindow, TListWindow)
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_COMMAND(CM_GOAL_STOP,     CmClore),
  EV_COMMAND(CM_GOAL_NEW,      CmCreate),
  EV_COMMAND(CM_GOAL_VALIDATE, CmValidate),
  EV_COMMAND(CM_GOAL_CHANGE,   CmModify),
  EV_COMMAND(CM_REFERENTIAL,   CmProtocol),
  EV_COMMAND(CM_GOAL_TRAIT,    CmTraitSelectedObj),
  EV_COMMAND(IDC_NEW_REF,      CmNewProtocol),
  EV_COMMAND(IDC_ADD_TO_REF,   CmAddToProtocol),
END_RESPONSE_TABLE ;

NSGoalsPropertyWindow::NSGoalsPropertyWindow(NSGoalView *parent, int id, int x, int y, int w, int h, TModule *module)
                      :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  pView  = parent ;
  iRes   = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  //Attr.ExStyle |= WS_EX_NOPARENTNOTIFY | LVS_EX_FULLROWSELECT ;

  Images = (OWL::TImageList*) 0 ;
}

NSGoalsPropertyWindow::~NSGoalsPropertyWindow()
{
	if (Images)
		delete Images ;
}

void
NSGoalsPropertyWindow::SetupWindow()
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

void
NSGoalsPropertyWindow::CmClore()
{
  pView->CmClore() ;
}

void
NSGoalsPropertyWindow::CmCreate()
{
  pView->CmNouveau() ;
}

void
NSGoalsPropertyWindow::CmModify()
{
  //pView->CmModify() ;
  pView->CmModifier() ;
}

void
NSGoalsPropertyWindow::CmProtocol()
{
  pView->CmFct3() ;
}

void
NSGoalsPropertyWindow::CmTraitSelectedObj()
{
  //pView->CmModify() ;
  pView->CmTraitSelectedObj() ;
}

void
NSGoalsPropertyWindow::CmValidate()
{
  pView->CmFct4() ;
}

void
NSGoalsPropertyWindow::CmNewProtocol()
{
  pView->CmFct5() ;
}

void
NSGoalsPropertyWindow::CmAddToProtocol()
{
  pView->CmFct6() ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSGoalsPropertyWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;
  //if (info.GetFlags() & LVHT_ONITEM)  //  pDlg->CmModifier();
}


void
NSGoalsPropertyWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
try
{
	TListWindow::EvLButtonDown(modkeys,point) ;

	//int count = GetItemCount() ;
	TLwHitTestInfo info(point) ;

	//int index = HitTest(info) ;

  NSSuper* pSuper = pView->pContexte->getSuperviseur() ;

	TPopupMenu *menu = new TPopupMenu() ;

	// We can't do that since we don't have a concern to attach the new goal
  //sTitle = pView->pContexte->getSuperviseur()->getText("goalsManagement", "goalCreate") ;
	//menu->AppendMenu(MF_STRING, CM_GOAL_NEW, sTitle.c_str());

  string sModif = pSuper->getText("goalsManagement", "goalModify") ;
  string sSelOb = pSuper->getText("goalsManagement", "goalTraitSelobj") ;
  string sClose = pSuper->getText("goalsManagement", "goalClose") ;
  string sGuide = pSuper->getText("goalsManagement", "newGoalFromGuideline") ;
  string sCreat = pSuper->getText("goalsManagement", "goalCreate") ;
  string sAddPr = pSuper->getText("goalsManagement", "addSelectedGoalssToAProtocol") ;
  string sNewPr = pSuper->getText("goalsManagement", "buildAProtocolFromSelectedGoals") ;
  string sValid = pSuper->getText("goalsManagement", "goalValidate") ;
  string sInval = pSuper->getText("goalsManagement", "goalInvalidate") ;

  // menu->AppendMenu(MF_STRING, CM_REFERENTIAL, sGuide.c_str()) ; // Goals must always be created from a line
  // menu->AppendMenu(MF_STRING, CM_GOAL_NEW,    sCreat.c_str()) ;

  menu->AppendMenu(MF_STRING, CM_GOAL_VALIDATE,   sValid.c_str()) ;
  menu->AppendMenu(MF_STRING, CM_GOAL_INVALIDATE, sInval.c_str()) ;
	menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
	menu->AppendMenu(MF_STRING, CM_GOAL_CHANGE, sModif.c_str()) ;

/*
  if (!pView->SelectedObjectifs())
  	menu->AppendMenu(MF_GRAYED, 0, sSelOb.c_str()) ;
  else
    menu->AppendMenu(MF_STRING, CM_GOAL_TRAIT, sSelOb.c_str()) ;
*/

  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
	menu->AppendMenu(MF_STRING, CM_GOAL_STOP, sClose.c_str()) ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, IDC_ADD_TO_REF, sAddPr.c_str()) ;
  menu->AppendMenu(MF_STRING, IDC_NEW_REF,    sNewPr.c_str()) ;

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
NSGoalsPropertyWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if      (key == VK_DELETE)
		pView->CmClore() ;
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
  	TListWindow::EvKeyDown(key, repeatCount, flags) ;
}

// -----------------------------------------------------------------------------//  Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSGoalsPropertyWindow::IndexItemSelect()
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
NSGoalsPropertyWindow::EvSetFocus(HWND hWndLostFocus)
{
  pView->EvSetFocus(hWndLostFocus) ;
}

