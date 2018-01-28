//---------------------------------------------------------------------------//    NSSAVARY.CPP
//    PA  mai 98
//  Implémentation des Arrays d'objets annexes du réseau sémantique de NAUTILUS
//---------------------------------------------------------------------------

#include "nsoutil\nssavary.h"

//***************************************************************************
// 					Implémentation des méthodes NSSavoirArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSSavoirArray::NSSavoirArray(NSSavoirArray& rv)
                :NSSavInfoArray()
{
    if (!(rv.empty()))
        for (SavInfoIter i = rv.begin(); i != rv.end(); i++)
   	        push_back(new NSSavoirInfo(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSSavoirArray::vider()
{
    if (empty())
        return ;

    for (SavInfoIter i = begin(); i != end(); )
    {
   	    delete *i ;
        erase(i) ;
    }
}

NSSavoirArray::~NSSavoirArray()
{
	vider() ;
}

//***************************************************************************
// 					Implémentation des méthodes NSFicSavArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFicSavArray::NSFicSavArray(NSFicSavArray& rv)
				  :NSFicInfoArray()
{
    if (!(rv.empty()))
	    for (SavFichIter i = rv.begin(); i != rv.end(); i++)
   	        push_back(new NSSavFicheInfo(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSFicSavArray::vider()
{
    if (empty())
        return ;

    for (SavFichIter i = begin(); i != end(); )
    {
   	    delete *i ;
        erase(i) ;
    }
}

NSFicSavArray::~NSFicSavArray()
{
	vider() ;
}

///////////////////////// fin de nssavary.cpp ////////////////////////////////

