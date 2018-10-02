//---------------------------------------------------------------------------//     NSCPTA.H
//
//  Contient les définitions des bases Compta, Fact, TPayant
//
//  1ere version : RS Aout 98   Dernière modif : Sept 98
//---------------------------------------------------------------------------
#ifndef __NSCPTA_H
#define __NSCPTA_H

#include <cstring.h>
#include "ns_sgbd\nsfiche.h"
#include "partage\nsperson.h"

#if defined(_DANSCPTADLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

//
// Taille des champs du fichier COMPT
//

#define CPTA_NUMCOMPT_LEN  		9
#define CPTA_NUMFSE_LEN  	    9
#define CPTA_PATIENT_LEN 	    PIDS_NSS_LEN
#define CPTA_DATE_LEN 		    8
#define CPTA_HEURE_LEN  	    4
#define CPTA_DUE_F_LEN  	    7
#define CPTA_PAYE_F_LEN  	    7
#define CPTA_DEPAS_F_LEN			7
#define CPTA_DUE_E_LEN  	    7
#define CPTA_PAYE_E_LEN  	    7
#define CPTA_DEPAS_E_LEN 			7
#define CPTA_EXAMEN_LEN  	    5
#define CPTA_SYNONYME_LEN  		1
#define CPTA_OKPAYE_LEN  	    1
#define CPTA_PRESCRIPT_LEN  	PIDS_NSS_LEN
#define CPTA_CONTEXTE_LEN    	5
#define CPTA_CODE_LEN  	     	10
#define CPTA_OPERATEUR_LEN    PIDS_NSS_LEN

//
// Indice des champs du fichier COMPT
//
#define CPTA_NUMCOMPT_FIELD     1
#define CPTA_NUMFSE_FIELD  	    2
#define CPTA_PATIENT_FIELD 	    3
#define CPTA_DATE_FIELD 				4
#define CPTA_HEURE_FIELD  			5
#define CPTA_DUE_F_FIELD  			6
#define CPTA_PAYE_F_FIELD  	    7
#define CPTA_DEPAS_F_FIELD  		8
#define CPTA_DUE_E_FIELD  			9
#define CPTA_PAYE_E_FIELD      10
#define CPTA_DEPAS_E_FIELD     11
#define CPTA_EXAMEN_FIELD      12
#define CPTA_SYNONYME_FIELD    13
#define CPTA_OKPAYE_FIELD      14
#define CPTA_PRESCRIPT_FIELD   15
#define CPTA_CONTEXTE_FIELD    16
#define CPTA_CODE_FIELD  	  	 17
#define CPTA_OPERATEUR_FIELD   18

//
// Taille des champs du fichier FACT
//

#define FACT_NUMCOMPT_LEN  	    9
#define FACT_NUMERO_LEN					2
#define FACT_OPERATEUR_LEN  		PIDS_NSS_LEN
#define FACT_DATE_PAIE_LEN 	    8
#define FACT_ORGANISME_LEN     11
#define FACT_LIBELLE_LEN	   	 15
#define FACT_MONTANT_LEN    		7
#define FACT_UNITE_LEN    			3
#define FACT_MODE_PAIE_LEN 	    2

//
// Indice des champs du fichier FACT
//
#define FACT_NUMCOMPT_FIELD     1
#define FACT_NUMERO_FIELD	    	2
#define FACT_OPERATEUR_FIELD    3
#define FACT_DATE_PAIE_FIELD 		4
#define FACT_ORGANISME_FIELD    5
#define FACT_LIBELLE_FIELD			6
#define FACT_MONTANT_FIELD      7
#define FACT_UNITE_FIELD    		8
#define FACT_MODE_PAIE_FIELD 		9

//
// Taille des champs du fichier TPAYANT
//
#define TPAY_NUMCOMPT_LEN  	    9
#define TPAY_NUMERO_LEN         2
#define TPAY_ORGANISME_LEN     11
#define TPAY_LIBELLE_LEN       15
#define TPAY_RESTE_DU_LEN       7
#define TPAY_MONNAIE_LEN        3
#define TPAY_OKPAYE_LEN         2

//
// Indice des champs du fichier TPAYANT
//
#define TPAY_NUMCOMPT_FIELD 	  1
#define TPAY_NUMERO_FIELD		    2
#define TPAY_ORGANISME_FIELD    3
#define TPAY_LIBELLE_FIELD		  4
#define TPAY_RESTE_DU_FIELD     5
#define TPAY_MONNAIE_FIELD  	  6
#define TPAY_OKPAYE_FIELD 		  7

