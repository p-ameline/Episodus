// -----------------------------------------------------------------------------
// ns_vidal.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: fabrice $
// $Date: 2003/07/10 09:51:55 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------
// DLL de récupération de données médicamenteuses à partir du VIDAL
// -----------------------------------------------------------------------------


#ifndef __NS_VIDAL_H__
# define __NS_VIDAL_H__

# include <bwcc.h>

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

#endif  // __NS_VIDAL_H__
