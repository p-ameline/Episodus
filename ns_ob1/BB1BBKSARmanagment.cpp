#ifndef _OWLDLL
# include "BB1BB.h"
# include "BB1KB.h"
# include "BB1KSAR.h"
# include "BB1Class.h"
# include "DPIODialog.h"
#else
# include "ns_bbk\BB1BB.h"
# include "ns_bbk\BB1KB.h"
# include "ns_bbk\BB1KSAR.h"
# include "ns_bbk\BB1Class.h"
# include "ns_bbk\DPIODialog.h"
#endif // _OWLDLL

#ifndef __TCONTROLER__

Errcode		BB1BB::KSARManagment()
{
#ifdef _DEBUG_
	cout << "BB1BB::KSARManagment()\n" ;
#endif
/*
	SetPhase(KSARS_MANAGMENT) ;

	// on ne parcourt pas toutes les KB car un objet de la classe "NeedDatas" se
	// trouve dans la KB "PatientDatas" ou la KB "DPIO"

	ObjectSet *needDatasObjects = SearchClassInKBs(string("NeedDatas"), KBNamed("PatientDatas"), KBNamed("DPIO")) ;

	for (ObjectCIter objectIter = needDatasObjects->begin() ; objectIter != needDatasObjects->end() ; objectIter++)
	{
		vector<BB1KSAR *>	*needDatasKSARs = new vector<BB1KSAR *> ;
		vector<BB1KSAR *>	*waitingKSARs		= new vector<BB1KSAR *> ;
		vector<BB1KSAR *>	*askuserKSARs		= new vector<BB1KSAR *> ;

		bool							bOneNoWaiting = false ;
		bool							bOneNoAskUser = false ;

		// no need to process in the Executed subAgenda, because the KSAR has
		// already been executed

		// no need to process in the Obviated subAgenda, the Obviated subAgenda is
		// deleted at each end of cycle

//		ProcessKSARsOnSubAgenda(agenda.TriggeredKSARs(),	*objectIter, &bOneNoWaiting, &bOneNoAskUser, needDatasKSARs) ;


		if (!agenda.TriggeredKSARs()->empty())
			for (KSARCIter iter = agenda.TriggeredKSARs()->begin() ; iter != agenda.TriggeredKSARs()->end() ; iter++)
			{
				if ((*objectIter)->LongName() == (*iter)->TriggerObject().LongName())
				{
					if ((*iter)->nsStatus == WAITUSERAUTH)
						waitingKSARs->push_back(*iter) ;
					else
						bOneNoWaiting = true ;

					/-
					if ((*iter)->nsStatus == ASKUSER)
						askuserKSARs->push_back(*iter) ;
					else
						bOneNoAskUser = true ;
					-/

					needDatasKSARs->push_back(*iter) ;
				}
			}

		if (!agenda.ExecutableKSARs()->empty())
			for (KSARCIter iter = agenda.ExecutableKSARs()->begin() ; iter != agenda.ExecutableKSARs()->end() ; iter++)
			{
				if ((*objectIter)->LongName() == (*iter)->TriggerObject().LongName())
				{
					if ((*iter)->nsStatus == WAITUSERAUTH)
						waitingKSARs->push_back(*iter) ;
					else
						bOneNoWaiting = true ;

					/-
					if ((*iter)->nsStatus == ASKUSER)
						askuserKSARs->push_back(*iter) ;
					else
						bOneNoAskUser = true ;
					-/

					needDatasKSARs->push_back(*iter) ;
				}
			}


		if (needDatasKSARs->empty() && !CheckAnswers(*objectIter))
		{
			// CheckAnswers vérifie qu'il n'y a pas eu de réponse à cette question

			// créer une réponse no answer
			//
			// puis gérer les réponses no answer dans les KSs pour faire remonter
			// l'information

			// avant de créer une réponse no-answer, essayer de lancer l'archetype
			// pour laisser le superviseur travailler

			// la tactique utilisée est de vérifier si la question est en askuser, si
			// elle est en askuser, on regarde si elle a un archetype, et si elle a un
			// archetype, si cet archetype est valide et la méthode BbkAskUser renvoie
			// true, alors on ne crée pas de réponse no-Answer
			// dans tous les autres cas, on répond avec une réponse noAnswer

			bool				nullBool = false ;
			bool				bNoAnswer = true ;

			if ((*objectIter)->Value("askuser", nullBool) == true)
			{
				string	sArchetype("") ;
				sArchetype = (*objectIter)->Value("archetype", NULLSTRING) ;
				if ((sArchetype != "") && pContexte->getSuperviseur()->BbkAskUser(sArchetype, pContexte))
					bNoAnswer = false ;
			}

			if (bNoAnswer)
			{
				BB1Class 		*nautilusDatas 	= ClassNamed("DatasFromNautilus") ;

				string			sLabel = (*objectIter)->Value("label", NULLSTRING) ;
				string			sAnswer("noAnswers") ;
				string			sName = sLabel + "-noanswer" ;
				AttValPair	label("label", sLabel) ;
				AttValPair	answer("answer", sAnswer) ;
				nautilusDatas->MakeInstance(sName, (*objectIter)->KB(), Collect(&label, &answer)) ;
			}
		}
		else
		{
			if (!bOneNoWaiting && !waitingKSARs->empty() && !CheckDPIODialog())
			{
				// lancer boite dialogue de choix des DPIO à faire
				// crée un événement sur le Blackboard pour ne pas relancer la boite de dialogue plusieurs fois
				BB1Class 		*nautilusDatas 	= ClassNamed("DatasFromNautilus") ;

				AttValPair	label("label", string("DPIO dialog")) ;
				AttValPair	answer("value", string("launched")) ;
				nautilusDatas->MakeInstance("DPIO-dialog", (*objectIter)->KB(), Collect(&label, &answer)) ;

				DPIODialog	*pDialog = new DPIODialog(pContexte, waitingKSARs) ;
				pDialog->Execute() ;

				delete pDialog ;

				GoNCycles(50);
			}

			/-
			if (!bOneNoAskUser && !askuserKSARs->empty())
			{
				// lancer boite de dialogue des valeurs de facteur de risque
				// crée un événement pour ne pas le faire plusieurs fois
			}
			-/
		}
		delete waitingKSARs ;
		delete askuserKSARs ;
		delete needDatasKSARs ;
	}

	// we must not use the destructor because it will delete each BB1Object
	// included in it !! so we clear all the iterator before delete the ObjectSet
	needDatasObjects->clear() ;
	delete needDatasObjects ;

//		if ((*objectIter)->nsStatus == WAITUSERAUTH) && ((*objectIter)->ExemplifiesP("DPIO")))
//			userAuthKSARs->push_back(*objectIter) ;

	return (SUCCESS) ;
*/
}
#endif

