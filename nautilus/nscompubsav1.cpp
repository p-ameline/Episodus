// NSCOMPUB.CPP : Méthodes dédiées à la composition et à la publication
// RS Septembre 1997
//////////////////////////////////////////////////////////////////////////

#include <owl\applicat.h>
#include <owl\olemdifr.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <owl\color.h>
#include <owl\module.h>
#include <idapi.h>
#include <stdio.h>
#include <string.h>
#include <winuser.h>

#include "nautilus\nssuper.h"
#include "nautilus\nsanxary.h"

#include "nautilus\nsresour.h"
#include "partage\nsdivfct.h"
#include "partage\nsperson.h"
#include "nautilus\nscompub.h"

// -----------------------------------------------------------------
//
//  Méthodes de ChoixTemplateDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ChoixTemplateDialog, NSUtilDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_TEMPLATEBOX, LBN_SELCHANGE, CmSelectTemplate),
	EV_CHILD_NOTIFY_AND_CODE(IDC_TEMPLATEBOX, LBN_DBLCLK, CmTemplateDblClk),
END_RESPONSE_TABLE;

ChoixTemplateDialog::ChoixTemplateDialog(TWindow* pere, NSSuper* pSup, char* typeDoc)
							:NSUtilDialog(pere, pSup, IDD_TEMPLATE)
{
	pTemplateBox 	= new TListBox(this, IDC_TEMPLATEBOX);
	pTemplateArray = new NSTemplateArray;
   strcpy(cTypeDoc,typeDoc);
   TemplateChoisi = 0;
}

ChoixTemplateDialog::~ChoixTemplateDialog()
{
	delete pTemplateBox;
	delete pTemplateArray;
}

/* void
ChoixTemplateDialog::LanceTemplate(int NumDoc)
{
	NSDocumentInfo* Document = (*pDocusArray)[NumDoc-1];
	pSuper->OuvreDocument(*Document);
} */

void
ChoixTemplateDialog::SetupWindow()
{
  	TDialog::SetupWindow();
	//
	// Remplissage de TemplateBox avec les libelles des fichiers modele
	//
	NSTemplate* pTemplate = new NSTemplate(pSuper);
	//
	// Ouverture du fichier
	//
	pTemplate->lastError = pTemplate->open();
	if (pTemplate->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture de la base des templates.", 0, pTemplate->lastError);
		delete pTemplate;
		return;
	}

   pTemplate->lastError = pTemplate->debut(dbiWRITELOCK);
   if (pTemplate->lastError != DBIERR_NONE)
	{
		erreur("Erreur de positionnement dans la base des templates.", 0, pTemplate->lastError);
		pTemplate->close();
		delete pTemplate;
		return;
	}

   do
   {
   	pTemplate->lastError = pTemplate->getRecord();
		if (pTemplate->lastError != DBIERR_NONE)
		{
			erreur("Erreur de lecture dans la base des templates.", 0, pTemplate->lastError);
			pTemplate->close();
			delete pTemplate;
			return;
		}

      // on remplit la TemplateBox et le TemplateArray si le type correspond au type Document
      // note : pour les CN on ne tient pas compte de la version

      if (((!strncmp(cTypeDoc,"CN",2)) && (!strncmp(pTemplate->pDonnees->type,"CN",2))) || (!strcmp(cTypeDoc,pTemplate->pDonnees->type)))
      {
      	pTemplateBox->AddString(pTemplate->pDonnees->libelle);
			pTemplateArray->push_back(new NSTemplateInfo(pTemplate));
      }

      // on se positionne sur la template suivante

      pTemplate->lastError = pTemplate->suivant(dbiWRITELOCK);
		if ((pTemplate->lastError != DBIERR_NONE) && (pTemplate->lastError != DBIERR_EOF))
      {
			erreur("Erreur d'accès à la template suivante.", 0, pTemplate->lastError);
         pTemplate->close();
			delete pTemplate;
			return;
      }

   }
   while (pTemplate->lastError != DBIERR_EOF);

	pTemplate->lastError = pTemplate->close();
   if (pTemplate->lastError != DBIERR_NONE)
		erreur("Erreur de fermeture de la base des templates.", 0, pTemplate->lastError);

	delete pTemplate;
}


void ChoixTemplateDialog::CmTemplateDblClk(WPARAM Cmd)
{
	TemplateChoisi = pTemplateBox->GetSelIndex() + 1;
	TDialog::CmOk();
}

void ChoixTemplateDialog::CmSelectTemplate(WPARAM Cmd)
{
	//
	// Récupération de l'indice du template sélectionné
	//
	TemplateChoisi = pTemplateBox->GetSelIndex() + 1;
}

void ChoixTemplateDialog::CmCancel()
{
	TemplateChoisi = 0;
	TDialog::CmCancel();
}

// -----------------------------------------------------------------
//
//  Méthodes de NomCompoDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NomCompoDialog, TDialog)
   EV_BN_CLICKED(IDC_NEWCOMPO, CmClickNewCompo),
   EV_BN_CLICKED(IDC_OLDCOMPO, CmClickOldCompo),
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NomCompoDialog::NomCompoDialog(TWindow* pere, NSSuper* pSup)
               : NSUtilDialog(pere, pSup, IDD_NOMCOMPO)
{
	pNomDocHtml = new TEdit(this, IDC_EDITNOMCOMPO, DOC_NOM_LEN);
   pNewCompo 	= new TRadioButton(this,IDC_NEWCOMPO,0);
   pOldCompo 	= new TRadioButton(this,IDC_OLDCOMPO,0);

   sNomDocHtml = "";
}

