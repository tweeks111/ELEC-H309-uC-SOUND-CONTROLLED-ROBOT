#include <xc.h>


void adcPollingInit(void)
{
    AD1CON1bits.AD12B = 1;  // Convertisseur sur 12 bits
    AD1CON3bits.ADCS = 5;   // Clock de l'adc
    AD1CON1bits.ASAM = 1;   // auto sample activé
    AD1CSSLbits.CSS0 = 1;   // Le convertisseur doit scanner la patte AN0
    AD1PCFGLbits.PCFG0 = 0; // AN0 en mode analogique
    AD1CON1bits.SSRC = 2;   // ADC activé manuellement (en software) => 0 , timer =>2
    AD1CON1bits.ADON = 1;   // l'ADC est actif
}
void adcTimerInit(int pr){
   PR3 = pr;
   T3CONbits.TON = 1; 
}


void adcPollingStart(void)
{
    AD1CON1bits.SAMP = 0;
}


int adcPollingConversionFinished(void)
{
    return (AD1CON1bits.DONE);
}


int adcPollingRead(void)
{
    if (AD1CON1bits.DONE) {
        return ADC1BUF0;
    } else {
        return (-32768);
    }
}

