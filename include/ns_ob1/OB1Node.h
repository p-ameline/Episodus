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
#ifndef  BB1TCNODE_H_
#define BB1TCNODE_H_

class OB1Controler ;
class OB1Node ;
class TypedVal ;
class OB1SubStrategy ;
class OB1Edge ;
class OB1Token ;
class OB1NKS ;
class OB1NLevel ;
class OB1NResult ;
class OB1NValidity ;
class OB1NTrigger ;
class BB1KS ;
class Transfert ;
class BB1Object ;

#include "ns_ob1\BB1Types.h"
#include "ns_ob1\OB1Edge.h"
// #include "ns_ob1\TypedVal.h"
// #include "ns_ob1\OB1Token.h"
#include "ns_ob1\MarkSet.h"
// #include "ns_ob1\Visitor.h"
// #include "ns_ob1\Transfer.h"

//------------------------------------------------------------------------------
// extern const    OB1EdgeType inverseLabel[9][2];
OB1EdgeType     getInverseLabel(OB1EdgeType temp);
void            printSonAndParent(OB1Node* node, std::string& input);
std::string     PrintBB1EdgeType(OB1EdgeType temp);                         /* return a string for an dge type */
std::string     PrintBB1NodeType(OB1NodeTypes temp);                        /* return a string for a node type */
//------------------------------------------------------------------------------
bool testKS(OB1Node* temp, OB1Node* temp2);
bool testOther(OB1Node* temp, OB1Node* temp2);
bool testAnd(OB1Node* temp, OB1Node* temp2);
bool testTrigger(OB1Node* temp, OB1Node* temp2);
bool testValidity(OB1Node* temp, OB1Node* temp2);
bool testLevel(OB1Node* temp, OB1Node* temp2);
bool testResult(OB1Node* temp, OB1Node* temp2);

/**
* \brief Link to a Node
*/
/**
* Link to a node <BR>
* Lien stroké dans un noeud vers un autre noeud
*/
class OB1NodeLink
{
 protected:

  OB1Edge* _Edge ;      /**< Label of the node \brief Label of the node */
  OB1Node* _Node ;      /**< Node of the link \brief Node of the link */
  bool     _bijective ; /**< Is the link is bijective \brief Is the link is bijective  */

  static long _lObjectCount ;

 public:

  /**
  * \brief Constructor
  */
  /**
  * Constructor
  *	@param  node destination node
  * @param  temp  Label of the link
  *	@param  bijective IS the link is bijectif  (true i A-B and B-A and false if A-B)
  */
  OB1NodeLink(OB1Node* node, OB1Edge* temp = NULL, bool bijective = false) ;     /**< default constructor */

  OB1NodeLink(const OB1NodeLink& src) ;

  /**
  * \brief default destructor
  */
  /**
  * Destructor
  */
  ~OB1NodeLink() ;

  OB1NodeLink& operator=(const OB1NodeLink& src) ;

  /**
  * \brief tells if it is bijective
  */
  /**
  * tells if it is bijective  <BR>
  * Indique si la relation existe dans les deux sens.
  * @return true if bijective and false else
  */
  bool        isBijective() { return _bijective ; }

  /**
  * \brief return the node
  */
  /**
  * return the node <BR>
  * renvoie le noeud auxquels le noeud contenant ce lien est relié
  * @return node which is given the node
  */
  OB1Node*    Node(){ return  _Node ; }

  /**
  *  \brief return the Edge
  */
  /**
  * return the Edge
  * @return edge
  */
  OB1Edge*    Edge() { return _Edge ; }

  /**
  * \brief put an Edge
  */
  /**
  * put an egde  <BR>
  * Ajoute une information sur le lien
  */
  void        putEdge(OB1Edge* edge) ;

  /**
  * \brief put a node
  */
  /**
  * put a node  for the link  <BR>
  * On insere le lien vers lequel le lien pointe
  * @param destination node
  */
  void        putNode(OB1Node* node) ;

  /**
  *  \brief put if it is bijectif
  */
  /**
  *  put if it is bijectif
  * Indique si le line entre les deux noeud est bidirectionnel
  * @param bijective true if the link is bijective and false else
  */
  void        putBijective(bool bijective) ;

  /**
  * \brief Compare two link
  */
  /**
  * Test equality with another link <BR>
  * Test l'egalité avec un autre noeud
  * @param  tocomp  link to test
  * @return true if the nodes are equals and false else
  */
  bool        Compare(OB1NodeLink* toComp) ;

