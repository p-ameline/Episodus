/*
** Copyright Nautilus, (10/9/2004)
** fabrice.le-perru@nautilus-info.com
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
#include "nsbb\nspatpat.h"
#include "ns_ob1\BB1BB.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
  #include "enterpriseLus\personLus.h"
  #include "ob1Lus\InterfaceLus.h"
#else
  #include "nautilus\nssuper.h"
  #include "nautilus\nautilus.h"
  #include "ns_ob1\Interface.h"
  #include "ns_ob1\ns_bbk.h"
#endif

#include "nautilus\nsepicap.h"
#include "nautilus\nscqdoc.h"

#include "ns_ob1\nsbbk.h"
#include "ns_ob1\nautilus-bbk.h"
#include "ns_ob1\Transfer.h"
#include "ns_ob1\BB1Task.h"
#include "ns_ob1\AgentManager.h"
#include "ns_ob1\InterfaceForKs.h"

#include "ns_ob1\OB1Controler.h"
#include "ns_ob1\OB1Token.h"

#include "ns_ob1\NSKS.h"

// #include <winuser.h>

// const int ID_BBKList = 0x442;
/*
** Crée une interface et la relie au BB
*/
int CreateInterface(NSContexte *pContexte)
{
try
{
	pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps,"OB1 Interface Creation") ;
  pContexte->getSuperviseur()->DebugNewLine() ;
	// BB1BBInterface* Interfax  = new BB1BBInterface(*(pContexte->getSuperviseur()->pNSApplication->prendClient()), pContexte);
  //Interfax->Create();
	// if (pContexte->getSuperviseur()->blackboardInterface == NULL)
  //{

#ifndef _ENTERPRISE_DLL
	BB1BBInterface* interfax = new BB1BBInterface(0, pContexte, pNSResModule) ;
	pContexte->setBBinterface(interfax) ;
	interfax->putBB(pContexte->getBlackboard()) ;
	OB1InterfaceContainer* interfaceMDI = new OB1InterfaceContainer(*(pContexte->getSuperviseur()->getApplication()->prendClient()), pContexte, interfax) ;
	interfaceMDI->Create() ;
	interfax->SetParent(interfaceMDI) ;
  interfax->SetParentContainer(interfaceMDI) ;

  // Minimize
  // interfaceMDI->Show(SW_MINIMIZE) ;

  interfaceMDI->SetWindowPos(0, 0, 0, 0, 0, SWP_NOZORDER) ;
  // Doing this bugs Windows list (à la MS KB article Q125435)
  // interfaceMDI->Show(SW_HIDE) ;
#else
  BB1BBInterface* interfax = new BB1BBInterface(pContexte) ;
	pContexte->setBBinterface(interfax) ;
	interfax->putBB(pContexte->getBlackboard()) ;
#endif

  pContexte->getBlackboard()->Controler()->_DefaultFunc = LeafDefaultComputation ;
  return (0) ;
}
catch (...)
{
	erreur("Exception CreateInterface", standardError) ;
  return 1 ;
}
}

/*
** Crée une interface et bb et les relie entre eux
*/
int
runBlackboard(NSContexte *pContexte)
{
  if (NULL == pContexte)
    return 0 ;

  BB1Class::initNbInstance() ;
  BB1Object::initNbInstance() ;
  BB1Link::initNbInstance() ;

  NautilusEvent::initNbInstance() ;

  ObjectList::initNbInstance() ;
  StringList::initNbInstance() ;
  PathsList::initNbInstance() ;
  AVPSet::initNbInstance() ;
  ContextSet::initNbInstance() ;
  KBSet::initNbInstance() ;
  KSSet::initNbInstance() ;
  AKSSet::initNbInstance() ;
  LinkSet::initNbInstance() ;
  LinkPairSet::initNbInstance() ;
  ObjectSet::initNbInstance() ;

  TypedVal::initNbInstance() ;
  AttValPair::initNbInstance() ;
  MarkerString::initNbInstance() ;
  MarkerInteger::initNbInstance() ;
  Valeur::initNbInstance() ;
  MarkSet::initNbInstance() ;

  OB1Node::initNbInstance() ;
  OB1NLevel::initNbInstance() ;
  OB1NResult::initNbInstance() ;
  OB1NTrigger::initNbInstance() ;
  OB1NAnd::initNbInstance() ;
  OB1NOther::initNbInstance() ;
  OB1NKS::initNbInstance() ;
  OB1NValidity::initNbInstance() ;

  OB1NodeLink::initNbInstance() ;
  OB1Edge::initNbInstance() ;
  Valeur::initNbInstance() ;
  MarkerInteger::initNbInstance() ;
  MarkerString::initNbInstance() ;

  OB1Token::initNbInstance() ;

  BB1BBInterfaceForKs::initNbInstance() ;

#ifndef _ENTERPRISE_DLL
	pContexte->getSuperviseur()->getApplication()->sendMessageToSplash("Start blackboard") ;
  pContexte->getSuperviseur()->getApplication()->setSplashPercentDone(30) ;
#endif

  pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps,"Welcome In OB1. LEt the power be with you...") ;
  pContexte->getSuperviseur()->DebugNewLine() ;

#ifdef __DEBUG__
  // on fait des traces en mode DEBUG
  string	sTrace("run Blackboard first initialization") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1) ;
