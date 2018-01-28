// -----------------------------------------------------------------------------
// nsattval.h
// -----------------------------------------------------------------------------
// Attributs/Valeurs d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.13 $
// $Author: pameline $
// $Date: 2015/02/07 14:30:47 $
// -----------------------------------------------------------------------------
// FLP - april 2005
// FLP - july 2003
// RS  - july 2003
// FLP - june 2003
// RS  - november 2002// -----------------------------------------------------------------------------

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

#ifndef __NSATTVAL_H__
# define __NSATTVAL_H__

# include <Services\defs.h>
# include <vector.h>
# include <string>

class NSSuper ;
class NSPatPathoArray ;
class NSPatPathoInfo ;
class NSContexte ;
class VecteurString ;

class Csteps ;
class CguidelineStep ;

# include "balise.h"
// # include "nautilus\nssuper.h"
// # include "nsbb\nspatpat.h"
# include "partage\NTArray.h"

# define LABEL_ARCHETYPE             "archetype"
# define LABEL_DIALOGUE              "dialog"
# define LABEL_REFERENCES            "references"
# define LABEL_CONCERN               "concern"
# define LABEL_HEAD                  "head"
# define LABEL_ITEMS                 "items"
# define LABEL_ITEM                  "item"
# define LABEL_CONTRAINTES           "constraints"
# define LABEL_CONTRAINTE            "constraint"
# define LABEL_REFERENTIEL           "referential"
# define LABEL_GUIDELINE             "guideline"
# define LABEL_PROPOSITION           "proposition"
# define LABEL_VALIDITE              "validity"
# define LABEL_VALIDATEUR            "validator"
# define LABEL_FRIENDSGROUP          "friendsgroup"
# define LABEL_EXCLUDE               "excludes"
# define LABEL_NEED                  "needs"
# define LABEL_FRIEND                "friend"
# define LABEL_VARIABLE              "var"
# define LABEL_ALIAS                 "alias"
# define LABEL_LOCAL_ALIAS           "local_alias"
# define LABEL_TREE                  "tree"
# define LABEL_NODE                  "node"
# define LABEL_GLOBALVARS            "globalvars"
# define LABEL_DIALOGBOX             "dialogbox"
# define LABEL_DIALOGBOX_CONTROL     "control"
# define LABEL_DIALOGBOX_RCDATA      "rcdata"
# define LABEL_DIALOGBOX_ONGLETS     "tabcontrol"
# define LABEL_ONGLET                "tab"
# define LABEL_PRESENTATION          "presentation"

