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

#ifndef   OB1GRAPH_H_
#define   OB1GRAPH_H_

class OB1Node ;
class OB1NKS ;
class OB1NValidity ;
class OB1NTrigger ;
class OB1NAnd ;
class OB1NResult ;
class OB1NLevel ;
class OB1NOther ;

#include "ns_ob1\OB1Types.h"
#include "ns_ob1\TypedVal.h"
#include "ns_ob1\OB1Node.h"
#include <map>

typedef  std::vector<OB1Node* > PointerBBNodeVector;

/**
* \brief Graph used int the controler
*/
/**
* Graph used in the controler <BR>
* Graph utilisé par le controleur. <BR>
* Le Graph contient tous les noeud par type (noeud KS, Noued resultats , ... )
*/
class OB1Graph
{
  protected:

    std::vector<OB1Node*>             _AllNode ;        /**< All node in the KS \brief  All node in the KS */

    std::vector<OB1NKS*>              _Ks_nodes ;       /**< All KS Node \brief  All KS Node  */
    std::vector<OB1NValidity*>        _Validity_nodes ; /**< All validity Node \brief All validity Node */
    std::vector<OB1NTrigger*>         _Trigger_nodes ;  /**< All trigger Node \brief  All trigger Node*/
    std::vector<OB1NAnd*>             _And_nodes ;      /**< And node \brief  And node  */
    std::vector<OB1NResult*>          _Result_Nodes ;   /**< All result Node \brief All result Node */
    std::vector<OB1NLevel*>           _Level_node ;     /**< All Level Node \brief All Level Node */
    std::map<std::string, OB1NOther*> _Other_nodes ;    /**< All Other Node \brief All Other Node  */
    unsigned int                      _graph_size ;

    OB1Controler*                     _pControler ;

  public:

    /**
    * \brief Constructor
    */
    /**
    *  Constructor
    */
    OB1Graph(OB1Controler* pControler) ;

    /**
    * \brief Copy Constructor
    */
    /**
    * Copy Constructor
    */
    OB1Graph(const OB1Graph& gr) ;

    /**
    * \brief Destructor
    */
    /**
    * Copy Destructor
    */
    ~OB1Graph() ;

    /**
    * \brief Return the number of vertices in the graph
    */
    /**
    * Return the number of vertices in the graph  <BR>
    * Retourne le nombre de noeud dans le graphe
    * @return vertices number
    */
    inline
    const int     numVertices() const;

    /**
    * \brief Add a node in the graph
    */
    /**
    * Add a node in the graph <BR>
    * Ajoute un noueud dans le graph.
    * @param nod Noeud to add
    */
    void          AddVertex(OB1Node* nod);

    /**
    * \brief Tells if the graph is empty
    */
    /*
    * Tells if the graph is empty   <BR>
    * Indique si le graph est vide
    * @return true if the graph is empty and false else
    */
    bool          isEmpty() const;

    /**
    * \brief  full the graph with the list of vertex :temp
    */
    /**
    * full the graph with the list of vertex
    * @param temp pointer to full
    */
    void          putAllVertex(PointerBBNodeVector& temp);
    #ifdef __OB1__UNOPTIM__
    void          print(std::ostream& ostr) const;                      /* Print information contains in the Graph */
    #endif
    inline
    OB1Node*    operator[] (unsigned int i) const;
    const int OB1Graph::Degree(OB1Node& vert) const;

    /*
    ** \brief return all vertex of the graph
    */
    inline
    PointerBBNodeVector& getAllVertex();

    /**
    *  \brief export graph in a file
    */
    /**
    * export graph in a file
    * @param Nam of th file
    */
    void save(std::string& FileName) ;

    /**
    * \brief Return an arbitrary vertex
    */
    /**
    * Return an arbitrary vertex <BR>
    * Renvoie un noeud arbitraire
    */
    OB1Node* aVertex() ;

    /**
    * \brief find a node
    */
    int find(OB1Node* temp) ;

