#include <owl\applicat.h>#include <owl\olemdifr.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <owl\color.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "partage\nsdivfct.h"
#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "nssavoir\nspathor.rh"
#include "nssavoir\nsfilgd.h"
#include "nsbb\nsdicogl.h"
#include "partage\nsglobal.h"
#include "nsbb\nsbb.h"
#include "nsbb\nstrewi.h"

//*******************************************************************
//						classe List   ListeBoxLexique
//*******************************************************************
DEFINE_RESPONSE_TABLE1(ListeBoxLexique, TListBox)
    EV_WM_KEYDOWN,
    EV_LBN_DBLCLK(IDC_PATHORLISTBOX, CmPathoDblClk),
END_RESPONSE_TABLE;

ListeBoxLexique::ListeBoxLexique(TWindow* parent, int ressource, ChoixPathoDialog* pChoixPatho, TModule* module)
				    :TListBox(parent,ressource, module)
{
    pChoixPathoDialog = pChoixPatho;
}

ListeBoxLexique::~ListeBoxLexique(){}

//--------------------------------------------------------------------------// touche interceprt�e par la bo�te lexique
//--------------------------------------------------------------------------
void
ListeBoxLexique::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  TWindow::EvKeyDown(key, repeatCount, flags) ;

  // Note : Dans le cas de NSUtilLexique, le Return est intercept� par EvKeyDown
  // Dans le cas d'un NSEditDico (consultation) le Return est intercept� par CmOk
  // NSUtilLexique n'utilise pas le m�me constructeur de NSEditDicoGlobal que NSEditDico
  if ((VK_INSERT == key) || (VK_RETURN == key))
  {
    pChoixPathoDialog->_iElementChoisi = GetSelIndex() ; //indice de l'�l�ment choisi
    if (pChoixPathoDialog->_pEdit)
      pChoixPathoDialog->_pEdit->ElementSelectionne() ;
    return ;
  }

/*   NSEditDico* pEditDico =  static_cast<NSEditDico*>(pChoixPathoDialog->pEdit);
   if(!pEditDico)
   	return;

   NSTreeWindow* pTree = pEditDico->GetpTreeAEditer();

   //fl�che basse
   if( key == VK_DOWN )
   {
      if(pTree->ModifierTexteLibre)
      {
         if(pTree->pNSTreeNodeEnCours->sTexteGlobal == "")
	         pTree->ModifierTexteLibre = false;
      	return;
      }
      //si ammorce vide alors passer au NStrreNode suivant
      if( pChoixPathoDialog->sAmmorce == "" )
         pTree->NStreeNodeSuivant();
      //sinon r�cup�rer un �l�ment dans le lexique
      else
      	pChoixPathoDialog->ElementSuivant();
   }
   //fl�che haute
   else if(key == VK_UP)
   {
   	//si ammorce vide alors passer au NStrreNode suivant
      if(pTree->ModifierTexteLibre)
      {
         if(pTree->pNSTreeNodeEnCours->sTexteGlobal == "")
	         pTree->ModifierTexteLibre = false;
      	return;
      }
      if( pChoixPathoDialog->sAmmorce == "" )
         pTree->NStreeNodePrcedent();
      //sinon r�cup�rer un �l�ment dans le lexique
      else
      	pChoixPathoDialog->ElementPrecedent();
   }
*/
}

voidListeBoxLexique::CmPathoDblClk()
{
  if (GetCount() <= 0)
    return ;
  pChoixPathoDialog->_iElementChoisi = GetSelIndex() ; //indice de l'�l�ment choisi
  if (pChoixPathoDialog->_pEdit)
    pChoixPathoDialog->_pEdit->ElementSelectionne() ;
}

//*******************************************************************
// -----------------------------------------------------------------
//
//  M�thodes de ChoixPathoDialog
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixPathoDialog, TDialog)
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
	EV_WM_KEYDOWN,
	EV_CHILD_NOTIFY_AND_CODE(IDC_PATHORLISTBOX, WM_KEYDOWN, CmSelectPatho),
	EV_CHILD_NOTIFY_AND_CODE(IDC_PATHORLISTBOX, LBN_DBLCLK, CmPathoDblClk),
	EV_BN_CLICKED(MEDICAMENT,LexiqueMedicament),
	EV_BN_CLICKED(GENERAL, LexiqueGeneral),
	EV_WM_CLOSE,
	EV_WM_DESTROY,      // �venement re�u lorsqu'on fait Return
    //le lexique re�oit l'�v�nement de se fermer, mais il se contente de
    // se cacher
