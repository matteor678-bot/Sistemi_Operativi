#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "header.h"

#define NUM_WORKER 3

static int coda;
static pthread_t threads[NUM_WORKER+1];


// struttura per il passaggio dei parametri ai thread worker
typedef struct {
	int id;
	Buffer* b;
} parametri;

void init_buffer(Buffer* b){
    b->somma = 0;
    b->num_som;
    pthread_mutex_init(&(b->mutex), NULL);
    pthread_cond_init(&(b->collect), NULL);
	printf("[SERVER] - Buffer inizializzato...\n");
}

void remove_buffer(Buffer* b){
    pthread_mutex_destroy(&(b->mutex));
    pthread_cond_destroy(&(b->collect));
	printf("[SERVER] - Buffer distrutto...\n");
}


void aggiorna_somma(Buffer* b, int somma_parziale){

    pthread_mutex_lock(&(b->mutex));

    b->somma+=somma_parziale;
    b->num_som++;

    if(b->num_som == NUM_WORKER*NUM_REQ) 
        pthread_cond_signal(&(b->collect));
    
    pthread_mutex_unlock(&(b->mutex));

}

int preleva_somma_finale(Buffer* b){
	int somma_finale;
	//TODO prelevare la somma finale dal buffer, 
	// 		solo quando sono state accumulati NUM_WORKER*NUM_REQ incrementi
    pthread_mutex_lock(&(b->mutex));

    if(b->num_som != NUM_WORKER*NUM_REQ)
        pthread_cond_wait(&(b->collect), &(b->mutex));

    somma_finale = b->somma;

    pthread_mutex_unlock(&(b->mutex));

	return somma_finale;
}

void* collector(void* arg){
	int somma_finale;
	float media;

    Buffer * b = (Buffer *)arg;

	printf("[COLLECTOR] - start del thread...\n");

    somma_finale = preleva_somma_finale(b);
	
	media = somma_finale/(NUM_REQ*NUM_WORKER);
	
	printf("[COLLECTOR] - somma finale: %d media: %f\n",somma_finale,media);

    pthread_exit(0);
	
}

void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

    parametri * p = (parametri *) arg;
	
	Misura m;
	
    for(int i=0;i<NUM_REQ; i++) {
        msgrcv(coda, (void *)&m, sizeof(Misura)-sizeof(long), p->id, 0);
        aggiorna_somma(p->b, m.valore);
    }
	
    pthread_exit(0);

}



int main(){

	int i,status;
	pid_t pid;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	key_t key;
	key = ftok("~", 'a');
    
	coda = msgget(key, 0);

	printf("[SERVER] - id coda %d\n",coda);


	//Inizializziamo strutture
	
	Buffer* b = (Buffer *)malloc(sizeof(Buffer));

	init_buffer(b);
	
	parametri p[NUM_WORKER];

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER+1;i++){

		printf("[MASTER] - creo thread %d...\n",i);

        if(i==0) {
            pthread_create(&(threads[i]), &attr, collector, (void *) b);
        }
        else {
            p[i].id = i;
            p[i].b = b;
            pthread_create(&(threads[i]), &attr, worker, (void *)&p[i]);

        }
		
	}

	printf("[MASTER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);

    return 0;
}
