// -----------------------------------------------------------------------------
// nsHealthTeamInterface.cpp
// -----------------------------------------------------------------------------
// Interface Visuelle de Gestion l'Equipe de Santé
// -----------------------------------------------------------------------------
// $Revision: 1.12 $
// $Author: pameline $
// $Date: 2013/11/11 23:13:07 $
// -----------------------------------------------------------------------------
// FLP - juin 2004
// -----------------------------------------------------------------------------
#include <owl\window.h>
#include <owl\dialog.h>

#include "nssavoir\nsHealthTeamInterface.h"
#include "nssavoir\nsHealthTeamMemberInterface.h"
#include "nssavoir\healthteam.rh"
#include "nssavoir\nssavoir.h"
#include "nsbb\nslistwind.h"
#include "nsdn\nsdocnoy.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nshistdo.h"

// -----------------------------------------------------------------------------
// fonctions de tri sur des objets NSMemberMandatePair
// --
// functions that sort objects NSMemberMandatePair
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// fonctions de comparaison sur les noms des membres de l'Equipe de Santé.
// --
// functions that sort from HealthTeamMember name.
// -----------------------------------------------------------------------------
bool
membermandatepairsSortByNameSup(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getName() > pMMpair2->getName()) ;
}


bool
membermandatepairsSortByNameInf(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getName() < pMMpair2->getName()) ;
}


// -----------------------------------------------------------------------------
// fonctions de comparaison sur le métier de la paire membre-mandat.
// --
// functions that sort from member-mandate pair profession.
// -----------------------------------------------------------------------------
bool
membermandatepairsSortByProfessionSup(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getProfessionLabel() > pMMpair2->getProfessionLabel()) ;
}


bool
membermandatepairsSortByProfessionInf(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getProfessionLabel() < pMMpair2->getProfessionLabel()) ;
}


// -----------------------------------------------------------------------------
// fonctions de comparaison sur la spécialité de la paire membre-mandat.
// --
// functions that sort from member-mandate pair speciality.
// -----------------------------------------------------------------------------
bool
membermandatepairsSortBySpecialitySup(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getSpecialityLabel() > pMMpair2->getSpecialityLabel()) ;
}


bool
membermandatepairsSortBySpecialityInf(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getSpecialityLabel() < pMMpair2->getSpecialityLabel()) ;
}


// -----------------------------------------------------------------------------
// fonctions de comparaison sur le statut de la paire membre-mandat.
// --
// functions that sort from member-mandate pair status.
// -----------------------------------------------------------------------------
bool
membermandatepairsSortByStatusSup(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	// 0 : utilisateur non actif ; 1 : utilisateur actif ; A : administrateur actif ; T : administrateur de tutelle actif
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getStatus() > pMMpair2->getStatus()) ;
}


bool
membermandatepairsSortByStatusInf(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getStatus() < pMMpair2->getStatus()) ;
}


// -----------------------------------------------------------------------------
// fonctions de comparaison sur la distance au patient de la paire membre-mandat.
// --
// functions that sort from distance to Patient of member-mandate pair.
// -----------------------------------------------------------------------------
bool
membermandatepairsSortByDistanceSup(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getDistance() > pMMpair2->getDistance()) ;
}


bool
membermandatepairsSortByDistanceInf(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getDistance() < pMMpair2->getDistance()) ;
}


// -----------------------------------------------------------------------------
// fonctions de comparaison sur l'angle de la paire membre-mandat.
// --
// functions that sort from angle of member-mandate pair.
// -----------------------------------------------------------------------------
bool
membermandatepairsSortByAngleSup(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getAngle() > pMMpair2->getAngle()) ;
}


bool
membermandatepairsSortByAngleInf(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getAngle() < pMMpair2->getAngle()) ;
}


// -----------------------------------------------------------------------------
// fonctions de comparaison sur la date de début de la paire membre-mandat.
// --
// functions that sort from begin date of member-mandate pair.
// -----------------------------------------------------------------------------
bool
membermandatepairsSortByBeginDateSup(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getBeginDate() > pMMpair2->getBeginDate()) ;
}


