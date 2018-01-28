// -----------------------------------------------------------------------------
// nsframedocs.cpp
// -----------------------------------------------------------------------------
// $Author: pameline $
// $Date: 2015/02/07 13:00:24 $
// -----------------------------------------------------------------------------
// Frames Index Documents
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#ifndef _ENTERPRISE_DLL
  #include "nautilus\nssuper.h"
#else
  #include "enterpriseLus\superLus.h"
  #include "enterpriseLus\nsdivfctForCgi.h"
#endif

#include "nsdn\nsdochis.h"
#include "nsdn\nsframedocs.h"

#include "nsbb\nspatpat.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsfilgd.h"

//-----------------------------------------------------------------------//
//         						Classe NSFrameInformation                          //
//-----------------------------------------------------------------------//

NSFrameDocuments::NSFrameDocuments(NSDocumentsTank* pDocumentsTank, NSContexte *pCtx, LDVFRAME iFrame, NSDocumentHisto *pDocHis)
                 :NSRoot(pCtx)
{
  _pDocumentsTank = pDocumentsTank ;
  _iFrame         = iFrame ;
  _pIndexDoc      = pDocHis ;

  loadSubTrees() ;
}

NSFrameDocuments::~NSFrameDocuments()
{
}

NSFrameDocuments::NSFrameDocuments(const NSFrameDocuments& rv)
                 :NSRoot(rv.pContexte)
{
  _pDocumentsTank = rv._pDocumentsTank ;
  _iFrame         = rv._iFrame ;
  _pIndexDoc      = rv._pIndexDoc ;

  if (false == rv._aConcernDocs.empty())
	  for (DocumentConstIter i = rv._aConcernDocs.begin() ; rv._aConcernDocs.end() != i ; i++)
		  _aConcernDocs.push_back(*i) ;

  if (false == rv._aTreatmentDocs.empty())
	  for (DocumentConstIter i = rv._aTreatmentDocs.begin() ; rv._aTreatmentDocs.end() != i ; i++)
		  _aTreatmentDocs.push_back(*i) ;

  if (false == rv._aGoalDocs.empty())
	  for (DocumentConstIter i = rv._aGoalDocs.begin() ; rv._aGoalDocs.end() != i ; i++)
		  _aGoalDocs.push_back(*i) ;
}

void
NSFrameDocuments::loadSubTrees()
{
  if ((NULL == _pIndexDoc) || (NULL == _pDocumentsTank))
    return ;

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  _pIndexDoc->initFromPatPatho(&PPT) ;

  if (PPT.empty())
    return ;

  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pGraphManager)
    return ;

  PatPathoIter iter = PPT.begin() ;
  string sRootId = (*iter)->getDoc() ;

  string sMetaId = pGraphManager->getMetaIdFromDataId(sRootId) ;

  loadSubTreesForDataType(sMetaId, &_aConcernDocs,   NSRootLink::indexConcerns) ;
  loadSubTreesForDataType(sMetaId, &_aTreatmentDocs, NSRootLink::drugOf) ;
  loadSubTreesForDataType(sMetaId, &_aGoalDocs,      NSRootLink::indexGoals) ;
}

void
NSFrameDocuments::loadSubTreesForDataType(string sDocRoot, NSDocumentHistoArray *pTank, NSRootLink::NODELINKTYPES iRelation)
{
  if ((string("") == sDocRoot) || (NULL == pTank) || (NULL == _pDocumentsTank))
    return ;

  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pGraphManager)
    return ;

  NSLinkManager* pGraphe = pGraphManager->getLinkManager() ;
  if (NULL == pGraphe)
    return ;

  VecteurString aVecteurString ;
  pGraphe->TousLesVrais(sDocRoot, iRelation, &aVecteurString) ;
  if (aVecteurString.empty())
  	return ;

  for (EquiItemIter i = aVecteurString.begin(); aVecteurString.end() != i; i++)
  {
    DocumentIter histoIter = _pDocumentsTank->TrouveDocHisto(**i) ;
    if (NULL != histoIter)
      pTank->push_back(*histoIter) ;
  }
}

