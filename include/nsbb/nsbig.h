//---------------------------------------------------------------------------
//    NSBigBro.H
//
//  Contient la déclaration de NSBigBrother, gestionnaire général des
//  applications NAUTILUS.
//  KP
//  Création : sept 93         Dernière mise à jour : mars 94
//---------------------------------------------------------------------------
#ifndef __NSBIGBROTHER_H
#define __NSBIGBROTHER_H

#include <windows.h>
#include <classlib\arrays.h>
#include <cstring.h>

#include "partage\nsglobal.h"
#include "ns_sgbd\nsfiche.h"
#include "nssavoir\nsguide.h"
//#include "nsdlg.h"
//#include "nslesion.h"
//#include "nslesarr.h"
#include "nautilus\nsrechd2.h"

//#if defined(_DANSNSBBDLL)
#if defined(_DANSDLL)
	#define _NSBBCLASSE __import
#else
	#define _NSBBCLASSE __export
#endif

//---------------------------------------------------------------------------
//  Objet Big-Brother
//  Big-Brother n'est habilité qu'à manipuler ce type d'objet pour la
//  création des objets d'interface. Il faut donc qu'il y trouve toutes les
//  informations nécssaires. Si toutefois il y a ajout d'information, elle ne
//  peut-être comprise que par la modification de la structure NSBBObj.
//  Il maintient également une liste chaînée de NSLesion (NSMonoLesion et
//  NSMultiLesion). Il est le seul à pouvoir manipuler les informations.
//---------------------------------------------------------------------------
/*class BBObj {
  public :
//  NSWindow*          pWnd;        // Pointeur sur la fenêtre OWL associée
//  BBItemArray*    pBBTranfert;    // Pointeur sur l'objet BB concerné
//  BBTransfertObj*    pBBParent;   // Pointeur sur l'objet BB parent
  WNDTYPE            iWndType;    // Indicateur de type de fenêtre associée

// Constructeurs
  BBObj(BBTransfertObj* parent = NULL, WNDTYPE type = isDlg)
  : pBBParent(parent), iWndType(type) {
	 pBBTransfert = new BBItemArray();
  }

  BBObj(RBBObj src)
  : pWnd(src.pWnd), pBBTransfert(src.pBBTransfert), pBBParent(src.pBBPparent),
	 iWndType(src.iWndType) {}

// Destructeur
//  ~BBObj() { delete pBBTransfert; }

// Surcharge de l'opérateur =
	BBObj& operator=(BBObj src) {
	pWnd         = src.pWnd;
	pBBTransfert = src.pBBTransfert;
	pBBParent    = src.pBBPparent;
	iWndType     = src.iWndType;
	return *this;
  }

// Accés aux données membres
  NSWindow* window()              { return pWnd; }
  void      window(NSWindow* src) { pWnd = src;}
  BBObj*    transfert()           { return pBBTransfert; }
  void      transfert(BBObj* src) { pBBTransfert = src; }
  BBObj*    parent()              { return pBBParent; }
  void      parent(BBObj* src)    { pBBParent = src; }
  WNDTYPE   type()                { return iWndType; }
  void      type(WNDTYPE src)     { iWndType = src; }
}; */

//---------------------------------------------------------------------------
//  Classe NSBIGBROTHER
//         ANCETRE   :
//         SURCHARGE :
// Cette classe sert à mettre en place le gestionnaire global de création
// des fenêtres NAUTILUS.
// Il est le seul habilité à manipuler les BBObj et les NSLesion.
//---------------------------------------------------------------------------
//class _NSBBCLASSE NSBigBrother
class NSBigBrother
{

	public :

	//NSSuper* pSuperviseur;
	//TWindow* pFenetreMere;
	//TModule* pNSResModule;

	BBFiche*	pBBFiche;

	char 		contexteModule;
	char		codeModule[3], bigModule[9];

	string* 	psCRString;             	// Eventuelle phrase codée

	//BBLesionsArray*   paLesions;      // Array de NSLesion
	//BBVariablesArray* paVariables;    // Array de NSVariable
	//unsigned int 	  	numeroLesion;   // Numéroteur de lésions
	//unsigned int 	  	numeroVariable; // Numéroteur de variables

	// Constructeur
	NSBigBrother(/*NSSuper* pSuper*/);
	NSBigBrother(const NSBigBrother& rv);
	// Destructeur
	~NSBigBrother();
	// Opérateur =
	NSBigBrother& operator=(const NSBigBrother& src);

	// Démarrage d'un script
	BOOL lanceModule();
	lanceScript();
	generePhrase();

	// Maintenance et sauvegarde des lésions
/*	int ajouteLesion(NSLesion* pLesion);  // Ajoute une lésion à l'array
	int remplaceLesion(NSLesion* pLesion);

	int litLesion(string sLocali, NSLesion* pLesion); // lésion de l'array
	int litLesionAmmorce(string sLocali, NSLesion* pLesion);

	int litLesionPrec(NSLesion* pLesion);
	int litLesionSuiv(NSLesion* pLesion);
	int litLesionAmmorceSuiv(int tailleAmmorce, NSLesion* pLesion);

	int detruitLesion(NSLesion* pLesion);

	int compteLesion(string sLocali);

	int compteLesionAmmorce(string sLocali);

	// Maintenance et sauvegarde des variables
	int ajouteVariable(NSVariable* pVariable);  // Ajoute une variable à l'array
	int remplaceVariable(NSVariable* pVariable);

	protected :

	//
	// L'accés normal à une lésion se fait par son numéro, toutes les méthodes
	// qui utilisent l'indice au sein de l'array doivent être déclarées
	// en accés protégé
	//
	int donneLesionNumero(int iNumero);
	int remplaceLesion(int iNumLesion, NSLesion* pLesion);
	int litLesion(int iNumLesion, NSLesion* pLesion);
	int litLesionPrec(int iNumLesion, string sLocali, NSLesion* pLesion);
	int litLesionSuiv(int iNumLesion, string sLocali, NSLesion* pLesion);
	int litLesionAmmorceSuiv(int iNumLesion, string sLocali, NSLesion* pLesion);
	int detruitLesion(int iNumLesion);

	int donneVariableNumero(int iNumero);
	int remplaceVariable(int iNumVariable, NSVariable* pVariable); */
};
//extern NSBigBrother BigBoss;

#endif