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
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\OB1Controler.h"
#include "ns_ob1\OB1Node.h"
#include "ns_ob1\OB1Sheduler.h"
#include "ns_ob1\OB1Token.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "ob1Lus/InterfaceLus.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
  #include "ns_ob1\Interface.h"
#endif

const int OPPORTUNISTIC_PRIORITY = 2;

Sheduler::Sheduler(OB1Controler* temp)
{
	control = temp;

	ReInit() ;
}

ShedulerAlgorithm Sheduler::getAlgorithm()
{
  return (_shedulingAlgorithm);
}

void Sheduler::putAlgoritm(ShedulerAlgorithm temp)
{
  _shedulingAlgorithm = temp;
}

void Sheduler::ReInit()
{
try
{
	ClearAllStrategies() ;
  current_problem = NULL ;

	// Ajout de la strategie opportuniste
  //
  OB1Strategy* Oppor = new OB1Strategy(control, control->StrategyCount(), 0, OPPORTUNIST, OPPORTUNISTIC_PRIORITY) ;
  _Controler_History_X.push_back(Oppor) ;
  _shedulingAlgorithm = STOCHASTIC ;
  _totalPriority = OPPORTUNISTIC_PRIORITY ;
}
catch (...)
{
	erreur("Exception BB1 Sheduler::ReInit.", standardError) ;
}
}

void
Sheduler::createStrategyPath(OB1Strategy* temp)
{
	if (NULL == temp)
  	return ;

  OB1Token* tempoN = temp->Token() ;
  if (NULL == tempoN)
  	return ;

	OB1Node* no = tempoN->Node() ;
  if (NULL == no)
  	return ;

  OB1NOther* pOtherNode = dynamic_cast<OB1NOther*> (no) ;
  if (NULL == pOtherNode)
  	no->createStrategy(temp, true) ;
  else
  {
  	bool bInKsDirection = !(pOtherNode->isLeaf()) ;
    /* int iResult = */ pOtherNode->createStrategy(temp, bInKsDirection) ;
  }
}

OB1Strategy*
Sheduler::addStrategie(OB1Token* token, int priority, int clas)
{
try
{
	if (NULL == token)
		return NULL ;

  string sActionLog = string("Scheduler is creating a strategy for token ") + IntToString(token->getNumero()) ;
  if (control->BB()->pContexte->getBBinterface())
    control->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

  OB1Strategy* temp = new OB1Strategy(control, control->StrategyCount(), clas, token, priority, token->getCreatedCycle()) ;
  createStrategyPath(temp) ;

  // If there is no substrategy, then it means creation failed
  //
  if (temp->getSubStrategy().empty())
  {
  	delete temp ;
    return NULL ;
  }

  _totalPriority += priority ;
  _Controler_History_X.push_back(temp) ;

  sActionLog = string("Strategy created by Scheduler (") + string("Id=") + temp->ID() + string(")") ;
  if (control->BB()->pContexte->getBBinterface())
    control->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

  return (temp) ;
}
catch (...)
{
	erreur("Exception BB1 Sheduler::addStrategie.", standardError) ;
  return NULL ;
}
}

Sheduler::~Sheduler()
{
	ClearAllStrategies() ;
}

void Sheduler::putNewCurrentProblem(OB1Strategy* temp)
{
  current_problem = temp ;
}

void
Sheduler::runOppotunisticDuring(int Time)
{
  _during = Time;
}

int
Sheduler::getTotalOfPriority()
{
  int result = 0 ;
  if (_Controler_History_X.empty())
		return (result) ;

	for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; _Controler_History_X.end() != stratIter ; stratIter++)
  {
    OB1Token* pStratToken = (*stratIter)->Token() ;
  	if (pStratToken && (pStratToken->isFree() || pStratToken->hasRunned()))
    	result += (*stratIter)->Priority() ;
  }

  return (result) ;
}