END_RESPONSE_TABLE ;

ChoixPathoDialog::ChoixPathoDialog(TWindow* pere, NSDico* pDictionnaire,                                    NSEditDicoGlobal* pEdite, string sTypeLexiq,
                                    TModule* module)
                 :TDialog(pere, "LEXIQUE", module), _pDico(pDictionnaire)
{
try
{
	_sTypeLexique = sTypeLexiq ;
	_bOuvert      = false ;
	_bSetup       = false ;
	_pEdit        = pEdite ;
	_pLexiqueGeneral    = new TRadioButton(this, GENERAL) ;
	_pLexiqueMedicament = new TRadioButton(this, MEDICAMENT) ;

	_pListeBox 	    = new ListeBoxLexique(this, IDC_PATHORLISTBOX, this, module) ;
	_pListeArray    = new NSListPathoArray ;
	_iElementChoisi = -1 ;
	_sAmmorce       = "" ;
	Create() ;
	ShowWindow(SW_HIDE) ;
}
catch (...)
{
	erreur("Exception (ChoixPathoDialog ctor)", standardError, 0);
	return;
}
}

//---------------------------------------------------------------------------
//  Function: ChoixPathoDialog::ChoixPathoDialog()
//
//  Description: Destructeur, ouvre la chemise ou le document s�lectionn�
//---------------------------------------------------------------------------
ChoixPathoDialog::~ChoixPathoDialog()
{
	//
	// Si une chemise ou un document a �t� choisi
	//
	delete _pListeBox ;
	delete _pListeArray ;
	delete _pLexiqueGeneral ;
	delete _pLexiqueMedicament ;
}

//---------------------------------------------------------------------------
//  Fonction : 	ChoixPathoDialog::SetupWindow()
//
//  Arguments :	Aucun
//
//  Description :	Initialise la boite de dialogue
//
//  Returns : 		Rien
//---------------------------------------------------------------------------
void
ChoixPathoDialog::SetupWindow()
{
	TDialog::SetupWindow() ;
	_bSetup = true ;
	if (string("_") == _sTypeLexique)
		_pLexiqueMedicament->Check() ;
	else
		_pLexiqueGeneral->Check() ;
	SetupListe() ;
}

//---------------------------------------------------------------------------
//					Recup�ration du code lexique
//---------------------------------------------------------------------------
void
ChoixPathoDialog::GetCodeLexique(char *codeRecupere)
{
  strcpy(codeRecupere, "") ;

	int index = _pListeBox->GetSelIndex() ;

	if ((index < 0) || (index >= (int) _pListeArray->size()))    return ;  NSListPathoInfo *pLPPt = (*_pListeArray)[index] ;  if (pLPPt)    strcpy(codeRecupere, pLPPt->_Donnees.code) ;
}

void
ChoixPathoDialog::LexiqueMedicament()
{
	_sTypeLexique = string("_") ;
	if (_pEdit)
		_pEdit->setTypeLexique(_sTypeLexique) ;

	_pLexiqueMedicament->Check() ;
	_pLexiqueGeneral->SetCheck(BF_UNCHECKED) ;

	SetupArray() ;
	SetupListe() ;
	Invalidate() ;

  if (_pEdit)
    _pEdit->SetFocus() ;
}

void
ChoixPathoDialog::LexiqueGeneral()
{
	_sTypeLexique = string("") ;
	if (_pEdit)
		_pEdit->setTypeLexique(_sTypeLexique) ;

	_pLexiqueGeneral->Check() ;
	_pLexiqueMedicament->SetCheck(BF_UNCHECKED) ;

	SetupArray() ;
	SetupListe() ;
	Invalidate() ;

  if (_pEdit)
    _pEdit->SetFocus() ;
}

