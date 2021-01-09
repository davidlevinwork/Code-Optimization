#include <stdbool.h>

/* Those define are faster then use the original function or those mathematics calculation */
/* Calculate index (as the origin function) */
#define calcIndex(X, Y, Z) (((X) * (Z)) + (Y))
/* Mathematics calculation - more efficients  */
#define multiplicationBy9(X) (((X) << 3) + (X))
#define minusMultiplication(X) ((X) * (-1))
/* Compute min and max of two integers*/
#define min(X, Y) ((X) < (Y) ? (X) : (Y))
#define max(X, Y) ((X) > (Y) ? (X) : (Y))


typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;


/*
 * I have made some optimization in this function. I will gather all the information here:
 * (1) The object image->data is a pixel - so we will use it for more efficient access
 * (2) Below you can see the descriptions for each value that i set (for the purposes of optimization)
 * (3) Registers int are faster in access in memory - i used them for variables that are in often use during the func
 * (4) Explanation of *pixel object:
 *  -> firstPixel will hold the right specific pixel that we want to bloor
 *  -> secondPixel will hold the next pixel after firstPixel (in first line of kernel)
 *  -> thirdPixel will hold the next pixel after secondPixel (in first line of kernel)
 *  -> fourthPixel will hold the next pixel after thirdPixel (in second line of kernel - adding m for the size of pic)
 *  -> fifthPixel will hold the next pixel after fourthPixel (in second line of kernel - adding m+1 for the size of pic
 *     to the firstPixel or add 1 to the fourthPixel (the same...))
 *  -> So on.. the kernel will be:    firstPixel   | secondPixel | thirdPixel
 *                                    fourthPixel  | fifthPixel  | sixthPixel
 *                                    seventhPixel | eightPixel  | ninthPixel
 * (5) Following (4), the main purpose of this optimization is to save the "for in for" part in the original code:
 * as i described above, we can reach each part of the kernel matrix with arithmetic pointers operations.
 * (6) The function "memcpy" will replace "copyPixels" function that you gave us
 * (7) The values in the for loops saves us the needless calculation of max & min indexes
 */
void blurNoFilter()
{
    pixel *imageData = (pixel*)(image->data);
    int minPixel = sizeof(pixel) * m * n;
    pixel* src = malloc(minPixel);
    /* Instead copyPixels function */
    memcpy(src, imageData, minPixel);

    int i, j;
    /* values that we can calculate once and use as the "stop condition" in the for loops */
    /* We are subtracting 1 from each variable because we dont want to use pixels that they are out of bounds */
    int max_i = n - 1;
    int max_j = m - 1;
    /* will help us to get the right location in the picture - especially in detecting the lines */
    int matrixIndex = 0;
    /* Registers int are faster in access in memory */
    register int sumBlue, sumGreen, sumRed, calculateIndexReturn;

    for(i = 1 ; i < max_i; ++i){
        /* This calculation will help us to reach the right row in the kernel matrix */
        int sumIndex = ((i - 1) * m) - 1;
        /* For each iteration we increment by m - the given size of the matrix */
        matrixIndex = matrixIndex + m;
        for(j =  1 ; j < max_j; ++j){
            sumRed = 0, sumGreen = 0, sumBlue = 0;
            /* Get the right specific pixel location that we want to bloor */
            calculateIndexReturn = sumIndex + j;

            /* Arbitrary pixel - will hold the changes that we want to make */
            pixel final;
            /* Pointers are faster in access to memory (full description above) */
            pixel* firstPixel = &src[calculateIndexReturn];
            pixel* secondPixel = firstPixel + 1;
            pixel* thirdPixel = firstPixel + 2;
            pixel* fourthPixel = firstPixel + m;
            pixel* fifthPixel = fourthPixel + 1;
            pixel* sixthPixel = fourthPixel + 2;
            pixel* seventhPixel = fourthPixel + m;
            pixel* eightPixel = seventhPixel + 1;
            pixel* ninthPixel = seventhPixel + 2;

            /* Sum all RGB pixel values and then divide the sum by 9 */
            sumBlue += ((int)firstPixel->blue + (int)secondPixel->blue + (int)thirdPixel->blue + (int)fourthPixel->blue+
                    (int)fifthPixel->blue + (int)sixthPixel->blue + (int)seventhPixel->blue + (int)eightPixel->blue+
                    (int)ninthPixel->blue);
            final.blue = sumBlue / 9;
            sumGreen += ((int)firstPixel->green + (int)secondPixel->green + (int)thirdPixel->green +
                    (int)fourthPixel->green + (int)fifthPixel->green + (int)sixthPixel->green + (int)seventhPixel->green
                    + (int)eightPixel->green + (int)ninthPixel->green);
            final.green = sumGreen / 9;
            sumRed += ((int)firstPixel->red + (int)secondPixel->red + (int)thirdPixel->red + (int)fourthPixel->red +
                    (int)fifthPixel->red + (int)sixthPixel->red + (int)seventhPixel->red + (int)eightPixel->red +
                    (int)ninthPixel->red);
            final.red = sumRed / 9;

            /* Save all the changes and calculations that i described above */
            imageData[matrixIndex + j] = final;
        }
    }
    /* Free the dynamic allocation */
    free(src);
}

