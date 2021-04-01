/* 
 * File:   init.h
 * Author: Kevin De Cuyper
 *
 * Created on 29 janvier 2013, 14:34
 */



/* This function initialises the main oscillator to work with an external
 * quartz crystal, at FOSC = 80MHZ (=> ths uC works at 40MIPS).
 * The parameter defines the resonance frequency of the quartz crystal.
 * 
 * this function accepts only two quartz frequencies : 8MHz and 10MHz
 * Using the correspondong define constants is recommended.
 */
#define OSC_FREQ_8MHZ   8
#define OSC_FREQ_10MHZ  10
void oscillatorInit(int oscFreqInMHz);
