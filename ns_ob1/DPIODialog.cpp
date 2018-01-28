
#include <owl/dialog.h>
#include <owl/groupbox.h>
#include <owl/radiobut.h>
#include <vector.h>
#include <string>

#include "ns_bbk\DPIODialog.h"
#include "ns_bbk\BB1KS.h"
#include "ns_bbk\BB1KSAR.h"
#include "ns_bbk\BB1types.h"
#include "ns_bbk\ns_bbk.h"

#include "nautilus\nsdochis.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nsldvgoal.h"
#include "nsepisod\nsldvuti.h"

#ifndef __TCONTROLER__
// Classe DPIOrefButton
//////////////////////////////////

DEFINE_RESPONSE_TABLE1(DPIOrefButton, TRadioButton)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
END_RESPONSE_TABLE;

DPIOrefButton::DPIOrefButton(TWindow* parent, int resId, int type, const char far* title,                             int x, int y, int w, int h)
              :TRadioButton(parent, resId, title, x, y, w, h)
{
    // Attention, le constructeur de TRadioButton employé ici attribue le style
    // BS_AUTORADIOBUTTON, que nous remplaçons par BS_RADIOBUTTON

    Attr.Style    = (Attr.Style ^ BS_AUTORADIOBUTTON) | BS_RADIOBUTTON;
    Attr.ExStyle |= WS_EX_NOPARENTNOTIFY;
    pGroup = 0;
    iType = type;
}

DPIOrefButton::~DPIOrefButton()
{
}

void
DPIOrefButton::BNClicked()
{
    if (GetCheck() != BF_CHECKED)
        pGroup->Check(iType);
}

// Classe DPIOref
//////////////////////////////////

DPIOref::DPIOref(BB1KSAR *theksar, DPIOrefButton* theOui, DPIOrefButton* theNon,
            DPIOrefButton* theRefus, TStatic *theText)
{
  ksar   = theksar ;

  pOui   = theOui ;
  pNon   = theNon ;
  pRefus = theRefus ;
  pTexte = theText ;

  pOui->pGroup   = this ;
  pNon->pGroup   = this ;
  pRefus->pGroup = this ;

  iEtat = -1 ;
}

DPIOref::~DPIOref()
{
  delete pOui ;
  delete pNon ;
  delete pRefus ;
  delete pTexte ;
}

void
DPIOref::Check(int etat, bool bInitEtat)
{
  if (bInitEtat)
    iEtat = etat ;

  if (etat == DPIOref::oui)
  {
    pOui->Check() ;
    pNon->Uncheck() ;
    pRefus->Uncheck() ;
  }
  else if (etat == DPIOref::non)
  {
    pNon->Check() ;
    pOui->Uncheck() ;
    pRefus->Uncheck() ;
  }
  else if (etat == DPIOref::refus)
  {
    pRefus->Check() ;
    pOui->Uncheck() ;
    pNon->Uncheck() ;
  }
}

// Classe DPIODialog
//////////////////////////////////

DEFINE_RESPONSE_TABLE1(DPIODialog, NSUtilDialog /*TDialog*/)
	EV_COMMAND(IDCANCEL, CmCancel),
	EV_COMMAND(IDOK,     CmOk),
  EV_COMMAND(IDHELP,   CmHelp),
END_RESPONSE_TABLE;


DPIODialog::DPIODialog(NSContexte *pCtx, vector<BB1KSAR *> *KSARvector)
           :NSUtilDialog(pCtx->GetMainWindow(), pCtx, "DPIO_BASE", pNSResModule)
        //TDialog(pCtx->GetMainWindow(), "DPIO_BASE", pNSResModule),
		//NSRoot(pCtx)
{
  if (KSARvector && (false == KSARvector->begin()))
	  for (vector<BB1KSAR *>::iterator iter = KSARvector->begin() ; KSARvector->end() != iter ; iter++)
		  ksars.push_back(*iter) ;

  sHindex = string("") ;
  sHcorps = string("dpio_preselection.htm") ;
}


DPIODialog::~DPIODialog()
{
  if (false == aDPIOrefs.empty())
	  for (vector<DPIOref *>::iterator iter = aDPIOrefs.begin() ; aDPIOrefs.end() != iter ; )
	  {
		  delete *iter ;
		  aDPIOrefs->erase(iter) ;
	  }
}

