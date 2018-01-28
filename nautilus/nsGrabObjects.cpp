// NSGRABOBJECTS.CPP : Dialogues de capture d'images
////////////////////////////////////////////////////////////
#include <cstring.h>#include <stdio.h>#include <assert.h>/** Includes spécifiques capture **/#include <string.h>#include <malloc.h>#include <windows.h>#include <wingdi.h>#include <mshtml.h>#include <iostream.h>///////////////////////////////////
#include <owl\clipboar.h>

#include "Compos.h"#include "Import.h"#include "ImpImg.h"#include "Lettre.h"#include "log_form.h"#include "WebService.h"#include "nautilus\nsbrowse.h"#include "nautilus\nsbasimg.h"#include "nautilus\nsdocref.h"#include "nautilus\nautilus.rh"#include "nautilus\nssuper.h"#include "nautilus\nshistdo.h"#include "nautilus\nsmodhtm.h"#include "nautilus\ns_html.h"#include "nautilus\nsvisual.h"#include "nautilus\nsresour.h"#include "nautilus\nsdocview.h"#include "nautilus\nscsdoc.h"#include "ns_grab\nsgrabfc.h"
#include "nsbb\nsattvaltools.h"
#include "nssavoir\nsfilecaptur.h"
#include "partage\nsdivfile.h"

#include "nautilus\nsGrabObjects.h"

#include "nautilus\nsepicap.h"

