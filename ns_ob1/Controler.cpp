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
#include "ns_ob1\OB1Controler.h"
#include "ns_ob1\InitStructure.h"
#include "ns_ob1\BB1KS.h"
#include "ns_ob1\OB1Token.h"
//#include "ns_ob1\DFS.h"
#include "ns_ob1\OB1Heuristix.h"
#include "ns_ob1\OB1Agenda.h"
#include "ns_ob1\BB1Object.h"
#include "ns_ob1\OB1Functor.h"
#include "ns_ob1\OB1Strategy.h"
#include "ns_ob1\BB1Class.h"
#include "ns_ob1\InterfaceForKs.h"
#include "ns_ob1\OB1Edge.h"
#include "ns_ob1\BB1KB.h"

#include "nssavoir\nsfilgd.h"

#ifdef _ENTERPRISE_DLL
  #include "ob1Lus/InterfaceLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
#else
  #include "ns_ob1\Interface.h"
  #include "nsepisod\nsToDo.h"
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
  #include "nautilus\nautilus.rh"
#endif

//Controler* _globalControler; //sert a recuperer un lien sur le controelr et le blackboard

Heuristix*
OB1Controler::GetHeuristix()
{
  return _choice.getHeuristix() ;
}

OB1Controler::OB1Controler(BB1BB* bbb)
             :_sheduler(this), _choice(this), _Agenda(this), _bb_state(this)
{
try
{
  _bb               = bbb ;
  _StrategyCounter  = 0 ;
  _lastTokenCreated = (OB1Token *) 0 ;
  _iNextHandle      = 1 ;
}
catch (...)
{
	erreur("Exception BB1 OB1Controler ctor.", standardError) ;
}
}

void OB1Controler::save()
{
  std::string res = "<br />" + _IClass.toHTML() ;

  for (int i = 0; i < _bb_state.numVertices(); i++)
    if (_bb_state[i]->getType() == KS_NODE)
    {
      OB1NKS* tp = dynamic_cast<OB1NKS*>(_bb_state[i]) ;
      if (tp)
      {
        const FunctorPublication* funcP = tp->getKS()->Publication() ;
        KsInitStructure* toPlace = (*funcP)() ; //tp->getKS()->Publication()();
        res += toPlace->toHTML() ;
        delete(toPlace) ;
      }
    }

  std::ofstream file("KSPUBLICATION.htm") ;
  if (file.is_open())
  {
    file << res ;
    file.close() ;
  }
}

void OB1Controler::addBB1Class(std::string name)
{
  _IClass.insertItem(name) ;
}

OB1Controler::~OB1Controler()
{
	deleteAllTokens() ;

  #ifdef __DEBUG__
  //std::string nem = "graphe2.htm";
  //_bb_state.save(nem);
  #endif
}

void OB1Controler::addKS(BB1KS* ks)
{
try
{
  if (NULL == ks)
		return ;

	const FunctorPublication* FuncP = ks->Publication() ;
	KsInitStructure* toPlace = (*FuncP)() ; //ks->Publication()();
	if (NULL == toPlace)
  {
    delete ks ;
		return ;
  }

	std::string name = toPlace->getKsName() ;
	OB1NKS* ksi = new OB1NKS(this, name, ks) ;
	ksi->putKsType(toPlace->KsType()) ;

	int ind =_bb_state.find(ksi) ;
	if (-42 == ind)
	{
		_bb_state.AddVertex(ksi) ; // add the KS node in graph

		CreateLevel(toPlace, ksi) ;
		CreateTrigger(toPlace, ksi) ;
		CreateResult(toPlace, ksi) ;
		CreateValidity(toPlace, ksi) ;
	}
  else
    delete ksi ;

	delete(toPlace) ;
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::addKS.", standardError) ;
}
}

bool
OB1Controler::CreateLevel(KsInitStructure* toPlace, OB1NKS* ksi )
{
try
{
	if ((NULL == toPlace) || (NULL == ksi))
		return false ;

  if ((toPlace->Required() == NULL) || (toPlace->Required()->getLevelsize() <= 0))
  	return false ;

	OB1NLevel* lev = new OB1NLevel(this) ;
  ksi->addBijection(new OB1NodeLink(lev, new OB1Edge(REQUIRES))) ;
  _bb_state.AddVertex(lev) ;
  addRecquired(toPlace->Required(), lev) ;
  if (toPlace->getLevel()->size() > 0)
  	AddLevel(toPlace->getLevel(), lev) ;

	return true ;
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::CreateLevel.", standardError) ;
	return false ;
}
}

bool
OB1Controler::CreateTrigger(KsInitStructure* toPlace, OB1NKS* ksi )
{
try
{
	if ((NULL == toPlace) || (NULL == ksi))
		return false ;

  if (toPlace->Trigger()->size() <= 0)
  	return false ;

	OB1NTrigger* trig = new OB1NTrigger(this) ;
  ksi->addBijection(new OB1NodeLink(trig, new OB1Edge(TRIGERS))) ;
  _bb_state.AddVertex(trig) ;
  AddTrigger(toPlace->Trigger(), trig) ;

  return true ;
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::CreateTrigger.", standardError) ;
	return false ;
}
}

bool
OB1Controler::CreateValidity(KsInitStructure* toPlace, OB1NKS* ksi )
{
try
{
	if ((NULL == toPlace) || (NULL == ksi))
		return false ;

  if ((toPlace->Validity() == NULL) || (toPlace->Validity()->getLevelsize() <= 0))
  	return false ;

  OB1NValidity* validity = new OB1NValidity(this) ;
  ksi->addBijection(new OB1NodeLink(validity, new OB1Edge(VALIDATED))) ;
  _bb_state.AddVertex(validity) ;
  AddValidity(toPlace->Validity(), validity) ;

  return true ;
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::CreateValidity.", standardError) ;
	return false ;
}
}

bool
OB1Controler::CreateResult(KsInitStructure* toPlace, OB1NKS* ksi )
{
try
{
	if ((NULL == toPlace) || (NULL == ksi))
		return false ;

  if (toPlace->getResult()->size() <= 0)
  	return false ;

	OB1NResult* res = new OB1NResult(this) ;
	ksi->addBijection(new OB1NodeLink(res, new OB1Edge(CREATES))) ;
	_bb_state.AddVertex(res) ;
	AddResult(toPlace->getResult(), res) ;

	return true ;
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::CreateResult.", standardError) ;
	return false ;
}
}

void
OB1Controler::AddValidity(Level* levels, OB1NValidity* lev)
{
try
{
	if ((NULL == levels) || (NULL == lev))
		return ;

  register unsigned int validitySize = levels->getLevelsize() ;
  int complexite = levels->Complexity() ;
  int fiabilite  = levels->Quality() ;
  for (register unsigned i = 0 ; i < validitySize ; i++)
  {
		INode* temp = (*levels)[i] ;
    TypedVal label(*(temp->_labeled)) ;
		OB1NOther* va = new OB1NOther(this, &label) ;

		OB1NOther* va_temp = _bb_state.find_other_nodes(va) ;
		if (va_temp)
		{
			delete (va) ;
			va = va_temp ;
		}
       // int ind =_bb_state.find(va);
        /*if (ind != -42)
          {
            delete (va);
            va =  dynamic_cast<OB1NOther* >(_bb_state[ind]);
          }*/
		else
			_bb_state.AddVertex(va) ;

		OB1EdgeLevel* ed00;
		ed00 = new OB1EdgeLevel(VALIDATED, createMask(temp->_classTo), complexite, fiabilite, i);
		lev->addBijection(new OB1NodeLink(va, ed00), createMask(temp->_classFrom)) ;
	}
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::AddValidity.", standardError) ;
}
}

void
OB1Controler::AddLevel(std::vector<Level*>* level, OB1NLevel* lev)
{
try
{
	if ((NULL == level) || (NULL == lev))
		return ;

  if (level->empty())
		return ;

	int i = 0 ;
	for (std::vector<Level*>::iterator levelIter = level->begin() ; levelIter != level->end() ; levelIter++, i++)
  {
		//On detecte le nombre de variable present dans le level
		register unsigned int obliSize2 = (*levelIter)->getLevelsize() ;

		// Creation du noeud et du niveaux
		OB1NAnd*      lev_i = new OB1NAnd(this) ;
		OB1EdgeLevel* ed_i  = new OB1EdgeLevel(IMPLIES, i) ;

		//ajout du noueud ET au noeud de level
		lev->addBijection(new OB1NodeLink(lev_i, ed_i)) ;

		// ajout du noeud et dans le graphe
		_bb_state.AddVertex(lev_i) ;
		int complexite = (*levelIter)->Complexity() ;
		int fiabilite = (*levelIter)->Quality() ;
		// Pour chaque variable du nievaux
		for (register unsigned  j = 0; j < obliSize2; j++)
		{
			// Creation du noeux
			INode* temp = (*(*levelIter))[j] ;
      TypedVal label(*(temp->_labeled)) ;
			OB1NOther* va      = new OB1NOther(this, &label) ;
			OB1NOther* va_temp = _bb_state.find_other_nodes(va) ;
			if (va_temp)
			{
				delete (va) ;
				va = va_temp ;
			}
			else
				_bb_state.AddVertex(va) ;

			OB1EdgeLevel* ed00 = new OB1EdgeLevel(IMPLIES, createMask(temp->_classTo), complexite, fiabilite, i) ;
			lev_i->addBijection(new OB1NodeLink(va, ed00), createMask(temp->_classFrom)) ;
		}
	}
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::AddLevel.", standardError) ;
}
}

void OB1Controler::addRecquired(Level* levels,OB1NLevel* lev)
{
try
{
  register unsigned int RecquiredSize = levels->getLevelsize() ;
  int complexite = levels->Complexity() ;
  int fiabilite =  levels->Quality() ;

  OB1NAnd*      and = new OB1NAnd(this) ;
  OB1EdgeLevel* ed  = new OB1EdgeLevel(REQUIRES, 0) ;
  lev->addBijection(new OB1NodeLink(and, ed)) ;
  _bb_state.AddVertex(and) ;
  for (register unsigned int i = 0 ; i < RecquiredSize ; i++)
  {
    INode* temp = (*levels)[i] ;
    TypedVal label(*(temp->_labeled)) ;
    OB1NOther* va      = new OB1NOther(this, &label) ;
    OB1NOther* va_temp = _bb_state.find_other_nodes(va) ;
    if (va_temp)
    {
      delete (va) ;
      va = va_temp ;
    }
    else
      _bb_state.AddVertex(va) ;

    OB1EdgeLevel* ed00 = new OB1EdgeLevel(REQUIRES, createMask(temp->_classTo), complexite, fiabilite, i) ;
    and->addBijection(new OB1NodeLink(va,ed00), createMask(temp->_classFrom)) ;
  }
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::addRecquired.", standardError) ;
}
}

