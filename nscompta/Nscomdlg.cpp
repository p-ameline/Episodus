// nscomdlg.cpp : fichier des dialogues compta

#include <stdio.h>
#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nspatdlg.h"
#include "nscompta\nscompta.h"
#include "nscompta\nscompta.rh"
#include "nscompta\nscomdlg.h"
// ----------------------------------------------------------------------// 					classe NSOrganEdit : pour les organismes (caisse, centre)
// ----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSOrganEdit, NSUtilEdit)
   EV_WM_CHAR,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSOrganEdit::NSOrganEdit(NSContexte *pCtx, NSUtilDialog* pUtilDialog, int resId, int iTextLen)
            :NSUtilEdit(pCtx, pUtilDialog, resId, iTextLen)
{
}

//
// Destructeur
//
NSOrganEdit::~NSOrganEdit()
{
}

void
NSOrganEdit::SetupWindow()
{
	NSUtilEdit::SetupWindow() ;
}

voidNSOrganEdit::EvChar(uint key, uint repeatCount, uint flags)
{
  if( (key == VK_RETURN) || (key == VK_TAB) )
   	//Process this message to avoid message beeps.
    return ;
  else
    TEdit::EvChar(key, repeatCount, flags) ;
}

boolNSOrganEdit::GetCode(string& sCode)
{
  sCode = GetText(10) ;
  return true;
}

// -----------------------------------------------------------------//
//  Méthodes de NSListBenWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListBenWindow, TListWindow)
   EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Function: NSListBenWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListBenWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
    pDlg->CmModifier() ;
}


//---------------------------------------------------------------------------
//  Function: NSListBenWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListBenWindow::IndexItemSelect()
{
	int count = GetItemCount();
   int index = -1;

   for (int i = 0; i < count; i++)
   	if (GetItemState(i, LVIS_SELECTED))
      {
      	index = i;
         break;
      }

   return index;
}
// -----------------------------------------------------------------//
//  Méthodes de CreerCarteVitaleDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CreerCarteVitaleDialog, NSUtilDialog)
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
   EV_LVN_GETDISPINFO(IDC_CV_LISTBEN, LvnGetDispInfo),
   EV_BN_CLICKED(IDC_CV_AJOUTER, CmAjouter),
   EV_BN_CLICKED(IDC_CV_MODIFIER, CmModifier),
   EV_BN_CLICKED(IDC_CV_ENLEVER, CmEnlever),
END_RESPONSE_TABLE;

//// Constructeur
//
CreerCarteVitaleDialog::CreerCarteVitaleDialog(TWindow* pere, NSContexte* pCtx)
                       :NSUtilDialog(pere, pCtx, "IDD_CVITALE", pNSResModule)
{
  // Initialisation des donnees
  pData        = new NSVitale1Data ;

  // Création de tous les "objets de contrôle"
	pCaisse      = new NSOrganEdit(pContexte, this, IDC_CV_CAISSE, 35) ;
  pNumImma     = new NSUtilEdit2(pContexte, this, IDC_CV_NUM, CSV1_IMMATRICUL_LEN) ;
  pCentre      = new NSOrganEdit(pContexte, this, IDC_CV_CENTRE, 35) ;
  pCodeGes     = new NSUtilEdit2(pContexte, this, IDC_CV_CODEGES, CSV1_CODE_GEST_LEN) ;
  pCodeReg     = new NSUtilEdit2(pContexte, this, IDC_CV_CODEREG, CSV1_REGIME_LEN) ;
  pAdrCaisse   = new NSUtilEdit(pContexte, this, IDC_CV_ADRCAISSE) ;

  pListeBenef  = new NSListBenWindow(this, IDC_CV_LISTBEN) ;
  pBenefArray  = new NSVitale2Array ;
  pBenefSelect = new NSVitale2Info() ;

  nbBenef = 0 ;
  bContientAssure = false ;
}

//
// Destructeur
//
CreerCarteVitaleDialog::~CreerCarteVitaleDialog()
{
  delete pData ;
  delete pCaisse ;
  delete pNumImma ;
  delete pCentre ;
  delete pCodeGes ;
  delete pCodeReg ;
  delete pAdrCaisse ;
	delete pBenefSelect ;
  delete pBenefArray ;
  delete pListeBenef ;
}

//
// Fonction SetupWindow : Mise en place des data à l'écran et initialisation
//	du tableau des bénéficiaires + liste
//
void
CreerCarteVitaleDialog::SetupWindow()
{
	TDialog::SetupWindow();

   pCaisse->SetText(pData->caisse);
   pNumImma->SetText(pData->immatricul);
   pCentre->SetText(pData->centre);
   pCodeGes->SetText(pData->codeGest);
   pCodeReg->SetText(pData->regime);
   // pAdrCaisse : à voir

   if (InitBenefArray())
   {
   	InitListe();
   	AfficheListe();
   }
}