  static long getNbInstance()  { return _lObjectCount ; }
  static void initNbInstance() { _lObjectCount = 0 ; }
};

typedef std::vector<OB1NodeLink* >::iterator LinkIterator ; /* iterator for link */

/**
* \brief Common implementation all node of the controler's graph
*/
/**
*  Common implementation all node of the controler's graph  <BR>
*  Interface de base de tous les noeuds présent dans le controler de noeud
*/
class OB1Node
{
 protected:

  OB1NodeTypes                _type ;        /**< node's type \brief node's type */
  std::vector<OB1Token* >     _tokens ;      /**< List of all tokens \brief List of all tokens*/
  std::vector<OB1NodeLink* >  _parent ;      /**< List of the parent's node \brief  List of the parent's node*/
  std::vector<OB1NodeLink* >  _children ;    /**< List of the children's node \brief List of the children's node */
  OB1Controler*               _controlers ;  /**< Pointer to the controler \brief Pointer to the controler  */
  MarkSet                     _attributes ;  /**< Dynamical attributes \brief Dynamical attributes */

  static long                 _lRootObjectCount ;

 public:

  /**
  * \brief Constructor
  */
  /**
  * Constructor
  * @param control Pointor to Controler (used to acces controler's method)
  * @param name 	 Name of the KS (You canot have to KS with same name)
  */
  OB1Node(OB1Controler* control) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  virtual ~OB1Node() ;

  /**
  * \brief return the i's parent
  */
  /**
  *  return the i's parent
  *  Renvoie le noeud enfant numero i
  * \warning No test on test are done. You are to be sure the node exist
  *  @param i parent number
  *  @return parent wanted
  */
  virtual OB1NodeLink* operator() (unsigned int i) ;

  /**
  * \brief return the i's children
  */
  /**
  *  return the i's chield
  *  Renvoie le noeud enfant numero i
  * \warning No test on test are done. You are to be sure the node exist
  *  @param i child number
  *  @return child wanted
  */
  virtual OB1NodeLink* operator[] (unsigned int i) ;

  /**
  * \brief return the number of parent
  */
  /**
  * return the number of parent <BR>
  * Renvoie le nombre de parents du noeud
  * @return Parent's number
  */
  size_t ParentCardinal() { return (_parent.size()) ; }

  /**
  * \brief Return a Pointor to the assiocates KS
  */
	/**
  * Return a Pointor to the assiocates KS <BR>
  * Renvoie un pointeur vers le KS auquel est relié ce noeud.
  */
  OB1NKS* KS() ;

  /**
  * \brief return the number of children
  */
   /**
  *  return the number of children
  *  Renvoie le nobmre d'enfants du noeud
  *  @return Children'number
  */
  size_t ChildrenCardinal() { return (_children.size()) ; }

  /**
  * \brief   Add a child Link
  */
  /**
  * Add a child link <BR>
  * Ajoute un lien enfant au noeud
  * @param  Child link to add
  * @return true if the lin have been date and false if the link already exist
  */
  virtual bool AddChild(OB1NodeLink* link) ;

  /**
  * \brief  Add a parent Link
  */
  /**
  * Add a parent link <BR>
  * Ajoute un lien parent au noeud
  * @param  Parent link to add
  * @return true if the lin have been date and false if the link already exist
  */
  virtual bool AddParent(OB1NodeLink* link) ;

  /**
  * \brief  get the node's type
  */
  /**
  * get the node's type <BR>
  * Renvoie le type de noeud
  * @return Node's Type
  */
  virtual OB1NodeTypes  getType() { return _type ; }

  /**
  * \brief return a Controler pointer
  */
  /**
  *  return a Controler pointer
  *  Renvoie un pointer vers le controler
  */
  OB1Controler* Controler() ;

  /**
  *  \brief reset all information present int he node
  */
  /**
  * reset all information present int he node   <BR>
  * Renvoie toutes les informations sur le noeud
  */
  virtual void Reinit() ;

  /**
  * \brief  tells if to node are equals
  */
  /**
  * tell if two node are equals
  * Indique si deux noeud sont egaux
  * @param test Node to test
  * @return  return true if the nodes are equals and false else
  */
  bool Compares(OB1Node* test) ;

