//---------------------------------------------------------------------------
//     NSANXARY.H
//
//  Contient les d�finitions des arrays d'objets annexes de NAUTILUS
//
//  Ce code est s�par� de NSANNEXE car il n'est pas possible de d�clarer
//  une Array comme Import
//
//  1ere version : PA juillet 94   Derni�re modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSSAVARY_H
#define __NSSAVARY_H

#include <classlib\arrays.h>

#include "nssavoir\nspatho.h"
#include "nssavoir\ns_fic.h"
#include "partage\ns_vector.h"
#include "nsoutil/nsexport.h"
//
// D�finition de NSSavoirArray (Array de NSSavoirInfo(s))
//
typedef vector<NSSavoirInfo*> NSSavInfoArray;
typedef NSSavInfoArray::iterator SavInfoIter;

class _NSOUTILCLASSE NSSavoirArray : public NSSavInfoArray
{
	public :
		// Constructeurs
		NSSavoirArray() : NSSavInfoArray() {}
		NSSavoirArray(NSSavoirArray& rv);
		// Destructeur
		virtual ~NSSavoirArray();
   	void vider();
};

//
// D�finition de NSFicSavArray (Array de NSSavFicheInfo(s))
//
typedef vector<NSSavFicheInfo*> NSFicInfoArray;
typedef NSFicInfoArray::iterator SavFichIter;

class _NSOUTILCLASSE NSFicSavArray : public NSFicInfoArray
{
	public :

		// Constructeurs
		NSFicSavArray() : NSFicInfoArray() {}
		NSFicSavArray(NSFicSavArray& rv);
		// Destructeur
		virtual ~NSFicSavArray();
   	void vider();
};


//---------------------------------------------------------------------------
#endif    // __NSANXARY_H