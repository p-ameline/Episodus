/***************************************************************************/
/*

    Filename:  MILPROTO.H
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the prototypes for the Matrox
               Imaging Library (MIL) C user's functions.

    COPYRIGHT (c) 1993  Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef __MILPROTO_H
#define __MILPROTO_H

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif													 


/***************************************************************************/
/* GRAPHIC MODULE:                                                         */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONTROL: */

      /* -------------------------------------------------------------- */

MFTYPE32 MIL_ID MFTYPE  MgraAlloc            (MIL_ID SystemId,
                                              MIL_ID MPTYPE *GraphContextIdVarPtr);

MFTYPE32 void MFTYPE    MgraFree             (MIL_ID GraphContextId);

MFTYPE32 void MFTYPE    MgraColor            (MIL_ID GraphContextId,
                                              MCAPICSTTYPE ForegroundColor);

MFTYPE32 void MFTYPE    MgraBackColor        (MIL_ID GraphContextId,
                                              MCAPICSTTYPE BackgroundColor);

MFTYPE32 void MFTYPE    MgraFont             (MIL_ID GraphContextId,
                                              long Font);

MFTYPE32 void MPTYPE * MFTYPE MgraFontGetLarge(void);
MFTYPE32 void MPTYPE * MFTYPE MgraFontGetSmall(void);
MFTYPE32 void MPTYPE * MFTYPE MgraFontGetMedium(void);

MFTYPE32 void MFTYPE    MgraFontScale        (MIL_ID GraphContextId,
                                              double xFontScale,
                                              double yFontScale);

MFTYPE32 void MFTYPE    MgraInquire          (MIL_ID GraphContextId,
                                              long inquireType,
                                              void MPTYPE *result_ptr);
MFTYPE32 void MFTYPE    MgraControl          (MIL_ID GraphContextId,
                                              long   ControlType,
                                              double ControlValue);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

  	   /* DRAWING : */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MgraDot              (MIL_ID GraphContextId,
                                              MIL_ID ImageId,
                                              long XPos,
                                              long YPos);

MFTYPE32 void MFTYPE    MgraLine             (MIL_ID GraphContextId,
                                              MIL_ID ImageId,
                                              long XStart,
                                              long YStart,
                                              long XEnd,
                                              long YEnd);

MFTYPE32 void MFTYPE    MgraArc              (MIL_ID GraphContextId,
                                              MIL_ID ImageId,
                                              long XCenter,
                                              long YCenter,
                                              long XRad,
                                              long YRad,
                                              double StartAngle,
                                              double EndAngle);

MFTYPE32 void MFTYPE    MgraArcFill          (MIL_ID GraphContextId,
                                              MIL_ID ImageId,
                                              long XCenter,
                                              long YCenter,
                                              long XRad,
                                              long YRad,
                                              double StartAngle,
                                              double EndAngle);

MFTYPE32 void MFTYPE    MgraRect             (MIL_ID GraphContextId,
                                              MIL_ID ImageId,
                                              long XStart,
                                              long YStart,
                                              long XEnd,
                                              long YEnd);

MFTYPE32 void MFTYPE    MgraRectFill         (MIL_ID GraphContextId,
                                              MIL_ID ImageId,
                                              long XStart,
                                              long YStart,
                                              long XEnd,
                                              long YEnd);

MFTYPE32 void MFTYPE    MgraFill             (MIL_ID GraphContextId,
                                              MIL_ID ImageId,
                                              long XStart,
                                              long YStart);

MFTYPE32 void MFTYPE    MgraClear            (MIL_ID GraphContextId,
                                              MIL_ID ImageId);

MFTYPE32 void MFTYPE    MgraText             (MIL_ID GraphContextId,
                                              MIL_ID ImageId,
                                              long XStart,
                                              long YStart,
                                              MIL_TEXT_PTR String);

      /* -------------------------------------------------------------- */

/***************************************************************************/
/* DATA GENERATION MODULE:                                                 */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* DATA BUFFERS: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MgenLutRamp          (MIL_ID LutBufId,
                                              long StartPoint,
                                              MCAPICSTTYPE StartValue,
         											    long EndPoint,
                                              MCAPICSTTYPE EndValue);

MFTYPE32 void MFTYPE    MgenLutFunction      (MIL_ID lut_id,
                                              long func,
                                              double a,
                                              double b,
                                              double c,
                                              long start_index,
                                              MCAPICSTTYPE StartXValue,
                                              long end_index);

MFTYPE32 void MFTYPE    MgenWarpParameter    (MIL_ID  InWarpParameter,
                                              MIL_ID  OutXLutorCoef,
                                              MIL_ID  OutYLut,
                                              long    OperationMode,
                                              long    Transform,
                                              double  Val1,
                                              double  Val2);

      /* -------------------------------------------------------------- */

/***************************************************************************/
/* DATA BUFFERS MODULE:                                                    */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CREATION: */

      /* -------------------------------------------------------------- */

