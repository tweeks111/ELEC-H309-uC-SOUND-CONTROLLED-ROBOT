#include <xc.h>
void motorInit(void){
    RPOR9bits.RP18R = 0b10010; //correspond à OC1 moteur gauche 
    RPOR6bits.RP13R = 0b10011; //correspond à OC2 moteur droit 
    
    AD1PCFGLbits.PCFG8 = 1;  //met ma pin analogique 8 en digital
    OC1CONbits.OCTSEL = 0; //alloue le timer 2 a OCx
    OC2CONbits.OCTSEL = 0; 
  
    OC1R = 7500; 
    OC1RS = 7500;    //moteur gauche  max 10000
    OC2R = 7500; 
    OC2RS = 7500;     //moteur droit max 5000
    
    OC2CONbits.OCM = 6; 
    OC1CONbits.OCM = 6;  
}
