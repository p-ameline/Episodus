// --------------------------------------------------------------------------// NSEPISOD.H		Document/Vues Capture Donn�es Utilisateur
// --------------------------------------------------------------------------
// Fabrice LE PERRU - Aout 2001
// --------------------------------------------------------------------------
#ifndef __NSEPISOD_H__
#define __NSEPISOD_H__

#include <owl\dialog.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nssavoir\nscaptur.h"

//--------------------------------------------------------------------------
//
// Classe EpisodusElemData
//
//--------------------------------------------------------------------------

//class ChildEpisodusData
class EpisodusElemData
{
    public:

        unsigned long   AppWindow;
        unsigned long   AppWndChild;
        string          sClassChild;
        string          sTextChild;
        string          sCorrespondence;
        ClassLib::TRect Placement;

        // Constructeurs - Destructeur
        EpisodusElemData();
        EpisodusElemData(EpisodusElemData &rv);
        EpisodusElemData(EpisodusElemData *pNewEpisodusElemData);
        EpisodusElemData(string sXMLBaliseContenu);
        EpisodusElemData(THandle aWindow, THandle aWndChild, string aClassChild, string aTextChild, ClassLib::TRect aPlacement, string aCorrespondence = "");
        ~EpisodusElemData();

        bool    ImportModelXML(string sXMLBaliseContenu);

        // fonction de comparaison
        bool    CompareWithElementModel(EpisodusElemData *);

	    EpisodusElemData& operator=(EpisodusElemData src);
};


// d�finition d'un array repr�sentant la fen�tre de capture
typedef vector<EpisodusElemData*>   ArrayEpisodusData;
typedef ArrayEpisodusData::iterator ArrayEpisodusDataIter;

class EpisodusData : public ArrayEpisodusData
{
    public:

        // Constructeurs - Destructeur
        EpisodusData();
        EpisodusData(EpisodusData& rv);
        EpisodusData(EpisodusData *pNewEpisodusData);
        EpisodusData(string sXMLBaliseContenu);
        ~EpisodusData();

        // Fonctions de Correspondance
        bool    SetName(int index);
        bool    SetLastName(int index);
        bool    SetSex(int index);
        bool    SetDayOfBirth(int index);
        bool    SetMedicalData(int index);

        bool    SetElement(int index, string sLabel);

        // fonction d'importation exportation
        bool    ExportModel(ofstream* pOutFile);
        bool    ImportModel();
        bool    ImportModel(string sXMLBaliseContenu);
        bool    Model2View(EpisodusData *pEpisodDataView);

        bool    SaveModel();

        // fonctions de comparaison
        bool    CompareWithModel(EpisodusData *);

        void    vider();

	    EpisodusData& operator=(EpisodusData src);
};

typedef EpisodusData::iterator  EpisodusDataIter;

class EpisodusModel : public EpisodusData
{
    public:

        string      sNom;
        bool        bModified;

        EpisodusModel();
        EpisodusModel(EpisodusModel& rv);
        EpisodusModel(EpisodusData *pNewEpisodusData) : EpisodusData(pNewEpisodusData) {}
        EpisodusModel(string sXMLBaliseContenu, string sXMLBaliseVariables);
        ~EpisodusModel();

        void donneCaptureElemts(NSCaptureArray* pCaptArray, EpisodusData *pData);
        bool ExportModel(ofstream* pOutFile);

	    EpisodusModel& operator=(EpisodusModel src);
};

// d�finition d'un array de mod�les
typedef vector<EpisodusModel*> ArrayEpisodusModelesData;
typedef ArrayEpisodusModelesData::iterator ArrayEpisodusModelIter;

class ArrayEpisodusModelData : public ArrayEpisodusModelesData
{
    public:

        // Constructeurs - Destructeur
        ArrayEpisodusModelData() : ArrayEpisodusModelesData() {}
        ArrayEpisodusModelData(ArrayEpisodusModelData& rv);
        ~ArrayEpisodusModelData();

        EpisodusModel* trouveModele(EpisodusData* pEpisodusData);

        bool    sauvegarder();

        void    vider();
	    ArrayEpisodusModelData& operator=(ArrayEpisodusModelData src);
};

//--------------------------------------------------------------------------
//
// Classe MylParam
// Sert pour l'envoi � la fonction d'�num�ration
//
//--------------------------------------------------------------------------

class MylParam
{
    public:

        THandle     AppWindow;
        THandle     theMainWindow;
        NSContexte  *pContexte;

        // Constructeurs - Destructeur
        MylParam();
        MylParam(THandle hWnd, THandle mainWindow, NSContexte *pCtx)
            {AppWindow = hWnd; theMainWindow = mainWindow; pContexte = pCtx;}
        ~MylParam() {;}
};

