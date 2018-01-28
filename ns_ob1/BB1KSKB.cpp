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
#ifdef __GNUC__
#pragma implementation
#endif

#ifndef FOR_WINDOWS
#include "BB1KSKB.h"
#include "BB1BB.h"
#include "BB1KB.h"
#include "BB1Event.h"     // for UpdatePrescriptionTC(...)

#ifndef __TCONTROLER__
#include "BB1Strategy.h"  // for UpdatePrescriptionTC(...)
#endif

#include "BB1Exception.h"    // add fab
#else
#include "ns_ob1\BB1KSKB.h"
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\BB1KB.h"
#include "ns_ob1\BB1Event.h"     // for UpdatePrescriptionTC(...)



#include "ns_ob1\BB1Exception.h"    // add fab
#endif


class BB1KB ;

// -----------------------------------------------------------------------------
// Trigger KS UpdatePrescription if the Trigger Object is a Strategy, its
// Current Prescription is empty, its Goal is unsatisfied, and its Remaining
// Strategic Generator is not empty.
// -----------------------------------------------------------------------------

#ifndef __TCONTROLER__
bool UpdatePrescriptionTC(const BB1Event& event)
{

#ifdef _DEBUG_
	cout << "UpdatePresciptionTC(event)\n\n" ;
#endif

	if (event.EventTypeIsP(ADD) || event.EventTypeIsP(MODIFYCP))
	{
		// Either object has been added or its current prescription modified
		// BB1Strategy overrides virtual BB1Object::UpdatePrescriptionTCAux(void)
		// If a Strategy, the Current Prescription must be empty, the Goal not
		// satisfied, and the Remaining Strategic Generator non-empty
		// If not a Strategy, return FALSE
		return (event.Object().UpdatePrescriptionTCAux()) ;
	}
	else
		{ return (false) ; } // not an add or modify-control-plan event
}


// -----------------------------------------------------------------------------
// Obviate a KSAR of UpdatePrescription if the TriggerObject, a Strategy, is
// inoperative.
// -----------------------------------------------------------------------------

bool UpdatePrescriptionOC(const BB1BB& bb, BB1KSAR& ksar)
{

#ifdef _DEBUG_
	cout << "-- DEBUG:: UpdatePrescriptionOC(bb)" ;
#endif

	try
	{
		const BB1Object *const o =  bb.TriggerObject() ;
		if (o)
		{
			// BB1Strategy overrides virtual BB1Object::UpdatePrescriptionOCAux(void)
			// If a Strategy, obviate if the Trigger Object is not operative
			// If not a Strategy, it is a BB1 error
			return (o->UpdatePrescriptionOCAux()) ;
		}
		else
		{
			NullTriggerObject nto(bb, "UpdatePrescriptionOC(...)") ;
			return (false) ; // throw nto;
		}
	} // end try
	catch (NullTriggerObject& nto)
		{ }
	return (true) ;
}


// -----------------------------------------------------------------------------
// The action of Update Prescription is to pop the Strategy's remaining
// strategic generator into its Current Prescription.
// -----------------------------------------------------------------------------

Errcode UpdatePrescriptionAC(BB1BB& bb, BB1KSAR& ksar)
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: UpdatePrescriptionAC(bb)" ;
#endif

  try
  {
    BB1Object *o = bb.TriggerObject() ;
    if (o)
    {
      // BB1Strategy overrides virtual BB1Object::UpdatePrescriptionACAux(void)
      // If a Strategy, pop the Remaining Strategic Generator into the
      // Current Prescription
      // If not a Strategy, it is a BB1 error
      return (o->UpdatePrescriptionACAux()) ;
    }
    else
    {
      NullTriggerObject nto(bb, "UpdatePrescriptionAC(...)") ;
      return (FAILURE) ; // throw nto;
    }
  } // end try
  catch (NullTriggerObject& nto)
    { }
  return (FAILURE) ;
}


// -----------------------------------------------------------------------------
// Monitor an existing Focus or Strategy for its termination Goal. When that
// Goal is met, turn off the Decision and its descendants, and tell its parent.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Trigger KS TerminatePrescription if the trigger object is a Decision (either
// Strategy or Focus) and it has been added to the Control Plan. Trigger
// immediately, but satisfy its precondition only at the end.
// -----------------------------------------------------------------------------

