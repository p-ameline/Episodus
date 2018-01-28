/*
**
** BB1Strategy.h
**
*/


/*
** A Strategy is either a maximal element or an intermediate node
** of a control plan, a forest whose leaf nodes are focus objects.
** This, it has IMPLEMENTS links to 0 or 1 strategy objects, and
** IMPLEMENTED_BY links to 1 or more strategy objects or to 1 or
** more focus objects.
*/


#ifndef _BB1Strategy_h
# define _BB1Strategy_h 1

class BB1BB ;

# ifdef __GNUC__
#  pragma interface
# endif

# include "BB1Decision.h"   // base class

#ifndef __TCONTROLER__
class BB1Strategy : public BB1Decision
{

  // Allowed to call BB1Strategy()
  friend class BB1BB ;
  friend class BB1Object ;

  friend class ObjectList ;
  friend class ObjectSet ;

public: // API
  const StringList	*CurrentPrescription() const { return (currentPrescription) ; } ;
  const StringList	*RemainingStrategicGenerator() const { return (remainingStrategicGenerator) ; } ;
  const StringList	*StrategicGenerator() const { return (strategicGenerator) ; } ;
  Errcode		AdvanceStrategicGenerator() ;
  Errcode		RemoveCurrentPrescription(const string& oname) ;
  bool			UpdatePrescriptionTCAux() const ;
  bool			UpdatePrescriptionOCAux() const ;
  Errcode		UpdatePrescriptionACAux() ;
  void			DeleteParentDecision(BB1Decision *decision) ;
  bool			FocusP() const { return (false) ; } ;
  bool			StrategyP() const { return (true) ; } ;

private:
  BB1Strategy(const string&	oname,
	      BB1KB&		kb,
	      BB1Class		*c,
	      const AVPSet	*attSpec,
	      const LinkSet	*linkSpec,
	      const GoalT	g,
	      int		OperativeP,
	      int		StableP,
	      WeightT		Weight,
	      const StringList	*CurrentPrescription,
	      const StringList	*StrategicGenerator,
	      const string&	Description) ;
  BB1Strategy(const BB1Strategy& src) ;

  void			TurnOnCurrentPrescription() ;
  void			TurnOnSubtree() ;

private:
  StringList	*currentPrescription ;			// Subset of strategic generator now in use
  StringList	*remainingStrategicGenerator ;		// List of BB1Focus objects not yet used
  StringList	*strategicGenerator ;			// List of all BB1Focus objects
} ;

typedef BB1Strategy *Strategyptr ;

#endif

#endif // _BB1Strategy_h
