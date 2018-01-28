#include "AttValter.h"

INodeScript::~INodeScript()
{
	if (_ListOfTypeToGenerate.empty())
		return ;

  register int end = _ListOfTypeToGenerate.size() ;
  for (register int i = 0; i < end; i++)
    delete(_ListOfTypeToGenerate[i]) ;
  _ListOfTypeToGenerate.clear() ;
}



