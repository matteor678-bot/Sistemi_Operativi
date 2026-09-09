#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void init_monitor(MonitorIO * m) {

    for(int i=0; i<DIM; i++) {
        m->stato[i] = LIBERO;
    }
    m->num_occupati = 0;
    m->num_liberi = DIM;
    pthread_mutex_init(&(m->mutexm), NULL);
    pthread_cond_init(&(m->spaziodisp), NULL);
    pthread_cond_init(&(m->msg_disp), NULL);
    pthread_cond_init(&(m->cv_attesa), NULL);
}

void end_monitor(MonitorIO * m) {

    pthread_mutex_destroy(&(m->mutexm));
    pthread_cond_destroy(&(m->spaziodisp));
    pthread_cond_destroy(&(m->msg_disp));
    pthread_cond_destroy(&(m->cv_attesa));
}

void produzione(MonitorIO * m, char c) {

    int i=0;

    pthread_mutex_lock(&(m->mutexm));
    while(m->num_liberi==0)
        pthread_cond_wait(&(m->spaziodisp), &(m->mutexm));
        
    while(i<DIM && m->stato[i]!=LIBERO)
        i++;

    m->stato[i] = IN_USO;
    m->num_liberi--;

    pthread_mutex_unlock(&(m->mutexm));

    printf("PRODUZIONE: inizio produzione vettore[%d]\n", i);
    sleep(1 + (rand() % 3));
    m->vettore[i] = c;
    m->stato[i] = OCCUPATO;
    pthread_mutex_lock(&(m->mutexm));
    m->num_occupati++;
    printf("PRODUZIONE: vettore[%d] <- '%c'\n", i, c);

    pthread_cond_signal(&(m->msg_disp));
    pthread_cond_broadcast(&(m->cv_attesa));
    pthread_mutex_unlock(&(m->mutexm));
}

char consumazione(MonitorIO * m) {

    int i=0;

    pthread_mutex_lock(&(m->mutexm));

    while(m->num_occupati==0)
        pthread_cond_wait(&(m->msg_disp), &(m->mutexm));

    while(i<DIM && m->stato[i]!=OCCUPATO) {
        i++;
    }

    m->stato[i] = IN_USO;
    m->num_occupati--;

    pthread_mutex_unlock(&(m->mutexm));

    printf("CONSUMAZIONE: inizio consumazione vettore[%d]\n", i);
    sleep(1);
    char c = m->vettore[i];
    m->stato[i] = LIBERO;
    pthread_mutex_lock(&(m->mutexm));
    m->num_liberi++;
    printf("CONSUMAZIONE: vettore[%d] -> '%c'\n", i, c);

    pthread_cond_signal(&(m->spaziodisp));
    pthread_mutex_unlock(&(m->mutexm));
    return c;
}

int attesa(MonitorIO * m) {

    int occupati;

    pthread_mutex_lock(&(m->mutexm));

    while(m->num_occupati<3)
        pthread_cond_wait(&(m->cv_attesa), &(m->mutexm));

    occupati = m->num_occupati;

    pthread_mutex_unlock(&(m->mutexm));

    return occupati;
}