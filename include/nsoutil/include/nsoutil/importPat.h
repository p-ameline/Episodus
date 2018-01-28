#ifndef __IMPORTPAT_H#define __IMPORTPAT_H

#include <classlib\arrays.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nscorlib.h"
// #include "nsbb\nspatbd.h"

#ifndef _MUE
class NSPatientImport : public NSPatient
{
    public :

	    NSPatientImport(NSContexte* pCtx);
	    ~NSPatientImport() {}

	    bool creeTout();
        bool CreeChemiseDefaut(string sNomChemise);
        bool CreePOMRindex();
        bool CreeDocumentZero();
};
#endif

class CocciPatient
{
	public :
	  	//
	  	// Variables de stockage des anciennes valeurs
	  	//
   	    char CocciNom[21];
	  	char CocciNumpa[12];
	  	char CocciPrenom[21];
	  	char CocciNomJF[21];
	  	char CocciDateNaiss[11];
	  	char CocciAdresse[61];
	  	char CocciCp[6];
	  	char CocciVille[21];
	  	char CocciTel_dom[16];
	  	char CocciTel_bur[16];
	  	char CocciNoSecu[21];
	  	char CocciSexe[2];
	  	char CocciActivite[26];
        char CocciALD[2];
        char CocciSit_fam[11];
        char CocciGrSng[4];
        char CocciTaille[6];
        char CocciPoids[6];
        char CocciPouls[6];
        char CocciTaMin[6];
        char CocciTaMax[6];
        char CocciNbEnfant[3];
        char CocciAllergie1[21];
        char CocciAllergie2[21];
        char CocciAllergie3[21];
};

#ifndef _MUE
//
// Objet "Boite de dialogue" utilisé pour la transformation des patients
//
class _NSOUTILCLASSE NsImportCocciPatientDlg : public NSUtilDialog
{
    public:

   	    bool                cvtPossible;
        bool                cvtTourner;

        // Pour la création de la fiche patients
        NSPatientImport*    pPatImport;
  		NSPatCor*           pPcoImport;
  		NSAdresses*         pAdrImport;
        NSCorLibre*         pCLiImport;

        // Pour la création des chemises et documents
        NSChemise* 	        pNewChemise;
        NSDocument*         pNewDocument;
        NSPatPaLoc*         pLoca;
        NSPatPaDat*         pData;

        CocciPatient*       pBufPat;

		TEdit*              pEditNSS;
        TEdit*              pEditCode;
        TEdit*              pEditNom;
        TEdit*              pEditPrenom;

        ifstream            inFile;
        string              sLine;

        //FILE*  infile;
        //int    entree;
        bool                bAppend;
        string              cptHomme;
        string              cptFemme;

        char 	            creation[CHE_CREATION_LEN + 1];

		NsImportCocciPatientDlg(TWindow* pere, NSContexte* pCtx);
		~NsImportCocciPatientDlg();

		void SetupWindow();

        //void CmOk();
		//void CmCancel();

        void demarreCvt();
        void stoppeCvt();

        bool emplisBuffer();
        bool traiteBuffer();
        bool initLastCodes();
        bool existePatient(string& nom, string& prenom, string& sexe1, string& date1, string& sexe2, string& date2);

        void initAffichage();

    DECLARE_RESPONSE_TABLE(NsImportCocciPatientDlg);
};
#endif

class DPIOPatient
{
	public :
	  	//
	  	// Variables de stockage des anciennes valeurs
	  	//
   	    char DPIONom[36];
	  	char DPIOPrenom[36];
        char DPIOSexe[2];
	  	char DPIODateNaiss[11];
        char DPIOCode[21];
};

#ifndef _MUE
//
// Objet "Boite de dialogue" utilisé pour la transformation des patients
//
class _NSOUTILCLASSE NsImportDPIOPatientDlg : public NSUtilDialog
{
    public:

   	    bool                cvtPossible;
        bool                cvtTourner;

        // Pour la création de la fiche patients
        NSPatientImport*    pPatImport;
  		NSPatCor*           pPcoImport;
  		NSAdresses*         pAdrImport;
        NSCorLibre*         pCLiImport;

        // Pour la création des chemises et documents
        NSChemise* 	        pNewChemise;
        NSDocument*         pNewDocument;
        NSPatPaLoc*         pLoca;
        NSPatPaDat*         pData;

        DPIOPatient*        pBufPat;

		TEdit*              pEditNSS;
        TEdit*              pEditCode;
        TEdit*              pEditNom;
        TEdit*              pEditPrenom;

        ifstream            inFile;
        string              sLine;

        //FILE*  infile;
        //int    entree;
        bool                bAppend;
        string              cptHomme;
        string              cptFemme;

        char 	            creation[CHE_CREATION_LEN + 1];

		NsImportDPIOPatientDlg(TWindow* pere, NSContexte* pCtx);
		~NsImportDPIOPatientDlg();

		void SetupWindow();

        //void CmOk();
		//void CmCancel();

        void demarreCvt();
        void stoppeCvt();

        bool emplisBuffer();
        bool traiteBuffer();
        bool initLastCodes();
        bool existePatient(string& nom, string& prenom, string& sexe1, string& date1, string& sexe2, string& date2);

        void initAffichage();

    DECLARE_RESPONSE_TABLE(NsImportDPIOPatientDlg);
};
#endif

#endif // __IMPORTPAT_H

