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

#ifndef   MARKSET_H_
# define   MARKSET_H_

#include <vector>
#include <string>
#include "ns_ob1\Marker.h"

/**
* \brief Decorator set for node
*/
/**
* Decorator set for node <BR>
* Ensemble de noeud de decoration
*/
class MarkSet
{
  protected:

    ValeurArray _Marks ;

    static long _lObjectCount ;

  public:

    /**
     * \brief Constructor
     */
    /**
     * Constructor
     */
    MarkSet() ;
    MarkSet(const MarkSet& rv) ;

    /**
     * \brief Destructor
     */
    /**
     * Destructor
     */
    ~MarkSet() ;

    void reset() ;

    /**
     * \brief Say if a var exist in the set
     */
    /**
     * Say if a var exist in the set   <BR>
     * Indique si il y a un decorateur de ce nom dans l'ensemble
     * @return true if temp exist, false else
     */
    bool Exist(std::string& temp) ;

    /**
     * \brief Insert the key and the object if they don't exist
     */
    /**
     *  Insert the key and the object if they don't exist
     *  Insere une marque dans l'ensemble si elle n'existe pas deja
     *  @param key Identifiant of mark to insert
     *  @param temp OBject to insert
     */
    void insert(std::string& key, MarkerObject* temp) ;

    /**
     * \brief set parameter string as key and put the Marker'objecct parameter as value.
     */
    /**
     * \brief set parameter string as key and put the Marker'objecct parameter as value.  <BR>
     * Insere un nouveau marker dans l'objet si il  déja
     *  @param key Identifiant of mark to set
     *  @param temp OBject to set
     */
    void set(std::string& key, MarkerObject* temp) ;

    /**
     * \brief get the number of object in the markset
     */
    /**
     * get the number of object in the markset  <BR>
     * Renvoie le nombre de mark présent dans l'ensemble
     * @return number of object in the markset
     */
    const int DecorateurSize()const { return _Marks.size() ; }

    /**
     * \brief Test if a marker with name anf and value vla exist
     */
    /**
     * Test if a marker with name anf and value vla exist <BR>
     * Test si une chaine dont le nom est name et la valeur est vla exist
     * @param name key name
     * @param  vla value of the object
     * @return true if the mark exist and false else
     */
    bool hasStringAttributes(std::string name, std::string vla) ;

    /**
     * \brief Test if a marker with substring name anf and value vla exist
     */
    /**
     * Test if a marker with substring name anf and value vla exist <BR>
     * Test si un marker de type string contient une sous string ayant une value vla existe
     * \todothe test seem to have in error != == npos
     * @param name first substring to find in key
     * @param  vla value of the object
     * @return true if the mark exist and false else
     */
    bool hasSubStringAttributes(std::string name, std::string vla) ;

    /**
     * \brief get Value from index
     */
    /**
     * get Value from index <BR>
     * Renvoie la valeur d'une mark présent en fonction de son index
     * @param i index
     * @return wanted value
     */
    Valeur* operator[] (unsigned int i) const ;

    /**
     *  \brief get the value of the key key_to_find
     */
    /**
     * get the value of the key key_to_find
     * @param key_to_find value to find
     */
    MarkerObject* get(std::string& key_to_find) ;

    /**
     * \brief Remove a marker
     */
    /**
     * Remove a marker
     * @param key key to remove
     */
    void Remove(std::string& key) ;

    MarkSet& operator=(const MarkSet& src) ;

    static long getNbInstance()  { return _lObjectCount ; }
    static void initNbInstance() { _lObjectCount = 0 ; }
};

/**
* \brief overloading of  << operator for MarkSet
*/
std::ostream&	operator<< (std::ostream& ostr, const MarkSet& m);

#endif /* MARKSET_H_ */
