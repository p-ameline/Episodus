// -----------------------------------------------------------------------------
// nsprocess.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: pameline $
// $Date: 2015/02/07 14:27:55 $
// -----------------------------------------------------------------------------
// classe représentant les processes
// -----------------------------------------------------------------------------
// FLP - aout 2003
// -----------------------------------------------------------------------------

#ifndef __NSPROCESS_H__
# define __NSPROCESS_H__

# include "nautilus\nssuper.h"
# include "nsepisod\nsldvuti.h"

class NSProcess : public NSRoot
{
 public:
  // constructeurs/destructeur
  NSProcess(NSContexte *pCtx) ;
  NSProcess(const NSProcess& rv) ;
  ~NSProcess() ;

  // operateur =
  NSProcess&      operator=(const NSProcess& src) ;

  // fonctions
  string          getDocNoeud()               { return (sReference) ; }
  void            setDocNoeud(string sNode)   { sReference = sNode ; }

  string          getMetaNoeud()              { return (sMetaReference) ; }
  void            setMetaNoeud(string sMeta)  { sMetaReference = sMeta ; }

  string          getLexique()                { return (sLexique) ; }
  void            setLexique(string sLex)     { sLexique = sLex ; }

  bool            isLinkedWithConcern(string sConcern) ;

  // variables
  string          sTitre ;
  string          sReference ;
  string          sMetaReference ;
  string          sLexique ;
  NVLdVTemps      tDateDemande ;
  NVLdVTemps      tDateOuverture ;
  NVLdVTemps      tDateFermeture ;
  VecteurString   aConcerns ;
} ;


typedef vector<NSProcess *>               ProcessesVector ;
typedef ProcessesVector::iterator         ProcessesIter ;
typedef ProcessesVector::const_iterator   ProcessesConstIter ;
typedef ProcessesVector::reverse_iterator ProcessesRIter ;


class ArrayProcesses : public ProcessesVector
{
 public:
  // constructeurs/destructeur
  ArrayProcesses(NSLdvDocument *pDocum) : ProcessesVector(), pDoc(pDocum) {}
  ArrayProcesses(const ArrayProcesses& src) ;
  ~ArrayProcesses() ;

  // operateur =
  ArrayProcesses& operator=(const ArrayProcesses& src) ;

  // fonctions
  void            vider() ;
//  void            initialiser() ;
//  void            loadProcesses(NSPatPathoArray *pPatPatoArray, PatPathoIter iter, int iColBase, bool bJustOne = false) ;

//  void            reinit() ;
//  void            reloadProcesses(NSPatPathoArray *pPatPatoArray, PatPathoIter iter, int iColBase) ;

//  void            deleteProcess(NSProcess *pProcess) ;
//  NSProcess       *getProcess(string sRef) ;

  // variables
  NSLdvDocument   *pDoc ;
} ;

# endif // __NSPROCESS_H__