void
DPIODialog::SetupWindow()
{
	NS_CLASSLIB::TRect cvtRect;

	int			iNbDPIOs					= ksars.size() ;

	int 		iBoxHeight 				= 10 ;    // hauteur du bouton
	int 		iBoxWidth  				= 200 ;   // largeur du bouton
	int 		iBoxInterv				= 3 ;     // intervalle entre deux boutons
	int 		iBoxTop    				= 4 ;     // haut du 1er bouton à partir du haut du groupbox
	// int 		iBoxTitleTop			= 12 ;
	int 		iBoxLeft   				= 5 ;     // gauche des boutons à partir de la gauche du groupbox

	int     iRBHeight         = iBoxHeight ;
	int     iRBWidth          = 40 ;
	int     iRBLeft ;

	int 		iGroupTop  				= 3 ;     // haut du groupbox
	int 		iGroupLeft 				= 2 ;     // gauche du groupbox

	int 		iSeparLeft 				= 3 ;     // gauche du séparateur	int 		iGB_Separ  				= 3 ;     // intervalle entre le bas du groupbox et le séparateur
	int 		iSepar_Btn 				= 8 ;     // intervalle entre le séparateur et le bouton

	int 		iBtnHeight 				= 25 ;    // hauteur d'un bouton Borland	int 		iBtnWidth  				= 43 ;    // largeur d'un bouton Borland
	int 		iBtnLeft   				= 3 ;     // gauche du premier bouton
	int 		iBtn_bas   				= 3 ;     // intervalle entre le bas du bouton et le bas de la boite

	int 		iGroupWidth 			= (2 * iBoxLeft) + iBoxWidth + (3 * iRBWidth) ;	int 		iTotalWidth 			= (2 * iGroupLeft) + iGroupWidth ;
	int 		iGroupHeight ;
	int 		iTotalGroupHeight = 0 ;
	int 		iTopOfBox ;
	int 		iID 							= 200 ;
	string 	sGroup 						= "" ;

	TGroupBox	*pGroup ;

	if (iNbDPIOs > 0)
	{		iGroupHeight = (2 * iBoxTop) + (iNbDPIOs * iBoxHeight) + ((iNbDPIOs - 1) * iBoxInterv) ;

		iTotalGroupHeight += iGroupHeight ;

		// On crée le groupe		cvtRect = NS_CLASSLIB::TRect(iGroupLeft, iGroupTop, iGroupLeft + iGroupWidth, iGroupTop + iGroupHeight) ;
		MapDialogRect(cvtRect) ;

		pGroup = new TGroupBox(this, -1, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), 0) ;		pGroup->Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
		iTopOfBox = iGroupTop + iBoxTop ;		for (KSARCIter iter = ksars.begin() ; ksars.end() != iter ; iter++)		{			// Rectangle du texte			cvtRect = NS_CLASSLIB::TRect(iGroupLeft + iBoxLeft, iTopOfBox, iGroupLeft + iBoxLeft + iBoxWidth, iTopOfBox + iBoxHeight) ;			MapDialogRect(cvtRect) ;
			// Titre			string sTitre = (*iter)->KS().Value("label-dialog", NULLSTRING) ;			// texte			TStatic	*pTxt = new TStatic(this, iID, sTitre.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), 0, 0) ;			iRBLeft = iGroupLeft + iBoxLeft + iBoxWidth;			iID++;			// Bouton oui			cvtRect = NS_CLASSLIB::TRect(iRBLeft, iTopOfBox, iRBLeft + iRBWidth, iTopOfBox + iRBHeight) ;			MapDialogRect(cvtRect) ;			DPIOrefButton* pOui = new DPIOrefButton(this, iID, DPIOref::oui, "oui", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height());			iRBLeft += iRBWidth;			iID++;			// Bouton non			cvtRect = NS_CLASSLIB::TRect(iRBLeft, iTopOfBox, iRBLeft + iRBWidth, iTopOfBox + iRBHeight) ;			MapDialogRect(cvtRect) ;			DPIOrefButton* pNon = new DPIOrefButton(this, iID, DPIOref::non, "non", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height());			iRBLeft += iRBWidth;			iID++;			// Bouton refus			cvtRect = NS_CLASSLIB::TRect(iRBLeft, iTopOfBox, iRBLeft + iRBWidth, iTopOfBox + iRBHeight) ;			MapDialogRect(cvtRect) ;			DPIOrefButton* pRefus = new DPIOrefButton(this, iID, DPIOref::refus, "refus", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height());			pDPIOrefs->push_back(new DPIOref(*iter, pOui, pNon, pRefus, pTxt)) ;			iTopOfBox += iBoxHeight + iBoxInterv ;
			iID++ ;
		}
	}	// On fixe la taille de la boite de dialogue
	int iTotalHeight = iGroupTop + iTotalGroupHeight + iGB_Separ + iSepar_Btn + iBtnHeight + iBtn_bas ;
	NS_CLASSLIB::TRect dlgSizeRect(0, 0, iTotalWidth, iTotalHeight) ;	MapDialogRect(dlgSizeRect) ;	// On crée le séparateur	int iSepareTop = iGroupHeight + iGroupTop + iGB_Separ ;
	cvtRect = NS_CLASSLIB::TRect(iSeparLeft, iSepareTop, iSeparLeft + iTotalWidth - (2 * iSeparLeft), iSepareTop + 3) ;
	MapDialogRect(cvtRect) ;

	TGroupBox* pDip = new TGroupBox(this, -1, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), 0) ;
	pDip->Attr.Style |= BSS_HDIP ;

	// On crée les boutons	int iNbBtn			= 3 ;
	int iBtnInterv	= (iTotalWidth - (2 * iBtnLeft) - (iNbBtn * iBtnWidth)) / (iNbBtn - 1) ;
	int iBtnTopPos  = iSepareTop + iSepar_Btn ;
	int iBtnLeftPos = iBtnLeft ;

	// Bouton OK
	cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;	MapDialogRect(cvtRect) ;
	/* TButton* pBtOK	= */ new TButton(this, IDOK, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), false, 0) ;
	iBtnLeftPos			+= iBtnWidth + iBtnInterv ;

	// Bouton CANCEL	cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;	MapDialogRect(cvtRect) ;
	/* TButton* pBtCn = */ new TButton(this, IDCANCEL, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), false, 0) ;
	iBtnLeftPos += iBtnWidth + iBtnInterv ;
	// Bouton HELP	cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;	MapDialogRect(cvtRect) ;
	/* TButton* pBtHl = */ new TButton(this, IDHELP, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), false, 0) ;
	iBtnLeftPos += iBtnWidth + iBtnInterv ;
	// Redimentionnement	NS_CLASSLIB::TRect	dlgRect ;
	GetWindowRect(dlgRect) ;
	NS_CLASSLIB::TRect	clientRect ;
	GetClientRect(clientRect) ;

	// On compare le clientRect réel avec les dimensions souhaitées,	// et on modifie le WindowRect en conséquence
	int nouvWindowWidth  = dlgRect.Width()  + (dlgSizeRect.Width()  - clientRect.Width()) ;
	int nouvWindowHeight = dlgRect.Height() + (dlgSizeRect.Height() - clientRect.Height()) ;

	// Titre de la boite de dialogue
	SetCaption("DPIO") ;	// Appel du SetupWindow() de TDialog	TDialog::SetupWindow() ;

  if (false == aDPIOrefs.empty())
    for (vector<DPIOref *>::iterator iter = aDPIOrefs.begin() ; aDPIOrefs.end() != iter ; iter++)
    {
      if (KSisChecked(*iter))
        (*iter)->Check(DPIOref::oui, true) ;
      else
        (*iter)->Check(DPIOref::non, true) ;
    }

	MoveWindow(dlgRect.left, dlgRect.top, nouvWindowWidth, nouvWindowHeight) ;
}

