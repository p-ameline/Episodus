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

#include "partage\ns_timer.h"
#include "partage\ns_search.h"

#include "ns_ob1\Interface.h"
#include "ns_ob1\BB1Task.h"
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\BB1KB.h"
#include "ns_ob1\BB1Object.h"
#include "ns_ob1\BB1Class.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
#else
  #include "nautilus\nssuper.h"
  #include "ns_ob1\InterfaceExt.h"
#endif

#include "ns_ob1\BB1Task.h"
#include "ns_ob1\ns_bbk.h"
#include "ns_ob1\OB1Controler.h"
#include "ns_ob1\OB1Token.h"

#include "nsepisod\nsToDo.h"
#include "nssavoir\nsguide.h"
#include "OB1.rh"

/*
** Tableau d'affichage d'un onglet de linterface
*/
const int TabbedOne[]   = { IDC_LIST_KS } ;       // KSs list       : tab 1
const int TabbedTwo[]   = { IDC_LIST_ACTION } ;   // actions list   : tab 2
const int TabbedThree[] = { IDC_LIST_QUESTION } ; // questions list : tab 3
const int TabbedFour[]  = { IDC_LIST_ANSWER } ;   // answers list   : tab 4
const int TabbedFive[]  = { IDC_LIST_EVENT } ;    // events list    : tab 5

const int ID_BBKList = 0x442 ;

//TMDIChild
DEFINE_RESPONSE_TABLE1(BB1BBInterface, TDialog)
    EV_COMMAND(IDM_BBKTODO, CmBBKEvent),
    EV_WM_TIMER,
    EV_TCN_SELCHANGE(IDC_SELECT_TODO, TabChanged),
    EV_LVN_GETDISPINFO(IDC_LIST_KS, DispInfoKs),
    EV_LVN_GETDISPINFO(IDC_LIST_ACTION, DispInfoAction),
    // EV_LVN_GETDISPINFO(IDC_LIST_QUESTION, DispInfoQuestion),
    EV_BN_CLICKED(IDC_MASK, Mask),
    EV_COMMAND_ON_POINT(IDC_LIST_KS, WM_LBUTTONDBLCLK, KsDblClick),
END_RESPONSE_TABLE ;

BB1BBInterface::BB1BBInterface(TWindow* parent, NSContexte* Contexte, TModule* module )
               :TDialog(parent, "IDD_OB1", module)
{
  _bb                     = (BB1BB *) 0 ;
  _pContexte              = Contexte ;
  _isDeterministe         = false ;
  _parent                 = (OB1InterfaceContainer *) 0 ;

  _ActionsPossible        = new TTabControl(this,IDC_SELECT_TODO) ;
  _KsList					        = new NSKSListWindow(this, IDC_LIST_KS) ;
  _QuestionsList					= new NSQuestionListWindow(this, IDC_LIST_QUESTION) ;
  _AnswersList					  = new NSAnswerListWindow(this, IDC_LIST_ANSWER) ;
  _EventsList				    	= new NSEventListWindow(this, IDC_LIST_EVENT) ;
  _actionsInOB1		        = new TListWindow(this, IDC_LIST_ACTION) ;

  _actionsLogLevel        = trSubDetails ;
  _interfaceTimerInterval = 10 ;
}

BB1BBInterface::~BB1BBInterface()
{
  string sMsg = string("Deleting OB1 Interface") ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

  if (_parent)
    _parent->setInterface((BB1BBInterface*) 0) ;

	KillTimer(ID_RCBBKTIMER) ;

  // On efface les membre de visualisation
  if (_ActionsPossible)
		delete (_ActionsPossible) ;
  if (_KsList)
  	delete (_KsList) ;
  if (_actionsInOB1)
  	delete (_actionsInOB1) ;
  if (_QuestionsList)
  	delete (_QuestionsList) ;
  if (_EventsList)
  	delete (_EventsList) ;

  _aToDoNow.vider() ;
}

void
BB1BBInterface::putBB(BB1BB* temp)
{
  _bb = temp ;
}

/*
** Hide blackboard interface
** Fenetre permettant de masquer ou de faire apparaitre le blackboard
** true : cache l'interface
** false montre l'interface
*/
void
BB1BBInterface::MaskBB(bool hide)
{
	if (NULL == _parent)
  	return ;

	if (hide)
  {
    // _parent->ShowWindow(SW_HIDE) ;
    _parent->SetWindowPos(0, 0, 0, 0, 0, SWP_NOZORDER) ;
  }
  else
  {
    _parent->ShowWindow(SW_SHOW) ;
    RefreshObjectsList() ;
  }
}

void
BB1BBInterface::Mask()
{
	MaskBB(true) ;
}

void
BB1BBInterface::SetupWindow()
{
	//TMDIChild::SetupWindow();
	TDialog::SetupWindow() ;
	InitListView() ;
  InitListViewForQuestions() ;
  InitListViewForAnswers() ;
  InitListViewForEvents() ;
  RefreshObjectsList() ;
	InitTabbedControl() ; // Initialisation du TabbedControl
	PrintAgentList() ;
  SetTimer(ID_RCBBKTIMER, _interfaceTimerInterval) ;
}

void
BB1BBInterface::ChangeTabbedControl(int index)
{
  register unsigned int i;
  for (i = 0; i < 1; i++) EnableControl(TabbedOne[i],   (0 == index) ) ;
  for (i = 0; i < 1; i++) EnableControl(TabbedTwo[i],   (1 == index) ) ;
  for (i = 0; i < 1; i++) EnableControl(TabbedThree[i], (2 == index) ) ;
  for (i = 0; i < 1; i++) EnableControl(TabbedFour[i],  (3 == index) ) ;
  for (i = 0; i < 1; i++) EnableControl(TabbedFive[i],  (4 == index) ) ;
}

void
BB1BBInterface::CmBBKEvent()
{
  getMessage() ;
}

void
BB1BBInterface::InitTabbedControl()
{
	char temp[] = "KS" ;
	TTabItem tab(temp) ;
	_ActionsPossible->Add(tab) ;

	char tem2[] = "Actions" ;
	TTabItem tab2(tem2) ;
	_ActionsPossible->Add(tab2) ;

  char tem3[] = "Questions" ;
	TTabItem tab3(tem3) ;
	_ActionsPossible->Add(tab3) ;

  char tem4[] = "Answers" ;
	TTabItem tab4(tem4) ;
	_ActionsPossible->Add(tab4) ;

  char tem5[] = "Events" ;
	TTabItem tab5(tem5) ;
	_ActionsPossible->Add(tab5) ;

  _ActionsPossible->SetSel(0) ;
  ChangeTabbedControl(0) ;
}

void
BB1BBInterface::InitListView()
{
	_KsList->InsertColumn(0,	TListWindColumn("Nom", 			 	100,	TListWindColumn::Left,   	0)) ;
	_KsList->InsertColumn(1, 	TListWindColumn("Type",			 		 50, 	TListWindColumn::Right,		1)) ;

  _actionsInOB1->InsertColumn(0, TListWindColumn("Action", 500, TListWindColumn::Left, 0)) ;
	//_actionsInOB1->InsertColumn(1, 	TListWindColumn("Type",			 		 50, 	TListWindColumn::Right,		1)) ;
}

void
BB1BBInterface::InitListViewForQuestions()
{
	_QuestionsList->InsertColumn(0, TListWindColumn("Name",   100, TListWindColumn::Left,  0)) ;
	_QuestionsList->InsertColumn(1, TListWindColumn("Path",   200, TListWindColumn::Right, 1)) ;
  _QuestionsList->InsertColumn(2, TListWindColumn("Status", 100, TListWindColumn::Right, 2)) ;
  _QuestionsList->InsertColumn(3, TListWindColumn("Stage",   80, TListWindColumn::Right, 3)) ;
}

void
BB1BBInterface::InitListViewForAnswers()
{
	_AnswersList->InsertColumn(0, TListWindColumn("Name",   100, TListWindColumn::Left,  0)) ;
	_AnswersList->InsertColumn(1, TListWindColumn("Path",   200, TListWindColumn::Right, 1)) ;
  _AnswersList->InsertColumn(2, TListWindColumn("Status", 100, TListWindColumn::Right, 2)) ;
  _AnswersList->InsertColumn(3, TListWindColumn("Stage",   80, TListWindColumn::Right, 3)) ;
}

void
BB1BBInterface::InitListViewForEvents()
{
	_EventsList->InsertColumn(0, TListWindColumn("Name",   100, TListWindColumn::Left,  0)) ;
	_EventsList->InsertColumn(1, TListWindColumn("Path",   200, TListWindColumn::Right, 1)) ;
  _EventsList->InsertColumn(2, TListWindColumn("Status", 100, TListWindColumn::Right, 2)) ;
  _EventsList->InsertColumn(3, TListWindColumn("Stage",   80, TListWindColumn::Right, 3)) ;
}

void
BB1BBInterface::RefreshObjectsList()
{
  if ((false == IsWindow()) || (false == IsWindowVisible()))
    return ;

  WINDOWPLACEMENT placement ;
  if ((GetWindowPlacement(&placement)) && (SW_HIDE	!= placement.showCmd))
  {
    RefreshQuestionsList() ;
    RefreshAnswersList() ;
    RefreshEventsList() ;
  }
}

