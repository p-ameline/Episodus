/*  +-----------------------------------------------------------------+  */
/*  ¦                PROGRAMME SECONDAIRE : DGEXGLOB.C                ¦  */
/*  ¦                                                                 ¦  */
/*  ¦        Gastroscopie : Classe mère des décodeurs de lésions      ¦  */
/*  ¦                                                                 ¦  */
/*  ¦             Début d'écriture : 17 Janvier 1989 (PA)             ¦  */
/*  ¦             Dernière version : 10 Août 1992 (PA)                ¦  */
/*  +-----------------------------------------------------------------+  */
#include <owl\olemdifr.h>
#include <stdio.h>
#include <string.h>
#include <cstring.h>
#include <stdlib.h>
#include <math.h>

#include "nssavoir\nssavoir.h"
#include "nsbb\nstlibre.h"
#include "dzz\dzzglob.h"
#include "nautilus\nssuper.h"

//
// ------------------- decLesion ----------------------------
//
decLesion::decLesion(NSContexte* pCtx, int iDecoType)
          :decodage(pCtx)
{
	iDcType  = iDecoType ;
	bInscrit = false ;
}

decLesion::decLesion(decodageBase* pBase, int iDecoType)
          :decodage(pBase)
{
	iDcType  = iDecoType ;
	bInscrit = false ;
}

void
decLesion::initPhrase()
{
	setDcodeur(string("")) ;
	return ;
}

void
decLesion::debutPhrase()
{
	return ;
}

void
decLesion::finPhrase()
{
	return ;
}

void
decLesion::BioLibLoc(int colonne)
{
	// int refCol = getCol() ;

	while ((getCol() > colonne) && iBon())
	{
/*
  	if 	  (*getSt() ==	"#TLI#")
    {
    	donnePhrase() ;
      NSTlibre NSTlibre(pContexte) ;
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
      	if (*getSt() == "£??")
        {
					bool bOK = NSTlibre.RecupereTexte(pContexte->getPatient()->getNss(), *getCpl(), sDcodeur()) ;
					Avance() ;
          if (bOK)
          	metPhrase() ;
				}
        else
        {
        	pContexte->getDico()->donneLibelle(sLangue, getStL(), sDcodeur()) ;
          Avance() ;
          *sDcodeur() += "." ;
					metPhrase() ;
        }
			}
		}
    else
    	Recupere() ;
*/
	}
  return ;
}

//
// ------------------- decSousLesion ----------------------------
//
decSousLesion::decSousLesion(NSContexte* pCtx, int iDecoType)
              :decodage(pCtx)
{
	iDcType  = iDecoType ;
	bInscrit = false ;
}

decSousLesion::decSousLesion(decodageBase* pBase, int iDecoType)
              :decodage(pBase)
{
	iDcType  = iDecoType ;
	bInscrit = false ;
}

void
decSousLesion::BioLibLoc(int colonne)
{
	int refCol = getCol() ;

	while ((getCol() > colonne) && iBon())
	{
  	if 	  (getSt() ==	"#TLI#")
    {
    	donnePhrase() ;
      // NSTlibre NSTlibre(pContexte) ;
      Avance() ;
      while ((getCol() > refCol) && iBon())
      {
      	if (getSt() == "£??")
        {
					// bool bOK = NSTlibre.RecupereTexte(pContexte->getPatient()->getNss(), *getCpl(), sDcodeur()) ;
					Avance() ;
          // if (bOK)
          	metPhrase() ;
				}
        else
        {
          setDcodeurFromLexique(getStL(), _sLangue) ;
          Avance() ;
          addToDcodeur(string(".")) ;
					metPhrase() ;
        }
			}
		}
    else
    	Recupere() ;
	}
  return ;
}
