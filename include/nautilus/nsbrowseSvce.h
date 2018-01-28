// NSBROWSE.H : D�finition des objets dialogues contenant ie4
#if !defined(__NSBROWSESVCE_H)
#include <owl\edit.h>

// #include "nautilus\nssuper.h"
#include "partage\NTArray.h"
#include "nsbb\nsutidlg.h"

class NSContexte;

class TWebServiceForm ;

/////////////////////////////////////////////////////////////////////////////////
class NSWebServiceWindow : public TWindow
{
	public:

		bool        bNavOk ;
		bool        bCanClose ;
    string      sStartingURL ;
  	string			sHtml ;
    NSContexte* pContexte ;

    NSWebServiceWindow(TWindow* parent, string sFirstURL, NSContexte* pCtx) ;
    ~NSWebServiceWindow() ;

    void EvClose() ;
    void PerformCreate(int menuOrId) ;
    void MakeVisible() ;
    void SetupWindow() ;
    void Navigate(string sURL) ;
    void NavigateErrorEvent(int iStatusCode, string sURL) ;
    bool InitInfosFichier() ;
    void CmAnnuler() ;

    void   Action() ;
    string getHTMLsource(IDispatch* pDocument) ;
    void   captureInformation() ;
    void   takeCapturedEntries(string *pEntry, string* pPath, string* pValue, string* pUnit) ;
    string getDataFromTag(string *pEntry, string sBeginTag, string sEndTag) ;

	protected:

		bool IsTabKeyMessage(MSG &msg) ;

	private:

		TWebServiceForm* Form ;
	DECLARE_RESPONSE_TABLE(NSWebServiceWindow) ;
};

// Classe TMDIChild pour la red�finition de EvClose
/////////////////////////////////////////////////////////
class NSWebServiceChild : public TMDIChild, public NSRoot
{
	public :

		NSWebServiceWindow* pClient ;

		NSWebServiceChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSWebServiceWindow* clientWnd = 0) ;
		~NSWebServiceChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSWebServiceChild) ;
};

#endif
// fin de nsbrowseSvce.h
