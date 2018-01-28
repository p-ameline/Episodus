// -----------------------------------------------------------------------------
// ns_vidal.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2015/02/07 14:44:30 $
// -----------------------------------------------------------------------------
// Import / Export for Schiller's SEMA2 format regarding ECG
// -----------------------------------------------------------------------------
// PA - juillet 2010
// -----------------------------------------------------------------------------

#ifndef __SCHILLERECG_H__
#define __SCHILLERECG_H__

#define PSEUDOPROTOCOLNAME "sema"

string getImportPath(NSContexte *pContext) ;
string getExportPath(NSContexte *pContext) ;
string getExePath(NSContexte *pContext) ;
string ExecuteCommand(string command, string argument, string directory_path) ;

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
//
class NsSchillerEcgInformation : public NSRoot
{
  public:

    OWL::TButton*	  pCreatePatientButton ;

    NsSchillerEcgInformation(NSContexte* pCtx) : NSRoot(pCtx) {} ;
    ~NsSchillerEcgInformation() {} ;

    void   init() ;

    bool   loadParams() ;
    bool   initLocalizationStrings() ;
    string getText(string sChapter, string sCode) ;

    bool   writeParams() ;
    bool   setLocalizationStrings() ;

    string getSemaPassString() ;

    string getExeName()   { return _sExeName ; }
    string getUserId()    { return _sUserId ; }
    string getUserPass()  { return _sUserPass ; }

    void   setExeName(string sEN)  { _sExeName  = sEN ; }
    void   setUserId(string sUI)   { _sUserId   = sUI ; }
    void   setUserPass(string sUP) { _sUserPass = sUP ; }

    string getExeDir()    { return _sExeDir ; }
    string getExportDir() { return _sExportDir ; }
    string getImportDir() { return _sImportDir ; }

  protected:

    NSLocalChapterArray _aLocalizationStrings ;

    string _sExeName ;
    string _sUserId ;
    string _sUserPass ;

    string _sExeDir ;
    string _sExportDir ;
    string _sImportDir ;
} ;

#ifdef _MAIN
    #ifndef __MAIN
        extern TModule    *pNSResModule ;
    #else   // __MAIN
        static TModule    *pResMod ;
        TModule           *pNSResModule ;
    #endif  // __MAIN
#else    // _MAIN
    extern TModule    *pNSResModule ;
#endif   // _MAIN

#endif    // __SCHILLERECG_H__

