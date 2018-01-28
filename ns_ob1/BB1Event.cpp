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

#include <iostream.h>     // for operator<<(...)

#ifndef FOR_WINDOWS
# include "BB1Event.h"
# include "AttValPair.h"   // for BB1Event::ChangedAttributeIsP(...)
# include "BB1KB.h"        // for BB1Event::EventKBNamedP(...)
# include "BB1Link.h"      // for BB1Event::ChangedLinkIsP(...)
# include "BB1Exception.h"    // add fab
#else
# include "ns_ob1\BB1Event.h"
# include "ns_ob1\AttValPair.h"   // for BB1Event::ChangedAttributeIsP(...)
# include "ns_ob1\BB1KB.h"        // for BB1Event::EventKBNamedP(...)
# include "ns_ob1\BB1Link.h"      // for BB1Event::ChangedLinkIsP(...)
# include "ns_ob1\BB1Exception.h"    // add fab
#endif

#ifndef __TCONTROLER__

long BB1Event::_lEventCount = 0 ;

BB1Event::BB1Event(const string& oname, BB1KB& kb, BB1Class *c, const AVPSet *chgAttributes, const LinkSet *chgLinks, EventType changeType, BB1Object& Object)
         :BB1Object(oname, kb, c),
          object(Object),
          type(changeType),
          BB1EventKSARcount(0),
          changedAttributes(*chgAttributes) ;
          changedLinks(*chgLinks) ;
{
  _lEventCount++ ;
}

// -----------------------------------------------------------------------------
// constructeur copie -- add fab
// -----------------------------------------------------------------------------

BB1Event::BB1Event(const BB1Event& src)
         :BB1Object(src),
          object(src.object),
          type(src.type),
          BB1EventKSARcount(src.BB1EventKSARcount),
          changedAttributes(src.changedAttributes),
          changedLinks(src.changedLinks) ;
{
  _lEventCount++ ;
}

BB1Event::~BB1Event()
{
  if (false == changedAttributes.empty())
    for (AVPIter ap = changedAttributes.begin() ; changedAttributes.end() != ap ; )
    {
      delete (*ap) ;
      changedAttributes->erase(ap) ;
		}

  if (false == changedLinks.empty())
    for (LinkIter lp = changedLinks.begin() ; changedLinks.end() != lp ; )
    {
      delete (*lp) ;
      changedLinks->erase(lp) ;
		}

  _lEventCount-- ;
}

// -----------------------------------------------------------------------------
// Send to the output stream a textual representation of the event
// -----------------------------------------------------------------------------

std::ostream& operator <<(std::ostream& os, const BB1Event& event)
{
  os << event.Name()
     << "("
     << "["
     << event.CycleCreated()
     << "] ("
     << event.TypeString()
     << " "
     << event.Object().LongName()
     << ")" ;
  return (os) ;
}

// -----------------------------------------------------------------------------
// Return TRUE if the blackboard change that caused the event to be created
// involved a change to the attribute; otherwise return FALSE.
// -----------------------------------------------------------------------------
/*
bool BB1Event::ChangedAttributeIsP(const string& attribute) const
{

#ifdef _DEBUG_
	cout << "-- DEBUG:: BB1Event::ChangedAttributeIsP(\"" << attribute << "\") const\n\n" ;
#endif

	if (changedAttributes && !changedAttributes->empty())
		for (AVPCIter a = changedAttributes->begin() ; a != changedAttributes->end() ; a++)
		{
			if ((*a)->AttributeName() == attribute)
				return (true) ;
		}

	return (false) ;
}
*/

// -----------------------------------------------------------------------------
// Return TRUE if the blackboard change that caused the event to be created
// involved a change to the link; otherwise return FALSE.
// -----------------------------------------------------------------------------

bool
BB1Event::ChangedLinkIsP(const string& linkName) const
{
#ifdef _DEBUG_
	cout << "-- DEBUG:: BB1Event::ChangedLinkIsP(\"" << linkName << "\") const\n\n" ;
#endif
	if (false == changedLinks.empty())
		for (LinkCIter l = changedLinks.begin() ; changedLinks.end() != l ; l++)
		{
			if ((*l)->Name() == linkName)
				return true ;
		}
	return false ;
}

// -----------------------------------------------------------------------------
// Did the event signal a change to the named KB?
// -----------------------------------------------------------------------------
bool
BB1Event::EventKBNamedP(const string& eventName) const
{
	return (Object().KB().Name() == eventName) ;
}

// -----------------------------------------------------------------------------
// Return a reference to a BB1String representing the event's type. The switch
// prevents this from being inline.
// -----------------------------------------------------------------------------
const string&
BB1Event::TypeString(void) const
{
	static string s = NULLSTRING ;
	try
	{
		switch(type)
		{
			case ADD			:	s = "ADD" ;
											break ;
			case EXTERNAL	:	s = "EXTERNAL" ;
											break ;
			case MODIFY		:	s = "MODIFY" ;
											break ;
			case MODIFYCP	:	s = "MODIFYCP" ;
											break ;
			default				:
			{
				UnknownEventType uet(BB(), "BB1Event::TypeString(...)") ;
				return (s) ; // throw uet;
			}
		}
	} // end try
	catch (UnknownEventType& uet)
		{ } // constructor aborts

	return (s) ;
}

#endif

