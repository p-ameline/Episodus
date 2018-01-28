// -----------------------------------------------------------------------------
// nsHealthTeamMemberInterface.cpp
// -----------------------------------------------------------------------------
// Interface Visuelle de Gestion d'un Membre de l'Equipe de Santé
// -----------------------------------------------------------------------------
// $Revision: 1.9 $
// $Author: pameline $
// $Date: 2014/05/01 17:02:21 $
// -----------------------------------------------------------------------------
// FLP - juin 2004
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------


#include <owl\window.h>
#include <owl\dialog.h>
#include <owl\static.h>


#include "nssavoir\nsHealthTeamMemberInterface.h"
#include "nssavoir\nsHealthTeamInterface.h"
#include "nssavoir\GraphicHealthTeam.h"
#include "nssavoir\healthteam.rh"
#include "nssavoir\nssavoir.h"
#include "pilot\NautilusPilot.hpp"
#include "nsbb\tagNames.h"
#include "nsutil\md5.h"


bool
mandateSortByTypeSup(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getType() > pM2->getType()) ;
}


bool
mandateSortByTypeInf(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getType() < pM2->getType()) ;
}


bool
mandateSortByProfessionSup(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getProfessionLabel() > pM2->getProfessionLabel()) ;
}


bool
mandateSortByProfessionInf(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getProfessionLabel() < pM2->getProfessionLabel()) ;
}


bool
mandateSortBySpecialitySup(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getSpecialityLabel() > pM2->getSpecialityLabel()) ;
}


bool
mandateSortBySpecialityInf(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getSpecialityLabel() < pM2->getSpecialityLabel()) ;
}


bool
mandateSortByDistanceSup(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getDistance() > pM2->getDistance()) ;
}


bool
mandateSortByDistanceInf(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getDistance() < pM2->getDistance()) ;
}


bool
mandateSortByAngleSup(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getAngle() > pM2->getAngle()) ;
}


bool
mandateSortByAngleInf(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getAngle() < pM2->getAngle()) ;
}


bool
mandateSortByBeginDateSup(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getBeginDate() > pM2->getBeginDate()) ;
}


bool
mandateSortByBeginDateInf(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getBeginDate() < pM2->getBeginDate()) ;
}


bool
mandateSortByEndDateSup(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getEndDate() > pM2->getEndDate()) ;
}


bool
mandateSortByEndDateInf(HTIMandate *pM1, HTIMandate *pM2)
{
	if (!pM1 || !pM2)
		return false ;
	return (pM1->getEndDate() < pM2->getEndDate()) ;
}

string
HTIMandate::getProfessionLabel()
{
	string	sLanguage 			= pContexte->getUtilisateur()->donneLang() ;
	string	sLexiCode				= getProfession() ;
	string	sLexiCodeLabel	= "" ;

  if (sLexiCode != "")
		pContexte->getDico()->donneLibelle(sLanguage, &sLexiCode, &sLexiCodeLabel) ;
  return sLexiCodeLabel ;
}

string
HTIMandate::getSpecialityLabel()
{
	string	sLanguage 			= pContexte->getUtilisateur()->donneLang() ;
	string	sLexiCode				= getSpeciality() ;
	string	sLexiCodeLabel	= "" ;

  if (sLexiCode != "")
		pContexte->getDico()->donneLibelle(sLanguage, &sLexiCode, &sLexiCodeLabel) ;
  return sLexiCodeLabel ;
}

// -----------------------------------------------------------------------------
// class HTIMandateArray
// -----------------------------------------------------------------------------
// liste d'objets contenant un pointeur sur les mandats utlisée pour l'interface
// de gestion d'un member de l'Equipe de Santé.
// --
// list of objects that have a pointer on mandate, used in the HealthTeam member
// interface.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// constructeur copie
// --
// copy constructor
// -----------------------------------------------------------------------------
HTIMandateArray::HTIMandateArray(const HTIMandateArray& src)
	              :HTIMandateVector()
{
	if (false == src.empty())
  	for (HTIMandateConstIter htiMandateIter = src.begin() ; src.end() != htiMandateIter ; htiMandateIter++)
    	push_back(new HTIMandate(**htiMandateIter)) ;
}


// -----------------------------------------------------------------------------
// constructeur à partir d'une liste de mandats
// --
// constructor from a list of mandates
// -----------------------------------------------------------------------------
HTIMandateArray::HTIMandateArray(NSContexte *pCtx, NSHTMMandateArray *pMandates)
                :HTIMandateVector()
{
	if (false == pMandates->empty())
  	for (NSHTMMandateIter mandateIter = pMandates->begin() ; pMandates->end() != mandateIter ; mandateIter++)
    	push_back(new HTIMandate(pCtx, *mandateIter)) ;
}

HTIMandateArray&
HTIMandateArray::operator=(const HTIMandateArray& src)
{
  if (&src == this)
  	return *this ;

	vider() ;

	if (false == src.empty())
  	for (HTIMandateConstIter htiMandateIter = src.begin() ; src.end() != htiMandateIter ; htiMandateIter++)
    	push_back(new HTIMandate(**htiMandateIter)) ;

  return (*this) ;
}

void
HTIMandateArray::reinit(NSContexte *pCtx, const NSHTMMandateArray *pMandates)
{
	vider() ;

  if ((NSHTMMandateArray*) NULL == pMandates)
    return ;

  if (false == pMandates->empty())
  	for (NSHTMMandateConstIter mandateIter = pMandates->begin() ; pMandates->end() != mandateIter ; mandateIter++)
    	push_back(new HTIMandate(pCtx, *mandateIter)) ;
}

// -----------------------------------------------------------------------------
// vide la liste en supprimant les objets contenus par les pointeurs listés.
// --
// erase all datas pointed by objects listed.
// -----------------------------------------------------------------------------
void
HTIMandateArray::vider()
{
	if (empty())
    return ;

  for (HTIMandateIter htiMandateIter = begin() ; end() != htiMandateIter ; )
  {
    delete (*htiMandateIter) ;
    erase(htiMandateIter) ;
  }
}

