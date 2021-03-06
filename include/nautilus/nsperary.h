//---------------------------------------------------------------------------//     NSANXARY.H
//
//  Contient les d�finitions des arrays d'objets annexes de NAUTILUS
//
//  Ce code est s�par� de NSANNEXE car il n'est pas possible de d�clarer
//  une Array comme Import
//
//  1ere version : PA juillet 94   Derni�re modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSPERARY_H
#define __NSPERARY_H

class NSPatInfo ;
class NSCorrespondantInfo ;
class NSUtiliInfo ;

#include <classlib\arrays.h>

// #include "nautilus\nsannexe.h"
// #include "nsdn\nsdocum.h"
#include "partage\ns_vector.h"#include "partage\NTArray.h"//// D�finition de NSPatientArray (Array de NSPatient(s))//typedef vector<NSPatInfo*> NSPatInfoArray ;
typedef NSPatInfoArray::iterator         PatientIter ;typedef NSPatInfoArray::const_iterator   PatientConstIter ;typedef NSPatInfoArray::reverse_iterator PatientReverseIter ;typedef NTArray<NSPatInfo> NSPatientArray ;
//
// D�finition de NSCorrespArray (Array de NSCorrespondant(s))
//
typedef vector<NSCorrespondantInfo*> NSCorrespInfoArray ;
typedef NSCorrespInfoArray::iterator       CorrespInfoIter ;typedef NSCorrespInfoArray::const_iterator CorrespInfoConstIter ;class NSCorrespArray : public NSCorrespInfoArray{  public :
    // Constructeurs	  NSCorrespArray() : NSCorrespInfoArray() {}
	  NSCorrespArray(const NSCorrespArray& rv) ;
	  // Destructeur
	  virtual ~NSCorrespArray() ;

    NSCorrespArray& operator=(const NSCorrespArray& src) ;
    int             operator==(const NSCorrespArray& o) ;
    void vider() ;
};
//// D�finition de NSUtiliArray (Array de NSUtiliInfo(s))
//
typedef vector<NSUtiliInfo*> NSUtiliInfoArray ;
typedef NSUtiliInfoArray::iterator       UtiliInfoIter ;
typedef NSUtiliInfoArray::const_iterator UtiliInfoConstIter ;
typedef NTArray<NSUtiliInfo> NSUtiliArray ;

//---------------------------------------------------------------------------

#endif    // __NSPERARY_H

