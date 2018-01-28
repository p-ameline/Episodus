/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*  Filename:  MILSETUP.H                                                  */
/*  Owner   :  Matrox Imaging dept.                                        */
/*  Rev     :  $Revision: 1.1 $                                         */
/*  Content :  This file contains definitions for specifying the target    */
/*             compile environment and the default state to set for        */
/*             MIL (Matrox Imaging Library). It also defines the           */
/*             MappAllocDefault() and MappFreeDefault() macros.            */
/*                                                                         */
/*  COPYRIGHT (c) 1993-1995  Matrox Electronic Systems Ltd.                */
/*  All Rights Reserved.                                                   */
/*                                                                         */
/*                                                                         */
/***************************************************************************/

/************************************************************************/
/* MIL LITE IDENTIFICATION FLAG                                         */
/* Activate or Deactivate MIL Lite flag                                 */
/************************************************************************/

#define M_MIL_LITE                 0

/************************************************************************/
/* SETUP SPECIFIED FLAG                                                 */
/* Activate or Deactivate MIL use-setup flag                            */
/************************************************************************/

#define M_MIL_USE_SETUP            1

/************************************************************************/
/* COMPILATION FLAG                                                     */
/* One and only one flag must be active                                 */
/*                                                                      */
/* Activate or Deactivate DOS32 compile mode                            */
/* Activate or Deactivate NT under WINDOWS compile mode                 */
/************************************************************************/

#ifndef M_MIL_USE_OS
#define M_MIL_USE_OS               1
#define M_MIL_USE_DOS_32           0
#define M_MIL_USE_WINDOWS          1
#define M_MIL_USE_CE               0
#define M_MIL_USE_PPC              0
#endif

#if !(M_MIL_USE_DOS_32 || M_MIL_USE_WINDOWS || M_MIL_USE_PPC || M_MIL_USE_QNX)
#error You must set at least one compilation flag.
#endif

#if (M_MIL_USE_DOS_32 && M_MIL_USE_WINDOWS)
#error You must select one and only one compilation flag.
#endif

#if (M_MIL_USE_DOS_32 && M_MIL_USE_PPC)
#error You must select one and only one compilation flag.
#endif

#if (M_MIL_USE_WINDOWS && M_MIL_USE_PPC)
#error You must select one and only one compilation flag.
#endif

#if (M_MIL_USE_DOS_32 && M_MIL_USE_QNX)
#error You must select one and only one compilation flag.
#endif

#if (M_MIL_USE_WINDOWS && M_MIL_USE_QNX)
#error You must select one and only one compilation flag.
#endif


#if defined(_MSC_VER) && (M_MIL_USE_DOS_32)
#error Do not use DOS 32 compile mode with MSC.
#endif

/************************************************************************/
/* IMAGE PATH                                                           */
/* Path to the images used by the examples                              */
/************************************************************************/
#if !M_MIL_USE_CE
#define M_IMAGE_PATH               MIL_TEXT("C:\\Program Files\\Matrox Imaging\\Images\\")
#else
#define M_IMAGE_PATH               MIL_TEXT("\\cedisk\\mil\\images\\")
#endif

/************************************************************************/
/* ERROR MESSAGES AND VALUES USAGE FLAG                                 */
/* Activate or Deactivate error messages inclusion                      */
/************************************************************************/

#define M_MIL_USE_ERROR_MESSAGE    0

/************************************************************************/
/* BLOB ANALYSIS MODULE PROGRAMMING FLAG                                */
/* Activate or Deactivate blob module programming                       */
/************************************************************************/

#define M_MIL_USE_BLOB             1

/************************************************************************/
/* PATTERN RECOGNITION MODULE PROGRAMMING FLAG                          */
/* Activate or Deactivate pat module programming                        */
/************************************************************************/

#define M_MIL_USE_PAT              1

