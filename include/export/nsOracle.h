#ifndef __NS_ORACLE_H#define __NS_ORACLE_H#ifndef OCI_ORACLE# include "oracle_ci\oci.h"
#endifclass NSSejourInfo ;class NSOracleInterface : public NSSuperRoot{
  public:

    NSOracleInterface(NSSuper* pSuper, string sDatabaseName, string sUserName, string sPassword) ;
    ~NSOracleInterface() ;

    bool initializeEnvironment() ;
    void cleanupEnvironment() ;

    bool getVenuesForNip(string sNip, vector<NSSejourInfo*> *paVenues) ;

    string getErrorType(sword status) ;

  protected:

    OCIError  *_pErrHp ;    // Error handle
    OCIServer *_pSrvHp ;    // Server Handle
    OCISvcCtx *_pSvcHp ;    // Service context handle

    OCIEnv    *_pOciEnv ;  // OCI environment

    string    _sDatabaseName ;
    string    _sUserName ;
    string    _sPassword ;

    void   oracleTrace(string sMsg, sword status, NSSuper::TRACETYPE iLevel = NSSuper::trDetails) ;
    string getSqlQuery(string sSqlQueryFilename) ;
    string getAdaptedSqlQuery(string sSqlQuery, string sNip) ;
};
#define MAX_NOIP_LEN 10typedef struct T_DOS{
  OCIString* NODA ;   // Numero de dossier administratif                       O  V(10)
  OCIString* KYNDA ;  // Cle du NDA                                            O  V(1)
  OCIString* TYADMI ; // Type d'admission (AD,PA,AC)                           O  V(2)
  OCIString* TYDOS ;  // Type de dossier                                       O  V(1)
  OCIString* CDETDO ; // Etat du dossier                                       O  V(1)
  OCIString* NOIP ;   // NIP du patient                                        O  V(10)
  OCIString* CDPSY ;  // Psychiatrie (O/N)                                        V(1)
  OCIString* CDPRJ ;  // Protection juridique (O/N)                               V(1)
  OCIDate    DAREAF ; // Date de reference de l'adresse francaise                 DATE
  OCIDate    DAREAA ; // Date de reference de l'autre adresse                     DATE
  OCIString* CDACCI ; // Accident (O/N)                                           V(1)
  OCIString* CDVIAU ; // Visites autorisees (O/N)                                 V(1)
  OCIString* CDOPER ; // Autorisation d'operer (O/N)                              V(1)
  OCIString* CDAUPH ; // Autorisation de photographier (O/N)                      V(1)
  OCIString* CDANES ; // Autorisation d'anesthesier (O/N)                         V(1)
  OCIString* CDANOM ; // Demande d'anonymat (O/N)                                 V(1)
  OCIString* NMEMPL ; // Nom de l'employeur                                       V(32)
  OCIString* COIDEM ; // Complement d'identite de l'employeur                     V(32)
  OCIString* ADREEM ; // Adresse de l'employeur                                   V(32)
  OCIString* LBCOEM ; // Libelle commune de l'employeur                           V(32)
  OCIString* CDPOEM ; // Code postal de l'employeur                               V(5)
  OCIString* LIACEM ; // Ligne d'acheminement de l'employeur                      V(26)
  OCIString* NOTLEM ; // Telephone de l'employeur                                 V(18)
  OCIString* NOSIRE ; // Numero de SIRET employeur                                V(14)
  OCIString* NOURSF ; // Numero URSSAF employeur                                  V(15)
  OCIString* CMEMP ;  // Commentaire employeur                                    V(80)
  OCIString* NOETPR ; // Code etablissement de provenance                         V(5)
  OCIString* LBETAP ; // Libelle etablissement de provenance                      V(40)
  OCIString* ADREEP ; // Adresse (No et rue) etab. de provenance                  V(32)
  OCIString* COADEP ; // Adresse (Complement) etab. de provenance                 V(32)
  OCIString* CDPOEP ; // Code postal etablissement de provenance                  V(5)
  OCIString* LBCOEP ; // Libelle commune etablissement de provenance              V(32)
  OCIString* LIACEP ; // Ligne d'acheminement etablissement de provenance         V(26)
  OCIString* NOFINP ; // No FINESS etablissement de provenance                    V(9)
  OCIString* NODAPR ; // NDA de provenance                                        V(9)
  OCIDate    DAHOPR ; // Date d'hospitalisation provenance                        DATE
  OCIString* TYTRPR ; // Type transfert de provenance                             V(1)
  OCIString* NOHODN ; // Code etablissement de destination                        V(5)
  OCIString* LBETAD ; // Libelle etablissement de destination                     V(40)
  OCIString* ADREED ; // Adresse (No et rue) etab. destination ou adr. sortie     V(32)
  OCIString* COADED ; // Adresse (complement) etab. destination ou adr. sortie    V(32)
  OCIString* LBCOED ; // Libelle commune etab. destination ou adr. sortie         V(32)
  OCIString* CDPOED ; // Code postal etab. destination ou adr. sortie             V(5)
  OCIString* LIACED ; // Ligne d'acheminement etab. destination ou adr. sortie    V(26)
  OCIString* NOFIND ; // No FINESS etablissement de destination                   V(9)
  OCIString* COIDED ; // Complement d'identite sortie                             V(32)
  OCIDate    DAENTR ; // Date d'entree                                         O  DATE
  OCIString* HHENTR ; // Heure d'entree                                           V(4)
  OCIString* MDENTR ; // Mode d'entree                                            V(1)
  OCIString* CDTREN ; // Transport d'entree                                       V(1)
  OCIString* NOUFEN ; // UH d'entree                                           O  V(3)
  OCIDate    DASOR ;  // Date de sortie                                           DATE
  OCIString* HHSOR ;  // Heure de sortie                                          V(4)
  OCIString* MDSOR ;  // Mode de sortie                                           V(1)
  OCIString* TYSOR ;  // Type de sortie                                           V(1)
  OCIString* NOUFSO ; // UH de sortie                                             V(3)
  OCIString* CDTRSO ; // Transport de sortie                                      V(1)
  OCIString* CDUSAD ; // Gestionnaire admissionniste du dossier                   V(8)
  OCIString* CDUSFS ; // Gestionnaire frais de sejour du dossier                  V(8)
  OCIString* CDCLOA ; // Indicateur de cloture administrative                     V(1)
  OCIString* NODAOR ; // NODA d'origine                                           V(9)
} T_DOS ;
#define MAX_NODA_LEN 10#define MAX_KYNDA ;  // Cle du NDA                                            O  V(1)
#define MAX_TYADMI ; // Type d'admission (AD,PA,AC)                           O  V(2)
#define MAX_TYDOS ;  // Type de dossier                                       O  V(1)
#define MAX_CDETDO ; // Etat du dossier                                       O  V(1)
#define MAX_NOIP ;   // NIP du patient                                        O  V(10)
#define MAX_CDPSY ;  // Psychiatrie (O/N)                                        V(1)
#define MAX_CDPRJ ;  // Protection juridique (O/N)                               V(1)
#define MAX_CDACCI ; // Accident (O/N)                                           V(1)
#define MAX_CDVIAU ; // Visites autorisees (O/N)                                 V(1)
#define MAX_CDOPER ; // Autorisation d'operer (O/N)                              V(1)
#define MAX_CDAUPH ; // Autorisation de photographier (O/N)                      V(1)
#define MAX_CDANES ; // Autorisation d'anesthesier (O/N)                         V(1)
#define MAX_CDANOM ; // Demande d'anonymat (O/N)                                 V(1)
#define MAX_NMEMPL ; // Nom de l'employeur                                       V(32)
#define MAX_COIDEM ; // Complement d'identite de l'employeur                     V(32)
#define MAX_ADREEM ; // Adresse de l'employeur                                   V(32)
#define MAX_LBCOEM ; // Libelle commune de l'employeur                           V(32)
#define MAX_CDPOEM ; // Code postal de l'employeur                               V(5)
#define MAX_LIACEM ; // Ligne d'acheminement de l'employeur                      V(26)
#define MAX_NOTLEM ; // Telephone de l'employeur                                 V(18)
#define MAX_NOSIRE ; // Numero de SIRET employeur                                V(14)
#define MAX_NOURSF ; // Numero URSSAF employeur                                  V(15)
#define MAX_CMEMP ;  // Commentaire employeur                                    V(80)
#define MAX_NOETPR ; // Code etablissement de provenance                         V(5)
#define MAX_LBETAP ; // Libelle etablissement de provenance                      V(40)
#define MAX_ADREEP ; // Adresse (No et rue) etab. de provenance                  V(32)
#define MAX_COADEP ; // Adresse (Complement) etab. de provenance                 V(32)
#define MAX_CDPOEP ; // Code postal etablissement de provenance                  V(5)
#define MAX_LBCOEP ; // Libelle commune etablissement de provenance              V(32)
#define MAX_LIACEP ; // Ligne d'acheminement etablissement de provenance         V(26)
#define MAX_NOFINP ; // No FINESS etablissement de provenance                    V(9)
#define MAX_NODAPR ; // NDA de provenance                                        V(9)
#define MAX_TYTRPR ; // Type transfert de provenance                             V(1)
#define MAX_NOHODN ; // Code etablissement de destination                        V(5)
#define MAX_LBETAD ; // Libelle etablissement de destination                     V(40)
#define MAX_ADREED ; // Adresse (No et rue) etab. destination ou adr. sortie     V(32)
#define MAX_COADED ; // Adresse (complement) etab. destination ou adr. sortie    V(32)
#define MAX_LBCOED ; // Libelle commune etab. destination ou adr. sortie         V(32)
#define MAX_CDPOED ; // Code postal etab. destination ou adr. sortie             V(5)
#define MAX_LIACED ; // Ligne d'acheminement etab. destination ou adr. sortie    V(26)
#define MAX_NOFIND ; // No FINESS etablissement de destination                   V(9)
#define MAX_COIDED ; // Complement d'identite sortie                             V(32)
#define MAX_HHENTR ; // Heure d'entree                                           V(4)
#define MAX_MDENTR ; // Mode d'entree                                            V(1)
#define MAX_CDTREN ; // Transport d'entree                                       V(1)
#define MAX_NOUFEN ; // UH d'entree                                           O  V(3)
#define MAX_HHSOR ;  // Heure de sortie                                          V(4)
#define MAX_MDSOR ;  // Mode de sortie                                           V(1)
#define MAX_TYSOR ;  // Type de sortie                                           V(1)
#define MAX_NOUFSO ; // UH de sortie                                             V(3)
#define MAX_CDTRSO ; // Transport de sortie                                      V(1)
#define MAX_CDUSAD ; // Gestionnaire admissionniste du dossier                   V(8)
#define MAX_CDUSFS ; // Gestionnaire frais de sejour du dossier                  V(8)
#define MAX_CDCLOA ; // Indicateur de cloture administrative                     V(1)
#define MAX_NODAOR ; // NODA d'origine                                           V(9)#endif // __NS_ORACLE_H

