// nsPilotProxy.h : Définition des objets dialogues contenant ie4/////////////////////////////////////////////////////////////
#if !defined(__NSPILOTPROXY_H)#define __NSPILOTPROXY_H
#include <owl\mdichild.h>class NSSuper ;

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "partage\ns_vector.h"

//---------------------------------------------------------------------------//// Classes controle browser :////---------------------------------------------------------------------------
// Classe TWindow offrant une interface durable au Pilot/////////////////////////////////////////////////////////////////////////////////
class NSPilotProxyWindow : public TWindow
{
	public:

    NSSuper* _pSuper ;

    NSPilotProxyWindow(TWindow* parent, NSSuper* pSuper) ;
    ~NSPilotProxyWindow() ;

    void SetupWindow() ;

    void getPerson(string sID, PIDSTYPE iTypePids) ;

    void Hide() ;

  protected:

    void EvTimer(uint timerId) ;
    void getPersonSynchro(string sParams) ;
    bool CanClose() ;

    vector<string> _aToDo ;

	DECLARE_RESPONSE_TABLE(NSPilotProxyWindow) ;
};

// Classe TMDIChild pour la redéfinition de EvClose
/////////////////////////////////////////////////////////
class NSPilotProxyChild : public TMDIChild, public NSSuperRoot
{
	public :

		NSPilotProxyWindow* _pClient ;

		NSPilotProxyChild(NSSuper* pCtx, TMDIClient& parent, const char far* title = 0, NSPilotProxyWindow* clientWnd = 0) ;
		~NSPilotProxyChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSPilotProxyChild) ;
};

#endif // #define __NSPILOTPROXY_H
// end nsPilotProxy.h///////////////////////////////////////////////////////////////

