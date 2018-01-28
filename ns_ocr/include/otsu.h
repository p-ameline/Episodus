/*

     Joerg.Schulenburg@physik.uni-magdeburg.de

 */


/*======================================================================*/
/* OTSU global thresholding routine                                     */
/*   takes a 2D unsigned char array pointer, number of rows, and        */
/*   number of cols in the array. returns the value of the threshold    */
/*   in addition to thresholding the image at that value                */ 
/*======================================================================*/
int
otsu (unsigned char *image, int rows, int cols, int vvv);

