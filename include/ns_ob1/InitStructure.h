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

#ifndef   KSINITSTRUCTURE_H_
# define   KSINITSTRUCTURE_H_

#ifndef _DANSBBKDLL
#define _BBKEXPORT __import
#else
#define _BBKEXPORT __export
#endif

#include <string>
#include <vector>
#include <iostream>
#include "ns_ob1\OB1types.h"

class TypedVal;

/**
* \brief Declaration node for controler
*/
/**
* Declaration node for controler <BR>
* Classe d 'intialisation
*/
class _BBKEXPORT INode
{
  public:
  TypedVal*   _labeled ;  	/**< Indicates the value for the control \brief Indicates the value for the control */
  std::string _classTo ;  	/**< the BB1Class of the event \brief  the BB1Class of the event */
  std::string _classFrom ;  /**< */

  /**
  * \brief  Constructor
  */
  /**
  * Constructor
  */
  INode(TypedVal* label, std::string clasto, std::string clasform) ;

  /**
  * copy Constructor
  */
  /**
  * Copy constructor
  * @param temp Inode to clone
  */
  INode(const INode& temp) ;

  INode& operator=(const INode& src) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
  ~INode() ;

  /**
  * \brief INode in html format
  */
  /**
  * write INode in html format <BR>
  * Ecrit le contenu sous forme HMTL
  * @return Inode data in HTML format
  */
  std::string toHTML() ;
};

/**
* \brief Describe the caracteristics of a level
*/
/**
* Class which describe the caracteristics of
* one level. A level is a class of response
* at one problem.  <BR>
* Un level est un ensemble de condition/informations.
*/
class _BBKEXPORT Level
{
  private:

    int _complexity ;             /**< Complexity indicator \brief  Complexity indicator */
    int _quality ;                /**< Quality indicator which can be compute whith this level \brief Quality indicator which can be compute whith this level */

    std::vector<INode*> _level ;  /**< Conditions vector */

  public:

    /**
    * \brief Constructor
    */
    /**
    * Constructor
    */
    Level() ;
    Level(int complex, int qualit) ;

    /**
    * \brief Copy constructor
    */
    /**
    * Copy constructor
    */
    Level(const Level& lev) ;

    Level& operator=(const Level& src) ;

    /**
    * \brief Destructor
    */
    /**
    * Destructor
    */
    ~Level() ;

    inline void putComplexty(int comp = 3) { _complexity = comp ; }
    inline int  Complexity() const         { return _complexity ; }

    inline void putQuality(int qualit)     { _quality = qualit ; }
    inline int  Quality()    const         { return _quality ; }

    inline std::vector<INode*>* GetData()  { return &_level ; }

    inline int getLevelsize() const        { return _level.size() ; }

    void   addLevelNode(TypedVal* tri, std::string& classto, std::string& classfrom) ;

    inline INode* operator[](size_t i)     { return _level.size() <= i ? (INode*) 0 : _level[i] ; }

    void  print(std::ostream& ostr) const ;
    std::string toHTML() ;
};

std::ostream&	operator<< (std::ostream& ostr, const Level& m)
{
  m.print(ostr) ;
  return (ostr) ;
}

/**
* \brief Structure initialise by KS
*/
/**
* Struture created by a KS  <BR>
* Structure de publication
*/
class _BBKEXPORT KsInitStructure
{
  private:

    Level*              _Required ;     /**< Required var \brief Required var */
    Level*              _ValidityVar ;  /**< var required for computing validity \brief var required for computing validity  */
    std::vector<Level*> _Levels ;       /**< variables which is not necessary for execution  The fisrt level is the required's level \brief variables which is not necessary for execution  The fisrt level is the required's level */
    std::vector<Level*> _Trigerred ;    /**< The triggered condiftion are the condition wich run an execution \brief The triggered condiftion are the condition wich run an execution  */
    std::vector<INode*> _Result ;       /**< return value by the ks \brief return value by the ks */
    std::string         _ksName ;       /**< Name of the KS \brief Name of the KS */
    KSType              _ksType ;       /**< Ks_Type \brief Ks_Type */

  public:
    /**
  	* \brief Constructor
  	*/
    /**
  	*  Constructor
  	*/
    KsInitStructure() ;  /* Default constructor Init all conditions for the controller for one KS */

    /**
  	* \brief Constructor
  	*/
    /**
    * Constructor
    * @param temp the name of the KS
    */
    KsInitStructure(char* temp) ;

    /**
    *  \brief Copy constructor
    */
    KsInitStructure(const KsInitStructure& ini) ;

    /**
    * \brief  Return the  KS name
    */
    inline std::string getKsName() { return _ksName ; }

    /**
    * \brief Put the ks's name
    */
    /**
    *  Put the ks's name <BR>
    *  Insère le nom du KS
    *  @param temp KS name
    */
    inline void putKsName(std::string& temp) { _ksName = temp ; }

    /**
    * \brief destructor
    */
    /**
    * Destructor
    */
    ~KsInitStructure() ;
    KsInitStructure& operator=(const KsInitStructure &temp) ;    /* Overloading of affectation operator */

    /**
    * \brief put Ks Type
    */
    /**
    *  put Ks Type <BR>
    *  Met le type de KS (service, IHM, user)
    * @pram typ KS type
    */
    void putKsType(KSType typ) { _ksType = typ ; }

    /**
    * \brief Return the KS type
    */
    /**
    * Return the ks Type  <BR>
    * Renvoie le type du KS
    * @return KS type
    */
    KSType KsType() { return _ksType ; }

    /**
    * \brief  Return the Trigerred which is necessary
    */
    inline Level* Required() const { return _Required ; }

    /**
    * \brief  Return the var for computing
    */
    inline Level* Validity() const { return _ValidityVar ; }

    /**
    * \brief Return the trigerred result conditions
    */
    inline std::vector<INode*>* getResult() { return &_Result ; }

    /**
    * \brief Add a new level, Memory zone allocated for the Level wich be deleted, By KsInitStructure
    */
    inline void addLevel(Level* temp) { _Levels.push_back(temp) ; }

    /**
    * \brief Get the var for improving the quality of the result
    */
    inline std::vector<Level*>* getLevel() { return &_Levels ; }

    /**
    * \brief  Get the triggered var
    */
    inline  std::vector<Level*>* Trigger() { return &_Trigerred ; }

    /**
    * \brief A result is the type of returns of KS
    */
    inline void addResult(TypedVal* temp,std::string& clasTo,std:: string& clasFrom)
    {
      INode* ad = new INode(temp, clasTo, clasFrom) ;
      _Result.push_back(ad) ;
    }

    inline void addRequired(Level* temp) { _Required = temp ; }
    inline void addValidity(Level* temp) { _ValidityVar = temp ; }

    /**
    * \brief Add a trigerred list, each list is type of AND condition
    */
    inline void addTriggeredList(Level* triger) { _Trigerred.push_back(triger) ; }

    /**
    * \brief  Get the number of triggered  list conditions
    */
    inline int getNumberOfTrigerred() const { return _Trigerred.size() ; }

    /**
    * \brief  Return the i trigerred
    */
    inline Level* getTrigerredAtRank(size_t i) ;

    inline Level*	operator[](size_t i) ;

    int getExecutionNumber() const ;

    inline int getNumberOfResult() { return _Result.size() ; }
    /**
    * \todo Test if a Trigerred is present in multiple Triggerred list
    * Ameliorer l'algorithme apres fonctionnement
    */
    bool checkCourtCircuit() ;

    std::string toHTML() ;
};

#endif
