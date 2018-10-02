#ifndef __EPISODEXPORT_H
#define __EPISODEXPORT_H
#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif

#endif         // __EPISODEXPORT_H