#endif

  // int temp = 1 ;

	// Blackboard initialisation
	BB1BB *bb = new BB1BB(pContexte, nautilusBlackboardInit, nautilusBlackboardTerminateP) ;
  pContexte->setBlackboard(bb) ;


	// on définit les classes, les KBs et les KSs de bases
	// on définit ensuite les classes, les KBs et les KSs propres au raisonnement

  /*
  ** KB servant a déposer des information sur le bb
  ** En general sert pour les reponse aux question
  */
  /* BB1KB *Information = */     bb->DefineKB("InformationKB") ;   // toutes les infor
  /* BB1KB *NautilusEventKB = */ bb->DefineKB("NautilusEventKB") ; // Evenement utilisateur
  /* BB1KB *QuestionKB = */      bb->DefineKB("QuestionKB") ;      // Toutes les questions
  /* BB1KB *DPIOKB = */          bb->DefineKB("KBKS") ;            // Info DPIO

	AttValPair labelAVP				  ("label",					NULLSTRING) ;
	AttValPair valueIntAVP			("value",					NULLLONG) ;
	AttValPair valueStringAVP   ("value",					NULLSTRING) ;
	AttValPair answerAVP				("answer",				NULLSTRING) ;
	AttValPair askAVP					  ("askuser",				bool(false)) ;
	AttValPair questionAVP			("question",			NULLSTRING) ;
	AttValPair dateMinAVP			  ("DateMin",				NULLSTRING) ;
	AttValPair dateMaxAVP			  ("DateMax",				NULLSTRING) ;
	AttValPair unitAVP					("unitValue",			NULLSTRING) ;
  AttValPair questionwith_arg ("question",      NULLLNAUTQUEST) ;
  AttValPair interfaceHandles ("interfaces",    NULLHANDLES) ;
  AttValPair tokenHandles     ("tokens",        NULLHANDLES) ;
  AttValPair persistenceHdles ("persistence",   NULLHANDLES) ;

	AttValPair formulaireAVP		("formulaire",		NULLSTRING) ;
	AttValPair archetypeAVP		  ("archetype",			NULLSTRING) ;

 //	AttValPair	fromAVP					("from",					NULLPTR) ;
	AttValPair sfromAVP				  ("sfrom",					NULLSTRING) ;

	AttValPair searchDepthAVP	  ("searchDepth",		NULLSTRING) ;
	AttValPair searchStateAVP	  ("searchState",		NULLSTRING) ;

	AttValPair nodeAVP					("node",					NULLPATHO) ;
  AttValPair whereIChangeAVP	("change",				NULLPATPATHOITER) ;
	AttValPair certAVP					("certitude",			NULLLONG) ;
	AttValPair needDatasAVP		  ("answers",				NULLOBJECT) ;
	AttValPair nbAnswersAVP		  ("nbAnswers",			NULLLONG) ;
  AttValPair userEventTypeAVP ("userEventType", NULLLONG) ;
  AttValPair userEventAVP     ("event",         NULLNAUTILUSEVENT) ;
	AttValPair listofAnswersAVP ("listofanswers",	NULLOBJECT) ;
	AttValPair dateAVP					("Date",					NULLLONG);

	AttValPair pathsAVP				  ("paths",					NULLPATHS) ;
	AttValPair path 						("question",      NULLSTRING) ;
  AttValPair searchParamsAVP 	("searchParams",  NULLSEARCHSTRUCT) ;

  AttValPair QuestionExplic   ("explication", std::string("QuestionClass")) ;
  AttValPair AnswerExplic     ("explication", std::string("QuestionClass")) ;
  AttValPair NautEventExplic  ("explication", std::string("EventClass")) ;


  // Class question permet de poser des question aux niveaux du blackboard
  //
  AVPSet* pAttSpec = Collect(&labelAVP,  // Explication
                             &askAVP,
                             &questionAVP,
                             &dateMinAVP,
                             &dateMaxAVP,
                             &unitAVP,
                             &formulaireAVP,
                             &archetypeAVP,
                             &questionwith_arg,
                             &sfromAVP,
                             &searchDepthAVP,
                             &searchStateAVP,
                             &listofAnswersAVP,
                             &path,
                             &interfaceHandles,
                             &tokenHandles,
                             &persistenceHdles,
                             &searchParamsAVP) ;

	/* BB1Class *QuestionClass = */
  bb->DefineClass("Question", QuestionExplic, (StringList*) 0, pAttSpec) ;
  delete pAttSpec ;

	// class DatasFromNautilus

  AVPSet* pAttSpecAnswer = Collect(&labelAVP,
                                   &valueIntAVP,
                                   &valueStringAVP,
                                   &answerAVP,
                                   &unitAVP,
                                   &nodeAVP,
                                   &certAVP,
                                   &needDatasAVP,
                                   &nbAnswersAVP,
                                   &questionwith_arg,
                                   &sfromAVP,
                                   &dateMinAVP,
                                   &dateMaxAVP,
                                   &searchParamsAVP) ;
	/* BB1Class *answerClass = */
  bb->DefineClass("Answer", AnswerExplic, (StringList*) 0, pAttSpecAnswer) ;
  delete pAttSpecAnswer ;

  AVPSet* pAttSpecUserEvent = Collect(&labelAVP,
                                      &nodeAVP,
                                      &whereIChangeAVP,
                                      &userEventTypeAVP,
                                      &userEventAVP) ;
  /* BB1Class *UserEventClass = */
  bb->DefineClass("UserEvent", NautEventExplic, (StringList*) 0, pAttSpecUserEvent) ;
  delete pAttSpecUserEvent ;

	/* BB1Class *NoneClass = */ bb->DefineClass("None", labelAVP) ;


	//BB1Class	*ComputeClass						= bb->DefineClass("Compute", NULL, Collect(&labelAVP, &archetypeAVP, &path)) ;

	//BB1Class *DPIOClass								= bb->DefineClass("DPIO", NULL, Collect(&labelAVP, &valueStringAVP)) ;

	// on instancie les KSs dispos - pour l'instant :
	//       SearchNSKS (KS faisant les recherches dans Nautilus)
	//      RiskFactorKS (prototype d'un KS faisant du calcul de risque)

  ifstream    inFile1, inFile2 ;
  string      sLine ;
  string      sData = "" ;
  string      sNomAttrib ;
  string      sValAttrib ;
  size_t      i = 0 ;  char        szFichier[255] ;
  int         etatEnCours = 0 ;

  VectString  aPairLabel ;

  string sUserIndice = "" ;
  if (pContexte->getUtilisateur())
