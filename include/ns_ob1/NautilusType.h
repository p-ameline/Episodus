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

#ifndef __NAUTILUS_OB1_TYPES_H__
#define __NAUTILUS_OB1_TYPES_H__

#include <string>
#include "partage\ns_vector.h"
#include "partage\NTArray.h"

#include "ns_ob1\OB1Export.h"

class _BBKEXPORT KSAttemptHistoryElement
{
 public:

	enum KSATTEMPTRESULT { ksrUnknown = 0, ksrFailure, ksrSuccess } ;
  // add an enum for reason for failure ?

	KSAttemptHistoryElement(std::string ksName) ;
	KSAttemptHistoryElement(const KSAttemptHistoryElement& src) ;
	~KSAttemptHistoryElement() ;

	KSAttemptHistoryElement& operator=(const KSAttemptHistoryElement& v) ;

  std::string     getKSName()       { return _ksName ; }
  KSATTEMPTRESULT getKSResult()     { return _ksResult ; }
  std::string     getProcessStart() { return _processStartDatetime ; }
  std::string     getProcessEnd()   { return _processEndDatetime ; }
  int             getCycleNumber()  { return _attemptCycleNumber ; }

  void setKSResult(KSATTEMPTRESULT result)    { _ksResult = result ; }
  void setProcessStart(std::string sDateTime) { _processStartDatetime = sDateTime ; }
  void setProcessEnd(std::string sDateTime)   { _processEndDatetime = sDateTime ; }
  void setCycleNumber(int iCycleNb)           { _attemptCycleNumber = iCycleNb ; }

 protected:

 	int             _attemptCycleNumber ;

	std::string     _ksName ;
	KSATTEMPTRESULT _ksResult ;

  std::string     _processStartDatetime ;
  std::string     _processEndDatetime ;
} ;

typedef vector<KSAttemptHistoryElement*> ArrayOfHistoryAttempts ;
typedef ArrayOfHistoryAttempts::iterator ArrayOfHistoryAttemptsIter ;
typedef NTArray<KSAttemptHistoryElement> KsHistoryOfAttempts ;

//
// Status of an answer, and history of KSs attempts
//

class _BBKEXPORT AnswerStatus
{
 public:

	enum ANSWERSTATUS          { astatusUnknown = -1, astatusProcessing, astatusProcessed, astatusDeprecated } ;
  enum ANSWERPROCESSINGSTAGE { apstageUnknown = -1, apstageStarting, apstageDirectKS, apstageRecord, apstageGenericKs, apstageExited } ;
  enum ANSWERVERBOSESTATUS   { apverboseUnknown = -1, apverboseStandard, apverboseSilent, apverboseSilentUpdated } ;
  enum UPDATABILITYSTATUS    { updatabilityUnknown = -1, updatabilityStandard, updatabilityNever } ;

 protected:

 	int                   _currentAttemptCycleNumber ;

	ANSWERSTATUS          _status ;
	ANSWERPROCESSINGSTAGE _processingStage ;
  ANSWERVERBOSESTATUS   _verboseStatus ;
  UPDATABILITYSTATUS    _updatabilityStatus ;

  KsHistoryOfAttempts   _aKsAttempsHistory ;

  bool                  _bAllowDirectKs ;

 public:

	AnswerStatus() ;
	AnswerStatus(const AnswerStatus& src) ;
  AnswerStatus(AnswerStatus* v) ;
	~AnswerStatus() ;

	AnswerStatus& operator=(const AnswerStatus& v) ;

	bool operator==(const AnswerStatus& node) const ;

	inline ANSWERSTATUS          Status()             { return _status ; }
	inline ANSWERPROCESSINGSTAGE ProcessingStage()    { return _processingStage ; }
  inline ANSWERVERBOSESTATUS   VerboseStatus()      { return _verboseStatus ; }
  inline UPDATABILITYSTATUS    UpdatabilityStatus() { return _updatabilityStatus ; }
  int                          Cycle()              { return _currentAttemptCycleNumber ; }
  bool                         directKsEnabled()    { return _bAllowDirectKs ; }

