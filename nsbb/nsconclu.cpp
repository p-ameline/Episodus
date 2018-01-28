//------------------------------------------------------------------------
//   nspardlg : paramètres des éléments Patpatho
//   Kaddachi Hafedh le 04/09/1997 à 11:03:04
//------------------------------------------------------------------------

#ifdef _INCLUS
  #include "partage\Nsglobal.h"
  #include "nsbb\nsutidlg.h"
#endif

#include "nsbb\nsconclu.h"

#ifdef _INCLUS
DEFINE_RESPONSE_TABLE1(NSConclusion, NSDialogWrapper)
#else
DEFINE_RESPONSE_TABLE1(NSConclusion, TDialog)
#endif
	EV_COMMAND(IDOK, 		CmOk),
	EV_COMMAND(IDCANCEL,	CmCancel),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NSConclusion::NSConclusion(TWindow* parent , TResId resId, BBItem* pBBitem , TModule* module)
#ifdef _INCLUS
              :NSDialogWrapper(pBBitem->pBigBoss->pContexte, parent, resId, module)
#else
              :TDialog(parent, resId, module)
#endif
{
   ressource 					= resId;
   AParent 						= parent;
   pBBitemConclusion       = pBBitem;

	pAvant 		 = new TRadioButton(this,CONCLUSION_AVANT);
   pPlace 		 = new TRadioButton(this, CONCLUSION_PLACE);
   pApres 		 = new TRadioButton(this, CONCLUSION_APRES);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NSConclusion::~NSConclusion()
{
	delete pApres;
   delete pPlace;
   delete pAvant;
}



//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSConclusion::NSConclusion(NSConclusion& src)
          :TDialog(src.AParent, src.ressource)
{
   ressource	=	src.ressource;
   AParent		=	src.AParent;
   pApres      =  src.pApres;
   pPlace      =  src.pPlace;
   pAvant      =  src.pAvant;
   pBBitemConclusion = src.pBBitemConclusion;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
inline
NSConclusion& NSConclusion::operator= (NSConclusion src)
{
	ressource	=	src.ressource;
   AParent		=	src.AParent;
   pApres      =  src.pApres;
   pPlace      =  src.pPlace;
   pAvant      =  src.pAvant;
   pBBitemConclusion = src.pBBitemConclusion;
	return *this;
}


//---------------------------------------------------------------------------
// SetupWindow
//---------------------------------------------------------------------------
void
NSConclusion::SetupWindow()
{
   TDialog::SetupWindow();

   if( pBBitemConclusion->sPositionConclusion == string("AVANT1") )
   	pAvant->Check();
   else if ( pBBitemConclusion->sPositionConclusion == string("PLACE1") )
   	pPlace->Check();
   else
   	pApres->Check();
}



//-----------------------------------------------------------------------
// confirmer le choix : ok
//-----------------------------------------------------------------------
void
NSConclusion::CmOk()
{
  //pluriel
  if (pAvant->GetCheck() == BF_CHECKED)
  		pBBitemConclusion->sPositionConclusion = string("AVANT1");
  else if (pPlace->GetCheck() == BF_CHECKED)
  		pBBitemConclusion->sPositionConclusion = string("PLACE1") ;
  else
	  pBBitemConclusion->sPositionConclusion = string("APRES1");

  pBBitemConclusion->pBigBoss->sPositionConclusion = pBBitemConclusion->sPositionConclusion;
  CloseWindow(IDOK);
}

//-----------------------------------------------------------------------
// annuler le choix : cancel
//-----------------------------------------------------------------------
void
NSConclusion::CmCancel()
{
 	Destroy(IDCANCEL);
}



