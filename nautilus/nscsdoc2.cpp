//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <iostream.h>
#include <cstring.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsdecode.h"
#include "nsbb\nsbb.h"
#include "nautilus\nsrechd2.h"

#include "nautilus\nscsdoc.h"
#include "nautilus\nscsvue.h"

// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSCSDocument --------------------------
// --------------------------------------------------------------------------

//
// Document server registration
//
/*BEGIN_REGISTRATION(CRDocReg)
  REGDATA(progid,     "Nautilus.Server.Consult")
  REGDATA(description,"Nautilus (Report Server) Consultation")
  REGDATA(menuname,   "Consultation")
  REGDATA(extension,  "NSR")
  REGDATA(docfilter,  "*.NSR")
  REGDOCFLAGS(dtAutoOpen | dtAutoDelete | dtUpdateDir | dtCreatePrompt | dtRegisterExt)
  REGDATA(debugger,   "tdw")
  REGDATA(insertable, "")
  REGDATA(verb0,      "&Edit")
  REGDATA(verb1,      "&Open")
  REGFORMAT(0, ocrEmbedSource,  ocrContent,  ocrIStorage,  ocrSet)
  REGFORMAT(1, ocrMetafilePict, ocrContent,  ocrMfPict|ocrStaticMed, ocrGet)
END_REGISTRATION

DEFINE_DOC_TEMPLATE_CLASS(NSCSDocument, NSCSView, NSCSTemplate);
NSCSTemplate CRTpl(CRDocReg);*/

/*BOOL
nouveauCompteRendu(NSSuper *pSuper)
{
	NSCRReadOnlyTemplate* DocVisuTemplate;
	DocVisuTemplate = new NSCRReadOnlyTemplate(CRDocReg);
	NSCRDocument* pDocVisuDoc = new NSCRDocument((TDocument*)0, pSuper);
	pDocVisuDoc->SetTemplate(DocVisuTemplate);
	DocVisuTemplate->CreateView(*pDocVisuDoc);
	//NSCRReadOnlyView* pVue = DocVisuTemplate->ConstructView(*pDocVisuDoc);
	//pVue->Create();
	return TRUE;
}

BOOL
afficheCompteRendu(string *pCRString, NSSuper *pSuper)
{
	NSCRReadOnlyTemplate* DocVisuTemplate = new NSCRReadOnlyTemplate("Compte rendu","",0,"",dtAutoDelete|dtUpdateDir);
	NSCRDocument*			 pDocVisuDoc 	  = new NSCRDocument((TDocument*)0, pSuper);
	*(pDocVisuDoc->DonneCR()) = *pCRString;
	//DocVisuDoc->sTitre		 = "Compte rendu d'Echographie cardiaque";
	pDocVisuDoc->SetTemplate(DocVisuTemplate);
	DocVisuTemplate->CreateView(*pDocVisuDoc);
} */

bool
afficheCompteRendu(NSDocumentInfo& Document, NSSuper *pSuper)
{
	/*NSCRReadOnlyTemplate* DocVisuTemplate = new NSCRReadOnlyTemplate("Compte rendu","",0,"",dtAutoDelete|dtUpdateDir);
	NSCRDocument*			 DocVisuDoc 	  = new NSCRDocument(0, &Document, pSuper);
	DocVisuDoc->SetTemplate(DocVisuTemplate);
	DocVisuTemplate->CreateView(*DocVisuDoc);*/
}

//---------------------------------------------------------------------------
//  Function: NSCRDocument::NSCRDocument(TDocument* parent = 0,
//													  NSDocumentInfo* pDocumentInfo)
//
//  Arguments:
//            parent 		 : TDocument parent (?)
//				  pDocumentInfo : pointeur sur l'objet NSDocumentInfo qui permet
//										de créer le NSCRDocument
//  Description:
//            Constructeur
//  Returns:
//            Rien
//---------------------------------------------------------------------------
NSCSDocument::NSCSDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
									NSDocumentInfo* pDocHtmlInfo, NSSuper* pSuper)
				 :TDocument(parent), NSRoot(pSuper)
{
	pPatPathoArray = new NSPatPathoArray(pSuper);

	sTitre		 = "";
	pBigBoss		 = 0;
   if (pDocumentInfo != 0)
	{
		pDocInfo = new NSDocumentInfo(*pDocumentInfo);

		strcpy(chAffiche, pDocInfo->pDonnees->nom);
		ote_blancs(chAffiche);
		if (strcmp(pDocInfo->pDonnees->creation, "        ") != 0)
		{
			strcat(chAffiche, " du ");
			donne_date(pDocInfo->pDonnees->creation, dateAffiche, 0);
			strcat(chAffiche, dateAffiche);
		}
		SetTitle(chAffiche);
	}
	else
		pDocInfo = 0;
   pHtmlInfo = pDocHtmlInfo;
   if (pDocInfo)
		Open(0, "");
}

