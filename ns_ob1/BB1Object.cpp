/*
** Copyright Nautilus, (10/9/2004)
** fabrice.le_perru@nautilus-info.com
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

#ifdef __GNUC__
#pragma implementation
#endif

#include <assert.h>                 // for BB1Object::ProcessLinkDel(...)
#include <iostream.h>               // for operator<<(...)

#ifndef FOR_WINDOWS

#include "BB1Object.h"
#include "BB1Class.h"
#include "BB1KB.h"
#include "BB1Link.h"         // for BB1Object::AddLink(...)
#include "BB1Exception.h"    // add fab
#include "AttValPair.h"
#include "BB1types.h"
#ifndef __TCONTROLER__
#include "BB1Event.h"
#include "BB1Focus.h"
#endif
#include "BB1KS.h"
#ifndef __TCONTROLER__
#include "BB1KSAR.h"
#endif
#include "BB1Message.h"
#ifndef __TCONTROLER__
#include "BB1Strategy.h"
#endif

#else

#include "ns_ob1\BB1Object.h"
#include "ns_ob1\BB1Class.h"
#include "ns_ob1\BB1KB.h"
#include "ns_ob1\BB1Link.h"         // for BB1Object::AddLink(...)
#include "ns_ob1\BB1Exception.h"    // add fab
#include "ns_ob1\AttValPair.h"
#include "ns_ob1\BB1types.h"
#include "ns_ob1\BB1Event.h"
#include "ns_ob1\BB1KS.h"
#include "ns_ob1\BB1Message.h"
#include "ns_ob1\OB1Controler.h"
#include "ns_ob1\OB1Token.h"

#endif

#ifdef _ENTERPRISE_DLL
  #include "ob1Lus/interfaceLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
#else
  #include "ns_ob1\interface.h"
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
  #include "nautilus\nautilus.rh"
#endif

long BB1Object::_lBB1ObjectCount = 0 ;

// Return the pathpatho of an Answer object
//
NSPatPathoArray* getPatPatho2(BB1Object* temp, const std::string& pathpathoname)
{
	if (NULL == temp)
  	return NULLPATHO ;
try
{
	// Old code
	// return (temp->Value(pathpathoname, NULLPATHO)) ;

  BB1Object* pAnswer = (BB1Object*) 0 ;

	// Is temp an answer?

  string sKbName = temp->KB().Name() ;
  if (sKbName == string("InformationKB"))
  	pAnswer = temp ;
  else
		pAnswer = temp->BB().searchAnswerForQuestion(temp) ;

	if (pAnswer)
		return pAnswer->Value(pathpathoname, NULLPATHO) ;

	return NULLPATHO ;
}
catch (...)
{
	return NULLPATHO ;
}
}

// -----------------------------------------------------------------------------
// Create a BB1 object with the given name in the given kb with the given
// attributes and links. It is assumed name, kb, attributes, and links have
// already been checked for validity. Creation of the event is handled elsewhere.
// -----------------------------------------------------------------------------

BB1Object::BB1Object(const string sOName, AttValPair dispatch, BB1KB& kb, BB1Class *c)
          :_name(sOName),
           _knowledgeBase(kb),
           _cycleCreated(kb.ThisCycle())
{
  _lBB1ObjectCount++ ;

  _attributes.push_back(new AttValPair(dispatch)) ;

  // Insert the object into its own knowledge base
  // The knowledge base is in charge of deleting it when closing
  _knowledgeBase.InsertObject(this) ;

	// Specially handle the Exemplifies link to the class
  if (c)
		Instantiate(c) ;

	DebugObjectCPT++ ;

  if (BB().pContexte->getBBinterface())
    BB().pContexte->getBBinterface()->RefreshObjectsList() ;
}

BB1Object::BB1Object(const BB1Object& src)
          :_name(src._name),
           _knowledgeBase(src._knowledgeBase),
           _cycleCreated(src._cycleCreated),
           _attributes(src._attributes),
           _links(src._links)
{
  _lBB1ObjectCount++ ;

	_strategyP = src._strategyP ;

	DebugObjectCPT++ ;

  if (BB().pContexte->getBBinterface())
    BB().pContexte->getBBinterface()->RefreshObjectsList() ;
}

BB1Object&
BB1Object::operator=(BB1Object& src)
{
	if (this == &src)
		return *this ;

/*
	name					= src.name ;
	knowledgeBase	= src.knowledgeBase ;
	cycleCreated	= src.CycleCreated() ;
*/

  _attributes = src._attributes ;
	_strategyP	= src._strategyP ;
  _links      = src._links ;

  return (*this) ;
}

// -----------------------------------------------------------------------------
// Deallocate the object's attributes and its links
// -----------------------------------------------------------------------------
BB1Object::~BB1Object()
{
  //	DeleteAllAttributes() ;
  DeleteAllLinks() ;

  if (BB().pContexte && BB().pContexte->getBBinterface())
  {
    string sActionLog = string("Entering ~BB1Object for object ") + _name + string(" (") + getQuestionPath() + string(")") ;
    BB().pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
  }

  // Erase the object pointer for every token attached to this object
  //
  OB1Controler* pControler = BB().Controler() ;
  if (pControler)
  {
    NTArray<OB1Token>* pTokenArray = pControler->getTokenArray() ;
    if ((pTokenArray) && (false == pTokenArray->empty()))
    {
	    std::vector<OB1Token*>::iterator tokIter = pTokenArray->begin() ;
	    for ( ; pTokenArray->end() != tokIter ; tokIter++)
	      if ((*tokIter)->getObject() == this)
        {
          if (BB().pContexte && BB().pContexte->getBBinterface())
          {
            string sActionLog = string("Reseting object pointer for token ") + IntToString((*tokIter)->getNumero()) ;
            BB().pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;
          }
          (*tokIter)->putObject((BB1Object *) 0) ;
        }
    }
  }

  // Make certain this object is removed from its KB's ObjectSet
  //
  _knowledgeBase.EraseObjectNamed(_name) ;

	DebugObjectCPT-- ;

  if (BB().pContexte->getBBinterface())
    BB().pContexte->getBBinterface()->RefreshObjectsList() ;

  _lBB1ObjectCount-- ;

/*
  char buf[255] = {0} ;
  sprintf(buf, "%d", _lBB1ObjectCount) ;
  string sLog = string("Remains ") + string(buf) + string(" BB1Object(s)") ;
  BB().pContexte->getBBinterface()->addActionString(sLog, BB1BBInterface::trSubDetails) ;
*/
}

