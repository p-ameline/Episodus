/*
** Copyright Nautilus, (10/9/2004)
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant � [rappeler les
** caract�ristiques techniques de votre logiciel].

** Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilit� au code source et des droits de copie,
** de modification et de redistribution accord�s par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limit�e.  Pour les m�mes raisons,
** seule une responsabilit� restreinte p�se sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les conc�dants successifs.

** A cet �gard  l'attention de l'utilisateur est attir�e sur les risques
** associ�s au chargement,  � l'utilisation,  � la modification et/ou au
** d�veloppement et � la reproduction du logiciel par l'utilisateur �tant
** donn� sa sp�cificit� de logiciel libre, qui peut le rendre complexe �
** manipuler et qui le r�serve donc � des d�veloppeurs et des professionnels
** avertis poss�dant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invit�s � charger  et  tester  l'ad�quation  du
** logiciel � leurs besoins dans des conditions permettant d'assurer la
** s�curit� de leurs syst�mes et ou de leurs donn�es et, plus g�n�ralement,
** � l'utiliser et l'exploiter dans les m�mes conditions de s�curit�.

** Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accept� les
** termes.
*/
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\NautilusType.h"
#include "nsldv\nsldvuti.h"

//
// ---------------------------- Class AnswerStatus ----------------------------
//

AnswerStatus::AnswerStatus()
{
	_currentAttemptCycleNumber = 0 ;

	_status          = astatusUnknown ;
	_processingStage = apstageUnknown ;
  _verboseStatus   = apverboseStandard ;
  _bAllowDirectKs  = true ;
}

AnswerStatus::AnswerStatus(const AnswerStatus& v)
{
	_currentAttemptCycleNumber = v._currentAttemptCycleNumber ;

  _status            = v._status ;
  _processingStage   = v._processingStage ;
  _verboseStatus     = v._verboseStatus ;
  _bAllowDirectKs    = v._bAllowDirectKs ;

  _aKsAttempsHistory = v._aKsAttempsHistory ;
}

AnswerStatus::AnswerStatus(AnswerStatus* v)
{
	if (v)
  {
  	_currentAttemptCycleNumber = v->_currentAttemptCycleNumber ;

		_status            = v->_status ;
  	_processingStage   = v->_processingStage ;
    _verboseStatus     = v->_verboseStatus ;
    _bAllowDirectKs    = v->_bAllowDirectKs ;

  	_aKsAttempsHistory = v->_aKsAttempsHistory ;
  }
  else
  {
  	_currentAttemptCycleNumber = 0 ;

		_status          = astatusUnknown ;
		_processingStage = apstageUnknown ;
    _verboseStatus   = apverboseStandard ;
    _bAllowDirectKs  = true ;
  }
}

AnswerStatus::~AnswerStatus()
{
	_aKsAttempsHistory.vider() ;
}

void
AnswerStatus::initializeForNewAttempt()
{
  _status          = astatusProcessing ;
  _processingStage = apstageStarting ;

  _aKsAttempsHistory.vider() ;
}

void
AnswerStatus::referenceKsStart(std::string ksName, string processStartDatetime)
{
	if (string("") == ksName)
		return ;

	KSAttemptHistoryElement* pNewHistory = new KSAttemptHistoryElement(ksName) ;
  pNewHistory->setCycleNumber(_currentAttemptCycleNumber) ;

  if (string("") == processStartDatetime)
	{
  	NVLdVTemps ldvTemps ;
  	ldvTemps.takeTime() ;
  	processStartDatetime = ldvTemps.donneDateHeure() ;
  }

  pNewHistory->setProcessStart(processStartDatetime) ;

  _aKsAttempsHistory.push_back(pNewHistory) ;
}

void
AnswerStatus::referenceKsEnd(std::string ksName, KSAttemptHistoryElement::KSATTEMPTRESULT ksResult, string processEndDatetime)
{
	if (string("") == ksName)
		return ;

	KSAttemptHistoryElement* pHistoElement = getHistoryElement(ksName) ;
  if (NULL == pHistoElement)
		return ;

  pHistoElement->setKSResult(ksResult) ;

  if (string("") == processEndDatetime)
	{
  	NVLdVTemps ldvTemps ;
  	ldvTemps.takeTime() ;
  	processEndDatetime = ldvTemps.donneDateHeure() ;
  }

	pHistoElement->setProcessEnd(processEndDatetime) ;
}

KSAttemptHistoryElement*
AnswerStatus::getHistoryElement(std::string ksName, int iCycle)
{
	if (_aKsAttempsHistory.empty())
		return (KSAttemptHistoryElement*) 0 ;

	ArrayOfHistoryAttemptsIter histIter = _aKsAttempsHistory.begin() ;
  for ( ; _aKsAttempsHistory.end() != histIter ; histIter++)
  	if (((*histIter)->getKSName() == ksName) && isInCycle(*histIter, iCycle))
      	break ;

	if (_aKsAttempsHistory.end() == histIter)
		return (KSAttemptHistoryElement*) 0 ;

	return *histIter ;
}

KSAttemptHistoryElement*
AnswerStatus::getSuccesfulHistoryElement(int iCycle)
{
	if (_aKsAttempsHistory.empty())
		return (KSAttemptHistoryElement*) 0 ;

	ArrayOfHistoryAttemptsIter histIter = _aKsAttempsHistory.begin() ;
  for ( ; _aKsAttempsHistory.end() != histIter ; histIter++)
  	if (((*histIter)->getKSResult() == KSAttemptHistoryElement::ksrSuccess) && isInCycle(*histIter, iCycle))
      	break ;

	if (_aKsAttempsHistory.end() == histIter)
		return (KSAttemptHistoryElement*) 0 ;

	return *histIter ;
}

