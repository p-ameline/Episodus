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
# include "AttValPair.h"
# include "BB1KB.h"
# include "BB1Message.h" // for BB1BB::CreateMessage(...)
#else
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\AttValPair.h"
# include "ns_ob1\BB1KB.h"
# include "ns_ob1\BB1Message.h" // for BB1BB::CreateMessage(...)
#endif

class BB1Class ;


// -----------------------------------------------------------------------------
// Create an event object and place it in the event knowledge base.
// -----------------------------------------------------------------------------

#ifdef __IPC__
BB1Message*
BB1BB::CreateMessage(const string& messageString)
{

#ifdef _DEBUG_
  trout << "BB1BB::CreateMessage(\"" << messageString << "\")\n" ;
#endif

  try
  {
    BB1Message *message = (BB1Message *) NULL ;
    BB1Class	*c = ClassNamed("Message") ;

    if (c)
    {
      // make message object
      AttValPair	avp("message", messageString) ;
      SetOfAVP		attSpec = Collect(&avp) ;

      // Memory freed in BB1KB::~BB1KB
      message = new BB1Message(GenerateMessageName(), *messageKB, c, attSpec) ;
      delete attSpec ;

      if (message)
      {
        #ifndef __TCONTROLER__
        // FIXME
				// Always create an event for a message
				CreateEvent(ADD, *message, attSpec);
        #else
       // CreateToken();
        #endif

				// Print trace message if enabled
				if (TraceMessageP())
					{ trout << "Creating message " << *message << "\n" ; }
      }
      else
      {
				ConstructFailure cf(*this, "BB1BB::CreateMessage(...)", "BB1Messge") ;
				return ((BB1Message *) NULL) ; // throw cf;
      }
    }
    else
    {
      ClassNotFound cnf(*this, "BB1BB::CreateMessage(...)", "Message") ;

      return ((BB1Message *) NULL) ; // throw cnf;
    }
    return (message) ;
  } // end try
  catch (ConstructFailure& cf)
    { } // constructor aborts
  catch (ClassNotFound& cnf)
    { } // constructor aborts
  
  return ((BB1Message *) NULL) ;
}

#endif // __IPC__
