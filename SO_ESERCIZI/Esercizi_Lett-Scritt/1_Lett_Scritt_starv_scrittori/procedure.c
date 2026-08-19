#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

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

void Scrittore(int sem, Buffer* buf) {

    InizioScrittura(sem);

    struct timeval t1;
    // struct timezone t2;
    gettimeofday(&t1, NULL); //Per avere un valore diverso ad ogni produzione
    msg val = t1.tv_usec;
    buf->messaggio = val;
    sleep(1);
    printf("Valore scritto=<%ld> \n", buf->messaggio);

    FineScrittura(sem);

}

void Lettore(int sem, Buffer* buf) {

    InizioLettura(sem, buf); 

    sleep(1); //Per simulare un ritardo di lettura
    printf("Valore letto=<%ld>, numero lettori=%d \n", buf->messaggio, buf->numlettori);

    FineLettura(sem, buf);
    
}

