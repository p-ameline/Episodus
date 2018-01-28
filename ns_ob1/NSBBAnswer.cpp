
#include "ns_ob1\NSBBAnswer.h"


NSBBAnswer::NSBBAnswer(NSBBQuestion *theQuestion, NSPatPathoInfo *theNode, string thePath)
{
	question	= theQuestion ;
	node			= theNode ;
	path			= thePath ;
}


NSBBAnswer::NSBBAnswer(BB1Object& obj)
{
	obj.Value("label",				NULLSTRING) ;
	obj.Value("node",					NULLPATHO) ;
	obj.Value("path",					NULLPATHS) ;
	obj.Value("QuestionPtr",	NULLPTR) ;
	obj.Value("Date",					NULLSTRING) ;
	obj.Value("StringValue",	NULLSTRING) ;
	obj.Value("IntValue",			NULLINT) ;
	obj.Value("unitValue",		NULLSTRING) ;
}
