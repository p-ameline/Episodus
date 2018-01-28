/***************************************************************************/
/*

    Filename:  MILERR.H
    Owner   :  Matrox Imaging dept.
    Rev     :  $Revision: 1.1 $
    Content :  This file contains the defines that are used by the
               mil functions to generate error codes and messages that
               will be used by the MIL Application Error Logging and
               other error related functions.

    COPYRIGHT (c) 1993  Matrox Electronic Systems Ltd.
    All Rights Reserved

*/
/***************************************************************************/

#ifndef  __MILERR_H
#define  __MILERR_H

/*
First line is the error code.
Second line is the error message for this code.
Other 9 lines are optional sub-messages with more detailed information.

------------------------------------------------------------------------
-                                                                      -
-  WARNING: Strings (including the NULL character at the end) should   -
-           never be longer than M_ERROR_MESSAGE_SIZE (128L).          -
-           No checking is done anywhere in the code to ensure this.   -
-                                                                      -
------------------------------------------------------------------------
*/


#define NO_MSG             MIL_TEXT("")  /* empty error message          */
#define NO_SUBMSG          MIL_TEXT("")  /* empty sub-error message      */
#define NO_FCTNAME         MIL_TEXT("")  /* empty function name          */

// ActiveMIL
// ychenard - 15/08/2000
// Please use the MILOCX_COMPILATION to remove any function definition from this file
// when not compiling ActiveMIL.
// This is a hack from us to include the low-level error messages from MIL.
// If MIL cleans up the error handling some day, please contact the ActiveMIL team.
// 
//   NOTE: if you add anything else than defines in this file, please put them 
//         within a "#ifndef MILOCX_COMPILATION" block.
#ifndef MILERR
   #define MILERR(Fake1, Fake2, Fake3, Fake4, Fake5)
#endif

/* LIST OF ERROR CODES AND MESSAGES */

#define M_NULL_ERROR             0L
#define M_ERR_0_MSG              MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_1         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_2         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_3         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_4         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_5         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_6         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_7         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_8         MIL_TEXT("Null.")
#define M_ERR_0_SUBMSG_9         MIL_TEXT("Null.")

#define M_MMX_ERROR              1L
#define M_ERR_1_MSG              MIL_TEXT("MMX Error.")
#define M_ERR_1_SUBMSG_1         MIL_TEXT("Cannot force MMX usage on a non-MMX CPU.")
#define M_ERR_1_SUBMSG_2         NO_SUBMSG
#define M_ERR_1_SUBMSG_3         NO_SUBMSG
#define M_ERR_1_SUBMSG_4         NO_SUBMSG
#define M_ERR_1_SUBMSG_5         NO_SUBMSG
#define M_ERR_1_SUBMSG_6         NO_SUBMSG
#define M_ERR_1_SUBMSG_7         NO_SUBMSG
#define M_ERR_1_SUBMSG_8         NO_SUBMSG
#define M_ERR_1_SUBMSG_9         NO_SUBMSG

#define M_SSE_ERROR              2L
#define M_ERR_2_MSG              MIL_TEXT("SSE Error.")
#define M_ERR_2_SUBMSG_1         MIL_TEXT("Cannot force SSE usage on a non-SSE CPU or OS doesn't support SSE.")
#define M_ERR_2_SUBMSG_2         NO_SUBMSG
#define M_ERR_2_SUBMSG_3         NO_SUBMSG
#define M_ERR_2_SUBMSG_4         NO_SUBMSG
#define M_ERR_2_SUBMSG_5         NO_SUBMSG
#define M_ERR_2_SUBMSG_6         NO_SUBMSG
#define M_ERR_2_SUBMSG_7         NO_SUBMSG
#define M_ERR_2_SUBMSG_8         NO_SUBMSG
#define M_ERR_2_SUBMSG_9         NO_SUBMSG

#define M_SSE2_ERROR             3L
#define M_ERR_3_MSG              MIL_TEXT("SSE2 Error.")
#define M_ERR_3_SUBMSG_1         MIL_TEXT("Cannot force SSE2 usage on a non-SSE2 CPU or OS doesn't support SSE2.")
#define M_ERR_3_SUBMSG_2         NO_SUBMSG
#define M_ERR_3_SUBMSG_3         NO_SUBMSG
#define M_ERR_3_SUBMSG_4         NO_SUBMSG
#define M_ERR_3_SUBMSG_5         NO_SUBMSG
#define M_ERR_3_SUBMSG_6         NO_SUBMSG
#define M_ERR_3_SUBMSG_7         NO_SUBMSG
#define M_ERR_3_SUBMSG_8         NO_SUBMSG
#define M_ERR_3_SUBMSG_9         NO_SUBMSG

#define M_AMD_ERROR              4L
#define M_ERR_4_MSG              MIL_TEXT("AMD Error.")
#define M_ERR_4_SUBMSG_1         MIL_TEXT("Cannot force 3DNow usage on a non-3DNow CPU.")
#define M_ERR_4_SUBMSG_2         MIL_TEXT("Cannot force MMX Extension usage on a non-MMX Extenion CPU.")
#define M_ERR_4_SUBMSG_3         MIL_TEXT("Cannot force 3DNow Extension usage on a non-3DNow Extenion CPU.")
#define M_ERR_4_SUBMSG_4         NO_SUBMSG
#define M_ERR_4_SUBMSG_5         NO_SUBMSG
#define M_ERR_4_SUBMSG_6         NO_SUBMSG
#define M_ERR_4_SUBMSG_7         NO_SUBMSG
#define M_ERR_4_SUBMSG_8         NO_SUBMSG
#define M_ERR_4_SUBMSG_9         NO_SUBMSG

#define M_XX5XX_ERROR            5L
#define M_ERR_5_MSG              NO_SUBMSG
#define M_ERR_5_SUBMSG_1         NO_SUBMSG
#define M_ERR_5_SUBMSG_2         NO_SUBMSG
#define M_ERR_5_SUBMSG_3         NO_SUBMSG
#define M_ERR_5_SUBMSG_4         NO_SUBMSG
#define M_ERR_5_SUBMSG_5         NO_SUBMSG
#define M_ERR_5_SUBMSG_6         NO_SUBMSG
#define M_ERR_5_SUBMSG_7         NO_SUBMSG
#define M_ERR_5_SUBMSG_8         NO_SUBMSG
#define M_ERR_5_SUBMSG_9         NO_SUBMSG

#define M_INVALID_PARAM_ERROR    6L
#define M_ERR_6_MSG              MIL_TEXT("Invalid parameter.")
#define M_ERR_6_SUBMSG_1         MIL_TEXT("Bad parameter value.")
#define M_ERR_6_SUBMSG_2         MIL_TEXT("One parameter does not reside within the buffer's limits.")
#define M_ERR_6_SUBMSG_3         MIL_TEXT("Pointer must be non-null.")
/* WARNING: Obsolete sub errors (4,5); if you want to use these, notify ActiveMIL team */
#define M_ERR_6_SUBMSG_4         MIL_TEXT("Parameter 1 not in supported list.")
#define M_ERR_6_SUBMSG_5         MIL_TEXT("Parameter 2 not in supported list.")
#define M_ERR_6_SUBMSG_6         MIL_TEXT("No graphic text font selected.")
#define M_ERR_6_SUBMSG_7         MIL_TEXT("The member StructSize from the MILBUFFERINFO structure given as an argument is invalid.")
#define M_ERR_6_SUBMSG_8         MIL_TEXT("Result buffer too small to hold result.")
#define M_ERR_6_SUBMSG_9         MIL_TEXT("Scale factors out of supported range.")

#define M_OVERSCAN_ERROR         7L
#define M_ERR_7_MSG              MIL_TEXT("Overscan processing error.")
#define M_ERR_7_SUBMSG_1         MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_7_SUBMSG_2         MIL_TEXT("Buffer too small to perform the selected overscan")
#define M_ERR_7_SUBMSG_3         NO_SUBMSG
#define M_ERR_7_SUBMSG_4         NO_SUBMSG
#define M_ERR_7_SUBMSG_5         NO_SUBMSG
#define M_ERR_7_SUBMSG_6         NO_SUBMSG
#define M_ERR_7_SUBMSG_7         NO_SUBMSG
#define M_ERR_7_SUBMSG_8         NO_SUBMSG
#define M_ERR_7_SUBMSG_9         NO_SUBMSG

#define M_ALLOC_ERROR            8L
#define M_ERR_8_MSG              MIL_TEXT("Allocation error.")
#define M_ERR_8_SUBMSG_1         MIL_TEXT("Not enough memory to allocate application.")
#define M_ERR_8_SUBMSG_2         MIL_TEXT("Only one application can be allocated by host thread.")
#define M_ERR_8_SUBMSG_3         MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_8_SUBMSG_4         MIL_TEXT("Not enough memory to allocate buffer.")
#define M_ERR_8_SUBMSG_5         MIL_TEXT("Cannot allocate system.")
#define M_ERR_8_SUBMSG_6         MIL_TEXT("Cannot allocate digitizer.")
#define M_ERR_8_SUBMSG_7         MIL_TEXT("Cannot allocate display.")
#define M_ERR_8_SUBMSG_8         MIL_TEXT("Not enough host memory to allocate buffer.")
#define M_ERR_8_SUBMSG_9         MIL_TEXT("Buffer type not supported.")

#define M_CHILD_ERROR            9L
#define M_ERR_9_MSG              MIL_TEXT("Child allocation error.")
#define M_ERR_9_SUBMSG_1         MIL_TEXT("Only one application can be allocated by host thread.")
#define M_ERR_9_SUBMSG_2         MIL_TEXT("Not enough memory to allocate child application.")
#define M_ERR_9_SUBMSG_3         MIL_TEXT("Not enough memory to allocate child buffer.")
#define M_ERR_9_SUBMSG_4         MIL_TEXT("Cannot allocate temporary child buffer in memory.")
#define M_ERR_9_SUBMSG_5         MIL_TEXT("Impossible to make a band child of a compressed buffer.")
#define M_ERR_9_SUBMSG_6         NO_SUBMSG
#define M_ERR_9_SUBMSG_7         NO_SUBMSG
#define M_ERR_9_SUBMSG_8         NO_SUBMSG
#define M_ERR_9_SUBMSG_9         NO_SUBMSG

#define M_ACCESS_ERROR           10L
#define M_ERR_10_MSG             MIL_TEXT("Buffer access error.")
#define M_ERR_10_SUBMSG_1        MIL_TEXT("Cannot M_RESTORE a M_RAW file format buffer.")
MILERR( M_ERR,10,SUBMSG,1,       MIL_TEXT("Cannot load a raw file format buffer with the AdjustImage parameter set to True."))
#define M_ERR_10_SUBMSG_2        MIL_TEXT("Cannot export buffer.")
#define M_ERR_10_SUBMSG_3        MIL_TEXT("Source buffer must be a M_IMAGE buffer.")
MILERR( M_ERR,10,SUBMSG,3,       MIL_TEXT("Source image invalid."))
#define M_ERR_10_SUBMSG_4        MIL_TEXT("Cannot import buffer.")
MILERR( M_ERR,10,SUBMSG,4,       MIL_TEXT("Cannot load image."))
#define M_ERR_10_SUBMSG_5        MIL_TEXT("Cannot restore buffer.")
MILERR( M_ERR,10,SUBMSG,5,       MIL_TEXT("Cannot load image."))
#define M_ERR_10_SUBMSG_6        MIL_TEXT("Cannot export child buffer in M_PLANAR format")
MILERR( M_ERR,10,SUBMSG,6,       MIL_TEXT("Cannot save child image in planar format"))
#define M_ERR_10_SUBMSG_7        NO_SUBMSG
#define M_ERR_10_SUBMSG_8        NO_SUBMSG
#define M_ERR_10_SUBMSG_9        NO_SUBMSG

#define M_DISPLAY_ERROR          11L
#define M_ERR_11_MSG             MIL_TEXT("Display error.")
#define M_ERR_11_SUBMSG_1        MIL_TEXT("Display and buffer must belong to same system.")
#define M_ERR_11_SUBMSG_2        MIL_TEXT("Display Lut dimensions are not compatible with the user Lut.")
#define M_ERR_11_SUBMSG_3        MIL_TEXT("Cannot associate a M_PSEUDO Lut with a monochrome display.")
MILERR( M_ERR,11,SUBMSG,3,       MIL_TEXT("Cannot associate a pseudo LUT with a monochrome display."))
#define M_ERR_11_SUBMSG_4        MIL_TEXT("Zoom factors out of zoom limits or equal to zero.")
#define M_ERR_11_SUBMSG_5        MIL_TEXT("Buffer not currently selected on display.")
MILERR( M_ERR,11,SUBMSG,5,       MIL_TEXT("Buffer not currently associated with a display."))
#define M_ERR_11_SUBMSG_6        MIL_TEXT("Incompatible display type.")
#define M_ERR_11_SUBMSG_7        MIL_TEXT("Display must be allocated with the M_WINDOWED init flag.")
MILERR( M_ERR,11,SUBMSG,7,       MIL_TEXT("The DisplayType must be set to dispDefaultWindow, dispUserWindow, or dispExternalWindow."))
#define M_ERR_11_SUBMSG_8        MIL_TEXT("Invalid window handle.")
#define M_ERR_11_SUBMSG_9        NO_SUBMSG

#define M_OPERATION_ERROR        12L
#define M_ERR_12_MSG             MIL_TEXT("Operation error.")
#define M_ERR_12_SUBMSG_1        MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_12_SUBMSG_2        MIL_TEXT("Not enough host memory to allocate buffer.")
#define M_ERR_12_SUBMSG_3        MIL_TEXT("Application still has MIL objects associated with it.")
#define M_ERR_12_SUBMSG_4        MIL_TEXT("Only logical addresses are supported on host system.")
#define M_ERR_12_SUBMSG_5        MIL_TEXT("Pitch must be a multiple of 4 bytes for binary buffers.")
#define M_ERR_12_SUBMSG_6        MIL_TEXT("Requested operation not supported.") 
#define M_ERR_12_SUBMSG_7        MIL_TEXT("Pitch must be a multiple of 2 pixels.")
#define M_ERR_12_SUBMSG_8        MIL_TEXT("Creation with physical address must be done on non paged buffers.")
#define M_ERR_12_SUBMSG_9        MIL_TEXT("The polar band to put must fit in the destination.")

#define M_DIGITIZER_ERROR        13L
#define M_ERR_13_MSG             MIL_TEXT("Digitizer error.")
#define M_ERR_13_SUBMSG_1        MIL_TEXT("Digitizer and buffer must belong to same system.")
#define M_ERR_13_SUBMSG_2        NO_SUBMSG
#define M_ERR_13_SUBMSG_3        NO_SUBMSG
#define M_ERR_13_SUBMSG_4        NO_SUBMSG
#define M_ERR_13_SUBMSG_5        NO_SUBMSG
#define M_ERR_13_SUBMSG_6        NO_SUBMSG
#define M_ERR_13_SUBMSG_7        NO_SUBMSG
#define M_ERR_13_SUBMSG_8        NO_SUBMSG
#define M_ERR_13_SUBMSG_9        NO_SUBMSG

#define M_HOOK_ERROR             14L
#define M_ERR_14_MSG             MIL_TEXT("Hook function error")
#define M_ERR_14_SUBMSG_1        MIL_TEXT("Unhooking by hooking the previous hook function is obsolete.  Please see documentation.")
#define M_ERR_14_SUBMSG_2        MIL_TEXT("A hook function must be provided.")
#define M_ERR_14_SUBMSG_3        MIL_TEXT("MxxxHookFunction functions should not be called within user hook function.")
#define M_ERR_14_SUBMSG_4        MIL_TEXT("Invalid hook type .")
#define M_ERR_14_SUBMSG_5        MIL_TEXT("Required hardware not present .")
#define M_ERR_14_SUBMSG_6        MIL_TEXT("Invalid system type .")
#define M_ERR_14_SUBMSG_7        MIL_TEXT("A function cannot be unhooked if it has not been hooked previously")
#define M_ERR_14_SUBMSG_8        NO_SUBMSG
#define M_ERR_14_SUBMSG_9        NO_SUBMSG

#define M_JPEG_COMPRESS_ERROR    15L
#define M_ERR_15_MSG             MIL_TEXT("JPEG compression error.")
#define M_ERR_15_SUBMSG_1        MIL_TEXT("Unable to allocate memory.")
#define M_ERR_15_SUBMSG_2        MIL_TEXT("Invalid image depth for JPEG compression.")
#define M_ERR_15_SUBMSG_3        MIL_TEXT("Invalid compression parameter.")
#define M_ERR_15_SUBMSG_4        MIL_TEXT("Invalid Huffman table")
#define M_ERR_15_SUBMSG_5        MIL_TEXT("Invalid predictor.")
#define M_ERR_15_SUBMSG_6        MIL_TEXT("Invalid Q Factor.")
#define M_ERR_15_SUBMSG_7        MIL_TEXT("Invalid quantization table.")
#define M_ERR_15_SUBMSG_8        MIL_TEXT("Invalid restart interval.")
#define M_ERR_15_SUBMSG_9        MIL_TEXT("Invalid source or destination format.")

#define M_COMPRESS_ERROR         16L
#define M_ERR_16_MSG             MIL_TEXT("Compression Error")
#define M_ERR_16_SUBMSG_1        MIL_TEXT("Invalid compression type.")
#define M_ERR_16_SUBMSG_2        MIL_TEXT("Corrupted data.")
#define M_ERR_16_SUBMSG_3        MIL_TEXT("Invalid image depth for required compression type.")
#define M_ERR_16_SUBMSG_4        MIL_TEXT("Buffer allocated with MbufCreateXX cannot be the destination of a compression operation.")
MILERR( M_ERR,16,SUBMSG,4,       MIL_TEXT("Image allocated with AssignMemory cannot be the destination of a compression operation."))
#define M_ERR_16_SUBMSG_5        MIL_TEXT("Buffer allocated with MbufCreateXX cannot be the destination of a MbufPut operation.")
MILERR( M_ERR,16,SUBMSG,5,       MIL_TEXT("Image allocated with AssignMemory cannot be the destination of a Put method."))
#define M_ERR_16_SUBMSG_6        NO_SUBMSG
#define M_ERR_16_SUBMSG_7        NO_SUBMSG
#define M_ERR_16_SUBMSG_8        NO_SUBMSG
#define M_ERR_16_SUBMSG_9        NO_SUBMSG

