//---------------------------------------------------------------------------//  NS_HTML.CPP
//  RS Juin 97
//  Génération de fichiers HTML à partir d'un compte-rendu
//---------------------------------------------------------------------------

#include <stdio.h>
#include <fstream>

#include "partage\nsdivfct.h"
#include "nssavoir\nsHtmBase.h"
#include "nsbb\nsdico.h"

long NSBlocHtml::lObjectCount = 0 ;
long NSHtml::lObjectCount = 0 ;

//----------------------------------------------------------------------------
//		Classe NSHtml
//----------------------------------------------------------------------------

NSHtml::NSHtml(const HtmlTypes th, const string sTxt){
  _type   = th ;
  _sType  = typeString(th) ;
  _sTexte = sTxt ;
  _sURL   = string("") ;
  _size   = 0 ;
  _sFace  = string("") ;

  _filsArray.vider() ;

  resetMarquers() ;

  lObjectCount++ ;
}

NSHtml::NSHtml(const string sTyp, const string sTxt)
{
	_type   = typeTag(sTyp) ;
	_sType  = sTyp ;
  _sTexte = sTxt ;
  _sURL   = string("") ;
  _size   = 0 ;
  _sFace  = string("") ;

  _filsArray.vider() ;

  resetMarquers() ;

  lObjectCount++ ;
}

NSHtml::NSHtml(const HtmlTypes th, const string face, const int taille){
  _type   = th ;
  _sType  = typeString(th) ;
  _sTexte = string("") ;
  _sURL   = string("") ;
  _size   = taille ;
  _sFace  = face ;

  _filsArray.vider() ;

  resetMarquers() ;

  lObjectCount++ ;
}

NSHtml::NSHtml(const NSHtml& rv)
{
	_type      = rv._type ;
  _sType     = rv._sType ;
  _sPath     = rv._sPath ;
  _sTexte    = rv._sTexte ;
  _sURL      = rv._sURL ;
  _size      = rv._size ;
  _sFace     = rv._sFace ;
  _filsArray = rv._filsArray ;

  copyMarquers(rv) ;

  lObjectCount++ ;
}

NSHtml::~NSHtml(){
  lObjectCount-- ;
}

NSHtml&
NSHtml::operator=(const NSHtml& src)
{
	if (this == &src)
		return (*this) ;

	_type      = src._type ;
	_sType     = src._sType ;
	_sPath     = src._sPath ;
	_sTexte    = src._sTexte ;
	_sURL      = src._sURL ;
	_size      = src._size ;
	_sFace     = src._sFace ;
	_filsArray = src._filsArray ;

  copyMarquers(src) ;

	return (*this) ;
}

void
NSHtml::resetMarquers()
{
  for (int i = 0 ; i < HTML_MARQUEURS_SIZE ; i++)
  {
    _marqueurTitreDebut[i] = string("") ;
    _marqueurTitreFin[i]   = string("") ;
  }

  _sPrefix = string("") ;
  _sSuffix = string("") ;
}

void
NSHtml::copyMarquers(const NSHtml& rv)
{
  for (int i = 0 ; i < HTML_MARQUEURS_SIZE ; i++)
  {
    _marqueurTitreDebut[i] = rv._marqueurTitreDebut[i] ;
    _marqueurTitreFin[i]   = rv._marqueurTitreFin[i] ;
  }

  _sPrefix = rv._sPrefix ;
  _sSuffix = rv._sSuffix ;
}

voidNSHtml::init(string titre)
{
try
{
  // Create header
  //
  NSHtml title(tTitle) ;
  title.setText(titre) ;

  NSHtml base(tBase) ;
	base.setURL(string("file://C:/html/")) ;

  NSHtml head(tHead) ;
	head.addSon(new NSHtml(title)) ;
  head.addSon(new NSHtml(base)) ;

  _filsArray.push_back(new NSHtml(head)) ;

  // Create body
  //
  _filsArray.push_back(new NSHtml(tBody)) ;
}
catch (...)
{
	erreur("Exception (NSHtml::init)", standardError, 0) ;
}
}

