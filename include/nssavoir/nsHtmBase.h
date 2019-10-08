//---------------------------------------------------------------------------//     NS_MODHTM.H
//
//  Contient les d�finitions des arrays d'objets HTML de NAUTILUS
//
//  1ere version : RS juin 97   Derni�re modif : 05/10/98
//---------------------------------------------------------------------------

#ifndef __NS_HTMBASE_H
#define __NS_HTMBASE_H

class NSHtml ;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "partage\ns_vector.h"
#include "partage\NTArray.h"

// Ensemble des types HTML
enum HtmlTypes {
    tUndefined = -1, tHtml=0, tHead, tTitle, tBase, tBody, tXML, tNautilus,
    tTitre1, tTitre2, tTitre3, tTitre4, tTitre5, tTitre6, tTitre7, tTitre8,
    tTitre9, tTitre10, tTitre11, tTitre12, tTitre13, tTitre14, tTitre15,
    tTitre16, tTitre17, tTitre18, tTitre19,
    tTable, tTableRow, tTableCell,
    tTexte, tGras, tSouligne, tItalic, tImage, tIndent,
    tPara, tParaCentre, tParaDroite, tIntitule, tIntituleNoBirth, tPolice,
    tOperateur, t2ndOperateur, tAdresse, tDest, tDate, tCR, tRTF, tSignature,
    tNomPatient, tPatientNom, tPatientPrenom, tPatientNomNaiss,
    tPatientNaiss, tPatientIPP, tPatientIEP, tPatientAdresse, tPatientGender,
    tConsult, tCS, tCQ, tTXT
  };

// type �num�r� des op�rations
enum OperationType { toComposer = 0, toImprimer, toExporter, toImporter, toVisualiser } ;

//
// D�finition de NSHtmlArray (Vecteur de NSHtml)
//
typedef vector<NSHtml*> NSHtmlModeleArray ;
typedef NSHtmlModeleArray::iterator HtmlIter ;
typedef NTArray<NSHtml> NSHtmlArray ;

///////////////////////////////////////////////////////////////// Fonctions globales de conversion
//

///////////////////////////////////////////////////////////////

#define HTML_MARQUEURS_SIZE 20

#ifndef __linux__
# if defined(_DANSLEXIDLL)
#  define _CLASSELEXI __export
# else
#  define _CLASSELEXI __import
# endif
#endif

class _CLASSELEXI NSHtml
{
  private:

 	  static long lObjectCount ;

	public :

    // Variables
    HtmlTypes   _type ;		    // type �num�r� de l'element HTML
    string      _sType ;		    // type string de l'element HTML
    string      _sPath ;       // chemin de cet �l�ment dans la patpatho
    string      _sTexte ;      // corps de l'element HTML
    string      _sURL ;		    // texte de l'URL (cas des liens)
    int         _size ;        // taille de la police
    string      _sFace ;       // nom de la police
    NSHtmlArray _filsArray ;   // tableau des fils

    // marqueurs pr�d�finis pour les titres
    string _sPrefix ;
    /* static */ string _marqueurTitreDebut[20] ;  // d�but de bloc
    /* static */ string _marqueurTitreFin[20] ;	  // fin de bloc
    string _sSuffix ;

    // Constructeurs
    NSHtml(const HtmlTypes th = tHtml, const string sTxt = string("")) ; // par d�faut : type fichier HTML
    NSHtml(const string sTyp, const string sTxt = string("")) ;	  // avec type string
    NSHtml(const HtmlTypes th, const string face, const int taille) ; // pour initialiser une police
    NSHtml(const NSHtml& rv) ;                      // constructeur par recopie

    // Op�rateurs
    NSHtml& operator=(const NSHtml& src) ;

    // Destructeur
    ~NSHtml() ;

    // M�thodes
    void    init(string titre) ;                    // fichier HTML de base
    NSHtml* GetBody() ;                             // r�cup�re le pointeur sur Body
    NSHtml* GetFirstSon()         { return _filsArray[0] ; }
    NSHtml* GetLastSon()          { return _filsArray.back() ; }
    bool    isEmpty()             { return _filsArray.empty() ; }
    void    insere(NSHtml *pPere, NSHtml *pFils) ;  // HTML = fils(HTML)
    void    addSon(NSHtml *pFils) { _filsArray.push_back(pFils) ; }
    bool    ecrire(char *fichier) ;							    // Objet HTML -> fichier HTML
    string  marqueurDebut() ;                    		// renvoie le marqueur d�but
    string  marqueurFin() ;                      		// renvoie le marqueur fin
    static string space(const int n) { return string(n, ' ') ; }
    void    remplaceTitre(string& sTag) ;					  // transforme un tag titre en html
    void    initTitres(string& params) ;                     // initialise les marqueurs titres
    static void genereTexte(const string texte, const int tab, string& sOut) ;	// ecrit texte dans sOut
    static void genereTexte(const string texte, string& sOut) ;		// ecrit texte dans sOut
    void    genere(string& sOut) ;              				// g�n�re fichier HTML dans sOut
    void    boldifier(string sOpeningTag, string sClosingTag) ; // apply bold tags

    // retrouve la string correspondant au type
    static string typeString(const HtmlTypes th) ;
    // retrouve le type html correspondant au tag
    static HtmlTypes typeTag(const string sTag) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    void   setType(string sType) { _sType  = sType ; }
    void   setText(string sText) { _sTexte = sText ; }
    void   addText(string sText) { _sTexte += sText ; }
    void   setURL(string sUrl)   { _sURL   = sUrl ;  }
    void   setPath(string sPath) { _sPath  = sPath ; }

    string getType()             { return _sType ; }
    string getText()             { return _sTexte ; }
    string getPath()             { return _sPath ; }
    string getPrefix()           { return _sPrefix ; }
    string getSuffix()           { return _sSuffix ; }

    NSHtmlArray* getSons()       { return &_filsArray ; }

  protected:

    void resetMarquers() ;
    void copyMarquers(const NSHtml& rv) ;
};

class _CLASSELEXI NSBlocHtml{
  private:

 	  static long lObjectCount ;

  public :

    int     _occur ;
    NSHtml* _pHtml ;

    NSBlocHtml(NSHtml* ph = 0, int nbOccur = 1) ;
    NSBlocHtml(const NSBlocHtml& rv) ;
    NSBlocHtml& operator=(const NSBlocHtml& src) ;    ~NSBlocHtml() ;

    NSHtml* getHtml() { return _pHtml ; }
    int     occur()   { return _occur ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

//// D�finition de NSBlocHtmlArray (Vecteur de NSBlocHtml)
//
typedef vector<NSBlocHtml*> NSBlocHtmlModeleArray ;
typedef NSBlocHtmlModeleArray::iterator NSBlocHtmlArrayIter ;
typedef NTArray<NSBlocHtml> NSBlocHtmlArray ;

//
// D�finition de NSBlocHtmlMatrice (Vecteur de NSBlocHtmlArray)
//
typedef vector<NSBlocHtmlArray*> NSBlocHtmlModeleMatrice ;
typedef NSBlocHtmlModeleMatrice::iterator NSBlocHtmlMatriceIter ;
typedef NTArray<NSBlocHtmlArray> NSBlocHtmlMatrice ;

//---------------------------------------------------------------------------#endif    // __NS_HTMBASE_H

