#include <stdio.h>
#include <stdlib.h>

typedef struct sPIXEL {      // a single pixel
	unsigned char r, g, b;   // three color components
} PIXEL;


typedef struct sIMG {      // an image of PPM style
	char header[3];        // header, either P3 or P6
	int W, H;              // width and height of the image
	int level;             // intensity level of each color component
	PIXEL **PX;            // two-dimensional array for all the pixels
} IMG;


IMG *PPMin(char *inFile);				//read in ppm files
void PPMout(IMG *p1, char *outFile);	//print out ppm files
IMG *PPMcvt(IMG *p1, IMG *ee, IMG *nthu, int x1, int y1, int x2, int y2);  //convert the ppm into requested one


/****************************************************************************
 * Main function will call PPMin function to read in 3 PPM files and call   *
 * the PPMcvt function to convert the image into the requested format, and	*
 * then call the PPMout function to print out the PPM file.					*
 ****************************************************************************/

int main(int argc,char *argv[])
{
	IMG *p1,*ee,*nthu;							//pointers of IMGs

	p1 = PPMin(argv[argc-4]);					//p1 is ptr of pic1.ppm
	ee = PPMin(argv[argc-3]);					//ee is ptr of EE.ppm
	nthu = PPMin(argv[argc-2]);					//nthu is ptr of NTHU.ppm

#if 1 // only zhen
	p1 = PPMcvt(p1, ee, nthu,
			2715, (945 + 105),
			(2715 + 105), 945);
#else // all classmates
	p1 = PPMcvt(p1, ee, nthu,
			210, (711 + 1779),
			(210 + 3800), 711);
#endif

	PPMout(p1,argv[argc-1]);					//store as file named pic1_out1

	return 0;									//function ends
}

/****************************************************************************
 * This function opens the inFile, reads the image data and returns a 		*
 * pointer pointing to the newly created image data structure.				*
 ****************************************************************************/

IMG *PPMin(char *inFile)
{
	FILE *fin;									//ptr to read-in file
	IMG *picin;									//pointer to an IMG
	int i,j;									//to run loops

	fin = fopen(inFile,"r");					//open read-in file
	picin = (IMG *)malloc(sizeof(IMG));				//ask memory of IMG
	if (!picin) {
		fclose(fin);
		return NULL;
	}

	fscanf(fin,"%s", picin->header);				//read the header and store it
	fscanf(fin,"%d",&picin->W);					//read the width and store it
	fscanf(fin,"%d",&picin->H);					//read the height and store it
	fscanf(fin,"%d\n",&picin->level);			//read the level and store it


	printf("[%s]\n", inFile);
	printf("%s\n", picin->header);
	printf("W=%d\n", picin->W);
	printf("H=%d\n", picin->H);
	printf("level=%d\n\n", picin->level);

	picin->PX = (PIXEL **)malloc(picin->W*sizeof(PIXEL *));
												//ask memory of W PIXELs
	for (i=0; i<picin->W; i++)					//for each column
		picin->PX[i] = malloc(picin->H*sizeof(PIXEL *));
												//ask memory of H PIXELs

	for (j=0; j<picin->H; j++)					//loop for scanning all PIXELs
	{											//and storing them to correct
		for (i=0; i<picin->W; i++)				//places respectively
		{
			fscanf(fin,"%c",&picin->PX[i][j].r);
			fscanf(fin,"%c",&picin->PX[i][j].g);
			fscanf(fin,"%c",&picin->PX[i][j].b);
		}
	}
	fclose(fin);								//close the read in file

	return picin;								//return picin and end fuction
}

/****************************************************************************
 * This function writes the image pointed by p1 to the output file outFile.	*
 ****************************************************************************/

void PPMout(IMG *p1, char *outFile)
{
	FILE *fout;									//ptr to print-out FILE
	int i,j;									//to run loops

	fout = fopen(outFile,"w");					//open the file and write it

	fprintf(fout,"%s\n",p1->header);			//print the header of PPM
	fprintf(fout,"%d ",p1->W);					//print the width of PPM
	fprintf(fout,"%d\n",p1->H);					//print the height of PPM
	fprintf(fout,"%d\n",p1->level);				//print the level of PPM

	for (j=0; j<p1->H; j++)						//loop for printing out the
	{											//PIXELs from the right places
		for (i=0; i<p1->W; i++)					//respectively
		{
			fprintf(fout,"%c",p1->PX[i][j].r);
			fprintf(fout,"%c",p1->PX[i][j].g);
			fprintf(fout,"%c",p1->PX[i][j].b);
		}
	}
	fclose(fout);								//close the file
}