void
DPIODialog::CmOk()
{
  if (false == aDPIOrefs.empty())
    for (vector<DPIOref *>::iterator iter = aDPIOrefs.begin() ; aDPIOrefs.end() != iter ; iter++)
    {
      if ((*iter)->pOui->GetCheck() == BF_CHECKED)
      {
        if ((*iter)->iEtat == DPIOref::oui)
          (*iter)->ksar->nsStatus = WAITFORASKINGUSER ;
        else
          (*iter)->ksar->nsStatus = YESFORCED ;
      }
      else
      {
        if ((*iter)->pNon->GetCheck() == BF_CHECKED)
          (*iter)->ksar->nsStatus = STOP ;
        else
          if ((*iter)->pRefus->GetCheck() == BF_CHECKED)
            (*iter)->ksar->nsStatus = REFUS ;
      }
    }

	if (pContexte->getSuperviseur()->pDPIO->bDPIOPreCoche)
		pContexte->getSuperviseur()->blackboard->addCycles2Do(200) ;

	// maintenant on finit d'exécuter les cycles ne cours puis on change l'état du KSAR
/*
	for (vector<DPIOref *>::iterator iter = pDPIOrefs->begin() ; iter != pDPIOrefs->end() ; iter++)
		if ((*iter)->ksar->nsStatus == WAITFORASKINGUSER)
		{
			string sArchetype = (*iter)->ksar->KS().Value("archetype", NULLSTRING) ;
			pContexte->getSuperviseur()->BbkAskUser(sArchetype, pContexte) ;
		}
*/
	TDialog::CmOk() ;
}

