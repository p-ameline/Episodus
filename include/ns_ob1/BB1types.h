// -----------------------------------------------------------------------------
// BB1types.h
// -----------------------------------------------------------------------------
// $Revision: 1.8 $
// $Author: pameline $
// $Date: 2015/02/07 14:29:27 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - 2002
// -----------------------------------------------------------------------------

#ifndef __BB1TYPES_H__
#define __BB1TYPES_H__

class KsInitStructure;
class AttValPair ;
class BB1BB ;
class BB1KB ;
class BB1Link ;
class BB1LinkPair ;
class BB1Object ;
class BB1Event ;
class BB1Focus ;
class BB1KS ;
class BB1KSAR ;
class BB1Strategy ;
class BB1Token ;
class Transfert ;
class NautilusEvent ;

class AKS ;

class NSPatPathoArray ;
class NSPatPathoInfo ;
typedef NSPatPathoInfo** PatPathoIter ;
class NautilusQuestion ;
class AnswerStatus ;

#include "partage\ns_vector.h"
#include "ns_ob1\OB1Export.h"
#include "ns_ob1\OB1Types.h"
#include "ns_ob1\BB1List.h"
#include "ns_ob1\BB1Set.h"
#include "partage\ns_search.h"

// #include "ns_ob1\NautilusType.h"

//# include "ns_bbk\AttValPair.h"

// #include "nsbb\nspatpat.h"
//# include "ns_bbk\TControler\InitStructure.h"

/*
extern "C"
{
  #include <wtypes.h>
}
*/


// -----------------------------------------------------------------------------
// Scalar types: double, enum, int, void *
// -----------------------------------------------------------------------------

# ifndef FALSE
#  define FALSE 0
# endif // FALSE

# ifndef TRUE
#  define TRUE 1
# endif // TRUE

typedef enum
{
  NO_NOTIFY = 0,
  ALARM,
  WARN
} ErrNotify ;

typedef enum
{
  NO_ACTION = 0,
  ABORT_BBK,
  ABORT_ANY
} ErrAction ;

typedef enum
{
	UNBOUND             = 0,
	BB1BOOL             = 1,
  BB1DOUBLE           = 2,
  BB1LONG             = 3,
  BB1STRING           = 4,
  BB1PATPATHO         = 5,
  BB1PATPATHOITER     = 6,
  EXTPATPATHO         = 7,
  OB1QEST             = 8,
  OB1ANSWERSTATUS     = 9,
  BB1POINTER          = 10,
  BB1OBJ              = 11,
  BB1INT              = 12,
  BB1NAUTEVENT        = 13,
  BB1HANDLES          = 14,
  BB1SEARCHSTRUCT     = 15,

  #ifdef __OB1__UNOPTIM__
  BB1LONG             = 16,
  BB1FLOAT            = 17,
  BB1PATHS            = 18,
  BB1CHAR             = 19,
  BB1SHORT            = 20,
  BB1UCHAR            = 21,
  BB1UINT             = 22,
  BB1ULONG            = 23,
  BB1USHORT           = 24,
  ENDATTTYPE          = 25
  #else
  ENDATTTYPE          = 16
  #endif

} AttType ;

// -----------------------------------------------------------------------------
// Have procedures return an Errcode rather than be void.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Note: Should there be one global table of Errcodes, or should each function
// have its own table?
// -----------------------------------------------------------------------------

typedef enum
{
  SUCCESS = 0,
  NOTHINGFOUND,
  BADPHASE,
  INVALIDATTRIBUTE,
  INVALIDLINK,
  LINKEXISTS,
  NOCLASSDELETION,
  OBJECTEXISTS,
  PROTECTEDKB,
  BADPRECONDITIONS,
  INVALIDFUNCTIONARGUMENT,
  FAILURE
} Errcode ;

typedef enum
{
  INITIALIZATION,
  CYCLE_0,
  INTERPRETER,
  CHECK_MESSAGES,
  TRIGGERING,
	AGENDA_MANAGER,
	KSARS_MANAGMENT,			// add fab - KSAR garbage collector
  SCHEDULER,
  BETWEEN_CYCLES,
  TERMCOND,
  POST_TERMINATION
} PhaseT ;

