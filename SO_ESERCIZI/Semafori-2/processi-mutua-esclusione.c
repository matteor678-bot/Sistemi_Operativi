#include "processi.h"
#include "semafori.h"

#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int inizializza_semafori() {
    int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0664);

    if(sem_id<0) {
        perror("Impossibile creare il semaforo per la mutua esclusione");
        exit(1);
    }

    int err = semctl(sem_id, 0, SETVAL, 1);

    return sem_id;
}

void figlio(int *vettore, int *buffer, int sem_id, int elemento_iniziale, int qta_elementi) {
    
    int minimo = INT_MAX;
    
    for ( int i = elemento_iniziale; i < elemento_iniziale + qta_elementi; i++) {

        if(vettore[i]<minimo) {

            minimo = vettore[i];

        }
    }

    printf("Figlio: Il minimo locale è %d\n", minimo);

    Wait_Sem(sem_id, 0);
    //INIZIO DELLA SEZIONE CRITICA
    if(minimo < *buffer) {

        *buffer = minimo;

    }
    //FINE SEZIONE CRITICA

    Signal_Sem(sem_id, 0);

}

void padre(int *buffer, int sem_id) 
{
    for(int i = 0; i<10; i++) {
        wait(NULL);
    }

    Wait_Sem(sem_id, 0);

    printf("Padre: Il valore minomo assoluto è: %d\n", *buffer);

    Signal_Sem(sem_id, 0);
}