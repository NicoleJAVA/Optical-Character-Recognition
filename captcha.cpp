
#include "stdio.h"
#include "iostream.h"
#include "stdlib.h"
#include "math.h"

#define PI 3.14159265
#define Size 256
#define ThreshUp 30
#define ThreshLow 20
#define startRow 120 
#define endRow 180
#define Size1 248
#define Size2 450
#define topbound 135
#define bottombound 147
using namespace std;
unsigned char rmvBg[Size][Size];
unsigned char trainImg[248][450];
unsigned char trmvBg[248][450];
int wordPos[5][2]; // word position : starting column and ending column
int tColPos[14][2];
int tRowPos[10][2];
int isBlankCol[Size];
int isBlankRow[Size];
int tBlankCol[Size1]; // t : training.raw 
int tBlankRow[Size2]; // t : training.raw 
int flag, c, i, j, a, b, bits, ID, quotient, remainderNum;
float captchaVec[5][10];
float trainVector[5][5];
float diff[5][70];   // difference between captcha and training vectors 
int trainCoord[70][4]; 
// for word whose ID == 12, int trainCoord[12][1] stores which row is the word's
// top boundary .  top-down-left-right
int captCoord[5][4];

int trainCol[14][2] = {   { 5,36},{ 37,68}, { 69,99}, { 100,131}, {132,162}, 
{163,193},{194,225}, {226,256}, {257,287}, {288,319}, {320,350}, {351,382},
{383,413}, {414,443}   };
int trainRow[5][2] = { {7,52},{53,98},{99,145},{146,191},{192,237} };


/*********************************************************************/

void get_bitQ(int left,int right,int top,int down, int wordID, int option)
{
	int Q[5];
	for( i=0; i<5; i++ ) Q[i] = 0; 
	int cc = 1, xxx=0;
	
	/**************
		
		option == 1   --->   get the bit quads from rmvBg[][]  ("captcha.raw" )
		option == 2   --->   get the bit quads from trmvBg[][] ("training.raw")
		
	/**************/
	//printf("\n   ID: %d", wordID);
   for( i=top; i<down; i++ ){
		//printf("\n\n");
		for( j=left; j<right; j++ ){
		   //printf("\n %d", cc );
		   cc++;
			bits = 0;
		   
			for( a=i; a<i+2; a++ ){
				for( b=j; b<j+2; b++ ){
					if( option == 1 ){
						if( rmvBg[a][b] == 0 ) bits++;
					}				
					if( option == 2 ){
						if( trmvBg[a][b] == 0 ) bits++;
					}
				}
			}
			
			
			/***************/
			if( bits == 1 ) {
				Q[0]++;
			}
			if( bits == 2 ) {
				//printf("\n%d  yo  ", i );
				if( option == 1 ){
					if( ( rmvBg[a][b]==255 && rmvBg[a+1][b+1]==0 ) || ( rmvBg[a][b]==0 && rmvBg[a+1][b+1]==255 ) ){
						Q[4] ++;
					} 
					else{ Q[1]++;	}
				}
				
				if( option == 2 ){
					if( ( trmvBg[a][b]==255 && trmvBg[a+1][b+1]==0 ) || ( trmvBg[a][b]==0 && trmvBg[a+1][b+1]==255 ) ){
						Q[4] ++;
					} 
					else{ Q[1]++;	}
				}
				
			}
			if( bits == 3 ) {			Q[2]++;     }
			if( bits == 4 ) {			Q[3]++;  	}
			//if( bits == 0 ) {     	xxx ++;	} // End - 3 - if
				
		}	// End - 2 - for 
   } // End - 1 - for 
  
  if( option == 1 ){
   	/* Area */
   	captchaVec[wordID][0] = 0.25*Q[0] + 0.5*Q[1] + 0.875*Q[2] + Q[3] + 0.75*Q[4]; 
   	/* Perimeter */
		captchaVec[wordID][1] = (Q[0] + Q[2] + 2*Q[4])*(1/sqrt(2)) + Q[1]; 
   	/* Euler Number using 8-Connectivity */
		captchaVec[wordID][2] =  0.25 * (Q[0] - Q[2] - 2*Q[4]);
	}


  if( option == 2 ){
   	/* Area */
   	trainVector[wordID][0] = 0.25*Q[0] + 0.5*Q[1] + 0.875*Q[2] + Q[3] + 0.75*Q[4]; 
   	/* Perimeter */
		trainVector[wordID][1] = (Q[0] + Q[2] + 2*Q[4])*(1/sqrt(2)) + Q[1]; 
   	/* Euler Number using 8-Connectivity */
		trainVector[wordID][2] =  0.25 * (Q[0] - Q[2] - 2*Q[4]);
	}
	/*************************************
	if( wordID == 4 ){
		printf("\n\n\nleft:%d right:%d top %d down%d", left,right,top,down);
		for( i=0; i<5; i++ ){
			printf("\n ID is %d Q  %d.", ID, Q[i] );
		}
	}
	*******************************/


}

