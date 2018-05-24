// --------------------------------------------------------------------------// NSDOCVIEW.CPP		Helper for Document/View operations
// --------------------------------------------------------------------------

#include <owl/uihelper.h>
#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <owl\splitter.h>
//#include <nautilus\nsSOAPview.h>
#include <fstream.h>
#include <stdlib.h>

#include "nautilus\nssuper.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nsdocview.h"

#include "nautilus\nscrvue.h"  // Document / Vues CN (Compte-Rendu Nautilus)
#include "nautilus\nsttx.h"    // Document / Vues TTX
#include "nautilus\nscsdoc.h"  // Document / Vues CS Consultation
#include "nautilus\nscsvue.h"  // Document / Vues CS Consultation
#include "nautilus\nscqvue.h"  // Document / Vues CQ Formulaires
#include "nautilus\nsldvvue.h"
#include "nautilus\nsldvtpl.h" // Template LdV
#include "nautilus\nshistor.h"
#include "nautilus\nsbrowse.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nscaptview.h"
#include "nautilus\nsdrugview.h"
#include "nautilus\nsdrughistoview.h"
#include "nautilus\nsepisodview.h"
#include "nautilus\nsgoalview.h"
#include "nautilus\nsFollowUpView.h"
#include "nautilus\nsprocessview.h"
#include "nautilus\nssoapview.h"
#include "nautilus\nsttx.h"
#include "nautilus\nsacroread.h"
#include "nautilus\nsvisual.h"
#include "nautilus\nsrcview.h"
#include "nautilus\nsTeamDocView.h"
#include "nautilus\nsVenueDocView.h"
#include "nautilus\nscqdoc.h"
#include "nautilus\nsInterface.h"
#include "nautilus\nsCoopChartDV.h"
#include "nautilus\decisionTreeDV.h"
#include "export\html2word.h"

REGISTRATION_FORMAT_BUFFER(100)

typedef TDocTemplateT<NSCRDocument, NSCRReadOnlyView> NSCRReadOnlyTemplate ;

