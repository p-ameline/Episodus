// -----------------------------------------------------------------------------// nsPapyrus.h
// -----------------------------------------------------------------------------
// Création d'une page HTML multi-documents
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: pameline $
// $Date: 2015/02/07 14:35:31 $
// -----------------------------------------------------------------------------
// PA - 05/2012
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

#ifndef __NSPAPYRUS_H__
# define __NSPAPYRUS_H__

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "nautilus\nssuper.h"

# include "nsepisod\episod_export.h"

// -----------------------------------------------------------------------------
//
// Classe NSPapyrusDocument
// Multi-documents XML file management
//
// -----------------------------------------------------------------------------
class _EPISODUS NSPapyrusDocument : public NSRoot
{
  public:

    // Constructeur - Destructeur
    NSPapyrusDocument(NSContexte *pCtx, string sIndex, NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray *pPatPatho, string sDocTxt) ;    NSPapyrusDocument(const NSPapyrusDocument& rv) ;    ~NSPapyrusDocument() ;

    // Operateurs
    NSPapyrusDocument& operator=(const NSPapyrusDocument& src) ;
    int		             operator==(const NSPapyrusDocument& o) ;

    string           getIndex()        { return _sIndex ; }
    NSDocumentInfo*  getDocument()     { return _pNSDocumentInfo ; }
    NSPatPathoArray* getPatPatho()     { return _pPatPatho ; }
    string           getDocumentText() { return _sDocumentText ; }

    void setDocument(NSDocumentInfo *pNSDocumentInfo) ;
    void setPatPatho(const NSPatPathoArray *pPatPatho) ;
    void setDocText(string sTxt) { _sDocumentText = sTxt ; }
    void addText(string sTxt)    { _sDocumentText += sTxt ; }

    void updateDocument(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray *pPatPatho, string sDocTxt) ;

  protected:

    string          _sIndex ;

    NSDocumentInfo  *_pNSDocumentInfo ;
    NSPatPathoArray *_pPatPatho ;
    string          _sDocumentText ;
} ;

//
// Definition of NSPapyrusDocument arrays objects
//
typedef vector<NSPapyrusDocument*>  NSPapyrusDocArray ;
typedef NSPapyrusDocArray::iterator NSPapyrusDocumentIter ;
typedef NTArray<NSPapyrusDocument>  NSPapyrusDocumentsArray ;

// -----------------------------------------------------------------------------
//
// Classe NSPapyrus
// Multi-documents XML file management
//
// -----------------------------------------------------------------------------
class _EPISODUS NSPapyrus : public NSRoot
{
  public:

    // Constructeur - Destructeur
    NSPapyrus(NSContexte *pCtx, string sFileName, string sTitle) ;    ~NSPapyrus() ;

    void ReinitLogPage() ;
    void ReinitLogFile() ;
    void AppendLogFile() ;
    void addToLogPage(string sAdd, string sBefore = string(""), string sAfter = string("")) ;

    void addTextToDoc(string sIndex, string sText) ;

    void LogDocument(string sIndex, NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray* pNSPatPathoArray, string sHtmlText) ;

    NSPapyrusDocument *getDocumentForId(string sIndex) ;
    string             getDivBlockForDoc(NSPapyrusDocument *pPapyrusDoc) ;

    string getLogPage() ;
    string getPageTitle() ;
    string getLogFile() { return _sLogFile ; }
    bool   wasLogPageModified() { return _bLogPageModified ; }

  protected:

    // string _sLogPage ;
    bool   _bLogPageModified ;

    NSPapyrusDocumentsArray _aPapyrusDocuments ;

    string _sTitle ;
    string _sFileName ;
    string _sLogFile ;

    string _sPapyrusDocumentID ;  // Document ID when Papyrus is saved
} ;

#endif // !__NSPAPYRUS_H__