void
BB1BBInterface::RefreshQuestionsList()
{
  if (_QuestionsList->GetItemCount() > 0)
    _QuestionsList->DeleteAllItems() ;

  if (NULL == _bb)
		return ;

  BB1KB	*EventKB = _bb->KBNamed(std::string("QuestionKB")) ;
	if (NULL == EventKB)
    return ;

  ObjectSet *pObjects = (ObjectSet*) EventKB->ObjectsInKB() ;
  if ((NULL == pObjects) || pObjects->empty())
    return ;

  int iLineIndex = 0 ;

  for (ObjectIter i = pObjects->begin() ; pObjects->end() != i ; i++)
  {
    string sQuestionName = (*i)->Name() ;
    TListWindItem	baseItem(sQuestionName.c_str(), 0) ;
    _QuestionsList->InsertItem(baseItem) ;

    string sQuestionPath = (*i)->getQuestionPath() ;
    TListWindItem Item(sQuestionPath.c_str(), 1) ;
    Item.SetIndex(iLineIndex) ;
    Item.SetSubItem(1) ;
    _QuestionsList->SetItem(Item) ;

    string sStatus = string("?") ;
    string sStage  = string("?") ;

    BB1Object* pAnswer = _bb->searchAnswerForQuestion(*i) ;

    if (pAnswer)
    {
      sStatus = pAnswer->getAnswerStatusAsString() ;
      sStage  = pAnswer->getAnswerProcessStageAsString() ;
    }

    TListWindItem ItemStatus(sStatus.c_str(), 2) ;
    ItemStatus.SetIndex(iLineIndex) ;
    ItemStatus.SetSubItem(2) ;
    _QuestionsList->SetItem(ItemStatus) ;

    TListWindItem ItemStage(sStage.c_str(), 3) ;
    ItemStage.SetIndex(iLineIndex) ;
    ItemStage.SetSubItem(3) ;
    _QuestionsList->SetItem(ItemStage) ;
  }
}

void
BB1BBInterface::RefreshAnswersList()
{
  if (_AnswersList->GetItemCount() > 0)
    _AnswersList->DeleteAllItems() ;

  if (NULL == _bb)
		return ;

  BB1KB	*AnswerKB = _bb->KBNamed(std::string("InformationKB")) ;
	if (NULL == AnswerKB)
    return ;

  ObjectSet *pObjects = (ObjectSet*) AnswerKB->ObjectsInKB() ;
  if ((NULL == pObjects) || pObjects->empty())
    return ;

  int iLineIndex = 0 ;

  for (ObjectIter i = pObjects->begin() ; pObjects->end() != i ; i++)
  {
    string sName = (*i)->Name() ;
    TListWindItem	baseItem(sName.c_str(), 0) ;
    _AnswersList->InsertItem(baseItem) ;

    string sPath = string("") ;
    TypedVal* pTV = (*i)->PathAttributes() ;
    if (pTV)
      sPath = pTV->getString() ;

    TListWindItem Item(sPath.c_str(), 1) ;
    Item.SetIndex(iLineIndex) ;
    Item.SetSubItem(1) ;
    _AnswersList->SetItem(Item) ;

    string sStatus = (*i)->getAnswerStatusAsString() ;
    string sStage  = (*i)->getAnswerProcessStageAsString() ;

    TListWindItem ItemStatus(sStatus.c_str(), 2) ;
    ItemStatus.SetIndex(iLineIndex) ;
    ItemStatus.SetSubItem(2) ;
    _AnswersList->SetItem(ItemStatus) ;

    TListWindItem ItemStage(sStage.c_str(), 3) ;
    ItemStage.SetIndex(iLineIndex) ;
    ItemStage.SetSubItem(3) ;
    _AnswersList->SetItem(ItemStage) ;
  }
}

void
BB1BBInterface::RefreshEventsList()
{
  if (_EventsList->GetItemCount() > 0)
    _EventsList->DeleteAllItems() ;

  if (NULL == _bb)
		return ;

  BB1KB	*EventKB = _bb->KBNamed(std::string("NautilusEventKB")) ;
	if (NULL == EventKB)
    return ;

  ObjectSet *pObjects = (ObjectSet*) EventKB->ObjectsInKB() ;
  if ((NULL == pObjects) || pObjects->empty())
    return ;

  int iLineIndex = 0 ;

  for (ObjectIter i = pObjects->begin() ; pObjects->end() != i ; i++)
  {
    string sName = (*i)->Name() ;
    TListWindItem	baseItem(sName.c_str(), 0) ;
    _EventsList->InsertItem(baseItem) ;

    string sPath = string("") ;
    TypedVal* pTV = (*i)->PathAttributes() ;
    if (pTV)
      sPath = pTV->getString() ;

    TListWindItem Item(sPath.c_str(), 1) ;
    Item.SetIndex(iLineIndex) ;
    Item.SetSubItem(1) ;
    _EventsList->SetItem(Item) ;

    string sStatus = (*i)->getAnswerStatusAsString() ;
    string sStage  = (*i)->getAnswerProcessStageAsString() ;

    TListWindItem ItemStatus(sStatus.c_str(), 2) ;
    ItemStatus.SetIndex(iLineIndex) ;
    ItemStatus.SetSubItem(2) ;
    _EventsList->SetItem(ItemStatus) ;

    TListWindItem ItemStage(sStage.c_str(), 3) ;
    ItemStage.SetIndex(iLineIndex) ;
    ItemStage.SetSubItem(3) ;
    _EventsList->SetItem(ItemStage) ;
  }
}

void
BB1BBInterface::TabChanged(TNotify far& /* nm */)
{
  int index = _ActionsPossible->GetSel() ;
  ChangeTabbedControl(index) ;
}

void
BB1BBInterface::EvTimer(uint id)
{
	if (ID_RCBBKTIMER == id)
	{
		if (false == _isDeterministe)
    {
			computeMessage() ;
      if (_bb)
			  _bb->Execute() ;
		}
	}
}

/*
** Get the message in NSuper
** and token it
**
*/
void
BB1BBInterface::getMessage()
{
	if (NULL == _pContexte)
		return ;

	NSSuper* pSuper = _pContexte->getSuperviseur() ;
	if (NULL == pSuper)
		return ;

	if (_pContexte->getBBKToDo()->empty())
		return ;

	NSTaskArray aLocalBBKToDo = *(_pContexte->getBBKToDo()) ;
	_pContexte->getBBKToDo()->vider() ;

	for (TaskIter iTD = aLocalBBKToDo.begin() ; aLocalBBKToDo.end() != iTD ; )
	{
		_aToDoNow.push_back(*iTD) ;
		//   delete *iTD;
		aLocalBBKToDo.erase(iTD);
	}
}

void
BB1BBInterface::computeMessage()
{
	if (_aToDoNow.empty())
		return ;

  NSSuper* pSuper = _pContexte->getSuperviseur() ;
  std::string ps = std::string("") ;

	NSTaskArray aLocalBBKToDo = _aToDoNow ;
	_aToDoNow.vider() ;

  for (TaskIter iTD = aLocalBBKToDo.begin() ; aLocalBBKToDo.end() != iTD ; )
	{
		ps = string("début Todo : ") + (*iTD)->getWhatToDo() ;
    pSuper->trace(&ps, 1) ;

    if (string("NautilusEvent") == (*iTD)->getWhatToDo())
    {
    	NautilusEvent* temp = (static_cast<NautilusEvent*>((*iTD)->getPointer1())) ;
      if (temp)
      	addNautilusEvent(temp);
    }

    if (string("IsAnswerPresentOnBlackBoard") == (*iTD)->getWhatToDo())
    {
    	IsAnswerPresentOnBlackBoard* temp = (static_cast<IsAnswerPresentOnBlackBoard*>((*iTD)->getPointer1())) ;
      if (temp)
      	askIsAnswerPresentOnBlackBoard(temp) ;
    }

    if (string("AskDeterministicQuestion") == (*iTD)->getWhatToDo())
    {
    	AskDeterministicQuestion* temp = (static_cast<AskDeterministicQuestion*> ((*iTD)->getPointer1())) ;
      if (temp)
      {
      	OB1Strategy* pResult = addAskDeterministicQuestion(temp) ;
        if (pResult) // The order is possible and i wait in the response list
        	_AnswerList.AddTask(new TaskStructure((BB1Task*) temp)) ;
      }
    }

    if (string("BB1Order") == (*iTD)->getWhatToDo())
    {
    	BB1Order* temp = (static_cast<BB1Order*> ((*iTD)->getPointer1())) ;
      if (temp)
      	addBB1Order(temp) ;
    }

    if (string("insertObjectOnBlacBoard") == (*iTD)->getWhatToDo())
    {
    	insertObjectOnBlacBoard* temp = (static_cast<insertObjectOnBlacBoard*> ((*iTD)->getPointer1())) ;
      if (temp)
      {
      }
    }

    delete *iTD ;
    aLocalBBKToDo.erase(iTD) ;
  }
}

