//---------------------------------------------------------------------------
#include <vcl.h>#pragma hdrstop

#include "Lettre.h"#include "nautilus\nscompub.h"
#include "nautilus\nsttx.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"

//---------------------------------------------------------------------------#pragma package(smart_init)
#pragma resource "*.dfm"

const int MAXLEN = 5000;TLettreType *LettreType;

//---------------------------------------------------------------------------__fastcall TLettreType::TLettreType(TComponent* Owner, PubliCorrespDialog* pere,
									NSRefDocument* pDoc)
	: pDlg(pere), pDocRef(pDoc), pDocTtx(0), TForm(Owner)
{
	// on autorise 5Ko pour les lettres
	RichEdit->MaxLength = MAXLEN ;
	InitFontDefaut() ;
}

//---------------------------------------------------------------------------__fastcall TLettreType::TLettreType(TComponent* Owner)
	: pDlg(0), pDocRef(0), pDocTtx(0), TForm(Owner)
{
}

__fastcall TLettreType::~TLettreType(){
	if (pDocTtx)
		delete pDocTtx ;
}

// Fonction SetText////////////////////////////////////////////////////////////////
void TLettreType::SetText(string sTexte)
{
	RichEdit->SetFocus() ;
	RichEdit->Lines->Text = AnsiString(sTexte.c_str()) ;
	RichEdit->SelStart    = strlen(sTexte.c_str()) ;
	RichEdit->SelLength   = 0 ;
}

void TLettreType::InitFontDefaut(){
  ifstream inFile ;
  string   line ;
  string   sData = "" ;
  string   sFichDat = pDlg->pContexte->PathName("FPER") + string("lettres.dat") ;
  string   sNomAttrib[3] = {"","",""} ;
  string   sValAttrib[3] = {"","",""} ;
  int      iTaille ;

  inFile.open(sFichDat.c_str());
  if (!inFile)
  {
		erreur("Erreur d'ouverture du fichier lettres.dat.", standardError, 0, pDlg->pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

  while (!inFile.eof())
  {
  	getline(inFile,line) ;
    if (line != "")
    	sData += line + "\n" ;
  }

	inFile.close() ;

	size_t i = 0 ;
	size_t j = 0 ;

	// boucle de chargement des attributs de police
	while (i < strlen(sData.c_str()))
	{
  	while ((i < strlen(sData.c_str())) && (sData[i] != ' '))
    	sNomAttrib[j] += sData[i++] ;

    while ((i < strlen(sData.c_str())) && (sData[i] == ' '))
    	i++ ;

    while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))
    	sValAttrib[j] += sData[i++] ;

    i++ ;
    j++ ;
	}

	iTaille = atoi((sValAttrib[1]).c_str()) ;

	if (iTaille == 0)
  	iTaille = 10 ;

	SetFont(sValAttrib[0], iTaille, sValAttrib[2]) ;
}

void TLettreType::SetFont(string sPolice, int taille, string sGraisse){
  RichEdit->Font->Name = AnsiString(sPolice.c_str()) ;
  RichEdit->Font->Size = taille ;

  if (sGraisse == "N")
		RichEdit->DefAttributes->Style = (RichEdit->DefAttributes->Style << 0) ;
  else if (sGraisse == "G")
		RichEdit->DefAttributes->Style = RichEdit->DefAttributes->Style << fsBold ;
  else if (sGraisse == "I")
		RichEdit->DefAttributes->Style = RichEdit->DefAttributes->Style << fsItalic ;
  else if (sGraisse == "S")
		RichEdit->DefAttributes->Style = RichEdit->DefAttributes->Style << fsUnderline ;
  else if ((sGraisse == "GI") || (sGraisse == "IG"))
		RichEdit->DefAttributes->Style = RichEdit->DefAttributes->Style << fsBold << fsItalic ;
  else if ((sGraisse == "GS") || (sGraisse == "SG"))
		RichEdit->DefAttributes->Style = RichEdit->DefAttributes->Style << fsBold << fsUnderline ;
  else if ((sGraisse == "IS") || (sGraisse == "SI"))
		RichEdit->DefAttributes->Style = RichEdit->DefAttributes->Style << fsItalic << fsUnderline ;
  else if (sGraisse == "GIS")
		RichEdit->DefAttributes->Style = RichEdit->DefAttributes->Style << fsBold << fsItalic << fsUnderline ;
  else
		RichEdit->DefAttributes->Style = RichEdit->DefAttributes->Style << 0 ;
}

