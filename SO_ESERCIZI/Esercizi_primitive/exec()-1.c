#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;
    switch(pid = fork()) {
        case -1: printf("ERRORE: La fork è fallita!");
                 break;
        case 0: // Qui sono dentro il figlio
                execl("/bin/ls", "ls" ,"-l", (char *) 0);
                printf("ERRORE: exec failed"); //Di norma non eseguito 
                break;
        default: 
                wait((int *)0); //Passare come argomento (int *)0 è l'equivalente di passare NULL, indicando al Kernel che al padre non interessa memorizzare lo stato di uscita del figlio.
                printf("Il comando ls è stato completato\n");
                exit(0);
    }
}