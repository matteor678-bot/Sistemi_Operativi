
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "header.h"

int main(void) {

    int sem, id_shared, k, numsem, numlettori, numscrittori;
    key_t chiave = IPC_PRIVATE;
    key_t c_sem = IPC_PRIVATE;
    int status, numprocessi;
    pid_t pid;
    Buffer *buf;

    numlettori = 6;
    numscrittori = 6;
    int num_processi = numlettori + numscrittori;

    //Richiesta del buffer di memoria condiviso:
    id_shared = shmget(chiave, sizeof(Buffer), IPC_CREAT | 0664);
    printf("id_shared = %d\n", id_shared);
    buf = (Buffer*) (shmat(id_shared, 0, 0));

    //Inizializzazione del buffer:
    buf->numlettori = 0;
    buf->messaggio = 0;

    //Richiesta di due semafori + inizializazzione:
    sem = semget(c_sem, 2, IPC_CREAT | 0664);
    semctl(sem, MUTEX_NUMLETTORI, SETVAL, 1);
    semctl(sem, MUTEX_LETTORI_SCRITTORI, SETVAL, 1);

    //Generazione di scrittori e lettori:
    for(k=0; k<num_processi; k++) {

        pid = fork();

        if (pid == 0) {
            //Sono nel processo figlio
            if ( (k%2) == 0) {
                printf("Sono il figlio scrittor. Il mio pid %d\n", getpid());
                Scrittore(sem, buf);

            }

            else {
                printf("Sono il figlio lettore. Il mio pid %d\n", getpid());
                Lettore(sem, buf);
            }

            exit(0);
        }



    }

    for(k=0; k<num_processi; k++) {
        pid=wait(&status);
        if (pid ==-1) {
            perror("Errore wait()");
        }
        else
            printf("Figlio n.ro %d è morto con status=%d\n", pid, status);
    }

    //Deallocazione dei semafori e della memoria condivisa:

    shmctl(id_shared, IPC_RMID, 0);
    semctl(sem, 0, IPC_RMID);

    return 0;

}