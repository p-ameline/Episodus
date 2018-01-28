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
# include "BB1KB.h"        // for BB1BB::ClassNamed(...)
# include "BB1Event.h"     // for BB1BB::TriggerObject(...)
# include "BB1KSAR.h"      // for BB1BB::KS(...)
# include "BB1Exception.h"    // add fab
#else
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\BB1KB.h"        // for BB1BB::ClassNamed(...)
# include "ns_ob1\BB1Event.h"     // for BB1BB::TriggerObject(...)
# include "ns_ob1\BB1KSAR.h"      // for BB1BB::KS(...)
# include "ns_ob1\BB1Exception.h"    // add fab
#endif


// -----------------------------------------------------------------------------
//  Return a pointer to the class meta-object whose name is the given name. If
//  there is no such object, return a null pointer.
// -----------------------------------------------------------------------------

BB1Class	*BB1BB::ClassNamed(const string& c) const
{
#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1BB::ClassNamed(\"" << c << "\") const\n" ;
#endif

  try
  {
    if (_classKB)
    {
      return ((BB1Class *) (_classKB->ObjectNamed(c))) ;
    }
    else
    {
      KBNotFound kbnf(*this, "BB1BB::ClassNamed(...)", "Class") ; /* throw kbnf; */
    }
  } // end try
  catch (KBNotFound& kbnf)
    { } // constructor aborts

  return ((BB1Class *) NULL) ;
}

#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
//  Return a pointer to the event object whose name is the given name. If there
//  is no such object, return a null pointer.
// -----------------------------------------------------------------------------

BB1Event*
BB1BB::EventNamed(const string& eventName) const
{

#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1BB::EventNamed(\"" << eventName << "\") const\n" ;
#endif

  try
  {
    if (eventKB)
      { return ((BB1Event *) (eventKB->ObjectNamed(eventName))) ; }
    else
      { KBNotFound kbnf(*this, "BB1BB::EventNamed(...)", "Event") ; /* throw kbnf; */ }
  } // end try
  catch (KBNotFound& kbnf)
    { } // constructor aborts
  
  return ((BB1Event *) NULL) ;
}
#endif



// -----------------------------------------------------------------------------
//  Return a pointer to the KB whose name is the given name. If there is no such
//  KB, return a null pointer.
// -----------------------------------------------------------------------------

BB1KB*
BB1BB::KBNamed(const string& KBName) const
{

#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1BB::KBNamed(\"" << KBName << "\") const\n" ;
#endif

  if (_kbs.empty())
    return ((BB1KB *) 0) ;

  for (KBCIter i = _kbs.begin() ; _kbs.end() != i ; i++)
  {
    if ((NULL != *i) && ((*i)->Name() == KBName))
      return (*i) ;
  }

  return ((BB1KB *) 0) ;
}

#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
//  Return a pointer to the KSAR object whose name is the given name. If there
//  is no such object, return a null pointer.
// -----------------------------------------------------------------------------

KSARptr	BB1BB::KSARNamed(const string& KSARName) const
{
#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::KSARNamed(\"" << KSARName << "\") const\n" ;
#endif

  try
  {
    if (ksarKB)
      { return (KSARptr(ksarKB->ObjectNamed(KSARName))) ; }
    else
      { KBNotFound kbnf(*this, "BB1BB::KSARNamed(...)", "KSAR") ; /* throw kbnf; */ }
  } // end try
  catch (KBNotFound& kbnf)
    { } // constructor aborts
  return ((BB1KSAR *) NULL) ;
}
#endif

#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
//  Return a pointer to the KS of the KSAR currently being executed, if any. The
//  user should not be able to modify the KS.
// -----------------------------------------------------------------------------

const BB1KS	*BB1BB::KS(void) const
{
#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::KS() const\n" ;
#endif

  try
  {
    if (KSPhaseCheck())
    { // might throw
      if (Phase() == TRIGGERING)
				{ return (ks) ; /* the KS currently triggering */ }
      else
      {
      	// get the KS from the KSAR
				const BB1KSAR * const ksarptr = KSAR() ;
				const BB1KS& ksref = ksarptr->KS() ;
				return (&ksref) ;
      }
    }
  } // end try
  catch (KSUndefined& ksu)
    { return ((BB1KS *) NULL) ; }
  catch (UnknownPhase& up)
    { } // constructor aborts

  return ((BB1KS *) NULL) ;
}
#endif

// -----------------------------------------------------------------------------
//  Return a pointer to the KSAR currently being executed, if any. The user
//  should be able to modify the KSAR only with BB1KSAR::Set(..).
// -----------------------------------------------------------------------------

