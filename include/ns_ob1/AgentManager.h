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
#ifndef __AGENT_MANAGER__HH_
#define  __AGENT_MANAGER__HH_

#include "ns_ob1/agent.h"

/**
* Used to Parse Agent XML File
*/
#include "nsutil\tinyxml.h"

/**
* \brief The goal is to give an Agent XML loader
*/
/**
* The goal is to give an Agent XML loader <BR>
* Le but de ce fichier est de fournir une interface pour la création
* d'un agent à a partir d'un fichier script XML
*/
class AgentManager
{
 public:
 		/**
    * \brief Constructor
    */
    /**
    * Constructor
    */
    AgentManager();

    /**
    * Create an agent from an xml script file <BR>
    * Crée un agent à partir d'un script
    * @param xmlNam XLM file'name
    * @return The agent
    */
		AgentScript* create_agent_from_script(std::string xmlName);
 private:
 		/**
    * parse declaration part of an XML script  <BR>
  	* Fonction parsant la déclaration du fichier XML
    * @param node Cml subtree to parse
    * @param ag  Agent to fill
    * @return return true if no parse error and false else
    */
 		bool 	ParseDescription(TiXmlNode* node, AgentScript* ag);

    /**
  	* \brief Fonction parsant la partie communication avec le controler du fichier XML
    */
    /**
    * Fonction parsant la partie communication avec le controler du fichier XML <BR>
    * @param node noeud xml a parser
    * @param ag agent a remplir
    * @return  true si le parse c'est déroulé correctement et false sinon
    */
 		bool 	ParseCommunicationWithScript(TiXmlNode* node, AgentScript* ag);

    /**
    * \brief Fonction parsant la partie liant l'interpreteur et le KSG
    */
    /**
  	* Fonction parsant la partie liant l'interpreteur et le KSG
    * @param node noeud xml a parser
    * @param ag agent a remplir
    * @return  true si le parse c'est déroulé correctement et false sinon
    */
 		bool 	ParseCommunicationWithInterpreter(TiXmlNode* node, AgentScript* ag);

    /**
    * \brief Fonction parsant la partie code du script
    */
    /**
  	* Fonction parsant la partie code du script
    * @param node noeud xml a parser
    * @param ag agent a remplir
    * @return  true si le parse c'est déroulé correctement et false sinon
    */
 		bool 	ParseCode(TiXmlNode* node, AgentScript* ag);

};



#endif
