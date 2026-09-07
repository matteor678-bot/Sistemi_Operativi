#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

void sensore_x(int coda_sensori) {

    printf("[Sensore X] Avviato...\n");
    messaggio_sensore m;
    srand(getpid());

    for(int i = 0; i < 10; i++) {

        int valore = rand() % 10;

        printf("[Sensore X] Invio %d\n", valore);
        m.type = X;
        m.val = valore;
        int err = msgsnd(coda_sensori,(void *)&m,sizeof(messaggio_sensore)-sizeof(long), 0);
        if(err!=0)
            perror("Errore msgsnd()\n");
        sleep(1);
    }

}


void sensore_y(int coda_sensori) {

    printf("[Sensore Y] Avviato...\n");
    messaggio_sensore m;
    srand(getpid());

    for(int i = 0; i < 10; i++) {

        int valore = rand() % 10;

        printf("[Sensore Y] Invio %d\n", valore);
        m.type = Y;
        m.val = valore;
        int err = msgsnd(coda_sensori,(void *)&m,sizeof(messaggio_sensore)-sizeof(long), 0);
        if(err!=0)
            perror("Errore msgsnd()\n");
        sleep(1);
    }

}