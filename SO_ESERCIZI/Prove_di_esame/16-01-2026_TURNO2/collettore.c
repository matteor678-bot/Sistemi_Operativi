#include "sensore.h"
#include "collettore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void collettore(int id_coda_collettore) {

    printf("[COLLETTORE] Avvio processo (id_coda_collettore = %d)...\n", id_coda_collettore);

    for(int i=0; i<10; i++) {

        messaggio msg;
        int ret;

        printf("Collettore: In attesa di messaggi...\n");

        ret = msgrcv(id_coda_collettore, &msg, sizeof(messaggio)-sizeof(long), TIPO, 0);

        if(ret < 0) {
            perror("Errore msgrcv");
            exit(1);
        }

        printf("Collettore: Ricevuto valore=%d\n", msg.val);

    }
}