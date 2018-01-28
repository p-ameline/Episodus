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
# pragma implementation
#endif

#ifndef FOR_WINDOWS
# include "BB1Class.h"
#else
# include "ns_ob1\BB1Class.h"
#endif

long BB1Class::_lBB1ClassCount = 0 ;

#ifdef __TCONTROLER__
BB1Class::BB1Class(const string& oname,
                   AttValPair&   dispatch,
                   BB1KB&        kb,
                   BB1Class*     c,
         /*const*/ AVPSet*       pAllowedAttributes)
         :BB1Object(oname, dispatch, kb, c)
{
  _lBB1ClassCount++ ;

#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1Class::BB1Class(\"" << oname << "\", k, *c, AllowedAttributes)\n\n" ;
#endif

  ProcessAttributes(pAllowedAttributes) ; // Classes can take any attributes
  // Class links (IsA) are handled specially at creation
}
#else
BB1Class::BB1Class(const string& oname,
                   BB1KB&        kb,
                   BB1Class*     c,
                   const AVPSet* AllowedAttributes)
         :BB1Object(oname, kb, c)
{
  _lBB1ClassCount++ ;

#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1Class::BB1Class(\"" << oname << "\", k, *c, AllowedAttributes)\n\n" ;
#endif

  ProcessAttributes(AllowedAttributes) ; // Classes can take any attributes
  // Class links (IsA) are handled specially at creation
}
#endif

// -----------------------------------------------------------------------------
// constructeur copie add fab
// -----------------------------------------------------------------------------
BB1Class::BB1Class(const BB1Class& bbClass)
         :BB1Object(bbClass)
{
  _lBB1ClassCount++ ;
//  attributes	= bbClass.attributes ;
//  strategyP	= bbClass.strategyP ;
//  links		= bbClass.links ;
}

BB1Class::~BB1Class(void)
{
#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1Class::~BB1Class()\n\n" ;
#endif
  _lBB1ClassCount-- ;
}

// -----------------------------------------------------------------------------
// Does this class allow its instances to have the named link?
// -----------------------------------------------------------------------------
bool
BB1Class::AllowsLinkP(const string sLinkName, const BB1Class* pToClass) const
{
  if (NULL == pToClass)
    return false ;

#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1Class::AllowsLinkP(\"" << linkName << "\"toClass) const\n\n" ;
#endif

  if (HasLinkP(sLinkName, pToClass))
    return true ; // base case

  // recursion
  ObjectList ds ;

  DirectSuperclasses(&ds) ;
  if (false == ds.empty())
    for (ObjectCIter fp = ds.begin() ; ds.end() != fp ; fp++)
    {
      BB1Class *fc = (BB1Class *)(*fp) ;
      if (fc->AllowsLinkP(sLinkName, pToClass))
        return true ; // "from" ancestor found
    }

  pToClass->DirectSuperclasses(&ds) ;

  if (false == ds.empty())
    for (ObjectCIter tp = ds.begin() ; ds.end() != tp ; tp++)
    {
      BB1Class *tc = (BB1Class *)(*tp) ;
      if (AllowsLinkP(sLinkName, tc))
        return true ; // "to" ancestor found
    }

  return false ; // no ancestor found
}

// -----------------------------------------------------------------------------
// If the attribute is bound locally, return a pointer to it; otherwise, if one
// of the class's superclasses binds the attribute locally, reutrn a pointer to
// that superclass's attribute; otherwise, return a null pointer.
// -----------------------------------------------------------------------------
AttValPair*
BB1Class::GetClassAttribute(const AttValPair& avp) const
{
#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1Class::GetClassAttribute(avp) const\n\n" ;
#endif

  AttValPair *lavp = GetLocalAttribute(avp) ;
	if (lavp)
		return (lavp) ; // base case

	// recursion: look for attribute in direct superclasses
	ObjectList ds ;
	DirectSuperclasses(&ds) ;

	if (false == ds.empty())
		for (ObjectCIter p = ds.begin() ; ds.end() != p ; p++)
		{
			BB1Class *c = (BB1Class *)(*p) ;
      lavp = c->GetClassAttribute(avp) ;
			if (lavp)
				return (lavp) ; // ancestor found
		}

	return (lavp) ; // no ancestor found
}

// -----------------------------------------------------------------------------
// Does the recipient have a sequence of "IsA" links to an object whose name is
// c? This is reflexive: a rose IsA rose.
// -----------------------------------------------------------------------------
bool
BB1Class::IsAP(const string sC) const
{
#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1Class::IsAP(\"" << sC << "\") const\n\n" ;
#endif

  if (Name() == sC)
    return true ; // base: reflexively true

  // recursion
  ObjectList directSuperclasses ;
  Objects(string("IsA"), &directSuperclasses) ;

  if (false == directSuperclasses.empty())
    for (ObjectCIter p = directSuperclasses.begin() ; directSuperclasses.end() != p ; p++)
    {
      if ((*p)->IsAP(sC))
        return true ; // ancestor found
    }

  return false ; // no ancestor found
}

// -----------------------------------------------------------------------------
// Delegate to a function that can call BB1BB::CreatePhaseCheck(...)
// -----------------------------------------------------------------------------

//#ifdef __TCONTROLER__
BB1AppInst*
BB1Class::MakeInstance(const string 	sName,
                       AttValPair&    dispatch,
                       BB1KB&         k,
                       AVPSet*        pAttSpec,
                       const LinkSet* pLinkSpec,
                       bool           bCreateToken,
                       bool           bCreateStrat)
{

//#ifdef _DEBUG_
 // trout << "-- DEBUG:: BB1Class::MakeInstance(\"" << Name << "\", k, attSpec, linkSpec)\n\n" ;
//#endif

  return MakeAppInst(*this, sName, dispatch, k, pAttSpec, pLinkSpec, bCreateToken, bCreateStrat) ;
}
/*#else
BB1AppInst *BB1Class::MakeInstance(const string&	Name,
				   BB1KB&		k,
				   const AVPSet		*attSpec,
				   const LinkSet	*linkSpec)
{

//#ifdef _DEBUG_
//  trout << "-- DEBUG:: BB1Class::MakeInstance(\"" << Name << "\", k, attSpec, linkSpec)\n\n" ;
//#endif

  return (MakeAppInst(*this, Name, k, attSpec, linkSpec)) ;
}
#endif                  */

void BB1Class::Save(ofstream& KBstream)
{
#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1Class::Save(KBstream)\n\n" ;
#endif

  KBstream << "(" ;
  SaveClass(KBstream) ;
  SaveName(KBstream) ;
  SaveAttributes(KBstream) ;
  KBstream << ")\n" ;
}