    /**
    * \brief Find a ks node
    */
    /*
    * Find a ks node  <BR>
    * Recherche un KS
    * @param temp KS to find
    * @return KS if it has been find and NULL else
    */
    inline
    OB1NKS* find_ks_nodes(OB1NKS* temp) ;

    /**
    * \brief Find a other node
    */
    /*
    * Find a other node  <BR>
    * Recherche un noeud other
    * @param temp other node to find
    * @return other node if it has been find and NULL else
    */
    inline
    OB1NOther* find_other_nodes(OB1NOther* temp) ;

    /**
    * \brief Find a other node
    */
    /*
    * Find a level node   <BR>
    * Recherche un noeud level
    * @param temp level node to find
    * @return level node if it has been find and NULL else
    */
    inline
    OB1NLevel* find_level_nodes(OB1NLevel* temp) ;

    /**
    * \brief Find a trigger node
    */
    /*
    * Find a trigger node   <BR>
    * Recherche un noeud trigger
    * @param temp trigger node to find
    * @return trigger  node if it has been find and NULL else
    */
    inline
    OB1NTrigger* find_trigger_nodes(OB1NTrigger* temp) ;

    /**
    * \brief Find a and node
    */
    /*
    * Find a and node   <BR>
    * Recherche un noeud and
    * @param temp and node to find
    * @return and node if it has been find and NULL else
    */
    inline
    OB1NAnd* find_and_nodes(OB1NAnd* temp) ;

    /**
    * \brief Find a validity node
    */
    /*
    * Find a validity node   <BR>
    * Recherche un noeud validity
    * @param temp validity node to find
    * @return validity node if it has been find and NULL else
    */
    inline
    OB1NValidity* find_validity_nodes(OB1NValidity* temp) ;

    /**
    * \brief Find a result node
    */
    /*
    * Find a result node   <BR>
    * Recherche un noeud result
    * @param temp result node to find
    * @return result node if it has been find and NULL else
    */
    inline
    OB1NResult* find_result_nodes(OB1NResult* temp) ;

    /**
    * \brief Find a other node
    */
    /*
    * Find a other node   <BR>
    * Recherche un noeud other
    * @param temp other node to find
    * @return other node if it has been find and NULL else
    */
    inline
    OB1NOther* find_other_nodes(TypedVal& val, TypedVal& searchStruct) ;

    /**
    * \brief get all KSs
    */
    /**
    * get all KSs <BR>
    * Renvoie tous les KS
    * @return List of all KSs
    */
    inline
    std::vector<OB1NKS* >* getKSs() ;

    /**
    * \brief get a pointer on the map of OB1NOther nodes
    */
    /**
    * get a pointer on the map of OB1NOther nodes <BR>
    * Renvoie un pointeur sur la map de otherNodes
    * @return a pointer on the map of otherNodes
    */
    inline
    std::map<std::string, OB1NOther* >* getOtherNodes() ;

    /**
    * \brief get all KSs of a certain type
    */
    /**
    * get all KSs of a certain type <BR>
    * Renvoie tous les KS d'un certains type
    * @param typ Type to find
    * @return List of all KSs
    */
    inline
    std::vector<OB1NKS* >* getKSOfType(KSType typ) ;

    /**
    * \brief get a KSs by its name
    */
    /**
    * get the KS of a given name <BR>
    * Renvoie le KS par son nom
    * @param typ Type to find
    * @return List of all KSs
    */
    inline
    OB1NKS* getKSByName(string sName) ;

    void testOtherNodes() ;
};

inline
OB1Node*
OB1Graph::operator[] (unsigned int i) const
{
  if (i >= _AllNode.size())
    return (OB1Node*) 0 ;

  return _AllNode[i] ;
}

inline
void
OB1Graph::putAllVertex(std::vector<OB1Node*>& temp)
{
  if (temp.empty())
    return ;

  std::vector<OB1Node*>::iterator it = temp.begin() ;
  for ( ; temp.end() != it ; it++)
    _AllNode.push_back(*it) ;

  //  for (unsigned int i = 0; i < temp.size(); i++)
  //    _AllNode->push_back(temp[i]) ;
}

