#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

int main() {

    /* TBD: Allocare le due code di messaggi */
    int coda_sensore = msgget(IPC_PRIVATE, IPC_CREAT | 0664);
    int coda_attuatore = msgget(IPC_PRIVATE, IPC_CREAT | 0664);

    pid_t pid;

    pid = fork();
    
    if(pid == 0) {
        //Sono il sensore_x
        sensore_x(coda_sensore);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork()");
    }

    pid = fork();
    
    if(pid == 0) {
        //Sono il sensore_y
        sensore_y(coda_sensore);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork()");
    }

    pid = fork();

    if(pid == 0) {
        //Sono il planner
        planner(coda_sensore, coda_attuatore);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork()");
    }

    pid = fork();

    if(pid == 0) {
        //Sono l'attuatore
        attuatore(coda_attuatore);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork()");
    }


    /* TBD: Attendere la terminazione dei processi figli e deallocare le risorse */
    for(int i = 0; i<4; i++) {
        wait(NULL);
    }

    msgctl(coda_sensore, IPC_RMID, NULL);
    msgctl(coda_attuatore, IPC_RMID, NULL);
    return 0;
}