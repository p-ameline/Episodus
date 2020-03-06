// -----------------------------------------------------------------------------
// nsmdiframe.cpp
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// $Revision: 1.54 $
// $Author: pameline $
// $Date: 2017/07/07 21:18:48 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2001-2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#include <ostream.h>
#include <checks.h>

#include <owl\pch.h>
#include <owl\applicat.h>
#include <owl\decmdifr.h>

#include <owl\gdiobjec.h>
#include <owl\dialog.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\statusba.h>
#include <owl\docmanag.h>
#include <owl\docview.h>
#include <owl\olemdifr.h>
#include <owl\oleview.h>
#include <owl\oledoc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winuser.h>

#include "nssavoir\nsbdmdriver.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "nsepisod\nsPilotProxy.h"
#include "nautilus\nsmdiframe.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsvisual.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nsTeamDocview.h"
#include "nautilus\nscqdoc.h"
#include "nautilus\nscqvue.h"
#include "nautilus\nscsdoc.h"
#include "nautilus\nscsvue.h"
#include "nautilus\nscrdoc.h"
#include "nautilus\nsDocView.h"
#include "nautilus\decisionTreeDV.h"
#include "nautilus\nsCoopChartDV.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsldvvue.h"
#include "nautilus\nsdrugview.h"
#include "nautilus\nsSOAPview.h"
#include "nautilus\nsgoalview.h"
#include "nautilus\nsFollowUpview.h"
#include "nautilus\nsrcview.h"
#include "nautilus\nsprocessview.h"
#include "nautilus\nsAlertSvce.h"
#include "partage\ns_timer.h"
#include "nssavoir\nscaptur.h"
#include "nsbb\nsWhoIsThere.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsarc.h"
#include "nsbb\nsdlg.h"
#include "nsldv\adopt.h"
#include "nsldv\nsldvstructs.h"
#include "nscompta\nscpta.h"

#include "ns_ob1\nautilus-bbk.h"
#include "ns_ob1\InterfaceForKs.h"

#include "nautilus\nautilus.rh"

DEFINE_RESPONSE_TABLE1(NSMDIFrame, NSMDIFrameBase)
  EV_COMMAND(IDM_TODO, CmToDo),
END_RESPONSE_TABLE;

NSMDIFrame::NSMDIFrame(NSContexte* pCtx, const char far *title, TResId menuResId, TMDIClient &clientWnd, bool trackMenuSelection, TModule* module)
           :NSMDIFrameBase(pCtx, title, menuResId, clientWnd, trackMenuSelection, module)
{
	// Printer = new NSPrinter ;
}

NSMDIFrame::~NSMDIFrame()
{
	// delete Printer;
  // Printer = 0;
}

void
NSMDIFrame::SetupWindow()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  char szThis[30] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsg = string("SetupWindow for NSMDIFrame ") + string(szThis) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  TDecoratedMDIFrame::SetupWindow() ;

  TMyApp* pApp = pContexte->getSuperviseur()->getApplication() ;
  char szApp[30] ;
  sprintf(szApp, "%p", pContexte->GetMainWindow()->HWindow) ;
  sMsg = string("- Main Window ") + string(szApp) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szStatusBar[30] ;
  if (pApp->getStatusBar())
    sprintf(szStatusBar, "%p", pApp->getStatusBar()->HWindow) ;
  else
    strcpy(szStatusBar, "not created") ;
  sMsg = string("- Status bar ") + string(szStatusBar) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szControlBar[30] ;
  if (pApp->getControlBar())
    sprintf(szControlBar, "%p", pApp->getControlBar()->HWindow) ;
  else
    strcpy(szControlBar, "not created") ;
  sMsg = string("- Control bar ") + string(szControlBar) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szHarbor[30] ;
  if (pApp->getHarbor())
    sprintf(szHarbor, "%p", pApp->getHarbor()->HWindow) ;
  else
    strcpy(szHarbor, "not created") ;
  sMsg = string("- Harbor ") + string(szHarbor) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  InitAutomaticImportTimers() ;

  // On reroute le parent de l'appli VCL vers la frame OWL
  //
  AdoptVCLAppWindow(GetHandle()) ;

  // Create Pilot Proxy window
  //
  openPilotProxy() ;
}

void
NSMDIFrame::openWhoIsThereWindow(string sInPatientsFileDesc)
{
  if (string("") == sInPatientsFileDesc)
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  NSMDIClient* pClient = pSuper->getApplication()->prendClient() ;

  string ps = string("Opening the in-patient window.") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  NSMDIChildContent*       pChildContent    = new NSMDIChildContent(pContexte, pClient) ;

  NSInPersonsWindow*       pInPersonsWindow = new NSInPersonsWindow(pContexte, pChildContent, sInPatientsFileDesc) ;
	pSuper->setInPersonsWindow(pInPersonsWindow) ;

  NSInPersonsGadgetWindow* pGadgetWnd       = new NSInPersonsGadgetWindow(pContexte, pChildContent) ;
  pGadgetWnd->setPaneSplitter(pInPersonsWindow) ;

  pInPersonsWindow->setGadgetWindow(pGadgetWnd) ;

  pChildContent->setSplitter(pInPersonsWindow) ;
  pChildContent->setToolBar(pGadgetWnd) ;

  string sTitle = pSuper->getText("inPatiensList", "inPatients") ;

  _pInPersonsChild = new NSInPersonsChild(pContexte, *(pSuper->getApplication()->prendClient()), 0, pChildContent) ;
  _pInPersonsChild->SetCaption(sTitle.c_str()) ;

	_pInPersonsChild->Create() ;
	pInPersonsWindow->SetParent(_pInPersonsChild) ;

  // _pInPersonsChild->SetWindowPos(0, 0, 0, 800, 400, SWP_NOZORDER) ;
  pInPersonsWindow->SetWindowPosit() ;
  pInPersonsWindow->SetFocusForInit() ;
}

// Check if new files appear in import directory
//
void
NSMDIFrame::CheckForAutomaticImport(TIMERIMPORTTYPES iImportType)
{
  if (((NSContexte*) NULL == pContexte) || ((NSPatientChoisi*) NULL == pContexte->getPatient()))
    return ;

  NSPatientChoisi *pPatient = pContexte->getPatient() ;
  if (false == pPatient->isOpen())
    return ;

  string sPathCode = string("") ;

  switch(iImportType)
  {
    case isScan :
      sPathCode = string("ASCN") ;
      break ;
    case isImage :
      sPathCode = string("AIMG") ;
      break ;
    default:
      return ;
  }

  string sSupport = string("") ;
  bool   bVerbose = false ;
  string sDocType = string("") ;

  string sImportDirectory = pContexte->PathName(sPathCode, &sSupport, bVerbose, &sDocType) ;
  if (string("") == sImportDirectory)
    return ;

  //
	// Looking for all files in directory
	//
  string sSearchPath = sImportDirectory + string("*.*") ;

  size_t iPathLength = strlen(sSearchPath.c_str()) + 1 ;
	char* szSearchPattern = new char[iPathLength] ;
  memset(szSearchPattern, 0, sizeof(iPathLength)) ;
  strcpy(szSearchPattern, sSearchPath.c_str()) ;

  // Looking for first file
  //
  WIN32_FIND_DATA stResult ;
	HANDLE hFoundFileHandle = FindFirstFile(szSearchPattern, &stResult) ;
	delete[] szSearchPattern ;

	//
	// If no file can be found, we leave
	//
	if (INVALID_HANDLE_VALUE == hFoundFileHandle)
		return ;  VectString _aFilesInDir ;  VectString *pPreviousFilesInDir = (VectString *) 0 ;  // We have to kill timer if we don't want the same document(s) to pile up  //  switch(iImportType)  {
    case isScan :
      KillTimer(ID_SCAN_TIMER) ;
      pPreviousFilesInDir = &(pPatient->_aFilesInScansDir) ;
      break ;
    case isImage :
      KillTimer(ID_IMAGE_TIMER) ;
      pPreviousFilesInDir = &(pPatient->_aFilesInImagesDir) ;
      break ;
  }

  //
	// While more files can be found
	//
	bool bTrouve = true ;
	while (bTrouve)
	{
		string sFileName = string(stResult.cFileName) ;

    _aFilesInDir.push_back(new string(sFileName)) ;

    if ((string(".") != sFileName) && (string("..") != sFileName) &&
        (false == pPreviousFilesInDir->contains(sFileName)))
    {
      string sNomSeul   = string("") ;
		  string sExtension = string("") ;

		  size_t pos1 = sFileName.find_last_of(".") ;
		  if (NPOS == pos1)
			  sNomSeul = sFileName ;
		  else
		  {
  		  sNomSeul   = string(sFileName, 0, pos1) ;
    	  sExtension = string(sFileName, pos1+1, strlen(sFileName.c_str())-pos1-1) ;
		  }

      // Get Mime type
      //
/*
      NSTypeMimeInfo infoTypeMime ;
      if (pContexte->getSuperviseur()->chercheTypeMimeInfo(sExtension, &infoTypeMime))
      {
        if (infoTypeMime.getType() == sDocType)
        {
          string ps = string("CheckForAutomaticImport import file ") + string(sFileName) ;
          pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

		      pContexte->getPatient()->Importer(sImportDirectory + sFileName) ;
        }
      }
*/

      pContexte->getPatient()->Importer(sImportDirectory + sFileName, true) ;
    }

    bTrouve = FindNextFile(hFoundFileHandle, &stResult) ;
  }

  FindClose(hFoundFileHandle) ;

  // In case patient was closed in meantime
  //
  if ((NULL == pContexte->getPatient()) ||
      (false == pContexte->getPatient()->isOpen()))
    return ;

  *pPreviousFilesInDir = _aFilesInDir ;

  // We can restart timers
  //
  switch(iImportType)
  {
    case isScan :
      SetTimer(ID_SCAN_TIMER, 5000) ;
      break ;
    case isImage :
      SetTimer(ID_IMAGE_TIMER, 5000) ;
      break ;
  }
}


void
NSMDIFrame::EvEndSession(bool endSession, bool logOff)
{
  // Si fin de session :
  // on detruit le superviseur, ce qui permet de débloquer le patient en cours
  if (bEndSession)
  {

    NSSuper* pSuper = pContexte->getSuperviseur() ;
    if (pSuper)
    {
      string sMsg = string("NSMDIFrame::EvEndSession Uninitializing Ole") ;
      pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;
    }

    ::OleUninitialize() ;

    if (pSuper)
    {
      delete pSuper ;
      pSuper = (NSSuper*) 0 ;
    }
  }
}

