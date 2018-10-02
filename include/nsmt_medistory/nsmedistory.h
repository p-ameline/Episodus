#ifndef __NSMEDISTORY_H#define __NSMEDISTORY_H

#include <owl\dialog.h>
#include <owl\button.h>

#include "nsbb\nsutidlg.h"

#if defined(_IN_EXE)
  #define _MEDICLASSE __import
#else
	#define _MEDICLASSE __export
#endif

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class _MEDICLASSE NsMedistoryToolBoxDialog : public NSUtilDialog
{
	public:

    OWL::TButton* pMailingExport ;

	  NsMedistoryToolBoxDialog(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
	  ~NsMedistoryToolBoxDialog() ;

	  void   SetupWindow() ;
    void   CmOk() ;

    void   exportMedistoryXml() ;
    void   processMedistoryXml(string sFileName) ;

    void   processPatient(string* psPatientInformation, int iNumPat) ;

    bool   gotoTag(string sTagName, FILE *inFile) ;
    string getTagContent(string sTagName, FILE *inFile) ;
    string getTagContent(string sTagName, string* psContent) ;
    string getNextTag(FILE *inFile) ;

	DECLARE_RESPONSE_TABLE(NsMedistoryToolBoxDialog) ;
};

#endif  // __NSMEDISTORY