OB1Strategy*
Sheduler::NextStrategyToCompute()
{
	if (_Controler_History_X.empty())
		return NULL ;

  OB1Strategy* pCandidateStrat = NULL ;

  int min = 1000;

  // We only take into account free strategies
  // int tot = _totalPriority ;
  int tot = getTotalOfPriority() ;

  switch(_shedulingAlgorithm)
  {
		case DETERMINIST:
    	for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; _Controler_History_X.end() != stratIter ; stratIter++)
      {
        OB1Token* pStratToken = (*stratIter)->Token() ;
      	if (pStratToken && (pStratToken->isFree() || pStratToken->hasRunned()))
        {
      		float percent = (int)((*stratIter)->Priority() * 100) /tot ;
      		float calc = (*stratIter)->getCountCycle() /  percent ;
      		if (calc < min)
        	{
          	min = calc ;
          	pCandidateStrat = *stratIter ;
          }
        }
			}
  		break ;

		case STOCHASTIC:
			int incSum = 0 ;
			int left = random(_totalPriority) ;
			for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; _Controler_History_X.end() != stratIter ; stratIter++)
      {
        OB1Token* pStratToken = (*stratIter)->Token() ;
      	if (pStratToken && (pStratToken->isFree() || pStratToken->hasRunned()))
        {
      		pCandidateStrat = *stratIter ;

					incSum += (int)(*stratIter)->Priority() ;
					if (incSum >= left)
          	break ;
        }
			}
			break ;
	}

  if (NULL == pCandidateStrat)
  	return NULL ;

  string sActionLog = string("Scheduler elected strategy ") + pCandidateStrat->ID() + string(" (") + pCandidateStrat->describe() + string(") as next strategy to compute") ;
  if (pCandidateStrat->Token())
    sActionLog += string(" (hosting token ") + IntToString(pCandidateStrat->Token()->getNumero()) + string(")") ;
  if (control->BB()->pContexte->getBBinterface())
    control->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSteps) ;

  // It is useless to run a strategy whose token is waiting for a sub-token
  // we have to run the sub-strategy instead
  //
  bool bLookForDeepestStrategy = true ;
  while (bLookForDeepestStrategy)
  {
  	if (pCandidateStrat->Token())
  	{
  		OB1Token* pToken = control->findSubToken(pCandidateStrat->Token()) ;
    	if (NULL == pToken)
				bLookForDeepestStrategy = false ;
      else
      {
      	OB1Strategy* pStrat = getStrategyForToken(pToken) ;
        if (NULL == pStrat)
        	bLookForDeepestStrategy = false ;
        else
        {
        	pCandidateStrat = pStrat ;
          sActionLog = string("Scheduler switchs to strategy ") + pCandidateStrat->ID() + string(" because of sub-token ") + IntToString(pToken->getNumero()) ;
          if (control->BB()->pContexte->getBBinterface())
            control->BB()->pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
        }
      }
    }
    else
    	bLookForDeepestStrategy = false ;
	}

	return pCandidateStrat ;
}

/*
int
Sheduler::getTotalOfPriority()
{
  int result = 0;
  if (!(_Controler_History_X.empty()))
		for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; stratIter != _Controler_History_X.end() ; stratIter++)
			result += (*stratIter)->Priority() ;

  return (result) ;
}

OB1Strategy*
Sheduler::NextStrategyToCompute()
{
	if (_Controler_History_X.empty())
		return NULL ;

  OB1Strategy* pCandidateStrat = NULL ;

  int min = 1000;
  int tot = _totalPriority ;

  switch(_shedulingAlgorithm)
  {
		case DETERMINIST:
    	for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; stratIter != _Controler_History_X.end() ; stratIter++)
      {
      	float percent = (int)((*stratIter)->Priority() * 100) /tot ;
      	float calc = (*stratIter)->getCountCycle() /  percent ;
      	if (calc < min)
        {
          min = calc ;
          pCandidateStrat = *stratIter ;
        }
			}
  		break ;

		case STOCHASTIC:
			int incSum = 0 ;
			int left = random(_totalPriority) ;
			for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; stratIter != _Controler_History_X.end() ; stratIter++)
      {
      	pCandidateStrat = *stratIter ;

				incSum +=  (int)(*stratIter)->Priority();
				if (incSum >= left)
          break ;
			}
			break ;
	}

	return pCandidateStrat ;
}
*/

