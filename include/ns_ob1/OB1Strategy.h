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

#ifndef   OB1STRATEGY_H_
#define   OB1STRATEGY_H_

class OB1Strategy ;
class OB1Controler ;
class OB1Token ;
class OB1Node ;
class OB1NKS ;

// #include "ns_ob1\OB1Node.h"
// #include "ns_ob1\OB1Token.h"
#include <fstream.h>
#include "partage\NTArray.h"
#include "ns_ob1\BB1Types.h"
#include "ns_ob1\OB1Types.h"

/**
* \brief State of resolution of a problems
*/
/**
* State of a problems <BR>
* Etat d'un problème en mode
*/
enum StrategyState
{
  IN_EXECUTION = 0, /**< We are computing the result <BR> La résolution esn cours \brief  We are computing the result*/
  NO_ANSWER = 2,    /**< We cannot have the answer   <BR> Nous n'aurons pas de réponse \brief We cannot have the answer  */
  ANSWER = 1   			/**< We have the result		<BR> Nous avons la solution. Ce n'est pas la peine de continuer	\brief We have the result */
};

/**
* \brief A strategy is a node in the tree controler to execute.
*/
/**
* A strategy is a node in the tree controler to execute.
* Une sous stratégie est un noeud a executer dans le graph du controleur
*/
class OB1SubStrategy
{
  friend class OB1SubStrategy ;

 protected:

	OB1Controler*                 _controlers ;      // < Pointer to the controler \brief Pointer to the controler

	OB1Token*                     _run ;             // < runnable token \brief runnable token
	OB1Node*                      _node ;            // < Node containind by the strategy it can be ks or other \brief Node containind by the strategy it can be ks or other
	OB1Strategy*                  _root ;            // < root's strategy \brief root's strategy
	OB1SubStrategy*               _father ;          // < Father of the strategy \brief Father of the strategy
	// std::vector<OB1SubStrategy* > _subStrategy;
	NTArray<OB1SubStrategy>       _subStrategy ;     // < Substrategy to execute \brief Substrategy to execute
	StrategyState                 _isAnswer ;        // < Is the anwser have be answer \brief Is the anwser have be answer
	int                           _wanted_class ;    // < Substrategy to execute \brief Substrategy to execute
	int                           _pass_class ;      // < Pass for accessing other class \brief Pass for accessing other class

 public:

  	/**
    * \brief Constructor
    */
    /**
    * Constructor
    * @param root The root strategy
    * @param father The local father streaty (or substrategy)
    * @param tells the class information that is needed
    * @param  pass_class
    */
    // OB1SubStrategy(OB1SubStrategy* root, OB1SubStrategy* father, int _wanted_clas, int pass_class);
    OB1SubStrategy(OB1Controler* pCtrler, OB1Strategy* root, OB1SubStrategy* father, int _wanted_clas, int pass_class) ;

    /**
    * \brief copy constructor
    */
    /**
    *  Copy constructor
    *  @param  temp SubStrategy to clone
    */
    OB1SubStrategy(OB1SubStrategy& temp) ;

    /**
    * \destructor
    */
    ~OB1SubStrategy() ;

    /**
    * \brief get the passed'class
    */
    /**
    *  get the passed'class <BR>
    *  @return get the passed class
    */
    int                                   Class();

    /**
    * \brief get the root of the strategy
    */
    /**
    *  get the root of the strategy <BR>
    *  Retourne la strategie racine
    *  @return root's strategy
    */
    OB1Strategy*                          Root();

    /**
    *  \brief return all strategy
    */
    /**
    * return all strategy <BR>
    * Renvoie toutes les strategie
    * @return list of all sub strategy
    */
    NTArray<OB1SubStrategy>&              getSubStrategy();
    // std::vector<OB1SubStrategy* >&        getSubStrategy();

    /**
    * \brief Not implemented
    */
    /**
    * \warning Not implemented
    * @return return false
    */
    bool                                  hasAnswer();

    /**
    * \brief Put the node in the strategy
    */
    /*
    * Put the node in th strategy <BR>
    * Insere un noeud dans une sous strategie
    * @param temp node to insert in the strategy
    */
    void                                  putNode(OB1Node* temp);

