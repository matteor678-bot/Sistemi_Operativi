#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "semafori.h"
 
void InizioLettura(int sem, Buffer * buf) {

    Wait_Sem(sem, MUTEX_NUMLETTORI); //Incremento numlettori in mutua esclusione

    buf->numlettori = buf->numlettori + 1;

    if(buf->numlettori == 1) //Se si tratta del primo lettore blocca gli scrittori
        Wait_Sem(sem, MUTEX_LETTORI_SCRITTORI);

    Signal_Sem(sem, MUTEX_NUMLETTORI); //Rilascia il mutex per fare entrare altri lettori

}

void FineLettura(int sem, Buffer * buf) {

    Wait_Sem(sem, MUTEX_NUMLETTORI); //Decremento numlettori in mutua esclusione

    buf->numlettori = buf->numlettori - 1;

    if(buf->numlettori == 0) //Se sono l'ultimo lettore devo rilasciare al risorsa per gli scrittori
        Signal_Sem(sem, MUTEX_LETTORI_SCRITTORI);

    Signal_Sem(sem, MUTEX_NUMLETTORI); //Rilascio il mutex per fare entrare altri lettori 

}

void InizioScrittura(int sem, Buffer * buf) {

    Wait_Sem(sem, MUTEX_NUMSCRITTORI); //Incremento numscrittori in mutua esclusione

    buf->numscrittori = buf->numscrittori +1;

    if (buf->numscrittori == 1) //Il primo scrittore blocca i lettori
        Wait_Sem(sem, MUTEX_LETTORI_SCRITTORI);
    
    Signal_Sem(sem, MUTEX_NUMSCRITTORI); //Rilascia il MUTEX per far entrare altri scrittori

    Wait_Sem(sem, MUTEX_SCRITTORI); //Blocco eventuali scrittori per la scrittura vera e propria

}

void FineScrittura(int sem, Buffer* buf){

    Signal_Sem(sem,MUTEX_SCRITTORI); //Rilascio il mutex per gli scrittori che devono scrivere
        
	Wait_Sem(sem, MUTEX_NUMSCRITTORI); //Indica agli scrittori che sto terminando la scrittura, decremento
                                	// numscrittori in mutua esclusione

    buf->numscrittori = buf->numscrittori - 1;

    if (buf->numscrittori == 0) //se sono l'ultimo scrittore devo rilasciare la risorsa per i lettori
     	Signal_Sem(sem, MUTEX_LETTORI_SCRITTORI);

    Signal_Sem(sem, MUTEX_NUMSCRITTORI); //rilascio il mutex per altri scrittori che vogliono iniziare la scrittura

}

void Scrittore(int sem, Buffer *buf){

	InizioScrittura(sem,buf);

    struct timeval t1;
 	gettimeofday(&t1,NULL);    //valore diverso ad ogni scrittura
    	
    buf->messaggio =t1.tv_usec;
	
    sleep(1);
    	
    printf("Valore scritto: <%ld> \n", buf->messaggio);

    FineScrittura(sem,buf);
}

void Lettore (int sem, Buffer* buf) {

	InizioLettura(sem,buf);

	/*********Lettura********/
	sleep(1); // per simulare un ritardo di lettura
    printf("Valore letto=<%ld>, numero lettori=%d \n", buf->messaggio, buf->numlettori);
	
    FineLettura(sem,buf);

}

