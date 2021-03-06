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

#ifndef   MARKER_H_
# define   MARKER_H_

# ifndef _DANSBBKDLL
#  define _BBKEXPORT __import
# else
#  define _BBKEXPORT __export
# endif

#include <iostream>
#include <string>
#include <typeinfo>

// extern "C"
// {
#include <stdlib.h>
// }

#include "partage\ns_vector.h"

/**
* \brief Decorator's pattern for node
*/
/**
* Abstraction Class used to flag a node
* It's use to decorate the node of a graph
*/
class MarkerObject
{
  public:
    /**
    *	\brief Destructor
    */
    /**
    *	 Destructor
    */
    virtual ~MarkerObject() {}

    /**
    * \brief return  int value
    */
    /**
    *  return  int value if the node is a int marker <BR>
    *  renvoie la valeur du marker si le noeud est un marker entier
    *  @return Int value
    *  \exception String if the marker is not an int marker
    */
    int intValue()  const throw(std::string) ;

    /**
    * \brief return  string  value
    */
    /**
    *  return  string value if the node is a string marker <BR>
    *  renvoie la valeur du marker si le noeud est un marker de string
    *  @return string value
    *  \exception String if the marker is not an string marker
    */
    std::string stringValue() const throw (std::string) ;

    /**
    *	\brief print the MarkerObject switch is type
    */
    /**
    *  Print the MarkerObject switch is type <BR>
    *  Ecrit le Marker en fonction de son type
    *  @param ostr Printing' stream
    */
    virtual void  print(std::ostream& ostr) const{} ;

    /**
    *	\brief print the MarkerObject switch is type in html format
    */
    /**
    *  Print the MarkerObject switch is type in html format<BR>
    *  Ecrit le Marker en fonction de son type  sous format HTML
    *  @param input Printing' string
    */
    virtual void htmlPrint(std::string& input) = 0 ;
};

/**
* \brief  Node Integer decorator
*/
/**
*  Node Integer decorator <BR>
*  Decorateur de type entier
*/
class _BBKEXPORT MarkerInteger : public MarkerObject
{
  private:

    int _value ;  /**< Value of the decorator \brief  Value of the decorator */

  protected:

    static long   _lObjectCount ;

  public:
    /**
    * \brief Constructor
    */
    /**
    * Constructor
    * @param Value to put in the decorator
    */
    MarkerInteger(int v)
    {
      _value = v ;
      _lObjectCount++ ;
    }

    MarkerInteger(const MarkerInteger& rv)
    {
      _value = rv._value ;
      _lObjectCount++ ;
    }

    ~MarkerInteger()
    {
      _lObjectCount-- ;
    }

    MarkerInteger& operator=(const MarkerInteger& src)
    {
      if (&src == this)
		    return *this ;

      _value = src._value ;

      return *this ;
    }

    /**
    * \brief Return Decorator value
    */
    /**
    * Return Decorator value
    * Renvoie la valeur du d�corateur
    * @param Decorator value
    */
    int getValue() const
    {
      return _value ;
    }

    /**
    *	\brief print the MarkerObject switch is type
    */
    /**
    *  Print the MarkerObject switch is type <BR>
    *  Ecrit le Marker en fonction de son type
    *  @param ostr Printing' stream
    */
 	 	void  print(std::ostream& ostr) const ;

    /**
    *	\brief print the MarkerObject switch is type in html format
    */
    /**
    *  Print the MarkerObject switch is type in html format<BR>
    *  Ecrit le Marker en fonction de son type  sous format HTML
    *  @param input Printing' string
    */
  	void htmlPrint(std::string& input) ;

    static long getNbInstance()  { return _lObjectCount ; }
    static void initNbInstance() { _lObjectCount = 0 ; }
};

/**
* \brief  Node String decorator
*/
/**
*  Node string decorator <BR>
*  Decorateur de type string
*/
class _BBKEXPORT MarkerString : public MarkerObject
{
  private:

    std::string _value ; /**< Value of the decorator \brief  Value of the decorator */

  protected:

    static long   _lObjectCount ;

  public:

    /**
    * \brief Constructor
    */
    /**
    * Constructor
    * @param Value to put in the decorator
    */
    MarkerString(const std::string& val)
    {
      _value = val ;
      _lObjectCount++ ;
    }

