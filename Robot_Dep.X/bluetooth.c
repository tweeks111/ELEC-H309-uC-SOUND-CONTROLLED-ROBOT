#include <xc.h>
#include <stdio.h>
void bluetoothInit(void){
    TRISBbits.TRISB2 = 0; 
    PORTBbits.RB2 = 1; 
     
    RPINR18bits.U1RXR = 15; 
    RPOR7bits.RP14R = 0b00011;
    RPINR18bits.U1CTSR = 3; 
    RPOR8bits.RP16R = 4; 
    
    
    U1MODEbits.BRGH = 0; // configure le baudrate 
    U1BRG = 21; 
    U1MODEbits.PDSEL = 00 ;  // format de donées 
    U1MODEbits.UARTEN = 1; // active l'UART
    U1STAbits.UTXEN = 1 ;//active la transmission
}
    
void bluetoothResponse(int data){
     //if (U1STAbits.URXDA){
             U1RXREG; 
             while (U1STAbits.UTXBF){ //vérifie qu'il y a de la place dans le buffer d'envoie 
             }
             U1TXREG = data;  //réponse limitée à 256
        // }
}
void bluetoothBegin(float attente){
    float time = 0; 
    while (time < attente){
            if (IFS0bits.T3IF){
                time += 0.01;
            }
    }
    while (U1STAbits.URXDA){
        U1RXREG;
        }  
    }

    
int bluetoothInstruction(){
    int res; 
    int g = U1STAbits.URXDA; 
    while (!U1STAbits.URXDA){
        res = U1RXREG; 
        g = U1STAbits.URXDA;
    }
return res ; 
}