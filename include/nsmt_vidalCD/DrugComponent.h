// -----------------------------------------------------------------------------
// fichier DrugComponent.h
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2004/05/10 18:11:50 $
// -----------------------------------------------------------------------------
// FLP - avril/mai 2004
// -----------------------------------------------------------------------------

#ifndef __DRUGCOMPONENT_H__
# define __DRUGCOMPONENT_H__



# ifndef __DANSVIDALDLL__
#  define __VIDALEXPORT__	__import
# else
#  define __VIDALEXPORT__	__export
# endif

# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

# include <string.h>
/*
# include <stdio.h>
# include <fstream.h>
*/

class	DrugComponent
{
 public:
  enum                    DCType { notDefined = 0, ActiveIngredient, Excipient, EEN } ;

	DrugComponent() ;
	DrugComponent(DrugComponent& src) ;
  DrugComponent(string sLibelle, DCType eType, string sVidal = "", bool bSyn = false, string sSyn = "") ;
  DrugComponent(string sLibelle, string sIndic, string sVidal = "", bool bSyn = false, string sSyn = "") ;
//	DrugComponent(string sLibelle, string sCodeVidal, string sIndice) ;
	~DrugComponent() ;

	DrugComponent&					operator=(DrugComponent& src) ;
  int                     operator==(DrugComponent& src) ;

  string                  getLabel()                                            { return sLabel ; }
  string                  getCodeVidal()                                        { return sCodeVidal ; }
  bool                    isSynonym()                                           { return bSynonym ; }
  string                  getSynonym()                                          { return sSynonym ; }

  DCType                  getType()                                             { return type ; }

 protected:
	string									sLabel ;
	string									sCodeVidal ;
	string									sLexiqCode ;
  bool                    bSynonym ;
  string                  sSynonym ;

  DCType                  type ;
} ;


typedef vector<DrugComponent *>					  VectorOfDrugComponents ;
typedef VectorOfDrugComponents::iterator	DrugComponentIter ;


class	ArrayOfDrugComponents : public VectorOfDrugComponents
{
 public:
	ArrayOfDrugComponents() ;
	ArrayOfDrugComponents(ArrayOfDrugComponents& src) ;
	~ArrayOfDrugComponents() ;

	ArrayOfDrugComponents&	operator=(ArrayOfDrugComponents& src) ;
	void										vider() ;

  bool                    isP(DrugComponent *pCompo) ;
  void                    retrieve(DrugComponent *pCompo) ;  
//	int											init(string sArray) ;
} ;


#endif // __DRUGCOMPONENT_H__
