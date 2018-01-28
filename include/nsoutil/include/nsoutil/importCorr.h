#ifndef __IMPORTCORR_H#define __IMPORTCORR_H

#include <classlib\arrays.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nscorlib.h"
// #include "nsbb\nspatbd.h"

class CocciCorresp
{
	public :
	  	//
	  	// Variables de stockage des anciennes valeurs
	  	//
   	    char CocciUser[7];
	  	char CocciTitre[11];
	  	char CocciNom[26];
	  	char CocciAdresse[51];
	  	char CocciAd1[51];
	  	char CocciCp[31];
	  	char CocciTel[21];
	  	char CocciFax[21];
	  	char CocciCompteur[12];
	  	char CocciSpecialite[31];
	  	char CocciTypeCorr[11];
};

//
// Objet "Boite de dialogue" utilisé pour la transformation des patients
//
class _NSOUTILCLASSE NsImportCocciCorrDlg : public NSUtilDialog
{
    public:

   	    bool                cvtPossible;
        bool                cvtTourner;

        // Pour la création de la fiche correspondant
        CocciCorresp*       pBufCorr;

        TEdit*              pEditCode;
        TEdit*              pEditNom;

        ifstream            inFile;
        string              sLine;

        bool                bAppend;
        string              cptCorresp;

		NsImportCocciCorrDlg(TWindow* pere, NSContexte* pCtx);
		~NsImportCocciCorrDlg();

		void SetupWindow();

        void demarreCvt();
        void stoppeCvt();

        bool emplisBuffer();
        bool traiteBuffer();

        void initAffichage();

    DECLARE_RESPONSE_TABLE(NsImportCocciCorrDlg);
};

#endif // __IMPORTCORR_H
