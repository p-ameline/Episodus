// -----------------------------------------------------------------------------
// nsattval_glif.cpp
// -----------------------------------------------------------------------------
// Attributs/Valeurs d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Author: pameline $
// $Date: 2013/11/11 22:35:28 $
// -----------------------------------------------------------------------------
// FLP - april 2005
// FLP - july/august 2003
// RS  - july 2003
// FLP - june 2003
// RS  - november 2002// -----------------------------------------------------------------------------

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

#include <bwcc.h>
#include <commctrl.h>

#include "nsbb\nsattval.h"#include "nsbb\nsattval_glif.h"#include "nsbb\nsarcParseError.h"#include "nsbb\nsarc.h"#include "nsbb\nspatpat.h"#ifdef _ENTERPRISE_DLL  #include "enterpriseLus\superLus.h"
  #include "..\episodus\nsbb\nsbb.rh"
#else
  #include "nautilus\nssuper.h"
  #include "nautilus\nsepicap.h"
  #include "nsbb\nsbb.rh"
#endif#include "ns_ob1\Interface.h"//#include "nsbb\nsbbtran.h"// -----------------------------------------------------------------------------//                             Csteps class// -----------------------------------------------------------------------------Csteps::Csteps(string attributs, string values, Cbalise *father, NSSuper* pSuper)       :Cbalise(attributs, values, father)
{
  _pSuper = pSuper ;
}

