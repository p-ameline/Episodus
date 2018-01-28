#ifndef __VCORLILL_H
#define __VCORLILL_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nscorlib.h"

class V2_3CorresLille
{
  	public :
   	//
	  	// Variables de stockage des valeurs
	  	//
	  	char V2code[24];
        char V2titre[11];
        char V2tit_suite[24];
	  	char V2nom[26];
	  	char V2prenom[15];
        char V2nom_ep[20];
	  	char V2adresse1[38];
	  	char V2adresse2[36];
	  	char V2ville[39];
	  	char V2lieu[7];
	  	char V2codepost[6];
};

//
// Objet "Boite de dialogue" utilisé pour la transformation des correspondants
// de l'Hôpital de Lille
//
class _NSOUTILCLASSE NsCvtCorrLilleDlg : public NSUtilDialog
{
	public:

   		bool cvtPossible;
      	bool cvtTourner;
#ifndef N_TIERS
      	NSCorrespondant* pCorImport;
  		NSAdresses* 	 pAdrImport;#endif

      	V2_3CorresLille* pBufCor;

      	TEdit* pEditCode;
      	TEdit* pEditNom;

      	ifstream infile;
      	int 	 entree;

		NsCvtCorrLilleDlg(TWindow* pere, NSContexte* pCtx);
		~NsCvtCorrLilleDlg();

		void SetupWindow();

      	//void CmOk();
		//void CmCancel();

      	void demarreCvt();
      	void stoppeCvt();

      	bool emplisBuffer();
      	bool traiteBuffer();

      	void initAffichage();

	DECLARE_RESPONSE_TABLE(NsCvtCorrLilleDlg);
};

#endif // __VCORLILL_H

