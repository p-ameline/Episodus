// --------------------------------------------------------------------------// NSDOCVIEW.H		Gestion du modèle Document/Vues - Doc/View model management
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------

#if !defined(__NSDOCVIEW_H)#define __NSDOCVIEW_H

#include "nsbb\docViewMgr.h"
/**
* Helper class for doc/view management
*/
class _NSBBCLASSE NSDocViewManager : public NSDocViewRootManager{	public:

		// Constructeur - Destructeur		NSDocViewManager(NSContexte* pCtx) ;
		~NSDocViewManager() ;

		TView* 				createView(TDocument* pDoc, string sDescription = "", TView* pView = 0) ;
};

#endif		// __NSDOCVIEW_H

