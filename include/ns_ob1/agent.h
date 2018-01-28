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

#ifndef __AGENT_OB1_HH__
#define  __AGENT_OB1_HH__

#include "ns_ob1/abstractKS.h"
#include "ns_ob1/AgentTypeDef.h"
#include <string>
#include <map>

/**
* \brief Class permettatnt de se relier a des fonctions ou des variables de l'interpreteur
*/
/**
* Class permettant de se relier a des fonctions de l'interpreteur
* Cette fonction sert a relier le nom d'une fonction ou d'une variable avec la fonction
* equivalente dans un script.
* \exemple ("trigger","ma_fonction_script_trigger")
*/
class InterpreterLink
{
	private:

    std::string _funcName ; /**< Name of the var */
    std::string _varName ;  /**< Equivalent function in interpreter */

	public:

  	InterpreterLink(std::string InterFuncNam, std::string VarName = "") : _funcName(InterFuncNam), _varName(VarName)
    {
    }

    /**
    * \brief Get the name of the function
    */
    /**
    *   Get the name of the function
    */
    inline
    std::string getFunc() ;

    /**
    * \brief Get the name of the var
    */
    /**
    *   Get the name of the var
    */
    inline
    std::string getVar() ;

    bool isVar() { return  (_funcName != "") ; }
};

inline
std::string InterpreterLink::getFunc(){ return _funcName; }

inline
std::string InterpreterLink::getVar() { return _varName; }

/**
* \brief prototype d'un agent script
*/
/**
*  Prototype d'un agent script <BR>
*	 Un agent script est un KS permettant l'execution
*  d'un KS sous forme de script. <BR>
*  Pour cela, il possède une sorte de dictionnaire d'equivalence
*  entre les fonction nécessaire a l'execution d'un KS (trigger, validity, ...)
*  et les fonction réalisant le travail qui seront interpreté.
*/
class AgentScript : public AbstractKS
{
  public:

  	AgentScript() ;
    ~AgentScript() ;

 // public:
  	bool				        TriggerCondition(const BB1Event& event) ;
  	KsInitStructure*    Publication() ;
  	ValidityContextType ContextValidity(const BB1BB& bb, Transfert* trans) ;
		bool				        ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
  	bool				        Precondition(const BB1BB& bb, Transfert* trans) ;
  	Errcode			        Action(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;

    /**
    * \brief Insere le nom du fichier de description de l'agent
    */
    inline
    void 							setDescriptionFile(std::string description);

    /**
    * \brief Insere le nom de l'agent. Le nom de l'agent sera utilisé par le controler de OB1
    */
    inline
    void 							setAgentName(std::string name);

    inline
    void							setFirstName(std::string firs);

    inline
    void							setLastName(std::string lest);

    inline
    void							setEmail(std::string mail);

    inline
    void							setVersionType(ScriptVersion vers);

    inline
    void							setRealVersionType(std::string vers);

    inline
    void							setPublicationVar(KsInitStructure* pub);

    inline
    void 							InsertLink(std::string name, InterpreterLink* temp);

  protected:

  	std::string  														_description_file_name; /**< Nom du fichier de description \brief Nom du fichier de description */
    std::string  														_agent_name;            /**< Nom de l'agent \brief Nom de l'agent              */
    std::string  														_author_first_name;     /**< Prénom  \brief  Prénom                    */
    std::string  														_author_last_name;      /**< Nom de famille  \brief  Nom de famille            */
    std::string  														_author_mail;           /**< Mail de l'autheur  \brief Mail de l'autheur          */
    ScriptVersion   												_versionType;           /**< Version  \brief  Version                   */
    std::string 														_realVers;              /**< Numero de version  \brief  Numero de version          */
    KsInitStructure*  											_publication;           /**< Variable de publication \brief  Variable de publication     */
    std::map<std::string, InterpreterLink*> _link;                  /**< Link with var \brief Link with var               */
};

inline
void 							AgentScript::setDescriptionFile(std::string description) { _description_file_name = description; }

inline
void 							AgentScript::setAgentName(std::string name) {  _agent_name =  name; }

inline
void							AgentScript::setFirstName(std::string firs) { _author_first_name = firs; }

inline
void							AgentScript::setLastName(std::string lest) { _author_last_name = lest; }

inline
void							AgentScript::setEmail(std::string mail) { _author_mail =mail; }

inline
void							AgentScript::setVersionType(ScriptVersion vers) {  _versionType = vers; }

inline
void							AgentScript::setRealVersionType(std::string vers) {  _realVers = vers; }

inline
void							AgentScript::setPublicationVar(KsInitStructure* pub) { _publication = pub; }

inline
void 							AgentScript::InsertLink(std::string name, InterpreterLink* temp)
{
	_link[name] = temp;
}

#endif