//// Initialise le tableau des bénéficiaires rattachés au meme numéro
// (cad à la meme carte vitale 1)
//
bool
CreerCarteVitaleDialog::InitBenefArray(bool bMajBase)
{
	string 		sCodeBenef;
   string 		sNumImma = "";
   int			compteur = 2;		// par defaut ne contient pas d'assure


   // on se remet dans les conditions initiales
   pBenefArray->vider();
   nbBenef = 0;
   bContientAssure = false;

   pNumImma->GetText(sNumImma);

   if (sNumImma != "")
   {
   	sCodeBenef = sNumImma + string("01");

      NSVitale2* pBenef = new NSVitale2(pContexte->getSuperviseur());

   	pBenef->lastError = pBenef->open();
   	if (pBenef->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à l'ouverture de la base carte vitale.",standardError,0);
      	delete pBenef;
      	return false;
   	}

      pBenef->lastError = pBenef->chercheClef(&sCodeBenef,
      															"",
																	0,
																	keySEARCHGEQ,
																	dbiWRITELOCK);

      if (pBenef->lastError == DBIERR_BOF)	// cas fichier vide
      {
      	pBenef->close();
      	delete pBenef;
         return true;		// le tableau est vide
      }

   	if ((pBenef->lastError != DBIERR_NONE) && (pBenef->lastError != DBIERR_EOF))
   	{
			erreur("Erreur à la recherche d'un bénéficiaire.", standardError, pBenef->lastError);
     		pBenef->close();
			delete pBenef;
			return false;
		}

   	while (pBenef->lastError != DBIERR_EOF)
   	{
   		pBenef->lastError = pBenef->getRecord();
   		if (pBenef->lastError != DBIERR_NONE)
   		{
				erreur("Erreur à la lecture d'un bénéficiaire.", standardError, pBenef->lastError);
      		pBenef->close();
				delete pBenef;
				return false;
			}

         // condition d'arret
      	if (!(string(pBenef->pDonnees->immatricul) == sNumImma)) break;

         // on vérifie la presence de l'assuré
			if (!bContientAssure)
         {
				if (!strcmp(pBenef->pDonnees->qualite, "00"))
            {
            	compteur = 1;
            	bContientAssure = true;
            }
         }

         if (bMajBase)
         {
         	char num[3];
      		sprintf(num, "%02d", compteur);
         	strcpy(pBenef->pDonnees->numero, num);

      		pBenef->lastError = pBenef->modifyRecord(TRUE);
   			if (pBenef->lastError != DBIERR_NONE)
   			{
   				erreur("Erreur à la modification de la fiche carte vitale 2.",standardError,pBenef->lastError);
      			pBenef->close();
      			delete pBenef;
      			return false;
   			}

            compteur++;
         }

         // on remplit le tableau
         pBenefArray->push_back(new NSVitale2Info(pBenef));
      	nbBenef++;

      	// ... on passe au composant suivant
      	pBenef->lastError = pBenef->suivant(dbiWRITELOCK);
   		if ((pBenef->lastError != DBIERR_NONE) && (pBenef->lastError != DBIERR_EOF))
   		{
				erreur("Erreur à la lecture d'un bénéficiaire.", standardError, pBenef->lastError);
      		pBenef->close();
				delete pBenef;
				return false;
			}
   	} // fin du while (recherche des composants images)

      // on ferme la base CARTE_SV2
      pBenef->lastError = pBenef->close();
   	if (pBenef->lastError != DBIERR_NONE)
   	{
			erreur("Erreur de fermeture du fichier ChemDoc.", standardError, pBenef->lastError);
			delete pBenef;
			return false;
		}

   	delete pBenef;
   }

   return true;
}

//
// Initialise la liste (ListWindow) des bénéficiaires
//
void
CreerCarteVitaleDialog::InitListe()
{
	TListWindColumn colNom("Nom", 100, TListWindColumn::Left, 0);
  	pListeBenef->InsertColumn(0, colNom);
  	TListWindColumn colPrenom("Prenom", 100, TListWindColumn::Left, 1);
  	pListeBenef->InsertColumn(1, colPrenom);
   TListWindColumn colDateNaiss("Date naiss", 80, TListWindColumn::Left, 1);
  	pListeBenef->InsertColumn(2, colDateNaiss);
   TListWindColumn colRangGem("RG", 30, TListWindColumn::Left, 1);
  	pListeBenef->InsertColumn(3, colRangGem);
   TListWindColumn colQualite("Qualite", 80, TListWindColumn::Left, 1);
  	pListeBenef->InsertColumn(4, colQualite);
   TListWindColumn colAmo("AMO", 50, TListWindColumn::Left, 1);
  	pListeBenef->InsertColumn(5, colAmo);
   TListWindColumn colMut("MUT", 50, TListWindColumn::Left, 1);
  	pListeBenef->InsertColumn(6, colMut);
}

