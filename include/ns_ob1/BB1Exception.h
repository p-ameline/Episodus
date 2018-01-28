/*
**
** BB1Exception.h
**
*/


#ifndef _BB1Exception_h
# define _BB1Exception_h 1

# include "BB1BB.h"

# include <string.h>
# include <stdlib.h>            // for abort()

//# include "BB1String.h"

class BB1BB ;

void    DefaultErrorHandler(const BB1BB& bb, const string& method, const string& message) ;

/**
* \brief This is the root of all exceptions.  Its constructor carries out its action
*/
/**
*  This is the root of all exceptions.  Its constructor carries out its action
*/
class Exception		// abstract
{

 public:
  Exception(const BB1BB& bb, const string& method, const string& message, std::string file ="", int line=-1)
    : d_method(method),
      d_message(message) ,
  		d_FileNameError(file),
      d_LineNumber(line)
  { ExceptionAction(bb) ; }

  Exception( const string& method, const string& message, std::string file ="", int line=-1)
    : d_method(method),
      d_message(message),
      d_FileNameError(file),
      d_LineNumber(line)
  {  }

  const string&	Method()  { return (d_method) ; }
  const string&	Message() { return (d_message); }
  std::string&  File()    { return (d_FileNameError);}
  int  LineNumber() { return (d_LineNumber); }


 protected:
  virtual void ExceptionAction(const BB1BB& bb) ;


 protected:
  const std::string	 d_method ;
  const std::string	 d_message ;
  std::string  			 d_FileNameError;
  int 			 d_LineNumber;
} ;

/**
* \brief The following exceptions are caused by BBK, not the application program
*/
/**
*  The following exceptions are caused by BBK, not the application program
*/
class BBKException : public Exception	// abstract
{
 public:
  BBKException(const BB1BB& bb, const string& method, const string& message)
    : Exception(bb, method, message)
  {}
} ;



