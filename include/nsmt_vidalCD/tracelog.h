// -----------------------------------------------------------------------------
// fichier tracelog.h
// -----------------------------------------------------------------------------
// $Revision: 1.5 $
// $Author: fabrice $
// $Date: 2004/05/18 18:15:53 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------

#ifndef __TRACELOG_H__
# define __TRACELOG_H__

# define __DEBUG__ 1

# include <string.h>
# include <fstream.h>
# include <bde.hpp>

void
tracelog(const string sTrace, const string sLogFile)
{
	if (string("") == sTrace)
		return ;

	ofstream outFile ;
	outFile.open(sLogFile.c_str(), ios::app) ;
	if (!outFile)
		return ;

	outFile << sTrace << string("\n") ;

	outFile.close() ;
}

/*
void		erreur(string sMessage, int iGravite, DBIResult dbiError, HWND Hparent)
{
	char		logstring[1024] ;
	string	sFileLog = "" ;

	// ajouter l'heure
	sprintf(logstring, "%s : gravite %d - erreur %s", sMessage.c_str(), iGravite, dbiError) ;
}
*/


void    ConvertitMajuscule(char* chaine)
{
	for (size_t i = 0 ; i < strlen(chaine) ; i++)
	{
		switch (chaine[i])
		{
			case 'È' ://200
			case 'É' :
			case 'Ê' :
			case 'Ë' :
			case 'é' :
			case 'ë' :
			case 'è' :
			case 'ê' : chaine[i] =  'E' ;
								 break ;
			case 'À' :
			case 'Á' :
			case 'Â' :
			case 'Ã' :
			case 'Ä' :
			case 'Å' : //197
			case 'Æ' :
			case 'à' : //226
			case 'â' :
			case 'ã' :
			case 'ä' :
			case 'å' :
			case 'æ' :
			case 'á' : chaine[i] =  'A' ;
								 break ;
			case 'Ì' :
			case 'Í' :
			case 'Î' :
			case 'Ï' :
			case 'í' :
			case 'î' :
			case 'ï' : chaine[i] =  'I' ;
								 break ;
			case 'Ü' ://220
			case 'Û' :
			case 'Ú' :
			case 'Ù' : //216
			case 'ú' :
			case 'û' :
			case 'ü' :
			case 'ù' : chaine[i] =  'U' ;
								 break ;
			case 'Õ' : //213
			case 'Ö' :
			case 'Ô' :
			case 'Ó' :
			case 'Ò' :
			case 'ò' : //242
			case 'ó' :
			case 'ô' :
			case 'õ' :
			case 'ö' : chaine[i] = 'O' ;
								 break ;
			case 'ñ' :
			case 'Ñ' : chaine[i] = 'N' ; //209
								 break ;
			case 'Ý' :
			case 'ÿ' : chaine[i] = 'Y' ; //209
								 break ;
			case 'Ç' :
			case 'ç' : chaine[i] = 'C' ; //209
								 break ;
			case 'Ð' : chaine[i] = 'D' ; //209
								 break ;
		}
	}
	chaine = strupr(chaine) ;
}

#endif // __TRACELOG_H__
