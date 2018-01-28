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

#ifndef _BB1KS_H
# define _BB1KS_H 1

class BB1BB ;
class BB1KB ;
class BB1KSAR ;

// Déclaration des fonctor
class FunctorPublication;
class FunctorValidity;
class FunctorPrecondition;
class FunctorAction;
class FunctorObviation;

# ifdef __GNUC__
#  pragma interface
# endif

# include "BB1Object.h" // base type

/**
* \brief Represention of the Knowledge Source in the Blackboard
*/
/**
* Represention of the Knowledge Source in the Blackboard
* Représenation d'un KS sur le Blackboard
*/
class BB1KS  : public BB1Object
{
	friend class BB1BB ;
  friend class Controler;
	friend class KSSet ;
	friend class ObjectSet ;

 public:

  /**
  * \brief Constructor
  */
  /**
  * constructor
  * @param oname Name of the KS (Must be unique)
  * @param kb  KB were is stored the object
  * @param c Class of the object
  * @param tdef  Publication functor
  * @param tVal  Validity functor
  * @param ac  Action functor
  * @param oc  Obviation functor
  * @param attSpec Set of Attributes
  */
  BB1KS(const string&										oname,
        BB1KB&													kb,
        BB1Class												*c,
        const FunctorPublication* 			tdef,
        const FunctorValidity*         	tVal,
        const FunctorPrecondition*			pc,
        const FunctorAction*						ac,
        const FunctorObviation*				  oc,
        const AVPSet							*attSpec) ;

  /**
  * \brief Copy Constructor
  */
  /**
  * Copy constructor
  */
	BB1KS(const BB1KS& src) ;

  /**
  * \brief Destructor
  */
  /**
  * Destructor
  */
	~BB1KS() { } ;

	/**
	* Fonction retournant une condition d'abandon du KS
	*/
	inline
	const FunctorObviation*   ObviationCondition() const ;

	/**
	* Fonction retournant la fonction permttant de récupérer la déclaration
	*/
	inline
	const FunctorPublication*  Publication() const ;

	/**
	* Fonction déclenchant le functor declarant l'action
	*/
	inline
	const FunctorAction*       Action() const ;

	/**
	* Fonction retournant la validité du contexte
	*/
	inline
	const FunctorValidity*     ValidityContext() const ;

	/**
	* Fonction retournant les préconditions
	*/
	inline
	const FunctorPrecondition* PreCondition() const ;

  int   getPriority() { return _iPriority ; }

	void  Save(ofstream& KBstream) ;

private:

  const FunctorPublication*  _tPublication ;  		 /**< Publication Functor \brief Publication Functor */
  const FunctorValidity*     _tValidity ;      		 /**< Validity Functor \brief Validity Functor  */
  const FunctorPrecondition* _preCondition ;    	 /**< Precondition Functor \brief Precondition Functor  */
  const FunctorAction*			 _action ;             /**< Action Functor \brief  Action Functor*/
  const FunctorObviation*		 _obviationCondition ; /**< Obviation Functor \brief Obviation Functor */

  int                        _iPriority ;
} ;

/*
** Fonction retournant une condition d'abandon du KS
*/
inline
const FunctorObviation*	BB1KS::ObviationCondition() const { return (_obviationCondition) ; }

/*
** Fonction retournant la fonction permttant de récupérer la déclaration
*/
inline
const FunctorPublication*  BB1KS::Publication() const { return (_tPublication); }

/*
** Fonction déclenchant le functor declarant l'action
*/
inline
const FunctorAction*	 BB1KS::Action() const { return (_action) ; }

/*
** Fonction retournant la validité du contexte
*/
inline
const FunctorValidity*   BB1KS::ValidityContext() const { return (_tValidity); }

/*
** Fonction retournant les préconditions
*/
inline
const FunctorPrecondition*	 BB1KS::PreCondition() const { return (_preCondition) ; }

typedef BB1KS* KSptr ;

class AKS
{
 public:

	AKS(BB1KS *dpioKS, APConditionT dpioAPCond) ;
	AKS(const AKS& src) ;
	~AKS() ;

	AKS&					operator=(const AKS& src) ;

	BB1KS					*ks ;
	APConditionT	apCondition ;
} ;

#endif // _BB1KS_h

