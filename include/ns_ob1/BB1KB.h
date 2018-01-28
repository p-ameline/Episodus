/*
**
** BB1KB.h
**
*/

/*
** The set of knowledge bases is a partition of the blackboard.
** There can be any number of knowledge bases.
** Every knowledge base has a unique non-empty name.
** One can retrieve a knowledge base's name.
** One can retrieve a knowledge base using its name as index.
** A failed attempt to retrieve a knowledge base from its name index is not a fatal error.
** It is usually loaded from or saved to a file.
** Attributes and links are declared blackboard by blackboard.
*/

#ifndef _BB1KB_h
#define _BB1KB_h 1

class BB1BB ;
class BB1Class ;
class BB1KSAR ;
class BB1Link ;
class ObjectSet ;

#ifdef __GNUC__
#pragma interface
#endif

#include "ns_ob1\OB1Export.h"
#include "BB1types.h"
// #include "BB1BB.h"

/**
* \brief Collection of \relates BB1Object
*/
/**
* Collection of \relates BB1OBject <BR>
* Collection de \relats BB1OBject
*/
class _BBKEXPORT BB1KB
{
	friend class NBBKB ;
	friend class BB1BB ;
	friend class BB1Object ;
	friend class BB1KSAR ;
	friend class KBSet ;

//	friend bool RiskFactorKSPrecondition(const BB1BB& bb) ;
//	friend Errcode RiskFactorKSAction(BB1BB& bb) ;
//	friend Errcode SearchNSKSAction(BB1BB& bb) ;

  public: // API

	  BB1KB(const string& name, BB1BB& BB, Cycle cycle) ;
	  BB1KB(const BB1KB& src) ;
	  ~BB1KB() ;

	  // Return a reference to the BB to which the KB belongs.
    inline BB1BB& BB() const          { return _bb ; }

    // Return the pathname of the file from which the KB was loaded
    const string& FileName() const    { return _filename ; }

    // Return the name of the knowledge base
    inline const string& Name() const { return _name ; }

    // If there is an object with the given name in the given KB, return a pointer to it; otherwise, return a null pointer.
    BB1Object* ObjectNamed(const string sObjectName) const ;

    // If there is an object with the given name in the given KB, delete it.
    void       DeleteObjectNamed(const string sObjectName) ;
    void       DeleteObjectForPath(const string sPath) ;

    // If there is an object with the given name in the given KB, just erase it.
    void       EraseObjectNamed(const string sObjectName) ;

    // Return a set of all objects in the knowledge base.
    const ObjectSet	*ObjectsInKB() const { return &_objects ; }

    // Is the knowledge base protected against user modifications?
    int		ProtectedP() const             { return _protectedP ; }

    Errcode		Save(const string& fileName) ;

  //private:
  protected:

    inline Cycle CycleCreated() const { return _cycleCreated ; }
    void			   DeleteClasses() ;
    void			   DeleteClassSubtree(BB1Class *root) ;
    bool			   operator==(const BB1KB& kb2) const	{ return (Name() == kb2.Name()) ; }
    bool			   operator<=(const BB1KB& kb2) const	{ return (Name() <= kb2.Name()) ; }

    // Insert the object into the KB.
    Errcode		InsertObject(BB1Object* obj) ; 

    // Protect/unprotect the knowledge base against user modifications
    Errcode		SetProtected(const int ProtectedP)  { _protectedP = ProtectedP ; return (SUCCESS) ; }

    // Empty the ObjectSet
    void      EmptyObjectSet()                    { _objects.vider() ; }

    // Fonction renvoyant le cycle courrant
    Cycle			ThisCycle() ;

  private:

    int						_protectedP ;			// is this a built-in KB?
    const string	_name ;						// the KB's unique identifier
    ObjectSet			_objects ;				// set of objects in the KB
    string				_filename ;				// file name from which loaded and to which saved
    BB1BB&				_bb ;							// back pointer to the blackboard
    const Cycle		_cycleCreated ;		// the cycle on which the KB was created
} ;

#endif // _BB1KB_h

