#include "init.h"
#include "adc.h"
#include "math.h"
#include "FskDetector.h"
#include "filtre.h"
#include <xc.h>

#define FCY 40000000                          // Fcy = Focs/2                      
#define fe 16000                              // Frequence d'échantillonage
#define baudrate 115200                       // Baudrate
#define Ne 20                                 // Nombre d'échantillons par "période"      
#define th900 2170
#define th1100 2200
//On vérifie 800 fois sur 20 échantillons, ce qui fait un total de 1600 échantillons 
//ce qui correspond à 100ms


int main(void){

    oscillatorInit(OSC_FREQ_10MHZ);
    adcPollingInit();                   
    
    PR3 = FCY/fe;
    T3CONbits.TON = 1;
    
    U1BRG = (FCY/(16*baudrate))-0.5;         // Pour arrondir correctement on fait +0.5 à la formule de base
    U1MODEbits.BRGH=0;                       
    U1MODEbits.UARTEN=1;                     // enable UART
    U1STAbits.UTXEN=1;                       // enable transmission
    
    RPINR18bits.U1RXR=6;                     // Configuration de RP6 en réception RX
    RPOR3bits.RP7R =3;                       // Configuration de RP7 en transmission TX
    
    //comTest();                             // Vérification de la communication entre les deux dsPic
    
    TRISBbits.TRISB5 = 0;                    // Confirme que le téléversement et le test de com se 
    LATBbits.LATB5 = 0;
    TRISBbits.TRISB8 =0;                     // LED 900Hz
    LATBbits.LATB8=0;
    TRISBbits.TRISB9 =0;                     // LED 1100Hz
    LATBbits.LATB9=0;
    int i;
    int n;
    int msg;
    
    int x;
    int data=0;
    
    long y900;
    long y1100;
    
    char detector900;
    char detector1100;
    
    while(1){
        i=0;
        msg=0;
        while(!msg){
            n=0;
            detector1100=0;
            detector900=0;
            while(n<Ne){
                if(IFS0bits.T3IF){
                    IFS0bits.T3IF=0;
                    adcPollingStart();
                    while(!adcPollingConversionFinished());  //Tant que la conversion n'est pas finie on attend

                    x=adcPollingRead();
                    y900 = filtre900(x);
                    y1100 = filtre1100(x);

                    if ((y900>=th900) || (y900 <= (4096-th900))){   //Si le seuil est dépassé, detector900 passe à 1
                        detector900 =1;                             //sur toute la période Ne
                        LATBbits.LATB9=1; 
                    }

                    if ((y1100>=th1100) || (y1100 <= (4096-th1100))){
                        detector1100 = 1;
                        LATBbits.LATB8=1;
                    }
                    n++;
                }  
            }
            if(!detector1100) LATBbits.LATB8=0;    
            if(!detector900) LATBbits.LATB9=0;
            msg=fskDetector(detector900,detector1100);
        }
            if(LATBbits.LATB5)LATBbits.LATB5=0;
            else LATBbits.LATB5=1;
            send(msg>>8);     //Envoi du message sur 2 bytes au deuxième dsPic
            send(msg);
           
            
    }
       
}  

void oscilloTest(int x, long y900, long y1100){ 
        //Fonction servant à vérifier si le filtrage fonctionne correctement
        //en envoyant les données à la fonction python Oscilloscope
    if (U1STAbits.URXDA){
        //int data = U1RXREG;
        while(U1STAbits.UTXBF);
        U1TXREG=x/16;
        while(U1STAbits.UTXBF);
        U1TXREG=y900/16;
        while(U1STAbits.UTXBF);
        U1TXREG=y1100/16;
    }
}

void send(int msg){
    while(U1STAbits.UTXBF);
    U1TXREG=msg;   
}
