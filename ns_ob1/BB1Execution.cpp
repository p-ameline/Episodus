/*
** Copyright Nautilus, (10/9/2004)
** fabrice.le_perru@nautilus-info.com
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/

#include "ns_ob1\BB1BB.h"
#include "ns_ob1\AttValPair.h"
#include "ns_ob1\BB1Object.h"
#include "ns_ob1\BB1Class.h"
#include "ns_ob1\BB1KS.h"
#include "ns_ob1\OB1Controler.h"
#include "ns_ob1\BB1KB.h"
#include "ns_ob1\OB1Token.h"

#ifdef _ENTERPRISE_DLL
  #include "ob1Lus\InterfaceLus.h"
  #include "enterpriseLus\superLus.h"
  #include "enterpriseLus\nsdivfctForCgi.h"
#else
  #include "ns_ob1\Interface.h"
  #include "nautilus\nssuper.h"
#endif

void
BB1BB::RunInOpportunisticMode(int nb_tour)
{
	_kenoby.RunInOpportunisticMode(nb_tour) ;
}

BB1Object*
BB1BB::find(TypedVal& path, std::string clas, TypedVal *pSearchParams, bool depp)
{
	BB1Object* result = (BB1Object *) 0 ;

  OB1Token* temp = _kenoby.find(path, clas) ;
  if ((OB1Token*) temp)
    return temp->getObject() ;

  if ((true == depp) && (false == _kbs.empty()))
		//
		for (BB1List<BB1KB*>::iterator iter = _kbs.begin() ; (NULL == result) && (_kbs.end() != iter) ; iter ++)
    {
      string sName = (*iter)->Name() ;
    	result = searchInKB(path, sName, pSearchParams) ;
    }

	return result ;
}

BB1Object*
BB1BB::searchInKB(TypedVal& question, std::string& ksName, TypedVal *pSearchParams)
{
  BB1KB	*EventKB = KBNamed(ksName) ;
	if ((BB1KB*) NULL == EventKB)
		return (BB1Object *) 0 ;

  const ObjectSet *datas = EventKB->ObjectsInKB() ;
	if (((ObjectSet*) NULL == datas) || (datas->empty()))
		return (BB1Object *) 0 ;

	for (ObjectCIter iter = datas->begin() ; datas->end() != iter ; iter++)
  	if (*iter)
    {
    	TypedVal* pTV = (*iter)->PathAttributes() ;
      if ((pTV) && (question == *pTV))
      {
        TypedVal* pSP = (*iter)->SearchParams() ;
        if ((NULL == pSearchParams) || (NULL == pSearchParams->getSearchPrm()) || pSearchParams->getSearchPrm()->isDefault())
        {
          if ((NULL == pSP) || (NULL == pSP->getSearchPrm()) || pSP->getSearchPrm()->isDefault())
            return (*iter) ;
        }
        else
        {
          if ((pSP) && (pSP->getSearchPrm()))
          {
            NSSearchStruct* pModelSearch = pSearchParams->getSearchPrm() ;
            if (pSP->getSearchPrm()->isSameAs(*pModelSearch))
	 			      return (*iter) ;
          }
        }
      }
    }

  return (BB1Object *) 0 ;
}

BB1Object*
BB1BB::searchAnswerForQuestion(BB1Object* pQuestion)
{
  if ((BB1Object*) NULL == pQuestion)
    return (BB1Object*) 0 ;

  TypedVal* pDispinf = pQuestion->PathAttributes() ;
  if ((TypedVal*) NULL == pDispinf)
    return (BB1Object*) 0 ;

  return searchAnswerInKB(*pDispinf, pQuestion->SearchParams()) ;
}

BB1Object*
BB1BB::askQuestion(std::string& /* sQuestion */, bool /* createToken */)
{
  return (BB1Object *) 0 ;
}

bool
BB1BB::PutInformation(BB1Object* /* respp */)
{
  return true ;
}

std::vector<OB1NKS*>*
BB1BB::getKSOfType(KSType typ)
{
	OB1Graph& temp = Controler()->Graph() ;
  return temp.getKSOfType(typ) ;
}

