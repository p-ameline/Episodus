//---------------------------------------------------------------------------
#include "ns_sgbd\nsbdeuti.h"
#include "ns_sgbd\nsrepare.h"
//---------------------------------------------------------------------------

DBIResult CheckDbiError(DBIResult ErrorValue, char* FileName, unsigned long LineNo)
{
	// This function is used to check all DbiFunctions
    //
    char        dbi_status[DBIMAXMSGLEN * 5] = {'\0'}; // Error String
    DBIMSG      dbi_string = {'\0'};
    DBIErrInfo  ErrInfo;    // Contains information about the error

    if (ErrorValue != DBIERR_NONE)
    {
        // Note - make certain to call DbiGetErrorInfo() right after
        // the error as it will only give information about the most
        // recent error.
        DbiGetErrorInfo(TRUE, ErrInfo);

        // Check to see if DbiGetErrorInfo is reporting information
        //   on the same error that you have. (Done in case DbiGetErrorInfo
        //   was not called immediately after the function which failed)
        if (ErrInfo.iError == ErrorValue)
        {
            wsprintf(dbi_status, "  ERROR %i - Cat:Code = [%xh:%xh]\r\n    %s",
                   ErrorValue, ErrCat(ErrInfo.iError), ErrCode(ErrInfo.iError),
                   ErrInfo.szErrCode);

            // Need to check how much information was provided -
            //   different errors return different amounts of information.
            if (strcmp(ErrInfo.szContext[1], ""))
            {
                wsprintf(dbi_status, "%s\r\n    %s", dbi_status,
                         ErrInfo.szContext[1]);
            }
            if (strcmp(ErrInfo.szContext[2], ""))
            {
                wsprintf(dbi_status, "%s\r\n    %s", dbi_status,
                         ErrInfo.szContext[2]);
            }
            if (strcmp(ErrInfo.szContext[3], ""))
            {
                wsprintf(dbi_status, "%s\r\n    %s", dbi_status,
                         ErrInfo.szContext[3]);
            }
            if (strcmp(ErrInfo.szContext[4], ""))
            {
                wsprintf(dbi_status, "%s\r\n    %s", dbi_status,
                         ErrInfo.szContext[4]);
            }
        }
        else
        {
            DbiGetErrorString(ErrorValue, dbi_string);
            wsprintf(dbi_status, "  ERROR %i - Cat:Code = [%xh:%xh]\r\n    %s",
                     ErrorValue, ErrCat(ErrorValue), ErrCode(ErrorValue),
                     dbi_string);
        }
        // Add the file name and line number to the error string
        wsprintf(dbi_status, "%s\r\n    FileName: '%s'  Line Number: %i",
                 dbi_status, FileName, LineNo);
        // Display error in snipit and in a MessageBox
        MessageBeep(MB_ICONEXCLAMATION);
        MessageBox(GetFocus(), dbi_status, "TUtility -- BDE Error",
                   MB_OK | MB_ICONEXCLAMATION);
    }
    return ErrorValue;
}
//---------------------------------------------------------------------------
DBIResult CheckTUError(DBIResult ErrorValue, char * FileName, unsigned long LineNo)
{
	// Check all TUTility functions
    //
	CHAR     dbi_status[DBIMAXMSGLEN * 2], dbi_string[DBIMAXMSGLEN];

	if (ErrorValue != DBIERR_NONE)
	{
		TUGetErrorString(ErrorValue, dbi_string);
		wsprintf(dbi_status, "%s\r\n    FileName: '%s'  Line Number: %i",
               dbi_string, FileName, LineNo);
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox(GetFocus(), dbi_status, "TUtility -- TU Error",
                  MB_OK | MB_ICONEXCLAMATION);
	}
	return ErrorValue;
}
//---------------------------------------------------------------------------
//  Updates the progress bar with the Status of the verify or rebuild
//
//---------------------------------------------------------------------------
CBRType __stdcall GenProgressCallBack(CBType ecbType, int Data, void* pcbInfo)
{
    TUVerifyCallBack* CBInfo;

    if (ecbType == cbGENPROGRESS)
    {
    	CBInfo = (TUVerifyCallBack far*)pcbInfo;

    	switch (CBInfo->Process)
        {
        	case TUVerifyHeader:
	            pBDEUtil->pMaitre->fixePositionEntete(CBInfo->PercentDone);
	            break;

            case TUVerifyIndex:
            	pBDEUtil->pMaitre->fixePositionIndex(CBInfo->PercentDone);
            	break;

            case TUVerifyData:
            	pBDEUtil->pMaitre->fixePositionDonnees(CBInfo->PercentDone);
            	break;

            case TURebuild:
            	pBDEUtil->pMaitre->fixePositionRepare(CBInfo->PercentDone);
            	break;
        }
    }
    return cbrUSEDEF;
}
//---------------------------------------------------------------------------
// Class implementation od the TUTilBDE class
//
//---------------------------------------------------------------------------
TBDEUtil::TBDEUtil()
{
	// Default Constructor...
    //
    hDb = 0;
    TUCheck(TUInit(&vhTSes));
}
//---------------------------------------------------------------------------
TBDEUtil::~TBDEUtil()
{
	// Default destructor...
    //
    TUExit(vhTSes);
}
//---------------------------------------------------------------------------
bool TBDEUtil::GetTCursorProps(char* szTable)
{
	if (TUCheck(TUFillCURProps(vhTSes, szTable, &tuProps)) == DBIERR_NONE)
    	return true;
    else
    	return false;
}
//---------------------------------------------------------------------------
bool TBDEUtil::RegisterCallBack()
{
//	CBRType(__stdcall *CallBackfp)(CBType, long, void*);
//	CallBackfp = &GenProgressCallBack;
	char CallbackData[100 * sizeof(char)];

	DBIResult rslt = Checker(DbiRegisterCallBack(NULL, cbGENPROGRESS, (int)CallbackData,
    		sizeof(TUVerifyCallBack), &cbInfo, GenProgressCallBack));

    if (rslt == DBIERR_NONE)
    	return true;
    else
	{
        return false;
    }
}
//---------------------------------------------------------------------------
bool TBDEUtil::UnRegisterCallBack()
{
	DBIResult rslt = Checker(DbiRegisterCallBack(NULL, cbGENPROGRESS, (int)NULL,
    		sizeof(TUVerifyCallBack), &cbInfo, GenProgressCallBack));

	if (rslt == DBIERR_NONE)
    	return true;
    else
    {
		return false;
    }
}
//---------------------------------------------------------------------------
