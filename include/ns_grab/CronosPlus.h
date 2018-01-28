//==============================================================================
//
// Filename:  CRONOS_PLUS.H
// Owner   :  Matrox Imaging dept.
// Rev     :
// Content :  This file contains the new defines that are needed by the user
//            to use the MIL current library with the CRONOS_PLUS.
//
// COPYRIGHT (c) Matrox Electronic Systems Ltd.
// All Rights Reserved
//==============================================================================
#if !M_MIL_USE_QNX && !M_MIL_USE_PPC

#ifndef __CRONOS_PLUS_H
#define __CRONOS_PLUS_H

#define M_IRQ_GLOBAL_OBJECT           125L

// CronosPlus board type definition
#define M_CRONOS_PLUS_MAX_DIG            17L        // If this value is changed, make sure the Table in InthdlerOb.h is of the proper size

//==============================================================================
//
// CRONOS_PLUS Lib prototypes
//
//==============================================================================
#ifdef __cplusplus
extern "C"
{
#endif

typedef long (MFTYPE *CPAPPHOOKFCTPTR)(long   FHookType, 
                                       void  *FExpansionFlagPtr);
                                      
long MFTYPE 
CpappAlloc(                  long            InitFlag, 
                             MIL_ID   *ApplicationID,
                             void     *ThreadContextPtr);
                                     
void MFTYPE 
CpappControl(                long            ControlType, 
                             long            ControlFlag,
                             void     *ThreadContextPtr);

long MFTYPE 
CpappInquire(                long            InquireType, 
                             long     *UserVarPtr,
                             void     *ThreadContextPtr);

long MFTYPE 
CpappGetError(               long            ErrorType, 
                             void     *UserPtr,
                             void     *ThreadContextPtr);

void MFTYPE 
CpappFree(                   MIL_ID          ApplicationID,
                             void     *ThreadContextPtr);

long MFTYPE 
CpsysAlloc(                  long            SystemType, 
                             long            Number, 
                             long            InitFlag, 
                             MIL_ID   *SystemID,
                             void     *ThreadContextPtr);
                                     
long MFTYPE 
CpsysInquire(                MIL_ID          SystemID,
                             long            InquireType, 
                             long     *UserVarPtr,
                             void     *ThreadContextPtr);

long MFTYPE 
CpsysControl(                MIL_ID          SystemID, 
                             long            ControlType, 
                             long            ControlFlag,
                             void     *ThreadContextPtr);
                                     
void MFTYPE 
CpsysConfigAccess(           MIL_ID          SystemID,
                             long            VendorId,      
                             long            DeviceId,      
                             long            DeviceNum,     
                             long            OperationFlag, 
                             long            OperationType, 
                             long            Offset,        
                             long            Size,
                             void           *UserArrayPtr,          
                             void     *ThreadContext);

void MFTYPE 
CpsysFree(                   MIL_ID          SystemID,
                             void     *ThreadContextPtr);

long MFTYPE 
CpdigAlloc(                  MIL_ID          SystemID, 
                             long            DeviceNum, 
                             MIL_TEXT_PTR        DataFormat, 
                             long            InitFlag, 
                             MIL_ID   *IdVarPtr,
                             void     *ThreadContextPtr);
                                        
void MFTYPE
CpdigFastAlloc(              MIL_ID          DigitizerID,
                             long            InitFlag,
                             void     *ThreadContextPtr);

void MFTYPE 
CpdigFree(                   MIL_ID          DigitizerID,
                             void     *ThreadContextPtr);

void MFTYPE 
CpdigFastFree(               MIL_ID          DigitizerID,
                             void     *ThreadContextPtr);

void MFTYPE 
CpdigGrabContinuous(         MIL_ID          DigitizerID, 
                             MIL_ID          BufferID,
                             void     *ThreadContextPtr);

void MFTYPE 
CpdigGrab(                   MIL_ID          DigitizerID, 
                             MIL_ID          BufferID,
                             long            NbIteration,
                             void     *ThreadContextPtr);
                                        
void MFTYPE 
CpdigHalt(                   MIL_ID          DigitizerID,
                             void     *ThreadContextPtr);

void MFTYPE 
CpdigGrabWait(               MIL_ID          DigitizerID,
                             long            Flag,
                             void     *ThreadContextPtr);
                                        
void MFTYPE 
CpdigControl(                MIL_ID          DigitizerID,
                             long            ControlType, 
                             double          ControlValue,
                             void     *ThreadContextPtr);
                                        
long MFTYPE 
CpdigInquire(                MIL_ID          DigitizerID,
                             long            InquireType, 
                             void     *UserVarPtr,
                             void     *ThreadContextPtr);
                                        
void MFTYPE 
CpdigReference(               MIL_ID          DigitizerID,
                             long            ReferenceType, 
                             long            ReferenceLevel,
                             void     *ThreadContextPtr);
                                        
void MFTYPE 
CpdigChannel(                MIL_ID          DigitizerID,
                             long            Channel,
                             void     *ThreadContextPtr);

MDIGHOOKFCTPTR MFTYPE 
CpdigHookFunction(           MIL_ID          DigitizerID,
                             long            HookType,
                             MDIGHOOKFCTPTR  HookHandlerPtr,
                             void     *UserDataPtr,
                             void     *ThreadContextPtr);

MSYSHOOKFCTPTR MFTYPE 
CpSysHookFunction(           MIL_ID          SystemId,
                             long            HookType,
                             MSYSHOOKFCTPTR  HookHandlerPtr,
                             void           *UserDataPtr,
                             void           *ThreadContextPtr);

CPAPPHOOKFCTPTR MFTYPE 
CpappHookFunction(           long            HookType,
                             CPAPPHOOKFCTPTR  HookHandlerPtr,
                             void     *UserDataPtr,
                             void     *ThreadContextPtr);

long MFTYPE 
CpdispAlloc(                 MIL_ID          SystemID, 
                             long            DeviceNum, 
                             MIL_TEXT_CHAR  *DispFormat, 
                             long            InitFlag, 
                             MIL_ID         *IdVarPtr,
                             void           *ThreadContextPtr);


void MFTYPE
CpdispSetLut(                 MIL_ID          DisplayID, 
                             void           *LutData, 
                             void     *ThreadContextPtr);

void MFTYPE 
CpdispOverlayKey(             MIL_ID          DisplayID, 
                             long            KeyMode, 
                             long            KeyCond, 
                             long            KeyMask,
                             long            KeyColor,
                             void     *ThreadContextPtr);
                                        
void MFTYPE 
CpdispControl(                MIL_ID          DisplayID,
                             long            ControlType, 
                             long            Value,
                             void     *ThreadContextPtr);

void MFTYPE 
CpdispSelect(                 MIL_ID         DisplayID,
                             MIL_ID         BufferID,
                             void     *ThreadContextPtr);
                                        
long MFTYPE 
CpdispInquire(                MIL_ID          DisplayID, 
                             long            InquireType, 
                             void     *UserVarPtr,
                             void     *ThreadContextPtr);
                                        
void MFTYPE 
CpdispFree(                   MIL_ID          DisplayID,
                             void     *ThreadContextPtr);
                                                 
#ifndef MDISPHOOKFCTPTR

typedef long (MFTYPE  *MDISPHOOKFCTPTR)(long         HookType,
                                        MIL_ID       EventId,
                                        void  *UserPtr);
#endif


MDISPHOOKFCTPTR MFTYPE 
CpdispHookFunction(           MIL_ID          DisplayID,
                             long            HookType,
                             MDISPHOOKFCTPTR HookHandlerPtr,
                             void     *UserDataPtr,
                             void     *ThreadContextPtr);

void MFTYPE 
CpdispZoom(                   MIL_ID          DisplayID, 
                             long            XFactor, 
                             long            YFactor,
                             void     *ThreadContextPtr);

void MFTYPE 
CpdispPan(                    MIL_ID          DisplayID, 
                             long            XOffset, 
                             long            YOffset,
                             void     *ThreadContextPtr);

long MFTYPE 
CExtGrabHandler(             long            FHookType, 
                             MIL_ID          EventId, 
                             void     *FExpansionFlagPtr);

long MFTYPE 
CpbufCreateColor(             MIL_ID         SystemID,
                              MILBUFFERINFO  BufInfo,
                              MIL_ID   *IdVarPtr,
                              void     *ThreadContextPtr);

void MFTYPE 
CpbufFree(                    MIL_ID         BufferID,
                              void     *ThreadContextPtr);

void MFTYPE 
CpbufControl(                 MIL_ID         BufferID,
                              long           ControlType, 
                              long           Value,
                              void     *ThreadContextPtr);

void MFTYPE 
CpbufInquire(                 MIL_ID         BufferID,
                              long           InquireType, 
                              void     *UserVarPtr,
                              void     *ThreadContextPtr);


#ifdef __cplusplus
}
#endif

#endif //#if !M_MIL_USE_QNX && !M_MIL_USE_PPC
#endif