/**
* \brief Return vertices numbers
*/
/**
* Return vertices numbers <BR>
* Renvoie le nombre de sommet total présent dans le graph
* @return vertices number
*/
inline
const int
OB1Graph::numVertices() const
{
	return _AllNode.size() ;
}

// Recherche un KS dans la lsite des KS
inline
OB1NKS*
OB1Graph::find_ks_nodes(OB1NKS* temp)
{
  if (_Ks_nodes.empty())
    return (OB1NKS*) 0 ;

  std::vector<OB1NKS*>::iterator it = _Ks_nodes.begin() ;
  for ( ; _Ks_nodes.end() != it ; it++)
    if (true == testKS(*it, temp))
      return *it ;

  return (OB1NKS*) 0 ;

/*
  register unsigned int end = _Ks_nodes->size() ;
  for (register unsigned int i = 0; i < end ; i++)
    if (true == testKS((*_Ks_nodes)[i], temp))
      return ( (*_Ks_nodes)[i]) ;
  return NULL ;
*/
}

inline
OB1NOther*
OB1Graph::find_other_nodes(OB1NOther* temp)
{
  if (NULL == temp)
    return (OB1NOther*) 0 ;

	return (find_other_nodes(temp->getPath(), temp->getSearchStruct())) ;
}

//
// Remember: there is only one node with a given path
//
inline
OB1NOther*
OB1Graph::find_other_nodes(TypedVal& val, TypedVal& searchStruct)
{
	if (_Other_nodes.empty())
		return (OB1NOther*) 0 ;

  // return (((*_Other_nodes)[ComputeSortingId(val)])) ;

  NSSearchStruct* pSearchParams = searchStruct.getSearchPrm() ;

  std::map<std::string, OB1NOther* >::iterator otherIter = _Other_nodes.begin() ;
  for ( ; _Other_nodes.end() != otherIter ; otherIter++)
  {
    OB1NOther* pOtherNode = otherIter->second ;

    if (pOtherNode->getPath() == val)
    {
      TypedVal SP = pOtherNode->getSearchStruct() ;
      if ((NULL == pSearchParams) || pSearchParams->isDefault())
      {
        if ((NULL == SP.getSearchPrm()) || SP.getSearchPrm()->isDefault())
          return pOtherNode ;
      }
      else
      {
        if (SP.getSearchPrm() && SP.getSearchPrm()->isSameAs(*pSearchParams))
          return pOtherNode ;
      }
    }
  }

  return (OB1NOther*) 0 ;
}

inline
OB1NLevel*
OB1Graph::find_level_nodes(OB1NLevel* temp)
{
  if (_Level_node.empty())
    return (OB1NLevel*) 0 ;

  std::vector<OB1NLevel*>::iterator it = _Level_node.begin() ;
  for ( ; _Level_node.end() != it ; it++)
    if (true == testLevel(*it, temp))
      return *it ;

  return (OB1NLevel*) 0 ;

/*
	register unsigned int end = _Level_node->size() ;
  for (register unsigned int i = 0; i < end ; i++)
  	if (true == testLevel((*_Level_node)[i], temp))
    	return ((*_Level_node)[i]) ;
  return NULL ;
*/
}

inline
OB1NTrigger*
OB1Graph::find_trigger_nodes(OB1NTrigger* temp)
{
  if (_Trigger_nodes.empty())
    return (OB1NTrigger*) 0 ;

  std::vector<OB1NTrigger*>::iterator it = _Trigger_nodes.begin() ;
  for ( ; _Trigger_nodes.end() != it ; it++)
    if (true == testTrigger(*it, temp))
      return *it ;

  return (OB1NTrigger*) 0 ;

/*
  register unsigned int end = _Trigger_nodes->size() ;
  for (register unsigned int i = 0; i < end ; i++)
    if (true == testTrigger((*_Trigger_nodes)[i], temp))
      return ((*_Trigger_nodes)[i]) ;
  return NULL ;
*/
}

