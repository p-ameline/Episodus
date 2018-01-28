/*
** Copyright Nautilus, (10/9/2004)
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
# include "BB1KS.h"
#else
# include "ns_ob1\BB1KS.h"
#endif

BB1KS::BB1KS(const string&							oname,
        BB1KB&													kb,
        BB1Class												*c,
        const FunctorPublication* 			tdef,
        const FunctorValidity*         	tVal,
				const FunctorPrecondition*			pc,
				const FunctorAction*						ac,
        const FunctorObviation*				  oc,
				const AVPSet*										attSpec)
      :BB1Object(oname, AttValPair("explication", std::string(oname)) , kb, c),
       _tPublication(tdef),
       _tValidity(tVal),
       _preCondition(pc),
       _action(ac),
       _obviationCondition(oc)
{
  ProcessAttributes(attSpec) ;

  _iPriority = 0 ;
}

BB1KS::BB1KS(const BB1KS& src)
  : BB1Object(src),
    _tPublication(src._tPublication),
    _tValidity(src._tValidity),
    _preCondition(src._preCondition),
    _action(src._action),
    _obviationCondition(src._obviationCondition),
    _iPriority(src._iPriority)
{
}

void BB1KS::Save(ofstream& /* KBstream */)
{
}

AKS::AKS(BB1KS *dpioKS, APConditionT dpioAPCond)
{
	ks					= dpioKS ;
	apCondition	= dpioAPCond ;
}

AKS::AKS(const AKS& src)
{
	ks					= src.ks ;
	apCondition	= src.apCondition ;
}

AKS::~AKS()
{
	ks					= NULL ;
	apCondition	= NULL ;
}

AKS&		AKS::operator=(const AKS& src)
{
  if (this == &src)
		return *this ;

	ks					= src.ks ;
	apCondition	= src.apCondition ;

	return *this ;
}

