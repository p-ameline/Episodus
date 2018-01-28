/*
** Copyright Nautilus, (10/9/2004)
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
#include "ns_ob1\toolbox.h" // For function protypes
#include "ns_ob1\BB1Object.h" // For BB1Object
#include "nsepisod\nsldvuti.h"// For Time function
#include "ns_ob1\BB1Types.h"  // For NULLPATHO
#include "ns_ob1\BB1Class.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "nsbbLus/nslistLus.h"
  #include "ob1Lus/InterfaceLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "nsbb\nslistwind.h"
  #include "ns_ob1\Interface.h"
#endif

#include "nsbb\nsbbtran.h"

#include "nssavoir\nsfilgd.h"

const string		 NULLSTRING	        = string("") ;
int							 NULLINT		        = 0 ;
Voidptr					 NULLPTR		        = NULL ;
long             NULLLONG           = 0 ;
NSPatPathoArray  *NULLPATHO         = (NSPatPathoArray *) NULL ;
PatPathoIter     *NULLPATPATHOITER  = (PatPathoIter *) NULL;
NautilusQuestion *NULLLNAUTQUEST    = (NautilusQuestion *) NULL ;
AnswerStatus     *NULLLANSWERSTATUS = (AnswerStatus *) NULL ;
PathsList		     *NULLPATHS         = (PathsList *) NULL ;
BB1Object				 *NULLOBJECT	      = (BB1Object *) NULL ;
NautilusEvent		 *NULLNAUTILUSEVENT	= (NautilusEvent *) NULL ;
NSSearchStruct   *NULLSEARCHSTRUCT  = (NSSearchStruct *) NULL ;
vector<HANDLE>   *NULLHANDLES       = (vector<HANDLE> *) 0 ;

string
getStringAnswer(const BB1BB& bb, BB1Object* pAnswerObject)
{
	if (NULL == pAnswerObject)
		return string("") ;

	// First, check if there is a "value string"
  //
	string sValue = pAnswerObject->Value("value", NULLSTRING) ;
  if (string("") != sValue)
		return sValue ;

	// Second, take the patpatho root
	//
	NSPatPathoArray* repo = getPatPatho2(pAnswerObject, std::string("node")) ;
  if ((NULLPATHO == repo) || (repo->empty()))
		return string("") ;

	PatPathoIter iter = repo->begin() ;

	string sResult = string("") ;
	string sTemp   = (*iter)->getLexique() ;
	bb.pContexte->getDico()->donneCodeSens(&sTemp, &sResult) ;

	return sResult ;
}

bool
getBoolAnswer(Transfert& reponse, std::string& sQuestion, std::string& sSearchParams)
{
	if (std::string("") == sQuestion)
		return false ;

  sQuestion = getRegularPath(sQuestion, cheminSeparationMARK, intranodeSeparationMARK) ;

	BB1Object* temp = reponse.getObject(sQuestion, sSearchParams) ;
  if (NULLOBJECT == temp)
  	return false ;

  // on recupere l'objet de la reponse
	NSPatPathoArray* repo = getPatPatho2(temp, std::string("node")) ;
  if ((NULLPATHO != repo) && (false == repo->empty()))
  	return true ;
  else
  	return false ;
}

bool
getIntAnswer(const BB1BB& bb, Transfert& reponse, string sQuestion, std::string& sSearchParams, string sRefUnit, int* iValue)
{
	if (NULL != iValue)
  	*iValue = 0 ;

	if (sQuestion == std::string(""))
		return false ;

  sQuestion = getRegularPath(sQuestion, cheminSeparationMARK, intranodeSeparationMARK) ;

	BB1Object* repo = reponse.getObject(sQuestion, sSearchParams) ;
	if (NULLOBJECT == repo)
  	return false ;

	std::string att = "node" ;
  NSPatPathoArray *pAnswer = getPatPatho2(repo, att) ;
  if ((NULL == pAnswer) || pAnswer->empty())
  	return false ;

	bool bAnswer = false ;

  PatPathoIter iter = pAnswer->begin() ;

  int iLigneBase = (*iter)->getLigne() ;

  string sUnite  = "" ;
  string sFormat = "" ;
  string sValeur = "" ;
  string sTemp   = "" ;

  while ((pAnswer->end() != iter) && ((*iter)->getLigne() == iLigneBase))
  {
    string sLexique = (*iter)->getLexique() ;
		if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
      sTemp = (*iter)->getLexique() ;
      bb.pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
      sValeur = (*iter)->getComplement() ;
      sTemp = (*iter)->getUnit() ;
      bb.pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
    }
		iter++ ;
	}

	if (sUnite == sRefUnit)
  {
  	*iValue = atoi(sValeur.c_str()) ;
    bAnswer = true ;
  }

  return bAnswer ;
}

bool
getDoubleAnswer(const BB1BB& bb, Transfert& reponse, string sQuestion, std::string& sSearchParams, string sRefUnit, double *dValue)
{
	if (NULL != dValue)
		*dValue = 0.0 ;

	if (sQuestion == std::string(""))
		return false ;

  sQuestion = getRegularPath(sQuestion, cheminSeparationMARK, intranodeSeparationMARK) ;

	BB1Object* repo = reponse.getObject(sQuestion, sSearchParams) ;
	if (NULLOBJECT == repo)
  	return false ;

  std::string att = "node";
  NSPatPathoArray *pAnswer = getPatPatho2(repo, att) ;
  if ((NULL == pAnswer) || pAnswer->empty())
  	return false ;

	bool bAnswer = false ;

  PatPathoIter iter = pAnswer->begin() ;
	int iLigneBase = (*iter)->getLigne() ;

	string sUnite  = "" ;
  string sFormat = "" ;
  string sValeur = "" ;
  string sTemp   = "" ;

  while ((pAnswer->end() != iter) && ((*iter)->getLigne() == iLigneBase))
  {
    string sLexique = (*iter)->getLexique() ;
		if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
    	sTemp = (*iter)->getLexique() ;
      bb.pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
      sValeur = (*iter)->getComplement() ;
      sTemp = (*iter)->getUnit() ;
      bb.pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
    }
    iter++ ;
  }

  if (sUnite == sRefUnit)
  {
  	*dValue = StringToDouble(sValeur) ;
    bAnswer = true ;
  }

  // delete pAnswer ;

  return bAnswer ;
}

bool
getIntervalAnswer(const BB1BB& bb, Transfert& reponse, string sQuestion, std::string& sSearchParams, string sRefUnit, string sInterval)
{
	if (string("") == sInterval)
		return false ;
  //
  // Parsing de l'intervalle
  //
  bool bExisteMaxQuestion = false ;
  int  iMaxValueQuestion ;
  bool bExisteMinQuestion = false ;
  int  iMinValueQuestion ;

  string sLeft  = string("") ;
  string sRight = string("") ;
  string sOper  = string("") ;
  int iTaille = strlen(sInterval.c_str()) ;
  int i       = 0;
  for ( ; (i < iTaille) && (sInterval[i] != '<') && (sInterval[i] != '>') && (sInterval[i] != '=') ; i++)
      sLeft += string(1, sInterval[i]) ;
  if (i < iTaille)
  {
  	for ( ; (i < iTaille) && ((sInterval[i] == '<') || (sInterval[i] == '>') || (sInterval[i] == '=')) ; i++)
    	sOper += string(1, sInterval[i]) ;
    if (i < iTaille)
    	sRight = string(sInterval, i, iTaille - i) ;
  }

  if (string("") != sOper)
  {
  	if ('>' == sOper[0])
    {
    	if (string("") != sLeft)
      {
      	iMaxValueQuestion = atoi(sLeft.c_str()) ;
        bExisteMaxQuestion = true ;
      }
      if (string("") != sRight)
      {
      	iMinValueQuestion = atoi(sRight.c_str()) ;
        bExisteMinQuestion = true ;
      }
    }
    else if ('<' == sOper[0])
    {
    	if (string("") != sLeft)
      {
      	iMinValueQuestion = atoi(sLeft.c_str()) ;
        bExisteMinQuestion = true ;
      }
      if (string("") != sRight)
      {
      	iMaxValueQuestion = atoi(sRight.c_str()) ;
        bExisteMaxQuestion = true ;
      }
    }
  }
  else
  {
  	iMaxValueQuestion = atoi(sLeft.c_str()) ;
    bExisteMaxQuestion = true ;
    iMinValueQuestion = atoi(sLeft.c_str()) ;
    bExisteMinQuestion = true ;
  }

  //
  //
  //

  string sUnite  = string("") ;
  string sFormat = string("") ;
  string sValeur = string("") ;

  sQuestion = getRegularPath(sQuestion, cheminSeparationMARK, intranodeSeparationMARK) ;

  BB1Object* repo = reponse.getObject(sQuestion, sSearchParams) ;
	if (NULL == repo)
  	return false ;

  std::string att = string("node") ;
  NSPatPathoArray *pAnswer  = getPatPatho2(repo, att);
  if (pAnswer && (false == pAnswer->empty()))
  {
  	PatPathoIter iter = pAnswer->begin() ;
    int iLigneBase = (*iter)->getLigne() ;

    string sTemp = string("") ;

    while ((pAnswer->end() != iter) && ((*iter)->getLigne() == iLigneBase))
    {
      string sLexique = (*iter)->getLexique() ;
		  if ((string("") != sLexique) && ('£' == sLexique[0]))
      {
      	sTemp = (*iter)->getLexique() ;
        bb.pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
        sValeur = (*iter)->getComplement() ;
        sTemp = (*iter)->getUnit() ;
        bb.pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
      }
			iter++ ;
		}
	}

	if ((string("") == sValeur) || (sUnite != sRefUnit))
  	return false ;

  //
  // Parsing de la valeur
  //
  bool    bExisteMaxReponse = false ;
  int     iMaxValueReponse ;
  bool    bExisteMinReponse = false ;
  int     iMinValueReponse ;

  string sLeft2  = string("") ;
  string sRight2 = string("") ;
  string sOper2  = string("") ;
  iTaille = strlen(sValeur.c_str()) ;
  i       = 0;
  for ( ; (i < iTaille) && (sValeur[i] != '<') && (sValeur[i] != '>') && (sValeur[i] != '=') ; i++)
  	sLeft2 += string(1, sValeur[i]) ;
  if (i < iTaille)
  {
  	for ( ; (i < iTaille) && ((sValeur[i] == '<') || (sValeur[i] == '>') || (sValeur[i] == '=')) ; i++)
    	sOper2 += string(1, sValeur[i]) ;
    if (i < iTaille)
    	sRight2 = string(sValeur, i, iTaille - i) ;
  }
  if (string("") != sOper2)
  {
  	if ('>' == sOper2[0])
    {
    	if (string("") != sLeft2)
      {
      	iMaxValueReponse = atoi(sLeft2.c_str()) ;
        bExisteMaxReponse = true ;
      }
      if (string("") != sRight2)
      {
      	iMinValueReponse = atoi(sRight2.c_str()) ;
        bExisteMinReponse = true ;
      }
    }
    else if ('<' == sOper2[0])
    {
    	if (string("") != sLeft2)
      {
      	iMinValueReponse = atoi(sLeft2.c_str()) ;
        bExisteMinReponse = true ;
      }
      if (string("") != sRight2)
      {
      	iMaxValueReponse = atoi(sRight2.c_str()) ;
        bExisteMaxReponse = true ;
      }
    }
    //
    //
    //
    if (bExisteMinQuestion)
    {
    	if ((false == bExisteMinReponse) || (iMinValueReponse < iMinValueQuestion))
      	return false ;
    }
    if (bExisteMaxQuestion)
    {
    	if ((false == bExisteMaxReponse) || (iMaxValueReponse > iMaxValueQuestion))
      	return false ;
    }
  }
  else
  {
  	int iValue = atoi(sLeft2.c_str()) ;
    if (bExisteMinQuestion && (iValue > iMinValueQuestion))
    	return false ;
    if (bExisteMaxQuestion && (iValue < iMaxValueQuestion))
    	return false ;
  }

  return true ;
}

bool  getTimeAnswer(const BB1BB& bb, Transfert& reponse, string sQuestion, std::string& sSearchParams, NVLdVTemps* pTpsValue)
{
	if (NULL == pTpsValue)
  	return false ;

  bool bAnswer = false ;

  sQuestion = getRegularPath(sQuestion, cheminSeparationMARK, intranodeSeparationMARK) ;

  BB1Object* repo = reponse.getObject(sQuestion, sSearchParams) ;
	if (NULL == repo)
  	return false ;

  std::string att = "node" ;

  NSPatPathoArray *pAnswer = getPatPatho2(repo, att) ;
  if ((NULL == pAnswer) || pAnswer->empty())
  	return false ;

  PatPathoIter iter = pAnswer->begin() ;

  int iLigneBase = (*iter)->getLigne() ;

  string sUnite  = "" ;
  string sFormat = "" ;
  string sValeur = "" ;
  string sTemp   = "" ;

  while ((pAnswer->end() != iter) && ((*iter)->getLigne() == iLigneBase))
  {
    string sLexique = (*iter)->getLexique() ;
		if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
      sTemp = (*iter)->getLexique() ;
      bb.pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
      sValeur = (*iter)->getComplement() ;
      sTemp = (*iter)->getUnit() ;
      bb.pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
    }

    iter++ ;
  }

  if (sUnite == "2DA01")
  {
    pTpsValue->initFromDate(sValeur) ;
    bAnswer = true ;
  }
  else if (sUnite == "2DA02")
  {
    pTpsValue->initFromDateHeure(sValeur) ;
    bAnswer = true ;
  }

	return bAnswer ;
}

bool
answer(const BB1BB& bb, string sQuestion, string sAnswer, NSSearchStruct *pSearchStruct)    // FIXME PATHO
{
	if (string("") == sQuestion)
		return false ;

	NSPatPathoArray *pAnswer = NULL ;

  sAnswer = getRegularPath(sAnswer, cheminSeparationMARK, intranodeSeparationMARK) ;

	if (string("") != sAnswer)
	{
		size_t      iPos        = sAnswer.find(string(1, intranodeSeparationMARK)) ;
		std::string sLexique    = "" ;
		std::string sComplement = "" ;
		std::string sUnit       = "" ;
		sLexique                = sAnswer ;

    //
    // model : 2CM001.£N0;03.$70 or 6CIS21.$B52
    //
		while (string::npos != iPos)
		{
			std::string  sLexiqueTemp = string(sAnswer, 0, iPos) ;
			sAnswer = string(sAnswer, iPos + 1, strlen(sAnswer.c_str()) - (iPos + 1)) ;

    	if ((('£' == sLexiqueTemp[0]) || ('6' == sLexiqueTemp[0])) && ('$' == sAnswer[0]))
			{
				sLexique    = sLexiqueTemp ;
				sComplement = string(sAnswer, 1, strlen(sAnswer.c_str()) - 1) ;
			}
      else if ('2' == sLexiqueTemp[0])
      	sUnit       = sLexiqueTemp ;

    	iPos = sAnswer.find(string(1, intranodeSeparationMARK)) ;
		}

		pAnswer = new NSPatPathoArray(bb.pContexte->getSuperviseur()) ;
		Message CodeMsg ;
		CodeMsg.SetLexique(sLexique) ;
		if (sComplement != "")
			CodeMsg.SetComplement(sComplement) ;
		if (sUnit != "")
			CodeMsg.SetUnit(sUnit) ;
		pAnswer->ajoutePatho(sLexique, &CodeMsg, 0) ;
	}

  string sDate = string("") ;

  sQuestion = getRegularPath(sQuestion, cheminSeparationMARK, intranodeSeparationMARK) ;

  TypedVal answerPath(sQuestion) ;
  TypedVal searchStruct(pSearchStruct) ;
  bool bAnswerSucceeded = bb.giveAnswerToQuestion(&answerPath, pAnswer, sDate, false, (HWND) 0, &searchStruct) ;
  if (false == bAnswerSucceeded)
  	delete pAnswer ;

  return bAnswerSucceeded ;

/*
  BB1BBInterface* temp = bb.pContexte->getSuperviseur()->getBBinterface();
  // Je recherche la question
  TypedVal* questionVal = new TypedVal(sQuestion);
  //BB1Object* reponse =  bb->find(questionVal,"Question");
  // Je recherche la reponse
  BB1Object* reponse = bb.find(*questionVal, "Answer") ;
  delete(questionVal);

  if  (NULL != reponse) // The question exists
  {
    AttValPair node("node", pAnswer) ;
    AttValPair noanswer("answer", string("")) ;
    //reponse->Modify(Collect(&node), NULL, Collect(&noanswer));
  }
  else     // The question doesn't exist ans I ask it
  {
    BB1Class    *answer  = bb.ClassNamed("Answer") ;
    BB1KB       *informationKB = bb.KBNamed("InformationKB") ;
    std::string id = " DPIO Colon created " + bb.getNBObject() ;

    AttValPair      node ("node",    pAnswer) ;
    AttValPair      label ("label",  string("answer from DPIO KS @ " + sQuestion)) ;
    BB1AppInst  *answ = answer->MakeInstance(id,AttValPair("explication", sQuestion),*informationKB, Collect(&label, &node),NULL, true) ;
   	// création de la réponse
  }

	return true ;
*/
}