/************************************************************************/
/* OCR MODULE PROGRAMMING FLAG                                          */
/* Activate or Deactivate ocr module programming                        */
/************************************************************************/

#define M_MIL_USE_OCR              1

/************************************************************************/
/* MEASUREMENT MODULE PROGRAMMING FLAG                                  */
/* Activate or Deactivate meas module programming                       */
/************************************************************************/

#define M_MIL_USE_MEAS             1

/************************************************************************/
/* CALIBRATION MODULE PROGRAMMING FLAG                                  */
/* Activate or Deactivate cal module programming                        */
/************************************************************************/

#define M_MIL_USE_CAL              1

/************************************************************************/
/* CODE MODULE PROGRAMMING FLAG                                         */
/* Activate or Deactivate code module programming                       */
/************************************************************************/

#define M_MIL_USE_CODE             1

/************************************************************************/
/* MODEL FINDER MODULE PROGRAMMING FLAG                                 */
/* Activate or Deactivate model finder programming                      */
/************************************************************************/

#define M_MIL_USE_MOD              1

/************************************************************************/
/* PULSAR SYSTEM USAGE FLAG AND INCLUDE PATH                            */
/* Activate or Deactivate pulsar module programming                     */
/************************************************************************/

#define M_MIL_USE_PULSAR           0

/************************************************************************/
/* GENESIS SYSTEM USAGE FLAG AND INCLUDE PATH                           */
/* Activate or Deactivate Genesis module programming                    */
/************************************************************************/

#define M_MIL_USE_GENESIS          0

/************************************************************************/
/* CORONA SYSTEM USAGE FLAG AND INCLUDE PATH                            */
/* Activate or Deactivate Corona module programming                     */
/************************************************************************/

#define M_MIL_USE_CORONA           0

/************************************************************************/
/* CORONA-II SYSTEM USAGE FLAG AND INCLUDE PATH                         */
/* Activate or Deactivate Corona II module programming                  */
/************************************************************************/

#define M_MIL_USE_CORONA_II        0

/************************************************************************/
/* METEOR_II SYSTEM USAGE FLAG AND INCLUDE PATH                         */
/* Activate or Deactivate Meteor II module programming                  */
/************************************************************************/

#define M_MIL_USE_METEOR_II        0

/************************************************************************/
/* METEOR_II_DIG SYSTEM USAGE FLAG AND INCLUDE PATH.                    */
/* Activate or Deactivate Meteor II Dig module programming.             */
/************************************************************************/

#define M_MIL_USE_METEOR_II_DIG    0

/************************************************************************/
/* METEOR_II_CL SYSTEM USAGE FLAG AND INCLUDE PATH.                     */
/* Activate or Deactivate Meteor II CL module programming.              */
/************************************************************************/

#define M_MIL_USE_METEOR_II_CL     0

/************************************************************************/
/* ORION SYSTEM USAGE FLAG AND INCLUDE PATH.                            */
/* Activate or Deactivate Orion module programming.                     */
/************************************************************************/

#define M_MIL_USE_ORION            0

/************************************************************************/
/* CRONOS SYSTEM USAGE FLAG AND INCLUDE PATH.                            */
/* Activate or Deactivate Cronos module programming.                     */
/************************************************************************/

#define M_MIL_USE_CRONOS            0

/************************************************************************/
/* ODYSSEY SYSTEM USAGE FLAG AND INCLUDE PATH.                            */
/* Activate or Deactivate Odyssey module programming.                     */
/************************************************************************/

#define M_MIL_USE_ODYSSEY          0

/************************************************************************/
/* NEWBOARD SYSTEM USAGE FLAG AND INCLUDE PATH                          */
/* Activate or Deactivate newboard module programming                   */
/************************************************************************/

#define M_MIL_USE_NEWBOARD         1

/************************************************************************/
/* DEFAULT STATE INITIALIZATION FLAG                                    */
/************************************************************************/

#define M_SETUP                    M_DEFAULT