BB1BB&
BB1Object::BB()
{
  return _knowledgeBase.BB() ;
}

// -----------------------------------------------------------------------------
// Send to the output stream a textual representation of the object
// -----------------------------------------------------------------------------

std::ostream& operator <<(std::ostream& os, const BB1Object& obj)
{
  os << obj.KB().Name() << ":" << obj.Name() << "[" << obj.CycleCreated() << "] {\n" ;
  os << "Attributes(\n" ;

  if (false == obj._attributes.empty())
    for (AVPCIter a = obj._attributes.begin() ; obj._attributes.end() != a ; a++)
      os << *a ;

  os << ")\n";
  os << "Links(\n";

  if (false == obj._links.empty())
    for (LinkCIter l = obj._links.begin() ; obj._links.end() != l ; l++)
      os << *l ;

  os << ")\n" ;
	os << "}\n" ;

  return (os) ;
}

// -----------------------------------------------------------------------------
// Add a link and its backward link.  All validity testing is assumed to have
// been done.
// -----------------------------------------------------------------------------

Errcode
BB1Object::AddLink(const string     sForwardLinkName,
                   const string     sInverseLinkName,
                         BB1Object* pToObject)
{
  try
  {
    int ReflexiveP = (this == pToObject) ;
    Cycle cycle = KB().ThisCycle() ;

    // add the forward link
    // Memory freed in BB1Object::DeleteAllLinks() and BB1Object::ProcessLinks()
    BB1Link *flp = new BB1Link(sForwardLinkName, pToObject, cycle, ReflexiveP) ;
    if (flp)
    {
      _links.push_back(flp) ;

      // add backward link, even if forwardLinkName == newOppLinkName or this = toObject
      // Memory freed in BB1Object::DeleteAllLinks() and BB1Object::ProcessLinks()
      BB1Link *ilp = new BB1Link(sInverseLinkName, this, cycle, ReflexiveP) ;
      if (ilp)
      {
				pToObject->_links.push_back(ilp) ;
				return (SUCCESS) ;
      }
      else
			{
				// throw cf;
				ConstructFailure cf(BB(), "BB1Object::AddLink(...)", "BB1Link") ;
				return (FAILURE) ;
			}
		}
		else
		{
			// throw cf;
			ConstructFailure cf(BB(), "BB1Object::AddLink(...)", "BB1Link") ;
			return (FAILURE) ;
    }
  } // end try
  catch (ConstructFailure& cf)
    { } // constructor aborts

  return (FAILURE) ;
}

// -----------------------------------------------------------------------------
// Return a reference to the object's blackboard. Not defined in header because
// of call to BB1KB::BB(void)
// -----------------------------------------------------------------------------
// moving in BB1Object.h
/*
BB1BB& BB1Object::BB() const
{

#ifdef _DEBUG_
	cout << "-- DEBUG:: BB1Object::BB() const\n\n" ;
#endif

	return (KB().BB()) ;
};
*/

// -----------------------------------------------------------------------------
// Make the object attribute-free
// -----------------------------------------------------------------------------
Errcode
BB1Object::DeleteAllAttributes()
{
  ClearSetOfAVP(&_attributes) ;
  return (SUCCESS) ;
}

// -----------------------------------------------------------------------------
// Make the object link-free. Be careful to take care of the inverse links too.
// -----------------------------------------------------------------------------
Errcode
BB1Object::DeleteAllLinks()
{
try
{
  // The only object without a class
  // Delete its links by way of its class instances
  //
  if (_links.empty() || (LongName() == string("Class.Class")))
    return SUCCESS ;

  const BB1Class * const pC = Class() ;
  if (NULL == pC)
  {
    ObjectHasNoClass ohnc(BB(), "BB1Object::DeleteAllLinks()", Name()) ;
    return FAILURE ; // throw ohnc;
  }

  // First, delete all links that are not "Exemplifies" nor reflexive
  //
  for (LinkIter fl = _links.begin() ; _links.end() != fl ; )
  {
    if (((*fl)->Name() != string("Exemplifies")) && (false == (*fl)->ReflexiveP()))
    {
      DeleteBackwardLink(*fl, pC) ;

      delete (*fl) ;
      _links.erase(fl) ;
    }
    else
      fl++ ;
  }

  if (_links.empty())
    return SUCCESS ;

  // Then delete "Exemplifies" link
  //
  for (LinkIter fl = _links.begin() ; _links.end() != fl ; )
  {
    if ((*fl)->Name() == string("Exemplifies"))
    {
      DeleteBackwardLink(*fl, pC) ;

      delete (*fl) ;
      _links.erase(fl) ;
    }
    else
      fl++ ;
  }

  if (_links.empty())
    return SUCCESS ;

  // Then delete the rest
  //
  for (LinkIter fl = _links.begin() ; _links.end() != fl ; )
  {
    delete (*fl) ;
    _links.erase(fl) ;
  }

  return SUCCESS ;
}
catch (ObjectHasNoClass& ohnc)
{
  return FAILURE ;
}
}

