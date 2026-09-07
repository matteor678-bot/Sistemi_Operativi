#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){
	int i,status;
	int coda;
	pid_t pid;

	key_t key;
	key = ftok(FTOK_QUEUE_PATH,FTOK_QUEUE_TOK);
    
	coda = msgget(key,IPC_CREAT | 0644);

	
	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){
		
		sleep(1);

		pid = fork();

		if(pid == 0){

			if(i == NUM_CLIENT+NUM_SERVER -1){
				printf("[SERVER %d] - Sono stato creato...\n",getpid());
				execve("./server",NULL,NULL);
				exit(-1);
			}else{

				char id_client[] = "1";
				id_client[0] += i;

				printf("[CLIENT %d] - Sono stato creato con ID %s\n",getpid(),id_client);
				execl("./client","./client",id_client,NULL);
				exit(-1);
			}
		}
	}

	for(i = 0; i < NUM_CLIENT+NUM_SERVER;i++){

		wait(&status);

		if (WIFEXITED(status)) {
    			printf("[PADRE] - Figlio terminato con stato %d\n",status);
  		}
	}

	msgctl(coda,IPC_RMID,0);

	printf("[PADRE] - Fine elaborazione...\n");

	return 0;	
}