// Attention GetBody ne s'utilise qu'après init sur un NSHtml(0)NSHtml*
NSHtml::GetBody()
{
  if (_filsArray.size() < 2)
    return (NSHtml*) 0 ;

	return _filsArray[1] ;
}

// insère un nouveau fils à la fin du tableau du père
void
NSHtml::insere(NSHtml *pPere, NSHtml *pFils)
{
try
{
	pPere->addSon(new NSHtml(*pFils)) ;
}
catch (...)
{
	erreur("Exception (NSHtml::insere)", standardError, 0) ;
}
}

stringNSHtml::marqueurDebut()
{
	string sm = string("") ;

	char   texteTag[255] ;

  switch (_type)
  {
    case tHtml:
      sm = string("<HTML>") ;
      break ;
    case tHead:
      sm = string("<HEAD>");
      break;
    case tBody:
      sm = string("<BODY>");
      break;
    case tTitle:
      sm = string("<TITLE>");
      break;
    case tBase:
      sm = string("<BASE HREF=\"") + _sURL + string("\">") ;
      break;
    case tXML:
      sm = string("") ;
      break ;
    case tImage:
      sm = string("<IMG SRC=\"") + _sURL + string("\">");
      break;
    case tTitre1:
      if (_marqueurTitreDebut[1] != "")
        sm = _marqueurTitreDebut[1];
      else
        sm = string("<H1>");
      break;
    case tTitre2:
      if (_marqueurTitreDebut[2] != "")
        sm = _marqueurTitreDebut[2];
      else        sm = string("<H2>");
      break;
    case tTitre3:
      if (_marqueurTitreDebut[3] != "")
        sm = _marqueurTitreDebut[3];
      else
        sm = string("<H3>");
      break;
    case tTitre4:
      if (_marqueurTitreDebut[4] != "")
        sm = _marqueurTitreDebut[4];
      else
        sm = string("<H4>");
      break;
    case tTitre5:
      if (_marqueurTitreDebut[5] != "")
        sm = _marqueurTitreDebut[5];
      else
        sm = string("<H5>");
      break;
    case tTitre6:
      if (_marqueurTitreDebut[6] != "")
        sm = _marqueurTitreDebut[6];
      else
        sm = string("<H6>");
      break;
    case tTitre7:
      if (_marqueurTitreDebut[7] != "")
        sm = _marqueurTitreDebut[7];
      else
        sm = string("<H7>");
      break;
    case tTitre8:
      if (_marqueurTitreDebut[8] != "")
        sm = _marqueurTitreDebut[8];
      else
        sm = string("");
      break;
    case tTitre9:
      if (_marqueurTitreDebut[9] != "")
        sm = _marqueurTitreDebut[9];
      else
        sm = string("");
      break;
    case tTitre10:
      if (_marqueurTitreDebut[10] != "")
        sm = _marqueurTitreDebut[10];
      else
        sm = string("");
      break;
    case tTitre11:
      if (_marqueurTitreDebut[11] != "")
        sm = _marqueurTitreDebut[11];
      else
        sm = string("");
      break;
    case tTitre12:
      if (_marqueurTitreDebut[12] != "")
        sm = _marqueurTitreDebut[12];
      else
        sm = string("");
      break;
    case tTitre13:
      if (_marqueurTitreDebut[13] != "")
        sm = _marqueurTitreDebut[13];
      else
        sm = string("");
      break;
    case tTitre14:
      if (_marqueurTitreDebut[14] != "")
        sm = _marqueurTitreDebut[14];
      else
        sm = string("");
      break;
    case tTitre15:
      if (_marqueurTitreDebut[15] != "")
        sm = _marqueurTitreDebut[15];
      else
        sm = string("");
      break;
    case tTitre16:
      if (_marqueurTitreDebut[16] != "")
        sm = _marqueurTitreDebut[16];
      else
        sm = string("");
      break;
    case tTitre17:
      if (_marqueurTitreDebut[17] != "")
        sm = _marqueurTitreDebut[17];
      else
        sm = string("");
      break;
    case tTitre18:
      if (_marqueurTitreDebut[18] != "")
        sm = _marqueurTitreDebut[18];
      else
        sm = string("");
      break;
    case tTitre19:
      if (_marqueurTitreDebut[19] != "")
        sm = _marqueurTitreDebut[19];
      else
        sm = string("");
      break;
    case tTable:
      sm = string("<TABLE>") ;
      break;
    case tTableRow:
      sm = string("<TR>") ;
      break;
    case tTableCell:
      sm = string("<TD>") ;
      break;
    case tTexte:
      if (string("") != _marqueurTitreDebut[0])
        sm = _marqueurTitreDebut[0];
      else
        sm = string("");
      break;
    case tIntitule:
    case tIntituleNoBirth:
      if (string("") != _marqueurTitreDebut[3])
        sm = _marqueurTitreDebut[3] ;
      else
        sm = string("<H3>") ;
      break;
    case tConsult:
      sm = string("<LI>") ;
      break;
    case tPolice:
      sprintf(texteTag, "<FONT FACE=\"%s\" SIZE=\"%d\">", _sFace.c_str(), _size) ;
      sm = string(texteTag) ;
      break;
    case tPara:
      sm = string("<P>");
      break;
    case tParaCentre:
      sm = string("<P ALIGN=\"CENTER\">") ;
      break;
    case tParaDroite:
      sm = string("<P ALIGN=\"RIGHT\">") ;
      break;
    case tGras:
      sm = string("<B>") ;
      break;
    case tSouligne:
      sm = string("<U>") ;
      break ;
    case tItalic:
      sm = string("<I>") ;
      break ;
    case tIndent:
      sm = string("<BLOCKQUOTE>") ;
      break ;
    case tSignature:
      sm = string("<B>") ;
      break ;
    default :
      sm = string("") ;
	}

  return sm ;
}