void
OB1Controler::AddTrigger(std::vector< Level* >* trigger, OB1NTrigger* trig)
{
try
{
	if ((NULL == trigger) || (NULL == trig))
		return ;

  if (trigger->empty())
		return ;

  register unsigned int levelSize = trigger->size() ;
  // Si les variables obligatoire existent

  // Pour chaque niveaux
  for (register unsigned i = 0; i <  levelSize; i++)
  {
    //On detecte le nombre de variable present dans le level
    register unsigned int obliSize2 = (*trigger)[i]->getLevelsize() ;
    if (obliSize2 > 1)
    {
    	// Creation du noeud et du niveaux
      OB1NAnd* lev_i = new OB1NAnd(this) ;
      OB1Edge* ed_i  = new OB1Edge(TRIGERS) ;

      //ajout du noueud ET au noeud de level
      trig->addBijection(new OB1NodeLink(lev_i, ed_i)) ;
      // ajout du noeud et dans le graphe
      _bb_state.AddVertex(lev_i) ;

      // Pour chaque variable du nievaux
      for (register unsigned j = 0; j < obliSize2; j++)
      {
        // Creation du noeux
        INode* temp = (*(*trigger)[i])[j] ;
        TypedVal label(*(temp->_labeled)) ;
        OB1NOther* va = new OB1NOther(this, &label) ;
        //int ind =_bb_state.find(va);
        OB1NOther* va_temp = _bb_state.find_other_nodes(va) ;
        if (va_temp)
        {
          delete (va) ;
          va = va_temp ;
        }
        else
        	_bb_state.AddVertex(va) ;

        OB1Edge* ed00 ;
        ed00 = new OB1Edge(TRIGERS, createMask(temp->_classTo)) ;
        lev_i->addBijection(new OB1NodeLink(va,ed00), createMask(temp->_classFrom)) ;
      }
    }
    else
    {
      INode* temp = (*(*trigger)[i])[0] ;
      TypedVal label(*(temp->_labeled)) ;
      OB1NOther* va = new OB1NOther(this, &label) ;
      OB1Edge*   ed = new OB1Edge(TRIGERS, createMask(temp->_classTo)) ;

      OB1NOther* va_temp = _bb_state.find_other_nodes(va) ;
      if (va_temp)
      {
        delete (va) ;
        va = va_temp ;
      }

      //int ind =_bb_state.find(va);
      //if (ind != -42)
        //{
           //   delete (va);
          //    va =  dynamic_cast<OB1NOther* >(_bb_state[ind]);
        //}
      else
        _bb_state.AddVertex(va) ;
      trig->addBijection(new OB1NodeLink(va, ed), createMask(temp->_classFrom)) ;
		}
	}
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::AddTrigger.", standardError) ;
}
}

int
OB1Controler::createMask(std::string mask)
{
  int result = 0;
  std::vector<std::string>* temp = parseClass(mask) ;

  register unsigned int taille = temp->size() ;
  register unsigned int i ;

	for (i = 0 ; i < taille ; i++)
		result += _IClass[(*temp)[i]] ;

  delete temp ;

	return result ;
}

// This function finds a node whose path is equal to the token's path
// If it doesn't find it, then it looks for a semantically equivalent node
//
// On cherche un noeud d'acceuil pour ce token (retour = true si on a trouvé)
// We look for a node to shelter this token (return true if found)
//
bool
OB1Controler::Dispatch(OB1Token* dis, bool createStat)
{
	if (NULL == dis)
		return false ;

	BB1Object* pAnswer = dis->getStatusObject() ;
  if (NULL == pAnswer)
  	return false ;

	AnswerStatus::ANSWERPROCESSINGSTAGE iProcessStage = pAnswer->getAnswerProcessStage() ;

  string sActionLog = string("Token ") + IntToString(dis->getNumero()) + string(" dispatching for process stage ") ;
  switch(iProcessStage)
  {
    case AnswerStatus::apstageUnknown   : sActionLog += string("Unknown") ;   break ;
    case AnswerStatus::apstageStarting  : sActionLog += string("Starting") ;  break ;
    case AnswerStatus::apstageDirectKS  : sActionLog += string("DirectKS") ;  break ;
    case AnswerStatus::apstageRecord    : sActionLog += string("Record") ;    break ;
    case AnswerStatus::apstageGenericKs : sActionLog += string("GenericKS") ; break ;
    case AnswerStatus::apstageExited    : sActionLog += string("Exited") ;    break ;
  }
  if (_bb->pContexte->getBBinterface())
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

  sActionLog = string("Token ") + IntToString(dis->getNumero()) + string(" dispatch") ;

  bool result = false ;

  // Récupération de l'objet blackboard attaché à ce token
  // We get the blackboard object related to this token
  //
	BB1Object* obj = dis->getObject() ;
	if (NULL == obj)
  {
  	sActionLog += string(" failed (no BB1Object)") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
  	return false ;
  }

	TypedVal* pDispinf = obj->PathAttributes() ;
  if (NULL == pDispinf)
  {
  	sActionLog += string(" failed: no \"explication\" attribute found") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
		return false ;
  }

	TypedVal cher = *pDispinf ;

  TypedVal searchStruct(NULLSEARCHSTRUCT) ;
  if (obj->SearchParams())
    searchStruct = *(obj->SearchParams()) ;

	// On cherche si un noeud héberge déjà un token de même chemin
  // We look for a node
	//
  // This OB1NOther node must be son of a Result or a Trigger
  //
	OB1NOther* tp = (OB1NOther*) 0 ;

  if      (AnswerStatus::apstageDirectKS == iProcessStage)
  {
  	tp = _bb_state.find_other_nodes(cher, searchStruct) ;
    if (tp && (false == isDispatchableNode(dis, tp)))
    	tp = (OB1NOther*) 0 ;
  }
  else if (AnswerStatus::apstageGenericKs == iProcessStage)
  {
  	std::string	sSearched = cher.getString() ;
  	//
  	// Semantic search
  	//
    std::map<std::string, OB1NOther* >* pMapOfOtherNodes = _bb_state.getOtherNodes() ;
    if ((NULL == pMapOfOtherNodes) || pMapOfOtherNodes->empty())
    	return false ;

    int iMinimalDistance = pathDistanceInfinite ;
    OB1NOther* pBestCandidate = (OB1NOther*) 0 ;

    NSFilGuide* pFilGuide = _bb->pContexte->getSuperviseur()->getFilGuide() ;

    std::map<std::string, OB1NOther*>::iterator curOnOther = pMapOfOtherNodes->begin() ;
    for (; pMapOfOtherNodes->end() != curOnOther ; curOnOther++)
    {
    	std::string sSortingId = (*curOnOther).first ;
      std::string sNodePath  = GetStringFromSortingId(sSortingId) ;

      if ((std::string("") != sNodePath) && (isDispatchableNode(dis, (*curOnOther).second)))
      {
      	// For the moment, we don't use semantics
        //
        int iDistance = pFilGuide->pathDistance(&sNodePath, &sSearched, false) ;
        if (iDistance < iMinimalDistance)
        	pBestCandidate = (*curOnOther).second ;
    	}
    }

    if (pBestCandidate)
    	tp = pBestCandidate ;
    else
    {
  		sActionLog += string(" failed: can't find a Node with same path") ;
      if (_bb->pContexte->getBBinterface())
    	  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
    	return false ;
    }
  }
  if (NULL == tp)
  {
  	sActionLog += string(" no node found") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
  	return false ;
  }

  if (tp->isLeaf())
  {
  	sActionLog += string(" on a leaf") ;
  	// sActionLog += string(" failed: can't find a result or trigger Node with same path") ;
    // bb->pContexte->getSuperviseur()->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
    // return false ;
  }

  //
  // If the node didn't accept the token...
  //
	if (tp->IsPlace(dis, cher) == false)
  {
  	sActionLog += string(" failed: token refused by Node ") + tp->getString() ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
    return false ;
  }

  //
  // If the node accepted the token...
  //
  result = true ;
  _lastTokenCreated = dis ;

  sActionLog += string(" succeeded on Node ") + tp->getString() ;
  if (_bb->pContexte->getBBinterface())
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

  if (createStat)
  {
    std::vector<OB1Node*>* vect_result = tp->startNode() ;
    if (vect_result)
    {
      int size = vect_result->size() ;
      if (size > 0)
        for (int i = 0 ; i < size ; i++)
          AskTriggerOrder(dis, 5) ;
      delete(vect_result) ;
    }
  }

	//run trigger
  return (result) ;
}

bool
OB1Controler::isDispatchableNode(OB1Token* pToken, OB1NOther* pOtherNode)
{
	if ((NULL == pToken) || (NULL == pOtherNode))
		return false ;

	BB1Object* pAnswer = pToken->getStatusObject() ;
  if (NULL == pAnswer)
  	return false ;

  AnswerStatus* pAnswerStatus = pAnswer->Value("answerStatus", NULLLANSWERSTATUS) ;
  if (NULL == pAnswerStatus)
		return false ;

	std::vector<OB1NodeLink*>& aParentLinks = pOtherNode->getParent() ;
  if (aParentLinks.empty())
		return false ;

	for (LinkIterator iter = aParentLinks.begin() ; aParentLinks.end() != iter ; iter++)
  {
		if (((*iter)->Edge()->TypeOf() == IS_CREATED) || ((*iter)->Edge()->TypeOf() == TRIGERS))
    {
    	OB1Node* pFatherNode = (*iter)->Node() ;

      OB1NKS* pKsNode = NULL ;

      if ((*iter)->Edge()->TypeOf() == IS_CREATED)
      {
      	OB1NResult* pResult = dynamic_cast<OB1NResult*>(pFatherNode) ;
    		if (pResult)
        	pKsNode = pResult->KS() ;
      }
      else if ((*iter)->Edge()->TypeOf() == TRIGERS)
      {
      	OB1NTrigger* pTrigger = dynamic_cast<OB1NTrigger*>(pFatherNode) ;
    		if (pTrigger)
    			pKsNode = pTrigger->KS() ;
      }
      if (pKsNode)
      {
      	std::string sKsName = pKsNode->getKSName() ;
        KSAttemptHistoryElement* pHistoElement = pAnswerStatus->getHistoryElement(sKsName) ;
        if (NULL == pHistoElement)
        	return true ;
      }
    }
  }
  return false ;
}

