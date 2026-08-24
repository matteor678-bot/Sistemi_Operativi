#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "header.h"

static int queue1; //STATICHE->Non è necessaria la loro visibilità fuori dal modulo
static int queue2;

//Inizializzazione delle code di servizio
void initServiceQueues() {
    queue1 = msgget(IPC_PRIVATE, IPC_CREAT|0664);
    queue2 = msgget(IPC_PRIVATE, IPC_CREAT|0664);
}

//Rimozione delle code di servizio
void removeServiceQueues() {
    msgctl(queue1, IPC_RMID, 0);
    msgctl(queue2, IPC_RMID, 0);
}

//Send Sincrona
void SendSincr(Messaggio * m, int queue) {
    Messaggio m1, m2;
    //Costruzione messaggio RTS
    m1.tipo = REQUEST_TO_SEND;
    strcpy(m1.mess, "Richiesta di invio");
    //Invio della RTS:
    msgsnd(queue1,&m1,sizeof(Messaggio)-sizeof(long),0);
    //Ricezione della OTS:
    msgrcv(queue2, &m2, sizeof(Messaggio)-sizeof(long), OK_TO_SEND, 0);
    //Invio del messaggio:
    msgsnd(queue,m,sizeof(Messaggio)-sizeof(long),0);
}

//Receive Bloccante
void ReceiveBloc (Messaggio *m, int queue, int tipomess) {
    Messaggio m1,m2; //Ricezione messaggio RTS
    msgrcv(queue1, &m1, sizeof(Messaggio)-sizeof(long), REQUEST_TO_SEND, 0); 
    m2.tipo = OK_TO_SEND;
    strcpy(m2.mess, "Ready to send");
    //Invio messaggio OTS
    msgsnd(queue2, &m2, sizeof(Messaggio)-sizeof(long), 0);
    //Ricezione del messaggio
    msgrcv(queue, m, sizeof(Messaggio)-sizeof(long), tipomess,  0);
}

void Produttore(int queue, char * text) {
    Messaggio m;
    //Costruzione del messaggio da trasmettere
    m.tipo =MESSAGGIO;
    strcpy(m.mess, text);
    //Invio del messaggio
    SendSincr(&m, queue); 
    printf("MESSAGGIO INVIATO: <%s> \n", m.mess);
}

void Consumatore(int queue) {
    Messaggio m;
    //Ricezione del messaggio
    ReceiveBloc(&m, queue, MESSAGGIO);
    printf("MESSAGGIO RICEVUTO: <%s> \n", m.mess);
}