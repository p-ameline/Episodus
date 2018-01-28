//---------------------------------------------------------------------------#include <owl\olemdifr.h>
#include <owl\applicat.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>

#include "partage\Nsglobal.h"
#include "nautilus\nssuper.h"

#define __NSREPARE_CPP

#include "ns_sgbd\ns_sgbd.h"
#include "ns_sgbd\nsrepare.h"
#include "ns_sgbd\ns_sgbd.rh"

//***********************************************************************//
//							Classe NSRepareTable
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSRepareTable, NSUtilDialog)
  EV_COMMAND(IDC_BVERIFIE, lanceVerifie),
  EV_COMMAND(IDC_BREPARE, lanceRepare),
END_RESPONSE_TABLE;

NSRepareTable::NSRepareTable(TWindow* parent, NSContexte* pCtx,
									string sTbl, string sWarn, OWL::TModule* module)
              :NSUtilDialog(parent, pCtx, "IDD_REPARE", module)
{
  sTable 	 = sTbl;
  sWarning = sWarn;

	pNomTable 	 	  = new TStatic(this, IDC_STABLE);
  pWarning  	 	  = new TStatic(this, IDC_SMESSAGE);

	pVerifie  	 	  = new TButton(this, IDC_BVERIFIE);
  pRepare	  	 	  = new TButton(this, IDC_BREPARE);

  pEntetProgressBar = new TGauge(this, IDC_PRGBAR_ENTETE);
  pEntetProgressBar->SetRange(0, 100);
  pEntetProgressBar->SetLed(4, 80);
  pEntetProgressBar->SetStep(1);

  pIndexProgressBar = new TGauge(this, IDC_PRGBAR_INDEX);
  pIndexProgressBar->SetRange(0, 100);
  pIndexProgressBar->SetLed(4, 80);
  pIndexProgressBar->SetStep(1);

  pDonneProgressBar = new TGauge(this, IDC_PRGRBAR_DATA);
  pDonneProgressBar->SetRange(0, 100);
  pDonneProgressBar->SetLed(4, 80);
  pDonneProgressBar->SetStep(1);

  pReparProgressBar = new TGauge(this, IDC_PRGBAR_REPARE);
  pReparProgressBar->SetRange(0, 100);
  pReparProgressBar->SetLed(4, 80);
  pReparProgressBar->SetStep(1);

  pVerifEvolution   = new TStatic(this, IDC_VERIF_MSG);
  pReparEvolution   = new TStatic(this, IDC_REPARE_MSG);

  pBDEUtil = new TBDEUtil();
  pBDEUtil->pMaitre = this;
  pBDEUtil->hDb 	  = pContexte->getSuperviseur()->getDBHandle() ;
}

NSRepareTable::~NSRepareTable()
{
  delete pBDEUtil ;

	delete pNomTable ;
  delete pWarning ;

	delete pVerifie ;
  delete pRepare ;

  delete pEntetProgressBar ;
  delete pIndexProgressBar ;
  delete pDonneProgressBar ;
  delete pReparProgressBar ;

  delete pVerifEvolution ;
  delete pReparEvolution ;
}

void
NSRepareTable::SetupWindow()
{
	TDialog::SetupWindow();

    resetVerifyGauge();
    resetRepareGauge();

    pNomTable->SetText(sTable.c_str());
    pWarning->SetText(sWarning.c_str());

    pVerifEvolution->SetText("");
    pReparEvolution->SetText("");
}

