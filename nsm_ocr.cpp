// DLL EPISODUS
///////////////////////////////////////////////////////////
#include <vcl.h>
#pragma hdrstop

#include <shellapi.h>
#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\owlpch.h>

#define _MAIN
#define __MAIN

#include "ns_ocr\ns_ocr.h"

USEUNIT("ns_ocr\nsocrfct.cpp");
USEUNIT("ns_ocr\nsocruti.cpp");
USEUNIT("ns_ocr\src\block.c");
USEUNIT("ns_ocr\src\find.c");
USEUNIT("ns_ocr\src\gui.c");
USEUNIT("ns_ocr\src\hash.c");
USEUNIT("ns_ocr\src\image.c");
USEUNIT("ns_ocr\src\init.c");
USEUNIT("ns_ocr\src\list.c");
USEUNIT("ns_ocr\src\modules.c");
USEUNIT("ns_ocr\src\lib_pixel.c");
USEUNIT("ns_ocr\src\pixel.c");
USEUNIT("ns_ocr\src\print.c");
USEUNIT("ns_ocr\src\recog.c");
USEUNIT("ns_ocr\src\text.c");
USEUNIT("ns_ocr\src\unicode.c");
USEUNIT("ns_ocr\src\lib_unicode.c");
USEUNIT("ns_ocr\src\pgm2asc.c");
USEUNIT("ns_ocr\src\ocr0.c");
USEUNIT("ns_ocr\src\ocr0n.c");
USEUNIT("ns_ocr\src\ocr1.c");
USEUNIT("ns_ocr\src\lines.c");
USEUNIT("ns_ocr\src\output.c");
USEUNIT("ns_ocr\src\otsu.c");
USEUNIT("ns_ocr\src\pnm.c");
USEUNIT("ns_ocr\src\pcx.c");
USEUNIT("ns_ocr\src\database.c");
USEUNIT("ns_ocr\src\box.c");
USEUNIT("ns_ocr\src\remove.c");
USEUNIT("ns_ocr\src\detect.c");
USERC("ns_ocr\ns_ocr.rc");
USELIB("nsm_sgbd.lib");
USELIB("nsmepisod.lib");
USELIB("nsmutil.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("nsmsavoir.lib");
USELIB("ns_capture.lib");
USERES("nsm_ocr.res");
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinstDll, unsigned long fdwReason, void*)
{
    switch (fdwReason)
    {
        case DLL_THREAD_ATTACH :
        case DLL_THREAD_DETACH :
      	    // cas des threads : on ne fait rien
      	    break;

        case DLL_PROCESS_ATTACH :
      	    // 1er appel : on instancie pResMod
      	    if (!pResMod)
   			    pResMod = new TModule(0, hinstDll);

            // si le new echoue : on renvoie false
      	    if (!pResMod)
      		    return 0;

            pNSResModule = pResMod;

            break;

        case DLL_PROCESS_DETACH :
      	    // dernier appel : on libere pResMod
       	    if (pResMod)
      	    {
      		    delete pResMod;
         	    pResMod = 0;
      	    } 
            break;

        default :
      	    return 0;
    }

	return 1;
}

