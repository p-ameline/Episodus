//**************************************************************************//  NOM du FICHIER  : nshistor.CPP : chemises et documents sous forme de TreeView
//  								  Kaddachi Hafedh 20/01/1998
//**************************************************************************

#define __NSHISTOR_CPP

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include "partage\nsdivfct.h"
#include "nautilus\nshistor.h"
#include "nautilus\nssuper.h"
#include "nautilus\nautilus.rh"
#include "nautilus\nsmodhtm.h"
#include "nautilus\nsdocaga.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nsfltbtnga.h"

#define NODES_COL_BASE 1
#define NODES_LIN_BASE 1

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//									 Classe NsHistorique
//----------------------------------------------------------------------
//----------------------------------------------------------------------

// DEFINE_RESPONSE_TABLE2(NsHistorique, TView, NSTreeHistorique)
DEFINE_RESPONSE_TABLE1(NsHistorique, NSTreeHistorique)
	//	EV_COMMAND (WM_RAFRAICHIR, Rafraichir),
  EV_WM_CLOSE,
  EV_WM_DESTROY,
  EV_WM_SETFOCUS,
  EV_VN_ISWINDOW,
  EV_WM_SYSCOMMAND,
  EV_COMMAND(CM_FILECLOSE, EvClose),
  EV_COMMAND_ENABLE(CM_FILECLOSE, CeFileClose),

  EV_COMMAND(CM_IMPRIME, CmPublier),

 	EV_COMMAND(CM_0P,  Cm0Plus),
  EV_COMMAND(CM_1P,  Cm1Plus),
  EV_COMMAND(CM_2P,  Cm2Plus),
  EV_COMMAND(CM_3P,  Cm3Plus),  EV_COMMAND(CM_4P,  Cm4Plus),

  EV_COMMAND_ENABLE(CM_0P,  Ce0Plus),
  EV_COMMAND_ENABLE(CM_1P,  Ce1Plus),
  EV_COMMAND_ENABLE(CM_2P,  Ce2Plus),
  EV_COMMAND_ENABLE(CM_3P,  Ce3Plus),
  EV_COMMAND_ENABLE(CM_4P,  Ce4Plus),
END_RESPONSE_TABLE;

NsHistorique::NsHistorique(NSHISTODocument& doc, TWindow* parent)
             :TView(doc), pDoc(&doc),
              NSTreeHistorique(parent, doc.pContexte, GetNextViewId(), 0,0,0,0, &doc)
{
	NSSuper* pSuper = doc.pContexte->getSuperviseur() ;

  pMUEViewMenu = (OWL::TMenuDescr*) 0 ;

  TMyApp* pMyApp = pSuper->getApplication() ;
  pMyApp->setMenu(string("menubar_histo"), &hAccelerator) ;

	// initMUEViewMenu("menubar_histo") ;

	bSetupToolBar = true ;
}

TWindow*NsHistorique::GetWindow()
{
  NSTreeHistorique *pWin = (NSTreeHistorique*) this ;
  return (TWindow*) pWin ;
}

//----------------------------------------------------------------------//			Destructeur
//----------------------------------------------------------------------
NsHistorique::~NsHistorique()
{
  string sMsg = string("Entering NsHistorique destructor") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	if (pMUEViewMenu)
  	delete pMUEViewMenu ;

  if (0 != hAccelerator)
  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }
}

//---------------------------------------------------------------------------// SetupWindow
//---------------------------------------------------------------------------
void
NsHistorique::SetupWindow()
{
	NSTreeHistorique::SetupWindow() ;

  SetWindowPosHisto() ;

  Parent->SetCaption("Historique") ;
}

