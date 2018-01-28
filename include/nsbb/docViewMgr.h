// --------------------------------------------------------------------------// NSDOCVIEW.H		Gestion du modèle Document/Vues - Doc/View model management
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------

#if !defined(__DOCVIEWMGR_H)#define __DOCVIEWMGR_H

#include <owl\docmanag.h>#include <owl\docview.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsbb\nsexport.h"

/**
* Helper class for doc/view management
*/
class _NSBBCLASSE NSDocViewRootManager : public NSRoot{	public:

		// Constructeur - Destructeur		NSDocViewRootManager(NSContexte* pCtx) ;
		~NSDocViewRootManager() ;

    TDocTemplate* findTemplateByDesc(string sDescription) ;
    TDocTemplate* getNextTemplate(TDocTemplate* tpl) ;
    TDocTemplate* getFirstTemplate() ;
    void          attachTemplate(TDocTemplate& ) ;

    TDocManager*  getManager() { return pDocManager ; }

	protected:

  	TDocManager* 	pDocManager ;
};

#endif		// __DOCVIEWMGR_H