/*
 * I have made some optimization in this function. I will gather all the information here:
 * (1) The object image->data is a pixel - so we will use it for more efficient access
 * (2) Below you can see the descriptions for each value that i set (for the purposes of optimization)
 * (3) Registers int are faster in access in memory - i used them for variables that are in often use during the func
 * (4) Explanation of *pixel object:
 *  -> firstPixel will hold the right specific pixel that we want to bloor
 *  -> secondPixel will hold the next pixel after firstPixel (in first line of kernel)
 *  -> thirdPixel will hold the next pixel after secondPixel (in first line of kernel)
 *  -> fourthPixel will hold the next pixel after thirdPixel (in second line of kernel - adding m for the size of pic)
 *  -> fifthPixel will hold the next pixel after fourthPixel (in second line of kernel - adding m+1 for the size of pic
 *     to the firstPixel or add 1 to the fourthPixel (the same...))
 *  -> So on.. the kernel will be:    firstPixel   | secondPixel | thirdPixel
 *                                    fourthPixel  | fifthPixel  | sixthPixel
 *                                    seventhPixel | eightPixel  | ninthPixel
 * (5) Following (4), the main purpose of this optimization is to save the "for in for" part in the original code:
 * as i described above, we can reach each part of the kernel matrix with arithmetic pointers operations.
 * (6) The function "memcpy" will replace "copyPixels" function that you gave us
 * (7) The values in the for loops saves us the needless calculation of max & min indexes
 * (8) The variable tempIndex will help us to get to the right pixel in the kernel matrix: ((i - 1) * m) + (j-1)
 *  -> (i-1) : because we start the counting of i' from 1, we will need to subtract 1
 *  -> ((i-1)*m) : will get us to the right line in big matrix (picture). This total multiplication will bring us to
 *     the right line.
 *  -> ((i-1)*m)+(j-1) : because we start the counting of j' from 1, we will need to subtract 1. This addition to the
 *     the total equation will bring us to the right column.
 * (9) The 18 if statements are checking the exact things that the original code checked, besides two important changes:
 *  -> The min_row & min_col & max_row & max_col omitted, and i calculated directly the value of the higher and lower
 *     indexes.
 *  -> Following the first change, the approach to the src picture is directed, and saves us the calculatins later.
 */
