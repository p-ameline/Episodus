
#include "nsepisod\objectif_viewer.rh"
#include "nsdn\nsanxary.h"
#include "nsdn\nsdochis.h"
#include "nsepisod\objectif_viewer.h"
#include "nsepisod\nsepisod.h"

// -----------------------------------------------------------------------------
//
// class ObjectifViewerDlg
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ObjectifViewerDlg, NSUtilDialog)
    EV_WM_CLOSE,
    EV_WM_PAINT,
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_BN_CLICKED(IDC_ONE,CmPunctual),
    EV_BN_CLICKED(IDC_CYCLE,CmCyclic),
    EV_BN_CLICKED(IDC_PERMANENT,CmPermanent),
END_RESPONSE_TABLE;

ObjectifViewerDlg::ObjectifViewerDlg(TWindow* pere, NSContexte* pCtx, bool withCreat, NSLdvGoal* pRefObj)
                  :NSUtilDialog(pere, pCtx, "IDD_OBJECTIF_VIEW", pNSResModule)
{
try
{
  // Initialisation des donnees
	creatMod = withCreat ;

	pObjectifLexicon    = new NSUtilLexique(pContexte, this, IDC_OBJECTIF_NAME,pContexte->getDico()) ;
	pUnitValue          = new NSUtilLexique(pContexte, this, IDC_VALUE_UNIT,pContexte->getDico()) ;

	const char *tempsUnit[] = { "2HEUR1", "2DAT01", "2DAT21", "2DAT11", "2DAT31" } ;
	pPeriodUnit         = new NSComboBox(this, IDC_PERIOD_UNIT, pContexte, tempsUnit, 5) ;
	pStaticPeriod       = new TStatic((TWindow*)this, IDC_STATIC_PERIOD) ;
	pStaticMoments      = new TStatic((TWindow*)this, IDC_STATIC_MOMMENTS) ;

	pStartDate          = new NSUtilEditDateHeure(pContexte, this, IDC_DATE_DEBUT) ;
	pStartEveniment     = new NSUtilLexique(pContexte, this, IDC_EVENIMENT_DEBUT, pContexte->getDico()) ;

  pEndDate            = new NSUtilEditDateHeure(pContexte, this, IDC_DATE_END) ;
  pEndEveniment       = new NSUtilLexique(pContexte, this, IDC_EVENIMENT_END, pContexte->getDico()) ;

  //lines
  pStaticLines1       = new TStatic((TWindow*)this, IDC_LINE1) ;
  pStaticLines2       = new TStatic((TWindow*)this, IDC_LINE2) ;
  pStaticLines3       = new TStatic((TWindow*)this, IDC_LINE3) ;
  pStaticLines4       = new TStatic((TWindow*)this, IDC_LINE4) ;
  pStaticLines5       = new TStatic((TWindow*)this, IDC_LINE5) ;
  pStaticLines6       = new TStatic((TWindow*)this, IDC_LINE6) ;

  //period management
  pPeriodAuthorizeMax = new NSObjectifEditNum(pContexte, this, IDC_PERIOD_AUTHORIZE_MAX, 5, &NS_CLASSLIB::TColor::LtRed,    &NS_CLASSLIB::TColor::LtYellow) ;
  pPeriodRecommendMax = new NSObjectifEditNum(pContexte, this, IDC_PERIOD_RECOMMEND_MAX, 4, &NS_CLASSLIB::TColor::LtYellow, &NS_CLASSLIB::TColor::LtGreen) ;
  pPeriodIdealMax     = new NSObjectifEditNum(pContexte, this, IDC_PERIOD_IDEAL_MAX,     3, &NS_CLASSLIB::TColor::LtGreen,  &NS_CLASSLIB::TColor::LtBlue) ;
  pPeriodIdealMin     = new NSObjectifEditNum(pContexte, this, IDC_PERIOD_IDEAL_MIN,     2, &NS_CLASSLIB::TColor::LtBlue,   &NS_CLASSLIB::TColor::LtGreen) ;
  pPeriodRecommendMin = new NSObjectifEditNum(pContexte, this, IDC_PERIOD_RECOMMEND_MIN, 1, &NS_CLASSLIB::TColor::LtGreen,  &NS_CLASSLIB::TColor::LtYellow) ;
  pPeriodAuthorizeMin = new NSObjectifEditNum(pContexte, this, IDC_PERIOD_AUTHORIZE_MIN, 0, &NS_CLASSLIB::TColor::LtYellow, &NS_CLASSLIB::TColor::LtRed) ;

  pValueAuthorizeMax  = new NSObjectifEditNum(pContexte, this, IDC_VALUE_AUTHORIZE_MAX, 5, &NS_CLASSLIB::TColor::LtRed,    &NS_CLASSLIB::TColor::LtYellow) ;
  pValueRecommendMax  = new NSObjectifEditNum(pContexte, this, IDC_VALUE_RECOMMEND_MAX, 4, &NS_CLASSLIB::TColor::LtYellow, &NS_CLASSLIB::TColor::LtGreen) ;
  pValueIdealMax      = new NSObjectifEditNum(pContexte, this, IDC_VALUE_IDEAL_MAX,     3, &NS_CLASSLIB::TColor::LtGreen,  &NS_CLASSLIB::TColor::LtBlue) ;
  pValueIdealMin      = new NSObjectifEditNum(pContexte, this, IDC_VALUE_IDEAL_MIN,     2, &NS_CLASSLIB::TColor::LtBlue,   &NS_CLASSLIB::TColor::LtGreen) ;
  pValueRecommendMin  = new NSObjectifEditNum(pContexte, this, IDC_VALUE_RECOMMEND_MIN, 1, &NS_CLASSLIB::TColor::LtGreen,  &NS_CLASSLIB::TColor::LtYellow) ;
  pValueAuthorizeMin  = new NSObjectifEditNum(pContexte, this, IDC_VALUE_AUTHORIZE_MIN, 0, &NS_CLASSLIB::TColor::LtYellow, &NS_CLASSLIB::TColor::LtRed) ;

  //date management
  pDateAuthorizeMax   = new NSObjectifEditDateHeure(pContexte, this, IDC_DATE_AUTHORIZE_MAX, &NS_CLASSLIB::TColor::LtRed,    &NS_CLASSLIB::TColor::LtYellow) ;
  pDateRecommendMax   = new NSObjectifEditDateHeure(pContexte, this, IDC_DATE_RECOMMAND_MAX, &NS_CLASSLIB::TColor::LtYellow, &NS_CLASSLIB::TColor::LtGreen) ;
  pDateIdealMax       = new NSObjectifEditDateHeure(pContexte, this, IDC_DATE_IDEAL_MAX,     &NS_CLASSLIB::TColor::LtGreen,  &NS_CLASSLIB::TColor::LtBlue) ;
  pDateIdealMin       = new NSObjectifEditDateHeure(pContexte, this, IDC_DATE_IDEAL_MIN,     &NS_CLASSLIB::TColor::LtBlue,   &NS_CLASSLIB::TColor::LtGreen) ;
  pDateRecommendMin   = new NSObjectifEditDateHeure(pContexte, this, IDC_DATE_RECOMMAND_MIN, &NS_CLASSLIB::TColor::LtGreen,  &NS_CLASSLIB::TColor::LtYellow) ;
  pDateAuthorizeMin   = new NSObjectifEditDateHeure(pContexte, this, IDC_DATE_AUTHORIZE_MIN, &NS_CLASSLIB::TColor::LtYellow, &NS_CLASSLIB::TColor::LtRed) ;

  //mode
  pMode               = new TGroupBox(this, IDC_GROUP) ;
  pCycle              = new TRadioButton(this, IDC_CYCLE,pMode) ;
  pOne                = new TRadioButton(this, IDC_ONE,pMode) ;
  pPermanent          = new TRadioButton(this, IDC_PERMANENT, pMode) ;

  pObjGroup           = new TGroupBox(this, IDC_OBJECTIF_BOX) ;
  pPresence           = new TRadioButton(this, IDC_PRESENCE, pMode) ;
  pAbsence            = new TRadioButton(this, IDC_ABSENT, pMode) ;

  pComplement         = new TEdit((TWindow*)this, IDC_COMPLEMENT) ;

  objectifType        = ONE_OBJECTIF ;

  pObjectif           = pRefObj ;

	for (int i = 0 ; i < 6 ; i++)
	{
		arrayLeftNumControls[i]  = NULL ;
		arrayRightNumControls[i] = NULL ;
		arrayLeftDateControls[i] = NULL ;
	}
}
catch (...)
{
	erreur("Exception (new ObjectifViewerDlg)", standardError, 0) ;
}
}