  /**
  * \brief  add a bijective link beetwen to node
  */
  /**
  * add a bijective link beetwen to node
  * Ajoute un lien entre bidirectionnel
  * La classe passé permet de remplir automatiquement le lien inverse dans le lien retour
  * @param link Link to insert
  * @clas class to inverse
  */
  void addBijection(OB1NodeLink* link, int clas = 0) ;

  /**
  * \brief  get a child pointer iterator
  */
  /**
  * get The link iterator of a child <BR>
  * Renvoie le un iterateur d'un enfant
  * @param toFind Node to find
  * @param LinkIterator that contains child's node to find
  */
  virtual LinkIterator findChild(OB1Node* toFind) ;

  /**
  * \brief  get a parent pointer iterator
  */
  /**
  * get The link iterator of a parent <BR>
  * Renvoie le un iterateur d'un parent
  * @param toFind Node to find
  * @param LinkIterator that contains parent node to find
  */
  virtual LinkIterator findParent(OB1Node* toFind) ;

 /**
  * \brief get The link of a parent
  */
  /**
  * get The link of a parent <BR>
  * Renvoie le lien d'un parent
  * @param toFind Node to find
  * @param Link that contains parent node to find
  */
  virtual OB1NodeLink* Parent(OB1Node* toFind) ;

  /**
  * \brief get The link of a child
  */
  /**
  * get The link of a child <BR>
  * Renvoie le lien d'un enfant
  * @param toFind Node to find
  * @param Link that contains children node to find
  */
  virtual OB1NodeLink* Children(OB1Node* toFind) ;

  /**
  * \brief Tell if the one node is a child
  */
  /**
  *  Tell if the one node is a child <BR>
  *  Indique si un noeud est un fils de ce noeud
  * @return Return true if the node is a child and false else
  */
  virtual bool existChild(OB1Node* toFind) ;

  /**
  * \brief Tell if the one node is parent
  */
  /**
  *  Tell if the one node is a parent <BR>
  *  Indique si un noeud est un parent de ce noeud
  * @return Return true if the node is a parent and false else
  */
  virtual bool existParent(OB1Node* toFind) ;

  /**
  *  \brief Print a summary of the node in an HTML string
  */
  /**
  * Print a summary of the node in an HTML string   <BR>
  * Ecrit un résumé du noeud sous forme HTML
  */
  virtual void htmlPrint(std::string& input) ;

  /**
  *  \brief Print a summary of the node in a string
  */
  /**
  * Print a summary of the node in a string   <BR>
  * Ecrit un résumé du noeud sous forme d'une string
  */
  virtual std::string getString() ;

  /**
  * \brief Add a tokens in the node
  */
  /**
  * Add a node in the node <BR>
  * Ajoute un token dans le graph
  * @param token Ajoute un parametre dans le noeud
  */
  virtual void addToken(OB1Token* token) ;
  void         removeToken(OB1Token* token) ;
  OB1Token*    getLastToken(int cl) ;
  bool         isReferencedToken(OB1Token* token) ;

  /**
  * \brief Tells if the node have tokens
  */
  /**
  *  Tells if the node have tokens  <BR>
  *  Indique si le noeud contient des tokens
  *  @return true if the node has no token and false if the node have tokens
  */
  bool isEmpty() { return (_tokens.empty()) ; }

  /**
  * \brief clear the tokens
  */
  /**
  *  clear the tokens <BR>
  *  efface tous les tokens présent dans le node
  */
  virtual void EmptyToken() ;

  /**
  * \brief return the number of token
  */
  /**
  * Return the number of function <BR>
  * Renvoie le nombre de fonction
  * @return Number of token present int the ndoe
  */
  int getTokensCardinal() ;

  /**
  * \brief  Flag with a priority
  */
  /**
  * Put a priority marker in the node  <BR>
  * Insert un marker de priorité dans le noeud
  * @param priority Priority to insert
  */
  void Flag(int priority) ;

  /**
  *  \brief Return the priority of the node
  */
  /**
  * Return the priority of the node <BR>
  * Renvoie la priorité du noeud
  * @return return priority
  */
  int Priority()
  {
    MarkerObject* priority = _attributes.get(std::string("priority")) ;
    if (priority)
      return (priority->intValue()) ;
    return OB1_UNINITIALIZED ;
  }

  /**
  * \brief Remove an int marker
  */
  /**
  * Remove an int marker <BR>
  * Retire un marker entrée
  * @priority Priorityto remove
  */
  void UnFlag(int priority) ;