#define M_JPEG_COMPRESS_ERROR_2  17L
#define M_ERR_17_MSG             MIL_TEXT("JPEG compression error.")
#define M_ERR_17_SUBMSG_1        MIL_TEXT("Chrominance tables are allowed only with 3 band YUV buffers.")
#define M_ERR_17_SUBMSG_2        MIL_TEXT("Luminance tables are allowed only with 3 band YUV buffers.")
#define M_ERR_17_SUBMSG_3        MIL_TEXT("The predictor value is only available for 1 band buffers or band childs.")
#define M_ERR_17_SUBMSG_4        MIL_TEXT("The Q factor is only available for 1 band buffers or band childs.")
#define M_ERR_17_SUBMSG_5        MIL_TEXT("The restart interval is only available for 1 band buffers or band childs.")
#define M_ERR_17_SUBMSG_6        MIL_TEXT("The source image must be a multiple of 16 in X and a multiple of 8 in Y.")
#define M_ERR_17_SUBMSG_7        MIL_TEXT("The destination image must be a multiple of 16 in X and a multiple of 8 in Y.")
#define M_ERR_17_SUBMSG_8        MIL_TEXT("The source image must be a multiple of 16 in X and a multiple of 16 in Y.")
#define M_ERR_17_SUBMSG_9        MIL_TEXT("The destination image must be a multiple of 16 in X and a multiple of 16 in Y.")


#define M_BMP_ERROR              18L
#define M_ERR_18_MSG             MIL_TEXT("BMP handler file access error.")
#define M_ERR_18_SUBMSG_1        MIL_TEXT("Not a bitmap file.")
#define M_ERR_18_SUBMSG_2        MIL_TEXT("Error closing bitmap file.")
#define M_ERR_18_SUBMSG_3        MIL_TEXT("Cannot open file in read mode.")
#define M_ERR_18_SUBMSG_4        MIL_TEXT("Error reading file.")
#define M_ERR_18_SUBMSG_5        MIL_TEXT("Unable to position file pointer.")
#define M_ERR_18_SUBMSG_6        MIL_TEXT("Cannot create or open file in write mode.")
#define M_ERR_18_SUBMSG_7        MIL_TEXT("No bitmap file opened in read mode.")
#define M_ERR_18_SUBMSG_8        MIL_TEXT("No bitmap file opened in write mode.")
#define M_ERR_18_SUBMSG_9        MIL_TEXT("Error writing file.")

#define M_BMP_ERROR_2            19L
#define M_ERR_19_MSG             MIL_TEXT("BMP handler general error.")
#define M_ERR_19_SUBMSG_1        MIL_TEXT("Unable to allocate sufficient memory.")
#define M_ERR_19_SUBMSG_2        MIL_TEXT("Not supported color format.")
#define M_ERR_19_SUBMSG_3        MIL_TEXT("Invalid write color format specified.")
#define M_ERR_19_SUBMSG_4        MIL_TEXT("Invalid write compression type specified.")
#define M_ERR_19_SUBMSG_5        MIL_TEXT("Not supported type of conversion.")
#define M_ERR_19_SUBMSG_6        MIL_TEXT("Invalid array format specified.")
#define M_ERR_19_SUBMSG_7        MIL_TEXT("Invalid length of palette arrays specified.")
#define M_ERR_19_SUBMSG_8        MIL_TEXT("No palette to read.")
#define M_ERR_19_SUBMSG_9        MIL_TEXT("Palette not needed for current write format.")


#define M_TIFF_ERROR             20L
#define M_ERR_20_MSG             MIL_TEXT("TIFF file access error.")
#define M_ERR_20_SUBMSG_1        MIL_TEXT("Cannot open file.")
#define M_ERR_20_SUBMSG_2        MIL_TEXT("Cannot close file.")
#define M_ERR_20_SUBMSG_3        MIL_TEXT("Cannot read file.")
#define M_ERR_20_SUBMSG_4        MIL_TEXT("Cannot read file.")
#define M_ERR_20_SUBMSG_5        MIL_TEXT("Cannot write to file.")
#define M_ERR_20_SUBMSG_6        MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_20_SUBMSG_7        MIL_TEXT("The image file does not conform to the TIFF 6.0 specification.")
#define M_ERR_20_SUBMSG_8        MIL_TEXT("Wrong Byte Order, Only INTEL Byte Ordering supported.")
#define M_ERR_20_SUBMSG_9        MIL_TEXT("Not a TIFF file.")

#define M_MIL_FILE_ERROR         21L
#define M_ERR_21_MSG             MIL_TEXT("MIL file access error.")
#define M_ERR_21_SUBMSG_1        MIL_TEXT("Cannot open file.")
#define M_ERR_21_SUBMSG_2        MIL_TEXT("Cannot close file.")
#define M_ERR_21_SUBMSG_3        MIL_TEXT("Cannot read file.")
#define M_ERR_21_SUBMSG_4        MIL_TEXT("Cannot read file.")
#define M_ERR_21_SUBMSG_5        MIL_TEXT("Cannot write to file.")
#define M_ERR_21_SUBMSG_6        MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_21_SUBMSG_7        MIL_TEXT("The image file does not conform to the TIFF 6.0 specification.")
#define M_ERR_21_SUBMSG_8        MIL_TEXT("Wrong Byte Order, Only INTEL Byte Ordering supported.")
#define M_ERR_21_SUBMSG_9        MIL_TEXT("Not a MIL file.")

#define M_MIL_FILE_ERROR_2       22L
#define M_ERR_22_MSG             MIL_TEXT("MIL file access error.")
#define M_ERR_22_SUBMSG_1        MIL_TEXT("Only 8, 16 or 32 BitsPerSample supported.")
#define M_ERR_22_SUBMSG_2        MIL_TEXT("Only the Lempel-Ziv & Welch (LZW) compression supported.")
#define M_ERR_22_SUBMSG_3        MIL_TEXT("PhotometricInterp incompatible with SamplePerPixel.")
#define M_ERR_22_SUBMSG_4        MIL_TEXT("Only PlanarConfiguration 2 supported for multi-band images.")
#define M_ERR_22_SUBMSG_5        MIL_TEXT("Up to 8 Samples Per Pixel supported.")
#define M_ERR_22_SUBMSG_6        MIL_TEXT("Only identical BitsPerSample for every sample supported.")
#define M_ERR_22_SUBMSG_7        MIL_TEXT("Cannot seek in file.")
#define M_ERR_22_SUBMSG_8        MIL_TEXT("Bad file format detected.")
#define M_ERR_22_SUBMSG_9        MIL_TEXT("Invalid info requested.")

#define M_MIL_FILE_ERROR_3       23L
#define M_ERR_23_MSG             MIL_TEXT("MIL file access error.")
#define M_ERR_23_SUBMSG_1        MIL_TEXT("Invalid parameter detected.")
#define M_ERR_23_SUBMSG_2        MIL_TEXT("Missing information in file.")
#define M_ERR_23_SUBMSG_3        MIL_TEXT("Bad size information detected in file.")
#define M_ERR_23_SUBMSG_4        MIL_TEXT("Bad identification information detected in file.")
#define M_ERR_23_SUBMSG_5        MIL_TEXT("Invalid data information detected in file.")
#define M_ERR_23_SUBMSG_6        MIL_TEXT("Unexpected internal error.")
#define M_ERR_23_SUBMSG_7        MIL_TEXT("Cannot open file: Invalid file name.")
#define M_ERR_23_SUBMSG_8        NO_SUBMSG
#define M_ERR_23_SUBMSG_9        NO_SUBMSG

#define M_MULTI_THREAD_ERROR     24L
#define M_ERR_24_MSG             MIL_TEXT("Multi thread error.")
#define M_ERR_24_SUBMSG_1        MIL_TEXT("Cannot find synchronization event to delete.")
#define M_ERR_24_SUBMSG_2        MIL_TEXT("Invalid MIL_ID, MIL thread or event was not allocated in current HOST thread.")
MILERR( M_ERR,24,SUBMSG,2,       MIL_TEXT("Invalid thread or event ID; it was not allocated in current host thread."))
#define M_ERR_24_SUBMSG_3        MIL_TEXT("Application must be freed before exiting a thread.")
#define M_ERR_24_SUBMSG_4        NO_SUBMSG
#define M_ERR_24_SUBMSG_5        NO_SUBMSG
#define M_ERR_24_SUBMSG_6        NO_SUBMSG
#define M_ERR_24_SUBMSG_7        NO_SUBMSG
#define M_ERR_24_SUBMSG_8        NO_SUBMSG
#define M_ERR_24_SUBMSG_9        NO_SUBMSG

#define M_JPEG_ERROR             25L
#define M_ERR_25_MSG             MIL_TEXT("JPEG handler general error.")
#define M_ERR_25_SUBMSG_1        MIL_TEXT("Not supported color format.")
#define M_ERR_25_SUBMSG_2        MIL_TEXT("Error closing JPEG file.")
#define M_ERR_25_SUBMSG_3        MIL_TEXT("Error opening JPEG file.")
#define M_ERR_25_SUBMSG_4        MIL_TEXT("Unable to allocate sufficient memory.")
#define M_ERR_25_SUBMSG_5        MIL_TEXT("Image too large to save.")
#define M_ERR_25_SUBMSG_6        MIL_TEXT("Invalid JPEG Marker.")
#define M_ERR_25_SUBMSG_7        MIL_TEXT("Bad identification information detected in file.")
#define M_ERR_25_SUBMSG_8        MIL_TEXT("Buffer to receive data does not fit with data.")
#define M_ERR_25_SUBMSG_9        MIL_TEXT("Error reading JPEG file.")

#define M_BUFFER_FREE_ERROR      26L
#define M_ERR_26_MSG             MIL_TEXT("Buffer free operation error.")
#define M_ERR_26_SUBMSG_1        MIL_TEXT("Buffer still has child(ren) associated with it.")
#define M_ERR_26_SUBMSG_2        MIL_TEXT("Use MnatBufDestroy() on this kind of buffer.")
#define M_ERR_26_SUBMSG_3        MIL_TEXT("User attempting to free a system allocated buffer.")
#define M_ERR_26_SUBMSG_4        MIL_TEXT("Internal error, internal function attempting to free a user allocated buffer.")
#define M_ERR_26_SUBMSG_5        NO_SUBMSG
#define M_ERR_26_SUBMSG_6        NO_SUBMSG
#define M_ERR_26_SUBMSG_7        NO_SUBMSG
#define M_ERR_26_SUBMSG_8        NO_SUBMSG
#define M_ERR_26_SUBMSG_9        NO_SUBMSG

#define M_SYSTEM_FREE_ERROR      27L
#define M_ERR_27_MSG             MIL_TEXT("System free operation error.")
#define M_ERR_27_SUBMSG_1        MIL_TEXT("System still has buffer(s) associated with it.")
#define M_ERR_27_SUBMSG_2        MIL_TEXT("System still has display(s) associated with it.")
#define M_ERR_27_SUBMSG_3        MIL_TEXT("System still has digitizer(s) associated with it.")
#define M_ERR_27_SUBMSG_4        NO_SUBMSG
#define M_ERR_27_SUBMSG_5        NO_SUBMSG
#define M_ERR_27_SUBMSG_6        NO_SUBMSG
#define M_ERR_27_SUBMSG_7        NO_SUBMSG
#define M_ERR_27_SUBMSG_8        NO_SUBMSG
#define M_ERR_27_SUBMSG_9        NO_SUBMSG

#define M_FUNCTION_START_ERROR   28L
#define M_ERR_28_MSG             MIL_TEXT("Function start error.")
#define M_ERR_28_SUBMSG_1        MIL_TEXT("No application allocated.")
#define M_ERR_28_SUBMSG_2        NO_SUBMSG
#define M_ERR_28_SUBMSG_3        NO_SUBMSG
#define M_ERR_28_SUBMSG_4        NO_SUBMSG
#define M_ERR_28_SUBMSG_5        NO_SUBMSG
#define M_ERR_28_SUBMSG_6        NO_SUBMSG
#define M_ERR_28_SUBMSG_7        NO_SUBMSG
#define M_ERR_28_SUBMSG_8        NO_SUBMSG
#define M_ERR_28_SUBMSG_9        NO_SUBMSG

#define M_COMMAND_DECODER_ERROR  29L
#define M_ERR_29_MSG             MIL_TEXT("System command error.")
#define M_ERR_29_SUBMSG_1        MIL_TEXT("Requested operation not supported.")
#define M_ERR_29_SUBMSG_2        MIL_TEXT("Operation execution failed.")
#define M_ERR_29_SUBMSG_3        NO_SUBMSG
#define M_ERR_29_SUBMSG_4        NO_SUBMSG
#define M_ERR_29_SUBMSG_5        NO_SUBMSG
#define M_ERR_29_SUBMSG_6        NO_SUBMSG
#define M_ERR_29_SUBMSG_7        NO_SUBMSG
#define M_ERR_29_SUBMSG_8        NO_SUBMSG
#define M_ERR_29_SUBMSG_9        NO_SUBMSG

#define M_LABELLING_ERROR        30L
#define M_ERR_30_MSG             MIL_TEXT("Labelling error.")
#define M_ERR_30_SUBMSG_1        MIL_TEXT("Maximum number of labels reached.")
#define M_ERR_30_SUBMSG_2        MIL_TEXT("Should use a buffer of greater bit depth.")
#define M_ERR_30_SUBMSG_3        NO_SUBMSG
#define M_ERR_30_SUBMSG_4        NO_SUBMSG
#define M_ERR_30_SUBMSG_5        NO_SUBMSG
#define M_ERR_30_SUBMSG_6        NO_SUBMSG
#define M_ERR_30_SUBMSG_7        NO_SUBMSG
#define M_ERR_30_SUBMSG_8        NO_SUBMSG
#define M_ERR_30_SUBMSG_9        NO_SUBMSG

#define M_FILE_ERROR             31L
#define M_ERR_31_MSG             MIL_TEXT("File access error.")
#define M_ERR_31_SUBMSG_1        MIL_TEXT("Cannot open output file.")
#define M_ERR_31_SUBMSG_2        MIL_TEXT("Cannot write to file.")
#define M_ERR_31_SUBMSG_3        MIL_TEXT("Cannot open input file.")
#define M_ERR_31_SUBMSG_4        MIL_TEXT("Cannot read file.")
#define M_ERR_31_SUBMSG_5        MIL_TEXT("Cannot close output file.")
#define M_ERR_31_SUBMSG_6        MIL_TEXT("Cannot close input file.")
#define M_ERR_31_SUBMSG_7        MIL_TEXT("The FileFormatBufId does not represent the actual file format.")
#define M_ERR_31_SUBMSG_8        NO_SUBMSG
#define M_ERR_31_SUBMSG_9        MIL_TEXT("Not a MIL file.")

#define M_APP_FREE_ERROR         32L
#define M_ERR_32_MSG             MIL_TEXT("Application free operation error.")
#define M_ERR_32_SUBMSG_1        MIL_TEXT("Application still has system(s) associated with it.")
#define M_ERR_32_SUBMSG_2        MIL_TEXT("Default host system still has buffer(s) associated with it.")
#define M_ERR_32_SUBMSG_3        MIL_TEXT("Application still has child(ren) associated with it.")
#define M_ERR_32_SUBMSG_4        MIL_TEXT("Application was not freed.")
#define M_ERR_32_SUBMSG_5        MIL_TEXT("Application still has object(s) associated with it.")
#define M_ERR_32_SUBMSG_6        NO_SUBMSG
#define M_ERR_32_SUBMSG_7        NO_SUBMSG
#define M_ERR_32_SUBMSG_8        NO_SUBMSG
#define M_ERR_32_SUBMSG_9        NO_SUBMSG

#define M_TIFF_ERROR_2           33L
#define M_ERR_33_MSG             MIL_TEXT("TIFF File access error.")
#define M_ERR_33_SUBMSG_1        MIL_TEXT("Only 1, 8, 16 or 32 BitsPerSample supported.")
#define M_ERR_33_SUBMSG_2        MIL_TEXT("Only the Lempel-Ziv & Welch (LZW) compression supported.")
#define M_ERR_33_SUBMSG_3        MIL_TEXT("PhotometricInterp incompatible with SamplePerPixel.")
#define M_ERR_33_SUBMSG_4        MIL_TEXT("Only PlanarConfiguration 2 supported for multi-band images.")
#define M_ERR_33_SUBMSG_5        MIL_TEXT("Up to 8 Samples Per Pixel supported.")
#define M_ERR_33_SUBMSG_6        MIL_TEXT("Only identical BitsPerSample for every sample supported.")
#define M_ERR_33_SUBMSG_7        MIL_TEXT("Cannot seek in file.")
#define M_ERR_33_SUBMSG_8        MIL_TEXT("Bad file format detected.")
#define M_ERR_33_SUBMSG_9        MIL_TEXT("Invalid info requested.")

#define M_PROCESSING_ERROR       34L
#define M_ERR_34_MSG             MIL_TEXT("Processing error.")
#define M_ERR_34_SUBMSG_1        MIL_TEXT("All buffers do not have the same working system.")
#define M_ERR_34_SUBMSG_2        MIL_TEXT("Cannot find any working system between buffers.")
#define M_ERR_34_SUBMSG_3        MIL_TEXT("Cannot process a HOST buffer as a whole and a temporary buffer.")
#define M_ERR_34_SUBMSG_4        MIL_TEXT("Source buffers cannot overlap destination buffer.")
#define M_ERR_34_SUBMSG_5        MIL_TEXT("No processor on target processing system.")
#define M_ERR_34_SUBMSG_6        NO_SUBMSG
#define M_ERR_34_SUBMSG_7        NO_SUBMSG
#define M_ERR_34_SUBMSG_8        NO_SUBMSG
#define M_ERR_34_SUBMSG_9        MIL_TEXT("Not enough memory or system limitation, cannot process buffer.")

