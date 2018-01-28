//-------------------------------------------------------------------------
//                    BIBLIOTHEQUE : NSDIVFCT.H
//
//                     Pot-pourri de fonctions
//
//             Début d'écriture : 30 Juin 1988 (PA)
//             Dernière version : 28 Juillet 1994 (PA)
//-------------------------------------------------------------------------
#ifndef __NSDIVFCT_H
#define __NSDIVFCT_H

#if defined _OWLDLL
    #include <vector.h>
#else
    // #include <stl.h>
    #include <vector>
#endif

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

	enum STRIPTYPE { stripBoth = 0, stripLeft, stripRight } ;

	int 	 erreur(const char *texte, int gravite, int DatabaseError, string sHelp = "") ;

	string texteHtml(string texte) ;
	string texteCourant(string textHtml) ;
	int 	 donneDigit(char nbr) ;
	char	 donneChar(int digit) ;
	double StringToDouble(string sValeur, string *psError = 0) ;
	string DoubleToString(double* pDouble, int iEntier, int iDecimal) ;

	void	 strip(string& s, STRIPTYPE n, char c = ' ') ;

#endif 		// __NSUTILDLL_H