  /**
  *  \brief evaluate computation time
  */
  /**
  * Evaluate computation time  <BR>
  * Evalue le temps d'execution
  * \warning Not implemented
  * \todo To implement
  * @return Computation time evalutation
  */
  int evaluateComputationTime() ;

  /**
  * \brief Return an attributes in function of name
  */
	/**
  * Return an attributes in function of name <BR>
  * Renvoie un attribut en fonctiond e son nom
  * @return Marker wanted
  */
  MarkerObject* Attributes(std::string& attr) ;

  /**
  *  fill the strategy
  */
  /**
  * Fill the strategy <BR>
  * Remplit la strategie et la complete
  * @param te Strategy to fill
  */
  virtual int createStrategy(OB1SubStrategy* te, bool bInRootDirection) { return 0 ; }

  /**
  * \brief Tell if the node is activable
  */
  /**
  * Tell if the node is activable  <BR>
  * Indique si un noeud est activable
  * @return return true if activable and false else
  */
  virtual bool isActivable() { return (false) ; }

  /**
  *  \brief Return the list of parent' node
  */
  /**
  *  Return the list of parent' node <BR>
  *  Renvoi la liste des noeud parent
  * @param parent'node list
  */
  std::vector<OB1NodeLink* >& getParent() { return (_parent) ; }

  /**
  * \brief Return the list of children' node
  */
  /**
  *  Return the list of children' node <BR>
  *  Renvoi la liste des noeud enfant
  * @param children'node list
  */
  std::vector<OB1NodeLink* >& getChildren()    { return _children ; }

  std::vector<OB1NodeLink* >* getChildrenPtr() { return &_children ; }

  /**
  * \brief Return the marker list
  */
  /**
  * Return the marker list <BR>
  * Renvoi la list de marker (propriété attaché)
  * @param MarkerList
  */
  MarkSet& Attribute() { return _attributes ; }

  bool hasParent(OB1NodeTypes temp) ;

  /**
  * \brief Tell if the node is a trigger
  */
  /**
  * Tell if the node is a trigger <BR>
  * Indique si ce noeud est une trigger pour un KS
  * @return true if the node is a trigger and false else
  */
  std::vector<OB1Node* >* startNode() ;

  void clearParentsLinks() ;
  void clearChildrenLinks() ;

  static long getNbInstance()  { return _lRootObjectCount ; }
  static void initNbInstance() { _lRootObjectCount = 0 ; }
};

/**
* Vector of OB1Node
*/
typedef std::vector<OB1Node* >* OB1VectNodePtr ;

/**
* \brief Representation of a knowledge source in the graph
*/
/**
*  Representation of a knowledge source in the graph <BR>
*  Noeud représentant une source de connaissance dans le graphe
*/
class OB1NKS  : public OB1Node
{
 protected:

  std::string         _ksName ;            /**< Name the KS \brief Name the KS */
  BB1KS*              _ks ;                /**< Pointer to the KS class \brief  Pointer to the KS class*/
  ValidityContextType _ValidationContext ; /**< Validy of the KS in a context \brief Validy of the KS in a context  */
  KSState             _ksState ;           /**< Validy of the KS in a context \brief Validy of the KS in a context  */
  KSType              _ksType ;            /**< Type of the KS  \brief Type of the KS */

  bool                _inKsProcessingDuty ; /**< The KS is processing */

  static long         _lObjectCount ;

 public:

  /**
  * \brief Constructor
  */
  /**
  * Constructor
  * @param control Pointor to Controler (used to acces controler's method)
  * @param name 	 Name of the KS (You canot have to KS with same name)
  */
  OB1NKS(OB1Controler* control, std::string& name) ;

  /**
  * \brief Constructor
  */
  /**
  * Constructor
  * @param control Pointor to Controler (used to acces controler's method)
  * @param name 	 Name of the KS (You canot have to KS with same name)
  * @param ks      Link to the \relates BB1Objet representing the KS
  */
  OB1NKS(OB1Controler* control, std::string& temp, BB1KS* ks) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~OB1NKS() ;

  /**
  *  \brief Reinit all data in the node
  */
  /**
  *  Reinit all data in the node  <BR>
  *  Clear all Token in the node and all the marker used for computation <BR>
  *  Reinitialise le noeud en effancant tous les tokens et les marker utilser
  * pour le calcul et le parcour
  */
  void          Reinit() ;

