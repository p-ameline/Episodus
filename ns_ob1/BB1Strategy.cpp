#ifdef __GNUC__
#pragma implementation
#endif

#ifndef _OWLDLL
# include "BB1Strategy.h"
# include "BB1BB.h"       // for BB1Strategy::RemoveCurrentPrescription(...)
# include "BB1Focus.h"    // for BB1Strategy::TurnOnCurrentPrescription(...)
#else
# include "ns_bbk\BB1Strategy.h"
# include "ns_bbk\BB1BB.h"       // for BB1Strategy::RemoveCurrentPrescription(...)
# include "ns_bbk\BB1Focus.h"    // for BB1Strategy::TurnOnCurrentPrescription(...)
#endif

#ifndef __TCONTROLER__

BB1Strategy::BB1Strategy(const string&		oname,
			 BB1KB&			kb,
			 BB1Class		*c,
			 const AVPSet		*attSpec,
			 const LinkSet		*linkSpec,
			 const GoalT		g,
			 int			OperativeP,
			 int			StableP,
			 WeightT		Weight,
			 const StringList	*CurrentPrescription,
			 const StringList	*StrategicGenerator,
			 const string&		Description)
  : BB1Decision(oname,
			kb,
			c,
			attSpec,
			linkSpec,
			g,
			OperativeP,
			StableP,
			Weight,
			Description)
//		currentPrescription(CurrentPrescription),
//		strategicGenerator(StrategicGenerator),
//		remainingStrategicGenerator(StrategicGenerator)
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::BB1Strategy(\""
       << oname
       << "\", kb, *c, attSpec, linkSpec, g "
       << OperativeP
       << ", "
       << StableP
       << ", Weight, CurrentPrescription, StrategicGenerator, "
       << Description
       << ")\n\n" ;
#endif

  currentPrescription 		= new StringList(*CurrentPrescription) ;
  strategicGenerator		= new StringList(*StrategicGenerator) ;
  remainingStrategicGenerator	= new StringList(*StrategicGenerator) ;

}


// -----------------------------------------------------------------------------
// constructeur copie -- add fab
// -----------------------------------------------------------------------------

BB1Strategy::BB1Strategy(const BB1Strategy& src)
  : BB1Decision(src),
    currentPrescription(src.currentPrescription),
    strategicGenerator(src.strategicGenerator),
    remainingStrategicGenerator(src.remainingStrategicGenerator)
{
}


// -----------------------------------------------------------------------------
// Remove and return the first element of the remaining strategic generator
// -----------------------------------------------------------------------------

Errcode BB1Strategy::AdvanceStrategicGenerator()
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::AdvanceStrategicGenerator()\n\n" ;
#endif

  currentPrescription->clear() ;
  if (!remainingStrategicGenerator->empty())
  {
    /*
      Transfer the first element of the Remaining Strategic Generator
      to the head of the Current Prescription
    */

    string s = remainingStrategicGenerator->pop_front() ;

    currentPrescription->push_front(s) ;
  }
  // Create an Event indicating that the Control Plan has been modified
  #ifndef __TCONTROLER__
  BB().CreateEvent(MODIFYCP, *this) ;
  #endif
  return (SUCCESS) ;
}

// -----------------------------------------------------------------------------
// If the supplied name is a member of the Current Prescription, remove it.
// -----------------------------------------------------------------------------

Errcode BB1Strategy::RemoveCurrentPrescription(const string& oname)
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::RemoveCurrentPrescription(\"" << oname << "\") const\n\n" ;
#endif

  if (currentPrescription && !currentPrescription->empty())
    for (StrIter i = currentPrescription->begin() ; i != currentPrescription->end() ; )
    {
      if ((*i) == oname)
      {
				currentPrescription->erase(i) ;
				break ;
      }
      else
        i++ ;
    }

  // Create an Event indicating that the Control Plan has been modified
  #if __TCONTROLER__
  BB().CreateToken((MODIFYCP, *this) ;
  #else
  BB().CreateEvent(MODIFYCP, *this) ;
  #endif
  return (SUCCESS) ;
};



// -----------------------------------------------------------------------------
// Turn on all descendant Decisions of the Strategy.
// -----------------------------------------------------------------------------
void    BB1Strategy::TurnOnSubtree()
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::TurnOnSubtree()\n\n" ;
#endif

  // retrieve the set of all child Decisions
  ObjectList decisions ;
  Objects("ImplementedBy", &decisions) ;
  // Turn on all descendant Decisions

  if (false == decisions.empty())
    for (ObjectCIter p = decisions.begin() ; decisions.end() != p ; p++)
    {
      BB1Decision *dp = (BB1Decision *)(*p) ;
      if (dp->StrategyP())
      {
				BB1Strategy *sp = (BB1Strategy *) dp ;
				sp->TurnOnSubtree() ;
      }
      else
      {
				if (dp->FocusP())
					dp->TurnOn() ;
      }
    }

  // Turn on self
	TurnOn() ;
}