// -----------------------------------------------------------------------------
// Delete the link pointed to by flptr and its backward link. c is the from
// object's class object
// -----------------------------------------------------------------------------
void
BB1Object::DeleteBackwardLink(BB1Link *flptr, const BB1Class* pC)
{
  if (NULL == flptr)
    return ;

try
{
  BB1Object *pToObject = flptr->ToObject() ;
  if (NULL == pToObject)
    return ;

  // If the link is reflexive, it will not be taken care of here
  //
  if ((this == pToObject) || pToObject->_links.empty())
    return ;

  const string sLinkName = flptr->OppositeLinkName(pC) ;
  if (string("") == sLinkName)
  {
    // throw blnnf ;
    BackwardLinkNameNotFound blnnf(BB(), "BB1Object::DeleteBackwardLink(...)", flptr->Name()) ;
    return ;
  }

  BB1Link *blptr = pToObject->GetLocalLink(sLinkName, this) ;
  // If there is a forward link, there must be a backward link
  if (NULL == blptr)
  {
    // throw lnf ;
    LinkNotFound lnf(BB(), "BB1Object::DeleteBackwardLink(...)", sLinkName) ;
    return ;
  }

  // Delete the backward link
  //
  for (LinkIter l = pToObject->_links.begin() ; pToObject->_links.end() != l ; )
  {
    if (*l == blptr)
    {
      if (*l)
        delete (*l) ;
      pToObject->_links.erase(l) ;
    }
    else
      l++ ;
  }
}
catch (LinkNotFound& lnf)
{ } // bad, but probably not fatal
catch (BackwardLinkNameNotFound& blnnf)
{ } // bad, but probably not fatal
}

// -----------------------------------------------------------------------------
// Does the recipient have an "Exemplifies" link to a class which has a sequence
// of "IsA" links to an object whose name is c?
// -----------------------------------------------------------------------------
bool
BB1Object::ExemplifiesP(const string sC) const
{
	BB1Object *parentClass = Object("Exemplifies") ;
	return (parentClass ? parentClass->IsAP(sC) : false) ;
}

// -----------------------------------------------------------------------------
// Auxiliary to BB1Object::GetLocalAttribute(...). Two attribute-value pairs
// match if they have the same (1) AttributeName() and
// (2) AttributeValue().Discriminant(). Ignore the recipient (this). A method to
// take advantage of BB1Object's friendship with respect to
// TypedVal::Discriminant().
// -----------------------------------------------------------------------------
bool
BB1Object::AVPMatch(const AttValPair& avp1, const AttValPair& avp2) const
{
  return ((avp1.AttributeName() == avp2.AttributeName()) && // name match
	  			(avp1.AttributeValue().Discriminant() == avp2.AttributeValue().Discriminant())) ;  // type match
}

// -----------------------------------------------------------------------------
// If the attribute is bound locally, return a pointer to it; otherwise, if the
// object's class allows the attribute, return a pointer to the class's
// attribute; otherwise, return a null pointer.
// -----------------------------------------------------------------------------
AttValPair*
BB1Object::GetInheritedAttribute(const AttValPair& avp) const
{
  try
  {
    AttValPair *lavp = GetLocalAttribute(avp) ;
    if (lavp)
    {
      // the attribute is stored on the object
      return (lavp) ;
    }
    else
    {
      // the attribute may be stored with the object's class
      const BB1Class *c = Class() ;
      if (c)
			{
				return (c->GetClassAttribute(avp)) ;
			}
      else
      {
				// ObjectHasNoClass ohnc(BB(), "BB1Object::GetInheritedAttribute(...)", Name()) ;
				return ((AttValPair *) NULL) ; // throw ohnc;
      }
    }
  } // end try
  catch (ObjectHasNoClass& ohnc)
    { return ((AttValPair *) NULL) ; }
}

// -----------------------------------------------------------------------------
// If the attribute is bound locally, return a pointer to it; otherwise, return
// a null pointer.
// -----------------------------------------------------------------------------
AttValPair*
BB1Object::GetLocalAttribute(const AttValPair& avp) const
{
//  static int i = 0; // count invocations
/*
	if (KB().Name() != "Class")
	{
		cout << i++ << " Object = " << LongName() << "\n" ;
	}
*/

	if (false == _attributes.empty())
		for (AVPCIter a = _attributes.begin() ; _attributes.end() != a ; a++)
		{
			/*
			if (KB().Name() != "Class")
			{
				cout << " Pix a = " << a << "\n";
				cout << " Att = "   << attributes(a)->AttributeName() << "\n";
			}
			*/
			AttValPair *cavp = *a ;
			if (AVPMatch(avp, *cavp))
				return *a ;
		}

	return (AttValPair *) 0 ;
}

// -----------------------------------------------------------------------------
// If there is a local link, return a pointer to it; otherwise return a null
// pointer
// -----------------------------------------------------------------------------
BB1Link*
BB1Object::GetLocalLink(const string sLinkName, const BB1Object* pToObject) const
{
	if ((NULL == pToObject) || (_links.empty()))
    return (Linkptr(NULL)) ;

  for (LinkCIter l = _links.begin() ; _links.end() != l ; l++)
  {
    BB1Link *lp = *l ;
    if ((lp->Name() == sLinkName) && lp->ToObject() && (lp->ToObject()->Name() == pToObject->Name()))
      return (*l) ;
  }

  return (Linkptr(NULL)) ;
}

// -----------------------------------------------------------------------------
// Create Exemplifies/ExemplifiedBy links with class c.
// -----------------------------------------------------------------------------
Errcode
BB1Object::Instantiate(BB1Class* pC)
{
  return (AddLink(string("Exemplifies"), string("ExemplifiedBy"), pC)) ;
}

// -----------------------------------------------------------------------------
// Return a pointer to an arbitrary object that is linked by a link name
// -----------------------------------------------------------------------------
BB1Object*
BB1Object::Object(const string& oname) const
{
	ObjectList s ;
	Objects(oname, &s) ;

	if (false == s.empty())
	{
		BB1Object *i = *(s.begin()) ;
		if (i)
			return (i) ;
		else
			return (BB1Object *) 0 ;
	}
	else
		return (BB1Object *) 0 ;
}