//
// Affiche dans la liste les donnees du tableau
//
void
CreerCarteVitaleDialog::AfficheListe()
{
   char nom[255];

	pListeBenef->DeleteAllItems();

	for (int i = nbBenef - 1; i >= 0; i--)
   {
   	sprintf(nom, "%s", ((*pBenefArray)[i])->pDonnees->nomPatro);
   	TListWindItem Item(nom, 0);
      pListeBenef->InsertItem(Item);
   }
}
//// LvnGetDispInfo : Pour afficher les subitems (colonnes 2 à n)
// de la ListWindow
//
void
CreerCarteVitaleDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255;
   static char buffer[BufLen];
   TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;
   int 			indexQualite;
   char far		qualite[100];
   char			dateNaissance[11] = "";
   string		sDateNS = string("") ;
   string		sQualite[10] = {"Assuré","Ascendant / Descendant","Conjoint",
											"Conjoint divorcé","Concubin","Conjoint séparé",
   										"Enfant","Bénéficiaire hors article 313",
											"Conjoint veuf","Autre ayant droit"};

	int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

   // Affiche les informations en fonction de la colonne

   switch (dispInfoItem.GetSubItem())   {
       case 1: 	// prenom
         sprintf(buffer, "%s", ((*pBenefArray)[index])->pDonnees->prenom);

         dispInfoItem.SetText(buffer);
         break;
       case 2:   // date de naissance          // strncpy(dateNS, ((*pBenefArray)[index])->pDonnees->dateNais, 8);
          sDateNS = donne_date(((*pBenefArray)[index])->pDonnees->dateNais, sLang) ;
          sprintf(buffer, "%s", sDateNS.c_str()) ;
          dispInfoItem.SetText(buffer);
          break;
       case 3: 	// rang gémellaire         sprintf(buffer, "%s", ((*pBenefArray)[index])->pDonnees->rangGem);         dispInfoItem.SetText(buffer);         break;
       case 4: 	// qualite          indexQualite = atoi(((*pBenefArray)[index])->pDonnees->qualite);         strcpy(qualite, (sQualite[indexQualite]).c_str());         sprintf(buffer, "%s", qualite);         dispInfoItem.SetText(buffer);         break;
       case 5: 	// droits AMO         sprintf(buffer, "%s", ((*pBenefArray)[index])->pDonnees->droits);         dispInfoItem.SetText(buffer);         break;       case 6: 	// droits mutuelle         sprintf(buffer, "%s", ((*pBenefArray)[index])->pDonnees->droitsMut);         dispInfoItem.SetText(buffer);         break;   }}

/************************************************************************************

								FAIT PAR InitBenefArray(true)

//
// Fonction MajNumerosCV2 pour remettre les indices (champ numero)
// de la base Carte_SV2 à partir de BenefArray dont tous les éléments ont
// le meme champ immatricul.
//
bool CreerCarteVitaleDialog::MajNumerosCV2()
{
	string		sCodeBenef, sNumImma = string(((*pBenefArray)[0])->pDonnees->immatricul);
	NSVitale2* 	pBenef = new NSVitale2(pSuper);
   int			i = 1;

   bContientAssure = false;		// on reprend le cas par defaut

   pBenef->lastError = pBenef->open();
   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base carte vitale.",0,0);
      delete pBenef;
      return false;
   }

   sCodeBenef = string(((*pBenefArray)[0])->pDonnees->immatricul) +
   				 string(((*pBenefArray)[0])->pDonnees->numero);


   pBenef->lastError = pBenef->chercheClef(&sCodeBenef,
      															"",
																	0,
																	keySEARCHGEQ,
																	dbiWRITELOCK);

   if ((pBenef->lastError != DBIERR_NONE) && (pBenef->lastError != DBIERR_EOF))
   {
   	erreur("Erreur à la recherche d'un bénéficiaire.", 0, pBenef->lastError);
      pBenef->close();
      delete pBenef;
      return false;
   }

   while (pBenef->lastError != DBIERR_EOF)
   {
   	pBenef->lastError = pBenef->getRecord();
      if (pBenef->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la lecture d'un bénéficiaire.", 0, pBenef->lastError);
         pBenef->close();
         delete pBenef;
         return false;
      }

      // condition d'arret
      if (!(string(pBenef->pDonnees->immatricul) == sNumImma)) break;

      // on vérifie la presence de l'assuré (forcement le premier)
      if (!bContientAssure)
      {
      	if (!strcmp(pBenef->pDonnees->qualite, "00"))
         {
         	strcpy(pBenef->pDonnees->numero, "01");
         	bContientAssure = true;
         }
         else // cas sans assuré
         {
         	char num[3];
      		sprintf(num, "%02d", i+1);
         	strcpy(pBenef->pDonnees->numero, num);
         	i++;
         }
      }
      else
      {
      	char num[3];
      	sprintf(num, "%02d", i+1);
         strcpy(pBenef->pDonnees->numero, num);
         i++;
      }

      pBenef->lastError = pBenef->modifyRecord(TRUE);
   	if (pBenef->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la modification de la fiche carte vitale 2.",0,pBenef->lastError);
      	pBenef->close();
      	delete pBenef;
      	return false;
   	}

      // ... on passe au composant suivant
      pBenef->lastError = pBenef->suivant(dbiWRITELOCK);
      if ((pBenef->lastError != DBIERR_NONE) && (pBenef->lastError != DBIERR_EOF))
      {
      	erreur("Erreur à la lecture d'un bénéficiaire.", 0, pBenef->lastError);
         pBenef->close();
         delete pBenef;
         return false;
      }
   } // fin du while

   // on ferme la base CARTE_SV2
   pBenef->lastError = pBenef->close();
   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur de fermeture du fichier ChemDoc.", 0, pBenef->lastError);
      delete pBenef;
      return false;
   }

   delete pBenef;
}
*********************************************************************************/

//
// Fonction InitDataBenef pour la création / modification des blocs 5
//
bool CreerCarteVitaleDialog::InitDataBenef(NSVitale2Info* pBenefInfo, bool bCreer)
{
   CreerBeneficiaireDialog* pBenefDlg;

	pBenefDlg = new CreerBeneficiaireDialog(this, pContexte);

   if (!bCreer)	// on est en modification
   {
   	*(pBenefDlg->pData) = *(pBenefInfo->pDonnees);
   }

   if ((pBenefDlg->Execute()) == IDCANCEL)
   {
   	delete pBenefDlg;
   	return false;
   }

   // on stocke les donnees du dialogue dans les Data
   *(pBenefInfo->pDonnees) = *(pBenefDlg->pData);

   delete pBenefDlg;

   return true;
}

//
// Fonction CmAjouter pour ajouter un nouveau bénéficiaire
// sur la carte vitale
//
void
CreerCarteVitaleDialog::CmAjouter()
{
	NSVitale2Info* pBenefInfo = new NSVitale2Info();
   string 		 	sNumImma = "";
   char				indice[3];

   pNumImma->GetText(sNumImma);

   // On vérifie si le numéro de sécu existe
   if (sNumImma == "")
   {
   	erreur("Vous n'avez pas indiqué le numero d'immatriculation",standardError,0);
      return;
   }

   if (!InitDataBenef(pBenefInfo,true))
   {
   	delete pBenefInfo;
      return;
   }

   // on vérifie l'unicité de l'assuré
   if (bContientAssure && (!strcmp(pBenefInfo->pDonnees->qualite, "00")))
   {
   	erreur("Il exite déjà un assuré sur cette carte",standardError,0);
      delete pBenefInfo;
      return;
   }

   NSVitale2* pBenef = new NSVitale2(pContexte->getSuperviseur());

	// on ajoute les Data à la base des carte vitales
   pBenef->lastError = pBenef->open();
   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base carte vitale 2.",standardError,0);
      delete pBenef;
      delete pBenefInfo;
      return;
   }

   *(pBenef->pDonnees) = *(pBenefInfo->pDonnees);

   // on copie le numero d'immatriculation
   strcpy(pBenef->pDonnees->immatricul, sNumImma.c_str());

   // on ajoute l'indice
   if (strcmp(pBenefInfo->pDonnees->qualite, "00") == 0)
   {
   	strcpy(indice, "01");
   }
   else
   {
   	sprintf(indice, "%02d", nbBenef + 1);
   }

   strcpy(pBenef->pDonnees->numero, indice);

   pBenef->lastError = pBenef->appendRecord();
   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ajout de la fiche carte vitale 2.",standardError,pBenef->lastError);
      pBenef->close();
      delete pBenef;
      delete pBenefInfo;
      return;
   }

   // on ajoute l'élément au tableau ou on l'insère en tete si assuré
   if (strcmp(pBenefInfo->pDonnees->qualite, "00") != 0)
   {
   	pBenefArray->push_back(new NSVitale2Info(*pBenefInfo));
   }
   else
   {
   	if (!pBenefArray->empty())
      {
   		pBenefArray->insert(pBenefArray->begin(), new NSVitale2Info(*pBenefInfo));
      }
      else
      {
      	pBenefArray->push_back(new NSVitale2Info(*pBenefInfo));
      }

      bContientAssure = true;
   }

   nbBenef++;

   pBenef->lastError = pBenef->close();
   if (pBenef->lastError != DBIERR_NONE)
   	erreur("Erreur à la fermeture de la base carte vitale 2.",standardError,pBenef->lastError);

   delete pBenef;
   delete pBenefInfo;

   // on remet la liste à jour
   AfficheListe();
}