/************************************************************************/
/* DEFAULT SYSTEM SPECIFICATIONS                                        */
/************************************************************************/

#define M_DEF_SYSTEM_TYPE          M_SYSTEM_CRONOSPLUS
#define M_DEF_SYSTEM_NUM           M_DEV0
#define M_SYSTEM_SETUP             M_DEF_SYSTEM_TYPE

/************************************************************************/
/* DEFAULT DIGITIZER SPECIFICATIONS                                     */
/************************************************************************/

#define M_DEF_DIGITIZER_NUM        M_DEV0
#define M_DEF_DIGITIZER_FORMAT     MIL_TEXT("M_PAL_YC")
#define M_DEF_DIGITIZER_INIT       M_DEFAULT
#define M_CAMERA_SETUP             M_DEF_DIGITIZER_FORMAT

/************************************************************************/
/* DEFAULT DISPLAY SPECIFICATIONS                                       */
/************************************************************************/

#define M_DEF_DISPLAY_NUM                  M_DEV0
#define M_DEF_DISPLAY_FORMAT               MIL_TEXT("M_DEFAULT")
#define M_DEF_DISPLAY_INIT                 M_DEFAULT
#define M_DISPLAY_SETUP                    M_DEF_DISPLAY_FORMAT
#define M_DEF_DISPLAY_KEY_COLOR            0
#define M_DEF_DISPLAY_KEY_ENABLE_ON_ALLOC  0
#define M_DEF_DISPLAY_KEY_DISABLE_ON_FREE  0

/************************************************************************/
/* DEFAULT IMAGE BUFFER SPECIFICATIONS                                  */
/************************************************************************/

#define M_DEF_IMAGE_NUMBANDS_MIN   1
#define M_DEF_IMAGE_SIZE_X_MIN     512
#define M_DEF_IMAGE_SIZE_Y_MIN     480
#define M_DEF_IMAGE_SIZE_X_MAX     4096
#define M_DEF_IMAGE_SIZE_Y_MAX     4096
#define M_DEF_IMAGE_TYPE           8+M_UNSIGNED
#define M_DEF_IMAGE_ATTRIBUTE_MIN  M_IMAGE+M_PROC