bool
BB1BB::CheckAnswers(BB1Object *obj)
{
	BB1KB *kb = KBNamed("PatientDatas") ;
  if (NULL == kb)
    return false ;

	const ObjectSet	*objects 	= kb->ObjectsInKB() ;
  if ((NULL == objects) || objects->empty())
    return false ;

	for (ObjectCIter iter = objects->begin() ; objects->end() != iter ; iter++)
	{
		AttValPair answers("answers", NULLOBJECT) ;
		if ((*iter)->HasAttributeP(answers))
			if ((*iter)->Value("answers", NULLOBJECT) == obj)
				return true ;
	}

	return false ;
}

bool
BB1BB::CheckDPIODialog()
{
	BB1KB						*kb 			= KBNamed("DPIO") ;
	const ObjectSet	*objects 	= kb->ObjectsInKB() ;

  if ((NULL == objects) || objects->empty())
    return false ;

	for (ObjectCIter iter = objects->begin() ; objects->end() != iter ; iter++)
		if (((*iter)->Value("label", NULLSTRING) == string("DPIO dialog")) && ((*iter)->Value("value", NULLSTRING) == string("launched")))
			return true ;

	return false ;
}

#ifndef __TCONTROLER__
bool
BB1BB::evalDPIODialog()
{
	SetPhase(KSARS_MANAGMENT) ;

	BB1KB *kb = KBNamed("DPIO") ;
  if (NULL == kb)
    return false ;

	const ObjectSet	*objects = kb->ObjectsInKB() ;
  if ((NULL == objects) || objects->empty())
    return false ;

	BB1Object	*DPIOobject    = (BB1Object*) 0 ;
	bool			 bOneNoWaiting = false ;

	for (ObjectCIter objectIter = objects->begin() ; objects->end() != objectIter ; objectIter++)
		if ((*objectIter)->Value("label", NULLSTRING) == string("DPIO"))
			DPIOobject = *objectIter ;

	if (NULL == DPIOobject)
		return false ;

  vector<BB1KSAR *>	waitingKSARs ;

	if (false == agenda.TriggeredKSARs()->empty())
		for (KSARCIter KSARiter = agenda.TriggeredKSARs()->begin() ; agenda.TriggeredKSARs()->end() != KSARiter ; KSARiter++)
			if (DPIOobject->LongName() == (*KSARiter)->TriggerObject().LongName())
			{
				if (WAITUSERAUTH == (*KSARiter)->nsStatus)
					waitingKSARs.push_back(*KSARiter) ;
				else
					bOneNoWaiting = true ;
			}

	if (false == agenda.ExecutableKSARs()->empty())
		for (KSARCIter KSARiter = agenda.ExecutableKSARs()->begin() ; agenda.ExecutableKSARs()->end() != KSARiter ; KSARiter++)
			if (DPIOobject->LongName() == (*KSARiter)->TriggerObject().LongName())
			{
				if (WAITUSERAUTH == (*KSARiter)->nsStatus)
					waitingKSARs.push_back(*KSARiter) ;
				else
					bOneNoWaiting = true ;
			}


	if ((false == bOneNoWaiting) && (false == waitingKSARs.empty()))
	{
		BB1Class 		*nautilusDatas 	= ClassNamed("DatasFromNautilus") ;

		AttValPair	label("label", string("DPIO dialog")) ;
		AttValPair	answer("value", string("launched")) ;

    AVPSet* pAttSpec = Collect(&label, &answer) ;
		nautilusDatas->MakeInstance("DPIO-dialog", *kb, pAttSpec) ;
    delete pAttSpec ;

		DPIODialog *pDialog = new DPIODialog(pContexte, &waitingKSARs) ;
		int	iRet = pDialog->Execute() ;
		delete pDialog ;

		addCycles2Do(50) ;

		return true ;
	}
	return false ;
}
#endif