// Fonction OuvreLettre////////////////////////////////////////////////////////////////
void TLettreType::OuvreLettre(bool bReadOnly)
{
try
{
	string sCodeDocTtx = "" ;

  if ((!(pDlg->CodeDocLettreSelect(sCodeDocTtx))) || (sCodeDocTtx == ""))
	{
  	string sErrMsg = pDlg->pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotOpenThisDocument") ;
    erreur(sErrMsg.c_str(), standardError, 0, pDlg->pContexte->GetMainWindow()->GetHandle()) ;
		return ;
	}

	NSDocumentInfo* pDocTtxInfo ;

  if (NULL == pDlg->pContexte->getPatient())
  	return ;

  NSPersonGraphManager* pGraphManager = pDlg->pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pGraphManager)
  	return ;

  NSDocumentInfo docInfo(sCodeDocTtx, pDlg->pContexte, pGraphManager) ;
  if (false == docInfo.InitDocumentBrut(&pDocTtxInfo))
  	return ;

  pDocTtx = new NSTtxDocument(0, bReadOnly, pDocTtxInfo, 0, pDlg->pContexte) ;

  if (pDocTtx->IsOpen())
  {
  	RichEdit->ReadOnly = bReadOnly ;
    RichEdit->PlainText = false ;
    RichEdit->Lines->LoadFromFile(pDocTtx->GetFileName().c_str()) ;
  }
}
catch (...)
{
	erreur("Exception TLettreType::OuvreLettre.", standardError, 0) ;
}
}

// Fonction OuvreLettreRTF////////////////////////////////////////////////////////////////
void TLettreType::OuvreLettreRTF(char* fichier)
{
	RichEdit->PlainText = true ;
	RichEdit->Lines->LoadFromFile(pDocTtx->GetFileName().c_str()) ;
	RichEdit->SetFocus() ;
}

stringTLettreType::InitTitre(){
	string sLang = pDlg->pContexte->getUserLanguage() ;

  string sTitreCode = string("ZLETT1") ;
	string sTitre ;
	pDlg->pContexte->getDico()->donneLibelle(sLang, &sTitreCode, &sTitre) ;

  if (string("") != sTitre)
		sTitre[0] = pseumaj(sTitre[0]) ;
  else
  	sTitre = string("Lettre") ;

  NSPersonInfo *pCorrespSelect = 0 ;
	UINT iCorrespChoisi = pDlg->_pCorrespBox->GetSelIndex() ;
  if (iCorrespChoisi < pDlg->_aCorrespArray.size())
		pCorrespSelect = pDlg->_aCorrespArray[iCorrespChoisi] ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
  	return sTitre ;

	if (string("") != pCorrespSelect->getTitre())	{		sTitre += string(" ") ;		sTitre += pCorrespSelect->getTitre() ;    sTitre += string(" ") ;		sTitre += pCorrespSelect->getNom() ;	}  else if (string("") != pCorrespSelect->getCivilite())  {		sTitre += string(" ") ;		sTitre += pCorrespSelect->getCivilite() ;	}	else  {		sTitre += string(" ") ;		sTitre += pCorrespSelect->getNom() ;
  }

	return sTitre ;}

