
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
#define ee 2.71828
using namespace std;
unsigned char rmvBg[Size][Size];
unsigned char dft[Size][Size];
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

	int N,u;
	float exp;
	double temp, sum;
	sum =0.0;
	int v = 0;
	int k = 0;
for(u=0; u<Size; u++ ){
	printf("Hi  ");
	for( v=0; v<Size; v++ ){
		sum =0.0;
		for( j=0; j<Size; j++ ){
			for( k=0; k<Size; k++ ){
				//X[k]=(1/N)*Sn=0N-1 x[n]*exp(-j*2p*n*k/N), k=0, ..., N-1
					temp = (u*j+v*k)/Size;
					exp = (-1)*( j*2*PI ) * temp;
					temp = pow( ee, exp );
					temp = temp * Imagedata[j][k];
					sum += temp;
				
			}	// k
		} // j 
		dft[u][v] = (unsigned char) sum;
		
	}
}
	/**********************************	do some image processing task... ***/
	

	
	
	
						/********************************************/
						/******                                ******/
						/******    write image to "M.raw"  ******/
						/******                                ******/
						/********************************************/
	if (  !(file=fopen("dft.raw","wb"))  ){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else{
		fwrite( dft, sizeof(unsigned char), Size*Size, file);
		fclose(file);
	}
	/********************************************
	if (  !(file=fopen("binary_training.raw","wb"))  ){
		cout<<"Cannot open file!"<<endl;
		exit(1);
	}
	else{
		fwrite( trmvBg, sizeof(unsigned char), Size1*Size2, file);
		fclose(file);
	}
/********************************************/
	//printf("\nHi\\n\n\n\n");
	system("PAUSE");
	exit(0);

}