void
NSMDIFrame::CmToDo()
{
  if (_bToDoOnDuty)
    return ;

  _bToDoOnDuty = true ;

try
{
	if ((NSContexte*) NULL == pContexte)
  {
    _bToDoOnDuty = false ;
		return ;
  }

	NSSuper *pSuper = pContexte->getSuperviseur() ;
	if ((NSSuper*) NULL == pSuper)
  {
    _bToDoOnDuty = false ;
		return ;
  }

  NSTaskArray* pTaskArray = pSuper->getToDoArray() ;
  if ((NSTaskArray*) NULL == pTaskArray)
  {
    _bToDoOnDuty = false ;
		return ;
  }

	if (pTaskArray->empty())
  {
    _bToDoOnDuty = false ;
    return ;
  }

/*
	{
		ps = string("CmToDo with an empty ToDo array") ;
		pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
		return ;
	}
*/


/* This idea was not that smart since a WM_TODO could happen asynchronously
   leading to a later message being processed before earlier ones

	// Pour éviter les problèmes de synchronisation (un todo est ajouté
	// à l'array au milieu du traitement des todo précédents, on fait
	// une copie locale et on vide le réservoir global
  while (pSuper->isToDoLocked())
		pSuper->getApplication()->PumpWaitingMessages() ;

	pSuper->lockToDo() ;

  for (TaskIter iTD = pSuper->getToDoArray()->begin() ; pSuper->getToDoArray()->end() != iTD ; iTD++)
	{
		ps = string("globalToDo ") + (*iTD)->getWhatToDo() + string(" sP1=") + (*iTD)->getParam1() + string(" sP2=") + (*iTD)->getParam2() ;
		pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
	}

	NSTaskArray aLocalToDo = *(pSuper->getToDoArray()) ;
	pSuper->getToDoArray()->vider() ;

	pSuper->unlockToDo() ;
*/

/*
	for (TaskIter iTD = aLocalToDo.begin() ; aLocalToDo.end() != iTD ; iTD++)
	{
		ps = string("localToDo ") + (*iTD)->getWhatToDo() + string(" sP1=") + (*iTD)->getParam1() + string(" sP2=") + (*iTD)->getParam2() ;
		pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
	}
*/

	string sLang = pContexte->getUserLanguage() ;

/*
  if (aLocalToDo.empty() == false)
	for (TaskIter iTD = aLocalToDo.begin() ; aLocalToDo.end() != iTD ; )
*/
  for (TaskIter iTD = pTaskArray->begin() ; pTaskArray->end() != iTD ; )
	{
    // Make certain that this todo can't be executed twice
    //
    NSToDoTask *pToDo = *iTD ;
    pTaskArray->erase(iTD) ;

    ExecuteToDo(pToDo) ;

    string ps = string("Killing Todo ") + pToDo->getWhatToDo() ;
    pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

    delete pToDo ;

    // aLocalToDo.erase(iTD) ;

    // May cause stack overflow because it allows todo messages to recurse
    // pSuper->getApplication()->PumpWaitingMessages() ;
  }

  _bToDoOnDuty = false ;
}
catch (...)
{
  string sErr = string("Exception NSMDIFrame::CmToDo.") ;
  pContexte->getSuperviseur()->trace(&sErr, 1, NSSuper::trError) ;
  erreur(sErr.c_str(), standardError, 0) ;
  _bToDoOnDuty = false ;
}
}

