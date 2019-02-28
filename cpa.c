#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#define NB_TRACES 10000


static const int sbox[256] =
	    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
            0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
            0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
            0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
            0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
            0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
            0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
            0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
            0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
            0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
            0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
            0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
            0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
            0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
            0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
            0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};


void rvereseArray(char arr[], int start, int end)
{
    if (start >= end)
    return;

    int temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;

    // Recursive Function calling
    rvereseArray(arr, start + 1, end - 1);
}
char* decToHexa(int n)
{
    // char array to store hexadecimal number
    char* hexaDeciNum=(char*)malloc(sizeof(char)*10);

    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;

        // storing remainder in temp variable.
        temp = n % 16;

        // check if temp < 10
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        n = n/16;
    }
    rvereseArray(hexaDeciNum,0,i-1);
    hexaDeciNum[i]='\0';
    // printing hexadecimal number array in reverse order
    return hexaDeciNum;
}

int* GetColonneInt(int** tab,int index,int size){
	int* t=(int*)malloc(sizeof(int)*size);
	for(int i=0;i<size;i++){
		t[i]=tab[i][index];
	}
	return t;
}

double* GetColonneDouble(double** tab,int index,int size){
	double* t=(double*)malloc(sizeof(double)*size);
	for(int i=0;i<size;i++){
		t[i]=tab[i][index];
	}
	return t;
}
double maxValue( double myArray [], int size)
{
    int i;
    double maxValue;
    maxValue=myArray[0];

    //find the largest no
    for (i=0;i<size;i++)
    {
    	if(myArray[i]>maxValue)
    	    maxValue=myArray[i];
        }
        return maxValue;
}
int ArgMax(double myArray [], int size){
	int argmax;
	int max=maxValue(myArray,size);
	for (int i=0;i<size;i++)
    	{
    		if(myArray[i]==max){
    			argmax=i;
    		}
    	}
	return argmax;
}


int* generate_key_byte(){
	int* keys=(int*)malloc(256*sizeof(int));
	for(int i=0;i<256;i++){
		keys[i]=i;
	}
	return keys;
}

int HammingDistance(int a,int b){
	int x = a ^ b;
	int setBits = 0;
	while (x > 0) {
		setBits += x & 1;
		x >>= 1;
	}
  	return setBits;
}

int hamming_weight(int x){
	int HammingWeight = 0 ;
	while (x > 0)
	{
		HammingWeight += x & 1 ;
		x = x >> 1 ;
	}
	return HammingWeight ;
}

double correlationPearson(double tableauX[], double tableauY[], int tailleTableau)
{
	double sommeXY=0; //somme des Xi*yi
	double sommeX=0;//somme des Xi
	double sommeY=0;//somme des Yi
	double sommeXiCarre=0;//somme des xi carré
	double sommeYiCarre=0;
	int i=0;
	for( i=0; i<tailleTableau; i++)
	{
		sommeX+=tableauX[i];
		sommeY+=tableauY[i];
		sommeXiCarre+=pow(tableauX[i], 2.0);
		sommeYiCarre+=pow(tableauY[i], 2.0);
		sommeXY=sommeXY+(tableauX[i]*tableauY[i]);
	}
	double r=((tailleTableau*sommeXY)-sommeX*sommeY)/sqrt((tailleTableau*sommeXiCarre-pow(sommeX,2.0))*(tailleTableau*sommeYiCarre-pow(sommeY,2.0)));
	return r;
}
double correlation_max_key_byte(int key_byte,int* plaintexts_byte,double** traces){
	double distance[NB_TRACES];
	int tmp;
	for(int i=0;i<NB_TRACES;i++){
		tmp=key_byte ^ plaintexts_byte[i];
		distance[i]=hamming_weight(sbox[tmp]);
	}
	int size=sizeof(traces[0])/sizeof(int);
	double corrcoeffs[size];
	for(int i=0;i<size;i++){
		corrcoeffs[i]=correlationPearson(distance,GetColonneDouble(traces,i,size),NB_TRACES);
	}
	return maxValue(corrcoeffs,size);

}

int CPA_byte(int* plaintexts_byte,double** traces){
	int ind_key;
	int* possible_keys=generate_key_byte();
	double corrcoeffs[256];
	for(int i=0;i<256;i++){
		corrcoeffs[i]=correlation_max_key_byte(possible_keys[i],plaintexts_byte, traces);
	}
	ind_key=ArgMax(corrcoeffs,256);
	return possible_keys[ind_key];
}

char** CPA_Attack(int** plaintexts,double** traces){
	int k;
	char** key_estimation=(char**)malloc(16*16*sizeof(char*));
	for(int i=0;i<16;i++){
		k=CPA_byte(GetColonneInt(plaintexts,i,NB_TRACES),traces);
		char* c=decToHexa(k);
		for(int j=0;j<strlen(c)+1;j++){
			key_estimation[i][j]=c[j];
		}
	}
	return key_estimation;
}








