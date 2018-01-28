#ifndef __NSGRABFC_H
#include "ns_grab\mil.h"
#include "ns_grab\mwinmil.h"
#include "partage\ns_vector.h"

class NSContexte;
class ParamArray;

class NSCamera
{
    public:

        long    digNum;
        long    channel;
        string  format;
        MIL_ID  digitizer;
        MIL_ID  buffer;
        MIL_ID  buffer1;
        MIL_ID  buffer2;
        HWND    handleClient;
        int     numCam;

        NSCamera(int noCamera, string dataFormat, HWND handle);
        ~NSCamera();

        NSCamera(const NSCamera& rv);
        NSCamera& operator=(const NSCamera& src);

        void AssociateChannel(string sChannel);
};

typedef vector<NSCamera*>              NSCameraVector ;
typedef NSCameraVector::iterator       NSCameraIter ;
class NSCameraArray : public NSCameraVector
		// Constructeurs
		// Destructeur

class NSMatrox
{
    public:

        MIL_ID  MilApplication;
        MIL_ID  MilSystem;
        MIL_ID  MilDisplay;
        NSCameraArray Camera;
        int nbCamera;
        ParamArray* pParamArray;

        NSMatrox(NSContexte* pCtx);
        ~NSMatrox();

        NSMatrox(const NSMatrox& rv);
        NSMatrox& operator=(const NSMatrox& src);
        void LireParams(NSContexte* pCtx);
        int IndexCamera(int noCamera);
        void AddCamera(int noCamera, string format, HWND handle);
        void RemoveCamera(int noCamera);
};

// Fonctions de capture Matrox
bool _export GrabInit(NSMatrox far **ppMatrox, NSContexte far *pCtx);