/****************** classe NSGrabObject **************************/NSGrabberObject::NSGrabberObject(NSContexte* pCtx)                :NSRoot(pCtx){}NSGrabberObject::~NSGrabberObject(){}boolNSGrabberObject::ConvertImage(string sFichier){	// Récupération du pointeur sur la fonction ConvertBmpToJpg	bool (FAR *pAdrConvert) (string far *, NSContexte far *) ;

	(FARPROC) pAdrConvert = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_CONVERTBMP)) ;

	if (pAdrConvert == NULL)
	{
		erreur("Impossible de récupérer la fonction ConvertBmpToJpg()", standardError, 0) ;
		return false ;
	}

	bool bOk = ((*pAdrConvert)((string far *) (&sFichier), (NSContexte far *) pContexte)) ;
	if (!bOk)
	{
		erreur("Erreur à la conversion d'une image bmp.", standardError, 0) ;
		return false ;
	}	return true ;}boolNSGrabberObject::FindFileName(string sNameSeed, string* pBmpFileName, string* pJpgFileName){	if ((NULL == pBmpFileName) || (NULL == pJpgFileName))		return false ;	if (sNameSeed == string(""))		sNameSeed = string("image") ;	string sFichier, sFichierDest ;	string sPath = pContexte->PathName("IHTM") ;
	int i = 0;
	char szNum[8] ;
	bool exist ;

	// Recherche d'un nom de fichier valide
	do
	{
		i++ ;
    itoa(i, szNum, 10) ;
    sFichier = sNameSeed + string(szNum) + string(".bmp") ;
    exist = NsFileExists(sPath + sFichier) ;
    if (false == exist)
    {
    	sFichierDest = sNameSeed + string(szNum) + string(".jpg") ;
      exist = NsFileExists(sPath + sFichierDest) ;
    }
	}
	while (exist) ;
  *pBmpFileName = sFichier ;  *pJpgFileName = sFichierDest ;  return true ;}/****************** classe NSMilGrabObject **************************/NSMilGrabObject::NSMilGrabObject(NSContexte* pCtx)                :NSGrabberObject(pCtx){}NSMilGrabObject::~NSMilGrabObject(){}boolNSMilGrabObject::InitCamera(HWND handleClient){	bool (FAR *pAdrInitCamera) (NSMatrox far *, int far, HWND far, NSContexte far *, long far *, long far *) ;	// Récupération du pointeur sur la fonction InitCamera	(FARPROC) pAdrInitCamera = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_INITCAMERA));

	if (pAdrInitCamera == NULL)	{		erreur("Impossible de récupérer la fonction InitCamera()", standardError, 0) ;		return false;	}
	// on initialise la camera 0
	bool bOk = ((*pAdrInitCamera)((NSMatrox far *) pContexte->getSuperviseur()->getMatrox(),
                                    (int far) 0, (HWND far) handleClient, (NSContexte far *) pContexte,
                                    (long far*) (&lSizeX), (long far *) (&lSizeY))) ;
	if (!bOk)
	{
		erreur("Erreur au lancement de la fonction InitCamera", standardError, 0) ;
		return false ;
	}	return true ;}boolNSMilGrabObject::FreeCamera(){	// Récupération du pointeur sur la fonction FreeCamera	bool (FAR *pAdrFreeCamera) (NSMatrox far *, int far) ;	(FARPROC) pAdrFreeCamera = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_FREECAMERA));

	if (pAdrFreeCamera != NULL)
	{		// on libère la camera 0    bool bOk = ((*pAdrFreeCamera)((NSMatrox far *) pContexte->getSuperviseur()->getMatrox(), (int far) 0)) ;
		if (!bOk)
		{
    	erreur("Erreur au lancement de la fonction FreeCamera", standardError, 0) ;
      return false ;
    }
	}
	else
	{
		erreur("Impossible de récupérer la fonction FreeCamera()", standardError, 0) ;
		return false ;
	}	return true ;}boolNSMilGrabObject::StopCamera(){	// Récupération du pointeur sur la fonction StopCamera	void (FAR *pAdrStopCamera) (NSMatrox far *, int far) ;	(FARPROC) pAdrStopCamera = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_STOPCAMERA)) ;

	if (pAdrStopCamera != NULL)
		// on stoppe la camera 0    ((*pAdrStopCamera)((NSMatrox far *) pContexte->getSuperviseur()->getMatrox(), (int far) 0)) ;
	else
	{
		erreur("Impossible de récupérer la fonction FreeCamera()", standardError, 0) ;
		return false ;
	}	return true ;}boolNSMilGrabObject::NbCamera(int& nbCam){	int (FAR *pAdrNbCamera) (NSMatrox far *) ;	(FARPROC) pAdrNbCamera = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_NBCAMERA)) ;

	if (pAdrNbCamera != NULL)
	{		nbCam = ((*pAdrNbCamera)((NSMatrox far *) (pContexte->getSuperviseur()->getMatrox()))) ;
		return true ;
	}
	else
	{
		erreur("Impossible de récupérer la fonction NbCamera()", standardError, 0) ;
    nbCam = -1 ;
    return false ;
  }}boolNSMilGrabObject::GrabFree(){#ifndef N_TIERS	// Récupération du pointeur sur la fonction GrabFree	bool (FAR *pAdrGrabFree) (NSMatrox far **) ;	(FARPROC) pAdrGrabFree = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_GRABFREE)) ;

	if (pAdrGrabFree != NULL)
	{  	NSMatrox* pMatrox = pContexte->getSuperviseur()->getMatrox() ;    NSMatrox far ** ppMatrox = &(pMatrox) ;		bool bOk = ((*pAdrGrabFree)((NSMatrox far **)  ppMatrox)) ;
		if (!bOk)
		{
			erreur("Erreur au lancement de la fonction GrabFree", standardError, 0) ;
			return false ;
		}

		pContexte->getSuperviseur()->initMatrox(*ppMatrox) ;
	}
	else
	{
		erreur("Impossible de récupérer la fonction GrabFree()", standardError, 0) ;
		return false ;
	}#endif	return true ;}boolNSMilGrabObject::GrabFrame(){	// Récupération du pointeur sur la fonction PlayCamera	bool (FAR *pAdrPlayCamera) (NSMatrox far *, int far) ;	(FARPROC) pAdrPlayCamera = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_GRABFRAME)) ;

	if (pAdrPlayCamera == NULL)	{		erreur("Impossible de récupérer la fonction GrabFrame()", standardError, 0) ;    return false ;	}
	// on lance la camera 0 (grabbing)
	bool bOk = ((*pAdrPlayCamera)((NSMatrox far *) pContexte->getSuperviseur()->getMatrox(), (int far) 0)) ;
	if (!bOk)
	{
		erreur("Erreur au lancement de la fonction GrabFrame", standardError, 0) ;
    return false ;
	}	return true ;}boolNSMilGrabObject::GrabOneBuffer(){	bool (FAR *pAdrPhotoCamera) (NSMatrox far *, int far) ;	(FARPROC) pAdrPhotoCamera = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_GRABONEBUFFER)) ;

	if (pAdrPhotoCamera == NULL)	{		erreur("Impossible de récupérer la fonction GrabOneBuffer()", standardError, 0) ;    return false ;	}
	// on lance la camera 0 (grabbing)
	bool bOk = ((*pAdrPhotoCamera)((NSMatrox far *) pContexte->getSuperviseur()->getMatrox(), (int far) 0)) ;
	if (!bOk)
	{
		erreur("Erreur au lancement de la fonction GrabOneBuffer", standardError, 0) ;
    return false ;
	}	return true ;}boolNSMilGrabObject::CaptureImage(string sFichier, HWND /* hwnd */){	// Récupération du pointeur sur la fonction CaptureImage de NSMatrox	bool (FAR *pAdrCaptureImage) (NSMatrox far *, int far, string far, NSContexte far *) ;	(FARPROC) pAdrCaptureImage = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_CAPTUREIMAGE)) ;

	if (pAdrCaptureImage == NULL)	{		erreur("Impossible de récupérer la fonction CaptureImage()", standardError, 0) ;    return false ;	}	// on arrête la camera 0 (grabbing)	bool bOk = ((*pAdrCaptureImage)((NSMatrox far *) pContexte->getSuperviseur()->getMatrox(), (int far) 0,
                                    (string far) sFichier, (NSContexte far *) pContexte)) ;
	if (!bOk)
	{
		erreur("Erreur au lancement de la fonction CaptureImage", standardError, 0) ;
    return false ;
	}	return true ;}boolNSMilGrabObject::PanImage(long X, long Y){	// Récupération du pointeur sur la fonction PanImage de NSMatrox	bool (FAR *pAdrPanImage) (NSMatrox far *, long far, long far) ;	(FARPROC) pAdrPanImage = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_PANIMAGE)) ;

	if (pAdrPanImage == NULL)	{		erreur("Impossible de récupérer la fonction PanImage()", standardError, 0) ;		return false ;	}	// panning de l'image...	((*pAdrPanImage)((NSMatrox far *) pContexte->getSuperviseur()->getMatrox(), (long far) X, (long far) Y)) ;	return true ;}boolNSMilGrabObject::ZoomImage(long X, long Y){	// Récupération du pointeur sur la fonction ZoomImage de NSMatrox	bool (FAR *pAdrZoomImage) (NSMatrox far *, long far, long far) ;	(FARPROC) pAdrZoomImage = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(M_ZOOMIMAGE)) ;

	if (pAdrZoomImage == NULL)	{		erreur("Impossible de récupérer la fonction ZoomImage()", standardError, 0) ;    return false ;	}	// zooming de l'image...	((*pAdrZoomImage)((NSMatrox far *) pContexte->getSuperviseur()->getMatrox(), (long far) X, (long far) Y)) ;	return true ;}/****************** classe NSGrabWindow **************************/
DEFINE_RESPONSE_TABLE1(NSMilGrabWindow, TWindow)   EV_WM_RBUTTONUP,   EV_WM_LBUTTONUP,   EV_WM_CREATE,   EV_WM_SIZE,   EV_COMMAND(IDC_PLAY, PlayCamera),   EV_COMMAND(IDC_STOP, StopCamera),   EV_COMMAND(IDC_CAPTURE, CaptureImage),   EV_COMMAND(IDC_PAN, PanImage),   EV_COMMAND(IDC_ZOOM, ZoomImage),   EV_COMMAND(IDC_GRAB, GrabContinuous),END_RESPONSE_TABLE;
NSMilGrabWindow::NSMilGrabWindow(TWindow* parent, NSContexte* pCtx, AssistCaptureDialog* pAssistDlg)                :TWindow(parent){	pContexte       = pCtx ;	pAssist         = pAssistDlg ;	bCanClose       = false ;	bOpenMenu       = false ;	bGrabInProgress = false ;	pGrabber  = new NSMilGrabObject(pCtx) ;  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;  if (NULL != pMyApp)    pMyApp->setMenu(string("menubar"), &hAccelerator) ;
}
NSMilGrabWindow::~NSMilGrabWindow(){	// delete pGrabber;  if (0 != hAccelerator)  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }}
boolNSMilGrabWindow::CanClose(){	string  sMsg;	int     nbCam;	if (bCanClose)		return true ;	bCanClose = true ;	sMsg = "Entree dans la fonction NSGrabWindow::CanClose" ;	pContexte->getSuperviseur()->trace(&sMsg, 1) ;	StopCamera() ;	pGrabber->FreeCamera() ;	sMsg = "CanClose : Camera libérée";	pContexte->getSuperviseur()->trace(&sMsg, 1) ;
	// Récupération du pointeur sur la fonction NbCamera	// pour savoir s'il reste encore une camera ou non	pGrabber->NbCamera(nbCam) ;	if (nbCam == 0)  	// si on a détruit la dernière caméra    // on libère le systeme
    pGrabber->GrabFree() ;

	sMsg = "Sortie de NSGrabWindow::CanClose()" ;	pContexte->getSuperviseur()->trace(&sMsg,1) ;	// pContexte->getSuperviseur()->bTrace = false;	// enfin, on envoie le message au dialogue assist	// pour qu'il se réveille	if (pAssist)  	pAssist->PostMessage(WM_COMMAND, CM_ASSIST) ;	return true ;}// Fonction SetupWindow////////////////////////////////////////////////////////////////voidNSMilGrabWindow::SetupWindow(){	TWindow::SetupWindow() ;	NS_CLASSLIB::TRect rect = Parent->GetWindowRect() ;	NS_CLASSLIB::TRect rectClient = Parent->GetClientRect() ;
	int dX = rect.Width() - rectClient.Width() ;	int dY = rect.Height() - rectClient.Height() ;	//PanImage();	//ZoomImage();	// pContexte->getSuperviseur()->bTrace = true;	if (!pGrabber->InitCamera(GetHandle()))		return ;	// Resize de la fenetre selon la taille du buffer caméra	lSizeX = pGrabber->lSizeX + ((long)dX) ;	lSizeY = pGrabber->lSizeY + ((long)dY) ;	Parent->MoveWindow(0, 0, (int) lSizeX, (int) lSizeY) ;	// Repeindre la zone client	::InvalidateRect(NULL,NULL,TRUE) ;	pContexte->getSuperviseur()->donneMainWindow()->UpdateWindow() ;	PanImage() ;	ZoomImage() ;	SetWindowPosit() ;	PlayCamera() ;}
int
NSMilGrabWindow::EvCreate(CREATESTRUCT far& createStruct)
{
	return TWindow::EvCreate(createStruct) ;

	// pGrabber->PlayCamera();
}

bool
NSMilGrabWindow::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;
}

void
NSMilGrabWindow::EvSize(uint sizeType, NS_CLASSLIB::TSize& size)
{
	// lSizeX = (long) size.X();
	// lSizeY = (long) size.Y();
}

void
NSMilGrabWindow::EvRButtonUp(uint a, NS_CLASSLIB::TPoint& p)
{
	NS_CLASSLIB::TPoint lp = p ;
	OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

	bOpenMenu = true ;

	menu->AppendMenu(MF_STRING, IDC_PLAY, "Jouer caméra") ;	menu->AppendMenu(MF_STRING, IDC_STOP, "Arrêt caméra") ;
	menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
	menu->AppendMenu(MF_STRING, IDC_CAPTURE,  "Capturer") ;	menu->AppendMenu(MF_STRING, IDC_PAN, "Panner") ;	menu->AppendMenu(MF_STRING, IDC_ZOOM, "Zoomer") ;
	ClientToScreen(lp) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, Parent->HWindow) ;
	delete menu ;
}

void
NSMilGrabWindow::EvLButtonUp(uint a, NS_CLASSLIB::TPoint& p)
{
	if (!bOpenMenu)
		CaptureImage() ;
}

void
NSMilGrabWindow::PlayCamera()
{
	bOpenMenu       = false ;
	bGrabInProgress = true ;
	GrabContinuous() ;

	//pGrabber->PlayCamera();
	// string sMsg = "Sortie de PlayCamera";
	// pContexte->getSuperviseur()->trace(&sMsg, 1);
}

void
NSMilGrabWindow::GrabContinuous()
{
	if (bGrabInProgress)
	{
		pGrabber->GrabFrame() ;
		// pContexte->getSuperviseur()->pNSApplication->PumpWaitingMessages();
		// PostMessage(WM_COMMAND, IDC_GRAB);
	}
}

void
NSMilGrabWindow::StopCamera()
{
	bOpenMenu       = false ;
	bGrabInProgress = false ;
	pGrabber->StopCamera() ;

	// string sMsg = "Sortie de StopCamera";
	// pContexte->getSuperviseur()->trace(&sMsg, 1);
}

void
NSMilGrabWindow::CaptureImage()
{
	string sFichier, sFichierDest ;
	string sPath = pContexte->PathName("IHTM") ;

	bOpenMenu = false ;

	StopCamera() ;

  if (!(pGrabber->FindFileName(string("Image"), &sFichier, &sFichierDest)))
  {
  	erreur("Can't find temporary file name.", standardError, 0) ;
  	PlayCamera() ;
		return ;
	}

	if (pGrabber->CaptureImage(sFichier, GetHandle()))
	{
		if (pGrabber->ConvertImage(sFichier))
		{
			sFichier = sPath + sFichier ;
			if (!DeleteFile(sFichier.c_str()))
				erreur("Erreur à la destruction du fichier BMP temporaire.", standardError, 0) ;
		}
	}

	PlayCamera() ;
}

void
NSMilGrabWindow::PanImage()
{
	string sRes ;
	long X, Y ;

	sRes = pContexte->getSuperviseur()->getMatrox()->pParamArray->ValParam("Pan") ;
	X = atol(sRes.c_str()) ;
	sRes = pContexte->getSuperviseur()->getMatrox()->pParamArray->ValParam("Scroll") ;
	Y = atol(sRes.c_str()) ;

	if ((X == 0L) && (Y == 0L))
		return ;

	NS_CLASSLIB::TRect rect = Parent->GetWindowRect() ;
	NS_CLASSLIB::TPoint point(rect.X(), rect.Y()) ;
	Parent->Parent->ScreenToClient(point) ;

	pGrabber->PanImage(X,Y) ;

	::InvalidateRect(NULL, NULL, TRUE) ;
	Parent->UpdateWindow() ;

	// resize de la fenetre
	lSizeX = lSizeX - X ;
	lSizeY = lSizeY - Y ;
	Parent->MoveWindow(point.X(), point.Y(), (int) lSizeX, (int) lSizeY) ;
	::InvalidateRect(NULL, NULL, TRUE) ;
	pContexte->getSuperviseur()->donneMainWindow()->UpdateWindow() ;
}

void
NSMilGrabWindow::ZoomImage()
{
	NS_CLASSLIB::TRect rect = Parent->GetWindowRect() ;
	NS_CLASSLIB::TRect rectClient = Parent->GetClientRect() ;
	int dX = rect.Width() - rectClient.Width() ;	int dY = rect.Height() - rectClient.Height() ;

	string sRes = pContexte->getSuperviseur()->getMatrox()->pParamArray->ValParam("Zoom") ;
	long   X    = atol(sRes.c_str()) ;

	if (X <= 1L)
		return ;

	NS_CLASSLIB::TPoint point(rect.X(), rect.Y()) ;
	Parent->Parent->ScreenToClient(point) ;

	// StopCamera();

	pGrabber->ZoomImage(X, X) ;

	// resize de la fenetre
	lSizeX = ((lSizeX - dX) * X) + dX ;
	lSizeY = ((lSizeY - dY) * X) + dY ;
	Parent->MoveWindow(point.X(), point.Y(), (int) lSizeX, (int) lSizeY) ;
	::InvalidateRect(NULL,NULL,TRUE) ;
	pContexte->getSuperviseur()->donneMainWindow()->UpdateWindow() ;
}

void
NSMilGrabWindow::SetWindowPosit()
{
	int X, Y, W, H ;
	string sPosition = pContexte->getSuperviseur()->getMatrox()->pParamArray->ValParam("Position") ;

	if (sPosition != string(""))
	{
		getWindowPosit(sPosition, X, Y, W, H) ;
		Parent->MoveWindow(X, Y, W, H) ;
	}
}

/****************** classe NSGrabChild **************************/
////////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE1(NSMilGrabChild, TMDIChild)   EV_WM_CLOSE,   EV_COMMAND(CM_FILECLOSE, EvClose),END_RESPONSE_TABLE;
NSMilGrabChild::NSMilGrabChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSMilGrabWindow* clientWnd)               :TMDIChild(parent, title, clientWnd), NSRoot(pCtx){	pClient = clientWnd ;}
NSMilGrabChild::~NSMilGrabChild(){}voidNSMilGrabChild::EvClose(){	string sMsg = "Entree dans la fonction NSGrabChild::EvClose" ;	pContexte->getSuperviseur()->trace(&sMsg, 1) ;	if (!pClient->bCanClose)	{		// string sCaption = string("Message ") + pContexte->getSuperviseur()->sAppName.c_str();		// int retVal = ::MessageBox(pContexte->getSuperviseur()->donneMainWindow()->GetHandle(),		//                "Voulez-vous vraiment fermer la capture d'images ?", sCaption.c_str(), MB_YESNO);
		// if (retVal == IDYES)		TWindow::EvClose() ;	}	else		TWindow::EvClose() ;}// -----------------------------------------------------------------////  Méthodes de AssistCaptureDialog//// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(AssistCaptureDialog, NSUtilDialog)	EV_COMMAND(IDC_ASSISTCAPTURE_SUITE, CmSuite),	EV_COMMAND(IDC_ASSISTCAPTURE_ANNULER, CmAnnuler),	EV_COMMAND(CM_ASSIST, CheckPhase),	EV_BN_CLICKED(IDC_ASSISTCAPTURE_CHECK1, CheckPhase),	EV_BN_CLICKED(IDC_ASSISTCAPTURE_CHECK2, CheckPhase),	EV_BN_CLICKED(IDC_ASSISTCAPTURE_CHECK3, CheckPhase),END_RESPONSE_TABLE;
AssistCaptureDialog::AssistCaptureDialog(TWindow* pere, NSContexte* pCtx)                    :NSUtilDialog(pere, pCtx, "IDD_ASSISTCAPTURE"){try{	phase = 1;	pContexte = pCtx;	pPhase1 = new OWL::TCheckBox(this, IDC_ASSISTCAPTURE_CHECK1) ;	pPhase2 = new OWL::TCheckBox(this, IDC_ASSISTCAPTURE_CHECK2) ;	pPhase3 = new OWL::TCheckBox(this, IDC_ASSISTCAPTURE_CHECK3) ;}catch (...)
{	erreur("Exception constructeur de AssistCaptureDialog.", standardError, 0) ;}}
AssistCaptureDialog::~AssistCaptureDialog(){	delete pPhase1 ;	delete pPhase2 ;	delete pPhase3 ;}
voidAssistCaptureDialog::SetupWindow(){try{  // fichiers d'aide	sHindex = "" ;
	sHcorps = "Importation_d_une_image.html" ;
	NSUtilDialog::SetupWindow() ;	CheckPhase() ;
}catch (...)
{	erreur("Exception AssistCaptureDialog::SetupWindow().", standardError, 0) ;}}
void AssistCaptureDialog::CheckPhase()
{
	Show(SW_SHOW) ;
	SetFocus() ;

	switch(phase)
	{
    case 1 :
        pPhase1->Check();
        pPhase2->Uncheck();
        pPhase3->Uncheck();
        break;
    case 2 :
        pPhase1->Uncheck();
        pPhase2->Check();
        pPhase3->Uncheck();
        break;
    case 3 :
        pPhase1->Uncheck();
        pPhase2->Uncheck();
        pPhase3->Check();
        break;
    default :
        erreur("Pb de phase inconnue dans le dialogue assistant de la capture", standardError, 0, GetHandle()) ;
	}
}

voidAssistCaptureDialog::CmSuite(){	// on cache d'abord le dialogue	Show(SW_HIDE) ;	switch(phase)	{  	case 1 :    	// capture d'images      pContexte->getPatient()->CmCapture(this) ;      break ;    case 2 :    	// Importation      pContexte->getPatient()->CmImportImg(this) ;      break ;    case 3 :    	// Edition planche d'images      pContexte->getPatient()->CmGenerePannel() ;      break ;    default :    	erreur("Pb de phase inconnue dans le dialogue assistant de la capture", standardError, 0, GetHandle()) ;	}	phase++ ;}
voidAssistCaptureDialog::CmAnnuler(){	TDialog::CmCancel() ;
}/****************** classe NSTwainObject **************************/NSTwainObject::NSTwainObject(NSContexte* pCtx)              :NSGrabberObject(pCtx){	sBmpFileName = string("") ;	sJpgFileName = string("") ;}NSTwainObject::~NSTwainObject(){}boolNSTwainObject::CaptureImage(string sFichier, HWND hwnd){	string sFileName = sFichier ;	if (sFileName == string(""))	{		if ((string("") == sBmpFileName) || (string("") == sJpgFileName))		{			bool bFound = FindFileName("scanImage", &sBmpFileName, &sJpgFileName) ;    	if (!bFound)			{    		erreur("Can't find temporary file name.", standardError, 0) ;      	return false ;			}		}    sFileName = sBmpFileName ;	}	bool (FAR *pTwainAcquire) (string far, HWND far) ;	// Récupération du pointeur sur la fonction InitCamera	(FARPROC) pTwainAcquire = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(T_CAPTUREIMAGE)) ;

	if (pTwainAcquire == NULL)	{		erreur("Impossible de récupérer la fonction CaptureImage()", standardError, 0) ;		return false;	}
	// on initialise la camera 0
	bool bOk = ((*pTwainAcquire)((string far) sFileName, (HWND far) hwnd)) ;
	if (!bOk)
	{
		erreur("Erreur au lancement de la fonction CaptureImage", standardError, 0) ;
		return false ;
	}	return true ;}intNSTwainObject::SelectImageSource(HWND hwnd){	int  _export TwainSelectSource(HWND far ClientWndHandle) ;	int (FAR *pTwainSelectSource) (HWND far) ;	// Récupération du pointeur sur la fonction InitCamera	(FARPROC) pTwainSelectSource = pContexte->getSuperviseur()->getGrabModule()->GetProcAddress(MAKEINTRESOURCE(T_SELECTSOURCE)) ;

	if (pTwainSelectSource == NULL)	{		erreur("Impossible de récupérer la fonction SelectSource()", standardError, 0) ;		return false;	}
	// on initialise la camera 0
	return ((*pTwainSelectSource)((HWND far) hwnd)) ;
}
/****************** classe NSTwainWindow **************************/DEFINE_RESPONSE_TABLE1(NSTwainWindow, TWindow)   EV_WM_RBUTTONUP,   EV_WM_CREATE,   EV_WM_SIZE,   EV_COMMAND(IDC_CAPTURE,      CaptureImage),   EV_COMMAND(IDC_SELECTSOURCE, SelectSource),END_RESPONSE_TABLE;NSTwainWindow::NSTwainWindow(TWindow* parent, NSContexte* pCtx){	pContexte = pCtx ;	bOpenMenu = false ;  pDib      = 0 ;	pGrabber  = new NSTwainObject(pCtx) ;}NSTwainWindow::~NSTwainWindow(){	if (pDib)		delete pDib ;}
voidNSTwainWindow::SetupWindow(){	TWindow::SetupWindow() ;	NS_CLASSLIB::TRect rect = Parent->GetWindowRect() ;	NS_CLASSLIB::TRect rectClient = Parent->GetClientRect() ;
	int dX = rect.Width() - rectClient.Width() ;	int dY = rect.Height() - rectClient.Height() ;	CaptureImage() ;}voidNSTwainWindow::EvRButtonUp(uint, NS_CLASSLIB::TPoint&){}voidNSTwainWindow::EvSize(uint sizeType, NS_CLASSLIB::TSize& size){	TWindow::EvSize(sizeType, size) ;}intNSTwainWindow::EvCreate(CREATESTRUCT far& createStruct){	return TWindow::EvCreate(createStruct) ;}voidNSTwainWindow::CaptureImage(){	bool bCaptured = pGrabber->CaptureImage(string(""), GetHandle()) ;  if (bCaptured && (pGrabber->sBmpFileName != string("")))	{		int size = 0 ;		ifstream inf(pGrabber->sBmpFileName.c_str(), ios::in ) ;
		inf.seekg(0L, ios::end ) ;
		if ((size = inf.tellg()) >= 0)
    {
  		pDib = new TDib(inf) ;
			lSizeX = pDib->Size().X() ;			lSizeY = pDib->Size().Y() ;			Parent->MoveWindow(0, 0, (int) lSizeX, (int) lSizeY) ;			// Repeindre la zone client			::InvalidateRect(NULL, NULL, TRUE) ;			pContexte->getSuperviseur()->donneMainWindow()->UpdateWindow() ;		}	}}voidNSTwainWindow::SelectSource(){	pGrabber->SelectImageSource(GetHandle()) ;}voidNSTwainWindow::SetWindowPosit(){}/****************** classe NSTwainWindow **************************/DEFINE_RESPONSE_TABLE1(NSTwainGrabChild, TMDIChild)   EV_WM_CLOSE,   EV_COMMAND(CM_FILECLOSE, EvClose),END_RESPONSE_TABLE;NSTwainGrabChild::NSTwainGrabChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSTwainWindow* clientWnd)                 :TMDIChild(parent, title, clientWnd), NSRoot(pCtx){	pClient = clientWnd ;}NSTwainGrabChild::~NSTwainGrabChild(){}
voidNSTwainGrabChild::EvClose(){	TWindow::EvClose() ;}//////////////////////// fin de nsGrabObjects.cpp /////////////////////////////
