#ifndef __NSLDVEXPORT_H
#define __NSLDVEXPORT_H
#if defined(_DANSNSLDVDLL)
	#define _NSLDVCLASSE __export
	#define _NSLDVCLASSE __import
#endif

#endif         // __NSLDVEXPORT_H