# define ATTRIBUT_ARCHETYPE_NAME     "name"
# define ATTRIBUT_ARCHETYPE_FUNCT    "function"
# define ATTRIBUT_GUIDELINE_NAME     "name"
# define ATTRIBUT_DIALOGUE_NOM       "name"
# define ATTRIBUT_DIALOGUE_DLL       "dll"
# define ATTRIBUT_DIALOGUE_REF       "ref"
# define ATTRIBUT_CONCERN_CODE       "code"
# define ATTRIBUT_CONCERN_FRAME      "frame"
# define ATTRIBUT_CONCERN_CATEGORY   "category"
# define ATTRIBUT_CONCERN_SEVE       "severity"
# define ATTRIBUT_CONCERN_AUTO       "autocreate"
# define ATTRIBUT_HEAD_LANG          "lang"
# define ATTRIBUT_HEAD_TITLE         "title"
# define ATTRIBUT_HEAD_HELP          "help_url"
# define ATTRIBUT_ITEM_CODE          "code"
# define ATTRIBUT_ITEM_DECAL         "decal"
# define ATTRIBUT_ITEM_CONTROL       "control"
# define ATTRIBUT_ITEM_TEXT          "text"
# define ATTRIBUT_ITEM_ARCHETYPE     "archetype"
# define ATTRIBUT_ITEM_COMPLEMENT    "complement"
# define ATTRIBUT_CONTR_NOM          "name"
# define ATTRIBUT_CONTR_LABEL        "label"
# define ATTRIBUT_CONTR_PUBLISH      "publish"
# define ATTRIBUT_CONTR_CLASSIFY     "classify"
# define ATTRIBUT_CONTR_TYPE         "type"
# define ATTRIBUT_CONTR_LISTE        "list"
# define ATTRIBUT_CONTR_EXP          "expression"
# define ATTRIBUT_CONTR_VAR          "var"
# define ATTRIBUT_CONTR_MIN          "min"
# define ATTRIBUT_CONTR_MAX          "max"
# define ATTRIBUT_REFER_TITLE        "title"
# define ATTRIBUT_REFER_NAME         "name"
# define ATTRIBUT_REFER_EVOLUTIONOF  "evolution_of"
# define ATTRIBUT_PROP_NOM           "name"
# define ATTRIBUT_PROP_GROUPE        "group"
# define ATTRIBUT_PROP_HELP          "help_url"
# define ATTRIBUT_PROP_ID            "id"
# define ATTRIBUT_PROP_REPLACE       "replace"
# define ATTRIBUT_PROP_SAMEAS        "same_as"
# define ATTRIBUT_PROP_EVIDENCE_LVL  "evidence_level"
# define ATTRIBUT_PROP_AUTOCHECK     "autocheck"
# define ATTRIBUT_PROP_UNCHECK_ARCH  "uncheck_archetype"
# define ATTRIBUT_VAR_NOM            "name"
# define ATTRIBUT_VAR_LABEL          "label"
# define ATTRIBUT_VAR_EXPRESSION     "expression"
# define ATTRIBUT_VAR_UNIT           "unit"
# define ATTRIBUT_VALIDATEUR_COND    "condition"
# define ATTRIBUT_TREE_LOC           "localisation"
# define ATTRIBUT_NODE_LOC           "codeloc"
# define ATTRIBUT_NODE_TYPE          "type"
# define ATTRIBUT_NODE_LEXIQUE       "lexique"
# define ATTRIBUT_NODE_UNIT          "unit"
# define ATTRIBUT_NODE_COMPLEMENT    "complement"
# define ATTRIBUT_NODE_CERTITUDE     "certitude"
# define ATTRIBUT_NODE_INTERET       "interet"
# define ATTRIBUT_NODE_PLURIEL       "pluriel"
# define ATTRIBUT_NODE_FREETEXT      "freetext"
# define ATTRIBUT_FRIEND_ID          "id"
# define ATTRIBUT_DIALOGBOX_LANG     "lang"
# define ATTRIBUT_DIALOGBOX_TYPE     "type"
# define ATTRIBUT_DIALOGBOX_COORDS   "coords"
# define ATTRIBUT_DIALOGBOX_STYLE    "style"
# define ATTRIBUT_DIALOGBOX_CLASS    "class"
# define ATTRIBUT_DIALOGBOX_CAPTION  "caption"
# define ATTRIBUT_DIALOGBOX_FONTSIZE "fontsize"
# define ATTRIBUT_DIALOGBOX_FONTTYPE "fonttype"
# define ATTRIBUT_DIALOGBOX_NSTITRE  "nstitre"
# define ATTRIBUT_CONTROL_VALUE      "value"
# define ATTRIBUT_CONTROL_CAPTION    "caption"
# define ATTRIBUT_CONTROL_REFID      "refid"
# define ATTRIBUT_CONTROL_TYPE       "type"
# define ATTRIBUT_CONTROL_STYLE      "style"
# define ATTRIBUT_CONTROL_COORDS     "coords"
# define ATTRIBUT_CONTROL_ONGLET     "tab"
# define ATTRIBUT_CONTROL_DATA       "data"
# define ATTRIBUT_CONTROL_FILLING    "filling"
# define ATTRIBUT_CONTROL_HELP_TEXT  "helptext"
# define ATTRIBUT_RCDATA_NSTITRE     "nstitre"
# define ATTRIBUT_RCDATA_NSCONTRO    "nscontro"
# define ATTRIBUT_ONGLET_TITRE       "title"
# define ATTRIBUT_ONGLET_NUM         "order"
# define ATTRIBUT_PRESENT_VALUE      "value"

# define VAL_ATTR_ITEM_CTRL_ACTIF    "active"
# define VAL_ATTR_ITEM_CTRL_NONE     "none"
# define VAL_ATTR_CONTR_TYPE_EXCLUS  "exclusion"
# define VAL_ATTR_CONTR_TYPE_EXP     "expression"
# define VAL_ATTR_CONTR_TYPE_COND    "condition"
# define VAL_ATTR_CONTR_TYPE_EXIST   "exist"
# define VAL_ATTR_CONTR_TYPE_SONSEX  "sonsexist"
# define VAL_ATTR_CONTR_TYPE_NEEDED  "needed"

