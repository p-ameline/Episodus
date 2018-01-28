// -----------------------------------------------------------------------------
//
// BB1KSAR.h
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// A KSAR carries the information of a KS being triggered by an event.
// -----------------------------------------------------------------------------

#ifndef _BB1KSAR_h
# define _BB1KSAR_h 1

//class ostream ;  // for BB1KSAR::Dump(...)
class BB1BB ;
class BB1Event ;
class BB1KS ;

# ifdef __GNUC__
#  pragma interface
# endif

# include "AttValPair.h" // for BB1KSAR::Set(...)
# include "BB1Object.h"

#ifndef __TCONTROLER__

string& 						decToString(string& S, const int i) ;

class BB1KSAR : public BB1Object
{

	// ---------------------------------------------------------------------------
	// Allowed to call BB1KSAR(), Demote(), Execute(), Obviate(), Promote(), Rate()
	// ---------------------------------------------------------------------------

	friend class 			BB1BB ;
	friend class 			BB1Object ;

	friend class 			ObjectList ;
	friend class 			KSARSet ;
	friend class 			ObjectSet ;
	friend class 			BB1Agenda ;


 public: // API

	BB1KSAR(const string& oname, BB1KB& kb, BB1Class *c, const AVPSet *attSpec, const LinkSet *linkSpec, BB1Event& tE, const BB1KS& k, const Cycle tC) ;
	BB1KSAR(const BB1KSAR& src) ;
	~BB1KSAR() ;

	// Identity queries
	int								ControlKSARP() const ;

	// Return the triggered KS
	const BB1KS&			KS() const                                            { return (ks) ; } ;

	// Return the event which triggered the KS
	const BB1Event&		TriggerEvent() const                                  { return (triggerEvent) ; } ;

	// Return the object, the creation or modification of which engendered the trigger event
	BB1Object&				TriggerObject() const                                 { return (triggerObject) ; } ;

	// Return the KSAR's priority rating
	PriorityT					Priority() const                                      { return (priority) ; } ;

	// Return the KSAR's sub-agenda
	KSARStatus				Status() const                                        { return (status) ; } ;

	const string&			StatusString() const ;


	// ---------------------------------------------------------------------------
	// Is the KSAR on the {executable executed obviated triggered} sub-agenda?
	// ---------------------------------------------------------------------------

	bool							ExecutableP() const                                   { return (status == EXECUTABLE) ; } ;
	bool							ExecutedP() const																			{ return (status == EXECUTED) ; } ;
	bool							ObviatedP() const                                     { return (status == OBVIATED) ; } ;
	bool							TriggeredP() const                                    { return (status == TRIGGERED) ; } ;
//	bool							AskUserP() const																			{ return (status == ASKUSER ) ; } ;


	// ---------------------------------------------------------------------------
	// Cycle queries
	// Return the cycle upon which the KSAR was rendered executable
	// ---------------------------------------------------------------------------
	Cycle							ExecutableCycle() const                               { return (executableCycle) ; } ;


	// ---------------------------------------------------------------------------
	// Return the cycle upon which the KSAR was executed
	// ---------------------------------------------------------------------------
	Cycle							ExecutedCycle() const                                 { return (executedCycle) ; } ;

	// ---------------------------------------------------------------------------
	// Return the cycle upon which the KSAR was obviated
	// ---------------------------------------------------------------------------
	Cycle							ObviatedCycle() const                                 { return (obviatedCycle) ; } ;

	// ---------------------------------------------------------------------------
	// Return the cycle upon which the KSAR was triggered
	// ---------------------------------------------------------------------------
	Cycle							TriggeredCycle() const																{ return (triggeredCycle) ; } ;

//	Cycle							AskUserCycle() const																	{ return (askuserCycle) ; } ;


	Errcode						Set(const string& var, const char c)									{ AttValPair avp(var, c ) ; 					return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const double d)  							{ AttValPair avp(var, d ) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const float f)   							{ AttValPair avp(var, f ) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const int i)     							{ AttValPair avp(var, i ) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const long l)    							{ AttValPair avp(var, l ) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const short s)   							{ AttValPair avp(var, s ) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const string& s) 							{ AttValPair avp(var, s ) ; 					return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const unsigned char uc)  			{ AttValPair avp(var, uc) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const unsigned int ui)   			{ AttValPair avp(var, ui) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const unsigned long ul)  			{ AttValPair avp(var, ul) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const unsigned short us) 			{ AttValPair avp(var, us) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, const Voidptr p)         			{ AttValPair avp(var, p ) ;						return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, PathsList *paths)							{ AttValPair avp(var, paths) ;				return (SetAux(avp)) ; } ;
	Errcode						Set(const string& var, NSPatPathoArray *patho)				{ AttValPair avp(var, patho) ; 				return (SetAux(avp)) ; } ;
	Errcode						Set(const string &var, const BB1Object& o)      			{ AttValPair avp(var, Voidptr(&o)) ;	return (SetAux(avp)) ; } ;