// -----------------------------------------------------------------------------
// A BB1Event has a unique EventType, corresponding to the BB action.
// -----------------------------------------------------------------------------

typedef enum
{
	ADD,
	MODIFY,
	MODIFYCP,
	EXTERNAL,
  FOCUS,
  ACTIVATE,
  NOTHING //ADDED by tabun
} EventType ;

// -----------------------------------------------------------------------------
// A BB1Strategy has a unique StabilityType
// -----------------------------------------------------------------------------

typedef enum
{
	TYPE_STABLE,
	TYPE_DYNAMIC
} StabilityType ;

typedef enum
{
	HIERARCHICAL
} StrategyType ;

// -----------------------------------------------------------------------------
// A BB1KSAR has a unique KSARStatus
// -----------------------------------------------------------------------------

typedef enum
{
  TRIGGERED,
  EXECUTABLE,
  EXECUTED,
	OBVIATED,
} KSARStatus ;

// -----------------------------------------------------------------------------
// A BB1KSAR has now already a NautilusStatus (NSStatus)
// -----------------------------------------------------------------------------

typedef enum
{
	NONE = 0,
	WAITUSERAUTH,
	WAITFORASKINGUSER,
	YESFORCED,
	REFUS,
	ASKUSER,
	STOP
} NautilusStatus ;

//extern const NautilusStatus										noNSStatus ;
//extern const NautilusStatus										checkNSStatus ;

// -----------------------------------------------------------------------------
// A BB1KSAR has a unique SubAgenda, whose name is identical to that of its KSARStatus
// -----------------------------------------------------------------------------

typedef KSARStatus														SubAgenda ;

// -----------------------------------------------------------------------------
// A trigger event has a unique change type
// Actually, a ChangeType can't be EXTERNAL, but this is an insignificant
// exception
// -----------------------------------------------------------------------------

typedef EventType															ChangeType ;

// -----------------------------------------------------------------------------
// The execution cycle.  Objects created before cycle 0 have cycle -1.
// -----------------------------------------------------------------------------

typedef ULONGLONG 																	Cycle ;
// typedef PRUint64 																	Cycle ;

// -----------------------------------------------------------------------------
// The rating of a KSAR, a float in [0.0...100.0]
// -----------------------------------------------------------------------------

typedef float 																Rating ;

// -----------------------------------------------------------------------------
// The weighting of a Focus, a float in [0.0...100.0]
// -----------------------------------------------------------------------------

typedef float 																WeightT ;

// -----------------------------------------------------------------------------
// A Priority is the summed ratings of all BB1Focus objects
// -----------------------------------------------------------------------------

typedef double 																PriorityT ;

// -----------------------------------------------------------------------------
// A void pointer is needed to allow attributes to have arbitrary values
// -----------------------------------------------------------------------------

typedef void 																	*Voidptr ;

// The value of cycleCreated for objects created before cycle 0
extern const Cycle             								PRECYCLE ;

// -----------------------------------------------------------------------------
// Function types, for pointers passed as arguments

// -----------------------------------------------------------------------------
// An ErrHandler is a pointer to a function of no arguments, other than the
// blackboard itself, with a void return value.
// -----------------------------------------------------------------------------

typedef void         ErrHandlerFn(const BB1BB&, const std::string& method, const std::string& message) ;
typedef ErrHandlerFn *ErrHandler ;

// -----------------------------------------------------------------------------
// An ArglessVoidFnPtr is a pointer to a function of no arguments, other than
// the blackboard itself, that returns an Errcode. KS actions, after-termination
// functions, and Strategy ?s are of this type.
// -----------------------------------------------------------------------------

/*
** Init function pointer create by KS
*/
typedef  KsInitStructure*                     PublicationFunction(void);
typedef  PublicationFunction                  *PublicationFunctionPtr;
typedef  ValidityContextType                  ValidityF(const BB1BB& ,Transfert* );
typedef  ValidityF                            *ValidityPtr;

typedef Errcode																ArglessVoidFn(BB1BB&) ;
typedef ArglessVoidFn													*ArglessVoidFnPtr ;
//typedef ArglessVoidFnPtr											ActionT ;
typedef ArglessVoidFnPtr											InitFunctionT ;