void blurWithFilter()
{
    pixel *imageData = (pixel*)(image->data);
    int minPixel = sizeof(pixel) * m * n;
    pixel* src = malloc(minPixel);
    /* Instead copyPixels function */
    memcpy(src, imageData, minPixel);

    int i, j;
    int min_row, min_col, max_row, max_col;
    /* values that we can calculate once and use as the "stop condition" in the for loops */
    /* We are subtracting 1 from each variable because we dont want to use pixels that they are out of bounds */
    int max_i = n - 1;
    int max_j = m - 1;
    /* will help us to get the right location in the picture - especially in detecting the lines */
    int matrixIndex = 0;
    /* values from the original code */
    int min_intensity = -1; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
    int max_intensity = 766; // arbitrary value that is lower than minimum possible intensity, which is 0
    /* Registers int are faster in access in memory */
    register int sumBlue, sumGreen, sumRed, calculateIndexReturn;

    for(i = 1 ; i < max_i; ++i){
        /* This calculation will help us to reach the right row in the kernel matrix */
        int sumIndex = ((i - 1) * m) - 1;
        /* For each iteration we increment by m - the given size of the matrix */
        matrixIndex = matrixIndex + m;
        for(j =  1 ; j < max_j; ++j){
            sumRed = 0, sumGreen = 0, sumBlue = 0;
            /* Get the right specific pixel location that we want to bloor */
            calculateIndexReturn = sumIndex + j;

            /* Arbitrary pixel - will hold the changes that we want to make */
            pixel final;
            /* Pointers are faster in access to memory (full description above) */
            /* Sum all the values of the specific pixel */
            pixel* firstPixel = &src[calculateIndexReturn];
            int sumFirstPixel = (int)firstPixel->red + (int)firstPixel->blue + (int)firstPixel->green;
            pixel* secondPixel = firstPixel + 1;
            int sumSecondPixel = (int)secondPixel->red + (int)secondPixel->blue + (int)secondPixel->green;
            pixel* thirdPixel = firstPixel + 2;
            int sumThirdPixel = (int)thirdPixel->red + (int)thirdPixel->blue + (int)thirdPixel->green;
            pixel* fourthPixel = firstPixel + m;
            int sumFourthPixel = (int)fourthPixel->red + (int)fourthPixel->blue + (int)fourthPixel->green;
            pixel* fifthPixel = fourthPixel + 1;
            int sumFifthPixel = (int)fifthPixel->red + (int)fifthPixel->blue + (int)fifthPixel->green;
            pixel* sixthPixel = fourthPixel + 2;
            int sumSixthPixel = (int)sixthPixel->red + (int)sixthPixel->blue + (int)sixthPixel->green;
            pixel* seventhPixel = fourthPixel + m;
            int sumSeventhPixel = (int)seventhPixel->red + (int)seventhPixel->blue + (int)seventhPixel->green;
            pixel* eightPixel = seventhPixel + 1;
            int sumEightPixel = (int)eightPixel->red + (int)eightPixel->blue + (int)eightPixel->green;
            pixel* ninthPixel = seventhPixel + 2;
            int sumNinthPixel = (int)ninthPixel->red + (int)ninthPixel->blue + (int)ninthPixel->green;

            /* tempIndex holds the equation to get the right pixel in the kernel matrix (full description above) */
            int tempIndex = ((i - 1) * m) + (j-1);
            int lowerIndexValue;
            int higherIndexValue;

            /* lowerIndexValue & higherIndexValue will help us get the values of the specific pixel */
            lowerIndexValue = tempIndex;
            higherIndexValue = tempIndex;
            /* We will enter the first if statement always, so i initialize with it's values */
            min_intensity = sumFirstPixel;
            max_intensity = sumFirstPixel;

            /* 18 if statements (full description above) */
            if(sumSecondPixel <= min_intensity)
            {
                min_intensity = sumSecondPixel;
                lowerIndexValue = tempIndex + 1;
            }
            if(sumSecondPixel > max_intensity)
            {
                max_intensity = sumSecondPixel;
                higherIndexValue = tempIndex + 1;
            }
            if(sumThirdPixel <= min_intensity)
            {
                min_intensity = sumThirdPixel;
                lowerIndexValue = tempIndex + 2;
            }
            if(sumThirdPixel > max_intensity)
            {
                max_intensity = sumThirdPixel;
                higherIndexValue = tempIndex + 2;
            }
            if(sumFourthPixel <= min_intensity)
            {
                min_intensity = sumFourthPixel;
                lowerIndexValue = tempIndex + m;
            }
            if(sumFourthPixel > max_intensity)
            {
                max_intensity = sumFourthPixel;
                higherIndexValue = tempIndex + m;
            }
            if(sumFifthPixel <= min_intensity)
            {
                min_intensity = sumFifthPixel;
                lowerIndexValue = tempIndex + m + 1;
            }
            if(sumFifthPixel > max_intensity)
            {
                max_intensity = sumFifthPixel;
                higherIndexValue = tempIndex + m + 1;
            }
            if(sumSixthPixel <= min_intensity)
            {
                min_intensity = sumSixthPixel;
                lowerIndexValue = tempIndex + m + 2;
            }
            if(sumSixthPixel > max_intensity)
            {
                max_intensity = sumSixthPixel;
                higherIndexValue = tempIndex + m + 2;
            }
            if(sumSeventhPixel <= min_intensity)
            {
                min_intensity = sumSeventhPixel;
                lowerIndexValue = tempIndex + m + m;
            }
            if(sumSeventhPixel > max_intensity)
            {
                max_intensity = sumSeventhPixel;
                higherIndexValue = tempIndex + m + m;
            }
            if(sumEightPixel <= min_intensity)
            {
                min_intensity = sumEightPixel;
                lowerIndexValue = tempIndex + m + m + 1;
            }
            if(sumEightPixel > max_intensity)
            {
                max_intensity = sumEightPixel;
                higherIndexValue = tempIndex + m + m + 1;
            }
            if(sumNinthPixel <= min_intensity)
            {
                min_intensity = sumNinthPixel;
                lowerIndexValue = tempIndex + m + m + 2;
            }
            if(sumNinthPixel > max_intensity)
            {
                max_intensity = sumNinthPixel;
                higherIndexValue = tempIndex + m + m + 2;
            }

            /* The following lines will display totally 3 calculations: */
            /* 1. Sum all RGB pixel values */
            /* 2. Filter out min and max */
            /* 3. Divide the total sum by 7 */
            sumBlue += ((int)firstPixel->blue + (int)secondPixel->blue + (int)thirdPixel->blue + (int)fourthPixel->blue+
                    (int)fifthPixel->blue + (int)sixthPixel->blue + (int)seventhPixel->blue + (int)eightPixel->blue+
                    (int)ninthPixel->blue);
            sumBlue += minusMultiplication((int)src[lowerIndexValue].blue);
            sumBlue += minusMultiplication((int)src[higherIndexValue].blue);
            final.blue = sumBlue / 7;
            sumGreen += ((int)firstPixel->green + (int)secondPixel->green + (int)thirdPixel->green +
                    (int)fourthPixel->green + (int)fifthPixel->green + (int)sixthPixel->green + (int)seventhPixel->green
                    + (int)eightPixel->green + (int)ninthPixel->green);
            sumGreen += minusMultiplication((int)src[lowerIndexValue].green);
            sumGreen += minusMultiplication((int)src[higherIndexValue].green);
            final.green = sumGreen / 7;
            sumRed += ((int)firstPixel->red + (int)secondPixel->red + (int)thirdPixel->red + (int)fourthPixel->red +
                    (int)fifthPixel->red + (int)sixthPixel->red + (int)seventhPixel->red + (int)eightPixel->red +
                    (int)ninthPixel->red);
            sumRed += minusMultiplication((int)src[lowerIndexValue].red);
            sumRed += minusMultiplication((int)src[higherIndexValue].red);
            final.red = sumRed / 7;

            /* Save all the changes and calculations that i described above */
            imageData[matrixIndex + j] = final;
        }
    }
    /* Free the dynamic allocation */
    free(src);
}

