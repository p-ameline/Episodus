//nsdzz.cpp : DLL de decodage
///////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\window.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring.h>

#define _MAIN
#define __MAIN

#include "partage\nsglobal.h"

#ifdef _INCLUS
  #include "nsbb\nsutidlg.h"
#endif

#include "partage\nsdivfct.h"
#include "dzz\nsdzz.h"

#include "dzz\dzzdecod.h"

USEUNIT("dzz\Dzzentet.cpp");
USEUNIT("dzz\Dzzxglob.cpp");
USEUNIT("dzz\Dzzdecod.cpp");
USEUNIT("dzz\Dzzspeci.cpp");
USERC("dcodeur\nsdecode.rc");
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmbb.lib");
USELIB("nsmdn.lib");
USELIB("nsmdecode.lib");
USELIB("memmgr.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USERES("nsmdzz.res");
//---------------------------------------------------------------------------
string          decodage::_sExamen ;
int             decodage::_iCompteur ;
int             decodage::_iNiveau ;
NSDocumentData* decodage::_pDocument ;

int WINAPI DllEntryPoint(HINSTANCE hinstDll, unsigned long fdwReason, void*)
{
    switch (fdwReason)
    {
   	    case DLL_THREAD_ATTACH :
        case DLL_THREAD_DETACH :
      	    // cas des threads : on ne fait rien
      	    break;

        case DLL_PROCESS_ATTACH :
      	    // 1er appel : on instancie pResMod
      	    if (!pResMod)
   			    pResMod = new TModule(0, hinstDll);

            // si le new echoue : on renvoie false
      	    if (!pResMod)
      		    return 0;

      	    pNSDLLModule = pResMod;
            break;

        case DLL_PROCESS_DETACH :
      	    // dernier appel : on libere pResMod
       	    if (pResMod)
      	    {
      		    delete pResMod;
         	    pResMod = 0;
      	    }
            break;

        default :
      	    return 0;
    }
	return 1;
}

int
dcodeKE(char /* Commande */, NSPatPathoArray far *pPPtArray, char far *Fichier,
        NSContexte far *pCtx, NSCRPhraseArray far * /* pPhra */,
        NSDocumentData far *pDoc)
{
	//
	// Initialisation de pNSSuperviseur
	//
	pContexte  = pCtx ;
	//pPhrases 		  = pPhra;
	//pPhrases 		  = new NSCRPhraseArray;

	//
	// Initialisation des variables
	//
	decGeneral Decogen(pContexte) ;

	Decogen.setPLA(new NSCRPhraLesArray) ;
  Decogen.setDsFichier(true) ;
  Decogen.setPPtArray(pPPtArray) ;
  Decogen.initialiseIterateurs() ;

  decodage::_iCompteur = 0 ;
  decodage::_iNiveau   = decodage::iTexte ;
  decodage::_pDocument = pDoc ;

  decodage::_sExamen   = Decogen.getStL() ;
  Decogen.Avance() ;
  //
  // Ouverture du fichier
  //
	Decogen.ouvreFichier(Fichier) ;
	//
	// Fabrication du compte-rendu
	//
  Decogen.set_iBon(1) ;
  Decogen.setRecupFen(true) ;

	Decogen.decode() ;

	Decogen.fermeFichier() ;
	//*pPhra = *pPhrases;
	//delete pPhrases;

  int iCodeRetour ;
  if (Decogen.iBon())
  	iCodeRetour = 0 ;
  else
  	iCodeRetour = Decogen.getCorrAnnu() ;

	return iCodeRetour ;
}

// Lancement du décodage par morceaux
//
bool
dcodePart(char /* Commande */ , NSPatPathoArray far *pPPtArray, char far * /* Fichier */,
          NSContexte far *pCtx, NSCRPhraseArray far *pPhra,
          string far * /* pFonction */, int /* iDecoType */)
{
	//
	// Initialisation de pNSSuperviseur
	//
	pContexte = pCtx;
	//
	// Initialisation des variables
	//
  decGeneral Decogen(pContexte) ;

  Decogen.setPLA(new NSCRPhraLesArray) ;
  Decogen.setPhA(pPhra) ;
  Decogen.setPPtArray(pPPtArray) ;
  Decogen.initialiseIterateurs() ;
  Decogen.set_iBon(1) ;
  Decogen.setDsFichier(false) ;
  Decogen.setRecupFen(false) ;

  // int refCol = Decogen.getCol() ;

/*    if 	  (*pFonction == "KE_CR_TOUT")
    {
   	    pDecogen->decode();
    }
    else if (*pFonction == "KK_SOUFFLE")
    {
        if (*(pDecogen->getSt()) == "S0SOU")
        {
            pDecogen->Avance();
            decKESouffle* pSouf = new decKESouffle(pDecogen, dcTiret);
            pSouf->decode(refCol);
            delete pSouf;
            if ((pDecogen->iBon()) && !(pPhra->empty()))
                return true;
        }
        else
            pDecogen->Recupere();
    }
    else if (*pFonction == "KK_INTERV")
    {
        classeChir geste;

        decKEAnteChir* pChir = new decKEAnteChir(pDecogen, dcTiret);

        if 	    (*(pDecogen->getSt()) == "G0CHI")
        {
      	    pDecogen->Avance();
            pChir->decode(refCol);
        }
        else
        {
        	if 	    (*(pDecogen->getSt()) == "GPON0")
        	{
      	    	pDecogen->Avance();
            	pChir->chirPontage(refCol, "pontage", &geste);
        	}
        	else if (*(pDecogen->getSt()) == "GPO20")
        	{
      	    	pDecogen->Avance();
            	pChir->chirPontage(refCol, "pontage redux", &geste);
        	}
        	else if (*(pDecogen->getSt()) == "GANGC")
        	{
      	    	pDecogen->Avance();
            	pChir->chirAngioplastie(refCol, &geste);
        	}
        	else if (*(pDecogen->getSt()) == "GMXTO")
        	{
      	    	pDecogen->Avance();
            	pChir->chirMyotomie(refCol, &geste);
        	}
        	else if (*(pDecogen->getSt()) == "GT1CA")
        	{
      	    	pDecogen->Avance();
            	pChir->chirDivers(refCol, "transplantation cardiaque", &geste);
        	}
        	else if (*(pDecogen->getSt()) == "GDRPE")
        	{
      	    	pDecogen->Avance();
            	pChir->chirDivers(refCol, "drainage péricardique", &geste);
        	}
        	else if (*(pDecogen->getSt()) == "GSTIM")
        	{
      	    	pDecogen->Avance();
            	pChir->chirStimulateur(refCol, &geste);
        	}
        	else
            	pDecogen->Recupere();

        	*(pChir->sDcodeur()) = geste.chir_phrases + geste.chir_date_libelle;
        	pChir->metPhrase();
        }
        delete pChir;

        if ((pDecogen->iBon()) && !(pPhra->empty()))
            return true;
    } */

	return false ;
}
