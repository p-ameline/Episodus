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

#include <iostream.h>               // for operator<<(...)

#ifndef FOR_WINDOWS
#include "AttValPair.h"
#include "BB1types.h"
#include "BB1Class.h"
//#include "BB1Event.h"


#include "BB1KB.h"
#include "BB1KS.h"
#include "BB1Link.h"
#include "BB1Message.h"
#include "BB1Object.h"

#else

#include "ns_ob1\AttValPair.h"
#include "ns_ob1\BB1types.h"
#include "ns_ob1\BB1Class.h"
#include "ns_ob1\BB1Event.h"
#include "ns_ob1\BB1KB.h"
#include "ns_ob1\BB1KS.h"
#include "ns_ob1\BB1Link.h"
#include "ns_ob1\BB1Message.h"
#include "ns_ob1\BB1Object.h"
#endif

long ObjectList::_lObjectListCount   = 0 ;
long StringList::_lStringListCount   = 0 ;
long PathsList::_lPathsListCount     = 0 ;
long AVPSet::_lAVPSetCount           = 0 ;
long ContextSet::_lContextSetCount   = 0 ;
long KBSet::_lKbSetCount             = 0 ;
long KSSet::_lKsSetCount             = 0 ;
long AKSSet::_lAKSSetCount           = 0 ;
long LinkSet::_lLinkSetCount         = 0 ;
long LinkPairSet::_lLinkPairSetCount = 0 ;
long ObjectSet::_lObjectSetCount     = 0 ;

ObjectList::ObjectList()
{
  _lObjectListCount++ ;
}


ObjectList::ObjectList(const ObjectList& src)
{
  _lObjectListCount++ ;

	if (false == src.empty())

		for (ObjectCIter o = src.begin() ; src.end() != o ; o++)
			push_back(*o) ;
/*
		{
			const BB1Class *pClass = dynamic_cast<const BB1Class *>(*o) ;
			if (pClass)
				push_back(pClass) ;

			const BB1Event *pEvent = dynamic_cast<const BB1Event *>(*o) ;
			if (pEvent)
				push_back(pEvent) ;

			const BB1Focus *pFocus = dynamic_cast<const BB1Focus *>(*o) ;
			if (pFocus)
				push_back(pFocus) ;

			const BB1KS *pKS = dynamic_cast<const BB1KS *>(*o) ;
			if (pKS)
				push_back(pKS) ;

			const BB1KSAR *pKSAR = dynamic_cast<const BB1KSAR *>(*o) ;
			if (pKSAR)
				push_back(pKSAR) ;

			const BB1Message *pMessage = dynamic_cast<const BB1Message *>(*o) ;
			if (pMessage)
				push_back(pMessage) ;

			// derivate from BB1Decision than derive from BB1Object
			const BB1Strategy *pStrategy = dynamic_cast<const BB1Strategy *>(*o) ;
			if (pStrategy)
				push_back(pStrategy) ;
		}
*/
}

ObjectList::~ObjectList()
{
	vider() ;

  _lObjectListCount-- ;
}

void
ObjectList::vider()
{
	if (false == empty())
		for (ObjectIter o = begin() ; end() != o ; )
			erase(o) ;
}

ObjectList&
ObjectList::operator=(const ObjectList& src)
{
	if (this == &src)
		return (*this) ;

	vider() ;

	if (false == src.empty())
		for (ObjectCIter o = src.begin() ; src.end() != o ; o++)
			push_back(*o) ;
/*
		{
			const BB1Class *pClass = dynamic_cast<const BB1Class *>(*o) ;
			if (pClass)
				push_back(pClass) ;

			const BB1Event *pEvent = dynamic_cast<const BB1Event *>(*o) ;
			if (pEvent)
				push_back(pEvent) ;

			const BB1Focus *pFocus = dynamic_cast<const BB1Focus *>(*o) ;
			if (pFocus)
				push_back(pFocus) ;

			const BB1KS *pKS = dynamic_cast<const BB1KS *>(*o) ;
			if (pKS)
				push_back(pKS) ;

			const BB1KSAR *pKSAR = dynamic_cast<const BB1KSAR *>(*o) ;
			if (pKSAR)
				push_back(pKSAR) ;

			const BB1Message *pMessage = dynamic_cast<const BB1Message *>(*o) ;
			if (pMessage)
				push_back(pMessage) ;

			// derivate from BB1Decision than derive from BB1Object
			const BB1Strategy *pStrategy = dynamic_cast<const BB1Strategy *>(*o) ;
			if (pStrategy)
				push_back(pStrategy) ;
		}
*/

	return (*this) ;
}

StringList::StringList()
{
  _lStringListCount++ ;
}

StringList::StringList(const StringList& src)
{
  _lStringListCount++ ;

	if (false == src.empty())
		for (StrCIter s = src.begin() ; src.end() != s ; s++)
			push_back(*s) ;
}