// -----------------------------------------------------------------------------
// class HTComboBox
// -----------------------------------------------------------------------------
// combo box permettant d'ajouter une distance ou un angle affichés par libellés
// --
// combo box that permits to add distance or angle and show correspondant label.
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(HTComboBox, TComboBox)
  EV_WM_KEYUP,
  EV_WM_KEYDOWN,
  EV_WM_GETDLGCODE,
  EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

HTComboBox::HTComboBox(TWindow *parent, int resId, NSContexte *pCtx, HTCOMBOTYPE type, VecteurString *pVecteurString)
           :TComboBox(parent, resId), NSRoot(pCtx)
{
  _sCode      = string("") ;
  _bWinStd    = false ;
  _HTType     = type ;
  _pDlgParent = parent ;

  if (pVecteurString && (false == pVecteurString->empty()))
  	for (EquiItemIter iter = pVecteurString->begin() ; pVecteurString->end() != iter ; iter++)
    	_aIntegerCodes.AddString(**iter) ;
}

HTComboBox::~HTComboBox()
{
  _aIntegerCodes.vider() ;
}

void
HTComboBox::SetupWindow()
{
  TComboBox::SetupWindow() ;

  // ---------------------------------------------------------------------------
  // la liste inclut dans pIntegerCodes correpond aux codes entiers des libellés
  // à mettre dans le combobox
  // ---------------------------------------------------------------------------
  // il faut donc faire des addstring à partir des libellés et non des codes
  // lexique, en fonction du type de la combo
  // ---------------------------------------------------------------------------

  if      (distanceType == _HTType)
  	AddDistances() ;
  else if (angleType == _HTType)
  	AddAngles() ;
}

string
HTComboBox::getSelCode()
{
  // récupétion de l'index de la sélection au sein du combobox
  int index = GetSelIndex() ;

  _sCode = string("") ;

  // récupération du code lexique
  EquiItemIter  iter ;
  int           iIter ;
  if (false == _aIntegerCodes.empty())
  {
    for (iter = _aIntegerCodes.begin(), iIter = 0 ; (_aIntegerCodes.end() != iter) && (index != iIter) ; iter++, iIter++)
    	;
    if ((_aIntegerCodes.end() != iter) && (iIter == index))
    	_sCode = **iter ;
  }

  return _sCode ;
}

int
HTComboBox::getIntSelCode()
{
  string sCd = getSelCode() ;
  return atoi(sCd.c_str()) ;
}

void
HTComboBox::setCode(string sValue)
{
  EquiItemIter  iter ;
  int           iIter ;

  if (false == _aIntegerCodes.empty())
  {
    for (iter = _aIntegerCodes.begin(), iIter = 0 ; (_aIntegerCodes.end() != iter) && ((**iter) != sValue) ; iter++, iIter++)
    	;
    if ((_aIntegerCodes.end() != iter) && ((**iter) == sValue))
    {
      SetSelIndex(iIter) ;
      _sCode = sValue ;
      return ;
    }
  }

  SetSelIndex(-1) ;
  _sCode = string("") ;
}

void
HTComboBox::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  if (_bWinStd)
  {
    TComboBox::EvKeyUp(key, repeatcount, flags) ;
    return ;
  }

  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(_pDlgParent, NSUtilDialog) ;
  if ((NSUtilDialog*) NULL == pNSUtilDialog)
  	return ;

  switch(key)
  {
    //return
    case VK_RETURN  :
    case VK_DOWN    :	//demander à la boîte de dialogue mère d'activer le controle suivant pControle sinon le premier
                      pNSUtilDialog->ActiveControlSuivant(this) ;
    default         :	TComboBox::EvKeyUp(key, repeatcount, flags) ;
  }
}

void
HTComboBox::AddIntegerCode(int code)
{
  char temp[10] ;
  itoa(code, temp, 10) ;
  _aIntegerCodes.AddString(string(temp)) ;
}


void
HTComboBox::AddDistances()
{
  if (false == _aIntegerCodes.empty())
  {
    for (EquiItemIter iter = _aIntegerCodes.begin() ; _aIntegerCodes.end() != iter ; iter++)
    {
      string sLabel ;
      string sIntCode = (*iter)->c_str() ;
      int code = atoi(sIntCode.c_str()) ;

      switch (code)
      {
        case 0 :  sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "distance0") ;  break ;
        case 1 :  sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "distance1") ;  break ;
        case 2 :  sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "distance2") ;  break ;
        default : erreur("Distance non définie dans HTComboBox.", standardError, 0) ;               return ;
      }
      AddString(sLabel.c_str()) ;
    }
  }
}

void
HTComboBox::AddAngles()
{
  if (false == _aIntegerCodes.empty())
  {
    for (EquiItemIter iter = _aIntegerCodes.begin() ; _aIntegerCodes.end() != iter ; iter++)
    {
      string sLabel ;
      string sIntCode = (*iter)->c_str() ;
      int code = atoi(sIntCode.c_str()) ;

      switch (code)
      {
        case  0 : sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "angle0") ;   break ;
        case  3 : sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "angle3") ;   break ;
        case  9 : sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "angle9") ;   break ;
        case 15 : sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "angle15") ;  break ;
        case 17 : sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "angle17") ;  break ;
        case 21 : sLabel = pContexte->getSuperviseur()->getText("rosaceManagement", "angle21") ;  break ;
        default : erreur("Angle non défini dans HTComboBox.", standardError, 0) ;                 return ;
      }
      AddString(sLabel.c_str()) ;
    }
  }
}

void
HTComboBox::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  if (_bWinStd)
  {
    TComboBox::EvKeyUp(key, repeatCount, flags) ;
    return ;
  }

  NSUtilDialog* pNSUtilDialog = TYPESAFE_DOWNCAST(_pDlgParent, NSUtilDialog) ;
  if (!pNSUtilDialog)
  	return ;

  switch(key)
  {
    case VK_TAB     : pNSUtilDialog->ActiveControlSuivant(this) ; break ;
    case VK_RETURN  :
    case VK_DOWN    : pNSUtilDialog->ActiveControlSuivant(this) ; break ;
    default         : TComboBox::EvKeyUp(key, repeatCount, flags) ;
  }
}

