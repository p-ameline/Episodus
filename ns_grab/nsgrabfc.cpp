#include <windows.h>#include <owl/window.h>
#include <list.h>

#include "ns_grab\nsgrabfc.h"#include "ns_grab\FreeImage.h"#include "nautilus\nssuper.h"
// Classe NSCamera
////////////////////////////////////////////////////

NSCamera::NSCamera(int noCamera, string dataFormat, HWND handle){    format = dataFormat;    handleClient = handle;    numCam = noCamera;    switch(noCamera)    {        case 0 : digNum = M_DEV0; break;        case 1 : digNum = M_DEV1; break;        case 2 : digNum = M_DEV2; break;        case 3 : digNum = M_DEV3; break;        default :            erreur("Identifiant de caméra non valide", standardError, 0);    }}NSCamera::~NSCamera(){}NSCamera::NSCamera(const NSCamera& rv){  digNum =    rv.digNum;  format =    rv.format;  digitizer = rv.digitizer;  buffer =    rv.buffer;  buffer1 =   rv.buffer1;  buffer2 =   rv.buffer2;  handleClient = rv.handleClient;  numCam =    rv.numCam;}NSCamera&NSCamera::operator=(const NSCamera& src){  if (this == &src)  	return *this ;  digNum =    src.digNum;  format =    src.format;  digitizer = src.digitizer;  buffer =    src.buffer;  buffer1 =   src.buffer1;  buffer2 =   src.buffer2;  handleClient = src.handleClient;  numCam =    src.numCam;  return *this;}voidNSCamera::AssociateChannel(string sChannel){    int numChannel = atoi(sChannel.c_str());    switch(numChannel)    {        case 0  : channel = M_CH0; break;        case 1  : channel = M_CH1; break;        case 2  : channel = M_CH2; break;        case 3  : channel = M_CH3; break;        case 4  : channel = M_CH4; break;        case 5  : channel = M_CH5; break;        case 6  : channel = M_CH6; break;        case 7  : channel = M_CH7; break;        case 8  : channel = M_CH8; break;        case 9  : channel = M_CH9; break;        case 10 : channel = M_CH10; break;        case 11 : channel = M_CH11; break;        case 12 : channel = M_CH12; break;        case 13 : channel = M_CH13; break;        case 14 : channel = M_CH14; break;        case 15 : channel = M_CH15; break;        default : erreur("Numéro de channel inconnu, le channel va être fixé sur le channel 0.", standardError, 0);                  channel = M_CH0;    }}// Classe NSCameraArray
//////////////////////////////////////////

//---------------------------------------------------------------------------
//  Constructeur copie//---------------------------------------------------------------------------
NSCameraArray::NSCameraArray(const NSCameraArray& rv)              :NSCameraVector(){try{  if (false == rv.empty())    for (NSCameraConstIter i = rv.begin() ; rv.end() != i ; i++)      push_back(new NSCamera(*(*i)));}catch (...){    erreur("Exception NSCameraArray ctor.", standardError, 0);}}
NSCameraArray&
NSCameraArray::operator=(const NSCameraArray& src)
{try{  if (this == &src)  	return *this ;  vider() ;  if (false == src.empty())    for (NSCameraConstIter i = src.begin() ; src.end() != i ; i++)      push_back(new NSCamera(*(*i))) ;  return *this ;}catch (...){    erreur("Exception NSCameraArray = operator.", standardError, 0);    return *this ;}}

//---------------------------------------------------------------------------//  Destructeur//---------------------------------------------------------------------------
voidNSCameraArray::vider(){    if (!empty())        for (NSCameraIter i = begin(); i != end(); )        {            delete *i;            erase(i);        }}
NSCameraArray::~NSCameraArray(){	vider();}
// Classe NSMatrox
/////////////////////////////////////////////

NSMatrox::NSMatrox(NSContexte* pCtx)
{
try
{
    nbCamera = 0;
    pParamArray = new ParamArray;

    LireParams(pCtx);
}
catch (...){    erreur("Exception NSMatrox ctor.", standardError, 0);}
}

NSMatrox::~NSMatrox()
{
  delete pParamArray ;
}

NSMatrox::NSMatrox(const NSMatrox& rv)
{
try
{
  MilApplication = rv.MilApplication;
  MilSystem = rv.MilSystem;
  MilDisplay = rv.MilDisplay;
  Camera = rv.Camera;
  nbCamera = rv.nbCamera;
  pParamArray = new ParamArray(*(rv.pParamArray));
}
catch (...){  erreur("Exception NSMatrox copy ctor.", standardError, 0);}
}

