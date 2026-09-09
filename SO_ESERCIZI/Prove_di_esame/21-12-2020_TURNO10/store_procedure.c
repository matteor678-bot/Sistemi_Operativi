#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "store.h"

extern int ds_queue_request, ds_queue_response;

void Wait_Sem(int ds_sem, int id_sem) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = id_sem;
    buf.sem_op = -1;
    semop(ds_sem,&buf,1);
}

void Signal_Sem(int ds_sem, int id_sem) {
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = id_sem;
    buf.sem_op = 1;
    semop(ds_sem,&buf,1);
}

void init_magazzino(Magazzino *magazzino){
    // TODO inizializzazione dei semafori , delle variabili necessarie alla sincronizzazione
    // e delle scorte di magazzino iniziali
    magazzino->synch_sem = semget(IPC_PRIVATE, MAX_PRODOTTI, IPC_CREAT | 0664);
    magazzino->mutexl_sem = semget(IPC_PRIVATE, MAX_PRODOTTI, IPC_CREAT | 0664);
    for(int i=0; i<MAX_PRODOTTI; i++) {
        semctl(magazzino->synch_sem,i,SETVAL,1);
        semctl(magazzino->mutexl_sem,i,SETVAL,1);
        magazzino->num_lettori[i] = 0;
        magazzino->scorta[i] = SCORTA_INIZIALE;
    }
}

void destroy_magazzino(Magazzino *magazzino){
    semctl(magazzino->synch_sem, 0, IPC_RMID);
    semctl(magazzino->mutexl_sem, 0, IPC_RMID);
}

void lettore(Magazzino *magazzino) {
    int ret;
    Msg_Req m_req;
    Msg_Resp m_resp;
    int k;

    for (k=0; k<4; k++) {
        msgrcv(ds_queue_request, (void *)&m_req, sizeof(Msg_Req)-sizeof(long), READ, 0);

        InizioLettura(magazzino, m_req.id_prodotto);

        
        ret = magazzino->scorta[m_req.id_prodotto];

        FineLettura(magazzino, m_req.id_prodotto);

        m_resp.pid = m_req.mittente;
        m_resp.id_prodotto = m_req.id_prodotto,
        m_resp.scorta = ret;

        printf("[lettore] Ricevuta richiesta di lettura per prodotto %d con livello scorte %d\n",m_resp.id_prodotto,m_resp.scorta);

        msgsnd(ds_queue_response, (void *)&m_resp, sizeof(Msg_Resp)-sizeof(long), 0);
    }
}

void scrittore(Magazzino *magazzino){
    int ret;
    Msg_Req m_req;
    int k;

    for (k = 0; k < 2; k++) {
        msgrcv(ds_queue_request, (void *)&m_req, sizeof(Msg_Req)-sizeof(long), WRITE, 0);
    
        
        InizioScrittura(magazzino, m_req.id_prodotto);
        
        magazzino->scorta[m_req.id_prodotto] -= m_req.value;

        FineScrittura(magazzino, m_req.id_prodotto);
        printf("[scrittore] Ricevuta richiesta di decremento di %d per prodotto %d\n",m_req.value, m_req.id_prodotto);
    }
}

void InizioLettura(Magazzino *magazzino, int id_prodotto){
    Wait_Sem(magazzino->mutexl_sem, id_prodotto);
    magazzino->num_lettori[id_prodotto]++;
    if(magazzino->num_lettori[id_prodotto] == 1) Wait_Sem(magazzino->synch_sem, id_prodotto);
    Signal_Sem(magazzino->mutexl_sem, id_prodotto);
}

void FineLettura(Magazzino *magazzino, int id_prodotto){
    Wait_Sem(magazzino->mutexl_sem, id_prodotto);
    magazzino->num_lettori[id_prodotto]--;
    if(magazzino->num_lettori[id_prodotto] == 0) Signal_Sem(magazzino->synch_sem, id_prodotto);
    Signal_Sem(magazzino->mutexl_sem, id_prodotto);
}

void InizioScrittura(Magazzino *magazzino, int id_prodotto){
    Wait_Sem(magazzino->synch_sem, id_prodotto);
}
void FineScrittura(Magazzino *magazzino, int id_prodotto){
    Signal_Sem(magazzino->synch_sem, id_prodotto);
}