//---------------------------------------------------------------------------
//  Description:  Fixe une nouvelle ammorce, met � jour la liste puis
//                (�ventuellement) ouvre ou ferme la fen�tre.
//                Establish a new seed, update the list and (if needed) open
//                or close the window
//
//  Returns:     	Rien
//---------------------------------------------------------------------------
int
ChoixPathoDialog::DonneAmmorce(string* pAmmorce)
{
  if (((string*) NULL == pAmmorce) || (strlen(pAmmorce->c_str()) > PATHO_LIBELLE_LEN))
    return 2 ;
  //
  // On met la liste � jour - uniquement si l'ammorce a chang�
  // Updating the list - only if seed changed
  //
	if (*pAmmorce != _sAmmorce)
	{
		_sAmmorce = *pAmmorce ;
		SetupArray() ;
	}
  else
    return 0 ;
    //
    // Si la liste est vide, on ferme la fen�tre (si elle �tait ouverte !)
    // If the list is empty, we close the window (only if it was open !)
    //
  if (_pListeArray->empty() )
	{
    if (_bOuvert)
            //cacherDialogue();
      return 2 ;
	}
  //
  // Si la liste n'est pas vide, on met � jour la fen�tre (ou on l'ouvre, ce
  // qui a pour effet de la mettre � jour)
  // ATTENTION : ne pas faire SetupListe avant que la boite de dialogue
  // 				n'ait fait son SetupWindow
  //
  // If the list is empty, we update the window (or open it, and it get
  // updated)
  // WARNING : don't call SetupListe before dialog box's SetupWindow has been
  //           called
  //
	else
	{
    if (false == _bOuvert)
			//montrerDialogue();
      return 1 ;
    else
    {
      SetupListe() ;
      return 0 ;
    }
  }

  return 2 ;
}