NSMatrox&
NSMatrox::operator=(const NSMatrox& src)
{
try
{
	if (this == &src)
  	return *this ;

	MilApplication = src.MilApplication ;
	MilSystem      = src.MilSystem ;
	MilDisplay     = src.MilDisplay ;
	Camera         = src.Camera ;
	nbCamera       = src.nbCamera ;
	pParamArray    = new ParamArray(*(src.pParamArray)) ;

	return *this ;
}
catch (...){	erreur("Exception NSMatrox = operator.", standardError, 0) ;	return *this ;}
}

void
NSMatrox::LireParams(NSContexte* pCtx)
{
	lireParam(pCtx->getUtilisateur()->getNss(), "matrox.dat", pParamArray) ;
}

int
NSMatrox::IndexCamera(int noCamera)
{
    int index = 0;
    bool trouve = false;

    if (!Camera.empty())
    for (NSCameraIter i = Camera.begin(); i != Camera.end(); i++)
    {
        if ((*i)->numCam == noCamera)
        {
            trouve = true;
            break;
        }

        index++;
    }

    if (!trouve)
        index = -1;

    return index;
}

void
NSMatrox::AddCamera(int noCamera, string format, HWND handle)
{
try
{
    Camera.push_back(new NSCamera(noCamera, format, handle)) ;
    nbCamera++ ;
}
catch (...){    erreur("Exception NSMatrox::AddCamera.", standardError, 0);}
}

void
NSMatrox::RemoveCamera(int noCamera)
{
    if (!Camera.empty())
        for (NSCameraIter i = Camera.begin(); i != Camera.end(); i++)
        {
            if ((*i)->numCam == noCamera)
            {
                delete (*i);
                Camera.erase(i);
                nbCamera--;
                break;
            }
        }
}

