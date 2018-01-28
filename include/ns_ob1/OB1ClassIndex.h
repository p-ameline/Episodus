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

#ifndef   BB1CLASINDEX_H_
# define   BB1CLASINDEX_H_

#include <string>
#include <map>

/**
* \brief Class used to optimize the test of the class
*/
/**
* Class used to optimize the test of the class
* Annuaire des classes.
* \todo Retirer Comlpique les choses et n'apporte pas reelement d'amélioration des performances
*/
class BB1ClassIndex
{
  std::map<std::string, int> _index ;  /**< Map beetween name and id \brief Map beetween name and id */
  int _max ;  												/**< Map beetween name and id \brief Map beetween name and id */

  public:
  /**
  * \brief Constructor
  */
  /**
  *Constructor
  */
  BB1ClassIndex() ;

  /**
  * \brief Destructor
  */
  /**
  *  Destructor
  */
  ~BB1ClassIndex() ;

  /**
  * \brief new class item to insert
  */
  /**
  * new class item to insert
  */
  void insertItem(std::string& key) ;

  /**
  * \brief return the id in function of the name
  */
  /**
  * return the id in function of the name <BR>
  * Renvoie l'identifiant entier en fonction du nom de la classe
  * @param clas Class name
  * @return id of the class
  */
  int operator[] (std::string& clas) const;

  /**
  * \brief summarize index in a string
  */
  /**
  * summarize index in a string
  * Résume le dico dans une string
  * @resume summaray
  */
  std::string toHTML() ;
};

#endif /* Finde BB1ClassIndex.h */

