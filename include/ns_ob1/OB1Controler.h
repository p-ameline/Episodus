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

#ifndef   OB1CONTROLER_H_
# define   OB1CONTROLER_H_

class BB1KS ;
class OB1Node ;
class Heuristix ;
class BB1BBInterface ;
class OB1Strategy ;
class BB1ClassIndex ;
class Level ;
class INode ;

#ifdef __DEBUG__
  #include <fstream>
#endif

#include <vector>
#include <string>
#include "ns_ob1\OB1ClassIndex.h"
#include "ns_ob1\OB1graph.h"
// #include "ns_ob1\InitStructure.h"
#include "ns_ob1\OB1Sheduler.h"
#include "ns_ob1\OB1Strategy.h"
#include "ns_ob1\OB1Heuristix.h"
#include "ns_ob1\OB1Agenda.h"
#include "ns_ob1\TypedVal.h"

std::vector<std::string>* parseClass(std::string& inp) ;

/**
* \brief Default computation for leaf node
*/
/**
*  Default computation for leaf node <BR>
*  Pointeur sur fonction d�finissant une fonction s'executant par d�fault quand il n'y a aucun KS qui sait r�pondre ou r�cuperer une donn�e
*  \remarks Actuellement ce role est jou� par une fonction qui va rechercher dans le dossier patient
*/
typedef  bool  DCLN(TypedVal& daf, TypedVal* pSearchParams, HWND interfaceHandle, BB1BBInterface* temp) ;

/**
* \brief Controler
*/
/**
*  Controler <BR>
*  Controleur
*/
class OB1Controler
{
	friend class Sheduler ;
	friend class BB1BB ;
	friend class OB1NKS ;

 protected:

	BB1ClassIndex        _IClass ;            /**< Map of the class \brief Map of the class  */
	OB1Graph             _bb_state ;          /**< Graph of dependencies \brief Graph of dependencies  */
	Sheduler             _sheduler ;          /**< The sheduler \brief The sheduler*/
	BB1BB*               _bb ;                /**< Pointer to the blackboard \brief Pointer to the blackboard  */
	HeuristixGenerator   _choice ;            /**< Heuristic's strategy generator \brief Heuristic's strategy generator */
	OB1Token*            _lastTokenCreated ;  /**< Last token executed \brief Last token executed */
	int                  _StrategyCounter ;
	OB1Agenda            _Agenda ;

  NTArray<OB1Token>    _aTokensArray ;
  NTArray<OB1Token>    _aDeprecatedTokensArray ;

  long int             _iNextHandle ;      /**< Handle for objects persistance */

 public:

	DCLN*                _DefaultFunc ;       /* Function for computing leaf node \brief Function for computing leaf node */

	/**
	* \brief Constructor
	*/
	/**
	* Constructor <BR>
	* Constructeur
	* @param bb Pointeur to bb's methods
	*/
	OB1Controler(BB1BB* bb) ;

	/**
	* \brief Destructor
	*/
	/**
	* Destructor
	*/
	~OB1Controler() ;

	/**
	* \brief Save the graph
	*/
	void   save() ;

	/**
	*  \brief  return the BB
	*/
	BB1BB* BB() { return (_bb) ; }

	/**
	* \brief Pose une nouvelle question en deterministic
	*/
	/**
	* Pose une nouvelle question en deterministic
	* @param token de tete de la strategie
	* @param priority priorit� de la strategy a cr�er
	*/
	OB1Strategy*  AskDeterminicOrder(OB1Token* token, int priority) ;
	OB1Strategy*  AskTriggerOrder(OB1Token* token, int priority) ;

	/**
	* \brief lance le controleur en mode opportuniste
	*/
	void          RunInOpportunisticMode(int nb_tour) ;

	/**
	* return an heuristix
	*/
	Heuristix*    GetHeuristix() ;