void
BB1BBInterface::DispInfoKs(TLwDispInfoNotify& dispInfo)
{
 	if ((NULL == _bb) || (NULL == _bb->Controler()))
		return ;

	std::vector<OB1NKS* >* kss = _bb->Controler()->Graph().getKSs() ;
	TListWindItem   &dispInfoItem = *(TListWindItem *) &dispInfo.item ;
	static char     buffer[100] ;

	int index = dispInfoItem.GetIndex() ;
	if ((index < 0) || ((unsigned int) index >= kss->size()))
  	return ;

  KSType type = (*kss)[index]->KsType() ;
  std::string name = (*kss)[index]->getKSName() ;

	switch (dispInfoItem.GetSubItem())
	{
		case 0  :
    	// std::string name = (*kss)[index]->getKSName() ;
      sprintf(buffer, "%s", name.c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;

    case 1	:
    	std::string types = "" ;
      // KSType type = (*kss)[index]->KsType() ;
      switch(type)
      {
      	case USER_KS :
        	types = "user ks" ;
          break ;
        case KS_SERVICE :
        	types = "service ks" ;
          break ;
        case SVCE_WINDOWS_KS :
        	types = "service MMI ks" ;
          break ;
        case USER_WINDOWS_KS :
        	types = "user MMI ks" ;
          break ;
        case USER_PREDI_KS :
        	types = "PreDi ks" ;
          break ;
        case USER_PROJECT_KS :
        	types = "Project ks" ;
          break ;
        case USER_PROJ_FU_KS :
        	types = "Follow-up project ks" ;
          break ;
        default :
        	types = "unknow type" ;
          break ;
      }
      sprintf(buffer, "%s", types.c_str()) ;
      dispInfoItem.SetText(buffer) ;
    	break ;
  }
}

void
BB1BBInterface::DispInfoAction(TLwDispInfoNotify& /* dispInfo */)
{
}

void
BB1BBInterface::KsDblClick(uint /* modKeys */, ClassLib::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  /* int indexItem = */ _KsList->HitTest(info) ;

	if (!(info.GetFlags() & LVHT_ONITEM))  	return ;
}

bool
BB1BBInterface::addNautilusEvent(NautilusEvent* ev)
{
	if (NULL == ev)
		return false ;

  _pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps, "Ajout d'un evenement") ;
  _pContexte->getSuperviseur()->DebugNewLine() ;

  const NSPatPathoArray* pPatPatho = ev->getRoot() ;
	PatPathoConstIter      PPTiter   = ev->getPointer() ;

	if (NULL == pPatPatho)
  	return false ;

  string sEventPath = string("") ;
  if (PPTiter)
	{
  	sEventPath = pPatPatho->donneCheminItem(PPTiter) ;
    if (string("") != sEventPath)
    {
    	string sStandardPath = sEventPath ;
  		_pContexte->getDico()->donneCodeSens(&sStandardPath, &sEventPath) ;
    }
	}

  AttValPair label			  ("label",         std::string("NautilusEvent")) ;
  // AttValPair node				  ("node",		      pPatPatho) ;
  // AttValPair whereIChange	("change",	      pPPTiter) ;
  AttValPair nsEvent	    ("event",         new NautilusEvent(*ev)) ;
  AttValPair expl         ("explication",   sEventPath) ;
  AttValPair userEventType("userEventType",	ev->getUserEvenType()) ;
  AttValPair Naut("control", string("NautEvent"));

  BB1KB*    EventKB    = _bb->KBNamed("NautilusEventKB") ;
  BB1Class* EventClass = _bb->ClassNamed("UserEvent") ;

  string sName = "NautilusEvent" ;
	char num[18] ;
	itoa(_bb->getNBObject(), num, 10) ;
	sName.append(num) ;

 	// BB1Object* creEv = EventClass->MakeInstance(sName, Naut, *EventKB, Collect(&label, &node, &whereIChange, &expl, &userEventType), NULL, false) ;
  AVPSet* pAttSpec = Collect(&label, &nsEvent, &expl, &userEventType) ;
  BB1Object* creEv = EventClass->MakeInstance(sName, Naut, *EventKB, pAttSpec, (LinkSet*) 0, false) ;
  delete pAttSpec ;

  creEv->setAnswerStatus(AnswerStatus::astatusProcessing, (HWND) 0) ;
  creEv->setAnswerProcessStage(AnswerStatus::apstageStarting) ;

  string sActionString = string("add NautilusEvent ") + creEv->Name() ;
	addActionString(sActionString, BB1BBInterface::trSteps) ;

  return _bb->Controler()->NextComputeAction(creEv, NULL) ;
}

bool
BB1BBInterface::addNautilusEvent(string sEventPath)
{
	if ((string("") == sEventPath) || (NULL == _bb))
		return false ;

  _pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps, "Ajout d'un evenement");
  _pContexte->getSuperviseur()->DebugNewLine();

  AttValPair label("label",       std::string("NautilusEvent")) ;
  AttValPair expl ("explication", sEventPath) ;
  AttValPair Naut ("control",     string("NautEvent")) ;

  BB1KB*    EventKB    = _bb->KBNamed("NautilusEventKB") ;
  BB1Class* EventClass = _bb->ClassNamed("UserEvent") ;

	string sName = "NautilusEvent" ;
	char num[18] ;
	itoa(_bb->getNBObject(), num, 10) ;
	sName.append(num) ;

  AVPSet* pAttSpec = Collect(&label, &expl) ;
 	BB1Object* creEv = EventClass->MakeInstance(sName, Naut, *EventKB, pAttSpec, (LinkSet*) 0, false) ;
  delete pAttSpec ;

  creEv->setAnswerStatus(AnswerStatus::astatusProcessing, (HWND) 0) ;
  creEv->setAnswerProcessStage(AnswerStatus::apstageStarting) ;

  string sActionString = string("add NautilusEvent ") + creEv->Name() ;
	addActionString(sActionString, BB1BBInterface::trSteps) ;

	return _bb->Controler()->NextComputeAction(creEv, NULL) ;
}

void
BB1BBInterface::askForWindowUpdate(HWND hTargetWindow)
{
  if (!hTargetWindow)
    return ;

  char szHandle[20] ;
  sprintf(szHandle, "%p", hTargetWindow) ;
  string sActionLog = string("BBK asking update for window ") + string(szHandle) ;

  addActionString(sActionLog, BB1BBInterface::trSteps) ;
  _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trSubDetails) ;

  ::PostMessage(hTargetWindow, WM_COMMAND, IDM_BBK_UPDATE, 0) ;

  _pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
}

BB1Object*
BB1BBInterface::askIsAnswerPresentOnBlackBoard(IsAnswerPresentOnBlackBoard* /* isp */)
{
	// return  (bb->find(isp->Path(), isp->getDepthSearch()));
	return NULL ; //FIXME
}

OB1Strategy*
BB1BBInterface::addAskDeterministicQuestion(AskDeterministicQuestion* aq)
{
	if ((NULL == aq) || (NULL == _bb))
		return NULL ;

	_pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps, "On pose une question à OB1") ;
	_pContexte->getSuperviseur()->DebugNewLine() ;

  string sActionLog = string("Ask deterministic question: ") +
                       string("label=\"") + aq->Label() + string("\"") +
                       string(" path=\"") + aq->Path()->getString() + string("\"") ;
  addActionString(sActionLog, BB1BBInterface::trSteps) ;

  //
  // First step : is there already a BB1Object with the same path, and whose
  // type is "Question"
  //

	BB1Class *question   = _bb->ClassNamed("Question") ;
	BB1KB	   *questionKB = _bb->KBNamed("QuestionKB") ;

  string sName = "Question" ;
	char num[18] ;
	itoa(_bb->getNBObject(), num, 10);
	sName.append(num);

	AttValPair 	labeldef   ("label",     aq->Label()) ;
	AttValPair  path("question",  *aq->Path()) ;     //fixme
	AttValPair  Explication("explication", *(aq->Path()));    //fixme

	TypedVal toFind(*aq->Path()) ;
	OB1Token* exist = _bb->Controler()->find(toFind, std::string("Question")) ;

	if ((exist) && _bb->Controler() && _bb->Controler()->isValidToken(exist))
	{
  	//
    // Question already exists on BB1
    //
  	sActionLog = string("Question already there (token ") + IntToString(exist->getNumero()) + string(")") ;

		exist->getObject()->Modify(NULL, NULL, NULL, NULL, TRUE, false) ;
		exist->incNumero(); // Incremente le conmpteur pour indiquer au controler qu'il y a une nouvelle information differente

    sActionLog += string(" becomes token ") + IntToString(exist->getNumero()) + string(")") ;

  	addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
	}
	else
	{
    //
    // Question doesn't already exists on BB1, we must create it
    //
  	sActionLog = string("New question") ;

    AVPSet* pAttSpec = Collect(&labeldef, &path) ;
		BB1Object* cre = question->MakeInstance(sName, Explication, *questionKB, pAttSpec, (LinkSet*) 0, false) ;
    delete pAttSpec ;

		if ((cre) && _bb->Controler())
    {
			exist = _bb->Controler()->createToken(ADD, cre) ;
      sActionLog += string(" ") + cre->Name() ;
      sActionLog += string(" (creates token ") + IntToString(exist->getNumero()) + string(")") ;
      addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
    }
    else
    {
    	sActionLog += string(" (creation failed)") ;
      addActionString(sActionLog, BB1BBInterface::trError) ;
    }
	}

  if (exist)
    return (_bb->Controler()->AskDeterminicOrder(exist, aq->Priority())) ;
  else
    return NULL ;

  //return (bb->AskDeterministicQuestion(aq->getLabel(), aq->getPath() , aq->getPriority()) ) ;
}

bool
BB1BBInterface::addBB1Order(BB1Order* bb1Temp)
{
  if (NULL == bb1Temp)
    return false ;

  switch(bb1Temp->getOrder())
  {
    case BB1CLOSE:
    	break ;
    case BB1CHANGE:
    	break ;
    default:
    	break ;
  }
  return false ;
}

/*
void
BB1BBInterface::EvSize(uint sizeType, ClassLib::TSize& size)
{
    TWindow::EvSize(sizeType, size);
    EventList->MoveWindow(GetClientRect(), true);
}   */

bool
BB1BBInterface::addinsertObjectOnBlacBoard(insertObjectOnBlacBoard* /* temp */)
{
  return (true);
}

