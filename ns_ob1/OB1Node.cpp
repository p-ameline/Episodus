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
#include "ns_ob1\OB1Types.h"

#define INVERSELABELSIZE 9

const OB1EdgeType inverseLabel[9][2]= {{CREATES,IS_CREATED},
                         {IS_CREATED, CREATES},
                         {IMPLIES,IS_NEEDED},
                         {REQUIRES,IS_NEEDED},
                         {TRIGERS, TRIGERS},
                         {RESULT, RESULT},
                         {LEVEL,LEVEL},
                         {IS_VALIDATED,VALIDATED},
                         {VALIDATED,IS_VALIDATED}} ;

#include "ns_ob1\OB1Node.h"
#include "ns_ob1\BB1Object.h"
#include "ns_ob1\BB1Link.h"
#include "ns_ob1\BB1KS.h"
#include "ns_ob1\OB1Heuristix.h"
#include "ns_ob1\OB1Functor.h"
#include "ns_ob1\OB1Controler.h"
#include "ns_ob1\OB1Token.h"
#include "ns_ob1\BB1KB.h"
#include "ns_ob1\Transfer.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "ob1Lus\InterfaceLus.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
  #include "ns_ob1\Interface.h"
#endif

#include <algorithm>

long OB1NodeLink::_lObjectCount  = 0 ;

long OB1Node::_lRootObjectCount  = 0 ;
long OB1NKS::_lObjectCount       = 0 ;
long OB1NOther::_lObjectCount    = 0 ;
long OB1NAnd::_lObjectCount      = 0 ;
long OB1NTrigger::_lObjectCount  = 0 ;
long OB1NLevel::_lObjectCount    = 0 ;
long OB1NResult::_lObjectCount   = 0 ;
long OB1NValidity::_lObjectCount = 0 ;

/*---------------------------- Link method's -------------------------------- */

OB1NodeLink::OB1NodeLink(OB1Node* node, OB1Edge* temp, bool bijective)
{
  _Node       =  node ;
  _Edge       =  temp ;
  _bijective  =  bijective ;

  _lObjectCount++ ;
}

OB1NodeLink::OB1NodeLink(const OB1NodeLink& rv)
{
  _Node      = rv._Node ;
  _bijective = rv._bijective ;

  _Edge      = (OB1Edge*) 0 ;
  if (rv._Edge)
    _Edge    = new OB1Edge(*(rv._Edge)) ;

  _lObjectCount++ ;
}

OB1NodeLink&
OB1NodeLink::operator=(const OB1NodeLink& src)
{
  if (&src == this)
		return *this ;

  _Node      = src._Node ;
  _bijective = src._bijective ;

  if (_Edge)
    delete _Edge ;
  _Edge = (OB1Edge*) 0 ;

  if (src._Edge)
    _Edge = new OB1Edge(*(src._Edge)) ;

  return *this ;
}

bool
OB1NodeLink::Compare(OB1NodeLink* toComp)
{
	if ((NULL == toComp) || (NULL == _Node))
		return false ;

  return _Node->Compares(toComp->_Node) ;
}

bool
OB1NKS::Precondition(Transfert* tr)
{
	if (NULL == tr)
		return false ;

	const FunctorPrecondition* pFuncPre = _ks->PreCondition() ;
  if (NULL == pFuncPre)
  {
  	delete tr ;
  	return false ;
	}

	BB1BB* bb = _controlers->BB() ;

	_inKsProcessingDuty = true ;

	bool bIsValid = (*pFuncPre)(*bb, tr) ;

	_inKsProcessingDuty = false ;

  delete tr ;

	return bIsValid ;
}

OB1NodeLink::~OB1NodeLink()
{
  _Node = (OB1Node*) 0 ;
  if (_Edge)
    delete (_Edge) ;

  _lObjectCount-- ;
}

void
OB1NodeLink::putEdge(OB1Edge*   edge)
{
  _Edge = edge ;
}

void
OB1NodeLink::putNode(OB1Node*   nod)
{
  _Node = nod ;
}

void
OB1NodeLink::putBijective(bool bijective)
{
  _bijective = bijective ;
}

OB1EdgeType
getInverseLabel(OB1EdgeType temp)
{
	for (unsigned int i = 0 ; i < INVERSELABELSIZE ; i++)
		if (temp == inverseLabel[i][0])
			return (inverseLabel[i][1]) ;

	return UNDEFINED ;
}

/*---------------------------- BB1TCNode Method ------------------------------*/
OB1NodeLink*
OB1Node::operator() (unsigned int i) /* return the i's parent */
{
	if ((_parent.empty()) || (i >= _parent.size()))
		return (OB1NodeLink *) 0 ;

  return (_parent[i]) ;
}

OB1NodeLink*
OB1Node::operator[] (unsigned int i) /* return the i's children */
{
	if ((_children.empty()) || (i >= _children.size()))
		return (OB1NodeLink *) 0 ;

  return (_children[i]) ;
}

int
OB1Node::getTokensCardinal()
{
  return _tokens.size() ;
}

bool
OB1Node::AddChild(OB1NodeLink* link)
{
	if (NULL == link)
		return false ;

  if (false == existChild(link->Node()) )
  {
    _children.push_back(link) ;
    return (true) ;
  }
  return (false) ;
}

MarkerObject*
OB1Node::Attributes(std::string& attr)
{
  return _attributes.get(attr) ;
}

void
OB1Node::Flag(int priority)
{
  MarkerObject* priorit = _attributes.get(std::string("priority")) ;
  int value = priorit->intValue() + priority ;
  MarkerInteger* temp = new MarkerInteger(value) ;
  _attributes.set(std::string("priority"), temp) ;
}

void
OB1Node::UnFlag(int priority)
{
  MarkerObject* priorit = _attributes.get(std::string("priority")) ;
  int value = priorit->intValue() - priority ;
  MarkerInteger *temp = new MarkerInteger(value) ;
  _attributes.set(std::string("priority"),temp ) ;
}

bool
OB1Node::AddParent(OB1NodeLink* link)
{
	if (NULL == link)
		return false ;

  if (false == existParent(link->Node()))
  {
    _parent.push_back(link) ;
    return (true) ;
  }
  return (false) ;
}

OB1NKS*
OB1Node::KS()
{
  if (false == _parent.empty())
  	for (LinkIterator iter = _parent.begin() ; _parent.end() != iter ; iter++)
      if (((*iter)->Node()) && (KS_NODE == (*iter)->Node()->getType()))
      {
      	OB1NKS* result = dynamic_cast<OB1NKS*> ((*iter)->Node()) ;
        return (result) ;
      }

	return (OB1NKS*) 0 ;
}

void
OB1NOther::Computed()
{
  _DefaultCompute = true ;
}

void
OB1Node::EmptyToken()
{
	if (_tokens.empty())
		return ;

  for (std::vector< OB1Token* >::iterator iter = _tokens.begin() ; _tokens.end() != iter ; )
  {
		if (*iter)
    {
      // Prevent circular problems since, when deleted, the token will try to
      // ask this node to remove it from the array
      //
      if (this == (*iter)->Node())
        (*iter)->PutStorageNode((OB1Node *) 0) ;
			delete(*iter) ;
    }
		_tokens.erase(iter) ;
  }
}

std::vector<OB1Node*>*
OB1Node::startNode()
{
  std::vector<OB1Node *>* res = new std::vector<OB1Node *>() ;

  if (false == _parent.empty())
  	for (LinkIterator iter = _parent.begin() ; _parent.end() != iter ; iter++)
   		if (((*iter)->Node()) && ((*iter)->Node()->getType() == TRIGGER_NODE))
      	res->push_back((*iter)->Node()) ;

  return res ;
}

OB1NodeLink*
OB1Node::Children(OB1Node* toFind)
{
	if ((NULL == toFind) || (_children.empty()))
  	return (OB1NodeLink *) 0 ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  	if (((*iter)->Node()) && (true == (*iter)->Node()->Compares(toFind)))
    	return (*iter) ;

  return (OB1NodeLink *) 0 ;
}