ObjectSet*
BB1BB::SearchClassInKBs(string sClassLabel, BB1KB *kb1, BB1KB *kb2, BB1KB *kb3, BB1KB *kb4, BB1KB *kb5)
{
	ObjectSet	*result = new ObjectSet() ;

	if (kb1)
	{
		const ObjectSet *datasInKB = kb1->ObjectsInKB() ;

		if (datasInKB && (false == datasInKB->empty()))
			for (ObjectCIter iter = datasInKB->begin() ; datasInKB->end() != iter ; iter++)
				if ((*iter)->ExemplifiesP(sClassLabel))
					result->push_back(*iter) ;
	}

	if (kb2)
	{
		const ObjectSet *datasInKB = kb2->ObjectsInKB() ;

		if (datasInKB && (false == datasInKB->empty()))
			for (ObjectCIter iter = datasInKB->begin() ; datasInKB->end() != iter ; iter++)
				if ((*iter)->ExemplifiesP(sClassLabel))
					result->push_back(*iter) ;
	}

	if (kb3)
	{
		const ObjectSet *datasInKB = kb3->ObjectsInKB() ;

		if (datasInKB && (false == datasInKB->empty()))
			for (ObjectCIter iter = datasInKB->begin() ; datasInKB->end() != iter ; iter++)
				if ((*iter)->ExemplifiesP(sClassLabel))
					result->push_back(*iter) ;
	}

	if (kb4)
	{
		const ObjectSet *datasInKB = kb4->ObjectsInKB() ;

		if (datasInKB && (false == datasInKB->empty()))
			for (ObjectCIter iter = datasInKB->begin() ; datasInKB->end() != iter ; iter++)
				if ((*iter)->ExemplifiesP(sClassLabel))
					result->push_back(*iter) ;
	}

	if (kb5)
	{
		const ObjectSet *datasInKB = kb5->ObjectsInKB() ;

		if (datasInKB && (false == datasInKB->empty()))
			for (ObjectCIter iter = datasInKB->begin() ; datasInKB->end() != iter ; iter++)
				if ((*iter)->ExemplifiesP(sClassLabel))
					result->push_back(*iter) ;
	}

	return result ;
}

#ifndef __TCONTROLER__
void			BB1BB::ProcessKSARsOnSubAgenda(const KSARSet *subAgenda, BB1Object *obj, bool *bOneNoWaiting, bool *bOneNoAskUser, vector<BB1KSAR *> *needDatasKSARs)
{
	if (!subAgenda->empty())
		for (KSARCIter iter = subAgenda->begin() ; iter != subAgenda->end() ; iter++)
		{
			if ((*iter)->nsStatus != WAITUSERAUTH)
				(*bOneNoWaiting) = true ;

			if ((*iter)->nsStatus != ASKUSER)
				(*bOneNoAskUser) = true ;

			if (obj->LongName() == (*iter)->TriggerObject().LongName())
				needDatasKSARs->push_back(*iter) ;
		}
}
#endif


#ifndef __TCONTROLER__
// -----------------------------------------------------------------------------
// recherche sur le blackboard s'il y a eu une réponse à la question passée en
// paramètre
// -----------------------------------------------------------------------------

bool      BB1BB::AnswerOnBlackboard(BB1Object *object)
{
	if (object)
		if (object->Value("listofanswers", NULLOBJECT) != NULL)
			return true ;

	return false ;
}

bool
BB1BB::AnswerOnBlackboard(string sPath)
{
	ObjectSet *needDatasObjects = SearchClassInKBs(string("NeedDatas"), KBNamed("PatientDatas"), KBNamed("DPIO")) ;

  if (NULL == needDatasObjects)
  	return false ;
  if (needDatasObjects->empty())
  {
  	delete needDatasObjects ;
  	return false ;
  }

	for (ObjectCIter objectIter = needDatasObjects->begin() ; needDatasObjects->end() != objectIter ; objectIter++)
	{
		PathsList	*pPaths = (*objectIter)->Value("paths", NULLPATHS) ;
		if (pPaths && (false == pPaths->empty()))
			for (PathsIterator iter = pPaths->begin() ; pPaths->end() != iter ; iter++)
				if (((**iter) == sPath) && (AnswerOnBlackboard(*objectIter)))
				{
					// we must not use the destructor because it will delete each
					// BB1Object included in it !! so we clear all the iterator before
					// delete the ObjectSet
					needDatasObjects->clear() ;
					delete needDatasObjects ;
					return true ;
				}
	}

	// we must not use the destructor because it will delete each BB1Object
	// included in it !! so we clear all the iterator before delete the ObjectSet
	needDatasObjects->clear() ;
	delete needDatasObjects ;
	return false ;
}