	/**
	* \brief Return the number of the strategy and increment the number of stragey
	*/
	/**
	*  Return the number of the strategy and increment the number of stragey <BR>
	*  Renvoie le nombre de strategies sous forme de string.
	*  Le renvoye sous forme de string permet une utilisation
	*  direct de ce nombre par les strategie comme identifiant unique
	*  Chaque appel incremente le nombre de strategie de 1
	*  @return Number of strategy
	*/
	std::string   StrategyCount() ;

	/**
	*  Execute the action for during one cycle
	*/
	void Execute() ;

	/**
	* Reinit the controler
	*/
	void ReInitControler() ;

	/**
	*  return the name of the class
	*/
	int  getClassName(std::string temp)
	{
  	return _IClass[temp] ;
	}

	/**
	* Return the graph
	*/
	OB1Graph& Graph()             { return  _bb_state ; }
	OB1Token* LastTokenCreated()  { return  _lastTokenCreated ; }
	void      addBB1Class(std::string name) ;

  NTArray<OB1Token>* getTokenArray() { return &_aTokensArray ; }

	/**
	* \brief Add a KS int the graph
	*/
	/**
	* Add a KS int the graph <BR>
	* Ajoute un KS. Le controleur demande au KS de se publier et rejoute les informations obtenues dans le graph global
	* @param temp KS to add
	*/
	void addKS(BB1KS* temp);

	/**
	* Dispatch the event in the network and return a bool which indicates if the token have find a place in the network
	*/
	bool Dispatch(OB1Token* dis, bool createStrat = false) ;
  bool isDispatchableNode(OB1Token* dis, OB1NOther* pOtherNode) ;

	bool DispatchAgain(OB1Token* dis, bool createStrat = false);

  bool checkForCyclicTokenDependancy(TypedVal pTargetedNodeName, OB1Token* pMasterToken) ;

	/**
	* Token management
	*/

  OB1Token* createToken(EventType type, BB1Object* pTr) ;
  OB1Token* createToken(EventType type, BB1Object* pTr, AVPSet* Change, LinkSet* chan) ;
  void      deleteToken(OB1Token* pToken) ;

	OB1Token* find(TypedVal& temp, std::string clasName) ;

  OB1Token* findTokenFromId(long int iTokId) ;
  OB1Token* findSubToken(OB1Token* pMasterToken) ;

  bool      isValidToken(OB1Token* pToken) ;
  bool      isInvalidToken(OB1Token* pToken) { return !isValidToken(pToken) ; }

  void      deleteAllTokens() ;
  void      deleteAllTokensForObject(BB1Object* pObject) ;

  void      deprecateProcessedTokens() ;
  void      resetProcessingTokens() ;

	/**
	* \brief Execute deterministic action
	*/
	/**
	* Execute deterministic action <BR>
	* Execute une strategy deterministe
	* @temp deterministic stratey to execute
	* \note L'execution d'une strategie ne correponda pas forecement � l'execution d'un KS
	*/
	bool ExecuteDeterministicAction(OB1Strategy** temp) ;

  bool                                createQuestionAndAnswer(TypedVal* pQuestion, TypedVal *pSearchStruct) ;

  bool                                NextComputeAction(BB1Object* pObject, OB1Token* pMasterToken, bool bUserIsWaiting = false, HWND interfaceHandle = 0) ;
  bool                                NextComputeActionForQuestion(BB1Object* pQuestion, OB1Token* pMasterToken, bool bUserIsWaiting = false, HWND interfaceHandle = 0) ;
  bool                                NextComputeActionForAnswer(BB1Object* pAnswer, OB1Token* pMasterToken, bool bUserIsWaiting = false, HWND interfaceHandle = 0) ;
  bool                                NextComputeActionForEvent(BB1Object* pAnswer, OB1Token* pMasterToken, bool bUserIsWaiting = false, HWND interfaceHandle = 0) ;

