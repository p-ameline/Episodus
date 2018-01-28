// -----------------------------------------------------------------------------
// nsgraphe.h
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// $Revision: 1.8 $
// $Author: pameline $
// $Date: 2013/11/25 16:07:09 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à [rappeler les
// caractéristiques techniques de votre logiciel].
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
// This software is a computer program whose purpose is to [describe
// functionalities and technical features of your software].
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

#ifndef __NSGRAPHE_H__
# define __NSGRAPHE_H__

class NSDataGraph ;
class VectString ;
class NSPatPathoArray ;
class NSPatLinkArray ;
class VecteurString ;
class NSPatPathoInfo ;

// # include "nssavoir\nsvarray.h"
// # include "nssavoir\nspatlnk.h"
// # include "nsdn\nsdocnoy.h"

# include <vector>
# ifndef _NO_BDE
#   include <bde.hpp>
# else
    typedef char DBITBLNAME[261];
# endif

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "nssavoir/nsRootLink.h"
# include "nsbb/nsmanager.h"

#ifndef __linux__
# if defined(_DANSLEXIDLL)
#  define _CLASSELEXI __export
# else
#  define _CLASSELEXI __import
# endif
#endif

// -----------------------------------------------------------------------------
// Important note about Doxygen tag
// -----------------------------------------------------------------------------
// All comments begining with "//!" or "//!<" must not be deleted, they concerns
// Doxygen doc generator.
// If you modify some classes and/or methods and/or variables please modify also
// the doxygen available informations.
// -----------------------------------------------------------------------------

// classe de gestion des liens par un NSDataGraph (en MUE / N_TIERS)
#ifndef __linux__
class _CLASSELEXI NSLinkManager : public NSRootLink, public NSSuperRoot
#else
class NSLinkManager : public NSRootLink, public NSSuperRoot
#endif
{
  public:

    // objet d'acces au graphe
    NSDataGraph* pGraph ;

    // Constructeur et destructeur
    NSLinkManager(NSSuper* pSuper, NSDataGraph* pDataGraph) ;
    ~NSLinkManager() ;
    NSLinkManager(const NSLinkManager& rv) ;
    NSLinkManager& operator=(const NSLinkManager& src) ;

    // savoir si sCode1 et sCode2 sont liés par sRelation
    // note : le lien inverse est donné par sensCle = "ENVERS"
    bool    VraiOuFaux(std::string sNoeud1, NODELINKTYPES iRelation, std::string sNoeud2, DBITBLNAME sensCle = "FLECHE") ;

    // mettre tous les items liés à sCode1 par sRelation dans pVecteurString ou pLinkArray
    // note : le lien inverse est donné par sensCle = "ENVERS"
    void    TousLesVrais(std::string sNoeud, NODELINKTYPES iRelation, VecteurString *pVecteurString, DBITBLNAME sensCle = "FLECHE") ;
    void    TousLesVraisDocument(std::string sDocRoot, NODELINKTYPES iRelation, VecteurString *pVecteurString, DBITBLNAME sensCle = "FLECHE") ;

    void    TousLesVrais(std::string sNoeud, NODELINKTYPES iRelation, NSPatLinkArray *pLinkArray, DBITBLNAME sensCle = "FLECHE") ;
    // mettre tous les items liés par iRelation dans pNoeudParam et leurs noeuds liés dans pNoeudResult
    void    TousLesVrais(std::string sTreeID, NODELINKTYPES iRelation, VecteurString *pNoeudParam, VecteurString *pNoeudResult, DBITBLNAME sensCle = "FLECHE") ;
    void    TousLesVrais(std::string sPatientID, NSPatLinkArray *pLinkArray, DBITBLNAME sensCle = "FLECHE") ;

    bool    etablirLien(std::string sQualifie, NODELINKTYPES iRelation, std::string sQualifiant) ;
    bool    etablirLienDocument(std::string sQualifie, NODELINKTYPES iRelation, std::string sQualifiant) ;

    bool    detruireTousLesLiens(std::string sQualifie, NODELINKTYPES iRelation) ;
    bool    detruireTousLesLiensDocument(std::string sDocRoot, NODELINKTYPES iRelation) ;
    bool    detruireTousLesLiens(std::string sNoeud) ;
    bool    detruireTousLesLiens() ;
    bool    detruireLien(std::string sQualifie, NODELINKTYPES iRelation, std::string sQualifiant) ;

    bool    existeLien(std::string sQualifie, NODELINKTYPES iRelation, std::string sQualifiant) ;

    bool    swapLiens(std::string sOldNode, NSPatPathoInfo* pNewPpt) ;
    string  nextDocToSave() ;
    int     numberDirtyLinksToDoc(std::string sTreeID) ;

    string  giveMetaDataRoot(std::string sDocRoot) ;

  protected:

    bool    etablirLien(std::string sQualifie, std::string sLink, std::string sQualifiant) ;
} ;