bool
OB1Controler::DispatchAgain(OB1Token* dis, bool createStat)
{
	if (NULL == dis)
		return false ;

  string sActionLog = string("Token ") + IntToString(dis->getNumero()) + string(" dispatch again") ;

  // Récupération de l'objet blackboard attaché à ce token
  // We get the blackboard object related to this token
  //
	BB1Object* obj = dis->getObject();
	if (NULL == obj)
  {
  	sActionLog += string(" failed (no BB1Object)") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
  	return false ;
  }

  OB1Node* pPrevNode = dis->Node() ;
  if (NULL == pPrevNode)
  {
  	sActionLog += string(" failed (no previous node)") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
  	return false ;
  }
  OB1NOther* pPreviousNode = static_cast <OB1NOther*>(pPrevNode) ;
  if (NULL == pPreviousNode)
  {
  	sActionLog += string(" failed (previous node is not a OB1NOther)") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
  	return false ;
  }

	TypedVal* pDispinf = obj->PathAttributes() ;
	if (NULL == pDispinf)
  {
  	sActionLog += string(" failed: no \"explication\" attribute found") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
		return false ;
  }

  TypedVal cher = *pDispinf ;

	// On cherche si un noeud héberge déjà un token de même chemin
  // We look for a node
	//
  // This OB1NOther node must be son of a Result or a Trigger
  //

  std::string	sSearched = cher.getString() ;
  //
  // Semantic search
  //
  std::map<std::string, OB1NOther* >* pMapOfOtherNodes = _bb_state.getOtherNodes() ;
  if (pMapOfOtherNodes->empty())
    return false ;

  int iMinimalDistance = pathDistanceInfinite ;
  OB1NOther* pBestCandidate = NULL ;

  NSFilGuide* pFilGuide = _bb->pContexte->getSuperviseur()->getFilGuide() ;

  std::map<std::string, OB1NOther*>::iterator curOnOther = pMapOfOtherNodes->begin() ;
  for ( ; curOnOther != pMapOfOtherNodes->end() ; curOnOther++)
  {
  	OB1NOther* pCandidateNode = (*curOnOther).second ;

    // Don't accept the previous node or leaf nodes
    //
    if ((pCandidateNode) && (pCandidateNode != pPreviousNode) && (!(pCandidateNode->isLeaf())))
    {
    	std::string sSortingId = (*curOnOther).first ;
    	std::string sNodePath  = GetStringFromSortingId(sSortingId) ;

    	if (sNodePath != std::string(""))
    	{
      	// For the moment, we don't use semantics
      	//
      	int iDistance = pFilGuide->pathDistance(&sNodePath, &sSearched, false) ;
      	if (iDistance < iMinimalDistance)
        	pBestCandidate = (*curOnOther).second ;
      }
    }
  }

  if (NULL == pBestCandidate)
  {
    sActionLog += string(" failed: can't find a Node") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
    return false ;
  }

  //
  // If the node, didn't accept the token...
  //
	if (pBestCandidate->IsPlace(dis, cher) == false)
  {
  	sActionLog += string(" failed: token refused by Node ") + pBestCandidate->getString() ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
    return false ;
  }

  // _lastTokenCreated = dis ;  token is not new

  // modifying the token
  //


  sActionLog += string(" succeeded on Node ") + pBestCandidate->getString() ;
  if (_bb->pContexte->getBBinterface())
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

  if (createStat)
  {
    std::vector<OB1Node*>* vect_result = pBestCandidate->startNode() ;
    int size = vect_result->size() ;
    if (size > 0)
      for (int i =0; i < size; i++)
        AskTriggerOrder(dis, 5) ;
    delete(vect_result) ;
  }

	//run trigger
  return true ;
}

// returns false if no cyclic dependancy, true if cyclic dependancy
//
bool
OB1Controler::checkForCyclicTokenDependancy(TypedVal targetedNodeName, OB1Token* pMasterToken)
{
  if (NULL == pMasterToken)
		return false ;

  BB1Object* pObject ;

try
{
	pObject = pMasterToken->getObject() ;

  if (NULL == pObject)
  	return false ;
}
catch (...)
{
	erreur("Exception OB1Controler::checkForCyclicTokenDependancy.", standardError) ;
  return true ;
}

  // First, check if the master token doesn't try to answer the same question
  //

  TypedVal* pDispinf ;

try
{
  pDispinf = pObject->PathAttributes() ;

  if (NULL == pDispinf)
  	return false ;

  if (*pDispinf == targetedNodeName)
    return true ;
}
catch (...)
{
  pMasterToken->putObject((BB1Object*) 0) ;
	erreur("Exception OB1Controler::checkForCyclicTokenDependancy.", standardError) ;
  return true ;
}

try
{
  // Then, check if the master token is not a sub-token. If yes, iterate.
  //
  OB1Token* pMasterOfMasterToken = pMasterToken->getMasterToken() ;
  if (NULL == pMasterOfMasterToken)
  	return false ;

  string sTrace = string("OB1Controler::checkForCyclicTokenDependancy iterating for targetedNodeName ") + targetedNodeName.getString() ;
  _bb->pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubSteps) ;

	return checkForCyclicTokenDependancy(targetedNodeName, pMasterOfMasterToken) ;
}
catch (...)
{
	erreur("Exception OB1Controler::checkForCyclicTokenDependancy.", standardError) ;
  return true ;
}
}

void
OB1Controler::AddResult(std::vector<INode *>* temp, OB1NResult *res)
{
try
{
	if ((NULL == temp) || (NULL == res))
		return ;

	if (temp->empty())
		return ;

  register unsigned int resSize = temp->size() ;

  for (register unsigned int i = 0 ; i < resSize ; i++)
  {
    INode* in = (*temp)[i] ;
    TypedVal label(*(in->_labeled)) ;
    OB1NOther* va      = new OB1NOther(this, &label) ;
    OB1NOther* va_temp = _bb_state.find_other_nodes(va) ;
    if (va_temp)
    {
      delete (va) ;
      va = va_temp ;
    }
    else
      _bb_state.AddVertex(va) ;

    OB1Edge *ed = new OB1Edge(CREATES, createMask(in->_classTo)) ;
    res->addBijection(new OB1NodeLink(va, ed), createMask(in->_classFrom)) ;
  }
}
catch (...)
{
	erreur("Exception BB1 OB1Controler::AddResult.", standardError) ;
}
}

// fonction utilitaire
std::vector<std::string>*
parseClass(std::string& inp)
{
try
{
  std::vector<std::string>* result = new std::vector<std::string>();
  while (inp.find(" ") != -1)
  {
    int temp = inp.find(" ");
    inp = inp.erase( temp,1);
  }
  while (inp.size() != 0)
  {
    int temp = inp.find("||");
    if (temp != -1)
      {
      result->push_back(inp.substr( 0, temp));
      inp = inp.erase( 0,temp+2);
      }
    else
      {
      result->push_back(inp);
      inp = inp.erase( 0,inp.size());
      }
  }
  return (result);
}
catch (...)
{
	erreur("Exception BB1 OB1Controler parseClass.", standardError) ;
	return NULL ;
}
}

// FIXME
// Ameliorer l'ago : le plus bourrin possible actuellment
OB1Token*
OB1Controler::find(TypedVal& temp, std::string clasName)
{
	std::string	sSearched = temp.getString() ;
  if (std::string("") == sSearched)
  	return (OB1Token*) 0 ;

	// Exact search
  //
  TypedVal searchStruct(NULLSEARCHSTRUCT) ;
  OB1NOther* nod = _bb_state.find_other_nodes(temp, searchStruct) ;

/*
  if (NULL == nod)
  {
  	//
  	// Semantic search
  	//
    std::map<std::string, OB1NOther* >* pMapOfOtherNodes = _bb_state.getOtherNodes() ;
    if (pMapOfOtherNodes->empty())
    	return NULL ;

    int iMinimalDistance = pathDistanceInfinite ;
    OB1NOther* pBestCandidate = NULL ;

    NSFilGuide* pFilGuide = BB()->pContexte->getSuperviseur()->getFilGuide() ;

    std::map<std::string, OB1NOther*>::iterator curOnOther = pMapOfOtherNodes->begin() ;
    for (; curOnOther != pMapOfOtherNodes->end() ; curOnOther++)
    {
    	std::string sSortingId = (*curOnOther).first ;
      std::string sNodePath  = GetStringFromSortingId(sSortingId) ;

      if (sNodePath != std::string(""))
      {
      	// For the moment, we don't use semantics
        //
      	int iDistance = pFilGuide->pathDistance(&sNodePath, &sSearched, false) ;
      	if (iDistance < iMinimalDistance)
        	pBestCandidate = (*curOnOther).second ;
    	}
    }

    if (pBestCandidate != NULL)
    	nod = pBestCandidate ;
  }
*/

  if (nod)
  {
  	int clas = _IClass[clasName] ;
  	OB1Token* test_tok = nod->getLastToken(clas) ;
    return (test_tok) ;
  }

  return (OB1Token*) 0 ;
}

OB1Token*
OB1Controler::findTokenFromId(long int iTokId)
{
	// return _sheduler.getTokenFromId(iTokId) ;

	if (_aTokensArray.empty())
		return (OB1Token*) 0 ;

	std::vector<OB1Token*>::iterator tokIter = _aTokensArray.begin() ;
	for ( ; _aTokensArray.end() != tokIter ; tokIter++ )
		if ((*tokIter)->_token_numero == iTokId)
    	return *tokIter ;

	return (OB1Token*) 0 ;
}

OB1Token*
OB1Controler::findSubToken(OB1Token* pMasterToken)
{
	// return _sheduler.getTokenFromId(iTokId) ;

	if ((NULL == pMasterToken) || (_aTokensArray.empty()))
		return (OB1Token*) 0 ;

	std::vector<OB1Token*>::iterator tokIter = _aTokensArray.begin() ;
	for ( ; _aTokensArray.end() != tokIter ; tokIter++ )
		if ((*tokIter)->getMasterToken() == pMasterToken)
    	return *tokIter ;

	return (OB1Token*) 0 ;
}

OB1Token*
OB1Controler::createToken(EventType type, BB1Object* pTr)
{
	OB1Token* pToken = new OB1Token(type, pTr) ;
  _aTokensArray.push_back(pToken) ;
  return pToken ;
}

OB1Token*
OB1Controler::createToken(EventType type, BB1Object* pTr, AVPSet* Change, LinkSet* chan)
{
	OB1Token* pToken = new OB1Token(type, pTr, Change, chan) ;
  _aTokensArray.push_back(pToken) ;
  return pToken ;
}

// Valid means "in the array"
//
bool
OB1Controler::isValidToken(OB1Token* pToken)
{
	if ((NULL == pToken) || (_aTokensArray.empty()))
		return false ;

	std::vector<OB1Token*>::iterator tokIter = _aTokensArray.begin() ;
	for ( ; (_aTokensArray.end() != tokIter) && (*tokIter != pToken) ; tokIter++) ;
	if (tokIter == _aTokensArray.end())
		return false ;

	return true ;
}

void
OB1Controler::deleteToken(OB1Token* pToken)
{
	if (NULL == pToken)
		return ;

  if (_bb->pContexte->getBBinterface())
  {
    string sActionLog = "Controler deletes token " + IntToString(pToken->getNumero()) ;
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
  }

	if (_aTokensArray.empty())
		return ;

	std::vector<OB1Token*>::iterator tokIter = _aTokensArray.begin() ;
	for ( ; (_aTokensArray.end() != tokIter) && (*tokIter != pToken) ; tokIter++) ;
	if (_aTokensArray.end() == tokIter)
		return ;

	// Unreference the token in the nodes
  //
	if (false == _bb_state.getAllVertex().empty())
	{
		std::vector<OB1Node*>::iterator iter = _bb_state.getAllVertex().begin() ;
  	for ( ; _bb_state.getAllVertex().end() != iter ; iter++)
  		if (*iter)
  			(*iter)->removeToken(pToken) ;
	}

  // Clear the strategy
  //
  OB1Strategy* pStrat = _sheduler.getStrategyForToken(pToken) ;

  // Just mark it as deleted, don't delete it
  //
  if (pStrat)
  {
  	pStrat->ToDelete() ;
    pStrat->putToken((OB1Token *) 0) ;
  }

  /*
  if (pStrat)
  {
  	// std::vector<OB1Strategy*>::iterator stratIter = _sheduler.findStrategy(pStrat) ;
    std::vector<OB1Strategy*>::iterator stratIter ;
    bool bStratFound = _sheduler.findStrategy(pStrat, stratIter) ;
    if (bStratFound && (NULL != stratIter))
    	_sheduler.RemoveStrategy(stratIter) ;
	}
  */

  // Delete and remove from token array
  //
	delete *tokIter ;
	_aTokensArray.erase(tokIter) ;
}

