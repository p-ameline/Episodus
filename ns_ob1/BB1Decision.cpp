#ifdef __GNUC__
#pragma implementation
#endif

#ifndef _OWLDLL
# include "BB1Decision.h"
# include "BB1BB.h"      // for BB1Decision::TurnOff(...)
#else
# include "ns_ob1\BB1Decision.h"
# include "ns_ob1\BB1BB.h"      // for BB1Decision::TurnOff(...)
#endif

#ifndef __TCONTROLER__

BB1Decision::BB1Decision(	const string& oname,
													BB1KB& 				kb,
													BB1Class 			*c,
													const AVPSet 	*attSpec,
													const LinkSet *linkSpec,
													const GoalT 	g,
													int 					OperativeP,
													int 					StableP,
													WeightT 			Weight,
													const string&	Description)
  : BB1Object(oname, kb, c),
    lastCycle(Cycle(0)),
    goal(g),
    BB1DecisionOperativeP(OperativeP),
    stableP(StableP),
    weight(Weight),
    description(Description)
{
  // LSB: can a decision's attributes be declared?
  ProcessAttributes(attSpec) ; // ignore Errcode
  ProcessLinksNN(linkSpec) ;   // ignore Errcode
}


// -----------------------------------------------------------------------------
// constructeur copie add fab
// -----------------------------------------------------------------------------

BB1Decision::BB1Decision(const BB1Decision& src)
  : BB1Object(src),
    lastCycle(src.lastCycle),
    goal(src.goal),
    BB1DecisionOperativeP(src.BB1DecisionOperativeP),
    stableP(src.stableP),
    weight(src.weight),
    description(src.description)
{
//  attributes	= src.attributes ;
//  strategyP	= src.strategyP ;
//  links		= src.links ;
}


// -----------------------------------------------------------------------------
// Return TRUE if either the recipient or any of its sub-decisions is operative.
// Only a Strategy should have operative sub-decisions.  For the sake of
// simplicity, don't make overriding methods.
// -----------------------------------------------------------------------------

bool BB1Decision::AnySubDecisionOperativeP() const
{
  if (OperativeP())
    return (true) ;

  if (StrategyP())
  {
    // it may have sub-decisions
    ObjectList subDecisions ;
    Objects("ImplementedBy", &subDecisions) ;
    if (false == subDecisions->empty())
      for (ObjectCIter p = subDecisions.begin() ; subDecisions.end() != p ; p++)
      {
        BB1Decision *subDecision = (BB1Decision *)(*p) ;
        if (subDecision && subDecision->AnySubDecisionOperativeP())
          return true ; // recursion
      }
    return false ;
  }

  // it's a Focus -- it can't have sub-decisions
  return (false) ;
}

// -----------------------------------------------------------------------------
// Construct a set of the subtree of BB1Decision objects which are operative,
// with the recipient as root. Since the elements of that set cannot be const,
// and the recipent is an element, it cannot be const.
// -----------------------------------------------------------------------------
ObjectSet*
BB1Decision::OperativeDecisions(ObjectSet *od)
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Decision::OperativeDecisions(d)\n\n" ;
#endif

  if (NULL == od)
    return od ;

  od->clear() ;
  OperativeDecisionsAux(od) ;
  return (od) ;
}

// -----------------------------------------------------------------------------
// Do the dirty (recursive) work of BB1Decision::OperativeDecisions(...).
// -----------------------------------------------------------------------------
void
BB1Decision::OperativeDecisionsAux(ObjectSet *od)
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Decision::OperativeDecisionsAux(od)\n\n" ;
#endif

  if (NULL == od)
    return ;

  // If the decision is operative, include it
  if (OperativeP())
    od->push_back(this) ;

  // Recur on all sub-decisions
	ObjectList subDecisions ;
  Objects("ImplementedBy", subDecisions) ;

  if (false == subDecisions.empty())
    for (ObjectCIter i = subDecisions.begin() ; subDecisions.end() != i ; i++)
    {
      BB1Decision *subDecision = (BB1Decision *) (*i) ;
      subDecision->OperativeDecisionsAux(od) ;
		}
}

// -----------------------------------------------------------------------------
// Action for KS TerminatePrescription. Overrides BB1Object
// -----------------------------------------------------------------------------
Errcode
BB1Decision::TerminatePrescriptionACAux()
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Decision::TerminatePrescriptionACAux()" ;
#endif

  // If the decision has a (Strategy) parent, remove
  // the child from the parent's current prescription
  BB1Object * parent = Object("Implements") ;
  if (parent)
  {
    parent->DeleteParentDecision(this) ;
  }
  // Render inoperative the decision and each descendant, setting the last cycle
  Cycle dLastCycle = LastCycle() ;
  Cycle lc = (dLastCycle == PRECYCLE) ? BB().ThisCycle() : dLastCycle ;

  ObjectSet od ;
  OperativeDecisions(&od) ;
  if (false == od.empty())
    for (ObjectCIter i = od.begin() ; od.end() != i ; i++)
    {
      BB1Decision *d = (BB1Decision *) (*i) ;
      d->TurnOff(lc) ;
    }

  return (SUCCESS) ;
}

// -----------------------------------------------------------------------------
// Render a decision inoperative. This results in an event.
// -----------------------------------------------------------------------------
Errcode
BB1Decision::TurnOff(const Cycle cycle)
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Decision::TurnOff(<" << cycle << ">)\n\n" ;
#endif

  BB1DecisionOperativeP = false ;
  lastCycle = cycle ;
  BB().CreateEvent(MODIFYCP, *this) ;
  return (SUCCESS) ;
} ;


// -----------------------------------------------------------------------------
// Render a decision operative. This results in an event.
// -----------------------------------------------------------------------------

Errcode BB1Decision::TurnOn()
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Decision::TurnOn()\n\n" ;
#endif

  BB1DecisionOperativeP = true ;
  lastCycle = Cycle(0) ;
  BB().CreateEvent(MODIFYCP, *this) ;
  return (SUCCESS) ;
} ;
#endif