bool LeafDefaultComputation(TypedVal& daf, TypedVal* pSearchParams, HWND interfaceHandle, BB1BBInterface* interfa)
{
	if (NULL == interfa)
  	return false ;

  std::string sDaf = daf.getString() ;
  if (sDaf == std::string(""))
		return false ;

  NSSearchStruct *pSearchStruct = (NSSearchStruct *) 0 ;
  if (pSearchParams)
    pSearchStruct = pSearchParams->getSearchPrm() ;

  std::string temp = ParseNautilusPath(sDaf) ;
  PathsList que ;
  que.push_back(new std::string(temp)) ;

  string sAnswerDate = string("") ;

  string sActionString = string("LeafDefaultComputation, start SearchInPatientFolder for ") + sDaf ;
	interfa->addActionString(sActionString, BB1BBInterface::trSubSteps) ;

  NSPatPathoArray* respat = interfa->SearchInPatientFolder(&que, &sAnswerDate, pSearchStruct) ;

  // Nothing found in patient's record - we try alternative paths
  //
  if (NULL == respat)
  {
    PathsList alternativePaths ;
    getAlternativePath(temp, &alternativePaths) ;

    if (alternativePaths.empty())
    {
      sActionString = string("LeafDefaultComputation, nothing found in record for ") + sDaf + string(" and no alternative path ; leaving") ;
      interfa->addActionString(sActionString, BB1BBInterface::trSubSteps) ;
      return false ;
    }

    sActionString = string("LeafDefaultComputation, nothing found in record for ") + sDaf + string(" Trying alternative paths") ;
    interfa->addActionString(sActionString, BB1BBInterface::trSubSteps) ;

    for (PathsIterator pathsIter = alternativePaths.begin() ; alternativePaths.end() != pathsIter ; pathsIter++)
	  {
      PathsList alternateQue ;
      alternateQue.push_back(new std::string(**pathsIter)) ;

      sAnswerDate = string("") ;

      respat = interfa->SearchInPatientFolder(&alternateQue, &sAnswerDate, pSearchStruct) ;
      if (respat)
        break ;
    }
  }

  // Nothing found in patient's record - we leave
  //
  if (NULL == respat)
  {
    sActionString = string("LeafDefaultComputation, nothing found in record for ") + sDaf + string(" Leaving") ;
    interfa->addActionString(sActionString, BB1BBInterface::trSubSteps) ;
  	return false ;
  }

  sActionString = string("LeafDefaultComputation, information found in record for ") + sDaf ;
  interfa->addActionString(sActionString, BB1BBInterface::trSubSteps) ;

  // if (!respat)  DON'T DO THAT BECAUSE THE TOKEN WOULD NOT BE CREATED
  //	return ;     FOR THE NODE AND THE FUNCTION isActivable() WOULD FAIL
  //               ON THE KS NODE

	// Creation de la reponse

  if (NULL == interfa->getBB())
  {
    sActionString = string("LeafDefaultComputation for ") + sDaf + string(" Cannot find BB ; leaving") ;
    interfa->addActionString(sActionString, BB1BBInterface::trSubSteps) ;
  	return false ;
  }

  BB1Object* res = interfa->getBB()->find(daf, std::string("Answer"), pSearchParams, true) ;
  if (NULL == res)
  {
		BB1KB			 *informationKB = interfa->getBB()->KBNamed("InformationKB") ;
		BB1Class	 *answer        = interfa->getBB()->ClassNamed("Answer") ;

		AttValPair label		("label", string("Default calcul")) ;
		AttValPair sfrom		("sfrom", string("Default Control Calculus")) ;
		AttValPair Explication("explication", sDaf);
		//
		// true : because we don't duplicate respat, so we must not delete respat
		//
		AttValPair node    ("node", respat, true) ;
    AttValPair dateNode("node_date", sAnswerDate) ;

		std::string sId = "default" ;
		char nb[10] ;
		itoa(interfa->getBB()->getNBObject(), nb, 10) ;
		sId.append(nb) ;

		// création de la réponse
    //
    AVPSet* pAttSpec = Collect(&label, &node, &dateNode, &sfrom) ;
		/* BB1AppInst *objectAnswer = */ answer->MakeInstance(sId,
                                                          Explication,
                                                          *informationKB,
                                                          pAttSpec,
                                                          (LinkSet*) 0,
                                                          true) ;
    delete pAttSpec ;

		res = interfa->getBB()->find(daf, std::string("Answer"), pSearchParams) ;
	}
  else
  {
  	res->updatePatPatho(respat, sAnswerDate) ;
    delete respat ;
  }

  if (res)
  	res->setAnswerStatus(AnswerStatus::astatusProcessed, interfaceHandle) ;

  sActionString = string("LeafDefaultComputation for ") + sDaf + string(" Leaving") ;
  interfa->addActionString(sActionString, BB1BBInterface::trSubSteps) ;

  return true ;
}

void getAlternativePath(string sPath, PathsList* pPathList)
{
  if ((string("") == sPath) || (NULL == pPathList))
    return ;

  // if 2DA01/£D0 we can look for the more accurate 2DA02/£T0
  //
  checkAlternate(sPath, pPathList, string("2DA01.£D0"), string("2DA02.£T0")) ;
}

void checkAlternate(string sPath, PathsList* pPathList, string sModel, string sAlternate)
{
  if ((string("") == sPath) || (NULL == pPathList))
    return ;

  // First find in path
  //
  size_t iPos = sPath.find(sModel) ;
  if (string::npos != iPos)
  {
    string sAltern = sPath ;
    sAltern.replace(iPos, strlen(sModel.c_str()), sAlternate) ;
    if (false == isInPath(sAltern, pPathList))
      pPathList->push_back(new std::string(sAltern)) ;
  }

  // Then check inside PathList
  //
  if (pPathList->empty())
    return ;

  for (PathsIterator pathsIter = pPathList->begin() ; pPathList->end() != pathsIter ; pathsIter++)
  {
    iPos = (*pathsIter)->find(sModel) ;
    if (string::npos != iPos)
    {
      string sAltern = **pathsIter ;
      sAltern.replace(iPos, strlen(sModel.c_str()), sAlternate) ;
      if (false == isInPath(sAltern, pPathList))
        pPathList->push_back(new std::string(sAltern)) ;
    }
  }
}

bool isInPath(string sPath, PathsList* pPathList)
{
  if ((string("") == sPath) || (NULL == pPathList) || pPathList->empty())
    return false ;

  for (PathsIterator pathsIter = pPathList->begin() ; pPathList->end() != pathsIter ; pathsIter++)
    if (sPath == **pathsIter)
      return true ;

  return false ;
}

void
BB1BBInterface::PrintAgentList()
{
	if ((NULL == _bb) || (NULL == _bb->Controler()))
		return ;

	std::vector<OB1NKS*>* kss = _bb->Controler()->Graph().getKSs() ;
  if ((NULL == kss) || (kss->empty()))
		return ;

	for (std::vector<OB1NKS* >::reverse_iterator rIter = kss->rbegin() ; kss->rend() != rIter ; rIter++)
	{
		std::string temp = (*rIter)->getKSName();
		TListWindItem	item(temp.c_str(), 0) ;
		_KsList->InsertItem(item) ;
	}
}

bool
BB1BBInterface::insertAnswerOnBlackboard(string sPath, NSPatPathoArray * pAnswer, NautilusObjectType type, HWND interfaceHandle, NSSearchStruct *pSearchStruct)
{
  sPath = getRegularPath(sPath, cheminSeparationMARK, intranodeSeparationMARK) ;

  _pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps,"Insère une réponse sur OB1 : %s",  sPath.c_str()) ;
  _pContexte->getSuperviseur()->DebugNewLine() ;

  string sActionLog = string("Insertion of an answer on BB ") +
                       string("(path=\"") + sPath + string("\"") ;

  AttValPair node("node", pAnswer) ;      // FIXME MERCREDI
  string dispatch ;
  TypedVal sear(sPath) ;

  string id = string("") ;
  switch (type)
  {
    case Preocupation : id = "Preocupation" ; dispatch = id ;     break ;
    case Drug         : id = "Drug" ;         dispatch = id ;     break ;
    case Goal         : id = "Goal" ;         dispatch = id ;     break ;
    default           : id = sPath ;          dispatch = sPath ;  break ;
  }

  sActionLog += string(" type=\"") + dispatch + string("\"") ;

  int temp = pAnswer->Taille() ;
  _pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps,"Taille patho : %d",  temp) ;
  _pContexte->getSuperviseur()->DebugNewLine() ;

  sActionLog += string(" pathoSize=\"") + IntToString(temp) + string("\"") ;
  addActionString(sActionLog, BB1BBInterface::trSteps) ;

  // Put the id
  char nb[10] ;
  itoa(getBB()->getNBObject(), nb, 10) ;
  id.append(nb) ;

  if (false == _isDeterministe)
  {
    // Modif 24/02/2011 : For ordinary cases, Answer is only given to already
    //                    asked Question
    //
    // Bad idea because the Archetype can create answers for questions that
    // will be later asked by the KS
    //
/*
    if (Undefined == type)
    {
      BB1Object* pQuestion = bb->searchQuestionInKB(sear) ;
      if (NULL == pQuestion)
        return true ;
    }
*/

    TypedVal *pSearchParam = (TypedVal *) 0 ;
    TypedVal searchParam(pSearchStruct) ;
    if (pSearchStruct)
      pSearchParam = &searchParam ;

    OB1Token * exist ;
    // BB1Object * res = bb->find(sear, string("Answer")) ;

    sActionLog = string("Deterministe mode: Searching an Answer for path ") + sPath ;
    addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

    BB1Object* res = _bb->searchAnswerInKB(sear, pSearchParam) ;
    if (res)
    {
    	// ... so we can get its answer status
  		//
  		AnswerStatus* pAnswerStatus = res->Value("answerStatus", NULLLANSWERSTATUS) ;
  		if (NULL == pAnswerStatus)
  			return false ;

      // To be modified
      //
    	sActionLog = string("Corresponding object exists") ;

      exist = _bb->Controler()->find(sear, string("Answer")) ;
      if ((exist) && _bb->Controler()->isValidToken(exist))
      {
      	sActionLog += string(" (token ") + IntToString(exist->getNumero()) ;
      	exist->incNumero() ;
        sActionLog += string(" becomes token ") + IntToString(exist->getNumero()) + string(")") ;
      }
      else
      	sActionLog += string(" (no token found)") ;

      addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

      // Modif 24/02/2011 : For ordinary cases, no token is started but
      //                    registered objects are warned
      //
      AVPSet* pAttSpecNode = Collect(&node) ;

      if (Undefined == type)
      {
        res->Modify(pAttSpecNode, (LinkSet*) 0, (AVPSet*) 0, (LinkSet*) 0, FALSE, false) ;
        _bb->alertClientsForAnswerUpdate(&sear, interfaceHandle) ;
      }
      else
        res->Modify(pAttSpecNode) ;

      delete pAttSpecNode ;
    }
    else
    {
    	sActionLog = string("No object found, creating the corresponding object") ;
      addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

      BB1Class*   answer        = _bb->ClassNamed("Answer") ;
      BB1KB*      informationKB = _bb->KBNamed("InformationKB") ;

      AVPSet* pAttSpec = Collect(&node) ;
      BB1AppInst* answ = answer->MakeInstance(id, AttValPair("explication", dispatch), *informationKB, pAttSpec, (LinkSet*) 0, false) ; // don't create a token
      delete pAttSpec ;

      if (answ)
      {
        sActionLog = string("Setting this object in Processing mode and asking Controler for NextComputeAction") ;
        addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

      	answ->setAnswerStatus(AnswerStatus::astatusProcessing, interfaceHandle) ;
				answ->setAnswerProcessStage(AnswerStatus::apstageStarting) ;

        return _bb->Controler()->NextComputeAction(answ, (OB1Token*) 0, true, interfaceHandle) ;
/*
        exist = bb->Controler()->createToken(ADD, answ) ;

        sActionLog += string(" (token ") + IntToString(exist->getNumero()) + string(" created)") ;
        addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

        BB()->Controler()->Dispatch(exist, true) ;
*/
      }
      else
      {
      	sActionLog += string(" (failed)") ;
        addActionString(sActionLog, BB1BBInterface::trError) ;
      }
    }
  }
  else
  	addActionString("Deterministic mode (nothing done)", BB1BBInterface::trSubSteps) ;

  return true ;
}