void
OB1Controler::deleteAllTokens()
{
	if (_aTokensArray.empty())
		return ;

  if (_bb->pContexte->getBBinterface())
  {
    string sActionLog = "Controler deletes all tokens" ;
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
  }

	std::vector<OB1Token*>::iterator tokIter = _aTokensArray.begin() ;
	for ( ; _aTokensArray.end() != tokIter ; )
	{
  	delete *tokIter ;
		_aTokensArray.erase(tokIter) ;
	}
}

void
OB1Controler::deleteAllTokensForObject(BB1Object* pObject)
{
	if ((NULL == pObject) || (_aTokensArray.empty()))
		return ;

  if (_bb->pContexte->getBBinterface())
  {
    string sActionLog = "Controler deletes all tokens for object " + pObject->Name() ;
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
  }

	std::vector<OB1Token*>::iterator tokIter = _aTokensArray.begin() ;
	for ( ; _aTokensArray.end() != tokIter ; )
	{
    if ((*tokIter)->getObject() == pObject)
    {
  	  // delete *tokIter ;
		  // _aTokensArray.erase(tokIter) ;
      
      deleteToken(*tokIter) ;
    }
    else
      tokIter++ ;
	}
}

// All tokens that have reached their ending point are put
// in the "deprecated" bag
//
void
OB1Controler::deprecateProcessedTokens()
{
	if (_aTokensArray.empty())
		return ;

	std::vector<OB1Token*>::iterator tokIter = _aTokensArray.begin() ;
	for ( ; _aTokensArray.end() != tokIter ; tokIter++)
  	if (true == (*tokIter)->isEnded())
    {
    	// Deprecate the Answer Object
      //
    	BB1Object* pAnswerObject = (*tokIter)->getAnswerObject() ;
      if (pAnswerObject)
      	pAnswerObject->setAnswerStatus(AnswerStatus::astatusDeprecated, (HWND) 0) ;

      string sActionLog = string("Token ") + IntToString((*tokIter)->getNumero()) + string(" deprecated") ;
      if (_bb->pContexte->getBBinterface())
      {
        _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
        sActionLog += string(" ; ") + pAnswerObject->Name() + string(" was set to Deprecated") ;
      }

      // Put the token in the "deprecated bag"
      //
    	_aDeprecatedTokensArray.push_back(*tokIter) ;
			_aTokensArray.erase(tokIter) ;
		}
}

/**
// This function resets and deletes all runing tokens.
//
// Take care having a way to create new tokens or strategy, or questions will
// get stuck in astatusProcessing status
//
**/
void
OB1Controler::resetProcessingTokens()
{
	if (_aTokensArray.empty())
		return ;

  std::vector<OB1Token*>::iterator tokIter = _aTokensArray.begin() ;
	for ( ; _aTokensArray.end() != tokIter ; )
  {
  	if (false == (*tokIter)->isEnded())
    {
    	// Deprecate the Answer Object
      //
    	BB1Object* pAnswerObject = (*tokIter)->getAnswerObject() ;
      if (pAnswerObject)
      {
        pAnswerObject->setAnswerStatus(AnswerStatus::astatusProcessing, (HWND) 0) ;
      	pAnswerObject->setAnswerProcessStage(AnswerStatus::apstageStarting) ;

        string sActionLog = string("Token ") + IntToString((*tokIter)->getNumero()) + string(" reseted ; ") + pAnswerObject->Name() + string(" was set to Processing and Starting") ;
        if (_bb->pContexte->getBBinterface())
          _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
      }

      // delete Token
      //
      deleteToken(*tokIter) ;
		}
    else
      tokIter++ ;
  }
}

std::string
OB1Controler::StrategyCount()
{
  std::string result = std::string("") ;
  char id[16] ;
  itoa(_StrategyCounter, id, 10) ;
  result.append(id) ;

  _StrategyCounter++ ;

  return (result) ;
}

// Cherche un noeud d'accueil, puis (si on l'a trouvé), lance une stratégie
//
OB1Strategy*
OB1Controler::AskDeterminicOrder(OB1Token* token, int priority)
{
	if (NULL == token)
		return NULL ;

  OB1Strategy* result = NULL ;
	//
	// On cherche si on peut placer le token sur un noeud
	//
  bool havePlace  = Dispatch(token) ;
  int  token_clas = token->Class() ;
  int  clas       = token_clas ;
	//
	// Si c'est le cas, on peut lancer une stratégie
	//
  if (true == havePlace)           /* you can run a strategy */
  {
    result = _sheduler.addStrategie(token, priority, clas) ;   // FIXME EROOR

    BB1Object* pAnswer = token->getStatusObject() ;
    if (result && pAnswer)
    	reportProcessStartToObject(pAnswer, result) ;
  }

  return (result) ;
}

OB1Strategy*
OB1Controler::AskTriggerOrder(OB1Token* token, int priority)
{
	if (NULL == token)
		return NULL ;

  OB1Strategy* result = NULL ;
  int token_clas = token->Class() ;
  int clas =  token_clas ;
  result = _sheduler.addStrategie(token, priority, clas) ;   // FIXME EROOR
  return (result) ;
}

void
OB1Controler::Execute()
{
  // _bb_state.testOtherNodes() ;

  OB1Strategy* strat2Compute = _sheduler.NextStrategyToCompute() ; /* get the strategy to execute */
  if (NULL == strat2Compute)
  	return ;

  TStrategyType stType = strat2Compute->geStratType() ;
  if      (DETERMINISTIC == stType)
  	ExecuteDeterministicAction(&strat2Compute) ;
  else if (OPPORTUNIST == stType)
  	ExecuteOpportunisticAction(strat2Compute) ;
}

// Deterministic action means that we just have the token progress one node in
// selected strategy
//
bool
OB1Controler::ExecuteDeterministicAction(OB1Strategy** temp)
{
	if (NULL == *temp)
		return true ;

	BB1BBInterface* pInterface = _bb->pContexte->getBBinterface() ;

	// This strategy was successful ; we clean it
  //
  if ((*temp)->getStrategyState() == ANSWER)
  {
    string sActionLog = string("Strategy ") + (*temp)->ID() + string(" was successful, controller sends it to garbage collector") ;
    if (pInterface)
      pInterface->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

    // New: delete the token
    //
  	OB1Token* pToken = (*temp)->Token() ;
  	if (pToken && isValidToken(pToken))
    {
      sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" is deleted since strategy ") + (*temp)->ID() + string(" was successful") ;
      if (pInterface)
        pInterface->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

    	deleteToken(pToken) ;
      (*temp)->putToken(NULL) ;
    }

    (*temp)->ToDelete() ;
    _sheduler.GarbageCollecting() ;

    return (true) ;
  }
  //
  // This strategy was unsuccessful ; we try another one
  //
  if ((*temp)->getStrategyState() == NO_ANSWER)
  {
    string sActionLog = string("Strategy ") + (*temp)->ID() + string(" was unsuccessful, controller sends it to garbage collector") ;
    if (pInterface)
      pInterface->addActionString(sActionLog, BB1BBInterface::trSteps) ;

    // Cleaning
    //
  	BB1Object* pObject = NULL ;
    HWND       hInterface = 0 ;

  	OB1Token* pToken = (*temp)->Token() ;
    OB1Token* pMasterToken = NULL ;
  	if (pToken && isValidToken(pToken))
    {
    	pMasterToken = pToken->getMasterToken() ;
  		pObject      = pToken->getObject() ;
      hInterface   = pToken->getInterfaceWindowHandle() ;

      sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" is deleted since strategy ") + (*temp)->ID() + string(" was unsuccessful") ;
      if (pInterface)
        pInterface->addActionString(sActionLog, BB1BBInterface::trSteps) ;

      deleteToken(pToken) ;
      pToken = (OB1Token *) 0 ;
      (*temp)->putToken((OB1Token *) 0) ;
    }

    (*temp)->ToDelete() ;
    _sheduler.GarbageCollecting() ;

    //
    // Try to find another strategy for the same question
    //
    if (pObject)
    {
    	// Getting question's path...
  		//
  		TypedVal* pDispinf = pObject->PathAttributes() ;
  		if (pDispinf)
      {
				TypedVal pathVal = *pDispinf ;

        string sObjectKbName = pObject->KB().Name() ;

				if      (sObjectKbName == string("QuestionKB"))
    			pToken = find(pathVal, std::string("Question")) ;
        else if (sObjectKbName == string("InformationKB"))
        	pToken = find(pathVal, std::string("Answer")) ;

				if (NULL == pToken)
      	{
      		pToken = createToken(ADD, pObject) ;

          if (pToken)
          {
      		  string sActionLog = string("Trying new strategy for object ") + pObject->Name() + string(" (creates token ") + IntToString(pToken->getNumero()) + string(")") ;
      		  pInterface->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
          }
      	}

      	pToken->setMasterToken(pMasterToken) ;
        pToken->setInterfaceWindowHandle(hInterface) ;

      	OB1Strategy* pStrat = AskDeterminicOrder(pToken, 10) ;

        // No strategy found, skip to next stage
      	//
      	if (NULL == pStrat)
        {
          string sActionLog = string("No strategy accepted token ") + IntToString(pToken->getNumero()) + string(" ; it is deleted") ;
          pInterface->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

        	deleteToken(pToken) ;
        	NextComputeAction(pObject, pToken->getMasterToken()) ;
        }
      }
    }

    return (true) ;
  }
  if ((*temp)->getStrategyState() == IN_EXECUTION)  //we continue execution
  {
		StrategyState iState = Searching(*temp) ;
		if (iState == IN_EXECUTION)
			return (false) ;
		else
    {
    	(*temp)->putComputationState(iState) ;
			return (true) ;
    }
  }
  //
  // We shouldn't be there, so we clean and quit
  //

  string sActionLog = string("Error: Strategy ") + (*temp)->ID() + string(" is not successful, not unsuccessful and not running ; sending it to garbagge collector") ;
  if (pInterface)
    pInterface->addActionString(sActionLog, BB1BBInterface::trSteps) ;

  (*temp)->inc() ;

  (*temp)->ToDelete() ;
	_sheduler.GarbageCollecting() ;
	*temp = NULL ;
  
	return (true) ;
}

