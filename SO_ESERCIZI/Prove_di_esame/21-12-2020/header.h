#ifndef __HEADER
#define __HEADER

//Funzioni che ci servono
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define NUM_REQ 10

typedef struct{
	long type;
    int valore;
}Misura;

typedef struct{
	int somma;
	int num_som;
    pthread_mutex_t mutex;
    pthread_cond_t collect;
}Buffer;

#endif