//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1997, 1998 by Borland International, All Rights Reserved
//
// ADOPT.CPP
//
// Implements the function for adopting the VCL's hidden main window
// as a child of an OWL window.
//----------------------------------------------------------------------------
// #include <owl/pch.h>
#include <vcl\forms.hpp>
#include "nsldv\adopt.h"

//
// Associate VCL form with an OWL parent.
//
void AdoptVCLAppWindow(HWND newParent)
{
  SetParent(Application->Handle, newParent);
  return;
}