OB1NodeLink*
OB1Node::Parent(OB1Node* toFind)
{
	if ((toFind) && (false == _parent.empty()))
		for (LinkIterator iter = _parent.begin() ; _parent.end() != iter ; iter++)
			if (((*iter)->Node()) && (true == (*iter)->Node()->Compares(toFind)))
				return (*iter) ;

  return (OB1NodeLink *) 0 ;
}

bool
OB1NValidity::CanCompute()
{
	if (_children.empty())
		return true ;

	for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  {
    OB1NodeLink* inv = Children(this) ;
    if ((inv) && inv->Edge() && (*iter)->Node())
    {
    	int invers = inv->Edge()->Class() ;
      OB1Token* last = (*iter)->Node()->getLastToken(invers) ;
      if (NULL == last)
        return (false) ;
    }
	}
  return (true) ;
}

void
OB1Node::Reinit()
{
  EmptyToken() ;
  _attributes.reset() ;

  MarkerObject* priority = new MarkerInteger(0) ;
  _attributes.insert(std::string("priority"),  priority) ;
}

void
OB1NKS::Reinit()
{
  _ValidationContext = NO_COMPUTATION ;

  OB1Node::Reinit() ;

  // Reinit the state
  if ((USER_WINDOWS_KS == _ksType) ||
      (USER_PREDI_KS   == _ksType) ||
      (USER_PROJECT_KS == _ksType) ||
      (USER_PROJ_FU_KS == _ksType))
  	_ksState = WAITING_FOR_USER_INPUT ;
  else
  	_ksState = FREE_AND_RUNNING ;
}

void
OB1NOther::Reinit()
{
  _DefaultCompute = false ;

  OB1Node::Reinit() ;
}

LinkIterator
OB1Node::findChild(OB1Node* toFind)
{
	if (_children.empty())
		return (NULL) ;

  LinkIterator compteur ;
  for (compteur = _children.begin() ; _children.end() != compteur ; compteur++)
  	if (((*compteur)->Node()) && (true == (*compteur)->Node()->Compares(toFind)))
      return (compteur) ;

  return (NULL) ;
}

LinkIterator
OB1Node::findParent(OB1Node* toFind)
{
	if (_parent.empty())
		return (NULL) ;

  LinkIterator compteur ;
  for (compteur = _parent.begin() ; _parent.end() != compteur ; compteur++)
    if (((*compteur)->Node()) && (true == (*compteur)->Node()->Compares(toFind)))
      return (compteur) ;

  return (NULL) ;
}

bool
OB1NKS::ComputeValidity(OB1Token* pToken)
{
	// La validité peut-elle être évaluée (variables "computables")
  // Can we evaluate the validity (computable variables
  //
	OB1NValidity* val = getValidityNode() ;

  // Si non, on part du principe que c'est valide
  // If not evaluable, we say it is valid
  //
	if ((NULL == val) || (false == val->CanCompute()))
  {
  	_ValidationContext = VALID ;
    return true ;
  }

  BB1BB* bb = _controlers->BB() ;

  long int iPersistanceHandle = _controlers->getNexPersistenceHandle() ;
  Transfert* trans = val->getActionStructure(iPersistanceHandle) ;

  const FunctorValidity* FuncV = _ks->ValidityContext() ;
  if (FuncV)
  {
  	_inKsProcessingDuty = true ;
    if (pToken)
    	pToken->setRunning() ;

  	_ValidationContext = (*FuncV)(*bb, trans) ;

  	_inKsProcessingDuty = false ;
    if (pToken)
    	pToken->setFree() ;
	}

  // Don't do this because it is done when deleting trans and leads to orphan
  // Questions pointers
  // bb->unsuscribePersistence(iPersistanceHandle) ;

	delete trans ;

  bb->unsuscribePersistence(iPersistanceHandle) ;

  // For robustness : if the KS is berzeck, we take care that something
  // understandable is returned
  switch (_ValidationContext)
  {
    case NO_COMPUTATION :
    case VALID :
    case PROPOSABLE :
    case INVALID :
      return true ;
    default :
      _ValidationContext = INVALID ;
      string sErreur = string("Bad validity answer for ks ") + _ksName ;
      erreur(sErreur.c_str(), standardError) ;
      return false ;
  }
}

Transfert*
OB1NValidity::getActionStructure(long int iPersistenceHandle)
{
try
{
  Transfert* result = new Transfert() ;

  if (false == _children.empty())
  	for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  	{
      OB1NOther* OTH = dynamic_cast<OB1NOther*>((*iter)->Node()) ;
    	if (OTH)
      {
        // long int iNodePersistanceHandle = _controlers->getNexPersistenceHandle() ;
    		// OTH->addActionStructure(result, (*iter)->Edge()->Class(), std::string(""), (OB1Token *) 0, iNodePersistanceHandle) ;
        OTH->addActionStructure(result, (*iter)->Edge()->Class(), std::string(""), (OB1Token *) 0, iPersistenceHandle) ;
      }
		}

  return result ;
}
catch (...)
{
	erreur("Exception OB1NValidity::getActionStructure", standardError) ;
  return NULL ;
}
}

bool
OB1NOther::isLeaf()
{
	if (_parent.empty())
    return true ;

  for (LinkIterator iter = _parent.begin() ; _parent.end() != iter ; iter++)
  {
    OB1Edge* pEdge = (*iter)->Edge() ;
    if (pEdge && ((pEdge->TypeOf() == IS_CREATED) || (pEdge->TypeOf() == TRIGERS)))
      return (false) ;
  }

  return true ;
}

bool
OB1Node::existChild(OB1Node* toFind)
{
	if (_children.empty())
    return (false) ;

  for (LinkIterator compteur = _children.begin() ; _children.end() != compteur ; compteur++)
  {
    if (((*compteur)->Node()) && (true == (*compteur)->Node()->Compares(toFind)))
      return (true) ;
  }

  return (false) ;
}

bool
OB1Node::existParent(OB1Node* toFind)
{
	if (false == _parent.empty())
  	for (LinkIterator compteur = _parent.begin() ; _parent.end() != compteur ; compteur++)
    	if (((*compteur)->Node()) && (true == (*compteur)->Node()->Compares(toFind)))
      	return (true) ;

  return (false) ;
}

void
OB1Node::htmlPrint(std::string& input)
{
	input +=  "[VERTEX : <br /> \n" ;

	input += "Nombre de Fils : " ;
	char nc[16] ;
	itoa(ChildrenCardinal(), nc, 10) ;
	input.append(nc) ;
	input += " <br />" ;

	input += "Nombre de Parents : " ;
	char np[16] ;
	itoa(ParentCardinal(), np, 10);
	input.append(np);
	input += " <br />";
  
	input += "] <br /> \n";
}

string
OB1Node::getString()
{
	return "Noeud" ;
}

/*----------------------------------------------------------------------------*/
/*------------------------ CONSTRUCTEUR --------------------------------------*/
/*----------------------------------------------------------------------------*/

OB1Node::OB1Node(OB1Controler* control)
{
  _lRootObjectCount++ ;

  _controlers = control ;
  _type       = ABSTRACT_NODE ;

  MarkerObject* priority = new MarkerInteger(0) ;
  _attributes.insert(std::string("priority"),  priority) ;
}

OB1NLevel::OB1NLevel(OB1Controler* control)
          :OB1Node(control)
{
  _type = LEVEL_NODE ;

  _lObjectCount++ ;
}

OB1NResult::OB1NResult(OB1Controler* control)
           :OB1Node(control)
{
  _type = RESULT_NODE ;

  _lObjectCount++ ;
}

OB1NTrigger::OB1NTrigger(OB1Controler* control)
            :OB1Node(control)
{
  _type = TRIGGER_NODE ;

  _lObjectCount++ ;
}

OB1NAnd::OB1NAnd(OB1Controler* control)
        :OB1Node(control)
{
  _type = AND_NODE ;

  _lObjectCount++ ;
}

OB1NOther::OB1NOther(OB1Controler* control)
          :OB1Node(control)
{
  _DefaultCompute = false ;
  _type           = OTHER_NODE ;

  _path           = (TypedVal *) 0 ;
  _searchData     = (TypedVal *) 0 ;

  _lObjectCount++ ;
}