    /**
    * \brief Get the token which is in the substrategy
    */
    /**
    * Get the token which is in the substrategy <BR>
    * Renvoie le token contenu dans cette strategie
    * @return token present in the sub strategy
    */
    OB1Token*                             Token();

    /**
    * \brief Put token in the strategy
    */
    /*
    * Put the token in th strategy <BR>
    * Insere un token dans une sous strategie
    * @param temp node to insert in the strategy
    */
    void                                  putToken(OB1Token* tok );

    /**
    * \brief Return the node to execute in this strategy
    */
    /**
    *  Return the node to execute in this strategy  <BR>
    *  Renvoie le noeud a utiliser dans la strategie
    *  @return node to execute
    */
    OB1Node*                              Node();

    /**
    * \brief Return the local father strategy
    */
    /**
    *  Return the local father strategy <BR>
    *  Renvoie la strategy parent
    *  @return Paretn strategy
    */
    inline OB1SubStrategy*                Father() {return (_father); }


    /**
    *  \brief put the computation finished
    */
    /**
    *  put the computation finished  <BR>
    *  Inqieu l'etat actuelle de la résolution du probleme auquel appartient cette strategie
    *  @param i Computation strategy's state
    */
    void                                  putComputationState(StrategyState i);

    /**
    * Get the computation' state
    */
    /**
    * Get the computation' state <BR>
    * Renvoie l'etat de résolution du problème
    * @return Computation strategy's state
    */
    StrategyState                         getStrategyState();

    /**
    * \brief Check if a token of the wanted's class existe in this node
    */
    /**
    * Check if a token of the wanted's class existe in this node <BR>
    * Si le noeud n'est pas un KS alors nous verifions si le noeud
    * possede un token de cette classe
    * @return true if the node has solution and false else
    */
    bool                                  HasWantedClass();                               

    /**
    * \brief Put the wanted class
    */
    /**
    * Put the wanted class <BR>
    * Indique la class d'information désiré
    * @param clas wanted class
    */
    void                                  WantedClass(int clas);

    /**
    * \brief  Return the wanted class
    */
    /**
    *  Return the wanted class <BR>
    *  Renvoie la class voulue
    * @return wanted class
    */
    int                                   getWantedClass() {return _wanted_class; }

    /**
    * \brief Add a sub strategy to solve
    */
    /**
    * Add a sub strategy to solve  <BR>
    * Ajoute une sous strategy nécessaire a la résolution du probleme
    * @param temp sustrategy to add
    */
    void                                  AddSubStrategy(OB1SubStrategy* temp);

    /**
    * \brief Remove the last sub strategy
    */
    /**
    * Remove the last sub strategy <br>
    * Efface la dernier sous strategy
  	* \todo L'element est retirer mais ne semble pas être delete
    */
    void                                  RemoveLastSonStrategy() ;

    void                                  SkipToNextNode() ;

    OB1NKS*                               getKSNode() ;

    string                                describe() ;
};


/**
* \brief  A strategy is a path to execute in the graph'controler
*/
/*
**  Une stratégie est une classe servant a contenir les informations sur un
**  problème donné. Son but est de pouvoir permettre l'ordonnancement entre
**  différents problèmes en cours de résolution.
**  Une stratégie est la composante principales du controler en mode déterministe.
**  Une statégie est une séquence de noeuds à executer 
*/
class OB1Strategy : public  OB1SubStrategy
{
  friend class OB1SubStrategy;

  protected:
  int             _count ;           // <  Number of  cycle executed by strategy \brief  Number of  cycle executed by strategy
  int             _last ;            // < Dernier cycle ou cette strategie a été executé \brief ernier cycle ou cette strategie a été executé
  Cycle           _beginCycle ;      // < Cycle auquels la stratégie a été commencer \brief Cycle auquels la stratégie a été commencer*/
  int             _priority ;        // < Priority of the strategy  \brief Priority of the strategy
  TStrategyType   _StratType ;       // < Opportuniste | deterministe  \brief Opportuniste | deterministe
  bool            _delete ;          // < To delete the Strategy \brief To delete the Strategy
  std::string     _id ;              // < Id of the strategy \brief Id of the strategy
  OB1SubStrategy* _lastStrategy ;

