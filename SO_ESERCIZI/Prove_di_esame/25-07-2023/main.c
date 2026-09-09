#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "procedure.h"

#define QUANTITA_INIZIALE 500
#define VALORE_INIZIALE 100

void * aggiornatore(void *);
void * azionista(void *);

int main() {

    MonitorPortafoglio * m[3];
    pthread_t Threads[4];

    for(int i = 0; i<3; i++) {
        m[i] =(MonitorPortafoglio *)malloc(sizeof(MonitorPortafoglio));
        inizializza(m[i], 500, 100);
    }


    /* TBD: Creare il thread aggiornatore, passargli il vettore di oggetti monitor */
    pthread_create(&(Threads[0]), NULL, aggiornatore, (void *)m);


    for(int i = 0; i<3; i++) {
            pthread_create(&(Threads[i+1]), NULL, azionista, (void *)m[i]);
    }


    /* TBD: Attendere la terminazione del thread aggiornatore */
    pthread_join(Threads[0], NULL);

    for(int i = 0; i<3; i++) {
        pthread_join(Threads[i+1], NULL);
    }

    for(int i=0; i<3; i++) {
        free(m[i]);
    }

    return 0;
}



void * aggiornatore(void * arg) {

    MonitorPortafoglio ** m = (MonitorPortafoglio **)arg;

    for(int i=0; i<10; i++) {

        for(int j=0; j<3; j++) {

            int valore = 100 + ((rand()%21) - 10);

            printf("[AGGIORNATORE] Aggiornamento titolo %d, valore %d\n", j, valore);

            aggiorna(m[j], valore);
        }

        sleep(1);
    }

    printf("[AGGIORNATORE] Terminazione\n");

    for(int j=0; j<3; j++) {

        aggiorna(m[j], 200);
        sleep(1);

        aggiorna(m[j], 0);
    }

    return NULL;
}


void * azionista(void * arg) {

    MonitorPortafoglio *m =(MonitorPortafoglio *)arg;

    printf("[AZIONISTA] Richiesta di vendita azioni, prezzo offerto 105\n");

    vendita(m, 50, 105);

    int valore = leggi(m);

    printf("[AZIONISTA] Vendita effettuata, valore attuale %d\n", valore);


    printf("[AZIONISTA] Richiesta di acquisto azioni, prezzo offerto 95\n");

    acquisto(m, 50, 95);

    valore = leggi(m);

    printf("[AZIONISTA] Acquisto effettuato, valore attuale %d\n", valore);


    printf("[AZIONISTA] Terminazione\n");

    return NULL;
}