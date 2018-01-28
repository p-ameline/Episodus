#ifndef __V2_3PAT_H
#define __V2_3PAT_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nscorlib.h"
#include "nsbb\nspatbd.h"

class V2_3Patient
{
	public :
	  	//
	  	// Variables de stockage des anciennes valeurs
	  	//
   	    char V2nss[7];
	  	char V2dernier[15];
	  	char V2nom[26];
	  	char V2prenom[26];
	  	char V2convoc[21];
	  	char V2adresse[51];
	  	char V2ville[26];
	  	char V2medic[6][4];
	  	char V2medinom[26];
	  	char V2medidres[26];
	  	char V2mediville[26];
	  	char V2naissance[9];
	  	char V2telephone[16];
	  	char V2code[21];
};
#ifndef _MUE//
// Objet dérivé de NSFiche servant aux accès de base de données
//

class NSPatBug : public NSPatient{

    public :
	    NSPatBug(NSContexte* pCtx) : NSPatient(pCtx) {}	    //~NSPatBug();

	    // void metABlanc() { pDonnees->metABlanc(); }
	    DBIResult open();};
#endif

#ifndef _MUE
//// Objet "Boite de dialogue" utilisé pour la transformation des patients
//

class _NSOUTILCLASSE NsCvtPatientDlg : public NSUtilDialog
{
    public:

   	    bool cvtPossible;
        bool cvtTourner;

        // Pour la création de la fiche patients
        NSPatient*  pPatImport;
  		NSPatCor*   pPcoImport;
  		NSAdresses* pAdrImport;
        NSCorLibre* pCLiImport;
        NSPatBug*   pPatBug;
        // Pour la création des chemises et documents
        NSChemise* 	pNewChemise;
        NSDocument* pNewDocument;
        NSPatPaLoc* pLoca;
        NSPatPaDat* pData;

        V2_3Patient* pBufPat;

		TEdit* pEditNSS;
        TEdit* pEditCode;
        TEdit* pEditNom;
        TEdit* pEditPrenom;

        FILE*  infile;
        int    entree;
        bool   bAppend;
        string cptHomme;
        string cptFemme;

        char 	 creation[CHE_CREATION_LEN + 1];

		NsCvtPatientDlg(TWindow* pere, NSContexte* pCtx, bool bModeAppend = false);
		~NsCvtPatientDlg();

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

    DECLARE_RESPONSE_TABLE(NsCvtPatientDlg);
};#endif

#ifndef _MUE
//// Objet "Boite de dialogue" utilisé pour la transformation des patients
//

class _NSOUTILCLASSE NsReparePatientDlg : public NSUtilDialog
{
    public:

        bool cvtPossible;
        bool cvtTourner;

        // Pour la création de la fiche patients
        NSPatient*     pPatImport;

        NSPatientData* pBufPat;

		TEdit* pEditNSS;
        TEdit* pEditCode;
        TEdit* pEditNom;
        TEdit* pEditPrenom;

        FILE*  infile;
        int    entree;

        char   creation[CHE_CREATION_LEN + 1];

		NsReparePatientDlg(TWindow* pere, NSContexte* pCtx);
		~NsReparePatientDlg();

		void SetupWindow();

        //void CmOk();
		//void CmCancel();

        void demarreCvt();
        void stoppeCvt();

        bool vaAuPremier();
        bool emplisBuffer();
        bool traiteBuffer();

        void initAffichage();

    DECLARE_RESPONSE_TABLE(NsReparePatientDlg);
};#endif
#endif // __V2_3PAT_H
