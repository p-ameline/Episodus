//------------------------------------------------------------------------//   nspardlg : paramètres des éléments Patpatho
//   Kaddachi Hafedh le 04/09/1997 à 11:03:04
//------------------------------------------------------------------------
#include "partage\nsdivfct.h"
#include "nsbb\nspardlg.h"
#include "nsbb\nsmPids.rh"
#include "nsbb\tagNames.h"

DEFINE_RESPONSE_TABLE1(Parametre, TDialog)	EV_COMMAND(IDOK, 		CmOk),
	EV_COMMAND(IDCANCEL,	CmCancel),

  EV_CHILD_NOTIFY(IDD_0_VAL, BN_CLICKED, TraitementCertitude0Val),
  EV_CHILD_NOTIFY(IDD_25_VAL, BN_CLICKED, TraitementCertitude25Val),
  EV_CHILD_NOTIFY(IDD_50_VAL, BN_CLICKED, TraitementCertitude50Val),
  EV_CHILD_NOTIFY(IDD_75_VAL, BN_CLICKED, TraitementCertitude75Val),
  EV_CHILD_NOTIFY(IDD_100_VAL, BN_CLICKED, TraitementCertitude100Val),
  EV_CHILD_NOTIFY(IDD_ABSENT_VAL, BN_CLICKED, TraitementCertitudeAbsentVal),
  EV_CHILD_NOTIFY(IDD_PRESENT_VAL, BN_CLICKED, TraitementCertitudePresentVal),
  EV_CHILD_NOTIFY(IDD_EVENTUEL_VAL, BN_CLICKED, TraitementCertitudeEventuelVal),

  EV_CHILD_NOTIFY(IDD_0P, BN_CLICKED, TraitementInteret0P),
  EV_CHILD_NOTIFY(IDD_1P, BN_CLICKED, TraitementInteret1P),  EV_CHILD_NOTIFY(IDD_2P, BN_CLICKED, TraitementInteret2P),
  EV_CHILD_NOTIFY(IDD_3P, BN_CLICKED, TraitementInteret3P),
  EV_CHILD_NOTIFY(IDD_4P, BN_CLICKED, TraitementInteret4P),

END_RESPONSE_TABLE ;