// -----------------------------------------------------------------------------
// Return the set of all objects that is linked by a link named name.
// -----------------------------------------------------------------------------
void
BB1Object::Objects(const string& oname, ObjectList *pList) const
{
  if (NULL == pList)
    return ;

	pList->clear() ;

	if (false == _links.empty())
		for (LinkCIter l = _links.begin() ; _links.end() != l ; l++)
		{
			BB1Link	*lp = *l ;
			if (lp && (lp->Name() == oname))
				pList->push_back(lp->ToObject()) ;
/*
			{
				BB1Object *o = &(lp->ToObject()) ;

				const BB1Class *pClass = dynamic_cast<const BB1Class *>(o) ;
				if (pClass)
					retval->push_back(new BB1Class(*pClass)) ;

				const BB1Event *pEvent = dynamic_cast<const BB1Event *>(o) ;
				if (pEvent)
					retval->push_back(new BB1Event(*pEvent)) ;

				const BB1Focus *pFocus = dynamic_cast<const BB1Focus *>(o) ;
				if (pFocus)
					retval->push_back(new BB1Focus(*pFocus)) ;

				const BB1KS	*pKS = dynamic_cast<const BB1KS *>(o) ;
				if (pKS)
					retval->push_back(new BB1KS(*pKS)) ;

				const BB1KSAR *pKSAR = dynamic_cast<const BB1KSAR *>(o) ;
				if (pKSAR)
					retval->push_back(new BB1KSAR(*pKSAR)) ;

				const BB1Message *pMessage = dynamic_cast<const BB1Message *>(o) ;
				if (pMessage)
					retval->push_back(new BB1Message(*pMessage)) ;

				// derivate from BB1Decision than derive from BB1Object
				const BB1Strategy *pStrategy = dynamic_cast<const BB1Strategy *>(o) ;
				if (pStrategy)
					retval->push_back(new BB1Strategy(*pStrategy)) ;
			}
*/
		}
}

// -----------------------------------------------------------------------------
// Return the object's "long" name -- a BB1String of the form <KB name>.<Object name> --
// primarily for output purposes.
// -----------------------------------------------------------------------------
inline
string
BB1Object::LongName() const
{
  return _knowledgeBase.Name() + string(".") + _name ;
}

// -----------------------------------------------------------------------------
// Assign the attributes as specified in the attribute specification. This is
// called whenever a BB1Object (BB1AppInst, BB1Class, BB1Decision, BB1KS,
// BB1KSAR, or BB1Message) is instantiated or modified.
// -----------------------------------------------------------------------------
Errcode
BB1Object::ProcessAttributes(const AVPSet *pAttSpec, const AVPSet *pDelAttSpec)
{
  if (pAttSpec && (false == pAttSpec->empty()))
    for (AVPCIter n = pAttSpec->begin() ; pAttSpec->end() != n ; n++)
    {
      // each attribute specifier
try
{
			AttValPair *newAtt = *n ;              // cache the AVPptr
      if (ValidAttributeP(*newAtt))
      {
      	// does class allow it?
        AttValPair *a = GetLocalAttribute(*newAtt) ; // retrieve from object
        if (a)
        	a->setValue(*newAtt) ;            // modify it if present
        else
        {
        	// add it if absent
          /*
          Memory freed in BB1Object::DeleteAllAttributes()
          and BB1Object::ProcessAttributes()
          */
					// AttValPair *newAVP = new AttValPair(newAtt->AttributeName(), newAtt->AttributeValue()) ;
					AttValPair *newAVP = new AttValPair(*newAtt) ;
          if (newAVP)
          	_attributes.push_back(newAVP) ;
          else
          {
          	ConstructFailure cf(BB(), "BB1Object::ProcessAttributes(...)", "AttValPair") ;
            return (FAILURE) ; // throw cf ;
          }
        }
      }
      else
      {
      	UndeclaredAttribute ex(BB(), "", Name(), newAtt->AttributeName()) ;
        // return (INVALIDATTRIBUTE) ; // throw ex ; Modif by PA: discard the bad attribute, but keep on processing
      }
} // end try
catch (UndeclaredAttribute& ex)
{ return(INVALIDATTRIBUTE) ; }
catch (ConstructFailure& cf)
{ } // constructor aborts
		} // end for

	if (pDelAttSpec && (false == pDelAttSpec->empty()))
		for (AVPCIter d = pDelAttSpec->begin() ; pDelAttSpec->end() != d ; d++)
		{
			AttValPair *delAtt = *d ;

			// const string& delAttName = delAtt->AttributeName() ;
			AttValPair *aptr = GetLocalAttribute(*delAtt) ;
			if (aptr)
			{
				// Delete from the object, not delAttSpec
				if (false == _attributes.empty())
					for (AVPIter i = _attributes.begin() ; _attributes.end() != i ; )
					{
						if (*i == aptr)
						{
							delete (*i) ;
							_attributes.erase(i) ;
						}
						else
							i++ ;
					}
				delete aptr ;
      }
      else
      {
				// It is OK to specify for deletion an attribute that doesn't exist
      }
		} // end for
	return (SUCCESS) ;
}

// -----------------------------------------------------------------------------
// Add one link.
// -----------------------------------------------------------------------------
Errcode
BB1Object::ProcessLinkAdd(const BB1Link* pNewLink)
{
  if (NULL == pNewLink)
    return INVALIDFUNCTIONARGUMENT ;
try
{
  const BB1Class *const pC = Class() ;
  if (NULL == pC)
  {
    ObjectHasNoClass ohnc(BB(), "BB1Object::ProcessLinkAdd(...)", Name()) ;
    return FAILURE ; // throw ohnc ;
  }

  const string sNewLinkName    = pNewLink->Name() ;
  const string sNewOppLinkName = pNewLink->OppositeLinkName(pC) ;

  if (NULLSTRING == sNewOppLinkName)
  {
    // relation undeclared
    LinkDoesNotExist ldne(BB(), "", sNewLinkName) ; // don't know method yet
    return INVALIDLINK ; // throw ldne ;
  }

  // relation declared
  BB1Object *pToObject = pNewLink->ToObject() ;
  if (false == ValidLinkP(sNewLinkName, pToObject))
  {
    UndeclaredLink ulex(BB(), "", Name(), sNewLinkName) ; // don't know method name yet
    return INVALIDLINK ; // throw ulex ;
  }

  // type OK
  if (HasLinkP(sNewLinkName, pToObject))
    return SUCCESS ;  // link already exists

  // no link exists yet, add it
  return AddLink(sNewLinkName, sNewOppLinkName, pToObject) ;
}
catch (UndeclaredLink& ulex)
{ return INVALIDLINK ; } // type not OK
catch (LinkDoesNotExist& ldne)
{ return INVALIDLINK ; }
catch (ObjectHasNoClass& ohnc)
{ } // constructor aborts
	return FAILURE ;
}

