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

#include "ns_ob1\Interface.h"
#include "ns_ob1\BB1Task.h"
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\BB1Object.h"
#include "ns_ob1\BB1Class.h"
#include "nautilus\nssuper.h"
#include "ns_ob1\BB1Task.h"
#include "ns_ob1\ns_bbk.h"
#include "ns_ob1\OB1Node.h"
#include "ns_ob1\InterfaceExt.h"
#include "ns_ob1\OB1Edge.h"
#include "OB1.rh"

/*******************************************************************************
********************************* Graphe de KS *********************************
*******************************************************************************/

NSOB1NodesTreeControl::NSOB1NodesTreeControl(TWindow* parent, BB1BBInterface * inter, OB1NKS* pKs, int id, int x, int y,
                     int w, int h, TStyle style, TModule* module)
                     :TTreeWindow(parent, id, x, y, w, h, style, module)
{
	_pKs       = pKs ;
	_interface = inter ;

  _pImages   = 0 ;

	uint32 ui32Style = GetStyle() ;
	ui32Style |= TTreeWindow::twsHasLines | TTreeWindow::twsHasButtons | TTreeWindow::twsEditLabels ;
#if defined(BI_PLAT_WIN32)
	ui32Style |= TTreeWindow::twsLinesAtRoot ;
#endif
	SetStyle(ui32Style) ;
}

NSOB1NodesTreeControl::~NSOB1NodesTreeControl()
{
	if (_pImages)
  	delete _pImages ;
}

void
NSOB1NodesTreeControl::SetupWindow()
{
	_pImages = new TImageList(NS_CLASSLIB::TSize(20, 16), ILC_COLOR4, 15, 5) ;

  iKSvalidityIndex = 0 ;

	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_NO_COMPUTATION)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_VALID)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_PRPOSABLE)) ;
  _pImages->Add(OWL::TBitmap(*pNSResModule, IDB_INVALID)) ;

  iEdgeTypeIndex = 4 ;

	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_CREATES)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_IS_CREATED)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_IMPLIES)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_REQUIRES)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_IS_NEEDED)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_TRIGERS)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_RESULT)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_LEVEL)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_UNDEFINED)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_IS_VALIDATED)) ;
	_pImages->Add(OWL::TBitmap(*pNSResModule, IDB_VALIDATED)) ;

  iRequiresIndex = 15 ;

  _pImages->Add(OWL::TBitmap(*pNSResModule, IDB_REQUIRES_BAD)) ;
  _pImages->Add(OWL::TBitmap(*pNSResModule, IDB_REQUIRES_OK)) ;
  _pImages->Add(OWL::TBitmap(*pNSResModule, IDB_REQUIRES_WAIT)) ;

	SetImageList(TTreeWindow::Normal, *_pImages) ;

	TTreeWindow::SetupWindow() ;
  addRootNode() ;
}

void
NSOB1NodesTreeControl::addRootNode()
{
	std::string sNodeString = _pKs->getString() ;

	TTreeNode root = GetRoot() ;
  TTreeNode theNode(*this, sNodeString.c_str()) ;
  theNode = root.AddChild(TTreeNode(*this, sNodeString.c_str())) ;

	addSubNodes((OB1Node*) _pKs, &theNode) ;
}

void
NSOB1NodesTreeControl::addSubNodes(OB1Node* pOBNode, TTreeNode* pTreeNode)
{
	if (!pOBNode)
		return ;

  std::vector<OB1NodeLink* >* pLinksArray = pOBNode->getChildrenPtr() ;
  if (!pLinksArray || (pLinksArray->empty()))
  	return ;

	std::vector<OB1NodeLink*>::iterator iter = pLinksArray->begin() ;
	for ( ; iter != pLinksArray->end(); iter++)
	{
  	OB1Node* pNode = (*iter)->Node() ;
    std::string sNodeString = pNode->getString() ;

    TTreeNode theNode(*this, sNodeString.c_str()) ;
    theNode = pTreeNode->AddChild(TTreeNode(*this, sNodeString.c_str())) ;

    OB1Edge* pEdge = (*iter)->Edge() ;
    setNodeValues(pNode, pEdge, &theNode) ;

    addSubNodes(pNode, &theNode) ;
	}
}

void
NSOB1NodesTreeControl::setNodeValues(OB1Node* pOBNode, OB1Edge* pOBEdge, TTreeNode* pTreeNode)
{
	OB1EdgeType edgeType = pOBEdge->TypeOf() ;

  int iLocalIndex = 0 ;

  OB1NOther* pOther = NULL ;

  switch (edgeType)
  {
  	case CREATES      : iLocalIndex = 0 ; break ;
  	case IS_CREATED   : iLocalIndex = 1 ; break ;
  	case IMPLIES      : iLocalIndex = 2 ; break ;
  	case REQUIRES     :

    	iLocalIndex = 3 ;

      pOther = TYPESAFE_DOWNCAST(pOBNode, OB1NOther) ;
      if (pOther)
      {
      	int clas = pOBEdge->Class() ;
        if (!(pOther->isActivableExt(clas)))
      		iLocalIndex = iRequiresIndex - iEdgeTypeIndex ;
      }

      break ;
  	case IS_NEEDED    : iLocalIndex = 4 ; break ;
  	case TRIGERS      : iLocalIndex = 5 ; break ;
  	case RESULT       : iLocalIndex = 6 ; break ;
  	case LEVEL        : iLocalIndex = 7 ; break ;
  	case UNDEFINED    : iLocalIndex = 8 ; break ;
  	case IS_VALIDATED : iLocalIndex = 9 ; break ;
  	case VALIDATED    : iLocalIndex = 10 ; break ;
  }

  pTreeNode->SetImageIndex(iLocalIndex + iEdgeTypeIndex) ;
  pTreeNode->SetSelectedImageIndex(iLocalIndex + iEdgeTypeIndex, true) ;
}

/*******************************************************************************
******** Implementation de fenetre gérant l'interface KS graph *****************
*******************************************************************************/

DEFINE_RESPONSE_TABLE1(OB1NodesInterfaceContainer, TMDIChild)
    EV_WM_DESTROY,
    EV_WM_SIZE,
    EV_WM_CLOSE,
END_RESPONSE_TABLE ;

OB1NodesInterfaceContainer::OB1NodesInterfaceContainer(TMDIClient& parent, NSContexte* pContexte, NSOB1NodesTreeControl* pControl)
                           :TMDIChild(parent, "KS graph")
{
	_control   = pControl ;
	_pContexte = pContexte ;
	//Attr.Style = WS_CHILD |WS_THICKFRAME	| WS_VISIBLE ;
	Attr.X = 0 ;
	Attr.Y = 0 ;
	Attr.H = 700 ;
	Attr.W = 300 ;
}

OB1NodesInterfaceContainer::~OB1NodesInterfaceContainer()
{
}

void
OB1NodesInterfaceContainer::SetupWindow()
{
	TMDIChild::SetupWindow();
  SetClientWindow(_control);
}

void
OB1NodesInterfaceContainer::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TMDIChild::EvSize(sizeType, size) ;
}

void
OB1NodesInterfaceContainer::EvDestroy()
{
}

bool
OB1NodesInterfaceContainer::CanClose()
{
  return true ;
}