void
BB1BB::getValidKs(std::vector<OB1NKS*>* ks, ValidityContextType type)
{
	if (((std::vector<OB1NKS*>*) NULL == ks) || (ks->empty()))
		return ;

  for (std::vector<OB1NKS* >::iterator iter = ks->begin() ; ks->end() != iter ; )
	{
		(*iter)->ComputeValidity(NULL) ;
    if ((*iter)->ContextValidity() != type)
    	ks->erase(iter) ;
    else
    	iter++ ;
	}
}

void
BB1BB::testOtherNodes()
{
  _kenoby.Graph().testOtherNodes() ;
}

void
BB1BB::getKSOfKB(std::vector<OB1NKS*>* ks, std::string sKbName)
{
	if (((std::vector<OB1NKS*>*) NULL == ks) || (ks->empty()))
		return ;

	for (std::vector<OB1NKS*>::iterator iter = ks->begin(); ks->end() != iter ; )
	{
    if ((NULL == *iter) ||
        (NULL == (*iter)->getKS()) ||
  	    ((*iter)->getKS()->KB().Name() != sKbName))
    	ks->erase(iter) ;
    else
    	iter++ ;
  }
}

ObjectSet*
BB1BB::getObjectsInKb(string sKbName)
{
  BB1KB	*EventKB = KBNamed(sKbName) ;
	if ((BB1KB *) NULL == EventKB)
    return (ObjectSet*) 0 ;

  return (ObjectSet*) EventKB->ObjectsInKB() ;
}

bool
BB1BB::Execute()
{
  // Incremente the count of the cycle int the BB
  IncrementCycle() ;

  // Execute the action
  _kenoby.Execute() ;

  return (true) ;
}

bool
BB1BB::giveAnswerToQuestion(TypedVal* pPathVal, NSPatPathoArray* pAnswerPatho, string sAnswerDate, bool bExt, HWND originatingInterfaceHandle, TypedVal *pSearchParams)
{
	BB1Object* pAnswer = searchAnswerInKB(*pPathVal, pSearchParams) ;
  if ((BB1Object*) NULL == pAnswer)
	{
  	// Creation of the answer
		//
		BB1KB		 *informationKB = KBNamed("InformationKB") ;
		BB1Class *answer        = ClassNamed("Answer") ;

		AttValPair label      ("label",       string("Follow up answer")) ;
		AttValPair sfrom      ("sfrom",       string("BB1BB::giveAnswerToQuestion")) ;
		AttValPair AnswExplica("explication", *pPathVal) ;
		//
		// true : because we don't duplicate respat, so we must not delete respat
		//
		NSPatPathoArray* respat = (NSPatPathoArray *) 0 ;

		AttValPair node("node", respat, true) ;

		std::string sId = "default" ;
		char nb[10] ;
		itoa(getNBObject(), nb, 10) ;
		sId.append(nb) ;

    AVPSet* pAttSpec = Collect(&label, &node, &sfrom) ;
		pAnswer = answer->MakeInstance(sId, AnswExplica, *informationKB, pAttSpec, (LinkSet*) 0, false) ;
    delete pAttSpec ;

  	string sActionLog = string("New answer") ;
  	if (NULL == pAnswer)
		{
  		sActionLog += string(" (creation failed)") ;
      if (pContexte->getBBinterface())
			  pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trError) ;

      if (pAnswerPatho)
        delete pAnswerPatho ;

    	return false ;
  	}

		sActionLog += string(" (creation succeeded)") ;
    if (pContexte->getBBinterface())
		  pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;

		pAnswer->setAnswerStatus(AnswerStatus::astatusProcessed, originatingInterfaceHandle) ;
  	pAnswer->setAnswerProcessStage(AnswerStatus::apstageUnknown) ;
  }

  bool bPathPathoChanged = false ;
  NSPatPathoArray* pCurrentAnswer = getPatPatho2(pAnswer, std::string("node")) ;
  if (pCurrentAnswer && (false == pCurrentAnswer->estEgal(pAnswerPatho)))
    bPathPathoChanged = true ;

	if (pAnswer->updatePatPatho(pAnswerPatho, sAnswerDate, bExt) && bPathPathoChanged)
  {
    bool bVerbose = true ;

    AnswerStatus* pAnswerStatus = pAnswer->Value("answerStatus", NULLLANSWERSTATUS) ;
    if (pAnswerStatus && ((AnswerStatus::apverboseSilent        == pAnswerStatus->VerboseStatus()) ||
                          (AnswerStatus::apverboseSilentUpdated == pAnswerStatus->VerboseStatus())))
    {
      bVerbose = false ;
      pAnswerStatus->setVerboseStatus(AnswerStatus::apverboseSilentUpdated) ;
    }

    if (bVerbose)
      alertClientsForAnswerUpdate(pPathVal, originatingInterfaceHandle) ;
  }

  return true ;
}