/*********************************************************************/

int main( int argc, char *argv[])
{
	FILE *file;
	unsigned char Imagedata[Size][Size];
//	unsigned char rmvBg[Size][Size];

	
						/************************************************/
						/******                                	 ******/
						/****** Read "input.raw" into Imagedata[][]******/
						/******                                	 ******/
						/************************************************/							
	if (!(file=fopen(argv[1],"rb"))){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size, file);
	fclose(file);
	
	if (!(file=fopen(argv[2],"rb"))){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(trainImg, sizeof(unsigned char), Size1*Size2, file);
	fclose(file);
	
	
	for( i=0; i<Size; i++ ){
		for( j=0; j<Size; j++ ){
			rmvBg[i][j] = Imagedata[i][j];
		}	
	}
	
	/**********************************	do some image processing task... ***/
	
	
							/* * * * * * * * * * * * * * * * * */
							/* turn rmvBg[][] into binary image*/
							/* * * * * * * * * * * * * * * * * */
	
	
	for( i=120; i<180; i++ ){
		for( j=0; j<Size; j++ ){
			if( Imagedata[i][j] > 50 ){
				rmvBg[i][j] = (unsigned char) 255;
			} // End - 3 - if 
			if( Imagedata[i][j] <= 50 ){
				rmvBg[i][j] = (unsigned char) 0;
			}
		} // End - 2 - for 
	} // End - 1 - for 
	
	
						/* * * * * * * * * * * * * * * * * */
						/* turn rmvBg[][] into binary image*/
						/* * * * * * * * * * * * * * * * * */	
						
	for( i=0; i<Size1; i++   ){
		for( j=0; j<Size2; j++ ){
			trmvBg[i][j] = trainImg[i][j]; 
		}
	}
		
						/* * * * * * * * * * * * * * * * * */
						/* turn trmvBg[][] into binary image*/
						/* * * * * * * * * * * * * * * * * */
	/***********************************************/
	for( i=0; i<Size1; i++ ){
		for( j=0; j<Size2; j++ ){
			if( trainImg[i][j] >= 100){
				trmvBg[i][j] = (unsigned char) 255;
			} 
			if( trainImg[i][j] <= 100 ){trmvBg[i][j] = (unsigned char) 0;}
		}  
	}
	/***********************************************/


	/* --------------   Cut   The   Words in  "captcha.raw" ------ */
	for( j=0; j<Size; j++ ){ 
		
		flag = 1;
		
		for( i=120; i<180; i++ ){
			if( rmvBg[i][j] ==  0 ){
				flag = 0;} // End - 3 - if 
		} // End - 2 - for 
		if( flag == 1 ){
			isBlankCol[j] = 1;} // End - 2 - if 
		if( flag == 0 ){
			isBlankCol[j] = 0;} // End - 2 - if 
	} // End - 1 - for 
	
	c = 0;
	for( j=0; j<Size; j++ ){
		if( isBlankCol[j-1]==1 && isBlankCol[j]==0 ){
			c++;
			//printf("   Hey  %d ", c);
			wordPos[c-1][0] = j;
		}
		if( isBlankCol[j+1]==1 && isBlankCol[j]==0 ){
			wordPos[c-1][1] = j;}
	}	
	
	//for(i=0;i<5;i++) printf("\n   %d   %d  ", wordPos[i][0], wordPos[i][1] ); 
/*--------------------------------------------------------------------------**/



/* * * * * * * * * * * * * * * * * */




	/* ------ Cut The  blank / non-blank rows in  "training.raw" ------ */
	for( j=0; j<Size2; j++ ){ 
		flag = 1;		
		for( i=0; i<Size1; i++ ){
			if( trmvBg[i][j] ==  0 ){
				flag = 0;
				} // End - 3 - if 
		} // End - 2 - for 
		if( flag == 1 ){
			tBlankRow[i] = 1;} // End - 2 - if 
		if( flag == 0 ){
			tBlankRow[i] = 0;} // End - 2 - if 
	} // End - 1 - for 
	
	c = 0;
	for( i=0; i<Size1; i++ ){
		if( tBlankRow[i-1]==1 && tBlankRow[i]==0 ){
			c++;	
			// the (c-1)th row is the boundary between blank / non-blank 
			tRowPos[c-1][1] = i; 
		}
		if( tBlankRow[i+1]==1 && tBlankRow[i]==0 ){tRowPos[c-1][2] = i;}
	}	
	
	//for(i=0;i<10;i++) printf("\n  tRowPos %d   %d  ", tRowPos[i][1], tRowPos[i][2] ); 

					/*********************************************************/
					/****                               
					/***       compute the top,down,left,right boundaries
					/***		  for each characters in "training.raw". 
					/***     
					/*********************************************************/	

		for( ID=0; ID<5; ID++ ){
			
			captCoord[ID][0] = 	wordPos[ID][0]; /* left boundary */
			captCoord[ID][1] = 	wordPos[ID][1]; /* right boundary */
			captCoord[ID][2] = 	135; /* top boundary */
			captCoord[ID][3] = 	147; /* down boundary */

		}

					/*********************************************************/
					/****                               
					/***       compute the top,down,left,right boundaries
					/***		  for each characters in "training.raw". 
					/***     
					/*********************************************************/	


	for( ID=0; ID<70; ID++ ){
		quotient = (int) ( (ID) / 14 );
		remainderNum = ID - ( 14 * quotient );
		/***********************************	
		int trainCol[14][2] = {   ........	{383,413}, {414,443}   };
		int trainRow[5][2] = { {7,52},.......};
		/*************************/	
		trainCoord[ID][0] = trainCol[remainderNum][0]; // left	
		trainCoord[ID][1] = trainCol[remainderNum][1]; // right
		trainCoord[ID][2] = trainRow[quotient][0];	  // top
		trainCoord[ID][3] = trainRow[quotient][1];	  // down

	}
	

					/*********************************************************/
					/****                               
					/***       compute trainVector by calling bitQ() function
					/***       compute captchaVec by calling bitQ() function
					/***     
					/*********************************************************/	

	int top, down, left, right;
	for( ID=0; ID<70; ID++ ){
		left = trainCoord[ID][0];
		right= trainCoord[ID][1];
		top  = trainCoord[ID][2];
		down = trainCoord[ID][3];
		get_bitQ( left, right, top, down, ID, 2 );
	}
	
	for( ID=0; ID<5; ID++ ){

		left = captCoord[ID][0];
		right= captCoord[ID][1];
		top  = captCoord[ID][2];
		down = captCoord[ID][3];
		
		get_bitQ( left, right, top, down, ID, 1 );
	}
	
					/*********************************************************/
					/****                               
					/***       compute captchaVector by calling bitQ() function
					/***     
					/*********************************************************/
	float temp1, temp2;
	for( i=0; i<5; i++ ){
		temp1 = 0;
		for( j=0; j<70; j++ ){
			temp2 = 0;
			for( c=0 ; c<3; c++ ){
				temp1 =  captchaVec[i][c] - trainVector[j][c] ;
				temp2 += temp1 * temp1; 
			}	
			diff[i][j] = sqrt(temp2); 
		}
	}
	
	float min = 2000;
	int minID = -1;
	for( i=0; i<5; i++ ){
		//printf("\n\n\n-------------");
		min = 2000;
		minID = -1;
		for( j=0; j<70; j++ ){
			
			//if(diff[i][j]>1000)
			//printf("\ndiff[%d][%d] is %f\n", i,j, diff[i][j]);
		   if( diff[i][j] < min )
			{
				//printf("     ..   %d   ..   ",j );
				min = diff[i][j];
				minID = j; 
				//break;
			}
		}
		printf("\n the %d-th word is %d.", i, minID );
		printf("\ndiff[%d][%d] is %f\n", i,minID, diff[i][minID]);
	}
	
	
	
	
	
	
	
	
	
	
	/********				Block partly the image   ******/
	for( i = trainCoord[22][0]; i< trainCoord[22][1]; i++ ){
		for( j=trainCoord[22][2]; j<trainCoord[22][3]; j++ ){
			//trmvBg[i][j] = 0;
		}
	}	
	
		/********				Block partly the image   ******
	for( i = 7; i< 52; i++ ){
		for( j=132; j<162; j++ ){
			//trmvBg[i][j] = 0;
		}
	}
	/********				Block partly the image   ******
	for( i = captCoord[3][0]; i< captCoord[3][1]; i++ ){
		for( j=captCoord[3][2]; j<captCoord[3][3]; j++ ){
			rmvBg[i][j] = 0;
		}
	}	
	
	
	/********************************************
	for(i=0;i<5;i++){
		printf("\n worPos %d %d", wordPos[i][0], wordPos[i][1]);
		}
	/********************************************/
	
						/********************************************/
						/******                                ******/
						/******    write image to "M.raw"  ******/
						/******                                ******/
						/********************************************	
	if (  !(file=fopen("binary_captha.raw","wb"))  ){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else{
		fwrite( rmvBg, sizeof(unsigned char), Size*Size, file);
		fclose(file);
	}
	
	if (  !(file=fopen("binary_training.raw","wb"))  ){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else{
		fwrite( trmvBg, sizeof(unsigned char), Size1*Size2, file);
		fclose(file);
	}
/********************************************/
	system("PAUSE");
	exit(0);

}