OB1NOther::OB1NOther(OB1Controler* control, TypedVal* val)
          :OB1Node(control)
{
  _type           = OTHER_NODE ;
  _DefaultCompute = false ;

  _path           = (TypedVal *) 0 ;
  _searchData     = (TypedVal *) 0 ;

  if (val)
  {
    string sQuestion = string("") ;
    string sParams   = string("") ;
    splitString(val->getString(), &sQuestion, &sParams, '|') ;

    NSSearchStruct searchStruct ;
    searchStruct.init(sParams) ;

    _path       = new TypedVal(sQuestion) ;
    _searchData = new TypedVal(&searchStruct) ;
  }

  _lObjectCount++ ;
}

OB1NKS::OB1NKS(OB1Controler* control, std::string& name)
       :OB1Node(control)
{
  _type               = KS_NODE ;
  _ValidationContext  = NO_COMPUTATION ;
  _ksName             = std::string(name) ;
  _inKsProcessingDuty = false ;

  _lObjectCount++ ;
}

OB1NKS::OB1NKS(OB1Controler* control,std::string& temp, BB1KS* ks)
       :OB1Node(control)
{
  _ValidationContext  = NO_COMPUTATION ;
  _type               = KS_NODE ;
  _ksName             = std::string(temp) ;
  _ks                 = ks ;
  _inKsProcessingDuty = false ;

  _lObjectCount++ ;
}

OB1NValidity::OB1NValidity(OB1Controler* control)
             :OB1Node(control)
{
  _type = VALIDITY_NODE ;

  _lObjectCount++ ;
}

//------------------------ ACtivable function ----------------------------------
bool
OB1NKS::isActivable()
{
  OB1NLevel* level = getLevelNode() ;
  if (NULL == level) // no level needed for execution
    return (true) ;
  else
    return (level->isActivable()) ;
}

bool
OB1NLevel::isActivable()
{
  if (_children.empty())
  	return true ;

  bool result = true ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
    if ((*iter)->Edge()->TypeOf() == REQUIRES)
    {
      result &= (*iter)->Node()->isActivable() ;
      break ;
    }

  return result ;
}

bool
OB1NAnd::isActivable()
{
  if (_children.empty())
  	return true ;

  bool result = true ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  {
    if (((*iter)->Edge()) && ((*iter)->Node()))
    {
      int clas = (*iter)->Edge()->Class() ;
      result &= ((OB1NOther*)(*iter)->Node())->isActivableExt(clas) ;
      if (false == result)
        break ;
    }
  }

  return (result) ;
}

bool
OB1NOther::isActivableExt(int clas)
{
  OB1Token* last_tok = getLastToken(clas) ;
  if (last_tok)
		return true ;    // watch is token has anwser or if answer is different of no_anwser

	TypedVal nodePath         = getPath() ;
  TypedVal nodeSearchStruct = getSearchStruct() ;
	BB1Object* pAnswer = Controler()->BB()->searchAnswerInKB(nodePath, &nodeSearchStruct) ;
	if (pAnswer)
  {
  	AnswerStatus::ANSWERSTATUS iAnswerStatus = pAnswer->getAnswerStatus() ;
		if (AnswerStatus::astatusProcessed == iAnswerStatus)
    	return true ;
  }

  return false ;
}

//--------------------------------- DESTRUCTOR ---------------------------------

OB1Node::~OB1Node()
{
  EmptyToken() ;
  // _tokens.clear() ;

  // _attributes.Clear() ; // done in MarkSet destructor

  clearParentsLinks() ;
  clearChildrenLinks() ;

  _lRootObjectCount-- ;
}

void
OB1Node::clearParentsLinks()
{
  if (_parent.empty())
    return ;

  for (LinkIterator iter = _parent.begin() ; _parent.end() != iter ; )
  {
    if (*iter)
      delete(*iter) ;
    _parent.erase(iter) ;
  }
}

void
OB1Node::clearChildrenLinks()
{
  if (_children.empty())
    return ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; )
  {
    if (*iter)
      delete(*iter) ;
    _children.erase(iter) ;
  }
}

OB1NOther::~OB1NOther()
{
  // string sMsg = string("Deleting OB1NOther (reste ") + IntToString(_lObjectCount) + string(")") ;
  // _controlers->BB()->pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubSteps, (void*)this) ;

  if (_path)
  	delete(_path) ;
  if (_searchData)
  	delete(_searchData) ;

  _lObjectCount-- ;
}

OB1NValidity::~OB1NValidity()
{
  _lObjectCount-- ;
}

OB1NResult::~OB1NResult()
{
  _lObjectCount-- ;
}

OB1NLevel::~OB1NLevel()
{
  _lObjectCount-- ;
}

OB1NTrigger::~OB1NTrigger()
{
  _lObjectCount-- ;
}

OB1NAnd::~OB1NAnd()
{
  _lObjectCount-- ;
}

OB1NKS::~OB1NKS()
{
  _lObjectCount-- ;
}

//------------------------------ Tokens functions ------------------------------

void
OB1Node::addToken(OB1Token* token)
{
	if (token)
  	_tokens.push_back(token) ;
}

void
OB1Node::removeToken(OB1Token* token)
{
	if ((NULL == token) || (_tokens.empty()))
		return ;

	for (std::vector<OB1Token* >::iterator iter = _tokens.begin() ; _tokens.end() != iter ; )
  	if ((*iter) == token)
    	_tokens.erase(iter) ;
    else
    	iter++ ;
}

OB1Token*
OB1Node::getLastToken(int clas)
{
  if (_tokens.empty())
  	return (OB1Token *) 0 ;

  for (std::vector<OB1Token* >::reverse_iterator iter = _tokens.rbegin() ; _tokens.rend() != iter ; iter++)
  	if ((*iter)->Class() == clas)
    	return (*iter) ;

  return (OB1Token *) 0 ;
}

bool
OB1Node::isReferencedToken(OB1Token* token)
{
	if ((NULL == token) || (_tokens.empty()))
  	return false ;

  for (std::vector<OB1Token* >::reverse_iterator iter = _tokens.rbegin() ; _tokens.rend() != iter ; iter++)
  	if ((*iter) == token)
    	return true ;

  return false ;
}

//------------------------------ Visitor Functions -----------------------------

// Si le chemin de référence du noeud est dispat, on place le token sur ce noeud
bool
OB1NOther::IsPlace(OB1Token* temp, TypedVal& /* dispat */)
{
	if (NULL == temp)
		return false ;

  // if (*name != dispat)
  // 	return false ;

  addToken(temp) ;
  temp->PutStorageNode(this) ;
  return true ;
}


bool
OB1Node::hasParent(OB1NodeTypes temp)
{
	if (_parent.empty())
  	return false ;

  for (LinkIterator iter = _parent.begin() ; _parent.end() != iter ; iter++)
  	if (((*iter)->Node()) && ((*iter)->Node()->getType() == temp))
    	return true ;

  return false ;
}


//------------------------------------------------------------------------------

bool
testKS(OB1Node* temp, OB1Node* temp2)
{
	if ((NULL == temp) || (NULL == temp2))
  	return false ;

  OB1NKS *temp11 = dynamic_cast<OB1NKS* >(temp) ;
  OB1NKS *temp12 = dynamic_cast<OB1NKS* >(temp2) ;

  if ((NULL == temp11) || (NULL == temp12))
  	return false ;

  return (temp11->getKSName() == temp12->getKSName()) ;
}

bool
testOther(OB1Node* temp, OB1Node* temp2)
{
	if ((NULL == temp) || (NULL == temp2))
  	return false ;

  OB1NOther *temp11 = dynamic_cast<OB1NOther* >(temp) ;
  OB1NOther *temp12 = dynamic_cast<OB1NOther* >(temp2) ;

  if ((NULL == temp11) || (NULL == temp12))
  	return false ;

  TypedVal& one = temp11->getPath() ;
  TypedVal& two = temp12->getPath() ;
  
  return (one == two) ;
}

