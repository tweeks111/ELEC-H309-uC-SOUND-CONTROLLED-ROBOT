/* 
 * File:   regulator.h
 * Author: Dessart  Thibault
 *
 * Created on 13 mars 2019, 16:21
 */
void regulatorInit(void);
char translationRegulator(float csg, float time);
char rotationRegulation(float alphaMax, float time);
float posCalc(float t, float max, float v, float a);
float theoricMaxTime(float max);
float rotCalc(float t, float max); 
float theoricMaxTimeRot(float max); 