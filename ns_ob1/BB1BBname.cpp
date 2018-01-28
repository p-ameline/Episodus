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
#include <stdio.h>      // for GenerateName(...)

#ifndef FOR_WINDOWS
#include "BB1BB.h"
#else
#include "ns_ob1\BB1BB.h"
#endif

// -----------------------------------------------------------------------------
// Return a copy of a unique name whose prefix is "Event-" and whose suffix is a
// serial enumerator.
// -----------------------------------------------------------------------------

string
BB1BB::GenerateEventName()
{
#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1BB::GenerateEventName()\n\n" ;
#endif

  return (GenerateName(EventPrefix, ++_eventCount)) ; // EventPrefix: global const BB1String
}



// -----------------------------------------------------------------------------
// Return a unique name whose prefix is "KSAR-" and whose suffix is a serial
// enumerator.
// -----------------------------------------------------------------------------

#ifndef __TCONTROLER__
string	BB1BB::GenerateKSARName()
{

#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::GenerateKSARName()\n\n" ;
#endif

  return (GenerateName(KSARPrefix, ++BB1BBKSARCount)) ; // KSARPrefix: global const BB1String
}
#endif



// -----------------------------------------------------------------------------
// Return a unique name whose prefix is "MESSAGE-" and whose suffix is a serial
// enumerator.
// -----------------------------------------------------------------------------

string	BB1BB::GenerateMessageName()
{
#ifdef _DEBUG_
  _trout << "-- DEBUG:: BB1BB::GenerateMessageName()\n\n" ;
#endif

  return (GenerateName(MessagePrefix, ++_messageCount)) ; // MessagePrefix: global const BB1String
}


// -----------------------------------------------------------------------------
// Construct and return a BB1String of the form <prefix>-<count>, where <prefix>
// is a BB1String and <count> is an integer.
// -----------------------------------------------------------------------------

string	GenerateName(const string& prefix, const int count)
{
  char	countStr[7] ;
  // sprintf() may return char * or int
  (void) sprintf(countStr, "%d", count) ;

  return (prefix + string(countStr)) ;
}