class _NSBBCLASSE Citem : public Cbalise
{
  public:

    bool            bItemRoot ;
    NSSuper*        _pSuper ;

    Citem(string attributs, string values, Cbalise *father, NSSuper* pSuper, bool bRoot = false) ;
    ~Citem() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;
    Valeur_array    *getArrayFils() ;

    // added by fab -- aout 2003
    bool            verifConstraintsWithPPatho(NSPatPathoArray *pTempPPT, string* psMessage) ;
    bool            searchInPPathoItemPath(NSPatPathoArray *pPPT) ;

    string          getCode() ;
    void            setCode(string sCode) ;
    string          getDecalage() ;
    string          getControl() ;
    void            setControl(string sControl) ;
    string          getText() ;
    void            setText(string sText) ;
    string          getArchetype() ;
    void            setArchetype(string sArchetype) ;

    Citem*          getFirstSon() ;
} ;

class _NSBBCLASSE Cdialogue : public Cbalise
{
  public:

    Cdialogue(string attributs, string values, Cbalise *father) ;
    ~Cdialogue() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;
} ;

class _NSBBCLASSE Cconcern : public Cbalise
{
  public:

    Cconcern(string attributs, string values, Cbalise *father) ;
    ~Cconcern() ;

    bool            parser() ;
    bool            verif();
    bool            compresser() ;
    bool            traiter() ;

    string          getCode() ;
    string          getFrame() ;
    string          getCategory() ;
    bool            getAutoCreate() ;
    int             getSeverity() ;
} ;

class _NSBBCLASSE Chead : public Cbalise
{
  public:

    Chead(string attributs, string values, Cbalise *father);
    ~Chead();

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    string          getTitle() ;
    string          getHelpUrl() ;
    string          getLang() ;
    string          getGroup() ;
} ;

class _NSBBCLASSE Creferences : public Cbalise
{
  public:

    Creferences(string attributs, string values, Cbalise *father) ;
    ~Creferences() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    Cconcern        *getFirstCconcern() ;
    Cconcern        *getNextCconcern(Cconcern *pPrevious) ;
    Chead           *getHead(string sLang = "") ;

    string          getHelpUrl(string sLang = "") ;
} ;

class _NSBBCLASSE Ccontrainte : public Cbalise
{
  public:

    bool            bContrainteRoot ;

    Ccontrainte(string attributs, string values, Cbalise *father, bool bRoot = false);
    ~Ccontrainte() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    Valeur_array    *getValArray() ;
    bool            mustPublish() ;
} ;

class _NSBBCLASSE Cvariable : public Cbalise
{
  public:

    bool            trouve ;

    Cvariable(string attributs, string values, Cbalise *father) ;
    ~Cvariable() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    string          getName() ;
    string          getLabel() ;
    string          getExpression() ;
    string          getUnit() ;

    bool            isEmpty() ;
} ;

class _NSBBCLASSE Calias : public Cbalise
{
  public:

    Calias(string attributs, string values, Cbalise *father) ;
    ~Calias() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    string          getValue() ;

    void            setLocal(bool bLoc) { _bIsLocal = bLoc ; }
    bool            isLocal()           { return _bIsLocal ; }

  protected:

    bool            _bIsLocal ;
} ;

class _NSBBCLASSE Cvalidateur : public Cbalise
{
  public:

    Cvalidateur(string attributs, string values, Cbalise *father) ;
    ~Cvalidateur() ;

    bool   parser() ;
    bool   verif() ;
    bool   compresser() ;
    bool   traiter() ;

    string getValidationString() ;
} ;

class Cglobalvars ;

// A GlobalVar is a proxy objet for a Cvariable or a Ccontrainte
//
class _NSBBCLASSE GlobalVar
{
  public:

    enum gvState { stateNotDef = -1, notFound = 0, found = 1 } ;
    enum gvType { typeNotDef = -1, variable = 1, constraint = 2, validator = 3 } ;
    enum gvSubType { subtypeNotDef = -1, plainVar = 1, compoundVar = 2 } ;

    GlobalVar(Cbalise *bal, gvType typ, Cglobalvars *pGlobalVars) ;
    ~GlobalVar() ;

