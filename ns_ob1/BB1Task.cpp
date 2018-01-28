/*
** Copyright Nautilus, (10/9/2004)
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/
#include "ns_ob1\BB1Task.h"
#include "ns_ob1\BB1UserEvent.h"

long NautilusEvent::lObjectCount = 0 ;

NautilusEvent::NautilusEvent(const NSPatPathoArray* root, PatPathoConstIter pointer, EventType userEventType, bool bDuplicatePatho) // FIXME PATHO
              // :BB1Task()
{
  lObjectCount++ ;

	if (bDuplicatePatho)
	{
  	_root = new NSPatPathoArray(*root) ;
    _bMustDeletePatho = true ;

    if (_root->empty() || (NULL == pointer))
  		_pointer = NULL ;
    else
    	synchronizeIterator(root, pointer) ;
  }
  else
  {
  	_bMustDeletePatho = false ;
  	_root    = root ;
  	_pointer = pointer ;
  }

  _userEventType = userEventType ;
}

NautilusEvent::NautilusEvent()
              // :BB1Task()
{
  lObjectCount++ ;

  _root    = (NSPatPathoArray*) 0 ;
  _pointer = NULL ;
  _bMustDeletePatho = false ;
}

NautilusEvent::NautilusEvent(const NautilusEvent& src)
              // :BB1Task(src)
{
  lObjectCount++ ;

	if (src._bMustDeletePatho)
	{
  	if (NULL == src._root)
    	_root = NULL ;
    else
  		_root = new NSPatPathoArray(*(src._root)) ;
    _bMustDeletePatho = true ;

    if (_root->empty() || (NULL == src._pointer))
  		_pointer = NULL ;
    else
    	synchronizeIterator(src._root, src._pointer) ;
  }
  else
  {
		_root    = src._root ;
  	_pointer = src._pointer ;
    _bMustDeletePatho = false ;
  }
  _userEventType = src._userEventType ;

  NVLdVTemps *pSourceTps = (NVLdVTemps*) &(src._informationDate) ;
  _informationDate = *pSourceTps ;
}

NautilusEvent&
NautilusEvent::operator=(const NautilusEvent& src)
{
	if (&src == this)
		return *this ;

	// using BB1Task = operator
  /*
	BB1Task *dest   = this ;
	BB1Task *source = &src ;
	*dest = *source ;
  */

	if (src._bMustDeletePatho)
	{
  	if (_root)
    	delete _root ;

  	if (NULL == src._root)
    	_root = NULL ;
    else
  		_root = new NSPatPathoArray(*(src._root)) ;

    _bMustDeletePatho = true ;

    if (_root->empty() || (NULL == src._pointer))
  		_pointer = NULL ;
    else
    	synchronizeIterator(src._root, src._pointer) ;
	}
  else
  {
  	_root    = src._root ;
  	_pointer = src._pointer ;
  }
  _userEventType = src._userEventType ;
  _informationDate = src._informationDate ;

	return *this ;
}

NautilusEvent::~NautilusEvent()
{
	if (_bMustDeletePatho && _root)
		delete _root ;

  lObjectCount-- ;
}

bool
NautilusEvent::synchronizeIterator(const NSPatPathoArray* pModelPPT, PatPathoConstIter modelPointer)
{
	if ((NULL == pModelPPT) || (NULL == modelPointer))
		return false ;

	_pointer = NULL ;

	if (pModelPPT->empty() || _root->empty())
		return true ;

	if (modelPointer == pModelPPT->end())
	{
  	_pointer = _root->end() ;
    return true ;
	}

	_pointer = _root->begin() ;
	PatPathoConstIter iterSynchro = pModelPPT->begin() ;
  while ((_pointer != _root->end()) &&
             (iterSynchro != pModelPPT->end()) && (*iterSynchro != *modelPointer))
  {
  	_pointer++ ;
    iterSynchro++ ;
	}

	if ((_pointer == _root->end()) || (iterSynchro == pModelPPT->end()))
  {
  	_pointer = NULL ;
		return false ;
  }

	return true ;
}

insertObjectOnBlacBoard::insertObjectOnBlacBoard(NSPatPathoArray* doc, TypedVal* path)     // FIXME PATHO
                        :BB1Task()
{
  _doc  = doc ;
  _path = new TypedVal(*path) ;
}

insertObjectOnBlacBoard::insertObjectOnBlacBoard()
                        :BB1Task()
{
  _doc = (NSPatPathoArray*) 0 ;
}

insertObjectOnBlacBoard::insertObjectOnBlacBoard(const insertObjectOnBlacBoard& src)
                        :BB1Task(src)
{
	_doc = src._doc ;
}

insertObjectOnBlacBoard&
insertObjectOnBlacBoard::operator=(const insertObjectOnBlacBoard& src)
{
	if (&src == this)
		return *this ;

	// using BB1Task = operator
	BB1Task *dest   = this ;
	const BB1Task *source = &src ;
	*dest = *source ;

	_doc = src._doc ;

  return *this ;
}

insertObjectOnBlacBoard::~insertObjectOnBlacBoard()
{
}