// -----------------------------------------------------------------------------
// Delete one link.
// -----------------------------------------------------------------------------
Errcode
BB1Object::ProcessLinkDel(BB1Link* pDelLink)
{
  if (NULL == pDelLink)
    return INVALIDFUNCTIONARGUMENT ;

try
{
  const BB1Class * const pC = Class() ;
  if (NULL == pC)
  {
    ObjectHasNoClass ohnc(BB(), "BB1Object::ProcessLinkDel(...)", Name()) ;
    return FAILURE ; // throw ohnc ;
  }

  const string fLinkName = pDelLink->Name() ;			          // forward link name
  const string bLinkName = pDelLink->OppositeLinkName(pC) ;	// reverse link name

  BB1Object* pToObject = pDelLink->ToObject() ;			        // to object

  BB1Link *flptr = GetLocalLink(fLinkName, pToObject) ;		// ptr to forward link
  if (NULL == flptr)
    return SUCCESS ; // It is OK to specify for deletion a link that doesn't exist

  BB1Link *blptr = pToObject->GetLocalLink(bLinkName, this) ;	// ptr to reverse link
  if (NULL == blptr)
  {
    BackwardLinkNotFound blnf(BB(), "BB1Object::ProcessLinksDel(...)", flptr->Name()) ;
    return FAILURE ; // throw blnf ;
  }

  // Erase forward link from _links
  //
  if (false == _links.empty())
    for (LinkIter l = _links.begin() ; _links.end() != l ; )
    {
      if (*l == flptr)
        _links.erase(l) ;
      else
        l++ ;
    }

  delete flptr ;		// delete forward link

  // Erase backward link from pToObject->_links
  //
  if (false == pToObject->_links.empty())
    for (LinkIter l = pToObject->_links.begin() ; pToObject->_links.end() != l ; )
    {
      if (*l == blptr)
        pToObject->_links.erase(l) ;
      else
        l++ ;
    }

  delete blptr ;		// delete reverse link

  return SUCCESS ;
}
catch (ObjectHasNoClass& ohnc)
{ } // constructor aborts
catch (BackwardLinkNotFound& blnf)
{ return FAILURE ; } // bad but probably not fatal
	return FAILURE ;
}

// -----------------------------------------------------------------------------
// Process the links as specified in the link specification. An error in a link
// specification will not prevent subsequent links from being processed. Thus,
// the values returned by ProcessLinkAdd and ProcessLinkDel are ignored, and the
// operation is always successful.
// -----------------------------------------------------------------------------
Errcode
BB1Object::ProcessLinksNN(const LinkSet *linkSpec, const LinkSet *delLinkSpec)
{
try
{
	if (linkSpec && (false == linkSpec->empty()))
		for (LinkCIter l = linkSpec->begin() ; linkSpec->end() != l ; l++)
    {
    	if (ProcessLinkAdd(*l) != SUCCESS)
      {
      	// throw something
      }
    }

	if (delLinkSpec && (false == delLinkSpec->empty()))
		for (LinkCIter l = delLinkSpec->begin() ; delLinkSpec->end() != l ; l++)
    {
    	if (ProcessLinkDel(*l) != SUCCESS)
      {
      	// throw something
      }
    }
  return SUCCESS ;
} // end try
catch (...)
{
	return FAILURE ; // rajouté par moi pas sur de l'incidence
}                  // every try must have a catch somewhere
}

void
BB1Object::SaveAttributes(ofstream& KBstream)
{
	KBstream << " (" ;
	const SetOfAVP *a = Attributes() ;

	if (a && (false == a->empty()))
		for (AVPCIter pa = a->begin() ; a->end() != pa ; pa++)
			KBstream << "(" << (*pa)->AttributeName() << " " << (*pa)->AttributeValue() << ")" ;
	KBstream << " )\n" ;
}

void
BB1Object::SaveLinks(ofstream& KBstream)
{
	const SetOfBB1Link *l = &_links ;
	KBstream << " (" ;

	if (l && (false == l->empty()))
		for (LinkCIter pl = l->begin() ; l->end() != pl ; pl++)
    {
			KBstream << "(" << (*pl)->Name() << " " ;
      if ((*pl)->ToObject())
        KBstream << (*pl)->ToObject()->LongName() ;
      KBstream << ")" ;
    }

	KBstream << " )\n" ;
}

// -----------------------------------------------------------------------------
// Is the object allowed to have the attribute? Overridden by BB1Class, BB1KSAR,
// and BB1Message.
// -----------------------------------------------------------------------------
bool
BB1Object::ValidAttributeP(const AttValPair& avp) const
{
try
{
  const BB1Class * const c = Class() ;
  if (NULL == c)
  {
    // throw ohnc ;
    ObjectHasNoClass ohnc(BB(), "BB1Object::ValidAttributeP(...)", Name()) ;
    return false ;
  }

  return (c->AllowsAttributeP(avp)) ;
}
catch (ObjectHasNoClass& ohnc)
{
  return (false) ;
}
}

// -----------------------------------------------------------------------------
// Is the object allowed to have the link? Overridden by BB1Class.
// -----------------------------------------------------------------------------
bool
BB1Object::ValidLinkP(const string sLinkName, const BB1Object* pToObject) const
{
  if (NULL == pToObject)
    return false ;

try
{
  const BB1Class *const pFromClass = Class() ;
  if (NULL == pFromClass)
  {
    ObjectHasNoClass ohnc(BB(), "BB1Object::ValidLinkP(...)", Name()) ;
    return false ; // throw ohnc ;
  }

  const BB1Class *const pToClass = pToObject->Class() ;
  if (NULL == pToClass)
  {
    ObjectHasNoClass ohnc(BB(), "BB1Object::ValidLinkP(...)", pToObject->Name()) ;
    return (false) ; // throw ohnc ;
  }

  return (pFromClass->AllowsLinkP(sLinkName, pToClass)) ;
}
catch (ObjectHasNoClass& ohnc)
{
  return (false) ;
}
}

