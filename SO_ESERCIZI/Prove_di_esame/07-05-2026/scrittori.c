#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "monitor_aria.h"

void scrittore_veloce(MonitorAria *m, int valore) {

    //Entro nel monitor:
    pthread_mutex_lock(&(m->mutex));

    printf("Scrittura - ingresso monitor\n");

    while((m->num_lettori>0) || (m->num_scrittorilenti>0) || (m->num_scrittoriveloci>0)) {
        printf("Scrittura - sospensione\n");
        m->cond_counts[1]+=1;
        pthread_cond_wait(&(m->condvar_scrittoriveloci), &(m->mutex));
        m->cond_counts[1]-=1;
    }

    m->num_scrittoriveloci+=1;

    printf("Numero scrittori veloci incrementato: %d\n", m->num_scrittoriveloci);

    pthread_mutex_unlock(&(m->mutex));

    //Esco dal monitor: Posso iniziare le operazioni di scrittura del buffer!
    sleep(1);
    m->buffer = valore;

    printf("Scrittura veloce - valore [%d]\n", valore);

    //Fine operazione di scrittura -> rientro nel monitor per modificare variabili condivise ed inviare signal ai thread sospesi
    pthread_mutex_lock(&(m->mutex));

    m->num_scrittoriveloci-=1;

    printf("Numero scrittori decrementato: %d\n", m->num_scrittoriveloci);

    if(m->cond_counts[1]>0) {

        printf("Scrittura - signal() su scrittori veloci\n");
        pthread_cond_signal(&(m->condvar_scrittoriveloci));
    }
    else {
        if(m->cond_counts[2]>0) {
            printf("Scrittura - signal() su scrittori lenti\n");
            pthread_cond_signal(&(m->condvar_scrittorilenti));
        }
        else if(m->cond_counts[0]>0) {
            printf("Scrittura - signal() su lettori\n");
            pthread_cond_broadcast(&(m->condvar_lettori));
        }
    }

    pthread_mutex_unlock(&(m->mutex));

    printf("Scrittura veloce - uscita dal monitor\n");

}

void scrittore_lento(MonitorAria *m, int valore) {

    //Entro nel monitor:
    pthread_mutex_lock(&(m->mutex));

    printf("Scrittura - ingresso monitor\n");

    while((m->num_lettori>0) || (m->num_scrittorilenti>0) || (m->num_scrittoriveloci>0) || (m->cond_counts[1]>0)) {
        printf("Scrittura lenta - sospensione\n");
        m->cond_counts[2]+=1;
        pthread_cond_wait(&(m->condvar_scrittorilenti), &(m->mutex));
        m->cond_counts[2]-=1;
    }

    m->num_scrittorilenti+=1;

    printf("Numero scrittori lenti incrementato: %d\n", m->num_scrittorilenti);

    pthread_mutex_unlock(&(m->mutex));

    //Esco dal monitor: Posso iniziare le operazioni di scrittura del buffer!
    sleep(2);
    m->buffer = valore;

    printf("Scrittura lenta - valore [%d]\n", valore);

    //Fine operazione di scrittura -> rientro nel monitor per modificare variabili condivise ed inviare signal ai thread sospesi
    pthread_mutex_lock(&(m->mutex));

    m->num_scrittorilenti-=1;

    printf("Numero scrittori decrementato: %d\n", m->num_scrittorilenti);

    if(m->cond_counts[1]>0) {
        printf("Scrittura - signal() su scrittori veloci\n");
        pthread_cond_signal(&(m->condvar_scrittoriveloci));
    }
    else {
        if(m->cond_counts[2]>0) {
            printf("Scrittura - signal() su scrittori lenti\n");
            pthread_cond_signal(&(m->condvar_scrittorilenti));
        }
        else if(m->cond_counts[0]>0) {
            printf("Scrittura - signal() su lettori\n");
            pthread_cond_broadcast(&(m->condvar_lettori));
        }
    }

    pthread_mutex_unlock(&(m->mutex));

    printf("Scrittura lenta - uscita dal monitor\n");

}

void *thread_scrittore_veloce(void *arg) {

    thread_args *args = (thread_args *) arg;

    MonitorAria *m = args->monitor;

    int id = args->id;

    srand((unsigned long)pthread_self());

    for(int i =0; i<ITERAZIONI; i++) {
        int valore = rand() % 11;

        printf("[Sensore veloce %d] richiesta scrittura %d: valore = %d\n", id, i+1, valore);

        scrittore_veloce(m, valore);

        printf("[Sensore veloce %d] completata scrittura %d: valore = %d\n", id, i+1, valore);

        sleep(3);
    }

    free(args);
    pthread_exit(NULL);

}

void *thread_scrittore_lento(void *arg) {

    thread_args *args = (thread_args *)arg;

    MonitorAria *m = args->monitor;
    int id = args->id;

    srand((unsigned long)pthread_self());

    for (int i = 0; i < ITERAZIONI; i++) {
        int valore = rand() % 11;

        printf("[Stazione lenta %d] richiesta scrittura %d: valore = %d\n", id, i + 1, valore);

        scrittore_lento(m, valore);

        printf("[Stazione lenta %d] completata scrittura %d: valore = %d\n", id, i + 1, valore);

        sleep(3);
    }

    free(args);
    pthread_exit(NULL);
}