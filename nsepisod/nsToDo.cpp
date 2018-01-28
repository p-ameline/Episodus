//----------------------------------------------------------------------------//              Objects used to send commands to core application
//----------------------------------------------------------------------------

#include "nsepisod\nsToDo.h"
#include "nautilus\nautilus.h"
#include "nautilus\nssuper.h"

NSToDoTask::NSToDoTask()
{
  _sWhatToDo = string("") ;
  _sParam1   = string("") ;
  _sParam2   = string("") ;
  _pPointer1 = 0 ;
  _pPointer2 = 0 ;
  _pP1InstanceCounter = (NSInstanceCounter*) 0 ;
  _pP2InstanceCounter = (NSInstanceCounter*) 0 ;
}

void
NSToDoTask::sendMe(NSSuper *pSuper)
{
  if ((NSSuper*) NULL == pSuper)
    return ;

  TMyApp *pNSApplication = pSuper->getApplication() ;
  if ((TMyApp*) NULL == pNSApplication)
    return ;

  while (pSuper->isToDoLocked())
		pNSApplication->PumpWaitingMessages() ;

	pSuper->lockToDo() ;
	pSuper->addToDoTask(this) ;
	pSuper->unlockToDo() ;

  string ps = string("addToDo ") + _sWhatToDo +
  						string(" sP1=") + _sParam1 +
              string(" sP2=") + _sParam2 ;
	pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

/*
	if (bSend && pNSApplication->GetMainWindow())
		pNSApplication->GetMainWindow()->PostMessage(WM_COMMAND, IDM_TODO) ;
*/
}

void
NSToDoTask::sendBB1BBInterface(BB1BBInterfaceForKs* pKsDesc, NSSuper *pSuper, bool bSend, bool bDeleteKsDesc)
{
  if (((NSSuper*) NULL == pSuper) || ((BB1BBInterfaceForKs*) NULL == pKsDesc))
    return ;

  reinit() ;

  _sWhatToDo = "KS_Archetype" ;
  setPointer1(pKsDesc, bDeleteKsDesc) ;

  sendMe(pSuper) ;
}

void
NSToDoTask::sendBB1BBDecisionTree(BB1BBInterfaceForKs* pKsDesc, NSSuper *pSuper, bool bSend, bool bDeleteKsDesc)
{
  if (((NSSuper*) NULL == pSuper) || ((BB1BBInterfaceForKs*) NULL == pKsDesc))
    return ;

  reinit() ;

  _sWhatToDo = "KS_DecisionTree" ;
  setPointer1(pKsDesc, bDeleteKsDesc) ;

  sendMe(pSuper) ;
}

void
NSToDoTask::BbkAskUser(NSSuper *pSuper, string sArchetype, int iTypeArc)
{
  if ((NSSuper*) NULL == pSuper)
    return ;

  reinit() ;

  char szType[10] ;
	itoa(iTypeArc, szType, 10) ;

	_sWhatToDo = string("Archetype") ;
	_sParam1   = sArchetype ;
	_sParam2   = string(szType) ;

	sendMe(pSuper) ;
}

NSToDoTask::NSToDoTask(const NSToDoTask& src)
{
  initFromTask(&src) ;
}

NSToDoTask::~NSToDoTask()
{
  reinitPointer(_pPointer1, _pP1InstanceCounter) ;
  reinitPointer(_pPointer2, _pP2InstanceCounter) ;
}

NSToDoTask&
NSToDoTask::operator=(const NSToDoTask& src)
{
	if (this == &src)
  	return *this ;

  reinit() ;

  initFromTask(&src) ;

  return (*this) ;
}

void
NSToDoTask::initFromTask(const NSToDoTask *pSrc)
{
  _sWhatToDo = pSrc->_sWhatToDo ;
  _sParam1   = pSrc->_sParam1 ;
  _sParam2   = pSrc->_sParam2 ;
  _pPointer1 = pSrc->_pPointer1 ;
  _pPointer2 = pSrc->_pPointer2 ;

  _pP1InstanceCounter = pSrc->_pP1InstanceCounter ;
  if (_pP1InstanceCounter)
    _pP1InstanceCounter->add() ;

  _pP2InstanceCounter = pSrc->_pP2InstanceCounter ;
  if (_pP2InstanceCounter)
    _pP2InstanceCounter->add() ;
}

void
NSToDoTask::reinit()
{
  _sWhatToDo = string("") ;
  _sParam1   = string("") ;
  _sParam2   = string("") ;

  reinitPointer(_pPointer1, _pP1InstanceCounter) ;
  reinitPointer(_pPointer2, _pP2InstanceCounter) ;
}

void
NSToDoTask::setPointer1(void* pPointer, bool bMustDelete)
{
  reinitPointer(_pPointer1, _pP1InstanceCounter) ;

  _pPointer1 = pPointer ;

  if (pPointer && bMustDelete)
    _pP1InstanceCounter = new NSInstanceCounter() ;
}

void
NSToDoTask::setPointer2(void* pPointer, bool bMustDelete)
{
  reinitPointer(_pPointer2, _pP2InstanceCounter) ;

  _pPointer2 = pPointer ;

  if (pPointer && bMustDelete)
    _pP2InstanceCounter = new NSInstanceCounter() ;
}

//
// No counter means don't delete pointer
//
void
NSToDoTask::reinitPointer(void* pPointer, NSInstanceCounter* pCounter)
{
  bool bMustDelete = false ;

  if (pCounter)
  {
    pCounter->remove() ;
    if (pCounter->getCount() == 0)
      bMustDelete = false ;

    delete pCounter ;
    pCounter = (NSInstanceCounter*) 0 ;
  }

  if (bMustDelete && pPointer)
    delete (void *) pPointer ;

  pPointer = (void*) 0 ;
}

NSConcernToDoInformation::NSConcernToDoInformation()
{
  _frame         = ldvframeHealth ;
  _sLexique      = string("") ;
  _sStartingDate = string("") ;
  _sEndingDate   = string("") ;
  _bVerbose      = true ;
  _iSeverity     = 10 ;
}

NSConcernToDoInformation::NSConcernToDoInformation(const NSConcernToDoInformation& src)
{
  _frame         = src._frame ;
  _sLexique      = src._sLexique ;
  _sStartingDate = src._sStartingDate ;
  _sEndingDate   = src._sEndingDate ;
  _bVerbose      = src._bVerbose ;
  _iSeverity     = src._iSeverity ;
}

NSConcernToDoInformation::~NSConcernToDoInformation()
{
}

NSConcernToDoInformation&
NSConcernToDoInformation::operator=(const NSConcernToDoInformation& src)
{
  if (this == &src)
  	return *this ;

  _frame         = src._frame ;
  _sLexique      = src._sLexique ;
  _sStartingDate = src._sStartingDate ;
  _sEndingDate   = src._sEndingDate ;
  _bVerbose      = src._bVerbose ;
  _iSeverity     = src._iSeverity ;

  return *this ;
}

