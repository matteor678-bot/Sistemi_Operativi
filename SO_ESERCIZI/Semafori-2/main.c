#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>

#include "processi.h"

int main(void) {

    int vett_id;
    int buffer_id;
    int sem_id;
    int *vettore;
    int *buffer;

    vett_id = shmget(IPC_PRIVATE, NUM_ELEMENTI*sizeof(int), IPC_CREAT | 0664);

    if(vett_id<0) {
        perror("Impossibile creare l'array di interi condiviso");
        exit(1);
    }

    vettore = shmat(vett_id, 0, 0);

    if(vettore == (void *)-1) {
        perror("Impossibile inizializzare l'array di interi condiviso");
        exit(1);
    }

    srand(time(NULL));

    for(int i= 0; i<NUM_ELEMENTI; i++) {

        vettore[i] = rand() % INT_MAX; //Intero casuale compreso tra 0 e (INT_MAX-1)

    }

    buffer_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0664);

    if(buffer_id<0) {
        perror("Impossibile creare il buffer condiviso");
        exit(1);
    }

    buffer = shmat(buffer_id, 0, 0);

    if(buffer == (void *)-1) {
        perror("Impossibile inizializzare il buffer condiviso");
        exit(1);
    }

    *buffer = INT_MAX;

    sem_id = inizializza_semafori();

    for(int i=0; i<NUM_PROCESSI; i++) { //Crea 10 processi figli

        pid_t pid = fork();

        if(pid<0) {
            perror("Errore FORK");
            exit(1);
        }

        if(pid == 0) {
            //Sono nel processo FIGLIO
            printf("Processo[%d] PID=%d creato \n", i, getpid());

            figlio(vettore, buffer, sem_id, i*1000, 1000);

            exit(0);
        }
    }

    //Sono il PADRE

    padre(buffer, sem_id);

    //Deallocazione risorse IPC
    semctl(sem_id, 0, IPC_RMID);
    shmctl(vett_id, IPC_RMID, 0);
    shmctl(buffer_id, IPC_RMID, 0);

    return 0;
}