#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\button.h>
#include <owl\edit.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "nautilus\nssuper.h"
#include "partage\nsperson.h"
#include "nautilus\nsannexe.h"
#include "nsutil\nsdivers.h"
#include "partage\nsdivfct.h"
#include "nsoutil\v2_3pers.h"

#include "nsoutil\nsoutil.h"

#include "nsoutil\v2_3.rh"
#include "nsoutil\importDlg.h"
#include "nsoutil\importPat.h"
#include "nsoutil\importCorr.h"

// -----------------------------------------------------------------
//
//  Méthodes de NsCocciDialog
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsCocciDialog, TDialog)
    EV_CHILD_NOTIFY(IDC_CVT_PAT,  	BN_CLICKED, lanceCvtPat),
    EV_CHILD_NOTIFY(IDC_CVT_COR,  	BN_CLICKED, lanceCvtCor),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCocciDialog::NsCocciDialog(TWindow* pere, NSContexte* pCtx)
              :NSUtilDialog(pere, pCtx, "IMPORTATION_COCCI", pNSResModule)
{
    pChkPat    = new TCheckBox(this, IDC_CVT_PAT);
    pChkCor    = new TCheckBox(this, IDC_CVT_COR);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsCocciDialog::~NsCocciDialog()
{
	//
	// Suppression de tous les objets
	//
	delete pChkPat;
	delete pChkCor;
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCocciDialog::SetupWindow()
{
	TDialog::SetupWindow();
}

//---------------------------------------------------------------------------
//  Lancement de la conversion du fichier PATIENTS
//---------------------------------------------------------------------------
void
NsCocciDialog::lanceCvtPat()
{
#ifndef _MUE
    NsImportCocciPatientDlg* pNSPatCvtDlg = new NsImportCocciPatientDlg(pContexte->GetMainWindow(), pContexte);
    pNSPatCvtDlg->Execute();
#endif
}

//---------------------------------------------------------------------------
//  Lancement de la conversion du fichier CORRESPONDANTS
//---------------------------------------------------------------------------
void
NsCocciDialog::lanceCvtCor()
{
    NsImportCocciCorrDlg* pNSCorCvtDlg = new NsImportCocciCorrDlg(pContexte->GetMainWindow(), pContexte);
    pNSCorCvtDlg->Execute();
}