//fonction permettant à la touche ENTREE d'avoir le même effet que TAB
uint
HTComboBox::EvGetDlgCode(MSG far * /* msg */)
{
  uint retVal = (uint)DefaultProcessing() ;
  if (false == _bWinStd)
  	retVal |= DLGC_WANTALLKEYS ;
  return retVal ;
}

bool
HTComboBox::isVisible()
{
	return IsWindowVisible() ;
}

// -----------------------------------------------------------------------------
// class HealthTeamMemberInterface
// -----------------------------------------------------------------------------
// interface visuelle permettant de créer/modifier un membre de l'Equipe de
// Santé.
// --
// visual interface that permits to create/modify an HealthTeam member.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// table de réponses.
// --
// response table.
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(HealthTeamMemberInterface, TDialog)
	EV_WM_SETFOCUS,
	EV_WM_LBUTTONDBLCLK,
	EV_WM_RBUTTONDOWN,
	EV_LVN_GETDISPINFO(IDC_MANDATELIST, DispInfoListe),
	EV_LVN_COLUMNCLICK(IDC_MANDATELIST,	LVNColumnclick),
	EV_WM_SIZE,
	EV_WM_SETFOCUS,
	EV_COMMAND(IDOK,              CmOk),
	EV_COMMAND(IDCANCEL,          CmCancel),
	EV_COMMAND(IDC_ADDMANDATE,    CmNewMandate),
	EV_COMMAND(IDC_REMOVEMEMBER,  CmDismissMember),
	EV_COMMAND(CM_NEWMANDATE,     CmNewMandate),
	EV_COMMAND(IDC_DELMANDATE,    CmCloseMandate),
	EV_COMMAND(CM_DELMANDATE,     CmCloseMandate),
	EV_COMMAND(IDC_MODMANDATE,    CmModMandate),
	EV_COMMAND(CM_MODMANDATE,     CmModMandate),
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// constructeur
// --
// constructor
// -----------------------------------------------------------------------------
HealthTeamMemberInterface::HealthTeamMemberInterface(HealthTeamInterface *parent, NSContexte *pCtx, NSHealthTeamMember *pM)
	                        :NSUtilDialog((TWindow *)parent, pCtx, "IDD_HEALTHTEAMMEMBER", pNSResModule)
{
 	pTManager					= parent->pHTManager ;
	pMember			 			= pM ;

	pInformation    	= new TStatic(this,	IDCE_ID) ;
	pListMandates 		= new NSListMandatesWindow(this, pContexte, IDC_MANDATELIST) ;

	pMandates 				= new HTIMandateArray(pContexte, pMember->pMandates) ;

	bNaturallySorted	= false ;
	iSortedColumn 	  = 0 ;
}


HealthTeamMemberInterface::HealthTeamMemberInterface(RosaceDrawer *parent, NSContexte *pCtx, NSHealthTeamMember *pM)
	                        :NSUtilDialog((TWindow *)parent, pCtx, "IDD_HEALTHTEAMMEMBER", pNSResModule)
{
	pTManager				  = parent->getManager()->getManager() ;
 	// *pTManager				= *(parent->getManager()) ;
	pMember			 			= pM ;

	pInformation    	= new TStatic(this,	IDCE_ID) ;
	pListMandates 		= new NSListMandatesWindow(this, pContexte, IDC_MANDATELIST) ;

	pMandates 				= new HTIMandateArray(pContexte, pMember->pMandates) ;

	bNaturallySorted	= false ;
	iSortedColumn 	  = 0 ;
}


// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
HealthTeamMemberInterface::~HealthTeamMemberInterface()
{
	delete (pMandates) ;
	delete (pInformation) ;
	delete (pListMandates) ;
}


/*
void
HealthTeamMemberInterface::SetStringName(string name)
{
  string toAff = "Nom : " + name;
  pInformation->SetText(toAff.c_str());
}
*/


// -----------------------------------------------------------------------------
// setupWindow
// -----------------------------------------------------------------------------
void
HealthTeamMemberInterface::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
	SetupColumns() ;

	string  sPersid = pMember->getID() ;
	NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPersid, pidsCorresp) ;
	sMemberName	= pPersonInfo->getCivilite() ;
	char	szBuffer[128] ;
	sprintf(szBuffer, "Nom : %s", sMemberName.c_str()) ;
	pInformation->SetText(szBuffer) ;

	if (pMember)
		displayList() ;
}


void
HealthTeamMemberInterface::SetupColumns()
{
	string work       = pContexte->getSuperviseur()->getText("HealthTeamWindows", "work") ;
  string type       = pContexte->getSuperviseur()->getText("HealthTeamWindows", "type") ;
  string speciality = pContexte->getSuperviseur()->getText("HealthTeamWindows", "speciality") ;
  string distance   = pContexte->getSuperviseur()->getText("HealthTeamWindows", "distance") ;
  string angle      = pContexte->getSuperviseur()->getText("HealthTeamWindows", "angle") ;
  string begindate  = pContexte->getSuperviseur()->getText("HealthTeamWindows", "begindate") ;
  string enddate    = pContexte->getSuperviseur()->getText("HealthTeamWindows", "enddate") ;

	pListMandates->InsertColumn(0,  TListWindColumn((char *)type.c_str(),        100, TListWindColumn::Right, 0)) ;
	pListMandates->InsertColumn(1,  TListWindColumn((char *)work.c_str(),        100, TListWindColumn::Right, 1)) ;
	pListMandates->InsertColumn(2,  TListWindColumn((char *)speciality.c_str(),  100, TListWindColumn::Right, 2)) ;
	pListMandates->InsertColumn(3,  TListWindColumn((char *)distance.c_str(),     55, TListWindColumn::Right, 3)) ;
	pListMandates->InsertColumn(4,  TListWindColumn((char *)angle.c_str(),       120, TListWindColumn::Right, 4)) ;
	pListMandates->InsertColumn(5,  TListWindColumn((char *)begindate.c_str(),   120, TListWindColumn::Right, 5)) ;
	pListMandates->InsertColumn(6,  TListWindColumn((char *)enddate.c_str(),     120, TListWindColumn::Right, 6)) ;
}