	// ---------------------------------------------------------------------------
	// Does this KSAR have the named locally-bound variable?
	// This version is being kept for compatibility.
	// ---------------------------------------------------------------------------

	// Does this KSAR have the named locally-bound variable of the given type?
	bool							HasVariableP(const AttValPair& avp) const										{ return (HasAttributeP(avp)) ; } ;

	char							Variable(const string& var, char& c) const      						{ return (Value(var, c)) ; } ;
	double						Variable(const string& var, double& d) const    						{ return (Value(var, d)) ; } ;
	float							Variable(const string& var, float& f) const     						{ return (Value(var, f)) ; } ;
	int								Variable(const string& var, int& i) const       						{ return (Value(var, i)); };
	long							Variable(const string& var, long& l) const      						{ return (Value(var, l)) ; } ;
	short							Variable(const string& var, short& s) const     						{ return (Value(var, s)) ; } ;
	const string			Variable(const string& var, string& s) const    						{ return (Value(var, s)) ; } ;
	unsigned char			Variable(const string& var, unsigned char& uc) const				{ return (Value(var, uc)) ; } ;
	unsigned int			Variable(const string& var, unsigned int& ui) const   			{ return (Value(var, ui)) ; } ;
	unsigned long			Variable(const string& var, unsigned long& ul) const  			{ return (Value(var, ul)) ; } ;
	unsigned short		Variable(const string& var, unsigned short& us) const 			{ return (Value(var, us)) ; } ;
	Voidptr						Variable(const string& var, Voidptr& p) const         			{ return (Value(var, p )) ; } ;
	PathsList					*Variable(const string& var, PathsList *paths) const				{ return (Value(var, paths)) ; } ;
	NSPatPathoArray		*Variable(const string& var, NSPatPathoArray *patho) const	{ return (Value(var, patho)) ; };
	BB1Object&				Variable(const string& var) const                     			{ Voidptr p ; return (*((BB1Object *) Value(var, p))) ; } ;

	void 							Dump(std::ostream& os = cout) const ;

 protected:

	// ---------------------------------------------------------------------------
	// Is the object allowed to have the attribute? A KSAR object can take any
	// attribute. Overrides BB1Object.
	// ---------------------------------------------------------------------------

	bool							ValidAttributeP(const AttValPair& avp) const					{ return (true) ; } ;

 private:
	// Set the KSAR's priority
	Errcode						Rate(const PriorityT Priority)												{ priority = Priority ; return (SUCCESS) ; } ;


	// Change the status, without manipulating the agendas
	Errcode						Demote()
	{
		if (ExecutableP())
			{ status = TRIGGERED ; executableCycle = 0 ; return (SUCCESS) ; }
		else
			{ return (FAILURE) ; }
	} ;

	Errcode						Execute()
	{
		if (ExecutableP())
			{ status = EXECUTED ; executedCycle = ThisCycle() ; return (SUCCESS) ; }
		else
			{ return (FAILURE) ; }
	} ;

	Errcode						Obviate()
	{
		if (ExecutableP() || TriggeredP())
			{ status = OBVIATED ; obviatedCycle = ThisCycle() ; return (SUCCESS) ; }
		else
			{ return (FAILURE) ; }
	} ;

	Errcode						Promote()
	{
		if (TriggeredP())
			{ status = EXECUTABLE ; executableCycle = ThisCycle() ; return (SUCCESS) ; }
		else
			{ return (FAILURE) ; }
	} ;

	Cycle							ThisCycle() const ;

	// Auxiliary to Set -- the part that is in common
	Errcode						SetAux(AttValPair& avp)
	{
		AVPSet *varSpec = new AVPSet() ;
		varSpec->push_back(&avp) ;
		ProcessAttributes(varSpec) ;
		return (SUCCESS) ;
	}

	void							Save(ofstream& KBstream) { } ;	// KSARs can't be saved


 private:
	Cycle							executableCycle ;								// If executable or executed, cycle it became executable
	Cycle							executedCycle ;									// If executed, the cycle upon which executed
	Cycle							obviatedCycle ;									// If obviated, the cycle upon which obviated
	const Cycle				triggeredCycle ;								// The cycle upon which triggered
//	Cycle							askuserCycle ;									// the cycle in which the askuser function was invoked

	const BB1KS&			ks ;														// The KS which was triggered
	PriorityT					priority ;											// The pooled rating
	KSARStatus				status ;												// The sub-agenda {EXECUTABLE EXECUTED OBVIATED TRIGGERED}
	BB1Event&					triggerEvent ;									// The event which triggered the KS
	BB1Object&				triggerObject ;									// The event's object, cached
	AVPSet						*variables ;										// The KSAR's variable list

 public:
	NautilusStatus		nsStatus ;
} ;


typedef BB1KSAR			*KSARptr ;


std::ostream& 			operator<<(std::ostream& os, const BB1KSAR& obj) ;

#endif

#endif // _BB1KSAR_h