#define M_INVALID_ID             35L
#define M_ERR_35_MSG             MIL_TEXT("Invalid MIL ID.")
#define M_ERR_35_SUBMSG_1        MIL_TEXT("Invalid parameter 1.")
#define M_ERR_35_SUBMSG_2        MIL_TEXT("Invalid parameter 2.")
#define M_ERR_35_SUBMSG_3        MIL_TEXT("Invalid parameter 3.")
#define M_ERR_35_SUBMSG_4        MIL_TEXT("Invalid parameter 4.")
#define M_ERR_35_SUBMSG_5        MIL_TEXT("Invalid parameter 5.")
#define M_ERR_35_SUBMSG_6        MIL_TEXT("Invalid parameter 6.")
#define M_ERR_35_SUBMSG_7        MIL_TEXT("Invalid parameter 7.")
#define M_ERR_35_SUBMSG_8        MIL_TEXT("Invalid parameter 8.")
#define M_ERR_35_SUBMSG_9        MIL_TEXT("Invalid parameter 9.")

#define M_INVALID_NATURE         36L
#define M_ERR_36_MSG             MIL_TEXT("Inappropriate MIL ID.")
#define M_ERR_36_SUBMSG_1        MIL_TEXT("Invalid parameter 1.")
#define M_ERR_36_SUBMSG_2        MIL_TEXT("Invalid parameter 2.")
#define M_ERR_36_SUBMSG_3        MIL_TEXT("Invalid parameter 3.")
#define M_ERR_36_SUBMSG_4        MIL_TEXT("Invalid parameter 4.")
#define M_ERR_36_SUBMSG_5        MIL_TEXT("Invalid parameter 5.")
#define M_ERR_36_SUBMSG_6        MIL_TEXT("Invalid parameter 6.")
#define M_ERR_36_SUBMSG_7        MIL_TEXT("Invalid parameter 7.")
#define M_ERR_36_SUBMSG_8        MIL_TEXT("Invalid parameter 8.")
#define M_ERR_36_SUBMSG_9        MIL_TEXT("Invalid parameter 9.")

#define M_INVALID_PARAM_ERROR_2  37L
#define M_ERR_37_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_37_SUBMSG_1        MIL_TEXT("For this operation, you should supply a LUT buffer with at least 512 entries.")
#define M_ERR_37_SUBMSG_2        MIL_TEXT("For this operation the grab mode must be asynchronous.")
#define M_ERR_37_SUBMSG_3        MIL_TEXT("This type of conversion requires two 3 band buffers.")
#define M_ERR_37_SUBMSG_4        MIL_TEXT("This type of conversion requires a 3 band source buffer.")
#define M_ERR_37_SUBMSG_5        MIL_TEXT("This type of conversion requires a 3 band destination buffer.")
#define M_ERR_37_SUBMSG_6        MIL_TEXT("Invalid interpolation type specified.")
#define M_ERR_37_SUBMSG_7        MIL_TEXT("Specified center is outside buffer.")
#define M_ERR_37_SUBMSG_8        MIL_TEXT("Image buffer must be 8 bit monochrome for this operation.")
MILERR( M_ERR,37,SUBMSG,8,       MIL_TEXT("Image must be 8 bit monochrome for this operation."))
#define M_ERR_37_SUBMSG_9        MIL_TEXT("Look up table must be 1 or 3 bands, 8 bits deep and have an X size of 256 or more.")

#define M_INVALID_ATTRIBUTE      38L
#define M_ERR_38_MSG             MIL_TEXT("Invalid attributes.")
#define M_ERR_38_SUBMSG_1        MIL_TEXT("Invalid parameter 1.")
#define M_ERR_38_SUBMSG_2        MIL_TEXT("Invalid parameter 2.")
#define M_ERR_38_SUBMSG_3        MIL_TEXT("Invalid parameter 3.")
#define M_ERR_38_SUBMSG_4        MIL_TEXT("Invalid parameter 4.")
#define M_ERR_38_SUBMSG_5        MIL_TEXT("Invalid parameter 5.")
#define M_ERR_38_SUBMSG_6        MIL_TEXT("Invalid parameter 6.")
#define M_ERR_38_SUBMSG_7        MIL_TEXT("Invalid parameter 7.")
#define M_ERR_38_SUBMSG_8        MIL_TEXT("Invalid parameter 8.")
#define M_ERR_38_SUBMSG_9        MIL_TEXT("Invalid parameter 9.")

#define M_CALL_CONTEXT_ERROR     39L
#define M_ERR_39_MSG             MIL_TEXT("Call context error")
#define M_ERR_39_SUBMSG_1        MIL_TEXT("Cannot allocate temporary buffer in memory.")
#define M_ERR_39_SUBMSG_2        NO_SUBMSG
#define M_ERR_39_SUBMSG_3        NO_SUBMSG
#define M_ERR_39_SUBMSG_4        NO_SUBMSG
#define M_ERR_39_SUBMSG_5        NO_SUBMSG
#define M_ERR_39_SUBMSG_6        NO_SUBMSG
#define M_ERR_39_SUBMSG_7        NO_SUBMSG
#define M_ERR_39_SUBMSG_8        NO_SUBMSG
#define M_ERR_39_SUBMSG_9        NO_SUBMSG

#define M_DRIVER_OBSOLETE        40L
#define M_ERR_40_MSG             MIL_TEXT("MIL driver obsolete.")
#define M_ERR_40_SUBMSG_1        MIL_TEXT("Version must be 7.0 or higher.")
#define M_ERR_40_SUBMSG_2        NO_SUBMSG
#define M_ERR_40_SUBMSG_3        NO_SUBMSG
#define M_ERR_40_SUBMSG_4        NO_SUBMSG
#define M_ERR_40_SUBMSG_5        NO_SUBMSG
#define M_ERR_40_SUBMSG_6        NO_SUBMSG
#define M_ERR_40_SUBMSG_7        NO_SUBMSG
#define M_ERR_40_SUBMSG_8        NO_SUBMSG
#define M_ERR_40_SUBMSG_9        NO_SUBMSG

#define M_INVALID_PARAM_ERROR_3  41L
#define M_ERR_41_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_41_SUBMSG_1        MIL_TEXT("Parameter 1 not in supported list.")
#define M_ERR_41_SUBMSG_2        MIL_TEXT("Parameter 2 not in supported list.")
#define M_ERR_41_SUBMSG_3        MIL_TEXT("Parameter 3 not in supported list.")
#define M_ERR_41_SUBMSG_4        MIL_TEXT("Parameter 4 not in supported list.")
#define M_ERR_41_SUBMSG_5        MIL_TEXT("Parameter 5 not in supported list.")
#define M_ERR_41_SUBMSG_6        MIL_TEXT("Parameter 6 not in supported list.")
#define M_ERR_41_SUBMSG_7        MIL_TEXT("Parameter 7 not in supported list.")
#define M_ERR_41_SUBMSG_8        MIL_TEXT("Parameter 8 not in supported list.")
#define M_ERR_41_SUBMSG_9        MIL_TEXT("Parameter 9 not in supported list.")

#define M_ALLOC_ERROR_2          42L
#define M_ERR_42_MSG             MIL_TEXT("Allocation error.")
#define M_ERR_42_SUBMSG_1        MIL_TEXT("Not enough host memory to do operation.")
#define M_ERR_42_SUBMSG_2        MIL_TEXT("Invalid attribute, M_GRAB not supported on host.")
MILERR( M_ERR,42,SUBMSG,2,       MIL_TEXT("The CanGrab property is not supported on host."))
#define M_ERR_42_SUBMSG_3        MIL_TEXT("Incompatible buffer dimensions, SizeBand, SizeY and (SizeX x SizeBit) must be identical.")
MILERR( M_ERR,42,SUBMSG,3,       MIL_TEXT("Incompatible buffer dimensions, NumberOfBands, SizeY and (SizeX x DataDepth) must be identical."))
#define M_ERR_42_SUBMSG_4        MIL_TEXT("Unable to communicate with MIL MtxDMA service.")
#define M_ERR_42_SUBMSG_5        MIL_TEXT("The size of the memory allocated is zero.")
#define M_ERR_42_SUBMSG_6        MIL_TEXT("Impossible to make a band child of a compressed buffer.")
#define M_ERR_42_SUBMSG_7        MIL_TEXT("Cannot create a host buffer with a physical address on DOS platforms.")
#define M_ERR_42_SUBMSG_8        MIL_TEXT("Cannot create a host buffer with a physical address on Windows 9x platforms.")
#define M_ERR_42_SUBMSG_9        MIL_TEXT("Cannot allocate a host buffer in the non paged pool on DOS platforms.")

#define M_TIMER_ERROR            43L
#define M_ERR_43_MSG             MIL_TEXT("Timer error.")
#define M_ERR_43_SUBMSG_1        MIL_TEXT("Invalid timer mode specified.")
#define M_ERR_43_SUBMSG_2        MIL_TEXT("TimePtr parameter cannot be null.")
#define M_ERR_43_SUBMSG_3        MIL_TEXT("Installed hardware does not supports a high-resolution performance counter.")
#define M_ERR_43_SUBMSG_4        MIL_TEXT("Timer must be resetted prior to reading it.")
#define M_ERR_43_SUBMSG_5        NO_SUBMSG
#define M_ERR_43_SUBMSG_6        NO_SUBMSG
#define M_ERR_43_SUBMSG_7        NO_SUBMSG
#define M_ERR_43_SUBMSG_8        NO_SUBMSG
#define M_ERR_43_SUBMSG_9        NO_SUBMSG

#define M_INVALID_PARAM_ERROR_4  44L
#define M_ERR_44_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_44_SUBMSG_1        MIL_TEXT("Valid data formats for binary buffers are M_SINGLE_BAND and M_PLANAR.")
MILERR( M_ERR,44,SUBMSG,1,       MIL_TEXT("Valid formats for binary images are single band and Planar."))
#define M_ERR_44_SUBMSG_2        MIL_TEXT("Operation not supported on float buffer.")
#define M_ERR_44_SUBMSG_3        MIL_TEXT("Destination buffer is binary, MaskValue must be equal to 0 or 1.")
#define M_ERR_44_SUBMSG_4        MIL_TEXT("Image buffer must be monochrome for this operation.")
MILERR( M_ERR,44,SUBMSG,4,       MIL_TEXT("Image must be monochrome for this operation."))
#define M_ERR_44_SUBMSG_5        MIL_TEXT("Source buffers must be of the same type")
#define M_ERR_44_SUBMSG_6        MIL_TEXT("Integer source buffers must be unsigned")
#define M_ERR_44_SUBMSG_7        MIL_TEXT("Operation not supported on binary buffer.")
#define M_ERR_44_SUBMSG_8        MIL_TEXT("Source buffers must be of the same pixel depth.")
#define M_ERR_44_SUBMSG_9        MIL_TEXT("The width and height of the source and destination buffers must be a power of 2.")

#define M_INVALID_PARAM_ERROR_5  45L
#define M_ERR_45_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_45_SUBMSG_1        MIL_TEXT("The number of bands is not valid.")
#define M_ERR_45_SUBMSG_2        MIL_TEXT("The X dimension is not valid.")
#define M_ERR_45_SUBMSG_3        MIL_TEXT("The Y dimension is not valid.")
#define M_ERR_45_SUBMSG_4        MIL_TEXT("The pixel depth is not valid.")
#define M_ERR_45_SUBMSG_5        MIL_TEXT("The buffer attributes are not valid.")
#define M_ERR_45_SUBMSG_6        MIL_TEXT("The data type is not valid.")
#define M_ERR_45_SUBMSG_7        MIL_TEXT("Invalid FFT type specified.")
#define M_ERR_45_SUBMSG_8        MIL_TEXT("Invalid operation mode specified.")
#define M_ERR_45_SUBMSG_9        MIL_TEXT("Invalid transform specified.")

#define M_INVALID_PARAM_ERROR_6  46L
#define M_ERR_46_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_46_SUBMSG_1        MIL_TEXT("Operation not supported on 32 bits buffer.")
#define M_ERR_46_SUBMSG_2        MIL_TEXT("Invalid minimal variation specified.")
#define M_ERR_46_SUBMSG_3        MIL_TEXT("Invalid combination of control flag.")
#define M_ERR_46_SUBMSG_4        MIL_TEXT("A marker image or a minimal variation must be supplied.")
#define M_ERR_46_SUBMSG_5        MIL_TEXT("Value out of range.")
#define M_ERR_46_SUBMSG_6        MIL_TEXT("Invalid mode specified.")
#define M_ERR_46_SUBMSG_7        MIL_TEXT("The pointer should not be null.")
#define M_ERR_46_SUBMSG_8        MIL_TEXT("The buffer must be a M_IMAGE + M_COMPRESS buffer.")
MILERR( M_ERR,46,SUBMSG,8,       MIL_TEXT("The image must be a compress image. CompressionType property no equal to imUncompressed."))
#define M_ERR_46_SUBMSG_9        MIL_TEXT("Invalid compression type.")

#define M_INVALID_PARAM_ERROR_7  47L
#define M_ERR_47_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_47_SUBMSG_1        MIL_TEXT("Cannot change compression type of a single band.")
#define M_ERR_47_SUBMSG_2        MIL_TEXT("Invalid ControlFlag value.")
MILERR( M_ERR,47,SUBMSG,2,       MIL_TEXT("Invalid constant specified."))
#define M_ERR_47_SUBMSG_3        MIL_TEXT("The MIL_INT64 pointer can not be null.")
#define M_ERR_47_SUBMSG_4        MIL_TEXT("Invalid source buffers.")
#define M_ERR_47_SUBMSG_5        MIL_TEXT("Invalid Q factor value.")
#define M_ERR_47_SUBMSG_6        MIL_TEXT("Invalid destination buffers.")
#define M_ERR_47_SUBMSG_7        MIL_TEXT("The second LUT must be M_NULL for M_WARP_POLYNOMIAL operation mode.")
MILERR( M_ERR,47,SUBMSG,7,       MIL_TEXT("When WarpParameters.Coefficients is used the WarpParameters.WarpLUT data are ignored."))
#define M_ERR_47_SUBMSG_8        MIL_TEXT("The angle range must be lower than 360 degrees.")
#define M_ERR_47_SUBMSG_9        MIL_TEXT("Invalid First Warp parameter. Size or Type invalid.")

#define M_INVALID_PARAM_ERROR_8  48L
#define M_ERR_48_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_48_SUBMSG_1        MIL_TEXT("Source buffers must be of the same type and pixel depth.")
#define M_ERR_48_SUBMSG_2        MIL_TEXT("Destination buffers must be of the same type and pixel depth.")
#define M_ERR_48_SUBMSG_3        MIL_TEXT("Angle value must be between -360 and 360.")
#define M_ERR_48_SUBMSG_4        MIL_TEXT("Polar conversion only supported on float buffer.")
#define M_ERR_48_SUBMSG_5        MIL_TEXT("This type of conversion requires a 1 band destination buffer.")
#define M_ERR_48_SUBMSG_6        MIL_TEXT("Invalid transform type")
#define M_ERR_48_SUBMSG_7        MIL_TEXT("The M_NORMALIZE flag must be used with this transform.")
MILERR( M_ERR,48,SUBMSG,7,       MIL_TEXT("The Normalize parameter must set to True with this transform."))
#define M_ERR_48_SUBMSG_8        MIL_TEXT("This type of conversion requires a 1 band source buffer.")
#define M_ERR_48_SUBMSG_9        MIL_TEXT("Invalid number of bytes. Use zero or a positive value.")

#define M_JPEG_ERROR_2           49L
#define M_ERR_49_MSG             MIL_TEXT("JPEG handler general error.")
#define M_ERR_49_SUBMSG_1        MIL_TEXT("Vertical sampling factor of more than 4 not supported.")
#define M_ERR_49_SUBMSG_2        MIL_TEXT("MIL supports only 1-band and 3-band buffers.")
MILERR( M_ERR,49,SUBMSG,2,       MIL_TEXT("Supports only 1-band and 3-band images."))
#define M_ERR_49_SUBMSG_3        MIL_TEXT("MIL only supports sequential baseline DCT JPEG and lossless JPEG files.")
MILERR( M_ERR,49,SUBMSG,3,       MIL_TEXT("Supports only sequential baseline DCT JPEG and lossless JPEG files."))
#define M_ERR_49_SUBMSG_4        MIL_TEXT("Point transform not supported.")
#define M_ERR_49_SUBMSG_5        MIL_TEXT("Reading beyond the available data.")
#define M_ERR_49_SUBMSG_6        MIL_TEXT("End Of Image marker before the end of the file.")
#define M_ERR_49_SUBMSG_7        MIL_TEXT("16-bit quantization table not supported.")
#define M_ERR_49_SUBMSG_8        MIL_TEXT("Horizontal sampling factor of more than 4 not supported.")
#define M_ERR_49_SUBMSG_9        MIL_TEXT("Sampling factors do not correspond to any supported format.")

#define M_INVALID_PARAM_ERROR_9  50L
#define M_ERR_50_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_50_SUBMSG_1        MIL_TEXT("Inquire type not supported.")
#define M_ERR_50_SUBMSG_2        MIL_TEXT("The look up table must have a Y size of 1.")
#define M_ERR_50_SUBMSG_3        MIL_TEXT("The number of pixels should be a positive value.")
#define M_ERR_50_SUBMSG_4        MIL_TEXT("The destination buffer cannot be 1-bit.")
#define M_ERR_50_SUBMSG_5        MIL_TEXT("SaturationSizeBit must be 1, 8, 16 or 32.")
#define M_ERR_50_SUBMSG_6        MIL_TEXT("SaturationType must be M_SIGNED, M_UNSIGNED or M_FLOAT.")
MILERR( M_ERR,50,SUBMSG,6,       MIL_TEXT("SaturationType must be imSigned, imUnsigned or imFloat."))
#define M_ERR_50_SUBMSG_7        MIL_TEXT("Invalid combinaison of SaturationType and SizeBit.")
#define M_ERR_50_SUBMSG_8        MIL_TEXT("YUV buffers must be M_UNSIGNED.")
MILERR( M_ERR,50,SUBMSG,8,       MIL_TEXT("YUV images DataType should must be imUnsigned."))
#define M_ERR_50_SUBMSG_9        MIL_TEXT("Invalid filename: Filename length must be at least 1.")