/***************************************************************************/
/* LocalBufferAllocDefault - Local macro to allocate a default MIL buffer: */
/*                                                                         */
/* MIL_ID *SystemIdVarPtr;                                                 */
/* MIL_ID *DisplayIdVarPtr;                                                */
/* MIL_ID *ImageIdVarPtr;                                                  */
/*                                                                         */
/***************************************************************************/
#define LocalBufferAllocDefault(SystemIdVarPtr,DisplayIdVarPtr,DigitizerIdVarPtr,ImageIdVarPtr) \
{                                                                            \
  /* local variables */                                                      \
  long m_def_image_numbands = M_DEF_IMAGE_NUMBANDS_MIN;                      \
  long m_def_image_size_x   = M_DEF_IMAGE_SIZE_X_MIN;                        \
  long m_def_image_size_y   = M_DEF_IMAGE_SIZE_Y_MIN;                        \
  long m_def_image_type     = M_DEF_IMAGE_TYPE;                              \
  long m_def_image_attribute= M_DEF_IMAGE_ATTRIBUTE_MIN;                     \
  MIL_ID *LNullPtr = M_NULL;                                                 \
  MIL_ID LNull     = M_NULL;                                                 \
                                                                             \
  /* determines the needed size band, x, y, type and attribute */            \
  if ((((MIL_ID *)(DisplayIdVarPtr))   != LNullPtr)   &&                     \
      ((*((MIL_ID *)(DisplayIdVarPtr)))!= LNull))                            \
     {                                                                       \
     if ((((MIL_ID *)(DigitizerIdVarPtr))   != LNullPtr)   &&                \
         ((*((MIL_ID *)(DigitizerIdVarPtr)))!= LNull))                       \
        {                                                                    \
        m_def_image_size_x =                                                 \
          MdigInquire(*((MIL_ID *)(DigitizerIdVarPtr)),M_SIZE_X,M_NULL);     \
        m_def_image_size_y =                                                 \
          MdigInquire(*((MIL_ID *)(DigitizerIdVarPtr)),M_SIZE_Y,M_NULL);     \
        m_def_image_type =                                                   \
          MdigInquire(*((MIL_ID *)(DigitizerIdVarPtr)),M_TYPE,M_NULL);       \
        }                                                                    \
     else                                                                    \
        {                                                                    \
        m_def_image_size_x = M_DEF_IMAGE_SIZE_X_MIN;                         \
        m_def_image_size_y = M_DEF_IMAGE_SIZE_Y_MIN;                         \
        m_def_image_type   = M_DEF_IMAGE_TYPE;                               \
        }                                                                    \
     }                                                                       \
  if (m_def_image_size_x < M_DEF_IMAGE_SIZE_X_MIN)                           \
      m_def_image_size_x = M_DEF_IMAGE_SIZE_X_MIN;                           \
  if (m_def_image_size_y < M_DEF_IMAGE_SIZE_Y_MIN)                           \
      m_def_image_size_y = M_DEF_IMAGE_SIZE_Y_MIN;                           \
  if (m_def_image_size_x > M_DEF_IMAGE_SIZE_X_MAX)                           \
      m_def_image_size_x = M_DEF_IMAGE_SIZE_X_MAX;                           \
  if (m_def_image_size_y > M_DEF_IMAGE_SIZE_Y_MAX)                           \
      m_def_image_size_y = M_DEF_IMAGE_SIZE_Y_MAX;                           \
  if (((m_def_image_type & 0xFF) < (M_DEF_IMAGE_TYPE & 0xFF)) ||             \
      ((m_def_image_type & 0xFF) > (M_DEF_IMAGE_TYPE & 0xFF)))               \
      m_def_image_type   = M_DEF_IMAGE_TYPE;                                 \
                                                                             \
  /* determines the needed attribute and number of band */                   \
  m_def_image_attribute = M_DEF_IMAGE_ATTRIBUTE_MIN;                         \
  m_def_image_numbands  = M_DEF_IMAGE_NUMBANDS_MIN;                          \
  if ((((MIL_ID *)(DisplayIdVarPtr))   != LNullPtr)   &&                     \
      ((*((MIL_ID *)(DisplayIdVarPtr)))!= LNull))                            \
     m_def_image_attribute += M_DISP;                                        \
  if (((((MIL_ID *)DigitizerIdVarPtr))   != LNullPtr) &&                     \
      ((*((MIL_ID *)(DigitizerIdVarPtr)))!= LNull))                          \
      {                                                                      \
      m_def_image_attribute += M_GRAB;                                       \
      m_def_image_numbands =                                                 \
        MdigInquire(*(MIL_ID *)DigitizerIdVarPtr,M_SIZE_BAND,M_NULL);        \
      if (m_def_image_numbands<M_DEF_IMAGE_NUMBANDS_MIN)                     \
         m_def_image_numbands=M_DEF_IMAGE_NUMBANDS_MIN;                      \
      }                                                                      \
                                                                             \
  /* allocates a monochromatic or color image buffer */                      \
  MbufAllocColor(*((MIL_ID *)(SystemIdVarPtr)),                              \
                m_def_image_numbands,                                        \
                m_def_image_size_x,                                          \
                m_def_image_size_y,                                          \
                m_def_image_type,                                            \
                m_def_image_attribute,                                       \
                ((MIL_ID *)(ImageIdVarPtr)));                                \
                                                                             \
  /* clear and display the image buffer */                                   \
  if (((DisplayIdVarPtr) !=LNullPtr) &&                                      \
      ((*((MIL_ID *)(DisplayIdVarPtr)))!=LNull)  &&                          \
      ((*((MIL_ID *)(ImageIdVarPtr)))  !=LNull))                             \
     {                                                                       \
     MbufClear(*((MIL_ID *)(ImageIdVarPtr)), 0);                            \
     MdispSelect(*((MIL_ID *)(DisplayIdVarPtr)),                             \
                 *((MIL_ID *)(ImageIdVarPtr)));                              \
     }                                                                       \
}