ObjectifViewerDlg::~ObjectifViewerDlg()
{
  delete pObjectifLexicon ;
  delete pUnitValue ;
  delete pPeriodUnit ;
  delete pStaticPeriod ;
  delete pStaticMoments ;

  delete pStartDate ;
  delete pStartEveniment ;

  delete pEndDate ;
  delete pEndEveniment ;

  delete pStaticLines1 ;
  delete pStaticLines2 ;
  delete pStaticLines3 ;
  delete pStaticLines4 ;
  delete pStaticLines5 ;
  delete pStaticLines6 ;

  delete pPeriodIdealMin ;
  delete pPeriodIdealMax ;
  delete pPeriodRecommendMin ;
  delete pPeriodRecommendMax ;
  delete pPeriodAuthorizeMin ;
	delete pPeriodAuthorizeMax ;

  delete pDateIdealMin ;
  delete pDateIdealMax ;
  delete pDateRecommendMin ;
  delete pDateRecommendMax ;
  delete pDateAuthorizeMin ;
	delete pDateAuthorizeMax ;

  delete pValueIdealMin ;
  delete pValueIdealMax ;
  delete pValueRecommendMin ;
  delete pValueRecommendMax ;
  delete pValueAuthorizeMin ;
	delete pValueAuthorizeMax ;

  delete pMode ;
  delete pCycle ;
  delete pOne ;
  delete pPermanent ;

  delete pObjGroup ;
  delete pPresence ;
  delete pAbsence ;

  delete pComplement ;
}

bool
ObjectifViewerDlg::InitFromSavedObjectif()
{
  if (NULL == pObjectif)
    return false ;

  //objectif name
  pObjectifLexicon->setLabel(pObjectif->getLexique()) ;
  if (string("") != pObjectif->_sCertitude)
    pAbsence->Check() ;
  else
    pPresence->Check() ;

  if (string("") != pObjectif->_sComplementText)
    pComplement->SetText(pObjectif->_sComplementText.c_str() ) ;
  //open data
  if (false == pObjectif->_tDateOuverture.estVide())
    pStartDate->setDate(pObjectif->_tDateOuverture.donneDateHeure()) ;
  else if (string("") != pObjectif->_sOpenEventNode)
    pStartEveniment->setLabel(pObjectif->_sOpenEventNode) ;
        // else
        //    return false ;

  //close data
  if ((false == pObjectif->_tDateFermeture.estVide()) &&
      (false == pObjectif->_tDateFermeture.estNoLimit()))
    pEndDate->setDate(pObjectif->_tDateFermeture.donneDateHeure()) ;
  else if (string("") != pObjectif->_sCloseEventNode)
    pEndEveniment->setLabel(pObjectif->_sCloseEventNode) ;
        // else
        //    return false ;

	switch(pObjectif->_iRythme)
	{
		case NSLdvGoal::ponctuel :
			setDate() ;
			setValue() ;
			CmPunctual();
			pOne->Check() ;
			break ;
		case NSLdvGoal::cyclic :
			setPeriode() ;
			setValue() ;
			CmCyclic();
			pCycle->Check() ;
			break ;
		case NSLdvGoal::permanent :
			setValue() ;
			CmPermanent();
			pPermanent->Check() ;
			break ;
	}
	return true ;
	// decodeObjectif(pObjectif->iRythme) ;
}

//if a data is in 10 chars add 10
string completeData(string sDate, NSUtilEditDateHeure* /* pControl */)
{
	if (strlen(sDate.c_str()) == 8)
  	return sDate + "000000" ;
	return sDate ;
}

void
ObjectifViewerDlg::setDate()
{
  if (pObjectif->_sDateDebutIdeal != "")
    pDateIdealMin->setDate(completeData(pObjectif->_sDateDebutIdeal, pDateIdealMin)) ;
  if (pObjectif->_sDateDebutIdealMax != "")
    pDateIdealMax->setDate(completeData(pObjectif->_sDateDebutIdealMax, pDateIdealMax)) ;
  if (pObjectif->_sDateDebutConseille != "")
    pDateRecommendMin->setDate(completeData(pObjectif->_sDateDebutConseille, pDateRecommendMin)) ;
  if (pObjectif->_sDateDebutConseilMax != "")
    pDateRecommendMax->setDate(completeData(pObjectif->_sDateDebutConseilMax, pDateRecommendMax)) ;
  if (pObjectif->_sDateDebutAutorise != "")
    pDateAuthorizeMin->setDate(completeData(pObjectif->_sDateDebutAutorise, pDateAuthorizeMin)) ;
  if (pObjectif->_sDateDebutCritique != "")
    pDateAuthorizeMax->setDate(completeData(pObjectif->_sDateDebutCritique, pDateAuthorizeMax)) ;
}

void
ObjectifViewerDlg::setPeriode()
{
	bool found = false;
	string sUnit = "";
	if (pObjectif->_dDelaiDebutIdeal > 0 )
	{
		pPeriodIdealMin->SetDoubleIntoEditNum(pObjectif->_dDelaiDebutIdeal) ;
		sUnit = pObjectif->_sUniteDebutIdeal ;
		found = true ;
	}
	if (pObjectif->_dDelaiDebutIdealMax > 0 )
	{
		pPeriodIdealMax->SetDoubleIntoEditNum(pObjectif->_dDelaiDebutIdealMax) ;
		sUnit = pObjectif->_sUniteDebutIdealMax ;
		found = true ;
	}
	if (pObjectif->_dDelaiDebutConseille > 0 )
	{
		pPeriodRecommendMin->SetDoubleIntoEditNum(pObjectif->_dDelaiDebutConseille) ;
		sUnit = pObjectif->_sUniteDebutConseille ;
		found = true ;
	}
	if (pObjectif->_dDelaiDebutConseilMax > 0 )
	{
		pPeriodRecommendMax->SetDoubleIntoEditNum(pObjectif->_dDelaiDebutConseilMax) ;
		sUnit = pObjectif->_sUniteDebutConseilMax ;
		found = true ;
	}
	if (pObjectif->_dDelaiDebutAutorise > 0)
	{
		pPeriodAuthorizeMin->SetDoubleIntoEditNum(pObjectif->_dDelaiDebutAutorise) ;
		sUnit = pObjectif->_sUniteDebutAutorise ;
		found = true ;
	}
	if(pObjectif->_dDelaiDebutCritique > 0 )
	{
		pPeriodAuthorizeMax->SetDoubleIntoEditNum(pObjectif->_dDelaiDebutCritique) ;
		sUnit = pObjectif->_sUniteDebutCritique ;
		found = true ;
	}
	if ((found) && (sUnit == ""))
		return ;

	if (found)
		pPeriodUnit->setCode(sUnit + "1") ;
}