  public:
  /**
  * \brief constructor
  */
  /**
  * Constructor
  * The identifiant is given by  OB1Controler::StrategyCount
  * @param id Unique identifiant fo strategy.
  * @param  Class of the object creator
  */
  OB1Strategy(OB1Controler* pCtrler, std::string id, int clas) ;

  /**
  * \brief constructor
  */
  /**
  * Constructor
  * The identifiant is given by  OB1Controler::StrategyCount
  * @param id Unique identifiant fo strategy.
  * @param  Class of the object creator
  * @param  Priority of the strategy (The priority is used by shedulin algorithm)
  * @param begin_cycle Creation streategy's cycle
  */
  OB1Strategy(OB1Controler* pCtrler, std::string id, int clas, int priority, Cycle begin_cycle) ;

  /**
  * \brief constructor
  */
  /**
  * Constructor
  * The identifiant is given by  OB1Controler::StrategyCount
  * @param id Unique identifiant fo strategy.
  * @param  Class of the object creator
  * @param  head Token of the question
  * @param  Priority of the strategy (The priority is used by shedulin algorithm)
  * @param begin_cycle Creation streategy's cycle
  */
  OB1Strategy(OB1Controler* pCtrler, std::string id, int clas, OB1Token *head, int priority, Cycle begin_cycle) ;

  /**
  * \brief constructor
  */
  /**
  * Constructor
  * The identifiant is given by  OB1Controler::StrategyCount
  * @param id Unique identifiant fo strategy.
  * @param  Class of the object creator
  * @param temp Type of strategy (used for opportunistic stategy )
   * @param  prior of the strategy (The priority is used by shedulin algorithm)
  */
  OB1Strategy(OB1Controler* pCtrler, std::string id, int clas, TStrategyType temp, int prior) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~OB1Strategy() ;

  /**
  * \brief Tells if the garbage have to delete this strategie
  */
  /**
  *  Tells if the garbage have to delete this strategie <BR>
  *  Indique si le ramasse strategie peut effacer cette strategie.
  *  @return return true  if you want the garbage collector to delete this strategie and false else
  */
  inline bool       Delete(){ return (_delete); }

  /**
  * \brief Put the strategy in Detetable state
  */
  /**
  * Put the strategy in Detetable state
  * Force la strategy en mode effacement.
  */
  inline void       ToDelete(){ _delete = true; }
  OB1SubStrategy*   LastSubstrategy() { return (_lastStrategy); }


  /**
  *  problem's priority   <BR>
  *  Renvoie la priorité du problème
  *  @return  problem's priority
  */
  int               Priority();                                     /* return problem's priority */

  /**
  * \brief  Tells the cycle numer wich was created the strategy
  */
  /**
  *  Tells the cycle numer wich was created the strategy <BR>
  *  Nous indique a quel cycle a été crée la stratégie
  *  @return  Creation's strategy cycle
  */
  Cycle             getBeginCycle();

  /**
  * \brief Number elapsed cycle since last execution
  */
  /**
  *  Number elapsed cycle since last execution <BR>
  *  Nous inqique le nombre de cycle passé depuis la derniere execution
  * @return  Number elapsed cycle since last execution
  */
  int               getLastExecution();

  /**
  * Add unity to the var wich indicates the number of cycle since the last execution 
  */
  /**
  *  Add unity to the var wich indicates the number of cycle since the last execution  <BR>
  *  Incremente le compteur indiquant le nombre de cycle depuis la derniere execution
  */
  void              inc();
  int               getCountCycle();                                /* return of executed cycle */

  /**
  * \brief Tells the strategie's type
  */
  /**
  * Tells the strategie's type  <BR>
  * Renvoie le type de la stratégie
  * @return  strategie's type
  */
  TStrategyType     geStratType() { return (_StratType) ; }

  /**
  * \brief get Id of the strategy
  */
  /**
  * get Id of the strategy  <BR>
  * Renvoi l'identifiant de strategy
  * @return Strategy's id
  */
  inline
  std::string       ID() { return (_id) ; }

  inline void       putLastSubStrategy(OB1SubStrategy* last) { _lastStrategy = last; }
};

#endif  /* End of Tstrategy */