bool
NsHistorique::PreProcessMsg(MSG &msg)
{
  // Code from TWindow::PreProcessMsg(MSG& msg):
  // PRECONDITION(GetHandle()) ;
  // return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;

  PRECONDITION(GetHandle()) ;
  if (NULL == hAccelerator)
    return false ;

  bool bTranslated = ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) ;
  if (bTranslated)
    return true ;

  // See if we are in an mdi child, & try mdi accelerator translation if so
  //
  THandle child ;
  THandle client ;
  if ((child = GetParentH()) != 0 &&
      (client = ::GetParent(child)) != 0 &&
      child == (THandle)::SendMessage(client, WM_MDIGETACTIVE, 0, 0) &&
      ::TranslateMDISysAccel(client, &msg))
    return true ;

  return false ;
}

//-------------------------------------------------------------------------//demander à NSTreeHistorique d'afficher pNSDocumentHisto
//-------------------------------------------------------------------------
void
NsHistorique::Rafraichir(NSDocumentInfo* pNSDocumentInfo,
                         const NSPatPathoArray* pNSPatPathoArray,
                         NSNoyauDocument* pNouveauDocument)
{
	NSTreeHistorique* pNSTreeHistorique = this ;
	pNSTreeHistorique->AjouteDocument(pNSDocumentInfo, pNSPatPathoArray, pNouveauDocument) ;
}

//-----------------------------------------------------------------------//demander l'autorisation de NSTreeHistorique de l'ouverture du document
//-----------------------------------------------------------------------
void
NsHistorique::AutoriserOuverture(NSDocumentInfo* pDocument)
{
 	NSTreeHistorique* pNSTreeHistorique = this ;
	pNSTreeHistorique->AutoriserOuverture(pDocument) ;
}

//-----------------------------------------------------------------------//demander l'autorisation de NSTreeHistorique de l'édition du document
//-----------------------------------------------------------------------
void
NsHistorique::AutoriserEdition(NSDocumentInfo* pDocument)
{
 	NSTreeHistorique* pNSTreeHistorique = this ;
	pNSTreeHistorique->AutoriserEdition(pDocument) ;
}

//-------------------------------------------------------------------------
//demander à NSTreeHistorique d'enlever le document pNSDocumentInfo
//-------------------------------------------------------------------------
void
NsHistorique::EnleverDocument(NSDocumentInfo* pNSDocumentInfo)
{
	NSTreeHistorique* pNSTreeHistorique = this ;
	pNSTreeHistorique->EnleverDocument(pNSDocumentInfo) ;
}

//-------------------------------------------------------------------------
//demander à NSTreeHistorique d'ajouter le document pNSDocumentInfo
//-------------------------------------------------------------------------
void
NsHistorique::AjouterDocument(NSDocumentInfo* pNSDocumentInfo)
{
	NSTreeHistorique* pNSTreeHistorique = this ;
	pNSTreeHistorique->Ajouter(pNSDocumentInfo) ;
}

//-------------------------------------------------------------------------
//rafraîchir l'historique en tenant compte des changements (exemple patpatho)
//de pNSDocumentHisto
//-------------------------------------------------------------------------
void
NsHistorique::VisualiserPatho(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray* pNSPatPathoArray)
{
	NSTreeHistorique* pNSTreeHistorique = this ;
	pNSTreeHistorique->VisualiserPatho(pNSDocumentInfo, pNSPatPathoArray) ;
}

//-------------------------------------------------------------------------
//demander à NSTreeHistorique de changer la BITmap d'un document qui vient d'être
//fermé
//-------------------------------------------------------------------------
void
NsHistorique::FermetureDocument(NSDocumentInfo* pDocumentInfo)
{
	NSTreeHistorique* pNSTreeHistorique = this ;
	pNSTreeHistorique->FermetureDocument(pDocumentInfo) ;
}