NSCSDocument::NSCSDocument(TDocument* parent, NSSuper* pSuper)
				 :TDocument(parent), NSRoot(pSuper)
{
	pDocInfo = 0;
   pHtmlInfo = 0;
   pPatPathoArray = new NSPatPathoArray(pSuper);
	sTitre		 = "";
	pBigBoss		 = 0;
}

//---------------------------------------------------------------------------
//  Description : Constructeur copie
//---------------------------------------------------------------------------
NSCSDocument::NSCSDocument(NSCSDocument& rv)
				 :TDocument(rv.GetParentDoc()), NSRoot(rv.pSuper)
{
	if (rv.pDocInfo)
   	pDocInfo = new NSDocumentInfo(*(rv.pDocInfo));
   else pDocInfo = 0;

   if (rv.pHtmlInfo)
   	pHtmlInfo = new NSDocumentInfo(*(rv.pHtmlInfo));
   else pHtmlInfo = 0;

   if (rv.pPatPathoArray)
   	pPatPathoArray = new NSPatPathoArray(*(rv.pPatPathoArray));
   else
   	pPatPathoArray = 0;

	if (rv.pBigBoss)
		pBigBoss = new NSSmallBrother(*(rv.pBigBoss));
	else
   	pBigBoss = 0;

	sTitre 	  = rv.sTitre;
}

//---------------------------------------------------------------------------
//  Description : Destructeur
//---------------------------------------------------------------------------
NSCSDocument::~NSCSDocument()
{
}

//---------------------------------------------------------------------------
//  Description : Operateur =
//---------------------------------------------------------------------------
NSCSDocument&
NSCSDocument::operator=(NSCSDocument& src)
{
   TDocument *dest, *source;

   // Appel de l'operateur = de NSRefDocument
   // (recopie des arguments de la classe NSRefDocument)

   dest = this;
   source = &src;
   *dest = *source;

   if (src.pDocInfo)
   	pDocInfo = new NSDocumentInfo(*(src.pDocInfo));
   else pDocInfo = 0;

   if (src.pHtmlInfo)
   	pHtmlInfo = new NSDocumentInfo(*(src.pHtmlInfo));
   else pHtmlInfo = 0;

   if (src.pPatPathoArray)
   	pPatPathoArray = new NSPatPathoArray(*(src.pPatPathoArray));
   else
   	pPatPathoArray = 0;

	if (src.pBigBoss)
		pBigBoss = new NSSmallBrother(*(src.pBigBoss));
   else pBigBoss = 0;

	sTitre 	  = src.sTitre;

	return *this;
}

//---------------------------------------------------------------------------
//  Function: NSCSDocument::Open(int mode, const char far* path=0)
//
//  Description : Ouvre le document en mettant le CR dans pPatPathoArray
//
//  Returns:		true si tout s'est bien passé, false sinon
//---------------------------------------------------------------------------
bool
NSCSDocument::Open(int mode, const char far* path)
{
	pPatPathoArray->vider();

	if (!pDocInfo)
		return true;
   //
	// Création d'un objet d'accès au fichier CR
	//
	NSPatPatho* pCR = new NSPatPatho(pSuper);
	//
	// Ouverture du fichier
	//
	pCR->lastError = pCR->open();
	if (pCR->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier de données.", 0, pCR->lastError);
		delete pCR;
		return false;
	}
   //
   // Recherche de la première fiche
   //
   string cle = string(pDocInfo->pDonnees->codePatient) +
   				 string(pDocInfo->pDonnees->codeDocument) +
                string(BASE_LOCALISATION_LEN, ' ');

   pCR->lastError = pCR->chercheClef(&cle,
   											 "",
                                     0,
                                     keySEARCHGEQ,
                                     dbiWRITELOCK);
   if (pCR->lastError != DBIERR_NONE)
	{
		erreur("Il est impossible de trouver ce compte rendu dans le fichier de données.", 0, pCR->lastError);
      pCR->close();
		delete pCR;
		return false;
	}
   pCR->lastError = pCR->getRecord();
   if (pCR->lastError != DBIERR_NONE)
	{
		erreur("Le fichier de données est défectueux.", 0, pCR->lastError);
      pCR->close();
		delete pCR;
		return false;
	}
   if ((strcmp(pDocInfo->pDonnees->codePatient, pCR->pDonnees->codePatient) != 0) ||
   	 (strcmp(pDocInfo->pDonnees->codeDocument, pCR->pDonnees->codeDocument) != 0))
   {
		erreur("Il est impossible de trouver ce compte rendu dans le fichier de données.", 0, pCR->lastError);
      pCR->close();
		delete pCR;
		return false;
	}
   //
   // On avance dans le fichier tant que les fiches trouvées appartiennent
   // à ce compte rendu
   //
   while ((pCR->lastError != DBIERR_EOF) &&
   		 (strcmp(pDocInfo->pDonnees->codePatient, pCR->pDonnees->codePatient) == 0) &&
          (strcmp(pDocInfo->pDonnees->codeDocument, pCR->pDonnees->codeDocument) == 0))
	{
      pPatPathoArray->push_back(new NSPatPathoInfo(pCR));
      pCR->lastError = pCR->suivant(dbiWRITELOCK);
      if ((pCR->lastError != DBIERR_NONE) && (pCR->lastError != DBIERR_EOF))
		{
			erreur("Pb d'acces a la PatPatho suivante.", 0, pCR->lastError);
      	pCR->close();
			delete pCR;
			return false;
		}

      if (pCR->lastError != DBIERR_EOF)
      {
   		pCR->lastError = pCR->getRecord();
   		if (pCR->lastError != DBIERR_NONE)
			{
				erreur("Le fichier de données est défectueux.", 0, pCR->lastError);
      		pCR->close();
				delete pCR;
				return false;
			}
      }
   }
   //
   // Fermeture du fichier
   //
   pCR->close();
   delete pCR;

   sTitre = (char*)(pDocInfo->pDonnees->nom);
   sTitre.strip(sTitre.Both, ' ');
   /*if (strcmp(pDocInfo->pDonnees->creation, "        ") != 0)
   {
   	sTitre += " du ";
      donne_date(pDocInfo->pDonnees->creation, message, 0);
      sTitre += message;
   } */
   sTitre += " (Compte rendu)";

	return true;
}