#ifndef __TCONTOLER__
typedef Errcode																ActionFunction(BB1BB&, Transfert*, bool isOpportuniste = true );
#else
typedef Errcode																ActionFunction(BB1BB&, BB1KSAR&) ;
#endif

typedef ActionFunction												*ActionFunctionPtr ;
typedef ActionFunctionPtr											ActionT ;

// -----------------------------------------------------------------------------
// An EventPredPtr is a pointer to a predicate (a function that returns a
// boolean int) with a BB1Event argument.  KS trigger conditions are of this
// type.
// -----------------------------------------------------------------------------

// original
typedef bool																	EventPred(const BB1Event&) ;
typedef EventPred															*EventPredPtr ;
typedef EventPredPtr													TriggerConditionT ;

// modified
/*
typedef bool																	EventPred(const BB1Event&, NautilusStatus) ;
typedef EventPred															*EventPredPtr ;
typedef EventPredPtr													TriggerConditionT ;
*/

// -----------------------------------------------------------------------------
// An ArglessPredPtr is a pointer to a predicate (a function that returns a
// boolean int) of zero arguments. Decision (Stragegy and Focus) goals, Focus
// criteria, KS preconditions and obviation conditions, and termination
// conditions are of this type.
// -----------------------------------------------------------------------------

typedef bool																	ArglessPred(const BB1BB&) ;
typedef ArglessPred														*ArglessPredPtr ;

typedef ArglessPredPtr												GoalT ;
//typedef ArglessPredPtr												PreconditionT ;
//typedef ArglessPredPtr												ObviationConditionT ;
typedef ArglessPredPtr												TerminationConditionT ;

typedef	ArglessPredPtr												APConditionT ;

typedef PublicationFunctionPtr                PublicationT;
typedef ValidityPtr                           ValidityT;

typedef bool																	EvalCondition(const BB1BB&, Transfert*) ;

typedef EvalCondition													*EvalConditionPtr ;

typedef EvalConditionPtr											PreconditionT ;
typedef EvalConditionPtr											ObviationConditionT ;

// -----------------------------------------------------------------------------
// A RatingFunction is a function that maps a BB1KSAR onto an int. It is used
// only to build the Criterion
// -----------------------------------------------------------------------------

typedef Rating																RatingFunction(const BB1KSAR&) ;
typedef RatingFunction												*CriterionT ;

// -----------------------------------------------------------------------------
// A StrategicGeneratorPtr is a pointer to a function which returns a reference
// to a BB1Strategy.
// A strategy's strategic-generator and remaining-strategic-generator are of
// this type.
// ---------------------------------------------------------------------

extern const ActionT               						NULLAC ;
extern const GoalT                 						NULLGOAL;
extern const ValidityT                        NULLVALI;
extern const InitFunctionT         						NULLINITFN ;
extern const ObviationConditionT   						NULLOC ;
extern const PreconditionT         						NULLPC ;
extern const TerminationConditionT 						NULLTERMCOND ;
extern const TriggerConditionT     						NULLTC ;
extern const PublicationT                     NULLPUB ;

// -----------------------------------------------------------------------------
// List types
// -----------------------------------------------------------------------------

typedef	BB1List<BB1Object *>									ListOfBB1Object ;
typedef BB1List<std::string>									ListOfString ;

typedef BB1Set<std::string *>									PathsVector ;

// -----------------------------------------------------------------------------
// Set types
// -----------------------------------------------------------------------------

typedef BB1Set<AttValPair *> 									SetOfAVP ;

typedef BB1Set<BB1KB *>												SetOfBB1KB ;
typedef BB1Set<BB1KS *>												SetOfBB1KS ;
typedef BB1Set<BB1Link *>											SetOfBB1Link ;
typedef BB1Set<BB1LinkPair *>									SetOfBB1LinkPair ;
typedef BB1Set<BB1Object *>										SetOfBB1Object ;

typedef BB1Set<AKS *>													SetOfAKS ;

// -----------------------------------------------------------------------------
// const iterator types
// -----------------------------------------------------------------------------

