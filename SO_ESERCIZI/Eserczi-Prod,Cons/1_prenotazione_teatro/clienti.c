#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#include "procedure.h"

int main(void) {

    sleep(5);

    key_t chiave = ftok("/home/matteo/Scrivania", 'a');

    int ds_shm = shmget(chiave, sizeof(Teatro), IPC_CREAT | 0664);

    if(ds_shm < 0) { perror("SHM errore"); exit(1); }

    Teatro * t;

    t = (Teatro *) shmat(ds_shm, NULL, 0);

    if( t == (void *)-1) {perror("Errore shmget!"); exit(1); }

    t->disponibilita = 80;
    for(int i = 0; i<DIM_BUFFER; i++) {
        t->posti[i].id_cliente = 0;
        t->posti[i].stato = LIBERO;
    }

    key_t chiavesem = IPC_PRIVATE;

    int sem_id = semget(chiavesem, 1, IPC_CREAT | 0664);

    if(sem_id<0) {perror("SEM errore"); exit(1); }

    //SEMAFORO COMPETIZIONE tra clienti
    semctl(sem_id, MUTEX_DISPONIBILITA, SETVAL, 1);

    for(int i = 0; i < NUM_CLIENTI; i++) {

        int pid = fork(); 

        if(pid == 0) {

            //Figlio cliente

            printf("Inizio del figlio cliente\n");

            prenotazione(t, sem_id);

            exit(1);
        }
    }

    for(int i = 0; i<NUM_CLIENTI; i++) {
        wait(NULL);
        printf("Figlio cliente terminato\n");
    }

    shmctl(ds_shm, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}