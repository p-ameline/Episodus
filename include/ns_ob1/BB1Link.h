/*
**
** BB1Link.h
**
*/

/*
** A link is an element of a binary relation on pairs of objects.
** A link is multiply labeled.  Its link type is a required symbolic label.
** In addition, links can be labeled with an arbitrary set of attribute-value pairs.
*/

#ifndef _BB1Link_h
# define _BB1Link_h 1

//class ostream; // for operator<<(...)
class BB1BB ;
class BB1Class ;
class BB1Object ;

# ifdef __GNUC__
#  pragma interface
# endif

# include "BB1types.h"

class BB1LinkPair
{
  // Allowed to call consructor
  friend class BB1BB;
  friend class LinkPairSet ;

  public: // Applications don't use this class directly

	  BB1LinkPair(const string& ForwardLink, const string& InverseLink, const string& FromClassName, const string& ToClassName);
    BB1LinkPair(const BB1LinkPair& src) ;
    ~BB1LinkPair() ;

  private:

    const string&	ForwardLink()   const { return (m_ForwardLink) ; } ;
    const string&	InverseLink()   const { return (m_InverseLink) ; } ;
    const string&	FromClassName() const { return (m_FromClassName) ; } ;
    const string&	ToClassName()   const { return (m_ToClassName) ; } ;

  private:

    const string m_ForwardLink ;
    const string m_InverseLink ;
    const string m_FromClassName ;
    const string m_ToClassName ;
} ;

typedef BB1LinkPair* LnPrptr ;

class BB1Link
{
  // Allowed to call BB1Link()
  friend class BB1Object;

  public: // API

    BB1Link(const string sName, BB1Object* pToObject) ;
    BB1Link(const string sName, BB1Object* pToObject, const Cycle cycle, bool bReflexiveP) ;

    BB1Link(const BB1Link& src) ;

    ~BB1Link() ;

    // Return the name of the forward link
    const string Name() const { return _sName ; } ;

    const string OppositeLinkName(const BB1Class* pC) const ;

    // Return the second element of the relation pair
    BB1Object*	 ToObject() const { return _pToObject ; } ;

    // Are the from class and to class the same?
    bool		     ReflexiveP() const { return _bReflexiveP ; } ;

    // Has the other of two reflexive links been handled yet?
    bool		     HandledP() const   { return _bHandledP ; } ;

    // Mark the opposite link as having been handled
    void		     MarkAsHandled()  { _bHandledP = true ; }

    static long  getNbInstance()  { return _lBB1LinkCount ; }
    static void  initNbInstance() { _lBB1LinkCount = 0 ; }

  // -------------------------------------------------------------------------
  //
  // private members
  //
  // -------------------------------------------------------------------------
  private:

    const string _sName ;		      // name R of the relation pair R(f, t)
    BB1Object*	 _pToObject ;		  // 2nd element t of the relation pair R(f, t)
    const Cycle	 _CycleCreated ;  // The execution cycle on which the link was created
    bool		     _bReflexiveP ;		// Are the from class and to class the same?
    bool		     _bHandledP ;		  // In cleanup, has the link been handled yet?

    static long  _lBB1LinkCount ;
};

typedef BB1Link* Linkptr ;

std::ostream&		operator<<(std::ostream& os, const BB1Link& link) ;

void			ClearSetOfBB1Link(LinkSet *linkset) ;

#endif // _BB1Link_h