bool
membermandatepairsSortByBeginDateInf(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getBeginDate() < pMMpair2->getBeginDate()) ;
}


// -----------------------------------------------------------------------------
// fonctions de comparaison sur la date de fin de la paire membre-mandat.
// --
// functions that sort from angle of member-mandate end date.
// -----------------------------------------------------------------------------
bool
membermandatepairsSortByEndDateSup(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getEndDate() > pMMpair2->getEndDate()) ;
}


bool
membermandatepairsSortByEndDateInf(NSMemberMandatePair *pMMpair1, NSMemberMandatePair *pMMpair2)
{
	if (!pMMpair1 || !pMMpair2)
		return false ;
	return (pMMpair1->getEndDate() < pMMpair2->getEndDate()) ;
}

// -------------------------------------------------------------------------
//
//  Class NSMemberMandatePair
//
// -------------------------------------------------------------------------

NSMemberMandatePair::NSMemberMandatePair(NSContexte *pCtx)
                    :NSRoot(pCtx)
{
  _sName           = string("") ;
  _sNameFromTraits = string("") ;

  _pMember  = (NSHealthTeamMember*) 0 ;
  _pMandate = (NSHealthTeamMandate*) 0 ;
}

NSMemberMandatePair::NSMemberMandatePair(NSContexte *pCtx, NSHealthTeamMember *pMe, NSHealthTeamMandate *pMa)
                    :NSRoot(pCtx)
{
  _sName           = string("") ;
  _sNameFromTraits = string("") ;

  _pMember  = pMe ;
  _pMandate = pMa ;
}

NSMemberMandatePair::NSMemberMandatePair(const NSMemberMandatePair& rv)
                    :NSRoot(rv.pContexte)
{
	_sName           = rv._sName ;
  _sNameFromTraits = rv._sNameFromTraits ;

  _pMember  = rv._pMember ;
  _pMandate = rv._pMandate ;
}

NSMemberMandatePair
NSMemberMandatePair::operator=(const NSMemberMandatePair& src)
{
  if (this == &src)
		return *this ;

	_sName           = src._sName ;
  _sNameFromTraits = src._sNameFromTraits ;

  _pMember  = src._pMember ;
  _pMandate = src._pMandate ;

  return (*this) ;
}


/*
string
NSMemberMandatePair::getProfessionLabel()
{
	string	sLanguage 			= pContexte->getUtilisateur()->donneLang() ;
	string	sLexiCode				= getProfession() ;
	string	sLexiCodeLabel	= "" ;

  if (sLexiCode != "")
		pContexte->getSuperviseur()->getDico()->donneLibelle(sLanguage, &sLexiCode, &sLexiCodeLabel) ;
  return sLexiCodeLabel ;
}


string
NSMemberMandatePair::getSpecialityLabel()
{
	string	sLanguage 			= pContexte->getUtilisateur()->donneLang() ;
	string	sLexiCode				= getSpeciality() ;
	string	sLexiCodeLabel	= "" ;

  if (sLexiCode != "")
		pContexte->getSuperviseur()->getDico()->donneLibelle(sLanguage, &sLexiCode, &sLexiCodeLabel) ;
  return sLexiCodeLabel ;
}
*/


// -----------------------------------------------------------------------------
// class NSMemberMandatePairArray
// -----------------------------------------------------------------------------
// paire Membre-Mandat, utilisée dans l'affichage de l'interface de gestion de
// l'Equipe de Santé.
// --
// member-mandate pair, used in the Graphical Interface for HealthTeam Managment.
// -----------------------------------------------------------------------------

NSMemberMandatePairArray::NSMemberMandatePairArray()
                         :NSMemberMandatePairVector()
{
}

NSMemberMandatePairArray::NSMemberMandatePairArray(const NSMemberMandatePairArray& src)
                         :NSMemberMandatePairVector()
{
	if (false == src.empty())
		for (NSMemberMandatePairConstIter iter = src.begin() ; src.end() != iter ; iter++)
			push_back(new NSMemberMandatePair(**iter)) ;
}