void
HealthTeamMemberInterface::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  const int       BufLen = 255 ;
  static char     buffer[BufLen] ;
  TListWindItem   &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  NVLdVTemps      tpsBegin, tpsEnd ;
  string          sBeginDate, sEndDate ;
  buffer[0] = '\0' ;

  string sLanguage = pContexte->getUtilisateur()->donneLang() ;

  int index = dispInfoItem.GetIndex() ;

  // Affiche les informations en fonction de la colonne
  switch (dispInfoItem.GetSubItem())
	{
		case 1	:	// métier - profession
							sprintf(buffer, "%s", ((*pMandates)[index])->getProfessionLabel().c_str()) ;
							dispInfoItem.SetText(buffer) ;
							break ;

		case 2	:	// spécialité - speciality
							sprintf(buffer, "%s", ((*pMandates)[index])->getSpecialityLabel().c_str()) ;
							dispInfoItem.SetText(buffer) ;
							break ;

		case 3	: // distance - distance
							sprintf(buffer, "%d", ((*pMandates)[index])->getDistance()) ;
							dispInfoItem.SetText(buffer) ;
							break ;

		case 4	: // angle - angle
							sprintf(buffer, "%s", (((*pMandates)[index])->getStringAngle()).c_str()) ;
							dispInfoItem.SetText(buffer) ;
							break ;

		case 5	: // date de début de mandat - mandate beginning date
    					sBeginDate = ((*pMandates)[index])->getBeginDate() ;
              tpsBegin.initFromDateHeure(sBeginDate) ;
							sprintf(buffer, "%s", (tpsBegin.donneFormattedDateHeure(pContexte, sLanguage)).c_str()) ;
							dispInfoItem.SetText(buffer) ;
							break ;

		case 6	:	// date de fin de durée mandat - mandate ending date
              sEndDate = ((*pMandates)[index])->getEndDate() ;
              if ("99990000000000" == sEndDate)
              {
              	sEndDate = pContexte->getSuperviseur()->getText("HealthTeamWindows", "notimelimit") ;
              	sprintf(buffer, "%s", (sEndDate).c_str()) ;
              }
              else
              {
              	tpsEnd.initFromDateHeure(sEndDate) ;
              	sprintf(buffer, "%s", (tpsEnd.donneFormattedDateHeure(pContexte, sLanguage)).c_str()) ;
              }
							dispInfoItem.SetText(buffer) ;
							break ;

		default	: break ;
	}
}


void
HealthTeamMemberInterface::LVNColumnclick(TLwNotify& lwn)
{
  sortByColumn(lwn.iSubItem) ;
}


void
HealthTeamMemberInterface::sortByColumn(int iColumn)
{
	// fabTODO
	// voir si il ne faudrait pas créer une deuxième liste de mandats pour
	// l'affichage - problème de triage
  // NB : normalement il n'y a pas de problèmes même aux triages puisque les
	// mandats ont un pointeur sur la patpatho

  if (iSortedColumn == iColumn)
  {
    if (bNaturallySorted)
      bNaturallySorted = false ;
    else
      bNaturallySorted = true ;
  }
  else
  {
    iSortedColumn = iColumn ;
    bNaturallySorted = true ;
  }

	if (pMandates->empty())
		return ;

  switch (iColumn)
  {
    case 0  : // type
							if (bNaturallySorted)
                sort(pMandates->begin(), pMandates->end(), mandateSortByTypeSup) ;
              else
                sort(pMandates->begin(), pMandates->end(), mandateSortByTypeInf) ;
              break ;

		case 1	: // métier - profession
							if (bNaturallySorted)
								sort(pMandates->begin(), pMandates->end(), mandateSortByProfessionSup) ;
							else
								sort(pMandates->begin(), pMandates->end(), mandateSortByProfessionInf) ;
              break ;

		case 2	: // spécialité - speciality
							if (bNaturallySorted)
								sort(pMandates->begin(), pMandates->end(), mandateSortBySpecialitySup) ;
							else
								sort(pMandates->begin(), pMandates->end(), mandateSortBySpecialityInf) ;
              break ;

    case 3  : // distance
							if (bNaturallySorted)
                sort(pMandates->begin(), pMandates->end(), mandateSortByDistanceSup) ;
              else
                sort(pMandates->begin(), pMandates->end(), mandateSortByDistanceInf) ;
              break ;

    case 4  : // angle
							if (bNaturallySorted)
                sort(pMandates->begin(), pMandates->end(), mandateSortByAngleSup) ;
              else
                sort(pMandates->begin(), pMandates->end(), mandateSortByAngleInf) ;
              break ;

    case 5  : // date de début de mandat
							if (bNaturallySorted)
                sort(pMandates->begin(), pMandates->end(), mandateSortByBeginDateSup) ;
              else
                sort(pMandates->begin(), pMandates->end(), mandateSortByBeginDateInf) ;
              break ;

    case 6  : // date de fin de mandat
							if (bNaturallySorted)
                sort(pMandates->begin(), pMandates->end(), mandateSortByEndDateSup) ;
              else
                sort(pMandates->begin(), pMandates->end(), mandateSortByEndDateInf) ;
              break ;

    default	: break ;
	}
	displayList() ;
}


void
HealthTeamMemberInterface::displayList()
{
	pListMandates->DeleteAllItems() ;

	if (pMandates->empty())
    	return ;

	// Attention : insert insère au dessus ; il faut inscrire les derniers en premier
	for (HTIMandateRIter rIter = pMandates->rbegin() ; rIter != pMandates->rend() ; rIter++)
	{
		char	buffer[150] ;
		switch ((*rIter)->getType())
		{
			case NSHealthTeamMandate::user	:	sprintf(buffer, "Mandat d'accès") ; 					break ;
			case NSHealthTeamMandate::root	:	sprintf(buffer, "Mandat d'Administration") ;	break ;
			default													: break ;
		}
		TListWindItem		item(buffer, 0) ;
		pListMandates->InsertItem(item) ;
	}
	return ;
}


