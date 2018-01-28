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
//#ifdef __GNUC__
# include <strstream> // for decToString(...)
//#endif // __GNUC__
#ifdef _MSC_VER
# include <strstrea.h>
#endif // _MSC_VER

#ifndef FOR_WINDOWS
# include "AttValPair.h"
# include "BB1BB.h"
# include "BB1Exception.h"    // add fab
#else
# include "ns_ob1\AttValPair.h"
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\BB1Exception.h"    // add fab
#endif

// Declare mapping of name String to function pointer

void	BB1BB::BB1BB_Intern(const string& /* n */, Voidptr /* p */, AVPSet& /* avpset */)
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1BB::BB1BB_Intern(\"" << n << "\", Voidptr, SetOfAVP &)\n" ;
#endif

  try
  {
    //AttValPair *avp = new AttValPair(n, p) ;
    //if (avp)
      //avpset.push_back(avp) ;
    //else
      //{ ConstructFailure cf(*this, "BB1BB_Intern(...)", "AttValPair") ; /* throw cf; */ }
  } // end try
  catch (ConstructFailure& cf)
    { } // constructor aborts
}



// Map name String to function pointer

void	*BB1BB::BB1BB_Lookup(const string& n, const AVPSet& avpset)
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: BB1BB::BB1BB_Lookup(\"" << n << "\", const SetOfAVP &)\n" ;
#endif

  if (avpset.empty())
    return (Voidptr(0)) ;

  for (AVPCIter p = avpset.begin() ; p != avpset.end() ; p++)
  {
    if ((*p)->AttributeName() == n)
      return ((*p)->AttributeValue().getPtr()) ;
  }

  return (Voidptr(0)) ;
}



// Construct a list of attribute-value pairs

AVPSet *Collect(AttValPair *a1,
                AttValPair *a2,
                AttValPair *a3,
                AttValPair *a4,
                AttValPair *a5,
                AttValPair *a6,
                AttValPair *a7,
                AttValPair *a8,
                AttValPair *a9,
                AttValPair *a10,
                AttValPair *a11,
                AttValPair *a12,
                AttValPair *a13,
                AttValPair *a14,
                AttValPair *a15,
                AttValPair *a16,
                AttValPair *a17,
                AttValPair *a18)
{
	AVPSet *retval = new AVPSet() ;

	if (a1 != 0)
		retval->push_back(new AttValPair(*a1)) ;

	if (a2  != 0)
		retval->push_back(new AttValPair(*a2)) ;

	if (a3  != 0)
		retval->push_back(new AttValPair(*a3)) ;

	if (a4  != 0)
		retval->push_back(new AttValPair(*a4)) ;

	if (a5  != 0)
		retval->push_back(new AttValPair(*a5)) ;

	if (a6  != 0)
		retval->push_back(new AttValPair(*a6)) ;

	if (a7  != 0)
		retval->push_back(new AttValPair(*a7)) ;

	if (a8  != 0)
		retval->push_back(new AttValPair(*a8)) ;

	if (a9  != 0)
		retval->push_back(new AttValPair(*a9)) ;

	if (a10 != 0)
		retval->push_back(new AttValPair(*a10)) ;

	if (a11 != 0)
		retval->push_back(new AttValPair(*a11)) ;

	if (a12 != 0)
		retval->push_back(new AttValPair(*a12)) ;

	if (a13 != 0)
		retval->push_back(new AttValPair(*a13)) ;

	if (a14 != 0)
		retval->push_back(new AttValPair(*a14)) ;

	if (a15 != 0)
		retval->push_back(new AttValPair(*a15)) ;

	if (a16 != 0)
		retval->push_back(new AttValPair(*a16)) ;

  if (a17 != 0)
		retval->push_back(new AttValPair(*a17)) ;

  if (a18 != 0)
		retval->push_back(new AttValPair(*a18)) ;

  // int i = retval->size();
  // Return a COPY
  return (retval) ;
}

// Construct a list of links

LinkSet	*Collect(BB1Link *a1,
                 BB1Link *a2,
                 BB1Link *a3,
                 BB1Link *a4,
                 BB1Link *a5,
                 BB1Link *a6,
                 BB1Link *a7,
                 BB1Link *a8,
                 BB1Link *a9,
                 BB1Link *a10,
                 BB1Link *a11,
                 BB1Link *a12,
                 BB1Link *a13,
                 BB1Link *a14,
                 BB1Link *a15,
                 BB1Link *a16,
                 BB1Link *a17,
                 BB1Link *a18)
{
  LinkSet *retval = new LinkSet() ;

  if (a1  != 0)
    retval->push_back(a1) ;

  if (a2  != 0)
    retval->push_back(a2) ;

  if (a3  != 0)
    retval->push_back(a3) ;

  if (a4  != 0)
    retval->push_back(a4) ;

  if (a5  != 0)
    retval->push_back(a5) ;

  if (a6  != 0)
    retval->push_back(a6) ;

  if (a7  != 0)
    retval->push_back(a7) ;

  if (a8  != 0)
    retval->push_back(a8) ;

  if (a9  != 0)
    retval->push_back(a9) ;

  if (a10 != 0)
    retval->push_back(a10) ;

  if (a11 != 0)
    retval->push_back(a11) ;

  if (a12 != 0)
    retval->push_back(a12) ;

  if (a13 != 0)
    retval->push_back(a13) ;

  if (a14 != 0)
    retval->push_back(a14) ;

  if (a15 != 0)
    retval->push_back(a15) ;

  if (a16 != 0)
    retval->push_back(a16) ;

  if (a17 != 0)
    retval->push_back(a17) ;

  if (a18 != 0)
    retval->push_back(a18) ;

  // Return a COPY
  return (retval) ;
}