inline
OB1NAnd*
OB1Graph::find_and_nodes(OB1NAnd* temp)
{
  if (_And_nodes.empty())
    return (OB1NAnd*) 0 ;

  std::vector<OB1NAnd*>::iterator it = _And_nodes.begin() ;
  for ( ; _And_nodes.end() != it ; it++)
    if (true == testAnd(*it, temp))
      return *it ;

  return (OB1NAnd*) 0 ;

/*
  register unsigned int end = _And_nodes->size() ;
  for (register unsigned int i = 0; i < end ; i++)
    if (true ==  testAnd( (*_And_nodes)[i], temp ))
      return ((*_And_nodes)[i]) ;
  return NULL ;
*/
}

inline
OB1NValidity*
OB1Graph::find_validity_nodes(OB1NValidity* temp)
{
  if (_Validity_nodes.empty())
    return (OB1NValidity*) 0 ;

  std::vector<OB1NValidity*>::iterator it = _Validity_nodes.begin() ;
  for ( ; _Validity_nodes.end() != it ; it++)
    if (true == testValidity(*it, temp))
      return *it ;

  return (OB1NValidity*) 0 ;

/*
  register unsigned int end = _Validity_nodes->size() ;
  for (register unsigned int i = 0; i < end ; i++)
           if (true == testValidity((*_Validity_nodes)[i], temp))
      return ( (*_Validity_nodes)[i]) ;
  return NULL;
*/
}

inline
OB1NResult*
OB1Graph::find_result_nodes(OB1NResult* temp)
{
  if (_Validity_nodes.empty())
    return (OB1NResult*) 0 ;

  std::vector<OB1NResult*>::iterator it = _Result_Nodes.begin() ;
  for ( ; _Result_Nodes.end() != it ; it++)
    if (true == testResult(*it, temp))
      return *it ;

  return (OB1NResult*) 0 ;

/*
  register unsigned int end = _Result_Nodes->size() ;
  for (register unsigned int i = 0; i < end ; i++)
    if (true == testResult((*_Result_Nodes)[i],temp ))
      return ((*_Result_Nodes)[i]) ;
  return NULL ;
*/
}

/**
* \brief Return the list of KS
*/
/**
/**
* Return the list of all KS <BR>
* Renvoie la liste de tous les KSs
* @return KS' list
*/
inline
std::vector<OB1NKS*>* OB1Graph::getKSs()
{
	return &_Ks_nodes ;
}

inline
std::map<std::string, OB1NOther*>* OB1Graph::getOtherNodes()
{
	return &_Other_nodes ;
}

inline
std::vector<OB1NKS*>*
OB1Graph::getKSOfType(KSType typ)
{
	std::vector<OB1NKS*>* res = new std::vector<OB1NKS*>() ;

  if (_Ks_nodes.empty())
    return res ;

  std::vector<OB1NKS*>::iterator it = _Ks_nodes.begin() ;
  for ( ; _Ks_nodes.end() != it ; it++)
    if ((*it)->KsType() == typ)
      res->push_back(*it) ;

/*
	register unsigned int end = _Ks_nodes->size() ;
	for (register unsigned int i = 0 ; i < end ; i++)
		if ((*_Ks_nodes)[i]->KsType() == typ)
			res->push_back((*_Ks_nodes)[i]) ;
*/

	return (res) ;
}

inline
OB1NKS*
OB1Graph::getKSByName(string sName)
{
  if (_Ks_nodes.empty())
    return (OB1NKS*) 0 ;

  std::vector<OB1NKS*>::iterator it = _Ks_nodes.begin() ;
  for ( ; _Ks_nodes.end() != it ; it++)
    if ((*it)->getKSName() == sName)
		 	return (*it) ;

  return (OB1NKS*) 0 ;

/*
	register unsigned int end = _Ks_nodes->size() ;
	for (register unsigned int i = 0; i < end; i++)
		if ((*_Ks_nodes)[i]->getKSName() == sName)
		 	return (*_Ks_nodes)[i] ;

	return NULL ;
*/
}

/**
* \brief Return all node
*/
/**
* Return all node <BR>
* Renvoie une liste contenant tous les noeuds
* @return list of all node
*/
inline
PointerBBNodeVector& OB1Graph::getAllVertex()
{
	return _AllNode ;
}

#endif