string
NSHtml::marqueurFin()
{
  string sm = string("") ;

  switch (_type)
  {
    case tHtml:
      sm = string("</HTML>") ;
      break ;
    case tHead:
      sm = string("</HEAD>") ;
      break ;
    case tBody:
      sm = string("</BODY>");
      break;
    case tTitle:
      sm = string("</TITLE>");
      break;
    case tBase:
      sm = string("");
      break;
    case tXML:
      sm = string("");
      break;
    case tImage:
      sm = string("");
      break;
    case tTitre1:
      if (_marqueurTitreFin[1] != "")
        sm = _marqueurTitreFin[1];
      else
        sm = string("</H1>");
      break;
    case tTitre2:
      if (_marqueurTitreFin[2] != "")
        sm = _marqueurTitreFin[2];
      else
        sm = string("</H2>");
      break;
    case tTitre3:
      if (_marqueurTitreFin[3] != "")
        sm = _marqueurTitreFin[3];
      else
        sm = string("</H3>");
      break;
    case tTitre4:
      if (_marqueurTitreFin[4] != "")
        sm = _marqueurTitreFin[4];
      else
        sm = string("</H4>");
      break;
    case tTitre5:
      if (_marqueurTitreFin[5] != "")
        sm = _marqueurTitreFin[5];
      else
        sm = string("</H5>");
      break;
    case tTitre6:
      if (_marqueurTitreFin[6] != "")
        sm = _marqueurTitreFin[6];
      else
        sm = string("</H6>");
      break;
    case tTitre7:
      if (_marqueurTitreFin[7] != "")
        sm = _marqueurTitreFin[7];
      else
        sm = string("</H7>");
      break;
    case tTitre8:
      if (_marqueurTitreFin[8] != "")
        sm = _marqueurTitreFin[8];
      else
        sm = string("");
      break;
    case tTitre9:
      if (_marqueurTitreFin[9] != "")
        sm = _marqueurTitreFin[9];
      else
        sm = string("");
      break;
    case tTitre10:
      if (_marqueurTitreFin[10] != "")
        sm = _marqueurTitreFin[10];
      else
        sm = string("");
      break;
    case tTitre11:
      if (_marqueurTitreFin[11] != "")
        sm = _marqueurTitreFin[11];
      else
        sm = string("");
      break;
    case tTitre12:
      if (_marqueurTitreFin[12] != "")
        sm = _marqueurTitreFin[12];
      else
        sm = string("");
      break;
    case tTitre13:
      if (_marqueurTitreFin[13] != "")
        sm = _marqueurTitreFin[13];
      else
        sm = string("");
      break;
    case tTitre14:
      if (_marqueurTitreFin[14] != "")
        sm = _marqueurTitreFin[14];
      else
        sm = string("");
      break;
    case tTitre15:
      if (_marqueurTitreFin[15] != "")
        sm = _marqueurTitreFin[15];
      else
        sm = string("");
      break;
    case tTitre16:
      if (_marqueurTitreFin[16] != "")
        sm = _marqueurTitreFin[16];
      else
        sm = string("");
      break;
    case tTitre17:
      if (_marqueurTitreFin[17] != "")
        sm = _marqueurTitreFin[17] ;
      else
        sm = string("") ;
      break;
    case tTitre18:
      if (_marqueurTitreFin[18] != "")
        sm = _marqueurTitreFin[18] ;
      else
        sm = string("") ;
      break;
    case tTitre19:
      if (_marqueurTitreFin[19] != "")
        sm = _marqueurTitreFin[19] ;
      else
        sm = string("") ;
      break;
    case tTable:
      sm = string("<TABLE>") ;
      break;
    case tTableRow:
      sm = string("<TR>") ;
      break;
    case tTableCell:
      sm = string("<TD>") ;
      break;
    case tTexte:
      if (string("") != _marqueurTitreFin[0])
        sm = _marqueurTitreFin[0] ;
      else
        sm = string("") ;
      break;
    case tIntitule:
    case tIntituleNoBirth:
      if (string("") != _marqueurTitreFin[3])
        sm = _marqueurTitreFin[3] ;
      else
        sm = string("</H3>") ;
      break;
    case tConsult:
      sm = string("</LI>") ;
      break;
    case tPolice:
      sm = string("</FONT>") ;
      break;
    case tPara:
      sm = string("</P>") ;
      break;
    case tParaCentre:
      sm = string("</P>") ;
      break;
    case tParaDroite:
      sm = string("</P>") ;
      break;
    case tGras:
      sm = string("</B>") ;
      break;
    case tSouligne:
      sm = string("</U>") ;
      break;
    case tItalic:
      sm = string("</I>") ;
      break;
    case tIndent:
      sm = string("</BLOCKQUOTE>") ;
      break;
    case tSignature:
      sm = string("</B>") ;
      break;
    default :
      sm = string("") ;
  }

  return sm ;
}

