#ifndef __V2_3MAT_H
#define __V2_3MAT_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "partage\nsmatfic.h"

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nscorlib.h"

class V2_3Endoscop
{
    public :
   	    //
	  	// Variables de stockage des valeurs
	  	//
	  	char V2code[3];
	  	char V2libelle[13];
};

class V2_3Echographe
{
    public :
   	    //
	  	// Variables de stockage des valeurs
	  	//
	  	char V2code[2];
	  	char V2libelle[36];
        char V2date[9];
};

//
// Objet "Boite de dialogue" utilisé pour la transformation des patients
//
class _NSOUTILCLASSE NsCvtMaterielDlg : public NSUtilDialog
{
    public:

   	    bool cvtPossible;
        bool cvtTourner;
#ifndef N_TIERS
        NSMateriel*     pMatImport;#endif

        V2_3Endoscop*   pBufEndo;
        V2_3Echographe* pBufEcho;

        TEdit* pEditCode;
        TEdit* pEditNom;

        FILE*  infile;
        int    entree;

		NsCvtMaterielDlg(TWindow* pere, NSContexte* pCtx);
		~NsCvtMaterielDlg();

		void SetupWindow();

        void demarreCvt();
        void stoppeCvt();

        bool emplisBufferEndo();
        bool traiteBufferEndo();

        bool emplisBufferEcho();
        bool traiteBufferEcho();

        void initAffichage();

    DECLARE_RESPONSE_TABLE(NsCvtMaterielDlg);
};

#endif // __V2_3CORR_H