// -----------------------------------------------------------------------------
// Return the value of the attribute for the object. If the attribute is
// unbound, it is an exception. The method is overloaded; there is one method
// for each possible data type of the value.
// -----------------------------------------------------------------------------
bool
BB1Object::Value(const string& attribute, bool& b) const
{
	try
	{
		AttValPair avp(attribute, b) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getBool()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return false ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
  {
    return false ;
  }
	//return (char(0)) ;
}

#ifdef __OB1__UNOPTIM__
char
BB1Object::Value(const string& attribute, char& c) const
{
	try
	{
		AttValPair avp(attribute, c) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getChar()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return (char(0)) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return (char(0)) ; }
	//return (char(0)) ;
}
#endif

double
BB1Object::Value(const string& attribute, double& d) const
{
	try
	{
		AttValPair avp(attribute, d) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getDouble()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return (double(0)) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
  {
    return (double(0)) ;
  }
}

#ifdef __OB1__UNOPTIM__
float
BB1Object::Value(const string& attribute, float& f) const
{
	try
	{
		AttValPair avp(attribute, f) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getFloat()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return (float(0)) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return (float(0)) ; }
      //	return (float(0)) ;
}

int
BB1Object::Value(const string& attribute, int& i) const
{
	try
	{
		AttValPair avp(attribute, i) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getInt()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return (int(0)) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return (int(0)) ; }
       //	return (int(0)) ;
}
#endif

long
BB1Object::Value(const string& attribute, long& l) const
{
	try
	{
		AttValPair avp(attribute, l) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getLong()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return (long(0)) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return (long(0)) ; }
      //	return (long(0)) ;
}

#ifdef __OB1__UNOPTIM__
short
BB1Object::Value(const string& attribute, short& s) const
{
  try
  {
    AttValPair avp(attribute, s) ;
    AVPptr a = GetInheritedAttribute(avp) ;
    if (a)
      return (a->AttributeValue().getShort()) ;
    else
    {
//      ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
      return (short(0)) ; // throw ola ;
    }
  } // end try
  catch (ObjectLacksAttribute& ola)
    { return (short(0)) ; }
 // return (short(0)) ;
}
#endif

const string
BB1Object::Value(const string& attribute, const string& s) const
{
	try
	{
		AttValPair avp(attribute, s) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getString()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return (NULLSTRING) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return (NULLSTRING) ; }
    //	return (NULLSTRING) ;
}

#ifdef __OB1__UNOPTIM__
unsigned char
BB1Object::Value(const string& attribute, unsigned char& uc) const
{
  try
  {
    AttValPair avp(attribute, uc) ;
    AVPptr a = GetInheritedAttribute(avp) ;
    if (a)
        return (a->AttributeValue().getUChar()) ;
    else
    {
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
        return ((unsigned char) 0) ; // throw ola ;
        }
        } // end try
  catch (ObjectLacksAttribute& ola)
    { return ((unsigned char) 0) ; }
 // return ((unsigned char) 0) ;
}
#endif

#ifdef __OB1__UNOPTIM__
unsigned int
BB1Object::Value(const string& attribute, unsigned int& ui) const
{
  try
  {
    AttValPair avp(attribute, ui) ;
    AVPptr a = GetInheritedAttribute(avp) ;
    if (a)
      return (a->AttributeValue().getUInt()) ;
    else
    {
//      ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
      return ((unsigned int) 0) ; // throw ola ;
    }
  } // end try
  catch (ObjectLacksAttribute& ola)
    { return ((unsigned int) 0) ; }
 // return ((unsigned int) 0) ;
}
#endif

#ifdef __OB1__UNOPTIM__
unsigned long
BB1Object::Value(const string& attribute, unsigned long& ul) const
{
	try
	{
		AttValPair avp(attribute, ul) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getULong()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return ((unsigned long) 0) ; // throw ola ;
		}
  } // end try
  catch (ObjectLacksAttribute& ola)
    { return ((unsigned long) 0) ; }
  //return ((unsigned long) 0) ;
}
#endif

#ifdef __OB1__UNOPTIM__
unsigned short
BB1Object::Value(const string& attribute, unsigned short& us) const
{
	try
	{
		AttValPair avp(attribute, us) ;
		AVPptr a = GetInheritedAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getUShort()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return ((unsigned short) 0) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return ((unsigned short) 0) ; }
       //	return ((unsigned short) 0) ;
}
#endif

   /*
const TypedVal&   BB1Object::Attributes(const std::string&  attribut)
{
  if (attributes != NULL)
  {
    register unsigned int end =  attributes->size();
    for (register unsigned int i = 0; i < end; i++)
    {
      if ( (*attributes)[i] != NULL)
        {
          std::string temp =  (*attributes)[i]->AttributeName();
          if (temp == attribut)
            return (( (*attributes)[i]->AttributeValue()) );
        }
    }
  }
  TypedVal *typ = new TypedVal();
  return (*typ);
}    */

const Voidptr
BB1Object::Value(const string& attribute, Voidptr& p) const
{
  try
  {
    AttValPair avp(attribute, p) ;
    AVPptr a = GetInheritedAttribute(avp) ;
    if (a)
       return (a->AttributeValue().getPtr()) ;
    else
	 	{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
	 		return ((void *) NULL) ; // throw ola ;
	 	}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return ((void *) NULL) ; }
}

#ifdef __OB1__UNOPTIM__
PathsList*
BB1Object::Value(const string& attribute, PathsList *paths) const
{
	try
	{
		AttValPair avp(attribute, paths) ;
		AVPptr a = GetLocalAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getPaths()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return ((PathsList *) NULL) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return ((PathsList *) NULL) ; }

	//return ((PathsList *) NULL) ;
}
#endif

NautilusQuestion*
BB1Object::Value(const string& attribute, NautilusQuestion *paths) const
{
	try
	{
		AttValPair avp(attribute, paths) ;
		AVPptr a = GetLocalAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getNautQuest()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return ((NautilusQuestion *) NULL) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return ((NautilusQuestion *) NULL) ; }

	//return ((PathsList *) NULL) ;
}

AnswerStatus*
BB1Object::Value(const string& attribute, AnswerStatus *paths) const
{
	try
	{
		AttValPair avp(attribute, paths) ;
		AVPptr a = GetLocalAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getAnsStatus()) ;
		else
		{
//			ObjectLacksAttribute ola(BB(), "BB1Object::Value(...)", Name(), attribute) ;
			return ((AnswerStatus *) NULL) ; // throw ola ;
		}
	} // end try
	catch (ObjectLacksAttribute& ola)
		{ return ((AnswerStatus *) NULL) ; }

	//return ((PathsList *) NULL) ;
}

