// fichier include contenant la classe NSBaseImages - RS Octobre 1997
#ifndef __NSBASIMG_H#define __NSBASIMG_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring.h>
#include <fstream.h>

#define MAXIMAGES 250
#define DOCIDLEN   13 // PAT_NSS_LEN + DOC_CODE_DOCUM_LEN

class NSBaseImages
{
	public:

  	string pathBase ;						// path base + nom de la base
    string tableDesImg[MAXIMAGES] ;
    string tableLocImg[MAXIMAGES] ;
    string tableImages[MAXIMAGES] ;
    string tableCompos[MAXIMAGES] ;
    string tableTypImg[MAXIMAGES] ;
    int    tableSelect[MAXIMAGES] ;
    int    nbImages ;

    NSBaseImages(string sPathBase) ;
    NSBaseImages(NSBaseImages& rv) ;
    ~NSBaseImages() {}

    void   reset() ;

    #ifdef __FLAT__
    bool	 ajouter(string nomImage, string pathDest, string pathSource, string nomSource, string type, int index = 0) ;
    bool	 copier(string pathDest) ;
    bool	 detruire() ;
    #endif

    void 	 select(int indexImg, int indexTag) ;
    bool   lire() ;
    bool	 ecrire() ;

    string getNewType(string sOldType) ;
    string getOldType(string sNewType) ;
};

NSBaseImages::NSBaseImages(string sPathBase){
	reset() ;

	pathBase = sPathBase ;
}

NSBaseImages::NSBaseImages(NSBaseImages& rv){
	for (int i = 0; i < MAXIMAGES; i++)
	{
    tableDesImg[i] = rv.tableDesImg[i] ;
    tableLocImg[i] = rv.tableLocImg[i] ;
    tableImages[i] = rv.tableImages[i] ;
    tableCompos[i] = rv.tableCompos[i] ;
    tableTypImg[i] = rv.tableTypImg[i] ;
    tableSelect[i] = rv.tableSelect[i] ;
	}
	nbImages = rv.nbImages ;
	pathBase = rv.pathBase ;
}

void
NSBaseImages::reset()
{
	for (int i = 0; i < MAXIMAGES; i++)
	{
    tableDesImg[i] = "" ;
    tableLocImg[i] = "" ;
    tableImages[i] = "" ;
    tableCompos[i] = "" ;
    tableTypImg[i] = "" ;
    tableSelect[i] = 0 ;
	}
  nbImages = 0 ;
}

#ifdef __FLAT__

bool
NSBaseImages::ajouter(string nomImage, string pathDest, string pathSource, string nomSource, string type, int index)
{
  tableLocImg[nbImages] = pathSource ;
  tableTypImg[nbImages] = type ;
  tableDesImg[nbImages] = pathDest ;

	// cas où l'image est déjà en place : on ne copie pas (on ne renomme pas)
  if (pathSource == pathDest)
  {
  	tableImages[nbImages] = nomSource ;
    tableCompos[nbImages] = string(nomImage, 0, DOCIDLEN) ;
	}
  else
  {
  	// on copie le fichier image sous son nouveau nom dans le repertoire du serveur
    string sSourceFile = pathSource + nomSource ;
    string sDestFile   = pathDest + nomImage ;

    if (!CopyFile(sSourceFile.c_str(), sDestFile.c_str(), false))
    	return false ;

    tableImages[nbImages] = nomImage ;
    tableCompos[nbImages] = string(nomImage, 0, DOCIDLEN) ;
	}

	tableSelect[nbImages] = index ;
	nbImages++ ;

	return true ;
}

boolNSBaseImages::copier(string pathDest)
{
	for (int i = 0; i < nbImages; i++)
	{
  	// on copie le fichier image sous son nouveau nom dans le nouveau repertoire
    if (!CopyFile((tableDesImg[i] + tableImages[i]).c_str(), (pathDest + tableImages[i]).c_str(), false))
    	return false ;
	}

	return true ;
}

boolNSBaseImages::detruire()
{
	bool bResult = true ;

	for (int i = 0; i < nbImages; i++)
	{
  	// on ne détruit dans le path dest que si la localisation est différente du path dest
    // (pour ne pas détruire l'image source)
    if (!(tableLocImg[i] == tableDesImg[i]))
    {
    	string fichDel = tableDesImg[i] + tableImages[i] ;

      if (!DeleteFile(fichDel.c_str()))
      	bResult = false ;
    }
	}

	// destruction du fichier base d'images
	if (!DeleteFile(pathBase.c_str()))
		bResult = false ;

	return bResult ;
}

#endif

void
NSBaseImages::select(int indexImg, int indexTag)
{
	for (int i = 0; i < nbImages; i++)
	{
  	if (tableSelect[i] == indexTag)
    {
    	tableSelect[i] = 0 ;
      break ;
    }
	}

	tableSelect[indexImg] = indexTag ;
}

