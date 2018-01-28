// -----------------------------------------------------------------------------// bdm_bases.h
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: fabrice $
// $Date: 2003/07/10 09:51:55 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------
// Définition des objets de base de donnée de base medicamenteuse
// -----------------------------------------------------------------------------
#ifndef __BDMBASES_H__
# define __BDMBASES_H__


# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

# include "ns_sgbd\nsfiche.h"
// Taille des champs du fichier BDM# define BDM_ID_LEN						4# define BDM_CIP_LEN    		 20   // Code CIP
# define BDM_EXT_LEN      		2   // Extension Nautilus
# define BDM_LEXIMED_LEN  		5   // Code Leximed
# define BDM_VIDAL_LEN    		7   // Code Vidal
# define BDM_LEXOLD_LEN   		6   // Code ancien Leximed
# define BDM_GRP_GENERIQ_LEN  5   // Groupe générique de substitution
# define BDM_TYPE_GENERIQ_LEN 1   // Relation générique
// Indice des champs du fichier BDM# define BDM_ID_FIELD					  1
# define BDM_CIP_FIELD				  2
# define BDM_EXT_FIELD				  3
# define BDM_LEXIMED_FIELD      4
# define BDM_VIDAL_FIELD        5
# define BDM_LEXOLD_FIELD       6
# define BDM_GRP_GENERIQ_FIELD  7   // Groupe générique de substitution
# define BDM_TYPE_GENERIQ_FIELD 8   // Relation générique


// **************************** Table BDM.DB ****************************
//---------------------------------------------------------------------------//  Classe NSPatLinkData
//
// Objet contenant les données
//---------------------------------------------------------------------------
//

class NSBdmData{
  public:

    char id         [BDM_ID_LEN           + 1] ;
	  char codeCIP    [BDM_CIP_LEN          + 1] ;
	  char extension  [BDM_EXT_LEN          + 1] ;
	  char leximed    [BDM_LEXIMED_LEN      + 1] ;
	  char codeVidal  [BDM_VIDAL_LEN        + 1] ;
	  char leximedOld [BDM_LEXOLD_LEN       + 1] ;
    char generiqGrpe[BDM_GRP_GENERIQ_LEN  + 1] ;
    char generiqType[BDM_TYPE_GENERIQ_LEN + 1] ;

    NSBdmData() ;	  NSBdmData(const NSBdmData& rv) ;
	  ~NSBdmData() ;

	  NSBdmData&	operator=(const NSBdmData& src) ;
	  int 	    	operator==(const NSBdmData& o) ;

    string getId()          const { return string(id) ;  }
	  string getCodeCIP()     const { return string(codeCIP) ; }
	  string getExtension()   const { return string(extension) ; }
	  string getLeximed()     const { return string(leximed) ; }
	  string getCodeVidal()   const { return string(codeVidal) ;  }
	  string getLeximedOld()  const { return string(leximedOld) ; }
    string getGeneriqGrpe() const { return string(generiqGrpe) ; }
    string getGeneriqType() const { return string(generiqType) ; }

    void   setId(string sS)          { strcpy(id, sS.c_str()) ;          }
	  void   setCodeCIP(string sS)     { strcpy(codeCIP, sS.c_str()) ;     }
	  void   setExtension(string sS)   { strcpy(extension, sS.c_str()) ;   }
	  void   setLeximed(string sS)     { strcpy(leximed, sS.c_str()) ;     }
	  void   setCodeVidal(string sS)   { strcpy(codeVidal, sS.c_str()) ;   }
	  void   setLeximedOld(string sS)  { strcpy(leximedOld, sS.c_str()) ;  }
    void   setGeneriqGrpe(string sS) { strcpy(generiqGrpe, sS.c_str()) ; }
    void   setGeneriqType(string sS) { strcpy(generiqType, sS.c_str()) ; }

	  void				metAZero() ;
} ;

class NSBdm ;
//---------------------------------------------------------------------------//  Classe NSBdmInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSBdmInfo
{
  public:

    NSBdmData	Donnees ;

	  NSBdmInfo() ;
	  NSBdmInfo(NSBdm*) ;
	  NSBdmInfo(const NSBdmInfo& rv) ;
	  ~NSBdmInfo() ;

    string getId()          const { return Donnees.getId() ;          }
	  string getCodeCIP()     const { return Donnees.getCodeCIP() ;     }
	  string getExtension()   const { return Donnees.getExtension() ;   }
	  string getLeximed()     const { return Donnees.getLeximed() ;     }
	  string getCodeVidal()   const { return Donnees.getCodeVidal() ;   }
	  string getLeximedOld()  const { return Donnees.getLeximedOld() ;  }
    string getGeneriqGrpe() const { return Donnees.getGeneriqGrpe() ; }
    string getGeneriqType() const { return Donnees.getGeneriqType() ; }

    void   setId(string sS)          { Donnees.setId(sS) ;          }
	  void   setCodeCIP(string sS)     { Donnees.setCodeCIP(sS) ;     }
	  void   setExtension(string sS)   { Donnees.setExtension(sS) ;   }
	  void   setLeximed(string sS)     { Donnees.setLeximed(sS) ;     }
	  void   setCodeVidal(string sS)   { Donnees.setCodeVidal(sS) ;   }
	  void   setLeximedOld(string sS)  { Donnees.setLeximedOld(sS) ;  }
    void   setGeneriqGrpe(string sS) { Donnees.setGeneriqGrpe(sS) ; }
    void   setGeneriqType(string sS) { Donnees.setGeneriqType(sS) ; }

	  NSBdmInfo& operator=(const NSBdmInfo& src) ;
	  int 	     operator==(const NSBdmInfo& o) ;
} ;

//---------------------------------------------------------------------------
//  Classe NSBdm
//
// Objet dérivé de NSFiche servant aux accès de base de données
//---------------------------------------------------------------------------
//
class NSBdm : public NSBdmInfo, public NSFiche
{
  public:

    NSBdm(NSContexte* pCtx) ;
	  NSBdm(const NSBdm& rv) ;
	  ~NSBdm() ;

	  void      alimenteFiche() ;
	  void      videFiche() ;
	  DBIResult	open() ;

	  NSBdm&    operator=(const NSBdm& src) ;
    int 	    operator==(const NSBdm& o) ;
} ;

//
// Définition de NSBdmArray (Array de NSBdmInfo(s))
//

typedef vector<NSBdmInfo*>        NSBMArray ;
typedef NSBMArray::iterator       NSBdmIter ;
typedef NSBMArray::const_iterator NSBdmConstIter ;

class NSBdmArray : public NSBMArray
{
  public:

	  // Constructeurs
	  NSBdmArray() : NSBMArray() {}
	  NSBdmArray(const NSBdmArray& rv) ;
	  // Destructeur
	  virtual			~NSBdmArray() ;
	  void				vider() ;

	  NSBdmArray&	operator=(const NSBdmArray& src) ;
} ;

//---------------------------------------------------------------------------
#endif    // __BDMBASES_H__