  /**
  * \brief Get the validity of a KS in a context : can be use for ContextGarbageCollecting
  */
  /**
  *  Get the validity of a KS in a context : can be use for ContextGarbageCollecting
  *  Renvoie le resulat du context validity
  *  @return Contexte validity
  */
  ValidityContextType ContextValidity() { return (_ValidationContext) ; }

  /**
  * \brief Return the type of the KS
  */
  /**
  *  Return the type of the KS <BR>
  *  Renvoie le type du KS
  * @return KS Type
  */
  KSType              KsType() { return (_ksType) ; }

  KSState             KsState() { return (_ksState) ; }

  /**
  * \brief Put the type of the KS
  */
  /**
  *  Put the type of the KS <BR>
  *  Insere le type de KS
  * @param typ KS Type
  */
  void                putKsType(KSType typ) { _ksType = typ ; }

  void                putKsState(KSState stat) { _ksState = stat ; }

  /**
  *  \brief  Get the name of the KS
  */
  /**
  *  Get the name of the KS   <BR>
  *  Renvoie le nom du KS
  *  @return renvoie le nom du KS
  */
  std::string         getKSName() { return (_ksName) ; }

  /**
  * \brief Get a pointer to the result node
  */
  /**
  *  Get a pointer to the result node  <BR>
  *  Renvoie un pointeur vers le noeud indiquant les result fournit par l'execution de ce KS
  *  @return Pointor to the result node
  */
  OB1NResult*         getResultNode() ;

  /**
  * \brief Get a pointer to the  level node
  */
  /**
  *  Get a pointer to the  level node  <BR>
  *  Renvoie un pointeur vers le noeud indiquant les differents niveaux de vraiable nécessaire pour l'execution <BR>
  * Un des niveau peut être obligatoire
  *  @return Pointor to the level node
  */
  OB1NLevel*          getLevelNode() ;

  /**
  *  \brief Get the validity node
  */
  /**
  *  Get the validity node <BR>
  *  Renvoie un pointeur vers le noeud indiquant les differents variable nécessaire pour calculer la validité de ce KS dans ce contexte d'execution <BR>
  *  Ce niveau n'est pas obligatoire
  *  @return Pointor to the level node
  */
  OB1NValidity*       getValidityNode() ;

  /**
  * \brief Get a pointer to the trigger node
  */
   /**
  *  Get the trigger node <BR>
  *  Renvoie un pointeur vers le noeud indiquant les differents variable declenchant l'execution d'un KS <BR>
  *  Ce niveau n'est pas obligatoire
  *  @return Pointor to the level node
  */
  OB1NTrigger*        getTriggerNode() ;

  /**
  * \brief  Print the data in the string in html format
  */
  /**
  *  Print the data in the string in html format  <BR>
  *  Ecrit le contenu de noeud dans une string four forme html
  * @param input string where you print information
  */
  void                htmlPrint(std::string& input) ;

  std::string         getString() ;

  /**
  * \brief Get the \relates BB1Oject that represent the KS
  */
  /**
  * Get the \relates BB1Oject that represent the KS
  * renvoie l'objet qui représente le KS en mémoire
  * @return  BB1Oject'KS
  */
  BB1KS*              getKS() { return (_ks) ; }

  /**
  * \brief Return the data needed for KS computation
  */
  /**
  * Return the Data needed for KS computation  <BR>
  * Retourne une structure qui permet l'execution du KS
  * @return Return the data needed for KS computation
  */
  Transfert*       getActionStructure(OB1SubStrategy* strat = NULL) ;

  /**
  * \brief Return the data needed for KS computation with precision information
  */
  /**
  * Return the data needed for KS computation with precision information  <BR>
  * Retourne une structure qui permet l'execution du KS  en preceisant jusqu'a quel niveau de précision vous voulez descendre
  * @return Return the data needed for KS computation
  */
  Transfert*       getActionStructure(int i) ;

  /**
  * \brief Execute the KS
  */
  /**
  * Execute KS  with data tr <BR>
  * Execute le KS avec les données contenu dans la structure de transfert
  * @param tr Data used for computation
  * @param createToken Tell if the result create a token or not
  */
  Errcode          ExecuteKS(Transfert* tr, OB1Token* pToken, bool createToken = true) ;

  bool             isKSInDuty() { return _inKsProcessingDuty ; }