BEGIN_REGISTRATION(CRtplreg)
 REGDATA(description,"CN Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "CN")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSCSDocument,NSCsVue> NSCSTemplate ;

BEGIN_REGISTRATION(CStplreg)
 REGDATA(description,"CS Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "CS")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

#define __TEST_ARCHETYPE
#ifdef __TEST_ARCHETYPE
typedef TDocTemplateT<NSCQDocument, NSCQWindowsView> NSCQTemplate ;
#else
typedef TDocTemplateT<NSCQDocument, NSCQVue> NSCQTemplate ;
#endif


BEGIN_REGISTRATION(CQtplreg)
 REGDATA(description,"CQ Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "CQ")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSLdvDocument, NSLdvView> NSLdvTemplate ;

BEGIN_REGISTRATION(LDVtplreg)
 REGDATA(description,"LDV Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "LDV")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSHISTODocument, NsHistorique> NsHsitorTemplate ;

BEGIN_REGISTRATION(HISTOtplreg)
 REGDATA(description,"HS Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "HS")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSHtmlModelDocument, NSHtmlModelView> HtmlViewTmpl ;

BEGIN_REGISTRATION(HTMLtplreg)
 REGDATA(description,"Html Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "Html")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION


typedef TDocTemplateT<NSRefDocument, NSComposView> ComposViewTmpl ;

BEGIN_REGISTRATION(COMPOtplreg)
 REGDATA(description,"Compos Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "Compo")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSCaptDocument, NSCaptView> CaptViewTmpl ;
typedef TDocTemplateT<NSCaptDocument, NSMultiCaptView> MultiCaptViewTmpl ;

BEGIN_REGISTRATION(multiCAPTtplreg)
 REGDATA(description,"Multi Capt Management")
 REGDATA(filter,     "")
 REGDATA(defaultext, "MultiCAPT")
 REGDATA(directory,  0)
 REGDOCFLAGS(0)
END_REGISTRATION

typedef TDocTemplateT<NSEpisodDocument,NSEpisodView> EpisodDlgViewTmpl ;

BEGIN_REGISTRATION(listCAPTtplreg)
 REGDATA(description,"Captured list")
 REGDATA(filter,     "")
 REGDATA(defaultext, "listCAPT")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSEpisodDocument,NSEpisodScreenView> EpisodScreenViewTmpl ;

BEGIN_REGISTRATION(screenCAPTtplreg)
 REGDATA(description,"Captured screen")
 REGDATA(filter,     "")
 REGDATA(defaultext, "screenCAPT")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSLdvDocument, NSDrugView> DrugViewTmpl ;

BEGIN_REGISTRATION(drugLDVtplreg)
 REGDATA(description,"Drug Management")
 REGDATA(filter,     "")
 REGDATA(defaultext, "Drug")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSLdvDocument, NSDrugHistoView> DrugHistoViewTmpl ;

BEGIN_REGISTRATION(drugHistoLDVtplreg)
 REGDATA(description,"Drug History")
 REGDATA(filter,     "")
 REGDATA(defaultext, "Drug history")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSLdvDocument, NSGoalView> GoalViewTmpl ;

BEGIN_REGISTRATION(goalLDVtplreg)
 REGDATA(description,"Goal Management")
 REGDATA(filter,     "")
 REGDATA(defaultext, "Goal")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSLdvDocument, NSFollowUpView> FollowUpViewTmpl ;

BEGIN_REGISTRATION(followUpLDVtplreg)
 REGDATA(description,"FollowUp Management")
 REGDATA(filter,     "")
 REGDATA(defaultext, "FollowUp")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSLdvDocument, NSProcessView> ProcessViewTmpl ;

BEGIN_REGISTRATION(processLDVtplreg)
 REGDATA(description,"Process Management")
 REGDATA(filter,     "")
 REGDATA(defaultext, "Process")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSLdvDocument, NSEpisodRCView> RCViewTmpl ;

BEGIN_REGISTRATION(RCtplreg)
 REGDATA(description,"RC view Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "RC")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSLdvDocument, NSRCHistoryView> RCHistoryViewTmpl ;

BEGIN_REGISTRATION(histoRCtplreg)
 REGDATA(description,"RC histo Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "RChisto")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSTtxDocument, NSAutoWordView> WordDlgViewTmpl ;

BEGIN_REGISTRATION(WORDtplreg)
 REGDATA(description,"Word Format")
 REGDATA(filter,     "*.doc")
 REGDATA(defaultext, "WORD")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSTtxDocument, NSTtxView> TtxDlgViewTmpl ;

BEGIN_REGISTRATION(RTFtplreg)
 REGDATA(description,"Rtf Format")
 REGDATA(filter,     "*.rtf;*.txt")
 REGDATA(defaultext, "RTX")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSTtxDocumentExp, NSAutoWordViewExp> EXWordViewTmpl ;

BEGIN_REGISTRATION(XPWORDtplreg)
 REGDATA(description,"HTML to Word Format")
 REGDATA(filter,     "*.htm")
 REGDATA(defaultext, "WORDXP")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSSimpleTxtDocument, NSSimpleTxtView> SimpleTxtViewTmpl ;

BEGIN_REGISTRATION(TXTtplreg)
 REGDATA(description,"Txt Format")
 REGDATA(filter,     "*.txt")
 REGDATA(defaultext, "TXT")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSAcrobatDocument, NSAcrobatView> PdfViewTmpl ;

BEGIN_REGISTRATION(PDFtplreg)
 REGDATA(description,"PDF Format")
 REGDATA(filter,     "*.pdf")
 REGDATA(defaultext, "PDF")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSRefDocument, NSVisualView> VisualViewTmpl ;

BEGIN_REGISTRATION(VISUALtplreg)
 REGDATA(description,"Visual Format")
 REGDATA(filter,     "")
 REGDATA(defaultext, "VISUAL")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSSOAPDocument, NSSOAPView> SOAPDlgViewTmpl ;

BEGIN_REGISTRATION(SOAPtplreg)
 REGDATA(description,"SOAP baskets")
 REGDATA(filter,     "")
 REGDATA(defaultext, "SOAPbaskets")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSSOAPDocument, NSSOAPTankView> SOAPTankDlgViewTmpl ;

BEGIN_REGISTRATION(SOAPTanktplreg)
 REGDATA(description,"SOAP tank")
 REGDATA(filter,     "")
 REGDATA(defaultext, "SOAPtank")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSHealthTeamDocument, NSHealthTeamRosaceView> NSHealthTeamRosaceTmpl ;

BEGIN_REGISTRATION(HealthTeamRosacetplreg)
 REGDATA(description,"HealthTeam rosace")
 REGDATA(filter,     "")
 REGDATA(defaultext, "HTrosace")
 REGDATA(directory,  0)
 REGDOCFLAGS(0)
END_REGISTRATION

typedef TDocTemplateT<NSHealthTeamDocument, NSHealthTeamListView> NSHealthTeamListTmpl ;

BEGIN_REGISTRATION(HealthTeamListtplreg)
 REGDATA(description,"HealthTeam list")
 REGDATA(filter,     "")
 REGDATA(defaultext, "HTlist")
 REGDATA(directory,  0)
 REGDOCFLAGS(0)
END_REGISTRATION

typedef TDocTemplateT<NSVenuesDocument, NSVenuesView> NSVenuesListTmpl ;

BEGIN_REGISTRATION(VenuesListtplreg)
 REGDATA(description,"Venues list")
 REGDATA(filter,     "")
 REGDATA(defaultext, "VenueList")
 REGDATA(directory,  0)
 REGDOCFLAGS(0)
END_REGISTRATION

typedef TDocTemplateT<NSInterfaceDocument, NSInterfaceView> NSInterfaceTmpl ;

BEGIN_REGISTRATION(Interfacetplreg)
 REGDATA(description,"Devices interface")
 REGDATA(filter,     "")
 REGDATA(defaultext, "Interface")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSCoopChartDocument, NSCoopChartView> NSCoopChartTmpl ;

BEGIN_REGISTRATION(CoopCharttplreg)
 REGDATA(description,"Coop Chart")
 REGDATA(filter,     "")
 REGDATA(defaultext, "CoopChart")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

typedef TDocTemplateT<NSDecisionTreeDocument, NSDecisionTreeView> NSDecisionTreeTmpl ;

BEGIN_REGISTRATION(DecisionTreetplreg)
 REGDATA(description,"Decision tree")
 REGDATA(filter,     "")
 REGDATA(defaultext, "DecisionTree")
 REGDATA(directory,  0)
 REGDOCFLAGS(dtAutoDelete)
END_REGISTRATION

// --------------------------------------------------------------------------
//
// Class NSDocViewManager
//
// --------------------------------------------------------------------------

// Constructeur
NSDocViewManager::NSDocViewManager(NSContexte* pCtx)
          			 :NSDocViewRootManager(pCtx)
{
try
{
	pDocManager = 0 ;

	if (!pContexte)
		return ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;
  if ((!pSuper) || (!(pSuper->getApplication())))
  	return ;

	pDocManager = pSuper->getApplication()->GetDocManager() ;
}
catch (...)
{
	erreur("Exception NSDocViewManager ctor.", standardError, 0) ;
}
}

// DestructeurNSDocViewManager::~NSDocViewManager()
{
}

/**
* Creates a view depending on document type
* If pView is not null, it means we have just to initView
*/
TView*
NSDocViewManager::createView(TDocument* pDoc, string sDescription, TView* pView)
{
try
{
	if ((TDocument*) NULL == pDoc)
  	return (TView*) 0 ;

	// DON'T DO THAT OR SOMETHING LIKE THAT (except if you want to lose 3 days)
  //
  // TModule* module = pContexte->getSuperviseur()->pExeModule ;
  // pTemplate = new NSCRReadOnlyTemplate(CRtplreg, module, tpl1) ;
  //
  // In the template constructor, module is reminded as a TModule**, and
  // it would point on your local variable (and not remain good for a long time)
  // You have to do :
  // pTemplate = new NSCRReadOnlyTemplate(CRtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
	//

	// CR doc ?
  //
	NSCRDocument* pCRDoc = TYPESAFE_DOWNCAST(pDoc, NSCRDocument) ;
	if (pCRDoc)
	{
		if (sDescription == "CN Format")
    {
      NSCRReadOnlyTemplate* pTemplate = 0 ;

      TDocTemplate* tpl = findTemplateByDesc("CN Format") ;
      if (tpl)
        pTemplate = TYPESAFE_DOWNCAST(tpl, NSCRReadOnlyTemplate) ;

      if (!pTemplate)
      {
        TDocTemplate* tpl1 = getFirstTemplate() ;
        pTemplate = new NSCRReadOnlyTemplate(CRtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
        pCRDoc->SetTemplate(pTemplate) ;
        attachTemplate(*pTemplate) ;
      }
      else
      	pCRDoc->SetTemplate(pTemplate) ;
      if (!pView)
        return pDocManager->CreateView(*pCRDoc, pTemplate) ;
      else
        return pCRDoc->InitView(pView) ;
  	}
	}

  // CS doc ?
  //
  NSCSDocument* pCSDoc = TYPESAFE_DOWNCAST(pDoc, NSCSDocument) ;
  if (pCSDoc)
	{
  	if (sDescription == "CS Format")
    {
      NSCSTemplate* pTemplate = 0 ;

      TDocTemplate* tpl = findTemplateByDesc("CS Format") ;
      if (tpl)
        pTemplate = TYPESAFE_DOWNCAST(tpl, NSCSTemplate) ;

      if (!pTemplate)
      {
        TDocTemplate* tpl1 = getFirstTemplate() ;
        pTemplate = new NSCSTemplate(CStplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
        pCSDoc->SetTemplate(pTemplate) ;
        attachTemplate(*pTemplate) ;
      }
      else
      	pCSDoc->SetTemplate(pTemplate) ;
      if (!pView)
        return pDocManager->CreateView(*pCSDoc, pTemplate) ;
      else
        return pCSDoc->InitView(pView) ;
		}
	}

  // CQ doc ?
  //
  NSCQDocument* pCQDoc = TYPESAFE_DOWNCAST(pDoc, NSCQDocument) ;
  if (pCQDoc)
	{
		if (sDescription == "CQ Format")
    {
      NSCQTemplate* pTemplate = 0 ;

      TDocTemplate* tpl = findTemplateByDesc("CQ Format") ;
      if (tpl)
        pTemplate = TYPESAFE_DOWNCAST(tpl, NSCQTemplate) ;

      if (!pTemplate)
      {
        TDocTemplate* tpl1 = getFirstTemplate() ;
        pTemplate = new NSCQTemplate(CQtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
        pCQDoc->SetTemplate(pTemplate) ;
        attachTemplate(*pTemplate) ;
      }
      else
      	pCQDoc->SetTemplate(pTemplate) ;
      if (!pView)
        return pDocManager->CreateView(*pCQDoc, pTemplate) ;
      else
        return pCQDoc->InitView(pView) ;
  	}
	}

  // LDV doc ?
  //
  NSLdvDocument* pDocLdv = TYPESAFE_DOWNCAST(pDoc, NSLdvDocument) ;
  if (pDocLdv)
	{
  	if (sDescription == "Drug Management")
    {
			DrugViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Drug Management") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, DrugViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new DrugViewTmpl(drugLDVtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocLdv->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocLdv->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocLdv, pTemplate) ;
    	else
    		return pDocLdv->InitView(pView) ;
    }
    else if (sDescription == "Drug History")
    {
			DrugHistoViewTmpl* pTemplate = (DrugHistoViewTmpl*) 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Drug History") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, DrugHistoViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new DrugHistoViewTmpl(drugHistoLDVtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocLdv->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocLdv->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocLdv, pTemplate) ;
    	else
    		return pDocLdv->InitView(pView) ;
    }
  	else if (sDescription == "Goal Management")
    {
			GoalViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Goal Management") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, GoalViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new GoalViewTmpl(goalLDVtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocLdv->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocLdv->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocLdv, pTemplate) ;
    	else
    		return pDocLdv->InitView(pView) ;
    }
    else if (sDescription == "FollowUp Management")
    {
			FollowUpViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("FollowUp Management") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, FollowUpViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new FollowUpViewTmpl(followUpLDVtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocLdv->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocLdv->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocLdv, pTemplate) ;
    	else
    		return pDocLdv->InitView(pView) ;
    }
  	else if (sDescription == "Process Management")
    {
			ProcessViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Process Management") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, ProcessViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new ProcessViewTmpl(processLDVtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocLdv->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocLdv->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocLdv, pTemplate) ;
    	else
    		return pDocLdv->InitView(pView) ;
    }
  	else if (sDescription == "RC view Format")
    {
			RCViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("RC view Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, RCViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new RCViewTmpl(RCtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocLdv->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocLdv->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocLdv, pTemplate) ;
    	else
    		return pDocLdv->InitView(pView) ;
    }
  	else if (sDescription == "RC histo Format")
    {
			RCHistoryViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("RC histo Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, RCHistoryViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new RCHistoryViewTmpl(histoRCtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocLdv->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocLdv->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocLdv, pTemplate) ;
    	else
    		return pDocLdv->InitView(pView) ;
    }
    else
    {
			NSLdvTemplate* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("LDV Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, NSLdvTemplate) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
        pTemplate = new NSLdvTemplate(LDVtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocLdv->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocLdv->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocLdv, pTemplate) ;
    	else
    		return pDocLdv->InitView(pView) ;
    }
	}

  // HISTO doc ?
  //
  NSHISTODocument* pDocHis = TYPESAFE_DOWNCAST(pDoc, NSHISTODocument) ;
  if (pDocHis)
	{
  	if (sDescription == "HS Format")
    {
      NsHsitorTemplate* pTemplate = 0 ;

      TDocTemplate* tpl = findTemplateByDesc("HS Format") ;
      if (tpl)
        pTemplate = TYPESAFE_DOWNCAST(tpl, NsHsitorTemplate) ;

      if (!pTemplate)
      {
        TDocTemplate* tpl1 = getFirstTemplate() ;
        pTemplate = new NsHsitorTemplate(HISTOtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
        pDocHis->SetTemplate(pTemplate) ;
        attachTemplate(*pTemplate) ;
      }
      else
      	pDocHis->SetTemplate(pTemplate) ;
      if (!pView)
        return pDocManager->CreateView(*pDocHis, pTemplate) ;
      else
        return pDocHis->InitView(pView) ;
		}
	}

  // TTX doc ?
  //
  NSTtxDocument* pNewDocTtx = TYPESAFE_DOWNCAST(pDoc, NSTtxDocument) ;
  if (pNewDocTtx)
	{
    if (sDescription == "Rtf Format")
    {
    	TtxDlgViewTmpl* pTemplate = 0 ;
			TDocTemplate* tpl = findTemplateByDesc("Rtf Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, TtxDlgViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new TtxDlgViewTmpl(RTFtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pNewDocTtx->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pNewDocTtx->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pNewDocTtx, pTemplate) ;
    	else
    		return pNewDocTtx->InitView(pView) ;
		}
    else if (sDescription == "Word Format")
    {
			WordDlgViewTmpl* pTemplate = 0 ;
			TDocTemplate* tpl = findTemplateByDesc("Word Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, WordDlgViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new WordDlgViewTmpl(WORDtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pNewDocTtx->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pNewDocTtx->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pNewDocTtx, pTemplate) ;
    	else
    		return pNewDocTtx->InitView(pView) ;
		}
	}

  // TTX export doc ?
  //
  NSTtxDocumentExp* pNewDocTtxExp = TYPESAFE_DOWNCAST(pDoc, NSTtxDocumentExp) ;
  if (pNewDocTtxExp)
	{
    if (sDescription == "HTML to Word Format")
    {
    	EXWordViewTmpl* pTemplate = 0 ;
			TDocTemplate* tpl = findTemplateByDesc("HTML to Word Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, EXWordViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new EXWordViewTmpl(XPWORDtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pNewDocTtxExp->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pNewDocTtxExp->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pNewDocTtxExp, pTemplate) ;
    	else
    		return pNewDocTtxExp->InitView(pView) ;
		}
	}

  // Capture doc ?
  //
  NSCaptDocument* pCaptureDoc = TYPESAFE_DOWNCAST(pDoc, NSCaptDocument) ;
  if (pCaptureDoc)
	{
    if (sDescription == "Multi Capt Management")
    {
    	MultiCaptViewTmpl* pTemplate = 0 ;
			TDocTemplate* tpl = findTemplateByDesc("Multi Capt Management") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, MultiCaptViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new MultiCaptViewTmpl(multiCAPTtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pCaptureDoc->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pCaptureDoc->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pCaptureDoc, pTemplate) ;
    	else
    		return pCaptureDoc->InitView(pView) ;
		}
    else if (sDescription == "Word Format")
    {

		}
	}

  // SOAP doc ?
  //
  NSSOAPDocument* pDocSOAP = TYPESAFE_DOWNCAST(pDoc, NSSOAPDocument) ;
  if (pDocSOAP)
	{
    if (sDescription == "SOAP baskets")
    {
    	SOAPDlgViewTmpl* pTemplate = 0 ;
			TDocTemplate* tpl = findTemplateByDesc("SOAP baskets") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, SOAPDlgViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new SOAPDlgViewTmpl(SOAPtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocSOAP->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocSOAP->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocSOAP, pTemplate) ;
    	else
    		return pDocSOAP->InitView(pView) ;
		}
    else if (sDescription == "SOAP tank")
    {
    	SOAPTankDlgViewTmpl* pTemplate = 0 ;
			TDocTemplate* tpl = findTemplateByDesc("SOAP tank") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, SOAPTankDlgViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new SOAPTankDlgViewTmpl(SOAPTanktplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocSOAP->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocSOAP->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocSOAP, pTemplate) ;
    	else
    		return pDocSOAP->InitView(pView) ;
		}
	}

  // Capture model doc ?
  //
  NSEpisodDocument* pDocEpisodus = TYPESAFE_DOWNCAST(pDoc, NSEpisodDocument) ;
  if (pDocEpisodus)
	{
    if (sDescription == "Captured list")
    {
    	EpisodDlgViewTmpl* pTemplate = 0 ;
			TDocTemplate* tpl = findTemplateByDesc("Captured list") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, EpisodDlgViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new EpisodDlgViewTmpl(listCAPTtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocEpisodus->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocEpisodus->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocEpisodus, pTemplate) ;
    	else
    		return pDocEpisodus->InitView(pView) ;
		}
    else if (sDescription == "Captured screen")
    {
    	EpisodScreenViewTmpl* pTemplate = 0 ;
			TDocTemplate* tpl = findTemplateByDesc("Captured screen") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, EpisodScreenViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new EpisodScreenViewTmpl(screenCAPTtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocEpisodus->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocEpisodus->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocEpisodus, pTemplate) ;
    	else
    		return pDocEpisodus->InitView(pView) ;
		}
	}


  // TXT doc ?
  //
  NSSimpleTxtDocument* pDocTxt = TYPESAFE_DOWNCAST(pDoc, NSSimpleTxtDocument) ;
  if (pDocTxt)
	{
  	if (sDescription == "Txt Format")
    {
      SimpleTxtViewTmpl* pTemplate = 0 ;

      TDocTemplate* tpl = findTemplateByDesc("Txt Format") ;
      if (tpl)
        pTemplate = TYPESAFE_DOWNCAST(tpl, SimpleTxtViewTmpl) ;

      if (!pTemplate)
      {
        TDocTemplate* tpl1 = getFirstTemplate() ;
        pTemplate = new SimpleTxtViewTmpl(TXTtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
        pDocTxt->SetTemplate(pTemplate) ;
        attachTemplate(*pTemplate) ;
      }
      else
      	pDocTxt->SetTemplate(pTemplate) ;
      if (!pView)
        return pDocManager->CreateView(*pDocTxt, pTemplate) ;
      else
        return pDocTxt->InitView(pView) ;
		}
	}

  // PDF doc ?
  //
  NSAcrobatDocument* pDocPdf = TYPESAFE_DOWNCAST(pDoc, NSAcrobatDocument) ;
  if (pDocPdf)
	{
  	if (sDescription == "Pdf Format")
    {
      PdfViewTmpl* pTemplate = 0 ;

      TDocTemplate* tpl = findTemplateByDesc("PDF Format") ;
      if (tpl)
        pTemplate = TYPESAFE_DOWNCAST(tpl, PdfViewTmpl) ;

      if (!pTemplate)
      {
        TDocTemplate* tpl1 = getFirstTemplate() ;
        pTemplate = new PdfViewTmpl(PDFtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
        pDocPdf->SetTemplate(pTemplate) ;
        attachTemplate(*pTemplate) ;
      }
      else
      	pDocPdf->SetTemplate(pTemplate) ;
      if (!pView)
        return pDocManager->CreateView(*pDocPdf, pTemplate) ;
      else
        return pDocPdf->InitView(pView) ;
		}
	}

  NSHtmlModelDocument* pDocHtml = TYPESAFE_DOWNCAST(pDoc, NSHtmlModelDocument) ;
  if (pDocHtml)
	{
		if (sDescription == "Html Format")
    {
			HtmlViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Html Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, HtmlViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new HtmlViewTmpl(HTMLtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocHtml->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocHtml->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocHtml, pTemplate) ;
    	else
    		return pDocHtml->InitView(pView) ;
    }
	}

  // Coop charts doc ? (keep before NSRefDocument)
  //
  NSCoopChartDocument* pCoopChartRef = TYPESAFE_DOWNCAST(pDoc, NSCoopChartDocument) ;
  if (pCoopChartRef)
	{
    if (sDescription == "Coop Chart")
    {
			NSCoopChartTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Coop Chart") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, NSCoopChartTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new NSCoopChartTmpl(CoopCharttplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pCoopChartRef->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pCoopChartRef->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pCoopChartRef, pTemplate) ;
    	else
    		return pCoopChartRef->InitView(pView) ;
    }
	}

  // Decision tree doc ? (keep before NSRefDocument)
  //
  NSDecisionTreeDocument* pDecisionTreeRef = TYPESAFE_DOWNCAST(pDoc, NSDecisionTreeDocument) ;
  if (pDecisionTreeRef)
	{
    if (sDescription == "Decision Tree")
    {
			NSDecisionTreeTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Decision Tree") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, NSDecisionTreeTmpl) ;

			if (NULL == pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new NSDecisionTreeTmpl(DecisionTreetplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDecisionTreeRef->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDecisionTreeRef->SetTemplate(pTemplate) ;

      if (NULL == pView)
				return pDocManager->CreateView(*pDecisionTreeRef, pTemplate) ;
    	else
    		return pDecisionTreeRef->InitView(pView) ;
    }
	}

  // VISUAL doc ?
  //
  NSRefDocument* pDocRef = TYPESAFE_DOWNCAST(pDoc, NSRefDocument) ;
  if (pDocRef)
	{
		if (sDescription == "Visual Format")
    {
			VisualViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Visual Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, VisualViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new VisualViewTmpl(VISUALtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocRef->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocRef->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocRef, pTemplate) ;
    	else
    		return pDocRef->InitView(pView) ;
    }
		else if (sDescription == "Compos Format")
    {
			ComposViewTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Compos Format") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, ComposViewTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new ComposViewTmpl(COMPOtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pDocRef->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pDocRef->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pDocRef, pTemplate) ;
    	else
    		return pDocRef->InitView(pView) ;
    }
	}

	// Health team doc ?
  //
  NSHealthTeamDocument* pTeamRef = TYPESAFE_DOWNCAST(pDoc, NSHealthTeamDocument) ;
  if (pTeamRef)
	{
		if (sDescription == "HealthTeam rosace")
    {
			NSHealthTeamRosaceTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("HealthTeam rosace") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, NSHealthTeamRosaceTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new NSHealthTeamRosaceTmpl(HealthTeamRosacetplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pTeamRef->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pTeamRef->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pTeamRef, pTemplate) ;
    	else
    		return pTeamRef->InitView(pView) ;
    }
    else if (sDescription == "HealthTeam list")
    {
			NSHealthTeamListTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("HealthTeam list") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, NSHealthTeamListTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new NSHealthTeamListTmpl(HealthTeamListtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pTeamRef->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pTeamRef->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pTeamRef, pTemplate) ;
    	else
    		return pTeamRef->InitView(pView) ;
    }
  }

  // Venue doc ?
  //
  NSVenuesDocument* pVenueRef = TYPESAFE_DOWNCAST(pDoc, NSVenuesDocument) ;
  if (pVenueRef)
	{
    if (sDescription == "Venues list")
    {
			NSVenuesListTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Venues list") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, NSVenuesListTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new NSVenuesListTmpl(VenuesListtplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pVenueRef->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pVenueRef->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pVenueRef, pTemplate) ;
    	else
    		return pVenueRef->InitView(pView) ;
    }
	}

  // Interface doc ?
  //
  NSInterfaceDocument* pInterfaceRef = TYPESAFE_DOWNCAST(pDoc, NSInterfaceDocument) ;
  if (pInterfaceRef)
	{
    if (sDescription == "Interface")
    {
			NSInterfaceTmpl* pTemplate = 0 ;

			TDocTemplate* tpl = findTemplateByDesc("Devices interface") ;
    	if (tpl)
				pTemplate = TYPESAFE_DOWNCAST(tpl, NSInterfaceTmpl) ;

			if (!pTemplate)
			{
    		TDocTemplate* tpl1 = getFirstTemplate() ;
    		pTemplate = new NSInterfaceTmpl(Interfacetplreg, pContexte->getSuperviseur()->pExeModule, tpl1) ;
				pInterfaceRef->SetTemplate(pTemplate) ;
				attachTemplate(*pTemplate) ;
			}
      else
      	pInterfaceRef->SetTemplate(pTemplate) ;
      if (!pView)
				return pDocManager->CreateView(*pInterfaceRef, pTemplate) ;
    	else
    		return pInterfaceRef->InitView(pView) ;
    }
	}

  return (TView*) 0 ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception NSDocViewManager::createView (") + sDescription + string(") : ") ;
  sErr += e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
  return (TView*) 0 ;
}
catch (...)
{
	string sErr = string("Exception NSDocViewManager::createView (") + sDescription + string(")") ;
	erreur(sErr.c_str(), standardError, 0) ;
  return (TView*) 0 ;
}
}