NSMemberMandatePairArray::NSMemberMandatePairArray(NSHealthTeamMemberArray *pMembersArray, NSContexte *pCtx)
                         :NSMemberMandatePairVector()
{
	initFromHealthTeam(pMembersArray, pCtx, true) ;
}

void
NSMemberMandatePairArray::initFromHealthTeam(NSHealthTeamMemberArray *pMembersArray, NSContexte* pCtx, bool bGetCivility)
{
try
{
	vider() ;

	if (((NSHealthTeamMemberArray*) NULL == pMembersArray) || (pMembersArray->empty()))
		return ;

  NSSuper *pSuper = pCtx->getSuperviseur() ;

	for (NSHealthTeamMemberIter memberIter = pMembersArray->begin() ; pMembersArray->end() != memberIter ; memberIter++)
	{
    if ((*memberIter)->pMandates && (false == (*memberIter)->pMandates->empty()))
    {
      for (NSHTMMandateIter mandateIter = (*memberIter)->pMandates->begin() ; (*memberIter)->pMandates->end() != mandateIter ; mandateIter++)
      {
        string memberName = string("") ;
        string memberId   = (*memberIter)->getID() ;

        switch ((*memberIter)->getType())
        {
          case (NSHealthTeamMember::person)	:
          {
            PIDSTYPE iPersonPidsType = pidsCorresp ;

            int iAngle = (*memberIter)->getAngle(*mandateIter) ;
            if ((0 == iAngle) || (21 == iAngle))
              iPersonPidsType = pidsPatient ;

            if (bGetCivility)
            {
              NSPersonInfo* pPersonInfo = pCtx->getPersonArray()->getPerson(pCtx, memberId, iPersonPidsType) ;
              memberName = pPersonInfo->getCivilite() ;
            }
            else
              memberName = string("-> ") + memberId ;

            break ;
          }
          case (NSHealthTeamMember::team) :
          {
            NSTeamGraphManager teamManager(pSuper) ;
            NSPatPathoArray	   ppt(pSuper) ;

            string sTrace = string("Getting health team ") + memberId ;
            pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

            teamManager.getTeamGraph(memberId, &ppt) ;

            sTrace = string("Health team ") + memberId + string(" obtained") ;
            pSuper->trace(&sTrace, 1, NSSuper::trDetails) ;

            memberName = teamManager.getTeamName(&ppt) ;
            break ;
          }
        }

        NSMemberMandatePair* temp = new NSMemberMandatePair(pCtx, *memberIter, *mandateIter) ;
        if (string("") == memberName)
          temp->setName("[inconnu]") ;
        else
          temp->setName(memberName) ;
        push_back(temp) ;
      }
    }
  }
}
catch (...)
{
  erreur("Exception NSMemberMandatePairArray::initFromHealthTeam", standardError, 0) ;
}
}

NSMemberMandatePairArray&
NSMemberMandatePairArray::operator=(const NSMemberMandatePairArray& src)
{
  if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (NSMemberMandatePairConstIter iter = src.begin() ; src.end() != iter ; iter++)
			push_back(new NSMemberMandatePair(**iter)) ;

	return (*this) ;
}

bool
NSMemberMandatePairArray::vider()
{
	if (false == empty())
		for (NSMemberMandatePairIter iter = begin() ; end() != iter ; )
		{
			delete (*iter) ;
			erase(iter) ;
		}

	return true ;
}

