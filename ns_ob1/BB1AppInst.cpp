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

#ifdef __GNUC__
# pragma implementation
#endif

#ifndef FOR_WINDOWS
# include "BB1AppInst.h"
#else
# include "ns_ob1\BB1AppInst.h"
# include "ns_ob1\BB1KB.h"
#endif

#ifdef __TCONTROLER__
BB1AppInst::BB1AppInst(const string sOName, AttValPair& dispatch, BB1KB& kb, BB1Class *c, /*const*/ AVPSet *pAttSpec, const LinkSet *pLinkSpec)
           :BB1Object(sOName, dispatch, kb, c)
{
  kb.BB().incNBObject() ;
//  attSpec->push_back(new  AttValPair(dispatch));
	ProcessAttributes(pAttSpec) ; // ignore Errcode
	ProcessLinksNN(pLinkSpec) ;   // ignore Errcode
}
#else
BB1AppInst::BB1AppInst(const string& oname, BB1KB& kb, BB1Class *c, const AVPSet *attSpec, const LinkSet *linkSpec)
	: BB1Object(oname, kb, c)
{
	ProcessAttributes(attSpec) ; // ignore Errcode
	ProcessLinksNN(linkSpec) ;   // ignore Errcode
}
#endif

BB1AppInst::BB1AppInst(const BB1AppInst& bbAppInst)
           :BB1Object(bbAppInst)
{
}

// -----------------------------------------------------------------------------
// Save a textual representation of the object to the ostream
// -----------------------------------------------------------------------------
void
BB1AppInst::Save(ofstream& KBstream)
{
  KBstream << "(" ;
  SaveClass(KBstream) ;
  SaveName(KBstream) ;
  SaveAttributes(KBstream) ;
  SaveLinks(KBstream) ;
  KBstream << ")\n" ;
}