typedef vector<std::string>::const_iterator		StrCIter ;
typedef	vector<AttValPair *>::const_iterator	AVPCIter ;
typedef vector<BB1KB *>::const_iterator				KBCIter ;
typedef vector<BB1KS *>::const_iterator				KSCIter ;
typedef vector<BB1Link *>::const_iterator			LinkCIter ;
typedef vector<BB1LinkPair *>::const_iterator	LinkPairCIter ;
typedef vector<BB1Object *>::const_iterator		ObjectCIter ;

typedef vector<AKS *>::const_iterator					AKSCIter ;

typedef vector<string *>::const_iterator			PathsCIter ;

// -----------------------------------------------------------------------------
// iterator types
// -----------------------------------------------------------------------------

typedef vector<string>::iterator							StrIter ;
typedef	vector<AttValPair *>::iterator				AVPIter ;
typedef vector<BB1KB *>::iterator							KBIter ;
typedef vector<BB1KS *>::iterator							KSIter ;
typedef vector<BB1Link *>::iterator						LinkIter ;
typedef vector<BB1LinkPair *>::iterator				LinkPairIter ;
typedef vector<BB1Object *>::iterator					ObjectIter ;

typedef vector<AKS *>::iterator								AKSIter ;

typedef vector<string *>::iterator						PathsIterator ;

// -----------------------------------------------------------------------------
// List classes
// -----------------------------------------------------------------------------

class ObjectList : public ListOfBB1Object
{
  public:

    ObjectList() ;
    ObjectList(const ObjectList& src) ;
    ~ObjectList() ;

    void					vider() ;

    ObjectList&		operator=(const ObjectList& src) ;

    static long getNbInstance()  { return _lObjectListCount ; }
    static void initNbInstance() { _lObjectListCount = 0 ; }

  private:

    static long _lObjectListCount ;
} ;

class StringList : public ListOfString
{
  public:

    StringList() ;
    StringList(const StringList& src) ;
    ~StringList();

    void					vider() ;

    StringList&		operator=(const StringList& src) ;

    static long getNbInstance()  { return _lStringListCount ; }
    static void initNbInstance() { _lStringListCount = 0 ; }

  private:

    static long _lStringListCount ;
} ;

class _BBKEXPORT PathsList : public PathsVector
{
  public:

    PathsList() ;
    PathsList(PathsList& src) ;
    ~PathsList() ;

    static long getNbInstance()  { return _lPathsListCount ; }
    static void initNbInstance() { _lPathsListCount = 0 ; }

  private:

    static long _lPathsListCount ;

//	PathsList&	operator=(PathsList& src) ;
//	void				vider() ;
} ;

// -----------------------------------------------------------------------------
// Set classes
// -----------------------------------------------------------------------------
class _BBKEXPORT AVPSet : public SetOfAVP
{
  public:

    AVPSet() ;
    AVPSet(const AVPSet& src) ;
    ~AVPSet() ;

    AVPSet& operator=(const AVPSet& src) ;
    AVPSet& operator|=(const AVPSet& src) ;

    static long getNbInstance()  { return _lAVPSetCount ; }
    static void initNbInstance() { _lAVPSetCount = 0 ; }

  private:

    static long _lAVPSetCount ;
} ;

class KBSet : public SetOfBB1KB
{
  public:

	  KBSet() ;
	  KBSet(const KBSet& src) ;
	  ~KBSet() ;

    static long getNbInstance()  { return _lKbSetCount ; }
    static void initNbInstance() { _lKbSetCount = 0 ; }

  private:

    static long _lKbSetCount ;

//	KBSet&				operator=(KBSet& src) ;
} ;

class KSSet : public SetOfBB1KS
{
  public:

    KSSet() ;
    KSSet(const KSSet& src) ;
    ~KSSet() ;

    static long getNbInstance()  { return _lKsSetCount ; }
    static void initNbInstance() { _lKsSetCount = 0 ; }

  private:

    static long _lKsSetCount ;

//	KSSet&				operator=(KSSet& src) ;
} ;

class AKSSet : public SetOfAKS
{
  public:

    AKSSet() ;
    AKSSet(const AKSSet& src) ;
    ~AKSSet() ;