#ifndef _ENTERPRISE_DLL
		sUserIndice = pContexte->getUtilisateur()->getNss() ;
#else
		sUserIndice = pContexte->getUtilisateur()->getId() ;
#endif

  if (sUserIndice != "")
  {
    sprintf(szFichier, "KSMUE_%3s.dat", sUserIndice.c_str()) ;
    string  sPath = "" ;
//    sPath = pContexte->PathName("FGLO") ;
    string sFichier = sPath +  string(szFichier) ;
    inFile1.open(sFichier.c_str()) ;
  }
  if ((!inFile1) || (sUserIndice == ""))
  {
    string  sPath = "" ;
//    sPath = pContexte->PathName("FGLO") ;
    string sFichier = sPath + string("KSMUE.dat") ;
    inFile2.open(sFichier.c_str()) ;
    if (!inFile2)
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
      sErrorText += string(" ") + sFichier ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError) ;
      return false ;
    }

    while (!inFile2.eof())
    {
      getline(inFile2, sLine) ;
      if (sLine != "")
        sData += sLine + "\n" ;
    }

    inFile2.close() ;
  }
  else
  {
    while (!inFile1.eof())
    {
      getline(inFile1, sLine) ;
      if (sLine != "")
        sData += sLine + "\n" ;
    }

    inFile1.close() ;
  }

  string  sDLLName      = "" ;
  string  sKBName       = "" ;
  string  sLabelDialog  = "" ;
  string  sLabelDiaLang = "" ;
	string  sArchetype    = "" ;
	string  sArcheForce   = "" ;
	string  sArcheRefus   = "" ;
	string  sFormulaire   = "" ;
  string  sScriptName   = "" ;

  //BB1KB *DPIOKB = bb->DefineKB("DPIOKS") ;

  string sLang = "" ;
	if (pContexte->getUtilisateur())
		sLang = pContexte->getUtilisateur()->donneLang() ;

  if (sLang != "")
  	pseumaj(&sLang) ;

  // boucle de chargement des attributs
  i = 0;
  while (i < strlen(sData.c_str()))
  {
    sNomAttrib = "" ;
    sValAttrib = "" ;

    size_t  iPos = sData.find("\n", i) ;
    if (iPos != string::npos)
    {
      sNomAttrib = string(sData, i, iPos - i) ;
      i = iPos ;
    }

    if (i < strlen(sData.c_str()))
      i++ ;

    strip(sNomAttrib, stripBoth) ;

		// Commentaire ou ligne vierge : on ne fait rien
		if ((strlen(sNomAttrib.c_str()) == 0) || ((sNomAttrib[0] == '/') && (sNomAttrib[1] == '/')))
		{
		}

		// Si on rencontre un délimiteur de Section
		else if ((sNomAttrib[0] == '[') && (sNomAttrib[strlen(sNomAttrib.c_str())-1] == ']'))
		{
      if (sLabelDiaLang != "")
      	sLabelDialog = sLabelDiaLang ;

			// On en termine avec l'élément précédent
      if ((sKBName != "") && (sDLLName != ""))
        loadKS(pContexte, bb, sDLLName, sKBName, sLabelDialog, sArchetype, sArcheForce, sArcheRefus, sFormulaire, true) ;
      else
      {
      	if ((sKBName != "") && (sScriptName != ""))
        	loadScriptKS(pContexte, bb, sScriptName, sScriptName, sLabelDialog, sArchetype, sArcheForce, sArcheRefus, sFormulaire, true) ;
      }

      sDLLName      = "" ;
      sKBName       = "" ;
      sLabelDialog  = "" ;
      sLabelDiaLang = "" ;
      sArchetype    = "" ;
      sArcheForce   = "" ;
      sArcheRefus   = "" ;
      sFormulaire   = "" ;
      sScriptName   = "" ;

      etatEnCours   = 1 ;
    }

		// Sinon traitement de la ligne
		else if (etatEnCours > 0)
		{
      string sChaineNom       = "" ;
      string sChaineContenu   = "" ;

			size_t recherche = sNomAttrib.find('=') ;
			if ((recherche != string::npos) && (recherche < strlen(sNomAttrib.c_str()) - 1))
			{
				// On sépare la chaine en Nom et Contenu
				//	Ex : "Font1=Times New Roman,120"
				// -> Nom : "Font1" et Contenu = "Times New Roman,120"
				sChaineNom 	    = string(sNomAttrib, 0, recherche) ;
				sChaineContenu  = string(sNomAttrib, recherche + 1, strlen(sNomAttrib.c_str()) - (recherche + 1)) ;

        strip(sChaineNom, stripBoth) ;
        strip(sChaineContenu, stripBoth) ;

        pseumaj(&sChaineNom) ;

        if      (sChaineNom == "DLL")
          sDLLName      = sChaineContenu ;
        else if  (sChaineNom == "SCRIPT")
        {
        	sScriptName = sChaineContenu ;

#ifndef _ENTERPRISE_DLL
          pContexte->getSuperviseur()->getApplication()->sendMessageToSplash(string("Loading script ") + sScriptName) ;
#endif

          pContexte->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps,"Chargement d'un script : %s", sScriptName.c_str()) ;
  				pContexte->getSuperviseur()->DebugNewLine() ;
        }
        else if (sChaineNom == "KB")
          sKBName       = sChaineContenu ;
        else if (sChaineNom == "LABEL-DIALOG")
          sLabelDialog  = sChaineContenu ;
        else if (sChaineNom == "ARCHETYPE")
          sArchetype    = sChaineContenu ;
        else if (sChaineNom == "ARCHETYPE-OUIFORCE")
          sArcheForce   = sChaineContenu ;
        else if (sChaineNom == "ARCHETYPE-REFUS")
          sArcheRefus   = sChaineContenu ;
        else if (sChaineNom == "FORMULAIRE")
          sFormulaire   = sChaineContenu ;
        else if ((strlen(sChaineNom.c_str()) >= 12) &&
                 (string(sChaineNom, 0, 12) == "LABEL-DIALOG"))
        {
        	sChaineNom = string(sChaineNom, 12, strlen(sChaineNom.c_str())-12) ;
          strip(sChaineNom, stripBoth) ;
          if (sChaineNom == sLang)
          	sLabelDiaLang = sChaineContenu ;
        }
      }
    }
  }

  if (string("") != sLabelDiaLang)
  	sLabelDialog = sLabelDiaLang ;

  // On en termine avec l'élément précédent
  if ((string("") != sKBName) && (string("") != sDLLName))
    loadKS(pContexte, bb, sDLLName, sKBName, sLabelDialog, sArchetype, sArcheForce, sArcheRefus, sFormulaire, true) ;
  if ((string("") != sKBName) && (string("") != sScriptName))
   	loadScriptKS(pContexte, bb, sScriptName, sKBName, sLabelDialog, sArchetype, sArcheForce, sArcheRefus, sFormulaire, true) ;

