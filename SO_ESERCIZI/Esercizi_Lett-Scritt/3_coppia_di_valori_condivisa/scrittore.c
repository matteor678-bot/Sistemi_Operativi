#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "header.h"

int main(void) {

    key_t chiave = ftok("/home/matteo/Documenti", 'b');
    key_t c_sem = ftok("/home/Scrivania/PythonProjects", 'c');
    int id_shared, sem;
    Buffer *buf;

    //Collegamento al buffer di memoria condiviso istanziato nel main:
    id_shared = shmget(chiave, sizeof(Buffer), 0);
    buf = (Buffer *) shmat(id_shared, 0, 0);

    //Collegamento alla struttura semaforica instanziata nel main:
    sem = semget(c_sem, 2, 0);

    //Fase di scrittura:
    for(int i = 0; i<5; i++) {
        srand(time(NULL));

        InizioScrittura(sem);

        sleep(1);
        buf->b.val_1 = rand() % 10;
        buf->b.val_2 = rand() % 10;
        printf("Valori scritti: val1=%d val2=%d \n", buf->b.val_1, buf->b.val_2);

        FineScrittura(sem);

    }

    exit(1);

}