// Construct a list of pointers to objects

ObjectList	*List(BB1Object *a1,
		      BB1Object *a2,
		      BB1Object *a3,
		      BB1Object *a4,
		      BB1Object *a5,
		      BB1Object *a6,
		      BB1Object *a7,
		      BB1Object *a8,
		      BB1Object *a9,
		      BB1Object *a10,
		      BB1Object *a11,
		      BB1Object *a12,
		      BB1Object *a13,
		      BB1Object *a14,
		      BB1Object *a15,
		      BB1Object *a16)
{
  ObjectList *retval = new ObjectList() ;

  if (a1  != 0)
    retval->append(a1) ;

  if (a2  != 0)
    retval->append(a2) ;

  if (a3  != 0)
    retval->append(a3) ;

  if (a4  != 0)
    retval->append(a4) ;

  if (a5  != 0)
    retval->append(a5) ;

  if (a6  != 0)
    retval->append(a6) ;

  if (a7  != 0)
    retval->append(a7) ;

  if (a8  != 0)
    retval->append(a8) ;

  if (a9  != 0)
    retval->append(a9) ;

  if (a10 != 0)
    retval->append(a10) ;

  if (a11 != 0)
    retval->append(a11) ;

  if (a12 != 0)
    retval->append(a12) ;

  if (a13 != 0)
    retval->append(a13) ;

  if (a14 != 0)
    retval->append(a14) ;

  if (a15 != 0)
    retval->append(a15) ;

  if (a16 != 0)
    retval->append(a16) ;

  // Return a COPY, not a reference
  return (retval) ;
}

/*
** Given up to 16 BB1String objects, construct a list of copies of those objects.
** The arguments are not references, since their default values are NULLSTRING,
** which is a const, and ListOfString::append() is not a const operation.
*/

StringList *List(string a1,
		      string a2,
		      string a3,
		      string a4,
		      string a5,
		      string a6,
		      string a7,
		      string a8,
		      string a9,
		      string a10,
		      string a11,
		      string a12,
		      string a13,
		      string a14,
		      string a15,
		      string a16)
{
  StringList *retval = new StringList() ;

  if (a1  != NULLSTRING)
    retval->append(a1) ;

  if (a2  != NULLSTRING)
    retval->append(a2) ;

  if (a3  != NULLSTRING)
    retval->append(a3) ;

  if (a4  != NULLSTRING)
    retval->append(a4) ;

  if (a5  != NULLSTRING)
    retval->append(a5) ;

  if (a6  != NULLSTRING)
    retval->append(a6) ;

  if (a7  != NULLSTRING)
    retval->append(a7) ;

  if (a8  != NULLSTRING)
    retval->append(a8) ;

  if (a9  != NULLSTRING)
    retval->append(a9) ;

  if (a10 != NULLSTRING)
    retval->append(a10) ;

  if (a11 != NULLSTRING)
    retval->append(a11) ;

  if (a12 != NULLSTRING)
    retval->append(a12) ;

  if (a13 != NULLSTRING)
    retval->append(a13) ;

  if (a14 != NULLSTRING)
    retval->append(a14) ;

  if (a15 != NULLSTRING)
    retval->append(a15) ;

  if (a16 != NULLSTRING)
    retval->append(a16) ;

  // Return a COPY, not a reference
  return (retval) ;
}

/*
** Is the BB1String a member of the list of BB1Strings?  Parameter s is a
** reference to a BB1String object and ListOfString is a list of BB1String objects.
*/

bool Member(const string& s, const StringList *list)
{
  if (list->empty())
    return (false) ;

  for (StrCIter p = list->begin() ; p != list->end() ; p++)
  {
    if (*p == s)
      return (true) ;
  }

  return (false) ;
}


/*
** Write int i to BB1String S, returning a reference to S.
*/

string&	decToString(string& S, const int i)
{
  // output string stream, to make use of << formatting
  ostrstream ost ;
  ost << i ;
  // extract the output string stream's character array
  char *s = ost.str() ;
  // convert from char * to BB1String
  S = string(s) ;
  // the user of ostrstream::str() is responsible for memory management
  delete s ;
  // supply useful return value
  return (S) ;
}

