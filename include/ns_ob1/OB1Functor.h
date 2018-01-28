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
#ifndef __OB1__FUNCTOR__
#define  __OB1__FUNCTOR__

#include "ns_ob1/abstractKS.h"

/**
*  Le but de ce fichier est de fournir au controler du blackboard
*  de pouvoir se connecter à des fonctions membre d'une classe
*  aussi bien qu'une fonction independante.
*  Pour ca, nous utilisons la notion de functor  <BR>
*  Un Functor est un concept de template de classe pour fonction
*/


/**
*\brief Functor used to link a representation of a KS to a publication
*/
/**
*	Functor used to link a representation of a KS to a publication<BR>
* Le functor de publication sert à se relier aux  méthodes de publication
*/
class FunctorPublication
{
	public:
  	/**
    *  Execute the function operation <BR>
    *  Execute le code de publication en appelant une méthode de classe
    *  ou d'une fonction
    *  @return The structure used to declare the KS in the controler
    */
    virtual KsInitStructure* Call() = 0 ;

    /**
    *  Execution operator BR>
    *  Execute le code de publication en appelant une fonction membre de classe
    *  @return The structure used to declare the KS in the controler
    */
    virtual KsInitStructure* operator()() = 0 ;
};


/**
* \brief Link the representation of the ks to a class method  publication
*/
/**
* Link the representation of the ks to a class method  publication <BR>
* Sert a se connecter à une fonction membre d'un classe pour la publication
*/
template <class TypeClass>
class MemFunctorPublication  : public FunctorPublication
{
  protected:

    TypeClass *_pt;   	/**< Pointer to an instance of a class of type TypeClass */
    typedef KsInitStructure* (TypeClass::*Thunk)(void);
    Thunk _thunk;      	/**< Pointer on a class méthod */

  public:

	  /**
    * constructor
    * @param pt Instance of the class
    * @param thunk Pointeur to a member function
    */
    MemFunctorPublication(TypeClass* pt, Thunk thunk) : _pt(pt)
    {
      _thunk = thunk ;
    }

    /**
    * Execute publication
    * Declenche l'execution de la publication
    * @return publication' structure
    */
 	  virtual	KsInitStructure* operator()()
    {
      return (_pt->*_thunk)() ;
    }

    /**
    * Execute publication
    * Declenche l'execution de la publication
    * @return publication' structure
    */
    virtual KsInitStructure* Call()
    {
      return (_pt->*_thunk)() ;
    }
};


/**
* \brief  Used to connect to publication function
*/
/**
* Used to connect to publication functionn <BR>
* Sert à se connecter à une fonction simple pour la connection : Sert à conserver
* la compatibilité avec les DLLS DPIO et rVDB
*/
class FuncFunctorPublication  : public FunctorPublication
{
  protected:

    typedef KsInitStructure* (*Thunk)(void); /* Definition of the function's pointer */
    Thunk _thunk;     /**< Pointer on a class méthod */

  public:

	  /**
    * Constructor
    * @param Pointer on the function's to execute
    */
    FuncFunctorPublication(Thunk thunk)
    {
      _thunk = thunk ;
    }

    /**
    * Execute publication
    * Declenche l'execution de la publication
    * @return publication' structure
    */
 	  virtual	KsInitStructure* operator()()
    {
      return (*_thunk)() ;
    }

    /**
    * Execute publication
    * Declenche l'execution de la publication
    * @return publication' structure
    */
    virtual KsInitStructure* Call()
    {
      return (*_thunk)() ;
    }
};


/*******************************************************************************
**************** Functor de d'action    ****************************************
*******************************************************************************/

/**
* \brief  Functor for KS's Action
*/
/**
* Abstraction used to link a representation of a KS to a function wich execute the action to a script  <BR>
* Le functor d'action sert à se relier aux methodes d'action definit dans une classe ou une méthode
*/
class FunctorAction
{
	public:

    virtual Errcode Call(BB1BB& bb, Transfert* trans, bool isOpportuniste) = 0 ;
    virtual Errcode operator() (BB1BB& bb, Transfert* trans, bool isOpportuniste) = 0 ;
};


/**
* \brief  Link the representation of the ks to a class method action
*/
/**
* Link the representation of the ks to a class method  action <br>
* Set a se connecter à une fonction membre d'un classe pour l'action
*/
template <class TypeClass>
class MemFunctorAction : public FunctorAction
{
  protected:

	  TypeClass *_pt ;
	  typedef Errcode (TypeClass::*Thunk)(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;
	  Thunk _thunk ;

  public:

    MemFunctorAction(TypeClass* pt, Thunk thunk) : _pt(pt)
    {
      _thunk = thunk ;
    }

 	  virtual	Errcode operator()(BB1BB& bb, Transfert* trans, bool isOpportuniste)
    {
      return (_pt->*_thunk)(bb, trans, isOpportuniste) ;
    }

    virtual Errcode Call(BB1BB& bb, Transfert* trans, bool isOpportuniste)
    {
      return (_pt->*_thunk)(bb, trans, isOpportuniste) ;
    }
};

/**
* \brief Link the representation of the ks to a action function
*/
/**
* Link the representation of the ks to a action function <BR>
* Sert à se connecter à une fonction simple pour la connection : Sert à conserver la compatibilité avec les DLLS DPIO et rVDB
*/
class FuncFunctorAction  : public FunctorAction
{
  protected:

