#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

#define MAX_DECR 4

int main(){
    
    //Creazione code richieste e risposte
    int ds_queue_request, ds_queue_response;

    //create queues
    int key_queue_request = ftok(".", 'a');
    int key_queue_response = ftok(".", 'b');
    
    ds_queue_request = msgget(key_queue_request, 0);
    ds_queue_response = msgget(key_queue_response, 0);
    
    printf("[updater] Coda ds_queue_request ID: %d\n", ds_queue_request);
    printf("[updater] Coda ds_queue_response ID: %d\n", ds_queue_response);
    
    
    Msg_Req m_req;

   
    
    
    int i;

    srand(time(NULL)*getpid());
    
    for (i=0; i<2; i++){

        m_req.tipo_richiesta = WRITE;
        m_req.mittente = 0;
        m_req.id_prodotto = rand()%MAX_PRODOTTI;
        m_req.value = 1 + rand()%(MAX_DECR);
        // TODO aggiungere codice per l'invio della richiesta
        int err = msgsnd(ds_queue_request, (void *)&m_req, sizeof(Msg_Req)-sizeof(long), 0);
        if(err<0)
            perror("Errore msgsnd()\n");
        printf("[updater] Richiesto decremento di %d unità del prodotto %d\n",m_req.value,m_req.id_prodotto);
    }
    
    return 0;
}