voidNSRepareTable::lanceVerifie()
{
  int Msg;
  AnsiString Table;
  unsigned short far* temp = new unsigned short(pBDEUtil->vhTSes);
  ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
  resetVerifyGauge();
  donneVerifyMessage("Message : Démarrage de la vérification");

  TUCheck(TUExit(pBDEUtil->vhTSes));  TUCheck(TUInit(temp));

	if (pBDEUtil->RegisterCallBack())  {

       	if (TUCheck(TUVerifyTable(pBDEUtil->vhTSes, (char*)(sTable.c_str()), szPARADOX,
           	"VERIFY.DB", NULL, 0, (unsigned int*)&Msg)) == DBIERR_NONE)
	    {
		   	switch(Msg)
        	{
    	   		case 0:
              		donneVerifyMessage("Vérification réussie. Aucune erreur détectée.");
	               	break;

    	        case 1:
        	       	donneVerifyMessage("Vérification réussie. Vérification terminée.");
            	    break;

		        case 2:
        	      	donneVerifyMessage("Vérification réussie. La vérification n'a pas aboutit.");
            	    break;

	            case 3:
    	          	donneVerifyMessage("Vérification réussie. La table doit être reconstruite manuellement.");
        	        break;

	            case 4:
    	           	donneVerifyMessage("Vérification réussie. La table ne peut pas être reconstruite.");
        	        break;
			}
	    }
	    else
	    	donneVerifyMessage("Vérification impossible.");

		pBDEUtil->UnRegisterCallBack();
        delete temp;
	}
    ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
}
voidNSRepareTable::lanceRepare()
{
	unsigned short iFld, iIdx, iSec, iVal, iRI, iOptP, iOptD ;
  // DBIResult rslt;
  int Msg ;
  CRTblDesc TblDesc ;
  AnsiString Backup ;
  unsigned short far* temp = new unsigned short(pBDEUtil->vhTSes) ;

  ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

  resetRepareGauge();
  TUCheck(TUExit(pBDEUtil->vhTSes));
  TUCheck(TUInit(temp));

  if (pBDEUtil->RegisterCallBack())
  {
    if (TUCheck(TUVerifyTable(pBDEUtil->vhTSes, (char*)(sTable.c_str()),
        			szPARADOX, "VERIFY.DB",NULL, 0, (unsigned int*)&Msg)) == DBIERR_NONE)
    {
      if (TUCheck(TUGetCRTblDescCount(pBDEUtil->vhTSes, (char*)(sTable.c_str()), &iFld,
	        			&iIdx, &iSec, &iVal, &iRI, &iOptP, &iOptD)) == DBIERR_NONE)
      {
        memset(&TblDesc, 0, sizeof(CRTblDesc));
        strcpy(TblDesc.szTblName, (char*)(sTable.c_str()));
        strcpy(TblDesc.szTblType, szPARADOX);
        strcpy(TblDesc.szErrTblName, "rebuild.db");

				TblDesc.iFldCount = iFld;
        TblDesc.pfldDesc = (pFLDDesc) malloc (iFld * sizeof(FLDDesc));

        TblDesc.iIdxCount = iIdx;
				TblDesc.pidxDesc = (pIDXDesc) malloc (iIdx * sizeof (IDXDesc));

        TblDesc.iSecRecCount = iSec;
        TblDesc.psecDesc = (pSECDesc) malloc (iSec * sizeof(SECDesc));

        TblDesc.iValChkCount = iVal;
        TblDesc.pvchkDesc = (pVCHKDesc) malloc (iVal * sizeof(VCHKDesc));

        TblDesc.iRintCount = iRI;
        TblDesc.printDesc = (pRINTDesc) malloc (iOptD * sizeof(RINTDesc));

        TblDesc.iOptParams = iOptP;
        TblDesc.pfldOptParams = (pFLDDesc) malloc (iOptP * sizeof(FLDDesc));

        TblDesc.pOptData = (BYTE*) malloc(iOptD * DBIMAXSCFLDLEN);

        if (TUCheck(TUFillCRTblDesc(pBDEUtil->vhTSes, &TblDesc, (char*)(sTable.c_str()),
        	    	NULL)) == DBIERR_NONE)
        {
          Backup = "Backup.db" ;

          if (TUCheck(TURebuildTable(pBDEUtil->vhTSes, (char*)(sTable.c_str()), szPARADOX,
    	            			Backup.c_str(), "KEYVIOL.DB", "PROBLEM.DB",
        	                    &TblDesc)) == DBIERR_NONE)
            donneRepareMessage("Reconstruction réussie");
          else
            donneRepareMessage("Echec de la reconstruction");
        }
        else
          MessageBox("Error Filling table structure.", "Error", MB_OK);
        free(TblDesc.pfldDesc);
				free(TblDesc.pidxDesc);
        free(TblDesc.psecDesc);
        free(TblDesc.pvchkDesc);
        free(TblDesc.printDesc);
        free(TblDesc.pfldOptParams);
        free(TblDesc.pOptData);
      }
      else
        MessageBox("Error getting description count.", "Error", MB_OK);

      pBDEUtil->UnRegisterCallBack();
      delete temp;
    }
    else
    {
      donneRepareMessage("Problème système");
      pBDEUtil->UnRegisterCallBack();
      delete temp;
    }
  }
  ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
}