bool  answerfalse(const BB1BB& bb, string sQuestion, string sAnswer, NSSearchStruct *pSearchStruct)
{
/*
  BB1Class* answer = bb.ClassNamed("Answer") ;
  string sLexique = sQuestion ;
  bb.pContexte->getDico()->donneCodeSens(&sQuestion, &sLexique) ;

  string sPath = sAnswer + string("/") + string(sLexique) ;
  BB1KB* informationKB = bb.KBNamed("InformationKB") ;

  std::string id = " Toolbox" ;
  char* nb = new char[10] ;
  itoa(bb.getNBObject(), nb, 10) ;
  id.append(nb) ;
  delete[] nb ;

  AttValPair node ("node",  new NSPatPathoArray(bb.pContexte)) ;
  AttValPair label("label", string("answer from DPIO KS")) ;
  BB1AppInst* answ = answer->MakeInstance(id,AttValPair("explication", sPath),*informationKB, Collect(&label, &node),NULL, true) ;
*/

	NSPatPathoArray *pAnswer = new NSPatPathoArray(bb.pContexte->getSuperviseur()) ;

  string sDate = string("") ;

	TypedVal answerPath(sQuestion) ;
  TypedVal searchStruct(pSearchStruct) ;
  bool bAnswerSucceeded = bb.giveAnswerToQuestion(&answerPath, pAnswer, sDate, false, (HWND) 0, &searchStruct) ;
  if (false == bAnswerSucceeded)
  	delete pAnswer ;

	return bAnswerSucceeded ;
}

