#include <windows.h>#include <owl/window.h>
#include <list.h>

#include "ns_grab\nsgrabfc.h"#include "ns_grab\ezTwain.h"#include "nautilus\nssuper.h"
bool TwainAcquire(string far filename, HWND far ClientWndHandle)
{
	unsigned wPixTypes = TWAIN_ANYTYPE ;
	HANDLE hdib = TWAIN_AcquireNative(ClientWndHandle, wPixTypes) ;
	if (hdib)
	{
  	// HPALETTE hpal = TWAIN_CreateDibPalette(hdib) ;
		int result = TWAIN_WriteNativeToFilename(hdib, filename.c_str()) ;
    TWAIN_FreeNative(hdib) ;

    if (result < -1)
    {
    	erreur("Error writing image file.", standardError, 0) ;
      return false ;
    }
    return true ;
	}
	return false ;
}

int TwainSelectSource(HWND far ClientWndHandle)
{
	return TWAIN_SelectImageSource(ClientWndHandle) ;
}

// fin de nsgrabtwain.cpp
////////////////////////////////////////////////