//
// Fonction CmModifier : pour modifier le bénéficiaire sélectionné
//
void
CreerCarteVitaleDialog::CmModifier()
{
	NSVitale2Info* pBenefInfo = new NSVitale2Info();
   string 		 	sNumImma = "";
   int 				index = pListeBenef->IndexItemSelect();
   string			sCodeBenef;

   pNumImma->GetText(sNumImma);

   // On vérifie si le numéro de sécu existe
   if (sNumImma == "")
   {
   	erreur("Vous n'avez pas indiqué le numero d'immatriculation",standardError,0);
      return;
   }

   if (index == -1)
   {
   	erreur("Vous devez sélectionner un bénéficiaire",standardError,0);
      return;
   }

   *pBenefInfo = *((*pBenefArray)[index]);

   if (!InitDataBenef(pBenefInfo,false))
   {
   	delete pBenefInfo;
      return;
   }

   NSVitale2* pBenef = new NSVitale2(pContexte->getSuperviseur());

	// on ajoute les Data à la base des carte vitales
   pBenef->lastError = pBenef->open();
   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base carte vitale 2.",standardError,0);
      delete pBenef;
      delete pBenefInfo;
      return;
   }

   sCodeBenef = string(pBenefInfo->pDonnees->immatricul) + string(pBenefInfo->pDonnees->numero);

   pBenef->lastError = pBenef->chercheClef(&sCodeBenef,
      															"",
																	0,
																	keySEARCHEQ,
																	dbiWRITELOCK);

   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à la recherche de la fiche carte vitale 2.",standardError,pBenef->lastError);
      pBenef->close();
      delete pBenef;
      delete pBenefInfo;
      return;
   }

   // on modifie les données d'apres les info du dialogue
   *(pBenef->pDonnees) = *(pBenefInfo->pDonnees);

   pBenef->lastError = pBenef->modifyRecord(TRUE);
   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à la modification de la fiche carte vitale 2.",standardError,pBenef->lastError);
      pBenef->close();
      delete pBenef;
      delete pBenefInfo;
      return;
   }

   // on conserve la modif dans le tableau
   *((*pBenefArray)[index]) = *pBenefInfo;

   pBenef->lastError = pBenef->close();
   if (pBenef->lastError != DBIERR_NONE)
   	erreur("Erreur à la fermeture de la base carte vitale 2.",standardError,pBenef->lastError);

   delete pBenef;
   delete pBenefInfo;

   // on remet le tableau à jour
   // (nécéssaire si la qualite change)
   if (!InitBenefArray())
   {
   	erreur("Pb à la mise à jour du tableau des bénéficiaires",standardError,0);
      return;
   }

   // on remet la liste à jour
   AfficheListe();
}

