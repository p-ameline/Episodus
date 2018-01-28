#ifndef __NSUPDATEDLG_H#define __NSUPDATEDLG_H

#include <owl\dialog.h>
#include <owl\static.h>
#include <owl\edit.h>
#include <owl\button.h>

class NSUpdate ;

// #include "nautilus\nsupdate.h"
#include "nsoutil\nsexport.h"
#include "nsbb\nsutidlg.h"

class _NSOUTILCLASSE NSUpdateParameters : public NSRoot
{
	public :

		string     _sSettingsFile ;

		string     _sURL ;
    string     _sLogin ;
  	string     _sPassword ;
  	string     _sProtocol ;
  	string     _sUpdateDir ;

  	int        _iInterval ;
  	bool       _bLastUpdateCheck ;
  	bool       _bInterval ;
    NVLdVTemps _tLastUpdateCheck ;
    NVLdVTemps _tNextUpdateCheck ;

  	bool       _bLastUpdate ;
  	NVLdVTemps _tLastUpdate ;

    bool       _bSilentMode ;

    NSUpdateParameters(NSContexte * pCtx) ;
		~NSUpdateParameters() ;

		bool init() ;
    bool mustCheckForUpdateAvailability() ;
    bool updateLastCheckDate() ;
    bool updateLastUpdateDate() ;

    bool isNetworkStation() { return _sURL == _sUpdateDir ; }

    string getReleaseTxt() ;
} ;

class _NSOUTILCLASSE NsUpdateYesNoWindow : public TLayoutWindow
{
	public:

    OWL::TButton*   _pYesButton ;
    OWL::TButton*   _pNoButton ;
    OWL::TButton*   _pEnlargeButton ;
    OWL::TEdit*     _pReleasesViewer ;
    OWL::TStatic* 	_pQuestionText ;

    NSUpdate*       _pUpdater ;

    NSContexte*     pContexte ;

	  NsUpdateYesNoWindow(TWindow* pere, NSContexte* pCtx, NSUpdate* pUpdater) ;
	  ~NsUpdateYesNoWindow() ;

	  void SetupWindow() ;
    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;

    void CmYes() ;
    void CmNo() ;
    void CmEnlarge() ;

    string getReleaseTxt() ;

	DECLARE_RESPONSE_TABLE(NsUpdateYesNoWindow) ;
};

class _NSOUTILCLASSE NsDownloadWindow : public TLayoutWindow
{
	public:

    OWL::TStatic* 	_pQuestionText ;
    OWL::TGauge*    _pGauge ;

    NSUpdate*       _pUpdater ;

    NSContexte*     pContexte ;

    string          _sCaption ;

	  NsDownloadWindow(TWindow* pere, NSContexte* pCtx, NSUpdate* pUpdater) ;
	  ~NsDownloadWindow() ;

	  void SetupWindow() ;
    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre) ;

    void SetPercentDone(int percent) ;
    void IsFinished(int iStatus) ;

	DECLARE_RESPONSE_TABLE(NsDownloadWindow) ;
};

#endif  // __NSUPDATEDLG_H
