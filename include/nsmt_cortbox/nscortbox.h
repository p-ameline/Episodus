#ifndef __NSCORTBOX_H#define __NSCORTBOX_H

#include <owl\dialog.h>
#include <owl\button.h>

#include "nsbb\nsutidlg.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsCorToolBoxDialog : public NSUtilDialog
{
	public:

    OWL::TButton* pMailingExport ;

	  NsCorToolBoxDialog(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
	  ~NsCorToolBoxDialog() ;

	  void SetupWindow() ;
    void CmOk() ;

    void exportMailingList() ;

	DECLARE_RESPONSE_TABLE(NsCorToolBoxDialog) ;
};

#endif