StringList::~StringList()
{
	vider() ;

  _lStringListCount-- ;
}

void
StringList::vider()
{
	if (false == empty())
		for (StrIter s = begin() ; end() != s ; )
			erase(s) ;
}

StringList&
StringList::operator=(const StringList& src)
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (StrCIter s = src.begin() ; src.end() != s ; s++)
			push_back(*s) ;

	return (*this) ;
}


PathsList::PathsList()
{
  _lPathsListCount++ ;
}

PathsList::PathsList(PathsList& src)
{
  _lPathsListCount++ ;

	if (false == src.empty())
		for (PathsIterator p = src.begin() ; src.end() != p ; p++)
			push_back(new string(**p)) ;
}

PathsList::~PathsList()
{
	vider() ;

  _lPathsListCount-- ;
}

/*
void        PathsList::vider()
{
	if (!empty())
		for (PathsIterator p = begin() ; p != end() ; )
		{
			delete (*p) ;
			erase(p) ;
		}
	clear() ;
}
*/
/*
PathsList&	PathsList::operator=(PathsList& src)
{
	vider() ;

	if (!src.empty())
		for (PathsCIter p = src.begin() ; p != src.end() ; p++)
			push_back(new string(**p)) ;

	return (*this) ;
}
*/

AVPSet::AVPSet()
{
  _lAVPSetCount++ ;
}

AVPSet::AVPSet(const AVPSet& src)
{
  _lAVPSetCount++ ;

	if (false == src.empty())
    for (AVPCIter avp = src.begin() ; src.end() != avp ; avp++)
      push_back(new AttValPair(**avp)) ;
}

AVPSet::~AVPSet()
{
	vider() ;

  _lAVPSetCount-- ;
}

AVPSet& AVPSet::operator=(const AVPSet& src)
{
  if (this == &src)
    return (*this) ;

	vider() ;

	if (false == src.empty())
		for (AVPCIter avp = src.begin() ; src.end() != avp ; avp++)
			push_back(new AttValPair(**avp)) ;

	return (*this) ;
}

// add takes care of duplicates

AVPSet&
AVPSet::operator|=(const AVPSet& src)
{
  if (this == &src)
    return (*this) ;

	if (&src && (false == src.empty()))
		for (AVPCIter p = src.begin() ; src.end() != p ; p++)
			push_back(new AttValPair(**p)) ;

	return (*this) ;
}

ContextSet::ContextSet()
{
  _lContextSetCount++ ;
}

ContextSet::ContextSet(const ContextSet& src)
{
  _lContextSetCount++ ;

	if (false == src.empty())
		for (ContextCIter c = src.begin() ; src.end() != c ; c++)
			push_back(new Context(**c)) ;
}

ContextSet::~ContextSet()
{
	vider() ;

  _lContextSetCount-- ;
}

/*
ContextSet& ContextSet::operator=(ContextSet& src)
{
	vider() ;

	if (!src.empty())
		for (ContextCIter c = src.begin() ; c != src.end() ; c++)
			push_back(new Context(**c)) ;
	return (*this) ;
}
*/

#ifndef __TCONTROLER__
EventSet::EventSet()
{
}

EventSet::EventSet(const EventSet& src)
{
	if (!src.empty())
		for (EventCIter e = src.begin() ; e != src.end() ; e++)
			push_back(new BB1Event(**e)) ;
}

EventSet::~EventSet()
{
	vider() ;
}

/*
EventSet& EventSet::operator=(EventSet& src)
{
	vider() ;

	if (!src.empty())
		for (EventIter e = src.begin() ; e != src.end() ; e++)
			push_back(new BB1Event(**e)) ;
	return (*this) ;
}
*/


FocusSet::FocusSet()
{
}


FocusSet::FocusSet(const FocusSet& src)
{
  if (!src.empty())
    for (FocusCIter f = src.begin() ; f != src.end() ; f++)
      push_back(new BB1Focus(**f)) ;
}


FocusSet::~FocusSet()
{
	vider() ;
}

#endif

/*
FocusSet& FocusSet::operator=(FocusSet& src)
{
	vider() ;

	if (!src.empty())
		for (FocusIter f = src.begin() ; f != src.end() ; f++)
			push_back(new BB1Focus(**f)) ;
	return (*this) ;
}
*/

KBSet::KBSet()
{
  _lKbSetCount++ ;
}

KBSet::KBSet(const KBSet& src)
{
  _lKbSetCount++ ;

	if (false == src.empty())
		for (KBCIter kb = src.begin() ; src.end() != kb ; kb++)
			push_back(new BB1KB(**kb)) ;
}

KBSet::~KBSet()
{
	vider() ;

  _lKbSetCount-- ;
}

