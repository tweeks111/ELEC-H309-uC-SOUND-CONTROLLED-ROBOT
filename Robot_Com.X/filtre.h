/* 
 * File:   filtre.h
 * Author: Théo Lepoutte
 *
 * Created on 9 mai 2019, 11:28
 */

#ifndef FILTRE_H
#define	FILTRE_H


        void filtres(int);
        long filtre900(int);
        long filtre1100(int);
        long directFormII(const int a[2],const int b[3], const int a0, long w[3],long input);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FILTRES_H */

