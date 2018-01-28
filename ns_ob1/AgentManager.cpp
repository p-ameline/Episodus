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

#include "ns_ob1/AgentManager.h"
#include "ns_ob1/BB1Exception.h"
#include "ns_ob1/InitStructure.h"
#include <fstream.h>

AgentManager::AgentManager()
{
}

AgentScript*
AgentManager::create_agent_from_script(std::string xmlName)
{
	AgentScript* result = new AgentScript();

  try
  {
		TiXmlDocument doc( xmlName.c_str());
  	bool loadOkay = doc.LoadFile();
		if ( !loadOkay )
		{
			throw Exception(std::string("create_agent_from_script"),std::string(doc.ErrorDesc()), std::string(__FILE__), __LINE__);
		}
  	TiXmlNode* node = NULL;
  // Recuperation de la racine du Document XML
  	node = doc.RootElement();
		std::string RootVal = node->Value();
  	if (RootVal != "Agent")
  		throw Exception(std::string("create_agent_from_script"),std::string("La racine du document n'est pas : Agent"), std::string(__FILE__), __LINE__);

	// TRaitemetn de toutes les noeud de niveau 1
  	for( node = node->FirstChild(); node; node = node->NextSibling() )
		{
  		std::string ValChapter = node->Value();
    	if (!((ValChapter == "description") || (ValChapter == "communication_with_script") || (ValChapter == "script_execution") || (ValChapter == "script_code")))
             throw Exception(std::string("create_agent_from_script"),std::string("nous nous attendons à communication_with_script , description, script_execution "), std::string(__FILE__), __LINE__);

    	if (ValChapter == "description")
         ParseDescription(node,result);

      if  ("communication_with_script" == ValChapter)
      	ParseCommunicationWithScript(node,result);

      if  ("script_execution" == ValChapter)
      	ParseCommunicationWithInterpreter(node,result);

		}

  }
  catch(Exception& e)
  {
  }

  return result ;
}

bool 	AgentManager::ParseDescription(TiXmlNode* node, AgentScript* ag)
{
	TiXmlNode* node2;
	for( node2 = node->FirstChild(); node2; node2 = node2->NextSibling() )
	{
  		std::string ValName = node2->Value();
       if (!((ValName == "agent_name") || (ValName == "author") || (ValName == "version") || (ValName == "description_file")))
             throw Exception(std::string("create_agent_from_script"),std::string("nous nous attendons à description_file , version, agent_name, author  "), std::string(__FILE__), __LINE__);

      TiXmlElement*	element = node2->ToElement();
      if (ValName == "description_file")
      {
      		const char* result = element->Attribute((char*)"file");
          if (NULL != result)
          	ag->setDescriptionFile(std::string(result));
      }
      if (ValName == "agent_name")
      {
      		const char* agN = element->Attribute((char*)"name");
          if (agN != NULL)
          	ag->setAgentName(std::string(agN));
      }
      if (ValName == "author")
      {
      		const char* name = element->Attribute((char*)"first_name");
          if (NULL != name)
          	ag->setFirstName(std::string(name));

          const char* last = element->Attribute((char*)"last_name");
          if (NULL != last)
          ag->setLastName(std::string(last));

          const char* imail = element->Attribute((char*)"email");
          if (NULL != imail)
               ag->setEmail(std::string(imail));
      }
      if (ValName == "version")
      {
        const char* verN = element->Attribute((char*)"version_number");
        if (NULL != verN)
        	ag->setRealVersionType(std::string(verN));

        const char* typ = element->Attribute((char*)"type");
        if (NULL != typ)
        {
        	if (strcmp(typ, "OBSOLETE") == 0)
              ag->setVersionType(OBSOLETE);
          if (strcmp(typ, "RELEASE") == 0)
              ag->setVersionType(RELEASE);
          if (strcmp(typ, "RESEARCHS") == 0)
              ag->setVersionType(RESEARCHS);
          if (strcmp(typ, "DEVELOPPEMENT") == 0)
              ag->setVersionType(DEVELOPPEMENT);
        }


      }

  }
  return true ;
}