/*
KBSet& KBSet::operator=(KBSet& src)
{
	if (!src.empty())
		for (KBIter kb = src.begin() ; kb != src.end() ; kb++)
			push_back(new BB1KB(**kb)) ;
	return (*this) ;
}
*/

KSSet::KSSet()
{
  _lKsSetCount++ ;
}

KSSet::KSSet(const KSSet& src)
{
  _lKsSetCount++ ;

	if (false == src.empty())
		for (KSCIter ks = src.begin() ; src.end() != ks ; ks++)
			push_back(new BB1KS(**ks)) ;
}

KSSet::~KSSet()
{
	vider() ;

  _lKsSetCount-- ;
}

/*
KSSet& KSSet::operator=(KSSet& src)
{
	vider() ;
	if (!src.empty())
		for (KSIter ks = src.begin() ; ks != src.end() ; ks++)
			push_back(new BB1KS(**ks)) ;
	return (*this) ;
}
*/

AKSSet::AKSSet()
{
  _lAKSSetCount++ ;
}

AKSSet::AKSSet(const AKSSet& src)
{
  _lAKSSetCount++ ;

	if (false == src.empty())
		for (AKSCIter aks = src.begin() ; src.end() != aks ; aks++)
			push_back(new AKS(**aks)) ;
}

AKSSet::~AKSSet()
{
	vider() ;

  _lAKSSetCount-- ;
}

#ifndef __TCONTROLER__
KSARSet::KSARSet()
{
}


KSARSet::KSARSet(const KSARSet& src)
{
  if (!src.empty())
    for (KSARCIter ksar = src.begin() ; ksar != src.end() ; ksar++)
			push_back(*ksar) ;
}


KSARSet::~KSARSet()
{
	if (!empty())
		clear() ;
}


KSARSet&
KSARSet::operator=(KSARSet& src)
{
	if (this == &src)
		return *this ;

	if (!empty())
		clear() ;

	if (!src.empty())
		for (KSARIter ksar = src.begin() ; ksar != src.end() ; ksar++)
			push_back(*ksar) ;

	return (*this) ;
}


KSARSet&	KSARSet::operator-=(KSARSet& src)
{
	KSARSet	t(*this) ;

	if (!empty())
		clear() ;

	for (KSARIter p = t.begin() ; p != t.end() ; p++)
	{
		BB1KSAR *e = (*p) ;
		if (!src.seek(e))
			push_back(e) ;
	}

	t.clear() ;

	return (*this) ;
}

#endif

LinkSet::LinkSet()
{
  _lLinkSetCount++ ;
}

LinkSet::LinkSet(const LinkSet& src)
{
  _lLinkSetCount++ ;

	if (false == src.empty())
		for (LinkCIter l = src.begin() ; src.end() != l ; l++)
			push_back(*l) ;
}

LinkSet::~LinkSet()
{
	if (false == empty())
		clear() ;

  _lLinkSetCount-- ;
}

LinkSet&
LinkSet::operator=(const LinkSet& src)
{
	if (this == &src)
		return *this ;

  if (false == empty())
		clear() ;

	if (false == src.empty())
		for (LinkCIter l = src.begin() ; src.end() != l ; l++)
			push_back(*l) ;

	return (*this) ;
}

LinkSet&
LinkSet::operator|=(const LinkSet& src)
{
	if ((&src && (false == src.empty())) && (&src != this))
	{
		for (LinkCIter p = src.begin() ; src.end() != p ; p++)
			push_back(*p) ;
	}
	return (*this) ;
}

LinkPairSet::LinkPairSet()
{
  _lLinkPairSetCount++ ;
}

LinkPairSet::LinkPairSet(const LinkPairSet& src)
{
  _lLinkPairSetCount++ ;

  if (false == src.empty())
    for (LinkPairCIter l = src.begin() ; src.end() != l ; l++)
			push_back(*l) ;
}

LinkPairSet::~LinkPairSet()
{
	if (false == empty())
		clear() ;

  _lLinkPairSetCount-- ;
}

LinkPairSet& LinkPairSet::operator=(const LinkPairSet& src)
{
	if (this == &src)
		return *this ;

	if (false == empty())
		clear() ;

	if (false == src.empty())
		for (LinkPairCIter l = src.begin() ; src.end() != l ; l++)
			push_back(*l) ;

	return (*this) ;
}


ObjectSet::ObjectSet()
{
  _lObjectSetCount++ ;
}

