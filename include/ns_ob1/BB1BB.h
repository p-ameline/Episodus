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
// -----------------------------------------------------------------------------
// The blackboard is a set of objects.
// -----------------------------------------------------------------------------
// In addition, this class represents all state, so that there can be multiple
// BB1s. All utility functions that have no other class should be members of
// this class.
// -----------------------------------------------------------------------------

#ifndef _BB1BB_h
#define _BB1BB_h 1

class BB1AppInst ;
class BB1Class ;
class BB1KB ;
class BB1KS ;
class BB1Link ;
class BB1Message ;
class BB1Object ;
class OB1Token ;

class OB1SubStrategy ;
class NSKS ;
class OB1NKS ;

// Defini pour la fonction DefineGeneralizeKS
class FctFunctorValidity ;
class FunctorValidity ;
class FunctorAction ;
class FunctorObviation ;
class FunctorPrecondition ;
class FunctorPublication ;

class NSConcernToDoInformation ;

//#define __DEBUG_ 1		// for debug

#include <stdlib.h>		// for abort();

//#ifdef __GNUC__
#include <values.h>		// for BB1BB::GoNCycles(...)
//#endif

#include "ns_ob1\BB1types.h"
// #include "ns_ob1\NSKS.h"
#include "ns_ob1\OB1types.h"
#include "ns_ob1\TypedVal.h"
#include "ns_ob1\OB1Controler.h"

#include "docking\stripeDock.h"

#ifdef _MSC_VER
#include <limits.h>
#define MAXINT INT_MAX
#endif

// #include "ns_ob1\OB1Controler.h"
// #include "nautilus\nssuper.h"

const int			BS					= 100 ;	// avoid loading stdio.h
extern const	StringList	NULLSTRINGLIST ;
extern const	AVPSet			NULLAVPSET ;
extern const	LinkSet			NULLLINKSET ;

const string		EventPrefix("Event-") ;
const string		KSARPrefix("KSAR-") ;
const string		MessagePrefix("Message-") ;

/**
* \brief The blackboard
*/
/**
* The blackboard conains the KS representation, the shared memory and the controler  <BR>
* Le blackboard contient la représentation des KSs, la mémoire et le controler
*/
class _BBKEXPORT BB1BB : public NSRoot
{
	friend class				BB1Link ;
  friend class        OB1Controler;
	friend class			 BB1Object ;
	friend char					**EventNames(BB1BB& bb, int& i) ;
	friend void					MenuCallbackInternals(const char *const label, BB1BB& bb) ;

	friend BB1AppInst		*MakeAppInst(	BB1Class&			    c,
																		const std::string	sName,
                                    AttValPair&       dispatch,
																		BB1KB&				    kb,
																		/*const*/ AVPSet	*pAttSpec,
																		const LinkSet	    *pLinkSpec,
                                    bool              bCreateEvent,
                                    bool              bCreatStrat
                                    ) ;

  friend Errcode			ModifyObject(	BB1Object&		object,
																	const AVPSet		*attSpec,
																	const LinkSet		*linkSpec,
																	const AVPSet		*delAttSpec,
																	const LinkSet		*delLinkSpec,
																	const int				eventP,
                                  bool  creat) ;

	// Allowed to call ClassMissing()
	friend class				BB1KB ;

  public: // API.  See "BB1API.c".

    BB1BB(NSContexte									*pCtx,
          const InitFunctionT					initFn 					= NULLINITFN,
          const TerminationConditionT	terminationCond	= NULLTERMCOND,
          int													argc 						= 0,
          char												**argv 					= (char**)0) ;

    BB1BB(const BB1BB& bb) ;
    ~BB1BB() ;

    bool reinit() ;

    // ---------------------------------------------------------------------------
    // Knowledge base operations
    // ---------------------------------------------------------------------------
    BB1Class* DefineClass(const string      sName,
                          AttValPair&       dispatch,
                          const StringList* pDirectSuperclasses = (StringList*) 0,
                          /*const*/ AVPSet* pAllowedAttributes = (AVPSet*) 0) ;

    BB1KB*    DefineKB(const string& name) ;

