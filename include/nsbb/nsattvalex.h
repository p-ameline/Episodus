//---------------------------------------------------------------------------
#ifndef __NSATTVALEX_H
# define __NSATTVALEX_H

class NSContexte ;
class NSPatPathoInfo ;
class NSPatPathoArray ;
class VecteurString ;

# include <vector.h>
# include <string>

# include "nsbb\nsexport.h"
# include "balise.h"

# define LABEL_EX_PATIENT                 "pmed_patient"
# define LABEL_EX_ADMINDATA               "pmed_admindata"
# define LABEL_EX_HEALTHDATA              "pmed_healthdata"
# define LABEL_EX_DOCUMENT                "document"
# define LABEL_EX_GOAL                    "goal"
# define LABEL_EX_NODE                    "node"

# define ATTRIBUT_EX_PATIENT_ID           "patientid"
# define ATTRIBUT_EX_PATIENT_CODE         "code"
# define ATTRIBUT_EX_PATIENT_CONTACT      "contact"
# define ATTRIBUT_EX_ADMIN_SEXE           "sexe"
# define ATTRIBUT_EX_ADMIN_NAISSANCE      "naissance"
# define ATTRIBUT_EX_HEALTHDATA_DATE      "date"
# define ATTRIBUT_EX_DOCU_CODE            "code"
# define ATTRIBUT_EX_DOCU_DATE            "date"
# define ATTRIBUT_EX_NODE_PAT_ID          "patId"
# define ATTRIBUT_EX_NODE_TREE_ID         "treeId"
# define ATTRIBUT_EX_NODE_NODE_ID         "nodeId"
# define ATTRIBUT_EX_NODE_UNIT_ID         "unitId"
# define ATTRIBUT_EX_NODE_LEXIQUE         "lexique"
# define ATTRIBUT_EX_NODE_UNITE           "unite"
# define ATTRIBUT_EX_NODE_COMPLMNT        "complement"
# define ATTRIBUT_EX_NODE_INTERET         "interet"
# define ATTRIBUT_EX_NODE_PLURIEL         "pluriel"

//
// Taille des champs du fichier NSREFLINK
//

# define REFLINK_CLASSIFICATION_LEN       5
# define REFLINK_CODE_LEN                15
# define REFLINK_CHEMIN_LEN             255
# define REFLINK_USE_VALUE_LEN            1
# define REFLINK_VALUE_CODE_LEN         255

//// Indice des champs du fichier NSREFLINK
//

# define REFLINK_CLASSIFICATION_FIELD     1
# define REFLINK_CODE_FIELD               2
# define REFLINK_CHEMIN_FIELD             3
# define REFLINK_USE_VALUE_FIELD          4
# define REFLINK_VALUE_CODE_FIELD         5


class _NSBBCLASSE CmessageExport : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  VectString        aVecteurPatients ;
  string            sUserId ;
  string            sPathMsg ;
  string            sReleaseNb ;

  CmessageExport(string attributs, string values, NSContexte *pCtx, string sMedId = "") ;
  ~CmessageExport() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  bool              addToSFMGfile(string sFileIndex, string sNewLine) ;
} ;


class _NSBBCLASSE CpatientExport : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExport    *pMsgExport ;

  CpatientExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp) ;
  ~CpatientExport() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getID() ;
  string            getCode() ;
  string            getContact() ;

  bool              createSFMGfiles() ;
  string            donnePatIndice() ;
  string            donneActeIndice() ;
} ;


class _NSBBCLASSE CadminDataExport : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExport    *pMsgExport ;

  CadminDataExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp) ;
  ~CadminDataExport() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getSexe() ;
  string            getNaissance() ;
} ;


class _NSBBCLASSE ChealthDataExport : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExport    *pMsgExport ;

  ChealthDataExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp) ;
  ~ChealthDataExport() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getDate() ;

  bool              createSFMGfiles(string sCodePat, string sCodeActe) ;
} ;


class _NSBBCLASSE CdocumentExport : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExport    *pMsgExport ;
  NSPatPathoArray   *pPatPathoArray ;

  CdocumentExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp) ;
  ~CdocumentExport() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getCode() ;
  string            getDate() ;

  bool              createSFMGfiles(string sCodePat, string sCodeActe, string sCodeRC, string sDate) ;
} ;


class _NSBBCLASSE CgoalExport : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExport    *pMsgExport ;
  NSPatPathoArray   *pPatPathoArray ;
  string            sUserId ;

  CgoalExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp) ;
  ~CgoalExport() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  bool              createSFMGfiles(string sCodePat, string sCodeActe, string sCodeRC, string sCodeGoal) ;
} ;


class CnodeExport : public Cbalise
{
 public:
  int               iLigne, iColonne ;

  NSPatPathoInfo    *pPatPathoInfo1 ;
  NSPatPathoInfo    *pPatPathoInfo2 ;

  CnodeExport(string attributs, string values, int iCol) ;
  ~CnodeExport() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;
  bool              chargePatPatho(NSPatPathoArray *pPPTArray) ;
} ;

class _NSBBCLASSE nsExportParseur
{
 public:
  CmessageExport    *pMessage ;
  NSContexte        *pContexte ;
  bool              bParsingOk ;
  bool              bVerbose ;
  string            sReleaseNb ;

  nsExportParseur(NSContexte *pCtx, string sRelease, bool bVerb = true) ;
  ~nsExportParseur();

  bool              open(string sXmlFileName) ;
  bool              parse_message(string message, string tag, int *iParsingError) ;
} ;


// -----------------------------------------------------------------------------
// Classe NSRefLink
// -----------------------------------------------------------------------------

//// Objet donn�es
//