/*
 * I have made some optimization in this function. I will gather all the information here:
 * (1) The object image->data is a pixel - so we will use it for more efficient access
 * (2) Below you can see the descriptions for each value that i set (for the purposes of optimization)
 * (3) Registers int are faster in access in memory - i used them for variables that are in often use during the func
 * (4) Explanation of *pixel object:
 *  -> firstPixel will hold the right specific pixel that we want to bloor
 *  -> secondPixel will hold the next pixel after firstPixel (in first line of kernel)
 *  -> thirdPixel will hold the next pixel after secondPixel (in first line of kernel)
 *  -> fourthPixel will hold the next pixel after thirdPixel (in second line of kernel - adding m for the size of pic)
 *  -> fifthPixel will hold the next pixel after fourthPixel (in second line of kernel - adding m+1 for the size of pic
 *     to the firstPixel or add 1 to the fourthPixel (the same...))
 *  -> So on.. the kernel will be:    firstPixel   | secondPixel | thirdPixel
 *                                    fourthPixel  | fifthPixel  | sixthPixel
 *                                    seventhPixel | eightPixel  | ninthPixel
 * (5) Following (4), the main purpose of this optimization is to save the "for in for" part in the original code:
 * as i described above, we can reach each part of the kernel matrix with arithmetic pointers operations.
 * (6) The function "memcpy" will replace "copyPixels" function that you gave us
 * (7) The values in the for loops saves us the needless calculation of max & min indexes
 */