void
BB1BB::alertClientsForAnswerUpdate(TypedVal* pPathVal, HWND originatingInterfaceHandle, TypedVal *pSearchParams)
{
  if ((TypedVal*) NULL == pPathVal)
    return ;

  string ps = string("BB1BB::alertClientsForAnswerUpdate") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  BB1Object* pQuestion = searchQuestionInKB(*pPathVal, pSearchParams) ;
  if ((BB1Object*) NULL == pQuestion)
    return ;

  vector<HANDLE> *pInterfacesHandles = pQuestion->Value("interfaces", NULLHANDLES) ;
  if (((vector<HANDLE>*) NULL == pInterfacesHandles) || pInterfacesHandles->empty())
    return ;

  vector<HANDLE>::iterator it = pInterfacesHandles->begin() ;
  for ( ; pInterfacesHandles->end() != it ; it++)
    if ((*it) && (originatingInterfaceHandle != (HWND)(*it)) && (pContexte->getBBinterface()))
      pContexte->getBBinterface()->askForWindowUpdate((HWND)(*it)) ;
}

void
BB1BB::suscribeHandleToQuestion(BB1Object* pQuestion, HWND interfaceHandle, bool bWithCounter)
{
  if ((BB1Object*) NULL == pQuestion)
    return ;

  TypedVal* pTypedVal = pQuestion->InterfacesHandlesAttributes() ;
  if ((TypedVal*) NULL == pTypedVal)
    return ;

  if (bWithCounter)
    pTypedVal->addHandleWithCounter((HANDLE)interfaceHandle) ;
  else
    pTypedVal->addHandle((HANDLE)interfaceHandle) ;
}

// -----------------------------------------------------------------------
//
//  Removes this interface handle from the registered list
//  If the list is empty afterward, the Question is deleted
//
//  Returns : true if Question was deleted, false elsewhere
//
// -----------------------------------------------------------------------
bool
BB1BB::unsuscribeHandleToQuestion(BB1Object* pQuestion, HWND interfaceHandle, bool bWithCounter)
{
  if ((BB1Object*) NULL == pQuestion)
    return false ;

  TypedVal* pTypedVal = pQuestion->InterfacesHandlesAttributes() ;
  if ((TypedVal*) NULL == pTypedVal)
    return false ;

  if (bWithCounter)
    pTypedVal->removeHandleWithCounter((HANDLE) interfaceHandle) ;
  else
    pTypedVal->removeHandle((HANDLE) interfaceHandle) ;

  if (false == pQuestion->isOrphanQuestion())
    return false ;

  // No longer any interface suscribed to this question, we delete it
  //
  deleteOrphanQuestion(pQuestion) ;

  return true ;
}

void
BB1BB::unsuscribeHandle(HWND interfaceHandle)
{
  ObjectSet *pObjects = getQuestions() ;
  if (((ObjectSet*) NULL == pObjects) || pObjects->empty())
    return ;

  for (ObjectIter i = pObjects->begin() ; pObjects->end() != i ; )
    if (false == unsuscribeHandleToQuestion(*i, interfaceHandle))
      i++ ;
}

void
BB1BB::suscribeTokenToQuestion(BB1Object* pQuestion, OB1Token* pToken)
{
  if (((BB1Object*) NULL == pQuestion) || ((OB1Token*) NULL == pToken))
    return ;

  TypedVal* pTypedVal = pQuestion->TokensHandlesAttributes() ;
  if ((TypedVal*) NULL == pTypedVal)
    return ;

  pTypedVal->addHandle(reinterpret_cast<HANDLE>(pToken->_token_numero)) ;
}