MFTYPE32 MIL_ID MFTYPE  MbufAlloc1d          (MIL_ID SystemId,
                                              long SizeX,
                                              long Type,
                                              long Attribute,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 MIL_ID MFTYPE  MbufAlloc2d          (MIL_ID SystemId,
                                              long SizeX,
                                              long SizeY,
                                              long Type,
                                              long Attribute,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 MIL_ID MFTYPE  MbufAllocColor       (MIL_ID SystemId,
                                              long SizeBand,
                                              long SizeX,
                                              long SizeY,
                                              long Type,
                                              long Attribute,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 MIL_ID MFTYPE  MbufChild1d          (MIL_ID ParentImageId,
                                              long OffX,
                                              long SizeX,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 MIL_ID MFTYPE  MbufChild2d          (MIL_ID ParentMilBufId,
                                              long OffX,
                                              long OffY,
                                              long SizeX,
                                              long SizeY,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 MIL_ID MFTYPE  MbufChildColor       (MIL_ID ParentMilBufId,
                                              long Band,
                                              MIL_ID MPTYPE *IdVarPtr);
                                              
MFTYPE32 MIL_ID MFTYPE  MbufChildColor2d     (MIL_ID ParentMilBufId,
                                              long Band,
                                              long OffX,
                                              long OffY,
                                              long SizeX,
                                              long SizeY,
                                              MIL_ID MPTYPE *IdVarPtr);
                               
MFTYPE32 MIL_ID MFTYPE  MbufCreateColor       (MIL_ID SystemId,
                                              long SizeBand,
                                              long SizeX,
                                              long SizeY,
                                              long Type,
                                              long Attribute,
                                              long ControlFlag,
                                              long Pitch,
                                              void MPTYPE **ArrayOfDataPtr,
                                              MIL_ID MPTYPE *IdVarPtr);
#if !M_MIL_USE_PPC
#ifdef M_USE_MBUFBLIT
MFTYPE32 void MFTYPE  MbufBlit                (MIL_ID  SrcBufId, 
                                              MIL_ID  DstBufId,
                                              LPMBLTDESC pBltDesc);
#endif
#endif

MFTYPE32 MIL_ID MFTYPE  MbufCreate2d          (MIL_ID SystemId,
                                              long SizeX,
                                              long SizeY,
                                              long Type,
                                              long Attribute,
                                              long ControlFlag,
                                              long Pitch,
                                              void MPTYPE *DataPtr,
                                              MIL_ID MPTYPE *IdVarPtr);                                              

MFTYPE32 void MFTYPE    MbufFree             (MIL_ID BufId);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* ACCESS: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MbufClear            (MIL_ID BufId,
                                              MCAPICSTTYPE Value);

MFTYPE32 void MFTYPE    MbufCopy             (MIL_ID SrcBufId,
                                              MIL_ID DestBufId);

MFTYPE32 void MFTYPE    MbufCopyColor        (MIL_ID SrcBufId,
                                              MIL_ID DestBufId,
                                              long Band);

MFTYPE32 void MFTYPE    MbufCopyColor2d      (MIL_ID SrcBufId,
                                              MIL_ID DestBufId,
                                              long SrcBand,
                                              long SrcOffX,
                                              long SrcOffY,
                                              long DstBand,
                                              long DstOffX,
                                              long DstOffY,
                                              long SizeX,
                                              long SizeY);

MFTYPE32 void MFTYPE    MbufCopyClip         (MIL_ID SrcBufId,
                                              MIL_ID DestBufId,
                                              long DestOffsetX,
                                              long DestOffsetY);

MFTYPE32 void MFTYPE    MbufCopyMask         (MIL_ID SrcBufId,
                                              MIL_ID DestBufId,
                                              long MaskValue);

MFTYPE32 void MFTYPE    MbufCopyCond         (MIL_ID SrcBufId,
                                              MIL_ID DestBufId,
                                              MIL_ID CondBufId,
                                              long Cond,
                                              MCAPICSTTYPE CondVal);

MFTYPE32 void MFTYPE    MbufGetLine          (MIL_ID SrcImageId,
                                              long   XStart,
                                              long   YStart,
                                              long   XEnd,
                                              long   YEnd,
                                              long   Mode,
                                              long   MPTYPE *NbPixelsValPtr,
                                              void   MPTYPE *BufferType);

MFTYPE32 void MFTYPE    MbufPut1d            (MIL_ID DestBufId,
                                              long Offx,
                                              long Sizex,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufPut2d            (MIL_ID DestBufId,
                                              long Offx,
                                              long Offy,
                                              long Sizex,
                                              long Sizey,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufPutColor         (MIL_ID DestBufId,
                                              long Format,
                                              long Band,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufPutColor2d       (MIL_ID DestBufId,
                                              long Format,
                                              long Band,
                                              long Offx,
                                              long Offy,
                                              long Sizex,
                                              long Sizey,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufPutLine          (MIL_ID SrcImageId,
                                              long   XStart,
                                              long   YStart,
                                              long   XEnd,
                                              long   YEnd,
                                              long   Mode,
                                              long   MPTYPE *NbPixelsValPtr,
                                              void   MPTYPE *BufferType);
                                              
MFTYPE32 void MFTYPE    MbufPut              (MIL_ID DestBufId,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufGet1d            (MIL_ID SourceBufId,
                                              long Offx,
                                              long Sizex,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufGet2d            (MIL_ID SourceBufId,
                                              long Offx,
                                              long Offy,
                                              long Sizex,
                                              long Sizey,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufGetColor         (MIL_ID SourceBufId,
                                              long Format,
                                              long Band,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufGet              (MIL_ID SourceBufId,
                                              void MPTYPE *BufferPtr);

MFTYPE32 void MFTYPE    MbufGetColor2d       (MIL_ID SourceBufId,
                                              long Format,
                                              long Band,
                                              long Offx,
                                              long Offy,
                                              long Sizex,
                                              long Sizey,
                                              void MPTYPE *BufferPtr);

MFTYPE32 long MFTYPE    MbufInquire          (MIL_ID BufId,
                                              long InquireType,
                                              void MPTYPE *ResultPtr);

MFTYPE32 long MFTYPE    MbufDiskInquire      (MIL_TEXT_PTR FileName,
                                              long InquireType,
                                              void MPTYPE *ResultPtr);

MFTYPE32 void MFTYPE    MbufSave             (MIL_TEXT_PTR FileName,
                                              MIL_ID BufId);

MFTYPE32 MIL_ID MFTYPE  MbufRestore          (MIL_TEXT_PTR FileName,
                                              MIL_ID SystemId,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 void MFTYPE    MbufLoad             (MIL_TEXT_PTR FileName,
                                              MIL_ID BufId);

MFTYPE32 MIL_ID MFTYPE  MbufImport           (MIL_TEXT_PTR FileName,
                                              long FileFormat,
                                              long Operation,
                                              MIL_ID SystemId,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 void MFTYPE    MbufExport           (MIL_TEXT_PTR FileName,
                                              long FileFormat,
                                              MIL_ID srcBufId);

MFTYPE32 void MFTYPE    MbufModified2d       (MIL_ID BufferId, 
                                              long OffsetX, 
                                              long OffsetY, 
                                              long SizeX, 
                                              long SizeY);
                                              
MFTYPE32 void MFTYPE    MbufCompression      (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long ControlValue,
                                              long OperationFlag);
MFTYPE32 void MFTYPE    MbufExportSequence   (MIL_TEXT_PTR FileName,
                                              long FileFormat,  
                                              MIL_ID *BufArrayPtr,
                                              long NumberOfFrames,
                                              double FrameRate,
                                              long ControlFlag);
MFTYPE32 void MFTYPE    MbufImportSequence   (MIL_TEXT_PTR FileName,
                                              long FileFormat,
                                              long Operation,
                                              MIL_ID SystemId,  
                                              MIL_ID *BufArrayPtr,
                                              long StartImage,
                                              long NumberOfFrames,
                                              long ControlFlag);

MFTYPE32 void MFTYPE    MbufBayer            (MIL_ID SrcImageBufId, 
                                              MIL_ID DestImageBufId,
                                              MIL_ID WhiteBalanceCoefficientsId,
                                              long ControlFlag);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONTROL: */

      /* -------------------------------------------------------------- */

typedef MFTYPE32 long (MFTYPE MPTYPE *MBUFHOOKFCTPTR)(long FHookType,
                                                      MIL_ID Id,
                                                      void MPTYPE *FExpansionFlagPtr);

MFTYPE32 void MFTYPE    MbufHookFunction        (MIL_ID BufferId,
                                                 long HookType,
                                                 MBUFHOOKFCTPTR HookHandlerPtr,
                                                 void MPTYPE * UserDataPtr);

MFTYPE32 long MFTYPE    MbufGetHookInfo         (MIL_ID Id,
                                                 long InfoType,
                                                 void MPTYPE *UserPtr);

MFTYPE32 void MFTYPE    MbufControlNeighborhood (MIL_ID BufId,
                                                 long OperationFlags,
                                                 long OperationValue);

MFTYPE32 void MFTYPE    MbufControl             (MIL_ID BufId,
                                                 long OperationFlags,
                                                 double OperationValue);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

/***************************************************************************/
/* I/O DEVICES:                                                            */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CREATION: */

      /* -------------------------------------------------------------- */

typedef MFTYPE32 long (MFTYPE MPTYPE *MDIGHOOKFCTPTR)(long HookType,
                                                      MIL_ID EventId,
                                                      void MPTYPE *UserPtr);

MFTYPE32 MDIGHOOKFCTPTR MFTYPE MdigHookFunction(MIL_ID DigitizerId,
                                                long HookType,
                                                MDIGHOOKFCTPTR HandlerPtr,
                                                void MPTYPE *UserPtr);
                                                
MFTYPE32 MIL_ID MFTYPE  MdigAlloc            (MIL_ID SystemId,
                                              long DeviceNum,
                                              MIL_TEXT_PTR DataFormat,
                                              long InitFlag,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 void MFTYPE    MdigFree             (MIL_ID DevId);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONTROL: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MdigChannel          (MIL_ID DevId,
                                              long Channel);

MFTYPE32 void MFTYPE    MdigReference        (MIL_ID DevId,
                                              long ReferenceType,
                                              long ReferenceLevel);

MFTYPE32 void MFTYPE    MdigLut              (MIL_ID DevId,
                                              MIL_ID LutBufId);

MFTYPE32 void MFTYPE    MdigHalt             (MIL_ID DevId);

MFTYPE32 long MFTYPE    MdigInquire          (MIL_ID DevId,
                                              long InquireType,
                                              void MPTYPE *ResultPtr);

MFTYPE32 void MFTYPE    MdigControl          (MIL_ID DigitizerId,
                                              long ControlType,
                                              double Value);

MFTYPE32 void MFTYPE    MdigGrabWait         (MIL_ID DevId,
                                              long Flag);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

	   /* ACCESS: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MdigGrab             (MIL_ID SrcDevId,
                                              MIL_ID DestImageId);

MFTYPE32 void MFTYPE    MdigGrabContinuous   (MIL_ID SrcDevId,
                                              MIL_ID DestImageId);

MFTYPE32 void MFTYPE    MdigAverage          (MIL_ID Digitizer,
                                              MIL_ID DestImageId,
                                              long WeightFactor,
                                              long AverageType,
                                              long NbIteration);

typedef MFTYPE32 long (MFTYPE MPTYPE *MFOCUSHOOKFCTPTR)(long HookType, 
									                            long position,
										                         void MPTYPE *UserDataPtr);

MFTYPE32 void MFTYPE    MdigFocus           (MIL_ID DigId,
                                             MIL_ID DestImageId,
                                             MIL_ID FocusImageRegionId,
                                             MFOCUSHOOKFCTPTR FocusHookPtr,
                                             void MPTYPE *UserDataPtr,
                                             long MinPosition, 
                                             long StartPosition, 
                                             long MaxPosition,
                                             long MaxPositionVariation,
                                             long ProcMode,
                                             long *ResultPtr); 

      /* -------------------------------------------------------------- */

/***************************************************************************/
/* DISPLAY MODULE:                                                         */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONTROL: */

      /* -------------------------------------------------------------- */

typedef MFTYPE32 long (MFTYPE MPTYPE *MDISPHOOKFCTPTR)(long HookType,
                                                       MIL_ID EventId,
                                                       void MPTYPE *UserPtr);

MFTYPE32 MDISPHOOKFCTPTR MFTYPE MdispHookFunction(MIL_ID DisplayId,
                                                  long HookType,
                                                  MDISPHOOKFCTPTR HandlerPtr,
                                                  void MPTYPE *UserPtr);
                                                
MFTYPE32 MIL_ID MFTYPE  MdispAlloc           (MIL_ID SystemId,
                                              long DispNum,
                                              MIL_TEXT_PTR DispFormat,
                                              long InitFlag,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 void MFTYPE    MdispFree            (MIL_ID DisplayId);

MFTYPE32 void MFTYPE    MdispSelect          (MIL_ID DisplayId,
                                              MIL_ID ImageId);

MFTYPE32 void MFTYPE    MdispDeselect        (MIL_ID DisplayId,
                                              MIL_ID ImageId);

MFTYPE32 long MFTYPE    MdispInquire         (MIL_ID DisplayId,
                                              long inquire_type,
                                              void MPTYPE *result_ptr);

MFTYPE32 void MFTYPE    MdispPan             (MIL_ID DisplayId,
                                              long XOffset,
                                              long YOffset);

MFTYPE32 void MFTYPE    MdispZoom            (MIL_ID DisplayId,
                                              long XFactor,
                                              long YFactor);

MFTYPE32 void MFTYPE    MdispLut             (MIL_ID DisplayId,
                                              MIL_ID LutBufId);

MFTYPE32 void MFTYPE    MdispOverlayKey      (MIL_ID DisplayId,
                                              long Mode,
                                              long Cond,
                                              long Mask,
                                              long Color);

MFTYPE32 void MFTYPE    MdispControl         (MIL_ID DisplayId,
                                              long ControlType,
                                              long Value);
MFTYPE32 void MFTYPE    MdispControlEx       (MIL_ID  DisplayId,
                                              long    ControlType,
                                              double  ControlValue);


      /* -------------------------------------------------------------- */

/***************************************************************************/
/* SYSTEM MODULE:                                                          */
/***************************************************************************/

	  /* -------------------------------------------------------------- */
     /* -------------------------------------------------------------- */

     /* CONTROL: */

     /* -------------------------------------------------------------- */

typedef MFTYPE32 long (MFTYPE MPTYPE *MSYSHOOKFCTPTR)(long HookType,
                                                      MIL_ID EventId,
                                                      void MPTYPE *UserPtr);

MFTYPE32 MSYSHOOKFCTPTR MFTYPE MsysHookFunction(MIL_ID SystemId,
                                                long HookType,
                                                MSYSHOOKFCTPTR HandlerPtr,
                                                void MPTYPE *UserPtr);
                                                
MFTYPE32 long MFTYPE  MsysGetHookInfo        (MIL_ID SystemId,
                                              MIL_ID Id,
                                              long InfoType,
                                              void MPTYPE *UserPtr);

MFTYPE32 MIL_ID MFTYPE MsysAlloc             (void MPTYPE * SystemTypePtr,
                                              long SystemNum,
                                              long InitFlag,
                                              MIL_ID MPTYPE *SystemIdVarPtr);

MFTYPE32 void MFTYPE  MsysFree               (MIL_ID SystemId);

MFTYPE32 long MFTYPE  MsysInquire            (MIL_ID SystemId,
                                              long   InquireType,
                                              void MPTYPE *ResultPtr);

MFTYPE32 void MFTYPE  MsysControl            (MIL_ID SystemId,
                                              long ControlType,
                                              MIL_ID TargetSysId);

MFTYPE32 void MFTYPE MsysConfigAccess(        MIL_ID       SystemId,      
                                              long         VendorId,      
                                              long         DeviceId,      
                                              long         DeviceNum,     
                                              long         OperationFlag, 
                                              long         OperationType, 
                                              long         Offset,        
                                              long         Size,
                                              void MPTYPE *UserArrayPtr);

      /* -------------------------------------------------------------- */

/***************************************************************************/
/* APPLICATION MODULE:                                                     */
/***************************************************************************/

	  /* -------------------------------------------------------------- */
     /* -------------------------------------------------------------- */

     /* CONTROL: */

     /* -------------------------------------------------------------- */


typedef MFTYPE32 long (MFTYPE MPTYPE *MAPPHOOKFCTPTR)(long FHookType,
                                                      MIL_ID Id,
                                                      void MPTYPE *FExpansionFlagPtr);

MFTYPE32 void   MFTYPE MappHookFunction(long HookType,
                                        MAPPHOOKFCTPTR HookErrorCurrentPtr,
                                        void MPTYPE *ExpansionFlagPtr);

MFTYPE32 MIL_ID MFTYPE  MappAlloc         (long InitFlag,
                                           MIL_ID MPTYPE *IdVarPtr);
MFTYPE32 MIL_ID MFTYPE  MappChild         (MIL_ID ParentId,
                                           long InitFlag,
                                           MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 void MFTYPE    MappFree          (MIL_ID ApplicationId);

MFTYPE32 void MFTYPE    MappControl       (long ControlType,
                                           long ControlFlag);

MFTYPE32 long MFTYPE    MappControlThread (MIL_ID ThreadOrEventId,
                                           long Operation,
                                           long OperationValue,
                                           long *IdVarPtr);

MFTYPE32 long MFTYPE    MappGetError      (long ErrorType,
                                           void MPTYPE *ErrorVarPtr);

MFTYPE32 long MFTYPE    MappGetHookInfo   (MIL_ID Id,
                                           long InfoType,
                                           void MPTYPE *UserPtr);

MFTYPE32 long MFTYPE    MappInquire       (long InquireType,
                                           void MPTYPE *UserVarPtr);

MFTYPE32 void MFTYPE    MappModify        (MIL_ID FirstId,
                                           MIL_ID SecondId,
                                           long ModificationType,
                                           long ModificationFlag);
                                           
MFTYPE32 void MFTYPE    MappTimer         (long Mode,
                                           double *Time);
                                           
/* -------------------------------------------------------------- */

/***************************************************************************/
/* MISC FUNCTIONS:                                                         */
/***************************************************************************/
MFTYPE32 int MFTYPE  MPtrToInt           (void *pvPtr);

/* -------------------------------------------------------------- */

/***************************************************************************/
/* TOOLKIT FOR PSEUDO MIL FUNCTIONS DEVELOPMENT                            */
/***************************************************************************/
typedef MFTYPE32 void (MFTYPE MPTYPE *MFUNCFCTPTR)(MIL_ID FunctionId);
MFTYPE32 MIL_ID MFTYPE MfuncAlloc(MIL_TEXT_PTR FunctionName,
                                  long ParameterNumber,
                                  MFUNCFCTPTR WorkFunctionPtr,
                                  MIL_TEXT_PTR WorkDllName,
                                  MIL_TEXT_PTR WorkFunctionName,
                                  long WorkFunctionOpcode,
                                  long InitFlag,
                                  MIL_ID *FuncIdVarPtr);
MFTYPE32 void MFTYPE MfuncParamId(MIL_ID FuncId,long ParamIndex,MIL_ID ParamVal,
                                  long ParamIs, long ParamHasAttr);
MFTYPE32 void MFTYPE MfuncParamLong(MIL_ID FuncId,long ParamIndex,long ParamVal);
MFTYPE32 void MFTYPE MfuncParamDouble(MIL_ID FuncId,long ParamIndex,double ParamVal);
MFTYPE32 void MFTYPE MfuncParamPointer(MIL_ID FuncId, long ParamIndex, void * ParamVal, long SizeOfData, long ParamAttribute);
MFTYPE32 void MFTYPE MfuncParamString(MIL_ID FuncId,long ParamIndex,void MPTYPE * ParamVal, long SizeOfData, long ParamAttribute);
MFTYPE32 long MFTYPE MfuncStart(MIL_ID FuncId);
MFTYPE32 long MFTYPE MfuncErrorReport(MIL_ID FuncId,
                                      long ErrorCode,
                                      MIL_TEXT_PTR ErrorString,
                                      MIL_TEXT_PTR ErrorSubString1,
                                      MIL_TEXT_PTR ErrorSubString2,
                                      MIL_TEXT_PTR ErrorSubString3);
MFTYPE32 long MFTYPE MfuncGetError(MIL_ID FunctId,long ErrorType,void MPTYPE *ErrorVarPtr);
MFTYPE32 void MFTYPE MfuncFreeAndEnd(MIL_ID FuncId);
MFTYPE32 long MFTYPE MfuncModified(MIL_ID ImageId);
MFTYPE32 long MFTYPE MfuncParamRegister(MIL_ID FuncId);
MFTYPE32 long MFTYPE MfuncParamCheck(MIL_ID FuncId);
MFTYPE32 MIL_ID MFTYPE MfuncAllocId(MIL_ID FunctionId, long ObjectType, void *UserPtr);
MFTYPE32 void MFTYPE MfuncFreeId(MIL_ID FunctionId, MIL_ID ObjectId);
MFTYPE32 void MFTYPE MfuncIdSetObjectType(MIL_ID FunctionId, MIL_ID ObjectId, long ObjectType);
MFTYPE32 long MFTYPE MfuncIdGetObjectType(MIL_ID FunctionId, MIL_ID ObjectId);
MFTYPE32 void MFTYPE MfuncIdSetUserPtr(MIL_ID FunctionId, MIL_ID ObjectId, void *UserPtr);
MFTYPE32 void* MFTYPE MfuncIdGetUserPtr(MIL_ID FunctionId, MIL_ID ObjectId);
MFTYPE32 long MFTYPE MfuncPrintMessage(long Mode, MIL_TEXT_PTR Str1,
                                        MIL_TEXT_PTR Str2 ,
                                        MIL_TEXT_PTR Str3 ,
                                        MIL_TEXT_PTR Str4 ,
                                        MIL_TEXT_PTR Str5);
MFTYPE32 long MFTYPE MfuncExit();
MFTYPE32 void MHUGETYPE * MFTYPE MfuncAllocMemory(long NbElements, long Size);
MFTYPE32 void             MFTYPE MfuncFreeMemory(void MHUGETYPE * MemoryP);
MFTYPE32 void MFTYPE MfuncFree(MIL_ID FuncId);
MFTYPE32 long MFTYPE MfuncCall(MIL_ID FuncId);
MFTYPE32 void MFTYPE MfuncParamValue(MIL_ID FuncId, long ParamIndex, void *ParamValuePtr);


/* -------------------------------------------------------------- */


/**************************************************************************/
/* VGA WINDOWS SPECIFIC FUNCTION SET                                              */
/**************************************************************************/

#if (!defined(__MWINEXT__))

   #define __MWINEXT__

   #if (defined(_INC_WINDOWS) || defined(_WINDOWS_) || defined(__WINDOWS_H) || defined(__WINDOWS__))

    MFTYPE32 void MFTYPE    MdispSelectWindow    (MIL_ID DisplayId,
                                                  MIL_ID ImageId,
                                                  HWND ClientWindowHandle);
                                                  
    /* Structure for hook to modification to bitmap */
    typedef struct
    {
    LPBITMAPINFO DIBHeaderPtr;          /* pointer to the DIB header          */
    UINT         DIBColorUse;           /* color usage of DIB header          */
    LPSTR        DIBDataPtr;            /* pointer to the DIB data            */
    LPLOGPALETTE DIBPalettePtr;         /* pointer to the DIB palette         */
    HPALETTE     DIBPaletteHdl;         /* handle to the DIB palette          */
    HBITMAP      DIBBitmapHdl;          /* handle to DIB bitmap               */
    LPBITMAPINFO DIBDisplayHeaderPtr;   /* pointer to the display DIB header  */
    UINT         DIBDisplayColorUse;    /* color usage of display DIB header  */
    LPSTR        DIBDisplayDataPtr;     /* pointer to the display DIB data    */
    LPLOGPALETTE DIBDisplayPalettePtr;  /* pointer to the display DIB palette */
    HPALETTE     DIBDisplayPaletteHdl;  /* handle to the display DIB palette  */
    HBITMAP      DIBDisplayBitmapHdl;   /* handle to the display DIB bitmap   */
    MIL_ID       MilVgaBufferId;        /* MIL ID of the MIL VGA buffer       */
    MIL_ID       MilVgaDisplayId;       /* MIL ID of the MIL VGA display      */
    long         VgaDriverBufferId;     /* Reserved                           */
    long         VgaDriverDisplayId;    /* Reserved                           */
    long         VgaDriverSystemId;     /* Reserved                           */
    long         ModificationHookType;  /* Type of hook on modification       */
    long         ModificationType;      /* Type of modification to buffer     */
    long         ModificationOffsetX;   /* Offset x of modification in buffer */
    long         ModificationOffsetY;   /* Offset y of modification in buffer */
    long         ModificationSizeX;     /* Size x of modification in buffer   */
    long         ModificationSizeY;     /* Size y of modification in buffer   */
    long         ModificationZoomX;     /* Zoom x of modification in buffer   */
    long         ModificationZoomY;     /* Zoom y of modification in buffer   */
    long         ModificationPanX;      /* Pan x of modification in buffer    */
    long         ModificationPanY;      /* Pan y of modification in buffer    */
    LPVOID       ExpansionPtr;          /* Reserved                           */
    } M_DIB_INFO;

    /* Structure for hook to modification to window */
    typedef struct
    {
    HWND         WindowHandle;          /* Window hdl associated to display   */
    LPBITMAPINFO DIBHeaderPtr;          /* pointer to the DIB header          */
    UINT         DIBColorUse;           /* color usage of DIB header          */
    LPSTR        DIBDataPtr;            /* pointer to the DIB data            */
    LPLOGPALETTE DIBPalettePtr;         /* pointer to the DIB palette         */
    HPALETTE     DIBPaletteHdl;         /* handle to the DIB palette          */
    HBITMAP      DIBBitmapHdl;          /* handle to DIB bitmap               */
    LPBITMAPINFO DIBDisplayHeaderPtr;   /* pointer to the display DIB header  */
    UINT         DIBDisplayColorUse;    /* color usage of display DIB header  */
    LPSTR        DIBDisplayDataPtr;     /* pointer to the display DIB data    */
    LPLOGPALETTE DIBDisplayPalettePtr;  /* pointer to the display DIB palette */
    HPALETTE     DIBDisplayPaletteHdl;  /* handle to the display DIB palette  */
    HBITMAP      DIBDisplayBitmapHdl;   /* handle to the display DIB bitmap   */
    MIL_ID       MilVgaBufferId;        /* MIL ID of the MIL VGA buffer       */
    MIL_ID       MilVgaDisplayId;       /* MIL ID of the MIL VGA display      */
    long         VgaDriverBufferId;     /* Reserved                           */
    long         VgaDriverDisplayId;    /* Reserved                           */
    long         VgaDriverSystemId;     /* Reserved                           */
    long         ModificationHookType;  /* Type of hook on modification       */
    long         ModificationType;      /* Type of modification to buffer     */
    long         ModificationOffsetX;   /* Offset x of modified window        */
    long         ModificationOffsetY;   /* Offset y of modified window        */
    long         ModificationSizeX;     /* Size x of modified window          */
    long         ModificationSizeY;     /* Size y of modified window          */
    long         ModificationZoomX;     /* Zoom x of modified window          */
    long         ModificationZoomY;     /* Zoom y of modified window          */
    long         ModificationPanX;      /* Pan x of modified window           */
    long         ModificationPanY;      /* Pan x of modified window           */
    long         ModificationMenu;      /* Menu that is used by user          */
    long         ModificationValue;     /* Optional value for modification    */
    long         VisibleOffsetX;        /* Offset x of visible rect. of wnd   */
    long         VisibleOffsetY;        /* Offset y of visible rect. of wnd   */
    long         VisibleSizeX;          /* Size x of visible rect. of wnd     */
    long         VisibleSizeY;          /* Size y of visible rect. of wnd     */
    LPVOID       ExpansionPtr;          /* Reserved                           */
    long         ExpansionFlag;         /* Reserved                           */
    long         ReservedValue1;        /* Reserved                           */
    long         ReservedValue2;        /* Reserved                           */
    long         ReservedValue3;        /* Reserved                           */
    long         ReservedValue4;        /* Reserved                           */
    } M_WINDOW_INFO;
    
    /* Structure for hook to modification messages */
    typedef struct
    {
    HWND         WindowHandle;          /* Window hdl associated to display   */
    UINT         message;               /* Window message                     */
    WPARAM       wParam;                /* Window message wParam              */
    LPARAM       lParam;                /* Window message lParam              */
    MIL_ID       MilVgaBufferId;        /* MIL ID of the MIL VGA buffer       */
    MIL_ID       MilVgaDisplayId;       /* MIL ID of the MIL VGA display      */
    long         VgaDriverBufferId;     /* Reserved                           */
    long         VgaDriverDisplayId;    /* Reserved                           */
    long         VgaDriverSystemId;     /* Reserved                           */
    long         ModificationHookType;  /* Type of hook on modification       */
    } M_WINDOW_MSG_INFO;

    typedef struct
    {
        long Style;
        long SizeX;
        long SizeY;
        HWND ParentWnd;
    } M_CREATE_CAPTURE_WINDOW;

    typedef MFTYPE32 long (MFTYPE MPTYPE *MVGADIBHOOKFCTPTR)
                                 (M_DIB_INFO MPTYPE *ModifiedVgaBufferInfoPtr,
                                 void MPTYPE *ExtraUserInfoPtr);

    typedef MFTYPE32 long (MFTYPE MPTYPE *MVGAWINHOOKFCTPTR)
                                 (M_WINDOW_INFO MPTYPE *ModifiedVgaWindowInfoPtr,
                                 void MPTYPE *ExtraUserInfoPtr);

    typedef MFTYPE32 long (MFTYPE MPTYPE *MVGAWINMSGHOOKFCTPTR)
                                 (M_WINDOW_MSG_INFO MPTYPE *ModifiedVgaWindowMsgInfoPtr,
                                 void MPTYPE *ExtraUserInfoPtr);

    typedef void MPTYPE *MVGAHOOKFCTPTR;

    /* To get a DIB info structure to display a VGA windows buffer */
    MFTYPE32 M_DIB_INFO MPTYPE * MFTYPE MvgaDispAllocDIBInfo(
                                    MIL_ID MilVgaDisplayId,
                                    MIL_ID MilVgaBufferId);


    /* To free a DIB info structure from a display a VGA windows buffer */
    MFTYPE32 void MFTYPE MvgaDispFreeDIBInfo(
                                    MIL_ID MilVgaDisplayId,
                                    MIL_ID MilVgaBufferId);


    /* To hook a user function that will handle display refresh */
    MFTYPE32 MVGAHOOKFCTPTR MFTYPE MvgaHookModifiedDIB(
                                    MIL_ID MilVgaDisplayId,
                                    MVGAHOOKFCTPTR FctPtr,
                                    void MPTYPE *ExtraUserInfoPtr);

    /* To hook a user function for modification of MIL VGA object */
    MFTYPE32 MVGAHOOKFCTPTR MFTYPE MvgaHookModified(
                                    MIL_ID MilVgaDisplayId,
                                    long HookTypeFlag,
                                    MVGAHOOKFCTPTR FctPtr,
                                    void MPTYPE *ExtraUserInfoPtr);

    /* To display a buffer in a display window creates by the user */
    MFTYPE32 void MFTYPE MvgaDispSelectClientArea(
                                    MIL_ID MilVgaDisplayId,
                                    MIL_ID MilVgaBufferId,
                                    HWND ClientWindowHandle);


    /* To undisplay a buffer from a display window created by user */
    MFTYPE32 void MFTYPE MvgaDispDeselectClientArea(
                                    MIL_ID MilVgaDisplayId,
                                    MIL_ID MilVgaBufferId,
                                    HWND ClientWindowHandle);

    MFTYPE32 void MFTYPE MvgaDispControl(
                                    MIL_ID MilVgaDisplayId,
                                    long ControlType,
                                    long ControlState);

    MFTYPE32 long MFTYPE MvgaDispInquire(
                                    MIL_ID MilVgaDisplayId,
                                    long InquireType,
                                    long MPTYPE *TargetVarPtr);

    MFTYPE32 void MFTYPE MvgaDispCapture(
                                    MIL_ID MilVgaDisplayId,
                                    long OffsetX,
                                    long OffsetY,
                                    long SizeX,
                                    long SizeY,
                                    long DestOffsetX,
                                    long DestOffsetY);

    MFTYPE32 void MFTYPE MvgaDispProtectArea(
                                    MIL_ID MilVgaDisplayId,
                                    long OffsetX,
                                    long OffsetY,
                                    long SizeX,
                                    long SizeY);

    MFTYPE32 void MFTYPE MvgaDispSetTitleName(
                                    MIL_ID MilVgaDisplayId,
                                    char *TitleName);

   #endif
#endif

typedef struct _MILBUFFERINFO { 
    long     StructSize;      // Size of the structure, in bytes. This member must be initialized before the structure is used. 
    long     SizeX;
    long     SizeY;
    long     SizeBand;
    long     SizeBit;
    long     Type;
    long     Attribute;
    long     Format;
    long     OffsetX;
    long     OffsetY;
    long     OffsetBand;
    long     AncestorOffsetX;
    long     AncestorOffsetY;
    long     AncestorOffsetBand;
    long     AncestorOffsetBit;
    long     Pitch;
    long     PitchByte;
    void    *HostPtr;
    void    *BandHostPtr[3];
    void    *PhysPtr;
    MIL_ID   Id;           
    MIL_ID   ParentId;         
    MIL_ID   AncestorId;           
} MILBUFFERINFO,* LPMILBUFFERINFO; 


#if (!M_MIL_LITE)
/***************************************************************************/
/* BASIC IMAGE PROCESSING MODULE:                                          */
/***************************************************************************/

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* POINT TO POINT : */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MimArith             (MCAPICSTTYPE Src1ImageIdOrConstant,
                                              MCAPICSTTYPE Src2ImageIdOrConstant,
                                              MIL_ID DestImageId,
                                              long Operation);
                                              
MFTYPE32 void MFTYPE    MimArithMultiple     (MCAPICSTTYPE Src1ImageIdOrConstant,
                                              MCAPICSTTYPE Src2ImageIdOrConstant,
                                              MCAPICSTTYPE Src3ImageIdOrConstant,
                                              MCAPICSTTYPE Src4ImageIdOrConstant,
                                              MCAPICSTTYPE Src5ImageIdOrConstant,
                                              MIL_ID DestImageId,
                                              long Operation,
                                              long OperationFlag);                                              

MFTYPE32 void MFTYPE    MimLutMap            (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              MIL_ID LutId);

MFTYPE32 void MFTYPE    MimShift             (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long NbBitsToShift);

MFTYPE32 long MFTYPE    MimBinarize          (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long Condition,
                                              MCAPICSTTYPE CondLow,
                                              MCAPICSTTYPE CondHigh);

MFTYPE32 void MFTYPE    MimClip              (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long Condition,
                                              MCAPICSTTYPE CondLow,
                                              MCAPICSTTYPE CondHigh,
                                              MCAPICSTTYPE WriteLow,
                                              MCAPICSTTYPE WriteHigh);

                                              

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* NEIGHBOURHOOD : */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MimConvolve          (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              MIL_ID KernelId);

MFTYPE32 void MFTYPE    MimRank              (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              MIL_ID KernelId,
                                              long Rank,
                                              long Mode);

MFTYPE32 void MFTYPE    MimEdgeDetect        (MIL_ID SrcImageId,
                                              MIL_ID DestIntensityImageId,
                                              MIL_ID DestAngleImageId,
                                              MIL_ID KernelId,
                                              long ControlFlag,
                                              long ThresholdVal);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* MORPHOLOGICAL: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MimLabel             (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long Mode);

MFTYPE32 void MFTYPE    MimConnectMap        (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              MIL_ID LutBufId);

MFTYPE32 void MFTYPE    MimDilate            (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long NbIteration,
                                              long Mode);

MFTYPE32 void MFTYPE    MimErode             (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long NbIteration,
                                              long Mode);

MFTYPE32 void MFTYPE    MimClose             (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long NbIteration,
                                              long Mode);

MFTYPE32 void MFTYPE    MimOpen              (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long NbIteration,
                                              long Mode);

MFTYPE32 void MFTYPE    MimMorphic           (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              MIL_ID StructElementId,
                                              long Operation,
                                              long NbIteration,
                                              long Mode);

MFTYPE32 void MFTYPE    MimThin              (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long NbIteration,
                                              long Mode);

MFTYPE32 void MFTYPE    MimThick             (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long NbIteration,
                                              long Mode);

MFTYPE32 void MFTYPE    MimDistance          (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long DistanceTranform);


MFTYPE32 void MFTYPE    MimWatershed         (MIL_ID SrcImageId,
                                              MIL_ID MarkerImageId,
                                              MIL_ID DestImageId,
                                              long lMinimalVariation,
                                              long lControlFlag);


MFTYPE32 void MFTYPE    MimZoneOfInfluence   (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long OperationFlag);

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* GEOMETRICAL: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MimResize             (MIL_ID SrcImageId,
                                               MIL_ID DestImageId,
                                               double FactorX,
                                               double FactorY,
                                               long InterpolationType);

MFTYPE32 void MFTYPE    MimRotate             (MIL_ID SrcImageId,
                                               MIL_ID DstImageId,
                                               double Angle,
                                               double SrcCenX,
                                               double SrcCenY,
                                               double DstCenX,
                                               double DstCenY,
                                               long   InterpolationMode);

MFTYPE32 void MFTYPE    MimTranslate          (MIL_ID SrcImageId,
                                               MIL_ID DestImageId,
                                               double xShift,
                                               double yShift,
                                               long InterpolationType);

MFTYPE32 void MFTYPE    MimFlip               (MIL_ID SrcImageId, 
                                               MIL_ID DestImageId,
                                               long Operation,
                                               long Mode);                                              

MFTYPE32 void MFTYPE    MimWarp               (MIL_ID SrcImageId, 
                                               MIL_ID DestImageId,
                                               MIL_ID WarpParam1Id,
                                               MIL_ID WarpParam2Id,
                                               long   OperationMode,
                                               long   InterpolationMode);


MFTYPE32 void MFTYPE    MimPolarTransform (MIL_ID  SrcImageId, 
                                           MIL_ID  DestImageId,
                                           double  CenterPosX,       
                                           double  CenterPosY,       
                                           double  StartRadius,      
                                           double  EndRadius,        
                                           double  StartAngle,       
                                           double  EndAngle,        
                                           long    OperationMode,
                                           long    InterpolationMode,
                                           double *DestinationSizeXPtr, 
                                           double *DestinationSizeYPtr); 
                                               
      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* STATISTICAL: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MimHistogram         (MIL_ID SrcImageId,
                                              MIL_ID HistogramListId);

MFTYPE32 void MFTYPE    MimHistogramEqualize (MIL_ID src_id,
                                              MIL_ID dst_id,
                                              long  EqualizationType,
                                              double Alpha,
                                              MCAPICSTTYPE Min,
                                              MCAPICSTTYPE Max);

MFTYPE32 void MFTYPE    MimProject           (MIL_ID SrcImageId,
                                              MIL_ID DestArrayId,
                                              double ProjectionAngle);

MFTYPE32 void MFTYPE    MimFindExtreme       (MIL_ID SrcImageId,
                                              MIL_ID ResultListId,
                                              long ExtremeType);

MFTYPE32 void MFTYPE    MimLocateEvent       (MIL_ID SrcImageId,
                                              MIL_ID EventResultId,
                                              long Condition,
                                              MCAPICSTTYPE CondLow,
                                              MCAPICSTTYPE CondHigh);

MFTYPE32 void MFTYPE    MimCountDifference   (MIL_ID Src1ImageId,
                                              MIL_ID Src2ImageId,
                                              MIL_ID ResId);

MFTYPE32 MIL_ID MFTYPE  MimAllocResult       (MIL_ID SystemId,
                                              long NumberOfResultElement,
                                              long ResultType,
                                              MIL_ID MPTYPE *IdVarPtr);

MFTYPE32 void MFTYPE    MimFree              (MIL_ID ImResultId);

MFTYPE32 long MFTYPE    MimInquire           (MIL_ID BufId,
                                              long InquireType,
                                              void MPTYPE *TargetVarPtr);

MFTYPE32 void MFTYPE    MimGetResult1d       (MIL_ID ImResultId,
                                              long Offresult,
                                              long Sizeresult,
                                              long ResultType,
                                              void MPTYPE *UserTargetArrayPtr);

MFTYPE32 void MFTYPE    MimGetResult         (MIL_ID ImResultId,
                                             long ResultType,
                                             void MPTYPE *UserTargetArrayPtr);


      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* CONVERSION: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE    MimConvert           (MIL_ID SrcImageId,
                                              MIL_ID DestImageId,
                                              long ConversionType);                                                                                            

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* TRANSFORM: */

      /* -------------------------------------------------------------- */

MFTYPE32 void MFTYPE MimFFT                  (MIL_ID SrcImageRId,
                                              MIL_ID SrcImageIId,
                                              MIL_ID DestImageRId,
                                              MIL_ID DestImageIId,
                                              long FFTType);
MFTYPE32 void MFTYPE MimTransform            (MIL_ID SrcImageRId,
                                              MIL_ID SrcImageIId,
                                              MIL_ID DestImageRId,
                                              MIL_ID DestImageIId,
                                              long TransformType,
                                              long ControlFlag);

      /* -------------------------------------------------------------- */

#endif // !M_MIL_LITE

      /* -------------------------------------------------------------- */
      /* -------------------------------------------------------------- */

      /* WINDOWS CE  */

      /* -------------------------------------------------------------- */

#if M_MIL_USE_CE

MFTYPE32 BOOL MFTYPE kbhit(void);
#endif


/* C++ directive if needed */
#ifdef __cplusplus
}
#endif
#endif


