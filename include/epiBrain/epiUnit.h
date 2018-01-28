//----------------------------------------------------------------------------// CAPTURE D'IMAGES//----------------------------------------------------------------------------
#ifndef __EPIUNIT_H#define __EPIUNIT_H
#ifdef _MAIN
	#ifndef __MAIN
		extern TModule *pNSResModule ;
	#else
		static TModule    *pResMod = 0 ;
    static NSContexte *pLocalContext = 0 ;
    static NSSuper    *pNSSuperviseur = 0 ;
		TModule *pNSResModule ;
	#endif
#else
	extern TModule *pNSResModule ;
#endif

#define EXPNETTYPE __declspec( dllexport )

#ifdef __cplusplus
extern "C" {
#endif

EXPNETTYPE int   WINAPI initEpiUnit(void) ;
EXPNETTYPE char* WINAPI getEpiUnitVersion(void) ;EXPNETTYPE char* WINAPI checkEpiUnit(char* szToSearch) ;EXPNETTYPE char* WINAPI getEpiBrainVersion(void) ;EXPNETTYPE int   WINAPI executeUnitTests(string* pLanguage, string* pReturnString, string* pChapter) ;EXPNETTYPE int   WINAPI closeEpiUnit(void) ;#ifdef __cplusplus}
#endif
void   resetTraces() ;void   resultForDouble(double dResult, double dTheorical, string sDescription, string* pReport) ;void   resultForInt(int dResult, int dTheorical, string sDescription, string* pReport) ;void   traceForString(char* szInput, char* InputName, bool bShouldNotBeNull = true, bool bShouldNotBeEmpty = true, NSSuper::TRACETYPE iDefaultTraceLevel = NSSuper::trDetails) ;bool   FileExists(string sFileNameAndPath) ;#endif  // __EPIBRAIN_H

