// -----------------------------------------------------------------------------
//                  Ligne de vie - Vue du modèle Document/Vue
// -----------------------------------------------------------------------------

// #include <vcl.h>
// #include <owl\owlpch.h>
// #include <owl\dc.h>
// #include <classlib\date.h>

#include "nssavoir\nsBdmDriver.h"
// #include "nautilus\nssuper.h"
// #include "dcodeur\nsdkd.h"

// #include "nsldv\nsldvuti.h"
// #include "nautilus\nautilus.rh"
// #include "nautilus\nsadmiwd.h"
// #include "nautilus\nscqvue.h"
#include "nautilus\decisionTreeDV.h"
// #include "nsbb\nstlibre.h"

// #include "nsepisod\nsepidiv.h"

// #include "nautilus\nshistdo.h"
// #include "nautilus\nstrihis.h"
#include "nautilus\nsepicap.h"

#include <windows.h>
// #include <owl\scrollba.h>

// #include "nssavoir\nsgraphe.h"
// #include "nssavoir\nsconver.h"
// #include "nsbb\nsattvaltools.h"
// #include "nsbb\nsfltbtnga.h"
// #include "dcodeur\nsgen.h"
// #include "nautilus\nsldvvue.rh"
#include "nautilus\nsldvvue.h"
// #include "nautilus\nsldvvuetech.h"
// #include "nsldv\nsldvdrug.h"
// #include "nautilus\nsFollowUpView.h"
#include "nsepisod\nsToDo.h"
// #include "ns_ob1\BB1KS.h"
#include "ns_ob1\InterfaceForKs.h"

// #include "nautilus\nsldvgoal.h"
// #include "nautilus\nsdocview.h"
// #include "nautilus\nsCoopChartDV.h"

#include "nsldv\nsldvstructs.h"
#include "nsepisod\objectif_viewer.h"

// long NSLdvTimeToon::lLocalObjectCount = 0 ;
// long NSPbModifView::lObjectCount = 0 ;
long NSLigneView::lObjectCount = 0 ;
// long NSConcernView::lObjectCount = 0 ;
// long NSPreoccupView::lObjectCount = 0 ;
// long NSEventView::lObjectCount = 0 ;
// long NSDrugLineView::lObjectCount = 0 ;
// long NSGoalLineView::lObjectCount = 0 ;
// long NSBaseLineView::lObjectCount = 0 ;
// long NSSsObjView::lObjectCount = 0 ;
// long NSLdvCurvePoint::lObjectCount = 0 ;
// long NSLdvObjetView::lObjectCount = 0 ;
// long NSQualityOfLifeChartView::lObjectCount = 0 ;
// long NSLdvGoalCycleView::lObjectCount = 0 ;
// long NSLdvTankView::lObjectCount = 0 ;
// long NSTimeZoomLevel::lObjectCount = 0 ;
long NSLdvView::lObjectCount = 0 ;
// long NSLdvViewArea::lObjectCount = 0 ;
// long NSLdvViewAreaWindow::lObjectCount = 0 ;

// long NSLdvToon::lCoreObjectCount = 0 ;

// -----------------------------------------------------------------------------
// -------------------------- METHODES DE NSLdvView ----------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  Définition de la table de réponse de la vue NSLdvView
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLdvView, NSLdvViewBase)
END_RESPONSE_TABLE;

/**
 *  Constructor
 */
NSLdvView::NSLdvView(NSLdvDocument& doc, TWindow* parent)
	        :NSLdvViewBase(doc, parent)
{
  lObjectCount++ ;

  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->setMenu(string("menubar"), &hAccelerator) ;

	bSetupToolBar   = true ;

	// Construction de la barre de statut
	pSB = new OWL::TStatusBar(this, TGadget::Recessed) ;
}

/**
 *  Copy constructor
 */
NSLdvView::NSLdvView(const NSLdvView& src)
          :NSLdvViewBase(src)
{
  lObjectCount++ ;
}

/**
 *  Destructor
 */
NSLdvView::~NSLdvView()
{
  lObjectCount-- ;
}

void
NSLdvView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
  NSLdvViewBase::EvSetFocus(hWndLostFocus) ;

  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;