  /**
  * \brief Compute the precondition and tell if the KS is executable
  */
  /**
  * Compute the precondition and tell if the KS is executable
  * \remarks Not used
  *  Calcul les precondition
  * @return true if you can execute KS and false else
  */
  bool             Precondition(Transfert* tr) ;

  /**
  *   \brief compute Validity
  */
  /**
  * Compute validity <BR>
  * Calcul la validité de ce KS dans ce contexte
  * @return true if the KS can be execute and false else
  */
  bool             ComputeValidity(OB1Token* pToken) ;

  /**
  *  \brief Tell if all the level have an answer
  */
  /**
  *  Tell if all the level have an answer  <BR>
  *  Indique si le KS est activable
  * @return true if the KS is activable and false else
  */
  bool             isActivable() ;

  /**
  * \brief   Tells if the KS have already be executed
  */
  bool             hasAlreadyBeAnswer() ;

  /**
  *  fill the strategy
  */
  /**
  * Fill the strategy <BR>
  * Remplit la strategie et la complete
  * @param te Strategy to fill
  */
  int              createStrategy(OB1SubStrategy* te, bool bInRootDirection) ;

  static long getNbInstance()  { return _lObjectCount ; }
  static void initNbInstance() { _lObjectCount = 0 ; }
};

/**
* \brief Condition node in the controler's graph
*/
/**
*  Condition node in the controler's graph <BR>
*  Noeud représentant une condition dans le graphe.  <BR>
* En général la condition et la présence d'un objet sur le BB
*/
class OB1NOther: public OB1Node
{
 protected:

  TypedVal*   _path ;           /**< path of the question like VAGE ou LSEXE ... \brief path of the question loke VAGE ou LSEXE ... */
  TypedVal*   _searchData ;     /**< complementary information for search */
  bool        _DefaultCompute ; /**< have already use the default computation function  \brief have already use the default computation function  */

  static long _lObjectCount ;

 public:

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~OB1NOther();

  /**
  * \brief constructor
  */
  /**
  * constructor
  * @param control Pointor to use Controler's method
  */
  OB1NOther(OB1Controler* control);

  /**
  * \brief constructor
  */
  /**
  * constructor
  * @param control Pointor to use Controler's method
  * @param dipat Name dispatcher
  */
  OB1NOther(OB1Controler* control, TypedVal* dispat) ;

  /*
  * \brief Say it have already be computed ?
  */
  /**
  *  Say it have already be computed ? <BR>
  *  Indique si le noeud a été calculé.
  */
  void Computed() ;

  /**
  *  \brief  Say it's have been already computed
  */
  /**
  * Say it's have been already computed <BR>
  * Indique que le noeud a daja été calculé
  * @return true if the node have already be computed and false else
  */
  bool IsComputed() { return _DefaultCompute ; }

  /**
  * \brief Return the dispatch information
  */
  /**
  * Return the dispatch information <BR>
  * Renvoie l'information de dispatchage
  * @return Disptatch Information
  */
  TypedVal& getPath()         { return (*_path) ; }
  TypedVal& getSearchStruct() { return (*_searchData) ; }

  /**
  *  \brief Print a summary of the node in a string
  */
  /**
  * Print a summary of the node in a string   <BR>
  * Ecrit un résumé du noeud sous forme HTML
  */
  void htmlPrint(std::string& input);

  std::string getString() ;

  /**
  * \brief Add ses informations
  */
  void addActionStructure(Transfert* temp, int trans, std::string sPrefixe = std::string(""), OB1Token* pToken = (OB1Token *) 0, long int iPersistenceHandle = -1) ;

	/**
  * \brief  Tells if it's token place
  */
  /**
  * Tells if it's token place <BR>
  * Indique si un token peut être stoké dans ce noeud.
  * Si le tokens peut être stocké, il est rajouté dans la liste des tokens du noeud
  * @param temp  Token to keep ?
  * @param dispat  dsipatch information (value that is compare to node to know if you have to keep the toke)
  * @return  true if the node has been keep in this node and fals else
  */
  bool IsPlace(OB1Token* temp, TypedVal& dispat) ;

  /**
  * \brief  tell if the node is ok
  */
  bool isActivableExt(int clas) ;

  /**
  * \brief Tells if the ks have already be executed
  */
  /**
  *  Tells if the ks have already be executed
  * \remarks Seems to be not used
  */
  bool hasAlreadyBeAnswer() ;

  BB1Object* getAnswerObject() ;
  BB1Object* getAnswerObjectIfProcessed() ;

