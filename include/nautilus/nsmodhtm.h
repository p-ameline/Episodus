//---------------------------------------------------------------------------//     NS_MODHTM.H
//
//  Contient les d�finitions des arrays d'objets HTML de NAUTILUS
//
//  1ere version : RS juin 97   Derni�re modif : 05/10/98
//---------------------------------------------------------------------------

#ifndef __NS_MODHTM_H
#define __NS_MODHTM_H

class NSHtml ;
class NSBaseImages ;
class NSNoyauDocument ;
class NSDocumentInfo ;
class NSPatPathoArray ;
class NSLdvDocument ;

#include <owl/dialog.h>
#include <owl/richedit.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "partage\ns_vector.h"
#include "partage\NTArray.h"
#include "nssavoir\nsHtmBase.h"

class NSModHtml : public NSRoot
{
  private:

 	  static long lObjectCount ;

  public :

    NSBlocHtmlArray   blocHtml ;  	     // ensemble de blocs html composant le fichier html final
    int               nbHtml ;           // nombre de blocs
    NSBlocHtmlMatrice _table ;           // matrice de blocs pour les tableaux
    bool              bExisteTable ;	   // pour le lancement du processTable
    int               nbTagsImage ;
    string            sModele ;          // chaine contenant le fichier template html
    string            sFichierTemplate ; // nom du fichier template html
    string            sFichierEnTete ;	 // nom du fichier en-tete du fichier template
    string            _sOut ;            // chaine de sortie (fichier html)
    string            sCodeDoc ;         // code document brut
    string            sBaseImages ;	     // fichier contenant les noms des images du document
    string            sBaseCompo ;		   // fichier contenant les noms des images de la chemise
    OperationType     typeOperation ;	   // type d'op�ration de cr�ation du html
    NSBaseImages*     pBaseImages ;	     // pointeur sur la base d'images du document
    NSBaseImages*     pBaseCompo ;		   // pointeur sur la base d'images de composition
    NSNoyauDocument*  _pDocBrut ;		     // pointeur sur le document brut
    NSLdvDocument*    _pLdvDoc ;

    // Le mod�le Html est cr�� � partir d'un fichier (pDocRef = 0) ou d'un document
    // Si pDocRef != 0, on doit transmettre le superviseur pour l'acc�s aux bases (+ chemins)
    NSModHtml(OperationType typeOp, NSNoyauDocument* pDocNoy = 0, NSContexte* pCtx = 0) ;
    NSModHtml(OperationType typeOp, NSLdvDocument* pDocNoy = 0, NSContexte* pCtx = 0) ;
    virtual ~NSModHtml() ;

    // cr�e un nouveau bloc Html
    void    ajoute(NSHtml* ph, int nbOccur = 1) ;
    void    ajoute(HtmlTypes th, string sNonHtmlTxt) ;

    // Adding value added blocks
    void    addSignature() ;
    void    addPatient() ;

    // initialise la chaine sModele
    bool    lireModele(string fichier, string entete = "") ;
    // texte Html <- texte normal
    // string  texteHtml(string texte) ;
    // cr�ation nom sans doublons
    string	nomSansDoublons(string pathRef, string nom, string ext) ;
    // renvoie true si la base est charg�e
    bool    chargeBaseImages(string sPathBase, string sPathImages, bool bPreSelect = false, NSDocumentInfo* pHtmlInfo = 0) ;
    // idem pour la publication
    bool    chargeBaseImages(string sPathBase, string sPathImages, NSDocumentInfo* pHtmlInfo) ;
    // R�cup�re les donn�es des documents images
    bool    donneesImages(string sCodeImage, string& sPathSource, string& sFichierSource, string& sTypeImage) ;
    // pour la visualisation des images
    bool    chargeDocImage(string sPathBase, string sPathImages, NSDocumentInfo* pDocInfo) ;
    // cr�e un lien vers le pgm CGI select
    bool    genereLienImage(int numImage, string sFichCompo, int largeur = 0, int hauteur = 0) ;
    // cr�e un appel de l'image sNomImage
    void    genereImage(string sUrlImg, string sNomImage, string sTypeImage,
   								int largeur = 0, int hauteur = 0) ;
    // R�cup�re les param�tres du tag image (largeur et hauteur)
    void    recupTailleImage(int numImage, string sParam, int& largeur, int& hauteur) ;
    // g�n�re une image � partir de la base d'images
    bool    genereImageBase(int numImage, int largeur = 0, int hauteur = 0) ;
    // PatPatho -> string
    bool    generePatPatho(NSPatPathoArray* pPatPathoArray) ;
    // g�n�re le fichier en-tete et les fichiers html bruts
    bool    genereHtmlBrut(string sNomTag, string sNomFichier) ;

    // Retrouve l'url associ�e � un chemin
    // (si ce chemin n'est pas sous la racine du serveur, sUrl <- sChemin)
    bool    chercheUrl(string sChemin, string& sUrl) ;

    // ins�re le bloc html correspondant au tag. Cas des tags XML : ne fait rien
    void    remplaceTag(string sTag, string params, string sPath = "") ;
    void    remplacePathTag(string sPath, string sParams, NSHtml* pBloc) ;

    // remplace un tag string par sa valeur
    bool    valeurTag(string sTag, string& sValeurTag, NSBlocHtmlMatriceIter iterTable) ;

    // compte le nombre de tags sTag
    int     compteTags(string sTag) ;

    // Avril 04 : scan dans la template des param�tres d'impression pour WebBrowserPrint
    void    scanPrintParams() ;

    // Pr�-process pour les tableaux g�n�riques
    void    processTable() ;

    // genere le modele en cours dans fichier html
    bool    genereModele(string sFichHtml, NSDocumentInfo* pHtmlInfo,
   						string sPathBase, string sPathImages = "",
                     string sFichierExterne = "", string sTypeFichier = "") ;

    // G�n�ration d'un fichier html � partir d'un document Nautilus
    // La template et le fichier en-tete sont r�cup�r�s � partir du document
    bool    genereHtml(string sPathHtml, string& sBaseImg,
                       NSDocumentInfo* pHtmlInfo = 0, string sPathDest = "") ;

    bool    genereHtml(string sPathHtml, string& sBaseImg,
   					           string sFichierTemplate, string sFichierEntete) ;

    // G�n�ration d'un fichier html � partir d'un fichier externe
    // La template est transmise et le fichier n'a pas d'en-tete
    bool    genereHtml(string sTemplate, string sPathHtml, string sPathSource,
   							string sNomFichier, string sTypeFichier) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

//---------------------------------------------------------------------------#endif    // __NS_MODHTM_H