// -----------------------------------------------------------------------------
// Auxiliary to BB1Strategy::UpdatePrescriptionACAux(void) The current
// prescription is a list of strings, each of which should be the short name of
// a Focus object. For each such name, if it is the name of one of the objects
// linked to the Strategy by an "ImplementedBy" link
// -----------------------------------------------------------------------------
void
BB1Strategy::TurnOnCurrentPrescription()
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::TurnOnCurrentPrescription()\n\n" ;
#endif

  // Retrieve the current prescription for this strategy
  StringList cp(*CurrentPrescription()) ;
  if (cp.empty())
    return ;

  // Retrieve all the focus objects for this strategy
  ObjectList decisions ;
  Objects("ImplementedBy", &ecisions) ;

  // For each element of the current prescription, turn on the corresponding decision
  for (StrCIter p = cp.begin() ; cp.end() != p ; p++)
  {
    string pfn = *p ;
    if (false == decisions.empty())
      for (ObjectCIter q = decisions.begin() ; decisions.end() != q ; q++)
      {
        BB1Decision *dp = (BB1Decision *)(*q) ;

        if (dp->Name() == pfn)
        {
          if (dp->StrategyP())
          {
            BB1Strategy *sp = (BB1Strategy *)dp ;
            sp->TurnOnSubtree() ;
          }
          else
            if (dp->FocusP())
          {
            BB1Focus *fp = (BB1Focus *)dp ;
            fp->TurnOn() ;
          }
          else
          {
            // throw something
          }
          break ;
        }
      }
  }
}

// -----------------------------------------------------------------------------
// Does this object trigger KS UpdatePrescription? Overrides BB1Object. True if
// the current prescription is exhausted, the goal is not satisfied, and the
// remaining strategic generator is not exhausted.
// -----------------------------------------------------------------------------
bool
BB1Strategy::UpdatePrescriptionTCAux() const
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::UpdatePrescriptionTCAux() const\n\n" ;
#endif

  return (CurrentPrescription()->empty() &&		// true initially & after TerminatePrescription
	  !(GoalTrueP()) &&				// still a job to do
	  !(RemainingStrategicGenerator()->empty())) ;	// still unexecuted plan
}


// -----------------------------------------------------------------------------
// Obviate KS UpdatePrescription if the Trigger Object, a Strategy, is not
// operative. Overrides BB1Object.
// -----------------------------------------------------------------------------
bool
BB1Strategy::UpdatePrescriptionOCAux() const
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::UpdatePrescriptionOCAux() const\n\n" ;
#endif

  return (!OperativeP()) ;
}

// -----------------------------------------------------------------------------
// Action for KS UpdatePrescription. Overrides BB1Object. Advance the strategic
// generator.
// -----------------------------------------------------------------------------
Errcode
BB1Strategy::UpdatePrescriptionACAux()
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::UpdatePrescriptionACAux() const\n\n" ;
#endif

  Errcode retval = AdvanceStrategicGenerator() ;
  TurnOnCurrentPrescription() ;
  return (retval) ;
}

// -----------------------------------------------------------------------------
// Overrides BB1Object. Remove the decision object from the current
// prescription.
// -----------------------------------------------------------------------------

void    BB1Strategy::DeleteParentDecision(BB1Decision *decision)
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1Strategy::DeleteParentDecision(decision) const\n\n" ;
#endif

  RemoveCurrentPrescription(decision->Name()) ;
}

#endif

