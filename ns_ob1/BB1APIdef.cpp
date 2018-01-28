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

#ifndef FOR_WINDOWS

# include "BB1BB.h"
# include "BB1Class.h"    				// for BB1BB::DefineClass(...)
# ifndef __TCONTROLER__
#  include "BB1Focus.h"    				// for BB1BB::ProposeFocus(...)
# endif
# include "BB1KB.h"       				// for BB1BB::DefineKB(...)
# include "BB1KS.h"       				// for BB1BB::DefineKS(...)
# include "BB1Link.h"     				// for BB1BB::DefineRelation(...)
# ifndef __TCONTROLER__
#  include "BB1Strategy.h" 				// for BB1BB::ProposeStrategy(...)
# endif
# include "BB1Exception.h"    		// add fab

#else

# include "ns_ob1\BB1Object.h"
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\BB1Class.h"    	// for BB1BB::DefineClass(...)
# ifndef __TCONTROLER__
#  include "ns_ob1\BB1Focus.h"    	// for BB1BB::ProposeFocus(...)
# endif
# include "ns_ob1\BB1KB.h"       	// for BB1BB::DefineKB(...)
# include "ns_ob1\BB1KS.h"       	// for BB1BB::DefineKS(...)
# include "ns_ob1\BB1Link.h"     	// for BB1BB::DefineRelation(...)
# ifndef __TCONTROLER__
#  include "ns_ob1\BB1Strategy.h" 	// for BB1BB::ProposeStrategy(...)
# endif
# include "ns_ob1\BB1Exception.h" // add fab
# include "ns_ob1/OB1Controler.h"

#endif

// -----------------------------------------------------------------------------
// Define a BB1 class.  Parameter directSuperclasses is a list rather than a set
// because, if multiple inheritance is implemented, the order of direct
// superclass declarations may become significant.
// -----------------------------------------------------------------------------
BB1Class*
BB1BB::DefineClass(const string sName, AttValPair& dispatch, const StringList *pDirectSuperclasses,/* const */ AVPSet *pAllowedAttributes)
{
try
{
  CreatePhaseCheck("BB1BB::DefineClass(...)") ; // might throw

  BB1Class *c = (BB1Class *) 0 ;

  // There ought to be a classKB
  if (NULL == _classKB)
    ClassKBNotFound cknf(*this, "BB1BB::DefineClass(...)") ; // throw cknf;
  else
    if (_classKB->ObjectNamed(sName))
      ClassAlreadyExists cae(*this, "BB1BB::DefineClass(...)", sName) ; // throw cae;
    else
    {
      if (CheckAllowedAttributes(pAllowedAttributes))
      {
        ObjectSet dscObjects ;
        if (CheckDirectSuperclasses(pDirectSuperclasses, &dscObjects))
        {
          // The "Class" meta-object has no class; all others have class "Class"
          BB1Class *pMetaClass = (BB1Class *) 0 ;
          if (string("Class") != sName)
          {
            pMetaClass = ClassNamed("Class") ;
            if (NULL == pMetaClass)
              // throw cnf;
              ClassNotFound cnf(*this, "BB1BB::DefineClass(...)", "Class") ;
          }

          // Memory freed in BB1KB::~BB1KB().  Note: no event
          c = new BB1Class(sName, dispatch, *_classKB, pMetaClass, pAllowedAttributes) ;
          if (c)
          {
            if (false == dscObjects.empty())
              // Specially handle the IsA links to the superclasses
              for (ObjectCIter p = dscObjects.begin() ; dscObjects.end() != p ; p++)
                c->AddLink(string("IsA"), string("CanBeA"), *p) ;
          }
          else
            // throw cf;
            ConstructFailure cf(*this, "BB1BB::DefineClass(...)", "BB1Class") ;
				}
				else
        {} // CheckDirectSuperclasses does exception handling

        // Erase content in order that contained objects are not deleted
        if (false == dscObjects.empty())
          dscObjects.clear() ;
			}
			else
	  		{} // All attributes are permitted
		}

    _kenoby.addBB1Class(sName) ;

    return (c) ;
  } // end try
  catch (CreatePhaseError& cpe)
    { return ((BB1Class *) NULL) ; }
  catch (UnknownPhase& up)
    { } // constructor aborts
  catch (ClassAlreadyExists& cae)
    { return ((BB1Class *) NULL) ; }
  catch (ClassKBNotFound& cknf)
    { } // constructor aborts
  catch (ClassNotFound& cnf)
    { } // constructor aborts
  catch (ConstructFailure& df)
    { } // constructor aborts
  return ((BB1Class *) NULL) ;
}

// -----------------------------------------------------------------------------
//  Define a knowledge base.  What properties should it initially have other
//  than its name?
// -----------------------------------------------------------------------------

