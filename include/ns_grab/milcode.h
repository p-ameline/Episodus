/////////////////////////////////////////////////////////////////
//
// Filename          :  MILCODE.H
// Content           :  function capi of McodeAlloc
//
// Copyright © 1999 Matrox Electronic Systems Ltd.
// All Rights Reserved
//
/////////////////////////////////////////////////////////////////

#ifndef __MILCODE_H__
#define __MILCODE_H__

#if (!M_MIL_LITE) // MIL FULL ONLY

/* C++ directive if needed */
#ifdef __cplusplus
extern "C"
{
#endif

// These are for the undocumented function : McodeAutoResizeDataMatrix
#define MARDM_AUTO_RESIZE_UP     1
#define MARDM_AUTO_RESIZE_DOWN   2

#define MARDM_STATUS_OFFSET                   100
#define MARDM_STATUS_OK                       MARDM_STATUS_OFFSET
#define MARDM_STATUS_INVALID_PARAMETER_FAIL   MARDM_STATUS_OFFSET+1
#define MARDM_STATUS_RATIO_OUT_OF_RANGE_FAIL  MARDM_STATUS_OFFSET+2
#define MARDM_STATUS_NOT_FOUND_FAIL           MARDM_STATUS_OFFSET+3
// End of these are...


/**************************************************************************/
/* CAPI defines                                                           */
/**************************************************************************/

/**************************************************************************/
/* McodeAlloc()  Code types                                               */
/**************************************************************************/

#define M_CODE39                 0x00000001
#define M_DATAMATRIX             0x00000002
#define M_EAN13                  0x00000004
#define M_MAXICODE               0x00000008
#define M_INTERLEAVED25          0x00000010
#define M_CODE128                0x00000020
#define M_BC412                  0x00000040
#define M_CODABAR                0x00000080
#define M_PDF417                 0x00000100
#define M_POSTNET                0x00000200
#define M_PLANET                 0x00000400
#define M_UPC_A                  0x00000800
#define M_UPC_E                  0x00001000
#define M_RSS14                  0x00002000
#define M_RSS14_TRUNCATED        0x00004000
#define M_RSS14_LIMITED          0x00008000
#define M_RSS14_EXPANDED         0x00010000
#define M_RSS14_STACKED          0x00020000
#define M_RSS14_STACKED_OMNI     0x00040000
#define M_RSS14_EXPANDED_STACKED 0x00080000
#define M_PHARMACODE             0x00100000

// Allocation control flags
#define M_CONTROL_FLAG           0x00019400L // Already define as 0x00019400L
#define M_IGNORE_START_STOP      0x00000001

/**************************************************************************/
/* McodeReadMultiple()  Code types                                        */
/**************************************************************************/
#define M_AUTO_DETECT_ANGLE      0x00000001

/**************************************************************************/
/* McodeInquire(), McodeGetResult(), McodeControl() parameters            */
/**************************************************************************/

#define M_CODE_TYPE               0x00000001L
#define M_WRITE_SIZE_X            0x00000002L
#define M_STRING                           3L    // Already define as 3L
#define M_FOREGROUND_VALUE                 4L     // Already define as 4L
#define M_WRITE_SIZE_Y            0x00000005L
#define M_THRESHOLD                         6      // Already define as 6
#define M_STATUS                  0x00008002L
#define M_FOREGROUND_WHITE              0x80L      // Already define as 0x80L
#define M_FOREGROUND_BLACK             0x100L      // Already define as 0x100L
#define M_SEARCH_ANGLE            0x00000100L      // Already define as 0x00000100L
#define M_SEARCH_ANGLE_DELTA_NEG  0x00000200L      // Already define as 0x00000200L
#define M_SEARCH_ANGLE_DELTA_POS  0x00000400L    // Already define as 0x00000400L
#define M_ANGLE                   0x00000800L      // Already define as 0x00000800L
#define M_CELL_SIZE               0x00001000L
#define M_CELL_SIZE_MIN           0x00002000L
#define M_CELL_SIZE_MAX           0x00004000L
#define M_ASPECT_RATIO                  6001L      // Already define as 6001L
#define M_CELL_NUMBER_X           0x00008000L
#define M_CELL_NUMBER_Y           0x00008001L
#define M_SPEED                   M_SPEED_FACTOR  // Already define as   8L
#define M_STRING_SIZE             0x00008003L
#define M_ERROR_CORRECTION        0x00008004L        
#define M_ENCODING                0x00008005L
#define M_DOT_SPACING             0x00008007L

#define M_BIT_STREAM              0x00008008L
#define M_POSITION_X              0x00003400L      // Already define as 0x00003400L  
#define M_POSITION_Y              0x00004400L      // Already define as 0x00004400L  
#define M_MATRIX_BASE_VECTOR_1_X  0x00004500L
#define M_MATRIX_BASE_VECTOR_1_Y  0x00004600L
#define M_MATRIX_BASE_VECTOR_2_X  0x00004700L
#define M_MATRIX_BASE_VECTOR_2_Y  0x00004800L

#define M_DISTANCE_MAX            0x00004900L 
#define M_SCAN_REVERSE            0x00004B00L
#define M_USE_SHARPEN             0x00004C00L 
#define M_USE_PRESEARCH           0x00004D00L

#define M_DATAMATRIX_TYPE         0x00004E00L //Undocumented flag usefull to increase
                                              //speed if the user know his Datamatrix
                                              //are always obsolete or never obsolete...
                                              //Possible values are : M_OBSOLETE, M_REGULAR or
                                              //M_ANY. M_ANY is the default value since it allow
                                              //to read both type of Datamatrix.

#define M_CHECK_DIGIT             0x0000000EL
#define M_SIZE_X                        1002L //Already define as 1002L
#define M_SIZE_Y                        1003L //Already define as 1003L
#define M_TOP_LEFT_X                    1004L
#define M_TOP_LEFT_Y                    1005L

#define M_TYPE_LONG               0x00040000L      // Already define as 0x00040000L
#define M_TYPE_DOUBLE             0x00100000L      // Already define as 0x00010000L
#define M_ESCAPE_SEQUENCE         0x00000010L

#define M_SIZE_BYTE                     5061L        // already defined as 5061L

/**************************************************************************/
/* McodeSave(), McodeRestore()
/**************************************************************************/
#define M_MEMORY                         5L  // Already defined in Mil.h

/**************************************************************************/
/* McodeInquire(), McodeGetResult(), McodeControl() parameter values      */
/**************************************************************************/

#define M_ECC_050                 0            // These numbers are importants!!
#define M_ECC_080                 1
#define M_ECC_100                 2
#define M_ECC_140                 3
#define M_ECC_NONE                4
#define M_ECC_CHECK_DIGIT         5
#define M_ECC_UNKNOWN             6
#define M_ECC_200                 7
#define M_ECC_REED_SOLOMON_0      10
#define M_ECC_REED_SOLOMON_1      (M_ECC_REED_SOLOMON_0 + 1) 
#define M_ECC_REED_SOLOMON_2      (M_ECC_REED_SOLOMON_0 + 2)
#define M_ECC_REED_SOLOMON_3      (M_ECC_REED_SOLOMON_0 + 3)
#define M_ECC_REED_SOLOMON_4      (M_ECC_REED_SOLOMON_0 + 4)
#define M_ECC_REED_SOLOMON_5      (M_ECC_REED_SOLOMON_0 + 5)
#define M_ECC_REED_SOLOMON_6      (M_ECC_REED_SOLOMON_0 + 6)
#define M_ECC_REED_SOLOMON_7      (M_ECC_REED_SOLOMON_0 + 7)
#define M_ECC_REED_SOLOMON_8      (M_ECC_REED_SOLOMON_0 + 8)
#define M_ECC_REED_SOLOMON        19
#define M_ECC_010                 20
#define M_ECC_040                 21
#define M_ECC_060                 22
#define M_ECC_070                 23
#define M_ECC_090                 24
#define M_ECC_110                 25
#define M_ECC_120                 26
#define M_ECC_130                 27

#define M_ENC_NUM                 0            // These numbers are importants!!
#define M_ENC_ALPHA               1    
#define M_ENC_ALPHANUM_PUNC       2
#define M_ENC_ALPHANUM            3
#define M_ENC_ASCII               4
#define M_ENC_ISO8                5
#define M_ENC_MODE2               6
#define M_ENC_MODE3               (M_ENC_MODE2 + 1)
#define M_ENC_MODE4               (M_ENC_MODE2 + 2)
#define M_ENC_MODE5               (M_ENC_MODE2 + 3)
#define M_ENC_MODE6               (M_ENC_MODE2 + 4)
#define M_ENC_STANDARD           11
#define M_ENC_UNKNOWN            13

#define M_STATUS_OK               0
#define M_STATUS_CRC_FAILED       1
#define M_STATUS_ECC_UNKNOWN      2
#define M_STATUS_ENC_UNKNOWN      3
#define M_STATUS_WRITE_OK         4
#define M_STATUS_WRITE_FAILED     5
#define M_STATUS_NOT_FOUND        6

#define M_VERY_LOW               0L   // Already define as 0L
#define M_LOW                    1L   // Already define as 1L
#define M_MEDIUM                 2L   // Already define as 2L
#define M_HIGH                   3L   // Already define as 3L
#define M_VERY_HIGH              4L   // Already define as 4L

#define M_ANY                     0x11000000L      // Already define as 0x11000000L
#define M_DEFAULT                 0x10000000L      // Already define as 0x10000000L

#define M_PRESENT                          2L// Already define as 2L
#define M_ABSENT                           3L

#define M_OBSOLETE                      25L
#define M_REGULAR                       0x00020000L  // Already define as 0x00020000L

/**************************************************************************/
/* Function prototypes                                                    */
/**************************************************************************/

#ifndef __midl // MIDL compiler used by ActiveMIL

MFTYPE32 MIL_ID MFTYPE McodeAlloc     (MIL_ID SystemId, long CodeType, long ControlFlag, MIL_ID *CodeIdPtr);
MFTYPE32 void   MFTYPE McodeControl   (MIL_ID CodeId, long ControlType, double ControlValue);
MFTYPE32 long   MFTYPE McodeInquire   (MIL_ID CodeId, long ParamToInquire, void *ValuePtr);
MFTYPE32 void   MFTYPE McodeRead      (MIL_ID CodeId, MIL_ID ImageBufId, long ControlFlag);
MFTYPE32 void   MFTYPE McodeWrite     (MIL_ID CodeId, MIL_ID ImageBufId, MIL_TEXT_PTR String, long ControlFlag);
MFTYPE32 void   MFTYPE McodeGetResult (MIL_ID CodeId, long ResultType, void *ResultPtr);
MFTYPE32 void   MFTYPE McodeSave      (MIL_TEXT_PTR FileName, MIL_ID CodeId, long ControlFlag);
MFTYPE32 MIL_ID MFTYPE McodeRestore   (MIL_TEXT_PTR FileName, MIL_ID SystemId, long ControlFlag, MIL_ID* CodeIdPtr);
MFTYPE32 void   MFTYPE McodeFree      (MIL_ID CodeId );
MFTYPE32 MIL_ID MFTYPE McodeAutoResizeDataMatrix( MIL_ID System, MIL_ID Image, 
                                                  long MinCellSize, long MaxCellSize,
                                                  long CellNumberX, long CellNumberY, 
                                                  long DotSpacing, long ForegroundColor,
                                                  double MaxRatioToApply, long ResizeDirection,
                                                  double* XRatio, double* YRatio, long* Status );


#endif /* #ifdef __midl */

/* C++ directive if needed */
#ifdef __cplusplus
}
#endif

#endif // !M_MIL_LITE

#endif /* __MILCODE_H__ */





