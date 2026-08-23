/* Il programma gestisce la  comunicazione tra due processi, modello asincrono 
viene inviato un burst di messaggi e stampato lo stato della coda
Header file: header.h
Programma chiamante: p_c_msg.c
Modulo delle procedure: procedure.c
Direttive per la compilazione dei moduli: Makefile
*/

#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"

int main(void) {

    int k, status, queue;
    pid_t pid;
    int i;
    char m[30];

    //Assegnazione coda di comunicazione
    queue = msgget(IPC_PRIVATE, IPC_CREAT | 0664);

    //Generazione produttore e consumatore
    pid = fork();

    if (pid==0) {
        printf("Sono il produttore. Il mio pid %d \n", getpid());
        for(i=0; i<10; i++) {
            sprintf(m, "stringa %d", i);
            usleep(100);
            Produttore(queue, m);
        }
        
        _exit(0);
    }

    else {
        pid = fork(); //Genero il secondo figlio: Consumatore
        if (pid == 0) {
            printf("Sono il figlio consumatore. Il mio pid %d \n", getpid());
            sleep(1);
            for(i=0; i<10;i++) {
                Consumatore(queue);
            }
            _exit(0);
        }
    }

    //Attesa di terminazione
    for(k=0; k<2;k++) {
        pid = wait(&status);
        if(pid==-1) 
            perror("Errore");
        else 
            printf("Figlio n.ro %d e\' morto con status= %d\n", pid, status>>8);
            
    }

    //Deallocazione code
    msgctl(queue, IPC_RMID, 0);
    return 1;

}