bool
BB1BB::unsuscribeTokenToQuestion(BB1Object* pQuestion, OB1Token* pToken)
{
  if (((BB1Object*) NULL == pQuestion) || ((OB1Token*) NULL == pToken))
    return false ;

  TypedVal* pTypedVal = pQuestion->TokensHandlesAttributes() ;
  if ((TypedVal*) NULL == pTypedVal)
    return false ;

  HANDLE hToken = reinterpret_cast<HANDLE>(pToken->_token_numero) ;
  if (false == pTypedVal->isThisHandleHere(hToken))
    return false ;

  string sActionLog = string("Unsuscribing Token ") + IntToString(pToken->getNumero()) + string(" from Question ") + pQuestion->Name() ;
  pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

  pTypedVal->removeHandle(hToken) ;

  if (false == pQuestion->isOrphanQuestion())
    return false ;

  // No longer any interface suscribed to this question, we delete it
  //
  deleteOrphanQuestion(pQuestion) ;

  return true ;
}

void
BB1BB::unsuscribeToken(OB1Token* pToken)
{
  ObjectSet *pObjects = getQuestions() ;
  if (((ObjectSet*) NULL == pObjects) || pObjects->empty())
  {
    string sActionLog = string("No question to delete when unsuscribing Token ") + IntToString(pToken->getNumero()) ;
    pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

    return ;
  }

  for (ObjectIter i = pObjects->begin() ; pObjects->end() != i ; )
    if (false == unsuscribeTokenToQuestion(*i, pToken))
      i++ ;
}

void
BB1BB::suscribePersistenceToQuestion(BB1Object* pQuestion, long int iPersistenceHandle)
{
  if ((BB1Object*) NULL == pQuestion)
    return ;

  TypedVal* pTypedVal = pQuestion->PersistenceHandlesAttributes() ;
  if ((TypedVal*) NULL == pTypedVal)
    return ;

  pTypedVal->addHandle((HANDLE)iPersistenceHandle) ;
}

// -----------------------------------------------------------------------
//
//  Removes this persistence handle from the registered list
//  If the list is empty afterward, the Question is deleted
//
//  Returns : true if Question was deleted, false elsewhere
//
// -----------------------------------------------------------------------
bool
BB1BB::unsuscribePersistenceToQuestion(BB1Object* pQuestion, long int iPersistenceHandle)
{
  if (NULL == pQuestion)
    return false ;

  TypedVal* pTypedVal = pQuestion->PersistenceHandlesAttributes() ;
  if ((TypedVal*) NULL == pTypedVal)
    return false ;

  pTypedVal->removeHandle((HANDLE) iPersistenceHandle) ;

  if (false == pQuestion->isOrphanQuestion())
    return false ;

  // No longer any interface suscribed to this question, we delete it
  //
  deleteOrphanQuestion(pQuestion) ;

  return true ;
}

void
BB1BB::unsuscribePersistence(long int iPersistenceHandle)
{
  ObjectSet *pObjects = getQuestions() ;
  if (((ObjectSet*) NULL == pObjects) || pObjects->empty())
    return ;

  for (ObjectIter i = pObjects->begin() ; pObjects->end() != i ; )
    if (false == unsuscribePersistenceToQuestion(*i, iPersistenceHandle))
      i++ ;
}

void
BB1BB::deleteOrphanQuestion(BB1Object* pQuestion)
{
  if ((BB1Object*) NULL == pQuestion)
    return ;

  // First, remove token(s) from the controler
  //
  Controler()->deleteAllTokensForObject(pQuestion) ;

  // Then, delete the question
  //
  BB1KB *EventKB = KBNamed(std::string("QuestionKB")) ;
	if ((BB1KB*) NULL == EventKB)
    return ;

  TypedVal *pQuestionPath = pQuestion->PathAttributes() ;
  string sQuestionPath = pQuestionPath->getString() ;

  string sQuestionName = pQuestion->Name() ;

  string sActionLog = string("Deleting orphan question ") + sQuestionName ;
  pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

  // Here, we delete the question, don't use any related pointer after this line
  //
  EventKB->DeleteObjectNamed(sQuestionName) ;

  // Delete answer for this question
  //
  EventKB = KBNamed(std::string("InformationKB")) ;
	if (NULL == EventKB)
    return ;

  sActionLog = string("Deleting answers (for path ") + sQuestionPath + string(") of question ") + sQuestionName ;
  pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trDetails) ;

  EventKB->DeleteObjectForPath(sQuestionPath) ;
}