void
ObjectifViewerDlg::setValue()
{
	bool found = false;
	string sUnit = "" ;

	if (pObjectif->_dValMinIdeal > 0)
	{
		pValueIdealMin->SetDoubleIntoEditNum(pObjectif->_dValMinIdeal) ;
		sUnit = pObjectif->_sUniteValMinIdeal ;
		found = true ;
	}
	if (pObjectif->_dValMaxIdeal > 0)
	{
		pValueIdealMax->SetDoubleIntoEditNum(pObjectif->_dValMaxIdeal) ;
		sUnit = pObjectif->_sUniteValMaxIdeal ;
		found = true ;
	}
	if (pObjectif->_dValMinConseille > 0)
	{
		pValueRecommendMin->SetDoubleIntoEditNum(pObjectif->_dValMinConseille) ;
		sUnit = pObjectif->_sUniteValMinConseille ;
		found = true ;
	}
	if (pObjectif->_dValMaxConseille > 0)
	{
		pValueRecommendMax->SetDoubleIntoEditNum(pObjectif->_dValMaxConseille) ;
		sUnit = pObjectif->_sUniteValMaxConseille ;
		found = true ;
	}
	if (pObjectif->_dValMinAutorise > 0)
	{
		pValueAuthorizeMin->SetDoubleIntoEditNum(pObjectif->_dValMinAutorise) ;
		sUnit = pObjectif->_sUniteValMinAutorise ;
		found = true ;
	}
	if (pObjectif->_dValMaxAutorise > 0)
	{
		pValueAuthorizeMax->SetDoubleIntoEditNum(pObjectif->_dValMaxAutorise) ;
		sUnit = pObjectif->_sUniteValMaxAutorise ;
		found = true ;
	}

	if((found) && (sUnit == ""))
		return ;

	if (found)
		pUnitValue->setLabel(sUnit + "1") ;
}

void
ObjectifViewerDlg::SetupWindow()
{
	TDialog::SetupWindow() ;

	if (false == creatMod)
		InitFromSavedObjectif() ;
	else
	{
		CmCyclic() ;
		pCycle->Check() ;
		pPresence->Check() ;

    NVLdVTemps tNow ;
    tNow.takeTime() ;
    pStartDate->setDate(tNow.donneDateHeure()) ;
	}
}

void
ObjectifViewerDlg::decodeObjectif(int type)
{
  switch(type)
  {
    case CYCLE_OBJECTIF :
      pCycle->Check() ;
      CmCyclic() ;
      break ;

    case ONE_OBJECTIF :
      pOne->Check() ;
      CmPunctual() ;
      break ;

    case PERMANENT_OBJECTIF :
      pPermanent->Check() ;
      CmPermanent() ;
      break ;
  }
}

void
ObjectifViewerDlg::ShowLine(int cmdShow)
{
  pStaticLines1->Show(cmdShow) ;
  pStaticLines2->Show(cmdShow) ;
  pStaticLines3->Show(cmdShow) ;
  pStaticLines4->Show(cmdShow) ;
  pStaticLines5->Show(cmdShow) ;
  pStaticLines6->Show(cmdShow) ;
}

void
ObjectifViewerDlg::ShowPeriod(int cmdShow)
{
  pPeriodIdealMin->Show(cmdShow) ;
  pPeriodIdealMax->Show(cmdShow) ;
  pPeriodRecommendMin->Show(cmdShow) ;
  pPeriodRecommendMax->Show(cmdShow) ;
  pPeriodAuthorizeMin->Show(cmdShow) ;
	pPeriodAuthorizeMax->Show(cmdShow) ;

  pPeriodUnit->Show(cmdShow) ;
  pStaticPeriod->Show(cmdShow) ;
}

void
ObjectifViewerDlg::ShowDate(int cmdShow)
{
  pStaticMoments->Show(cmdShow) ;

  pDateIdealMin->Show(cmdShow) ;
  pDateIdealMax->Show(cmdShow) ;
  pDateRecommendMin->Show(cmdShow) ;
  pDateRecommendMax->Show(cmdShow) ;
  pDateAuthorizeMin->Show(cmdShow) ;
	pDateAuthorizeMax->Show(cmdShow) ;
}

void
ObjectifViewerDlg::ShowValue(int cmdShow)
{
  pValueIdealMin->Show(cmdShow) ;
  pValueIdealMax->Show(cmdShow) ;
  pValueRecommendMin->Show(cmdShow) ;
  pValueRecommendMax->Show(cmdShow) ;
  pValueAuthorizeMin->Show(cmdShow) ;
	pValueAuthorizeMax->Show(cmdShow) ;
}

//pos = 0 if left
//pos = 1 if right
void
ObjectifViewerDlg::getArrayNumControls(NSObjectifEditNum *pArray[6], int pos)
{
	if (pArray == NULL)
		return ;

	switch (pos)
	{
		case 0:
			pArray[5] = pPeriodAuthorizeMax ;
			pArray[4] = pPeriodRecommendMax ;
			pArray[3] = pPeriodIdealMax ;
			pArray[2] = pPeriodIdealMin ;
			pArray[1] = pPeriodRecommendMin ;
			pArray[0] = pPeriodAuthorizeMin ;
			break;

		case 1:
			pArray[5] = pValueAuthorizeMax ;
			pArray[4] = pValueRecommendMax ;
			pArray[3] = pValueIdealMax ;
			pArray[2] = pValueIdealMin ;
			pArray[1] = pValueRecommendMin ;
			pArray[0] = pValueAuthorizeMin ;
			break ;
	}
}

void
ObjectifViewerDlg::getArrayDateControls(NSObjectifEditDateHeure *pArray[6])
{
	if (NULL == pArray)
		return ;

	pArray[5] = pDateAuthorizeMax ;
	pArray[4] = pDateRecommendMax ;
	pArray[3] = pDateIdealMax ;
	pArray[2] = pDateIdealMin ;
	pArray[1] = pDateRecommendMin ;
	pArray[0] = pDateAuthorizeMin ;
}

void
ObjectifViewerDlg::CmCyclic()
{
  ShowDate(SW_HIDE) ;
  ShowPeriod(SW_SHOW) ;
  ShowLine(SW_SHOW) ;
  ShowValue(SW_SHOW);

  objectifType = CYCLE_OBJECTIF ;
  typeLeft = 1 ;
  getArrayNumControls(arrayLeftNumControls, LEFT) ;
  getArrayNumControls(arrayRightNumControls, RIGHT) ;

  TWindow::Invalidate() ;
}

void
ObjectifViewerDlg::CmPunctual()
{
  ShowDate(SW_SHOW) ;
  ShowPeriod(SW_HIDE) ;
  ShowValue(SW_SHOW) ;
  ShowLine(SW_SHOW) ;

  objectifType = ONE_OBJECTIF ;
  typeLeft = 2 ;

  getArrayDateControls(arrayLeftDateControls);
  getArrayNumControls(arrayRightNumControls, RIGHT);
  TWindow::Invalidate();
}

void
ObjectifViewerDlg::CmPermanent()
{
  ShowDate(SW_HIDE) ;
  ShowPeriod(SW_HIDE) ;
  ShowLine(SW_HIDE) ;
  ShowValue(SW_SHOW) ;

  typeLeft = 0 ;
  getArrayNumControls(arrayRightNumControls, RIGHT);
  objectifType = PERMANENT_OBJECTIF ;
  TWindow::Invalidate();
}

bool
ObjectifViewerDlg::hasControlValue(NSEditNum* pControl)
{
	if (((NSEditNum*) NULL == pControl) || !(pControl->HWindow))
		return false ;

  string sVal = pControl->GetText() ;
	if (string("") != sVal)
		return true ;

	return false ;
}