//---------------------------------------------------------------------------
//fixer la position de la fiche historique
//---------------------------------------------------------------------------
void
NsHistorique::SetWindowPosHisto()
{
  Parent->Show(SW_HIDE) ;

  NSWindowProperty* pWinProp = pContexte->getUtilisateur()->getWindowProperty("History") ;
  if (pWinProp)
  {
    NS_CLASSLIB::TRect targetRect = pWinProp->getRect(pContexte->getSuperviseur()) ;

    //
    // fixer la nouvelle position
    // (on ne tient pas compte de la taille, vu le probleme pour restaurer
    //  une fenetre TView,TWindow mise en icone)
    //
	  Parent->SetWindowPos(0, targetRect.Left(), targetRect.Top(), targetRect.Width(), targetRect.Height(), SWP_NOZORDER) ;
  }

  Parent->Show(SW_SHOWNORMAL) ;

  /************************************
  if (sTaille == "I")
      Parent->Show(SW_SHOWMINIMIZED);
  else if (sTaille == "Z")
      Parent->Show(SW_SHOWMAXIMIZED);
  else
      Parent->Show(SW_SHOWNORMAL);
  *************************************/
}

//---------------------------------------------------------------------------//enregistrer la position de la fiche historique la base UTILISAT.DB
//---------------------------------------------------------------------------
void
NsHistorique::EnregistrePosHisto()
{
	if (false == pContexte->getUtilisateur()->isEnregWin())
  	return ;

	NS_CLASSLIB::TRect rectDlg = Parent->GetWindowRect() ;//coordonnées par % à l'écran
  int X, Y, W, H ;
  NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y()) ;
  Parent->Parent->ScreenToClient(point) ;
  X = point.X() ;
  Y = point.Y() ;
  W = rectDlg.Width() ;
  H = rectDlg.Height() ;

	if (Parent->IsIconic())
  	return ;

	string sUserId = pContexte->getUtilisateur()->getNss() ;
	NS_CLASSLIB::TRect  rect(X, Y, X + W, Y + H) ;
  NSWindowProperty wndProp ;
  wndProp.setX(X) ;
  wndProp.setY(Y) ;
  wndProp.setW(W) ;
  wndProp.setH(H) ;
  wndProp.setActivity(NSWindowProperty::undefined) ;
  wndProp.setFunction("History") ;

  pContexte->getUtilisateur()->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &wndProp, false) ;
  pContexte->getUtilisateur()->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &wndProp, true) ;
  //pContexte->getUtilisateur()->aWinProp.setProperty(sUserId, "History", rect, pContexte->PathName("FGLO")) ;
}

// Prevent processing of Close commands
//
/*
voidNsHistorique::EvSysCommand(uint cmdType, NS_CLASSLIB::TPoint& point)
{
	switch (cmdType& 0xFFF0)
	{
  	case SC_CLOSE:
    	break ;
    default:
    	TTreeWindow::EvSysCommand(cmdType, point) ;
	}
}
*/

void
NsHistorique::CeFileClose(TCommandEnabler& ce)
{
	ce.Enable(false) ;
}

void
NsHistorique::EvClose()
{
}

voidNsHistorique::EvDestroy()
{
	EnregistrePosHisto() ;
}

//---------------------------------------------------------------------------// Libération de la dernière barre d'outils créee
//---------------------------------------------------------------------------
void
NsHistorique::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
  NSTreeHistorique::EvSetFocus(hWndLostFocus) ;

	activateMUEViewMenu() ;

	TMyApp* pMyApp = pDoc->pContexte->getSuperviseur()->getApplication() ;

	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))	{
		SetupHistoBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}

  pMyApp->setMenu(string("menubar_histo"), &hAccelerator) ;
}

voidNsHistorique::Cm0Plus()
{
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "A")
	{
  	pNSTreeHistorique->setImportance(string("")) ;
    pNSTreeHistorique->EnlevePatho() ;//enlever les pathos des documents
	}
	else
	{
  	pNSTreeHistorique->setImportance(string("A")) ;
   	//affichage des patpathos des compte rendus et des consultations
   	pNSTreeHistorique->AffichePatho() ;
	}
}

voidNsHistorique::Cm1Plus()
{
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "B")
	{
  	pNSTreeHistorique->setImportance(string("")) ;
    pNSTreeHistorique->EnlevePatho() ; //enlever les pathos des documents
	}
	else
	{
  	pNSTreeHistorique->setImportance(string("B")) ;
   	//affichage des patpathos des compte rendus et des consultations
   	pNSTreeHistorique->AffichePatho() ;
	}
}