//---------------------------------------------------------------------------void __fastcall TLettreType::OKButtonClick(TObject *Sender)
{
	string sNomFichier ;
  string sLocalis ;
  string sCodeDocRef ;
  string sCodeDocTtx ;
  string sFichier ;
  bool	 bRef = false ;

  NSSuper *pSuper = pDlg->pContexte->getSuperviseur() ;

	pSuper->afficheStatusMessage("Chargement du document texte...") ;
	if (NULL == pDocTtx)
  {
  	sFichier = "";
    pDocTtx = new NSTtxDocument(0, true, 0, 0, pDlg->pContexte) ;
    pDocTtx->Renommer(sNomFichier, sLocalis) ;
    bRef = true ;
  }
  else
  	sFichier = pDocTtx->GetFileName() ;

  // Sauvegarde du fichier sur disque
  pSuper->afficheStatusMessage("Enregistrement du document texte...") ;
  RichEdit->PlainText = false ;
  RichEdit->Lines->SaveToFile(pDocTtx->GetFileName().c_str()) ;
  pDocTtx->SetDirty(false) ;

	// Référencement du document Ttx
	pSuper->afficheStatusMessage("Référencement du document texte...");

	string sMasterMeta = pDlg->_pPubli->_pDocMaitre->_pDocInfo->getCodeDocMeta() ;

	if (bRef &&
       (false == pDocTtx->Referencer(string("ZTRTF"), InitTitre(), sNomFichier,
                                     sLocalis, false, true, string(""),
                                     NSRootLink::personDocument,
                                     (NSPersonGraphManager*) 0,
                                     string("_User_"), string(""), string(""),
                                     pDlg->CodeCorrespSelect(), string(""),
                                     sMasterMeta, NSRootLink::letterOfDocument)))
                                   //  "ZLETT1", sMasterMeta, NSRootLink::letterOfDocument)))
	{    NsDeleteTemporaryFile(pSuper, pDocTtx->GetFileName()) ;    pDocTtx->SetDirty(true) ;    // on remet le document dans l'état initial
    pDocTtx->SetNomFichier(sFichier) ;
    return ;
  }

	pSuper->afficheStatusMessage("Fermeture du document texte...") ;
	pDlg->CheckLettre() ;
	Close() ;
}

//---------------------------------------------------------------------------void __fastcall TLettreType::AnnulButtonClick(TObject *Sender)
{
	pDlg->CheckLettre() ;
	Close() ;
}

//---------------------------------------------------------------------------void __fastcall TLettreType::SelectButtonClick(TObject *Sender)
{
	RichEdit->SelStart = 0 ;
	RichEdit->SelLength = MAXLEN ;
	RichEdit->SelectAll() ;
	RichEdit->Invalidate() ;
	// MouseDown(mbRight,0,0,0);
}

//---------------------------------------------------------------------------void __fastcall TLettreType::CopyButtonClick(TObject *Sender)
{
    RichEdit->CopyToClipboard();
}

//---------------------------------------------------------------------------
void __fastcall TLettreType::CutButtonClick(TObject *Sender)
{
    RichEdit->CutToClipboard();
    RichEdit->Modified = true;      // ne marche pas (???)
    RichEdit->Invalidate();
}

//---------------------------------------------------------------------------void __fastcall TLettreType::PasteButtonClick(TObject *Sender)
{
    RichEdit->PasteFromClipboard();
    RichEdit->Modified = true;      // ne marche pas (???)
    RichEdit->Invalidate();
    RichEdit->SetFocus();
}

//---------------------------------------------------------------------------void __fastcall TLettreType::RichEditMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    // Pour installer un menu click-droit
    if (Button == mbRight)
    {
        // RichEdit->GetCaretPos(); est inaccessible (arrrgh...)
        Windows::TPoint point(0,0);
        char buffer[MAXLEN];
        OWL::TPopupMenu *menu = new OWL::TPopupMenu();

        RichEdit->GetSelTextBuf(buffer, MAXLEN);
        if (!strcmp(buffer, ""))
        {
            // cas pas de texte sélectionné
            menu->AppendMenu(MF_GRAYED, 0,  "Couper");
            menu->AppendMenu(MF_GRAYED, 0,  "Copier");
        }
        else
        {
            // cas texte sélectionné            menu->AppendMenu(MF_STRING, CM_EDITCUT,     "Couper");
            menu->AppendMenu(MF_STRING, CM_EDITCOPY,    "Copier");
        }

        menu->AppendMenu(MF_STRING, CM_EDITPASTE,   "Coller");

        point = RichEdit->ClientToScreen(point);
  	    menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, ClientHandle);
  	    delete menu;
    }
}

//---------------------------------------------------------------------------void __fastcall TLettreType::FormCreate(TObject *Sender)
{
    // ATTENTION cette méthode est lancée sur TForm(Owner)
    // AVANT le constructeur de TLettreType
}

//---------------------------------------------------------------------------
