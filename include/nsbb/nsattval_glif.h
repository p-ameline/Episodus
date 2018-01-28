// -----------------------------------------------------------------------------
// nsattval_glif.h
// -----------------------------------------------------------------------------
// Attributs/Valeurs d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: pameline $
// $Date: 2013/11/25 16:04:30 $
// -----------------------------------------------------------------------------
// FLP - april 2005
// FLP - july 2003
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

#ifndef __NSATTVAL_GLIF_H__
# define __NSATTVAL_GLIF_H__

# include <Services\defs.h>
# include <vector.h>
# include <string>

class NSSuper ;
class NSPatPathoArray ;
class NSPatPathoInfo ;
class NSContexte ;
class VecteurString ;
class CbranchStepBranch ;
class CactionArchetype ;

# include "balise.h"
// # include "nautilus\nssuper.h"
// # include "nsbb\nspatpat.h"
# include "partage\NTArray.h"

# define LABEL_STEPS                 "steps"
# define LABEL_STEP_DECISION         "decision_step"
# define LABEL_STEP_DECISION_OPTION  "decision_option"
# define LABEL_STEP_ACTION           "action_step"
# define LABEL_STEP_ACTION_ARCHETYPE "action_archetype"
# define LABEL_STEP_ACTION_REF       "action_referential"
# define LABEL_STEP_ACTION_LETTER    "action_letter"
# define LABEL_STEP_BRANCH           "branch_step"
# define LABEL_STEP_BRANCH_BRANCH    "branch_step_branch"
# define LABEL_STEP_SYNCHRO          "synchronization_step"
# define LABEL_STEP_PATIENT_STATE    "patient_state_step"

# define ATTRIBUT_STEP_ID                     "step_id"
# define ATTRIBUT_STEP_NEXT_STEP_ID           "next_step_id"
# define ATTRIBUT_STEP_STRENGTH               "strength"
# define ATTRIBUT_DECISION_AUTOMATIC          "automatic_decision"
# define ATTRIBUT_DECISION_DEFAULT            "default_option_when_automatic"
# define ATTRIBUT_DECISION_TYPE               "type"
# define ATTRIBUT_DECISION_OPTION_ID          "option_id"
# define ATTRIBUT_DECISION_OPTION_DESTINATION "destination_step_id"
# define ATTRIBUT_DECISION_OPTION_NAME        "option_name"
# define ATTRIBUT_DECISION_OPTION_STRENGTH    "option_strength"
# define ATTRIBUT_ACTION_CREATE               "create_line"
# define ATTRIBUT_ACTION_RISK                 "create_risk_line"
# define ATTRIBUT_ACTION_TEST                 "open_test_archetype"
# define ATTRIBUT_ACTION_QOL                  "open_quality_of_life"
# define ATTRIBUT_BRANCH_ORDER_CONSTRAINT     "order_constraint"
# define ATTRIBUT_BRANCH_BRANCH_ID            "branch_id"
# define ATTRIBUT_SYNCHRO_NEXT_STEP           "next_step"
# define ATTRIBUT_SYNCHRO_LOGICAL_EXPRESSION  "logical_expression"

class _NSBBCLASSE Csteps : public Cbalise
{
 public:

  NSSuper* _pSuper ;

  Csteps(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
  ~Csteps() ;
  Csteps(const Csteps& rv) ;
  Csteps& operator=(const Csteps& src) ;

  bool parser() ;
  bool verif() ;
  bool compresser() ;
  bool traiter() ;

  CguidelineStep *getGuidelineStepById(string sId) ;
} ;

class _NSBBCLASSE CguidelineStep : public Cbalise
{
 public:

  CguidelineStep(string attributs, string values, Cbalise *father) ;
  virtual ~CguidelineStep() ;

  virtual bool parser()     = 0 ;
  virtual bool verif()      = 0 ;
  virtual bool compresser() = 0 ;
  virtual bool traiter()    = 0 ;

  string getStepId() ;
  string getTitle(string sLang) ;

  Chead * getHead(string sLang) ;
} ;

class CdecisionStepOption ;

class _NSBBCLASSE CdecisionStep : public CguidelineStep
{
 public:

  CdecisionStep(string attributs, string values, Cbalise *father) ;
  ~CdecisionStep() ;
  CdecisionStep(const CdecisionStep& rv) ;
  CdecisionStep& operator=(const CdecisionStep& src) ;

  bool parser() ;
  bool verif() ;
  bool compresser() ;
  bool traiter() ;

  bool                 isAutomatic() ;
  CdecisionStepOption* getDefaultOptionWhenAutomatic() ;
  string               getType() ;

  CdecisionStepOption *getNextOption(CdecisionStepOption *pPrevious) ;
} ;

class _NSBBCLASSE CdecisionStepOption : public Cbalise
{
 public:

  CdecisionStepOption(string attributs, string values, Cbalise *father) ;
  ~CdecisionStepOption() ;
  CdecisionStepOption(const CdecisionStepOption& rv) ;
  CdecisionStepOption& operator=(const CdecisionStepOption& src) ;

  bool         parser() ;
  bool         verif() ;
  bool         compresser() ;
  bool         traiter() ;