voidNsHistorique::Cm2Plus()
{
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "C")
	{
  	pNSTreeHistorique->setImportance(string("")) ;
    pNSTreeHistorique->EnlevePatho() ;//enlever les pathos des documents
	}
	else
	{
  	pNSTreeHistorique->setImportance(string("C")) ;
   	//affichage des patpathos des compte rendus et des consultations
   	pNSTreeHistorique->AffichePatho() ;
	}
}

voidNsHistorique::Cm3Plus()
{
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "D")
	{
  	pNSTreeHistorique->setImportance(string("")) ;
    pNSTreeHistorique->EnlevePatho() ;//enlever les pathos des documents
	}
  else
	{
  	pNSTreeHistorique->setImportance(string("D")) ;
   	//affichage des patpathos des compte rendus et des consultations
   	pNSTreeHistorique->AffichePatho() ;
	}
}

voidNsHistorique::Cm4Plus()
{
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "E")
	{
  	pNSTreeHistorique->setImportance(string("")) ;
    pNSTreeHistorique->EnlevePatho() ;//enlever les pathos des documents
	}
	else
	{
  	pNSTreeHistorique->setImportance(string("E")) ;
   	//affichage des patpathos des compte rendus et des consultations
   	pNSTreeHistorique->AffichePatho() ;
	}
}

voidNsHistorique::CmPublier()
{
	NSTreeHistorique* pTreeHis = this ;

	// on doit avoir un utilisateur pour lancer le pDocRefHisto
	if (pContexte->getUtilisateur() == 0)
  	return ;

	if (pDoc->_pHtmlCS)
  	delete pDoc->_pHtmlCS ;

	pDoc->_pHtmlCS = new NSHtml(tCS) ; 	// htmlNode racine du html de consultation

	InscrireHtml(pDoc->_pHtmlCS) ;

  // on ne contrôle pas si l'historique est vide
  // car il contient au moins la synthèse
  // on ne doit pas deleter pDocRefHisto car cela est fait
  // par la VisualView en fin d'impression

  pContexte->getUtilisateur()->setDocRefHisto(new NSHistoRefDocument(pDoc, pTreeHis->getImportance())) ;
  pContexte->getUtilisateur()->getDocRefHisto()->Publier() ;
}

voidNsHistorique::Ce0Plus(TCommandEnabler& ce)
{
	uint result = 0;
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "A")
  	result = 1 ;

	ce.SetCheck(result  ? TCommandEnabler::Checked : TCommandEnabler::Unchecked) ;
}

voidNsHistorique::Ce1Plus(TCommandEnabler& ce)
{
	uint result = 0 ;
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "B")  	result = 1 ;

	ce.SetCheck(result  ? TCommandEnabler::Checked : TCommandEnabler::Unchecked) ;
}

voidNsHistorique::Ce2Plus(TCommandEnabler& ce)
{
	uint result = 0 ;
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "C")
  	result = 1 ;

	ce.SetCheck(result  ? TCommandEnabler::Checked : TCommandEnabler::Unchecked) ;
}

voidNsHistorique::Ce3Plus(TCommandEnabler& ce){
	uint result = 0;
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "D")
  	result = 1 ;

	ce.SetCheck(result  ? TCommandEnabler::Checked : TCommandEnabler::Unchecked) ;
}

voidNsHistorique::Ce4Plus(TCommandEnabler& ce)
{
	uint result = 0 ;
	NSTreeHistorique* pNSTreeHistorique = this ;

	if (pNSTreeHistorique->getImportance() == "E")
  	result = 1 ;

	ce.SetCheck(result  ? TCommandEnabler::Checked : TCommandEnabler::Unchecked) ;
}