//
// Taille des champs du fichier CODEORGA
//
#define ORGA_CODE_LEN			     11
#define ORGA_LIB_COURT_LEN		 15
#define ORGA_LIB_LONG_LEN		   70
#define ORGA_ADRESSE_LEN	    160

//
// Indice des champs du fichier CODEORGA
//
#define ORGA_CODE_FIELD			    1
#define ORGA_LIB_COURT_FIELD	  2
#define ORGA_LIB_LONG_FIELD	    3
#define ORGA_ADRESSE_FIELD		  4

//
// Taille des champs du fichier CODEREG
//
#define REG_CODE_LEN		        9
#define REG_LIB_COURT_LEN	     15
#define REG_LIB_LONG_LEN	     70
#define REG_DESTINATAIRE_LEN	  3
#define REG_CENTRE_INFO_LEN	    3

//
// Indice des champs du fichier CODEREG
//
#define REG_CODE_FIELD		      1
#define REG_LIB_COURT_FIELD		  2
#define REG_LIB_LONG_FIELD	    3
#define REG_DESTINATAIRE_FIELD  4
#define REG_CENTRE_INFO_FIELD   5

//
// Taille des champs du fichier NSM_CPT_DOCU
//
#define CPTDOCU_NUMCOMPT_LEN		CPTA_NUMCOMPT_LEN
#define CPTDOCU_INDICE_LEN	    2
#define CPTDOCU_PATIENT_LEN     PIDS_NSS_LEN
#define CPTDOCU_DOCUMENT_LEN	  DOC_CODE_DOCUM_LEN

//
// Indice des champs du fichier NSM_CPT_DOCU
//
#define CPTDOCU_NUMCOMPT_FIELD	1
#define CPTDOCU_INDICE_FIELD		2
#define CPTDOCU_PATIENT_FIELD		3
#define CPTDOCU_DOCUMENT_FIELD	4

//---------------------------------------------------------------------------
//  Classe NSCompt
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSComptData
{
	public :

		//
		// Variables de stockage des valeurs
		//
		char numcompt[CPTA_NUMCOMPT_LEN + 1] ;
		char numfse[CPTA_NUMFSE_LEN + 1] ;
		char patient[CPTA_PATIENT_LEN + 1] ;
		char date[CPTA_DATE_LEN + 1] ;
		char heure[CPTA_HEURE_LEN + 1] ;
		char duFranc[CPTA_DUE_F_LEN + 1] ;
		char payeFranc[CPTA_PAYE_F_LEN + 1] ;
		char depassFranc[CPTA_DEPAS_F_LEN + 1] ;
		char duEuro[CPTA_DUE_E_LEN + 1] ;
		char payeEuro[CPTA_PAYE_E_LEN + 1] ;
		char depassEuro[CPTA_DEPAS_E_LEN + 1] ;
		char examen[CPTA_EXAMEN_LEN + 1] ;
		char synonyme[CPTA_SYNONYME_LEN + 1] ;
		char okPaye[CPTA_OKPAYE_LEN + 1] ;
		char prescript[CPTA_PRESCRIPT_LEN + 1] ;
		char contexte[CPTA_CONTEXTE_LEN + 1] ;
		char code[CPTA_CODE_LEN + 1] ;
		char operateur[CPTA_OPERATEUR_LEN + 1] ;

		NSComptData() ;
		NSComptData(const NSComptData& rv) ;
		~NSComptData() ;

		NSComptData& operator=(const NSComptData& src) ;
		int          operator==(const NSComptData& o) ;

		void metAZero() ;

    string getNumCompt()    const { return string(numcompt) ;    }
		string getNumFse()      const { return string(numfse) ;      }
		string getPatientId()   const { return string(patient) ;     }
		string getDate()        const { return string(date) ;        }
		string getTime()        const { return string(heure) ;       }
		string getDueLocal()    const { return string(duFranc) ;     }
		string getPaidLocal()   const { return string(payeFranc) ;   }
		string getDepassLocal() const { return string(depassFranc) ; }
		string getDueEuro()     const { return string(duEuro) ;      }
		string getPaidEuro()    const { return string(payeEuro) ;    }
		string getDepassEuro()  const { return string(depassEuro) ;  }
		string getExam()        const { return string(examen) ;      }
		string getSynonym()     const { return string(synonyme) ;    }
		string getFullyPaid()   const { return string(okPaye) ;      }
		string getPrescriber()  const { return string(prescript) ;   }
		string getContext()     const { return string(contexte) ;    }
		string getCode()        const { return string(code) ;        }
		string getOperator()    const { return string(operateur) ;   }

    bool   isPayed()  const { return ('1' == okPaye[0]) ; }
    bool   isDepass() const { return (('\0' != depassFranc[0]) || ('\0' != depassEuro[0])) ; }
};

//// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCompt : public NSFiche
{
	public :

		//
    // Objet qui contient les données
    //
    NSComptData _Donnees ;

    NSCompt(NSSuper* pSuper) ;
    NSCompt(const NSCompt& rv) ;
    ~NSCompt() ;

		void         alimenteFiche() ;
    void         videFiche() ;
    DBIResult    open() ;
    DBIResult    getPatRecord() ;

    virtual bool Create() ;
    virtual bool Modify() ;

    void metAZero() { _Donnees.metAZero() ; }

    NSCompt&     operator=(const NSCompt& src) ;
    int 	       operator==(const NSCompt& o) ;

    string getNumCompt()    const { return _Donnees.getNumCompt() ;    }
		string getNumFse()      const { return _Donnees.getNumFse() ;      }
		string getPatientId()   const { return _Donnees.getPatientId() ;   }
		string getDate()        const { return _Donnees.getDate() ;        }
		string getTime()        const { return _Donnees.getTime() ;        }
		string getDueLocal()    const { return _Donnees.getDueLocal() ;    }
		string getPaidLocal()   const { return _Donnees.getPaidLocal() ;   }
		string getDepassLocal() const { return _Donnees.getDepassLocal() ; }
		string getDueEuro()     const { return _Donnees.getDueEuro() ;     }
		string getPaidEuro()    const { return _Donnees.getPaidEuro() ;    }
		string getDepassEuro()  const { return _Donnees.getDepassEuro() ;  }
		string getExam()        const { return _Donnees.getExam() ;        }
		string getSynonym()     const { return _Donnees.getSynonym() ;     }
		string getFullyPaid()   const { return _Donnees.getFullyPaid() ;   }
		string getPrescriber()  const { return _Donnees.getPrescriber() ;  }
		string getContext()     const { return _Donnees.getContext() ;     }
		string getCode()        const { return _Donnees.getCode() ;        }
		string getOperator()    const { return _Donnees.getOperator() ;    }

    bool   isPayed()        const { return _Donnees.isPayed() ;        }
    bool   isDepass()       const { return _Donnees.isDepass() ;       }
};

class NSFse16Array ;

//---------------------------------------------------------------------------//  Classe NSComptInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSComptInfo
{
	public :

		//		// Objet qui contient les données
		//		NSComptData _Donnees ;

		NSComptInfo() ;
		NSComptInfo(const NSCompt* pCompt) ;
		NSComptInfo(const NSComptInfo& rv) ;
    ~NSComptInfo() ;

    bool InitFse1610Array(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte) ;
		bool InitFse1620Array(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte) ;
   	bool InitFse1630Array(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte) ;
    bool InitCCAMArray(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte) ;
    bool InitLibreArray(NSFse16Array* pFseArray, int* pCounter, NSContexte* pContexte) ;

    void metAZero() { _Donnees.metAZero() ; }

		NSComptInfo& operator=(const NSComptInfo& src) ;		int          operator==(const NSComptInfo& o) ;

    string getNumCompt()    const { return _Donnees.getNumCompt() ;    }
		string getNumFse()      const { return _Donnees.getNumFse() ;      }
		string getPatientId()   const { return _Donnees.getPatientId() ;   }
		string getDate()        const { return _Donnees.getDate() ;        }
		string getTime()        const { return _Donnees.getTime() ;        }
		string getDueLocal()    const { return _Donnees.getDueLocal() ;    }
		string getPaidLocal()   const { return _Donnees.getPaidLocal() ;   }
		string getDepassLocal() const { return _Donnees.getDepassLocal() ; }
		string getDueEuro()     const { return _Donnees.getDueEuro() ;     }
		string getPaidEuro()    const { return _Donnees.getPaidEuro() ;    }
		string getDepassEuro()  const { return _Donnees.getDepassEuro() ;  }
		string getExam()        const { return _Donnees.getExam() ;        }
		string getSynonym()     const { return _Donnees.getSynonym() ;     }
		string getFullyPaid()   const { return _Donnees.getFullyPaid() ;   }
		string getPrescriber()  const { return _Donnees.getPrescriber() ;  }
		string getContext()     const { return _Donnees.getContext() ;     }
		string getCode()        const { return _Donnees.getCode() ;        }
		string getOperator()    const { return _Donnees.getOperator() ;    }

    bool   isPayed()        const { return _Donnees.isPayed() ;        }
    bool   isDepass()       const { return _Donnees.isDepass() ;       }
};