boolNSBaseImages::lire()
{
  reset() ;

	ifstream inFile ;
	inFile.open(pathBase.c_str()) ;
	if (!inFile)
		return false ;

	string line ;
	string sBase = "" ;

	while (!inFile.eof())
	{
  	getline(inFile, line) ;
    if (line != "")
    	sBase += line + "\n" ;
	}
	inFile.close() ;

	size_t i = 0 ;
  size_t k = 0 ;
	// lecture de la string sBase

	while (i < sBase.length())
	{
  	while ((i < sBase.length()) && (sBase[i] != '|'))
    	tableDesImg[k] += sBase[i++] ;

    if (i < sBase.length())
    	i++ ;

    while ((i < sBase.length()) && (sBase[i] != '|'))
    	tableLocImg[k] += sBase[i++] ;

    if (i < sBase.length())
    	i++ ;

    while ((i < sBase.length()) && (sBase[i] != '|'))
    	tableImages[k] += sBase[i++] ;

    if (i < sBase.length())
    	i++ ;

    while ((i < sBase.length()) && (sBase[i] != '|'))
    	tableCompos[k] += sBase[i++] ;

    if (i < sBase.length())
    	i++ ;

    while ((i < sBase.length()) && (sBase[i] != '|'))
    	tableTypImg[k] += sBase[i++] ;

    tableTypImg[k] = getNewType(tableTypImg[k]) ;

    if (i < sBase.length())
    	i++ ;

    char   cIndex[4] ;
    size_t j = 0 ;
    while ((i < sBase.length()) && (sBase[i] != '\n'))
    	cIndex[j++] = sBase[i++] ;

    if (i < sBase.length())
    	i++ ;

    cIndex[j] = '\0' ;
    tableSelect[k] = atoi(cIndex) ;
    k++ ;
	}

	nbImages = k ;

	return true ;
}

boolNSBaseImages::ecrire()
{
	ofstream outFile ;
	string   sOutFile = "" ;
	char     cIndex[4] ;

	// on copie les images et leur selection dans le fichier de sortie
  for (int i = 0; i < nbImages; i++)
  {
    sOutFile += tableDesImg[i] ;
    sOutFile += string("|") ;
    sOutFile += tableLocImg[i] ;
    sOutFile += string("|") ;
    sOutFile += tableImages[i] ;
    sOutFile += string("|") ;
    sOutFile += tableCompos[i] ;
    sOutFile += string("|") ;

    sOutFile += getOldType(tableTypImg[i]) ;

    sOutFile += string("|") ;
    itoa(tableSelect[i],cIndex,10) ;
    sOutFile += string(cIndex) ;
    sOutFile += string("\n") ;
	}

	outFile.open(pathBase.c_str()) ;
	if (!outFile)
		return false ;

	for (size_t i = 0 ; i < sOutFile.length() ; i++)
  	outFile.put(sOutFile[i]) ;

	outFile.close() ;

	return true ;
}

string
NSBaseImages::getNewType(string sOldType)
{
	if      (string("IFART") == sOldType)
		return string("ZIART") ;
	else if (string("IAAVI") == sOldType)
		return string("ZIAVI") ;
	else if (string("IFBMP") == sOldType)
  	return string("ZIBMP") ;
	else if (string("IFGIF") == sOldType)
  	return string("ZIGIF") ;
	else if (string("HSHTM") == sOldType)
  	return string("ZSHTM") ;
	else if (string("IFJPG") == sOldType)
  	return string("ZIJPG") ;
	else if (string("IAMPG") == sOldType)
  	return string("ZIMPG") ;
	else if (string("HSMHT") == sOldType)
  	return string("ZSMHT") ;
	else if (string("IAMOV") == sOldType)
  	return string("ZIMOV") ;
	else if (string("TNRTF") == sOldType)
  	return string("ZTRTF") ;
	else if (string("TNTXT") == sOldType)
  	return string("ZTTXT") ;
	else if (string("IFXBM") == sOldType)
  	return string("ZIXBM") ;
	else
  	return string("") ;
}

string
NSBaseImages::getOldType(string sNewType)
{
	if      (string("ZIART") == sNewType)
		return string("IFART") ;
	else if (string("ZIAVI") == sNewType)
		return string("IAAVI") ;
	else if (string("ZIBMP") == sNewType)
  	return string("IFBMP") ;
	else if (string("ZIGIF") == sNewType)
  	return string("IFGIF") ;
	else if (string("ZSHTM") == sNewType)
  	return string("HSHTM") ;
	else if (string("ZIJPG") == sNewType)
  	return string("IFJPG") ;
	else if (string("ZIMPG") == sNewType)
  	return string("IAMPG") ;
	else if (string("ZSMHT") == sNewType)
  	return string("HSMHT") ;
	else if (string("ZIMOV") == sNewType)
  	return string("IAMOV") ;
	else if (string("ZTRTF") == sNewType)
  	return string("TNRTF") ;
	else if (string("ZTTXT") == sNewType)
  	return string("TNTXT") ;
	else if (string("ZIXBM") == sNewType)
  	return string("IFXBM") ;
	else
  	return string("") ;
}

#endif  // fin du fichier nsbasimg.h