    static long getNbInstance()  { return _lAKSSetCount ; }
    static void initNbInstance() { _lAKSSetCount = 0 ; }

  private:

    static long _lAKSSetCount ;

//	AKSSet&				operator=(AKSSet& src) ;
} ;

class LinkSet : public SetOfBB1Link
{
  public:

    LinkSet() ;
    LinkSet(const LinkSet& src) ;
    ~LinkSet() ;

    LinkSet& operator=(const LinkSet& src) ;
    LinkSet& operator|=(const LinkSet& src) ;

    static long getNbInstance()  { return _lLinkSetCount ; }
    static void initNbInstance() { _lLinkSetCount = 0 ; }

  private:

    static long _lLinkSetCount ;
} ;

class LinkPairSet : public SetOfBB1LinkPair
{
  public:

    LinkPairSet() ;
	  LinkPairSet(const LinkPairSet& src) ;
	  ~LinkPairSet() ;

	  LinkPairSet& operator=(const LinkPairSet& src) ;

    static long getNbInstance()  { return _lLinkPairSetCount ; }
    static void initNbInstance() { _lLinkPairSetCount = 0 ; }

  private:

    static long _lLinkPairSetCount ;
} ;

class ObjectSet : public SetOfBB1Object
{
  public:

    ObjectSet() ;
    ObjectSet(const ObjectSet& src) ;
//	ObjectSet(const ObjectList& src) ;
    ~ObjectSet() ;

//	void					vider() ;

    ObjectSet&		operator=(const ObjectSet& src) ;

    static long getNbInstance()  { return _lObjectSetCount ; }
    static void initNbInstance() { _lObjectSetCount = 0 ; }

  private:

    static long _lObjectSetCount ;
} ;

// -----------------------------------------------------------------------------
// Context generator types
// -----------------------------------------------------------------------------
// A context is a list of pointers to objects, to be bound to KSAR variables.
// -----------------------------------------------------------------------------

typedef	ObjectList														Context ;
typedef BB1Set<Context *>											SetOfContext;
typedef vector<Context *>::const_iterator			ContextCIter ;
typedef vector<Context *>::iterator						ContextIter ;

class ContextSet : public SetOfContext
{
  public :

    ContextSet() ;
    ContextSet(const ContextSet& src) ;
    ~ContextSet() ;

    static long getNbInstance()  { return _lContextSetCount ; }
    static void initNbInstance() { _lContextSetCount = 0 ; }

  private:

    static long _lContextSetCount ;

//	ContextSet&		operator=(ContextSet& src) ;
} ;


// -----------------------------------------------------------------------------
// An AVPGenPtr is a pointer to function that changes two reference parameters:
// to a KSARVar and to a SetOfContext.
// Context generators are of this type.
// -----------------------------------------------------------------------------

typedef void																	AVPGenerator(const BB1BB&, StringList *, ContextSet *) ;
typedef AVPGenerator													*AvpGenPtr ;
typedef AvpGenPtr															ContextGeneratorT ;

extern const ContextGeneratorT								NULLCONTEXTGENERATOR ;

// -----------------------------------------------------------------------------
// NULL variable
// -----------------------------------------------------------------------------

extern 	int																		NULLINT ;
extern 	long																	NULLLONG ;
extern 	const string													NULLSTRING ;
extern 	Voidptr																NULLPTR ;
extern 	NSPatPathoArray												*NULLPATHO ;
extern  PatPathoIter                          *NULLPATPATHOITER ;
extern 	PathsList															*NULLPATHS ;
extern 	BB1Object															*NULLOBJECT ;
extern 	ObjectList														*NULLLISTOBJ ;
extern  NautilusQuestion                      *NULLLNAUTQUEST ;
extern  AnswerStatus                          *NULLLANSWERSTATUS ;
extern  NautilusEvent                         *NULLNAUTILUSEVENT ;
extern  vector<HANDLE>                        *NULLHANDLES ;
extern  NSSearchStruct                        *NULLSEARCHSTRUCT ;

extern	int																		DebugObjectCPT ;

// NSBBQuestion & NSBBAnswer

#endif // __BB1TYPES_H__