//// Définition de NSComptArray (Array de NSComptInfo(s))
//
typedef vector<NSComptInfo*> NSFicheComptArray ;
typedef NSFicheComptArray::iterator       NSComptIter ;
typedef NSFicheComptArray::const_iterator NSComptConstIter ;

class NSComptArray : public NSFicheComptArray
{
	public :

		// Constructeurs
    NSComptArray() : NSFicheComptArray() {}
    NSComptArray(const NSComptArray& rv) ;
    // Destructeur
    virtual ~NSComptArray() ;
    void vider() ;

    NSComptArray& operator=(const NSComptArray& src) ;
};

//------------------------------ Fact.db -------------------------------------//
//---------------------------------------------------------------------------
//  Classe NSFactData
//---------------------------------------------------------------------------

//// Objet contenant les données
//
class _CLASSELEXI NSFactData
{
	public :

		//    // Variables de stockage des valeurs
    //
    char numcompt[FACT_NUMCOMPT_LEN + 1] ;
    char numero[FACT_NUMERO_LEN + 1] ;
    char operateur[FACT_OPERATEUR_LEN + 1] ;
    char date_paie[FACT_DATE_PAIE_LEN + 1] ;
    char organisme[FACT_ORGANISME_LEN + 1] ;
    char libelle[FACT_LIBELLE_LEN + 1] ;
    char montant[FACT_MONTANT_LEN + 1] ;
    char unite[FACT_UNITE_LEN + 1] ;
    char mode_paie[FACT_MODE_PAIE_LEN + 1] ;

    NSFactData() ;
    NSFactData(const NSFactData& rv) ;
    ~NSFactData() ;

    NSFactData& operator=(const NSFactData& src) ;
    int 			 operator==(const NSFactData& o) ;

    void metAZero() ;

    string getNumCompt() const { return string(numcompt) ;  }
    string getFactId()   const { return string(numero) ;    }
    string getOperator() const { return string(operateur) ; }
    string getPaidDate() const { return string(date_paie) ; }
    string getPayor()    const { return string(organisme) ; }
    string getLabel()    const { return string(libelle) ;   }
    string getSum()      const { return string(montant) ;   }
    string getUnit()     const { return string(unite) ;     }
    string getMode()     const { return string(mode_paie) ; }
};

//---------------------------------------------------------------------------//  Classe NSFact
//---------------------------------------------------------------------------

//// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSFact : public NSFiche
{
	public :

    NSFactData _Donnees ;

    NSFact(NSSuper* pSuper) ;
    NSFact(const NSFact& rv) ;
    ~NSFact() ;

    void alimenteFiche() ;
    void videFiche() ;
    DBIResult open() ;

    virtual bool Create() ;    virtual bool Modify() ;

    NSFact& operator=(const NSFact& src) ;
    int 	  operator==(const NSFact& o) ;

    string getNumCompt() const { return _Donnees.getNumCompt() ; }
    string getFactId()   const { return _Donnees.getFactId() ;   }
    string getOperator() const { return _Donnees.getOperator() ; }
    string getPaidDate() const { return _Donnees.getPaidDate() ; }
    string getPayor()    const { return _Donnees.getPayor() ;    }
    string getLabel()    const { return _Donnees.getLabel() ;    }
    string getSum()      const { return _Donnees.getSum() ;      }
    string getUnit()     const { return _Donnees.getUnit() ;     }
    string getMode()     const { return _Donnees.getMode() ;     }
};

