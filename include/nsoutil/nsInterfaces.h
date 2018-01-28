#ifndef __NSINTERFACES_H#define __NSINTERFACES_H

#include <classlib\arrays.h>

#include <owl\listbox.h>

#include "nsoutil\nsexport.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class _NSOUTILCLASSE NsInterfacesDialog : public NSUtilDialog
{
	public:

    OWL::TListBox* pInterfacesBox ;

    map<string, string> *_paInterfaces ;
    bool _bMustDeleteMap ;

    string* psSelectedModuleName ;

	  NsInterfacesDialog(TWindow* pere, NSContexte* pCtx, string* psModuleName, map<string, string> *paInterfaces) ;
	  ~NsInterfacesDialog() ;

	  void SetupWindow() ;
    void CmOk() ;

    void initInterfacesList() ;
    void CmInterfaceDblClk(WPARAM Cmd) ;

	DECLARE_RESPONSE_TABLE(NsInterfacesDialog) ;
};

#endif  // __NSINTERFACES_H