bool
NSCSDocument::Close()
{
	//sCompteRendu = "" ;
	return true;
}

bool
NSCSDocument::CanClose()
{
	return TDocument::CanClose();
}

void
NSCSDocument::SetTitle(LPCSTR title)
{
	TDocument::SetTitle(sTitre.c_str());
}

//---------------------------------------------------------------------------
//  Fonction : 	NSCSDocument::enregistre()
//
//  Arguments :	Aucun
//
//  Description :	Enregistre le compte rendu
//
//  Retour :		Rien
//---------------------------------------------------------------------------
bool
NSCSDocument::enregistre()
{
	int  i, j, k, l;
   bool existeInfo;

   // ancienne version
	// if (sCompteRendu == "") return FALSE;
	// if (strlen(sCompteRendu.c_str()) == 0) return FALSE;
	//
	// Création d'un objet d'accès au fichier CR
	//
	NSPatPatho* pCR = new NSPatPatho(pSuper);
	//
	// Ouverture du fichier
	//
	pCR->lastError = pCR->open();
	if (pCR->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier de données.", 0, pCR->lastError);
		delete pCR;
		return false;
	}
	//
	// Si c'est un nouveau compte rendu
	//
	if (pDocInfo == 0)
   {
   	existeInfo = Referencer("CS030");
      if (!existeInfo)
      {
      	pCR->close();
   		delete pCR;
   		return existeInfo;
      }

   }
   //
	// Si c'est un ancien compte rendu, on efface d'abord les anciennes données
	//
   else
   {
   	string cle = string(pDocInfo->pDonnees->codePatient) +
      				 string(pDocInfo->pDonnees->codeDocument) +
                   string(BASE_LOCALISATION_LEN, ' ');
      bool effacer;
      while (effacer)
      {
         effacer = false;
      	pCR->lastError = pCR->chercheClef(&cle,
      											 	 "",
                                           0,
                                           keySEARCHGEQ,
                                           dbiWRITELOCK);
			if (pCR->lastError == DBIERR_NONE)
      	{
         	pCR->lastError = pCR->getRecord();
				if (pCR->lastError == DBIERR_NONE)
         		if ((strcmp(pDocInfo->pDonnees->codePatient, pCR->pDonnees->codePatient) == 0) &&
               	 (strcmp(pDocInfo->pDonnees->codeDocument, pCR->pDonnees->codeDocument) == 0))
               	effacer = true;
      	}
         if (effacer)
         {
         	pCR->lastError = pCR->deleteRecord();
            if (pCR->lastError != DBIERR_NONE)
            	effacer = false;
         }
      }
   }
   //
   // Stockage de la PatPathoArray dans la base
   //
   for (PatPathoIter iJ = pPatPathoArray->begin(); iJ != pPatPathoArray->end(); iJ++)
   {
   	*(pCR->pDonnees) = *((*iJ)->pDonnees);
      strcpy(pCR->pDonnees->codePatient, pDocInfo->pDonnees->codePatient);
      strcpy(pCR->pDonnees->codeDocument, pDocInfo->pDonnees->codeDocument);

      pCR->lastError = pCR->appendRecord();
      if (pCR->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la sauvegarde des données.", 0, pCR->lastError);
         delete pCR;
         return false;
      }
   }
   //
   // Fermeture de la base
   //
   pCR->close();
   delete pCR;
   return existeInfo;
}