#define MAX_NB_WORDS 50
//---------------------------------------------------------------------------
//  Description :	Initialise l'Array de NSListPathoInfo
//
//  bFirstPass = true   : on cherche dans le lexique coch�
//  bFirstPass = false  : on cherche dans l'autre lexique
//
//---------------------------------------------------------------------------
void
ChoixPathoDialog::SetupArray(bool bFirstPass)
{
try
{
  // char chAffiche[200], dateAffiche[20];
  //
  // Vidange de l'Array
  //
 	_pListeArray->vider() ;
	//
	// Remplissage de l'Array avec les �l�ments du lexique qui correspondent
  // � l'ammorce
	//
  if (false == _pDico->Prend())
    return ;

  NSPatholog* pPathor = (NSPatholog*) 0 ;
  if (((_sTypeLexique == "_") && bFirstPass) ||
      ((_sTypeLexique != "_") && !bFirstPass))
  {
    pPathor = _pDico->getLexiMed() ;
    _pLexiqueGeneral->SetCheck(BF_UNCHECKED) ;
    _pLexiqueMedicament->Check() ;
  }
  else
  {
    pPathor = _pDico->getPatholog() ;
    _pLexiqueMedicament->SetCheck(BF_UNCHECKED) ;
    _pLexiqueGeneral->Check() ;
  }

  if ((NSPatholog*) NULL == pPathor)
    return ;

	//
	// Recherche du premier mot qui correspond � l'ammorce
	//
	char codePatho[PATHO_LIBELLE_LEN + 1] ; //tronqu�
  char codePathoVrai[PATHO_LIBELLE_LEN + 1] ;
  char donneeFiche[255] ;

  strncpy(codePathoVrai, _sAmmorce.c_str(), PATHO_LIBELLE_LEN) ;
  codePathoVrai[PATHO_LIBELLE_LEN] = '\0' ;
  if (string("") == string(codePathoVrai))
    return ;

  // Ammorce tronqu�e : codePatho = ammorce tronqu�e au premier espace
  //
  size_t i = 0 ;
  while ((i < strlen(codePathoVrai)) && (' ' != codePathoVrai[i]))
  {
    codePatho[i] = codePathoVrai[i] ;
    i++ ;
  }
  codePatho[i] = '\0' ;

  // Cas ou il n'y a que des blancs
  //
  if ('\0' == codePatho[0])
    return ;

  //  // Pour �viter les recherches tr�s longues sur une ammorce comme "A "
  // (a espace), on fait codePatho = codePathoVrai
  //
  if ((strlen(codePatho) == 1) && (strlen(codePathoVrai) > 1))
    strcpy(codePatho, codePathoVrai) ;

  // Remove special characters
  //
  while (('\0' != codePatho[0]) && (false == isProperChar(codePatho[0])))
    for (int i = 0 ; '\0' != codePatho[i] ; i++)
      codePatho[i] = codePatho[i+1] ;

  if ('\0' == codePatho[0])
    return ;

  ConvertitMajuscule(codePatho) ;
  ConvertitMajuscule(codePathoVrai) ;

  string sCode = string(codePatho) ;
  pPathor->lastError = pPathor->trouveLibelle(&sCode, keySEARCHGEQ, dbiWRITELOCK) ;

  int comp = -1 ;
  //
  // Prise du terme lexical pr�c�dent
  //
  pPathor->lastError = pPathor->precedent(dbiWRITELOCK) ;
  if (pPathor->lastError != DBIERR_NONE)
  {
    if (pPathor->lastError != DBIERR_BOF)
      erreur("Erreur � l'acc�s � l'�l�ment pr�c�dent.", standardError, pPathor->lastError) ;
  }
  else if (pPathor->lastError == DBIERR_NONE)
  {
    pPathor->lastError = pPathor->getRecord() ;
    if (pPathor->lastError != DBIERR_NONE)
      erreur("Erreur � la lecture du lexique.", standardError, pPathor->lastError) ;
    else
    {
      strncpy(donneeFiche, pPathor->_Donnees.libelle, strlen(pPathor->_Donnees.libelle)) ;
      donneeFiche[strlen(pPathor->_Donnees.libelle)] = '\0' ;
      ConvertitMajuscule(donneeFiche) ;

      comp = strncmp(donneeFiche, codePatho, strlen(codePatho)) ;
    }
  }
  //
  // S'il ne contient pas l'ammorce, on avance � nouveau
  //
  if (0 != comp)
  {
    pPathor->lastError = pPathor->suivant(dbiWRITELOCK) ;
    if (DBIERR_NONE != pPathor->lastError)
    {
      if (DBIERR_EOF != pPathor->lastError)
        erreur("Erreur � l'acc�s � l'�l�ment suivante.", standardError, pPathor->lastError) ;
    }
    else
    {
      pPathor->lastError = pPathor->getRecord() ;
      if (DBIERR_NONE != pPathor->lastError)
        erreur("Erreur � la lecture du lexique.", standardError, pPathor->lastError) ;
      else
      {
        strncpy(donneeFiche, pPathor->_Donnees.libelle, strlen(pPathor->_Donnees.libelle)) ;
        donneeFiche[strlen(pPathor->_Donnees.libelle)] = '\0' ;
        ConvertitMajuscule(donneeFiche) ;

        comp = strncmp(donneeFiche, codePatho, strlen(codePatho)) ;
      }
    }
  }

  if (0 != comp)
  {
    _pDico->Relache() ;
    if (bFirstPass)
      SetupArray(false) ;
		return ;
  }

	//
	// Si la premi�re fiche ne correspond pas � l'ammorce
	//
  if (DBIERR_NONE != pPathor->lastError)
  {
    _pDico->Relache() ;
    if (bFirstPass)
      SetupArray(false) ;
		return ;
  }

  //
  // On regarde si le mot r�cup�r� contient l'ammorce tronqu�e
  //
  strncpy(donneeFiche, pPathor->_Donnees.libelle, strlen(pPathor->_Donnees.libelle)) ;
  donneeFiche[strlen(pPathor->_Donnees.libelle)] = '\0' ;
  ConvertitMajuscule(donneeFiche) ;

  comp = strncmp(donneeFiche, codePatho, strlen(codePatho)) ;

  /*
    if (pPathor->lastError != DBIERR_NONE)
    {
   	    pDico->Relache();
		return;
    }
  */

  UINT nbFiches = 0 ;

  if (0 != comp)
  {
    _pDico->Relache() ;
    return ;
  }

  while ((DBIERR_NONE == pPathor->lastError) && (0 == comp) &&
                                             (nbFiches < MAX_NB_WORDS))
  {
    //
    // Si l'ammorce et l'ammorce tronqu�e sont diff�rentes
    //
    if (strcmp(codePatho,codePathoVrai))
    {
      string sLibelleChoix ;
      pPathor->_Donnees.donneLibelleChoix(&sLibelleChoix) ;

      char* pLibelleChoix = new char[strlen(sLibelleChoix.c_str()) + 1] ;
      strcpy(pLibelleChoix, sLibelleChoix.c_str()) ;

      ConvertitMajuscule(pLibelleChoix) ;
      string sLibelle = string(pLibelleChoix) ;

      // Si sLibelleChoix commence par codePathoVrai
      if (strncmp(pLibelleChoix, codePathoVrai, strlen(codePathoVrai)) == 0)
      {
        _pListeArray->push_back(new NSListPathoInfo(pPathor)) ;
        nbFiches++ ;
      }
      delete[] pLibelleChoix ;
    }
    //
    // Si l'ammorce et l'ammorce tronqu�e sont �gales
    //
    else
    {
      _pListeArray->push_back(new NSListPathoInfo(pPathor)) ;
      nbFiches++ ;
    }
    //
    // Prise du terme lexical suivant
    //
    pPathor->lastError = pPathor->suivant(dbiWRITELOCK) ;
    if (pPathor->lastError != DBIERR_NONE)
    {
      if (pPathor->lastError != DBIERR_EOF)
        erreur("Erreur � l'acc�s � l'�l�ment suivante.", standardError, pPathor->lastError) ;
    }
    else
    {
      pPathor->lastError = pPathor->getRecord() ;
      if (pPathor->lastError != DBIERR_NONE)
        erreur("Erreur � la lecture du lexique.", standardError, pPathor->lastError) ;
      else
      {
        strncpy(donneeFiche, pPathor->_Donnees.libelle, strlen(pPathor->_Donnees.libelle)) ;
        donneeFiche[strlen(pPathor->_Donnees.libelle)] = '\0' ;
        ConvertitMajuscule(donneeFiche) ;

        comp = strncmp(donneeFiche, codePatho, strlen(codePatho)) ;
      }
    }
  }

  _pDico->Relache() ;

  if ((0 == nbFiches) && bFirstPass)
    SetupArray(false) ;
}
catch (...)
{
	erreur("Exception (ChoixPathoDialog::SetupArray)", standardError, 0);
    return;
}
}