// -----------------------------------------------------------------------------
// CmNewMandate
// -----------------------------------------------------------------------------
void
HealthTeamMemberInterface::CmNewMandate()
{
	NSHealthTeamMandate	*pMandate = new NSHealthTeamMandate() ;

	HealthTeamMandateInterface	*pNewMandateInterface = new HealthTeamMandateInterface(this, pContexte, pMandate) ;
	if (pNewMandateInterface->Execute() == IDOK)
  {
    pTManager->addMandate(pMember, pMandate) ;
    if (pMandate->isValid())
    {
    	pMandates->reinit(pContexte, pMember->pMandates) ;
    }
  }

	delete pNewMandateInterface ;
	// delete pMandate ;

	displayList() ;
}


void
HealthTeamMemberInterface::CmDismissMember()
{
	pTManager->closeAllMandates(pContexte, pMember) ;
//  pHTManager->delMember(pMember) ;
//  TDialog::CmOk() ;
}


void
HealthTeamMemberInterface::CmCloseMandate()
{
	int	index = pListMandates->IndexItemSelect() ;
  if (index != -1)
	{
  	pTManager->closeMandate(pContexte, pMember, ((*pMandates)[index])->getMandate()) ;
  }
  displayList() ;
}


/*
// -----------------------------------------------------------------------------
// CmDelMandate
// -----------------------------------------------------------------------------
void
HealthTeamMemberInterface::CmDelMandate()
{
	int	index = pListMandates->IndexItemSelect() ;
  if (index != -1)
    pHTManager->delMandate(pMember, ((*pMandates)[index])->getMandate()) ;
  displayList() ;
}
*/


// -----------------------------------------------------------------------------
// CmModMandate
// -----------------------------------------------------------------------------
void
HealthTeamMemberInterface::CmModMandate()
{
	int	index = pListMandates->IndexItemSelect() ;
  if (index != -1)
  {
    NSHealthTeamMandate	*pMandate = ((*pMandates)[index])->getMandate() ;

    HealthTeamMandateInterface	*pNewMandateInterface = new HealthTeamMandateInterface(this, pContexte, pMandate) ;
    if (pNewMandateInterface->Execute() == IDOK)
      pTManager->modMandate(pContexte, pMember, pMandate) ;
    delete pNewMandateInterface ;
    displayList() ;
  }
}


string
HealthTeamMemberInterface::getLabelFromLexiCode(string sLexiCode)
{
	string	sLanguage 			= pContexte->getUtilisateur()->donneLang() ;
	string	sLexiCodeLabel	= "" ;
  pContexte->getDico()->donneLibelle(sLanguage, &sLexiCode, &sLexiCodeLabel) ;
	return sLexiCodeLabel ;}


/*
string
HealthTeamMemberInterface::getName()
{
  char	temp[256] ;
  pInformation->GetText(temp, 255) ;
  return string(temp) ;
}
*/


// -----------------------------------------------------------------------------
// CmOk
// -----------------------------------------------------------------------------
void
HealthTeamMemberInterface::CmOk()
{
	// fabTODO
	NSUtilDialog::CmOk() ;
}


// -----------------------------------------------------------------------------
// CmCancel
// -----------------------------------------------------------------------------
void
HealthTeamMemberInterface::CmCancel()
{
  NSUtilDialog::CmCancel() ;
}



// -----------------------------------------------------------------------------
// class HealthTeamMandateInterface
// -----------------------------------------------------------------------------
// --
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// table de réponses
// --
// response table
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(HealthTeamMandateInterface, TDialog)
	EV_COMMAND(IDOK,                      CmOk),
  EV_COMMAND(IDCANCEL,                  CmCancel),
  EV_EN_KILLFOCUS(IDCE_DURATION,        ActualiseEnd),
  EV_CBN_KILLFOCUS(IDCCB_DURATIONUNIT,  ActualiseEnd),
  EV_CBN_KILLFOCUS(IDCCB_ANGLE,         GetProfession),
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// constructeur
// --
// constructor
// -----------------------------------------------------------------------------

HealthTeamMandateInterface::HealthTeamMandateInterface(HealthTeamMemberInterface *pParentWindow, NSContexte *pCtx, NSHealthTeamMandate *pM)
                           :NSUtilDialog(pParentWindow, pCtx, "IDD_HEALTHTEAMMANDATE", pNSResModule)
{
	initObjects() ;

	pMandate           = pM ;
	pHTMemberInterface = pParentWindow ;
  _special_init      = false ;
}

HealthTeamMandateInterface::HealthTeamMandateInterface(TWindow* parent, NSContexte *pCtx, string nam, NSHealthTeamMandate *pM, int angle, string distance )
                           :NSUtilDialog(parent, pCtx, "IDD_HEALTHTEAMMANDATE", pNSResModule), name(nam)
{
  if ((angle != -1) && (distance != "-1"))
  {
    _special_init = true ;
    _change_angle = angle ;
    _change_distance = atoi(distance.c_str()) ;
  }
  else
    _special_init = false ;

	initObjects() ;

  pProfession->SetReadOnly(true) ;
  pSpeciality->SetReadOnly(true) ;

	pMandate = pM ;
	pHTMemberInterface = NULL ;
}

