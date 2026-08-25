#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wait.h>

#include "header.h"

int  main(int argc, char *argv[]) {

    int status, queue_1, queue_2;
    pid_t pid;
    Messaggio *m;

    char coda_1 = argv[1][0];
    char coda_2 = argv [2][0];


    key_t chiave_1 = ftok("./", coda_1);
    key_t chiave_2 = ftok("./", coda_2);

    //Assegnazione coda di comunicazione:
    queue_1 = msgget(chiave_1, IPC_CREAT | 0664);
    queue_2  = msgget(chiave_2, IPC_CREAT | 0664);

    //Generazione del mittente:
    pid = fork();

    if(pid == 0) {
        printf("MITTENTE. Il mio PID:%d \n", getpid());
        char messaggio[40];

        while(true) {
             fgets(messaggio, sizeof(messaggio), stdin);
             if(strcmp(messaggio, "exit\n")==0) {
                Mittente(queue_2, messaggio);
                exit(1);
             }
             Mittente(queue_1, messaggio);
        }
    }

    //Generazione figlio ricevente:
    else {
        pid = fork();
        if(pid ==0) {
            printf("RICEVENTE. Il mio PID:%d \n", getpid());
            while(true) {
                m = Ricevente(queue_2);
                if(strcmp(m->mess, "exit\n") == 0) 
                    exit(1);
            }
        }
    }

    //Attesa di  terminazione: 
    for(int k=0; k<2; k++) {
        pid = wait(&status);
        if(pid == -1) 
            perror("Errore wait() \n");
        else
            printf("Figlio n.ro %d e\' morto con status:%d \n",  pid, status>>8);
    }

    //Deallocazione code:
    msgctl(queue_1, IPC_RMID, 0);
    msgctl(queue_2,   IPC_RMID, 0);

    //Deallocazione memoria dinamica:
    free(m);

    return 0;
}