StrategyState
OB1Controler::Searching(OB1Strategy* temp)
{
	if ((NULL == temp) || temp->Delete())
		return NO_ANSWER ;

	// stupid, but sometime we get a bad object here
	if (temp->ID() == string(""))
		return NO_ANSWER ;

	OB1SubStrategy* tempo = (OB1SubStrategy*) temp ;
	while (tempo && (false == tempo->getSubStrategy().empty()))
	{
		//  if (tempo->getSubStrategy().back()->getStrategyState() != NO_ANSWER)
		tempo = tempo->getSubStrategy().back() ;
	}

  // There is a node, and it is not the root (the "arrival" node)
  //
  if (tempo && (temp->Root() != tempo))
	{
  	if (tempo->Node())
    {
			switch(tempo->Node()->getType())
			{
				case KS_NODE :
        	DeterministicKS(tempo) ;
        	break ;
				case OTHER_NODE :
        	DeterministicNode(tempo) ;
        	break ;
    	}
		}
    else
			tempo->putComputationState(NO_ANSWER) ;
	}

  // Either there is no node, or it is the final one
  //
	else
	{
		if (temp->Node() && (temp->Node()->getType() == OTHER_NODE))
    {
    	// We are on the final node
      //
    	OB1NOther* other = (OB1NOther*)temp->Node() ;
  		if (other)
  		{
    		if ((other->isLeaf() == true) && (other->IsComputed() == false))
    			DeterministicNode(temp) ;
      	else
        {
        	TypedVal nodeName         = other->getPath() ;
          TypedVal nodeSearchStruct = other->getSearchStruct() ;

    			BB1Object* pAnswer = _bb->searchAnswerInKB(nodeName, &nodeSearchStruct) ;
    			if (pAnswer)
          {
    				AnswerStatus::ANSWERSTATUS iAnswerStatus = pAnswer->getAnswerStatus() ;
            if (AnswerStatus::astatusProcessed == iAnswerStatus)
            	return ANSWER ;
          }
          else
          {
          	// If the starting object is an event, then we consider that we are successfully done
            //
          	pAnswer = _bb->searchEventInKB(nodeName) ;
            if (pAnswer)
            	return ANSWER ;
          }
        }
    	}
      return NO_ANSWER ;
    }
		else if (tempo)
			tempo->putComputationState(NO_ANSWER) ;
    else
    	return NO_ANSWER ;
	}
  return IN_EXECUTION ;
}

bool
OB1Controler::DeterministicKS(OB1SubStrategy* strat)
{
	if (NULL == strat)
		return false ;

  OB1NKS* ks = (OB1NKS*)strat->Node() ;
  if (NULL == ks)
		return false ;

  BB1BBInterface* pInterface = _bb->pContexte->getBBinterface() ;

  OB1Token* pToken = (OB1Token *) 0 ;
  if (strat->Root())
  	pToken = strat->Root()->Token() ;

  string sActionLog = string("") ;

  if (pToken)
  {
    string sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" is on ks ") + ks->getKSName() ;
    pInterface->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
  }

	// No need to proceed again if the ks is already on duty
	//
	if (ks->isKSInDuty())
  {
    if (pToken)
      sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" waiting: ks ") + ks->getKSName() + string(" is on duty.") ;
    else
      sActionLog = string("Ks ") + ks->getKSName() + string(" is on duty (without token).") ;
    pInterface->addActionString(sActionLog, BB1BBInterface::trDetails) ;
		return true ;
  }

	AnswerStatus* pAnswerStatus = NULL ;

	// Without token, nothing can be done
  //
	// OB1Token* pToken = NULL ;
  // if (NULL != strat->Root())
  // 	pToken = strat->Root()->Token() ;
  if ((NULL == pToken) || isInvalidToken(pToken))
	{
  	strat->SkipToNextNode() ;
  	return false ;
	}

	// No need to go further is ks is waiting for user
	// if ((ks->KsState() == WAITING_FOR_USER_INPUT) && pToken->isMMIInDuty())
  if (false == pToken->isFree())
  {
    sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" waiting: token is not free") ;
    pInterface->addActionString(sActionLog, BB1BBInterface::trDetails) ;
		return true ;
  }

	BB1Object* pAnswer = pToken->getStatusObject() ;
	if (pAnswer)
		pAnswerStatus = pAnswer->Value("answerStatus", NULLLANSWERSTATUS) ;

  sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" compute validity for ks ") + ks->getKSName() ;
  pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

  ks->ComputeValidity(pToken) ; // Compute the validity of the concept

  sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" validity for ks ") + ks->getKSName() + string(" returns:") ;
  switch (ks->ContextValidity())
  {
    case NO_COMPUTATION : sActionLog += string(" not done yet") ; break ;
    case VALID          : sActionLog += string(" valid") ; break ;
    case PROPOSABLE     : sActionLog += string(" proposable") ; break ;
    case INVALID        : sActionLog += string(" invalid") ; break ;
  }
  pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

  switch (ks->ContextValidity())
  {
    case NO_COMPUTATION:    // We detect we are in the good case or we don't know and we consider it can be applied
    case PROPOSABLE:
    case VALID:
      if (true == ks->isActivable())
      {
/*
        // If an Archetype is specified, we have to start it if this Ks is
        // not already in the free_and_running mode
        // if (NULL != pToken)
        // {
        	string sArchetype = ks->getKS()->Value("archetype", NULLSTRING) ;
			  	// if ((sArchetype != string("")) && (ks->KsState() == WAITING_FOR_USER_INPUT) && !pToken->isMMIInDuty())
          // if ((sArchetype != string("")) && !pToken->isMMIInDuty())

          // Take care: we can't wait for the window to open, because we can
          //            come here multiple time before it happens ; this is
          //            the reason why the MMIInDuty information is mandatory
          //
          if ((sArchetype != string("")) && !pToken->bIsValidInterface())
        	{
          	// Waiting for interface to open (and connect itself to the token)
          	if (pToken->isMMIInDuty())
            	return false ;

        		pToken->setMMIInDuty(true) ;
        		string sKsName = ks->getKSName() ;
        		bb->pContexte->getSuperviseur()->Ob1AskUser(new BB1BBInterfaceForKs(pToken->getNumero(), sKsName, sArchetype), bb->pContexte) ;
          	return false ;
        	}
        // }
*/

        pToken->setRunning() ;

        sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" execute ks ") + ks->getKSName() ;
        pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

        Errcode iErrCode = ks->ExecuteKS(ks->getActionStructure(strat), pToken, true) ;

        setPostKSStatus(ks, pToken->getStatusObject(), iErrCode) ;

        if (strat->Root())
        	ks->UnFlag(strat->Root()->Priority()) ;

        sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(", ks ") + ks->getKSName() + string(" has been executed") ;
        switch (iErrCode)
        {
          case SUCCESS :
            sActionLog += string(" (success).") ;
            break ;
          case NOTHINGFOUND :
            sActionLog += string(" (nothing found).") ;
            break ;
          case FAILURE :
            sActionLog += string(" (failure).") ;
            break ;
          default :
    	      sActionLog += string(" (?).") ;
        }
        pInterface->addActionString(sActionLog, BB1BBInterface::trSteps) ;

        strat->SkipToNextNode() ;

        pToken->setRunned() ;

        // If an interface is involved, let it know that the job is done
        if (pToken->bIsValidInterface())
        {
          sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(", telling user interface that ks ") + ks->getKSName() + string(" has been executed.") ;
          pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
#ifndef _ENTERPRISE_DLL
        	HWND hInterfaceHandle = pToken->getInterfaceWindowHandle() ;
          //
          // If this ks "pilots" the interface, it sends a IDM_BBK_EVENT msg
          //
          if ((ks->KsType() == USER_WINDOWS_KS) || (ks->KsType() == USER_PREDI_KS))
            ::PostMessage(hInterfaceHandle, WM_COMMAND, IDM_BBK_EVENT, 0) ;
          else
            pInterface->askForWindowUpdate(hInterfaceHandle) ;
#endif
        }

        // Now, we can free all Questions asked by this token
        //
        sActionLog = string("Can free all questions asked by Token ") + IntToString(pToken->getNumero()) ;
        pInterface->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
        
        _bb->unsuscribeToken(pToken) ;
      }
      // Add PA
      else
      {
      	if (pAnswerStatus)
        	pAnswerStatus->referenceKsEnd(ks->getKSName(), KSAttemptHistoryElement::ksrFailure) ;

        strat->SkipToNextNode() ;
      }
      // End Add PA
      break ;

    case INVALID:           // We have to change the case
    default:                // For robustness : if ComputeValidity is berzeck,
                            //                  we don't enter an infinite loop

    	if (pAnswerStatus)
        pAnswerStatus->referenceKsEnd(ks->getKSName(), KSAttemptHistoryElement::ksrFailure) ;

    	if (strat->Root() != NULL)
      	ks->UnFlag(strat->Root()->Priority());

      strat->SkipToNextNode() ;

    	break ;
  }

  return true ;
}