bool
ObjectifViewerDlg::hasControlValue(NSUtilEditDateHeure* pControl)
{
	if (!pControl || !(pControl->HWindow))
		return false ;

	string sDate ;
	pControl->getDate(&sDate);
	if ((string("") != sDate) && (sDate != pControl->_sMask) )
		return true ;
	return false ;
}

bool
ObjectifViewerDlg::hasControlValue(NSUtilLexique* pControl)
{
	if (((NSUtilLexique*) NULL == pControl) || !(pControl->HWindow))
		return false ;

	if (pControl->getLabel() != string(""))
		return true ;
	return false ;
}


/*void
ObjectifViewerDlg::getEditDateControlTree(NSUtilEditDateHeure* pControl, string codControl,
                                    NSPatPathoArray* pPatho, int col)
{
    char far val[40] ;
	pControl->GetText(val, 40) ;
    string sVal;
    pControl->getDate(&sVal);
    pPatho->ajoutePatho(codControl, col, 0) ;
    col++ ;
    pPatho->ajoutePatho("KDAT01", col, 0) ;
    col++ ;
    Message *pCodeMsg = new Message("", "", "", "A", "", "", "") ;

    pCodeMsg->SetUnit("2DA021") ;
    pCodeMsg->SetComplement(sVal) ;
    pPatho->ajoutePatho("£D0;10", pCodeMsg, col, 1) ;
    delete  pCodeMsg ;

}    */


/*void
ObjectifViewerDlg::getTreeCyclicObjectif(NSPatPathoArray* pPatho, int col)
{
    //TO DO KOUVR1 and KFERM1
    pPatho->ajoutePatho("1CYCL1", col, 0) ;
    //add period
    if( hasControlValue(pPeriodIdealMin) || hasControlValue(pPeriodIdealMax) ||
        hasControlValue(pPeriodRecommendMin) || hasControlValue(pPeriodRecommendMax) ||
        hasControlValue(pPeriodAuthorizeMin) || hasControlValue(pPeriodAuthorizeMax))
    {
        pPatho->ajoutePatho("KMOD01", col, 0) ;
        getPeriodTree(pPatho, col+1) ;
    }
    //add value
     if( hasControlValue(pValueIdealMin) || hasControlValue(pValueIdealMax) ||
        hasControlValue(pValueRecommendMin) || hasControlValue(pValueRecommendMax) ||
        hasControlValue(pValueAuthorizeMin) || hasControlValue(pValueAuthorizeMax))
    {
        pPatho->ajoutePatho("0ENVV1", col, 0) ;
        getValueTree(pPatho, col+1) ;
    }
}

void
ObjectifViewerDlg::getTreePunctualObjectif(NSPatPathoArray* pPatho, int col)
{
    //TO DO KOUVR1 and KFERM1
   //add period
    if( hasControlValue(pDateIdealMin) || hasControlValue(pDateIdealMax) ||
        hasControlValue(pDateRecommendMin) || hasControlValue(pDateRecommendMax) ||
        hasControlValue(pDateAuthorizeMin) || hasControlValue(pDateAuthorizeMax))
    {
        pPatho->ajoutePatho("KMOD01", col, 0) ;
        getDateTree(pPatho, col+1) ;
    }
    //add value
     if( hasControlValue(pValueIdealMin) || hasControlValue(pValueIdealMax) ||
        hasControlValue(pValueRecommendMin) || hasControlValue(pValueRecommendMax) ||
        hasControlValue(pValueAuthorizeMin) || hasControlValue(pValueAuthorizeMax))
    {
        pPatho->ajoutePatho("0ENVV1", col, 0) ;
        getValueTree(pPatho, col+1) ;
    }
}

void
ObjectifViewerDlg::getTreePermanentObjectif(NSPatPathoArray* pPatho, int col)
{

    pPatho->ajoutePatho("KPERM1", col, 0) ;
    //add value
    if( hasControlValue(pValueIdealMin) || hasControlValue(pValueIdealMax) ||
        hasControlValue(pValueRecommendMin) || hasControlValue(pValueRecommendMax) ||
        hasControlValue(pValueAuthorizeMin) || hasControlValue(pValueAuthorizeMax))
    {
        pPatho->ajoutePatho("0ENVV1", col, 0) ;
        getValueTree(pPatho, col+1) ;
    }
}
 */

bool
ObjectifViewerDlg::getPeriodData()
{
  string sPeriodUnitCode = pPeriodUnit->getSelCode() ;
  if ((sPeriodUnitCode == "") &&
      ((hasControlValue(pPeriodIdealMin))     || (hasControlValue(pPeriodIdealMax))     ||
       (hasControlValue(pPeriodRecommendMin)) || (hasControlValue(pPeriodRecommendMax)) ||
       (hasControlValue(pPeriodAuthorizeMin)) || (hasControlValue(pPeriodAuthorizeMax))))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "mustUnitPeriod") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
  }

  if (hasControlValue(pPeriodIdealMin))
  {
    pPeriodIdealMin->donneValeur() ;
    pObjectif->_dDelaiDebutIdeal = pPeriodIdealMin->_dValeur ;
    pObjectif->_sUniteDebutIdeal = sPeriodUnitCode ;
  }
  else if ((pObjectif->_dDelaiDebutIdeal > 0) || (pObjectif->_sUniteDebutIdeal != ""))
  {
    pObjectif->_dDelaiDebutIdeal = 0 ;
    pObjectif->_sUniteDebutIdeal = string("") ;
  }

  if (hasControlValue(pPeriodIdealMax))
  {
    pPeriodIdealMax->donneValeur() ;
    pObjectif->_dDelaiDebutIdealMax = pPeriodIdealMax->_dValeur ;
    pObjectif->_sUniteDebutIdealMax = sPeriodUnitCode ;
  }
  else if ((pObjectif->_dDelaiDebutIdealMax > 0) || (pObjectif->_sUniteDebutIdealMax != ""))
  {
    pObjectif->_dDelaiDebutIdealMax = 0 ;
    pObjectif->_sUniteDebutIdealMax = string("") ;
  }

  if (hasControlValue(pPeriodRecommendMin))
  {
    pPeriodRecommendMin->donneValeur() ;
    pObjectif->_dDelaiDebutConseille = pPeriodRecommendMin->_dValeur ;
    pObjectif->_sUniteDebutConseille = sPeriodUnitCode ;
  }
  else if ((pObjectif->_dDelaiDebutConseille > 0) || (pObjectif->_sUniteDebutConseille != ""))
  {
    pObjectif->_dDelaiDebutConseille = 0 ;
    pObjectif->_sUniteDebutConseille = string("") ;
  }

  if (hasControlValue(pPeriodRecommendMax))
  {
    pPeriodRecommendMax->donneValeur() ;
    pObjectif->_dDelaiDebutConseilMax = pPeriodRecommendMax->_dValeur ;
    pObjectif->_sUniteDebutConseilMax = sPeriodUnitCode ;
  }
  else if ((pObjectif->_dDelaiDebutConseilMax > 0) || (pObjectif->_sUniteDebutConseilMax != ""))
  {
    pObjectif->_dDelaiDebutConseilMax = 0 ;
    pObjectif->_sUniteDebutConseilMax = string("") ;
  }

  if (hasControlValue(pPeriodAuthorizeMin))
  {
    pPeriodAuthorizeMin->donneValeur() ;
    pObjectif->_dDelaiDebutAutorise = pPeriodAuthorizeMin->_dValeur ;
    pObjectif->_sUniteDebutAutorise = sPeriodUnitCode ;
  }
  else if((pObjectif->_dDelaiDebutAutorise > 0) || (pObjectif->_sUniteDebutAutorise != ""))
  {
    pObjectif->_dDelaiDebutAutorise = 0 ;
    pObjectif->_sUniteDebutAutorise = string("") ;
  }

  if (hasControlValue(pPeriodAuthorizeMax))
  {
    pPeriodAuthorizeMax->donneValeur() ;
    pObjectif->_dDelaiDebutCritique = pPeriodAuthorizeMax->_dValeur ;
    pObjectif->_sUniteDebutCritique = sPeriodUnitCode ;
  }
  else if((pObjectif->_dDelaiDebutCritique > 0) || (pObjectif->_sUniteDebutCritique != ""))
  {
    pObjectif->_dDelaiDebutCritique = 0 ;
    pObjectif->_sUniteDebutCritique = string("") ;
  }

  return true ;
}