bool TerminatePrescriptionTC(const BB1Event& event)
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: TerminatePrescriptionTC(event)" ;
#endif

  if (event.EventTypeIsP(ADD))
  {
    // The Trigger Object has been added
    // BB1Decision overrides virtual BB1Object::TerminatePrescriptionTCAux(void)
    // If a Decision, return TRUE
    // If not a Decision, return FALSE
    return(event.Object().TerminatePrescriptionTCAux()) ;
  }
  else
    { return (false) ; } // Not an ADD event
}


// -----------------------------------------------------------------------------
// TerminatePrescription can be executed when the Decision's (Strategy's or
// Focus's) Goal is satisfied.
// -----------------------------------------------------------------------------

bool TerminatePrescriptionPC(const BB1BB& bb, BB1KSAR& ksar)
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: TerminatePrescriptionPC(bb)" ;
#endif

  try
  {
    if (bb.TriggerObject())
    {
      // BB1Decision overrides virtual BB1Object::TerminatePrescriptionPCAux(void)
      // If a Decision, obviate if the Trigger Object's Goal is satisified
      // If not a Decision, it is a BB1 error
      return (bb.TriggerObject()->TerminatePrescriptionPCAux()) ;
    }
    else
    {
      NullTriggerObject nto(bb, "TerminatePrescriptionPC(...)") ;
      return (false) ; // throw nto;
    }
  } // end try
  catch (NullTriggerObject& nto)
    { }
  return (false) ;
}


// -----------------------------------------------------------------------------
// None of its descendants is operative.
// -----------------------------------------------------------------------------

bool TerminatePrescriptionOC(const BB1BB& bb, BB1KSAR& ksar)
{

#ifdef _DEBUG_
	cout << "-- DEBUG:: TerminatePrescriptionOC(bb)" ;
#endif

	try
	{
		if (bb.TriggerObject())
		{
			// BB1Focus & BB1Strategy override virtual
			// BB1Object::TerminatePrescriptionOCAux(void)
			// If a Focus, return FALSE
			// If a Strategy, obviate if the Trigger Object is not operative
			// If not a Focus or Strategy, it is a BB1 error
			return (bb.TriggerObject()->TerminatePrescriptionOCAux()) ;
		}
		else
		{
			NullTriggerObject nto(bb, "TerminatePrescriptionOC(...)") ;
			return (false) ; // throw nto;
		}
	} // end try
	catch (NullTriggerObject& nto)
		{ }
	catch (NotADecision& nad)
		{ }
	return (false) ;
}


Errcode TerminatePrescriptionAC(BB1BB& bb, BB1KSAR& ksar)
{

#ifdef _DEBUG_
	cout << "-- DEBUG:: TerminatePrescriptionAC(bb)" ;
#endif

	try
	{
		if (bb.TriggerObject())
		{
			return (bb.TriggerObject()->TerminatePrescriptionACAux()) ;
		}
		else
		{
			NullTriggerObject nto(bb, "TerminatePrescriptionAC(...)") ;
			return (FAILURE) ; // throw nto;
		}
	} // end try
	catch (NullTriggerObject& nto)
		{ }
	return (FAILURE) ;
}


void    loadBB1KSKB(BB1BB& bb)
{

#ifdef _DEBUG_
	cout << "-- DEBUG:: LoadBB1KSLB(bb)" ;
#endif

	BB1KB *BB1KSKB = bb.DefineKB("BB1KS") ;
	if (BB1KSKB)
	{
		bb.DefineKS("UpdatePrescription",
								*BB1KSKB,
								true,
								UpdatePrescriptionTC,
								NULLPC,
								UpdatePrescriptionOC,
								UpdatePrescriptionAC,
								NULL, // NULLAVPSET,
								NULL, // NULLLINKSET,
								NULLCONTEXTGENERATOR) ;

		bb.DefineKS("TerminatePrescription",
								*BB1KSKB,
								true,
								TerminatePrescriptionTC,
								TerminatePrescriptionPC,
								TerminatePrescriptionOC,
								TerminatePrescriptionAC,
								NULL, // NULLAVPSET,
								NULL, // NULLLINKSET,
								NULLCONTEXTGENERATOR) ;
	}
	else
	{
		// Define KS will warn if unable to make KB
	}
}

#endif