void
BB1BBInterface::insertLeavesOnBlackBoard(string sPath, NSPatPathoArray *pAnswers, NautilusObjectType type, HWND interfaceHandle)
{
	if ((NULL == pAnswers) || pAnswers->empty())
		return ;

	NSSuper *pSuper = _pContexte->getSuperviseur() ;

  PatPathoIter iterSuiv ;
	for (PatPathoIter i = pAnswers->begin() ; pAnswers->end() != i ; i++)
  {
  	iterSuiv = i ;
    iterSuiv++ ;

    if ((pAnswers->end() == iterSuiv) || ((*iterSuiv)->getColonne() <= (*i)->getColonne()))
    {
    	// on est sûr ici que iterElement est une feuille
      NSPatPathoArray* pPatPatho = new NSPatPathoArray(_pContexte->getSuperviseur(), graphPerson) ;
      pPatPatho->ajoutePatho(i, 0, 0) ;
      string sCheminLex = pAnswers->donneCheminItem(i) ;
      if ((*i)->getUnit() != "")
      	sCheminLex += string(1, cheminSeparationMARK) + (*i)->getUnit() ;
      sCheminLex += string(1, cheminSeparationMARK) + (*i)->getLexique() ;
      string sCheminFils = "" ;
      string sElemLex, sCodeSens ;
      // on enlève d'abord la racine
      size_t pos = sCheminLex.find(string(1, cheminSeparationMARK)) ;
      if (string::npos != pos)
      	sCheminLex = string(sCheminLex, pos+1, strlen(sCheminLex.c_str())-pos-1) ;
      else
      	sCheminLex = "" ;

      while (string("") != sCheminLex)
      {
      	pos = sCheminLex.find(string(1, cheminSeparationMARK)) ;
        if (string::npos != pos)
        {
        	sElemLex = string(sCheminLex, 0, pos) ;
          NSDico::donneCodeSens(&sElemLex, &sCodeSens) ;
          sCheminFils += string(1, cheminSeparationMARK) + sCodeSens ;
          sCheminLex = string(sCheminLex, pos+1, strlen(sCheminLex.c_str())-pos-1) ;
        }
        else
        {
        	sElemLex = sCheminLex ;
          NSDico::donneCodeSens(&sElemLex, &sCodeSens) ;
          sCheminFils += string(1, cheminSeparationMARK) + sCodeSens ;
          sCheminLex = string("") ;
        }
      }

      if (string("") != sCheminFils)
      	sCheminFils = sPath + sCheminFils ;
      else
      	sCheminFils = sPath ;

      sCheminFils = getRegularPath(sCheminFils, cheminSeparationMARK, intranodeSeparationMARK) ;

			insertAnswerOnBlackboard(sCheminFils, pPatPatho, type, interfaceHandle) ;
		}
	}
}

void
BB1BBInterface::signalThatPatpathoWasSaved(const NSPatPathoArray* pPPT, bool /* bNew */)
{
	if ((NULL == _bb) || (NULL == _bb->Controler()) || (NULL == pPPT) || pPPT->empty())
		return ;

	_bb->Controler()->deprecateProcessedTokens() ;
  _bb->Controler()->resetProcessingTokens() ;

  _bb->challengeQuestionsWhenPatPathoSaved(pPPT) ;
}

void
BB1BBInterface::driveKSfromDialog(long int iToken, KSCONTROL iControl)
{
 	if ((NULL == _bb) || (NULL == _bb->Controler()))
		return ;

  string sActionLog = string("Driving a Token from a dialog ") +
                       string("(name=\"") + IntToString(iToken) + string("\"") ;
  if      (iControl == ksFree)
  	sActionLog += string(" new mode=\"free_and_running\")") ;
  else if (iControl == ksHold)
  	sActionLog += string(" new mode=\"waiting_for_user_input\")") ;

  addActionString(sActionLog, BB1BBInterface::trSteps) ;

  OB1Token* pTok = _bb->Controler()->findTokenFromId(iToken) ;
  if ((NULL == pTok) || (_bb->Controler()->isInvalidToken(pTok)))
	{
  	string sActionLog = string("Token ") + IntToString(iToken) + string(" not found") ;
  	addActionString("KS not found", BB1BBInterface::trError) ;
		return ;
	}

	if      (ksFree == iControl)
		pTok->setFree() ;
	else if (ksHold == iControl)
  	pTok->setWaiting() ;
}

/*
void
BB1BBInterface::driveKSfromDialog(string sKsName, KSCONTROL iControl)
{
 	if ((NULL == bb) || (sKsName == ""))
		return ;

  string sActionLog = string("Driving a KS from a dialog ") +
                       string("(name=\"") + sKsName + string("\"") ;
  if      (iControl == ksFree)
  	sActionLog += string(" new mode=\"free_and_running\")") ;
  else if (iControl == ksHold)
  	sActionLog += string(" new mode=\"waiting_for_user_input\")") ;

  addActionString(sActionLog, BB1BBInterface::trSteps) ;

	OB1NKS* ks = bb->Controler()->Graph().getKSByName(sKsName) ;
  if (!ks)
  {
  	addActionString("KS not found", BB1BBInterface::trError) ;
  	return ;
  }

	if      (iControl == ksFree)
		ks->putKsState(FREE_AND_RUNNING) ;
	else if (iControl == ksHold)
  	ks->putKsState(WAITING_FOR_USER_INPUT) ;
}
*/

void
BB1BBInterface::connectTokenToWindow(long int iToken, HWND hInterfaceWindow)
{
	if ((NULL == _bb) || (NULL == _bb->Controler()))
		return ;

	OB1Token* pTok = _bb->Controler()->findTokenFromId(iToken) ;
  if (NULL == pTok)
		return ;

	pTok->setInterfaceWindowHandle(hInterfaceWindow) ;

  pTok->setMMIInDuty(false) ;
}

// Synchronous function : gives an answer if any, else just look in patient's record
//
// Returns : 1 if something found, 0 if not
//
AnswerStatus::ANSWERSTATUS
BB1BBInterface::precoche(string sPath, NSPatPathoArray ** pAnswer, HWND interfaceHandle, string* pAnswerDate, bool bCompute, NSSearchStruct *pSearchStruct)
{
  sPath = getRegularPath(sPath, cheminSeparationMARK, intranodeSeparationMARK) ;

	_pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps, "Précochage : %s", sPath.c_str()) ;
  _pContexte->getSuperviseur()->DebugNewLine() ;

  string sActionLog = string("Asking for precoche ") +
                       string("(path=\"") + sPath + string("\")") ;

  addActionString(sActionLog, BB1BBInterface::trSteps) ;
  _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trSubDetails) ;

  //
  // First, look for an answer to such a question
  //
  // AnswerStatus::ANSWERSTATUS iAnswerStatus = AnswerStatus::astatusUnknown ;

  TypedVal temp(sPath) ;
  TypedVal searchParam(pSearchStruct) ;
  BB1Object* res = _bb->find(temp, std::string("Answer"), &searchParam, true) ;

  if ((res) &&
	           (res->getAnswerStatus() != AnswerStatus::astatusProcessing) &&
             (res->getAnswerStatus() != AnswerStatus::astatusDeprecated))
  {
    NSPatPathoArray *pTempAnswer = getPatPatho2(res, string("node")) ;// res->Value("node", NULLPATHO) ;
    if (pTempAnswer)
    	*pAnswer = new NSPatPathoArray(*pTempAnswer) ;
    else
      *pAnswer = new NSPatPathoArray(_pContexte->getSuperviseur()) ;

    if (pAnswerDate)
    	*pAnswerDate = res->Value("node_date", string("")) ;

    sActionLog = string("information found on BB, leaving precoche for path ") + sPath ;
    if (pTempAnswer)
    	sActionLog += string(" (empty result)") ;
    else
      sActionLog += string(" (not empty result)") ;
  	addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
    _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trSubDetails) ;

    return res->getAnswerStatus() ;
  }

  sActionLog = string("calling getAnswer2Question to answer precoche for path ") + sPath ;
  _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trSubDetails) ;

  //  New
  if (bCompute)
    return getAnswer2Question(sPath, "", pAnswer, *pAnswerDate, true /* bCompute */, false /* bUserIsWaiting */, interfaceHandle, pSearchStruct) ;

  return AnswerStatus::astatusUnknown ;