    BB1KS*    DefineKS(const string&           name,
                       BB1KB&                  kb,
                       const PublicationT      tpub             = NULLPUB,
                       const ValidityT         tval             = NULLVALI,
                       const PreconditionT     pCond            = NULLPC,
                       const ActionT           action           = NULLAC,
                       /*const*/ AVPSet        *attSpec         = NULL,
                       const LinkSet           *linkSpec        = NULL,
                       const ContextGeneratorT contextGenerator = NULLCONTEXTGENERATOR) ;

    //
    // Fonction permet de définir des KS dans le Blackboard
    // Definir un KS dans le Blackboard sert à le réfencer pour utilisation
    // Le réferencement généraliser permet de se connecter à des Méthodes de classes et à des fonctions
    BB1KS*    DefineGeneralizedKS(const std::string&         name,
                                  BB1KB&                     kb,
                                  const FunctorPublication*  tpub,
                                  const FunctorValidity*     tval,
                                  const FunctorPrecondition* pCond,
                                  const FunctorAction*       action,
                                  const FunctorObviation*    oBViated,
                                  AVPSet*                    attSpec ) ;

    Errcode		DefineRelation(const string& ForwardRelation,
                             const string& InverseRelation,
                             const string& FromClass,
                             const string& ToClass) ;

    // Is the system on the first execution cycle?
    inline
    bool      FirstCycleP() const;
    inline
    const KBSet	*AllKBs() const	{ return &_kbs ; }
    const BB1KS	*KS()     const ;
    PhaseT			 Phase()  const	{ return _BB1BBphase ; }

    // Return the current execution cycle
    inline
    Cycle				 ThisCycle() const { return _cycle ; }

    // BB1String-to-object mapping
    BB1Class*  ClassNamed(const string& c) const ;
    BB1Event*  EventNamed(const string& eventName) const ;
    BB1KB*     KBNamed(const string& KBName) const ;

    BB1Object* ObjectNamed(const string& LongName) const ;
    BB1Object* ObjectNamed(const string& KBname, const string& ObjName) const ;

    // Return the error notification level
    ErrNotify	 ErrorNotify() const  { return _errorNotify ; }

    // Return a pointer to the error handler function
    ErrHandler ErrorHandler() const { return _errorHandler ; }

    // Return the error action option
    ErrAction	 ErrorAction() const  { return _errorAction ; }

    // Establish what is to be done when an error condition arises
    void			 SetErrorAction(const ErrAction a)   { _errorAction = a ; }

    // Establish what function is to be called by the exception handler
    void			 SetErrorHandler(const ErrHandler h) { _errorHandler = h ; }

    // Establish what is to be written to cerr whan an error condition arises
    void			 SetErrorNotify(const ErrNotify n)   { _errorNotify = n ; }

    Errcode    SetTraceCycle(const int traceP) ;

    // The body uses global variables; isolate them in KBLangFns.c
    void       LoadKB(const string& KBPathName)    { LoadKBAux(KBPathName) ; }

    void       dispatchLogMessage(string sMessage, int iAlertLevel, string sBefore = "", string sAfter = "") ;

    void       RunInOpportunisticMode(int nb_tour) ;   // give a number process turn
    BB1Object* find(TypedVal& path, std::string clas, TypedVal *pSearchParams, bool depp = false) ; //find a question on blackboard
    BB1Object* askQuestion(std::string& sQuestion, bool createToken = true) ;
    bool       PutInformation(BB1Object* respp) ;
    bool       Execute() ; /* Execute bb during one cycle */
    bool       giveAnswerToQuestion(TypedVal* pPathVal, NSPatPathoArray* pAnswerPatho, string sAnswerDate, bool bExt = false, HWND originatingInterfaceHandle = (HWND) 0, TypedVal *pSearchParams = (AttValPair *) 0) ;
    void       alertClientsForAnswerUpdate(TypedVal* pPathVal, HWND originatingInterfaceHandle, TypedVal *pSearchParams = (AttValPair *) 0) ;
    void       challengeQuestionsWhenPatPathoSaved(const NSPatPathoArray* pPPT) ;

    void       addToSubStratGarbage(OB1SubStrategy* pSubStrat) ;