// -----------------------------------------------------------------------------
// class HealthTeamInterface
// -----------------------------------------------------------------------------
// Interface Graphique de la gestion de l'Equipe de Santé
// --
// Graphical Interface for HealthTeam Managment
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// table de réponse
// --
// response table
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(HealthTeamInterface, TDialog)
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_LVN_GETDISPINFO(IDC_MEMBERSLIST, DispInfoListe),
  EV_LVN_COLUMNCLICK(IDC_MEMBERSLIST,	LVNColumnclick),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
	EV_COMMAND(IDC_ADDMEMBER,						CmNewMember),
  EV_COMMAND(CM_NEW_MEMBER,						CmNewMember),
  EV_COMMAND(IDOK,                    CmOk),
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------
// constructeur par défaut
// --
// default constructor
// -----------------------------------------------------------------------------
HealthTeamInterface::HealthTeamInterface(TWindow *father, NSContexte *pCtx)
	                  :NSUtilDialog(father, pCtx, "IDD_HEALTHTEAM", pNSResModule)
{
#ifdef N_TIERS
	pHTManager						= new NSHealthTeamManager(pCtx->getPatient()->getHealthTeam()) ;
#endif
	pLWMemberMandatePairs	= new NSListMeMaPairsWindow	(this, IDC_MEMBERSLIST) ;
	pStaticPatient 				= new OWL::TStatic					(this, IDC_LABEL) ;

	pMemberMandatePairs		= new NSMemberMandatePairArray(pHTManager->getMembers(), pContexte) ;
  string capt = pContexte->getSuperviseur()->getText("HealthTeamWindows", "healthTeamCaption") ;
  SetCaption(capt.c_str()) ;

  bNaturallySorted 			= false ;
  iSortedColumn 				= 0 ;
}

// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
HealthTeamInterface::~HealthTeamInterface()
{
	delete pHTManager ;
	delete pMemberMandatePairs	;
	delete pLWMemberMandatePairs ;
	delete pStaticPatient ;
}

// -----------------------------------------------------------------------------
// setupWindow
// -----------------------------------------------------------------------------
void
HealthTeamInterface::SetupWindow()
{
	TDialog::SetupWindow() ;
	SetupColumns() ;

	displayList() ;
}

// -----------------------------------------------------------------------------
// configuration des colonnes.
// --
// setup columns.
// -----------------------------------------------------------------------------
void
HealthTeamInterface::SetupColumns()
{
	string work       = pContexte->getSuperviseur()->getText("HealthTeamWindows", "work") ;
  string type       = pContexte->getSuperviseur()->getText("HealthTeamWindows", "type") ;
  string speciality = pContexte->getSuperviseur()->getText("HealthTeamWindows", "speciality") ;
  string distance   = pContexte->getSuperviseur()->getText("HealthTeamWindows", "distance") ;
  string angle      = pContexte->getSuperviseur()->getText("HealthTeamWindows", "angle") ;
  string begindate  = pContexte->getSuperviseur()->getText("HealthTeamWindows", "begindate") ;
  string enddate    = pContexte->getSuperviseur()->getText("HealthTeamWindows", "enddate") ;
  string member     = pContexte->getSuperviseur()->getText("HealthTeamWindows", "member") ;
  string status     = pContexte->getSuperviseur()->getText("HealthTeamWindows", "member") ;

  pLWMemberMandatePairs->InsertColumn(0,  TListWindColumn((char *)member.c_str(),      100, TListWindColumn::Left,    0)) ;
	pLWMemberMandatePairs->InsertColumn(1,  TListWindColumn((char *)work.c_str(),         70, TListWindColumn::Right,   1)) ;
	pLWMemberMandatePairs->InsertColumn(2,  TListWindColumn((char *)speciality.c_str(),   70, TListWindColumn::Right,   2)) ;
	pLWMemberMandatePairs->InsertColumn(3,  TListWindColumn((char *)status.c_str(),       40, TListWindColumn::Center,  3)) ;
  pLWMemberMandatePairs->InsertColumn(4,  TListWindColumn((char *)distance.c_str(),     60, TListWindColumn::Right,   4)) ;
  pLWMemberMandatePairs->InsertColumn(5,  TListWindColumn((char *)angle.c_str(),       120, TListWindColumn::Right,   5)) ;
	pLWMemberMandatePairs->InsertColumn(6,  TListWindColumn((char *)begindate.c_str(),   120, TListWindColumn::Right,   6)) ;
	pLWMemberMandatePairs->InsertColumn(7,  TListWindColumn((char *)enddate.c_str(),     120, TListWindColumn::Right,   7)) ;
}

