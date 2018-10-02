//
// BB1UserEvent.h
//
// Made by (PA)
//
// Started on  Jan 2006 PA
//

#ifndef   BB1USEREVENT_H_
# define  BB1USEREVENT_H_

#include <string>

#include "ns_ob1\OB1Export.h"
#include "nsldv\nsldvuti.h"
// #include "nsbb\nspatpat.h"
// #include "ns_ob1\BB1types.h"

/*
** Classs which describe a Nautilus Event
** A Nautilus event is a pathpatho as root for
** the document and an a pathpathoiter for the change
*/

/**
* \brief Describe an event that append in Nautilus
*/
/**
* Describe an event that append in Nautilus <BR>
* Evenement survenu dans nautilus
*/
class _BBKEXPORT NautilusEvent // : public BB1Task
{
  private:

    static long lObjectCount ;

  protected:

  	const NSPatPathoArray* _root ;
  	PatPathoConstIter      _pointer ;
    NVLdVTemps             _informationDate ;
  	EventType              _userEventType ;
    bool                   _bMustDeletePatho ;

  public:

  	NautilusEvent(const NSPatPathoArray* root, PatPathoConstIter pointer, EventType UserEventType, bool bDuplicatePatho) ;
  	NautilusEvent() ;

    NautilusEvent(const NautilusEvent& src) ;
    NautilusEvent& operator=(const NautilusEvent& src) ;

    ~NautilusEvent() ;

  	/**
  	*
  	*/
  	int                 Priority() { return (1) ; }
  	// virtual BB1TaskType Type() { return (NAUTILUS_EVENT_TASK) ; }

  	void             putRoot(NSPatPathoArray* root)    { _root = root ; }
  	void             putPointer(PatPathoIter pointer)  { _pointer = pointer ; }
    void             putDate(NVLdVTemps tpsDate)       { _informationDate = tpsDate ; }
    void             putUserEventType(EventType iType) { _userEventType = iType ; }

  	const NSPatPathoArray* getRoot()         { return _root ; }
  	PatPathoConstIter      getPointer()      { return _pointer ; }
    NVLdVTemps             getDate()         { return _informationDate ; }
  	EventType              getUserEvenType() { return _userEventType ; }

    bool             synchronizeIterator(const NSPatPathoArray* pModelPPT, PatPathoConstIter modelPointer) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

#endif