#ifndef __TCONTROLER__
BB1KSAR	*BB1BB::KSAR(void) const
{
#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::KSAR() const\n" ;
#endif

  try
  {
    if(KSARPhaseCheck())
      { return(ksar) ; /* might throw */ }
  } // end try
  catch (UnknownPhase& up)
    { } // constructor aborts
  catch (KSARUndefined& ksaru)
    { return((BB1KSAR *) NULL) ; }

  return((BB1KSAR *) NULL) ;
}
#endif

// -----------------------------------------------------------------------------
//  The user interface uses this. The "long name" has the syntax
//  "<KB name>.<Object name>". This function parses and then calls the
//  two-argument version of this method. This should be a const method, but
//  neither before() nor after() is.
// -----------------------------------------------------------------------------

BB1Object	*BB1BB::ObjectNamed(const string& LongName) const
{
#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::ObjectNamed(\"" << LongName << "\") const\n" ;
#endif

  //  string ln = LongName; // copy: BB1String::before & BB1String::after aren't const
  int dotpos = LongName.find(".") ;
  return (ObjectNamed(LongName.substr(0,dotpos), LongName.substr(dotpos+1,LongName.length() - dotpos - 1))) ;
}

// This is syntactic sugar so that the user doesn't have to call KBNamed.

BB1Object	*BB1BB::ObjectNamed(const string& KBName, const string& ObjName) const
{
#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::ObjectNamed(\"" << KBName << "\", \"" << ObjName << "\") const\n" ;
#endif

  try
  {
    BB1KB *kb = KBNamed(KBName) ;
    if (kb)
      { return (kb->ObjectNamed(ObjName)) ; }
    else
      { KBDoesNotExist kbdne(*this, "BB1BB::ObjectNamed(...)", KBName) ; /* throw kbdne; */ }
  }
  catch (KBDoesNotExist& kbdne)
    { return (Objptr(0)) ; }
  
  return ((BB1Object *) NULL) ;
}


#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
//  Return a pointer to the trigger event. During triggering, this is the event
//  being considered. During execution, it is the KSAR's trigger event.
// -----------------------------------------------------------------------------

const BB1Event	*BB1BB::TriggerEvent() const
{

#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::TriggerEvent() const\n" ;
#endif

  try
  {
    BB1Event const *te = (BB1Event *) NULL ;
    if (TriggerEventPhaseCheck())
    {
      // might throw
      if (Phase() == TRIGGERING)
      {
				// get trigger event from BB1BB
				if (triggerEvent)
					{ te = triggerEvent ; }
        else
					{ NullTriggerEvent nte(*this, "BB1BB::TriggerEvent()") ; /* throw nte; */ }
      }
      else
      {
				// get trigger event from BB1BB's BB1KSAR
				if (KSAR())
					{ te = &(KSAR()->TriggerEvent()) ; }
        else
					{ NullKSAR nksar(*this, "BB1BB::TriggerEvent()") ; /* throw nksar; */ }
      }
      return (te) ;
    }
  } // end try
  catch (NullTriggerEvent& nte)
    { } // constructor aborts
  catch (NullKSAR& nksar)
    { } // constructor aborts
  catch (TriggerEventUndefined& teu)
    { return ((BB1Event *) NULL) ; }
  catch (UnknownPhase& up)
    { } // constructor aborts
  
  return ((BB1Event *) NULL) ;
}
#endif



#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
//  Return a pointer to the trigger object of the KSAR currently being executed,
//  if any.
// -----------------------------------------------------------------------------

BB1Object	*BB1BB::TriggerObject() const
{

#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::TriggerObject() const\n" ;
#endif

  try
  {
    BB1Object *to = Objptr(0) ;
    if (TriggerObjectPhaseCheck())
    {
      // might throw
      if (Phase() == TRIGGERING)
      {
				// get the trigger object from the trigger event
				if (triggerEvent)
					{ to = &(triggerEvent->Object()) ; }
				else
					{ NullTriggerEvent nte(*this, "BB1BB::TriggerEvent()") ; /* throw nte; */ }
      }
      else
      {
				if (KSAR())
					{ to = &(KSAR()->TriggerObject()) ; }
				else
					{ NullKSAR nksar(*this, "BB1BB::TriggerEvent()") ; /* throw nksar; */ }
      }
      return (to);
    }
  } // end try
  catch (NullTriggerEvent& nte)
    { } // constructor aborts
  catch (NullKSAR& nksar)
    { } // constructor aborts
  catch (TriggerObjectUndefined& tou)
    { return ((BB1Object *) NULL) ; }
  catch (UnknownPhase& up)
    { } // constructor aborts

  return ((BB1Object *) NULL) ;
}
#endif
