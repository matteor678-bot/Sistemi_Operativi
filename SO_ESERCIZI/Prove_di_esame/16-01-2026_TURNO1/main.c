#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include "header.h"

#define REQUEST_TO_SEND 1
#define OK_TO_SEND 2
#define MESSAGGIO 3

static int queue1;
static int queue2;

void Produttore(MonitorProdCons *p) {

    srand(getpid());

    for(int i = 0; i < 5; i++) {

        int valore = rand() % 10;

        printf("Produzione [%d]: %d\n", getpid(), valore);

        produzione(p, valore);

    }
}

void Consumatore(MonitorProdCons *a, MonitorProdCons *b, int queue) {

    Messaggio m, m1, m2;

    for(int i = 0; i < 5; i++) {

        int valore1 = consumazione(a);

        printf("[Consumatore] Consumazione n.1: %d\n", valore1);

        int valore2 = consumazione(b);

        printf("[Consumatore] Consumazione n.2: %d\n", valore2);

        int valore_tot = valore1 + valore2;

        m1.type = REQUEST_TO_SEND;
        m1.val = 0;
        //Invio della RTS:
        msgsnd(queue1, &m1, sizeof(Messaggio)-sizeof(long), 0);
        //Ricezione della OTS:
        msgrcv(queue2, &m2, sizeof(Messaggio)-sizeof(long), OK_TO_SEND, 0);
        //Invio del messaggio
        m.type = MESSAGGIO;
        m.val = valore_tot;
        msgsnd(queue, &m, sizeof(Messaggio)-sizeof(long), 0);

    }
}


void Gestore(int queue, int tipomess) {

    Messaggio m, m1, m2;
    for(int i = 0; i < 5; i++) {
        //Ricezione della RTS:
        msgrcv(queue1, &m1, sizeof(Messaggio) - sizeof(long), REQUEST_TO_SEND, 0);
        //Invio della OTS
        m2.type = OK_TO_SEND;
        m2.val = 0;
        msgsnd(queue2, &m2, sizeof(Messaggio) - sizeof(long), 0);
        //Ricezione del messaggio:
        msgrcv(queue, &m, sizeof(Messaggio) - sizeof(long), tipomess, 0);
        int valore_totale = m.val;

        printf("[Gestore] Valore totale ricevuto: %d\n", valore_totale);

    }

}


int main() {

    key_t chiave_shma = IPC_PRIVATE;

    int id_shma = shmget(chiave_shma, sizeof(MonitorProdCons), IPC_CREAT | 0664);

    if(id_shma<0) {
        perror("errore shma");
        exit(1);
    }

    MonitorProdCons *a =(MonitorProdCons *)shmat(id_shma, NULL, 0);

    if( a ==(void *) -1) {
        perror("Errore shmat");
        exit(1);
    }

    init_monitor_prodcons(a);

    key_t chiave_shmb = IPC_PRIVATE;

    int id_shmb = shmget(chiave_shmb, sizeof(MonitorProdCons), IPC_CREAT | 0664);

    if(id_shmb<0) {
        perror("errore shmb");
        exit(1);
    }

    MonitorProdCons *b =(MonitorProdCons *)shmat(id_shmb, NULL, 0);

    if( b ==(void *) -1) {
        perror("Errore shmat");
        exit(1);
    }

    init_monitor_prodcons(b);

    //Inizializzazione queue di servizio:
    queue1 = msgget(IPC_PRIVATE, IPC_CREAT | 0664);
    queue2 = msgget(IPC_PRIVATE, IPC_CREAT | 0664);

    int queue;
    //Inizializzazione della queue per il trasferimento del messaggio:
    queue = msgget(IPC_PRIVATE, IPC_CREAT | 0664);

    pid_t pid;

    for(int j =0; j<2; j++) {

        pid = fork();

        if(pid == 0) {
            //figlio Produttore
            printf("Inizio Produttore[%d]\n", j+1);

            if(j==0) 
                Produttore(a);
            else
                Produttore(b);

            exit(0);
        }

        else if( pid<0) {
            perror("Errore fork");
            exit(1);
        }
    }


    pid = fork();

    if(pid == 0) {
        //figlio Consumatore
        printf("Inizio Consumatore\n");

        Consumatore(a, b, queue);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork");
        exit(1);
    }

    pid = fork();

    if(pid == 0) {
        //figlio Gestore
        printf("Inizio Gestore\n");

        Gestore(queue, MESSAGGIO);

        exit(0);
    }
    else if(pid<0) {
        perror("Errore fork\n");
        exit(1);
    }

    for(int i = 0; i<4; i++) {
        wait(NULL);
    }

    /* TBD: De-inizializzare le risorse IPC */
    //Rimozione dei monitor:
    remove_monitor_prodcons(a);
    remove_monitor_prodcons(b);

    //Rimozione memorie condivise:
    shmctl(id_shma, IPC_RMID, 0);
    shmctl(id_shmb, IPC_RMID, 0);

    //Rimozione code di servizio:
    msgctl(queue1, IPC_RMID, 0);
    msgctl(queue2, IPC_RMID, 0);

    //Rimozione coda di messaggi:
    msgctl(queue, IPC_RMID, 0);

    return 0;
}