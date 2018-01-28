// -----------------------------------------------------------------------------
// nsTeamDefs.h
// -----------------------------------------------------------------------------
// Gestion l'Equipe de Santé
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:05:29 $
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

#ifndef __TEAMDEFS_H__
# define __TEAMDEFS_H__


// -----------------------------------------------------------------------------
// Définitions des termes Lexique utilisés.
// --
// Definition of Lexical Terms that we use.
// -----------------------------------------------------------------------------


# define HEALTHTEAM								"LEQUI1"																			// code lexique correspondant à "Equipe de Santé"
# define HTTEAM										"LEQUM1"																			// code lexique correspondant à "Equipe (personne morale)"

# define TEAMADMIN								"LADMI1"																			// code lexique correspondant à "données administratives"
# define TEAMLABEL								"LNOMA1"																			// code lexique correspondant à "nom {chose}"
# define TEAMSPEC									"LSPEC1"																			// code lexique correspondant à "spécialité"
# define TEAMADDRS								"LCADR1"																			// code lexique correspondant à "carnet d'adresses"
# define TEAMADDRESS							"LADRE1"																			// code lexique correspondant à "adresse"

# define HTMEMBER									"HMEMB1"																			// code lexique correspondant à "membre (au sein de l'Equipe de Santé)"

// role
# define HTMEMBERPROFESSION				"LMETI1"																			// code lexique correspondant à "métier"
# define HTMEMBERSPECIALITY				"LSPEC1"																			// code lexique correspondant à "spécialité [médicale]"

// mandat durée
# define HTMEMBERUSERMANDATE			"LMAND1"																			// code lexique correspondant à un "Mandat d'Utilisation"
# define HTMEMBERROOTMANDATE			"LROOT1"																			// code lexique correspondant à un "Mandat d'Administration"
# define HTMEMBERMANDATEBEGIN			"KOUVR1"																			// code lexique correspondant à "date d'ouverture"
# define HTMEMBERMANDATEEND				"KFERM1"																			// code lexique correspondant à "date de fermeture"
# define HTMEMBERMANDATEDURATION	"VDURE1"																			// code lexique correspondant à "durée"

// position
# define HTMEMBERPOSITION					"LPOSI1"																			// code lexique correspondant à "position (au sein de l'Equipe de Santé)"
# define HTMEMBERDISTANCE					"VDIPA1"																			// code lexique correspondant à "distance (au patient)"
# define HTMEMBERANGLE						"VANPA1"																			// code lexique correspondant à "angle (dans le sens rosace du terme)"

// valeur
# define HTTEAMFREETEXT						"£CL000"
# define HTTEAMLEXIVAL						"£C;020"
# define HTMEMBERID								"£SPID1"																			// code lexique correspondant à "id (nss)" (PIDS)
# define HTOBJECTID								"£OBJE1"																			// code lexique correspondant à l'ID d'un objet (OIDS)
# define HTVALDATE               	"£T0;19"
# define HTVALNUM									"£N"                                        	// "valeur numérique (sur 2 digits)"
# define HTVALNUM2								"£N0;02"                                      // code lexique correspondant à une "valeur numérique (sur 2 digits)"
// #define HTVALNUM3								"£N0;03"                                    // code lexique correspondant à une "valeur numérique (sur 3 digits)"

// unité
# define HTNOUNIT								"200001"																				// code lexique correspondant à "pas d'unité"
# define HTDATE									"2DA021"																				// code lexique d'une valeur correspondant à "une date"
# define HTANGUNIT								"2RODE1"																				// code lexique correspondant à "degré d'angle de rosace"

#endif // __TEAMDEFS_H__
