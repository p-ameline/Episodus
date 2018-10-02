//----------------------------------------------------------------------------
// Styles utilises pour le traitement du texte (Cadre, Paragraphe, Bordures)
//
// PA Septembre 94
//----------------------------------------------------------------------------

#ifndef __NSTXSTYL_H
#define __NSTXSTYL_H

#include <owl\color.h>
#include <owl\gdiobjec.h>

#include "partage/ns_vector.h"
#include "partage/NTArray.h"

#define NBRE_TABULATIONS 10
#define TA_JUSTIFIED 200

#if defined(_DKD_DLL)
	#define _NSDKDCLASSE __export
#else
	#define _NSDKDCLASSE __import
#endif

class _NSDKDCLASSE NSDecor
{
  public:

	  string Titre;
    long 	 coinHautGcheX, coinHautGcheY;
    WORD	 AlignementX, AlignementY;
    string titreCadre;

    // Constructeur, Constructeur copie et destructeur

    NSDecor();
    NSDecor(const NSDecor& rv);
    ~NSDecor();

    // Opérateurs d'affectation et de comparaison

    NSDecor& operator=(const NSDecor& src);
    int operator == (const NSDecor& o);
    int operator != (const NSDecor& o) {return !(*this==o);}
};

typedef vector <NSDecor*>             NSNSDecoArray ;
typedef NSNSDecoArray::iterator       NSDecoArrayIter ;
typedef NSNSDecoArray::const_iterator NSDecoArrayConstIter ;
typedef NTArray<NSDecor>              NSDecorArray ;

//
// Objet qui represente un style de bordure à 1 ou 2 traits
//
class _NSDKDCLASSE NSStyleBordure
{
  public:

	 UINT	DistanceDuBord;
	 UINT	EpaisseurLigneExt, EspaceEntreLignes, EpaisseurLigneInt;

	 // Constructeur
	 NSStyleBordure();

	 // Retourne la largeur de la bordure
	 UINT Largeur() { return(DistanceDuBord + EpaisseurLigneExt +
									 EspaceEntreLignes + EpaisseurLigneInt); }
	 int operator == (const NSStyleBordure& o);
	 int operator != (const NSStyleBordure& o) {return !(*this==o);}
};

//
// Objet Police Nautilus
//
class _NSDKDCLASSE NSFont
{
  public :

	LOGFONT logFont;

	// ---------------------- Constructeurs ------------------------
	// Convenient font ctor
	NSFont(const char far* facename=0,
			 int height=0, int width=0, int escapement=0, int orientation=0,
			 int weight=FW_NORMAL,
			 BYTE pitchAndFamily=DEFAULT_PITCH|FF_DONTCARE,
			 BYTE italic=FALSE, BYTE underline=FALSE, BYTE strikeout=FALSE,
			 BYTE charSet=1,  // DEFAULT_CHARSET or UNICODE_CHARSET
			 BYTE outputPrecision=OUT_DEFAULT_PRECIS,
			 BYTE clipPrecision=CLIP_DEFAULT_PRECIS,
			 BYTE quality=DEFAULT_QUALITY);

	// Constructeur à partir d'une TFont
	NSFont(OWL::TFont* rv);
	NSFont(LOGFONT* rv);
	// Constructeur à partir d'un DC : prend le modèle de la police en cours
	NSFont(TDC* pdc);
	// Constructeur copie et destructeur
	NSFont(const NSFont& rv);
	~NSFont();

	// méthodes
	void prendLogFont(const LOGFONT* lf);

	// Opérateurs d'affectation et de comparaison
	NSFont& operator=(const NSFont& src);
	int operator == (const NSFont& o);
	int operator != (const NSFont& o) {return !(*this==o);}
};

//
// Objet qui représente un style de paragraphe
//
class _NSDKDCLASSE NSStyleParagraphe
{
  public:

    string Titre ;

    UINT	 NumPolice ;
    UINT	 HauteurLigne ;
    UINT	 RetraitGauche, RetraitDroit, RetraitPremLigne ;
    UINT	 EspaceDessus, EspaceDessous ;
    WORD	 Alignement ;
    UINT	 Tabulations[NBRE_TABULATIONS] ;

    //TPalette	*pCouleurTexte;
    // TColor	CouleurPuces, CouleurLettrines, CouleurNumeroteur, CouleurFilets;
    // Prévoir puces, lettrines, filets et numéroteurs

    // Constructeur et constructeur copie et destructeur

    NSStyleParagraphe() ;
    NSStyleParagraphe(const NSStyleParagraphe& rv) ;
    ~NSStyleParagraphe() ;

    // Opérateurs d'initialisation
    void Reinitialiser();

    void InitPolice(string* chaineInit) ;
    void InitRetrait(string* chaineInit) ;
    void InitEspacement(string* chaineInit) ;
    void InitAlignement(string* chaineInit) ;
    void InitTabulations(string* chaineInit) ;

    /*InitFilets(string* chaineInit);
	 InitCouleurs(string* chaineInit);
	 InitPuce(string* chaineInit);
	 InitLettrine(string* chaineInit);
	 InitNumerotation(string* chaineInit); */