void
HealthTeamMandateInterface::initObjects()
{
try
{
	pName = new OWL::TStatic(this, IDC_CIVILITE) ;

	VecteurString	VSType ;
	VSType.AddString(string("LMAND1")) ;					// mandat d'accès
	VSType.AddString(string("LROOT1")) ;					// mandat d'administration

	VecteurString	VSDurationUnit ;
//	pVSDurationUnit->push_back(new string("2SEC01")) ;  // secondes
//	pVSDurationUnit->push_back(new string("2MINU1")) ;  // minutes
  VSDurationUnit.AddString(string("2HEUR1")) ;  // heures
  VSDurationUnit.AddString(string("2DAT01")) ;  // jours
	VSDurationUnit.AddString(string("2DAT11")) ;	// semaines
  VSDurationUnit.AddString(string("2DAT21")) ;  // mois
	VSDurationUnit.AddString(string("2DAT31")) ;	// années

  VecteurString VSDistances ;
  VSDistances.AddString(string("0")) ;
  VSDistances.AddString(string("1")) ;
  VSDistances.AddString(string("2")) ;

  VecteurString VSAngles ;
  VSAngles.AddString(string("0")) ;
  VSAngles.AddString(string("3")) ;
  VSAngles.AddString(string("9")) ;
  VSAngles.AddString(string("15")) ;
  VSAngles.AddString(string("17")) ;
  VSAngles.AddString(string("21")) ;

	pProfession     = new NSUtilLexique(pContexte, this, IDCE_PROFESSION, pContexte->getDico()) ;
	pSpeciality     = new NSUtilLexique(pContexte, this, IDCE_SPECIALITY, pContexte->getDico()) ;
	pCBType         = new NSComboBox((TWindow *)this, IDCCB_TYPE, pContexte, &VSType) ;
	pDistance       = new HTComboBox(this, IDCCB_DISTANCE, pContexte, HTComboBox::distanceType, &VSDistances) ;
	pAngle          = new HTComboBox(this, IDCCB_ANGLE, pContexte, HTComboBox::angleType, &VSAngles) ;
	pBeginDate      = new NSUtilEditDateHeure(pContexte, this, IDCE_BEGINDATE) ;
	pEndDate        = new NSUtilEditDateHeure(pContexte, this, IDCE_ENDDATE) ;
	pDuration       = new NSEditNum(pContexte, this, IDCE_DURATION) ;
	pCBDurationUnit = new NSComboBox((TWindow *)this, IDCCB_DURATIONUNIT,	pContexte, &VSDurationUnit) ;
}
catch (...)
{
	erreur("Exception HealthTeamMandateInterface::initObjects.",  standardError, 0) ;
}
}

void
HealthTeamMandateInterface::GetProfession()
{
  switch(pAngle->getIntSelCode())
  {
    case  0 : // patient
              pProfession->setLabel("") ;
              pSpeciality->setLabel("") ;
              pProfession->SetReadOnly(true) ;
              pSpeciality->SetReadOnly(true) ;
              break ;
    case  3 : // médecin
              pProfession->setLabel("DMEDE1") ;
              pProfession->SetReadOnly(true) ;
              pSpeciality->SetReadOnly(false) ;
              pSpeciality->SetFocus() ;
              break ;
    case  9 : // professionnel de santé
              pProfession->setLabel("") ;
              pProfession->SetReadOnly(false) ;
              pSpeciality->SetReadOnly(false) ;
              pProfession->SetFocus() ;
              break ;
    case 15 : // social
              pProfession->setLabel("") ;
              pSpeciality->setLabel("") ;
              pProfession->SetReadOnly(false) ;
              pSpeciality->SetReadOnly(true) ;
              pProfession->SetFocus() ;
              break ;
    case 17 : // adm
              pProfession->setLabel("") ;
              pSpeciality->setLabel("") ;
              pProfession->SetReadOnly(false) ;
              pSpeciality->SetReadOnly(true) ;
              pProfession->SetFocus() ;
              break ;
    case 21 : // famille
              pProfession->setLabel("") ;
              pSpeciality->setLabel("") ;
              pProfession->SetReadOnly(true) ;
              pSpeciality->SetReadOnly(true) ;
              break ;
    default : break ;
  }
}

// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
HealthTeamMandateInterface::~HealthTeamMandateInterface()
{
	delete (pName) ;
	delete (pProfession) ;
	delete (pSpeciality) ;
	delete (pCBType) ;
	delete (pDistance) ;
	delete (pAngle) ;
	delete (pBeginDate) ;
	delete (pEndDate) ;
	delete (pDuration) ;
	delete (pCBDurationUnit) ;
}

void
HealthTeamMandateInterface::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
  pProfession->SetReadOnly(true) ;
  pSpeciality->SetReadOnly(true) ;
  if (pHTMemberInterface != NULL)
		name = pHTMemberInterface->getName() ;
  pName->SetText(name.c_str()) ;

	// Tabun Todo
 	//Effecuter le chargement de l'interface en mode modification
 	if (pMandate->getBeginDate() != "")
 	{
    pProfession->setLabel(pMandate->getProfession()) ;
    pSpeciality->setLabel(pMandate->getSpeciality()) ;
    int angle = pMandate->getAngle() ;
    char tangle[10] ;
    itoa(angle, tangle, 10) ;
    pAngle->setCode(tangle) ;

    switch(pMandate->getDistance())
    {
      case DTnotDef	   	:	pDistance->setCode("3") ;	break ;
      case patDist	   	:	pDistance->setCode("0") ;	break ;
      case nearDist	  	:	pDistance->setCode("1") ;	break ;
      case farDist	  	:	pDistance->setCode("2") ;	break ;
      case farAwayDist	:	pDistance->setCode("3") ;	break ;
    }

    switch (pMandate->getType())
    {
      case NSHealthTeamMandate::user	: pCBType->SetSelIndex(0) ;	break ;
      case NSHealthTeamMandate::root	: pCBType->SetSelIndex(1) ;	break ;
    }

    string date_deb = pMandate->getBeginDate() ;
    string date_end = pMandate->getEndDate() ;
    pBeginDate->setDate(date_deb) ;
    pEndDate->setDate(date_end) ;
  }
	else
	{
    // Initialisation de BeginDate à la date et heure du jour
    NVLdVTemps tpsToday ;
    tpsToday.takeTime() ;
    pBeginDate->setDate(tpsToday.donneDateHeure()) ;

    // Init job and speciality with Member's one
    if (pHTMemberInterface != NULL)
    {
      string  sPersid = pHTMemberInterface->pMember->getID() ;
      NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPersid, pidsCorresp) ;
      if (pPersonInfo)
      {
        pProfession->setLabel(pPersonInfo->getMetier()) ;
        pSpeciality->setLabel(pPersonInfo->getCivilite()) ;
      }
      // Default mandate : not administrator
    }
    pCBType->SetSelIndex(0) ;
	}

  if (_special_init == true)
  	specialInit() ;
}

void
HealthTeamMandateInterface::specialInit()
{
//	int		previous_angle	= pAngle->getIntSelCode() ;
// 	int 	previous_dist		= pDistance->getIntSelCode() ;
  char	dist[10] ;
  itoa(_change_distance, dist, 10) ;
	pDistance->setCode(dist) ;

 	char	tangle[10] ;
  itoa(_change_angle, tangle, 10) ;
  pAngle->setCode(tangle) ;
  pMandate->setDistance(getDistanceFromInt(_change_distance)) ;
  pMandate->setAngle(_change_angle) ;

  GetProfession() ;

  ::EnableWindow(GetDlgItem(IDCCB_ANGLE), FALSE) ;
	::EnableWindow(GetDlgItem(IDCCB_DISTANCE), FALSE) ;

  // on cache le bouton cancel
  ::EnableWindow(GetDlgItem(IDCANCEL), FALSE) ;
  ::ShowWindow(GetDlgItem(IDCANCEL),   SW_HIDE) ;
}

