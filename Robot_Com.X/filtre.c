#include <xc.h>
#include "adc.h"
#include "filtre.h"

//Multiplicateurs
const int mult = 1024;
const long multsquare = 1048576;

//Constantes des filtres
        //Filtre 900Hz
const int a09=7;  //Gain 1/ao
const int a91[2]={-1912,1018};const int a92[2]={-1921,1019};const int a93[2]={-1907,1011};const int a94[2]={-1911,1011};
const int b91[3]={1024,0,-1024};const int b92[3]={1024,0,-1024};const int b93[3]={1024,0,-1024};const int b94[3]={1024,0,-1024};
        //Filtre 1100Hz
const int a011=8;  //Gain 1/ao
const int a111[2]={-1847,1017};const int a112[2]={-1861,1018};const int a113[2]={-1843,1008};const int a114[2]={-1849,1008};
const int b111[3]={1024,0,-1024};const int b112[3]={1024,0,-1024};const int b113[3]={1024,0,-1024};const int b114[3]={1024,0,-1024};


//Variables de stockage
static long W91[3] = {0, 0, 0};static long W92[3] = {0, 0, 0};static long W93[3] = {0, 0, 0};static long W94[3] = {0, 0, 0};
static long W111[3] = {0, 0, 0};static long W112[3] = {0, 0, 0};static long W113[3] = {0, 0, 0};static long W114[3] = {0, 0, 0};
    
long filtre900(int x){
    
    long output1;long output2;long output3;long output4;
    
    output1 = directFormII(a91,b91,a09,W91,x);
    
    output2 = directFormII(a92,b92,a09,W92,output1);
    
    output3 = directFormII(a93,b93,a09,W93,output2);
    
    output4 = directFormII(a94,b94,a09,W94,output3);
    
    return (2048+output4);
}

long filtre1100(int x){
    
    long output1;long output2;long output3;long output4;
    
    output1 = directFormII(a111,b111,a011,W111,x);
    
    output2 = directFormII(a112,b112,a011,W112,output1);
    
    output3 = directFormII(a113,b113,a011,W113,output2);
    
    output4 = directFormII(a114,b114,a011,W114,output3);
    
    return (2048+output4);
}

long directFormII(const int a[2], const int b[3], const int a0 ,long W[3], long input){
    
    long out;
    
    W[0] = (1024*input - a[0]*W[1] - a[1]*W[2]) >> 10;
    out = ( a0*( (b[0]*W[0] + b[1]*W[1] + b[2]*W[2]) >> 10) ) >> 10 ;
    
    W[2]=W[1];
    W[1]=W[0];
    
    return out;
}


