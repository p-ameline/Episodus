#ifndef __V2_3UTIL_H
#define __V2_3UTIL_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nscorlib.h"

class V2_3Utilisat
{
    public :
   	    //
	  	// Variables de stockage des valeurs
	  	//
        char V2indice[4];
	  	char V2code[7];
	  	char V2nom[26];
	  	char V2prenom[26];
	  	char V2adresse[51];
        char V2ville[26];
	  	char V2GParam[14];
        char V2CParam[10];
        char V2PParam[10];
        char V2WParam[7];
        char V2LParam[7];
	  	char V2dercod[9];
	  	char V2securite[21];
};

//
// Objet "Boite de dialogue" utilisé pour la transformation des patients
//
class _NSOUTILCLASSE NsCvtUtilisatDlg : public NSUtilDialog
{
    public:

   	    bool cvtPossible;
        bool cvtTourner;
#ifndef N_TIERS
        NSUtilisateur*   pUtiliImport;#endif

        V2_3Utilisat* pBufCor;

        TEdit* pEditCode;
        TEdit* pEditNom;

        FILE*  infile;
        int    entree;

		NsCvtUtilisatDlg(TWindow* pere, NSContexte* pCtx);
		~NsCvtUtilisatDlg();

		void SetupWindow();

        void demarreCvt();
        void stoppeCvt();

        bool emplisBuffer();
        bool traiteBuffer();

        void initAffichage();

    DECLARE_RESPONSE_TABLE(NsCvtUtilisatDlg);
};

#endif // __V2_3CORR_H
