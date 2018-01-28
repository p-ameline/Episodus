//---------------------------------------------------------------------------
#ifndef __NSV_PATLNK_H
#define __NSV_PATLNK_H

#include "nsmt_version\nsv_transcode.h"

//
// Taille des champs du fichier NSPATLNK
//
#define PATLINK_QUALIFIE_LEN 	   NSV_PAT_NSS_LEN + NSV_DOC_CODE_DOCUM_LEN + NSV_PPD_NOEUD_LEN   // Arrivée de la flêche / Arrow end
#define PATLINK_LIEN_LEN 	  	   2   // Lien / Link
#define PATLINK_LIEN_LEVEL_LEN 	   2   // Niveau de certitude du lien / Link's level of certitude
#define PATLINK_QUALIFIANT_LEN     PATLINK_QUALIFIE_LEN   // Origine de la flêche / Arrow start
#define PATLINK_TRAN_NEW_LEN       TRAN_CODE_LEN   // Session de création / Creation session
#define PATLINK_TRAN_DEL_LEN       TRAN_CODE_LEN   // Session de destruction / Delete session
#define PATLINK_CAUSE_LEN    	   1   // Cause / Reason

//
// Indice des champs du fichier NSPATLNK
//
#define PATLINK_QUALIFIE_FIELD 	  	1
#define PATLINK_LIEN_FIELD 	  	    2
#define PATLINK_QUALIFIANT_FIELD    3
#define PATLINK_TRAN_NEW_FIELD      4
#define PATLINK_TRAN_DEL_FIELD    	5
#define PATLINK_CAUSE_FIELD    	    6
#define PATLINK_LIEN_LEVEL_FIELD    7

// **************************** Table NSPATLNK.DB ****************************

//---------------------------------------------------------------------------
//  Classe NSVPatLinkData
//
// Objet contenant les données
//---------------------------------------------------------------------------
//    
class NSVPatLinkData
{
	public :

     	char qualifie[PATLINK_QUALIFIE_LEN + 1];
     	char lien[PATLINK_LIEN_LEN + 1];
        char lien_level[PATLINK_LIEN_LEVEL_LEN + 1];
     	char qualifiant[PATLINK_QUALIFIANT_LEN + 1];
     	char tranNew[PATLINK_TRAN_NEW_LEN + 1];
     	char tranDel[PATLINK_TRAN_DEL_LEN + 1];
     	char cause[PATLINK_CAUSE_LEN + 1];

	  	NSVPatLinkData();	  	NSVPatLinkData(NSVPatLinkData& rv);
     	~NSVPatLinkData();

	  	NSVPatLinkData& operator=(NSVPatLinkData src);
	  	int 		   operator==(NSVPatLinkData& o);

     	void metAZero();
};

class NSVPatLink;

//---------------------------------------------------------------------------
//  Classe NSVPatLinkInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSVPatLinkInfo
{
	public :

		NSVPatLinkData*  pDonnees;
        bool            isDirty;

		NSVPatLinkInfo();
		NSVPatLinkInfo(NSVPatLink*);
		NSVPatLinkInfo(NSVPatLinkInfo& rv);
      	~NSVPatLinkInfo();

        void    setDirty(bool flag) { isDirty = flag; }
        bool    getDirty()          { return isDirty; }

		NSVPatLinkInfo& operator=(NSVPatLinkInfo src);
		int 		   operator==(NSVPatLinkInfo& o);
};

//---------------------------------------------------------------------------
//  Classe NSVPatLink
//
// Objet dérivé de NSFiche servant aux accès de base de données
//---------------------------------------------------------------------------
//
class NSVPatLink : public NSVPatLinkInfo, public NSFiche
{
    public :

        int iTypeLink;

	  	NSVPatLink(NSSuper* pSuper, int iType = graphPerson);
     	NSVPatLink(NSVPatLink& rv);
	  	~NSVPatLink();

	  	void alimenteFiche();
	  	void videFiche();
	  	DBIResult open(string pathbase);

     	NSVPatLink& operator=(NSVPatLink src);
     	int 	   operator==(NSVPatLink& o);
};

//
// Définition de NSCodPrestArray (Array de NSCodPrestInfo(s))
//

typedef vector<NSVPatLinkInfo*> NSVPatLkArray;
typedef NSVPatLkArray::iterator NSVPatLinkIter;

class NSVPatLinkArray : public NSVPatLkArray
{
	public :

		// Constructeurs
		NSVPatLinkArray() : NSVPatLkArray() {}
		NSVPatLinkArray(NSVPatLinkArray& rv);
		// Destructeur
		virtual ~NSVPatLinkArray();
   		void vider();

        NSVPatLinkArray& operator=(NSVPatLinkArray src);
        int 	        operator==(NSVPatLinkArray& o);
};

#endif
