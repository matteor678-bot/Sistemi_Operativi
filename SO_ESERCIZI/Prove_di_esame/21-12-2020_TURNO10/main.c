#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include "header.h"

#define NUM_VIEWERS 3
#define NUM_UPDATERS 2


int main(){

    pid_t pid;
    int status, k;
    
    //Creazione code richieste e risposte
    int ds_queue_request, ds_queue_response;

    //create queues
    int key_queue_request = ftok(".", 'a');
    int key_queue_response = ftok(".", 'b');
    
    // TODO aggiungere codice di inizializzazione delle code
    ds_queue_request = msgget(key_queue_request, IPC_CREAT | 0664);
    ds_queue_response = msgget(key_queue_response, IPC_CREAT | 0664);
    
    printf("[master] Coda ds_queue_request creata! ID: %d\n", ds_queue_request);
    printf("[master] Coda ds_queue_response creata! ID: %d\n", ds_queue_response);
    

    for(int i=0; i<NUM_VIEWERS; i++) {
        pid = fork();
        sleep(1);
        if(pid == 0) {
            execl("./viewer", "viewer", NULL);
        }

        else if(pid<0) {
            perror("Errore fork()");
        }

    }
    

    // TODO aggiungere codice per generare NUM_UPDATERS processi updaters, attraverso l'exec
    for(int i=0; i<NUM_UPDATERS; i++) {
        pid = fork();
        sleep(1);
        if(pid == 0) {
            execl("./updater", "updater", NULL);
        }

        else if(pid<0) {
            perror("Errore fork()");
        }

    }
    
    pid = fork();

    if(pid == 0) {
            execl("./store", "store", NULL);
        }

    else if(pid<0) {
            perror("Errore fork()");
        }
    
    for (k=0; k<NUM_VIEWERS+NUM_UPDATERS+1; k++){
        pid = wait(&status);
        if (pid == -1)
            perror("errore");
        else
            printf ("[master] Figlio n.ro %d e\' morto con status= %d \n", pid, status);
    }

    msgctl(ds_queue_request, IPC_RMID, NULL);
    msgctl(ds_queue_response, IPC_RMID, NULL);

    printf("[master] code rimosse\n");
    
    return 0;
}