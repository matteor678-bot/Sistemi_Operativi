#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    /* Supponiamo di voler creare 5 processi -> 1°Soluzione:
    for(int i = 0; i<5; i++) {
        fork();
    }
    
    PROBLEMA: In questo modo creerò 32 processi: 31 filgi + 1 padre */

    for(int i=0; i<5; i++) {
        int sono_il_padre = fork();
        if(!sono_il_padre) {
            printf("Sono il processo figlio che uscirà dal ciclo.\n");
            i=5;   
        }
    }
} 