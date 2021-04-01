#include <xc.h>
#include "math.h"
#include "bluetooth.h"

float posCalc(float t, float max, float v, float a);
float theoricMaxTime(float max, float a, float v, float time); 
float theoricMaxTimeRot(float max);
float rotCalc(float t, float max);
  
float kpR = 1.89656/2; 
float kp = 8.49;
float Uot = 0.15; 
float Uor = 0.25; 

void regulatorInit(void){
    
    RPINR14bits.QEA1R = 24;// RP24R Encl droit A
    RPINR14bits.QEB1R = 25; //RP25R ENcle driot B
    RPINR16bits.QEA2R = 19; //RP19R Encl gauche A
    RPINR16bits.QEB2R = 20; //RP20R Encl gauche B
   
    QEI1CONbits.QEIM = 0b111; //reset quand il arrive au max 
    QEI2CONbits.QEIM = 0b111; 
    
    QEI1CONbits.SWPAB = 1; // change le sens positif de rotation 
    QEI2CONbits.SWPAB = 0; 
}

float abs(float x){
    if(x<0){
        x = x*-1; 
    }
    return x;
}

char translationRegulator(float max, float time){
    char res = 1; 
    int x = POS1CNT; 
    int y = POS2CNT; 
    float eps = 0.025;
    float gauche = (x/180.0)*3.14 * (101.6/2.0)/1000.0;
    float droit = (y/180.0)*3.14 * (101.6/2.0)/1000.0; 
    float moy = (gauche+droit)/2.0;
    int alphaMax = 0;    //rotation nulle 
    
    float alpha; float rapRot;
    alpha = (101.6/215.0)*(y-x)*(3.14/180.0); 
    rapRot = (kpR *(alphaMax - alpha)+Uor)*2500; //calcul du rapport cyclique 
    float v = 0.4; //vitesse
    float a = 0.5; //accélération
    float pos; 
    if (max > 0){  //regarde si le robot doit avancer ou reculer 
        pos = posCalc(time, max, v, a);
    }
    else{
        pos = -1*posCalc(time, max, v, a);
    }
    if(abs(max-moy) > eps && time <= theoricMaxTime(max, a, v, v/a) + 0.5){                      
        float rapDroit = (kp*(pos - droit)+Uot)*2500;        
        float rapGauche = (kp*(pos - gauche)+Uot)*2500; 
        OC1RS = 7500 - rapDroit + rapRot;
        OC2RS = 7500 + rapGauche - rapRot; 
    }
    else{
        OC1RS = 7500; OC2RS = 7500;
        res = 0;
        POS1CNT = 0; POS2CNT = 0; 
  }
    return res; 
}

float posCalc(float t, float max, float v, float a){
    max = abs(max);
    float time = v/a;   //temps d'acceleration
    float tMax = theoricMaxTime(max, a , v, time);
    float res;
     if (time*v > max){ //cas triangulaire
        if (t <= tMax/2.0){
            res = (a/2.0)*t*t; 
        }
        else{
            v = a*tMax/2.0;
            res = max-(a/2.0)*(tMax/2.0)*(tMax/2.0) + v *(t-(tMax-(tMax/2.0)))-(a/2.0)*(t-(tMax-(tMax/2.0)))*(t-(tMax-(tMax/2.0)));  //ATTENTION 2.0 
        }
    }
     else{ //cas trapezoidale
        if (t<=time){
            res = (a/2.0)*t*t; 
        }
        else if (t>time && t<(tMax-time)){
            res = (time*time*(a/2.0)) + v*(t-time); 
        }

        else if (t>=(tMax-time)){ 
            res = max-(time*time*(a/2.0)) + v *(t-(tMax-time))-(a/2.0)*(t-(tMax-time))*(t-(tMax-time));  // 0.25 => a/2.0
        }
        else {
            res = max;
        }
    }   
    return res; 
}

float theoricMaxTime(float max, float a, float v, float time){
   float res; 
   max = abs(max);
   if (time*v > max){
       res = sqrt(max/a)*2; //cas triangulaire 
   }
   else {   //cas trapezoidale
       res = (2*time) +(max - (time*time*a))/v;
   }
   return res;
}

char rotationRegulation(float alphaCsg, float time){
    float mult = 1; 
    if (alphaCsg<0){ //sert a savoir si on va à gauche ou à droite
        mult = -1; 
    }
    char fin = 1; 
    int x = POS1CNT; 
    int y = POS2CNT; 
    float eps = 0.1; 
    float alphaMax = 2*(alphaCsg/180.0)*3.14; 
    float alpha = ((101.6/215.0)*(y-x)*(3.14/180.0));// %2

    int v = 3;  //vitesse angulaire 
    int a = 1.5; //accélération angulaire

    float rot = mult*posCalc(time, alphaMax,v,a);
    float rap = (kpR*(rot - alpha))*2500;

    
    float gauche = (x/180.0)*3.14 * (101.6/2.0)/1000.0;  
    float droit = (y/180.0)*3.14 * (101.6/2.0)/1000.0; 
    float moy = (gauche+droit)/2.0; 
    float rap2 = (kp*(0-moy))*2500; //impose une translation nulle 
    
    if(abs(alphaMax-alpha) > eps && time <= theoricMaxTime(alphaMax, a, v, v/a) + 0.5){
        OC1RS = 7500 - rap - rap2; //droit     
        OC2RS = 7500 - rap + rap2; //gauche
    }
    else{
        OC1RS = 7500; OC2RS = 7500;
        fin = 0;
        POS1CNT = 0; POS2CNT = 0; 
    }    
    return fin; 
}

