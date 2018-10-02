//----------------------------------------------------------------------------// NSTXSTYL.CPP
// Styles utilisés dans le formatage de documents
//
// PA Septembre 1994
//----------------------------------------------------------------------------
#include <owl\dc.h>
#include <owl\gdiobjec.h>

#include "partage\nsdivfct.h"
#include "ns_sgbd\nsannexe.h"
#include "dcodeur\nstxstyl.h"
#include "dcodeur\nstxstar.h"

// -------------------------------------------------------------------------
// ------------------------- METHODES DE NSFont ----------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Description : Constructeur à partir des valeurs explicites
//---------------------------------------------------------------------------
NSFont::NSFont(const char far* facename,
					int height,
					int width,
					int escapement,
					int orientation,
					int weight,
					BYTE pitchAndFamily,
					BYTE italic,
					BYTE underline,
					BYTE strikeout,
					BYTE charSet,
					BYTE outputPrecision,
					BYTE clipPrecision,
					BYTE quality)
{
	if (facename != 0)
		strcpy(logFont.lfFaceName, facename);
	else
		strcpy(logFont.lfFaceName, "");
	logFont.lfHeight 			 = height;
	logFont.lfWidth  			 = width;
	logFont.lfEscapement 	 = escapement;
	logFont.lfOrientation 	 = orientation;
	logFont.lfWeight 			 = weight;
	logFont.lfPitchAndFamily = pitchAndFamily;
	logFont.lfItalic 			 = italic;
	logFont.lfUnderline 		 = underline;
	logFont.lfStrikeOut 		 = strikeout;
	logFont.lfCharSet 		 = charSet;
	logFont.lfOutPrecision   = outputPrecision;
	logFont.lfClipPrecision  = clipPrecision;
	logFont.lfQuality 		 = quality;}


//---------------------------------------------------------------------------
//  Description : Constructeur à partir d'une TFont
//---------------------------------------------------------------------------
NSFont::NSFont(TFont* rv)
{
	LOGFONT logTFont;
	//
	// Récupération de la structure LOGFONT de l'objet TFont
	//
	rv->GetObject(logTFont);
	//
	// Copie du LOGFONT dans logFont de l'objet NSFont
	//
	prendLogFont(&logTFont);
}

//---------------------------------------------------------------------------
//  Description : Constructeur à partir d'une structure LOGFONT
//---------------------------------------------------------------------------
NSFont::NSFont(LOGFONT* lf)
{
	prendLogFont(lf);
}

//---------------------------------------------------------------------------
//  Description : Constructeur à partir d'un DC
//						(copie la police en cours)
//---------------------------------------------------------------------------
NSFont::NSFont(TDC* pdc)
{
	TEXTMETRIC tm;
	char		  szNomPolice[LF_FACESIZE];
	//
	// Prise de la description de la police en cours
	//
	int j = pdc->GetTextFace(sizeof szNomPolice, szNomPolice);
	if(pdc->GetTextFace(sizeof szNomPolice, szNomPolice) > 0)
	{
     if( pdc->GetTextMetrics(tm))
     {
			strcpy(logFont.lfFaceName, szNomPolice);
			logFont.lfHeight 			 = tm.tmHeight;
			logFont.lfWidth  			 = tm.tmAveCharWidth;
			logFont.lfEscapement 	 = 0;
			logFont.lfOrientation 	 = 0;
			logFont.lfWeight 			 = tm.tmWeight;
			logFont.lfPitchAndFamily = tm.tmPitchAndFamily;
			logFont.lfItalic 			 = tm.tmItalic;
			logFont.lfUnderline 		 = tm.tmUnderlined;
			logFont.lfStrikeOut 		 = tm.tmStruckOut;
			logFont.lfCharSet 		 = tm.tmCharSet;
			logFont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
			logFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
			logFont.lfQuality 		 = DEFAULT_QUALITY;
    }
	}
}

//---------------------------------------------------------------------------
//  Description : Constructeur copie
//---------------------------------------------------------------------------
NSFont::NSFont(const NSFont& rv)
{
	prendLogFont(&(rv.logFont));
}