bool
ObjectifViewerDlg::getValueData()
{
  string sValueUnitCode = pUnitValue->getCode() ;

  if ((sValueUnitCode == "") &&
      ((hasControlValue(pValueIdealMin))     || (hasControlValue(pValueIdealMax)) ||
       (hasControlValue(pValueRecommendMin)) || (hasControlValue(pValueRecommendMax)) ||
       (hasControlValue(pValueAuthorizeMin)) || (hasControlValue(pValueAuthorizeMax)) ))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "mustUnitValue") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
  }

  if (hasControlValue(pValueIdealMin))
  {
    pValueIdealMin->donneValeur() ;
    pObjectif->_dValMinIdeal      = pValueIdealMin->_dValeur ;
    pObjectif->_sUniteValMinIdeal = sValueUnitCode ;
    pObjectif->_bValMinIdeal      = true ;
  }
  else if (pObjectif->_bValMinIdeal)
  {
    pObjectif->_dValMinIdeal      = 0 ;
    pObjectif->_sUniteValMinIdeal = string("") ;
    pObjectif->_bValMinIdeal      = false ;
  }

  if (hasControlValue(pValueIdealMax))
  {
    pValueIdealMax->donneValeur();
    pObjectif->_dValMaxIdeal      = pValueIdealMax->_dValeur ;
    pObjectif->_sUniteValMaxIdeal = sValueUnitCode ;
    pObjectif->_bValMaxIdeal      = true ;
  }
  else if (pObjectif->_bValMaxIdeal)
  {
    pObjectif->_dValMaxIdeal      = 0 ;
    pObjectif->_sUniteValMaxIdeal = string("") ;
    pObjectif->_bValMaxIdeal      = false ;
  }

  if (hasControlValue(pValueRecommendMin))
  {
    pValueRecommendMin->donneValeur();
    pObjectif->_dValMinConseille      = pValueRecommendMin->_dValeur ;
    pObjectif->_sUniteValMinConseille = sValueUnitCode ;
    pObjectif->_bValMinConseille      = true ;
  }
  else if (pObjectif->_bValMinConseille)
  {
    pObjectif->_dValMinConseille      = 0 ;
    pObjectif->_sUniteValMinConseille = string("") ;
    pObjectif->_bValMinConseille      = false ;
  }

  if (hasControlValue(pValueRecommendMax))
  {
    pValueRecommendMin->donneValeur();
    pObjectif->_dValMaxConseille      = pValueRecommendMax->_dValeur ;
    pObjectif->_sUniteValMaxConseille = sValueUnitCode ;
    pObjectif->_bValMaxConseille      = true ;
  }
  else if (pObjectif->_bValMaxConseille)
  {
    pObjectif->_dValMaxConseille      = 0 ;
    pObjectif->_sUniteValMaxConseille = string("") ;
    pObjectif->_bValMaxConseille      = false ;
  }

  if (hasControlValue(pValueAuthorizeMin))
  {
    pValueAuthorizeMin->donneValeur() ;
    pObjectif->_dValMinAutorise      = pValueAuthorizeMin->_dValeur ;
    pObjectif->_sUniteValMinAutorise = sValueUnitCode ;
    pObjectif->_bValMinAutorise      = true ;
  }
  else if (pObjectif->_bValMinAutorise)
  {
    pObjectif->_dValMinAutorise      = 0 ;
    pObjectif->_sUniteValMinAutorise = string("") ;
    pObjectif->_bValMinAutorise      = false ;
  }

  if (hasControlValue(pValueAuthorizeMax))
  {
    pValueAuthorizeMax->donneValeur() ;
    pObjectif->_dValMaxAutorise      = pValueAuthorizeMax->_dValeur ;
    pObjectif->_sUniteValMaxAutorise = sValueUnitCode ;
    pObjectif->_bValMaxAutorise      = true ;
  }
  else if (pObjectif->_bValMaxAutorise)
  {
    pObjectif->_dValMaxAutorise      = 0 ;
    pObjectif->_sUniteValMaxAutorise = string("") ;
    pObjectif->_bValMaxAutorise      = false ;
  }

  return true ;
}


void
ObjectifViewerDlg::getDateData()
{
  string sDate ;
  if (hasControlValue(pDateIdealMin))
  {
    pDateIdealMin->getDate(&sDate);
    pObjectif->_sDateDebutIdeal = sDate ;//completeData(sDate, pDateIdealMin);
  }
  else if (pObjectif->_sDateDebutIdeal != "")
    pObjectif->_sDateDebutIdeal = "";

  if (hasControlValue(pDateIdealMax))
  {
    pDateIdealMax->getDate(&sDate);
    pObjectif->_sDateDebutIdealMax  = sDate ; //completeData(sDate, pDateIdealMax);
  }
  else if (pObjectif->_sDateDebutIdealMax != "")
    pObjectif->_sDateDebutIdealMax = "";

  if (hasControlValue(pDateRecommendMin))
  {
    pDateRecommendMin->getDate(&sDate);
    pObjectif->_sDateDebutConseille = sDate ;//completeData(sDate, pDateRecommendMin);
  }
  else if (pObjectif->_sDateDebutConseille != "")
    pObjectif->_sDateDebutConseille = "";

  if (hasControlValue(pDateRecommendMax))
  {
    pDateRecommendMax->getDate(&sDate);
    pObjectif->_sDateDebutConseilMax = sDate ; //completeData(sDate, pDateRecommendMax);
  }
  else if (pObjectif->_sDateDebutConseilMax != "")
    pObjectif->_sDateDebutConseilMax = "";

  if (hasControlValue(pDateAuthorizeMin))
  {
    pDateAuthorizeMin->getDate(&sDate);
    pObjectif->_sDateDebutAutorise = sDate ; //completeData(sDate, pDateAuthorizeMin);
  }
  else if (pObjectif->_sDateDebutAutorise != "")
    pObjectif->_sDateDebutAutorise = "";

  if (hasControlValue(pDateAuthorizeMax))
  {
    pDateAuthorizeMax->getDate(&sDate);
    pObjectif->_sDateDebutCritique = sDate ; //completeData(sDate, pDateAuthorizeMax);
  }
  else if (pObjectif->_sDateDebutCritique != "")
    pObjectif->_sDateDebutCritique = "";
}

void
ObjectifViewerDlg::getDataOpenMoment()
{
  if (hasControlValue(pStartDate))
  {
    string sVal ;
    pStartDate->getDate(&sVal) ;
    pObjectif->_tDateOuverture.initFromDate(sVal) ;
  }
  else
    if (false == pObjectif->_tDateOuverture.estVide())
      pObjectif->_tDateOuverture.init() ;
    else
      //apres un eveniment
      if (hasControlValue(pStartEveniment))
        pObjectif->_sOpenEventNode = pStartEveniment->getCode() ;
      else
        if (pObjectif->_sOpenEventNode != "")
          pObjectif->_sOpenEventNode = "" ;
}