    bool              process(NSContexte* pCtx, bool bUserIsWaiting = false, HWND interfaceHandle = 0, NSPatPathoArray* pLocalPatho = 0) ;
    void              resetProcessState() ;
    NSPatPathoArray * getPPT() ;
    string            getName() ;
    string            getLabel() ;
    bool              getToPublish() ;
    string            getClassification() ;
    gvType            getType() ;
    gvSubType         getSubtype() ;
    gvState           getState() ;
    Cbalise *         getBalise() ;

    void              setGlobalVars(Cglobalvars *pGV) { _globalVars = pGV ; }
    Cglobalvars *     getGlobalVars()                 { return _globalVars ; }

  protected:

    string            _name ;
    string            _alias ;
    gvState           _state ;
    gvType            _type ;
    gvSubType         _subtype ;

    string            _label ;
    bool              _bPublish ;
    string            _classification ;

    Cbalise *         _balise ;
    NSPatPathoArray * _ppt ;

    Cglobalvars *     _globalVars ;
} ;

typedef vector<GlobalVar *>					      GlobalVarVector ;
typedef GlobalVarVector::iterator					GlobalVarIter ;
typedef GlobalVarVector::reverse_iterator GlobalVarRIter ;
typedef GlobalVarVector::const_iterator   GlobalVarCIter ;
typedef NTArray<GlobalVar>                GlobalVarArray ;

class _NSBBCLASSE Cglobalvars : public Cbalise
{
  public:

    Cglobalvars(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~Cglobalvars() ;

    bool                parser() ;
    bool                verif() ;
    bool                compresser() ;
    bool                traiter() ;

    Valeur_array *      getValArray() ;

    bool                isVarP(string label, GlobalVar::gvType typ) ;
    NSPatPathoArray *   getPPT(string label, GlobalVar::gvType typ) ;
    GlobalVar::gvState  getState(string label, GlobalVar::gvType typ) ;
    Cbalise *           getBalise(string label, GlobalVar::gvType typ) ;

    bool                isProcessed() ;
    bool                process(NSContexte* pCtx, bool bUserIsWaiting = false, HWND interfaceHandle = 0, NSPatPathoArray* pLocalPatho = 0) ;
    void                resetProcessState() ;

#ifndef _ENTERPRISE_DLL
    string              getPublishingString(NSContexte *pContexte) ;
#else
    string              getPublishingString() ;
#endif
    bool                existPublishingString() ;

    NSSuper*            _pSuper ;

  protected:

    GlobalVarArray *    _pGVars ;
    bool                _processed ;
} ;

// -----------------------------------------------------------------------------
// referentiels "amis"
// -----------------------------------------------------------------------------
class _NSBBCLASSE Cfriend : public Cbalise
{
  public:

    NSSuper* _pSuper ;

    Cfriend(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~Cfriend() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    string          getId() ;
} ;

// -----------------------------------------------------------------------------
// referentiels amis exclus par ce référentiel
// -----------------------------------------------------------------------------
class _NSBBCLASSE CExcludedFriends : public Cbalise
{
  public:

    NSSuper* _pSuper ;

    CExcludedFriends(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~CExcludedFriends() ;

    bool     parser() ;
    bool     verif() ;
    bool     compresser() ;
    bool     traiter() ;

    Cfriend* getNextExcludedFriend(Cfriend* pPrevious) ;
} ;

// -----------------------------------------------------------------------------
// referentiels amis exclus par ce référentiel
// -----------------------------------------------------------------------------
class _NSBBCLASSE CNeededFriends : public Cbalise
{
  public:

    NSSuper* _pSuper ;

    CNeededFriends(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~CNeededFriends() ;

    bool     parser() ;
    bool     verif() ;
    bool     compresser() ;
    bool     traiter() ;

    Cfriend* getNextNeededFriend(Cfriend *pPrevious) ;
} ;

// -----------------------------------------------------------------------------
// Ensemble des referentiels amis
// -----------------------------------------------------------------------------
class _NSBBCLASSE CFriendsGroup : public Cbalise
{
  public:

    NSSuper* _pSuper ;

    CFriendsGroup(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~CFriendsGroup() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    CExcludedFriends* getExcludedFriends() ;
    CNeededFriends*		getNeededFriends() ;
} ;

// -----------------------------------------------------------------------------
// cas des referentiels
// -----------------------------------------------------------------------------
class _NSBBCLASSE Creferentiel : public Cbalise
{
  public:

