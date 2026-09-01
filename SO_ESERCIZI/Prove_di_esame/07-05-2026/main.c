#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "monitor_aria.h"

void inizializza_monitor(MonitorAria *m) {
    
    m->buffer = 0;
    pthread_mutex_init(&(m->mutex), NULL);
    m->num_lettori = 0;
    m->num_scrittorilenti = 0;
    m->num_scrittoriveloci = 0;
    for(int i=0; i<3; i++) {
        m->cond_counts[i] = 0;
    }
    pthread_cond_init(&(m->condvar_lettori), NULL);
    pthread_cond_init(&(m->condvar_scrittoriveloci), NULL);
    pthread_cond_init(&(m->condvar_scrittorilenti), NULL);

}

void distruggi_monitor(MonitorAria *m) {

    pthread_mutex_destroy(&(m->mutex));
    pthread_cond_destroy(&(m->condvar_lettori));
    pthread_cond_destroy(&(m->condvar_scrittoriveloci));
    pthread_cond_destroy(&(m->condvar_scrittorilenti));

}

int main(void) {

    MonitorAria *monitor = (MonitorAria*)malloc(sizeof(MonitorAria));

    pthread_t threads[7];

    inizializza_monitor(monitor);

    for (int i = 0; i < NUM_LETTORI; i++) {

        thread_args *arg =(thread_args*)malloc(sizeof(thread_args));
        arg->monitor = monitor;
        arg->id = i;
         
        pthread_create(&threads[i],NULL, thread_lettore, (void*)arg);
        
    }

    for (int i = 0; i < NUM_SCRITTORI_LENTI; i++) {
    
        thread_args *arg =(thread_args*)malloc(sizeof(thread_args));
        arg->monitor = monitor;
        arg->id = i+3;
        
        pthread_create(&threads[i+3], NULL, thread_scrittore_lento, (void *)arg);
    
    }

    for (int i = 0; i < NUM_SCRITTORI_VELOCI; i++) {
        
        thread_args *arg =(thread_args*)malloc(sizeof(thread_args));
        arg->monitor = monitor;
        arg->id = i+5;

        pthread_create(&threads[i+5], NULL, thread_scrittore_veloce, (void *)arg);
        
    }

    //TODO: attendere la terminazione di tutti i thread
    for(int i=0; i<7; i++) {
        if(pthread_join(threads[i], NULL)==0) {
            printf("Threads[%d] terminato con successo!\n", i);
        }
        else 
            printf("Errore nella join()\n");
    }

    distruggi_monitor(monitor);

    free(monitor);
    printf("Terminazione\n");

    return 0;
}