    // Opérateurs d'affectation et de comparaison
    NSStyleParagraphe& operator=(const NSStyleParagraphe& src) ;
    int operator == (const NSStyleParagraphe& o) ;
    int operator != (const NSStyleParagraphe& o) { return !(*this == o) ; }
};

//
// Objet qui représente un style de décor (ligne de titre...)
//
class _NSDKDCLASSE NSStyleDecor
{
  public:

    string				    Texte ;
    NSStyleParagraphe styleParagraphe ;

    // Constructeur et constructeur copie et destructeur
    NSStyleDecor() ;
    NSStyleDecor(const NSStyleDecor& rv) ;
    ~NSStyleDecor() ;

    // Opérateurs d'affectation et de comparaison
    NSStyleDecor& operator=(const NSStyleDecor& src) ;
    int operator == (const NSStyleDecor& o) ;
    int operator != (const NSStyleDecor& o) { return !(*this == o) ; }
};

//typedef TArrayAsVector<NSStyleDecor> NSStylDecoArray;
typedef vector<NSStyleDecor*>             NSNSStylDecoArray ;
typedef NSNSStylDecoArray::iterator       NSStylDecoArrayIter ;
typedef NSNSStylDecoArray::const_iterator NSStylDecoArrayConstIter ;
typedef NTArray<NSStyleDecor>             NSStyleDecorArray ;

//
// Objet qui représente un style de cadre
//
class _NSDKDCLASSE NSStyleCadre
{
  public:

	 string			  Titre;

	 UINT				  MargeGauche, MargeDroite, MargeHaute, MargeBasse;
	 NSStyleBordure  *pBordGauche, *pBordDroit, *pBordHaut, *pBordBas, *pTousBords;
						  // Lorsque la même bordure s'applique à tous les bords,
						  // on peut demander des coins arrondis.
	 UINT				  RayonCoinsRonds;
	 //TPalette		  *pCouleurFond, *pCouleurBordure;
						  // Colorier jusqu'à 0 : marges,
						  //						 1 : Bordure intérieure
						  //						 2 : Bordure extérieure
	 short			  ColorierJusqua;
	 BOOL				  CadreTransparent, HabillageTexte;

	 // Constructeur, Constructeur copie et destructeur
	 NSStyleCadre();
	 NSStyleCadre(const NSStyleCadre& rv);
	 ~NSStyleCadre();

	 // Opérateurs d'affectation et de comparaison
	 NSStyleCadre& operator=(const NSStyleCadre& src);
	 int operator == (const NSStyleCadre& o);
	 int operator != (const NSStyleCadre& o) {return !(*this==o);}
};

//
// NSCadreDecor donne le style et le contenu du cadre.
// NSDecor positionne le cadre dans la page.
//
// Cette approche permet de positionner un même décor à un emplacement
// différent suivant la page.
//
class _NSDKDCLASSE NSCadreDecor
{
  	public:

  	string			  	 Titre;

   NSStyleCadre	  	 style;
   long				  	 tailleHorizontale, tailleVerticale;

   NSStyleDecorArray  DecorArray;

   NSStyleDecor*		 pStyleDecor; // Utilisé pour initialiser pDecorArray

   // Constructeur, Constructeur copie et destructeur
   NSCadreDecor();
   NSCadreDecor(const NSCadreDecor& rv);
   ~NSCadreDecor();

   // Fonctions d'initialisation des variables
   void InitTitre(string* sTitre);
   void InitTaille(string* chaineInit);

   void debutTexte();
   void finTexte();
   void InitTexte(string* chaineInit);
   void InitPolice(string* chaineInit);
   void InitRetrait(string* chaineInit);
   void InitEspacement(string* chaineInit);
   void InitAlignement(string* chaineInit);
   void InitTabulations(string* chaineInit);

   // Opérateurs d'affectation et de comparaison
   NSCadreDecor& operator=(const NSCadreDecor& src);
   int operator == (const NSCadreDecor& o);
   int operator != (const NSCadreDecor& o) {return !(*this==o);}
};

class _NSDKDCLASSE NSPage
{
  	public:

   string			    Titre;

   UINT				    numeroPage;

   long				    MargeGauche, MargeDroite, MargeHaute, MargeBasse;
   long				    MargeTxtGch, MargeTxtDte, MargeTxtHaut, MargeTxtBas;
   NS_CLASSLIB::TColor	CouleurFond, CouleurTexte, CouleurBordure;
   NSStyleBordure*      pBordure;

   NSDecorArray	 Decors;

   // Constructeur, Constructeur copie et destructeur
   NSPage();
   NSPage(const NSPage& rv);
   ~NSPage();

   // Fonctions d'initialisation des variables
   void InitNumPage(string* chaineInit);
   void InitMarges(string* chaineInit);
   void InitMargesTxt(string* chaineInit);
   void InitDecor(string* chaineNom, string* chaineInit);
   void loadDefault();

   // Opérateurs d'affectation et de comparaison
   NSPage& operator=(const NSPage& src);
   int operator == (const NSPage& o);
   int operator != (const NSPage& o) {return !(*this==o);}
};

#endif // __NSTXSTYL_H

