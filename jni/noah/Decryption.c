/*
 ============================================================================
 Name        : Decryption.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <sys/unistd.h>

#define Nb 4
#define Nr 10
extern unsigned char DB[592][256];
//
void InvShiftRows (unsigned char *State)
{
unsigned char tmp;

	//first row unchange
	//second row anti_shift_row
	tmp = State[13];
	State[13] = State[9];
	State[9] = State[5];
	State[5] = State[1];
	State[1] = tmp;

	//third row anti_shift_row
	tmp = State[2];
	State[2] = State[10];
	State[10] = tmp;
	tmp = State[6];
	State[6] = State[14];
	State[14] = tmp;

	//fourth row anti_shift_row
	tmp = State[3];
	State[3] = State[7];
	State[7] = State[11];
	State[11] = State[15];
	State[15] = tmp;
}


//
//unsigned char LookUp(const char * path,const int position){
//	FILE *fp=NULL;
//	int ret=0;
//	size_t retlen=0;
//	unsigned char test[1];
//	if(path ==NULL||position<1){
//			printf("path ==NULL||position<1\n");
//			ret = 0x1001101;
//			fclose(fp);
//		    fp=NULL;
//		}
//	//printf("path is %s\n",path);
//	//printf("position is %d\n",position);
//	fp=fopen(path,"rb");
//
//	if(fp==NULL){
//			printf("fp==NULL\n");
//			ret = 0x1001101;
//			fclose(fp);
//		    fp=NULL;
//		}
//	fseek(fp,position,SEEK_SET);
//	retlen=fread(test,1,1,fp);
//	if(retlen==0){
//			ret = 0x1001101;
//			printf("retlen==0\n");
//			fclose(fp);
//			fp=NULL;
//		}
//	fclose(fp);
//	//printf("test[0] is %#x",test[0]);
//	return test[0];
//
//
//}




void  Decrypt(unsigned char *InText, unsigned char *OutText)
{
//char path[35];
//strcpy(path,"/root/1.txt");
unsigned idx;
unsigned char State[Nb * 4];
int i,j;

unsigned char temp[16];



	for( idx = 0; idx < Nb; idx++ ) {
		State[4*idx+0] = *InText++;
		State[4*idx+1] = *InText++;
		State[4*idx+2] = *InText++;
		State[4*idx+3] = *InText++;
	}
//look up wb_table in the first round
	InvShiftRows(State);
	for(i=0;i<4;i++){
		temp[4*i+0]=DB[(16*i+0)][State[4*i+0]]^DB[(16*i+4)][State[4*i+1]]^DB[(16*i+8)][State[4*i+2]]^DB[(16*i+12)][State[4*i+3]];
		temp[4*i+1]=DB[(16*i+1)][State[4*i+0]]^DB[(16*i+5)][State[4*i+1]]^DB[(16*i+9)][State[4*i+2]]^DB[(16*i+13)][State[4*i+3]];
		temp[4*i+2]=DB[(16*i+2)][State[4*i+0]]^DB[(16*i+6)][State[4*i+1]]^DB[(16*i+10)][State[4*i+2]]^DB[(16*i+14)][State[4*i+3]];
		temp[4*i+3]=DB[(16*i+3)][State[4*i+0]]^DB[(16*i+7)][State[4*i+1]]^DB[(16*i+11)][State[4*i+2]]^DB[(16*i+15)][State[4*i+3]];

		State[4*i+0]=temp[4*i+0];
		State[4*i+1]=temp[4*i+1];
		State[4*i+2]=temp[4*i+2];
		State[4*i+3]=temp[4*i+3];
	}
//look up wb_table in the 2-9 round
	for(j=1;j<9;j++){
	InvShiftRows(State);
	for(i=0;i<4;i++){

		temp[4*i+0]=DB[64*j+(16*i+0)][State[4*i+0]]^DB[64*j+(16*i+4)][State[4*i+1]]^DB[64*j+(16*i+8)][State[4*i+2]]^DB[64*j+(16*i+12)][State[4*i+3]];
		temp[4*i+1]=DB[64*j+(16*i+1)][State[4*i+0]]^DB[64*j+(16*i+5)][State[4*i+1]]^DB[64*j+(16*i+9)][State[4*i+2]]^DB[64*j+(16*i+13)][State[4*i+3]];
		temp[4*i+2]=DB[64*j+(16*i+2)][State[4*i+0]]^DB[64*j+(16*i+6)][State[4*i+1]]^DB[64*j+(16*i+10)][State[4*i+2]]^DB[64*j+(16*i+14)][State[4*i+3]];
		temp[4*i+3]=DB[64*j+(16*i+3)][State[4*i+0]]^DB[64*j+(16*i+7)][State[4*i+1]]^DB[64*j+(16*i+11)][State[4*i+2]]^DB[64*j+(16*i+15)][State[4*i+3]];

		State[4*i+0]=temp[4*i+0];
		State[4*i+1]=temp[4*i+1];
		State[4*i+2]=temp[4*i+2];
		State[4*i+3]=temp[4*i+3];
		}

	}
	//look up wb_table in the 10 round
	InvShiftRows(State);
	for(i=0;i<16;i++){
		State[i]=DB[64*9+i][State[i]];
	}

	for( idx = 0; idx < Nb; idx++ ) {
			*OutText++ = State[4*idx+0];
			*OutText++ = State[4*idx+1];
			*OutText++ = State[4*idx+2];
			*OutText++ = State[4*idx+3];
		}
}
