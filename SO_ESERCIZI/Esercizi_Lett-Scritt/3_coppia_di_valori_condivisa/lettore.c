#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "header.h"

int main(int argc, char *argv[]) {

    key_t chiave = ftok("/home/matteo/Documenti", 'b');
    key_t c_sem = ftok("/home/Scrivania/PythonProjects", 'c');
    int id_shared, sem;
    Buffer *buf;

    //Collegamento al buffer di memoria condiviso istanziato nel main:
    id_shared = shmget(chiave, sizeof(Buffer), 0);
    buf = (Buffer *) shmat(id_shared, 0, 0);

    //Collegamento alla struttura semaforica instanziata nel main:
    sem = semget(c_sem, 2, 0);

    //Fase di lettura:
    for( int i = 0; i<5; i++) {
    
    InizioLettura(sem, buf);

    if(strcmp(argv[1], "1")) {
        
        sleep(2); //Simulo il ritardo della lettura
        printf("Val1=<%d>, numero lettori=%d \n", buf->b.val_1, buf->numlettori);
    }

    else {

        sleep(2); //Simulo ritardo della lettura
        printf("Val2=<%d>, numero lettori=%d \n", buf->b.val_2, buf->numlettori);
    }

    FineLettura(sem, buf);
    
    }

    exit(1);
    
}