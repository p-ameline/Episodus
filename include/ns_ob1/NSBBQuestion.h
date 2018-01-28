//
// NSBBQuestion.h
//

#ifndef __NSBBQUESTION_H__
# define __NSBBQUESTION_H__

# include <string>

# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

# include "ns_ob1\BB1KS.h"

class NSBBAnswer ;


class NSBBQuestion
{
 public:
	// constructeur, destructeur
	NSBBQuestion(	string	label,
								bool 		ask,
								string	dateMin,
								string	dateMax,
								string	question,
								string	unitValue,
								string	formulaire,
								string	archeType,
								string	flagRules,
								string	searchDepth,
								string	searchState) ;
	NSBBQuestion(BB1Object &obj) ;
	~NSBBQuestion() ;

	BB1Object		*toBB1Object(BB1BB &bb) ;

	
	// variables
	string								sLabel ;
	bool									askuser ;
	string								sDateMin ;
	string								sDateMax ;
	string								sQuestion ;

	string								sUnitValue ;

	string								sFormulaire ;
	string								sArcheType ;

	string								sFlagRules ;
	string								sSearchDepth ;
	string								sSearchState ;

	int										nbAnswers ;
	
	vector<BB1KS *>				*kss ;
  vector<NSBBAnswer *>	*answers ;
} ;

#endif // __NSBBQUESTION_H__
