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

#include <iostream.h>

#ifndef OB1HEURISTIX_H_
# define OB1HEURISTIX_H_

#include <vector>
#include "ns_ob1\OB1Node.h"

class OB1Controler ;
class OB1Strategy ;

/**
* \brief Abstraction of heuristic
*/
/**
* Abstraction of heuristic <BR>
* Abstraction de methode de choix
*/
class Heuristix
{
  int           choice ;
  OB1Controler* _controler ; /**< Controlor \ brief controlor */

 public:
  /**
  * \brief Constructor
  */
  /**
  * Constructor
  * @param control Pointor to controler
  */
  Heuristix(OB1Controler* control) { _controler = control ; }

  /**
  * \brief get a pointer to controler
  */
  /**
  * Get a pointer to controler <BR>
  * Renvoie un pointeur vers le controleur
  */
  inline OB1Controler  *Controler() { return _controler ; }

  /**
  * \brief Choice one node to compute
  */
  /**
  *  Choice one node to compute  <BR>
  *  Choisi un noeud a executer
  */
  virtual OB1Node* NextNode(std::vector<OB1Node* >& temp, OB1Strategy* pRootStrat) = 0 ;
};

/**
* \brief Simple heuristic
*/
/**
* Simple heuritic  <BR>
* Heuristique simple utilse dans OB1 actuellement
*/
class DFSHeuristix  : public Heuristix
{
 public:

  /**
  * \brief Constructor
  */
  /**
  * Constructor
  * @param control Pointor to controler
  */
  DFSHeuristix(OB1Controler* control) ;

  /**
  * \brief Choice one node to compute
  */
  /**
  *  Choice one node to compute  <BR>
  *  Choisi un noeud a executer
  */
  OB1Node* NextNode(std::vector<OB1Node* >& temp, OB1Strategy* pRootStrat) ;

  void     sortPossibleNodes(std::vector<OB1Node* >& temp, OB1Strategy* pRootStrat) ;
  OB1Node* chooseBestNode(std::vector<OB1Node* >& temp, OB1Strategy* pRootStrat) ;
};

/**
* \brief  Heuristic Generator
*/
/**
* Heuristic Generator   <BR>
* Permet de crer des heuristiques
* \todo Add a parameter to generete other strategie
*/
class HeuristixGenerator
{
 protected:

	DFSHeuristix  _createStrategy ; /**< Heuristic use for a the path in the publication graph \brief Heuristic use for a the path in the publication graph */
	OB1Controler* _controler ;      /**< Pointeur to controler \brief Pointer to controler */

 public:

	/**
	* \brief constructor
	*/
	/**
	* constructor
	* @param control pointor to controlor
	*/
	HeuristixGenerator(OB1Controler* control) ;

	/**
	* \brief destructor
	*/
	/**
	*  destructor
	*/
	~HeuristixGenerator() ;

	/**
	* \brief get a pointer to controler
	*/
	/**
	* Get a pointer to controler <BR>
	* Renvoie un pointeur vers le controleur
	*/
	OB1Controler *Controler() ;

	/**
	* \brief Get an heuristic
	*/
	/**
	* Get an heuristic <BR>
	* Renvoie une heuristique pour la r�solution d'un probl�me
	* utilis� pour la cr�ation de strategie de OB1NOther::createStrategy
	*/
	Heuristix* getHeuristix() ;
};

#endif /* end of OB1Heuristix */