ObjectSet::ObjectSet(const ObjectSet& src)
{
  _lObjectSetCount++ ;

	if (src.empty())
    return ;

  for (ObjectCIter o = src.begin() ; src.end() != o ; o++)
  {
    const BB1Class *pClass = dynamic_cast<const BB1Class *>(*o) ;
    if (pClass)
    {
      BB1Class *pTemp = new BB1Class(*pClass) ;
      push_back(pTemp) ;
    }

#ifndef __TCONTROLER__

    const BB1Event *pEvent = dynamic_cast<const BB1Event *>(*o) ;
    if (pEvent)
      push_back(new BB1Event(*pEvent)) ;

    const BB1Focus *pFocus = dynamic_cast<const BB1Focus *>(*o) ;
    if (pFocus)
      push_back(new BB1Focus(*pFocus)) ;

    const BB1KSAR *pKSAR = dynamic_cast<const BB1KSAR *>(*o) ;
    if (pKSAR)
      push_back(new BB1KSAR(*pKSAR)) ;

    const BB1Strategy *pStrategy = dynamic_cast<const BB1Strategy *>(*o) ;
    if (pStrategy)
      push_back(new BB1Strategy(*pStrategy)) ;

    const BB1KS *pKS = dynamic_cast<const BB1KS *>(*o) ;
    if (pKS)
      push_back(new BB1KS(*pKS)) ;

#endif

    //FIXME

    const BB1Message *pMessage = dynamic_cast<const BB1Message *>(*o) ;
    if (pMessage)
    {
      BB1Message *pNewMessage = new BB1Message(*pMessage) ;
      push_back(pNewMessage) ;
    }
  }
}

/*
ObjectSet::ObjectSet(const ObjectList& src)
{
	if (!src.empty())
		for (ObjectCIter o = src.begin() ; o != src.end() ; o++)
		{
			const BB1Class *pClass = dynamic_cast<const BB1Class *>(*o) ;
			if (pClass)
				push_back(new BB1Class(*pClass)) ;

			const BB1Event *pEvent = dynamic_cast<const BB1Event *>(*o) ;
			if (pEvent)
				push_back(new BB1Event(*pEvent)) ;

			const BB1Focus *pFocus = dynamic_cast<const BB1Focus *>(*o) ;
			if (pFocus)
				push_back(new BB1Focus(*pFocus)) ;

			const BB1KS *pKS = dynamic_cast<const BB1KS *>(*o) ;
			if (pKS)
				push_back(new BB1KS(*pKS)) ;

			const BB1KSAR *pKSAR = dynamic_cast<const BB1KSAR *>(*o) ;
			if (pKSAR)
				push_back(new BB1KSAR(*pKSAR)) ;

			const BB1Message *pMessage = dynamic_cast<const BB1Message *>(*o) ;
			if (pMessage)
				push_back(new BB1Message(*pMessage)) ;

			// derivate from BB1Decision than derive from BB1Object
			const BB1Strategy *pStrategy = dynamic_cast<const BB1Strategy *>(*o) ;
			if (pStrategy)
				push_back(new BB1Strategy(*pStrategy)) ;
		}
}
*/

ObjectSet::~ObjectSet()
{
	vider() ;

  _lObjectSetCount-- ;
}

/*
void				ObjectSet::vider()
{
	if (empty())
		return ;
	for (ObjectIter iter = begin() ; iter != end() ; )
	{
		delete (*iter) ;
		erase(iter) ;
	}
	clear() ;
}
*/

ObjectSet&
ObjectSet::operator=(const ObjectSet& src)
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (ObjectCIter o = src.begin() ; src.end() != o ; o++)
		{
			const BB1Class *pClass = dynamic_cast<const BB1Class *>(*o) ;
			if (pClass)
      {
        BB1Class *pTemp = new BB1Class(*pClass) ;
				push_back(pTemp) ;
      }

      #ifndef __TCONTROLER__
			const BB1Event *pEvent = dynamic_cast<const BB1Event *>(*o) ;
      if (pEvent)
      	push_back(new BB1Event(*pEvent)) ;

      const BB1Focus *pFocus = dynamic_cast<const BB1Focus *>(*o) ;
      if (pFocus)
      	push_back(new BB1Focus(*pFocus)) ;

      const BB1KSAR *pKSAR = dynamic_cast<const BB1KSAR *>(*o) ;
      if (pKSAR)
      	push_back(new BB1KSAR(*pKSAR)) ;

        // derivate from BB1Decision than derive from BB1Object
      const BB1Strategy *pStrategy = dynamic_cast<const BB1Strategy *>(*o) ;
      if (pStrategy)
      	push_back(new BB1Strategy(*pStrategy)) ;

        const BB1KS *pKS = dynamic_cast<const BB1KS *>(*o) ;
      if (pKS)
      	push_back(new BB1KS(*pKS)) ;
      #endif

       //FIXME : KS

      const BB1Message *pMessage = dynamic_cast<const BB1Message *>(*o) ;
      if (pMessage)
      {
        BB1Message *pTemp = new BB1Message(*pMessage) ;
      	push_back(pTemp) ;
      }
    }

  return (*this) ;
}