bool testAnd(OB1Node* temp, OB1Node* temp2)
{
	if ((NULL == temp) || (NULL == temp2))
  	return false ;

	OB1NAnd* pAndTemp  = dynamic_cast<OB1NAnd* >(temp) ;
  OB1NAnd* pAndTemp2 = dynamic_cast<OB1NAnd* >(temp2) ;

  if ((NULL == pAndTemp) || (NULL == pAndTemp2))
  	return false ;

  return (temp == temp2) ;
}

bool testTrigger(OB1Node* temp, OB1Node* temp2)
{
	if ((NULL == temp) || (NULL == temp2))
  	return false ;

  OB1NTrigger* pTrigerTemp  = dynamic_cast<OB1NTrigger* >(temp) ;
  OB1NTrigger* pTrigerTemp2 = dynamic_cast<OB1NTrigger* >(temp2) ;

  if ((NULL == pTrigerTemp) || (NULL == pTrigerTemp2))
  	return false ;

  return (pTrigerTemp->KS() == pTrigerTemp2->KS()) ;
}

bool testValidity(OB1Node* temp, OB1Node* temp2)
{
	if ((NULL == temp) || (NULL == temp2))
  	return false ;

  OB1NValidity* pValidityTemp  = dynamic_cast<OB1NValidity* >(temp) ;
  OB1NValidity* pValidityTemp2 = dynamic_cast<OB1NValidity* >(temp2) ;

  if ((NULL == pValidityTemp) || (NULL == pValidityTemp2))
  	return false ;

  return (pValidityTemp->KS() == pValidityTemp2->KS()) ;
}

bool testLevel(OB1Node* temp, OB1Node* temp2)
{
	if ((NULL == temp) || (NULL == temp2))
  	return false ;

  OB1NLevel* pLevelTemp  = dynamic_cast<OB1NLevel* >(temp) ;
  OB1NLevel* pLevelTemp2 = dynamic_cast<OB1NLevel* >(temp2) ;

  if ((NULL == pLevelTemp) || (NULL == pLevelTemp2))
  	return false ;

  return (pLevelTemp->KS() == pLevelTemp2->KS()) ;
}

bool testResult(OB1Node* temp, OB1Node* temp2)
{
	if ((NULL == temp) || (NULL == temp2))
  	return false ;

	OB1NResult* pResultTemp  = dynamic_cast<OB1NResult* >(temp) ;
  OB1NResult* pResultTemp2 = dynamic_cast<OB1NResult* >(temp2) ;

  if ((NULL == pResultTemp) || (NULL == pResultTemp2))
  	return false ;

  return (pResultTemp->KS() == pResultTemp2->KS()) ;
}

typedef bool F(OB1Node* temp, OB1Node* temp2) ;
typedef F* func;
func castTab[] = {testKS, testTrigger, testLevel, testResult, testOther, testAnd, testValidity} ;

bool
OB1Node::Compares(OB1Node* test)
{
  if (NULL == test)
    return false ;

  if (test->_type == _type)
  	return  castTab[_type](this, test) ;

  return (false) ;
}

//------------------------------------------------------------------------------

OB1NResult*
OB1NKS::getResultNode()
{
  if (_children.empty())
  	return (OB1NResult *) 0 ;

  OB1NResult* result = (OB1NResult *) 0 ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
    if (((*iter)->Node()) && ((*iter)->Node()->getType() == RESULT_NODE))
    {
      result = (OB1NResult*)(*iter)->Node() ;
      break ;
    }

  return (result) ;
}

OB1NValidity*
OB1NKS::getValidityNode()
{
  if (_children.empty())
  	return (OB1NValidity *) 0 ;

  OB1NValidity* valid = (OB1NValidity *) 0 ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
    if (((*iter)->Node()) && ((*iter)->Node()->getType() == VALIDITY_NODE))
    {
      valid = (OB1NValidity*)(*iter)->Node() ;
      break ;
    }

  return (valid) ;
}

OB1NLevel*
OB1NKS::getLevelNode()
{
  if (_children.empty())
  	return (OB1NLevel *) 0 ;

  OB1NLevel* result = (OB1NLevel *) 0 ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
    if (((*iter)->Node()) && ((*iter)->Node()->getType() == LEVEL_NODE))
    {
      result = (OB1NLevel* )(*iter)->Node() ;
      break ;
    }

  return (result) ;
}

OB1NTrigger*
OB1NKS::getTriggerNode()
{
  if (_children.empty())
  	return (OB1NTrigger *) 0 ;

  OB1NTrigger* result = (OB1NTrigger *) 0 ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
    if (((*iter)->Node()) && ((*iter)->Node()->getType() == TRIGGER_NODE))
    {
      result = (OB1NTrigger*)(*iter)->Node() ;
      break ;
    }

  return (result) ;
}

/*--------- Fucntion servant a recuper la structure pour les actions ---------*/
Transfert*
OB1NKS::getActionStructure(OB1SubStrategy* strat)
{
  Transfert* result = new Transfert() ;

  // First, insert the question object
  //
  if (strat->Root())
	{
  	OB1Token* pToken = strat->Root()->Token() ;
  	if ((pToken) && (pToken->getObject()))
    {
      long int iPersistanceHandle = _controlers->getNexPersistenceHandle() ;
  		result->setAskedQuestion(pToken->getObject(), iPersistanceHandle) ;
    }
	}

  //
  // Init Result network
  //
  OB1NLevel* temp = getLevelNode() ;
  if (temp)
  	temp->getActionStructure(result) ;

/*
  OB1NResult* pResultNode = getResultNode() ;
  if (pResultNode)
  	pResultNode->getActionStructure(result, strat) ;

  OB1NTrigger* pTriggerNode = getTriggerNode() ;
  if (pTriggerNode)
  	pTriggerNode->getActionStructure(result) ;
*/

  return (result) ;
}

Transfert*
OB1NKS::getActionStructure(int i)
{
  Transfert* result = new Transfert() ;
  OB1Node* temp = getLevelNode() ;
  if (temp)
  {
    OB1NLevel* leveltemp = dynamic_cast<OB1NLevel*>(temp) ;
    if (leveltemp)
      leveltemp->getActionStructure(result, i) ;
  }
  return (result) ;
}

void
OB1NLevel::getActionStructure(Transfert* temp)
{
	if (_children.empty())
		return ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  {
    OB1NAnd* and = (OB1NAnd*) (*iter)->Node() ;
    if (and)
      and->getActionStructure(temp) ;
  }
}

void
OB1NLevel::getActionStructure(Transfert* temp, int pas)
{
	if (_children.empty())
  	return ;

  int i = 0 ;
  for (LinkIterator iter = _children.begin() ;
                      (_children.end() != iter) && (i <= pas) ; iter++, i++)
  {
    OB1NAnd* and = (OB1NAnd*) (*iter)->Node() ;
    if (and)
      and->getActionStructure(temp) ;
  }
  // ((OB1NAnd*)(*iter)->Node())->getActionStructure(temp) ;
}

void
OB1NResult::getActionStructure(Transfert* temp, OB1SubStrategy* strat)
{
	if ((NULL == temp) || (_children.empty()))
		return ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  {
    OB1NOther* OTH = dynamic_cast<OB1NOther*>((*iter)->Node()) ;
    if (OTH)
		{
    	OB1Token* pToken = NULL ;

    	if (strat)
      {
      	OB1Strategy* pRootStrat = strat->Root() ;
      	if ((pRootStrat) && (OTH->isReferencedToken(pRootStrat->Token())))
      		pToken = pRootStrat->Token() ;
      }

      long int iNodePersistanceHandle = _controlers->getNexPersistenceHandle() ;
      OTH->addActionStructure(temp, (*iter)->Edge()->Class(), std::string("Result:"), pToken, iNodePersistanceHandle) ;
    }
  }
}

void
OB1NTrigger::getActionStructure(Transfert* temp)
{
	if ((NULL == temp) || (_children.empty()))
		return ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  {
    OB1NOther* OTH = (OB1NOther*)(*iter)->Node();
    if (OTH)
      OTH->addActionStructure(temp, (*iter)->Edge()->Class(), std::string("Trigger:"));
  }
}