voidNsHistorique::SetupHistoBar()
{
	TMyApp* pMyApp = pDoc->pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_0P, CM_0P, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_1P, CM_1P, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_2P, CM_2P, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_3P, CM_3P, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_4P, CM_4P, TButtonGadget::Command)) ;

	pMyApp->getSecondaryControlBar()->LayoutSession() ;
}

// Fonction CanClose////////////////////////////////////////////////////////////////
bool
NsHistorique::CanClose()
{
	// on ruse avec un booléen dans PatientChoisi
	// pour résoudre le bug de la sauvegarde lors de la fermeture historique
	if ((pContexte->getPatient()) && (pContexte->getPatient()->_bCanCloseHisto))
	{
		TMyApp* pMyApp = pDoc->pContexte->getSuperviseur()->getApplication() ;
		pMyApp->FlushControlBar() ;
		bSetupToolBar = false ;

    string sMsg = string("History window is Ok to close") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

		return true ;
	}

  string sMsg = string("History window refuses to close") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

	return false ;
}

voidNsHistorique::InscrireHtml(NSHtml* pHtml)
{
	NSHtml htmlNode(tConsult) ;
	int		 cpt = 0 ;

	if (_NodeArray.empty())
		return ;

	for (iterNSTreeNode i = _NodeArray.begin() ; _NodeArray.end() != i ; i++)
	{
  	// on prend les éléments de premier niveau
    if (NULL == (*i)->getFatherNode())
    {
    	cpt++ ;

      if ((*i)->getControl())
      {
      	htmlNode.setText((*i)->GetText()) ;
        if (false == (*i)->getVectLinkedBros()->empty())
        {
        	for (iterNSTreeNode j = (*i)->getVectLinkedBros()->begin() ; (*i)->getVectLinkedBros()->end() != j ; j++)
          	htmlNode.addText(string(" ") + (*j)->GetText()) ;
        }
        pHtml->addSon(new NSHtml(htmlNode)) ;
        InscrireFils(*i, pHtml->GetLastSon()) ;
        // bConsultVide = false;
      }
      else if (cpt == 1)
      {
      	// bConsultVide = true;
        return ;
      }
    }
  }
}

voidNsHistorique::InscrireFils(NSTreeNode* pNode, NSHtml* pHtml)
{
  if (((NSTreeNode*) NULL == pNode) || ((NSHtml*) NULL == pHtml))
    return ;

  if (pNode->getVectSons()->empty())
    return ;

  // s'il existe des fils visibles
  //
  iterNSTreeNode itSon = pNode->getVectSons()->begin() ;

  NS_CLASSLIB::TRect rectItem ;
  if (false == (*itSon)->GetItemRect(rectItem))
    return ;

  NSHtml htmlNode(tConsult) ;

  for (iterNSTreeNode i = pNode->getVectSons()->begin(); pNode->getVectSons()->end() != i ; i++)
  {
    if ((*i)->getControl())
    {
      htmlNode.setText((*i)->GetText()) ;
      if (false == (*i)->getVectLinkedBros()->empty())
      {
        for (iterNSTreeNode j = (*i)->getVectLinkedBros()->begin(); (*i)->getVectLinkedBros()->end() != j ; j++)
          htmlNode.setText(string(" ") + (*j)->GetText()) ;
      }
      pHtml->addSon(new NSHtml(htmlNode)) ;
      InscrireFils(*i, pHtml->GetLastSon()) ;
    }
  }
}

voidNsHistorique::initMUEViewMenu(string sMenuName)
{
	if (pMUEViewMenu)
  	delete pMUEViewMenu ;

	nsMenuIniter menuIter(pDoc->pContexte) ;
	pMUEViewMenu = new OWL::TMenuDescr ;
  menuIter.initMenuDescr(pMUEViewMenu, sMenuName) ;

	return ;
}

void
NsHistorique::activateMUEViewMenu()
{
	if ((OWL::TMenuDescr*) NULL == pMUEViewMenu)
		return ;

	TMyApp* pMyApp = pDoc->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->GetMainWindow()->SetMenuDescr(*pMUEViewMenu) ;
	return ;
}