// FONCTIONS DE LA DLL
///////////////////////////////////////////////////////////////////
bool
GrabInit(NSMatrox far **ppMatrox, NSContexte* pCtx)
{
try
{
    *ppMatrox = new NSMatrox(pCtx);
    string sSys;
    char msg[255];

    sSys = (*ppMatrox)->pParamArray->ValParam("Systeme");

    /* Allouer une appli MIL */
    MappAlloc(M_DEFAULT, &((*ppMatrox)->MilApplication));
    /* Allouer un systeme MIL */
    if (sSys == "HOST")
        MsysAlloc(M_SYSTEM_HOST, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "VGA")
        MsysAlloc(M_SYSTEM_VGA, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "METEOR")
        MsysAlloc(M_SYSTEM_METEOR, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "PULSAR")
        MsysAlloc(M_SYSTEM_PULSAR, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "GENESIS")
        MsysAlloc(M_SYSTEM_GENESIS, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "GENESIS_PLUS")
        MsysAlloc(M_SYSTEM_GENESIS_PLUS, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "GENESIS_HOST")
        MsysAlloc(M_SYSTEM_GENESIS_HOST, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "CORONA")
        MsysAlloc(M_SYSTEM_CORONA, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "CORONA_II")
        MsysAlloc(M_SYSTEM_CORONA_II, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "METEOR_II")
        MsysAlloc(M_SYSTEM_METEOR_II, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "METEOR_II_DIG")
        MsysAlloc(M_SYSTEM_METEOR_II_DIG, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "METEOR_II_CL")
        MsysAlloc(M_SYSTEM_METEOR_II_CL, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "METEOR_II_1394")
        MsysAlloc(M_SYSTEM_METEOR_II_1394, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "ORION")
        MsysAlloc(M_SYSTEM_ORION, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "CRONOS")
        MsysAlloc(M_SYSTEM_CRONOS, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "CRONOSPLUS")
        MsysAlloc(M_SYSTEM_CRONOSPLUS, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else if (sSys == "ODYSSEY")
        MsysAlloc(M_SYSTEM_ODYSSEY, M_DEFAULT, M_DEFAULT, &((*ppMatrox)->MilSystem));
    else
    {
        sprintf(msg, "Le systeme %s de matrox.dat n'est pas disponible dans Nautilus", sSys.c_str());
        erreur(msg, standardError, 0);
        return false;
    }
    /* Allouer un display MIL */
    // M_DEF_DISPLAY_FORMAT remplacé par M_DEFAULT

    // Pour afficher la vidéo plein écran sur le tv-out :
    // Paramètre 3 : "M_PAL_YC"
    // Paramètre 4 : "M_AUXILIARY" ou

    MdispAlloc((*ppMatrox)->MilSystem, M_DEFAULT, M_DEF_DISPLAY_FORMAT, M_WINDOWED, &((*ppMatrox)->MilDisplay));

    // Autoriser le zoom des fenetres (inutile)
    // MdispControl((*ppMatrox)->MilDisplay, M_WINDOW_ZOOM, M_ENABLE);

    return true;
}
catch (...){    erreur("Exception GrabInit.", standardError, 0) ;    return false ;}
}

bool
GrabFree(NSMatrox far **ppMatrox)
{
try
{
    MdispFree((*ppMatrox)->MilDisplay);
    MsysFree((*ppMatrox)->MilSystem);
    MappFree((*ppMatrox)->MilApplication);

    // on libère le contexte vidéo
    delete *ppMatrox;
    *ppMatrox = 0;

    return true;
}
catch (...){    erreur("Exception GrabFree.", standardError, 0);    return false ;}
}

bool
InitCamera(NSMatrox far *pMatrox, int far noCamera, HWND far ClientWndHandle,
            NSContexte* /* pCtx */, long far *pSizeX, long far *pSizeY)
{
try
{
    long BufSizeX;
    long BufSizeY;
    long BufSizeBand;
    char szNum[4];
    char msg[255];
    char szFormat[255];

    int iCam = pMatrox->IndexCamera(noCamera);
    if (iCam >= 0)
    {
        sprintf(msg, "InitCamera : La camera %d a déjà été initialisée", noCamera);
        erreur(msg, standardError, 0);
        return false;
    }

    /* Allouer un digitizer MIL */
    if (MsysInquire(pMatrox->MilSystem, M_DIGITIZER_NUM, M_NULL) > 0)
    {
        itoa(noCamera, szNum, 10);
        string sCamera = string("Camera") + string(szNum);
        string sFormat = pMatrox->pParamArray->ValParam(sCamera);
        string sChannel = pMatrox->pParamArray->ValParam("Channel");

        // on push une nouvelle camera dans le tableau
        pMatrox->AddCamera(noCamera, sFormat, ClientWndHandle);
        iCam = pMatrox->nbCamera - 1;

        strcpy(szFormat, sFormat.c_str());
        MdigAlloc(pMatrox->MilSystem, (pMatrox->Camera[iCam])->digNum, szFormat,
                    M_DEFAULT, &((pMatrox->Camera[iCam])->digitizer));

        if (false == IsNo(sChannel))
        {
            (pMatrox->Camera[iCam])->AssociateChannel(sChannel);
            MdigChannel((pMatrox->Camera[iCam])->digitizer, (pMatrox->Camera[iCam])->channel);
        }

        MdigInquire((pMatrox->Camera[iCam])->digitizer, M_SIZE_X, &BufSizeX);
        MdigInquire((pMatrox->Camera[iCam])->digitizer, M_SIZE_Y, &BufSizeY);
        MdigInquire((pMatrox->Camera[iCam])->digitizer, M_SIZE_BAND, &BufSizeBand);
    }
    else
    {
        strcpy(msg, "Impossible d'allouer un digitizer dans InitCamera()");
        erreur(msg, standardError, 0);
        return false;
    }

    if (MdispInquire(pMatrox->MilDisplay, M_DISPLAY_MODE, M_NULL) != M_WINDOWED)
    {
        strcpy(msg, "InitCamera() ne fonctionne qu'avec des display de type WINDOWED");
        erreur(msg, standardError, 0);
        return false;
    }

    if (pSizeX)
        *pSizeX = BufSizeX;

    if (pSizeY)
        *pSizeY = BufSizeY;

    /* Double-buffering : on alloue deux "grab" buffers (param M_DDRAW enlevé)*/
    // MbufAllocColor(pMatrox->MilSystem, BufSizeBand, BufSizeX, BufSizeY, 8+M_UNSIGNED,
    //                 M_IMAGE+M_GRAB+M_PROC, &((pMatrox->Camera[iCam])->buffer1));

    // MbufAllocColor(pMatrox->MilSystem, BufSizeBand, BufSizeX, BufSizeY, 8+M_UNSIGNED,
    //                 M_IMAGE+M_GRAB+M_PROC, &((pMatrox->Camera[iCam])->buffer2));

    /* On alloue un "display" buffer et on l'efface */
    MbufAllocColor(pMatrox->MilSystem, BufSizeBand, BufSizeX, BufSizeY, 8+M_UNSIGNED,
                    M_IMAGE+M_GRAB+M_PROC+M_DISP, &((pMatrox->Camera[iCam])->buffer));

    MbufClear((pMatrox->Camera[iCam])->buffer, 0);

    /* Selectionner le buffer MIL à afficher dans la fenetre */
    MdispSelectWindow(pMatrox->MilDisplay, (pMatrox->Camera[iCam])->buffer, ClientWndHandle);

    /********************************************
    if ((pMatrox->Camera[iCam])->digitizer)
    {
        // Put the digitizer in asynchronous mode.
        MdigControl((pMatrox->Camera[iCam])->digitizer, M_GRAB_MODE, M_ASYNCHRONOUS);

        // Grab the first buffer.
        MdigGrab((pMatrox->Camera[iCam])->digitizer, (pMatrox->Camera[iCam])->buffer1);
    }
    ********************************************/

    return true ;
}
catch (...){    erreur("Exception InitCamera.", standardError, 0);    return false ;}
}

bool
FreeCamera(NSMatrox far *pMatrox, int far noCamera)
{
try
{
    char msg[255];
    int iCam = pMatrox->IndexCamera(noCamera);
    if (iCam == -1)
    {
        sprintf(msg, "FreeCamera : La camera %d n'a pas été initialisée", noCamera);
        erreur(msg, standardError, 0);
        return false;
    }

    // on libère les buffers
    if ((pMatrox->Camera[iCam])->buffer)
    {        MdispDeselect(pMatrox->MilDisplay, (pMatrox->Camera[iCam])->buffer);        MbufFree((pMatrox->Camera[iCam])->buffer);
    }

    //if ((pMatrox->Camera[iCam])->buffer1)
    //{    //    MbufFree((pMatrox->Camera[iCam])->buffer1);    //}

    //if ((pMatrox->Camera[iCam])->buffer2)
    //{    //    MbufFree((pMatrox->Camera[iCam])->buffer2);    //}

    // on libère le digitizer (caméra)
    if ((pMatrox->Camera[iCam])->digitizer)
        MdigFree((pMatrox->Camera[iCam])->digitizer);

    // on détruit l'objet caméra
    pMatrox->RemoveCamera(noCamera);

    return true;
}
catch (...){    erreur("Exception FreeCamera.", standardError, 0) ;    return false ;}
}

void
StopCamera(NSMatrox far *pMatrox, int far noCamera)
{
try
{
    char msg[255];
    int iCam = pMatrox->IndexCamera(noCamera);
    if (iCam == -1)
    {
        sprintf(msg, "FreeCamera : La camera %d n'a pas été initialisée", noCamera);
        erreur(msg, standardError, 0);
        return;
    }

    if ((pMatrox->Camera[iCam])->digitizer)
        MdigHalt((pMatrox->Camera[iCam])->digitizer);
}
catch (...){    erreur("Exception StopCamera.", standardError, 0) ;}}

bool
GrabFrame(NSMatrox far *pMatrox, int far noCamera)
{
try
{
    char msg[255];
    int iCam = pMatrox->IndexCamera(noCamera);
    if (iCam == -1)
    {
        sprintf(msg, "GrabFrame : La camera %d n'a pas été initialisée", noCamera);
        erreur(msg, standardError, 0);
        return false;
    }

    if ((pMatrox->Camera[iCam])->digitizer)
        MdigGrabContinuous((pMatrox->Camera[iCam])->digitizer, (pMatrox->Camera[iCam])->buffer);

    /******************************************
    // Si OK, on grab...
    if ((pMatrox->Camera[iCam])->digitizer)
    {
        // Grab second buffer while processing first buffer.
        MdigGrab((pMatrox->Camera[iCam])->digitizer, (pMatrox->Camera[iCam])->buffer2);
        MbufCopy((pMatrox->Camera[iCam])->buffer1, (pMatrox->Camera[iCam])->buffer);

        // Grab first buffer while processing second buffer.
        MdigGrab((pMatrox->Camera[iCam])->digitizer, (pMatrox->Camera[iCam])->buffer1);
        MbufCopy((pMatrox->Camera[iCam])->buffer2, (pMatrox->Camera[iCam])->buffer);
    }
    else
    {
        sprintf(msg, "GrabFrame : La camera %d n'a pas de digitizer associé", noCamera);
        erreur(msg, standardError, 0);
        return false;
    }
    ****************************************/

    return true;
}
catch (...){    erreur("Exception GrabFrame.", standardError, 0) ;    return false ;}
}

bool
CaptureImage(NSMatrox far *pMatrox, int far noCamera, string far filename, NSContexte far *pCtx)
{
try
{
    char msg[255];
    char pathname[255];

    int iCam = pMatrox->IndexCamera(noCamera);
    if (iCam == -1)
    {
        sprintf(msg, "CaptureImage : La camera %d n'a pas été initialisée", noCamera);
        erreur(msg, standardError, 0);
        return false;
    }

    // on stocke l'image dans download
    string sPathName = pCtx->PathName("IHTM");
    sPathName += filename;
    strcpy(pathname, sPathName.c_str());
    MbufExport(pathname, M_BMP, (pMatrox->Camera[iCam])->buffer);

    return true;
}
catch (...){    erreur("Exception CaptureImage.", standardError, 0) ;    return false ;}
}

bool
GrabOneBuffer(NSMatrox far *pMatrox, int far noCamera)
{
try
{
    char msg[255];
    int iCam = pMatrox->IndexCamera(noCamera);
    if (iCam == -1)
    {
        sprintf(msg, "GrabOneBuffer : La camera %d n'a pas été initialisée", noCamera);
        erreur(msg, standardError, 0);
        return false;
    }

    /* Si OK, on grab... */
    if ((pMatrox->Camera[iCam])->digitizer)
    {
        MdigGrab((pMatrox->Camera[iCam])->digitizer, (pMatrox->Camera[iCam])->buffer1);
        MbufCopy((pMatrox->Camera[iCam])->buffer1, (pMatrox->Camera[iCam])->buffer);
    }
    else
    {
        sprintf(msg, "GrabOneBuffer : La camera %d n'a pas de digitizer associé", noCamera);
        erreur(msg, standardError, 0);
        return false;
    }

    return true;
}
catch (...){    erreur("Exception GrabOneBuffer.", standardError, 0) ;    return false ;}
}

int
NbCamera(NSMatrox far *pMatrox)
{
    return pMatrox->nbCamera;
}

void
PanImage(NSMatrox far *pMatrox, long far XOffset, long far YOffset)
{
try
{
    MdispPan(pMatrox->MilDisplay, XOffset, YOffset);
}
catch (...){    erreur("Exception PanImage.", standardError, 0) ;}}

void
ZoomImage(NSMatrox far *pMatrox, long far XFactor, long far YFactor)
{
try
{
    MdispZoom(pMatrox->MilDisplay, XFactor, YFactor);
}
catch (...){    erreur("Exception ZoomImage.", standardError, 0) ;}
}

// Fonctions de conversion de format d'image
bool
ConvertBmpToJpg(string far *filename, NSContexte far *pCtx)
{
try
{
    FIBITMAP *dib = NULL;
    FIBITMAP *dib2 = NULL;
    bool bRes;
    char *directory = new char[128];
    string sPathName = pCtx->PathName("IHTM");
    sPathName += (*filename);
    strcpy(directory, sPathName.c_str());

    dib = FreeImage_LoadBMP(directory);

    sPathName = *filename;
    size_t pos = sPathName.find('.');
    if (pos == NPOS)
        return false;

    sPathName = string(sPathName, 0, pos);
    sPathName += string(".jpg");
    sPathName = pCtx->PathName("IHTM") + sPathName;
    strcpy(directory, sPathName.c_str());

    // pCtx->getSuperviseur()->bTrace = true;

    if (dib != NULL)
    {
        unsigned int bits = 0;
        char szBits[50];
        string sMsg;

        bits = FreeImage_GetBPP(dib);
        sprintf(szBits, "Nombre de bits de l'image : %d", bits);
        sMsg = string(szBits);
        pCtx->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails);

        if ((bits) > 24)
            dib2 = FreeImage_ConvertTo24Bits(dib);
        else
            dib2 = dib;

        if (dib2 != NULL)
        {
            if (FreeImage_SaveJPEG(dib2, directory))
            {
                FreeImage_Free(dib);
                bRes = true;
            }
            else
                bRes = false;
        }
        else
            bRes = false;
    }
    else
        bRes = false;

    delete [] directory;
    return bRes;
}
catch (...){    erreur("Exception ConvertBmpToJpg.", standardError, 0) ;    return false ;}
}


// fin de nsgrabfc.cpp
////////////////////////////////////////////////

