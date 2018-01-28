// -----------------------------------------------------------------------------
// nsbackup.h
// -----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Author: pameline $
// $Date: 2011/03/29 16:37:25 $
// -----------------------------------------------------------------------------
// FLP - aout 2003
// -----------------------------------------------------------------------------

#ifndef __NSUPDATE_H__
# define __NSUPDATE_H__

# include "nautilus\nssuper.h"
# include "nsepisod\nsldvuti.h"
# include "nsoutil\nsUpdateDlg.h"
# include "curl\nscurl.h"

class NSUpdate : public NSRoot
{
	public :

    NSUpdateParameters updateParameters ;

    MemUnaFunctor<NsDownloadWindow, int>* pProgressFunctor ;
    MemUnaFunctor<NsDownloadWindow, int>* pFinishedFunctor ;

    NSUpdate(NSContexte * pCtx) ;
		~NSUpdate() ;

		bool init() ;
    bool isUpdateAvailable() ;
    bool startUpdate(string sExecutionDirectory) ;

    string getVersionInformation() ;

    bool mustForceUpdate() { return updateParameters.isNetworkStation() ; }

    bool mustCheckForUpdateAvailability() { return updateParameters.mustCheckForUpdateAvailability() ; }

    bool getWaitingForUpdate()        { return _bWaitingForUpdateChoice ; }
    void setWaitingForUpdate(bool bW) { _bWaitingForUpdateChoice = bW; }

    bool getWantUpdate()        { return _bWantUpdate ; }
    void setWantUpdate(bool bW) { _bWantUpdate = bW; }

	protected :

		NSLibCurl curl ;

    bool   _bWaitingForUpdateChoice ;
    bool   _bWantUpdate ;

    string _sUpdateExecFullFileName ;
    string _sExpectedVersionNumber ;
} ;

#endif // __NSUPDATE_H__

