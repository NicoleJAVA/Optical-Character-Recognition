
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
using namespace std;
unsigned char rmvBg[Size][Size];
struct BitQuadStruct{
	double M11;
	double M12;
	double M21;
//	double M22;
	double Area;
	double Peri;
	double hei_ratio;
	int Euler;
	int Q[5]; // Q[0]=n{Q1}, Q[1]=n{Q2} ... etc 
};
int wordPos[5][2]; // word position : starting column and ending column
int isBlankCol[Size];
int flag, c, i, j, a, b, bits;



void get_bitQ(int left,int right,int top,int down, int wordID)
{
	int Q[5];
	for( i=0; i<5; i++ ) Q[i] = 0; 
	int cc = 1;
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
				//printf("\n%d  xxxxxxxxxxxxx  ", i );
			
			} // End - 3 - if
				
		}	// End - 2 - for 
   } // End - 1 - for 
   
   for( i=0; i<5; i++ ) printf("\n     %d", Q[i] );
}


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
	for( i=0; i<Size; i++ ){
		for( j=0; j<Size; j++ ){
			rmvBg[i][j] = Imagedata[i][j];
		}	
	}
	
	/**********************************	do some image processing task... ***/
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


	for( i=0; i<5; i++ ){
			
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
	


	system("PAUSE");
	exit(0);

}
