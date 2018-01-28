#ifndef __NSPATBOX_H#define __NSPATBOX_H

#include <owl\dialog.h>
#include <owl\button.h>

#include "nsbb\nsutidlg.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsPatToolBoxDialog : public NSUtilDialog
{
	public:

    OWL::TButton* pMailingExport ;

	  NsPatToolBoxDialog(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
	  ~NsPatToolBoxDialog() ;

	  void SetupWindow() ;
    void CmOk() ;

    void exportMailingList() ;

	DECLARE_RESPONSE_TABLE(NsPatToolBoxDialog) ;
};

#endif