    MarkerString(const MarkerString& rv)
    {
      _value = rv._value ;
      _lObjectCount++ ;
    }

    ~MarkerString()
    {
      _lObjectCount-- ;
    }

    MarkerString& operator=(const MarkerString& src)
    {
      if (&src == this)
		    return *this ;

      _value = src._value ;

      return *this ;
    }

    /**
    * \brief Return Decorator value
    */
    /**
    * Return Decorator value
    * Renvoie la valeur du d�corateur
    * @param Decorator string value
    */
    std::string getValue() const
    {
      return _value ;
    }

    /**
    *	\brief print the MarkerObject switch is type
    */
    /**
    *  Print the MarkerObject switch is type <BR>
    *  Ecrit le Marker en fonction de son type
    *  @param ostr Printing' stream
    */
    void  print(std::ostream& ostr) const;

    /**
    *	\brief print the MarkerObject switch is type in html format
    */
    /**
    *  Print the MarkerObject switch is type in html format<BR>
    *  Ecrit le Marker en fonction de son type  sous format HTML
    *  @param input Printing' string
    */
    void htmlPrint(std::string& input);

    static long getNbInstance()  { return _lObjectCount ; }
    static void initNbInstance() { _lObjectCount = 0 ; }
};

/**
* \brief
*/
/**
* The value class is a sort of  bucket
* It's contains a key to acceed and a value
* The key is a string, and the value is of the type of
* MarkerObject
*/
class _BBKEXPORT Valeur
{
  private:

    std::string  	_key ; 	/**< Key of the object \brief Key of the object */
    MarkerObject* _data ; /**< Marker \brief Marker */

  protected:

    static long   _lObjectCount ;

  public:

    /**
     * \brief constructor
     */
    /**
     * Constructor
     * @param keys Identifiant
     * @param val Marker
     */
    Valeur(std::string keys, MarkerObject* val) ;
    Valeur(const Valeur& rv) ;

    /**
     * \brief Destructor
     */
    /**
     * Destructor
     */
    ~Valeur() ;

    /**
     * \brief  get identifiant
     */
    /**
     * get identifiant
     * @return Identifiant
     */
    inline std::string& Key() { return _key ; }

    /**
     *  \brief get Marker
     */
    /**
     * get Marker
     * @return MArker
     */
    inline const MarkerObject* ConstMark() { return _data ; }

    /**
     * \brief get Marker
     */
    /**
     * get Marker
     * @return MArker
     */
    inline MarkerObject* Mark() { return _data ; }

    /**
     *  \brief put a marker
     */
    /**
     *  put a marker
     *  @param temp marker to put
     */
    void putValue(MarkerObject* temp, bool bDuplicate = false) ;
    void putIntValue(MarkerInteger* temp, bool bDuplicate) ;
    void putStringValue(MarkerString* temp, bool bDuplicate) ;

    /**
     * \brief  Print the Data in htlm format
     */
    /**
     * Print the Data in htlm format
     * @param input data to fill
     */
    void htmlPrint(std::string& input) ;

    Valeur& operator=(const Valeur& src) ;

    static long getNbInstance()  { return _lObjectCount ; }
    static void initNbInstance() { _lObjectCount = 0 ; }
};

typedef vector<Valeur*> ValeurVecteur ;
typedef ValeurVecteur::iterator       ValeurIter ;
typedef ValeurVecteur::const_iterator ValeurConstIter ;

class ValeurArray : public ValeurVecteur
{
	public :

		// Constructeurs
    ValeurArray() : ValeurVecteur() {}
    ValeurArray(const ValeurArray& rv) ;
    // Destructeur
    ~ValeurArray() ;

    void vider() ;

    ValeurArray& operator=(const ValeurArray& src) ;

    bool          Exist(std::string& sKey) ;
    void          Remove(std::string& sKey) ;
    bool          hasStringAttributes(std::string sName, std::string sVal) ;
    bool          hasSubStringAttributes(std::string sName, std::string sVal) ;
    MarkerObject* get(std::string& key_to_find) ;
};

//------------------------------------------------------------------------------
/* overloading of  << operator for MarkSet */
std::ostream&	operator<< (std::ostream& ostr, const MarkerObject& m);

#endif /* MARKER_H_ */