/**************************************************************************/
/* MappAllocDefault - macro to allocate default MIL objects:              */
/*                                                                        */
/* long    InitFlag;                                                      */
/* MIL_ID *ApplicationIdVarPtr;                                           */
/* MIL_ID *SystemIdVarPtr;                                                */
/* MIL_ID *DisplayIdVarPtr;                                               */
/* MIL_ID *DigitizerIdVarPtr;                                             */
/* MIL_ID *ImageIdVarPtr;                                                 */
/*                                                                        */
/* Note:                                                                  */
/*       An application must be allocated before a system.                */
/*       An system must be allocated before a display,digitzer or image.  */
/*                                                                        */
/**************************************************************************/

#define MappAllocDefault(InitFlag,ApplicationIdVarPtr,SystemIdVarPtr,DisplayIdVarPtr,DigitizerIdVarPtr,ImageIdVarPtr) \
{                                                                      \
  /* local variables */                                                \
  MIL_ID *LNullPtr = M_NULL;                                           \
  MIL_ID LNull     = M_NULL;                                           \
                                                                       \
  /* allocate a MIL application. */                                    \
  if (((MIL_ID *)(ApplicationIdVarPtr)) != LNullPtr)                   \
     MappAlloc(InitFlag,((MIL_ID *)(ApplicationIdVarPtr)));            \
                                                                       \
  /* allocate a system */                                              \
  if ((((MIL_ID *)(SystemIdVarPtr))        != LNullPtr) &&             \
      (((MIL_ID *)(ApplicationIdVarPtr))   != LNullPtr) &&             \
      ((*((MIL_ID *)(ApplicationIdVarPtr)))!= LNull))                  \
     MsysAlloc(M_DEF_SYSTEM_TYPE,                                      \
               M_DEF_SYSTEM_NUM,                                       \
               (InitFlag),                                             \
               ((MIL_ID *)(SystemIdVarPtr)));                          \
                                                                       \
  /* allocate a display */                                             \
  if ((((MIL_ID *)(DisplayIdVarPtr)) != LNullPtr)  &&                  \
      (((MIL_ID *)(SystemIdVarPtr))  != LNullPtr)  &&                  \
      ((*((MIL_ID *)(SystemIdVarPtr))) != LNull))                      \
     MdispAlloc(*((MIL_ID *)(SystemIdVarPtr)),                         \
               M_DEF_DISPLAY_NUM,                                      \
               M_DEF_DISPLAY_FORMAT,                                   \
               M_DEF_DISPLAY_INIT,                                     \
               ((MIL_ID *)(DisplayIdVarPtr)));                         \
                                                                       \
  /* allocate a digitizer */                                           \
  if ((((MIL_ID *)(DigitizerIdVarPtr)) != LNullPtr) &&                 \
      (((MIL_ID *)(SystemIdVarPtr))    != LNullPtr) &&                 \
      ((*((MIL_ID *)(SystemIdVarPtr))) != LNull))                      \
     MdigAlloc(*((MIL_ID *)(SystemIdVarPtr)),                          \
               M_DEF_DIGITIZER_NUM,                                    \
               M_DEF_DIGITIZER_FORMAT,                                 \
               M_DEF_DIGITIZER_INIT,                                   \
               ((MIL_ID *)(DigitizerIdVarPtr)));                       \
                                                                       \
  /* enable keying if keying is supported */                           \
  if (((DisplayIdVarPtr) !=LNullPtr) &&                                \
      ((*((MIL_ID *)(DisplayIdVarPtr)))!=LNull)  &&                    \
      (M_DEF_DISPLAY_KEY_ENABLE_ON_ALLOC!=LNull) &&                    \
      (MdispInquire(*((MIL_ID *)(DisplayIdVarPtr)),M_KEY_SUPPORTED,0)))\
     MdispOverlayKey(*((MIL_ID *)(DisplayIdVarPtr)),M_KEY_ON_COLOR,    \
                        M_EQUAL,0xFFL,M_DEF_DISPLAY_KEY_COLOR);        \
                                                                       \
  /* allocate an image buffer */                                       \
  if ((((MIL_ID *)(ImageIdVarPtr))  != LNullPtr)  &&                   \
      (((MIL_ID *)(SystemIdVarPtr)) != LNullPtr)  &&                   \
      ((*((MIL_ID *)(SystemIdVarPtr)))!= LNull))                       \
     LocalBufferAllocDefault(((MIL_ID *)(SystemIdVarPtr)),             \
                             ((MIL_ID *)(DisplayIdVarPtr)),            \
                             ((MIL_ID *)(DigitizerIdVarPtr)),          \
                             ((MIL_ID *)(ImageIdVarPtr)));             \
                                                                       \
  /* enable keying if keying is supported */                           \
  if (((DisplayIdVarPtr) !=LNullPtr) &&                                \
      ((*((MIL_ID *)(DisplayIdVarPtr)))!=LNull)  &&                    \
      (M_DEF_DISPLAY_KEY_ENABLE_ON_ALLOC!=LNull) &&                    \
      (MdispInquire(*((MIL_ID *)(DisplayIdVarPtr)),M_KEY_SUPPORTED,0)))\
     MdispOverlayKey(*((MIL_ID *)(DisplayIdVarPtr)),M_KEY_ON_COLOR,    \
                        M_EQUAL,0xFFL,M_DEF_DISPLAY_KEY_COLOR);        \
}