NSPatPathoArray*
BB1Object::Value(const string& attribute, NSPatPathoArray *patho) const
{
try
{
	AttValPair avp(attribute, patho) ;
	AVPptr a = GetLocalAttribute(avp) ;
	if (a)
		return (a->AttributeValue().getPatPatho()) ;
	else
		// L'attribut n'est pas trouvé
		// throw Exception(knowledgeBase.BB(), "BB1Object::Value", "Not Found", __FILE__, __LINE__);
		return NULLPATHO ;
} // end try
catch (ObjectLacksAttribute& ola)
{
	throw Exception(_knowledgeBase.BB(), "BB1Object::Value", "Error", __FILE__, __LINE__);
}
}

BB1Object*
BB1Object::Value(const string& attribute, BB1Object *objet) const
{
	try
	{
		AttValPair	avp(attribute, objet) ;
		AVPptr a = GetLocalAttribute(avp) ;
		if (a)
			return (a->AttributeValue().getObj()) ;
		else
			return ((BB1Object *) NULL) ;
	}
	catch (ObjectLacksAttribute& ola)
		{ return ((BB1Object *) NULL) ; }
      //	return ((BB1Object *) NULL) ;
}

NautilusEvent*
BB1Object::Value(const string& attribute, NautilusEvent *pEvent) const
{
try
{
	AttValPair avp(attribute, pEvent) ;
	AVPptr a = GetLocalAttribute(avp) ;
	if (a)
		return (a->AttributeValue().getNautEvent()) ;
	else
		// L'attribut n'est pas trouvé
		// throw Exception(knowledgeBase.BB(), "BB1Object::Value", "Not Found", __FILE__, __LINE__);
		return NULLNAUTILUSEVENT ;
} // end try
catch (ObjectLacksAttribute& ola)
{
	throw Exception(_knowledgeBase.BB(), "BB1Object::Value", "Error", __FILE__, __LINE__);
}
}

vector<HANDLE>*
BB1Object::Value(const string& attribute, vector<HANDLE> *pHandles) const
{
try
{
	AttValPair avp(attribute, pHandles) ;
	AVPptr a = GetLocalAttribute(avp) ;
	if (a)
		return (a->AttributeValue().getHandles()) ;
	else
		// L'attribut n'est pas trouvé
		// throw Exception(knowledgeBase.BB(), "BB1Object::Value", "Not Found", __FILE__, __LINE__);
		return NULLHANDLES ;
} // end try
catch (ObjectLacksAttribute& ola)
{
	throw Exception(_knowledgeBase.BB(), "BB1Object::Value", "Error", __FILE__, __LINE__);
}
}

// -----------------------------------------------------------------------------
// Getting a question's path can be done through a question or a string pair
// this is the reason why we build this function
// -----------------------------------------------------------------------------
std::string
BB1Object::getQuestionPath()
{
	NautilusQuestion* pNsQuestion = Value("question", NULLLNAUTQUEST) ;

  if (pNsQuestion)
		return pNsQuestion->Question() ;

	return Value("question", string("")) ;
}

AnswerStatus::ANSWERSTATUS
BB1Object::getAnswerStatus()
{
	AnswerStatus* pAnswerStatus = Value("answerStatus", NULLLANSWERSTATUS) ;

  if (NULL == pAnswerStatus)
  	return AnswerStatus::astatusUnknown ;

	return pAnswerStatus->Status() ;
}

AnswerStatus::ANSWERPROCESSINGSTAGE
BB1Object::getAnswerProcessStage()
{
	AnswerStatus* pAnswerStatus = Value("answerStatus", NULLLANSWERSTATUS) ;

  if (NULL == pAnswerStatus)
  	return AnswerStatus::apstageUnknown ;

	return pAnswerStatus->ProcessingStage() ;
}

string
BB1Object::getAnswerStatusAsString()
{
  AnswerStatus* pAnswerStatus = Value("answerStatus", NULLLANSWERSTATUS) ;

  if (NULL == pAnswerStatus)
  	return string("Unaccessible") ;

  return pAnswerStatus->getStatusAsString() ;
}

string
BB1Object::getAnswerProcessStageAsString()
{
  AnswerStatus* pAnswerStatus = Value("answerStatus", NULLLANSWERSTATUS) ;

  if (NULL == pAnswerStatus)
  	return string("Unaccessible") ;

  return pAnswerStatus->getProcessingStageAsString() ;
}

void
BB1Object::setAnswerStatus(AnswerStatus::ANSWERSTATUS iStatus, HWND interfaceHandle)
{
	AnswerStatus* pAnswerStatus = Value("answerStatus", NULLLANSWERSTATUS) ;

  if (pAnswerStatus)
  	pAnswerStatus->setStatus(iStatus) ;
  else
  {
    AnswerStatus* pNewStatus = new AnswerStatus() ;
    pNewStatus->setStatus(iStatus) ;
    AttValPair *newAVP = new AttValPair("answerStatus", pNewStatus) ;
    if (newAVP)
  	  _attributes.push_back(newAVP) ;
    pAnswerStatus = Value("answerStatus", NULLLANSWERSTATUS) ;
  }

  // If Answer has been processed; warn suscribed interfaces
  //
  if (AnswerStatus::astatusProcessed == iStatus)
  {
    bool bVerbose = true ;

    if (pAnswerStatus && ((AnswerStatus::apverboseSilent        == pAnswerStatus->VerboseStatus()) ||
                          (AnswerStatus::apverboseSilentUpdated == pAnswerStatus->VerboseStatus())))
    {
      bVerbose = false ;
      pAnswerStatus->setVerboseStatus(AnswerStatus::apverboseSilentUpdated) ;
    }

    if (bVerbose)
    {
      TypedVal *pPathVal      = PathAttributes() ;
      TypedVal *pSearchParams = SearchParams() ;
      BB().alertClientsForAnswerUpdate(pPathVal, interfaceHandle, pSearchParams) ;
    }
  }
}

