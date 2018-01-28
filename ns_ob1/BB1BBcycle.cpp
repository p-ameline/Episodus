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

#ifndef FOR_WINDOWS
# include "BB1BB.h"
# include "BB1Event.h"   				// for BB1BB::Triggering(...)
# include "BB1KB.h"      				// for BB1BB::AgendaManager(void) and BB1BB::Interpreter(void)
# include "BB1KS.h"      				// for BB1BB::Triggering(...)
# include "BB1KSAR.h"    				// for BB1BB::Interpreter(void)
# include "BB1Class.h"
# include "BB1types.h"
#else
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\BB1Event.h"   // for BB1BB::Triggering(...)
# include "ns_ob1\BB1KB.h"      // for BB1BB::AgendaManager(void) and BB1BB::Interpreter(void)
# include "ns_ob1\BB1KS.h"      // for BB1BB::Triggering(...)
# include "ns_ob1\BB1KSAR.h"    // for BB1BB::Interpreter(void)
# include "ns_ob1\BB1Class.h"
# include "ns_ob1\BB1types.h"
#endif

// from /a/bb1/ipc/io.h
extern "C"
{
  int recv_buf(int sock, char *buf, char *bufptr) ;
}


#ifndef __TCONTROLER__
///
/// FIXME : A rempalcer avec une procedure tcontrole
///
// -----------------------------------------------------------------------------
// The agenda manager triggers KSs with the newly-created events and places them
// on the appropriate sub-agenda.
// -----------------------------------------------------------------------------

Errcode		BB1BB::AgendaManager()
{
  SetPhase(AGENDA_MANAGER) ;
  ManageTriggeredSubAgenda() ;
  ManageExecutableSubAgenda() ;

  if (DeleteObviatedKSARsP())
  {
    // Delete each obviated KSAR
    if (false == agenda.ObviatedSubAgenda->empty())
      for (KSARCIter p = agenda.ObviatedSubAgenda->begin() ; agenda.ObviatedSubAgenda->end() != p ; p++)
				DeleteKSAR(*p) ;
		// Clear the obviated sub-agenda
		agenda.ObviatedSubAgenda->clear() ;
	}

  return (SUCCESS) ;
}
#endif


// -----------------------------------------------------------------------------
// Try to get a message. If there is one, return TRUE; else FALSE.
// -----------------------------------------------------------------------------

#ifdef __IPC__
bool		BB1BB::CheckMessage(void)
{
  int	size = recv_buf(socket, inbuf, inbuf) ; // try to read a message
  return (size > 0) ;
}
#endif // __IPC__



// -----------------------------------------------------------------------------
// If there is a socket, check it for external messages.
// -----------------------------------------------------------------------------

#ifdef __IPC__
Errcode		BB1BB::CheckMessages(void)
{
  SetPhase(CHECK_MESSAGES);
  if (socket > 0)
  {
    while(CheckMessage())
      CreateMessage(string(inbuf)) ;
  }
  return (SUCCESS) ;
}
#endif // __IPC__


#ifndef __TCONTROLER__
void
BB1BB::DeleteKSAR(BB1KSAR *k)
{
  if (NULL == k)
    return ;

  // Reference data member because BB1KSAR::TriggerEvent(void) is const
  BB1Event& te = k->triggerEvent ;

  if (false == ksarKB->objects->empty())
    for (ObjectIter iter = ksarKB->objects->begin() ; ksarKB->objects->end() != iter ; )
    {
      if (*iter == k)
				ksarKB->objects->erase(iter) ;
			else
				iter++ ;
    }
	delete k ;

  te.IncrementKSARCount(-1) ;
  if (DeleteKSARlessEventsP() && te.KSARlessP() )
    DeleteEvent(&te) ;
}
#endif

#ifndef __TCONTROLER__
void
BB1BB::DeleteEvent(BB1Event *e)
{
  if (NULL == e)
    return ;

  if (false == eventKB->objects->empty())
    for (ObjectIter iter = eventKB->objects->begin() ; eventKB->objects->end() != iter ; )
    {
      if (*iter == e)
				eventKB->objects->erase(iter) ;
			else
				iter++ ;
    }
  delete e ;
}
#endif


// -----------------------------------------------------------------------------
// On cycle 0, no triggering has occurred, so there are no KSARs. Retrieve a
// designated function and execute it. It is expected to alter the blackboard,
// thereby creating events and so triggering KSARs for the following cycle.
// -----------------------------------------------------------------------------

Errcode		BB1BB::ExecuteInitFunction()
{
  const InitFunctionT	initFunctionPtr = InitFunction() ;

  if (initFunctionPtr)
    { (*initFunctionPtr)(*this) ; }
  else
    { } // It's OK for there to be a null initialization function

  return (SUCCESS) ;
}

