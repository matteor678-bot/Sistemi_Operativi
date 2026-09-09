#ifndef _PROCEDURE_
#define _PROCEDURE_

#include <pthread.h>

typedef struct {
    int valore;
    int quantita;
    pthread_cond_t varcond_acquisto;
    pthread_cond_t varcond_vendita;
    pthread_mutex_t mutex_port;
} MonitorPortafoglio;

void inizializza(MonitorPortafoglio * m, int quantita_iniziale, int valore_iniziale);
void termina(MonitorPortafoglio * m);

void acquisto(MonitorPortafoglio * m, int quantita, int prezzo);
void vendita(MonitorPortafoglio * m, int quantita, int prezzo); 
void aggiorna(MonitorPortafoglio * m, int valore);
int leggi(MonitorPortafoglio * m);

#endif