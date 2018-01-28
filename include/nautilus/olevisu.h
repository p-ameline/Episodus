// nsvisual.h : Objet de visualisation des fichiers html statiques - RS Fevrier 1998
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(__NSVISUAL_H)
#define 		 __NSVISUAL_H

#include "nautilus\nsdocref.h"
#include "owl\oledoc.h"
#include "owl\oleview.h"

class NSVisualDocument : public TOleDocument
{
    public :

        NSRefDocument*	pDocRef;		// pointeur sur document de référence
      									// (pour éviter un double héritage)

   	    NSVisualDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
      						NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx);
		NSVisualDocument(TDocument* parent = 0, NSContexte* pCtx = 0);
		~NSVisualDocument();

        // implement virtual methods of TDocument
    	// bool   Open(int mode, const char far* path=0);
    	// bool   Close();
};

class NSVisualView : public TOleView
{
    public :
        // Données de visualisation
        NSVisualDocument*	pDoc;		// pointeur sur TOleDocument
        string 				sUrl; 		// url de référence
        string 				sHtml;     	// fichier html à visualiser
        TOcControl* 		Ocx;		// controle ocx
        TIWebBrowserProxy 	Control;    // Objet proxy WebBrowser

   	    NSVisualView(NSVisualDocument& doc, TWindow *parent = 0);
        ~NSVisualView();

        // Redéfiniton des fonctions virtuelles de TView
        static LPCSTR StaticName() { return "Visualisateur Html"; }
        const char*	  GetViewName() { return StaticName(); }
        TWindow*	  GetWindow();

        // Fonctions de la VisualView
        void SetupWindow();
        void SetSizeOcx(TSize newSize);
        void Navigate();
        bool GenereHtml();
        void CmPublier();

    DECLARE_RESPONSE_TABLE(NSVisualView);
};

// Déclaration de la template associée au modèle document/vue visualisation

BEGIN_REGISTRATION(DocHtmlReg)
  REGDATA(progid,     "Nautilus.DocumentHtml.1")
  REGDATA(description,"Nautilus 3.0 Document html")
  REGDATA(extension,  "HTM")
  REGDATA(docfilter,  "*.htm")
  REGDOCFLAGS(dtAutoOpen | dtAutoDelete | dtUpdateDir | dtCreatePrompt | dtRegisterExt)
  REGFORMAT(0, ocrEmbedSource,  ocrContent,  ocrIStorage, ocrGet)
  REGFORMAT(1, ocrMetafilePict, ocrContent,  ocrMfPict|ocrStaticMed, ocrGet)
  REGFORMAT(2, ocrBitmap, ocrContent,  ocrGDI|ocrStaticMed, ocrGet)
  REGFORMAT(3, ocrDib, ocrContent,  ocrHGlobal|ocrStaticMed, ocrGet)
END_REGISTRATION

DEFINE_DOC_TEMPLATE_CLASS(NSVisualDocument, NSVisualView, NSVisualTemplate);

#endif		// __NSVISUAL_H