  void setStatus(ANSWERSTATUS iStatus)                   { _status          = iStatus ; }
  void setStage(ANSWERPROCESSINGSTAGE iStage)            { _processingStage = iStage ; }
  void setVerboseStatus(ANSWERVERBOSESTATUS iVerbose)    { _verboseStatus   = iVerbose ; }
  void setUpdatabilityStatus(UPDATABILITYSTATUS iUpdate) { _updatabilityStatus = iUpdate ; }
  void setDirectKsEnabled(bool bDirectKsEnabled)         { _bAllowDirectKs  = bDirectKsEnabled ; }

  void initializeForNewAttempt() ;

  void referenceKsStart(std::string ksName, string processStartDatetime = string("")) ;
  void referenceKsEnd(std::string ksName, KSAttemptHistoryElement::KSATTEMPTRESULT ksResult, string processEndDatetime = string("")) ;

	KSAttemptHistoryElement* getHistoryElement(std::string ksName, int iCycle = -1) ;
  KSAttemptHistoryElement* getSuccesfulHistoryElement(int iCycle = -1) ;

  KSAttemptHistoryElement* getNextHistoryElement(KSAttemptHistoryElement* pCurrent, int iCycle = -1) ;
  KSAttemptHistoryElement* getPreviousHistoryElement(KSAttemptHistoryElement* pCurrent, int iCycle = -1) ;
  KSAttemptHistoryElement* getFirstHistoryElement(int iCycle = -1) ;
  KSAttemptHistoryElement* getLastHistoryElement(int iCycle = -1) ;

  bool isInCycle(KSAttemptHistoryElement* pHistoElemt, int iCycle) ;

  string getStatusAsString() ;
  string getProcessingStageAsString() ;
  string getVerboseStatusAsString() ;
  string getUpdatabilityStatusAsString() ;
};

/**
* \brief Used to dispath a question with one parameter
*/
/**
*  Used to dispath a question with one parameter
*  Utiliser dans les types Val pour poser une question avec argument dans les dispatch
* \remarks En effet, les noeud et les donnes sont identifi�s par un typeVal
*/
class _BBKEXPORT NautilusQuestion
{
  // friend  class NautilusQuestion ;
  protected:
    std::string _question ;   /**< Question to ask \brief Question to ask*/
    std::string _argument ;   /**< Question's argument \brief Question's argument */

  public:
  	/**
    * \brief Constructor
    */
    /**
    *  Constructor
    * @param question Question to ask (generaly a nautilus path)
    * @param argument complementary information about question
    */
    NautilusQuestion(std::string&  question, std::string& argument) ;

   	/**
    * \brief Constructor
    */
    /**
    *  Constructor
    * @param question Question to ask (generaly a nautilus path)
    */
    NautilusQuestion(std::string& question) ;

    /**
    * \brief copy constructor
    */
    /**
    * copy constructor
    * @param NautilusQuestion to clone
    */
    NautilusQuestion(const NautilusQuestion& question) ;

    /**
    * \brief constructor
    */
    /**
    * constructor
    * @param NautilusQuestion to clone
    */
    NautilusQuestion::NautilusQuestion(const NautilusQuestion* v) ;

    /**
    * \brief destructor
    */
    /**
    * Destructor
    */
    ~NautilusQuestion();

    /**
    * \brief affectation operator
    */
    /**
    *   affectation operator
    */
    NautilusQuestion& operator=(const NautilusQuestion& v) ;

    /**
    * \brief test equality between two Nautilus Question
    */
    /**
    * test equality between two Nautilus Question
    * @param node Question to test
    * @return return true if question adn argumetn are the same and false else
    */
    bool operator==(const NautilusQuestion& node) const ;

    /**
    * \brief get the question
    */
    /**
    * Return the Question
    * @return Question
    */
    inline
    std::string Question() const { return _question ; }

    /**
    * \brief get the Argument
    */
    /**
    * Return the Argument
    * @return Argument
    */
    inline
    std::string Argument() const { return _argument ; }
};

#endif