//
// Fonction CmEnlever : pour supprimer le bénéficiaire sélectionné
// de la carte vitale.
//
void
CreerCarteVitaleDialog::CmEnlever()
{
	NSCV2Iter 		i;
   int 				j;
   NSVitale2Info* pBenefInfo = new NSVitale2Info();
   string 		 	sNumImma = "";
   int 				index = pListeBenef->IndexItemSelect();
   string			sCodeBenef;

   pNumImma->GetText(sNumImma);

   // On vérifie si le numéro de sécu existe
   if (sNumImma == "")
   {
   	erreur("Vous n'avez pas indiqué le numero d'immatriculation",standardError,0);
      return;
   }

   if (index == -1)
   {
   	erreur("Vous devez sélectionner un bénéficiaire",standardError,0);
      return;
   }

   *pBenefInfo = *((*pBenefArray)[index]);

   NSVitale2* pBenef = new NSVitale2(pContexte->getSuperviseur());

	// on ajoute les Data à la base des carte vitales
   pBenef->lastError = pBenef->open();
   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture de la base carte vitale 2.",standardError,0);
      delete pBenef;
      delete pBenefInfo;
      return;
   }

   sCodeBenef = string(pBenefInfo->pDonnees->immatricul) + string(pBenefInfo->pDonnees->numero);

   pBenef->lastError = pBenef->chercheClef(&sCodeBenef,
      															"",
																	0,
																	keySEARCHEQ,
																	dbiWRITELOCK);

   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à la recherche de la fiche carte vitale 2.",standardError,pBenef->lastError);
      pBenef->close();
      delete pBenef;
      delete pBenefInfo;
      return;
   }

   // on supprime l'enregistrement de la base
   pBenef->lastError = pBenef->deleteRecord();
   if (pBenef->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à la suppression de la fiche carte vitale 2.",standardError,pBenef->lastError);
      pBenef->close();
      delete pBenef;
      delete pBenefInfo;
      return;
   }

   pBenef->lastError = pBenef->close();
   if (pBenef->lastError != DBIERR_NONE)
   	erreur("Erreur à la fermeture de la base carte vitale 2.",standardError,pBenef->lastError);

   delete pBenef;
   delete pBenefInfo;

   // on remet le tableau à jour
   if (!InitBenefArray())
   {
   	erreur("Pb à la mise à jour du tableau des bénéficiaires",standardError,0);
      return;
   }

   // on remet la liste à jour
   AfficheListe();
}

//
// Fonction CmOk : on conserve les données saisies dans pData
//
void
CreerCarteVitaleDialog::CmOk()
{
   string         sCode;
   string			sTexte;

   pCaisse->GetCode(sCode);
   strcpy(pData->caisse, sCode.c_str());

   pNumImma->GetText(sTexte);
   strcpy(pData->immatricul, sTexte.c_str());

   pCaisse->GetCode(sCode);
   strcpy(pData->centre, sCode.c_str());

	pCodeGes->GetText(sTexte);
   strcpy(pData->codeGest, sTexte.c_str());

   pCodeReg->GetText(sTexte);
   strcpy(pData->regime, sTexte.c_str());

   if (!strlen(pData->immatricul))
   {
   	erreur("Le champ N° d'immatriculation est obligatoire",standardError,0);
      return;
   }

   // attention ne pas rentrer les old data
   // sinon n'enregistre pas les cartes vides (!!!)
   // - qui contiennent seulement le n° -

   TDialog::CmOk();
}

//
// Fonction CmCancel : on supprime le tableau des nouveaux bénéficiaires
// qui ont été créés par CmAjouter.
//
void
CreerCarteVitaleDialog::CmCancel()
{
   TDialog::CmCancel();
}
// -----------------------------------------------------------------//
//  Méthodes de NumSSDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NumSSDialog, NSUtilDialog)
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NumSSDialog::NumSSDialog(TWindow* pere, NSContexte* pCtx)
            :NSUtilDialog(pere, pCtx, "IDD_NUMSS", pNSResModule)
{
  pNumImma = new NSUtilEdit2(pContexte, this, IDC_NUMSS_EDIT, CSV1_IMMATRICUL_LEN) ;
}

NumSSDialog::~NumSSDialog(){
   delete pNumImma;
}

