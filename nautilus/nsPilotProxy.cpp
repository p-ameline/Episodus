// NSBROWSE.CPP : Dialogues de composition et de publication
////////////////////////////////////////////////////////////
#include <cstring.h>
#include <stdio.h>#include <assert.h>/** Includes spécifiques capture **/#include <string.h>#include <windows.h>#include <wingdi.h>///////////////////////////////////
#include "nautilus\nsPilotProxy.h"
#include "nautilus\nssuper.h"
#include "partage\ns_timer.h"
/****************** classe NSImportWindow **************************/
DEFINE_RESPONSE_TABLE1(NSPilotProxyWindow, TWindow)
   EV_WM_CLOSE,
   EV_WM_TIMER,
END_RESPONSE_TABLE;

NSPilotProxyWindow::NSPilotProxyWindow(TWindow* parent, NSSuper* pSuper)
                   :TWindow(parent)
{
	_pSuper = pSuper ;
}

NSPilotProxyWindow::~NSPilotProxyWindow()
{
  string sMsg = string("Deleting Pilot Proxy Window") ;
  _pSuper->trace(&sMsg, 1, NSSuper::trSteps) ;
}

bool
NSPilotProxyWindow::CanClose()
{
  if ((NSSuper*) NULL == _pSuper)
    return true ;

  NSContexte* pContext = _pSuper->getContexte() ;
  if (pContext && pContext->getUtilisateur())
	  return false ;

  return true ;
}

void
NSPilotProxyWindow::SetupWindow()
{
	TWindow::SetupWindow() ;

  SetTimer(ID_PILOTPROXY_TIMER, 50) ;

  string sMsg = string("Pilot Proxy Window is opening") ;
  _pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;
}

void
NSPilotProxyWindow::Hide()
{
  Parent->Show(SW_MINIMIZE) ;
}

void
NSPilotProxyWindow::EvTimer(uint timerId)
{
	if (ID_PILOTPROXY_TIMER != timerId)
		return ;

  if (_aToDo.empty())
    return ;

  // Get next job to get done and immediately remove it from jobs stack
  //
  vector<string>::iterator iter = _aToDo.begin() ;
  string sJobToDo = *iter ;
  _aToDo.erase(iter) ;

  // Parse job
  //
  string sCommand = sJobToDo ;
  string sParams  = string("") ;

  size_t iSeparPos = sJobToDo.find("|") ;
  if (NPOS != iSeparPos)
  {
    sCommand = string(sJobToDo, 0, iSeparPos) ;
    sParams  = string(sJobToDo, iSeparPos + 1, strlen(sJobToDo.c_str()) - iSeparPos - 1) ;
  }

  if (string("GetPerson") == sCommand)
    getPersonSynchro(sParams) ;
}

void
NSPilotProxyWindow::getPerson(string sID, PIDSTYPE iTypePids)
{
  if (string("") == sID)
    return ;

  string sCommand = string("GetPerson|") + sID + string("|") ;

  switch(iTypePids)
  {
    case pidsPatient :
      sCommand += string("pidsPatient") ;
      break ;
    case pidsCorresp :
      sCommand += string("pidsCorresp") ;
      break ;
    case pidsUtilisat :
      sCommand += string("pidsUtilisat") ;
      break ;
    case pidsPatientGroup :
      sCommand += string("pidsPatientGroup") ;
      break ;
    case pidsAll :
      sCommand += string("pidsAll") ;
      break ;
  }

  _aToDo.push_back(sCommand) ;
}

//
// This function synchronously gets a person from database in order to have
// her available inside persons buffer
//
void
NSPilotProxyWindow::getPersonSynchro(string sParams)
{
  if ((NULL == _pSuper) || (NULL == _pSuper->getContexte()))
    return ;

  NSPersonArray* pPersonsBuffer = _pSuper->getContexte()->getPersonArray() ;
  if (NULL == pPersonsBuffer)
    return ;

  string   sPersonID = sParams ;
  string   sPidsType = string("") ;
  PIDSTYPE iTypePids = pidsAll ;      // Default is pidsAll

  size_t iSeparPos = sParams.find("|") ;
  if (NPOS != iSeparPos)
  {
    sPersonID = string(sParams, 0, iSeparPos) ;
    sPidsType = string(sParams, iSeparPos + 1, strlen(sParams.c_str()) - iSeparPos - 1) ;
  }

  if (string("") != sPidsType)
  {
    if      (string("pidsPatient") == sPidsType)
      iTypePids = pidsPatient ;
    else if (string("pidsCorresp") == sPidsType)
      iTypePids = pidsCorresp ;
    else if (string("pidsUtilisat") == sPidsType)
      iTypePids = pidsUtilisat ;
    else if (string("pidsPatientGroup") == sPidsType)
      iTypePids = pidsPatientGroup ;
    else if (string("pidsAll") == sPidsType)
      iTypePids = pidsAll ;
  }

  string ps = string("NSPilotProxyWindow::getPersonSynchro asking for person ") + sPersonID ;
  _pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  pPersonsBuffer->getPerson(_pSuper->getContexte(), sPersonID, iTypePids, NSPersonArray::synchronous) ;

  ps = string("NSPilotProxyWindow::getPersonSynchro person ") + sPersonID + string(" received") ;
  _pSuper->trace(&ps, 1, NSSuper::trDetails) ;
}

/****************** classe NSImportChild **************************/
DEFINE_RESPONSE_TABLE1(NSPilotProxyChild, TMDIChild)
END_RESPONSE_TABLE;

NSPilotProxyChild::NSPilotProxyChild(NSSuper* pSuper, TMDIClient& parent, const char far* title, NSPilotProxyWindow* clientWnd)                  :TMDIChild(parent, title, clientWnd), NSSuperRoot(pSuper)
{
	_pClient = clientWnd ;
}

NSPilotProxyChild::~NSPilotProxyChild()
{
}

////////////////////////// end nsPilotProxy.cpp ///////////////////////////////
