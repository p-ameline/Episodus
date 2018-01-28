#ifdef __GNUC__
# pragma implementation
#endif

#include <iostream.h>               // for operator<<(...)

#ifndef _OWLDLL
# include "BB1KSAR.h"
# include "BB1Event.h"        			// for BB1KSAR::BB1KSAR(...)
# include "BB1KB.h"           			// for BB1KSAR::ThisCycle(...)
# include "BB1KS.h"           			// for BB1KSAR::ControlKSARP(...)
# include "BB1Exception.h"    			// add fab
#else
# include "ns_bbk\BB1KSAR.h"
# include "ns_bbk\BB1Event.h"        // for BB1KSAR::BB1KSAR(...)
# include "ns_bbk\BB1KB.h"           // for BB1KSAR::ThisCycle(...)
# include "ns_bbk\BB1KS.h"           // for BB1KSAR::ControlKSARP(...)
# include "ns_bbk\BB1Exception.h"    // add fab
#endif

#ifndef __TCONTROLER__

BB1KSAR::BB1KSAR(	const string&	oname,
									BB1KB&				kb,
									BB1Class			*c,
									const AVPSet	*attSpec,
									const LinkSet	*linkSpec,
									BB1Event&			tE,
									const BB1KS&	k,
									const Cycle		tC)
  : BB1Object(oname, kb, c),
    ks(k),
    triggerEvent(tE),
    triggerObject(triggerEvent.Object()),
    triggeredCycle(tC),
    executableCycle(PRECYCLE),
    executedCycle(PRECYCLE),
    obviatedCycle(PRECYCLE),
    priority(0),
		status(TRIGGERED),
		nsStatus(NONE)

{
  ProcessAttributes(attSpec) ; // These are the locally-bound variables; ignore Errcode
	ProcessLinksNN(linkSpec) ;   // ignore Errcode

	variables	= new AVPSet() ;
}


// -----------------------------------------------------------------------------
// constructeur copie -- add fab
// -----------------------------------------------------------------------------

BB1KSAR::BB1KSAR(const BB1KSAR& src)
  : BB1Object(src),
    ks(src.ks),
    triggerEvent(src.triggerEvent),
    triggerObject(src.triggerObject),
		triggeredCycle(src.triggeredCycle),
    executableCycle(src.executableCycle),
    executedCycle(src.executedCycle),
    obviatedCycle(src.obviatedCycle),
    priority(src.priority),
		status(src.status),
		nsStatus(src.nsStatus)
{
		variables = new AVPSet(*(src.variables)) ;
}


// -----------------------------------------------------------------------------
// destructeur -- add fab
// -----------------------------------------------------------------------------

BB1KSAR::~BB1KSAR()
{
	ClearSetOfAVP(variables) ;
}


// -----------------------------------------------------------------------------
// Send to the output stream a textual representation of the KSAR
// -----------------------------------------------------------------------------

std::ostream& operator <<(std::ostream& os, const BB1KSAR& ksar)
{
  os << ksar.Name()
     << "("
     << ksar.KS().Name()
     << ", "
     << ksar.TriggerEvent().Name()
     << ", "
     << ksar.TriggerObject().LongName()
     << ")";
  
  return (os) ;
}


// -----------------------------------------------------------------------------
// Is the KSAR's KS a control KS?
// -----------------------------------------------------------------------------

int BB1KSAR::ControlKSARP() const
{
  return (KS().ControlKSP()) ;
}


// -----------------------------------------------------------------------------
// Display more than operator << does
// -----------------------------------------------------------------------------

void BB1KSAR::Dump(std::ostream& os) const
{
  os << *this << "\n" ;
  os << " Priority " << Priority() << "\n" ;
  switch (status)
  {
		case TRIGGERED	:	os << " Triggered on Cycle "  << TriggeredCycle()  << "\n" ;

		case EXECUTABLE	:	os << " Executable on Cycle " << ExecutableCycle() << "\n" ;
											break ;

		case EXECUTED		:	os << " Executed on Cycle "   << ExecutedCycle()   << "\n" ;
											break ;

		case OBVIATED		:	os << " Obviated on Cycle "   << ObviatedCycle()   << "\n" ;
											break ;
  }
  os << "[\n" ;

  if (Attributes() && !Attributes()->empty())
    for (AVPCIter p = Attributes()->begin() ; p != Attributes()->end() ; p++)
    {
      os  << "  (" << (*p)->AttributeName()
					<< " "   << Objptr((*p)->AttributeValue().getPtr())->LongName()
	  			<< ")\n" ;
    }
  os << "]\n" ;
}


// -----------------------------------------------------------------------------
// Return a reference to a BB1String representing the KSAR's status. The switch
// prevents this from being inline.
// -----------------------------------------------------------------------------

const string& BB1KSAR::StatusString() const
{
  static string s = NULLSTRING ;
  try
  {
    switch (status)
    {
			case TRIGGERED 	:	s = "Triggered" ;
												break ;

			case EXECUTABLE	:	s = "Executable" ;
												break ;

			case EXECUTED		:	s = "Executed" ;
												break ;

			case OBVIATED		:	s = "Obviated" ;
												break ;

			default					:	UnknownKSARStatus uks(BB(), "BB1KSAR::StatusString()") ;
												return (s) ; // throw uks;
    }
  }
  catch (UnknownKSARStatus& uks)
    { } // constructor aborts
  return (s) ;
}



// -----------------------------------------------------------------------------
// Convenience function for accessing BB1BB::ThisCycle()
// -----------------------------------------------------------------------------

Cycle BB1KSAR::ThisCycle() const
{
	return (KB().ThisCycle()) ;
}

#endif
