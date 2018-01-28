//---------------------------------------------------------------------------
//    NSBigBro.H
//
//  Contient la déclaration de NSBigBrother, gestionnaire général des
//  applications NAUTILUS.
//  KP
//  Création : sept 93         Dernière mise à jour : mars 94
//---------------------------------------------------------------------------
#ifndef __NSBBITEC_H
#define __NSBBITEC_H

#include <classlib\arrays.h>
#include <cstring.h>

#include "partage\nsglobal.h"
#include "ns_sgbd\nsfiche.h"
#include "nssavoir\nsguide.h"
//#include "nsbb\nsbig.h"
//#include "nslesion.h"

#ifndef __NSDLG_H
	class NSBBFonction;
#endif

//---------------------------------------------------------------------------
//
// Structure de classement des BBItem fils d'un BBItem donné
//
//---------------------------------------------------------------------------
class BBFilsPhraseItem {

	  public :

	  //NSSuper*			pSuperviseur;
	  NSSmallBrother*	pBigBoss;
     BOOL				bTemporaire;	// Constitution d'un code avec marqueurs de lésions

	  char				szNom[BB_CODE_LEN+1]; // Code de la fiche Paradox

	  string 			sEtiquette; 			 // Etiquette de la branche

	  string 			sCode;     				 // Codage de l'item
	  string 			sPonct;    				 // Ponctuation terminale

	  // constructeurs
	  BBFilsPhraseItem(NSSmallBrother*	pBig = 0);
	  BBFilsPhraseItem(const BBFilsPhraseItem& src);

	  BBFilsPhraseItem& operator=(BBFilsPhraseItem src);
	  int operator == ( const BBFilsPhraseItem& o );
};

//---------------------------------------------------------------------------
// Matrice dynamique de BBFilsPhraseItem
//---------------------------------------------------------------------------
typedef TArrayAsVector<BBFilsPhraseItem> BBFilsPhraseItemArray;




class BBFilsPhraseArray : public BBFilsPhraseItemArray
{
public :
	// Constructeurs
  	BBFilsPhraseArray() : BBFilsPhraseItemArray(0, 0, 1) {}
	BBFilsPhraseArray(BBFilsPhraseArray& rv);
	// Destructeur
	virtual ~BBFilsPhraseArray();
};

//---------------------------------------------------------------------------
// Class BBPhraseItem
//
// Cette structure sert à construire la phrase codée
//---------------------------------------------------------------------------
class BBPhraseItem {

	public :

	//NSSuper*			pSuperviseur;
	NSSmallBrother*	pBigBoss;
	BOOL				bTemporaire;	// Constitution d'un code avec marqueurs de lésions

 //	NSLesion* 		pLesion;       // Pointeur sur la lésion de travail

	string 			sCode;     		// Codage de l'item
	string 			sPonct;    		// Ponctuation terminale

	string 			sLocalisation; // Position dans l'arbre
	string 			sPhraseLocali; // Position pour la phrase codée

	BBItemData* 	pDonnees;		// Données de la fiche PARADOX

	BBPhraseItem*  pBBItemPere;  	// Pointeur sur le BBPhraseItem père
	BBPhraseItem*	pBBItemFils;  	// Pointeur sur le BBPhraseItem fils

	BBFilsPhraseArray	aBBPhraseItemFils;  	// Pointeurs sur les fils

	int				iFilsActif;		// Numéro du BBPhraseItem fils qui a la main

	NSBBFonction*	pNSFonction;	// Pointeur sur l'éventuelle NSFonction

	int				iAvantLes;		// Vrai lorsqu'on est en aval de la lésion

	// Constructeurs et destructeur
	BBPhraseItem(BBPhraseItem* pere = 0, NSSmallBrother* pBig = 0);

	BBPhraseItem(const BBPhraseItem& src);
	~BBPhraseItem();

	// Fonctions de mise en place de l'arborescence des BBItems
	int creer(char* codeFiche);			// Crée les fils, rempli aBBItemFils
	int creer(BBFilsPhraseItem* pFilsPhraseItem);

	int activer();								// Lance la BBItem

	int appeler(char* szNom);				// Appelle la fiche BB

	int  createurLesion(); 	 // Indique si le BBItem est ou non créateur de Lesion
	int  uniciteLesion();  	 // Indique si la Lesion est unique ou non
	int  lienFonction();	  	 // Indique si le BBItem est rattaché à une fonction
	int  triComplet();
	int  tri();

	// Surcharge de l'opérateur d'affectation
	BBPhraseItem& operator=(BBPhraseItem src);

};

//---------------------------------------------------------------------------
//  Classe des fonctions Big Brother
//
//  Cette classe sert à mettre en place les fonctions qui interviennent au
//  sein de l'arborescence Big Brother
//  Elle est homogène à NSDialog
//----------------------------------------------------------------------------
/*class NSBBPhraseFonction
{
	public :

	BBPhraseItem*		pBBItem;			// BBPhraseItem créateur de la fonction
	string				sNomFonction;
	BOOL (FAR *pAdresseFct) (NSBBFonction far *, NSSuper far*, int, int far*);

	// Constructeurs
	NSBBFonction(string sNomFonct, BBPhraseItem* pBBItemPere);

	// Destructeur
	virtual ~NSBBFonction();

	int execute(int iParam, int* piValeur, string sEtiquette = "", NSControle* pNSCtrl = 0);
}; */

#endif