NSDocumentHisto*
NSFrameDocuments::findNode(string sNodeId)
{
  if (string("") == sNodeId)
    return (NSDocumentHisto*) 0 ;

  if (_pIndexDoc)
  {
    NSPatPathoArray* pPPT = _pIndexDoc->getPatPathoPointer() ;

    if (pPPT && (false == pPPT->empty()))
    {
      PatPathoIter pptIter = pPPT->ChercherNoeud(sNodeId) ;
      if ((NULL != pptIter) && (pPPT->end() != pptIter))
        return _pIndexDoc ;
    }
  }

  NSDocumentHisto* pFoundDoc = findNodeInArray(sNodeId, &_aConcernDocs) ;
  if (pFoundDoc)
    return pFoundDoc ;

  pFoundDoc = findNodeInArray(sNodeId, &_aTreatmentDocs) ;
  if (pFoundDoc)
    return pFoundDoc ;

  pFoundDoc = findNodeInArray(sNodeId, &_aGoalDocs) ;
  if (pFoundDoc)
    return pFoundDoc ;

  return (NSDocumentHisto*) 0 ;
}

NSDocumentHisto*
NSFrameDocuments::findNodeInArray(string sNodeId, NSDocumentHistoArray* pDocsArray)
{
  if ((NULL == pDocsArray) || pDocsArray->empty())
    return (NSDocumentHisto*) 0 ;

  for (DocumentIter i = pDocsArray->begin() ; pDocsArray->end() != i ; i++)
  {
    if (*i)
    {
      NSPatPathoArray* pPPT = (*i)->getPatPathoPointer() ;

      if (pPPT && (false == pPPT->empty()))
      {
        PatPathoIter pptIter = pPPT->ChercherNoeud(sNodeId) ;
        if ((NULL != pptIter) && (pPPT->end() != pptIter))
          return *i ;
      }
    }
  }

  return (NSDocumentHisto*) 0 ;
}

void
NSFrameDocuments::addDocumentInTank(NSDocumentHisto* pDoc, NSDocumentHistoArray *pTank)
{
  if ((NULL == pDoc) || (NULL == pTank))
    return ;

  // Check if this document is not already there
  //
  if (false == pTank->empty())
  {
    for (DocumentIter i = _aConcernDocs.begin() ; _aConcernDocs.end() != i ; i++)
      if ((*i)->getCodeDocMeta() == pDoc->getCodeDocMeta())
        return ;
  }

  // Add document to array
  //
  pTank->push_back(pDoc) ;
}

NSFrameDocuments&
NSFrameDocuments::operator=(const NSFrameDocuments& src)
{
try
{
	if (this == &src)
		return *this ;

  _iFrame         = src._iFrame ;
  _pDocumentsTank = src._pDocumentsTank ;
  _pIndexDoc      = src._pIndexDoc ;

  if (false == _aConcernDocs.empty())
	  for (DocumentIter i = _aConcernDocs.begin() ; _aConcernDocs.end() != i ; i++)
		  _aConcernDocs.erase(i) ;

  if (false == src._aConcernDocs.empty())
	  for (DocumentConstIter i = src._aConcernDocs.begin() ; src._aConcernDocs.end() != i ; i++)
		  _aConcernDocs.push_back(*i) ;

  if (false == _aTreatmentDocs.empty())
	  for (DocumentIter i = _aTreatmentDocs.begin() ; _aTreatmentDocs.end() != i ; i++)
		  _aTreatmentDocs.erase(i) ;

  if (false == src._aTreatmentDocs.empty())
	  for (DocumentConstIter i = src._aTreatmentDocs.begin() ; src._aTreatmentDocs.end() != i ; i++)
		  _aTreatmentDocs.push_back(*i) ;

  if (false == _aGoalDocs.empty())
	  for (DocumentIter i = _aGoalDocs.begin() ; _aGoalDocs.end() != i ; i++)
		  _aGoalDocs.erase(i) ;

  if (false == src._aGoalDocs.empty())
	  for (DocumentConstIter i = src._aGoalDocs.begin() ; src._aGoalDocs.end() != i ; i++)
		  _aGoalDocs.push_back(*i) ;

	return *this ;
}
catch (...)
{
#ifndef _ENTERPRISE_DLL
	erreur("Exception NSFrameInformation::operator=", standardError, 0) ;
#else
  erreur("Exception NSFrameInformation::operator=", standardError) ;
#endif
	return *this ;
}
}