//---------------------------------------------------------------------------//  Classe NSFactInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSFactInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSFactData _Donnees ;

		NSFactInfo() ;
		NSFactInfo(const NSFact* pFact) ;
		NSFactInfo(const NSFactInfo& rv) ;
    ~NSFactInfo() ;

		NSFactInfo& operator=(const NSFactInfo& src) ;		int 		    operator==(const NSFactInfo& o) ;

    string getNumCompt() const { return _Donnees.getNumCompt() ; }
    string getFactId()   const { return _Donnees.getFactId() ;   }
    string getOperator() const { return _Donnees.getOperator() ; }
    string getPaidDate() const { return _Donnees.getPaidDate() ; }
    string getPayor()    const { return _Donnees.getPayor() ;    }
    string getLabel()    const { return _Donnees.getLabel() ;    }
    string getSum()      const { return _Donnees.getSum() ;      }
    string getUnit()     const { return _Donnees.getUnit() ;     }
    string getMode()     const { return _Donnees.getMode() ;     }
};

//// Définition de NSFactArray (Array de NSFactInfo(s))
//
typedef vector<NSFactInfo*> NSFicheFactArray ;
typedef NSFicheFactArray::iterator       NSFactIter ;
typedef NSFicheFactArray::const_iterator NSFactConstIter ;

class NSFactArray : public NSFicheFactArray
{
	public :

		// Constructeurs		NSFactArray() : NSFicheFactArray() {}
		NSFactArray(const NSFactArray& rv) ;
		// Destructeur
		virtual ~NSFactArray() ;  	void vider() ;

    NSFactArray& operator=(const NSFactArray& src) ;
};

//---------------------------------------------------------------------------
//  Classe NSComptDocu
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSComptDocuData
{
	public :

		//
		// Variables de stockage des valeurs
		//
		char numcompt[CPTDOCU_NUMCOMPT_LEN + 1] ;
		char indice[CPTDOCU_INDICE_LEN + 1] ;
    char patient[CPTDOCU_PATIENT_LEN + 1] ;
		char document[CPTDOCU_DOCUMENT_LEN + 1] ;

		NSComptDocuData() ;
		NSComptDocuData(const NSComptDocuData& rv) ;
		~NSComptDocuData() ;

		NSComptDocuData& operator=(const NSComptDocuData& src) ;
		int              operator==(const NSComptDocuData& o) ;

		void metAZero() ;

    string getNumCompt()   const { return string(numcompt) ; }
    string getIndex()      const { return string(indice) ;   }
    string getPatientId()  const { return string(patient) ;  }
		string getDocumentId() const { return string(document) ; }
};

//// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSComptDocu : public NSFiche
{
	public :

		//
    // Objet qui contient les données
    //
    NSComptDocuData _Donnees ;

    NSComptDocu(NSSuper* pSuper) ;
    NSComptDocu(const NSComptDocu& rv) ;
    ~NSComptDocu() ;

		void         alimenteFiche() ;
    void         videFiche() ;
    DBIResult    open() ;
    DBIResult    getPatRecord() ;

    virtual bool Create() ;
    virtual bool Modify() ;

    NSComptDocu& operator=(const NSComptDocu& src) ;
    int 	       operator==(const NSComptDocu& o) ;
};

//---------------------------------------------------------------------------
//  Classe NSComptInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSComptDocuInfo
{
	public :

		//		// Objet qui contient les données
		//		NSComptDocuData _Donnees ;

		NSComptDocuInfo() ;
		NSComptDocuInfo(const NSComptDocu* pComptDocu) ;
		NSComptDocuInfo(const NSComptDocuInfo& rv) ;
    ~NSComptDocuInfo() ;

		NSComptDocuInfo& operator=(const NSComptDocuInfo& src) ;
		int              operator==(const NSComptDocuInfo& o) ;
};

//// Définition de NSComptArray (Array de NSComptInfo(s))
//
typedef vector<NSComptDocuInfo*> NSComptDocuVector ;
typedef NSComptDocuVector::iterator       NSComptDocuIter ;
typedef NSComptDocuVector::const_iterator NSComptDocuConstIter ;

class _CLASSELEXI NSComptDocuArray : public NSComptDocuVector
{
	public :

		// Constructeurs
    NSComptDocuArray() : NSComptDocuVector() {}
    NSComptDocuArray(const NSComptDocuArray& rv) ;
    // Destructeur
    virtual ~NSComptDocuArray() ;
    void vider() ;

    NSComptDocuArray& operator=(const NSComptDocuArray& src) ;
};

