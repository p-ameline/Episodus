/*
** Copyright Nautilus, (10/9/2004)
** fabrice.le_perru@nautilus-info.com
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/

#ifndef FOR_WINDOWS
#include "BB1List.h"
#include "BB1Exception.h" // Don't #include in BB1List.h since this #includes BB1String.h
#else
#include "ns_ob1\BB1List.h"
#include "ns_ob1\BB1Exception.h" // Don't #include in BB1List.h since this #includes BB1String.h
#endif

#ifdef __GNUC__
#pragma implementation
#endif

/*

// all contained in this section is obsolete
// this is replace by STL vector

// -----------------------------------------------------------------------------
// Null constructor (p. 259)
// -----------------------------------------------------------------------------

slink::slink()
{
    next = 0;
} // Won't link if in header file.


// -----------------------------------------------------------------------------
// Constructor with successor (p. 259)
// -----------------------------------------------------------------------------

slink::slink(slink* p)
{
    next = p;
}


// -----------------------------------------------------------------------------
// Null constructor; create an empty list (p. 266)
// -----------------------------------------------------------------------------

slist_base::slist_base()
{
    last = 0;
} // won't link if in header file


// -----------------------------------------------------------------------------
// Constructor with initial element (p. 266)
// -----------------------------------------------------------------------------

slist_base::slist_base(slink* a)
{
    last = a->next = a;
}


// -----------------------------------------------------------------------------
// Add to head of list (p. 266)
// -----------------------------------------------------------------------------

void slist_base::insert(slink* a)
{
    if (last)
        { a->next = last->next; }
    else
        { last = a; }
    last->next = a;
}


// -----------------------------------------------------------------------------
// Add to tail of list (p. 266)
// -----------------------------------------------------------------------------

void slist_base::append(slink* a)
{
    if (last)
    {
        // not the first item
        a->next = last->next;
        last = last->next = a;
    }
    else
        { last = a->next = a; } // the first item
}


// -----------------------------------------------------------------------------
// Return and remove head of list (p. 266)
// -----------------------------------------------------------------------------

slink* slist_base::get()
{
    slink* first = 0;
    try
    {
        if (last == 0)
        {
            // attempt to get from null list
            ListException le("slist_base::get()", "get from empty list");
            // throw le
        }
        else
        {
            // get from non-null list
            first = last->next;
            if (first == last)
                { last = 0; }
            else
                { last->next = first->next; }
        }
    } // end try
    catch (ListException &le)
        { }

    return (first);
}


Pix slist_base::first() const
{
    return (last == 0) ? 0 : Pix(last->next);
}


void slist_base::next(Pix& p) const
{
    p = (p == 0 || p == last) ? 0 : Pix(((slink*)(p))->next);
}


int slist_base::empty() const
{
    return (last == 0);
}


// -----------------------------------------------------------------------------
// Delete the item following the one pointed to by p
// -----------------------------------------------------------------------------

void slist_base::del_after(Pix p)
{
    try
    {
        slink* here = (slink*)p;
        if (last == 0 || here == last)
        {
            ListException le("slist_base::del_after(Pix p)", "cannot del_after last");
        }
        else
        {
            if (here == 0)
                here = last;       /- del_after(0) means delete first -/
            slink* after = here->next;
            if (here == after)
                { last = 0; }  /- singleton list -/
            else
            {
                here->next = after->next;
                if (last == after)
                    last = here;
            }
            delete after;
        }
    } // end try
    catch (ListException &le)
        { }
}
*/

template<class T> std::ostream& operator<<(std::ostream& os, const BB1List<T>& s)
{
  os << "( " ;
  for (Pix p = s.first() ; p != NULL ; s.next(p))
    { cout << *(s(p)) << " " ; }
  os << ")" ;
}