void
NSMDIFrame::ExecuteToDo(NSToDoTask *pToDo)
{
  if ((NSToDoTask*) NULL == pToDo)
    return ;

  string sWhatToDo = pToDo->getWhatToDo() ;

try
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string ps = string("Executing Todo ") + sWhatToDo ;
  pSuper->trace(&ps, 1) ;

  if (string("Archetype") == sWhatToDo)
  {
    ToDo_Archetype(pToDo) ;
    return ;
  }
  if (string("ReferenceArchetypes") == sWhatToDo)
  {
    pContexte->getSuperviseur()->ReferenceTousArchetypes() ;
    return ;
  }
  if (string("KS_Archetype") == sWhatToDo)
  {
    ToDo_KS_Archetype(pToDo) ;
    return ;
  }
  if (string("KS_DecisionTree") == sWhatToDo)
  {
    ToDo_KS_DecisionTree(pToDo) ;
    return ;
  }
  if (string("FermeArchetype") == sWhatToDo)
  {
    ToDo_FermeArchetype(pToDo) ;
    return ;
  }
  if (string("Referential") == sWhatToDo)
  {
    if (pContexte->getPatient())
    {
      LDVFRAME iFrame = ldvframeHealth ;

      string sFrameIndex = pToDo->getParam2() ;
      if (string("") != sFrameIndex)
        iFrame = getFrameForIndex(StringToInt(sFrameIndex)) ;

      pContexte->getPatient()->CmReferentiel(pToDo->getParam1(), string(""), iFrame) ;
    }
    return ;
  }
  if (string("GenericReferential") == sWhatToDo)
  {
    if (pContexte->getPatient())
      pContexte->getPatient()->CmGenericReferentiel(pToDo->getParam1(), string("")) ;
    return ;
  }
  if (string("ClosePatientNextStep") == sWhatToDo)
  {
    if (pContexte->getUtilisateur())
      pContexte->getUtilisateur()->ClosePatientNextStep() ;
    return ;
  }
  if (string("ClosePatientWaitForHistory") == sWhatToDo)
  {
    // GetClientWindow()->MdiChildrenGarbageCollector() ;
    time(&_startOfWaitingTime) ;
    SetTimer(ID_HISTORY_TIMER, 50) ;
    return ;
  }
  if (string("PatientInitNextStep") == sWhatToDo)
  {
    if (pContexte->getPatient())
      pContexte->getPatient()->InitialisationNextStep() ;
    return ;
  }
  if (string("CreerBibArch") == sWhatToDo)
  {
    ToDo_CreerBibArch(pToDo) ;
    return ;
  }
  if (string("CreerBibChem") == sWhatToDo)
  {
    ToDo_CreerBibChem(pToDo) ;
    return ;
  }
  if (string("FermeDPIO") == sWhatToDo)
  {
    ToDo_FermeDPIO(pToDo) ;
    return ;
  }
  if (string("ActivateMUEView") == sWhatToDo)
  {
    ToDo_ActivateMUEView(pToDo) ;
    return ;
  }
  if (string("NewPreoccup") == sWhatToDo)
  {
    ToDo_NewPreoccup(pToDo) ;
    return ;
  }
  if (string("NewPreoccupProposal") == sWhatToDo)
  {
    ToDo_NewPreoccupProposal(pToDo) ;
    return ;
  }
  if (string("NewPreoccupFromNode") == sWhatToDo)
  {
    ToDo_NewPreoccupFromNode(pToDo) ;
    return ;
  }
  if (string("NewDrugFromNode") == sWhatToDo)
  {
    ToDo_NewDrugFromNode(pToDo) ;
    return ;
  }
  if (string("CreateMedication") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    if (pToDo->getPointer1())
    {
      NSPatPathoArray* pPatpatho = (static_cast<NSPatPathoArray*>(pToDo->getPointer1())) ;
      if (pPatpatho)
      {
        VecteurString* pRelatedConcerns = (static_cast<VecteurString*>(pToDo->getPointer2())) ;

        pPatient->CreeTraitement(pPatpatho, pRelatedConcerns) ;
      }
    }
    return ;
  }
  if (string("CreatePrescription") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    bool bManageALD = false ;
    string sWithALD = pToDo->getParam1() ;
    if (string("TRUE") == pseumaj(sWithALD))
      bManageALD = true ;

    pPatient->CreeOrdonnance(bManageALD, false) ;

    return ;
  }
  if (string("CreatePrescriptionFromSelection") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    bool bManageALD = false ;
    bool bSecured   = false ;
    string sWithALD = pToDo->getParam1() ;

    ClasseStringVector aParams ;
    DecomposeChaine(&sWithALD, &aParams, string("|")) ;

    if (false == aParams.empty())
    {
      if (string("TRUE") == pseumaj(aParams[0]->getItem()))
        bManageALD = true ;

      if ((aParams.size() > 1) && (string("TRUE") == pseumaj(aParams[1]->getItem())))
        bSecured   = true ;
    }

    bool bDCI = false ;
    string sDCI = pToDo->getParam2() ;
    if (string("TRUE") == pseumaj(sDCI))
      bDCI = true ;

    if (pToDo->getPointer1())
    {
      VecteurString* pDrugsRefs = (static_cast<VecteurString*>(pToDo->getPointer1())) ;
      if (pDrugsRefs)
        pPatient->CreeOrdonnanceFromSelection(bManageALD, pDrugsRefs, bDCI, bSecured) ;
    }

    return ;
  }
  if (string("RenewDrugFromNodes") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    if (pToDo->getPointer1())
    {
      VecteurString* pDrugNodes = (static_cast<VecteurString*>(pToDo->getPointer1())) ;
      if (pDrugNodes)
      {
        if (pToDo->getPointer2())
        {
          NSPatPathoArray* pPatpatho = (static_cast<NSPatPathoArray*>(pToDo->getPointer2())) ;
          if (pPatpatho)
            pPatient->RenouvelerTraitement(pDrugNodes, pPatpatho) ;
        }
      }
    }
    return ;
  }
  if (string("DrugAlertDisplayHtml") == sWhatToDo)
  {
    NSAlertServiceWindow* pAlertSWindow = pContexte->_pAlertBoxWindow ;
    if ((NSAlertServiceWindow*) NULL == pAlertSWindow)
      return ;

    pAlertSWindow->displayHtml(pToDo->getParam1(), pToDo->getParam2()) ;

    return ;
  }
  if (string("DrugAlertDisplayTxt") == sWhatToDo)
  {
    NSAlertServiceWindow* pAlertSWindow = pContexte->_pAlertBoxWindow ;
    if ((NSAlertServiceWindow*) NULL == pAlertSWindow)
      return ;

    pAlertSWindow->displayTxt(pToDo->getParam1(), pToDo->getParam2()) ;

    return ;
  }
  if (string("StopElementsFromNodes") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    VecteurString* pNodesToStop = (static_cast<VecteurString*>(pToDo->getPointer1())) ;
    if (pNodesToStop)
    {
      string sTimeStamp = pToDo->getParam2() ;
      if (string("") == sTimeStamp)
      {
        NVLdVTemps tpsNow ;
        tpsNow.takeTime() ;
        sTimeStamp = tpsNow.donneDateHeure() ;
      }

      pPatient->ArreterElement(pNodesToStop, sTimeStamp) ;
    }
    return ;
  }
  if (string("AddSubElement") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    NSPatPathoArray* pPhasePPT = (static_cast<NSPatPathoArray*>(pToDo->getPointer1())) ;
    if (pPhasePPT)
    {
      bool bSave = false ;
      string sSave = pToDo->getParam2() ;
      if (string("TRUE") == pseumaj(sSave))
        bSave = true ;

      pPatient->addSubElement(pToDo->getParam1(), pPhasePPT, bSave) ;
    }
    return ;
  }
  if (string("DeleteElementFromNode") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    string sNode = pToDo->getParam1() ;
    if (string("") != sNode)
      pPatient->SupprimerElement(sNode) ;

    string sReasonForUpdate = pToDo->getParam2() ;
    if ((string("") != sReasonForUpdate) && pPatient->getLdvDocument())
      pPatient->getLdvDocument()->invalidateViews(sReasonForUpdate) ;

    return ;
  }
  if (string("SwitchElementLexicon") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    if (pToDo->getPointer1())
    {
      ArrayLdvDrugs* pModifiedDrugs = (static_cast<ArrayLdvDrugs*>(pToDo->getPointer1())) ;
      if (pModifiedDrugs)
      {
        pPatient->SwitchElementLexicon(pModifiedDrugs) ;
        pPatient->getLdvDocument()->invalidateViews(string("DRUG_CHANGED")) ;
      }
    }

    return ;
  }
  if (string("ChangePreoccupFromNode") == sWhatToDo)
  {
    ToDo_ChangePreoccupFromNode(pToDo) ;
    return ;
  }
  if (string("DisplayAlert") == sWhatToDo)
  {
    ToDo_Message(pToDo, NsStripe::isAlert) ;
    return ;
  }
  if (string("DisplayWarning") == sWhatToDo)
  {
    ToDo_Message(pToDo, NsStripe::isWarning) ;
    return ;
  }
  if (string("DisplayMessage") == sWhatToDo)
  {
    ToDo_Message(pToDo, NsStripe::isInformation) ;
    return ;
  }
  // Ouverture d'une Fonction - Function opening
  if (string("OpenNewWindow") == sWhatToDo)
  {
    ToDo_OpenNewWindow(pToDo) ;
    return ;
  }
  if (string("FocusWindow") == sWhatToDo)
  {
    if (pToDo->getPointer1())
    {
      NSMUEView* pView = (static_cast<NSMUEView*>(pToDo->getPointer1())) ;
      if (pView)
        pView->SetFocus() ;
    }
    return ;
  }
  if (string("FreshenDocument") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatient)
      return ;

    NSHISTODocument* pHistory = pPatient->getDocHis() ;
    if ((NSHISTODocument*) NULL == pHistory)
      return ;

    if (pToDo->getPointer1())
    {
      NSDocumentHisto* pDocument = (static_cast<NSDocumentHisto*>(pToDo->getPointer1())) ;
      if (pDocument)
      {
        if (pToDo->getPointer2())
        {
          NSPatPathoArray* pPatpatho = (static_cast<NSPatPathoArray*>(pToDo->getPointer2())) ;
          if (pPatpatho)
            pHistory->Rafraichir(pDocument, pPatpatho) ;
        }
      }
    }
    return ;
  }
  if (string("NavigateComplete") == sWhatToDo)
  {
    if (pToDo->getPointer1())
    {
      NSVisualView* pView = (static_cast<NSVisualView *>(pToDo->getPointer1())) ;

      if (pView)
      {
      	// temporisation avant de lancer la publication
      	if (pView && pView->iTempPostNav > 0)
        	pSuper->Delay(pView->iTempPostNav) ;      	// publication [impression] du document (ExecWB)      	pView->CmPublier() ;
      }
    }
    return ;
  }
  if (string("AddDatasOnBBK") == sWhatToDo)
  {
    if (pToDo->getPointer1())
    {
      NSPatPathoArray *pPt = (NSPatPathoArray *)(pToDo->getPointer1()) ;
      if (pPt)
        addDatasFromNautilusOnBlackboard(pContexte, pPt) ;
    }
    return ;
  }
  if (string("AddToSOAP") == sWhatToDo)
  {
    NSEpisodus* pEpisod = pContexte->getEpisodus() ;
    if (pEpisod && pToDo->getPointer1())
    {
      NSCapture *pPt = (NSCapture *)(pToDo->getPointer1()) ;
      if (pPt)
        pEpisod->addToSOAP(pPt) ;
      else
      {
        SOAPObject *pPt2 = (SOAPObject *)(pToDo->getPointer1()) ;
        if (pPt2)
          pEpisod->addToSOAP(pPt2) ;
      }
    }
    return ;
  }
  if (string("RefreshHealthTeamInformation") == sWhatToDo)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if (pPatient && pPatient->_pHealthDoc)
      pPatient->_pHealthDoc->invalidateViews(pToDo->getParam1(), pToDo->getParam2()) ;
    return ;
  }
  if (string("SaveTreeviewInformation") == sWhatToDo)
  {
    if (NULL == pToDo->getPointer1())
      return ;

    NSTreeWindow *pTreeWindow = (NSTreeWindow *)(pToDo->getPointer1()) ;

    NSCsVue* pCsVue = dynamic_cast<NSCsVue*>(pTreeWindow) ;
    if ((NSCsVue*) NULL == pCsVue)
      return ;

    pCsVue->CmEnregistre() ;

    return ;
  }
  if (string("SaveTreeviewInformationAndConnectNew") == sWhatToDo)
  {
    if (NULL == pToDo->getPointer1())
      return ;

    NSTreeWindow *pTreeWindow = (NSTreeWindow *)(pToDo->getPointer1()) ;

    // Save the NSCsVue
    //
    NSCsVue* pCsVue = dynamic_cast<NSCsVue*>(pTreeWindow) ;
    if ((NSCsVue*) NULL == pCsVue)
      return ;

    pCsVue->CmEnregistre() ;

    // Establish the "copy of" link with newly created node
    //
    if (NULL == pToDo->getPointer2())
      return ;

    NSFatheredPatPathoArray *pFatherPpt = (NSFatheredPatPathoArray*)(pToDo->getPointer2()) ;
    if ((NSFatheredPatPathoArray*) NULL == pFatherPpt)
      return ;

    NSPatPathoInfo* pFatherItem = pFatherPpt->getFatherNode() ;

    // Looking for the node that fits context path
    //
    string sContextPath = pToDo->getParam1() ;

    NSTreeNode* pNode = pTreeWindow->GetNodeForPath(sContextPath, string(1, cheminSeparationMARK)) ;
    if ((NSTreeNode*) NULL == pNode)
      return ;

    // Find back just connexted node in order to set a "isCopy" link
    //
    if (pNode->getVectSons()->empty())
      return ;

    NSTreeNode *pNewNode = pNode->getVectSons()->back() ;
    if ((NSPatPathoInfo*) NULL == pFatherItem)
    {
      string sItemLabel = pToDo->getParam2() ;

      Message ConceptMsg ;
      NSPatPathoArray newPpt(pSuper) ;
      newPpt.parseBlock(sItemLabel, &ConceptMsg) ;

      NSPatPathoInfo pptInfo ;
      pptInfo.setLexique(ConceptMsg.GetLexique()) ;
      pptInfo.setUnit(ConceptMsg.GetUnit()) ;
      pptInfo.setPluriel(ConceptMsg.GetPluriel()) ;
      pptInfo.setCertitude(ConceptMsg.GetCertitude()) ;
      pptInfo.setComplement(ConceptMsg.GetComplement()) ;

      pFatherPpt->referenceFatherNode(&pptInfo) ;
      pFatherItem = pFatherPpt->getFatherNode() ;
    }
    if (pFatherItem)
    {
      string sNewNodeId = pNewNode->getNodeId() ;
      if (string("") != sNewNodeId)
        pFatherItem->addTemporaryLink(sNewNodeId, NSRootLink::copyOf, dirFleche) ;
    }

    return ;
  }
  if (string("NavigateEncyclopedia") == sWhatToDo)
  {
    if (NULL == pToDo->getPointer1())
      return ;

    NSTreeNode* pNSTreeNode = (static_cast<NSTreeNode *>(pToDo->getPointer1())) ;
    if ((NSTreeNode*) NULL == pNSTreeNode)
      return ;

    if (string("") == pNSTreeNode->getUrl())
      return ;

    string sLink = pNSTreeNode->getUrl() ;

    string sClassif ;
    string sTemp = pNSTreeNode->getEtiquette() ;
    pContexte->getDico()->donneCodeSens(&sTemp, &sClassif);
    //
    // If it is a classification, we have to check if we must adapt the URL
    //
    if (pSuper->getFilGuide()->VraiOuFaux(sClassif, "ES", "0CODE"))
    {
      size_t pos = sLink.find(sClassif) ;
      if (string::npos != pos)
      {
        string sCode = pNSTreeNode->getFreeTextCode() ;
        if (strlen(sLink.c_str()) > pos - strlen(sClassif.c_str()))
          sLink = string(sLink, 0, pos) + sCode + string(sLink, pos + strlen(sClassif.c_str()), strlen(sLink.c_str()) - pos - strlen(sClassif.c_str())) ;
        else
          sLink = string(sLink, 0, pos) + sCode ;
      }
    }

    pSuper->NavigationEncyclopedie(sLink, pContexte) ;

    return ;
  }
  if (string("NavigateCertificat") == sWhatToDo)
  {
    if (NULL == pToDo->getPointer1())
      return ;

    NSTreeNode* pNSTreeNode = (static_cast<NSTreeNode *>(pToDo->getPointer1())) ;
    if ((NSTreeNode*) NULL == pNSTreeNode)
      return ;

    if (string("") == pNSTreeNode->getUrl())
      return ;

    string sLink = pNSTreeNode->getUrl() ;

    string sClassif ;
    string sTemp = pNSTreeNode->getEtiquette() ;
    pContexte->getDico()->donneCodeSens(&sTemp, &sClassif);
    //
    // If it is a classification, we have to check if we must adapt the URL
    //
    if (pSuper->getFilGuide()->VraiOuFaux(sClassif, "ES", "0CODE"))
    {
      size_t pos = sLink.find(sClassif) ;
      if (string::npos != pos)
      {
        string sCode = pNSTreeNode->getFreeTextCode() ;
        if (strlen(sLink.c_str()) > pos - strlen(sClassif.c_str()))
          sLink = string(sLink, 0, pos) + sCode + string(sLink, pos + strlen(sClassif.c_str()), strlen(sLink.c_str()) - pos - strlen(sClassif.c_str())) ;
        else
          sLink = string(sLink, 0, pos) + sCode ;
      }
    }

    pSuper->NavigationCertificat(sLink, pContexte) ;

    return ;
  }
  if (string("ExportDocForDeletedCompt") == sWhatToDo)
  {
    ToDo_ExportDocForDeletedCompt(pToDo) ;
    return ;
  }
  if (string("GetPersonAsynch") == sWhatToDo)
  {
    ToDo_GetPersonAsynch(pToDo) ;
    return ;
  }
  if (string("CloseCurrentPatient") == sWhatToDo)
  {
    TMyApp *pMyApp = pContexte->getSuperviseur()->getApplication() ;
    if (pMyApp)
      pMyApp->CmBureau() ;
    return ;
  }
  if (string("OpenDocument") == sWhatToDo)
  {
    ToDo_OpenDocument(pToDo) ;
    return ;
  }
  if (string("EditDocument") == sWhatToDo)
  {
    ToDo_EditDocument(pToDo) ;
    return ;
  }
  if (string("ExportDocument") == sWhatToDo)
  {
    ToDo_ExportDocument(pToDo) ;
    return ;
  }
  if (string("SaveEpisodusData") == sWhatToDo)
  {
    NSEpisodus* pEpisodus = pContexte->getEpisodus() ;
    if ((NSEpisodus*) NULL == pEpisodus)
      return ;

    if (string("") == pToDo->getParam1())
      pEpisodus->saveParams() ;
    else
      pEpisodus->saveParams(pToDo->getParam1()) ;

    return ;
  }
  if (string("SettleUser") == sWhatToDo)
  {
    if (pToDo->getPointer1())
    {
      NSUtilisateurChoisi* pNewUser = (static_cast<NSUtilisateurChoisi *>(pToDo->getPointer1())) ;
      if (pNewUser)
        pSuper->SettleNewUser(pNewUser) ;
		}
    return ;
  }
  if (string("CloseMainWindow") == sWhatToDo)
  {
    pSuper->donneMainWindow()->CloseWindow() ;
    return ;
  }
  if (string("CloseDocument") == sWhatToDo)
  {
    if (pToDo->getPointer1())
    {
      NSDocumentInfo* pDocInfo = (static_cast<NSDocumentInfo*>(pToDo->getPointer1())) ;
      if (pDocInfo)
      {
        // Get Documents manager
        //
        NSHISTODocument* pDocHis = pContexte->getPatient()->getDocHis() ;
        if ((NSHISTODocument*) NULL == pDocHis)
          return ;

        // Tell Documents manager to consider this document as closed
        //
        pDocHis->FermetureDocument(pDocInfo) ;
      }
		}
    return ;
  }
  if (string("CloseDocumentAndWindow") == sWhatToDo)
  {
    if (pToDo->getPointer1())
    {
      NSDocumentInfo* pDocInfo = (static_cast<NSDocumentInfo*>(pToDo->getPointer1())) ;
      if (pDocInfo)
      {
        // Get Documents manager
        //
        NSHISTODocument* pDocHis = pContexte->getPatient()->getDocHis() ;
        if ((NSHISTODocument*) NULL == pDocHis)
          return ;

        // Tell Documents manager to consider this document as closed
        //
        pDocHis->FermetureDocument(pDocInfo) ;

        // Close window
        //
        if (pToDo->getPointer2())
        {
          TWindow* pOpenedWindows = (static_cast<TWindow*>(pToDo->getPointer2())) ;
          if (pOpenedWindows)
            pOpenedWindows->CloseWindow() ;
        }
      }
		}
    return ;
  }
  if (string("OpenFromCapture") == sWhatToDo)
  {
    NSEpisodus* pEpisod = pContexte->getEpisodus() ;
    if (pEpisod)
    {
      NSCaptureArray* pNewCaptureArray = &(pEpisod->newCaptureArray) ;
      pContexte->captureData(pNewCaptureArray) ;
    }
    else
    {
      string sErrorMsg = string("NSMDIFrame::ExecuteToDo OpenFromCapture can't find NSEpisodus") ;
      pSuper->trace(&ps, 1, NSSuper::trError) ;
    }
    return ;
  }
  if (string("OpenFromLdvId") == sWhatToDo)
  {
    string sLdvId = pToDo->getParam1() ;
    if (string("") == sLdvId)
      return ;

    NSUtilisateurChoisi *pUtilisat = pContexte->getUtilisateur() ;
    if ((NSUtilisateurChoisi*) NULL == pUtilisat)
      return ;

    pUtilisat->OuvrePatient(sLdvId) ;

    return ;
  }
  if (string("OpenPatSearchDialog") == sWhatToDo)
  {
    NSUtilisateurChoisi *pUtilisat = pContexte->getUtilisateur() ;
    if ((NSUtilisateurChoisi*) NULL == pUtilisat)
      return ;

    pUtilisat->AppelPatient() ;

    return ;
  }

  ps = string("Unknown Todo ") + sWhatToDo ;
  pSuper->trace(&ps, 1, NSSuper::trWarning) ;
}
catch (...)
{
  string sErr = string("Exception NSMDIFrame::ExecuteToDo for ToDo ") + sWhatToDo ;
  pContexte->getSuperviseur()->trace(&sErr, 1, NSSuper::trError) ;
  erreur(sErr.c_str(), standardError, 0) ;
}
}

