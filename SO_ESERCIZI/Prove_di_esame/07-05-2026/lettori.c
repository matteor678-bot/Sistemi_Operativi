#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "monitor_aria.h"

int lettore(MonitorAria *m) {
    int valore;

    //Entro nel monitor:
    pthread_mutex_lock(&(m->mutex));

    printf("Lettura - ingresso monitor\n");

    while((m->num_scrittorilenti > 0) || (m->num_scrittoriveloci > 0)) {

        printf("Sospensione  - lettura\n");
        m->cond_counts[0]+=1;
        pthread_cond_wait(&(m->condvar_lettori), &(m->mutex));
        m->cond_counts[0]-=1;
    }

    m->num_lettori+=1;
    
    printf("Numero lettori incrementato: %d\n", m->num_lettori);

    pthread_mutex_unlock(&(m->mutex));

    //Esco dal monitor: Posso iniziare le operazioni di lettura del buffer!

    valore = m->buffer;

    printf("Lettura - valore [%d]\n", valore);


    // Operazioni di fine lettura:
    pthread_mutex_lock(&(m->mutex));

    m->num_lettori-=1;

    printf("Numero lettori decrementato: %d\n", m->num_lettori);

    if(m->num_lettori == 0) {
        if(m->cond_counts[1]>0) {
            printf("Lettura - signal() su scrittori veloci\n");

            pthread_cond_signal(&(m->condvar_scrittoriveloci));
        }

        else if(m->cond_counts[2]>0) {
            printf("Lettura - signal su scrittori lenti\n");

            pthread_cond_signal(&(m->condvar_scrittorilenti));
        }
    }

    pthread_mutex_unlock(&(m->mutex));

    printf("Lettura - uscita monitor\n");

    return valore;
}

void *thread_lettore(void *arg) {
    thread_args *args =(thread_args *)arg; //Gestione del passaggio degli argomenti

    MonitorAria *m = args->monitor; //Gestione del passaggio degli argomenti

    int id = args->id;

    for(int i=0; i<ITERAZIONI; i++) {
        int valore = lettore(m);

        printf("[Lettore %d] lettura %d: indice di qualità aria = %d\n", id, i+1, valore);

        sleep(3);
    }

    free(args);
    pthread_exit(NULL);
}