    BB1Object* searchInKB(TypedVal& question, std::string& kbName, TypedVal *pSearchParams = (TypedVal *) 0) ;
    //
    // Search helpers
    //
    BB1Object* searchQuestionInKB(TypedVal& question, TypedVal *pSearchParams) { return searchInKB(question, std::string("QuestionKB"), pSearchParams) ; }
    BB1Object* searchAnswerInKB(TypedVal& question, TypedVal *pSearchParams)   { return searchInKB(question, std::string("InformationKB"), pSearchParams) ; }
    BB1Object* searchEventInKB(TypedVal& question)                             { return searchInKB(question, std::string("NautilusEventKB")) ; }

    BB1Object* searchAnswerForQuestion(BB1Object* pQuestion) ;

    //
    //
    ObjectSet* getObjectsInKb(string sKbName) ;
    ObjectSet* getQuestions() { return getObjectsInKb("QuestionKB") ; }
    ObjectSet* getAnswers()   { return getObjectsInKb("InformationKB") ; }
    ObjectSet* getEvents()    { return getObjectsInKb("NautilusEventKB") ; }

    void       createConcern(NSConcernToDoInformation *pConcernInfo) ;
    void       sendMessage(string sText, string sUrl, NsStripe::STRIPETYPE iStripeType) ;

    void       suscribeHandleToQuestion(BB1Object* pQuestion, HWND interfaceHandle, bool bWithCounter = false) ;
    bool       unsuscribeHandleToQuestion(BB1Object* pQuestion, HWND interfaceHandle, bool bWithCounter = false) ;
    void       unsuscribeHandle(HWND interfaceHandle) ;

    void       suscribeTokenToQuestion(BB1Object* pQuestion, OB1Token* pToken) ;
    bool       unsuscribeTokenToQuestion(BB1Object* pQuestion, OB1Token* pToken) ;
    void       unsuscribeToken(OB1Token* pToken) ;

    void       suscribePersistenceToQuestion(BB1Object* pQuestion, long int iPersistenceHandle) ;
    bool       unsuscribePersistenceToQuestion(BB1Object* pQuestion, long int iPersistenceHandle) ;
    void       unsuscribePersistence(long int iPersistenceHandle) ;

    void       deleteOrphanQuestion(BB1Object* pQuestion) ;

  protected: // See "BB1BBB.c".

    string     GenerateEventName() ;
    string     GenerateMessageName() ;

    bool       CheckAnswers(BB1Object *obj) ;
    bool       CheckDPIODialog() ;
    bool       evalDPIODialog() ;

    bool       TerminateP(TerminationConditionT tc)
    {
      PhaseT	savePhase = Phase() ; SetPhase(TERMCOND) ;
      bool	retval = (tc ? ((*tc)(*this)) : false) ;

      SetPhase(savePhase) ;
      return (retval) ;
    }

    // ---------------------------------------------------------------------------
    // Agenda management
    // ---------------------------------------------------------------------------

    // Phase checking
    void       SetPhase(PhaseT phase) { _BB1BBphase = phase ; }

    bool       CreatePhaseCheck(const string &method) const ;

    int        ClearPhaseCheck() const ;
    bool       GoNCyclesPhaseCheck() const ;
    bool       KSPhaseCheck() const ;
    bool       KSARPhaseCheck() const ;

   // bool													TracePhaseCheck() const ;

    bool       CheckAllowedAttributes(const AVPSet *allowedAttributes) ;
    bool       CheckDirectSuperclasses(const StringList *dscNamed, ObjectSet *dscObjects) ;

    Errcode    ClearLinkPairs() ;

  //	Errcode												ClearKSs() 														{ kss->clear(); return (SUCCESS) ; }

   // #ifdef __TCONTROLER__
    int        BB1BB::CreateToken(EventType type, BB1Object& object,
                                  const AVPSet *attSpec = NULL,
                                  const LinkSet *linkSpec = NULL,
                                  bool createStrat = false) ;
    /*#else
    BB1Event											*CreateEvent(	EventType			type,
                                                BB1Object&		object,
                                                const AVPSet	*changedAttributes = NULL,
                                                const LinkSet	*changedLinks = NULL) ;
    #endif  */

    void       DoOneContext(ObjectList	*olist, StringList *variables, BB1KS& ks, BB1Event& te) ;
    Errcode		 ExecuteInitFunction() ;

    Errcode		 GoOneCycle() ;

    // Advance the cycle count to the next cycle
    void       IncrementCycle() { _cycle < MAXLONG ? _cycle++ : _cycle = 0 ; }

    InitFunctionT	InitFunction() const { return _initFunction ; }