try
{
  pMyApp->setMenu(string("menubar"), &hAccelerator) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvView EvSetFocus (setting menu).") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
	pContexte->setAideIndex(pContexte->getSuperviseur()->getText("help", "helpLDVindex"));
	pContexte->setAideCorps(pContexte->getSuperviseur()->getText("help", "helpLDV")) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvView EvSetFocus (setting help).") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvView::CmNewPreoccup()
{
try
{
	string sNewNode = string("") ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

	NSNewConcernDlg* pNPDialog = new NSNewConcernDlg(this, pContexte, &concernInfo) ;
	int iExecReturn = pNPDialog->Execute() ;

	if (IDOK == iExecReturn)
    getDoc()->newLine(ldvframeHealth, sNewNode, &concernInfo) ;
	//	pContexte->getPatient()->pDocLdv->getConcerns()->reinit() ;
}
catch (...)
{
	erreur("Exception CmNewPreoccup.", standardError, 0) ;
}
}

void
NSLdvView::CmNewRisk()
{
try
{
	string sNewNode = string("") ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

	NSNewRiskDlg* pNPDialog = new NSNewRiskDlg(this, pContexte, &concernInfo) ;
	int iExecReturn = pNPDialog->Execute() ;

  if (IDOK == iExecReturn)
    getDoc()->newLine(ldvframeRisk, sNewNode, &concernInfo) ;
}
catch (...)
{
	erreur("Exception CmNewRisk.", standardError, 0) ;
}
}

void
NSLdvView::CmNewSocialConcern()
{
try
{
	string sNewNode = string("") ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

	NSNewSocialDlg* pNPDialog = new NSNewSocialDlg(this, pContexte, &concernInfo) ;
	int iExecReturn = pNPDialog->Execute() ;

  if (IDOK == iExecReturn)
    getDoc()->newLine(ldvframeSocial, sNewNode, &concernInfo) ;
}
catch (...)
{
	erreur("Exception CmNewSocialConcern.", standardError, 0) ;
}
}

void
NSLdvView::CmProject(NSConcern* pConcern, LDVFRAME iFrame, string sKsName, string sKsArchetype)
{
try
{
  if (((NSConcern*) NULL == pConcern) || (string("") == sKsName))
		return ;

	if (false == pConcern->isActiveConcern())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "inactiveProcedure") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

  if (string("") != sKsArchetype)
  {
    NSPredi* pPredi = pContexte->getPredi() ;
    if (pPredi)
    {
      TWindow* pOpenedWindow = pPredi->isDecisionTreeOpen(sKsArchetype) ;
      if (pOpenedWindow)
      {
        NSDecisionTreeView *pDTView = dynamic_cast<NSDecisionTreeView*>(pOpenedWindow) ;
        if (pDTView)
          pDTView->getCreatedMdiChild()->BringWindowToTop() ;
        return ;
      }
    }
  }

	NSToDoTask* ptask = new NSToDoTask() ;
  // ptask->sendBB1BBInterface(new BB1BBInterfaceForKs(0, sKsName, sKsArchetype), pContexte->getSuperviseur(), true, true) ;
  ptask->sendBB1BBDecisionTree(new BB1BBInterfaceForKs(0, sKsName, sKsArchetype), pContexte->getSuperviseur(), true, true) ;
}
catch (...)
{
	erreur("Exception NSLdvView CmProject.", standardError, 0) ;
}
}

void
NSLdvView::CmAddObjectifs(NSConcern* pConcern, LDVFRAME iFrame)
{
try
{
  if ((NSConcern*) NULL == pConcern)
		return ;

	if (false == pConcern->isActiveConcern())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "inactiveProcedure") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

	NSLdvGoal Objectif(pContexte, _pLdVDoc, iFrame) ;

	ObjectifViewerDlg* objDlg = new ObjectifViewerDlg((TWindow*)this, _pLdVDoc->pContexte, true, &Objectif) ;
	objDlg->Execute() ;
	delete objDlg ;

	NSPatPathoArray PathArray(_pLdVDoc->pContexte->getSuperviseur()) ;
	Objectif.setGoalPatho(&PathArray) ;

  // CreeObjectif(NSPatPathoArray *pPatPathoCree, string sDocument, string sRefId, NSPatPathoArray *pPatPathoMotif, string goal = "") ;
	pContexte->getPatient()->CreeObjectif(&PathArray, string(""), string(""), string(""), (NSPatPathoArray*) 0, iFrame, true, pConcern->_sReference) ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase CmAddObjectifs.", standardError, 0) ;
}
}

void
NSLdvView::CmEvolPreoccup(LDVFRAME iFrame)
{
try
{
	string sNewNode = string("") ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

  if (ldvframeHealth == iFrame)
  {
	  NSNewConcernDlg* pNPDialog = new NSNewConcernDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    getDoc()->newLine(ldvframeHealth, sNewNode, &concernInfo) ;
  }
  else if (ldvframeRisk == iFrame)
  {
    NSNewRiskDlg* pNPDialog = new NSNewRiskDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    getDoc()->newLine(ldvframeRisk, sNewNode, &concernInfo) ;
  }
  else if (ldvframeSocial == iFrame)
  {
    NSNewSocialDlg* pNPDialog = new NSNewSocialDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    getDoc()->newLine(ldvframeSocial, sNewNode, &concernInfo) ;
  }
  else
    return ;

  ArrayConcern* pConcerns = getDoc()->getConcerns(iFrame) ;
  if (pConcerns)
    pConcerns->reinit() ;
}
catch (...)
{
	erreur("Exception CmNewPreoccup.", standardError, 0) ;
}
}

void
NSLdvView::CmEvolPreoType(NSConcern* pConcern, LDVFRAME iFrame)
{
try
{
  if ((NSConcern*) NULL == pConcern)
		return ;

  string sFatherNode = pConcern->getNoeud() ;

	string sNewNode = string("") ;
  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

  if (ldvframeHealth == iFrame)
  {
	  NSNewConcernDlg* pNPDialog = new NSNewConcernDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    getDoc()->newLine(ldvframeHealth, sNewNode, &concernInfo) ;
  }
  else if (ldvframeRisk == iFrame)
  {
    NSNewRiskDlg* pNPDialog = new NSNewRiskDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    getDoc()->newLine(ldvframeRisk, sNewNode, &concernInfo) ;
  }
  else if (ldvframeSocial == iFrame)
  {
    NSNewSocialDlg* pNPDialog = new NSNewSocialDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    getDoc()->newLine(ldvframeSocial, sNewNode, &concernInfo) ;
  }
  else
    return ;

  ArrayConcern* pConcerns = _pLdVDoc->getConcerns(iFrame) ;
	pConcerns->reinit() ;

	NSConcern* pFatherConcern = pConcerns->getConcern(sFatherNode) ;
  NSConcern* pSonConcern    = pConcerns->getConcern(sNewNode) ;

  getDoc()->creeConcernSuite(iFrame, pFatherConcern, pSonConcern) ;
}
catch (...)
{
	erreur("Exception NSLdvView::CmEvolPreoType.", standardError, 0) ;
}
}