  AnswerStatus::ANSWERPROCESSINGSTAGE getNextProcessingStageForQuestion(AnswerStatus::ANSWERPROCESSINGSTAGE iCurrentStage) ;
  AnswerStatus::ANSWERPROCESSINGSTAGE getNextProcessingStageForAnswer(AnswerStatus::ANSWERPROCESSINGSTAGE iCurrentStage) ;
  AnswerStatus::ANSWERPROCESSINGSTAGE getNextProcessingStageForEvent(AnswerStatus::ANSWERPROCESSINGSTAGE iCurrentStage) ;

  void                                setPostKSStatus(OB1NKS* pKs, BB1Object* pTokenObject, Errcode iErrCode) ;
  void                                setPostKSStatusForQuestion(AnswerStatus* pAnswerStatus, Errcode iErrCode) ;
	void                                setPostKSStatusForAnswer(AnswerStatus* pAnswerStatus, Errcode iErrCode) ;
  void                                setPostKSStatusForEvent(AnswerStatus* pAnswerStatus, Errcode iErrCode) ;

  void stopKs(OB1NKS* pKs) ;

  long int getNexPersistenceHandle() { return _iNextHandle++ ; }

 protected:

	/**
	* \brief Execute the opportunistic Strategy
	*/
	/**
	* Execute the opportunistic Strategy <BR>
	* Execute la strategy opporutniste
	* @param temp la strategie opporutniste
	*/
	bool          ExecuteOpportunisticAction(OB1Strategy* temp) ;
	StrategyState Searching(OB1Strategy* temp) ;
	bool          DeterministicKS(OB1SubStrategy* temp) ;
	bool          DeterministicNode(OB1SubStrategy* temp) ;

	/**
	* \brief Add level in the network (publication method)
	*/
	/**
	* Add level in the network (publication method)
	*/
	void AddLevel(std::vector< Level* >* level, OB1NLevel* ksi) ;

	/**
	* \brief Add trigger (publication method)
	*/
	/**
	* Add trigger (publication method)
	*/
	void AddTrigger(std::vector< Level* >* trigger, OB1NTrigger* trig) ;

	/**
	* \brief Add  result (publication method)
	*/
	/**
	* Add  result (publication method)
	*/
	void AddResult(std::vector<INode *>* temp, OB1NResult* res) ;

	/**
	* \brief create a mask
	*/
	/**
	* create a mask
	*/
	int  createMask(std::string mask) ;

	/**
	* \brief create Level of a KS in graph
	*/
	/**
	* create Level of a KS in graph
	*/
	bool CreateLevel(KsInitStructure* pub, OB1NKS* ksi) ;

	/**
	* \brief create Trigger  of a KS in graph
	*/
	/**
	* create Trigger  of a KS in graph
	*/
	bool CreateTrigger(KsInitStructure* pub, OB1NKS* ksi) ;

	/**
	* \brief create result of a KS in graph
	*/
	/**
	* create result of a KS in graph
	*/
	bool CreateResult(KsInitStructure* pub, OB1NKS* ksi) ;

	/**
	* \brief Add the level validity of a KS in graph
	*/
	/**
	* Add the level validity of a KS in graph
	*/
	void AddValidity(Level* levels, OB1NValidity* lev) ;

	/**
	* \brief Add the level recquired of a KS in graph
	*/
	/**
	* Add the level recquired of a KS in graph
	*/
	void addRecquired(Level* levels, OB1NLevel* lev) ;

	/**
	* \brief Add the validity node and it son in the controler graph
	*/
	/**
	*  Add the validity node and it son in the controler graph <BR>
	*  Insere les informations de validit� d'un KS dans le graph
	*  @param toPlace Publication information
	*  @ksi Pointor' to the KS which have made publication
	*/
	bool CreateValidity(KsInitStructure* toPlace, OB1NKS* ksi) ;

  void reportProcessStartToObject(BB1Object* pAnswer, OB1Strategy* pStrat) ;
  void reportProcessEndToObject(BB1Object* pAnswer, OB1Strategy* pStrat) ;
};

#endif /* End of Controler */