//---------------------------------------------------------------------------//---------------------------------------------------------------------------
Parametre::Parametre(TWindow* parent , TResId resId, Message* pMessage,
                       NSTreeNode* pNstreeNode, TModule* module)
              :TDialog(parent, resId, module)
{
try
{
    ressource 			    = resId;
    AParent 			    = parent;
    pPatpatho			    = pMessage;
    pNSTreeNodeSelection    = pNstreeNode;

 	pCertitude 		    = new THSlider(this, IDC_CERTITUDE);
    pImportance         = new THSlider(this, IDC_INTERET); //interêt

    sPlurielDlg   = "";
    sInteretDlg   = "";
    sCertitudeDlg = "";

    pNSSlider0 	   	    = new TButton(this, IDD_0_VAL);
    pNSSlider25	        = new TButton(this, IDD_25_VAL);
    pNSSlider50		    = new TButton(this, IDD_50_VAL);
    pNSSlider75		    = new TButton(this, IDD_75_VAL);
    pNSSlider100	    = new TButton(this, IDD_100_VAL);
    pNSSliderAbsent     = new TButton(this, IDD_ABSENT_VAL);
    pNSSliderEventuel   = new TButton(this, IDD_EVENTUEL_VAL);
    pNSSliderPresent    = new TButton(this, IDD_PRESENT_VAL);

    pNSSlider0P    	    = new TButton(this, IDD_0P);
    pNSSlider1P   	    = new TButton(this, IDD_1P);
    pNSSlider2P   	    = new TButton(this, IDD_2P);
    pNSSlider3P   	    = new TButton(this, IDD_3P);
    pNSSlider4P   	    = new TButton(this, IDD_4P);

	pPlurielDes         = new TRadioButton(this, IDC_DES);
    pPlurielLes 	    = new TRadioButton(this, IDC_LES);
    pSingulier 		    = new TRadioButton(this, IDC_SINGULIER);
}
catch (...)
{
	erreur("Exception Parametre ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//---------------------------------------------------------------------------
Parametre::~Parametre()
{
    if (pCertitude)
   	    delete pCertitude;
    if (pImportance)
   	    delete pImportance;

    if (pPlurielDes)
   	    delete pPlurielDes;
  	if (pPlurielLes)
   	    delete pPlurielLes;
    if (pSingulier)
   	    delete pSingulier;
    if (pPatpatho)
   	    delete pPatpatho;

    delete pNSSlider0 ;
    delete pNSSlider25 ;
    delete pNSSlider50	;
    delete pNSSlider75	;
    delete pNSSlider100 ;
    delete pNSSliderAbsent ;
    delete pNSSliderPresent ;
    delete pNSSliderEventuel ;
    delete pNSSlider0P ;
    delete pNSSlider1P ;
    delete pNSSlider2P ;
    delete pNSSlider3P ;
    delete pNSSlider4P ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
Parametre::Parametre(const Parametre& src)
          :TDialog(src.AParent, src.ressource)
{
  ressource			    = src.ressource;
  AParent				    = src.AParent;
  pPatpatho		        = src.pPatpatho;
  pCertitude 		        = src.pCertitude;
  pImportance 		    = src.pImportance;
  pPlurielDes 		    = src.pPlurielDes;
  pPlurielLes 		    = src.pPlurielLes;
  pSingulier 			    = src.pSingulier;

  pNSSlider0     			= src.pNSSlider0;
  pNSSlider25			    = src.pNSSlider25;
  pNSSlider50			    = src.pNSSlider50;
  pNSSlider75			    = src.pNSSlider75;
  pNSSlider100		    = src.pNSSlider100;
  pNSSliderAbsent 	    = src.pNSSliderAbsent;
  pNSSliderPresent 	    = src.pNSSliderPresent;
  pNSSliderEventuel 		= src.pNSSliderEventuel;
  pNSSlider0P    		   	= src.pNSSlider0P;
  pNSSlider1P   		    = src.pNSSlider1P;
  pNSSlider2P   		    = src.pNSSlider2P;
  pNSSlider3P   		    = src.pNSSlider3P;
  pNSSlider4P   		    = src.pNSSlider4P;
  pNSTreeNodeSelection  = src.pNSTreeNodeSelection;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
inline
Parametre&
Parametre::operator=(const Parametre& src)
{
  if (this == &src)
    return *this ;

  ressource					=	src.ressource;
  AParent						=	src.AParent;
  pPatpatho					=	src.pPatpatho;
  pCertitude 					= 	src.pCertitude;
  pImportance 				= 	src.pImportance;
  pPlurielDes 				= 	src.pPlurielDes;
  pPlurielLes 				=  src.pPlurielLes;
  pSingulier 					= 	src.pSingulier;

  pNSSlider0     			=  src.pNSSlider0;
  pNSSlider25					=  src.pNSSlider25;
  pNSSlider50					=  src.pNSSlider50;
  pNSSlider75					=  src.pNSSlider75;
  pNSSlider100				=  src.pNSSlider100;
  pNSSliderAbsent 			=  src.pNSSliderAbsent;
  pNSSliderPresent 			=  src.pNSSliderPresent;
  pNSSliderEventuel 		=  src.pNSSliderEventuel;
  pNSSlider0P    			= 	src.pNSSlider0P;
  pNSSlider1P   				=  src.pNSSlider1P;
  pNSSlider2P   				=	src.pNSSlider2P;
  pNSSlider3P   				=	src.pNSSlider3P;
  pNSSlider4P   				=	src.pNSSlider4P;
  pNSTreeNodeSelection    =  src.pNSTreeNodeSelection;

	return *this ;
}

//---------------------------------------------------------------------------
// SetupWindow
//---------------------------------------------------------------------------
void
Parametre::SetupWindow()
{
  TDialog::SetupWindow() ;

  pCertitude->SetRange(0, 100) ;    //fixer min et max
  pCertitude->SetRuler(25, false) ; //espacement entre deux graduations
  pImportance->SetRange(0, 4) ;
  pImportance->SetRuler(1, false) ;

   //pluriel
  if      (pNSTreeNodeSelection->getPluriel() == string("WPLUR1") )
    pPlurielDes->Check();
  else if (pNSTreeNodeSelection->getPluriel() == string("WPLUS1") )
   	pPlurielLes->Check();
  else
   	pSingulier->Check();

  //certitude
  if (pNSTreeNodeSelection->getCertitude() == string("WCE001") ) //min
    pCertitude->SetPosition(0);
  else if( pNSTreeNodeSelection->getCertitude() == string("WCE251") )
   	pCertitude->SetPosition(25);
  else if( pNSTreeNodeSelection->getCertitude() == string("WCE501") )
   	pCertitude->SetPosition(50);
  else if ( pNSTreeNodeSelection->getCertitude() == string("WCE751") )
   	pCertitude->SetPosition(75);
  else
   	pCertitude->SetPosition(100);

  //interêt  if( pNSTreeNodeSelection->getInteret() == string("E") ) //min    pImportance->SetPosition(4) ;
  else if( pNSTreeNodeSelection->getInteret() == string("D") )
   	pImportance->SetPosition(3) ;
  else if( pNSTreeNodeSelection->getInteret() == string("C") )
   	pImportance->SetPosition(2) ;
  else if( pNSTreeNodeSelection->getInteret() == string("B") )
   	pImportance->SetPosition(1) ;
  else
   	pImportance->SetPosition(0) ;}

//------------------------------------------------------------------------//------------------------------------------------------------------------
void
Parametre::TraitementCertitude0Val()
{
	pCertitude->SetPosition(0);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementCertitude25Val()
{
	pCertitude->SetPosition(25);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementCertitude50Val()
{
	pCertitude->SetPosition(50);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementCertitude75Val()
{
	pCertitude->SetPosition(75);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementCertitude100Val()
{
	pCertitude->SetPosition(100);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementCertitudeAbsentVal()
{
	pCertitude->SetPosition(0);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementCertitudeEventuelVal()
{
	pCertitude->SetPosition(50);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementCertitudePresentVal()
{
	pCertitude->SetPosition(100);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementInteret0P()
{
	pImportance->SetPosition(0);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementInteret1P()
{
	pImportance->SetPosition(1);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementInteret2P()
{
	pImportance->SetPosition(2);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementInteret3P()
{
	pImportance->SetPosition(3);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
Parametre::TraitementInteret4P()
{
	pImportance->SetPosition(4);
}

//-----------------------------------------------------------------------
// confirmer le choix : ok
//-----------------------------------------------------------------------
void
Parametre::CmOk()
{
    //pluriel
    if (pPlurielDes->GetCheck() == BF_CHECKED)
  		pPatpatho->SetPluriel(string("WPLUR1"));
    else if (pPlurielLes->GetCheck() == BF_CHECKED)
  		pPatpatho->SetPluriel(string("WPLUS1"));
    else
	    pPatpatho->SetPluriel(string(""));

  	//certitude : Slider
  	if ((pCertitude->GetPosition() >= 0) &&
  	    (pCertitude->GetPosition() <= 12))
   	    pPatpatho->SetCertitude(string("WCE001"));
  	else if ((pCertitude->GetPosition() > 12) &&
           (pCertitude->GetPosition() <= 37))
   	    pPatpatho->SetCertitude(string("WCE251"));
  	else if ((pCertitude->GetPosition() > 37) &&
           (pCertitude->GetPosition() <= 62))
  		pPatpatho->SetCertitude(string("WCE501"));
    else if ((pCertitude->GetPosition() > 62) &&
           (pCertitude->GetPosition() <= 87))
  		pPatpatho->SetCertitude(string("WCE751"));
  	else
  		pPatpatho->SetCertitude(string(""));

    //interêt
    if( pImportance->GetPosition() == 4)  //max
   	    pPatpatho->SetInteret(string("E") );
    else if ( pImportance->GetPosition() == 3)
   	    pPatpatho->SetInteret(string("D"));
    else if ( pImportance->GetPosition() == 2 )
   	    pPatpatho->SetInteret(string("C"));
    else if ( pImportance->GetPosition() == 1 )
   	    pPatpatho->SetInteret(string("B"));
    else
  		pPatpatho->SetInteret(string("A"));

    sPlurielDlg = pPatpatho->GetPluriel();
    sInteretDlg = pPatpatho->GetInteret();
    sCertitudeDlg = pPatpatho->GetCertitude();
    CloseWindow(IDOK);
}

//-----------------------------------------------------------------------
// annuler le choix : cancel
//-----------------------------------------------------------------------
void
Parametre::CmCancel()
{
 	Destroy(IDCANCEL);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSSliderStatic::NSSliderStatic(const NSSliderStatic& src)
               :TStatic(src.AParent, src.ressource)
{
  ressource = src.ressource ;
  pSlider   = src.pSlider ;
  valeur    = src.valeur ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
inline
NSSliderStatic&
NSSliderStatic::operator=(const NSSliderStatic& src)
{
  if (this == &src)
    return *this ;

  ressource = src.ressource ;
  AParent   = src.AParent ;
  pSlider   = src.pSlider ;
  valeur    = src.valeur ;

  return *this;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NSSliderStatic::NSSliderStatic(TWindow* parent, int resId, TSlider* pOintSlider, int val, uint texlen , TModule* module  )
               :TStatic(parent, resId, texlen , module )
{
  ressource = resId ;
  AParent		= parent ;
  pSlider		= pOintSlider ;
  valeur		= val ;

  Attr.Style |= SS_NOTIFY ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSSliderStatic::~NSSliderStatic()
{
  if (pSlider)
    delete pSlider ;
}

DEFINE_RESPONSE_TABLE1(NSSliderStatic, TStatic)
 EV_NOTIFY_AT_CHILD(STN_CLICKED, EvStnClicked),
END_RESPONSE_TABLE;

void
NSSliderStatic::EvStnClicked()
{
   pSlider->SetPosition(valeur);
}

void
NSSliderStatic::SetupWindow()
{
	TWindow::SetupWindow();
}

DEFINE_RESPONSE_TABLE1(whatToEditDialog, NSUtilDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

whatToEditDialog::whatToEditDialog(TWindow* pere, string sRole, NSContexte* pCtx)
                 :NSUtilDialog(pere, pCtx, "IDD_WHATEDIT")
{
try
{
	_pEditPersonalData     = new OWL::TRadioButton(this, PERSO_BUTTON, 0) ;
  _pEditProfessionalData = new OWL::TRadioButton(this, PRO_BUTTON, 0) ;
  _pEditUserData         = new OWL::TRadioButton(this, USER_BUTTON, 0) ;

  _sRoles = sRole ;
}
catch (...)
{
	erreur("Exception whatToEditDialog ctor.", standardError, 0) ;
}
}

whatToEditDialog::~whatToEditDialog()
{
	delete _pEditPersonalData ;
  delete _pEditProfessionalData ;
  delete _pEditUserData ;
}

void
whatToEditDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  _pEditPersonalData->Check() ;
  _pEditProfessionalData->Check() ;

  size_t iUserPos  = _sRoles.find(USER_ROLE) ;
  size_t iAdminPos = _sRoles.find(ADMIN_ROLE) ;

  if ((NPOS != iUserPos) || (NPOS != iAdminPos))
		_pEditUserData->Check() ;
	else
  	_pEditUserData->SetCheck(BF_GRAYED) ;
}

void
whatToEditDialog::CmOk()
{
	TDialog::CmOk() ;
}

voidwhatToEditDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

bool
whatToEditDialog::isPersonalDataActive()
{
	return (BF_CHECKED == _pEditPersonalData->GetCheck()) ;
}

bool
whatToEditDialog::isProfessionalDataActive()
{
	return (BF_CHECKED == _pEditProfessionalData->GetCheck()) ;
}

bool
whatToEditDialog::isUserDataActive()
{
	return (BF_CHECKED == _pEditUserData->GetCheck()) ;
}