// classe de liaison noeud-archetype
class _CLASSELEXI NSArcNode
{
  public:

    string _sNode ;
    string _sArc ;
    int    _iEtat ;

    NSArcNode(string sNode, string sArc, int iEtat = 1) ;
    ~NSArcNode() ;
    NSArcNode(const NSArcNode& rv) ;
    NSArcNode& operator=(const NSArcNode& src) ;

    string getNode()  { return _sNode ; }
    string getArc()   { return _sArc ;  }
    int    getState() { return _iEtat ; }

    void   setNode(string sN) { _sNode = sN ; }
    void   setArc(string sA)  { _sArc  = sA ; }
    void   setState(int iS)   { _iEtat = iS ; }
} ;

// array de liaison
typedef vector<NSArcNode *>             NSArcNodeVector ;
typedef NSArcNodeVector::iterator       NSArcNodeIter ;
typedef NSArcNodeVector::const_iterator NSArcNodeConstIter ;

class _CLASSELEXI NSArcNodeArray : public NSArcNodeVector
{
  public:

    // Constructeurs
    NSArcNodeArray() : NSArcNodeVector() {}
    NSArcNodeArray(const NSArcNodeArray& rv) ;
    NSArcNodeArray& operator=(const NSArcNodeArray& src) ;

    // Destructeur
    virtual ~NSArcNodeArray() ;
    void vider() ;
} ;

class _CLASSELEXI NSArcManager : public NSSuperRoot
{
  public:

    enum TYPEARC { archetype = 1, referentiel, decisionTree } ;

    NSArcManager(NSSuper* pSuper) ;
    NSArcManager(const NSArcManager& src) ;
    ~NSArcManager() ;

    NSArcManager& operator=(const NSArcManager& src) ;

    bool            OuvreLibrairie() ;
    std::string     DonneNomArchetypeLie(TYPEARC type, std::string sNode, NSPersonGraphManager* pGraphPerson) ;
    std::string     DonneNomArchetypeDansLibrairie(TYPEARC type, std::string sNodeArc) ;
    std::string     DonneFichierArchetypeParNom(TYPEARC type, std::string sNomArc, bool bAvecChemin = true) ;
    bool            ExisteFichierArchetype(std::string sFichArc, std::string sPathArc, std::string& sNomArc, TYPEARC& type) ;
    std::string     AjouteArchetype(NSContexte *pContexte, TYPEARC type, std::string sNomArc, std::string sFichArc, std::string sPathArc = "", std::string sType = "", std::string sCategory = "", std::string sLabel = "", std::string sFrame = "") ;
    void            AjouteIdentifiantArchetype(TYPEARC type, std::string sNomArc, std::string sFichArc) ;
    std::string     DonneNoeudArchetype(TYPEARC type, std::string sNomArc) ;

  protected:

    NSDataTreeArray	_aModelArray ;
} ;

string _CLASSELEXI ArchetypeGetID(NSSuper *pSuper, NSDataTree *pTree, std::string sType) ;
string _CLASSELEXI ArchetypeGetFile(NSSuper *pSuper, NSDataTree *pTree, std::string sType) ;
string _CLASSELEXI ArchetypeGetProperty(NSSuper *pSuper, NSDataTree *pTree, std::string sType, std::string sPropSens) ;

bool _CLASSELEXI ArchetypeSortByIDInf(NSDataTree *pTree1, NSDataTree *pTree2) ;
bool _CLASSELEXI ArchetypeSortByIDSup(NSDataTree *pTree1, NSDataTree *pTree2) ;
bool _CLASSELEXI ArchetypeSortByFileInf(NSDataTree *pTree1, NSDataTree *pTree2) ;
bool _CLASSELEXI ArchetypeSortByFileSup(NSDataTree *pTree1, NSDataTree *pTree2) ;

#endif // !__NSGRAPHE_H__

