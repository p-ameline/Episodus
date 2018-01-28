//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------

//#include <windows.h>

#ifndef __NS_BBK_H__
# define __NS_BBK_H__

# include <bwcc.h>

//void _export lancerBBK(NSContexte* pCtx) ;

# ifdef _MAIN
#  ifndef __MAIN
extern TModule	*pNSResModule ;
extern TModule	*pNSDLLModule ;
#  else
static TModule	*pResMod ;
TModule					*pNSResModule ;
TModule					*pNSDLLModule ;
#  endif // __MAIN
# else
extern TModule	*pNSResModule ;
extern TModule	*pNSDLLModule ;
# endif // _MAIN

#endif // __NS_BBK_H__