void
HealthTeamMandateInterface::ActualiseEnd()
{
	string date ;
  pBeginDate->getDate(&date) ;
	NVLdVTemps data ;
  data.initFromDate(date) ;

	string sVal = pDuration->GetText() ;

  int duree = atoi(sVal.c_str()) ;
  string sym = pCBDurationUnit->getSelCode() ;

  if (sym == string("2HEUR1")) // Ajoute heure
  	data.ajouteHeures(duree) ;
  if (sym == string("2DAT01")) // Ajoute jour
  	data.ajouteJours(duree) ;
  if (sym == string("2DAT11")) // Ajoute semaine
  	data.ajouteJours(duree) ;
  if (sym == string("2DAT21")) //Ajoute mois
  	data.ajouteMois(duree) ;
  if (sym == string("2DAT31")) //Ajoute année
  	data.ajouteAns(duree) ;

  date = data.donneDateHeure() ;
  pEndDate->setDate(date) ;
}

void
HealthTeamMandateInterface::CmOk()
{
	// fabTODO
	// enregistrer dans pMandate les données remplies dans la boite de dialogue
	string sType = getCBValue(pCBType) ;
	if (sType == "LROOT1")
		pMandate->setType(NSHealthTeamMandate::root) ;
	else // if (sType == "LMAND1") par défaut
		pMandate->setType(NSHealthTeamMandate::user) ;

	pMandate->setProfession(pProfession->getCode()) ;
	pMandate->setSpeciality(pSpeciality->getCode()) ;
	pMandate->setDistance(getDistanceFromInt(pDistance->getIntSelCode())) ;
	pMandate->setAngle(pAngle->getIntSelCode()) ;
	pMandate->setBeginDate(getStringDate(pBeginDate)) ;

  // attention la date de fin est prioritaire sur la durée
  if (getStringDate(pEndDate) != "")
		pMandate->setEndDate(getStringDate(pEndDate)) ;
  else
		pMandate->setDuration(getStringValue(pDuration), getCBValue(pCBDurationUnit)) ;

  if ((pAngle->getIntSelCode() == 3) || (pAngle->getIntSelCode() == 9))     // médecin ou professionel de santé
  	if (pContexte->getSuperviseur()->getFilGuide()->VraiOuFaux(pSpeciality->getCode(), "ES", "LSPES")  == false)
  	{
    	string sError = pContexte->getSuperviseur()->getText("HealthMandate", "SpecialityError") ;
    	erreur(sError.c_str(), standardError, 0) ;
      return ;
    }

	NSUtilDialog::CmOk() ;
}

void
HealthTeamMandateInterface::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------------------
// table de réponse
// --
// response table
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSListMandatesWindow, TListWindow)
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_COMMAND(CM_NEWMANDATE, CmNewMandate),
	EV_COMMAND(CM_DELMANDATE,	CmCloseMandate),
	EV_COMMAND(CM_MODMANDATE, CmModMandate),
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// constructeur
// --
// constructor
// -----------------------------------------------------------------------------
NSListMandatesWindow::NSListMandatesWindow(HealthTeamMemberInterface *parent, NSContexte *pCtx, int ressourceId, TModule *module)
                     :TListWindow((TWindow *)parent, ressourceId, module), NSRoot(pCtx)
{
	indexSel			= -1 ;
	pHTMInterface	= parent ;
}

// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
NSListMandatesWindow::~NSListMandatesWindow()
{
}

void
NSListMandatesWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;
}

void
NSListMandatesWindow::EvLButtonDblClk(uint /*modKeys*/, NS_CLASSLIB::TPoint& /*point*/)
{
}

voidNSListMandatesWindow::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point){  TListWindow::EvRButtonDown(modKeys, point) ;
  //int count = GetItemCount() ;
	TLwHitTestInfo 	info(point) ;

	int							index	= IndexItemSelect() ;
  //int index = HitTest(info) ;

  TPopupMenu			*menu = new TPopupMenu() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sEdit = pSuper->getText("HealthTeamWindows", "editThisMandate") ;
	string sNew  = pSuper->getText("HealthTeamWindows", "newMandate") ;
	string sDel  = pSuper->getText("HealthTeamWindows", "deleteThisMandate") ;

	if (index != -1)
	{
		menu->AppendMenu(MF_STRING, CM_MODMANDATE, sEdit.c_str()) ;
		menu->AppendMenu(MF_STRING, CM_NEWMANDATE, sNew.c_str()) ;
  	menu->AppendMenu(MF_STRING, CM_DELMANDATE, sDel.c_str()) ;
	}
	else
	{
		menu->AppendMenu(MF_STRING, CM_NEWMANDATE, sNew.c_str()) ;
	}

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}voidNSListMandatesWindow::EvKeyDown(uint /*key*/, uint /*repeatCount*/, uint /*flags*/){}int
NSListMandatesWindow::IndexItemSelect()
{
	int	count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)    if (GetItemState(i, LVIS_SELECTED))
    {
      index = i ;
      break ;
    }

  return index ;}