NomCompoDialog::~NomCompoDialog()
{
	delete pNomDocHtml;
   delete pNewCompo;
   delete pOldCompo;
}

void
NomCompoDialog::SetupWindow()
{
	TDialog::SetupWindow();
   pOldCompo->Check();
   pNewCompo->Uncheck();
}

void
NomCompoDialog::CmClickNewCompo()
{
	pOldCompo->Uncheck();
}

void
NomCompoDialog::CmClickOldCompo()
{
	pNewCompo->Uncheck();
}

void
NomCompoDialog::CmOk()
{
	char far cfNomDocHtml[DOC_NOM_LEN + 1];

   pNomDocHtml->GetText(cfNomDocHtml,DOC_NOM_LEN);
	sNomDocHtml = string(cfNomDocHtml);

	if ((pNewCompo->GetCheck()) == BF_CHECKED)
   {
   	if (sNomDocHtml == "")
      {
      	MessageBox("Erreur : Vous n'avez pas saisi de nom pour le nouveau fichier");
         return;
      }
   }

   TDialog::CmOk();
}

void
NomCompoDialog::CmCancel()
{
	TDialog::CmCancel();
}

// -----------------------------------------------------------------
//
//  Méthodes de NSPubliEdit
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSPubliEdit, NSUtilEdit)
	EV_WM_CHAR,
END_RESPONSE_TABLE;


NSPubliEdit::NSPubliEdit(NSUtilDialog* pUtilDialog, int resId, int iTextLen, bool bReturn) :
					NSUtilEdit(pUtilDialog, resId, iTextLen, bReturn)
{
}

NSPubliEdit::~NSPubliEdit()
{
}

void
NSPubliEdit::EvChar(uint key, uint repeatCount, uint flags)
{
	char far sNbExpl[WF_NB_PUBLI_LEN + 1];
	UINT 		correspSelect;
   PubliCorrespDialog*  pPCDialog = dynamic_cast<PubliCorrespDialog*>(pNSUtilDialog);
   NSCorrespondantInfo* pCorrespSelect;

	NSUtilEdit::EvChar(key,repeatCount,flags);

   GetText(sNbExpl, WF_NB_PUBLI_LEN + 1);
   correspSelect = pPCDialog->pCorrespBox->GetSelIndex();
   pCorrespSelect = pPCDialog->aCorrespArray[correspSelect];
   pPCDialog->SetNbExpl(pCorrespSelect,sNbExpl);
}

// -----------------------------------------------------------------
//
//  Méthodes de PubliCorrespDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(PubliCorrespDialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_CORRESPBOX, LBN_SELCHANGE, CmSelectCorresp),
	EV_CHILD_NOTIFY_AND_CODE(IDC_CORRESPBOX, LBN_DBLCLK, CmCorrespDblClk),
   EV_COMMAND(IDC_SELECT, LanceChoixCorrespDialog),
   EV_BN_CLICKED(IDC_IMPRIMANTE, CmClickImprimante),
   EV_BN_CLICKED(IDC_EMAIL, CmClickEmail),
   EV_BN_CLICKED(IDC_INTRANET, CmClickIntranet),
   // EV_BN_CLICKED(IDC_SERVEUR, CmClickServeur),
   // EV_BN_CLICKED(IDC_LETTRE, CmClickLettre),
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

PubliCorrespDialog::PubliCorrespDialog(TWindow* pere, NSSuper* pSup, NSPublication* pPublication)
							:NSUtilDialog(pere, pSup, IDD_PUBLIER)
{
	pCorrespBox 	= new TListBox(this, IDC_CORRESPBOX);
   pAdresse = new NSUtilEdit(this, IDC_ADRESSE);
   pNbExpl = new NSPubliEdit(this, IDC_NBEXPL, WF_NB_PUBLI_LEN);
   pImprimante = 	new TRadioButton(this,IDC_IMPRIMANTE,0);
   pEmail =       new TRadioButton(this,IDC_EMAIL,0);
   pIntranet =		new TRadioButton(this,IDC_INTRANET,0);
   // pServeur =		new TRadioButton(this,IDC_SERVEUR,0);
   // pLettre = 		new TCheckBox(this,IDC_LETTRE);
	pPubli = pPublication;
   pCorrespBaseArray = &(pPubli->aCorrespBaseArray);
}

PubliCorrespDialog::~PubliCorrespDialog()
{
	// delete pLettre;
   // delete pServeur;
   delete pIntranet;
   delete pEmail;
   delete pImprimante;
   delete pAdresse;
   delete pNbExpl;
	delete pCorrespBox;
   pPubli = 0;					  // ne pas deleter
   pCorrespBaseArray = 0;    // ne pas deleter
}

void
PubliCorrespDialog::InitCorrespArray()
{
	int j = 0;

	aCorrespArray.vider();

   for (CorrespInfoIter i = pCorrespBaseArray->begin(); i != pCorrespBaseArray->end(); i++,j++)
   {
   	if (pPubli->aChoixPubli[j].select)
      {
      	aCorrespArray.push_back(new NSCorrespondantInfo(*(*i)));
      }
   }
}

