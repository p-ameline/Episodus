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

# include "AttValPair.h"   // for BB1BB::CreateEvent(...)
# include "BB1Link.h"      // for BB1BB::CreateEvent(...)
# include "BB1BB.h"
# include "BB1Event.h"     // for BB1BB::CreateEvent(...)
# include "BB1KB.h"        // for BB1BB::CreateEvent(...)
# include "BB1Exception.h"    // add fab

#else

# include "ns_ob1\AttValPair.h"   // for BB1BB::CreateEvent(...)
# include "ns_ob1\BB1Link.h"      // for BB1BB::CreateEvent(...)
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\BB1Event.h"     // for BB1BB::CreateEvent(...)
# include "ns_ob1\BB1KB.h"        // for BB1BB::CreateEvent(...)
# include "ns_ob1\BB1Exception.h"    // add fab
# include "ns_ob1\OB1Controler.h"
# include "ns_ob1\OB1Token.h"
# include "ns_ob1\Interface.h"

#  ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
#  else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#  endif

#endif

// -----------------------------------------------------------------------------
// Create an event object and place it in the event knowledge base. Called from
// BB1BB::ProposeFocus(...), BB1BB::ProposeStrategy(...), ::ModifyObject(...),
// BB1BB::NewAppInst(...), BB1BB::CreateMessage(...), BB1Decision::TurnOff(...),
// BB1Strategy::{AdvanceStrategicGenerator(...), RemoveCurrentPrescription(...)}.
// -----------------------------------------------------------------------------
// Do NOT create an event when any of the following are created: BB1Event,
// BB1KSAR.
// -----------------------------------------------------------------------------
// LSB: Should there be an event when the following are created: BB1Class,
// BB1KS?
// -----------------------------------------------------------------------------
int
BB1BB::CreateToken(EventType /* type */, BB1Object& object, const AVPSet *attSpec, const LinkSet *linkSpec, bool createStrat)
{
  // attribut ayant changé
	//
  AVPSet *changedAttributes = new AVPSet() ;
  if (attSpec && (false == attSpec->empty()))
		*changedAttributes |= *attSpec ;

  // Check les liens qui ont changé
  //
  LinkSet *changedLinks = new LinkSet() ;
  if (linkSpec && (false == linkSpec->empty()))
		*changedLinks |= *linkSpec ;

  OB1Token* temp = _kenoby.createToken(ADD, &object, changedAttributes, changedLinks) ;
  bool bDispatchSucceeded = _kenoby.Dispatch(temp, createStrat) ;

  if (false == bDispatchSucceeded)
  {
    string sActionLog = string("Deleting token ") + IntToString(temp->getNumero()) ;
    if (pContexte->getBBinterface())
      pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

    // _kenoby->deleteToken(temp) ;
  }

	return 0 ;
}