#define M_AVI_FILE_ERROR         51L
#define M_ERR_51_MSG             MIL_TEXT("AVI file error.")
#define M_ERR_51_SUBMSG_1        MIL_TEXT("Error creating AVI file.")
#define M_ERR_51_SUBMSG_2        MIL_TEXT("Error writing into AVI file.")
#define M_ERR_51_SUBMSG_3        MIL_TEXT("File format not supported.")
#define M_ERR_51_SUBMSG_4        MIL_TEXT("Incompatible format between file and images.")
#define M_ERR_51_SUBMSG_5        MIL_TEXT("Error opening AVI file.")
#define M_ERR_51_SUBMSG_6        MIL_TEXT("One of the buffers to export is empty.")
#define M_ERR_51_SUBMSG_7        MIL_TEXT("Incompatible JPEG format between the buffers and the file type requested.")
#define M_ERR_51_SUBMSG_8        MIL_TEXT("The number of bands should be the same for all the buffers in the sequence.")
#define M_ERR_51_SUBMSG_9        MIL_TEXT("The depth should be the same for all the buffers in the sequence.")

#define M_DMA_ERROR              52L
#define M_ERR_52_MSG             MIL_TEXT("Error using Matrox Dma service")
#define M_ERR_52_SUBMSG_1        MIL_TEXT("Cannot open the Matrox Dma manager")
#define M_ERR_52_SUBMSG_2        MIL_TEXT("Cannot acces PCI data")
#define M_ERR_52_SUBMSG_3        MIL_TEXT("Cannot perform this operation on the Host system with MS_DOS.")
#define M_ERR_52_SUBMSG_4        MIL_TEXT("Cannot perform this operation on the Host system with Windows 95/98.")
#define M_ERR_52_SUBMSG_5        MIL_TEXT("Cannot acces hook information")
#define M_ERR_52_SUBMSG_6        MIL_TEXT("Can perform this operation only on Default Host")
#define M_ERR_52_SUBMSG_7        MIL_TEXT("Unable to recover info on Dma pool")
#define M_ERR_52_SUBMSG_8        MIL_TEXT("Cannot perform this operation on the Host system with Windows 95.")
#define M_ERR_52_SUBMSG_9        NO_SUBMSG

#define M_BMP_ERROR_3            53L
#define M_ERR_53_MSG             MIL_TEXT("BMP handler general error.")
#define M_ERR_53_SUBMSG_1        MIL_TEXT("MIL does not support a bit count of zero in BMP files.")
#define M_ERR_53_SUBMSG_2        MIL_TEXT("MIL does not support 1-bit BMP files.")
#define M_ERR_53_SUBMSG_3        MIL_TEXT("MIL does not support 4-bit BMP files.")
#define M_ERR_53_SUBMSG_4        MIL_TEXT("Run Length Encoding ( RLE ) compression not supported.")
#define M_ERR_53_SUBMSG_5        NO_SUBMSG
#define M_ERR_53_SUBMSG_6        NO_SUBMSG
#define M_ERR_53_SUBMSG_7        NO_SUBMSG
#define M_ERR_53_SUBMSG_8        NO_SUBMSG
#define M_ERR_53_SUBMSG_9        NO_SUBMSG

#define M_JPEG_ERROR_3           54L
#define M_ERR_54_MSG             MIL_TEXT("JPEG handler general error.")
#define M_ERR_54_SUBMSG_1        MIL_TEXT("Invalid JPEG data.")
#define M_ERR_54_SUBMSG_2        MIL_TEXT("Invalid data for a MTRX specific marker.")
#define M_ERR_54_SUBMSG_3        MIL_TEXT("Extended Sequential DCT not supported in MIL.")
#define M_ERR_54_SUBMSG_4        MIL_TEXT("Progressive DCT not supported in MIL.")
#define M_ERR_54_SUBMSG_5        MIL_TEXT("YUV9 Packed images not supported.")
#define M_ERR_54_SUBMSG_6        MIL_TEXT("YUV12 Packed images not supported.")
#define M_ERR_54_SUBMSG_7        MIL_TEXT("YUV24 Packed images not supported.")
#define M_ERR_54_SUBMSG_8        MIL_TEXT("Invalid data. EOI marker not found.")
#define M_ERR_54_SUBMSG_9        MIL_TEXT("Cannot create or open file in write mode.")
  
#define M_ALLOC_ERROR_3          55L
#define M_ERR_55_MSG             MIL_TEXT("Allocation error.")
#define M_ERR_55_SUBMSG_1        MIL_TEXT("Cannot allocate a host buffer in the non paged pool on Windows 9x platforms.")
#define M_ERR_55_SUBMSG_2        MIL_TEXT("Cannot create a buffer with physical address.")
#define M_ERR_55_SUBMSG_3        MIL_TEXT("Cannot allocate a M_COMPRESS buffer with the M_DISP attribute.")
MILERR( M_ERR,55,SUBMSG,3,       MIL_TEXT("Cannot allocate an image with a compression type and the CanDisplay property set to True."))
#define M_ERR_55_SUBMSG_4        MIL_TEXT("Cannot allocate a M_COMPRESS buffer with the M_SIGNED type.")
MILERR( M_ERR,55,SUBMSG,4,       MIL_TEXT("Cannot allocate an image with a compression type and the DataType property set to imSigned."))
#define M_ERR_55_SUBMSG_5        MIL_TEXT("Interlaced buffers should have a height of at least 2.")
#define M_ERR_55_SUBMSG_6        MIL_TEXT("Requested system DLL cannot be loaded.")
#define M_ERR_55_SUBMSG_7        MIL_TEXT("MIL DLLs not found.")
#define M_ERR_55_SUBMSG_8        MIL_TEXT("Not enough non-paged memory to allocate buffer.")
#define M_ERR_55_SUBMSG_9        MIL_TEXT("Not enough non-paged memory for MIL CE allocation.\nContinuing the application may cause abnormal behavior!")

#define M_LIMITATION_ERROR       56L
#define M_ERR_56_MSG             MIL_TEXT("Limitation error.")
#define M_ERR_56_SUBMSG_1        MIL_TEXT("This operation cannot be performed on a section of a compressed buffer.")
#define M_ERR_56_SUBMSG_2        MIL_TEXT("This operation cannot be performed on a child of a compressed buffer.")
#define M_ERR_56_SUBMSG_3        MIL_TEXT("This operation cannot be performed on a section of a YUV buffer.")
#define M_ERR_56_SUBMSG_4        MIL_TEXT("This operation cannot be performed on a child of a YUV buffer.")
#define M_ERR_56_SUBMSG_5        MIL_TEXT("This operation cannot be performed on a section of a M_PACKED buffer.")
MILERR( M_ERR,56,SUBMSG,5,       MIL_TEXT("This operation cannot be performed on a section of an imPacked format image."))
#define M_ERR_56_SUBMSG_6        MIL_TEXT("This operation cannot be performed on a child of a M_PACKED buffer.")
MILERR( M_ERR,56,SUBMSG,6,       MIL_TEXT("This operation cannot be performed on a child of an imPacked format image."))
#define M_ERR_56_SUBMSG_7        MIL_TEXT("This operation cannot be performed on a section of a M_PLANAR buffer.")
MILERR( M_ERR,56,SUBMSG,7,       MIL_TEXT("This operation cannot be performed on a section of an imPlanar format image."))
#define M_ERR_56_SUBMSG_8        MIL_TEXT("This operation cannot be performed on a child of a M_PLANAR buffer.")
MILERR( M_ERR,56,SUBMSG,8,       MIL_TEXT("This operation cannot be performed on a child of an imPlanar format image."))
#define M_ERR_56_SUBMSG_9        NO_SUBMSG

#define M_OPERATION_ERROR_2      57L
#define M_ERR_57_MSG             MIL_TEXT("Operation error.")
#define M_ERR_57_SUBMSG_1        MIL_TEXT("Pitch must be a multiple of 4 pixels.")
#define M_ERR_57_SUBMSG_2        NO_SUBMSG
#define M_ERR_57_SUBMSG_3        NO_SUBMSG
#define M_ERR_57_SUBMSG_4        NO_SUBMSG
#define M_ERR_57_SUBMSG_5        NO_SUBMSG
#define M_ERR_57_SUBMSG_6        NO_SUBMSG
#define M_ERR_57_SUBMSG_7        NO_SUBMSG
#define M_ERR_57_SUBMSG_8        NO_SUBMSG
#define M_ERR_57_SUBMSG_9        NO_SUBMSG

#define M_LICENSING_ERROR        58L
#define M_ERR_58_MSG             MIL_TEXT("Licensing error")
#define M_ERR_58_SUBMSG_1        MIL_TEXT("Dependency error")
#define M_ERR_58_SUBMSG_2        MIL_TEXT("License watchdog timed out")
#define M_ERR_58_SUBMSG_3        MIL_TEXT("A module was used without a valid license")
#define M_ERR_58_SUBMSG_4        MIL_TEXT("Debugging is not allowed with a run-time license. You must use a development license.")
#define M_ERR_58_SUBMSG_5        MIL_TEXT("Your license does not allow the allocation of JPEG buffers")
#define M_ERR_58_SUBMSG_6        MIL_TEXT("Your license does not allow the allocation of JPEG2000 buffers")
#define M_ERR_58_SUBMSG_7        NO_SUBMSG
#define M_ERR_58_SUBMSG_8        NO_SUBMSG
#define M_ERR_58_SUBMSG_9        NO_SUBMSG

#define M_AVI_FILE_ERROR_2       59L
#define M_ERR_59_MSG             MIL_TEXT("AVI file error.")
#define M_ERR_59_SUBMSG_1        MIL_TEXT("Cannot export a sequence of compressed child buffers.")
#define M_ERR_59_SUBMSG_2        MIL_TEXT("The compression type should be the same for all the buffers in the sequence.")
#define M_ERR_59_SUBMSG_3        MIL_TEXT("The internal format should be the same for all the buffers in the sequence.")
#define M_ERR_59_SUBMSG_4        MIL_TEXT("The file format requested for the M_APPEND is not the same as the file format of the file to append into.")
MILERR( M_ERR,59,SUBMSG,4,       MIL_TEXT("The file format requested for the aviAppendToFile parameter is not the same as the file format of the file to append into.")) 
#define M_ERR_59_SUBMSG_5        MIL_TEXT("The size of the images should all be the same." )
#define M_ERR_59_SUBMSG_6        MIL_TEXT("Impossible to write into a closed file.")
#define M_ERR_59_SUBMSG_7        MIL_TEXT("Error creating AVI stream.")
#define M_ERR_59_SUBMSG_8        MIL_TEXT("The file is already opened.")
#define M_ERR_59_SUBMSG_9        MIL_TEXT("Impossible to read from a closed file.")

#define M_JPEG_COMPRESS_ERROR_3  60L
#define M_ERR_60_MSG             MIL_TEXT("JPEG compression error.")
#define M_ERR_60_SUBMSG_1        MIL_TEXT("Cannot allocate 3-band M_JPEG_LOSSLESS_INTERLACED buffer.")
MILERR( M_ERR,60,SUBMSG,1,       MIL_TEXT("Cannot allocate 3-band image with the Format property set to imJPEGLosslessInterlaced.")) 
#define M_ERR_60_SUBMSG_2        MIL_TEXT("In lossless, the RestartInterval multiplied by the number of \ncolumns in the buffer must fit on 16 bits ( smaller than 65535 ).")
#define M_ERR_60_SUBMSG_3        MIL_TEXT("The restart interval must fit on 16 bits ( smaller than 65535 ).")
#define M_ERR_60_SUBMSG_4        MIL_TEXT("Only JPEG lossless buffers can have predictors.")
#define M_ERR_60_SUBMSG_5        MIL_TEXT("Luminance parameters are only allowed for YUV buffers.")
#define M_ERR_60_SUBMSG_6        MIL_TEXT("Chrominance parameters are only allowed for YUV buffers.")
#define M_ERR_60_SUBMSG_7        MIL_TEXT("AC tables and quantization parameters are only allowed \nfor JPEG lossy buffers.")
#define M_ERR_60_SUBMSG_8        MIL_TEXT("For YUV buffers, specific luminance or chrominance \nflags must be used.")
#define M_ERR_60_SUBMSG_9        NO_SUBMSG

#define M_AVI_FILE_ERROR_3       61L
#define M_ERR_61_MSG             MIL_TEXT("AVI file error.")
#define M_ERR_61_SUBMSG_1        MIL_TEXT("Error reading from the AVI file.")
#define M_ERR_61_SUBMSG_2        MIL_TEXT("Cannot read after the last frame.")
#define M_ERR_61_SUBMSG_3        MIL_TEXT("Cannot initialize the COM library.")
#define M_ERR_61_SUBMSG_4        MIL_TEXT("Images must have a width greater than or equal to 16.")
#define M_ERR_61_SUBMSG_5        MIL_TEXT("Images must have a height greater than or equal to 8.")
#define M_ERR_61_SUBMSG_6        NO_SUBMSG
#define M_ERR_61_SUBMSG_7        NO_SUBMSG
#define M_ERR_61_SUBMSG_8        NO_SUBMSG
#define M_ERR_61_SUBMSG_9        NO_SUBMSG

#define M_USER_BIT_CONFIG_ERROR  62L
#define M_ERR_62_MSG             MIL_TEXT("Auxiliary I/O Error:")
#define M_ERR_62_SUBMSG_1        MIL_TEXT("4Sight not detected, or pins already allocated on an other system.")
#define M_ERR_62_SUBMSG_2        MIL_TEXT("Cannot write register, register is read only.")
#define M_ERR_62_SUBMSG_3        MIL_TEXT("Cannot read register, register is write only.")
#define M_ERR_62_SUBMSG_4        MIL_TEXT("Invalid data or register.")
#define M_ERR_62_SUBMSG_5        MIL_TEXT("Unkown Error.")
#define M_ERR_62_SUBMSG_6        MIL_TEXT("Interrupt Overrun.")
#define M_ERR_62_SUBMSG_7        MIL_TEXT("Interrupts have been dropped.")
#define M_ERR_62_SUBMSG_8        NO_SUBMSG
#define M_ERR_62_SUBMSG_9        NO_SUBMSG

#define M_USER_BIT_ERROR         63L
#define M_ERR_63_MSG             MIL_TEXT("Invalid parameter error:")
#define M_ERR_63_SUBMSG_1        MIL_TEXT("Cannot write selected pin, pin is in input mode.")
#define M_ERR_63_SUBMSG_2        MIL_TEXT("Cannot enable interrupt, pin is in output mode.")
#define M_ERR_63_SUBMSG_3        MIL_TEXT("Cannot hook to selected pin, pin is not configured as an interrupt.")
#define M_ERR_63_SUBMSG_4        MIL_TEXT("Control type not supported.")
#define M_ERR_63_SUBMSG_5        MIL_TEXT("Control value not supported.")
#define M_ERR_63_SUBMSG_6        MIL_TEXT("Inquire type not supported.")
#define M_ERR_63_SUBMSG_7        MIL_TEXT("Invalid bit mask.")
#define M_ERR_63_SUBMSG_8        MIL_TEXT("Cannot hook or set interrupt.")
#define M_ERR_63_SUBMSG_9        MIL_TEXT("No interrupt line has been assigned to auxiliary IO.")

#define M_TIFF_ERROR_3           64L
#define M_ERR_64_MSG             MIL_TEXT("TIFF File access error.")
#define M_ERR_64_SUBMSG_1        MIL_TEXT("Error recording tag")
#define M_ERR_64_SUBMSG_2        MIL_TEXT("LZW compression not supported in multi strips TIFF file")
#define M_ERR_64_SUBMSG_3        NO_SUBMSG
#define M_ERR_64_SUBMSG_4        NO_SUBMSG
#define M_ERR_64_SUBMSG_5        NO_SUBMSG
#define M_ERR_64_SUBMSG_6        NO_SUBMSG
#define M_ERR_64_SUBMSG_7        NO_SUBMSG
#define M_ERR_64_SUBMSG_8        NO_SUBMSG
#define M_ERR_64_SUBMSG_9        NO_SUBMSG

#define M_MIL_FILE_ERROR_4       65L
#define M_ERR_65_MSG             MIL_TEXT("MIL File access error.")
#define M_ERR_65_SUBMSG_1        MIL_TEXT("Error recording tag")
#define M_ERR_65_SUBMSG_2        MIL_TEXT("LZW compression not supported in multi strips MIL file")
#define M_ERR_65_SUBMSG_3        NO_SUBMSG
#define M_ERR_65_SUBMSG_4        NO_SUBMSG
#define M_ERR_65_SUBMSG_5        NO_SUBMSG
#define M_ERR_65_SUBMSG_6        NO_SUBMSG
#define M_ERR_65_SUBMSG_7        NO_SUBMSG
#define M_ERR_65_SUBMSG_8        NO_SUBMSG
#define M_ERR_65_SUBMSG_9        NO_SUBMSG

#define M_JPEG_ERROR_4           66L
#define M_ERR_66_MSG             MIL_TEXT("JPEG handler general error.")
#define M_ERR_66_SUBMSG_1        MIL_TEXT("Second field of interlaced image not found.")
#define M_ERR_66_SUBMSG_2        NO_SUBMSG
#define M_ERR_66_SUBMSG_3        NO_SUBMSG
#define M_ERR_66_SUBMSG_4        NO_SUBMSG
#define M_ERR_66_SUBMSG_5        NO_SUBMSG
#define M_ERR_66_SUBMSG_6        NO_SUBMSG
#define M_ERR_66_SUBMSG_7        NO_SUBMSG
#define M_ERR_66_SUBMSG_8        NO_SUBMSG
#define M_ERR_66_SUBMSG_9        NO_SUBMSG

