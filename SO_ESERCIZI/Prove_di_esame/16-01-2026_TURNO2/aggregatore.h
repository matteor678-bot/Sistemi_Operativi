#ifndef _AGGREGATORE_H_
#define _AGGREGATORE_H_

#include <pthread.h>

typedef struct {

    int variabile;
    pthread_mutex_t mutex;
    pthread_cond_t varcond_lettore;
    pthread_cond_t varcond_scrittore;

    int cond_counts[2]; //Array delle variabili condition_count
    int num_lettori;
    int num_scrittori;

} MonitorLS;


typedef struct {

    MonitorLS * m;
    int coda;

} Parametri;


void aggregatore(int id_coda_sensore, int id_code_collettori);
void * thread_lettore(void *);
void * thread_scrittore(void *);
void lettura(MonitorLS *, int * valore);
void scrittura(MonitorLS *, int valore);

#endif