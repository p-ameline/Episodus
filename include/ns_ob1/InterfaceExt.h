// -----------------------------------------------------------------------------
// InterfaceExt.h
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 16:52:41 $
// -----------------------------------------------------------------------------
// DG  - october 2004
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

#ifndef __INTERFACE_EXT_H__
# define __INTERFACE_EXT_H__

class OB1NKS ;
class NSOB1NodesTreeControl ;
class OB1Edge ;

# include <owl\mdichild.h>    // Fenetre MDI client
# include <owl\listwind.h>    // List
# include <owl\listbox.h>
# include <owl\dialog.h>
# include <owl\window.h>
# include <owl\tabctrl.h>
# include <owl\commctrl.h>
# include <owl\listview.h>
# include <owl\treewind.h>

# ifndef _DANSBBKDLL
#  define _BBKEXPORT __import
# else
#  define _BBKEXPORT __export
# endif

class _BBKEXPORT OB1NodesInterfaceContainer : public TMDIChild
{
 public:

  //! \brief constructor
  //!
  //! constructor
  //! @param MDI parent windows
  //! @param Pointer to context
  //! @param inter interaface dialog box to set in the client windows
  OB1NodesInterfaceContainer(TMDIClient& parent, NSContexte* pContexte, NSOB1NodesTreeControl* pControl) ;

  //! \brief destructor
  //!
  //! Destructor
  ~OB1NodesInterfaceContainer() ;

  void EvSize(uint sizeType, ClassLib::TSize& size) ;
  void SetupWindow() ;
  void EvDestroy() ;
	bool CanClose() ;

 private:

  NSOB1NodesTreeControl * _control ;
  NSContexte *            _pContexte ;  //!< Context of interface \brief Context of interface

  DECLARE_RESPONSE_TABLE(OB1NodesInterfaceContainer) ;
} ;

//
// Objet TreeWindow d'affichage des KSNodes
//
class NSOB1NodesTreeControl : public TTreeWindow
{
	public:

    NSOB1NodesTreeControl(TWindow* parent, BB1BBInterface* inter, OB1NKS* pKs, int id, int x, int y,
                     int w, int h, TStyle style = twsNone, TModule* module = 0) ;

		~NSOB1NodesTreeControl() ;

  	BB1BBInterface* getInterface() { return _interface ; }
  	OB1NKS*         getKS()        { return _pKs ; }

    void SetupWindow() ;

	private:

  	OB1NKS*                _pKs ;
    BB1BBInterface *       _interface ;  //!< Pointer to interface \brief Pointer to interface

  	OB1InterfaceContainer* _pOb1Container ;
    TImageList*            _pImages ;

    int                    iKSvalidityIndex, iEdgeTypeIndex, iRequiresIndex ;

    void addRootNode() ;
    void addSubNodes(OB1Node* pOBNode, OWL::TTreeNode* pTreeNode = NULL) ;

    void setNodeValues(OB1Node* pOBNode, OB1Edge* pOBEdge, OWL::TTreeNode* pTreeNode) ;
} ;

#endif // !__INTERFACEBB_H__

