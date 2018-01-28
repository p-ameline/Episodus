// -----------------------------------------------------------------------------
// BB1Agenda.h
// -----------------------------------------------------------------------------
// class BB1Agenda
// contient les 4 sous-agendas (ensemble de KSARs dans l'état
//		* déclencés 	(triggered)
//		* exécutables (executable)
//		* exécutés		(exécuted)
//		* écartés			(obviated)
// -----------------------------------------------------------------------------

#ifndef __BB1AGENDA_H__
# define __BB1AGENDA_H__ 1

# pragma interface

# include "ns_ob1\BB1types.h"

#ifndef __TCONTROLER__
//class ostream; // for BB1Agenda::Print(...)
class BB1BB ;
class BB1KSAR ;

class _BBKEXPORT BB1Agenda
{
  // Allowed to call all member functions and to access all data members
	friend class BB1BB ;

 public:

	BB1Agenda() ;
	BB1Agenda(const BB1Agenda& bbAgenda) ;
	~BB1Agenda() ;

	const KSARSet	*ExecutableKSARs() const { return &ExecutableSubAgenda ; }
	const KSARSet	*ExecutedKSARs()   const { return &ExecutedSubAgenda ;   }
	const KSARSet	*ObviatedKSARs()   const { return &ObviatedSubAgenda ;   }
	const KSARSet	*TriggeredKSARs()  const { return &TriggeredSubAgenda ;  }

	Errcode				Clear() ;

	// Insert the KSAR into the Triggered sub-agenda
	Errcode				Insert(BB1KSAR *const ksar)				{ TriggeredSubAgenda.push_back(ksar) ; return (SUCCESS) ; }

	// Is the executable sub-agenda empty?
	bool					NullP() const											{ return (ExecutableSubAgenda.empty()) ; }

	Errcode				Print(std::ostream& os = cout) const ;
	Errcode				Print(const KSARSet *subAgenda, std::ostream& os = cout) const ;

 private:

	// The four sub-agendas are members, not attributes, to improve access speed.
  //
	KSARSet				ExecutableSubAgenda ;
	KSARSet				ExecutedSubAgenda ;
	KSARSet				ObviatedSubAgenda ;
	KSARSet				TriggeredSubAgenda ;
} ;

#endif

#endif // __BB1AGENDA_H__

