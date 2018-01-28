#ifdef __GNUC__
# pragma implementation
#endif

#include <iostream.h>  // for BB1Agenda::Print(...)

#ifndef _OWLDLL
# include "BB1Agenda.h"
# include "BB1KSAR.h"   // for BB1Agenda::Print(...)
#else
# include "ns_ob1\BB1Agenda.h"
# include "ns_ob1\BB1KSAR.h"   // for BB1Agenda::Print(...)
#endif

// -----------------------------------------------------------------------------
// Remark
// -----------------------------------------------------------------------------
//  The architecture retrieves and manipulates all four sub-agendas. The matcher
//  is a source, with KSARs going to the Triggered sub-agenda. Both the Executed
//  and Obviated sub-agendas are sinks. Items are singly inserted into all four
//  sub-agendas. Items are singly deleted from Triggered and Executable, but not
//  Executed or Obviated. The following movements are possible:
//   *    Insert Triggered
//   *	  Triggered to Executable
//   *    Executable to Triggered
//   *    Triggered to Obviated
//   *    Executable to Obviated
//   *    Executable to Executed
//  All four sub-agendas can be cleared at once. The user interface examines the
//  agendas to display their objects.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//  The null constructor creates an agenda with four empty sub-agendas.
// -----------------------------------------------------------------------------

#ifndef __TCONTROLER__

BB1Agenda::BB1Agenda()
{
  ExecutableSubAgenda	= new KSARSet() ;
  ExecutedSubAgenda		= new KSARSet() ;
  ObviatedSubAgenda		= new KSARSet() ;
  TriggeredSubAgenda	= new KSARSet() ;
}


// -----------------------------------------------------------------------------
// The copy constructor
// -----------------------------------------------------------------------------

BB1Agenda::BB1Agenda(const BB1Agenda &bbAgenda)
{
  ExecutableSubAgenda	= new KSARSet(*(bbAgenda.ExecutableSubAgenda)) ;
  ExecutedSubAgenda		= new KSARSet(*(bbAgenda.ExecutedSubAgenda)) ;
  ObviatedSubAgenda		= new KSARSet(*(bbAgenda.ObviatedSubAgenda)) ;
  TriggeredSubAgenda	= new KSARSet(*(bbAgenda.TriggeredSubAgenda)) ;
}


// -----------------------------------------------------------------------------
// The destructor
// -----------------------------------------------------------------------------

BB1Agenda::~BB1Agenda()
{
  Clear() ;
  delete ExecutableSubAgenda ;
  delete ExecutedSubAgenda ;
  delete ObviatedSubAgenda ;
  delete TriggeredSubAgenda ;
}


// -----------------------------------------------------------------------------
//  The agenda is cleared at the end of a run. The sets are cleared of their
//  elements. The KSAR objects themselves are destroyed when their knowledge
//  bases are cleared.
// -----------------------------------------------------------------------------

Errcode		BB1Agenda::Clear(void)
{
  // ExecutableSubAgenda
  if (!ExecutableSubAgenda->empty())
    for (KSARIter ksar = ExecutableSubAgenda->begin() ; ksar != ExecutableSubAgenda->end() ; )
    {
			delete (*ksar) ;
			ExecutableSubAgenda->erase(ksar) ;
		}
	ExecutableSubAgenda->clear() ;

	// ExecutedSubAgenda
	if (!ExecutedSubAgenda->empty())
		for (KSARIter ksar = ExecutedSubAgenda->begin() ; ksar != ExecutedSubAgenda->end() ; )
		{
			delete (*ksar) ;
			ExecutedSubAgenda->erase(ksar) ;
		}
	ExecutedSubAgenda->clear() ;

	// ObviatedSubAgenda
	if (!ObviatedSubAgenda->empty())
		for (KSARIter ksar = ObviatedSubAgenda->begin() ; ksar != ObviatedSubAgenda->end() ; )
		{
			delete (*ksar) ;
			ObviatedSubAgenda->erase(ksar) ;
		}
	ObviatedSubAgenda->clear() ;

	// TriggeredSubAgenda
	if (!TriggeredSubAgenda->empty())
		for (KSARIter ksar = TriggeredSubAgenda->begin() ; ksar != TriggeredSubAgenda->end() ; )
		{
			delete (*ksar) ;
			TriggeredSubAgenda->erase(ksar) ;
		}
	TriggeredSubAgenda->clear() ;

  return (SUCCESS) ;
}


// -----------------------------------------------------------------------------
//  Write to the standard output a very brief summary of the agenda. It would be
//  nice to iterate over the four agenda instances, but that would mean a
//  function which returns a set -- not worth the effort.
// -----------------------------------------------------------------------------

Errcode		BB1Agenda::Print(std::ostream& os) const
{

	os << "Triggered sub-agenda:\n" ;		Print(TriggeredSubAgenda,		os) ;

	os << "Executable sub-agenda:\n" ;	Print(ExecutableSubAgenda,	os) ;

	os << "Obviated sub-agenda:\n" ;		Print(ObviatedSubAgenda,		os) ;

	os << "Executed sub-agenda:\n" ;		Print(ExecutedSubAgenda,		os) ;

  return (SUCCESS) ;
}



// -----------------------------------------------------------------------------
//  Print one of the sub-agendas. The loop prevents this from being inline.
// -----------------------------------------------------------------------------

Errcode		BB1Agenda::Print(const KSARSet *subAgenda, std::ostream& os) const
{
  if (subAgenda && !subAgenda->empty())
    for (KSARCIter p = subAgenda->begin() ; p != subAgenda->end() ; p++)
      os << "  " << *p << "\n" ;

  return (SUCCESS) ;
}

#endif