/************************************************************************/
/* MappFreeDefault - macro to free default MIL objects:                 */
/*                                                                      */
/* MIL_ID ApplicationId;                                                */
/* MIL_ID SystemId;                                                     */
/* MIL_ID DisplayId;                                                    */
/* MIL_ID DigitizerId;                                                  */
/* MIL_ID ImageId;                                                      */
/*                                                                      */
/************************************************************************/

#define MappFreeDefault(ApplicationId,SystemId,DisplayId,DigitizerId,BufferId) \
{                                                                           \
  MIL_ID NullId = M_NULL;                                                   \
                                                                            \
  /* free the image buffer */                                               \
  if ((BufferId) != NullId)                                                 \
     {                                                                      \
     MbufFree((BufferId));                                                  \
     }                                                                      \
                                                                            \
  /* free digitizer */                                                      \
  if ((DigitizerId) != NullId)                                              \
     {                                                                      \
     MdigFree((DigitizerId));                                               \
     }                                                                      \
                                                                            \
  /* free the display */                                                    \
  if ((DisplayId) != NullId)                                                \
     {                                                                      \
     if ((M_DEF_DISPLAY_KEY_DISABLE_ON_FREE != NullId)&&                    \
         MdispInquire((DisplayId),M_KEY_SUPPORTED,0))                       \
        MdispOverlayKey((DisplayId),M_KEY_OFF,M_NULL,M_NULL,M_NULL);        \
     MdispFree((DisplayId));                                                \
     }                                                                      \
                                                                            \
  /* free the system */                                                     \
  if ((SystemId) != NullId)                                                 \
     {                                                                      \
     MsysFree((SystemId));                                                  \
     }                                                                      \
                                                                            \
  /* free the system */                                                     \
  if ((ApplicationId) != NullId)                                            \
     {                                                                      \
     MappFree((ApplicationId));                                             \
     }                                                                      \
}