//---------------------------- TPayant.db -----------------------------------//
//---------------------------------------------------------------------------
//  Classe NSTPayantData
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSTPayantData
{
	public :

   	//    // Variables de stockage des valeurs
    //
    char numcompt[TPAY_NUMCOMPT_LEN + 1] ;
    char numero[TPAY_NUMERO_LEN + 1] ;
    char organisme[TPAY_ORGANISME_LEN + 1] ;
    char libelle[TPAY_LIBELLE_LEN + 1] ;
    char reste_du[TPAY_RESTE_DU_LEN + 1] ;
    char monnaie[TPAY_MONNAIE_LEN + 1] ;
    char okpaye[TPAY_OKPAYE_LEN + 1] ;

    NSTPayantData() ;    NSTPayantData(const NSTPayantData& rv) ;
    ~NSTPayantData() ;

    NSTPayantData& operator=(const NSTPayantData& src) ;
    int            operator==(const NSTPayantData& o) ;

    void metAZero() ;};
//---------------------------------------------------------------------------
//  Classe NSTPayant
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSTPayant : public NSFiche
{
  public :

	  //	  // Objet qui contient les données
	  //
	  NSTPayantData _Donnees ;

	  NSTPayant(NSSuper* pSuper) ;
		NSTPayant(const NSTPayant& rv) ;
	  ~NSTPayant() ;

	  void alimenteFiche() ;
	  void videFiche() ;
	  DBIResult open() ;

	  virtual bool Create() ;
	  virtual bool Modify() ;

		NSTPayant& operator=(const NSTPayant& src) ;
		int 	     operator==(const NSTPayant& o) ;
};

//---------------------------------------------------------------------------//  Classe NSTPayantInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSTPayantInfo
{
	public :

		//		// Objet qui contient les données
		//
		NSTPayantData _Donnees ;

		NSTPayantInfo() ;
		NSTPayantInfo(const NSTPayant* pTPayant) ;
		NSTPayantInfo(const NSTPayantInfo& rv) ;
    ~NSTPayantInfo() ;

		NSTPayantInfo& operator=(const NSTPayantInfo& src) ;
		int 		       operator==(const NSTPayantInfo& o) ;
};

//// Définition de NSTPArray (Array de NSTPayantInfo(s))
//
typedef vector<NSTPayantInfo*> NSTPayantArray ;
typedef NSTPayantArray::iterator       NSTPIter ;
typedef NSTPayantArray::const_iterator NSTPConstIter ;

class NSTPArray : public NSTPayantArray
{
	public :

		// Constructeurs		NSTPArray() : NSTPayantArray() {}
		NSTPArray(const NSTPArray& rv) ;
		// Destructeur
		virtual ~NSTPArray() ;
		void vider() ;

    NSTPArray& operator=(const NSTPArray& src) ;
};

//---------------------------- CodeOrga.db -----------------------------------//
//---------------------------------------------------------------------------
//  Classe NSCodeOrgaData
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSCodeOrgaData
{
	public :

   	//
		// Variables de stockage des valeurs
    //
    char code[ORGA_CODE_LEN + 1] ;
    char lib_court[ORGA_LIB_COURT_LEN + 1] ;
    char lib_long[ORGA_LIB_LONG_LEN + 1] ;
    char adresse[ORGA_ADRESSE_LEN + 1] ;

    NSCodeOrgaData() ;
    NSCodeOrgaData(const NSCodeOrgaData& rv) ;
    ~NSCodeOrgaData() ;

    NSCodeOrgaData& operator=(const NSCodeOrgaData& src) ;
    int 			      operator==(const NSCodeOrgaData& o) ;

    void metABlanc() ;
    void metAZero() ;
};

//---------------------------------------------------------------------------//  Classe NSCodeOrga
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCodeOrga : public NSFiche
{
  public :

	  //	  // Objet qui contient les données
	  //
	  NSCodeOrgaData _Donnees ;

	  NSCodeOrga(NSSuper* pSuper);
		NSCodeOrga(const NSCodeOrga& rv);
	  ~NSCodeOrga();

	  void metABlanc() { _Donnees.metABlanc() ; }

	  void alimenteFiche();
	  void videFiche();
	  DBIResult open();

	  virtual bool Create();
	  virtual bool Modify();

		NSCodeOrga& operator=(const NSCodeOrga& src) ;
		int 	      operator==(const NSCodeOrga& o) ;
};

