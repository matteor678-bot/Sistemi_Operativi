#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

typedef struct {
    int coda_sensori;
    int coda_planner;
    coppia_buffer * b;
} parametri_thread;

void * thread_produttore(void * arg) {

    parametri_thread * p = (parametri_thread *) arg;
    for(int i = 0; i < 10; i++) {

        messaggio_sensore m;
        int err = msgrcv(p->coda_sensori, (void *)&m, sizeof(messaggio_sensore)-sizeof(long), X, 0);
        if (err<0)
            perror("Errore msgrcv()");
        int tipo = m.type;
        int valore_x = m.val;

        printf("[Planner] Ricevuto %d di tipo X\n", valore_x);

        produci(p->b, tipo, valore_x);


        err = msgrcv(p->coda_sensori, (void *)&m, sizeof(messaggio_sensore)-sizeof(long), Y, 0);
        tipo = m.type;
        int valore_y = m.val;

        printf("[Planner] Ricevuto %d di tipo Y\n", valore_y);

        produci(p->b, tipo, valore_y);

    }

    return NULL;
}

void * thread_consumatore(void * arg) {
    parametri_thread * p = (parametri_thread *)arg;
    messaggio_attuatore m;
    for(int i = 0; i < 10; i++) {

        int valore_x;
        int valore_y;

        consuma(p->b, &valore_x, &valore_y);

        int valore_attuatore = valore_x + valore_y;

        printf("[Planner] Invio valore %d all'attuatore\n", valore_attuatore);

        m.type = MESSAGGIO;
        m.val = valore_attuatore;
        int err = msgsnd(p->coda_planner, (void *)&m, sizeof(messaggio_attuatore)-sizeof(long), 0);

    }

    return NULL;
}

void init_coppia_buffer(coppia_buffer * b) {
    b->buffer_x = 0;
    b->buffer_y = 0;
    pthread_mutex_init(&(b->buffervuoto_x), NULL);
    pthread_mutex_init(&(b->buffervuoto_y), NULL);
    pthread_mutex_init(&(b->bufferpieno_x), NULL);
    pthread_mutex_lock(&(b->bufferpieno_x));
    pthread_mutex_init(&(b->bufferpieno_y), NULL);
    pthread_mutex_lock(&(b->bufferpieno_y));
}

void remove_coppia_buffer(coppia_buffer * b) {
    b->buffer_x = 0;
    b->buffer_y = 0;
    pthread_mutex_destroy(&(b->buffervuoto_x));
    pthread_mutex_destroy(&(b->buffervuoto_y));
    pthread_mutex_destroy(&(b->bufferpieno_x));
    pthread_mutex_destroy(&(b->bufferpieno_y));
}

void produci(coppia_buffer * buffer, int tipo, int valore) {

    /* TBD: Completare la sincronizzazione.

       Nota: Se il valore è di tipo X, il produttore deve attendere che buffer_x diventi libero.
             Se il valore è di tipo Y, il produttore deve attendere che buffer_y diventi libero.
     */
    


    if(tipo == X) {
        pthread_mutex_lock(&(buffer->buffervuoto_x));
        printf("[Planner] Produzione valore %d di tipo X\n", valore);

        buffer->buffer_x = valore;
        
        pthread_mutex_unlock(&(buffer->bufferpieno_x));

    } else if(tipo == Y) {
        pthread_mutex_lock(&(buffer->buffervuoto_y));
        printf("[Planner] Produzione valore %d di tipo Y\n", valore);

        buffer->buffer_y = valore;

        pthread_mutex_unlock(&(buffer->bufferpieno_y));

    }

}

void consuma(coppia_buffer * buffer, int * valore_x, int * valore_y) {
    
    pthread_mutex_lock(&(buffer->bufferpieno_x));
    pthread_mutex_lock(&(buffer->bufferpieno_y));

    *valore_x = buffer->buffer_x;
    *valore_y = buffer->buffer_y;

    printf("[Planner] Consumazione valori %d (X) e %d (Y)\n", *valore_x, *valore_y);

    pthread_mutex_unlock(&(buffer->buffervuoto_x));
    pthread_mutex_unlock(&(buffer->buffervuoto_y));

}

void planner(int coda_sensori, int coda_attuatore) {

    printf("[Planner] Avviato...\n");

    coppia_buffer * b =(coppia_buffer *)malloc(sizeof(coppia_buffer));
    init_coppia_buffer(b);

    // Allocazione dinamica paramteri dei threads:
    pthread_t threads[2];
    parametri_thread * p =(parametri_thread *)malloc(sizeof(parametri_thread));
    p->coda_sensori = coda_sensori;
    p->coda_planner = coda_attuatore;
    p->b = b;



    /* TBD: Avviare i due thread, facendogli eseguire le funzioni
            "thread_produttore" e "thread_consumatore", e passandogli i parametri necessari.
     */
    pthread_create(&(threads[0]), NULL, thread_produttore, (void *)p);
    pthread_create(&(threads[1]), NULL, thread_consumatore, (void *)p);

     /* TBD: Attendere la terminazione dei thread e de-allocare le risorse */
    for( int i=0; i<2; i++) {
        pthread_join(threads[i], NULL);
    }

    remove_coppia_buffer(b);
    free(b);
    free(p);
    
}