void
OB1NAnd::getActionStructure(Transfert* temp)
{
	if ((NULL == temp) || (_children.empty()))
		return ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  {
    OB1NOther* OTH = (OB1NOther*)(*iter)->Node() ;
    if (OTH)
      OTH->addActionStructure(temp, (*iter)->Edge()->Class()) ;
  }
}

BB1Object*
OB1NOther::getAnswerObject()
{
	if (NULL == _controlers)
		return (BB1Object*) 0 ;

	BB1BB* bb = _controlers->BB() ;
  if (NULL == bb)
		return (BB1Object*) 0 ;

	BB1Object* pAnswer = bb->searchAnswerInKB(*_path, _searchData) ;

  return pAnswer ;
}

BB1Object*
OB1NOther::getAnswerObjectIfProcessed()
{
	BB1Object* pAnswer = getAnswerObject() ;
  if (NULL == pAnswer)
		return (BB1Object*) 0 ;

	if (pAnswer->getAnswerStatus() == AnswerStatus::astatusProcessed)
  	return pAnswer ;

	return (BB1Object*) 0 ;
}

void
OB1NOther::addActionStructure(Transfert* temp, int clas, std::string sPrefixe, OB1Token* pToken, long int iPersistenceHandle)
{
	if (NULL == temp)
		return ;

/* Previous code (David): the answer object was accessed through the token

  OB1Token* lasts = NULL ;
	if (NULL == pToken)
  	lasts = getLastToken(clas) ;  // get the last token
  else
		lasts = pToken ;
  if (!lasts)
		return ;

  BB1Object* obj = lasts->getObject() ; // get the question
  if (!obj)
		return ;
*/

	// New code: the answer object is directly accessed through it's path

	BB1Object* obj = getAnswerObjectIfProcessed() ;
  if (obj)
    Controler()->BB()->suscribePersistenceToQuestion(obj, iPersistenceHandle) ;

	string sTokenNumber = string("0") ;

	OB1Token* lasts = (OB1Token*) 0 ;
	if (NULL == pToken)
  	lasts = getLastToken(clas) ;  // get the last token
  else
		lasts = pToken ;

  if (lasts)
  {
  	sTokenNumber = IntToString((int)lasts->getNumero()) ;

    // If there is no answer object, the get the question
    //
    if (NULL == obj)
    	obj = lasts->getObject() ;
  }

	if (NULL == obj)
  {
    if (-1 != iPersistenceHandle)
    {
      TypedVal nodeName         = getPath() ;
      TypedVal nodeSearchStruct = getSearchStruct() ;

      // First, check if object is not already being processed
      //
      obj = getAnswerObject() ;
      if (obj)
      {
        Controler()->BB()->suscribePersistenceToQuestion(obj, iPersistenceHandle) ;
#ifndef _ENTERPRISE_DLL
        while (AnswerStatus::astatusProcessing == obj->getAnswerStatus())
          Controler()->BB()->pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
#endif
      }
      else
      {
        BB1BBInterface* pInterface = Controler()->BB()->pContexte->getBBinterface() ;
        if (NULL == pInterface)
		      return ;

        pInterface->ComputeQuestion(&nodeName, (OB1Token *) 0, true, (HWND) 0, iPersistenceHandle, nodeSearchStruct.getSearchPrm()) ;

        obj = getAnswerObjectIfProcessed() ;
        if (NULL == obj)
        {
          addActionStructure(temp, clas, sPrefixe, pToken, iPersistenceHandle) ;
          return ;
        }
      }
    }
    else
		  return ;
  }

	temp->_transfer_class += sTokenNumber + string("-") ;
	temp->insertItem(sPrefixe + _path->toString(), _searchData->getSearchPrm(), obj, iPersistenceHandle) ;
}

OB1Controler*
OB1Node::Controler()
{
  return _controlers ;
}

Errcode
OB1NKS::ExecuteKS(Transfert* tr, OB1Token* /* pToken */, bool /* createToken */)
{
try
{
	if (NULL == tr)
		return FAILURE ;

	if (NULL == _ks)
	{
  	delete tr ;
		return FAILURE ;
	}

	BB1BB* temp = _controlers->BB() ;
	if (NULL == temp)
	{
  	delete tr ;
		return FAILURE ;
	}

  Errcode iErrCode = FAILURE ;

	if ( (_attributes.hasStringAttributes("transfert_execute", tr->_transfer_class) == false) ||
       (SVCE_WINDOWS_KS == _ksType)  ||
       (KS_SERVICE      == _ksType)
	   )
	{
    const FunctorAction* FuncA = _ks->Action() ;
    if (FuncA)
		{
			MarkerInteger* top = new MarkerInteger(1) ;
			_attributes.insert(std::string("executed"), top) ;

			_attributes.insert(std::string("transfert_execute"), new MarkerString(tr->_transfer_class)) ;

      _inKsProcessingDuty = true ;
      // if (pToken)
      //	pToken->setRunning() ;  // Now done by controler

			iErrCode = (*FuncA)(*temp, tr, false) ;

      _inKsProcessingDuty = false ;
      // if (pToken)
      //  pToken->setRunned() ;   // Now done by controler
		}
	}

  delete tr ;

  return iErrCode ;
}
catch (...)
{
	erreur("Exception OB1NKS::ExecuteKS", standardError) ;
  if (tr)
  	delete tr ;
  return FAILURE ;
}
}

//------------------ADD Bijection function -------------------------------------
void OB1Node::addBijection(OB1NodeLink* link, int clas)
{
	if (NULL == link)
		return ;

try
{
  AddChild(link) ;

  link->putBijective(true) ;
  
  OB1Edge* reci = new OB1Edge(getInverseLabel(link->Edge()->TypeOf()), clas) ;
  OB1NodeLink* linkinverse = new OB1NodeLink(this, reci, true) ;
  link->Node()->AddParent(linkinverse) ;
}
catch (...)
{
	erreur("Exception OB1Node::addBijection", standardError) ;
}
}

//---------------PRINT METHOD --------------------------------------------------
void
OB1NResult::htmlPrint(std::string& input)
{
  input += "<table width ='100%' border='1' bgcolor ='gray'>" ;
  input += "<tr><td bgcolor='996600'>BB1TCNodeResult</td></tr>" ;
  printSonAndParent(this,input) ;
  input +="</table> <br /><br />" ;
}

string
OB1NResult::getString()
{
	return "NodeResult" ;
}

void
OB1NLevel::htmlPrint(std::string& input)
{
  input += "<table width ='100%' border='1' bgcolor ='gray'>";
  input +="<tr><td bgcolor='996600'>BB1TCNodeLevel</td></tr>";
  printSonAndParent(this,input);
  input +="</table> <br /><br />";
}

string
OB1NLevel::getString()
{
	return "NodeLevel" ;
}

void
OB1NTrigger::htmlPrint(std::string& input)
{
  input += "<table width ='100%' border='1' bgcolor ='gray'>" ;
  input += "<tr><td  bgcolor='996600'> BB1TCNodeTrigger</td></tr>" ;
  printSonAndParent(this,input) ;
  input += "</table> <br /><br />" ;
}

string
OB1NTrigger::getString()
{
	return "NodeTrigger" ;
}

void OB1NOther::htmlPrint(std::string& input)
{
  input += "<table width ='100%' border='1' bgcolor ='gray'>" ;
  input += "<tr><td  bgcolor='996600'> BB1TCNodeOther:" + _path->toString() + "</td></tr>" ;
  printSonAndParent(this, input) ;
  input += "</table> <br /><br />" ;
}

string
OB1NOther::getString()
{
  string sResult = "NodeOther " + getPath().toString() ;

  NSSearchStruct* pSearchStruct = getSearchStruct().getSearchPrm() ;
  if (pSearchStruct && (string("") != pSearchStruct->_sStartingDatePattern))
    sResult += string(" | D") + pSearchStruct->_sStartingDatePattern ;

	return sResult ;
}