/*
  // Search in patient's file
  //
  std::string pa_n = ParseNautilusPath(sPath) ;
  PathsList que ;
  que.push_back(new std::string(pa_n)) ;
  NSPatPathoArray* tem = SearchInPatientFolder(&que, pAnswerDate, start_date, end_date) ;

  if (tem == NULL)
  {
  	*pAnswer = new NSPatPathoArray(pContexte) ;

    sActionLog = string("Information not found in patient record") ;
  	addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

    return 0 ;
  }

  // On a quelque chose dans le dossier patient
  // We have found something in patient's record

  *pAnswer = new NSPatPathoArray(*tem) ;

  BB1KB			 *informationKB = bb->KBNamed("InformationKB") ;
  BB1Class	 *answer 	   = bb->ClassNamed("Answer") ;

  AttValPair label		("label", string("Default calcul")) ;
  AttValPair Explication("explication", sPath) ;
  //
  // true : because we don't duplicate tem, so we must not delete tem
  //
  AttValPair node	("node", tem, true) ;   // FIXME MARDI

  std::string id = "default" ;
  char* nb = new char[10] ;
  itoa(bb->getNBObject(), nb, 10) ;
  id.append(nb) ;
  delete[] nb ;

  sActionLog = string("Information found in patient record, creating answer on BB") ;
  addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

  // création de la réponse
  BB1AppInst	*objectAnswer =  answer->MakeInstance(id,Explication, *informationKB,
                              Collect(&label, &node), NULL, true) ;

	return 1 ;
*/
}

bool
BB1BBInterface::synchronousCall(string sPath, NSPatPathoArray ** pAnswer, HWND interfaceHandle, string* pAnswerDate, NSSearchStruct *pSearchStruct)
{
  if ((HWND) 0 == interfaceHandle)
    interfaceHandle = HWindow ;

  // Make certain path is a path of concepts
  //
  string sConceptsPath = string("") ;
  _pContexte->getDico()->donneCodeSens(&sPath, &sConceptsPath) ;

  AnswerStatus::ANSWERSTATUS res = precoche(sConceptsPath, pAnswer, interfaceHandle, pAnswerDate, true, pSearchStruct) ;

  if ((AnswerStatus::astatusProcessed == res) && (*pAnswer) && (false == (*pAnswer)->empty()))
    return true ;

  //
  // A KS is at work... let's wait
  //
  if (AnswerStatus::astatusProcessing == res)
  {
    while (AnswerStatus::astatusProcessing == res)
    {
      if (*pAnswer)
      {
        delete *pAnswer ;
        *pAnswer = (NSPatPathoArray*) 0 ;
      }

      _pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

      res = getAnswer2Question(sConceptsPath, "", pAnswer, *pAnswerDate, false /* don't compute */, true, interfaceHandle, pSearchStruct) ;
      if ((AnswerStatus::astatusProcessed == res) && (*pAnswer) && (false == (*pAnswer)->empty()))
        return true ;
    }
  }

  if (*pAnswer)
  {
    delete *pAnswer ;
    *pAnswer = (NSPatPathoArray*) 0 ;
  }

  return false ;
}

/*
** Ask Blackboard for a question as a path
**
** sPath           : Question, in the form "SINC0/KEFFO/WCEA0"
** sArchetype      : Empty, except when this Archetype is started by the Ks that answers questions : means directKS is enabled just once for the question
** pAnswer         : Answer in the form of a patpatho
** sAnswerDate     : Date of information in answer
** bCompute        : true = ask the question, false = just look for an answer
** bUserIsWaiting  : Ask the Blackboard to prioritize this question (because user is waiting!)
** interfaceHandle : Interface to send message to when answer is ready
** pSearchStruct   : Search parameters
*/
AnswerStatus::ANSWERSTATUS
BB1BBInterface::getAnswer2Question(string sPath, string sArchetype, NSPatPathoArray** pAnswer, string &sAnswerDate, bool bCompute, bool bUserIsWaiting, HWND interfaceHandle, NSSearchStruct *pSearchStruct)
{
  sPath = getRegularPath(sPath, cheminSeparationMARK, intranodeSeparationMARK) ;

	AnswerStatus::ANSWERSTATUS iAnswerStatus = AnswerStatus::astatusUnknown ;

	string sActionLog = string("Get answer to question ") +
                       string("(path=\"") + sPath + string("\" Compute=") ;
  if (bCompute)
  	sActionLog += string("true)") ;
  else
  	sActionLog += string("false)") ;

  addActionString(sActionLog, BB1BBInterface::trSteps) ;
  _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trDetails) ;

  _isDeterministe = true ;
  TypedVal temp(sPath) ;

  TypedVal *pSearchParam = (TypedVal *) 0 ;
  TypedVal searchParam(pSearchStruct) ;
  if (pSearchStruct)
    pSearchParam = &searchParam ;

try
{
  if (true == bCompute) // we have to compute if it possible
  {
    string sActionLog = string("Get answer to question for path ") + sPath + string(". Asking BBK to compute.") ;
    _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trSubDetails) ;

  	COMPUTABILITYLEVEL iCLevel = ComputeQuestion(&temp, NULL, bUserIsWaiting, interfaceHandle, -1, pSearchStruct) ;

    // No KS wants to work on it, then look in patient's file
    //
    if (clNotComputable == iCLevel)
    {
    	TypedVal toFind(sPath) ;
      LeafDefaultComputation(toFind, pSearchParam, interfaceHandle, this) ;
    }
  }
}
catch(...)
{
  string sActionLog = string("Get answer to question ") +
                       string("(path=\"") + sPath + string(") when computing question") ;
  _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trError) ;

	erreur(sActionLog.c_str(), standardError, 0, _bb->pContexte->GetMainWindow()->GetHandle()) ;
	_isDeterministe = false ;
}

try
{
  // true means "don't only look at the nodes"
  //
  BB1Object* res = _bb->find(temp, std::string("Answer"), pSearchParam, true) ;
  if (NULL == res)
  	res = _bb->find(temp, std::string("InformationKB"), pSearchParam, true) ;

  sActionLog = string("Get answer to question for path ") + sPath + string(".") ;

  if (res)
  {
  	iAnswerStatus = res->getAnswerStatus() ;

    NSPatPathoArray *pTempAnswer = getPatPatho2(res, string("node")) ;
    if ((pTempAnswer) && (AnswerStatus::astatusProcessed == iAnswerStatus))
    {
    	*pAnswer = new NSPatPathoArray(*pTempAnswer) ;
      sAnswerDate = res->Value("node_date", string("")) ;
      sActionLog += string(" Answer found as ppt.") ;
    }
    else
    {
    	*pAnswer = new NSPatPathoArray(_pContexte->getSuperviseur()) ;
      sActionLog += string(" Empty answer found.") ;
    }

    // If there is an archetype involved in setting this question's answer
    // we have to prevent directKS to re-evaluate it automatically later
    //
    if (string("") != sArchetype)
    {
      AnswerStatus* pAnswerStatus = res->Value("answerStatus", NULLLANSWERSTATUS) ;
      if (pAnswerStatus)
      {
        pAnswerStatus->setDirectKsEnabled(false) ;

        // While this question exists, no need to update it
        //
        pAnswerStatus->setUpdatabilityStatus(AnswerStatus::updatabilityNever) ;
      }
    }
  }
  else
  {
    *pAnswer = new NSPatPathoArray(_pContexte->getSuperviseur()) ;
    sActionLog += string(" No answer found.") ;
  }

  _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trDetails) ;

  // If interfaceHadle is BBK interface, we must release one instance counter
  //
  if ((HWindow == interfaceHandle) && (AnswerStatus::astatusProcessing != iAnswerStatus))
  {
    BB1Object* quest = _bb->searchQuestionInKB(temp, pSearchParam) ;
    if (quest)
      _bb->unsuscribeHandleToQuestion(quest, HWindow, true /* bWithCounter */) ;
  }

  _isDeterministe = false ;
}
catch(...)
{
  string sActionLog = string("Get answer to question ") +
                       string("(path=\"") + sPath + string(")") ;
  _pContexte->getSuperviseur()->trace(&sActionLog, 1, NSSuper::trError) ;

	erreur(sActionLog.c_str(), standardError, 0, _bb->pContexte->GetMainWindow()->GetHandle()) ;
	_isDeterministe = false ;
}
  return iAnswerStatus ; // FIXME return
}