#define M_INVALID_PARAM_ERROR_10 67L
#define M_ERR_67_MSG             MIL_TEXT("Invalid parameter.")
#define M_ERR_67_SUBMSG_1        MIL_TEXT("The width and height of the source and destination buffers must be a multiple of 8.")
#define M_ERR_67_SUBMSG_2        MIL_TEXT("Operation not supported on 16 bits buffer.")
#define M_ERR_67_SUBMSG_3        MIL_TEXT("Size of image buffers must be greater than or equal to the size of the kernel.")
MILERR( M_ERR,67,SUBMSG,3,       MIL_TEXT("Size of images must be greater than or equal to the size of the kernel."))
#define M_ERR_67_SUBMSG_4        MIL_TEXT("Invalid data formats")
#define M_ERR_67_SUBMSG_5        NO_SUBMSG
#define M_ERR_67_SUBMSG_6        NO_SUBMSG
#define M_ERR_67_SUBMSG_7        NO_SUBMSG
#define M_ERR_67_SUBMSG_8        NO_SUBMSG
#define M_ERR_67_SUBMSG_9        NO_SUBMSG

#define M_J2K_ERROR_1            68L
#define M_ERR_68_MSG             MIL_TEXT("JPEG2000 handler general error.")
#define M_ERR_68_SUBMSG_1        MIL_TEXT("Error closing JPEG2000 file.")
#define M_ERR_68_SUBMSG_2        MIL_TEXT("Unable to allocate sufficient memory.")
#define M_ERR_68_SUBMSG_3        MIL_TEXT("Cannot create or open file in write mode.")
#define M_ERR_68_SUBMSG_4        MIL_TEXT("Error opening JPEG2000 file.")
#define M_ERR_68_SUBMSG_5        NO_SUBMSG
#define M_ERR_68_SUBMSG_6        NO_SUBMSG
#define M_ERR_68_SUBMSG_7        NO_SUBMSG
#define M_ERR_68_SUBMSG_8        NO_SUBMSG
#define M_ERR_68_SUBMSG_9        NO_SUBMSG

#define M_J2K_ERROR_2            69L
#define M_ERR_69_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_69_SUBMSG_1        MIL_TEXT("Invalid JPEG2000 data.")
#define M_ERR_69_SUBMSG_2        MIL_TEXT("Unexpected marker in the JPEG2000 image.")
#define M_ERR_69_SUBMSG_3        MIL_TEXT("Component index out-of-range.")
#define M_ERR_69_SUBMSG_4        MIL_TEXT("MIL supports only 1-band and 3-band buffers.")
#define M_ERR_69_SUBMSG_5        MIL_TEXT("All components must have the same bit depth.")
#define M_ERR_69_SUBMSG_6        MIL_TEXT("Only 8-bit or 16-bit images are supported.")
#define M_ERR_69_SUBMSG_7        MIL_TEXT("Invalid subsampling factors.")
#define M_ERR_69_SUBMSG_8        MIL_TEXT("Scalar implicit quantization not supported.")
#define M_ERR_69_SUBMSG_9        MIL_TEXT("Only the entropy coding style is supported.")

#define M_LICENSE_ERROR_2        70L
#define M_ERR_70_MSG             MIL_TEXT("License error.")
#define M_ERR_70_SUBMSG_1        MIL_TEXT("No valid license found (1)")
#define M_ERR_70_SUBMSG_2        MIL_TEXT("No valid license found (2)")
#define M_ERR_70_SUBMSG_3        MIL_TEXT("No valid license found (3)")
#define M_ERR_70_SUBMSG_4        MIL_TEXT("No valid license found (4)")
#define M_ERR_70_SUBMSG_5        MIL_TEXT("No valid license found (5)")
#define M_ERR_70_SUBMSG_6        MIL_TEXT("No valid license found (6)")
#define M_ERR_70_SUBMSG_7        MIL_TEXT("No valid license found (7)")
#define M_ERR_70_SUBMSG_8        NO_SUBMSG
#define M_ERR_70_SUBMSG_9        NO_SUBMSG

#define M_J2K_ERROR_3            71L
#define M_ERR_71_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_71_SUBMSG_1        MIL_TEXT("Unsupported progression order.")
#define M_ERR_71_SUBMSG_2        MIL_TEXT("Multi-layer images are not supported.")
#define M_ERR_71_SUBMSG_3        MIL_TEXT("Unsupported compression setting.")
#define M_ERR_71_SUBMSG_4        MIL_TEXT("Settings on a tile-by-tile basis are not allowed.")
#define M_ERR_71_SUBMSG_5        MIL_TEXT("Wavelet type cannot be changed.")
#define M_ERR_71_SUBMSG_6        MIL_TEXT("EOC marker not found.")
#define M_ERR_71_SUBMSG_7        MIL_TEXT("Not enough user memory to do the operation.")
#define M_ERR_71_SUBMSG_8        MIL_TEXT("Invalid number of decomposition levels.")
#define M_ERR_71_SUBMSG_9        MIL_TEXT("Invalid number of wavelet sub-bands.")

#define M_J2K_ERROR_4            72L
#define M_ERR_72_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_72_SUBMSG_1        MIL_TEXT("The target size must be a positive number of bytes.")
#define M_ERR_72_SUBMSG_2        MIL_TEXT("Specification of target size is only allowed in lossy mode.")
#define M_ERR_72_SUBMSG_3        MIL_TEXT("Quantization tables are applicable to lossy buffers only.")
#define M_ERR_72_SUBMSG_4        MIL_TEXT("The Q factor is only valid with lossy buffers.")
#define M_ERR_72_SUBMSG_5        MIL_TEXT("Wrong number of entries in the quantization table.")
#define M_ERR_72_SUBMSG_6        MIL_TEXT("Invalid MIL_ID for the quantization table.")
#define M_ERR_72_SUBMSG_7        MIL_TEXT("The quantization table must be a one-band, 32 bits floating-point array.")
#define M_ERR_72_SUBMSG_8        MIL_TEXT("Valid Q factor values are between 1 and 99 (both inclusive).")
#define M_ERR_72_SUBMSG_9        MIL_TEXT("Expected marker missing.")

#define M_J2K_ERROR_5            73L
#define M_ERR_73_MSG             MIL_TEXT("JPEG2000 compression error.")
#define M_ERR_73_SUBMSG_1        MIL_TEXT("Corrupted JPEG2000 data.")
#define M_ERR_73_SUBMSG_2        MIL_TEXT("Quantization coefficient out-of-range.")
#define M_ERR_73_SUBMSG_3        MIL_TEXT("Luminance and chrominance parameters are only allowed for YUV buffers.")
#define M_ERR_73_SUBMSG_4        NO_SUBMSG
#define M_ERR_73_SUBMSG_5        NO_SUBMSG
#define M_ERR_73_SUBMSG_6        NO_SUBMSG
#define M_ERR_73_SUBMSG_7        NO_SUBMSG
#define M_ERR_73_SUBMSG_8        NO_SUBMSG
#define M_ERR_73_SUBMSG_9        NO_SUBMSG

#define M_FUNC_CALL_ERROR        74L
#define M_ERR_74_MSG             MIL_TEXT("Function call error.")
#define M_ERR_74_SUBMSG_1        MIL_TEXT("Calibration not supported on remote system")
#define M_ERR_74_SUBMSG_2        MIL_TEXT("Unable to load the DLL")
#define M_ERR_74_SUBMSG_3        MIL_TEXT("Work function not found in the DLL")
#define M_ERR_74_SUBMSG_4        NO_SUBMSG
#define M_ERR_74_SUBMSG_5        NO_SUBMSG
#define M_ERR_74_SUBMSG_6        NO_SUBMSG
#define M_ERR_74_SUBMSG_7        NO_SUBMSG
#define M_ERR_74_SUBMSG_8        NO_SUBMSG
#define M_ERR_74_SUBMSG_9        NO_SUBMSG

#define M_XX75XX_ERROR           75L
#define M_ERR_75_MSG             NO_MSG
#define M_ERR_75_SUBMSG_1        NO_SUBMSG
#define M_ERR_75_SUBMSG_2        NO_SUBMSG
#define M_ERR_75_SUBMSG_3        NO_SUBMSG
#define M_ERR_75_SUBMSG_4        NO_SUBMSG
#define M_ERR_75_SUBMSG_5        NO_SUBMSG
#define M_ERR_75_SUBMSG_6        NO_SUBMSG
#define M_ERR_75_SUBMSG_7        NO_SUBMSG
#define M_ERR_75_SUBMSG_8        NO_SUBMSG
#define M_ERR_75_SUBMSG_9        NO_SUBMSG

#define M_XX76XX_ERROR           76L
#define M_ERR_76_MSG             NO_MSG
#define M_ERR_76_SUBMSG_1        NO_SUBMSG
#define M_ERR_76_SUBMSG_2        NO_SUBMSG
#define M_ERR_76_SUBMSG_3        NO_SUBMSG
#define M_ERR_76_SUBMSG_4        NO_SUBMSG
#define M_ERR_76_SUBMSG_5        NO_SUBMSG
#define M_ERR_76_SUBMSG_6        NO_SUBMSG
#define M_ERR_76_SUBMSG_7        NO_SUBMSG
#define M_ERR_76_SUBMSG_8        NO_SUBMSG
#define M_ERR_76_SUBMSG_9        NO_SUBMSG

#define M_XX77XX_ERROR           77L
#define M_ERR_77_MSG             NO_MSG
#define M_ERR_77_SUBMSG_1        NO_SUBMSG
#define M_ERR_77_SUBMSG_2        NO_SUBMSG
#define M_ERR_77_SUBMSG_3        NO_SUBMSG
#define M_ERR_77_SUBMSG_4        NO_SUBMSG
#define M_ERR_77_SUBMSG_5        NO_SUBMSG
#define M_ERR_77_SUBMSG_6        NO_SUBMSG
#define M_ERR_77_SUBMSG_7        NO_SUBMSG
#define M_ERR_77_SUBMSG_8        NO_SUBMSG
#define M_ERR_77_SUBMSG_9        NO_SUBMSG

#define M_XX78XX_ERROR           78L
#define M_ERR_78_MSG             NO_MSG
#define M_ERR_78_SUBMSG_1        NO_SUBMSG
#define M_ERR_78_SUBMSG_2        NO_SUBMSG
#define M_ERR_78_SUBMSG_3        NO_SUBMSG
#define M_ERR_78_SUBMSG_4        NO_SUBMSG
#define M_ERR_78_SUBMSG_5        NO_SUBMSG
#define M_ERR_78_SUBMSG_6        NO_SUBMSG
#define M_ERR_78_SUBMSG_7        NO_SUBMSG
#define M_ERR_78_SUBMSG_8        NO_SUBMSG
#define M_ERR_78_SUBMSG_9        NO_SUBMSG

#define M_XX79XX_ERROR           79L
#define M_ERR_79_MSG             NO_MSG
#define M_ERR_79_SUBMSG_1        NO_SUBMSG
#define M_ERR_79_SUBMSG_2        NO_SUBMSG
#define M_ERR_79_SUBMSG_3        NO_SUBMSG
#define M_ERR_79_SUBMSG_4        NO_SUBMSG
#define M_ERR_79_SUBMSG_5        NO_SUBMSG
#define M_ERR_79_SUBMSG_6        NO_SUBMSG
#define M_ERR_79_SUBMSG_7        NO_SUBMSG
#define M_ERR_79_SUBMSG_8        NO_SUBMSG
#define M_ERR_79_SUBMSG_9        NO_SUBMSG

#define M_XX80XX_ERROR           80L
#define M_ERR_80_MSG             NO_MSG
#define M_ERR_80_SUBMSG_1        NO_SUBMSG
#define M_ERR_80_SUBMSG_2        NO_SUBMSG
#define M_ERR_80_SUBMSG_3        NO_SUBMSG
#define M_ERR_80_SUBMSG_4        NO_SUBMSG
#define M_ERR_80_SUBMSG_5        NO_SUBMSG
#define M_ERR_80_SUBMSG_6        NO_SUBMSG
#define M_ERR_80_SUBMSG_7        NO_SUBMSG
#define M_ERR_80_SUBMSG_8        NO_SUBMSG
#define M_ERR_80_SUBMSG_9        NO_SUBMSG

#define M_XX81XX_ERROR           81L
#define M_ERR_81_MSG             NO_MSG
#define M_ERR_81_SUBMSG_1        NO_SUBMSG
#define M_ERR_81_SUBMSG_2        NO_SUBMSG
#define M_ERR_81_SUBMSG_3        NO_SUBMSG
#define M_ERR_81_SUBMSG_4        NO_SUBMSG
#define M_ERR_81_SUBMSG_5        NO_SUBMSG
#define M_ERR_81_SUBMSG_6        NO_SUBMSG
#define M_ERR_81_SUBMSG_7        NO_SUBMSG
#define M_ERR_81_SUBMSG_8        NO_SUBMSG
#define M_ERR_81_SUBMSG_9        NO_SUBMSG

#define M_XX82XX_ERROR           82L
#define M_ERR_82_MSG             NO_MSG
#define M_ERR_82_SUBMSG_1        NO_SUBMSG
#define M_ERR_82_SUBMSG_2        NO_SUBMSG
#define M_ERR_82_SUBMSG_3        NO_SUBMSG
#define M_ERR_82_SUBMSG_4        NO_SUBMSG
#define M_ERR_82_SUBMSG_5        NO_SUBMSG
#define M_ERR_82_SUBMSG_6        NO_SUBMSG
#define M_ERR_82_SUBMSG_7        NO_SUBMSG
#define M_ERR_82_SUBMSG_8        NO_SUBMSG
#define M_ERR_82_SUBMSG_9        NO_SUBMSG

#define M_XX83XX_ERROR           83L
#define M_ERR_83_MSG             NO_MSG
#define M_ERR_83_SUBMSG_1        NO_SUBMSG
#define M_ERR_83_SUBMSG_2        NO_SUBMSG
#define M_ERR_83_SUBMSG_3        NO_SUBMSG
#define M_ERR_83_SUBMSG_4        NO_SUBMSG
#define M_ERR_83_SUBMSG_5        NO_SUBMSG
#define M_ERR_83_SUBMSG_6        NO_SUBMSG
#define M_ERR_83_SUBMSG_7        NO_SUBMSG
#define M_ERR_83_SUBMSG_8        NO_SUBMSG
#define M_ERR_83_SUBMSG_9        NO_SUBMSG

#define M_XX84XX_ERROR           84L
#define M_ERR_84_MSG             NO_MSG
#define M_ERR_84_SUBMSG_1        NO_SUBMSG
#define M_ERR_84_SUBMSG_2        NO_SUBMSG
#define M_ERR_84_SUBMSG_3        NO_SUBMSG
#define M_ERR_84_SUBMSG_4        NO_SUBMSG
#define M_ERR_84_SUBMSG_5        NO_SUBMSG
#define M_ERR_84_SUBMSG_6        NO_SUBMSG
#define M_ERR_84_SUBMSG_7        NO_SUBMSG
#define M_ERR_84_SUBMSG_8        NO_SUBMSG
#define M_ERR_84_SUBMSG_9        NO_SUBMSG

#define M_XX85XX_ERROR           85L
#define M_ERR_85_MSG             NO_MSG
#define M_ERR_85_SUBMSG_1        NO_SUBMSG
#define M_ERR_85_SUBMSG_2        NO_SUBMSG
#define M_ERR_85_SUBMSG_3        NO_SUBMSG
#define M_ERR_85_SUBMSG_4        NO_SUBMSG
#define M_ERR_85_SUBMSG_5        NO_SUBMSG
#define M_ERR_85_SUBMSG_6        NO_SUBMSG
#define M_ERR_85_SUBMSG_7        NO_SUBMSG
#define M_ERR_85_SUBMSG_8        NO_SUBMSG
#define M_ERR_85_SUBMSG_9        NO_SUBMSG

#define M_XX86XX_ERROR           86L
#define M_ERR_86_MSG             NO_MSG
#define M_ERR_86_SUBMSG_1        NO_SUBMSG
#define M_ERR_86_SUBMSG_2        NO_SUBMSG
#define M_ERR_86_SUBMSG_3        NO_SUBMSG
#define M_ERR_86_SUBMSG_4        NO_SUBMSG
#define M_ERR_86_SUBMSG_5        NO_SUBMSG
#define M_ERR_86_SUBMSG_6        NO_SUBMSG
#define M_ERR_86_SUBMSG_7        NO_SUBMSG
#define M_ERR_86_SUBMSG_8        NO_SUBMSG
#define M_ERR_86_SUBMSG_9        NO_SUBMSG

#define M_XX87XX_ERROR           87L
#define M_ERR_87_MSG             NO_MSG
#define M_ERR_87_SUBMSG_1        NO_SUBMSG
#define M_ERR_87_SUBMSG_2        NO_SUBMSG
#define M_ERR_87_SUBMSG_3        NO_SUBMSG
#define M_ERR_87_SUBMSG_4        NO_SUBMSG
#define M_ERR_87_SUBMSG_5        NO_SUBMSG
#define M_ERR_87_SUBMSG_6        NO_SUBMSG
#define M_ERR_87_SUBMSG_7        NO_SUBMSG
#define M_ERR_87_SUBMSG_8        NO_SUBMSG
#define M_ERR_87_SUBMSG_9        NO_SUBMSG

#define M_XX88XX_ERROR           88L
#define M_ERR_88_MSG             NO_MSG
#define M_ERR_88_SUBMSG_1        NO_SUBMSG
#define M_ERR_88_SUBMSG_2        NO_SUBMSG
#define M_ERR_88_SUBMSG_3        NO_SUBMSG
#define M_ERR_88_SUBMSG_4        NO_SUBMSG
#define M_ERR_88_SUBMSG_5        NO_SUBMSG
#define M_ERR_88_SUBMSG_6        NO_SUBMSG
#define M_ERR_88_SUBMSG_7        NO_SUBMSG
#define M_ERR_88_SUBMSG_8        NO_SUBMSG
#define M_ERR_88_SUBMSG_9        NO_SUBMSG

#define M_XX89XX_ERROR           89L
#define M_ERR_89_MSG             NO_MSG
#define M_ERR_89_SUBMSG_1        NO_SUBMSG
#define M_ERR_89_SUBMSG_2        NO_SUBMSG
#define M_ERR_89_SUBMSG_3        NO_SUBMSG
#define M_ERR_89_SUBMSG_4        NO_SUBMSG
#define M_ERR_89_SUBMSG_5        NO_SUBMSG
#define M_ERR_89_SUBMSG_6        NO_SUBMSG
#define M_ERR_89_SUBMSG_7        NO_SUBMSG
#define M_ERR_89_SUBMSG_8        NO_SUBMSG
#define M_ERR_89_SUBMSG_9        NO_SUBMSG

