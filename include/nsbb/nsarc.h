// -----------------------------------------------------------------------------
// nsarc.h
// -----------------------------------------------------------------------------
// Parseur d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.7 $
// $Author: pameline $
// $Date: 2014/03/28 15:52:16 $
// -----------------------------------------------------------------------------
// FLP - april 2005
// RS  - november 2002
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

#ifndef __NSARC_H__
# define __NSARC_H__

# include <string>

class NSCapture ;
class NSCaptureArray ;

# include "nsbb\nsattval.h"

string TexteXmlVersNormal(string texte) ;

class _NSBBCLASSE nsGenericParseur
{
  private:

    static long lGenericObjectCount ;

	public:

    nsGenericParseur(NSSuper* pSuper, bool bVerb = true) ;
    virtual ~nsGenericParseur() ;

    virtual bool open(string sXmlFileName) = 0 ;

    void getGroups(VecteurString* pGroups, string sLang) ;

    Cbalise* getRootBalise()                 { return _pRootBalise ; }
    void     setRootBalise(Cbalise* pBalise) { _pRootBalise = pBalise ; }

    NSSuper* getSuper() { return _pSuper ; }

    bool     isParsingOk()          { return _bParsingOk ; }
    void     setParsingOk(bool bPO) { _bParsingOk = bPO ; }

    bool     isVerbose()            { return _bVerbose ; }
    void     setVerbose(bool bVb)   { _bVerbose = bVb ; }

    static long getGenericNbInstance() { return lGenericObjectCount ; }
    static void iniGenerictNbInstance() { lGenericObjectCount = 0 ; }

  protected:

    Cbalise* _pRootBalise ;
    NSSuper* _pSuper ;
    bool     _bParsingOk ;
    bool     _bVerbose ;
} ;

class _NSBBCLASSE nsarcParseur : public nsGenericParseur
{
  private:

    static long lObjectCount ;

	public:

    nsarcParseur(NSSuper* pSuper, bool bVerb = true) ;
    ~nsarcParseur() ;
    nsarcParseur(const nsarcParseur& rv) ;
    nsarcParseur& operator=(const nsarcParseur& src) ;

    bool open(const string sXmlFileName) ;
    bool parse_archetype(string message, string tag, int* iParsingError) ;

    bool createArchetypedPpt(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo) ;
    bool updateArchetypedPpt(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo) ;

    bool fillSonNodes(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo, Citem* pFather, string sFatherPath) ;
    bool updateSonNodes(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo, Citem* pFather, string sFatherPath) ;

    void addCapturedNode(NSCapture* pCaptureEntry, Citem* pSonItem, NSPatPathoArray* pPPT, string sPath) ;
    void updateCapturedNode(NSCapture* pCaptureEntry, Citem* pSonItem, NSPatPathoArray* pPPT, string sPath) ;

    Carchetype* getArchetype() { return (Carchetype*) _pRootBalise ; }
    void        setArchetype(Carchetype* pArch) { _pRootBalise = pArch ; }

    static long getNbInstance() { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

class _NSBBCLASSE nsrefParseur : public nsGenericParseur
{
  private:

    static long lObjectCount ;

	public:

    nsrefParseur(NSSuper* pSuper, bool bVerb = true) ;
    ~nsrefParseur() ;

    bool open(string sXmlFileName) ;
    bool parse_referentiel(string message, string tag, int* iParsingError) ;

    void getGroups(VecteurString* pGroups, string sLang) ;

    Creferentiel* getReferentiel() { return (Creferentiel*) _pRootBalise ; }

    static long getNbInstance() { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

class _NSBBCLASSE nsGuidelineParseur : public nsGenericParseur
{
  private:

    static long lObjectCount ;

	public:

    nsGuidelineParseur(NSSuper* pSuper, bool bVerb = true) ;
    ~nsGuidelineParseur() ;

    bool open(string sXmlFileName) ;
    bool parse_guideline(string message, string tag, int* iParsingError) ;

    Cguideline* getGuideline() { return (Cguideline*) _pRootBalise ; }

    static long getNbInstance() { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

class _NSBBCLASSE NSValidateur
{
  private:

    static long lObjectCount ;

	public:

		Ccontrainte*     _pValidity ;
  	NSContexte*      _pContexte ;
    NSPatPathoArray* _pLocalPatho ;

  	NSValidateur(Ccontrainte* pValid, NSContexte* pCtx, NSPatPathoArray* pLocPat = 0) ;
  	~NSValidateur() ;

  	bool        getValeurVariable(string sName, string& sAlias, NSPatPathoArray** ppPatPathoArray) ;
  	bool        getValeurContrainte(string sName = "") ;
  	bool        AnalyseExpression(string sExp) ;
  	bool        carAutorise(char c, bool debut = false) ;
  	int         Interprete(string sReq, size_t& cc) ;
  	string      getValidator() ;
  	bool        Validation() ;
    bool        Validation(string sValidator) ;
  	Cglobalvars *getGlobalVars() ;

    void        displayThinkingCursor() ;
    void        displayNormalCursor() ;

    void        setGlobalVars(Cglobalvars *pGlobalVars) { _pGlobalVars = pGlobalVars ; }

    static long getNbInstance() { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

  protected:

    Cglobalvars *_pGlobalVars ;
} ;

class _NSBBCLASSE NSDrufRefFileManager
{
  private:

    static long lObjectCount ;

	protected:

  	nsrefParseur* _pRefParseur ;

    string        _sFullPathFileName ;

	public:

		NSDrufRefFileManager() ;
    ~NSDrufRefFileManager() ;

    bool writeFile() ;

    static long getNbInstance() { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

#endif // !__NSARC_H__

