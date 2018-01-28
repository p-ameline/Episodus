// --------------------------------------------------------------------------// NSDOCVIEW.CPP		Helper for Document/View operations
// --------------------------------------------------------------------------

#include "nautilus\nssuper.h"
#include "nsbb\docViewMgr.h"

// REGISTRATION_FORMAT_BUFFER(100)

// --------------------------------------------------------------------------
//
// Class NSDocViewManager
//
// --------------------------------------------------------------------------

// Constructeur
NSDocViewRootManager::NSDocViewRootManager(NSContexte* pCtx)
          			     :NSRoot(pCtx)
{
try
{
	pDocManager = 0 ;

	if (NULL == pContexte)
		return ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;
  if ((NULL == pSuper) || (NULL == pSuper->getApplication()))
  	return ;

	pDocManager = pSuper->getApplication()->GetDocManager() ;
}
catch (...)
{
	erreur("Exception NSDocViewRootManager ctor.", standardError, 0) ;
}
}

// DestructeurNSDocViewRootManager::~NSDocViewRootManager()
{
}

TDocTemplate*
NSDocViewRootManager::findTemplateByDesc(string sDescription)
{
	TDocTemplate* tpl = getFirstTemplate() ;
	for ( ; tpl && (string(tpl->GetDescription()) != sDescription);
  																						tpl = getNextTemplate(tpl)) ;
  return tpl ;
}

/**
* Get next template in template tank
*/
TDocTemplate*
NSDocViewRootManager::getNextTemplate(TDocTemplate* tpl)
{
	if (!tpl)
  	return pDocManager->GetNextTemplate(0) ;

  // To prevent infinite cycle when tpl == tplNext
	TDocTemplate* tplNext = pDocManager->GetNextTemplate(tpl) ;
	if (tpl == tplNext)
		return NULL ;

	return tplNext ;
}

/**
* Get the first template in template tank
*/
TDocTemplate*
NSDocViewRootManager::getFirstTemplate()
{
	return getNextTemplate(0) ;
}

void
NSDocViewRootManager::attachTemplate(TDocTemplate& tpl)
{
	if (getFirstTemplate() == DocTemplateStaticHead)
		pDocManager->AttachTemplate(tpl) ;
}

