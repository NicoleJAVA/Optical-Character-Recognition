#include<stdio.h>
#include<math.h>

#define size 256

struct segment{
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
char character[21]="ajrycms!ent+fou*hpw/";

void get_bitquads(struct segment* seg,int k,unsigned char image[][66]){
	int i,j,a,b,one;

	for(i=0;i<5;i++)
		seg[k].Q[i]=0;
	for(i=0;i<65;i++){
		for(j=0;j<65;j++){
			one=0;
			for(a=i;a<i+2;a++)
				for(b=j;b<j+2;b++)
					if(image[a][b]==1){
						one++;
					}
			if(one>0){
				if(one!=2)
					seg[k].Q[one]++;
				else{
					if(image[i][j]==image[i+1][j+1])
						seg[k].Q[0]++;
					else
						seg[k].Q[2]++;
				}
			}
		}
	}
}

void get_moments(struct segment *seg,int k,unsigned char image[][66]){
	int i,j,di,dj;
	double m10=0,m01=0,m11=0,m12=0,m21=0;
	for(i=1,di=1;i<65;i++,di++)
		for(j=1,dj=1;j<65;j++,dj++){
			m10+=dj*((double)image[i][j]);
			m01+=di*((double)image[i][j]);
		}
	/*centre coordinate*/
	m10/=seg[k].A;
	m01/=seg[k].A;
	for(i=1,di=1;i<65;i++,di++)
		for(j=1,dj=1;j<65;j++,dj++){
			m11+=(di-m01)*(dj-m10)*((double)image[i][j]);
			m21+=pow(dj-m10,2)*(di-m01)*((double)image[i][j]);
			m12+=pow(di-m01,2)*(dj-m10)*((double)image[i][j]);
		}
	seg[k].M11=m11/pow(seg[k].A,2);
	seg[k].M21=m21/pow(seg[k].A,2.5);
	seg[k].M12=m12/pow(seg[k].A,2.5);
}

void get_attribute(int l,int r,int t,int d,struct segment *seg,int k,unsigned char image[][size]){
	int a,b,i,j;
        int left,right,top,down;
	double delta_dt,delta_rl,x,y;
	unsigned char Nimage[66][66];
	
	for(a=l;a<r;a++)
		for(b=t;b<d;b++)
			if(image[b][a]<255){
				left=a;
				a=r;
				break;
			}
	for(a=r-1;a>=l;a--)
		for(b=t;b<d;b++)
			if(image[b][a]<255){
				right=a;
				a=-1;
				break;
			}
	for(a=t;a<d;a++)
		for(b=l;b<r;b++)
			if(image[a][b]<255){
				top=a;
				a=d;
				break;
			}
	for(a=d-1;a>=t;a--)
		for(b=l;b<r;b++)
			if(image[a][b]<255){
				down=a;
				a=-1;
				break;
			}
	
	/*normalize ot size 64x64*/
	delta_rl=(double)(right-left);
	delta_dt=(double)(down-top);
	for(i=0;i<66;i++){
		if(i==0||i==65){
			for(j=0;j<66;j++)
				Nimage[i][j]=0;
			continue;
		}
		for(j=0;j<66;j++){
			if(j==0||j==65){
				Nimage[i][j]=0;
				continue;
			}
			y=((double)i-1)*delta_dt/63+top;
			x=((double)j-1)*delta_rl/63+left;
			a=(int)y;
			b=(int)x;
			Nimage[i][j]=(b+1-x)*(a+1-y)*image[a][b]+(x-b)*(a+1-y)*image[a][b+1]+(b+1-x)*(y-a)*image[a+1][b]+(x-b)*(y-a)*image[a+1][b+1];
		}
	}

	for(i=1;i<65;i++)
		for(j=1;j<65;j++){
			if(Nimage[i][j]>127)
				Nimage[i][j]=0;  //background
			else
				Nimage[i][j]=1;  //image
			if(Nimage[i][j]>1)
				printf(">1\n");
		}

	/*height ratio*/
	seg[k].hei_ratio=delta_dt/(delta_dt+delta_rl);
	get_bitquads(seg,k,Nimage);
	/*Euler number*/
	seg[k].Euler=(seg[k].Q[1]-seg[k].Q[3]-2*seg[k].Q[0])/4;
	/*Area*/
	seg[k].A=((double)(seg[k].Q[1]+2*seg[k].Q[2]+3*seg[k].Q[3]+4*seg[k].Q[4]+2*seg[k].Q[0]))/4;
	/*Perimeter*/
	seg[k].P=(double)(seg[k].Q[1]+seg[k].Q[2]+seg[k].Q[3]+2*seg[k].Q[0]);
	/*centralised moment*/
	get_moments(seg,k,Nimage);
}
int analysis(struct segment *seg, unsigned char image[][size]){
	int i,j,k,l,a,b,flag,v_count,h_count,r,t,d;
	int v_bound[10],h_bound[10];
	FILE *temp;
	/*segment*/
	/*vertical boundary*/
	for(i=0,flag=0,v_count=0,r=0;i<size;i++){
		for(j=0;j<size;j++)
			if(image[j][i]<255){
				if(flag==0){
					flag=1;
					if(r>0){
						v_bound[v_count++]=(r+i)/2;
					}
				}
				break;
			}
		if(j==size && flag==1){
			r=i;
			flag=0;
		}
	}
	/*horizontal boundary*/
	for(i=0,flag=0,h_count=0,d=0;i<size;i++){
		for(j=0;j<size;j++)
			if(image[i][j]<255){
				if(flag==0){
					flag=1;
					if(d>0){
						h_bound[h_count++]=(d+i)/2;
					}
				}
				break;
			}
		if(j==size && flag==1){
			d=i;
			flag=0;
		}
	}

	/*for each segment, get attribute*/
	for(i=0,r=0,k=0;i<=v_count;i++){
		l=r;
		if(i==v_count)
			r=size;
		else
			r=v_bound[i];
		for(j=0,d=0;j<=h_count;j++,k++){
			t=d;
			if(j==h_count)
				d=size;
			else
				d=h_bound[j];
			get_attribute(l,r,t,d,seg,k,image);
//			printf("k:%d E:%d M11: %.3lf M12: %.3lf M21: %.3lf hei: %.3lf P: %.3lf\n",k,seg[k].Euler,seg[k].M11,seg[k].M12,seg[k].M21,seg[k].hei_ratio,seg[k].P);
		}
	}
	/*number of total characters*/
	return k;
}
int main(int argc, char *argv[]){
	struct distance{
		double P;
		double M11;
		double M12M21;
		double moment;
		double hei_ratio;
	};
	FILE *in1,*in2;
	struct distance delta[10][20];
	struct segment train_data[20];
	struct segment input_data[10];
	unsigned char image[size][size];
	int i,j,k,num,maxtrain,maxinput,flag[20];
	double min;

	/*tainning data*/
	in1=fopen("training.raw","rb");
	fread(image,size*size,1,in1);
	fclose(in1);
	maxtrain=analysis(train_data,image);

	/*input data*/
	in2=fopen("captcha.raw","rb");
	fread(image,size*size,1,in2);
	fclose(in2);
	maxinput=analysis(input_data,image);

	/*recognize input character*/
	/*1. calculate difference of some features between input data and trainning data*/
	for(i=0;i<maxinput;i++)
		for(j=0;j<maxtrain;j++){
			delta[i][j].hei_ratio=fabs(input_data[i].hei_ratio-train_data[j].hei_ratio);
			delta[i][j].P=fabs(input_data[i].P-train_data[j].P);
			delta[i][j].M11=fabs(input_data[i].M11-train_data[j].M11);
			delta[i][j].M12M21=pow(input_data[i].M12-train_data[j].M12,2)+pow(input_data[i].M21-train_data[j].M21,2);
			delta[i][j].moment=pow(input_data[i].M11-train_data[j].M11,2)+pow(input_data[i].M12-train_data[j].M12,2)+pow(input_data[i].M21-train_data[j].M21,2);
		}
	
	/*2. classification*/
	printf("The result of my OCR program is:\n");
	for(i=0;i<maxinput;i++){
		if(input_data[i].Euler==0){
			// a,e,o,p
			min=delta[i][0].moment;
			num=0;
			for(j=1;j<20;j++)
				if(train_data[j].Euler==0){
					if(delta[i][j].moment<min){
						min=delta[i][j].moment;
						num=j;
					}
				}
			printf("%c ",character[num]);
		}
		else if(input_data[i].Euler==1){
			// r,f,+,t,/,n,h,u,*,y,c,m,s,w
			for(j=0;j<maxtrain;j++)
				flag[j]=0;
			min=delta[i][2].moment;
			num=2;
			for(j=3;j<20;j++)
				if(train_data[j].Euler==1){
					if(delta[i][j].moment<min){
						min=delta[i][j].moment;
						num=j;
					}
				}
			if(num==2)
				printf("%c ",character[num]);	//r
			else if(num==11 || num==15){
				//*,+
				if(delta[i][11].P<delta[i][15].P)
					printf("%c ",character[11]);	//+
				else
					printf("%c ",character[15]);	//*
			}
			else{
				//f,t,/,n,h,u,y,m,s,w,c
				flag[2]=1;
				flag[4]=1;	//c
				flag[11]=1;
				flag[15]=1;
				min=delta[i][3].hei_ratio;
				num=3;
				for(j=4;j<maxtrain;j++)
					if(train_data[j].Euler==1 && flag[j]==0){
						if(delta[i][j].hei_ratio<min){
							min=delta[i][j].hei_ratio;
							num=j;
						}
					}
				if(num==5||num==18){
					//m,w,n,u,c
					min=delta[i][18].M12M21;
					num=18;
					if(delta[i][5].M12M21<min){
						num=5;
						min=delta[i][5].M12M21;
					}
					if(delta[i][4].M12M21<min)
						printf("%c ",character[4]);	//c
					else if(num==5){
						//m,n
						if(delta[i][5].P<delta[i][9].P)
							printf("%c ",character[5]);	//m
						else
							printf("%c ",character[9]);	//n
					}
					else{
						//w,u
						if(delta[i][18].M11<delta[i][14].M11)
							printf("%c ",character[18]);     //w
						else
							printf("%c ",character[14]);     //u
					}
				}
				else{
					//f,t,/,n,h,u,y,s,c
					flag[5]=1;
					flag[18]=1;
					min=delta[i][4].M12M21;
					num=4;
					for(j=3;j<maxtrain;j++)
						if(train_data[j].Euler==1 && flag[j]==0){
							if(delta[i][j].M12M21<min){
							//	min=delta[i][j].moment;
								min=delta[i][j].M12M21;
								num=j;
							}
						}
			//		printf("%c ",character[num]);
					if(num==6||num==19){
						//s,/
						if(delta[i][6].M11<delta[i][19].M11)
							printf("%c ",character[6]);	//s
						else
							printf("%c ",character[19]);	//'/'
					}
					else if(num==12||num==14){
						//f,u
						if(delta[i][12].M11<delta[i][14].M11)
							printf("%c ",character[12]);	//f
						else
							printf("%c ",character[14]);	//u
					}
					else{
						//t,n,h,y,c
						printf("%c ",character[num]);
					}
				}
			}
		}
		else if(input_data[i].Euler==2){
			if(delta[i][1].moment<delta[i][7].moment)
				printf("%c ",character[1]);	//j
			else
				printf("%c ",character[7]);	//!
		}
		else{
			/*alphabet not in trainning data*/
			for(j=0;j<maxtrain;j++)
				flag[j]=0;
			/*1. select 5 symbols whose height ratio is close to input alphabet*/
			for(k=0;k<5;k++){
				min=1;
				for(j=0;j<maxtrain;j++)
					if(flag[j]==0){
						if(delta[i][j].hei_ratio<min){
							min=delta[i][j].hei_ratio;
							num=j;
						}
					}
				flag[j]=1;
			}
			/*2. use moment to determine one of 5 symbols seleted by step 1.*/
			min=100000000;
			for(j=0;j<maxtrain;j++)
				if(flag[j]==1){
					if(delta[i][j].moment<min){
						min=delta[i][j].moment;
						num=j;
					}
				}
			printf("%c ",character[num]);
		}
	}
	printf("\n");
	return 0;
}