// -----------------------------------------------------------------------------
// This is the loop body from BB1BB::GoNCycles(...)
// -----------------------------------------------------------------------------
Errcode
BB1BB::GoOneCycle(void)
{
  #ifndef __TCONTROLER__
	Interpreter() ;             // execute the action of the scheduled KSAR
  #endif

#ifdef __IPC__
	CheckMessages() ;           // check socket for messages from outside
#endif // __IPC__

///
/// FIXME a rempalcer avec une procedure tcontrole
///
#ifndef __TCONTROLER__
	Triggering() ;              // trigger KSARs from new events


	if (!CheckDPIODialog())
		evalDPIODialog() ;
	else
		// on vérifie si des KSARs ont une chance d'avoir une réponse
		KSARManagment() ;

	AgendaManager() ;           // test conditions, changing KSAR state as required

	Scheduler() ;               // select the KSAR to execute on the next cycle
#endif
	IncrementCycle() ;          // increment the cycle count

	return (SUCCESS) ;
}

 /*
#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
// If it is the first cycle, execute the initialization function. Otherwise, if
// there is a scheduled KSAR, execute it. Otherwise, do nothing.
// -----------------------------------------------------------------------------

Errcode		BB1BB::Interpreter()
{
  if (TraceCycleP())
		trout << "\n *** Cycle " << cycle << " " ;

  if (FirstCycleP())
  {
    if (TraceCycleP())
      trout << "Executing initialization function\n" ;
    SetPhase(CYCLE_0) ;

    // On cycle 0 there is no KSAR to execute; execute the initialization function
    ExecuteInitFunction() ;

    return (SUCCESS) ;
  }
  else
  {
    SetPhase(INTERPRETER) ;

    // On cycle > 0, there may be a KSAR to execute
    KSARptr const nK = NextKSAR() ;

    if (nK)
    {
      if (TraceCycleP())
				trout << "Executing " << *(NextKSAR()) << "\n" ;
      ksar = nK ;
      ExecuteKSAR(*nK) ;
      if (DeleteExecutedKSARsP())
      {
				if (!agenda.ExecutedSubAgenda->empty())
					for (KSARIter ksarIter= agenda.ExecutedSubAgenda->begin() ; ksarIter != agenda.ExecutedSubAgenda->end() ; )
					{
						if (*ksarIter == nK)
							agenda.ExecutedSubAgenda->erase(ksarIter) ;
						else
							ksarIter++ ;
					}
				DeleteKSAR(nK) ;                   // delete the KSAR object
      }
      ksar = KSARptr(0) ;

      return (SUCCESS) ;
    }
    else
    {
      // This is normal if external messages can create events
      if (TraceCycleP())
				trout << "No KSAR to execute\n" ;

			return (SUCCESS) ;
		}
	}
}
#endif
*/

/*
#ifndef __TCONTROLER__
/// FIXME
// -----------------------------------------------------------------------------
// Given a set of KSARs, return a pointer to one with the maximal priority.
// Compute the priority at the last possible moment.
// -----------------------------------------------------------------------------

BB1KSAR&	BB1BB::Schedule()
{
	KSARSet *a = agenda.ExecutableSubAgenda ;
  BB1KSAR* temp = (BB1KSAR*)NULLPTR;
  BB1KSAR& temp2 = *temp;

	// Caller must assure that a.length > 0
	if (a && !a->empty())
	{
		KSARIter bestKSAR = a->begin() ;
		for (KSARIter k = a->begin() ; k != a->end() ; k++)
		{
			RateKSAR(**k) ; // First KSAR rated twice
			if ((*k)->Priority() > (*bestKSAR)->Priority())
				bestKSAR = k ;
		}
		return (**bestKSAR) ;
	}
        return (temp2);
}
#endif


#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
// Select an Executable KSAR for execution on the following cycle.
// -----------------------------------------------------------------------------

Errcode		BB1BB::Scheduler()
{
  SetPhase(SCHEDULER) ;

  // Schedule the KSAR for the next cycle
  if (agenda.ExecutableSubAgenda->size() == 0)
  {
    // If there are no KSAR, set a null pointer
    SetNextKSAR((BB1KSAR *) NULL) ;

    return (SUCCESS) ;
  }
  else
  {
    // Select the hightest-priority KSAR
    BB1KSAR& scheduledKSAR = Schedule() ;

    // Store this as the scheduled KSAR
    SetNextKSAR(&scheduledKSAR) ;

    return (SUCCESS) ;
  }
}

#endif
*/

#ifndef __TCONTROLER__

///
/// FIXME : a remplacer par une procedure controleur
///
// -----------------------------------------------------------------------------
// For each event, for each KS, if the KS matches the event, trigger a KSAR.
// Then clear the set of events.
// -----------------------------------------------------------------------------

Errcode		BB1BB::Triggering()
{
  /*SetPhase(TRIGGERING) ;

  if (newEvents && !newEvents->empty())
    for (EventCIter e = newEvents->begin() ; e != newEvents->end() ; e++)
    {
      triggerEvent = *e ;
      if (!kss->empty())
				for (KSCIter k = kss->begin() ; k != kss->end() ; k++)
				{
					ks = *k ; // stored in BB1BB
					if (Match(triggerEvent, ks))
						CreateKSARs(*triggerEvent, *ks) ;
					ks = (BB1KS *) NULL ; // better be null than the wrong one
				}
      // If the event triggered nothing, it may be deleted
      if (DeleteKSARlessEventsP() && triggerEvent->KSARlessP())
				DeleteEvent(triggerEvent) ;
    }

  // Reset the list of newly-arrived events
  ClearNewEvents() ;
  triggerEvent = (BB1Event*) NULL ;
                               */
  return (SUCCESS) ;
}

#endif
