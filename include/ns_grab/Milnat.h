/***************************************************************************/
/*

    Filename:  MILNAT.H
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the defines necessary to use the
               MIL NATIVE MODE DEVELOPER'S TOOLKIT.

    COPYRIGHT (c) 1993  Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

/***************************************************************************/
/* MIL_USE_NATIVE_FLAG */

/* activate MIL native mode (if not done) */
#ifndef M_MIL_USE_NATIVE
#define M_MIL_USE_NATIVE   1
#endif

/************************************************************************/
/* Used by MnatEnter/LeaveNativeMode()                                  */
/************************************************************************/
#define M_NAT_NULL                                    0x0L
#define M_NAT_PROC                                    0x1L
#define M_NAT_GRAPH                                   0x2L
#define M_NAT_DISP                                    0x4L
#define M_NAT_GRAB                                    0x8L
#define M_NAT_ACCESS                                  0x10L
#define M_NAT_ALL                                     0x1fL

/************************************************************************/
/* Used by MnatAccessSystemInfo                                         */
/************************************************************************/
#define M_MODULE_NAT                                  0
#define M_READ                                        1
#define M_WRITE                                       2
#define M_RDWR                                        3

/************************************************************************/
/* Used by MisNatGetLocGraph()                                          */
/************************************************************************/
#define M_ORG                                         1L
#define M_SURF                                        2L

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************/
/* TOOLKIT FOR NATIVE MODE FUNCTION CALLS                                  */
/***************************************************************************/

MFTYPE32 long   MFTYPE MnatEnterNativeMode(MIL_ID SystemId, long ControlFlag);
MFTYPE32 long   MFTYPE MnatLeaveNativeMode(MIL_ID SystemId, long ControlFlag);
MFTYPE32 long   MFTYPE MnatModified(MIL_ID BufferId);
MFTYPE32 long   MFTYPE MnatModifiedRegion(MIL_ID BufferId, long OffX, long OffY, long SizeX, long SizeY);
MFTYPE32 long   MFTYPE MnatGetMilDriverId(MIL_ID Id);
MFTYPE32 MIL_ID MFTYPE MnatGetMilId(long OwnerSystemType,long OwnerSystemId,long ObjectAttribute,long ObjectDriverId);
MFTYPE32 long   MFTYPE MnatGetNativeId(MIL_ID Id);
MFTYPE32 long   MFTYPE MnatGetLocOrg(MIL_ID BufferId);
MFTYPE32 long   MFTYPE MnatGetLocSurf(MIL_ID BufferId);
MFTYPE32 long   MFTYPE MnatGetLocPosX(MIL_ID BufferId);
MFTYPE32 long   MFTYPE MnatGetLocPosY(MIL_ID BufferId);
MFTYPE32 long   MFTYPE MnatGetLocSizeX(MIL_ID BufferId);
MFTYPE32 long   MFTYPE MnatGetLocSizeY(MIL_ID BufferId);
MFTYPE32 long   MFTYPE MnatGetLocSizeBit(MIL_ID BufferId);
MFTYPE32 void   MPTYPE * MFTYPE MnatAccessSystemInfo(MIL_ID SystemId, long Operation,
                                                     long ModuleType,
                                                     void MPTYPE * MPTYPE *InfoPtr);


/***************************************************************************/
/* IMAGE SERIES SPECIFIC FUNCTION SET                                      */
/***************************************************************************/

MFTYPE32 MIL_ID MFTYPE MisDigToBufAlloc(MIL_ID DigitizerId,
                                        MIL_ID MPTYPE *BufferIdVarPtr);
MFTYPE32 void MFTYPE MisDigToBufFree(MIL_ID BufferId);
MFTYPE32 void MFTYPE MisBufCondImageAttach(MIL_ID BufferId,
                                           MIL_ID RoiId,
                                           long OffsetX,
                                           long OffsetY,
                                           long Condition,
                                           long CondValue);
MFTYPE32 void MFTYPE MisBufMask(MIL_ID BufferId, long Mask);
MFTYPE32 short MFTYPE MisNatGetLocGraph(MIL_ID BufferId, long Parameter);
MFTYPE32 void MFTYPE MisDigIteration(MIL_ID SystemId, long NbIteration);

/***************************************************************************/
/* HOST SPECIFIC FUNCTION SET                                              */
/***************************************************************************/

MFTYPE32 MIL_ID MFTYPE MnatBufCreate2d(MIL_ID SystemId,
                                       long SizeX,
                                       long SizeY,
                                       long Type,
                                       long Attribute,
                                       void MHUGETYPE *HostArrayPtr,
                                       MIL_ID MPTYPE *BufferIdVarPtr);
MFTYPE32 MIL_ID MFTYPE MnatBufCreateColor(MIL_ID SystemId,
                                          long SizeBand,
                                          long SizeX,
                                          long SizeY,
                                          long Type,
                                          long Attribute,
                                          void MHUGETYPE * MPTYPE * ArrayOfHostBufPtr,
                                          MIL_ID MPTYPE *BufferIdVarPtr);
MFTYPE32 void MFTYPE MnatBufDestroy(MIL_ID MilBufId);

MFTYPE32 long MFTYPE MnatBufGetPitch(MIL_ID BufferId);

MFTYPE32 void MHUGETYPE * MFTYPE MnatBufGetPointer(MIL_ID BufferId);


/***************************************************************************/
/* COMET SPECIFIC FUNCTION SET                                             */
/***************************************************************************/

MFTYPE32   void MFTYPE McometSysControl(MIL_ID SystemId,
                                        long ControlType,
                                        long ControlValue);

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