AskDeterministicQuestion::AskDeterministicQuestion(std::string label ,TypedVal* question, int pririty)
                         :BB1Task()
{
  _label = label;
  _path = new TypedVal(*question);
  _priority = pririty ;
}

AskDeterministicQuestion::AskDeterministicQuestion()
                         :BB1Task()
{
   _path = NULL;
   _priority = 1;
}

AskDeterministicQuestion::AskDeterministicQuestion(const AskDeterministicQuestion& src)
                         :BB1Task(src)
{
	_priority = src._priority ;
}

AskDeterministicQuestion&
AskDeterministicQuestion::operator=(const AskDeterministicQuestion& src)
{
	if (&src == this)
		return *this ;

	// using BB1Task = operator
	BB1Task *dest   = this ;
	const BB1Task *source = &src ;
	*dest = *source ;

	_priority = src._priority ;

  return *this ;
}

AskDeterministicQuestion::~AskDeterministicQuestion()
{
}

BB1Order::BB1Order()
         :BB1Task()
{
}

BB1Order::BB1Order(BB1ORDER order, std::string tochange, std::string change)
         :BB1Task()
{
	_order     = order ;          // Typed'ordre
	toChange   = tochange ;    // Chose a modifier ddans le fonctionnement
	changement = change ;
}

BB1Order::BB1Order(const BB1Order& src)
         :BB1Task(src)
{
	_order     = src._order ;
	toChange   = src.toChange ;
	changement = src.changement ;
}

BB1Order&
BB1Order::operator=(const BB1Order& src)
{
	if (&src == this)
		return *this ;

	// using BB1Task = operator
	BB1Task *dest   = this ;
	const BB1Task *source = &src ;
	*dest = *source ;

	_order     = src._order ;
	toChange   = src.toChange ;
	changement = src.changement ;

  return *this ;
}

BB1Order::~BB1Order()
{
}

IsAnswerPresentOnBlackBoard::IsAnswerPresentOnBlackBoard()
                            :BB1Task()
{
  _path = NULL;
}

IsAnswerPresentOnBlackBoard::IsAnswerPresentOnBlackBoard(TypedVal* path, bool deepResult)
                            :BB1Task()
{
  _path = new TypedVal(*path);
  _deepResearch = deepResult;
}

IsAnswerPresentOnBlackBoard::IsAnswerPresentOnBlackBoard(const IsAnswerPresentOnBlackBoard& src)
                            :BB1Task(src)
{
	_deepResearch = src._deepResearch ;
}

IsAnswerPresentOnBlackBoard&
IsAnswerPresentOnBlackBoard::operator=(const IsAnswerPresentOnBlackBoard& src)
{
	if (&src == this)
		return *this ;

	// using BB1Task = operator
	BB1Task *dest   = this ;
	const BB1Task *source = &src ;
	*dest = *source ;

	_deepResearch = src._deepResearch ;

  return *this ;
}

IsAnswerPresentOnBlackBoard::~IsAnswerPresentOnBlackBoard()
{
}

TaskStructure::TaskStructure(BB1Task* temp)
{
  _task     = temp ;
  _toDelete = false ;
  _answer   = NULL ;
}

TaskStructure::~TaskStructure()
{
  if (_task)
    deleteTask() ;
  if (_answer)
  	delete(_answer) ;
}

TaskStructure::TaskStructure(const TaskStructure& src)
{
  if (src._task)
		setTask(src._task) ;
  else
  	_task = NULL ;
	_answer   = src._answer ;
	_toDelete = src._toDelete ;
}

TaskStructure&
TaskStructure::operator=(const TaskStructure& src)
{
	if (&src == this)
		return *this ;

  if (_task)
    deleteTask() ;

  if (src._task)
		setTask(src._task) ;
  else
  	_task = NULL ;
	_answer   = src._answer ;
	_toDelete = src._toDelete ;

  return *this ;
}

void
TaskStructure::setTask(BB1Task* pTask)
{
	if (NULL == pTask)
  {
  	_task = (BB1Task*) 0 ;
  	return ;
  }

/*
	NautilusEvent* pEvent = dynamic_cast<NautilusEvent*>(pTask);
  if (pEvent != NULL)
  {
  	_task = new NautilusEvent(*pEvent) ;
    return ;
  }
*/

	IsAnswerPresentOnBlackBoard* pIsAnswer = dynamic_cast<IsAnswerPresentOnBlackBoard*>(pTask);
  if (pIsAnswer)
  {
  	_task = new IsAnswerPresentOnBlackBoard(*pIsAnswer) ;
    return ;
  }

	AskDeterministicQuestion* pAskDeterministic = dynamic_cast<AskDeterministicQuestion*>(pTask);
  if (pAskDeterministic)
  {
  	_task = new AskDeterministicQuestion(*pAskDeterministic) ;
    return ;
  }

	BB1Order* pOrder = dynamic_cast<BB1Order*>(pTask);
  if (pOrder)
  {
  	_task = new BB1Order(*pOrder) ;
    return ;
  }

	insertObjectOnBlacBoard* pInsertObject = dynamic_cast<insertObjectOnBlacBoard*>(pTask);
  if (pInsertObject)
  {
  	_task = new insertObjectOnBlacBoard(*pInsertObject) ;
    return ;
  }

/*
	Precoche* pPrecoche = dynamic_cast<Precoche*>(pTask);
  if (pPrecoche != NULL)
  {
  	_task = new Precoche(*pPrecoche) ;
    return ;
  }
*/

  _task = (BB1Task*) 0 ;
	return ;
}