void
NumSSDialog::SetupWindow()
{
  // fichiers d'aide
  sHindex = "" ;
  sHcorps = "Carte_Vitale.html" ;

  TDialog::SetupWindow() ;
}

voidNumSSDialog::CmOk()
{
  string sNumImma;
  pNumImma->GetText(sNumImma);

  if (sNumImma == "")
  {
   	erreur("Vous devez rentrer le numéro d'immatriculation",standardError,0);
      return;
   }

   strcpy(numImma, sNumImma.c_str());

   TDialog::CmOk();
}


void
NumSSDialog::CmCancel()
{
   TDialog::CmCancel();
}
// -----------------------------------------------------------------//
//  Méthodes de CreerBeneficiaireDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CreerBeneficiaireDialog, NSUtilDialog)
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
   EV_BN_CLICKED(IDC_BNF_PATENCOURS, CmPatEnCours),
   EV_BN_CLICKED(IDC_BNF_AUTREPAT, CmAutrePat),
END_RESPONSE_TABLE;

//
// Constructeur
//
CreerBeneficiaireDialog::CreerBeneficiaireDialog(TWindow* pere, NSContexte* pCtx)
							 : NSUtilDialog(pere, pCtx, "IDD_BENEFICIAIRE", pNSResModule)
{
	// Initialisation des donnees
   pData 				= new NSVitale2Data;
   pCBQualiteArray = new NSComboArray;

   // Création de tous les "objets de contrôle"
   pNomPatro			= new NSUtilEdit2(pContexte, this, IDC_BNF_NOMPATRO, CSV2_NOM_PATRO_LEN) ;
   pNomUsuel			= new NSUtilEdit2(pContexte, this, IDC_BNF_NOMUSUEL, CSV2_NOM_USUEL_LEN) ;
   pPrenom				= new NSUtilEdit2(pContexte, this, IDC_BNF_PRENOM,	CSV2_PRENOM_LEN) ;
   pDateNaiss			= new NSUtilEditDate(pContexte, this, IDC_BNF_DATENAISS) ;
   pHeureNaiss		= new NSUtilEditHeure(pContexte, this, IDC_BNF_HEURENAISS) ;
   pRangGem				= new NSUtilEdit2(pContexte, this, IDC_BNF_RANGGEM,	CSV2_RANG_GEM_LEN) ;
   pAdrBenef			= new NSUtilEdit2(pContexte, this, IDC_BNF_ADRESSE) ;

   pCBQualite			= new TComboBox(this, IDC_BNF_CBQUALITE) ;
   pGBAld				  = new TGroupBox(this, IDC_BNF_GBALD) ;
   pNoAld				  = new TRadioButton(this, IDC_BNF_NOALD, pGBAld) ;
   pDListe1				= new TRadioButton(this, IDC_BNF_DLISTE1, pGBAld) ;
   pHListe1				= new TRadioButton(this, IDC_BNF_HLISTE1, pGBAld) ;
   pMult1				  = new TRadioButton(this, IDC_BNF_MULT1, pGBAld) ;
   pDListe2				= new TRadioButton(this, IDC_BNF_DLISTE2, pGBAld) ;
   pHListe2				= new TRadioButton(this, IDC_BNF_HLISTE2, pGBAld) ;
   pMult2				  = new TRadioButton(this, IDC_BNF_MULT2, pGBAld) ;

   pGBAmo				  = new TGroupBox(this, IDC_BNF_GBAMO) ;
   pAmoOui				= new TRadioButton(this, IDC_BNF_AMOOUI, pGBAmo) ;
   pAmoNon				= new TRadioButton(this, IDC_BNF_AMONON, pGBAmo) ;
   pMutuelle			= new NSUtilEdit2(pContexte, this, IDC_BNF_MUTUELLE, CSV2_MUTUELLE_LEN) ;
   pGBMutu				= new TGroupBox(this, IDC_BNF_GBMUTU) ;
   pMutuOui				= new TRadioButton(this, IDC_BNF_MUTUOUI, pGBMutu) ;
   pMutuNon				= new TRadioButton(this, IDC_BNF_MUTUNON, pGBMutu) ;
   pCodeSitu			= new NSUtilEdit2(pContexte, this, IDC_BNF_CODESITU, CSV2_SITUATION_LEN) ;
   pGaranties			= new NSUtilEdit2(pContexte, this, IDC_BNF_GARANTIES, CSV2_GARANTIES_LEN) ;
}

//// Destructeur
//
CreerBeneficiaireDialog::~CreerBeneficiaireDialog()
{
  delete pData;
  delete pCBQualiteArray;
  delete pNomPatro;
  delete pNomUsuel;
  delete pPrenom;
  delete pRangGem;
  delete pDateNaiss;
  delete pHeureNaiss;
  delete pAdrBenef;
  delete pGBAld;
  delete pNoAld;
  delete pDListe1;
  delete pHListe1;
  delete pMult1;
  delete pDListe2;
  delete pHListe2;
  delete pMult2;
  delete pCBQualite;
  delete pGBAmo;
  delete pAmoOui;
  delete pAmoNon;
  delete pMutuelle;
  delete pGBMutu;
  delete pMutuOui;
  delete pMutuNon;
  delete pCodeSitu;
  delete pGaranties;
}


