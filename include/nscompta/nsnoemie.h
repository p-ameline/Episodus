#ifndef __NSNOEMIE_H#define __NSNOEMIE_H

#include <classlib\arrays.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>
#include <owl\listwind.h>

#if defined(_DANSCPTADLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"
#include "nssavoir\nslangue.h"

//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------

class NSEntiteNoemie
{
	public:

		NSEntiteNoemie(string sEntit, string sNiv, string sDonn) ;
    ~NSEntiteNoemie() {}

    string sEntite ;
    string sNiveau ;
    string sDonnee ;
};

typedef vector<NSEntiteNoemie*> NSNoeArray ;
typedef NSNoeArray::iterator       NSNoemieArrayIter ;
typedef NSNoeArray::const_iterator NSNoemieArrayConstIter ;

class _CLASSELEXI NSNoemieArray : public NSNoeArray
{
	public :

		// Constructeurs
		NSNoemieArray() : NSNoeArray() {}
		NSNoemieArray(const NSNoemieArray& rv) ;
		// Destructeur
		virtual ~NSNoemieArray() ;

		// Opérateur =
		NSNoemieArray& operator=(const NSNoemieArray& src) ;
		void vider() ;
};

class _CLASSELEXI NSNoemie :  public TDialog, public NSRoot
{
  public:

  	OWL::TEdit*   pFichierRsp ;
  	OWL::TButton* pLancer ;

  	NSNoemieArray aEntites ;

  	NSNoemie(TWindow* pere, NSContexte* pCtx) ;
  	~NSNoemie() ;

  	void CmCancel() ;
  	void CmOk() ;
  	void CmLancer() ;
  	void ChangeEuroFranc(string sDate, string sUnite ) ;
  	void Traiter(string sFichierRsp) ;
  	void AjouteErreur(string sErreur, string sFichierRsp) ;
    bool cherchePat(NSPatient* pPat, string sNom, string sPrenom, string sDateNais, string sErreur, string sLog) ;
    bool goodPat(NSPatient* pPat, string sNom, string sPrenom) ;
  	bool existeActe(string sNumCompt, string sActe, string sQteActe, string sCoefActe) ;
  	bool CreerFact(string sNumCompt, string sOperateur, string sDate_paie ,string sOrganisme, string sLibelle, string sMontant, string sUnite , string sMode_paie) ;

  	unsigned int Moulinette() ;

  DECLARE_RESPONSE_TABLE(NSNoemie) ;
};

#endif  // #ifndef __NSNOEMIE_H