void
PubliCorrespDialog::AfficheCorresp()
{
	string sCorresp;

	InitCorrespArray();

   // on vide la liste si elle contient des items
   if (pCorrespBox->GetCount())
   	pCorrespBox->ClearList();

   for (CorrespInfoIter i = aCorrespArray.begin(); i != aCorrespArray.end(); i++)
   {
      // on remplit la CorrespBox
      sCorresp = string((*i)->pDonnees->nom) + string(" ") + string((*i)->pDonnees->prenom);
      pCorrespBox->AddString(sCorresp.c_str());
   }

   pAdresse->FormatLines(true);
   pAdresse->SetText("");
   pNbExpl->SetText("");

   if (!aCorrespArray.empty())
   {
   	pCorrespBox->SetSelIndex(0);
   	AfficheAdresse(aCorrespArray[0]);
   	AfficheChoixPubli(aCorrespArray[0]);
   }
   else
   {
   	pImprimante->Uncheck();
      pEmail->Uncheck();
      pIntranet->Uncheck();
      // pServeur->Uncheck();
      // pLettre->Uncheck();
   }
}

void
PubliCorrespDialog::SetupWindow()
{
	TDialog::SetupWindow();
	//
	// Remplissage de CorrespBox avec le NSCorrespArray
	//
   AfficheCorresp();
}

void
PubliCorrespDialog::LanceChoixCorrespDialog()
{
	ChoixCorrespDialog(pSuper->pNSApplication->GetMainWindow(),pSuper, pPubli).Execute();
	AfficheCorresp();
}

void
PubliCorrespDialog::SetNbExpl(NSCorrespondantInfo *pCorrespSelect, char* sNbExpl)
{
	for (int index = 0; index < MAXSELECT; index ++)
   {
   	if (!strcmp(pCorrespSelect->pDonnees->code, pPubli->aChoixPubli[index].corresp))
      {
      	if (pPubli->aChoixPubli[index].imprimante)
         {
      		strcpy(pPubli->aChoixPubli[index].nb_expl, sNbExpl);
         	break;
         }
      }
   }
}

void
PubliCorrespDialog::AfficheChoixPubli(NSCorrespondantInfo *pCorrespSelect)
{
	for (int index = 0; index < MAXSELECT; index ++)
   {
   	if (!strcmp(pCorrespSelect->pDonnees->code, pPubli->aChoixPubli[index].corresp))
      {
      	if (pPubli->aChoixPubli[index].imprimante)
         {
         	pImprimante->Check();
            pNbExpl->SetText(pPubli->aChoixPubli[index].nb_expl);
         }
         else
         {
         	pImprimante->Uncheck();
            pNbExpl->SetText("");
         }

         if (pPubli->aChoixPubli[index].email)
         	pEmail->Check();
         else
         	pEmail->Uncheck();

         if (pPubli->aChoixPubli[index].intranet)
         	pIntranet->Check();
         else
         	pIntranet->Uncheck();

         /*
         if (pPubli->aChoixPubli[index].serveur)
         	pServeur->Check();
         else
         	pServeur->Uncheck();

         if (pPubli->aChoixPubli[index].lettre)
         	pLettre->Check();
         else
         	pLettre->Uncheck();
         */

         break;
      }
   }
}

void
PubliCorrespDialog::AfficheAdresse(NSCorrespondantInfo *pCorrespSelect)
{
   string sAdresse = "";

   sAdresse = pCorrespSelect->pAdresseInfo->StringAdresse();

   pAdresse->SetText(sAdresse.c_str());
}

void
PubliCorrespDialog::CmSelectCorresp(WPARAM Cmd)
{
	UINT CorrespChoisi;
   NSCorrespondantInfo *pCorrespSelect;

   CorrespChoisi = pCorrespBox->GetSelIndex();
   pCorrespSelect = aCorrespArray[CorrespChoisi];

   AfficheAdresse(pCorrespSelect);
   AfficheChoixPubli(pCorrespSelect);
}

void
PubliCorrespDialog::CmCorrespDblClk(WPARAM Cmd)
{
}

void
PubliCorrespDialog::CmClickImprimante()
{
	UINT CorrespChoisi;
   NSCorrespondantInfo *pCorrespSelect;

   CorrespChoisi = pCorrespBox->GetSelIndex();
   pCorrespSelect = aCorrespArray[CorrespChoisi];

   for (int index = 0; index < MAXSELECT; index ++)
   {
   	if (!strcmp(pCorrespSelect->pDonnees->code, pPubli->aChoixPubli[index].corresp))
      {
      	if (pPubli->aChoixPubli[index].imprimante)
         {
         	pPubli->aChoixPubli[index].imprimante = false;
            strcpy(pPubli->aChoixPubli[index].nb_expl, "");
         }
         else
         {
         	pPubli->aChoixPubli[index].imprimante = true;
            strcpy(pPubli->aChoixPubli[index].nb_expl, "1");
         }

         break;
      }
   }
   AfficheChoixPubli(pCorrespSelect);
}

void
PubliCorrespDialog::CmClickEmail()
{
	UINT CorrespChoisi;
   NSCorrespondantInfo *pCorrespSelect;

   CorrespChoisi = pCorrespBox->GetSelIndex();
   pCorrespSelect = aCorrespArray[CorrespChoisi];

   for (int index = 0; index < MAXSELECT; index ++)
   {
   	if (!strcmp(pCorrespSelect->pDonnees->code, pPubli->aChoixPubli[index].corresp))
      {
      	if (pPubli->aChoixPubli[index].email)
         	pPubli->aChoixPubli[index].email = false;
         else
         	pPubli->aChoixPubli[index].email = true;
         break;
      }
   }
   AfficheChoixPubli(pCorrespSelect);
}