  string       getDecisionOptionId() ;
  string       getDestination() ;
  Ccontrainte* getValidator() ;
  string       getStrengthOfRecommandation() ;
  string       getName() ;
  Chead*       getHead(string sLang = string("")) ;
  string       getHelpUrl(string sLang = string("")) ;
  string       getDisplayName(string sLang = string("")) ;
} ;

class _NSBBCLASSE CactionStep : public CguidelineStep
{
 public:

  NSSuper* _pSuper ;

  CactionStep(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
  ~CactionStep() ;
  CactionStep(const CactionStep& rv) ;
  CactionStep& operator=(const CactionStep& src) ;

  bool parser() ;
  bool verif() ;
  bool compresser() ;
  bool traiter() ;

  string        getNextStepId() ;
  string        getStrengthOfRecommandation() ;
  string        getName() ;
  Chead*        getHead(string sLang = string("")) ;
  string        getHelpUrl(string sLang = string("")) ;
  string        getDisplayName(string sLang = string("")) ;

  Cproposition*     getGoalToCreate() ;
  Cconcern*         getConcernToCreate() ;
  CactionArchetype* getArchetype() ;
} ;

class _NSBBCLASSE CactionArchetype : public Cbalise
{
 public:

  CactionArchetype(string attributs, string values, Cbalise *father) ;
  ~CactionArchetype() ;
  CactionArchetype(const CactionArchetype& rv) ;
  CactionArchetype& operator=(const CactionArchetype& src) ;

  bool   parser() ;
  bool   verif() ;
  bool   compresser() ;
  bool   traiter() ;

  string getArchetypeId() ;
  string getQualityOfLifeId() ;
} ;

class _NSBBCLASSE CactionReferential : public Cbalise
{
 public:

  CactionReferential(string attributs, string values, Cbalise *father) ;
  ~CactionReferential() ;
  CactionReferential(const CactionReferential& rv) ;
  CactionReferential& operator=(const CactionReferential& src) ;

  bool   parser() ;
  bool   verif() ;
  bool   compresser() ;
  bool   traiter() ;
} ;

class _NSBBCLASSE CactionLetter : public Cbalise
{
 public:

  CactionLetter(string attributs, string values, Cbalise *father) ;
  ~CactionLetter() ;
  CactionLetter(const CactionLetter& rv) ;
  CactionLetter& operator=(const CactionLetter& src) ;

  bool   parser() ;
  bool   verif() ;
  bool   compresser() ;
  bool   traiter() ;
} ;

class _NSBBCLASSE CbranchStep : public CguidelineStep
{
 public:

  CbranchStep(string attributs, string values, Cbalise *father) ;
  ~CbranchStep() ;
  CbranchStep(const CbranchStep& rv) ;
  CbranchStep& operator=(const CbranchStep& src) ;

  bool parser() ;
  bool verif() ;
  bool compresser() ;
  bool traiter() ;

  string getOrderConstraint() ;

  CbranchStepBranch *getNextBranch(CbranchStepBranch *pPrevious) ;
} ;

class _NSBBCLASSE CbranchStepBranch : public Cbalise
{
 public:

  CbranchStepBranch(string attributs, string values, Cbalise *father) ;
  ~CbranchStepBranch() ;
  CbranchStepBranch(const CbranchStepBranch& rv) ;
  CbranchStepBranch& operator=(const CbranchStepBranch& src) ;

  bool parser() ;
  bool verif() ;
  bool compresser() ;
  bool traiter() ;

  string getNextStepId() ;
  string getSubBranchId() ;
} ;

class _NSBBCLASSE CsynchronizationStep : public CguidelineStep
{
 public:

  CsynchronizationStep(string attributs, string values, Cbalise *father) ;
  ~CsynchronizationStep() ;
  CsynchronizationStep(const CsynchronizationStep& rv) ;
  CsynchronizationStep& operator=(const CsynchronizationStep& src) ;

  bool parser() ;
  bool verif() ;
  bool compresser() ;
  bool traiter() ;

  string getNextStepId() ;
  string getStrengthOfRecommandation() ;
  string getName() ;
  Chead* getHead(string sLang = string("")) ;
  string getHelpUrl(string sLang = string("")) ;
  string getDisplayName(string sLang = string("")) ;
  string getLogicalExpression() ;
} ;

class _NSBBCLASSE CpatientStateStep : public CguidelineStep
{
 public:

  CpatientStateStep(string attributs, string values, Cbalise *father) ;
  ~CpatientStateStep() ;
  CpatientStateStep(const CpatientStateStep& rv) ;
  CpatientStateStep& operator=(const CpatientStateStep& src) ;

  bool parser() ;
  bool verif() ;
  bool compresser() ;
  bool traiter() ;

  string        getNextStepId() ;
  string        getStrengthOfRecommandation() ;
  Cvalidateur*  getValidator() ;
  string        getName() ;
  Chead*        getHead(string sLang = string("")) ;
  string        getHelpUrl(string sLang = string("")) ;
  string        getDisplayName(string sLang = string("")) ;
} ;

#endif // !__NSATTVAL_GLIF_H__

