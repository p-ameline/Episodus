/*
** Copyright Nautilus, (10/9/2004)
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/
#ifndef   TOOLBOX_H
# define   TOOLBOX_H

class BB1Object;
class NVLdVTemps;
class NSPatPathoArray;
class BB1BB;
class NSHistoryValManagementArray ;

#include <string>
// #include "nsbb\nslistwind.h"
#include "ns_ob1\Transfer.h"
#include "ns_ob1\OB1Export.h"

std::string _BBKEXPORT getStringAnswer(const BB1BB& bb, BB1Object* pAnswerObject) ;

/**
*  Return a bool information that is present in the transfert structure and identified by string sQuestion <BR>
*  Renvoie une information (sous forme de bool) présente dans la structure de transfert est identifié par la chaine sQuestion
*  @param  reponse collection of information given to the KS
*  @param  sQuestion Name of the boolean question to find
*  @return the bool information
*/
bool  _BBKEXPORT    getBoolAnswer(Transfert& reponse, std::string& sQuestion, std::string& sSearchParams) ;

/**
*  Return an int information that is present in the transfert structure and identified by string sQuestion <BR>
*  Renvoie une information (sous forme d'entier) présente dans la structure de transfert est identifié par la chaine sQuestion
*  @param  bb Pointer to acces BB's function
*  @param  reponse collection of information given to the KS
*  @param  sQuestion Name of the boolean question to find
*  @param sRefUnit Unit of the information
*  @param iValue  Information to fill
*  @return the true if the information has been filled and false else
*/
bool  _BBKEXPORT    getIntAnswer(const BB1BB& bb, Transfert& reponse, std::string sQuestion, std::string& sSearchParams, std::string sRefUnit, int* iValue) ;

/**
*  Return a double information that is present in the transfert structure and identified by string sQuestion <BR>
*  Renvoie une information (sous forme de double) présente dans la structure de transfert est identifié par la chaine sQuestion
*  @param  bb Pointer to acces BB's function
*  @param  reponse collection of information given to the KS
*  @param  sQuestion Name of the boolean question to find
*  @param sRefUnit Unit of the information
*  @param dValue  Information to fill
*  @return the true if the information has been filled and false else
*/
bool  _BBKEXPORT    getDoubleAnswer(const BB1BB& bb, Transfert& reponse, string sQuestion, std::string& sSearchParams, string sRefUnit, double *dValue) ;

/** \todo A commenter */
bool  _BBKEXPORT    getIntervalAnswer(const BB1BB& bb, Transfert& reponse, std::string sQuestion, std::string& sSearchParams, std::string sRefUnit, std::string sInterval) ;

/** \todo A commenter */
bool  _BBKEXPORT    getTimeAnswer(const BB1BB& bb, Transfert& reponse, std::string sQuestion, std::string& sSearchParams, NVLdVTemps* pTpsValue) ;

/** \todo A commenter */
int	  _BBKEXPORT    sexePrecondition(const BB1BB& bb, std::string& sexe, Transfert* trns) ;

/** \todo A commenter */
bool  _BBKEXPORT    answer(const BB1BB& bb, string sQuestion, string sAnswer, NSSearchStruct *pSearchStruct) ;

/**
* Write a boolean answer on blackboard <BR>
* Ecrit une reponse booleene sur le blackboard
* @param bb pointer to access bb' functions
* @param sQuestion Identifiant of the question to answer
* @param sAnswer Response to do
* @return return true if the operation suceed and false else
*/
bool  _BBKEXPORT    answerfalse(const BB1BB& bb, string sQuestion, string sAnswer, NSSearchStruct *pSearchStruct) ;

/**
* Write a boolean answer on blackboard <BR>
* Ecrit une reponse booleene sur le blackboard
* @param bb pointer to access bb' functions
* @param  sAnswer Answer to respond
* @param 	Type of risk
* @return return true if the operation suceed and false else
*/
bool  _BBKEXPORT    answertrue(const BB1BB& bb, string sAnswer, string sRisque, NSSearchStruct *pSearchStruct) ;

bool  _BBKEXPORT    getFullSortedHistory(const BB1BB& bb, NSHistoryValManagementArray* pArray, string sValue) ;

#endif

