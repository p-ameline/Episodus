#ifndef __NSBBANSWER_H__

# define __NSBBANSWER_H__

# include "ns_ob1\NSBBQuestion.h"
# include "nsbb\nspatpat.h"

class NSBBAnswer
{
 public:
  // constructeur, destructeur
	NSBBAnswer(NSBBQuestion *theQuestion, NSPatPathoInfo *theNode, string thePath) ;
	NSBBAnswer(BB1Object& obj) ;
	~NSBBAnswer() {} ;

	BB1Object	*toObject(BB1BB& bb) ;
	
	// variables
	string					sLabel ;
	
	NSPatPathoInfo	*node ;
	string					path ;

	NSBBQuestion		*question ;
	string					sDate ;
	string					sValue ;
	int							iValue ;
  string					sUnitValue ;
} ;

#endif // __NSBBANSWER_H__