void
PubliCorrespDialog::CmClickIntranet()
{
	UINT CorrespChoisi;
   NSCorrespondantInfo *pCorrespSelect;

   CorrespChoisi = pCorrespBox->GetSelIndex();
   pCorrespSelect = aCorrespArray[CorrespChoisi];

   for (int index = 0; index < MAXSELECT; index ++)
   {
   	if (!strcmp(pCorrespSelect->pDonnees->code, pPubli->aChoixPubli[index].corresp))
      {
      	if (pPubli->aChoixPubli[index].intranet)
         	pPubli->aChoixPubli[index].intranet = false;
         else
         	pPubli->aChoixPubli[index].intranet = true;
         break;
      }
   }
   AfficheChoixPubli(pCorrespSelect);
}

void
PubliCorrespDialog::CmClickServeur()
{
	UINT CorrespChoisi;
   NSCorrespondantInfo *pCorrespSelect;

   CorrespChoisi = pCorrespBox->GetSelIndex();
   pCorrespSelect = aCorrespArray[CorrespChoisi];

   for (int index = 0; index < MAXSELECT; index ++)
   {
   	if (!strcmp(pCorrespSelect->pDonnees->code, pPubli->aChoixPubli[index].corresp))
      {
      	if (pPubli->aChoixPubli[index].serveur)
         	pPubli->aChoixPubli[index].serveur = false;
         else
         	pPubli->aChoixPubli[index].serveur = true;
         break;
      }
   }
   AfficheChoixPubli(pCorrespSelect);
}

void
PubliCorrespDialog::CmClickLettre()
{
	UINT CorrespChoisi;
   NSCorrespondantInfo *pCorrespSelect;

   CorrespChoisi = pCorrespBox->GetSelIndex();
   pCorrespSelect = aCorrespArray[CorrespChoisi];

   for (int index = 0; index < MAXSELECT; index ++)
   {
   	if (!strcmp(pCorrespSelect->pDonnees->code, pPubli->aChoixPubli[index].corresp))
      {
      	if (pPubli->aChoixPubli[index].lettre)
         	pPubli->aChoixPubli[index].lettre = false;
         else
         	pPubli->aChoixPubli[index].lettre = true;
         break;
      }
   }
   AfficheChoixPubli(pCorrespSelect);
}

void
PubliCorrespDialog::CmOk()
{
	TDialog::CmOk();
}

void
PubliCorrespDialog::CmCancel()
{
	TDialog::CmCancel();
}

// -----------------------------------------------------------------
//
//  Méthodes de ChoixCorrespDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ChoixCorrespDialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_BASEBOX, LBN_SELCHANGE, CmSelectCorrespBase),
	EV_CHILD_NOTIFY_AND_CODE(IDC_BASEBOX, LBN_DBLCLK, CmCorrespBaseDblClk),
   EV_COMMAND(IDOK,CmOk),
   EV_COMMAND(IDCANCEL,CmCancel),
END_RESPONSE_TABLE;

ChoixCorrespDialog::ChoixCorrespDialog(TWindow* pere, NSSuper* pSup, NSPublication* pPublication)
							:NSUtilDialog(pere, pSup, IDD_SELECTCORR)
{
	pPubli = pPublication;
	pCorrespBaseBox 		= new TListBox(this, IDC_BASEBOX);
   pCorrespSelectBox 	= new TListBox(this, IDC_SELECTBOX);
	pCorrespBaseArray 	= &(pPubli->aCorrespBaseArray);
}

ChoixCorrespDialog::~ChoixCorrespDialog()
{
	delete pCorrespBaseBox;
   delete pCorrespSelectBox;
   pCorrespBaseArray = 0;
}

void
ChoixCorrespDialog::SetupWindow()
{
	string sCorresp;
   int CorrespBaseIndex[MAXSELECT];
   int numSelect = 0;
   int j = 0;

  	TDialog::SetupWindow();
	//
	// Remplissage de CorrespBaseBox avec les noms des fichiers correspondants
	//
   for (CorrespInfoIter i = pCorrespBaseArray->begin(); i != pCorrespBaseArray->end(); i++,j++)
   {
      // on remplit la CorrespBox
      sCorresp = string((*i)->pDonnees->nom) + string(" ") + string((*i)->pDonnees->prenom);
      pCorrespBaseBox->AddString(sCorresp.c_str());
      if (pPubli->aChoixPubli[j].select)
      {
      	CorrespBaseIndex[numSelect] = j;
         numSelect++;
      }
   }

   pCorrespBaseBox->SetSelIndexes(CorrespBaseIndex,numSelect,true);

   AfficheSelect();
}

void
ChoixCorrespDialog::AfficheSelect()
{
	string sCorresp;
   NSCorrespondantInfo *pCorrespInfoChoisi;
   int index, numSelect, i;
   int CorrespBaseIndex[MAXSELECT];

   // on vide la liste si elle contient des items
   if (pCorrespSelectBox->GetCount())
   	pCorrespSelectBox->ClearList();

	// Récupération des indices des correspondants sélectionnés
	//
	numSelect = pCorrespBaseBox->GetSelIndexes(CorrespBaseIndex,MAXSELECT);

   for (i=0; i < numSelect; i++)
   {
      index = CorrespBaseIndex[i];
   	pCorrespInfoChoisi = (*(pCorrespBaseArray))[index];
      sCorresp = string(pCorrespInfoChoisi->pDonnees->nom) + string(" ") + string(pCorrespInfoChoisi->pDonnees->prenom);
      pCorrespSelectBox->AddString(sCorresp.c_str());
   }
}

