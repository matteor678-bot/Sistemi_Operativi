#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "header.h"

void * produttore(void *);
void * consumatore(void *);

int main() {

    pthread_t thread_consumatore;
    pthread_t thread_produttori[3];

    MonitorIO * m =(MonitorIO *)malloc(sizeof(MonitorIO));
    init_monitor(m);

    for(int i=0; i<3;i++) {
        pthread_create(&(thread_produttori[i]), NULL, produttore, (void *)m);
    }

    pthread_create(&(thread_consumatore), NULL, consumatore, (void *)m);

    for(int i=0; i<3; i++) {
        pthread_join(thread_produttori[i], NULL);
    }

    pthread_join(thread_consumatore, NULL);

    end_monitor(m);

    free(m);

    return 0;
}


void * produttore(void * p) {

    printf("Avvio thread produttore...\n");
    MonitorIO * m =(MonitorIO *)p;

    for(int i=0; i<5; i++) {

        sleep(1);

        // genera carattere casuale tra "a" e "z"
        char carattere = 97 + (rand() % 26);

        produzione(m, carattere);
    }

    return NULL;
}

void * consumatore(void * p) {

    printf("Avvio thread consumatore...\n");
    MonitorIO * m =(MonitorIO *)p;

    int totale_consumazioni = 0;

    while(totale_consumazioni < 15) {

        int occupati = 1;
        
        if(totale_consumazioni < 13) {

            printf("CONSUMATORE: In attesa di elementi disponibili...\n");

            occupati = attesa(m);

            printf("CONSUMATORE: %d elementi disponibili\n", occupati);
        }

        for(int j=0; j<occupati; j++) {

            char carattere;
            
            carattere = consumazione(m);
        }
        totale_consumazioni += occupati;
    }

    return NULL;
}