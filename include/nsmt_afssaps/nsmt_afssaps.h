// -----------------------------------------------------------------------------
// nsmt_afssaps.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2015/02/07 14:45:18 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------
// DLL de récupération de données médicamenteuses à partir du VIDAL
// -----------------------------------------------------------------------------


#ifndef  __NSMT_AFSSAPS_H__
# define __NSMT_AFSSAPS_H__

# include <bwcc.h>

#include "nautilus\nssuper.h"

void _export nsmtoolsInformation(string far *psToolName, string far *psDescription) ;void _export nsmToolsAction(TWindow far *pere, NSContexte far *pCtx) ;
# ifdef _MAIN
#  ifndef __MAIN
extern TModule	*pNSResModule ;
#  else
static TModule	*pResMod ;
TModule					*pNSResModule ;
#	 endif
# else
extern TModule	*pNSResModule ;
# endif

#endif  // __NSMT_AFSSAPS_H__