    BB1AppInst* NewAppInst(const string 		 sObjName,
                           AttValPair&       dispatch,
                           BB1KB&					   kb,
                           BB1Class&         c,
                           /*const*/ AVPSet* pAttSpec,
                           const LinkSet*    pLinkSpec,
                           const int				 eventP,
                           bool              bCreateTok   = true,
                           bool              bCreateStrat = false) ;

    const string OppositeLinkName(const string sName, const BB1Class* pC) const ;
    Errcode			 ClearKBs() ;
    bool				 ReservedLinkP(const string sLinkName) const ;


    const	TerminationConditionT	TerminationCondition() const { return _terminationCondition ; }


    // Are trace messages enabled for the execution cycle?
    int           TraceCycleP() const { return _traceCycle ; }

    // Are trace messages enabled for event creation?
    int           TraceEventP() const { return _traceEvent ; }

    // Are trace messages enabled for KSAR creation?
    int           TraceKSARP() const 	{ return _traceKSAR ; }

    // Are trace messages enabled for message creation?
    int           TraceMessageP() const { return _traceMessage ; }

    void					LoadKBAux(const string& KBPathName) ;

    // ---------------------------------------------------------------------------
    // Reclaim memory
    // ---------------------------------------------------------------------------
    void					BB1BB_Intern(const string& n, void *p, AVPSet& avpset) ;
    void*         BB1BB_Lookup(const string& name, const AVPSet& avpset) ;

  public:

    vector<NSKS*> _nskss ;
    // -----------------------------------------------------------------------------
    // DPIO add
    AKSSet        _Akss ;
    // -----------------------------------------------------------------------------

  public:

    long int               getNBObject() { return (_nbObject) ; }
    void                   incNBObject() { _nbObject++ ; }
    inline
    OB1Controler*          Controler() ;
    std::vector<OB1NKS* >* getKSOfType(KSType typ) ;
    void                   getKSOfKB(std::vector<OB1NKS*>* ks, std::string skbName) ;
    void                   getValidKs(std::vector<OB1NKS*>* temp, ValidityContextType type) ;
    vector<NSKS*>*         getNsKss() { return &_nskss ; }

    void                   testOtherNodes() ;

  protected:

    OB1Controler                _kenoby ;    // Controler
    long int                    _nbObject ;  //  numbers of objects in the BB

    const InitFunctionT         _initFunction ;					// function called on cycle 0
    const TerminationConditionT _terminationCondition ;	// termination predicate
   //	int														socket ;								// socket for external messages
    Cycle												_cycle ;									// iteration of recognize-act cycle
    BB1KS												*_ks ;										// during triggering, current KS
    BB1Event										*_triggerEvent ;					// the current event
    PhaseT											_BB1BBphase ;						// internal state; to check validity of API actions
    int													_eventCount ;						// event enumerator for unique names

    int													_messageCount ;					// message enumerator for unique names
    //	KSSet										*kss ;									// cached set of all KSs

    KBSet												_kbs ;									// cached set of all KBs
    int													_traceCycle ;						// trace each cycle?
    int													_traceEvent ;						// trace event creation?
    int													_traceKSAR ;							// trace KSAR creation?
    int													_traceMessage ;					// trace external messages?

    LinkPairSet									_linkPairs ;						// set of all declared link pairs
    BB1KB												*_classKB ;							// cached pointer to Class KB

    ErrAction										_errorAction ;						// error action
    ErrNotify										_errorNotify ;						// error notification level
    ErrHandler									_errorHandler ;					// error handler function
    std::ostream&								_trout ;									// trace output

    int													_cycles2add ;

    NTArray<OB1SubStrategy>     _subStratGarbagge ;
} ;

// -----------------------------------------------------------------------------
// The following are non-member utility functions
// -----------------------------------------------------------------------------

inline bool   Not(const int b) { return (b ? false : true) ; }

inline bool   BB1BB::FirstCycleP() const { return ((ThisCycle() == 0) ? true : false) ; }

inline OB1Controler* BB1BB::Controler() { return &_kenoby ; }

// -----------------------------------------------------------------------------
// Generate an enumerated name
// -----------------------------------------------------------------------------

string			  GenerateName(const string& prefix, const int count) ;

// -----------------------------------------------------------------------------
// construct a set
// -----------------------------------------------------------------------------