Level*
ParseLevel(TiXmlNode* node)
{
	Level* result = new Level() ;

  TiXmlElement*	elementImp = node->ToElement() ;
  std::string nameAg(elementImp->Attribute((char*)"nb")) ;
  TiXmlNode* nodeV = NULL;
  for( nodeV = node->FirstChild(); nodeV; nodeV = nodeV->NextSibling() )
  	{
      	std::string ValName = nodeV->Value();
        if ("node" == ValName)
        	{
          	TiXmlElement*	elementNode = nodeV->ToElement();
            std::string in = "";
            const char* res1 = elementNode->Attribute((char*)"in");
            if (NULL != res1)
            	in = std::string(res1);

            std::string out = "";
            const char* res2 = elementNode->Attribute((char*)"out");
            if (NULL != res2)
            		out = std::string(res2);

            TiXmlNode* nodeL = nodeV->FirstChild();
            if (NULL != nodeL)
            {
            	TiXmlText* resul =		nodeL->ToText();
            	const char* res6 = resul->Value();
           		result->addLevelNode( new TypedVal(res6), in, out);
            }
          }
    }

  return result ;
}

/*
** Fonction parsant la partie communication avec le controler du fichier XML
*/
bool
AgentManager::ParseCommunicationWithScript(TiXmlNode* node, AgentScript* ag)
{
	TiXmlNode* node2 = NULL;
  KsInitStructure* result;

	node2 = node->FirstChild( "agent_system_name" );
  if (node2)
  {
  	TiXmlElement*	elementImp = node2->ToElement() ;
    std::string nameAg(  elementImp->Attribute((char*)"name"));
    if ("" != nameAg)
       result = new KsInitStructure((char *)nameAg.c_str());
    else
    	return false;

    // Mettre le nom
    std::string KSType(  elementImp->Attribute((char*)"type"));
    if (KSType == "USER_KS")
    	result->putKsType(USER_KS) ;
  }

  // Recuperation obligatoire du nom systeme du KS

	for( node2 = node->FirstChild(); node2; node2 = node2->NextSibling() )
	{
  		std::string ValName = node2->Value();
       if (!((ValName == "agent_system_name") || (ValName == "trigger_condition") || (ValName == "required_condition") || (ValName == "validity_condition")|| (ValName == "agent_output")))
             throw Exception(std::string("create_agent_from_script"),std::string("nous nous attendons à description_file , agent_ouput, required_condition, trigger_condition ,agent_system_name ,validity_condition"), std::string(__FILE__), __LINE__);

      TiXmlElement*	element = node2->ToElement();
      if ("trigger_condition" == ValName)
      {
      	TiXmlNode* nodeTi = NULL;
      	for( nodeTi = node2->FirstChild(); nodeTi; nodeTi = nodeTi->NextSibling() )
        	{
          	std::string val = nodeTi->Value() ;
            if ("level" == val)
            {
            		Level* res = ParseLevel(nodeTi) ;
            		result->addTriggeredList(res) ;
            }
          }
      }
      if ("required_condition" == ValName)
      {
      	TiXmlNode* nodeTi = NULL;
      	for( nodeTi = node2->FirstChild(); nodeTi; nodeTi = nodeTi->NextSibling() )
        	{
          	std::string val =   nodeTi->Value();
            if ("level" == val)
            {
            		Level * res = ParseLevel(nodeTi);
            		result->addRequired(res);
            }
          }
      }
      if ("validity_condition" == ValName)
      {
      	TiXmlNode* nodeTi = NULL;
      	for( nodeTi = node2->FirstChild(); nodeTi; nodeTi = nodeTi->NextSibling() )
        	{
          	std::string val = nodeTi->Value() ;
            if ("level" == val)
            {
            		Level * res = ParseLevel(nodeTi);
            		result->addValidity(res);
            }
          }
      }
      if ("agent_ouput" == ValName)
      {
      	TiXmlNode* nodeTi = NULL;
      	for( nodeTi = node2->FirstChild(); nodeTi; nodeTi = nodeTi->NextSibling() )
        	{
          	std::string val =   nodeTi->Value();
            if ("node" == val)
            {
            		TiXmlElement*	elementNode = nodeTi->ToElement();
            		std::string in = "";
            		const char* res1 = elementNode->Attribute((char*)"in");
            		if (NULL != res1)
            			in = std::string(res1);

            		std::string out = "";
            		const char* res2 = elementNode->Attribute((char*)"out");
            		if (NULL != res2)
            		out = std::string(res2);

            		TiXmlNode* nodeL = nodeTi->FirstChild();
            		if (nodeL)
            		{
            			TiXmlText* resul =		nodeL->ToText();
            			const char* res6 = resul->Value();
           				result->addResult( new TypedVal(res6), in, out);
            		}
            }
          }
      }
  }

  ag->setPublicationVar(result);
  return true;
}

