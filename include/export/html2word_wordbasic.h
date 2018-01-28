#if !defined(__HTML2WORD_H)#define __HTML2WORD_H

#include <owl\docmanag.h>
#include <owl\docview.h>
#include <owl\mdichild.h>
#include <owl\decframe.h>
#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\applicat.h>
#include <owl\richedit.h>
#include <owl\richedpr.h>
#include <owl\choosefo.h>
#include <richedit.h>

#include "nautilus\nsdocref.h"#include "nautilus\nautilus.rh"
#include "nautilus\richedap.h"
#include "nsbb\nsednum.h"

//#define MAXCARS 100000      // Limite du texte pour RichEdit
// #if defined(_DANSTXTDLL)// #define _NSTTXCLASS __export
// #else
// #define _NSTTXCLASS __import
// #endif

// Constantes//const int IDRichEd = 0x100;
//const int UnChk    = TCommandEnabler::Unchecked;
//const int Chk	   = TCommandEnabler::Checked;

#define _NSTTXCLASS _USERCLASS
class NSTtxDocumentExp;        // classe générique de documents texte nautilusclass TWordBasicProxy;      // classe d'automation Word.Basic (par ProxyWizard)
class TAutoWordFormEx;        // classe "Form" permettant de faire une vue - dialogue

// Vue pour les documents Word : windowview - dialogue d'automationclass NSAutoWordViewExp : public TWindowView
{
	public :

		NSUtilDialog*       pDialog;        // on garde un dialogue comme toolbox		bool                bSessionEnCours;
		TWordBasicProxy*    pControl;
		wchar_t             wcWindowName[255];
		NSTtxDocumentExp*   pDocTtx;

		NSAutoWordViewExp(NSTtxDocumentExp& doc, TWindow *parent = 0);		~NSAutoWordViewExp();

		// fonction virtuelles de TView		static LPCSTR StaticName()  { return "NSAutoWordViewExp"; }
		const char*   GetViewName() { return StaticName(); }
		TWindow*      GetWindow()   { return (TWindow*) this; }

		void PerformCreate(int menuOrId) ;		void MakeVisible() ;
		void SetupWindow() ;
		void EvClose() ;
		bool CanClose() ;
		void FillStartupInfo(LPSTARTUPINFO psi) ;

		void CmLancer() ;    void CmEditer() ;		bool saveHTMLtoWORD(string sWordName) ;

	protected:
		bool IsTabKeyMessage(MSG &msg);
		bool PreProcessMsg(MSG &msg);

	private:
		TAutoWordFormEx* Form;
	DECLARE_RESPONSE_TABLE(NSAutoWordViewExp);};

class NSTtxDocumentExp : public TDocument, public NSRoot
{
	public:

		NSTtxDocumentExp(TDocument *parent = 0, NSContexte *pCtx = 0) ;		~NSTtxDocumentExp() {}

		void   SetNomFichier(string sFich)     { _sNomFichier     = sFich ; }		void   SetFichierExterne(string sFich) { _sFichierExterne = sFich ; }    string GetNomFichier()     { return _sNomFichier ; }		string GetFichierExterne() { return _sFichierExterne ; }		string GetDocExt() ;

		// Méthodes virtuelles de TDocument		bool Open(int mode, const char far *path = 0) ;
		bool Close() ;

		bool IsOpen() ;  protected:    string _sNomFichier ;       // fichier texte associé		string _sFichierExterne ;};

#endif		// __HTML2WORD_H