void OB1NAnd::htmlPrint(std::string& input)
{
  input += "<table width ='100%' border='1' bgcolor ='gray'>" ;
  input += "<tr><td  bgcolor='996600'>BB1TCNodeAnd</td></tr>" ;
  printSonAndParent(this, input) ;
  input += "</table> <br /><br />" ;
}

string
OB1NAnd::getString()
{
	return "NodeAnd" ;
}

void OB1NValidity::htmlPrint(std::string& input)
{
  input += "<table width ='100%' border='1' bgcolor ='gray'>" ;
  input += "<tr><td  bgcolor='996600'>BB1TCNValidity</td></tr>" ;
  printSonAndParent(this,input) ;
  input += "</table> <br /><br />" ;
}

string
OB1NValidity::getString()
{
	return "NodeValidity" ;
}

void
OB1NKS::htmlPrint(std::string& input)
{
  input += "<table width ='100%' border='1' bgcolor ='gray'>" ;
  input += "<tr><td  bgcolor='996600'>BB1TCNodeKS:" + _ksName +"</td></tr>" ;
  printSonAndParent(this,input) ;
  input += "</table> <br /><br />" ;
}

string
OB1NKS::getString()
{
	return "NodeKS " + _ksName ;
}

void
printSonAndParent(OB1Node* node, std::string& input)
{
  char temp1[16] ;
  char temp2[16] ;
  if (node->getTokensCardinal() != 0)
  {
    char temp3[16] ;
    itoa( node->getTokensCardinal(), temp3, 10) ;
    input +="<tr><td  bgcolor='996602'>Number of token:" ;
    input.append(temp3) ;
    input += "</td></tr>" ;
  }

  itoa( node->ChildrenCardinal(), temp1, 10);
  itoa( node->ParentCardinal(), temp2, 10);
  input +=  "<tr><td><table width ='100%' bgcolor='33ffff'>";
	input += "<tr><td bgcolor='336600'>nombre de fils: ";
	input.append(temp1);
	input += "</td></tr>";
	unsigned int end = node->ChildrenCardinal();
	for (unsigned int  i =0; i < end; i++)
	{
		input +=  "<tr><td>" +  PrintBB1NodeType((*node)[i]->Node()->getType() );
		if (KS_NODE == (*node)[i]->Node()->getType())
		{
			OB1NKS* kstemp = dynamic_cast<OB1NKS*>( (*node)[i]->Node() );
			if (kstemp != NULL)
				input += ":" +  (kstemp)->getKSName();
		}
		if (OTHER_NODE ==  (*node)[i]->Node()->getType())
		{
    	OB1NOther* ksoth = dynamic_cast< OB1NOther*>( (*node)[i]->Node() );
      if (ksoth != NULL)
      	input += ":" + ksoth->getPath().toString();
		}
		input += "<br /> Link Type : " + PrintBB1EdgeType( (*node)[i]->Edge()->TypeOf()  );
		char temp4[16] ;
		itoa( (*node)[i]->Edge()->Class(), temp4, 10) ;
		input +=  "<br /> Class allowed : " ;
		input.append(temp4) ;
		input +="</td></tr>" ;
	}
  input +="</table></td></tr><tr><td><table width ='100%' bgcolor='ff99ff'>";

  input += "<tr><td bgcolor='336600'>nombre de parents:";
  input.append(temp2);
  input += "</td></tr>";
  unsigned int end2 = node->ParentCardinal();
  for (unsigned int i =0 ; i < end2 ; i++)
  {
    input += "<tr><td>" + PrintBB1NodeType((*node)(i)->Node()->getType() );
    if (KS_NODE == (*node)(i)->Node()->getType())
    {
      OB1NKS* kstemp = dynamic_cast<OB1NKS*>( (*node)(i)->Node() );
      if (kstemp != NULL)
        input += ":" +  (kstemp)->getKSName();
    }
    if (OTHER_NODE ==  (*node)(i)->Node()->getType())
    {
      OB1NOther* ksoth = dynamic_cast< OB1NOther*>( (*node)(i)->Node() );
      if (ksoth != NULL)
        input += ":" + ksoth->getPath().toString();
    }
    input += "<br /> Link Type : " + PrintBB1EdgeType( (*node)(i)->Edge()->TypeOf()  );
    char temp4[16] ;
    itoa((*node)(i)->Edge()->Class(), temp4, 10) ;
    input +=  "<br /> Class allowed : ";
    input.append(temp4);
    input +="</td></tr>";
  }
  input += "</table></td></tr>" ;
}

std::string
PrintBB1EdgeType(OB1EdgeType temp)
{
  switch(temp)
  {
    case CREATES      : return ("creates") ;
    case IS_CREATED   : return ("is-create") ;
    case IMPLIES      : return ("implies") ;
    case REQUIRES     : return ("recquires") ;
    case IS_NEEDED    : return ("is-needed") ;
    case TRIGERS      : return ("triggers") ;
    case RESULT       : return ("result") ;
    case LEVEL        : return ("level") ;
    case UNDEFINED    : return ("undefined") ;
    case IS_VALIDATED : return ("is_validated") ;
    case VALIDATED    : return ("validated") ;
    default           : return ("Label definition Error") ;
  };
}

std::string
PrintBB1NodeType(OB1NodeTypes temp)
{
  switch(temp)
  {
    case KS_NODE          : return ("KS_NODE") ;
    case TRIGGER_NODE     : return ("TRIGGER_NODE") ;
    case LEVEL_NODE       : return ("LEVEL_NODE") ;
    case RESULT_NODE      : return ("RESULT_NODE") ;
    case OTHER_NODE       : return ("OTHER_NODE") ;
    case AND_NODE         : return ("AND_NODE") ;
    case ABSTRACT_NODE    : return ("ABSTRACT_NODE") ;
    case VALIDITY_NODE    : return ("VALIDITY_NODE") ;
    default               : return ("UNDEFINED") ;
  };
}

 /*
bool     OB1NKS::hasAlreadyBeAnswer()
{
  OB1NResult* temp = getResultNode();
  if (NULL != temp)
    return (temp->hasAlreadyBeAnswer());
  return (false);
} */


bool OB1NResult::hasResult()
{
	if (_children.empty())
		return (false) ;

  for (LinkIterator iter = _children.begin() ; _children.end() != iter ; iter++)
  {
    if (((*iter)->Edge()) && ((*iter)->Node()))
    {
      int clasAllowed = (*iter)->Edge()->Class() ;
      OB1Token* temp = (*iter)->Node()->getLastToken(clasAllowed) ;
      if (temp)
        return (true) ;
    }
  }

  return (false) ;
}

