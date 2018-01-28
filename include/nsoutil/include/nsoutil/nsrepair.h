#ifndef __NSREPAIR_H#define __NSREPAIR_H

#include <classlib\arrays.h>
#include <owl\dialog.h>
#include <owl\edit.h>

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"

#ifndef _MUE
//
// Objet "Boite de dialogue" utilisé pour la transformation des patients
//
class _NSOUTILCLASSE NsRepairDocumDlg : public NSUtilDialog
{
    public:

		TEdit*  pEditNSS;
        TEdit*  pEditCode;
        TEdit*  pEditNom;
        TEdit*  pEditPrenom;

        bool    bTourner ;

		NsRepairDocumDlg(TWindow* pere, NSContexte* pCtx);
		~NsRepairDocumDlg();

		void SetupWindow();

        //void CmOk();
		//void CmCancel();

        void repairStart();
        void repairStop();

    DECLARE_RESPONSE_TABLE(NsRepairDocumDlg);
};
#endif

#endif // __IMPORTPAT_H

