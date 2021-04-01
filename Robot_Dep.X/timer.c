#include <xc.h>
void timerInit(void){
    PR2 = 15000;
    T2CONbits.TCKPS = 0b01; //prescaler de 8
    T2CONbits.TON = 1;
}
void timerInitRegulator(void){
    /*initialise la fréquence du régulateur a 100Hz*/
    PR3 = 50000; 
    T3CONbits.TCKPS = 0b01; 
    T3CONbits.TON = 1;
}
