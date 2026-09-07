#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "header.h"


int main(int argc, char *argv[]){
	int coda;
	pid_t pid;
	long id_sensore;
        
	if (argc < 2 ) {
		printf("ERRORE: il client deve ricevere il proprio ID\n");
		return -1;
	}
        
	id_sensore = atol(argv[1]); 
       
	key_t key;
	key = ftok(FTOK_QUEUE_PATH,FTOK_QUEUE_TOK);
    
	coda = msgget(key,0);

    printf("[CLIENT] CODA %d\n", coda);
    
	printf("[DEBUG] - sensore id coda %d\n",coda);	
	printf("[Client %d] - invio richieste...\n",getpid());


	int i = 0;
	Misura misura;	

	srand(time(NULL));

	while(i < NUM_REQ){

		misura.sensore = id_sensore;
		misura.valore = rand()%100;

		// Invio di una misura
		printf("[Client %d] - Invio richiesta {%ld,%d} al server...\n",getpid(),misura.sensore,misura.valore);
		msgsnd(coda,&misura,sizeof(Misura)-sizeof(long),0);

		sleep(1);

		i++;
	}


	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}