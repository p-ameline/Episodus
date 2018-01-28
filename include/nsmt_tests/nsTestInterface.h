#if !defined(__NSTESTINTERFACE_H)#define __NSTESTINTERFACE_H

#include <owl\listwind.h>

//---------------------------------------------------------------------
//---------------------------------------------------------------------
class NSTestView : public TListWindow
{
	public:

  	NSContexte  *_pContexte ;

    NSTestView(NSContexte *pCtx) ;
	  ~NSTestView() ;

    // Fonction d'initialisation
	  void SetupWindow() ;

    void displayMessage(const char* pSzMessage, const char* pSzResult) ;

  protected:

    int _iLineNb ;

  DECLARE_RESPONSE_TABLE(NSTestView) ;
};


#endif // __NSTESTINTERFACE_H