Csteps::~Csteps()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      CdecisionStep *pCDecis = dynamic_cast<CdecisionStep*>(pBalise) ;
      if (pCDecis)
      {
        delete pCDecis ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        CactionStep *pCAction = dynamic_cast<CactionStep*>(pBalise) ;
        if (pCAction)
        {
          delete pCAction ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          CbranchStep *pCBrch = dynamic_cast<CbranchStep*>(pBalise) ;
          if (pCBrch)
          {
            delete pCBrch ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
          else
          {
            CsynchronizationStep *pCSynch = dynamic_cast<CsynchronizationStep*>(pBalise) ;
            if (pCSynch)
            {
              delete pCSynch ;
              (*ival)->setObject((Cbalise*) 0) ;
            }
            else
            {
              CpatientStateStep *pCState = dynamic_cast<CpatientStateStep*>(pBalise) ;
              if (pCState)
              {
                delete pCState ;
                (*ival)->setObject((Cbalise*) 0) ;
              }
              else
              {
                Creferences *pCRef = dynamic_cast<Creferences*>(pBalise) ;
                if (pCRef)
                {
                  delete pCRef ;
                  (*ival)->setObject((Cbalise*) 0) ;
                }
                else
                {
                  Cdialogbox *pCDlg = dynamic_cast<Cdialogbox*>(pBalise) ;
                  if (pCDlg)
                  {
                    delete pCDlg ;
                    (*ival)->setObject((Cbalise*) 0) ;
                  }
                }
              }
            }
          }
        }
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

Csteps::Csteps(const Csteps& rv)
       :Cbalise(rv)
{
  _pSuper = rv._pSuper ;
}

Csteps&
Csteps::operator=(const Csteps& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this;
	Cbalise* pbdata2 = (Csteps*)&src;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2;

	_pSuper = src._pSuper ;

  return (*this) ;
}

bool
Csteps::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EGUIDELINE_VAL_EMPTY ;
    return false ;
  }

  int nb_decision  = 0 ;
  int nb_action    = 0 ;
  int nb_branch    = 0 ;
  int nb_synchro   = 0 ;
  int nb_patState  = 0 ;
  int nb_dialogbox = 0 ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_STEP_DECISION == (*ival)->getLabel())
		{
      (*ival)->setObject(new CdecisionStep((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_decision++ ;
		}
    else if (LABEL_STEP_ACTION == (*ival)->getLabel()) // dialogue
		{
      (*ival)->setObject(new CactionStep((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_action++ ;
		}
    else if (LABEL_STEP_BRANCH == (*ival)->getLabel()) // modèle html de présentation
		{
      (*ival)->setObject(new CbranchStep((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_branch++ ;
		}
    else if (LABEL_STEP_SYNCHRO == (*ival)->getLabel()) // references
    {
      (*ival)->setObject(new CsynchronizationStep((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
      nb_synchro++ ;
    }
    else if (LABEL_STEP_PATIENT_STATE == (*ival)->getLabel()) // items
    {
      (*ival)->setObject(new CpatientStateStep((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_patState++ ;
    }
    else if (LABEL_REFERENCES == (*ival)->getLabel()) // references
    {
      (*ival)->setObject(new Creferences((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
    }
    else if (LABEL_DIALOGBOX == (*ival)->getLabel()) // dialogue
		{
      (*ival)->setObject(new Cdialogbox((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_dialogbox++ ;
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EGUIDELINE_VAL ;
      return false ;
    }
  }

/*
	if ((nb_dialogue > 1) || (nb_dialogbox > 1) || (nb_items != 1) || (nb_contraintes > 1) || (nb_present > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
*/
	return true ;
}boolCsteps::verif(){
  return true ;
}

bool
Csteps::compresser()
{
  return true ;
}

bool
Csteps::traiter()
{
  return true ;
}

CguidelineStep *
Csteps::getGuidelineStepById(string sId)
{
  if (vect_val.empty())
    return (CguidelineStep *) 0 ;

  for (ValIter ival = vect_val.begin() ; ival != vect_val.end() ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_STEPS == (*ival)->getLabel())
    {
      CguidelineStep *pStep = dynamic_cast<CguidelineStep *>((*ival)->getObject()) ;
      if (pStep && (pStep->getStepId() == sId))
        return pStep ;
    }
  }
  return (CguidelineStep *) 0 ;
}

// -----------------------------------------------------------------------------
//                           CguidelineStep class// -----------------------------------------------------------------------------
CguidelineStep::CguidelineStep(string attributs, string values, Cbalise *father)
               :Cbalise(attributs, values, father)
{
}

CguidelineStep::~CguidelineStep()
{
}

string
CguidelineStep::getStepId()
{
  return getStringAttribute(ATTRIBUT_STEP_ID) ;
}

string
CguidelineStep::getTitle(string sLang)
{
  Chead* pHead = getHead(sLang) ;
  if (pHead)
    return pHead->getTitle() ;

  return string("") ;
}

Chead *
CguidelineStep::getHead(string sLang)
{
  Chead *pDefRef = NULL ;
  Chead *pRef    = NULL ;

  if (vect_val.empty())
  	return pRef ;

  for (ValIter ival = vect_val.begin() ; ival != vect_val.end() ; ival++)
  {
  	// on récupère l'objet dialogue
    if (LABEL_HEAD == (*ival)->getLabel())
    {
    	pRef = dynamic_cast<Chead *>((*ival)->getObject()) ;
      if (pRef)
      {
      	if (pRef->getLang() == sLang)
        	break ;
        if (pRef->getLang() == "")
        	pDefRef = pRef ;
      }
    }
  }
  if (pRef && (pRef->getLang() == sLang))
  	return pRef ;
  else
  	return pDefRef ;
}

// -----------------------------------------------------------------------------
//                           CdecisionStep class// -----------------------------------------------------------------------------
CdecisionStep::CdecisionStep(string attributs, string values, Cbalise *father)
              :CguidelineStep(attributs, values, father)
{
}

CdecisionStep::~CdecisionStep()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      CdecisionStepOption *pCDecis = dynamic_cast<CdecisionStepOption*>(pBalise) ;
      if (pCDecis)
      {
        delete pCDecis ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Chead *pCHead = dynamic_cast<Chead*>(pBalise) ;
        if (pCHead)
        {
          delete pCHead ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

CdecisionStep::CdecisionStep(const CdecisionStep& rv)
              :CguidelineStep(rv)
{
}

CdecisionStep&
CdecisionStep::operator=(const CdecisionStep& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this;
	Cbalise* pbdata2 = (CdecisionStep*)&src;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2;

  return (*this) ;
}

bool
CdecisionStep::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (getStringAttribute(ATTRIBUT_STEP_ID) == string(""))
  {
    // erreur (pas de nom)
    _iParsingError = EDECISION_STEP_NO_ID ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

  int nb_option = 0 ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_STEP_DECISION_OPTION == (*ival)->getLabel())
		{
      (*ival)->setObject(new CdecisionStepOption((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_option++ ;
		}
    else if (LABEL_HEAD == (*ival)->getLabel())
		{
      (*ival)->setObject(new Chead((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

/*
	if ((nb_dialogue > 1)  || (nb_dialogbox > 1) || (nb_items != 1) || (nb_contraintes > 1) || (nb_present > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
*/
	return true ;
}boolCdecisionStep::verif(){
  return true ;
}

bool
CdecisionStep::compresser()
{
  return true ;
}

bool
CdecisionStep::traiter()
{
  return true ;
}

CdecisionStepOption *
CdecisionStep::getNextOption(CdecisionStepOption *pPrevious)
{
  CdecisionStepOption *pOption = (CdecisionStepOption *) 0 ;

  if (vect_val.empty())
  	return pOption ;

  bool bReturnNext = false ;
  if (NULL == pPrevious)
  	bReturnNext = true ;

  for (ValIter ival = vect_val.begin() ;  vect_val.end() != ival ; ival++)
  {
  	// on récupère l'objet dialogue
    if (LABEL_STEP_DECISION_OPTION == (*ival)->getLabel())
    {
    	CdecisionStepOption *pBuf = dynamic_cast<CdecisionStepOption *>((*ival)->getObject()) ;
      if (pBuf && bReturnNext)
      {
      	pOption = pBuf ;
        break ;
      }
      if (pBuf == pPrevious)
      	bReturnNext = true ;
    }
  }

  return pOption ;
}

string
CdecisionStep::getType()
{
  return getStringAttribute(ATTRIBUT_DECISION_TYPE) ;
}

// -----------------------------------------------------------------------------
//                        CdecisionStepOption class// -----------------------------------------------------------------------------
CdecisionStepOption::CdecisionStepOption(string attributs, string values, Cbalise *father)
                    :Cbalise(attributs, values, father)
{
}

CdecisionStepOption::~CdecisionStepOption()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      Ccontrainte *pCCtr = dynamic_cast<Ccontrainte*>(pBalise) ;
      if (pCCtr)
      {
        delete pCCtr ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Chead *pCHead = dynamic_cast<Chead*>(pBalise) ;
        if (pCHead)
        {
          delete pCHead ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

CdecisionStepOption::CdecisionStepOption(const CdecisionStepOption& rv)
                    :Cbalise(rv)
{
}

CdecisionStepOption&
CdecisionStepOption::operator=(const CdecisionStepOption& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this;
	Cbalise* pbdata2 = (CdecisionStepOption*)&src;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2;

  return (*this) ;
}

bool
CdecisionStepOption::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_HEAD == (*ival)->getLabel())
		{
      (*ival)->setObject(new Chead((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_VALIDITE == (*ival)->getLabel())
		{
      (*ival)->setObject(new Ccontrainte((*ival)->getAttribute(), (*ival)->getValue(), this, true)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

/*
	if ((nb_dialogue > 1)  || (nb_dialogbox > 1) || (nb_items != 1) || (nb_contraintes > 1) || (nb_present > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
*/
	return true ;
}

bool
CdecisionStepOption::verif(){
  return true ;
}

bool
CdecisionStepOption::compresser()
{
  return true ;
}

bool
CdecisionStepOption::traiter()
{
  return true ;
}

string
CdecisionStepOption::getDestination()
{
  return getStringAttribute(ATTRIBUT_DECISION_OPTION_DESTINATION) ;
}

string
CdecisionStepOption::getDecisionOptionId()
{
  return getStringAttribute(ATTRIBUT_DECISION_OPTION_ID) ;
}

Ccontrainte*
CdecisionStepOption::getValidator()
{
  Ccontrainte *pValidator = (Ccontrainte *) 0 ;

  if (vect_val.empty())
    return pValidator ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    if (LABEL_VALIDITE == (*ival)->getLabel())
    {
      pValidator = dynamic_cast<Ccontrainte *>((*ival)->getObject()) ;
      break ;
    }
  }

  return pValidator ;
}

// -----------------------------------------------------------------------------
//                           CactionStep class// -----------------------------------------------------------------------------
CactionStep::CactionStep(string attributs, string values, Cbalise *father, NSSuper* pSuper)
            :CguidelineStep(attributs, values, father)
{
  _pSuper = pSuper ;
}

CactionStep::~CactionStep()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      Cproposition *pCProp = dynamic_cast<Cproposition*>(pBalise) ;
      if (pCProp)
      {
        delete pCProp ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Chead *pCHead = dynamic_cast<Chead*>(pBalise) ;
        if (pCHead)
        {
          delete pCHead ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          Cconcern *pCCcrn = dynamic_cast<Cconcern*>(pBalise) ;
          if (pCCcrn)
          {
            delete pCCcrn ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
          else
          {
            CactionArchetype *pCArc = dynamic_cast<CactionArchetype*>(pBalise) ;
            if (pCArc)
            {
              delete pCArc ;
              (*ival)->setObject((Cbalise*) 0) ;
            }
            else
            {
              CactionReferential *pCRef = dynamic_cast<CactionReferential*>(pBalise) ;
              if (pCRef)
              {
                delete pCRef ;
                (*ival)->setObject((Cbalise*) 0) ;
              }
              else
              {
                CactionLetter *pCLtr = dynamic_cast<CactionLetter*>(pBalise) ;
                if (pCLtr)
                {
                  delete pCLtr ;
                  (*ival)->setObject((Cbalise*) 0) ;
                }
              }
            }
          }
        }
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

CactionStep::CactionStep(const CactionStep& rv)
            :CguidelineStep(rv)
{
  _pSuper = rv._pSuper ;
}

CactionStep&
CactionStep::operator=(const CactionStep& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this ;
	Cbalise* pbdata2 = (CactionStep*)&src ;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2 ;

  return (*this) ;
}

bool
CactionStep::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (getStringAttribute(ATTRIBUT_STEP_ID) == string(""))
  {
    // erreur (pas de nom)
    _iParsingError = EDECISION_STEP_NO_ID ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_HEAD == (*ival)->getLabel())
		{
      (*ival)->setObject(new Chead((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_PROPOSITION == (*ival)->getLabel())
		{
      (*ival)->setObject(new Cproposition((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_CONCERN == (*ival)->getLabel())
		{
      (*ival)->setObject(new Cconcern((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_STEP_ACTION_ARCHETYPE == (*ival)->getLabel())
		{
      (*ival)->setObject(new CactionArchetype((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_STEP_ACTION_REF == (*ival)->getLabel())
		{
      (*ival)->setObject(new CactionReferential((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_STEP_ACTION_LETTER == (*ival)->getLabel())
		{
      (*ival)->setObject(new CactionLetter((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

/*
	if ((nb_dialogue > 1)  || (nb_dialogbox > 1) || (nb_items != 1) || (nb_contraintes > 1) || (nb_present > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
*/
	return true ;
}boolCactionStep::verif(){
  return true ;
}

bool
CactionStep::compresser()
{
  return true ;
}

bool
CactionStep::traiter()
{
  return true ;
}

string
CactionStep::getNextStepId()
{
  return getStringAttribute(ATTRIBUT_STEP_NEXT_STEP_ID) ;
}

Cconcern*
CactionStep::getConcernToCreate()
{
  if (vect_val.empty())
  	return (Cconcern *) 0 ;

  for (ValIter ival = vect_val.begin() ;  vect_val.end() != ival ; ival++)
  {
    if (LABEL_CONCERN == (*ival)->getLabel())
    {
    	Cconcern *pConcern = dynamic_cast<Cconcern *>((*ival)->getObject()) ;
      if (pConcern)
        return pConcern ;
    }
  }

  return (Cconcern *) 0 ;
}

CactionArchetype*
CactionStep::getArchetype()
{
  if (vect_val.empty())
  	return (Cconcern *) 0 ;

  for (ValIter ival = vect_val.begin() ;  vect_val.end() != ival ; ival++)
  {
    if (LABEL_STEP_ACTION_ARCHETYPE == (*ival)->getLabel())
    {
    	CactionArchetype *pArchetype = dynamic_cast<CactionArchetype *>((*ival)->getObject()) ;
      if (pArchetype)
        return pArchetype ;
    }
  }

  return (CactionArchetype *) 0 ;
}

// -----------------------------------------------------------------------------
//                           CactionArchetype class// -----------------------------------------------------------------------------
CactionArchetype::CactionArchetype(string attributs, string values, Cbalise *father)
                 :Cbalise(attributs, values, father)
{
}

CactionArchetype::~CactionArchetype()
{
}

CactionArchetype::CactionArchetype(const CactionArchetype& rv)
                 :Cbalise(rv)
{
}

CactionArchetype&
CactionArchetype::operator=(const CactionArchetype& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this ;
	Cbalise* pbdata2 = (CactionArchetype*) &src ;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2 ;

  return (*this) ;
}

bool
CactionArchetype::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	return true ;
}boolCactionArchetype::verif(){
  return true ;
}

bool
CactionArchetype::compresser()
{
  return true ;
}

bool
CactionArchetype::traiter()
{
  return true ;
}

string
CactionArchetype::getArchetypeId()
{
  return getStringAttribute(ATTRIBUT_ACTION_TEST) ;
}

string
CactionArchetype::getQualityOfLifeId()
{
  return getStringAttribute(ATTRIBUT_ACTION_QOL) ;
}

// -----------------------------------------------------------------------------
//                           CactionReferential class// -----------------------------------------------------------------------------
CactionReferential::CactionReferential(string attributs, string values, Cbalise *father)
                   :Cbalise(attributs, values, father)
{
}

CactionReferential::~CactionReferential()
{
}

CactionReferential::CactionReferential(const CactionReferential& rv)
                   :Cbalise(rv)
{
}

CactionReferential&
CactionReferential::operator=(const CactionReferential& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this ;
	Cbalise* pbdata2 = (CactionReferential*) &src ;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2 ;

  return (*this) ;
}

bool
CactionReferential::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	return true ;
}boolCactionReferential::verif(){
  return true ;
}

bool
CactionReferential::compresser()
{
  return true ;
}

bool
CactionReferential::traiter()
{
  return true ;
}

// -----------------------------------------------------------------------------
//                           CactionLetter class// -----------------------------------------------------------------------------
CactionLetter::CactionLetter(string attributs, string values, Cbalise *father)
              :Cbalise(attributs, values, father)
{
}

CactionLetter::~CactionLetter()
{
}

CactionLetter::CactionLetter(const CactionLetter& rv)
              :Cbalise(rv)
{
}

CactionLetter&
CactionLetter::operator=(const CactionLetter& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this ;
	Cbalise* pbdata2 = (CactionLetter*) &src ;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2 ;

  return (*this) ;
}

bool
CactionLetter::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	return true ;
}boolCactionLetter::verif(){
  return true ;
}

bool
CactionLetter::compresser()
{
  return true ;
}

bool
CactionLetter::traiter()
{
  return true ;
}

// -----------------------------------------------------------------------------
//                           CpatientStateStep class// -----------------------------------------------------------------------------
CpatientStateStep::CpatientStateStep(string attributs, string values, Cbalise *father)
                  :CguidelineStep(attributs, values, father)
{
}

CpatientStateStep::~CpatientStateStep()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      Chead *pCHead = dynamic_cast<Chead*>(pBalise) ;
      if (pCHead)
      {
        delete pCHead ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Cvalidateur *pCValid = dynamic_cast<Cvalidateur*>(pBalise) ;
        if (pCValid)
        {
          delete pCValid ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

CpatientStateStep::CpatientStateStep(const CpatientStateStep& rv)
                  :CguidelineStep(rv)
{
}

CpatientStateStep&
CpatientStateStep::operator=(const CpatientStateStep& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this ;
	Cbalise* pbdata2 = (CpatientStateStep*) &src ;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2 ;

  return (*this) ;
}

bool
CpatientStateStep::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (getStringAttribute(ATTRIBUT_STEP_ID) == string(""))
  {
    // erreur (pas de nom)
    _iParsingError = EDECISION_STEP_NO_ID ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_HEAD == (*ival)->getLabel())
		{
      (*ival)->setObject(new Chead((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_VALIDATEUR == (*ival)->getLabel())
		{
      (*ival)->setObject(new Cvalidateur((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

/*
	if ((nb_dialogue > 1)  || (nb_dialogbox > 1) || (nb_items != 1) || (nb_contraintes > 1) || (nb_present > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
*/
	return true ;
}boolCpatientStateStep::verif(){
  return true ;
}

bool
CpatientStateStep::compresser()
{
  return true ;
}

bool
CpatientStateStep::traiter()
{
  return true ;
}

string
CpatientStateStep::getNextStepId()
{
  return getStringAttribute(ATTRIBUT_STEP_NEXT_STEP_ID) ;
}

// -----------------------------------------------------------------------------
//                            CbranchStep class// -----------------------------------------------------------------------------
CbranchStep::CbranchStep(string attributs, string values, Cbalise *father)
            :CguidelineStep(attributs, values, father)
{
}

CbranchStep::~CbranchStep()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      Chead *pCHead = dynamic_cast<Chead*>(pBalise) ;
      if (pCHead)
      {
        delete pCHead ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        CbranchStepBranch *pCBrch = dynamic_cast<CbranchStepBranch*>(pBalise) ;
        if (pCBrch)
        {
          delete pCBrch ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

CbranchStep::CbranchStep(const CbranchStep& rv)
            :CguidelineStep(rv)
{
}

CbranchStep&
CbranchStep::operator=(const CbranchStep& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this ;
	Cbalise* pbdata2 = (CbranchStep*) &src ;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2 ;

  return (*this) ;
}

bool
CbranchStep::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (getStringAttribute(ATTRIBUT_STEP_ID) == string(""))
  {
    // erreur (pas de nom)
    _iParsingError = EDECISION_STEP_NO_ID ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_STEP_BRANCH_BRANCH == (*ival)->getLabel())
		{
      (*ival)->setObject(new CbranchStepBranch((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_HEAD == (*ival)->getLabel())
		{
      (*ival)->setObject(new Chead((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

/*
	if ((nb_dialogue > 1)  || (nb_dialogbox > 1) || (nb_items != 1) || (nb_contraintes > 1) || (nb_present > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
*/
	return true ;
}boolCbranchStep::verif(){
  return true ;
}

bool
CbranchStep::compresser()
{
  return true ;
}

bool
CbranchStep::traiter()
{
  return true ;
}

string
CbranchStep::getOrderConstraint()
{
  return getStringAttribute(ATTRIBUT_BRANCH_ORDER_CONSTRAINT) ;
}

CbranchStepBranch *
CbranchStep::getNextBranch(CbranchStepBranch *pPrevious)
{
  CbranchStepBranch *pBranch = (CbranchStepBranch *) 0 ;

  if (vect_val.empty())
  	return pBranch ;

  bool bReturnNext = false ;
  if (NULL == pPrevious)
  	bReturnNext = true ;

  for (ValIter ival = vect_val.begin() ;  vect_val.end() != ival ; ival++)
  {
  	// on récupère l'objet dialogue
    if (LABEL_STEP_BRANCH_BRANCH == (*ival)->getLabel())
    {
    	CbranchStepBranch *pBuf = dynamic_cast<CbranchStepBranch *>((*ival)->getObject()) ;
      if (pBuf && bReturnNext)
      {
      	pBranch = pBuf ;
        break ;
      }
      if (pBuf == pPrevious)
      	bReturnNext = true ;
    }
  }

  return pBranch ;
}

// -----------------------------------------------------------------------------
//                           CbranchStepBranch class// -----------------------------------------------------------------------------
CbranchStepBranch::CbranchStepBranch(string attributs, string values, Cbalise *father)
                  :Cbalise(attributs, values, father)
{
}

CbranchStepBranch::~CbranchStepBranch()
{
}

CbranchStepBranch::CbranchStepBranch(const CbranchStepBranch& rv)
                  :Cbalise(rv)
{
}

CbranchStepBranch&
CbranchStepBranch::operator=(const CbranchStepBranch& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this ;
	Cbalise* pbdata2 = (CbranchStepBranch*) &src ;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2 ;

  return (*this) ;
}

bool
CbranchStepBranch::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	return true ;
}boolCbranchStepBranch::verif(){
  return true ;
}

bool
CbranchStepBranch::compresser()
{
  return true ;
}

bool
CbranchStepBranch::traiter()
{
  return true ;
}

string
CbranchStepBranch::getNextStepId()
{
  return getStringAttribute(ATTRIBUT_STEP_NEXT_STEP_ID) ;
}

string
CbranchStepBranch::getSubBranchId()
{
  return getStringAttribute(ATTRIBUT_BRANCH_BRANCH_ID) ;
}

// -----------------------------------------------------------------------------
//                            CsynchronizationStep class// -----------------------------------------------------------------------------
CsynchronizationStep::CsynchronizationStep(string attributs, string values, Cbalise *father)
                     :CguidelineStep(attributs, values, father)
{
}

CsynchronizationStep::~CsynchronizationStep()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      Chead *pCHead = dynamic_cast<Chead*>(pBalise) ;
      if (pCHead)
      {
        delete pCHead ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Cvalidateur *pCValid = dynamic_cast<Cvalidateur*>(pBalise) ;
        if (pCValid)
        {
          delete pCValid ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

CsynchronizationStep::CsynchronizationStep(const CsynchronizationStep& rv)
                     :CguidelineStep(rv)
{
}

CsynchronizationStep&
CsynchronizationStep::operator=(const CsynchronizationStep& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this ;
	Cbalise* pbdata2 = (CsynchronizationStep*) &src ;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2 ;

  return (*this) ;
}

bool
CsynchronizationStep::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (getStringAttribute(ATTRIBUT_STEP_ID) == string(""))
  {
    // erreur (pas de nom)
    _iParsingError = EDECISION_STEP_NO_ID ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_HEAD == (*ival)->getLabel())
		{
      (*ival)->setObject(new Chead((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_VALIDATEUR == (*ival)->getLabel())
		{
      (*ival)->setObject(new Cvalidateur((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

/*
	if ((nb_dialogue > 1)  || (nb_dialogbox > 1) || (nb_items != 1) || (nb_contraintes > 1) || (nb_present > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
*/
	return true ;
}
bool
CsynchronizationStep::verif(){
  return true ;
}

bool
CsynchronizationStep::compresser()
{
  return true ;
}

bool
CsynchronizationStep::traiter()
{
  return true ;
}

string
CsynchronizationStep::getNextStepId()
{
  return getStringAttribute(ATTRIBUT_SYNCHRO_NEXT_STEP) ;
}

string
CsynchronizationStep::getLogicalExpression()
{
  return getStringAttribute(ATTRIBUT_SYNCHRO_LOGICAL_EXPRESSION) ;
}