//---------------------------------------------------------------------------// conversion  d'une chaine en majuscule
//---------------------------------------------------------------------------
void
ChoixPathoDialog::ConvertitMajuscule(char* chaine)
{
  for (size_t i = 0 ; i < strlen(chaine); i++)
    chaine[i] = pseumaj(chaine[i]) ;

/*
    for (int i = 0 ; i< strlen(chaine); i++)
    {
        switch (chaine[i])
        {
            case '�' ://200
            case '�' :
            case '�' :
            case '�' :
			case '�' :
            case '�' :
            case '�' :
            case '�' : chaine[i] =  'E' ;
                       break;
            case '�' :
            case '�' :
            case '�' :
            case '�' :
            case '�' :            case '�' : //197
            case '�' :
            case '�' : //226
            case '�' :
            case '�' :
            case '�' :
            case '�' :
            case '�' :
            case '�' : chaine[i] =  'A' ;
                       break;
            case '�' :
            case '�' :
            case '�' :
            case '�' :
            case '�' :
            case '�' :
            case '�' : chaine[i] =  'I' ;
                       break;
            case '�' ://220
            case '�' :
            case '�' :
            case '�' : //216
            case '�' :
            case '�' :
            case '�' :
            case '�' : chaine[i] =  'U' ;
                       break;
            case '�' : //213
            case '�' :
            case '�' :
            case '�' :
            case '�' :
            case '�' : //242
            case '�' :
            case '�' :            case '�' :
            case '�' : chaine[i] = 'O' ;
                       break;
            case '�' :
            case '�' : chaine[i] = 'N' ; //209
                       break;
            case '�' :
            case '�' : chaine[i] = 'Y' ; //209
                       break;
            case '�' :
            case '�' : chaine[i] = 'C' ; //209
                       break;
            case '�' : chaine[i] = 'D' ; //209
                       break;
        }
    }
    chaine = strupr(chaine);
*/
}

//--------------------------------------------------------------------------// Touche intercept�e par la bo�te lexique
//--------------------------------------------------------------------------
void
ChoixPathoDialog::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  TWindow::EvKeyDown(key, repeatCount, flags) ;
  if (VK_INSERT == key)
  {
    _iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'�l�ment choisi
    if (_pEdit)
      _pEdit->ElementSelectionne() ;
  }