class NSEpisodView;

//--------------------------------------------------------------------------
//
// Classe NSEpisodus
// Classe de gestion des fonctions Episodus
//
//--------------------------------------------------------------------------
class NSEpisodus : public NSRoot
{
    public:

        // tableau correspondant � la capture
        EpisodusData            *pEpisodusData;

        // pointeur sur la vue
        NSEpisodView            *pViewData;

        // tableau de mod�les
        ArrayEpisodusModelData  *pModels;        // pointeur sur la liste de mod�les
        EpisodusModel           *pCurrentModel;  // pointeur sur le mod�le courant

        NSCaptureArray          CaptureArray;

        // Constructeur - Destructeur   	    NSEpisodus(NSContexte* pCtx);        ~NSEpisodus();

        // fonctions de capture
        void    Capture(HWND HWnd);             // fonction de capture standard
        void    CaptureOld(HWND HWnd);
        bool    CaptureHookOK(HWND HWnd);       // fonction permettant de savoir si une capture de donn�es est possible

        HWND    hOldWindowCaptured;

        // Hook de la souris
        void    lanceHook();        // lancement du hook
        void    fermeHook();        // arr�t du hook

        // r�cup�ration de la position de la souris
        POINT   *getPMousePoint() {return &MousePoint;}

        // taches � effectuer en cas de changement d'utilisateur
        void    UserChanged();

        // fonction d'importation - exportation de mod�les
        bool    ImportModels(bool verbose = true);
        bool    ExportModels();

        // fonction de log
        void    logSpyMessage(string sSpyMessage);

        // fonctions d'�num�ration
        static bool FAR PASCAL _export  NSEpisodus::SearchChildWindow(HWND hWnd, LPARAM lParam);
        static bool FAR PASCAL _export  NSEpisodus::SearchChildWindowRecur(HWND hWnd, LPARAM lParam);

        void createViewData(EpisodusModel* pModel);

        void reveilleToi();

        // fonctions pour l'�tat du module Episodus
        bool    GetStateOn()                            {return bStateOn;}
        void    SetStateOn(bool bStateChange)           {bStateOn = bStateChange;}

        // fonctions controllant l'�tat de la capture � partir du hook de la souris
        bool    GetMouseHookState()                     {return bMouseHookState;}
        void    SetMouseHookState(bool bStateChange)    {bMouseHookState = bStateChange;}

        // fonctions
        bool    GetModelFound()                         {return bModelFound;}
        void    SetModelFound(bool bStateChange)        {bModelFound = bStateChange;}

       // fonctions
        bool    GetLearning()                           {return bLearning;}
        void    SetLearning(bool bLearn)                {bLearning = bLearn;}

        void    SetPhareOn();
        void    SetPhareOff();

        void    resetCaptureArray()                     {CaptureArray.vider();}

    protected:
        int         CM_MOUSE_HOOK;
        POINT       MousePoint;                     // emplacement de la souris
        bool        bStateOn;                       // �tat du module Episodus
        bool        bMouseHookState;                // �tat du hook
        bool        bModelFound;                    // mod�le trouv�
        bool        bLearning;                      // mode apprentissage
};

class NSModelDlg : public TDialog, public NSRoot
{
	public :

        OWL::TEdit*     pNewLib;
        OWL::TButton*   pCreer;

        OWL::TListBox*  pLibelles;
   	    OWL::TButton*   pModifier;
   	    OWL::TButton*   pSupprimer;

        EpisodusModel*  pModel;

   	    NSModelDlg(TWindow* pere, NSContexte* pCtx, EpisodusModel* pModel);
   	    ~NSModelDlg();

   	    void CmOk();
   	    // void CmCancel();
   	    void SetupWindow();

        void creer();
        void modifier();
        void supprimer();

        void rempliListe();

    DECLARE_RESPONSE_TABLE(NSModelDlg);
};

//--------------------------------------------------------------------------
//
// Classe NSDPIO
// Classe de gestion des fonctions DPIO
//
//--------------------------------------------------------------------------
class NSDPIO : public NSRoot
{
    public:

        bool    bMinimalInterface ;

        bool    bLogPage ;
        string  sLogPage ;

        int     iPosX ;
        int     iPosY ;
        int     iDeltaPosX ;
        int     iDeltaPosY ;

        int     iCurrentX ;
        int     iCurrentY ;

        // Constructeur - Destructeur
   	    NSDPIO(NSContexte* pCtx);        ~NSDPIO();

} ;


#endif // #ifndef __NSEPISOD_H__

