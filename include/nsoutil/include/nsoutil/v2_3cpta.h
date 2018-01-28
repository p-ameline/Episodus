#ifndef __V2_3CPTA_H
#define __V2_3CPTA_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "nscompta\nscpta.h"

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nscorlib.h"

class V2_3Compt
{
    public :
   	    //
	  	// Variables de stockage des valeurs
	  	//
	  	char V2date[9];
	  	char V2patient[7];
        char V2sommedue[9];
        char V2sommepayee[9];
        char V2depassmnt[9];
        char V2nomencl[5];
        char V2typexam[2];
        char V2okpaye[2];
        char V2prescript[4];
        char V2hospext[2];
        char V2code[11];
        char V2operateur[4];
};

class V2_3Fact
{
    public :
   	    //
	  	// Variables de stockage des valeurs
	  	//
	  	char V2code[];
        char V2date[9];
        char V2sommepayee[];
        char V2modepaye[];
        char V2operateur[];
};

//
// Objet "Boite de dialogue" utilisé pour la transformation des patients
//
class _NSOUTILCLASSE NsCvtComptDlg : public NSUtilDialog
{
    public:

   	    bool cvtPossible;
        bool cvtTourner;

        bool cvtGlobale;

        NSCompt*   pComptImport;
        NSFact*    pFactImport;

        V2_3Compt* pBufCompt;
        V2_3Fact*  pBufFact;

        TEdit*     pEditCode;
        TEdit*     pEditNom;

        FILE*  infile;
        int    entree;

		NsCvtComptDlg(TWindow* pere, NSContexte* pCtx, bool cvtGlob);
		~NsCvtComptDlg();

		void SetupWindow();

        void demarreCvt();
        void stoppeCvt();

        bool emplisBufferCompt();
        bool traiteBufferCompt();
        bool emplisBufferFact();
        bool traiteBufferFact();

        bool traiteBufferCompt(string sUti);
        bool traiteBufferFact(string sUti);

        void initAffichage();

    DECLARE_RESPONSE_TABLE(NsCvtComptDlg);
};

#endif // __V2_3CPTA_H
