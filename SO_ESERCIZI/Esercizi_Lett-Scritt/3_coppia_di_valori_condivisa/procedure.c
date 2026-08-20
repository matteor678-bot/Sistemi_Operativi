#include "header.h"
#include "semafori.h"

void InizioLettura(int sem, Buffer* buf) {

    Wait_Sem(sem, MUTEX_NUMLETTORI); //Incremento numlettori in mutua esclusione

    buf->numlettori = buf->numlettori+1;

    if (buf->numlettori == 1) //Se si tratta del primo lettore blocca gli altri scrittori
        Wait_Sem(sem, MUTEX_LETTORI_SCRITTORI);

    Signal_Sem(sem, MUTEX_NUMLETTORI); //Rilascia il mutex per fare entrare altri lettori
}

void FineLettura(int sem, Buffer* buf) {

    Wait_Sem(sem, MUTEX_NUMLETTORI); //Indica ai lettori che sto terminando la letttur, decremento numlettori in mutua esclusione

    buf->numlettori = buf->numlettori - 1;

    if (buf->numlettori == 0) //Se sono l'ultimo lettore devo rilasciare la risorsa per gli scrittori
        Signal_Sem(sem, MUTEX_LETTORI_SCRITTORI);

    Signal_Sem(sem, MUTEX_NUMLETTORI); //Rilascio il mutex per altri lettori
}

void InizioScrittura(int sem) {
    Wait_Sem(sem, MUTEX_LETTORI_SCRITTORI);
}

void FineScrittura(int sem) {
    Signal_Sem(sem, MUTEX_LETTORI_SCRITTORI);
}