int
OB1NOther::createStrategy(OB1SubStrategy* strat, bool bInRootDirection) /* this algorithme forbid the use of one path for trigger and result */
{
try
{
	if (NULL == strat)
		return 0 ;

	OB1Strategy* rootstrat = (OB1Strategy*)strat->Root() ; /* get the root strategy */
  if (NULL == rootstrat)
		return 0 ;

	BB1BBInterface* pInterface = Controler()->BB()->pContexte->getBBinterface() ;
  if (NULL == pInterface)
		return 0 ;

  string sActionLog = string("Create strategy on Nother node ") + getString() + string(" for root strategy ") + rootstrat->ID() ;
  pInterface->addActionString(sActionLog, BB1BBInterface::trDetails) ;

	Attribute().insert(rootstrat->ID(), new MarkerInteger(0)) ;

  //if (!bInRootDirection)
		strat->putNode(this) ;

  // If we are not going in the KSNode direction, we are on a leaf. Job is done
  //
  if (false == bInRootDirection)
  	return 1 ;

  // If we are not on a leaf, it means that we have to go up toward the KSNode.
  // In order not to cycle undefinitely, we must return 0 if we have not been
  // able to do it.
  //
  int iAction = 0 ;

  if (_parent.empty())
	{
  	sActionLog = string("Unable to create a strategy for parents nodes of node ") + getString() + string(" (no parents)") ;
		pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  	return (iAction) ;
	}

  std::vector<OB1Node*> explore_choice ;

	sActionLog = string("Create strategy for parents nodes of node ") + getString() ;
	pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

/*
	for (LinkIterator iter = _parent.begin() ; iter != _parent.end() ; iter++)
	{
		OB1EdgeType ParentType = (*iter)->Edge()->TypeOf() ;

		if ((TRIGERS == ParentType) && (false == (*iter)->Node()->Attribute().Exist(rootstrat->ID())))
		{
    	sActionLog = string("The inter-node type is TRIGERS and the parent doesn't already have strategy ") + rootstrat->ID() + string(" in its attributes") ;

      if (strat->Class() == OB1_UNINITIALIZED)
      {
      	OB1NodeLink* un = (*iter)->Node()->Children(this) ;
        if (un)
        {
        	int inverse = un->Edge()->Class() ;

          sActionLog += string("; strat class is \"uninitialized\" and the parent has a \"children\" link going to this node: we create a strategy for the parent with class ") + IntToString(inverse) ;

          // OB1SubStrategy* newSubStrat = new OB1SubStrategy(strat,rootstrat, 0,  inverse);
          OB1SubStrategy* newSubStrat = new OB1SubStrategy(_controlers, rootstrat, strat, 0,  inverse) ;
          strat->AddSubStrategy(newSubStrat) ;
          (*iter)->Node()->createStrategy(newSubStrat, true) ;
          iAction = 1 ;
        }
      }
      else
      	if (strat->Class() == (*iter)->Edge()->Class())
      {
      	OB1NodeLink* un = (*iter)->Node()->Children(this) ;
        if (un)
        {
        	int inverse = un->Edge()->Class() ;

          sActionLog += string("; strat class corresponds to inter-nodes class and the parent has a \"children\" link going to this node: we create a strategy for the parent with class ") + IntToString(inverse) ;

          // OB1SubStrategy* newSubStrat = new OB1SubStrategy(strat, rootstrat, 0, inverse);
          OB1SubStrategy* newSubStrat = new OB1SubStrategy(_controlers, rootstrat, strat, 0, inverse) ;
          strat->AddSubStrategy(newSubStrat) ;
          int iAttachedToNode = (*iter)->Node()->createStrategy(newSubStrat, true) ;
          iAction = 1 ;
        }
      }

      pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
    }

		if ((TRIGERS == ParentType) && (false == (*iter)->Node()->Attribute().Exist(rootstrat->ID())))
		{
    	OB1Token* tempoN = temp->Token() ;
			if (NULL != tempoN)

    }

    if  (IS_CREATED == ParentType)
    {
    	sActionLog = string("The inter-node type is IS_CREATED") ;

      if (strat->Class() < 0)
      {
      	sActionLog += string("; the strat class is negative, we add this node to explore_choice") ;
        explore_choice.push_back((*iter)->Node()) ;
      }
      else
      	if (strat->Class() == (*iter)->Edge()->Class() )
      {
      	if (false == (*iter)->Node()->Attribute().Exist(rootstrat->ID()))
        {
        	sActionLog += string("; the strat class is positive and corresponds to inter-nodes class and the parent doesn't have this strategy in its attributes, we add this node to explore_choice") ;
          explore_choice.push_back((*iter)->Node()) ;
        }
      }

      pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
    }
  }
*/

	fillConsistentStrategiesList(strat, &explore_choice) ;

  //
  // The following part is only usefull for nodes in the result network
  //
  if (explore_choice.empty())
  	return (iAction) ;

	Heuristix* choice = Controler()->GetHeuristix() ;  // if the class are triger
  if (NULL == choice)
  	return (iAction) ;

  sActionLog = string("Getting next node to explore from parents of node ") + getString() ;

	OB1Node* next = choice->NextNode(explore_choice, rootstrat) ;
	if (NULL == next)
  {
  	sActionLog += string(" (result is none... exiting)") ;
  	pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  	return (iAction) ;
  }

  sActionLog += string(" (result is node ") + next->getString() + string(")") ;
  pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

  LinkIterator itLink = next->findChild(this) ;
  if (NULL == itLink)
  {
  	sActionLog = string("This parent node doesn't have a link with node ") + getString() + string(" (exiting)") ;
  	pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  	return (1) ;
  }

	OB1NodeLink* temp = *itLink ; // we check if we didn't have an answer
	if (NULL == temp)
  	return (1) ;

/*
	LinkIterator itLink = findParent(next) ;
  if (NULL == itLink)
  {
  	sActionLog = string("This parent node doesn't have a link with node ") + getString() + string(" (exiting)") ;
  	Controler()->BB()->pContexte->getSuperviseur()->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  	return (iAction) ;
  }

	OB1NodeLink* temp = *itLink ; // we check if we didn't have an answer
	if (NULL == temp)
  	return (iAction) ;

	int inverseClass = temp->Edge()->Class() ;
*/

/*
  sActionLog = string("The inter-node class is ") + IntToString(inverseClass) ;
  bool Terminate = false ;

	OB1Token* res = getLastToken(inverseClass) ; // On verifie que l'on à pas déja la réponse a la question
	if (NULL != res)
	{
  	sActionLog += string(" and it corresponds to token ") + IntToString(res->getNumero()) ;
		if (Terminate == false)
    {
     	sActionLog += string(". Since Terminate is false, we put strat in computation state ANSWER") ;
			strat->putComputationState(ANSWER) ;     //FIXME
      //  next->createStrategy(strat);
      iAction = 1 ;
    }
	}
  else
  {
  	sActionLog += string(" and it doesn't correspond to a token, so we ask the parent node (") + next->getString() + string(") to create a strategy") ;
  	// OB1SubStrategy* newSubStrat =  new OB1SubStrategy(rootstrat,strat,0, inverseClass);
    //strat->AddSubStrategy(newSubStrat);
    next->createStrategy(strat, true) ;
    iAction = 1 ;
  }

  pInterface->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
*/

	next->createStrategy(strat, true) ;
	iAction = 1 ;

	return (iAction) ;
}
catch (...)
{
	erreur("Exception OB1NOther::createStrategy", standardError) ;
  return 0 ;
}
}

//
// The goal is to select all the strategies, starting from this node, that
// are consistent with the token
//
void
OB1NOther::fillConsistentStrategiesList(OB1SubStrategy* pStrat, std::vector<OB1Node*>* pExploreChoice)
{
	if (NULL == pExploreChoice)
		return ;

	if (NULL == pStrat)
		return ;

	if (_parent.empty())
		return ;

	OB1Strategy* rootstrat = (OB1Strategy*)pStrat->Root() ; /* get the root strategy */
  if (NULL == rootstrat)
		return ;

	OB1Token* pToken = rootstrat->Token() ;
  if (NULL == pToken)
		return ;

	BB1Object* pTokenObject = pToken->getObject() ;
  if (NULL == pTokenObject)
		return ;

	string sObjectKbName = pTokenObject->KB().Name() ;

  //
  // Very simple algorithme :
  //   - Events are sent to the TRIGER strategies
  //   - Questions are sent to the IS_CREATED strategies
  //
	for (LinkIterator iter = _parent.begin() ; _parent.end() != iter ; iter++)
	{
		OB1EdgeType ParentType = (*iter)->Edge()->TypeOf() ;

    if ((string("NautilusEventKB") == sObjectKbName) ||
        (string("InformationKB")   == sObjectKbName))
		{
			if ((TRIGERS == ParentType) && (false == (*iter)->Node()->Attribute().Exist(rootstrat->ID())))
				pExploreChoice->push_back((*iter)->Node()) ;
		}
    else if (string("QuestionKB") == sObjectKbName)
    {

    	if ((IS_CREATED == ParentType) && (false == (*iter)->Node()->Attribute().Exist(rootstrat->ID())))
      	pExploreChoice->push_back((*iter)->Node()) ;
    }
  }
}