void
TaskStructure::deleteTask()
{
	if (NULL == _task)
  	return ;

  BB1TaskType iType = _task->Type() ;

  if (NAUTILUS_EVENT_TASK == iType)
  {
		NautilusEvent* pEvent = dynamic_cast<NautilusEvent*>(_task) ;
  	if (pEvent)
  	{
  		delete pEvent ;
  		_task = (BB1Task*) 0 ;
    	return ;
  	}
  }

  if (IS_ANSWER_PRESENT_ON_BLACKBOARD == iType)
  {
		IsAnswerPresentOnBlackBoard* pIsAnswer = dynamic_cast<IsAnswerPresentOnBlackBoard*>(_task) ;
  	if (pIsAnswer)
  	{
  		delete pIsAnswer ;
  		_task = (BB1Task*) 0 ;
    	return ;
  	}
	}

  if (ASK_DETERMINISTIC_QUESTION == iType)
  {
		AskDeterministicQuestion* pAskDeterministic = dynamic_cast<AskDeterministicQuestion*>(_task) ;
  	if (pAskDeterministic)
  	{
  		delete pAskDeterministic ;
  		_task = (BB1Task*) 0 ;
    	return ;
  	}
	}

  if (ORDER == iType)
  {
		BB1Order* pOrder = dynamic_cast<BB1Order*>(_task) ;
  	if (pOrder)
  	{
    	delete pOrder ;
  		_task = (BB1Task*) 0 ;
    	return ;
		}
  }

  if (INSERT_OBJECT_ON_BLACKOARD == iType)
  {
		insertObjectOnBlacBoard* pInsertObject = dynamic_cast<insertObjectOnBlacBoard*>(_task) ;
  	if (pInsertObject)
  	{
  		delete pInsertObject ;
  		_task = (BB1Task*) 0 ;
    	return ;
  	}
	}

  delete _task ;
  _task = (BB1Task*) 0 ;
	return ;
}

BB1TaskList::BB1TaskList()
{
}

BB1TaskList::~BB1TaskList()
{
	vider() ;
}

TaskStructure*
BB1TaskList::getTask(TypedVal* val)
{
	if (NULL == val)
  	return (TaskStructure*) 0 ;

	if (_TaskStructureList.empty())
		return NULL ;

  register unsigned int end = _TaskStructureList.size() ;
  for (register unsigned int i = 0 ; i < end ; i++)
	{
		TaskStructure* temp = _TaskStructureList[i] ;
		if (temp->Task()->Path() == val)
			return (temp) ;
	}

	return (TaskStructure*) 0 ;
}

void
BB1TaskList::AddTask(TaskStructure* temp)
{
	if (NULL == temp)
  	return ;

	_TaskStructureList.push_back(temp) ;
}

/*
void                BB1TaskList::remove(BB1Task* temp)
{
}
   */

void
BB1TaskList::GarbageCollecting()
{
	if (_TaskStructureList.empty())
		return ;

  std::vector<TaskStructure* >::iterator iter ;
  for (iter = _TaskStructureList.begin() ; _TaskStructureList.end() != iter ; )
  {
  	if (NULL == *iter)
    	_TaskStructureList.erase(iter) ;
		else if (true == (*iter)->Delete())
		{
			delete(*iter) ;
			_TaskStructureList.erase(iter) ;
		}
    else
    	iter++ ;
  }
}

BB1TaskList::BB1TaskList(const BB1TaskList& src)
{
	if (src._TaskStructureList.empty())
		return ;

  std::vector<TaskStructure* >::const_iterator iter ;
  for (iter = src._TaskStructureList.begin() ; src._TaskStructureList.end() != iter ; iter++)
  	if (*iter)
  		_TaskStructureList.push_back(new TaskStructure(**iter)) ;
}

BB1TaskList&
BB1TaskList::operator=(const BB1TaskList& src)
{
	if (&src == this)
		return *this ;

  vider() ;

  if (src._TaskStructureList.empty())
		return *this ;

  std::vector<TaskStructure* >::const_iterator iter ;
  for (iter = src._TaskStructureList.begin() ; src._TaskStructureList.end() != iter ; iter++)
  	if (*iter)
  		_TaskStructureList.push_back(new TaskStructure(**iter)) ;

  return *this ;
}

void
BB1TaskList::vider()
{
	if (_TaskStructureList.empty())
		return ;

  std::vector<TaskStructure* >::iterator iter ;
  for (iter = _TaskStructureList.begin() ; _TaskStructureList.end() != iter ; )
  {
  	if (*iter)
  		delete *iter ;
    _TaskStructureList.erase(iter) ;
  }
}

//FIXME
// Precochage
// - handle de fenetre
// - question
// - label
// - void * reponse

// Alert
// - label
// - path
// - priorité de l'alerte
// -

