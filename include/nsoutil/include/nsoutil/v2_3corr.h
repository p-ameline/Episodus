#ifndef __V2_3CORR_H
#define __V2_3CORR_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nsoutil\nscorlib.h"

class V2_3Correspondant
{
  	public :
   	//
	  	// Variables de stockage des valeurs
	  	//
	  	char V2code[4];
	  	char V2nom[26];
	  	char V2prenom[26];
	  	char V2adresse[51];
	  	char V2ville[26];
	  	char V2docteur[2];
	  	char V2telephone[16];
	  	char V2nb_exemp[3];
	  	char V2messagerie[36];
};

//
// Objet "Boite de dialogue" utilisé pour la transformation des patients
//
class _NSOUTILCLASSE NsCvtCorrespDlg : public NSUtilDialog
{
	public:

   	bool cvtPossible;
      bool cvtTourner;
#ifndef N_TIERS
      NSCorrespondant* pCorImport;
  		NSAdresses* 	  pAdrImport;#endif

      V2_3Correspondant* pBufCor;

      TEdit* pEditCode;
      TEdit* pEditNom;

      FILE*  infile;
      int 	 entree;

		NsCvtCorrespDlg(TWindow* pere, NSContexte* pCtx);
		~NsCvtCorrespDlg();

		void SetupWindow();

      //void CmOk();
		//void CmCancel();

      void demarreCvt();
      void stoppeCvt();

      bool emplisBuffer();
      bool traiteBuffer();

      void initAffichage();

	DECLARE_RESPONSE_TABLE(NsCvtCorrespDlg);
};

#endif // __V2_3CORR_H
