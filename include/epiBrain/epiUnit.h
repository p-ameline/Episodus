//----------------------------------------------------------------------------
#ifndef __EPIUNIT_H
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

#endif