/*
BB1BBInterface::COMPUTABILITYLEVEL
BB1BBInterface::ComputeQuestion(TypedVal* quest)
{
	if (quest == NULL)
		return clError ;

	//
	// First, create an "answer" for follow up
  //
  BB1Object* res = bb->find(*quest, std::string("Answer")) ;
  if (NULL == res)
	{
    std::string sDaf = quest->getName().getString() ;
    std::string temp = ParseNautilusPath(sDaf) ;

    // Creation of the answer
    //
    BB1KB		 *informationKB = BB()->KBNamed("InformationKB") ;
    BB1Class *answer        = BB()->ClassNamed("Answer") ;

		AttValPair label      ("label",       string("Follow up answer")) ;
		AttValPair sfrom      ("sfrom",       string("Interface::ComputeQuestion")) ;
		AttValPair Explication("explication", sDaf) ;
    //
    // true : because we don't duplicate respat, so we must not delete respat
    //
    NSPatPathoArray* respat = NULL ;

    AttValPair node ("node", respat, true) ;

    std::string id = "default" ;
    char* nb = new char[10] ;
    itoa(other->Controler()->BB()->getNBObject(), nb, 10) ;
    id.append(nb) ;
    delete[] nb ;

    // création de la réponse
    BB1AppInst *objectAnswer =
    answer->MakeInstance(id,Explication, *informationKB,
                               Collect(&label, &node, &sfrom), NULL, true) ;

    res = bb->find(*quest, std::string("Answer")) ;
  }

  if (NULL != res)
  {
  	res->setAnswerStatus(AnswerStatus::astatusProcessing) ;
		res->setAnswerProcessStage(AnswerStatus::apstageUnknown) ;
  }

	AskDeterministicQuestion* question = new AskDeterministicQuestion("DeterministicQuestion", quest, 10) ;
	OB1Strategy* strat = addAskDeterministicQuestion(question) ;

  if (NULL == strat)
  {
  	if (NULL != res)
			res->setAnswerProcessStage(AnswerStatus::apstageRecord) ;
  	return clNotComputable ;
  }

  if (NULL != res)
			res->setAnswerProcessStage(AnswerStatus::apstageDirectKS) ;

	bool out = false ;
	while ((out != true) && (NULL != strat))
		out = bb->Controler()->ExecuteDeterministicAction(&strat) ;
	// return out ;
  return clComputable ;
}
*/

BB1BBInterface::COMPUTABILITYLEVEL
BB1BBInterface::ComputeQuestion(TypedVal* quest, OB1Token* pMasterToken, bool bUserIsWaiting, HWND interfaceHandle, long int persistenceHandle, NSSearchStruct *pSearchStruct)
{
	if ((NULL == quest) || (NULL == _bb) || (NULL == _bb->Controler()))
		return clError ;

  TypedVal *pSearchParam = (TypedVal *) 0 ;
  TypedVal searchParam(pSearchStruct) ;
  if (pSearchStruct)
    pSearchParam = &searchParam ;

  bool bQuestionCreated = _bb->Controler()->createQuestionAndAnswer(quest, pSearchParam) ;
  if (false == bQuestionCreated)
  	return clError ;

  RefreshObjectsList() ;

  BB1Object* pQuestion = _bb->searchQuestionInKB(*quest, pSearchParam) ;
  if (NULL == pQuestion)
		return clError ;

  if (interfaceHandle)
    if (interfaceHandle == HWindow)
      _bb->suscribeHandleToQuestion(pQuestion, interfaceHandle, true /* bWithCounter */) ;
    else
      _bb->suscribeHandleToQuestion(pQuestion, interfaceHandle) ;

  if (pMasterToken)
    _bb->suscribeTokenToQuestion(pQuestion, pMasterToken) ;

  if (-1 != persistenceHandle)
    _bb->suscribePersistenceToQuestion(pQuestion, persistenceHandle) ;

	_bb->Controler()->NextComputeAction(pQuestion, pMasterToken, bUserIsWaiting, interfaceHandle) ;

  return clComputable ;
}

// Unregister this interface from all objects
//
void
BB1BBInterface::DisconnectInterface(HWND interfaceHandle)
{
  if (_bb)
  {
    _bb->unsuscribeHandle(interfaceHandle) ;
    RefreshObjectsList() ;
  }
}

//
// Returns :
//    - NULL is nothing can be found in patient record
//    - A new NSPatPathoArray corresponding to the found node's array elsewhere
//
NSPatPathoArray*
BB1BBInterface::SearchInPatientFolder(PathsList* pPaths, string* pAnswerDate, string start_date, string end_date)
{
	NSPatPathoArray	*pAnswer = NULL ;

	if ((NULL == pPaths) || (pPaths->empty()))
		return pAnswer ;

  NSSearchStruct searchStruct(NSSearchStruct::modeAsIs,
                              NSSearchStruct::typeAsIs ,                              NSSearchStruct::positLastInTime,                              1,
                              string(""),
                              start_date,
                              string(""),
                              end_date,
                              NULL) ;

  return SearchInPatientFolder(pPaths, pAnswerDate, &searchStruct) ;
}

NSPatPathoArray*
BB1BBInterface::SearchInPatientFolder(PathsList* pPaths, string* pAnswerDate, NSSearchStruct *pSearchStruct)
{
  NSPatPathoArray	*pAnswer = (NSPatPathoArray	*) 0 ;

	if ((NULL == pPaths) || (pPaths->empty()))
		return pAnswer ;

  for (PathsIterator pathsIter = pPaths->begin() ; pPaths->end() != pathsIter ; pathsIter++)
	{
    // Create a local copy of search structure
    //
    NSSearchStruct searchStruct ;
    if (pSearchStruct)
      searchStruct = *pSearchStruct ;

    string sActionString = string("BB1BBInterface::SearchInPatientFolder, start ChercheChemin for ") + **pathsIter ;
    addActionString(sActionString, BB1BBInterface::trSubSteps) ;

		bool bOk = _pContexte->getPatient()->ChercheChemin(**pathsIter, &searchStruct) ;
		if (bOk)
		{
			std::string date   = std::string("") ;
			std::string sNoeud = std::string("") ;
			if (false == searchStruct.isEmpty())
			{
        pAnswer = new NSPatPathoArray(_pContexte->getSuperviseur()) ;

				for (MappingNSSearchResult::MMapIt it = searchStruct.getFoundNodes()->begin() ; searchStruct.getFoundNodes()->end() != it ; it++)
        {
          std::string foundDate = std::string("") ;
        	searchStruct.getFoundNodes()->fullRData(it, foundDate, sNoeud) ;

          // no previous date or newer date, this result replaces previous ones
          //
          if ((std::string("") == date) || (foundDate > date))
          {
            date = foundDate ;
            pAnswer->vider() ;
				    _pContexte->getPatient()->DonneArray(sNoeud, pAnswer) ;
				    pAnswer->clearAllIDs() ;
          }
          // same date, we add new result to previous one
          //
          else if (foundDate == date)
          {
            NSPatPathoArray localAnswer(_pContexte->getSuperviseur()) ;
            _pContexte->getPatient()->DonneArray(sNoeud, &localAnswer) ;
            pAnswer->InserePatPatho(pAnswer->end(), &localAnswer, 0) ;
				    pAnswer->clearAllIDs() ;
          }
        }

        *pAnswerDate = date ;

				break ;
			}
		}
	}

	return pAnswer ;
}

void
BB1BBInterface::EnableControl(int RessourceId, bool visible)
{
  if (visible)
  {
    ::ShowWindow(GetDlgItem(RessourceId),   SW_SHOW);
    ::EnableWindow(GetDlgItem(RessourceId), TRUE );
  }
  else
  {
    ::ShowWindow(GetDlgItem(RessourceId),   SW_HIDE);
    ::EnableWindow(GetDlgItem(RessourceId), FALSE );
  }
}

void
BB1BBInterface::showKsProperty(uint iIndex)
{
	std::vector<OB1NKS*>* kss = _bb->Controler()->Graph().getKSs() ;
	if (iIndex >= kss->size())
		return ;

  NSOB1NodesTreeControl* pNodesCtrl = new NSOB1NodesTreeControl(0, this, (*kss)[iIndex], 0, 0, 0, 0, 0, TTreeWindow::twsNone, pNSResModule) ;
	OB1NodesInterfaceContainer* interfaceMDI = new OB1NodesInterfaceContainer(*(_pContexte->getSuperviseur()->getApplication()->prendClient()), _pContexte, pNodesCtrl) ;
	interfaceMDI->Create() ;
	pNodesCtrl->SetParent(interfaceMDI) ;
}

void
BB1BBInterface::addActionString(string sActionText, BBTRACETYPE iActionLevel)
{
	if ((iActionLevel > _pContexte->getSuperviseur()->getBbkTraceLevel()) || (sActionText == string("")))
  	return ;

  string sActionTexteInList = string("") ;

  switch(iActionLevel)
  {
    case trError      : sActionTexteInList = string("* ") ; break ;
    case trWarning    : sActionTexteInList = string("! ") ; break ;
    case trSteps      : sActionTexteInList = string("> ") ; break ;
    case trSubSteps   : sActionTexteInList = string("    ") ; break ;
    case trDetails    : sActionTexteInList = string("      ") ; break ;
    case trSubDetails : sActionTexteInList = string("        ") ; break ;
  }

  sActionTexteInList += sActionText ;

  // TListWindItem	item(sActionTexteInList.c_str(), 0) ;
  // _actionsInOB1->InsertItem(item) ;
  // _actionsInOB1->Invalidate() ;

  struct  date dateSys;
	struct  time heureSys;
	char    msg[255] ;

	string sTraceFileName ;
/*
	if (numInstance > 1)
		sTraceFileName = string("traceBBK") + IntToString(numInstance) + string(".inf") ;
  else
*/
  sTraceFileName = string("traceBBK.inf") ;
	ofstream outFile ;
	outFile.open(sTraceFileName.c_str(), ios::app) ;
	if (!outFile)
		return ;

	// calcul de l'heure système - get system time
	getdate(&dateSys) ;
	gettime(&heureSys) ;
	sprintf(msg, "%02d/%02d/%4d - %02d:%02d:%02d,%02d>", dateSys.da_day, dateSys.da_mon,
                dateSys.da_year, heureSys.ti_hour, heureSys.ti_min,
                heureSys.ti_sec, heureSys.ti_hund) ;

	outFile << string(msg) ;

  for (int j = 0; j < iActionLevel; j++)
    outFile << string("\t") ;
  outFile << sActionText ;

	outFile << string("\n") ;
	outFile.close() ;
}