void sharpen()
{
    pixel *imageData = (pixel*)(image->data);
    int minPixel = sizeof(pixel) * m * n;
    pixel* src = malloc(minPixel);
    /* Instead copyPixels function */
    memcpy(src, imageData, minPixel);

    int i, j;
    /* values that we can calculate once and use as the "stop condition" in the for loops */
    /* We are subtracting 1 from each variable because we dont want to use pixels that they are out of bounds */
    int max_i = n - 1;
    int max_j = m - 1;
    /* will help us to get the right location in the picture - especially in detecting the lines */
    int matrixIndex = 0;
    /* Registers int are faster in access in memory */
    register int sumBlue, sumGreen, sumRed, calculateIndexReturn;
    for (i = 1; i < max_i; ++i) {
        /* This calculation will help us to reach the right row in the kernel matrix */
        int sumIndex = ((i - 1) * m) - 1;
        /* For each iteration we increment by m - the given size of the matrix */
        matrixIndex = matrixIndex + m;
        for (j = 1; j < max_j; ++j) {
            sumRed = 0, sumGreen = 0, sumBlue = 0;
            /* Get the right specific pixel location that we want to bloor */
            calculateIndexReturn = sumIndex + j;

            /* Arbitrary pixel - will hold the changes that we want to make */
            pixel final;
            /* Pointers are faster in access to memory (full description above) */
            pixel *firstPixel = &src[calculateIndexReturn];
            pixel *secondPixel = firstPixel + 1;
            pixel *thirdPixel = firstPixel + 2;
            pixel *fourthPixel = firstPixel + m;
            pixel *fifthPixel = fourthPixel + 1;
            pixel *sixthPixel = fourthPixel + 2;
            pixel *seventhPixel = fourthPixel + m;
            pixel *eightPixel = seventhPixel + 1;
            pixel *ninthPixel = seventhPixel + 2;

            /* The following lines will display totally 4 calculations: */
            /* (all those calculations concentrates the equations from the original code, in efficient way) */
            /* 1. Multiply the center pixel by 9 (the fifthPixel, as i explained above in the illustration) */
            /* 2. Sum all the RGB values of all the pixels, besides the center pixel (fifthPixel) */
            /* 3. Multiply the sum we calculate (2.) by minus 1 */
            /* 4. Add the two values from 1. and 3. */
            sumRed = multiplicationBy9((int)fifthPixel->red) + minusMultiplication((int)firstPixel->red +
                    (int)secondPixel->red + (int)thirdPixel->red + (int)fourthPixel->red + (int)sixthPixel->red +
                    (int)seventhPixel->red + (int)eightPixel->red + (int)ninthPixel->red);
            sumGreen = multiplicationBy9((int)fifthPixel->green) + minusMultiplication((int)firstPixel->green +
                    (int)secondPixel->green + (int)thirdPixel->green + (int)fourthPixel->green + (int)sixthPixel->green+
                    (int)seventhPixel->green + (int)eightPixel->green + (int)ninthPixel->green);
            sumBlue = multiplicationBy9((int)fifthPixel->blue) + minusMultiplication((int)firstPixel->blue +
                    (int)secondPixel->blue + (int)thirdPixel->blue + (int)fourthPixel->blue +(int) sixthPixel->blue +
                    (int)seventhPixel->blue + (int)eightPixel->blue + (int) ninthPixel->blue);
            /* Truncate each pixel's color values to match the range [0,255] (from original code) */
            final.red = (unsigned char)min(max(sumRed, 0), 255);
            final.green = (unsigned char)min(max(sumGreen, 0), 255);
            final.blue = (unsigned char)min(max(sumBlue, 0), 255);

            /* Save all the changes and calculations that i described above */
            imageData[matrixIndex + j] = final;
        }
    }
    /* Free the dynamic allocation */
    free(src);
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {
    int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

    if (flag == '1') {
        // blur image
        blurNoFilter();

        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);

        // sharpen the resulting image
        sharpen();

        // write result image to file
        writeBMP(image, srcImgpName, sharpRsltImgName);
    } else {
        // apply extermum filtered kernel to blur image
        blurWithFilter();

        // write result image to file
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        sharpen();

        // write result image to file
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
    }
}