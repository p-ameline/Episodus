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
#include "ns_ob1\BB1Object.h"
#include "ns_ob1\BB1Ks.h"
#include "ns_ob1\OB1Token.h"
#include "ns_ob1\OB1Node.h"
#include "ns_ob1\OB1Heuristix.h"
#include "ns_ob1\OB1Strategy.h"

HeuristixGenerator::HeuristixGenerator(OB1Controler* control)
                   :_createStrategy(control)
{
  _controler = control ;
}

OB1Controler*
HeuristixGenerator::Controler()
{
  return _controler ;
}

HeuristixGenerator::~HeuristixGenerator()
{
}

Heuristix*
HeuristixGenerator::getHeuristix()
{
  return &_createStrategy ;
}

DFSHeuristix::DFSHeuristix(OB1Controler* control)
             :Heuristix(control)
{
}

OB1Node*
DFSHeuristix::NextNode(std::vector<OB1Node*>& explore, OB1Strategy* pRootStrat)
{
	if (explore.empty())
		return (OB1Node*) 0 ;

	sortPossibleNodes(explore, pRootStrat) ;

  return chooseBestNode(explore, pRootStrat) ;

  // Ancien algo :)
  // if (explore.size() > 0)
  //  return (explore[0]);
}

// eliminates nodes from already tried KSs
//
void
DFSHeuristix::sortPossibleNodes(std::vector<OB1Node* >& explore, OB1Strategy* pRootStrat)
{
	OB1Token* pToken = pRootStrat->Token() ;
  if (NULL == pToken)
  	return ;

	BB1Object* pAnswer = pToken->getStatusObject() ;
  if (NULL == pAnswer)
  	return ;

  AnswerStatus* pAnswerStatus = pAnswer->Value("answerStatus", NULLLANSWERSTATUS) ;
  if (NULL == pAnswerStatus)
		return ;

	std::vector<OB1Node*>::iterator nodesIter = explore.begin() ;
  for ( ; explore.end() != nodesIter ; )
	{
  	bool bKeepIt = false ;

    OB1NKS* pKsNode = (OB1NKS*) 0 ;

    OB1Node* pTheNode = *nodesIter ;

    // Processing intermediate NAnd node
    //
    OB1NAnd* pNand = dynamic_cast<OB1NAnd*>(pTheNode) ;
    if (pNand)
    {
      std::vector<OB1NodeLink*>* pParents = &(pNand->getParent()) ;
      pTheNode = (*pParents)[0]->Node() ;
    }

  	// First step, we only accept OB1NResult or OB1NTrigger nodes
    //
    OB1NResult* pResult = dynamic_cast<OB1NResult*>(pTheNode) ;
    if (pResult)
    	pKsNode = pResult->KS() ;
    else
    {
    	OB1NTrigger* pTrigger = dynamic_cast<OB1NTrigger*>(pTheNode) ;
    	if (pTrigger)
      	pKsNode = pTrigger->KS() ;
    }


		// Second step, we check if this KS has already been tried during the
    // current cycle
    //
    if (pKsNode)
    {
    	std::string sKsName = pKsNode->getKSName() ;
      KSAttemptHistoryElement* pHistoElement = pAnswerStatus->getHistoryElement(sKsName) ;
      if (NULL == pHistoElement)
      	bKeepIt = true ;
    }

    if (bKeepIt)
    	nodesIter++ ;
    else
    	explore.erase(nodesIter) ;
  }
}

OB1Node*
DFSHeuristix::chooseBestNode(std::vector<OB1Node* >& explore, OB1Strategy* /* pRootStrat */)
{
	if (explore.empty())
		return (OB1Node*) 0 ;

	OB1Node* pBestCandidate   = (OB1Node*) 0 ;
  int      iCurrentPriority = 0 ;

  std::vector<OB1Node*>::iterator nodesIter = explore.begin() ;
  for ( ; explore.end() != nodesIter ; nodesIter++)
	{
		OB1NKS* pKsNode = (OB1NKS*) 0 ;

    OB1Node* pTheNode = *nodesIter ;

    // Processing intermediate NAnd node
    //
    OB1NAnd* pNand = dynamic_cast<OB1NAnd*>(pTheNode) ;
    if (pNand)
    {
      std::vector<OB1NodeLink*>* pParents = &(pNand->getParent()) ;
      pTheNode = (*pParents)[0]->Node() ;
    }

  	// First step, we only accept OB1NResult or OB1NTrigger nodes
    //
    OB1NResult* pResult = dynamic_cast<OB1NResult*>(pTheNode) ;
    if (pResult)
    	pKsNode = pResult->KS() ;
    else
    {
    	OB1NTrigger* pTrigger = dynamic_cast<OB1NTrigger*>(pTheNode) ;
    	if (pTrigger)
      	pKsNode = pTrigger->KS() ;
    }

    if (pKsNode)
    {
    	BB1KS* pCurrentKs = pKsNode->getKS() ;

      if (pCurrentKs)
      {
      	int iKsPriority = pCurrentKs->getPriority() ;

        if ((NULL == pBestCandidate) || (iKsPriority > iCurrentPriority))
        {
        	pBestCandidate   = *nodesIter ;
          iCurrentPriority = iKsPriority ;
				}
      }
    }
  }

  return pBestCandidate ;
}