void
BB1BB::challengeQuestionsWhenPatPathoSaved(const NSPatPathoArray* pPPT)
{
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return ;

  ObjectSet *pObjects = getQuestions() ;
  if (((ObjectSet*) NULL == pObjects) || pObjects->empty())
    return ;

  if (pContexte->getBBinterface())
  {
    string sActionLog = string("Challenging questions after a patpatho was saved") ;
    pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSteps) ;
  }

  for (ObjectIter i = pObjects->begin() ; pObjects->end() != i ; i++)
  {
    BB1Object* pAnswer = searchAnswerForQuestion(*i) ;

    if (pAnswer)
    {
      // This question was answered, it is challenged
      //
      if (pAnswer->getAnswerStatus() == AnswerStatus::astatusProcessed)
      {
        // Reset status
        //
        AnswerStatus* pAnswerStatus = pAnswer->Value("answerStatus", NULLLANSWERSTATUS) ;
        if (pAnswerStatus && (pAnswerStatus->UpdatabilityStatus() != AnswerStatus::updatabilityNever))
        {
          if (pContexte->getBBinterface())
          {
            string sActionLog = string("Question ") + (*i)->Name() + string(" is asked to refresh") ;
            pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
          }

          pAnswerStatus->initializeForNewAttempt() ;

          // Ask it not to refresh interfaces (in order to do it just once at the end)
          //
          pAnswerStatus->setVerboseStatus(AnswerStatus::apverboseSilent) ;

          // Reset answer
          //
          pAnswer->updatePatPatho((NSPatPathoArray *) 0, string("")) ;

          // Compute again
          //
          Controler()->NextComputeAction(*i, NULL) ;
        }
      }
      //
      // This question was being answered but the process was stopped by
      // function resetProcessingTokens ; we have to start a new process
      //
      else if (pAnswer->getAnswerStatus() == AnswerStatus::astatusProcessing)
      {
        if (pContexte->getBBinterface())
        {
          string sActionLog = string("Calling Question ") + (*i)->Name() + string(" again") ;
          pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
        }

        Controler()->NextComputeAction(*i, NULL) ;
      }
      else
      {
        if (pContexte->getBBinterface())
        {
          string sActionLog = string("Question ") + (*i)->Name() + string(" not asked to refresh because of status ") + pAnswer->getAnswerStatusAsString() ;
          pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSubSteps) ;
        }
      }
    }
  }

  // Time to ask related interfaces to update
  //
  if (pContexte->getBBinterface())
  {
    string sActionLog = string("Updating interfaces after a patpatho was saved") ;
    pContexte->getBBinterface()->addActionString(sActionLog, BB1BBInterface::trSteps) ;
  }

  vector<HANDLE> aUpdatedHandles ;

  for (ObjectIter i = pObjects->begin() ; pObjects->end() != i ; i++)
  {
    BB1Object* pAnswer = searchAnswerForQuestion(*i) ;

    // Check if answers has verbose status apverboseSilentUpdated
    //
    if (pAnswer && (pAnswer->getAnswerStatus() == AnswerStatus::astatusProcessed))
    {
      AnswerStatus* pAnswerStatus = pAnswer->Value("answerStatus", NULLLANSWERSTATUS) ;
      if (pAnswerStatus && (AnswerStatus::apverboseSilentUpdated == pAnswerStatus->VerboseStatus()) &&
                           (AnswerStatus::updatabilityNever      != pAnswerStatus->UpdatabilityStatus()))
      {
        // Check interfaces linked to this question and not already updated
        //
        vector<HANDLE> *pInterfacesHandles = (*i)->Value("interfaces", NULLHANDLES) ;
        if (pInterfacesHandles && (false == pInterfacesHandles->empty()))
        {
          vector<HANDLE>::iterator it = pInterfacesHandles->begin() ;
          for ( ; pInterfacesHandles->end() != it ; it++)
            if ((*it) && (pContexte->getBBinterface()))
            {
              bool bNeedToUpdate = true ;

              if (false == aUpdatedHandles.empty())
                for (vector<HANDLE>::iterator ih = aUpdatedHandles.begin() ; aUpdatedHandles.end() != ih ; ih++)
                  if ((*it) == (*ih))
                    bNeedToUpdate = false ;

              if (bNeedToUpdate)
              {
                pContexte->getBBinterface()->askForWindowUpdate((HWND)(*it)) ;
                aUpdatedHandles.push_back(*it) ;
              }
            }
        }
      }
    }
  }

  if (pContexte->getBBinterface())
    pContexte->getBBinterface()->RefreshObjectsList() ;
}