void
ChoixCorrespDialog::CmSelectCorrespBase(WPARAM Cmd)
{
   AfficheSelect();
}

void
ChoixCorrespDialog::CmCorrespBaseDblClk(WPARAM Cmd)
{
}

void
ChoixCorrespDialog::CmOk()
{
   int index, numSelect, i;
   int CorrespBaseIndex[MAXSELECT];

   // on efface l'ancienne sélection
   for (i=0; i < MAXSELECT; i++)
   {
   	pPubli->aChoixPubli[i].select = false;
   }

	// Récupération des indices des correspondants sélectionnés
	//
	numSelect = pCorrespBaseBox->GetSelIndexes(CorrespBaseIndex,MAXSELECT);

   // mise a jour du tableau de NSPublication
   for (i=0; i < numSelect; i++)
   {
      index = CorrespBaseIndex[i];
      pPubli->aChoixPubli[index].select = true;
   	// pCorrespInfoChoisi = (*(pCorrespBaseArray))[index];
      // pPubli->aCorrespArray.push_back(new NSCorrespondantInfo(*pCorrespInfoChoisi));
   }

   TDialog::CmOk();
}

void
ChoixCorrespDialog::CmCancel()
{
	TDialog::CmCancel();
}

/****************** Hook Procedure ********************************/
/* Appelée après SendMessage sur le message WH_CALLWNDPROCRET     */
/******************************************************************/

LRESULT WINAPI CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
   char cName[30];
   string sName;
   LPCWPRETSTRUCT pMsg;

	if (nCode < 0) // do not process msg
   	return CallNextHookEx(hhook, nCode, wParam, lParam);

   switch(nCode)
   {
   	case HC_ACTION:
      	{
            pMsg = (LPCWPRETSTRUCT) lParam;

            switch (pMsg->message)
            {
            	// Attention le seul evenement utilisable est WM_PAINT
               // WM_CREATE arrive trop tot et fait planter le serveur

               case WM_PAINT:

               	// On récupère le nom de la classe de fenetre

                  if (GetClassName(pMsg->hwnd, cName, 30))
            	      sName = string(cName);
                  else
            	      sName = "";

                  // La fenetre d'impression et la fenetre d'annulation
                  // qui est créee quand on envoie un Return ont toutes
                  // les deux le meme nom de classe #32770.
                  // On est donc obligé d'envoyer les Return un coup sur
                  // deux pour ne pas annuler l'impression.

                  if (sName == string("#32770"))
                  {
                     if (bEnvoyerReturn)
                     {
                     	keybd_event(VK_RETURN,0,0,0);
                     	keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
                        bEnvoyerReturn = false;
                     }
                     else bEnvoyerReturn = true;
                  }
                  break;
         	}
         }
         break;

      default:
      	break;
   }

   return CallNextHookEx(hhook, nCode, wParam, lParam);
}

// -----------------------------------------------------------------
//
//  Méthodes de NSPublication
//
// -----------------------------------------------------------------

NSPublication::NSPublication(NSPatientChoisi* pPatientChoisi, NSRefDocument* pDocEnCours)
{
	// Patient en cours
	pPatChoisi = pPatientChoisi;
   pDocMaitre = pDocEnCours;

	codeDoc = string(pDocMaitre->pDocInfo->pDonnees->codePatient) + string(pDocMaitre->pDocInfo->pDonnees->codeDocument);

   if (pDocMaitre->pHtmlInfo) // cas composition deja effectuee
   	codeTmpl = "";
   else // on est en presence d'un document brut
   {    // on récupère la template de composition par défaut du type document
      if (!strncmp(pDocMaitre->pDocInfo->pDonnees->type, "CN", 2))
      	codeTmpl = tempCompo("CN");
      else if (!strncmp(pDocMaitre->pDocInfo->pDonnees->type, "CS", 2))
      	codeTmpl = tempCompo("CS");
      else
      	codeTmpl = tempCompo(pDocMaitre->pDocInfo->pDonnees->type);
   }
}

NSPublication::~NSPublication()
{
	// !!! Ne pas deleter ces deux pointeurs
   // (ce qui tuerait le patient et le document)
   pPatChoisi = 0;
   pDocMaitre = 0;
}

