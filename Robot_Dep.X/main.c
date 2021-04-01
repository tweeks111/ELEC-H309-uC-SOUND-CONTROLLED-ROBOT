#include "init.h"
#include <xc.h>
#include "math.h"
#include "regulator.h"
#include "motor.h"
#include "timer.h"
#include "bluetooth.h"
#include "communication.h"
#include <libpic30.h>


int main(void){
    /*roues = 4''  cad 101.6mm
     * elles sont �cart�es de 215mm
     */
	oscillatorInit(OSC_FREQ_10MHZ);
    AD1PCFGL = 0xFFFF;  // toutes les pattes sont mises en num�rique 
    
    //initialisation de tout les �l�ments
    timerInit();
    motorInit(); 
    regulatorInit();
    timerInitRegulator();
    communicationInit();
   
    //initialisation des variables 
    float centSec = 0; 
    float sec = 0; 
    float time = 0; 
    POS1CNT = 0; 
    POS2CNT = 0; 
    char fin = 0;
    int instr ;
    float value = 0;
    
    // attente d'une seconde n�cessaire afin que la chaine d'acquisition puisse d�marrer correctement 
    while (value < 100){
        if (IFS0bits.T3IF){
            value = value + 1; 
            IFS0bits.T3IF = 0;
        }
    }
    while(U1STAbits.URXDA){
       value = U1RXREG;
    }
    value = 0; 
	while(1){
        if (fin == 0){
            instr = communicationIn();  //instuction du mode de d�placement
            value = communicationIn();  //instructiion de la valeur de d�placement
            fin = 1; centSec = 0; sec = 0; POS1CNT = 0; POS2CNT = 0; 
        }
        if (IFS0bits.T3IF){
            centSec +=1;
            if (centSec == 100){
                centSec = 0; sec+=1;
                }
            time = sec + (centSec/100); 
            IFS0bits.T3IF = 0;
            if (fin == 1){
                if (instr == 0b00){//avance
                    fin = translationRegulator(value/100.0, time);
                }
                else if (instr == 0b01){ // tourne � droite
                    fin = rotationRegulation(value, time);
                }
                else if (instr == 0b10){ //tourne � gauche
                    fin = rotationRegulation(-1*value, time);
                }
            }
        }
    }
}