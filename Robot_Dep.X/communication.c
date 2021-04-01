#include <xc.h>
#include <stdio.h>
void communicationInit(void){
    RPINR18bits.U1RXR = 8;
    RPOR4bits.RP9R = 3;
    U1BRG=21;
    U1MODEbits.BRGH=0;
    U1MODEbits.UARTEN=1;
    U1STAbits.UTXEN=1;
}
int communicationIn(void){
    //recoit les valeurs de la chaine d'acquisition
    int res = 0;
    while(!U1STAbits.URXDA);
    res = U1RXREG;
    return res; 
    }

