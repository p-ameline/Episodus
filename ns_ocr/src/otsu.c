/*
 the following code was send by Ryan Dibble <dibbler@umich.edu>
 
  The algorithm is very simple but works good hopefully.
 
  Compare the grayscale histogram with a mass density diagram:
  I think the algorithm is a kind of
  divide a body into two parts in a way that the mass 
  centers have the largest distance from each other,
  the function is weighted in a way that same masses have a advantage
  
  TODO:
    RGB: do the same with all colors (CMYG?) seperately 

    test: hardest case = two colors
       bbg: test done, using a two color gray file. Output:
       # OTSU: thresholdValue = 43 gmin=43 gmax=188

 my changes:
   - float -> double
   - debug option added (vvv & 1..2)
   - **image => *image,  &image[i][1] => &image[i*cols+1]

     Joerg.Schulenburg@physik.uni-magdeburg.de

 */

#include <stdio.h>
#include <string.h>

/*======================================================================*/
/* OTSU global thresholding routine                                     */
/*   takes a 2D unsigned char array pointer, number of rows, and        */
/*   number of cols in the array. returns the value of the threshold    */
/*   in addition to thresholding the image at that value                */ 
/*======================================================================*/
int
otsu (unsigned char *image, int rows, int cols, int vvv) {

  unsigned char *np;    // pointer to position in the image we are working with
  int thresholdValue=1; // value we will threshold at
  int ihist[256];       // image histogram

  int i, j, k;          // various counters
  int n, n1, n2, gmin, gmax;
  double m1, m2, sum, csum, fmax, sb;

  // zero out histogram ...
  memset(ihist, 0, sizeof(ihist));

  gmin=255; gmax=0;
  // generate the histogram
  for (i = 1; i < rows - 1; i++) {
    np = &image[i*cols+1];
    for (j = 1; j < cols - 1; j++) {
      ihist[*np]++;
      if(*np > gmax) gmax=*np;
      if(*np < gmin) gmin=*np;
      np++;
    }
  }

  // set up everything
  sum = csum = 0.0;
  n = 0;

  for (k = 0; k <= 255; k++) {
    sum += (double) k * (double) ihist[k];  /* x*f(x) mass moment */
    n   += ihist[k];                        /*  f(x)    mass      */
  }

  if (!n) {
    // if n has no value we have problems...
    thresholdValue = 1500;
    fprintf (stderr, "NOT NORMAL thresholdValue = %d\n", thresholdValue);
    return (thresholdValue);
  }

  // do the otsu global thresholding method

  fmax = -1.0;
  n1 = 0;
  for (k = 0; k < 255; k++) {
    n1 += ihist[k];
    if (!n1) { continue; }
    n2 = n - n1;
    if (n2 == 0) { break; }
    csum += (double) k *ihist[k];
    m1 = csum / n1;
    m2 = (sum - csum) / n2;
    sb = (double) n1 *(double) n2 *(m1 - m2) * (m1 - m2);
    /* bbg: note: can be optimized. */
    if (sb > fmax) {
      fmax = sb;
      thresholdValue = k;
    }
  }

  // at this point we have our thresholding value

  // debug code to display thresholding values
  if ( vvv & 1 )
  fprintf(stderr,"# OTSU: thresholdValue = %d gmin=%d gmax=%d\n",
     thresholdValue, gmin, gmax);

  // actually performs the thresholding of the image...
  // comment it out if you only want to know what value to threshold at...
  //  later: grayvalues should also be used, only rescaling threshold=160=0xA0

  /*

  if ( vvv & 2 )
  for (i = 0; i < rows; i++) {
    np = &image[i*cols+0];
    for (j = 0; j < cols; j++) {
      *np = (unsigned char) (*np > thresholdValue ?
         (160+(*np - thresholdValue)* 80/(gmax - thresholdValue + 1)) :
         (  0+(*np - gmin          )*160/(thresholdValue - gmin + 1)) );
      np++;
    }
  }

  */

  return(thresholdValue);
}