void
ObjectifViewerDlg::getDataCloseMoment()
{
  if (hasControlValue(pEndDate))
  {
    string sVal ;
    pEndDate->getDate(&sVal) ;
    pObjectif->_tDateFermeture.initFromDate(sVal) ;
  }
  else
    //apres un eveniment
    if (hasControlValue(pEndEveniment))
    {
      pObjectif->_sCloseEventNode = pEndEveniment->getCode() ;
    }
    else
      pObjectif->_tDateFermeture.setNoLimit() ;
}

/*void
ObjectifViewerDlg::getValueTree (NSPatPathoArray* pPatho, int iChildCol)
{
    string sValueUnitCode   = pUnitValue->getCode();
    if(hasControlValue(pValueIdealMin))
        getEditNumControlTree(pValueIdealMin, "VMIN01", pPatho, iChildCol, sValueUnitCode) ;
    if(hasControlValue(pValueIdealMax))
        getEditNumControlTree(pValueIdealMin, "VMAX01", pPatho, iChildCol, sValueUnitCode) ;
    if(hasControlValue(pValueRecommendMin))
        getEditNumControlTree(pValueRecommendMin, "VMIN11", pPatho, iChildCol, sValueUnitCode) ;
    if(hasControlValue(pValueRecommendMax))
        getEditNumControlTree(pValueRecommendMax, "VMAX11", pPatho, iChildCol, sValueUnitCode) ;
    if(hasControlValue(pValueAuthorizeMin))
        getEditNumControlTree(pValueAuthorizeMin, "VMIN21", pPatho, iChildCol, sValueUnitCode) ;
    if(hasControlValue(pValueAuthorizeMax))
        getEditNumControlTree(pValueAuthorizeMax, "VMAX21", pPatho, iChildCol, sValueUnitCode) ;
} */


/*void
ObjectifViewerDlg::getDateTree(NSPatPathoArray* pPatho, int colonne)
{
    int iChildCol = colonne + 1;

    if(hasControlValue(pDateIdealMin))
        getEditDateControlTree(pDateIdealMin, "KMOD11", pPatho, iChildCol) ;
    if(hasControlValue(pDateIdealMax))
        getEditDateControlTree(pDateIdealMin, "KMOD21", pPatho, iChildCol) ;
    if(hasControlValue(pDateRecommendMin))
        getEditDateControlTree(pDateRecommendMin, "KMOD31", pPatho, iChildCol) ;
    if(hasControlValue(pDateRecommendMax))
        getEditDateControlTree(pDateRecommendMax, "KMOD41", pPatho, iChildCol) ;
    if(hasControlValue(pDateAuthorizeMin))
        getEditDateControlTree(pDateAuthorizeMin, "KMOD51", pPatho, iChildCol) ;
    if(hasControlValue(pDateAuthorizeMax))
        getEditDateControlTree(pDateAuthorizeMax, "KMOD61", pPatho, iChildCol) ;
}  */


/*void
ObjectifViewerDlg::getTreeOpenMoment(NSPatPathoArray* pPatho, int colonne)
{
    pPatho->ajoutePatho("KOUVR1", colonne, 0) ;
    //date de debut
    if(hasControlValue(pStartDate))
    {
        string sVal;
        pStartDate->getDate(&sVal);
        Message *pCodeMsg = new Message("", "", "", "A", "", "", "") ;
        pCodeMsg->SetUnit("2DA021") ;
        pCodeMsg->SetComplement(sVal) ;
        pPatho->ajoutePatho("£D0;10", pCodeMsg, colonne++, 1) ;
        delete  pCodeMsg;
    }
    else
        //apres un eveniment
        if(hasControlValue(pStartEveniment))
        {
             pPatho->ajoutePatho("KEVOU1", colonne++, 1) ;
             pPatho->ajoutePatho(pStartEveniment->getCode(), colonne++, 1) ;
        }
} */

/*void
ObjectifViewerDlg::getTreeCloseMoment(NSPatPathoArray* pPatho, int colonne)
{

    //date de debut
    if(hasControlValue(pEndDate))
    {
        pPatho->ajoutePatho("KFERM1", colonne, 0) ;
        string sVal;
        pEndDate->getDate(&sVal);
        Message *pCodeMsg = new Message("", "", "", "A", "", "", "") ;
        pCodeMsg->SetUnit("2DA021") ;
        pCodeMsg->SetComplement(sVal) ;
        pPatho->ajoutePatho("£D0;10", pCodeMsg, colonne++, 1) ;
        delete  pCodeMsg;
    }
    else
        //apres un eveniment
        if(hasControlValue(pStartEveniment))
        {
             pPatho->ajoutePatho("KFERM1", colonne, 0) ;
             pPatho->ajoutePatho("KEVFE1", colonne++, 1) ;
             pPatho->ajoutePatho(pStartEveniment->getCode(), colonne++, 1) ;
        }
}   */

void
ObjectifViewerDlg::CmOk()
{
  pObjectif->setLexique(pObjectifLexicon->getCode()) ;

  if (pPresence->GetCheck() == BF_CHECKED)
    pObjectif->_sCertitude = "" ;
  else if (pAbsence->GetCheck() == BF_CHECKED)
    pObjectif->_sCertitude = "WCE001" ;

  char far val[200] ;
	pComplement->GetText(val, 200) ;
  if(val && (string(val) != ""))
  {
    pObjectif->_sComplementText = string(val) ;
  }

  getDataOpenMoment() ;
  switch (objectifType)
  {
    case ONE_OBJECTIF :
      //traitment one obj
      if (!getValueData())
        return ;
      getDateData() ;
      pObjectif->_iRythme = NSLdvGoal::ponctuel ;
      break ;

    case CYCLE_OBJECTIF :      if (!getPeriodData())        return ;      if (!getValueData())        return ;      pObjectif->_iRythme = NSLdvGoal::cyclic ;      break ;
    case PERMANENT_OBJECTIF :
      if (!getValueData())
        return ;
      pObjectif->_iRythme = NSLdvGoal::permanent ;
      break ;
  }

  getDataCloseMoment() ;
  TDialog::CmOk() ;
}

void
ObjectifViewerDlg::CmCancel()
{
  TDialog::CmCancel() ;
}

void
ObjectifViewerDlg::EvClose()
{
  TDialog::EvClose() ;
}

void
ObjectifViewerDlg::EvPaint()
{
  TPaintDC Dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&Dc.Ps.rcPaint ;
  TDialog::Paint(Dc, false, rect) ;
  Paint(Dc, false, rect) ;
}

void
ObjectifViewerDlg::Paint(TDC& pDc, bool /* erase */, NS_CLASSLIB::TRect& /* RectAPeindre */)
{
  int iAncDC = pDc.SaveDC() ;
  // bool paintLeft = false ;
  // bool paintRight = false ;

  NS_CLASSLIB::TPoint ptTopLeft, ptBottomRight ;
  //centre
  ptTopLeft.x = 229 ;
  ptTopLeft.y = 232 ;
  ptBottomRight.x = 251 ;
  ptBottomRight.y = 256 ;
  initColorControlRect(&pDc, ptTopLeft, ptBottomRight ) ;
  //color utils
  // TColor*  pCurrentColor   = &NS_CLASSLIB::TColor::LtRed ;
  // TColor*  pCentreColor ;

  switch (objectifType)
  {
        case ONE_OBJECTIF :      //date + value
            ptTopLeft.x = 40 ;
            ptTopLeft.y = 400 ;
            ptBottomRight.x = 60 ;
            ptBottomRight.y = 400 ;
            repaintColorBar(&pDc, arrayLeftDateControls, ptTopLeft, ptBottomRight);

            ptTopLeft.x = 420;
            ptTopLeft.y = 400;
            ptBottomRight.x = 440 ;
            ptBottomRight.y = 400 ;
            repaintColorBar(&pDc, arrayRightNumControls, ptTopLeft, ptBottomRight) ;
            break;

        case CYCLE_OBJECTIF :      //period + value            ptTopLeft.x = 40 ;            ptTopLeft.y = 400 ;
            ptBottomRight.x = 60 ;
            ptBottomRight.y = 400 ;
            repaintColorBar(&pDc, arrayLeftNumControls, ptTopLeft, ptBottomRight) ;            ptTopLeft.x = 420 ;            ptTopLeft.y = 400 ;
            ptBottomRight.x = 440 ;
            ptBottomRight.y = 400 ;
            repaintColorBar(&pDc, arrayRightNumControls, ptTopLeft, ptBottomRight) ;            break;
        case PERMANENT_OBJECTIF :   //only value
            ptTopLeft.x = 420 ;
            ptTopLeft.y = 400 ;
            ptBottomRight.x = 440 ;
            ptBottomRight.y = 400 ;
            repaintColorBar(&pDc, arrayRightNumControls, ptTopLeft, ptBottomRight) ;
            break;
	}
	pDc.RestoreDC(iAncDC) ;
}

