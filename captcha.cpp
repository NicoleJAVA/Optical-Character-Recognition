
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
double captchaVec[5][10];
double trainVector[5][10];
int trainCoord[70][5]; 
// for word whose ID == 12, int trainCoord[12][1] stores which row is the word's
// top boundary .  top-down-left-right

int trainCol[14][2] = {   { 5,36},{ 37,68}, { 69,99}, { 100,131}, {132,162}, 
{163,193},{194,225}, {226,256}, {257,287}, {288,319}, {320,350}, {351,382},
{383,413}, {414,443}   };
int trainRow[5][2] = { {7,52},{53,98},{99,145},{146,191},{192,237} };


/*********************************************************************/

void get_bitQ(int left,int right,int top,int down, int wordID)
{
	int Q[5];
	for( i=0; i<5; i++ ) Q[i] = 0; 
	int cc = 1, xxx=0;
   for( i=top; i<down; i++ ){
		//printf("\n\n");
		for( j=left; j<right; j++ ){
		   //printf("\n %d", cc );
		   cc++;
			bits = 0;
		   
			for( a=i; a<i+2; a++ ){
				for( b=j; b<j+2; b++ ){
					if( rmvBg[a][b] == 0 ) bits++;
				}
			}
			if( bits == 1 ) {
				//printf("\n%d  hi  ", i ); 
				Q[0]++;
			}
			if( bits == 2 ) {
				//printf("\n%d  yo  ", i );
				if( ( rmvBg[a][b]==255 && rmvBg[a+1][b+1]==0 ) || ( rmvBg[a][b]==0 && rmvBg[a+1][b+1]==255 ) ){
					Q[4] ++;
				} 
				else{
					Q[1]++;
				}
			}
			if( bits == 3 ) {
				//printf("\n%d  yeee  ", i );
				Q[2]++;
			}
			if( bits == 4 ) {
				//printf("\n%d  UUuuu  ", i );
				Q[3]++;
			}
			if( bits == 0 ) {
				xxx ++;
			
			} // End - 3 - if
				
		}	// End - 2 - for 
   } // End - 1 - for 
   
   for( i=0; i<5; i++ ) printf("\n     %d", Q[i] );
   printf("\n     %d", xxx );
   
   /* Area */
   captchaVec[wordID][0] = 0.25*Q[0] + 0.5*Q[1] + 0.875*Q[2] + Q[3] + 0.75*Q[4]; 
   /* Perimeter */
	captchaVec[wordID][1] = (Q[0] + Q[2] + 2*Q[4])*(1/sqrt(2)) + Q[1]; 
   /* Euler Number using 8-Connectivity */
	captchaVec[wordID][2] =  0.25 * (Q[0] - Q[2] - 2*Q[4]);
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
	if (!(file=fopen("captcha.raw","rb"))){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size, file);
	fclose(file);
	
	if (!(file=fopen("training.raw","rb"))){
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
				printf("  %d ¡@", rmvBg[i][j] ); 
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
						/* Partly blocking*/
						/* * * * * * * * * * * * * * * * * */	
						
	for( i=0; i<238; i++   ){
		for( j=0; j<450; j++ ){
			trmvBg[i][j] = (unsigned char) 0; 
		}
	}

	
	/***********************************************
	for( i=0; i<Size1; i++ ){
		for( j=0; j<Size2; j++ ){
			if( trainImg[i][j] >= 100){
				trmvBg[i][j] = (unsigned char) 255;
			} // End - 3 - if 
		
			if( trainImg[i][j] <= 100 ){trmvBg[i][j] = (unsigned char) 0;}
		
		} // End - 2 - for 
	} // End - 1 - for 
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
			wordPos[c-1][1] = j;
		}
		if( isBlankCol[j+1]==1 && isBlankCol[j]==0 ){wordPos[c-1][2] = j;}
	}	
	
	for(i=0;i<5;i++) printf("\n   %d   %d  ", wordPos[i][1], wordPos[i][2] ); 
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
			printf("\n   HeyHeyUU  %d ", c);
			
			// the (c-1)th row is the boundary between blank / non-blank 
			tRowPos[c-1][1] = i; 
		}
		if( tBlankRow[i+1]==1 && tBlankRow[i]==0 ){tRowPos[c-1][2] = i;}
	}	
	
	//for(i=0;i<10;i++) printf("\n  tRowPos %d   %d  ", tRowPos[i][1], tRowPos[i][2] ); 



	//int top, down, left, right;
	for( ID=0; ID<70; ID++ ){
		quotient = (int) ( (ID) / 14 );
		remainderNum = ID - ( 14 * quotient );
		if(ID==3)
		printf("\n %d divided by 14 is %d, remainder is %d", ID, quotient, remainderNum );
	
		trainVector[ID][9] = quotient;
		//trainVector[ID][10] = (double) remainder;
	
/***********************************	
	int trainCol[14][2] = {   { 5,36},{ 37,68}, { 69,99}, { 100,131}, {132,162}, 
		{163,193},{194,225}, {226,256}, {257,287}, {288,319}, {320,350}, {351,382},
		{383,413}, {414,443}   };
	int trainRow[5][2] = { {7,52},{53,98},{99,145},{146,191},{192,237} };
/*************************/	

		trainCoord[ID][0] = trainRow[quotient][0];
		trainCoord[ID][1] = trainRow[quotient][1];	
		trainCoord[ID][2] = trainCol[remainderNum][0];		
		trainCoord[ID][3] = trainCol[remainderNum][1];
		if(ID==3) printf(" \nID 3 top is %d", trainCoord[ID][0] );
	}

	for( ID=0; ID<20; ID++ ){
			printf("\n ID %d: top %d down %d %d %d", ID, trainCoord[ID][0],trainCoord[ID][1],trainCoord[ID][2],trainCoord[ID][3]);

		}


	get_bitQ( 101, 108, 120, 180, 1);


						/********************************************/
						/******                                ******/
						/******     Constructing Element Type I 
						/******     8-Neighbor
						/******                                ******/
						/********************************************/	

	
						/********************************************/
						/******                                ******/
						/******    write image to "M.raw"  ******/
						/******                                ******/
						/********************************************/		
	if (  !(file=fopen("M.raw","wb"))  ){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else{
		fwrite( rmvBg, sizeof(unsigned char), Size*Size, file);
		fclose(file);
	}
	
	if (  !(file=fopen("trainRmv.raw","wb"))  ){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else{
		fwrite( trmvBg, sizeof(unsigned char), Size1*Size2, file);
		fclose(file);
	}

	system("PAUSE");
	exit(0);

}
