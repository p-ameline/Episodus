//---------------------------------------------------------------------------
#ifndef BDEUtilH
#define BDEUtilH
#include <bde.hpp>
//---------------------------------------------------------------------------
// TUtility (TUVerifyTable) Session Option
//
const TU_Append_Errors = 1;
const TU_No_Secondary  = 2;
const TU_No_Warnings   = 4;
const TU_Header_Only   = 8;
const TU_Dialog_Hide   = 16;
const TU_No_Lock       = 32;

// TUtility type definition
//
typedef unsigned short hTUses;
typedef hTUses far *phTUses ;
typedef Word *pUINT16;
typedef TIMESTAMP *pTIMESTAMP;
typedef BOOL *pBOOL;
//typedef CBList *PCBList;
//---------------------------------------------------------------------------
// Error checking functions
//
#define Checker(x) CheckDbiError(x, __FILE__, __LINE__)
#define TUCheck(x) CheckTUError(x, __FILE__, __LINE__)

DBIResult CheckDbiError(DBIResult, char*, unsigned long);
DBIResult CheckTUError(DBIResult, char*, unsigned long);
//---------------------------------------------------------------------------
// TUTility Function prototypes
//

extern "C"
{

	__declspec(dllexport) DBIResult __stdcall TUInit(phTUses hTUSession);

	__declspec(dllexport) DBIResult __stdcall TUVerifyTable(hTUses hTUSession,
				char far * pszTableName, char far *	pszDriverType,
                char far * pszErrTableName, char far * pszPassword,
				int iOptions, unsigned int far * piErrorLevel);

	__declspec(dllexport) DBIResult __stdcall TURebuildTable(hTUses hTUSession,
    			char far * pszTableName, char far * pszDriverType,
				char far * pszBackupTableName, char far * pszKeyviolName,
                char far * pszProblemTableName, pCRTblDesc pCrDesc);

	__declspec(dllexport) DBIResult	__stdcall TUGetCRTblDescCount(hTUses hTUSession,
   				char far * pszTableName, pUINT16 iFldCount,
				pUINT16	iIdxCount, pUINT16 iSecRecCount,
				pUINT16 iValChkCount, pUINT16 iRintCount,
				pUINT16 iOptParams, pUINT16 iOptDataLen);

	__declspec(dllexport) DBIResult	__stdcall TUFillCRTblDesc(hTUses hTUSession,
				pCRTblDesc pCrDesc, char far * pszTableName, char far *	pszPassword);

    __declspec(dllexport) DBIResult __stdcall TUFillCURProps(hTUses hTUSession,
				char far * pszTableName, pCURProps tblProps);

	__declspec(dllexport) DBIResult __stdcall TUGetExtTblProps(hTUses hTUSession,
				char far * pszTableName, pTIMESTAMP pTS, pBOOL pbReadOnly);

	__declspec(dllexport) DBIResult __stdcall TUExit(hTUses hTUSession);

	__declspec(dllexport) DBIResult __stdcall TUGetErrorString(DBIResult iErrorcode,
				char far *pszError);
}
//---------------------------------------------------------------------------

// TUtulity verify callback processes
//
typedef enum
	{TUVerifyHeader,
     TUVerifyIndex,
     TUVerifyData,
     TUVerifySXHeader,
     TUVerifySXIndex,
     TUVerifySXData,
     TUVerifySXIntegrity,
     TUVerifyTableName,
     TURebuild
	}TUVerifyProcess ;

// TUtility call back info for verifying call back functions.
//
typedef struct
{
	unsigned short PercentDone;
    DBIPATH TableName;
    TUVerifyProcess Process;
    unsigned short CurrentIndex;
    unsigned short TotalIndex;
}TUVerifyCallBack;
//---------------------------------------------------------------------------

class NSRepareTable;

class TBDEUtil
{
public:
	TBDEUtil();
    ~TBDEUtil();
    bool GetTCursorProps(char* szTable);
    bool RegisterCallBack();
    bool UnRegisterCallBack();

//private:
	TUVerifyCallBack cbInfo;
    CURProps tuProps;
    hDBIDb hDb;
    hTUses vhTSes;

// Nautilus
	NSRepareTable* pMaitre;
};
//---------------------------------------------------------------------------
#endif
