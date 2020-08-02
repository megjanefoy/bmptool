#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "bmplib.h"

/*
 * Title: Assignment 2
 * Semester: COP4338 - Spring 2021
 * Author Megan Jane Thompson
 * Panther ID: 3276301
 *
 * I affirm that the revisions of this program is entirely
 * my own work and none of it is the work of any other person.
 *
 * This program extracts options from the command line and
 * adjusts a bmp file accordingly.
 *
 */

int altered = 0;			//global boolen for if image was already altered


/* This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale,
	        PIXEL** new, int* newrows, int* newcols){
	int row, col;									//place holders
	*newrows = rows * scale;						//updates number of rows to scale
	*newcols = cols * scale;						//updated number of columns to scale

	if ((rows <= 0) || (cols <= 0)){
		return -1;
	}

	*new = (PIXEL*)malloc((*newrows)*(*newcols)*sizeof(PIXEL));		//memory size to hold new scale

	for (row=0; row < *newrows; row++){				//loop throughout entire image
		for (col=0; col < *newcols; col++) {
			PIXEL* o = original + (row/scale)*cols + (col/scale);
			PIXEL* n = (*new) + row*(*newcols) + col;
			*n = *o;
		}
	}

	altered = 1;									//updates altered status

	return 0;
}


/* This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
		   PIXEL** new, int* newrows, int* newcols){
	int row, col;									//place holders

	if ((rows <= 0) || (cols <= 0)){
		return -1;
	}

	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

	if(rotation % 180 == 0){						//updates rows and columns
		*newrows = rows;
		*newcols = cols;
	}
	else{
		*newrows = cols;
		*newcols = rows;
	}

	if(rotation > 0){								//clockwise rotation
		if (rotation > 360){
			do {
				rotation = rotation - 360;			//calculates rotation within 360 degrees
			}while(rotation > 360);
		}
	}
	else if(rotation < 0){							//counterclockwise rotation
		if (rotation < -360){
			do {
				rotation = rotation + 360;			//calculates rotation within 360 degrees
			}while(rotation < -360);
		}
	}

	if(rotation == 180 || rotation == -180){		//180 degree rotation
		for (row=0; row < rows; row++){
			for (col=0; col < cols; col++) {
				PIXEL* o = original + (row*cols) + col;
				PIXEL* n = (*new) + ((rows-1-row)*cols) + (cols-1-col);
				*n = *o;
			}
		}
	}
	else if(rotation == 90 || rotation == -270){	//90 or -270 rotation
		for (row=0; row < rows; row++){
			for (col=0; col < cols; col++) {
				PIXEL* o = original + (row*cols) + col;
				PIXEL* n = (*new) + ((cols-1-col)*rows) + row;
				*n = *o;
			}
		}
	}
	else if(rotation == -90 || rotation == 270){	//-90 or 270 rotation
		for (row=0; row < rows; row++){
			for (col=0; col < cols; col++) {
				PIXEL* o = original + (row*cols) + col;
				PIXEL* n = (*new) + (col*cols) + (rows-1-row);
				*n = *o;
			}
		}
	}
	else{											//360 or 0 rotation
		for (row=0; row < rows; row++){
			for (col=0; col < cols; col++) {
				PIXEL* o = original + (row*cols) + col;
				PIXEL* n = (*new) + (row*cols) + col;
				*n = *o;
			}
		}
	}

	altered = 1;									//updates altered status

	return 0;
}


/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int verticalflip (PIXEL *original, PIXEL **new, int rows, int cols){
	int row, col;									//place holders

	if ((rows <= 0) || (cols <= 0)){
		return -1;
	}

	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

	for (row=0; row < rows; row++){					//loop through entire image
		for (col=0; col < cols; col++) {
			PIXEL* o = original + row*cols + col;
			PIXEL* n = (*new) + (rows-1-row)*cols + col;
			*n = *o;
		}
	}

	altered = 1;									//updates altered status

	return 0;
}


/* This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols){
	int row, col;									//place holders

	if ((rows <= 0) || (cols <= 0)){
		return -1;
	}

	*new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

	for (row=0; row < rows; row++){					//loop throughout entire image
		for (col=0; col < cols; col++) {
			PIXEL* o = original + row*cols + col;
			PIXEL* n = (*new) + row*cols + (cols-1-col);
			*n = *o;
		}
	}

	altered = 1;									//updates altered status

	return 0;
}


int main(int argc, char ** argv){
	extern char *optarg;
	extern int optind;
	int c;
	int sflag=0, rflag=0, fflag=0, vflag=0, err=0;
	int scaleAmt, rotateAmt;
	int numRows, numCols;
	char *outFileName = NULL;
	char *inFileName = NULL;
	PIXEL *b, *nb;
	static char usage[] = "usage: bmptool [-s: scale | -r: degree | -f | -v ] [-o: output_file] [input_file]\n";


	if(argc < 2){
		printf("Try running the program with some arguments\n\t%sSomething like:\texample -s 2 -o outname.bmp inputname.bmp\n\t\texample -s 3 -r 90 -o outname.bmp inputname.bmp\n", usage);
		exit(0);
	}
	/*set the flags per command line prompt*/
	while ((c = getopt(argc, argv, "s:r:fvo:")) > 0){
		switch (c) {
		case 's':
			sflag = 1;
			scaleAmt = atoi(optarg);				//obtains scale amount
			break;
		case 'r':
			rflag = 1;
			rotateAmt = atoi(optarg);				//obtains rotation amount
			break;
		case 'v':
			vflag = 1;
			break;
		case 'f':
			fflag = 1;
			break;
		case 'o':
			outFileName = (char*)calloc(strlen(optarg)+1, sizeof(char));
			//failure allocating memory
			if (outFileName == NULL){
				printf("Error allocating memory for outFileName\n");
				exit(1);
			}
			//success allocating memory
			else{
				strcpy(outFileName, optarg);		//obtains the output file's filename
				strcat(outFileName, "\0");
			}
			break;
		case '?':
			err = 1;
			break;
		}
	}


	/*check for errors*/
	if((sflag == 1) && (scaleAmt < 1)){				//scale amount must be greater than 0
		printf("Option s requires a positive integer entry that is greater than 0. Please try again.\n");
		printf(usage);
		exit(1);
	}
	if((rflag == 1) && (rotateAmt % 90 != 0)){		//rotation amount must a multiple of 90
		printf("Option r requires an integer entry as a multiple of 90. Please try again.\n");
		printf(usage);
		exit(1);
	}
	if (err) {                                		//general error
		fprintf(stderr, "%s %s %s", "General Error.", usage, argv[0]);
		exit(1);
	}


	/*process arguments after the command-line options, may contain an input file name*/
	if (optind < argc){
		/*input file name included in command line*/
		inFileName = (char*)calloc(strlen((argv[optind]))+1, sizeof(char));
		//failure allocating memory
		if (inFileName == NULL){
			printf("Error allocating memory for inFileName\n");
			exit(1);
		}
		//success allocating memory
		else{
			strcpy(inFileName, argv[optind]);
			readFile(inFileName, &numRows, &numCols, &b);	//pass filename to readFile
		}
	}
	else {
		readFile(inFileName, &numRows, &numCols, &b);		//pass NULL to readFile
	}


	/*process command line options*/
	if(sflag == 1){											//scale image
		if (altered == 0){									//pass original photo
			enlarge(b, numRows, numCols, scaleAmt, &nb, &numRows, &numCols);
		}
		else{												//pass altered photo
			enlarge(nb, numRows, numCols, scaleAmt, &nb, &numRows, &numCols);
		}
	}
	if(rflag == 1){											//rotate image
		if(altered == 0){									//pass original photo
			rotate(b, numRows, numCols, rotateAmt, &nb, &numRows, &numCols);
		}
		else{												//pass altered photo
			rotate(nb, numRows, numCols, rotateAmt, &nb, &numRows, &numCols);
		}
	}
	if(vflag == 1){											//flip image vertically
		if(altered == 0){									//pass original photo
			verticalflip(b, &nb, numRows, numCols);
		}
		else{												//pass altered photo
			verticalflip(nb, &nb, numRows, numCols);
		}
	}
	if(fflag == 1){											//flip image horizontally
		if(altered == 0){									//pass original photo
			flip(b, &nb, numRows, numCols);
		}
		else{												//pass altered photo
			flip(nb, &nb, numRows, numCols);
		}
	}


	/*print out to either file or stdout*/
	writeFile(outFileName, numRows, numCols, nb);


	/*reclaim dynamic memory*/
	free(b);
	free(nb);
	return 0;

}


