//---------------------------------------------------------------------------
//    NSHORLOG.H
//    KRZISCH Ph. janvier 92
// Contient la déclaration de l'objet NShorloge.
//---------------------------------------------------------------------------

#ifndef __NSHORLOG_H
#define __NSHORLOG_H

/*#ifndef __NSWPARAM_H
#include "nswparam.h"  // Paramètres globaux
#endif */

#ifndef __TIME_H
#include <time.h>
#endif

#ifndef __OWLPCH_H
#include <owl\owlpch.h>
#endif

/*
#ifndef __NSGLOBAL_H
#include "nsglobal.h"  // Déclarations globales
#endif

#ifndef __NSUTIL_H
#include "nsutil.h"    // Fonctions utiles NAUTILUS
#endif
*/
#ifndef __WINDOWS_H
#include <windows.h>
#endif

//#ifndef __OWL_H
//#include <owl.h>
//#endif

#ifndef __WINDOW_H
#include <owl\window.h>
#endif

//---------------------------------------------------------------------------
// Classe NSHorloge
//        ANCETRE   : TWindow
//        SURCHARGE :
//  Cette classe sert à mettre enplace l'horloge.
//---------------------------------------------------------------------------
// _CLASSDEF(NSHorloge)
class NSHorloge : public TWindow
{
 public :
 // Constructeur
	NSHorloge(TWindow* AParent, const char far* ATitle, TModule* module);

 // Destructeur
	virtual ~NSHorloge();
 protected :
 // Fonctions de réponse aux messages
	void EvPaint(UINT , TPoint&);
	void EvCreate(UINT , TPoint&);
	void EvTimer(UINT , TPoint&) ;
	void EvWininiChange(UINT , TPoint&);

// Fonction de fermeture
	virtual bool CanClose();

	DECLARE_RESPONSE_TABLE(NSHorloge);
};
#endif  // __NSHORLOG_H
