#if !defined(__NSTESTCONTROLER_H)#define __NSTESTCONTROLER_H

#include <owl\listwind.h>

class NSTestView ;

//---------------------------------------------------------------------
//---------------------------------------------------------------------
class NSTestControler
{
	public:

    enum STATUSTYPE { validTest = 0, errorTest } ;

  	NSContexte  *_pContexte ;
    NSTestView  *_pView ;

    NSTestControler(NSContexte *pCtx, NSTestView *pView) ;
	  ~NSTestControler() ;

    void displayMessageBlock(string *psMessage) ;
    void displayMessage(string *psMessage) ;
    void displayMessage(const char* pSzMessage, const char* pSzResult) ;
    void displayMessage(const char* pSzMessage, STATUSTYPE iStatus) ;
    void displayMessage(const string sMessage, STATUSTYPE iStatus) ;

    void resultForString(string sResult, string sTheorical, string sDescription) ;
    void resultForDouble(double dResult, double dTheorical, string sDescription) ;
    void resultForInt(int dResult, int dTheorical, string sDescription) ;
    void resultForBool(bool bResult, bool bTheorical, string sDescription) ;
    void resultTrue(bool bResult, string sDescription) { resultForBool(bResult, true, sDescription) ; }
    void resultFalse(bool bResult, string sDescription) { resultForBool(bResult, false, sDescription) ; }
};


#endif // __NSTESTCONTROLER_H

