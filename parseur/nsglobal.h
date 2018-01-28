//---------------------------------------------------------------------------
//   NSGLOBAL.H
//   KRZISCH Ph.   janvier 92
//  Contient toutes les définitions et variables globales de NAUTILUS.
//---------------------------------------------------------------------------
#ifndef __NSGLOBAL_H
#define __NSGLOBAL_H

#include <string>

class CSuperviseur;

class NSRoot
{
   public:
		CSuperviseur* pSuperviseur;
		NSRoot(CSuperviseur* pCtx) : pSuperviseur(pCtx) {}
};

//---------------------------------------------------------------------------
#endif   // __NSGLOBAL_H
