#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"

//
// Objet "Boite de dialogue" utilisé pour choisir la table à importer
//
class _NSOUTILCLASSE NsCvtDialog : public NSUtilDialog
{
    public:

        TCheckBox*	pChkPat;
        TCheckBox*	pChkCor;
        TCheckBox*	pChkUti;
        TCheckBox*	pChkMat;
        TCheckBox*  pChkPatSup;
        TCheckBox*  pChkAutre;

		NsCvtDialog(TWindow* pere, NSContexte* pCtx);
		~NsCvtDialog();

		void SetupWindow();

        void lanceCvtPat();
        void lanceCvtCor();
        void lanceCvtUti();
        void lanceCvtMat();
        void lanceCvtPatSup();
        void lanceCvtAutre();

    DECLARE_RESPONSE_TABLE(NsCvtDialog);
};

//
// Objet "Boite de dialogue" utilisé pour les conversions spécifiques
//
class _NSOUTILCLASSE NsCvtAutreDlg : public NSUtilDialog
{
    public:

        TCheckBox*	pChkLille;

		NsCvtAutreDlg(TWindow* pere, NSContexte* pCtx);
		~NsCvtAutreDlg();

		void SetupWindow();

        void lanceCvtLille();

    DECLARE_RESPONSE_TABLE(NsCvtAutreDlg);
};

class V2_3Dialog : public TDialog
{

    public:

        NSContexte* pContexte;

	    V2_3Dialog(TWindow* pere, NSContexte* pCtx);

	    void CmModificationPatient(void);
	    void CmModificationCorrespondant(void);
	    void CmModificationComptesRendus(void);
	    void CmInitialisationDocuments(void);

	DECLARE_RESPONSE_TABLE(V2_3Dialog);
};