BB1KB*
BB1BB::DefineKB(const string& name)
{
  try
  {
    if (CreatePhaseCheck("BB1BB::DefineKB(...)"))
		{
			// might throw
      if (KBNamed(name))
				KBAlreadyExists kbae(*this, "BB1BB::DefineKB(...)", name) ; // throw kbae;
      else
      {
				// Create and initialize the knowledge base object.
				// Memory freed in BB1BB::ClearKBs(void)

				BB1KB *k = new BB1KB(name, *this, ThisCycle()) ;
				if (k)
					// Insert it into the blackboard's kb set
					_kbs.push_back(k) ;
				else
					// throw cf;
					ConstructFailure cf(*this, "BB1BB::DefineKB(...)", "BB1KB") ;

				// Return a pointer to the KB.  Or should it be a reference?
				return (k) ;
      }
    }
  } // end try
  catch (CreatePhaseError& cpe)
    { return ((BB1KB *) NULL) ; }
  catch (UnknownPhase& up)
    { } // constructor aborts
  catch (ConstructFailure& df)
    { } // constructor aborts
  catch (KBAlreadyExists& kbae)
    { return ((BB1KB *) NULL) ; }

  return ((BB1KB *) NULL) ;
}

BB1KS*
BB1BB::DefineGeneralizedKS(const std::string&         name,
                           BB1KB&                     kb,
                           const FunctorPublication*  tpub,
                           const FunctorValidity*     tVal,
                           const FunctorPrecondition* pCond,
                           const FunctorAction*       action,
                           const FunctorObviation*    oBViated,
                           AVPSet*                    attSpec  )
{
  try
  {
    BB1KS *k = KSptr(0) ;
    if (CreatePhaseCheck("BB1BB::DefineKS(...)"))
    {
      BB1Class *c = ClassNamed("KS") ;
      if (c)
      {
        k = new BB1KS(name, kb, c, tpub, tVal, pCond, action, oBViated, attSpec) ;
        if (k)
          _kenoby.addKS(k) ;
        else
          ConstructFailure cf(*this, "BB1BB::DefineKS(...)", "BB1KS") ;
        return (k) ;
      }
    }
  } // end try
  catch (CreatePhaseError& cpe)
    { return ((BB1KS *) NULL) ; }
  catch (UnknownPhase& up)
    { } // constructor aborts
  catch (ClassNotFound& cnf)
    { } // aborts
  catch (ProtectedKB& pkb)
    { return ((BB1KS *) NULL) ; }
  catch (KSAlreadyExists& ksae)
    { return ((BB1KS *) NULL) ; }
  catch (ConstructFailure& df)
    { }

  return ((BB1KS *) NULL) ;
}

Errcode
BB1BB::DefineRelation(const string& ForwardRelation,
                      const string& InverseRelation,
                      const string&	FromClassName,
                      const string&	ToClassName)
{
  BB1Class *FromClass = ClassNamed(FromClassName) ;
  BB1Class *ToClass   = ClassNamed(ToClassName) ;

  try
  {
    if (!CreatePhaseCheck("BB1BB::DefineRelation(...)"))
    {
      // might throw
    }
    else
			if (!FromClass)
				// throw cdne;
				ClassDoesNotExist cdne(*this, "BB1BB::DefineRelation(...)", FromClassName) ;
			else
				if (!ToClass)
					// throw cdne;
					ClassDoesNotExist cdne(*this, "BB1BB::DefineRelation(...)", ToClassName) ;
				else
					if (FromClass->HasLinkP(ForwardRelation, ToClass))
						// throw lae;
						LinkAlreadyExists lae(*this, "BB1BB::DefineRelation(...)", ForwardRelation) ;
					else
						if (ToClass->HasLinkP(InverseRelation, FromClass))
							// throw lae;
							LinkAlreadyExists lae(*this, "BB1BB::DefineRelation(...)", InverseRelation) ;
						else
							if (ReservedLinkP(ForwardRelation))
								// throw lnr;
								LinkNameReserved lnr(*this, "BB1BB::DefineRelation(...)", ForwardRelation) ;
							else
								if (ReservedLinkP(InverseRelation))
								{
									LinkNameReserved lnr(*this, "BB1BB::DefineRelation(...)", InverseRelation) ;
									return (FAILURE) ; // throw lnr;
								}
								else
								{
									// The linkPair allows retrieval of the inverse link
									// Memory freed in BB1BB::ClearLinkPairs(void)
									BB1LinkPair* lp = new BB1LinkPair(ForwardRelation, InverseRelation, FromClass->Name(), ToClass->Name()) ;
									if (lp)
									{
										_linkPairs.push_back(lp) ;
										// link the two classes
										FromClass->AddLink(ForwardRelation, InverseRelation, ToClass) ;
										return (SUCCESS) ;
									}
									else
										// throw cf;
										ConstructFailure cf(*this, "BB1BB::DefineRelation(...)", "BB1LinkPair") ;
								}
  } // end try
  catch (CreatePhaseError& cpe)
    { return (FAILURE) ; }
  catch (UnknownPhase& up)
    { } // constructor aborts
  catch (ClassDoesNotExist& cdne)
    { return (FAILURE) ; }
  catch (LinkAlreadyExists& lae)
    { return (FAILURE) ; }
  catch (LinkNameReserved& lnr)
    { return (FAILURE) ; }
  catch (ConstructFailure& cf)
    { } // constructor aborts
  
  return (FAILURE) ;
}