#define M_XX90XX_ERROR           90L
#define M_ERR_90_MSG             NO_MSG
#define M_ERR_90_SUBMSG_1        NO_SUBMSG
#define M_ERR_90_SUBMSG_2        NO_SUBMSG
#define M_ERR_90_SUBMSG_3        NO_SUBMSG
#define M_ERR_90_SUBMSG_4        NO_SUBMSG
#define M_ERR_90_SUBMSG_5        NO_SUBMSG
#define M_ERR_90_SUBMSG_6        NO_SUBMSG
#define M_ERR_90_SUBMSG_7        NO_SUBMSG
#define M_ERR_90_SUBMSG_8        NO_SUBMSG
#define M_ERR_90_SUBMSG_9        NO_SUBMSG

#define M_XX91XX_ERROR           91L
#define M_ERR_91_MSG             NO_MSG
#define M_ERR_91_SUBMSG_1        NO_SUBMSG
#define M_ERR_91_SUBMSG_2        NO_SUBMSG
#define M_ERR_91_SUBMSG_3        NO_SUBMSG
#define M_ERR_91_SUBMSG_4        NO_SUBMSG
#define M_ERR_91_SUBMSG_5        NO_SUBMSG
#define M_ERR_91_SUBMSG_6        NO_SUBMSG
#define M_ERR_91_SUBMSG_7        NO_SUBMSG
#define M_ERR_91_SUBMSG_8        NO_SUBMSG
#define M_ERR_91_SUBMSG_9        NO_SUBMSG

#define M_XX92XX_ERROR           92L
#define M_ERR_92_MSG             NO_MSG
#define M_ERR_92_SUBMSG_1        NO_SUBMSG
#define M_ERR_92_SUBMSG_2        NO_SUBMSG
#define M_ERR_92_SUBMSG_3        NO_SUBMSG
#define M_ERR_92_SUBMSG_4        NO_SUBMSG
#define M_ERR_92_SUBMSG_5        NO_SUBMSG
#define M_ERR_92_SUBMSG_6        NO_SUBMSG
#define M_ERR_92_SUBMSG_7        NO_SUBMSG
#define M_ERR_92_SUBMSG_8        NO_SUBMSG
#define M_ERR_92_SUBMSG_9        NO_SUBMSG

#define M_XX93XX_ERROR           93L
#define M_ERR_93_MSG             NO_MSG
#define M_ERR_93_SUBMSG_1        NO_SUBMSG
#define M_ERR_93_SUBMSG_2        NO_SUBMSG
#define M_ERR_93_SUBMSG_3        NO_SUBMSG
#define M_ERR_93_SUBMSG_4        NO_SUBMSG
#define M_ERR_93_SUBMSG_5        NO_SUBMSG
#define M_ERR_93_SUBMSG_6        NO_SUBMSG
#define M_ERR_93_SUBMSG_7        NO_SUBMSG
#define M_ERR_93_SUBMSG_8        NO_SUBMSG
#define M_ERR_93_SUBMSG_9        NO_SUBMSG

#define M_XX94XX_ERROR           94L
#define M_ERR_94_MSG             NO_MSG
#define M_ERR_94_SUBMSG_1        NO_SUBMSG
#define M_ERR_94_SUBMSG_2        NO_SUBMSG
#define M_ERR_94_SUBMSG_3        NO_SUBMSG
#define M_ERR_94_SUBMSG_4        NO_SUBMSG
#define M_ERR_94_SUBMSG_5        NO_SUBMSG
#define M_ERR_94_SUBMSG_6        NO_SUBMSG
#define M_ERR_94_SUBMSG_7        NO_SUBMSG
#define M_ERR_94_SUBMSG_8        NO_SUBMSG
#define M_ERR_94_SUBMSG_9        NO_SUBMSG

#define M_XX95XX_ERROR           95L
#define M_ERR_95_MSG             NO_MSG
#define M_ERR_95_SUBMSG_1        NO_SUBMSG
#define M_ERR_95_SUBMSG_2        NO_SUBMSG
#define M_ERR_95_SUBMSG_3        NO_SUBMSG
#define M_ERR_95_SUBMSG_4        NO_SUBMSG
#define M_ERR_95_SUBMSG_5        NO_SUBMSG
#define M_ERR_95_SUBMSG_6        NO_SUBMSG
#define M_ERR_95_SUBMSG_7        NO_SUBMSG
#define M_ERR_95_SUBMSG_8        NO_SUBMSG
#define M_ERR_95_SUBMSG_9        NO_SUBMSG

#define M_XX96XX_ERROR           96L
#define M_ERR_96_MSG             NO_MSG
#define M_ERR_96_SUBMSG_1        NO_SUBMSG
#define M_ERR_96_SUBMSG_2        NO_SUBMSG
#define M_ERR_96_SUBMSG_3        NO_SUBMSG
#define M_ERR_96_SUBMSG_4        NO_SUBMSG
#define M_ERR_96_SUBMSG_5        NO_SUBMSG
#define M_ERR_96_SUBMSG_6        NO_SUBMSG
#define M_ERR_96_SUBMSG_7        NO_SUBMSG
#define M_ERR_96_SUBMSG_8        NO_SUBMSG
#define M_ERR_96_SUBMSG_9        NO_SUBMSG

#define M_XX97XX_ERROR           97L
#define M_ERR_97_MSG             NO_MSG
#define M_ERR_97_SUBMSG_1        NO_SUBMSG
#define M_ERR_97_SUBMSG_2        NO_SUBMSG
#define M_ERR_97_SUBMSG_3        NO_SUBMSG
#define M_ERR_97_SUBMSG_4        NO_SUBMSG
#define M_ERR_97_SUBMSG_5        NO_SUBMSG
#define M_ERR_97_SUBMSG_6        NO_SUBMSG
#define M_ERR_97_SUBMSG_7        NO_SUBMSG
#define M_ERR_97_SUBMSG_8        NO_SUBMSG
#define M_ERR_97_SUBMSG_9        NO_SUBMSG

#define M_XX98XX_ERROR           98L
#define M_ERR_98_MSG             NO_MSG
#define M_ERR_98_SUBMSG_1        NO_SUBMSG
#define M_ERR_98_SUBMSG_2        NO_SUBMSG
#define M_ERR_98_SUBMSG_3        NO_SUBMSG
#define M_ERR_98_SUBMSG_4        NO_SUBMSG
#define M_ERR_98_SUBMSG_5        NO_SUBMSG
#define M_ERR_98_SUBMSG_6        NO_SUBMSG
#define M_ERR_98_SUBMSG_7        NO_SUBMSG
#define M_ERR_98_SUBMSG_8        NO_SUBMSG
#define M_ERR_98_SUBMSG_9        NO_SUBMSG

#define M_XX99XX_ERROR           99L
#define M_ERR_99_MSG             NO_MSG
#define M_ERR_99_SUBMSG_1        NO_SUBMSG
#define M_ERR_99_SUBMSG_2        NO_SUBMSG
#define M_ERR_99_SUBMSG_3        NO_SUBMSG
#define M_ERR_99_SUBMSG_4        NO_SUBMSG
#define M_ERR_99_SUBMSG_5        NO_SUBMSG
#define M_ERR_99_SUBMSG_6        NO_SUBMSG
#define M_ERR_99_SUBMSG_7        NO_SUBMSG
#define M_ERR_99_SUBMSG_8        NO_SUBMSG
#define M_ERR_99_SUBMSG_9        NO_SUBMSG


/* LIST FUNCTION NAMES AND MESSAGES */


/* MIM */

#define M_NO_FUNCTION               0L
#define M_FCT_0_NAME                MIL_TEXT("")

#define M_MIMSTARTOPCODE            1L
#define M_MIMALLOCRESULT            1L
#define M_FCT_1_NAME                MIL_TEXT("MimAllocResult")
#define M_MIMARITH                  2L
#define M_FCT_2_NAME                MIL_TEXT("MimArith")
#define M_MIMEDGEDETECT             3L
#define M_FCT_3_NAME                MIL_TEXT("MimEdgeDetect")
#define M_MIMBINARIZE               4L
#define M_FCT_4_NAME                MIL_TEXT("MimBinarize")
#define M_MIMCLIP                   5L
#define M_FCT_5_NAME                MIL_TEXT("MimClip")
#define M_MIMCLOSE                  6L
#define M_FCT_6_NAME                MIL_TEXT("MimClose")
#define M_MIMCONNECTMAP             7L
#define M_FCT_7_NAME                MIL_TEXT("MimConnectMap")
#define M_MIMCONVOLVE               8L
#define M_FCT_8_NAME                MIL_TEXT("MimConvolve")
#define M_MIMCOUNTDIFFERENCE        9L
#define M_FCT_9_NAME                MIL_TEXT("MimCountDifference")
#define M_MIMDILATE                10L
#define M_FCT_10_NAME              MIL_TEXT("MimDilate")
#define M_MIMERODE                 11L
#define M_FCT_11_NAME              MIL_TEXT("MimErode")
#define M_MIMFINDEXTREME           12L
#define M_FCT_12_NAME              MIL_TEXT("MimFindExtreme")
#define M_MIMFREE                  13L
#define M_FCT_13_NAME              MIL_TEXT("MimFree")
#define M_MIMGETRESULT1D           14L
#define M_FCT_14_NAME              MIL_TEXT("MimGetResult1d")
#define M_MIMHISTOGRAM             15L
#define M_FCT_15_NAME              MIL_TEXT("MimHistogram")
#define M_MIMHISTOGRAMEQUALIZE     16L
#define M_FCT_16_NAME              MIL_TEXT("MimHistogramEqualize")
#define M_MIMLABEL                 17L
#define M_FCT_17_NAME              MIL_TEXT("MimLabel")
#define M_MIMLOCATEEVENT           18L
#define M_FCT_18_NAME              MIL_TEXT("MimLocateEvent")
#define M_MIMLUTMAP                19L
#define M_FCT_19_NAME              MIL_TEXT("MimLutMap")
#define M_MIMMORPHIC               20L
#define M_FCT_20_NAME              MIL_TEXT("MimMorphic")
#define M_MIMOPEN                  21L
#define M_FCT_21_NAME              MIL_TEXT("MimOpen")
#define M_MIMPROJECT               22L
#define M_FCT_22_NAME              MIL_TEXT("MimProject")
#define M_MIMRANK                  23L
#define M_FCT_23_NAME              MIL_TEXT("MimRank")
#define M_MIMRESIZE                24L
#define M_FCT_24_NAME              MIL_TEXT("MimResize")
#define M_MIMROTATE                25L
#define M_FCT_25_NAME              MIL_TEXT("MimRotate")
#define M_MIMSHIFT                 26L
#define M_FCT_26_NAME              MIL_TEXT("MimShift")
#define M_MIMTHIN                  27L
#define M_FCT_27_NAME              MIL_TEXT("MimThin")
#define M_MIMTRANSLATE             28L
#define M_FCT_28_NAME              MIL_TEXT("MimTranslate")
#define M_MIMGETRESULT             29L
#define M_FCT_29_NAME              MIL_TEXT("MimGetResult")
#define M_MIMINQUIRE               30L
#define M_FCT_30_NAME              MIL_TEXT("MimInquire")
#define M_MIMCONVERT               31L
#define M_FCT_31_NAME              MIL_TEXT("MimConvert")
#define M_MIMTHICK                 32L
#define M_FCT_32_NAME              MIL_TEXT("MimThick")
#define M_MIMDISTANCE              33L
#define M_FCT_33_NAME              MIL_TEXT("MimDistance")
#define M_MIMARITHMULTIPLE         34L
#define M_FCT_34_NAME              MIL_TEXT("MimArithMultiple")
#define M_MIMFLIP                  35L
#define M_FCT_35_NAME              MIL_TEXT("MimFlip")
#define M_MIMWARP                  36L
#define M_FCT_36_NAME              MIL_TEXT("MimWarp")
#define M_MIMFFT                   37L
#define M_FCT_37_NAME              MIL_TEXT("MimFFT")
#define M_MIMWATERSHED             38L
#define M_FCT_38_NAME              MIL_TEXT("MimWatershed")
#define M_MIMZONEOFINFLUENCE       39L
#define M_FCT_39_NAME              MIL_TEXT("MimZoneOfInfluence")
#define M_MIMTRANSFORM             40L
#define M_FCT_40_NAME              MIL_TEXT("MimTransform")
#define M_MIMPOLAR                 41L
#define M_FCT_41_NAME              MIL_TEXT("MimPolarTransform")
#define M_FCT_42                   42L
#define M_FCT_42_NAME              NO_FCTNAME
#define M_FCT_43                   43L
#define M_FCT_43_NAME              NO_FCTNAME
#define M_FCT_44                   44L
#define M_FCT_44_NAME              NO_FCTNAME
#define M_FCT_45                   45L
#define M_FCT_45_NAME              NO_FCTNAME
#define M_FCT_46                   46L
#define M_FCT_46_NAME              NO_FCTNAME
#define M_FCT_47                   47L
#define M_FCT_47_NAME              NO_FCTNAME
#define M_FCT_48                   48L
#define M_FCT_48_NAME              NO_FCTNAME
#define M_FCT_49                   49L
#define M_FCT_49_NAME              NO_FCTNAME
#define M_FCT_50                   50L
#define M_FCT_50_NAME              NO_FCTNAME
#define M_FCT_51                   51L
#define M_FCT_51_NAME              NO_FCTNAME
#define M_FCT_52                   52L
#define M_FCT_52_NAME              NO_FCTNAME
#define M_FCT_53                   53L
#define M_FCT_53_NAME              NO_FCTNAME
#define M_FCT_54                   54L
#define M_FCT_54_NAME              NO_FCTNAME
#define M_FCT_55                   55L
#define M_FCT_55_NAME              NO_FCTNAME
#define M_FCT_56                   56L
#define M_FCT_56_NAME              NO_FCTNAME
#define M_FCT_57                   57L
#define M_FCT_57_NAME              NO_FCTNAME
#define M_FCT_58                   58L
#define M_FCT_58_NAME              NO_FCTNAME
#define M_FCT_59                   59L
#define M_FCT_59_NAME              NO_FCTNAME
#define M_FCT_60                   60L
#define M_FCT_60_NAME              NO_FCTNAME
#define M_FCT_61                   61L
#define M_FCT_61_NAME              NO_FCTNAME
#define M_FCT_62                   62L
#define M_FCT_62_NAME              NO_FCTNAME
#define M_FCT_63                   63L
#define M_FCT_63_NAME              NO_FCTNAME
#define M_FCT_64                   64L
#define M_FCT_64_NAME              NO_FCTNAME
#define M_FCT_65                   65L
#define M_FCT_65_NAME              NO_FCTNAME
#define M_FCT_66                   66L
#define M_FCT_66_NAME              NO_FCTNAME
#define M_FCT_67                   67L
#define M_FCT_67_NAME              NO_FCTNAME
#define M_FCT_68                   68L
#define M_FCT_68_NAME              NO_FCTNAME
#define M_FCT_69                   69L
#define M_FCT_69_NAME              NO_FCTNAME
#define M_FCT_70                   70L
#define M_FCT_70_NAME              NO_FCTNAME
#define M_FCT_71                   71L
#define M_FCT_71_NAME              NO_FCTNAME
#define M_FCT_72                   72L
#define M_FCT_72_NAME              NO_FCTNAME
#define M_FCT_73                   73L
#define M_FCT_73_NAME              NO_FCTNAME
#define M_FCT_74                   74L
#define M_FCT_74_NAME              NO_FCTNAME
#define M_FCT_75                   75L
#define M_FCT_75_NAME              NO_FCTNAME
#define M_FCT_76                   76L
#define M_FCT_76_NAME              NO_FCTNAME
#define M_FCT_77                   77L
#define M_FCT_77_NAME              NO_FCTNAME