/****************************************************************************
 * This function processes the image pointed by p1 adds the logos and 		*
 * returns the new image as a result.										*
 ****************************************************************************/

/*
 * x1, y1: lower-left corner of the box
 * x2, y2: uper-right corner of the box
 */
IMG *PPMcvt(IMG *p1, IMG *ee, IMG *nthu, int x1, int y1, int x2, int y2)
{
	int i,j;								//to run loops
	unsigned char gray;						//calculate the value of gray PIXEL
	int place_W,place_H;					//calculate the place of EE & NTHU


	for (j=0; j<p1->H; j++)					//for all PIXELs, it should be
	{										//replaced by the result of the
		for (i=0; i<p1->W; i++)				//given formula
		{
			gray = p1->PX[i][j].r*0.2126 +	//calculate the value of gray PIXEL
				   p1->PX[i][j].g*0.7152 +
				   p1->PX[i][j].b*0.0722;
#if 0
			if ((i < 2715 || i > (2715 + 105)) || // width
					(j < 945 || j > (945 + 105))) { // heigh
				p1->PX[i][j].r = gray;			//all red green blue intensity
				p1->PX[i][j].g = gray;			//should be same as "gray"
				p1->PX[i][j].b = gray;
			}
#else
			if ((i < x1 || i > x2) ||     // width
					(j < y2 || j > y1)) { // heigh
				p1->PX[i][j].r = gray;			//all red green blue intensity
				p1->PX[i][j].g = gray;			//should be same as "gray"
				p1->PX[i][j].b = gray;
			} else if ((i == x1 &&  (j > y2 && j < y1)) ||  // left
					(i == x2 &&  (j > y2 && j < y1)) ||     // right
					(j == y2 &&  (i > x1 && i < x2)) ||     // top
					(j == y1 &&  (i > x1 && i < x2))) {     // bottom
				//printf("green: w:%d, h:%d\n", i, j);
				// green line
				p1->PX[i][j].r = 0;
				p1->PX[i][j].g = 255;   // only green for box
				p1->PX[i][j].b = 0;
			}
#endif
		}
	}

	// EE logo : lower center
#define DITCH_HEIGHT  200
	place_W = (p1->W - ee->W) / 2 - 1;				//these formula calculates the
	place_H = p1->H - DITCH_HEIGHT - ee->H - 1;				//position of the EE logo

	for (j=0; j<ee->H; j++)					//loop to do "if the EE logo isn't
	{										//white, then pic1 will be replaced
		for (i=0; i<ee->W; i++)				//by EE logo"
		{
			if (ee->PX[i][j].r != 255 ||
				ee->PX[i][j].g != 255 ||
				ee->PX[i][j].b != 255)
			{
				p1->PX[place_W+i][place_H+j].r = ee->PX[i][j].r;
				p1->PX[place_W+i][place_H+j].g = ee->PX[i][j].g;
				p1->PX[place_W+i][place_H+j].b = ee->PX[i][j].b;
			}
		}
	}

	// NTHU logo : uper left corner
#if 0
	place_W = (p1->W - nthu->W)/2-1;		//these formula calculate the
#else
	place_W = 0 + 1;		//these formula calculate the
#endif
#if 0
	place_H = (p1->H - nthu->H)/2-1;		//position of the NTHU logo
#else
	place_H = 0 + 1;		//position of the NTHU logo
#endif

	for (j=0; j<nthu->H; j++)				//loop to do "if the NTHU logo is
	{										//not white, then the intensity of
		for (i=0; i<nthu->W; i++)			//red and blue PIXELs will be
		{									//changed into 255"
			if (nthu->PX[i][j].r != 255 ||
				nthu->PX[i][j].g != 255 ||
				nthu->PX[i][j].b != 255)
			{
				p1->PX[place_W+i][place_H+j].r = 255;
				p1->PX[place_W+i][place_H+j].b = 255;
			}
		}
	}


	return p1;								//return p1 and end the function
}