	  typedef Errcode (*Thunk)(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;
	  Thunk   _thunk ;

  public:

    FuncFunctorAction( Thunk thunk)
    {
      _thunk = thunk ;
    }

	  virtual	Errcode operator()(BB1BB& bb, Transfert* trans, bool isOpportuniste)
    {
		  return (*_thunk)(bb, trans, isOpportuniste) ;
    }

    virtual Errcode Call(BB1BB& bb, Transfert* trans, bool isOpportuniste)
    {
      return (*_thunk)(bb, trans, isOpportuniste) ;
    }
};

/*******************************************************************************
**************** Functor de validité de contexte  ******************************
*******************************************************************************/

/**
* \brief  Functor for KS's Validity method
*/
/**
* Le functor de publication sert à se relier aux methodes de validity
*/
class FunctorValidity
{
	public:

    virtual ValidityContextType Call(const BB1BB& bb, Transfert* trans) = 0 ;
    virtual ValidityContextType operator() (const BB1BB& bb, Transfert* trans) = 0 ;
};

/**
* \brief   Link the representation of the ks to a class validity  method
*/
/**
* Link the representation of the ks to a class validity  method <BR>
* Set a se connecter à une fonction membre d'un classe pour validity
*/
template <class TypeClass>
class MemFunctorValidity : public FunctorValidity
{
  protected:

	  TypeClass *_pt ;
	  typedef   ValidityContextType (TypeClass::*Thunk)(const BB1BB& bb, Transfert* trans) ;
	  Thunk     _thunk ;

  public:

    MemFunctorValidity(TypeClass* pt, Thunk thunk) : _pt(pt)
    {
      _thunk = thunk ;
    }

 	  virtual ValidityContextType operator()(const BB1BB& bb, Transfert* trans)
    {
      return (_pt->*_thunk)(bb, trans );
    }

    virtual ValidityContextType Call(const BB1BB& bb, Transfert* trans)
    {
      return (_pt->*_thunk)(bb, trans );
    }
};

/**
* \brief Link the representation of the ks to a  validity  function
*/
/**
* Link the representation of the ks to a  validity  function  <BR>
* Sert à se connecter à une fonction simple pour la connection : Sert à conserver la compatibilité avec les DLLS DPIO et rVDB
*/
class FuncFunctorValidity : public FunctorValidity
{
  protected:

	  typedef ValidityContextType (*Thunk)(const BB1BB& bb, Transfert* trans) ;
	  Thunk   _thunk ;

  public:

    FuncFunctorValidity(Thunk thunk)
    {
      _thunk = thunk ;
    }

 	  virtual	ValidityContextType operator()(const BB1BB& bb, Transfert* trans)
    {
      return (*_thunk)(bb, trans) ;
    }

    virtual ValidityContextType Call(const BB1BB& bb, Transfert* trans)
    {
      return (*_thunk)(bb, trans) ;
    }
};

/*******************************************************************************
**************** Functor de Trigger de contexte  *******************************
*******************************************************************************/
/**
* \brief  Functor for KS's Trigger method
*/
/**
*  Functor for KS's Trigger method <BR>
* Le functor de Trigger sert à se relier aux methodes de Trigger
*/
class FunctorTrigger
{
	public:

    virtual bool Call(const BB1Event& event) = 0 ;
    virtual bool operator() (const BB1Event& event) = 0 ;
};


/**
* \brief   Link the representation of the ks to a class trigger  method
*/
/**
* Link the representation of the ks to a class trigger  method <BR>
* Set a se connecter à une fonction membre d'un classe pour executer les trigger
* conditions
*/
template <class TypeClass>
class MemFunctorTrigger : public  FunctorTrigger
{
  protected:

    TypeClass *_pt ;
    typedef bool (TypeClass::*Thunk)(const BB1Event& event) ;
    Thunk _thunk ;

  public:

    MemFunctorTrigger(TypeClass* pt, Thunk thunk) : _pt(pt)
    {
      _thunk = thunk ;
    }

    virtual	bool operator()(const BB1Event& event)
    {
      return (_pt->*_thunk)(event) ;
    }

    virtual bool Call(const BB1Event& event)
    {
      return (_pt->*_thunk)(event) ;
    }
};


/**
* \brief Link the representation of the ks to a  trigger  function
*/
/**
* Link the representation of the ks to a  trigger  function  <BR>
* Sert à se connecter à une fonction simple pour la connection : Sert à conserver la compatibilité avec les DLLS DPIO et rVDB
*/
class FuncFunctorTrigger : public  FunctorTrigger
{
  protected:

    typedef bool (*Thunk)(const BB1Event& event) ;
    Thunk _thunk ;

  public:

    FuncFunctorTrigger( Thunk thunk)
    {
      _thunk = thunk ;
    }

    virtual bool operator()(const BB1Event& event)
    {
      return (*_thunk)(event) ;
    }

    virtual bool Call(const BB1Event& event)
    {
      return (*_thunk)(event) ;
    }
};


/*******************************************************************************
**************** Functor de Precondition de contexte  **************************
*******************************************************************************/
/**
* \brief  Functor for KS's Precondition
*/
/**
* Functor for KS's Precondition
* Le functor de Precondition sert à se relier aux methodes de Precondition
*/
class FunctorPrecondition
{
	public:

    virtual bool Call(const BB1BB& bb, Transfert* trans) = 0 ;
    virtual bool operator() (const BB1BB& bb, Transfert* trans) = 0 ;
};

/**
* \brief   Link the representation of the ks to a class preconditon  method
*/
/**
* Link the representation of the ks to a class precondition  method <BR>
* Set a se connecter à une fonction membre d'un classe pour executer les preconditon
*/
template <class TypeClass>
class MemFunctorPrecondition : public FunctorPrecondition
{
  protected:

    TypeClass *_pt ;
    typedef bool (TypeClass::*Thunk)(const BB1BB& bb, Transfert* trans) ;
    Thunk _thunk ;

  public:

    MemFunctorPrecondition(TypeClass* pt, Thunk thunk) : _pt(pt)
    {
      _thunk = thunk ;
    }

    virtual	bool operator()(const BB1BB& bb, Transfert* trans)
    {
      return (_pt->*_thunk)(bb, trans) ;
    }

    virtual bool Call(const BB1BB& bb, Transfert* trans)
    {
      return (_pt->*_thunk)(bb, trans) ;
    }
};

/**
* \brief Link the representation of the ks to a  precondition  function
*/
/**
* Link the representation of the ks to a  precondition  function  <BR>
* Sert à se connecter à une fonction simple pour la connection : Sert à conserver la compatibilité avec les DLLS DPIO et rVDB
*/
class FuncFunctorPrecondition : public  FunctorPrecondition
{
  protected:

    typedef bool (*Thunk)(const BB1BB& bb, Transfert* trans) ;
    Thunk _thunk ;

  public:

    FuncFunctorPrecondition( Thunk thunk)
    {
      _thunk = thunk ;
    }

    virtual	bool operator()(const BB1BB& bb, Transfert* trans)
    {
      return (*_thunk)(bb, trans) ;
    }

    virtual bool Call(const BB1BB& bb, Transfert* trans)
    {
      return (*_thunk)(bb, trans) ;
    }
};

/*******************************************************************************
************ Functor de Obviation condition de contexte  ***********************
*******************************************************************************/
/**
* \brief  Functor for KS's Obviated condition
*/
/**
* Functor for KS's Obviated condition
* Le functor de Obviatedcondition sert à se relier aux methodes d'Obviatedcondition
*/
class FunctorObviation
{
	public:

    virtual bool Call(const BB1BB& bb, BB1KSAR& ksar) = 0 ;
    virtual bool operator() (const BB1BB& bb, BB1KSAR& ksar) = 0 ;
};

/**
* \brief   Link the representation of the ks to a class Obviated condition  method
*/
/**
*  Link the representation of the ks to a class Obviated condition  method <BR>
* Set a se connecter à une fonction membre d'un classe pour executer les Obviated conditions
*/
template <class TypeClass>
class MemFunctorObviation : public  FunctorObviation
{
  protected:

    TypeClass *_pt ;
    typedef bool (TypeClass::*Thunk)(const BB1BB& bb, BB1KSAR& ksar) ;
    Thunk _thunk ;

  public:

    MemFunctorObviation(TypeClass* pt, Thunk thunk) : _pt(pt)
    {
      _thunk = thunk ;
    }

    virtual	bool operator()(const BB1BB& bb, BB1KSAR& ksar)
    {
      return (_pt->*_thunk)(bb, ksar) ;
    }

    virtual bool Call(const BB1BB& bb, BB1KSAR& ksar)
    {
      return (_pt->*_thunk)(bb, ksar) ;
    }
};

/**
* \brief Link the representation of the ks to a   Obviated condition  function
*/
/**
* Link the representation of the ks to a   Obviated condition  function  <BR>
* Sert à se connecter à une fonction simple pour la connection : Sert à conserver la compatibilité avec les DLLS DPIO et rVDB
*/
class FuncFunctorObviation : public  FunctorObviation
{
  protected:

    typedef bool (*Thunk)(const BB1BB& bb, BB1KSAR& ksar) ;
    Thunk _thunk ;

  public:

    FuncFunctorObviation( Thunk thunk)
    {
      _thunk = thunk ;
    }

    virtual bool operator()(const BB1BB& bb, BB1KSAR& ksar)
    {
      return (*_thunk)(bb, ksar) ;
    }

    virtual bool Call(const BB1BB& bb, BB1KSAR& ksar)
    {
      return (*_thunk)(bb, ksar) ;
    }
};

#endif

