// -----------------------------------------------------------------------------
// nsmedicphase.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:03:17 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#ifndef __NSMEDICPHASE_H__
# define __NSMEDICPHASE_H__


# include "nsbb\nsExport.h"
# include "nsbb\nspatpat.h"
# include "nsbb\nsmediccycle.h"


class NSMedicDlg ;
class NSparseMedic ;



class NSphaseMedic
{
 public:
 /*
 ** Cree une phase
 ** Relie a la fenetre de medicament NSMedicamentDlg   pou l'affichage
 ** auto_cycle : Sert a indiquer si on doit cree autamatiquement un cycle ou pas
 **               - true pour la creation
 **               - false dans le cas contraire
 */
 NSphaseMedic(NSMedicamentDlg *pDlg, bool auto_cycle = true) ;
  NSphaseMedic(NSphaseMedic& src) ;
  ~NSphaseMedic() ;

  NSphaseMedic&       operator=(NSphaseMedic src) ;
  int                 operator==(NSphaseMedic& src) ;

  //void                reinitDialog() ;
  //void                initFromTree(NSPatPathoArray *pPPT, PatPathoIter *pptIter) ;
  //void                createTree(/*NSPatPathoArray *pPPT,*/ int iColonne) ;


  bool                isValid() ;


  NSMedicCycleGlobal*  operator[](int index) ;                               // Permet d'acceder a un Cycle
  void                 AddCycle() ;                                          // Permet de rajouter un cycle
  bool                 Load(PatPathoIter& pPPT, PatPathoIter& pptIter) ;  // Permet de charger une phase
  bool                 LoadDate(PatPathoIter& pptIter, PatPathoIter& pptIEnd, NVLdVTemps* pDate) ; // Cherche a charger la date de début
  bool                 LoadDuree(PatPathoIter& pptIter, PatPathoIter& pptIEnd) ; // Cherche a charger la duree d'une phase
  bool                 LoadRenouvellement(PatPathoIter& pptIter, PatPathoIter& pptIEnd) ; // Permet de charger le no mbre de renouvellement
  bool                 recupereDataForPhase(PatPathoIter& pptIter, PatPathoIter& pptIEnd, int Col) ; // Fonction de recuperationd 'erreur
  void                 save() ;                                              // Sauvegarde une phase
  void                 create() ;                                            // Creer une phase
  void                 Load() ;                                              // Charge la phase dans la fenetre
  int                  CycleNumber() ;                                       // Renvoie le nombre de cyle
  int                  GetIndexOfCyclePreview() { return _currentCycle ; }   // Renvoie le cycle courrant
  std::string          CheckIntegrity() ;                                    // Verifie que les donnée peuvent être enregistré
  NSMedicamentDlg*     GetWindow(){ return pDialog ; }                       // Obtient un pointeur sur le fentre
  void                 SaveCurrentCycle() ;                                  // Sauvegarde le cycle courrant
  void                 SetCurrentCycle(int index) ;                          // Mettre le cycle courrant
  void                 LinkToDialog() ;                                      // Relie la phase a la boite de dialogue
  void                 test() ;
  std::string          Decode() ;
  NSPatPathoArray*     SimplifiedTreeForInterpretation() ;                   //  Cree un arbre simplifie pour le decodeur
  void                 SetPhasePreview(int i) { _phasepreviewIndex = i ; }                             //  int temp
  int                  GetPhasePreviewIndex() { return  _phasepreviewIndex ; }  //  int windows
  NSPatPathoArray*     CreateTree() ;                                        //  cree l'arbre

  inline
  int                  GetDureePhase()    { return iDureeCycle ; }                 // nombre de cycle ! doit etre allier a u code
  inline
  std::string          GetSymBolOfPhase() { return  sUnitDureeCycle ; }
  inline
  NVLdVTemps           GetStartingDate()  { return  _tStartingDate ; }
  inline
  NVLdVTemps           GetClosingDate()   { return  _tClosingDate ; }

  inline
  int GetNumberOfRenouvellement() { return  iNbRenouvellement ; }

 protected:

	NVLdVTemps      _tStartingDate ;
  NVLdVTemps      _tClosingDate ;

  std::string     sLabel ;
  int             iDureeCycle ;
  std::string     sUnitDureeCycle ;      // correspond au code lexique
  int             iNbRenouvellement ;
  int             _phasepreviewIndex ;

  NSMedicamentDlg *pDialog ;      // Pointeur vers la boite de dialogue

  std::vector<NSMedicCycleGlobal * > *_Cycles ;       // Cycle dans la boite de dialogue
  bool            _Monocycle ;    // Cycle mono phase
  int             _currentCycle ; // Cyclecourrant
} ;

void createNodeComplement(NSPatPathoArray *pPPT, string sCode, string sUnit, string  sVal, int iColonne);

typedef vector<NSphaseMedic *>                NSphaseMedicVector ;
typedef NSphaseMedicVector::iterator          NSphaseMedicIter ;
typedef NSphaseMedicVector::reverse_iterator  NSphaseMedicRIter ;
typedef NSphaseMedicVector::const_iterator    NSphaseMedicConstIter ;


class NSphaseMedicArray : public NSphaseMedicVector
{
 public:
  NSphaseMedicArray() ;                           // constructors
  NSphaseMedicArray(NSphaseMedicArray& src) ;     // constructors par copie
  ~NSphaseMedicArray() ;                          // Destructeur

  NSphaseMedicArray&  operator=(NSphaseMedicArray src) ;
  int                 operator==(const NSphaseMedicArray& src) ;

  void                vider() ;
} ;


#endif // __NSMEDICPHASE_H__