//---------------------------------------------------------------------------//  Classe NSCodeOrgaInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCodeOrgaInfo
{
	public :

		//		// Objet qui contient les données
		//
		NSCodeOrgaData _Donnees ;

		NSCodeOrgaInfo() ;
		NSCodeOrgaInfo(const NSCodeOrga* pCodeOrga) ;
		NSCodeOrgaInfo(const NSCodeOrgaInfo& rv);
    ~NSCodeOrgaInfo();

		NSCodeOrgaInfo& operator=(const NSCodeOrgaInfo& src) ;
		int 		        operator==(const NSCodeOrgaInfo& o) ;
};

//// Définition de NSCodeOrgaArray (Array de NSCodeOrgaInfo(s))
//
typedef vector<NSCodeOrgaInfo*> NSOrganArray ;
typedef NSOrganArray::iterator       NSCodeOrgaIter ;
typedef NSOrganArray::const_iterator NSCodeOrgaConstIter ;

class NSCodeOrgaArray : public NSOrganArray{
	public :

		// Constructeurs		NSCodeOrgaArray() : NSOrganArray() {}
		NSCodeOrgaArray(const NSCodeOrgaArray& rv) ;
		// Destructeur
		virtual ~NSCodeOrgaArray() ;
   	void vider() ;

    NSCodeOrgaArray& operator=(const NSCodeOrgaArray& src) ;
};

//---------------------------- CodeRegime.db -----------------------------------//
//---------------------------------------------------------------------------
//  Classe NSCodeRegimeData
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSCodeRegimeData
{
	public :

		//    // Variables de stockage des valeurs
    //
    char code[REG_CODE_LEN + 1];
    char lib_court[REG_LIB_COURT_LEN + 1];
    char lib_long[REG_LIB_LONG_LEN + 1];
    char destinataire[REG_DESTINATAIRE_LEN + 1];
    char centre_info[REG_CENTRE_INFO_LEN + 1];

    NSCodeRegimeData() ;
    NSCodeRegimeData(const NSCodeRegimeData& rv) ;
    ~NSCodeRegimeData() ;

    NSCodeRegimeData& operator=(const NSCodeRegimeData& src) ;
    int 			        operator==(const NSCodeRegimeData& o) ;

    void metAZero() ;
};

//---------------------------------------------------------------------------//  Classe NSCodeRegime
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCodeRegime : public NSFiche
{
  public :

	  //	  // Objet qui contient les données
	  //
	  NSCodeRegimeData _Donnees ;

		NSCodeRegime(NSSuper* pSuper) ;
    NSCodeRegime(const NSCodeRegime& rv) ;
	  ~NSCodeRegime() ;

	  void metAZero() { _Donnees.metAZero() ; }

	  void alimenteFiche() ;	  void videFiche() ;
	  DBIResult open() ;

	  virtual bool Create() ;
	  virtual bool Modify() ;

		NSCodeRegime& operator=(const NSCodeRegime& src) ;
		int 	        operator==(const NSCodeRegime& o) ;
};

//---------------------------------------------------------------------------//  Classe NSCodeRegimeInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCodeRegimeInfo
{
	public :

		//		// Objet qui contient les données
		//
		NSCodeRegimeData _Donnees ;

		NSCodeRegimeInfo() ;
		NSCodeRegimeInfo(const NSCodeRegime* pCodeRegime) ;
		NSCodeRegimeInfo(const NSCodeRegimeInfo& rv) ;
    ~NSCodeRegimeInfo() ;

		NSCodeRegimeInfo& operator=(const NSCodeRegimeInfo& src) ;
		int               operator==(const NSCodeRegimeInfo& o) ;
};

//// Définition de NSCodeRegimeArray (Array de NSCodeRegimeInfo(s))
//
typedef vector<NSCodeRegimeInfo*> NSRegimeArray ;
typedef NSRegimeArray::iterator       NSCodeRegimeIter ;
typedef NSRegimeArray::const_iterator NSCodeRegimeConstIter ;

class NSCodeRegimeArray : public NSRegimeArray
{
	public :

		// Constructeurs		NSCodeRegimeArray() : NSRegimeArray() {}
		NSCodeRegimeArray(const NSCodeRegimeArray& rv) ;
		// Destructeur
		virtual ~NSCodeRegimeArray() ;
   	void vider() ;

    NSCodeRegimeArray& operator=(const NSCodeRegimeArray& src) ;
};

//---------------------------------------------------------------------------#endif    // __NSCPTA_H

