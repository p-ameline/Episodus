//---------------------------------------------------------------------------#include <owl\olemdifr.h>
#include <owl\applicat.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>

#include "nsdn\nsdn.h"#include "partage\Nsglobal.h"
#include "nautilus\nssuper.h"

#include "nsdn\nsdn_dlg.h"
#include "nsdn\nsdnres.h"

//***********************************************************************////							Classe NSEnregProgressDlg
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSEnregProgressDlg, NSUtilDialog)
END_RESPONSE_TABLE;

NSEnregProgressDlg::NSEnregProgressDlg(TWindow* parent, NSContexte* pCtx,
															TModule* /* module */)
                   :NSUtilDialog(parent, pCtx, "NSDN_ENREG", pNSResModule)
{
	pDataProgressBar = new TGauge(this, IDC_BAR_DATA) ;
	pLocProgressBar  = new TGauge(this, IDC_BAR_LOC) ;
}

NSEnregProgressDlg::~NSEnregProgressDlg(){
	delete pDataProgressBar ;
	delete pLocProgressBar ;
}

voidNSEnregProgressDlg::SetupWindow()
{
	TDialog::SetupWindow() ;

	pDataProgressBar->SetRange(0, 100) ;  pDataProgressBar->SetLed(4, 80) ;
  pDataProgressBar->SetStep(1) ;

  pLocProgressBar->SetRange(0, 100) ;
  pLocProgressBar->SetLed(4, 80) ;
  pLocProgressBar->SetStep(1) ;

  resetDataGauge() ;  resetLocGauge() ;
}

voidNSEnregProgressDlg::resetDataGauge()
{
	pDataProgressBar->SetValue(0) ;
}

voidNSEnregProgressDlg::resetLocGauge()
{
	pLocProgressBar->SetValue(0) ;
}