void
DPIODialog::CmCancel()
{
  if (false == aDPIOrefs.empty())
    for (vector<DPIOref *>::iterator iter = aDPIOrefs.begin() ; aDPIOrefs.end() != iter ; iter++)
      (*iter)->ksar->nsStatus = STOP ;

	TDialog::CmCancel() ;
}

bool
DPIODialog::KSisChecked(DPIOref *refKS)
{
	if (NULL == refKS)
		return false ;

	bool bDPIOPresent = false ;
	bool bDPIOPrevu = false ;
	string sSensDPIO ;
	string sSensPatho ;
	string sSensGoal ;
	NVLdVTemps tToday ;
	tToday.takeTime() ;

  string sFormulaire = refKS->ksar->KS().Value("formulaire", NULLSTRING) ;
  if (string("") == sFormulaire)
		return false ;

	pContexte->getDico()->donneCodeSens(&sFormulaire, &sSensDPIO) ;

	// On regarde d'abord si le DPIO appartient au dossier patient
	for (DocumentIter iter = pContexte->getPatient()->pDocHis->VectDocument.begin() ;
					iter != pContexte->getPatient()->pDocHis->VectDocument.end() ;
					iter++)
	{
		if (((*iter)->pPatPathoArray) && (!((*iter)->pPatPathoArray->empty())))
    {
			PatPathoIter pathoIter = (*iter)->pPatPathoArray->begin();
			string sLexique = (*pathoIter)->pDonnees->lexique;
			pContexte->getDico()->donneCodeSens(&sLexique, &sSensPatho);
    }
#ifdef N_TIERS
    else
    	sSensPatho = sTypeContenuSens ;
#endif

		if (sSensPatho == sSensDPIO)
		{
			bDPIOPresent = true;

			for (ArrayGoalIter goalIter = pContexte->getPatient()->pDocLdv->getGoals()->begin();
                            goalIter != pContexte->getPatient()->pDocLdv->getGoals()->end();
                            goalIter++)
			{
				string sLexGoal = (*goalIter)->sLexique ;
				pContexte->getDico()->donneCodeSens(&sLexGoal, &sSensGoal) ;

				if (sSensGoal == sSensDPIO)
				{
					// Si l'objectif n'est pas déjà fermé
					if ((((*goalIter)->tFermeLe.estVide()) || ((*goalIter)->tFermeLe > tToday)) &&
                        (!(*goalIter)->pJalons->empty()))
					{
						GoalInfoRIter i = (*goalIter)->pJalons->rbegin() ;
						do						{							if ((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonIntermediaire)              {              	if ((tToday.donneDate() >= (*i)->tpsInfo.donneDate()) &&                                    ((*i)->iLevel != NSLdvGoalInfo::AVjaune))                {                	bDPIOPrevu = true ;                  break ;                }              }              else if ((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonCycle)              	break ;              else if ((*i)->iTypeJalonEvent == NSLdvGoalInfo::jalonOuverture)              	break ;              i++ ;            }            while (i != (*goalIter)->pJalons->rend()) ;

            if (bDPIOPrevu)
            	break ;
          }
        }
      }

      break ;
    }
	}

	if (bDPIOPresent)
	{
		if (bDPIOPrevu)
			return true ;
		else
			return false ;
	}

	// ---------------------------------------------------------------------------
	// on vérifie si la précondition sur l'age est valide
	// ---------------------------------------------------------------------------

	bool				bAgePrecondition = false ;
	const BB1KS	*bbKS	= &(refKS->ksar->KS()) ;
	AKSSet			*akss	= pContexte->getSuperviseur()->blackboard->Akss ;

  if (false == akss->empty())
  {
    for (AKSCIter aks = akss->begin() ; akss->end() != aks ; aks++)
    {
      if ((*aks)->ks == bbKS)
      {
        const APConditionT apC = (*aks)->apCondition ;
        if (apC)
        {
          bAgePrecondition = (*apC)(*(pContexte->getSuperviseur()->blackboard)) ;
          break ;
        }
      }
    }
  }

	// ---------------------------------------------------------------------------
	// si la précondition sur l'age est valide, c'est-à-dire si le patient est
	// concerné de part son age, on vérifie s'il existe un formulaire de calcul de
	// risque correspondant
	// ---------------------------------------------------------------------------

	if (bAgePrecondition)
		return true ;

	return false ;
}

#endif