/*   NSEditDico* pEditDico =  static_cast<NSEditDico*>(pEdit);
   if(!pEditDico)
   	return;

   NSTreeWindow* pTree = pEditDico->GetpTreeAEditer();

   //fl�che basse
   if( key == VK_DOWN )
   {
      if(pTree->ModifierTexteLibre)
      {
         if(pTree->pNSTreeNodeEnCours->sTexteGlobal == "")
	         pTree->ModifierTexteLibre = false;
      	return;
      }
      //si ammorce vide alors passer au NStrreNode suivant
      if( sAmmorce == "" )
         pTree->NStreeNodeSuivant();
      //sinon r�cup�rer un �l�ment dans le lexique
      else
      	ElementSuivant();
   }
   //fl�che haute
   else if(key == VK_UP)
   {
   	//si ammorce vide alors passer au NStrreNode suivant
      if(pTree->ModifierTexteLibre)
      {
         if(pTree->pNSTreeNodeEnCours->sTexteGlobal == "")
	         pTree->ModifierTexteLibre = false;
      	return;
      }
      if(sAmmorce == "" )
         pTree->NStreeNodePrcedent();
      //sinon r�cup�rer un �l�ment dans le lexique
      else
      	ElementPrecedent();
   }
*/
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void
ChoixPathoDialog::GetLabelChoisi(string* pChaineRecuperee)
{
  if ((_iElementChoisi >= 0) && (_iElementChoisi < int(_pListeArray->size())))
    ((*_pListeArray)[_iElementChoisi])->_Donnees.donneLibelleAffiche(pChaineRecuperee) ;
  else
    *pChaineRecuperee = string("") ;
}

//--------------------------------------------------------------------------// S�lectionner dans la liste le code lexique transmis en param�tre
//--------------------------------------------------------------------------
void
ChoixPathoDialog::SelectCodeLexique(string sCodeLexique)
{
  int Element = 0 ;

  for (iterNSListPathoInfoArray i = _pListeArray->begin() ; _pListeArray->end() != i ; i++)
  {
    if (string((*i)->_Donnees.code) == sCodeLexique)
    {
      _pListeBox->SetSelIndex(Element) ;
      break ;
    }

    Element++ ;
  }
}

//---------------------------------------------------------------------------//  Fonction : 	ChoixPathoDialog::SetupListe()
//
//  Arguments :	Aucun
//
//  Description :	Initialise la boite de liste du lexique
//
//  Returns : 		Rien
//---------------------------------------------------------------------------
void
ChoixPathoDialog::SetupListe()
{
  if (_bOuvert && _bSetup)
  {
    int listeCount = _pListeBox->GetCount() ;
    if (listeCount < 0)
      erreur("Boite de liste d�fectueuse.", standardError, 0) ;
    if (listeCount > 0)
      _pListeBox->ClearList() ;
    if (false == _pListeArray->empty())
    {
      string sLibelleChoix ;
      for (iterNSListPathoInfoArray i = _pListeArray->begin() ; _pListeArray->end() != i ; i++)
      {
        (*i)->_Donnees.donneLibelleChoix(&sLibelleChoix) ;
        _pListeBox->AddString(sLibelleChoix.c_str()) ;
      }
    }
  }
}

//---------------------------------------------------------------------------
// s�lection d'un �l�ment
//---------------------------------------------------------------------------
void
ChoixPathoDialog::ElementSuivant()
{
  int nbElement = _pListeBox->GetCount() ;
  if (nbElement <= 0)
    return ;

  int Element = _pListeBox->GetSelIndex() ; //indice de l'�l�ment choisi;
  Element++ ;
  if (Element < nbElement)
    _pListeBox->SetSelIndex(Element) ;
}

//---------------------------------------------------------------------------
// s�lection d'un �l�ment
//---------------------------------------------------------------------------
void
ChoixPathoDialog::ElementPrecedent()
{
  int nbElement = _pListeBox->GetCount() ;
  if (nbElement <= 0)
    return ;

  int Element = _pListeBox->GetSelIndex() ; //indice de l'�l�ment choisi;
  Element-- ;
  if (Element >= 0)
    _pListeBox->SetSelIndex(Element) ;
}

//---------------------------------------------------------------------------// inserer un �l�ment du lexique suite � la touche Inserer
//---------------------------------------------------------------------------
void
ChoixPathoDialog::InsererElementLexique()
{
  if (false == hasSelectedElement())
    return ;

  if (_pEdit)
    _pEdit->ElementSelectionne() ;
}

