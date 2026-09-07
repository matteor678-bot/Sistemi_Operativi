#ifndef __HEADER
#define __HEADER

//Funzioni che ci servono
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>    
#include <pthread.h>

#define FTOK_QUEUE_PATH "."
#define FTOK_QUEUE_TOK 'A'

#define NUM_REQ 10

// messaggio misura dai sensori
typedef struct{
	long sensore;
	int valore;
}Misura;

// somma condivisa lato server
typedef struct{
	pthread_cond_t ok_cons;
	pthread_mutex_t mutex;
	int num_somme;
	int somma;
}Buffer;

#endif