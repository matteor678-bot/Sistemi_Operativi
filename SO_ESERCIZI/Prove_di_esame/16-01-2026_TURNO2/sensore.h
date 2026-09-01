#ifndef _SENSORE_H_
#define _SENSORE_H_

#define TIPO 1

typedef struct {
    long type;
    int val; //Valore numerico casuale, generato tra 0 e 5
} messaggio;

void sensore(int id_coda_sensore);

#endif