bool
OB1Controler::DeterministicNode(OB1SubStrategy* strat)
{
try
{
	if (NULL == strat)
		return false ;

  BB1BBInterface* pInterface = _bb->pContexte->getBBinterface() ;

	AnswerStatus::ANSWERSTATUS iNodeAnswerStatus = AnswerStatus::astatusUnknown ;
  TypedVal nodePath ;
  TypedVal nodeSearchStruct ;
  BB1Object* pAnswer = (BB1Object *) 0 ;

  OB1NOther* other = (OB1NOther*)strat->Node() ;
  if (other)
  {
  	nodePath         = other->getPath() ;
    nodeSearchStruct = other->getSearchStruct() ;

    OB1Token* pToken = NULL ;
    if (strat->Root())
      pToken = strat->Root()->Token() ;

    string sActionLog = string("Token ") + IntToString(pToken->getNumero()) + string(" is on OtherNode ") + nodePath.getString() ;
    pInterface->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

/*
    OB1Strategy* pRootStrat = strat->Root() ;
    if (NULL != pRootStrat)
    {
    	OB1Token* pToken = pRootStrat->Token() ;
      if (NULL != pToken)
      {
      	BB1Object* pStatusObject = pToken->getStatusObject() ;
        if (NULL != pStatusObject)
        	iAnswerStatus = pStatusObject->getAnswerStatus() ;
      }
    }
*/

		// Warning: We are looking for the status of the node,
    //          not the status of the strategy
    //
		pAnswer = _bb->searchAnswerInKB(nodePath, &nodeSearchStruct) ;
    if (pAnswer)
    	iNodeAnswerStatus = pAnswer->getAnswerStatus() ;
  }

  switch (strat->getStrategyState())
  {
    case IN_EXECUTION:
      if ((true == strat->HasWantedClass()) ||
          (AnswerStatus::astatusProcessed == iNodeAnswerStatus))   // On a une reponse
      	//
      	// go forward
        //
        strat->SkipToNextNode() ;

      else
      {
      	if (other)
        {
        	switch(iNodeAnswerStatus)
          {
          	case AnswerStatus::astatusUnknown    :
            {
            	// Ask again
              //
              OB1Token* pStratToken = (OB1Token *) 0 ;
              if (strat->Root())
								pStratToken = strat->Root()->Token() ;

              if (pStratToken)
              {
              	bool bIsCyclicDependancy = checkForCyclicTokenDependancy(nodePath, pStratToken) ;
              	if (bIsCyclicDependancy)
              	{
              		strat->SkipToNextNode() ;
                	return true ;
              	}
              }
            	_bb->pContexte->getBBinterface()->ComputeQuestion(&nodePath, pStratToken, false, (HWND) 0, -1, nodeSearchStruct.getSearchPrm()) ;
            	return true ;
            }
            case AnswerStatus::astatusProcessing :
            {
            	// It means that this node is under-processing. Since we need its
              // result, we have to launch it, or we will never get any result.
            	//
              OB1Token* pNodeToken = find(nodePath, std::string("Question")) ;
              if (pNodeToken)
              {
              	OB1Strategy* pStrat = _sheduler.getStrategyForToken(pNodeToken) ;
                if (NULL == pStrat)
                	ExecuteDeterministicAction(&pStrat) ;
                else
                {
                	// If we are there, it can mean that :
                  // - this token is waiting for a subtoken to find the information (so, let it keep on working)
                  // - this token is stuck here do to a problem
                  //
                  // In the last case, we have to force the node to NO_ANSWER in
                  // order for the token to keep on its way
                  //
                  OB1Token* pSubToken = findSubToken(pNodeToken) ;
                  if (NULL == pSubToken)
                  	strat->SkipToNextNode() ;
                }
              }
              else
              {
              	OB1Token* pStratToken = NULL ;
              	if (strat->Root())
									pStratToken = strat->Root()->Token() ;

                if (pStratToken)
              	{
              		bool bIsCyclicDependancy = checkForCyclicTokenDependancy(nodePath, pStratToken) ;
              		if (bIsCyclicDependancy)
              		{
              			strat->SkipToNextNode() ;
                		return true ;
              		}
                }
            		_bb->pContexte->getBBinterface()->ComputeQuestion(&nodePath, pStratToken, false, (HWND) 0, -1, nodeSearchStruct.getSearchPrm()) ;
              }

            	return true ;
            }
            case AnswerStatus::astatusProcessed  :

            	other->Computed() ;
              return true ;
          }

        	/*
        	bool bInKsDirection = !(other->isLeaf()) ;
          int iResult = other->createStrategy(strat, bInKsDirection) ;  //We check we cannot find another strategy

          //
          // No KS seems to be able to do the job
          //
          if ((iResult != 1) || (strat->getSubStrategy().size() == 0))
          {
          	bool bMustStop = false ;

            if (true == other->isLeaf())  // If it is a leaf
            {
              if (other->IsComputed() == false) // SI on a pas déja executé la fonction par défault
              {
                bool bSuccess = other->Controler()->_DefaultFunc(other->getName(), other->Controler()->BB()->pContexte->getSuperviseur()->getBBinterface());
                if (bSuccess)
                	other->Computed() ;
                else
                {
                	int iPriority = strat->Root()->Priority() ;
                	OB1Token* pToken = strat->Token() ;
                  bool bHasPlace = DispatchAgain(pToken) ;
  								if (true == bHasPlace)
    								OB1Strategy* pResult = _sheduler.addStrategie(pToken, iPriority, pToken->Class()) ;
                  else
                  {
                  	NSPatPathoArray* respat = NULL ;

                    std::string sDaf = other->getName().getString() ;
  									std::string temp = ParseNautilusPath(sDaf) ;

										// Creationd e la reponse
                    BB1KB				*informationKB = other->Controler()->BB()->KBNamed("InformationKB") ;
                    BB1Class		*answer 	   = other->Controler()->BB()->ClassNamed("Answer") ;

                    AttValPair	label		("label", string("Default calcul")) ;
                    AttValPair	sfrom		("sfrom", string("Default Control Calculus")) ;
                    AttValPair  Explication("explication", sDaf);
                    //
                    // true : because we don't duplicate respat, so we must not delete respat
                    //
                    AttValPair node ("node", respat, true) ;

                    std::string id = "default" ;
                    char* nb = new char[10] ;
                    itoa(other->Controler()->BB()->getNBObject(), nb, 10) ;
                    id.append(nb) ;
                    delete[] nb ;

                    // création de la réponse
                    BB1AppInst *objectAnswer = answer->MakeInstance(id,Explication, *informationKB,
                                                    Collect(&label, &node, &sfrom), NULL, true) ;

                    other->Computed() ;
                  }
                }
              }
              else
              	bMustStop = true ;
            }
            else
            	bMustStop = true ;

            if (bMustStop)
            {
            	// Modif PA : si seul Father est mis en NO_ANSWER, on continue
              // à boucler sur strat (faut-il le supprimer ?)
              strat->Father()->RemoveStrategy() ;
              // strat->putComputationState(NO_ANSWER); -> BOUCLE
              // Fin modif PA
              strat->Father()->putComputationState(NO_ANSWER) ; // On a pas de reponse est on arrivera pas au bout
            }
          }
          */
        }
        else
        	strat->SkipToNextNode() ;
      }
      break ;
  	case ANSWER :
      strat->SkipToNextNode() ;
  		break ;
  }
  return (true) ;
}
catch (...)
{
	erreur("Exception OB1Controler::DeterministicNode.", standardError) ;
  return false ;
}
}

bool
OB1Controler::ExecuteOpportunisticAction(OB1Strategy* strat)
{
	if (NULL == strat)
		return false ;

  OB1Node* pStratNode = strat->Node() ;
  if (NULL == pStratNode)
  	return false ;

  OB1NKS* pKSNode = NULL ;

	if      (pStratNode->getType() == KS_NODE)
  	pKSNode = dynamic_cast<OB1NKS * >(pStratNode) ;
	else if (pStratNode->getType() == OTHER_NODE)
  {
  	// to be developped
	}

  if (NULL == pKSNode)
		return false ;

	if (pKSNode->KsState() == FREE_AND_RUNNING)
	{
  	ValidityContextType valid = pKSNode->ContextValidity() ;
    bool                bActi = pKSNode->isActivable() ;
    KSType              ksTyp = pKSNode->KsType() ;

    if ((USER_WINDOWS_KS == ksTyp) || (USER_PREDI_KS == ksTyp) ||
        (USER_PROJECT_KS == ksTyp) || (USER_KS       == ksTyp) ||
        (USER_PROJ_FU_KS == ksTyp))
    {
    	string sActionLog = string("KS node ") + pKSNode->getKSName() + string(" is candidate for execute opportunistic action") ;
      if (_bb->pContexte->getBBinterface())
        _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSteps) ;
    }

    if ((SVCE_WINDOWS_KS != ksTyp) && (bActi) && (INVALID != valid))
    // we check if it has not been answered yet
    {
    	OB1NResult* result = pKSNode->getResultNode() ;
      if (result) // we check if only one result have been answer . It's only possible when the ks has a result
      {
      	bool bResult = result->hasResult() ;
        if (false == bResult)
        	pKSNode->ExecuteKS(pKSNode->getActionStructure(), NULL, true) ;
      }
      else     /* if the ks doesn't create BB1Object then we check if the executed attributes have been used */
      {
      	MarkerObject* tempMarker = pKSNode->Attributes(std::string("executed")) ;
        if (NULL == tempMarker)
        {
        	if (NO_COMPUTATION == valid)
          {
          	pKSNode->ComputeValidity(NULL) ;
            if (pKSNode->ContextValidity() == VALID)
            	pKSNode->ExecuteKS(pKSNode->getActionStructure(), NULL, true) ;
          }
        }
      }
    }
  }
  return (true) ;
}

bool
OB1Controler::createQuestionAndAnswer(TypedVal* quest, TypedVal *pSearchStruct)
{
	if (NULL == quest)
		return false ;

	BB1Object* pQuestion = _bb->searchQuestionInKB(*quest, pSearchStruct) ;
  if (pQuestion)
		return true ;

  //
	// Create the question
	//

	// std::string sDaf = quest->getName().getString() ;
	// std::string temp = ParseNautilusPath(sDaf) ;

	BB1Class *question   = _bb->ClassNamed("Question") ;
	BB1KB	   *questionKB = _bb->KBNamed("QuestionKB") ;

  string sName = "Question" ;
	char num[18] ;
	itoa(_bb->getNBObject(), num, 10) ;
	sName.append(num) ;

	AttValPair labeldef   ("label",       "DeterministicQuestion") ;
	AttValPair path       ("question",    *quest) ;
  AttValPair interfaces ("interfaces",  NULLHANDLES) ;  // suscribed interfaces handles repository
  AttValPair tokens     ("tokens",      NULLHANDLES) ;  // tokens that asked this question
  AttValPair persistence("persistence", NULLHANDLES) ;  // persistence handles that asked this question
	AttValPair Explication("explication", *quest) ;

  NSSearchStruct *pSearchParams = (NSSearchStruct *) 0 ;
  if (pSearchStruct)
    pSearchParams = pSearchStruct->getSearchPrm() ;
  AttValPair searchParams("searchParams", pSearchParams) ;

	string sActionLog = string("New question") ;

  AVPSet* pAttSpec = Collect(&labeldef, &path, &interfaces, &tokens, &persistence, &searchParams) ;
	BB1Object* cre = question->MakeInstance(sName, Explication, *questionKB, pAttSpec, (LinkSet*) 0, false) ;
  delete pAttSpec ;

	if (cre)
	{
  	sActionLog += string(" (creation succeeded for ") + cre->Name() + string(")") ;
    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
  }
	else
	{
		sActionLog += string(" (creation failed)") ;
    if (_bb->pContexte->getBBinterface())
		  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
	}

  //
	// Create an answer for follow up
  //
  BB1Object* pAnswer = _bb->searchAnswerInKB(*quest, pSearchStruct) ;
  if (pAnswer)
  	// to do: re-init the answer
  	return true ;

	// Creation of the answer
	//
	BB1KB		 *informationKB = _bb->KBNamed("InformationKB") ;
	BB1Class *answer        = _bb->ClassNamed("Answer") ;

	AttValPair label      ("label",       string("Follow up answer")) ;
	AttValPair sfrom      ("sfrom",       string("OB1Controler::createQuestionAndAnswer")) ;
	AttValPair AnswExplica("explication", *quest) ;
	//
	// true : because we don't duplicate respat, so we must not delete respat
	//
	NSPatPathoArray* respat = NULL ;

	AttValPair node ("node", respat, true) ;

	std::string sId = "default" ;
	char nb[10] ;
	itoa(_bb->getNBObject(), nb, 10) ;
	sId.append(nb) ;

  AVPSet* pAttSpec2 = Collect(&label, &node, &sfrom, &searchParams) ;
	BB1Object* creQ = answer->MakeInstance(sId, AnswExplica, *informationKB, pAttSpec2, (LinkSet*) 0, false) ;
  delete pAttSpec2 ;

  sActionLog = string("New answer") ;
  if (NULL == creQ)
	{
  	sActionLog += string(" (creation failed)") ;
    if (_bb->pContexte->getBBinterface())
		  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;
    return false ;
  }

	sActionLog += string(" (creation succeeded for ") + creQ->Name() + string(")") ;
  if (_bb->pContexte->getBBinterface())
	  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

	creQ->setAnswerStatus(AnswerStatus::astatusProcessing, (HWND) 0) ;
  creQ->setAnswerProcessStage(AnswerStatus::apstageStarting) ;

  return true ;
}

// OB1 can we waken up by a Question, an Event, or even an Answer to a Question
//
bool
OB1Controler::NextComputeAction(BB1Object* pObject, OB1Token* pMasterToken, bool bUserIsWaiting, HWND interfaceHandle)
{
	if (NULL == pObject)
		return false ;

	string sObjectKbName = pObject->KB().Name() ;

  if (sObjectKbName == string("QuestionKB"))
		return NextComputeActionForQuestion(pObject, pMasterToken, bUserIsWaiting, interfaceHandle) ;

  if (sObjectKbName == string("InformationKB"))
  	return NextComputeActionForAnswer(pObject, pMasterToken, bUserIsWaiting, interfaceHandle) ;

	if (sObjectKbName == string("NautilusEventKB"))
  	return NextComputeActionForEvent(pObject, pMasterToken, bUserIsWaiting, interfaceHandle) ;

	return false ;
}