// -----------------------------------------------------------------------------
// renvoie la réponse à la question passée en paramètre
// -----------------------------------------------------------------------------

NSPatPathoArray		*BB1BB::getAnswerOnBlackboard(BB1Object *object)
{
	if (object)
	{
		BB1Object	*answerObj = object->Value("listofanswers", NULLOBJECT) ;
		if ((answerObj != NULL) && (answerObj->ExemplifiesP("DatasFromNautilus") || answerObj->ExemplifiesP("DatasFromInitBlackboard")))
		{
			NSPatPathoArray	*result = answerObj->Value("node", NULLPATHO) ;
			if (result != NULL)
				return result ;        // FIXME PATHO
			else
				return NULLPATHO ;
		}
	}
	return NULLPATHO ;
}

int
BB1BB::getAnswer2Question(string sPath, string sArchetype, NSPatPathoArray **pAnswer, bool bCompute)
{
	// strategie
	// on cherche s'il y a une question avec ce chemin qui a déjà été posé
	// s'il y en une on renvoit sa réponse (si elle existe) par le pointeur en
	// paramètre
	// si la réponse n'existe pas (ou pas encore) on renvoit 0

	if (bCompute)
		ComputeQuestion(sPath, sArchetype, pAnswer) ;

	ObjectSet *needDatasObjects = SearchClassInKBs(string("NeedDatas"), KBNamed("PatientDatas"), KBNamed("DPIO")) ;

  if (NULL == needDatasObjects)
  	return 0 ;
  if (needDatasObjects->empty())
  {
  	delete needDatasObjects ;
  	return 0 ;
  }

	for (ObjectCIter objectIter = needDatasObjects->begin() ; needDatasObjects->end() != objectIter ; objectIter++)
	{
		PathsList	*pPaths = (*objectIter)->Value("paths", NULLPATHS) ;
		if (pPaths && (false == pPaths->empty()))
		{
			bool	bOneNoAnswer = false ;
			for (PathsIterator iter = pPaths->begin() ; pPaths->end() != iter ; iter++)
			{
				if ((**iter) == sPath)
				{
					// trace
					char		szTracelog[255] = "" ;
					sprintf(szTracelog, "La question a été trouvé path = <%s>", sPath.c_str()) ;
					string	sTraceLog = szTracelog ;
					pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;

					sprintf(szTracelog, "le pointeur de la question est <%u>", (*objectIter)) ;
					sTraceLog = szTracelog ;
					pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
					// end trace
				}

				if (((**iter) == sPath) && (AnswerOnBlackboard(*objectIter)))
				{
					NSPatPathoArray	*pTempAnswer = getAnswerOnBlackboard(*objectIter) ;    // FIXME PATHO
					if (NULLPATHO != pTempAnswer)
					{
						*pAnswer = new NSPatPathoArray(*pTempAnswer) ;

						// we must not use the destructor because it will delete each
						// BB1Object included in it !! so we clear all the iterator before
						// delete the ObjectSet
						needDatasObjects->clear() ;
						delete needDatasObjects ;

						// trace
						char		szTracelog[255] = "" ;
						sprintf(szTracelog, "Renvoi du résultat de calcul pour <%s>", sPath.c_str()) ;
						string	sTraceLog = szTracelog ;
						pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
						// end trace

						return 1 ;
					}

					if ((*objectIter)->Value("answer", NULLSTRING) == "noAnswers")
						bOneNoAnswer = true ;
				}
			}

			if (bOneNoAnswer)
			{
				// we must not use the destructor because it will delete each BB1Object
				// included in it !! so we clear all the iterator before delete the
				// ObjectSet
				needDatasObjects->clear() ;
				delete needDatasObjects ;

				// trace
				char		szTracelog[255] = "" ;
				sprintf(szTracelog, "Il n'y a pas de  résultat pour le calcul de <%s>", sPath.c_str()) ;
				string	sTraceLog = szTracelog ;
				pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
				// end trace

				return 1 ;
			}
		}
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// normalement il faut poser la question
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// we must not use the destructor because it will delete each BB1Object
	// included in it !! so we clear all the iterator before delete the ObjectSet
	needDatasObjects->clear() ;
	delete needDatasObjects ;

	// trace
	char		szTracelog[255] = "" ;
	sprintf(szTracelog, "On a pas trouvé de résultat pour le calcul de <%s>", sPath.c_str()) ;
	string	sTraceLog = szTracelog ;

	pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
	// end trace

	return 0 ;
}

bool
BB1BB::insertAnswerOnBlackboard(string sPath, NSPatPathoArray *pAnswer)
{
	// strategie
	// on recherche une question posé avace comme chemin sPath
	// une fois qu'on a trouve cette question, on lui repond avec pAnswer

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// on ne gère pas toutes les nuances sur le questions cette fonction sera a
	// retravailler
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	ObjectSet *pNeedDatasObjects = SearchClassInKBs(string("NeedDatas"), KBNamed("PatientDatas"), KBNamed("DPIO")) ;

  if (NULL == pNeedDatasObjects)
  	return false ;

  if (pNeedDatasObjects->empty())
  {
  	delete pNeedDatasObjects ;
  	return false ;
  }

	for (ObjectCIter objectIter = needDatasObjects->begin() ; needDatasObjects->end() != objectIter ; objectIter++)
	{
		PathsList	*pPaths = (*objectIter)->Value("paths", NULLPATHS) ;
		if (pPaths && (false == pPaths->empty()))
			for (PathsIterator iter = pPaths->begin() ; pPaths->end() != iter ; iter++)
				if ((**iter) == sPath)
				{
					BB1KB* patientDatas = KBNamed("PatientDatas") ;
					if (!AnswerOnBlackboard(*objectIter))
					{
						BB1Class* pNautilusDatas = ClassNamed("DatasFromNautilus") ;

						AttValPair label	("label",   string("answerFromNS-" + (*objectIter)->Value("label", NULLSTRING))) ;
						AttValPair answers("answers", *objectIter) ;
						AttValPair node		("node",    pAnswer) ;
						AttValPair sfrom	("sfrom",   string("answer from archetype")) ;

            AVPSet* pAttSpec = Collect(&label, &answers, &node) ;
						BB1AppInst *objectAnswer = pNautilusDatas->MakeInstance("answerFromNS-" + (*objectIter)->Value("label", NULLSTRING), *patientDatas, pAttSpec) ;
            delete pAttSpec ;

						AttValPair answerAVP("listofanswers", (BB1Object *) objectAnswer) ;
            
            AVPSet* pAttSpecMod = Collect(&answerAVP) ;
						(*objectIter)->Modify(pAttSpecMod) ;
            delete pAttSpecMod ;

						// we must not use the destructor because it will delete each
						// BB1Object included in it !! so we clear all the iterator before
						// delete the ObjectSet
						pNeedDatasObjects->clear() ;
						delete pNeedDatasObjects ;
						return true ;
					}
					else
					{
						const ObjectSet *datas = patientDatas->ObjectsInKB() ;

            if (datas && (false == datas->empty()))
              for (ObjectCIter datasIter = datas->begin() ; datas->end() != datasIter ; datasIter++)
              {
                if ((*datasIter)->ExemplifiesP("DatasFromNautilus") || (*datasIter)->ExemplifiesP("DatasFromInitBlackboard"))
                  if ((*datasIter)->Value("answers", NULLOBJECT) == *objectIter)
                  {
                    AttValPair node			("node", 		/*new NSPatPathoArray(* */ pAnswer) ;
                    AttValPair noanswer	("answer",	string("")) ;

                    // modification de la réponse
                    AVPSet* pAttSpecNode     = Collect(&node) ;
                    AVPSet* pAttSpecNoAnswer = Collect(&noanswer) ;
                    (*datasIter)->Modify(pAttSpecNode, (LinkSet*) 0, pAttSpecNoAnswer) ;
                    delete pAttSpecNode ;
                    delete pAttSpecNoAnswer ;

                    // ajout du lien de la question vers la réponse
                    AttValPair	listanswer("listofanswers", (BB1Object *)(*datasIter)) ;

                    AVPSet* pAttSpecListAnswer = Collect(&listanswer) ;
                    (*objectIter)->Modify(pAttSpecListAnswer) ;
                    delete pAttSpecListAnswer ;

                    // we must not use the destructor because it will delete each
                    // BB1Object included in it !! so we clear all the iterator
                    // before delete the ObjectSet
                    needDatasObjects->clear() ;
                    delete needDatasObjects ;
                    return true ;
                  }
              }
					}
				}
	}
	// we must not use the destructor because it will delete each BB1Object
	// included in it !! so we clear all the iterator before delete the ObjectSet
	needDatasObjects->clear() ;
	delete needDatasObjects ;

	return false ;
}

bool
BB1BB::CallReferentiel(string sPath)
{
//	pContexte->getSuperviseur()->BbkShowReferential(sPath, pContexte) ;
}

bool
BB1BB::ComputeQuestion(string sPath, string sArchetype, NSPatPathoArray **pAnswer)
{
	if (string("") == sPath)
  	return false ;

  // trace
  char	 szTracelog[255] = "" ;
  sprintf(szTracelog, "Demande de calcul pour <%s>", sPath.c_str()) ;
  string sTraceLog = szTracelog ;
  pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
  // end trace

  BB1KB				*kb			 	= KBNamed("PatientDatas") ;
  BB1Class		*nsclass 	= ClassNamed("Compute") ;
  BB1AppInst	*objInst	= NULL ;

  AttValPair	label			("label",			string("Compute-" + sPath)) ;
  AttValPair	path			("path",			sPath) ;
  AttValPair	archetype	("archetype", sArchetype) ;

  if (kb->ObjectNamed("Compute-" + sPath))
  {
    const ObjectSet *datas = kb->ObjectsInKB() ;
    if (datas && (false == datas->empty()))
    	for (ObjectCIter iter = datas->begin() ; datas->end() != iter ; iter++)
      	if ((*iter)->Name() == string("Compute-" + sPath))
        	(*iter)->Modify() ;
  }
  else
  {
    AVPSet* pAttSpec = Collect(&label, &path) ;
    objInst = nsclass->MakeInstance(string("Compute-" + sPath), *kb, pAttSpec) ;
    delete pAttSpec ;
  }

  GoNCycles(100) ;

  return true ;
}

bool
BB1BB::DPIOanswertrue(string sAnswer, string sRisque)
{
	string	sPath = sRisque + string("/") + string(sAnswer, 0, 5) ;

	// trace
	char		szTracelog[255] = "" ;
	sprintf(szTracelog, "DPIOAnswerTrue pour <%s> - risque <%s>", sAnswer.c_str(), sRisque.c_str()) ;
	string	sTraceLog = szTracelog ;

	pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
	// end trace

	// -------------------------------------------------------------------------
	// recherche de la question correspondante
	// -------------------------------------------------------------------------

	ObjectSet *needDatasObjects = SearchClassInKBs(string("NeedDatas"), KBNamed("PatientDatas"), KBNamed("DPIO")) ;

  if (NULL == needDatasObjects)
  	return false ;
  if (needDatasObjects->empty())
  {
  	delete needDatasObjects ;
  	return false ;
  }

  // -------------------------------------------------------------------------
	// création de la patpatho
	// -------------------------------------------------------------------------

	NSPatPathoArray	Answer(pContexte) ;   // FIXME PATHO
	Message	CodeMsg("") ;
	CodeMsg.SetLexique(sAnswer) ;
	Answer.ajoutePatho(sAnswer, &CodeMsg, 0, 1) ;

	for (ObjectCIter objectIter = needDatasObjects->begin() ; objectIter != needDatasObjects->end() ; objectIter++)
	{
		AttValPair	pathsAVP("paths", NULLPATHS) ;
		if ((*objectIter)->HasAttributeP(pathsAVP))
		{
			PathsList	*pPaths = (*objectIter)->Value("paths", NULLPATHS) ;
			if (pPaths && (false == pPaths->empty()))
				for (PathsIterator iter = pPaths->begin() ; pPaths->end() != iter ; iter++)
					if ((**iter) == sPath)
					{
						// ---------------------------------------------------------------
						// on a trouvé la question, on recherche une éventuelle réponse
						// ---------------------------------------------------------------

						// trace
						sprintf(szTracelog, "question trouvé pour <%s>", sPath.c_str()) ;
						sTraceLog = szTracelog ;

						pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
						// end trace

						BB1KB *patientDatas = KBNamed("PatientDatas") ;
						if (!(AnswerOnBlackboard(*objectIter)))
						{
							// -------------------------------------------------------------
							// la question n'a pas de réponse --> on en créé une
							// -------------------------------------------------------------

							BB1Class		*nautilusDatas 	= ClassNamed("DatasFromNautilus") ;

							AttValPair	answers	("answers",		*objectIter) ;							AttValPair	node		("node",			&Answer) ;
							AttValPair	label		("label", 		string("answer from DPIO KS" + sRisque)) ;

							// Creating answer - Création de la réponse
              //
              AVPSet* pAttSpec = Collect(&label, &answers, &node) ;
							BB1AppInst *objectAnswer = nautilusDatas->MakeInstance("answer from DPIO KS" + sRisque, *patientDatas, pAttSpec) ;
              delete pAttSpec ;

							// ajout du lien vers la réponse dans la question
							AttValPair	answerAVP	("listofanswers",	(BB1Object *) objectAnswer) ;

              AVPSet* pAttSpecAnswerAVP = Collect(&answerAVP) ;
							(*objectIter)->Modify(pAttSpecAnswerAVP) ;
              delete pAttSpecAnswerAVP ;

							// trace
							sprintf(szTracelog, "la question a été trouvé - on lui a mis une réponse pour <%s>", sPath.c_str()) ;
							sTraceLog = szTracelog ;

							pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;

							sprintf(szTracelog, "le pointeur de la question est <%u>", (*objectIter)) ;
							sTraceLog = szTracelog ;

							pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
							// end trace

							// we must not use the destructor because it will delete each
							// BB1Object included in it !! so we clear all the iterator before
							// deleting the ObjectSet
							needDatasObjects->clear() ;
							delete needDatasObjects ;
							return true ;
						}
						else
						{
							// -------------------------------------------------------------
							// on a trouvé une réponse --> on veut la modifier
							// -------------------------------------------------------------

							const ObjectSet *datas = patientDatas->ObjectsInKB() ;

              if (datas && (false == datas->empty()))
                for (ObjectCIter datasIter = datas->begin() ; datas->end() != datasIter ; datasIter++)
                {
                  if ((*datasIter)->ExemplifiesP("DatasFromNautilus") || (*datasIter)->ExemplifiesP("DatasFromInitBlackboard"))
                    if ((*datasIter)->Value("answers", NULLOBJECT) == *objectIter)
                    {
                      AttValPair	node			("node", 		pAnswer) ;
                      AttValPair	noanswer	("answer",	string("")) ;

                      AVPSet* pAttSpecNode     = Collect(&node) ;
                      AVPSet* pAttSpecNoAnswer = Collect(&noanswer) ;
                      (*datasIter)->Modify(pAttSpecNode, (LinkSet*) 0, pAttSpecNoAnswer) ;
                      delete pAttSpecNoAnswer ;
                      delete pAttSpecNode ;

                      AttValPair	answerAVP	("listofanswers",	(BB1Object *)(*datasIter)) ;

                      AVPSet* pAttSpecAnswerAVP = Collect(&answerAVP) ;
                      (*objectIter)->Modify(pAttSpecAnswerAVP) ;
                      delete pAttSpecAnswerAVP ;

                      // trace
                      sprintf(szTracelog, "la question a été trouvé - on vient de modifier la réponse pour <%s>", sPath.c_str()) ;
                      sTraceLog = szTracelog ;

                      pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;
                      // end trace

                      // we must not use the destructor because it will delete each
                      // BB1Object included in it !! so we clear all the iterator
                      // before delete the ObjectSet
                      needDatasObjects->clear() ;
                      delete needDatasObjects ;
                      return true ;
                    }
                }
						}
					}
		}
	}

	needDatasObjects->clear() ;
	delete needDatasObjects ;
	return false ;
}


bool
BB1BB::DPIOanswerfalse(string sAnswer, string sRisque)
{
	string	sPath = sRisque + string("/") + string(sAnswer, 0, 5) ;

	// -------------------------------------------------------------------------
	// recherche de la question correspondante
	// -------------------------------------------------------------------------

	ObjectSet *needDatasObjects = SearchClassInKBs(string("NeedDatas"), KBNamed("PatientDatas"), KBNamed("DPIO")) ;

  if (NULL == needDatasObjects)
  	return false ;
  if (needDatasObjects->empty())
  {
  	delete needDatasObjects ;
  	return false ;
  }

	for (ObjectCIter objectIter = needDatasObjects->begin() ; needDatasObjects->end() != objectIter ; objectIter++)
	{
		AttValPair	pathsAVP("paths", NULLPATHS) ;
		if ((*objectIter)->HasAttributeP(pathsAVP))
		{
			PathsList	*pPaths = (*objectIter)->Value("paths", NULLPATHS) ;
			if (pPaths && (false == pPaths->empty()))
				for (PathsIterator iter = pPaths->begin() ; pPaths->end() != iter ; iter++)
					if ((**iter) == sPath)
					{
						// -----------------------------------------------------------------
						// on a trouvé la question, on remplace le pointeur sur la réponse
						// par un pointeur nulle
						// on ne veut pas qu'il y ait de réponse
						// -----------------------------------------------------------------

						AttValPair answerAVP("listofanswers", (BB1Object*) 0) ;

            AVPSet* pAttSpecAnswerAVP = Collect(&answerAVP) ;
						(*objectIter)->Modify(pAttSpecAnswerAVP) ;
            delete pAttSpecAnswerAVP
					}
		}
	}

	needDatasObjects->clear() ;
	delete needDatasObjects ;
	return false ;
}

bool
BB1BB::DPIOanswer(string sQuestion, string sAnswer)
{
	string sLexique    = sAnswer ;
  string sComplement = string("") ;

  size_t iPos = sAnswer.find("/") ;
	if (string::npos != iPos)
	{
		string	sLexiqueTemp		= string(sAnswer, 0, iPos) ;
		string	sComplementTemp	= string(sAnswer, iPos + 1, strlen(sAnswer.c_str()) - (iPos + 1)) ;

		if ((('£' == sLexiqueTemp[0]) || ('6' == sLexiqueTemp[0])) && ('$' == sComplementTemp[0]))
		{
			sLexique		= sLexiqueTemp ;
			sComplement	= string(sComplementTemp, 1, strlen(sComplementTemp.c_str()) - 1) ;
		}
	}

	return (DPIOanswerComplement(sQuestion, sLexique, sComplement)) ;
}

bool
BB1BB::DPIOanswerComplement(string sQuestion, string sLexique, string sComplement)
{
	// -------------------------------------------------------------------------
	// recherche de la question correspondante
	// -------------------------------------------------------------------------

	ObjectSet *needDatasObjects = SearchClassInKBs(string("NeedDatas"), KBNamed("PatientDatas"), KBNamed("DPIO")) ;

  if (NULL == needDatasObjects)
  	return false ;
  if (needDatasObjects->empty())
  {
  	delete needDatasObjects ;
  	return false ;
  }

  // -------------------------------------------------------------------------
	// création de la patpatho
	// -------------------------------------------------------------------------

	NSPatPathoArray	Answer(pContexte) ; // FIXME PATHO
	Message					CodeMsg("") ;
	CodeMsg.SetLexique(sLexique) ;
	if (string("") != sComplement)
		CodeMsg.SetComplement(sComplement) ;
	pAnswer->ajoutePatho(sLexique, &CodeMsg, 0, 1) ;

	for (ObjectCIter objectIter = needDatasObjects->begin() ; needDatasObjects->end() != objectIter ; objectIter++)
	{
		AttValPair pathsAVP("paths", NULLPATHS) ;
		if ((*objectIter)->HasAttributeP(pathsAVP))
		{
			PathsList	*pPaths = (*objectIter)->Value("paths", NULLPATHS) ;
			if (pPaths && (false == pPaths->empty()))
				for (PathsIterator iter = pPaths->begin() ; pPaths->end() != iter ; iter++)
					if ((**iter) == sQuestion)
					{
						// ---------------------------------------------------------------
						// on a trouvé la question, on recherche une éventuelle réponse
						// ---------------------------------------------------------------

						BB1KB				*patientDatas 	= KBNamed("PatientDatas") ;
						if (!(AnswerOnBlackboard(*objectIter)))
						{
							// -------------------------------------------------------------
							// la question n'a pas de réponse --> on en créé une
							// -------------------------------------------------------------

							BB1Class		*nautilusDatas 	= ClassNamed("DatasFromNautilus") ;

							AttValPair	answers	("answers", *objectIter) ;							AttValPair	node		("node",		&Answer) ;
							AttValPair	label		("label", 	string("answer from DPIO KS @ " + sQuestion)) ;

							// Creating answer - Création de la réponse
              AVPSet* pAttSpec = Collect(&label, &answers, &node) ;
							BB1AppInst *objectAnswer = nautilusDatas->MakeInstance("answer from DPIO KS @ " + sQuestion, *patientDatas, pAttSpec) ;
              delete pAttSpec ;

							// ajout du lien vers la réponse dans la question
							AttValPair answerAVP("listofanswers",	(BB1Object *) objectAnswer) ;

              AVPSet* pAttSpecAnswerAVP = Collect(&answerAVP) ;
							(*objectIter)->Modify(pAttSpecAnswerAVP) ;
              delete pAttSpecAnswerAVP ;

							// we must not use the destructor because it will delete each
							// BB1Object included in it !! so we clear all the iterator before
							// delete the ObjectSet
							needDatasObjects->clear() ;
							delete needDatasObjects ;
							return true ;
						}
						else
						{
							// -------------------------------------------------------------
							// on a trouvé une réponse --> on veut la modifier
							// -------------------------------------------------------------

							const ObjectSet *datas = patientDatas->ObjectsInKB() ;

							for (ObjectCIter datasIter = datas->begin() ; datas->end() != datasIter ; datasIter++)
							{
								if ((*datasIter)->ExemplifiesP("DatasFromNautilus") || (*datasIter)->ExemplifiesP("DatasFromInitBlackboard"))
									if ((*datasIter)->Value("answers", NULLOBJECT) == *objectIter)
									{
										AttValPair node			("node", 	 &Answer) ;
										AttValPair noanswer	("answer", string("")) ;

                    AVPSet* pAttSpecNode     = Collect(&node) ;
                    AVPSet* pAttSpecNoAnswer = Collect(&noanswer) ;
										(*datasIter)->Modify(pAttSpecNode, (LinkSet*) 0, pAttSpecNoAnswer) ;
                    delete pAttSpecNode ;
                    delete pAttSpecNoAnswer ;

										AttValPair	answerAVP	("listofanswers",	(BB1Object *)(*datasIter)) ;

                    AVPSet* pAttSpecAnswerAVP = Collect(&answerAVP) ;
										(*objectIter)->Modify(pAttSpecAnswerAVP) ;
                    delete pAttSpecAnswerAVP ;

										// we must not use the destructor because it will delete each
										// BB1Object included in it !! so we clear all the iterator
										// before delete the ObjectSet
										needDatasObjects->clear() ;
										delete needDatasObjects ;
										return true ;
									}
							}
						}
					}
		}
	}

	needDatasObjects->clear() ;
	delete needDatasObjects ;
	return false ;
}

#endif

