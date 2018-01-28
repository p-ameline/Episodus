// -----------------------------------------------------------------------------
// nsarcParseError.h
// erreurs de parsing au niveau des archetypes
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: pameline $
// $Date: 2011/03/04 13:55:03 $
// -----------------------------------------------------------------------------
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

#ifndef __NSARCPARSEERROR_H__
# define __NSARCPARSEERROR_H__

# include <vector.h>
# include <string>


string parseErrorMessage(int iError) ;

enum nsarcParseError { EA_NOERROR = 0, EATTR_EMPTY, EATTR_NOEQUAL, EATTR_EQUAL0, EATTR_EQUALLAST, EATTR_NOVALUE,
  /* 6 */   EATTR_QUOTELAST,EATTR_QUOTECLOSE, EATTR_QUOTEFIN, EATTR_QUOTESPACE, EATTR_DQUOTELAST,
	/* 11 */  EATTR_DQUOTECLOSE, EATTR_DQUOTEFIN, EATTR_DQUOTESPACE,
	/* 16 */  EVAL_NOCLOSE, EVAL_NOOPEN, EVAL_SPACE, EVAL_CLOSE, EVAL_CLOSEBALISE,
	/* 21 */  EVAL_DEF, EARCHETYPE_VAL_EMPTY, EARCHETYPE_VAL, EARCHETYPE_NONAME, EARCHETYPE_NBVAL,
  EARCHETYPE_BALISE_DEBUT, EARCHETYPE_BALISE_DEBUT_FERMANTE, EARCHETYPE_BALISE_FIN,
  EDIALOGUE_ATTR_EMPTY, EDIALOGUE_ATTR, EDIALOGUE_NBATTR, EDIALOGUE_VAL_NOT_EMPTY,
  EITEM_ATTR_EMPTY, EITEM_ATTR, EITEM_NBATTR, EITEM_VAL_EMPTY, EITEM_VAL,
  ECONTR_ATTR_EMPTY, ECONTR_ATTR, ECONTR_NBATTR, ECONTR_VAL_EMPTY, ECONTR_VAL,
  EVARIABLE_ATTR_EMPTY, EVARIABLE_ATTR, EVARIABLE_NBATTR, EVARIABLE_VAL_EMPTY, EVARIABLE_VAL, EVARIABLE_NBVAL,
  EVALIDATEUR_ATTR_EMPTY, EVALIDATEUR_ATTR, EVALIDATEUR_NBATTR, EVALIDATEUR_VAL_NOT_EMPTY,
  EREFERENTIEL_VAL_EMPTY, EREFERENTIEL_VAL, EREFERENTIEL_NBVAL, EREFERENTIEL_NONAME,
  EREFERENTIEL_BALISE_DEBUT, EREFERENTIEL_BALISE_DEBUT_FERMANTE, EREFERENTIEL_BALISE_FIN,
  EGVARS_VAL_EMPTY, EGVARS_VAL,
  EPROP_ATTR_EMPTY, EPROP_ATTR, EPROP_NBATTR, EPROP_VAL_EMPTY, EPROP_VAL, EPROP_NBVAL,
  ETREE_ATTR_EMPTY, ETREE_ATTR, ETREE_NBATTR, ETREE_VAL_EMPTY, ETREE_VAL, ETREE_NBVAL,
  ENODE_ATTR_EMPTY, ENODE_ATTR, ENODE_NBATTR, ENODE_VAL,
  EDIALOGBOX_ATTR_EMPTY, EDIALOGBOX_ATTR, EDIALOGBOX_NBATTR, EDIALOGBOX_VAL_EMPTY, EDIALOGBOX_VAL, EDIALOGBOX_NBVAL,
  ECONTROL_ATTR_EMPTY, ECONTROL_ATTR, ECONTROL_ATTR_PARSING, ECONTROL_NBATTR, ECONTROL_VAL_NOT_EMPTY,
  ERCDATA_ATTR_EMPTY, ERCDATA_ATTR, ERCDATA_NBATTR, ERCDATA_VAL_NOT_EMPTY,
  ETABCONTROL_VAL_EMPTY, ETABCONTROL_VAL, ETAB_ATTR_EMPTY, ETAB_ATTR, ETAB_NBATTR, ETAB_VAL_NOT_EMPTY,
  EPRESENT_ATTR_EMPTY, EPRESENT_ATTR, EPRESENT_NBATTR, EPRESENT_VAL_NOT_EMPTY,
  EGUIDELINE_NONAME, EGUIDELINE_VAL_EMPTY, EGUIDELINE_VAL, EDECISION_STEP_NO_ID, EDECISION_STEP_VAL_EMPTY } ;

#endif // !__NSARCPARSEERROR_H__