bool answertrue(const BB1BB& bb, string sAnswer, string sRisque, NSSearchStruct *pSearchStruct)
{
	if ((string("") == sAnswer) || (strlen(sAnswer.c_str()) < BASE_SENS_LEN))
		return false ;

  string sPath = string(sAnswer, 0, BASE_SENS_LEN) ;
  if (string("") != sRisque)
  	sPath = sRisque + string(1, cheminSeparationMARK) + sPath ;

  std::string sTraceLog = string("DPIOAnswerTrue pour ") + sAnswer + string(" - risque ") + sRisque ;
 	bb.pContexte->getSuperviseur()->trace(&sTraceLog, 1) ;

 	// -------------------------------------------------------------------------
 	// création de la patpatho
 	// -------------------------------------------------------------------------
 	NSPatPathoArray *pptAnswer = new NSPatPathoArray(bb.pContexte->getSuperviseur()) ;
 	Message CodeMsg ;
 	CodeMsg.SetLexique(sAnswer) ;
 	pptAnswer->ajoutePatho(sAnswer, &CodeMsg, 0) ;

  string sDate = string("") ;

  TypedVal answerPath(sPath) ;
  TypedVal searchStruct(pSearchStruct) ;
  bool bAnswerSucceeded = bb.giveAnswerToQuestion(&answerPath, pptAnswer, sDate, false, (HWND) 0, &searchStruct) ;
  if (false == bAnswerSucceeded)
  	delete pptAnswer ;

  return bAnswerSucceeded ;

/*
 	// -------------------------------------------------------------------------
 	// recherche de la question correspondante
 	// -------------------------------------------------------------------------
 	BB1BBInterface* temp = bb.pContexte->getSuperviseur()->getBBinterface() ;
 	// Je recherche la question
 	TypedVal questionVal(sPath) ;
 	// Je recherche la reponse
	BB1Object* reponse = bb.find(questionVal,std::string("Answer")) ;

	if  (NULL != reponse) // The question already exists
 	{
 		AttValPair node("node",   pAnswer) ;
    reponse->Modify(Collect(&node), NULL, NULL) ;
    return true;
 	}
 	else     // The question doesn't exist and i ask it
 	{
    BB1Class    *answer  = bb.ClassNamed("Answer") ;
    BB1KB       *informationKB = bb.KBNamed("InformationKB");

    AttValPair      node   ("node",  pAnswer) ;
    AttValPair      label  ("label",  string("answer from DPIO KS" + sRisque)) ;

    std::string id = " Toolbox" ;
    char* nb = new char[10] ;
    itoa(bb.getNBObject(), nb, 10) ;
    id.append(nb) ;
    delete[] nb ;

   	BB1AppInst *answ = answer->MakeInstance(id,AttValPair("explication", sPath),*informationKB, Collect(&label, &node),NULL,true) ;
   	return true ;
 	}
*/
}