bool
NSHtml::ecrire(char *fichier)
{
try
{
	ofstream outFile ;
	string sOut = string("") ;

	outFile.open(fichier) ;
	if (!outFile)
		return false ;

	genere(sOut) ;

	for (size_t i = 0; i < strlen(sOut.c_str()); i++)
		outFile.put(sOut[i]) ;

	outFile.close() ;
	return true ;
}
catch (...)
{
	erreur("Exception (NSHtml::ecrire)", standardError, 0) ;
	return false ;
}
}

void
NSHtml::genereTexte(const string texte, const int tab, string& sOut)
{
  if (tab)
    sOut += space(tab) ;

  for (size_t i = 0 ; i < strlen(texte.c_str()) ; i++)
  {
    if ('\n' == texte[i])     		// remplace les \n par des BR
    {
      sOut += string("<BR>\n") ;
      if (tab)
        sOut += space(tab) ;
    }
    else
      sOut += string(1, texte[i]) ;
  }
  sOut += string(1, '\n') ;
}

voidNSHtml::genereTexte(const string sText, string& sOut)
{
  if (string("") == sText)
    return ;

  size_t iTextLen = strlen(sText.c_str()) ;

  for (size_t i = 0 ; i < iTextLen ; i++)
  {
    if ('\n' == sText[i])
    {
      sOut += string("<BR>") ;
      if (i < iTextLen - 1)
        sOut += string(1, '\n') ;
    }
    else
      sOut += string(1, sText[i]) ;
  }
  sOut += string(1, '\n') ;
}