class NSRefLinkData{
  public:

    // Variables de stockage des valeurs
    char              classification[REFLINK_CLASSIFICATION_LEN + 1] ;
    char              code[REFLINK_CODE_LEN + 1] ;
    char              chemin[REFLINK_CHEMIN_LEN + 1] ;
    char              use_value[REFLINK_USE_VALUE_LEN + 1] ;
    char              value_code[REFLINK_VALUE_CODE_LEN + 1] ;

    // constructeurs
    NSRefLinkData() ;
    NSRefLinkData(const NSRefLinkData& rv) ;

    // op�rateurs
    NSRefLinkData& operator=(const NSRefLinkData& src) ;    int 			     operator==(const NSRefLinkData& o) ;

    // fonctions
    void              metAZero();} ;

class NSRefLink ;class NSRefLinkInfo{
  public:

    // Objet qui contient les donn�es
    NSRefLinkData     *pDonnees ;

    // constructeurs/destructeur
    NSRefLinkInfo() ;    NSRefLinkInfo(NSRefLink *) ;
    NSRefLinkInfo(const NSRefLinkInfo& rv) ;
    ~NSRefLinkInfo() ;

    // op�rateurs
    NSRefLinkInfo& operator=(const NSRefLinkInfo& src) ;    int            operator==(const NSRefLinkInfo& o) ;
} ;

typedef vector<NSRefLinkInfo *>       NSRefLinkInfoVector ;
typedef NSRefLinkInfoVector::iterator NSRefLinkInfoIter ;

class NSRefLinkInfoArray : public NSRefLinkInfoVector
{
  public:

    NSRefLinkInfoArray(NSContexte *pCtx, string sFile) ;
    NSRefLinkInfoArray(NSContexte *pCtx) ;
    ~NSRefLinkInfoArray() ;

    string            sFileName ;
    NSContexte        *pContexte ;

    bool              fillArray() ;
    bool              GetAllCodesAndValues(NSPatPathoArray *pPatPathoArray, string sClassification, VecteurString *pResult, bool bEmptyResult = true) ;
    bool              vider() ;
} ;

// -----------------------------------------------------------------------------// Objet d�riv� de NSFiche utilis� pour les op�rations de base de donn�es// -----------------------------------------------------------------------------

/*
class NSRefLink : public NSFiche
{
 public:
  // Variables de stockage des valeurs
  NSRefLinkData     *pDonnees ;

  NSRefLink(NSContexte *pCtx) ;  ~NSRefLink() ;

  DBIResult         open() ;  void              metAZero()            { pDonnees->metAZero() ; }
  void              alimenteFiche() ;
  void              videFiche() ;

  bool              GetAllCodesAndValues(NSPatPathoArray *pPatPathoArray, string sClassification, VecteurString *pResult, bool bEmptyResult = true) ;
} ;
*/

// -----------------------------------------------------------------------------
// Ancienne version (fichier dpioXXX.msg)
// -----------------------------------------------------------------------------
class _NSBBCLASSE CmessageExportOld : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  VectString        aVecteurPatients ;
  string            sUserId ;
  string            sPathMsg ;

  CmessageExportOld(string attributs, string values, NSContexte *pCtx, string sMedId = "") ;
  ~CmessageExportOld() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  bool              addToSFMGfile(string sFileIndex, string sNewLine) ;
} ;


class _NSBBCLASSE CpatientExportOld : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExportOld *pMsgExport ;

  CpatientExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp) ;
  ~CpatientExportOld() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getID() ;
  string            getCode() ;

  bool              createSFMGfiles(int iNumActe) ;
  string            donnePatIndice() ;
  string            donneActeIndice(int iNumActe) ;
} ;


class _NSBBCLASSE CadminDataExportOld : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExportOld *pMsgExport ;

  CadminDataExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp) ;
  ~CadminDataExportOld() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getSexe() ;
  string            getNaissance() ;
  string            donneNaissance() ;
} ;


class _NSBBCLASSE ChealthDataExportOld : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExportOld *pMsgExport ;

  ChealthDataExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp) ;
  ~ChealthDataExportOld();

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getDate() ;

  bool              createSFMGfiles(string sCodePat, string sCodeActe) ;
} ;


class _NSBBCLASSE CdocumentExportOld : public Cbalise
{
 public:
  NSContexte        *pContexte;
  CmessageExportOld *pMsgExport ;
  NSPatPathoArray   *pPatPathoArray ;

  CdocumentExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp) ;
  ~CdocumentExportOld() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  string            getCode() ;
  string            getDate();

  bool              createSFMGfiles(string sCodePat, string sCodeActe, string sCodeRC, string sDate) ;
} ;


class _NSBBCLASSE CgoalExportOld : public Cbalise
{
 public:
  NSContexte        *pContexte ;
  CmessageExportOld *pMsgExport ;
  NSPatPathoArray   *pPatPathoArray ;

  CgoalExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp) ;
  ~CgoalExportOld() ;

  bool              parser() ;
  bool              verif() ;
  bool              compresser() ;
  bool              traiter() ;

  bool              createSFMGfiles(string sCodePat, string sCodeActe, string sCodeRC, string sCodeGoal) ;
} ;


class _NSBBCLASSE nsExportParseurOld
{
 public:
  CmessageExportOld *pMessage ;
  NSContexte        *pContexte ;
  bool              bParsingOk ;
  bool              bVerbose ;

  nsExportParseurOld(NSContexte *pCtx, bool bVerb = true) ;
  ~nsExportParseurOld();

  bool              open(string sXmlFileName) ;
  bool              parse_message(string message, string tag, int *iParsingError) ;
} ;

#endif // __NSATTVALEX_H