void
HealthTeamInterface::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
	// fabTODO
  const int       BufLen = 255 ;
  static char     buffer[BufLen] ;
  TListWindItem   &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  NVLdVTemps      tpsBegin, tpsEnd ;
  string          sBeginDate, sEndDate ;

  buffer[0] = '\0' ;
  int index = dispInfoItem.GetIndex() ;

	string	sLanguage = pContexte->getUtilisateur()->donneLang() ;	string	sLexiCode = "" ;
	string	sLexiCodeLabel = "" ;
  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
	{
  	case 0  : // name
          		sprintf(buffer, "%s", ((*pMemberMandatePairs)[index])->getName().c_str()) ;
              dispInfoItem.SetText(buffer) ;
              break ;

		case 1	:	// métier
							sprintf(buffer, "%s", ((*pMemberMandatePairs)[index])->getProfessionLabel().c_str()) ;
							dispInfoItem.SetText(buffer) ;
							break ;

		case 2	: // spécialité
							sprintf(buffer, "%s", ((*pMemberMandatePairs)[index])->getSpecialityLabel().c_str()) ;
							dispInfoItem.SetText(buffer) ;
							break ;

		case 3	:	// statut
							switch (((*pMemberMandatePairs)[index])->getStatus())
							{
								case 0	:
								case 1	: sprintf(buffer, "%d", ((*pMemberMandatePairs)[index])->getStatus()) ;	break ;
								case 2	: sprintf(buffer, "A") ;      break ;
								case 3	:	sprintf(buffer, "T") ;      break ;
								default	: sprintf(buffer, "Error") ;  break ;
              }
              dispInfoItem.SetText(buffer) ;
              break ;

		case 4	:	// distance
							sprintf(buffer, "%d", ((*pMemberMandatePairs)[index])->getDistance()) ;
              dispInfoItem.SetText(buffer) ;
              break ;

		case 5	: // angle
							sprintf(buffer, "%s", (((*pMemberMandatePairs)[index])->getStringAngle()).c_str()) ;
              dispInfoItem.SetText(buffer) ;
              break ;

		case 6	:	// date de début de mandat
              sBeginDate = ((*pMemberMandatePairs)[index])->getBeginDate() ;
              tpsBegin.initFromDateHeure(sBeginDate) ;
							sprintf(buffer, "%s", (tpsBegin.donneFormattedDateHeure(pContexte, sLanguage)).c_str()) ;
              dispInfoItem.SetText(buffer) ;
              break ;

		case 7	:	// date de fin de durée mandat
              sEndDate = ((*pMemberMandatePairs)[index])->getEndDate() ;
              if ("99990000000000" == sEndDate)
              {
                sEndDate = pContexte->getSuperviseur()->getText("HealthTeamWindows", "notimelimit") ;
                sprintf(buffer, "%s", sEndDate.c_str()) ;
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

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
HealthTeamInterface::LVNColumnclick(TLwNotify& lwn)
{
  sortByColumn(lwn.iSubItem) ;
}

// -----------------------------------------------------------------------------
// fonction qui trie les items en fonction d'une colonne.
// --
// function that sort items by a column.
// -----------------------------------------------------------------------------
void
HealthTeamInterface::sortByColumn(int iColumn)
{
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

	if (pMemberMandatePairs->empty())
		return ;

  switch (iColumn)
  {
    case 0  : // name
							if (bNaturallySorted)
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByNameSup) ;
              else
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByNameInf) ;
              break ;

		case 1	: // métier
							if (bNaturallySorted)
								sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByProfessionSup) ;
							else
								sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByProfessionInf) ;
              break ;

		case 2	: // spécialité
							if (bNaturallySorted)
								sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortBySpecialitySup) ;
							else
								sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortBySpecialityInf) ;
              break ;

    case 3  : // statuts
							if (bNaturallySorted)
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByStatusSup) ;
              else
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByStatusInf) ;
              break ;

    case 4  : // distance
							if (bNaturallySorted)
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByDistanceSup) ;
              else
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByDistanceInf) ;
              break ;

    case 5  : // angle
							if (bNaturallySorted)
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByAngleSup) ;
              else
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByAngleInf) ;
              break ;

    case 6  : // date de début de mandat
							if (bNaturallySorted)
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByBeginDateSup) ;
              else
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByBeginDateInf) ;
              break ;

    case 7  : // date de fin de mandat
							if (bNaturallySorted)
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByEndDateSup) ;
              else
                sort(pMemberMandatePairs->begin(), pMemberMandatePairs->end(), membermandatepairsSortByEndDateInf) ;
              break ;

		default	: break ;
	}

	displayList() ;
}

