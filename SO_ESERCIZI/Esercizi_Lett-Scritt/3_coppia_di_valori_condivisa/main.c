#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#include "header.h"

int main(void) {
    key_t chiave = ftok("/home/matteo/Documenti", 'b');
    key_t c_sem = ftok("/home/Scrivania/PythonProjects", 'c');
    int status, sem, id_shared, numprocessi;

    numprocessi = 3;

    pid_t pid;
    Buffer * buf;

    //Richiesta del buffer di memoria condiviso:
    id_shared = shmget(chiave, sizeof(Buffer), IPC_CREAT | 0664);
    printf("id_shared = %d\n", id_shared);
    buf = (Buffer *) (shmat(id_shared, 0, 0));

    //Inizializzazione del buffer:
    buf->numlettori = 0;
    buf->b.val_1 = 0;
    buf->b.val_2 = 0;

    //Richiesta di due semafori+inizializzazione:
    sem = semget(c_sem, 2, IPC_CREAT | 0664);
    semctl(sem, MUTEX_NUMLETTORI, SETVAL, 1);
    semctl(sem, MUTEX_LETTORI_SCRITTORI, SETVAL, 1);

    for(int i = 0; i<numprocessi; i++) {

        pid = fork();

        if(pid == 0) {

            //Sono nel processo figlio
            if (i == 0) {
                printf("Sono il figlio scrittore. Il mio pid %d\n", getpid());
                execl("./scrittore", "scrittore", NULL);
            }

            else if (i == 1) {
                printf("Sono il filgio lettore. Il mio pid %d\n", getpid());
                execl("./lettore", "lettore", "1", NULL);
            }

            else {
                printf("Sono il figlio lettore. Il mio pid %d\n", getpid());
                execl("./lettore", "lettore", "2", NULL);
            }

        }
    }

    for(int i=0; i<3; i++) {
        pid = wait(&status);
        if (pid == -1) {
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