/*
* \brief If a call to "new" returns a null pointer, memory is probably exhausted.
*/
/*
*  If a call to "new" returns a null pointer, memory is probably exhausted.
* So the constructor will call abort().
*/
class ConstructFailure : public BBKException
{
 public:
  ConstructFailure(const BB1BB& bb, const string& method, const string& cl)
    : BBKException(bb, method, "Failure to instantiate class " + cl)
  { abort() ; }
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class UnknownEventType : public BBKException
{
public:
  UnknownEventType(const BB1BB& bb, const string& method)
    : BBKException(bb, method, "Unknown Event type")
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class UnknownKSARStatus : public BBKException
{
public:
  UnknownKSARStatus(const BB1BB& bb, const string& method)
    : BBKException(bb, method, "Unknown KSAR status")
  {}
} ;



/**
* \brief The value of the phase is not recognized by one of the checkers.
*/
/**
* The value of the phase is not recognized by one of the checkers.
* Call abort()
*/
class UnknownPhase : public BBKException
{
public:
  UnknownPhase(const BB1BB& bb, const string& method)
    : BBKException(bb, method, "Unknown phase")
  { abort() ; }
} ;



/**
* \brief A built-in entity (KB, Class, or Link) that must exist was not found.
*/
/**
* A built-in entity (KB, Class, or Link) that must exist was not found. <BR>
* Call abort()
*/
class NotFound : public BBKException		// abstract
{
public:
  NotFound(const BB1BB& bb, const string& method, const string& type, const string& name)
    : BBKException(bb, method, type + " " + name + " not found")
  { abort() ; }
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ClassNotFound : public NotFound		// abstract
{
public:
  ClassNotFound(const BB1BB& bb, const string& method, const string& className)
    : NotFound(bb, method, "Class", className)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class KBNotFound : public NotFound
{
public:
  KBNotFound(const BB1BB& bb, const string& method, const string& KBName)
    : NotFound(bb, method, "Knowledge base", KBName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ClassKBNotFound : public KBNotFound
{
public:
  ClassKBNotFound(const BB1BB& bb, const string& method)
    : KBNotFound(bb, method, "Class")
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class BackwardLinkNotFound : public NotFound
{
public:
  BackwardLinkNotFound(const BB1BB& bb, const string& method, const string& linkName)
    : NotFound(bb, method, "backward Link", linkName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class BackwardLinkNameNotFound : public NotFound
{
public:
  BackwardLinkNameNotFound(const BB1BB& bb, const string& method, const string& linkName)
    : NotFound(bb, method, "backward Link name", linkName)
  {}
} ;

/**
* \brief Exception.
*/
/**
*  Exception
*/
class LinkNotFound : public NotFound
{
public:
  LinkNotFound(const BB1BB& bb, const string& method, const string& linkName)
    : NotFound(bb, method, "Link", linkName)
  {}
} ;



/**
* \brief Exception.
*/
/**
*  Exception
*/
class NullClassObject : public BBKException
{
public:
  NullClassObject(const BB1BB& bb, const string& method)
    : BBKException(bb, method, "Null class object")
  {}
} ;



/**
* \brief There must be a scheduled KSAR, but is none.
*/
/**
* There must be a scheduled KSAR, but is none. <BR> Call abort()
*/
class NullKSAR : public BBKException
{
public:
  NullKSAR(const BB1BB& bb, const string& method)
    : BBKException(bb, method, "Null KSAR during agenda management or interpretation")
    { abort() ; }
} ;



/**
* \brief There must be a trigger event, but is none.
*/
/**
* There must be a trigger event, but is none.  Call abort()
*/
class NullTriggerEvent : public BBKException
{
public:
  NullTriggerEvent(const BB1BB& bb, const string& method)
    : BBKException(bb, method, "Null Trigger Event during triggering")
  { abort() ; }
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class NullTriggerObject : public BBKException
{
public:
  NullTriggerObject(const BB1BB& bb, const string& method)
    : BBKException(bb, method, "Null Trigger Object during triggering")
  {}
} ;



/**
* \brief An object must have a class.
*/
/**
* An object must have a class.  Call abort().
*/
class ObjectHasNoClass : public BBKException
{
public:
  ObjectHasNoClass(const BB1BB& bb, const string& method, const string& objectName)
    : BBKException(bb, method, "Object " + objectName + " has no class")
  { abort() ; }
} ;




/**
* \brief The cycle count has become out of synch.
*/
/**
* The cycle count has become out of synch.  call abort().
*/
class TooManyCycles : public BBKException
{
public:
  TooManyCycles(const BB1BB& bb, const string& method)
    : BBKException(bb, method, "Too many cycles were executed")
  { abort() ; }
} ;



/**
* \brief Exception.
*/
/**
*  Exception
*/
class WrongType : public BBKException		// abstract
{
public:
  WrongType(const BB1BB& bb, const string& method, const string& objectName, const string& t)
    : BBKException(bb, method, "Object " + objectName + "is not a " + t)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class NotADecision : public WrongType
{
public:
  NotADecision(const BB1BB& bb, const string& method, const string& objectName)
    : WrongType(bb, method, objectName, "BB1Decision")
  {}
} ;



/**
* \brief Exception.
*/
/**
*  Exception
*/
class NotAStrategy : public WrongType
{
public:
  NotAStrategy(const BB1BB& bb, const string& method, const string& objectName)
    : WrongType(bb, method, objectName, "BB1Strategy")
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ListException
{
public:
  ListException(const string& method, const string& message) 
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class StringException
{
public:
  StringException(const string& method, const string& message)
  {}
} ;



/**
* \brief The following exceptions are caused by the application program, not BBK
*/
/**
*  The following exceptions are caused by the application program, not BBK
*/
class AppException : public Exception		// abstract
{
public:
  AppException(const BB1BB& bb, const string& method, const string& message)
    : Exception(bb, method, message)
  {}
} ;



/**
* \brief Exception.
*/
/**
*  Exception
*/
class AlreadyExists : public AppException	// abstract
{
public:
  AlreadyExists(const BB1BB& bb, const string& method, const string& type, const string& name)
    : AppException(bb, method, type + " named " + name + " already exists")
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class KBAlreadyExists : public AlreadyExists
{
public:
  KBAlreadyExists(const BB1BB& bb, const string& method, const string& KBName)
    : AlreadyExists(bb, method, "Knowledge Base", KBName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class LinkAlreadyExists : public AlreadyExists
{
public:
  LinkAlreadyExists(const BB1BB& bb, const string& method, const string& linkName)
    : AlreadyExists(bb, method, "Link", linkName) 
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ObjectAlreadyExists : public AlreadyExists	// abstract
{
public:
  ObjectAlreadyExists(const BB1BB& bb, const string& method, const string& type, const string& objectName)
    : AlreadyExists(bb, method, type, objectName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class AppInstAlreadyExists : public ObjectAlreadyExists
{
public:
  AppInstAlreadyExists(const BB1BB& bb, const string& method, const string& instName)
    : ObjectAlreadyExists(bb, method, "AppInst", instName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ClassAlreadyExists : public ObjectAlreadyExists
{
public:
  ClassAlreadyExists(const BB1BB& bb, const string& method, const string& className)
    : ObjectAlreadyExists(bb, method, "Class", className)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class DecisionAlreadyExists : public ObjectAlreadyExists	// abstract
{
public:
  DecisionAlreadyExists(const BB1BB& bb, const string& method, const string& type, const string& decisionName)
    : ObjectAlreadyExists(bb, method, type, decisionName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class FocusAlreadyExists : public DecisionAlreadyExists
{
public:
  FocusAlreadyExists(const BB1BB& bb, const string& method, const string& focusName)
    : DecisionAlreadyExists(bb, method, "Focus", focusName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class StrategyAlreadyExists : public DecisionAlreadyExists
{
public:
  StrategyAlreadyExists(const BB1BB& bb, const string& method, const string& strategyName)
    : DecisionAlreadyExists(bb, method, "Strategy", strategyName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class KSAlreadyExists : public ObjectAlreadyExists
{
public:
  KSAlreadyExists(const BB1BB& bb, const string& method, const string& KSName)
    : ObjectAlreadyExists(bb, method, "Knowledge Source", KSName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class DoesNotExist : public AppException
{
public:
  DoesNotExist(const BB1BB& bb, const string& method, const string& type, const string& name)
    : AppException(bb, method, type + " named " + name + " does not exist")
    {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class KBDoesNotExist : public DoesNotExist
{
public:
  KBDoesNotExist(const BB1BB& bb, const string& method, const string& KBName)
    : DoesNotExist(bb, method, "Knowledge Base", KBName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class LinkDoesNotExist : public DoesNotExist
{
public:
  LinkDoesNotExist(const BB1BB& bb, const string& method, const string& linkName)
    : DoesNotExist(bb, method, "Link", linkName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ObjectDoesNotExist : public DoesNotExist	// abstract
{
public:
  ObjectDoesNotExist(const BB1BB& bb, const string& method, const string& type, const string& objectName)
    : DoesNotExist(bb, method, type, objectName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ClassDoesNotExist : public ObjectDoesNotExist
{
public:
  ClassDoesNotExist(const BB1BB& bb, const string& method, const string& className)
    : ObjectDoesNotExist(bb, method, "Class", className)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class TypeError : public AppException		// abstract
{
public:
  TypeError(const BB1BB& bb, const string& method, const string& objName, const string& entType, const string& entName)
    :  AppException(bb, method, "Object " + objName + " can not take " + entType + " " + entName)
  {}
} ;



/**
* \brief Exception.
*/
/**
*  Exception
*/
class UndeclaredAttribute : public TypeError
{
public:
  UndeclaredAttribute(const BB1BB& bb, const string& method, const string& objName, const string& attName)
    : TypeError(bb, method, objName, "attribute", attName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class UndeclaredLink : public TypeError
{
public:
  UndeclaredLink(const BB1BB& bb, const string& method, const string& objName, const string& linkName)
    : TypeError(bb, method, objName, "link", linkName)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class PhaseError : public AppException		// abstract
{
public:
  PhaseError(const BB1BB& bb, const string& method, const string& message)
    : AppException(bb, method, message)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class CreatePhaseError : public PhaseError
{
public:
  CreatePhaseError(const BB1BB& bb, const string& method, const string& entType, const string& name)
    : PhaseError(bb, method, name + ": Create KBs and objects only outside the cycle or from KS actions")
  {}
} ;



/**
* \brief Exception.
*/
/**
*  Exception
*/
class CreateAppInstPhaseError : public CreatePhaseError
{
public:
  CreateAppInstPhaseError(const BB1BB& bb, const string& method, const string& instName)
    : CreatePhaseError(bb, method, "AppInst", instName)
  {}
} ;



/**
* \brief Exception.
*/
/**
*  Exception
*/
class CreateClassPhaseError : public CreatePhaseError
{
public:
  CreateClassPhaseError(const BB1BB& bb, const string& method, const string& className)
    : CreatePhaseError(bb, method, "Class", className)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class CreateKBPhaseError : public CreatePhaseError
{
public:
  CreateKBPhaseError(const BB1BB& bb, const string& method, const string& KBName)
    : CreatePhaseError(bb, method, "Knowledge Base", KBName)
  {}
} ;



class CreateKSPhaseError : public CreatePhaseError
{
public:
  CreateKSPhaseError(const BB1BB& bb, const string& method, const string& KSName)
    : CreatePhaseError(bb, method, "Knowledge Source", KSName)
  {}
} ;




class SocketPhaseError : public PhaseError
{
public:
  SocketPhaseError(const BB1BB& bb, const string& method)
    : PhaseError(bb, method, "Do not manipulate the socket during cycle execution")
  {}
} ;



class TracePhaseError : public PhaseError
{
public:
  TracePhaseError(const BB1BB& bb, const string& method)
    : PhaseError(bb, method, "Do not set tracing during cycle execution")
  {}
} ;

class GoPhaseError : public PhaseError
{
public:
  GoPhaseError(const BB1BB& bb, const string& method, const string& message)
    : PhaseError(bb, method, message)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ProtectedKB : public AppException
{
public:
  ProtectedKB(const BB1BB& bb, const string& method, const string& kb)
    : AppException(bb, method, "Knowledge base " + kb + " is protected against application changes")
    {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class NullKSTC : public AppException
{
public:
  NullKSTC(const BB1BB& bb, const string& method, const string& KSName)
    : AppException(bb, method, "Knowledge Source " + KSName + " has a null Trigger Condition")
    {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class NullKSAction : public AppException
{
public:
  NullKSAction(const BB1BB& bb, const string& method, const string& KSName)
    : AppException(bb, method, "Knowledge Source " + KSName + " has a null Action")
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class LinkNameReserved : public AppException
{
public:
  LinkNameReserved(const BB1BB& bb, const string& method, const string& linkName) :
    AppException(bb, method, "Link name " + linkName + " is reserved")
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class Undefined : public AppException		// abstract
{
public:
  Undefined(const BB1BB& bb, const string& method, const string& message)
    : AppException(bb, method, message)
  {}
} ;



/**
* \brief Exception.
*/
/**
*  Exception
*/
class KSUndefined : public Undefined
{
public:
  KSUndefined(const BB1BB& bb, const string& method, const string& message)
    : Undefined(bb, method, message)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class KSARUndefined : public Undefined
{
public:
  KSARUndefined(const BB1BB& bb, const string& method, const string& message)
    : Undefined(bb, method, message)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class TriggerEventUndefined : public Undefined
{
public:
  TriggerEventUndefined(const BB1BB& bb, const string& method, const string& message)
    : Undefined(bb, method, message)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class TriggerObjectUndefined : public Undefined
{
public:
  TriggerObjectUndefined(const BB1BB& bb, const string& method, const string& message)
    : Undefined(bb, method, message)
  {}
} ;


/**
* \brief Exception.
*/
/**
*  Exception
*/
class ObjectLacksAttribute : public AppException
{
public:
  ObjectLacksAttribute(const BB1BB& bb, const string& method, const string& objectName, const string& attribute)
    : AppException(bb, method, "Object " + objectName + " does not have attribute " + attribute)
  {}
} ;


#endif // _BB1Exception_h
