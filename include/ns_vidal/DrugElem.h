// -----------------------------------------------------------------------------
// Drugs.h
// -----------------------------------------------------------------------------
// construction des bases liées au médicament avec comme source le VIDAL
// -----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Author: fabrice $
// $Date: 2004/05/12 17:49:17 $
// -----------------------------------------------------------------------------
// FLP - avril/mai 2004
// -----------------------------------------------------------------------------


#ifndef __DRUGELEM_H__
# define __DRUGELEM_H__


# ifndef __DANSVIDALDLL__
#  define __VIDALEXPORT__	__import
# else
#  define __VIDALEXPORT__	__export
# endif

# ifndef _OWLDLL
#  include <stl.h>
# else
#  include <vector.h>
# endif

# include <string.h>

# include "ns_vidal\DrugComponent.h"


class VidalMaster ;
class CDComponentExplorer ;


// -----------------------------------------------------------------------------
// Classe médicament de base. Toutes les classes de médicaments dérivent de
// cette classe de base.
// --
// Basic Drug Class. All Drug classes inherited from this one. 
// -----------------------------------------------------------------------------
class DrugBase
{
 public:
  // constructors/destructor
  DrugBase() ;
  DrugBase(DrugBase& src) ;
  ~DrugBase() ;

  // operator
  DrugBase                operator=(DrugBase& src) ;

  // functions
  bool                    isP(DrugComponent *pCompo) ;

  string                  getFatherLexiCode()                     { return sFatherLexiCode ; }
  string                  getLabel()                              { return sLabel ; }
  string                  getLexiCode()                           { return sLexiCode ; }

  void                    SetFather(DrugBase *pDad)               { pFather = pDad ; }

	void										setLexiCode(string sCode)               { sLexiCode = sCode ; }

  DrugBase                *getFather()                            { return pFather ; }
  ArrayOfDrugComponents   *getAIs()                               { return pActiveIngredients ; }
  ArrayOfDrugComponents   *getExcipients()                        { return pExcipients ; }
  ArrayOfDrugComponents   *getEENs()                              { return pEENs ; }

  void                    addAI       (DrugComponent *pCompo)     { pActiveIngredients->push_back(new DrugComponent(*pCompo)) ;  }
  void                    addExcipient(DrugComponent *pCompo)     { pExcipients->push_back(new DrugComponent(*pCompo)) ; }
  void                    addEEN      (DrugComponent *pCompo)     { pEENs->push_back(new DrugComponent(*pCompo)) ; }

  void                    processFLink(VidalMaster *pVM, DrugBase *pFather) ;
  void                    processSLink(VidalMaster *pVM, DrugBase *pSon) ;
  void                    processFLink(VidalMaster *pVM, DrugComponent *pCompo) ;

  bool                    saveInDB      (VidalMaster *pVM) ;
  bool                    saveInLexiMed (VidalMaster *pVM) ;
  bool                    saveInBdm     (VidalMaster *pVM) ;

 protected:
  // variables
  string                  sFatherLexiCode ;
  string                  sLabel ;
  string                  sLexiCode ;

  DrugBase                *pFather ;
  ArrayOfDrugComponents   *pActiveIngredients ;
  ArrayOfDrugComponents   *pExcipients ;
  ArrayOfDrugComponents   *pEENs ;
} ;


// -----------------------------------------------------------------------------
// Classe médicament élémentaire. Cette classe correspond à un médicament sous
// sa forme : médicament + forme + dosage + présentation.
// --
// Elementary Drug class. This class concern a Drug in the form : Drug + form +
// dose + presentation.
// -----------------------------------------------------------------------------
class	DrugElem : public DrugBase
{
 public:
  // constructors/destructor
	DrugElem() ;
	DrugElem(DrugElem& src) ;
	DrugElem(string sPtr) ;
	~DrugElem() ;

  // define enum
  enum                    USAGE { notDefined = -1, normal = 0, HOP, IH, IHA, IH6, IH3, IH1, IA, I6, I } ;
                          // HOP : hopital
                          // IH  : prescription Initiale Hospitalière
                          // IHA : prescription Initiale Hospitalière Annuelle
                          // IH6 : prescription Initiale Hospitalière de 6 mois
                          // IH3 : prescription Initiale Hospitalière de 3 mois
                          // IH1 : (erreur ??) prescription Initiale Hospitalière de 6 mois
                          // IA  : prescription Initiale Annuelle
                          // I6  : prescription Initiale de 6 mois
                          // I   : prescription Initiale et Renouvellement

  // operator
	DrugElem								operator=(DrugElem& src) ;

  // functions
	string									convertOn7Dgts(string sPtr) ;
	bool										init(VidalMaster *pVM) ;
	int											initComponents() ;

  void                    processLinks(VidalMaster *pVM) ;

  string                  getForm()                                             { return sForm ; }
  string                  getDose()                                             { return sDose ; }
  string                  getCIPCode()                                          { return sDrugCIP ; }
  string                  getVIDALPtr()                                         { return sDrugPtr ; }

