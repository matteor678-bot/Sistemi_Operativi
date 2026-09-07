#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "header.h"

#define NUM_WORKER 3

static int coda;
static pthread_t threads[NUM_WORKER+1];

typedef struct {
	int id;
	Buffer* b;
} parametri;

void init_buffer(Buffer* b){
	b->somma = 0;
	b->num_somme = 0;
	
	pthread_cond_init(&(b->ok_cons),NULL);
	pthread_mutex_init(&(b->mutex),NULL);

	printf("[SERVER] - Buffer inizializzato...\n");
}

void remove_buffer(Buffer* b){
	pthread_cond_destroy(&(b->ok_cons));
	pthread_mutex_destroy(&(b->mutex));

	printf("[SERVER] - Buffer distrutto...\n");
}


void aggiorna_somma(Buffer* b, int somma_parziale){
	pthread_mutex_lock(&(b->mutex));
	
	b->somma += somma_parziale;
	b->num_somme++;
	
	if (b->num_somme == (NUM_WORKER*NUM_REQ))
        pthread_cond_signal(&(b->ok_cons));

	printf("[DEBUG] - Aggiornamento buffer: somma %d, num_somme %d\n",b->somma,b->num_somme);

	pthread_mutex_unlock(&(b->mutex));
}

int preleva_somma_finale(Buffer* b){
	int somma_finale;
	
	pthread_mutex_lock(&(b->mutex));
	while(b->num_somme < (NUM_WORKER*NUM_REQ)){
		pthread_cond_wait(&(b->ok_cons),&(b->mutex));
	}

	somma_finale = b->somma;

	printf("[DEBUG] - Consumo buffer: somma finale %d, num_somme %d\n",b->somma,b->num_somme);
	
	pthread_mutex_unlock(&(b->mutex));

	return somma_finale;
}

void* collector(void* arg){
	int somma_finale;
	float media;

	Buffer* b = (Buffer*) arg;

	printf("[COLLECTOR] - start del thread...\n");

	somma_finale = preleva_somma_finale(b);
	
	media = (float) somma_finale/(NUM_WORKER*NUM_REQ);
	
	printf("[COLLECTOR] - somma finale: %d media: %f\n",somma_finale,media);
	
    pthread_exit(0);
}

void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	parametri* par = (parametri*) arg;
	Buffer* b = par->b;
	int id = par->id;

	Misura m;
	
	int i = 0;

	for (i = 0; i < NUM_REQ; i++) {
	
		printf("[WORKER] sto per ricevere misura dal sensore %d \n",id);
        printf("CODA %d\n", coda);
		msgrcv(coda,&m,sizeof(Misura)-sizeof(long),id,0);
		
	    printf("[WORKER] - Ho ricevuto la misura %d dal sensore %ld \n",m.valore,m.sensore);
	
		aggiorna_somma(b,m.valore);
		

  
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
	key = ftok(FTOK_QUEUE_PATH,FTOK_QUEUE_TOK);
    
	coda = msgget(key,0);

	printf("[SERVER] - id coda %d\n",coda);


	//Inizializziamo strutture
	
	Buffer* b = (Buffer *) malloc(sizeof(Buffer));

	init_buffer(b);
	
	parametri param[NUM_WORKER];

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER+1;i++){

		printf("[MASTER] - creo thread %d...\n",i);

		if(i == 0){
			pthread_create(&threads[i],&attr,collector,(void *) b);
		}else{
			param[i].id = i;
			param[i].b = b;
			pthread_create(&threads[i],&attr,worker,(void *) &param[i]);
		}
	}

	printf("[MASTER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}