// -----------------------------------------------------------------------------
// affiche la liste des items.
// --
// display list of items.
// -----------------------------------------------------------------------------
void
HealthTeamInterface::displayList()
{
  pLWMemberMandatePairs->DeleteAllItems() ;

  if (pMemberMandatePairs->empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
	for (NSMemberMandatePairRIter rIter = pMemberMandatePairs->rbegin() ; rIter != pMemberMandatePairs->rend() ; rIter++)
	{
		TListWindItem		item((*rIter)->getName().c_str(), 0) ;
		pLWMemberMandatePairs->InsertItem(item) ;
	}
	return ;
}

// -----------------------------------------------------------------------------
// créer un nouveau membre.
// --
// create a new member.
// -----------------------------------------------------------------------------
void
HealthTeamInterface::CmNewMember()
{
try
{
	NSHealthTeamMember	*pMember = new NSHealthTeamMember() ;
  NSTPersonListDialog indep((TWindow *)this, pidsUtilisat, true, pContexte, 0, false, string(""), pNSResModule) ;
  if (indep.Execute() == IDOK)
  {
  	if (indep.bCreer)
    {
    	NSPersonInfo *pPersonInfo = pContexte->getPersonArray()->createPerson(pContexte, pidsCorresp) ;
      if (pPersonInfo)
      {
      	string sPids = pPersonInfo->getNss() ;  // On recupere le personne id
        pMember->setID(sPids) ;
        if (pMember->isValid())
        	pHTManager->addMember(pMember) ;                // On ajoute le membre
        HealthTeamMemberInterface	pNewMemberInterface(this, pContexte, pMember) ;
        if (pNewMemberInterface.Execute() == IDOK)
        {
        }
      }
    }
    else
    {
    	NSPersonInfo *temp = indep.pPersonSelect ;
      string sPersid = temp->getNss() ;
      if ("" != sPersid)
      {
      	pMember->setID(sPersid) ;
        if (pMember->isValid())
        	pHTManager->addMember(pMember) ;

        HealthTeamMemberInterface	*pNewMemberInterface = new HealthTeamMemberInterface(this, pContexte, pMember) ;
        if (pNewMemberInterface->Execute() == IDOK)
        {
        	if (pMember->isValid())
          {
          }
        }
        else
        {
        	// TODO
          // Rajouter l'erreur
        }
      }
        //delete pMember ;
    }
    if (NULL != pMemberMandatePairs)
      delete (pMemberMandatePairs) ;

    pMemberMandatePairs = new NSMemberMandatePairArray(pHTManager->getMembers(), pContexte) ;
    displayList() ;
	}
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception HealthTeamInterface::CmNewMember: ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception HealthTeamInterface::CmNewMember.", standardError, 0) ;
}
}


// -----------------------------------------------------------------------------
// ouvre l'affichage détaillé d'un membre.
// --
// open detailed display of a member.
// -----------------------------------------------------------------------------
void
HealthTeamInterface::openDetailsMember(int rank)
{
	if (rank == -1)
		return  ;

  HealthTeamMemberInterface	*pNewMemberInterface = new HealthTeamMemberInterface(this, pContexte, (*pMemberMandatePairs)[rank]->getMember()) ;
	pNewMemberInterface->Execute() ;
	delete pNewMemberInterface ;

  if (NULL != pMemberMandatePairs)
  	delete (pMemberMandatePairs) ;

  pMemberMandatePairs = new NSMemberMandatePairArray(pHTManager->getMembers(), pContexte) ;
  displayList() ;
}

void
HealthTeamInterface::CmOk()
{
	NSNoyauDocument	noyau(0, 0, 0, pContexte, false) ;
  bool					bContinuer = true ;

  NSHISTODocument *pDocHis = pContexte->getPatient()->getDocHis() ;

  DocumentIter iterDoc = pDocHis->getVectDocument()->begin() ;
  while ((pDocHis->getVectDocument()->end() != iterDoc) && bContinuer)
	{
    NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
    (*iterDoc)->initFromPatPatho(&PPT) ;

    if (false == PPT.empty())
    {
    	PatPathoIter iter = PPT.begin() ;
      if ((*iter)->getLexique() == string("LEQUI1"))
      	bContinuer = false ;
      else
      	iterDoc++ ;
    }
    else
    	iterDoc++ ;
	}

  (noyau._pDocInfo) = new NSDocumentInfo(pContexte) ;// *((*iterDoc)->pDonnees);

  NSDocumentData InfoData ;
  (*iterDoc)->initFromData(&InfoData) ;
  noyau._pDocInfo->setData(&InfoData) ;

  noyau.setPatPatho(pHTManager->getPatho()) ;
  noyau.enregistrePatPatho() ;

  TDialog::CmOk() ;
}



// -----------------------------------------------------------------------------
// class NSListMeMaPairsWindow
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// table de réponse
// --
// response table
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSListMeMaPairsWindow, TListWindow)
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_COMMAND(CM_NEW_MEMBER, CmNewMember),
	/*EV_COMMAND(CM_DEL_MEMBER,	CmDelMember),
	EV_COMMAND(CM_MOD_MEMBER, CmModMember),  */
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// construteur
// --
// constructor
// -----------------------------------------------------------------------------
NSListMeMaPairsWindow::NSListMeMaPairsWindow(HealthTeamInterface *parent, int ressourceId, TModule *module)
	: TListWindow((TWindow *)parent, ressourceId, module)
{
	indexSel			= -1 ;
	pHTInterface	= parent ;
}


// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
NSListMeMaPairsWindow::~NSListMeMaPairsWindow()
{
}


// -----------------------------------------------------------------------------
// setupWindow
// -----------------------------------------------------------------------------
void
NSListMeMaPairsWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;
}

// -----------------------------------------------------------------------------
// EvLButtonDblClk
// -----------------------------------------------------------------------------
void
NSListMeMaPairsWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& /* point */)
{
	int	index = IndexItemSelect() ;
	pHTInterface->openDetailsMember(index) ;
}

// -----------------------------------------------------------------------------// EvRButtonDown// -----------------------------------------------------------------------------voidNSListMeMaPairsWindow::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point){  TListWindow::EvRButtonDown(modKeys, point) ;
  //int count = GetItemCount() ;
	TLwHitTestInfo 	info(point) ;

	int							index	= IndexItemSelect() ;
  //int index = HitTest(info) ;

  TPopupMenu			*menu = new TPopupMenu() ;
	if (index != -1)
	{
		menu->AppendMenu(MF_STRING, CM_MOD_MEMBER, "Modifier ce Membre") ;
		menu->AppendMenu(MF_STRING, CM_NEW_MEMBER, "Créer un nouveau Membre") ;
  	menu->AppendMenu(MF_STRING, CM_DEL_MEMBER, "Effacer ce Membre") ;
	}
	else
	{
		menu->AppendMenu(MF_STRING, CM_NEW_MEMBER, "Créer un nouveau Membre") ;
	}

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}// -----------------------------------------------------------------------------// EvKeyDown// -----------------------------------------------------------------------------voidNSListMeMaPairsWindow::EvKeyDown(uint /*key*/, uint /*repeatCount*/, uint /*flags*/){}
// -----------------------------------------------------------------------------
// renvoie l'index correspondant au membre sélectionné.
// --
// return index that corresponds a selected member.
// -----------------------------------------------------------------------------
int
NSListMeMaPairsWindow::IndexItemSelect()
{
	int	count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)    if (GetItemState(i, LVIS_SELECTED))
    {
      index = i ;
      break ;
    }

  return index ;}


// -----------------------------------------------------------------------------
// EvSetFocus
// -----------------------------------------------------------------------------
void
NSListMeMaPairsWindow::EvSetFocus(HWND /* hWndLostFocus */)
{
	int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

  SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}



