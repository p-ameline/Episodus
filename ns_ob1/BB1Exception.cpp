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
#include "BB1Exception.h"
#include "BB1BB.h"
#else
#include "ns_ob1\BB1Exception.h"
#include "ns_ob1\BB1BB.h"
#endif

// -----------------------------------------------------------------------------
// The constructor for class Exception calls this method. Therefore, when any
// subclass of Exception is instantiated, it is executed. If the Exception is
// thrown, other actions can be taken. Although the body is small, it is not
// implicitly inlined by inclusion in BB1Exception.h because that header file is
// included in a lot of places, and it should be possible to experiment with the
// body of this method without having to recompile everything.
// -----------------------------------------------------------------------------

void Exception::ExceptionAction(const BB1BB& bb)
{
  ErrHandler h = bb.ErrorHandler() ;
  (*h)(bb, Method(), Message()) ;
}


// -----------------------------------------------------------------------------
// This is the function that is used by the constructor BB1BB::BB1BB(...) to
// initialize the (default) error handler.
// -----------------------------------------------------------------------------

void DefaultErrorHandler(const BB1BB& bb, const string& method, const string& message)
{
  switch (bb.ErrorNotify())
  {
    case NO_NOTIFY:
      break;
    case ALARM:
    case WARN:
    {
#ifdef MFC
      // If using Microsoft Foundation Classes, display error in popup
      AfxMessageBox(Cstring(method.chars()) + Cstring(": ") + Cstring(message.chars())) ;
#else
      // Otherwise, display error in cerr
      cerr << method << ": " << message << "\n";
#endif
      break ;
    }
  }
}
