#ifndef __CONSOLE_TOOL__
#define  __CONSOLE_TOOL__

#include "nautilus\nautilus.h"
#include <owl\appdict.h>

void ConsoleOut(std::string temp)
{
	 ((TMyApp*)GetApplicationObject())->voidDebugPrintf(temp.c_str());
}


#endif