string
NSPublication::tempCompo(char *typeDoc)
{
   char sCode[4];
   string sErr = "";
   bool trouve = false;
	NSTemplate* pTemplate = new NSTemplate(pNSSuperviseur); // fiche template

	pTemplate->lastError = pTemplate->open();
	if (pTemplate->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier Template.db", 0, pTemplate->lastError);
		delete pTemplate;
		return sErr;
	}

   pTemplate->lastError = pTemplate->debut(dbiWRITELOCK);
   if (pTemplate->lastError != DBIERR_NONE)
	{
		erreur("Erreur de positionnement dans la base des templates.", 0, pTemplate->lastError);
		pTemplate->close();
		delete pTemplate;
		return sErr;
	}

   do
   {
   	pTemplate->lastError = pTemplate->getRecord();
		if (pTemplate->lastError != DBIERR_NONE)
		{
			erreur("Erreur de lecture dans la base des templates.", 0, pTemplate->lastError);
			pTemplate->close();
			delete pTemplate;
			return sErr;
		}

      // si correspond au type document et par defaut

      if ((!strcmp(pTemplate->pDonnees->type, typeDoc)) &&
      	 (!strcmp(pTemplate->pDonnees->defaut, "1")))
      {
      	strcpy(sCode, pTemplate->pDonnees->code);
         trouve = true;
         break;
      }

      // on se positionne sur la template suivante

      pTemplate->lastError = pTemplate->suivant(dbiWRITELOCK);
		if ((pTemplate->lastError != DBIERR_NONE) && (pTemplate->lastError != DBIERR_EOF))
      {
      	erreur("Erreur d'accès à la template suivante.", 0, pTemplate->lastError);
         pTemplate->close();
			delete pTemplate;
			return sErr;
      }

   }
   while (pTemplate->lastError != DBIERR_EOF);

	pTemplate->lastError = pTemplate->close();
   if (pTemplate->lastError != DBIERR_NONE)
   {
		erreur("Erreur de fermeture de la base des templates.", 0, pTemplate->lastError);
		delete pTemplate;
		return sErr;
	}
	delete pTemplate;

   if (trouve)
   	return string(sCode);
   else
   {
		MessageBox(0,"Erreur : Pas de template de composition par defaut dans la base",0,MB_OK);
      return sErr;
   }
}

string
NSPublication::tempPubli(char *typePub)
{
	// par defaut pour l'instant :
   // retourne la template de composition
	return codeTmpl;
}

string
NSPublication::dateSysteme()
{
   char cDate[9];
   TDate dateSys;

   sprintf(cDate,"%4d%02d%02d",(int)dateSys.Year(),(int)dateSys.Month(),(int)dateSys.DayOfMonth());
   return string(cDate);
}

string
NSPublication::heureSysteme()
{
   char cHeure[7];
   TTime heureSys;

   sprintf(cHeure,"%02d%02d%02d",(int)heureSys.Hour(),(int)heureSys.Minute(),(int)heureSys.Second());
   return string(cHeure);
}

bool
NSPublication::ChargeCorrespBase()
{
	int j = 0;

   aCorrespBaseArray.vider();

   for (CorrespInfoIter i = pPatChoisi->pCorrespArray->begin(); i != pPatChoisi->pCorrespArray->end(); i++)
   	aCorrespBaseArray.push_back(new NSCorrespondantInfo(*(*i)));

   for (CorrespInfoIter i = aCorrespBaseArray.begin(); i != aCorrespBaseArray.end(); i++,j++)
   {
   	if (j < MAXSELECT)
      {
      	// Charge les valeurs par défaut des choix publication
   		strcpy(aChoixPubli[j].corresp, (*i)->pDonnees->code);
         aChoixPubli[j].select = false;
         aChoixPubli[j].imprimante = true;
         strcpy(aChoixPubli[j].nb_expl, "1");
         aChoixPubli[j].email = false;
         aChoixPubli[j].intranet = false;
         aChoixPubli[j].serveur = false;
         aChoixPubli[j].lettre = false;
      }
      else
      {
         erreur("Le nombre des correspondants dépasse le maximum des sélectionnés",0,0);
         return false;
      }
   }

   for (int i = j; i < MAXSELECT; i++)
   {
   	strcpy(aChoixPubli[i].corresp, "");
      aChoixPubli[i].select = false;
      aChoixPubli[i].imprimante = false;
      strcpy(aChoixPubli[i].nb_expl, "");
      aChoixPubli[i].email = false;
      aChoixPubli[i].intranet = false;
      aChoixPubli[i].serveur = false;
      aChoixPubli[i].lettre = false;
   }

   return true;
}

bool
NSPublication::ExisteCorrespSelect()
{
	bool bExiste = false;

	for (int i = 0; i < MAXSELECT; i++)
   {
   	if (aChoixPubli[i].select)
      {
      	bExiste = true;
         break;
      }
   }

   return bExiste;
}

bool
NSPublication::AppendWorkflow(char *dest, char *type, char *nb_expl, string tmpl)
{
   NSWorkflow* pWorkflow = new NSWorkflow(pPatChoisi->pSuper);
	pWorkflow->lastError = pWorkflow->open();
	if (pWorkflow->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier Workflow.db", 0, pWorkflow->lastError);
		delete pWorkflow;
		return false;
	}


   strcpy(pWorkflow->pDonnees->code_docu, codeDoc.c_str());
   strcpy(pWorkflow->pDonnees->code_dest, dest);
   strcpy(pWorkflow->pDonnees->type_publi, type);
   strcpy(pWorkflow->pDonnees->date_publi, dateSysteme().c_str());
   strcpy(pWorkflow->pDonnees->heure_publi, heureSysteme().c_str());
   strcpy(pWorkflow->pDonnees->nb_publi, nb_expl);
   strcpy(pWorkflow->pDonnees->tmpl_publi, tmpl.c_str());
   strcpy(pWorkflow->pDonnees->ar_publi, "0");
   strcpy(pWorkflow->pDonnees->valide, "1");

   pWorkflow->lastError = pWorkflow->appendRecord();

	if (pWorkflow->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ajout d'une fiche Workflow.", 0, pWorkflow->lastError);
		pWorkflow->close();
		delete pWorkflow;
		return false;
	}

   pWorkflow->lastError = pWorkflow->close();
   if (pWorkflow->lastError != DBIERR_NONE)
		erreur("Erreur de fermeture du fichier Workflow.", 0, pWorkflow->lastError);

   delete pWorkflow;

   return true;
}

