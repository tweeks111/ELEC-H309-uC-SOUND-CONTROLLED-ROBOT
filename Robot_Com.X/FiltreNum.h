#ifndef FILTRENUM_H
#define	FILTRENUM_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

typedef struct secondValues {
    long interval;
    long newval;
}secondValues;

secondValues sos(int coef[6], long *bridge, long input);

int filtre900(int input);

int filtre1100(int input);

#endif	/* FILTRENUM_H */