    NSSuper* _pSuper ;

    Creferentiel(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~Creferentiel() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;
    Valeur_array    *getValArray() ;
    int             getNbPropGroup(NSContexte *pCtx, string sGroup, bool bWithValidation = false, string sLang = "") ;
    bool            getNextGroup(string& sGroup, string sLang = "") ;

    string          getTitle(string sLang = "") ;
    string          getHelpUrl(string sLang = "") ;
    string          getName() ;
    string          getEvolutionOf() ;
    Cglobalvars *   getGVars() ;
    Creferences     *getReference() ;

  protected:

    Cglobalvars *   _pGlobalVars ;
} ;

class _NSBBCLASSE Cproposition : public Cbalise
{
  public:

    enum EVIDENCE_LEVEL { levelUnknown = -1, levelPro, levelC, levelB, levelA } ;

    NSSuper*        _pSuper ;
    bool            traite ;
    bool            check ;

    bool            bInitialyChecked ;
    NSPatPathoArray *pReasonTree ;

    Cproposition(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~Cproposition() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    Valeur_array    *getValArray() ;
    Ccontrainte     *getValidity() ;
    Creferences     *getReference() ;

    bool            getTraite() ;
    void            setTraite(bool bTraite) ;
    bool            getCheck() ;
    void            setCheck(bool bCheck) ;

    string          getPropID() ;
    string          getReplacedPropID() ;
    string          getSameAsPropID() ;
    EVIDENCE_LEVEL  getEvidenceLevel() ;
    bool            getIsAutocheck() ;
    string          getUncheckArchetype() ;

    string          getTitle(string sLang = "") ;
    string          getGroup(string sLang = "") ;
    string          getHelpUrl(string sLang = "") ;

    string					getRefIDFromPropID() ;
    string					getLocalPropIDFromPropID() ;

    void            addExcludedToVector(VecteurString* pVector) ;
    void            addNeededToVector(VecteurString* pVector) ;
} ;

class _NSBBCLASSE Ctree : public Cbalise
{
  public:

    NSSuper*        _pSuper ;
    NSPatPathoArray *pPatPathoArray ;

    Ctree(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~Ctree() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    bool            chargePatPatho() ;
    bool            initFromPatPatho() ;

    bool            traiteCodeLoc() ;
    string          getLocalisation() ;
} ;

class _NSBBCLASSE Cnode : public Cbalise
{
  public:

    NSPatPathoInfo  *pPatPathoInfo ;

    Cnode(string attributs, string values, Cbalise *father) ;
    ~Cnode() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;
    bool            chargePatPatho(NSPatPathoArray *pPPTArray) ;
    bool            initFromPatPatho(NSPatPathoArray *pPPTArray) ;

    bool            traiteCodeLoc(int *iLigne, int iColonne) ;
} ;

class XressourceAttributes
{
  public:

    XressourceAttributes() ;
    XressourceAttributes(const XressourceAttributes& rv) ;
    virtual ~XressourceAttributes() {}

    // Getters
    string getCaption()  { return _caption ; }
    string getLang()     { return _sLang ; }
    string getType()     { return _type ; }
    string getStrStyle() { return _style ; }
    uint32 getStyle()    { return _iStyle ; }
    string getCoords()   { return _coords ; }
    int    getX()        { return _x ; }
    int    getY()        { return _y ; }
    int    getW()        { return _w ; }
    int    getH()        { return _h ; }

    // Setters
    void setCaption(string sC)  { _caption = sC ; }
    void setLang(string sL)     { _sLang   = sL ; }
    void setType(string sT)     { _type    = sT ; }
    void setStrStyle(string sS) { _style   = sS ; }
    void setStyle(uint32 iS)    { _iStyle  = iS ; }
    void setCoords(string sC)   { _coords  = sC; parseCoords() ; }
    void setX(int iX)           { _x = iX ; }
    void setY(int iY)           { _y = iY ; }
    void setW(int iW)           { _w = iW ; }
    void setH(int iH)           { _h = iH ; }

    // Transformers
    void setIStyleFromSStyle(string *pErrors) ;
    void setIStyleAttribute(uint32 iAttribute, bool bNot = false) ;
    void setSStyleFromIStyle() ;
    void setSStyleAttribute(string sAttribute, uint32 iAttribute, bool bProcessNot = false) ;
    void buildCoords() ;
    void parseCoords() ;