#define M_FCT_78                   78L
#define M_FCT_78_NAME              NO_FCTNAME
#define M_FCT_79                   79L
#define M_FCT_79_NAME              NO_FCTNAME
#define M_FCT_80                   80L
#define M_FCT_80_NAME              NO_FCTNAME
#define M_FCT_81                   81L
#define M_FCT_81_NAME              NO_FCTNAME
#define M_FCT_82                   82L
#define M_FCT_82_NAME              NO_FCTNAME
#define M_FCT_83                   83L
#define M_FCT_83_NAME              NO_FCTNAME
#define M_FCT_84                   84L
#define M_FCT_84_NAME              NO_FCTNAME
#define M_FCT_85                   85L
#define M_FCT_85_NAME              NO_FCTNAME
#define M_FCT_86                   86L
#define M_FCT_86_NAME              NO_FCTNAME
#define M_FCT_87                   87L
#define M_FCT_87_NAME              NO_FCTNAME
#define M_FCT_88                   88L
#define M_FCT_88_NAME              NO_FCTNAME
#define M_FCT_89                   89L
#define M_FCT_89_NAME              NO_FCTNAME
#define M_FCT_90                   90L
#define M_FCT_90_NAME              NO_FCTNAME
#define M_FCT_91                   91L
#define M_FCT_91_NAME              NO_FCTNAME
#define M_FCT_92                   92L
#define M_FCT_92_NAME              NO_FCTNAME
#define M_FCT_93                   93L
#define M_FCT_93_NAME              NO_FCTNAME
#define M_FCT_94                   94L
#define M_FCT_94_NAME              NO_FCTNAME
#define M_FCT_95                   95L
#define M_FCT_95_NAME              NO_FCTNAME
#define M_FCT_96                   96L
#define M_FCT_96_NAME              NO_FCTNAME
#define M_FCT_97                   97L
#define M_FCT_97_NAME              NO_FCTNAME
#define M_FCT_98                   98L
#define M_FCT_98_NAME              NO_FCTNAME
#define M_FCT_99                   99L
#define M_FCT_99_NAME              NO_FCTNAME
#define M_FCT_100                  100L
#define M_FCT_100_NAME             NO_FCTNAME
#define M_FCT_101                  101L
#define M_FCT_101_NAME             NO_FCTNAME
#define M_FCT_102                  102L
#define M_FCT_102_NAME             NO_FCTNAME
#define M_FCT_103                  103L
#define M_FCT_103_NAME             NO_FCTNAME
#define M_FCT_104                  104L
#define M_FCT_104_NAME             NO_FCTNAME
#define M_FCT_105                  105L
#define M_FCT_105_NAME             NO_FCTNAME
#define M_FCT_106                  106L
#define M_FCT_106_NAME             NO_FCTNAME
#define M_FCT_107                  107L
#define M_FCT_107_NAME             NO_FCTNAME
#define M_FCT_108                  108L
#define M_FCT_108_NAME             NO_FCTNAME
#define M_FCT_109                  109L
#define M_FCT_109_NAME             NO_FCTNAME
#define M_FCT_110                  110L
#define M_FCT_110_NAME             NO_FCTNAME
#define M_FCT_111                  111L
#define M_FCT_111_NAME             NO_FCTNAME
#define M_FCT_112                  112L
#define M_FCT_112_NAME             NO_FCTNAME
#define M_FCT_113                  113L
#define M_FCT_113_NAME             NO_FCTNAME
#define M_FCT_114                  114L
#define M_FCT_114_NAME             NO_FCTNAME
#define M_FCT_115                  115L
#define M_FCT_115_NAME             NO_FCTNAME
#define M_FCT_116                  116L
#define M_FCT_116_NAME             NO_FCTNAME
#define M_FCT_117                  117L
#define M_FCT_117_NAME             NO_FCTNAME
#define M_FCT_118                  118L
#define M_FCT_118_NAME             NO_FCTNAME
#define M_FCT_119                  119L
#define M_FCT_119_NAME             NO_FCTNAME
#define M_FCT_120                  120L
#define M_FCT_120_NAME             NO_FCTNAME
#define M_FCT_121                  121L
#define M_FCT_121_NAME             NO_FCTNAME
#define M_FCT_122                  122L
#define M_FCT_122_NAME             NO_FCTNAME
#define M_FCT_123                  123L
#define M_FCT_123_NAME             NO_FCTNAME
#define M_FCT_124                  124L
#define M_FCT_124_NAME             NO_FCTNAME
#define M_FCT_125                  125L
#define M_FCT_125_NAME             NO_FCTNAME
#define M_FCT_126                  126L
#define M_FCT_126_NAME             NO_FCTNAME
#define M_FCT_127                  127L
#define M_FCT_127_NAME             NO_FCTNAME
#define M_FCT_128                  128L
#define M_FCT_128_NAME             NO_FCTNAME
#define M_FCT_129                  129L
#define M_FCT_129_NAME             NO_FCTNAME
#define M_FCT_130                  130L
#define M_FCT_130_NAME             NO_FCTNAME
#define M_FCT_131                  131L
#define M_FCT_131_NAME             NO_FCTNAME
#define M_FCT_132                  132L
#define M_FCT_132_NAME             NO_FCTNAME
#define M_FCT_133                  133L
#define M_FCT_133_NAME             NO_FCTNAME
#define M_FCT_134                  134L
#define M_FCT_134_NAME             NO_FCTNAME
#define M_FCT_135                  135L
#define M_FCT_135_NAME             NO_FCTNAME
#define M_FCT_136                  136L
#define M_FCT_136_NAME             NO_FCTNAME
#define M_FCT_137                  137L
#define M_FCT_137_NAME             NO_FCTNAME
#define M_FCT_138                  138L
#define M_FCT_138_NAME             NO_FCTNAME
#define M_FCT_139                  139L
#define M_FCT_139_NAME             NO_FCTNAME
#define M_FCT_140                  140L
#define M_FCT_140_NAME             NO_FCTNAME
#define M_FCT_141                  141L
#define M_FCT_141_NAME             NO_FCTNAME
#define M_FCT_142                  142L
#define M_FCT_142_NAME             NO_FCTNAME
#define M_FCT_143                  143L
#define M_FCT_143_NAME             NO_FCTNAME
#define M_FCT_144                  144L
#define M_FCT_144_NAME             NO_FCTNAME
#define M_FCT_145                  145L
#define M_FCT_145_NAME             NO_FCTNAME
#define M_FCT_146                  146L
#define M_FCT_146_NAME             NO_FCTNAME
#define M_FCT_147                  147L
#define M_FCT_147_NAME             NO_FCTNAME
#define M_FCT_148                  148L
#define M_FCT_148_NAME             NO_FCTNAME
#define M_FCT_149                  149L
#define M_FCT_149_NAME             NO_FCTNAME
#define M_FCT_150                  150L
#define M_FCT_150_NAME             NO_FCTNAME
#define M_FCT_151                  151L
#define M_FCT_151_NAME             NO_FCTNAME
#define M_FCT_152                  152L
#define M_FCT_152_NAME             NO_FCTNAME
#define M_FCT_153                  153L
#define M_FCT_153_NAME             NO_FCTNAME
#define M_FCT_154                  154L
#define M_FCT_154_NAME             NO_FCTNAME
#define M_FCT_155                  155L
#define M_FCT_155_NAME             NO_FCTNAME
#define M_FCT_156                  156L
#define M_FCT_156_NAME             NO_FCTNAME
#define M_FCT_157                  157L
#define M_FCT_157_NAME             NO_FCTNAME
#define M_FCT_158                  158L
#define M_FCT_158_NAME             NO_FCTNAME
#define M_FCT_159                  159L
#define M_FCT_159_NAME             NO_FCTNAME
#define M_FCT_160                  160L
#define M_FCT_160_NAME             NO_FCTNAME
#define M_MIMENDOPCODE             160L

/* MGRA */
#define M_MGRASTARTOPCODE          161L
#define M_MGRAARC                  161L
#define M_FCT_161_NAME             MIL_TEXT("MgraArc")
#define M_MGRAARCFILL              162L
#define M_FCT_162_NAME             MIL_TEXT("MgraArcFill")
#define M_MGRABACKCOLOR            163L
#define M_FCT_163_NAME             MIL_TEXT("MgraBackColor")
#define M_MGRAFILL                 164L
#define M_FCT_164_NAME             MIL_TEXT("MgraFill")
#define M_MGRACLEAR                165L
#define M_FCT_165_NAME             MIL_TEXT("MgraClear")
#define M_MGRACOLOR                166L
#define M_FCT_166_NAME             MIL_TEXT("MgraColor")
#define M_MGRADOT                  167L
#define M_FCT_167_NAME             MIL_TEXT("MgraDot")
#define M_MGRAFONT                 168L
#define M_FCT_168_NAME             MIL_TEXT("MgraFont")
#define M_MGRAFONTSCALE            169L
#define M_FCT_169_NAME             MIL_TEXT("MgraFontScale")
#define M_MGRAINQUIRE              170L
#define M_FCT_170_NAME             MIL_TEXT("MgraInquire")
#define M_MGRALINE                 171L
#define M_FCT_171_NAME             MIL_TEXT("MgraLine")
#define M_MGRARECT                 172L
#define M_FCT_172_NAME             MIL_TEXT("MgraRect")
#define M_MGRARECTFILL             173L
#define M_FCT_173_NAME             MIL_TEXT("MgraRectFill")
#define M_MGRATEXT                 174L
#define M_FCT_174_NAME             MIL_TEXT("MgraText")
#define M_MGRAALLOC                175L
#define M_FCT_175_NAME             MIL_TEXT("MgraAlloc")
#define M_MGRAFREE                 176L
#define M_FCT_176_NAME             MIL_TEXT("MgraFree")
#define M_MGRACONTROL              177L
#define M_FCT_177_NAME             MIL_TEXT("MgraControl")
#define M_FCT_178                  178L
#define M_FCT_178_NAME             NO_FCTNAME
#define M_FCT_179                  179L
#define M_FCT_179_NAME             NO_FCTNAME
#define M_FCT_180                  180L
#define M_FCT_180_NAME             NO_FCTNAME
#define M_FCT_181                  181L
#define M_FCT_181_NAME             NO_FCTNAME
#define M_FCT_182                  182L
#define M_FCT_182_NAME             NO_FCTNAME
#define M_FCT_183                  183L
#define M_FCT_183_NAME             NO_FCTNAME
#define M_FCT_184                  184L
#define M_FCT_184_NAME             NO_FCTNAME
#define M_FCT_185                  185L
#define M_FCT_185_NAME             NO_FCTNAME
#define M_FCT_186                  186L
#define M_FCT_186_NAME             NO_FCTNAME
#define M_FCT_187                  187L
#define M_FCT_187_NAME             NO_FCTNAME
#define M_FCT_188                  188L
#define M_FCT_188_NAME             NO_FCTNAME
#define M_MGRAENDOPCODE            188L

/* MGEN */

#define M_MGENSTARTOPCODE          189L
#define M_MGENLUTFUNCTION          189L
#define M_FCT_189_NAME             MIL_TEXT("MgenLutFunction")
#define M_MGENLUTRAMP              190L
#define M_FCT_190_NAME             MIL_TEXT("MgenLutRamp")
#define M_MGENWARPPARAMETER        191L
#define M_FCT_191_NAME             MIL_TEXT("MgenWarpParameter")
#define M_FCT_192                  192L
#define M_FCT_192_NAME             NO_FCTNAME
#define M_FCT_193                  193L
#define M_FCT_193_NAME             NO_FCTNAME
#define M_FCT_194                  194L
#define M_FCT_194_NAME             NO_FCTNAME
#define M_FCT_195                  195L
#define M_FCT_195_NAME             NO_FCTNAME
#define M_FCT_196                  196L
#define M_FCT_196_NAME             NO_FCTNAME
#define M_FCT_197                  197L
#define M_FCT_197_NAME             NO_FCTNAME
#define M_FCT_198                  198L
#define M_FCT_198_NAME             NO_FCTNAME
#define M_FCT_199                  199L
#define M_FCT_199_NAME             NO_FCTNAME
#define M_FCT_200                  200L
#define M_FCT_200_NAME             NO_FCTNAME
#define M_FCT_201                  201L
#define M_FCT_201_NAME             NO_FCTNAME
#define M_FCT_202                  202L
#define M_FCT_202_NAME             NO_FCTNAME
#define M_MGENENDOPCODE            202L

/* MBUF */

#define M_MBUFSTARTOPCODE          203L
#define M_MBUFALLOC1D              203L
#define M_FCT_203_NAME             MIL_TEXT("MbufAlloc1d")
#define M_MBUFALLOC2D              204L
#define M_FCT_204_NAME             MIL_TEXT("MbufAlloc2d")
#define M_MBUFATTRIBUTE            205L
#define M_FCT_205_NAME             MIL_TEXT("MbufAttribute")
#define M_MBUFCHILD1D              206L
#define M_FCT_206_NAME             MIL_TEXT("MbufChild1d")
#define M_MBUFCHILD2D              207L
#define M_FCT_207_NAME             MIL_TEXT("MbufChild2d")
#define M_MBUFCONTROLNEIGHBORHOOD  208L
#define M_FCT_208_NAME             MIL_TEXT("MbufControlNeighborhood")
#define M_MBUFCOPY                 209L
#define M_FCT_209_NAME             MIL_TEXT("MbufCopy")
#define M_MBUFDISKINQUIRE          210L
#define M_FCT_210_NAME             MIL_TEXT("MbufDiskInquire")
#define M_MBUFEXPORT               211L
#define M_FCT_211_NAME             MIL_TEXT("MbufExport")
#define M_MBUFFREE                 212L
#define M_FCT_212_NAME             MIL_TEXT("MbufFree")
#define M_MBUFGET1D                213L
#define M_FCT_213_NAME             MIL_TEXT("MbufGet1d")
#define M_MBUFGET2D                214L
#define M_FCT_214_NAME             MIL_TEXT("MbufGet2d")
#define M_MBUFIMPORT               215L
#define M_FCT_215_NAME             MIL_TEXT("MbufImport")
#define M_MBUFINQUIRE              216L
#define M_FCT_216_NAME             MIL_TEXT("MbufInquire")
#define M_MBUFLOAD                 217L
#define M_FCT_217_NAME             MIL_TEXT("MbufLoad")
#define M_MBUFRESTORE              218L
#define M_FCT_218_NAME             MIL_TEXT("MbufRestore")
#define M_MBUFSAVE                 219L
#define M_FCT_219_NAME             MIL_TEXT("MbufSave")
#define M_MBUFPUT1D                220L
#define M_FCT_220_NAME             MIL_TEXT("MbufPut1d")
#define M_MBUFPUT2D                221L
#define M_FCT_221_NAME             MIL_TEXT("MbufPut2d")
#define M_MBUFPUT                  222L
#define M_FCT_222_NAME             MIL_TEXT("MbufPut")
#define M_MBUFCLEAR                223L
#define M_FCT_223_NAME             MIL_TEXT("MbufClear")
#define M_MBUFCOPYCLIP             224L
#define M_FCT_224_NAME             MIL_TEXT("MbufCopyClip")
#define M_MBUFCOPYCOND             225L
#define M_FCT_225_NAME             MIL_TEXT("MbufCopyCond")
#define M_MBUFCOPYMASK             226L
#define M_FCT_226_NAME             MIL_TEXT("MbufCopyMask")
#define M_MBUFGET                  227L
#define M_FCT_227_NAME             MIL_TEXT("MbufGet")
#define M_MBUFCHILDCOLOR           228L
#define M_FCT_228_NAME             MIL_TEXT("MbufChildColor")
#define M_MBUFALLOCCOLOR           229L
#define M_FCT_229_NAME             MIL_TEXT("MbufAllocColor")
#define M_MBUFGETCOLOR             230L
#define M_FCT_230_NAME             MIL_TEXT("MbufGetColor")
#define M_MBUFPUTCOLOR             231L
#define M_FCT_231_NAME             MIL_TEXT("MbufPutColor")
#define M_MBUFCOPYCOLOR            232L
#define M_FCT_232_NAME             MIL_TEXT("MbufCopyColor")
#define M_MBUFCONTROL              233L
#define M_FCT_233_NAME             MIL_TEXT("MbufControl")
#define M_MBUFRESIZE               234L
#define M_FCT_234_NAME             MIL_TEXT("MbufResize")
#define M_MBUFSHIFT                235L
#define M_FCT_235_NAME             MIL_TEXT("MbufShift")
#define M_MBUFGETLINE              236L
#define M_FCT_236_NAME             MIL_TEXT("MbufGetLine")
#define M_MBUFPUTLINE              237L
#define M_FCT_237_NAME             MIL_TEXT("MbufPutLine")
#define M_MBUFCLIP                 238L
#define M_FCT_238_NAME             MIL_TEXT("MbufClip")
#define M_MBUFLUTMAP               239L
#define M_FCT_239_NAME             MIL_TEXT("MbufLutMap")
#define M_MBUFMODIFIED2D           240L
#define M_FCT_240_NAME             MIL_TEXT("MbufModified2d")
#define M_MBUFCREATECOLOR          241L
#define M_FCT_241_NAME             MIL_TEXT("MbufCreateColor")
#define M_MBUFCHILDCOLOR2D         242L
#define M_FCT_242_NAME             MIL_TEXT("MbufChildColor2d")
#define M_MBUFPUTCOLOR2D           243L
#define M_FCT_243_NAME             MIL_TEXT("MbufPutColor2d")
#define M_MBUFGETCOLOR2D           244L
#define M_FCT_244_NAME             MIL_TEXT("MbufGetColor2d")
#define M_MBUFCOPYCOLOR2D          245L
#define M_FCT_245_NAME             MIL_TEXT("MbufCopyColor2d")
#define M_MBUFCREATE2D             246L
#define M_FCT_246_NAME             MIL_TEXT("MbufCreate2d")
#define M_MBUFCOMPRESSION          247L
#define M_FCT_247_NAME             MIL_TEXT("MbufCompression")
#define M_MBUFBINARIZE             248L
#define M_FCT_248_NAME             MIL_TEXT("MbufBinarize")
#define M_MBUFEXPORTSEQ            249L
#define M_FCT_249_NAME             MIL_TEXT("MbufExportSequence")
#define M_MBUFIMPORTSEQ            250L
#define M_FCT_250_NAME             MIL_TEXT("MbufImportSequence")
#define M_MBUFBAYER                251L
#define M_FCT_251_NAME             MIL_TEXT("MbufBayer")
#define M_MBUFHOOKFUNCTION         252L
#define M_FCT_252_NAME             MIL_TEXT("MbufHookFunction")
#define M_MBUFGETHOOKINFO          253L
#define M_FCT_253_NAME             MIL_TEXT("MbufGetHookInfo")
#define M_FCT_254                  254L
#define M_FCT_254_NAME             NO_FCTNAME
#define M_FCT_255                  255L
#define M_FCT_255_NAME             NO_FCTNAME
#define M_FCT_256                  256L
#define M_FCT_256_NAME             NO_FCTNAME
#define M_FCT_257                  257L
#define M_FCT_257_NAME             NO_FCTNAME
#define M_FCT_258                  258L
#define M_FCT_258_NAME             NO_FCTNAME
#define M_FCT_259                  259L
#define M_FCT_259_NAME             NO_FCTNAME
#define M_FCT_260                  260L
#define M_FCT_260_NAME             NO_FCTNAME
#define M_FCT_261                  261L
#define M_FCT_261_NAME             NO_FCTNAME
#define M_FCT_262                  262L
#define M_FCT_262_NAME             NO_FCTNAME
#define M_FCT_263                  263L
#define M_FCT_263_NAME             NO_FCTNAME
#define M_FCT_264                  264L
#define M_FCT_264_NAME             NO_FCTNAME
#define M_FCT_265                  265L
#define M_FCT_265_NAME             NO_FCTNAME
#define M_FCT_266                  266L
#define M_FCT_266_NAME             NO_FCTNAME
#define M_FCT_267                  267L
#define M_FCT_267_NAME             NO_FCTNAME
#define M_FCT_268                  268L
#define M_FCT_268_NAME             NO_FCTNAME
#define M_FCT_269                  269L
#define M_FCT_269_NAME             NO_FCTNAME
#define M_FCT_270                  270L
#define M_FCT_270_NAME             NO_FCTNAME
#define M_FCT_271                  271L
#define M_FCT_271_NAME             NO_FCTNAME
#define M_FCT_272                  272L
#define M_FCT_272_NAME             NO_FCTNAME
#define M_FCT_273                  273L
#define M_FCT_273_NAME             NO_FCTNAME
#define M_FCT_274                  274L
#define M_FCT_274_NAME             NO_FCTNAME
#define M_FCT_275                  275L
#define M_FCT_275_NAME             NO_FCTNAME
#define M_FCT_276                  276L
#define M_FCT_276_NAME             NO_FCTNAME
#define M_FCT_277                  277L
#define M_FCT_277_NAME             NO_FCTNAME
#define M_FCT_278                  278L
#define M_FCT_278_NAME             NO_FCTNAME
#define M_FCT_279                  279L
#define M_FCT_279_NAME             NO_FCTNAME
#define M_FCT_280                  280L
#define M_FCT_280_NAME             NO_FCTNAME
#define M_FCT_281                  281L
#define M_FCT_281_NAME             NO_FCTNAME
#define M_FCT_282                  282L
#define M_FCT_282_NAME             NO_FCTNAME
#define M_FCT_283                  283L
#define M_FCT_283_NAME             NO_FCTNAME
#define M_FCT_284                  284L
#define M_FCT_284_NAME             NO_FCTNAME
#define M_FCT_285                  285L
#define M_FCT_285_NAME             NO_FCTNAME
#define M_FCT_286                  286L
#define M_FCT_286_NAME             NO_FCTNAME
#define M_FCT_287                  287L
#define M_FCT_287_NAME             NO_FCTNAME
#define M_FCT_288                  288L
#define M_FCT_288_NAME             NO_FCTNAME
#define M_FCT_289                  289L
#define M_FCT_289_NAME             NO_FCTNAME
#define M_FCT_290                  290L
#define M_FCT_290_NAME             NO_FCTNAME
#define M_FCT_291                  291L
#define M_FCT_291_NAME             NO_FCTNAME
#define M_FCT_292                  292L
#define M_FCT_292_NAME             NO_FCTNAME
#define M_FCT_293                  293L
#define M_FCT_293_NAME             NO_FCTNAME
#define M_FCT_294                  294L
#define M_FCT_294_NAME             NO_FCTNAME
#define M_FCT_295                  295L
#define M_FCT_295_NAME             NO_FCTNAME
#define M_FCT_296                  296L
#define M_FCT_296_NAME             NO_FCTNAME
#define M_FCT_297                  297L
#define M_FCT_297_NAME             NO_FCTNAME
#define M_FCT_298                  298L
#define M_FCT_298_NAME             NO_FCTNAME
#define M_FCT_299                  299L
#define M_FCT_299_NAME             NO_FCTNAME
#define M_FCT_300                  300L
#define M_FCT_300_NAME             NO_FCTNAME
#define M_FCT_301                  301L
#define M_FCT_301_NAME             NO_FCTNAME
#define M_FCT_302                  302L
#define M_FCT_302_NAME             NO_FCTNAME
#define M_FCT_303                  303L
#define M_FCT_303_NAME             NO_FCTNAME
#define M_FCT_304                  304L
#define M_FCT_304_NAME             NO_FCTNAME
#define M_FCT_305                  305L
#define M_FCT_305_NAME             NO_FCTNAME
#define M_FCT_306                  306L
#define M_FCT_306_NAME             NO_FCTNAME
#define M_FCT_307                  307L
#define M_FCT_307_NAME             NO_FCTNAME
#define M_FCT_308                  308L
#define M_FCT_308_NAME             NO_FCTNAME
#define M_FCT_309                  309L
#define M_FCT_309_NAME             NO_FCTNAME
#define M_FCT_310                  310L
#define M_FCT_310_NAME             NO_FCTNAME
#define M_FCT_311                  311L
#define M_FCT_311_NAME             NO_FCTNAME
#define M_FCT_312                  312L
#define M_FCT_312_NAME             NO_FCTNAME
#define M_FCT_313                  313L
#define M_FCT_313_NAME             NO_FCTNAME
#define M_FCT_314                  314L
#define M_FCT_314_NAME             NO_FCTNAME
#define M_FCT_315                  315L
#define M_FCT_315_NAME             NO_FCTNAME
#define M_FCT_316                  316L
#define M_FCT_316_NAME             NO_FCTNAME
#define M_FCT_317                  317L
#define M_FCT_317_NAME             NO_FCTNAME
#define M_FCT_318                  318L
#define M_FCT_318_NAME             NO_FCTNAME
#define M_FCT_319                  319L
#define M_FCT_319_NAME             NO_FCTNAME
#define M_FCT_320                  320L
#define M_FCT_320_NAME             NO_FCTNAME
#define M_FCT_321                  321L
#define M_FCT_321_NAME             NO_FCTNAME
#define M_FCT_322                  322L
#define M_FCT_322_NAME             NO_FCTNAME
#define M_FCT_323                  323L
#define M_FCT_323_NAME             NO_FCTNAME
#define M_FCT_324                  324L
#define M_FCT_324_NAME             NO_FCTNAME
#define M_FCT_325                  325L
#define M_FCT_325_NAME             NO_FCTNAME
#define M_FCT_326                  326L
#define M_FCT_326_NAME             NO_FCTNAME
#define M_FCT_327                  327L
#define M_FCT_327_NAME             NO_FCTNAME
#define M_FCT_328                  328L
#define M_FCT_328_NAME             NO_FCTNAME
#define M_FCT_329                  329L
#define M_FCT_329_NAME             NO_FCTNAME
#define M_FCT_330                  330L
#define M_FCT_330_NAME             NO_FCTNAME
#define M_FCT_331                  331L
#define M_FCT_331_NAME             NO_FCTNAME
#define M_FCT_332                  332L
#define M_FCT_332_NAME             NO_FCTNAME
#define M_FCT_333                  333L
#define M_FCT_333_NAME             NO_FCTNAME
#define M_FCT_334                  334L
#define M_FCT_334_NAME             NO_FCTNAME
#define M_FCT_335                  335L
#define M_FCT_335_NAME             NO_FCTNAME
#define M_FCT_336                  336L
#define M_FCT_336_NAME             NO_FCTNAME
#define M_FCT_337                  337L
#define M_FCT_337_NAME             NO_FCTNAME
#define M_FCT_338                  338L
#define M_FCT_338_NAME             NO_FCTNAME
#define M_FCT_339                  339L
#define M_FCT_339_NAME             NO_FCTNAME
#define M_FCT_340                  340L
#define M_FCT_340_NAME             NO_FCTNAME
#define M_FCT_341                  341L
#define M_FCT_341_NAME             NO_FCTNAME
#define M_FCT_342                  342L
#define M_FCT_342_NAME             NO_FCTNAME
#define M_FCT_343                  343L
#define M_FCT_343_NAME             NO_FCTNAME
#define M_FCT_344                  344L
#define M_FCT_344_NAME             NO_FCTNAME
#define M_FCT_345                  345L
#define M_FCT_345_NAME             NO_FCTNAME
#define M_FCT_346                  346L
#define M_FCT_346_NAME             NO_FCTNAME
#define M_FCT_347                  347L
#define M_FCT_347_NAME             NO_FCTNAME
#define M_FCT_348                  348L
#define M_FCT_348_NAME             NO_FCTNAME
#define M_FCT_349                  349L
#define M_FCT_349_NAME             NO_FCTNAME
#define M_FCT_350                  350L
#define M_FCT_350_NAME             NO_FCTNAME
#define M_FCT_351                  351L
#define M_FCT_351_NAME             NO_FCTNAME
#define M_MBUFENDOPCODE            351L