#ifndef _ENTERPRISE_DLL
	pContexte->getSuperviseur()->getApplication()->sendMessageToSplash("Blackboard started") ;
  pContexte->getSuperviseur()->getApplication()->setSplashPercentDone(70) ;
#endif

	return true ;
}

bool loadScriptKS(NSContexte* pCtx, BB1BB* bb, string ScriptFile, string sKB, string sLabel, string sArc, string sArcForce, string sArcRefus, string sFormulaire, bool /* dpioKS */)
{
  if (NULL == bb)
    return false ;

  BB1KB *kb = bb->KBNamed(sKB) ;
  if (NULL == kb)
  {
  	string sErrorText = pCtx->getSuperviseur()->getText("bbkKsManagement", "unknownKB") ;
    sErrorText += string(" ") + sKB ;
    pCtx->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  	erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  AgentManager loader ;
  AgentScript* agent_to_load = loader.create_agent_from_script(ScriptFile + ".xml") ;
  if (NULL == agent_to_load)
    return false ;

	AttValPair	avpLabel	        ("label-dialog",	      sLabel) ;
	AttValPair	avpArchetype      ("archetype",		        sArc)   ;
	AttValPair	avpFormulaire     ("formulaire",		      sFormulaire)  ;

	AVPSet* pAttr = Collect(&avpLabel, &avpArchetype, &avpFormulaire) ;

  /* BB1KS *bbKS = */ bb->DefineGeneralizedKS(ScriptFile,
  																		*kb,
                                      new MemFunctorPublication<AgentScript>(agent_to_load,&AgentScript::Publication),
                                      new MemFunctorValidity<AgentScript>(agent_to_load,&AgentScript::ContextValidity),
                                      new MemFunctorPrecondition<AgentScript>(agent_to_load,&AgentScript::Precondition),
                                      new MemFunctorAction<AgentScript>(agent_to_load,&AgentScript::Action),
                                      NULL,
                                      pAttr) ;
  delete pAttr ;
  delete agent_to_load ;

	return true ;
}

bool
loadKS(NSContexte *pCtx, BB1BB *bb, string sKS, string sKB, string sLabel, string sArc, string sArcForce, string sArcRefus, string sFormulaire, bool /* dpioKS */)
{
#ifndef _ENTERPRISE_DLL
	pCtx->getSuperviseur()->getApplication()->sendMessageToSplash("Loading KS " + sKS) ;
#endif

	pCtx->getSuperviseur()->voidDebugPrintf(NSSuper::trSteps, "Loading KS : %s ...", sKS.c_str());
  pCtx->getSuperviseur()->DebugNewLine();

  BB1KB* kb = bb->KBNamed(sKB) ;
  if (NULL == kb)
	{
  	string sError = pCtx->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(" ") + sKS + string(". ") ;
  	sError += pCtx->getSuperviseur()->getText("bbkKsManagement", "unknownKB") ;
    sError += string(" (") + sKB + string(")") ;
  	erreur(sError.c_str(), standardError) ;
    return false ;
	}

  if ((string("") != sLabel) && (string("") != sArc) && (string("") != sFormulaire))
  {
    AttValPair	avpLabel	   ("label-dialog",	sLabel) ;
    AttValPair	avpArchetype ("archetype",		sArc) ;
    AttValPair	avpFormulaire("formulaire",	  sFormulaire) ;

    if      ((string("") != sArcForce) && (string("") != sArcRefus))
    {
      AttValPair	avpArcOuiForce  ("archetype-ouiforce",  sArcForce)  ;
      AttValPair	avpArcRefus	    ("archetype-refus",	    sArcRefus)  ;
      return (loadKS(pCtx, bb, sKS, kb, Collect(&avpLabel, &avpArchetype, &avpArcOuiForce, &avpArcRefus, &avpFormulaire), true)) ;
    }
    else if (string("") != sArcForce)
    {
      AttValPair  avpArcOuiForce  ("archetype-ouiforce",  sArcForce) ;
      return (loadKS(pCtx,bb,sKS, kb, Collect(&avpLabel, &avpArchetype, &avpArcOuiForce, &avpFormulaire), true)) ;
    }
    else if (string("") != sArcRefus)
    {
      AttValPair  avpArcRefus     ("archetype-refus",     sArcRefus) ;
      return (loadKS(pCtx, bb, sKS, kb, Collect(&avpLabel, &avpArchetype, &avpArcRefus, &avpFormulaire), true)) ;
    }
    else
      return (loadKS(pCtx, bb, sKS, kb, Collect(&avpLabel, &avpArchetype, &avpFormulaire), true)) ;
  }
  else
    return (loadKS(pCtx, bb, sKS, kb)) ;
}

bool
loadKS(NSContexte *pContexte, BB1BB* bb, string sKS, BB1KB *kb, AVPSet *attributes, bool dpioKS)
{
#ifdef __DEBUG__
{
	string	sTrace = string("loadKS <") + sKS + string(">") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
}
#endif

  // FIXME
 	//	BB1BB *bb = pContexte->getSuperviseur()->blackboardInterface->getBB();

	if (initKS(pContexte, bb, sKS, dpioKS))
	{
  	NSKS *theKS = (NSKS *) 0 ;
    if (false == bb->getNsKss()->empty())
		  for (vector<NSKS *>::iterator iter = bb->getNsKss()->begin() ; bb->getNsKss()->end() != iter ; iter++)
			  if ((*iter)->sLabel == sKS)
			  {
				  theKS = (*iter) ;
				  break ;
			  }

		/* BB1KS *bbKS = */ bb->DefineGeneralizedKS(sKS,
  																		*kb,
                                      new FuncFunctorPublication(theKS->tpublication),
                                      new FuncFunctorValidity(theKS->tvalidity),
                                      new FuncFunctorPrecondition(theKS->pCondition),
                                      new FuncFunctorAction(theKS->action),
                                      NULL,
                                      attributes) ;
    delete attributes ;

   // 	BB1KS *bbKS = bb->DefineKS(sKS,*kb, theKS->tpublication,theKS->tvalidity, theKS->pCondition, theKS->action, attributes) ;

	  //bb->Akss->push_back(new AKS(bbKS, theKS->apCondition)) ;
    return true ;
	}

  delete attributes ;

#ifdef __DEBUG__
  {
	string	sTrace = string("KS <") + sKS + string("> loaded") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
  }
#endif

  return false ;
}

bool
initKS(NSContexte *pContexte, BB1BB *bb, string sKS, bool /* dpioKS */)
{
#ifdef __DEBUG__
{
	string	sTrace = string("initKS <") + sKS + string(">") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
}
#endif

	// Initialisation du module de la dll du KS
	NSKS *theKS = new NSKS() ;
	theKS->sLabel = sKS ;
	string sKSdll = sKS + ".dll" ;

try
{
#ifndef _ENTERPRISE_DLL

	theKS->pKSModule = new TModule(sKSdll.c_str(), TRUE) ;

  if ((TModule *) NULL == theKS->pKSModule)  {  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;		sError += string(". ") ;
		sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotLoadTheDll") ;
  	sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
		erreur(sError.c_str(), standardError) ;		// string sErreur = "Impossible d'ouvrir la DLL " + sKS + ".dll ." ;		// erreur(sErreur.c_str(), 0, -1, pContexte->GetMainWindow()->GetHandle());		// pContexte->getSuperviseur()->afficheStatusMessage("");    delete theKS ;    return false ;  }

  // Initialisation de la méthode de publication
  KsInitStructure* (FAR *pTPub) () ;

  (FARPROC) pTPub = theKS->pKSModule->GetProcAddress(MAKEINTRESOURCE(6)) ;
  if (pTPub == NULL)
  {
  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(". ") ;
  	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotConnectThePublicationFunction") ;
    sError += string(". ") ;
    sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
    sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
  	erreur(sError.c_str(), standardError) ;
    delete theKS ;
    return false ;
  }
  else
  	theKS->tpublication = pTPub ;

	bool (FAR *pPC) (const BB1BB&, Transfert* ) ;

  (FARPROC) pPC = theKS->pKSModule->GetProcAddress(MAKEINTRESOURCE(4)) ;
  if (pPC == NULL)
  {
  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(". ") ;
  	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotConnectThePreConditionFunction") ;
    sError += string(". ") ;
    sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
    sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
  	erreur(sError.c_str(), standardError) ;
    delete theKS ;
    return false ;
  }
  else
  	theKS->pCondition = pPC ;

  Errcode	(FAR *pAC) (BB1BB&, Transfert*, bool ) ;


  (FARPROC) pAC = theKS->pKSModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  if (pAC == NULL)
  {
  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(". ") ;
  	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotConnectTheActionFunction") ;
    sError += string(". ") ;
    sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
    sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
  	erreur(sError.c_str(), standardError) ;
    delete theKS ;
    return false ;
  }
  else
  	theKS->action = pAC ;

  /* Initialisation de la méthode de validation de contexte */
  ValidityContextType (FAR *pAV) (const BB1BB&, Transfert* ) ;

  (FARPROC) pAV = theKS->pKSModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
  if (pAV == NULL)
  {
  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(". ") ;
  	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotConnectTheValidationFunction") ;
    sError += string(". ") ;
    sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
    sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
  	erreur(sError.c_str(), standardError) ;
    delete theKS ;
    return false ;
  }
  else
  	theKS->tvalidity = pAV ;

#else

	theKS->pKSModule = PR_LoadLibrary(sKSdll.c_str()) ;

  if ((PRLibrary *) NULL == theKS->pKSModule)  {  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;		sError += string(". ") ;
		sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotLoadTheDll") ;
  	sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
		erreur(sError.c_str(), standardError) ;		// string sErreur = "Impossible d'ouvrir la DLL " + sKS + ".dll ." ;		// erreur(sErreur.c_str(), 0, -1, pContexte->GetMainWindow()->GetHandle());		// pContexte->getSuperviseur()->afficheStatusMessage("");    delete theKS ;    return false ;  }

  // Initialisation de la méthode de publication
  KsInitStructure* (FAR *pTPub) () ;

  pTPub = (KsInitStructure* (FAR *)()) PR_FindSymbol(theKS->pKSModule, "Publication$qv") ;
  if (NULL == pTPub)
  {
  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(". ") ;
  	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotConnectThePublicationFunction") ;
    sError += string(". ") ;
    sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
    sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
  	erreur(sError.c_str(), standardError) ;
    delete theKS ;
    return false ;
  }
  else
  	theKS->tpublication = pTPub ;

	bool (FAR *pPC) (const BB1BB&, Transfert* ) ;

  pPC = (bool (FAR *)(const BB1BB&, Transfert*)) PR_FindSymbol(theKS->pKSModule, "KSPrecondition$qrx5BB1BBp9Transfert") ;
  if (NULL == pPC)
  {
  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(". ") ;
  	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotConnectThePreConditionFunction") ;
    sError += string(". ") ;
    sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
    sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
  	erreur(sError.c_str(), standardError) ;
    delete theKS ;
    return false ;
  }
  else
  	theKS->pCondition = pPC ;

  Errcode	(FAR *pAC) (BB1BB&, Transfert*, bool ) ;

  pAC = (Errcode (FAR *) (BB1BB&, Transfert*, bool)) PR_FindSymbol(theKS->pKSModule, "KSAction$qr5BB1BBp9Transfert4bool") ;
  if (pAC == NULL)
  {
  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(". ") ;
  	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotConnectTheActionFunction") ;
    sError += string(". ") ;
    sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
    sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
  	erreur(sError.c_str(), standardError) ;
    delete theKS ;
    return false ;
  }
  else
  	theKS->action = pAC ;

  /* Initialisation de la méthode de validation de contexte */
  ValidityContextType (FAR *pAV) (const BB1BB&, Transfert* ) ;

  pAV = (ValidityContextType (FAR *) (const BB1BB&, Transfert*)) PR_FindSymbol(theKS->pKSModule, "ContextValidity$qrx5BB1BBp9Transfert") ;
  if (NULL == pAV)
  {
  	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
    sError += string(". ") ;
  	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "cannotConnectTheValidationFunction") ;
    sError += string(". ") ;
    sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
    sError += string(" (") + sKSdll + string(").") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
  	erreur(sError.c_str(), standardError) ;
    delete theKS ;
    return false ;
  }
  else
  	theKS->tvalidity = pAV ;

#endif

	// FIXME
  /*pContexte->getSuperviseur()->blackboardInterface->getBB()*/
  bb->getNsKss()->push_back(theKS) ;
}
#ifndef _ENTERPRISE_DLL
catch (const TXInvalidModule& e)
{
	// Trying to fire an error leads to system crash
/*
	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
	sError += string(". ") ;
	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
  sError += string(" (") + sKSdll + string(").") ;
	erreur(sError.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
*/
  return false ;
}
#endif
catch (...)
{
	string sError = pContexte->getSuperviseur()->getText("bbkKsManagement", "unableToLoadKS") ;
	sError += string(". ") ;
	sError += pContexte->getSuperviseur()->getText("bbkKsManagement", "corruptedDll") ;
  sError += string(" (") + sKSdll + string(").") ;
  pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;
	erreur(sError.c_str(), standardError) ;
  return false ;
}

#ifdef __DEBUG__
  {
	string	sTrace = string("KS <") + sKS + string("> initialized") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
  }
#endif
	return true ;
}

int
addDatasFromNautilusOnBlackboard(NSContexte* pContexte, NSPatPathoArray* /* pPatPathoArray */)
{
#ifdef __DEBUG__
{
 	string	sTrace = string("adding datas from Nautilus") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
}
#endif


#ifdef __DEBUG__
  {
	string	sTrace = string("datas from Nautilus added") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
  }
#endif
  return 1; //  FIXME
}

int
goOneCycleBlackboard(NSContexte* /* pContexte */)
{
  // FIXME
	//pContexte->getSuperviseur()->blackboard->GoNCycles(1) ;
  return 1; //FIXME
}

int
goNCyclesBlackboard(NSContexte* /* pContexte */)
{
  // FIXME
	//pContexte->getSuperviseur()->blackboard->GoNCycles() ;
  return 1; //FIXME;
}

int
closeBlackboard(NSContexte *pContexte)
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

#ifdef __DEBUG__
	string sTrace = string("closing Blackboard") ;
	pSuper->trace(&sTrace, 1, NSSuper::trSteps) ;
#endif

  BB1BB* pBB = pContexte->getBlackboard() ;
  if (pBB)
  {
    delete pBB ;
    pContexte->setBlackboard((BB1BB*) 0) ;
  }

  //	pContexte->getSuperviseur()->blackboard = NULL ;
	// cette fonction arrête le Blackboard raisonnant sur le patient.
	// avant d'arrêter le Blackboard on sauvegarde son état afin de pouvoir le
	// réinstancier à l'identique à sa prochaine ouverture.

  // Leaks detector
  //
  pSuper->CheckZeroObjectCount(string("BB1Class"),   BB1Class::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("BB1Object"),  BB1Object::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("BB1Link"),    BB1Link::getNbInstance()) ;

  pSuper->CheckZeroObjectCount(string("NautilusEvent"), NautilusEvent::getNbInstance()) ;

  pSuper->CheckZeroObjectCount(string("ObjectList"),  ObjectList::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("StringList"),  StringList::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("PathsList"),   PathsList::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("AVPSet"),      AVPSet::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("ContextSet"),  ContextSet::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("KBSet"),       KBSet::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("KSSet"),       KSSet::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("AKSSet"),      AKSSet::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("LinkSet"),     LinkSet::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("LinkPairSet"), LinkPairSet::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("ObjectSet"),   ObjectSet::getNbInstance()) ;

  pSuper->CheckZeroObjectCount(string("TypedVal"),      TypedVal::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("AttValPair"),    AttValPair::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("MarkerString"),  MarkerString::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("MarkerInteger"), MarkerInteger::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("Valeur"),        Valeur::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("MarkSet"),       MarkSet::getNbInstance()) ;

  pSuper->CheckZeroObjectCount(string("OB1Node"),      OB1Node::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("OB1NLevel"),    OB1NLevel::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("OB1NResult"),   OB1NResult::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("OB1NTrigger"),  OB1NTrigger::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("OB1NAnd"),      OB1NAnd::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("OB1NOther"),    OB1NOther::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("OB1NKS"),       OB1NKS::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("OB1NValidity"), OB1NValidity::getNbInstance()) ;

  pSuper->CheckZeroObjectCount(string("OB1NodeLink"),   OB1NodeLink::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("OB1Edge"),       OB1Edge::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("Valeur"),        Valeur::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("MarkerInteger"), MarkerInteger::getNbInstance()) ;
  pSuper->CheckZeroObjectCount(string("MarkerString"),  MarkerString::getNbInstance()) ;

  pSuper->CheckZeroObjectCount(string("OB1Token"), OB1Token::getNbInstance()) ;

  pSuper->CheckZeroObjectCount(string("BB1BBInterfaceForKs"), BB1BBInterfaceForKs::getNbInstance()) ;

#ifdef __DEBUG__
	sTrace = string("Blackboard closed - all datas have been removed") ;
	pSuper->trace(&sTrace, 1, NSSuper::trSteps) ;
#endif

  return 1 ; // FIXME
}

int
DPIO(NSContexte *pContexte)
{
#ifdef __DEBUG__
	string	sTrace = string("DPIO message which launched DPIO actions") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
#endif

#ifdef __DEBUG__
	sTrace = string("20 cycles after DPIO event have been runned succesfully") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
#endif

  return 1 ; // FIXME
}

int
RVDB(NSContexte *pContexte)
{
#ifdef __DEBUG__
	string	sTrace = string("RVDB call function") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
#endif

		//NeedDatasClass->MakeInstance(sName, *PatientDatasKB, Collect(&label, &question)) ; */

#ifdef __DEBUG__
	sTrace = string("RVDB event created - now going to run 50 cycles on the blackboard") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
#endif

#ifdef __DEBUG__
	sTrace = string("ending process after RVDB event") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1) ;
#endif

  return 1 ; //FIXME
}