//
// Fonction SetupWindow : Inititalise la ComboBox et affiche les data
//
void
CreerBeneficiaireDialog::SetupWindow()
{
	TDialog::SetupWindow();

   pAdrBenef->FormatLines(true);
   AfficheDonnees();
}

//
// Fonction AfficheDonnees : Affiche le contenu de pData
//
void
CreerBeneficiaireDialog::AfficheDonnees()
{
	int 			codeAld;
    NSComboIter i;
	int 			j;
    string 		    sFichier = pContexte->PathName("BCPT") + string("benef.dat");

	pNomPatro->SetText(pData->nomPatro);
   pNomUsuel->SetText(pData->nomUsuel);
   pPrenom->SetText(pData->prenom);

   if (strcmp(pData->dateNais, ""))
   {
   		pDateNaiss->setDate(string(pData->dateNais,0,8));
   		pHeureNaiss->setHeure(string(pData->dateNais,8,4));
   }
   pRangGem->SetText(pData->rangGem);
   pAdrBenef->SetText(pData->adresse);

	if (!InitComboBox(pCBQualite, pCBQualiteArray, sFichier))
   	erreur("Pb à l'initialisation de la combobox Qualite",standardError,0);
   else
   	for (i = pCBQualiteArray->begin(), j = 0; i != pCBQualiteArray->end(); i++,j++)
      {
   		if (!strcmp(((*i)->sCode).c_str(), pData->qualite))
      	{
      		pCBQualite->SetSelIndex(j);
         	break;
      	}
      }

   if (!strcmp(pData->ald, ""))
   {
   	codeAld = -1;
   }
   else
   	codeAld = atoi(pData->ald);

   switch (codeAld)
   {
   	case 0 :
      	pNoAld->Check();
         break;
      case 1 :
      	pDListe1->Check();
         break;
      case 2 :
      	pHListe1->Check();
         break;
      case 3 :
      	pMult1->Check();
         break;
      case 4 :
      	pDListe2->Check();
         break;
      case 5 :
      	pHListe2->Check();
      	break;
      case 6 :
      	pMult2->Check();
         break;
   }

   if (!strcmp(pData->droits, "O"))
   	pAmoOui->Check();

   if (!strcmp(pData->droits, "N"))
   	pAmoNon->Check();

   pMutuelle->SetText(pData->mutuelle);

	if (!strcmp(pData->droitsMut, "O"))
   	pMutuOui->Check();

   if (!strcmp(pData->droitsMut, "N"))
   	pMutuNon->Check();

   pCodeSitu->SetText(pData->situation);
   pGaranties->SetText(pData->garanties);
}

/**************************
void CreerBeneficiaireDialog::StockerDonneesPatient(NSPatientChoisi* pPat)
{
	// on sauve ici le code patient
   strcpy(pData->patient, pPat->pDonnees->nss);
   strncpy(pData->nomPatro, pPat->pDonnees->nom, 27);
   strncpy(pData->prenom, pPat->pDonnees->prenom, 27);
   strcpy(pData->adresse, (pPat->pAdresseInfo->StringAdresse()).c_str());
   strncpy(pData->dateNais, pPat->pDonnees->naissance, 8);
}
****************************/

void CreerBeneficiaireDialog::CmPatEnCours()
{
	NSPatientChoisi* pPat = pContexte->getPatient();

   // on sauve ici le code patient
#ifndef _MUE
   strcpy(pData->patient, pPat->pDonnees->nss);
   strncpy(pData->nomPatro, pPat->pDonnees->nom, 27);
   strncpy(pData->prenom, pPat->pDonnees->prenom, 27);
   strcpy(pData->adresse, (pPat->pAdresseInfo->StringAdresse()).c_str());
   strncpy(pData->dateNais, pPat->pDonnees->naissance, 8);#else   strcpy(pData->patient, pPat->getszNss());
   strncpy(pData->nomPatro, pPat->getszNom(), 27);
   strncpy(pData->prenom, pPat->getszPrenom(), 27);
   // strcpy(pData->adresse, (pPat->pAdresseInfo->StringAdresse()).c_str());
   strncpy(pData->dateNais, pPat->getszNaissance(), 8);#endif

   AfficheDonnees();
}