/*
// -----------------------------------------------------------------------------
// class NSListMembersWindow
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// table de réponse
// --
// response table
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSListMembersWindow, TListWindow)
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_COMMAND(CM_NEW_MEMBER, CmNewMember),
	EV_COMMAND(CM_DEL_MEMBER,	CmDelMember),
	EV_COMMAND(CM_MOD_MEMBER, CmModMember),
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// construteur
// --
// constructor
// -----------------------------------------------------------------------------
NSListMembersWindow::NSListMembersWindow(HealthTeamInterface *parent, int ressourceId, TModule *module)
	: TListWindow((TWindow *)parent, ressourceId, module)
{
	indexSel			= -1 ;
	pHTInterface	= parent ;
}


// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
NSListMembersWindow::~NSListMembersWindow()
{
}

// -----------------------------------------------------------------------------
// setupWindow
// -----------------------------------------------------------------------------
void
NSListMembersWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;
}


// -----------------------------------------------------------------------------
// créé un nouveau membre.
// --
// creates a new member.
// -----------------------------------------------------------------------------
void
NSListMembersWindow::CmNewMember()
{
	pHTInterface->CmNewMember() ;
}


// -----------------------------------------------------------------------------
// efface le membre sélectionné.
// --
// deletes selected member.
// -----------------------------------------------------------------------------
void
NSListMembersWindow::CmDelMember()
{
}


// -----------------------------------------------------------------------------
// modifie le membre sélectionné.
// --
// modifies selected member.
// -----------------------------------------------------------------------------
void
NSListMembersWindow::CmModMember()
{
	pHTInterface->CmModMember() ;
}


// -----------------------------------------------------------------------------
// EvLButtonDblClk
// -----------------------------------------------------------------------------
void
NSListMembersWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	int	index = IndexItemSelect() ;
	pHTInterface->openDetailsMember(index) ;
}


// -----------------------------------------------------------------------------// EvRButtonDown// -----------------------------------------------------------------------------voidNSListMembersWindow::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point){  TListWindow::EvRButtonDown(modKeys, point) ;
  //int count = GetItemCount() ;
	TLwHitTestInfo 	info(point) ;

	int							index	= IndexItemSelect() ;
  //int index = HitTest(info) ;

  TPopupMenu			*menu = new TPopupMenu() ;
	if (index != -1)
	{
		menu->AppendMenu(MF_STRING, CM_MOD_MEMBER, "Modifier ce Membre") ;
		menu->AppendMenu(MF_STRING, CM_NEW_MEMBER, "Créer un nouveau Membre") ;
  	menu->AppendMenu(MF_STRING, CM_DEL_MEMBER, "Effacer ce Membre") ;
	}
	else
	{
		menu->AppendMenu(MF_STRING, CM_NEW_MEMBER, "Créer un nouveau Membre") ;
	}

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}// -----------------------------------------------------------------------------// EvKeyDown// -----------------------------------------------------------------------------voidNSListMembersWindow::EvKeyDown(uint key, uint repeatCount, uint flags){}
// -----------------------------------------------------------------------------
// renvoie l'index correspondant au membre sélectionné.
// --
// return index that corresponds a selected member.
// -----------------------------------------------------------------------------
int
NSListMembersWindow::IndexItemSelect()
{
	int	count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)    if (GetItemState(i, LVIS_SELECTED))
    {
      index = i ;
      break ;
    }

  return index ;}


// -----------------------------------------------------------------------------
// EvSetFocus
// -----------------------------------------------------------------------------
void
NSListMembersWindow::EvSetFocus(HWND hWndLostFocus)
{
	int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

  SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIF_STATE) ;
}
*/


// -----------------------------------------------------------------------------
// lance l'interface de l'Equipe de Santé.
// --
// launch HealthTeam interface.
// -----------------------------------------------------------------------------
void
_CLASSMODE runHTDialog(NSContexte *pCtx)
{
	HealthTeamInterface	*pHTInterface = new HealthTeamInterface(pCtx->GetMainWindow(), pCtx) ;
	pHTInterface->Execute() ;
	delete pHTInterface ;
}