void
NSMDIFrame::ToDo_Archetype(NSToDoTask* pToDo)
{
  if ((NSToDoTask*) NULL == pToDo)
    return ;

	NSSuper *pSuper = pContexte->getSuperviseur() ;
	string ps = string("Todo Archetype : ") + pToDo->getParam1() ;
  pSuper->trace(&ps, 1) ;

  string sArchetype = pToDo->getParam1() ;
  string sTypeArc   = pToDo->getParam2() ;
  int    iTypeArc   = atoi(sTypeArc.c_str()) ;

  if (NSCQDocument::creatpat == iTypeArc)
  {
  	// En cas de création d'un nouveau patient
    // on ferme l'eventuel patient en cours
    if (pContexte->getPatient())
    {
      NSUtilisateurChoisi* pUser = pContexte->getUtilisateur() ;

      if (false == pUser->CanClosePatient())
        return ;

      pUser->setToDoAfterPatClose(pToDo) ;
      pUser->ClosePatientAsynchronous(NSUtilisateurChoisi::cpnExecToDo) ;

      return ;

    	// pContexte->getUtilisateur()->fermePatEnCours() ;
    }
  }

  ps = string("Todo Archetype : ") + sArchetype + string(" création du document") ;
  pSuper->trace(&ps, 1) ;

  // création d'un document vierge (on transmet l'archetype)
  // on lance le modèle document/vues formulaire
  NSCQDocument* pCQDoc = new NSCQDocument(0, pContexte, sArchetype, iTypeArc) ;
  if (pCQDoc->isParsed())
  {
  	ps = string("Todo Archetype : ") + sArchetype + string(" création de la vue") ;
    pSuper->trace(&ps, 1) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pCQDoc, "CQ Format") ;
  }
  else
  {
  	delete pCQDoc ;
    ps = string("Todo Archetype : ") + sArchetype + string(" abort (mauvais Archetype)") ;
    pSuper->trace(&ps, 1) ;
  }
}

voidNSMDIFrame::ToDo_KS_Archetype(NSToDoTask* pToDo){#ifdef __OB1__  if ((NSToDoTask*) NULL == pToDo)    return ;	NSSuper *pSuper = pContexte->getSuperviseur() ;	BB1BBInterfaceForKs* pKSinterface = (static_cast<BB1BBInterfaceForKs *>(pToDo->getPointer1())) ;
  string ps ;

  if ((BB1BBInterfaceForKs*) NULL == pKSinterface)
  {
  	ps = string("Todo KS_Archetype with wrong BB1BBInterfaceForKs pointer") ;
    pSuper->trace(&ps, 1) ;
    return ;
  }

  BB1BBInterfaceForKs ksInterface = *pKSinterface ;

  string sArchetype = ksInterface.getArchetypeID() ;
  string sKSname    = ksInterface.getKsName() ;

  ps = string("Todo KS_Archetype : ") + sArchetype + string(" for KS : ") + sKSname ;
  pSuper->trace(&ps, 1) ;

  ps = string("Todo Archetype : ") + sArchetype + string(" création du document") ;
  pSuper->trace(&ps, 1) ;

  // création d'un document vierge (on transmet l'archetype)
  // on lance le modèle document/vues formulaire
  NSCQDocument* pCQDoc = new NSCQDocument(0, pContexte, &ksInterface) ;
  if (pCQDoc->isParsed())
  {
  	ps = string("Todo Archetype : ") + sArchetype + string(" création de la vue") ;
    pSuper->trace(&ps, 1) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pCQDoc, "CQ Format") ;
  }
  else
  {
  	delete pCQDoc ;
    ps = string("Todo Archetype : ") + sArchetype + string(" abort (mauvais Archetype)") ;
    pSuper->trace(&ps, 1) ;
  }

/*
  if (pToDo->mustDeleteP1())
  {
  	delete pKSinterface ;
    pToDo->setPointer1(0) ;
  }
*/
#endif
}
voidNSMDIFrame::ToDo_KS_DecisionTree(NSToDoTask* pToDo){#ifdef __OB1__  if ((NSToDoTask*) NULL == pToDo)    return ;	NSSuper *pSuper = pContexte->getSuperviseur() ;	BB1BBInterfaceForKs* pKSinterface = (static_cast<BB1BBInterfaceForKs *>(pToDo->getPointer1())) ;
  string ps ;

  if ((BB1BBInterfaceForKs*) NULL == pKSinterface)
  {
  	ps = string("Todo Decision tree with wrong BB1BBInterfaceForKs pointer") ;
    pSuper->trace(&ps, 1) ;
    return ;
  }

  BB1BBInterfaceForKs ksInterface = *pKSinterface ;

  string sArchetype = ksInterface.getArchetypeID() ;
  string sKSname    = ksInterface.getKsName() ;

  ps = string("Todo Decision tree : ") + sArchetype + string(" for KS : ") + sKSname ;
  pSuper->trace(&ps, 1) ;

  ps = string("Todo Decision tree : ") + sArchetype + string(" création du document") ;
  pSuper->trace(&ps, 1) ;

  // création d'un document vierge (on transmet l'archetype)
  // on lance le modèle document/vues formulaire
  NSDecisionTreeDocument* pDecTreeDoc = new NSDecisionTreeDocument(0, pContexte, &ksInterface) ;
  if (pDecTreeDoc->getParsingOk())
  {
  	ps = string("Todo Decision tree : ") + sArchetype + string(" création de la vue") ;
    pSuper->trace(&ps, 1) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pDecTreeDoc, "Decision Tree") ;
  }
  else
  {
  	delete pDecTreeDoc ;
    ps = string("Todo Decision tree : ") + sArchetype + string(" abort (mauvais Archetype)") ;
    pSuper->trace(&ps, 1) ;
  }

