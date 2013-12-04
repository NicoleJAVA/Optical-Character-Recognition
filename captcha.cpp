
#include "stdio.h"
#include "iostream.h"
#include "stdlib.h"
#include "math.h"

#define PI 3.14159265
#define Size 256
#define ThreshUp 30
#define ThreshLow 20
using namespace std;
struct BitQuad{
	double M11;
	double M12;
	double M21;
//	double M22;
	double A;
	double P;
	double hei_ratio;
	int Euler;
	int Q[5];	
};
int wordPos[5][2]; // word position : starting column and ending column
int isBlankCol[Size];
int flag, c;
int main( int argc, char *argv[])
{
	FILE *file;
	unsigned char Imagedata[Size][Size];
	unsigned char rmvBg[Size][Size];
	int i,j, bwflag;
	
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
	for( i=0; i<Size; i++ ){
		for( j=0; j<Size; j++ ){
			rmvBg[i][j] = Imagedata[i][j];
		}	
	}
	
	/**********************************	do some image processing task... ***/
	bwflag = 0;
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


	/* ¤Á¦r */
	for( j=0; j<Size; j++ ){ 
		
		flag = 1;
		
		for( i=120; i<180; i++ ){
			if( rmvBg[i][j] ==  0 ){
				flag = 0;
				//printf("    %d %d    ", i, j );
			} // End - 3 - if 

		} // End - 2 - for 
		if( flag == 1 ){
			isBlankCol[j] = 1; 
			//printf("\n\n  %d",  j );
		} // End - 2 - if 
		if( flag == 0 ){
			isBlankCol[j] = 0; 
			//printf("\n\n  %d",  j );
		} // End - 2 - if 
	} // End - 1 - for 
	
	c = 0;
	for( j=0; j<Size; j++ ){
		if( isBlankCol[j-1]==1 && isBlankCol[j]==0 ){
			c++;
			printf("   Hey  %d ", c);
			wordPos[c-1][1] = j;
		}
		if( isBlankCol[j+1]==1 && isBlankCol[j]==0 ){
			wordPos[c-1][2] = j;
		}
	}
	for(i=0;i<5;i++) printf("\n   %d   %d  ", wordPos[i][1], wordPos[i][2] ); 
	
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
	


	system("PAUSE");
	exit(0);
}

