//---------------------------------------------------------------------------
//     NSCACHE.CPP
//
//  Implémentation de CacheElement.
//  Implémentation de NSCache.
//
//  KP
//  Création : janv.92         Dernière mise à jour : sept 93
//---------------------------------------------------------------------------

//#include <stdio.h>



#include "nsutil\nscache.h"

//#include <cstring.h>
//--------------------------------------------------------------------------
//
// Implémentation de NSCacheElement
//
//--------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  Function: CacheElement::CacheElement(CacheElement& src)
//
//  Arguments:
//            src  -> CacheElement à copier
//  Description:
//            Constructeur copie. Défini pour assurer la sémantique copie.
//            ATTENTION : Les CacheElement ne sont pas redimensionnables.
//                        On en définit la taille dans le constructeur.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
CacheElement::CacheElement(CacheElement& src)
{
	nbItems = src.nbItems;

	// On alloue un tableau non redimensionnable
	listItems = new ChaineArray(nbItems, 0);

	for (int i = 0; i < nbItems; i++)
		listItems->Add(src.getItem(i));
}

//---------------------------------------------------------------------------
//  Function: CacheElement::CacheElement(ChaineArray& src, int indexBegin,
//                                       int nb)
//  Arguments:
//            src        ->  Vecteur de Chaine à partir de laquelle on veut
//                           construire un CacheElement
//            indexBegin ->  Index de début
//            nb         ->  Nombre d'éléments de src intervenant.
//  Description:
//            Ce constructeur est destiné à être utilisé dans strAnalyse()
//            puisque à l'aide de strtok() on peut construire un vecteur
//            de chaînes à partir duquel on construit des CacheElement.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
CacheElement::CacheElement(ChaineArray& src, int indexBegin, int nb)
{
	nbItems = nb;
	listItems = new ChaineArray(nbItems, 0);

	for (int i = 0; i < nbItems; i++)
		listItems->Add(src[indexBegin+i]);
}

//---------------------------------------------------------------------------
//  Function: RCacheElement CacheElement::operator=(CacheElement src)
//
//  Arguments:
//            src  ->  CacheElement que l'on veut affecter
//  Description:
//            Cet opérateur est redéfini pour préserver la sémantique copie.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
CacheElement& CacheElement::operator=(CacheElement src)
{
	if (listItems)
		delete listItems;

	nbItems = src.nbItems;
	listItems = new ChaineArray(nbItems, 0);

	for (int i = 0; i < nbItems; i++)
		listItems->Add(src.getItem(i));

	return *this;
}

//---------------------------------------------------------------------------
//  Function: int CacheElement::operator==(const CacheElement& src)
//
//  Arguments:
//            src  ->  CacheElement que l'on veut affecter
//  Description:
//            Cet opérateur est redéfini pour préserver la sémantique copie.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
int CacheElement::operator==(const CacheElement& x) {
	bool result = true;       // Résultat de la comparaison terme à terme

	result = (nbItems != x.nbItems);

	for ( int i = 0; (i < nbItems)&& result; i++)
		result = ((*(this->listItems))[i] == (*(x.listItems))[i]);
	return result;
}

//--------------------------------------------------------------------------
//
// Implémentation de CacheArray
//
//--------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  Function: CacheArray::CacheArray(RCacheArray rv)
//
//  Arguments:
//            src  -> CacheArray que l'on veut copier
//  Description:
//            Constructeur copie.
//  Returns:
//
//---------------------------------------------------------------------------
CacheArray::CacheArray(CacheArray& rv)
: CacheElementArray(), nIndexCurr(0)
{
   if (!(rv.empty()))
	   for (CacheElementArrayIter i = rv.begin(); i != rv.end(); i++)
			 	push_back(new CacheElement(*(*i)));
}
//---------------------------------------------------------------------------
//  Function: CacheArray::~CacheArray()
//
//  Arguments:
//            AUCUN
//  Description:
//            Destructeur.
//            Détruit chaque élément de la CacheArray.
//  Returns:
//
//---------------------------------------------------------------------------
void
CacheArray::vider()
{
	for (CacheElementArrayIter i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}

CacheArray::~CacheArray()
{
	vider();
}


//---------------------------------------------------------------------------
//  Function: int CacheArray::null(int i)
//
//  Arguments:
//            i -> indice dont on veut tester la nullité
//  Description:
//            Teste si l'objet placé à l'indice i est NULL
//  Returns:
//            0 si l'objet n'est pas le pointeur NULL
//            1 si il l'est
//---------------------------------------------------------------------------
/*int CacheArray::null(int i)
{
  CacheElement test = (*this)[i];
  return (test == NULL);
} */