    XressourceAttributes& operator=(const XressourceAttributes& src) ;

  protected:

    string _caption ;
    string _sLang ;
    string _type ;
    string _style ;
    uint32 _iStyle ;
    string _coords ;
    int    _x ;
    int    _y ;
    int    _w ;
    int    _h ;

    map<string, uint32> styleMap ;

    void   initStyleAttributes() ;
    uint32 getStyleIntFromString(string sAttribute) ;
    string getStyleStringFromInt(uint32 iAttribute) ;
} ;

class _NSBBCLASSE Xdialogbox : public XressourceAttributes
{
  public:

    Xdialogbox() : XressourceAttributes()  {}
    Xdialogbox(const Xdialogbox& rv) ;
    ~Xdialogbox() {}

    Xdialogbox& operator=(const Xdialogbox& src) ;
} ;

class _NSBBCLASSE Xcontrol : public XressourceAttributes
{
  public:

    Xcontrol()  {}
    Xcontrol(const Xcontrol& rv) ;
    ~Xcontrol() {}

    int  getRefId()       { return _refId ; }
    void setRefId(int iR) { _refId = iR ; }
    void setStrRefId(string sR) ;

    bool   initFromValue(string sValue) ;
    string buildValueString() ;

    Xcontrol& operator=(const Xcontrol& src) ;

  protected:

    int _refId ;
} ;

class _NSBBCLASSE Ccontrol : public Cbalise
{
  public:

    Xcontrol value ;

    Ccontrol(string attributes, string values, Cbalise *father) ;
    ~Ccontrol() ;

    bool   parser() ;
    bool   verif() ;
    bool   compresser() ;
    bool   traiter() ;

    string getValue() ;
    string getUpdatedValue() ;
    string getTab() ;
    string getData() ;
    string getDataIdentity() ;
    string getDataFunction() ;
    string getDataExtension() ;
    string getDataLeft(string sData) ;
    string getDataRight(string sData) ;
    string getFilling() ;
    string getHelpText() ;
    void   updateParamsFromValue() ;

    void   setValue(string sValue) ;
    void   setTab(string sTab) ;
    void   setData(string sData) ;
    void   setDataIdentity(string sIdentity) ;
    void   setDataFunction(string sFunction) ;
    void   setFilling(string sFilling) ;
    void   setHelpText(string sHelpText) ;

    void   setIStyle() ;
    // void            setStyleAttribute(int* iStyle, int iAttribute, bool bNot = false) ;
    void   setStyleString() ;
    void   setStyleStringElement() ;
    int    getIStyle() ;
    void   buildCoords() { value.buildCoords() ; }
    bool   parseValue() ;

    // EN : all above is for the future - do not care about it for the moment
    // FR : tout ce qui suit dans cette classe est pour plus tard - pour l'instant il ne faut pas y faire attention
    string getCaption() ;
    int    getRefId() ;
    string getType() ;
    string getStyle() ;
    string getCoords() ;
    int    getX() ;
    int    getY() ;
    int    getW() ;
    int    getH() ;

    void setCaption(string sC)  { value.setCaption(sC) ; }
    void setLang(string sL)     { value.setLang(sL) ; }
    void setType(string sT)     { value.setType(sT) ; }
    void setStrStyle(string sS) { value.setStrStyle(sS) ; }
    void setStyle(uint32 iS)    { value.setStyle(iS) ; }
    void setCoords(string sC)   { value.setCoords(sC) ; }
    void setX(int iX)           { value.setX(iX) ; }
    void setY(int iY)           { value.setY(iY) ; }
    void setW(int iW)           { value.setW(iW) ; }
    void setH(int iH)           { value.setH(iH) ; }
    void setRefId(int iD)       { value.setRefId(iD) ; }
    void setStrRefId(string sD) { value.setStrRefId(sD) ; }

    void setRect(int iX, int iY, int iW, int iH) ;
} ;

class _NSBBCLASSE Crcdata : public Cbalise
{
  public:

    Crcdata(string attributes, string values, Cbalise *father) ;
    ~Crcdata() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    string          getNSTitre() ;
    string          getNSContro() ;
    string          getValue() ;      // retourne une chaine qui correspond exactement à ce qu'il y a dans .rc
} ;


class _NSBBCLASSE Ctab : public Cbalise
{
  public:

