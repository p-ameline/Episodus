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
# include "BB1KSAR.h"
#else
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\BB1KSAR.h"
#endif


#ifndef __TCONTROLER__
///
/// FIXME : a rempalcer avec la procedure de TCOntroler
///
// The second half of the agenda manager


Errcode		BB1BB::ManageExecutableSubAgenda(void)
{

#ifdef _DEBUG_
  cout << "BB1BB::ManageExecutableSubAgenda()\n" ;
#endif

  // Obviate first, then check to see whether still executable.
  if (!agenda.ExecutableSubAgenda->empty())
    for (KSARCIter e = agenda.ExecutableSubAgenda->begin() ; e != agenda.ExecutableSubAgenda->end() ; e++)
    {
      BB1KSAR& k = *(*e) ;
      ksar = &k; // So that PC and OC can retrieve KSAR
      if (k.ExecutableCycle() == ThisCycle())
      {
				// It was just transferred from Triggered sub-agenda on this cycle
				// Avoid redundant precondition and obviation condition checks
      }
      else
				if (ObviatedP(k))
				{
					k.Obviate() ;
					agenda.ObviatedSubAgenda->push_back(&k) ;
				}
				else
					if (!ExecutableP(k))
					{
						k.Demote() ;
						agenda.TriggeredSubAgenda->push_back(&k) ;
	  			}
      ksar = KSARptr(0);
    }

  // remove the newly-obviated elements from the triggered sub-agenda
  *(agenda.ExecutableSubAgenda) -= *(agenda.ObviatedSubAgenda) ;

  // remove the newly-nonexecutable elements from the triggered sub-agenda
  *(agenda.ExecutableSubAgenda) -= *(agenda.TriggeredSubAgenda) ;

  return (SUCCESS) ;
}

#endif


#ifndef __TCONTROLER__
///
/// FIXME : a changer par les procedure du controleur
///
// The first half of the agenda manager

Errcode		BB1BB::ManageTriggeredSubAgenda()
{

#ifdef _DEBUG_
  cout << "BB1BB::ManageTriggeredSubAgenda()\n" ;
#endif

  // Obviate first, then check to see whether executable.
  if (!agenda.TriggeredSubAgenda->empty())
    for (KSARCIter t = agenda.TriggeredSubAgenda->begin() ; t != agenda.TriggeredSubAgenda->end() ; t++)
    {
      BB1KSAR& k = *(*t) ;
      ksar = &k ; // So that PC and OC can retrieve KSAR
      if (ObviatedP(k))
      {
				k.Obviate() ;
				agenda.ObviatedSubAgenda->push_back(&k) ;
      }
      else
				if (ExecutableP(k))
				{
					k.Promote() ;
					agenda.ExecutableSubAgenda->push_back(&k) ;
				}
      ksar = KSARptr(0);
    }

  // remove the newly-obviated elements from the triggered sub-agenda
  *(agenda.TriggeredSubAgenda) -= *(agenda.ObviatedSubAgenda) ;

  // remove the newly-executable elements from the triggered sub-agenda
  *(agenda.TriggeredSubAgenda) -= *(agenda.ExecutableSubAgenda) ;

  return (SUCCESS) ;
}

#endif