// remplace les accolades du tag titre par des balises html//void
NSHtml::remplaceTitre(string& sTag)
{
  size_t pos = sTag.find("{") ;

  while (NPOS != pos)  {
    sTag.replace(pos, 1, "<") ;
    pos = sTag.find("{", pos+1) ;
  }

  pos = sTag.find("}") ;

  while (NPOS != pos)
  {
    sTag.replace(pos, 1, ">") ;
    pos = sTag.find("}", pos+1) ;
  }
}

// initialise marqueurTitreDebut et marqueurTitreFin//void
NSHtml::initTitres(string& params)
{
  size_t pos = params.find("T") ;
  while (NPOS != pos)
  {
    // on récupère le type titre
    //
    char cType     = params[pos+1] ;
    int  iTypeSlot = -1 ;

    // getting starting and separating chars
    //
    size_t pos1 = params.find("(", pos+2) ;
    size_t pos2 = params.find("|", pos+2) ;

    if ((NPOS == pos1) || (NPOS == pos2) || (pos1 > pos2))
    {
      erreur("Erreur syntaxique dans un bloc titre du fichier modele", standardError, 0) ;
      return ;
    }

    if (isdigit(cType))
    {
      string sTypeTitre = string(1, cType) ;
      iTypeSlot = atoi(sTypeTitre.c_str()) ;
    }

    if ('X' == cType)
      _sPrefix = string(params, pos1 + 1, pos2 - pos1 - 1) ;
    else if ((iTypeSlot >= 0) && (iTypeSlot < HTML_MARQUEURS_SIZE))
      _marqueurTitreDebut[iTypeSlot] = string(params, pos1 + 1, pos2 - pos1 - 1) ;

    // on récupère le marqueur fin
    //
    pos1 = pos2 ;
    pos2 = params.find(")", pos1 + 1) ;

    if (NPOS == pos2)
    {
      erreur("Erreur syntaxique dans un bloc titre du fichier modele", standardError, 0) ;
      return ;
    }

    if ('X' == cType)
      _sSuffix = string(params, pos1 + 1, pos2 - pos1 - 1) ;
    else if ((iTypeSlot >= 0) && (iTypeSlot < HTML_MARQUEURS_SIZE))
      _marqueurTitreFin[iTypeSlot] = string(params, pos1 + 1, pos2 - pos1 - 1) ;

    // on relance le process pour le titre suivant
    //
    pos = params.find("T", pos2+1) ;
  }
}

voidNSHtml::genere(string& sOut)
{
  string sOpeningTag = marqueurDebut() ;
  string sClosingTag = marqueurFin() ;

	genereTexte(sOpeningTag, sOut) ;

  if ((tXML == _type) || (tCQ == _type))   // les balises XML ou CQ doivent etre remplacées telles quelles
    sOut += _sTexte ;
  else // pour le texte HTML on remplace les '\n' par des <BR>
  {
    boldifier(sOpeningTag, sClosingTag) ;
    genereTexte(_sTexte, sOut) ;
  }

  // appel récursif de genere sur les fils éventuels
  if (false == _filsArray.empty())
  {
    // cas des listes, types tCS : tag consultation
    // et type tConsult : élément consultation (cf nscsvue.cpp)
    if ((tCS == _type) || (tConsult == _type))
      genereTexte(string("<UL>"), sOut) ;

    for (HtmlIter i = _filsArray.begin() ; _filsArray.end() != i ; i++)
    {
      (*i)->copyMarquers(*this) ;
      (*i)->genere(sOut) ;
    }

    if (tConsult == _type)
      genereTexte(string("</UL>"), sOut) ;
  }

  genereTexte(sClosingTag, sOut) ;
}