/*
  if (pToDo->mustDeleteP1())
  {
  	delete pKSinterface ;
    pToDo->setPointer1(0) ;
  }
*/
#endif
}
voidNSMDIFrame::ToDo_FermeArchetype(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;	NSSuper *pSuper = pContexte->getSuperviseur() ;  if ((NSSuper*) NULL == pSuper)  	return ;	string ps = string("Début Todo : FermeArchetype") ;  pSuper->trace(&ps, 1) ;

  string sLang = pContexte->getUserLanguage() ;

  NSSmallBrother  *pBB   = (static_cast<NSSmallBrother *>(pToDo->getPointer1())) ;
  NSCQWindowsView *pView = (static_cast<NSCQWindowsView *>(pToDo->getPointer2())) ;
  string           sName = pToDo->getParam1() ;

  if ((NSSmallBrother*) NULL == pBB)
    return ;

  // Empty patpatho
  //
  if (pBB->getPatPatho()->empty())
  {
    string ps = string("NSMDIFrame::ToDo_FermeArchetype TMyapp Init main window: Entering.") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;

    string sWarnMsg = pSuper->getText("documentManagementErrors", "cannotSaveAnEmptyDocument") ;
    erreur(sWarnMsg.c_str(), warningError, 0) ;

    if (pView)
      pView->setWaitingForSystem(false) ;

    return ;
  }

  bool existeInfo ;
  NSCQDocument *pDocum = pView->_pDoc ;
  pDocum->setPatPatho(pBB->getPatPatho()) ;

  if (NSCQDocument::creatpat == pDocum->getDocType())
  {
    NSPatPathoArray aPPt(pContexte->getSuperviseur()) ;
    pDocum->initFromPatPatho(&aPPt) ;
    pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

    if (pContexte->getUtilisateur()->createPatient(&aPPt))
      pContexte->getPatient()->InitialisationAsynchronous() ;
    else
      pContexte->setPatient(NULL) ;

    pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;

    if (pView)
      pView->setWaitingForSystem(false) ;

    pBB->canCloseArchetype(true, false) ;
  }
  else if (NSCQDocument::creatadr == pDocum->getDocType())
  {
    NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
    pDocum->initFromPatPatho(&PatPatho) ;

    NSAddressGraphManager GraphManager(pSuper) ;
    GraphManager.setGraphAdr(pContexte, &PatPatho, sLang) ;

    if (pView)
      pView->setWaitingForSystem(false) ;

    pBB->canCloseArchetype(true) ;
  }
  else if (pDocum->_pDocInfo)
  {
    bool bIsDemographicArchetype = false ;
    if (pContexte->getSuperviseur()->getDemographicArchetypeId() == pDocum->getDefaultArchetype())
      bIsDemographicArchetype = true ;

    // string sArchetype = string("") ;
    // if ((NULL != pDocum->pPatPathoArray) && (false == pDocum->pPatPathoArray->empty()))
    // 	sArchetype = (*(pDocum->pPatPathoArray->begin()))->getArchetype() ;

    if (true == bIsDemographicArchetype)
    {
      // Manage IPP
      //
      string sIPP = string("") ;

      NSPatPathoArray PatPathoAdmin(pContexte->getSuperviseur()) ;
      pDocum->initFromPatPatho(&PatPathoAdmin) ;

      pContexte->getUtilisateur()->getAndRemoveIppFromPatpatho(&PatPathoAdmin, &sIPP) ;

      if ((string("") != sIPP) && (string("") != pSuper->getIppSite()))
        pContexte->getPatient()->updateIPPEnCours(pSuper->getIppSite(), sIPP) ;
    }

    pDocum->enregistrePatPatho() ;
    // on recharge la patpatho pour récupérer les nouveaux nodes
    // Reloading the patpatho in order to freshen nodes
    pDocum->NSNoyauDocument::chargePatPatho() ;
    // string sArchetype = (*(pDocum->pPatPathoArray->begin()))->pDonnees->getArchetype() ;

    NSPatPathoArray PatPathoDoc(pContexte->getSuperviseur()) ;
    pDocum->initFromPatPatho(&PatPathoDoc) ;

    pContexte->getPatient()->getDocHis()->Rafraichir(pDocum->_pDocInfo, &PatPathoDoc) ;

    if (pView)
      pView->setWaitingForSystem(false) ;

    if (false == bIsDemographicArchetype)
    {
      ps = string("Fermeture effective du document ") + pDocum->getArchetype() ;
      pSuper->trace(&ps, 1) ;
      pBB->canCloseArchetype(true) ;
    }
    else
      pContexte->getPatient()->PatientDataChanged(&PatPathoDoc) ;
  }
  else
  {
    // cas des nouveaux documents formulaire
    string sLibelleDoc = sName ;
    if (string("") == sLibelleDoc)
    {
      string sCode = (*(pBB->getPatPatho()->begin()))->getLexique() ;
      if (string("") != sCode)
        pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
    }

    string ps = string("Fermeture formulaire ") + sLibelleDoc ;
    pSuper->trace(&ps, 1) ;

    ps = string("Referencement formulaire ") + sLibelleDoc ;
    pSuper->trace(&ps, 1) ;

    // Référencement sans boite de dialogue
    existeInfo = pDocum->Referencer("ZCQ00", sLibelleDoc, "", "", true, false) ;

    if (existeInfo)
    {
      ps = string("Enregistrement de l'arbre du formulaire ") + sLibelleDoc ;
      pSuper->trace(&ps, 1) ;
      existeInfo = pDocum->enregistrePatPatho() ;
    }

    // on recharge la patpatho pour récupérer les nouveaux nodes
    if (existeInfo)
    {
      existeInfo = pDocum->NSNoyauDocument::chargePatPatho() ;

      // This is done later... and it leads to showing it to BBK twice
      // pContexte->getPatient()->pDocHis->Rafraichir(pDocum->pDocInfo, pDocum->pPatPathoArray) ;
    }

    if (existeInfo)
    {
      NSPatPathoArray PatPathoDoc(pContexte->getSuperviseur()) ;
      pDocum->initFromPatPatho(&PatPathoDoc) ;

      pContexte->getPatient()->getLdvDocument()->showNewTree(&PatPathoDoc, pDocum->GetDateDoc(false)) ;
      pDocum->makeSOAP() ;
    }

    NSPredi *pPredi = pContexte->getPredi() ;

    if (existeInfo)
    {
      string sRacineDoc = pDocum->DonneDocInfo()->getID() ;

      NSPatPathoArray PatPathoDoc(pContexte->getSuperviseur()) ;
      pDocum->initFromPatPatho(&PatPathoDoc) ;

      if (pPredi->_bSendPage)
      {
        string sDateDoc  = pDocum->GetDateDoc(false) ;
        string sDateDPIO = string(sDateDoc, 6, 2) + string("/") + string(sDateDoc, 4, 2) + string("/") + string(sDateDoc, 0, 4) ;

        pPredi->_sSendPage += string("<document code=\"") + sRacineDoc + string("\" date=\"") + sDateDPIO + string("\">\r\n") + PatPathoDoc.genereXML() + string("</document>\r\n") ;
      }

      // Rafraichir l'historique
      pContexte->getPatient()->getDocHis()->Rafraichir(pDocum->_pDocInfo, &PatPathoDoc /*, NSNoyauDocument* pNouveauDocument*/) ;

      // Archetype ?
      string sNomRef = string("") ;

      if (pBB && pBB->getBBItem() && pBB->getBBItem()->getParseur() && pBB->getBBItem()->getParseur()->getArchetype())
      {
        Carchetype *pArchetype = pBB->getBBItem()->getParseur()->getArchetype() ;

        if (pArchetype->getDialog())
          sNomRef = pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_REF) ;

        // Connexion à (ouverture de) la préoccupation
        string sConcernCode = string("") ;
        // bool    bConnected      = false ;

        Creferences *pRef = pArchetype->getReference() ;
        if (pRef)
        {
          Cconcern *pConcern = pRef->getFirstCconcern() ;
          if (pConcern)
          {
            sConcernCode = pConcern->getCode() ;

            bool bCreate = pConcern->getAutoCreate() ;
            int  iSevere = pConcern->getSeverity() ;

            string sFrameCode = pConcern->getFrame() ;

            if ((string("") != sConcernCode) && (pContexte->getPatient()) && (pContexte->getPatient()->getLdvDocument()))
            {
              ps = string("Connection à sa preoccupation du formulaire ") + sLibelleDoc ;
              pContexte->getSuperviseur()->trace(&ps, 1) ;
              NSLdvDocument *pDocLdv = pContexte->getPatient()->getLdvDocument() ;

              if (('I' == sConcernCode[0]) || ('_' == sConcernCode[0]))
              {
                bool bConnected = false ;
                for (int i = 0 ; (i < FRAMECOUNT) && (false == bConnected) ; i++)
                {
                  LDVFRAME iFrame = getFrameForIndex(i) ;
                  bConnected = pDocLdv->connectObjectToDrug(iFrame, sConcernCode, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
                }
              }
              else if ('#' != sConcernCode[0])
              {
                LDVFRAME iFrame = ldvframeHealth ;
                if (string("") != sFrameCode)
                  iFrame = pDocLdv->getFrameTypeFromRoot(sFrameCode) ;

                /*bConnected =*/ pDocLdv->connectObjectToConcern(iFrame, sConcernCode, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
              }
              // Si sConcernCode[0] == '#', c'est qu'on se connecte à
              // la préoccupation lanceuse du formulaire
              else
              {
                string sPreoType = string(sConcernCode, 1, strlen(sConcernCode.c_str()) - 1) ;
                string sQuestion = string("0PRO1/IsA/") + sPreoType ;
                NSPatPathoArray *pAnswer = NULL ;
#ifdef __OB1__
                string sAnswerDate ;
                if ((pContexte->getBBinterface()) && (pContexte->getBBinterface()->getAnswer2Question(sQuestion, "", &pAnswer, sAnswerDate, false)))
#else
                if (pSuper->getBlackboard()->getAnswer2Question(sQuestion, "", &pAnswer, false))
#endif // !__OB1__
                {
                  if (pAnswer && (false == pAnswer->empty()))
                  {
                    PatPathoIter iter = pAnswer->begin() ;
                    string sLaunchingPreoc = (*iter)->getLexique() ;
                    LDVFRAME iFrame = ldvframeNotSpecified ;
                    NSConcern* pConcern = pDocLdv->getConcern((*iter)->getNode(), iFrame) ;
                    if (pConcern)
                      pDocLdv->connectObjectToConcern(iFrame, sLaunchingPreoc, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
                  }
                }
                if (pAnswer)
                  delete pAnswer ;
              }
            }
          }
        }

        // Lancement du référentiel
        if (string("") != sNomRef)
        {
          ps = string("Lancement du referentiel pour le formulaire ") + sLibelleDoc ;
          pSuper->trace(&ps, 1) ;
          pContexte->getPatient()->CmReferentiel(sNomRef, sRacineDoc, ldvframeHealth) ;
        }

        if ((NSCQDocument::dpio == pDocum->getDocType()) && pPredi && pPredi->mustLogPage())
        {
          ps = string("Log PréDI du formulaire ") + sLibelleDoc ;
          pSuper->trace(&ps, 1) ;
          // pDPIO->sLogPage += pDocum->donneTexte() ;
          pPredi->AppendLogFile() ;
        }

        // Fermeture du questionnaire
        // A faire en dernier car pDocum disparait à la fermeture de la vue
        ps = string("Fermeture effective du formulaire ") + sLibelleDoc ;
        pSuper->trace(&ps, 1) ;

        if (pView)
          pView->setWaitingForSystem(false) ;

        pBB->canCloseArchetype(true) ;
        // Fait par la vue
        //delete pBB ;
      }
      else if (pBB)
      {
        if (pView)
          pView->setWaitingForSystem(false) ;
        pBB->canCloseArchetype(true) ;
        // Fait par la vue
        //delete pBB ;
      }
    }
  }
}
voidNSMDIFrame::ToDo_CreerBibArch(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;  NSDocumentInfo DocInfo(pContexte) ;
  // Patient : Patient en cours
  DocInfo.setPatient("_------") ;

  // Code : par définition "_00000"
  string sCodeDocum = string("_") + string(DOC_CODE_DOCUM_LEN - 1, '0') ;
  DocInfo.setDocument(sCodeDocum) ;

  // Ne pas passer pDocInfo dans le cteur pour éviter qu'il essaie de
  // charger la patpatho
  NSRefDocument DocLibArch(0, 0, 0, pContexte, false) ;
  DocLibArch._pDocInfo = new NSDocumentInfo(DocInfo) ;

  NSPatPathoArray PptLibArch(pContexte->getSuperviseur()) ;
  DocLibArch.initFromPatPatho(&PptLibArch) ;

  PptLibArch.ajoutePatho("0LIBA1", 0) ;

	DocLibArch.setPatPatho(&PptLibArch) ;
  DocLibArch.enregistrePatPatho() ;

  // IMPORTANT : on doit remettre pDocInfo et pHtmlInfo à 0
  // pour éviter que ~NSRefDocument ne remette à jour l'historique
  DocLibArch._pDocInfo = 0 ;

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
  MessageBox("La bibliothèque d'Archetypes a été créée avec succès.", sCaption.c_str(), MB_OK) ;}voidNSMDIFrame::ToDo_CreerBibChem(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;	// Création de l'arbre-bibliothèque de chemises  if (pContexte->getPatient() == 0)
  {
    erreur("Vous devez ouvrir un patient avant de lancer cette option.", standardError, 0) ;
    return ;
  }

  NSRefDocument LibChem(0, 0, 0, pContexte, false) ;

  NSPatPathoArray PptLibChem(pContexte->getSuperviseur()) ;
  LibChem.initFromPatPatho(&PptLibChem) ;

  PptLibChem.ajoutePatho("0LIBC1", 0) ;

  // ajout de la chemise corbeille
  // Note : cette chemise doit être créée en premier afin que la chemise par défaut
  // soit sélectionnée plus tard dans EnregDocDialog (on sélectionne la dernière chemise)
  PptLibChem.ajoutePatho("0CHEM1", 1) ;

  // Intitulé : nom de la chemise
  PptLibChem.ajoutePatho("0INTI1", 2) ;
  Message Msg ;
  Msg.SetTexteLibre("corbeille") ;
  PptLibChem.ajoutePatho("£?????", &Msg, 3) ;

  // Date d'ouverture
  PptLibChem.ajoutePatho("KOUVR1", 2) ;
  string sDateCreation = donne_date_duJour() + string("000000") ;
  Msg.Reset() ;
  Msg.SetUnit("2DA021") ;
  Msg.SetComplement(sDateCreation.c_str()) ;
  PptLibChem.ajoutePatho("£T0;19", &Msg, 3) ;

  // ajout de la chemise par défaut
  PptLibChem.ajoutePatho("0CHEM1", 1) ;

  // Intitulé : nom de la chemise
  PptLibChem.ajoutePatho("0INTI1", 2) ;
  Msg.Reset() ;
  Msg.SetTexteLibre("défaut") ;
  PptLibChem.ajoutePatho("£?????", &Msg, 3) ;

  // Date d'ouverture
  PptLibChem.ajoutePatho("KOUVR1", 2) ;
  Msg.Reset() ;
  Msg.SetUnit("2DA021") ;
  Msg.SetComplement(sDateCreation.c_str()) ;
  PptLibChem.ajoutePatho("£T0;19", &Msg, 3) ;

  LibChem.setPatPatho(&PptLibChem) ;

  if (false == LibChem.Referencer("ZCS00", "Bibliothèque de chemises", "", "", true, false, "", NSRootLink::personFolderLibrary))
  {
    erreur("Impossible de referencer la bibliothèque de chemises du patient.", standardError, 0) ;
    return ;
  }

  if (false == LibChem.enregistrePatPatho())
  {
    erreur("Impossible d'enregistrer la bibliothèque de chemises du patient.", standardError, 0) ;
    return ;
  }
  LibChem.NSNoyauDocument::chargePatPatho() ;

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
  MessageBox("La bibliothèque de chemises a été créée avec succès.", sCaption.c_str(), MB_OK) ;}voidNSMDIFrame::ToDo_FermeDPIO(NSToDoTask* pToDo){try{  if ((NULL == pToDo) || (NULL == pToDo->getPointer1()))    return ;  NSPredi *pPredi = pContexte->getPredi() ;  if (NULL == pPredi)    return ;  pPredi->fermeFormulaire(pToDo->getPointer1()) ;
/*  TMDIChild *pView = (static_cast<TMDIChild *>(pToDo->pPointer1)) ;  if (NULL == pView)
  	return ;

  //if (pView->pDoc->iTypeDoc == NSCQDocument::dpio)
  //{
  NSDPIO  *pDPIO = pContexte->getSuperviseur()->getDPIO() ;
  //TMDIChild* pChild = pView->Parent ;
  pDPIO->fermeFormulaire((TWindow*)pView) ;
  //}

  // PA 29/05/09
  // if (pView->IsWindow())
  //   delete pView ;
*/
}
catch (...)
{
  erreur("Exception ToDo_FermeDPIO.", standardError, 0) ;
}
}voidNSMDIFrame::ToDo_ActivateMUEView(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;	// NSMUEView* pMueView = NULL ;  // if (pToDo->pPointer1)
  //  pMueView = (static_cast<NSMUEView *>(pToDo->pPointer1)) ;

  NSDialog* pDialog = (NSDialog*) 0 ;
  if (pToDo->getPointer2())
    pDialog = (static_cast<NSDialog *>(pToDo->getPointer2())) ;

  ::SetForegroundWindow(pContexte->GetMainWindow()->GetHandle());
  if (pDialog)
  {
    pDialog->_bActivateMUEView = false;
    ::SetActiveWindow(pDialog->GetHandle());
  }
  if (pContexte->GetMainWindow()->IsIconic())
    ::SetForegroundWindow(pContexte->GetMainWindow()->GetHandle());}// So far, this only creates a new concern in the health frame//voidNSMDIFrame::ToDo_NewPreoccup(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;	// TWindow *pView  = (static_cast<TWindow *>(pToDo->pPointer1)) ;  string sLexique = pToDo->getParam1() ;
  string sDate    = pToDo->getParam2() ;

  NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
  if ((NSLdvDocument*) NULL == pLdvDoc)
    return ;

  // On vérifie que la préoccupation n'existe pas déjà
  //
  string sCodeSensConcern ;
  pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSensConcern) ;

  NSConcern* pConcern = pLdvDoc->donneOpenConcern(ldvframeHealth, sCodeSensConcern) ;
  if (pConcern)
    return ;

  string sNewNode = string("") ;

  TWindow* pView = 0 ;
  NSLdvView *pLdvView = pLdvDoc->getLdvView() ;
  if (pLdvView)
    pView = pLdvView ;
  else
    pView = this ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;
  concernInfo.setLexique(sLexique) ;
  concernInfo.setStartDate(sDate) ;

  NSNewConcernDlg *pNPDialog = new NSNewConcernDlg(pView, pLdvDoc, pContexte, &concernInfo) ;
  int iExecReturn = pNPDialog->Execute() ;

  if (IDOK == iExecReturn)
    pLdvDoc->newLine(ldvframeHealth, sNewNode, &concernInfo) ;

  delete pNPDialog ;
}voidNSMDIFrame::ToDo_NewPreoccupFromNode(NSToDoTask* pToDo){try{  if ((NSToDoTask*) NULL == pToDo)    return ;	NSTreeWindow *pView = (static_cast<NSTreeWindow *>(pToDo->getPointer1())) ;  NSTreeNode   *pNode = (static_cast<NSTreeNode *>(pToDo->getPointer2())) ;
  if (((NSTreeWindow*) NULL == pView) || ((NSTreeNode*) NULL == pNode))
  	return ;

  NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;

  string sNewNode = string("") ;
  string sLexique = pNode->getEtiquette() ;

  // Get possible CISP code
  //
  string sCocCode = pToDo->getParam1() ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;
  concernInfo.setLexique(sLexique) ;
  concernInfo.setCocCode(sCocCode) ;

	NSNewConcernDlg *pNPDialog = (NSNewConcernDlg*) 0 ;
  TDialog *pDlg = (TDialog*) 0 ;

  TWindow* pControlParent = pView->Parent ;
  if (pControlParent)
	{
  	pDlg = TYPESAFE_DOWNCAST(pControlParent, TDialog) ;
    if (pDlg)
    	pNPDialog = new NSNewConcernDlg(pDlg, pLdvDoc, pContexte, &concernInfo) ;
  }

	if ((NSNewConcernDlg*) NULL == pNPDialog)
		pNPDialog = new NSNewConcernDlg(pView, pLdvDoc, pContexte, &concernInfo) ;

  int iExecReturn = pNPDialog->Execute() ;
  delete pNPDialog ;

  if (IDOK == iExecReturn)
  {
    pLdvDoc->newLine(ldvframeHealth, sNewNode, &concernInfo) ;

    if (string("") != sNewNode)
      pNode->addTemporaryLink(sNewNode, NSRootLink::problemContactElement, dirFleche) ;
    pLdvDoc->getConcerns(ldvframeHealth)->reinit() ;
  }
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception ToDo_NewPreoccupFromNode : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception ToDo_NewPreoccupFromNode.", standardError, 0) ;
}
}// Creation of a proposal stripe//voidNSMDIFrame::ToDo_NewPreoccupProposal(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;  NSConcernToDoInformation *pConcernToDo = (static_cast<NSConcernToDoInformation *>(pToDo->getPointer1())) ;  if ((NSConcernToDoInformation*) NULL == pConcernToDo)    return ;	// TWndow *pView  = (static_cast<TWindow *>(pToDo->pPointer1)) ;  string sLexique = pConcernToDo->getLexique() ;
  string sDate    = pConcernToDo->getStartingDate() ;

  NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
  if ((NSLdvDocument*) NULL == pLdvDoc)
    return ;

  // On vérifie que la préoccupation n'existe pas déjà
  //
  string sCodeSensConcern ;
  pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSensConcern) ;

  NSConcern* pConcern = pLdvDoc->donneOpenConcern(ldvframeHealth, sCodeSensConcern) ;
  if (pConcern)
    return ;

  NsPreoccupStripe* stripe = new NsPreoccupStripe(pContexte, sLexique, sDate, pConcernToDo->getText(), pConcernToDo->getUrl(), this, NsStripe::isWarning) ;
  stripe->Create() ;
  pContexte->getSuperviseur()->getApplication()->getHarbor()->Insert(*stripe, OWL::alBottom) ;
  stripe->LayoutSession() ;
}