AVPSet							* _BBKEXPORT Collect(	AttValPair* a1,                    AttValPair* a2  = (AttValPair*) 0,
																					AttValPair* a3  = (AttValPair*) 0, AttValPair* a4  = (AttValPair*) 0,
																					AttValPair* a5  = (AttValPair*) 0, AttValPair* a6  = (AttValPair*) 0,
																					AttValPair* a7  = (AttValPair*) 0, AttValPair* a8  = (AttValPair*) 0,
																					AttValPair* a9  = (AttValPair*) 0, AttValPair* a10 = (AttValPair*) 0,
																					AttValPair* a11 = (AttValPair*) 0, AttValPair* a12 = (AttValPair*) 0,
																					AttValPair* a13 = (AttValPair*) 0, AttValPair* a14 = (AttValPair*) 0,
																					AttValPair* a15 = (AttValPair*) 0, AttValPair* a16 = (AttValPair*) 0,
                                          AttValPair* a17 = (AttValPair*) 0, AttValPair* a18 = (AttValPair*) 0) ;

LinkSet							*Collect(	BB1Link* a1,                 BB1Link* a2  = (BB1Link*) 0,
															BB1Link* a3  = (BB1Link*) 0, BB1Link* a4  = (BB1Link*) 0,
															BB1Link* a5  = (BB1Link*) 0, BB1Link* a6  = (BB1Link*) 0,
															BB1Link* a7  = (BB1Link*) 0, BB1Link* a8  = (BB1Link*) 0,
															BB1Link* a9  = (BB1Link*) 0, BB1Link* a10 = (BB1Link*) 0,
															BB1Link* a11 = (BB1Link*) 0, BB1Link* a12 = (BB1Link*) 0,
															BB1Link* a13 = (BB1Link*) 0, BB1Link* a14 = (BB1Link*) 0,
															BB1Link* a15 = (BB1Link*) 0, BB1Link* a16 = (BB1Link*) 0,
                              BB1Link* a17 = (BB1Link*) 0, BB1Link* a18 = (BB1Link*) 0) ;


// -----------------------------------------------------------------------------
// contstruct a list
// -----------------------------------------------------------------------------

ObjectList					*List(BB1Object* a1,                   BB1Object* a2  = (BB1Object*) 0,
													BB1Object* a3  = (BB1Object*) 0, BB1Object* a4  = (BB1Object*) 0,
													BB1Object* a5  = (BB1Object*) 0, BB1Object* a6  = (BB1Object*) 0,
													BB1Object* a7  = (BB1Object*) 0, BB1Object* a8  = (BB1Object*) 0,
													BB1Object* a9  = (BB1Object*) 0, BB1Object* a10 = (BB1Object*) 0,
													BB1Object* a11 = (BB1Object*) 0, BB1Object* a12 = (BB1Object*) 0,
													BB1Object* a13 = (BB1Object*) 0, BB1Object* a14 = (BB1Object*) 0,
													BB1Object* a15 = (BB1Object*) 0, BB1Object* a16 = (BB1Object*) 0) ;

StringList					*List(string a1              , string a2  = NULLSTRING,
													string a3  = NULLSTRING, string a4  = NULLSTRING,
													string a5  = NULLSTRING, string a6  = NULLSTRING,
													string a7  = NULLSTRING, string a8  = NULLSTRING,
													string a9  = NULLSTRING, string a10 = NULLSTRING,
													string a11 = NULLSTRING, string a12 = NULLSTRING,
													string a13 = NULLSTRING, string a14 = NULLSTRING,
													string a15 = NULLSTRING, string a16 = NULLSTRING) ;

bool								Member(const string& s, const StringList *l) ;

Rating							NormalizeRating(const Rating rating) ;

string&							decToString(string& S, const int i) ;

// -----------------------------------------------------------------------------
// These 5 lookup functions are to be library defaults.  The application will
// have to override these defaults if it is to read KS definitions from a file.
// -----------------------------------------------------------------------------
 /*
ActionT							lookupAC(const string& ac) ;
ContextGeneratorT		lookupCG(const string& cg) ;
ObviationConditionT	lookupOC(const string& oc) ;
PreconditionT				lookupPC(const string& pc) ;
TriggerConditionT		lookupTC(const string& tc) ;
     */
#endif // _BB1BB_h