void
NSHtml::boldifier(string sOpeningTag, string sClosingTag)
{
  if (string("") == _sTexte)
    return ;

  size_t iFirstStart = _sTexte.find(START_BOLD) ;
  size_t iFirstEnd   = _sTexte.find(END_BOLD) ;

  if ((NPOS == iFirstStart) && (NPOS == iFirstEnd))
    return ;

  string sOpenTag  = string("<strong>") ;
  string sCloseTag = string("</strong>") ;

  // find "bold" or "b" or "strong" tags in sOpeningTag
  //
  size_t iOpenTag = sOpeningTag.find("<") ;
  while (NPOS != iOpenTag)
  {
    size_t iCloseTag = sOpeningTag.find(">", iOpenTag) ;
    if (iCloseTag > iOpenTag + 1)
    {
      string sTag = string(sOpeningTag, iOpenTag + 1, iCloseTag - iOpenTag - 1) ;
      strip(sTag) ;
      pseumaj(&sTag) ;
      if ((string("B") == sTag) || (string("BOLD") == sTag) || (string("STRONG") == sTag))
      {
        sOpenTag  = string("<i>") ;
        sCloseTag = string("</i>") ;
        break ;
      }
    }
    iOpenTag = sOpeningTag.find("<", iCloseTag) ;
  }

  while (NPOS != iFirstStart)
  {
    _sTexte.replace(iFirstStart, strlen(START_BOLD), sOpenTag) ;
    iFirstStart = _sTexte.find(START_BOLD) ;
  }

  // Re-evaluate iFirstEnd because sTexte was probably changed
  //
  iFirstEnd = _sTexte.find(END_BOLD) ;

  while (NPOS != iFirstEnd)
  {
    _sTexte.replace(iFirstEnd, strlen(END_BOLD), sCloseTag) ;
    iFirstEnd = _sTexte.find(END_BOLD) ;
  }
}

string
NSHtml::typeString(const HtmlTypes th)
{
  string sType ;

  switch (th)
  {
    case tCR :
      sType = string("CR") ;
      break ;
    case tCS :
      sType = string("CS") ;
      break ;
    case tCQ :
      sType = string("CQ") ;
      break ;
    case tRTF :
      sType = string("RTF") ;
      break ;
    case tTXT :
      sType = string("TXT") ;
      break ;
    case tXML :
      sType = string("XML") ;
      break ;
    case tTitle :
      sType = string("titre") ;
      break ;
    case tOperateur :
      sType = string("operateur") ;
      break ;
    case t2ndOperateur :
      sType = string("2ndOperateur") ;
      break ;
    case tDate :
      sType = string("date") ;
      break ;
    case tDest :
      sType = string("destinataire") ;
      break ;
    case tIntitule :
      sType = string("intitule") ;
      break ;
    case tIntituleNoBirth :
      sType = string("intituleNoBirth") ;
      break ;
    case tSignature :
      sType = string("signature") ;
      break ;

    // Patient tags
    //
    case tNomPatient :
      sType = string("nomPatient") ;
      break ;
    case tPatientNom :
      sType = string("patientNom") ;
      break ;
    case tPatientPrenom :
      sType = string("patientPrenom") ;
      break ;
    case tPatientNomNaiss :
      sType = string("patientNomNaiss") ;
      break ;
    case tPatientNaiss :
      sType = string("patientNaiss") ;
      break ;
    case tPatientIPP :
      sType = string("patientIPP") ;
      break ;
    case tPatientIEP :
      sType = string("patientIEP") ;
      break ;
    case tPatientAdresse :
      sType = string("patientAdresse") ;
      break ;
    case tPatientGender :
      sType = string("patientGender") ;
      break ;

    default :   // on doit introduire un type par défaut pour pouvoir gérer les XML
      sType = string("nautilus") ;
  }

  return sType ;
}