/*
** Fonction parsant la partie liant l'interpreteur et le KSG
*/
bool 	AgentManager::ParseCommunicationWithInterpreter(TiXmlNode* node, AgentScript* ag)
{
	TiXmlNode* node2 = NULL;
  TiXmlElement*	element = node->ToElement();

	for( node2 = node->FirstChild(); node2; node2 = node2->NextSibling() )
	{
  		std::string ValName = node2->Value();
      if (!(ValName == "function"))
             throw Exception(std::string("create_agent_from_script"),std::string("nous nous attendons à function"), std::string(__FILE__), __LINE__);

  		if (ValName == "function")
      	{
        	TiXmlElement*	element = node2->ToElement();
        	const char* res1 = element->Attribute((char*)"type");
  				if (NULL == res1) return false;

          std::string func = "";
          const char* res2 = element->Attribute((char*)"func_to_call");
          if (NULL != res2)
          	func = std::string(res2);

          std::string var = "";
          const char* res3 = element->Attribute((char*)"var");
          if (NULL != res3)
          	var = std::string (res3);

					ag->InsertLink(std::string(res1), new InterpreterLink(func, var ));
        }
	}
  return true;
}

std::string LoadFile(std::string file)
{
  std::ifstream fil(file.c_str()) ;
  std::string buffer = std::string("") ;
  if ( fil )
  {
    std::string ligne ;
    while (std::getline(fil, ligne))
      buffer += ligne + "\n" ;
  }
  return buffer ;
}

/*
** Fonction parsant la partie code du script
*/
bool 	AgentManager::ParseCode(TiXmlNode* node, AgentScript* ag)
{
	std::string buffer= "" ;
	TiXmlNode* node2 = NULL;
	for( node2 = node->FirstChild(); node2; node2 = node2->NextSibling() )
	{
  		std::string ValName = node2->Value();
      if (!((ValName == "code_to_add") || (ValName == "file") ||(ValName == "type_to_define") ))
             throw Exception(std::string("create_agent_from_script"),std::string("nous nous attendons à file , type_to_define, code_to_add"), std::string(__FILE__), __LINE__);

      TiXmlElement*	element = node2->ToElement();
      if ( "file" == ValName)
      {
      		const char* res1 = element->Attribute((char*)"name");
          if (res1)
          		{
              	std::string temp(res1);
          			buffer += LoadFile(temp);
              }
      }

      if ( "code_to_add" == ValName)
      {
      		TiXmlNode* nodeL = node2->FirstChild();
          if (nodeL)
          {
              TiXmlText* resul =		nodeL->ToText();
              const char* res6 = resul->Value();
              if (NULL != res6)
              {
              	std::string test(res6);
                buffer += test;
              }
          }
      }
	}

  return true;
}