OB1Strategy*
Sheduler::GetCurrentStrategy()
{
  return (current_problem) ;
}

void
Sheduler::RemoveStrategy(std::vector<OB1Strategy*>::iterator& index)
{
	if ((NULL == index) || (NULL == *index))
		return ;

  _totalPriority -= (*index)->Priority() ;
  
  delete(*index) ;
  _Controler_History_X.erase(index) ;
}

void
Sheduler::ClearAllStrategies()
{
	if (_Controler_History_X.empty())
		return ;

	for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; _Controler_History_X.end() != stratIter ; )
	{
		if (*stratIter)
			delete *stratIter ;
		_Controler_History_X.erase(stratIter) ;
	}
}

void
Sheduler::GarbageCollecting()
{
	if (_Controler_History_X.empty())
		return ;

  for (std::vector<OB1Strategy* >::iterator iter = _Controler_History_X.begin() ; _Controler_History_X.end() != iter ; )
		if ((*iter)->Delete() == true)
		{
			delete(*iter) ;
			_Controler_History_X.erase(iter) ;
      // break ;
		}
		else
			iter++ ;

	_totalPriority = getTotalOfPriority() ;
}

bool
Sheduler::findStrategy(OB1Strategy* temp, std::vector<OB1Strategy*>::iterator& it)
{
	it = NULL ;

	if (_Controler_History_X.empty())
		return false ;

	std::vector<OB1Strategy*>::iterator itStrat = _Controler_History_X.begin() ;
  for ( ; itStrat != _Controler_History_X.end() ; itStrat++)
	{
  	if ((*itStrat) && (*itStrat == temp))
    {
    	it = itStrat ;
    	return true ;
    }
	}

  return false ;
}

std::vector<OB1Strategy*>::iterator&
Sheduler::findStrategy(OB1Strategy* temp)
{
	std::vector<OB1Strategy*>::iterator* find = new std::vector<OB1Strategy*>::iterator() ;

try
{
	if (false == _Controler_History_X.empty())
	{
		std::vector<OB1Strategy*>::iterator fin = _Controler_History_X.end() ;
  	for (*find = _Controler_History_X.begin(); *find != fin; *find++)
    {
			OB1Strategy* test = *(*find) ;
			if ((test->Priority()      == temp->Priority()) &&
          (temp->getBeginCycle() == test->getBeginCycle()) &&
          (test->Token()         == temp->Token()) &&
          (temp->geStratType()   == test->geStratType()))
				return (*find) ;
    }
	}
}
catch (...)
{
	erreur("Exception BB1 Sheduler::findStrategy.", standardError) ;
}

  find = NULL ;
  return (*find) ;
}

OB1Strategy*
Sheduler::getStrategyForToken(OB1Token* pToken)
{
	if (_Controler_History_X.empty())
		return NULL ;

	for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; stratIter != _Controler_History_X.end() ; stratIter++)
		if ((*stratIter)->Token() == pToken)
    	return *stratIter ;

	return NULL ;
}

OB1Token*
Sheduler::getTokenFromId(long int iTokId)
{
	if (_Controler_History_X.empty())
		return NULL ;

	for (std::vector<OB1Strategy*>::iterator stratIter = _Controler_History_X.begin() ; stratIter != _Controler_History_X.end() ; stratIter++)
		if ((*stratIter)->Token() && ((*stratIter)->Token()->getNumero() == iTokId))
    	return (*stratIter)->Token() ;

	return NULL ;
}