void
ObjectifViewerDlg::repaintColorBar(TDC* pDc, NSObjectifEditNum** arrayControls, NS_CLASSLIB::TPoint ptInitTopLeft, NS_CLASSLIB::TPoint ptInitBottomRight)
{
	if (!arrayControls || !pDc)
		return ;

	TColor*  pCurrentColor   = &NS_CLASSLIB::TColor::LtRed ;
	TColor*  pCentreColor ;
	NS_CLASSLIB::TPoint ptTopLeft, ptBottomRight ;
	ptTopLeft.x = ptInitTopLeft.x ;
	ptBottomRight.x = ptInitBottomRight.x ;

  // The only possible algoritm is to define the center color (hearth)
	// then move up from there, using existing values as color transitions
  // then start again from the center and go down (in the same way we went up)

  //
  // looking for center color
	// il faut trouver la couleur du coeur
  //
	if ((hasControlValue(arrayControls[2])) || (hasControlValue(arrayControls[3])))
		pCentreColor = &NS_CLASSLIB::TColor::LtBlue ;
	else
		if ((hasControlValue(arrayControls[1])) || (hasControlValue(arrayControls[4])))
			pCentreColor = &NS_CLASSLIB::TColor::LtGreen ;
		else
		{
			if ((hasControlValue(arrayControls[0])) || (hasControlValue(arrayControls[5])))
				pCentreColor   = &NS_CLASSLIB::TColor::LtYellow ;
			else
				return ;
		}

	ColorRect *allRect[7] ;
	for (int i = 0 ; i < 7 ; i++)
		allRect[i] = NULL ;

	// set heart's color
	//
	ptTopLeft.y = ptInitTopLeft.y - 4*24;
	ptBottomRight.y = ptInitBottomRight.y - 24*3;
	allRect[3] = new ColorRect(ptTopLeft, ptBottomRight, pCentreColor);
  //
	// all rects towards top
  //
	pCurrentColor = pCentreColor;
	for (int i = 3 ; i < 6 ; i++)
	{
		ptTopLeft.y = ptInitTopLeft.y - 24*(i+2);
		ptBottomRight.y = ptInitBottomRight.y - 24*(i+1);
		if (hasControlValue(arrayControls[i]))
			pCurrentColor = arrayControls[i]->brushColorForward;
		allRect[i+1] = new ColorRect(ptTopLeft, ptBottomRight, pCurrentColor) ;
	}
	//
	// all rects towards bottom
	//
	pCurrentColor = pCentreColor;
	for (int i = 2 ; i > -1 ; i--)
	{
		ptTopLeft.y = ptInitTopLeft.y - 24*(i+1) ;
		ptBottomRight.y = ptInitBottomRight.y - 24*i;
		if (hasControlValue(arrayControls[i]))
			pCurrentColor = arrayControls[i]->brushColorBack;
		allRect[i] = new ColorRect(ptTopLeft, ptBottomRight, pCurrentColor);
	}

	for (int i = 0; i < 7 ; i++)
	{
		allRect[i]->RectFillWithDefaultColor(pDc) ;
    delete allRect[i] ;
	}
}


void
ObjectifViewerDlg::repaintColorBar(TDC* pDc, NSObjectifEditDateHeure** arrayControls,
                    NS_CLASSLIB::TPoint ptInitTopLeft, NS_CLASSLIB::TPoint ptInitBottomRight)
{
    if(!arrayControls)
        return;
    NS_CLASSLIB::TPoint ptTopLeft, ptBottomRight ;
    ptTopLeft.x = ptInitTopLeft.x ;
    ptBottomRight.x = ptInitBottomRight.x ;

    TColor*  pCurrentColor   = &NS_CLASSLIB::TColor::LtRed ;
    TColor*  pCentreColor ;


    //il faut trouver la couleur du coeur
    if( (hasControlValue(arrayControls[2])) || (hasControlValue(arrayControls[3])) )
    {
        pCentreColor   = &NS_CLASSLIB::TColor::LtBlue ;
    }
    else
        if( (hasControlValue(arrayControls[1])) || (hasControlValue(arrayControls[4])) )
        {
            pCentreColor   = &NS_CLASSLIB::TColor::LtGreen ;
        }
        else
        {
            if( (hasControlValue(arrayControls[0])) || (hasControlValue(arrayControls[5])) )
                pCentreColor   = &NS_CLASSLIB::TColor::LtYellow ;
            else
                return;
        }
    //set the centre
    ColorRect *allRect[7];
    ptTopLeft.y = ptInitTopLeft.y - 4*24;
    ptBottomRight.y = ptInitBottomRight.y - 24*3;
    allRect[3] = new ColorRect(ptTopLeft, ptBottomRight, pCentreColor);
    //all rect towards top
    pCurrentColor = pCentreColor;
    for(int i=3; i<6;i++)
    {
        ptTopLeft.y = ptInitTopLeft.y - 24*(i+2);
        ptBottomRight.y = ptInitBottomRight.y - 24*(i+1);
        if(hasControlValue(arrayControls[i]))
        {
            pCurrentColor = arrayControls[i]->brushColorForward;
        }
        allRect[i+1] = new ColorRect(ptTopLeft, ptBottomRight, pCurrentColor);
    }
    //all rect towards bottom
    pCurrentColor = pCentreColor;
    for(int i=2; i>-1;i--)
    {
        ptTopLeft.y = ptInitTopLeft.y - 24*(i+1);
        ptBottomRight.y = ptInitBottomRight.y - 24*i;
        if(hasControlValue(arrayControls[i]))
        {
            pCurrentColor = arrayControls[i]->brushColorBack;
        }
        allRect[i] = new ColorRect(ptTopLeft, ptBottomRight, pCurrentColor);
    }
    for(int i=0; i<7; i++)
    {
        allRect[i]->RectFillWithDefaultColor(pDc );
        delete allRect[i] ;
    }

}