Errcode
nautilusBlackboardInit(BB1BB& /* bb */)
{
//	AttValPair labelAVP("label", string("Age")) ;
//	AttValPair certAVP("certitude", (int) 54) ;
//	DatasFromNautilusClass->MakeInstance("Age", *PatientDatasKB, Collect(&labelAVP, &certAVP)) ;

	return SUCCESS ;
}


bool
nautilusBlackboardTerminateP(const BB1BB& /* bb */)
{
	// le fonctionnement du Blackboard ne devrait pas s'arrêter, les données qui
	// arrivent dessus, arrivent au fur et à mesure que le médecin les saisie
	// (pour les données relatives à la consultation), ou au fur et à mesure que
	// la recherche dans Nautilus donne ses résultats

	return false ;
}

bool
reinitBlackboard(NSContexte *pContexte)
{
  // FIXME

  BB1BB* tempBb = pContexte->getBlackboard() ;
  if (tempBb)
    tempBb->reinit() ;

  //if ( pContexte->getSuperviseur()->blackboardInterface  != NULL)
  	//delete (pContexte->getSuperviseur()->blackboardInterface );

  CreateInterface(pContexte) ;

  //TypedVal  Quest(std::string("Init"));

  // if (pContexte->getBBinterface())
  // {
/*
    std::string question("0PRO1/IsA/");
    std::string args("PDIAB");
    NautilusQuestion val(question, args);
    TypedVal  quest0(&val);
    pContexte->getSuperviseur()->getBBinterface()->ComputeQuestion(&quest0, NULL);
*/

    // TypedVal  quest(std::string("Init"));
    // pContexte->getSuperviseur()->getBBinterface()->ComputeQuestion(&quest, NULL);

  //   pContexte->getBBinterface()->addNautilusEvent(std::string("Init")) ;
  // }
                              //PreInit
  //AskDeterministicQuestion *PreInitMessage = new AskDeterministicQuestion("init","init", 10);
  //pContexte->getSuperviseur()->bbkToDo(pContexte,0, "AskDeterministicQuestion", "", "", PreInitMessage ,true, NULL,false);
// AskDeterministicQuestion *InitMessage = new AskDeterministicQuestion("Initialisation fenetre","Init", 10);
 //pContexte->getSuperviseur()->bbkToDo(pContexte,0, "AskDeterministicQuestion", "", "", InitMessage ,true, NULL,false);
  //AskDeterministicQuestion *DPIo = new AskDeterministicQuestion("Calcul du risque du colon","DPIOColon", 10);
  //pContexte->getSuperviseur()->bbkToDo(pContexte,0, "AskDeterministicQuestion", "", "", DPIo ,true, NULL,false);

  return true ;
}

