/*
**
** BB1Message.h
**
*/

/*
** A message is a record of a communication from an external process.
** Only BB1 can create, modify, or remove messages.
*/

#ifndef _BB1Message_h
# define _BB1Message_h 1

//class ostream; // for operator<<(...)
class BB1BB ;

# ifdef __GNUC__
#  pragma interface
# endif

# include "BB1Object.h"

/**
* \brief BB1 Message
*/
/**
* BB1 Message <BR>
* Message BB1
*/
class BB1Message : public BB1Object
{
  // Allowed to call BB1Message()
  friend class BB1BB ;
  friend class BB1Object ;

  friend class ObjectList ;
  friend class ObjectSet ;


public:

protected:
   /*
   ** Is the object allowed to have the attribute?  A message can take any attribute.
   ** Overrides BB1Object.
   */

  bool			ValidAttributeP(const AttValPair& avp) const { return (true) ; } ;


private:
  BB1Message(const string&	oname,
       AttValPair&   dispatch,
	     BB1KB&		kb,
	     BB1Class		*c,
	     const AVPSet	*attSpec,
	     const LinkSet	*linkSpec = 0) ;


  BB1Message(const BB1Message& src) ;

  ~BB1Message() { } ;

  void			Save(ofstream& KBstream) { } ; // Messages can't be saved
} ;


typedef BB1Message *Msgptr ;


std::ostream&	operator<<(std::ostream& os, const BB1Message& message) ;


#endif // _BB1Message_h
