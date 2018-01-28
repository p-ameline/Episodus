/*
**
** BB1Event.h
**
*/

/*
** An event is a record of a change of a single object on the blackboard.
** The object can be added or deleted, or any number of attributes and
** links can have been modified.
*/

#ifndef _BB1Event_h
# define _BB1Event_h 1

//class ostream; // for operator<<(...)
class BB1BB ;
class BB1KB ;

# ifdef __GNUC__
#  pragma interface
# endif // __GNUC__

# include "BB1Object.h"

#ifndef __TCONTROLER__

string&		decToString(string& S, const int i) ;

class BB1Event : public BB1Object
{
  // -------------------------------------------------------------------------
	// friend class

  /* Allowed to call BB1Event() */
  friend class BB1BB ;

  friend class BB1Object ;

  friend class ObjectList ;
	friend class EventSet ;
	friend class ObjectSet ;

	// -------------------------------------------------------------------------
	// public functions

  public: // API

	  BB1Event(	const string&		oname,
		  				BB1KB&					kb,
			  			BB1Class				*c,
				  		const AVPSet		*chgAttributes,
					  	const LinkSet		*chgLinks,
					  	EventType				changeType,
					  	BB1Object&			Object) ;

	  BB1Event(const BB1Event& src) ;

	  ~BB1Event() ;

	  bool ChangedAttributeIsP(const string &attribute) const
	  {
		  if (false == _changedAttributes.empty())
			  for (AVPCIter a = _changedAttributes.begin() ; _changedAttributes.end() != a ; a++)
			  {
				  if ((*a)->AttributeName() == attribute)
				  	return true ;
			  }
		  return false ;
	  } ;

	  bool							ChangedLinkIsP(const string &linkName) const ;
	  bool							EventKBNamedP(const string& eventName) const ;

	  /* Is the event of the specified type? */
	  bool							EventTypeIsP(const EventType eventType) const { return (_type == eventType) ; } ;

	  /* Return the object whose creation or modification engendered the event */
	  BB1Object& 				Object() const { return _object ; } ;

	  const string&			TypeString() const ;

	  const AVPSet			*ChangeAttributes() const { return &_changedAttributes ; }

    static long getNbInstance() { return _lEventCount ; }

  // -------------------------------------------------------------------------
  // private functions
  private:

	  /* Return the kind of blackboard change that the event represents */
	  EventType					Type() const { return _type ; } ;

	  void							Save(ofstream& KBstream) { } ; // Events can't be saved
	  void							IncrementKSARCount(int inc = 1) { _BB1EventKSARcount += inc ; } ;
	  bool							KSARlessP() const               { return 0 == _BB1EventKSARcount ; } ;

  // -------------------------------------------------------------------------
  // private members
  private:

	  const EventType _type ;							  // what kind of blackboard modification
	  BB1Object&      _object ;						  // the object added, deleted, or modified
	  AVPSet          _changedAttributes ;	// list of attributes added, deleted, or modified
	  LinkSet         _changedLinks ;			  // list of links added or deleted
	  int             _BB1EventKSARcount ;	// # of KSARs for which this is trigger event

    static long     _lEventCount ;
};

std::ostream& operator<<(std::ostream& os, const BB1Event& obj) ;

#endif

#endif // _BB1Event_h

