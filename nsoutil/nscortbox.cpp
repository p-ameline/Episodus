#include <owl\olemdifr.h>
#include <owl\applicat.h>
#include <owl\dialog.h>
#include <stdio.h>
#include <string.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nsbb\nsutidlg.h"

#include "nsoutil\nscortbox.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsoutil.rh"

// -----------------------------------------------------------------
//
//  Méthodes de NsCorToolBoxDialog
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsCorToolBoxDialog, NSUtilDialog)
	EV_COMMAND(IDOK, CmOk),
  EV_BN_CLICKED(MAIL_BTN, exportMailingList),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsCorToolBoxDialog::NsCorToolBoxDialog(TWindow* pere, NSContexte* pCtx)
	                 :NSUtilDialog(pere, pCtx, "CORTOOLBOX", pNSResModule)
{
  pMailingExport = new OWL::TButton(this, MAIL_BTN) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsCorToolBoxDialog::~NsCorToolBoxDialog()
{
	delete pMailingExport ;
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsCorToolBoxDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
}

void
NsCorToolBoxDialog::CmOk()
{
	CloseWindow(IDOK) ;
}

void
NsCorToolBoxDialog::exportMailingList()
{
}

// fin de nscortbox.cpp ................................
///////////////////////////////////////////////////////