bool
NSPublication::RemplitWorkflow()
{
	for (int i=0; i < MAXSELECT; i++)
   {
   	if (aChoixPubli[i].select)
      {
         if (aChoixPubli[i].imprimante)
         {
         	AppendWorkflow(aChoixPubli[i].corresp, "I", aChoixPubli[i].nb_expl, tempPubli("I"));
         }

         if (aChoixPubli[i].email)
         {
         	AppendWorkflow(aChoixPubli[i].corresp, "E", "1", tempPubli("E"));
         }

         if (aChoixPubli[i].intranet)
         {
         	AppendWorkflow(aChoixPubli[i].corresp, "R", "1", tempPubli("R"));
         }

         if (aChoixPubli[i].serveur)
         {
         	AppendWorkflow(aChoixPubli[i].corresp, "S", "1", tempPubli("S"));
         }
      }
   }

   return true;
}

string
NSPublication::StringAdresse(char *codeCorresp)
{
   string sAdresse = "";
   NSCorrespondantInfo *pCorrespSelect;

   for (CorrespInfoIter k = aCorrespBaseArray.begin(); k != aCorrespBaseArray.end(); k++)
   {
   	if (!strcmp((*k)->pDonnees->code, codeCorresp))
      {
      	pCorrespSelect = *k;
         break;
      }
   }

   sAdresse = pCorrespSelect->pDonnees->donneTitre() + string("\n");
   sAdresse += pCorrespSelect->pAdresseInfo->StringAdresse(false);

   return sAdresse;
}

string
NSPublication::NomCorresp(char *codeCorresp)
{
	string sNom = "";
   NSCorrespondantInfo *pCorrespSelect;

   for (CorrespInfoIter k = aCorrespBaseArray.begin(); k != aCorrespBaseArray.end(); k++)
   {
   	if (!strcmp((*k)->pDonnees->code, codeCorresp))
      {
      	pCorrespSelect = *k;
         break;
      }
   }

   sNom = string(pCorrespSelect->pDonnees->prenom);
   if (sNom != "")
   	sNom += string(" ");
   sNom += string(pCorrespSelect->pDonnees->nom);

   return sNom;
}

void
NSPublication::Publication()
{
	bool bImpress = false;

   // recuperation de la template de publication

   if (pDocMaitre->pHtmlInfo)
   {
     	// pour l'instant on prend la template de composition
      // la donnee fichier contient le path complet de la template
     	pDocMaitre->sTemplate = string(pDocMaitre->pHtmlInfo->pDonnees->fichier);
   }
   else // on recupere la template de publication par defaut
   {
   	pDocMaitre->sTemplate = pNSSuperviseur->PathName("NTPL");

     	if (!strncmp(pDocMaitre->pDocInfo->pDonnees->type,"CN",2))
     		pDocMaitre->sTemplate += string("dfmodcn.htm");
      else if (!strncmp(pDocMaitre->pDocInfo->pDonnees->type,"CS",2))
 			pDocMaitre->sTemplate += string("dfmodcs.htm");
      else if (!strcmp(pDocMaitre->pDocInfo->pDonnees->type,"TNRTF"))
         pDocMaitre->sTemplate += string("dfmodrtf.htm");
   }

   // On envoie d'abord les e-mail
   // L'impression est lancée plus tard à cause du hook
   if (!aCorrespBaseArray.empty())
   {
		for (int i = 0; i < MAXSELECT; i++)
   	{
   		if (aChoixPubli[i].select)
      	{
         	if (!bImpress && (aChoixPubli[i].imprimante))
            	bImpress = true;

            if (aChoixPubli[i].email)
            {
            	if (!Exporter(i, pNSSuperviseur->PathName("IHTM")))
            	{
               	char msg[100];
                  string sDest = NomCorresp(aChoixPubli[i].corresp);
                  sprintf(msg,"Impossible d'envoyer le document destiné à %s.",sDest.c_str());
               	erreur(msg,0,0);
               }
            }

            if (aChoixPubli[i].serveur)
            {
            	if (!Exporter(i, pNSSuperviseur->PathName("SIDX")))
            	{
               	char msg[100];
                  string sDest = NomCorresp(aChoixPubli[i].corresp);
                  sprintf(msg,"Impossible d'envoyer le document destiné à %s.",sDest.c_str());
               	erreur(msg,0,0);
               }
            }
      	}
   	}
   }
   else bImpress = true;

   numImpress = 0;
   indexSelect = -1;

   // Lancement de l'impression : gérée par le browser
   if (bImpress)
   {
   	// On lance le create du TOleDialog
   	pDocMaitre->pWebBrowserPubli->Create();

      if (aCorrespBaseArray.empty())
   		Imprimer();
   	else
      {
   		ImprimerSuivant();

         // si aucun exemplaire n'a été imprimé, on doit fermer le browser
         // (ce qui est normalement fait par le dernier Print())
         if (!numImpress)
         	pDocMaitre->pWebBrowserPubli->CmOk();
      }
   }
}