void CreerBeneficiaireDialog::CmAutrePat()
{
	int idRetour;
#ifndef _MUE
	// on ouvre la base des patients pour lancer la liste
   NSPatient* pPatient = new NSPatient(pContexte);

   pPatient->lastError = pPatient->open();
   if (pPatient->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à l'ouverture du fichier Patient.db", 0, pPatient->lastError);
      delete pPatient;
      return;
   }

   string sNomBlanc = string(PAT_NOM_LEN, ' ');
   string sPrenomBlanc = string(PAT_PRENOM_LEN, ' ');
   CURProps curProps;
   DBIResult lastError = DbiGetCursorProps(pPatient->PrendCurseur(), curProps);
   Byte* pIndexRec = new Byte[curProps.iRecBufSize];
   memset(pIndexRec, 0, curProps.iRecBufSize);
   DbiPutField(pPatient->PrendCurseur(), PAT_NOM_FIELD, pIndexRec, (Byte*)sNomBlanc.c_str());
   DbiPutField(pPatient->PrendCurseur(), PAT_PRENOM_FIELD, pIndexRec, (Byte*)sPrenomBlanc.c_str());

   pPatient->lastError = pPatient->chercheClefComposite("NOM_PRENOM",
														    NODEFAULTINDEX,
															keySEARCHGEQ,
															dbiWRITELOCK,
															pIndexRec);
   delete[] pIndexRec;

   if (pPatient->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à la recherche dans le fichier Patient.db", 0, pPatient->lastError);
      delete pPatient;
      return;
   }

   // on appelle la liste des patients
   idRetour = NSListePatDialog(pContexte->GetMainWindow(), pContexte, pPatient).Execute();

   if (idRetour == IDOK)
   {
   	// on récupère l'adresse du patient et on l'affiche
      pPatient->lastError = pPatient->getRecord();
      if (pPatient->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à lecture du fichier Patient.db", 0, pPatient->lastError);
         pPatient->close();
         delete pPatient;
         return;
      }

      pPatient->initAdresseInfo();

      // on remet à jour les donnees patient
      // on sauve ici le code patient
   	strcpy(pData->patient, pPatient->pDonnees->nss);
   	strncpy(pData->nomPatro, pPatient->pDonnees->nom, 27);
   	strncpy(pData->prenom, pPatient->pDonnees->prenom, 27);
   	strcpy(pData->adresse, (pPatient->pAdresseInfo->StringAdresse()).c_str());
   	strncpy(pData->dateNais, pPatient->pDonnees->naissance, 8);
   }

   pPatient->lastError = pPatient->close();
   if (pPatient->lastError != DBIERR_NONE)
   {
   	erreur("Erreur à la fermeture du fichier Patient.db", 0, pPatient->lastError);
      delete pPatient;
      return;
   }

   delete pPatient;

   // on remet à jour
   AfficheDonnees();#else
    erreur("Fonction CmAutrePat non intégrée en MUE.", standardError, 0);
#endif    
}

void
CreerBeneficiaireDialog::CmOk()
{
	NSVitale2Data 	oldData;
   string			sText;
   string			sDateN;
   string			sHeureN;
   int 				indexQual;

   oldData = *pData;

   pNomPatro->GetText(sText);
   strcpy(pData->nomPatro, sText.c_str());

   pNomUsuel->GetText(sText);
   strcpy(pData->nomUsuel, sText.c_str());

   pPrenom->GetText(sText);
   strcpy(pData->prenom, sText.c_str());

   pAdrBenef->GetText(sText);
   strcpy(pData->adresse, sText.c_str());

	pDateNaiss->getDate(&sDateN);
   pHeureNaiss->getHeure(&sHeureN);

   sDateN += sHeureN;
   strcpy(pData->dateNais, (sDateN).c_str());

   pRangGem->GetText(sText);
   strcpy(pData->rangGem, sText.c_str());

   indexQual = pCBQualite->GetSelIndex();
   if (indexQual >= 0)
   {
   	strcpy(pData->qualite, ((*pCBQualiteArray)[indexQual]->sCode).c_str());
   }

   if (pNoAld->GetCheck() == BF_CHECKED)
   	strcpy(pData->ald, "0");
   if (pDListe1->GetCheck() == BF_CHECKED)
   	strcpy(pData->ald, "1");
   if (pHListe1->GetCheck() == BF_CHECKED)
   	strcpy(pData->ald, "2");
   if (pMult1->GetCheck() == BF_CHECKED)
   	strcpy(pData->ald, "3");
   if (pDListe2->GetCheck() == BF_CHECKED)
   	strcpy(pData->ald, "4");
   if (pHListe2->GetCheck() == BF_CHECKED)
   	strcpy(pData->ald, "5");
   if (pMult2->GetCheck() == BF_CHECKED)
   	strcpy(pData->ald, "6");

   if (pAmoOui->GetCheck() == BF_CHECKED)
   	strcpy(pData->droits, "O");
   if (pAmoNon->GetCheck() == BF_CHECKED)
   	strcpy(pData->droits, "N");

	pMutuelle->GetText(sText);
   strcpy(pData->mutuelle, sText.c_str());

   if (pMutuOui->GetCheck() == BF_CHECKED)
   	strcpy(pData->droitsMut, "O");
   if (pMutuNon->GetCheck() == BF_CHECKED)
   	strcpy(pData->droitsMut, "N");

	pCodeSitu->GetText(sText);
   strcpy(pData->situation, sText.c_str());

   pGaranties->GetText(sText);
   strcpy(pData->garanties, sText.c_str());

   if (!strlen(pData->nomPatro))
   {
   	erreur("Le champ Nom Patronymique est obligatoire",standardError,0);
      return;
   }

   if (!strlen(pData->prenom))
   {
   	erreur("Le champ Prénom est obligatoire",standardError,0);
      return;
   }

   if (!strlen(pData->qualite))
   {
   	erreur("Le champ Qualite est obligatoire",standardError,0);
      return;
   }

   if (oldData == (*pData))
   {
   	erreur("Les donnees n'ont pas changé",standardError,0);
   	TDialog::CmCancel();	// pour ne pas mettre à jour la base pour rien
   }
   else
		TDialog::CmOk();
}

void
CreerBeneficiaireDialog::CmCancel()
{
   TDialog::CmCancel();
}