bool
OB1Controler::NextComputeActionForQuestion(BB1Object* pQuestion, OB1Token* pMasterToken, bool bUserIsWaiting, HWND interfaceHandle)
{
	if (NULL == pQuestion)
		return false ;

  // Getting question's path...
  //
  TypedVal* pDispinf = pQuestion->PathAttributes() ;
  if (NULL == pDispinf)
  	return false ;

	TypedVal pathVal = *pDispinf ;

	string sPath = pathVal.getString() ;
  if (NULLSTRING == sPath)
  {
  	NautilusQuestion* pNSQuest = pathVal.getNautQuest() ;
    if (pNSQuest)
    	sPath = pNSQuest->Question() + pNSQuest->Argument() ;
  }

	string sActionLog = string("Next compute action for question ") + pQuestion->Name() + string(" (") + sPath + string(")") ;
  if (_bb->pContexte->getBBinterface())
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

  // ... in order to get the corresponding answer...
  //
  BB1Object* pResponse = _bb->searchAnswerInKB(pathVal, pQuestion->SearchParams()) ;
  if (NULL == pResponse)
  	return false ;

  // ... so we can get its answer status
  //
  AnswerStatus* pAnswerStatus = pResponse->Value("answerStatus", NULLLANSWERSTATUS) ;
  if (NULL == pAnswerStatus)
  	return false ;

  // Switching to next processing stage
  //
	switch(pAnswerStatus->Status())
  {
  	case AnswerStatus::astatusUnknown    :
    	return true ;

    case AnswerStatus::astatusProcessing :
    {
    	AnswerStatus::ANSWERPROCESSINGSTAGE iProcessingStage = pAnswerStatus->ProcessingStage() ;
      iProcessingStage = getNextProcessingStageForQuestion(iProcessingStage) ;

      // If directKS is not allowed, switch to next stage
      //
      if ((AnswerStatus::apstageDirectKS == iProcessingStage) && (false == pAnswerStatus->directKsEnabled()))
      {
        if (_bb->pContexte->getBBinterface())
        {
          sActionLog = string("Stage directKS not allowed for question ") + pQuestion->Name() + string(" - switching to next stage") ;
          _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
        }

        iProcessingStage = getNextProcessingStageForQuestion(iProcessingStage) ;
      }

    	pAnswerStatus->setStage(iProcessingStage) ;

      if (AnswerStatus::apstageExited == iProcessingStage)
    		pAnswerStatus->setStatus(AnswerStatus::astatusProcessed) ;
    }
      break ;

    case AnswerStatus::astatusProcessed :
    	// to do: start another cycle, historize
      break ;
	}

  if (_bb->pContexte->getBBinterface())
    _bb->pContexte->getBBinterface()->RefreshObjectsList() ;

  if (pAnswerStatus->Status() != AnswerStatus::astatusProcessing)
  	return true ;

  // Launching the new process
  //
	switch(pAnswerStatus->ProcessingStage())
  {
  	case AnswerStatus::apstageDirectKS  :
    case AnswerStatus::apstageGenericKs :
    {
      sActionLog = string("question: ") + sPath + string(" is at processing stage ") ;
      if      (pAnswerStatus->ProcessingStage() == AnswerStatus::apstageDirectKS)
        sActionLog += string("DirectKS") ;
      else if (pAnswerStatus->ProcessingStage() == AnswerStatus::apstageGenericKs)
        sActionLog += string("GenericKs") ;
      if (_bb->pContexte->getBBinterface())
        _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

    	// Is there already a token in charge of this question ?
      //
			OB1Token* pToken = find(pathVal, std::string("Question")) ;
			if ((NULL == pToken) || isInvalidToken(pToken))
      {
      	pToken = createToken(ADD, pQuestion) ;
      	sActionLog = string("No existing token for question ") + pQuestion->Name() + string(" ; creating token ") + IntToString(pToken->getNumero()) ;
        if (_bb->pContexte->getBBinterface())
      	  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

        pToken->setInterfaceWindowHandle(interfaceHandle) ;
      }
      pToken->setMasterToken(pMasterToken) ;

      int iPriority = 10 ;
      if (bUserIsWaiting)
      	iPriority = 100 ;

      OB1Strategy* pStrat = AskDeterminicOrder(pToken, iPriority) ;

      // No strategy found, skip to next stage
      //
      if (NULL == pStrat)
      {
        sActionLog = string("Deleting token ") + IntToString(pToken->getNumero()) ;
        if (_bb->pContexte->getBBinterface())
      	  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

      	deleteToken(pToken) ;
      	return NextComputeAction(pQuestion, pMasterToken, bUserIsWaiting, interfaceHandle) ;
      }

      // Here starts an asynchronous process, since we just put a new token in OB1
      //
      return true ;
    }
    case AnswerStatus::apstageRecord    :
    {
      sActionLog = string("question: ") + sPath + string(" is at processing stage Record") ;
      if (_bb->pContexte->getBBinterface())
        _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

    	bool bFound = LeafDefaultComputation(pathVal, pQuestion->SearchParams(), interfaceHandle, _bb->pContexte->getBBinterface()) ;
      if (bFound == true)
      	return true ;
      else
      	return NextComputeAction(pQuestion, pMasterToken, bUserIsWaiting, interfaceHandle) ;
    }
  }
  return true ;
}

bool
OB1Controler::NextComputeActionForAnswer(BB1Object* pAnswer, OB1Token* pMasterToken, bool bUserIsWaiting, HWND interfaceHandle)
{
	if (NULL == pAnswer)
		return false ;

  // Getting question's path...
  //
  TypedVal* pDispinf = pAnswer->PathAttributes() ;
  if (NULL == pDispinf)
  	return false ;

	TypedVal pathVal = *pDispinf ;

	string sActionLog = string("Next compute action for answer ") + pAnswer->Name() ;
  if (_bb->pContexte->getBBinterface())
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

  // ... so we can get its answer status
  //
  AnswerStatus* pAnswerStatus = pAnswer->Value("answerStatus", NULLLANSWERSTATUS) ;
  if (NULL == pAnswerStatus)
  	return false ;

  // Switching to next processing stage
  //
	switch(pAnswerStatus->Status())
  {
  	case AnswerStatus::astatusUnknown    :
    	return true ;

    case AnswerStatus::astatusProcessing :
    {
    	AnswerStatus::ANSWERPROCESSINGSTAGE iProcessingStage = pAnswerStatus->ProcessingStage() ;
      iProcessingStage = getNextProcessingStageForAnswer(iProcessingStage) ;
    	pAnswerStatus->setStage(iProcessingStage) ;

      if (AnswerStatus::apstageExited == iProcessingStage)
    		pAnswerStatus->setStatus(AnswerStatus::astatusProcessed) ;
    }
      break ;

    case AnswerStatus::astatusProcessed :
    	// to do: start another cycle, historize
      break ;
	}

  if (pAnswerStatus->Status() != AnswerStatus::astatusProcessing)
  	return true ;

  // Launching the new process
  //
	switch(pAnswerStatus->ProcessingStage())
  {
  	case AnswerStatus::apstageDirectKS  :
    case AnswerStatus::apstageGenericKs :
    {
			OB1Token* pToken = find(pathVal, std::string("Answer")) ;
			if ((NULL == pToken) || isInvalidToken(pToken))
      {
      	pToken = createToken(ADD, pAnswer) ;
      	sActionLog += string(" (creates token ") + IntToString(pToken->getNumero()) + string(")") ;
        if (_bb->pContexte->getBBinterface())
      	  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

        pToken->setInterfaceWindowHandle(interfaceHandle) ;
      }
      pToken->setMasterToken(pMasterToken) ;

      int iPriority = 10 ;
      if (bUserIsWaiting)
      	iPriority = 100 ;

      OB1Strategy* pStrat = AskDeterminicOrder(pToken, iPriority) ;

      // No strategy found, skip to next stage
      //
      if (NULL == pStrat)
      {
        sActionLog = string("Deleting token ") + IntToString(pToken->getNumero()) ;
        if (_bb->pContexte->getBBinterface())
      	  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

      	deleteToken(pToken) ;
      	return NextComputeActionForAnswer(pAnswer, pMasterToken, bUserIsWaiting, interfaceHandle) ;
      }

      // Here starts an asynchronous process, since we just put a new token in OB1
      //
      return true ;
    }
  }
  return true ;
}

bool
OB1Controler::NextComputeActionForEvent(BB1Object* pEvent, OB1Token* pMasterToken, bool bUserIsWaiting, HWND interfaceHandle)
{
	if (NULL == pEvent)
		return false ;

  // Getting question's path...
  //
  TypedVal* pDispinf = pEvent->PathAttributes() ;
  if (NULL == pDispinf)
  	return false ;

	TypedVal pathVal = *pDispinf ;

	string sActionLog = string("Next compute action for event ") + pEvent->Name() ;
  if (_bb->pContexte->getBBinterface())
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

  // ... so we can get its answer status
  //
  AnswerStatus* pAnswerStatus = pEvent->Value("answerStatus", NULLLANSWERSTATUS) ;
  if (NULL == pAnswerStatus)
  	return false ;

	if (pAnswerStatus->ProcessingStage() == AnswerStatus::apstageStarting)
  {
  	AnswerStatus::ANSWERPROCESSINGSTAGE iProcessingStage = pAnswerStatus->ProcessingStage() ;
		iProcessingStage = getNextProcessingStageForAnswer(iProcessingStage) ;
		pAnswerStatus->setStage(iProcessingStage) ;
  }

	// Launching the new process
  //
	switch(pAnswerStatus->ProcessingStage())
  {
  	case AnswerStatus::astatusUnknown    :
    	return true ;

  	case AnswerStatus::apstageDirectKS  :
    case AnswerStatus::apstageGenericKs :
    {
			OB1Token* pToken = find(pathVal, std::string("Answer")) ;
			if ((pToken == NULL) || isInvalidToken(pToken))
      {
      	pToken = createToken(ADD, pEvent) ;
      	sActionLog += string(" (creates token ") + IntToString(pToken->getNumero()) + string(")") ;
        if (_bb->pContexte->getBBinterface())
      	  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

        pToken->setInterfaceWindowHandle(interfaceHandle) ;
      }
      pToken->setMasterToken(pMasterToken) ;

      int iPriority = 10 ;
      if (bUserIsWaiting)
      	iPriority = 100 ;

      OB1Strategy* pStrat = AskDeterminicOrder(pToken, iPriority) ;

      // No strategy found, skip to next stage
      //
      if (NULL == pStrat)
      {
        sActionLog = string("Deleting token ") + IntToString(pToken->getNumero()) ;
        if (_bb->pContexte->getBBinterface())
      	  _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

      	deleteToken(pToken) ;

      	AnswerStatus::ANSWERPROCESSINGSTAGE iProcessingStage = pAnswerStatus->ProcessingStage() ;
      	iProcessingStage = getNextProcessingStageForAnswer(iProcessingStage) ;
    		pAnswerStatus->setStage(iProcessingStage) ;

      	if (iProcessingStage == AnswerStatus::apstageExited)
    			pAnswerStatus->setStatus(AnswerStatus::astatusProcessed) ;

        if (pAnswerStatus->Status() != AnswerStatus::astatusProcessing)
  				return true ;

      	return NextComputeActionForEvent(pEvent, pMasterToken, bUserIsWaiting, interfaceHandle) ;
      }

      //
      // If a strategy was found, we must check the Precondition for its ks
      // (in order to check if this trigger is really the one this ks processes)
      //
      bool bPrecon = false ;

      OB1NKS* pKS = pStrat->getKSNode() ;
      if (pKS)
      	bPrecon = pKS->Precondition(pKS->getActionStructure(pStrat)) ;

      if (bPrecon)
      {
      	// First, check if this ks is "interface-driven"
        //
        string sArchetype = pKS->getKS()->Value("archetype", NULLSTRING) ;

        if ((string("") != sArchetype) && (false == pToken->bIsValidInterface()))
        {
        	// Waiting for interface to open (and connect itself to the token)
          if (pToken->isMMIInDuty())
          	return false ;

          // pToken->setMMIInDuty(true) ;

          // Prevent this token to move while interface opens
          pToken->setWaiting() ;

          string sKsName = pKS->getKSName() ;
          NSToDoTask *pToDo = new NSToDoTask() ;

          KSType ksType = pKS->KsType() ;
          if (USER_PROJ_FU_KS == ksType)
            pToDo->sendBB1BBDecisionTree(new BB1BBInterfaceForKs(pToken->getNumero(), sKsName, sArchetype), _bb->pContexte->getSuperviseur(), true, true) ;
          else
            pToDo->sendBB1BBInterface(new BB1BBInterfaceForKs(pToken->getNumero(), sKsName, sArchetype), _bb->pContexte->getSuperviseur(), true, true) ;
          // bb->pContexte->Ob1AskUser(new BB1BBInterfaceForKs(pToken->getNumero(), sKsName, sArchetype)) ;
        }

        // Here starts an asynchronous process, since we just put a new token in OB1
      	//
        return true ;
      }

      setPostKSStatus(pKS, pToken->getStatusObject(), BADPRECONDITIONS) ;
      deleteToken(pToken) ;

      // Try again
      //
      return NextComputeActionForEvent(pEvent, pMasterToken, bUserIsWaiting, interfaceHandle) ;
    }
  }

  return true ;
}