void
NSListMandatesWindow::EvSetFocus(HWND /* hWndLostFocus */)
{
	int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

  SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

// -----------------------------------------------------------------------------// interface de création de l'utilisateur// -----------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(CreateMandatInterface, TDialog)	EV_COMMAND(IDOK,              CmOk),
  EV_COMMAND(IDCANCEL,          CmCancel),
  EV_COMMAND(IDHELP,            CmHelp),
  EV_COMMAND(IDC_DEFINE_MANDAT, CmDefineMandat),
END_RESPONSE_TABLE ;CreateMandatInterface::CreateMandatInterface(TWindow *parent, NSContexte *pCtx, string login, string pass)	: NSUtilDialog(parent, pCtx, "IDD_CREATE_MANDAT_DIALOG", pNSResModule){ 	_acteur_edit_login 	 	= new TEdit(this,IDC_MC_LOGIN) ;	_acteur_edit_password	= new TEdit(this,IDC_MC_PASSWORD) ;
  _member_edit_ldv			= new TEdit(this,IDC_MC_LDV) ;
	_member_edit_adeli		= new TEdit(this,IDC_MC_ID_PROS) ;  _acteur_edit_liden		= new TEdit(this, IDC_LIDEN) ;  _is_admin							= new TCheckBox(this, IDC_CHECK_ADMIN) ;  _pass_to_add 					= pass ;	_login_to_add   			= login ;  if ((_login_to_add != "") && (_pass_to_add != ""))  	NewHealthMemberInformation() ;}

CreateMandatInterface::~CreateMandatInterface()
{	delete (_acteur_edit_login) ;  delete (_acteur_edit_password) ;  delete (_member_edit_ldv) ;  delete (_member_edit_adeli) ;  delete (_acteur_edit_liden) ;}

void
CreateMandatInterface::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
}


void
CreateMandatInterface::CmOk()
{
	full_data() ;
	if (CheckFull() == false)
  {
  	string rerror = pContexte->getSuperviseur()->getText("HealthCreation", "nonFullEdit") ;
    erreur(rerror.c_str(), standardError, 0) ;
    return ;
  }
	NSUtilDialog::CmOk() ;
}


void
CreateMandatInterface::NewHealthMemberInformation()
{
//	NSBasicAttributeArray *pAttrList = new NSBasicAttributeArray() ;
}


void
CreateMandatInterface::CmHelp()
{
	NSUtilDialog::CmHelp() ;
}


void
CreateMandatInterface::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

void
CreateMandatInterface::CmDefineMandat()
{
  // Appel du service
  // On cherche à récupérer le PersonID de la personne à mandater
  full_data() ;

  NSBasicAttributeArray AttrList ;
  const char            *serviceName = (NautilusPilot::SERV_GET_PERSON_INFORMATION).c_str() ;

  NSPersonGraphManager  GraphManager(pContexte->getSuperviseur()) ;
  NSDataGraph           *pGraph = GraphManager.getDataGraph() ;

  AttrList.push_back(new NSBasicAttribute(LOGIN, _acteur_login)) ;
  AttrList.push_back(new NSBasicAttribute(PASSWORD, _acteur_password)) ;
//	pAttrList->push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;
	if (_member_adeli != "")
  	AttrList.push_back(new NSBasicAttribute(ADELI_ID, _member_adeli)) ;
  else
  	AttrList.push_back(new NSBasicAttribute(LIDEN, _member_ldv)) ;

  bool bRes = pContexte->getSuperviseur()->getPilot()->getPersonInformation(serviceName, &AttrList, pGraph) ;

  if ((!bRes)||(!pGraph))
  {
    string sWarning = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
    if (sWarning != "")
    	erreur(sWarning.c_str(), standardError, 0) ;
    else
    {
      string sError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
      if (sError != "")
      	erreur(sError.c_str(), standardError, 0) ;
      else
      {
        string sErrorText = "Echec de la procédure de recuperation des informations du correspondent !" ;
        erreur(sError.c_str(), standardError, 0) ;
      }
    }
    return ;
  }

  string sPersonID = GraphManager.getPersonID() ;

  //recupération de la civité de la personne
  NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, sPersonID, pidsCorresp) ;

  // création du mandat
  NSHealthTeamMandate* mandate = new  NSHealthTeamMandate() ;
  _member = new  NSHealthTeamMember(sPersonID) ;
	HealthTeamMandateInterface *mandateInterface = new HealthTeamMandateInterface(this, pContexte, pPersonInfo->getCivilite(), mandate) ;
  _member->AddMandate(mandate) ;
  if (NULL != mandateInterface)
  {
    if (mandateInterface->Execute() == IDOK)
    {
      NSPatPathoArray Htpaht(pContexte->getSuperviseur()) ;
      _member->getTree(&Htpaht, 0) ;
      NSDataGraph AddGraph(pContexte->getSuperviseur(), graphPerson) ;
      AddGraph.setTree(&Htpaht, "") ;

      NSBasicAttributeArray	AttributesList ;
      AttributesList.push_back(new NSBasicAttribute(LOGIN, _acteur_login)) ;
      AttributesList.push_back(new NSBasicAttribute(PASSWORD, _acteur_password)) ;
      if (_acteur_liden != "")
        AttributesList.push_back(new NSBasicAttribute(LIDEN, _acteur_liden)) ;

      bool bRes = pContexte->getSuperviseur()->getPilot()->addHealthTeamMember((NautilusPilot::SERV_ADD_MANDATE).c_str(), &AttributesList, &AddGraph) ;
      if (!bRes)
        pContexte->getSuperviseur()->getText("patientManagement", "addMemberFaillure") ;
    }
    delete (mandateInterface) ;
	}
}

bool
CreateMandatInterface::CheckFull()
{
	return ((_acteur_login.size() > 0) && (_acteur_password.size() > 0) && ((_member_ldv.size() > 0) || (_member_adeli.size() > 0))) ;
}


void
CreateMandatInterface::full_data()
{
  char far login_acteur[80] ;
  char far pass_acteur[80] ;
  char far adeli_member[80] ;
  char far ldv_member[80] ;
  char far liden_acteur[80] ;

	_acteur_edit_login->GetText(login_acteur, 80) ;
	_acteur_edit_password->GetText(pass_acteur, 80) ;
  _member_edit_ldv->GetText(ldv_member, 80) ;
	_member_edit_adeli->GetText(adeli_member, 80) ;
  _acteur_edit_liden->GetText(liden_acteur, 80) ;

  _acteur_login     = string(login_acteur) ;
 	_acteur_password  = MD5_encrypt(string(pass_acteur)) ;
 	_member_ldv       = string(ldv_member) ;
  _member_adeli     = string(adeli_member) ;
  _acteur_liden     = string(liden_acteur) ;

  _admin = (_is_admin->GetCheck() == BF_CHECKED) ? true : false ;
}