void
BB1Object::setAnswerProcessStage(AnswerStatus::ANSWERPROCESSINGSTAGE iStage)
{
	AnswerStatus* pAnswerStatus = Value("answerStatus", NULLLANSWERSTATUS) ;

  if (pAnswerStatus)
  {
  	pAnswerStatus->setStage(iStage) ;
    return ;
  }

  AnswerStatus* pNewStatus = new AnswerStatus() ;
  pNewStatus->setStage(iStage) ;
  AttValPair *newAVP = new AttValPair("answerStatus", pNewStatus) ;
  if (newAVP)
  	_attributes.push_back(newAVP) ;
}

bool
BB1Object::updatePatPatho(NSPatPathoArray* patho, string sAnswerDate, bool bExt)
{
	AttValPair avp("node", patho) ;
	AVPptr a = GetLocalAttribute(avp) ;
	if (!a)
  	return false ;

	a->swapPatho(patho, bExt) ;

  AttValPair avpDate("node_date", sAnswerDate) ;
	AVPptr b = GetLocalAttribute(avpDate) ;
	if (!b)
	{
  	AttValPair *newAVP = new AttValPair("node_date", sAnswerDate) ;
		if (newAVP)
  		_attributes.push_back(newAVP) ;
	}
  else
  {
  	TypedVal newTVal(sAnswerDate) ;
  	TypedVal* pVal = b->AttributeValuePtr() ;
    if (pVal)
    	*pVal = newTVal ;
  }

  return true ;
}

// Is there any interface or token registered to this question
//
//
bool
BB1Object::isOrphanQuestion()
{
  TypedVal* pInterfaceTypedVal = InterfacesHandlesAttributes() ;
  if (pInterfaceTypedVal && (pInterfaceTypedVal->getHandlesCount() > 0))
    return false ;

  TypedVal* pTokenTypedVal = TokensHandlesAttributes() ;
  if (pTokenTypedVal && (pTokenTypedVal->getHandlesCount() > 0))
    return false ;

  TypedVal* pPersistenceTypedVal = PersistenceHandlesAttributes() ;
  if (pPersistenceTypedVal && (pPersistenceTypedVal->getHandlesCount() > 0))
    return false ;

  return true ;
}

// -----------------------------------------------------------------------------
// Is this a subclass of the named class? Default; BB1Class overrides
// -----------------------------------------------------------------------------
bool
BB1Object::IsAP(const string /* c */) const
{
  return false ;
}

// -----------------------------------------------------------------------------
// Does this object trigger KS UpdatePrescription? Default; BB1Strategy
// overrides
// -----------------------------------------------------------------------------
bool
BB1Object::UpdatePrescriptionTCAux() const
{
  return (false) ;
}

// -----------------------------------------------------------------------------
// Does this object obviate KS UpdatePrescription? Default; BB1Strategy
// overrides
// -----------------------------------------------------------------------------
bool
BB1Object::UpdatePrescriptionOCAux() const
{
  try
  {
    NotAStrategy nas(BB(), "UpdatePrescriptionOC(...)", Name()) ;
    return (true) ; // throw nas ;
  } // end try
  catch (NotAStrategy& nas)
    { return (true) ; }
  //return (true) ;
}

Errcode
BB1Object::UpdatePrescriptionACAux()
{
  try
  {
    NotAStrategy nas(BB(), "UpdatePrescriptionAC(...)", Name()) ;
    return (FAILURE) ; // throw nas;
  } // end try
  catch (NotAStrategy& nas)
    { return (FAILURE) ; }
  //return (FAILURE) ;
}

// -----------------------------------------------------------------------------
// Does this object trigger KS TerminatePrescription? Default; BB1Decision
// overrides
// -----------------------------------------------------------------------------
bool
BB1Object::TerminatePrescriptionTCAux() const
{
  return (false) ;
};

// -----------------------------------------------------------------------------
// Does this object satisfy the precondition of KS TerminatePrescription?
// Default; BB1Decision overrides
// -----------------------------------------------------------------------------
bool
BB1Object::TerminatePrescriptionPCAux() const
{
  try
  {
    NotADecision nad(BB(), "TerminatePrescriptionPC(...)", Name()) ;
    return (false) ; // throw nad ;
  } // end try
  catch (NotADecision& nad)
    { return (false) ; }
  //return (false) ;
}

// -----------------------------------------------------------------------------
// Does this object obviate KS TerminatePrescription? Default; BB1Decision
// overrides
// -----------------------------------------------------------------------------
bool
BB1Object::TerminatePrescriptionOCAux() const
{
  try
  {
    NotADecision nad(BB(), "TerminatePrescriptionOC(...)", Name()) ;
    return (true) ; // throw nad ;
  } // end try
  catch (NotADecision& nad)
    { return (true) ; }
 // return (true) ;
}

Errcode
BB1Object::TerminatePrescriptionACAux()
{
  try
  {
    NotAStrategy nas(BB(), "TerminatePrescriptionAC(...)", Name()) ;
    return (FAILURE) ; // throw nas ;
  } // end try
  catch (NotAStrategy& nas)
    { return (FAILURE) ; }
  //return (FAILURE) ;
}

// -----------------------------------------------------------------------------
// Default; BB1Strategy overrides.
// -----------------------------------------------------------------------------
void
BB1Object::DeleteParentDecision(BB1Decision* /* decision */)
{
try
{
	NotAStrategy nas(BB(), "TerminatePrescriptionAC(...)", Name()) ;
  // throw nas ;
}
catch (NotAStrategy& nas)
{ }
}