void
NSMDIFrame::ToDo_Message(NSToDoTask* pToDo, NsStripe::STRIPETYPE iStripeType)
{
  if ((NSToDoTask*) NULL == pToDo)
    return ;  string sText = pToDo->getParam1() ;  string sUrl  = pToDo->getParam2() ;

  NsAlertStripe* stripe = new NsAlertStripe(pContexte, sText, sUrl, 0, this, iStripeType) ;
  stripe->Create() ;
  pContexte->getSuperviseur()->getApplication()->getHarbor()->Insert(*stripe, OWL::alBottom) ;
  stripe->LayoutSession() ;
}
voidNSMDIFrame::ToDo_NewDrugFromNode(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;	NSTreeWindow *pView = (static_cast<NSTreeWindow *>(pToDo->getPointer1())) ;  NSTreeNode   *pNode = (static_cast<NSTreeNode *>(pToDo->getPointer2())) ;
  if (((NSTreeWindow*) NULL == pView) || ((NSTreeNode*) NULL == pNode))
  	return ;

  string sLexique = pNode->getEtiquette() ;

  NSLdvDocument   *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
  NSDrugView      *pDrugView = pLdvDoc->getDrugView("") ;
  if ((NSDrugView*) NULL == pDrugView)
  {
    pContexte->getPatient()->drugs_show() ;
    for (int i = 0 ; (i < 50) && !pDrugView ; i++)
      pDrugView = pLdvDoc->getDrugView("") ;
  }

  if (pDrugView)
    pDrugView->CmAddWithCode(sLexique) ;
}voidNSMDIFrame::ToDo_ChangePreoccupFromNode(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;	NSTreeWindow *pView = (static_cast<NSTreeWindow *>(pToDo->getPointer1())) ;  NSTreeNode   *pNode = (static_cast<NSTreeNode *>(pToDo->getPointer2())) ;
  string       sCcrn  = pToDo->getParam1() ;

  if (((NSTreeWindow*) NULL == pView) || ((NSTreeNode*) NULL == pNode))
  	return ;

  NSLdvDocument *pLdvDoc     = pContexte->getPatient()->getLdvDocument() ;
  NSConcern     *pOldConcern = pLdvDoc->getConcerns(ldvframeHealth)->getConcern(sCcrn) ;

  string sNewNode = string("") ;
  string sLexique = pNode->getEtiquette() ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;
  concernInfo.setLexique(sLexique) ;

  NSNewConcernDlg *pNPDialog = new NSNewConcernDlg(pView, pLdvDoc, pContexte, &concernInfo) ;
  int iExecReturn = pNPDialog->Execute() ;
  delete pNPDialog  ;

  if (IDOK == iExecReturn)
  {
    pLdvDoc->newLine(ldvframeHealth, sNewNode, &concernInfo) ;

    pNode->addTemporaryLink(sNewNode, NSRootLink::problemContactElement, dirFleche) ;
    pLdvDoc->getConcerns(ldvframeHealth)->reinit() ;

    // On part du principe que la nouvelle préoccupation est en fin d'array

    ArrayConcern  *pConcernArray  = pLdvDoc->getConcerns(ldvframeHealth) ;
    NSConcern     *pNewConcern    = pConcernArray->back() ;

    /* bool bSuccess = */ pLdvDoc->creeConcernSuite(ldvframeHealth, pOldConcern, pNewConcern) ;
  }
}voidNSMDIFrame::ToDo_OpenNewWindow(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;	// sParam1 = fonction - function  // sParam2 = préoccupation - concern
  // pPointer1 = fenêtre splittée - splitted window
  //
  string sDocumentType = string("") ;

  // Reservoir SOAP - SOAP tank
  if      (pToDo->getParam1() == "Tanking")
  {
    sDocumentType = "SoapDoc" ;

    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if ((NSMUEView*) NULL == pSplittedView)
      return ;

    NSPaneSplitter *pPaneSpl = pSplittedView->getPaneSplitter() ;
    NSMUEView      *pWindWithDocType = pPaneSpl->researchDocument("SoapDoc") ;

    NSSOAPDocument *pNewDocSOAP = (NSSOAPDocument*) 0 ;
    if ((NSMUEView*) NULL == pWindWithDocType)
      pNewDocSOAP = new NSSOAPDocument(0, pContexte) ;
    else
      pNewDocSOAP = TYPESAFE_DOWNCAST(pWindWithDocType->pDoc, NSSOAPDocument) ;

    if ((NSSOAPDocument*) NULL == pNewDocSOAP)
      return ;

    NSSOAPTankView *pNSSOAPTankView = new NSSOAPTankView(*pNewDocSOAP, 0) ;

    if (pSplittedView->getCreateWindow())
      pNSSOAPTankView->setCreateWindow(pSplittedView->getCreateWindow()) ;
    else
      pNSSOAPTankView->setCreateWindow(pSplittedView) ;

    pNSSOAPTankView->setSplittedWindow(pSplittedView) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pNewDocSOAP, "SOAP tank", pNSSOAPTankView) ;
  }
  // Paniers SOAP - SOAP baskets
  else if (pToDo->getParam1() == "Soaping")
  {
    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if ((NSMUEView*) NULL == pSplittedView)
    {
      // createSOAPView();
      NSSOAPDocument *pNewDocSOAP = new NSSOAPDocument(0, pContexte) ;
      NSSOAPView     *pNSSOAPView = new NSSOAPView(*pNewDocSOAP, 0, "Soaping") ;

      pNSSOAPView->setCreateWindow(NULL) ;
      pNSSOAPView->setSplittedWindow(NULL) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pNewDocSOAP, "SOAP baskets", pNSSOAPView) ;
    }
  }
  // Médicaments - Drugs
  else if (pToDo->getParam1() == "DrugManagement")
  {
    sDocumentType = string("LdvDoc") ;

    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if ((NSMUEView*) NULL == pSplittedView)
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if ((NSPatientChoisi*) NULL == pPatEnCours)
      {
        erreur("Il n'y a pas de patient actif!", standardError, 0) ;
        return ;
      }
      pPatEnCours->drugs_show(pToDo->getParam2()) ;
    }
    else
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if ((NSPatientChoisi*) NULL == pPatEnCours)
      {
        erreur("Imposible d'ouvrir la fenetre demandée ! \nIl n'y a pas de dossier patient actif.\nVeuillez ouvrir un patient.", standardError, 0) ;
        return ;
      }

      // NSPaneSplitter  *pPaneSpl = pSplittedView->getPaneSplitter() ;
      NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
      if ((NSLdvDocument*) NULL == pLdvDoc)
        return ;

      NSDrugView *pDrugView = new NSDrugView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pDrugView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pDrugView->setCreateWindow(pSplittedView) ;

      pDrugView->setSplittedWindow(pSplittedView) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pLdvDoc, "Drug Management", pDrugView) ;
    }
  }
  // Objectifs de santé - Health goals
  else if (pToDo->getParam1() == "GoalManagement")
  {
    NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatEnCours)
    {
      erreur("Imposible d'ouvrir la fenetre demandée ! \nIl n'y a pas de dossier patient actif.\nVeuillez ouvrir un patient.", standardError, 0) ;
      return ;
    }

    sDocumentType = string("LdvDoc") ;

    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if ((NSMUEView*) NULL == pSplittedView)
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if ((NSPatientChoisi*) NULL == pPatEnCours)
      {
        erreur("Il n'y a pas de patient actif!", standardError, 0) ;
        return ;
      }
      pPatEnCours->goals_show(pToDo->getParam2()) ;
    }
    else
    {
      NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
      if ((NSLdvDocument*) NULL == pLdvDoc)
        return ;

      NSGoalView *pGoalView = new NSGoalView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pGoalView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pGoalView->setCreateWindow(pSplittedView) ;

      pGoalView->setSplittedWindow(pSplittedView) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pLdvDoc, "Goal Management", pGoalView) ;
    }
  }
  // Follow Up
  else if (pToDo->getParam1() == "FollowUpManagement")
  {
    sDocumentType = string("LdvDoc") ;

    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if ((NSMUEView*) NULL == pSplittedView)
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if ((NSPatientChoisi*) NULL == pPatEnCours)
      {
        erreur("Il n'y a pas de patient actif!", standardError, 0) ;
        return ;
      }
      pPatEnCours->drugs_show(pToDo->getParam2()) ;
    }
    else
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if ((NSPatientChoisi*) NULL == pPatEnCours)
      {
        erreur("Imposible d'ouvrir la fenetre demandée ! \nIl n'y a pas de dossier patient actif.\nVeuillez ouvrir un patient.", standardError, 0) ;
        return ;
      }

      // NSPaneSplitter  *pPaneSpl = pSplittedView->getPaneSplitter() ;
      NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
      if ((NSLdvDocument*) NULL == pLdvDoc)
        return ;

      NSFollowUpView *pFUView = new NSFollowUpView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pFUView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pFUView->setCreateWindow(pSplittedView) ;

      pFUView->setSplittedWindow(pSplittedView) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pLdvDoc, "FollowUp Management", pFUView) ;
    }
  }
  else if (pToDo->getParam1() == "FollowUpShow")
  {
    NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatEnCours)
      return ;

    pPatEnCours->followUp_show(pToDo->getParam2()) ;

    return ;
  }
  // Procédures - Processes
  else if (pToDo->getParam1() == "ProcessManagement")
  {
    NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
    if ((NSPatientChoisi*) NULL == pPatEnCours)
    {
      erreur("Imposible d'ouvrir la fenetre demandée ! \nIl n'y a pas de dossier patient actif.\nVeuillez ouvrir un patient.  ", standardError, 0) ;
      return ;
    }

    sDocumentType = string("LdvDoc") ;

    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if ((NSMUEView*) NULL == pSplittedView)
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if ((NSPatientChoisi*) NULL == pPatEnCours)
      {
        erreur("Il n'y a pas de patient actif!", standardError, 0) ;
        return ;
      }
      pPatEnCours->process_show(pToDo->getParam2()) ;
    }
    else
    {
      NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
      if ((NSLdvDocument*) NULL == pLdvDoc)
        return ;

      NSProcessView *pProcessView = new NSProcessView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pProcessView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pProcessView->setCreateWindow(pSplittedView) ;

      pProcessView->setSplittedWindow(pSplittedView) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pLdvDoc, "Process Management", pProcessView) ;
    }
  }
  // Liste des résultats de consultation
  else if (pToDo->getParam1() == "EpiRCManagement")
  {
    sDocumentType = string("LdvDoc") ;
    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;

    NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
    if ((NSLdvDocument*) NULL == pLdvDoc)
      return ;

    NSDocViewManager dvManager(pContexte) ;

    if (NULL == pSplittedView)
      dvManager.createView(pLdvDoc, "RC view Format") ;
    else
    {
      NSEpisodRCView *pRCView = new NSEpisodRCView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pRCView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pRCView->setCreateWindow(pSplittedView) ;
      pRCView->setSplittedWindow(pSplittedView) ;

      dvManager.createView(pLdvDoc, "RC view Format", pRCView) ;
    }
  }
  else if (pToDo->getParam1() == string("CoopChart"))
  {
    string sQualityCardFile = string("") ;
    string sNode            = string("") ;
    splitString(pToDo->getParam2(), &sQualityCardFile, &sNode, '|') ;

    if ((string("") != sQualityCardFile) && (string("") != sNode))
    {
      NSCoopChartDocument* pDocNewCoopChart = new NSCoopChartDocument(0, pContexte, sQualityCardFile, sNode) ;
      NSDocViewManager dvManager(pContexte) ;
	    dvManager.createView(pDocNewCoopChart, string("Coop Chart")) ;
    }
  }
  // Historique des résultats de consultation
  else if (pToDo->getParam1() == "RcItemHistory")
  {
    sDocumentType = string("LdvDoc") ;
    NSMUEView* pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;

    NSLdvDocument* pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
    if ((NSLdvDocument*) NULL == pLdvDoc)
      return ;

    NSDocViewManager dvManager(pContexte) ;

    if (NULL == pSplittedView)
      dvManager.createView(pLdvDoc, "RC histo Format") ;
    else
    {
      NSRCHistoryView *pHistoryView = new NSRCHistoryView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pHistoryView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pHistoryView->setCreateWindow(pSplittedView) ;

      pHistoryView->setSplittedWindow(pSplittedView) ;
      dvManager.createView(pLdvDoc, "RC histo Format", pHistoryView) ;
    }
  }
  // Fiche administrative -
  else if (pToDo->getParam1() == "AdminData")
  {
    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;

    if ((NSMUEView*) NULL == pSplittedView)
    {
      NSDocumentInfo DocumentInfo(pContexte) ;

      NSPatPathoArray PatPathoAdmin(pContexte->getSuperviseur()) ;

      NSPatientChoisi *pPat = pContexte->getPatient() ;
      string sCodeDocAdminData = pPat->getAdminData(&PatPathoAdmin) ;
      if (string("") != sCodeDocAdminData)
      {
        string sCodePat = string(sCodeDocAdminData, 0,            PAT_NSS_LEN) ;
        string sCodeDoc = string(sCodeDocAdminData, PAT_NSS_LEN,  DOC_CODE_DOCUM_LEN) ;

        DocumentInfo.setPatient(sCodePat) ;
        DocumentInfo.setDocument(sCodeDoc) ;
        DocumentInfo.setType("CQ030") ;

        pPat->ChargeDonneesPatient(&PatPathoAdmin) ;
        pPat->initCorrespArray(&PatPathoAdmin) ;
      }
      else
      {
        string ps = string("Impossible de retrouver le document fiche administrative du patient.") ;
        erreur(ps.c_str(), standardError, 0) ;
        pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
      }

      // On doit lancer postinitialisation AVANT d'ouvrir la fiche administrative
      // (fabrication du nom long, calcul de l'age du patient, etc.)
      //
      pPat->getDocHis()->AutoriserEdition(&DocumentInfo) ;
    }
  }
  // Document treeWindow - treeWindow document
  else if ((strlen(pToDo->getParam1().c_str()) == BASE_LEXI_LEN + 3) && (string(pToDo->getParam1(), 0, 3) == "Cs:"))
  {
    string sRootLexique = string(pToDo->getParam1(), 3, BASE_LEXI_LEN) ;

    sDocumentType = "LdvDoc" ;
    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;

    if (!pSplittedView)
    {
      NSCSDocument *pNewDocCS = new NSCSDocument(0, pContexte, sRootLexique) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pNewDocCS, "CS Format") ;
    }
    else
    {
      /*
      NSPaneSplitter  *pPaneSpl = pSplittedView->getPaneSplitter() ;

      NSCSTemplate    *pCSTemplate = new NSCSTemplate("CS", "", 0, "CS", dtAutoDelete) ;
      NSCSDocument    *pNewDocCS = new NSCSDocument(0, _appContext, sRootLexique) ;
      pNewDocCS->SetTemplate(pCSTemplate) ;
      NSCsVue         *pCsVue = new NSCsVue(*pNewDocCS, 0) ;

      if (pSplittedView->getCreateWindow())
        pCsVue->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pCsVue->setCreateWindow(pSplittedView) ;

      pCsVue->setSplittedWindow(pSplittedView) ;

      pCSTemplate->InitView(pCsVue) ;
      */
    }
  }}voidNSMDIFrame::ToDo_ExportDocForDeletedCompt(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;  if ((string("") == pToDo->getParam1()) || (string("") == pToDo->getParam2()))    return ;  if (CPTA_NUMCOMPT_LEN != strlen(pToDo->getParam1().c_str()))    return ;  string sNumCompt = pToDo->getParam1() ;  if (PAT_NSS_LEN + DOC_CODE_DOCUM_LEN != strlen(pToDo->getParam2().c_str()))    return ;  string sPatientId  = string(pToDo->getParam2(), 0, PAT_NSS_LEN) ;  string sDocumentId = string(pToDo->getParam2(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;  NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;  if ((NSPatientChoisi*) NULL == pPatEnCours)    return ;  if (pPatEnCours->getNss() != sPatientId)    return ;  NSHISTODocument* pDocHis = pPatEnCours->getDocHis() ;  if ((NSHISTODocument*) NULL == pDocHis)    return ;  NSDocHistoArray* pVectDocument = pDocHis->getVectDocument() ;	if (((NSDocHistoArray*) NULL == pVectDocument) || pVectDocument->empty())
  	return ;  DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sPatientId + sDocumentId) ;	if ((DocumentIter) NULL == iterDoc)    return ;
  if (pDocHis->EstUnDocumentOuvert(*iterDoc))
    return ;

  if (string("ZCN00") != (*iterDoc)->getTypeSem())
    return ;

  NSDocumentInfo *pDocum = new NSDocumentInfo(pContexte) ;
  (*iterDoc)->initDocumentInfo(pDocum) ;
  NSDocumentInfo *pDocHtml  = 0 ;

  NSCRDocument ReportDoc(0, pDocum, pDocHtml, pContexte, "", true /* read only */) ;
  int iReussi = ReportDoc.decodeCR(&(ReportDoc._aCRPhrases)) ;

  NSDocStatus docStatus ;
  docStatus.setStatus(NSDocStatus::StatusUnchanged) ;
  docStatus.setCptaStatus(NSDocStatus::StatusDeleted) ;
  docStatus.setDocId(sPatientId + sDocumentId) ;
  docStatus.setNumCompt(sNumCompt) ;

  ReportDoc.exportReport(&docStatus) ;
}
voidNSMDIFrame::ToDo_GetPersonAsynch(NSToDoTask* pToDo){  if ((NSToDoTask*) NULL == pToDo)    return ;  if ((string("") == pToDo->getParam1()) || (string("") == pToDo->getParam2()))    return ;  PIDSTYPE iTypePids = pidsPatient ;  if      (string("pidsPatient")      == pToDo->getParam2())    iTypePids = pidsPatient ;  else if (string("pidsCorresp")      == pToDo->getParam2())    iTypePids = pidsCorresp ;  else if (string("pidsUtilisat")     == pToDo->getParam2())    iTypePids = pidsUtilisat ;  else if (string("pidsPatientGroup") == pToDo->getParam2())    iTypePids = pidsPatientGroup ;  else if (string("pidsAll")          == pToDo->getParam2())    iTypePids = pidsAll ;  NSPilotProxyWindow* pPilotProxy = pContexte->getSuperviseur()->getPilotProxyWindow() ;  if ((NSPilotProxyWindow*) NULL == pPilotProxy)    return ;  pPilotProxy->getPerson(pToDo->getParam1(), iTypePids) ;}/** * Open a document from its ID */voidNSMDIFrame::ToDo_OpenDocument(NSToDoTask* pToDo){  if (((NSToDoTask*) NULL == pToDo) || (string("") == pToDo->getParam1()))    return ;  // No patient or patient being closed -> no way  //  NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;  if (((NSPatientChoisi*) NULL == pPatEnCours) || pPatEnCours->isClosing())    return ;  // Get the documents manager  //  NSHISTODocument* pDocManager = pPatEnCours->getDocHis() ;  if ((NSHISTODocument*) NULL == pDocManager)    return ;  // Find the document from its ID, then open it  //  DocumentIter docIter = pDocManager->TrouveDocHisto(pToDo->getParam1()) ;  if ((DocumentIter) NULL == docIter)    return ;  pDocManager->AutoriserOuverture((NSDocumentInfo*) *docIter) ;}/** * Edit a document from its ID */voidNSMDIFrame::ToDo_EditDocument(NSToDoTask* pToDo){  if (((NSToDoTask*) NULL == pToDo) || (string("") == pToDo->getParam1()))    return ;  // Get the documents manager  //  NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;  if ((NSPatientChoisi*) NULL == pPatEnCours)    return ;  NSHISTODocument* pDocManager = pPatEnCours->getDocHis() ;  if ((NSHISTODocument*) NULL == pDocManager)    return ;  // Find the document from its ID, then open it in edit mode  //  DocumentIter docIter = pDocManager->TrouveDocHisto(pToDo->getParam1()) ;  if ((DocumentIter) NULL == docIter)    return ;  pDocManager->AutoriserEdition((NSDocumentInfo*) *docIter) ;}/** * Export a document from its ID */voidNSMDIFrame::ToDo_ExportDocument(NSToDoTask* pToDo){  if (((NSToDoTask*) NULL == pToDo) || (string("") == pToDo->getParam1()))    return ;  // Get the documents manager  //  NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;  if ((NSPatientChoisi*) NULL == pPatEnCours)    return ;  NSHISTODocument* pDocManager = pPatEnCours->getDocHis() ;  if ((NSHISTODocument*) NULL == pDocManager)    return ;  // Find the document from its ID, then open it  //  DocumentIter docIter = pDocManager->TrouveDocHisto(pToDo->getParam1()) ;  if ((DocumentIter) NULL == docIter)    return ;  pPatEnCours->DocumentXmlExport(*docIter) ;}

