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
			case '�' ://200
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' : chaine[i] =  'E' ;
								 break ;
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' : //197
			case '�' :
			case '�' : //226
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' : chaine[i] =  'A' ;
								 break ;
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' : chaine[i] =  'I' ;
								 break ;
			case '�' ://220
			case '�' :
			case '�' :
			case '�' : //216
			case '�' :
			case '�' :
			case '�' :
			case '�' : chaine[i] =  'U' ;
								 break ;
			case '�' : //213
			case '�' :
			case '�' :
			case '�' :
			case '�' :
			case '�' : //242
			case '�' :
			case '�' :
			case '�' :
			case '�' : chaine[i] = 'O' ;
								 break ;
			case '�' :
			case '�' : chaine[i] = 'N' ; //209
								 break ;
			case '�' :
			case '�' : chaine[i] = 'Y' ; //209
								 break ;
			case '�' :
			case '�' : chaine[i] = 'C' ; //209
								 break ;
			case '�' : chaine[i] = 'D' ; //209
								 break ;
		}
	}
	chaine = strupr(chaine) ;
}

#endif // __TRACELOG_H__
