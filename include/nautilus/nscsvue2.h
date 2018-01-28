#if !defined(__NSCSVUE_H)
#define 		 __NSCSVUE_H

#include <owl\docmanag.h>
#include <owl\docview.h>
// #include <owl\pch.h>
#include <owl\decframe.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\applicat.h>

#include "nautilus\nsdocref.h"
#include "nautilus\nautilus.rh"
#include "nautilus\richedap.h"
#include "nsbb\nstrnode.h"
#include "nautilus\nscsdoc.h"


// Constantes
const int IDTrView = 0x100;
/*const int UnChk   = TCommandEnabler::Unchecked;
const int Chk		= TCommandEnabler::Checked;
*/
#define _NSTTXCLASS _USERCLASS

// Classe document ttx

/*class _NSTTXCLASS NSCSDocument : public NSRefDocument
{
	public:

      char far nomDoc[255];       // fichier texte associé

      NSTtxDocument(TDocument *parent, NSDocumentInfo *pDocumentInfo,
      					NSDocumentInfo* pDocHtmlInfo = 0, NSSuper *pNSSuperviseur = 0);

      NSTtxDocument(TDocument *parent = 0, NSSuper *pNSSuperviseur = 0);

   	~NSTtxDocument() {}

      void SetDocName(char *name);

      // Méthodes virtuelles de TDocument
      bool Open(int mode, const char far *path = 0);
      bool Close();
      bool Commit(bool force = false);
      bool Revert(bool clear = false);
      bool IsOpen();
};
  */
class _TYPEDECLASSE NSCsVue : public TView, public NSTreeWindow
{
	public:
      char far nomDoc[255];       // fichier texte associé
	 	string			NomDuFichier;
    	NSSmallBrother* pBigBoss;
    	// Pointeurs
      NSCSDocument*	  pDoc;			// Pointeur sur document TTX
    	TControlBar*	  pToolBar;		// Barre d'outils
    	TPrinter*		  pPrinter;   	// Printer

	 	NSCsVue(NSCSDocument& doc, TWindow* parent = 0);
	 	~NSCsVue();
	 	void 		lanceConsult();
	 	void 		CmEnregistre();
	 	static const char far* StaticName() { return "Affichage Consultation"; }
	 	const char far* GetViewName(){return StaticName();}
	 	// Fonction d'initialisation
      TWindow*	GetWindow();
	 	void 		SetupWindow();
      DECLARE_RESPONSE_TABLE(NSCsVue);
};

/* Table de recensement du document (modèle OLE)
BEGIN_REGISTRATION(DocReg)

   REGDATA(progid, "Serveur-Conteneur TTX")
   REGDATA(menuname, "RichEdit")
   REGDATA(description, "OWL Serveur-Conteneur TTX")
   REGDATA(extension, "TTX")
   REGDATA(docfilter, "*.rtf | *.txt")
   REGDOCFLAGS(dtAutoOpen | dtAutoDelete | dtUpdateDir | dtCreatePrompt | dtRegisterExt)

   REGDATA(insertable, "")
   REGDATA(verb0, "&File")
   REGDATA(verb1, "&Edit")
   REGDATA(verb2, "&Search")
   REGDATA(verb3, "&View")
   REGDATA(verb4, "F&ormat")
   REGDATA(verb5, "&Help")

   REGFORMAT(0, ocrEmbedSource, ocrContent, ocrIStorage, ocrGet)
   REGFORMAT(1, ocrMetafilePict, ocrContent, ocrMfPict, ocrGet)
   REGFORMAT(2, ocrBitmap, ocrContent, ocrGDI | ocrStaticMed, ocrGet)
   REGFORMAT(3, ocrDib, ocrContent, ocrHGlobal | ocrStaticMed, ocrGet)
   REGFORMAT(4, ocrLinkSource, ocrContent, ocrIStream, ocrGet)

END_REGISTRATION

// Table de recensement de la vue
BEGIN_REGISTRATION(ViewReg)
	REGDATA(description, "Vue TTX")
   REGDATA(extension, "TTX")
   REGDATA(docfilter, "*.rtf;*.txt")
   REGDOCFLAGS(dtAutoDelete | dtUpdateDir)
END_REGISTRATION
*/

#if defined __NSCSVUE_CPP
	DEFINE_DOC_TEMPLATE_CLASS(NSCSDocument,NSCsVue,NSCSTemplate);
#else
	typedef TDocTemplateT<NSCSDocument,NSCsVue> NSCSTemplate;
#endif

#endif		// __NS_TTX_H