  bool                    isP(DrugComponent *pCompo) ;
  bool                    saveInDB  (VidalMaster *pVM) ;
  bool                    saveInBdm (VidalMaster *pVM) ;

 protected:
  // variables
	string									sDrugPtr ;
	string									sDrugCIP ;
  string                  sForm ;
  string                  sDose ;
	string									sDescription ;
	string									sComponents ;
  USAGE                   iUsage ;
} ;


typedef vector<DrugElem *>			VectorOfDrugs ;
typedef VectorOfDrugs::iterator	DrugIter ;


// -----------------------------------------------------------------------------
// Liste de médicaments élémentaires.
// --
// List of elementary drugs.
// -----------------------------------------------------------------------------
class ArrayOfDrugs : public VectorOfDrugs
{
 public:
	ArrayOfDrugs() ;
	ArrayOfDrugs(ArrayOfDrugs& src) ;
	~ArrayOfDrugs() ;

	ArrayOfDrugs&						operator=(ArrayOfDrugs& src) ;
	int											init(string sArray) ;
	void										vider() ;

  bool                    isP         (DrugComponent *pCompo) ;
  bool                    isPinAll    (DrugComponent *pCompo) ;
  bool                    saveInDB(VidalMaster *pVM) ;
} ;


// -----------------------------------------------------------------------------
// Classe médicament élémentaire sous une forme et un dosage. Cette classe
// correspond à un médicament sous sa forme : médicament + forme + dosage.
// --
// Elementary Drug class with Form and Dose. This class concern a Drug in the
// form : Drug + form + dose.
// -----------------------------------------------------------------------------
class DrugFormDose : public DrugBase
{
 public:
  // constructor/destructor
  DrugFormDose() ;
  DrugFormDose(string sLabel, string sFormT, string sDoseT) ;
  DrugFormDose(DrugFormDose& src) ;
  ~DrugFormDose() ;

  // ioerator
  DrugFormDose            operator=(DrugFormDose& src) ;

  // functions
  string                  getForm()                                             { return sForm ; }
  string                  getDose()                                             { return sDose ; }
  string                  getLabelDoseForm()                                    { return sLabel ; }

  bool                    isP         (DrugComponent *pCompo) ;
  bool                    isPinAll    (DrugComponent *pCompo) ;
  bool                    processCompo(CDComponentExplorer *pExplorer) ;
  bool                    saveInDB(VidalMaster *pVM) ;

  // variables
	ArrayOfDrugs						*pDrugElems ;

 protected:
  string                  sForm ;
  string                  sDose ;
} ;


typedef vector<DrugFormDose *>            VectorOfDrugsFormDose ;
typedef VectorOfDrugsFormDose::iterator   DrugFormDoseIter ;


// -----------------------------------------------------------------------------
// Liste de médicaments ayant une forme.
// --
// List of Drugs with form.
// -----------------------------------------------------------------------------
class DrugsFormDoseArray : public VectorOfDrugsFormDose
{
 public:
  // constructor/destructor
  DrugsFormDoseArray() ;
  DrugsFormDoseArray(DrugsFormDoseArray& src) ;
  ~DrugsFormDoseArray() ;

  // operator
  DrugsFormDoseArray      operator=(DrugsFormDoseArray& src) ;

  // functions
  void                    vider() ;

  bool                    isP         (DrugComponent *pCompo) ;
  bool                    isPinAll    (DrugComponent *pCompo) ;
  bool                    saveInDB(VidalMaster *pVM) ;

  bool                    isFormDoseP(string sForm, string sDose) ;
  DrugFormDose            *getFormDoseElem(string sForm, string sDose) ;
} ;


// -----------------------------------------------------------------------------
// Classe médicament élémentaire sous une forme Cette classe correspond à un
// médicament sous sa forme : médicament + forme.
// --
// Elementary Drug class with Form. This class concern a Drug in the form : Drug
// + form .
// -----------------------------------------------------------------------------
class DrugForm : public DrugBase
{
 public:
  // constructor/destructor
  DrugForm() ;
  DrugForm(string sLabel, string sFormT) ;
  DrugForm(DrugForm& src) ;
  ~DrugForm() ;

  // operator
  DrugForm                operator=(DrugForm& src) ;

  // functions
  string                  getForm()                                             { return sForm ; }
  string                  getLabelForm()                                        { return sLabel ; }

  bool                    isP         (DrugComponent *pCompo) ;
  bool                    isPinAll    (DrugComponent *pCompo) ;
  bool                    processCompo(CDComponentExplorer *pExplorer) ;
  bool                    saveInDB(VidalMaster *pVM) ;

	ArrayOfDrugs						*pDrugElems ;
  DrugsFormDoseArray      *pDrugFormDoseElems ;

 protected:
  string                  sForm ;
} ;


