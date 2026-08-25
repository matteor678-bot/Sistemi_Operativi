#include <string.h>
#include <sys/msg.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>

#include "header.h"

void  Mittente(int queue, char * text) {

    Messaggio m;
    //Costruzione del messaggio da trasmettere:
    m.tipo = MESSAGGIO;
    strcpy(m.mess, text);

    //Trasmissione del messaggio sulla queue passata come argomento:
    int n = msgsnd(queue, (void *)&m, sizeof(Messaggio)-sizeof(long), IPC_NOWAIT);
    if(n == -1) {
        printf("Le code sono state rimosse: USICTA\n");
        exit(1);
    }
    printf("MESSAGGIO INVIATO: %s", m.mess);
}

Messaggio* Ricevente(int queue) {
    Messaggio* m = (Messaggio *)malloc(sizeof(Messaggio)) ;
    //Ricezione del messaggio:
    int n = msgrcv(queue, (void  *)m, sizeof(Messaggio)-sizeof(long), MESSAGGIO, 0);
    if(n == -1) {
        printf("Le code sono state rimosse: USICTA\n");
        exit(1);
    }
    printf("MESSAGGIO RICEVUTO: %s", m->mess);
    return m;
}