    Ctab(string attributes, string values, Cbalise *father) ;
    ~Ctab() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    string          getTitle() ;
    string          getOrder() ;
} ;


class _NSBBCLASSE Ctabcontrol : public Cbalise
{
  public:

    Ctabcontrol(string attributes, string values, Cbalise *father) ;
    ~Ctabcontrol() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    Ctab            *getFirstTab() ;
    Ctab            *getNextTab(Ctab *pLastTab) ;
    int             getNbTabs() ;
} ;


class _NSBBCLASSE Cdialogbox : public Cbalise
{
  public:

    Xdialogbox value ;

    Cdialogbox(string attributes, string values, Cbalise *father) ;
    ~Cdialogbox() ;

    bool        parser() ;
    bool        verif() ;
    bool        compresser() ;
    bool        traiter() ;

    void        setIStyle() ;
    int         getIStyle() ;
    bool        parseValue() ;
    int         getX() ;
    int         getY() ;
    int         getW() ;
    int         getH() ;

    string      getLang() ;
    string      getType() ;
    string      getCoords() ;
    string      getStyle() ;
    string      getClass() ;
    string      getCaption() ;
    string      getFontSize() ;
    string      getFontType() ;
    string      getNSTitre() ;

    void        setLang(string sLang) ;
    void        setType(string sTy) ;
    void        setCoords(string sC) ;
    void        setStyle(string sSty) ;
    void        setClass(string sC) ;
    void        setCaption(string sCap) ;
    void        setFontSize(string sFS) ;
    void        setFontType(string sFT) ;
    void        setNSTitre(string sNST) ;

    void        setValueCaption(string sCap) { value.setCaption(sCap) ; }
    void        setX(int iX) { value.setX(iX) ; }
    void        setY(int iY) { value.setY(iY) ; }
    void        setW(int iW) { value.setW(iW) ; }
    void        setH(int iH) { value.setH(iH) ; }

    void        updateParamsFromValue() ;
    void        buildCoords() { value.buildCoords() ; }

    Ccontrol    *getFirstControl() ;
    Ccontrol    *getNextControl(Ccontrol *pLastControl) ;
    int         getNbControl() ;

    Crcdata     *getRCData() ;
    Ctabcontrol *getTabControl() ;
    string      getStringRCData() ;
    string      getValueRCData() ;

    void        addControl(Ccontrol *pControl) ;
} ;


class _NSBBCLASSE Cpresentation : public Cbalise
{
  public:

    Cpresentation(string attributes, string values, Cbalise *father) ;
    ~Cpresentation() ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter() ;

    string          getValue() ;
} ;

class _NSBBCLASSE Carchetype : public Cbalise
{
  public:

    NSSuper* _pSuper ;

    Carchetype(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~Carchetype() ;
    Carchetype(const Carchetype& rv) ;
    Carchetype& operator=(const Carchetype& src) ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter();

    Citem           *getRootItem() ;
    Cdialogue       *getDialog() ;
    Creferences     *getReference() ;
    Chead           *getHead(string sLang = "") ;
    string           getTitle(string sLang = "") ;
    Cdialogbox      *getDialogBox(string sLang = "") ;
    Cpresentation   *getPresentation() ;

    string          getName() ;
    string          getFunction() ;
    string          getHelpUrl(string sLang = "") ;
} ;

class _NSBBCLASSE Cguideline : public Cbalise
{
  public:

    NSSuper* _pSuper ;

    Cguideline(string attributs, string values, Cbalise *father, NSSuper* pSuper) ;
    ~Cguideline() ;
    Cguideline(const Cguideline& rv) ;
    Cguideline& operator=(const Cguideline& src) ;

    bool            parser() ;
    bool            verif() ;
    bool            compresser() ;
    bool            traiter();

    Citem           *getRootItem() ;
    Creferences     *getReference() ;
    Cdialogbox      *getDialogBox(string sLang = "") ;
    Cglobalvars     *getGVars() ;
    Csteps          *getSteps() ;

    CguidelineStep  *getGuidelineStepById(string sId) ;

    string          getName() ;
    string          getTitle(string sLang = "") ;
    string          getFunction() ;
    string          getHelpUrl(string sLang = "") ;

  protected:

    Cglobalvars *   _pGlobalVars ;
} ;

#endif // !__NSATTVAL_H__

