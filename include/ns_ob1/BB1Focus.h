/*
**
** BB1Focus.h
**
*/

/*
** A Focus is a leaf node of a control plan.  
** A focus may have an IMPLEMENTS link to 1 strategy object.
*/


#ifndef _BB1Focus_h
# define _BB1Focus_h 1

# ifdef __GNUC__
#  pragma interface
# endif // __GNUC__

# include "BB1Decision.h"


#ifndef __TCONTROLER__

class BB1Focus : public BB1Decision
{
	// Allowed to call BB1Focus()
	friend class BB1BB ;
	friend class BB1Object ;

	friend class ObjectList ;
	friend class FocusSet ;
	friend class ObjectSet ;

 public:
	bool	TerminatePrescriptionOCAux() const		{ return (false) ; }
	bool	UpdatePrescriptionTCAux() const				{ return (false) ; }
	bool	FocusP() const												{ return (true) ;  }
	bool	StrategyP() const											{ return (false) ; }


 private:
	BB1Focus(	const string&			oname,
						BB1KB&						kb,
						BB1Class					*c,
						const AVPSet			*attSpec,
						const LinkSet			*linkSpec,
						const GoalT				g,
						int								OperativeP,
						int								StableP,
						WeightT						Weight,
						const CriterionT	crit,
						const string&			Description) ;
  
  BB1Focus(const BB1Focus& src) ;

 private:
	const	CriterionT	Criterion() const					{ return (criterion) ; }

 private:
	const	CriterionT	criterion ; // Pointer to function which maps a BB1KSAR to [0...100]
} ;

#endif

#endif // _BB1Focus_h
