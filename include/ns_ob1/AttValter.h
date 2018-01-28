//
// AttValTer.h
//
// Made by (Tabun)
// Login   <giocan_d@epita.fr>
//
// Started on  Jan Sep 16 16:16:25 2004 Tabun
//

#ifndef   ATTVALTER_H_
# define   ATTVALTER_H_

# ifdef _DANSNSKUTILDLL
#  define __NSKSUTILEXPORT__ __export
# else
#  define __NSKSUTILEXPORT__ __import
# endif

#include "InitStructure.h"
#include <vector>

enum SCRIPT_TYPE
{
  SINT     = 0,
  SDOUBLE  = 1,
  SSTRING  = 2,
  SBOOL    = 3
};

/* Unit to generate data to interpreter */
class InterpretableData
{
  public :
  SCRIPT_TYPE   _type;
  std::string   _unit; /* unité de l'interpreter */
  InterpretableData(SCRIPT_TYPE type, std::string test);
};


class __NSKSUTILEXPORT__ INodeScript : public INode
{
  protected :
    std::vector<InterpretableData*> _ListOfTypeToGenerate;
  public:
  INodeScript(TypedVal* label, std::string clasto,std::string clasform);
  INodeScript(INode& temp);
  ~INodeScript();
  InterpretableData& operator[] (int i)
  {
    if ((i >= 0)  && (i < _ListOfTypeToGenerate.size()))
      return   *(_ListOfTypeToGenerate[i]);
  }
};

#endif  /* ATTVALTER */
