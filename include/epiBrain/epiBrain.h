//----------------------------------------------------------------------------// CAPTURE D'IMAGES//----------------------------------------------------------------------------
#ifndef __EPIBRAIN_H#define __EPIBRAIN_H
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

EXPNETTYPE int   WINAPI initEpiBrain(void) ;
EXPNETTYPE char* WINAPI testEpiBrain(char* szToSearch) ;EXPNETTYPE char* WINAPI getEpiBrainVersion(void) ;EXPNETTYPE int   WINAPI getCodesFromText(char* szLanguage, char* szToSearch, char* szClassif, char* szPatInfo, char* szSoapSlot, char* szSeparators, char* szCodes, int iCodeLen) ;EXPNETTYPE int   WINAPI getClassifLabelsFromCodes(char* szLanguage, char* szClassif, char* szCodes, char* szLabels, int iLabelsLen) ;EXPNETTYPE int   WINAPI closeEpiBrain(void) ;#ifdef __cplusplus}
#endif
void   resetTraces() ;void   traceForString(char* szInput, char* InputName, bool bShouldNotBeNull = true, bool bShouldNotBeEmpty = true, NSSuper::TRACETYPE iDefaultTraceLevel = NSSuper::trDetails) ;bool   FileExists(string sFileNameAndPath) ;#endif  // __EPIBRAIN_H