AnswerStatus::ANSWERPROCESSINGSTAGE
OB1Controler::getNextProcessingStageForQuestion(AnswerStatus::ANSWERPROCESSINGSTAGE iCurrentStage)
{
	switch (iCurrentStage)
  {
  	case AnswerStatus::apstageUnknown   : return AnswerStatus::apstageUnknown ;
    case AnswerStatus::apstageStarting  : return AnswerStatus::apstageDirectKS ;
    case AnswerStatus::apstageDirectKS  : return AnswerStatus::apstageRecord ;
    case AnswerStatus::apstageRecord    : return AnswerStatus::apstageGenericKs ;
    case AnswerStatus::apstageGenericKs : return AnswerStatus::apstageExited ;
    case AnswerStatus::apstageExited    : return AnswerStatus::apstageExited ;
    default                             : return AnswerStatus::apstageUnknown ;
  }
}

AnswerStatus::ANSWERPROCESSINGSTAGE
OB1Controler::getNextProcessingStageForAnswer(AnswerStatus::ANSWERPROCESSINGSTAGE iCurrentStage)
{
	switch (iCurrentStage)
  {
  	case AnswerStatus::apstageUnknown   : return AnswerStatus::apstageUnknown ;
    case AnswerStatus::apstageStarting  : return AnswerStatus::apstageDirectKS ;
    case AnswerStatus::apstageDirectKS  : return AnswerStatus::apstageGenericKs ;
    case AnswerStatus::apstageRecord    : return AnswerStatus::apstageGenericKs ;
    case AnswerStatus::apstageGenericKs : return AnswerStatus::apstageExited ;
    case AnswerStatus::apstageExited    : return AnswerStatus::apstageExited ;
    default                             : return AnswerStatus::apstageUnknown ;
  }
}

AnswerStatus::ANSWERPROCESSINGSTAGE
OB1Controler::getNextProcessingStageForEvent(AnswerStatus::ANSWERPROCESSINGSTAGE iCurrentStage)
{
	switch (iCurrentStage)
  {
  	case AnswerStatus::apstageUnknown   : return AnswerStatus::apstageUnknown ;
    case AnswerStatus::apstageStarting  : return AnswerStatus::apstageDirectKS ;
    case AnswerStatus::apstageDirectKS  : return AnswerStatus::apstageGenericKs ;
    case AnswerStatus::apstageRecord    : return AnswerStatus::apstageGenericKs ;
    case AnswerStatus::apstageGenericKs : return AnswerStatus::apstageExited ;
    case AnswerStatus::apstageExited    : return AnswerStatus::apstageExited ;
    default                             : return AnswerStatus::apstageUnknown ;
  }
}

void
OB1Controler::setPostKSStatus(OB1NKS* pKs, BB1Object* pTokenObject, Errcode iErrCode)
{
	if ((NULL == pTokenObject) || (NULL == pKs))
		return ;

  AnswerStatus* pAnswerStatus = pTokenObject->Value("answerStatus", NULLLANSWERSTATUS) ;
	if (NULL == pAnswerStatus)
		return ;

	// First, log the ks result in object's status
	//
	KSAttemptHistoryElement::KSATTEMPTRESULT iKsResult ;
	switch (iErrCode)
	{
		case SUCCESS      :
    	iKsResult = KSAttemptHistoryElement::ksrSuccess ;
      break ;
    case NOTHINGFOUND :
    case FAILURE      :
    	iKsResult = KSAttemptHistoryElement::ksrFailure ;
      break ;
    default           :
    	iKsResult = KSAttemptHistoryElement::ksrUnknown ;
  }
	pAnswerStatus->referenceKsEnd(pKs->getKSName(), iKsResult) ;

	// Then set the new object's status
  //
  string sObjectKbName = pTokenObject->KB().Name() ;

  if      (string("QuestionKB") == sObjectKbName)
  {
  	setPostKSStatusForQuestion(pAnswerStatus, iErrCode) ;

    string sActionLog = string("Question ") + pTokenObject->Name() + string(" receives post-ks status for ks ") + pKs->getKSName() ;
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  }

  else if (string("InformationKB") == sObjectKbName)
  {
  	setPostKSStatusForAnswer(pAnswerStatus, iErrCode) ;

    string sActionLog = string("Answer ") + pTokenObject->Name() + string(" received post-ks status for ks ") + pKs->getKSName() + string(" (new status ") + pAnswerStatus->getStatusAsString() + string(")") ;
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  }

	else if (string("NautilusEventKB") == sObjectKbName)
  {
  	setPostKSStatusForEvent(pAnswerStatus, iErrCode) ;

    string sActionLog = string("Event ") + pTokenObject->Name() + string(" receives post-ks status for ks ") + pKs->getKSName() ;
    _bb->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  }

  // If Answer has been processed; warn suscribed interfaces
  //
  if (AnswerStatus::astatusProcessed == pAnswerStatus->Status())
  {
    TypedVal *pPathVal = pTokenObject->PathAttributes() ;
    _bb->alertClientsForAnswerUpdate(pPathVal, (HWND) 0) ;
  }
}

void
OB1Controler::setPostKSStatusForQuestion(AnswerStatus* pAnswerStatus, Errcode iErrCode)
{
	if (NULL == pAnswerStatus)
		return ;

	if (SUCCESS == iErrCode)
  {
  	// We have an answer, job done
    //
		pAnswerStatus->setStatus(AnswerStatus::astatusProcessed) ;
		pAnswerStatus->setStage(AnswerStatus::apstageExited) ;

    if (_bb->pContexte->getBBinterface())
      _bb->pContexte->getBBinterface()->RefreshObjectsList() ;

    return ;
	}
  // No answer, keep on searching
  //
	return ;
}

void
OB1Controler::setPostKSStatusForAnswer(AnswerStatus* pAnswerStatus, Errcode iErrCode)
{
	if (NULL == pAnswerStatus)
		return ;

	if (iErrCode == SUCCESS)
  {
  	// We have an answer, job done
    //
		pAnswerStatus->setStatus(AnswerStatus::astatusProcessed) ;
		pAnswerStatus->setStage(AnswerStatus::apstageExited) ;
    return ;
	}
  // No answer, keep on searching
  //
	return ;
}

void
OB1Controler::setPostKSStatusForEvent(AnswerStatus* /* pAnswerStatus */, Errcode /* iErrCode */)
{
}

void
OB1Controler::stopKs(OB1NKS* /* pKs */)
{
}

/*
bool OB1Controler::ExecuteOpportunisticAction(OB1Strategy* temp)
{
  register unsigned int i ;
  register unsigned end = _bb_state.numVertices() ;
  for (i = 0; i < end; i++)  //for each ks
	{
		if (_bb_state[i]->getType() == KS_NODE)
		{
			OB1NKS* ks = dynamic_cast<OB1NKS*>(_bb_state[i]) ;
			if ((NULL != ks) && (ks->KsState() == FREE_AND_RUNNING))
			{
				ValidityContextType valid   = ks->ContextValidity() ;
        bool                bActi   = ks->isActivable() ;

        KSType              ksTyp   = ks->KsType() ;
        if ((ksTyp == USER_WINDOWS_KS) || (ksTyp == USER_KS))
        {
        	string sActionLog = string("KS node ") + ks->getKSName() + string(" is candidate for execute opportunistic action") ;
    			BB()->pContexte->getSuperviseur()->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSteps) ;
        }

				// if ((ksTyp != WINDOWS_KS) && (bActi) && (valid != INVALID))
        if ((ksTyp != SVCE_WINDOWS_KS) && (bActi) && (valid != INVALID))

        // we check if it has not been answered yet
        // if ((bActi) && (valid != INVALID))
				{
					OB1NResult* result = ks->getResultNode();
					if (result != NULL) // we check if only one result have been answer . It's only possible when the ks has a result
					{
						bool reuslt = result->hasResult();
						if (false == reuslt)
							ks->ExecuteKS(ks->getActionStructure(), true) ;
					}
					else     // if the ks doesn't create BB1Object then we check if the executed attributes have been used
					{
						MarkerObject* tempMarker = ks->Attributes(std::string("executed"));
						if (NULL == tempMarker)
						{
							if (valid == NO_COMPUTATION)
							{
								ks->ComputeValidity() ;
								if (ks->ContextValidity() == VALID)
									ks->ExecuteKS(ks->getActionStructure(), true) ;
							}
						}
					}
				}
			}
		}
	}
  return (true) ;
}
*/

void
OB1Controler::RunInOpportunisticMode(int nb_tour)
{
  _sheduler.runOppotunisticDuring(nb_tour) ;
}

void
OB1Controler::ReInitControler()
{
  register unsigned end = _bb_state.numVertices() ;
  for (register unsigned i = 0 ; i < end ; i++)
    _bb_state[i]->Reinit() ;
    
  _sheduler.ReInit() ;
  _StrategyCounter = 0 ;

  deleteAllTokens() ;
  _lastTokenCreated = NULL ;
}

void
OB1Controler::reportProcessStartToObject(BB1Object* pAnswer, OB1Strategy* pStrat)
{
	if ((NULL == pAnswer) || (NULL == pStrat))
		return ;

	OB1NKS* pKSNode = pStrat->getKSNode() ;
  if (NULL == pKSNode)
		return ;

	AnswerStatus* pAnswerStatus = pAnswer->Value("answerStatus", NULLLANSWERSTATUS) ;
  if (NULL == pAnswerStatus)
		return ;

	string sKsName = pKSNode->getKSName() ;

  pAnswerStatus->referenceKsStart(sKsName) ;
}

void
OB1Controler::reportProcessEndToObject(BB1Object* /* pAnswer */, OB1Strategy* /* pStrat */)
{
}