bool getFullSortedHistory(const BB1BB& bb, NSHistoryValManagementArray* pValArray, string sValue)
{
	if ((NULL == pValArray) || (string("") == sValue))
		return false ;

	string sValueSens = string("") ;
	bb.pContexte->getDico()->donneCodeSens(&sValue, &sValueSens) ;

  // 1. On récupère l'ensemble des valeurs du dossier patient
  // 1. Getting information from patient's record
	pValArray->ChargeHistoryValueItem(sValue) ;

  // 2. on va chercher les nouvelles données sur le blackboard
  // 2. Getting new information on blackboard
  NSPatPathoArray *pAnswer = (NSPatPathoArray*) 0 ;
  string sAnswerDate ;

  // if ((bb.pContexte->getSuperviseur()->getBBinterface()->getAnswer2Question(sValue, "", &pAnswer, sAnswerDate, false)) && pAnswer && (!(pAnswer->empty())))
  if ((bb.pContexte->getBBinterface()->getAnswer2Question(sValueSens, "", &pAnswer, sAnswerDate, false)) && pAnswer && (!(pAnswer->empty())))
  {
    string sElemLex, sSens;
    PatPathoIter iter = pAnswer->begin() ;
    while ((NULL != iter) && (pAnswer->end() != iter))
    {
    	sSens = (*iter)->getLexiqueSens() ;
      if (sSens == sValueSens)
        pValArray->ChargeValueFromPatPatho(pAnswer, iter) ;
      iter = pAnswer->ChercherFrereSuivant(iter) ;
    }
  }
  if (pAnswer)
  	delete pAnswer ;

  // 3. on trie l'array trouvé par ordre anti-chronologique
  // 3. sorting, most recent information on begin()
  if (false == pValArray->empty())
    sort(pValArray->begin(), pValArray->end(), sortByDateSup) ;

	return true ;
}