typedef vector<DrugForm *>                VectorOfDrugsForm ;
typedef VectorOfDrugsForm::iterator       DrugFormIter ;


// -----------------------------------------------------------------------------
// Liste de médicaments ayant une forme.
// --
// List of Drugs with a form.
// -----------------------------------------------------------------------------
class DrugsFormArray : public VectorOfDrugsForm
{
 public:
  // constructor/destructor
  DrugsFormArray() ;
  DrugsFormArray(DrugsFormArray& src) ;
  ~DrugsFormArray() ;

  // operator
  DrugsFormArray          operator=(DrugsFormArray& src) ;

  // functions
  void                    vider() ;

  bool                    isP         (DrugComponent *pCompo) ;
  bool                    isPinAll    (DrugComponent *pCompo) ;
  bool                    saveInDB(VidalMaster *pVM) ;

  bool                    isFormP(string sForm) ;
  DrugForm                *getFormElem(string sForm) ;
} ;


// -----------------------------------------------------------------------------
// Médicament commercial.
// --
// Commercial Drug.
// -----------------------------------------------------------------------------
class	CommercialDrugElem : public DrugBase
{
 public:
  // constructors/destructor
	CommercialDrugElem() ;
	CommercialDrugElem(string sLibelle) ;
	CommercialDrugElem(CommercialDrugElem& src) ;
	~CommercialDrugElem() ;

  // operator
	CommercialDrugElem&		  operator=(CommercialDrugElem& src) ;

  // functions
	void										process() ;

  bool                    isP         (DrugComponent *pCompo) ;
  bool                    isPinAll    (DrugComponent *pCompo) ;
  bool                    processCompo(CDComponentExplorer *pExplorer) ;

  void                    saveInDB(VidalMaster *pVM) ;
//  void                    processFLink(DrugComponent *pCompo) ;

  // variables
	ArrayOfDrugs						*pDrugElems ;
  DrugsFormArray          *pDrugFormElems ;
  DrugsFormDoseArray      *pDrugFormDoseElems ;
} ;


typedef vector<CommercialDrugElem *>			VectorOfCommercialDrugs ;
typedef VectorOfCommercialDrugs::iterator	CommercialDrugIter ;


// -----------------------------------------------------------------------------
// Liste des médicaments commerciaux.
// --
// List of commercial drugs.
// -----------------------------------------------------------------------------
class	ArrayOfCommercialDrugs : public VectorOfCommercialDrugs
{
 public:
  // constructors/destructor
	ArrayOfCommercialDrugs() ;
	ArrayOfCommercialDrugs(ArrayOfCommercialDrugs& src) ;
	~ArrayOfCommercialDrugs() ;

  // operator
	ArrayOfCommercialDrugs&	operator=(ArrayOfCommercialDrugs& src) ;

  // functions
	int											init(string sArray) ;
	void										vider() ;
} ;


typedef vector<DrugBase *>                  DrugBaseVector ;
typedef DrugBaseVector::iterator            DrugBaseIter ;

// -----------------------------------------------------------------------------
// Classe qui explore un médicament et qui par rapport à un composant référence
// les médicaments qui le contiennent.
// --
// Class that explore a drug and from a component references all drugs that
// contain it.
// -----------------------------------------------------------------------------
class CDComponentExplorer
{
 public:
  // constructors/destructor
  CDComponentExplorer() ;
  CDComponentExplorer(DrugComponent *pCompo) ;
  CDComponentExplorer(CDComponentExplorer& src) ;
  ~CDComponentExplorer() ;

  // operator
  CDComponentExplorer&    operator=(CDComponentExplorer& src) ;

  void                    retrieve(DrugBase *dBase) ;

  // variables
  DrugComponent           *pComponent ;
  DrugBaseVector          *pDrugs ;
} ;


typedef vector<CDComponentExplorer *>       CDComponentExplorerVector ;
typedef CDComponentExplorerVector::iterator CDComponentExplorerIter ;


// -----------------------------------------------------------------------------
// Liste d'explorateur de médicaments par rapport à un composant.
// --
// Drug Explorers list from a component.
// -----------------------------------------------------------------------------
class ComponentExplorerArray : public CDComponentExplorerVector
{
 public:
  // constructors/destructor
  ComponentExplorerArray() ;
  ComponentExplorerArray(ArrayOfDrugs       *pDArray) ;
  ComponentExplorerArray(DrugsFormArray     *pDFArray) ;
  ComponentExplorerArray(DrugsFormDoseArray *pDFDArray) ;
  ComponentExplorerArray(ComponentExplorerArray& src) ;
  ~ComponentExplorerArray() ;

  // operator
  ComponentExplorerArray& operator=(ComponentExplorerArray& src) ;

  // functions
  void                    add(ComponentExplorerArray *pCEArray) ;

  bool                    isP(string sItem, DrugComponent::DCType type) ;
  CDComponentExplorer     *getComponent(string sItem, DrugComponent::DCType type) ;
} ;


#endif // __DRUGELEM_H__