  /**
  *  fill the strategy
  */
  /**
  * Fill the strategy <BR>
  * Remplit la strategie et la complete
  * @param te Strategy to fill
  */
  int  createStrategy(OB1SubStrategy* te, bool bInRootDirection) ;
  void fillConsistentStrategiesList(OB1SubStrategy* te, std::vector<OB1Node*>* pExploreChoice) ;

  /**
  * \brief Tell if the node is a leaf
  */
  /**
  * Tell if the node is a leaf <BR>
  * Indique si le noeud est une feuille (c-a-d) n'a pas de fils)
  * return true is the node is a leaf and false else
  */
  bool isLeaf() ;

  /**
  * \brief Erase all information in the node
  */
  /**
  *  Erase all information in the node  <BR>
  *  Efface les tokens et les markers
  */
  void Reinit() ;

  static long getNbInstance()  { return _lObjectCount ; }
  static void initNbInstance() { _lObjectCount = 0 ; }
};

/**
* \brief  Node representing and condition
*/
/**
*  Node representing and condition<BR>
*  Noeud représentant une conditon de type et. <BR>
*  Signifie qu'il faut que tous les sous noeud soient OK pour l'execeution
*/
class OB1NAnd: public OB1Node
{
  private:

    static long _lObjectCount ;

  public:

  /**
  * \brief constructor
  */
  /**
  * constructor
  * @param control Pointor to use Controler's method
  */
  OB1NAnd(OB1Controler* control) ;

   /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~OB1NAnd() ;

  /**
  *  \brief Print a summary of the node in a string
  */
  /**
  * Print a summary of the node in a string   <BR>
  * Ecrit un résumé du noeud sous forme HTML
  */
  void htmlPrint(std::string& input) ;

  std::string getString() ;

  /**
  * \brief Get all information for computation
  */
  /**
  * Get all information for computation     <BR>
  * Récupere toutes les informations nécessaire pour l'execution   <BR>
  * \remarks Cette fonction peut être appelé lors de la construction de la
  * structure transféré pour le calcul de validité ou pour l'execution
  * @temp struture to fill
  */
  void getActionStructure(Transfert* temp) ;
  bool isActivable() ;

  /**
  *  fill the strategy
  */
  /**
  * Fill the strategy <BR>
  * Remplit la strategie et la complete
  * @param te Strategy to fill
  */
  int  createStrategy(OB1SubStrategy* te, bool bInRootDirection) ;

  static long getNbInstance()  { return _lObjectCount ; }
  static void initNbInstance() { _lObjectCount = 0 ; }
};

/**
* \brief  Node representing the trigger condition of the KS
*/
/**
*  Node representing the trigger condition of the KS<BR>
*  Noeud représentant les conditions de déclenchement d'un KS
*/
class OB1NTrigger : public OB1Node
{
  private:

    static long _lObjectCount ;

  public:
  /**
  * \brief constructor
  */
  /**
  * constructor
  * @param control Pointor to use Controler's method
  */
  OB1NTrigger(OB1Controler* control) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~OB1NTrigger() ;

  /**
  *  \brief Print a summary of the node in a string
  */
  /**
  * Print a summary of the node in a string   <BR>
  * Ecrit un résumé du noeud sous forme HTML
  */
  void htmlPrint(std::string& input) ;

  std::string getString() ;

  /**
  * \brief Get all information for computation
  */
  /**
  * Get all information for computation     <BR>
  * Récupere toutes les informations nécessaire pour l'execution   <BR>
  * @temp struture to fill
  */
  int  createStrategy(OB1SubStrategy* te, bool bInRootDirection) ;

  void getActionStructure(Transfert* temp) ;

  static long getNbInstance()  { return _lObjectCount ; }
  static void initNbInstance() { _lObjectCount = 0 ; }
} ;

/**
* \brief  Node representing the KS needed data for execution
*/
/**
*  Node representing the KS needed data for execution <BR>
*  Noeud représentant les donnée nécessaire pour l'execution d'un KS
*/
class OB1NLevel : public OB1Node
{
  private:

    static long _lObjectCount ;

  public:
  /**
  * \brief constructor
  */
  /**
  * constructor
  * @param control Pointor to use Controler's method
  */
  OB1NLevel(OB1Controler* control) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~OB1NLevel() ;