// Check that a path of the kind VINR0/VMIN1/20000/£N0 get transformed into
//                                                       VINR0/VMIN1/20000.£N0
//
std::string
ParseNautilusPath(std::string& naut_path)
{
  return getRegularPath(naut_path, cheminSeparationMARK, intranodeSeparationMARK) ;

/*
  std::string result = naut_path ;
  for (register unsigned int i = 0; i < result.size(); i++)
  {
    if (cheminSeparationMARK == result[i])
    {
      bool change = false ;
      if (result.size() > i + 1)
      {
        if ('$' == result[i+1])
          change = true ;
        else if (result.size() > i + 2)
        {
          std::string temp = result.substr(i+1, 2) ;
          if ((temp == "£N") || (temp == "£D") || (temp == "£T"))
            change = true ;
          else if (result.size() > i + 3)
          {
            std::string temp = result.substr(i+1, 3) ;
            if (temp == "WCE")
              change = true ;
            else if (result.size() > i + 4)
            {
              std::string temp = result.substr(i+1, 4) ;
              if (temp == "WPLU")
                change = true ;
            }
          }
        }
      }
      if (true == change)
        result[i] = intranodeSeparationMARK ;
    }
  }
  return (result) ;
*/
}

/*******************************************************************************
************* Implementation de fenetre gérant l'interface *********************
*******************************************************************************/

DEFINE_RESPONSE_TABLE1(OB1InterfaceContainer, TMDIChild)
    EV_WM_DESTROY,
    EV_WM_SIZE,
    EV_WM_CLOSE,
END_RESPONSE_TABLE ;

OB1InterfaceContainer::OB1InterfaceContainer(TMDIClient& parent, NSContexte *Contexte, BB1BBInterface* interf)
                      :TMDIChild(parent, "*system* Blackboard")
{
	_interface = interf;
	_pContexte = Contexte;
	//Attr.Style = WS_CHILD |WS_THICKFRAME	| WS_VISIBLE ;
	Attr.X = 0 ;
	Attr.Y = 0 ;
	Attr.H = 700 ;
	Attr.W = 300 ;
}

OB1InterfaceContainer::~OB1InterfaceContainer()
{
  string sMsg = string("Deleting OB1 Interface container") ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

  if (_interface)
    _interface->SetParentContainer((OB1InterfaceContainer *) 0) ;
}

void
OB1InterfaceContainer::SetupWindow()
{
	TMDIChild::SetupWindow();
  SetClientWindow(_interface);
}

void
OB1InterfaceContainer::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TMDIChild::EvSize(sizeType, size) ;
}

void
OB1InterfaceContainer::EvDestroy()
{
}

bool
OB1InterfaceContainer::CanClose()
{
  if (NULL == _pContexte->getPatient())
  {
    string sMsg = string("Can close OB1 Interface container window (no patient)") ;
    _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
    return true ;
  }

	// on ruse avec un booléen dans PatientChoisi
	// pour résoudre le bug de la sauvegarde lors de la fermeture historique
	if (_pContexte->getPatient()->_bCanCloseHisto)
  {
    string sMsg = string("Can close OB1 Interface container window (with patient)") ;
    _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;
		return true ;
  }

	return false ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSKSListWindow//
// -----------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(NSKSListWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSKSListWindow::NSKSListWindow(BB1BBInterface *parent, int id, int x, int y, int w, int h, TModule *module)
               :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  pInterface    = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}
NSKSListWindow::NSKSListWindow(BB1BBInterface *parent, int id, TModule *module)
               :TListWindow((TWindow *) parent, id, module)
{
  pInterface    = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

void
NSKSListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}


// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSKSListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	int indexItem = HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		pInterface->showKsProperty(indexItem) ;
}

void
NSKSListWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& /* point */)
{
}

void
NSKSListWindow::EvKeyDown(uint /* key */, uint /* repeatCount */, uint /* flags */)
{
}

void
NSKSListWindow::EvLButtonUp(uint /* modKeys */, NS_CLASSLIB::TPoint& /* pt */)
{
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSKSListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0 ; i < count ; i++)		if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}


void
NSKSListWindow::EvSetFocus(HWND /* hWndLostFocus */)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
	SetTextBkColor(0x00fff0f0) ;
	Invalidate() ;

	int count = GetItemCount() ;

	for (int i = 0 ; i < count ; i++)
		if (GetItemState(i, LVIS_SELECTED))
			return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSKSListWindow::EvKillFocus(HWND /* hWndGetFocus */)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
	SetTextBkColor(0x00ffffff) ;
	Invalidate() ;
}

DEFINE_RESPONSE_TABLE1(NSQuestionListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSQuestionListWindow::NSQuestionListWindow(BB1BBInterface *parent, int id, int x, int y, int w, int h, TModule *module)
                     :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  pInterface    = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}
NSQuestionListWindow::NSQuestionListWindow(BB1BBInterface *parent, int id, TModule *module)
               :TListWindow((TWindow *) parent, id, module)
{
  pInterface    = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

void
NSQuestionListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}


// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSQuestionListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	int indexItem = HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		pInterface->showKsProperty(indexItem) ;
}

void
NSQuestionListWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& /* point */)
{
}

void
NSQuestionListWindow::EvKeyDown(uint /* key */, uint /* repeatCount */, uint /* flags */)
{
}

void
NSQuestionListWindow::EvLButtonUp(uint /* modKeys */, NS_CLASSLIB::TPoint& /* pt */)
{
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSQuestionListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0 ; i < count ; i++)		if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

void
NSQuestionListWindow::EvSetFocus(HWND /* hWndLostFocus */)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
	SetTextBkColor(0x00fff0f0) ;
	Invalidate() ;

	int count = GetItemCount() ;

	for (int i = 0 ; i < count ; i++)
		if (GetItemState(i, LVIS_SELECTED))
			return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSQuestionListWindow::EvKillFocus(HWND /* hWndGetFocus */)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
	SetTextBkColor(0x00ffffff) ;
	Invalidate() ;
}

DEFINE_RESPONSE_TABLE1(NSAnswerListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSAnswerListWindow::NSAnswerListWindow(BB1BBInterface *parent, int id, int x, int y, int w, int h, TModule *module)
                   :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  pInterface    = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}
NSAnswerListWindow::NSAnswerListWindow(BB1BBInterface *parent, int id, TModule *module)
                   :TListWindow((TWindow *) parent, id, module)
{
  pInterface    = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

void
NSAnswerListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}


// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSAnswerListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	int indexItem = HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		pInterface->showKsProperty(indexItem) ;
}

void
NSAnswerListWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& /* point */)
{
}

void
NSAnswerListWindow::EvKeyDown(uint /* key */, uint /* repeatCount */, uint /* flags */)
{
}

void
NSAnswerListWindow::EvLButtonUp(uint /* modKeys */, NS_CLASSLIB::TPoint& /* pt */)
{
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSAnswerListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0 ; i < count ; i++)		if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

void
NSAnswerListWindow::EvSetFocus(HWND /* hWndLostFocus */)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
	SetTextBkColor(0x00fff0f0) ;
	Invalidate() ;

	int count = GetItemCount() ;

	for (int i = 0 ; i < count ; i++)
		if (GetItemState(i, LVIS_SELECTED))
			return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSAnswerListWindow::EvKillFocus(HWND /* hWndGetFocus */)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
	SetTextBkColor(0x00ffffff) ;
	Invalidate() ;
}

DEFINE_RESPONSE_TABLE1(NSEventListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONUP,
END_RESPONSE_TABLE ;

NSEventListWindow::NSEventListWindow(BB1BBInterface *parent, int id, int x, int y, int w, int h, TModule *module)
                  :TListWindow((TWindow *) parent, id, x, y, w, h, module)
{
  pInterface    = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}
NSEventListWindow::NSEventListWindow(BB1BBInterface *parent, int id, TModule *module)
                  :TListWindow((TWindow *) parent, id, module)
{
  pInterface    = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

void
NSEventListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSEventListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	int indexItem = HitTest(info) ;
	if (info.GetFlags() & LVHT_ONITEM)		pInterface->showKsProperty(indexItem) ;
}

void
NSEventListWindow::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& /* point */)
{
}

void
NSEventListWindow::EvKeyDown(uint /* key */, uint /* repeatCount */, uint /* flags */)
{
}

void
NSEventListWindow::EvLButtonUp(uint /* modKeys */, NS_CLASSLIB::TPoint& /* pt */)
{
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSEventListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0 ; i < count ; i++)		if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

void
NSEventListWindow::EvSetFocus(HWND /* hWndLostFocus */)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
	SetTextBkColor(0x00fff0f0) ;
	Invalidate() ;

	int count = GetItemCount() ;

	for (int i = 0 ; i < count ; i++)
		if (GetItemState(i, LVIS_SELECTED))
			return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSEventListWindow::EvKillFocus(HWND /* hWndGetFocus */)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
	SetTextBkColor(0x00ffffff) ;
	Invalidate() ;
}

