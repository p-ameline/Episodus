//---------------------------------------------------------------------------
//    NSANXARY.CPP
//    PA   juillet 94
//  Implémentation des Arrays d'objets annexes de NAUTILUS
//---------------------------------------------------------------------------

#ifdef _INCLUS
  #include "partage\Nsglobal.h"
  #include "nsbb\nsutidlg.h"
#endif

#include "nautilus\nsanxary.h"

//***************************************************************************
// 					Implémentation des méthodes NSDocumentArray
//***************************************************************************
  /*
//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDocumentArray::NSDocumentArray(NSDocumentArray& rv)
                :NSDocInfoArray()
{
    if (!(rv.empty()))
        for (DocInfoIter i = rv.begin(); i != rv.end(); i++)
   	        push_back(new NSDocumentInfo(*(*i)));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSDocumentArray::vider()
{
    if (empty())
        return;
    for (DocInfoIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}


NSDocumentArray::~NSDocumentArray()
{
	vider();
}
*/
//***************************************************************************
// Implémentation des méthodes NSChemiseArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
/*
NSChemiseArray::NSChemiseArray(NSChemiseArray& rv)
               :NSChemInfoArray()
{
    if (!(rv.empty()))
	    for (ChemInfoIter i = rv.begin(); i != rv.end(); i++)
   	        push_back(new NSChemiseInfo(*(*i)));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSChemiseArray::vider()
{
    if (empty())
        return;
	for (ChemInfoIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSChemiseArray::~NSChemiseArray()
{
	vider();
}*/
/*
//***************************************************************************// 				Implémentation des méthodes NSArmoireArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSArmoireArray::NSArmoireArray(NSArmoireArray& rv)
					:NSArmoiInfoArray()
{
    if (!(rv.empty()))
	    for (ArmoiInfoIter i = rv.begin(); i != rv.end(); i++)
   	        push_back(new NSArmoireInfo(*(*i)));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSArmoireArray::vider()
{
    if (empty())
        return;
	for (ArmoiInfoIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSArmoireArray::~NSArmoireArray()
{
	vider();
}

*/
///////////////////////// fin de nsanxary.cpp ////////////////////////////////