  /**
  *  \brief Print a summary of the node in a string
  */
  /**
  * Print a summary of the node in a string   <BR>
  * Ecrit un résumé du noeud sous forme HTML
  */
  void htmlPrint(std::string& input) ;

  std::string getString() ;

  /**
  * \brief  fill the action structure
  */
  /**
  * fill an action structure   <BR>
  * Remplit une struction permettant l'execution des KS
  * @param temp structure to fill
  */
  void getActionStructure(Transfert* temp) ;

  /**
  * \brief fill the action structure for some level
  */
  /**
  * fill an action structure   <BR>
  * Remplit une struction permettant l'execution des KS avec un certaind egré de précision
  * @param temp structure to fill
  */
  void getActionStructure(Transfert* temp, int i) ;

  /**
  * \brief Get all information for computation
  */
  /**
  * Get all information for computation     <BR>
  * Récupere toutes les informations nécessaire pour l'execution   <BR>
  * @temp struture to fill
  */
  int  createStrategy(OB1SubStrategy* te, bool bInRootDirection) ;

  /**
  * \brief Tell if the node is activable
  */
  /**
  *  Tell if the node is activable <BR>
  *  Indique si un noeud est activable
  * @return true if activable and false else
  */
  bool isActivable() ;

  static long getNbInstance()  { return _lObjectCount ; }
  static void initNbInstance() { _lObjectCount = 0 ; }
} ;

/**
* \brief  Node representing the result of the computation of a KS
*/
/**
*  Node representing the result of the computation of a KS <BR>
*  Noeud représentant le fait q'un KS a des données.
*/
class OB1NResult : public OB1Node
{
  private:

    static long _lObjectCount ;

  public:

  /**
  * \brief constructor
  */
  /**
  * constructor
  * @param control Pointor to use Controler's method
  */
  OB1NResult(OB1Controler* control) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~OB1NResult() ;

  /**
  *  \brief Print a summary of the node in a string
  */
  /**
  * Print a summary of the node in a string   <BR>
  * Ecrit un résumé du noeud sous forme HTML
  */
  void htmlPrint(std::string& input) ;

  std::string getString() ;

  /**
  * \brief Tell if one result node has a token
  */
  /**
  *  Tell if one result node has a token <BR>
  *  Indique si au moins un noeud resultat du KS possede une réponse
  * @return true if the answer exsit and false else
  */
  bool hasResult() ;

  /**
  *  fill the strategy
  */
  /**
  * Fill the strategy <BR>
  * Remplit la strategie et la complete
  * @param te Strategy to fill
  */
  int  createStrategy(OB1SubStrategy* te, bool bInRootDirection) ;

  void getActionStructure(Transfert* temp, OB1SubStrategy* strat = NULL) ;

  static long getNbInstance()  { return _lObjectCount ; }
  static void initNbInstance() { _lObjectCount = 0 ; }
} ;

/**
* \brief Node link to KSNode that describe the validity condition
*/
/**
*  Node link to KSNode that describe the validity condition <BR>
*  Liens relié à un noeud KS représeant que ce KS a des condition de validité a remplir
*/
class OB1NValidity : public OB1Node
{
  private:

    static long _lObjectCount ;

  public:

  /**
  * \brief constructor
  */
  /**
  * constructor
  * @param control Pointor to use Controler's method
  */
  OB1NValidity(OB1Controler* control) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~OB1NValidity() ;

  /**
  * \brief Create a structure that is used for validity's computation
  */
  /**
  * Create a structure that is used for validity's computation <BR>
  * Calcul une structure receuillant les données nécessaires au calcul
  * de validité. Cette données sont recoltés en parcourant les fils
  * de la structure
  * @return Computation structure for validity's method
  */
  Transfert*  getActionStructure(long int iPersistenceHandle = -1) ;

  /**
  *  \brief Print a summary of the node in a string
  */
  /**
  * Print a summary of the node in a string   <BR>
  * Ecrit un résumé du noeud sous forme HTML
  */
  void        htmlPrint(std::string& input) ;

  std::string getString() ;

  /**
  * \brief Tells if the validity can be compute
  */
  /**
  * Tells if the validity can be compute <BR>
  * Indique si chaque sous noeud possede les odnnée permetant de verifier
  * qu'un fichier peut être isntallé.
  */
  bool        CanCompute() ;

  static long getNbInstance()  { return _lObjectCount ; }
  static void initNbInstance() { _lObjectCount = 0 ; }
} ;

#endif     /* end of OB1Node.h */

