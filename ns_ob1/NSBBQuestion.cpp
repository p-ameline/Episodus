
#include "ns_bbk\NSBBQuestion.h"

/*
#include "ns_bbk\BB1BB.h"
#include "ns_bbk\BB1Object.h"
#include "ns_bbk\BB1Class.h"



NSBBQuestion::NSBBQuestion(	string	label,
														bool		ask,
														string	dateMin,
														string	dateMax,
														string	question,
														string	unitValue,
														string	formulaire,
														string	archeType,
														string	flagRules,
														string	searchDepth,
														string	searchState)
{
	sLabel				= label ;
	askuser				= ask ;
	sDateMin			= dateMin ;
	sDateMax			= dateMax ;
	sQuestion			= question ;
	sUnitValue		= unitValue ;
	sFormulaire		= formulaire ;
	sArcheType		= archeType ;
	sFlagRules		= flagRules ;
	sSearchDepth	= searchDepth ;
	sSearchState	= searchState ;
	
	kss						= new vector<BB1KS *> ;
	answers				= new vector<NSBBAnswer *> ;
}


NSBBQuestion::NSBBQuestion(BB1Object &obj)
{
	obj.Value("label",				sLabel) ;
	obj.Value("askuser",			askuser) ;
	obj.Value("DateMin",			sDateMin) ;
	obj.Value("DateMax",			sDateMax) ;
	obj.Value("unitValue",		sUnitValue) ;
	obj.Value("question",			sQuestion) ;
	obj.Value("formulaire",		sFormulaire) ;
	obj.Value("archetype",		sArcheType) ;
	obj.Value("searchDepth",	sSearchDepth) ;
	obj.Value("searchState",	sSearchState) ;

	obj.Value("answers",			answers) ;
	obj.Value("KSs",					kss) ;
	obj.Value("nbAnswers",		nbAnswers) ;
}


NSBBQuestion::~NSBBQuestion()
{
	// il ne faut pas détruire les KSs à ce niveau !
	// on efface juste la liste des pointeurs sur ces KSs à partir de l'objet
	// NSBBQuestion
	// si on effaçait les KSs, ceux-ci seraient également détruits au sein du
	// Blackboard et non uniquement au sein de cet objet
	
	delete kss ;


	// on efface les réponses qui correspondent à cette question
	// il faudra faire attention (en optimisation) aux réponses qui serait liées à
	// plusieurs questions !

	for (vector<NSBBAnswer *>::iterator NSBBAIter = answers->begin() ; NSBBAIter != kss->end() ; )
		delete (*NSBBAIter) ;
	delete answers ;
}


BB1Object		*NSBBQuestion::toBB1Object(BB1BB &bb)
{
	BB1KB *kb = bb.KBNamed("PatientDatas") ;
	BB1Class *theclass = bb.ClassNamed("NeedDatas") ;

	AttValPair	labelAVP				("label",				sLabel) ;
	AttValPair	askAVP					("askuser",			askuser) ;

	AttValPair	questionAVP			("question",		sQuestion) ;
	AttValPair	dateMinAVP			("DateMin",			sDateMin) ;
	AttValPair	dateMaxAVP			("DateMax",			sDateMax) ;
	AttValPair	unitAVP					("unitValue",		sUnitValue) ;

	AttValPair	formulaireAVP		("formulaire",	sFormulaire) ;
	AttValPair	archetypeAVP		("archetype",		sArcheType) ;

	AttValPair	searchDepthAVP	("searchDepth",	sSearchDepth) ;
	AttValPair  searchStateAVP	("searchState",	sSearchState) ;

	AttValPair	needDatasAVP		("answers",			Voidptr(answers)) ;
	AttValPair	kssAVP					("KSs",					Voidptr(kss)) ;
	AttValPair	nbAnswersAVP		("nbAnswers",		nbAnswers) ;

//	theclass->MakeInstance(sLabel, kb, Collect()) ;
}
*/