//-----------------------------------------------------------------------//
//        					Classe NSFrameInformationArray                       //
//-----------------------------------------------------------------------//

NSFrameDocumentsArray::NSFrameDocumentsArray(NSDocumentsTank* pDocumentsTank)
                      :NSVectFrameDocuments()
{
  _pDocumentsTank = pDocumentsTank ;
}

NSFrameDocumentsArray::NSFrameDocumentsArray(const NSFrameDocumentsArray& rv)
                      :NSVectFrameDocuments()
{
try
{
  _pDocumentsTank = rv._pDocumentsTank ;

  if (false == rv.empty())
    for (FrameDocsConstIter it = rv.begin() ; rv.end() != it ; it++)
      push_back(new NSFrameDocuments(*(*it))) ;
}
catch (...)
{
#ifndef _ENTERPRISE_DLL
  erreur("Exception NSFrameDocumentsArray copy ctor.", standardError, 0) ;
#else
  erreur("Exception NSFrameDocumentsArray copy ctor.", standardError) ;
#endif
}
}

NSFrameDocumentsArray::~NSFrameDocumentsArray()
{
  vider() ;
}

void
NSFrameDocumentsArray::vider()
{
  if (empty())
    return ;

  for (FrameDocsIter it = begin() ; end() != it ; )
	{
		delete *it ;
		erase(it) ;
	}
}

void
NSFrameDocumentsArray::openFrameInformation(LDVFRAME iFrame)
{
  // If already opened, nothing to be done
  //
  NSFrameDocuments* pFrameDocs = getFrameDocuments(iFrame) ;
  if (pFrameDocs)
    return ;

  if (NULL == _pDocumentsTank)
    return ;

  NSContexte *pCtx = _pDocumentsTank->pContexte ;

  NSDocHistoArray* pVectDocument = _pDocumentsTank->getVectDocument() ;
  if ((NULL == pVectDocument) || pVectDocument->empty())
    return ;

  string sFrameRoot = getRootForFrame(iFrame) ;
  string sRootSens ;
  pCtx->getDico()->donneCodeSens(&sFrameRoot, &sRootSens) ;

  bool bContinuer = true ;
  DocumentIter iterDoc = pVectDocument->begin() ;
  while ((pVectDocument->end() != iterDoc) && bContinuer)
  {
    NSPatPathoArray FramePpt(pCtx->getSuperviseur()) ;
    (*iterDoc)->initFromPatPatho(&FramePpt) ;

    if (false == FramePpt.empty())
    {
      PatPathoIter iter = FramePpt.begin() ;
      if ((*iter)->getLexiqueSens() == sRootSens)
        bContinuer = false ;
    }

    if (bContinuer)
      iterDoc++ ;
  }

  if (pVectDocument->end() == iterDoc)
    return ;

  push_back(new NSFrameDocuments(_pDocumentsTank, pCtx, iFrame, *iterDoc)) ;
}

NSFrameDocuments*
NSFrameDocumentsArray::getFrameDocuments(LDVFRAME iFrame)
{
  if (empty())
    return (NSFrameDocuments*) 0 ;

  for (FrameDocsIter i = begin() ; end() != i ; i++)
    if ((*i)->getFrame() == iFrame)
      return *i ;

  return (NSFrameDocuments*) 0 ;
}

NSFrameDocumentsArray&
NSFrameDocumentsArray::operator=(const NSFrameDocumentsArray& src)
{
  if (this == &src)
		return *this ;

try
{
  _pDocumentsTank = src._pDocumentsTank ;

	vider() ;

	if (false == src.empty())
		for (FrameDocsConstIter it = src.begin() ; src.end() != it ; it++)
			push_back(new NSFrameDocuments(*(*it))) ;

	return *this ;
}
catch (...)
{
#ifndef _ENTERPRISE_DLL
	erreur("Exception NSFrameDocumentsArray= .", standardError, 0) ;
#else
  erreur("Exception NSFrameDocumentsArray= .", standardError) ;
#endif
	return *this ;
}
}

