#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
#else
  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
#endif

#include "partage\nscim10.h"
#include "nssavoir\nsfilgd.h"
#include "dcodeur\decordon.h"
#include "nssavoir\nsguide.h"

//  +-----------------------------------------------------------------+
//  ¦               Décode la prescription médicamenteuse             ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1988 Dernière mise à jour 23/08/1989
decodeMedicament::decodeMedicament(NSContexte* pCtx, string sLangue)
                 :decodageBase(pCtx, sLangue)
{
	pAdministration = new decodeMediAdmin(this) ;
	pDose           = new gereNum(pContexte->getSuperviseur(), sLangue) ;
	sUniteAnthropo  = string("") ;
	sMedicament     = string("") ;
}

decodeMedicament::decodeMedicament(decodageBase* pBase)
                 :decodageBase(pBase)
{
  pAdministration = new decodeMediAdmin(this) ;
  pDose           = new gereNum(pContexte->getSuperviseur(), _sLangue) ;
  sUniteAnthropo  = string("") ;
  sMedicament     = string("") ;
}

decodeMedicament::~decodeMedicament()
{
	delete pAdministration ;
  delete pDose ;
}

void
decodeMedicament::decode(int colonne)
{
	int refCol = getCol() ;

	while ((getCol() > colonne) && iBon())
	{
    if      (getSt() == "GADMI")
    {
    	Avance() ;
      pAdministration->decode(refCol) ;
    }
    else if (getSt() == "VDOSM")
    {
    	Avance() ;
      donneDimension(refCol, pDose) ;
    }
    else if (getSt() == "0UNAN")
    {
    	Avance() ;
      string sLex = getStL() ;
      pContexte->getDico()->donneLibelle(_sLangue, &sLex, &sUniteAnthropo) ;
      Avance() ;
    }
    else
    	Recupere() ;
	}
}

void
decodeMedicament::donnePhrase()
{
	setDcodeur(string("")) ;

	if (false == pDose->estVide())
	{
  	setDcodeur(pDose->getNum(pContexte, string("")) + string(" ") + pDose->donneLibelleUnite(pContexte)) ;
    if (CommenceParVoyelle(&sMedicament))
    	addToDcodeur(string(" d'")) ;
    else
    	addToDcodeur(string(" de ")) ;
	}
	addToDcodeur(sMedicament) ;

	if (sUniteAnthropo != "")
		addToDcodeur(string(" par ") + sUniteAnthropo) ;
}

//  +-----------------------------------------------------------------+
//  ¦               Décode l'administration médicamenteuse            ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1988 Dernière mise à jour 23/08/1989
decodeMediAdmin::decodeMediAdmin(NSContexte* pCtx, string sLangue)
                :decodageBase(pCtx, sLangue)
{
  voieAdmin = string("") ;
  lieuAdmin = string("") ;
}

decodeMediAdmin::decodeMediAdmin(decodageBase* pBase)
                :decodageBase(pBase)
{
  voieAdmin = string("") ;
  lieuAdmin = string("") ;
}

void
decodeMediAdmin::decode(int colonne)
{
  int refCol = getCol() ;

  while ((getCol() > colonne) && iBon())
  {
    if      (getSt() == "0VADM")
    {
      Avance() ;

      string sLabel1 = string("") ;
      string sLabel2 = string("") ;

      while ((getCol() > refCol) && iBon())
      {
        string sLex = getStL() ;
        pContexte->getDico()->donneLibelle(_sLangue, &sLex, &sLabel2) ;
        Avance();
        etDuMilieu(&voieAdmin, &sLabel1, &sLabel2, string(", ")) ;
      }
      etFinal(&voieAdmin, &sLabel1, string(" et ")) ;
    }
    else if (getSt() == "0LOCA")
    {
      Avance();

      string 	sLabel1, sLabel2;
      sLabel1 = sLabel2 = "";

      while ((getCol() > refCol) && iBon())
      {
        string sLex = getStL() ;
        pContexte->getDico()->donneLibelle(_sLangue, &sLex, &sLabel2) ;
        Avance() ;
        etDuMilieu(&lieuAdmin, &sLabel1, &sLabel2, string(", ")) ;
      }
      etFinal(&lieuAdmin, &sLabel1, string(" et ")) ;
    }
    else
      Recupere() ;
  }
}