void
NSRepareTable::resetVerifyGauge()
{
	pEntetProgressBar->SetValue(0);
    pIndexProgressBar->SetValue(0);
    pDonneProgressBar->SetValue(0);
}

void
NSRepareTable::resetRepareGauge()
{
	pReparProgressBar->SetValue(0);
}

/*

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainDialog *MainDialog;
//---------------------------------------------------------------------------
__fastcall TMainDialog::TMainDialog(TComponent* Owner)
	: TForm(Owner)
{
	Session1->Active = true;
    OpenDataBaseList();
    BDEUtil = new TBDEUtil();
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::AboutBtnClick(TObject *Sender)
{
	AboutDialog->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::OpenDataBaseList()
{
	hDBICur TmpCursor = 0;
    DBDesc vDBDesc;

    AliasCombo->Items->Clear();
    Check(DbiOpenDatabaseList(TmpCursor));
    Check(DbiSetToBegin(TmpCursor));
    while (DbiGetNextRecord(TmpCursor, dbiNOLOCK, (void*)&vDBDesc, NULL) == DBIERR_NONE)
    {
    	if (stricmp(vDBDesc.szDbType, "STANDARD") == 0)
        	AliasCombo->Items->Add(vDBDesc.szName);
    }
	if (TmpCursor !=0)
	    Check(DbiCloseCursor(TmpCursor));
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::ClearBars()
{
	MessageLB->Caption = "";
    PBHeader->Position = 0;
    PBIndexes->Position = 0;
    PBData->Position = 0;
    PBRebuild->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::ClearLabels()
{
	FieldsLB->Caption = '0';
    RecSizeLB->Caption = '0';
    IndexLB->Caption = '0';
    ValidLB->Caption = '0';
    RefLB->Caption = '0';
    RestructLB->Caption = '0';
    AuxPassLB->Caption = '0';
    CodePageLB->Caption = '0';
    BlockSizeLB->Caption = '0';
    TabLvlLB->Caption = '0';
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::ClearTable()
{
	TableLocEdit->Text = "";
    VerifyBtn->Enabled = false;
    RebuildBtn->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::SetTable(char* TableName)
{
	TableLocEdit->Text = TableName;
    VerifyBtn->Enabled = true;
    RebuildBtn->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::SetTableAndDir(bool ByDirectory)
{
	DBDesc vDBDesc;
    AnsiString Alias, Table, DirTable;
    char* tempBuf = new char[255];

    Alias = AliasCombo->Items->Strings[AliasCombo->ItemIndex];
    Table = TableCombo->Items->Strings[TableCombo->ItemIndex];

    Check(DbiGetDatabaseDesc(Alias.c_str(), &vDBDesc));
	sprintf(tempBuf, "%s\\%s", vDBDesc.szPhyName, Table.c_str());
    SetTable(tempBuf);
    ClearBars();
    SetTableInfo();

    delete [] tempBuf;
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::SetTableInfo()
{
	AnsiString Buffer, Table;

    Table = TableLocEdit->Text;

    if (BDEUtil->GetTCursorProps(Table.c_str()))
    {
    	FieldsLB->Caption = IntToStr(BDEUtil->tuProps.iFields);
        RecSizeLB->Caption = IntToStr(BDEUtil->tuProps.iRecBufSize);
        IndexLB->Caption = IntToStr(BDEUtil->tuProps.iIndexes);
        ValidLB->Caption = IntToStr(BDEUtil->tuProps.iValChecks);
        RefLB->Caption = IntToStr(BDEUtil->tuProps.iRefIntChecks);
        RestructLB->Caption = IntToStr(BDEUtil->tuProps.iRestrVersion);
        AuxPassLB->Caption = IntToStr(BDEUtil->tuProps.iPasswords);
        CodePageLB->Caption = IntToStr(BDEUtil->tuProps.iCodePage);
        BlockSizeLB->Caption = IntToStr(BDEUtil->tuProps.iBlockSize);
        TabLvlLB->Caption = IntToStr(BDEUtil->tuProps.iTblLevel);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::FormDestroy(TObject *Sender)
{
	delete BDEUtil;
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::AliasComboChange(TObject *Sender)
{
	// Database1->Connected = false;
    // Database1->AliasName = AliasCombo->Items[AliasCombo->ItemIndex].Text;
    //Database1->Connected = true;
	Session1->GetTableNames(AliasCombo->Items->Strings[AliasCombo->ItemIndex],
    			"*.db", true, false, TableCombo->Items);

    ClearBars();
    ClearLabels();
    ClearTable();
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::ByDirectBtnClick(TObject *Sender)
{
	if (OpenDialog1->Execute())
    {
    	SetTable(OpenDialog1->FileName.c_str());
        AliasCombo->ItemIndex = -1;
        TableCombo->Items->Clear();
        ClearBars();
        SetTableInfo();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::TableComboChange(TObject *Sender)
{
	SetTableAndDir(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::ExitBtnClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::VerifyBtnClick(TObject *Sender)
{
	int Msg;
    AnsiString Table;
    unsigned short far* temp = new unsigned short(BDEUtil->vhTSes);

    Screen->Cursor = crHourGlass;

 	ClearBars();
    Table = TableLocEdit->Text;
    TUCheck(TUExit(BDEUtil->vhTSes));
    TUCheck(TUInit(temp));
	if (BDEUtil->RegisterCallBack())
    {
       	if (TUCheck(TUVerifyTable(BDEUtil->vhTSes, Table.c_str(), szPARADOX,
           	"VERIFY.DB", NULL, 0, (unsigned int*)&Msg)) == DBIERR_NONE)
	    {
		   	switch(Msg)
        	{
    	   		case 0:
              		MessageLB->Caption = "Verification Successful.  Table has no erros.";
	               	break;

    	        case 1:
        	       	MessageLB->Caption = "Verification Successful.  Verification completed.";
            	    break;

		        case 2:
        	      	MessageLB->Caption = "Verification Successful.  Verification could not be completed.";
            	    break;

	            case 3:
    	          	MessageLB->Caption = "Verification Successful.  Table must be rebuilt manually.";
        	        break;

	            case 4:
    	           	MessageLB->Caption = "Verification Successful.  Table cannot be rebuilt.";
        	        break;
			}
	    }
	    else
	    	MessageLB->Caption = "Verification unSuccessful.";

		BDEUtil->UnRegisterCallBack();
		Screen->Cursor = crDefault;
        delete temp;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainDialog::RebuildBtnClick(TObject *Sender)
{
	unsigned short iFld, iIdx, iSec, iVal, iRI, iOptP, iOptD;
    AnsiString szTable;
    DBIResult rslt;
    int Msg;
    CRTblDesc TblDesc;
    AnsiString Backup;
    unsigned short far* temp = new unsigned short(BDEUtil->vhTSes);

    Screen->Cursor = crHourGlass;

    ClearBars();
    TUCheck(TUExit(BDEUtil->vhTSes));
    TUCheck(TUInit(temp));
    szTable = TableLocEdit->Text;
    if (BDEUtil->RegisterCallBack())
    {
    	if (TUCheck(TUVerifyTable(BDEUtil->vhTSes, szTable.c_str(),
        			szPARADOX, "VERIFY.DB",NULL, 0, (unsigned int*)&Msg)) == DBIERR_NONE)
        {
	        if (TUCheck(TUGetCRTblDescCount(BDEUtil->vhTSes, szTable.c_str(), &iFld,
	        			&iIdx, &iSec, &iVal, &iRI, &iOptP, &iOptD)) == DBIERR_NONE)
	        {
    	    	memset(&TblDesc, 0, sizeof(CRTblDesc));
        	    strcpy(TblDesc.szTblName, szTable.c_str());
            	strcpy(TblDesc.szTblType, szPARADOX);
	            strcpy(TblDesc.szErrTblName, "rebuild.db");

				TblDesc.iFldCount = iFld;
        	    TblDesc.pfldDesc = (pFLDDesc) malloc (iFld * sizeof(FLDDesc));

	            TblDesc.iIdxCount = iIdx;
				TblDesc.pidxDesc = (pIDXDesc) malloc (iIdx * sizeof (IDXDesc));

	            TblDesc.iSecRecCount = iSec;
    	        TblDesc.psecDesc = (pSECDesc) malloc (iSec * sizeof(SECDesc));

	            TblDesc.iValChkCount = iVal;
	            TblDesc.pvchkDesc = (pVCHKDesc) malloc (iVal * sizeof(VCHKDesc));

    	        TblDesc.iRintCount = iRI;
        	    TblDesc.printDesc = (pRINTDesc) malloc (iOptD * sizeof(RINTDesc));

	            TblDesc.iOptParams = iOptP;
    	        TblDesc.pfldOptParams = (pFLDDesc) malloc (iOptP * sizeof(FLDDesc));

	            TblDesc.pOptData = (BYTE*) malloc(iOptD * DBIMAXSCFLDLEN);

    	        if (TUCheck(TUFillCRTblDesc(BDEUtil->vhTSes, &TblDesc, szTable.c_str(),
        	    	NULL)) == DBIERR_NONE)
            	{
            		Backup = "Backup.db";

	                if (TUCheck(TURebuildTable(BDEUtil->vhTSes, szTable.c_str(), szPARADOX,
    	            			Backup.c_str(), "KEYVIOL.DB", "PROBLEM.DB",
        	                    &TblDesc)) == DBIERR_NONE)
            	    {
                		MessageLB->Caption = "Rebuild was successful";
	                }
	                else
	                {
	                	MessageLB->Caption = "Rebuild was not successful";
	                }
	            }
	            else
	            {
	            	MessageBox(0, "Error Filling table structure.", "Error", MB_OK);
	            }
                free(TblDesc.pfldDesc);
				free(TblDesc.pidxDesc);
		        free(TblDesc.psecDesc);
	    	    free(TblDesc.pvchkDesc);
	        	free(TblDesc.printDesc);
		        free(TblDesc.pfldOptParams);
		        free(TblDesc.pOptData);
	        }
            else
            {
            	MessageBox(0, "Error getting description count.", "Error", MB_OK);
            }

	        BDEUtil->UnRegisterCallBack();
	        Screen->Cursor = crDefault;
            delete temp;
	    }
    }
    else
    {
    	MessageLB->Caption = "Verification unSuccessfulk.";
        BDEUtil->UnRegisterCallBack();
        Screen->Cursor = crDefault;
        delete temp;
    }
}
//---------------------------------------------------------------------------

*/