KSAttemptHistoryElement*
AnswerStatus::getNextHistoryElement(KSAttemptHistoryElement* /* pCurrent */, int /* iCycle */)
{
	return (KSAttemptHistoryElement*) 0 ;
}

KSAttemptHistoryElement*
AnswerStatus::getPreviousHistoryElement(KSAttemptHistoryElement* /* pCurrent */, int /* iCycle */)
{
	return (KSAttemptHistoryElement*) 0 ;
}

KSAttemptHistoryElement*
AnswerStatus::getFirstHistoryElement(int /* iCycle */)
{
	return (KSAttemptHistoryElement*) 0 ;
}

KSAttemptHistoryElement*
AnswerStatus::getLastHistoryElement(int /* iCycle */)
{
	return (KSAttemptHistoryElement*) 0 ;
}

bool
AnswerStatus::isInCycle(KSAttemptHistoryElement* pHistoElemt, int iCycle)
{
	if (NULL == pHistoElemt)
		return false ;

	if (pHistoElemt->getCycleNumber() == iCycle)
		return true ;

	if ((iCycle == -1) && (pHistoElemt->getCycleNumber() == _currentAttemptCycleNumber))
		return true ;

	return false ;
}

AnswerStatus&
AnswerStatus::operator=(const AnswerStatus& v)
{
	if (this == &v)
		return *this ;

  _status            = v._status ;
  _processingStage   = v._processingStage ;
  _verboseStatus     = v._verboseStatus ;
  _bAllowDirectKs    = v._bAllowDirectKs ;

  _aKsAttempsHistory = v._aKsAttempsHistory ;

  return (*this) ;
}

string
AnswerStatus::getStatusAsString()
{
  switch (_status)
  {
    case astatusUnknown    : return string("Unknown") ;
    case astatusProcessing : return string("Processing") ;
    case astatusProcessed  : return string("Processed") ;
    case astatusDeprecated : return string("Deprecated") ;
    default                : return string("???") ;
  }
}

string
AnswerStatus::getProcessingStageAsString()
{
  switch (_processingStage)
  {
    case apstageUnknown   : return string("Unknown") ;
    case apstageStarting  : return string("Starting") ;
    case apstageDirectKS  : return string("Direct KS") ;
    case apstageRecord    : return string("Record") ;
    case apstageGenericKs : return string("Generic KS") ;
    case apstageExited    : return string("Exited") ;
    default               : return string("???") ;
  }
}

string
AnswerStatus::getVerboseStatusAsString()
{
  switch (_verboseStatus)
  {
    case apverboseUnknown       : return string("Unknown") ;
    case apverboseStandard      : return string("Standard") ;
    case apverboseSilent        : return string("Silent") ;
    case apverboseSilentUpdated : return string("Silent updated") ;
    default                     : return string("???") ;
  }
}

string
AnswerStatus::getUpdatabilityStatusAsString()
{
  switch (_verboseStatus)
  {
    case updatabilityUnknown  : return string("Unknown") ;
    case updatabilityStandard : return string("Standard") ;
    case updatabilityNever    : return string("Never") ;
    default                   : return string("???") ;
  }
}

//
// ----------------------- Class KSAttemptHistoryElement -----------------------
//

KSAttemptHistoryElement::KSAttemptHistoryElement(std::string ksName)
{
	_ksName   = ksName ;
	_ksResult = ksrUnknown ;

	_processStartDatetime = std::string("") ;
	_processEndDatetime   = std::string("") ;
}

KSAttemptHistoryElement::KSAttemptHistoryElement(const KSAttemptHistoryElement& src)
{
	_ksName   = src._ksName ;
	_ksResult = src._ksResult ;

	_processStartDatetime = src._processStartDatetime ;
	_processEndDatetime   = src._processEndDatetime ;
}

KSAttemptHistoryElement::~KSAttemptHistoryElement()
{
}

KSAttemptHistoryElement&
KSAttemptHistoryElement::operator=(const KSAttemptHistoryElement& src)
{
	if (this == &src)
		return *this ;

	_ksName   = src._ksName ;
	_ksResult = src._ksResult ;

	_processStartDatetime = src._processStartDatetime ;
	_processEndDatetime   = src._processEndDatetime ;

  return *this ;
}

//
// -------------------------- Class NautilusQuestion --------------------------
//

NautilusQuestion::NautilusQuestion(std::string&  question, std::string& argument)
                 :_question(std::string(question)), _argument(std::string(argument))
{
}

NautilusQuestion::NautilusQuestion(const NautilusQuestion& v)
{
  _question = std::string(v._question) ;
  _argument = std::string(v._argument) ;
}

NautilusQuestion::NautilusQuestion(const NautilusQuestion* v)
{
	if (NULL != v)
  {
		_question = std::string(v->_question) ;
		_argument = std::string(v->_argument) ;
  }
  else
  {
		_question = std::string("") ;
		_argument = std::string("") ;
  }

}

NautilusQuestion::~NautilusQuestion()
{
}

NautilusQuestion::NautilusQuestion(std::string& question)
                 :_question(std::string(question)), _argument(std::string(""))
{
}

bool
NautilusQuestion::operator==(const NautilusQuestion& node) const
{
	return (_question == node.Question()) ;
}

NautilusQuestion&
NautilusQuestion::operator=(const NautilusQuestion& v)
{
	if (this == &v)
		return *this ;

  _question = std::string(v._question) ;
  _argument = std::string(v._argument) ;

  return  (*this) ;
}