/* MDIG */

#define M_MDIGSTARTOPCODE         352L
#define M_MDIGALLOC               352L
#define M_FCT_352_NAME            MIL_TEXT("MdigAlloc")
#define M_MDIGCHANNEL             353L
#define M_FCT_353_NAME            MIL_TEXT("MdigChannel")
#define M_MDIGFREE                354L
#define M_FCT_354_NAME            MIL_TEXT("MdigFree")
#define M_MDIGGRAB                355L
#define M_FCT_355_NAME            MIL_TEXT("MdigGrab")
#define M_MDIGINQUIRE             356L
#define M_FCT_356_NAME            MIL_TEXT("MdigInquire")
#define M_MDIGLUT                 357L
#define M_FCT_357_NAME            MIL_TEXT("MdigLut")
#define M_MDIGREFERENCE           358L
#define M_FCT_358_NAME            MIL_TEXT("MdigReference")
#define M_MDIGGRABCONTINUOUS      359L
#define M_FCT_359_NAME            MIL_TEXT("MdigGrabContinuous")
#define M_MDIGHALT                360L
#define M_FCT_360_NAME            MIL_TEXT("MdigHalt")
#define M_MDIGCONTROL             361L
#define M_FCT_361_NAME            MIL_TEXT("MdigControl")
#define M_MDIGGRABWAIT            362L
#define M_FCT_362_NAME            MIL_TEXT("MdigGrabWait")
#define M_MDIGAVERAGE             363L
#define M_FCT_363_NAME            MIL_TEXT("MdigAverage")
#define M_MDIGHOOKFUNCTION        364L
#define M_FCT_364_NAME            MIL_TEXT("MdigHookFunction")
#define M_MDISPHOOKFUNCTION       365L
#define M_FCT_365_NAME            MIL_TEXT("MdispHookFunction")
#define M_MDIGFOCUS               366L
#define M_FCT_366_NAME            MIL_TEXT("MdigFocus")
#define M_FCT_367                 367L
#define M_FCT_367_NAME            NO_FCTNAME
#define M_FCT_368                 368L
#define M_FCT_368_NAME            NO_FCTNAME
#define M_FCT_369                 369L
#define M_FCT_369_NAME            NO_FCTNAME
#define M_FCT_370                 370L
#define M_FCT_370_NAME            NO_FCTNAME
#define M_FCT_371                 371L
#define M_FCT_371_NAME            NO_FCTNAME
#define M_FCT_372                 372L
#define M_FCT_372_NAME            NO_FCTNAME
#define M_FCT_373                 373L
#define M_FCT_373_NAME            NO_FCTNAME
#define M_FCT_374                 374L
#define M_FCT_374_NAME            NO_FCTNAME
#define M_FCT_375                 375L
#define M_FCT_375_NAME            NO_FCTNAME
#define M_FCT_376                 376L
#define M_FCT_376_NAME            NO_FCTNAME
#define M_FCT_377                 377L
#define M_FCT_377_NAME            NO_FCTNAME
#define M_MDIGENDOPCODE           377L

/* MDISP */

#define M_MDISPSTARTOPCODE        378L
#define M_MDISPDESELECT           378L
#define M_FCT_378_NAME            MIL_TEXT("MdispDeselect")
#define M_MDISPINQUIRE            379L
#define M_FCT_379_NAME            MIL_TEXT("MdispInquire")
#define M_MDISPLUT                380L
#define M_FCT_380_NAME            MIL_TEXT("MdispLut")
#define M_MDISPPAN                381L
#define M_FCT_381_NAME            MIL_TEXT("MdispPan")
#define M_MDISPSELECT             382L
#define M_FCT_382_NAME            MIL_TEXT("MdispSelect")
#define M_MDISPZOOM               383L
#define M_FCT_383_NAME            MIL_TEXT("MdispZoom")
#define M_MDISPALLOC              384L
#define M_FCT_384_NAME            MIL_TEXT("MdispAlloc")
#define M_MDISPFREE               385L
#define M_FCT_385_NAME            MIL_TEXT("MdispFree")
#define M_MDISPOVERLAYKEY         386L
#define M_FCT_386_NAME            MIL_TEXT("MdispOverlayKey")
#define M_MDISPSELECTWINDOW       387L
#define M_FCT_387_NAME            MIL_TEXT("MdispSelectWindow")
#define M_MDISPCONTROL            388L
#define M_FCT_388_NAME            MIL_TEXT("MdispControl")
#define M_FCT_389                 389L
#define M_FCT_389_NAME            NO_FCTNAME
#define M_FCT_390                 390L
#define M_FCT_390_NAME            NO_FCTNAME
#define M_FCT_391                 391L
#define M_FCT_391_NAME            NO_FCTNAME
#define M_FCT_392                 392L
#define M_FCT_392_NAME            NO_FCTNAME
#define M_FCT_393                 393L
#define M_FCT_393_NAME            NO_FCTNAME
#define M_FCT_394                 394L
#define M_FCT_394_NAME            NO_FCTNAME
#define M_FCT_395                 395L
#define M_FCT_395_NAME            NO_FCTNAME
#define M_FCT_396                 396L
#define M_FCT_396_NAME            NO_FCTNAME
#define M_FCT_397                 397L
#define M_FCT_397_NAME            NO_FCTNAME
#define M_FCT_398                 398L
#define M_FCT_398_NAME            NO_FCTNAME
#define M_MDISPENDOPCODE          398L

/* MSYS */

#define M_MSYSSTARTOPCODE         399L
#define M_MSYSALLOC               399L
#define M_FCT_399_NAME            MIL_TEXT("MsysAlloc")
#define M_MSYSFREE                400L
#define M_FCT_400_NAME            MIL_TEXT("MsysFree")
#define M_MSYSINQUIRE             401L
#define M_FCT_401_NAME            MIL_TEXT("MsysInquire")
#define M_MSYSCONTROL             402L
#define M_FCT_402_NAME            MIL_TEXT("MsysControl")
#define M_MSYSCONFIGACCESS        403L
#define M_FCT_403_NAME            MIL_TEXT("MsysConfigAccess")
#define M_MSYSHOOKFCT             404L
#define M_FCT_404_NAME            MIL_TEXT("MsysHookFunction")
#define M_FCT_405                 405L
#define M_FCT_405_NAME            NO_FCTNAME
#define M_FCT_406                 406L
#define M_FCT_406_NAME            NO_FCTNAME
#define M_FCT_407                 407L
#define M_FCT_407_NAME            NO_FCTNAME
#define M_FCT_408                 408L
#define M_FCT_408_NAME            NO_FCTNAME
#define M_FCT_409                 409L
#define M_FCT_409_NAME            NO_FCTNAME
#define M_FCT_410                 410L
#define M_FCT_410_NAME            NO_FCTNAME
#define M_FCT_411                 411L
#define M_FCT_411_NAME            NO_FCTNAME
#define M_FCT_412                 412L
#define M_FCT_412_NAME            NO_FCTNAME
#define M_FCT_413                 413L
#define M_FCT_413_NAME            NO_FCTNAME
#define M_FCT_414                 414L
#define M_FCT_414_NAME            NO_FCTNAME
#define M_FCT_415                 415L
#define M_FCT_415_NAME            NO_FCTNAME
#define M_FCT_416                 416L
#define M_FCT_416_NAME            NO_FCTNAME
#define M_MSYSENDOPCODE           416L

/* MAPP */

#define M_MAPPSTARTOPCODE         417L
#define M_MAPPALLOC               417L
#define M_FCT_417_NAME            MIL_TEXT("MappAlloc")
#define M_MAPPCONTROL             418L
#define M_FCT_418_NAME            MIL_TEXT("MappControl")
#define M_MAPPFREE                419L
#define M_FCT_419_NAME            MIL_TEXT("MappFree")
#define M_MAPPGETERROR            420L
#define M_FCT_420_NAME            MIL_TEXT("MappGetError")
#define M_MAPPHOOKFUNCTION        421L
#define M_FCT_421_NAME            MIL_TEXT("MappHookFunction")
#define M_MAPPGETHOOKINFO         422L
#define M_FCT_422_NAME            MIL_TEXT("MappGetHookInfo")
#define M_MAPPINQUIRE             423L
#define M_FCT_423_NAME            MIL_TEXT("MappInquire")
#define M_MAPPMODIFY              424L
#define M_FCT_424_NAME            MIL_TEXT("MappModify")
#define M_MAPPGETDEVICETYPE       425L
#define M_FCT_425_NAME            MIL_TEXT("MappGetDeviceType")
#define M_MAPPTIMER               426L
#define M_FCT_426_NAME            MIL_TEXT("MappTimer")
#define M_MAPPCHILD               427L
#define M_FCT_427_NAME            MIL_TEXT("MappChild")
#define M_MAPPCONTROLTHREAD       428L
#define M_FCT_428_NAME            MIL_TEXT("MappControlThread")
#define M_ALLOCNONPAGEDMEMORY_CE  429L
#define M_FCT_429_NAME            MIL_TEXT("AllocNonPagedMemory") //This is for CE allocation
#define M_MALLOC_CE               430L
#define M_FCT_430_NAME            MIL_TEXT("dlmalloc") //This is for CE allocation
#define M_FCT_431                 431L
#define M_FCT_431_NAME            NO_FCTNAME
#define M_FCT_432                 432L
#define M_FCT_432_NAME            NO_FCTNAME
#define M_FCT_433                 433L
#define M_FCT_433_NAME            NO_FCTNAME
#define M_FCT_434                 434L
#define M_FCT_434_NAME            NO_FCTNAME
#define M_FCT_435                 435L
#define M_FCT_435_NAME            NO_FCTNAME
#define M_FCT_436                 436L
#define M_FCT_436_NAME            NO_FCTNAME
#define M_FCT_437                 437L
#define M_FCT_437_NAME            NO_FCTNAME
#define M_FCT_438                 438L
#define M_FCT_438_NAME            NO_FCTNAME
#define M_FCT_439                 439L
#define M_FCT_439_NAME            NO_FCTNAME
#define M_FCT_440                 440L
#define M_FCT_440_NAME            NO_FCTNAME
#define M_MAPPENDOPCODE           440L

/* MFUNC */

#define M_MFUNCSTARTOPCODE        441L
#define M_MFUNC                   441L
#define M_FCT_441_NAME            NO_FCTNAME
#define M_FCT_442                 442L
#define M_FCT_442_NAME            NO_FCTNAME
#define M_FCT_443                 443L
#define M_FCT_443_NAME            NO_FCTNAME
#define M_FCT_444                 444L
#define M_FCT_444_NAME            NO_FCTNAME
#define M_MFUNCENDOPCODE          444L


#endif