void
addInitEvent(NSContexte *pContexte)
{
  if (pContexte && pContexte->getBBinterface())
    pContexte->getBBinterface()->addNautilusEvent(std::string("Init")) ;
}

void
CheckZeroObjectCount(NSSuper *pSuper, string sObjectName, long lInstanceCounter)
{
  if ((long)0 == lInstanceCounter)
    return ;

  char buf[255] = {0} ;
  sprintf(buf, "%d", lInstanceCounter) ;

  string sTrace = string("Memory leak for object ") + sObjectName +
                  string(" (Instance counter = ") + string(buf) + string(")") ;
	pSuper->trace(&sTrace, 1) ;
}

bool
BB1BB::reinit()
{
#ifdef __DEBUG__
	string	sTrace = string("reinitialization of Blackboard - cleaning KBs PREDI, PatientDatas and all events that have been created") ;
	pContexte->getSuperviseur()->trace(&sTrace, 1);
#endif

	// clear PatientDatas kb
	BB1KB	*patiendatasKB = KBNamed("InformationKB") ;
  if (patiendatasKB)
	  patiendatasKB->EmptyObjectSet() ;

  BB1KB *QuestionKB = KBNamed("QuestionKB") ;
  if (QuestionKB)
    QuestionKB->EmptyObjectSet() ;

  BB1KB *eventsKB = KBNamed("NautilusEventKB") ;
  if (eventsKB)
    eventsKB->EmptyObjectSet() ;

	_kenoby.ReInitControler() ;

	_subStratGarbagge.vider() ;

  // reinit counters
  //
  OB1Token::_token_counter = 0 ;

  return true ;
}

void
BB1BB::addToSubStratGarbage(OB1SubStrategy* pSubStrat)
{
  if (NULL == pSubStrat)
    return ;

	_subStratGarbagge.push_back(pSubStrat) ;
}

void
BB1BB::dispatchLogMessage(string sMessage, int /* iAlertLevel */, string sBefore, string sAfter)
{
#ifndef _ENTERPRISE_DLL
	if (NULL == pContexte->getPredi())
		return ;

	pContexte->getPredi()->addToLogPage(sMessage, sBefore, sAfter) ;
#endif
}