/*bool
ObjectifViewerDlg::testDraw(NSObjectifEditNum*  arrayControls[6])
{
    bool  blueHeartMin      = false;
    bool  blueHeartMax      = false;
    bool  greenHeartMin     = false;
    bool  greenHeartMax     = false;
    bool  yellowHeartMin    = false;
    bool  yellowHeartMax    = false;

    for(int i=0; i<6; i++)
    {
        char far val[40] ;
	    arrayControls[i]->GetText(val, 40) ;
        if(string(val) != "")
        {
            switch(i)
            {
                case 0  :
                    yellowHeartMin = true ;
                    break;
                case 1  :
                    greenHeartMin = true ;
                    break;
                case 2  :
                    blueHeartMin = true ;
                    break;
                case 3  :
                    blueHeartMax = true ;
                    break;
                case 4  :
                    greenHeartMax = true ;
                    break;
                case 5  :
                    yellowHeartMax = true ;
                    break;
            }
        }
    }
    bool res =((yellowHeartMin && yellowHeartMax) || (greenHeartMax && greenHeartMin) ||
                    (blueHeartMax && blueHeartMin)) ;
    return  res;
}

bool
ObjectifViewerDlg::testDraw(NSObjectifEditDateHeure**  arrayControls)
{
    bool  blueHeartMin      = false;
    bool  blueHeartMax      = false;
    bool  greenHeartMin     = false;
    bool  greenHeartMax     = false;
    bool  yellowHeartMin    = false;
    bool  yellowHeartMax    = false;

    for(int i=0; i<6; i++)
    {
        string sVal = "";
        arrayControls[i]->getDate(&sVal) ;
        if(sVal != "00/00/0000 00:00:00")
        {
            switch(i)
            {
                case 0  :
                    yellowHeartMin = true ;
                    break;
                case 1  :
                    greenHeartMin = true ;
                    break;
                case 2  :
                    blueHeartMin = true ;
                    break;
                case 3  :
                    blueHeartMax = true ;
                    break;
                case 4  :
                    greenHeartMax = true ;
                    break;
                case 5  :
                    yellowHeartMax = true ;
                    break;
            }
        }
    }
    return  ((yellowHeartMin && yellowHeartMax) || (greenHeartMax && greenHeartMin) ||
                    (blueHeartMax && blueHeartMin)) ;
}  */

void
ObjectifViewerDlg::initColorControlRect( TDC *pDc, NS_CLASSLIB::TPoint ptTopLeft, NS_CLASSLIB::TPoint ptBottomRight )
{
    ColorRect* pColorRect = new ColorRect(ptTopLeft, ptBottomRight);
    for(int i=1; i<=7; i++)
    {
        if((i==1)||(i==7))
            pColorRect->RectFill(pDc, NS_CLASSLIB::TColor::LtRed) ;
        else
            if((i==2)||(i==6))
                pColorRect->RectFill(pDc, NS_CLASSLIB::TColor::LtYellow) ;
            else
                if((i==3)||(i==5))
                    pColorRect->RectFill(pDc, NS_CLASSLIB::TColor::LtGreen) ;
                else
                    pColorRect->RectFill(pDc, NS_CLASSLIB::TColor::LtBlue) ;

        ptTopLeft.y = ptBottomRight.y ;
        ptBottomRight.y =  ptBottomRight.y + 24;
        pColorRect->SetRect( ptTopLeft, ptBottomRight);
    }
    delete  pColorRect;
}

//on paint in pCurrentColor tous les rectangles depuis lastPaintedRect
//jusqu'a current rectangle
//rect = 0(centre), 1(gauche), 2(droit)
void
ObjectifViewerDlg::ColorControlRect(int rect, NSObjectifEditNum *lastActivEdit,
                                TColor* pCurrentColor, int lastPaintedRect)
{
    NS_CLASSLIB::TPoint ptTopLeft, ptBottomRight ;

    ptTopLeft.y = 343 ;
    ptBottomRight.y = 343 - 24 ;
    switch( rect )
    {
        case 1 :
                ptTopLeft.x = 20;
                ptBottomRight.x = 38 ;
                break;
        case 2 :
                ptTopLeft.x = 360;
                ptBottomRight.x = 378 ;
                break;

    }
    TWindowDC* pWinDC = new TWindowDC(HWindow); // HWnd

    int listPos = lastActivEdit->posInObjectControlList ;
    for(int i = lastPaintedRect ; i<=listPos; i++)
    {
        ptBottomRight.y = 343 - 24*i ;
        TBrush* pBrush = new TBrush(*pCurrentColor) ;
        pWinDC->FillRect(NS_CLASSLIB::TRect(ptTopLeft, ptBottomRight), *pBrush) ;
        delete pBrush;
    }

    delete pWinDC;
    
}


ColorRect::ColorRect( NS_CLASSLIB::TPoint topLeft, NS_CLASSLIB::TPoint bottomRight,  NS_CLASSLIB::TColor* color )
{
    ptTopLeft = topLeft;
    ptBottomRight = bottomRight;
    defaultColor = color;
}

ColorRect::ColorRect( NS_CLASSLIB::TPoint topLeft, NS_CLASSLIB::TPoint bottomRight )
{
    ptTopLeft = topLeft;
    ptBottomRight = bottomRight;
}

void
ColorRect::SetRect( NS_CLASSLIB::TPoint topLeft, NS_CLASSLIB::TPoint bottomRight )
{
    ptTopLeft = topLeft;
    ptBottomRight = bottomRight;
}

void
ColorRect::RectFill(TDC *pDc, const  NS_CLASSLIB::TColor& color)
{
     TBrush* pBrush = new TBrush(color) ;
     pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptBottomRight.x, ptBottomRight.y, *pBrush);
     delete pBrush;
}

void
ColorRect::RectFillWithDefaultColor(TDC *pDc)
{
    TBrush* pBrush = new TBrush(*defaultColor) ;
    pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptBottomRight.x, ptBottomRight.y, *pBrush);
    delete pBrush;
}

//------------------------------------------------------------------------------
//                          NSObjectifEditNum
//------------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSObjectifEditNum, NSEditNum)
	EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

NSObjectifEditNum::NSObjectifEditNum(NSContexte *pCtx, ObjectifViewerDlg* pUtilDialog, int resId, int listPos,
                 NS_CLASSLIB::TColor* forwardColor,  NS_CLASSLIB::TColor* backColor, int iTextLen, int iDecimales, string sValidator)
                  :NSEditNum(pCtx, pUtilDialog, resId, iTextLen, iDecimales, sValidator)
{
    pParent = pUtilDialog ;
    iValue = 0 ;
    brushColorForward = forwardColor ;
    brushColorBack = backColor ;
    posInObjectControlList = listPos ;
}


NSObjectifEditNum::~NSObjectifEditNum()
{

}

void
NSObjectifEditNum::EvKillFocus(HWND hWndGetFocus)
{
	NSEditNum::EvKillFocus(hWndGetFocus);
    pParent->TWindow::Invalidate();
}

void
NSObjectifEditNum::SetIntIntoEditNum(int iVal)
{
	char sVal[10] ;
	itoa(iVal, sVal, 10) ;
	_sContenuTransfert = string (sVal) ;
	donneBrut() ;
	SetText(_sContenuBrut.c_str()) ;
}

void
NSObjectifEditNum::SetDoubleIntoEditNum(double dVal)
{
	_sContenuTransfert = DoubleToString(&dVal, -1, -1) ;
	donneBrut() ;
	SetText(_sContenuBrut.c_str()) ;
}

//------------------------------------------------------------------------------
//                          NSObjectifEditDateHeure
//------------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSObjectifEditDateHeure, NSUtilEditDateHeure)
	EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

NSObjectifEditDateHeure::NSObjectifEditDateHeure(NSContexte *pCtx, ObjectifViewerDlg* pNSUtilDialog, int resId,
                 NS_CLASSLIB::TColor* forwardColor,  NS_CLASSLIB::TColor* backColor, UINT iTextLen, bool b2000)
                        :NSUtilEditDateHeure(pCtx, pNSUtilDialog, resId, iTextLen, b2000)
{
  pParent = pNSUtilDialog ;
  brushColorForward = forwardColor ;
  brushColorBack    = backColor ;
}


NSObjectifEditDateHeure::~NSObjectifEditDateHeure()
{
}

void
NSObjectifEditDateHeure::EvKillFocus(HWND hWndGetFocus)
{
	TEdit::EvKillFocus(hWndGetFocus) ;
  pParent->TWindow::Invalidate() ;
}

