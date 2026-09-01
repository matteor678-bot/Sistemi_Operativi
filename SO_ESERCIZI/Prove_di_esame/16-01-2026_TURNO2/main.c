#include "sensore.h"
#include "aggregatore.h"
#include "collettore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {


    int id_coda_sensore = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
    if(id_coda_sensore < 0) {
        perror("Errore msgget");
        exit(1);
    }
    
    int id_coda_collettore = msgget(IPC_PRIVATE, IPC_CREAT | 0660);

    if(id_coda_collettore < 0) {
            perror("Errore msgget");
            exit(1);
    }

    printf("[MAIN] coda sensore: %d coda collettore %d\n", id_coda_sensore, id_coda_collettore);

    int pid;

    pid = fork();

    if(pid==0) {
        //figlio sensore
        printf("Avvio del processo sensore");

        sensore(id_coda_sensore);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork()");
        exit(1);
    }

    pid = fork();

    if(pid==0) {
        //figlio aggregatore
        printf("Avvio del processo aggregatore");

        aggregatore(id_coda_sensore, id_coda_collettore);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork()");
        exit(1);
    }

    pid = fork();

    if(pid==0) {
        //figlio collettore
        printf("Avvio del processo collettore");

        collettore(id_coda_collettore);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork()");
        exit(1);
    }

    for(int i=0; i<3; i++) {
        wait(NULL);
    }
    
    msgctl(id_coda_sensore, IPC_RMID, NULL);
    msgctl(id_coda_collettore, IPC_RMID, NULL);

    return 0;
}