//---------------------------------------------------------------------------//  Function :		ChoixPathoDialog::CmPathoDblClk(WPARAM Cmd)
//
//  Arguments :	Cmd : WPARAM retourn� par Windows
//
//  Description :	S�lectionne un �l�ment du lexique
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void
ChoixPathoDialog::CmPathoDblClk(WPARAM /* Cmd */)
{
  if (false == hasSelectedElement())
    return ;

  if (_pEdit)
    _pEdit->ElementSelectionne() ;
}

bool
ChoixPathoDialog::hasSelectedElement()
{
  if (((ListeBoxLexique*) NULL == _pListeBox) || (_pListeBox->GetCount() <= 0))
    return false ;

  _iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'�l�ment choisi

  return ((_iElementChoisi >= 0) && (_iElementChoisi < _pListeBox->GetCount())) ;
}

bool
ChoixPathoDialog::isProperChar(char cChar)
{
  if (((cChar >= 'a') && (cChar <= 'z')) ||
      ((cChar >= 'A') && (cChar <= 'Z')) ||
      ((cChar >= '0') && (cChar <= '9')))
    return true ;

  char pseumajChar = pseumaj(cChar) ;
  if (pseumajChar != cChar)
    return isProperChar(pseumajChar) ;

  return false ;
}

//---------------------------------------------------------------------------//  Function :		ChoixPathoDialog::CmSelectPatho(WPARAM Cmd)
//
//  Arguments :	Cmd : WPARAM retourn� par Windows
//
//  Description :	S�lectionne un document de la chemise en cours
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void
ChoixPathoDialog::CmSelectPatho(WPARAM Cmd)
{
  if (VK_INSERT == Cmd)
  {
    _iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'�l�ment choisi
    if (_pEdit)
      _pEdit->ElementSelectionne() ;
  }
}

//---------------------------------------------------------------------------//  Fonction : 	ChoixPathoDialog::montrerDialogue()
//
//  Arguments:	  	Aucun
//
//  Description: 	Fait apparaitre la boite de dialogue
//
//  Returns:     	Rien
//---------------------------------------------------------------------------
void ChoixPathoDialog::montrerDialogue()
{
  if (_bOuvert)
    return ;

  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsg = string("ChoixPathoDialog::montrerDialogue for DicoDlg ") + string(szThis) ;
  _pDico->pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  _bOuvert = true ;
  SetupListe() ;
  ShowWindow(SW_SHOW) ;
}

//---------------------------------------------------------------------------
//  Fonction : 	ChoixPathoDialog::cacherDialogue()
//
//  Arguments:	  	Aucun
//
//  Description: 	Fait disparaitre la boite de dialogue
//
//  Returns:     	Rien
//---------------------------------------------------------------------------
void ChoixPathoDialog::cacherDialogue()
{
  if (false == _bOuvert)
    return ;

  _bOuvert  = false ;
  _sAmmorce = string("") ;
  _pListeArray->vider() ;

  int listeCount = _pListeBox->GetCount() ;  if (listeCount < 0)
    erreur("Boite de liste d�fectueuse.", standardError, 0) ;
  if (listeCount > 0)
    _pListeBox->ClearList() ;

  ShowWindow(SW_HIDE) ;}

voidChoixPathoDialog::EvClose()
{
	cacherDialogue() ;
}

// Note : Lorsqu'on appuie sur Return lorsque la ListBox a le focus// le dialogue intercepte le Return et appelle CmOk
void
ChoixPathoDialog::CmOk()
{
  _iElementChoisi = _pListeBox->GetSelIndex() ; //indice de l'�l�ment choisi
  if (_pEdit)
    _pEdit->ElementSelectionne() ;
}

voidChoixPathoDialog::CmCancel()
{
	cacherDialogue() ;
}

void
ChoixPathoDialog::EvDestroy()
{
	//cacherDialogue();
}