int
OB1NAnd::createStrategy(OB1SubStrategy* strat, bool /* bInRootDirection */) /* this algorithme forbid the use of one path for trigger and result */
{
try
{
	if (NULL == strat)
		return (0) ;

  OB1Strategy* rootstrat = (OB1Strategy*)strat->Root() ;
  if (NULL == rootstrat)
		return (0) ;

  string sActionLog = string("Create strategy on NAnd node ") + getString() + string(" for root strategy ") + rootstrat->ID() ;
  if (Controler()->BB()->pContexte->getBBinterface())
    Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

  Attribute().insert(rootstrat->ID(), new MarkerInteger(0)) ;
  if (isActivable() == true)
  {
    string sActionLog = string("The node is activable, it asks its parent to create a strategy for root strategy ") + rootstrat->ID() ;
    if (Controler()->BB()->pContexte->getBBinterface())
      Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

    OB1Node* next = _parent[0]->Node() ;
    if (next)
      return next->createStrategy(strat, true) ;
  }
  else
  {
  	string sActionLog = string("The node is not activable. Nothing done.") ;
    if (Controler()->BB()->pContexte->getBBinterface())
      Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  }

  return (0) ;
}
catch (...)
{
	erreur("Exception OB1NAnd::createStrategy", standardError) ;
  return 0 ;
}
}

int
OB1NLevel::createStrategy(OB1SubStrategy* strat, bool /* bInRootDirection */) /* this algorithme forbid the use of one path for trigger and result */
{
try
{
	if (NULL == strat)
		return (0) ;

  OB1Strategy* rootstrat = (OB1Strategy*) strat->Root() ;
  if (NULL == rootstrat)
		return (0) ;

  string sActionLog = string("Create strategy on NLevel node ") + getString() + string(" for root strategy ") + rootstrat->ID() ;
  if (Controler()->BB()->pContexte->getBBinterface())
    Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

  Attribute().insert(rootstrat->ID(), new MarkerInteger(0)) ;
  if (ChildrenCardinal() == 0)
		return (1) ;

	OB1Node* andNode = _children[0]->Node() ;
	if (NULL == andNode)
		return (1) ;

	std::vector<OB1NodeLink* >& andChidldren = andNode->getChildren() ;
  // std::vector<OB1SubStrategy* >& substrat = strat->getSubStrategy() ;

  if (false == andChidldren.empty())
  {
    string sActionLog = string("Asking all the sons of the andNode to create a strategy for root strategy ") + rootstrat->ID() ;
    if (Controler()->BB()->pContexte->getBBinterface())
  	  Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

  	for (LinkIterator iter = andChidldren.begin() ; andChidldren.end() != iter ; iter++)
  	{
    	// Taking the link going from the son node to the andNode
      //
  		OB1NodeLink* temp = (*iter)->Node()->Parent(andNode) ;
      if (temp)
      {
    		int inverse = temp->Edge()->Class() ;
    		OB1Token *res = (*iter)->Node()->getLastToken(inverse) ;
    		if (NULL == res)
    		{
        	string sActionLog = string("The son ") + (*iter)->Node()->getString() + string(" has no token for the class ") + IntToString(inverse) + string("; we ask him to create a strategy for root strategy ") + rootstrat->ID() ;
          if (Controler()->BB()->pContexte->getBBinterface())
  				  Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

    			OB1SubStrategy* newSubStrat = new OB1SubStrategy(_controlers, rootstrat, strat, (*iter)->Edge()->Class(), inverse) ;
      		strat->AddSubStrategy(newSubStrat) ;
      		(*iter)->Node()->createStrategy(newSubStrat, false) ;
        }
        else
        {
        	string sActionLog = string("The son ") + (*iter)->Node()->getString() + string(" has already a token (") + IntToString(res->getNumero()) + string(") for the class ") + IntToString(inverse) ;
          if (Controler()->BB()->pContexte->getBBinterface())
  				  Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
        }
    	}
  	}
  }
  return (1) ;
}
catch (...)
{
	erreur("Exception OB1NLevel::createStrategy", standardError) ;
  return 0 ;
}
}

int
OB1NKS::createStrategy(OB1SubStrategy* strat, bool /* bInRootDirection */) /* this algorithme forbid the use of one path for trigger and result */
{
try
{
	if (NULL == strat)
		return (0) ;

	OB1Strategy* rootstrat = (OB1Strategy*)strat->Root() ;
	if (NULL == rootstrat)
		return (0) ;

  string sActionLog = string("Create strategy on KS node ") + getString() + string(" for root strategy ") + rootstrat->ID() ;
  if (Controler()->BB()->pContexte->getBBinterface())
    Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

	Attribute().insert(rootstrat->ID(), new MarkerInteger(0)) ;
	strat->putNode(this) ;

	Flag(rootstrat->Priority()) ;
	if (false == isActivable())
	{
    string sActionLog = string("KS node is not activable: asking Level node to create a strategy for root strategy ") + rootstrat->ID() ;
    if (Controler()->BB()->pContexte->getBBinterface())
  	  Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

    OB1NLevel* level = getLevelNode() ;
    if (level)
    	level->createStrategy(strat, false) ;
	}

	return (1) ;
}
catch (...)
{
	erreur("Exception OB1NKS::createStrategy", standardError) ;
  return 0 ;
}
}

int
OB1NTrigger::createStrategy(OB1SubStrategy* strat, bool /* bInRootDirection */)
{
try
{
	if (NULL == strat)
		return (0) ;

  OB1Strategy* rootstrat = (OB1Strategy*)strat->Root() ;
  if (NULL == rootstrat)
		return (0) ;

  string sActionLog = string("Create strategy on Trigger node ") + getString() + string(" for root strategy ") + rootstrat->ID() ;
  if (Controler()->BB()->pContexte->getBBinterface())
    Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

  Attribute().insert(rootstrat->ID(), new MarkerInteger(0)) ;
  OB1NodeLink* ks = _parent[0] ;
  if (NULL == ks)
  	return (0) ;

	if (false == ks->Node()->Attribute().Exist(rootstrat->ID()))
  {
  	string sActionLog = string("Trigger node asks ks node to connect to the strategy for root strategy ") + rootstrat->ID() ;
    if (Controler()->BB()->pContexte->getBBinterface())
  	  Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

    OB1SubStrategy* newSubStrat = new OB1SubStrategy(_controlers, rootstrat,strat, 0, ks->Edge()->Class()) ;
		strat->AddSubStrategy(newSubStrat) ;

		return ks->Node()->createStrategy(newSubStrat, true) ;
  }
  else
  {
  	string sActionLog = string("The ks node has already a strategy for root strategy ") + rootstrat->ID() + string(" (nothing to do)") ;
    if (Controler()->BB()->pContexte->getBBinterface())
    	Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  }

	return 0 ;
}
catch (...)
{
	erreur("Exception OB1NTrigger::createStrategy", standardError) ;
  return 0 ;
}
}

int
OB1NResult::createStrategy(OB1SubStrategy* strat, bool /* bInRootDirection */)
{
try
{
	if (NULL == strat)
		return (0) ;

  OB1Strategy* rootstrat = (OB1Strategy*)strat->Root() ;
  if (NULL == rootstrat)
		return (0) ;

  string sActionLog = string("Create strategy on Result node ") + getString() + string(" for root strategy ") + rootstrat->ID() ;
  if (Controler()->BB()->pContexte->getBBinterface())
    Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

  Attribute().insert(rootstrat->ID(), new MarkerInteger(0)) ;
  OB1NodeLink* ks = _parent[0] ;
  if (NULL == ks)
		return 0 ;

	if (false == ks->Node()->Attribute().Exist(rootstrat->ID()))
	{
    string sActionLog = string("Result node asks ks node to create a strategy for root strategy ") + rootstrat->ID() ;
    if (Controler()->BB()->pContexte->getBBinterface())
  	  Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;

		OB1SubStrategy* newSubStrat = new OB1SubStrategy(_controlers, rootstrat,strat,0, ks->Edge()->Class()) ;
		strat->AddSubStrategy(newSubStrat) ;
		ks->Node()->createStrategy(newSubStrat, true) ;
	}
  else
  {
  	string sActionLog = string("The ks node has already a strategy for root strategy ") + rootstrat->ID() + string(" (nothing to do)") ;
    if (Controler()->BB()->pContexte->getBBinterface())
  	  Controler()->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubDetails) ;
  }

	return 0 ;
}
catch (...)
{
	erreur("Exception OB1NResult::createStrategy", standardError) ;
  return 0 ;
}
}

