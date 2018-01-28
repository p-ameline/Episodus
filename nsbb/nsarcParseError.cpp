// -----------------------------------------------------------------------------
// nsarcParseError.cpp
// erreurs de parsing au niveau des archetypes
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: pameline $
// $Date: 2011/03/04 13:22:47 $
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


#include "nsbb\nsarcParseError.h"

string parseErrorMessage(int iError)
{
  switch (iError)
  {
    case EA_NOERROR                         : return string ("il n'y pas d'erreur constatée") ;
    case EATTR_EMPTY                        : return string("la chaine des attributs est vide") ;
    case EATTR_NOEQUAL                      : return string("la chaine des attributs ne comporte pas de signe egal") ;
    case EATTR_EQUAL0                       : return string("la chaine des attributs a un signe egal comme premier caractere") ;
    case EATTR_EQUALLAST                    : return string("la chaine des attributs a un signe egal mais en derniere position") ;
    case EATTR_NOVALUE                      : return string("la chaine des attributs a un signe egal non suivi d'un attribut") ;

    case EATTR_QUOTELAST                    : return string("la chaine des attributs a un caratere \' mais en derniere position") ;
    case EATTR_QUOTECLOSE                   : return string("la chaine des attributs ne possede pas de caractere \' de fermeture") ;
    case EATTR_QUOTEFIN                     : return string("la chaine des attributs possede un caractère \' mais c est la fin de chaine") ;
    case EATTR_QUOTESPACE                   : return string("la chaine des attributs n a pas de caractere espace derriere le \'") ;

    case EATTR_DQUOTELAST                   : return string("la chaine des attributs a un caractere \" mais en derniere position ") ;
    case EATTR_DQUOTECLOSE                  : return string("la chaine des attributs ne possede pas de caractere \" de fermeture") ;
    case EATTR_DQUOTEFIN                    : return string("la chaine des attributs possede un caractere \" mais c est la fin de chaine") ;
    case EATTR_DQUOTESPACE                  : return string("la chaine des attributs n a pas de caractere espace derriere le \"") ;

    case EVAL_NOCLOSE                       : return string("la chaine n a pas d indicateur de fin de balise : >") ;
    case EVAL_NOOPEN                        : return string("il manque un caractere de balise ouvrante : < dans la chaine des valeurs") ;
    case EVAL_SPACE                         : return string("il y a un caractere espace juste apres l ouverture de la balise") ;
    case EVAL_CLOSE                         : return string("il n y a pas ou il manque une balise de fermeture") ;
    case EVAL_CLOSEBALISE                   : return string("il manque un caractere de fermeture de balise : >") ;
    case EVAL_DEF                           : return string("la fin de la chaine des attributs de la balise est mal definie ") ;

    case EARCHETYPE_VAL_EMPTY               : return string("le champ valeur de l'archetype est vide") ;
    case EARCHETYPE_VAL                     : return string("le champ valeur de l'archetype est incorrect") ;
    case EARCHETYPE_NONAME                  : return string("l'archetype n'a pas de nom") ;
    case EARCHETYPE_NBVAL                   : return string("le nombre de valeurs de l'archetype est incorrect") ;
    case EARCHETYPE_BALISE_DEBUT            : return string("l'archetype ne possede pas de balise de debut") ;
    case EARCHETYPE_BALISE_DEBUT_FERMANTE   : return string("l'archetype ne possede pas de balise de debut fermante") ;
    case EARCHETYPE_BALISE_FIN              : return string("l'archetype ne possede pas de balise de fin") ;

    case EDIALOGUE_ATTR_EMPTY               : return string("la chaine des attributs du dialogue est vide") ;
    case EDIALOGUE_ATTR                     : return string("un(les) attribut(s) du dialogue est(sont) incorrect(s)") ;
    case EDIALOGUE_NBATTR                   : return string("le nombre d'attributs du dialogue est incorrect") ;
    case EDIALOGUE_VAL_NOT_EMPTY            : return string("le champ valeur du dialogue n'est pas vide") ;

    case EITEM_ATTR_EMPTY                   : return string("la chaine des attributs d'un item est vide") ;
    case EITEM_ATTR                         : return string("un(les) attribut(s) d'un item est(sont) incorrect(s)") ;
    case EITEM_NBATTR                       : return string("le nombre d'attributs d'un item est incorrect") ;
    case EITEM_VAL_EMPTY                    : return string("le champ valeur de items est vide") ;
    case EITEM_VAL                          : return string("le champ valeur d'un item est incorrect") ;

    case ECONTR_ATTR_EMPTY                  : return string("la chaine des attributs d'une contrainte est vide") ;
    case ECONTR_ATTR                        : return string("un(les) attribut(s) d'une contrainte est(sont) incorrect(s)") ;
    case ECONTR_NBATTR                      : return string("le nombre d'attributs d'une contrainte est incorrect") ;
    case ECONTR_VAL_EMPTY                   : return string("le champ valeur de contraintes est vide") ;
    case ECONTR_VAL                         : return string("le champ valeur d'une contrainte est incorrect") ;

    case EVARIABLE_ATTR_EMPTY               : return string("la chaine des attributs d'une variable est vide") ;
    case EVARIABLE_ATTR                     : return string("un(les) attribut(s) d'une variable est(sont) incorrect(s)") ;
    case EVARIABLE_NBATTR                   : return string("le nombre d'attributs d'une variable est incorrect") ;
    case EVARIABLE_VAL_EMPTY                : return string("le champ valeur d'une variable est vide") ;
    case EVARIABLE_VAL                      : return string("le champ valeur d'une variable est incorrect") ;
    case EVARIABLE_NBVAL                    : return string("le nombre de valeurs d'une variable est incorrect") ;

    case EGVARS_VAL_EMPTY                   : return string("la liste des variables globales est vide") ;
    case EGVARS_VAL                         : return string("la liste des variables globales est incorrecte") ;

    case EVALIDATEUR_ATTR_EMPTY             : return string("la chaine des attributs du validateur est vide") ;
    case EVALIDATEUR_ATTR                   : return string("un(les) attribut(s) du validateur est(sont) incorrect(s)") ;
    case EVALIDATEUR_NBATTR                 : return string("le nombre d'attributs du validateur est incorrect") ;
    case EVALIDATEUR_VAL_NOT_EMPTY          : return string("le champ valeur du validateur n'est pas vide") ;

    case EREFERENTIEL_VAL_EMPTY             : return string("le champ valeur du referentiel est vide") ;
    case EREFERENTIEL_VAL                   : return string("le champ valeur du referentiel est incorrect") ;
    case EREFERENTIEL_NONAME                : return string("le referentiel n'a pas de nom") ;
    case EREFERENTIEL_NBVAL                 : return string("le nombre de valeurs du referentiel est incorrect") ;
    case EREFERENTIEL_BALISE_DEBUT          : return string("le referentiel ne possede pas de balise de debut") ;
    case EREFERENTIEL_BALISE_DEBUT_FERMANTE : return string("le referentiel ne possede pas de balise de debut fermante") ;
    case EREFERENTIEL_BALISE_FIN            : return string("le referentiel ne possede pas de balise de fin") ;

    case EPROP_ATTR_EMPTY                   : return string("la chaine des attributs d'une proposition est vide") ;
    case EPROP_ATTR                         : return string("un(les) attribut(s) d'une proposition est(sont) incorrect(s)") ;
    case EPROP_NBATTR                       : return string("le nombre d'attributs d'une proposition est incorrect") ;
    case EPROP_VAL_EMPTY                    : return string("le champ valeur d'une proposition est vide") ;
    case EPROP_VAL                          : return string("le champ valeur d'une proposition est incorrect") ;
    case EPROP_NBVAL                        : return string("le nombre de valeurs d'une proposition est incorrect") ;

    case ETREE_ATTR_EMPTY                   : return string("la chaine des attributs d'un tree est vide") ;
    case ETREE_ATTR                         : return string("un(les) attribut(s) d'un tree est(sont) incorrect(s)") ;
    case ETREE_NBATTR                       : return string("le nombre d'attributs d'un tree est incorrect") ;
    case ETREE_VAL_EMPTY                    : return string("le champ valeur d'un tree est vide") ;
    case ETREE_VAL                          : return string("le champ valeur d'un tree est incorrect") ;
    case ETREE_NBVAL                        : return string("le nombre de valeurs d'un tree est incorrect") ;

    case ENODE_ATTR_EMPTY                   : return string("la chaine des attributs d'un node est vide") ;
    case ENODE_ATTR                         : return string("un(les) attribut(s) d'un node est(sont) incorrect(s)") ;
    case ENODE_NBATTR                       : return string("le nombre d'attributs d'un node est incorrect") ;
    case ENODE_VAL                          : return string("le champ valeur d'un node est incorrect") ;

    case EDIALOGBOX_ATTR_EMPTY              : return string("La chaine des attributs d'une dialogbox est vide") ;
    case EDIALOGBOX_ATTR                    : return string("(un,les) attribut(s) d'une dialogbox (est,sont) incorrects") ;
    case EDIALOGBOX_NBATTR                  : return string("le nombre d'attributs d'une dialogbox est incorrect") ;
    case EDIALOGBOX_VAL_EMPTY               : return string("le champ valeur d'une dialogbox est vide") ;
    case EDIALOGBOX_VAL                     : return string("le champ valeur d'une dialogbox est incorrect") ;
    case EDIALOGBOX_NBVAL                   : return string("le nombre de valeurs d'une dialogbox est incorrect") ;

    case ECONTROL_ATTR_EMPTY                : return string("La chaine des attributs d'un control est vide") ;
    case ECONTROL_ATTR                      : return string("(un,les) attribut(s) d'un control (est,sont) incorrects") ;
    case ECONTROL_ATTR_PARSING              : return string("Le parsing des attributs d'un control est incorrect");
    case ECONTROL_NBATTR                    : return string("le nombre d'attributs d'un control est incorrect") ;
    case ECONTROL_VAL_NOT_EMPTY             : return string("le champ valeur d'un control n'est pas vide") ;

    case ERCDATA_ATTR_EMPTY                 : return string("La chaine des attributs d'un rcdata est vide") ;
    case ERCDATA_ATTR                       : return string("(un,les) attribut(s) d'un rcdata (est,sont) incorrects") ;
    case ERCDATA_NBATTR                     : return string("le nombre d'attributs d'un rcdata est incorrect") ;
    case ERCDATA_VAL_NOT_EMPTY              : return string("le champ valeur d'un rcdata n'est pas vide") ;

    case ETABCONTROL_VAL_EMPTY              : return string("Le champ valeur d'un tabcontrol est vide");
    case ETABCONTROL_VAL                    : return string("Le champ valeur d'un tabcontrol est incorrect");

    case ETAB_ATTR_EMPTY                    : return string("La chaine des attributs d'un onglet est vide") ;
    case ETAB_ATTR                          : return string("(un,les) attribut(s) d'un onglet (est,sont) incorrects") ;
    case ETAB_NBATTR                        : return string("le nombre d'attributs d'un onglet est incorrect") ;
    case ETAB_VAL_NOT_EMPTY                 : return string("le champ valeur d'un onglet n'est pas vide") ;

    case EPRESENT_ATTR_EMPTY                : return string("La chaine des attributs d'une presentation est vide") ;
    case EPRESENT_ATTR                      : return string("(un,les) attribut(s) d'une presentation (est,sont) incorrects") ;
    case EPRESENT_NBATTR                    : return string("le nombre d'attributs d'une presentation est incorrect") ;
    case EPRESENT_VAL_NOT_EMPTY             : return string("le champ valeur d'une presentation n'est pas vide") ;

    case EGUIDELINE_NONAME                  : return string("l'arbre décisionnel n'a pas de nom") ;
    case EGUIDELINE_VAL_EMPTY               : return string("le champ valeur de l'arbre décisionnel est vide") ;
    case EGUIDELINE_VAL                     : return string("le champ valeur de l'arbre décisionnel est incorrect") ;
    case EDECISION_STEP_NO_ID               : return string("un step ne possède pas d'identifiant") ;
    case EDECISION_STEP_VAL_EMPTY           : return string("le champ valeur de l'archetype est vide") ;

    default                                 : return string("erreur inconnue") ;
  }
}


