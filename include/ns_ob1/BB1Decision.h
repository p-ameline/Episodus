/*
**
** BB1Decision.h
**
*/

/* A decision is simply that which is common to a strategy and a focus. */

#ifndef _BB1Decision_h
# define _BB1Decision_h 1

# ifdef __GNUC__
#  pragma interface
# endif // __GNUC__

# include "BB1Object.h" // base type


#ifndef _TCONTROLER__
class BB1Decision : public BB1Object
{

  // -------------------------------------------------------------------------
  //
  // public functions
  //
  // -------------------------------------------------------------------------

 public: // API

  const string&		Description() const			{ return (description) ; } ;
  Cycle			FirstCycle() const			{ return (CycleCreated()) ; } ;
  GoalT			Goal() const				{ return (goal) ; } ;
  bool			GoalTrueP() const			{ return (goal ? (*goal)(BB()) : false) ; }
  Cycle			LastCycle() const			{ return (lastCycle) ; } ;
  int			OperativeP() const			{ return (BB1DecisionOperativeP) ; } ;
  int			StableP() const				{ return (stableP) ; } ;
  WeightT		Weight() const				{ return (weight) ; } ;
  virtual Errcode	TurnOff(const Cycle cycle) ;
  virtual Errcode	TurnOn() ;
    /* Does this object trigger KS TerminatePrescription? Overrides BB1Object.h */
  bool			TerminatePrescriptionTCAux() const { return(true) ; } ;
  ObjectSet		*OperativeDecisions(ObjectSet *od) ;



  /*
  ** Does this object satisfy the precondition of KS TerminatePrescription?
  ** Overrides BB1Object
  */
  bool			TerminatePrescriptionPCAux() const { return (GoalTrueP()) ; } ;
  bool			TerminatePrescriptionOCAux() const { return (!AnySubDecisionOperativeP()) ; }

  /*
  ** Does this object obviate KS TerminatePrescription? Default; overrides BB1Object.
  ** Obviate the KSAR if none of its sub-decisions is operative.
  */
  Errcode		TerminatePrescriptionACAux() ;
  virtual bool		StrategyP() const = 0 ;
  virtual bool		FocusP() const = 0 ;


  // -------------------------------------------------------------------------
  //
  // protected functions
  //
  // -------------------------------------------------------------------------

 protected:

  BB1Decision(const string&	oname,
	      BB1KB&		kb,
	      BB1Class		*c,
              const AVPSet	*attSpec,
	      const LinkSet	*linkSpec,
              const GoalT	g,
	      int		OperativeP,
	      int		StableP,
	      WeightT		Weight,
              const string&	Description) ;

// -----------------------------------------------------------------------------
// constructeur copie add fab
// -----------------------------------------------------------------------------
  BB1Decision(const BB1Decision& src) ;

  void			OperativeDecisionsAux(ObjectSet *od) ;

  void			Save(ofstream &KBstream) { } ; // Control pan can't be saved


 protected:
  ~BB1Decision() { } ;
  bool			AnySubDecisionOperativeP() const ;


 protected:
  const string 	description ;			// user comment annotation
  const GoalT  	goal ;				// predicate which, when satisfied, makes inoperative
  Cycle 	lastCycle ;			// if !OperativeP(), cycle on which became inoperative
  int   	BB1DecisionOperativeP ;		// is it turned on?
  const int 	stableP ;			// if TRUE, a rating is assumed not to change
  const WeightT weight ;			// weighting relative to other peers
} ;

#endif

#endif // _BB1Decision_h