// fonction appelée par le browser en fin d'impression
// Retourne false si on relance une impression
// Retourne true si la tache d'impression est terminée
bool
NSPublication::ImprimerSuivant()
{
	int i;
   bool bFinish = false;

   if (!aCorrespBaseArray.empty())
   {
   	for (i = indexSelect+1; i < MAXSELECT; i++)
   	{
   		if ((aChoixPubli[i].select) && (aChoixPubli[i].imprimante))
      	{
         	indexSelect = i;

            if (!Imprimer(i))
            {
            	// dans ce cas on essaie directement d'imprimer le suivant (pas de break)
					char msg[100];
               string sDest = NomCorresp(aChoixPubli[i].corresp);
               sprintf(msg,"Impossible d'imprimer le document destiné à %s.",sDest.c_str());
               erreur(msg,0,0);
            }
         	else break;
      	}
   	}

   	if (i == MAXSELECT) // plus de corresp => on ferme le browser
   		bFinish = true;
   }
   else bFinish = true;	// cas sans corresp => on ferme le browser

   return bFinish;
}

// Fonction appelée par CmOk et CmCancel de TDIEPubli :
// Destruction des fichiers temporaires d'impression
///////////////////////////////////////////////////////
void
NSPublication::DeleteFichPubli()
{
	string pathHtml = pNSSuperviseur->PathName("SHTM");

	for (int j = 0; j < numImpress; j++)
   {
   	sFichHtml[j] = pathHtml + sFichHtml[j];
      if (!DeleteFile(sFichHtml[j].c_str()))
      	erreur("Pb de destruction d'un fichier de publication",0,0);
   }
   numImpress = 0;
}

void
NSPublication::InstalleHook()
{
	hhook = SetWindowsHookEx(WH_CALLWNDPROCRET, CallWndRetProc, (HINSTANCE) NULL, GetCurrentThreadId());
}

void
NSPublication::LibereHook()
{
	UnhookWindowsHookEx(hhook);
}

bool
NSPublication::Imprimer(int index)
{
	string sAdrCorresp;
   string sPathHtml;
	char   nomFichHtml[100];

   sPathHtml = pNSSuperviseur->PathName("SHTM");

   // generation du compte-rendu à la bonne adresse
   if (index >= 0)
   {
   	sAdrCorresp = StringAdresse(aChoixPubli[index].corresp);
      sprintf(nomFichHtml,"%s-%s",codeDoc.c_str(),aChoixPubli[index].corresp);
   }
   else
   {
   	sAdrCorresp = "";
      sprintf(nomFichHtml,"%s-000",codeDoc.c_str());
   }

   sFichHtml[numImpress] = string(nomFichHtml);

   if (!pDocMaitre->GenereHtml(sPathHtml,sFichHtml[numImpress],toImprimer,sAdrCorresp))
   {
    	erreur("Impossible de créer le fichier html à imprimer",0,0);
      return false;
   }

   int n = atoi(aChoixPubli[index].nb_expl);

   // on lance le navigate qui est ensuite intercepté par l'objet browser
   do
   {
   	n--;
   	pDocMaitre->pWebBrowserPubli->Navigate(sFichHtml[numImpress],n);
   }
   while (n);

   numImpress++;

   return true;
}

bool
NSPublication::Exporter(int index, string sPathHtml)
{
	string sAdrCorresp;
   string sNomHtml;
	char cNumImpress[20];

   // nom du fichier à exporter : nom document + nom patient + prenom patient
   sNomHtml = string(pDocMaitre->pDocInfo->pDonnees->nom) + string("_");
   sNomHtml += string(pNSSuperviseur->pUtilisateur->pPatient->pDonnees->nom) + string("_");
   sNomHtml += string(pNSSuperviseur->pUtilisateur->pPatient->pDonnees->prenom);

   // generation du compte-rendu à la bonne adresse
   if (index >= 0)
   	sAdrCorresp = StringAdresse(aChoixPubli[index].corresp);
   else
   	sAdrCorresp = "";

	if (!pDocMaitre->GenereHtml(sPathHtml,sNomHtml,toExporter,sAdrCorresp,sPathHtml))
   {
   	erreur("Impossible de créer le fichier html à exporter",0,0);
   	return false;
   }

   return true;
}

void
NSPublication::Publier()
{
	if (!ChargeCorrespBase())
   	return;

   if (!aCorrespBaseArray.empty())
   {
		if (aCorrespBaseArray.size() > 1)
      {
   		if (ChoixCorrespDialog(pPatChoisi->pSuper->pNSApplication->GetMainWindow(),pPatChoisi->pSuper, this).Execute() == IDCANCEL)
   			return;
      }
      else
      {
      	aChoixPubli[0].select = true;
      }

   	if (PubliCorrespDialog(pPatChoisi->pSuper->pNSApplication->GetMainWindow(),pPatChoisi->pSuper, this).Execute() == IDCANCEL)
   		return;

   	RemplitWorkflow();
   }

   if ((aCorrespBaseArray.empty()) || (ExisteCorrespSelect()))
   {
   	// si le document n'est ni un compte-rendu, ni un RTF :
   	if ((strncmp(pDocMaitre->pDocInfo->pDonnees->type,"CN",2)) &&
      	 (strncmp(pDocMaitre->pDocInfo->pDonnees->type,"CS",2)) &&
      	 (strcmp(pDocMaitre->pDocInfo->pDonnees->type,"TNRTF")))
      {
      	erreur("Ce type de document n'est pas géré par la publication",0,0);
         return;
      }
      // sinon :
   	Publication();
   }
}

// fin de nscompub.cpp