HtmlTypesNSHtml::typeTag(const string sTag)
{
  if      (sTag == string("CR"))
    return tCR ;
  else if (sTag == string("CS"))
    return tCS ;
  else if (sTag == string("CQ"))
    return tCQ ;
  else if (sTag == string("RTF"))
    return tRTF ;
  else if (sTag == string("TXT"))
    return tTXT ;
  // cas des balises XML (htmlXML, headXML, bodyXML)
  else if (sTag.find("XML") != NPOS)
    return tXML ;
  else if (sTag == string("titre"))
    return tTitle ;
  else if (sTag == string("operateur"))
    return tOperateur ;
  else if (sTag == string("2ndOperateur"))
    return t2ndOperateur ;
  else if (sTag == string("date"))
    return tDate ;
  else if (sTag == string("destinataire"))
    return tDest ;
  else if (sTag == string("intitule"))
    return tIntitule ;
  else if (sTag == string("intituleNoBirth"))
    return tIntituleNoBirth ;
  else if (sTag == string("signature"))
    return tSignature ;

  // Patient tags
  //
  else if (sTag == string("nomPatient"))
    return tNomPatient ;
  else if (sTag == string("patientNom"))
    return tPatientNom ;
  else if (sTag == string("patientPrenom"))
    return tPatientPrenom ;
  else if (sTag == string("patientNomNaiss"))
    return tPatientNomNaiss ;
  else if (sTag == string("patientNaiss"))
    return tPatientNaiss ;
  else if (sTag == string("patientIPP"))
    return tPatientIPP ;
  else if (sTag == string("patientIEP"))
    return tPatientIEP ;
  else if (sTag == string("patientAdresse"))
    return tPatientAdresse ;
  else if (sTag == string("patientGender"))
    return tPatientGender ;

  else // cas de tous les types "string" : on doit savoir que ce sont des tags Nautilus
    return tNautilus ; // pour ne pas les confondre avec des tags XML
}

//----------------------------------------------------------------------------//		Classe NSBlocHtml
//----------------------------------------------------------------------------

NSBlocHtml::NSBlocHtml(NSHtml* ph, int nbOccur)
{
try
{
  _occur = nbOccur ;

  if (ph)
    _pHtml = ph ;
  else
    _pHtml = (NSHtml*) 0 ;

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception (NSBlocHtml ctor)", standardError, 0) ;
}
}

NSBlocHtml::NSBlocHtml(const NSBlocHtml& rv){
try
{
  _occur = rv._occur ;

  if (rv._pHtml)
    _pHtml = new NSHtml(*(rv._pHtml)) ;
  else
    _pHtml = (NSHtml*) 0 ;

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception (NSBlocHtml copy ctor)", standardError, 0) ;
}
}

NSBlocHtml&NSBlocHtml::operator=(const NSBlocHtml& src)
{
try
{
  if (this == &src)
  	return *this ;

	_occur = src._occur ;

  if (_pHtml)
    delete _pHtml ;

  if (src._pHtml)
    _pHtml = new NSHtml(*(src._pHtml)) ;
  else
    _pHtml = (NSHtml*) 0 ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception (NSBlocHtml = oper)", standardError, 0) ;
  return *this ;
}
}

NSBlocHtml::~NSBlocHtml(){
  if (_pHtml)
    delete _pHtml ;

  lObjectCount-- ;
}

// fin du fichier nsmodhtm.cpp