bool
ChoixPathoDialog::doesBelong(THandle hWndFocus)
{
  if ((hWndFocus == HWindow) || IsChild(hWndFocus))
    return true ;

  return false ;
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSPathoArray -----------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPathoArray::NSPathoArray(const NSPathoArray& rv)
             :NSPathoInfoArray()
{
try
{
  if (rv.empty())
    return ;

  for (pthConstIter it = rv.begin() ; rv.end() != it ; it++)
    push_back(new NSPathologInfo(*(*it))) ;
}
catch (...)
{
	erreur("Exception (NSPathoInfoArray copy ctor)", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSPathoArray::~NSPathoArray()
{
  vider() ;
}

//---------------------------------------------------------------------------
//  Op�rateur d'affectation
//---------------------------------------------------------------------------
NSPathoArray&
NSPathoArray::operator=(const NSPathoArray& src)
{
try
{
  if (this == &src)
    return *this ;

	vider() ;

	if (false == src.empty())
		for (pthConstIter it = src.begin() ; src.end() != it ; it++)
			push_back(new NSPathologInfo(*(*it))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception (NSPathoInfoArray =operator)", standardError, 0) ;
	return *this ;
}
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSPathoArray::vider()
{
  if (empty())
    return ;

  for (pthIter it = begin() ; end() != it ; )
  {
    delete *it ;
    erase(it) ;
  }
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSListPathoInfo ------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSListPathoInfo::NSListPathoInfo()
//
//  Description:	Constructeur par d�faut
//---------------------------------------------------------------------------
NSListPathoInfo::NSListPathoInfo()
                :NSPathologInfo()
{
  ammorce = false ;
}

//---------------------------------------------------------------------------//  Fonction:		NSListPathoInfo::NSListPathoInfo(NSPatholog*)
//
//  Description:	Constructeur � partir d'un NSPatient
//---------------------------------------------------------------------------
NSListPathoInfo::NSListPathoInfo(NSPatholog* pPatho)
                :NSPathologInfo(pPatho)
{
  ammorce = false ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSListPathoInfo::NSListPathoInfo(const NSListPathoInfo& rv)
{
try
{
	//
	// Copie les valeurs du NSListPathoInfo d'origine
	//
	_Donnees = rv._Donnees ;
	ammorce	 = rv.ammorce ;
}
catch (...)
{
	erreur("Exception (NSListPathoInfo copy ctor)", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSListPathoInfo::~NSListPathoInfo()
{}

//---------------------------------------------------------------------------
//  Fonction:		NSListPathoInfo::operator=(NSListPathoInfo src)
//  Description:	Op�rateur d'affectation
//  Retour:			R�f�rence de l'objet cible
//---------------------------------------------------------------------------
NSListPathoInfo&
NSListPathoInfo::operator=(const NSListPathoInfo& src)
{
  if (this == &src)
    return *this ;

  _Donnees = src._Donnees ;
	ammorce	 = src.ammorce ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSListPathoInfo::operator==(NSListPathoInfo src)
//  Description:	Op�rateur de comparaison
//  Retour:			R�f�rence de l'objet cible
//---------------------------------------------------------------------------
int
NSListPathoInfo::operator==(const NSListPathoInfo& o)
{
  return (_Donnees == o._Donnees) ;
}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSListPathoArray -----------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction :		NSListPathoArray(NSListPathoArray& rv)
//  Description :	Constructeur copie
//  Retour :		Rien
//---------------------------------------------------------------------------
NSListPathoArray::NSListPathoArray(const NSListPathoArray& rv)
					  //:NSListPathoInfoArray(0,0,1)
                 :NSListPathoInfoArray()
{
try
{
  if (false == rv.empty())
    for (constIterNSListPathoInfoArray it = rv.begin() ; rv.end() != it ; it++)
      push_back(new NSListPathoInfo(*(*it))) ;
}
catch (...)
{
	erreur("Exception (NSListPathoArray copy ctor)", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Fonction:		~NSListPathoArray()
//  Description:	Destructeur
//---------------------------------------------------------------------------
NSListPathoArray::~NSListPathoArray()
{
  vider() ;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSListPathoArray::operator=(NSListPathoArray src)
//
//  Description : Op�rateur d'affectation
//---------------------------------------------------------------------------
NSListPathoArray&
NSListPathoArray::operator=(const NSListPathoArray& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (constIterNSListPathoInfoArray it = src.begin() ; src.end() != it ; it++)
			push_back(new NSListPathoInfo(*(*it))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception (NSListPathoArray =operator)", standardError, 0) ;
	return *this ;
}
}

void
NSListPathoArray::vider()
{
  if (empty())    return ;

  for (iterNSListPathoInfoArray it = begin() ; end() != it ; )
  {
    delete *it ;
    erase(it) ;
  }
}