//---------------------------------------------------------------------------//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSFont&
NSFont::operator=(const NSFont& src)
{
  if (this == &src)
		return *this ;

	prendLogFont(&(src.logFont)) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Description : Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFont::operator == ( const NSFont& o )
{
  if ((strcmp(logFont.lfFaceName, o.logFont.lfFaceName) == 0)  &&
      (logFont.lfHeight 			 == o.logFont.lfHeight) 		  &&
      (logFont.lfWidth  			 == o.logFont.lfWidth)  		  &&
      (logFont.lfEscapement 	 == o.logFont.lfEscapement) 	  &&
      (logFont.lfOrientation 	 == o.logFont.lfOrientation) 	  &&
      (logFont.lfWeight 			 == o.logFont.lfWeight)			  &&
      (logFont.lfPitchAndFamily == o.logFont.lfPitchAndFamily) &&
      (logFont.lfItalic 			 == o.logFont.lfItalic)			  &&
      (logFont.lfUnderline 		 == o.logFont.lfUnderline)		  &&
      (logFont.lfStrikeOut 		 == o.logFont.lfStrikeOut)		  &&
      (logFont.lfCharSet 		 == o.logFont.lfCharSet)		  &&
      (logFont.lfOutPrecision 	 == o.logFont.lfOutPrecision)   &&
      (logFont.lfClipPrecision  == o.logFont.lfClipPrecision)  &&
      (logFont.lfQuality 		 == o.logFont.lfQuality))
    return 1 ;
    
  return 0 ;
}


//---------------------------------------------------------------------------
//  Description : Destructeur
//---------------------------------------------------------------------------
NSFont::~NSFont()
{
}

//---------------------------------------------------------------------------
//  Fonction	 : NSFont::prendLogFont(LOGFONT& lf)
//
//  Description : Initialise logFont à partir d'une LOFONT existante
//---------------------------------------------------------------------------
void
NSFont::prendLogFont(const LOGFONT* lf)
{
	strcpy(logFont.lfFaceName, lf->lfFaceName);
	logFont.lfHeight 			 = lf->lfHeight;
	logFont.lfWidth  			 = lf->lfWidth;
	logFont.lfEscapement 	 = lf->lfEscapement;
	logFont.lfOrientation 	 = lf->lfOrientation;
	logFont.lfWeight 			 = lf->lfWeight;
	logFont.lfPitchAndFamily = lf->lfPitchAndFamily;
	logFont.lfItalic 			 = lf->lfItalic;
	logFont.lfUnderline 		 = lf->lfUnderline;
	logFont.lfStrikeOut 		 = lf->lfStrikeOut;
	logFont.lfCharSet 		 = lf->lfCharSet;
	logFont.lfOutPrecision 	 = lf->lfOutPrecision;
	logFont.lfClipPrecision  = lf->lfClipPrecision;
	logFont.lfQuality 		 = lf->lfQuality;
}

// -------------------------------------------------------------------------
// ---------------------- METHODES DE NSStyleCadre -------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Description : Constructeur par défaut
//---------------------------------------------------------------------------
NSStyleCadre::NSStyleCadre()
{
	//
	// Initialisation des marges
	//
	MargeGauche = 0;
	MargeDroite = 0;
	MargeHaute  = 0;
	MargeBasse  = 0;
	//
	// Initialisation des bordures
	//
	pBordGauche = 0;
	pBordDroit  = 0;
	pBordHaut   = 0;
	pBordBas    = 0;
	pTousBords  = 0;
	RayonCoinsRonds = 0;
	//
	// Initialisation des couleurs à "blanc" pour le fond et "noir" au bord
	//
	/*pCouleurFond 	  = new TColor(255, 255, 255);
	pCouleurBordure  = new TColor(0, 0, 0); */
	ColorierJusqua   = 0;
	CadreTransparent = FALSE;
	HabillageTexte   = TRUE;
	Titre = "";
}

//---------------------------------------------------------------------------
//  Description : Destructeur
//---------------------------------------------------------------------------
NSStyleCadre::~NSStyleCadre()
{
	//
	// Destruction des bordures
	//
	if (pBordGauche != 0) delete pBordGauche;
	if (pBordDroit  != 0) delete pBordDroit;
	if (pBordHaut   != 0) delete pBordHaut;
	if (pBordBas    != 0) delete pBordBas;
	if (pTousBords  != 0) delete pTousBords;
	//
	// Destruction des couleurs
	//
	/*delete pCouleurFond;
	delete pCouleurBordure; */
}

//---------------------------------------------------------------------------//  Description : Constructeur copie
//---------------------------------------------------------------------------
NSStyleCadre::NSStyleCadre(const NSStyleCadre& rv)
{
try
{
	Titre 		= rv.Titre;

	MargeGauche = rv.MargeGauche;
	MargeDroite = rv.MargeDroite;
	MargeHaute  = rv.MargeHaute;
	MargeBasse  = rv.MargeBasse;

	//	// Initialisation des bordures
	//
	if (rv.pBordGauche == 0)
		pBordGauche = 0;
	else
	{
		pBordGauche = new NSStyleBordure();
		*(pBordGauche) = *(rv.pBordGauche);
	}
	if (rv.pBordDroit == 0)
		pBordDroit  = 0;
	else
	{
		pBordDroit = new NSStyleBordure();
		*(pBordDroit) = *(rv.pBordDroit);
	}
	if (rv.pBordHaut == 0)
		pBordHaut   = 0;
	else
	{
		pBordHaut = new NSStyleBordure();
		*(pBordHaut) = *(rv.pBordHaut);
	}

	if (rv.pBordBas == 0)		pBordBas    = 0;
	else
	{
		pBordBas = new NSStyleBordure();
		*(pBordBas) = *(rv.pBordBas);
	}

	if (rv.pTousBords == 0)		pTousBords  = 0;
	else
	{
		pTousBords = new NSStyleBordure();
		*(pTousBords) = *(rv.pTousBords);
	}

	RayonCoinsRonds = rv.RayonCoinsRonds;
	//
	// Initialisation des couleurs
	//
	/*pCouleurFond 	  	 = new TColor(rv.pCouleurFond->Red(),
											  rv.pCouleurFond->Green(),
											  rv.pCouleurFond->Blue());
	pCouleurBordure  	 = new TColor(rv.pCouleurBordure->Red(),
											  rv.pCouleurBordure->Green(),
											  rv.pCouleurBordure->Blue()); */

	ColorierJusqua   	 = rv.ColorierJusqua;	CadreTransparent 	 = rv.CadreTransparent;
	HabillageTexte   	 = rv.HabillageTexte;
}
catch (...)
{
	erreur("Exception NSStyleCadre copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSStyleCadre&
NSStyleCadre::operator=(const NSStyleCadre& src)
{
  if (this == &src)
		return *this ;

	Titre			= src.Titre;

	MargeGauche = src.MargeGauche;
	MargeDroite = src.MargeDroite;
	MargeHaute  = src.MargeHaute;
	MargeBasse  = src.MargeBasse;

	//	// Initialisation des bordures	//
	if (src.pBordGauche == 0)
		pBordGauche = 0;
	else
	{
		pBordGauche = new NSStyleBordure();
		*(pBordGauche) = *(src.pBordGauche);
	}
	if (src.pBordDroit == 0)
		pBordDroit  = 0;
	else
	{
		pBordDroit = new NSStyleBordure();
		*(pBordDroit) = *(src.pBordDroit);
	}
	if (src.pBordHaut == 0)		pBordHaut   = 0;
	else
	{
		pBordHaut = new NSStyleBordure();
		*(pBordHaut) = *(src.pBordHaut);
	}
	if (src.pBordBas == 0)
		pBordBas    = 0;	else
	{
		pBordBas = new NSStyleBordure();
		*(pBordBas) = *(src.pBordBas);
	}
	if (src.pTousBords == 0)
		pTousBords  = 0;
  else
	{
		pTousBords = new NSStyleBordure();
		*(pTousBords) = *(src.pTousBords);
	}
	RayonCoinsRonds = src.RayonCoinsRonds;
	//
	// Initialisation des couleurs
	//
	/*pCouleurFond 	  	 = new TColor(src.pCouleurFond->Red(),
											  src.pCouleurFond->Green(),
											  src.pCouleurFond->Blue());
	pCouleurBordure  	 = new TColor(src.pCouleurBordure->Red(),
											  src.pCouleurBordure->Green(),
											  src.pCouleurBordure->Blue()); */
	ColorierJusqua   	 = src.ColorierJusqua;
	CadreTransparent 	 = src.CadreTransparent;
	HabillageTexte   	 = src.HabillageTexte;
	return *this;
}

//---------------------------------------------------------------------------
//  Description : Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSStyleCadre::operator == ( const NSStyleCadre& o )
{
	 if ((Titre			== o.Titre)			&&
		  (MargeGauche == o.MargeGauche) &&
		  (MargeDroite == o.MargeDroite) &&
		  (MargeHaute  == o.MargeHaute)	&&
		  (MargeBasse  == o.MargeBasse) 	&&
		  (((pBordGauche == 0) && (o.pBordGauche == 0)) || (*(pBordGauche) == *(o.pBordGauche))) &&
		  (((pBordDroit  == 0) && (o.pBordDroit  == 0)) || (*(pBordDroit)  == *(o.pBordDroit)))  &&
		  (((pBordHaut   == 0) && (o.pBordHaut   == 0)) || (*(pBordHaut) 	 == *(o.pBordHaut)))   &&
		  (((pBordBas 	  == 0) && (o.pBordBas 	  == 0)) || (*(pBordBas) 	 == *(o.pBordBas))) 	  &&
		  (((pTousBords  == 0) && (o.pTousBords  == 0)) || (*(pTousBords)  == *(o.pTousBords)))  &&
		  (RayonCoinsRonds 	 == o.RayonCoinsRonds) 	  &&
		  (ColorierJusqua   	 == o.ColorierJusqua)	  &&
		  (CadreTransparent 	 == o.CadreTransparent)   &&
		  (HabillageTexte   	 == o.HabillageTexte))
		  return 1;
	 else
		  return 0;
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSStyleParagraphe ----------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Description : Constructeur par défaut
//---------------------------------------------------------------------------
NSStyleParagraphe::NSStyleParagraphe()
{
	Reinitialiser();
}

//---------------------------------------------------------------------------
//  Description : Destructeur
//---------------------------------------------------------------------------
NSStyleParagraphe::~NSStyleParagraphe()
{
	//
	// Destruction des pointeurs
	//
}

//---------------------------------------------------------------------------
//  Description : Constructeur copie
//---------------------------------------------------------------------------
NSStyleParagraphe::NSStyleParagraphe(const NSStyleParagraphe& rv)
{
	Titre 	  		  = rv.Titre;
	NumPolice 	  	  = rv.NumPolice;
	HauteurLigne 	  = rv.HauteurLigne;
	RetraitGauche 	  = rv.RetraitGauche;
	RetraitDroit  	  = rv.RetraitDroit;
	RetraitPremLigne = rv.RetraitPremLigne;
	EspaceDessus	  = rv.EspaceDessus;
	EspaceDessous	  = rv.EspaceDessous;
	Alignement		  = rv.Alignement;
	for (int i = 0; i < NBRE_TABULATIONS; i++)
		Tabulations[i] = rv.Tabulations[i];
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSStyleParagraphe&
NSStyleParagraphe::operator=(const NSStyleParagraphe& src)
{
  if (this == &src)
		return *this ;

	Titre 	  		  = src.Titre;
	NumPolice 	  	  = src.NumPolice;
	HauteurLigne 	  = src.HauteurLigne;
	RetraitGauche 	  = src.RetraitGauche;
	RetraitDroit  	  = src.RetraitDroit;
	RetraitPremLigne = src.RetraitPremLigne;
	EspaceDessus	  = src.EspaceDessus;
	EspaceDessous	  = src.EspaceDessous;
	Alignement		  = src.Alignement;
	for (int i = 0; i < NBRE_TABULATIONS; i++)
		Tabulations[i] = src.Tabulations[i];
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSStyleParagraphe::operator == ( const NSStyleParagraphe& o )
{
	 if ((strcmp(Titre.c_str(),o.Titre.c_str()) != 0 ) ||
		  (NumPolice		  != o.NumPolice)			 		||
		  (HauteurLigne 	  != o.HauteurLigne)		 		||
		  (RetraitGauche 	  != o.RetraitGauche)	 		||
		  (RetraitDroit  	  != o.RetraitDroit)		 		||
		  (RetraitPremLigne != o.RetraitPremLigne) 		||
		  (EspaceDessus	  != o.EspaceDessus)		 		||
		  (EspaceDessous	  != o.EspaceDessous)	 		||
		  (Alignement		  != o.Alignement))
		 return 0;
	 for (int i = 0; i < NBRE_TABULATIONS; i++)
		  if (Tabulations[i] != o.Tabulations[i])
				return 0;
	 return 1;

}

//---------------------------------------------------------------------------
//  Fonction	 : NSStyleParagraphe::initPolice(string* chaineInit)
//
//  Description : récupère le numéro XX de la police à utiliser à partir
//						d'une chaine de la forme FontXX
//---------------------------------------------------------------------------
void
NSStyleParagraphe::Reinitialiser()
{
	Titre = "";
	//
	// Initialisation de la police (police par défaut = numéro 1)
	//
	NumPolice 	 = 1;
	HauteurLigne = 0;
	//
	// Initialisation des retraits
	//
	RetraitGauche 	  = 0;
	RetraitDroit  	  = 0;
	RetraitPremLigne = 0;
	EspaceDessus	  = 0;
	EspaceDessous	  = 0;
	Alignement		  = TA_LEFT | TA_BASELINE;
	for (int i = 0; i < NBRE_TABULATIONS; i++)
		Tabulations[i] = 0;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSStyleParagraphe::initPolice(string* chaineInit)
//
//  Description : récupère le numéro XX de la police à utiliser à partir
//						d'une chaine de la forme FontXX
//---------------------------------------------------------------------------
void
NSStyleParagraphe::InitPolice(string* chaineInit)
{
    if (strlen(chaineInit->c_str()) < 4)
        return;

 	UINT i;
	if (string(*chaineInit, 0, 4) == "Font")
	{
		if ((strlen(chaineInit->c_str()) > 4) && (isdigit((*chaineInit)[4])))
		{
			i = UINT((*chaineInit)[4]) - UINT('0');
			if ((strlen(chaineInit->c_str()) > 5) && (isdigit((*chaineInit)[5])))
			{
				i = 10 * i;
				i += UINT((*chaineInit)[5]) - UINT('0');
			}
		}
		else i = 1;
		NumPolice = i;
	}

}

//---------------------------------------------------------------------------
//  Fonction	 : NSStyleParagraphe::InitRetrait(string* chaineInit)
//
//  Description : récupère les valeurs des retraits (1ère, gche, dte)
//						à partir d'une chaine de la forme XXX,XXX,XXX
//---------------------------------------------------------------------------
void
NSStyleParagraphe::InitRetrait(string* chaineInit)
{
  	UINT i, j, k, indice, longueur;
	longueur = strlen(chaineInit->c_str());
	//
	// Prise de la première valeur : Retrait gauche
	//
	for (j = 0, indice = 1; indice < 4; indice++, j++)
	{
		//
		// Prise d'une valeur
		//
		for (i = 0, k = 0; (j < longueur) && (isdigit((*chaineInit)[j])) && (k < 3); j++, k++)
		{
			i = 10 * i;
			i += UINT((*chaineInit)[j]) - UINT('0');
		}
		//
		// Test de "propreté" de la chaine d'initialisation
		//
		if ((indice < 3) && (j < longueur) && ((*chaineInit)[j] != ',')) return;
		if ((indice == 3) && (j < longueur)) return;
		//
		// Mise de la valeur dans sa variable
		//
		switch (indice) {
			case 1 : RetraitPremLigne = i; break;
			case 2 : RetraitGauche 	  = i; break;
			case 3 : RetraitDroit  	  = i; break;
		}
	}
}

//---------------------------------------------------------------------------
//  Fonction	 : NSStyleParagraphe::InitEspacement(string* chaineInit)
//
//  Description : récupère les valeurs des espaces (dessus, dessous) à partir
//						d'une chaine de la forme XXX,XXX
//---------------------------------------------------------------------------
void
NSStyleParagraphe::InitEspacement(string* chaineInit)
{
  	UINT i, j, k, indice, longueur;
	longueur = strlen(chaineInit->c_str());
	//
	// Prise de la première valeur : Retrait gauche
	//
	for (j = 0, indice = 1; indice < 3; indice++, j++)
	{
		//
		// Prise d'une valeur
		//
		for (i = 0, k = 0; (j < longueur) && (isdigit((*chaineInit)[j])) && (k < 3); j++, k++)
		{
			i = 10 * i;
			i += UINT((*chaineInit)[j]) - UINT('0');
		}
		//
		// Test de "propreté" de la chaine d'initialisation
		//
		if ((indice < 2) && (j < longueur) && ((*chaineInit)[j] != ',')) return;
		if ((indice == 2) && (j < longueur)) return;
		//
		// Mise de la valeur dans sa variable
		//
		switch (indice) {
			case 1 : EspaceDessus  = i; break;
			case 2 : EspaceDessous = i; break;
		}
	}
}

//---------------------------------------------------------------------------
//  Fonction	 : NSStyleParagraphe::InitAlignement(string* chaineInit)
//
//  Description : récupère le numéro XX de la police à utiliser à partir
//						d'une chaine de la forme FontXX
//---------------------------------------------------------------------------
void
NSStyleParagraphe::InitAlignement(string* chaineInit)
{
	if 	  (((*chaineInit) == "0") 		 ||
				((*chaineInit) == "gauche") ||
				((*chaineInit) == "left"))
		Alignement = TA_LEFT;
	else if (((*chaineInit) == "1") 		 ||
				((*chaineInit) == "droit")  ||
				((*chaineInit) == "droite") ||
				((*chaineInit) == "right"))
		Alignement = TA_RIGHT;
	else if (((*chaineInit) == "2") 		 ||
				((*chaineInit) == "centré") ||
				((*chaineInit) == "centre") ||
				((*chaineInit) == "center"))
		Alignement = TA_CENTER;
	else if (((*chaineInit) == "3") 		 	||
				((*chaineInit) == "justifié") ||
				((*chaineInit) == "justif") 	||
				((*chaineInit) == "justified"))
		Alignement = TA_JUSTIFIED;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSStyleParagraphe::InitTabulations(string* chaineInit)
//
//  Description : récupère les valeurs des espaces (dessus, dessous) à partir
//						d'une chaine de la forme XXX,XXX
//---------------------------------------------------------------------------
void
NSStyleParagraphe::InitTabulations(string* chaineInit)
{
  	UINT i, j, k, indice, longueur;
	longueur = strlen(chaineInit->c_str());
	//
	// Prise de la première valeur : Retrait gauche
	//
	for (j = 0, indice = 0; indice < NBRE_TABULATIONS; indice++, j++)
	{
		//
		// Prise d'une valeur
		//
		for (i = 0, k = 0; (j < longueur) && (isdigit((*chaineInit)[j])) && (k < 3); j++, k++)
		{
			i = 10 * i;
			i += UINT((*chaineInit)[j]) - UINT('0');
		}
		//
		// Test de "propreté" de la chaine d'initialisation
		//
		if ((indice < NBRE_TABULATIONS-1) && (j < longueur) && ((*chaineInit)[j] != ',')) return;
		if ((indice == NBRE_TABULATIONS-1) && (j < longueur)) return;
		//
		// Mise de la valeur dans sa variable
		//
		Tabulations[indice] = i;
	}
}

/*	 InitFilets(string* chaineInit);
	 InitCouleurs(string* chaineInit);
	 InitPuce(string* chaineInit);
	 InitLettrine(string* chaineInit);
	 InitNumerotation(string* chaineInit); */

// -------------------------------------------------------------------------
// --------------------- METHODES DE NSStyleBordure ------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Description : Constructeur par défaut
//---------------------------------------------------------------------------
NSStyleBordure::NSStyleBordure()
{
	//
	// Initialisation
	//
	DistanceDuBord = 0;
	EpaisseurLigneExt = 0;
	EspaceEntreLignes = 0;
	EpaisseurLigneInt = 0;
}

//---------------------------------------------------------------------------
//  Description : Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSStyleBordure::operator == ( const NSStyleBordure& o )
{
	 if ((DistanceDuBord 	== o.DistanceDuBord)		&&
		  (EpaisseurLigneExt == o.EpaisseurLigneExt) &&
		  (EspaceEntreLignes == o.EspaceEntreLignes) &&
		  (EpaisseurLigneInt == o.EpaisseurLigneInt))
		  return 1;
	 else
		  return 0;
}

// -------------------------------------------------------------------------
// ---------------------- METHODES DE NSStyleDecor -------------------------
// -------------------------------------------------------------------------
// Objet qui représente un style de décor (ligne de titre...)

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSStyleDecor::NSStyleDecor()
{
	Texte = "";
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSStyleDecor::NSStyleDecor(const NSStyleDecor& rv)
{
	Texte 			 = rv.Texte;
   styleParagraphe = rv.styleParagraphe;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSStyleDecor::~NSStyleDecor()
{}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSStyleDecor&
NSStyleDecor::operator=(const NSStyleDecor& src)
{
  if (this == &src)
		return *this ;

  Texte 			    = src.Texte ;  styleParagraphe = src.styleParagraphe ;

	return *this ;}
//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSStyleDecor::operator == ( const NSStyleDecor& o )
{
	 if ((Texte 			 == o.Texte)			  &&
		  (styleParagraphe == o.styleParagraphe))
		  return 1;
	 else
		  return 0;
}

// -------------------------------------------------------------------------
// ----------------------- METHODES DE NSCadreDecor ------------------------
// -------------------------------------------------------------------------
// Objet qui représente un cadre avec son contenu

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSCadreDecor::NSCadreDecor()
{
	Titre = "";
    tailleHorizontale = tailleVerticale = 0;
    pStyleDecor = 0;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCadreDecor::NSCadreDecor(const NSCadreDecor& rv)
{
	Titre 			  = rv.Titre;
    tailleHorizontale = rv.tailleHorizontale;
    tailleVerticale	  = rv.tailleVerticale;
    DecorArray		  = rv.DecorArray;
    pStyleDecor = 0;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCadreDecor::~NSCadreDecor()
{
    if (pStyleDecor != 0)
   	    delete pStyleDecor;
    pStyleDecor = 0 ;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::InitTitre(string* sTitre)
//
//  Description : récupère le titre du décor
//---------------------------------------------------------------------------
void
NSCadreDecor::InitTitre(string* sTitre)
{
    // Il faut enlever le '[' initial et le ']' final
    //
    int taille = strlen(sTitre->c_str());
	Titre = string(*sTitre, 1, taille-2);
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::InitTaille(string* chaineInit)
//
//  Description : récupère les valeurs de dimension (horiz, vert) à partir
//						d'une chaine de la forme XXXXX,XXXXX
//---------------------------------------------------------------------------
void
NSCadreDecor::InitTaille(string* chaineInit)
{
	UINT j, k, indice, longueur, nbMax, nbVal;
    long i;
	longueur = strlen(chaineInit->c_str());
    //
    // On fixe à 5 la taille maximale de chaque valeur
    //
    nbMax = 5;
    //
    // On fixe à 2 le nombre de valeurs
    //
    nbVal = 2;
	//
	// Prise de la première valeur : Retrait gauche
	//
	for (j = 0, indice = 1; indice <= nbVal; indice++, j++)
	{
		//
		// Prise d'une valeur
		//
        i = 0;
		for (k = 0; (j < longueur) && (isdigit((*chaineInit)[j])) && (k < nbMax); j++, k++)
		{
			i = long(10) * i;
			i += long((*chaineInit)[j]) - long('0');
		}
		//
		// Test de "propreté" de la chaine d'initialisation
		//
		if ((indice < nbVal) && (j < longueur) && ((*chaineInit)[j] != ',')) return;
		if ((indice == nbVal) && (j < longueur)) return;
		//
		// Mise de la valeur dans sa variable
		//
		switch (indice) {
			case 1 : tailleHorizontale = i; break;
			case 2 : tailleVerticale 	= i; break;
		}
	}
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::debutTexte()
//
//  Description : début d'initialisation de pStyleDecor, qui sera stocké
//						ensuite dans pDecorArray (à la réception de finTexte())
//---------------------------------------------------------------------------
void
NSCadreDecor::debutTexte()
{
    if (pStyleDecor != 0)
   	    delete pStyleDecor;
    pStyleDecor = new NSStyleDecor;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::finTexte()
//
//  Description : Fin d'initialisation de pStyleDecor, qu'on stocke
//						dans pDecorArray
//---------------------------------------------------------------------------
void
NSCadreDecor::finTexte()
{
	if (pStyleDecor == 0)
   	    return;
    DecorArray.push_back(new NSStyleDecor(*pStyleDecor));
    delete pStyleDecor;
    pStyleDecor = 0;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::InitTexte(string* chaineInit)
//
//  Description : Initialisation de la variable Texte de pStyleDecor
//---------------------------------------------------------------------------
void
NSCadreDecor::InitTexte(string* chaineInit)
{
	if (pStyleDecor == 0)
   	    return;
    pStyleDecor->Texte = *chaineInit;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::InitPolice(string* chaineInit)
//
//  Description : Initialisation de la police de pStyleDecor
//---------------------------------------------------------------------------
void
NSCadreDecor::InitPolice(string* chaineInit)
{
	if (pStyleDecor == 0)
   	    return;
    pStyleDecor->styleParagraphe.InitPolice(chaineInit);
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::InitRetrait(string* chaineInit)
//
//  Description : Initialisation des retraits de pStyleDecor
//---------------------------------------------------------------------------
void
NSCadreDecor::InitRetrait(string* chaineInit)
{
	if (pStyleDecor == 0)
   	    return;
    pStyleDecor->styleParagraphe.InitPolice(chaineInit);
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::InitEspacement(string* chaineInit)
//
//  Description : Initialisation des espacements de pStyleDecor
//---------------------------------------------------------------------------
void
NSCadreDecor::InitEspacement(string* chaineInit)
{
    if (pStyleDecor == 0)
   	    return;
    pStyleDecor->styleParagraphe.InitEspacement(chaineInit);
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::InitAlignement(string* chaineInit)
//
//  Description : Initialisation de l'alignement de pStyleDecor
//---------------------------------------------------------------------------
void
NSCadreDecor::InitAlignement(string* chaineInit)
{
	if (pStyleDecor == 0)
   	    return;
    pStyleDecor->styleParagraphe.InitAlignement(chaineInit);
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCadreDecor::InitTabulations(string* chaineInit)
//
//  Description : Initialisation des tabulations de pStyleDecor
//---------------------------------------------------------------------------
void
NSCadreDecor::InitTabulations(string* chaineInit)
{
	if (pStyleDecor == 0)
   	    return;
    pStyleDecor->styleParagraphe.InitTabulations(chaineInit);
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCadreDecor&
NSCadreDecor::operator=(const NSCadreDecor& src)
{
  if (this == &src)
		return *this ;

	Titre 			  = src.Titre ;
  tailleHorizontale = src.tailleHorizontale ;  tailleVerticale	  = src.tailleVerticale ;

  DecorArray		  = src.DecorArray ;

  return *this ;}


//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCadreDecor::operator == ( const NSCadreDecor& o )
{
    if ((Titre 			   == o.Titre)			  	&&
        (tailleHorizontale == o.tailleHorizontale)	&&
        (tailleVerticale   == o.tailleVerticale)	&&
        (DecorArray 	   == o.DecorArray))
        return 1;
    else
        return 0;
}

// -------------------------------------------------------------------------
// -------------------------- METHODES DE NSDecor --------------------------
// -------------------------------------------------------------------------
// Objet qui représente un cadre positionné dans la page

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSDecor::NSDecor()
{
	Titre 		  = "";
    titreCadre	  = "";
    AlignementX   = AlignementY   = TA_JUSTIFIED;
    coinHautGcheX = coinHautGcheY = 0;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDecor::NSDecor(const NSDecor& rv)
{
	Titre 		  = rv.Titre;
   titreCadre	  = rv.titreCadre;
   AlignementX   = rv.AlignementX;
   AlignementY   = rv.AlignementY;
   coinHautGcheX = rv.coinHautGcheX;
   coinHautGcheY = rv.coinHautGcheY;
}

NSDecor::~NSDecor()
{}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSDecor&
NSDecor::operator=(const NSDecor& rv)
{
  if (this == &rv)
		return *this ;

	Titre 		  = rv.Titre;
   titreCadre	  = rv.titreCadre;
   AlignementX   = rv.AlignementX;
   AlignementY   = rv.AlignementY;
   coinHautGcheX = rv.coinHautGcheX;
   coinHautGcheY = rv.coinHautGcheY;
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSDecor::operator == ( const NSDecor& o )
{
	 if ((Titre 		 == o.Titre)			&&
		  (titreCadre	 == o.titreCadre))
		  return 1;
	 else
		  return 0;
}

// -------------------------------------------------------------------------
// ------------------------ METHODES DES NSDecorArray ----------------------
// -------------------------------------------------------------------------

//--------------------------------------------------------------------------
//  Constructeur copie
//--------------------------------------------------------------------------
/*
NSDecorArray::NSDecorArray(NSDecorArray& rv)
				 :NSNSDecoArray()
{

    if (!(rv.empty()))
        for (NSDecoArrayIter i = rv.begin(); i != rv.end(); i++)
      	    push_back(new NSDecor(*(*i)));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSDecorArray::~NSDecorArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSDecorArray&
NSDecorArray::operator=(NSDecorArray src)
{
	vider();
  	if (!(src.empty()))
		for (NSDecoArrayIter i = src.begin(); i != src.end(); i++)
      	push_back(new NSDecor(*(*i)));
	return *this;
}

NSDecorArray::copier(NSDecorArray* src)
{
	vider();
  	if (!(src->empty()))
		for (NSDecoArrayIter i = src->begin(); i != src->end(); i++)
      	push_back(new NSDecor(*(*i)));
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSDecorArray::operator==(NSDecorArray& o)
{
   NSDecoArrayIter i,j;
	if (!(o.empty()))
   {
      if( (o.size() == size()) )
      {
   		for (i = o.begin(),j = begin(); (j != end()) && (i != o.end()); i++,j++)
         {
            if (*(*j) != *(*i))
            	return 0;
         }
      }
      return 1;
   }
   if (empty())
   	return 1;
   else
   	return 0;
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSDecorArray::vider()
{
	for (NSDecoArrayIter i = begin(); i != end(); )
   {
   	delete *i;
   	erase(i);
   }
}
 */
// -------------------------------------------------------------------------
// -------------------------- METHODES DE NSPage ---------------------------
// -------------------------------------------------------------------------
// Objet qui représente une page

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSPage::NSPage()
{
	Titre 	  = "";
   numeroPage = 0;

	MargeGauche = MargeDroite = MargeHaute = MargeBasse = 0;
   MargeTxtGch = MargeTxtDte = MargeTxtHaut = MargeTxtBas = 0;
   // CouleurFond, CouleurTexte, CouleurBordure;
   pBordure = 0;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPage::NSPage(const NSPage& rv)
{
	Titre 			= rv.Titre;
   numeroPage 		= rv.numeroPage;

   MargeGauche 	= rv.MargeGauche;
   MargeDroite 	= rv.MargeDroite;
   MargeHaute 		= rv.MargeHaute;
   MargeBasse 		= rv.MargeBasse;

   MargeTxtGch		= rv.MargeTxtGch;
   MargeTxtDte		= rv.MargeTxtDte;
   MargeTxtHaut	= rv.MargeTxtHaut;
   MargeTxtBas		= rv.MargeTxtBas;

   CouleurFond 	= rv.CouleurFond;
   CouleurTexte	= rv.CouleurTexte;
   CouleurBordure = rv.CouleurBordure;

   if (rv.pBordure == 0)
		pBordure = 0;
	else
		pBordure = new NSStyleBordure(*(rv.pBordure));

	Decors = rv.Decors;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPage::~NSPage()
{
   if (pBordure != 0)
		delete pBordure;
   pBordure = 0 ;

}
//---------------------------------------------------------------------------//  Fonction	 : NSPage::InitNumPage(string* chaineInit)
//
//  Description : Initialisation des marges
//---------------------------------------------------------------------------
void
NSPage::InitNumPage(string* chaineInit)
{
  //
  // Recherche de la dernière occurence du mot "page"
  //
  size_t recherche = chaineInit->rfind("page");
  if (string::npos == recherche)
  {
    numeroPage = 0;
    return;
  }
  //
  // On isole la sous-chaine correspondante
  //
  string sSuite = string(*chaineInit, recherche+1, strlen(chaineInit->c_str())-(recherche+1));
  //
  // On recherche dans cette sous-chaine les premiers chiffres
  //
  unsigned longueur = strlen(sSuite.c_str());
  int j;
  for (j = 0; (j < longueur) && (!isdigit(sSuite[j])); j++);
  if (j < longueur)
  {
    long i = 0;
		for (int k = 0; (j < longueur) && (isdigit(sSuite[j])) && (k < 3); j++, k++)
		{
			i = long(10) * i;
			i += long(sSuite[j]) - long('0');
		}
    numeroPage = i;
  }
}


//---------------------------------------------------------------------------
//  Fonction	 : NSPage::InitMarges(string* chaineInit)
//
//  Description : Initialisation des marges
//---------------------------------------------------------------------------
void
NSPage::InitMarges(string* chaineInit)
{
	UINT j, k, indice, longueur, nbMax, nbVal;
   long i;
	longueur = strlen(chaineInit->c_str());
   //
   // On fixe à 5 la taille maximale de chaque valeur
   //
   nbMax = 5;
   //
   // On fixe le nombre de valeurs
   //
   nbVal = 4;
	//
	// Prise de la première valeur : Retrait gauche
	//
	for (j = 0, indice = 1; indice <= nbVal; indice++, j++)
	{
		//
		// Prise d'une valeur
		//
      i = 0;
		for (k = 0; (j < longueur) && (isdigit((*chaineInit)[j])) && (k < nbMax); j++, k++)
		{
			i = long(10) * i;
			i += long((*chaineInit)[j]) - long('0');
		}
		//
		// Test de "propreté" de la chaine d'initialisation
		//
		if ((indice < nbVal) && (j < longueur) && ((*chaineInit)[j] != ',')) return;
		if ((indice == nbVal) && (j < longueur)) return;
		//
		// Mise de la valeur dans sa variable
		//
		switch (indice) {
			case 1 : MargeGauche = i; break;
			case 2 : MargeDroite = i; break;
         case 3 : MargeHaute 	= i; break;
			case 4 : MargeBasse 	= i; break;
		}
	}
}

//---------------------------------------------------------------------------
//  Fonction	 : NSPage::InitMargesTxt(string* chaineInit)
//
//  Description : Initialisation des marges supplémentaires du texte
//---------------------------------------------------------------------------
void
NSPage::InitMargesTxt(string* chaineInit)
{
	UINT j, k, indice, longueur, nbMax, nbVal;
   long i;
	longueur = strlen(chaineInit->c_str());
   //
   // On fixe à 5 la taille maximale de chaque valeur
   //
   nbMax = 5;
   //
   // On fixe le nombre de valeurs
   //
   nbVal = 4;
	//
	// Prise de la première valeur : Retrait gauche
	//
	for (j = 0, indice = 1; indice <= nbVal; indice++, j++)
	{
		//
		// Prise d'une valeur
		//
      i = 0;
		for (k = 0; (j < longueur) && (isdigit((*chaineInit)[j])) && (k < nbMax); j++, k++)
		{
			i = long(10) * i;
			i += long((*chaineInit)[j]) - long('0');
		}
		//
		// Test de "propreté" de la chaine d'initialisation
		//
		if ((indice < nbVal) && (j < longueur) && ((*chaineInit)[j] != ',')) return;
		if ((indice == nbVal) && (j < longueur)) return;
		//
		// Mise de la valeur dans sa variable
		//               MargeTxtGch, MargeTxtDte, MargeTxtHaut, MargeTxtBas
		switch (indice) {
			case 1 : MargeTxtGch  = i; break;
			case 2 : MargeTxtDte  = i; break;
         case 3 : MargeTxtHaut = i; break;
			case 4 : MargeTxtBas	 = i; break;
		}
	}
}

//---------------------------------------------------------------------------
//  Fonction	 : NSPage::InitDecor(string* chaineNom, string* chaineInit)
//
//  Description : Initialisation des décors
//---------------------------------------------------------------------------
void
NSPage::InitDecor(string* chaineNom, string* chaineInit){
	NSDecor* pDecor = new NSDecor;

   pDecor->Titre = *chaineNom;

   size_t recherche;
   string sCoord, sCoordXY[2];

   sCoordXY[0] = "";
   sCoordXY[1] = "";

   string sTempo = *chaineInit;
   //
   // Séparation entre titre et coordonnées
   //
   recherche = sTempo.find(',');
   if ((string::npos != recherche) && (recherche < strlen(sTempo.c_str())-1))
   {
   	pDecor->titreCadre = string(sTempo, 0, recherche);
      //
      // Séparation entre coordonnée X et coordonnée Y
      //
      sCoord = string(sTempo, recherche+1, strlen(sTempo.c_str())-(recherche+1));
      recherche = sCoord.find(',');
   	if ((string::npos != recherche) && (recherche < strlen(sCoord.c_str())-1))
   	{
   		sCoordXY[0] = string(sCoord, 0, recherche);
      	sCoordXY[1] = string(sCoord, recherche+1, strlen(sCoord.c_str())-(recherche+1));
      }
      else
      {
      	if (string::npos == recherche)
      		sCoordXY[0] = sCoord;
      	else
      		sCoordXY[0] = string(sCoord, 0, recherche);
      }
      //
      // Traitement des coordonnées X et Y
      //
      WORD Alignement;
      long Taille;
      for (int i = 0; i < 2; i++)
      {
         Taille = 0;
         //
         // Elément de justification
         //
			if 	  ((sCoordXY[i] == "gauche") ||
				 		(sCoordXY[i] == "left"))
				Alignement = TA_LEFT;
			else if ((sCoordXY[i] == "droit")  ||
						(sCoordXY[i] == "droite") ||
						(sCoordXY[i] == "right"))
				Alignement = TA_RIGHT;
			else if ((sCoordXY[i] == "centré") ||
						(sCoordXY[i] == "centre") ||
						(sCoordXY[i] == "center"))
				Alignement = TA_CENTER;
			else if ((sCoordXY[i] == "justifié") ||
						(sCoordXY[i] == "justif") 	||
						(sCoordXY[i] == "justified"))
				Alignement = TA_JUSTIFIED;
         //
         // Elément numérique
         //
      	else if  (sCoordXY[i] != "")
         {
            unsigned longueur = min(unsigned(5), strlen(sCoordXY[i].c_str()));
				for (int j = 0; (j < longueur) && (isdigit(sCoordXY[i][j])); j++)
				{
					Taille = long(10) * Taille;
					Taille += long(sCoordXY[i][j]) - long('0');
				}
         }
         //
         // Stockage des valeurs
         //
         if (Taille > 0)
         {
         	if (i == 0)
            	pDecor->coinHautGcheX = Taille;
            if (i == 1)
            	pDecor->coinHautGcheY = Taille;
         }
         else
         {
         	if (i == 0)
            	pDecor->AlignementX = Alignement;
            if (i == 1)
            	pDecor->AlignementY = Alignement;
         }
      }
   }
   else
   {
   	if (string::npos == recherche)
      	pDecor->titreCadre = sTempo;
      else
      	pDecor->titreCadre = string(sTempo, 0, recherche);
   }
   //
   // On ajoute le décor à l'array
   //
   Decors.push_back(new NSDecor(*pDecor));
   delete pDecor;
   pDecor = 0 ;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSPage::loadDefault()
//
//  Description : Initialisation des décors
//---------------------------------------------------------------------------
void
NSPage::loadDefault()
{
	MargeGauche = MargeDroite = 150;
   MargeHaute  = MargeBasse  = 250;

   MargeTxtGch  = MargeTxtDte = 0;
   MargeTxtHaut = MargeTxtBas = 0;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPage&
NSPage::operator=(const NSPage& rv)
{
  if (this == &rv)
		return *this ;

	Titre 			= rv.Titre;
   numeroPage 		= rv.numeroPage;

   MargeGauche 	= rv.MargeGauche;
   MargeDroite 	= rv.MargeDroite;
   MargeHaute 		= rv.MargeHaute;
   MargeBasse 		= rv.MargeBasse;

   MargeTxtGch		= rv.MargeTxtGch;
   MargeTxtDte		= rv.MargeTxtDte;
   MargeTxtHaut	= rv.MargeTxtHaut;
   MargeTxtBas		= rv.MargeTxtBas;

   CouleurFond 	= rv.CouleurFond;
   CouleurTexte	= rv.CouleurTexte;
   CouleurBordure = rv.CouleurBordure;

   if (rv.pBordure == 0)
		pBordure = 0;
	else
		pBordure = new NSStyleBordure(*(rv.pBordure));

   Decors = rv.Decors;

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPage::operator == ( const NSPage& o )
{
	 if ((Titre 	  == o.Titre) &&
		  (numeroPage == o.numeroPage))
		  return 1;
	 else
		  return 0;
}

