#include "sensore.h"
#include "aggregatore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

void aggregatore(int id_coda_sensore, int id_coda_collettori) {

    printf("[AGGREGATORE] Avvio processo...\n");

    pthread_t threads[4];
    
    // TODO: allocazione della struct MonitorLS e inizializzazione delle var. di sincronizzazione MonitorLS //...
    MonitorLS *mLS = (MonitorLS *)malloc(sizeof(MonitorLS));
    pthread_mutex_init(&(mLS->mutex), NULL);
    pthread_cond_init(&(mLS->varcond_lettore), NULL);
    pthread_cond_init(&(mLS->varcond_scrittore), NULL);
    mLS->variabile = 0;
    mLS->num_lettori = 0;
    mLS->num_scrittori = 0;

    for(int i=0;i<2; i++) {
        mLS->cond_counts[i] = 0;
    }

    Parametri * p_scrittore = (Parametri *)malloc(sizeof(Parametri));
    p_scrittore ->coda = id_coda_sensore;
    p_scrittore->m = mLS;

    int ret = pthread_create(&threads[0], NULL, thread_scrittore, (void *)p_scrittore);

    if(ret < 0) {
        printf("Errore pthread_create");
        exit(1);
    }

    Parametri * p_lettori[3];

    for(int i=0; i<3; i++) {

        // TODO: allocazione della struct Parametri da utilizzare nei thread lettori e inizializzazione dei parametri        
        p_lettori[i] =(Parametri *)malloc(sizeof(Parametri));
        p_lettori[i]->m = mLS;
        p_lettori[i]->coda = id_coda_collettori;
        ret = pthread_create(&threads[i+1], NULL, thread_lettore, (void *)p_lettori[i]);

        if(ret < 0) {
          printf("Errore pthread_create");
            exit(1);
        }
    }

    pthread_join(threads[0], NULL);

    for(int i=0; i<3; i++) {

        pthread_join(threads[i+1], NULL);
    }

    free(p_scrittore);

    for(int i=0; i<3; i++) {
        free(p_lettori[i]);
    }

    free(mLS);
}

void * thread_lettore(void * x) {

    Parametri * p = (Parametri *)x;

    for(int i=0; i<10; i++) {

        messaggio msg;
        int valore;
        int ret;

        sleep(1);

        lettura(p->m, &valore);

        msg.type = TIPO;
        msg.val = valore;
        
        printf("Aggregatore: Invio valore=%d\n", valore);

        ret = msgsnd(p->coda, &msg, sizeof(messaggio)-sizeof(long), 0);

        if(ret < 0) {
            perror("Errore msgsnd");
            exit(1);
        }
    }


    pthread_exit(NULL);
}

void * thread_scrittore(void * x) {

    Parametri * p = (Parametri *)x;

    for(int i=0; i<10; i++) {

        messaggio msg;
        int ret;

        printf("Aggregatore: In attesa di messaggi...\n");

        ret = msgrcv(p->coda, &msg, sizeof(messaggio)-sizeof(long), 1, 0);

        if(ret < 0) {
            printf("Errore msgrcv");
            exit(1);
        }

        printf("Aggregatore: Ricevuto valore=%d\n", msg.val);

        scrittura(p->m, msg.val);
    }

    pthread_exit(NULL);
}

void lettura(MonitorLS * m, int * valore) {

    pthread_mutex_lock(&(m->mutex));

    printf("Ingresso nel monitor - lettura\n");

    while(m->num_scrittori == 1) {
        printf("Sospensione - lettura\n");
        m->cond_counts[0]++;
        pthread_cond_wait(&(m->varcond_lettore), &(m->mutex));
        printf("Riattivazione - lettura\n");
        m->cond_counts[0]--;
    }

    m->num_lettori++;

    printf("Numero di lettori incrementato: %d\n", m->num_lettori);

    pthread_mutex_unlock(&(m->mutex));

    *valore = m->variabile;

    printf("[AGGREGATORE] Lettura valore=%d\n", *valore);

    pthread_mutex_lock(&(m->mutex));

    m->num_lettori--;

    printf("Numero lettori decrementato: %d\n", m->num_lettori);

    if(m->num_lettori == 0 && m->cond_counts[1]==1) {
        printf("Lettura - signal() su scrittore\n");
        pthread_cond_signal(&(m->varcond_scrittore));
    }

    pthread_mutex_unlock(&(m->mutex));

    printf("Lettura - uscita monitor\n");
}

void scrittura(MonitorLS * m, int valore) {

    pthread_mutex_lock(&(m->mutex));

    printf("Ingresso nel monitor - scrittore\n");

    while(m->num_lettori>0) {
        printf("Sospensione - scrittura\n");
        m->cond_counts[1]++;
        pthread_cond_wait(&(m->varcond_scrittore), &(m->mutex));
        printf("Riattivazione - scrittura\n");
        m->cond_counts[1]--;
    }
    m->num_scrittori++;

    printf("[AGGREGATORE] Scrittura valore=%d\n", valore);
    m->variabile = valore;


    m->num_scrittori--;

    if(m->cond_counts[0]>0) {
        printf("Scrittura - signal() su lettura\n");
        pthread_cond_signal(&(m->varcond_lettore));
    }

    pthread_mutex_unlock(&(m->mutex));

    printf("Scrittura - uscita monitor\n");
}